#ifndef DUSK_IMGUI_MENUGAME_HPP
#define DUSK_IMGUI_MENUGAME_HPP

#include <aurora/aurora.h>
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

        bool m_showControllerConfig = false;

        bool m_showInputViewer = false;
        int m_inputOverlayCorner = 3;
        std::string m_controllerName;
    };
}

#endif  // DUSK_IMGUI_MENUGAME_HPP
