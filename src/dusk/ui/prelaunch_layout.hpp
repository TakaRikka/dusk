#pragma once

#include <RmlUi/Core/Element.h>
#include <RmlUi/Core/ElementDocument.h>

#include <string_view>

namespace dusk::ui::prelaunch::layout {

enum class ScreenLayout {
    Standard,
    CompactSplit,
};

void style_document(Rml::ElementDocument* document);
Rml::Element* add_screen(Rml::ElementDocument* document,
                         ScreenLayout layout = ScreenLayout::Standard);
Rml::Element* add_brand(Rml::Element* parent, std::string_view logoPath, bool compact = false);
Rml::Element* add_heading(Rml::Element* parent, std::string_view title);
Rml::Element* add_panel(Rml::Element* parent, bool wide, bool compact = false);

}  // namespace dusk::ui::prelaunch::layout
