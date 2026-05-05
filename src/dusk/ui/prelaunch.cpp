#include "prelaunch.hpp"

#include "dusk/config.hpp"
#include "dusk/file_select.hpp"
#include "dusk/iso_validate.hpp"
#include "dusk/main.h"
#include "dusk/settings.h"
#include "modal.hpp"
#include "popup.hpp"
#include "preset.hpp"
#include "settings.hpp"
#include "version.h"

#include <SDL3/SDL_dialog.h>
#include <aurora/lib/window.hpp>

#include "m_Do/m_Do_MemCard.h"

namespace dusk::ui {

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
                <div class="eyebrow"><span>Twilit Realm</span> presents</div>
                <img src="res/logo-mascot.png" />
            </hero>
            <div id="menu-list" />
        </menu>
        <disc-info class="intro-item delay-4">
            <div id="disc-status">
                <icon />
                <span id="disc-status-label" />
            </div>
            <span id="disc-version" class="detail" />
        </disc-info>
        <version-info class="intro-item delay-5">
            <div class="version">Version <span id="version-text"></span></div>
            <div class="update"><span>Update available!</span> Download</div>
        </version-info>
    </content>
</body>
</rml>
)RML";

constexpr std::array<SDL_DialogFileFilter, 2> kDiscFileFilters{{
    {"Game Disc Images", "iso;gcm;ciso;gcz;nfs;rvz;wbfs;wia;tgc"},
    {"All Files", "*"},
}};

static std::string get_error_msg(iso::ValidationError error) {
    switch (error) {
    case iso::ValidationError::IOError:
        return "Unable to read the selected file.";
    case iso::ValidationError::InvalidImage:
        return "The selected file is not a valid disc image.";
    case iso::ValidationError::WrongGame:
        return "The selected game is not supported by Dusk.";
    case iso::ValidationError::WrongVersion:
        return "Dusk currently supports GameCube USA and PAL disc images only.";
    case iso::ValidationError::Success:
        return "The selected disc image is valid.";
    default:
        return "The selected disc image could not be validated.";
    }
}

void file_dialog_callback(void*, const char* path, const char* error) {
    auto& state = prelaunch_state();
    if (error != nullptr) {
        return;
    }
    if (path == nullptr) {
        return;
    }

    const auto validation = iso::validate(path);
    if (validation != iso::ValidationError::Success) {
        state.errorString = escape(get_error_msg(validation));
        return;
    }

    state.selectedDiscPath = path;
    state.errorString.clear();
    getSettings().backend.isoPath.setValue(state.selectedDiscPath);
    config::Save();
    refresh_state();
}

PrelaunchState sPrelaunchState;

PrelaunchState& prelaunch_state() noexcept {
    return sPrelaunchState;
}

void refresh_state() noexcept {
    auto& state = prelaunch_state();
    const auto validation = iso::validate(state.selectedDiscPath.c_str());
    if (state.selectedDiscPath.empty() || validation != iso::ValidationError::Success) {
        state.selectedDiscIsValid = false;
        return;
    }
    state.selectedDiscIsValid = true;
    state.selectedDiscIsPal = iso::isPal(state.selectedDiscPath.c_str());
}

void ensure_initialized() noexcept {
    auto& state = prelaunch_state();
    if (state.initialized) {
        return;
    }

    state.selectedDiscPath = getSettings().backend.isoPath;
    state.initialDiscPath = state.selectedDiscPath;
    if (iso::validate(state.initialDiscPath.c_str()) == iso::ValidationError::Success) {
        state.initialDiscIsPal = iso::isPal(state.initialDiscPath.c_str());
    }
    state.initialLanguage = getSettings().game.language;
    state.initialGraphicsBackend = getSettings().backend.graphicsBackend;
    state.initialCardFileType = getSettings().backend.cardFileType;
    state.errorString.clear();
    state.initialized = true;
    refresh_state();
}

void open_iso_picker() noexcept {
    ensure_initialized();
    ShowFileSelect(&file_dialog_callback, nullptr, aurora::window::get_sdl_window(),
        kDiscFileFilters.data(), kDiscFileFilters.size(), nullptr, false);
}

