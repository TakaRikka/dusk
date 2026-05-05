#pragma once

#include "button.hpp"
#include "component.hpp"
#include "select_button.hpp"

namespace dusk::ui {

class Pane : public FluentComponent<Pane> {
public:
    enum class Type {
        Controlled,
        Uncontrolled,
    };

    explicit Pane(Rml::Element* parent, Type type);

    bool focus() override;
    void update() override;

    void set_selected_item(int index);
    Component& register_control(
        Component& component, Pane& nextPane, std::function<void(Pane&)> callback);

    Rml::Element* add_section(const Rml::String& text);
    ControlledButton& add_button(ControlledButton::Props props) {
        return add_child<ControlledButton>(std::move(props));
    }
    Button& add_button(Rml::String text) { return add_child<Button>(std::move(text)); }
    ControlledSelectButton& add_select_button(ControlledSelectButton::Props props) {
        return add_child<ControlledSelectButton>(std::move(props));
    }
    Rml::Element* add_text(const Rml::String& text);
    Rml::Element* add_rml(const Rml::String& rml);
    void finalize();
    void clear();

private:
    Type mType;
    bool finalized = false;
};

}  // namespace dusk::ui
