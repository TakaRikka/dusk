#include "m_Do/m_Do_controller_pad.h"

#include "imgui.h"
#include "ImGuiConsole.hpp"

void DuskDebugPad() {
    auto& pad = mDoCPd_c::getCpadInfo(PAD_1);
    auto KeyFlag = [&](ImGuiKey key, u32 padFlag) {
        if (ImGui::IsKeyDown(key))
            pad.mButtonFlags |= padFlag;
        if (ImGui::IsKeyPressed(key))
            pad.mPressedButtonFlags |= padFlag;

    };

    KeyFlag(ImGuiKey_K, PAD_BUTTON_A);
    KeyFlag(ImGuiKey_J, PAD_BUTTON_B);
    KeyFlag(ImGuiKey_L, PAD_BUTTON_X);
    KeyFlag(ImGuiKey_I, PAD_BUTTON_Y);
    KeyFlag(ImGuiKey_H, PAD_BUTTON_START);
    KeyFlag(ImGuiKey_O, PAD_TRIGGER_Z);
    KeyFlag(ImGuiKey_Keypad8, PAD_BUTTON_UP);
    KeyFlag(ImGuiKey_Keypad2, PAD_BUTTON_DOWN);
    KeyFlag(ImGuiKey_Keypad6, PAD_BUTTON_RIGHT);
    KeyFlag(ImGuiKey_Keypad4, PAD_BUTTON_LEFT);

    if (ImGui::IsKeyDown(ImGuiKey_W)) {
        pad.mMainStickPosY = 1.0f;
        pad.mMainStickValue = 1.0f;
        pad.mMainStickAngle = 0x8000;
    }

    if (ImGui::IsKeyDown(ImGuiKey_S)) {
        pad.mMainStickPosY = -1.0f;
        pad.mMainStickValue = 1.0f;
        pad.mMainStickAngle = 0;
    }

    if (ImGui::IsKeyDown(ImGuiKey_D)) {
        pad.mMainStickPosX = 1.0f;
        pad.mMainStickValue = 1.0f;
        pad.mMainStickAngle = 0x4000;
    }

    if (ImGui::IsKeyDown(ImGuiKey_A)) {
        pad.mMainStickPosX = -1.0f;
        pad.mMainStickValue = 1.0f;
        pad.mMainStickAngle = -0x4000;
    }
}
