#include "d/d_com_inf_game.h"

#include "imgui.h"
#include <imgui_internal.h>
#include "ImGuiConsole.hpp"
#include "ImGuiMenuTools.hpp"
#include "dusk/map_loader_definitions.h"

namespace dusk {
    void ImGuiMenuTools::ShowMapLoader() {
        if (!m_showMapLoader) {
            return;
        }

        ImGuiWindowFlags windowFlags = ImGuiWindowFlags_AlwaysAutoResize |
            ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
    
        ImGui::SetNextWindowBgAlpha(0.65f);
        ImGui::SetNextWindowSizeConstraints(ImVec2(300, 0), ImVec2(FLT_MAX, FLT_MAX));
    
        if (!ImGui::Begin("Map Loader", &m_showMapLoader, windowFlags)) {
            ImGui::End();
            return;
        }

        ImGui::Checkbox("Show Internal Names", &m_mapLoaderInfo.showInternalNames);

        const char* previewRegion = "None";
        if (m_mapLoaderInfo.regionIdx != -1) {
            previewRegion = gameRegions[m_mapLoaderInfo.regionIdx].regionName;
        }

        if (ImGui::BeginCombo("Select Region", previewRegion)) {
            int idx = 0;
            for (const auto& region : gameRegions) {
                if (ImGui::Selectable(region.regionName)) {
                    if (m_mapLoaderInfo.regionIdx != idx) {
                        m_mapLoaderInfo.mapIdx = 0;
                        m_mapLoaderInfo.roomNoIdx = 0;
                        m_mapLoaderInfo.pointNoIdx = 0;
                    }
                    m_mapLoaderInfo.regionIdx = idx;
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
                int prevMapIdx = m_mapLoaderInfo.mapIdx;
                for (int i = 0; i < region.numMaps; ++i) {
                    const auto& map = region.maps[i];
                    std::string label = m_mapLoaderInfo.showInternalNames ? fmt::format("{} ({})", map.mapName, map.mapFile) : map.mapName;
                    if (ImGui::Selectable(label.data())) {
                        m_mapLoaderInfo.mapIdx = i;
                    }
                }
                ImGui::EndCombo();
                if (m_mapLoaderInfo.mapIdx != prevMapIdx) {
                    m_mapLoaderInfo.roomNoIdx = 0;
                    m_mapLoaderInfo.pointNoIdx = 0;
                }
            }
        } else {
            ImGui::Text("No region selected.");
        }

        if (m_mapLoaderInfo.regionIdx != -1 && m_mapLoaderInfo.mapIdx != -1) {
            const auto& region = gameRegions[m_mapLoaderInfo.regionIdx];
            const auto& map = region.maps[m_mapLoaderInfo.mapIdx];
            const auto& room = map.mapRooms[m_mapLoaderInfo.roomNoIdx];

            if (map.numRooms > 1) {
                ImGui::Text("Selected Room:   %2d", room.roomNo);
                ImGui::SameLine();
                if (ImGui::Button("-###RoomNoIdxDec")) {
                    m_mapLoaderInfo.roomNoIdx--;
                    if (m_mapLoaderInfo.roomNoIdx < 0) {
                        m_mapLoaderInfo.roomNoIdx = map.numRooms - 1;
                    }
                    m_mapLoaderInfo.pointNoIdx = 0;
                }
                ImGui::SameLine();
                if (ImGui::Button("+###RoomNoIdxInc")) {
                    m_mapLoaderInfo.roomNoIdx++;
                    if (m_mapLoaderInfo.roomNoIdx >= map.numRooms) {
                        m_mapLoaderInfo.roomNoIdx = 0;
                    }
                    m_mapLoaderInfo.pointNoIdx = 0;
                }
            }

            constexpr int MAX_LAYER = 14;

            ImGui::Text("Selected Layer: %3d", m_mapLoaderInfo.layer);
            ImGui::SameLine();
            if (ImGui::Button("-###layerDec")) {
                m_mapLoaderInfo.layer--;
                if (m_mapLoaderInfo.layer < -1) {
                    m_mapLoaderInfo.layer = MAX_LAYER;
                }
            }
            ImGui::SameLine();
            if (ImGui::Button("+###layerInc")) {
                m_mapLoaderInfo.layer++;
                if (m_mapLoaderInfo.layer > MAX_LAYER) {
                    m_mapLoaderInfo.layer = -1;
                }
            }

            if (room.numPoints > 1) {
                ImGui::Text("Selected Point: %3d", room.roomPoints[m_mapLoaderInfo.pointNoIdx]);
                ImGui::SameLine();
                if (ImGui::Button("-###PointNoIdxDec")) {
                    m_mapLoaderInfo.pointNoIdx--;
                    if (m_mapLoaderInfo.pointNoIdx < 0) {
                        m_mapLoaderInfo.pointNoIdx = room.numPoints - 1;
                    }
                }
                ImGui::SameLine();
                if (ImGui::Button("+###PointNoIdxInc")) {
                    m_mapLoaderInfo.pointNoIdx++;
                    if (m_mapLoaderInfo.pointNoIdx >= room.numPoints) {
                        m_mapLoaderInfo.pointNoIdx = 0;
                    }
                }
            }

            if (ImGui::Button("Warp")) {
                dComIfGp_setNextStage(map.mapFile, room.roomPoints[m_mapLoaderInfo.pointNoIdx], room.roomNo, m_mapLoaderInfo.layer);
            }
        }

    
        ImGui::End();
    }

    }  // namespace dusk
