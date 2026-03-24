#include "fmt/format.h"
#include "imgui.h"
#include "aurora/gfx.h"

#include "ImGuiConsole.hpp"
#include "ImGuiMenuGame.hpp"
#include <imgui_internal.h>

#include "JSystem/JUtility/JUTGamePad.h"
#include "m_Do/m_Do_controller_pad.h"
#include "m_Do/m_Do_audio.h"

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

    // from https://github.com/ocornut/imgui/issues/1496#issuecomment-569892444
    void ImGuiBeginGroupPanel(const char* name, const ImVec2& size) {
        ImGui::BeginGroup();

        auto cursorPos = ImGui::GetCursorScreenPos();
        auto itemSpacing = ImGui::GetStyle().ItemSpacing;
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.0f, 0.0f));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));

        auto frameHeight = ImGui::GetFrameHeight();
        ImGui::BeginGroup();

        ImVec2 effectiveSize = size;
        if (size.x < 0.0f)
            effectiveSize.x = ImGui::GetContentRegionAvail().x;
        else
            effectiveSize.x = size.x;
        ImGui::Dummy(ImVec2(effectiveSize.x, 0.0f));

        ImGui::Dummy(ImVec2(frameHeight * 0.5f, 0.0f));
        ImGui::SameLine(0.0f, 0.0f);
        ImGui::BeginGroup();
        ImGui::Dummy(ImVec2(frameHeight * 0.5f, 0.0f));
        ImGui::SameLine(0.0f, 0.0f);
        ImGui::TextUnformatted(name);
        ImGui::SameLine(0.0f, 0.0f);
        ImGui::Dummy(ImVec2(0.0, frameHeight + itemSpacing.y));
        ImGui::BeginGroup();

        ImGui::PopStyleVar(2);

        ImGui::GetCurrentWindow()->ContentRegionRect.Max.x -= frameHeight * 0.5f;
        ImGui::GetCurrentWindow()->WorkRect.Max.x -= frameHeight * 0.5f;
        ImGui::GetCurrentWindow()->Size.x -= frameHeight;

        ImGui::PushItemWidth(effectiveSize.x - frameHeight);
    }

    // from https://github.com/ocornut/imgui/issues/1496#issuecomment-569892444
    void ImGuiEndGroupPanel() {
        ImGui::PopItemWidth();

        auto itemSpacing = ImGui::GetStyle().ItemSpacing;

        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.0f, 0.0f));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));

        auto frameHeight = ImGui::GetFrameHeight();

        // workaround for incorrect capture of columns/table width by placing
        // zero-sized dummy element in the same group, this ensure
        // max X cursor position is updated correctly
        ImGui::SameLine(0.0f, 0.0f);
        ImGui::Dummy(ImVec2(0.0f, 0.0f));

        ImGui::EndGroup();
        ImGui::EndGroup();

        ImGui::SameLine(0.0f, 0.0f);
        ImGui::Dummy(ImVec2(frameHeight * 0.5f, 0.0f));
        ImGui::Dummy(ImVec2(0.0, frameHeight - frameHeight * 0.5f - itemSpacing.y));

        ImGui::EndGroup();

        auto itemMin = ImGui::GetItemRectMin();
        auto itemMax = ImGui::GetItemRectMax();

        ImVec2 halfFrame = ImVec2((frameHeight * 0.25f) * 0.5f, frameHeight * 0.5f);
        ImGui::GetWindowDrawList()->AddRect(
            ImVec2(itemMin.x + halfFrame.x, itemMin.y + halfFrame.y),
            ImVec2(itemMax.x - halfFrame.x, itemMax.y),
            ImColor(ImGui::GetStyleColorVec4(ImGuiCol_Border)),
            halfFrame.x);

        ImGui::PopStyleVar(2);

        ImGui::GetCurrentWindow()->ContentRegionRect.Max.x += frameHeight * 0.5f;
        ImGui::GetCurrentWindow()->WorkRect.Max.x += frameHeight * 0.5f;
        ImGui::GetCurrentWindow()->Size.x += frameHeight;

        ImGui::Dummy(ImVec2(0.0f, 0.0f));

        ImGui::EndGroup();
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

        if (!ImGui::Begin("Controller Config", nullptr, windowFlags)) {
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
                    : fmt::format("{0}##-{1}", PADGetNativeButtonName(mappingList[i].nativeButton), i).c_str(),
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
