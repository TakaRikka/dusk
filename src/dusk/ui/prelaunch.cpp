#include "prelaunch.hpp"

#include "dusk/app_info.hpp"
#include "dusk/config.hpp"
#include "dusk/data.hpp"
#include "dusk/game_mode.hpp"
#include "dusk/iso_validate.hpp"
#include "dusk/language.hpp"
#include "dusk/main.h"
#include "dusk/settings.h"
#include "dusk/ui/menu_bar.hpp"
#include "modal.hpp"
#include "mods_window.hpp"
#include "preset.hpp"
#include "settings.hpp"

#include <SDL3/SDL_error.h>
#include <SDL3/SDL_misc.h>
#include <aurora/lib/window.hpp>
#include <borealis/file_select.hpp>
#include <borealis/log.hpp>
#include <borealis/update.hpp>
#include <borealis/version.h>
#include <fmt/format.h>

#include <algorithm>
#include <array>
#include <atomic>
#include <exception>
#include <filesystem>
#include <optional>
#include <thread>

#include "m_Do/m_Do_MemCard.h"

namespace dusk::ui {
namespace {
constexpr borealis::Log PrelaunchLog{"dusk::ui::prelaunch"};

PrelaunchState sPrelaunchState;

const Rml::String kDocumentSource = R"RML(
<rml>
<head>
    <link type="text/rcss" href="res/rml/prelaunch.rcss" />
</head>
<body>
    <div class="gradient" />
    <div class="background" />
    <content id="root" open>
        <menu>
            <hero class="intro-item delay-0">
                <eyebrow><span>Twilit Realm</span> presents</eyebrow>
                <img src="res/logo.png" />
            </hero>
            <div id="menu-list" />
        </menu>
        <disc-info class="intro-item delay-5">
            <div id="disc-status">
                <icon />
                <span id="disc-status-label" />
            </div>
            <span id="disc-version" class="detail" />
        </disc-info>
        <version-info class="intro-item delay-6">
            <div class="version">Version <span id="version-text"></span></div>
            <div id="update-status" class="update">
                <span id="update-message"></span>
                <button id="update-download">
                    <span id="update-download-label"></span>
                    &nbsp;<icon />
                </button>
            </div>
        </version-info>
    </content>
</body>
</rml>
)RML";

const std::vector<borealis::file_select::Filter> kDiscFileFilters{
    {"Game Disc Images", "iso;gcm;ciso;gcz;nfs;rvz;wbfs;wia;tgc"},
    {"All Files", "*"},
};

struct DiscVerificationResult {
    std::string path;
    iso::DiscInfo info;
    iso::ValidationError validation = iso::ValidationError::Unknown;
};

struct DiscVerificationTask {
    explicit DiscVerificationTask(std::string discPath) : path(std::move(discPath)) {
        worker = std::thread([this] {
            try {
                validation = iso::validate(path.c_str(), status, info);
            } catch (const std::exception& e) {
                PrelaunchLog.error(
                    "Disc verification failed with exception for '{}': {}", path, e.what());
                validation = iso::ValidationError::Unknown;
            } catch (...) {
                PrelaunchLog.error(
                    "Disc verification failed with unknown exception for '{}'", path);
                validation = iso::ValidationError::Unknown;
            }
            done.store(true, std::memory_order_release);
        });
    }

    ~DiscVerificationTask() {
        status.cancelRequested.store(true, std::memory_order_relaxed);
        join();
    }

    void join() {
        if (worker.joinable()) {
            worker.join();
        }
    }

    [[nodiscard]] bool finished() const { return done.load(std::memory_order_acquire); }

    std::string path;
    iso::DiscInfo info;
    iso::VerificationStatus status;
    iso::ValidationError validation = iso::ValidationError::Unknown;
    std::atomic_bool done = false;
    std::thread worker;
};

std::unique_ptr<DiscVerificationTask> sDiscVerificationTask;
bool sDiscVerificationModalPushed = false;

struct UpdateCheckTask {
    UpdateCheckTask() {
        worker = std::thread([this] {
            try {
                result = borealis::update::check_latest_github_release(AppInfo);
            } catch (const std::exception& e) {
                result = {
                    .status = borealis::update::Status::Failed,
                    .message = fmt::format("Update check failed with exception: {}", e.what()),
                };
            } catch (...) {
                result = {
                    .status = borealis::update::Status::Failed,
                    .message = "Update check failed with an unknown exception",
                };
            }
            done.store(true, std::memory_order_release);
        });
    }

    ~UpdateCheckTask() { join(); }

    void join() {
        if (worker.joinable()) {
            worker.join();
        }
    }

    [[nodiscard]] bool finished() const { return done.load(std::memory_order_acquire); }

