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
                    ImGui::SetTooltip("Wallet sizes are like in the HD version. (500, 1000, 2000)");
                }

                config::ImGuiCheckbox("No Rupee Returns", getSettings().game.noReturnRupees);
                if (ImGui::IsItemHovered()) {
                    ImGui::SetTooltip("Always collect Rupees even if your Wallet is too full.");
                }

                config::ImGuiCheckbox("Disable Rupee Cutscenes", getSettings().game.disableRupeeCutscenes);
                if (ImGui::IsItemHovered()) {
                    ImGui::SetTooltip("Rupees won't play cutscenes after you've collected them the first time.");
                }

                config::ImGuiCheckbox("No Sword Recoil", getSettings().game.noSwordRecoil);
                if (ImGui::IsItemHovered()) {
                    ImGui::SetTooltip("Link won't recoil when his sword hits walls.");
                }

                config::ImGuiCheckbox("Faster Climbing", getSettings().game.fastClimbing);
                if (ImGui::IsItemHovered()) {
                    ImGui::SetTooltip("Quicker climbing on ladders and vines like the HD version.");
                }

                config::ImGuiCheckbox("No Climbing Miss Animation", getSettings().game.noMissClimbing);
                if (ImGui::IsItemHovered()) {
                    ImGui::SetTooltip("Prevents Link from playing a struggle animation\n"
                                      "when grabbing ledges or climbing on vines.");
                }

                config::ImGuiCheckbox("Faster Tears of Light", getSettings().game.fastTears);
                if (ImGui::IsItemHovered()) {
                    ImGui::SetTooltip("Tears of Light dropped by Shadow Insects pop out faster like the HD version.");
                }

                config::ImGuiCheckbox("Hide TV Settings Screen", getSettings().game.hideTvSettingsScreen);
                if (ImGui::IsItemHovered()) {
                    ImGui::SetTooltip("Hides the TV calibration screen shown when loading a save.");
                }

                config::ImGuiCheckbox("Skip Warning Screen", getSettings().game.skipWarningScreen);
                if (ImGui::IsItemHovered()) {
                    ImGui::SetTooltip("Skips the warning screen shown when loading the game.");
                }

                config::ImGuiCheckbox("Instant Saves", getSettings().game.instantSaves);
                if (ImGui::IsItemHovered()) {
                    ImGui::SetTooltip("Skip the delay when writing to the Memory Card.");
                }

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Preferences")) {
                config::ImGuiCheckbox("Mirror Mode", getSettings().game.enableMirrorMode);
                if (ImGui::IsItemHovered()) {
                    ImGui::SetTooltip("Mirrors the world, matching the Wii version of the game.");
                }

                config::ImGuiCheckbox("Invert Camera X Axis", getSettings().game.invertCameraXAxis);

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Graphics")) {
                config::ImGuiCheckbox("Unlock Framerate", getSettings().game.enableFrameInterpolation);
                const bool frameInterpolationHovered = ImGui::IsItemHovered();

                ImGui::SameLine();
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.72f, 0.2f, 1.0f));
                ImGui::TextUnformatted("[EXPERIMENTAL]");
                ImGui::PopStyleColor();

                if (frameInterpolationHovered || ImGui::IsItemHovered()) {
                    ImGui::SetTooltip("Uses inter-frame interpolation to enable higher frame rates.\nVisual artifacts, animation glitches, or instability may occur.");
                }

                config::ImGuiSliderInt("Shadow Resolution", getSettings().game.shadowResolutionMultiplier, 1, 8, "x%d");
                if (ImGui::IsItemHovered()) {
                    ImGui::SetTooltip("Improves the shadow resolution, making them higher quality.");
                }

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Audio")) {
                config::ImGuiCheckbox("No Low HP Sound", getSettings().game.noLowHpSound);
                if (ImGui::IsItemHovered()) {
                    ImGui::SetTooltip("Disable the beeping sound when having low health.");
                }

                config::ImGuiCheckbox("Non-Stop Midna's Lament", getSettings().game.midnasLamentNonStop);
                if (ImGui::IsItemHovered()) {
                    ImGui::SetTooltip("Prevents enemy music while Midna's Lament is playing.");
                }

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Cheats")) {
                config::ImGuiCheckbox("Fast Iron Boots", getSettings().game.enableFastIronBoots);

                config::ImGuiCheckbox("Can Transform Anywhere", getSettings().game.canTransformAnywhere);
                if (ImGui::IsItemHovered()) {
                    ImGui::SetTooltip("Allows you to transform even if NPCs are looking.");
                }

                config::ImGuiCheckbox("Fast Spinner", getSettings().game.fastSpinner);
                if (ImGui::IsItemHovered()) {
                    ImGui::SetTooltip("Speeds up Spinner movement when holding R.");
                }

                config::ImGuiCheckbox("Free Magic Armor", getSettings().game.freeMagicArmor);
                if (ImGui::IsItemHovered()) {
                    ImGui::SetTooltip("Makes the magic armor work without rupees.");
                }

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Difficulty")) {
                config::ImGuiSliderInt("Damage Multiplier", getSettings().game.damageMultiplier, 1, 8, "x%d");

                config::ImGuiCheckbox("No Heart Drops", getSettings().game.noHeartDrops);
                if (ImGui::IsItemHovered()) {
                    ImGui::SetTooltip("Hearts will never drop from enemies,\n"
                                      "pots and various other places.");
                }

                config::ImGuiCheckbox("Instant Death", getSettings().game.instantDeath);
                if (ImGui::IsItemHovered()) {
                    ImGui::SetTooltip("Any hit will instantly kill you.");
                }

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Technical")) {
                config::ImGuiCheckbox("Restore Wii 1.0 Glitches", getSettings().game.restoreWiiGlitches);
                if (ImGui::IsItemHovered()) {
                    ImGui::SetTooltip("Restores patched glitches from Wii USA 1.0,\n"
                                      "the first released version.");
                }

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Tools")) {
                config::ImGuiCheckbox("Enable Turbo Key", getSettings().game.enableTurboKeybind);
                if (ImGui::IsItemHovered()) {
                    ImGui::SetTooltip("Holding TAB will speed up the game.\n"
                                      "This will not work with the \"Unlock Framerate\" enhancement.");
                }

                ImGui::EndMenu();
            }

            ImGui::EndMenu();
        }
    }
}
