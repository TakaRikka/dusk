#include "fmt/format.h"
#include "imgui.h"
#include "aurora/gfx.h"

#include "ImGuiConsole.hpp"
#include "ImGuiMenuEnhancements.hpp"
#include <imgui_internal.h>

namespace dusk {
    EnhancementsSettings ImGuiMenuEnhancements::m_enhancements = {
        .fastIronBoots = false,
        .invertCameraXAxis = false,
        .restoreWiiGlitches = false,
        .enableBloom = true,
        .useWaterProjectionOffset = false,
    };

    ImGuiMenuEnhancements::ImGuiMenuEnhancements() {}

    void ImGuiMenuEnhancements::draw() {
        if (ImGui::BeginMenu("Enhancements")) {
            if (ImGui::BeginMenu("Quality of Life")) {
                ImGui::Checkbox("Fast Iron Boots", &m_enhancements.fastIronBoots);
                ImGui::Checkbox("Invert Camera X Axis", &m_enhancements.invertCameraXAxis);
                ImGui::Checkbox("Quick Transform (R+Y)", &m_enhancements.quickTransform);
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Graphics")) {
                ImGui::Checkbox("Native Bloom", &m_enhancements.enableBloom);
                ImGui::Checkbox("Water Projection Offset", &m_enhancements.useWaterProjectionOffset);
                if (ImGui::IsItemHovered()) {
                    ImGui::SetTooltip("Adds GC-specific -0.01 transS offset\n"
                        "that causes ~6px ghost artifacts in water reflections");
                }

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Restorations")) {
                ImGui::Checkbox("Restore Wii 1.0 Glitches", &m_enhancements.restoreWiiGlitches);
                if (ImGui::IsItemHovered()) {
                    ImGui::SetTooltip("Restores patched glitches from Wii USA 1.0, the first released version");
                }

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Cheats")) {
                ImGui::EndMenu();
            }

            ImGui::EndMenu();
        }
    }
}