bool is_restart_pending() noexcept {
    const auto& state = prelaunch_state();
    if (!state.initialDiscPath.empty() && state.selectedDiscPath != state.initialDiscPath) {
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

Prelaunch::Prelaunch() : Document(kDocumentSource), mRoot(mDocument->GetElementById("root")) {
    ensure_initialized();

    if (auto* menuList = mDocument->GetElementById("menu-list")) {
        auto& state = prelaunch_state();
        mMenuButtons.push_back(std::make_unique<Button>(
            menuList, state.selectedDiscIsValid ? "Play" : "Select Disc Image"));
        mMenuButtons.back()->on_pressed([this] {
            if (!prelaunch_state().selectedDiscIsValid) {
                open_iso_picker();
                return;
            }

            mDoAud_seStartMenu(kSoundPlay);

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

            IsGameLaunched = true;
            if (!getSettings().backend.wasPresetChosen) {
                push_document(std::make_unique<dusk::ui::PresetWindow>());
            }
            hide(true);
        });
        apply_intro_animation(mMenuButtons.back()->root(), "delay-1");

        mMenuButtons.push_back(std::make_unique<Button>(menuList, "Options"));
        mMenuButtons.back()->on_pressed([this] {
            mRestartSuppressed = false;
            push(std::make_unique<SettingsWindow>(true));
        });
        apply_intro_animation(mMenuButtons.back()->root(), "delay-2");

        mMenuButtons.push_back(std::make_unique<Button>(menuList, "Quit To Desktop"));
        mMenuButtons.back()->on_pressed([] { IsRunning = false; });
        apply_intro_animation(mMenuButtons.back()->root(), "delay-3");
    }

    mDiscStatus = mDocument->GetElementById("disc-status");
    mDiscDetail = mDocument->GetElementById("disc-version");
    mVersion = mDocument->GetElementById("version-text");

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
        if constexpr (dusk::SupportsProcessRestart) {
            actions.push_back(ModalAction{
                .label = "Restart later",
                .onPressed = dismiss,
            });
            actions.push_back(ModalAction{
                .label = "Restart now",
                .onPressed = [](Modal&) { dusk::RequestRestart(); },
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
                dusk::SupportsProcessRestart ?
                    "A restart is required to apply selected options.<br/><br/>Restart now to "
                    "apply them immediately?" :
                    "A restart is required to apply selected options.<br/><br/>Close and reopen "
                    "Dusk to apply them.",
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

    auto& state = prelaunch_state();
    if (!state.errorString.empty() && top_document() == this) {
        auto dismiss = [](Modal& modal) {
            prelaunch_state().errorString.clear();
            modal.pop();
        };
        push(std::make_unique<Modal>(Modal::Props{
            .title = "Invalid disc image",
            .bodyRml = state.errorString,
            .actions =
                {
                    ModalAction{
                        .label = "OK",
                        .onPressed = dismiss,
                    },
                },
            .onDismiss = dismiss,
        }));
    }

    const bool hasValidPath = prelaunch_state().selectedDiscIsValid;
    mDocument->SetClass("disc-ready", hasValidPath);
    if (hasValidPath) {
        if (getSettings().backend.skipPreLaunchUI) {
            hide(true);
        }
        IsGameLaunched = true;
    }

    if (!mEntranceAnimationStarted && mDocument != nullptr) {
        mDocument->SetClass("animate-in", true);
        mEntranceAnimationStarted = true;
    }

    if (!mMenuButtons.empty()) {
        mMenuButtons[0]->set_text(hasValidPath ? "Play" : "Select Disc Image");
    }

    const auto discStatusLabel = mDiscStatus->GetElementById("disc-status-label");

    if (mDiscStatus != nullptr && discStatusLabel != nullptr) {
        if (hasValidPath) {
            mDiscStatus->SetAttribute("status", "good");
            discStatusLabel->SetInnerRML("Disc ready.");
        }
    }
    if (mDiscDetail != nullptr) {
        if (hasValidPath) {
            mDiscDetail->SetProperty(Rml::PropertyId::Display, Rml::Style::Display::Block);
            mDiscDetail->SetInnerRML(
                prelaunch_state().initialDiscIsPal ? "GameCube • EUR" : "GameCube • USA");
        } else {
            mDiscDetail->SetProperty(Rml::PropertyId::Display, Rml::Style::Display::None);
        }
    }
    if (mVersion != nullptr) {
        std::string_view versionStr(DUSK_WC_DESCRIBE);
        if (versionStr[0] == 'v') {
            versionStr = versionStr.substr(1);
        }
        mVersion->SetInnerRML(escape(versionStr));
    }

    Document::update();
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
