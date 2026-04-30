#pragma once

#include <RmlUi/Core/Event.h>
#include <SDL3/SDL_events.h>
#include <filesystem>

#include "nav_types.hpp"

namespace dusk::ui {
class Window;
class Popup;

bool initialize() noexcept;
void shutdown() noexcept;

void handle_event(const SDL_Event& event) noexcept;
void update() noexcept;

Window& add_window(std::unique_ptr<Window> window) noexcept;
void remove_window(Window& window) noexcept;

Popup& add_popup(std::unique_ptr<Popup> popup) noexcept;

std::filesystem::path resource_path(const std::filesystem::path& filename) noexcept;
std::string escape(std::string_view str) noexcept;

NavCommand map_nav_event(const Rml::Event& event) noexcept;

}  // namespace dusk::ui
