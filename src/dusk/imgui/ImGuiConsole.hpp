#ifndef DUSK_IMGUI_HPP
#define DUSK_IMGUI_HPP

#include <aurora/aurora.h>
#include <deque>
#include <string>

#include "ImGuiMenuEnhancements.hpp"
#include "ImGuiMenuGame.hpp"
#include "ImGuiMenuTools.hpp"
#include "imgui.h"

namespace dusk {
class ImGuiConsole {
public:
    ImGuiConsole();
    void PreDraw();
    void PostDraw();

	static bool CheckMenuViewToggle(ImGuiKey key, bool& active);

private:
    struct Toast {
        std::string message;
        float remain;
        float current = 0.f;
        Toast(std::string message, float duration) noexcept : message(std::move(message)),
                                                              remain(duration) {}
    };

    bool m_isHidden = true;
    bool m_isLaunchInitialized = false;
    std::deque<Toast> m_toasts;
    ImGuiMenuGame m_menuGame;
    ImGuiMenuTools m_menuTools;
    ImGuiMenuEnhancements m_menuEnhancements;

    void ShowToasts();
    void ShowPipelineProgress();
};

extern ImGuiConsole g_imguiConsole;

std::string_view backend_name(AuroraBackend backend);
std::string BytesToString(size_t bytes);
void SetOverlayWindowLocation(int corner);
bool ShowCornerContextMenu(int& corner, int avoidCorner);
void ImGuiStringViewText(std::string_view text);
void ImGuiBeginGroupPanel(const char* name, const ImVec2& size);
void ImGuiEndGroupPanel();
float ImGuiScale();
}  // namespace dusk

void DuskDebugPad();

#endif  // DUSK_IMGUI_HPP
