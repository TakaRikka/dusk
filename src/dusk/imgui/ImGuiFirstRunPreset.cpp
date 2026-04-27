#include "ImGuiFirstRunPreset.hpp"

#include "imgui.h"
#include "ImGuiConsole.hpp"
#include "ImGuiEngine.hpp"
#include "dusk/settings.h"
#include "dusk/config.hpp"
#include <dusk/dusk.h>

namespace dusk {

static void ApplyPresetClassic() {
    auto& s = getSettings();
    s.video.lockAspectRatio.setValue(true);
    s.game.bloomMode.setValue(BloomMode::Classic);
    AuroraSetViewportPolicy(AURORA_VIEWPORT_FIT);
}

static void ApplyPresetHD() {
    auto& s = getSettings();
    s.game.bloomMode.setValue(BloomMode::Classic);
    s.game.hideTvSettingsScreen.setValue(true);
    s.game.skipWarningScreen.setValue(true);
    s.game.noReturnRupees.setValue(true);
    s.game.disableRupeeCutscenes.setValue(true);
    s.game.noSwordRecoil.setValue(true);
    s.game.fastClimbing.setValue(true);
    s.game.noMissClimbing.setValue(true);
    s.game.fastTears.setValue(true);
    s.game.biggerWallets.setValue(true);
    s.game.invertCameraXAxis.setValue(true);
    s.game.freeCamera.setValue(true);
}

static void ApplyPresetDusk() {
    ApplyPresetHD();

    auto& s = getSettings();
    s.game.enableAchievementNotifications.setValue(true);
    s.game.enableQuickTransform.setValue(true);
    s.game.instantSaves.setValue(true);
    s.game.midnasLamentNonStop.setValue(true);
    s.game.enableFrameInterpolation.setValue(true);
    s.game.sunsSong.setValue(true);
    s.game.bloomMode.setValue(BloomMode::Dusk);
}

// =========================================================================

void ImGuiFirstRunPreset::draw() {
    const char* modalTitle = "Welcome to Dusk!";

    if (m_done) return;

    if (!m_opened) {
        ImGui::OpenPopup(modalTitle);
        m_opened = true;
    }

    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->GetCenter(), ImGuiCond_Always, ImVec2(0.5f, 0.5f));
    ImGui::SetNextWindowSize(ImVec2(800.0f * ImGuiScale(), 0.0f), ImGuiCond_Always);

    if (!ImGui::BeginPopupModal(modalTitle, nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove)) {
        // force the user to actually pick one, and not just hit escape to skip the dialog
        m_opened = false;
        return;
    }

    ImGui::TextWrapped("Choose a preset to get started. You can change any setting later from the Enhancements menu.");
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    int chosen = -1;

    if (ImGui::BeginTable("##presets", 5, ImGuiTableFlags_None)) {
        ImGui::TableSetupColumn(nullptr, ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupColumn(nullptr, ImGuiTableColumnFlags_WidthFixed, 16.0f * ImGuiScale());
        ImGui::TableSetupColumn(nullptr, ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupColumn(nullptr, ImGuiTableColumnFlags_WidthFixed, 16.0f * ImGuiScale());
        ImGui::TableSetupColumn(nullptr, ImGuiTableColumnFlags_WidthStretch);

        ImGui::TableNextRow();

        ImGui::PushFont(ImGuiEngine::fontLarge);

        ImGui::TableSetColumnIndex(0);
        if (ImGui::Button("Classic##btn", ImVec2(ImGui::GetContentRegionAvail().x, 80.0f * ImGuiScale()))) {
            chosen = 0;
        }

        ImGui::TableSetColumnIndex(2);
        if (ImGui::Button("HD##btn", ImVec2(ImGui::GetContentRegionAvail().x, 80.0f * ImGuiScale()))) {
            chosen = 1;
        }

        ImGui::TableSetColumnIndex(4);
        if (ImGui::Button("Dusk##btn", ImVec2(ImGui::GetContentRegionAvail().x, 80.0f * ImGuiScale())))
        {
            chosen = 2;
        }

        ImGui::PopFont();

        ImGui::TableNextRow();

        ImGui::TableSetColumnIndex(0);
        ImGui::Spacing();
        ImGui::TextWrapped("All enhancements disabled to match the GameCube version. Good for speedrunning or simple nostalgia!");

        ImGui::TableSetColumnIndex(2);
        ImGui::Spacing();
        ImGui::TextWrapped("Some enhancements enabled to match the HD version. A good starting point for most players!");

        ImGui::TableSetColumnIndex(4);
        ImGui::Spacing();
        ImGui::TextWrapped("More enhancements enabled than the HD preset. Veteran players will appreciate the additional tweaks!");

        ImGui::EndTable();
    }

    if (chosen >= 0) {
        if (chosen == 0) ApplyPresetClassic();
        if (chosen == 1) ApplyPresetHD();
        if (chosen == 2) ApplyPresetDusk();

        getSettings().backend.wasPresetChosen.setValue(true);
        config::Save();

        m_done = true;

        ImGui::CloseCurrentPopup();
    }

    ImGui::EndPopup();
}

}  // namespace dusk
