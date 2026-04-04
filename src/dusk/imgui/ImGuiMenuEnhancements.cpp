#include "imgui.h"

#include "ImGuiMenuEnhancements.hpp"

#include "dusk/settings.h"

namespace dusk {
    ImGuiMenuEnhancements::ImGuiMenuEnhancements() {}

    void ImGuiMenuEnhancements::draw() {
        if (ImGui::BeginMenu("Enhancements")) {
            if (ImGui::BeginMenu("Quality of Life")) {
                ImGui::Checkbox("Quick Transform (R+Y)", &getSettings().game.enableQuickTransform);

                ImGui::Checkbox("Bigger Wallets", &getSettings().game.biggerWallets);
                if (ImGui::IsItemHovered()) {
                    ImGui::SetTooltip("Wallet sizes are like in the HD version (500, 1000, 2000)");
                }

                ImGui::Checkbox("No Rupee Returns", &getSettings().game.noReturnRupees);
                if (ImGui::IsItemHovered()) {
                    ImGui::SetTooltip("Always collect Rupees even if your Wallet is too full");
                }

                ImGui::Checkbox("Disable Rupee Cutscenes", &getSettings().game.disableRupeeCutscenes);
                if (ImGui::IsItemHovered()) {
                    ImGui::SetTooltip("Rupees won't play cutscenes after you've collected them the first time");
                }

                ImGui::Checkbox("Hide TV Settings Screen", &getSettings().game.hideTvSettingsScreen);
                if (ImGui::IsItemHovered()) {
                    ImGui::SetTooltip("Hides the TV calibration screen shown when loading a save");
                }
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
