#pragma once

#include <cstddef>
#include <cstdint>

namespace dusk::mods::svc {

// Apply registered patch/delete edits for the current stage/layer to the record about to spawn
bool stage_apply_actor_edits(void* actorData, void* actorPrm, int8_t roomNo);

// Creates registered new actors from their record's on room load
void stage_create_new_actors(
    int8_t roomNo, void (*createFn)(void* user, const void* record, size_t size), void* user);

}  // namespace dusk::mods
