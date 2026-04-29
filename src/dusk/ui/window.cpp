#include "window.hpp"

#include <RmlUi/Core.h>

#include "aurora/rmlui.hpp"
#include "button.hpp"

namespace dusk::ui {

Window::Window() {
    auto* context = aurora::rmlui::get_context();
    if (context == nullptr) {
        return;
    }

    mDocument = context->LoadDocument("res/rml/window.rml");
    if (mDocument == nullptr) {
        return;
    }
}

Window::~Window() {
    auto* context = aurora::rmlui::get_context();
    if (context != nullptr && mDocument != nullptr) {
        context->UnloadDocument(mDocument);
        mDocument = nullptr;
    }
}

void Window::show() {
    if (mDocument != nullptr) {
        mDocument->Show();
    }
}

void Window::hide() {
    if (mDocument != nullptr) {
        mDocument->Hide();
    }
}

void Window::update() {
    for (const auto& component : mContentComponents) {
        component->update();
    }
}

void Window::set_active_tab(int index) {
    if (index < 0 || index >= mTabs.size() || index == mSelectedTabIndex) {
        return;
    }
    clear_content();
    for (int i = 0; i < mTabs.size(); i++) {
        mTabs[i].button->set_selected(i == index);
    }
    mSelectedTabIndex = index;
    const auto& tab = mTabs[index];
    if (tab.builder) {
        tab.builder(mDocument->GetElementById("content"));
    }
}

void Window::add_tab(const Rml::String& title, TabBuilder builder) {
    const int index = static_cast<int>(mTabs.size());
    auto* tabBar = mDocument->GetElementById("tab-bar");
    mTabs.emplace_back(Tab{
        .title = title,
        .button = std::make_unique<Button>(tabBar,
            Button::Props{
                .text = title,
                .onPressed = [this, index](Rml::Event&) { set_active_tab(index); },
                .selected = index == mSelectedTabIndex,
            },
            "tab"),
        .builder = std::move(builder),
    });
    if (index == mSelectedTabIndex && builder) {
        builder(mDocument->GetElementById("content"));
    }
}

void Window::clear_content() noexcept {
    mContentComponents.clear();
    auto* content = mDocument->GetElementById("content");
    while (content->GetNumChildren() != 0) {
        content->RemoveChild(content->GetFirstChild());
    }
}

}  // namespace dusk::ui
