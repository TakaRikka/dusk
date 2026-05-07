#include "settings.hpp"

#include "aurora/gfx.h"
#include "bool_button.hpp"
#include "controller_config.hpp"
#include "dusk/audio/DuskAudioSystem.h"
#include "dusk/audio/DuskDsp.hpp"
#include "dusk/config.hpp"
#include "dusk/imgui/ImGuiEngine.hpp"
#include "dusk/livesplit.h"
#include "graphics_tuner.hpp"
#include "m_Do/m_Do_main.h"
#include "number_button.hpp"
#include "pane.hpp"
#include "prelaunch.hpp"
#include "ui.hpp"

#include <algorithm>

namespace dusk::ui {
namespace {

constexpr std::array kLanguageNames = {
    "English",
    "German",
    "French",
    "Spanish",
    "Italian",
};

constexpr std::array kCardFileTypes = {
    "Card Image",
    "GCI Folder",
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

    getSettings().game.damageMultiplier.setValue(1);
    getSettings().game.instantDeath.setValue(false);
    getSettings().game.noHeartDrops.setValue(false);

    getSettings().game.infiniteHearts.setValue(false);
    getSettings().game.infiniteArrows.setValue(false);
    getSettings().game.infiniteBombs.setValue(false);
    getSettings().game.infiniteOil.setValue(false);
    getSettings().game.infiniteOxygen.setValue(false);
    getSettings().game.infiniteRupees.setValue(false);
    getSettings().game.enableIndefiniteItemDrops.setValue(false);

    getSettings().game.moonJump.setValue(false);
    getSettings().game.superClawshot.setValue(false);
    getSettings().game.alwaysGreatspin.setValue(false);
    getSettings().game.enableFastIronBoots.setValue(false);
    getSettings().game.canTransformAnywhere.setValue(false);
    getSettings().game.fastSpinner.setValue(false);
    getSettings().game.freeMagicArmor.setValue(false);

    getSettings().game.enableTurboKeybind.setValue(false);
}

const Rml::String kInternalResolutionHelpText =
    "Configure the resolution used for rendering the game. Higher values are more demanding on "
    "your graphics hardware.";
const Rml::String kShadowResolutionHelpText =
    "Configure the shadow-map resolution. Higher values improve shadow quality but increase GPU "
    "and memory usage.";
const Rml::String kBloomHelpText =
    "Configure the post-processing bloom effect. Classic uses the original bloom pass; Dusk uses "
    "a higher-quality bloom pass.";
const Rml::String kBloomBrightnessHelpText =
    "Configure bloom intensity. Higher values make bright areas glow more strongly.";
const Rml::String kUnlockFramerateHelpText =
    "Uses inter-frame interpolation to enable higher frame rates.<br/><br/>May introduce minor "
    "visual artifacts or animation glitches.";

int float_setting_percent(ConfigVar<float>& var) {
    return static_cast<int>(var.getValue() * 100.0f + 0.5f);
}

bool gyro_enabled() {
    return getSettings().game.enableGyroAim || getSettings().game.enableGyroRollgoal;
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
    const GraphicsTunerProps& props) {
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
            .on_nav_command([&window, props](Rml::Event&, NavCommand cmd) {
                if (cmd == NavCommand::Confirm || cmd == NavCommand::Left ||
                    cmd == NavCommand::Right) {
                    window.push(std::make_unique<GraphicsTuner>(props));
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
        add_tab("Prelaunch", [this](Rml::Element* content) {
            auto& leftPane = add_child<Pane>(content, Pane::Type::Controlled);
            auto& rightPane = add_child<Pane>(content, Pane::Type::Uncontrolled);

            leftPane.register_control(
                leftPane
                    .add_select_button({
                        .key = "Disc Image",
                        .getValue =
                            [] {
                                const auto& path = prelaunch_state().configuredDiscPath;
                                std::string display;
                                if (path.empty()) {
                                    display = "(none)";
                                } else {
                                    display = std::filesystem::path(path).filename().string();
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
                    pane.add_rml("Set the disc image that Dusk uses to launch the game.<br/><br/>"
                                 "Changes require a restart.");
                });
            leftPane.register_control(
                leftPane.add_select_button({
                    .key = "Language",
                    .getValue =
                        [] {
                            const auto& state = prelaunch_state();
                            if (!state.configuredDiscCanLaunch ||
                                !state.configuredDiscInfo.isPal)
                            {
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
                                            .text = kLanguageNames[i],
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
                    pane.add_rml("<br/>Changes require a restart.");
                });
            leftPane.register_control(
                leftPane.add_select_button({
                    .key = "Graphics Backend",
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
                    pane.add_rml("<br/>Changes require a restart.");
                });
            leftPane.register_control(
                leftPane.add_select_button({
                    .key = "Save File Type",
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

    add_tab("Graphics", [this](Rml::Element* content) {
        auto& leftPane = add_child<Pane>(content, Pane::Type::Controlled);
        auto& rightPane = add_child<Pane>(content, Pane::Type::Uncontrolled);

        leftPane.add_section("Display");

        leftPane.register_control(leftPane.add_button("Toggle Fullscreen").on_pressed([] {
            mDoAud_seStartMenu(kSoundItemChange);
            getSettings().video.enableFullscreen.setValue(!getSettings().video.enableFullscreen);
            VISetWindowFullscreen(getSettings().video.enableFullscreen);
            config::Save();
        }),
            rightPane, [](Pane& pane) { pane.clear(); });
        leftPane.register_control(leftPane.add_button("Restore Default Window Size").on_pressed([] {
            mDoAud_seStartMenu(kSoundItemChange);
            getSettings().video.enableFullscreen.setValue(false);
            VISetWindowFullscreen(false);
            VISetWindowSize(FB_WIDTH * 2, FB_HEIGHT * 2);
            VICenterWindow();
        }),
            rightPane, [](Pane& pane) { pane.clear(); });
        config_bool_select(leftPane, rightPane, getSettings().video.enableVsync,
            {
                .key = "Enable VSync",
                .helpText = "Synchronizes the frame rate to your monitor's refresh rate.",
                .onChange = [](bool value) { aurora_enable_vsync(value); },
            });
        config_bool_select(leftPane, rightPane, getSettings().video.lockAspectRatio,
            {
                .key = "Lock 4:3 Aspect Ratio",
                .helpText = "Lock the game's aspect ratio to the original.",
                .onChange =
                    [](bool value) {
                        AuroraSetViewportPolicy(
                            value ? AURORA_VIEWPORT_FIT : AURORA_VIEWPORT_STRETCH);
                    },
            });
        config_bool_select(leftPane, rightPane, getSettings().game.pauseOnFocusLost,
            {
                .key = "Pause on Focus Lost",
                .isDisabled = [] { return IsMobile; },
            });

        leftPane.add_section("Resolution");
        graphics_tuner_control(*this, leftPane, rightPane,
            getSettings().game.internalResolutionScale,
            GraphicsTunerProps{
                .option = GraphicsOption::InternalResolution,
                .title = "Internal Resolution",
                .helpText = kInternalResolutionHelpText,
                .valueMin = 0,
                .valueMax = 12,
                .defaultValue = 0,
            });
        graphics_tuner_control(*this, leftPane, rightPane,
            getSettings().game.shadowResolutionMultiplier,
            GraphicsTunerProps{
                .option = GraphicsOption::ShadowResolution,
                .title = "Shadow Resolution",
                .helpText = kShadowResolutionHelpText,
                .valueMin = 1,
                .valueMax = 8,
                .defaultValue = 1,
            });

        leftPane.add_section("Post-Processing");
        graphics_tuner_control(*this, leftPane, rightPane, getSettings().game.bloomMode,
            GraphicsTunerProps{
                .option = GraphicsOption::BloomMode,
                .title = "Bloom",
                .helpText = kBloomHelpText,
                .valueMin = static_cast<int>(BloomMode::Off),
                .valueMax = static_cast<int>(BloomMode::Dusk),
                .defaultValue = static_cast<int>(BloomMode::Classic),
            });
        graphics_tuner_control(*this, leftPane, rightPane, getSettings().game.bloomMultiplier,
            GraphicsTunerProps{
                .option = GraphicsOption::BloomMultiplier,
                .title = "Bloom Brightness",
                .helpText = kBloomBrightnessHelpText,
                .valueMin = 0,
                .valueMax = 100,
                .defaultValue = 100,
            });

        leftPane.add_section("Rendering");
        config_bool_select(leftPane, rightPane, getSettings().game.enableFrameInterpolation,
            {
                .key = "Unlock Framerate",
                .helpText = kUnlockFramerateHelpText,
            });
        config_bool_select(leftPane, rightPane, getSettings().game.enableDepthOfField,
            {
                .key = "Enable Depth of Field",
            });
        config_bool_select(leftPane, rightPane, getSettings().game.enableMapBackground,
            {
                .key = "Enable Mini-Map Shadows",
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

        leftPane.add_section("Controller");
        leftPane.register_control(leftPane.add_button("Configure Controller").on_pressed([this] {
            push(std::make_unique<ControllerConfigWindow>());
        }),
            rightPane, [](Pane& pane) {
                pane.clear();
                pane.add_text("Open controller binding configuration.");
            });

        leftPane.add_section("Camera");
        addOption("Free Camera", getSettings().game.freeCamera,
            "Enables twin-stick camera control, letting the C-Stick move the camera vertically as "
            "well as horizontally.");
        addOption("Invert Camera X Axis", getSettings().game.invertCameraXAxis,
            "Invert horizontal camera movement.");
        addOption("Invert Camera Y Axis", getSettings().game.invertCameraYAxis,
            "Invert vertical camera movement when Free Camera is enabled.",
            [] { return !getSettings().game.freeCamera; });
        config_percent_select(leftPane, rightPane, getSettings().game.freeCameraSensitivity,
            "Free Camera Sensitivity", "Adjusts twin-stick camera sensitivity.", 50, 200, 5,
            [] { return !getSettings().game.freeCamera; });

        leftPane.add_section("Gyro");
        addOption("Gyro Aim", getSettings().game.enableGyroAim,
            "Enables gyro controls while in look mode, aiming a hawk, and aiming "
            "supported items.<br/><br/>Supported items include the Slingshot, Gale Boomerang, "
            "Hero's Bow, Clawshot(s), Ball and Chain, and Dominion Rod.");
        addOption("Gyro Rollgoal", getSettings().game.enableGyroRollgoal,
            "Enables gyro controls for Rollgoal in Hena's Cabin.");
        config_percent_select(leftPane, rightPane, getSettings().game.gyroSensitivityY,
            "Gyro Pitch Sensitivity", "Controls vertical gyro aiming sensitivity.", 25, 400, 5,
            [] { return !gyro_enabled(); });
        config_percent_select(leftPane, rightPane, getSettings().game.gyroSensitivityX,
            "Gyro Yaw Sensitivity", "Controls horizontal gyro aiming sensitivity.", 25, 400, 5,
            [] { return !gyro_enabled(); });
        config_percent_select(leftPane, rightPane, getSettings().game.gyroSensitivityRollgoal,
            "Rollgoal Sensitivity", "Controls how strongly gyro input tilts the Rollgoal table.",
            25, 400, 5, [] { return !getSettings().game.enableGyroRollgoal; });
        config_percent_select(leftPane, rightPane, getSettings().game.gyroDeadband, "Gyro Deadband",
            "Ignores small gyro movement to reduce drift and jitter.", 0, 50, 1,
            [] { return !gyro_enabled(); });
        config_percent_select(leftPane, rightPane, getSettings().game.gyroSmoothing,
            "Gyro Smoothing", "Higher values smooth gyro input over time.", 0, 100, 1,
            [] { return !gyro_enabled(); });
        addOption("Invert Gyro Pitch", getSettings().game.gyroInvertPitch,
            "Invert vertical gyro aiming.", [] { return !gyro_enabled(); });
        addOption("Invert Gyro Yaw", getSettings().game.gyroInvertYaw,
            "Invert horizontal gyro aiming.", [] { return !gyro_enabled(); });

        leftPane.add_section("Tools");
        addOption("Turbo Key", getSettings().game.enableTurboKeybind,
            "Hold Tab to increase game speed by up to 4x.",
            [] { return getSettings().game.speedrunMode; });
    });

    add_tab("Audio", [this](Rml::Element* content) {
        auto& leftPane = add_child<Pane>(content, Pane::Type::Controlled);
        auto& rightPane = add_child<Pane>(content, Pane::Type::Uncontrolled);

        // TODO: Individual sliders for Main Music, Sub Music, Sound Effects, and Fanfare.
        leftPane.add_section("Volume");
        leftPane.register_control(
            leftPane.add_child<NumberButton>(NumberButton::Props{
                .key = "Master Volume",
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
                pane.add_text("Adjusts the volume of all sounds in the game.");
            });

        leftPane.add_section("Effects");
        config_bool_select(leftPane, rightPane, getSettings().audio.enableReverb,
            {
                .key = "Enable Reverb",
                .helpText = "Enables the reverb effect in game audio.",
                .onChange = [](bool value) { audio::SetEnableReverb(value); },
            });
        config_bool_select(leftPane, rightPane, getSettings().audio.enableHrtf,
            {
                .key = "Enable Spatial Sound",
                .helpText =
                    "Emulate surround sound via HRTF. Recommended only for use with headphones!",
                .onChange = [](bool value) { audio::EnableHrtf = value; },
            });
        config_bool_select(leftPane, rightPane, getSettings().audio.menuSounds,
            {
                .key = "Dusk Menu Sounds",
                .helpText = "Play sound effects when navigating the Dusk menu.",
            });

        leftPane.add_section("Tweaks");
        config_bool_select(leftPane, rightPane, getSettings().game.noLowHpSound,
            {
                .key = "No Low HP Sound",
                .helpText = "Disable the beeping sound when having low health.",
            });
        config_bool_select(leftPane, rightPane, getSettings().game.midnasLamentNonStop,
            {
                .key = "Non-Stop Midna's Lament",
                .helpText = "Prevents enemy music while Midna's Lament is playing.",
            });
    });

    add_tab("Gameplay", [this](Rml::Element* content) {
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
            config_bool_select(leftPane, rightPane, value,
                {
                    .key = key,
                    .helpText = helpText,
                    .isDisabled = [] { return getSettings().game.speedrunMode; },
                });
        };

        leftPane.add_section("General");
        addOption("Mirror Mode", getSettings().game.enableMirrorMode,
            "Mirrors the world horizontally, matching the Wii version of the game.");
        addOption("Disable Main HUD", getSettings().game.disableMainHUD,
            "Disables the main HUD of the game.<br/>Useful for recording or a more immersive "
            "experience.");
        addOption("Restore Wii 1.0 Glitches", getSettings().game.restoreWiiGlitches,
            "Restores patched glitches from Wii USA 1.0, the first released version.");
        addOption("Enable Rotating Link Doll", getSettings().game.enableLinkDollRotation,
            "Enables rotating Link in the collection menu with the C-Stick.");

        leftPane.add_section("Difficulty");
        leftPane.register_control(
            leftPane.add_child<NumberButton>(NumberButton::Props{
                .key = "Damage Multiplier",
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
                pane.add_text("Multiplies incoming damage.");
            });
        addSpeedrunDisabledOption(
            "Instant Death", getSettings().game.instantDeath, "Any hit will instantly kill you.");
        addSpeedrunDisabledOption("No Heart Drops", getSettings().game.noHeartDrops,
            "Hearts will never drop from enemies, pots, and various other places.");

        leftPane.add_section("Quality of Life");
        addOption("Bigger Wallets", getSettings().game.biggerWallets,
            "Wallet sizes are like in the HD version. (500, 1000, 2000)");
        addOption("Disable Rupee Cutscenes", getSettings().game.disableRupeeCutscenes,
            "Rupees will not play cutscenes after you have collected them the first time.");
        addOption("Faster Climbing", getSettings().game.fastClimbing,
            "Quicker climbing on ladders and vines like the HD version.");
        addOption("Faster Tears of Light", getSettings().game.fastTears,
            "Tears of Light dropped by Shadow Insects pop out faster like the HD version.");
        config_bool_select(leftPane, rightPane, getSettings().game.autoSave,
            {
                .key = "Autosave",
                .icon = "warning",
                .helpText =
                    "Autosaves the game when going to a new area, opening a dungeon door, "
                    "or getting a new item.<br/><br/><icon class=\"warning\"/> Experimental "
                    "feature: Use at your own risk.",
            });
        addOption("Instant Saves", getSettings().game.instantSaves,
            "Skips the delay when writing to the Memory Card.");
        addOption("Hold B for Instant Text", getSettings().game.instantText,
            "Makes text scroll immediately by holding B.");
        addOption("No Climbing Miss Animation", getSettings().game.noMissClimbing,
            "Prevents Link from playing a struggle animation when grabbing ledges or "
            "climbing on vines.");
        addOption("No Rupee Returns", getSettings().game.noReturnRupees,
            "Always collect Rupees even if your Wallet is too full.");
        addOption("No Sword Recoil", getSettings().game.noSwordRecoil,
            "Link will not recoil when his sword hits walls.");
        addOption("No 2nd Fish for Cat", getSettings().game.no2ndFishForCat,
            "Skip needing to catch a second fish for Sera's cat.");
        addOption("Sun's Song (R+X)", getSettings().game.sunsSong,
            "Allows Wolf Link to howl and change the time of day.");
        addOption("Quick Transform (R+Y)", getSettings().game.enableQuickTransform,
            "Transform instantly by pressing R and Y simultaneously.");

        leftPane.add_section("Speedrunning");
        config_bool_select(leftPane, rightPane, getSettings().game.speedrunMode,
            {
                .key = "Speedrun Mode",
                .helpText =
                    "Enables speedrunning options while restricting certain gameplay modifiers.",
                .onChange = [](bool) { reset_for_speedrun_mode(); },
            });
        config_bool_select(leftPane, rightPane, getSettings().game.liveSplitEnabled,
            {
                .key = "LiveSplit Connection",
                .helpText = "Connect to LiveSplit server on localhost:16834.",
                .onChange =
                    [](bool enabled) {
                        if (enabled) {
                            speedrun::connectLiveSplit();
                        } else {
                            speedrun::disconnectLiveSplit();
                        }
                    },
                .isDisabled = [] { return !getSettings().game.speedrunMode; },
            });
    });

    add_tab("Cheats", [this](Rml::Element* content) {
        auto& leftPane = add_child<Pane>(content, Pane::Type::Controlled);
        auto& rightPane = add_child<Pane>(content, Pane::Type::Uncontrolled);

        auto addCheat = [&](const Rml::String& key, ConfigVar<bool>& value,
                            const Rml::String& helpText) {
            config_bool_select(leftPane, rightPane, value,
                {
                    .key = key,
                    .helpText = helpText,
                    .isDisabled = [] { return getSettings().game.speedrunMode; },
                });
        };

        leftPane.add_section("Resources");
        addCheat("Infinite Hearts", getSettings().game.infiniteHearts, "Keeps your health full.");
        addCheat(
            "Infinite Arrows", getSettings().game.infiniteArrows, "Keeps your arrow count full.");
        addCheat("Infinite Bombs", getSettings().game.infiniteBombs, "Keeps all bomb bags full.");
        addCheat("Infinite Oil", getSettings().game.infiniteOil, "Keeps your lantern oil full.");
        addCheat("Infinite Oxygen", getSettings().game.infiniteOxygen,
            "Keeps your underwater oxygen meter full.");
        addCheat(
            "Infinite Rupees", getSettings().game.infiniteRupees, "Keeps your rupee count full.");
        addCheat("No Item Timer", getSettings().game.enableIndefiniteItemDrops,
            "Item drops such as rupees and hearts will never disappear after they drop.");

        leftPane.add_section("Abilities");
        addCheat(
            "Moon Jump (R+A)", getSettings().game.moonJump, "Hold R and A to rise into the air.");
        addCheat("Super Clawshot", getSettings().game.superClawshot,
            "Extends clawshot behavior beyond the normal game rules.");
        addCheat("Always Greatspin", getSettings().game.alwaysGreatspin,
            "Allows the Great Spin attack without requiring full health.");
        addCheat("Fast Iron Boots", getSettings().game.enableFastIronBoots,
            "Speeds up movement while wearing the Iron Boots.");
        addCheat("Can Transform Anywhere", getSettings().game.canTransformAnywhere,
            "Allows transforming even if NPCs are looking.");
        addCheat("Fast Spinner", getSettings().game.fastSpinner,
            "Speeds up Spinner movement while holding R.");
        addCheat("Free Magic Armor", getSettings().game.freeMagicArmor,
            "Lets the magic armor work without consuming rupees.");
    });

    // TODO: Reorganize all of this?
    add_tab("Interface", [this](Rml::Element* content) {
        auto& leftPane = add_child<Pane>(content, Pane::Type::Controlled);
        auto& rightPane = add_child<Pane>(content, Pane::Type::Uncontrolled);

        config_bool_select(leftPane, rightPane, getSettings().game.enableAchievementNotifications,
            {
                .key = "Achievement Notifications",
                .helpText = "Display a toast when an achievement is unlocked.",
            });
#if DUSK_ENABLE_SENTRY_NATIVE
        config_bool_select(leftPane, rightPane, getSettings().backend.enableCrashReporting,
            {.key = "Crash Reporting",
                .helpText = "Enable automatic reporting of crashes to the developers.<br/><br/>"
                            "Submissions include logs which may contain sensitive information. "
                            "Refrain from "
                            "enabling reporting if you do not agree with the following "
                            "inclusions:<br/><br/> "
                            "- Operating System<br/>- CPU Architecture<br/>- GPU Model & Driver "
                            "Version<br/>"
                            "- Account Username"});
#endif
        config_bool_select(leftPane, rightPane, getSettings().backend.skipPreLaunchUI,
            {
                .key = "Skip Dusk Main Menu",
                .helpText = "When starting Dusk, skip the main menu and boot straight into the "
                            "game if a disc image is available.",
            });
        config_bool_select(leftPane, rightPane, getSettings().game.hideTvSettingsScreen,
            {
                .key = "Skip TV Settings Screen",
                .helpText = "Skips the TV calibration screen shown when loading a save.",
            });
        config_bool_select(leftPane, rightPane, getSettings().backend.showPipelineCompilation,
            {
                .key = "Show Pipeline Compilation",
                .helpText = "Show an overlay when shaders are being compiled for your hardware.",
            });
    });
}

void SettingsWindow::update() {
    if (mPrelaunch && top_document() == this) {
        try_push_verification_modal(*this);
    }

    Window::update();
}

}  // namespace dusk::ui
