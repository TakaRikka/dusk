#include "label.hpp"

#include "element.hpp"
#include "theme.hpp"

namespace dusk::ui {

void apply_label_style(Rml::Element* element, LabelStyle style) {
    using namespace theme;

    switch (style) {
    case LabelStyle::Annotation:
        set_props(element, {
                               {Rml::PropertyId::FontSize, rml_dp(18.0f)},
                               {Rml::PropertyId::LetterSpacing, rml_dp(2.0f)},
                               {Rml::PropertyId::FontWeight, Rml::Style::FontWeight::Normal},
                               {Rml::PropertyId::Color, rml_color(TextDim)},
                           });
        break;
    case LabelStyle::Body:
        set_props(element, {
                               {Rml::PropertyId::FontSize, rml_dp(20.0f)},
                               {Rml::PropertyId::LetterSpacing, rml_px(0.0f)},
                               {Rml::PropertyId::FontWeight, Rml::Style::FontWeight::Normal},
                               {Rml::PropertyId::Color, rml_color(Text)},
                           });
        break;
    case LabelStyle::Medium:
        set_props(element, {
                               {Rml::PropertyId::FontSize, rml_dp(28.0f)},
                               {Rml::PropertyId::LetterSpacing, rml_dp(3.0f)},
                               {Rml::PropertyId::FontWeight, Rml::Style::FontWeight::Bold},
                               {Rml::PropertyId::Color, rml_color(Text)},
                           });
        break;
    case LabelStyle::Large:
        set_props(element, {
                               {Rml::PropertyId::FontSize, rml_dp(36.0f)},
                               {Rml::PropertyId::LetterSpacing, rml_dp(4.0f)},
                               {Rml::PropertyId::FontWeight, Rml::Style::FontWeight::Bold},
                               {Rml::PropertyId::Color, rml_color(Text)},
                           });
        break;
    }
}

Rml::Element* add_label(Rml::Element* parent, std::string_view text, LabelStyle style) {
    Rml::Element* label = append_text(parent, "div", text);
    apply_label_style(label, style);
    return label;
}

}  // namespace dusk::ui
