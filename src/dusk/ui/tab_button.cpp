#include "tab_button.hpp"

#include <utility>

namespace dusk::ui {

std::unique_ptr<Button> create_tab_button(Rml::Element* tabBar, const Rml::String& title,
    bool selected, std::function<void(Rml::Event&)> onPressed) {
    return std::make_unique<Button>(tabBar,
        Button::Props{
            .text = title,
            .onPressed = std::move(onPressed),
            .selected = selected,
        },
        "tab");
}

void set_selected_tab(std::vector<Button*>& tabs, int selectedIndex) {
    for (int i = 0; i < static_cast<int>(tabs.size()); ++i) {
        tabs[i]->set_selected(i == selectedIndex);
    }
}

}  // namespace dusk::ui
