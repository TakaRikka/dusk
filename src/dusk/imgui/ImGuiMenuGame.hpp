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

    private:
        struct {
            float m_masterVolume = 1.0f;
            float m_mainMusicVolume = 1.0f;
            float m_subMusicVolume = 1.0f;
            float m_soundEffectsVolume = 1.0f;
            float m_fanfareVolume = 1.0f;
        } m_audioSettings;

        struct {
            int m_selectedPort = 0;
            bool m_isReading = false;
            PADButtonMapping* m_pendingButtonMapping = nullptr;
            PADAxisMapping* m_pendingAxisMapping = nullptr;
            int m_pendingPort = -1;
        } m_controllerConfig;

        bool m_fullscreen = false;

        bool m_showControllerConfig = false;

        bool m_showInputViewer = false;
        int m_inputOverlayCorner = 3;
        std::string m_controllerName;
    };
}

#endif  // DUSK_IMGUI_MENUGAME_HPP
