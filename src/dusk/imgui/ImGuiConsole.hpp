#ifndef DUSK_IMGUI_HPP
#define DUSK_IMGUI_HPP

#include <deque>
#include <string>
#include <string_view>

#include <aurora/aurora.h>
#include <SDL3/SDL_touch.h>

#include "ImGuiFirstRunPreset.hpp"
#include "ImGuiMenuGame.hpp"
#include "ImGuiMenuTools.hpp"
#include "ImGuiPreLaunchWindow.hpp"
#include "imgui.h"

union SDL_Event;
struct ImGuiWindow;

namespace dusk {
class ImGuiConsole {
public:
    ImGuiConsole();
    void HandleSDLEvent(const SDL_Event& event);
    void UpdateSettings();
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

    float mouseHideTimer = 0.0f;

    bool m_isHidden = true;
    bool m_isLaunchInitialized = false;
    bool m_touchTapActive = false;
    bool m_touchTapMoved = false;
    SDL_FingerID m_touchTapFingerId = 0;
    ImVec2 m_touchTapStartPos = {};
    ImGuiWindow* m_dragScrollWindow = nullptr;
    ImVec2 m_dragScrollLastMousePos = {};
    std::deque<Toast> m_toasts;

    ImGuiFirstRunPreset m_firstRunPreset;
    ImGuiMenuGame m_menuGame;
    ImGuiPreLaunchWindow m_preLaunchWindow;

    // Keep always last
    ImGuiMenuTools m_menuTools;

    void ShowToasts();
    void ShowPipelineProgress();
    void UpdateDragScroll();
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
