#include "tools.h"

#include "../generator/logic/world.hpp"
#include "d/actor/d_a_alink.h"
#include "d/d_com_inf_game.h"
#include "d/d_item.h"
#include "d/d_item_data.h"
#include "f_op/f_op_actor_mng.h"
#include "item_ids.h"
#include "randomizer_context.hpp"
#include "session.hpp"
#include "stages.h"
#include "verify_item_functions.h"

#include <mods/items.h>
#include <mods/svc/log.hpp>

bool playerIsInRoomStage(s32 room, const char* stage)
{
    // Only check room if it is valid
    // Room numbers are normally stored as int8_t, so the highest positive value is 127
    if ((room < 0) || (room > 127))
    {
        return false;
    }

    if (room != dStage_roomControl_c::mStayNo)
    {
        return false;
    }

    // Only check stage if it is valid
    if (!stage)
    {
        return false;
    }

    return daAlink_c::checkStageName(stage);
}

void checkTransformFromWolf()
{
    if (dComIfGs_getTransformStatus())
    {
        daAlink_getAlinkActorClass()->procCoMetamorphoseInit();
    }
}

u8 setNextWarashibeItem()
{
    static const u8 questItemsList[] = {
        dItemNo_Randomizer_LETTER_e,
        dItemNo_Randomizer_BILL_e,
        dItemNo_Randomizer_WOOD_STATUE_e,
        dItemNo_Randomizer_IRIAS_PENDANT_e,
        dItemNo_Randomizer_HORSE_FLUTE_e
    };

    u32 listLength = sizeof(questItemsList) / sizeof(questItemsList[0]);

    u8 newItem = 0xFF; // null by default

    for (u32 i = 0; i < listLength; i++)
    {
        const u32 item = questItemsList[i];
        const u8 slotItem = dComIfGs_getItem(21, 0);
        if (item == slotItem)
        {
            newItem = item;
            u32 j = i;
            do
            {
                j = (j + 1) % listLength; // Move to next index, wrapping around if needed.
                if (checkItemGet(questItemsList[j], 1))
                {
                    newItem = questItemsList[j];
                    break;
                }
            } while (j != i);

            // If the item to switch to is the same as the current item and we don't have the item anymore, null the slot
            if ((newItem == item) && !checkItemGet(item, 1))
            {
                newItem = 0xFF;
            }
            dComIfGs_setItem(21, newItem);

            break;
        }
    }
    return newItem;
}

void offWarashibeItem(u8 item)
{
    g_dComIfG_gameInfo.info.getSavedata().getPlayer().getGetItem().offFirstBit(item);
    setNextWarashibeItem();
}

int initCreatePlayerItem(u32 item, u32 flag, const cXyz* pos, int roomNo, const csXyz* angle, const cXyz* scale)
{
    u32 params = 0xFF0000 | ((flag & 0xFF) << 0x8) | (item & 0xFF);
    return fopAcM_create(539, params, pos, roomNo, angle, scale, -1);
}

int getStageID(const char* stage)
{
    int loopCount = sizeof(allStages) / sizeof(allStages[0]);
    for (int i = 0; i < loopCount; i++)
    {
        // If stage is NULL, check for current stage
        if (stage == NULL) {
            if (daAlink_c::checkStageName(allStages[i])) return i;
        } else if (strcmp(stage, allStages[i]) == 0) {
            return i;
        }
    }
    // Didn't find the current stage for some reason
    return -1;
}

bool playerIsOnTitleScreen() {
    // Player is either on title screen movie stage (S_MV000) or on title screen map layer 10
    return strcmp(dComIfGp_getStartStageName(), "S_MV000") == 0 ||
          (strcmp(dComIfGp_getStartStageName(), "F_SP102") == 0 && dComIfG_play_c::getLayerNo(0) == 10);
}

u16 getItemMessageID(u8 itemId) {
    // If heart piece, choose from the different heart piece messages
    if (itemId == dItemNo_Randomizer_KAKERA_HEART_e) {
        static u32 const heartPieceMessage[5] = {0x86, 0x9C, 0x9D, 0x9E, 0x9F};
        return heartPieceMessage[dComIfGs_getMaxLife() % 5];
    }

    return itemId + 0x65;
}

int numCompletedDungeons() {
    int numCompleted{0};
    // Loop through dungeon area node ids
    for (int i = 0x10; i < 0x18; ++i) {
        numCompleted += dComIfGs_isStageBossEnemy(i);
    }
    return numCompleted;
}

int numFusedShadows() {
    int numFusedShadows{0};
    for (int i = 0; i < 3; ++i) {
        numFusedShadows += dComIfGs_isCollectCrystal(i);
    }
    return numFusedShadows;
}

int numMirrorShards() {
    int numMirrorShards{0};
    for (int i = 0; i < 4; ++i) {
        numMirrorShards += dComIfGs_isCollectMirror(i);
    }
    return numMirrorShards;
}

