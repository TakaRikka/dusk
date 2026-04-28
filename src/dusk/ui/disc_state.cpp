#include "disc_state.hpp"

#include "element.hpp"
#include "focus_border.hpp"
#include "label.hpp"
#include "theme.hpp"

#include <RmlUi/Core.h>

#include <utility>

namespace dusk::ui {

DiscState::DiscState(Rml::Element* parent, std::string_view id, std::string_view text,
    std::string_view statusText, bool statusIsError, std::function<void()> pressedCallback)
    : m_pressedCallback(std::move(pressedCallback)), m_statusIsError(statusIsError) {
    using namespace theme;

    m_element = append(parent, "button", id,
        {
            {Rml::PropertyId::Display, Rml::Style::Display::Flex},
            {Rml::PropertyId::Position, Rml::Style::Position::Relative},
            {Rml::PropertyId::FlexDirection, Rml::Style::FlexDirection::Column},
            {Rml::PropertyId::AlignItems, Rml::Style::AlignItems::Stretch},
            {Rml::PropertyId::RowGap, rml_dp(6.0f)},
            {Rml::PropertyId::ColumnGap, rml_dp(6.0f)},
            {Rml::PropertyId::Width, rml_percent(100.0f)},
            {Rml::PropertyId::BoxSizing, Rml::Style::BoxSizing::BorderBox},
            {Rml::PropertyId::PaddingTop, rml_dp(14.0f)},
            {Rml::PropertyId::PaddingRight, rml_dp(16.0f)},
            {Rml::PropertyId::PaddingBottom, rml_dp(14.0f)},
            {Rml::PropertyId::PaddingLeft, rml_dp(16.0f)},
            {Rml::PropertyId::BorderTopWidth, rml_dp(BorderWidth)},
            {Rml::PropertyId::BorderRightWidth, rml_dp(BorderWidth)},
            {Rml::PropertyId::BorderBottomWidth, rml_dp(BorderWidth)},
            {Rml::PropertyId::BorderLeftWidth, rml_dp(BorderWidth)},
            {Rml::PropertyId::BorderTopLeftRadius, rml_dp(BorderRadiusSmall)},
            {Rml::PropertyId::BorderTopRightRadius, rml_dp(BorderRadiusSmall)},
            {Rml::PropertyId::BorderBottomRightRadius, rml_dp(BorderRadiusSmall)},
            {Rml::PropertyId::BorderBottomLeftRadius, rml_dp(BorderRadiusSmall)},
            {Rml::PropertyId::Cursor, rml_string("pointer")},
            {Rml::PropertyId::TabIndex, Rml::Style::TabIndex::Auto},
            {Rml::PropertyId::NavUp, Rml::Style::Nav::Auto},
            {Rml::PropertyId::NavDown, Rml::Style::Nav::Auto},
            {Rml::PropertyId::NavLeft, Rml::Style::Nav::Auto},
            {Rml::PropertyId::NavRight, Rml::Style::Nav::Auto},
            {Rml::PropertyId::FontFamily, rml_string("Inter")},
        });

    add_focus_border(m_element, BorderRadiusSmall);

    m_value = add_label(m_element, text, LabelStyle::Body);
    set_props(m_value, {
                           {Rml::PropertyId::OverflowX, Rml::Style::Overflow::Hidden},
                           {Rml::PropertyId::OverflowY, Rml::Style::Overflow::Hidden},
                           {Rml::PropertyId::TextOverflow, Rml::Style::TextOverflow::Ellipsis},
                           {Rml::PropertyId::WhiteSpace, Rml::Style::WhiteSpace::Nowrap},
                           {Rml::PropertyId::PointerEvents, Rml::Style::PointerEvents::None},
                       });

    if (!statusText.empty()) {
        m_status = add_label(m_element, statusText, LabelStyle::Annotation);
        set_props(m_status, {
                                {Rml::PropertyId::PointerEvents, Rml::Style::PointerEvents::None},
                                {Rml::PropertyId::WhiteSpace, Rml::Style::WhiteSpace::Normal},
                            });
    }

    m_element->AddEventListener(Rml::EventId::Click, this);
    m_element->AddEventListener(Rml::EventId::Focus, this);
    m_element->AddEventListener(Rml::EventId::Blur, this);
    m_element->AddEventListener(Rml::EventId::Mouseover, this);
    m_element->AddEventListener(Rml::EventId::Mouseout, this);
    apply_style();
}

DiscState::~DiscState() {
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

void DiscState::ProcessEvent(Rml::Event& event) {
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

Rml::Element* DiscState::element() const {
    return m_element;
}

std::string DiscState::id() const {
    return m_element == nullptr ? std::string{} : m_element->GetId();
}

void DiscState::apply_style() {
    using namespace theme;

    if (m_element == nullptr) {
        return;
    }

    const bool active = m_hovered || m_focused;
    const Color accent = m_statusIsError ? Danger : Primary;

    m_element->SetProperty(Rml::PropertyId::BackgroundColor,
        rml_color(accent, active ? 52 : (m_statusIsError ? 32 : 20)));
    const auto borderColor = rml_color(accent, active ? 220 : (m_statusIsError ? 190 : 120));
    m_element->SetProperty(Rml::PropertyId::BorderTopColor, borderColor);
    m_element->SetProperty(Rml::PropertyId::BorderRightColor, borderColor);
    m_element->SetProperty(Rml::PropertyId::BorderBottomColor, borderColor);
    m_element->SetProperty(Rml::PropertyId::BorderLeftColor, borderColor);
    m_element->SetProperty(Rml::PropertyId::Color, rml_color(active ? TextActive : Text));

    m_value->SetProperty(Rml::PropertyId::Color, rml_color(active ? TextActive : Text));
    if (m_status != nullptr) {
        m_status->SetProperty(
            Rml::PropertyId::Color, rml_color(m_statusIsError ? Danger : TextDim));
    }
    set_focus_border_visible(m_element, m_focused);
}

}  // namespace dusk::ui
