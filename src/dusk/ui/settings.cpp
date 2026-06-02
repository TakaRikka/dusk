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
#include "dusk/discord_presence.hpp"
#include "graphics_tuner.hpp"
#include "m_Do/m_Do_main.h"
#include "menu_bar.hpp"
#include "modal.hpp"
#include "number_button.hpp"
#include "pane.hpp"
#include "prelaunch.hpp"
#include "i18n.hpp"
#include "ui.hpp"

#include <aurora/lib/window.hpp>
#include <SDL3/SDL_filesystem.h>
#include <fmt/format.h>

#if DUSK_ENABLE_SENTRY_NATIVE
#include "dusk/crash_reporting.h"
#endif

#include <algorithm>
#include <cctype>
#include <filesystem>

namespace dusk::ui {
namespace {

constexpr std::array kLanguageNames = {
    "English",
    "German",
    "French",
    "Spanish",
    "Italian",
};

constexpr std::array<const char*, 4> kUiLanguageIds = {
    "en",
    "zh-cn",
    "fr",
    "ja",
};

constexpr std::array<const char*, 4> kUiLanguageTokenNames = {
    "[ENGLISH]",
    "[SIMPLIFIED_CHINESE]",
    "[FRENCH]",
    "[JAPANESE]",
};

int ui_language_index(std::string_view languageId) {
    std::string lowered;
    lowered.reserve(languageId.size());
    for (const char ch : languageId) {
        lowered.push_back(static_cast<char>(std::tolower(static_cast<unsigned char>(ch))));
    }
    if (lowered == "zh-cn" || lowered == "zh-hans") {
        return 1;
    }
    if (lowered == "fr") {
        return 2;
    }
    if (lowered == "ja") {
        return 3;
    }
    return 0;
}

constexpr std::array kCardFileTypes = {
    "[CARD_IMAGE]",
    "[GCI_FOLDER]",
};

constexpr std::array kFpsOverlayCornerNames = {
    "[TOP_LEFT]",
    "[TOP_RIGHT]",
    "[BOTTOM_LEFT]",
    "[BOTTOM_RIGHT]",
};

constexpr std::array kGyroInputModeLabels = {
    "[SENSOR]",
    "[MOUSE]",
};

constexpr std::array kInterpolationModes = {
    "[OFF]",
    "[CAPPED]",
    "[UNLIMITED]",
};

constexpr std::array kMenuScalingModeLabels = {
    "[GAMECUBE]",
    "[WII]",
    "[DUSKLIGHT]",
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
        // Do not expose NULL
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
        return "(none)";
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
        const Rml::String rml = "<span class=\"data-folder-current\">[CURRENT_DATA_FOLDER]<br/>" +
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
        .title = "[DATA_FOLDER_NOT_CHANGED]",
        .bodyRml = escape(message),
        .actions =
            {
                ModalAction{
                    .label = "[OK]",
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
    "[CONFIGURE_THE_RESOLUTION_USED_FOR_RENDERING_THE_GAME_HIGHER_VALUES_ARE_MORE_DE]";
const Rml::String kShadowResolutionHelpText =
    "[CONFIGURE_THE_SHADOW_MAP_RESOLUTION_HIGHER_VALUES_IMPROVE_SHADOW_QUALITY_BUT]";
const Rml::String kResamplerHelpText =
    "[CONFIGURE_THE_SAMPLING_METHOD_USED_WHEN_SCALING_THE_INTERNAL_RESOLUTION_FOR_FINAL_PRESENTATION]";
const Rml::String kBloomHelpText =
    "[CONFIGURE_THE_POST_PROCESSING_BLOOM_EFFECT_CLASSIC_USES_THE_ORIGINAL_BLO]";
const Rml::String kBloomBrightnessHelpText =
    "[CONFIGURE_BLOOM_INTENSITY_HIGHER_VALUES_MAKE_BRIGHT_AREAS_GLOW_MORE]";
const Rml::String kDepthOfFieldHelpText =
    "[CONFIGURE_THE_POST_PROCESSING_DEPTH_OF_FIELD_EFFECT_CLASSIC_USES_THE_ORIGINAL]";
const Rml::String kUnlockFramerateHelpText =
    "[USES_INTER_FRAME_INTERPOLATION_TO_ENABLE_HIGHER_FRAME_RATES_MAY_INTRODUCE_MINOR]";
const Rml::String kTextureReplacementHelpText =
    "[ENABLE_INSTALLED_TEXTURE_REPLACEMENTS]";

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
        pane.add_rml(helpText);
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
        add_tab("[PRELAUNCH]", [this](Rml::Element* content) {
            auto& leftPane = add_child<Pane>(content, Pane::Type::Controlled);
            auto& rightPane = add_child<Pane>(content, Pane::Type::Uncontrolled);

            leftPane.register_control(
                leftPane
                    .add_select_button({
                        .key = "[DISC_IMAGE]",
                        .getValue =
                            [] {
                                const auto& path = prelaunch_state().configuredDiscPath;
                                std::string display;
                                if (path.empty()) {
                                    display = "(none)";
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
                    pane.add_rml("[SET_THE_DISC_IMAGE_THAT_DUSKLIGHT_USES_TO_LAUNCH_THE_GAME_CHANGES_REQUIR]");
                });
#if DUSK_CAN_CHANGE_DATA_FOLDER
            leftPane.register_control(
                leftPane.add_select_button({
                    .key = "[DATA_FOLDER]",
                    .getValue = [] { return configured_data_path_display_name(); },
                    .isModified = [] { return data::is_data_path_restart_pending(); },
                }),
                rightPane, [](Pane& pane) {
                    pane.add_text("[THE_DATA_FOLDER_IS_WHERE_DUSKLIGHT_STORES_SETTINGS_SAVES_LOGS_TEXTURE_RE]");
                    pane.add_child<DataFolderPathText>();
#if DUSK_CAN_OPEN_DATA_FOLDER
                    pane.add_button("[OPEN_DATA_FOLDER]").on_pressed([] {
                        if (data::open_data_path()) {
                            mDoAud_seStartMenu(kSoundClick);
                        }
                    });
#endif
                    pane.add_button("[CHANGE_DATA_FOLDER]").on_pressed([] {
                        const auto defaultLocation =
                            io::fs_path_to_string(data::configured_data_path());
                        ShowFolderSelect(&data_folder_dialog_callback, nullptr,
                            aurora::window::get_sdl_window(),
                            defaultLocation.empty() ? nullptr : defaultLocation.c_str());
                    });
#if defined(_WIN32)
                    pane.add_button("[PORTABLE_MODE]").on_pressed([] {
                        if (data::set_portable_data_path()) {
                            mDoAud_seStartMenu(kSoundItemChange);
                        }
                    });
#endif
                    pane.add_button({
                        .text = "[RESET_TO_DEFAULT]",
                        .isDisabled = [] { return data::is_default_data_path(); },
                    }).on_pressed([] {
                        if (data::reset_data_path()) {
                            mDoAud_seStartMenu(kSoundItemChange);
                        }
                    });
                    pane.add_rml("[DATA_WILL_BE_MIGRATED_AUTOMATICALLY_ON_RESTART]");
                });
#endif
            leftPane.register_control(
                leftPane.add_select_button({
                    .key = "[LANGUAGE]",
                    .getValue =
                        [] {
                            const auto& state = prelaunch_state();
                            if (!state.configuredDiscCanLaunch || !state.configuredDiscInfo.isPal) {
                                return kLanguageNames[0];
                            }
                            const u8 idx = static_cast<u8>(getSettings().game.language.getValue());
                            return kLanguageNames[idx];
                        },
                    .isDisabled =
                        [] {
                            const auto& state = prelaunch_state();
                            return !state.configuredDiscCanLaunch ||
                                   !state.configuredDiscInfo.isPal;
                        },
                    .isModified =
                        [] {
                            return getSettings().game.language.getValue() !=
                                   prelaunch_state().initialLanguage;
                        },
                }),
                rightPane, [](Pane& pane) {
                    for (int i = 0; i < kLanguageNames.size(); i++) {
                        pane.add_button({
                                            .text = i == 0 ? "[ENGLISH]" :
                                                            i == 1 ? "[GERMAN]" :
                                                            i == 2 ? "[FRENCH]" :
                                                            i == 3 ? "[SPANISH]" :
                                                                     "[ITALIAN]",
                                            .isSelected =
                                                [i] {
                                                    return getSettings().game.language.getValue() ==
                                                           static_cast<GameLanguage>(i);
                                                },
                                        })
                            .on_pressed([i] {
                                mDoAud_seStartMenu(kSoundItemChange);
                                getSettings().game.language.setValue(static_cast<GameLanguage>(i));
                                config::Save();
                            });
                    }
                    pane.add_rml("[CHANGES_REQUIRE_A_RESTART]");
                });
            leftPane.register_control(
                leftPane.add_select_button({
                    .key = "[GRAPHICS_BACKEND]",
                    .getValue = [] { return Rml::String{backend_name(configured_backend())}; },
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
                                .text = Rml::String{backend_name(backend)},
                                .isSelected = [backend] { return configured_backend() == backend; },
                            })
                            .on_pressed([backend] {
                                mDoAud_seStartMenu(kSoundItemChange);
                                getSettings().backend.graphicsBackend.setValue(
                                    std::string{backend_id(backend)});
                                config::Save();
                            });
                    }
                    pane.add_rml("[CHANGES_REQUIRE_A_RESTART]");
                });
            leftPane.register_control(
                leftPane.add_select_button({
                    .key = "[SAVE_FILE_TYPE]",
                    .getValue =
                        [] {
                            return kCardFileTypes[getSettings().backend.cardFileType.getValue()];
                        },
                    .isModified =
                        [] {
                            return getSettings().backend.cardFileType.getValue() !=
                                   prelaunch_state().initialCardFileType;
                        },
                }),
                rightPane, [](Pane& pane) {
                    for (int i = 0; i < kCardFileTypes.size(); i++) {
                        pane
                            .add_button({
                                .text = kCardFileTypes[i],
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

    add_tab("[VIDEO]", [this](Rml::Element* content) {
        auto& leftPane = add_child<Pane>(content, Pane::Type::Controlled);
        auto& rightPane = add_child<Pane>(content, Pane::Type::Uncontrolled);

        leftPane.add_section("[DISPLAY]");

        leftPane.register_control(leftPane.add_button("[TOGGLE_FULLSCREEN]").on_pressed([] {
            mDoAud_seStartMenu(kSoundItemChange);
            getSettings().video.enableFullscreen.setValue(!getSettings().video.enableFullscreen);
            VISetWindowFullscreen(getSettings().video.enableFullscreen);
            config::Save();
        }),
            rightPane, [](Pane& pane) { pane.clear(); });
        leftPane.register_control(leftPane.add_button("[RESTORE_DEFAULT_WINDOW_SIZE]").on_pressed([] {
            mDoAud_seStartMenu(kSoundItemChange);
            getSettings().video.enableFullscreen.setValue(false);
            VISetWindowFullscreen(false);
            VISetWindowSize(FB_WIDTH * 2, FB_HEIGHT * 2);
            VICenterWindow();
        }),
            rightPane, [](Pane& pane) { pane.clear(); });
        config_bool_select(leftPane, rightPane, getSettings().video.enableVsync,
            {
                .key = "[ENABLE_VSYNC]",
                .helpText = "[SYNCHRONIZES_THE_FRAME_RATE_TO_YOUR_MONITOR_S_REFRESH_RATE]",
                .onChange = [](bool value) { aurora_enable_vsync(value); },
            });
        config_bool_select(leftPane, rightPane, getSettings().video.lockAspectRatio,
            {
                .key = "[LOCK_4_3_ASPECT_RATIO]",
                .helpText = "[LOCK_THE_GAME_S_ASPECT_RATIO_TO_THE_ORIGINAL]",
                .onChange =
                    [](bool value) {
                        AuroraSetViewportPolicy(
                            value ? AURORA_VIEWPORT_FIT : AURORA_VIEWPORT_STRETCH);
                    },
            });
        config_bool_select(leftPane, rightPane, getSettings().game.pauseOnFocusLost,
            {
                .key = "[PAUSE_ON_FOCUS_LOST]",
                .helpText = "[PAUSE_THE_GAME_WHEN_WINDOW_FOCUS_IS_LOST]",
                .onChange = [](bool value) { aurora_set_pause_on_focus_lost(value); },
                .isDisabled = [] { return IsMobile || getSettings().game.speedrunMode; },
            });
        leftPane.register_control(
            leftPane.add_select_button({
                .key = "[SHOW_FPS_COUNTER]",
                .getValue =
                    [] {
                        if (!getSettings().video.enableFpsOverlay.getValue()) {
                            return Rml::String{"Off"};
                        }
                        const int idx = getSettings().video.fpsOverlayCorner.getValue();
                        return Rml::String{kFpsOverlayCornerNames[idx]};
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
                            .text = "[OFF]",
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
                                .text = kFpsOverlayCornerNames[i],
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
                    "[DISPLAY_THE_CURRENT_FRAMERATE_IN_A_CORNER_OF_THE_SCREEN_WHILE_PLAYING]");
            });
        leftPane.add_section("[RESOLUTION]");
        graphics_tuner_control(*this, leftPane, rightPane,
            getSettings().game.internalResolutionScale,
            GraphicsTunerProps{
                .option = GraphicsOption::InternalResolution,
                .title = "[INTERNAL_RESOLUTION]",
                .helpText = kInternalResolutionHelpText,
                .valueMin = 0,
                .valueMax = 12,
                .defaultValue = 0,
            }, mPrelaunch);
        graphics_tuner_control(*this, leftPane, rightPane,
            getSettings().game.shadowResolutionMultiplier,
            GraphicsTunerProps{
                .option = GraphicsOption::ShadowResolution,
                .title = "[SHADOW_RESOLUTION]",
                .helpText = kShadowResolutionHelpText,
                .valueMin = 1,
                .valueMax = 8,
                .defaultValue = 1,
            }, mPrelaunch);
        graphics_tuner_control(*this, leftPane, rightPane, getSettings().game.resampler,
            GraphicsTunerProps{
                .option = GraphicsOption::Resampler,
                .title = "[OUTPUT_RESAMPLING]",
                .helpText = kResamplerHelpText,
                .valueMin = static_cast<int>(Resampler::Bilinear),
                .valueMax = static_cast<int>(Resampler::Area),
                .defaultValue = static_cast<int>(Resampler::Bilinear),
            }, mPrelaunch);

        leftPane.add_section("[POST_PROCESSING]");
        graphics_tuner_control(*this, leftPane, rightPane, getSettings().game.bloomMode,
            GraphicsTunerProps{
                .option = GraphicsOption::BloomMode,
                .title = "[BLOOM]",
                .helpText = kBloomHelpText,
                .valueMin = static_cast<int>(BloomMode::Off),
                .valueMax = static_cast<int>(BloomMode::Dusk),
                .defaultValue = static_cast<int>(BloomMode::Classic),
            }, mPrelaunch);
        graphics_tuner_control(*this, leftPane, rightPane, getSettings().game.bloomMultiplier,
            GraphicsTunerProps{
                .option = GraphicsOption::BloomMultiplier,
                .title = "[BLOOM_BRIGHTNESS]",
                .helpText = kBloomBrightnessHelpText,
                .valueMin = 0,
                .valueMax = 100,
                .defaultValue = 100,
                .step = 10,
            },
            mPrelaunch);
        graphics_tuner_control(*this, leftPane, rightPane, getSettings().game.depthOfFieldMode,
            GraphicsTunerProps{
                .option = GraphicsOption::DepthOfFieldMode,
                .title = "[DEPTH_OF_FIELD]",
                .helpText = kDepthOfFieldHelpText,
                .valueMin = static_cast<int>(DepthOfFieldMode::Off),
                .valueMax = static_cast<int>(DepthOfFieldMode::Dusk),
                .defaultValue = static_cast<int>(DepthOfFieldMode::Classic),
            },
            mPrelaunch);

        leftPane.add_section("[RENDERING]");
        graphics_tuner_control(*this, leftPane, rightPane,
            getSettings().game.enableTextureReplacements,
            GraphicsTunerProps{
                .option = GraphicsOption::TextureReplacements,
                .title = "[USE_TEXTURE_PACK]",
                .helpText = kTextureReplacementHelpText,
                .valueMin = static_cast<int>(false),
                .valueMax = static_cast<int>(true),
                .defaultValue = static_cast<int>(false),
            },
            mPrelaunch);
        leftPane.register_control(
            leftPane.add_select_button({
                .key = "[UNLOCK_FRAMERATE]",
                .getValue =
                    [] {
                        return kInterpolationModes[static_cast<u8>(
                            getSettings().game.enableFrameInterpolation.getValue())];
                    },
                .isModified =
                    [] {
                        return getSettings().game.enableFrameInterpolation.getValue() !=
                               getSettings().game.enableFrameInterpolation.getDefaultValue();
                    },
            }),
            rightPane, [](Pane& pane) {
                for (int i = 0; i < kInterpolationModes.size(); i++) {
                    pane.add_button({
                            .text = kInterpolationModes[i],
                            .isSelected =
                                [i] {
                                    return getSettings().game.enableFrameInterpolation.getValue() ==
                                           static_cast<FrameInterpMode>(i);
                                },
                        })
                        .on_pressed([i] {
                            mDoAud_seStartMenu(kSoundItemChange);
                            getSettings().game.enableFrameInterpolation.setValue(
                                static_cast<FrameInterpMode>(i));
                            config::Save();
                        });
                }
                pane.add_rml(kUnlockFramerateHelpText);
            });
        config_int_select(leftPane, rightPane, getSettings().video.maxFrameRate,
            "[FRAMERATE_CAP]", "[LIMIT_THE_FRAMERATE_TO_THE_SPECIFIED_VALUE]", 30, 540, 1,
            [] {
                return getSettings().game.enableFrameInterpolation.getValue() !=
                       FrameInterpMode::Capped;
            });
        config_bool_select(leftPane, rightPane, getSettings().game.enableMapBackground,
            {
                .key = "[ENABLE_MINI_MAP_SHADOWS]",
                .helpText = "[RENDER_A_THICK_SHADOW_AROUND_THE_MINI_MAP_MAY_IMPACT_PERFORMANCE]",
            });
        config_bool_select(leftPane, rightPane, getSettings().game.disableCutscenePillarboxing,
            {
                .key = "[DISABLE_CUTSCENE_PILLARBOXING]",
                .helpText = "[DISABLE_BLACK_BARS_ON_THE_LEFT_AND_RIGHT_SIDES_OF_THE_SCREEN_DURING_SOME]",
            });
    });

    add_tab("[INPUT]", [this](Rml::Element* content) {
        auto& leftPane = add_child<Pane>(content, Pane::Type::Controlled);
        auto& rightPane = add_child<Pane>(content, Pane::Type::Uncontrolled);

        auto addOption = [&](const Rml::String& key, ConfigVar<bool>& value,
                             const Rml::String& helpText, std::function<bool()> isDisabled = {}) {
            config_bool_select(leftPane, rightPane, value,
                {
                    .key = key,
                    .helpText = helpText,
                    .isDisabled = std::move(isDisabled),
                });
        };

        leftPane.add_section("[CONTROLLER]");
        leftPane.register_control(leftPane.add_button("[CONFIGURE_CONTROLLER]").on_pressed([this] {
            push(std::make_unique<ControllerConfigWindow>(mPrelaunch));
        }),
            rightPane, [](Pane& pane) {
                pane.clear();
                pane.add_text("[OPEN_CONTROLLER_BINDING_CONFIGURATION]");
            });
        config_bool_select(leftPane, rightPane, getSettings().game.allowBackgroundInput,
            {
                .key = "[ALLOW_BACKGROUND_INPUT]",
                .helpText = "[ALLOW_CONTROLLER_INPUT_EVEN_WHEN_THE_GAME_WINDOW_IS_NOT_FOCUSED]",
                .onChange = [](bool value) { aurora_set_background_input(value); },
            });

        leftPane.add_section("[CAMERA]");
        addOption("[FREE_CAMERA]", getSettings().game.freeCamera,
            "[ENABLES_TWIN_STICK_CAMERA_CONTROL_LETTING_THE_C_STICK_MOVE_THE_CAMERA_VE]");
        addOption("[INVERT_CAMERA_X_AXIS]", getSettings().game.invertCameraXAxis,
            "[INVERT_HORIZONTAL_CAMERA_MOVEMENT]");
        addOption("[INVERT_CAMERA_Y_AXIS]", getSettings().game.invertCameraYAxis,
            "[INVERT_VERTICAL_CAMERA_MOVEMENT_WHEN_FREE_CAMERA_IS_ENABLED]",
            [] { return !getSettings().game.freeCamera; });
        config_percent_select(leftPane, rightPane, getSettings().game.freeCameraXSensitivity,
            "[FREE_CAMERA_X_SENSITIVITY]", "[ADJUSTS_TWIN_STICK_CAMERA_X_AXIS_SENSITIVITY]", 50, 200, 5,
            [] { return !getSettings().game.freeCamera; });
        config_percent_select(leftPane, rightPane, getSettings().game.freeCameraYSensitivity,
            "[FREE_CAMERA_Y_SENSITIVITY]", "[ADJUSTS_TWIN_STICK_CAMERA_Y_AXIS_SENSITIVITY]", 50, 200, 5,
            [] { return !getSettings().game.freeCamera; });
        addOption("[INVERT_FIRST_PERSON_X_AXIS]", getSettings().game.invertFirstPersonXAxis,
            "[INVERT_HORIZONTAL_MOVEMENT_WHILE_AIMING_WITH_ITEMS_OR_FIRST_PERSON_CAMERA]");
        addOption("[INVERT_FIRST_PERSON_Y_AXIS]", getSettings().game.invertFirstPersonYAxis,
            "[INVERT_VERTICAL_MOVEMENT_WHILE_AIMING_WITH_ITEMS_OR_FIRST_PERSON_CAMERA]");

        leftPane.add_section("[GYRO]");
        leftPane.register_control(
            leftPane.add_select_button({
                .key = "[GYRO_INPUT_METHOD]",
                .getValue =
                    [] {
                        const auto mode = getSettings().game.gyroMode.getValue();
                        const auto idx = static_cast<size_t>(mode);
                        return Rml::String{kGyroInputModeLabels[idx]};
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
                            .text = Rml::String{kGyroInputModeLabels[i]},
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
                    "[SENSOR_READS_MOTION_DIRECTLY_FROM_A_SUPPORTED_CONTROLLER_S_GYRO_VIA_SDL]");
            });
        addOption("[GYRO_AIM]", getSettings().game.enableGyroAim,
            "[ENABLES_GYRO_CONTROLS_WHILE_IN_LOOK_MODE_AIMING_A_HAWK_AND_AIMING_SUPPOR]");
        addOption("[GYRO_ROLLGOAL]", getSettings().game.enableGyroRollgoal,
            "[ENABLES_GYRO_CONTROLS_FOR_ROLLGOAL_IN_HENA_S_CABIN]",
            [] { return getSettings().game.gyroMode.getValue() == GyroMode::Mouse; });
        config_percent_select(leftPane, rightPane, getSettings().game.gyroSensitivityY,
            "[GYRO_PITCH_SENSITIVITY]", "[CONTROLS_VERTICAL_GYRO_AIMING_SENSITIVITY]", 25, 400, 5,
            [] { return !gyro_enabled(); });
        config_percent_select(leftPane, rightPane, getSettings().game.gyroSensitivityX,
            "[GYRO_YAW_SENSITIVITY]", "[CONTROLS_HORIZONTAL_GYRO_AIMING_SENSITIVITY]", 25, 400, 5,
            [] { return !gyro_enabled(); });
        config_percent_select(leftPane, rightPane, getSettings().game.gyroSensitivityRollgoal,
            "[ROLLGOAL_SENSITIVITY]", "[CONTROLS_HOW_STRONGLY_GYRO_INPUT_TILTS_THE_ROLLGOAL_TABLE]",
            25, 400, 5,
            [] {
                return !getSettings().game.enableGyroRollgoal ||
                       getSettings().game.gyroMode.getValue() == GyroMode::Mouse;
            });
        config_percent_select(leftPane, rightPane, getSettings().game.gyroDeadband, "[GYRO_DEADBAND]",
            "[IGNORES_SMALL_GYRO_MOVEMENT_TO_REDUCE_DRIFT_AND_JITTER]", 0, 50, 1,
            [] { return !gyro_enabled(); });
        config_percent_select(leftPane, rightPane, getSettings().game.gyroSmoothing,
            "[GYRO_SMOOTHING]", "[HIGHER_VALUES_SMOOTH_GYRO_INPUT_OVER_TIME]", 0, 100, 1,
            [] { return !gyro_enabled(); });
        addOption("[INVERT_GYRO_PITCH]", getSettings().game.gyroInvertPitch,
            "[INVERT_VERTICAL_GYRO_AIMING]", [] { return !gyro_enabled(); });
        addOption("[INVERT_GYRO_YAW]", getSettings().game.gyroInvertYaw,
            "[INVERT_HORIZONTAL_GYRO_AIMING]", [] { return !gyro_enabled(); });

        leftPane.add_section("[MOUSE]");
        addOption("[MOUSE_AIM]", getSettings().game.enableMouseAim,
            "[ENABLES_MOUSE_INPUT_WHILE_IN_LOOK_MODE_AIMING_A_HAWK_AND_AIMING_SUPPORTED_ITEMS]");
        addOption("[MOUSE_CAMERA]", getSettings().game.enableMouseCamera,
            "[ENABLES_MOUSE_INPUT_FOR_CONTROLLING_THE_THIRD_PERSON_CAMERA]");
        config_percent_select(leftPane, rightPane, getSettings().game.mouseAimSensitivity,
            "[MOUSE_AIM_SENSITIVITY]", "[CONTROLS_MOUSE_AIM_SENSITIVITY]", 25, 400, 5,
            [] { return !getSettings().game.enableMouseAim; });
        config_percent_select(leftPane, rightPane, getSettings().game.mouseCameraSensitivity,
            "[MOUSE_CAMERA_SENSITIVITY]", "[CONTROLS_MOUSE_CAMERA_SENSITIVITY]", 25, 400, 5,
            [] { return !getSettings().game.enableMouseCamera; });
        addOption("[INVERT_MOUSE_Y]", getSettings().game.invertMouseY,
            "[INVERT_VERTICAL_MOUSE_CONTROL_FOR_BOTH_AIMING_AND_CAMERA]",
            [] { return !getSettings().game.enableMouseAim || !getSettings().game.enableMouseCamera; });

        leftPane.add_section("[GAMEPLAY]");
        addOption("[INVERT_AIR_SWIM_X_AXIS]", getSettings().game.invertAirSwimX,
            "[INVERT_HORIZONTAL_MOVEMENT_WHILE_FLYING_OR_SWIMMING]");
        addOption("[INVERT_AIR_SWIM_Y_AXIS]", getSettings().game.invertAirSwimY,
            "[INVERT_VERTICAL_MOVEMENT_WHILE_FLYING_OR_SWIMMING]");
        addOption("[SWAP_DIRECT_SELECT_INPUT]", getSettings().game.swapDirectSelect,
            "[SWAP_THE_CONTROLS_FOR_USING_DIRECT_SELECT_ON_THE_ITEM_WHEEL_MAKING_DIRECT]");

        leftPane.add_section("[TOOLS]");
        addOption("[TURBO_KEY]", getSettings().game.enableTurboKeybind,
            "[HOLD_TAB_TO_INCREASE_GAME_SPEED_BY_UP_TO_4X]",
            [] { return getSettings().game.speedrunMode; });
        addOption(Rml::String{"[RESET_KEY] ("} + Rml::String{hotkeys::DO_RESET} + ")",
            getSettings().game.enableResetKeybind,
            "[PRESS] " + Rml::String{hotkeys::DO_RESET} + " [TO_RESET_THE_GAME]");
    });

    add_tab("[AUDIO]", [this](Rml::Element* content) {
        auto& leftPane = add_child<Pane>(content, Pane::Type::Controlled);
        auto& rightPane = add_child<Pane>(content, Pane::Type::Uncontrolled);

        // TODO: Individual sliders for Main Music, Sub Music, Sound Effects, and Fanfare.
        leftPane.add_section("[VOLUME]");
        leftPane.register_control(
            leftPane.add_child<NumberButton>(NumberButton::Props{
                .key = "[MASTER_VOLUME]",
                .getValue = [] { return getSettings().audio.masterVolume.getValue(); },
                .setValue =
                    [](int value) {
                        getSettings().audio.masterVolume.setValue(value);
                        config::Save();
                        audio::SetMasterVolume(value / 100.f);
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
                pane.add_text("[ADJUSTS_THE_VOLUME_OF_ALL_SOUNDS_IN_THE_GAME]");
            });

        leftPane.add_section("[EFFECTS]");
        config_bool_select(leftPane, rightPane, getSettings().audio.enableReverb,
            {
                .key = "[ENABLE_REVERB]",
                .helpText = "[ENABLES_THE_REVERB_EFFECT_IN_GAME_AUDIO]",
                .onChange = [](bool value) { audio::SetEnableReverb(value); },
            });
        config_bool_select(leftPane, rightPane, getSettings().audio.enableHrtf,
            {
                .key = "[ENABLE_SPATIAL_SOUND]",
                .helpText = "[EMULATE_SURROUND_SOUND_VIA_HRTF_RECOMMENDED_ONLY_FOR_USE_WITH_HEADPHONES]",
                .onChange = [](bool value) { audio::EnableHrtf = value; },
            });
        config_bool_select(leftPane, rightPane, getSettings().audio.menuSounds,
            {
                .key = "[DUSKLIGHT_MENU_SOUNDS]",
                .helpText = "[PLAY_SOUND_EFFECTS_WHEN_NAVIGATING_THE_DUSKLIGHT_MENU]",
            });

        leftPane.add_section("[TWEAKS]");
        config_bool_select(leftPane, rightPane, getSettings().game.noLowHpSound,
            {
                .key = "[NO_LOW_HP_SOUND]",
                .helpText = "[DISABLE_THE_BEEPING_SOUND_WHEN_HAVING_LOW_HEALTH]",
            });
        config_bool_select(leftPane, rightPane, getSettings().game.midnasLamentNonStop,
            {
                .key = "[NON_STOP_MIDNA_S_LAMENT]",
                .helpText = "[PREVENTS_ENEMY_MUSIC_WHILE_MIDNA_S_LAMENT_IS_PLAYING]",
            });
    });

    add_tab("[GAMEPLAY]", [this](Rml::Element* content) {
        auto& leftPane = add_child<Pane>(content, Pane::Type::Controlled);
        auto& rightPane = add_child<Pane>(content, Pane::Type::Uncontrolled);

        auto addOption = [&](const Rml::String& key, ConfigVar<bool>& value,
                             const Rml::String& helpText) {
            config_bool_select(leftPane, rightPane, value,
                {
                    .key = key,
                    .helpText = helpText,
                });
        };
        auto addSpeedrunDisabledOption = [&](const Rml::String& key, ConfigVar<bool>& value,
                                             const Rml::String& helpText) {
            add_speedrun_disabled_option(leftPane, rightPane, value, key, helpText);
        };

        leftPane.add_section("[GENERAL]");
        addOption("[MIRROR_MODE]", getSettings().game.enableMirrorMode,
            "[MIRRORS_THE_WORLD_HORIZONTALLY_MATCHING_THE_WII_VERSION_OF_THE_GAME]");
        addOption("[MINIMAL_HUD]", getSettings().game.minimalHUD,
            "[DISABLES_THE_ELEMENTS_OF_THE_MAIN_HUD_OF_THE_GAME_USEFUL_FOR_A_MORE_IMME]");
        addOption("[RESTORE_WII_1_0_GLITCHES]", getSettings().game.restoreWiiGlitches,
            "[RESTORES_PATCHED_GLITCHES_FROM_WII_USA_1_0_THE_FIRST_RELEASED_VERSION]");
        addOption("[ENABLE_ROTATING_LINK_DOLL]", getSettings().game.enableLinkDollRotation,
            "[ENABLES_ROTATING_LINK_IN_THE_COLLECTION_MENU_WITH_THE_C_STICK]");
        addOption("[HIDE_OWL_STATUE_MARKERS]", getSettings().game.removeQuestMapMarkers,
            "[REMOVES_COMPLETED_OWL_STATUE_MARKERS_FROM_THE_MAP_AND_MINIMAP]");

        leftPane.add_section("[DIFFICULTY]");
        leftPane.register_control(
            leftPane.add_child<NumberButton>(NumberButton::Props{
                .key = "[DAMAGE_MULTIPLIER]",
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
                pane.add_text("[MULTIPLIES_INCOMING_DAMAGE]");
            });
        addSpeedrunDisabledOption(
            "[INSTANT_DEATH]", getSettings().game.instantDeath, "[ANY_HIT_WILL_INSTANTLY_KILL_YOU]");
        addSpeedrunDisabledOption("[NO_HEART_DROPS]", getSettings().game.noHeartDrops,
            "[HEARTS_WILL_NEVER_DROP_FROM_ENEMIES_POTS_AND_VARIOUS_OTHER_PLACES]");

        leftPane.add_section("[QUALITY_OF_LIFE]");
        addOption("[BIGGER_WALLETS]", getSettings().game.biggerWallets,
            "[WALLET_SIZES_ARE_LIKE_IN_THE_HD_VERSION_500_1000_2000]");
        addOption("[DISABLE_RUPEE_CUTSCENES]", getSettings().game.disableRupeeCutscenes,
            "[RUPEES_WILL_NOT_PLAY_CUTSCENES_AFTER_YOU_HAVE_COLLECTED_THEM_THE_FIRST_T]");
        addOption("[FASTER_CLIMBING]", getSettings().game.fastClimbing,
            "[QUICKER_CLIMBING_ON_LADDERS_AND_VINES_LIKE_THE_HD_VERSION]");
        addOption("[FASTER_TEARS_OF_LIGHT]", getSettings().game.fastTears,
            "[TEARS_OF_LIGHT_DROPPED_BY_SHADOW_INSECTS_POP_OUT_FASTER_LIKE_THE_HD_VERS]");
        addSpeedrunDisabledOption("[AUTOSAVE]", getSettings().game.autoSave,
            "[AUTOSAVES_THE_GAME_WHEN_GOING_TO_A_NEW_AREA_OPENING_A_DUNGEON_DOOR_OR_GE]");
        addOption("[INSTANT_SAVES]", getSettings().game.instantSaves,
            "[SKIPS_THE_DELAY_WHEN_WRITING_TO_THE_MEMORY_CARD]");
        addOption("[HOLD_B_FOR_INSTANT_TEXT]", getSettings().game.instantText,
            "[MAKES_TEXT_SCROLL_IMMEDIATELY_BY_HOLDING_B]");
        addOption("[NO_CLIMBING_MISS_ANIMATION]", getSettings().game.noMissClimbing,
            "[PREVENTS_LINK_FROM_PLAYING_A_STRUGGLE_ANIMATION_WHEN_GRABBING_LEDGES_OR]");
        addOption("[NO_RUPEE_RETURNS]", getSettings().game.noReturnRupees,
            "[ALWAYS_COLLECT_RUPEES_EVEN_IF_YOUR_WALLET_IS_TOO_FULL]");
        addOption("[NO_SWORD_RECOIL]", getSettings().game.noSwordRecoil,
            "[LINK_WILL_NOT_RECOIL_WHEN_HIS_SWORD_HITS_WALLS]");
        addOption("[NO_2ND_FISH_FOR_CAT]", getSettings().game.no2ndFishForCat,
            "[SKIP_NEEDING_TO_CATCH_A_SECOND_FISH_FOR_SERA_S_CAT]");
        addOption("[SHOW_POE_COUNT_ON_MAP]", getSettings().game.enhancedMapMenus,
            "[DISPLAYS_COLLECTED_TOTAL_NUMBER_OF_POE_SOULS_FOR_A_REGION_ON_THE_MAP]");
        addSpeedrunDisabledOption("[SUN_S_SONG_R_X]", getSettings().game.sunsSong,
            "[ALLOWS_WOLF_LINK_TO_HOWL_AND_CHANGE_THE_TIME_OF_DAY]");
        addOption("[QUICK_TRANSFORM_R_Y]", getSettings().game.enableQuickTransform,
            "[TRANSFORM_INSTANTLY_BY_PRESSING_R_AND_Y_SIMULTANEOUSLY]");

        leftPane.add_section("[SPEEDRUNNING]");
        config_bool_select(leftPane, rightPane, getSettings().game.speedrunMode,
            {
                .key = "[SPEEDRUN_MODE]",
                .helpText = "[ENABLES_SPEEDRUNNING_OPTIONS_WHILE_RESTRICTING_CERTAIN_GAMEPLAY_MODIFIER]",
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
                .key = "[LIVESPLIT_CONNECTION]",
                .helpText = "[CONNECT_TO_LIVESPLIT_SERVER_ON_LOCALHOST_16834_FOR_THIS_TO_WORK_YOU]",
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
                .key = "[SHOW_RTA]",
                .helpText = "[DISPLAY_THE_RTA_TIMER_IGT_IS_ALWAYS_VISIBLE]",
                .isDisabled = [] { return !getSettings().game.speedrunMode; },
            });
    });

    add_tab("[CHEATS]", [this](Rml::Element* content) {
        auto& leftPane = add_child<Pane>(content, Pane::Type::Controlled);
        auto& rightPane = add_child<Pane>(content, Pane::Type::Uncontrolled);

        auto addCheat = [&](const Rml::String& key, ConfigVar<bool>& value,
                            const Rml::String& helpText) {
            add_speedrun_disabled_option(leftPane, rightPane, value, key, helpText);
        };

        leftPane.add_section("[RESOURCES]");
        addCheat("[CHEAT_INFINITE_HEARTS]", getSettings().game.infiniteHearts,
            "[CHEAT_INFINITE_HEARTS_HELP]");
        addCheat("[CHEAT_INFINITE_ARROWS]", getSettings().game.infiniteArrows,
            "[CHEAT_INFINITE_ARROWS_HELP]");
        addCheat("[CHEAT_INFINITE_SEEDS]", getSettings().game.infiniteSeeds,
            "[CHEAT_INFINITE_SEEDS_HELP]");
        addCheat("[CHEAT_INFINITE_BOMBS]", getSettings().game.infiniteBombs,
            "[CHEAT_INFINITE_BOMBS_HELP]");
        addCheat("[CHEAT_INFINITE_OIL]", getSettings().game.infiniteOil,
            "[CHEAT_INFINITE_OIL_HELP]");
        addCheat("[CHEAT_INFINITE_OXYGEN]", getSettings().game.infiniteOxygen,
            "[CHEAT_INFINITE_OXYGEN_HELP]");
        addCheat("[CHEAT_INFINITE_RUPEES]", getSettings().game.infiniteRupees,
            "[CHEAT_INFINITE_RUPEES_HELP]");
        addCheat("[CHEAT_NO_ITEM_TIMER]", getSettings().game.enableIndefiniteItemDrops,
            "[CHEAT_NO_ITEM_TIMER_HELP]");

        leftPane.add_section("[ABILITIES]");
        addCheat("[CHEAT_MOON_JUMP_R_A]", getSettings().game.moonJump, "[CHEAT_MOON_JUMP_R_A_HELP]");
        addCheat("[CHEAT_SUPER_CLAWSHOT]", getSettings().game.superClawshot,
            "[CHEAT_SUPER_CLAWSHOT_HELP]");
        addCheat("[CHEAT_ALWAYS_GREATSPIN]", getSettings().game.alwaysGreatspin,
            "[CHEAT_ALWAYS_GREATSPIN_HELP]");
        addCheat("[CHEAT_FAST_IRON_BOOTS]", getSettings().game.enableFastIronBoots,
            "[CHEAT_FAST_IRON_BOOTS_HELP]");
        addCheat("[CHEAT_CAN_TRANSFORM_ANYWHERE]", getSettings().game.canTransformAnywhere,
            "[CHEAT_CAN_TRANSFORM_ANYWHERE_HELP]");
        addCheat("[CHEAT_FAST_ROLL]", getSettings().game.fastRoll, "[CHEAT_FAST_ROLL_HELP]");
        addCheat("[CHEAT_FAST_SPINNER]", getSettings().game.fastSpinner,
            "[CHEAT_FAST_SPINNER_HELP]");
        addCheat("[CHEAT_FREE_MAGIC_ARMOR]", getSettings().game.freeMagicArmor,
            "[CHEAT_FREE_MAGIC_ARMOR_HELP]");
        addCheat("[CHEAT_INVINCIBLE_ENEMIES]", getSettings().game.invincibleEnemies,
            "[CHEAT_INVINCIBLE_ENEMIES_HELP]");
        addCheat("[CHEAT_TRANSFORM_WITHOUT_SHADOW_CRYSTAL]",
            getSettings().game.transformWithoutShadowCrystal,
            "[CHEAT_TRANSFORM_WITHOUT_SHADOW_CRYSTAL_HELP]");
    });

    add_tab("[INTERFACE]", [this](Rml::Element* content) {
        auto& leftPane = add_child<Pane>(content, Pane::Type::Controlled);
        auto& rightPane = add_child<Pane>(content, Pane::Type::Uncontrolled);

        leftPane.add_section("[DUSKLIGHT]");
        leftPane.register_control(
            leftPane.add_select_button({
                .key = "[UI_LANGUAGE]",
                .getValue =
                    [] {
                        const int idx =
                            ui_language_index(getSettings().backend.uiLanguage.getValue());
                        return Rml::String{kUiLanguageTokenNames[idx]};
                    },
                .isModified =
                    [] {
                        return getSettings().backend.uiLanguage.getValue() !=
                               getSettings().backend.uiLanguage.getDefaultValue();
                    },
            }),
            rightPane, [](Pane& pane) {
                for (int i = 0; i < static_cast<int>(kUiLanguageIds.size()); ++i) {
                    pane
                        .add_button({
                            .text = kUiLanguageTokenNames[i],
                            .isSelected =
                                [i] {
                                    return ui_language_index(
                                               getSettings().backend.uiLanguage.getValue()) == i;
                                },
                        })
                        .on_pressed([i] {
                            mDoAud_seStartMenu(kSoundItemChange);
                            getSettings().backend.uiLanguage.setValue(kUiLanguageIds[i]);
                            i18n::set_language(kUiLanguageIds[i]);
                            config::Save();
                            for (auto& doc : get_document_stack()) {
                                if (doc) {
                                    doc->hide(true);
                                }
                            }
                            push_document(std::make_unique<MenuBar>());
                        });
                }
                pane.add_rml("[APPLIES_TO_DUSKLIGHT_UI_TEXT]");
            });
#if DUSK_CAN_OPEN_DATA_FOLDER
        leftPane.register_control(
            leftPane.add_button("[OPEN_DATA_FOLDER]").on_pressed([] {
                mDoAud_seStartMenu(kSoundClick);
                data::open_data_path();
            }),
            rightPane, [](Pane& pane) {
                pane.add_text("[OPEN_THE_FOLDER_WHERE_DUSKLIGHT_STORES_SETTINGS_SAVES_LOGS_TEXTURE_REPLA]");
            });
#endif
        leftPane.register_control(
            leftPane.add_select_button({
                .key = "[NOTIFICATIONS]",
                .getValue = [] {
                    const bool ach = getSettings().game.enableAchievementToasts.getValue();
                    const bool ctl = getSettings().game.enableControllerToasts.getValue();
                    if (!ach && !ctl) {
                        return Rml::String{"[OFF]"};
                    }
                    if (ach && ctl) {
                        return Rml::String{"[ALL]"};
                    }
                    return Rml::String{"[SOME]"};
                },
                .isModified = [] {
                    const auto& ach = getSettings().game.enableAchievementToasts;
                    const auto& ctl = getSettings().game.enableControllerToasts;
                    return ach.getValue() != ach.getDefaultValue() || ctl.getValue() != ctl.getDefaultValue();
                },
            }),
            rightPane, [](Pane& pane) {
                pane.clear();
                pane.add_button("[SELECT_ALL]").on_pressed([] {
                    mDoAud_seStartMenu(kSoundItemChange);
                    getSettings().game.enableAchievementToasts.setValue(true);
                    getSettings().game.enableControllerToasts.setValue(true);
                    config::Save();
                });
                pane.add_button("[SELECT_NONE]").on_pressed([] {
                    mDoAud_seStartMenu(kSoundItemChange);
                    getSettings().game.enableAchievementToasts.setValue(false);
                    getSettings().game.enableControllerToasts.setValue(false);
                    config::Save();
                });

                pane.add_section("[TYPES]");
                pane.add_button(
                    {
                        .text = "[ACHIEVEMENTS]",
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
                        .text = "[CONTROLLER]",
                        .isSelected =
                            [] { return getSettings().game.enableControllerToasts.getValue(); },
                    })
                    .on_pressed([] {
                        mDoAud_seStartMenu(kSoundItemChange);
                        auto& v = getSettings().game.enableControllerToasts;
                        v.setValue(!v.getValue());
                        config::Save();
                    });
                pane.add_rml("[CHOOSE_WHICH_NOTIFICATIONS_CAN_BE_DISPLAYED]");
            });
#if DUSK_ENABLE_SENTRY_NATIVE
        auto& crashReporting = leftPane.add_child<BoolButton>(BoolButton::Props{
            .key = "[CRASH_REPORTING]",
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
            pane.add_rml("[DUSKLIGHT_CAN_AUTOMATICALLY_SEND_CRASH_REPORTS_TO_THE_DEVELOPERS_CRASH_R]");
        });
#endif
        config_bool_select(leftPane, rightPane, getSettings().backend.skipPreLaunchUI,
            {
                .key = "[SKIP_DUSKLIGHT_MAIN_MENU]",
                .helpText = "[WHEN_STARTING_DUSKLIGHT_SKIP_THE_MAIN_MENU_AND_BOOT_STRAIGHT_INTO_THE_GA]",
            });
        config_bool_select(leftPane, rightPane, getSettings().backend.showPipelineCompilation,
            {
                .key = "[SHOW_PIPELINE_COMPILATION]",
                .helpText = "[SHOW_AN_OVERLAY_WHEN_SHADERS_ARE_BEING_COMPILED_FOR_YOUR_HARDWARE]",
            });
        config_bool_select(leftPane, rightPane, getSettings().backend.checkForUpdates,
            {
                .key = "[CHECK_FOR_UPDATES]",
                .helpText = "[CHECKS_GITHUB_RELEASES_FOR_A_NEW_DUSKLIGHT_VERSION_ON_STARTUP_NO_PERSONA]",
            });
#ifdef DUSK_DISCORD
        config_bool_select(leftPane, rightPane, getSettings().game.enableDiscordPresence,
            {
                .key = "[ENABLE_DISCORD_RICH_PRESENCE]",
                .helpText = "[ENABLE_DUSK_TO_INTEGRATE_WITH_DISCORD_RICH_PRESENCE_THIS_ALLOWS_DISCO]",
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
                .key = "[ENABLE_ADVANCED_SETTINGS]",
                .icon = "warning",
                .helpText = "[SHOW_ADVANCED_SETTINGS_AND_DEBUGGING_TOOLS_WITH_SHIFT_F1_WARNING_DEBUGGING]",
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
                .key = "[SHOW_INPUT_VIEWER]",
                .helpText = "[DISPLAY_A_CONTROLLER_INPUT_OVERLAY_WHILE_PLAYING]",
            });
        config_bool_select(leftPane, rightPane, getSettings().game.showInputViewerGyro,
            {
                .key = "[SHOW_GYRO_INPUT_VIEWER]",
                .helpText = "[SHOW_GYRO_SENSOR_VALUES_IN_THE_INPUT_VIEWER]",
                .isDisabled = [] { return !getSettings().game.showInputViewer; },
            });

        leftPane.add_section("[GAME]");
        config_bool_select(leftPane, rightPane, getSettings().game.enableChineseNameKeyboard,
            {
                .key = "[CHINESE_NAME_KEYBOARD]",
                .helpText = "[REPLACES_THE_NAME_ENTRY_KEYBOARD_WITH_COMMON_CHINESE_CHARACTERS]",
            });
        leftPane.register_control(
            leftPane.add_select_button({
                .key = "[MENU_SCALING_MODE]",
                .getValue =
                    [] {
                        return kMenuScalingModeLabels[static_cast<u8>(
                            getSettings().game.menuScalingMode.getValue())];
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
                            .text = kMenuScalingModeLabels[i],
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
                pane.add_rml("[CHANGES_HOW_THE_COLLECTION_AND_FILE_SELECT_MENUS_SCALE_TO_YOUR_ASPECT_RATIO]");
            });
        config_bool_select(leftPane, rightPane, getSettings().game.hideTvSettingsScreen,
            {
                .key = "[SKIP_TV_SETTINGS_SCREEN]",
                .helpText = "[SKIPS_THE_TV_CALIBRATION_SCREEN_SHOWN_WHEN_LOADING_A_SAVE]",
            });
        add_speedrun_disabled_option(leftPane, rightPane, getSettings().game.recordingMode,
            "[RECORDING_MODE]",
            "[DISABLES_THE_GAME_HUD_AND_ALL_BACKGROUND_MUSIC_USEFUL_FOR_RECORDING_FOOTAGE]");
    });
}

void SettingsWindow::update() {
    if (mPrelaunch && top_document() == this) {
        try_push_verification_modal(*this);
    }

    i18n::set_language(getSettings().backend.uiLanguage.getValue());

    Window::update();
}

void SettingsWindow::hide(bool close) {
    config::Save();
    Window::hide(close);
}

}  // namespace dusk::ui
