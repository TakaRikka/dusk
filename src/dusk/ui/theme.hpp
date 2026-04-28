#pragma once

#include <RmlUi/Core/Property.h>

#include <string>

namespace dusk::ui {
namespace theme {

struct Color {
    int r = 255;
    int g = 255;
    int b = 255;
    int a = 255;
};

inline constexpr Color Background1{12, 18, 17, 255};
inline constexpr Color Text{225, 236, 231, 255};
inline constexpr Color TextActive{248, 255, 251, 255};
inline constexpr Color TextDim{160, 191, 182, 255};
inline constexpr Color Primary{69, 184, 170, 255};
inline constexpr Color PrimaryLight{135, 225, 211, 255};
inline constexpr Color Secondary{184, 113, 5, 255};
inline constexpr Color Elevated{160, 191, 182, 44};
inline constexpr Color ElevatedSoft{47, 56, 55, 154};
inline constexpr Color ElevatedBorder{160, 191, 182, 96};
inline constexpr Color Transparent{0, 0, 0, 0};
inline constexpr Color Danger{192, 30, 24, 255};

inline constexpr Color WindowSurface{21, 22, 16, 178};
inline constexpr Color WindowTitleOverlay{217, 217, 217, 26};
inline constexpr Color WindowDivider{146, 135, 91, 255};
inline constexpr Color WindowAccent{194, 164, 45, 255};
inline constexpr Color WindowAccentSoft{204, 184, 119, 255};
inline constexpr Color WindowItemSurface{17, 16, 10, 128};
inline constexpr Color WindowGlyph{224, 219, 200, 255};
inline constexpr float WindowTabBarHeight = 66.0f;

inline constexpr float BorderRadiusSmall = 8.0f;
inline constexpr float BorderRadiusMedium = 12.0f;
inline constexpr float BorderWidth = 2.0f;

}  // namespace theme

static Rml::Property rml_color(theme::Color color, int opacity = -1) {
    if (opacity >= 0) {
        color.a = std::clamp(opacity, 0, 255);
    }
    return Rml::Property(Rml::Colourb(color.r, color.g, color.b, color.a), Rml::Unit::COLOUR);
}

static Rml::Property rml_dp(float value) {
    return Rml::Property(value, Rml::Unit::DP);
}

static Rml::Property rml_percent(float value) {
    return Rml::Property(value, Rml::Unit::PERCENT);
}

static Rml::Property rml_px(float value) {
    return Rml::Property(value, Rml::Unit::PX);
}

static Rml::Property rml_number(float value) {
    return Rml::Property(value, Rml::Unit::NUMBER);
}

static Rml::Property rml_string(std::string_view value) {
    return Rml::Property(Rml::String(value), Rml::Unit::STRING);
}
}  // namespace dusk::ui
