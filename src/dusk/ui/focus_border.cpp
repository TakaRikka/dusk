#include "focus_border.hpp"

#include "element.hpp"
#include "theme.hpp"

namespace dusk::ui {

Rml::Element* add_focus_border(Rml::Element* parent, float radius) {
    using namespace theme;

    auto* border = append(parent, "div");
    set_props(border, {
                          {"position", "absolute"},
                          {"pointer-events", "none"},
                          {"left", dp(-(BorderWidth * 3.0f))},
                          {"top", dp(-(BorderWidth * 3.0f))},
                          {"right", dp(-(BorderWidth * 3.0f))},
                          {"bottom", dp(-(BorderWidth * 3.0f))},
                          {"border-width", dp(BorderWidth * 2.0f)},
                          {"border-radius", dp(radius + BorderWidth * 4.0f)},
                          {"border-color", rgba(PrimaryLight, 0)},
                      });
    return border;
}

void set_focus_border_visible(Rml::Element* parent, bool visible) {
    if (parent == nullptr || parent->GetNumChildren() == 0) {
        return;
    }

    parent->GetChild(0)->SetProperty("border-color", visible ?
                                                         theme::rgba(theme::PrimaryLight, 255) :
                                                         theme::rgba(theme::PrimaryLight, 0));
}

}  // namespace dusk::ui
