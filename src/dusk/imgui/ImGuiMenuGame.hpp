#ifndef DUSK_IMGUI_MENUGAME_HPP
#define DUSK_IMGUI_MENUGAME_HPP

#include <aurora/aurora.h>
#include <pad.h>
#include <string>

#include "imgui.h"

namespace dusk {
    class ImGuiMenuGame {
    public:
        ImGuiMenuGame();
        void draw();

        void windowInputViewer();
        void windowControllerConfig();
        void drawSpeedrunTimerOverlay();

        static void ToggleFullscreen();

        static void resetForSpeedrunMode();
        bool isRunStarted() const { return m_speedrunInfo.m_isRunStarted; }
        void startRun() {
            resetForSpeedrunMode();
            m_speedrunInfo.m_isRunStarted = true;
            m_speedrunInfo.m_startTimestamp = OSGetTime();
        }
        void stopRun() {
            m_speedrunInfo.m_isRunStarted = false;
            m_speedrunInfo.m_endTimestamp = OSGetTime() - m_speedrunInfo.m_startTimestamp;
        }
        void incTotalLoadTime(OSTime time) { m_speedrunInfo.m_totalLoadTime += time; }

    private:
        void drawAudioMenu();
        void drawInputMenu();
        void drawGraphicsMenu();
        void drawGameplayMenu();
        void drawCheatsMenu();
        void drawInterfaceMenu();

        struct {
            int m_selectedPort = 0;
            bool m_isReading = false;
            PADButtonMapping* m_pendingButtonMapping = nullptr;
            PADAxisMapping* m_pendingAxisMapping = nullptr;
            int m_pendingPort = -1;
        } m_controllerConfig;

        bool m_showControllerConfig = false;

        bool m_showInputViewer = false;
        bool m_showInputViewerGyro = false;
        int m_inputOverlayCorner = 3;
        std::string m_controllerName;

        struct {
            bool m_showTimerWindow = false;
            bool m_isRunStarted = false;
            OSTime m_startTimestamp = 0;
            OSTime m_endTimestamp = 0;
            OSTime m_totalLoadTime = 0;
        } m_speedrunInfo;
    };
}

#endif  // DUSK_IMGUI_MENUGAME_HPP
