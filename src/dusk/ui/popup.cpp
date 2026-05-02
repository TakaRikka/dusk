#include "popup.hpp"

#include <RmlUi/Core.h>

#include "aurora/rmlui.hpp"
#include "dusk/main.h"
#include "editor.hpp"
#include "imgui.h"
#include "settings.hpp"
#include "ui.hpp"
#include "window.hpp"

#include <chrono>
#include <cmath>

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
        hide(false);
    });
    mTabBar->add_tab("Exit", [] { IsRunning = false; });

    // Hide document after transition completion
    listen(mRoot, Rml::EventId::Transitionend, [this](Rml::Event& event) {
        if (event.GetTargetElement() == mRoot && !mRoot->HasAttribute("open") &&
            Document::visible())
        {
            Document::hide(mPendingClose);
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

void Popup::hide(bool close) {
    mRoot->RemoveAttribute("open");
    if (close) {
        mPendingClose = true;
    }
}

void Popup::update() {
    update_safe_area();
    Document::update();
}

void Popup::update_safe_area() noexcept {
    if (mDocument == nullptr || mTabBar == nullptr) {
        return;
    }

    // Avoid ImGui menu bar if shown
    if (const auto* viewport = ImGui::GetMainViewport();
        viewport != nullptr && mTopMargin != viewport->WorkPos.y)
    {
        mTopMargin = viewport->WorkPos.y;
        mRoot->SetProperty(Rml::PropertyId::MarginTop, Rml::Property(mTopMargin, Rml::Unit::DP));
    }

    Rml::Context* context = mDocument->GetContext();
    Insets safeInsets = safe_area_insets(context);
    safeInsets = {
        0.0f,
        std::round(safeInsets.right),
        0.0f,
        std::round(safeInsets.left),
    };
    if (safeInsets == mTabBarPadding) {
        return;
    }

    mTabBarPadding = safeInsets;
    auto* tabBar = mTabBar->root();
    tabBar->SetProperty(
        Rml::PropertyId::PaddingRight, Rml::Property(safeInsets.right, Rml::Unit::PX));
    tabBar->SetProperty(
        Rml::PropertyId::PaddingLeft, Rml::Property(safeInsets.left, Rml::Unit::PX));
}

bool Popup::visible() const {
    return mRoot->HasAttribute("open");
}

bool Popup::handle_nav_command(Rml::Event& event, NavCommand cmd) {
    if (cmd == NavCommand::Cancel) {
        hide(false);
        return true;
    }
    return Document::handle_nav_command(event, cmd);
}

bool Popup::focus() {
    return mTabBar->focus();
}

}  // namespace dusk::ui
