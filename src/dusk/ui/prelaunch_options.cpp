#include "prelaunch_options.hpp"

#include "dusk/config.hpp"
#include "dusk/main.h"
#include "dusk/settings.h"
#include "pane.hpp"
#include "prelaunch.hpp"

namespace dusk::ui {
namespace {

static constexpr std::array<const char*, 5> kLanguageNames = {
    "English",
    "German",
    "French",
    "Spanish",
    "Italian",
};

// TODO: Copied from ImGui prelaunch. Needs a refactor?
bool try_parse_backend(std::string_view backend, AuroraBackend& outBackend) {
    if (backend == "auto") {
        outBackend = BACKEND_AUTO;
        return true;
    }
    if (backend == "d3d11") {
        outBackend = BACKEND_D3D11;
        return true;
    }
    if (backend == "d3d12") {
        outBackend = BACKEND_D3D12;
        return true;
    }
    if (backend == "metal") {
        outBackend = BACKEND_METAL;
        return true;
    }
    if (backend == "vulkan") {
        outBackend = BACKEND_VULKAN;
        return true;
    }
    if (backend == "opengl") {
        outBackend = BACKEND_OPENGL;
        return true;
    }
    if (backend == "opengles") {
        outBackend = BACKEND_OPENGLES;
        return true;
    }
    if (backend == "webgpu") {
        outBackend = BACKEND_WEBGPU;
        return true;
    }
    if (backend == "null") {
        outBackend = BACKEND_NULL;
        return true;
    }

    return false;
}

std::string_view backend_name(AuroraBackend backend) {
    switch (backend) {
    default:
        return "Auto";
    case BACKEND_D3D12:
        return "D3D12";
    case BACKEND_D3D11:
        return "D3D11";
    case BACKEND_METAL:
        return "Metal";
    case BACKEND_VULKAN:
        return "Vulkan";
    case BACKEND_OPENGL:
        return "OpenGL";
    case BACKEND_OPENGLES:
        return "OpenGL ES";
    case BACKEND_WEBGPU:
        return "WebGPU";
    case BACKEND_NULL:
        return "Null";
    }
}

std::string_view backend_id(AuroraBackend backend) {
    switch (backend) {
    default:
        return "auto";
    case BACKEND_D3D12:
        return "d3d12";
    case BACKEND_D3D11:
        return "d3d11";
    case BACKEND_METAL:
        return "metal";
    case BACKEND_VULKAN:
        return "vulkan";
    case BACKEND_OPENGL:
        return "opengl";
    case BACKEND_OPENGLES:
        return "opengles";
    case BACKEND_WEBGPU:
        return "webgpu";
    case BACKEND_NULL:
        return "null";
    }
}

std::vector<AuroraBackend> available_backends() {
    std::vector<AuroraBackend> backends;
    backends.emplace_back(BACKEND_AUTO);
    size_t backendCount = 0;
    const AuroraBackend* raw = aurora_get_available_backends(&backendCount);
    for (size_t i = 0; i < backendCount; ++i) {
        // Do not expose NULL or D3D11
        if (raw[i] != BACKEND_NULL && raw[i] != BACKEND_D3D11) {
            backends.emplace_back(raw[i]);
        }
    }
    return backends;
}

class DiscSelect final : public SelectButton {
public:
    explicit DiscSelect(Rml::Element* parent)
        : SelectButton(parent, Props{.key = "Change Disc Image"}) {}

    void update() override {
        ensure_initialized();

        const auto& path = prelaunch_state().selectedDiscPath;
        std::string display;
        if (path.empty()) {
            display = "(none)";
        } else {
            display = std::filesystem::path(path).filename().string();
            if (display.empty()) {
                display = path;
            }
        }
        const auto& initial = prelaunch_state().initialDiscPath;
        if (!initial.empty() && path != initial) {
            display += " (restart required)";
        }
        set_value_label(Rml::String(display));
        SelectButton::update();
    }

protected:
    bool handle_nav_command(NavCommand cmd) override {
        if (cmd != NavCommand::Confirm) {
            return false;
        }
        open_iso_picker();
        return true;
    }
};

class LanguageSelect final : public SelectButton {
public:
    explicit LanguageSelect(Rml::Element* parent)
        : SelectButton(parent, Props{.key = "Language"}) {}

