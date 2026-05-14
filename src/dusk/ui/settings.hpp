#pragma once
#include "window.hpp"

namespace dusk::ui {

class SettingsWindow : public Window {
public:
    SettingsWindow(bool prelaunch = false);

    void AddPrelaunchTab();

    void AddVideoTab();

    void AddInputTab();

    void AddAudioTab();

    void AddGameplayTab();

    void AddCheatsTab();

    void AddInterfaceTab();

    void update() override;

protected:
    bool mPrelaunch;
};

}  // namespace dusk::ui