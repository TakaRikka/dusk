#include "imgui.h"

#include "ImGuiMenuEnhancements.hpp"
#include "ImGuiConfig.hpp"
#include "dusk/settings.h"

namespace dusk {
    ImGuiMenuEnhancements::ImGuiMenuEnhancements() {}

    void ImGuiMenuEnhancements::draw() {
        if (ImGui::BeginMenu("Enhancements")) {
            if (ImGui::BeginMenu("Quality of Life")) {
                config::ImGuiCheckbox("Quick Transform (R+Y)", settings::game::enableQuickTransform);

                config::ImGuiCheckbox("Bigger Wallets", settings::game::biggerWallets);
                if (ImGui::IsItemHovered()) {
                    ImGui::SetTooltip("Wallet sizes are like in the HD version (500, 1000, 2000)");
                }

                config::ImGuiCheckbox("No Rupee Returns", settings::game::noReturnRupees);
                if (ImGui::IsItemHovered()) {
                    ImGui::SetTooltip("Always collect Rupees even if your Wallet is too full");
                }

                config::ImGuiCheckbox("Disable Rupee Cutscenes", settings::game::disableRupeeCutscenes);
                if (ImGui::IsItemHovered()) {
                    ImGui::SetTooltip("Rupees won't play cutscenes after you've collected them the first time");
                }

                config::ImGuiCheckbox("No Sword Recoil", settings::game::noSwordRecoil);
                if (ImGui::IsItemHovered()) {
                    ImGui::SetTooltip("Link won't recoil when his sword hits walls");
                }

                config::ImGuiCheckbox("Faster Climbing", settings::game::fastClimbing);
                if (ImGui::IsItemHovered()) {
                    ImGui::SetTooltip("Quicker climbing on ladders and vines like the HD version");
                }

                config::ImGuiCheckbox("Faster Tears of Light", settings::game::fastTears);
                if (ImGui::IsItemHovered()) {
                    ImGui::SetTooltip("Tears of Light dropped by Shadow Insects pop out faster like the HD version");
                }

                config::ImGuiCheckbox("Hide TV Settings Screen", settings::game::hideTvSettingsScreen);
                if (ImGui::IsItemHovered()) {
                    ImGui::SetTooltip("Hides the TV calibration screen shown when loading a save");
                }
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Preferences")) {
                config::ImGuiCheckbox("Mirror Mode", settings::game::enableMirrorMode);
                config::ImGuiCheckbox("Invert Camera X Axis", settings::game::invertCameraXAxis);

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Graphics")) {
                config::ImGuiCheckbox("Native Bloom", settings::game::enableBloom);
                config::ImGuiCheckbox("Water Projection Offset", settings::game::useWaterProjectionOffset);
                if (ImGui::IsItemHovered()) {
                    ImGui::SetTooltip("Adds GC-specific -0.01 transS offset\n"
                        "that causes ~6px ghost artifacts in water reflections");
                }

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Cheats")) {
                config::ImGuiCheckbox("Fast Iron Boots", settings::game::enableFastIronBoots);

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Difficulty")) {
                config::ImGuiSliderInt("Damage Multiplier", settings::game::damageMultiplier, 1, 8, "x%d");
                config::ImGuiCheckbox("Instant Death", settings::game::instantDeath);
                if (ImGui::IsItemHovered()) {
                    ImGui::SetTooltip("Any hit will instantly kill you");
                }

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Technical")) {
                config::ImGuiCheckbox("Restore Wii 1.0 Glitches", settings::game::restoreWiiGlitches);
                if (ImGui::IsItemHovered()) {
                    ImGui::SetTooltip("Restores patched glitches from Wii USA 1.0, the first released version");
                }

                ImGui::EndMenu();
            }

            ImGui::EndMenu();
        }
    }
}
