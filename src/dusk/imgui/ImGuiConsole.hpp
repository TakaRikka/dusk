#ifndef DUSK_IMGUI_HPP
#define DUSK_IMGUI_HPP

#include <aurora/aurora.h>
#include <string>

#include "imgui.h"

namespace dusk {
	class ImGuiConsole {
	public:
		ImGuiConsole();

		void draw();

		void ShowDebugOverlay();
        void ShowCameraOverlay();
		void ShowProcessManager();
		void ShowHeapOverlay();
        void ShowInputViewer();
		void ShowStubLog();

		bool CheckMenuViewToggle(ImGuiKey key, bool& active);

	private:
		bool m_isHidden = false;

		bool m_showDebugOverlay = false;
		int m_debugOverlayCorner = 0; // top-left

		bool m_showCameraOverlay = false;
		int m_cameraOverlayCorner = 3;

		bool m_showProcessManagement = false;

		bool m_showHeapOverlay = false;

		bool m_showStubLog = false;

		bool m_showInputViewer = false;
		int m_inputOverlayCorner = 3;
		std::string m_controllerName;
	};

	extern ImGuiConsole g_imguiConsole;

	std::string_view backend_name(AuroraBackend backend);
	std::string BytesToString(size_t bytes);
	void SetOverlayWindowLocation(int corner);
	bool ShowCornerContextMenu(int& corner, int avoidCorner);
}

void DuskDebugPad();

#endif  // DUSK_IMGUI_HPP
