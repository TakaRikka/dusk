#pragma once

#include <cstddef>
#include <cstdint>
#include <limits>
#include <optional>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <vector>

namespace dusk::mods {

struct LoadedMod;

namespace svc {

template <typename T>
class SlotMap {
public:
    static_assert(std::is_nothrow_move_constructible_v<T>);

    using Handle = uint64_t;
    static constexpr Handle InvalidHandle = 0;

    struct Entry {
        LoadedMod* owner = nullptr;
        T value;
    };

    template <typename... Args>
    Handle emplace(LoadedMod& owner, Args&&... args) {
        T value{std::forward<Args>(args)...};
        const auto index = allocate_index();
        auto& slot = m_slots[index];
        slot.entry.emplace(Entry{.owner = &owner, .value = std::move(value)});
        return make_handle(index, slot.generation);
    }

    // Returned pointers remain valid only until the next mutating operation.
    Entry* find(Handle handle) {
        auto* slot = find_slot(handle);
        return slot != nullptr ? &*slot->entry : nullptr;
    }

    const Entry* find(Handle handle) const {
        const auto* slot = find_slot(handle);
        return slot != nullptr ? &*slot->entry : nullptr;
    }

    Entry* find_owned(Handle handle, const LoadedMod& owner) {
        auto* entry = find(handle);
        return entry != nullptr && entry->owner == &owner ? entry : nullptr;
    }

    const Entry* find_owned(Handle handle, const LoadedMod& owner) const {
        const auto* entry = find(handle);
        return entry != nullptr && entry->owner == &owner ? entry : nullptr;
    }

    std::optional<Entry> take(Handle handle) {
        const auto index = handle_index(handle);
        auto* slot = find_slot(handle);
        if (slot == nullptr) {
            return std::nullopt;
        }
        std::optional<Entry> entry{std::move(slot->entry)};
        release_slot(index);
        return entry;
    }

    std::optional<Entry> take_owned(Handle handle, const LoadedMod& owner) {
        if (find_owned(handle, owner) == nullptr) {
            return std::nullopt;
        }
        return take(handle);
    }

    std::vector<Entry> take_all(const LoadedMod& owner) {
        std::vector<Entry> entries;
        for (size_t slotIndex = 0; slotIndex < m_slots.size(); ++slotIndex) {
            const auto index = static_cast<uint32_t>(slotIndex);
            auto& slot = m_slots[index];
            if (!slot.entry.has_value() || slot.entry->owner != &owner) {
                continue;
            }
            entries.push_back(std::move(*slot.entry));
            release_slot(index);
        }
        return entries;
    }

    bool erase(Handle handle) {
        const auto index = handle_index(handle);
        if (find_slot(handle) == nullptr) {
            return false;
        }
        release_slot(index);
        return true;
    }

    bool erase_owned(Handle handle, const LoadedMod& owner) {
        if (find_owned(handle, owner) == nullptr) {
            return false;
        }
        return erase(handle);
    }

    size_t erase_all(const LoadedMod& owner) {
        return take_all(owner).size();
    }

    template <typename Fn>
    void for_each(Fn&& fn) const {
        // The visitor may inspect entries but must not mutate this SlotMap.
        for (size_t slotIndex = 0; slotIndex < m_slots.size(); ++slotIndex) {
            const auto index = static_cast<uint32_t>(slotIndex);
            const auto& slot = m_slots[index];
            if (slot.entry.has_value()) {
                fn(make_handle(index, slot.generation), *slot.entry);
            }
        }
    }

private:
    struct Slot {
        uint32_t generation = 1;
        std::optional<Entry> entry;
    };

    static constexpr Handle make_handle(uint32_t index, uint32_t generation) {
        return static_cast<Handle>(generation) << 32 | index;
    }

    static constexpr uint32_t handle_index(Handle handle) {
        return static_cast<uint32_t>(handle & std::numeric_limits<uint32_t>::max());
    }

    static constexpr uint32_t handle_generation(Handle handle) {
        return static_cast<uint32_t>(handle >> 32);
    }

    Slot* find_slot(Handle handle) {
        return const_cast<Slot*>(std::as_const(*this).find_slot(handle));
    }

    const Slot* find_slot(Handle handle) const {
        const auto index = handle_index(handle);
        if (handle == InvalidHandle || index >= m_slots.size()) {
            return nullptr;
        }
        const auto& slot = m_slots[index];
        if (!slot.entry.has_value() || slot.generation != handle_generation(handle)) {
            return nullptr;
        }
        return &slot;
    }

    uint32_t allocate_index() {
        if (!m_freeSlots.empty()) {
            const auto index = m_freeSlots.back();
            m_freeSlots.pop_back();
            return index;
        }
        if (m_slots.size() > std::numeric_limits<uint32_t>::max()) {
            throw std::length_error{"SlotMap handle space exhausted"};
        }
        const auto index = static_cast<uint32_t>(m_slots.size());
        m_slots.emplace_back();
        return index;
    }

    void release_slot(uint32_t index) {
        auto& slot = m_slots[index];
        slot.entry.reset();
        if (slot.generation == std::numeric_limits<uint32_t>::max()) {
            return;
        }
        ++slot.generation;
        m_freeSlots.push_back(index);
    }

    std::vector<Slot> m_slots;
    std::vector<uint32_t> m_freeSlots;
};

}  // namespace svc
}  // namespace dusk::mods
