#ifndef DUSK_IMGUI_MENUGAME_HPP
#define DUSK_IMGUI_MENUGAME_HPP

#include <aurora/aurora.h>
#include <pad.h>
#include <string>

#include "imgui.h"

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

    class ImGuiMenuGame {
    public:
        ImGuiMenuGame();
        void draw();

        void windowInputViewer();
        void drawSpeedrunTimerOverlay();

        static void resetForSpeedrunMode();

    private:
        struct {
            int m_selectedPort = 0;
            bool m_isReading = false;
            PADButtonMapping* m_pendingButtonMapping = nullptr;
            PADAxisMapping* m_pendingAxisMapping = nullptr;
            int m_pendingPort = -1;
            bool m_isRumbling = false;
        } m_controllerConfig;

        bool m_showControllerConfig = false;

        bool m_showInputViewer = false;
        bool m_showInputViewerGyro = false;
        int m_inputOverlayCorner = 3;
        std::string m_controllerName;

        bool m_showTimerWindow = false;
    };
}

#endif  // DUSK_IMGUI_MENUGAME_HPP