int getTempleKeysFound(int saveId) {
    static std::unordered_map<int, std::vector<int>> keyDoorFlags = {
        {0xA,  {0x0}},
        {0x10, {0x7,  0xB,  0x2B, 0x3E}},
        {0x11, {0x33, 0x3D, 0x3F}},
        {0x12, {0x23, 0x24, 0x34}},
        {0x13, {0x27, 0x46, 0x4D, 0x5A, 0x5B}},
        {0x14, {0x2B, 0x2C, 0x2F, 0x30}},
        {0x15, {0x1B, 0x1C, 0x1D}},
        {0x16, {0x6}},
        {0x17, {0x6,  0x7,   0x8, 0xB, 0x23, 0x24, 0x25}},
        {0x18, {0x4C, 0x6F, 0x7C}}
    };

    int count = getAreaKeyNum(saveId);

    // Add number of unlocked key doors for this dungeon to current key count
    for (auto flag : keyDoorFlags[saveId]) {
        if (tracker_isStageSwitch(saveId, flag)) {
            count += 1;
        }
    }

    return count;
}

bool isTempleBigKeyFound(int stage) {
    // The boss key never gets taken away unlike small keys
    return dComIfGs_isDungeonItemBossKey(stage);
}

randomizer::logic::item_pool::ItemPool getSaveItemPool(randomizer::logic::world::World* world) {
    randomizer::logic::item_pool::ItemPool pool{};

    // Item wheel items
    for (int i = 0; i < MAX_ITEM_SLOTS; ++i) {
        switch (dComIfGs_getItem(i, false)) {
        case dItemNo_Randomizer_HAWK_EYE_e:
            pool.push_back(world->GetItem("Hawkeye", true));
            break;
        case dItemNo_Randomizer_BOOMERANG_e:
            pool.push_back(world->GetItem("Gale Boomerang", true));
            break;
        case dItemNo_Randomizer_SPINNER_e:
            pool.push_back(world->GetItem("Spinner", true));
            break;
        case dItemNo_Randomizer_IRONBALL_e:
            pool.push_back(world->GetItem("Ball and Chain", true));
            break;
        case dItemNo_Randomizer_BOW_e:
            pool.push_back(world->GetItem("Progressive Bow", true));
            break;
        case dItemNo_Randomizer_W_HOOKSHOT_e:
            pool.push_back(world->GetItem("Progressive Clawshot", true));
            [[fallthrough]];
        case dItemNo_Randomizer_HOOKSHOT_e:
            pool.push_back(world->GetItem("Progressive Clawshot", true));
            break;
        case dItemNo_Randomizer_HVY_BOOTS_e:
            pool.push_back(world->GetItem("Iron Boots", true));
            break;
        case dItemNo_Randomizer_COPY_ROD_e:
            pool.push_back(world->GetItem("Progressive Dominion Rod", true));
            // Powered up dominion rod
            if (dComIfGs_isEventBit(0x2580)) {
                pool.push_back(world->GetItem("Progressive Dominion Rod", true));
            }
            break;
        case dItemNo_Randomizer_KANTERA_e:
            pool.push_back(world->GetItem("Lantern", true));
            break;
        case dItemNo_Randomizer_JEWEL_ROD_e:
        case dItemNo_Randomizer_JEWEL_BEE_ROD_e:
        case dItemNo_Randomizer_JEWEL_WORM_ROD_e:
            pool.push_back(world->GetItem("Progressive Fishing Rod", true));
            [[fallthrough]];
        case dItemNo_Randomizer_FISHING_ROD_1_e:
        case dItemNo_Randomizer_BEE_ROD_e:
        case dItemNo_Randomizer_WORM_ROD_e:
            pool.push_back(world->GetItem("Progressive Fishing Rod", true));
            break;
        case dItemNo_Randomizer_PACHINKO_e:
            pool.push_back(world->GetItem("Slingshot", true));
            break;
        case dItemNo_Randomizer_BOMB_BAG_LV1_e:
        case dItemNo_Randomizer_NORMAL_BOMB_e:
        case dItemNo_Randomizer_WATER_BOMB_e:
        case dItemNo_Randomizer_POKE_BOMB_e:
            pool.push_back(world->GetItem("Bomb Bag", true));
            break;
        case dItemNo_Randomizer_RAFRELS_MEMO_e:
            pool.push_back(world->GetItem("Aurus Memo", true));
            break;
        case dItemNo_Randomizer_ASHS_SCRIBBLING_e:
            pool.push_back(world->GetItem("Asheis Sketch", true));
            break;
        case dItemNo_Randomizer_ANCIENT_DOCUMENT_e:
        case dItemNo_Randomizer_ANCIENT_DOCUMENT2_e:
        case dItemNo_Randomizer_AIR_LETTER_e:
            pool.push_back(world->GetItem("Progressive Sky Book", true));
            break;
        case dItemNo_Randomizer_EMPTY_BOTTLE_e:
        case dItemNo_Randomizer_RED_BOTTLE_e:
        case dItemNo_Randomizer_GREEN_BOTTLE_e:
        case dItemNo_Randomizer_BLUE_BOTTLE_e:
        case dItemNo_Randomizer_MILK_BOTTLE_e:
        case dItemNo_Randomizer_HALF_MILK_BOTTLE_e:
        case dItemNo_Randomizer_OIL_BOTTLE_e:
        case dItemNo_Randomizer_WATER_BOTTLE_e:
        case dItemNo_Randomizer_OIL_BOTTLE_2_e:
        case dItemNo_Randomizer_RED_BOTTLE_2_e:
        case dItemNo_Randomizer_UGLY_SOUP_e:
        case dItemNo_Randomizer_HOT_SPRING_e:
        case dItemNo_Randomizer_FAIRY_e:
        case dItemNo_Randomizer_HOT_SPRING_2_e:
        case dItemNo_Randomizer_OIL2_e:
        case dItemNo_Randomizer_OIL_e:
        case dItemNo_Randomizer_FAIRY_DROP_e:
        case dItemNo_Randomizer_DROP_BOTTLE_e:
        case dItemNo_Randomizer_BEE_CHILD_e:
        case dItemNo_Randomizer_CHUCHU_RARE_e:
        case dItemNo_Randomizer_CHUCHU_RED_e:
        case dItemNo_Randomizer_CHUCHU_BLUE_e:
        case dItemNo_Randomizer_CHUCHU_GREEN_e:
        case dItemNo_Randomizer_CHUCHU_YELLOW_e:
        case dItemNo_Randomizer_CHUCHU_PURPLE_e:
        case dItemNo_Randomizer_LV1_SOUP_e:
        case dItemNo_Randomizer_LV2_SOUP_e:
        case dItemNo_Randomizer_LV3_SOUP_e:
        case dItemNo_Randomizer_OIL_BOTTLE3_e:
        case dItemNo_Randomizer_CHUCHU_BLACK_e:
            pool.push_back(world->GetItem("Empty Bottle", true));
            break;
        default:
            break;
        }
    }

    // Shadow Crystal
    if (dComIfGs_isEventBit(0xD04)) {
        pool.push_back(world->GetItem("Shadow Crystal", true));
    }

    // Showed Auru's Memo to Fyer
    if (dComIfGs_isEventBit(0x2680)) {
        pool.push_back(world->GetItem("Aurus Memo", true));
    }

    // Showed Ralis Ashei's Sketch
    if (dComIfGs_isEventBit(0x3B80)) {
        pool.push_back(world->GetItem("Asheis Sketch", true));
    }

    // Fused Shadows
    for (int i = 0; i < numFusedShadows(); ++i) {
        pool.push_back(world->GetItem("Progressive Fused Shadow", true));
    }

    // Mirror Shards
    for (int i = 0; i < numMirrorShards(); ++i) {
        pool.push_back(world->GetItem("Progressive Mirror Shard", true));
    }

    // Poe Souls
    for (int i = 0; i < dComIfGs_getPohSpiritNum(); ++i) {
        pool.push_back(world->GetItem("Poe Soul", true));
    }

    // Hearts
    for (int i = 0; i < dComIfGs_getMaxLife(); ++i) {
        pool.push_back(world->GetItem("Piece of Heart", true));
    }

    // Sky Book characters
    for (int i = 0; i < getAncientDocumentNum(); ++i) {
        pool.push_back(world->GetItem("Progressive Sky Book", true));
    }

    // Small Keys
    static std::unordered_map<int, std::string> keyRegionItemNameMap = {
        {0xA,  "Gerudo Desert Bulblin Camp Key"},
        {0x10, "Forest Temple Small Key"},
        {0x11, "Goron Mines Small Key"},
        {0x12, "Lakebed Temple Small Key"},
        {0x13, "Arbiters Grounds Small Key"},
        {0x14, "Snowpeak Ruins Small Key"},
        {0x15, "Temple of Time Small Key"},
        {0x16, "City in the Sky Small Key"},
        {0x17, "Palace of Twilight Small Key"},
        {0x18, "Hyrule Castle Small Key"},
    };
    for (auto& [stage, keyName] : keyRegionItemNameMap) {
        for (int i = 0; i < getTempleKeysFound(stage); ++i) {
            pool.push_back(world->GetItem(keyName, true));
        }
    }

    // Gate Keys
    if (haveItem(dItemNo_Randomizer_BOSSRIDER_KEY_e)) {
        pool.push_back(world->GetItem(dItemNo_Randomizer_BOSSRIDER_KEY_e, true));
    }

    // Big Keys
    static std::unordered_map<int, std::string> bigKeyRegionItemNameMap = {
        {0x10, "Forest Temple Big Key"},
        {0x12, "Lakebed Temple Big Key"},
        {0x13, "Arbiters Grounds Big Key"},
        {0x14, "Snowpeak Ruins Bedroom Key"},
        {0x15, "Temple of Time Big Key"},
        {0x16, "City in the Sky Big Key"},
        {0x17, "Palace of Twilight Big Key"},
        {0x18, "Hyrule Castle Big Key"},
    };
    for (auto& [stage, keyName] : bigKeyRegionItemNameMap) {
        if (isTempleBigKeyFound(stage)) {
            pool.push_back(world->GetItem(keyName, true));
        }
    }

    // Goron Mines Key Shards
    if (haveItem(dItemNo_Randomizer_L2_KEY_PIECES3_e)) {
        for (int i = 0; i < 3; ++i) {
            pool.push_back(world->GetItem("Goron Mines Key Shard", true));
        }
    }

    // Ordon Pumpkin
    if (haveItem(dItemNo_Randomizer_TOMATO_PUREE_e)) {
        pool.push_back(world->GetItem(dItemNo_Randomizer_TOMATO_PUREE_e, true));
    }

    // Ordon Cheese
    if (haveItem(dItemNo_Randomizer_TASTE_e)) {
        pool.push_back(world->GetItem(dItemNo_Randomizer_TASTE_e, true));
    }

    // Golden Bugs
    for (int i = dItemNo_Randomizer_M_BEETLE_e; i < dItemNo_Randomizer_F_MAYFLY_e; ++i) {
        if (haveItem(i)) {
            pool.push_back(world->GetItem(i, true));
        }
    }

    // Ilia quest items
    for (int i = dItemNo_Randomizer_LETTER_e; i < dItemNo_Randomizer_HORSE_FLUTE_e; ++i) {
        if (haveItem(i)) {
            pool.push_back(world->GetItem(i, true));
        }
    }

    // Warp Portals
    // Item ids are scattered so we have to explicitly list them all
    static const int portals[] = {
        dItemNo_Randomizer_ORDON_PORTAL_e,
        dItemNo_Randomizer_SOUTH_FARON_PORTAL_e,
        dItemNo_Randomizer_NORTH_FARON_PORTAL_e,
        dItemNo_Randomizer_SACRED_GROVE_PORTAL_e,
        dItemNo_Randomizer_KAKARIKO_GORGE_PORTAL_e,
        dItemNo_Randomizer_KAKARIKO_VILLAGE_PORTAL_e,
        dItemNo_Randomizer_DEATH_MOUNTAIN_PORTAL_e,
        dItemNo_Randomizer_ELDIN_BRIDGE_PORTAL_e,
        dItemNo_Randomizer_CASTLE_TOWN_PORTAL_e,
        dItemNo_Randomizer_UPPER_ZORAS_RIVER_PORTAL_e,
        dItemNo_Randomizer_ZORAS_DOMAIN_PORTAL_e,
        dItemNo_Randomizer_SNOWPEAK_PORTAL_e,
        dItemNo_Randomizer_GERUDO_DESERT_PORTAL_e,
        dItemNo_Randomizer_MIRROR_CHAMBER_PORTAL_e,
    };
    for (auto portal : portals) {
        if (haveItem(portal)) {
            pool.push_back(world->GetItem(portal, true));
        }
    }

    // Swords
    static const int swords[] = {
        dItemNo_Randomizer_WOOD_STICK_e,
        dItemNo_Randomizer_SWORD_e,
        dItemNo_Randomizer_MASTER_SWORD_e,
        dItemNo_Randomizer_LIGHT_SWORD_e,
    };
    for (auto sword : swords) {
        if (haveItem(sword)) {
            pool.push_back(world->GetItem("Progressive Sword", true));
        }
    }

    // Other Equipment
    static const int equipment[] = {
        dItemNo_Randomizer_WOOD_SHIELD_e,
        dItemNo_Randomizer_HYLIA_SHIELD_e,
        dItemNo_Randomizer_WEAR_ZORA_e,
        dItemNo_Randomizer_ARMOR_e,
    };
    for (auto item : equipment) {
        if (haveItem(item)) {
            pool.push_back(world->GetItem(item, true));
        }
    }

    // Hidden Skills
    static const int hiddenSkills[] = {
        dItemNo_Randomizer_ENDING_BLOW_e,
        dItemNo_Randomizer_SHIELD_ATTACK_e,
        dItemNo_Randomizer_BACK_SLICE_e,
        dItemNo_Randomizer_HELM_SPLITTER_e,
        dItemNo_Randomizer_MORTAL_DRAW_e,
        dItemNo_Randomizer_JUMP_STRIKE_e,
        dItemNo_Randomizer_GREAT_SPIN_e,
    };
    for (auto skill : hiddenSkills) {
        if (haveItem(skill)) {
            pool.push_back(world->GetItem("Progressive Hidden Skill", true));
        }
    }

    // Wallets
    switch (dComIfGs_getWalletSize()) {
    case GIANT_WALLET:
        pool.push_back(world->GetItem("Progressive Wallet", true));
        [[fallthrough]];
    case BIG_WALLET:
        pool.push_back(world->GetItem("Progressive Wallet", true));
        [[fallthrough]];
    default:
        break;
    }

    // Twilight Tears
    for (int i = 0; i < dComIfGs_getLightDropNum(0); ++i) {
        pool.push_back(world->GetItem("Faron Twilight Tear", true));
    }
    for (int i = 0; i < dComIfGs_getLightDropNum(1); ++i) {
        pool.push_back(world->GetItem("Eldin Twilight Tear", true));
    }
    for (int i = 0; i < dComIfGs_getLightDropNum(2); ++i) {
        pool.push_back(world->GetItem("Lanayru Twilight Tear", true));
    }

    return pool;
}

