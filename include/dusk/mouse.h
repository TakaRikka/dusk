#pragma once

#include <SDL3/SDL_events.h>

namespace dusk::mouse {
void read();
void getAimDeltas(float& out_yaw, float& out_pitch);
void getCameraDeltas(float& out_yaw, float& out_pitch);
void handle_event(const SDL_Event& event) noexcept;
void onFocusLost();
void onFocusGained();
}  // namespace dusk::mouse