    borealis::update::Result result;
    std::atomic_bool done = false;
    std::thread worker;
};

std::unique_ptr<UpdateCheckTask> sUpdateCheckTask;
std::optional<borealis::update::Result> sUpdateCheckResult;

bool verification_state_allows_launch(iso::ValidationError validation) noexcept {
    return validation == iso::ValidationError::Unknown ||
           validation == iso::ValidationError::Success ||
           validation == iso::ValidationError::HashMismatch;
}

iso::ValidationError verification_from_config(DiscVerificationState value) noexcept {
    switch (value) {
    case DiscVerificationState::Success:
        return iso::ValidationError::Success;
    case DiscVerificationState::HashMismatch:
        return iso::ValidationError::HashMismatch;
    default:
        return iso::ValidationError::Unknown;
    }
}

DiscVerificationState verification_to_config(iso::ValidationError validation) {
    switch (validation) {
    case iso::ValidationError::Success:
        return DiscVerificationState::Success;
    case iso::ValidationError::HashMismatch:
        return DiscVerificationState::HashMismatch;
    default:
        return DiscVerificationState::Unknown;
    }
}

std::string format_bytes(std::size_t bytes) {
    constexpr double KiB = 1024.0;
    constexpr double MiB = KiB * 1024.0;
    constexpr double GiB = MiB * 1024.0;
    if (bytes >= static_cast<std::size_t>(GiB)) {
        return fmt::format("{:.2f} GiB", static_cast<double>(bytes) / GiB);
    }
    if (bytes >= static_cast<std::size_t>(MiB)) {
        return fmt::format("{:.0f} MiB", static_cast<double>(bytes) / MiB);
    }
    if (bytes >= static_cast<std::size_t>(KiB)) {
        return fmt::format("{:.0f} KiB", static_cast<double>(bytes) / KiB);
    }
    return fmt::format("{} B", bytes);
}

void begin_disc_verification(std::string path) noexcept {
    if (path.empty()) {
        return;
    }
    if (sDiscVerificationTask != nullptr) {
        sDiscVerificationTask->status.cancelRequested.store(true, std::memory_order_relaxed);
        sDiscVerificationTask.reset();
    }
    sDiscVerificationTask = std::make_unique<DiscVerificationTask>(std::move(path));
    sDiscVerificationModalPushed = false;
}

std::optional<DiscVerificationResult> take_finished_disc_verification() {
    if (sDiscVerificationTask == nullptr || !sDiscVerificationTask->finished()) {
        return std::nullopt;
    }
    DiscVerificationResult result{
        .path = sDiscVerificationTask->path,
        .info = sDiscVerificationTask->info,
        .validation = sDiscVerificationTask->validation,
    };
    sDiscVerificationTask->join();
    sDiscVerificationTask.reset();
    sDiscVerificationModalPushed = false;
    return result;
}

void begin_update_check() {
    if (!getSettings().backend.checkForUpdates.getValue()) {
        return;
    }
    if (sUpdateCheckTask != nullptr || sUpdateCheckResult.has_value()) {
        return;
    }
    sUpdateCheckTask = std::make_unique<UpdateCheckTask>();
}

std::optional<borealis::update::Result> take_finished_update_check() {
    if (sUpdateCheckTask == nullptr || !sUpdateCheckTask->finished()) {
        return std::nullopt;
    }

    sUpdateCheckTask->join();
    auto result = std::move(sUpdateCheckTask->result);
    sUpdateCheckTask.reset();
    return result;
}

std::string update_release_label(const borealis::update::Release& release) {
    std::string_view tagName = release.tagName;
    if (!tagName.empty() && tagName.front() == 'v') {
        tagName.remove_prefix(1);
    }
    return std::string(tagName);
}

void open_update_release() {
    if (!sUpdateCheckResult.has_value() ||
        sUpdateCheckResult->status != borealis::update::Status::UpdateAvailable)
    {
        return;
    }

    const std::string url = sUpdateCheckResult->latest.htmlUrl;
    if (url.empty()) {
        PrelaunchLog.warn("Update is available, but the release did not include a download URL");
        return;
    }
    if (!SDL_OpenURL(url.c_str())) {
        PrelaunchLog.warn("Failed to open update URL '{}': {}", url, SDL_GetError());
    }
}

std::string get_error_msg(iso::ValidationError error) {
    switch (error) {
    default:
        return "The selected disc image could not be validated.";
    case iso::ValidationError::IOError:
        return "Unable to read the selected file.";
    case iso::ValidationError::InvalidImage:
        return "The selected file is not a valid disc image.";
    case iso::ValidationError::WrongGame:
        return "The selected game is not supported by Dusklight.";
    case iso::ValidationError::WrongVersion:
        return "Dusklight does not currently support the Wii's Korean version.";
    case iso::ValidationError::Canceled:
        return "Disc verification was canceled. Dusklight cannot guarantee the selected disc "
               "image is compatible.";
    case iso::ValidationError::HashMismatch:
        return "The selected disc image did not pass hash verification. It may be corrupt or "
               "modified.";
    case iso::ValidationError::Success:
        return "The selected disc image is valid.";
    }
}

void persist_disc_choice(const std::string& path, iso::ValidationError validation) {
    const auto previousPath = getSettings().backend.isoPath.getValue();
    const auto previousVerification = getSettings().backend.isoVerification.getValue();
    const auto verification = verification_to_config(validation);

    getSettings().backend.isoPath.setValue(path);
    getSettings().backend.isoVerification.setValue(verification);
    config::save();

    if (previousPath != path || previousVerification != verification) {
        iso::log_verification_state(path, verification);
    }
}

void apply_language_for_disc(const iso::DiscInfo& info) {
    const auto langs = language::available_languages(info);
    auto& language = getSettings().game.language;
    const GameLanguage previous = language.getValue();
    if (std::ranges::find(langs, previous) != langs.end()) {
        return;
    }

    const GameLanguage fallback = langs.front();
    language.setValue(fallback);
    config::save();

    auto& state = prelaunch_state();
    state.initialLanguage = fallback;
    state.unavailableLanguage = previous;
    state.pendingLanguageUnavailableNotice = true;
}

void apply_valid_disc_result(
    const std::string& path, const iso::DiscInfo& info, iso::ValidationError validation) {
    auto& state = prelaunch_state();
    state.configuredDiscPath = path;
    state.configuredDiscCanLaunch = true;
    state.configuredDiscInfo = info;
    state.configuredDiscValidation = validation;
    if (state.activeDiscPath.empty() || path == state.activeDiscPath) {
        state.activeDiscPath = path;
        state.activeDiscInfo = info;
    }
    persist_disc_choice(path, validation);
    apply_language_for_disc(info);
}

void apply_disc_verification_result(const DiscVerificationResult& result) {
    auto& state = prelaunch_state();

    if (result.validation == iso::ValidationError::HashMismatch ||
        result.validation == iso::ValidationError::Canceled)
    {
        state.pendingDiscPath = result.path;
        state.pendingDiscInfo = result.info;
        state.pendingDiscValidation = result.validation;
        state.errorString = escape(get_error_msg(result.validation));
        return;
    }

    if (result.validation == iso::ValidationError::Success) {
        apply_valid_disc_result(result.path, result.info, result.validation);
        state.errorString.clear();
        state.pendingDiscPath.clear();
        state.pendingDiscInfo = {};
        state.pendingDiscValidation = iso::ValidationError::Unknown;
        return;
    }

    state.pendingDiscPath.clear();
    state.pendingDiscInfo = {};
    state.pendingDiscValidation = iso::ValidationError::Unknown;
    state.errorString = escape(get_error_msg(result.validation));
}

class DiscVerificationModal : public WindowSmall {
public:
    DiscVerificationModal() : WindowSmall("modal", "modal-dialog") {
        auto* header = append(mDialog, "div");
        header->SetClass("modal-header", true);

        auto* title = append(header, "div");
        title->SetClass("modal-title", true);
        title->SetInnerRML("Verifying disc image");

        auto* icon = append(header, "icon");
        icon->SetClass("verifying", true);

        auto* body = append(mDialog, "div");
        body->SetClass("modal-body", true);

        auto* content = append(body, "div");
        content->SetClass("verification-progress", true);

        mFileName = append(content, "div");
        mFileName->SetClass("verification-file", true);

        mProgress = append(content, "progress");
        mProgress->SetClass("progress-ongoing", true);
        mProgress->SetClass("verification-progress-bar", true);
        mProgress->SetAttribute("value", 0.f);

        mDetail = append(content, "div");
        mDetail->SetClass("verification-detail", true);

        auto* actions = append(mDialog, "div");
        actions->SetClass("modal-actions", true);
        mCancelButton = std::make_unique<Button>(actions, "Cancel");
        mCancelButton->root()->SetClass("modal-btn", true);
        mCancelButton->on_pressed([this] { request_cancel(); });

        refresh();
    }

