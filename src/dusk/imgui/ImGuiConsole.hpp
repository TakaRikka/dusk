#ifndef DUSK_IMGUI_HPP
#define DUSK_IMGUI_HPP

#include <aurora/aurora.h>
#include <deque>
#include <string>
#include <string_view>

#include "ImGuiFirstRunPreset.hpp"
#include "ImGuiMenuEnhancements.hpp"
#include "ImGuiMenuGame.hpp"
#include "ImGuiMenuTools.hpp"
#include "ImGuiPreLaunchWindow.hpp"
#include "imgui.h"

namespace dusk {
class ImGuiConsole {
public:
    ImGuiConsole();
    void UpdateSettings();
    void PreDraw();
    void PostDraw();

    void ShowToast(std::string, float);

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

    ImGuiFirstRunPreset m_firstRunPreset;
    ImGuiMenuGame m_menuGame;
    ImGuiMenuEnhancements m_menuEnhancements;
    ImGuiPreLaunchWindow m_preLaunchWindow;

    // Keep always last
    ImGuiMenuTools m_menuTools;

    void ShowToasts();
    void ShowPipelineProgress();
};

extern ImGuiConsole g_imguiConsole;

std::string_view backend_name(AuroraBackend backend);
std::string_view backend_id(AuroraBackend backend);
bool try_parse_backend(std::string_view backend, AuroraBackend& outBackend);
std::string BytesToString(size_t bytes);
void SetOverlayWindowLocation(int corner);
bool ShowCornerContextMenu(int& corner, int avoidCorner);
void ImGuiStringViewText(std::string_view text);
void ImGuiBeginGroupPanel(const char* name, const ImVec2& size);
void ImGuiEndGroupPanel();
void ImGuiTextCenter(std::string_view text);
bool ImGuiButtonCenter(std::string_view text);
float ImGuiScale();
}  // namespace dusk

void DuskDebugPad();

#endif  // DUSK_IMGUI_HPP
