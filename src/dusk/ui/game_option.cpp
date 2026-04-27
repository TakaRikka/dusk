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
                       std::string_view value, std::string_view detail,
                       std::function<void()> pressedCallback)
    : m_pressedCallback(std::move(pressedCallback)) {
    using namespace theme;

    m_element = append(parent, "button", id);
    set_props(m_element, {
                             {"display", "flex"},
                             {"position", "relative"},
                             {"flex-direction", "row"},
                             {"align-items", "center"},
                             {"justify-content", "space-between"},
                             {"box-sizing", "border-box"},
                             {"gap", "16dp"},
                             {"width", "100%"},
                             {"height", "auto"},
                             {"padding", "16dp"},
                             {"border-width", dp(BorderWidth)},
                             {"border-radius", dp(BorderRadiusSmall)},
                             {"background-color", rgba(Transparent)},
                             {"border-color", rgba(ElevatedBorder, 0)},
                             {"color", rgba(TextDim)},
                             {"cursor", "pointer"},
                             {"tab-index", "auto"},
                             {"nav-up", "auto"},
                             {"nav-down", "auto"},
                             {"nav-left", "auto"},
                             {"nav-right", "auto"},
                             {"opacity", "1"},
                             {"font-family", "Inter"},
                         });

    add_focus_border(m_element, BorderRadiusSmall);

    auto* left = append(m_element, "div");
    set_props(left, {
                        {"display", "flex"},
                        {"flex-direction", "column"},
                        {"gap", "4dp"},
                        {"min-width", "0"},
                        {"width", "0"},
                        {"flex-grow", "1"},
                        {"flex-shrink", "1"},
                        {"pointer-events", "none"},
                    });

    m_title = add_label(left, title, LabelStyle::Large);
    set_props(m_title, {
                           {"color", rgba(TextDim)},
                           {"font-size", "28dp"},
                           {"letter-spacing", "1dp"},
                       });

    if (!value.empty() || !detail.empty()) {
        auto* right = append(m_element, "div");
        set_props(right, {
                             {"display", "flex"},
                             {"flex-direction", "column"},
                             {"align-items", "flex-end"},
                             {"justify-content", "center"},
                             {"gap", "4dp"},
                             {"min-width", "170dp"},
                             {"max-width", "48%"},
                             {"flex-shrink", "0"},
                             {"pointer-events", "none"},
                         });

        if (!value.empty()) {
            m_value = add_label(right, value, LabelStyle::Body);
            set_props(m_value, {
                                   {"color", rgba(TextDim)},
                                   {"text-align", "right"},
                                   {"overflow", "hidden"},
                                   {"text-overflow", "ellipsis"},
                                   {"white-space", "nowrap"},
                               });
        }

        if (!detail.empty()) {
            m_detail = add_label(right, detail, LabelStyle::Annotation);
            set_props(m_detail, {
                                    {"color", rgba(TextDim)},
                                    {"text-align", "right"},
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
    apply_control_surface_style(m_element, control_surface_style(ControlSurfaceTone::Quiet),
                                active);
    m_element->SetProperty("color",
                           active ? theme::rgba(theme::TextActive) : theme::rgba(theme::TextDim));
    m_title->SetProperty("color",
                         active ? theme::rgba(theme::TextActive) : theme::rgba(theme::TextDim));
    if (m_value != nullptr) {
        m_value->SetProperty("color",
                             active ? theme::rgba(theme::TextActive) : theme::rgba(theme::TextDim));
    }
    if (m_detail != nullptr) {
        m_detail->SetProperty("color", theme::rgba(theme::TextDim));
    }
    set_focus_border_visible(m_element, m_focused);
}

}  // namespace dusk::ui
