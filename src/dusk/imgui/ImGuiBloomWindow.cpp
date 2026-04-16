#include <algorithm>

#include "imgui.h"

#include "ImGuiBloomWindow.hpp"
#include "ImGuiMenuTools.hpp"
#include "m_Do/m_Do_graphic.h"

namespace dusk {
namespace {
struct BloomOverride {
    bool enabled = false;
    bool bloomEnabled = true;
    int mode = 0;
    int point = 128;
    int blureSize = 64;
    int blureRatio = 128;
    GXColor blendColor = {255, 255, 255, 255};
    GXColor monoColor = {0, 0, 0, 0};
};

BloomOverride s_bloomOverride;

void SyncFromCurrentBloom() {
    mDoGph_gInf_c::bloom_c* bloom = mDoGph_gInf_c::getBloom();
    s_bloomOverride.bloomEnabled = bloom->getEnable() != 0;
    s_bloomOverride.mode = bloom->mMode;
    s_bloomOverride.point = bloom->getPoint();
    s_bloomOverride.blureSize = bloom->getBlureSize();
    s_bloomOverride.blureRatio = bloom->getBlureRatio();
    s_bloomOverride.blendColor = *bloom->getBlendColor();
    s_bloomOverride.monoColor = *bloom->getMonoColor();
}

u8 ClampToByte(int value) {
    return static_cast<u8>(std::clamp(value, 0, 255));
}

void DrawColorEdit(const char* label, GXColor& color) {
    float colorValue[4] = {
        color.r / 255.0f,
        color.g / 255.0f,
        color.b / 255.0f,
        color.a / 255.0f,
    };
    if (ImGui::ColorEdit4(label, colorValue, ImGuiColorEditFlags_Uint8)) {
        color.r = ClampToByte(static_cast<int>(colorValue[0] * 255.0f + 0.5f));
        color.g = ClampToByte(static_cast<int>(colorValue[1] * 255.0f + 0.5f));
        color.b = ClampToByte(static_cast<int>(colorValue[2] * 255.0f + 0.5f));
        color.a = ClampToByte(static_cast<int>(colorValue[3] * 255.0f + 0.5f));
    }
}
}  // namespace

void ApplyBloomOverride() {
    if (!s_bloomOverride.enabled) {
        return;
    }

    mDoGph_gInf_c::bloom_c* bloom = mDoGph_gInf_c::getBloom();
    bloom->setEnable(s_bloomOverride.bloomEnabled ? 1 : 0);
    bloom->setMode(static_cast<u8>(std::clamp(s_bloomOverride.mode, 0, 1)));
    bloom->setPoint(ClampToByte(s_bloomOverride.point));
    bloom->setBlureSize(ClampToByte(s_bloomOverride.blureSize));
    bloom->setBlureRatio(ClampToByte(s_bloomOverride.blureRatio));
    bloom->setBlendColor(s_bloomOverride.blendColor);
    bloom->setMonoColor(s_bloomOverride.monoColor);
}

void DrawBloomWindow(bool& open) {
    if (!open) {
        return;
    }

    if (!ImGui::Begin("Bloom", &open)) {
        ImGui::End();
        return;
    }

    bool copyCurrent = false;
    if (ImGui::Checkbox("Override", &s_bloomOverride.enabled) && s_bloomOverride.enabled) {
        copyCurrent = true;
    }

    ImGui::SameLine();
    if (ImGui::Button("Sync with current")) {
        copyCurrent = true;
    }

    if (copyCurrent) {
        SyncFromCurrentBloom();
    }

    ImGui::SeparatorText("Values");
    ImGui::Checkbox("Enabled", &s_bloomOverride.bloomEnabled);
    ImGui::SliderInt("Mode", &s_bloomOverride.mode, 0, 1);
    ImGui::SliderInt("Threshold", &s_bloomOverride.point, 0, 255);
    ImGui::SliderInt("Blur Size", &s_bloomOverride.blureSize, 0, 255);
    ImGui::SliderInt("Blur Ratio", &s_bloomOverride.blureRatio, 0, 255);
    DrawColorEdit("Blend Color", s_bloomOverride.blendColor);
    DrawColorEdit("Mono Color", s_bloomOverride.monoColor);

    ApplyBloomOverride();

    ImGui::SeparatorText("Current");
    mDoGph_gInf_c::bloom_c* bloom = mDoGph_gInf_c::getBloom();
    GXColor blendColor = *bloom->getBlendColor();
    GXColor monoColor = *bloom->getMonoColor();
    ImGui::Text("Enabled: %d", bloom->getEnable());
    ImGui::Text("Mode: %d", bloom->mMode);
    ImGui::Text("Threshold: %u", static_cast<unsigned>(bloom->getPoint()));
    ImGui::Text("Blur Size: %u", static_cast<unsigned>(bloom->getBlureSize()));
    ImGui::Text("Blur Ratio: %u", static_cast<unsigned>(bloom->getBlureRatio()));
    ImGui::Text("Blend RGBA: %u, %u, %u, %u", static_cast<unsigned>(blendColor.r),
                static_cast<unsigned>(blendColor.g), static_cast<unsigned>(blendColor.b),
                static_cast<unsigned>(blendColor.a));
    ImGui::Text("Mono RGBA: %u, %u, %u, %u", static_cast<unsigned>(monoColor.r),
                static_cast<unsigned>(monoColor.g), static_cast<unsigned>(monoColor.b),
                static_cast<unsigned>(monoColor.a));
    ImGui::End();
}

void ImGuiMenuTools::ShowBloomWindow() {
    DrawBloomWindow(m_showBloomWindow);
}
}  // namespace dusk
