#include "fmt/format.h"
#include "imgui.h"
#include "aurora/gfx.h"

#include "ImGuiConsole.hpp"
#include "ImGuiMenuTools.hpp"

#include "m_Do/m_Do_main.h"
#include "d/d_com_inf_game.h"
#include "d/actor/d_a_alink.h"
#include "d/actor/d_a_horse.h"

namespace dusk {
    ImGuiMenuTools::ImGuiMenuTools() {}

    void ImGuiMenuTools::draw() {
        bool isToggleDevelopmentMode = false;

        if (ImGui::BeginMenu("Tools")) {
            if (ImGui::Checkbox("Development Mode", &m_isDevelopmentMode)) {
                isToggleDevelopmentMode = true;
            }

            ImGui::Separator();

            if (ImGui::BeginMenu("Collision View")) {
                ImGui::Checkbox("Enable Terrain view", &m_collisionViewSettings.m_enableTerrainView);
                ImGui::Checkbox("Enable wireframe view", &m_collisionViewSettings.m_enableWireframe);
                ImGui::SliderFloat("Opacity##terrain", &m_collisionViewSettings.m_terrainViewOpacity, 0.0f, 100.0f);
                ImGui::SliderFloat("Draw Range", &m_collisionViewSettings.m_drawRange, 0.0f, 1000.0f);
                ImGui::Separator();
                ImGui::Checkbox("Enable Attack Collider view", &m_collisionViewSettings.m_enableAtView);
                ImGui::Checkbox("Enable Target Collider view", &m_collisionViewSettings.m_enableTgView);
                ImGui::Checkbox("Enable Push Collider view", &m_collisionViewSettings.m_enableCoView);
                ImGui::SliderFloat("Opacity##colliders", &m_collisionViewSettings.m_colliderViewOpacity, 0.0f, 100.0f);
                ImGui::EndMenu();
            }

            ImGui::MenuItem("Process Management", "F2", &m_showProcessManagement);
            ImGui::MenuItem("Debug Overlay", "F3", &m_showDebugOverlay);
            ImGui::MenuItem("Heap Viewer", "F4", &m_showHeapOverlay);
            ImGui::MenuItem("Stub Log", "F5", &m_showStubLog);
            ImGui::MenuItem("Debug Camera", "F6", &m_showCameraOverlay);
            ImGui::MenuItem("Map Loader", nullptr, &m_showMapLoader);
            ImGui::MenuItem("Player Info", nullptr, &m_showPlayerInfo);
            ImGui::MenuItem("Save Editor", nullptr, &m_showSaveEditor);
            ImGui::MenuItem("Audio Debug", "F7", &m_showAudioDebug);
            ImGui::MenuItem("OSReport Force", nullptr, &OSReportReallyForceEnable);
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
        ShowPlayerInfo();
        ShowAudioDebug();

        if (m_showSaveEditor) {
            m_saveEditor.draw(m_showSaveEditor);
        }

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
            ImGuiStringViewText(fmt::format(FMT_STRING("Tex upload size:   {}\n"),
                BytesToString(stats->lastTextureUploadSize)));
            ImGuiStringViewText(fmt::format(
                FMT_STRING("Total:             {}\n"),
                BytesToString(stats->lastVertSize + stats->lastUniformSize +
                    stats->lastIndexSize + stats->lastStorageSize +
                    stats->lastTextureUploadSize)));
        }
        ImGui::End();
    }

    void ImGuiMenuTools::ShowPlayerInfo() {
        if (!m_showPlayerInfo) {
            return;
        }

        ImGuiIO& io = ImGui::GetIO();
        ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoResize;

        ImGui::SetNextWindowBgAlpha(0.65f);
        ImGui::SetNextWindowSizeConstraints(ImVec2(300, 200), ImVec2(300, 200));

        if (ImGui::Begin("Player Info", &m_showPlayerInfo, windowFlags)) {
            daAlink_c* player = (daAlink_c*)dComIfGp_getPlayer(0);
            daHorse_c* horse = dComIfGp_getHorseActor();

            ImGui::Text("Link");
            ImGuiStringViewText(
                player != nullptr
                ? fmt::format("Position: {: .2f}, {: .2f}, {: .2f}\n", player->current.pos.x, player->current.pos.y, player->current.pos.z)
                : "Position: ?, ?, ?\n"
            );

            ImGuiStringViewText(
                player != nullptr
                ? fmt::format("Angle: {0}\n", player->shape_angle.y)
                : "Angle: ?\n"
            );

            ImGuiStringViewText(
                player != nullptr
                ? fmt::format("Speed: {0}\n", player->speedF)
                : "Speed: ?\n"
            );

            ImGui::Separator();
            ImGui::Text("Epona");
            ImGuiStringViewText(
                horse != nullptr
                ? fmt::format("Position: {: .2f}, {: .2f}, {: .2f}\n", horse->current.pos.x, horse->current.pos.y, horse->current.pos.z)
                : "Position: ?, ?, ?\n"
            );

            ImGuiStringViewText(
                horse != nullptr
                ? fmt::format("Angle: {0}\n", horse->shape_angle.y)
                : "Angle: ?\n"
            );

            ImGuiStringViewText(
                horse != nullptr
                ? fmt::format("Speed: {0}\n", horse->speedF)
                : "Speed: ?\n"
            );
        }

        ImGui::End();
    }
}
