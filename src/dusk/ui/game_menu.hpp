#pragma once

#include <SDL3/SDL_events.h>

namespace dusk::ui::game_menu {

bool initialize();
void shutdown();

bool is_active();
void toggle();
void set_active(bool active);

void handle_event(const SDL_Event& event);
void update();

}  // namespace dusk::ui::game_menu
