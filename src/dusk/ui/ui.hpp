#pragma once

#include <RmlUi/Core/Event.h>
#include <SDL3/SDL_events.h>
#include <filesystem>

namespace dusk::ui {
class Window;

enum class NavCommand {
    None,
    Up,
    Down,
    Left,
    Right,
    Next,      // R1
    Previous,  // L1
    Confirm,   // A
    Cancel,    // B
};

bool initialize() noexcept;
void shutdown() noexcept;

void handle_event(const SDL_Event& event) noexcept;
void update() noexcept;

Window& add_window(std::unique_ptr<Window> window) noexcept;
void remove_window(Window& window) noexcept;

std::filesystem::path resource_path(const std::filesystem::path& filename) noexcept;
std::string escape(std::string_view str) noexcept;

NavCommand map_nav_event(const Rml::Event& event) noexcept;

}  // namespace dusk::ui
