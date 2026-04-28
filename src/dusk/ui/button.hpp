#pragma once

#include <RmlUi/Core/EventListener.h>

#include <functional>
#include <string>
#include <string_view>

namespace Rml {
class Element;
}

namespace dusk::ui {

enum class ButtonVariant {
    Primary,
    Secondary,
    Quiet,
};

class Button : public Rml::EventListener {
public:
    Button(Rml::Element* parent, std::string_view id, std::string_view text, ButtonVariant variant,
        std::function<void()> pressedCallback);
    ~Button() override;

    Button(const Button&) = delete;
    Button& operator=(const Button&) = delete;

    void ProcessEvent(Rml::Event& event) override;

    Rml::Element* element() const;
    std::string id() const;
    void set_text(std::string_view text);

private:
    Rml::Element* m_element = nullptr;
    Rml::Element* m_label = nullptr;
    ButtonVariant m_variant = ButtonVariant::Secondary;
    std::function<void()> m_pressedCallback;
    bool m_hovered = false;
    bool m_focused = false;

    void apply_style();
};

}  // namespace dusk::ui
