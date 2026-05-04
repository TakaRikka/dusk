#include "popup.hpp"

#include <RmlUi/Core.h>

#include "Z2AudioLib/Z2SeMgr.h"
#include "m_Do/m_Do_audio.h"

#include "aurora/rmlui.hpp"
#include "dusk/main.h"
#include "dusk/settings.h"
#include "f_pc/f_pc_manager.h"
#include "f_pc/f_pc_name.h"
#include "achievements.hpp"
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
    <popup id="popup"></popup>
</body>
</rml>
)RML";

}

Popup::Popup() : Document(kDocumentSource), mRoot(mDocument->GetElementById("popup")) {
    mTabBar = std::make_unique<TabBar>(mRoot, TabBar::Props{
                                                  .onClose = [this] { hide(false); },
                                                  .autoSelect = false,
                                              });
    mTabBar->add_tab("Settings", [this] { push(std::make_unique<SettingsWindow>()); });
    // mTabBar->add_tab("Warp", [] {
    //     // TODO
    // });
    mTabBar->add_tab("Editor", [this] { push(std::make_unique<EditorWindow>()); });
    mTabBar->add_tab("Achievements", [this] { push(std::make_unique<AchievementsWindow>()); });
    mTabBar->add_tab("Reset", [this] {
        mTabBar->set_active_tab(-1);
        if (fpcM_SearchByName(fpcNm_LOGO_SCENE_e)) {
            return;
        }
        JUTGamePad::C3ButtonReset::sResetSwitchPushing = true;
        hide(false);
    });
    mTabBar->add_tab("Quit", [] { IsRunning = false; });


    listen(mRoot, Rml::EventId::Transitionend, [this](Rml::Event& event) {
        if (event.GetTargetElement() == mRoot && !mRoot->HasAttribute("open") &&
            Document::visible() && mPendingClose)
        {
            Document::hide(true);
        }
    });

    // We start hidden, but want focus for an open nav event
    mDocument->Focus();
}

void Popup::show() {
    Document::show();
    mRoot->SetAttribute("open", "");
    mTabBar->set_active_tab(-1);
    if (!mTabBar->focus_tab(mFocusedTabIndex)) {
        mTabBar->focus();
    }
}

void Popup::hide(bool close) {
    mFocusedTabIndex = mTabBar->focused_tab_index();
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
    if (auto* close = tabBar->QuerySelector("close")) {
        close->SetProperty(Rml::PropertyId::Right,
            Rml::Property(safeInsets.right + 8.0f * context->GetDensityIndependentPixelRatio(),
                Rml::Unit::PX));
    }
}

bool Popup::visible() const {
    return mRoot->HasAttribute("open");
}

bool Popup::handle_nav_command(Rml::Event& event, NavCommand cmd) {
    if (!getSettings().backend.wasPresetChosen) {
        return true;
    }
    if (cmd == NavCommand::Cancel && visible()) {
        mDoAud_seStartMenu(Z2SE_SY_MENU_OUT);
        hide(false);
        return true;
    }
    return Document::handle_nav_command(event, cmd);
}

bool Popup::focus() {
    return mTabBar->focus();
}

}  // namespace dusk::ui
