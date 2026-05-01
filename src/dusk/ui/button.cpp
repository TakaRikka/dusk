#include "button.hpp"

#include "ui.hpp"

#include <utility>

namespace dusk::ui {
namespace {

Rml::Element* createRoot(Rml::Element* parent, const Rml::String& tagName) {
    auto* doc = parent->GetOwnerDocument();
    auto elem = doc->CreateElement(tagName);
    return parent->AppendChild(std::move(elem));
}

}  // namespace

Button::Button(Rml::Element* parent, Props props, const Rml::String& tagName)
    : FluentComponent(createRoot(parent, tagName)) {
    update_props(std::move(props));
}

void Button::set_text(const Rml::String& text) {
    if (mProps.text != text) {
        mRoot->SetInnerRML(escape(text));
        mProps.text = text;
    }
}

Button& Button::on_pressed(ButtonCallback callback) {
    if (!callback) {
        return *this;
    }
    // TODO: convert this to a FluentComponent method?
    on_nav_command([callback = std::move(callback)](Rml::Event&, NavCommand cmd) {
        if (cmd == NavCommand::Confirm) {
            callback();
            return true;
        }
        return false;
    });
    return *this;
}

void Button::update_props(Props props) {
    set_text(props.text);
    mProps = std::move(props);
}

void ControlledButton::update() {
    if (mIsSelected) {
        set_selected(mIsSelected());
    }
    Button::update();
}

bool ControlledButton::selected() const {
    if (mIsSelected) {
        return mIsSelected();
    }
    return Button::selected();
}

}  // namespace dusk::ui