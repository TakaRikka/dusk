#pragma once

#include "component.hpp"
#include "ui.hpp"

#include <functional>
#include <utility>

namespace dusk::ui {

using SelectButtonCallback = std::function<void()>;

class SelectButton : public FluentComponent<SelectButton> {
public:
    struct Props {
        Rml::String key;
        Rml::String value;
        Rml::String icon;
        bool modified = false;
    };

    SelectButton(Rml::Element* parent, Props props);

    virtual bool modified() const;
    void set_modified(bool value);
    void set_value_label(const Rml::String& value);
    SelectButton& on_pressed(SelectButtonCallback callback);

protected:
    void update_props(Props props);
    virtual bool handle_nav_command(NavCommand cmd);

    Props mProps;
    Rml::Element* mKeyElem = nullptr;
    Rml::Element* mIconElem = nullptr;
    Rml::Element* mValueElem = nullptr;
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
        std::function<bool()> isModified;
    };

    ControlledSelectButton(Rml::Element* parent, Props props)
        : BaseControlledSelectButton(parent, {std::move(props.key)}),
          mGetValue(std::move(props.getValue)), mIsDisabled(std::move(props.isDisabled)),
          mIsModified(std::move(props.isModified)) {}

    bool modified() const override;
    bool disabled() const override;

protected:
    Rml::String format_value() override;

    std::function<Rml::String()> mGetValue;
    std::function<bool()> mIsDisabled;
    std::function<bool()> mIsModified;
};

}  // namespace dusk::ui
