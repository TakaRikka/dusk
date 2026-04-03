#include "imgui.h"

#include "ImGuiMenuEnhancements.hpp"

#include "dusk/settings.h"

namespace dusk {
    ImGuiMenuEnhancements::ImGuiMenuEnhancements() {}

    void ImGuiMenuEnhancements::draw() {
        if (ImGui::BeginMenu("Enhancements")) {
            if (ImGui::BeginMenu("Quality of Life")) {
                ImGui::Checkbox("Quick Transform (R+Y)", &getSettings().game.enableQuickTransform);
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Preferences")) {
                ImGui::Checkbox("Mirror Mode", &getSettings().game.enableMirrorMode);
                ImGui::Checkbox("Invert Camera X Axis", &getSettings().game.invertCameraXAxis);

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Graphics")) {
                ImGui::Checkbox("Native Bloom", &getSettings().game.enableBloom);
                ImGui::Checkbox("Water Projection Offset", &getSettings().game.useWaterProjectionOffset);
                if (ImGui::IsItemHovered()) {
                    ImGui::SetTooltip("Adds GC-specific -0.01 transS offset\n"
                        "that causes ~6px ghost artifacts in water reflections");
                }

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Cheats")) {
                ImGui::Checkbox("Fast Iron Boots", &getSettings().game.enableFastIronBoots);

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Technical")) {
                ImGui::Checkbox("Restore Wii 1.0 Glitches", &getSettings().game.restoreWiiGlitches);
                if (ImGui::IsItemHovered()) {
                    ImGui::SetTooltip("Restores patched glitches from Wii USA 1.0, the first released version");
                }

                ImGui::EndMenu();
            }

            ImGui::EndMenu();
        }
    }
}