// overriding location names from generator to match item service api format
std::string nameLookupOverride(const std::string& locationName) {
    // [generator name], [service name]
    static std::unordered_map<std::string, std::string> nameLookup = {
        {"Arbiters Grounds Dungeon Reward", ITEM_CHECK_DUNGEON_REWARD_ARBITERS},
        {"Ashei Sketch", ITEM_CHECK_ASHEI_SKETCH},
        {"Auru Gift To Fyer", ITEM_CHECK_AURU_MEMO},
        {"Cave of Ordeals Great Fairy Reward", ITEM_CHECK_FAIRY_REWARD},
        {"Charlo Donation Blessing", ITEM_CHECK_PRAYER_REWARD},
        {"Coro Bottle", ITEM_CHECK_CORO_BOTTLE},
        {"Fishing Hole Bottle", ITEM_CHECK_FISHING_BOTTLE},
        {"Fishing Hole Heart Piece", ITEM_CHECK_FISHING_HEART_PIECE},
        {"Forest Temple Dungeon Reward", ITEM_CHECK_DUNGEON_REWARD_FOREST},
        {"Gift From Ralis", ITEM_CHECK_CORAL_EARRING},
        {"Goron Mines Dungeon Reward", ITEM_CHECK_DUNGEON_REWARD_GORON},
        {"Goron Mines Gor Amato Key Shard", ITEM_CHECK_KEY_SHARD_1},
        {"Goron Mines Gor Ebizo Key Shard", ITEM_CHECK_KEY_SHARD_2},
        {"Goron Mines Gor Liggs Key Shard", ITEM_CHECK_KEY_SHARD_3},
        {"Herding Goats Reward", ITEM_CHECK_GOATS_REWARD},
        {"Hyrule Castle King Bulblin Key", ITEM_CHECK_BULBLIN_KEY},
        {"Ilia Charm", ITEM_CHECK_ILIA_CHARM},
        {"Ilia Memory Reward", ITEM_CHECK_ILIA_MEMORY},
        {"Iza Helping Hand", ITEM_CHECK_IZA_REWARD_1},
        {"Iza Raging Rapids Minigame", ITEM_CHECK_IZA_REWARD_2},
        {"Jovani 20 Poe Soul Reward", ITEM_CHECK_JOVANI_REWARD_1},
        {"Jovani 60 Poe Soul Reward", ITEM_CHECK_JOVANI_REWARD_2},
        {"Kakariko Village Malo Mart Hawkeye", "shop:R_SP109:62"},
        {"Kakariko Village Malo Mart Hylian Shield", "shop:R_SP109:44"},
        {"Kakariko Village Malo Mart Red Potion", "shop:R_SP109:97"},
        {"Kakariko Village Malo Mart Wooden Shield", "shop:R_SP109:43"},
        {"Castle Town Malo Mart Magic Armor", "shop:R_SP160:48"},
        {"Lakebed Temple Dungeon Reward", ITEM_CHECK_DUNGEON_REWARD_LAKEBED},
        {"Ordon Cat Rescue", ITEM_CHECK_SERA_REWARD},
        {"Ordon Shield", ITEM_CHECK_ORDON_SHIELD},
        {"Ordon Sword", ITEM_CHECK_ORDON_SWORD},
        {"Plumm Fruit Balloon Minigame", ITEM_CHECK_PLUMM_REWARD},
        {"Renados Letter", ITEM_CHECK_RENADO_LETTER},
        {"Rutelas Blessing", ITEM_CHECK_ZORA_ARMOR},
        {"Sacred Grove Pedestal Master Sword", ITEM_CHECK_MASTER_SWORD},
        {"Sacred Grove Pedestal Shadow Crystal", ITEM_CHECK_SHADOW_CRYSTAL},
        {"Sera Shop Slingshot", "shop:R_SP01:75"},
        {"Skybook From Impaz", ITEM_CHECK_SKYBOOK},
        {"Snowboard Racing Prize", ITEM_CHECK_SNOWBOARD_REWARD},
        {"Snowpeak Ruins Dungeon Reward", ITEM_CHECK_DUNGEON_REWARD_SNOWPEAK},
        {"Snowpeak Ruins Ball and Chain", ITEM_CHECK_BALL_AND_CHAIN},
        {"Snowpeak Ruins Mansion Map", ITEM_CHECK_DUNGEON_MAP_SNOWPEAK},
        {"STAR Prize 1", ITEM_CHECK_STAR_REWARD_1},
        {"STAR Prize 2", ITEM_CHECK_STAR_REWARD_2},
        {"Talo Sharpshooting", ITEM_CHECK_ARCHERY_REWARD},
        {"Telma Invoice", ITEM_CHECK_TELMA_INVOICE},
        {"Temple of Time Dungeon Reward", ITEM_CHECK_DUNGEON_REWARD_TIME},
        {"Uli Cradle Delivery", ITEM_CHECK_ULI_CRADLE_REWARD},
        {"Wooden Statue", ITEM_CHECK_WOOD_STATUE},
        {"City in the Sky Dungeon Reward", ITEM_CHECK_DUNGEON_REWARD_CITY},
        {"Zoras Domain Underwater Goron", ITEM_CHECK_GORON_REWARD},
    };

    for (auto& [oldName, newName] : nameLookup) {
        if (locationName == oldName) {
            return newName;
        }
    }

    return locationName;
}

