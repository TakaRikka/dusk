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
std::string g_pending_seed_hash{};

SaveObserverHandle s_save_observer{};
ItemCheckHandle s_check_resolver{};
ItemGiveHandle s_check_observer{};
std::vector<StageActorHandle> s_stage_edits{};

constexpr const char* kSeedHashBlobName = "seed_hash";

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

    if (std::strcmp(info->check_name, "dungeon_reward:D_MN10") == 0) {
        dComIfGs_onItem(0x9E, -1);
    } else if (auto key = parse_derived(info->check_name, "freestanding:");
               key && key->stage_id == Ook)
    {
        dComIfGs_onItem(0x9D, -1);
        randomizer_setTempFlagForLocation("Forest Temple Gale Boomerang");
    }
}

bool activateSeed(const char* hash) {
    auto& ctx = randomizer_GetContext();
    ctx = RandomizerContext();
    if (auto err = ctx.LoadFromHash(hash); err.has_value() || ctx.mHash.empty()) {
        mods::log::error("failed to load seed {}", hash);
        return false;
    }

    item::apply_item_data_tables();

    svc_mng.item->set_check_resolver(mod_ctx, nullptr, resolve_check, nullptr, &s_check_resolver);
    svc_mng.item->observe_gives(mod_ctx, observe_give, nullptr, &s_check_observer);

    registerStageEdits();
    mods::log::info("activated seed {}", ctx.mHash);
    return true;
}

void deactivateSeed() {
    if (s_check_resolver != 0) {
        svc_mng.item->clear_check_resolver(mod_ctx, s_check_resolver);
        s_check_resolver = 0;
    }

    if (s_check_observer != 0) {
        svc_mng.item->unobserve_gives(mod_ctx, s_check_observer);
        s_check_observer = 0;
    }

    for (auto handle : s_stage_edits) {
        svc_mng.stage->remove_actor_edit(mod_ctx, handle);
    }
    s_stage_edits.clear();

    item::restore_item_data_tables();
    randomizer_GetContext() = RandomizerContext{};
    g_randomizerState = RandomizerState{};
}

void setupRandomizerFile() {
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

            if (res == MOD_OK) {
                s_stage_edits.push_back(handle);
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
            ModResult rt;
            rt = svc_mng.stage->add_actor(mod_ctx, stage, room, layer, bytes.data(), bytes.size(), &handle);
            if (rt == MOD_OK) {
                s_stage_edits.push_back(handle);
            }
        }
    }
}

void onNewSave(ModContext*, uint32_t, void*) {
    const std::string hash = g_pending_seed_hash;
    if (hash.empty())
        return;

    if (!activateSeed(hash.c_str()))
        return;

    svc_mng.save->set_blob(svc_mng.mod_ctx, kSeedHashBlobName, hash.data(), hash.size());
    setupRandomizerFile();
}

void onSaveLoaded(ModContext*, uint32_t, void*) {
    size_t size = 0;
    if (svc_mng.save->get_blob(mod_ctx, kSeedHashBlobName, nullptr, &size) != MOD_OK || size == 0) {
        mods::log::error("seed_hash not found!");
        deactivateSeed();
        return;
    }

    std::string hash(size, '\0');
    if (svc_mng.save->get_blob(mod_ctx, kSeedHashBlobName, hash.data(), &size) != MOD_OK) {
        mods::log::error("failed to get seed_hash!");
        deactivateSeed();
        return;
    }

    if (randomizer_GetContext().mHash != hash) {
        deactivateSeed();
        activateSeed(hash.c_str());
    }

    loadAncientDocumentNum();
}

ModResult initialize(const ServiceManager& services) {
    svc_mng = services;

    ModResult rt = svc_mng.save->observe_saves(
        svc_mng.mod_ctx,
        onNewSave,
        onSaveLoaded,
        nullptr,
        nullptr,
        &s_save_observer);
    if (rt != MOD_OK) {
        return rt;
    }

    return MOD_OK;
}

void update() {
    if (!g_randomizerState.mInitialized) {
        g_randomizerState._create();
    }
    g_randomizerState.execute();
}

}