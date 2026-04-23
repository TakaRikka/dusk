#include "fmt/format.h"
#include "imgui.h"

#include "ImGuiEngine.hpp"
#include "ImGuiConsole.hpp"
#include "ImGuiMenuGame.hpp"
#include "ImGuiConfig.hpp"

#include "JSystem/JUtility/JUTGamePad.h"
#include "dusk/audio/DuskAudioSystem.h"
#include "dusk/audio/DuskDsp.hpp"
#include "dusk/main.h"
#include "dusk/hotkeys.h"
#include "dusk/settings.h"
#include "m_Do/m_Do_controller_pad.h"
#include "m_Do/m_Do_graphic.h"

#include <aurora/gfx.h>
#include <SDL3/SDL_gamepad.h>

namespace {
constexpr int kInternalResolutionScaleMax = 12;
}  // namespace

namespace aurora::gx {
extern bool enableLodBias;
}

namespace dusk {
    void ImGuiMenuGame::ToggleFullscreen() {
        getSettings().video.enableFullscreen.setValue(!getSettings().video.enableFullscreen);
        VISetWindowFullscreen(getSettings().video.enableFullscreen);
        config::Save();
    }

    ImGuiMenuGame::ImGuiMenuGame() {}

    void ImGuiMenuGame::draw() {
        if (ImGui::BeginMenu("Settings")) {
            drawAudioMenu();
            drawCheatsMenu();
            drawGameplayMenu();
            drawGraphicsMenu();
            drawInputMenu();
            drawInterfaceMenu();

            ImGui::Separator();

            if (ImGui::MenuItem("Reset", hotkeys::DO_RESET)) {
                JUTGamePad::C3ButtonReset::sResetSwitchPushing = true;
            }

            if (!IsMobile && ImGui::MenuItem("Exit")) {
                dusk::IsRunning = false;
            }

            ImGui::EndMenu();
        }
    }

    void ImGuiMenuGame::drawGraphicsMenu() {
        if (ImGui::BeginMenu("Graphics")) {
            ImGui::SeparatorText("Display");

            if (!IsMobile) {
                if (ImGui::MenuItem("Toggle Fullscreen", hotkeys::TOGGLE_FULLSCREEN)) {
                    ToggleFullscreen();
                }

                if (ImGui::Button("Restore Default Window Size")) {
                    getSettings().video.enableFullscreen.setValue(false);
                    VISetWindowFullscreen(false);
                    VISetWindowSize(FB_WIDTH * 2, FB_HEIGHT * 2);
                    VICenterWindow();
                }
            }

            ImGui::Separator();

            bool vsync = getSettings().video.enableVsync;
            if (ImGui::Checkbox("Enable VSync", &vsync)) {
                getSettings().video.enableVsync.setValue(vsync);
                aurora_enable_vsync(vsync);
                config::Save();
            }

            bool lockAspect = getSettings().video.lockAspectRatio;
            if (ImGui::Checkbox("Force 4:3 Aspect Ratio", &lockAspect)) {
                getSettings().video.lockAspectRatio.setValue(lockAspect);

                if (lockAspect) {
                    AuroraSetViewportPolicy(AURORA_VIEWPORT_FIT);
                } else {
                    AuroraSetViewportPolicy(AURORA_VIEWPORT_STRETCH);
                }

                config::Save();
            }

            ImGui::SeparatorText("Resolution");

            u32 internalResolutionWidth = 0;
            u32 internalResolutionHeight = 0;
            AuroraGetRenderSize(&internalResolutionWidth, &internalResolutionHeight);
            ImGui::TextDisabled("Current internal resolution: %ux%u", internalResolutionWidth,
                                internalResolutionHeight);

            int scale = std::clamp(getSettings().game.internalResolutionScale.getValue(), 0,
                                   kInternalResolutionScaleMax);
            if (ImGui::SliderInt("Internal Resolution", &scale, 0, kInternalResolutionScaleMax,
                                 scale == 0 ? "Auto" : "%dx"))
            {
                getSettings().game.internalResolutionScale.setValue(scale);
                VISetFrameBufferScale(static_cast<float>(scale));
                config::Save();
            }
            if (ImGui::IsItemHovered()) {
                ImGui::SetTooltip("Auto renders at the native window resolution.\n"
                                  "Higher values scale the game's internal framebuffer.");
            }

            config::ImGuiSliderInt("Shadow Resolution", getSettings().game.shadowResolutionMultiplier, 1, 8, "x%d");
            if (ImGui::IsItemHovered()) {
                ImGui::SetTooltip("Improves the shadow resolution, making them higher quality.");
            }

            ImGui::SeparatorText("Post-Processing");

            constexpr const char* bloomModeNames[] = {"Off", "Classic", "Dusk"};
            int bloomMode = static_cast<int>(getSettings().game.bloomMode.getValue());
            if (ImGui::BeginCombo("Bloom", bloomModeNames[bloomMode])) {
                for (int i = 0; i < IM_ARRAYSIZE(bloomModeNames); i++) {
                    const bool selected = bloomMode == i;
                    if (ImGui::Selectable(bloomModeNames[i], selected)) {
                        getSettings().game.bloomMode.setValue(static_cast<BloomMode>(i));
                        config::Save();
                    }
                    if (selected) {
                        ImGui::SetItemDefaultFocus();
                    }
                }
                ImGui::EndCombo();
            }

            bool bloomOff = bloomMode == static_cast<int>(BloomMode::Off);
            if (bloomOff) ImGui::BeginDisabled();
            float mult = getSettings().game.bloomMultiplier.getValue();
            if (ImGui::SliderFloat("Bloom Brightness", &mult, 0.0f, 1.0f, "%.2f")) {
                getSettings().game.bloomMultiplier.setValue(mult);
                config::Save();
            }
            if (bloomOff) ImGui::EndDisabled();

            ImGui::SeparatorText("Rendering");

            config::ImGuiCheckbox("Unlock Framerate", getSettings().game.enableFrameInterpolation);
            const bool frameInterpolationHovered = ImGui::IsItemHovered();
            ImGui::SameLine();
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.72f, 0.2f, 1.0f));
            ImGui::TextUnformatted("[EXPERIMENTAL]");
            ImGui::PopStyleColor();
            if (frameInterpolationHovered || ImGui::IsItemHovered()) {
                ImGui::SetTooltip("Uses inter-frame interpolation to enable higher frame rates.\nVisual artifacts, animation glitches, or instability may occur.");
            }