bool isLocationObtained(randomizer::logic::location::Location* location) {
    auto& locationMeta = location->GetMetadata();
    if (auto& chestNode = locationMeta["Chest"]) {
        auto tboxId = chestNode[0]["Tbox Id"].as<u8>();
        auto stageId = getStageSaveId(chestNode[0]["Stage"].as<u8>());
        return dComIfGs_isStageTbox(stageId, tboxId);
    }
    if (auto& poeNode = locationMeta["Poe"]) {
        auto flag = poeNode[0]["Flag"].as<u8>();
        auto stageId = getStageSaveId(poeNode[0]["Stage"].as<u8>());
        return tracker_isStageSwitch(stageId, flag);
    }
    if (auto& freeStandingItemNode = locationMeta["Freestanding Item"]) {
        auto flag = freeStandingItemNode[0]["Flag"].as<u8>();
        auto stageId = getStageSaveId(freeStandingItemNode[0]["Stage"].as<u8>());
        // big baba uses tbox, hardcode this edge case
        if (location->GetName() == "Forest Temple Big Baba Key") {
            return dComIfGs_isStageTbox(stageId, flag);
        }else {
            return tracker_isStageItem(stageId, flag);
        }
    }
    if (auto& eventFlagNode = locationMeta["Event Flag"]) {
        auto flag = eventFlagNode.as<u16>();
        return tracker_isEventBit(flag);
    }
    if (auto& wolfNode = locationMeta["Golden Wolf"]) {
        auto flag = wolfNode[0]["Flag"].as<u16>();
        return tracker_isEventBit(flag);
    }
    if (auto& switchFlagNode = locationMeta["Switch Flag"]) {
        auto flag = switchFlagNode["Flag"].as<u8>();
        auto stageId = getStageSaveId(switchFlagNode["Stage"].as<u8>());
        return tracker_isStageSwitch(stageId, flag);
    }
    if (auto& itemFlagNode = locationMeta["Item Flag"]) {
        auto flag = itemFlagNode["Flag"].as<u8>();
        auto stageId = getStageSaveId(itemFlagNode["Stage"].as<u8>());
        return tracker_isStageItem(stageId, flag);
    }
    if (auto& twilitInsectNode = locationMeta["Twilit Insect"]) {
        auto flag = twilitInsectNode[0]["Flag"].as<u8>();
        auto stageId = getStageSaveId(twilitInsectNode[0]["Stage"].as<u8>());
        return dComIfGs_isStageTbox(stageId, flag);
    }
    return false;
}

