#include "fmt/format.h"
#include "imgui.h"

#include "ImGuiConsole.hpp"
#include "ImGuiMenuGame.hpp"
#include "ImGuiConfig.hpp"
#include <imgui_internal.h>

#include "JSystem/JUtility/JUTGamePad.h"
#include "dusk/audio/DuskAudioSystem.h"
#include "dusk/audio/DuskDsp.hpp"
#include "dusk/dusk.h"
#include "dusk/hotkeys.h"
#include "dusk/settings.h"
#include "m_Do/m_Do_controller_pad.h"
#include "m_Do/m_Do_graphic.h"

#include <aurora/gfx.h>

namespace dusk {
    void ImGuiMenuGame::ToggleFullscreen() {
        getSettings().video.enableFullscreen.setValue(!getSettings().video.enableFullscreen);
        VISetWindowFullscreen(getSettings().video.enableFullscreen);
        config::Save();
    }

    ImGuiMenuGame::ImGuiMenuGame() {}

    void ImGuiMenuGame::draw() {
        if (ImGui::BeginMenu("Game")) {
            if (ImGui::MenuItem("Reset", hotkeys::DO_RESET)) {
                JUTGamePad::C3ButtonReset::sResetSwitchPushing = true;
            }

            ImGui::Separator();

            if (ImGui::BeginMenu("Graphics")) {
                if (ImGui::MenuItem("Toggle Fullscreen", hotkeys::TOGGLE_FULLSCREEN)) {
                    ToggleFullscreen();
                }

                if (ImGui::MenuItem("Default Window Size")) {
                    getSettings().video.enableFullscreen.setValue(false);
                    VISetWindowFullscreen(false);
                    VISetWindowSize(FB_WIDTH * 2, FB_HEIGHT * 2);
                    VICenterWindow();
                }

                bool vsync = getSettings().video.enableVsync;
                if (ImGui::Checkbox("Enable Vsync", &vsync)) {
                    getSettings().video.enableVsync.setValue(vsync);
                    aurora_enable_vsync(vsync);
                    config::Save();
                }

                bool lockAspect = getSettings().video.lockAspectRatio;
                if (ImGui::Checkbox("Force 4:3 Aspect Ratio", &lockAspect)) {
                    getSettings().video.lockAspectRatio.setValue(lockAspect);

                    if (lockAspect) {
                        VILockAspectRatio(defaultAspectRatioW, defaultAspectRatioH);
                    } else {
                        VIUnlockAspectRatio();
                    }

                    config::Save();
                }

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Audio")) {
                ImGui::Text("Master Volume");
                if (config::ImGuiSliderInt("##masterVolume", getSettings().audio.masterVolume, 0, 100)) {
                    dusk::audio::SetMasterVolume(getSettings().audio.masterVolume / 100.0f);
                }

                if (config::ImGuiCheckbox("Enable Reverb", getSettings().audio.enableReverb)) {
                    dusk::audio::SetEnableReverb(getSettings().audio.enableReverb);
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

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Controller")) {
                ImGui::MenuItem("Configure Controller", nullptr, &m_showControllerConfig);
                ImGui::Checkbox("Show Input Viewer", &m_showInputViewer);

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Misc")) {
                config::ImGuiCheckbox("Skip Pre-Launch UI", getSettings().backend.skipPreLaunchUI);

                ImGui::EndMenu();
            }

            ImGui::EndMenu();
        }

        windowInputViewer();
        windowControllerConfig();
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
            } else {
                auto nativeButton = PADGetNativeButtonPressed(m_controllerConfig.m_pendingPort);
                if (nativeButton != -1) {
                    m_controllerConfig.m_pendingAxisMapping->nativeAxis = {-1, AXIS_SIGN_POSITIVE};
                    m_controllerConfig.m_pendingAxisMapping->nativeButton = nativeButton;
                    m_controllerConfig.m_pendingAxisMapping = nullptr;
                    m_controllerConfig.m_pendingPort = -1;
                    PADBlockInput(false);
                }
            }
        }

        float scale = ImGuiScale();
        ImGuiWindowFlags windowFlags =
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_AlwaysAutoResize;

        ImGui::SetNextWindowBgAlpha(0.65f);

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
        }

        // save mappings button
        ImGui::SameLine();
        if (ImGui::Button("Save")) {
            PADSerializeMappings();
        }

        // restore defaults button
        ImGui::SameLine();
        if (ImGui::Button("Default")) {
            PADRestoreDefaultMapping(m_controllerConfig.m_selectedPort);
        }

        // buttons panel
        const float uiButtonSize = 40 * scale;

        ImGuiBeginGroupPanel("Buttons", ImVec2(150 * scale, 20 * scale));

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
                    dispName = fmt::format("{0}##-{1}", PADGetNativeButtonName(btnMappingList[i].nativeButton), i);
                }
                bool pressed = ImGui::Button(dispName.c_str(),
                    ImVec2(100.0f * scale, 20.0f * scale));

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
                    ImVec2(100.0f * scale, 20.0f * scale));

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
                bool pressed = ImGui::Button(dispName.c_str(), ImVec2(100.0f * scale, 20.0f * scale));

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
                bool pressed = ImGui::Button(fmt::format("{0}##sub{1}", dispName, label).c_str(), ImVec2(100.0f * scale, 20.0f * scale));

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
                }
            }
        }

        ImGuiEndGroupPanel();
        ImGui::SameLine();

        // Options panel
        ImGuiBeginGroupPanel("Options", ImVec2(150 * scale, 20 * scale));

        if (deadZones != nullptr) {
            ImGui::Checkbox("Enable Dead Zones", &deadZones->useDeadzones);
            ImGui::Checkbox("Emulate Triggers", &deadZones->emulateTriggers);
        }

        ImGuiEndGroupPanel();

        ImGui::End();
    }
}
