#include "select_button.hpp"

#include "ui.hpp"

#include <utility>

namespace dusk::ui {
namespace {

Rml::Element* createRoot(Rml::Element* parent) {
    auto* doc = parent->GetOwnerDocument();
    auto elem = doc->CreateElement("button");
    elem->SetClass("select-button", true);
    return parent->AppendChild(std::move(elem));
}

}  // namespace

SelectButton::SelectButton(Rml::Element* parent, Props props) : Component(createRoot(parent)) {
    mKeyElem = append(mRoot, "div");
    mKeyElem->SetClass("key", true);
    mValueElem = append(mRoot, "div");
    mValueElem->SetClass("value", true);
    update_props(std::move(props));
    listen(mRoot, Rml::EventId::Click, [this](Rml::Event& event) {
        if (mProps.onPressed) {
            mProps.onPressed(*this, event);
        }
    });
}

void SelectButton::update() {
    if (mProps.getValue) {
        set_value(mProps.getValue());
    }
    Component::update();
}

void SelectButton::set_selected(bool selected) {
    if (mProps.selected != selected) {
        mRoot->SetClass("selected", selected);
        mProps.selected = selected;
    }
}

void SelectButton::set_value(const Rml::String& value) {
    if (mProps.value != value) {
        mValueElem->SetInnerRML(escape(value));
        mProps.value = value;
    }
}

void SelectButton::update_props(Props props) {
    if (mProps.key != props.key) {
        mKeyElem->SetInnerRML(escape(props.key));
    }
    set_value(props.value);
    set_selected(props.selected);
    mProps = std::move(props);
}

}  // namespace dusk::ui