int getLocationItem(randomizer::logic::location::Location* location) {
    auto& locationMeta = location->GetMetadata();
    auto& context = randomizer_GetContext();

    if (auto& chestNode = locationMeta["Chest"]) {
        auto tboxId = chestNode[0]["Tbox Id"].as<u8>();
        auto stage = chestNode[0]["Stage"].as<u8>();
        auto key = (stage << 8) | tboxId;
        return context.mTreasureChestOverrides[key];
    }
    if (auto& poeNode = locationMeta["Poe"]) {
        auto flag = poeNode[0]["Flag"].as<u8>();
        auto stage = poeNode[0]["Stage"].as<u8>();
        auto key = (stage << 8) | flag;
        return context.mPoeOverrides[key];
    }
    if (auto& freeStandingItemNode = locationMeta["Freestanding Item"]) {
        auto flag = freeStandingItemNode[0]["Flag"].as<u8>();
        auto stage = freeStandingItemNode[0]["Stage"].as<u8>();
        auto key = (stage << 8) | flag;
        return context.mFreestandingItemOverrides[key];
    }
    if (auto& bugRewardNode = locationMeta["Bug Reward"]) {
        u8 bugItemId = bugRewardNode[0]["Item Id"].as<u8>();
        return context.mBugRewardOverrides[bugItemId];
    }
    if (auto& skyCharacterNode = locationMeta["Sky Character"]) {
        u8 stageIdx = skyCharacterNode[0]["Stage"].as<u8>();
        u8 roomNo = skyCharacterNode[0]["Room"].as<u8>();
        u16 key = (stageIdx << 8) | roomNo;
        return context.mSkyCharacterOverrides[key];
    }
    if (auto& wolfNode = locationMeta["Golden Wolf"]) {
        auto flag = wolfNode[0]["Flag"].as<u16>();
        return context.mGoldenWolfOverrides[flag];
    }
    if (auto& shopNode = locationMeta["Shop"]) {
        u8 stage = shopNode[0]["Stage"].as<u8>();
        u8 originalItem = shopNode[0]["Item"].as<u8>();
        u16 key = (stage << 8) | originalItem;
        return context.mShopOverrides[key];
    }
    if (auto& twilitInsectNode = locationMeta["Twilit Insect"]) {
        auto flag = twilitInsectNode[0]["Flag"].as<u8>();
        auto stage = twilitInsectNode[0]["Stage"].as<u8>();
        auto key = (stage << 8) | flag;
        return context.mTwilitInsectOverrides[key];
    }
    if (auto& nameNode = locationMeta["Name Lookup"]) {
        auto name = nameLookupOverride(nameNode[0].as<std::string>());
        return context.mItemLocations[name].itemId;
    }
    return -1;
}

