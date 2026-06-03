#pragma once
#include "select_button.hpp"

namespace dusk::ui {

class BoolButton : public BaseControlledSelectButton {
public:
    struct Props {
        Rml::String key;
        Rml::String icon;
        std::function<bool()> getValue;
        std::function<void(bool)> setValue;
        std::function<bool()> isDisabled;
        std::function<bool()> isModified;
        std::function<Rml::String()> valueOverride;
    };

    BoolButton(Rml::Element* parent, Props props);

    bool modified() const override;
    bool disabled() const override;

protected:
    Rml::String format_value() override;
    bool handle_nav_command(NavCommand cmd) override;

private:
    std::function<int()> mGetValue;
    std::function<void(int)> mSetValue;
    std::function<bool()> mIsDisabled;
    std::function<bool()> mIsModified;
    std::function<Rml::String()> mValueOverride;
};

}  // namespace dusk::ui
