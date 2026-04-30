#include "window.hpp"

#include <RmlUi/Core.h>

#include "aurora/rmlui.hpp"
#include "button.hpp"
#include "magic_enum.hpp"
#include "ui.hpp"

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

    mKeyListener = std::make_unique<ScopedEventListener>(
        mDocument, Rml::EventId::Keydown, [this](Rml::Event& event) {
            // 1-9 for quick switching tabs
            const auto key = static_cast<Rml::Input::KeyIdentifier>(
                event.GetParameter<int>("key_identifier", Rml::Input::KI_UNKNOWN));
            if (key >= Rml::Input::KeyIdentifier::KI_1 && key <= Rml::Input::KeyIdentifier::KI_9) {
                if (set_active_tab(key - Rml::Input::KeyIdentifier::KI_1)) {
                    if (!mContentComponents.empty()) {
                        mContentComponents.front()->focus();
                    }
                    event.StopPropagation();
                    return;
                }
            }

            const auto cmd = map_nav_event(event);
            if (cmd == NavCommand::None) {
                return;
            }
            auto* target = event.GetTargetElement();
            if (cmd == NavCommand::Next || cmd == NavCommand::Previous ||
                target->Closest(".tab-bar")) {
                if (handle_tab_bar_nav(event, cmd)) {
                    event.StopPropagation();
                }
            } else if (target->Closest(".content")) {
                if (handle_content_nav(event, cmd)) {
                    event.StopPropagation();
                }
            }
        });
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

bool Window::set_active_tab(int index) {
    if (index < 0 || index >= mTabs.size() || index == mSelectedTabIndex) {
        return false;
    }
    const auto& tab = mTabs[index];
    if (tab.button->focus()) {
        clear_content();
        for (int i = 0; i < mTabs.size(); i++) {
            mTabs[i].button->set_selected(i == index);
        }
        mSelectedTabIndex = index;
        if (tab.builder) {
            tab.builder(mDocument->GetElementById("content"));
        }
        return true;
    }
    return false;
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
        focus_active_tab();
    }
}

void Window::clear_content() noexcept {
    mContentComponents.clear();
    auto* content = mDocument->GetElementById("content");
    while (content->GetNumChildren() != 0) {
        content->RemoveChild(content->GetFirstChild());
    }
}

bool Window::focus_active_tab() noexcept {
    if (mTabs.empty()) {
        return false;
    }
    int i = mSelectedTabIndex;
    if (i < 0 || i >= mTabs.size()) {
        i = 0;
    }
    return mTabs[i].button->focus();
}

bool Window::handle_tab_bar_nav(Rml::Event& event, NavCommand cmd) noexcept {
    if (cmd == NavCommand::Down) {
        if (!mContentComponents.empty()) {
            return mContentComponents.front()->focus();
        }
    } else if (cmd == NavCommand::Left || cmd == NavCommand::Right || cmd == NavCommand::Next ||
               cmd == NavCommand::Previous)
    {
        bool isNext = cmd == NavCommand::Right || cmd == NavCommand::Next;
        int currentComponent = -1;
        for (int i = 0; i < mTabs.size(); ++i) {
            if (mTabs[i].button->contains(event.GetTargetElement())) {
                currentComponent = i;
                break;
            }
        }
        int direction = isNext ? 1 : -1;
        int i = currentComponent + direction;
        if (currentComponent == -1) {
            // If the container itself is focused and right is pressed, focus the first element
            if (isNext) {
                i = 0;
            } else {
                // Otherwise, allow event to bubble
                return false;
            }
        }
        while (i >= 0 && i < static_cast<int>(mTabs.size())) {
            if (set_active_tab(i)) {
                return true;
            }
            i += direction;
        }
    } else if (cmd == NavCommand::Cancel) {
        // TODO: close window
    } else if (cmd == NavCommand::Confirm) {
        if (!mContentComponents.empty()) {
            return mContentComponents.front()->focus();
        }
    }
    return false;
}

bool Window::handle_content_nav(Rml::Event& event, NavCommand cmd) noexcept {
    if (cmd == NavCommand::Up || cmd == NavCommand::Cancel) {
        return focus_active_tab();
    } else if (cmd == NavCommand::Left || cmd == NavCommand::Right) {
        int currentComponent = -1;
        for (int i = 0; i < mContentComponents.size(); ++i) {
            if (mContentComponents[i]->contains(event.GetTargetElement())) {
                currentComponent = i;
                break;
            }
        }
        int direction = cmd == NavCommand::Right ? 1 : -1;
        int i = currentComponent + direction;
        if (currentComponent == -1) {
            if (cmd == NavCommand::Right) {
                return mContentComponents.front()->focus();
            }
        } else if (i >= 0 && i < mContentComponents.size()) {
            return mContentComponents[i]->focus();
        }
    }
    return false;
}

}  // namespace dusk::ui