    void update() override {
        if (mFinished) {
            return;
        }
        if (auto result = take_finished_disc_verification()) {
            mFinished = true;
            apply_disc_verification_result(*result);
            pop();
            return;
        }
        if (sDiscVerificationTask == nullptr) {
            mFinished = true;
            pop();
            return;
        }
        refresh();
    }

    bool focus() override { return mCancelButton != nullptr && mCancelButton->focus(); }

protected:
    bool handle_nav_command(Rml::Event& event, NavCommand cmd) override {
        if (cmd == NavCommand::Cancel || cmd == NavCommand::Menu) {
            request_cancel();
            event.StopPropagation();
            return true;
        }
        if (cmd == NavCommand::Left || cmd == NavCommand::Right) {
            return true;
        }
        return false;
    }

private:
    void request_cancel() {
        if (sDiscVerificationTask == nullptr || mCancelRequested) {
            return;
        }

        mCancelRequested = true;
        sDiscVerificationTask->status.cancelRequested.store(true, std::memory_order_relaxed);
        if (mCancelButton != nullptr) {
            mCancelButton->set_text("Cancelling...");
            mCancelButton->set_disabled(true);
        }
    }

    void refresh() {
        if (sDiscVerificationTask == nullptr) {
            return;
        }

        if (mCancelRequested) {
            return;
        }

        if (mFileName != nullptr) {
            std::string fileName = borealis::file_select::display_name(sDiscVerificationTask->path);
            if (fileName.empty()) {
                fileName = sDiscVerificationTask->path;
            }
            mFileName->SetInnerRML(escape(fileName));
        }

        const std::size_t bytesRead =
            sDiscVerificationTask->status.bytesRead.load(std::memory_order_relaxed);
        const std::size_t bytesTotal =
            sDiscVerificationTask->status.bytesTotal.load(std::memory_order_relaxed);

        if (bytesTotal == 0) {
            if (mProgress != nullptr) {
                mProgress->SetAttribute("value", 0.f);
            }
            if (mDetail != nullptr) {
                mDetail->SetInnerRML("Opening disc image...");
            }
            return;
        }

        const float fraction =
            std::clamp(static_cast<float>(bytesRead) / static_cast<float>(bytesTotal), 0.0f, 1.0f);
        if (mProgress != nullptr) {
            mProgress->SetAttribute("value", fraction);
        }
        if (mDetail != nullptr) {
            mDetail->SetInnerRML(escape(fmt::format("{} / {} ({:.0f}%)", format_bytes(bytesRead),
                format_bytes(bytesTotal), fraction * 100.0f)));
        }
    }

