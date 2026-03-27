#include "fmt/format.h"
#include "imgui.h"
#include "aurora/gfx.h"

#include "ImGuiConsole.hpp"
#include "ImGuiMenuGame.hpp"
#include <imgui_internal.h>

#include "JSystem/JUtility/JUTGamePad.h"
#include "m_Do/m_Do_controller_pad.h"
#include "m_Do/m_Do_audio.h"
#include <SDL3/SDL_gamepad.h>

namespace dusk {
    ImGuiMenuGame::ImGuiMenuGame() {}

    void ImGuiMenuGame::draw() {
        if (ImGui::BeginMenu("Game")) {
            if (ImGui::MenuItem("Reset", "Ctrl+R")) {
                JUTGamePad::C3ButtonReset::sResetSwitchPushing = true;
            }

            ImGui::Separator();

            if (ImGui::BeginMenu("Graphics")) {
                ImGui::Checkbox("Native Bloom", &m_graphicsSettings.m_enableBloom);
                ImGui::Checkbox("Water Projection Offset", &m_graphicsSettings.m_waterProjectionOffset);
                if (ImGui::IsItemHovered()) {
                    ImGui::SetTooltip("Adds GC-specific -0.01 transS offset\n"
                                      "that causes ~6px ghost artifacts in water reflections");
                }
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Audio")) {
                ImGui::Text("Master Volume");
                ImGui::SliderFloat("##m_masterVolume", &m_audioSettings.m_masterVolume, 0.0f, 1.0f, "");

                ImGui::Text("Main Music Volume");
                ImGui::SliderFloat("##m_mainMusicVolume", &m_audioSettings.m_mainMusicVolume, 0.0f, 1.0f, "");

                ImGui::Text("Sub Music Volume");
                ImGui::SliderFloat("##m_subMusicVolume", &m_audioSettings.m_subMusicVolume, 0.0f, 1.0f, "");

                ImGui::Text("Sound Effects Volume");
                ImGui::SliderFloat("##m_soundEffectsVolume", &m_audioSettings.m_soundEffectsVolume, 0.0f, 1.0f, "");

                ImGui::Text("Fanfare Volume");
                ImGui::SliderFloat("##m_fanfareVolume", &m_audioSettings.m_fanfareVolume, 0.0f, 1.0f, "");

                Z2AudioMgr* audioMgr = Z2AudioMgr::getInterface();
                if (audioMgr != nullptr) {
                    // TODO: actually apply volume settings
                }

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Controller")) {
                ImGui::MenuItem("Configure Controller", nullptr, &m_showControllerConfig);
                ImGui::Checkbox("Show Input Viewer", &m_showInputViewer);

                ImGui::EndMenu();
            }

            ImGui::EndMenu();
        }

        windowInputViewer();
        windowControllerConfig();

        if ((ImGui::IsKeyDown(ImGuiKey_LeftCtrl) || ImGui::IsKeyDown(ImGuiKey_RightCtrl)) && ImGui::IsKeyPressed(ImGuiKey_R)) {
            JUTGamePad::C3ButtonReset::sResetSwitchPushing = true;
        }
    }

