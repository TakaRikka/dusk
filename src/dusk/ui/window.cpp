#include "window.hpp"

#include "aurora/lib/window.hpp"
#include "aurora/rmlui.hpp"
#include "magic_enum.hpp"
#include "tab_button.hpp"
#include "ui.hpp"

#include <algorithm>
#include <cmath>

namespace dusk::ui {
namespace {

float base_body_padding(Rml::Context* context) noexcept {
    if (context == nullptr) {
        return 64.0f;
    }
    const float dpRatio = std::max(context->GetDensityIndependentPixelRatio(), 0.001f);
    const float heightDp = static_cast<float>(context->GetDimensions().y) / dpRatio;
    if (heightDp <= 640.0f) {
        return 16.0f * dpRatio;
    }
    return 64.0f * dpRatio;
}

}  // namespace

Window::Window() : Document("res/rml/window.rml") {
    listen(Rml::EventId::Keydown, [this](Rml::Event& event) {
        // 1-9 for quick switching tabs
        const auto key = static_cast<Rml::Input::KeyIdentifier>(
            event.GetParameter<int>("key_identifier", Rml::Input::KI_UNKNOWN));
        if (key >= Rml::Input::KeyIdentifier::KI_1 && key <= Rml::Input::KeyIdentifier::KI_9) {
            if (set_active_tab(key - Rml::Input::KeyIdentifier::KI_1)) {
                if (!mContentComponents.empty()) {
                    mContentComponents.front()->focus();
                }
                event.StopPropagation();
            }
        }
    });
}

void Window::update() {
    update_safe_area();
    for (const auto& component : mContentComponents) {
        component->update();
    }
    Document::update();
}

void Window::update_safe_area() noexcept {
    if (mDocument == nullptr) {
        return;
    }

    Rml::Context* context = mDocument->GetContext();
    const float basePadding = base_body_padding(context);
    Insets safeInsets = safe_area_insets(context);
    safeInsets = {
        std::round(std::max(basePadding, safeInsets.top)),
        std::round(std::max(basePadding, safeInsets.right)),
        std::round(std::max(basePadding, safeInsets.bottom)),
        std::round(std::max(basePadding, safeInsets.left)),
    };
    if (safeInsets == mBodyPadding) {
        return;
    }

    mBodyPadding = safeInsets;
    mDocument->SetProperty(
        Rml::PropertyId::PaddingTop, Rml::Property(safeInsets.top, Rml::Unit::PX));
    mDocument->SetProperty(
        Rml::PropertyId::PaddingRight, Rml::Property(safeInsets.right, Rml::Unit::PX));
    mDocument->SetProperty(
        Rml::PropertyId::PaddingBottom, Rml::Property(safeInsets.bottom, Rml::Unit::PX));
    mDocument->SetProperty(
        Rml::PropertyId::PaddingLeft, Rml::Property(safeInsets.left, Rml::Unit::PX));
}

bool Window::set_active_tab(int index) {
    if (index < 0 || index >= mTabs.size() || index == mSelectedTabIndex) {
        return false;
    }
    const auto& tab = mTabs[index];
    if (tab.button->focus()) {
        clear_content();
        std::vector<Button*> buttons;
        buttons.reserve(mTabs.size());
        for (auto& tab : mTabs) {
            buttons.push_back(tab.button.get());
        }
        set_selected_tab(buttons, index);
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
    if (index == mSelectedTabIndex && builder) {
        builder(mDocument->GetElementById("content"));
    }
    auto* tabBar = mDocument->GetElementById("tab-bar");
    mTabs.emplace_back(Tab{
        .title = title,
        .button = create_tab_button(
            tabBar, title, index == mSelectedTabIndex, [this, index](Rml::Event&) {
                set_active_tab(index);
            }),
        .builder = std::move(builder),
    });
}

void Window::clear_content() noexcept {
    mContentComponents.clear();
    auto* content = mDocument->GetElementById("content");
    while (content->GetNumChildren() != 0) {
        content->RemoveChild(content->GetFirstChild());
    }
}

bool Window::focus() {
    if (mTabs.empty()) {
        return false;
    }
    int i = mSelectedTabIndex;
    if (i < 0 || i >= mTabs.size()) {
        i = 0;
    }
    return mTabs[i].button->focus();
}

bool Window::handle_nav_command(Rml::Event& event, NavCommand cmd) {
    auto* target = event.GetTargetElement();
    if (cmd != NavCommand::Next && cmd != NavCommand::Previous && target->Closest(".content")) {
        if (handle_content_nav(event, cmd)) {
            return true;
        }
    }
    if (handle_tab_bar_nav(event, cmd)) {
        return true;
    }
    return false;
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
        return focus();
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
