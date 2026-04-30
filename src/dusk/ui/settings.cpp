#include "settings.hpp"

#include <fmt/format.h>

#include "aurora/gfx.h"
#include "dusk/audio/DuskAudioSystem.h"
#include "dusk/config.hpp"
#include "pane.hpp"
#include "ui.hpp"

#include <algorithm>

namespace dusk::ui {

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
                mRightPane->add_text(mHelpText);
            });
            listen(Rml::EventId::Mouseover, [this](Rml::Event&) {
                mRightPane->clear();
                mRightPane->add_text(mHelpText);
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

class ConfigIntPercentSelect : public ConfigSelect<int> {
public:
    ConfigIntPercentSelect(Rml::Element* parent, Props props)
        : ConfigSelect(parent, std::move(props)) {}

protected:
    Rml::String get_value() override { return fmt::format("{}%", mVar->getValue()); }
    bool handle_nav_command(NavCommand cmd) override {
        if (cmd == NavCommand::Left) {
            set_value(std::max(mVar->getValue() - 1, 0));
            return true;
        } else if (cmd == NavCommand::Right) {
            set_value(std::min(mVar->getValue() + 1, 100));
            return true;
        }
        return false;
    }
};

SettingsWindow::SettingsWindow() {
    add_tab("Audio", [this](Rml::Element* content) {
        auto& leftPane = add_child<Pane>(content, Pane::Direction::Vertical);
        auto& rightPane = add_child<Pane>(content, Pane::Direction::Vertical);

        leftPane.add_section("Volume");
        leftPane.add_child<ConfigIntPercentSelect>(leftPane.root(),
            ConfigIntPercentSelect::Props{
                .key = "Master Volume",
                .value = &getSettings().audio.masterVolume,
                .onChange = [](int value) { audio::SetMasterVolume(value / 100.f); },
                .helpText = "Adjusts the volume of all sounds in the game.",
                .rightPane = &rightPane,
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
                                     .isDisabled = [] { return !getSettings().game.speedrunMode; },
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
    });
}

}  // namespace dusk::ui
