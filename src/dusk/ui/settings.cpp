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
#include "menu_bar.hpp"
#include "pane.hpp"
#include "prelaunch.hpp"
#include "ui.hpp"
#include "lang.hpp"

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
    "settings_menu.sma01-en",
    "settings_menu.sma01-de",
    "settings_menu.sma01-fr",
    "settings_menu.sma01-es",
    "settings_menu.sma01-it",
};

constexpr std::array kCardFileTypes = {
    "Card Image",
    "GCI Folder",
};

constexpr std::array kFpsOverlayCornerNames = {
    "settings_menu.vs01-tl",
    "settings_menu.vs01-tr",
    "settings_menu.vs01-bl",
    "settings_menu.vs01-br",
};

constexpr std::array kGyroInputModeLabels = {
    "Sensor",
    "Mouse",
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
        // Do not expose NULL or D3D11
        if (raw[i] != BACKEND_NULL && raw[i] != BACKEND_D3D11) {
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
        const Rml::String rml = "<span class=\"data-folder-current\">" + _("settings_menu.pls02-loc") +
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
        .title = "Data Folder Not Changed",
        .bodyRml = escape(message),
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
const Rml::String kBloomHelpText =
    "Configure the post-processing bloom effect. Classic uses the original bloom pass; Dusklight uses "
    "a higher-quality bloom pass.";
const Rml::String kBloomBrightnessHelpText =
    "Configure bloom intensity. Higher values make bright areas glow more strongly.";
const Rml::String kUnlockFramerateHelpText =
    _("settings_menu.vh04-b01-desc")
const Rml::String kEnableDephOfFHelpText =
    "A post-processing effect that mimics real-world camera lenses by keeping objects at a specific"
    "distance in sharp focus while blurring the foreground and background."
    "Enabling this may reduce performance on your graphics hardware. ";
const Rml::String kEnableMini-MapShadowsHelpText =
    "Adds a shadow effect to the mini-map's background, improving its visibility against bright colors in the Hyrule.";
const Rml::String kDisableCutscenePillarboxingHelpText =
    "Disables pillarboxing during cutscenes. Enabling this may reduce performance on your graphics hardware.  ";

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
        add_tab(_("settings_menu.tab-prelaunch"), [this](Rml::Element* content) {
            auto& leftPane = add_child<Pane>(content, Pane::Type::Controlled);
            auto& rightPane = add_child<Pane>(content, Pane::Type::Uncontrolled);

            leftPane.register_control(
                leftPane
                    .add_select_button({
                        .key = _("settings_menu.prelaunch-s01"),
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
                    pane.add_rml(_("settings_menu.pls01-desc"));
                });
#if DUSK_CAN_CHANGE_DATA_FOLDER
            leftPane.register_control(
                leftPane.add_select_button({
                    .key = _("settings_menu.prelaunch-s02"),
                    .getValue = [] { return configured_data_path_display_name(); },
                    .isModified = [] { return data::is_data_path_restart_pending(); },
                }),
                rightPane, [](Pane& pane) {
                    pane.add_text(_("settings_menu.pls02-desc"));
                    pane.add_child<DataFolderPathText>();
#if DUSK_CAN_OPEN_DATA_FOLDER
                    pane.add_button(_("settings_menu.pls02-btn1")).on_pressed([] {
                        if (data::open_data_path()) {
                            mDoAud_seStartMenu(kSoundClick);
                        }
                    });
#endif
                    pane.add_button(_("settings_menu.pls02-btn2")).on_pressed([] {
                        const auto defaultLocation =
                            io::fs_path_to_string(data::configured_data_path());
                        ShowFolderSelect(&data_folder_dialog_callback, nullptr,
                            aurora::window::get_sdl_window(),
                            defaultLocation.empty() ? nullptr : defaultLocation.c_str());
                    });
#if defined(_WIN32)
                    pane.add_button(_("settings_menu.pls02-btn4")).on_pressed([] {
                        if (data::set_portable_data_path()) {
                            mDoAud_seStartMenu(kSoundItemChange);
                        }
                    });
#endif
                    pane.add_button({
                        .text = _("settings_menu.pls02-btn3"),
                        .isDisabled = [] { return data::is_default_data_path(); },
                    }).on_pressed([] {
                        if (data::reset_data_path()) {
                            mDoAud_seStartMenu(kSoundItemChange);
                        }
                    });
                    pane.add_rml(_("settings_menu.pls02-ftr"));
                });
#endif
            leftPane.register_control(
                leftPane.add_select_button({
                    .key = _("settings_menu.prelaunch-d01"),
                    .getValue =
                        [] {
                            const auto& state = prelaunch_state();
                            if (!state.configuredDiscCanLaunch || !state.configuredDiscInfo.isPal) {
                                return _(kLanguageNames[0]);
                            }
                            const u8 idx = static_cast<u8>(getSettings().game.language.getValue());
                            return _(kLanguageNames[idx]);
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
                                            .text = _(kLanguageNames[i]),
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
                    pane.add_rml(_("settings_menu.restart-ftr"));
                });
            leftPane.register_control(
                leftPane.add_select_button({
                    .key = _("settings_menu.prelaunch-d02"),
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
                    pane.add_rml(_("settings_menu.restart-ftr"));
                });
            leftPane.register_control(
                leftPane.add_select_button({
                    .key = _("settings_menu.prelaunch-d03"),
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

    add_tab("Video", [this](Rml::Element* content) {
        auto& leftPane = add_child<Pane>(content, Pane::Type::Controlled);
        auto& rightPane = add_child<Pane>(content, Pane::Type::Uncontrolled);

        leftPane.add_section(_("settings_menu.video-h01"));

        leftPane.register_control(leftPane.add_button(_("settings_menu.vh01-b01")).on_pressed([] {
            mDoAud_seStartMenu(kSoundItemChange);
            getSettings().video.enableFullscreen.setValue(!getSettings().video.enableFullscreen);
            VISetWindowFullscreen(getSettings().video.enableFullscreen);
            config::Save();
        }),
            rightPane, [](Pane& pane) { pane.clear(); });
        leftPane.register_control(leftPane.add_button(_("settings_menu.vh01-b02")).on_pressed([] {
            mDoAud_seStartMenu(kSoundItemChange);
            getSettings().video.enableFullscreen.setValue(false);
            VISetWindowFullscreen(false);
            VISetWindowSize(FB_WIDTH * 2, FB_HEIGHT * 2);
            VICenterWindow();
        }),
            rightPane, [](Pane& pane) { pane.clear(); });
        config_bool_select(leftPane, rightPane, getSettings().video.enableVsync,
            {
                .key = _("settings_menu.vh01-b03"),
                .helpText = _("settings_menu.vh01-b03-desc"),
                .onChange = [](bool value) { aurora_enable_vsync(value); },
            });
        config_bool_select(leftPane, rightPane, getSettings().video.lockAspectRatio,
            {
                .key = _("settings_menu.vh01-b04"),
                .helpText = _("settings_menu.vh01-b04-desc"),
                .onChange =
                    [](bool value) {
                        AuroraSetViewportPolicy(
                            value ? AURORA_VIEWPORT_FIT : AURORA_VIEWPORT_STRETCH);
                    },
            });
        config_bool_select(leftPane, rightPane, getSettings().game.pauseOnFocusLost,
            {
                .key = _("settings_menu.vh01-b05"),
                .helpText = _("settings_menu.vh01-b05-desc"),
                .onChange = [](bool value) { aurora_set_pause_on_focus_lost(value); },
                .isDisabled = [] { return IsMobile || getSettings().game.speedrunMode; },
            });
        leftPane.register_control(
            leftPane.add_select_button({
                .key = _("settings_menu.vh01-vs01"),
                .getValue =
                    [] {
                        if (!getSettings().video.enableFpsOverlay.getValue()) {
                            return Rml::String{_("settings_menu.bool-off")};
                        }
                        const int idx = getSettings().video.fpsOverlayCorner.getValue();
                        return Rml::String{_(kFpsOverlayCornerNames[idx])};
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
                            .text = "Off",
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
                                .text = _(kFpsOverlayCornerNames[i]),
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
                    "<br/>Display the current framerate in a corner of the screen while playing.");
            });
        leftPane.add_section(_("settings_menu.video-h02"));
        graphics_tuner_control(*this, leftPane, rightPane,
            getSettings().game.internalResolutionScale,
            GraphicsTunerProps{
                .option = GraphicsOption::InternalResolution,
                .title = _("settings_menu.vh02-s01"),
                .helpText = kInternalResolutionHelpText,
                .valueMin = 0,
                .valueMax = 12,
                .defaultValue = 0,
            }, mPrelaunch);
        graphics_tuner_control(*this, leftPane, rightPane,
            getSettings().game.shadowResolutionMultiplier,
            GraphicsTunerProps{
                .option = GraphicsOption::ShadowResolution,
                .title = _("settings_menu.vh02-s02"),
                .helpText = kShadowResolutionHelpText,
                .valueMin = 1,
                .valueMax = 8,
                .defaultValue = 1,
            }, mPrelaunch);

        leftPane.add_section(_("settings_menu.video-h03"));
        graphics_tuner_control(*this, leftPane, rightPane, getSettings().game.bloomMode,
            GraphicsTunerProps{
                .option = GraphicsOption::BloomMode,
                .title = _("settings_menu.vh03-d01"),
                .helpText = kBloomHelpText,
                .valueMin = static_cast<int>(BloomMode::Off),
                .valueMax = static_cast<int>(BloomMode::Dusk),
                .defaultValue = static_cast<int>(BloomMode::Classic),
            }, mPrelaunch);
        graphics_tuner_control(*this, leftPane, rightPane, getSettings().game.bloomMultiplier,
            GraphicsTunerProps{
                .option = GraphicsOption::BloomMultiplier,
                .title = _("settings_menu.vh03-i01"),
                .helpText = kBloomBrightnessHelpText,
                .valueMin = 0,
                .valueMax = 100,
                .defaultValue = 100,
            }, mPrelaunch);

        leftPane.add_section(_("settings_menu.video-h04"));
        config_bool_select(leftPane, rightPane, getSettings().game.enableFrameInterpolation,
            {
                .key = _("settings_menu.vh04-b01"),
                .helpText = kUnlockFramerateHelpText,
            });
        config_bool_select(leftPane, rightPane, getSettings().game.enableDepthOfField,
            {
                .key = _("settings_menu.vh04-b02"),
                .helpText = kEnableDephOfFHelpText,
            });
        config_bool_select(leftPane, rightPane, getSettings().game.enableMapBackground,
            {
                .key = _("settings_menu.vh04-b03"),
                .helpText = kEnableMini-MapShadowsHelpText,
            });
        config_bool_select(leftPane, rightPane, getSettings().game.disableCutscenePillarboxing,
            {
                .key = _("settings_menu.vh04-b04"),
                .helpText = kDisableCutscenePillarboxingHelpText,
        
            });
    });

    add_tab("Input", [this](Rml::Element* content) {
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

        leftPane.add_section(_("settings_menu.input-h01"));
        leftPane.register_control(leftPane.add_button(_("settings_menu.ih01-s01")).on_pressed([this] {
            push(std::make_unique<ControllerConfigWindow>(mPrelaunch));
        }),
            rightPane, [](Pane& pane) {
                pane.clear();
                pane.add_text(_("settings_menu.ih01-s01-desc"));
            });
        config_bool_select(leftPane, rightPane, getSettings().game.allowBackgroundInput,
            {
                .key = _("settings_menu.ih01-b01"),
                .helpText = _("settings_menu.ih01-b01-desc"),
                .onChange = [](bool value) { aurora_set_background_input(value); },
            });

        leftPane.add_section(_("settings_menu.input-h02"));
        addOption(_("settings_menu.ih02-b01"), getSettings().game.freeCamera,
            _("settings_menu.ih02-b01-desc"));
        addOption(_("settings_menu.ih02-b02"), getSettings().game.invertCameraXAxis,
            _("settings_menu.ih02-b02-desc"));
        addOption(_("settings_menu.ih02-b03"), getSettings().game.invertCameraYAxis,
            _("settings_menu.ih02-b03-desc"),
            [] { return !getSettings().game.freeCamera; });
        config_percent_select(leftPane, rightPane, getSettings().game.freeCameraSensitivity,
            _("settings_menu.ih02-i01"), _("settings_menu.ih02-i01-desc"), 50, 200, 5,
            [] { return !getSettings().game.freeCamera; });
        addOption(_("settings_menu.ih02-b04"), getSettings().game.invertFirstPersonXAxis,
            _("settings_menu.ih02-b04-desc"));
        addOption(_("settings_menu.ih02-b05"), getSettings().game.invertFirstPersonYAxis,
            _("settings_menu.ih02-b05-desc"));

        leftPane.add_section(_("settings_menu.input-h03"));
        leftPane.register_control(
            leftPane.add_select_button({
                .key = _("settings_menu.ih03-d01"),
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
                pane.add_rml(_("settings_menu.ih03-d01-desc"));
            });
        addOption(_("settings_menu.ih03-b01"), getSettings().game.enableGyroAim,
            _("settings_menu.ih03-b01-desc"));

        addOption(_("settings_menu.ih03-b02"), getSettings().game.enableGyroRollgoal,
            _("settings_menu.ih03-b02-desc"),
            [] { return getSettings().game.gyroMode.getValue() == GyroMode::Mouse; });
        config_percent_select(leftPane, rightPane, getSettings().game.gyroSensitivityY,
            _("settings_menu.ih03-i01"), _("settings_menu.ih03-i01-desc"), 25, 400, 5,
            [] { return !gyro_enabled(); });
        config_percent_select(leftPane, rightPane, getSettings().game.gyroSensitivityX,
            _("settings_menu.ih03-i02"), _("settings_menu.ih03-i02-desc"), 25, 400, 5,
            [] { return !gyro_enabled(); });
        config_percent_select(leftPane, rightPane, getSettings().game.gyroSensitivityRollgoal,
            _("settings_menu.ih03-i03"), _("settings_menu.ih03-i03-desc"), 25, 400, 5,
            [] {
                return !getSettings().game.enableGyroRollgoal ||
                       getSettings().game.gyroMode.getValue() == GyroMode::Mouse;
            });
        config_percent_select(leftPane, rightPane, getSettings().game.gyroDeadband, _("settings_menu.ih03-i04"), _("settings_menu.ih03-i04-desc"), 0, 50, 1,
            [] { return !gyro_enabled(); });
        config_percent_select(leftPane, rightPane, getSettings().game.gyroSmoothing,
            _("settings_menu.ih03-i05"), _("settings_menu.ih03-i05-desc"), 0, 100, 1,
            [] { return !gyro_enabled(); });
        addOption(_("settings_menu.ih03-b03"), getSettings().game.gyroInvertPitch,
            _("settings_menu.ih03-b03-desc"), [] { return !gyro_enabled(); });
        addOption(_("settings_menu.ih03-b04"), getSettings().game.gyroInvertYaw,
            _("settings_menu.ih03-b04-desc"), [] { return !gyro_enabled(); });

        leftPane.add_section(_("settings_menu.input-h04"));
        addOption(_("settings_menu.ih04-b01"), getSettings().game.enableTurboKeybind,
            _("settings_menu.ih04-b01-desc"),
            [] { return getSettings().game.speedrunMode; });
        addOption(_("settings_menu.ih04-b02") + "(" + Rml::String{hotkeys::DO_RESET} + ")",
            getSettings().game.enableResetKeybind,
            _("settings_menu.ih04-b02-desc") + Rml::String{hotkeys::DO_RESET} + _("settings_menu.ih04-b02-desc-p2"));
    });

    add_tab("Audio", [this](Rml::Element* content) {
        auto& leftPane = add_child<Pane>(content, Pane::Type::Controlled);
        auto& rightPane = add_child<Pane>(content, Pane::Type::Uncontrolled);

        // TODO: Individual sliders for Main Music, Sub Music, Sound Effects, and Fanfare.
        leftPane.add_section(_("settings_menu.audio-h01"));
        leftPane.register_control(
            leftPane.add_child<NumberButton>(NumberButton::Props{
                .key = _("settings_menu.ah01-i01"),
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
                pane.add_text(_("settings_menu.ah01-i01-desc"));
            });

        leftPane.add_section(_("settings_menu.audio-h02"));
        config_bool_select(leftPane, rightPane, getSettings().audio.enableReverb,
            {
                .key = _("settings_menu.ah02-b01"),
                .helpText = _("settings_menu.ah02-b01-desc"),
                .onChange = [](bool value) { audio::SetEnableReverb(value); },
            });
        config_bool_select(leftPane, rightPane, getSettings().audio.enableHrtf,
            {
                .key = _("settings_menu.ah02-b02"),
                .helpText = _("settings_menu.ah02-b02-desc"),
                .onChange = [](bool value) { audio::EnableHrtf = value; },
            });
        config_bool_select(leftPane, rightPane, getSettings().audio.menuSounds,
            {
                .key = _("settings_menu.ah02-b03"),
                .helpText = _("settings_menu.ah02-b03-desc"),
            });

        leftPane.add_section(_("settings_menu.audio-h03"));
        config_bool_select(leftPane, rightPane, getSettings().game.noLowHpSound,
            {
                .key = _("settings_menu.ah03-b01"),
                .helpText = _("settings_menu.ah03-b01-desc"),
            });
        config_bool_select(leftPane, rightPane, getSettings().game.midnasLamentNonStop,
            {
                .key = _("settings_menu.ah03-b02"),
                .helpText = _("settings_menu.ah03-b02-desc"),
            });
    });

    add_tab(_("settings_menu.tab-gameplay"), [this](Rml::Element* content) {
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

        leftPane.add_section(_("settings_menu.gameplay-h01"));
        addOption(_("settings_menu.gh01-b01"), getSettings().game.enableMirrorMode,
            _("settings_menu.gh01-b01-desc"));
        addOption(_("settings_menu.gh01-b02"), getSettings().game.minimalHUD,
            _("settings_menu.gh01-b02-desc"));
        addOption(_("settings_menu.gh01-b03"), getSettings().game.restoreWiiGlitches,
            _("settings_menu.gh01-b03-desc"));
        addOption(_("settings_menu.gh01-b04"), getSettings().game.enableLinkDollRotation,
            _("settings_menu.gh01-b04-desc"));

        leftPane.add_section(_("settings_menu.gameplay-h02"));
        leftPane.register_control(
            leftPane.add_child<NumberButton>(NumberButton::Props{
                .key = _("settings_menu.gh02-i01"),
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
                pane.add_text(_("settings_menu.gh02-i01-desc"));
            });
        addSpeedrunDisabledOption(
            _("settings_menu.gh02-b01"), getSettings().game.instantDeath, _("settings_menu.gh02-b01-desc"));
        addSpeedrunDisabledOption(_("settings_menu.gh02-b02"), getSettings().game.noHeartDrops,
            _("settings_menu.gh02-b02-desc"));

        leftPane.add_section(_("settings_menu.gameplay-h03"));
        addOption(_("settings_menu.gh03-b01"), getSettings().game.biggerWallets,
            _("settings_menu.gh03-b01-desc"));
        addOption(_("settings_menu.gh03-b02"), getSettings().game.disableRupeeCutscenes,
            _("settings_menu.gh03-b02-desc"));
        addOption(_("settings_menu.gh03-b03"), getSettings().game.fastClimbing,
            _("settings_menu.gh03-b03-desc"));
        addOption(_("settings_menu.gh03-b04"), getSettings().game.fastTears,
            _("settings_menu.gh03-b04-desc"));
        addSpeedrunDisabledOption(_("settings_menu.gh03-b05"), getSettings().game.autoSave,
            _("settings_menu.gh03-b05-desc"));
        addOption(_("settings_menu.gh03-b06"), getSettings().game.instantSaves,
            _("settings_menu.gh03-b06-desc"));
        addOption(_("settings_menu.gh03-b07"), getSettings().game.instantText,
            _("settings_menu.gh03-b07-desc"));
        addOption(_("settings_menu.gh03-b08"), getSettings().game.noMissClimbing,
            _("settings_menu.gh03-b08-desc"));
        addOption(_("settings_menu.gh03-b09"), getSettings().game.noReturnRupees,
            _("settings_menu.gh03-b09-desc"));
        addOption(_("settings_menu.gh03-b10"), getSettings().game.noSwordRecoil,
            _("settings_menu.gh03-b10-desc"));
        addOption(_("settings_menu.gh03-b11"), getSettings().game.no2ndFishForCat,
            _("settings_menu.gh03-b11-desc"));
        addSpeedrunDisabledOption(_("settings_menu.gh03-b12"), getSettings().game.sunsSong,
            _("settings_menu.gh03-b12-desc"));
        addOption(_("settings_menu.gh03-b13"), getSettings().game.enableQuickTransform,
            _("settings_menu.gh03-b13-desc"));

        leftPane.add_section(_("settings_menu.gameplay-h04"));
        config_bool_select(leftPane, rightPane, getSettings().game.speedrunMode,
            {
                .key = _("settings_menu.gh04-b01"),
                .helpText =
                    _("settings_menu.gh04-b01-desc"),
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
                .key = _("settings_menu.gh04-b02"),
                .helpText = _("settings_menu.gh04-b02-desc"),
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
                .key = _("settings_menu.gh04-b03"),
                .helpText = _("settings_menu.gh04-b03-desc"),
                .isDisabled = [] { return !getSettings().game.speedrunMode; },
            });
    });

    add_tab(_("settings_menu.tab-cheats"), [this](Rml::Element* content) {
        auto& leftPane = add_child<Pane>(content, Pane::Type::Controlled);
        auto& rightPane = add_child<Pane>(content, Pane::Type::Uncontrolled);

        auto addCheat = [&](const Rml::String& key, ConfigVar<bool>& value,
                            const Rml::String& helpText) {
            add_speedrun_disabled_option(leftPane, rightPane, value, key, helpText);
        };

        leftPane.add_section(_("settings_menu.cheats-h01"));
        addCheat(_("settings_menu.ch01-b01"), getSettings().game.infiniteHearts, _("settings_menu.ch01-b01-desc"));
        addCheat(_("settings_menu.ch01-b02"), getSettings().game.infiniteArrows, _("settings_menu.ch01-b02-desc"));
        addCheat(_("settings_menu.ch01-b03"), getSettings().game.infiniteSeeds, _("settings_menu.ch01-b03-desc"));
        addCheat(_("settings_menu.ch01-b04"), getSettings().game.infiniteBombs, _("settings_menu.ch01-b04-desc"));
        addCheat(_("settings_menu.ch01-b05"), getSettings().game.infiniteOil, _("settings_menu.ch01-b05-desc"));
        addCheat(_("settings_menu.ch01-b06"), getSettings().game.infiniteOxygen, _("settings_menu.ch01-b06-desc"));
        addCheat(_("settings_menu.ch01-b07"), getSettings().game.infiniteRupees, _("settings_menu.ch01-b07-desc"));
        addCheat(_("settings_menu.ch01-b08"), getSettings().game.enableIndefiniteItemDrops, _("settings_menu.ch01-b08-desc"));

        leftPane.add_section(_("settings_menu.cheats-h02"));
        addCheat(_("settings_menu.ch02-b01"), getSettings().game.moonJump, _("settings_menu.ch02-b01-desc"));
        addCheat(_("settings_menu.ch02-b02"), getSettings().game.superClawshot, _("settings_menu.ch02-b02-desc"));
        addCheat(_("settings_menu.ch02-b03"), getSettings().game.alwaysGreatspin, _("settings_menu.ch02-b03-desc"));
        addCheat(_("settings_menu.ch02-b04"), getSettings().game.enableFastIronBoots, _("settings_menu.ch02-b04-desc"));
        addCheat(_("settings_menu.ch02-b05"), getSettings().game.canTransformAnywhere, _("settings_menu.ch02-b05-desc"));
        addCheat(_("settings_menu.ch02-b06"), getSettings().game.fastRoll, _("settings_menu.ch02-b06-desc"));
        addCheat(_("settings_menu.ch02-b07"), getSettings().game.fastSpinner, _("settings_menu.ch02-b07-desc"));
        addCheat(_("settings_menu.ch02-b08"), getSettings().game.freeMagicArmor, _("settings_menu.ch02-b08-desc"));
        addCheat(_("settings_menu.ch02-b09"), getSettings().game.invincibleEnemies, _("settings_menu.ch02-b09-desc"));
    });

    add_tab(_("settings_menu.tab-interface"), [this](Rml::Element* content) {
        auto& leftPane = add_child<Pane>(content, Pane::Type::Controlled);
        auto& rightPane = add_child<Pane>(content, Pane::Type::Uncontrolled);

        leftPane.add_section("Dusklight");
#if DUSK_CAN_OPEN_DATA_FOLDER
        leftPane.register_control(
            leftPane.add_button(_("settings_menu.pls02-btn1")).on_pressed([] {
                mDoAud_seStartMenu(kSoundClick);
                data::open_data_path();
            }),
            rightPane, [](Pane& pane) {
                pane.add_text(_("settings_menu.pls02-desc"));
            });
#endif
        leftPane.register_control(
            leftPane.add_select_button({
                .key = _("settings_menu.inh01-d01"),
                .getValue = [] {
                    const bool ach = getSettings().game.enableAchievementToasts.getValue();
                    const bool ctl = getSettings().game.enableControllerToasts.getValue();
                    if (!ach && !ctl) {
                        return Rml::String{"Off"};
                    }
                    if (ach && ctl) {
                        return Rml::String{"All"};
                    }
                    return Rml::String{"Some"};
                },
                .isModified = [] {
                    const auto& ach = getSettings().game.enableAchievementToasts;
                    const auto& ctl = getSettings().game.enableControllerToasts;
                    return ach.getValue() != ach.getDefaultValue() || ctl.getValue() != ctl.getDefaultValue();
                },
            }),
            rightPane, [](Pane& pane) {
                pane.clear();
                pane.add_button(_("settings_menu.inh01-d01-btn1")).on_pressed([] {
                    mDoAud_seStartMenu(kSoundItemChange);
                    getSettings().game.enableAchievementToasts.setValue(true);
                    getSettings().game.enableControllerToasts.setValue(true);
                    config::Save();
                });
                pane.add_button(_("settings_menu.inh01-d01-btn2")).on_pressed([] {
                    mDoAud_seStartMenu(kSoundItemChange);
                    getSettings().game.enableAchievementToasts.setValue(false);
                    getSettings().game.enableControllerToasts.setValue(false);
                    config::Save();
                });

                pane.add_section(_("settings_menu.inh01-d01-h01"));
                pane.add_button(
                    {
                        .text = _("settings_menu.inh01-d01-btn3"),
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
                        .text = _("settings_menu.inh01-d01-btn4"),
                        .isSelected =
                            [] { return getSettings().game.enableControllerToasts.getValue(); },
                    })
                    .on_pressed([] {
                        mDoAud_seStartMenu(kSoundItemChange);
                        auto& v = getSettings().game.enableControllerToasts;
                        v.setValue(!v.getValue());
                        config::Save();
                    });
                pane.add_rml(_("settings_menu.inh01-d01-desc"));
            });
#if DUSK_ENABLE_SENTRY_NATIVE
        auto& crashReporting = leftPane.add_child<BoolButton>(BoolButton::Props{
            .key = _("settings_menu.inh01-b08"),
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
            pane.add_rml(_("settings_menu.inh01-b08-desc"));
        });
#endif
        config_bool_select(leftPane, rightPane, getSettings().backend.skipPreLaunchUI,
            {
                .key = _("settings_menu.inh01-b01"),
                .helpText = _("settings_menu.inh01-b01-desc"),
            });
        config_bool_select(leftPane, rightPane, getSettings().backend.showPipelineCompilation,
            {
                .key = _("settings_menu.inh01-b02"),
                .helpText = _("settings_menu.inh01-b02-desc"),
            });
        config_bool_select(leftPane, rightPane, getSettings().backend.checkForUpdates,
            {
                .key = _("settings_menu.inh01-b03"),
                .helpText = _("settings_menu.inh01-b03-desc"),
            });
#ifdef DUSK_DISCORD
        config_bool_select(leftPane, rightPane, getSettings().game.enableDiscordPresence,
            {
                .key = _("settings_menu.inh01-b04"),
                .helpText = _("settings_menu.inh01-b04-desc"),
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
                .key = _("settings_menu.inh01-b05"),
                .icon = "warning",
                .helpText = _("settings_menu.inh01-b05-desc"),
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
                .key = _("settings_menu.inh01-b06"),
                .helpText = _("settings_menu.inh01-b06-desc"),
            });
        config_bool_select(leftPane, rightPane, getSettings().game.showInputViewerGyro,
            {
                .key = _("settings_menu.inh01-b07"),
                .helpText = _("settings_menu.inh01-b07-desc"),
                .isDisabled = [] { return !getSettings().game.showInputViewer; },
            });

        leftPane.add_section(_("settings_menu.interface-h02"));
        config_bool_select(leftPane, rightPane, getSettings().game.hideTvSettingsScreen,
            {
                .key = _("settings_menu.inh02-b01"),
                .helpText = _("settings_menu.inh02-b01-desc"),
            });
        add_speedrun_disabled_option(leftPane, rightPane, getSettings().game.recordingMode,
            _("settings_menu.inh02-b02"),
            _("settings_menu.inh02-b02-desc"));
    });
}

void SettingsWindow::update() {
    if (mPrelaunch && top_document() == this) {
        try_push_verification_modal(*this);
    }

    Window::update();
}

}  // namespace dusk::ui
