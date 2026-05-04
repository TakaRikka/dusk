#include "settings.hpp"

#include "aurora/gfx.h"
#include "bool_button.hpp"
#include "controller_config.hpp"
#include "dusk/audio/DuskAudioSystem.h"
#include "dusk/audio/DuskDsp.hpp"
#include "dusk/config.hpp"
#include "dusk/imgui/ImGuiEngine.hpp"
#include "dusk/livesplit.h"
#include "m_Do/m_Do_main.h"
#include "number_button.hpp"
#include "overlay.hpp"
#include "pane.hpp"
#include "ui.hpp"

#include <algorithm>

namespace dusk::ui {
namespace {

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

int bloom_multiplier_percent() {
    return std::clamp(
        static_cast<int>(getSettings().game.bloomMultiplier.getValue() * 100.0f + 0.5f), 0, 100);
}

int float_setting_percent(ConfigVar<float>& var) {
    return static_cast<int>(var.getValue() * 100.0f + 0.5f);
}

bool gyro_enabled() {
    return getSettings().game.enableGyroAim || getSettings().game.enableGyroRollgoal;
}

struct ConfigBoolProps {
    Rml::String key;
    Rml::String helpText;
    std::function<void(bool)> onChange;
    std::function<bool()> isDisabled;
};

SelectButton& config_bool_select(
    Pane& leftPane, Pane& rightPane, ConfigVar<bool>& var, ConfigBoolProps props) {
    return leftPane
        .add_child<BoolButton>(BoolButton::Props{.key = std::move(props.key),
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
            .isModified = [&var] { return var.getValue() != var.getDefaultValue(); }})
        .on_focus([&rightPane, helpText = std::move(props.helpText)](Rml::Event&) {
            rightPane.clear();
            rightPane.add_rml(helpText);
        });
}

SelectButton& config_percent_select(Pane& leftPane, Pane& rightPane, ConfigVar<float>& var,
    Rml::String key, Rml::String helpText, int min, int max, int step = 5,
    std::function<bool()> isDisabled = {}) {
    return leftPane
        .add_child<NumberButton>(NumberButton::Props{
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
        })
        .on_focus([&rightPane, helpText = std::move(helpText)](Rml::Event&) {
            rightPane.clear();
            rightPane.add_text(helpText);
        });
}

}  // namespace

SettingsWindow::SettingsWindow() {
    add_tab("Audio", [this](Rml::Element* content) {
        auto& leftPane = add_child<Pane>(content, Pane::Type::Controlled);
        auto& rightPane = add_child<Pane>(content, Pane::Type::Uncontrolled);

        // TODO: Individual sliders for Main Music, Sub Music, Sound Effects, and Fanfare.
        leftPane.add_section("Volume");
        leftPane
            .add_child<NumberButton>(NumberButton::Props{
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
            })
            .on_focus([&rightPane](Rml::Event&) {
                rightPane.clear();
                rightPane.add_text("Adjusts the volume of all sounds in the game.");
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
                .helpText = "Emulate surround sound via HRTF. Recommended only for use with headphones!",
                .onChange = [](bool value) { audio::EnableHrtf = value; },
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
        addCheat("Infinite Hearts", getSettings().game.infiniteHearts,
            "Keeps your health full.");
        addCheat("Infinite Arrows", getSettings().game.infiniteArrows,
            "Keeps your arrow count full.");
        addCheat("Infinite Bombs", getSettings().game.infiniteBombs,
            "Keeps all bomb bags full.");
        addCheat("Infinite Oil", getSettings().game.infiniteOil,
            "Keeps your lantern oil full.");
        addCheat("Infinite Oxygen", getSettings().game.infiniteOxygen,
            "Keeps your underwater oxygen meter full.");
        addCheat("Infinite Rupees", getSettings().game.infiniteRupees,
            "Keeps your rupee count full.");
        addCheat("No Item Timer", getSettings().game.enableIndefiniteItemDrops,
            "Item drops such as rupees and hearts will never disappear after they drop.");

        leftPane.add_section("Abilities");
        addCheat("Moon Jump (R+A)", getSettings().game.moonJump,
            "Hold R and A to rise into the air.");
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
        leftPane
            .add_child<NumberButton>(NumberButton::Props{
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
                .prefix = "x",
            })
            .on_focus([&rightPane](Rml::Event&) {
                rightPane.clear();
                rightPane.add_text("Multiplies incoming damage.");
            });
        addSpeedrunDisabledOption("Instant Death", getSettings().game.instantDeath,
            "Any hit will instantly kill you.");
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
        addOption("Autosave", getSettings().game.autoSave,
            "Autosaves the game when going to a new area, opening a dungeon door, or getting "
            "a new item.<br/><br/>This feature is currently experimental, use at your own risk.");
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
        addOption("Skip TV Settings Screen", getSettings().game.hideTvSettingsScreen,
            "Skips the TV calibration screen shown when loading a save.");
        addOption("Skip Warning Screen", getSettings().game.skipWarningScreen,
            "Skips the warning screen shown when starting the game.");
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
        leftPane.add_button("Configure Controller")
            .on_pressed([this] { push(std::make_unique<ControllerConfigWindow>()); })
            .on_focus([&rightPane](Rml::Event&) {
                rightPane.clear();
                rightPane.add_text("Open controller binding configuration.");
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

    add_tab("Graphics", [this](Rml::Element* content) {
        auto& leftPane = add_child<Pane>(content, Pane::Type::Controlled);
        auto& rightPane = add_child<Pane>(content, Pane::Type::Uncontrolled);

        leftPane.add_section("Display");

        leftPane.add_button("Toggle Fullscreen").on_pressed([] {
            getSettings().video.enableFullscreen.setValue(!getSettings().video.enableFullscreen);
            VISetWindowFullscreen(getSettings().video.enableFullscreen);
            config::Save();
        });
        leftPane.add_button("Restore Default Window Size").on_pressed([] {
            getSettings().video.enableFullscreen.setValue(false);
            VISetWindowFullscreen(false);
            VISetWindowSize(FB_WIDTH * 2, FB_HEIGHT * 2);
            VICenterWindow();
        });
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
        leftPane
            .add_select_button({
                .key = "Internal Resolution",
                .getValue =
                    [] {
                        return format_graphics_setting_value(GraphicsOption::InternalResolution,
                            getSettings().game.internalResolutionScale.getValue());
                    },
                .isModified =
                    [] {
                        return getSettings().game.internalResolutionScale.getValue() !=
                               getSettings().game.internalResolutionScale.getDefaultValue();
                    },
            })
            .on_nav_command([this](Rml::Event&, NavCommand cmd) {
                if (cmd == NavCommand::Confirm || cmd == NavCommand::Left ||
                    cmd == NavCommand::Right) {
                    push(std::make_unique<Overlay>(OverlayProps{
                        .option = GraphicsOption::InternalResolution,
                        .title = "Internal Resolution",
                        .helpText = kInternalResolutionHelpText,
                        .valueMin = 0,
                        .valueMax = 12,
                        .defaultValue = 0,
                    }));
                    return true;
                }
                return false;
            })
            .on_focus([&rightPane](Rml::Event&) {
                rightPane.clear();
                rightPane.add_text(kInternalResolutionHelpText);
            });
        leftPane
            .add_select_button({
                .key = "Shadow Resolution",
                .getValue =
                    [] {
                        return format_graphics_setting_value(GraphicsOption::ShadowResolution,
                            getSettings().game.shadowResolutionMultiplier.getValue());
                    },
                .isModified =
                    [] {
                        return getSettings().game.shadowResolutionMultiplier.getValue() !=
                               getSettings().game.shadowResolutionMultiplier.getDefaultValue();
                    },
            })
            .on_nav_command([this](Rml::Event&, NavCommand cmd) {
                if (cmd == NavCommand::Confirm || cmd == NavCommand::Left ||
                    cmd == NavCommand::Right) {
                    push(std::make_unique<Overlay>(OverlayProps{
                        .option = GraphicsOption::ShadowResolution,
                        .title = "Shadow Resolution",
                        .helpText = kShadowResolutionHelpText,
                        .valueMin = 1,
                        .valueMax = 8,
                        .defaultValue = 1,
                    }));
                    return true;
                }
                return false;
            })
            .on_focus([&rightPane](Rml::Event&) {
                rightPane.clear();
                rightPane.add_text(kShadowResolutionHelpText);
            });

        leftPane.add_section("Post-Processing");
        leftPane
            .add_select_button({
                .key = "Bloom",
                .getValue =
                    [] {
                        return format_graphics_setting_value(GraphicsOption::BloomMode,
                            static_cast<int>(getSettings().game.bloomMode.getValue()));
                    },
                .isModified =
                    [] {
                        return getSettings().game.bloomMode.getValue() !=
                               getSettings().game.bloomMode.getDefaultValue();
                    },
            })
            .on_nav_command([this](Rml::Event&, NavCommand cmd) {
                if (cmd == NavCommand::Confirm || cmd == NavCommand::Left ||
                    cmd == NavCommand::Right) {
                    push(std::make_unique<Overlay>(OverlayProps{
                        .option = GraphicsOption::BloomMode,
                        .title = "Bloom",
                        .helpText = kBloomHelpText,
                        .valueMin = static_cast<int>(BloomMode::Off),
                        .valueMax = static_cast<int>(BloomMode::Dusk),
                        .defaultValue = static_cast<int>(BloomMode::Classic),
                    }));
                    return true;
                }
                return false;
            })
            .on_focus([&rightPane](Rml::Event&) {
                rightPane.clear();
                rightPane.add_text(kBloomHelpText);
            });
        leftPane
            .add_select_button({
                .key = "Bloom Brightness",
                .getValue =
                    [] {
                        return format_graphics_setting_value(
                            GraphicsOption::BloomMultiplier, bloom_multiplier_percent());
                    },
                .isDisabled =
                    [] { return getSettings().game.bloomMode.getValue() == BloomMode::Off; },
                .isModified =
                    [] {
                        return getSettings().game.bloomMultiplier.getValue() !=
                               getSettings().game.bloomMultiplier.getDefaultValue();
                    },
            })
            .on_nav_command([this](Rml::Event&, NavCommand cmd) {
                if (cmd == NavCommand::Confirm || cmd == NavCommand::Left ||
                    cmd == NavCommand::Right) {
                    push(std::make_unique<Overlay>(OverlayProps{
                        .option = GraphicsOption::BloomMultiplier,
                        .title = "Bloom Brightness",
                        .helpText = kBloomBrightnessHelpText,
                        .valueMin = 0,
                        .valueMax = 100,
                        .defaultValue = 100,
                    }));
                    return true;
                }
                return false;
            })
            .on_focus([&rightPane](Rml::Event&) {
                rightPane.clear();
                rightPane.add_text(kBloomBrightnessHelpText);
            });

        leftPane.add_section("Rendering");
        config_bool_select(leftPane, rightPane, getSettings().game.enableFrameInterpolation,
            {
                .key = "Unlock Framerate",
                .helpText =
                    "Uses inter-frame interpolation to enable higher frame rates.<br/><br/>Visual "
                    "artifacts, animation glitches, or instability may occur.",
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

    // TODO: Reorganize all of this?
    add_tab("Interface", [this](Rml::Element* content) {
        auto& leftPane = add_child<Pane>(content, Pane::Type::Controlled);
        auto& rightPane = add_child<Pane>(content, Pane::Type::Uncontrolled);

        config_bool_select(leftPane, rightPane, getSettings().game.enableAchievementNotifications,
            {
                .key = "Enable Achievement Notifications",
                .helpText = "Display a toast when an achievement is unlocked.",
            });
#if DUSK_ENABLE_SENTRY_NATIVE
        config_bool_select(leftPane, rightPane, getSettings().backend.enableCrashReporting,
            {
                .key = "Enable Crash Reporting",
                .helpText = "Enable automatic reporting of crashes to the developers.<br/><br/>"
                "Submissions include logs which may contain sensitive information. Refrain from "
                "enabling reporting if you do not agree with the following inclusions:<br/><br/> "
                "- Operating System<br/>- CPU Architecture<br/>- GPU Model & Driver Version<br/>"
                "- Account Username"
            });
#endif
        config_bool_select(leftPane, rightPane, getSettings().backend.skipPreLaunchUI,
            {
                .key = "Skip Pre-Launch UI",
            });
        config_bool_select(leftPane, rightPane, getSettings().backend.showPipelineCompilation,
            {
                .key = "Show Pipeline Compilation",
                .helpText = "Show an overlay when shaders are being compiled for your hardware."
            });
    });
}

}  // namespace dusk::ui
