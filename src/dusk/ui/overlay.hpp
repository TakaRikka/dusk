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

private:
    void update_pipeline_progress();

    Rml::Element* mFpsCounter = nullptr;
    Rml::Element* mPipelineProgress = nullptr;
    Rml::Element* mPipelineProgressLabel = nullptr;
    Rml::Element* mPipelineProgressBar = nullptr;
    Rml::Element* mCurrentToast = nullptr;
    Rml::Element* mControllerWarning = nullptr;
    Rml::Element* mMenuNotification = nullptr;
    Rml::Element* mSpeedrunTimer = nullptr;
    Rml::Element* mSpeedrunRta = nullptr;
    Rml::Element* mSpeedrunIgt = nullptr;
    clock::time_point mCurrentToastStartTime;
    clock::time_point mMenuNotificationStartTime;
    clock::time_point mPipelineProgressStartTime;
    Uint64 mFpsLastUpdate = 0;
    uint32_t mPipelineBatchCreatedBase = 0;
    uint32_t mLastQueuedPipelines = 0;
    bool mPipelineProgressActive = false;
};

}  // namespace dusk::ui
