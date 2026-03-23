#ifndef DUSK_IMGUI_HPP
#define DUSK_IMGUI_HPP

#include <aurora/aurora.h>
#include <string>

#include "imgui.h"
#include "ImGuiMenuGame.hpp"
#include "ImGuiMenuTools.hpp"

namespace dusk {
	class ImGuiConsole {
	public:
		ImGuiConsole();
		void draw();

        bool isBloomEnabled() { return m_menuGame.isBloomEnabled(); }
		ImGuiMenuTools::CollisionViewSettings& getCollisionViewSettings() { return m_menuTools.getCollisionViewSettings(); }

		static bool CheckMenuViewToggle(ImGuiKey key, bool& active);

	private:
		bool m_isHidden = false;

		ImGuiMenuGame m_menuGame;
        ImGuiMenuTools m_menuTools;
	};

	extern ImGuiConsole g_imguiConsole;

	std::string_view backend_name(AuroraBackend backend);
	std::string BytesToString(size_t bytes);
	void SetOverlayWindowLocation(int corner);
	bool ShowCornerContextMenu(int& corner, int avoidCorner);
	void ImGuiStringViewText(std::string_view text);
	void ImGuiBeginGroupPanel(const char* name, const ImVec2& size);
	void ImGuiEndGroupPanel();
}

void DuskDebugPad();

#endif  // DUSK_IMGUI_HPP
