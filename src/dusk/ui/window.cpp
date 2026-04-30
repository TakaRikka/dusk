#include "window.hpp"

#include "aurora/lib/window.hpp"
#include "aurora/rmlui.hpp"
#include "magic_enum.hpp"
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

Window::Window() : Document("res/rml/window.rml"), mRoot(mDocument->GetElementById("window")) {
    mTabBar = std::make_unique<TabBar>(mRoot, TabBar::Props{
                                                  .selectedTabIndex = 0,
                                                  .autoSelect = true,
                                              });

    auto elem = mDocument->CreateElement("div");
    elem->SetAttribute("id", "content");
    elem->SetClass("content", true);
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

bool Window::handle_nav_command(Rml::Event& event, NavCommand cmd) {
    auto* target = event.GetTargetElement();
    if (cmd != NavCommand::Next && cmd != NavCommand::Previous && target->Closest(".content")) {
        if (handle_content_nav(event, cmd)) {
            return true;
        }
    }
    if (cmd == NavCommand::Confirm || cmd == NavCommand::Down) {
        if (!mContentComponents.empty()) {
            return mContentComponents.front()->focus();
        }
    }
    if (cmd == NavCommand::Cancel) {
        pop_document();
        return true;
    }
    return mTabBar->handle_nav_command(event, cmd);
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
