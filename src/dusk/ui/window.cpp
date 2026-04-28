#include "window.hpp"
#include "element.hpp"
#include "focus_border.hpp"
#include "theme.hpp"

#include <RmlUi/Core.h>
#include <RmlUi/Core/PropertyDictionary.h>
#include <RmlUi/Core/StyleSheetSpecification.h>

#include <algorithm>

namespace dusk::ui {

WindowTab::WindowTab(Rml::Element* parent, std::string_view id, std::string_view label,
    std::function<void()> selectedCallback)
    : m_selectedCallback(std::move(selectedCallback)) {
    using namespace theme;

    m_element = append(parent, "button", id,
        {
            {Rml::PropertyId::Display, Rml::Style::Display::Flex},
            {Rml::PropertyId::Position, Rml::Style::Position::Relative},
            {Rml::PropertyId::FlexDirection, Rml::Style::FlexDirection::Column},
            {Rml::PropertyId::AlignItems, Rml::Style::AlignItems::Center},
            {Rml::PropertyId::JustifyContent, Rml::Style::JustifyContent::Center},
            {Rml::PropertyId::BoxSizing, Rml::Style::BoxSizing::BorderBox},
            {Rml::PropertyId::Height, rml_percent(100.0f)},
            {Rml::PropertyId::PaddingLeft, rml_dp(20.0f)},
            {Rml::PropertyId::PaddingRight, rml_dp(20.0f)},
            {Rml::PropertyId::BackgroundColor, rml_color(Transparent)},
            {Rml::PropertyId::BorderTopWidth, rml_px(0.0f)},
            {Rml::PropertyId::BorderRightWidth, rml_px(0.0f)},
            {Rml::PropertyId::BorderBottomWidth, rml_px(0.0f)},
            {Rml::PropertyId::BorderLeftWidth, rml_px(0.0f)},
            {Rml::PropertyId::Cursor, rml_string("pointer")},
            {Rml::PropertyId::TabIndex, Rml::Style::TabIndex::Auto},
            {Rml::PropertyId::NavUp, Rml::Style::Nav::Auto},
            {Rml::PropertyId::NavDown, Rml::Style::Nav::Auto},
            {Rml::PropertyId::NavLeft, Rml::Style::Nav::Auto},
            {Rml::PropertyId::NavRight, Rml::Style::Nav::Auto},
            {Rml::PropertyId::FontFamily, rml_string("Inter")},
        });

    add_focus_border(m_element, BorderRadiusSmall);

    m_label = append(m_element, "span", {},
        {
            {Rml::PropertyId::PointerEvents, Rml::Style::PointerEvents::None},
            {Rml::PropertyId::FontSize, rml_dp(20.0f)},
            {Rml::PropertyId::LetterSpacing, rml_dp(1.0f)},
            {Rml::PropertyId::FontWeight, Rml::Style::FontWeight::Bold},
            {Rml::PropertyId::TextAlign, Rml::Style::TextAlign::Center},
            {Rml::PropertyId::Color, rml_color(Text)},
        });
    set_text(m_label, label);

    m_indicator = append(m_element, "div", {},
        {
            {Rml::PropertyId::Position, Rml::Style::Position::Absolute},
            {Rml::PropertyId::Left, rml_px(0.0f)},
            {Rml::PropertyId::Right, rml_px(0.0f)},
            {Rml::PropertyId::Bottom, rml_dp(-BorderWidth)},
            {Rml::PropertyId::Height, rml_dp(2.0f)},
            {Rml::PropertyId::BackgroundColor, rml_color(WindowAccent, 0)},
            {Rml::PropertyId::PointerEvents, Rml::Style::PointerEvents::None},
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
    m_label->SetProperty(Rml::PropertyId::Color, rml_color(textColor, textOpacity));

    if (m_indicator != nullptr) {
        const int indicatorOpacity = m_selected ? 255 : (active ? 96 : 0);
        m_indicator->SetProperty(
            Rml::PropertyId::BackgroundColor, rml_color(WindowAccent, indicatorOpacity));
    }

    set_focus_border_visible(m_element, m_focused);
}

Window::Window(Rml::Element* parent, std::string_view id, std::function<void()> closeCallback)
    : m_closeCallback(std::move(closeCallback)) {
    using namespace theme;

    m_element = append(parent, "div", id,
        {
            {Rml::PropertyId::Display, Rml::Style::Display::Flex},
            {Rml::PropertyId::FlexDirection, Rml::Style::FlexDirection::Column},
            {Rml::PropertyId::BoxSizing, Rml::Style::BoxSizing::BorderBox},
            {Rml::PropertyId::Width, rml_percent(100.0f)},
            {Rml::PropertyId::MaxWidth, rml_dp(1088.0f)},
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
            {Rml::PropertyId::BackgroundColor, rml_color(WindowSurface)},
            {Rml::PropertyId::OverflowX, Rml::Style::Overflow::Hidden},
            {Rml::PropertyId::OverflowY, Rml::Style::Overflow::Hidden},
        });
    set_props(m_element, {
                             {"backdrop-filter", "blur(5dp)"},
                             {"box-shadow", "0 0 25dp 5dp"},
                         });

    m_tabBar = append(m_element, "div", {},
        {
            {Rml::PropertyId::Display, Rml::Style::Display::Flex},
            {Rml::PropertyId::Position, Rml::Style::Position::Relative},
            {Rml::PropertyId::FlexDirection, Rml::Style::FlexDirection::Row},
            {Rml::PropertyId::AlignItems, Rml::Style::AlignItems::Center},
            {Rml::PropertyId::BoxSizing, Rml::Style::BoxSizing::BorderBox},
            {Rml::PropertyId::Width, rml_percent(100.0f)},
            {Rml::PropertyId::Height, rml_dp(WindowTabBarHeight)},
            {Rml::PropertyId::MinHeight, rml_dp(WindowTabBarHeight)},
            {Rml::PropertyId::PaddingLeft, rml_dp(12.0f)},
            {Rml::PropertyId::PaddingRight, rml_dp(12.0f)},
            {Rml::PropertyId::RowGap, rml_dp(4.0f)},
            {Rml::PropertyId::ColumnGap, rml_dp(4.0f)},
            {Rml::PropertyId::BackgroundColor, rml_color(WindowTitleOverlay)},
            {Rml::PropertyId::BorderBottomWidth, rml_dp(BorderWidth * 1.5f)},
            {Rml::PropertyId::BorderBottomColor, rml_color(WindowDivider)},
        });

    m_tabStrip = append(m_tabBar, "div", {},
        {
            {Rml::PropertyId::Display, Rml::Style::Display::Flex},
            {Rml::PropertyId::FlexDirection, Rml::Style::FlexDirection::Row},
            {Rml::PropertyId::AlignItems, Rml::Style::AlignItems::Stretch},
            {Rml::PropertyId::JustifyContent, Rml::Style::JustifyContent::FlexStart},
            {Rml::PropertyId::FlexGrow, rml_number(1.0f)},
            {Rml::PropertyId::FlexShrink, rml_number(1.0f)},
            {Rml::PropertyId::MinWidth, rml_px(0.0f)},
            {Rml::PropertyId::Height, rml_percent(100.0f)},
            {Rml::PropertyId::RowGap, rml_dp(4.0f)},
            {Rml::PropertyId::ColumnGap, rml_dp(4.0f)},
        });

    const std::string closeId = id.empty() ? std::string{} : std::string(id) + "-close";
    m_closeButton = append(m_tabBar, "button", closeId,
        {
            {Rml::PropertyId::Display, Rml::Style::Display::Flex},
            {Rml::PropertyId::Position, Rml::Style::Position::Relative},
            {Rml::PropertyId::AlignItems, Rml::Style::AlignItems::Center},
            {Rml::PropertyId::JustifyContent, Rml::Style::JustifyContent::Center},
            {Rml::PropertyId::BoxSizing, Rml::Style::BoxSizing::BorderBox},
            {Rml::PropertyId::Width, rml_dp(36.0f)},
            {Rml::PropertyId::Height, rml_dp(36.0f)},
            {Rml::PropertyId::FlexShrink, rml_number(0.0f)},
            {Rml::PropertyId::BorderTopWidth, rml_px(0.0f)},
            {Rml::PropertyId::BorderRightWidth, rml_px(0.0f)},
            {Rml::PropertyId::BorderBottomWidth, rml_px(0.0f)},
            {Rml::PropertyId::BorderLeftWidth, rml_px(0.0f)},
            {Rml::PropertyId::BorderTopLeftRadius, rml_dp(BorderRadiusSmall)},
            {Rml::PropertyId::BorderTopRightRadius, rml_dp(BorderRadiusSmall)},
            {Rml::PropertyId::BorderBottomRightRadius, rml_dp(BorderRadiusSmall)},
            {Rml::PropertyId::BorderBottomLeftRadius, rml_dp(BorderRadiusSmall)},
            {Rml::PropertyId::BackgroundColor, rml_color(Transparent)},
            {Rml::PropertyId::Cursor, rml_string("pointer")},
            {Rml::PropertyId::TabIndex, Rml::Style::TabIndex::Auto},
            {Rml::PropertyId::FontFamily, rml_string("Inter")},
        });
    add_focus_border(m_closeButton, BorderRadiusSmall);

    auto* closeGlyph = append(m_closeButton, "span", {},
        {
            {Rml::PropertyId::FontSize, rml_dp(22.0f)},
            {Rml::PropertyId::FontWeight, Rml::Style::FontWeight::Normal},
            {Rml::PropertyId::Color, rml_color(WindowGlyph)},
            {Rml::PropertyId::PointerEvents, Rml::Style::PointerEvents::None},
        });
    set_text(closeGlyph, "\xc3\x97");

    m_closeButton->AddEventListener(Rml::EventId::Click, this);
    m_closeButton->AddEventListener(Rml::EventId::Focus, this);
    m_closeButton->AddEventListener(Rml::EventId::Blur, this);
    m_closeButton->AddEventListener(Rml::EventId::Mouseover, this);
    m_closeButton->AddEventListener(Rml::EventId::Mouseout, this);

    m_contentRow = append(m_element, "div", {},
        {
            {Rml::PropertyId::Display, Rml::Style::Display::Flex},
            {Rml::PropertyId::FlexDirection, Rml::Style::FlexDirection::Row},
            {Rml::PropertyId::AlignItems, Rml::Style::AlignItems::Stretch},
            {Rml::PropertyId::BoxSizing, Rml::Style::BoxSizing::BorderBox},
            {Rml::PropertyId::Width, rml_percent(100.0f)},
            {Rml::PropertyId::FlexGrow, rml_number(1.0f)},
            {Rml::PropertyId::FlexShrink, rml_number(1.0f)},
            {Rml::PropertyId::MinHeight, rml_px(0.0f)},
            {Rml::PropertyId::MinWidth, rml_px(0.0f)},
            {Rml::PropertyId::RowGap, rml_dp(20.0f)},
            {Rml::PropertyId::ColumnGap, rml_dp(20.0f)},
        });

    m_leftPane = append(m_contentRow, "div", {},
        {
            {Rml::PropertyId::Display, Rml::Style::Display::Flex},
            {Rml::PropertyId::FlexDirection, Rml::Style::FlexDirection::Column},
            {Rml::PropertyId::BoxSizing, Rml::Style::BoxSizing::BorderBox},
            {Rml::PropertyId::MinWidth, rml_px(0.0f)},
            {Rml::PropertyId::MinHeight, rml_px(0.0f)},
            {Rml::PropertyId::PaddingTop, rml_dp(24.0f)},
            {Rml::PropertyId::PaddingRight, rml_dp(24.0f)},
            {Rml::PropertyId::PaddingBottom, rml_dp(24.0f)},
            {Rml::PropertyId::PaddingLeft, rml_dp(24.0f)},
            {Rml::PropertyId::RowGap, rml_dp(12.0f)},
            {Rml::PropertyId::ColumnGap, rml_dp(12.0f)},
        });

    m_rightPane = append(m_contentRow, "div", {},
        {
            {Rml::PropertyId::Display, Rml::Style::Display::None},
            {Rml::PropertyId::FlexDirection, Rml::Style::FlexDirection::Column},
            {Rml::PropertyId::BoxSizing, Rml::Style::BoxSizing::BorderBox},
            {Rml::PropertyId::MinWidth, rml_px(0.0f)},
            {Rml::PropertyId::MinHeight, rml_px(0.0f)},
            {Rml::PropertyId::PaddingTop, rml_dp(24.0f)},
            {Rml::PropertyId::PaddingBottom, rml_dp(24.0f)},
            {Rml::PropertyId::PaddingRight, rml_dp(24.0f)},
            {Rml::PropertyId::PaddingLeft, rml_dp(8.0f)},
            {Rml::PropertyId::AlignItems, Rml::Style::AlignItems::FlexStart},
            {Rml::PropertyId::OverflowY, Rml::Style::Overflow::Auto},
        });

    m_rightPane = append(m_contentRow, "div", {},
        {
            {Rml::PropertyId::Display, Rml::Style::Display::None},
            {Rml::PropertyId::FlexDirection, Rml::Style::FlexDirection::Column},
            {Rml::PropertyId::BoxSizing, Rml::Style::BoxSizing::BorderBox},
            {Rml::PropertyId::MinWidth, rml_px(0.0f)},
            {Rml::PropertyId::MinHeight, rml_px(0.0f)},
            {Rml::PropertyId::PaddingTop, rml_dp(24.0f)},
            {Rml::PropertyId::PaddingBottom, rml_dp(24.0f)},
            {Rml::PropertyId::PaddingRight, rml_dp(24.0f)},
            {Rml::PropertyId::PaddingLeft, rml_dp(8.0f)},
            {Rml::PropertyId::AlignItems, Rml::Style::AlignItems::FlexStart},
            {Rml::PropertyId::OverflowY, Rml::Style::Overflow::Auto},
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

WindowTab* Window::add_tab(
    std::string_view id, std::string_view label, std::function<void()> selectedCallback) {
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
                                  {Rml::PropertyId::Display, Rml::Style::Display::Flex},
                                  {Rml::PropertyId::FlexGrow, rml_number(1.0f)},
                                  {Rml::PropertyId::FlexShrink, rml_number(1.0f)},
                                  {Rml::PropertyId::FlexBasis, rml_px(0.0f)},
                                  {Rml::PropertyId::MinWidth, rml_px(0.0f)},
                              });
        set_props(m_rightPane, {
                                   {Rml::PropertyId::Display, Rml::Style::Display::Flex},
                                   {Rml::PropertyId::FlexGrow, rml_number(0.0f)},
                                   {Rml::PropertyId::FlexShrink, rml_number(0.0f)},
                                   {Rml::PropertyId::FlexBasis, rml_percent(40.0f)},
                                   {Rml::PropertyId::MinWidth, rml_px(0.0f)},
                               });
    } else {
        set_props(
            m_leftPane, {
                            {Rml::PropertyId::Display, Rml::Style::Display::Flex},
                            {Rml::PropertyId::FlexGrow, rml_number(1.0f)},
                            {Rml::PropertyId::FlexShrink, rml_number(1.0f)},
                            {Rml::PropertyId::FlexBasis, Rml::Style::LengthPercentageAuto::Auto},
                            {Rml::PropertyId::Width, rml_percent(100.0f)},
                        });
        set_props(m_rightPane, {
                                   {Rml::PropertyId::Display, Rml::Style::Display::None},
                               });
    }
}

void Window::apply_close_style() {
    using namespace theme;

    if (m_closeButton == nullptr) {
        return;
    }

    const bool active = m_closeHovered || m_closeFocused;
    m_closeButton->SetProperty(Rml::PropertyId::BackgroundColor,
        active ? rml_color(WindowAccent, 56) : rml_color(Transparent));
    set_focus_border_visible(m_closeButton, m_closeFocused);
}
}  // namespace dusk::ui
