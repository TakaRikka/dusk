#pragma once

#include <SDL3/SDL_events.h>

namespace dusk::mouse {
void read();
void get_aim_deltas(float& out_yaw, float& out_pitch);
void get_camera_deltas(float& out_yaw, float& out_pitch);
void handle_event(const SDL_Event& event) noexcept;
void on_focus_lost();
void on_focus_gained();
}  // namespace dusk::mouse
