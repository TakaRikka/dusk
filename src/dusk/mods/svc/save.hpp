#pragma once

#include <cstddef>
#include <cstdint>

// Save-lifecycle seam entry points for the mod save service, called from the slot-aware save
// paths (d_file_select.cpp, d_menu_save.cpp, d_s_logo.cpp quick-load, src/dusk/autosave.cpp).
// All three save slots live in one emulated-card file, so slot identity only exists at these
// game-layer call sites.
// slotData points at the slot's serialized save (QUEST_LOG_SIZE bytes) for the staleness
// snapshot; it is read synchronously during the call.

namespace dusk::mods::svc {

// A new file was finalized in file select (name entry complete) for slot.
void save_slot_new(uint32_t slot);
// slot was decoded into the live game info (file select start / quick-load).
void save_slot_loaded(uint32_t slot, const void* slotData);
// slot was successfully written to the card (menu save / autosave).
void save_slot_written(uint32_t slot, const void* slotData);
// File-select copy/erase (both rewrite the card; the sidecar follows).
void save_slot_copied(uint32_t fromSlot, uint32_t toSlot);
void save_slot_erased(uint32_t slot);
// File select opened: no slot is current until load/new fires again.
void save_no_slot();

}  // namespace dusk::mods
