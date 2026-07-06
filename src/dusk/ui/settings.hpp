#pragma once
#include "window.hpp"

namespace dusk::ui {

class SettingsWindow : public Window {
public:
    SettingsWindow(bool prelaunch = false);

    void update() override;
    void hide(bool close) override;

protected:
    bool mPrelaunch;
    // set when the UI language changes
    bool mPendingLanguageRefresh = false;
};

}  // namespace dusk::ui
