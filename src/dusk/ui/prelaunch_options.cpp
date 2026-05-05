#include "prelaunch_options.hpp"

#include "dusk/config.hpp"
#include "dusk/settings.h"
#include "pane.hpp"
#include "prelaunch.hpp"

namespace dusk::ui {
namespace {

static constexpr std::array<const char*, 5> kLanguageNames = {
    "English", "German", "French", "Spanish", "Italian",
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

class LanguageSelect final : public SelectButton {
public:
    explicit LanguageSelect(Rml::Element* parent) : SelectButton(parent, Props{.key = "Language"}) {}

    void update() override {
        ensure_initialized();
        refresh_path_state();

        const bool validPath = is_selected_path_valid();
        const bool ntscDiscLocked = validPath && !prelaunch_state().isPal;

        if (ntscDiscLocked) {
            if (getSettings().game.language.getValue() != GameLanguage::English) {
                getSettings().game.language.setValue(GameLanguage::English);
                config::Save();
            }
            set_disabled(true);
        } else {
            set_disabled(false);
        }

        const auto lang = getSettings().game.language.getValue();
        auto value = static_cast<u8>(lang);
        if (value >= kLanguageNames.size()) {
            getSettings().game.language.setValue(GameLanguage::English);
            config::Save();
            value = static_cast<u8>(getSettings().game.language.getValue());
        }
        set_value_label(kLanguageNames[value]);
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
    explicit BackendSelect(Rml::Element* parent) : SelectButton(parent, Props{.key = "Graphics Backend"}) {}

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
        getSettings().backend.graphicsBackend.setValue(std::string(backend_id(backends[static_cast<size_t>(idx)])));
        config::Save();
        return true;
    }
};

class SaveTypeSelect final : public SelectButton {
public:
    explicit SaveTypeSelect(Rml::Element* parent) : SelectButton(parent, Props{.key = "Save File Type"}) {}

    void update() override {
        const CARDFileType cft = static_cast<CARDFileType>(getSettings().backend.cardFileType.getValue());
        set_value_label(cft == CARD_GCIFOLDER ? "GCI Folder" : "Card Image");
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
    add_tab("Options", [this](Rml::Element* content) {
        auto& leftPane = add_child<Pane>(content, Pane::Type::Controlled);
        leftPane.add_child<LanguageSelect>();
        leftPane.add_child<BackendSelect>();
        leftPane.add_child<SaveTypeSelect>();
    });
}

}  // namespace dusk::ui
