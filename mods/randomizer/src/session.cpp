#include "session.hpp"

#include <mods/svc/log.hpp>

#include "randomizer_context.hpp"
#include "hooks.hpp"
#include "ui/ui.hpp"
#include "flags.h"
#include "item_ids.h"
#include "tools.h"
#include "stages.h"
#include "item.hpp"
#include "messages.hpp"
#include "verify_item_functions.h"

#include "d/d_com_inf_game.h"
#include "d/d_item.h"
#include "d/d_meter2_info.h"

#include <mods/items.h>

#include <cstdlib>
#include <cstring>
#include <optional>
#include <string_view>

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

std::optional<int> parse_stage_check(const char* name, std::string_view prefix) {
    if (std::strncmp(name, prefix.data(), prefix.size()) != 0) {
        return std::nullopt;
    }
    const char* stage = name + prefix.size();
    if (*stage == '\0' || std::strchr(stage, ':') != nullptr) {
        return std::nullopt;
    }
    const int stageId = getStageID(stage);
    return stageId >= 0 ? std::optional{stageId} : std::nullopt;
}

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

std::optional<u16> parse_flag_check(const char* name, std::string_view prefix) {
    if (std::strncmp(name, prefix.data(), prefix.size()) != 0) {
        return std::nullopt;
    }
    const char* value = name + prefix.size();
    char* end = nullptr;
    const unsigned long flag = std::strtoul(value, &end, 10);
    if (value == end || *end != '\0' || flag > 0xFFFF) {
        return std::nullopt;
    }
    return static_cast<u16>(flag);
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
        *out_item = static_cast<uint8_t>(verifyProgressiveItem(it->second.itemId));
        return true;
    }

    if (auto key = parse_derived(info->name, ITEM_CHECK_CHEST_PREFIX)) {
        return lookup_override(ctx.mTreasureChestOverrides, key->key, out_item, false);
    }
    if (auto key = parse_derived(info->name, ITEM_CHECK_FREESTANDING_PREFIX)) {
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
    if (auto flag = parse_flag_check(info->name, ITEM_CHECK_GOLDEN_WOLF_PREFIX)) {
        return lookup_override(ctx.mGoldenWolfOverrides, *flag, out_item, true);
    }
    if (auto key = parse_derived(info->name, ITEM_CHECK_POE_PREFIX)) {
        return lookup_override(ctx.mPoeOverrides, key->key, out_item, true);
    }
    if (auto stageId = parse_stage_check(info->name, ITEM_CHECK_BOSS_PREFIX)) {
        const u16 key = static_cast<u16>((*stageId << 8) | 0x9F);
        return lookup_override(ctx.mFreestandingItemOverrides, key, out_item, true);
    }
    if (auto key = parse_derived(info->name, ITEM_CHECK_SHOP_PREFIX)) {
        return lookup_override(ctx.mShopOverrides, key->key, out_item, true);
    }
    if (auto key = parse_derived(info->name, ITEM_CHECK_SKY_PREFIX)) {
        return lookup_override(ctx.mSkyCharacterOverrides, key->key, out_item, true);
    }

    constexpr std::string_view bugPrefix{ITEM_CHECK_BUG_PREFIX};
    if (std::strncmp(info->name, bugPrefix.data(), bugPrefix.size()) == 0) {
        const u8 insect = static_cast<u8>(std::atoi(info->name + bugPrefix.size()));
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
    if (const auto it = ctx.mItemLocations.find(info->check_name);
        it != ctx.mItemLocations.end())
    {
        randomizer_setTempFlag(it->second);
    }

    if (auto key = parse_derived(info->check_name, ITEM_CHECK_FREESTANDING_PREFIX);
        key && key->stage_id == Ook)
    {
        if (const auto it = ctx.mItemLocations.find("Forest Temple Gale Boomerang");
            it != ctx.mItemLocations.end())
        {
            randomizer_setTempFlag(it->second);
        }
    }
}

bool activateSeed(const char* hash) {
    auto& ctx = randomizer_GetContext();
    ctx = RandomizerContext();
    if (auto err = ctx.LoadFromHash(hash); err.has_value() || ctx.mHash.empty()) {
        mods::log::error("failed to load seed {}", hash);
        return false;
    }

    if (messages::activate(ctx) != MOD_OK) {
        ctx = RandomizerContext{};
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

    messages::deactivate();
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
        for (const auto& [crc, actor] : patches) {
            StageActorHandle handle{};

            ModResult res;
            if (actor.bytes.size() == RandomizerContext::OBJ_DELETE_SIZE) {
                res = svc_mng.stage->delete_actor(mod_ctx, stage, room, layer, crc, &handle);
            } else {
                res = svc_mng.stage->patch_actor(mod_ctx, stage, room, layer, crc,
                    actor.bytes.data(), actor.bytes.size(), &handle);
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
        for (const auto& actor : additions) {
            StageActorHandle handle{};
            ModResult rt;
            rt = svc_mng.stage->add_actor(mod_ctx, stage, room, layer, actor.bytes.data(),
                actor.bytes.size(), &handle);
            if (rt == MOD_OK) {
                s_stage_edits.push_back(handle);
            }
        }
    }
}

// Set link to spawn outside of his house if we are playing with entrance rando and aren't already spawning in a dungeon
void registerStartingLocation() {
    auto& ctx = randomizer_GetContext();
    
    if (ctx.mEntranceOverrides.empty()) {
        return;
    }

    // Check that we aren't playing only with the setting Mirror Chamber Access set to Closed
    bool isExclusivelyMirrorChamber = false;
    const auto& mirrorChamberIt = ctx.mEntranceOverrides.find(RandomizerContext::EntranceOverride{
        .stageId = StageIDs::Mirror_Chamber,
        .roomNo = 4,
        .mapLayer = -1,
        .pointNo = 0
    });
    if (ctx.mEntranceOverrides.size() == 1 && mirrorChamberIt != ctx.mEntranceOverrides.end()) {
        if (mirrorChamberIt->second == RandomizerContext::EntranceOverride{
            .stageId = StageIDs::Bulblin_Camp,
            .roomNo = 3,
            .mapLayer = -1,
            .pointNo = 3,
        }) {
            isExclusivelyMirrorChamber = true;
        }
    }
    
    if (isExclusivelyMirrorChamber) {
        return;
    }

    dSv_player_return_place_c& returnPlace = dComIfGs_getSaveData()->mPlayer.getPlayerReturnPlace();
    
    // If we are loading into a dungeon, preserve our original return place
    if (std::string(returnPlace.getName()).find("D_MN") == 0) {
        // Force-set the entrance without the override
        g_dComIfG_gameInfo.play.mNextStage.getStartStage()->set(returnPlace.getName(), returnPlace.getRoomNo(), returnPlace.mPlayerStatus, -1);
        return;
    }

    // Set the spawn to outside of link's house, which will get overrided later if we are replacing it
    returnPlace.set("F_SP103",1,1);
    dComIfGp_setNextStage("F_SP103", 1, 1, -1);
}

ModResult onNewSave(void*, ModError*) {
    const std::string hash = g_pending_seed_hash;
    if (hash.empty())
        return MOD_ERROR;

    if (!activateSeed(hash.c_str()))
        return MOD_ERROR;

    svc_mng.save->set_blob(svc_mng.mod_ctx, kSeedHashBlobName, hash.data(), hash.size());
    setupRandomizerFile();
    return MOD_OK;
}

ModResult onSaveLoaded(void*, ModError*) {
    size_t size = 0;
    if (svc_mng.save->get_blob(mod_ctx, kSeedHashBlobName, nullptr, &size) != MOD_OK || size == 0) {
        mods::log::error("seed_hash not found!");
        deactivateSeed();
        return MOD_ERROR;
    }

    std::string hash(size, '\0');
    if (svc_mng.save->get_blob(mod_ctx, kSeedHashBlobName, hash.data(), &size) != MOD_OK) {
        mods::log::error("failed to get seed_hash!");
        deactivateSeed();
        return MOD_ERROR;
    }

    if (randomizer_GetContext().mHash != hash) {
        deactivateSeed();
        activateSeed(hash.c_str());
    }

    loadAncientDocumentNum();
    return MOD_OK;
}

void onSaveWritten(ModContext*, uint32_t, void*) {
    const std::string hash = randomizer_GetContext().mHash;
    svc_mng.save->set_blob(svc_mng.mod_ctx, kSeedHashBlobName, hash.data(), hash.size());
}

TextureReplacementHandle logoTexHandle{};

ModResult onGameModeActivated(void*, ModError* error) {
    ModResult result = hooks::initialize();
    if (result != MOD_OK) {
        return mods::set_error(error, result, "failed to initialize hooks");
    }

    result = svc_mng.save->observe_saves(
        svc_mng.mod_ctx,
        nullptr,
        nullptr,
        onSaveWritten,
        nullptr,
        &s_save_observer);
    if (result != MOD_OK) {
        return mods::set_error(error, result, "failed to initialize save observation");
    }

    result = svc_mng.texture->register_file(mod_ctx, "res/tex1_608x100_0c1c70378fb8cb46_6.png", &logoTexHandle);
    if (result != MOD_OK) {
        return mods::set_error(error, result, "failed to register texture replacement");
    }

    result = ui::initialize();
    if (result != MOD_OK) {
        return mods::set_error(error, result, "failed to initialize ui");
    }

    mods::log::info("randomizer game mode activated");
    return MOD_OK;
}

void shutdown() {
    deactivateSeed();
    hooks::uninstall();
    ui::shutdown();
    svc_mng.save->unobserve_saves(mod_ctx, s_save_observer);
    svc_mng.texture->unregister(mod_ctx, logoTexHandle);
}

ModResult onGameModeDeactivated(void*, ModError* error) {
    shutdown();

    mods::log::info("randomizer game mode deactivated");
    return MOD_OK;
}

ModResult onGameModeUpdate(void*, ModError* error) {
    ui::update();
    session::update();
    return MOD_OK;
}

ModResult initialize(const ServiceManager& services) {
    svc_mng = services;

    const ModResult messageResult = messages::initialize();
    if (messageResult != MOD_OK) {
        return messageResult;
    }

    const GameModeDesc gameModeDesc = {
        .struct_size = sizeof(GameModeDesc),
        .game_mode_id = "randomizer",
        .full_name = "Randomizer",
        .save_name = "randomizer",
        .user_data = nullptr,
        .on_activated = onGameModeActivated,
        .on_deactivated = onGameModeDeactivated,
        .on_save_loaded = onSaveLoaded,
        .on_new_save = onNewSave,
        .on_tick = onGameModeUpdate,
    };
    svc_mng.game_mode->register_game_mode(mod_ctx, &gameModeDesc);
    return MOD_OK;
}

void update() {
    if (!g_randomizerState.mInitialized) {
        g_randomizerState._create();
    }
    g_randomizerState.execute();
}

}
