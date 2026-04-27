#pragma once

#include <RmlUi/Core/EventListener.h>

#include <functional>
#include <string>
#include <string_view>

namespace Rml {
class Element;
}

namespace dusk::ui {

class DiscState : public Rml::EventListener {
public:
    DiscState(Rml::Element* parent, std::string_view id, std::string_view text, bool error,
              std::function<void()> pressedCallback);
    ~DiscState() override;

    DiscState(const DiscState&) = delete;
    DiscState& operator=(const DiscState&) = delete;

    void ProcessEvent(Rml::Event& event) override;

    Rml::Element* element() const;
    std::string id() const;

private:
    Rml::Element* m_element = nullptr;
    Rml::Element* m_label = nullptr;
    Rml::Element* m_value = nullptr;
    std::function<void()> m_pressedCallback;
    bool m_error = false;
    bool m_hovered = false;
    bool m_focused = false;

    void apply_style();
};

}  // namespace dusk::ui
