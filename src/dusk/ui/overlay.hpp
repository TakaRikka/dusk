#pragma once

#include "document.hpp"

#include <chrono>
#include <deque>

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
    clock::time_point mCurrentToastStartTime;
    clock::time_point mMenuNotificationStartTime;

    struct FpsFrameEvent {
        Uint64 endCounter;
        Uint64 processingTicks;
    };

    std::deque<FpsFrameEvent> mFpsFrameEvents;
    Uint64 mFpsSumTicks = 0;
    bool mFpsHavePrevCounter = false;
    Uint64 mFpsPrevCounter = 0;
    Uint64 mFpsLastUpdate = 0;

    void advance_fps_counter(float& outFps, Uint64 perfFreq);
};

}  // namespace dusk::ui
