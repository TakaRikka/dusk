#include "focus_border.hpp"

#include "element.hpp"
#include "theme.hpp"

#include <RmlUi/Core.h>

namespace dusk::ui {

Rml::Element* add_focus_border(Rml::Element* parent, float radius) {
    using namespace theme;

    const auto borderColor = rml_color(PrimaryLight, 0);
    return append(parent, "div", {},
        {
            {Rml::PropertyId::Position, Rml::Style::Position::Absolute},
            {Rml::PropertyId::PointerEvents, Rml::Style::PointerEvents::None},
            {Rml::PropertyId::Left, rml_dp(-(BorderWidth * 3.0f))},
            {Rml::PropertyId::Top, rml_dp(-(BorderWidth * 3.0f))},
            {Rml::PropertyId::Right, rml_dp(-(BorderWidth * 3.0f))},
            {Rml::PropertyId::Bottom, rml_dp(-(BorderWidth * 3.0f))},
            {Rml::PropertyId::BorderTopWidth, rml_dp(BorderWidth * 2.0f)},
            {Rml::PropertyId::BorderRightWidth, rml_dp(BorderWidth * 2.0f)},
            {Rml::PropertyId::BorderBottomWidth, rml_dp(BorderWidth * 2.0f)},
            {Rml::PropertyId::BorderLeftWidth, rml_dp(BorderWidth * 2.0f)},
            {Rml::PropertyId::BorderTopLeftRadius, rml_dp(radius + BorderWidth * 4.0f)},
            {Rml::PropertyId::BorderTopRightRadius, rml_dp(radius + BorderWidth * 4.0f)},
            {Rml::PropertyId::BorderBottomRightRadius, rml_dp(radius + BorderWidth * 4.0f)},
            {Rml::PropertyId::BorderBottomLeftRadius, rml_dp(radius + BorderWidth * 4.0f)},
            {Rml::PropertyId::BorderTopColor, borderColor},
            {Rml::PropertyId::BorderRightColor, borderColor},
            {Rml::PropertyId::BorderBottomColor, borderColor},
            {Rml::PropertyId::BorderLeftColor, borderColor},
        });
}

void set_focus_border_visible(Rml::Element* parent, bool visible) {
    if (parent == nullptr || parent->GetNumChildren() == 0) {
        return;
    }

    const auto borderColor = rml_color(theme::PrimaryLight, visible ? 255 : 0);
    set_props(parent->GetChild(0), {
                                       {Rml::PropertyId::BorderTopColor, borderColor},
                                       {Rml::PropertyId::BorderRightColor, borderColor},
                                       {Rml::PropertyId::BorderBottomColor, borderColor},
                                       {Rml::PropertyId::BorderLeftColor, borderColor},
                                   });
}

}  // namespace dusk::ui
