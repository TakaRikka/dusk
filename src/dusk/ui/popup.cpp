#include "popup.hpp"

#include <RmlUi/Core.h>

#include "aurora/rmlui.hpp"
#include "editor.hpp"
#include "settings.hpp"
#include "ui.hpp"
#include "window.hpp"

#include <chrono>

namespace dusk::ui {

Popup::Popup() : Document("res/rml/popup.rml"), mRoot(mDocument->GetElementById("popup")) {
    mTabBar = std::make_unique<TabBar>(mRoot, TabBar::Props{.autoSelect = false});
    mTabBar->add_tab("Settings", [this] {
        hide();
        // TODO: make this better
        auto& settingsWindow = add_document(std::make_unique<SettingsWindow>());
        settingsWindow.show();
    });
    mTabBar->add_tab("Warp", [] {
        // TODO
    });
    mTabBar->add_tab("Editor", [this] {
        hide();
        // TODO: make this better
        auto& editorWindow = add_document(std::make_unique<EditorWindow>());
        editorWindow.show();
    });
    mTabBar->add_tab("Reset", [] {
        // TODO
    });
    mTabBar->add_tab("Exit", [] {
        // TODO
    });
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
    Document::update();
}

bool Popup::focus() {
    return mTabBar->focus();
}

}  // namespace dusk::ui
