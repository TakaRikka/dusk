#include "button.hpp"

#include "control_surface.hpp"
#include "element.hpp"
#include "focus_border.hpp"
#include "label.hpp"
#include "theme.hpp"

#include <RmlUi/Core.h>

#include <utility>

namespace dusk::ui {
namespace {

ControlSurfaceTone control_surface_tone(ButtonVariant variant) {
    switch (variant) {
    case ButtonVariant::Primary:
        return ControlSurfaceTone::Primary;
    case ButtonVariant::Secondary:
        return ControlSurfaceTone::Secondary;
    case ButtonVariant::Quiet:
    default:
        return ControlSurfaceTone::Quiet;
    }
}

}  // namespace

Button::Button(Rml::Element* parent, std::string_view id, std::string_view text,
    ButtonVariant variant, std::function<void()> pressedCallback)
    : m_variant(variant), m_pressedCallback(std::move(pressedCallback)) {
    using namespace theme;

    m_element = append(parent, "button", id,
        {
            {Rml::PropertyId::Display, Rml::Style::Display::Flex},
            {Rml::PropertyId::Position, Rml::Style::Position::Relative},
            {Rml::PropertyId::FlexDirection, Rml::Style::FlexDirection::Row},
            {Rml::PropertyId::AlignItems, Rml::Style::AlignItems::Center},
            {Rml::PropertyId::JustifyContent, Rml::Style::JustifyContent::Center},
            {Rml::PropertyId::BoxSizing, Rml::Style::BoxSizing::BorderBox},
            {Rml::PropertyId::Width, rml_percent(100.0f)},
            {Rml::PropertyId::Height, rml_dp(68.0f)},
            {Rml::PropertyId::MinHeight, rml_dp(68.0f)},
            {Rml::PropertyId::MaxHeight, rml_dp(68.0f)},
            {Rml::PropertyId::PaddingLeft, rml_dp(22.0f)},
            {Rml::PropertyId::PaddingRight, rml_dp(22.0f)},
            {Rml::PropertyId::BorderTopWidth, rml_dp(BorderWidth)},
            {Rml::PropertyId::BorderRightWidth, rml_dp(BorderWidth)},
            {Rml::PropertyId::BorderBottomWidth, rml_dp(BorderWidth)},
            {Rml::PropertyId::BorderLeftWidth, rml_dp(BorderWidth)},
            {Rml::PropertyId::BorderTopLeftRadius, rml_dp(BorderRadiusMedium)},
            {Rml::PropertyId::BorderTopRightRadius, rml_dp(BorderRadiusMedium)},
            {Rml::PropertyId::BorderBottomRightRadius, rml_dp(BorderRadiusMedium)},
            {Rml::PropertyId::BorderBottomLeftRadius, rml_dp(BorderRadiusMedium)},
            {Rml::PropertyId::Cursor, rml_string("pointer")},
            {Rml::PropertyId::TabIndex, Rml::Style::TabIndex::Auto},
            {Rml::PropertyId::NavUp, Rml::Style::Nav::Auto},
            {Rml::PropertyId::NavDown, Rml::Style::Nav::Auto},
            {Rml::PropertyId::NavLeft, Rml::Style::Nav::Auto},
            {Rml::PropertyId::NavRight, Rml::Style::Nav::Auto},
            {Rml::PropertyId::Opacity, rml_number(1.0f)},
            {Rml::PropertyId::FontFamily, rml_string("Inter")},
            {Rml::PropertyId::Color, rml_color(Text)},
        });

    add_focus_border(m_element, BorderRadiusMedium);
    m_label = append_text(m_element, "span", text);
    apply_label_style(m_label, LabelStyle::Medium);
    set_props(m_label, {
                           {Rml::PropertyId::PointerEvents, Rml::Style::PointerEvents::None},
                           {Rml::PropertyId::TextAlign, Rml::Style::TextAlign::Center},
                       });

    m_element->AddEventListener(Rml::EventId::Click, this);
    m_element->AddEventListener(Rml::EventId::Focus, this);
    m_element->AddEventListener(Rml::EventId::Blur, this);
    m_element->AddEventListener(Rml::EventId::Mouseover, this);
    m_element->AddEventListener(Rml::EventId::Mouseout, this);
    apply_style();
}

Button::~Button() {
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

void Button::ProcessEvent(Rml::Event& event) {
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

Rml::Element* Button::element() const {
    return m_element;
}

std::string Button::id() const {
    return m_element == nullptr ? std::string{} : m_element->GetId();
}

void Button::set_text(std::string_view text) {
    ui::set_text(m_label, text);
}

void Button::apply_style() {
    using namespace theme;

    if (m_element == nullptr) {
        return;
    }

    const bool active = m_hovered || m_focused;
    apply_control_surface_style(
        m_element, control_surface_style(control_surface_tone(m_variant)), active);
    m_element->SetProperty(Rml::PropertyId::Color, rml_color(active ? TextActive : Text));
    m_label->SetProperty(Rml::PropertyId::Color, rml_color(active ? TextActive : Text));
    set_focus_border_visible(m_element, m_focused);
}

}  // namespace dusk::ui
