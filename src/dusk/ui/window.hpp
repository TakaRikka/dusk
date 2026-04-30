#pragma once

#include "button.hpp"
#include "component.hpp"
#include "document.hpp"
#include "ui.hpp"
#include "nav_types.hpp"

namespace dusk::ui {

class Window : public Document {
public:
    using TabBuilder = std::function<void(Rml::Element*)>;
    struct Tab {
        Rml::String title;
        std::unique_ptr<Button> button;
        TabBuilder builder;
    };

    Window();

    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    void update() override;
    bool focus() override;
    bool set_active_tab(int index);

protected:
    void add_tab(const Rml::String& title, TabBuilder builder);
    void update_safe_area() noexcept;
    void clear_content() noexcept;
    bool handle_nav_command(Rml::Event& event, NavCommand cmd) override;
    bool handle_tab_bar_nav(Rml::Event& event, NavCommand cmd) noexcept;
    bool handle_content_nav(Rml::Event& event, NavCommand cmd) noexcept;

    template <typename T, typename... Args>
    requires std::is_base_of_v<Component, T> T& add_child(Args&&... args) {
        auto child = std::make_unique<T>(std::forward<Args>(args)...);
        T& ref = *child;
        mContentComponents.emplace_back(std::move(child));
        return ref;
    }

    std::vector<Tab> mTabs;
    std::vector<std::unique_ptr<Component> > mContentComponents;
    Insets mBodyPadding;
    int mSelectedTabIndex = 0;
    std::unique_ptr<ScopedEventListener> mKeyListener;
};

}  // namespace dusk::ui
