#pragma once

namespace dusk::mods {
struct LoadedMod;
}

namespace dusk::mods::svc {

void hook_resolve_mod_records(LoadedMod& mod);

}  // namespace dusk::mods::svc
