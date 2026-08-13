#include "session.hpp"

#include <mods/svc/log.hpp>

#include "randomizer_context.hpp"
#include "flags.h"
#include "item_ids.h"
#include "tools.h"
#include "stages.h"
#include "item.hpp"
#include "verify_item_functions.h"

#include "d/d_com_inf_game.h"
#include "d/d_item.h"
#include "d/d_meter2_info.h"

namespace randomizer::session {
ServiceManager svc_mng;

ModResult initialize(const ServiceManager& services) {
    svc_mng = services;

    return MOD_OK;
}

ItemCheckHandle s_check_resolver{};
ItemGiveHandle s_check_observer{};

struct DerivedKey {
    int stage_id;
    u16 key;
};

std::optional<DerivedKey> parse_derived(const char* name, std::string_view prefix) {
    if (std::strncmp(name, prefix.data(), prefix.size()) != 0) {
        return std::nullopt;
    }
    const char* stage_begin = name + prefix.size();
    const char* stage_end = std::strchr(stage_begin, ':');
    if (stage_end == nullptr) {
        return std::nullopt;
    }
    const std::string stage{stage_begin, stage_end};
    const int stage_id = getStageID(stage.c_str());
    if (stage_id < 0) {
        return std::nullopt;
    }
    const int n = std::atoi(stage_end + 1);
    return DerivedKey{stage_id, static_cast<u16>((stage_id << 8) | (n & 0xFF))};
}

template <typename Map>
bool lookup_override(const Map& map, u16 key, uint8_t* out_item, bool progressive) {
    const auto it = map.find(key);
    if (it == map.end()) {
        return false;
    }
    *out_item = progressive ? static_cast<uint8_t>(verifyProgressiveItem(it->second)) : it->second;
    return true;
}

bool resolve_check(ModContext*, const ItemCheckInfo* info, uint8_t* out_item, void*) {
    auto& ctx = randomizer_GetContext();

    if (auto it = ctx.mItemLocations.find(info->name); it != ctx.mItemLocations.end()) {
        *out_item = static_cast<uint8_t>(it->second.itemId);
        return true;
    }

    if (auto key = parse_derived(info->name, "chest:")) {
        return lookup_override(ctx.mTreasureChestOverrides, key->key, out_item, false);
    }
    if (auto key = parse_derived(info->name, "freestanding:")) {
        if (key->stage_id == Ook) {
            if (auto it = ctx.mItemLocations.find("Forest Temple Gale Boomerang");
                it != ctx.mItemLocations.end()) {
                *out_item = static_cast<uint8_t>(verifyProgressiveItem(it->second.itemId));
                return true;
            }
            return false;
        }
        return lookup_override(ctx.mFreestandingItemOverrides, key->key, out_item, true);
    }
    if (auto key = parse_derived(info->name, "poe:")) {
        return lookup_override(ctx.mPoeOverrides, key->key, out_item, false);
    }
    if (auto key = parse_derived(info->name, "shop:")) {
        return lookup_override(ctx.mShopOverrides, key->key, out_item, true);
    }
    if (auto key = parse_derived(info->name, "sky:")) {
        return lookup_override(ctx.mSkyCharacterOverrides, key->key, out_item, true);
    }

    if (std::strncmp(info->name, "bug:", 4) == 0) {
        const u8 insect = static_cast<u8>(std::atoi(info->name + 4));
        if (auto it = ctx.mBugRewardOverrides.find(insect); it != ctx.mBugRewardOverrides.end()) {
            *out_item = static_cast<uint8_t>(verifyProgressiveItem(it->second));
            return true;
        }
        return false;
    }

    return false;
}

void observe_give(ModContext*, const ItemGiveInfo* info, void*) {
    if (info->check_name == nullptr) {
        return;
    }

    auto& ctx = randomizer_GetContext();
    if (ctx.mItemLocations.contains(info->check_name)) {
        randomizer_setTempFlagForLocation(info->check_name);
    }

    if (std::strcmp(info->check_name, "Arbiters Grounds Dungeon Reward") == 0) {
        dComIfGs_onItem(0x9E, -1);
    } else if (auto key = parse_derived(info->check_name, "freestanding:");
               key && key->stage_id == Ook)
    {
        dComIfGs_onItem(0x9D, -1);
        randomizer_setTempFlagForLocation("Forest Temple Gale Boomerang");
    }
}

void activateSeed() {
    auto& ctx = randomizer_GetContext();

    item::apply_item_data_tables();

    svc_mng.item->set_check_resolver(mod_ctx, nullptr, resolve_check, nullptr, &s_check_resolver);
    svc_mng.item->observe_gives(mod_ctx, observe_give, nullptr, &s_check_observer);
}

void setupRandomizerFile() {
    activateSeed();

    // Setup file based on randomizer data
    auto& randoData = randomizer_GetContext();
    randoData.mCreatingSave = true;

    // Set starting flags
    // Event Flags
    for (const auto& flag : randoData.mStartEventFlags) {
        dComIfGs_onEventBit(flag);
    }
    // Region Flags
    for (const auto& [region, flags] : randoData.mStartRegionFlags) {
        for (const auto& flag : flags) {
            onRegionFlag(region, flag);
        }
    }

    // Map bits (fills in overworld on map)
    setRegionBit(randoData.mMapBits);

    // Other flags based on starting flags
    if (dComIfGs_isEventBit(CLEARED_FARON_TWILIGHT))
    {
        dComIfGs_onDarkClearLV(0);
        dComIfGs_setLightDropNum(0, 0x10);
        item::exec_item_get(dItemNo_Randomizer_DROP_CONTAINER_e);
        item::exec_item_get(dItemNo_Randomizer_WEAR_KOKIRI_e);
    }

    if (dComIfGs_isEventBit(CLEARED_ELDIN_TWILIGHT))
    {
        dComIfGs_onDarkClearLV(1);
        dComIfGs_setLightDropNum(1, 0x10);
        item::exec_item_get(dItemNo_Randomizer_DROP_CONTAINER02_e);
    }

    if (dComIfGs_isEventBit(CLEARED_LANAYRU_TWILIGHT))
    {
        dComIfGs_onDarkClearLV(2);
        dComIfGs_setLightDropNum(2, 0x10);
        item::exec_item_get(dItemNo_Randomizer_DROP_CONTAINER03_e);
    }

    if (randoData.mSettings[RandomizerContext::SKIP_MINOR_CUTSCENES] == RandomizerContext::ON)
    {
        // Add letter data in this order to more or less reflect an order they can be obtained in game
        static const int letterOrder[] = {3, 2, 4, 7, 5, 6, 13, 12, 10, 9, 8, 15, 0, 14, 11};
        int letterNum = 0;
        for (int i : letterOrder) {
            if (dMenu_Letter::getLetterName(i) != 0) {
                dComIfGs_onLetterGetFlag(i);
                dComIfGs_setGetNumber(letterNum++, i + 1);
            }
        }
        setAllLetterRead();
    }

    // If MDH and the twilights are pre-completed
    if (dComIfGs_isEventBit(MIDNAS_DESPERATE_HOUR_COMPLETED))
    {
        if ((dComIfGs_getSaveData()->getPlayer().getPlayerStatusB().mDarkClearLevelFlag & 0x7) == 0x7)
        {
            dComIfGs_onDarkClearLV(3);
            dComIfGs_onTransformLV(3); // Puts Midna on players back
        }
    }

    // Set starting inventory
    for (const auto& itemId: randoData.mStartingInventory) {
        item::exec_item_get(itemId);
    }

    g_randomizerState = RandomizerState();
    mods::log::debug("Created Rando Save");
    randoData.mCreatingSave = false;
}

void registerStageEdits() {
    auto& ctx = randomizer_GetContext();
    auto stage_of = [](u32 key) -> const char* {
        const u32 stage_id = key >> 16;
        if (stage_id >= sizeof(allStages) / sizeof(allStages[0])) {
            return nullptr;
        }
        return allStages[stage_id];
    };

    for (const auto& [key, patches] : ctx.mObjectPatches) {
        const char* stage = stage_of(key);
        if (stage == nullptr) {
            continue;
        }

        const u8 room = (key >> 8) & 0xFF;
        const s8 layer = static_cast<s8>(key & 0xFF);
        for (const auto& [crc, bytes] : patches) {
            StageActorHandle handle{};

            ModResult res;
            if (bytes.size() == RandomizerContext::OBJ_DELETE_SIZE) {
                res = svc_mng.stage->delete_actor(mod_ctx, stage, room, layer, crc, &handle);
            } else {
                res = svc_mng.stage->patch_actor(
                    mod_ctx, stage, room, layer, crc, bytes.data(), bytes.size(), &handle);
            }
        }
    }

    for (const auto& [key, additions] : ctx.mObjectAdditions) {
        const char* stage = stage_of(key);
        if (stage == nullptr) {
            continue;
        }

        const u8 room = (key >> 8) & 0xFF;
        const s8 layer = static_cast<s8>(key & 0xFF);
        for (const auto& bytes : additions) {
            StageActorHandle handle{};
            svc_mng.stage->add_actor(mod_ctx, stage, room, layer, bytes.data(), bytes.size(), &handle);
        }
    }
}

}