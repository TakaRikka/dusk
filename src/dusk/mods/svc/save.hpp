#pragma once

#include <cstdint>

namespace dusk::mods::svc {

void save_slot_new(uint32_t slot);
void save_slot_loaded(uint32_t slot, const void* slotData);
void save_slot_written(uint32_t slot, const void* slotData);
void save_slot_copied(uint32_t fromSlot, uint32_t toSlot);
void save_slot_erased(uint32_t slot);
void save_no_slot();

}  // namespace dusk::mods::svc
