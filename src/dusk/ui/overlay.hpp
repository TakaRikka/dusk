#pragma once

#include "document.hpp"

#include <chrono>

namespace dusk::ui {

class Overlay : public Document {
public:
    Overlay();

    void show() override;
    void update() override;

protected:
    bool handle_nav_command(Rml::Event& event, NavCommand cmd) override;

    Rml::Element* mFpsCounter = nullptr;
    Rml::Element* mCurrentToast = nullptr;
    Rml::Element* mControllerWarning = nullptr;
    Rml::Element* mMenuNotification = nullptr;
    Rml::Element* mSpeedrunTimer = nullptr;
    Rml::Element* mSpeedrunRta = nullptr;
    Rml::Element* mSpeedrunIgt = nullptr;
    clock::time_point mCurrentToastStartTime;
    clock::time_point mMenuNotificationStartTime;
    Uint64 mFpsLastUpdate = 0;
};

}  // namespace dusk::ui
