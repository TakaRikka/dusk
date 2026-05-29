#include "settings.hpp"

#include "aurora/gfx.h"
#include "bool_button.hpp"
#include "controller_config.hpp"
#include "dusk/app_info.hpp"
#include "dusk/audio/DuskAudioSystem.h"
#include "dusk/audio/DuskDsp.hpp"
#include "dusk/config.hpp"
#include "dusk/hotkeys.h"
#include "dusk/data.hpp"
#include "dusk/file_select.hpp"
#include "dusk/imgui/ImGuiEngine.hpp"
#include "dusk/io.hpp"
#include "dusk/livesplit.h"
#include "dusk/main.h"
#include "dusk/discord_presence.hpp"
#include "graphics_tuner.hpp"
#include "m_Do/m_Do_main.h"
#include "menu_bar.hpp"
#include "modal.hpp"
#include "number_button.hpp"
#include "pane.hpp"
#include "prelaunch.hpp"
#include "ui.hpp"

#include "dusk/i18n.hpp" 

#include <aurora/lib/window.hpp>
#include <SDL3/SDL_filesystem.h>
#include <fmt/format.h>

#if DUSK_ENABLE_SENTRY_NATIVE
#include "dusk/crash_reporting.h"
#endif

#include <algorithm>
#include <filesystem>

namespace dusk::ui {
namespace {

constexpr std::array kLanguageNames = {
    "English",
    "Spanish",
    "German",
    "French",
    "Italian",
    "Dutch",
    "Japanese",
    "Korean",
    "Chinese",
};

constexpr std::array kCardFileTypes = {
    "Card Image",
    "GCI Folder",
};

constexpr std::array kFpsOverlayCornerNames = {
    "Top Left",
    "Top Right",
    "Bottom Left",
    "Bottom Right",
};

constexpr std::array kInterpolationModes = {
    "Off",
    "Capped",
    "Unlimited",
};

constexpr std::array kGyroInputModeLabels = {
    "Sensor",
    "Mouse",
};

constexpr std::array kMenuScalingModeLabels = {
    "GameCube",
    "Wii",
    "Dusklight",
};

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
        if (raw[i] != BACKEND_NULL) {
            backends.emplace_back(raw[i]);
        }
    }
    return backends;
}

AuroraBackend configured_backend() {
    AuroraBackend configuredBackend = BACKEND_AUTO;
    const auto configuredId = getSettings().backend.graphicsBackend.getValue();
    if (!try_parse_backend(configuredId, configuredBackend)) {
        configuredBackend = BACKEND_AUTO;
    }
    return configuredBackend;
}

void reset_for_speedrun_mode() {
    mDoMain::developmentMode = -1;

    getSettings().game.enableTurboKeybind.setSpeedrunValue(false);

    getSettings().game.damageMultiplier.setSpeedrunValue(1);
    getSettings().game.instantDeath.setSpeedrunValue(false);
    getSettings().game.noHeartDrops.setSpeedrunValue(false);
    getSettings().game.autoSave.setSpeedrunValue(false);
    getSettings().game.sunsSong.setSpeedrunValue(false);

    getSettings().game.infiniteHearts.setSpeedrunValue(false);
    getSettings().game.infiniteArrows.setSpeedrunValue(false);
    getSettings().game.infiniteSeeds.setSpeedrunValue(false);
    getSettings().game.infiniteBombs.setSpeedrunValue(false);
    getSettings().game.infiniteOil.setSpeedrunValue(false);
    getSettings().game.infiniteOxygen.setSpeedrunValue(false);
    getSettings().game.infiniteRupees.setSpeedrunValue(false);
    getSettings().game.enableIndefiniteItemDrops.setSpeedrunValue(false);
    getSettings().game.moonJump.setSpeedrunValue(false);
    getSettings().game.superClawshot.setSpeedrunValue(false);
    getSettings().game.alwaysGreatspin.setSpeedrunValue(false);
    getSettings().game.enableFastIronBoots.setSpeedrunValue(false);
    getSettings().game.canTransformAnywhere.setSpeedrunValue(false);
    getSettings().game.fastRoll.setSpeedrunValue(false);
    getSettings().game.fastSpinner.setSpeedrunValue(false);
    getSettings().game.freeMagicArmor.setSpeedrunValue(false);
    getSettings().game.invincibleEnemies.setSpeedrunValue(false);

    getSettings().game.pauseOnFocusLost.setSpeedrunValue(false);
    aurora_set_pause_on_focus_lost(false);

    getSettings().backend.enableAdvancedSettings.setSpeedrunValue(false);
    getSettings().game.recordingMode.setSpeedrunValue(false);
    getSettings().game.debugFlyCam.setSpeedrunValue(false);
}

void clear_speedrun_overrides() {
    config::EnumerateRegistered([](config::ConfigVarBase& cvar) {
        cvar.clearSpeedrunOverride();
    });
}

void restore_from_speedrun_mode() {
    clear_speedrun_overrides();
    aurora_set_pause_on_focus_lost(getSettings().game.pauseOnFocusLost.getValue());
}

std::filesystem::path normalized_display_path(const std::filesystem::path& path) {
    std::error_code ec;
    auto normalized = std::filesystem::weakly_canonical(path, ec);
    if (!ec) {
        return normalized;
    }

    normalized = std::filesystem::absolute(path, ec);
    if (!ec) {
        return normalized.lexically_normal();
    }

    return path.lexically_normal();
}

std::filesystem::path user_home_path() {
    const char* homePath = SDL_GetUserFolder(SDL_FOLDER_HOME);
    if (homePath == nullptr || homePath[0] == '\0') {
        return {};
    }
    return std::filesystem::path{reinterpret_cast<const char8_t*>(homePath)};
}

Rml::String abbreviated_data_path_string() {
    const auto path = data::configured_data_path();
    const auto homePath = user_home_path();
    if (path.empty() || homePath.empty()) {
        return io::fs_path_to_string(path);
    }

    const auto normalizedPath = normalized_display_path(path);
    const auto normalizedHome = normalized_display_path(homePath);
    if (normalizedPath == normalizedHome) {
        return "~";
    }

    const auto relativePath = normalizedPath.lexically_relative(normalizedHome);
    if (!relativePath.empty() && !relativePath.is_absolute()) {
        const auto it = relativePath.begin();
        if (it == relativePath.end() || *it != "..") {
            return io::fs_path_to_string(std::filesystem::path{"~"} / relativePath);
        }
    }

    return io::fs_path_to_string(path);
}

Rml::String configured_data_path_display_name() {
    const auto path = abbreviated_data_path_string();
    if (path.empty()) {
        return tr("(none)");
    }

    auto display = display_name_for_path(path);
    if (display.empty()) {
        return path;
    }
    return display;
}

class DataFolderPathText : public Component {
public:
    explicit DataFolderPathText(Rml::Element* parent) : Component(append(parent, "div")) {}

    void update() override {
        const Rml::String rml = "<span class=\"data-folder-current\">" + tr("Current data folder:") + "<br/>" +
                                escape(abbreviated_data_path_string()) + "</span>";
        if (rml != mCurrentRml) {
            mRoot->SetInnerRML(rml);
            mCurrentRml = rml;
        }
        Component::update();
    }

private:
    Rml::String mCurrentRml;
};

