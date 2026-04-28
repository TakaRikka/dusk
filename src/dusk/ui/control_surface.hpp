#pragma once

#include "theme.hpp"

namespace Rml {
class Element;
}

namespace dusk::ui {
enum class ControlSurfaceTone {
    Primary,
    Secondary,
    Quiet,
    Window,
};

struct ControlSurfaceStyle {
    theme::Color accent = theme::Primary;
    int inactiveBorderOpacity = 86;
    int inactiveBackgroundOpacity = 0;
    int activeBorderOpacity = 150;
    int activeBackgroundOpacity = 68;
};

ControlSurfaceStyle control_surface_style(ControlSurfaceTone tone);
void apply_control_surface_style(
    Rml::Element* element, const ControlSurfaceStyle& style, bool active);
}  // namespace dusk::ui
