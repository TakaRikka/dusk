#pragma once

#include "button.hpp"

#include <functional>
#include <memory>
#include <vector>

namespace dusk::ui {

std::unique_ptr<Button> create_tab_button(Rml::Element* tabBar, const Rml::String& title, bool selected, std::function<void(Rml::Event&)> onPressed);
void set_selected_tab(std::vector<Button*>& tabs, int selectedIndex);

}  // namespace dusk::ui
