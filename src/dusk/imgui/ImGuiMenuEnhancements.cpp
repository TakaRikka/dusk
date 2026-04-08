#include "imgui.h"

#include "ImGuiMenuEnhancements.hpp"
#include "ImGuiConfig.hpp"
#include "dusk/settings.h"

namespace dusk {
    ImGuiMenuEnhancements::ImGuiMenuEnhancements() {}

    void ImGuiMenuEnhancements::draw() {
        if (ImGui::BeginMenu("Enhancements")) {
            if (ImGui::BeginMenu("Quality of Life")) {
                config::ImGuiCheckbox("Quick Transform (R+Y)", getSettings().game.enableQuickTransform);

                config::ImGuiCheckbox("Bigger Wallets", getSettings().game.biggerWallets);
                if (ImGui::IsItemHovered()) {
                    ImGui::SetTooltip("Wallet sizes are like in the HD version (500, 1000, 2000)");
                }

                config::ImGuiCheckbox("No Rupee Returns", getSettings().game.noReturnRupees);
                if (ImGui::IsItemHovered()) {
                    ImGui::SetTooltip("Always collect Rupees even if your Wallet is too full");
                }

                config::ImGuiCheckbox("Disable Rupee Cutscenes", getSettings().game.disableRupeeCutscenes);
                if (ImGui::IsItemHovered()) {
                    ImGui::SetTooltip("Rupees won't play cutscenes after you've collected them the first time");
                }

                config::ImGuiCheckbox("No Sword Recoil", getSettings().game.noSwordRecoil);
                if (ImGui::IsItemHovered()) {
                    ImGui::SetTooltip("Link won't recoil when his sword hits walls");
                }

                config::ImGuiCheckbox("Faster Climbing", getSettings().game.fastClimbing);
                if (ImGui::IsItemHovered()) {
                    ImGui::SetTooltip("Quicker climbing on ladders and vines like the HD version");
                }

                config::ImGuiCheckbox("No Climbing Miss Animation", getSettings().game.noMissClimbing);
                if (ImGui::IsItemHovered()) {
                    ImGui::SetTooltip("Prevents Link from playing a struggle animation\n"
                                      "when using the Clawshot on vines at a weird angle");
                }

                config::ImGuiCheckbox("Faster Tears of Light", getSettings().game.fastTears);
                if (ImGui::IsItemHovered()) {
                    ImGui::SetTooltip("Tears of Light dropped by Shadow Insects pop out faster like the HD version");
                }

                config::ImGuiCheckbox("Hide TV Settings Screen", getSettings().game.hideTvSettingsScreen);
                if (ImGui::IsItemHovered()) {
                    ImGui::SetTooltip("Hides the TV calibration screen shown when loading a save");
                }
                
                config::ImGuiCheckbox("Instant Saves", getSettings().game.instantSaves);
                if (ImGui::IsItemHovered()) {
                    ImGui::SetTooltip("Skip the delay when writing to the Memory Card");
                }

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Preferences")) {
                config::ImGuiCheckbox("Mirror Mode", getSettings().game.enableMirrorMode);
                if (ImGui::IsItemHovered()) {
                    ImGui::SetTooltip("Mirrors the world, matching the Wii version of the game");
                }

                config::ImGuiCheckbox("Invert Camera X Axis", getSettings().game.invertCameraXAxis);

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Graphics")) {
                config::ImGuiCheckbox("Native Bloom", getSettings().game.enableBloom);

                config::ImGuiCheckbox("Water Projection Offset", getSettings().game.useWaterProjectionOffset);
                if (ImGui::IsItemHovered()) {
                    ImGui::SetTooltip("Adds GC-specific -0.01 transS offset\n"
                                      "that causes ~6px ghost artifacts in water reflections");
                }

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Audio")) {
                config::ImGuiCheckbox("No Low HP Sound", getSettings().game.noLowHpSound);
                if (ImGui::IsItemHovered()) {
                    ImGui::SetTooltip("Disable the beeping sound when having low health");
                }

                config::ImGuiCheckbox("Non-Stop Midna's Lament", getSettings().game.midnasLamentNonStop);
                if (ImGui::IsItemHovered()) {
                    ImGui::SetTooltip("Prevents enemy music while Midna's Lament is playing");
                }

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Cheats")) {
                config::ImGuiCheckbox("Fast Iron Boots", getSettings().game.enableFastIronBoots);

                config::ImGuiCheckbox("Can Transform Anywhere", getSettings().game.canTransformAnywhere);
                if (ImGui::IsItemHovered()) {
                    ImGui::SetTooltip("Allows you to transform even if NPCs are looking");
                }

                config::ImGuiCheckbox("Fast Spinner", getSettings().game.fastSpinner);
                if (ImGui::IsItemHovered()) {
                    ImGui::SetTooltip("Speeds up Spinner movement when holding R.");
                }

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Difficulty")) {
                config::ImGuiSliderInt("Damage Multiplier", getSettings().game.damageMultiplier, 1, 8, "x%d");

                config::ImGuiCheckbox("Instant Death", getSettings().game.instantDeath);
                if (ImGui::IsItemHovered()) {
                    ImGui::SetTooltip("Any hit will instantly kill you");
                }

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Technical")) {
                config::ImGuiCheckbox("Restore Wii 1.0 Glitches", getSettings().game.restoreWiiGlitches);
                if (ImGui::IsItemHovered()) {
                    ImGui::SetTooltip("Restores patched glitches from Wii USA 1.0,\n"
                                      "the first released version");
                }

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Tools")) {
                config::ImGuiCheckbox("Enable Turbo Key", getSettings().game.enableTurboKeybind);

                ImGui::EndMenu();
            }

            ImGui::EndMenu();
        }
    }
}