    Rml::Element* mFileName = nullptr;
    Rml::Element* mProgress = nullptr;
    Rml::Element* mDetail = nullptr;
    std::unique_ptr<Button> mCancelButton;
    bool mCancelRequested = false;
    bool mFinished = false;
};

void file_dialog_callback(borealis::file_select::Result result) {
    if (result.status != borealis::file_select::Status::Selected || result.locations.empty()) {
        if (result.status == borealis::file_select::Status::Failed) {
            PrelaunchLog.warn("File selection failed: {}", result.message);
        }
        return;
    }

    begin_disc_verification(result.locations.front());
}

std::vector<const gamemode::GameMode*> carousel_game_modes() {
    const auto& registered = gamemode::getGameModeManager().getRegisteredGameModes();
    std::vector<const gamemode::GameMode*> modes;
    modes.reserve(registered.size());

    if (const auto vanilla = registered.find(gamemode::kVanillaGameModeId);
        vanilla != registered.end())
    {
        modes.push_back(&vanilla->second);
    }
    for (const auto& [id, mode] : registered) {
        if (id != gamemode::kVanillaGameModeId) {
            modes.push_back(&mode);
        }
    }
    std::ranges::sort(modes.begin() + std::min<size_t>(1, modes.size()), modes.end(),
        [](const auto* lhs, const auto* rhs) { return lhs->getFullName() < rhs->getFullName(); });
    return modes;
}

std::string game_mode_button_text() {
    if (prelaunch_state().activeDiscPath.empty()) {
        return "Select Disc Image";
    }
    const auto* currentGameMode = gamemode::getGameModeManager().getCurrentGameMode();
    if (currentGameMode == nullptr || currentGameMode->getId() == gamemode::kVanillaGameModeId) {
        return "Play";
    }
    return currentGameMode->getFullName();
}

class GameModeButton final : public Button {
public:
    GameModeButton(Rml::Element* parent, ButtonCallback onPressed) : Button{parent, ""} {
        root()->SetClass("game-mode-button", true);
        mPrevious = append(root(), "game-mode-previous");
        mLabelViewport = append(root(), "game-mode-label-viewport");
        for (auto& label : mLabels) {
            label = append(mLabelViewport, "game-mode-label");
        }
        mNext = append(root(), "game-mode-next");

        Component::listen(mPrevious, Rml::EventId::Click, [this](Rml::Event& event) {
            cycle(-1);
            event.StopPropagation();
        });
        Component::listen(mNext, Rml::EventId::Click, [this](Rml::Event& event) {
            cycle(1);
            event.StopPropagation();
        });
        Component::listen(root(), Rml::EventId::Keydown, [this](Rml::Event& event) {
            const auto command = map_nav_event(event);
            if (command == NavCommand::Left || command == NavCommand::Right) {
                cycle(command == NavCommand::Left ? -1 : 1);
                event.StopPropagation();
            }
        });
        on_pressed(std::move(onPressed));
        refresh(0);
    }

    void update() override {
        refresh(0);
        Button::update();
    }

private:
    bool can_cycle() const {
        return !prelaunch_state().activeDiscPath.empty() &&
               gamemode::getGameModeManager().getRegisteredGameModes().size() > 1;
    }

    void cycle(int direction) {
        const auto modes = carousel_game_modes();
        if (!can_cycle() || modes.empty()) {
            return;
        }

        const auto* current = gamemode::getGameModeManager().getCurrentGameMode();
        const auto currentIt = std::ranges::find_if(modes, [current](const auto* mode) {
            return current != nullptr && mode->getId() == current->getId();
        });
        const int currentIndex =
            currentIt == modes.end() ? 0 : static_cast<int>(currentIt - modes.begin());
        const int count = static_cast<int>(modes.size());
        const int nextIndex = ((currentIndex + direction) % count + count) % count;
        const auto nextId = modes[nextIndex]->getId();
        if (gamemode::getGameModeManager().setCurrentGameMode(nextId)) {
            mDoAud_seStartMenu(kSoundItemChange);
        }
        refresh(direction);
    }

