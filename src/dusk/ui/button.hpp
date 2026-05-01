#pragma once

#include "component.hpp"

namespace dusk::ui {

struct ButtonProps {
    Rml::String text;
    std::function<void(Rml::Event&)> onPressed;
    bool selected = false;
};

class Button : public Component {
public:
    using Props = ButtonProps;

    Button(Rml::Element* parent, ButtonProps props, const Rml::String& tagName = "button");

    void set_text(const Rml::String& text);
    void set_selected(bool selected);

    const Rml::String& get_text() const { return mProps.text; }

private:
    void update_props(Props props);

    ButtonProps mProps;
};

}  // namespace dusk::ui