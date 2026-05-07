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

    Rml::Element* mCurrentToast = nullptr;
    Rml::Element* mControllerWarning = nullptr;
    Rml::Element* mMenuNotification = nullptr;
    clock::time_point mCurrentToastStartTime;
    clock::time_point mMenuNotificationStartTime;
};

}  // namespace dusk::ui
