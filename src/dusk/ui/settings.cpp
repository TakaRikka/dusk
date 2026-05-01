#include "settings.hpp"

#include <fmt/format.h>

#include "aurora/gfx.h"
#include "dusk/audio/DuskAudioSystem.h"
#include "dusk/config.hpp"
#include "dusk/livesplit.h"
#include "m_Do/m_Do_main.h"
#include "overlay.hpp"
#include "pane.hpp"
#include "ui.hpp"

#include <climits>

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

}  // namespace

template <typename T>
struct ConfigProps {
    Rml::String key;
    ConfigVar<T>* value;
    std::function<void(T)> onChange;
    std::function<bool()> isDisabled;
    Rml::String helpText;
    Pane* rightPane = nullptr;
};

template <typename T>
class ConfigSelect : public SelectButton {
public:
    using Props = ConfigProps<T>;

    ConfigSelect(Rml::Element* parent, Props props)
        : SelectButton(parent, {std::move(props.key)}), mVar(props.value),
          mOnChange(std::move(props.onChange)), mIsDisabled(std::move(props.isDisabled)),
          mHelpText(std::move(props.helpText)), mRightPane(props.rightPane) {
        if (!mHelpText.empty() && mRightPane != nullptr) {
            listen(Rml::EventId::Focus, [this](Rml::Event&) {
                mRightPane->clear();
                mRightPane->add_rml(mHelpText);
            });
            listen(Rml::EventId::Mouseover, [this](Rml::Event&) {
                mRightPane->clear();
                mRightPane->add_rml(mHelpText);
            });
        }
    }

    void update() override {
        if (mIsDisabled) {
            set_disabled(mIsDisabled());
        }
        set_value_label(get_value());
        SelectButton::update();
    }

protected:
    virtual Rml::String get_value() = 0;

    void set_value(T newValue) {
        if (mVar->getValue() == newValue) {
            return;
        }
        mVar->setValue(newValue);
        if (mOnChange) {
            mOnChange(newValue);
        }
        config::Save();
    }

    ConfigVar<T>* mVar;
    std::function<void(T)> mOnChange;
    std::function<bool()> mIsDisabled;
    Rml::String mHelpText;
    Pane* mRightPane;
};

class ConfigBoolSelect : public ConfigSelect<bool> {
public:
    ConfigBoolSelect(Rml::Element* parent, Props props) : ConfigSelect(parent, std::move(props)) {}

protected:
    Rml::String get_value() override { return mVar->getValue() ? "On" : "Off"; }
    bool handle_nav_command(NavCommand cmd) override {
        if (cmd == NavCommand::Confirm || cmd == NavCommand::Left || cmd == NavCommand::Right) {
            set_value(!mVar->getValue());
            return true;
        }
        return false;
    }
};

class ConfigIntSelect : public ConfigSelect<int> {
public:
    struct Props {
        Rml::String key;
        ConfigVar<int>* value;
        std::function<void(int)> onChange;
        std::function<bool()> isDisabled;
        Rml::String helpText;
        Pane* rightPane = nullptr;
        int min = 0;
        int max = INT_MAX;
        int step = 1;
        Rml::String prefix;
        Rml::String suffix;
    };

    ConfigIntSelect(Rml::Element* parent, Props props)
        : ConfigSelect(parent,
              {
                  .key = std::move(props.key),
                  .value = props.value,
                  .onChange = std::move(props.onChange),
                  .isDisabled = std::move(props.isDisabled),
                  .helpText = std::move(props.helpText),
                  .rightPane = props.rightPane,
              }),
          mMin(props.min), mMax(props.max), mStep(props.step), mPrefix(std::move(props.prefix)),
          mSuffix(std::move(props.suffix)) {}

protected:
    Rml::String get_value() override {
        return fmt::format("{}{}{}", mPrefix, mVar->getValue(), mSuffix);
    }

    bool handle_nav_command(NavCommand cmd) override {
        if (cmd == NavCommand::Left) {
            set_value(std::clamp(mVar->getValue() - mStep, mMin, mMax));
            return true;
        } else if (cmd == NavCommand::Right) {
            set_value(std::clamp(mVar->getValue() + mStep, mMin, mMax));
            return true;
        }
        return false;
    }

private:
    int mMin;
    int mMax;
    int mStep;
    Rml::String mPrefix;
    Rml::String mSuffix;
};

