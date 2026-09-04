#include "group_button.hpp"

namespace dusk::ui {

GroupButton::GroupButton(Rml::Element* parent, Props props)
    : SelectButton{parent, {.key = std::move(props.text)}},
      mIsSelected{std::move(props.isSelected)}, mIsDisabled{std::move(props.isDisabled)} {
    mRoot->SetClass("group-button", true);
}

void GroupButton::update() {
    set_selected(selected());
    set_disabled(disabled());
    SelectButton::update();
}

bool GroupButton::selected() const {
    return mIsSelected ? mIsSelected() : SelectButton::selected();
}

bool GroupButton::disabled() const {
    return mIsDisabled ? mIsDisabled() : SelectButton::disabled();
}

}  // namespace dusk::ui
