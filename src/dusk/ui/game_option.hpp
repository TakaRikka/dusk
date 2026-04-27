#pragma once

#include <RmlUi/Core/EventListener.h>

#include <functional>
#include <string>
#include <string_view>

namespace Rml {
class Element;
}

namespace dusk::ui {

class GameOption : public Rml::EventListener {
public:
    GameOption(Rml::Element* parent, std::string_view id, std::string_view title,
               std::string_view value, std::string_view detail,
               std::function<void()> pressedCallback);
    ~GameOption() override;

    GameOption(const GameOption&) = delete;
    GameOption& operator=(const GameOption&) = delete;

    void ProcessEvent(Rml::Event& event) override;

    Rml::Element* element() const;
    std::string id() const;
    void set_value(std::string_view value);

private:
    Rml::Element* m_element = nullptr;
    Rml::Element* m_title = nullptr;
    Rml::Element* m_value = nullptr;
    Rml::Element* m_detail = nullptr;
    std::function<void()> m_pressedCallback;
    bool m_hovered = false;
    bool m_focused = false;

    void apply_style();
};

}  // namespace dusk::ui
