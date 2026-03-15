#include "fmt/format.h"
#include "imgui.h"
#include "aurora/gfx.h"

#include "ImGuiConsole.hpp"
#include "ImGuiMenuTools.hpp"

#include "m_Do/m_Do_main.h"

namespace dusk {
    ImGuiMenuTools::ImGuiMenuTools() {}

    void ImGuiMenuTools::draw() {
        bool isToggleDevelopmentMode = false;

        if (ImGui::BeginMenu("Tools")) {
            if (ImGui::Checkbox("Development Mode", &m_isDevelopmentMode)) {
                isToggleDevelopmentMode = true;
            }
            ImGui::Separator();

            ImGui::MenuItem("Process Management", "F2", &m_showProcessManagement);
            ImGui::MenuItem("Debug Overlay", "F3", &m_showDebugOverlay);
            ImGui::MenuItem("Heap Viewer", "F4", &m_showHeapOverlay);
            ImGui::MenuItem("Stub Log", "F5", &m_showStubLog);
            ImGui::MenuItem("Debug Camera", "F6", &m_showCameraOverlay);
            ImGui::MenuItem("Map Loader", nullptr, &m_showMapLoader);
            ImGui::EndMenu();
        }

        if (isToggleDevelopmentMode) {
            mDoMain::developmentMode = m_isDevelopmentMode ? 1 : -1;
        }

        ShowDebugOverlay();
        ShowCameraOverlay();
        ShowProcessManager();
        ShowHeapOverlay();
        ShowStubLog();
        ShowMapLoader();

        DuskDebugPad(); // temporary, remove later
    }

    void ImGuiMenuTools::ShowDebugOverlay() {
        if (!ImGuiConsole::CheckMenuViewToggle(ImGuiKey_F3, m_showDebugOverlay)) {
            return;
        }

        ImGuiIO& io = ImGui::GetIO();
        ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDecoration |
            ImGuiWindowFlags_AlwaysAutoResize |
            ImGuiWindowFlags_NoFocusOnAppearing |
            ImGuiWindowFlags_NoNav;
        if (m_debugOverlayCorner != -1) {
            SetOverlayWindowLocation(m_debugOverlayCorner);
            windowFlags |= ImGuiWindowFlags_NoMove;
        }

        ImGui::SetNextWindowBgAlpha(0.65f);
        if (ImGui::Begin("Debug Overlay", nullptr, windowFlags)) {
            bool hasPrevious = false;
            if (hasPrevious) {
                ImGui::Separator();
            }
            hasPrevious = true;

            ImGuiStringViewText(fmt::format(FMT_STRING("FPS: {:.2f}\n"), io.Framerate));

            if (hasPrevious) {
                ImGui::Separator();
            }
            hasPrevious = true;

            ImGuiStringViewText(fmt::format(FMT_STRING("Backend: {}\n"), backend_name(aurora_get_backend())));

            if (hasPrevious) {
                ImGui::Separator();
            }
            hasPrevious = true;

            AuroraStats const* stats = aurora_get_stats();

            ImGuiStringViewText(
                fmt::format(FMT_STRING("Queued pipelines:  {}\n"), stats->queuedPipelines));
            ImGuiStringViewText(
                fmt::format(FMT_STRING("Done pipelines:    {}\n"), stats->createdPipelines));
            ImGuiStringViewText(
                fmt::format(FMT_STRING("Draw call count:   {}\n"), stats->drawCallCount));
            ImGuiStringViewText(fmt::format(FMT_STRING("Merged draw calls: {}\n"),
                stats->mergedDrawCallCount));
            ImGuiStringViewText(fmt::format(FMT_STRING("Vertex size:       {}\n"),
                BytesToString(stats->lastVertSize)));
            ImGuiStringViewText(fmt::format(FMT_STRING("Uniform size:      {}\n"),
                BytesToString(stats->lastUniformSize)));
            ImGuiStringViewText(fmt::format(FMT_STRING("Index size:        {}\n"),
                BytesToString(stats->lastIndexSize)));
            ImGuiStringViewText(fmt::format(FMT_STRING("Storage size:      {}\n"),
                BytesToString(stats->lastStorageSize)));
            ImGuiStringViewText(fmt::format(
                FMT_STRING("Total:             {}\n"),
                BytesToString(stats->lastVertSize + stats->lastUniformSize +
                    stats->lastIndexSize + stats->lastStorageSize)));
        }
        ImGui::End();
    }
}