int getStageSaveId(int id) {
    switch (id) {
        case 41: // F_SP00  (Ordon Ranch)
        case 43: // F_SP103 (Ordon Village / Outside Link's House)
        case 44: // F_SP104 (Ordon Spring)
        case 65: // R_SP01  (Ordon House Interiors)
            return 0x0;
        case 66: // R_SP107 (Castle Town Sewers)
            return 0x1;
        case 40: // D_SB10  (Faron Woods Cave)
        case 45: // F_SP108 (Faron Woods)
        case 67: // R_SP108 (Coro's House)
            return 0x2;
        case 46: // F_SP109 (Kakariko Village)
        case 47: // F_SP110 (Death Mountain Trail)
        case 48: // F_SP111 (Kakariko Graveyard)
        case 63: // F_SP128 (Hidden Village)
        case 68: // R_SP109 (Kakariko Interiors)
        case 69: // R_SP110 (Goron Elder's Hall)
        case 72: // R_SP128 (Impaz's House)
        case 75: // R_SP209 (Sanctuary Basement)
            return 0x3;
        case 49: // F_SP112 (Zora's River)
        case 50: // F_SP113 (Zora's Domain)
        case 52: // F_SP115 (Lake Hylia)
        case 61: // F_SP126 (Upper Zora's River)
        case 71: // R_SP127 (Hena's Cabin)
            return 0x4;
        case 56: // F_SP121 (Hyrule Field)
        case 57: // F_SP122 (Outside Castle Town)
        case 58: // F_SP123 (King Bulblin 2)
        case 64: // F_SP200 (Wolf Howling Cutscene Map)
            return 0x6;
        case 54: // F_SP117 (Lost Woods)
            return 0x7;
        case 51: // F_SP114 (Snowpeak Mountain)
            return 0x8;
        case 53: // F_SP116 (Castle Town)
        case 70: // R_SP116 (Telma's Bar / Secret Passage)
        case 73: // R_SP160 (Hyrule Castle Town Interiors)
        case 74: // R_SP161 (STAR Tent)
            return 0x9;
        case 55: // F_SP118 (Bulblin Camp)
        case 59: // F_SP124 (Gerudo Desert)
        case 60: // F_SP125 (Mirror Chamber)
            return 0xA;
        case 62: // F_SP127 (Fishing Pond)
            return 0xB;
        case 6:  // D_MN05  (Forest Temple)
        case 7:  // D_MN05A (Diababa Arena)
        case 8:  // D_MN05B (Ook Arena)
            return 0x10;
        case 3:  // D_MN04  (Goron Mines)
        case 4:  // D_MN04A (Fyrus Arena)
        case 5:  // D_MN04B (Dangoro Arena)
            return 0x11;
        case 0:  // D_MN01  (Lakebed Temple)
        case 1:  // D_MN01A (Morpheel Arena)
        case 2:  // D_MN01B (Deku Toad Arena)
            return 0x12;
        case 24: // D_MN10  (Arbiter's Grounds)
        case 25: // D_MN10A (Stallord Arena)
        case 26: // D_MN10B (Death Sword Arena)
            return 0x13;
        case 27: // D_MN11  (Snowpeak Ruins)
        case 28: // D_MN11A (Blizzeta Arena)
        case 29: // D_MN11B (Darkhammer Arena)
            return 0x14;
        case 9:  // D_MN06  (Temple of Time)
        case 10: // D_MN06A (Armogohma Arena)
        case 11: // D_MN06B (Darknut Arena)
            return 0x15;
        case 12: // D_MN07  (City in the Sky)
        case 13: // D_MN07A (Argorok Arena)
        case 14: // D_MN07B (Aeralfos Arena)
            return 0x16;
        case 15: // D_MN08  (Palace of Twilight)
        case 16: // D_MN08A (Palace of Twilight Throne Room)
        case 17: // D_MN08B (Phantom Zant Arena 1)
        case 18: // D_MN08C (Phantom Zant Arena 2)
        case 19: // D_MN08D (Zant Arenas)
            return 0x17;
        case 20: // D_MN09  (Hyrule Castle)
        case 21: // D_MN09A (Hyrule Castle Throne Room)
        case 22: // D_MN09B (Horseback Ganondorf Arena)
        case 23: // D_MN09C (Dark Lord Ganondorf Arena)
            return 0x18;
        case 30: // D_SB00  (Ice Cavern)
        case 31: // D_SB01  (Cave Of Ordeals)
        case 32: // D_SB02  (Kakariko Gorge Cavern)
            return 0x19;
        case 33: // D_SB03  (Lake Hylia Cavern)
        case 34: // D_SB04  (Goron Stockcave)
            return 0x1A;
        case 35: // D_SB05  (Grotto 1)
        case 36: // D_SB06  (Grotto 2)
        case 37: // D_SB07  (Grotto 3)
        case 38: // D_SB08  (Grotto 4)
        case 39: // D_SB09  (Grotto 5)
            return 0x1B;
        case 42: // F_SP102 (Title Screen / King Bulblin 1)
            return 0xFF;
    default:
            mods::log::warn("Failed to find Save Id for ID: {}" , id);
            return -1;
    }
}

