#pragma once

#include <mods/api.h>

class RandomizerContext;

namespace randomizer::messages {

ModResult initialize();
ModResult activate(RandomizerContext& context);
void deactivate();

}  // namespace randomizer::messages
