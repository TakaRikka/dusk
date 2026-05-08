#include "f_op/f_op_camera_mng.h"
#include "SSystem/SComponent/c_xyz.h"
#include "d/d_com_inf_game.h"

#include "imgui.h"
#include "ImGuiConfig.hpp"
#include "ImGuiConsole.hpp"
#include "ImGuiMenuTools.hpp"
#include "dusk/settings.h"

namespace dusk {
    void ImGuiMenuTools::ShowCameraOverlay() {
        if (!getSettings().backend.enableAdvancedSettings ||
            !ImGuiConsole::CheckMenuViewToggle(ImGuiKey_F9, m_showCameraOverlay))
        {
            return;
        }

        auto* cam = (camera_process_class*)dCam_getCamera();

        if (!m_showCameraOverlay || cam == nullptr)
            return;

        auto* dCam = &cam->mCamera;

        ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize |
            ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
        if (m_cameraOverlayCorner != -1) {
            SetOverlayWindowLocation(m_cameraOverlayCorner);
            windowFlags |= ImGuiWindowFlags_NoMove;
        }

        // ImGui::SetNextWindowBgAlpha(0.65f);

        if (!ImGui::Begin("Camera Debug", nullptr, windowFlags)) {
            ImGui::End();
            return;
        }

        ImGui::SeparatorText("Camera Transform Data");

        cXyz center = dCam->mCenter;
        cXyz eye = dCam->mEye;

        if (ImGui::InputFloat3("Camera Center", &center.x)) {
            dCam->Reset(center, eye);
        }
        if (ImGui::InputFloat3("Camera Eye", &eye.x)) {
            dCam->Reset(center, eye);
        }

        ImGui::InputFloat("Camera FOV", &dCam->mFovy);

        ImGui::SeparatorText("Options");

        bool eventRunning = (dComIfGp_event_runCheck() || dComIfGp_isPauseFlag()) && !getSettings().game.debugFlyCam;
        if (eventRunning) {
            ImGui::BeginDisabled();
        }
        config::ImGuiCheckbox("Fly Mode", getSettings().game.debugFlyCam);
        if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) {
            if (eventRunning) {
                ImGui::SetTooltip("Cannot enable while paused or during an active event.");
            } else {
                ImGui::SetTooltip("Detach camera and fly freely.\n"
                                  "WASD/Arrows/Left stick: move, Mouse/C-stick: look\n"
                                  "Ctrl/L: down, Space/R: up, Shift/Z: fast\n"
                                "Q Key/Y: roll left, R Key/X: roll right");
            }
        }
        if (eventRunning) {
            ImGui::EndDisabled();
        }

        if (!getSettings().game.debugFlyCam) {
            ImGui::BeginDisabled();
        }
        config::ImGuiCheckbox("Lock Events", getSettings().game.debugFlyCamLockEvents);
        if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) {
            if (!getSettings().game.debugFlyCam) {
                ImGui::SetTooltip("Enable Fly Mode first.");
            } else {
                ImGui::SetTooltip("Freeze game events while flying.");
            }
        }
        if (!getSettings().game.debugFlyCam) {
            ImGui::EndDisabled();
        }

        ShowCornerContextMenu(m_cameraOverlayCorner, 0);

        ImGui::End();
    }
}
