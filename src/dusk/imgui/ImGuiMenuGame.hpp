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

        static void ToggleFullscreen();

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
    };
}

#endif  // DUSK_IMGUI_MENUGAME_HPP
