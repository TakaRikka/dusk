#pragma once

#include "component.hpp"
#include "ui.hpp"

namespace dusk::ui {

class SelectButton : public Component {
public:
    struct Props {
        Rml::String key;
        Rml::String value;
        bool selected = false;
        bool disabled = false;
    };

    SelectButton(Rml::Element* parent, Props props);

    bool focus() override;

    void set_selected(bool selected);
    bool get_selected() const { return mProps.selected; }
    void set_disabled(bool disabled);
    bool get_disabled() const { return mProps.disabled; }
    void set_value_label(const Rml::String& value);

protected:
    void update_props(Props props);
    virtual bool handle_nav_command(NavCommand cmd);

    Props mProps;
    Rml::Element* mKeyElem = nullptr;
    Rml::Element* mValueElem = nullptr;
};

class ControlledSelectButton : public SelectButton {
public:
    ControlledSelectButton(Rml::Element* parent, Props props)
        : SelectButton(parent, std::move(props)) {}

    void update() override;

protected:
    virtual Rml::String format_value() = 0;
    virtual bool is_disabled() { return false; }
};

}  // namespace dusk::ui
