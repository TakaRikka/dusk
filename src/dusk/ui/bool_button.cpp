#include "bool_button.hpp"

namespace dusk::ui {

BoolButton::BoolButton(Rml::Element* parent, Props props)
    : BaseControlledSelectButton(parent, {std::move(props.key)}),
      mGetValue(std::move(props.getValue)), mSetValue(std::move(props.setValue)),
      mIsDisabled(std::move(props.isDisabled)), mIsModified(std::move(props.isModified)) {}

bool BoolButton::modified() const {
    if (mIsModified) {
        return mIsModified();
    }
    return BaseControlledSelectButton::modified();
}

bool BoolButton::disabled() const {
    if (mIsDisabled) {
        return mIsDisabled();
    }
    return BaseControlledSelectButton::disabled();
}

Rml::String BoolButton::format_value() {
    return mGetValue() ? "On" : "Off";
}

bool BoolButton::handle_nav_command(NavCommand cmd) {
    if (cmd == NavCommand::Confirm || cmd == NavCommand::Left || cmd == NavCommand::Right) {
        mSetValue(!mGetValue());
        return true;
    }
    return false;
}

}  // namespace dusk::ui