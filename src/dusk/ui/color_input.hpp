#pragma once

#include "popover.hpp"
#include "select_button.hpp"

namespace dusk::ui {

class NavGroup;

class ColorInput : public BaseControlledSelectButton {
public:
    struct Props {
        Rml::String key;
        Popover::Side side = Popover::Side::Left;
        std::function<Rml::String()> getValue;
        std::function<void(Rml::String)> setValue;
        std::function<bool()> isDisabled;
        std::function<bool()> isModified;
        std::vector<Rml::String> presets;
        bool alpha = false;
    };

    ColorInput(Rml::Element* parent, Props props);
    ~ColorInput() override;

    void update() override;
    bool modified() const override;
    bool disabled() const override;

protected:
    Rml::String format_value() override;
    bool handle_nav_command(NavCommand cmd) override;

private:
    void toggle_picker();
    void build_picker();
    void add_presets(NavGroup& navigation);
    void add_history(NavGroup& navigation);
    void add_swatch_button(NavGroup& navigation, const Rml::String& value);
    void begin_adjustment();
    void cancel_adjustment();
    void adjust_sv(float x, float y, float deltaSeconds);
    void adjust_hue(float x, float deltaSeconds);
    void adjust_alpha(float x, float deltaSeconds);
    void nudge_sv(NavCommand direction);
    void nudge_hue(NavCommand direction);
    void nudge_alpha(NavCommand direction);
    void commit_color();
    void commit_value(Rml::String value);
    void remember_initial_value();
    void refresh_swatch();
    void refresh_picker();
    Rml::Colourb current_color() const;

    Props mProps;
    Rml::Element* mSwatch = nullptr;
    Rml::String mDisplayedValue;
    Rml::String mInitialValue;
    Rml::String mAdjustmentStartValue;
    bool mHasDisplayedValue = false;

    Popover* mPicker = nullptr;
    Rml::Element* mSvArea = nullptr;
    Rml::Element* mSvCursor = nullptr;
    Rml::Element* mHueBar = nullptr;
    Rml::Element* mHueCursor = nullptr;
    Rml::Element* mAlphaBar = nullptr;
    Rml::Element* mAlphaCursor = nullptr;
    Rml::Element* mPickerValue = nullptr;
    std::unique_ptr<NavGroup> mPickerNavigation;
    std::vector<std::unique_ptr<ScopedEventListener>> mPickerListeners;

    float mHue = 0.0f;
    float mSat = 0.0f;
    float mVal = 0.0f;
    float mAlpha = 1.0f;
    float mAdjustmentStartHue = 0.0f;
    float mAdjustmentStartSat = 0.0f;
    float mAdjustmentStartVal = 0.0f;
    float mAdjustmentStartAlpha = 1.0f;
    bool mHexFormat = true;
    bool mInitialValueRemembered = false;
};

}  // namespace dusk::ui