void show_data_folder_error_modal(std::string_view message) {
    auto dismiss = [](Modal& modal) {
        mDoAud_seStartMenu(kSoundWindowClose);
        modal.pop();
    };
    push_document(std::make_unique<Modal>(Modal::Props{
        .title = tr("Data Folder Not Changed"),
        .bodyRml = escape(std::string(message)),
        .actions =
            {
                ModalAction{
                    .label = tr("OK"),
                    .onPressed = dismiss,
                },
            },
        .onDismiss = dismiss,
        .icon = "warning",
    }));
    if (auto* doc = top_document()) {
        doc->focus();
    }
}

void data_folder_dialog_callback(void*, const char* path, const char* error) {
    if (error != nullptr) {
        show_data_folder_error_modal(error);
        return;
    }
    if (path == nullptr) {
        return;
    }

    std::string dataPathError;
    if (data::set_custom_data_path(path, &dataPathError)) {
        mDoAud_seStartMenu(kSoundItemChange);
        return;
    }

    if (dataPathError.empty()) {
        dataPathError =
            fmt::format("{} could not use the selected folder as its data folder.", AppName);
    }
    show_data_folder_error_modal(dataPathError);
}

const Rml::String kInternalResolutionHelpText =
    "Configure the resolution used for rendering the game. Higher values are more demanding on "
    "your graphics hardware.";
const Rml::String kShadowResolutionHelpText =
    "Configure the shadow-map resolution. Higher values improve shadow quality but increase GPU "
    "and memory usage.";
const Rml::String kResamplerHelpText =
    "Configure the sampling method used when scaling the internal resolution for final presentation.";
const Rml::String kBloomHelpText =
    "Configure the post-processing bloom effect. Classic uses the original bloom pass; Dusklight uses "
    "a higher-quality bloom pass.";
const Rml::String kBloomBrightnessHelpText =
    "Configure bloom intensity. Higher values make bright areas glow more strongly.";
const Rml::String kDepthOfFieldHelpText =
    "Configure the post-processing depth-of-field effect. Classic uses the original depth-of-field pass;"
    " Dusklight uses a higher-quality depth-of-field pass.";
const Rml::String kUnlockFramerateHelpText =
    "<br/>Uses inter-frame interpolation to enable higher frame rates.<br/><br/>May introduce minor "
    "visual artifacts or animation glitches.";

int float_setting_percent(ConfigVar<float>& var) {
    return static_cast<int>(var.getValue() * 100.0f + 0.5f);
}

bool gyro_enabled() {
    return getSettings().game.enableGyroAim ||
           (getSettings().game.enableGyroRollgoal &&
            getSettings().game.gyroMode.getValue() != GyroMode::Mouse);
}

struct ConfigBoolProps {
    Rml::String key;
    Rml::String icon;
    Rml::String helpText;
    std::function<void(bool)> onChange;
    std::function<bool()> isDisabled;
};

SelectButton& config_bool_select(
    Pane& leftPane, Pane& rightPane, ConfigVar<bool>& var, ConfigBoolProps props) {
    auto& button = leftPane.add_child<BoolButton>(BoolButton::Props{
        .key = std::move(props.key),
        .icon = std::move(props.icon),
        .getValue = [&var] { return var.getValue(); },
        .setValue =
            [&var, callback = std::move(props.onChange)](bool value) {
                if (value == var.getValue()) {
                    return;
                }
                var.setValue(value);
                config::Save();
                if (callback) {
                    callback(value);
                }
            },
        .isDisabled = std::move(props.isDisabled),
        .isModified = [&var] { return var.getValue() != var.getDefaultValue(); },
    });
    leftPane.register_control(
        button, rightPane, [helpText = std::move(props.helpText)](Pane& pane) {
            pane.clear();
            pane.add_rml(helpText);
        });
    return button;
}

void add_speedrun_disabled_option(Pane& leftPane, Pane& rightPane, ConfigVar<bool>& var,
    const Rml::String& key, const Rml::String& helpText) {
    config_bool_select(leftPane, rightPane, var, {
        .key = key,
        .helpText = helpText,
        .isDisabled = [] { return getSettings().game.speedrunMode; },
    });
}

SelectButton& config_percent_select(Pane& leftPane, Pane& rightPane, ConfigVar<float>& var,
    Rml::String key, Rml::String helpText, int min, int max, int step = 5,
    std::function<bool()> isDisabled = {}) {
    auto& button = leftPane.add_child<NumberButton>(NumberButton::Props{
        .key = std::move(key),
        .getValue = [&var] { return float_setting_percent(var); },
        .setValue =
            [&var, min, max](int value) {
                var.setValue(std::clamp(value, min, max) / 100.0f);
                config::Save();
            },
        .isDisabled = std::move(isDisabled),
        .isModified = [&var] { return var.getValue() != var.getDefaultValue(); },
        .min = min,
        .max = max,
        .step = step,
        .suffix = "%",
    });
    leftPane.register_control(button, rightPane, [helpText = std::move(helpText)](Pane& pane) {
        pane.clear();
        pane.add_text(helpText);
    });
    return button;
}

SelectButton& config_int_select(Pane& leftPane, Pane& rightPane, ConfigVar<int>& var,
    Rml::String key, Rml::String helpText, int min, int max, int step = 5,
    std::function<bool()> isDisabled = {}, std::string suffix = "") {
    auto& button = leftPane.add_child<NumberButton>(NumberButton::Props{
        .key = std::move(key),
        .getValue = [&var] { return var; },
        .setValue =
            [&var, min, max](int value) {
                var.setValue(std::clamp(value, min, max));
                config::Save();
            },
        .isDisabled = std::move(isDisabled),
        .isModified = [&var] { return var.getValue() != var.getDefaultValue(); },
        .min = min,
        .max = max,
        .step = step,
        .suffix = suffix,
    });
    leftPane.register_control(button, rightPane, [helpText = std::move(helpText)](Pane& pane) {
        pane.clear();
        pane.add_text(helpText);
    });
    return button;
}

template <typename T>
void graphics_tuner_control(Window& window, Pane& leftPane, Pane& rightPane, ConfigVar<T>& var,
    const GraphicsTunerProps& props, bool prelaunch) {
    leftPane.register_control(
        leftPane
            .add_select_button({
                .key = props.title,
                .getValue =
                    [&var, option = props.option] {
                        if constexpr (std::is_same_v<T, float>) {
                            return format_graphics_setting_value(
                                option, float_setting_percent(var));
                        } else {
                            return format_graphics_setting_value(
                                option, static_cast<int>(var.getValue()));
                        }
                    },
                .isModified = [&var] { return var.getValue() != var.getDefaultValue(); },
                .submit = false,
            })
            .on_nav_command([&window, props, prelaunch](Rml::Event&, NavCommand cmd) {
                if (cmd == NavCommand::Confirm || cmd == NavCommand::Left ||
                    cmd == NavCommand::Right) {
                    window.push(std::make_unique<GraphicsTuner>(props, prelaunch));
                    return true;
                }
                return false;
            }),
        rightPane, [helpText = props.helpText](Pane& pane) {
            pane.clear();
            pane.add_text(helpText);
        });
}

}  // namespace