int getStageSaveId(const char* stage) {
    int id = getStageID(stage);
    return getStageSaveId(id);
}

bool tracker_isEventBit(u16 flag) {
    return g_randomizerState.mTrackerTempEventFlag == flag || dComIfGs_isEventBit(flag);
}

bool tracker_isStageSwitch(int stage, int flag) {
    return dComIfGs_isStageSwitch(stage, flag) ||
        (g_randomizerState.mTrackerTempSwitchFlag.flag == flag &&
         g_randomizerState.mTrackerTempSwitchFlag.stage == stage);
}

bool tracker_isStageItem(int stage, int flag) {
    if (g_randomizerState.mTrackerTempItemFlag.flag == flag &&
         g_randomizerState.mTrackerTempItemFlag.stage == stage) {
        return true;
    }

    if (dComIfGp_getStageStagInfo() && stage == dStage_stagInfo_GetSaveTbl(dComIfGp_getStageStagInfo())) {
        return dComIfGs_isItem(flag, -1);
    } else {
        // Need to subtract 0x80 (MEMORY_ITEM constant in d_save.cpp) because the above function does it
        return g_dComIfG_gameInfo.info.getSavedata().getSave(stage).getBit().isItem(flag - 0x80);
    }
}

// Kinda hacky, but will do for now
void onRegionFlag(int i_stageNo, int i_no) {
    auto regionFlags = reinterpret_cast<u8*>(&dComIfGs_getSaveData()->getSave(i_stageNo).getBit());
    const int offset = i_no / 8;
    const int shift = i_no % 8;
    regionFlags[offset] |= (0x80 >> shift);
}

