#pragma once

#include "component.hpp"

namespace dusk::ui {

class SelectButton;

struct SelectButtonProps {
    Rml::String key;
    Rml::String value;
    std::function<Rml::String()> getValue;
    std::function<void(SelectButton& self, Rml::Event&)> onPressed;
    bool selected = false;
};

class SelectButton : public Component {
public:
    using Props = SelectButtonProps;

    SelectButton(Rml::Element* parent, SelectButtonProps props);

    void update() override;

    void set_selected(bool selected);
    bool get_selected() const { return mProps.selected; }

    void set_value(const Rml::String& value);

private:
    void update_props(Props props);

    SelectButtonProps mProps;
    Rml::Element* mKeyElem = nullptr;
    Rml::Element* mValueElem = nullptr;
};

}  // namespace dusk::ui