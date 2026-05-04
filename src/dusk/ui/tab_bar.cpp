#include "tab_bar.hpp"

namespace dusk::ui {
namespace {

Rml::Element* createRoot(Rml::Element* parent) {
    auto* doc = parent->GetOwnerDocument();
    auto elem = doc->CreateElement("tab-bar");
    return parent->AppendChild(std::move(elem));
}

int key_modifiers_from_event(const Rml::Event& event) {
    int modifiers = 0;
    if (event.GetParameter("ctrl_key", 0)) {
        modifiers |= Rml::Input::KM_CTRL;
    }
    if (event.GetParameter("shift_key", 0)) {
        modifiers |= Rml::Input::KM_SHIFT;
    }
    if (event.GetParameter("alt_key", 0)) {
        modifiers |= Rml::Input::KM_ALT;
    }
    if (event.GetParameter("meta_key", 0)) {
        modifiers |= Rml::Input::KM_META;
    }
    if (event.GetParameter("caps_lock_key", 0)) {
        modifiers |= Rml::Input::KM_CAPSLOCK;
    }
    if (event.GetParameter("num_lock_key", 0)) {
        modifiers |= Rml::Input::KM_NUMLOCK;
    }
    if (event.GetParameter("scroll_lock_key", 0)) {
        modifiers |= Rml::Input::KM_SCROLLLOCK;
    }
    return modifiers;
}

}  // namespace

TabBar::TabBar(Rml::Element* parent, Props props)
    : FluentComponent(createRoot(parent)), mProps(std::move(props)) {
    if (mProps.onClose) {
        mRoot->SetAttribute("closable", "");
        auto& closeButton =
            add_child<Button>(Button::Props{}, "close").on_pressed([this] { mProps.onClose(); });
        closeButton.root()->SetInnerRML("&#xe5cd;");
        mEndSpacer = append(mRoot, "tab-end-spacer");
    }

    listen(Rml::EventId::Keydown, [this](Rml::Event& event) {
        const auto cmd = map_nav_event(event);
        if (cmd != NavCommand::None && handle_nav_command(event, cmd)) {
            event.StopPropagation();
        }
    });

    // Remap vertical scroll events into horizontal scroll events
    listen(Rml::EventId::Mousescroll, [this](Rml::Event& event) {
        if (mRedirectingScroll) {
            return;
        }
        if (mRoot->GetScrollWidth() <= mRoot->GetClientWidth() + 0.5f) {
            return;
        }

        const float wheelDeltaX = event.GetParameter("wheel_delta_x", 0.0f);
        const float wheelDeltaY = event.GetParameter("wheel_delta_y", 0.0f);
        const float absWheelDeltaX = wheelDeltaX < 0.0f ? -wheelDeltaX : wheelDeltaX;
        const float absWheelDeltaY = wheelDeltaY < 0.0f ? -wheelDeltaY : wheelDeltaY;
        if (absWheelDeltaY == 0.0f || absWheelDeltaX >= absWheelDeltaY) {
            return;
        }

        auto* context = mRoot->GetContext();
        if (context == nullptr) {
            return;
        }

        mRedirectingScroll = true;
        context->ProcessMouseWheel({wheelDeltaY, 0.0f}, key_modifiers_from_event(event));
        mRedirectingScroll = false;
        event.StopPropagation();
    });
}

bool TabBar::focus() {
    if (mProps.selectedTabIndex >= 0 && mProps.selectedTabIndex < mTabs.size()) {
        // Try to focus the currently selected tab
        if (mTabs[mProps.selectedTabIndex].button.focus()) {
            return true;
        }
    }
    // Otherwise, focus the first enabled tab
    for (const auto& tab : mTabs) {
        if (tab.button.focus()) {
            return true;
        }
    }
    return false;
}

void TabBar::add_tab(const Rml::String& title, TabCallback callback) {
    const int index = static_cast<int>(mTabs.size());
    const bool selected = index == mProps.selectedTabIndex;
    if (selected && callback) {
        callback();
    }
    auto& button = add_child<Button>(Button::Props{title}, "tab");
    button.on_pressed([this, index] { set_active_tab(index); });
    if (selected) {
        button.set_selected(true);
    }
    if (mEndSpacer != nullptr) {
        auto spacer = mRoot->RemoveChild(mEndSpacer);
        mEndSpacer = mRoot->AppendChild(std::move(spacer));
    }
    mTabs.emplace_back(Tab{
        .title = title,
        .button = button,
        .callback = std::move(callback),
    });
}

bool TabBar::set_active_tab(int index) {
    if (index == -1) {
        // Clear currently selected tab
        for (auto& tab : mTabs) {
            tab.button.set_selected(false);
        }
        mProps.selectedTabIndex = -1;
        return true;
    }

    if (index < 0 || index >= mTabs.size() || index == mProps.selectedTabIndex) {
        return false;
    }
    const auto& tab = mTabs[index];
    if (tab.button.focus()) {
        for (int i = 0; i < static_cast<int>(mTabs.size()); ++i) {
            mTabs[i].button.set_selected(i == index);
        }
        mProps.selectedTabIndex = index;
        if (tab.callback) {
            tab.callback();
        }
        return true;
    }
    return false;
}

bool TabBar::focus_tab(int index) {
    if (index < 0 || index >= mTabs.size() || index == mProps.selectedTabIndex) {
        return false;
    }
    return mTabs[index].button.focus();
}

int TabBar::tab_containing(Rml::Element* element) const {
    for (int i = 0; i < mTabs.size(); ++i) {
        if (mTabs[i].button.contains(element)) {
            return i;
        }
    }
    return -1;
}

bool TabBar::handle_nav_command(Rml::Event& event, NavCommand cmd) {
    if (cmd == NavCommand::Left || cmd == NavCommand::Right || cmd == NavCommand::Next ||
        cmd == NavCommand::Previous)
    {
        bool isNext = cmd == NavCommand::Right || cmd == NavCommand::Next;
        int currentComponent = tab_containing(event.GetTargetElement());
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
        while (i >= 0 && i < mTabs.size()) {
            if (mProps.autoSelect ? set_active_tab(i) : focus_tab(i)) {
                return true;
            }
            i += direction;
        }
    }
    return false;
}

}  // namespace dusk::ui
