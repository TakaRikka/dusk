#include "window.hpp"
#include "element.hpp"
#include "focus_border.hpp"
#include "label.hpp"
#include "theme.hpp"

#include <RmlUi/Core.h>

namespace dusk::ui {
WindowTab::WindowTab(Rml::Element* parent, std::string_view id, std::string_view label, std::function<void()> selectedCallback) : m_selectedCallback(std::move(selectedCallback)) {
    using namespace theme;

    m_element = append(parent, "button", id);
    set_props(m_element, {
        {"display", "flex"},
        {"position", "relative"},
        {"flex-direction", "column"},
        {"align-items", "center"},
        {"justify-content", "center"},
        {"box-sizing", "border-box"},
        {"height", "100%"},
        {"padding-left", "20dp"},
        {"padding-right", "20dp"},
        {"background-color", rgba(Transparent)},
        {"border-width", "0"},
        {"cursor", "pointer"},
        {"tab-index", "auto"},
        {"nav-up", "auto"},
        {"nav-down", "auto"},
        {"nav-left", "auto"},
        {"nav-right", "auto"},
        {"font-family", "Inter"},
    });

    add_focus_border(m_element, BorderRadiusSmall);

    m_label = append_text(m_element, "span", label);
    apply_label_style(m_label, LabelStyle::Body);
    set_props(m_label, {
        {"pointer-events", "none"},
        {"font-size", "20dp"},
        {"letter-spacing", "1dp"},
        {"font-weight", "700"},
        {"text-align", "center"},
    });

    m_indicator = append(m_element, "div");
    set_props(m_indicator, {
        {"position", "absolute"},
        {"left", "0"},
        {"right", "0"},
        {"bottom", dp(-BorderWidth)},
        {"height", dp(2.0f)},
        {"background-color", rgba(WindowAccent, 0)},
        {"pointer-events", "none"},
    });

    m_element->AddEventListener(Rml::EventId::Click, this);
    m_element->AddEventListener(Rml::EventId::Focus, this);
    m_element->AddEventListener(Rml::EventId::Blur, this);
    m_element->AddEventListener(Rml::EventId::Mouseover, this);
    m_element->AddEventListener(Rml::EventId::Mouseout, this);
    apply_style();
}

WindowTab::~WindowTab() {
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

void WindowTab::ProcessEvent(Rml::Event& event) {
    switch (event.GetId()) {
    case Rml::EventId::Click:
        if (m_selectedCallback) {
            m_selectedCallback();
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

std::string WindowTab::id() const {
    return m_element == nullptr ? std::string{} : m_element->GetId();
}

void WindowTab::set_selected(bool selected) {
    if (m_selected == selected) {
        return;
    }
    m_selected = selected;
    apply_style();
}

void WindowTab::apply_style() {
    using namespace theme;

    if (m_element == nullptr) {
        return;
    }

    const bool active = m_hovered || m_focused;

    int textOpacity;
    if (m_selected) {
        textOpacity = 255;
    } else if (active) {
        textOpacity = 200;
    } else {
        textOpacity = 110;
    }
    const Color textColor = m_selected ? TextActive : Text;
    m_label->SetProperty("color", rgba(textColor, textOpacity));

    if (m_indicator != nullptr) {
        const int indicatorOpacity = m_selected ? 255 : (active ? 96 : 0);
        m_indicator->SetProperty("background-color", rgba(WindowAccent, indicatorOpacity));
    }

    set_focus_border_visible(m_element, m_focused);
}

Window::Window(Rml::Element* parent, std::string_view id, std::function<void()> closeCallback) : m_closeCallback(std::move(closeCallback)) {
    using namespace theme;

    m_element = append(parent, "div", id);
    set_props(m_element, {
        {"display", "flex"},
        {"flex-direction", "column"},
        {"box-sizing", "border-box"},
        {"width", "100%"},
        {"max-width", "1088dp"},
        {"border-width", dp(BorderWidth)},
        {"border-radius", dp(BorderRadiusMedium)},
        {"border-color", rgba(ElevatedBorder)},
        {"background-color", rgba(WindowSurface)},
        {"overflow", "hidden"},
    });

    m_tabBar = append(m_element, "div");
    set_props(m_tabBar, {
        {"display", "flex"},
        {"position", "relative"},
        {"flex-direction", "row"},
        {"align-items", "center"},
        {"box-sizing", "border-box"},
        {"width", "100%"},
        {"height", dp(WindowTabBarHeight)},
        {"min-height", dp(WindowTabBarHeight)},
        {"padding-left", "12dp"},
        {"padding-right", "12dp"},
        {"gap", "4dp"},
        {"background-color", rgba(WindowTitleOverlay)},
        {"border-bottom-width", dp(BorderWidth * 1.5f)},
        {"border-bottom-color", rgba(WindowDivider)},
    });

    m_tabStrip = append(m_tabBar, "div");
    set_props(m_tabStrip, {
        {"display", "flex"},
        {"flex-direction", "row"},
        {"align-items", "stretch"},
        {"justify-content", "flex-start"},
        {"flex-grow", "1"},
        {"flex-shrink", "1"},
        {"min-width", "0"},
        {"height", "100%"},
        {"gap", "4dp"},
    });

    const std::string closeId = id.empty() ? std::string{} : std::string(id) + "-close";
    m_closeButton = append(m_tabBar, "button", closeId);
    set_props(m_closeButton, {
        {"display", "flex"},
        {"position", "relative"},
        {"align-items", "center"},
        {"justify-content", "center"},
        {"box-sizing", "border-box"},
        {"width", "36dp"},
        {"height", "36dp"},
        {"flex-shrink", "0"},
        {"border-width", "0"},
        {"border-radius", dp(BorderRadiusSmall)},
        {"background-color", rgba(Transparent)},
        {"cursor", "pointer"},
        {"tab-index", "auto"},
        {"font-family", "Inter"},
    });
    add_focus_border(m_closeButton, BorderRadiusSmall);

    auto* closeGlyph = append_text(m_closeButton, "span", "\xc3\x97");
    set_props(closeGlyph, {
        {"font-size", "22dp"},
        {"font-weight", "400"},
        {"color", rgba(WindowGlyph)},
        {"pointer-events", "none"},
    });

    m_closeButton->AddEventListener(Rml::EventId::Click, this);
    m_closeButton->AddEventListener(Rml::EventId::Focus, this);
    m_closeButton->AddEventListener(Rml::EventId::Blur, this);
    m_closeButton->AddEventListener(Rml::EventId::Mouseover, this);
    m_closeButton->AddEventListener(Rml::EventId::Mouseout, this);

    m_contentRow = append(m_element, "div");
    set_props(m_contentRow, {
        {"display", "flex"},
        {"flex-direction", "row"},
        {"align-items", "stretch"},
        {"box-sizing", "border-box"},
        {"width", "100%"},
        {"flex-grow", "1"},
        {"flex-shrink", "1"},
        {"min-height", "0"},
        {"min-width", "0"},
        {"gap", "20dp"},
    });

    m_leftPane = append(m_contentRow, "div");
    set_props(m_leftPane, {
        {"display", "flex"},
        {"flex-direction", "column"},
        {"box-sizing", "border-box"},
        {"min-width", "0"},
        {"min-height", "0"},
        {"padding", "24dp"},
        {"gap", "12dp"},
    });

    m_rightPane = append(m_contentRow, "div");
    set_props(m_rightPane, {
        {"display", "none"},
        {"flex-direction", "column"},
        {"box-sizing", "border-box"},
        {"min-width", "0"},
        {"min-height", "0"},
        {"padding-top", "24dp"},
        {"padding-bottom", "24dp"},
        {"padding-right", "24dp"},
        {"padding-left", "8dp"},
        {"align-items", "flex-start"},
        {"overflow-y", "auto"},
    });

    m_rightPane = append(m_contentRow, "div");
    set_props(m_rightPane, {
                                {"display", "none"},
                                {"flex-direction", "column"},
                                {"box-sizing", "border-box"},
                                {"min-width", "0"},
                                {"min-height", "0"},
                                {"padding-top", "24dp"},
                                {"padding-bottom", "24dp"},
                                {"padding-right", "24dp"},
                                {"padding-left", "8dp"},
                                {"align-items", "flex-start"},
                                {"overflow-y", "auto"},
                            });

    apply_pane_layout();
    apply_close_style();
}

Window::~Window() {
    m_tabs.clear();
    if (m_closeButton != nullptr) {
        m_closeButton->RemoveEventListener(Rml::EventId::Click, this);
        m_closeButton->RemoveEventListener(Rml::EventId::Focus, this);
        m_closeButton->RemoveEventListener(Rml::EventId::Blur, this);
        m_closeButton->RemoveEventListener(Rml::EventId::Mouseover, this);
        m_closeButton->RemoveEventListener(Rml::EventId::Mouseout, this);
        m_closeButton = nullptr;
    }
    m_element = nullptr;
}

void Window::ProcessEvent(Rml::Event& event) {
    if (event.GetTargetElement() != m_closeButton) {
        return;
    }

    switch (event.GetId()) {
    case Rml::EventId::Click:
        if (m_closeCallback) {
            m_closeCallback();
        }
        break;
    case Rml::EventId::Focus:
        m_closeFocused = true;
        apply_close_style();
        break;
    case Rml::EventId::Blur:
        m_closeFocused = false;
        apply_close_style();
        break;
    case Rml::EventId::Mouseover:
        m_closeHovered = true;
        apply_close_style();
        break;
    case Rml::EventId::Mouseout:
        m_closeHovered = false;
        apply_close_style();
        break;
    default:
        break;
    }
}

WindowTab* Window::add_tab(std::string_view id, std::string_view label, std::function<void()> selectedCallback) {
    if (m_tabStrip == nullptr) {
        return nullptr;
    }

    const std::string idString(id);
    auto wrapped = [this, idString, cb = std::move(selectedCallback)]() {
        set_selected_tab(idString);
        if (cb) {
            cb();
        }
    };
    auto tab = std::make_unique<WindowTab>(m_tabStrip, idString, label, std::move(wrapped));
    WindowTab* raw = tab.get();
    m_tabs.push_back(std::move(tab));
    return raw;
}

void Window::set_selected_tab(std::string_view id) {
    for (auto& tab : m_tabs) {
        tab->set_selected(tab->id() == id);
    }
}

std::string Window::selected_tab_id() const {
    for (const auto& tab : m_tabs) {
        if (tab->is_selected()) {
            return tab->id();
        }
    }
    return {};
}

void Window::set_right_pane_visible(bool visible) {
    if (m_rightPaneVisible == visible) {
        return;
    }
    m_rightPaneVisible = visible;
    apply_pane_layout();
}

void Window::apply_pane_layout() {
    using namespace theme;

    if (m_leftPane == nullptr || m_rightPane == nullptr) {
        return;
    }

    if (m_rightPaneVisible) {
        set_props(m_leftPane, {
            {"display", "flex"},
            {"flex", "1 1 0"},
            {"min-width", "0"},
        });
        set_props(m_rightPane, {
            {"display", "flex"},
            {"flex", "0 0 40%"},
            {"min-width", "0"},
        });
    } else {
        set_props(m_leftPane, {
            {"display", "flex"},
            {"flex", "1 1 auto"},
            {"width", "100%"},
        });
        set_props(m_rightPane, {
            {"display", "none"},
        });
    }
}

void Window::apply_close_style() {
    using namespace theme;

    if (m_closeButton == nullptr) {
        return;
    }

    const bool active = m_closeHovered || m_closeFocused;
    m_closeButton->SetProperty("background-color", active ? rgba(WindowAccent, 56) : rgba(Transparent));
    set_focus_border_visible(m_closeButton, m_closeFocused);
}
}  // namespace dusk::ui
