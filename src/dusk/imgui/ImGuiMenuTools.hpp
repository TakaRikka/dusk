#ifndef DUSK_IMGUI_MENUTOOLS_HPP
#define DUSK_IMGUI_MENUTOOLS_HPP

#include <aurora/aurora.h>
#include <string>

#include "imgui.h"

namespace dusk {
    class ImGuiMenuTools {
    public:
        ImGuiMenuTools();
        void draw();

		void ShowDebugOverlay();
		void ShowCameraOverlay();
		void ShowProcessManager();
		void ShowHeapOverlay();
		void ShowStubLog();
		void ShowMapLoader();

		bool isCollisionView() { return m_enableCollisionView; }

    private:
		bool m_showDebugOverlay = false;
		int m_debugOverlayCorner = 0; // top-left

		bool m_showCameraOverlay = false;
		int m_cameraOverlayCorner = 3;

		bool m_showProcessManagement = false;

		bool m_showHeapOverlay = false;

		bool m_showStubLog = false;

		bool m_showMapLoader = false;
		struct {
			int mapIdx = -1;
			int regionIdx = -1;
			int roomNoIdx = 0;
			int pointNoIdx = 0;
			int roomNo = -1;
			int pointNo = -1;
			int spawnId = 0;
			int layer = -1;
			bool showInternalNames = false;
		} m_mapLoaderInfo;

		bool m_isDevelopmentMode = false;

		bool m_enableCollisionView = false;
    };
}

#endif  // DUSK_IMGUI_MENUTOOLS_HPP