    static void drawVirtualStick(const char* id, const ImVec2& stick) {
        ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPos().x + 5, ImGui::GetCursorPos().y));

        ImGui::BeginChild(id, ImVec2(80, 80));
        ImDrawList* dl = ImGui::GetWindowDrawList();
        ImVec2 p = ImGui::GetCursorScreenPos();

        float radius = ImGui::GetCurrentContext()->CurrentDpiScale * 30.0f;
        ImVec2 pos = ImVec2(p.x + radius, p.y + radius);

        constexpr ImU32 stickGray = IM_COL32(150, 150, 150, 255);
        constexpr ImU32 white = IM_COL32(255, 255, 255, 255);
        constexpr ImU32 red = IM_COL32(230, 0, 0, 255);

        dl->AddCircleFilled(pos, radius, stickGray, 8);
        dl->AddCircleFilled(ImVec2(pos.x + stick.x * (radius), pos.y + -stick.y * (radius)), 3, red);
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

        // if pending for an input mapping, check to set new input
        if (m_controllerConfig.m_pendingMapping != nullptr) {
            s32 nativeButton = PADGetNativeButtonPressed(m_controllerConfig.m_pendingPort);
            if (nativeButton != -1) {
                m_controllerConfig.m_pendingMapping->nativeButton = nativeButton;
                m_controllerConfig.m_pendingMapping = nullptr;
                m_controllerConfig.m_pendingPort = -1;
                PADBlockInput(false);
            }
        }

        ImGuiWindowFlags windowFlags =
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_AlwaysAutoResize;

        ImGui::SetNextWindowBgAlpha(0.65f);
        ImGui::SetNextWindowSizeConstraints(ImVec2(850, 400), ImVec2(850, 400));

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
        if (m_controllerConfig.m_pendingMapping != nullptr &&
            m_controllerConfig.m_pendingPort != m_controllerConfig.m_selectedPort)
        {
            m_controllerConfig.m_pendingMapping = nullptr;
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
        constexpr float buttonSize = 40;

        ImGuiBeginGroupPanel("Buttons", ImVec2(150, 20));

        SDL_Gamepad* gamepad = PADGetSDLGamepadForIndex(PADGetIndexForPort(m_controllerConfig.m_selectedPort));
        u32 buttonCount;
        PADButtonMapping* mappingList = PADGetButtonMappings(m_controllerConfig.m_selectedPort, &buttonCount);
        if (mappingList != nullptr) {
            for (int i = 0; i < buttonCount; i++) {
                const char* btnName = PADGetButtonName(mappingList[i].padButton);
                ImVec2 len = ImGui::CalcTextSize(btnName);
                ImVec2 pos = ImGui::GetCursorPos();

                ImGui::SetCursorPosY(pos.y + len.y / 4);
                ImGui::SetCursorPosX(pos.x + abs(len.x - buttonSize));
                ImGui::Text("%s", btnName);
                ImGui::SameLine();

                ImGui::SetCursorPosY(pos.y);

                bool pressed = ImGui::Button(m_controllerConfig.m_isReading && m_controllerConfig.m_pendingMapping == &mappingList[i]
                    ? fmt::format("Press a Key...##{}", btnName).c_str()
                    : fmt::format("{0}##-{1}", GetNameForGamepadButton(gamepad, mappingList[i].nativeButton), i).c_str(),
                    ImVec2(100.0f, 20.0f));

                if (pressed) {
                    m_controllerConfig.m_isReading = true;
                    m_controllerConfig.m_pendingPort = m_controllerConfig.m_selectedPort;
                    m_controllerConfig.m_pendingMapping = &mappingList[i];
                    PADBlockInput(true);
                }
            }
        }

        ImGuiEndGroupPanel();
        ImGui::SameLine();

        int port = m_controllerConfig.m_selectedPort;

        const char* stickDirections[] = {
            "Up",
            "Down",
            "Left",
            "Right",
        };

        // main stick panel
        ImGuiBeginGroupPanel("Control Stick", ImVec2(150, 20));

        drawVirtualStick("##mainStick", ImVec2{ mDoCPd_c::getStickX(port), mDoCPd_c::getStickY(port) });

        {
            for (int i = 0; i < 4; i++) {
                const char* label = stickDirections[i];
                ImVec2 len = ImGui::CalcTextSize(label);
                ImVec2 pos = ImGui::GetCursorPos();

                ImGui::SetCursorPosY(pos.y + len.y / 4);
                ImGui::SetCursorPosX(pos.x + abs(len.x - buttonSize));
                ImGui::Text("%s", label);
                ImGui::SameLine();

                ImGui::SetCursorPosY(pos.y);

                bool pressed = ImGui::Button(fmt::format("Temp##{}", label).c_str(), ImVec2(100.0f, 20.0f));
            }
        }

        PADDeadZones* deadZones = PADGetDeadZones(port);

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
        ImGuiBeginGroupPanel("C Stick", ImVec2(150, 20));

        drawVirtualStick("##subStick", ImVec2{ mDoCPd_c::getSubStickX(port), mDoCPd_c::getSubStickY(port) });

        {
            for (int i = 0; i < 4; i++) {
                const char* label = stickDirections[i];
                ImVec2 len = ImGui::CalcTextSize(label);
                ImVec2 pos = ImGui::GetCursorPos();

                ImGui::SetCursorPosY(pos.y + len.y / 4);
                ImGui::SetCursorPosX(pos.x + abs(len.x - buttonSize));
                ImGui::Text("%s", label);
                ImGui::SameLine();

                ImGui::SetCursorPosY(pos.y);

                bool pressed = ImGui::Button(fmt::format("Temp##sub{}", label).c_str(), ImVec2(100.0f, 20.0f));
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

        // Triggers Panel
        ImGuiBeginGroupPanel("Triggers", ImVec2(150, 20));

        if (deadZones != nullptr) {
            ImGui::Text("L Threshold");
            {
                float tmp = static_cast<float>(deadZones->leftTriggerActivationZone * 100.f) / 32767.f;
                if (ImGui::DragFloat("##LThreshold", &tmp, 0.5f, 0.f, 100.f, "%.3f%%")) {
                    deadZones->leftTriggerActivationZone = static_cast<u16>((tmp / 100.f) * 32767);
                }
            }
        }

        if (deadZones != nullptr) {
            ImGui::Text("R Threshold");
            {
                float tmp = static_cast<float>(deadZones->rightTriggerActivationZone * 100.f) / 32767.f;
                if (ImGui::DragFloat("##RThreshold", &tmp, 0.5f, 0.f, 100.f, "%.3f%%")) {
                    deadZones->rightTriggerActivationZone = static_cast<u16>((tmp / 100.f) * 32767);
                }
            }
        }

        ImGuiEndGroupPanel();
        ImGui::SameLine();

        // Options panel
        ImGuiBeginGroupPanel("Options", ImVec2(150, 20));

        if (deadZones != nullptr) {
            ImGui::Checkbox("Enable Dead Zones", &deadZones->useDeadzones);
            ImGui::Checkbox("Emulate Triggers", &deadZones->emulateTriggers);
        }

        ImGuiEndGroupPanel();

        ImGui::End();
    }
}
