#pragma once

#include "component.hpp"
#include "ui.hpp"

namespace dusk::ui {

class SelectButton : public FluentComponent<SelectButton> {
public:
    struct Props {
        Rml::String key;
        Rml::String value;
    };

    SelectButton(Rml::Element* parent, Props props);

    void set_value_label(const Rml::String& value);

    SelectButton& on_selected(std::function<void(bool)> callback) {
        return listen(Rml::EventId::Change, [callback = std::move(callback)](Rml::Event& event) {
            callback(event.GetParameter("selected", false));
        });
    }

protected:
    void update_props(Props props);
    virtual bool handle_nav_command(NavCommand cmd);

    Props mProps;
    Rml::Element* mKeyElem = nullptr;
    Rml::Element* mValueElem = nullptr;
    std::function<void()> mOnHover;
};

class BaseControlledSelectButton : public SelectButton {
public:
    BaseControlledSelectButton(Rml::Element* parent, Props props)
        : SelectButton(parent, std::move(props)) {}

    void update() override;

protected:
    virtual Rml::String format_value() = 0;
};

class ControlledSelectButton : public BaseControlledSelectButton {
public:
    struct Props {
        Rml::String key;
        std::function<Rml::String()> getValue;
        std::function<bool()> isDisabled;
    };

    ControlledSelectButton(Rml::Element* parent, Props props)
        : BaseControlledSelectButton(parent, {std::move(props.key)}),
          mGetValue(std::move(props.getValue)), mIsDisabled(std::move(props.isDisabled)) {}

    bool disabled() const override;

protected:
    Rml::String format_value() override;

    std::function<Rml::String()> mGetValue;
    std::function<bool()> mIsDisabled;
};

}  // namespace dusk::ui
