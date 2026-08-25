#include "onboarding.hpp"

#include "modal.hpp"

#include "dusk/data.hpp"
#include "dusk/disc_discovery.hpp"
#include "dusk/iso_validate.hpp"
#include "dusk/transfer/server.hpp"
#include "dusk/ui/prelaunch.hpp"

#if defined(__APPLE__)
#include <TargetConditionals.h>
#endif
#if defined(TARGET_OS_TV) && TARGET_OS_TV
#include "dusk/tvos/network_info.hpp"
#endif

#include <fmt/format.h>

#include <memory>
#include <string>

namespace dusk::ui::onboarding {
namespace {

std::string device_address() {
#if defined(TARGET_OS_TV) && TARGET_OS_TV
    return dusk::tvos::lan_address();
#else
    return "127.0.0.1";
#endif
}

// Maps the app's own verification result onto something worth showing a person. The distinction
// that matters is HashMismatch: a correct upload of a bad dump lands there, and telling someone to
// re-upload would send them round a loop that cannot succeed.
transfer::Validator make_validator() {
    return [](const std::filesystem::path& staged) {
        iso::DiscInfo info{};
        iso::VerificationStatus status{};
        const auto err = iso::validate(staged.string().c_str(), status, info);
        using E = iso::ValidationError;
        switch (err) {
        case E::Success:
            return transfer::ValidateOutcome{true, false, "verified"};
        case E::IOError:
            // Our failure, not the file's: keep the upload so verification can be retried without
            // transferring it all over again.
            return transfer::ValidateOutcome{false, true,
                "Could not read the uploaded file. Try verifying again."};
        case E::Canceled:
            return transfer::ValidateOutcome{false, true, "Verification cancelled."};
        case E::InvalidImage:
            return transfer::ValidateOutcome{false, false,
                "That file is not a readable disc image. It may be truncated - try uploading it again."};
        case E::WrongGame:
            return transfer::ValidateOutcome{false, false, "That disc is a different game."};
        case E::WrongVersion:
            return transfer::ValidateOutcome{false, false,
                "That is the right game, but a version this app does not support."};
        case E::HashMismatch:
            return transfer::ValidateOutcome{false, false,
                "The disc contents do not match a known good dump. The upload itself was fine - the "
                "source disc image needs to be dumped again."};
        case E::Unknown:
        default:
            return transfer::ValidateOutcome{false, false, "Verification failed."};
        }
    };
}

std::string human_bytes(std::uint64_t bytes) {
    static constexpr const char* kUnits[] = {"B", "KB", "MB", "GB"};
    double value = static_cast<double>(bytes);
    int unit = 0;
    while (value >= 1024.0 && unit < 3) {
        value /= 1024.0;
        ++unit;
    }
    return fmt::format("{:.1f} {}", value, kUnits[unit]);
}

class TransferModal : public Modal {
public:
    explicit TransferModal(Modal::Props props, std::unique_ptr<transfer::Server> server,
                           std::string url)
        : Modal(std::move(props)), mServer(std::move(server)), mUrl(std::move(url)) {}

    void update() override {
        Modal::update();
        if (!mServer) {
            return;
        }
        const transfer::Progress progress = mServer->progress();
        const std::string body = render(progress);
        if (body != mLastBody) {
            mLastBody = body;
            set_body(body);
        }
        if (progress.phase == transfer::Phase::Published && !mPublished) {
            mPublished = true;
            // The disc is in place; hand the picker a fresh scan so it appears without a relaunch.
            auto& state = prelaunch_state();
            state.pendingDiscChoices.clear();
            for (const auto& candidate : disc_discovery::scan()) {
                state.pendingDiscChoices.push_back(candidate.path.string());
            }
            state.pendingDiscChoiceNotice = true;
        }
    }

private:
    std::string render(const transfer::Progress& progress) const {
        switch (progress.phase) {
        case transfer::Phase::Receiving:
            return fmt::format(
                "Receiving <b>{}</b> of <b>{}</b>.<br/><br/>Keep this page open on your device.<br/>"
                "<b>{}</b>",
                human_bytes(progress.received), human_bytes(progress.total), mUrl);
        case transfer::Phase::Validating:
            return "Verifying the disc image. This can take a few minutes for a large disc.";
        case transfer::Phase::Failed:
            return fmt::format("{}<br/><br/>Open <b>{}</b> to try again.", progress.message, mUrl);
        case transfer::Phase::Published:
            return "Disc added. Choose it from the list to start playing.";
        case transfer::Phase::Idle:
        default:
            return fmt::format(
                "On a phone or computer on the same network, open:<br/><br/><b>{}</b><br/><br/>"
                "Then choose your disc image. It is checked before anything is sent.",
                mUrl);
        }
    }

    std::unique_ptr<transfer::Server> mServer;
    std::string mUrl;
    std::string mLastBody;
    bool mPublished = false;
};

}  // namespace

void push(Document& host) {
    const std::string address = device_address();
    auto dismiss = [](Modal& modal) { modal.pop(); };

    if (address.empty()) {
        host.push(std::make_unique<Modal>(Modal::Props{
            .title = "No network connection",
            .bodyRml = "Connect this Apple TV to your network, then try again. A disc image is "
                       "transferred over the local network from a phone or computer.",
            .actions = {ModalAction{.label = "OK", .onPressed = dismiss}},
            .onDismiss = dismiss,
            .icon = "warning",
        }));
        return;
    }

    transfer::ServerConfig config;
    config.discsDir = data::configured_data_path() / "discs";
    config.acceptedGameIds = iso::accepted_game_ids_json();

    auto server = std::make_unique<transfer::Server>(config, make_validator());
    if (!server->start()) {
        host.push(std::make_unique<Modal>(Modal::Props{
            .title = "Could not start the transfer service",
            .bodyRml = "No network port was available. Close the app and open it again.",
            .actions = {ModalAction{.label = "OK", .onPressed = dismiss}},
            .onDismiss = dismiss,
            .icon = "warning",
        }));
        return;
    }

    const std::string url = fmt::format("http://{}:{}", address, server->port());
    Modal::Props props{
        .title = "Add a disc",
        .bodyRml = fmt::format(
            "On a phone or computer on the same network, open:<br/><br/><b>{}</b><br/><br/>"
            "Then choose your disc image. It is checked before anything is sent.",
            url),
        // Dismissing destroys the modal, which stops the server: it never runs while the game does.
        .actions = {ModalAction{.label = "Done", .onPressed = dismiss}},
        .onDismiss = dismiss,
        .icon = "",
    };
    host.push(std::make_unique<TransferModal>(std::move(props), std::move(server), url));
}

}  // namespace dusk::ui::onboarding
