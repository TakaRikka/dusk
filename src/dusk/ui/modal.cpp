#include "modal.hpp"

#include <algorithm>

namespace dusk::ui {

Modal::Modal(Props props) : WindowSmall("modal", "modal-dialog"), mProps(std::move(props)) {
    if (!mProps.variant.empty()) {
        mRoot->SetClass(mProps.variant, true);
    }

    auto* header = append(mDialog, "div");
    header->SetClass("modal-header", true);

    auto* title = append(header, "div");
    title->SetClass("modal-title", true);
    title->SetInnerRML(mProps.title);

    if (!mProps.icon.empty()) {
        auto* icon = append(header, "icon");
        icon->SetClass(mProps.icon, true);
    }

    auto* body = append(mDialog, "div");
    body->SetClass("modal-body", true);
    body->SetInnerRML(mProps.bodyRml);

    mContentRoot = append(mDialog, "div");
    mContentRoot->SetClass("modal-content", true);

    auto* actions = append(mDialog, "div");
    actions->SetClass("modal-actions", true);
    if (props.isVertical) {
        actions->SetClass("modal-actions-vertical", true);
    }

    for (auto& action : mProps.actions) {
        add_action(std::move(action));
    }

    mDoAud_seStartMenu(kSoundWindowOpen);
}

void Modal::update() {
    if (mContentPane != nullptr) {
        mContentPane->update();
    }
    for (const auto& button : mButtons) {
        button->update();
    }
    if (mPendingAction) {
        auto action = std::move(mPendingAction);
        action(*this);
    }
    WindowSmall::update();
}

Pane& Modal::content_pane() {
    if (mContentPane == nullptr) {
        mContentRoot->SetClass("active", true);
        mContentPane = std::make_unique<Pane>(mContentRoot, Pane::Type::Uncontrolled);
    }
    return *mContentPane;
}

void Modal::add_action(ModalAction action) {
    auto* actions = mDialog->QuerySelector(".modal-actions");
    auto btn =
        std::make_unique<ControlledButton>(actions, ControlledButton::Props{
                                                        .text = std::move(action.label),
                                                        .isDisabled = std::move(action.isDisabled),
                                                    });
    btn->root()->SetClass("modal-btn", true);
    btn->on_pressed([this, callback = std::move(action.onPressed)] {
        if (!callback) {
            return;
        }
        if (mContentPane != nullptr) {
            mPendingAction = callback;
        } else {
            callback(*this);
        }
    });
    mButtons.push_back(std::move(btn));
}

void Modal::set_body(const Rml::String& bodyRml) {
    mDialog->QuerySelector(".modal-body")->SetInnerRML(bodyRml);
}

void Modal::set_icon(const Rml::String& icon) {
    auto* iconElem = mDialog->QuerySelector("icon");
    if (icon.empty()) {
        if (iconElem != nullptr) {
            iconElem->GetParentNode()->RemoveChild(iconElem);
        }
        return;
    }
    if (iconElem == nullptr) {
        // The constructor only creates the icon element when Props.icon is set.
        iconElem = append(mDialog->QuerySelector(".modal-header"), "icon");
    }
    iconElem->SetClassNames(icon);
}

bool Modal::focus() {
    if (mContentPane != nullptr && mContentPane->focus()) {
        return true;
    }
    for (const auto& button : mButtons) {
        if (button->focus()) {
            return true;
        }
    }
    return false;
}

void Modal::dismiss() {
    if (mProps.onDismiss) {
        mProps.onDismiss(*this);
        return;
    }
    pop();
}

bool Modal::handle_nav_command(Rml::Event& event, NavCommand cmd) {
    if (cmd == NavCommand::Cancel || cmd == NavCommand::Menu) {
        mDoAud_seStartMenu(kSoundWindowClose);
        dismiss();
        return true;
    }

    auto* target = event.GetTargetElement();
    if (mContentPane != nullptr && mContentPane->contains(target) && cmd == NavCommand::Down) {
        for (const auto& button : mButtons) {
            if (button->focus()) {
                mDoAud_seStartMenu(kSoundItemFocus);
                return true;
            }
        }
    }
    if (mContentPane != nullptr && cmd == NavCommand::Up &&
        std::ranges::any_of(
            mButtons, [target](const auto& button) { return button->contains(target); }) &&
        mContentPane->focus_last())
    {
        mDoAud_seStartMenu(kSoundItemFocus);
        return true;
    }

    int direction = 0;
    NavCommand prevCommand = mProps.isVertical ? NavCommand::Up : NavCommand::Left;
    NavCommand nextCommand = mProps.isVertical ? NavCommand::Down : NavCommand::Right;
    if (cmd == prevCommand) {
        direction = -1;
    } else if (cmd == nextCommand) {
        direction = 1;
    } else {
        return false;
    }

    for (int i = 0; i < static_cast<int>(mButtons.size()); ++i) {
        if (mButtons[i]->contains(target)) {
            for (int next = i + direction; next >= 0 && next < static_cast<int>(mButtons.size());
                next += direction)
            {
                if (mButtons[next]->focus()) {
                    mDoAud_seStartMenu(kSoundItemFocus);
                    return true;
                }
            }
            return false;
        }
    }
    return false;
}

}  // namespace dusk::ui
