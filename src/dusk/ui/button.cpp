#include "button.hpp"

#include "element.hpp"
#include "focus_border.hpp"
#include "label.hpp"
#include "theme.hpp"

#include <RmlUi/Core.h>

#include <utility>

namespace dusk::ui {
namespace {

theme::Color variant_color(ButtonVariant variant) {
    switch (variant) {
    case ButtonVariant::Primary:
        return theme::Primary;
    case ButtonVariant::Secondary:
        return theme::Secondary;
    case ButtonVariant::Quiet:
    default:
        return theme::Elevated;
    }
}

}  // namespace

Button::Button(Rml::Element* parent, std::string_view id, std::string_view text,
               ButtonVariant variant, std::function<void()> pressedCallback)
    : m_variant(variant), m_pressedCallback(std::move(pressedCallback)) {
    using namespace theme;

    m_element = append(parent, "button", id);
    set_props(m_element, {
                             {"display", "flex"},
                             {"position", "relative"},
                             {"flex-direction", "row"},
                             {"align-items", "center"},
                             {"justify-content", "center"},
                             {"box-sizing", "border-box"},
                             {"width", "100%"},
                             {"height", "68dp"},
                             {"min-height", "68dp"},
                             {"max-height", "68dp"},
                             {"padding-left", "22dp"},
                             {"padding-right", "22dp"},
                             {"border-width", dp(BorderWidth)},
                             {"border-radius", dp(BorderRadiusMedium)},
                             {"cursor", "pointer"},
                             {"tab-index", "auto"},
                             {"nav-up", "auto"},
                             {"nav-down", "auto"},
                             {"nav-left", "auto"},
                             {"nav-right", "auto"},
                             {"opacity", "1"},
                             {"font-family", "Inter"},
                             {"color", rgba(Text)},
                         });

    add_focus_border(m_element, BorderRadiusMedium);
    m_label = append_text(m_element, "span", text);
    apply_label_style(m_label, LabelStyle::Medium);
    set_props(m_label, {
                           {"pointer-events", "none"},
                           {"text-align", "center"},
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
        m_focused = true;
        apply_style();
        break;
    case Rml::EventId::Mouseout:
        m_focused = false;
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
    const bool isBasic = m_variant == ButtonVariant::Quiet;
    const Color color = variant_color(m_variant);

    int borderOpacity = isBasic ? 0 : 190;
    int backgroundOpacity = isBasic ? 0 : 28;
    int backgroundHoverOpacity = 116;
    int borderHoverOpacity = isBasic ? backgroundHoverOpacity : 255;

    if (m_variant == ButtonVariant::Quiet) {
        backgroundHoverOpacity = 68;
        borderHoverOpacity = 150;
    }

    m_element->SetProperty("border-color",
                           rgba(color, active ? borderHoverOpacity : borderOpacity));
    m_element->SetProperty("background-color",
                           rgba(color, active ? backgroundHoverOpacity : backgroundOpacity));
    m_element->SetProperty("color", rgba(active ? TextActive : Text));
    m_label->SetProperty("color", rgba(active ? TextActive : Text));
    set_focus_border_visible(m_element, m_focused);
}

}  // namespace dusk::ui
