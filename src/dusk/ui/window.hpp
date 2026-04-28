#pragma once

#include <RmlUi/Core/EventListener.h>

#include <functional>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

namespace Rml {
class Element;
}

namespace dusk::ui {
class WindowTab : public Rml::EventListener {
public:
    WindowTab(Rml::Element* parent, std::string_view id, std::string_view label, std::function<void()> selectedCallback);
    ~WindowTab() override;

    WindowTab(const WindowTab&) = delete;
    WindowTab& operator=(const WindowTab&) = delete;

    void ProcessEvent(Rml::Event& event) override;

    Rml::Element* element() const { return m_element; }
    std::string id() const;

    void set_selected(bool selected);
    bool is_selected() const { return m_selected; }

private:
    Rml::Element* m_element = nullptr;
    Rml::Element* m_label = nullptr;
    Rml::Element* m_indicator = nullptr;
    std::function<void()> m_selectedCallback;
    bool m_hovered = false;
    bool m_focused = false;
    bool m_selected = false;

    void apply_style();
};

class Window : public Rml::EventListener {
public:
    Window(Rml::Element* parent, std::string_view id, std::function<void()> closeCallback = {});
    ~Window() override;

    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    void ProcessEvent(Rml::Event& event) override;

    Rml::Element* element() const { return m_element; }
    Rml::Element* body() const { return m_leftPane; }
    Rml::Element* right_pane() const { return m_rightPane; }
    void set_right_pane_visible(bool visible);

    Rml::Element* tab_strip() const { return m_tabStrip; }

    WindowTab* add_tab(std::string_view id, std::string_view label, std::function<void()> selectedCallback);
    void set_selected_tab(std::string_view id);
    std::string selected_tab_id() const;

private:
    Rml::Element* m_element = nullptr;
    Rml::Element* m_tabBar = nullptr;
    Rml::Element* m_tabStrip = nullptr;
    Rml::Element* m_closeButton = nullptr;
    Rml::Element* m_contentRow = nullptr;
    Rml::Element* m_leftPane = nullptr;
    Rml::Element* m_rightPane = nullptr;
    bool m_rightPaneVisible = false;
    std::function<void()> m_closeCallback;
    std::vector<std::unique_ptr<WindowTab> > m_tabs;
    bool m_closeHovered = false;
    bool m_closeFocused = false;

    void apply_close_style();
    void apply_pane_layout();
};
}  // namespace dusk::ui
