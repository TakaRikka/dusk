#include "popup.hpp"

#include <RmlUi/Core.h>

#include "aurora/rmlui.hpp"
#include "editor.hpp"
#include "settings.hpp"
#include "ui.hpp"
#include "window.hpp"

#include <chrono>

#include "dusk/main.h"

namespace dusk::ui {
namespace {

const Rml::String kDocumentSource = R"RML(
<rml>
<head>
    <link type="text/rcss" href="res/rml/tabbing.rcss" />
    <link type="text/rcss" href="res/rml/popup.rcss" />
</head>
<body>
    <popup id="popup"></div>
</body>
</rml>
)RML";

}

Popup::Popup() : Document(kDocumentSource), mRoot(mDocument->GetElementById("popup")) {
    mTabBar = std::make_unique<TabBar>(mRoot, TabBar::Props{.autoSelect = false});
    mTabBar->add_tab("Settings", [] { push_document(std::make_unique<SettingsWindow>()); });
    mTabBar->add_tab("Warp", [] {
        // TODO
    });
    mTabBar->add_tab("Editor", [] { push_document(std::make_unique<EditorWindow>()); });
    mTabBar->add_tab("Reset", [this] {
        JUTGamePad::C3ButtonReset::sResetSwitchPushing = true;
        mTabBar->set_active_tab(-1);
        hide();
    });
    mTabBar->add_tab("Exit", [] { IsRunning = false; });

    // Hide document after transition completion
    listen(mRoot, Rml::EventId::Transitionend, [this](Rml::Event& event) {
        if (event.GetTargetElement() == mRoot &&
            *mRoot->GetProperty(Rml::PropertyId::Visibility) == Rml::Style::Visibility::Visible &&
            !mRoot->HasAttribute("open"))
        {
            Document::hide();
        }
    });

    // We start hidden, but want focus for an open nav event
    mDocument->Focus();
}

void Popup::show() {
    Document::show();
    mRoot->SetAttribute("open", "");
    mTabBar->set_active_tab(-1);
}

void Popup::hide() {
    mRoot->RemoveAttribute("open");
}

void Popup::toggle() {
    if (visible()) {
        hide();
    } else {
        show();
    }
}

bool Popup::visible() const {
    return mRoot->HasAttribute("open");
}

bool Popup::handle_nav_command(Rml::Event& event, NavCommand cmd) {
    if (cmd == NavCommand::Cancel) {
        hide();
        return true;
    }
    return Document::handle_nav_command(event, cmd);
}

bool Popup::focus() {
    return mTabBar->focus();
}

}  // namespace dusk::ui
