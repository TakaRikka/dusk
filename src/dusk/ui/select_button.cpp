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

SelectButton::SelectButton(Rml::Element* parent, Props props)
    : FluentComponent(createRoot(parent)) {
    mKeyElem = append(mRoot, "key");
    mValueElem = append(mRoot, "value");
    update_props(std::move(props));
    listen(Rml::EventId::Click, [this](Rml::Event& event) {
        if (disabled()) {
            return;
        }
        if (handle_nav_command(NavCommand::Confirm)) {
            event.StopPropagation();
        }
    });
    listen(Rml::EventId::Keydown, [this](Rml::Event& event) {
        if (disabled()) {
            return;
        }
        const auto cmd = map_nav_event(event);
        if (cmd != NavCommand::None && handle_nav_command(cmd)) {
            event.StopPropagation();
        }
    });
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
    mProps = std::move(props);
}

bool SelectButton::handle_nav_command(NavCommand cmd) {
    if (cmd == NavCommand::Confirm) {
        set_selected(!selected());
        return true;
    }
    return false;
}

void BaseControlledSelectButton::update() {
    set_disabled(disabled());
    set_value_label(format_value());
    SelectButton::update();
}

bool ControlledSelectButton::disabled() const {
    if (mIsDisabled) {
        return mIsDisabled();
    }
    return BaseControlledSelectButton::disabled();
}

Rml::String ControlledSelectButton::format_value() {
    if (!mGetValue) {
        return "";
    }
    return mGetValue();
}

}  // namespace dusk::ui