    void refresh(int direction) {
        root()->SetClass("can-cycle", can_cycle());

        const auto text = game_mode_button_text();
        if (text == mText) {
            return;
        }
        mText = text;
        if (direction == 0) {
            mLabels[mActiveLabel]->SetInnerRML(escape(text));
            mLabels[mActiveLabel]->SetProperty(
                Rml::PropertyId::Left, Rml::Property{0.0f, Rml::Unit::PERCENT});
            mLabels[mActiveLabel]->SetClass("active", true);
            mLabels[1 - mActiveLabel]->SetClass("active", false);
            return;
        }

        constexpr float kSlideDistance = 100.0f;
        constexpr float kSlideDuration = 0.24f;
        auto* outgoing = mLabels[mActiveLabel];
        mActiveLabel = 1 - mActiveLabel;
        auto* incoming = mLabels[mActiveLabel];
        incoming->SetInnerRML(escape(text));

        const Rml::Property incomingOffset{
            static_cast<float>(direction) * kSlideDistance, Rml::Unit::PERCENT};
        const Rml::Property outgoingOffset{
            static_cast<float>(-direction) * kSlideDistance, Rml::Unit::PERCENT};

        outgoing->Animate(Rml::PropertyId::Left, outgoingOffset, kSlideDuration,
            Rml::Tween{Rml::Tween::Cubic, Rml::Tween::InOut});
        incoming->Animate(Rml::PropertyId::Left, Rml::Property{0.0f, Rml::Unit::PERCENT},
            kSlideDuration, Rml::Tween{Rml::Tween::Cubic, Rml::Tween::InOut}, 1, false, 0.0f,
            &incomingOffset);
        outgoing->SetClass("active", false);
        incoming->SetClass("active", true);
    }

    Rml::Element* mPrevious = nullptr;
    Rml::Element* mLabelViewport = nullptr;
    std::array<Rml::Element*, 2> mLabels{};
    Rml::Element* mNext = nullptr;
    Rml::String mText;
    std::size_t mActiveLabel = 0;
};

}  // namespace

PrelaunchState& prelaunch_state() noexcept {
    return sPrelaunchState;
}

void refresh_configured_disc_state() noexcept {
    auto& state = prelaunch_state();
    if (state.configuredDiscPath.empty()) {
        state.configuredDiscCanLaunch = false;
        state.configuredDiscInfo = {};
        state.configuredDiscValidation = iso::ValidationError::Unknown;
        return;
    }

    iso::DiscInfo info{};
    const auto metadataValidation = iso::inspect(state.configuredDiscPath.c_str(), info);
    if (metadataValidation != iso::ValidationError::Success) {
        state.configuredDiscCanLaunch = false;
        state.configuredDiscInfo = {};
        state.configuredDiscValidation = metadataValidation;
        if (state.configuredDiscPath == state.activeDiscPath) {
            state.activeDiscInfo = {};
        }
        return;
    }

    auto verification = iso::ValidationError::Unknown;
    if (state.configuredDiscPath == getSettings().backend.isoPath.getValue()) {
        verification = verification_from_config(getSettings().backend.isoVerification.getValue());
    }

    if (verification_state_allows_launch(verification)) {
        state.configuredDiscCanLaunch = true;
        state.configuredDiscInfo = info;
        state.configuredDiscValidation = verification;
        if (state.configuredDiscPath == state.activeDiscPath) {
            state.activeDiscInfo = info;
        }
        apply_language_for_disc(info);
        return;
    }

    state.configuredDiscCanLaunch = false;
    state.configuredDiscInfo = {};
    state.configuredDiscValidation = iso::ValidationError::Unknown;
    if (state.configuredDiscPath == state.activeDiscPath) {
        state.activeDiscInfo = {};
    }
}

void try_push_verification_modal(Document& host) {
    auto& state = prelaunch_state();
    if (sDiscVerificationTask != nullptr && !sDiscVerificationModalPushed) {
        sDiscVerificationModalPushed = true;
        host.push(std::make_unique<DiscVerificationModal>());
        return;
    }

    if (state.errorString.empty()) {
        return;
    }

    auto dismiss = [](Modal& modal) {
        auto& state = prelaunch_state();
        state.errorString.clear();
        state.pendingDiscPath.clear();
        state.pendingDiscInfo = {};
        state.pendingDiscValidation = iso::ValidationError::Unknown;
        modal.pop();
    };

    if (!state.pendingDiscPath.empty()) {
        const Rml::String bodyRml =
            state.errorString + "<br/><br/>You may proceed at your own risk.";
        auto acceptHashMismatch = [](Modal& modal) {
            auto& st = prelaunch_state();
            std::string path = std::move(st.pendingDiscPath);
            const auto info = st.pendingDiscInfo;
            const auto validation = st.pendingDiscValidation;
            st.pendingDiscPath.clear();
            st.pendingDiscInfo = {};
            st.pendingDiscValidation = iso::ValidationError::Unknown;
            st.errorString.clear();
            apply_valid_disc_result(path, info, validation);
            refresh_configured_disc_state();
            modal.pop();
        };
        host.push(std::make_unique<Modal>(Modal::Props{
            .title = "Disc verification warning",
            .bodyRml = bodyRml,
            .actions =
                {
                    ModalAction{
                        .label = "Cancel",
                        .onPressed = dismiss,
                    },
                    ModalAction{
                        .label = "Continue anyway",
                        .onPressed = acceptHashMismatch,
                    },
                },
            .onDismiss = dismiss,
            .variant = "danger",
            .icon = "warning",
        }));
        return;
    }

    host.push(std::make_unique<Modal>(Modal::Props{
        .title = "Disc verification error",
        .bodyRml = state.errorString,
        .actions =
            {
                ModalAction{
                    .label = "OK",
                    .onPressed = dismiss,
                },
            },
        .onDismiss = dismiss,
        .icon = "error",
    }));
}

