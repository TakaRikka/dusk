#pragma once

#include "button.hpp"
#include "component.hpp"
#include "select_button.hpp"

namespace dusk::ui {

class Pane : public Component {
public:
    enum class Direction {
        Vertical,
        Horizontal,
    };

    explicit Pane(Rml::Element* parent, Direction direction);

    bool focus() override;
    void update() override;

    Rml::Element* add_section(const Rml::String& text);
    Button& add_button(Button::Props props, ButtonCallback callback) {
        return add_child<Button>(std::move(props)).on_pressed(std::move(callback));
    }
    Button& add_button(Rml::String text, ButtonCallback callback) {
        return add_child<Button>(std::move(text)).on_pressed(std::move(callback));
    }
    SelectButton& add_select_button(SelectButton::Props props) {
        return add_child<SelectButton>(std::move(props));
    }
    Rml::Element* add_text(const Rml::String& text);
    Rml::Element* add_rml(const Rml::String& rml);
    void finalize();
    void clear();

private:
    Direction mDirection;
    bool finalized = false;
};

}  // namespace dusk::ui