class ConfigOverlaySelect : public ConfigSelect<int> {
public:
    struct Props {
        Rml::String key;
        ConfigVar<int>* value;
        GraphicsOption option;
        Rml::String title;
        int min = 0;
        int max = 0;
        Rml::String helpText;
        Pane* rightPane = nullptr;
        std::function<bool()> isDisabled;
    };

    ConfigOverlaySelect(Rml::Element* parent, Props props)
        : ConfigSelect<int>(parent,
              {
                  .key = std::move(props.key),
                  .value = props.value,
                  .onChange = {},
                  .isDisabled = std::move(props.isDisabled),
                  .helpText = std::move(props.helpText),
                  .rightPane = props.rightPane,
              }),
          mOption(props.option), mTitle(std::move(props.title)), mValueMin(props.min),
          mValueMax(props.max) {}

protected:
    Rml::String get_value() override {
        return format_graphics_setting_value(mOption, mVar->getValue());
    }

    bool handle_nav_command(NavCommand cmd) override {
        if (cmd == NavCommand::Confirm) {
            open_overlay();
            return true;
        }
        return false;
    }

private:
    void open_overlay() {
        push_document(std::make_unique<Overlay>(OverlayProps{
            .option = mOption,
            .title = mTitle,
            .helpText = mHelpText,
            .valueMin = mValueMin,
            .valueMax = mValueMax,
        }));
    }

    GraphicsOption mOption;
    Rml::String mTitle;
    int mValueMin = 0;
    int mValueMax = 0;
};

