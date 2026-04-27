#pragma once

#include <RmlUi/Core/Element.h>

namespace dusk::ui {

Rml::Element* add_focus_border(Rml::Element* parent, float radius);
void set_focus_border_visible(Rml::Element* parent, bool visible);

}  // namespace dusk::ui
