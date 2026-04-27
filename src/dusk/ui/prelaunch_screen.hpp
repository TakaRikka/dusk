#pragma once

#include <SDL3/SDL_events.h>

namespace dusk::ui::prelaunch {

bool initialize();
void shutdown();
bool is_active();
void handle_event(const SDL_Event& event);
void update();

}  // namespace dusk::ui::prelaunch