            ImGui::Checkbox("Enable LOD Bias", &aurora::gx::enableLodBias);

            config::ImGuiCheckbox("Enable Depth of Field", getSettings().game.enableDepthOfField);

            ImGui::EndMenu();
        }
    }

    void ImGuiMenuGame::drawGameplayMenu() {
        if (ImGui::BeginMenu("Gameplay")) {
            ImGui::SeparatorText("General");

            config::ImGuiCheckbox("Mirror Mode", getSettings().game.enableMirrorMode);
            if (ImGui::IsItemHovered()) {
                ImGui::SetTooltip("Mirrors the world horizontally, matching the Wii version of the game.");
            }

            config::ImGuiCheckbox("Disable Main HUD", getSettings().game.disableMainHUD);
            if (ImGui::IsItemHovered()) {
                ImGui::SetTooltip("Disables the main HUD of the game.\n"
                                  "Useful for recording or a more immersive experience!");
            }

            config::ImGuiCheckbox("Restore Wii 1.0 Glitches", getSettings().game.restoreWiiGlitches);
            if (ImGui::IsItemHovered()) {
                ImGui::SetTooltip("Restores patched glitches from Wii USA 1.0,\n"
                                  "the first released version.");
            }
            
            config::ImGuiCheckbox("Enable Rotating Link Doll", getSettings().game.enableLinkDollRotation);
            if (ImGui::IsItemHovered()) {
                ImGui::SetTooltip("Enables Rotating Link in the collection menu with the C-Stick");
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

            config::ImGuiCheckbox("Hold B for Instant Text", getSettings().game.instantText);
            if (ImGui::IsItemHovered()) {
                ImGui::SetTooltip("Make text scroll immediately by holding B.");
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
    }

    void ImGuiMenuGame::drawCheatsMenu() {
        if (ImGui::BeginMenu("Cheats")) {
            ImGui::SeparatorText("Resources");
            config::ImGuiCheckbox("Infinite Hearts", getSettings().game.infiniteHearts);
            config::ImGuiCheckbox("Infinite Arrows", getSettings().game.infiniteArrows);
            config::ImGuiCheckbox("Infinite Bombs", getSettings().game.infiniteBombs);
            config::ImGuiCheckbox("Infinite Oil", getSettings().game.infiniteOil);
            config::ImGuiCheckbox("Infinite Oxygen", getSettings().game.infiniteOxygen);
            config::ImGuiCheckbox("Infinite Rupees", getSettings().game.infiniteRupees);
            config::ImGuiCheckbox("Items Don't Despawn", getSettings().game.enableIndefiniteItemDrops);
            ImGui::SetItemTooltip("Items Don't Despawn Unless You Load A Different Room In Which Case They Do But Even Under Some Circumstances They Don't, It Is Quite Rare Though");

            ImGui::SeparatorText("Abilities");
            config::ImGuiCheckbox("Moon Jump (R+A)", getSettings().game.moonJump);
            config::ImGuiCheckbox("Super Clawshot", getSettings().game.superClawshot);
            config::ImGuiCheckbox("Always Greatspin", getSettings().game.alwaysGreatspin);
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
    }

    void ImGuiMenuGame::drawAudioMenu() {
        if (ImGui::BeginMenu("Audio")) {

            ImGui::SeparatorText("Volume");

            ImGui::Text("Master Volume");
            if (config::ImGuiSliderInt("##masterVolume", getSettings().audio.masterVolume, 0, 100)) {
                dusk::audio::SetMasterVolume(getSettings().audio.masterVolume / 100.0f);
            }

            /*
            // TODO: Implement additional settings
            ImGui::Text("Main Music Volume");
            ImGui::SliderFloat("##mainMusicVolume", &getSettings().audio.mainMusicVolume, 0, 100);

            ImGui::Text("Sub Music Volume");
            ImGui::SliderFloat("##subMusicVolume", &getSettings().audio.subMusicVolume, 0, 100);

            ImGui::Text("Sound Effects Volume");
            ImGui::SliderFloat("##soundEffectsVolume", &getSettings().audio.soundEffectsVolume, 0, 100);

            ImGui::Text("Fanfare Volume");
            ImGui::SliderFloat("##fanfareVolume", &getSettings().audio.fanfareVolume, 0, 100);

            Z2AudioMgr* audioMgr = Z2AudioMgr::getInterface();
            if (audioMgr != nullptr) {
            }
            */

            ImGui::SeparatorText("Effects");

            if (config::ImGuiCheckbox("Enable Reverb", getSettings().audio.enableReverb)) {
                dusk::audio::SetEnableReverb(getSettings().audio.enableReverb);
            }


            ImGui::SeparatorText("Tweaks");

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
    }

    void ImGuiMenuGame::drawInputMenu() {
        if (ImGui::BeginMenu("Input")) {
            ImGui::SeparatorText("Controller");

            if (ImGui::Button("Configure Controller")){
                m_showControllerConfig = !m_showControllerConfig;
            }

            ImGui::SeparatorText("Camera");

            config::ImGuiCheckbox("Invert Camera X Axis", getSettings().game.invertCameraXAxis);

            ImGui::SeparatorText("Gyro");

            config::ImGuiCheckbox("Gyro Aim", getSettings().game.enableGyroAim);
            if (ImGui::IsItemHovered()) {
                ImGui::SetTooltip("Enables the gyroscope on supported controllers\n"
                                  "while in look mode (C-Up) and while aiming the\n"
                                  "Slingshot, Gale Boomerang, Hero's Bow, Clawshot(s),\n"
                                  "Ball and Chain, and Dominion Rod.");
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

            ImGui::Checkbox("Show Input Viewer", &m_showInputViewer);

            ImGui::EndMenu();
        }
    }

    void ImGuiMenuGame::drawInterfaceMenu() {
        if (ImGui::BeginMenu("Interface")) {
            config::ImGuiCheckbox("Skip Pre-Launch UI", getSettings().backend.skipPreLaunchUI);
            config::ImGuiCheckbox("Show Pipeline Compilation", getSettings().backend.showPipelineCompilation);
#if DUSK_ENABLE_SENTRY_NATIVE
            config::ImGuiCheckbox("Enable Crash Reporting", getSettings().backend.enableCrashReporting);
#endif
            if (!IsMobile) {
                config::ImGuiCheckbox("Pause on Focus Lost", getSettings().game.pauseOnFocusLost);
            }

            ImGui::EndMenu();
        }
    }

    static void drawVirtualStick(const char* id, const ImVec2& stick) {
        float scale = ImGuiScale();
        ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPos().x + 45 * scale, ImGui::GetCursorPos().y + 10));

        ImGui::BeginChild(id, ImVec2(80 * scale, 80 * scale), 0, ImGuiWindowFlags_NoBackground);
        ImDrawList* dl = ImGui::GetWindowDrawList();
        ImVec2 p = ImGui::GetCursorScreenPos();

        float radius = 30.0f * scale;
        ImVec2 pos = ImVec2(p.x + radius, p.y + radius);

        constexpr ImU32 stickGray = IM_COL32(150, 150, 150, 255);
        constexpr ImU32 white = IM_COL32(255, 255, 255, 255);
        constexpr ImU32 red = IM_COL32(230, 0, 0, 255);

        dl->AddCircleFilled(pos, radius, stickGray, 8);
        dl->AddCircleFilled(ImVec2(pos.x + stick.x * (radius), pos.y + -stick.y * (radius)), 3 * scale, red);
        ImGui::EndChild();
    }

    struct SpecificButtonName {
        SDL_GamepadType Type;
        const char* Name;
    };

    struct ButtonNames {
        SDL_GamepadButton Button;
        std::vector<SpecificButtonName> Names;
    };

// clang-format off
    static const std::vector<ButtonNames> GamepadButtonNames = {
        { SDL_GAMEPAD_BUTTON_LEFT_STICK, {
           {SDL_GAMEPAD_TYPE_PS3, "L3"},
           {SDL_GAMEPAD_TYPE_PS4, "L3"},
           {SDL_GAMEPAD_TYPE_PS5, "L3"},
           {SDL_GAMEPAD_TYPE_XBOX360, "Left Stick"},
           {SDL_GAMEPAD_TYPE_XBOXONE, "Left Stick"},
           {SDL_GAMEPAD_TYPE_GAMECUBE, "Control Stick"},
        }},
        { SDL_GAMEPAD_BUTTON_RIGHT_STICK, {
           {SDL_GAMEPAD_TYPE_PS3, "R3"},
           {SDL_GAMEPAD_TYPE_PS4, "R3"},
           {SDL_GAMEPAD_TYPE_PS5, "R3"},
           {SDL_GAMEPAD_TYPE_XBOX360, "Right Stick"},
           {SDL_GAMEPAD_TYPE_XBOXONE, "Right Stick"},
           {SDL_GAMEPAD_TYPE_GAMECUBE, "C Stick"},
        }},
        { SDL_GAMEPAD_BUTTON_LEFT_SHOULDER, {
           {SDL_GAMEPAD_TYPE_PS3, "L1"},
           {SDL_GAMEPAD_TYPE_PS4, "L1"},
           {SDL_GAMEPAD_TYPE_PS5, "L1"},
           {SDL_GAMEPAD_TYPE_XBOX360, "LB"},
           {SDL_GAMEPAD_TYPE_XBOXONE, "LB"},
        }},
        { SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER, {
           {SDL_GAMEPAD_TYPE_PS3, "R1"},
           {SDL_GAMEPAD_TYPE_PS4, "R1"},
           {SDL_GAMEPAD_TYPE_PS5, "R1"},
           {SDL_GAMEPAD_TYPE_XBOX360, "RB"},
           {SDL_GAMEPAD_TYPE_XBOXONE, "RB"},
           {SDL_GAMEPAD_TYPE_GAMECUBE, "Z"},
        }},
        { SDL_GAMEPAD_BUTTON_BACK, {
           {SDL_GAMEPAD_TYPE_PS3, "Select"},
           {SDL_GAMEPAD_TYPE_PS4, "Share"},
           {SDL_GAMEPAD_TYPE_PS5, "Create"},
           {SDL_GAMEPAD_TYPE_XBOX360, "Back"},
           {SDL_GAMEPAD_TYPE_XBOXONE, "View"},
        }},
        { SDL_GAMEPAD_BUTTON_START, {
           {SDL_GAMEPAD_TYPE_PS3, "Start"},
           {SDL_GAMEPAD_TYPE_PS4, "Options"},
           {SDL_GAMEPAD_TYPE_PS5, "Options"},
           {SDL_GAMEPAD_TYPE_XBOX360, "Start"},
           {SDL_GAMEPAD_TYPE_XBOXONE, "Menu"},
           {SDL_GAMEPAD_TYPE_GAMECUBE, "Start/Pause"},
        }},
    };
// clang-format on

    static const char* GetNameForGamepadButton(SDL_Gamepad* gamepad, u32 buttonUntyped) {
        if (buttonUntyped == PAD_NATIVE_BUTTON_INVALID) {
            return "Not bound";
        }

        auto button = static_cast<SDL_GamepadButton>(buttonUntyped);
        auto label = SDL_GetGamepadButtonLabel(gamepad, button);

        switch (label) {
        case SDL_GAMEPAD_BUTTON_LABEL_A:
            return "A";
        case SDL_GAMEPAD_BUTTON_LABEL_B:
            return "B";
        case SDL_GAMEPAD_BUTTON_LABEL_X:
            return "X";
        case SDL_GAMEPAD_BUTTON_LABEL_Y:
            return "Y";
        case SDL_GAMEPAD_BUTTON_LABEL_CROSS:
            return "Cross";
        case SDL_GAMEPAD_BUTTON_LABEL_CIRCLE:
            return "Circle";
        case SDL_GAMEPAD_BUTTON_LABEL_TRIANGLE:
            return "Triangle";
        case SDL_GAMEPAD_BUTTON_LABEL_SQUARE:
            return "Square";
        default:; // Fall through
        }

        auto padType = SDL_GetGamepadType(gamepad);
        for (const auto& buttonNames : GamepadButtonNames) {
            if (buttonNames.Button != button) {
                continue;
            }

            for (const auto& name : buttonNames.Names) {
                if (name.Type == padType) {
                    return name.Name;
                }
            }
        }

        switch (button) {
        case SDL_GAMEPAD_BUTTON_DPAD_LEFT:
            return "D-pad left";
        case SDL_GAMEPAD_BUTTON_DPAD_RIGHT:
            return "D-pad right";
        case SDL_GAMEPAD_BUTTON_DPAD_UP:
            return "D-pad up";
        case SDL_GAMEPAD_BUTTON_DPAD_DOWN:
            return "D-pad down";
        default:
            return PADGetNativeButtonName(buttonUntyped);
        }
    }

    void ImGuiMenuGame::windowControllerConfig() {
        if (!m_showControllerConfig) {
            return;
        }

        // if pending for a button mapping, check to set new input
        if (m_controllerConfig.m_pendingButtonMapping != nullptr) {
            s32 nativeButton = PADGetNativeButtonPressed(m_controllerConfig.m_pendingPort);
            if (nativeButton != -1) {
                m_controllerConfig.m_pendingButtonMapping->nativeButton = nativeButton;
                m_controllerConfig.m_pendingButtonMapping = nullptr;
                m_controllerConfig.m_pendingPort = -1;
                PADBlockInput(false);
                PADSerializeMappings();
            }
        }

        // if pending for an axis mapping, check to set new input
        if (m_controllerConfig.m_pendingAxisMapping != nullptr) {
            auto nativeAxis = PADGetNativeAxisPulled(m_controllerConfig.m_pendingPort);
            if (nativeAxis.nativeAxis != -1) {
                m_controllerConfig.m_pendingAxisMapping->nativeAxis = nativeAxis;
                m_controllerConfig.m_pendingAxisMapping->nativeButton = -1;
                m_controllerConfig.m_pendingAxisMapping = nullptr;
                m_controllerConfig.m_pendingPort = -1;
                PADBlockInput(false);
                PADSerializeMappings();
            } else {
                auto nativeButton = PADGetNativeButtonPressed(m_controllerConfig.m_pendingPort);
                if (nativeButton != -1) {
                    m_controllerConfig.m_pendingAxisMapping->nativeAxis = {-1, AXIS_SIGN_POSITIVE};
                    m_controllerConfig.m_pendingAxisMapping->nativeButton = nativeButton;
                    m_controllerConfig.m_pendingAxisMapping = nullptr;
                    m_controllerConfig.m_pendingPort = -1;
                    PADBlockInput(false);
                    PADSerializeMappings();
                }
            }
        }

        float scale = ImGuiScale();
        ImGuiWindowFlags windowFlags =
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_AlwaysAutoResize;

        // ImGui::SetNextWindowBgAlpha(0.65f);

        if (!ImGui::Begin("Controller Config", &m_showControllerConfig, windowFlags)) {
            ImGui::End();
            return;
        }

        // port tabs
        ImGui::BeginTabBar("##ControllerTabs");
        for (int i = PAD_1; i <= PAD_4; i++) {
            if (ImGui::BeginTabItem(fmt::format("Port {}", i + 1).c_str())) {
                m_controllerConfig.m_selectedPort = i;
                ImGui::EndTabItem();
            }
        }
        ImGui::EndTabBar();

        // if tab is changed while waiting for input, cancel pending
        if ((m_controllerConfig.m_pendingButtonMapping != nullptr ||
             m_controllerConfig.m_pendingAxisMapping != nullptr) &&
            m_controllerConfig.m_pendingPort != m_controllerConfig.m_selectedPort)
        {
            m_controllerConfig.m_pendingButtonMapping = nullptr;
            m_controllerConfig.m_pendingAxisMapping = nullptr;
            m_controllerConfig.m_pendingPort = -1;
            PADBlockInput(false);
        }

        // get a list of all available controller's names
        std::vector<std::string> controllerList;
        controllerList.push_back("None");
        for (int i = 0; i < PADCount(); i++) {
            // attach index to name for unique name
            controllerList.push_back(fmt::format("{}-{}", PADGetNameForControllerIndex(i), i));
        }

        // get current controller name
        const char* tmpName = PADGetName(m_controllerConfig.m_selectedPort);
        std::string currentName = "None";
        if (tmpName != nullptr) {
            currentName = fmt::format("{}-{}", tmpName, PADGetIndexForPort(m_controllerConfig.m_selectedPort));
        }

        // controller selection combo box
        bool changedController = false;
        int changedControllerIndex = 0;
        ImGui::SetNextItemWidth(400.0f * scale);
        if (ImGui::BeginCombo("##ControllerDeviceList", currentName.c_str())) {
            for (int i = 0; const auto& name : controllerList) {
                if (ImGui::Selectable(name.c_str(), currentName == name)) {
                    changedControllerIndex = i;
                    changedController = true;
                }
                i++;
            }
            ImGui::EndCombo();
        }

        // handle controller change
        if (changedController) {
            if (changedControllerIndex > 0) {
                PADSetPortForIndex(changedControllerIndex - 1, m_controllerConfig.m_selectedPort);
            }
            else if (changedControllerIndex == 0) {
                // if "None" selected
                PADClearPort(m_controllerConfig.m_selectedPort);
            }
            PADSerializeMappings();
        }

        // restore defaults button
        ImGui::SameLine();
        if (ImGui::Button("Default")) {
            PADRestoreDefaultMapping(m_controllerConfig.m_selectedPort);
            PADSerializeMappings();
        }

        // buttons panel
        const float uiButtonSize = 40 * scale;
        ImVec2 btnSize(110.0f * scale, 30.0f * scale);

        ImGuiBeginGroupPanel("Buttons", ImVec2(150 * scale, 20 * scale));

        SDL_Gamepad* gamepad = PADGetSDLGamepadForIndex(PADGetIndexForPort(m_controllerConfig.m_selectedPort));
        u32 buttonCount;
        PADButtonMapping* btnMappingList = PADGetButtonMappings(m_controllerConfig.m_selectedPort, &buttonCount);
        if (btnMappingList != nullptr) {
            for (int i = 0; i < buttonCount; i++) {
                const char* btnName = PADGetButtonName(btnMappingList[i].padButton);
                ImVec2 len = ImGui::CalcTextSize(btnName);
                ImVec2 pos = ImGui::GetCursorPos();

                ImGui::SetCursorPosY(pos.y + len.y / 4);
                ImGui::SetCursorPosX(pos.x + abs(len.x - uiButtonSize));
                ImGui::Text("%s", btnName);
                ImGui::SameLine();

                ImGui::SetCursorPosY(pos.y);

                std::string dispName;
                if (m_controllerConfig.m_isReading && m_controllerConfig.m_pendingButtonMapping == &btnMappingList[i]) {
                    dispName = fmt::format("Press a Key...##{}", btnName);
                } else {
                    const char* nativeName = GetNameForGamepadButton(gamepad, btnMappingList[i].nativeButton);
                    if (nativeName == nullptr) {
                        nativeName = "[unbound]";
                    }
                    dispName = fmt::format("{0}##-{1}", nativeName, i);
                }
                bool pressed = ImGui::Button(dispName.c_str(),
                    btnSize);

                if (pressed) {
                    m_controllerConfig.m_isReading = true;
                    m_controllerConfig.m_pendingPort = m_controllerConfig.m_selectedPort;
                    m_controllerConfig.m_pendingButtonMapping = &btnMappingList[i];
                    PADBlockInput(true);
                }
            }
        }

        ImGuiEndGroupPanel();
        ImGui::SameLine();

        uint32_t axisCount;
        PADAxisMapping* axisMappingList = PADGetAxisMappings(m_controllerConfig.m_selectedPort, &axisCount);

        ImGuiBeginGroupPanel("Analog Triggers", ImVec2(150 * scale, 20 * scale));

        PADAxis triggers[] = {PAD_AXIS_TRIGGER_L, PAD_AXIS_TRIGGER_R};
        if (axisMappingList != nullptr) {
            for (PADAxis trigger : triggers) {
                const char* axisName = PADGetAxisName(axisMappingList[trigger].padAxis);
                ImVec2 len = ImGui::CalcTextSize(axisName);
                ImVec2 pos = ImGui::GetCursorPos();

                ImGui::SetCursorPosY(pos.y + len.y / 4);
                ImGui::SetCursorPosX(pos.x + abs(len.x - uiButtonSize));
                ImGui::Text("%s", axisName);
                ImGui::SameLine();

                ImGui::SetCursorPosY(pos.y);

                std::string dispName;
                if (m_controllerConfig.m_isReading && m_controllerConfig.m_pendingAxisMapping == &axisMappingList[trigger]) {
                    dispName = fmt::format("Press a Key...##{}", axisName);
                } else {
                    dispName = fmt::format("{0}##-{1}", PADGetNativeAxisName(axisMappingList[trigger].nativeAxis), trigger);
                }
                bool pressed = ImGui::Button(dispName.c_str(),
                    btnSize);

                if (pressed) {
                    m_controllerConfig.m_isReading = true;
                    m_controllerConfig.m_pendingPort = m_controllerConfig.m_selectedPort;
                    m_controllerConfig.m_pendingAxisMapping = &axisMappingList[trigger];
                    PADBlockInput(true);
                }
            }
        }

        int port = m_controllerConfig.m_selectedPort;
        PADDeadZones* deadZones = PADGetDeadZones(port);

        if (deadZones != nullptr) {
            ImGui::Text("L Threshold");
            ImGui::SameLine();
            {
                float tmp = static_cast<float>(deadZones->leftTriggerActivationZone * 100.f) / 32767.f;
                if (ImGui::DragFloat("##LThreshold", &tmp, 0.5f, 0.f, 100.f, "%.3f%%")) {
                    deadZones->leftTriggerActivationZone = static_cast<u16>((tmp / 100.f) * 32767);
                    PADSerializeMappings();
                }
            }
        }

        if (deadZones != nullptr) {
            ImGui::Text("R Threshold");
            ImGui::SameLine();
            {
                float tmp = static_cast<float>(deadZones->rightTriggerActivationZone * 100.f) / 32767.f;
                if (ImGui::DragFloat("##RThreshold", &tmp, 0.5f, 0.f, 100.f, "%.3f%%")) {
                    deadZones->rightTriggerActivationZone = static_cast<u16>((tmp / 100.f) * 32767);
                    PADSerializeMappings();
                }
            }
        }

        ImGuiEndGroupPanel();
        ImGui::SameLine();

        // main stick panel
        ImGuiBeginGroupPanel("Control Stick", ImVec2(150 * scale, 20 * scale));

        drawVirtualStick("##mainStick", ImVec2{ mDoCPd_c::getStickX(port), mDoCPd_c::getStickY(port) });

        if (axisMappingList != nullptr) {
            const PADAxis lStickAxes[] = {PAD_AXIS_LEFT_Y_POS, PAD_AXIS_LEFT_Y_NEG, PAD_AXIS_LEFT_X_NEG, PAD_AXIS_LEFT_X_POS};
            for (auto axis : lStickAxes) {
                const char* label = PADGetAxisDirectionLabel(axis);
                ImVec2 len = ImGui::CalcTextSize(label);
                ImVec2 pos = ImGui::GetCursorPos();

                ImGui::SetCursorPosY(pos.y + len.y / 4);
                ImGui::SetCursorPosX(pos.x + abs(len.x - uiButtonSize));
                ImGui::Text("%s", label);
                ImGui::SameLine();

                ImGui::SetCursorPosY(pos.y);

                std::string dispName;
                if (m_controllerConfig.m_isReading && m_controllerConfig.m_pendingAxisMapping == &axisMappingList[axis]) {
                    dispName = fmt::format("Press a Key...##{}", label);
                } else {
                    if (axisMappingList[axis].nativeAxis.nativeAxis != -1) {
                        const char* signStr;
                        if (axis == PAD_AXIS_TRIGGER_L || axis == PAD_AXIS_TRIGGER_R) {
                            signStr = "";
                        } else if (axisMappingList[axis].nativeAxis.sign == AXIS_SIGN_POSITIVE) {
                            signStr = "+";
                        } else {
                            signStr = "-";
                        }
                        dispName = fmt::format("{0}{1}##-{2}", PADGetNativeAxisName(axisMappingList[axis].nativeAxis), signStr, axis);
                    } else {
                        assert(axisMappingList[axis].nativeButton != -1);
                        dispName = fmt::format("{0}##-{1}", PADGetNativeButtonName(axisMappingList[axis].nativeButton), axis);
                    }
                }
                bool pressed = ImGui::Button(dispName.c_str(), btnSize);

                if (pressed) {
                    m_controllerConfig.m_isReading = true;
                    m_controllerConfig.m_pendingPort = m_controllerConfig.m_selectedPort;
                    m_controllerConfig.m_pendingAxisMapping = &axisMappingList[axis];
                    PADBlockInput(true);
                }
            }
        }

        if (deadZones != nullptr) {
            ImGui::Text("Dead Zone");
            {
                float tmp = static_cast<float>(deadZones->stickDeadZone * 100.f) / 32767.f;
                if (ImGui::DragFloat("##mainDeadZone", &tmp, 0.5f, 0.f, 100.f, "%.3f%%")) {
                    deadZones->stickDeadZone = static_cast<u16>((tmp / 100.f) * 32767);
                    PADSerializeMappings();
                }
            }
        }

        ImGuiEndGroupPanel();
        ImGui::SameLine();

        // sub stick panel
        ImGuiBeginGroupPanel("C Stick", ImVec2(150 * scale, 20 * scale));

        drawVirtualStick("##subStick", ImVec2{ mDoCPd_c::getSubStickX(port), mDoCPd_c::getSubStickY(port) });

        if (axisMappingList != nullptr) {
            const PADAxis rStickAxes[] = {PAD_AXIS_RIGHT_Y_POS, PAD_AXIS_RIGHT_Y_NEG, PAD_AXIS_RIGHT_X_NEG, PAD_AXIS_RIGHT_X_POS};
            for (auto axis : rStickAxes) {
                const char* label = PADGetAxisDirectionLabel(axisMappingList[axis].padAxis);
                ImVec2 len = ImGui::CalcTextSize(label);
                ImVec2 pos = ImGui::GetCursorPos();

                ImGui::SetCursorPosY(pos.y + len.y / 4);
                ImGui::SetCursorPosX(pos.x + abs(len.x - uiButtonSize));
                ImGui::Text("%s", label);
                ImGui::SameLine();

                ImGui::SetCursorPosY(pos.y);

                std::string dispName;
                if (m_controllerConfig.m_isReading && m_controllerConfig.m_pendingAxisMapping == &axisMappingList[axis]) {
                    dispName = fmt::format("Press a Key...##sub{}", label);
                } else {
                    if (axisMappingList[axis].nativeAxis.nativeAxis != -1) {
                        const char* signStr;
                        if (axis == PAD_AXIS_TRIGGER_L || axis == PAD_AXIS_TRIGGER_R) {
                            signStr = "";
                        } else if (axisMappingList[axis].nativeAxis.sign == AXIS_SIGN_POSITIVE) {
                            signStr = "+";
                        } else {
                            signStr = "-";
                        }
                        dispName = fmt::format("{0}{1}##-{2}", PADGetNativeAxisName(axisMappingList[axis].nativeAxis), signStr, axis);
                    } else {
                        assert(axisMappingList[axis].nativeButton != -1);
                        dispName = fmt::format("{0}##-{1}", PADGetNativeButtonName(axisMappingList[axis].nativeButton), axis);
                    }
                }
                bool pressed = ImGui::Button(fmt::format("{0}##sub{1}", dispName, label).c_str(), btnSize);

                if (pressed) {
                    m_controllerConfig.m_isReading = true;
                    m_controllerConfig.m_pendingPort = m_controllerConfig.m_selectedPort;
                    m_controllerConfig.m_pendingAxisMapping = &axisMappingList[axis];
                    PADBlockInput(true);
                }
            }
        }

        if (deadZones != nullptr) {
            ImGui::Text("Dead Zone");
            {
                float tmp = static_cast<float>(deadZones->substickDeadZone * 100.f) / 32767.f;
                if (ImGui::DragFloat("##subDeadZone", &tmp, 0.5f, 0.f, 100.f, "%.3f%%")) {
                    deadZones->substickDeadZone = static_cast<u16>((tmp / 100.f) * 32767);
                    PADSerializeMappings();
                }
            }
        }

        ImGuiEndGroupPanel();
        ImGui::SameLine();

        // Options panel
        ImGuiBeginGroupPanel("Options", ImVec2(150 * scale, 20 * scale));

        if (deadZones != nullptr) {
            if (ImGui::Checkbox("Enable Dead Zones", &deadZones->useDeadzones)) {
                PADSerializeMappings();
            }
            if (ImGui::Checkbox("Emulate Triggers", &deadZones->emulateTriggers)) {
                PADSerializeMappings();
            }
        }

        ImGuiEndGroupPanel();

        ImGui::End();
    }
}
