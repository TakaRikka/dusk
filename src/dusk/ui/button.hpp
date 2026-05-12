#pragma once

#include "component.hpp"

namespace dusk::ui {

using ButtonCallback = std::function<void()>;

class Button : public FluentComponent<Button> {
public:
    struct Props {
        Rml::String text;
    };

    Button(Rml::Element* parent, Props props, const Rml::String& tagName = "button");
    Button(Rml::Element* parent, Rml::String text, const Rml::String& tagName = "button")
        : Button(parent, Props{std::move(text)}, tagName) {}

    void set_text(const Rml::String& text);
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
        std::function<bool()> isDisabled;
    };

    ControlledButton(Rml::Element* parent, Props props, const Rml::String& tagName = "button")
        : Button(parent, {std::move(props.text)}, tagName),
          mIsSelected(std::move(props.isSelected)),
          mIsDisabled(std::move(props.isDisabled)) {}

    void update() override;
    bool selected() const override;
    bool disabled() const override;

private:
    std::function<bool()> mIsSelected;
    std::function<bool()> mIsDisabled;
};

}  // namespace dusk::ui
