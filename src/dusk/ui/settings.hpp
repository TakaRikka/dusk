#pragma once
#include "window.hpp"

namespace dusk::ui {

class SettingsWindow : public Window {
public:
    SettingsWindow(bool prelaunch = false);

    void add_prelaunch_tab();

    void add_video_tab();

    void add_input_tab();

    void add_audio_tab();

    void add_gameplay_tab();

    void add_cheats_tab();

    void add_interface_tab();

    void update() override;

protected:
    bool mPrelaunch;
};

}  // namespace dusk::ui