#pragma once
#include <aurora/aurora.h>

namespace dusk {

struct SpeedrunInfo {
    void startRun() {
        m_isRunStarted = true;
        m_rtaStartTimestamp = OSGetNativeTime();
        m_igtStartTimestamp = OSGetTime();
    }

    void stopRun() {
        m_isRunStarted = false;
        m_rtaTimer = OSGetNativeTime() - m_rtaStartTimestamp;
        if (!m_isPauseIGT) {
            m_igtTimer = OSGetTime() - m_igtStartTimestamp - m_totalLoadTime;
        }
    }

    void reset() {
        m_isRunStarted = false;
        m_rtaStartTimestamp = 0;
        m_rtaTimer = 0;
        m_igtStartTimestamp = 0;
        m_isPauseIGT = false;
        m_loadStartTimestamp = 0;
        m_totalLoadTime = 0;
        m_igtTimer = 0;
    }

    bool m_isRunStarted = false;
    OSTime m_rtaStartTimestamp = 0;
    OSTime m_rtaTimer = 0;
    OSTime m_igtStartTimestamp = 0;

    bool m_isPauseIGT = false;
    OSTime m_loadStartTimestamp = 0;
    OSTime m_totalLoadTime = 0;
    OSTime m_igtTimer = 0;
};

extern SpeedrunInfo m_speedrunInfo;

void resetForSpeedrunMode();
void restoreFromSpeedrunMode();

}  // namespace dusk
