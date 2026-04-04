#include "fmt/format.h"
#include "imgui.h"
#include "aurora/gfx.h"

#include "ImGuiConsole.hpp"
#include "ImGuiMenuEnhancements.hpp"
#include "ImGuiConfig.hpp"
#include "dusk/settings.hpp"
#include <imgui_internal.h>

namespace dusk {
    ImGuiMenuEnhancements::ImGuiMenuEnhancements() {}

    void ImGuiMenuEnhancements::draw() {
        if (ImGui::BeginMenu("Enhancements")) {
            if (ImGui::BeginMenu("Quality of Life")) {
                config::ImguiCheckbox("Fast Iron Boots", settings::enhancements::FastIronBoots);
                config::ImguiCheckbox("Invert Camera X Axis", settings::enhancements::InvertCameraXAxis);
                config::ImguiCheckbox("Quick Transform (R+Y)", settings::enhancements::QuickTransform);
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Graphics")) {
                config::ImguiCheckbox("Native Bloom", settings::enhancements::EnableBloom);
                config::ImguiCheckbox("Water Projection Offset", settings::enhancements::UseWaterProjectionOffset);
                if (ImGui::IsItemHovered()) {
                    ImGui::SetTooltip("Adds GC-specific -0.01 transS offset\n"
                        "that causes ~6px ghost artifacts in water reflections");
                }
                config::ImguiCheckbox("Mirror Mode", settings::enhancements::MirrorMode);

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Restorations")) {
                config::ImguiCheckbox("Restore Wii 1.0 Glitches", settings::enhancements::RestoreWiiGlitches);
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
