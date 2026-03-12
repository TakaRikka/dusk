#include "d/d_com_inf_game.h"

#include "imgui.h"
#include <imgui_internal.h>
#include "ImGuiConsole.hpp"
#include "dusk/map_loader_definitions.h"

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

        ImGui::Checkbox("Show Internal Names", &m_mapLoaderInfo.showInternalNames);

        const char* previewRegion = "None";
        if (m_mapLoaderInfo.regionIdx != -1) {
            previewRegion = gameRegions[m_mapLoaderInfo.regionIdx].regionName;
        }

        if (ImGui::BeginCombo("Select Region", previewRegion)) {
            int idx = 0;
            for (const auto& region : gameRegions) {
                if (ImGui::Selectable(region.regionName)) {
                    m_mapLoaderInfo.regionIdx = idx;
                    m_mapLoaderInfo.mapIdx = -1;
                }
                idx++;
            }
            ImGui::EndCombo();
        }

        if (m_mapLoaderInfo.regionIdx != -1) {
            const auto& region = gameRegions[m_mapLoaderInfo.regionIdx];

            std::string previewMap = "None";
            if (m_mapLoaderInfo.mapIdx != -1) {
                const auto& map = region.maps[m_mapLoaderInfo.mapIdx];
                previewMap = m_mapLoaderInfo.showInternalNames ? fmt::format("{} ({})", map.mapName, map.mapFile) : map.mapName;
            }

            if (ImGui::BeginCombo("Select Map", previewMap.data())) {
                for (int i = 0; i < region.numMaps; ++i) {
                    const auto& map = region.maps[i];
                    std::string label = m_mapLoaderInfo.showInternalNames ? fmt::format("{} ({})", map.mapName, map.mapFile) : map.mapName;
                    if (ImGui::Selectable(label.data())) {
                        m_mapLoaderInfo.mapIdx = i;
                    }
                }
                ImGui::EndCombo();
            }
        }else {
            ImGui::Text("No region selected.");
        }

        if (m_mapLoaderInfo.regionIdx != -1 && m_mapLoaderInfo.mapIdx != -1) {
            const auto& region = gameRegions[m_mapLoaderInfo.regionIdx];
            const auto& map = region.maps[m_mapLoaderInfo.mapIdx];

            if (map.numRooms > 0) {
                ImGui::Text("Selected Room: %d", map.mapRooms[m_mapLoaderInfo.roomNoIdx]);
                ImGui::SameLine();
                if (ImGui::Button("-###RoomNoIdxDec")) {
                    m_mapLoaderInfo.roomNoIdx--;
                    if (m_mapLoaderInfo.roomNoIdx < 0) {
                        m_mapLoaderInfo.roomNoIdx = 0;
                    }
                }
                ImGui::SameLine();
                if (ImGui::Button("+###RoomNoIdxInc")) {
                    m_mapLoaderInfo.roomNoIdx++;
                    if (m_mapLoaderInfo.roomNoIdx >= map.numRooms) {
                        m_mapLoaderInfo.roomNoIdx = map.numRooms - 1;
                    }
                }
            }

            if (ImGui::Button("Warp")) {
                dComIfGp_setNextStage(map.mapFile, 0, map.mapRooms[m_mapLoaderInfo.roomNoIdx], -1);
            }
        }

    
        ImGui::End();
    }

    }  // namespace dusk
