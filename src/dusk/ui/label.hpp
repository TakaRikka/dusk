#pragma once

#include <RmlUi/Core/Element.h>

#include <string_view>

namespace dusk::ui {

enum class LabelStyle {
    Annotation,
    Body,
    Medium,
    Large,
};

Rml::Element* add_label(Rml::Element* parent, std::string_view text, LabelStyle style);
void apply_label_style(Rml::Element* element, LabelStyle style);

}  // namespace dusk::ui
