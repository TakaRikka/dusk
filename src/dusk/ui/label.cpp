#include "label.hpp"

#include "element.hpp"
#include "theme.hpp"

namespace dusk::ui {

void apply_label_style(Rml::Element* element, LabelStyle style) {
    using namespace theme;

    switch (style) {
    case LabelStyle::Annotation:
        set_props(element, {{"font-size", "18dp"},
                            {"letter-spacing", "2dp"},
                            {"font-weight", "400"},
                            {"color", rgba(TextDim)}});
        break;
    case LabelStyle::Body:
        set_props(element, {{"font-size", "20dp"},
                            {"letter-spacing", "0"},
                            {"font-weight", "400"},
                            {"color", rgba(Text)}});
        break;
    case LabelStyle::Medium:
        set_props(element, {{"font-size", "28dp"},
                            {"letter-spacing", "3dp"},
                            {"font-weight", "700"},
                            {"color", rgba(Text)}});
        break;
    case LabelStyle::Large:
        set_props(element, {{"font-size", "36dp"},
                            {"letter-spacing", "4dp"},
                            {"font-weight", "700"},
                            {"color", rgba(Text)}});
        break;
    }
}

Rml::Element* add_label(Rml::Element* parent, std::string_view text, LabelStyle style) {
    Rml::Element* label = append_text(parent, "div", text);
    apply_label_style(label, style);
    return label;
}

}  // namespace dusk::ui
