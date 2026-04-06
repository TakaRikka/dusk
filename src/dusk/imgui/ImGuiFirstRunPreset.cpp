#include "ImGuiFirstRunPreset.hpp"

#include "imgui.h"
#include "ImGuiConsole.hpp"
#include "ImGuiEngine.hpp"
#include "dusk/settings.h"
#include "dusk/config.hpp"

namespace dusk {

static void ApplyPresetVanilla() {
    auto& s = getSettings();
    // todo: lock aspect ratio
}

static void ApplyPresetDefault() {
    auto& s = getSettings();
    // todo: unlock aspect ratio
    // todo: instant saving
    s.game.hideTvSettingsScreen.setValue(true);
}

static void ApplyPresetQoL() {
    auto& s = getSettings();
    // todo: unlock aspect ratio
    // todo: instant saving
    // todo: more save files
    // todo: autosave
    s.game.enableQuickTransform.setValue(true);
    s.game.hideTvSettingsScreen.setValue(true);
    s.game.noReturnRupees.setValue(true);
    s.game.disableRupeeCutscenes.setValue(true);
    s.game.noSwordRecoil.setValue(true);
    s.game.fastClimbing.setValue(true);
    s.game.noMissClimbing.setValue(true);
    s.game.fastTears.setValue(true);
    s.game.noLowHpSound.setValue(true);
    s.game.midnasLamentNonStop.setValue(true);
    s.game.enableFastIronBoots.setValue(true);
    s.game.canTransformAnywhere.setValue(true);
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
        if (ImGui::Button("Vanilla##btn", ImVec2(ImGui::GetContentRegionAvail().x, 80.0f * ImGuiScale()))) {
            chosen = 0;
        }

        ImGui::TableSetColumnIndex(2);
        if (ImGui::Button("Default##btn", ImVec2(ImGui::GetContentRegionAvail().x, 80.0f * ImGuiScale()))) {
            chosen = 1;
        }

        ImGui::TableSetColumnIndex(4);
        if (ImGui::Button("Quality of Life##btn", ImVec2(ImGui::GetContentRegionAvail().x, 80.0f * ImGuiScale()))) {
            chosen = 2;
        }

        ImGui::PopFont();

        ImGui::TableNextRow();

        ImGui::TableSetColumnIndex(0);
        ImGui::Spacing();
        ImGui::TextWrapped("All enhancements disabled. Plays closest to the original game; good for speedrunning or simple nostalgia!");

        ImGui::TableSetColumnIndex(2);
        ImGui::Spacing();
        ImGui::TextWrapped("Some enhancements enabled, maintaining a vanilla feel. A good starting point for most players!");

        ImGui::TableSetColumnIndex(4);
        ImGui::Spacing();
        ImGui::TextWrapped("Many quality of life enhancements enabled. Good for seasoned players!");

        ImGui::EndTable();
    }

    if (chosen >= 0) {
        if (chosen == 0) ApplyPresetVanilla();
        if (chosen == 1) ApplyPresetDefault();
        if (chosen == 2) ApplyPresetQoL();
        config::Save();
        m_done = true;
        ImGui::CloseCurrentPopup();
    }

    ImGui::EndPopup();
}

}  // namespace dusk
