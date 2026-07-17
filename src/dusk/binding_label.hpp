#pragma once

#include "dusk/actions.hpp"

#include <SDL3/SDL_gamepad.h>

#include <string>

namespace dusk::actions {

// Internal helpers used by actions.cpp; public API is in dusk/actions.hpp.
std::string format_gamepad_axis_label(SDL_Gamepad* gamepad, SDL_GamepadAxis axis);

}  // namespace dusk::actions
