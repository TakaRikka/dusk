#pragma once

#include "controls.hpp"

#include <cstdint>
#include <optional>
#include <string>

class J2DPane;

namespace dusk::ui {

void register_icon_texture_provider() noexcept;
void unregister_icon_texture_provider() noexcept;

void update_midna_icon_texture(J2DPane* pane) noexcept;
std::string midna_icon_source();
uint64_t midna_icon_revision() noexcept;
std::string item_icon_source_for_button(Control control);
std::string item_count_label_for_button(Control control);
std::optional<float> item_oil_fill_for_button(Control control) noexcept;

}  // namespace dusk::ui
