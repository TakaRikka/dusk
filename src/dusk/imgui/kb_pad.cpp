#include "m_Do/m_Do_controller_pad.h"

#include "imgui.h"
#include "imgui.hpp"

void DuskDebugPad() {
    if (ImGui::IsKeyPressed(ImGuiKey_K)) {
        mDoCPd_c::getCpadInfo(PAD_1).mPressedButtonFlags |= PAD_BUTTON_A;
    }

    if (ImGui::IsKeyPressed(ImGuiKey_J)) {
        mDoCPd_c::getCpadInfo(PAD_1).mPressedButtonFlags |= PAD_BUTTON_B;
    }

    if (ImGui::IsKeyPressed(ImGuiKey_L)) {
        mDoCPd_c::getCpadInfo(PAD_1).mPressedButtonFlags |= PAD_BUTTON_X;
    }

    if (ImGui::IsKeyPressed(ImGuiKey_I)) {
        mDoCPd_c::getCpadInfo(PAD_1).mPressedButtonFlags |= PAD_BUTTON_Y;
    }

    if (ImGui::IsKeyPressed(ImGuiKey_H)) {
        mDoCPd_c::getCpadInfo(PAD_1).mPressedButtonFlags |= PAD_BUTTON_START;
    }

    if (ImGui::IsKeyPressed(ImGuiKey_O)) {
        mDoCPd_c::getCpadInfo(PAD_1).mPressedButtonFlags |= PAD_TRIGGER_Z;
    }

    if (ImGui::IsKeyPressed(ImGuiKey_Keypad8)) {
        mDoCPd_c::getCpadInfo(PAD_1).mPressedButtonFlags |= PAD_BUTTON_UP;
    }

    if (ImGui::IsKeyPressed(ImGuiKey_Keypad2)) {
        mDoCPd_c::getCpadInfo(PAD_1).mPressedButtonFlags |= PAD_BUTTON_DOWN;
    }

    if (ImGui::IsKeyPressed(ImGuiKey_Keypad6)) {
        mDoCPd_c::getCpadInfo(PAD_1).mPressedButtonFlags |= PAD_BUTTON_RIGHT;
    }

    if (ImGui::IsKeyPressed(ImGuiKey_Keypad4)) {
        mDoCPd_c::getCpadInfo(PAD_1).mPressedButtonFlags |= PAD_BUTTON_LEFT;
    }

    if (ImGui::IsKeyPressed(ImGuiKey_W)) {
        mDoCPd_c::getCpadInfo(PAD_1).mMainStickPosY = 1.0f;
        mDoCPd_c::getCpadInfo(PAD_1).mMainStickValue = 1.0f;
        mDoCPd_c::getCpadInfo(PAD_1).mMainStickAngle = 0x8000;
    }

    if (ImGui::IsKeyPressed(ImGuiKey_S)) {
        mDoCPd_c::getCpadInfo(PAD_1).mMainStickPosY = -1.0f;
        mDoCPd_c::getCpadInfo(PAD_1).mMainStickValue = 1.0f;
        mDoCPd_c::getCpadInfo(PAD_1).mMainStickAngle = 0;
    }

    if (ImGui::IsKeyPressed(ImGuiKey_D)) {
        mDoCPd_c::getCpadInfo(PAD_1).mMainStickPosX = 1.0f;
        mDoCPd_c::getCpadInfo(PAD_1).mMainStickValue = 1.0f;
        mDoCPd_c::getCpadInfo(PAD_1).mMainStickAngle = 0x4000;
    }

    if (ImGui::IsKeyPressed(ImGuiKey_A)) {
        mDoCPd_c::getCpadInfo(PAD_1).mMainStickPosX = -1.0f;
        mDoCPd_c::getCpadInfo(PAD_1).mMainStickValue = 1.0f;
        mDoCPd_c::getCpadInfo(PAD_1).mMainStickAngle = -0x4000;
    }
}