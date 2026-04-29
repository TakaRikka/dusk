#include "settings.hpp"

#include <fmt/format.h>

#include "aurora/gfx.h"
#include "dusk/audio/DuskAudioSystem.h"
#include "dusk/config.hpp"
#include "pane.hpp"
#include "ui.hpp"

namespace dusk::ui {

template <typename T>
struct ConfigProps {
    Rml::String key;
    ConfigVar<T>* value;
    std::function<void(T)> onChange;
    Rml::String helpText;
    Pane* rightPane = nullptr;
    bool selected = false;
};

class ConfigBoolSelect : public SelectButton {
public:
    using Props = ConfigProps<bool>;

    ConfigBoolSelect(Rml::Element* parent, Props props)
        : SelectButton(
              parent, {
                          .key = std::move(props.key),
                          .getValue = [this] { return mValue->getValue() ? "On" : "Off"; },
                          .onPressed = [this](SelectButton&, Rml::Event&) { toggle_value(); },
                          .selected = props.selected,
                      }),
          mValue(props.value), onChange(std::move(props.onChange)),
          mHelpText(std::move(props.helpText)), mRightPane(props.rightPane) {
        if (!mHelpText.empty() && mRightPane != nullptr) {
            listen(nullptr, Rml::EventId::Focus, [this](Rml::Event&) {
                mRightPane->clear();
                mRightPane->add_text(mHelpText);
            });
            listen(nullptr, Rml::EventId::Mouseover, [this](Rml::Event&) {
                mRightPane->clear();
                mRightPane->add_text(mHelpText);
            });
        }
        listen(nullptr, Rml::EventId::Keydown, [this](Rml::Event& event) {
            const auto cmd = map_nav_event(event);
            if (cmd == NavCommand::Left || cmd == NavCommand::Right || cmd == NavCommand::Confirm) {
                toggle_value();
                event.StopPropagation();
            }
        });
    }

private:
    void toggle_value() {
        const bool newValue = !mValue->getValue();
        mValue->setValue(newValue);
        if (onChange) {
            onChange(newValue);
        }
        config::Save();
    }

    ConfigVar<bool>* mValue;
    std::function<void(bool)> onChange;
    Rml::String mHelpText;
    Pane* mRightPane;
};

SettingsWindow::SettingsWindow() {
    add_tab("Audio", [this](Rml::Element* content) {
        auto& leftPane = add_child<Pane>(content, Pane::Direction::Vertical);
        auto& rightPane = add_child<Pane>(content, Pane::Direction::Vertical);

        leftPane.add_section("Volume");
        {
            auto& btn = leftPane.add_select_button({
                .key = "Master Volume",
                .getValue =
                    [] { return fmt::format("{}%", getSettings().audio.masterVolume.getValue()); },
            });
            btn.listen(nullptr, Rml::EventId::Focus, [&](Rml::Event&) {
                rightPane.clear();
                rightPane.add_text("Adjusts the volume of all sounds in the game.");
            });
            btn.listen(nullptr, Rml::EventId::Mouseover, [&](Rml::Event&) {
                rightPane.clear();
                rightPane.add_text("Adjusts the volume of all sounds in the game.");
            });
        }

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