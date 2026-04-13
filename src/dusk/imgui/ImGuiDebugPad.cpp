#include "m_Do/m_Do_controller_pad.h"

#include "imgui.h"
#include "ImGuiConsole.hpp"

void DuskDebugPad() {
    if (ImGui::IsKeyDown(ImGuiKey_K)) {
        mDoCPd_c::getCpadInfo(PAD_1).mPressedButtonFlags |= PAD_BUTTON_A;
    }

    if (ImGui::IsKeyDown(ImGuiKey_J)) {
        mDoCPd_c::getCpadInfo(PAD_1).mPressedButtonFlags |= PAD_BUTTON_B;
    }

    if (ImGui::IsKeyDown(ImGuiKey_L)) {
        mDoCPd_c::getCpadInfo(PAD_1).mPressedButtonFlags |= PAD_BUTTON_X;
    }

    if (ImGui::IsKeyDown(ImGuiKey_I)) {
        mDoCPd_c::getCpadInfo(PAD_1).mPressedButtonFlags |= PAD_BUTTON_Y;
    }

    if (ImGui::IsKeyDown(ImGuiKey_H)) {
        mDoCPd_c::getCpadInfo(PAD_1).mPressedButtonFlags |= PAD_BUTTON_START;
    }

    if (ImGui::IsKeyDown(ImGuiKey_O)) {
        mDoCPd_c::getCpadInfo(PAD_1).mPressedButtonFlags |= PAD_TRIGGER_Z;
    }

    if (ImGui::IsKeyDown(ImGuiKey_Keypad8)) {
        mDoCPd_c::getCpadInfo(PAD_1).mPressedButtonFlags |= PAD_BUTTON_UP;
    }

    if (ImGui::IsKeyDown(ImGuiKey_Keypad2)) {
        mDoCPd_c::getCpadInfo(PAD_1).mPressedButtonFlags |= PAD_BUTTON_DOWN;
    }

    if (ImGui::IsKeyDown(ImGuiKey_Keypad6)) {
        mDoCPd_c::getCpadInfo(PAD_1).mPressedButtonFlags |= PAD_BUTTON_RIGHT;
    }

    if (ImGui::IsKeyDown(ImGuiKey_Keypad4)) {
        mDoCPd_c::getCpadInfo(PAD_1).mPressedButtonFlags |= PAD_BUTTON_LEFT;
    }

    if (ImGui::IsKeyDown(ImGuiKey_W)) {
        mDoCPd_c::getCpadInfo(PAD_1).mMainStickPosY = 1.0f;
        mDoCPd_c::getCpadInfo(PAD_1).mMainStickValue = 1.0f;
        mDoCPd_c::getCpadInfo(PAD_1).mMainStickAngle = 0x8000;
    }

    if (ImGui::IsKeyDown(ImGuiKey_S)) {
        mDoCPd_c::getCpadInfo(PAD_1).mMainStickPosY = -1.0f;
        mDoCPd_c::getCpadInfo(PAD_1).mMainStickValue = 1.0f;
        mDoCPd_c::getCpadInfo(PAD_1).mMainStickAngle = 0;
    }

    if (ImGui::IsKeyDown(ImGuiKey_D)) {
        mDoCPd_c::getCpadInfo(PAD_1).mMainStickPosX = 1.0f;
        mDoCPd_c::getCpadInfo(PAD_1).mMainStickValue = 1.0f;
        mDoCPd_c::getCpadInfo(PAD_1).mMainStickAngle = 0x4000;
    }

    if (ImGui::IsKeyDown(ImGuiKey_A)) {
        mDoCPd_c::getCpadInfo(PAD_1).mMainStickPosX = -1.0f;
        mDoCPd_c::getCpadInfo(PAD_1).mMainStickValue = 1.0f;
        mDoCPd_c::getCpadInfo(PAD_1).mMainStickAngle = -0x4000;
    }
}