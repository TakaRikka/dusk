#pragma once

#include "string_button.hpp"

namespace dusk::ui {

class NumberButtonFloat : public BaseStringButton {
public:
    struct Props {
        Rml::String key;
        std::function<float()> getValue;
        std::function<void(float)> setValue;
        std::function<bool()> isDisabled;
        std::function<bool()> isModified;
        float min = 0.0f;
        float max = FLT_MAX;
        float step = 1.0f;
        Rml::String prefix;
        Rml::String suffix;
    };

    NumberButtonFloat(Rml::Element* parent, Props props);

    bool modified() const override;
    bool disabled() const override;

protected:
    Rml::String format_value() override;
    Rml::String input_value() override;
    void set_value(Rml::String value) override;
    bool handle_nav_command(NavCommand cmd) override;

private:
    std::function<float()> mGetValue;
    std::function<void(float)> mSetValue;
    std::function<bool()> mIsDisabled;
    std::function<bool()> mIsModified;
    float mMin;
    float mMax;
    float mStep;
    Rml::String mPrefix;
    Rml::String mSuffix;
};

}  // namespace dusk::ui