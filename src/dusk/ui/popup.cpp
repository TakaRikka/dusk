#include "popup.hpp"

#include <RmlUi/Core.h>

#include "aurora/rmlui.hpp"
#include "editor.hpp"
#include "settings.hpp"
#include "tab_button.hpp"
#include "ui.hpp"
#include "window.hpp"

#include <algorithm>
#include <array>
#include <chrono>

namespace dusk::ui {

Popup::Popup() : Document("res/rml/popup.rml") {
    auto* tabBar = mDocument->GetElementById("tab-bar");
    if (tabBar == nullptr) {
        return;
    }

    const std::array<Rml::String, 5> tabLabels = {
        "Settings",
        "Warp",
        "Editor",
        "Reset",
        "Exit",
    };

    // TODO: Make warp, reset, and exit buttons work
    mTabActions = {
        [this] {
            hide();
            // TODO: make this better
            auto& settingsWindow = add_document(std::make_unique<SettingsWindow>());
            settingsWindow.show();
            set_selected_tab(0);
        },
        [this] { set_selected_tab(1); },
        [this] {
            hide();
            // TODO: make this better
            auto& editorWindow = add_document(std::make_unique<EditorWindow>());
            editorWindow.show();
            set_selected_tab(2);
        },
        [this] { set_selected_tab(3); },
        [this] { set_selected_tab(4); },
    };

    mTabs.reserve(tabLabels.size());
    for (int i = 0; i < static_cast<int>(tabLabels.size()); ++i) {
        mTabs.push_back(
            create_tab_button(tabBar, tabLabels[i], i == mSelectedTabIndex, [this, i](Rml::Event&) {
                if (i >= 0 && i < static_cast<int>(mTabActions.size())) {
                    mTabActions[i]();
                }
            }));
    }
}

void Popup::show() {
    if (mDocument == nullptr) {
        return;
    }

    mHideDeadline.reset();
    Document::show();
    mVisible = true;
}

void Popup::hide() {
    if (mDocument == nullptr) {
        mVisible = false;
        return;
    }

    if (auto* popup = mDocument->GetElementById("popup")) {
        popup->SetClass("popup-hidden", true);
        mHideDeadline =
            std::chrono::steady_clock::now() +
            std::chrono::milliseconds(500);  // Must match the transition duration in popup.rcss
    } else {
        Document::hide();
    }
    mVisible = false;
}

void Popup::toggle() {
    if (is_visible()) {
        hide();
    } else {
        show();
    }
}

bool Popup::is_visible() const {
    return mVisible;
}

bool Popup::handle_nav_command(Rml::Event& event, NavCommand cmd) {
    if (cmd == NavCommand::Left || cmd == NavCommand::Previous) {
        focus_tab(std::max(0, mSelectedTabIndex - 1));
        return true;
    }
    if (cmd == NavCommand::Right || cmd == NavCommand::Next) {
        focus_tab(std::min(static_cast<int>(mTabs.size()) - 1, mSelectedTabIndex + 1));
        return true;
    }
    if (cmd == NavCommand::Confirm && mSelectedTabIndex >= 0 &&
        mSelectedTabIndex < static_cast<int>(mTabActions.size()))
    {
        mTabActions[mSelectedTabIndex]();
        return true;
    }
    if (cmd == NavCommand::Cancel) {
        hide();
        return true;
    }
    return false;
}

void Popup::update() {
    if (mDocument == nullptr) {
        return;
    }
    if (mHideDeadline.has_value() && std::chrono::steady_clock::now() >= *mHideDeadline) {
        mDocument->Hide();
        mHideDeadline.reset();
    }
    if (mTabs.empty()) {
        return;
    }
    std::vector<Button*> tabs;
    tabs.reserve(mTabs.size());
    for (const auto& tab : mTabs) {
        tabs.push_back(tab.get());
    }
    ui::set_selected_tab(tabs, mSelectedTabIndex);
}

void Popup::set_selected_tab(int index) {
    if (index < 0 || index >= static_cast<int>(mTabs.size())) {
        return;
    }
    mSelectedTabIndex = index;
    std::vector<Button*> tabs;
    tabs.reserve(mTabs.size());
    for (const auto& tab : mTabs) {
        tabs.push_back(tab.get());
    }
    ui::set_selected_tab(tabs, mSelectedTabIndex);
}

bool Popup::focus_tab(int index) {
    if (index < 0 || index >= static_cast<int>(mTabs.size())) {
        return false;
    }
    set_selected_tab(index);
    return mTabs[index]->focus();
}

}  // namespace dusk::ui
