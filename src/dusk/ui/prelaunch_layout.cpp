#include "prelaunch_layout.hpp"

#include "element.hpp"
#include "label.hpp"
#include "theme.hpp"

namespace dusk::ui::prelaunch::layout {

void style_document(Rml::ElementDocument* document) {
    using namespace theme;

    set_props(document, {
                            {"width", "100%"},
                            {"height", "100%"},
                            {"margin", "0"},
                            {"padding", "0"},
                            {"font-family", "Inter"},
                            {"background-color", rgba(Background1)},
                            {"color", rgba(Text)},
                        });
}

Rml::Element* add_screen(Rml::ElementDocument* document, ScreenLayout layout) {
    using namespace theme;

    auto* screen = append(document, "div", "prelaunch-screen");
    set_props(screen,
              {
                  {"display", "flex"},
                  {"position", "absolute"},
                  {"left", "0"},
                  {"top", "0"},
                  {"right", "0"},
                  {"bottom", "0"},
                  {"flex-direction", layout == ScreenLayout::CompactSplit ? "row" : "column"},
                  {"align-items", "center"},
                  {"justify-content", "center"},
                  {"gap", layout == ScreenLayout::CompactSplit ? "28dp" : "24dp"},
                  {"box-sizing", "border-box"},
                  {"padding", layout == ScreenLayout::CompactSplit ? "24dp" : "48dp 28dp"},
                  {"background-color", rgba(Background1)},
              });
    return screen;
}

Rml::Element* add_brand(Rml::Element* parent, std::string_view logoPath, bool compact) {
    auto* brand = append(parent, "div");
    set_props(brand, {
                         {"display", "flex"},
                         {"flex-direction", "column"},
                         {"align-items", "center"},
                         {"justify-content", "center"},
                         {"gap", compact ? "8dp" : "12dp"},
                         {"width", compact ? "260dp" : "100%"},
                         {"max-width", compact ? "32%" : "720dp"},
                         {"flex-shrink", compact ? "0" : "1"},
                     });

    auto* subtitle = add_label(brand, "Twilit Realm presents", LabelStyle::Annotation);
    set_props(subtitle, {
                            {"text-align", "center"},
                            {"font-size", compact ? "14dp" : "18dp"},
                        });

    if (!logoPath.empty()) {
        auto* logo = append(brand, "img");
        logo->SetAttribute("src", std::string(logoPath));
        set_props(logo, {
                            {"width", compact ? "220dp" : "360dp"},
                            {"max-width", compact ? "100%" : "70%"},
                            {"height", "auto"},
                        });
    } else {
        auto* title = add_label(brand, "Dusk", LabelStyle::Large);
        set_props(title, {
                             {"font-size", compact ? "42dp" : "54dp"},
                             {"letter-spacing", compact ? "3dp" : "4dp"},
                         });
    }
    return brand;
}

Rml::Element* add_heading(Rml::Element* parent, std::string_view title) {
    auto* heading = add_label(parent, title, LabelStyle::Large);
    set_props(heading, {
                           {"width", "100%"},
                           {"max-width", "840dp"},
                           {"text-align", "left"},
                       });
    return heading;
}

Rml::Element* add_panel(Rml::Element* parent, bool wide, bool compact) {
    using namespace theme;

    auto* panel = append(parent, "div");
    set_props(panel, {
                         {"display", "flex"},
                         {"flex-direction", "column"},
                         {"gap", "12dp"},
                         {"width", wide ? "840dp" : "520dp"},
                         {"max-width", compact ? "62%" : "100%"},
                         {"box-sizing", "border-box"},
                         {"padding", compact ? "16dp" : "20dp"},
                         {"border-width", dp(BorderWidth)},
                         {"border-radius", dp(BorderRadiusMedium)},
                         {"border-color", rgba(ElevatedBorder, ElevatedBorder.a)},
                         {"background-color", rgba(ElevatedSoft, ElevatedSoft.a)},
                     });
    return panel;
}

}  // namespace dusk::ui::prelaunch::layout
