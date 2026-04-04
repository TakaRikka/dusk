#ifndef DUSK_IMGUI_MENUTOOLS_HPP
#define DUSK_IMGUI_MENUTOOLS_HPP

#include <aurora/aurora.h>
#include <string>

#include "imgui.h"
#include "ImGuiSaveEditor.hpp"

namespace dusk {
    class ImGuiMenuTools {
    public:
		struct CollisionViewSettings {
			bool m_enableTerrainView = false;
			bool m_enableWireframe = false;
			bool m_enableAtView = false;
			bool m_enableTgView = false;
			bool m_enableCoView = false;
			float m_terrainViewOpacity = 50.0f;
			float m_colliderViewOpacity = 50.0f;
			float m_drawRange = 100.0f;
		};

        ImGuiMenuTools();
        void draw();
        void afterDraw();

		void ShowDebugOverlay();
		void ShowCameraOverlay();
		void ShowProcessManager();
		void ShowHeapOverlay();
		void ShowStubLog();
		void ShowMapLoader();
        void ShowPlayerInfo();
        void ShowAudioDebug();

        CollisionViewSettings& getCollisionViewSettings() { return m_collisionViewSettings; }

    private:
		bool m_showDebugOverlay = false;
		int m_debugOverlayCorner = 2; // bottom-left

		bool m_showCameraOverlay = false;
		int m_cameraOverlayCorner = 3;

		bool m_showProcessManagement = false;

		bool m_showHeapOverlay = false;

		bool m_showStubLog = false;

		bool m_showMapLoader = false;

        bool m_showAudioDebug = false;
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
		bool m_showPlayerInfo = false;

		CollisionViewSettings m_collisionViewSettings;

		bool m_showSaveEditor = false;
        ImGuiSaveEditor m_saveEditor;
    };
}

#endif  // DUSK_IMGUI_MENUTOOLS_HPP
