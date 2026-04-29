#pragma once

#include <RmlUi/Core/DataModelHandle.h>
#include <RmlUi/Core/ElementDocument.h>

#include "button.hpp"
#include "component.hpp"

namespace dusk::ui {

class Window {
public:
    using TabBuilder = std::function<void(Rml::Element*)>;
    struct Tab {
        Rml::String title;
        std::unique_ptr<Button> button;
        TabBuilder builder;
    };

    Window();
    ~Window();

    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    void show();
    void hide();

    void update();
    void set_active_tab(int index);

protected:
    void add_tab(const Rml::String& title, TabBuilder builder);
    void clear_content() noexcept;

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
    int mSelectedTabIndex = 0;
};

}  // namespace dusk::ui