void try_push_language_unavailable_modal(Document& host) {
    auto& state = prelaunch_state();

    if (!state.pendingLanguageUnavailableNotice) {
        return;
    }
    state.pendingLanguageUnavailableNotice = false;

    const Rml::String bodyRml = fmt::format(
        "<b>{}</b> is not available on this disc. Language has been reset to <b>{}</b>.",
        language::language_name(state.unavailableLanguage),
        language::language_name(getSettings().game.language.getValue()));

    auto dismiss = [](Modal& modal) { modal.pop(); };

    host.push(std::make_unique<Modal>(Modal::Props{
        .title = "Language unavailable",
        .bodyRml = bodyRml,
        .actions =
            {
                ModalAction{
                    .label = "OK",
                    .onPressed = dismiss,
                },
            },
        .onDismiss = dismiss,
        .icon = "warning",
    }));
}

void ensure_initialized() noexcept {
    auto& state = prelaunch_state();
    if (state.initialized) {
        return;
    }

    state.configuredDiscPath = getSettings().backend.isoPath;
    state.activeDiscPath = state.configuredDiscPath;
    state.configuredDiscValidation =
        verification_from_config(getSettings().backend.isoVerification.getValue());
    state.initialLanguage = getSettings().game.language;
    state.initialGraphicsBackend = getSettings().backend.graphicsBackend;
    state.initialCardFileType = getSettings().backend.cardFileType;
    state.errorString.clear();
    state.initialized = true;
    refresh_configured_disc_state();
}

void open_iso_picker() noexcept {
    ensure_initialized();
    borealis::file_select::open_file(
        {
            .parentWindow = aurora::window::get_sdl_window(),
            .filters = kDiscFileFilters,
        },
        &file_dialog_callback);
}

bool is_restart_pending() noexcept {
    const auto& state = prelaunch_state();
    if (!state.activeDiscPath.empty() && state.configuredDiscPath != state.activeDiscPath) {
        return true;
    }
    if (data::is_data_path_restart_pending()) {
        return true;
    }
    if (getSettings().backend.graphicsBackend.getValue() != state.initialGraphicsBackend) {
        return true;
    }
    if (getSettings().game.language.getValue() != state.initialLanguage) {
        return true;
    }
    return false;
}

void apply_intro_animation(Rml::Element* element, const char* delay_class) {
    if (element == nullptr || delay_class == nullptr) {
        return;
    }
    element->SetClass("intro-item", true);
    element->SetClass(delay_class, true);
}

void try_apply_mirrored_layout(Rml::Element* body) {
    if (body == nullptr) {
        return;
    }
    body->SetClass("mirrored", getSettings().game.enableMirrorMode.getValue());
}

void Prelaunch::refresh_menu_buttons() {
    auto* prelaunch = static_cast<Prelaunch*>(find_document(DocumentScope::Prelaunch));
    if (prelaunch == nullptr) {
        return;
    }
    auto* menuList = prelaunch->mDocument->GetElementById("menu-list");
    while (menuList->GetNumChildren() > 0) {
        menuList->RemoveChild(menuList->GetChild(0));
    }
    prelaunch->mMenuButtons.clear();
    prelaunch->build_menu_buttons();
}

Prelaunch::Prelaunch() : Document(kDocumentSource, false, DocumentScope::Prelaunch) {
    mRoot = mDocument->GetElementById("root");
    ensure_initialized();
    begin_update_check();

    build_menu_buttons();

    mDiscStatus = mDocument->GetElementById("disc-status");
    mDiscDetail = mDocument->GetElementById("disc-version");
    mVersion = mDocument->GetElementById("version-text");
    mUpdateStatus = mDocument->GetElementById("update-status");
    mUpdateMessage = mDocument->GetElementById("update-message");
    mUpdateDownload = mDocument->GetElementById("update-download");
    mUpdateDownloadLabel = mDocument->GetElementById("update-download-label");

    if (mUpdateDownload != nullptr) {
        listen(mUpdateDownload, Rml::EventId::Click, [](Rml::Event& event) {
            open_update_release();
            event.StopPropagation();
        });
        listen(mUpdateDownload, Rml::EventId::Keydown, [](Rml::Event& event) {
            if (map_nav_event(event) == NavCommand::Confirm) {
                open_update_release();
                event.StopPropagation();
            }
        });
    }

    try_apply_mirrored_layout(mDocument);

    listen(mDocument, Rml::EventId::Transitionend, [this](Rml::Event& event) {
        auto* target = event.GetTargetElement();
        if (target == nullptr) {
            return;
        }
        if (target == mDocument && !mDocument->HasAttribute("open")) {
            Document::hide(true);
        } else if (target->GetTagName() == "button" && !target->IsClassSet("anim-done")) {
            target->SetClass("anim-done", true);
        }
    });
}

