#pragma once

#include "dusk/config.hpp"
#include "mods/svc/config.h"

namespace dusk::mods {
struct LoadedMod;
}  // namespace dusk::mods

namespace dusk::mods::svc {

// Returns a config var owned by `mod` when the handle exists and its type matches.
config::ConfigVarBase* config_find_var(
    LoadedMod& mod, ConfigVarHandle handle, uint32_t expectedType);

// Marks the mods' config keys dirty for the config service's debounced save. The loader
// calls this for the enabled cvars it owns.
void config_mark_dirty();

}  // namespace dusk::mods::svc
