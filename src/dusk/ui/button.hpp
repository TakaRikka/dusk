#pragma once

#include "component.hpp"

namespace dusk::ui {

using ButtonCallback = std::function<void()>;

class Button : public Component {
public:
    struct Props {
        Rml::String text;
        bool selected = false;
    };

    Button(Rml::Element* parent, Props props, const Rml::String& tagName = "button");
    Button(Rml::Element* parent, Rml::String text, const Rml::String& tagName = "button")
        : Button(parent, Props{std::move(text)}, tagName) {}

    void set_text(const Rml::String& text);
    void set_selected(bool selected);
    Button& on_pressed(ButtonCallback callback);

    const Rml::String& get_text() const { return mProps.text; }

private:
    void update_props(Props props);

    Props mProps;
};

class ControlledButton : public Button {
public:
    struct Props {
        Rml::String text;
        std::function<bool()> isSelected;
    };

    ControlledButton(Rml::Element* parent, Props props, const Rml::String& tagName = "button")
        : Button(parent, Button::Props{std::move(props.text)}, tagName),
          mIsSelected(std::move(props.isSelected)) {}

    void update() override;

private:
    std::function<bool()> mIsSelected;
};

}  // namespace dusk::ui