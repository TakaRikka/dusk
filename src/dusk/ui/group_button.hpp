#pragma once

#include "select_button.hpp"

namespace dusk::ui {

class GroupButton : public SelectButton {
public:
    struct Props {
        Rml::String text;
        std::function<bool()> isDisabled;
    };

    GroupButton(Rml::Element* parent, Props props);

    void update() override;
    bool disabled() const override;

private:
    std::function<bool()> mIsDisabled;
};

}  // namespace dusk::ui
