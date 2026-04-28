#include "game_option.hpp"

#include "control_surface.hpp"
#include "element.hpp"
#include "focus_border.hpp"
#include "label.hpp"
#include "theme.hpp"

#include <RmlUi/Core.h>

#include <utility>

namespace dusk::ui {

GameOption::GameOption(Rml::Element* parent, std::string_view id, std::string_view title,
    std::string_view value, std::string_view detail, std::function<void()> pressedCallback)
    : m_pressedCallback(std::move(pressedCallback)) {
    using namespace theme;

    m_element = append(parent, "button", id,
        {
            {Rml::PropertyId::Display, Rml::Style::Display::Flex},
            {Rml::PropertyId::Position, Rml::Style::Position::Relative},
            {Rml::PropertyId::FlexDirection, Rml::Style::FlexDirection::Row},
            {Rml::PropertyId::AlignItems, Rml::Style::AlignItems::Center},
            {Rml::PropertyId::JustifyContent, Rml::Style::JustifyContent::SpaceBetween},
            {Rml::PropertyId::BoxSizing, Rml::Style::BoxSizing::BorderBox},
            {Rml::PropertyId::RowGap, rml_dp(16.0f)},
            {Rml::PropertyId::ColumnGap, rml_dp(16.0f)},
            {Rml::PropertyId::Width, rml_percent(100.0f)},
            {Rml::PropertyId::PaddingTop, rml_dp(16.0f)},
            {Rml::PropertyId::PaddingRight, rml_dp(16.0f)},
            {Rml::PropertyId::PaddingBottom, rml_dp(16.0f)},
            {Rml::PropertyId::PaddingLeft, rml_dp(16.0f)},
            {Rml::PropertyId::BorderTopWidth, rml_dp(BorderWidth)},
            {Rml::PropertyId::BorderRightWidth, rml_dp(BorderWidth)},
            {Rml::PropertyId::BorderBottomWidth, rml_dp(BorderWidth)},
            {Rml::PropertyId::BorderLeftWidth, rml_dp(BorderWidth)},
            {Rml::PropertyId::BorderTopLeftRadius, rml_dp(BorderRadiusSmall)},
            {Rml::PropertyId::BorderTopRightRadius, rml_dp(BorderRadiusSmall)},
            {Rml::PropertyId::BorderBottomRightRadius, rml_dp(BorderRadiusSmall)},
            {Rml::PropertyId::BorderBottomLeftRadius, rml_dp(BorderRadiusSmall)},
            {Rml::PropertyId::BackgroundColor, rml_color(Transparent)},
            {Rml::PropertyId::BorderTopColor, rml_color(ElevatedBorder, 0)},
            {Rml::PropertyId::BorderRightColor, rml_color(ElevatedBorder, 0)},
            {Rml::PropertyId::BorderBottomColor, rml_color(ElevatedBorder, 0)},
            {Rml::PropertyId::BorderLeftColor, rml_color(ElevatedBorder, 0)},
            {Rml::PropertyId::Color, rml_color(TextDim)},
            {Rml::PropertyId::Cursor, rml_string("pointer")},
            {Rml::PropertyId::TabIndex, Rml::Style::TabIndex::Auto},
            {Rml::PropertyId::NavUp, Rml::Style::Nav::Auto},
            {Rml::PropertyId::NavDown, Rml::Style::Nav::Auto},
            {Rml::PropertyId::NavLeft, Rml::Style::Nav::Auto},
            {Rml::PropertyId::NavRight, Rml::Style::Nav::Auto},
            {Rml::PropertyId::Opacity, rml_number(1.0f)},
            {Rml::PropertyId::FontFamily, rml_string("Inter")},
        });

    add_focus_border(m_element, BorderRadiusSmall);

    auto* left = append(m_element, "div", {},
        {
            {Rml::PropertyId::Display, Rml::Style::Display::Flex},
            {Rml::PropertyId::FlexDirection, Rml::Style::FlexDirection::Column},
            {Rml::PropertyId::RowGap, rml_dp(4.0f)},
            {Rml::PropertyId::ColumnGap, rml_dp(4.0f)},
            {Rml::PropertyId::MinWidth, rml_px(0.0f)},
            {Rml::PropertyId::Width, rml_px(0.0f)},
            {Rml::PropertyId::FlexGrow, rml_number(1.0f)},
            {Rml::PropertyId::FlexShrink, rml_number(1.0f)},
            {Rml::PropertyId::PointerEvents, Rml::Style::PointerEvents::None},
        });

    m_title = add_label(left, title, LabelStyle::Large);
    set_props(m_title, {
                           {Rml::PropertyId::Color, rml_color(TextDim)},
                           {Rml::PropertyId::FontSize, rml_dp(28.0f)},
                           {Rml::PropertyId::LetterSpacing, rml_dp(1.0f)},
                       });

    if (!value.empty() || !detail.empty()) {
        auto* right = append(m_element, "div", {},
            {
                {Rml::PropertyId::Display, Rml::Style::Display::Flex},
                {Rml::PropertyId::FlexDirection, Rml::Style::FlexDirection::Column},
                {Rml::PropertyId::AlignItems, Rml::Style::AlignItems::FlexEnd},
                {Rml::PropertyId::JustifyContent, Rml::Style::JustifyContent::Center},
                {Rml::PropertyId::RowGap, rml_dp(4.0f)},
                {Rml::PropertyId::ColumnGap, rml_dp(4.0f)},
                {Rml::PropertyId::MinWidth, rml_dp(170.0f)},
                {Rml::PropertyId::MaxWidth, rml_percent(48.0f)},
                {Rml::PropertyId::FlexShrink, rml_number(0.0f)},
                {Rml::PropertyId::PointerEvents, Rml::Style::PointerEvents::None},
            });

        if (!value.empty()) {
            m_value = add_label(right, value, LabelStyle::Body);
            set_props(
                m_value, {
                             {Rml::PropertyId::Color, rml_color(TextDim)},
                             {Rml::PropertyId::TextAlign, Rml::Style::TextAlign::Right},
                             {Rml::PropertyId::OverflowX, Rml::Style::Overflow::Hidden},
                             {Rml::PropertyId::OverflowY, Rml::Style::Overflow::Hidden},
                             {Rml::PropertyId::TextOverflow, Rml::Style::TextOverflow::Ellipsis},
                             {Rml::PropertyId::WhiteSpace, Rml::Style::WhiteSpace::Nowrap},
                         });
        }

        if (!detail.empty()) {
            m_detail = add_label(right, detail, LabelStyle::Annotation);
            set_props(m_detail, {
                                    {Rml::PropertyId::Color, rml_color(TextDim)},
                                    {Rml::PropertyId::TextAlign, Rml::Style::TextAlign::Right},
                                });
        }
    }

    m_element->AddEventListener(Rml::EventId::Click, this);
    m_element->AddEventListener(Rml::EventId::Focus, this);
    m_element->AddEventListener(Rml::EventId::Blur, this);
    m_element->AddEventListener(Rml::EventId::Mouseover, this);
    m_element->AddEventListener(Rml::EventId::Mouseout, this);
    apply_style();
}

GameOption::~GameOption() {
    if (m_element == nullptr) {
        return;
    }

    m_element->RemoveEventListener(Rml::EventId::Click, this);
    m_element->RemoveEventListener(Rml::EventId::Focus, this);
    m_element->RemoveEventListener(Rml::EventId::Blur, this);
    m_element->RemoveEventListener(Rml::EventId::Mouseover, this);
    m_element->RemoveEventListener(Rml::EventId::Mouseout, this);
    m_element = nullptr;
}

void GameOption::ProcessEvent(Rml::Event& event) {
    switch (event.GetId()) {
    case Rml::EventId::Click:
        if (m_pressedCallback) {
            m_pressedCallback();
        }
        break;
    case Rml::EventId::Focus:
        m_focused = true;
        apply_style();
        break;
    case Rml::EventId::Blur:
        m_focused = false;
        apply_style();
        break;
    case Rml::EventId::Mouseover:
        m_hovered = true;
        apply_style();
        break;
    case Rml::EventId::Mouseout:
        m_hovered = false;
        apply_style();
        break;
    default:
        break;
    }
}

Rml::Element* GameOption::element() const {
    return m_element;
}

std::string GameOption::id() const {
    return m_element == nullptr ? std::string{} : m_element->GetId();
}

void GameOption::set_value(std::string_view value) {
    set_text(m_value, value);
}

void GameOption::apply_style() {
    if (m_element == nullptr) {
        return;
    }

    const bool active = m_hovered || m_focused;
    apply_control_surface_style(
        m_element, control_surface_style(ControlSurfaceTone::Quiet), active);
    m_element->SetProperty(
        Rml::PropertyId::Color, rml_color(active ? theme::TextActive : theme::TextDim));
    m_title->SetProperty(
        Rml::PropertyId::Color, rml_color(active ? theme::TextActive : theme::TextDim));
    if (m_value != nullptr) {
        m_value->SetProperty(
            Rml::PropertyId::Color, rml_color(active ? theme::TextActive : theme::TextDim));
    }
    if (m_detail != nullptr) {
        m_detail->SetProperty(Rml::PropertyId::Color, rml_color(theme::TextDim));
    }
    set_focus_border_visible(m_element, m_focused);
}

}  // namespace dusk::ui
