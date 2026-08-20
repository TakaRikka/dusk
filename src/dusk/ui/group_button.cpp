#include "group_button.hpp"

namespace dusk::ui {

GroupButton::GroupButton(Rml::Element* parent, Props props)
    : SelectButton{parent, {.key = std::move(props.text)}},
      mIsDisabled{std::move(props.isDisabled)} {
    mRoot->SetClass("group-button", true);
}

void GroupButton::update() {
    set_disabled(disabled());
    SelectButton::update();
}

bool GroupButton::disabled() const {
    return mIsDisabled ? mIsDisabled() : SelectButton::disabled();
}

}  // namespace dusk::ui
