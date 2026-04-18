#pragma once

namespace dusk {
struct MeterColorsOverride {
    GXColor lanternCustomTop = {230, 170, 0, 255};
    GXColor lanternCustomBottom = {255, 255, 140, 255};
    GXColor oxygen1CustomBottom = {200, 200, 255, 255};
    GXColor oxygen1CustomTop = {80, 180, 255, 255};
    GXColor oxygen2CustomBottom = {255, 100, 100, 255};
    GXColor oxygen2CustomTop = {255, 10, 10, 255};
};

extern MeterColorsOverride s_meterColorsOverride;
void DrawMeterColorsWindow(bool& open);
void DrawBloomWindow(bool& open);
void ApplyBloomOverride();
}  // namespace dusk
