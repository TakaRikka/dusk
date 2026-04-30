#pragma once

#include "component.hpp"
#include "ui.hpp"

namespace dusk::ui {

class SelectButton;

struct SelectButtonProps {
    Rml::String key;
    Rml::String value;
    bool selected = false;
    bool disabled = false;
};

class SelectButton : public Component {
public:
    using Props = SelectButtonProps;

    SelectButton(Rml::Element* parent, SelectButtonProps props);

    bool focus() override;

    void set_selected(bool selected);
    bool get_selected() const { return mProps.selected; }
    void set_disabled(bool disabled);
    bool get_disabled() const { return mProps.disabled; }

    void set_value_label(const Rml::String& value);

protected:
    void update_props(Props props);
    virtual bool handle_nav_command(NavCommand cmd);

    SelectButtonProps mProps;
    Rml::Element* mKeyElem = nullptr;
    Rml::Element* mValueElem = nullptr;
};

}  // namespace dusk::ui
