#pragma once

#include <dolphin/pad.h>

union SDL_Event;

namespace dusk::ios::touch_controls {

void handle_event(const SDL_Event& event) noexcept;
void apply_pad_state(PADStatus& status) noexcept;
void draw() noexcept;
void reset() noexcept;

}  // namespace dusk::ios::touch_controls
