#pragma once

#include "string_button.hpp"

namespace dusk::ui {

class NumberButton : public BaseStringButton {
public:
    struct Props {
        Rml::String key;
        std::function<int()> getValue;
        std::function<void(int)> setValue;
        std::function<bool()> isDisabled;
        int min = 0;
        int max = INT_MAX;
        int step = 1;
        Rml::String prefix;
        Rml::String suffix;
    };

    NumberButton(Rml::Element* parent, Props props);

    bool disabled() const override;

protected:
    Rml::String format_value() override;
    Rml::String input_value() override;
    void set_value(Rml::String value) override;
    bool handle_nav_command(NavCommand cmd) override;

private:
    std::function<int()> mGetValue;
    std::function<void(int)> mSetValue;
    std::function<bool()> mIsDisabled;
    int mMin;
    int mMax;
    int mStep;
    Rml::String mPrefix;
    Rml::String mSuffix;
};

}  // namespace dusk::ui