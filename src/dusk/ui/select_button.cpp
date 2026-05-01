#include "select_button.hpp"

#include "ui.hpp"

#include <utility>

namespace dusk::ui {
namespace {

Rml::Element* createRoot(Rml::Element* parent) {
    auto* doc = parent->GetOwnerDocument();
    auto elem = doc->CreateElement("select-button");
    return parent->AppendChild(std::move(elem));
}

}  // namespace

SelectButton::SelectButton(Rml::Element* parent, Props props) : Component(createRoot(parent)) {
    mKeyElem = append(mRoot, "key");
    mValueElem = append(mRoot, "value");
    update_props(std::move(props));
    listen(mRoot, Rml::EventId::Click, [this](Rml::Event& event) {
        if (mProps.disabled) {
            return;
        }
        if (handle_nav_command(NavCommand::Confirm)) {
            event.StopPropagation();
        }
    });
    listen(mRoot, Rml::EventId::Keydown, [this](Rml::Event& event) {
        if (mProps.disabled) {
            return;
        }
        const auto cmd = map_nav_event(event);
        if (cmd != NavCommand::None && handle_nav_command(cmd)) {
            event.StopPropagation();
        }
    });
}

bool SelectButton::focus() {
    if (mProps.disabled) {
        return false;
    }
    return Component::focus();
}

void SelectButton::set_selected(bool selected) {
    if (mProps.selected != selected) {
        mRoot->SetPseudoClass("selected", selected);
        mProps.selected = selected;
    }
}

void SelectButton::set_disabled(bool disabled) {
    if (mProps.disabled != disabled) {
        if (disabled) {
            mRoot->SetAttribute("disabled", "");
            mRoot->SetPseudoClass("disabled", true);
            mRoot->Blur();
        } else {
            mRoot->RemoveAttribute("disabled");
            mRoot->SetPseudoClass("disabled", false);
        }
        mProps.disabled = disabled;
    }
}

void SelectButton::set_value_label(const Rml::String& value) {
    if (mProps.value != value) {
        mValueElem->SetInnerRML(escape(value));
        mProps.value = value;
    }
}

void SelectButton::update_props(Props props) {
    if (mProps.key != props.key) {
        mKeyElem->SetInnerRML(escape(props.key));
    }
    set_value_label(props.value);
    set_selected(props.selected);
    set_disabled(props.disabled);
    mProps = std::move(props);
}

bool SelectButton::handle_nav_command(NavCommand cmd) {
    if (cmd == NavCommand::Confirm) {
        set_selected(!get_selected());
        return true;
    }
    return false;
}

void BaseControlledSelectButton::update() {
    set_disabled(is_disabled());
    set_value_label(format_value());
    SelectButton::update();
}

Rml::String ControlledSelectButton::format_value() {
    if (!mGetValue) {
        return "";
    }
    return mGetValue();
}

bool ControlledSelectButton::is_disabled() {
    if (!mIsDisabled) {
        return false;
    }
    return mIsDisabled();
}

}  // namespace dusk::ui
