#pragma once

#include <SDL3/SDL_events.h>

#include <filesystem>

namespace dusk::ui {

bool initialize() noexcept;
void shutdown() noexcept;

void handle_event(const SDL_Event& event) noexcept;
void update() noexcept;

std::filesystem::path resource_path(const std::filesystem::path& filename) noexcept;

}  // namespace dusk::ui
