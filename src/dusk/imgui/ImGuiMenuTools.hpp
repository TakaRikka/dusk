#ifndef DUSK_IMGUI_MENUTOOLS_HPP
#define DUSK_IMGUI_MENUTOOLS_HPP

#include <aurora/aurora.h>
#include <queue>
#include <string>

#include "imgui.h"
#include "ImGuiSaveEditor.hpp"
#include "ImGuiStateShare.hpp"

namespace dusk {
    class ImGuiMenuTools {
    public:
        ImGuiMenuTools();
        void draw();
        void afterDraw();

		void ShowDebugOverlay();
		void ShowCameraOverlay();
		void ShowProcessManager();
		void ShowHeapOverlay();
		void ShowStubLog();
        void ShowBloomWindow();
        void ShowPlayerInfo();
        void ShowAudioDebug();
        void ShowSaveEditor();
        void ShowStateShare();
        void ShowInputViewer();
        void ShowActorSpawner();

    private:
		bool m_showDebugOverlay = false;
		int m_debugOverlayCorner = 2; // bottom-left

		bool m_showCameraOverlay = false;
		int m_cameraOverlayCorner = 3;

		bool m_showProcessManagement = false;

		bool m_showHeapOverlay = false;

		bool m_showStubLog = false;

        bool m_showBloomWindow = false;

        bool m_showAudioDebug = false;

		bool m_showPlayerInfo = false;
		int m_playerInfoOverlayCorner = 1; // top-right

		bool m_showSaveEditor = false;
        ImGuiSaveEditor m_saveEditor;

        bool m_showStateShare = false;
        ImGuiStateShare m_stateShare;

        bool m_showActorSpawner = false;
        int m_inputOverlayCorner = 3;
        std::string m_controllerName;
    };
}

#endif  // DUSK_IMGUI_MENUTOOLS_HPP