    void update() override {
        ensure_initialized();

        // LanguageInit already forces English for USA discs, so we can just change the button's
        // value instead of actually updating the config. This allows the old language setting to
        // be remembered when switching back to a PAL disc.
        const auto& state = prelaunch_state();
        std::string value;
        if (state.selectedDiscIsValid && !state.selectedDiscIsPal) {
            value = kLanguageNames[0];
            set_disabled(true);
        } else {
            const u8 idx = static_cast<u8>(getSettings().game.language.getValue());
            value = kLanguageNames[idx];
            set_disabled(false);
        }
        if (getSettings().game.language.getValue() != state.initialLanguage) {
            value += " (restart required)";
        }

        set_value_label(Rml::String(value));
        SelectButton::update();
    }

protected:
    bool handle_nav_command(NavCommand cmd) override {
        if (disabled()) {
            return false;
        }
        if (cmd != NavCommand::Confirm && cmd != NavCommand::Left && cmd != NavCommand::Right) {
            return false;
        }

        constexpr int n = static_cast<int>(kLanguageNames.size());
        int idx = static_cast<int>(getSettings().game.language.getValue());
        const int dir = (cmd == NavCommand::Left) ? -1 : 1;
        idx = ((idx + dir) % n + n) % n;
        getSettings().game.language.setValue(static_cast<GameLanguage>(idx));
        config::Save();
        return true;
    }
};

class BackendSelect final : public SelectButton {
public:
    explicit BackendSelect(Rml::Element* parent)
        : SelectButton(parent, Props{.key = "Graphics Backend"}) {}

    void update() override {
        AuroraBackend configuredBackend = BACKEND_AUTO;
        const auto configuredId = getSettings().backend.graphicsBackend.getValue();
        if (!try_parse_backend(configuredId, configuredBackend)) {
            configuredBackend = BACKEND_AUTO;
        }
        // Do not expose NULL or D3D11
        if (configuredBackend == BACKEND_NULL || configuredBackend == BACKEND_D3D11) {
            getSettings().backend.graphicsBackend.setValue("auto");
            config::Save();
            configuredBackend = BACKEND_AUTO;
        }

        const auto backend = getSettings().backend.graphicsBackend.getValue();
        Rml::String value = backend_name(configuredBackend).data();
        if (backend != prelaunch_state().initialGraphicsBackend) {
            value += " (restart required)";
        }
        set_value_label(value);
        SelectButton::update();
    }

protected:
    bool handle_nav_command(NavCommand cmd) override {
        if (cmd != NavCommand::Confirm && cmd != NavCommand::Left && cmd != NavCommand::Right) {
            return false;
        }

        const auto backends = available_backends();
        const int n = static_cast<int>(backends.size());
        if (n <= 0) {
            return false;
        }

        AuroraBackend configuredBackend = BACKEND_AUTO;
        const auto configuredId = getSettings().backend.graphicsBackend.getValue();
        if (!try_parse_backend(configuredId, configuredBackend)) {
            configuredBackend = BACKEND_AUTO;
        }

        int idx = 0;
        for (int i = 0; i < n; ++i) {
            if (backends[static_cast<size_t>(i)] == configuredBackend) {
                idx = i;
                break;
            }
        }

        const int dir = (cmd == NavCommand::Left) ? -1 : 1;
        idx = ((idx + dir) % n + n) % n;
        getSettings().backend.graphicsBackend.setValue(
            std::string(backend_id(backends[static_cast<size_t>(idx)])));
        config::Save();
        return true;
    }
};

class SaveTypeSelect final : public SelectButton {
public:
    explicit SaveTypeSelect(Rml::Element* parent)
        : SelectButton(parent, Props{.key = "Save File Type"}) {}