void Prelaunch::build_menu_buttons() {
    if (auto* menuList = mDocument->GetElementById("menu-list")) {
        // Restore the previously selected game mode before creating the play control.
        gamemode::getGameModeManager().setGameModeToPrevious();

        auto playButton = std::make_unique<GameModeButton>(menuList, [this] {
            if (prelaunch_state().activeDiscPath.empty()) {
                open_iso_picker();
                return;
            }

            if (const auto* gameMode = gamemode::getGameModeManager().getCurrentGameMode();
                gameMode != nullptr && !gameMode->invokeOnPlayFunction())
            {
                gamemode::getGameModeManager().setCurrentGameMode(gamemode::kVanillaGameModeId);
                return;
            }

            mDoAud_seStartMenu(kSoundPlay);
            show_menu_notification();

            if (getSettings().audio.menuSounds) {
                JAISoundHandle* handle = g_mEnvSeMgr.field_0x144.getHandle();
                if (*handle) {
                    (*handle)->stop(60);
                    (*handle)->releaseHandle();
                }
            }

            if (g_mDoMemCd_control.mCardCommand == mDoMemCd_Ctrl_c::Command_e::COMM_NONE_e) {
                mDoMemCd_ThdInit();
            }

            prelaunch_state().firstLaunch = false;
            IsGameLaunched = true;
            hide(true);
            MenuBar::refresh_tabs();
        });
        apply_intro_animation(playButton->root(), "delay-1");
        mMenuButtons.push_back(std::move(playButton));

        mMenuButtons.push_back(std::make_unique<Button>(menuList, "Settings"));
        mMenuButtons.back()->on_pressed([this] {
            mRestartSuppressed = false;
            bool showPrelaunchSettings = prelaunch_state().firstLaunch;
            push(std::make_unique<SettingsWindow>(showPrelaunchSettings));
        });
        apply_intro_animation(mMenuButtons.back()->root(), "delay-2");

        mMenuButtons.push_back(std::make_unique<Button>(menuList, "Mods"));
        mMenuButtons.back()->on_pressed([this] {
            mRestartSuppressed = false;
            push(std::make_unique<ModsWindow>());
        });
        apply_intro_animation(mMenuButtons.back()->root(), "delay-3");

        mMenuButtons.push_back(std::make_unique<Button>(menuList, "Quit"));
        mMenuButtons.back()->on_pressed([] { IsRunning = false; });
        apply_intro_animation(mMenuButtons.back()->root(), "delay-4");
    }
}

void Prelaunch::show() {
    Document::show();
    mDocument->SetAttribute("open", "");
    mRoot->SetAttribute("open", "");

    if (is_restart_pending() && !mRestartSuppressed) {
        const auto dismiss = [this](Modal& modal) {
            mRestartSuppressed = true;
            modal.pop();
        };
        std::vector<ModalAction> actions;
        if constexpr (SupportsProcessRestart) {
            actions.push_back(ModalAction{
                .label = "Restart later",
                .onPressed = dismiss,
            });
            actions.push_back(ModalAction{
                .label = "Restart now",
                .onPressed = [](Modal&) { RequestRestart(); },
            });
        } else {
            actions.push_back(ModalAction{
                .label = "OK",
                .onPressed = dismiss,
            });
        }
        push(std::make_unique<Modal>(Modal::Props{
            .title = "Apply Options",
            .bodyRml =
                SupportsProcessRestart ?
                    "A restart is required to apply selected options.<br/><br/>Restart now to "
                    "apply them immediately?" :
                    "A restart is required to apply selected options.<br/><br/>Close and reopen "
                    "Dusklight to apply them.",
            .actions = std::move(actions),
            .onDismiss = dismiss,
        }));
    }
}

void Prelaunch::hide(bool close) {
    if (close) {
        if (!mEntranceAnimationStarted) {
            // Close document immediately
            Document::hide(true);
        } else {
            mPendingClose = true;
        }
        mDocument->RemoveAttribute("open");
    } else {
        mRoot->RemoveAttribute("open");
    }
}

