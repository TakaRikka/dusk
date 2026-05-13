#include "imgui.h"

#include "ImGuiEnhancedLightingWindow.hpp"
#include "ImGuiMenuTools.hpp"
#include "dusk/config.hpp"
#include "dusk/settings.h"

#include <aurora/gfx.h>

namespace dusk {

static void ApplyFromSettings() {
    const auto& g = getSettings().game;
    aurora_set_enhanced_lighting_state({
        g.enhancedLighting.getValue(),
        g.enableSpecularLighting.getValue(),
        g.enableRimLighting.getValue(),
        g.specularIntensity.getValue(),
        g.rimIntensity.getValue(),
        g.ambientLightMultiplier.getValue(),
        g.diffuseLightMultiplier.getValue(),
    });
}

void DrawEnhancedLightingWindow(bool& open) {
    if (!open) {
        return;
    }

    if (!ImGui::Begin("Enhanced Lighting", &open)) {
        ImGui::End();
        return;
    }

    auto& g = getSettings().game;
    bool changed = false;

    bool enabled = g.enhancedLighting.getValue();
    if (ImGui::Checkbox("Enhanced Lighting", &enabled)) {
        g.enhancedLighting.setValue(enabled);
        changed = true;
    }
    if (ImGui::IsItemHovered()) {
        ImGui::SetTooltip("Enables per-pixel lighting with Blinn-Phong shading.");
    }

    if (!enabled) {
        ImGui::BeginDisabled();
    }

    bool enableSpecular = g.enableSpecularLighting.getValue();
    if (ImGui::Checkbox("Specular Highlights", &enableSpecular)) {
        g.enableSpecularLighting.setValue(enableSpecular);
        changed = true;
    }

    bool enableRim = g.enableRimLighting.getValue();
    if (ImGui::Checkbox("Rim Lighting", &enableRim)) {
        g.enableRimLighting.setValue(enableRim);
        changed = true;
    }

    float specularIntensity = g.specularIntensity.getValue();
    if (ImGui::SliderFloat("Specular Intensity", &specularIntensity, 0.0f, 1.0f, "%.2f")) {
        g.specularIntensity.setValue(specularIntensity);
        changed = true;
    }

    float rimIntensity = g.rimIntensity.getValue();
    if (ImGui::SliderFloat("Rim Intensity", &rimIntensity, 0.0f, 0.5f, "%.3f")) {
        g.rimIntensity.setValue(rimIntensity);
        changed = true;
    }

    float ambientMultiplier = g.ambientLightMultiplier.getValue();
    if (ImGui::SliderFloat("Ambient Multiplier", &ambientMultiplier, 0.0f, 3.0f, "%.2f")) {
        g.ambientLightMultiplier.setValue(ambientMultiplier);
        changed = true;
    }

    float diffuseMultiplier = g.diffuseLightMultiplier.getValue();
    if (ImGui::SliderFloat("Diffuse Multiplier", &diffuseMultiplier, 0.0f, 3.0f, "%.2f")) {
        g.diffuseLightMultiplier.setValue(diffuseMultiplier);
        changed = true;
    }

    if (!enabled) {
        ImGui::EndDisabled();
    }

    if (changed) {
        ApplyFromSettings();
        config::Save();
    }

    ImGui::End();
}

void ImGuiMenuTools::ShowEnhancedLightingWindow() {
    DrawEnhancedLightingWindow(m_showEnhancedLightingWindow);
}

}  // namespace dusk
