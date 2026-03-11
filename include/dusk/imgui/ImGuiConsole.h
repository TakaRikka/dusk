#pragma once

#include <set>
#include <string_view>
#include <deque>

#include <aurora/aurora.h>

namespace dusk {
void ImGuiStringViewText(std::string_view text);
void ImGuiTextCenter(std::string_view text);

struct Toast {
    std::string message;
    float remain;
    float current = 0.f;
    Toast(std::string message, float duration) noexcept : message(std::move(message)), remain(duration) {}
};

class ImGuiConsole {
public:
    ImGuiConsole();
    void PreUpdate();
    void PostUpdate();
    void PostDraw();
    void Shutdown();

    void ControllerAdded(uint32_t idx);
    void ControllerRemoved(uint32_t idx);
    void ToggleVisible();

    std::optional<std::string> m_errorString;
    bool m_quitRequested = false;

private:
    bool m_showDemoWindow = false;
    bool m_showAboutWindow = false;
    bool m_showPreLaunchSettingsWindow = false;

    bool m_paused = false;
    bool m_stepFrame = false;
    bool m_isVisible = false;

    bool m_isInitialized = false;
    bool m_isLaunchInitialized = false;

    std::deque<Toast> m_toasts;
    std::string m_controllerName;
    u32 m_whichController = -1;

    bool m_controllerConfigVisible = false;
    ImGuiControllerConfig m_controllerConfig;

    void ShowAboutWindow(bool preLaunch);
    void ShowAppMainMenuBar(bool canInspect, bool preLaunch);
    void ShowMenuGame();

    void UpdateEntityEntries();
    void ShowDebugOverlay();

    void ShowToasts();
    void ShowInputViewer();
    void SetOverlayWindowLocation(int corner) const;
    void ShowPipelineProgress();
    void ShowPreLaunchSettingsWindow();
};

AuroraBackend backend_from_string(const std::string& str);
std::string_view backend_to_string(AuroraBackend backend);
std::string_view backend_name(AuroraBackend backend);
} // namespace metaforce
