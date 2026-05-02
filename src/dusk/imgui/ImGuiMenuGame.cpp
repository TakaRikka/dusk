#include "fmt/format.h"
#include "imgui.h"

#include "ImGuiEngine.hpp"
#include "ImGuiConsole.hpp"
#include "ImGuiConfig.hpp"

#include "dusk/main.h"
#include "dusk/hotkeys.h"
#include "m_Do/m_Do_main.h"

#include <SDL3/SDL_gamepad.h>

namespace dusk {
    void ImGuiMenuGame::ToggleFullscreen() {
        getSettings().video.enableFullscreen.setValue(!getSettings().video.enableFullscreen);
        VISetWindowFullscreen(getSettings().video.enableFullscreen);
        config::Save();
    }

    ImGuiMenuGame::ImGuiMenuGame() {}

    void ImGuiMenuGame::draw() {
        if (ImGui::BeginMenu("Settings")) {
            // TODO: Remove this once Controller Config exists in RmlUi
            if (ImGui::Button("Configure Controller")){
                m_showControllerConfig = !m_showControllerConfig;
            }

            ImGui::Checkbox("Show Input Viewer", &m_showInputViewer);

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

    void ImGuiMenuGame::drawInterfaceMenu() {
        if (ImGui::BeginMenu("Interface")) {
            config::ImGuiCheckbox("Achievement Notifications", getSettings().game.enableAchievementNotifications);
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
        ImGuiBeginGroupPanel("Options", ImVec2(150 * scale, -1));

        if (deadZones != nullptr) {
            if (ImGui::Checkbox("Enable Dead Zones", &deadZones->useDeadzones)) {
                PADSerializeMappings();
            }
            if (ImGui::Checkbox("Emulate Triggers", &deadZones->emulateTriggers)) {
                PADSerializeMappings();
            }
        }
        
        if (PADSupportsRumbleIntensity(m_controllerConfig.m_selectedPort)) {
            ImGuiBeginGroupPanel("Rumble Intensity", ImVec2(150 * scale, -1));
            u16 low;
            u16 high;
            (void)PADGetRumbleIntensity(m_controllerConfig.m_selectedPort, &low, &high);
            float fLow = (static_cast<float>(low) / 32767.f) * 100.f;
            bool changed = ImGui::SliderFloat("Low", &fLow, 0.f, 100.f, "%.0f%%");
            float fHigh = (static_cast<float>(high) / 32767.f) * 100.f;
            changed |= ImGui::SliderFloat("High", &fHigh, 0.f, 100.f, "%.0f%%");
            if (changed) {
                PADSetRumbleIntensity(m_controllerConfig.m_selectedPort,
                    static_cast<u16>((fLow / 100) * 32767),
                    static_cast<u16>((fHigh / 100) * 32767));
                PADSerializeMappings();
            }
            if (ImGui::Button(fmt::format("{0}...##rumbleTest", m_controllerConfig.m_isRumbling ? "Stop": "Test").c_str(), {-1, 0})) {
                PADControlMotor(m_controllerConfig.m_selectedPort, !m_controllerConfig.m_isRumbling ? PAD_MOTOR_RUMBLE : PAD_MOTOR_STOP_HARD);
                m_controllerConfig.m_isRumbling ^= 1;
            } 
            ImGuiEndGroupPanel();
        }
        ImGuiEndGroupPanel();

        ImGui::End();
    }

    static std::string GetFormattedTime(OSTime ticks) {
        OSCalendarTime time;
        OSTicksToCalendarTime(ticks, &time);

        return fmt::format("{0:02}:{1:02}:{2:02}.{3:03}", time.hour, time.min, time.sec, time.msec);
    }

    void ImGuiMenuGame::resetForSpeedrunMode() {
        // reset settings that should be off for speedrun mode
        mDoMain::developmentMode = -1;

        getSettings().game.damageMultiplier.setValue(1);
        getSettings().game.instantDeath.setValue(false);
        getSettings().game.noHeartDrops.setValue(false);

        getSettings().game.infiniteHearts.setValue(false);
        getSettings().game.infiniteArrows.setValue(false);
        getSettings().game.infiniteBombs.setValue(false);
        getSettings().game.infiniteOil.setValue(false);
        getSettings().game.infiniteOxygen.setValue(false);
        getSettings().game.infiniteRupees.setValue(false);
        getSettings().game.enableIndefiniteItemDrops.setValue(false);

        getSettings().game.moonJump.setValue(false);
        getSettings().game.superClawshot.setValue(false);
        getSettings().game.alwaysGreatspin.setValue(false);
        getSettings().game.enableFastIronBoots.setValue(false);
        getSettings().game.canTransformAnywhere.setValue(false);
        getSettings().game.fastSpinner.setValue(false);
        getSettings().game.freeMagicArmor.setValue(false);

        getSettings().game.enableTurboKeybind.setValue(false);
    }

    SpeedrunInfo m_speedrunInfo;

    void ImGuiMenuGame::drawSpeedrunTimerOverlay() {
        if (!getSettings().game.speedrunMode) {
            return;
        }

        // L+R+A+Start to reset timer
        if (mDoCPd_c::getHoldL(PAD_1) && mDoCPd_c::getHoldR(PAD_1) && mDoCPd_c::getHoldA(PAD_1) && mDoCPd_c::getTrigStart(PAD_1)) {
            m_speedrunInfo.reset();
        }

        // L+R+A+Z to manually stop timer
        if (mDoCPd_c::getHoldL(PAD_1) && mDoCPd_c::getHoldR(PAD_1) && mDoCPd_c::getHoldA(PAD_1) && mDoCPd_c::getTrigZ(PAD_1)) {
            if (m_speedrunInfo.m_isRunStarted) {
                m_speedrunInfo.m_endTimestamp = OSGetTime() - m_speedrunInfo.m_startTimestamp;
                m_speedrunInfo.m_isRunStarted = false;
            }
        }

        ImGui::SetNextWindowBgAlpha(0.65f);
        ImGuiWindowFlags flags =
            ImGuiWindowFlags_NoResize
            | ImGuiWindowFlags_NoDocking
            | ImGuiWindowFlags_NoTitleBar
            | ImGuiWindowFlags_NoScrollbar;

        if (ImGui::Begin("##SpeedrunTimerWindow", nullptr, flags)) {
            OSTime elapsedTime = 0;
            if (m_speedrunInfo.m_isRunStarted) {
                elapsedTime = OSGetTime() - m_speedrunInfo.m_startTimestamp;
            } else if (m_speedrunInfo.m_endTimestamp != 0) {
                elapsedTime = m_speedrunInfo.m_endTimestamp;
            }

            ImGui::Text("RTA");
            ImGui::SameLine(60.0f);
            ImGuiStringViewText(GetFormattedTime(elapsedTime));

            if (!m_speedrunInfo.m_isPauseIGT) {
                m_speedrunInfo.m_igtTimer = elapsedTime - m_speedrunInfo.m_totalLoadTime;
            }

            ImGui::Text("IGT");
            ImGui::SameLine(60.0f);
            ImGuiStringViewText(GetFormattedTime(m_speedrunInfo.m_igtTimer));
        }
        ImGui::End();
    }
}
