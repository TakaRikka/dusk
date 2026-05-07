#ifndef DUSK_IMGUI_HPP
#define DUSK_IMGUI_HPP

#include <deque>
#include <filesystem>
#include <string>
#include <string_view>

#include <SDL3/SDL_misc.h>
#include <aurora/aurora.h>

#include "ImGuiMenuGame.hpp"
#include "ImGuiMenuTools.hpp"
#include "dusk/main.h"
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
    void AddToast(std::string_view message, float duration = 3.f);

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
    ImGuiWindow* m_dragScrollWindow = nullptr;
    ImVec2 m_dragScrollLastMousePos = {};
    std::deque<Toast> m_toasts;

    ImGuiMenuGame m_menuGame;

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
void DuskToast(std::string_view message, float duration = 3.f);
void ImGuiBeginGroupPanel(const char* name, const ImVec2& size);
void ImGuiEndGroupPanel();
void ImGuiTextCenter(std::string_view text);
bool ImGuiButtonCenter(std::string_view text);
float ImGuiScale();
}  // namespace dusk

#if defined(_WIN32) ||                                                                             \
    (defined(__APPLE__) && !TARGET_OS_IOS && !TARGET_OS_TV && !TARGET_OS_MACCATALYST) ||           \
    (defined(__linux__) && !defined(__ANDROID__))
#define DUSK_CAN_OPEN_DATA_FOLDER 1

namespace fs = std::filesystem;

static void OpenDataFolder() {
    const std::string path = fs::absolute(dusk::ConfigPath).generic_string();
#if defined(_WIN32)
    const std::string url = std::string("file:///") + path;
#else
    const std::string url = std::string("file://") + path;
#endif
    (void)SDL_OpenURL(url.c_str());
}
#else
#define DUSK_CAN_OPEN_DATA_FOLDER 0
#endif

#endif  // DUSK_IMGUI_HPP
