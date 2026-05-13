#pragma once

#include <dolphin/pad.h>

union SDL_Event;

namespace dusk::ios::touch_controls {

void handle_event(const SDL_Event& event) noexcept;
void apply_pad_state(PADStatus& status, u32 port = PAD_CHAN0) noexcept;
void draw() noexcept;
void reset() noexcept;
bool enabled_for_port(u32 port) noexcept;
void set_enabled_for_port(u32 port, bool enabled) noexcept;
const char* controller_name() noexcept;

}  // namespace dusk::ios::touch_controls
