#include "f_op/f_op_camera_mng.h"
#include "SSystem/SComponent/c_xyz.h"

#include "imgui.h"
#include "imgui.hpp"

static bool Active = false;
static int m_cameraOverlayCorner = 3;

void DuskCameraDebug() {
    if (ImGui::BeginMenu(MenuView)) {
        ImGui::MenuItem("Camera", "F6", &Active);

        ImGui::EndMenu();
    }

    if (ImGui::IsKeyPressed(ImGuiKey_F6)) {
        Active = !Active;
    }

    auto* cam = (camera_process_class*)dCam_getCamera();

    if (!Active || cam == nullptr)
        return;

    auto* dCam = &cam->mCamera;

    ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize |
        ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
    if (m_cameraOverlayCorner != -1) {
        SetOverlayWindowLocation(m_cameraOverlayCorner);
        windowFlags |= ImGuiWindowFlags_NoMove;
    }

    ImGui::SetNextWindowBgAlpha(0.65f);
    ImGui::SetNextWindowSizeConstraints(ImVec2(300, 0), ImVec2(FLT_MAX, FLT_MAX));

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

    ImGui::SeparatorText("Free-look Data");

    static float eyeYawDeg = 0.0f;
    static float moveSpeed = 10000.0f;
    static float rotSpeed = 5.0f;
    static cXyz freeLookPos = cXyz::Zero;

    bool changed = false;

    if (ImGui::IsKeyDown(ImGuiKey_LeftArrow)) {
        eyeYawDeg += rotSpeed;
        if (eyeYawDeg >= 360.0f)
            eyeYawDeg -= 360.0f;

        changed = true;
    } else if (ImGui::IsKeyDown(ImGuiKey_RightArrow)) {
        eyeYawDeg -= rotSpeed;
        if (eyeYawDeg < 0.0f)
            eyeYawDeg += 360.0f;

        changed = true;
    }
    cSAngle yawAngle = cSAngle(eyeYawDeg);
    cXyz frontDir = cXyz(yawAngle.Sin(), 0.0f, yawAngle.Cos());

    if (ImGui::IsKeyDown(ImGuiKey_UpArrow)) {
        freeLookPos -= frontDir * moveSpeed * ImGui::GetIO().DeltaTime;
        changed = true;
    } else if (ImGui::IsKeyDown(ImGuiKey_DownArrow)) {
        freeLookPos += frontDir * moveSpeed * ImGui::GetIO().DeltaTime;
        changed = true;
    }

    if (ImGui::IsKeyDown(ImGuiKey_LeftShift)) {
        freeLookPos += cXyz::BaseY * moveSpeed * ImGui::GetIO().DeltaTime;
        changed = true;
    }

    if (ImGui::IsKeyDown(ImGuiKey_LeftCtrl)) {
        freeLookPos -= cXyz::BaseY * moveSpeed * ImGui::GetIO().DeltaTime;
        changed = true;
    }

    if (changed) {
        dCam->Reset(freeLookPos, freeLookPos + (frontDir * 100.0f));
    }

    ImGui::InputFloat("Free-look Yaw", &eyeYawDeg);
    ImGui::InputFloat3("Free-look Position", &freeLookPos.x);
    ImGui::InputFloat("Free-look Move Speed", &moveSpeed);
    ImGui::InputFloat("Free-look Rotation Speed", &rotSpeed);

    ShowCornerContextMenu(m_cameraOverlayCorner, 0);

    ImGui::End();
}