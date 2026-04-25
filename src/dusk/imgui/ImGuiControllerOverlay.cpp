#include "m_Do/m_Do_controller_pad.h"

#include "imgui.h"
#include <imgui_internal.h>
#include "ImGuiConsole.hpp"
#include "ImGuiMenuGame.hpp"

#include <dolphin/pad.h>

namespace dusk {
    void ImGuiMenuGame::windowInputViewer() {
        if (!m_showInputViewer) {
            return;
        }

        const char* controller_name = PADGetName(PAD_1);
        if (controller_name != nullptr) {
            m_controllerName = controller_name;
        }

        ImGuiIO& io = ImGui::GetIO();
        ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize |
            ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
        if (m_inputOverlayCorner != -1) {
            SetOverlayWindowLocation(m_inputOverlayCorner);
            windowFlags |= ImGuiWindowFlags_NoMove;
        }

        ImGui::SetNextWindowBgAlpha(0.65f);
        if (ImGui::Begin("Input Viewer", nullptr, windowFlags)) {
            float scale = ImGuiScale();
            if (!m_controllerName.empty()) {
                ImGuiTextCenter(m_controllerName);
                ImGui::Separator();
            }

            ImDrawList* dl = ImGui::GetWindowDrawList();
            ImVec2 p = ImGui::GetCursorScreenPos();

            // Left Stick vars
            float leftStickRadius = 30 * scale;
            p.x += 20 * scale;
            p.y += 20 * scale;
            ImVec2 leftStickCenter;
            leftStickCenter.x = p.x + 30 * scale;
            leftStickCenter.y = p.y + 45 * scale;

            // Right Stick vars
            float rightStickRadius = 20 * scale;
            ImVec2 rightStickCenter;
            rightStickCenter.x = p.x + 160 * scale;
            rightStickCenter.y = p.y + 90 * scale;

            // D-Pad vars
            float dpadRadius = 15 * scale;
            float dpadWidth = 8 * scale;
            ImVec2 dpadCenter;
            dpadCenter.x = p.x + 80 * scale;
            dpadCenter.y = p.y + 90 * scale;

            // Start Button vars
            float startButtonRadius = 5 * scale;
            ImVec2 startButtonCenter;
            startButtonCenter.x = p.x + 120 * scale;
            startButtonCenter.y = p.y + 55 * scale;

            // A Button vars
            float aButtonRadius = 16 * scale;
            ImVec2 aButtonCenter;
            aButtonCenter.x = p.x + 210 * scale;
            aButtonCenter.y = p.y + 48 * scale;

            // B Button vars
            float bButtonRadius = 8 * scale;
            ImVec2 bButtonCenter;
            bButtonCenter.x = aButtonCenter.x + -24 * scale;
            bButtonCenter.y = aButtonCenter.y + 16 * scale;

            // X Button vars
            ImVec2 xButtonRadius{ 7 * scale, 12 * scale};
            ImVec2 xButtonCenter;
            xButtonCenter.x = aButtonCenter.x + 24 * scale;
            xButtonCenter.y = aButtonCenter.y + -5 * scale;

            // Y Button vars
            ImVec2 yButtonRadius{ 12 * scale, 7 * scale };
            ImVec2 yButtonCenter;
            yButtonCenter.x = aButtonCenter.x + -8 * scale;
            yButtonCenter.y = aButtonCenter.y + -24 * scale;

            // Trigger vars
            float triggerWidth = leftStickRadius * 2;
            float triggerHeight = 8 * scale;
            ImVec2 lTrigCenter;
            lTrigCenter.x = leftStickCenter.x + 0 * scale;
            lTrigCenter.y = leftStickCenter.y + -60 * scale;
            ImVec2 rTrigCenter;
            rTrigCenter.x = aButtonCenter.x * scale;
            rTrigCenter.y = lTrigCenter.y * scale;

            // Z Button vars
            ImVec2 zButtonRadius{ 10 * scale, 5 * scale };
            ImVec2 zButtonCenter;
            zButtonCenter.x = aButtonCenter.x + 18 * scale;
            zButtonCenter.y = aButtonCenter.y + -30 * scale;

            const float zButtonHalfWidth = triggerWidth / 2;
            const float zButtonHalfHeight = 4 * scale;

            constexpr ImU32 stickGray = IM_COL32(150, 150, 150, 255);
            constexpr ImU32 darkGray = IM_COL32(60, 60, 60, 255);
            constexpr ImU32 red = IM_COL32(230, 0, 0, 255);
            constexpr ImU32 yellow = IM_COL32(255, 219, 109, 255);
            constexpr ImU32 white = IM_COL32(255, 255, 255, 255);
            constexpr ImU32 green = IM_COL32(0, 225, 255, 255);
            constexpr ImU32 purple = IM_COL32(165, 75, 165, 255);

            // Draw Left Stick
            {
                float x = mDoCPd_c::getStickX(PAD_1);
                float y = -mDoCPd_c::getStickY(PAD_1);
                dl->AddCircleFilled(leftStickCenter, leftStickRadius, stickGray, 8);

                ImVec2 center;
                center.x = leftStickCenter.x + x * (leftStickRadius / 2);
                center.y = leftStickCenter.y + y * (leftStickRadius / 2);
                dl->AddCircleFilled(center, leftStickRadius / 2, white);
            }

            // Draw Right Stick
            {
                float x = mDoCPd_c::getSubStickX(PAD_1);
                float y = -mDoCPd_c::getSubStickY(PAD_1);
                dl->AddCircleFilled(rightStickCenter, rightStickRadius, stickGray, 8);

                ImVec2 center;
                center.x = rightStickCenter.x + x * (rightStickRadius / 2);
                center.y = rightStickCenter.y + y * (rightStickRadius / 2);
                dl->AddCircleFilled(center, leftStickRadius / 3, yellow);
            }

            // Draw D-Pad
            {
                float halfWidth = dpadWidth / 2;
                {
                    ImVec2 pmin;
                    pmin.x = dpadCenter.x + -halfWidth;
                    pmin.y = dpadCenter.y + -dpadRadius;
                    ImVec2 pmax;
                    pmax.x = dpadCenter.x + halfWidth;
                    pmax.y = dpadCenter.y + dpadRadius;
                    dl->AddRectFilled(pmin, pmax, stickGray);
                }
                {
                    ImVec2 pmin;
                    pmin.x = dpadCenter.x + -dpadRadius;
                    pmin.y = dpadCenter.y + -halfWidth;
                    ImVec2 pmax;
                    pmax.x = dpadCenter.x + dpadRadius;
                    pmax.y = dpadCenter.y + halfWidth;
                    dl->AddRectFilled(pmin, pmax, stickGray);
                }

                if (mDoCPd_c::getHoldUp(PAD_1)) {
                    ImVec2 pmin;
                    pmin.x = dpadCenter.x + -halfWidth;
                    pmin.y = dpadCenter.y + -dpadRadius;
                    ImVec2 pmax;
                    pmax.x = dpadCenter.x + halfWidth;
                    pmax.y = dpadCenter.y + -dpadRadius / 2;
                    dl->AddRectFilled(pmin, pmax, white);
                }
                if (mDoCPd_c::getHoldDown(PAD_1)) {
                    ImVec2 pmin;
                    pmin.x = dpadCenter.x + -halfWidth;
                    pmin.y = dpadCenter.y + dpadRadius;
                    ImVec2 pmax;
                    pmax.x = dpadCenter.x + halfWidth;
                    pmax.y = dpadCenter.y + dpadRadius / 2;
                    dl->AddRectFilled(pmin, pmax, white);
                }
                if (mDoCPd_c::getHoldLeft(PAD_1)) {
                    ImVec2 pmin;
                    pmin.x = dpadCenter.x + -dpadRadius;
                    pmin.y = dpadCenter.y + -halfWidth;
                    ImVec2 pmax;
                    pmax.x = dpadCenter.x + -dpadRadius / 2;
                    pmax.y = dpadCenter.y + halfWidth;
                    dl->AddRectFilled(pmin, pmax, white);
                }
                if (mDoCPd_c::getHoldRight(PAD_1)) {
                    ImVec2 pmin;
                    pmin.x = dpadCenter.x + dpadRadius;
                    pmin.y = dpadCenter.y + -halfWidth;
                    ImVec2 pmax;
                    pmax.x = dpadCenter.x + dpadRadius / 2;
                    pmax.y = dpadCenter.y + halfWidth;
                    dl->AddRectFilled(pmin, pmax, white);
                }
            }

            // Draw Buttons
            {
                // start
                dl->AddCircleFilled(startButtonCenter, startButtonRadius, mDoCPd_c::getHoldStart(PAD_1) ? white : stickGray);

                // a
                dl->AddCircleFilled(aButtonCenter, aButtonRadius, mDoCPd_c::getHoldA(PAD_1) ? green : stickGray);

                // b
                dl->AddCircleFilled(bButtonCenter, bButtonRadius, mDoCPd_c::getHoldB(PAD_1) ? red : stickGray);

                // x
                dl->AddEllipseFilled(xButtonCenter, xButtonRadius, mDoCPd_c::getHoldX(PAD_1) ? white : stickGray, 25);

                // y
                dl->AddEllipseFilled(yButtonCenter, yButtonRadius, mDoCPd_c::getHoldY(PAD_1) ? white : stickGray, 25);

                // z
                dl->AddEllipseFilled(zButtonCenter, zButtonRadius, mDoCPd_c::getHoldZ(PAD_1) ? purple : stickGray, 35);
            }

            // Draw Triggers
            {
                float halfTriggerWidth = triggerWidth / 2;
                ImVec2 lStart;
                lStart.x = lTrigCenter.x - halfTriggerWidth;
                lStart.y = lTrigCenter.y - 0;
                ImVec2 lEnd;
                lEnd.x = lTrigCenter.x + halfTriggerWidth;
                lEnd.y = lTrigCenter.y + triggerHeight;

                float lValue = triggerWidth * std::min(1.f, mDoCPd_c::getAnalogL(PAD_1));
                ImVec2 p2;
                p2.x = lStart.x + lValue;
                p2.y = lStart.y + triggerHeight;
                dl->AddRectFilled(lStart, p2, mDoCPd_c::getHoldL(PAD_1) ? IM_COL32(0, 225, 0, 255) : white);
                p2.x = lStart.x + lValue;
                p2.y = lStart.y;
                dl->AddRectFilled(p2, lEnd, darkGray);

                ImVec2 rStart;
                rStart.x = rTrigCenter.x - halfTriggerWidth;
                rStart.y = rTrigCenter.y - 0;
                ImVec2 rEnd;
                rEnd.x = rTrigCenter.x + halfTriggerWidth;
                rEnd.y = rTrigCenter.y + triggerHeight;

                float rValue = triggerWidth * std::min(1.f, mDoCPd_c::getAnalogR(PAD_1));
                p2.x = rEnd.x - rValue;
                p2.y = rEnd.y - triggerHeight;
                dl->AddRectFilled(p2, rEnd, mDoCPd_c::getHoldR(PAD_1) ? IM_COL32(0, 225, 0, 255) : white);
                p2.x = rEnd.x - rValue;
                p2.y = rEnd.y;
                dl->AddRectFilled(rStart, p2, darkGray);
            }

            ImVec2 size;
            size.x = 270 * scale;
            size.y = 130 * scale;
            ImGui::Dummy(size);

            if (PADHasSensor(PAD_1, PAD_SENSOR_GYRO) == TRUE) {
                ImGui::Separator();
                ImGui::Checkbox("Gyro Values", &m_showInputViewerGyro);
                if (m_showInputViewerGyro) {
                    ImGui::TextUnformatted("Gyro");

                    constexpr float kBarScale = 4.0f;
                    auto bar = [kBarScale](const char* label, float v) {
                        const float a = std::fabs(v);
                        const float t = std::min(1.f, a / kBarScale);
                        char overlay[32];
                        snprintf(overlay, sizeof(overlay), "%s %+.3f", label, v);
                        ImGui::ProgressBar(t, ImVec2(-1, 0), overlay);
                    };

                    if (PADSetSensorEnabled(PAD_1, PAD_SENSOR_GYRO, TRUE) == TRUE) {
                        f32 gyro[3];
                        if (PADGetSensorData(PAD_1, PAD_SENSOR_GYRO, gyro, 3) == TRUE) {
                            bar("X", gyro[0]);
                            bar("Y", gyro[1]);
                            bar("Z", gyro[2]);
                        }
                    } else {
                        bar("X", 0.f);
                        bar("Y", 0.f);
                        bar("Z", 0.f);
                    }
                }
            }

            ShowCornerContextMenu(m_inputOverlayCorner, 0);
        }

        ImGui::End();
    }
}  // namespace dusk
