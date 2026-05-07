#include "window.hpp"

#include "aurora/lib/window.hpp"
#include "aurora/rmlui.hpp"
#include "magic_enum.hpp"
#include "pane.hpp"
#include "ui.hpp"

#include "Z2AudioLib/Z2SeMgr.h"
#include "m_Do/m_Do_audio.h"

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

const Rml::String kDocumentSource = R"RML(
<rml>
<head>
    <link type="text/rcss" href="res/rml/tabbing.rcss" />
    <link type="text/rcss" href="res/rml/window.rcss" />
</head>
<body>
    <window id="window"></window>
</body>
</rml>
)RML";

const Rml::String kDocumentSourceSmall = R"RML(
<rml>
<head>
    <link type="text/rcss" href="res/rml/window.rcss" />
</head>
<body>
    <window id="window" class="small">
        <div id="dialog"/>
    </window>
</body>
</rml>
)RML";

}  // namespace

Window::Window() : Document(kDocumentSource), mRoot(mDocument->GetElementById("window")) {
    mTabBar = std::make_unique<TabBar>(mRoot, TabBar::Props{
                                                  .onClose = [this] { request_close(); },
                                                  .selectedTabIndex = 0,
                                                  .autoSelect = true,
                                              });

    auto elem = mDocument->CreateElement("content");
    elem->SetAttribute("id", "content");
    mContentRoot = mRoot->AppendChild(std::move(elem));

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

    // Hide document after transition completion
    listen(mRoot, Rml::EventId::Transitionend, [this](Rml::Event& event) {
        if (event.GetTargetElement() == mRoot && !mRoot->HasAttribute("open") &&
            Document::visible())
        {
            Document::hide(mPendingClose);
        }
    });

    // If an item is selected in a pane, focus the next pane in the tree
    listen(mRoot, Rml::EventId::Submit, [this](Rml::Event& event) {
        int paneIndex = -1;
        for (int i = 0; i < mContentComponents.size(); i++) {
            if (mContentComponents[i]->contains(event.GetTargetElement())) {
                paneIndex = i;
                break;
            }
        }
        if (paneIndex >= 0 && paneIndex < mContentComponents.size() - 1) {
            mContentComponents[paneIndex + 1]->focus();
        }
    });
}

void Window::show() {
    Document::show();
    mRoot->SetAttribute("open", "");
    if (mInitialOpen) {
        mDoAud_seStartMenu(kSoundWindowOpen);
        mInitialOpen = false;
    }
}

void Window::hide(bool close) {
    mRoot->RemoveAttribute("open");
    mPendingClose = close;
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
    return mTabBar->set_active_tab(index);
}

void Window::request_close() {
    if (!consume_close_request()) {
        mDoAud_seStartMenu(kSoundWindowClose);
        pop();
    }
}

bool Window::consume_close_request() {
    return false;
}

void Window::refresh_active_tab() {
    mTabBar->refresh_active_tab();
}

void Window::add_tab(const Rml::String& title, TabBuilder builder) {
    mTabBar->add_tab(title, [this, builder = std::move(builder)] {
        clear_content();
        if (builder) {
            builder(mContentRoot);
        }
    });
}

void Window::clear_content() noexcept {
    mContentComponents.clear();
    while (mContentRoot->GetNumChildren() != 0) {
        mContentRoot->RemoveChild(mContentRoot->GetFirstChild());
    }
}

bool Window::focus() {
    return mTabBar->focus();
}

bool Window::visible() const {
    return mRoot->HasAttribute("open");
}

bool Window::handle_nav_command(Rml::Event& event, NavCommand cmd) {
    auto* target = event.GetTargetElement();
    if (cmd != NavCommand::Next && cmd != NavCommand::Previous && target->Closest("content")) {
        if (handle_content_nav(event, cmd)) {
            return true;
        }
    }
    if (cmd == NavCommand::Confirm || cmd == NavCommand::Down) {
        if (!mContentComponents.empty() && mContentComponents.front()->focus()) {
            mDoAud_seStartMenu(kSoundItemFocus);
            return true;
        }
    }
    if (cmd == NavCommand::Cancel) {
        request_close();
        return true;
    }
    if (mTabBar->handle_nav_command(event, cmd)) {
        return true;
    }
    return mSuppressNavFallback ? false : Document::handle_nav_command(event, cmd);
}

bool Window::handle_content_nav(Rml::Event& event, NavCommand cmd) noexcept {
    if (cmd == NavCommand::Up) {
        if (focus()) {
            mDoAud_seStartMenu(kSoundItemFocus);
            return true;
        }
        return false;
    } else if (cmd == NavCommand::Down) {
        // End of content, avoid looping
        return true;
    } else if (cmd == NavCommand::Cancel) {
        int currentComponent = -1;
        for (int i = 0; i < mContentComponents.size(); ++i) {
            if (mContentComponents[i]->contains(event.GetTargetElement())) {
                currentComponent = i;
                break;
            }
        }
        for (; currentComponent > 0; --currentComponent) {
            if (mContentComponents[currentComponent - 1]->focus()) {
                // When returning to a previous pane, deselect the item after focusing
                if (auto* pane =
                        dynamic_cast<Pane*>(mContentComponents[currentComponent - 1].get()))
                {
                    pane->set_selected_item(-1);
                }
                return true;
            }
        }
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
            if (mContentComponents[i]->focus()) {
                if (direction == -1) {
                    // When returning to a previous pane, deselect the item after focusing
                    if (auto* pane = dynamic_cast<Pane*>(mContentComponents[i].get())) {
                        pane->set_selected_item(-1);
                    }
                }
                return true;
            }
        }
    }
    return false;
}

WindowSmall::WindowSmall(const Rml::String& windowClass, const Rml::String& dialogClass)
    : Document(kDocumentSourceSmall), mRoot(mDocument->GetElementById("window")),
      mDialog(mDocument->GetElementById("dialog")) {
    listen(mRoot, Rml::EventId::Transitionend, [this](Rml::Event& event) {
        if (event.GetTargetElement() == mRoot && !mRoot->HasAttribute("open") &&
            Document::visible())
        {
            Document::hide(mPendingClose);
        }
    });

    mRoot->SetClass(windowClass, true);
    mDialog->SetClass(dialogClass, true);
}

void WindowSmall::show() {
    Document::show();
    mRoot->SetAttribute("open", "");
}

void WindowSmall::hide(bool close) {
    mRoot->RemoveAttribute("open");
    mPendingClose = close;
}

bool WindowSmall::visible() const {
    return mRoot->HasAttribute("open");
}

}  // namespace dusk::ui