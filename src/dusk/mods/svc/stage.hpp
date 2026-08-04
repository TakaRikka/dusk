#pragma once

#include <cstddef>
#include <cstdint>

namespace dusk::mods::svc {

bool stage_apply_actor_edits(void* actorData, void* actorPrm, size_t recordSize, int8_t roomNo);

void stage_create_new_actors(
    int8_t roomNo, void (*createFn)(void* user, const void* record, size_t size), void* user);

}  // namespace dusk::mods::svc