    void update() override {
        ensure_initialized();

        const CARDFileType cft =
            static_cast<CARDFileType>(getSettings().backend.cardFileType.getValue());
        std::string label = cft == CARD_GCIFOLDER ? "GCI Folder" : "Card Image";
        if (getSettings().backend.cardFileType.getValue() != prelaunch_state().initialCardFileType)
        {
            label += " (restart required)";
        }
        set_value_label(Rml::String(label));
        SelectButton::update();
    }

protected:
    bool handle_nav_command(NavCommand cmd) override {
        if (cmd != NavCommand::Confirm && cmd != NavCommand::Left && cmd != NavCommand::Right) {
            return false;
        }

        CARDFileType cft = static_cast<CARDFileType>(getSettings().backend.cardFileType.getValue());
        const CARDFileType newValue = cft == CARD_GCIFOLDER ? CARD_RAWIMAGE : CARD_GCIFOLDER;
        getSettings().backend.cardFileType.setValue(newValue);
        config::Save();
        return true;
    }
};

}  // namespace

PrelaunchOptions::PrelaunchOptions() {
    mSuppressNavFallback = true;
    add_tab("Options", [this](Rml::Element* content) {
        auto& leftPane = add_child<Pane>(content, Pane::Type::Controlled);
        leftPane.add_child<DiscSelect>();
        leftPane.add_child<LanguageSelect>();
        leftPane.add_child<BackendSelect>();
        leftPane.add_child<SaveTypeSelect>();
    });
}

void PrelaunchOptions::push_modal(Modal::Props props) {
    for (auto& action : props.actions) {
        auto originalOnPressed = std::move(action.onPressed);
        action.onPressed = [this, props, callback = std::move(originalOnPressed)](Modal& modal) {
            if (props.doBlur) {
                mRoot->SetClass("blurred", false);
            }
            if (callback) {
                callback(modal);
            }
        };
    }
    auto originalOnDismiss = std::move(props.onDismiss);
    props.onDismiss = [this, props, callback = std::move(originalOnDismiss)](Modal& modal) {
        if (props.doBlur) {
            mRoot->SetClass("blurred", false);
        }
        if (callback) {
            callback(modal);
        }
    };
    if (props.doBlur) {
        mRoot->SetClass("blurred", true);
    }
    push_document(std::make_unique<Modal>(std::move(props)));
}

void PrelaunchOptions::hide(bool close) {
    mRoot->SetClass("blurred", false);
    Window::hide(close);
}

void PrelaunchOptions::update() {
    Window::update();

    auto& state = prelaunch_state();
    if (state.errorString.empty() || top_document() != this) {
        return;
    }

    auto dismissInvalidDisc = [](Modal& modal) {
        prelaunch_state().errorString.clear();
        modal.pop();
    };
    push_modal(Modal::Props{
        .title = "Invalid disc image",
        .bodyRml = state.errorString,
        .actions =
            {
                ModalAction{
                    .label = "OK",
                    .onPressed = dismissInvalidDisc,
                },
            },
        .onDismiss = dismissInvalidDisc,
        .doBlur = true,
    });
}

bool PrelaunchOptions::consume_close_request() {
    if (!is_restart_pending()) {
        return false;
    }
    if (top_document() != this) {
        return true;
    }

    std::vector<ModalAction> actions;
    if constexpr (dusk::SupportsProcessRestart) {
        actions.push_back(ModalAction{
            .label = "Restart later",
            .onPressed =
                [this](Modal& modal) {
                    modal.pop();
                    pop();
                },
        });
        actions.push_back(ModalAction{
            .label = "Restart now",
            .onPressed = [](Modal&) { dusk::RequestRestart(); },
        });
    } else {
        actions.push_back(ModalAction{
            .label = "OK",
            .onPressed =
                [this](Modal& modal) {
                    modal.pop();
                    pop();
                },
        });
    }

    push_modal(Modal::Props{
        .title = "Apply Options",
        .bodyRml = dusk::SupportsProcessRestart ?
                       "A restart is required to apply selected options.<br/><br/>Restart now to "
                       "apply them immediately?" :
                       "A restart is required to apply selected options.<br/><br/>Close and reopen "
                       "Dusk to apply them.",
        .actions = std::move(actions),
        .onDismiss = [](Modal& modal) { modal.pop(); },
        .doBlur = true,
    });
    return true;
}

}  // namespace dusk::ui
