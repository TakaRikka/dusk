#include "control_surface.hpp"

#include <RmlUi/Core/Element.h>

namespace dusk::ui {

ControlSurfaceStyle control_surface_style(ControlSurfaceTone tone) {
    switch (tone) {
    case ControlSurfaceTone::Primary:
        return {
            .accent = theme::Primary,
            .inactiveBorderOpacity = 112,
            .inactiveBackgroundOpacity = 28,
            .activeBorderOpacity = 255,
            .activeBackgroundOpacity = 116,
        };
    case ControlSurfaceTone::Secondary:
        return {
            .accent = theme::Secondary,
            .inactiveBorderOpacity = 112,
            .inactiveBackgroundOpacity = 28,
            .activeBorderOpacity = 255,
            .activeBackgroundOpacity = 116,
        };
    case ControlSurfaceTone::Quiet:
    default:
        return {
            .accent = theme::Elevated,
            .inactiveBorderOpacity = 86,
            .inactiveBackgroundOpacity = 0,
            .activeBorderOpacity = 150,
            .activeBackgroundOpacity = 68,
        };
    }
}

void apply_control_surface_style(Rml::Element* element, const ControlSurfaceStyle& style,
                                 bool active) {
    if (element == nullptr) {
        return;
    }

    element->SetProperty("border-color",
                         active ? theme::rgba(style.accent, style.activeBorderOpacity) :
                                  theme::rgba(theme::ElevatedBorder,
                                              style.inactiveBorderOpacity));
    element->SetProperty("background-color",
                         theme::rgba(style.accent,
                                     active ? style.activeBackgroundOpacity :
                                              style.inactiveBackgroundOpacity));
}

}  // namespace dusk::ui
