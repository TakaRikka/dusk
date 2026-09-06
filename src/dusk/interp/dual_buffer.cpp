#include "dusk/interp/dual_buffer.h"

#include <absl/container/flat_hash_map.h>
#include <vector>

namespace dusk::interp {
namespace {

struct Slot {
    const void* type;
    void* ptr;
    void (*destroy)(void*);
};

using OwnerMap = absl::flat_hash_map<uintptr_t, std::vector<Slot>>;

OwnerMap& owner_map() {
    static OwnerMap s_map;
    return s_map;
}

}  // namespace

void* detail::acquire(const void* key, const void* type, void* (*make)(), void (*destroy)(void*)) {
    const uintptr_t id = reinterpret_cast<uintptr_t>(key);
    auto& slots = owner_map()[id];
    for (Slot& slot : slots) {
        if (slot.type == type) {
            return slot.ptr;
        }
    }

    void* ptr = make();
    slots.push_back({type, ptr, destroy});
    return ptr;
}

void erase_owned_buffers(const void* key) {
    if (key == nullptr) {
        return;
    }

    OwnerMap& stored = owner_map();
    auto it = stored.find(reinterpret_cast<uintptr_t>(key));
    if (it == stored.end()) {
        return;
    }

    for (Slot& slot : it->second) {
        slot.destroy(slot.ptr);
    }
    stored.erase(it);
}

void clear_owned_buffers() {
    OwnerMap& stored = owner_map();
    for (auto& entry : stored) {
        for (Slot& slot : entry.second) {
            slot.destroy(slot.ptr);
        }
    }
    stored.clear();
}

}  // namespace dusk::interp
