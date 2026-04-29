#include "settings.hpp"

#include <fmt/format.h>

#include "aurora/gfx.h"
#include "dusk/audio/DuskAudioSystem.h"
#include "dusk/config.hpp"
#include "pane.hpp"

namespace dusk::ui {

SettingsWindow::SettingsWindow() {
    add_tab("Audio", [this](Rml::Element* content) {
        auto& leftPane = add_child<Pane>(content);
        auto& rightPane = add_child<Pane>(content);

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
        {
            auto& btn = leftPane.add_select_button({
                .key = "Enable Reverb",
                .getValue = [] { return getSettings().audio.enableReverb ? "On" : "Off"; },
                .onPressed =
                    [](SelectButton& self, Rml::Event& event) {
                        getSettings().audio.enableReverb.setValue(
                            !getSettings().audio.enableReverb);
                        dusk::audio::SetEnableReverb(getSettings().audio.enableReverb);
                        config::Save();
                    },
            });
            btn.listen(nullptr, Rml::EventId::Focus, [&](Rml::Event&) {
                rightPane.clear();
                rightPane.add_text("Enables the reverb effect in game audio.");
            });
            btn.listen(nullptr, Rml::EventId::Mouseover, [&](Rml::Event&) {
                rightPane.clear();
                rightPane.add_text("Enables the reverb effect in game audio.");
            });
        }

        leftPane.add_section("Tweaks");
        leftPane.add_select_button({
            .key = "No Low HP Sound",
            .value = "Off",
        });
        leftPane.add_select_button({
            .key = "Non-Stop Midna's Lament",
            .value = "On",
        });
    });

    add_tab("Cheats", [this](Rml::Element* content) {

    });

    add_tab("Gameplay", [this](Rml::Element* content) {

    });

    add_tab("Graphics", [this](Rml::Element* content) {
        auto& leftPane = add_child<Pane>(content);
        auto& rightPane = add_child<Pane>(content);

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
        leftPane.add_select_button({
            .key = "Enable VSync",
            .getValue = [] { return getSettings().video.enableVsync ? "On" : "Off"; },
            .onPressed =
                [](SelectButton&, Rml::Event&) {
                    getSettings().video.enableVsync.setValue(!getSettings().video.enableVsync);
                    aurora_enable_vsync(getSettings().video.enableVsync);
                    config::Save();
                },
        });
        leftPane.add_select_button({
            .key = "Force 4:3 Aspect Ratio",
            .getValue = [] { return getSettings().video.lockAspectRatio ? "On" : "Off"; },
            .onPressed =
                [](SelectButton&, Rml::Event&) {
                    getSettings().video.lockAspectRatio.setValue(
                        !getSettings().video.lockAspectRatio);
                    if (getSettings().video.lockAspectRatio) {
                        AuroraSetViewportPolicy(AURORA_VIEWPORT_FIT);
                    } else {
                        AuroraSetViewportPolicy(AURORA_VIEWPORT_STRETCH);
                    }
                    config::Save();
                },
        });

        leftPane.add_section("Resolution");
    });
}

}  // namespace dusk::ui