#include "d/d_com_inf_game.h"

#include "imgui.h"
#include <imgui_internal.h>
#include "ImGuiConsole.hpp"

namespace dusk {
    void ImGuiConsole::ShowMapLoader() {
        if (!m_showMapLoader) {
            return;
        }

        ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize |
            ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
    
        ImGui::SetNextWindowBgAlpha(0.65f);
        ImGui::SetNextWindowSizeConstraints(ImVec2(300, 0), ImVec2(FLT_MAX, FLT_MAX));
    
        if (!ImGui::Begin("Map Loader", nullptr, windowFlags)) {
            ImGui::End();
            return;
        }

        ImGui::SeparatorText("Map Loader");
    
        ImGui::InputText("Stage", m_mapLoaderInfo.stageName, sizeof(m_mapLoaderInfo.stageName));
        ImGui::InputInt("Room", &m_mapLoaderInfo.roomNo);
        ImGui::InputInt("Spawn ID", &m_mapLoaderInfo.spawnId);
        ImGui::InputInt("Layer", &m_mapLoaderInfo.layer);
        
        if (ImGui::Button("Load Map")) {
            dComIfGp_setNextStage(m_mapLoaderInfo.stageName, m_mapLoaderInfo.spawnId, m_mapLoaderInfo.roomNo, m_mapLoaderInfo.layer);
        }
    
        ImGui::End();
    }
}  // namespace dusk
