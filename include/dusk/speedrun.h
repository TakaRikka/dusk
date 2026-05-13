#pragma once
#include <aurora/aurora.h>

namespace dusk {

struct SpeedrunInfo {
    void startRun() {
        m_isRunStarted = true;
        m_startTimestamp = OSGetTime();
    }

    void stopRun() {
        m_isRunStarted = false;
        m_endTimestamp = OSGetTime() - m_startTimestamp;
    }

    void reset() {
        m_isRunStarted = false;
        m_startTimestamp = 0;
        m_endTimestamp = 0;
        m_isPauseIGT = false;
        m_loadStartTimestamp = 0;
        m_totalLoadTime = 0;
        m_igtTimer = 0;
    }

    bool m_isRunStarted = false;
    OSTime m_startTimestamp = 0;
    OSTime m_endTimestamp = 0;

    bool m_isPauseIGT = false;
    OSTime m_loadStartTimestamp = 0;
    OSTime m_totalLoadTime = 0;
    OSTime m_igtTimer = 0;
};

extern SpeedrunInfo m_speedrunInfo;

void resetForSpeedrunMode();

}  // namespace dusk