void Prelaunch::update() {
    ensure_initialized();
    try_apply_mirrored_layout(mDocument);

    if (top_document() == this) {
        try_push_verification_modal(*this);
        try_push_language_unavailable_modal(*this);
    }

    const auto& state = prelaunch_state();

    const bool canLaunchConfiguredDisc = state.configuredDiscCanLaunch;
    const bool activeDiscLoaded = !state.activeDiscPath.empty();
    const bool discRestartPending =
        activeDiscLoaded && state.configuredDiscPath != state.activeDiscPath;
    mDocument->SetClass("disc-ready", IsGameLaunched);
    if (canLaunchConfiguredDisc) {
        IsGameLaunched = true;
    }

    if (!mEntranceAnimationStarted && mDocument != nullptr) {
        mDocument->SetClass("animate-in", true);
        mEntranceAnimationStarted = true;
    }

    for (const auto& button : mMenuButtons) {
        button->update();
    }

    const auto discStatusLabel = mDiscStatus->GetElementById("disc-status-label");

    if (mDiscStatus != nullptr && discStatusLabel != nullptr) {
        if (!activeDiscLoaded) {
            mDiscStatus->RemoveAttribute("status");
            discStatusLabel->SetInnerRML("No disc image found.");
        } else if (discRestartPending) {
            mDiscStatus->SetAttribute("status", "pending");
            discStatusLabel->SetInnerRML("Pending restart.");
        } else if (state.configuredDiscValidation == iso::ValidationError::Success) {
            mDiscStatus->SetAttribute("status", "good");
            discStatusLabel->SetInnerRML("Disc ready.");
        } else if (state.configuredDiscValidation == iso::ValidationError::HashMismatch) {
            mDiscStatus->SetAttribute("status", "mismatch");
            discStatusLabel->SetInnerRML("Disc hash mismatch.");
        } else if (canLaunchConfiguredDisc) {
            mDiscStatus->SetAttribute("status", "unknown");
            discStatusLabel->SetInnerRML("Disc not verified.");
        } else {
            mDiscStatus->SetAttribute("status", "bad");
            discStatusLabel->SetInnerRML("Disc unavailable.");
        }
    }
    if (mDiscDetail != nullptr) {
        if (activeDiscLoaded) {
            mDiscDetail->SetProperty(Rml::PropertyId::Display, Rml::Style::Display::Block);
            Rml::String innerRML = "";

            switch (state.activeDiscInfo.platform) {
            case iso::Platform::Unknown:
                innerRML += "Unknown";
                break;
            case iso::Platform::GameCube:
                innerRML += "GameCube";
                break;
            case iso::Platform::Wii:
                innerRML += "Wii";
                break;
            }

            innerRML += " • ";

            switch (state.activeDiscInfo.region) {
            case iso::Region::Japan:
                innerRML += "JPN";
                break;
            case iso::Region::Europe:
                innerRML += "EUR";
                break;
            case iso::Region::NorthAmerica:
                innerRML += "USA";
                if (state.activeDiscInfo.platform == iso::Platform::Wii) {
                    innerRML += fmt::format(" Rev. {}", state.activeDiscInfo.revision);
                }
                break;
            case iso::Region::Korea:
                innerRML += "KOR";
                break;
            default:
                innerRML += "Unknown";
                break;
            }
            mDiscDetail->SetInnerRML(innerRML);
        } else {
            mDiscDetail->SetProperty(Rml::PropertyId::Display, Rml::Style::Display::None);
        }
    }
    if (mVersion != nullptr) {
        std::string_view versionStr(BOREALIS_APP_DESCRIBE);
        if (versionStr[0] == 'v') {
            versionStr = versionStr.substr(1);
        }
        mVersion->SetInnerRML(escape(versionStr));
    }
    if (mUpdateStatus != nullptr && mUpdateMessage != nullptr) {
        if (auto result = take_finished_update_check()) {
            if (result->status == borealis::update::Status::Failed) {
                PrelaunchLog.error("Failed to check for updates: {}", result->message);
            }
            sUpdateCheckResult = std::move(*result);
        }

        if (sUpdateCheckTask != nullptr) {
            mUpdateStatus->SetAttribute("state", "checking");
            mUpdateMessage->SetInnerRML("Checking for updates...");
        } else if (!sUpdateCheckResult.has_value() ||
                   sUpdateCheckResult->status == borealis::update::Status::UpToDate)
        {
            mUpdateStatus->RemoveAttribute("state");
            mUpdateMessage->SetInnerRML("");
        } else if (sUpdateCheckResult->status == borealis::update::Status::UpdateAvailable) {
            mUpdateStatus->SetAttribute("state", "available");
            mUpdateMessage->SetInnerRML("Update available!");
            if (mUpdateDownloadLabel != nullptr) {
                mUpdateDownloadLabel->SetInnerRML(escape(
                    fmt::format("Download {}", update_release_label(sUpdateCheckResult->latest))));
            }
        } else {
            mUpdateStatus->SetAttribute("state", "failed");
            mUpdateMessage->SetInnerRML("Failed to check for updates");
        }
    }

    Document::update();
}

void return_to_prelaunch() noexcept {
    close_all_documents();
    push_document(std::make_unique<Prelaunch>(), true);
}

bool Prelaunch::focus() {
    if (mMenuButtons.empty()) {
        return false;
    }
    return mMenuButtons.front()->focus();
}

bool Prelaunch::visible() const {
    return mDocument->HasAttribute("open") && mRoot->HasAttribute("open");
}

bool Prelaunch::handle_nav_command(Rml::Event& event, NavCommand cmd) {
    int direction = 0;
    if (cmd == NavCommand::Down) {
        direction = 1;
    } else if (cmd == NavCommand::Up) {
        direction = -1;
    } else {
        return false;
    }
    auto* target = event.GetTargetElement();
    int focusedButton = -1;
    for (int i = 0; i < mMenuButtons.size(); ++i) {
        if (mMenuButtons[i]->contains(target)) {
            focusedButton = i;
            break;
        }
    }
    const auto n = static_cast<int>(mMenuButtons.size());
    int i = ((focusedButton + direction) % n + n) % n;
    while (i >= 0 && i < mMenuButtons.size()) {
        if (mMenuButtons[i]->focus()) {
            mDoAud_seStartMenu(kSoundItemFocus);
            event.StopPropagation();
            return true;
        }
        i += direction;
    }
    return false;
}

}  // namespace dusk::ui
