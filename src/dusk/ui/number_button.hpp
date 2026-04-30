#pragma once

#include "string_button.hpp"

namespace dusk::ui {

class NumberButton : public BaseStringButton {
public:
    struct Props {
        Rml::String key;
        std::function<int()> getValue;
        std::function<void(int)> setValue;
        int min = 0;
        int max = INT_MAX;
        int step = 1;
    };

    NumberButton(Rml::Element* parent, Props props);

protected:
    Rml::String format_value() override;
    void set_value(Rml::String value) override;
    bool handle_nav_command(NavCommand cmd) override;

private:
    std::function<int()> mGetValue;
    std::function<void(int)> mSetValue;
    int mMin;
    int mMax;
    int mStep;
};

}  // namespace dusk::ui