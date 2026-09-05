#pragma once

#include "mods/svc/item.h"

#include <cstdint>
#include <unordered_map>
#include <utility>

class fopAc_ac_c;

namespace dusk::mods {

enum class ItemGiveMode : uint8_t {
    Demo,
    Silent,
    ForcedDemo,
};

struct ItemCheckResult {
    uint32_t tag = 0;
    uint8_t itemNo = 0;
    uint8_t displayItemNo = 0;
    bool was_resolved = false;
};

ItemCheckResolution item_check_resolve(const char* name, uint8_t itemNo, fopAc_ac_c* giver);
ItemCheckResolution item_check_resolve(uint32_t giveTag, uint8_t itemNo, fopAc_ac_c* giver);
uint8_t item_check(const char* name, uint8_t itemNo, fopAc_ac_c* giver);
ItemCheckResult item_check_commit(const char* name, uint8_t itemNo, fopAc_ac_c* giver);
ItemCheckResult item_check_commit(uint32_t giveTag, uint8_t itemNo, fopAc_ac_c* giver);

uint8_t item_check_chest(uint8_t boxNo, uint8_t itemNo, fopAc_ac_c* chest);
uint8_t item_check_boss(uint8_t itemNo, fopAc_ac_c* boss);
uint8_t item_check_freestanding(uint8_t bitNo, uint8_t itemNo, fopAc_ac_c* item);
uint8_t item_check_golden_wolf(uint16_t eventFlag, uint8_t itemNo, fopAc_ac_c* item);
uint8_t item_check_shop(uint8_t itemNo, fopAc_ac_c* giver);

uint32_t item_give_tag(const char* name);
uint32_t item_give_tag_chest(uint8_t boxNo);
uint32_t item_give_tag_boss();
uint32_t item_give_tag_freestanding(uint8_t bitNo);
uint32_t item_give_tag_golden_wolf(uint16_t eventFlag);
uint32_t item_give_tag_poe(uint8_t bitNo);
uint32_t item_give_tag_shop(uint8_t itemNo);
uint32_t item_give_tag_bug(uint8_t insectId);
uint32_t item_give_tag_sky_character();

void item_check_enqueue_deferred(const char* name, uint8_t itemNo);
bool item_check_enqueue(ItemCheckResult check, ItemGiveMode mode);

void item_check_complete(ItemCheckResult check, fopAc_ac_c* giver);
void item_check_cancel(uint32_t giveTag);
void item_check_clear_committed();
void item_granted(uint8_t itemNo, uint32_t giveTag, fopAc_ac_c* giver);

uint32_t item_check_message(uint16_t group, uint32_t messageId);

bool item_give_queue_dispatching();
uint32_t item_give_queue_take_tag();

struct BossItemActorSpeeds {
    f32 f;
    f32 y;
};
void set_boss_item_actor_speeds(f32 f, f32 y);
BossItemActorSpeeds get_boss_item_actor_speeds();

namespace detail {
struct CommittedCheck {
    uint8_t vanillaItem = 0;
    ItemCheckResolution resolution{};
    bool queued = false;
};

class ItemCommitStore {
public:
    CommittedCheck* find(uint32_t giveTag) {
        const auto it = mChecks.find(giveTag);
        return it != mChecks.end() ? &it->second : nullptr;
    }

    const CommittedCheck* find(uint32_t giveTag) const {
        const auto it = mChecks.find(giveTag);
        return it != mChecks.end() ? &it->second : nullptr;
    }

    template <typename ResolveFn>
    CommittedCheck& commit(uint32_t giveTag, uint8_t vanillaItem, ResolveFn&& resolve) {
        const auto existing = mChecks.find(giveTag);
        if (existing != mChecks.end()) {
            return existing->second;
        }

        const ItemCheckResolution resolution = std::forward<ResolveFn>(resolve)();
        return mChecks
            .emplace(giveTag,
                CommittedCheck{
                    .vanillaItem = vanillaItem,
                    .resolution = resolution,
                })
            .first->second;
    }

    bool contains(uint32_t giveTag) const { return mChecks.contains(giveTag); }

    void erase(uint32_t giveTag) { mChecks.erase(giveTag); }

    void clear() { mChecks.clear(); }

    bool empty() const { return mChecks.empty(); }

private:
    std::unordered_map<uint32_t, CommittedCheck> mChecks;
};
}  // namespace detail

}  // namespace dusk::mods
