#include "button.hpp"

#include "ui.hpp"

#include <utility>

namespace dusk::ui {
namespace {

Rml::Element* createRoot(Rml::Element* parent, const Rml::String& className) {
    auto* doc = parent->GetOwnerDocument();
    auto elem = doc->CreateElement("button");
    elem->SetClass(className, true);
    return parent->AppendChild(std::move(elem));
}

}  // namespace

Button::Button(Rml::Element* parent, ButtonProps props, const Rml::String& className)
    : Component(createRoot(parent, className)) {
    update_props(std::move(props));
    listen(mRoot, Rml::EventId::Click, [this](Rml::Event& event) {
        if (mProps.onPressed) {
            mProps.onPressed(event);
        }
    });
}

void Button::set_text(const Rml::String& text) {
    if (mProps.text != text) {
        mRoot->SetInnerRML(escape(text));
        mProps.text = text;
    }
}

void Button::set_selected(bool selected) {
    if (mProps.selected != selected) {
        mRoot->SetClass("selected", selected);
        mProps.selected = selected;
    }
}

void Button::update_props(Props props) {
    set_text(props.text);
    set_selected(props.selected);
    mProps = std::move(props);
}

}  // namespace dusk::ui