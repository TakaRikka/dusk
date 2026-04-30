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
    mTabBar->add_tab("Settings", [] { push_document(std::make_unique<SettingsWindow>()); });
    mTabBar->add_tab("Warp", [] {
        // TODO
    });
    mTabBar->add_tab("Editor", [] { push_document(std::make_unique<EditorWindow>()); });
    mTabBar->add_tab("Reset", [this] {
        // TODO
        mTabBar->set_active_tab(-1);
    });
    mTabBar->add_tab("Exit", [this] {
        // TODO
        mTabBar->set_active_tab(-1);
    });

    // Hide document after transition completion
    listen(mRoot, Rml::EventId::Transitionend, [this](Rml::Event& event) {
        if (event.GetTargetElement() == mRoot &&
            *mRoot->GetProperty(Rml::PropertyId::Visibility) == Rml::Style::Visibility::Visible &&
            !mVisible)
        {
            Document::hide();
        }
    });

    // We start hidden, but want focus for an open nav event
    mDocument->Focus();
}

void Popup::show() {
    if (mDocument == nullptr || mVisible) {
        return;
    }

    Document::show();
    mRoot->SetClass("popup-hidden", false);
    mTabBar->set_active_tab(-1);
    mVisible = true;
}

void Popup::hide() {
    if (mDocument == nullptr) {
        mVisible = false;
        return;
    }
    if (!mVisible) {
        return;
    }

    mRoot->SetClass("popup-hidden", true);
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
        toggle();
        return true;
    }
    return false;
}

bool Popup::focus() {
    return mTabBar->focus();
}

}  // namespace dusk::ui
