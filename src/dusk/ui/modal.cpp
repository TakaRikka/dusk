#include "modal.hpp"

namespace dusk::ui {

const Rml::String kDocumentSource = R"RML(
<rml>
<head>
    <link type="text/rcss" href="res/rml/window.rcss" />
</head>
<body>
    <window id="window" class="small modal">
        <div id="modal" class="modal-dialog" />
    </window>
</body>
</rml>
)RML";

Rml::Element* createElement(Rml::Element* parent, const Rml::String& tag) {
    auto* doc = parent->GetOwnerDocument();
    auto elem = doc->CreateElement(tag);
    return parent->AppendChild(std::move(elem));
}

Modal::Modal(Props props)
    : Document(kDocumentSource), mProps(std::move(props)), mRoot(mDocument->GetElementById("window")) {
    listen(mRoot, Rml::EventId::Transitionend, [this](Rml::Event& event) {
        if (event.GetTargetElement() == mRoot && !mRoot->HasAttribute("open") && Document::visible()) {
            Document::hide(mPendingClose);
        }
    });

    auto* dialog = mDocument->GetElementById("modal");

    auto* title = createElement(dialog, "div");
    title->SetClass("preset-title", true);
    title->SetInnerRML(mProps.title);

    auto* body = createElement(dialog, "div");
    body->SetClass("preset-intro", true);
    body->SetInnerRML(mProps.bodyRml);

    auto* actions = createElement(dialog, "div");
    actions->SetClass("modal-actions", true);

    for (auto& action : mProps.actions) {
        auto btn = std::make_unique<Button>(actions, action.label);
        btn->root()->SetClass("modal-btn", true);
        btn->on_pressed([callback = std::move(action.onPressed)] {
            if (callback) {
                callback();
            }
        });
        mButtons.push_back(std::move(btn));
    }
}

void Modal::show() {
    Document::show();
    mRoot->SetAttribute("open", "");
}

void Modal::hide(bool close) {
    mRoot->RemoveAttribute("open");
    mPendingClose = close;
}

bool Modal::visible() const {
    return mRoot->HasAttribute("open");
}

bool Modal::focus() {
    if (!mButtons.empty()) {
        return mButtons.back()->focus();
    }
    return false;
}

void Modal::dismiss() {
    if (mProps.onDismiss) {
        mProps.onDismiss();
        return;
    }
    pop();
}

bool Modal::handle_nav_command(Rml::Event& event, NavCommand cmd) {
    if (cmd == NavCommand::Cancel || cmd == NavCommand::Menu) {
        mDoAud_seStartMenu(Z2SE_SY_CURSOR_CANCEL);
        dismiss();
        return true;
    }

    int direction = 0;
    if (cmd == NavCommand::Left) {
        direction = -1;
    } else if (cmd == NavCommand::Right) {
        direction = 1;
    } else {
        return false;
    }

    auto* target = event.GetTargetElement();
    for (int i = 0; i < static_cast<int>(mButtons.size()); ++i) {
        if (mButtons[i]->contains(target)) {
            const int next = i + direction;
            if (next >= 0 && next < static_cast<int>(mButtons.size()) && mButtons[next]->focus()) {
                mDoAud_seStartMenu(Z2SE_SY_NAME_CURSOR);
                return true;
            }
            return false;
        }
    }
    return false;
}

}  // namespace dusk::ui
