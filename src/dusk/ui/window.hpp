#pragma once

#include <RmlUi/Core/DataModelHandle.h>
#include <RmlUi/Core/ElementDocument.h>

#include "button.hpp"
#include "component.hpp"
#include "ui.hpp"

namespace dusk::ui {

class Window {
public:
    using TabBuilder = std::function<void(Rml::Element*)>;
    struct Tab {
        Rml::String title;
        std::unique_ptr<Button> button;
        TabBuilder builder;
    };
    struct Insets {
        float top = 0.0f;
        float right = 0.0f;
        float bottom = 0.0f;
        float left = 0.0f;

        bool operator==(const Insets& other) const noexcept {
            return top == other.top && right == other.right && bottom == other.bottom &&
                   left == other.left;
        }
    };

    Window();
    ~Window();

    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    void show();
    void hide();

    void update();
    bool set_active_tab(int index);

protected:
    void add_tab(const Rml::String& title, TabBuilder builder);
    void update_safe_area() noexcept;
    void clear_content() noexcept;
    bool focus_active_tab() noexcept;
    bool handle_tab_bar_nav(Rml::Event& event, NavCommand cmd) noexcept;
    bool handle_content_nav(Rml::Event& event, NavCommand cmd) noexcept;

    template <typename T, typename... Args>
    requires std::is_base_of_v<Component, T> T& add_child(Args&&... args) {
        auto child = std::make_unique<T>(std::forward<Args>(args)...);
        T& ref = *child;
        mContentComponents.emplace_back(std::move(child));
        return ref;
    }

    Rml::ElementDocument* mDocument = nullptr;
    std::vector<Tab> mTabs;
    std::vector<std::unique_ptr<Component> > mContentComponents;
    Insets mBodyPadding;
    int mSelectedTabIndex = 0;
    std::unique_ptr<ScopedEventListener> mKeyListener;
};

}  // namespace dusk::ui
