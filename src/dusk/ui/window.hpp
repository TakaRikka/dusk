#pragma once

#include "button.hpp"
#include "component.hpp"
#include "document.hpp"
#include "nav_types.hpp"
#include "tab_bar.hpp"
#include "ui.hpp"

namespace dusk::ui {

class Window : public Document {
public:
    using TabBuilder = std::function<void(Rml::Element*)>;
    struct Tab {
        Rml::String title;
        std::unique_ptr<Button> button;
        TabBuilder builder;
    };
    struct Props {
        bool tabBar = true;
        std::vector<Rml::String> styleSheets;
    };

    Window() : Window(Props{}) {}
    explicit Window(Props props);

    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    void show() override;
    void hide(bool close) override;
    void update() override;
    bool focus() override;
    bool visible() const override;
    bool set_active_tab(int index);

protected:
    void request_close();
    virtual bool consume_close_request();
    void add_tab(const Rml::String& title, TabBuilder builder);
    // Tab-bar-less counterpart of add_tab: stores the builder and runs it immediately.
    void set_content(TabBuilder builder);
    void rebuild_content();
    void refresh_active_tab();
    void update_safe_area() noexcept;
    void clear_content() noexcept;
    bool handle_nav_command(Rml::Event& event, NavCommand cmd) override;
    bool handle_content_nav(Rml::Event& event, NavCommand cmd) noexcept;

    template <typename T, typename... Args>
        requires std::is_base_of_v<Component, T>
    T& add_child(Args&&... args) {
        auto child = std::make_unique<T>(std::forward<Args>(args)...);
        T& ref = *child;
        mContentComponents.emplace_back(std::move(child));
        return ref;
    }

    Rml::Element* mRoot;
    Rml::Element* mContentRoot;
    std::unique_ptr<TabBar> mTabBar;
    // Only set for tab-bar-less windows.
    std::unique_ptr<Button> mCloseButton;
    TabBuilder mContentBuilder;
    std::vector<std::unique_ptr<Component>> mContentComponents;
    Insets mBodyPadding;
    bool mInitialOpen = true;
};

// Shared shell for small-style windows such as Modal and PresetWindow
class WindowSmall : public Document {
public:
    WindowSmall(const Rml::String& windowClass, const Rml::String& dialogClass);

    void show() override;
    void hide(bool close) override;
    bool visible() const override;
    Rml::Element* root() { return mRoot;}

protected:
    Rml::Element* mRoot = nullptr;
    Rml::Element* mDialog = nullptr;
};

}  // namespace dusk::ui
