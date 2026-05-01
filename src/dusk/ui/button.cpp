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
    : Component(createRoot(parent, tagName)) {
    update_props(std::move(props));
}

void Button::set_text(const Rml::String& text) {
    if (mProps.text != text) {
        mRoot->SetInnerRML(escape(text));
        mProps.text = text;
    }
}

void Button::set_selected(bool selected) {
    if (mProps.selected != selected) {
        mRoot->SetPseudoClass("selected", selected);
        mProps.selected = selected;
    }
}

Button& Button::on_pressed(ButtonCallback callback) {
    if (!callback) {
        return *this;
    }
    listen(mRoot, Rml::EventId::Click, [callback](Rml::Event&) { callback(); });
    listen(mRoot, Rml::EventId::Keydown, [callback = std::move(callback)](Rml::Event& event) {
        const auto cmd = map_nav_event(event);
        if (cmd == NavCommand::Confirm) {
            callback();
            event.StopPropagation();
        }
    });
    return *this;
}

void Button::update_props(Props props) {
    set_text(props.text);
    set_selected(props.selected);
    mProps = std::move(props);
}

void ControlledButton::update() {
    if (mIsSelected) {
        set_selected(mIsSelected());
    }
    Button::update();
}

}  // namespace dusk::ui