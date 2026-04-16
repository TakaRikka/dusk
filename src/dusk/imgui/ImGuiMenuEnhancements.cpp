#include "imgui.h"

#include "ImGuiMenuEnhancements.hpp"
#include "ImGuiConfig.hpp"
#include "dusk/settings.h"

namespace dusk {
    ImGuiMenuEnhancements::ImGuiMenuEnhancements() {}

    void ImGuiMenuEnhancements::draw() {
        if (ImGui::BeginMenu("Enhancements")) {
            if (ImGui::BeginMenu("Gameplay")) {
                ImGui::SeparatorText("Preferences");

                config::ImGuiCheckbox("Mirror Mode", getSettings().game.enableMirrorMode);
                if (ImGui::IsItemHovered()) {
                    ImGui::SetTooltip("Mirrors the world horizontally, matching the Wii version of the game.");
                }

                config::ImGuiCheckbox("Disable Main HUD", getSettings().game.disableMainHUD);
                if (ImGui::IsItemHovered()) {
                    ImGui::SetTooltip("Disables the main HUD of the game.\n"
                                      "Useful for recording or a more immersive experience!");
                }

                ImGui::SeparatorText("Difficulty");

                config::ImGuiSliderInt("Damage Multiplier", getSettings().game.damageMultiplier, 1, 8, "x%d");

                config::ImGuiCheckbox("Instant Death", getSettings().game.instantDeath);
                if (ImGui::IsItemHovered()) {
                    ImGui::SetTooltip("Any hit will instantly kill you.");
                }

                config::ImGuiCheckbox("No Heart Drops", getSettings().game.noHeartDrops);
                if (ImGui::IsItemHovered()) {
                    ImGui::SetTooltip("Hearts will never drop from enemies,\n"
                                      "pots and various other places.");
                }

                ImGui::SeparatorText("Quality of Life");

                config::ImGuiCheckbox("Bigger Wallets", getSettings().game.biggerWallets);
                if (ImGui::IsItemHovered()) {
                    ImGui::SetTooltip("Wallet sizes are like in the HD version. (500, 1000, 2000)");
                }

                config::ImGuiCheckbox("Disable Rupee Cutscenes", getSettings().game.disableRupeeCutscenes);
                if (ImGui::IsItemHovered()) {
                    ImGui::SetTooltip("Rupees won't play cutscenes after you've collected them the first time.");
                }

                config::ImGuiCheckbox("Faster Climbing", getSettings().game.fastClimbing);
                if (ImGui::IsItemHovered()) {
                    ImGui::SetTooltip("Quicker climbing on ladders and vines like the HD version.");
                }

                config::ImGuiCheckbox("Faster Tears of Light", getSettings().game.fastTears);
                if (ImGui::IsItemHovered()) {
                    ImGui::SetTooltip("Tears of Light dropped by Shadow Insects pop out faster like the HD version.");
                }

                config::ImGuiCheckbox("Instant Saves", getSettings().game.instantSaves);
                if (ImGui::IsItemHovered()) {
                    ImGui::SetTooltip("Skip the delay when writing to the Memory Card.");
                }

                config::ImGuiCheckbox("No Climbing Miss Animation", getSettings().game.noMissClimbing);
                if (ImGui::IsItemHovered()) {
                    ImGui::SetTooltip("Prevents Link from playing a struggle animation\n"
                                      "when grabbing ledges or climbing on vines.");
                }

                config::ImGuiCheckbox("No Rupee Returns", getSettings().game.noReturnRupees);
                if (ImGui::IsItemHovered()) {
                    ImGui::SetTooltip("Always collect Rupees even if your Wallet is too full.");
                }

                config::ImGuiCheckbox("No Sword Recoil", getSettings().game.noSwordRecoil);
                if (ImGui::IsItemHovered()) {
                    ImGui::SetTooltip("Link won't recoil when his sword hits walls.");
                }

                config::ImGuiCheckbox("Skip TV Settings Screen", getSettings().game.hideTvSettingsScreen);
                if (ImGui::IsItemHovered()) {
                    ImGui::SetTooltip("Skip the TV calibration screen shown when loading a save.");
                }

                config::ImGuiCheckbox("Skip Warning Screen", getSettings().game.skipWarningScreen);
                if (ImGui::IsItemHovered()) {
                    ImGui::SetTooltip("Skip the warning screen shown when starting the game.");
                }

                config::ImGuiCheckbox("Sun's Song (R+X)", getSettings().game.sunsSong);
                if (ImGui::IsItemHovered()) {
                    ImGui::SetTooltip("Allows Wolf Link to howl and change the time of day.");
                }

                config::ImGuiCheckbox("Quick Transform (R+Y)", getSettings().game.enableQuickTransform);
                if (ImGui::IsItemHovered()) {
                    ImGui::SetTooltip("Transform instantly by pressing R and Y simultaneously.");
                }

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Graphics")) {
                config::ImGuiSliderInt("Shadow Resolution", getSettings().game.shadowResolutionMultiplier, 1, 8, "x%d");
                if (ImGui::IsItemHovered()) {
                    ImGui::SetTooltip("Improves the shadow resolution, making them higher quality.");
                }

                config::ImGuiCheckbox("Unlock Framerate", getSettings().game.enableFrameInterpolation);
                const bool frameInterpolationHovered = ImGui::IsItemHovered();
                ImGui::SameLine();
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.72f, 0.2f, 1.0f));
                ImGui::TextUnformatted("[EXPERIMENTAL]");
                ImGui::PopStyleColor();
                if (frameInterpolationHovered || ImGui::IsItemHovered()) {
                    ImGui::SetTooltip("Uses inter-frame interpolation to enable higher frame rates.\nVisual artifacts, animation glitches, or instability may occur.");
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

            if (ImGui::BeginMenu("Input")) {
                config::ImGuiCheckbox("Invert Camera X Axis", getSettings().game.invertCameraXAxis);

                ImGui::SeparatorText("Gyro");

                config::ImGuiCheckbox("Gyro Aim", getSettings().game.enableGyroAim);
                if (ImGui::IsItemHovered()) {
                    ImGui::SetTooltip("Enables the gyroscope on supported controllers while aiming the\n"
                                      "Slingshot, Gale Boomerang, Hero's Bow, Clawshot(s), Ball and Chain, and Dominion Rod.");
                }

                config::ImGuiCheckbox("Gyro Rollgoal", getSettings().game.enableGyroRollgoal);
                if (ImGui::IsItemHovered()) {
                    ImGui::SetTooltip("Enables the gyroscope on supported controllers to\n"
                                      "tilt the Rollgoal table in Hena's Cabin.");
                }

                if (getSettings().game.enableGyroAim || getSettings().game.enableGyroRollgoal) {
                    config::ImGuiSliderFloat("Gyro Pitch Sensitivity", getSettings().game.gyroSensitivityY, 0.25f, 4.0f, "%.2f");
                    config::ImGuiSliderFloat("Gyro Yaw Sensitivity", getSettings().game.gyroSensitivityX, 0.25f, 4.0f, "%.2f");

                    if (getSettings().game.enableGyroRollgoal) {
                        config::ImGuiSliderFloat("Rollgoal Sensitivity", getSettings().game.gyroSensitivityRollgoal, 0.25f, 4.0f, "%.2f");
                        if (ImGui::IsItemHovered()) {
                            ImGui::SetTooltip("Additional multiplier for scaling how strongly\n"
                                              "the gyroscope affects the Rollgoal table.");
                        }
                    }

                    config::ImGuiSliderFloat("Gyro Deadband", getSettings().game.gyroDeadband, 0.0f, 0.5f, "%.3f");
                    if (ImGui::IsItemHovered()) {
                        ImGui::SetTooltip("Angular rates below this magnitude are treated as zero,\n"
                                          "reducing drift and jitter when the controller is still.");
                    }

                    config::ImGuiSliderFloat("Gyro Smoothing", getSettings().game.gyroSmoothing, 0.0f, 1.0f, "%.2f");
                    if (ImGui::IsItemHovered()) {
                        ImGui::SetTooltip("Low values track raw gyro input more closely,\n"
                                          "while higher values smooth out input over time.");
                    }

                    config::ImGuiCheckbox("Invert Gyro Pitch", getSettings().game.gyroInvertPitch);
                    config::ImGuiCheckbox("Invert Gyro Yaw", getSettings().game.gyroInvertYaw);
                }

                ImGui::SeparatorText("Tools");

                config::ImGuiCheckbox("Turbo Key", getSettings().game.enableTurboKeybind);
                if (ImGui::IsItemHovered()) {
                    ImGui::SetTooltip("Hold TAB to increase game speed by up to 4x.");
                }

                ImGui::EndMenu();
            }

            ImGui::Separator();

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

            if (ImGui::BeginMenu("Technical")) {
                config::ImGuiCheckbox("Restore Wii 1.0 Glitches", getSettings().game.restoreWiiGlitches);
                if (ImGui::IsItemHovered()) {
                    ImGui::SetTooltip("Restores patched glitches from Wii USA 1.0,\n"
                                      "the first released version.");
                }

                ImGui::EndMenu();
            }

            ImGui::EndMenu();
        }
    }
}
