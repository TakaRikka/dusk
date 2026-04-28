#include "prelaunch_layout.hpp"

#include "element.hpp"
#include "label.hpp"
#include "theme.hpp"

#include <RmlUi/Core.h>

namespace dusk::ui::prelaunch::layout {

void style_document(Rml::ElementDocument* document) {
    using namespace theme;

    set_props(document, {
                            {Rml::PropertyId::Width, rml_percent(100.0f)},
                            {Rml::PropertyId::Height, rml_percent(100.0f)},
                            {Rml::PropertyId::MarginTop, rml_px(0.0f)},
                            {Rml::PropertyId::MarginRight, rml_px(0.0f)},
                            {Rml::PropertyId::MarginBottom, rml_px(0.0f)},
                            {Rml::PropertyId::MarginLeft, rml_px(0.0f)},
                            {Rml::PropertyId::PaddingTop, rml_px(0.0f)},
                            {Rml::PropertyId::PaddingRight, rml_px(0.0f)},
                            {Rml::PropertyId::PaddingBottom, rml_px(0.0f)},
                            {Rml::PropertyId::PaddingLeft, rml_px(0.0f)},
                            {Rml::PropertyId::FontFamily, rml_string("Inter")},
                            {Rml::PropertyId::BackgroundColor, rml_color(Background1)},
                            {Rml::PropertyId::Color, rml_color(Text)},
                        });
}

Rml::Element* add_screen(Rml::ElementDocument* document, ScreenLayout layout) {
    using namespace theme;

    const bool compact = layout == ScreenLayout::CompactSplit;
    return append(document, "div", "prelaunch-screen",
        {
            {Rml::PropertyId::Display, Rml::Style::Display::Flex},
            {Rml::PropertyId::Position, Rml::Style::Position::Absolute},
            {Rml::PropertyId::Left, rml_px(0.0f)},
            {Rml::PropertyId::Top, rml_px(0.0f)},
            {Rml::PropertyId::Right, rml_px(0.0f)},
            {Rml::PropertyId::Bottom, rml_px(0.0f)},
            {Rml::PropertyId::FlexDirection,
                compact ? Rml::Style::FlexDirection::Row : Rml::Style::FlexDirection::Column},
            {Rml::PropertyId::AlignItems, Rml::Style::AlignItems::Center},
            {Rml::PropertyId::JustifyContent, Rml::Style::JustifyContent::Center},
            {Rml::PropertyId::RowGap, rml_dp(compact ? 28.0f : 24.0f)},
            {Rml::PropertyId::ColumnGap, rml_dp(compact ? 28.0f : 24.0f)},
            {Rml::PropertyId::BoxSizing, Rml::Style::BoxSizing::BorderBox},
            {Rml::PropertyId::PaddingTop, rml_dp(compact ? 24.0f : 48.0f)},
            {Rml::PropertyId::PaddingRight, rml_dp(compact ? 24.0f : 28.0f)},
            {Rml::PropertyId::PaddingBottom, rml_dp(compact ? 24.0f : 48.0f)},
            {Rml::PropertyId::PaddingLeft, rml_dp(compact ? 24.0f : 28.0f)},
            {Rml::PropertyId::BackgroundColor, rml_color(Background1)},
        });
}

Rml::Element* add_brand(Rml::Element* parent, std::string_view logoPath, bool compact) {
    auto* brand = append(parent, "div", {},
        {
            {Rml::PropertyId::Display, Rml::Style::Display::Flex},
            {Rml::PropertyId::FlexDirection, Rml::Style::FlexDirection::Column},
            {Rml::PropertyId::AlignItems, Rml::Style::AlignItems::Center},
            {Rml::PropertyId::JustifyContent, Rml::Style::JustifyContent::Center},
            {Rml::PropertyId::RowGap, rml_dp(compact ? 8.0f : 12.0f)},
            {Rml::PropertyId::ColumnGap, rml_dp(compact ? 8.0f : 12.0f)},
            {Rml::PropertyId::Width, compact ? rml_dp(260.0f) : rml_percent(100.0f)},
            {Rml::PropertyId::MaxWidth, compact ? rml_percent(32.0f) : rml_dp(720.0f)},
            {Rml::PropertyId::FlexShrink, rml_number(compact ? 0.0f : 1.0f)},
        });

    auto* subtitle = add_label(brand, "Twilit Realm presents", LabelStyle::Annotation);
    set_props(subtitle, {
                            {Rml::PropertyId::TextAlign, Rml::Style::TextAlign::Center},
                            {Rml::PropertyId::FontSize, rml_dp(compact ? 14.0f : 18.0f)},
                        });

    if (!logoPath.empty()) {
        auto* logo = append(brand, "img");
        logo->SetAttribute("src", std::string(logoPath));
        set_props(logo, {
                            {Rml::PropertyId::Width, rml_dp(compact ? 220.0f : 360.0f)},
                            {Rml::PropertyId::MaxWidth, rml_percent(compact ? 100.0f : 70.0f)},
                            {Rml::PropertyId::Height, Rml::Style::Height::Auto},
                        });
    } else {
        auto* title = add_label(brand, "Dusk", LabelStyle::Large);
        set_props(title, {
                             {Rml::PropertyId::FontSize, rml_dp(compact ? 42.0f : 54.0f)},
                             {Rml::PropertyId::LetterSpacing, rml_dp(compact ? 3.0f : 4.0f)},
                         });
    }
    return brand;
}

Rml::Element* add_heading(Rml::Element* parent, std::string_view title) {
    auto* heading = add_label(parent, title, LabelStyle::Large);
    set_props(heading, {
                           {Rml::PropertyId::Width, rml_percent(100.0f)},
                           {Rml::PropertyId::MaxWidth, rml_dp(840.0f)},
                           {Rml::PropertyId::TextAlign, Rml::Style::TextAlign::Left},
                       });
    return heading;
}

Rml::Element* add_panel(Rml::Element* parent, bool wide, bool compact) {
    using namespace theme;

    return append(parent, "div", {},
        {
            {Rml::PropertyId::Display, Rml::Style::Display::Flex},
            {Rml::PropertyId::FlexDirection, Rml::Style::FlexDirection::Column},
            {Rml::PropertyId::RowGap, rml_dp(12.0f)},
            {Rml::PropertyId::ColumnGap, rml_dp(12.0f)},
            {Rml::PropertyId::Width, rml_dp(wide ? 840.0f : 520.0f)},
            {Rml::PropertyId::MaxWidth, rml_percent(compact ? 62.0f : 100.0f)},
            {Rml::PropertyId::BoxSizing, Rml::Style::BoxSizing::BorderBox},
            {Rml::PropertyId::PaddingTop, rml_dp(compact ? 16.0f : 20.0f)},
            {Rml::PropertyId::PaddingRight, rml_dp(compact ? 16.0f : 20.0f)},
            {Rml::PropertyId::PaddingBottom, rml_dp(compact ? 16.0f : 20.0f)},
            {Rml::PropertyId::PaddingLeft, rml_dp(compact ? 16.0f : 20.0f)},
            {Rml::PropertyId::BorderTopWidth, rml_dp(BorderWidth)},
            {Rml::PropertyId::BorderRightWidth, rml_dp(BorderWidth)},
            {Rml::PropertyId::BorderBottomWidth, rml_dp(BorderWidth)},
            {Rml::PropertyId::BorderLeftWidth, rml_dp(BorderWidth)},
            {Rml::PropertyId::BorderTopLeftRadius, rml_dp(BorderRadiusMedium)},
            {Rml::PropertyId::BorderTopRightRadius, rml_dp(BorderRadiusMedium)},
            {Rml::PropertyId::BorderBottomRightRadius, rml_dp(BorderRadiusMedium)},
            {Rml::PropertyId::BorderBottomLeftRadius, rml_dp(BorderRadiusMedium)},
            {Rml::PropertyId::BorderTopColor, rml_color(ElevatedBorder)},
            {Rml::PropertyId::BorderRightColor, rml_color(ElevatedBorder)},
            {Rml::PropertyId::BorderBottomColor, rml_color(ElevatedBorder)},
            {Rml::PropertyId::BorderLeftColor, rml_color(ElevatedBorder)},
            {Rml::PropertyId::BackgroundColor, rml_color(ElevatedSoft)},
        });
}

}  // namespace dusk::ui::prelaunch::layout