SettingsWindow::SettingsWindow(bool prelaunch) : mPrelaunch(prelaunch) {
    if (prelaunch) {
        mSuppressNavFallback = true;
        add_tab(tr("Prelaunch"), [this](Rml::Element* content) {
            auto& leftPane = add_child<Pane>(content, Pane::Type::Controlled);
            auto& rightPane = add_child<Pane>(content, Pane::Type::Uncontrolled);

            leftPane.register_control(
                leftPane
                    .add_select_button({
                        .key = tr("Disc Image"),
                        .getValue =
                            [] {
                                const auto& path = prelaunch_state().configuredDiscPath;
                                std::string display;
                                if (path.empty()) {
                                    display = tr("(none)");
                                } else {
                                    display = display_name_for_path(path);
                                    if (display.empty()) {
                                        display = path;
                                    }
                                }
                                return display;
                            },
                        .isModified =
                            [] {
                                const auto& state = prelaunch_state();
                                const auto& active = state.activeDiscPath;
                                return !active.empty() && state.configuredDiscPath != active;
                            },
                    })
                    .on_pressed([] { open_iso_picker(); }),
                rightPane, [](Pane& pane) {
                    pane.add_rml(tr("Set the disc image that Dusklight uses to launch the game.<br/><br/>"
                                 "Changes require a restart."));
                });
#if DUSK_CAN_CHANGE_DATA_FOLDER
            leftPane.register_control(
                leftPane.add_select_button({
                    .key = tr("Data Folder"),
                    .getValue = [] { return configured_data_path_display_name(); },
                    .isModified = [] { return data::is_data_path_restart_pending(); },
                }),
                rightPane, [](Pane& pane) {
                    pane.add_text(tr("The data folder is where Dusklight stores settings, saves, "
                                  "logs, texture replacements, and other app data."));
                    pane.add_child<DataFolderPathText>();
#if DUSK_CAN_OPEN_DATA_FOLDER
                    pane.add_button(tr("Open Data Folder")).on_pressed([] {
                        if (data::open_data_path()) {
                            mDoAud_seStartMenu(kSoundClick);
                        }
                    });
#endif
                    pane.add_button(tr("Change Data Folder")).on_pressed([] {
                        const auto defaultLocation =
                            io::fs_path_to_string(data::configured_data_path());
                        ShowFolderSelect(&data_folder_dialog_callback, nullptr,
                            aurora::window::get_sdl_window(),
                            defaultLocation.empty() ? nullptr : defaultLocation.c_str());
                    });
#if defined(_WIN32)
                    pane.add_button(tr("Portable Mode")).on_pressed([] {
                        if (data::set_portable_data_path()) {
                            mDoAud_seStartMenu(kSoundItemChange);
                        }
                    });
#endif
                    pane.add_button({
                        .text = tr("Reset to Default"),
                        .isDisabled = [] { return data::is_default_data_path(); },
                    }).on_pressed([] {
                        if (data::reset_data_path()) {
                            mDoAud_seStartMenu(kSoundItemChange);
                        }
                    });
                    pane.add_rml(tr("Data will be migrated automatically on restart."));
                });
#endif
            leftPane.register_control(
                leftPane.add_select_button({
                    .key = tr("Language"),
                    .getValue =
                        [] {
                            const auto& state = prelaunch_state();
                            if (!state.configuredDiscCanLaunch) {
                                return tr(kLanguageNames[0]);
                            }
                            const u8 idx = static_cast<u8>(getSettings().game.language.getValue());
                            return tr(kLanguageNames[idx]);
                        },
                    .isDisabled =
                        [] {
                            const auto& state = prelaunch_state();
                            return !state.configuredDiscCanLaunch;
                        },
                    .isModified =
                        [] {
                            return getSettings().game.language.getValue() !=
                                   prelaunch_state().initialLanguage;
                        },
                }),
                rightPane, [](Pane& pane) { 
                    for (int i = 0; i < static_cast<int>(kLanguageNames.size()); i++) {
                        pane.add_button({
                                            .text = tr(kLanguageNames[i]),
                                            .isSelected =
                                                [i] {
                                                    return getSettings().game.language.getValue() ==
                                                           static_cast<GameLanguage>(i);
                                                },
                                        })
                            // [this] 캡처 삭제하고 순수하게 언어값 저장만 수행
                            .on_pressed([i] { 
                                mDoAud_seStartMenu(kSoundItemChange);
                                getSettings().game.language.setValue(static_cast<GameLanguage>(i));
                                config::Save();
                            });
                    }
                    pane.add_rml(tr("<br/>Changes require a restart."));
                });
            leftPane.register_control(
                leftPane.add_select_button({
                    .key = tr("Graphics Backend"),
                    .getValue = [] { return tr(std::string(backend_name(configured_backend()))); },
                    .isModified =
                        [] {
                            return getSettings().backend.graphicsBackend.getValue() !=
                                   prelaunch_state().initialGraphicsBackend;
                        },
                }),
                rightPane, [](Pane& pane) {
                    const auto availableBackends = available_backends();
                    for (const auto backend : availableBackends) {
                        pane
                            .add_button({
                                .text = tr(std::string(backend_name(backend))),
                                .isSelected = [backend] { return configured_backend() == backend; },
                            })
                            .on_pressed([backend] {
                                mDoAud_seStartMenu(kSoundItemChange);
                                getSettings().backend.graphicsBackend.setValue(
                                    std::string{backend_id(backend)});
                                config::Save();
                            });
                    }
                    pane.add_rml(tr("<br/>Changes require a restart."));
                });
            leftPane.register_control(
                leftPane.add_select_button({
                    .key = tr("Save File Type"),
                    .getValue =
                        [] {
                            return tr(kCardFileTypes[getSettings().backend.cardFileType.getValue()]);
                        },
                    .isModified =
                        [] {
                            return getSettings().backend.cardFileType.getValue() !=
                                   prelaunch_state().initialCardFileType;
                        },
                }),
                rightPane, [](Pane& pane) {
                    for (int i = 0; i < static_cast<int>(kCardFileTypes.size()); i++) {
                        pane
                            .add_button({
                                .text = tr(kCardFileTypes[i]),
                                .isSelected =
                                    [i] {
                                        return getSettings().backend.cardFileType.getValue() == i;
                                    },
                            })
                            .on_pressed([i] {
                                mDoAud_seStartMenu(kSoundItemChange);
                                getSettings().backend.cardFileType.setValue(i);
                                config::Save();
                            });
                    }
                });
        });
    }

    add_tab(tr("Video"), [this](Rml::Element* content) {
        auto& leftPane = add_child<Pane>(content, Pane::Type::Controlled);
        auto& rightPane = add_child<Pane>(content, Pane::Type::Uncontrolled);

        leftPane.add_section(tr("Display"));

        leftPane.register_control(leftPane.add_button(tr("Toggle Fullscreen")).on_pressed([] {
            mDoAud_seStartMenu(kSoundItemChange);
            getSettings().video.enableFullscreen.setValue(!getSettings().video.enableFullscreen);
            VISetWindowFullscreen(getSettings().video.enableFullscreen);
            config::Save();
        }),
            rightPane, [](Pane& pane) { pane.clear(); });
        leftPane.register_control(leftPane.add_button(tr("Restore Default Window Size")).on_pressed([] {
            mDoAud_seStartMenu(kSoundItemChange);
            getSettings().video.enableFullscreen.setValue(false);
            VISetWindowFullscreen(false);
            VISetWindowSize(FB_WIDTH * 2, FB_HEIGHT * 2);
            VICenterWindow();
        }),
            rightPane, [](Pane& pane) { pane.clear(); });
        config_bool_select(leftPane, rightPane, getSettings().video.enableVsync,
            {
                .key = tr("Enable VSync"),
                .helpText = tr("Synchronizes the frame rate to your monitor's refresh rate."),
                .onChange = [](bool value) { aurora_enable_vsync(value); },
            });
        config_bool_select(leftPane, rightPane, getSettings().video.lockAspectRatio,
            {
                .key = tr("Lock 4:3 Aspect Ratio"),
                .helpText = tr("Lock the game's aspect ratio to the original."),
                .onChange =
                    [](bool value) {
                        AuroraSetViewportPolicy(
                            value ? AURORA_VIEWPORT_FIT : AURORA_VIEWPORT_STRETCH);
                    },
            });
        config_bool_select(leftPane, rightPane, getSettings().game.pauseOnFocusLost,
            {
                .key = tr("Pause on Focus Lost"),
                .helpText = tr("Pause the game when window focus is lost."),
                .onChange = [](bool value) { aurora_set_pause_on_focus_lost(value); },
                .isDisabled = [] { return IsMobile || getSettings().game.speedrunMode; },
            });
        leftPane.register_control(
            leftPane.add_select_button({
                .key = tr("Show FPS Counter"),
                .getValue =
                    [] {
                        if (!getSettings().video.enableFpsOverlay.getValue()) {
                            return tr("Off");
                        }
                        const int idx = getSettings().video.fpsOverlayCorner.getValue();
                        return tr(kFpsOverlayCornerNames[idx]);
                    },
                .isModified =
                    [] {
                        const auto& enable = getSettings().video.enableFpsOverlay;
                        const auto& corner = getSettings().video.fpsOverlayCorner;
                        return enable.getValue() != enable.getDefaultValue() ||
                               (enable.getValue() && corner.getValue() != corner.getDefaultValue());
                    },
            }),
            rightPane, [](Pane& pane) {
                pane.add_button(
                        {
                            .text = tr("Off"),
                            .isSelected =
                                [] { return !getSettings().video.enableFpsOverlay.getValue(); },
                        })
                    .on_pressed([] {
                        mDoAud_seStartMenu(kSoundItemChange);
                        getSettings().video.enableFpsOverlay.setValue(false);
                        config::Save();
                    });
                for (int i = 0; i < static_cast<int>(kFpsOverlayCornerNames.size()); ++i) {
                    pane.add_button(
                            {
                                .text = tr(kFpsOverlayCornerNames[i]),
                                .isSelected =
                                    [i] {
                                        return getSettings().video.enableFpsOverlay.getValue() &&
                                               getSettings().video.fpsOverlayCorner.getValue() == i;
                                    },
                            })
                        .on_pressed([i] {
                            mDoAud_seStartMenu(kSoundItemChange);
                            getSettings().video.enableFpsOverlay.setValue(true);
                            getSettings().video.fpsOverlayCorner.setValue(i);
                            config::Save();
                        });
                }
                pane.add_rml(
                    tr("<br/>Display the current framerate in a corner of the screen while playing."));
            });
        leftPane.add_section(tr("Resolution"));
        graphics_tuner_control(*this, leftPane, rightPane,
            getSettings().game.internalResolutionScale,
            GraphicsTunerProps{
                .option = GraphicsOption::InternalResolution,
                .title = tr("Internal Resolution"),
                .helpText = tr(std::string(kInternalResolutionHelpText)),
                .valueMin = 0,
                .valueMax = 12,
                .defaultValue = 0,
            }, mPrelaunch);
        graphics_tuner_control(*this, leftPane, rightPane,
            getSettings().game.shadowResolutionMultiplier,
            GraphicsTunerProps{
                .option = GraphicsOption::ShadowResolution,
                .title = tr("Shadow Resolution"),
                .helpText = tr(std::string(kShadowResolutionHelpText)),
                .valueMin = 1,
                .valueMax = 8,
                .defaultValue = 1,
            }, mPrelaunch);
        graphics_tuner_control(*this, leftPane, rightPane, getSettings().game.resampler,
            GraphicsTunerProps{
                .option = GraphicsOption::Resampler,
                .title = tr("Output Resampling"),
                .helpText = tr(std::string(kResamplerHelpText)),
                .valueMin = static_cast<int>(Resampler::Bilinear),
                .valueMax = static_cast<int>(Resampler::Area),
                .defaultValue = static_cast<int>(Resampler::Bilinear),
            }, mPrelaunch);

        leftPane.add_section(tr("Post-Processing"));
        graphics_tuner_control(*this, leftPane, rightPane, getSettings().game.bloomMode,
            GraphicsTunerProps{
                .option = GraphicsOption::BloomMode,
                .title = tr("Bloom"),
                .helpText = tr(std::string(kBloomHelpText)),
                .valueMin = static_cast<int>(BloomMode::Off),
                .valueMax = static_cast<int>(BloomMode::Dusk),
                .defaultValue = static_cast<int>(BloomMode::Classic),
            }, mPrelaunch);
        graphics_tuner_control(*this, leftPane, rightPane, getSettings().game.bloomMultiplier,
            GraphicsTunerProps{
                .option = GraphicsOption::BloomMultiplier,
                .title = tr("Bloom Brightness"),
                .helpText = tr(std::string(kBloomBrightnessHelpText)),
                .valueMin = 0,
                .valueMax = 100,
                .defaultValue = 100,
                .step = 10,
            },
            mPrelaunch);
        graphics_tuner_control(*this, leftPane, rightPane, getSettings().game.depthOfFieldMode,
            GraphicsTunerProps{
                .option = GraphicsOption::DepthOfFieldMode,
                .title = tr("Depth of Field"),
                .helpText = tr(std::string(kDepthOfFieldHelpText)),
                .valueMin = static_cast<int>(DepthOfFieldMode::Off),
                .valueMax = static_cast<int>(DepthOfFieldMode::Dusk),
                .defaultValue = static_cast<int>(DepthOfFieldMode::Classic),
            },
            mPrelaunch);

        leftPane.add_section(tr("Rendering"));
        config_bool_select(leftPane, rightPane, getSettings().game.enableTextureReplacements,
            {
                .key = tr("Use Texture Pack"),
                .helpText = tr("Enable installed texture replacements."),
                .onChange = [](bool value) { aurora_set_texture_replacements_enabled(value); },
            });
        leftPane.register_control(
            leftPane.add_select_button({
                .key = tr("Unlock Framerate"),
                .getValue =
                    [] {
                        return tr(kInterpolationModes[static_cast<u8>(getSettings().game.enableFrameInterpolation.getValue())]);
                    },
                .isModified =
                    [] {
                        return getSettings().game.enableFrameInterpolation.getValue() !=
                               getSettings().game.enableFrameInterpolation.getDefaultValue();
                    },
            }),
            rightPane, [](Pane& pane) {
                for (int i = 0; i < static_cast<int>(kInterpolationModes.size()); i++) {
                    pane.add_button({
                            .text = tr(kInterpolationModes[i]),
                            .isSelected =
                                [i] {
                                    return getSettings().game.enableFrameInterpolation.getValue() == static_cast<FrameInterpMode>(i);
                                },
                        })
                        .on_pressed([i] {
                            mDoAud_seStartMenu(kSoundItemChange);
                            getSettings().game.enableFrameInterpolation.setValue(static_cast<FrameInterpMode>(i));
                            config::Save();
                        });
                }
                pane.add_rml(tr(std::string(kUnlockFramerateHelpText)));
            });
        config_int_select(leftPane, rightPane, getSettings().video.maxFrameRate,
            tr("Framerate Cap"), tr("Limit the framerate to the specified value."), 30, 540, 1,
            [] { return getSettings().game.enableFrameInterpolation.getValue() != FrameInterpMode::Capped; });
        config_bool_select(leftPane, rightPane, getSettings().game.enableMapBackground,
            {
                .key = tr("Enable Mini-Map Shadows"),
                .helpText = tr("Render a thick shadow around the mini-map. May impact performance.")
            });
        config_bool_select(leftPane, rightPane, getSettings().game.disableCutscenePillarboxing,
            {
                .key = tr("Disable Cutscene Pillarboxing"),
                .helpText = tr("Disable black bars on the left and right sides of the screen "
                               "during some cutscenes, particularly on ultra-wide displays. "
                               "Visuals beyond the original intended framing may appear buggy.")
            });
    });

    add_tab(tr("Input"), [this](Rml::Element* content) {
        auto& leftPane = add_child<Pane>(content, Pane::Type::Controlled);
        auto& rightPane = add_child<Pane>(content, Pane::Type::Uncontrolled);

        auto addOption = [&](const Rml::String& key, ConfigVar<bool>& value,
                             const Rml::String& helpText, std::function<bool()> isDisabled = {}) {
            config_bool_select(leftPane, rightPane, value,
                {
                    .key = tr(key),
                    .helpText = tr(helpText),
                    .isDisabled = std::move(isDisabled),
                });
        };

        leftPane.add_section(tr("Inputs"));
        leftPane.register_control(leftPane.add_button(tr("Configure Inputs")).on_pressed([this] {
            push(std::make_unique<ControllerConfigWindow>(mPrelaunch));
        }),
            rightPane, [](Pane& pane) {
                pane.clear();
                pane.add_text(tr("Open input binding configuration."));
            });
        config_bool_select(leftPane, rightPane, getSettings().game.allowBackgroundInput,
            {
                .key = tr("Allow Background Inputs"),
                .helpText = tr("Allow inputs even when the game window is not focused."),
                .onChange = [](bool value) { aurora_set_background_input(value); },
            });

        leftPane.add_section(tr("Camera"));
        addOption(tr("Free Camera"), getSettings().game.freeCamera,
            tr("Enables twin-stick camera control, letting the C-Stick move the camera vertically as ") + 
            tr("well as horizontally."));
        addOption(tr("Invert Camera X Axis"), getSettings().game.invertCameraXAxis,
            tr("Invert horizontal camera movement."));
        addOption(tr("Invert Camera Y Axis"), getSettings().game.invertCameraYAxis,
            tr("Invert vertical camera movement when Free Camera is enabled."),
            [] { return !getSettings().game.freeCamera; });
        config_percent_select(leftPane, rightPane, getSettings().game.freeCameraSensitivity,
            tr("Free Camera Sensitivity"), tr("Adjusts twin-stick camera sensitivity."), 50, 200, 5,
            [] { return !getSettings().game.freeCamera; });
        addOption(tr("Invert First Person X Axis"), getSettings().game.invertFirstPersonXAxis,
            tr("Invert horizontal movement while aiming with items or first person camera. Applies only to the control stick (the gyroscope can be inverted in Input settings)."));
        addOption(tr("Invert First Person Y Axis"), getSettings().game.invertFirstPersonYAxis,
            tr("Invert vertical movement while aiming with items or first person camera. Applies only to the control stick (the gyroscope can be inverted in Input settings)."));
        addOption(tr("Invert Air/Swim X Axis"), getSettings().game.invertAirSwimX,
            tr("Invert horizontal movement while flying or swimming."));
        addOption(tr("Invert Air/Swim Y Axis"), getSettings().game.invertAirSwimY,
            tr("Invert vertical movement while flying or swimming."));

        leftPane.add_section(tr("Gyro"));
        leftPane.register_control(
            leftPane.add_select_button({
                .key = tr("Gyro Input Method"),
                .getValue =
                    [] {
                        const auto mode = getSettings().game.gyroMode.getValue();
                        const auto idx = static_cast<size_t>(mode);
                        return tr(kGyroInputModeLabels[idx]);
                    },
                .isModified =
                    [] {
                        return getSettings().game.gyroMode.getValue() !=
                               getSettings().game.gyroMode.getDefaultValue();
                    },
            }),
            rightPane, [](Pane& pane) {
                for (size_t i = 0; i < kGyroInputModeLabels.size(); i++) {
                    pane
                        .add_button({
                            .text = tr(kGyroInputModeLabels[i]),
                            .isSelected =
                                [i] {
                                    return getSettings().game.gyroMode.getValue() == static_cast<GyroMode>(i);
                                },
                        })
                        .on_pressed([i] {
                            mDoAud_seStartMenu(kSoundItemChange);
                            const GyroMode mode = static_cast<GyroMode>(i);
                            getSettings().game.gyroMode.setValue(mode);
                            config::Save();
                        });
                }
                pane.add_rml(
                    tr("<br/><b>Sensor</b> reads motion directly from a supported controller's gyro via SDL.<br/>"
                    "<br/><b>Mouse</b> treats mouse input as gyro, intended for use with the Steam Deck.<br/>"
                    "<br/>Mouse input cannot currently be used with Gyro Rollgoal."));
            });
        addOption(tr("Gyro Aim"), getSettings().game.enableGyroAim,
            tr("Enables gyro controls while in look mode, aiming a hawk, and aiming ") +
            tr("supported items.<br/><br/>Supported items include the Slingshot, Gale Boomerang, ") +
            tr("Hero's Bow, Clawshot(s), Ball and Chain, and Dominion Rod."));
        addOption(tr("Gyro Rollgoal"), getSettings().game.enableGyroRollgoal,
            tr("Enables gyro controls for Rollgoal in Hena's Cabin."),
            [] { return getSettings().game.gyroMode.getValue() == GyroMode::Mouse; });
        config_percent_select(leftPane, rightPane, getSettings().game.gyroSensitivityY,
            tr("Gyro Pitch Sensitivity"), tr("Controls vertical gyro aiming sensitivity."), 25, 400, 5,
            [] { return !gyro_enabled(); });
        config_percent_select(leftPane, rightPane, getSettings().game.gyroSensitivityX,
            tr("Gyro Yaw Sensitivity"), tr("Controls horizontal gyro aiming sensitivity."), 25, 400, 5,
            [] { return !gyro_enabled(); });
        config_percent_select(leftPane, rightPane, getSettings().game.gyroSensitivityRollgoal,
            tr("Rollgoal Sensitivity"), tr("Controls how strongly gyro input tilts the Rollgoal table."),
            25, 400, 5,
            [] {
                return !getSettings().game.enableGyroRollgoal ||
                       getSettings().game.gyroMode.getValue() == GyroMode::Mouse;
            });
        config_percent_select(leftPane, rightPane, getSettings().game.gyroDeadband, tr("Gyro Deadband"),
            tr("Ignores small gyro movement to reduce drift and jitter."), 0, 50, 1,
            [] { return !gyro_enabled(); });
        config_percent_select(leftPane, rightPane, getSettings().game.gyroSmoothing,
            tr("Gyro Smoothing"), tr("Higher values smooth gyro input over time."), 0, 100, 1,
            [] { return !gyro_enabled(); });
        addOption(tr("Invert Gyro Pitch"), getSettings().game.gyroInvertPitch,
            tr("Invert vertical gyro aiming."), [] { return !gyro_enabled(); });
        addOption(tr("Invert Gyro Yaw"), getSettings().game.gyroInvertYaw,
            tr("Invert horizontal gyro aiming."), [] { return !gyro_enabled(); });

        leftPane.add_section(tr("Tools"));
        addOption(tr("Turbo Key"), getSettings().game.enableTurboKeybind,
            tr("Hold Tab to increase game speed by up to 4x."),
            [] { return getSettings().game.speedrunMode; });
        addOption(tr("Reset Key") + " (" + Rml::String{hotkeys::DO_RESET} + ")",
            getSettings().game.enableResetKeybind,
            tr("Press ") + Rml::String{hotkeys::DO_RESET} + tr(" to reset the game."));
    });

    add_tab(tr("Audio"), [this](Rml::Element* content) {
        auto& leftPane = add_child<Pane>(content, Pane::Type::Controlled);
        auto& rightPane = add_child<Pane>(content, Pane::Type::Uncontrolled);

        leftPane.add_section(tr("Volume"));
        leftPane.register_control(
            leftPane.add_child<NumberButton>(NumberButton::Props{
                .key = tr("Master Volume"),
                .getValue = [] { return getSettings().audio.masterVolume.getValue(); },
                .setValue =
                    [](int value) {
                        getSettings().audio.masterVolume.setValue(value);
                        config::Save();
                        audio::SetMasterVolume(audio::MasterVolumeToLinear(value / 100.0f));
                    },
                .isModified =
                    [] {
                        return getSettings().audio.masterVolume.getValue() !=
                               getSettings().audio.masterVolume.getDefaultValue();
                    },
                .max = 100,
                .suffix = "%",
            }),
            rightPane, [](Pane& pane) {
                pane.clear();
                pane.add_text(tr("Adjusts the volume of all sounds in the game."));
            });

        leftPane.add_section(tr("Effects"));
        config_bool_select(leftPane, rightPane, getSettings().audio.enableReverb,
            {
                .key = tr("Enable Reverb"),
                .helpText = tr("Enables the reverb effect in game audio."),
                .onChange = [](bool value) { audio::SetEnableReverb(value); },
            });
        config_bool_select(leftPane, rightPane, getSettings().audio.enableHrtf,
            {
                .key = tr("Enable Spatial Sound"),
                .helpText =
                    tr("Emulate surround sound via HRTF. Recommended only for use with headphones!"),
                .onChange = [](bool value) { audio::EnableHrtf = value; },
            });
        config_bool_select(leftPane, rightPane, getSettings().audio.menuSounds,
            {
                .key = tr("Dusklight Menu Sounds"),
                .helpText = tr("Play sound effects when navigating the Dusklight menu."),
            });

        leftPane.add_section(tr("Tweaks"));
        config_bool_select(leftPane, rightPane, getSettings().game.noLowHpSound,
            {
                .key = tr("No Low HP Sound"),
                .helpText = tr("Disable the beeping sound when having low health."),
            });
        config_bool_select(leftPane, rightPane, getSettings().game.midnasLamentNonStop,
            {
                .key = tr("Non-Stop Midna's Lament"),
                .helpText = tr("Prevents enemy music while Midna's Lament is playing."),
            });
    });

    add_tab(tr("Gameplay"), [this](Rml::Element* content) {
        auto& leftPane = add_child<Pane>(content, Pane::Type::Controlled);
        auto& rightPane = add_child<Pane>(content, Pane::Type::Uncontrolled);

        auto addOption = [&](const Rml::String& key, ConfigVar<bool>& value,
                             const Rml::String& helpText) {
            config_bool_select(leftPane, rightPane, value,
                {
                    .key = tr(key),
                    .helpText = tr(helpText),
                });
        };
        auto addSpeedrunDisabledOption = [&](const Rml::String& key, ConfigVar<bool>& value,
                                             const Rml::String& helpText) {
            add_speedrun_disabled_option(leftPane, rightPane, value, tr(key), tr(helpText));
        };

        leftPane.add_section(tr("General"));
        addOption(tr("Mirror Mode"), getSettings().game.enableMirrorMode,
            tr("Mirrors the world horizontally, matching the Wii version of the game."));
        addOption(tr("Minimal HUD"), getSettings().game.minimalHUD,
            tr("Disables the elements of the main HUD of the game.<br/>Useful for a more immersive ") +
            tr("experience."));
        addOption(tr("Restore Wii 1.0 Glitches"), getSettings().game.restoreWiiGlitches,
            tr("Restores patched glitches from Wii USA 1.0, the first released version."));
        addOption(tr("Enable Rotating Link Doll"), getSettings().game.enableLinkDollRotation,
            tr("Enables rotating Link in the collection menu with the C-Stick."));
        addOption(tr("Hide Owl Statue Markers"), getSettings().game.removeQuestMapMarkers,
            tr("Removes completed Owl Statue markers from the map and Minimap."));

        leftPane.add_section(tr("Difficulty"));
        leftPane.register_control(
            leftPane.add_child<NumberButton>(NumberButton::Props{
                .key = tr("Damage Multiplier"),
                .getValue = [] { return getSettings().game.damageMultiplier.getValue(); },
                .setValue =
                    [](int value) {
                        getSettings().game.damageMultiplier.setValue(value);
                        config::Save();
                    },
                .isDisabled = [] { return getSettings().game.speedrunMode; },
                .isModified =
                    [] {
                        return getSettings().game.damageMultiplier.getValue() !=
                               getSettings().game.damageMultiplier.getDefaultValue();
                    },
                .min = 1,
                .max = 8,
                .suffix = "×",
            }),
            rightPane, [](Pane& pane) {
                pane.clear();
                pane.add_text(tr("Multiplies incoming damage."));
            });
        addSpeedrunDisabledOption(
            tr("Instant Death"), getSettings().game.instantDeath, tr("Any hit will instantly kill you."));
        addSpeedrunDisabledOption(tr("No Heart Drops"), getSettings().game.noHeartDrops,
            tr("Hearts will never drop from enemies, pots, and various other places."));

        leftPane.add_section(tr("Quality of Life"));
        addOption(tr("Bigger Wallets"), getSettings().game.biggerWallets,
            tr("Wallet sizes are like in the HD version. (500, 1000, 2000)"));
        addOption(tr("Disable Rupee Cutscenes"), getSettings().game.disableRupeeCutscenes,
            tr("Rupees will not play cutscenes after you have collected them the first time."));
        addOption(tr("Faster Climbing"), getSettings().game.fastClimbing,
            tr("Quicker climbing on ladders and vines like the HD version."));
        addOption(tr("Faster Tears of Light"), getSettings().game.fastTears,
            tr("Tears of Light dropped by Shadow Insects pop out faster like the HD version."));
        addSpeedrunDisabledOption(tr("Autosave"), getSettings().game.autoSave,
            tr("Autosaves the game when going to a new area or opening a dungeon door."));
        addOption(tr("Instant Saves"), getSettings().game.instantSaves,
            tr("Skips the delay when writing to the Memory Card."));
        addOption(tr("Hold B for Instant Text"), getSettings().game.instantText,
            tr("Makes text scroll immediately by holding B."));
        addOption(tr("No Climbing Miss Animation"), getSettings().game.noMissClimbing,
            tr("Prevents Link from playing a struggle animation when grabbing ledges or ") +
            tr("climbing on vines."));
        addOption(tr("No Rupee Returns"), getSettings().game.noReturnRupees,
            tr("Always collect Rupees even if your Wallet is too full."));
        addOption(tr("No Sword Recoil"), getSettings().game.noSwordRecoil,
            tr("Link will not recoil when his sword hits walls."));
        addOption(tr("No 2nd Fish for Cat"), getSettings().game.no2ndFishForCat,
            tr("Skip needing to catch a second fish for Sera's cat."));
        addOption(tr("Show Poe Count on Map"), getSettings().game.enhancedMapMenus,
            tr("Displays collected/total number of Poe Souls for a region on the map."));
        addSpeedrunDisabledOption(tr("Sun's Song (R+X)"), getSettings().game.sunsSong,
            tr("Allows Wolf Link to howl and change the time of day."));
        addOption(tr("Quick Transform (R+Y)"), getSettings().game.enableQuickTransform,
            tr("Transform instantly by pressing R and Y simultaneously."));

        leftPane.add_section(tr("Speedrunning"));
        config_bool_select(leftPane, rightPane, getSettings().game.speedrunMode,
            {
                .key = tr("Speedrun Mode"),
                .helpText =
                    tr("Enables speedrunning options while restricting certain gameplay modifiers."),
                .onChange =
                    [](bool enabled) {
                        if (enabled) {
                            reset_for_speedrun_mode();
                        } else {
                            restore_from_speedrun_mode();
                            if (getSettings().game.liveSplitEnabled) {
                                speedrun::disconnectLiveSplit();
                            }
                        }
                        for (auto& doc : get_document_stack()) {
                            if (dynamic_cast<MenuBar*>(doc.get())) {
                                doc = std::make_unique<MenuBar>();
                                break;
                            }
                        }
                    },
            });
        config_bool_select(leftPane, rightPane, getSettings().game.liveSplitEnabled,
            {
                .key = tr("LiveSplit Connection"),
                .helpText = tr("Connect to LiveSplit server on localhost:16834. For this to work you must right click LiveSplit, and turn on Control -> Start TCP Server.") +
                tr(" To see IGT in LiveSplit you must change your comparison to Game Time."),
                .onChange =
                    [](bool enabled) {
                        if (enabled) {
                            speedrun::connectLiveSplit();
                        } else {
                            speedrun::disconnectLiveSplit();
                        }
                    },
                .isDisabled = [] { return IsMobile || !getSettings().game.speedrunMode; },
            });
        config_bool_select(leftPane, rightPane, getSettings().game.showSpeedrunRTATimer,
            {
                .key = tr("Show RTA"),
                .helpText = tr("Display the RTA timer. IGT is always visible."),
                .isDisabled = [] { return !getSettings().game.speedrunMode; },
            });
    });

    add_tab(tr("Cheats"), [this](Rml::Element* content) {
        auto& leftPane = add_child<Pane>(content, Pane::Type::Controlled);
        auto& rightPane = add_child<Pane>(content, Pane::Type::Uncontrolled);

        auto addCheat = [&](const Rml::String& key, ConfigVar<bool>& value,
                            const Rml::String& helpText) {
            add_speedrun_disabled_option(leftPane, rightPane, value, tr(key), tr(helpText));
        };

        leftPane.add_section(tr("Resources"));
        addCheat(tr("Infinite Hearts"), getSettings().game.infiniteHearts, tr("Keeps your health full."));
        addCheat(
            tr("Infinite Arrows"), getSettings().game.infiniteArrows, tr("Keeps your arrow count full."));
        addCheat(tr("Infinite Seeds"), getSettings().game.infiniteSeeds, tr("Keeps your slingshot pellets (seeds) full."));
        addCheat(tr("Infinite Bombs"), getSettings().game.infiniteBombs, tr("Keeps all bomb bags full."));
        addCheat(tr("Infinite Oil"), getSettings().game.infiniteOil, tr("Keeps your lantern oil full."));
        addCheat(tr("Infinite Oxygen"), getSettings().game.infiniteOxygen,
            tr("Keeps your underwater oxygen meter full."));
        addCheat(
            tr("Infinite Rupees"), getSettings().game.infiniteRupees, tr("Keeps your rupee count full."));
        addCheat(tr("No Item Timer"), getSettings().game.enableIndefiniteItemDrops,
            tr("Item drops such as rupees and hearts will never disappear after they drop."));

        leftPane.add_section(tr("Abilities"));
        addCheat(
            tr("Moon Jump (R+A)"), getSettings().game.moonJump, tr("Hold R and A to rise into the air."));
        addCheat(tr("Super Clawshot"), getSettings().game.superClawshot,
            tr("Extends Clawshot behavior beyond the normal game rules."));
        addCheat(tr("Always Greatspin"), getSettings().game.alwaysGreatspin,
            tr("Allows the Great Spin attack without requiring full health."));
        addCheat(tr("Fast Iron Boots"), getSettings().game.enableFastIronBoots,
            tr("Speeds up movement while heavy, including wearing the Iron Boots, holding the Ball and Chain, wearing Magic Armor without rupees, etc."));
        addCheat(tr("Can Transform Anywhere"), getSettings().game.canTransformAnywhere,
            tr("Allows transforming even if NPCs are looking."));
        addCheat(tr("Fast Roll"), getSettings().game.fastRoll,
            tr("Makes Link's roll animation and movement twice as fast."));
        addCheat(tr("Fast Spinner"), getSettings().game.fastSpinner,
            tr("Speeds up Spinner movement while holding R."));
        addCheat(tr("Free Magic Armor"), getSettings().game.freeMagicArmor,
            tr("Lets the magic armor work without consuming rupees."));
        addCheat(tr("Invincible Enemies"), getSettings().game.invincibleEnemies,
            tr("Prevents enemies from taking damage."));
    });

    add_tab(tr("Interface"), [this](Rml::Element* content) {
        auto& leftPane = add_child<Pane>(content, Pane::Type::Controlled);
        auto& rightPane = add_child<Pane>(content, Pane::Type::Uncontrolled);

        leftPane.add_section(tr("Dusklight"));
#if DUSK_CAN_OPEN_DATA_FOLDER
        leftPane.register_control(
            leftPane.add_button(tr("Open Data Folder")).on_pressed([] {
                mDoAud_seStartMenu(kSoundClick);
                data::open_data_path();
            }),
            rightPane, [](Pane& pane) {
                pane.add_text(
                    tr("Open the folder where Dusklight stores settings, saves, logs, texture ") +
                    tr("replacements, and other app data."));
            });
#endif
        leftPane.register_control(
            leftPane.add_select_button({
                .key = tr("Notifications"),
                .getValue = [] {
                    const bool ach = getSettings().game.enableAchievementToasts.getValue();
                    const bool ctl = getSettings().game.enableControllerToasts.getValue();
                    if (!ach && !ctl) {
                        return tr("Off");
                    }
                    if (ach && ctl) {
                        return tr("All");
                    }
                    return tr("Some");
                },
                .isModified = [] {
                    const auto& ach = getSettings().game.enableAchievementToasts;
                    const auto& ctl = getSettings().game.enableControllerToasts;
                    return ach.getValue() != ach.getDefaultValue() || ctl.getValue() != ctl.getDefaultValue();
                },
            }),
            rightPane, [](Pane& pane) {
                pane.clear();
                pane.add_button(tr("Select All")).on_pressed([] {
                    mDoAud_seStartMenu(kSoundItemChange);
                    getSettings().game.enableAchievementToasts.setValue(true);
                    getSettings().game.enableControllerToasts.setValue(true);
                    config::Save();
                });
                pane.add_button(tr("Select None")).on_pressed([] {
                    mDoAud_seStartMenu(kSoundItemChange);
                    getSettings().game.enableAchievementToasts.setValue(false);
                    getSettings().game.enableControllerToasts.setValue(false);
                    config::Save();
                });

                pane.add_section(tr("Types"));
                pane.add_button(
                    {
                        .text = tr("Achievements"),
                        .isSelected =
                        [] {
                            return getSettings().game.enableAchievementToasts.getValue();
                        },
                    })
                    .on_pressed([] {
                        mDoAud_seStartMenu(kSoundItemChange);
                        auto& v = getSettings().game.enableAchievementToasts;
                        v.setValue(!v.getValue());
                        config::Save();
                    });
                pane.add_button(
                    {
                        .text = tr("Missing Device"),
                        .isSelected =
                            [] { return getSettings().game.enableControllerToasts.getValue(); },
                    })
                    .on_pressed([] {
                        mDoAud_seStartMenu(kSoundItemChange);
                        auto& v = getSettings().game.enableControllerToasts;
                        v.setValue(!v.getValue());
                        config::Save();
                    });
                pane.add_rml(tr("<br/>Choose which notifications can be displayed."));
            });
#if DUSK_ENABLE_SENTRY_NATIVE
        auto& crashReporting = leftPane.add_child<BoolButton>(BoolButton::Props{
            .key = tr("Crash Reporting"),
            .getValue =
                [] { return crash_reporting::get_consent() == crash_reporting::Consent::Given; },
            .setValue = [](bool enabled) { crash_reporting::set_consent(enabled); },
            .isDisabled =
                [] {
                    return crash_reporting::get_consent() == crash_reporting::Consent::Unavailable;
                },
            .isModified = [] { return false; },
        });
        leftPane.register_control(crashReporting, rightPane, [](Pane& pane) {
            pane.clear();
            pane.add_rml(tr("Dusklight can automatically send crash reports to the developers. Crash ") +
                         tr("reports contain the following:<br/>• Operating system version<br/>• CPU ") +
                         tr("architecture<br/>• GPU model & driver version<br/>• File paths (may ") +
                         tr("include account username)<br/>• Stack trace"));
        });
#endif
        config_bool_select(leftPane, rightPane, getSettings().backend.skipPreLaunchUI,
            {
                .key = tr("Skip Dusklight Main Menu"),
                .helpText = tr("When starting Dusklight, skip the main menu and boot straight into the ") +
                            tr("game if a disc image is available."),
            });
        config_bool_select(leftPane, rightPane, getSettings().backend.showPipelineCompilation,
            {
                .key = tr("Show Pipeline Compilation"),
                .helpText = tr("Show an overlay when shaders are being compiled for your hardware."),
            });
        config_bool_select(leftPane, rightPane, getSettings().backend.checkForUpdates,
            {
                .key = tr("Check for Updates"),
                .helpText = tr("Checks GitHub releases for a new Dusklight version on startup.<br/><br/>") +
                            tr("No personal information is transmitted or collected."),
            });
#ifdef DUSK_DISCORD
        config_bool_select(leftPane, rightPane, getSettings().game.enableDiscordPresence,
            {
                .key = tr("Enable Discord Rich Presence"),
                .helpText = tr("Enable Dusklight to integrate with Discord Rich Presence. This allows Discord to show your status in-game."),
                .onChange = [](bool enabled) {
                    if (enabled) {
                        dusk::discord::initialize();
                    } else {
                        dusk::discord::shutdown();
                    }
                },
            });
#endif
        config_bool_select(leftPane, rightPane, getSettings().backend.enableAdvancedSettings,
            {
                .key = tr("Enable Advanced Settings"),
                .icon = "warning",
                .helpText = tr("Show advanced settings and debugging tools with ") +
                            tr("Shift+F1.<br/><br/><icon class=\"warning\"/> WARNING: Debugging tools ") +
                            tr("can easily break your game. Do not use on a regular save!"),
                .onChange =
                    [](bool) {
                        for (auto& doc : get_document_stack()) {
                            if (dynamic_cast<MenuBar*>(doc.get())) {
                                doc = std::make_unique<MenuBar>();
                                break;
                            }
                        }
                    },
                .isDisabled = [] { return getSettings().game.speedrunMode; },
            });
        config_bool_select(leftPane, rightPane, getSettings().game.showInputViewer,
            {
                .key = tr("Show Input Viewer"),
                .helpText = tr("Display a controller input overlay while playing."),
            });
        config_bool_select(leftPane, rightPane, getSettings().game.showInputViewerGyro,
            {
                .key = tr("Show Gyro Input Viewer"),
                .helpText = tr("Show gyro sensor values in the input viewer."),
                .isDisabled = [] { return !getSettings().game.showInputViewer; },
            });
        leftPane.add_section(tr("Game"));
        leftPane.register_control(
            leftPane.add_select_button({
                .key = tr("Menu Scaling Mode"),
                .getValue =
                    [] {
                        return tr(kMenuScalingModeLabels[static_cast<u8>(
                            getSettings().game.menuScalingMode.getValue())]);
                    },
                .isModified =
                    [] {
                        const auto& mode = getSettings().game.menuScalingMode;
                        return mode.getValue() != mode.getDefaultValue();
                    },
            }),
            rightPane, [](Pane& pane) {
                for (int i = 0; i < static_cast<int>(kMenuScalingModeLabels.size()); ++i) {
                    pane
                        .add_button({
                            .text = tr(kMenuScalingModeLabels[i]),
                            .isSelected =
                                [i] {
                                    return getSettings().game.menuScalingMode.getValue() ==
                                           static_cast<MenuScaling>(i);
                                    ;
                                },
                        })
                        .on_pressed([i] {
                            mDoAud_seStartMenu(kSoundItemChange);
                            getSettings().game.menuScalingMode.setValue(
                                static_cast<MenuScaling>(i));
                            ;
                            config::Save();
                        });
                }
                pane.add_rml(tr("<br/>Changes how the Collection and File Select menus scale to your ") +
                             tr("aspect ratio."));
            });
        config_bool_select(leftPane, rightPane, getSettings().game.hideTvSettingsScreen,
            {
                .key = tr("Skip TV Settings Screen"),
                .helpText = tr("Skips the TV calibration screen shown when loading a save."),
            });
        add_speedrun_disabled_option(leftPane, rightPane, getSettings().game.recordingMode,
            tr("Recording Mode"),
            tr("Disables the game HUD and all background music.<br/><br/>Useful for recording footage."));
    });
}

void SettingsWindow::update() {
    if (mPrelaunch && top_document() == this) {
        try_push_verification_modal(*this);
    }

    Window::update();
}

void SettingsWindow::hide(bool close) {
    config::Save();
    Window::hide(close);
}

}  // namespace dusk::ui