void setRegionBit(u8 i_region) {
    dComIfGs_getSaveData()->getPlayer().getPlayerFieldLastStayInfo().mRegion |= i_region;
}

void setAllLetterGet() {
    dComIfGs_getSaveData()->getPlayer().getLetterInfo().mLetterGetFlags[0] |= 0xFFFF;
}

void setAllLetterRead() {
    dComIfGs_getSaveData()->getPlayer().getLetterInfo().mLetterReadFlags[0] |= 0xFFFF;
}

constexpr const char* skyCharacterBlobName = "sky_characters";
u8 g_skyCharacters = 0;

void saveAncientDocumentNum() {
    auto* ctx = randomizer::session::svc_mng.mod_ctx;
    randomizer::session::svc_mng.save->set_blob(ctx, skyCharacterBlobName, &g_skyCharacters, sizeof(g_skyCharacters));
}

void loadAncientDocumentNum() {
    g_skyCharacters = 0;
    size_t size = sizeof(g_skyCharacters);
    auto* ctx = randomizer::session::svc_mng.mod_ctx;
    randomizer::session::svc_mng.save->get_blob(ctx, skyCharacterBlobName, &g_skyCharacters, &size);
}

u8 getAncientDocumentNum() {
    return g_skyCharacters;
}

void setAncientDocumentNum(u8 num) {
    g_skyCharacters = num;
}

u8 getAreaKeyNum(int i_stageNo) {
    stage_stag_info_class* stagInfo = dComIfGp_getStageStagInfo();
    if (stagInfo != nullptr) {
        if (i_stageNo == dStage_stagInfo_GetSaveTbl(stagInfo)) {
            return dComIfGs_getKeyNum();
        }
    }

    return dComIfGs_getSaveData()->getSave(i_stageNo).getBit().getKeyNum();
}
