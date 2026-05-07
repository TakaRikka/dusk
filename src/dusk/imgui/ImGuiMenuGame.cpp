#include "fmt/format.h"
#include "imgui.h"

#include "ImGuiConsole.hpp"
#include "ImGuiConfig.hpp"

#include "dusk/main.h"
#include "m_Do/m_Do_main.h"

namespace dusk {
    ImGuiMenuGame::ImGuiMenuGame() {}

    void ImGuiMenuGame::draw() {
        if (ImGui::BeginMenu("Settings")) {
            ImGui::Checkbox("Show Input Viewer", &m_showInputViewer);
            ImGui::EndMenu();
        }
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
        getSettings().game.debugFlyCam.setValue(false);
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
