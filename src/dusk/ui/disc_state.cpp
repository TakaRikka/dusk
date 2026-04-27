#include "disc_state.hpp"

#include "element.hpp"
#include "focus_border.hpp"
#include "label.hpp"
#include "theme.hpp"

#include <RmlUi/Core.h>

#include <utility>

namespace dusk::ui {

DiscState::DiscState(Rml::Element* parent, std::string_view id, std::string_view text,
                     std::string_view statusText, bool statusIsError,
                     std::function<void()> pressedCallback)
    : m_pressedCallback(std::move(pressedCallback)), m_statusIsError(statusIsError) {
    using namespace theme;

    m_element = append(parent, "button", id);
    set_props(m_element, {
                             {"display", "flex"},
                             {"position", "relative"},
                             {"flex-direction", "column"},
                             {"align-items", "stretch"},
                             {"gap", "6dp"},
                             {"width", "100%"},
                             {"box-sizing", "border-box"},
                             {"padding", "14dp 16dp"},
                             {"border-width", dp(BorderWidth)},
                             {"border-radius", dp(BorderRadiusSmall)},
                             {"cursor", "pointer"},
                             {"tab-index", "auto"},
                             {"nav-up", "auto"},
                             {"nav-down", "auto"},
                             {"nav-left", "auto"},
                             {"nav-right", "auto"},
                             {"font-family", "Inter"},
                         });

    add_focus_border(m_element, BorderRadiusSmall);

    m_value = add_label(m_element, text, LabelStyle::Body);
    set_props(m_value, {
                           {"overflow", "hidden"},
                           {"text-overflow", "ellipsis"},
                           {"white-space", "nowrap"},
                           {"pointer-events", "none"},
                       });

    if (!statusText.empty()) {
        m_status = add_label(m_element, statusText, LabelStyle::Annotation);
        set_props(m_status, {
                                {"pointer-events", "none"},
                                {"white-space", "normal"},
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

    m_element->SetProperty("background-color",
                           rgba(accent, active ? 52 : (m_statusIsError ? 32 : 20)));
    m_element->SetProperty("border-color",
                           rgba(accent, active ? 220 : (m_statusIsError ? 190 : 120)));
    m_element->SetProperty("color", rgba(active ? TextActive : Text));

    m_value->SetProperty("color", rgba(active ? TextActive : Text));
    if (m_status != nullptr) {
        m_status->SetProperty("color", rgba(m_statusIsError ? Danger : TextDim));
    }
    set_focus_border_visible(m_element, m_focused);
}

}  // namespace dusk::ui
