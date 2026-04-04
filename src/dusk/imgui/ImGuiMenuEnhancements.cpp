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

                ImGui::Checkbox("No Sword Recoil", &getSettings().game.noSwordRecoil);
                if (ImGui::IsItemHovered()) {
                    ImGui::SetTooltip("Link won't recoil when his sword hits walls");
                }

                ImGui::Checkbox("Faster Climbing", &getSettings().game.fastClimbing);
                if (ImGui::IsItemHovered()) {
                    ImGui::SetTooltip("Quicker climbing on ladders and vines like the HD version");
                }

                ImGui::Checkbox("No Climbing Miss Animation", &getSettings().game.noMissClimbing);
                if (ImGui::IsItemHovered()) {
                    ImGui::SetTooltip("Prevents Link from playing a struggle animation\n"
                                      "when using the Clawshot on vines at a weird angle");
                }

                ImGui::Checkbox("Faster Tears of Light", &getSettings().game.fastTears);
                if (ImGui::IsItemHovered()) {
                    ImGui::SetTooltip("Tears of Light dropped by Shadow Insects pop out faster like the HD version");
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

            if (ImGui::BeginMenu("Audio")) {
                ImGui::Checkbox("Non-Stop Midna's Lament", &getSettings().game.midnasLamentNonStop);
                if (ImGui::IsItemHovered()) {
                    ImGui::SetTooltip("Prevents enemy music while Midna's Lament is playing");
                }

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Cheats")) {
                ImGui::Checkbox("Fast Iron Boots", &getSettings().game.enableFastIronBoots);

                ImGui::Checkbox("Can Transform Anywhere",
                                &getSettings().game.canTransformAnywhere);
                if (ImGui::IsItemHovered()) {
                    ImGui::SetTooltip("Allows you to transform between forms even if NPCs are looking");
                }

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Difficulty")) {
                ImGui::SliderInt("Damage Multiplier", &getSettings().game.damageMultiplier, 1, 8, "x%d");
                ImGui::Checkbox("Instant Death", &getSettings().game.instantDeath);
                if (ImGui::IsItemHovered()) {
                    ImGui::SetTooltip("Any hit will instantly kill you");
                }

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
