#pragma once

#include "mods/svc/item.h"

#include <cstdint>

namespace dusk::mods {

struct LoadedMod;

namespace svc {

ModResult item_check_set_override(LoadedMod& mod, const char* name, uint8_t itemNo);
ModResult item_check_clear_override(LoadedMod& mod, const char* name);
ModResult item_check_add_resolver(LoadedMod& mod, const char* name, ItemCheckResolveFn fn,
    void* userData, ItemCheckHandle& outHandle);
ModResult item_check_remove_resolver(LoadedMod& mod, ItemCheckHandle handle);
void item_checks_remove_mod(LoadedMod& mod);

ModResult item_give_enqueue(LoadedMod& mod, const char* checkName, uint8_t itemNo, uint32_t flags);
ModResult item_give_add_observer(
    LoadedMod& mod, ItemGiveObserveFn fn, void* userData, ItemGiveHandle& outHandle);
ModResult item_give_remove_observer(LoadedMod& mod, ItemGiveHandle handle);
void item_gives_remove_mod(LoadedMod& mod);
void item_gives_tick();
void item_gives_clear();

}  // namespace svc
}  // namespace dusk::mods