SettingsWindow::SettingsWindow() {
    add_tab("Audio", [this](Rml::Element* content) {
        auto& leftPane = add_child<Pane>(content, Pane::Direction::Vertical);
        auto& rightPane = add_child<Pane>(content, Pane::Direction::Vertical);

        leftPane.add_section("Volume");
        leftPane.add_child<ConfigIntSelect>(leftPane.root(),
            ConfigIntSelect::Props{
                .key = "Master Volume",
                .value = &getSettings().audio.masterVolume,
                .onChange = [](int value) { audio::SetMasterVolume(value / 100.f); },
                .helpText = "Adjusts the volume of all sounds in the game.",
                .rightPane = &rightPane,
                .max = 100,
                .suffix = "%",
            });

        leftPane.add_section("Effects");
        leftPane.add_child<ConfigBoolSelect>(
            leftPane.root(), ConfigBoolSelect::Props{
                                 .key = "Enable Reverb",
                                 .value = &getSettings().audio.enableReverb,
                                 .onChange = [](bool value) { audio::SetEnableReverb(value); },
                                 .helpText = "Enables the reverb effect in game audio.",
                                 .rightPane = &rightPane,
                             });

        leftPane.add_section("Tweaks");
        leftPane.add_child<ConfigBoolSelect>(
            leftPane.root(), ConfigBoolSelect::Props{
                                 .key = "No Low HP Sound",
                                 .value = &getSettings().game.noLowHpSound,
                                 .helpText = "Disable the beeping sound when having low health.",
                                 .rightPane = &rightPane,
                             });
        leftPane.add_child<ConfigBoolSelect>(leftPane.root(),
            ConfigBoolSelect::Props{
                .key = "Non-Stop Midna's Lament",
                .value = &getSettings().game.midnasLamentNonStop,
                .helpText = "Prevents enemy music while Midna's Lament is playing.",
                .rightPane = &rightPane,
            });
    });

    add_tab("Cheats", [this](Rml::Element* content) {
        auto& leftPane = add_child<Pane>(content, Pane::Direction::Vertical);
        auto& rightPane = add_child<Pane>(content, Pane::Direction::Vertical);

        auto addCheat = [&](const Rml::String& key, ConfigVar<bool>& value,
                            const Rml::String& helpText) {
            leftPane.add_child<ConfigBoolSelect>(
                leftPane.root(), ConfigBoolSelect::Props{
                                     .key = key,
                                     .value = &value,
                                     .isDisabled = [] { return getSettings().game.speedrunMode; },
                                     .helpText = helpText,
                                     .rightPane = &rightPane,
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

    add_tab("Gameplay", [this](Rml::Element* content) {
        auto& leftPane = add_child<Pane>(content, Pane::Direction::Vertical);
        auto& rightPane = add_child<Pane>(content, Pane::Direction::Vertical);

        auto addOption = [&](const Rml::String& key, ConfigVar<bool>& value,
                             const Rml::String& helpText) {
            leftPane.add_child<ConfigBoolSelect>(leftPane.root(), ConfigBoolSelect::Props{
                                                                      .key = key,
                                                                      .value = &value,
                                                                      .helpText = helpText,
                                                                      .rightPane = &rightPane,
                                                                  });
        };

        auto addSpeedrunDisabledOption = [&](const Rml::String& key, ConfigVar<bool>& value,
                                             const Rml::String& helpText) {
            leftPane.add_child<ConfigBoolSelect>(
                leftPane.root(), ConfigBoolSelect::Props{
                                     .key = key,
                                     .value = &value,
                                     .isDisabled = [] { return getSettings().game.speedrunMode; },
                                     .helpText = helpText,
                                     .rightPane = &rightPane,
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
        leftPane.add_child<ConfigIntSelect>(
            leftPane.root(), ConfigIntSelect::Props{
                                 .key = "Damage Multiplier",
                                 .value = &getSettings().game.damageMultiplier,
                                 .isDisabled = [] { return getSettings().game.speedrunMode; },
                                 .helpText = "Multiplies incoming damage.",
                                 .rightPane = &rightPane,
                                 .min = 1,
                                 .max = 8,
                                 .prefix = "x",
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
        addOption("Instant Saves", getSettings().game.instantSaves,
            "Skips the delay when writing to the Memory Card.");
        addOption("Hold B for Instant Text", getSettings().game.instantText,
            "Makes text scroll immediately by holding B.");
        addOption("No Climbing Miss Animation", getSettings().game.noMissClimbing,
            "Prevents Link from playing a struggle animation when grabbing ledges or climbing on "
            "vines.");
        addOption("No Rupee Returns", getSettings().game.noReturnRupees,
            "Always collect Rupees even if your Wallet is too full.");
        addOption("No Sword Recoil", getSettings().game.noSwordRecoil,
            "Link will not recoil when his sword hits walls.");
        addOption("Skip TV Settings Screen", getSettings().game.hideTvSettingsScreen,
            "Skips the TV calibration screen shown when loading a save.");
        addOption("Skip Warning Screen", getSettings().game.skipWarningScreen,
            "Skips the warning screen shown when starting the game.");
        addOption("Sun's Song (R+X)", getSettings().game.sunsSong,
            "Allows Wolf Link to howl and change the time of day.");
        addOption("Quick Transform (R+Y)", getSettings().game.enableQuickTransform,
            "Transform instantly by pressing R and Y simultaneously.");

        leftPane.add_section("Speedrunning");
        leftPane.add_child<ConfigBoolSelect>(leftPane.root(),
            ConfigBoolSelect::Props{
                .key = "Speedrun Mode",
                .value = &getSettings().game.speedrunMode,
                .onChange = [](bool) { reset_for_speedrun_mode(); },
                .helpText = "Enables speedrunning options while restricting certain "
                            "gameplay modifiers.",
                .rightPane = &rightPane,
            });
        leftPane.add_child<ConfigBoolSelect>(
            leftPane.root(), ConfigBoolSelect::Props{
                                 .key = "LiveSplit Connection",
                                 .value = &getSettings().game.liveSplitEnabled,
                                 .onChange =
                                     [](bool enabled) {
                                         if (enabled) {
                                             speedrun::connectLiveSplit();
                                         } else {
                                             speedrun::disconnectLiveSplit();
                                         }
                                     },
                                 .isDisabled = [] { return !getSettings().game.speedrunMode; },
                                 .helpText = "Connect to LiveSplit server on localhost:16834.",
                                 .rightPane = &rightPane,
                             });
    });

    add_tab("Graphics", [this](Rml::Element* content) {
        auto& leftPane = add_child<Pane>(content, Pane::Direction::Vertical);
        auto& rightPane = add_child<Pane>(content, Pane::Direction::Vertical);

        leftPane.add_section("Display");

        leftPane.add_button({
            .text = "Toggle Fullscreen",
            .onPressed =
                [](Rml::Event&) {
                    getSettings().video.enableFullscreen.setValue(
                        !getSettings().video.enableFullscreen);
                    VISetWindowFullscreen(getSettings().video.enableFullscreen);
                    config::Save();
                },
        });
        leftPane.add_button({
            .text = "Restore Default Window Size",
            .onPressed =
                [](Rml::Event&) {
                    getSettings().video.enableFullscreen.setValue(false);
                    VISetWindowFullscreen(false);
                    VISetWindowSize(FB_WIDTH * 2, FB_HEIGHT * 2);
                    VICenterWindow();
                },
        });
        leftPane.add_child<ConfigBoolSelect>(leftPane.root(),
            ConfigBoolSelect::Props{
                .key = "Enable VSync",
                .value = &getSettings().video.enableVsync,
                .onChange = [](bool value) { aurora_enable_vsync(value); },
                .helpText = "Synchronizes the frame rate to your monitor's refresh rate.",
                .rightPane = &rightPane,
            });
        leftPane.add_child<ConfigBoolSelect>(
            leftPane.root(), ConfigBoolSelect::Props{
                                 .key = "Lock 4:3 Aspect Ratio",
                                 .value = &getSettings().video.lockAspectRatio,
                                 .onChange =
                                     [](bool value) {
                                         AuroraSetViewportPolicy(
                                             value ? AURORA_VIEWPORT_FIT : AURORA_VIEWPORT_STRETCH);
                                     },
                                 .helpText = "Lock the game's aspect ratio to the original.",
                                 .rightPane = &rightPane,
                             });

        leftPane.add_section("Resolution");
        leftPane.add_child<ConfigOverlaySelect>(leftPane.root(),
            ConfigOverlaySelect::Props{
                .key = "Internal Resolution",
                .value = &getSettings().game.internalResolutionScale,
                .option = GraphicsOption::InternalResolution,
                .title = "Internal Resolution",
                .min = 0,
                .max = 12,
                .helpText =
                    "Configure the resolution used for rendering the game. Higher values are more "
                    "demanding on your graphics hardware.",
                .rightPane = &rightPane,
            });
        leftPane.add_child<ConfigOverlaySelect>(leftPane.root(),
            ConfigOverlaySelect::Props{
                .key = "Shadow Resolution",
                .value = &getSettings().game.shadowResolutionMultiplier,
                .option = GraphicsOption::ShadowResolution,
                .title = "Shadow Resolution",
                .min = 1,
                .max = 8,
                .helpText =
                    "Configure the shadow-map resolution. Higher values improve shadow quality but "
                    "increase GPU and memory usage.",
                .rightPane = &rightPane,
            });

        leftPane.add_section("Post-Processing");
        // TODO: Bloom
        // TODO: Bloom Brightness

        leftPane.add_section("Rendering");
        leftPane.add_child<ConfigBoolSelect>(leftPane.root(),
            ConfigBoolSelect::Props{
                .key = "Unlock Framerate",
                .value = &getSettings().game.enableFrameInterpolation,
                .helpText =
                    "Uses inter-frame interpolation to enable higher frame rates.<br/><br/>Visual "
                    "artifacts, animation glitches, or instability may occur.",
                .rightPane = &rightPane,
            });
        leftPane.add_child<ConfigBoolSelect>(
            leftPane.root(), ConfigBoolSelect::Props{
                                 .key = "Enable Depth of Field",
                                 .value = &getSettings().game.enableDepthOfField,
                             });
        leftPane.add_child<ConfigBoolSelect>(
            leftPane.root(), ConfigBoolSelect::Props{
                                 .key = "Enable Mini-Map Shadows",
                                 .value = &getSettings().game.enableMapBackground,
                             });
    });
}

}  // namespace dusk::ui
