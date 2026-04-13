#include "fmt/format.h"
#include "imgui.h"

#include "ImGuiMenuSpeedrunTimer.hpp"
#include "ImGuiConfig.hpp"
#include "ImGuiConsole.hpp"
#include "dusk/livesplit.h"
#include "dusk/settings.h"

namespace dusk {
    static auto formatTime(uint64_t frames) {
        const uint64_t totalSec = frames / 30;

        return fmt::format(
            FMT_STRING("{:d}:{:02d}:{:02d}.{:02d}"),
            totalSec / 3600,
            (totalSec / 60) % 60,
            totalSec % 60,
            (int)(((f32)(frames % 30) / 30.0f) * 100.0f)
        );
    }

    void ImGuiMenuSpeedrunTimer::draw() {
        if (!getSettings().game.speedrunTimer) return;

        const uint64_t frames = dusk::speedrun::getFrameCount();

        if (ImGui::BeginMenu("Timer##speedrun_timer")) {
            ImGui::TextUnformatted(formatTime(frames).c_str());

            ImGui::Separator();

            if (ImGui::MenuItem("Reset")) {
                dusk::speedrun::reset();
            }

            config::ImGuiCheckbox("Show Overlay", getSettings().game.speedrunTimerOverlay);

            bool prevLiveSplit = getSettings().game.liveSplitEnabled;
            config::ImGuiCheckbox("LiveSplit Connection", getSettings().game.liveSplitEnabled);

            if (ImGui::IsItemHovered()) {
                ImGui::SetTooltip("Connect to LiveSplit server on localhost:16834.");
            }

            if ((bool)getSettings().game.liveSplitEnabled != prevLiveSplit) {
                if (getSettings().game.liveSplitEnabled) {
                    dusk::speedrun::connectLiveSplit();
                } else {
                    dusk::speedrun::disconnectLiveSplit();
                    DuskToast("LiveSplit disconnected", 3.f);
                }
            }

            ImGui::EndMenu();
        }

    }

    void ImGuiMenuSpeedrunTimer::drawOverlay() {
        if (!getSettings().game.speedrunTimer || !getSettings().game.speedrunTimerOverlay) {
            return;
        }
        
        const uint64_t frames = dusk::speedrun::getFrameCount();
        const ImGuiViewport* viewport = ImGui::GetMainViewport();

        const float padding = 10.f;

        ImGui::SetNextWindowPos(
            ImVec2(
                viewport->WorkPos.x + viewport->WorkSize.x - padding,
                viewport->WorkPos.y + viewport->WorkSize.y - padding
            ),
            ImGuiCond_Always, ImVec2(1.f, 1.f)
        );

        ImGui::SetNextWindowBgAlpha(0.65f);

        const float fixedWidth = ImGui::CalcTextSize("9:59:59.99").x;

        ImGui::SetNextWindowContentSize(ImVec2(fixedWidth, 0.f));

        if (
            ImGui::Begin("##speedrun_overlay", nullptr,
                ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoInputs |
                ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove |
                ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing |
                ImGuiWindowFlags_NoNav
            )
        ) {
            ImGui::TextUnformatted(formatTime(frames).c_str());
        }
        ImGui::End();
    }
}
