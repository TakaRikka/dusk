#pragma once

#include <RmlUi/Core/Element.h>

#include <initializer_list>
#include <string_view>

namespace dusk::ui {

std::string escape(std::string_view text);
Rml::Element* append(Rml::Element* parent, std::string_view tag, std::string_view id = {});
Rml::Element* append_text(Rml::Element* parent, std::string_view tag, std::string_view text,
                          std::string_view id = {});
void set_text(Rml::Element* element, std::string_view text);
void set_props(Rml::Element* element,
               std::initializer_list<std::pair<std::string_view, std::string_view> > properties);

}  // namespace dusk::ui
