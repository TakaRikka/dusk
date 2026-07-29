#pragma once

#include "mods/svc/window.h"

union SDL_Event;
struct SDL_Window;

namespace dusk::mods {
struct LoadedMod;
}

namespace dusk::mods::svc {

// Routes an SDL event for an auxiliary mod window.
// Returns true when the event belongs to one.
bool window_dispatch_event(const SDL_Event& event);
ModResult window_acquire_for_graphics(LoadedMod& mod, WindowHandle handle, SDL_Window*& outWindow);
void window_release_for_graphics(LoadedMod& mod, WindowHandle handle);
bool window_get_pixel_size(LoadedMod& mod, WindowHandle handle, uint32_t& width, uint32_t& height);

}  // namespace dusk::mods::svc
