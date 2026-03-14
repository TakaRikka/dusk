#include "fmt/format.h"
#include "imgui.h"
#include "aurora/gfx.h"

#include "ImGuiConsole.hpp"
#include "ImGuiMenuGame.hpp"

#include "JSystem/JUtility/JUTGamePad.h"
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

    void ImGuiMenuGame::windowControllerConfig() {
        if (!m_showControllerConfig) {
            return;
        }

        ImGuiWindowFlags windowFlags =
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_AlwaysAutoResize;

        ImGui::SetNextWindowBgAlpha(0.65f);
        ImGui::SetNextWindowSizeConstraints(ImVec2(500, 200), ImVec2(1000, 200));

        if (!ImGui::Begin("Controller Config", nullptr, windowFlags)) {
            ImGui::End();
            return;
        }

        ImGui::End();
    }
}