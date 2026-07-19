/**
 * d_item.cpp
 * Item Get Handling
 */

#include "d/dolzel.h" // IWYU pragma: keep

#include "d/d_item.h"
#include "d/d_com_inf_game.h"
#include "d/d_meter2_info.h"
#include "dusk/logging.h"
#if TARGET_PC
#include "dusk/randomizer/game/flags.h"
#include "dusk/randomizer/game/tools.h"
#include "dusk/randomizer/game/stages.h"
#include "dusk/randomizer/game/verify_item_functions.h"
#endif
#include <cstring>

#include "dusk/archipelago/archipelago_context.hpp"

static void (*item_func_ptr[256])() = {
    item_func_HEART,
    item_func_GREEN_RUPEE,
    item_func_BLUE_RUPEE,
    item_func_YELLOW_RUPEE,
    item_func_RED_RUPEE,
    item_func_PURPLE_RUPEE,
    item_func_ORANGE_RUPEE,
    item_func_SILVER_RUPEE,
    item_func_S_MAGIC,
    item_func_L_MAGIC,
    item_func_BOMB_5,
    item_func_BOMB_10,
    item_func_BOMB_20,
    item_func_BOMB_30,
    item_func_ARROW_10,
    item_func_ARROW_20,
    item_func_ARROW_30,
    item_func_ARROW_1,
    item_func_PACHINKO_SHOT,
    item_func_noentry,
    item_func_noentry,
    item_func_noentry,
    item_func_WATER_BOMB_5,
    item_func_WATER_BOMB_10,
    item_func_WATER_BOMB_20,
    item_func_WATER_BOMB_30,
    item_func_BOMB_INSECT_5,
    item_func_BOMB_INSECT_10,
    item_func_BOMB_INSECT_20,
    item_func_BOMB_INSECT_30,
    item_func_RECOVER_FAILY,
    item_func_TRIPLE_HEART,
    item_func_SMALL_KEY,
    item_func_KAKERA_HEART,
    item_func_UTUWA_HEART,
    item_func_MAP,
    item_func_COMPUS,
    item_func_DUNGEON_EXIT,
    item_func_BOSS_KEY,
    item_func_DUNGEON_BACK,
    item_func_SWORD,
    item_func_MASTER_SWORD,
    item_func_WOOD_SHIELD,
    item_func_SHIELD,
    item_func_HYLIA_SHIELD,
    item_func_TKS_LETTER,
    item_func_WEAR_CASUAL,
    item_func_WEAR_KOKIRI,
    item_func_ARMOR,
    item_func_WEAR_ZORA,
    item_func_MAGIC_LV1,
    item_func_DUNGEON_EXIT_2,
    item_func_WALLET_LV1,
    item_func_WALLET_LV2,
    item_func_WALLET_LV3,
    item_func_noentry,
    item_func_noentry,
    item_func_noentry,
    item_func_noentry,
    item_func_noentry,
    item_func_noentry,
    item_func_ZORAS_JEWEL,
    item_func_HAWK_EYE,
    item_func_WOOD_STICK,
    item_func_BOOMERANG,
    item_func_SPINNER,
    item_func_IRONBALL,
    item_func_BOW,
    item_func_HOOKSHOT,
    item_func_HVY_BOOTS,
    item_func_COPY_ROD,
    item_func_W_HOOKSHOT,
    item_func_KANTERA,
    item_func_LIGHT_SWORD,
    item_func_FISHING_ROD_1,
    item_func_PACHINKO,
    item_func_COPY_ROD_2,
    item_func_noentry,
    item_func_noentry,
    item_func_BOMB_BAG_LV2,
    item_func_BOMB_BAG_LV1,
    item_func_BOMB_IN_BAG,
    item_func_noentry,
    item_func_LIGHT_ARROW,
    item_func_ARROW_LV1,
    item_func_ARROW_LV2,
    item_func_ARROW_LV3,
    item_func_noentry,
    item_func_LURE_ROD,
    item_func_BOMB_ARROW,
    item_func_HAWK_ARROW,
    item_func_BEE_ROD,
    item_func_JEWEL_ROD,
    item_func_WORM_ROD,
    item_func_JEWEL_BEE_ROD,
    item_func_JEWEL_WORM_ROD,
    item_func_EMPTY_BOTTLE,
    item_func_RED_BOTTLE,
    item_func_GREEN_BOTTLE,
    item_func_BLUE_BOTTLE,
    item_func_MILK_BOTTLE,
    item_func_HALF_MILK_BOTTLE,
    item_func_OIL_BOTTLE,
    item_func_WATER_BOTTLE,
    item_func_OIL_BOTTLE2,
    item_func_RED_BOTTLE2,
    item_func_UGLY_SOUP,
    item_func_HOT_SPRING,
    item_func_FAIRY_BOTTLE,
    item_func_HOT_SPRING2,
    item_func_OIL2,
    item_func_OIL,
    item_func_NORMAL_BOMB,
    item_func_WATER_BOMB,
    item_func_POKE_BOMB,
    item_func_FAIRY_DROP,
    item_func_WORM,
    item_func_DROP_BOTTLE,
    item_func_BEE_CHILD,
    item_func_CHUCHU_RARE,
    item_func_CHUCHU_RED,
    item_func_CHUCHU_BLUE,
    item_func_CHUCHU_GREEN,
    item_func_CHUCHU_YELLOW,
    item_func_CHUCHU_PURPLE,
    item_func_LV1_SOUP,
    item_func_LV2_SOUP,
    item_func_LV3_SOUP,
    item_func_LETTER,
    item_func_BILL,
    item_func_WOOD_STATUE,
    item_func_IRIAS_PENDANT,
    item_func_HORSE_FLUTE,
    item_func_noentry,
    item_func_noentry,
    item_func_noentry,
    item_func_noentry,
    item_func_noentry,
    item_func_noentry,
    item_func_noentry,
    item_func_noentry,
    item_func_noentry,
    item_func_noentry,
    item_func_noentry,
    item_func_RAFRELS_MEMO,
    item_func_ASHS_SCRIBBLING,
    item_func_noentry,
    item_func_noentry,
    item_func_noentry,
    item_func_noentry,
    item_func_noentry,
    item_func_noentry,
    item_func_noentry,
    item_func_noentry,
    item_func_noentry,
    item_func_noentry,
    item_func_CHUCHU_YELLOW2,
    item_func_OIL_BOTTLE3,
    item_func_SHOP_BEE_CHILD,
    item_func_CHUCHU_BLACK,
    item_func_LIGHT_DROP,
    item_func_DROP_CONTAINER,
    item_func_DROP_CONTAINER02,
    item_func_DROP_CONTAINER03,
    item_func_FILLED_CONTAINER,
    item_func_MIRROR_PIECE_2,
    item_func_MIRROR_PIECE_3,
    item_func_MIRROR_PIECE_4,
    item_func_noentry,
    item_func_noentry,
    item_func_noentry,
    item_func_noentry,
    item_func_noentry,
    item_func_noentry,
    item_func_noentry,
    item_func_noentry,
    item_func_SMELL_YELIA_POUCH,
    item_func_SMELL_PUMPKIN,
    item_func_SMELL_POH,
    item_func_SMELL_FISH,
    item_func_SMELL_CHILDREN,
    item_func_SMELL_MEDICINE,
    item_func_noentry,
    item_func_noentry,
    item_func_noentry,
    item_func_noentry,
    item_func_noentry,
    item_func_noentry,
    item_func_noentry,
    item_func_noentry,
    item_func_noentry,
    item_func_noentry,
    item_func_M_BEETLE,
    item_func_F_BEETLE,
    item_func_M_BUTTERFLY,
    item_func_F_BUTTERFLY,
    item_func_M_STAG_BEETLE,
    item_func_F_STAG_BEETLE,
    item_func_M_GRASSHOPPER,
    item_func_F_GRASSHOPPER,
    item_func_M_NANAFUSHI,
    item_func_F_NANAFUSHI,
    item_func_M_DANGOMUSHI,
    item_func_F_DANGOMUSHI,
    item_func_M_MANTIS,
    item_func_F_MANTIS,
    item_func_M_LADYBUG,
    item_func_F_LADYBUG,
    item_func_M_SNAIL,
    item_func_F_SNAIL,
    item_func_M_DRAGONFLY,
    item_func_F_DRAGONFLY,
    item_func_M_ANT,
    item_func_F_ANT,
    item_func_M_MAYFLY,
    item_func_F_MAYFLY,
    item_func_noentry,
    item_func_noentry,
    item_func_noentry,
    item_func_noentry,
    item_func_noentry,
    item_func_noentry,
    item_func_noentry,
    item_func_noentry,
    item_func_POU_SPIRIT,
    item_func_noentry,
    item_func_noentry,
    item_func_noentry,
    item_func_noentry,
    item_func_noentry,
    item_func_noentry,
    item_func_noentry,
    item_func_noentry,
    item_func_ANCIENT_DOCUMENT,
    item_func_AIR_LETTER,
    item_func_ANCIENT_DOCUMENT2,
    item_func_LV7_DUNGEON_EXIT,
    item_func_LINKS_SAVINGS,
    item_func_SMALL_KEY2,
    item_func_POU_FIRE1,
    item_func_POU_FIRE2,
    item_func_POU_FIRE3,
    item_func_POU_FIRE4,
    item_func_BOSSRIDER_KEY,
    item_func_TOMATO_PUREE,
    item_func_TASTE,
    item_func_LV5_BOSS_KEY,
    item_func_SURFBOARD,
    item_func_KANTERA2,
    item_func_L2_KEY_PIECES1,
    item_func_L2_KEY_PIECES2,
    item_func_L2_KEY_PIECES3,
    item_func_KEY_OF_CARAVAN,
    item_func_LV2_BOSS_KEY,
    item_func_KEY_OF_FILONE,
    item_func_noentry,
};

#if TARGET_PC
static void (*item_func_ptr_randomizer[256])() = {
    /* 0x00 */ item_func_HEART,
    /* 0x01 */ item_func_GREEN_RUPEE,
    /* 0x02 */ item_func_BLUE_RUPEE,
    /* 0x03 */ item_func_YELLOW_RUPEE,
    /* 0x04 */ item_func_RED_RUPEE,
    /* 0x05 */ item_func_PURPLE_RUPEE,
    /* 0x06 */ item_func_ORANGE_RUPEE,
    /* 0x07 */ item_func_SILVER_RUPEE,
    /* 0x08 */ item_func_S_MAGIC,
    /* 0x09 */ item_func_L_MAGIC,
    /* 0x0A */ item_func_BOMB_5,
    /* 0x0B */ item_func_BOMB_10,
    /* 0x0C */ item_func_BOMB_20,
    /* 0x0D */ item_func_BOMB_30,
    /* 0x0E */ item_func_ARROW_10,
    /* 0x0F */ item_func_ARROW_20,
    /* 0x10 */ item_func_ARROW_30,
    /* 0x11 */ item_func_ARROW_1,
    /* 0x12 */ item_func_PACHINKO_SHOT,
    /* 0x13 */ item_func_FOOLISH_ITEM,
    /* 0x14 */ item_func_ORDON_PORTAL,
    /* 0x15 */ item_func_SOUTH_FARON_PORTAL,
    /* 0x16 */ item_func_WATER_BOMB_5,
    /* 0x17 */ item_func_WATER_BOMB_10,
    /* 0x18 */ item_func_WATER_BOMB_20,
    /* 0x19 */ item_func_WATER_BOMB_30,
    /* 0x1A */ item_func_BOMB_INSECT_5,
    /* 0x1B */ item_func_BOMB_INSECT_10,
    /* 0x1C */ item_func_BOMB_INSECT_20,
    /* 0x1D */ item_func_BOMB_INSECT_30,
    /* 0x1E */ item_func_RECOVER_FAILY,
    /* 0x1F */ item_func_TRIPLE_HEART,
    /* 0x20 */ item_func_SMALL_KEY,
    /* 0x21 */ item_func_KAKERA_HEART,
    /* 0x22 */ item_func_UTUWA_HEART,
    /* 0x23 */ item_func_MAP,
    /* 0x24 */ item_func_COMPUS,
    /* 0x25 */ item_func_DUNGEON_EXIT,
    /* 0x26 */ item_func_BOSS_KEY,
    /* 0x27 */ item_func_DUNGEON_BACK,
    /* 0x28 */ item_func_SWORD,
    /* 0x29 */ item_func_MASTER_SWORD,
    /* 0x2A */ item_func_WOOD_SHIELD,
    /* 0x2B */ item_func_SHIELD,
    /* 0x2C */ item_func_HYLIA_SHIELD,
    /* 0x2D */ item_func_TKS_LETTER,
    /* 0x2E */ item_func_WEAR_CASUAL,
    /* 0x2F */ item_func_WEAR_KOKIRI,
    /* 0x30 */ item_func_ARMOR,
    /* 0x31 */ item_func_WEAR_ZORA,
    /* 0x32 */ item_func_MAGIC_LV1,
    /* 0x33 */ item_func_DUNGEON_EXIT_2,
    /* 0x34 */ item_func_WALLET_LV1,
    /* 0x35 */ item_func_WALLET_LV2,
    /* 0x36 */ item_func_WALLET_LV3,
    /* 0x37 */ item_func_noentry,
    /* 0x38 */ item_func_noentry,
    /* 0x39 */ item_func_UPPER_ZORAS_RIVER_PORTAL,
    /* 0x3A */ item_func_CASTLE_TOWN_PORTAL,
    /* 0x3B */ item_func_GERUDO_DESERT_PORTAL,
    /* 0x3C */ item_func_NORTH_FARON_PORTAL,
    /* 0x3D */ item_func_ZORAS_JEWEL,
    /* 0x3E */ item_func_HAWK_EYE,
    /* 0x3F */ item_func_WOOD_STICK,
    /* 0x40 */ item_func_BOOMERANG,
    /* 0x41 */ item_func_SPINNER,
    /* 0x42 */ item_func_IRONBALL,
    /* 0x43 */ item_func_BOW,
    /* 0x44 */ item_func_HOOKSHOT,
    /* 0x45 */ item_func_HVY_BOOTS,
    /* 0x46 */ item_func_COPY_ROD,
    /* 0x47 */ item_func_W_HOOKSHOT,
    /* 0x48 */ item_func_KANTERA,
    /* 0x49 */ item_func_LIGHT_SWORD,
    /* 0x4A */ item_func_FISHING_ROD_1,
    /* 0x4B */ item_func_PACHINKO,
    /* 0x4C */ item_func_COPY_ROD_2,
    /* 0x4D */ item_func_KAKARIKO_GORGE_PORTAL,
    /* 0x4E */ item_func_KAKARIKO_VILLAGE_PORTAL,
    /* 0x4F */ item_func_BOMB_BAG_LV2,
    /* 0x50 */ item_func_BOMB_BAG_LV1,
    /* 0x51 */ item_func_BOMB_IN_BAG,
    /* 0x52 */ item_func_DEATH_MOUNTAIN_PORTAL,
    /* 0x53 */ item_func_LIGHT_ARROW,
    /* 0x54 */ item_func_ARROW_LV1,
    /* 0x55 */ item_func_ARROW_LV2,
    /* 0x56 */ item_func_ARROW_LV3,
    /* 0x57 */ item_func_ZORAS_DOMAIN_PORTAL,
    /* 0x58 */ item_func_LURE_ROD,
    /* 0x59 */ item_func_BOMB_ARROW,
    /* 0x5A */ item_func_HAWK_ARROW,
    /* 0x5B */ item_func_BEE_ROD,
    /* 0x5C */ item_func_JEWEL_ROD,
    /* 0x5D */ item_func_WORM_ROD,
    /* 0x5E */ item_func_JEWEL_BEE_ROD,
    /* 0x5F */ item_func_JEWEL_WORM_ROD,
    /* 0x60 */ item_func_EMPTY_BOTTLE,
    /* 0x61 */ item_func_RED_BOTTLE,
    /* 0x62 */ item_func_GREEN_BOTTLE,
    /* 0x63 */ item_func_BLUE_BOTTLE,
    /* 0x64 */ item_func_MILK_BOTTLE,
    /* 0x65 */ item_func_HALF_MILK_BOTTLE,
    /* 0x66 */ item_func_OIL_BOTTLE,
    /* 0x67 */ item_func_WATER_BOTTLE,
    /* 0x68 */ item_func_OIL_BOTTLE2,
    /* 0x69 */ item_func_RED_BOTTLE2,
    /* 0x6A */ item_func_UGLY_SOUP,
    /* 0x6B */ item_func_HOT_SPRING,
    /* 0x6C */ item_func_FAIRY_BOTTLE,
    /* 0x6D */ item_func_HOT_SPRING2,
    /* 0x6E */ item_func_OIL2,
    /* 0x6F */ item_func_OIL,
    /* 0x70 */ item_func_NORMAL_BOMB,
    /* 0x71 */ item_func_WATER_BOMB,
    /* 0x72 */ item_func_POKE_BOMB,
    /* 0x73 */ item_func_FAIRY_DROP,
    /* 0x74 */ item_func_WORM,
    /* 0x75 */ item_func_DROP_BOTTLE,
    /* 0x76 */ item_func_BEE_CHILD,
    /* 0x77 */ item_func_CHUCHU_RARE,
    /* 0x78 */ item_func_CHUCHU_RED,
    /* 0x79 */ item_func_CHUCHU_BLUE,
    /* 0x7A */ item_func_CHUCHU_GREEN,
    /* 0x7B */ item_func_CHUCHU_YELLOW,
    /* 0x7C */ item_func_CHUCHU_PURPLE,
    /* 0x7D */ item_func_LV1_SOUP,
    /* 0x7E */ item_func_LV2_SOUP,
    /* 0x7F */ item_func_LV3_SOUP,
    /* 0x80 */ item_func_LETTER,
    /* 0x81 */ item_func_BILL,
    /* 0x82 */ item_func_WOOD_STATUE,
    /* 0x83 */ item_func_IRIAS_PENDANT,
    /* 0x84 */ item_func_HORSE_FLUTE,
    /* 0x85 */ item_func_FOREST_SMALL_KEY,
    /* 0x86 */ item_func_MINES_SMALL_KEY,
    /* 0x87 */ item_func_LAKEBED_SMALL_KEY,
    /* 0x88 */ item_func_ARBITERS_SMALL_KEY,
    /* 0x89 */ item_func_SNOWPEAK_SMALL_KEY,
    /* 0x8A */ item_func_TEMPLE_OF_TIME_SMALL_KEY,
    /* 0x8B */ item_func_CITY_SMALL_KEY,
    /* 0x8C */ item_func_PALACE_SMALL_KEY,
    /* 0x8D */ item_func_HYRULE_SMALL_KEY,
    /* 0x8E */ item_func_CAMP_SMALL_KEY,
    /* 0x8F */ item_func_LAKE_HYLIA_PORTAL,
    /* 0x90 */ item_func_RAFRELS_MEMO,
    /* 0x91 */ item_func_ASHS_SCRIBBLING,
    /* 0x92 */ item_func_FOREST_BOSS_KEY,
    /* 0x93 */ item_func_LAKEBED_BOSS_KEY,
    /* 0x94 */ item_func_ARBITERS_BOSS_KEY,
    /* 0x95 */ item_func_TEMPLE_OF_TIME_BOSS_KEY,
    /* 0x96 */ item_func_CITY_BOSS_KEY,
    /* 0x97 */ item_func_PALACE_BOSS_KEY,
    /* 0x98 */ item_func_HYRULE_BOSS_KEY,
    /* 0x99 */ item_func_FOREST_COMPASS,
    /* 0x9A */ item_func_MINES_COMPASS,
    /* 0x9B */ item_func_LAKEBED_COMPASS,
    /* 0x9C */ item_func_CHUCHU_YELLOW2,
    /* 0x9D */ item_func_OIL_BOTTLE3,
    /* 0x9E */ item_func_SHOP_BEE_CHILD,
    /* 0x9F */ item_func_CHUCHU_BLACK,
    /* 0xA0 */ item_func_LIGHT_DROP,
    /* 0xA1 */ item_func_DROP_CONTAINER,
    /* 0xA2 */ item_func_DROP_CONTAINER02,
    /* 0xA3 */ item_func_DROP_CONTAINER03,
    /* 0xA4 */ item_func_FILLED_CONTAINER,
    /* 0xA5 */ item_func_MIRROR_PIECE_2,
    /* 0xA6 */ item_func_MIRROR_PIECE_3,
    /* 0xA7 */ item_func_MIRROR_PIECE_4,
    /* 0xA8 */ item_func_ARBITERS_COMPASS,
    /* 0xA9 */ item_func_SNOWPEAK_COMPASS,
    /* 0xAA */ item_func_TEMPLE_OF_TIME_COMPASS,
    /* 0xAB */ item_func_CITY_COMPASS,
    /* 0xAC */ item_func_PALACE_COMPASS,
    /* 0xAD */ item_func_HYRULE_COMPASS,
    /* 0xAE */ item_func_MIRROR_CHAMBER_PORTAL,
    /* 0xAF */ item_func_SNOWPEAK_PORTAL,
    /* 0xB0 */ item_func_SMELL_YELIA_POUCH,
    /* 0xB1 */ item_func_SMELL_PUMPKIN,
    /* 0xB2 */ item_func_SMELL_POH,
    /* 0xB3 */ item_func_SMELL_FISH,
    /* 0xB4 */ item_func_SMELL_CHILDREN,
    /* 0xB5 */ item_func_SMELL_MEDICINE,
    /* 0xB6 */ item_func_FOREST_MAP,
    /* 0xB7 */ item_func_MINES_MAP,
    /* 0xB8 */ item_func_LAKEBED_MAP,
    /* 0xB9 */ item_func_ARBITERS_MAP,
    /* 0xBA */ item_func_SNOWPEAK_MAP,
    /* 0xBB */ item_func_TEMPLE_OF_TIME_MAP,
    /* 0xBC */ item_func_CITY_MAP,
    /* 0xBD */ item_func_PALACE_MAP,
    /* 0xBE */ item_func_HYRULE_MAP,
    /* 0xBF */ item_func_SACRED_GROVE_PORTAL,
    /* 0xC0 */ item_func_M_BEETLE,
    /* 0xC1 */ item_func_F_BEETLE,
    /* 0xC2 */ item_func_M_BUTTERFLY,
    /* 0xC3 */ item_func_F_BUTTERFLY,
    /* 0xC4 */ item_func_M_STAG_BEETLE,
    /* 0xC5 */ item_func_F_STAG_BEETLE,
    /* 0xC6 */ item_func_M_GRASSHOPPER,
    /* 0xC7 */ item_func_F_GRASSHOPPER,
    /* 0xC8 */ item_func_M_NANAFUSHI,
    /* 0xC9 */ item_func_F_NANAFUSHI,
    /* 0xCA */ item_func_M_DANGOMUSHI,
    /* 0xCB */ item_func_F_DANGOMUSHI,
    /* 0xCC */ item_func_M_MANTIS,
    /* 0xCD */ item_func_F_MANTIS,
    /* 0xCE */ item_func_M_LADYBUG,
    /* 0xCF */ item_func_F_LADYBUG,
    /* 0xD0 */ item_func_M_SNAIL,
    /* 0xD1 */ item_func_F_SNAIL,
    /* 0xD2 */ item_func_M_DRAGONFLY,
    /* 0xD3 */ item_func_F_DRAGONFLY,
    /* 0xD4 */ item_func_M_ANT,
    /* 0xD5 */ item_func_F_ANT,
    /* 0xD6 */ item_func_M_MAYFLY,
    /* 0xD7 */ item_func_F_MAYFLY,
    /* 0xD8 */ item_func_FUSED_SHADOW_1,
    /* 0xD9 */ item_func_FUSED_SHADOW_2,
    /* 0xDA */ item_func_FUSED_SHADOW_3,
    /* 0xDB */ item_func_MIRROR_PIECE_1,
    /* 0xDC */ item_func_ARCHIPELAGO_ITEM,
    /* 0xDD */ item_func_noentry,
    /* 0xDE */ item_func_noentry,
    /* 0xDF */ item_func_noentry,
    /* 0xE0 */ item_func_POU_SPIRIT,
    /* 0xE1 */ item_func_ENDING_BLOW,
    /* 0xE2 */ item_func_SHIELD_ATTACK,
    /* 0xE3 */ item_func_BACK_SLICE,
    /* 0xE4 */ item_func_HELM_SPLITTER,
    /* 0xE5 */ item_func_MORTAL_DRAW,
    /* 0xE6 */ item_func_JUMP_STRIKE,
    /* 0xE7 */ item_func_GREAT_SPIN,
    /* 0xE8 */ item_func_ELDIN_BRIDGE_PORTAL,
    /* 0xE9 */ item_func_ANCIENT_DOCUMENT,
    /* 0xEA */ item_func_AIR_LETTER,
    /* 0xEB */ item_func_ANCIENT_DOCUMENT2,
    /* 0xEC */ item_func_LV7_DUNGEON_EXIT,
    /* 0xED */ item_func_LINKS_SAVINGS,
    /* 0xEE */ item_func_SMALL_KEY2,
    /* 0xEF */ item_func_POU_FIRE1,
    /* 0xF0 */ item_func_POU_FIRE2,
    /* 0xF1 */ item_func_POU_FIRE3,
    /* 0xF2 */ item_func_POU_FIRE4,
    /* 0xF3 */ item_func_BOSSRIDER_KEY,
    /* 0xF4 */ item_func_TOMATO_PUREE,
    /* 0xF5 */ item_func_TASTE,
    /* 0xF6 */ item_func_LV5_BOSS_KEY,
    /* 0xF7 */ item_func_SURFBOARD,
    /* 0xF8 */ item_func_KANTERA2,
    /* 0xF9 */ item_func_L2_KEY_PIECES1,
    /* 0xFA */ item_func_L2_KEY_PIECES2,
    /* 0xFB */ item_func_L2_KEY_PIECES3,
    /* 0xFC */ item_func_KEY_OF_CARAVAN,
    /* 0xFD */ item_func_LV2_BOSS_KEY,
    /* 0xFE */ item_func_KEY_OF_FILONE,
    /* 0xFF */ item_func_noentry,
};
#endif


inline void getItemFunc(u8 i_itemNo) {
    dComIfGs_onItemFirstBit(i_itemNo);
    dComIfGs_syncInsectMiscFlags();
#if TARGET_PC
    (randomizer_IsActive() ? item_func_ptr_randomizer : item_func_ptr)[i_itemNo]();
#else
    item_func_ptr[i_itemNo]();
#endif
}

void execItemGet(u8 i_itemNo) {
#if TARGET_PC
    if (randomizer_IsActive()) {
        i_itemNo = verifyProgressiveItem(i_itemNo);
        g_randomizerState.mUpdateTracker = true;
        dusk::archi::ArchipelagoContext::SetNeedUpdateLocations(true);
    }

#endif
    getItemFunc(i_itemNo);
}

static int (*item_getcheck_func_ptr[256])() = {
    /* 0x00 */ item_getcheck_func_HEART,
    /* 0x01 */ item_getcheck_func_GREEN_RUPEE,
    /* 0x02 */ item_getcheck_func_BLUE_RUPEE,
    /* 0x03 */ item_getcheck_func_YELLOW_RUPEE,
    /* 0x04 */ item_getcheck_func_RED_RUPEE,
    /* 0x05 */ item_getcheck_func_PURPLE_RUPEE,
    /* 0x06 */ item_getcheck_func_ORANGE_RUPEE,
    /* 0x07 */ item_getcheck_func_SILVER_RUPEE,
    /* 0x08 */ item_getcheck_func_S_MAGIC,
    /* 0x09 */ item_getcheck_func_L_MAGIC,
    /* 0x0A */ item_getcheck_func_BOMB_5,
    /* 0x0B */ item_getcheck_func_BOMB_10,
    /* 0x0C */ item_getcheck_func_BOMB_20,
    /* 0x0D */ item_getcheck_func_BOMB_30,
    /* 0x0E */ item_getcheck_func_ARROW_10,
    /* 0x0F */ item_getcheck_func_ARROW_20,
    /* 0x10 */ item_getcheck_func_ARROW_30,
    /* 0x11 */ item_getcheck_func_ARROW_1,
    /* 0x12 */ item_getcheck_func_PACHINKO_SHOT,
    /* 0x13 */ item_getcheck_func_noentry,
    /* 0x14 */ item_getcheck_func_noentry,
    /* 0x15 */ item_getcheck_func_noentry,
    /* 0x16 */ item_getcheck_func_WATER_BOMB_5,
    /* 0x17 */ item_getcheck_func_WATER_BOMB_10,
    /* 0x18 */ item_getcheck_func_WATER_BOMB_20,
    /* 0x19 */ item_getcheck_func_WATER_BOMB_30,
    /* 0x1A */ item_getcheck_func_BOMB_INSECT_5,
    /* 0x1B */ item_getcheck_func_BOMB_INSECT_10,
    /* 0x1C */ item_getcheck_func_BOMB_INSECT_20,
    /* 0x1D */ item_getcheck_func_BOMB_INSECT_30,
    /* 0x1E */ item_getcheck_func_RECOVER_FAILY,
    /* 0x1F */ item_getcheck_func_TRIPLE_HEART,
    /* 0x20 */ item_getcheck_func_SMALL_KEY,
    /* 0x21 */ item_getcheck_func_KAKERA_HEART,
    /* 0x22 */ item_getcheck_func_UTUWA_HEART,
    /* 0x23 */ item_getcheck_func_MAP,
    /* 0x24 */ item_getcheck_func_COMPUS,
    /* 0x25 */ item_getcheck_func_DUNGEON_EXIT,
    /* 0x26 */ item_getcheck_func_BOSS_KEY,
    /* 0x27 */ item_getcheck_func_DUNGEON_BACK,
    /* 0x28 */ item_getcheck_func_SWORD,
    /* 0x29 */ item_getcheck_func_MASTER_SWORD,
    /* 0x2A */ item_getcheck_func_WOOD_SHIELD,
    /* 0x2B */ item_getcheck_func_SHIELD,
    /* 0x2C */ item_getcheck_func_HYLIA_SHIELD,
    /* 0x2D */ item_getcheck_func_TKS_LETTER,
    /* 0x2E */ item_getcheck_func_WEAR_CASUAL,
    /* 0x2F */ item_getcheck_func_WEAR_KOKIRI,
    /* 0x30 */ item_getcheck_func_ARMOR,
    /* 0x31 */ item_getcheck_func_WEAR_ZORA,
    /* 0x32 */ item_getcheck_func_MAGIC_LV1,
    /* 0x33 */ item_getcheck_func_DUNGEON_EXIT_2,
    /* 0x34 */ item_getcheck_func_WALLET_LV1,
    /* 0x35 */ item_getcheck_func_WALLET_LV2,
    /* 0x36 */ item_getcheck_func_WALLET_LV3,
    /* 0x37 */ item_getcheck_func_noentry,
    /* 0x38 */ item_getcheck_func_noentry,
    /* 0x39 */ item_getcheck_func_noentry,
    /* 0x3A */ item_getcheck_func_noentry,
    /* 0x3B */ item_getcheck_func_noentry,
    /* 0x3C */ item_getcheck_func_noentry,
    /* 0x3D */ item_getcheck_func_ZORAS_JEWEL,
    /* 0x3E */ item_getcheck_func_HAWK_EYE,
    /* 0x3F */ item_getcheck_func_WOOD_STICK,
    /* 0x40 */ item_getcheck_func_BOOMERANG,
    /* 0x41 */ item_getcheck_func_SPINNER,
    /* 0x42 */ item_getcheck_func_IRONBALL,
    /* 0x43 */ item_getcheck_func_BOW,
    /* 0x44 */ item_getcheck_func_HOOKSHOT,
    /* 0x45 */ item_getcheck_func_HVY_BOOTS,
    /* 0x46 */ item_getcheck_func_COPY_ROD,
    /* 0x47 */ item_getcheck_func_W_HOOKSHOT,
    /* 0x48 */ item_getcheck_func_KANTERA,
    /* 0x49 */ item_getcheck_func_LIGHT_SWORD,
    /* 0x4A */ item_getcheck_func_FISHING_ROD_1,
    /* 0x4B */ item_getcheck_func_PACHINKO,
    /* 0x4C */ item_getcheck_func_COPY_ROD_2,
    /* 0x4D */ item_getcheck_func_noentry,
    /* 0x4E */ item_getcheck_func_noentry,
    /* 0x4F */ item_getcheck_func_BOMB_BAG_LV2,
    /* 0x50 */ item_getcheck_func_BOMB_BAG_LV1,
    /* 0x51 */ item_getcheck_func_BOMB_IN_BAG,
    /* 0x52 */ item_getcheck_func_noentry,
    /* 0x53 */ item_getcheck_func_LIGHT_ARROW,
    /* 0x54 */ item_getcheck_func_ARROW_LV1,
    /* 0x55 */ item_getcheck_func_ARROW_LV2,
    /* 0x56 */ item_getcheck_func_ARROW_LV3,
    /* 0x57 */ item_getcheck_func_noentry,
    /* 0x58 */ item_getcheck_func_LURE_ROD,
    /* 0x59 */ item_getcheck_func_BOMB_ARROW,
    /* 0x5A */ item_getcheck_func_HAWK_ARROW,
    /* 0x5B */ item_getcheck_func_BEE_ROD,
    /* 0x5C */ item_getcheck_func_JEWEL_ROD,
    /* 0x5D */ item_getcheck_func_WORM_ROD,
    /* 0x5E */ item_getcheck_func_JEWEL_BEE_ROD,
    /* 0x5F */ item_getcheck_func_JEWEL_WORM_ROD,
    /* 0x60 */ item_getcheck_func_EMPTY_BOTTLE,
    /* 0x61 */ item_getcheck_func_RED_BOTTLE,
    /* 0x62 */ item_getcheck_func_GREEN_BOTTLE,
    /* 0x63 */ item_getcheck_func_BLUE_BOTTLE,
    /* 0x64 */ item_getcheck_func_MILK_BOTTLE,
    /* 0x65 */ item_getcheck_func_HALF_MILK_BOTTLE,
    /* 0x66 */ item_getcheck_func_OIL_BOTTLE,
    /* 0x67 */ item_getcheck_func_WATER_BOTTLE,
    /* 0x68 */ item_getcheck_func_OIL_BOTTLE2,
    /* 0x69 */ item_getcheck_func_RED_BOTTLE2,
    /* 0x6A */ item_getcheck_func_UGLY_SOUP,
    /* 0x6B */ item_getcheck_func_HOT_SPRING,
    /* 0x6C */ item_getcheck_func_FAIRY_BOTTLE,
    /* 0x6D */ item_getcheck_func_HOT_SPRING2,
    /* 0x6E */ item_getcheck_func_OIL2,
    /* 0x6F */ item_getcheck_func_OIL,
    /* 0x70 */ item_getcheck_func_NORMAL_BOMB,
    /* 0x71 */ item_getcheck_func_WATER_BOMB,
    /* 0x72 */ item_getcheck_func_POKE_BOMB,
    /* 0x73 */ item_getcheck_func_FAIRY_DROP,
    /* 0x74 */ item_getcheck_func_WORM,
    /* 0x75 */ item_getcheck_func_DROP_BOTTLE,
    /* 0x76 */ item_getcheck_func_BEE_CHILD,
    /* 0x77 */ item_getcheck_func_CHUCHU_RARE,
    /* 0x78 */ item_getcheck_func_CHUCHU_RED,
    /* 0x79 */ item_getcheck_func_CHUCHU_BLUE,
    /* 0x7A */ item_getcheck_func_CHUCHU_GREEN,
    /* 0x7B */ item_getcheck_func_CHUCHU_YELLOW,
    /* 0x7C */ item_getcheck_func_CHUCHU_PURPLE,
    /* 0x7D */ item_getcheck_func_LV1_SOUP,
    /* 0x7E */ item_getcheck_func_LV2_SOUP,
    /* 0x7F */ item_getcheck_func_LV3_SOUP,
    /* 0x80 */ item_getcheck_func_LETTER,
    /* 0x81 */ item_getcheck_func_BILL,
    /* 0x82 */ item_getcheck_func_WOOD_STATUE,
    /* 0x83 */ item_getcheck_func_IRIAS_PENDANT,
    /* 0x84 */ item_getcheck_func_HORSE_FLUTE,
    /* 0x85 */ item_getcheck_func_noentry,
    /* 0x86 */ item_getcheck_func_noentry,
    /* 0x87 */ item_getcheck_func_noentry,
    /* 0x88 */ item_getcheck_func_noentry,
    /* 0x89 */ item_getcheck_func_noentry,
    /* 0x8A */ item_getcheck_func_noentry,
    /* 0x8B */ item_getcheck_func_noentry,
    /* 0x8C */ item_getcheck_func_noentry,
    /* 0x8D */ item_getcheck_func_noentry,
    /* 0x8E */ item_getcheck_func_noentry,
    /* 0x8F */ item_getcheck_func_noentry,
    /* 0x90 */ item_getcheck_func_RAFRELS_MEMO,
    /* 0x91 */ item_getcheck_func_ASHS_SCRIBBLING,
    /* 0x92 */ item_getcheck_func_noentry,
    /* 0x93 */ item_getcheck_func_noentry,
    /* 0x94 */ item_getcheck_func_noentry,
    /* 0x95 */ item_getcheck_func_noentry,
    /* 0x96 */ item_getcheck_func_noentry,
    /* 0x97 */ item_getcheck_func_noentry,
    /* 0x98 */ item_getcheck_func_noentry,
    /* 0x99 */ item_getcheck_func_noentry,
    /* 0x9A */ item_getcheck_func_noentry,
    /* 0x9B */ item_getcheck_func_noentry,
    /* 0x9C */ item_getcheck_func_CHUCHU_YELLOW2,
    /* 0x9D */ item_getcheck_func_OIL_BOTTLE3,
    /* 0x9E */ item_getcheck_func_SHOP_BEE_CHILD,
    /* 0x9F */ item_getcheck_func_CHUCHU_BLACK,
    /* 0xA0 */ item_getcheck_func_LIGHT_DROP,
    /* 0xA1 */ item_getcheck_func_DROP_CONTAINER,
    /* 0xA2 */ item_getcheck_func_DROP_CONTAINER02,
    /* 0xA3 */ item_getcheck_func_DROP_CONTAINER03,
    /* 0xA4 */ item_getcheck_func_FILLED_CONTAINER,
    /* 0xA5 */ item_getcheck_func_MIRROR_PIECE_2,
    /* 0xA6 */ item_getcheck_func_MIRROR_PIECE_3,
    /* 0xA7 */ item_getcheck_func_MIRROR_PIECE_4,
    /* 0xA8 */ item_getcheck_func_noentry,
    /* 0xA9 */ item_getcheck_func_noentry,
    /* 0xAA */ item_getcheck_func_noentry,
    /* 0xAB */ item_getcheck_func_noentry,
    /* 0xAC */ item_getcheck_func_noentry,
    /* 0xAD */ item_getcheck_func_noentry,
    /* 0xAE */ item_getcheck_func_noentry,
    /* 0xAF */ item_getcheck_func_noentry,
    /* 0xB0 */ item_getcheck_func_SMELL_YELIA_POUCH,
    /* 0xB1 */ item_getcheck_func_SMELL_PUMPKIN,
    /* 0xB2 */ item_getcheck_func_SMELL_POH,
    /* 0xB3 */ item_getcheck_func_SMELL_FISH,
    /* 0xB4 */ item_getcheck_func_SMELL_CHILDREN,
    /* 0xB5 */ item_getcheck_func_SMELL_MEDICINE,
    /* 0xB6 */ item_getcheck_func_noentry,
    /* 0xB7 */ item_getcheck_func_noentry,
    /* 0xB8 */ item_getcheck_func_noentry,
    /* 0xB9 */ item_getcheck_func_noentry,
    /* 0xBA */ item_getcheck_func_noentry,
    /* 0xBB */ item_getcheck_func_noentry,
    /* 0xBC */ item_getcheck_func_noentry,
    /* 0xBD */ item_getcheck_func_noentry,
    /* 0xBE */ item_getcheck_func_noentry,
    /* 0xBF */ item_getcheck_func_noentry,
    /* 0xC0 */ item_getcheck_func_M_BEETLE,
    /* 0xC1 */ item_getcheck_func_F_BEETLE,
    /* 0xC2 */ item_getcheck_func_M_BUTTERFLY,
    /* 0xC3 */ item_getcheck_func_F_BUTTERFLY,
    /* 0xC4 */ item_getcheck_func_M_STAG_BEETLE,
    /* 0xC5 */ item_getcheck_func_F_STAG_BEETLE,
    /* 0xC6 */ item_getcheck_func_M_GRASSHOPPER,
    /* 0xC7 */ item_getcheck_func_F_GRASSHOPPER,
    /* 0xC8 */ item_getcheck_func_M_NANAFUSHI,
    /* 0xC9 */ item_getcheck_func_F_NANAFUSHI,
    /* 0xCA */ item_getcheck_func_M_DANGOMUSHI,
    /* 0xCB */ item_getcheck_func_F_DANGOMUSHI,
    /* 0xCC */ item_getcheck_func_M_MANTIS,
    /* 0xCD */ item_getcheck_func_F_MANTIS,
    /* 0xCE */ item_getcheck_func_M_LADYBUG,
    /* 0xCF */ item_getcheck_func_F_LADYBUG,
    /* 0xD0 */ item_getcheck_func_M_SNAIL,
    /* 0xD1 */ item_getcheck_func_F_SNAIL,
    /* 0xD2 */ item_getcheck_func_M_DRAGONFLY,
    /* 0xD3 */ item_getcheck_func_F_DRAGONFLY,
    /* 0xD4 */ item_getcheck_func_M_ANT,
    /* 0xD5 */ item_getcheck_func_F_ANT,
    /* 0xD6 */ item_getcheck_func_M_MAYFLY,
    /* 0xD7 */ item_getcheck_func_F_MAYFLY,
    /* 0xD8 */ item_getcheck_func_noentry,
    /* 0xD9 */ item_getcheck_func_noentry,
    /* 0xDA */ item_getcheck_func_noentry,
    /* 0xDB */ item_getcheck_func_noentry,
    /* 0xDC */ item_getcheck_func_noentry,
    /* 0xDD */ item_getcheck_func_noentry,
    /* 0xDE */ item_getcheck_func_noentry,
    /* 0xDF */ item_getcheck_func_noentry,
    /* 0xE0 */ item_getcheck_func_POU_SPIRIT,
    /* 0xE1 */ item_getcheck_func_noentry,
    /* 0xE2 */ item_getcheck_func_noentry,
    /* 0xE3 */ item_getcheck_func_noentry,
    /* 0xE4 */ item_getcheck_func_noentry,
    /* 0xE5 */ item_getcheck_func_noentry,
    /* 0xE6 */ item_getcheck_func_noentry,
    /* 0xE7 */ item_getcheck_func_noentry,
    /* 0xE8 */ item_getcheck_func_noentry,
    /* 0xE9 */ item_getcheck_func_ANCIENT_DOCUMENT,
    /* 0xEA */ item_getcheck_func_AIR_LETTER,
    /* 0xEB */ item_getcheck_func_ANCIENT_DOCUMENT2,
    /* 0xEC */ item_getcheck_func_LV7_DUNGEON_EXIT,
    /* 0xED */ item_getcheck_func_LINKS_SAVINGS,
    /* 0xEE */ item_getcheck_func_SMALL_KEY2,
    /* 0xEF */ item_getcheck_func_POU_FIRE1,
    /* 0xF0 */ item_getcheck_func_POU_FIRE2,
    /* 0xF1 */ item_getcheck_func_POU_FIRE3,
    /* 0xF2 */ item_getcheck_func_POU_FIRE4,
    /* 0xF3 */ item_getcheck_func_BOSSRIDER_KEY,
    /* 0xF4 */ item_getcheck_func_TOMATO_PUREE,
    /* 0xF5 */ item_getcheck_func_TASTE,
    /* 0xF6 */ item_getcheck_func_LV5_BOSS_KEY,
    /* 0xF7 */ item_getcheck_func_SURFBOARD,
    /* 0xF8 */ item_getcheck_func_KANTERA2,
    /* 0xF9 */ item_getcheck_func_L2_KEY_PIECES1,
    /* 0xFA */ item_getcheck_func_L2_KEY_PIECES2,
    /* 0xFB */ item_getcheck_func_L2_KEY_PIECES3,
    /* 0xFC */ item_getcheck_func_KEY_OF_CARAVAN,
    /* 0xFD */ item_getcheck_func_LV2_BOSS_KEY,
    /* 0xFE */ item_getcheck_func_KEY_OF_FILONE,
    /* 0xFF */ item_getcheck_func_noentry,
};

#if TARGET_PC
static int (*item_getcheck_func_ptr_randomizer[256])() = {
    /* 0x00 */ item_getcheck_func_HEART,
    /* 0x01 */ item_getcheck_func_GREEN_RUPEE,
    /* 0x02 */ item_getcheck_func_BLUE_RUPEE,
    /* 0x03 */ item_getcheck_func_YELLOW_RUPEE,
    /* 0x04 */ item_getcheck_func_RED_RUPEE,
    /* 0x05 */ item_getcheck_func_PURPLE_RUPEE,
    /* 0x06 */ item_getcheck_func_ORANGE_RUPEE,
    /* 0x07 */ item_getcheck_func_SILVER_RUPEE,
    /* 0x08 */ item_getcheck_func_S_MAGIC,
    /* 0x09 */ item_getcheck_func_L_MAGIC,
    /* 0x0A */ item_getcheck_func_BOMB_5,
    /* 0x0B */ item_getcheck_func_BOMB_10,
    /* 0x0C */ item_getcheck_func_BOMB_20,
    /* 0x0D */ item_getcheck_func_BOMB_30,
    /* 0x0E */ item_getcheck_func_ARROW_10,
    /* 0x0F */ item_getcheck_func_ARROW_20,
    /* 0x10 */ item_getcheck_func_ARROW_30,
    /* 0x11 */ item_getcheck_func_ARROW_1,
    /* 0x12 */ item_getcheck_func_PACHINKO_SHOT,
    /* 0x13 */ item_getcheck_func_noentry,
    /* 0x14 */ item_getcheck_func_ORDON_PORTAL,
    /* 0x15 */ item_getcheck_func_SOUTH_FARON_PORTAL,
    /* 0x16 */ item_getcheck_func_WATER_BOMB_5,
    /* 0x17 */ item_getcheck_func_WATER_BOMB_10,
    /* 0x18 */ item_getcheck_func_WATER_BOMB_20,
    /* 0x19 */ item_getcheck_func_WATER_BOMB_30,
    /* 0x1A */ item_getcheck_func_BOMB_INSECT_5,
    /* 0x1B */ item_getcheck_func_BOMB_INSECT_10,
    /* 0x1C */ item_getcheck_func_BOMB_INSECT_20,
    /* 0x1D */ item_getcheck_func_BOMB_INSECT_30,
    /* 0x1E */ item_getcheck_func_RECOVER_FAILY,
    /* 0x1F */ item_getcheck_func_TRIPLE_HEART,
    /* 0x20 */ item_getcheck_func_SMALL_KEY,
    /* 0x21 */ item_getcheck_func_KAKERA_HEART,
    /* 0x22 */ item_getcheck_func_UTUWA_HEART,
    /* 0x23 */ item_getcheck_func_MAP,
    /* 0x24 */ item_getcheck_func_COMPUS,
    /* 0x25 */ item_getcheck_func_DUNGEON_EXIT,
    /* 0x26 */ item_getcheck_func_BOSS_KEY,
    /* 0x27 */ item_getcheck_func_DUNGEON_BACK,
    /* 0x28 */ item_getcheck_func_SWORD,
    /* 0x29 */ item_getcheck_func_MASTER_SWORD,
    /* 0x2A */ item_getcheck_func_WOOD_SHIELD,
    /* 0x2B */ item_getcheck_func_SHIELD,
    /* 0x2C */ item_getcheck_func_HYLIA_SHIELD,
    /* 0x2D */ item_getcheck_func_TKS_LETTER,
    /* 0x2E */ item_getcheck_func_WEAR_CASUAL,
    /* 0x2F */ item_getcheck_func_WEAR_KOKIRI,
    /* 0x30 */ item_getcheck_func_ARMOR,
    /* 0x31 */ item_getcheck_func_WEAR_ZORA,
    /* 0x32 */ item_getcheck_func_MAGIC_LV1,
    /* 0x33 */ item_getcheck_func_DUNGEON_EXIT_2,
    /* 0x34 */ item_getcheck_func_WALLET_LV1,
    /* 0x35 */ item_getcheck_func_WALLET_LV2,
    /* 0x36 */ item_getcheck_func_WALLET_LV3,
    /* 0x37 */ item_getcheck_func_noentry,
    /* 0x38 */ item_getcheck_func_noentry,
    /* 0x39 */ item_getcheck_func_UPPER_ZORAS_RIVER_PORTAL,
    /* 0x3A */ item_getcheck_func_CASTLE_TOWN_PORTAL,
    /* 0x3B */ item_getcheck_func_GERUDO_DESERT_PORTAL,
    /* 0x3C */ item_getcheck_func_NORTH_FARON_PORTAL,
    /* 0x3D */ item_getcheck_func_ZORAS_JEWEL,
    /* 0x3E */ item_getcheck_func_HAWK_EYE,
    /* 0x3F */ item_getcheck_func_WOOD_STICK,
    /* 0x40 */ item_getcheck_func_BOOMERANG,
    /* 0x41 */ item_getcheck_func_SPINNER,
    /* 0x42 */ item_getcheck_func_IRONBALL,
    /* 0x43 */ item_getcheck_func_BOW,
    /* 0x44 */ item_getcheck_func_HOOKSHOT,
    /* 0x45 */ item_getcheck_func_HVY_BOOTS,
    /* 0x46 */ item_getcheck_func_COPY_ROD,
    /* 0x47 */ item_getcheck_func_W_HOOKSHOT,
    /* 0x48 */ item_getcheck_func_KANTERA,
    /* 0x49 */ item_getcheck_func_LIGHT_SWORD,
    /* 0x4A */ item_getcheck_func_FISHING_ROD_1,
    /* 0x4B */ item_getcheck_func_PACHINKO,
    /* 0x4C */ item_getcheck_func_COPY_ROD_2,
    /* 0x4D */ item_getcheck_func_KAKARIKO_GORGE_PORTAL,
    /* 0x4E */ item_getcheck_func_KAKARIKO_VILLAGE_PORTAL,
    /* 0x4F */ item_getcheck_func_BOMB_BAG_LV2,
    /* 0x50 */ item_getcheck_func_BOMB_BAG_LV1,
    /* 0x51 */ item_getcheck_func_BOMB_IN_BAG,
    /* 0x52 */ item_getcheck_func_DEATH_MOUNTAIN_PORTAL,
    /* 0x53 */ item_getcheck_func_LIGHT_ARROW,
    /* 0x54 */ item_getcheck_func_ARROW_LV1,
    /* 0x55 */ item_getcheck_func_ARROW_LV2,
    /* 0x56 */ item_getcheck_func_ARROW_LV3,
    /* 0x57 */ item_getcheck_func_ZORAS_DOMAIN_PORTAL,
    /* 0x58 */ item_getcheck_func_LURE_ROD,
    /* 0x59 */ item_getcheck_func_BOMB_ARROW,
    /* 0x5A */ item_getcheck_func_HAWK_ARROW,
    /* 0x5B */ item_getcheck_func_BEE_ROD,
    /* 0x5C */ item_getcheck_func_JEWEL_ROD,
    /* 0x5D */ item_getcheck_func_WORM_ROD,
    /* 0x5E */ item_getcheck_func_JEWEL_BEE_ROD,
    /* 0x5F */ item_getcheck_func_JEWEL_WORM_ROD,
    /* 0x60 */ item_getcheck_func_EMPTY_BOTTLE,
    /* 0x61 */ item_getcheck_func_RED_BOTTLE,
    /* 0x62 */ item_getcheck_func_GREEN_BOTTLE,
    /* 0x63 */ item_getcheck_func_BLUE_BOTTLE,
    /* 0x64 */ item_getcheck_func_MILK_BOTTLE,
    /* 0x65 */ item_getcheck_func_HALF_MILK_BOTTLE,
    /* 0x66 */ item_getcheck_func_OIL_BOTTLE,
    /* 0x67 */ item_getcheck_func_WATER_BOTTLE,
    /* 0x68 */ item_getcheck_func_OIL_BOTTLE2,
    /* 0x69 */ item_getcheck_func_RED_BOTTLE2,
    /* 0x6A */ item_getcheck_func_UGLY_SOUP,
    /* 0x6B */ item_getcheck_func_HOT_SPRING,
    /* 0x6C */ item_getcheck_func_FAIRY_BOTTLE,
    /* 0x6D */ item_getcheck_func_HOT_SPRING2,
    /* 0x6E */ item_getcheck_func_OIL2,
    /* 0x6F */ item_getcheck_func_OIL,
    /* 0x70 */ item_getcheck_func_NORMAL_BOMB,
    /* 0x71 */ item_getcheck_func_WATER_BOMB,
    /* 0x72 */ item_getcheck_func_POKE_BOMB,
    /* 0x73 */ item_getcheck_func_FAIRY_DROP,
    /* 0x74 */ item_getcheck_func_WORM,
    /* 0x75 */ item_getcheck_func_DROP_BOTTLE,
    /* 0x76 */ item_getcheck_func_BEE_CHILD,
    /* 0x77 */ item_getcheck_func_CHUCHU_RARE,
    /* 0x78 */ item_getcheck_func_CHUCHU_RED,
    /* 0x79 */ item_getcheck_func_CHUCHU_BLUE,
    /* 0x7A */ item_getcheck_func_CHUCHU_GREEN,
    /* 0x7B */ item_getcheck_func_CHUCHU_YELLOW,
    /* 0x7C */ item_getcheck_func_CHUCHU_PURPLE,
    /* 0x7D */ item_getcheck_func_LV1_SOUP,
    /* 0x7E */ item_getcheck_func_LV2_SOUP,
    /* 0x7F */ item_getcheck_func_LV3_SOUP,
    /* 0x80 */ item_getcheck_func_LETTER,
    /* 0x81 */ item_getcheck_func_BILL,
    /* 0x82 */ item_getcheck_func_WOOD_STATUE,
    /* 0x83 */ item_getcheck_func_IRIAS_PENDANT,
    /* 0x84 */ item_getcheck_func_HORSE_FLUTE,
    /* 0x85 */ item_getcheck_func_noentry,
    /* 0x86 */ item_getcheck_func_noentry,
    /* 0x87 */ item_getcheck_func_noentry,
    /* 0x88 */ item_getcheck_func_noentry,
    /* 0x89 */ item_getcheck_func_noentry,
    /* 0x8A */ item_getcheck_func_noentry,
    /* 0x8B */ item_getcheck_func_noentry,
    /* 0x8C */ item_getcheck_func_noentry,
    /* 0x8D */ item_getcheck_func_noentry,
    /* 0x8E */ item_getcheck_func_CAMP_SMALL_KEY,
    /* 0x8F */ item_getcheck_func_LAKE_HYLIA_PORTAL,
    /* 0x90 */ item_getcheck_func_RAFRELS_MEMO,
    /* 0x91 */ item_getcheck_func_ASHS_SCRIBBLING,
    /* 0x92 */ item_getcheck_func_noentry,
    /* 0x93 */ item_getcheck_func_noentry,
    /* 0x94 */ item_getcheck_func_noentry,
    /* 0x95 */ item_getcheck_func_noentry,
    /* 0x96 */ item_getcheck_func_noentry,
    /* 0x97 */ item_getcheck_func_noentry,
    /* 0x98 */ item_getcheck_func_noentry,
    /* 0x99 */ item_getcheck_func_noentry,
    /* 0x9A */ item_getcheck_func_noentry,
    /* 0x9B */ item_getcheck_func_noentry,
    /* 0x9C */ item_getcheck_func_CHUCHU_YELLOW2,
    /* 0x9D */ item_getcheck_func_OIL_BOTTLE3,
    /* 0x9E */ item_getcheck_func_SHOP_BEE_CHILD,
    /* 0x9F */ item_getcheck_func_CHUCHU_BLACK,
    /* 0xA0 */ item_getcheck_func_LIGHT_DROP,
    /* 0xA1 */ item_getcheck_func_DROP_CONTAINER,
    /* 0xA2 */ item_getcheck_func_DROP_CONTAINER02,
    /* 0xA3 */ item_getcheck_func_DROP_CONTAINER03,
    /* 0xA4 */ item_getcheck_func_FILLED_CONTAINER,
    /* 0xA5 */ item_getcheck_func_MIRROR_PIECE_2,
    /* 0xA6 */ item_getcheck_func_MIRROR_PIECE_3,
    /* 0xA7 */ item_getcheck_func_MIRROR_PIECE_4,
    /* 0xA8 */ item_getcheck_func_noentry,
    /* 0xA9 */ item_getcheck_func_noentry,
    /* 0xAA */ item_getcheck_func_noentry,
    /* 0xAB */ item_getcheck_func_noentry,
    /* 0xAC */ item_getcheck_func_noentry,
    /* 0xAD */ item_getcheck_func_noentry,
    /* 0xAE */ item_getcheck_func_MIRROR_CHAMBER_PORTAL,
    /* 0xAF */ item_getcheck_func_SNOWPEAK_PORTAL,
    /* 0xB0 */ item_getcheck_func_SMELL_YELIA_POUCH,
    /* 0xB1 */ item_getcheck_func_SMELL_PUMPKIN,
    /* 0xB2 */ item_getcheck_func_SMELL_POH,
    /* 0xB3 */ item_getcheck_func_SMELL_FISH,
    /* 0xB4 */ item_getcheck_func_SMELL_CHILDREN,
    /* 0xB5 */ item_getcheck_func_SMELL_MEDICINE,
    /* 0xB6 */ item_getcheck_func_noentry,
    /* 0xB7 */ item_getcheck_func_noentry,
    /* 0xB8 */ item_getcheck_func_noentry,
    /* 0xB9 */ item_getcheck_func_noentry,
    /* 0xBA */ item_getcheck_func_noentry,
    /* 0xBB */ item_getcheck_func_noentry,
    /* 0xBC */ item_getcheck_func_noentry,
    /* 0xBD */ item_getcheck_func_noentry,
    /* 0xBE */ item_getcheck_func_noentry,
    /* 0xBF */ item_getcheck_func_SACRED_GROVE_PORTAL,
    /* 0xC0 */ item_getcheck_func_M_BEETLE,
    /* 0xC1 */ item_getcheck_func_F_BEETLE,
    /* 0xC2 */ item_getcheck_func_M_BUTTERFLY,
    /* 0xC3 */ item_getcheck_func_F_BUTTERFLY,
    /* 0xC4 */ item_getcheck_func_M_STAG_BEETLE,
    /* 0xC5 */ item_getcheck_func_F_STAG_BEETLE,
    /* 0xC6 */ item_getcheck_func_M_GRASSHOPPER,
    /* 0xC7 */ item_getcheck_func_F_GRASSHOPPER,
    /* 0xC8 */ item_getcheck_func_M_NANAFUSHI,
    /* 0xC9 */ item_getcheck_func_F_NANAFUSHI,
    /* 0xCA */ item_getcheck_func_M_DANGOMUSHI,
    /* 0xCB */ item_getcheck_func_F_DANGOMUSHI,
    /* 0xCC */ item_getcheck_func_M_MANTIS,
    /* 0xCD */ item_getcheck_func_F_MANTIS,
    /* 0xCE */ item_getcheck_func_M_LADYBUG,
    /* 0xCF */ item_getcheck_func_F_LADYBUG,
    /* 0xD0 */ item_getcheck_func_M_SNAIL,
    /* 0xD1 */ item_getcheck_func_F_SNAIL,
    /* 0xD2 */ item_getcheck_func_M_DRAGONFLY,
    /* 0xD3 */ item_getcheck_func_F_DRAGONFLY,
    /* 0xD4 */ item_getcheck_func_M_ANT,
    /* 0xD5 */ item_getcheck_func_F_ANT,
    /* 0xD6 */ item_getcheck_func_M_MAYFLY,
    /* 0xD7 */ item_getcheck_func_F_MAYFLY,
    /* 0xD8 */ item_getcheck_func_FUSED_SHADOW_1,
    /* 0xD9 */ item_getcheck_func_FUSED_SHADOW_2,
    /* 0xDA */ item_getcheck_func_FUSED_SHADOW_3,
    /* 0xDB */ item_getcheck_func_MIRROR_PIECE_1,
    /* 0xDC */ item_getcheck_func_noentry,
    /* 0xDD */ item_getcheck_func_noentry,
    /* 0xDE */ item_getcheck_func_noentry,
    /* 0xDF */ item_getcheck_func_noentry,
    /* 0xE0 */ item_getcheck_func_POU_SPIRIT,
    /* 0xE1 */ item_getcheck_func_ENDING_BLOW,
    /* 0xE2 */ item_getcheck_func_SHIELD_ATTACK,
    /* 0xE3 */ item_getcheck_func_BACK_SLICE,
    /* 0xE4 */ item_getcheck_func_HELM_SPLITTER,
    /* 0xE5 */ item_getcheck_func_MORTAL_DRAW,
    /* 0xE6 */ item_getcheck_func_JUMP_STRIKE,
    /* 0xE7 */ item_getcheck_func_GREAT_SPIN,
    /* 0xE8 */ item_getcheck_func_ELDIN_BRIDGE_PORTAL,
    /* 0xE9 */ item_getcheck_func_ANCIENT_DOCUMENT,
    /* 0xEA */ item_getcheck_func_AIR_LETTER,
    /* 0xEB */ item_getcheck_func_ANCIENT_DOCUMENT2,
    /* 0xEC */ item_getcheck_func_LV7_DUNGEON_EXIT,
    /* 0xED */ item_getcheck_func_LINKS_SAVINGS,
    /* 0xEE */ item_getcheck_func_SMALL_KEY2,
    /* 0xEF */ item_getcheck_func_POU_FIRE1,
    /* 0xF0 */ item_getcheck_func_POU_FIRE2,
    /* 0xF1 */ item_getcheck_func_POU_FIRE3,
    /* 0xF2 */ item_getcheck_func_POU_FIRE4,
    /* 0xF3 */ item_getcheck_func_BOSSRIDER_KEY,
    /* 0xF4 */ item_getcheck_func_TOMATO_PUREE,
    /* 0xF5 */ item_getcheck_func_TASTE,
    /* 0xF6 */ item_getcheck_func_LV5_BOSS_KEY,
    /* 0xF7 */ item_getcheck_func_SURFBOARD,
    /* 0xF8 */ item_getcheck_func_KANTERA2,
    /* 0xF9 */ item_getcheck_func_L2_KEY_PIECES1,
    /* 0xFA */ item_getcheck_func_L2_KEY_PIECES2,
    /* 0xFB */ item_getcheck_func_L2_KEY_PIECES3,
    /* 0xFC */ item_getcheck_func_KEY_OF_CARAVAN,
    /* 0xFD */ item_getcheck_func_LV2_BOSS_KEY,
    /* 0xFE */ item_getcheck_func_KEY_OF_FILONE,
    /* 0xFF */ item_getcheck_func_noentry,
};

#endif

inline int getCheckItemFunc(u8 i_itemNo) {
#if TARGET_PC
    return (randomizer_IsActive() ? item_getcheck_func_ptr_randomizer : item_getcheck_func_ptr)[i_itemNo]();
#else
    return item_getcheck_func_ptr[i_itemNo]();
#endif
}

int checkItemGet(u8 i_itemNo, int i_default) {
#if TARGET_PC
    // Check special randomizer cases
    if (randomizer_IsActive()) {
        switch (i_itemNo) {
        case dItemNo_Randomizer_HYLIA_SHIELD_e:
            // Check if we are at Kakariko Malo mart and verify that we have not bought the shield.
            if (playerIsInRoomStage(3, allStages[Kakariko_Village_Interiors]) &&
                !dComIfGs_isEventBit(BOUGHT_HYLIAN_SHIELD_AT_MALO_MART)) {
                    // Return false so we can buy the shield.
                    return 0;
            }
            break;
        case dItemNo_Randomizer_HAWK_EYE_e:
            // Check if we are at Kakariko Village and that the hawkeye is currently not for sale.
            if (getStageID() == Kakariko_Village && !dComIfGs_isSwitch(0x3E, 0)) {
                // Return false so we can buy the hawkeye.
                return 0;
            }
            break;
        case dItemNo_Randomizer_SHIELD_e:
        case dItemNo_Randomizer_WOOD_SHIELD_e:
            // Check if we are at Kakariko Malo mart and that the Wooden Shield has not been bought.
            if (playerIsInRoomStage(3, allStages[Kakariko_Village_Interiors]) &&
                !dComIfGs_isSwitch(0x5, 3)) {
                    // Return false so we can buy the shield.
                    return 0;
                }
            break;
        case dItemNo_Randomizer_TOMATO_PUREE_e:
        case dItemNo_Randomizer_TASTE_e:
            // Check to see if currently in Snowpeak Ruins
            if (getStageID() == Snowpeak_Ruins) {
                // Return false so that yeta will give the map item no matter what.
                return 0;
            }
            break;
        case dItemNo_Randomizer_IRONBALL_e:
            // Check to see if currently in Snowpeak Ruins Darkhammer room
            if (getStageID() == Darkhammer) {
                return dComIfGs_isSwitch(0x5F, 51); // Picked up the Ball and Chain check.
            }
        }
    }
#endif

    int result = getCheckItemFunc(i_itemNo);

    if (result == -1) {
        result = i_default;
    }

    return result;
}

void item_func_HEART() {
    dComIfGp_setItemLifeCount(4.0f, 0);
}

void item_func_GREEN_RUPEE() {
    dComIfGp_setItemRupeeCount(1);
}

void item_func_BLUE_RUPEE() {
    dComIfGp_setItemRupeeCount(5);
}

void item_func_YELLOW_RUPEE() {
    dComIfGp_setItemRupeeCount(10);
}

void item_func_RED_RUPEE() {
    dComIfGp_setItemRupeeCount(20);
}

void item_func_PURPLE_RUPEE() {
    dComIfGp_setItemRupeeCount(50);
}

void item_func_ORANGE_RUPEE() {
    dComIfGp_setItemRupeeCount(100);
}

void item_func_SILVER_RUPEE() {
    dComIfGp_setItemRupeeCount(200);
}

void item_func_S_MAGIC() {
    dComIfGp_setItemMagicCount(4);
}

void item_func_L_MAGIC() {
    dComIfGp_setItemMagicCount(8);
}

void item_func_BOMB_5() {
    addBombCount(dItemNo_NORMAL_BOMB_e, 5);
}

void item_func_BOMB_10() {
    addBombCount(dItemNo_NORMAL_BOMB_e, 10);
}

void item_func_BOMB_20() {
    addBombCount(dItemNo_NORMAL_BOMB_e, 20);
}

void item_func_BOMB_30() {
    addBombCount(dItemNo_NORMAL_BOMB_e, 30);
}

void item_func_ARROW_10() {
    dComIfGp_setItemArrowNumCount(10);
}

void item_func_ARROW_20() {
    dComIfGp_setItemArrowNumCount(20);
}

void item_func_ARROW_30() {
    dComIfGp_setItemArrowNumCount(30);
}

void item_func_ARROW_1() {
    dComIfGp_setItemArrowNumCount(1);
}

void item_func_PACHINKO_SHOT() {
    dComIfGp_setItemPachinkoNumCount(50);
}

#if TARGET_PC
void item_func_FOOLISH_ITEM() {
    // Failsafe: Make sure the count does not somehow exceed 100
    if (g_randomizerState.mFoolishItemCount < 100)
    {
        g_randomizerState.mFoolishItemCount += 1;
    }
}

void item_func_ORDON_PORTAL() {
    dComIfGs_onStageSwitch(0x0, 0x34); // Unlock Ordon Portal
}

void item_func_SOUTH_FARON_PORTAL() {
    dComIfGs_onStageSwitch(0x2, 0x47); // Unlock S Faron Portal
}

#endif

void item_func_WATER_BOMB_5() {
    addBombCount(dItemNo_WATER_BOMB_e, 5);
}

void item_func_WATER_BOMB_10() {
    addBombCount(dItemNo_WATER_BOMB_e, 10);
}

void item_func_WATER_BOMB_20() {
    addBombCount(dItemNo_WATER_BOMB_e, 15);
}

void item_func_WATER_BOMB_30() {
    addBombCount(dItemNo_WATER_BOMB_e, 3);
}

void item_func_BOMB_INSECT_5() {
    addBombCount(dItemNo_POKE_BOMB_e, 5);
}

void item_func_BOMB_INSECT_10() {
    addBombCount(dItemNo_POKE_BOMB_e, 10);
}

void item_func_BOMB_INSECT_20() {
    addBombCount(dItemNo_POKE_BOMB_e, 3);
}

void item_func_BOMB_INSECT_30() {}

void item_func_RECOVER_FAILY() {
    dComIfGp_setItemLifeCount(32.0f, 0);
}

void item_func_TRIPLE_HEART() {}

void item_func_SMALL_KEY() {
    dComIfGp_setItemKeyNumCount(1);
}

void item_func_KAKERA_HEART() {
    dComIfGp_setItemMaxLifeCount(1);
}

void item_func_UTUWA_HEART() {
    dComIfGp_setItemMaxLifeCount(5);

    f32 max_life = dComIfGs_getMaxLifeGauge();
    dComIfGp_setItemLifeCount(max_life, 0);

    stage_stag_info_class* stag_info = dComIfGp_getStageStagInfo();
    int tmp = dStage_stagInfo_GetSaveTbl(stag_info);
    dComIfGs_onStageLife();
#if TARGET_PC
    if (randomizer_IsActive()) {
        // TODO rando
        /*
        Pasting rando code until the framework has been updated
        uint8_t maxLife = libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.player.player_status_a.maxHealth + 1;

            // Check if we have enough hearts to break the barrier.
            randoPtr->checkSetHCBarrierFlag(rando::HC_Hearts, maxLife);

            // Check if we have enough hearts to unlock the BK check.
            randoPtr->checkSetHCBkFlag(rando::HC_BK_Hearts, maxLife);
        */
    }
#endif
}

void item_func_MAP() {
    dComIfGs_onDungeonItemMap();
}

void item_func_COMPUS() {
    dComIfGs_onDungeonItemCompass();
}

void item_func_DUNGEON_EXIT() {
    dComIfGs_onDungeonItemWarp();
    dComIfGs_setItem(SLOT_18, dItemNo_DUNGEON_EXIT_e);
}

void item_func_BOSS_KEY() {
    dComIfGs_onDungeonItemBossKey();
}

void item_func_DUNGEON_BACK() {
    dComIfGs_setItem(SLOT_18, dItemNo_DUNGEON_BACK_e);
}

void item_func_SWORD() {
    dComIfGs_setCollectSword(COLLECT_ORDON_SWORD);
    dComIfGs_setSelectEquipSword(dItemNo_SWORD_e);
}

void item_func_MASTER_SWORD() {
    dComIfGs_setCollectSword(COLLECT_MASTER_SWORD);
    dComIfGs_setSelectEquipSword(dItemNo_MASTER_SWORD_e);
}

void item_func_WOOD_SHIELD() {
    dComIfGs_setCollectShield(COLLECT_WOODEN_SHIELD);
    dComIfGs_setSelectEquipShield(dItemNo_WOOD_SHIELD_e);
}

void item_func_SHIELD() {}

void item_func_HYLIA_SHIELD() {}

void item_func_TKS_LETTER() {
    dComIfGs_setItem(SLOT_18, dItemNo_TKS_LETTER_e);
}

void item_func_WEAR_CASUAL() {
    dComIfGs_setSelectEquipClothes(dItemNo_WEAR_CASUAL_e);
}

void item_func_WEAR_KOKIRI() {
    dComIfGs_setCollectClothes(KOKIRI_CLOTHES_FLAG);
    dComIfGs_setSelectEquipClothes(dItemNo_WEAR_KOKIRI_e);
}

void item_func_ARMOR() {}

void item_func_WEAR_ZORA() {}

void item_func_MAGIC_LV1() {
#if TARGET_PC
    if (randomizer_IsActive()) {
        dComIfGs_onEventBit(0xD04); // Can transform at will
        dComIfGs_onEventBit(0x501); // Midna Charge Unlocked
        return;
    }
#endif
    dComIfGp_setItemMagicCount(16);
    dComIfGp_setItemMaxMagicCount(16);
}

void item_func_DUNGEON_EXIT_2() {
    dComIfGs_setItem(SLOT_18, dItemNo_DUNGEON_EXIT_e);
}

void item_func_WALLET_LV1() {
    dComIfGs_setWalletSize(WALLET);
}

void item_func_WALLET_LV2() {
    dComIfGs_setWalletSize(BIG_WALLET);
#if TARGET_PC
    if (randomizer_IsActive()) {
        // TODO rando
        // Putting rando code here until the framework gets built:
        /*
        Basically we fill the wallet if desired and then we change the color of the rupee icon to red
        libtp::tp::J2DWindow::J2DWindow* windowPtr =
                libtp::tp::d_meter2_info::g_meter2_info.mMeterClass->mpMeterDraw->mpBigHeart->mWindow;

            if (seedPtr->walletsAreAutoFilled())
            {
                plrStatusAPtr->currentRupees = mod::user_patch::walletValues[seedPtr->getHeaderPtr()->getWalletSize()][1];
            }

            if (!windowPtr)
            {
                return;
            }

            for (uint32_t rupee = 0x1038; rupee <= 0x1044; rupee += 0x4)
            {
                *reinterpret_cast<uint32_t*>(reinterpret_cast<uint32_t>(windowPtr) + rupee) = 0xff0000ff;
            }
        */
    }
#endif
}

void item_func_WALLET_LV3() {
    dComIfGs_setWalletSize(GIANT_WALLET);
#if TARGET_PC
    // TODO rando
    if (randomizer_IsActive()) {
        // Putting rando code here until the framework gets built:
        /*
        Basically we fill the wallet if desired and then we change the color of the rupee icon to red
        libtp::tp::J2DWindow::J2DWindow* windowPtr =
                libtp::tp::d_meter2_info::g_meter2_info.mMeterClass->mpMeterDraw->mpBigHeart->mWindow;

            if (seedPtr->walletsAreAutoFilled())
            {
                plrStatusAPtr->currentRupees = mod::user_patch::walletValues[seedPtr->getHeaderPtr()->getWalletSize()][2];
            }

            if (!windowPtr)
            {
                return;
            }

            for (uint32_t rupee = 0x1038; rupee <= 0x1044; rupee += 0x4)
            {
                *reinterpret_cast<uint32_t*>(reinterpret_cast<uint32_t>(windowPtr) + rupee) = 0xff00ffff;
            }
        */
    }
#endif
}

#if TARGET_PC
void item_func_UPPER_ZORAS_RIVER_PORTAL() {
    dComIfGs_onStageSwitch(0x4, 0x17); // Talked to Iza before portal
    dComIfGs_onStageSwitch(0x4, 0x37); // Talked to Iza after portal
    dComIfGs_onStageSwitch(0x4, 0x15); // Unlock UZR Portal
    dComIfGs_onEventBit(0xB80); // Declined to help Iza
    dComIfGs_onEventBit(0x1304); // Talked to Iza before UZR portal
    dComIfGs_onEventBit(0xB02); // Iza 1 Minigame Unlocked
}

void item_func_CASTLE_TOWN_PORTAL() {
    dComIfGs_onStageSwitch(0x6, 0x3); // Unlock Castle Town Portal
}

void item_func_GERUDO_DESERT_PORTAL() {
    dComIfGs_onStageSwitch(0xA, 0x15); // Unlock Desert Portal
}

void item_func_NORTH_FARON_PORTAL() {
    dComIfGs_onStageSwitch(0x2, 0x2); // Unlock N Faron Portal
}
#endif

void item_func_ZORAS_JEWEL() {
    if (item_getcheck_func_FISHING_ROD_1()) {
        dComIfGs_setRodTypeLevelUp();
    } else {
        dComIfGs_setItem(SLOT_20, dItemNo_ZORAS_JEWEL_e);
    }
}

void item_func_HAWK_EYE() {
    dComIfGs_setItem(SLOT_5, dItemNo_HAWK_EYE_e);
}

void item_func_WOOD_STICK() {
    dComIfGs_setCollectSword(COLLECT_WOODEN_SWORD);
    dComIfGs_setSelectEquipSword(dItemNo_WOOD_STICK_e);
#if TARGET_PC
    if (!randomizer_IsActive())
#endif
    dComIfGs_onSwitch(28, dComIfGp_roomControl_getStayNo());
}

void item_func_BOOMERANG() {
    dComIfGs_setItem(SLOT_0, dItemNo_BOOMERANG_e);
}

void item_func_SPINNER() {
    dComIfGs_setItem(SLOT_2, dItemNo_SPINNER_e);
}

void item_func_IRONBALL() {
    dComIfGs_setItem(SLOT_6, dItemNo_IRONBALL_e);
}

void item_func_BOW() {
    dComIfGs_setItem(SLOT_4, dItemNo_BOW_e);
    dComIfGs_setArrowNum(30);
    dComIfGs_setArrowMax(30);
}

void item_func_HOOKSHOT() {
    dComIfGs_setItem(SLOT_9, dItemNo_HOOKSHOT_e);
}

void item_func_HVY_BOOTS() {
    dComIfGs_setItem(SLOT_3, dItemNo_HVY_BOOTS_e);
}

void item_func_COPY_ROD() {
    dComIfGs_setItem(SLOT_8, dItemNo_COPY_ROD_e);
}

void item_func_W_HOOKSHOT() {
    dComIfGs_setItem(SLOT_9, dItemNo_NONE_e);
    dComIfGs_setItem(SLOT_10, dItemNo_W_HOOKSHOT_e);
}

void item_func_KANTERA() {
    dComIfGs_setMaxOil(21600);
    dComIfGs_setOil(21600);
    dComIfGs_setItem(SLOT_1, dItemNo_KANTERA_e);
}

void item_func_LIGHT_SWORD() {
    dComIfGs_setCollectSword(COLLECT_LIGHT_SWORD);
    dMeter2Info_setSword(dItemNo_LIGHT_SWORD_e, false);
}

void item_func_FISHING_ROD_1() {
    dComIfGs_setItem(SLOT_20, dItemNo_FISHING_ROD_1_e);
}

void item_func_PACHINKO() {
    u8 pachinko_max = dComIfGs_getPachinkoMax();
    dComIfGp_setItemPachinkoNumCount(pachinko_max);
    dComIfGs_setItem(SLOT_23, dItemNo_PACHINKO_e);
}

void item_func_COPY_ROD_2() {
#if TARGET_PC
    if (randomizer_IsActive())
        dComIfGs_onEventBit(0x2580); // Power up dominion rod
    else
#endif
    dComIfGs_setItem(SLOT_8, dItemNo_COPY_ROD_e);
}

#if TARGET_PC
void item_func_KAKARIKO_GORGE_PORTAL() {
    dComIfGs_onStageSwitch(0x6, 0x15); // Unlock Gorge Portal
}

void item_func_KAKARIKO_VILLAGE_PORTAL() {
    dComIfGs_onStageSwitch(0x3, 0x1F); // Unlock Kak Portal
}
#endif

void item_func_BOMB_BAG_LV2() {}

void item_func_BOMB_BAG_LV1() {
    dComIfGs_setEmptyBombBag(dItemNo_NORMAL_BOMB_e, 30);
}

void item_func_BOMB_IN_BAG() {
    dComIfGs_setEmptyBombBag(dItemNo_NORMAL_BOMB_e, 30);
}

#if TARGET_PC
void item_func_DEATH_MOUNTAIN_PORTAL() {
    dComIfGs_onStageSwitch(0x3, 0x15); // Unlock DM Portal
}
#endif

void item_func_LIGHT_ARROW() {
    dComIfGs_setItem(SLOT_4, dItemNo_LIGHT_ARROW_e);
}

void item_func_ARROW_LV1() {
    dComIfGs_setArrowNum(60);
    dComIfGs_setArrowMax(60);
}

void item_func_ARROW_LV2() {
    dComIfGs_setArrowNum(60);
    dComIfGs_setArrowMax(60);
}

void item_func_ARROW_LV3() {
    dComIfGs_setArrowNum(100);
    dComIfGs_setArrowMax(100);
}

#if TARGET_PC
void item_func_ZORAS_DOMAIN_PORTAL() {
    dComIfGs_onStageSwitch(0x4, 0x2); // Unlock ZD Portal
}
#endif

void item_func_LURE_ROD() {}

void item_func_BOMB_ARROW() {}

void item_func_HAWK_ARROW() {}

void item_func_BEE_ROD() {}

void item_func_JEWEL_ROD() {}

void item_func_WORM_ROD() {}

void item_func_JEWEL_BEE_ROD() {}

void item_func_JEWEL_WORM_ROD() {}

void item_func_EMPTY_BOTTLE() {
    dComIfGs_setEmptyBottle();
}

void item_func_RED_BOTTLE() {
    dComIfGs_setEmptyBottleItemIn(dItemNo_RED_BOTTLE_e);
}

void item_func_GREEN_BOTTLE() {
    dComIfGs_setEmptyBottleItemIn(dItemNo_GREEN_BOTTLE_e);
}

void item_func_BLUE_BOTTLE() {
    dComIfGs_setEmptyBottleItemIn(dItemNo_BLUE_BOTTLE_e);
}

void item_func_MILK_BOTTLE() {
    dComIfGs_setEmptyBottleItemIn(dItemNo_MILK_BOTTLE_e);
}

void item_func_HALF_MILK_BOTTLE() {
    dComIfGs_setEmptyBottle(dItemNo_HALF_MILK_BOTTLE_e);
}

void item_func_OIL_BOTTLE() {
    dComIfGs_setEmptyBottleItemIn(dItemNo_OIL_BOTTLE_e);
}

void item_func_WATER_BOTTLE() {
    dComIfGs_setEmptyBottleItemIn(dItemNo_WATER_BOTTLE_e);
}

void item_func_OIL_BOTTLE2() {
    dComIfGs_setEmptyBottleItemIn(dItemNo_OIL_BOTTLE_e);
}

void item_func_RED_BOTTLE2() {
    dComIfGs_setEmptyBottleItemIn(dItemNo_RED_BOTTLE_e);
}

void item_func_UGLY_SOUP() {
    dComIfGs_setEmptyBottleItemIn(dItemNo_UGLY_SOUP_e);
}

void item_func_HOT_SPRING() {
    dComIfGs_setEmptyBottleItemIn(dItemNo_HOT_SPRING_e);
}

void item_func_FAIRY_BOTTLE() {
    dComIfGs_setEmptyBottleItemIn(dItemNo_FAIRY_e);
}

void item_func_HOT_SPRING2() {
    dComIfGs_setEmptyBottleItemIn(dItemNo_HOT_SPRING_e);
}

void item_func_OIL2() {}

void item_func_OIL() {}

void item_func_NORMAL_BOMB() {
    dComIfGs_setEmptyBombBag(dItemNo_NORMAL_BOMB_e, 60);
}

void item_func_WATER_BOMB() {
    dComIfGs_setEmptyBombBag();
    dComIfGs_setEmptyBombBagItemIn(dItemNo_WATER_BOMB_e, true);
}

void item_func_POKE_BOMB() {
    dComIfGs_setEmptyBombBag();
    dComIfGs_setEmptyBombBagItemIn(dItemNo_POKE_BOMB_e, true);
}

void item_func_FAIRY_DROP() {
    dComIfGs_setEmptyBottleItemIn(dItemNo_FAIRY_DROP_e);
}

void item_func_WORM() {}

void item_func_DROP_BOTTLE() {
    dComIfGs_setEmptyBottle(dItemNo_FAIRY_DROP_e);
}

void item_func_BEE_CHILD() {
    int bottleIdx;
    int i;

    for (bottleIdx = 0xFF, i = 0; i < 4; i++) {
        u8 getItem = dComIfGs_getItem(i + SLOT_11, true);

        if (getItem == dItemNo_EMPTY_BOTTLE_e) {
            bottleIdx = i;
            break;
        }
    }

    if (bottleIdx != 0xff) {
        dComIfGs_setBottleNum(bottleIdx, 10);
        dComIfGs_setEmptyBottleItemIn(dItemNo_BEE_CHILD_e);
    }
}

void item_func_CHUCHU_RARE() {
    dComIfGs_setEmptyBottleItemIn(dItemNo_CHUCHU_RARE_e);
}

void item_func_CHUCHU_RED() {
    dComIfGs_setEmptyBottleItemIn(dItemNo_CHUCHU_RED_e);
}

void item_func_CHUCHU_BLUE() {
    dComIfGs_setEmptyBottleItemIn(dItemNo_CHUCHU_BLUE_e);
}

void item_func_CHUCHU_GREEN() {
    dComIfGs_setEmptyBottleItemIn(dItemNo_CHUCHU_GREEN_e);
}

void item_func_CHUCHU_YELLOW() {
    dComIfGs_setEmptyBottleItemIn(dItemNo_CHUCHU_YELLOW_e);
}

void item_func_CHUCHU_PURPLE() {
    dComIfGs_setEmptyBottleItemIn(dItemNo_CHUCHU_PURPLE_e);
}

void item_func_LV1_SOUP() {
    dComIfGs_setEmptyBottleItemIn(dItemNo_LV1_SOUP_e);
}

void item_func_LV2_SOUP() {
    dComIfGs_setEmptyBottleItemIn(dItemNo_LV2_SOUP_e);
}

void item_func_LV3_SOUP() {
    dComIfGs_setEmptyBottleItemIn(dItemNo_LV3_SOUP_e);
}

void item_func_LETTER() {
#if TARGET_PC
    if (randomizer_IsActive()) {
        if (dComIfGs_getItem(SLOT_21, true) != dItemNo_Randomizer_HORSE_FLUTE_e)
            dComIfGs_setItem(SLOT_21, dItemNo_Randomizer_LETTER_e);
    } else
#endif
    dComIfGs_setItem(SLOT_21, dItemNo_LETTER_e);
}

void item_func_BILL() {
#if TARGET_PC
    if (randomizer_IsActive()) {
        if (dComIfGs_getItem(SLOT_21, true) != dItemNo_Randomizer_HORSE_FLUTE_e)
            dComIfGs_setItem(SLOT_21, dItemNo_Randomizer_BILL_e);
    } else
#endif
    dComIfGs_setItem(SLOT_21, dItemNo_BILL_e);
}

void item_func_WOOD_STATUE() {
    /* dSv_event_flag_c::F_283 - Hyrule Field - Get wood carving */
    dComIfGs_onEventBit(dSv_event_flag_c::saveBitLabels[283]);
#if TARGET_PC
    if (randomizer_IsActive()) {
        if (dComIfGs_getItem(SLOT_21, true) != dItemNo_Randomizer_HORSE_FLUTE_e)
            dComIfGs_setItem(SLOT_21, dItemNo_Randomizer_WOOD_STATUE_e);
    } else
#endif
    dComIfGs_setItem(SLOT_21, dItemNo_WOOD_STATUE_e);
}

void item_func_IRIAS_PENDANT() {
#if TARGET_PC
    if (randomizer_IsActive()) {
        if (dComIfGs_getItem(SLOT_21, true) != dItemNo_Randomizer_HORSE_FLUTE_e)
            dComIfGs_setItem(SLOT_21, dItemNo_Randomizer_IRIAS_PENDANT_e);
    } else
#endif
    dComIfGs_setItem(SLOT_21, dItemNo_IRIAS_PENDANT_e);
}

void item_func_HORSE_FLUTE() {
#if TARGET_PC
    if (randomizer_IsActive()) {
        if (dComIfGs_getItem(SLOT_21, true) == dItemNo_Randomizer_NONE_e)
            dComIfGs_setItem(SLOT_21, dItemNo_Randomizer_HORSE_FLUTE_e);
    } else
#endif
    dComIfGs_setItem(SLOT_21, dItemNo_HORSE_FLUTE_e);
}

#if TARGET_PC
// Logged so a double-increment (two calls for what should be one server-sent key) is directly
// visible in the log, rather than only showing up as an unexplained gap between the server's
// authoritative send count and the in-game held count.
static void logged_incrementKeyNum(const char* dungeonName, u8 keyType) {
    u8 currentKeys = dComIfGs_getKeyNum(keyType);
    DuskLog.info("item_func: incrementing {} small key count {} -> {}", dungeonName, currentKeys, currentKeys + 1);
    dComIfGs_setKeyNum(keyType, currentKeys + 1);
}

void item_func_FOREST_SMALL_KEY() {
    logged_incrementKeyNum("Forest Temple", 0x10);
}

void item_func_MINES_SMALL_KEY() {
    logged_incrementKeyNum("Goron Mines", 0x11);
}

void item_func_LAKEBED_SMALL_KEY() {
    logged_incrementKeyNum("Lakebed Temple", 0x12);
}

void item_func_ARBITERS_SMALL_KEY() {
    logged_incrementKeyNum("Arbiters Grounds", 0x13);
}

void item_func_SNOWPEAK_SMALL_KEY() {
    logged_incrementKeyNum("Snowpeak Ruins", 0x14);
}

void item_func_TEMPLE_OF_TIME_SMALL_KEY() {
    logged_incrementKeyNum("Temple of Time", 0x15);
}

void item_func_CITY_SMALL_KEY() {
    logged_incrementKeyNum("City in the Sky", 0x16);
}

void item_func_PALACE_SMALL_KEY() {
    logged_incrementKeyNum("Palace of Twilight", 0x17);
}

void item_func_HYRULE_SMALL_KEY() {
    logged_incrementKeyNum("Hyrule Castle", 0x18);
}

void item_func_CAMP_SMALL_KEY() {
    logged_incrementKeyNum("Bulblin Camp", 0xA);
}

void item_func_LAKE_HYLIA_PORTAL() {
    dComIfGs_onStageSwitch(0x4, 0xA); // Unlock Lake Portal
}
#endif

void item_func_RAFRELS_MEMO() {
#if TARGET_PC
    if (randomizer_IsActive())
        dComIfGs_setItem(SLOT_7, dItemNo_Randomizer_RAFRELS_MEMO_e);
    else
#endif
    dComIfGs_setItem(SLOT_19, dItemNo_RAFRELS_MEMO_e);
}

void item_func_ASHS_SCRIBBLING() {
#if TARGET_PC
    if (randomizer_IsActive()) {
        if (!dComIfGs_isEventBit(0x3B80)) { // Got earring from Ralis
            dComIfGs_setItem(SLOT_19, dItemNo_Randomizer_ASHS_SCRIBBLING_e);
        }
        return;
    }
#endif
    dComIfGs_setItem(SLOT_19, dItemNo_ASHS_SCRIBBLING_e);
}

#if TARGET_PC

void item_func_FOREST_BOSS_KEY() {
    dComIfGs_onDungeonItemBossKey(0x10);
}

void item_func_LAKEBED_BOSS_KEY() {
    dComIfGs_onDungeonItemBossKey(0x12);
}

void item_func_ARBITERS_BOSS_KEY() {
    dComIfGs_onDungeonItemBossKey(0x13);
}

void item_func_TEMPLE_OF_TIME_BOSS_KEY() {
    dComIfGs_onDungeonItemBossKey(0x15);
}

void item_func_CITY_BOSS_KEY() {
    dComIfGs_onDungeonItemBossKey(0x16);
}

void item_func_PALACE_BOSS_KEY() {
    dComIfGs_onDungeonItemBossKey(0x17);
}

void item_func_HYRULE_BOSS_KEY() {
    dComIfGs_onDungeonItemBossKey(0x18);
}

void item_func_FOREST_COMPASS() {
    dComIfGs_onDungeonItemCompass(0x10);
}

void item_func_MINES_COMPASS() {
    dComIfGs_onDungeonItemCompass(0x11);
}

void item_func_LAKEBED_COMPASS() {
    dComIfGs_onDungeonItemCompass(0x12);
}
#endif

void item_func_CHUCHU_YELLOW2() {
    dComIfGs_setEmptyBottleItemIn(dItemNo_CHUCHU_YELLOW_e);
}

void item_func_OIL_BOTTLE3() {
    dComIfGs_setEmptyBottle(dItemNo_OIL_BOTTLE_e);
}

void item_func_SHOP_BEE_CHILD() {
    item_func_BEE_CHILD();
}

void item_func_CHUCHU_BLACK() {
    dComIfGs_setEmptyBottleItemIn(dItemNo_CHUCHU_BLACK_e);
}

void item_func_LIGHT_DROP() {}

void item_func_DROP_CONTAINER() {
    dComIfGs_onLightDropGetFlag(FARON_VESSEL);
}

void item_func_DROP_CONTAINER02() {
    dComIfGs_onLightDropGetFlag(ELDIN_VESSEL);
}

void item_func_DROP_CONTAINER03() {
    dComIfGs_onLightDropGetFlag(LANAYRU_VESSEL);
#if TARGET_PC
    if (randomizer_IsActive()) {
        dComIfGs_onEventBit(0x1E80); // Malo Mart Fundraising Starts
    }
#endif
}

void item_func_FILLED_CONTAINER() {}

void item_func_MIRROR_PIECE_2() {
#if TARGET_PC
    if (randomizer_IsActive()) {
        dComIfGs_onCollectMirror(1);
    }
#endif
}

void item_func_MIRROR_PIECE_3() {
#if TARGET_PC
    if (randomizer_IsActive()) {
        dComIfGs_onCollectMirror(2);
    }
#endif
}

void item_func_MIRROR_PIECE_4() {
#if TARGET_PC
    if (randomizer_IsActive()) {
        dComIfGs_onCollectMirror(3);
    }
#endif
}

#if TARGET_PC
void item_func_ARBITERS_COMPASS() {
    dComIfGs_onDungeonItemCompass(0x13);
}

void item_func_SNOWPEAK_COMPASS() {
    dComIfGs_onDungeonItemCompass(0x14);
}

void item_func_TEMPLE_OF_TIME_COMPASS() {
    dComIfGs_onDungeonItemCompass(0x15);
}

void item_func_CITY_COMPASS() {
    dComIfGs_onDungeonItemCompass(0x16);
}

void item_func_PALACE_COMPASS() {
    dComIfGs_onDungeonItemCompass(0x17);
}

void item_func_HYRULE_COMPASS() {
    dComIfGs_onDungeonItemCompass(0x18);
}

void item_func_MIRROR_CHAMBER_PORTAL() {
    dComIfGs_onStageSwitch(0xA, 0x28); // Unlock MC Portal
}

void item_func_SNOWPEAK_PORTAL() {
    dComIfGs_onStageSwitch(0x8, 0x15); // Unlock Snowpeak Portal
}
#endif

void item_func_SMELL_YELIA_POUCH() {}

void item_func_SMELL_PUMPKIN() {}

void item_func_SMELL_POH() {}

void item_func_SMELL_FISH() {}

void item_func_SMELL_CHILDREN() {}

void item_func_SMELL_MEDICINE() {}

#if TARGET_PC
void item_func_FOREST_MAP() {
    dComIfGs_onDungeonItemMap(0x10);
}

void item_func_MINES_MAP() {
    dComIfGs_onDungeonItemMap(0x11);
}

void item_func_LAKEBED_MAP() {
    dComIfGs_onDungeonItemMap(0x12);
}

void item_func_ARBITERS_MAP() {
    dComIfGs_onDungeonItemMap(0x13);
}

void item_func_SNOWPEAK_MAP() {
    dComIfGs_onDungeonItemMap(0x14);
}

void item_func_TEMPLE_OF_TIME_MAP() {
    dComIfGs_onDungeonItemMap(0x15);
}

void item_func_CITY_MAP() {
    dComIfGs_onDungeonItemMap(0x16);
}

void item_func_PALACE_MAP() {
    dComIfGs_onDungeonItemMap(0x17);
}

void item_func_HYRULE_MAP() {
    dComIfGs_onDungeonItemMap(0x18);
}

void item_func_SACRED_GROVE_PORTAL() {
    dComIfGs_onStageSwitch(0x7, 0x64); // Unlock Grove Portal
}
#endif

void item_func_M_BEETLE() {}

void item_func_F_BEETLE() {}

void item_func_M_BUTTERFLY() {}

void item_func_F_BUTTERFLY() {}

void item_func_M_STAG_BEETLE() {}

void item_func_F_STAG_BEETLE() {}

void item_func_M_GRASSHOPPER() {}

void item_func_F_GRASSHOPPER() {}

void item_func_M_NANAFUSHI() {}

void item_func_F_NANAFUSHI() {}

void item_func_M_DANGOMUSHI() {}

void item_func_F_DANGOMUSHI() {}

void item_func_M_MANTIS() {}

void item_func_F_MANTIS() {}

void item_func_M_LADYBUG() {}

void item_func_F_LADYBUG() {}

void item_func_M_SNAIL() {}

void item_func_F_SNAIL() {}

void item_func_M_DRAGONFLY() {}

void item_func_F_DRAGONFLY() {}

void item_func_M_ANT() {}

void item_func_F_ANT() {}

void item_func_M_MAYFLY() {}

void item_func_F_MAYFLY() {}

#if TARGET_PC

void item_func_FUSED_SHADOW_1() {
    dComIfGs_onCollectCrystal(0);
    /*
    Adding rando code until framework is implemented
    // Check if the requirement for the HC barrier is set to shadows, and if so, set the flag
    rando::gRandomizer->checkSetHCBarrierFlag(rando::HC_Fused_Shadows, 1);

    // Check if the requirement for the HC BK is set to shadows, and if so, set the flag
    rando::gRandomizer->checkSetHCBkFlag(rando::HC_BK_Fused_Shadows, 1);
    */
}

void item_func_FUSED_SHADOW_2() {
    if (randomizer_IsActive()) {
        dComIfGs_onCollectCrystal(1);
        /*
        Adding rando code until framework is implemented
        // Check if the requirement for the HC barrier is set to shadows, and if so, set the flag
        rando::gRandomizer->checkSetHCBarrierFlag(rando::HC_Fused_Shadows, 2);

        // Check if the requirement for the HC BK is set to shadows, and if so, set the flag
        rando::gRandomizer->checkSetHCBkFlag(rando::HC_BK_Fused_Shadows, 2);
        */
    }
}

void item_func_FUSED_SHADOW_3() {
    if (randomizer_IsActive()) {
        dComIfGs_onCollectCrystal(2);
        /*
        Adding rando code until framework is implemented
        // If the player has the palace requirement set to Fused Shadows.
        if (headerPtr->getPalaceRequirements() == rando::PalaceEntryRequirements::PoT_Fused_Shadows)
        {
            events::setSaveFileEventFlag(libtp::data::flags::FIXED_THE_MIRROR_OF_TWILIGHT);
        }

        // Check if the requirement for the HC barrier is set to shadows, and if so, set the flag
        rando::gRandomizer->checkSetHCBarrierFlag(rando::HC_Fused_Shadows, 3);

        // Check if the requirement for the HC BK is set to shadows, and if so, set the flag
        rando::gRandomizer->checkSetHCBkFlag(rando::HC_BK_Fused_Shadows, 3);
        */
    }
}

void item_func_MIRROR_PIECE_1() {
    if (randomizer_IsActive()) {
        dComIfGs_onCollectMirror(0);
    }
}

void item_func_ARCHIPELAGO_ITEM() {

}

#endif

void item_func_POU_SPIRIT() {
#if TARGET_PC
    if (randomizer_IsActive()) {
        dComIfGs_addPohSpiritNum();
    }
#endif
}

#if TARGET_PC
void item_func_ENDING_BLOW() {
    dComIfGs_onEventBit(0x2904);
}

void item_func_SHIELD_ATTACK() {
    dComIfGs_onEventBit(0x2908);
}

void item_func_BACK_SLICE() {
    dComIfGs_onEventBit(0x2902);
}

void item_func_HELM_SPLITTER() {
    dComIfGs_onEventBit(0x2901);
}

void item_func_MORTAL_DRAW() {
    dComIfGs_onEventBit(0x2A80);
}

void item_func_JUMP_STRIKE() {
    dComIfGs_onEventBit(0x2A40);
}

void item_func_GREAT_SPIN() {
    dComIfGs_onEventBit(0x2A20);
}

void item_func_ELDIN_BRIDGE_PORTAL() {
    dComIfGs_onStageSwitch(0x6, 0x63); // Unlock Eldin Bridge Portal
}
#endif

void item_func_ANCIENT_DOCUMENT() {
    dComIfGs_setItem(SLOT_22, dItemNo_ANCIENT_DOCUMENT_e);
}

void item_func_AIR_LETTER() {
#if TARGET_PC
    if (randomizer_IsActive()) {
        u8 letterCount = dComIfGs_getAncientDocumentNum();
        dComIfGs_setAncientDocumentNum(letterCount + 1);
    } else
#endif
    dComIfGs_setItem(SLOT_22, dItemNo_AIR_LETTER_e);
}

void item_func_ANCIENT_DOCUMENT2() {
#if TARGET_PC
    if (randomizer_IsActive()) {
        dComIfGs_onEventBit(0x3B08); // Repaired Cannon at Lake
        dComIfGs_setAncientDocumentNum(6);
    }
#endif
    dComIfGs_setItem(SLOT_22, dItemNo_ANCIENT_DOCUMENT2_e);
}

void item_func_LV7_DUNGEON_EXIT() {
    dComIfGs_setItem(SLOT_18, dItemNo_LV7_DUNGEON_EXIT_e);
}

void item_func_LINKS_SAVINGS() {
    dComIfGp_setItemRupeeCount(50);
}

void item_func_SMALL_KEY2() {
#if TARGET_PC
    if (randomizer_IsActive())
        dComIfGs_onStageSwitch(0x2, 0x14); // Unlock North Faron Gate
    else
#endif
    dComIfGp_setItemKeyNumCount(1);
}

void item_func_POU_FIRE1() {}

void item_func_POU_FIRE2() {}

void item_func_POU_FIRE3() {}

void item_func_POU_FIRE4() {}

void item_func_BOSSRIDER_KEY() {
#if TARGET_PC
    if (randomizer_IsActive()) {
        dComIfGs_onStageSwitch(0x3, 0x69); // Started Escort
        dComIfGs_onStageSwitch(0x3, 0x65); // Followed Rutella to Graveyard
        dComIfGs_onEventBit(0x840); // Started Zora Escort
        dComIfGs_onEventBit(0x810); // Completed Zora Escort
    }
#endif
}

void item_func_TOMATO_PUREE() {
#if TARGET_PC
    if (randomizer_IsActive()) {
        dComIfGs_onEventBit(0x480); // Told Yeta about Pumpkin
        dComIfGs_onEventBit(0x2); // Yeto put Pumpkin in Soup
        dComIfGs_onEventBit(0x1440); // SPR Lobby Door Unlocked
        dComIfGs_onStageSwitch(0x14, 0x12); // Unlock North Door
    }
#endif
}

void item_func_TASTE() {
#if TARGET_PC
    if (randomizer_IsActive()) {
        dComIfGs_onEventBit(0x120); // Told Yeta about Cheese
        dComIfGs_onEventBit(0x1); // Yeto put Pumpkin in Soup
        dComIfGs_onEventBit(0x1420); // SPR Lobby west Door Unlocked
        dComIfGs_onStageSwitch(0x14, 0x13); // Unlock West Door
    }
#endif
}

void item_func_LV5_BOSS_KEY() {
#if TARGET_PC
    if (randomizer_IsActive())
        dComIfGs_onDungeonItemBossKey(0x14);
    else
#endif
    dComIfGs_onDungeonItemBossKey();
}

void item_func_SURFBOARD() {}

void item_func_KANTERA2() {}

void item_func_L2_KEY_PIECES1() {}

void item_func_L2_KEY_PIECES2() {}

void item_func_L2_KEY_PIECES3() {}

void item_func_KEY_OF_CARAVAN() {}

void item_func_LV2_BOSS_KEY() {
#if TARGET_PC
    if (randomizer_IsActive()) {
        dComIfGs_onDungeonItemBossKey(0x11);
        execItemGet(dItemNo_Randomizer_L2_KEY_PIECES3_e);
    } else
#endif
    dComIfGs_onDungeonItemBossKey();
}

void item_func_KEY_OF_FILONE() {
#if TARGET_PC
    if (randomizer_IsActive())
        dComIfGs_onStageSwitch(0x2, 0xC); // Unlock Coro Gate
    else
#endif
    dComIfGp_setItemKeyNumCount(1);
}

void item_func_noentry() {}

int item_getcheck_func_noentry() {
    return -1;
}

int item_getcheck_func_HEART() {
    return -1;
}

int item_getcheck_func_GREEN_RUPEE() {
    return dComIfGs_isItemFirstBit(dItemNo_GREEN_RUPEE_e);
}

int item_getcheck_func_BLUE_RUPEE() {
    return dComIfGs_isItemFirstBit(dItemNo_BLUE_RUPEE_e);
}

int item_getcheck_func_YELLOW_RUPEE() {
    return dComIfGs_isItemFirstBit(dItemNo_YELLOW_RUPEE_e);
}

int item_getcheck_func_RED_RUPEE() {
    return dComIfGs_isItemFirstBit(dItemNo_RED_RUPEE_e);
}

int item_getcheck_func_PURPLE_RUPEE() {
    return dComIfGs_isItemFirstBit(dItemNo_PURPLE_RUPEE_e);
}

int item_getcheck_func_ORANGE_RUPEE() {
    return dComIfGs_isItemFirstBit(dItemNo_ORANGE_RUPEE_e);
}

int item_getcheck_func_SILVER_RUPEE() {
    return dComIfGs_isItemFirstBit(dItemNo_SILVER_RUPEE_e);
}

int item_getcheck_func_S_MAGIC() {
    return -1;
}

int item_getcheck_func_L_MAGIC() {
    return -1;
}

int item_getcheck_func_BOMB_5() {
    return dComIfGs_isItemFirstBit(dItemNo_BOMB_5_e);
}

int item_getcheck_func_BOMB_10() {
    return dComIfGs_isItemFirstBit(dItemNo_BOMB_10_e);
}

int item_getcheck_func_BOMB_20() {
    return dComIfGs_isItemFirstBit(dItemNo_BOMB_20_e);
}

int item_getcheck_func_BOMB_30() {
    return dComIfGs_isItemFirstBit(dItemNo_BOMB_30_e);
}

int item_getcheck_func_ARROW_10() {
    return dComIfGs_isItemFirstBit(dItemNo_ARROW_10_e);
}

int item_getcheck_func_ARROW_20() {
    return dComIfGs_isItemFirstBit(dItemNo_ARROW_20_e);
}

int item_getcheck_func_ARROW_30() {
    return dComIfGs_isItemFirstBit(dItemNo_ARROW_30_e);
}

int item_getcheck_func_ARROW_1() {
    return dComIfGs_isItemFirstBit(dItemNo_ARROW_1_e);
}

int item_getcheck_func_PACHINKO_SHOT() {
    return dComIfGs_isItemFirstBit(dItemNo_PACHINKO_SHOT_e);
}

#if TARGET_PC
int item_getcheck_func_ORDON_PORTAL() {
    return dComIfGs_isStageSwitch(0x0, 0x34); // Unlock Ordon Portal
}

int item_getcheck_func_SOUTH_FARON_PORTAL() {
    return dComIfGs_isStageSwitch(0x2, 0x47); // Unlock S Faron Portal
}
#endif

int item_getcheck_func_WATER_BOMB_5() {
    return -1;
}

int item_getcheck_func_WATER_BOMB_10() {
    return -1;
}

int item_getcheck_func_WATER_BOMB_20() {
    return -1;
}

int item_getcheck_func_WATER_BOMB_30() {
    return -1;
}

int item_getcheck_func_BOMB_INSECT_5() {
    return -1;
}

int item_getcheck_func_BOMB_INSECT_10() {
    return dComIfGs_isItemFirstBit(dItemNo_BOMB_INSECT_10_e);
}

int item_getcheck_func_BOMB_INSECT_20() {
    return dComIfGs_isItemFirstBit(dItemNo_BOMB_INSECT_20_e);
}

int item_getcheck_func_BOMB_INSECT_30() {
    return dComIfGs_isItemFirstBit(dItemNo_BOMB_INSECT_30_e);
}

int item_getcheck_func_RECOVER_FAILY() {
    return -1;
}

int item_getcheck_func_TRIPLE_HEART() {
    return -1;
}

int item_getcheck_func_SMALL_KEY() {
    return -1;
}

int item_getcheck_func_KAKERA_HEART() {
    return -1;
}

int item_getcheck_func_UTUWA_HEART() {
    return -1;
}

int item_getcheck_func_MAP() {
    return dComIfGs_isDungeonItemMap();
}

int item_getcheck_func_COMPUS() {
    return -1;
}

int item_getcheck_func_DUNGEON_EXIT() {
    return dComIfGs_getItem(SLOT_18, true) == dItemNo_DUNGEON_EXIT_e ? TRUE : FALSE;
}

int item_getcheck_func_BOSS_KEY() {
    return dComIfGs_isDungeonItemBossKey();
}

int item_getcheck_func_DUNGEON_BACK() {
    return dComIfGs_getItem(SLOT_18, true) == dItemNo_DUNGEON_BACK_e ? TRUE : FALSE;
}

int item_getcheck_func_SWORD() {
    return dComIfGs_isCollectSword(COLLECT_ORDON_SWORD);
}

int item_getcheck_func_MASTER_SWORD() {
    return dComIfGs_isCollectSword(COLLECT_MASTER_SWORD);
}

int item_getcheck_func_WOOD_SHIELD() {
    return dComIfGs_isItemFirstBit(dItemNo_WOOD_SHIELD_e);
}

int item_getcheck_func_SHIELD() {
    return dComIfGs_isItemFirstBit(dItemNo_SHIELD_e);
}

int item_getcheck_func_HYLIA_SHIELD() {
    return dComIfGs_isItemFirstBit(dItemNo_HYLIA_SHIELD_e);
}

int item_getcheck_func_TKS_LETTER() {
    return dComIfGs_getItem(SLOT_18, true) == dItemNo_TKS_LETTER_e ? TRUE : FALSE;
}

int item_getcheck_func_WEAR_CASUAL() {
    return dComIfGs_isItemFirstBit(dItemNo_WEAR_CASUAL_e);
}

int item_getcheck_func_WEAR_KOKIRI() {
    return dComIfGs_isCollectClothing(KOKIRI_CLOTHES_FLAG);
}

int item_getcheck_func_ARMOR() {
    return dComIfGs_isItemFirstBit(dItemNo_ARMOR_e);
}

int item_getcheck_func_WEAR_ZORA() {
    return dComIfGs_isItemFirstBit(dItemNo_WEAR_ZORA_e);
}

int item_getcheck_func_MAGIC_LV1() {
#if TARGET_PC
    if (randomizer_IsActive())
        return dComIfGs_isItemFirstBit(dItemNo_Randomizer_MAGIC_LV1_e);
#endif
    return -1;
}

int item_getcheck_func_DUNGEON_EXIT_2() {
    return dComIfGs_getItem(SLOT_18, true) == dItemNo_DUNGEON_EXIT_e ? TRUE : FALSE;
}

int item_getcheck_func_WALLET_LV1() {
    return -1;
}

int item_getcheck_func_WALLET_LV2() {
#if TARGET_PC
    if (randomizer_IsActive())
        return dComIfGs_isItemFirstBit(dItemNo_Randomizer_WALLET_LV2_e);
#endif
    return -1;
}

int item_getcheck_func_WALLET_LV3() {
#if TARGET_PC
    if (randomizer_IsActive())
        return dComIfGs_isItemFirstBit(dItemNo_Randomizer_WALLET_LV3_e);
#endif
    return -1;
}

#if TARGET_PC
int item_getcheck_func_UPPER_ZORAS_RIVER_PORTAL() {
    return dComIfGs_isStageSwitch(0x4, 0x15); // Unlock UZR Portal
}

int item_getcheck_func_CASTLE_TOWN_PORTAL() {
    return dComIfGs_isStageSwitch(0x6, 0x3); // Unlock Castle Town Portal
}

int item_getcheck_func_GERUDO_DESERT_PORTAL() {
    return dComIfGs_isStageSwitch(0xA, 0x15); // Unlock Desert Portal
}

int item_getcheck_func_NORTH_FARON_PORTAL() {
    return dComIfGs_isStageSwitch(0x2, 0x2); // Unlock N Faron Portal
}
#endif

int item_getcheck_func_ZORAS_JEWEL() {
    return dComIfGs_getItem(SLOT_20, true) == dItemNo_ZORAS_JEWEL_e ? TRUE : FALSE;
}

int item_getcheck_func_HAWK_EYE() {
    return dComIfGs_isItemFirstBit(dItemNo_HAWK_EYE_e);
}

int item_getcheck_func_WOOD_STICK() {
    return dComIfGs_isItemFirstBit(dItemNo_WOOD_STICK_e);
}

int item_getcheck_func_BOOMERANG() {
    return dComIfGs_getItem(SLOT_0, true) == dItemNo_BOOMERANG_e ? TRUE : FALSE;
}

int item_getcheck_func_SPINNER() {
    return dComIfGs_getItem(SLOT_2, true) == dItemNo_SPINNER_e ? TRUE : FALSE;
}

int item_getcheck_func_IRONBALL() {
    return dComIfGs_getItem(SLOT_6, true) == dItemNo_IRONBALL_e ? TRUE : FALSE;
}

int item_getcheck_func_BOW() {
    return dComIfGs_getItem(SLOT_4, false) == dItemNo_BOW_e ? TRUE : FALSE;
}

int item_getcheck_func_HOOKSHOT() {
    return dComIfGs_getItem(SLOT_9, true) == dItemNo_HOOKSHOT_e ? TRUE : FALSE;
}

int item_getcheck_func_HVY_BOOTS() {
    return dComIfGs_getItem(SLOT_3, true) == dItemNo_HVY_BOOTS_e ? TRUE : FALSE;
}

int item_getcheck_func_COPY_ROD() {
    return dComIfGs_getItem(SLOT_8, true) == dItemNo_COPY_ROD_e ? TRUE : FALSE;
}

int item_getcheck_func_W_HOOKSHOT() {
    return dComIfGs_getItem(SLOT_10, true) == dItemNo_W_HOOKSHOT_e ? TRUE : FALSE;
}

int item_getcheck_func_KANTERA() {
    return dComIfGs_getItem(SLOT_1, true) == dItemNo_KANTERA_e ? TRUE : FALSE;
}

int item_getcheck_func_LIGHT_SWORD() {
    return dComIfGs_isCollectSword(COLLECT_LIGHT_SWORD);
}

int item_getcheck_func_FISHING_ROD_1() {
    return (dComIfGs_getItem(SLOT_20, true) == dItemNo_FISHING_ROD_1_e ||
            dComIfGs_getItem(SLOT_20, true) == dItemNo_BEE_ROD_e ||
            dComIfGs_getItem(SLOT_20, true) == dItemNo_WORM_ROD_e ||
            dComIfGs_getItem(SLOT_20, true) == dItemNo_JEWEL_ROD_e ||
            dComIfGs_getItem(SLOT_20, true) == dItemNo_JEWEL_BEE_ROD_e ||
            dComIfGs_getItem(SLOT_20, true) == dItemNo_JEWEL_WORM_ROD_e) ?
               TRUE :
               FALSE;
}

int item_getcheck_func_PACHINKO() {
    return dComIfGs_getItem(SLOT_23, true) == dItemNo_PACHINKO_e ? TRUE : FALSE;
}

int item_getcheck_func_COPY_ROD_2() {
    return -1;
}

#if TARGET_PC
int item_getcheck_func_KAKARIKO_GORGE_PORTAL() {
    return dComIfGs_isStageSwitch(0x6, 0x15); // Unlock Gorge Portal
}

int item_getcheck_func_KAKARIKO_VILLAGE_PORTAL() {
    return dComIfGs_isStageSwitch(0x3, 0x1F); // Unlock Kak Portal
}
#endif

int item_getcheck_func_BOMB_BAG_LV2() {
    return dComIfGs_isItemFirstBit(dItemNo_BOMB_BAG_LV2_e);
}

int item_getcheck_func_BOMB_BAG_LV1() {
    return dComIfGs_isItemFirstBit(dItemNo_BOMB_BAG_LV1_e);
}

int item_getcheck_func_BOMB_IN_BAG() {
    return dComIfGs_isItemFirstBit(dItemNo_BOMB_IN_BAG_e);
}

#if TARGET_PC
int item_getcheck_func_DEATH_MOUNTAIN_PORTAL() {
    return dComIfGs_isStageSwitch(0x3, 0x15); // Unlock DM Portal
}
#endif

int item_getcheck_func_LIGHT_ARROW() {
    return dComIfGs_isItemFirstBit(dItemNo_LIGHT_ARROW_e);
}

int item_getcheck_func_ARROW_LV1() {
    return (dComIfGs_getItem(SLOT_4, false) == dItemNo_BOW_e && dComIfGs_getArrowMax() >= 30) ? TRUE : FALSE;
}

int item_getcheck_func_ARROW_LV2() {
    return (dComIfGs_getItem(SLOT_4, false) == dItemNo_BOW_e && dComIfGs_getArrowMax() >= 60) ? TRUE : FALSE;
}

int item_getcheck_func_ARROW_LV3() {
    return (dComIfGs_getItem(SLOT_4, false) == dItemNo_BOW_e && dComIfGs_getArrowMax() >= 100) ? TRUE : FALSE;
}

#if TARGET_PC
int item_getcheck_func_ZORAS_DOMAIN_PORTAL() {
    return dComIfGs_isStageSwitch(0x4, 0x2); // Unlock ZD Portal
}
#endif

int item_getcheck_func_LURE_ROD() {
    return -1;
}

int item_getcheck_func_BOMB_ARROW() {
    return -1;
}

int item_getcheck_func_HAWK_ARROW() {
    return -1;
}

int item_getcheck_func_BEE_ROD() {
    return item_getcheck_func_FISHING_ROD_1();
}

int item_getcheck_func_JEWEL_ROD() {
    return item_getcheck_func_FISHING_ROD_1();
}

int item_getcheck_func_WORM_ROD() {
    return item_getcheck_func_FISHING_ROD_1();
}

int item_getcheck_func_JEWEL_BEE_ROD() {
    return item_getcheck_func_FISHING_ROD_1();
}

int item_getcheck_func_JEWEL_WORM_ROD() {
    return item_getcheck_func_FISHING_ROD_1();
}

int item_getcheck_func_EMPTY_BOTTLE() {
    return dComIfGs_checkBottle(dItemNo_EMPTY_BOTTLE_e);
}

int item_getcheck_func_RED_BOTTLE() {
    return dComIfGs_checkBottle(dItemNo_RED_BOTTLE_e);
}

int item_getcheck_func_GREEN_BOTTLE() {
    return dComIfGs_checkBottle(dItemNo_GREEN_BOTTLE_e);
}

int item_getcheck_func_BLUE_BOTTLE() {
    return dComIfGs_checkBottle(dItemNo_BLUE_BOTTLE_e);
}

int item_getcheck_func_MILK_BOTTLE() {
    return dComIfGs_checkBottle(dItemNo_MILK_BOTTLE_e);
}

int item_getcheck_func_HALF_MILK_BOTTLE() {
    return dComIfGs_checkBottle(dItemNo_HALF_MILK_BOTTLE_e);
}

int item_getcheck_func_OIL_BOTTLE() {
    return dComIfGs_checkBottle(dItemNo_OIL_BOTTLE_e);
}

int item_getcheck_func_WATER_BOTTLE() {
    return dComIfGs_checkBottle(dItemNo_WATER_BOTTLE_e);
}

int item_getcheck_func_OIL_BOTTLE2() {
    return dComIfGs_checkBottle(dItemNo_OIL_BOTTLE_2_e);
}

int item_getcheck_func_RED_BOTTLE2() {
    return dComIfGs_checkBottle(dItemNo_RED_BOTTLE_2_e);
}

int item_getcheck_func_UGLY_SOUP() {
    return dComIfGs_checkBottle(dItemNo_UGLY_SOUP_e);
}

int item_getcheck_func_HOT_SPRING() {
    return dComIfGs_checkBottle(dItemNo_HOT_SPRING_e);
}

int item_getcheck_func_FAIRY_BOTTLE() {
    return dComIfGs_checkBottle(dItemNo_FAIRY_e);
}

int item_getcheck_func_HOT_SPRING2() {
    return dComIfGs_checkBottle(dItemNo_HOT_SPRING_e);
}

int item_getcheck_func_OIL2() {
    return dComIfGs_checkBottle(dItemNo_OIL2_e);
}

int item_getcheck_func_OIL() {
    return dComIfGs_checkBottle(dItemNo_OIL_e);
}

int item_getcheck_func_NORMAL_BOMB() {
    return dComIfGs_isItemFirstBit(dItemNo_NORMAL_BOMB_e);
}

int item_getcheck_func_WATER_BOMB() {
    return dComIfGs_isItemFirstBit(dItemNo_WATER_BOMB_e);
}

int item_getcheck_func_POKE_BOMB() {
    return dComIfGs_isItemFirstBit(dItemNo_POKE_BOMB_e);
}

int item_getcheck_func_FAIRY_DROP() {
    return dComIfGs_checkBottle(dItemNo_FAIRY_DROP_e);
}

int item_getcheck_func_WORM() {
    return dComIfGs_checkBottle(dItemNo_WORM_e);
}

int item_getcheck_func_DROP_BOTTLE() {
    return dComIfGs_isItemFirstBit(dItemNo_DROP_BOTTLE_e);
}

int item_getcheck_func_BEE_CHILD() {
    return -1;
}

int item_getcheck_func_CHUCHU_RARE() {
    return dComIfGs_checkBottle(dItemNo_CHUCHU_RARE_e);
}

int item_getcheck_func_CHUCHU_RED() {
    return dComIfGs_checkBottle(dItemNo_CHUCHU_RED_e);
}

int item_getcheck_func_CHUCHU_BLUE() {
    return dComIfGs_checkBottle(dItemNo_CHUCHU_BLUE_e);
}

int item_getcheck_func_CHUCHU_GREEN() {
    return dComIfGs_checkBottle(dItemNo_CHUCHU_GREEN_e);
}

int item_getcheck_func_CHUCHU_YELLOW() {
    return dComIfGs_checkBottle(dItemNo_CHUCHU_YELLOW_e);
}

int item_getcheck_func_CHUCHU_PURPLE() {
    return dComIfGs_checkBottle(dItemNo_CHUCHU_PURPLE_e);
}

int item_getcheck_func_LV1_SOUP() {
    return dComIfGs_isItemFirstBit(dItemNo_LV1_SOUP_e);
}

int item_getcheck_func_LV2_SOUP() {
    return dComIfGs_isItemFirstBit(dItemNo_LV2_SOUP_e);
}

int item_getcheck_func_LV3_SOUP() {
    return dComIfGs_isItemFirstBit(dItemNo_LV3_SOUP_e);
}

int item_getcheck_func_LETTER() {
#if TARGET_PC
    if (randomizer_IsActive())
        return dComIfGs_isItemFirstBit(dItemNo_Randomizer_LETTER_e);
#endif
    return dComIfGs_getItem(SLOT_21, true) == dItemNo_LETTER_e ? TRUE : FALSE;
}

int item_getcheck_func_BILL() {
#if TARGET_PC
    if (randomizer_IsActive())
        return dComIfGs_isItemFirstBit(dItemNo_Randomizer_BILL_e);
#endif
    return dComIfGs_getItem(SLOT_21, true) == dItemNo_BILL_e ? TRUE : FALSE;
}

int item_getcheck_func_WOOD_STATUE() {
#if TARGET_PC
    if (randomizer_IsActive())
        return dComIfGs_isItemFirstBit(dItemNo_Randomizer_WOOD_STATUE_e);
#endif
    return dComIfGs_getItem(SLOT_21, true) == dItemNo_WOOD_STATUE_e ? TRUE : FALSE;
}

int item_getcheck_func_IRIAS_PENDANT() {
#if TARGET_PC
    if (randomizer_IsActive())
        return dComIfGs_isItemFirstBit(dItemNo_Randomizer_IRIAS_PENDANT_e);
#endif
    return dComIfGs_getItem(SLOT_21, true) == dItemNo_IRIAS_PENDANT_e ? TRUE : FALSE;
}

int item_getcheck_func_HORSE_FLUTE() {
#if TARGET_PC
    if (randomizer_IsActive())
        return dComIfGs_isItemFirstBit(dItemNo_Randomizer_HORSE_FLUTE_e);
#endif
    return dComIfGs_getItem(SLOT_22, true) == dItemNo_HORSE_FLUTE_e ? TRUE : FALSE;
}

#if TARGET_PC
int item_getcheck_func_CAMP_SMALL_KEY() {
    return dComIfGs_isItemFirstBit(dItemNo_Randomizer_CAMP_SMALL_KEY_e);
}

int item_getcheck_func_LAKE_HYLIA_PORTAL() {
    return dComIfGs_isStageSwitch(0x4, 0xA); // Unlock Lake Portal
}
#endif

int item_getcheck_func_RAFRELS_MEMO() {
    return dComIfGs_getItem(SLOT_19, true) == dItemNo_RAFRELS_MEMO_e ? TRUE : FALSE;
}

int item_getcheck_func_ASHS_SCRIBBLING() {
    return dComIfGs_getItem(SLOT_19, true) == dItemNo_ASHS_SCRIBBLING_e ? TRUE : FALSE;
}

int item_getcheck_func_CHUCHU_YELLOW2() {
    return dComIfGs_checkBottle(dItemNo_CHUCHU_YELLOW2_e);
}

int item_getcheck_func_OIL_BOTTLE3() {
    return -1;
}

int item_getcheck_func_SHOP_BEE_CHILD() {
    return -1;
}

int item_getcheck_func_CHUCHU_BLACK() {
    return dComIfGs_checkBottle(dItemNo_CHUCHU_BLACK_e);
}

int item_getcheck_func_LIGHT_DROP() {
    return dComIfGs_isItemFirstBit(dItemNo_LIGHT_DROP_e);
}

int item_getcheck_func_DROP_CONTAINER() {
    return dComIfGs_isLightDropGetFlag(FARON_VESSEL);
}

int item_getcheck_func_DROP_CONTAINER02() {
    return dComIfGs_isLightDropGetFlag(ELDIN_VESSEL);
}

int item_getcheck_func_DROP_CONTAINER03() {
    return dComIfGs_isLightDropGetFlag(LANAYRU_VESSEL);
}

int item_getcheck_func_FILLED_CONTAINER() {
    return -1;
}

int item_getcheck_func_MIRROR_PIECE_2() {
#if TARGET_PC
    if (randomizer_IsActive())
        return dComIfGs_isItemFirstBit(dItemNo_Randomizer_MIRROR_PIECE_2_e);
#endif
    return -1;
}

int item_getcheck_func_MIRROR_PIECE_3() {
#if TARGET_PC
    if (randomizer_IsActive())
        return dComIfGs_isItemFirstBit(dItemNo_Randomizer_MIRROR_PIECE_3_e);
#endif
    return -1;
}

int item_getcheck_func_MIRROR_PIECE_4() {
#if TARGET_PC
    if (randomizer_IsActive())
        return dComIfGs_isItemFirstBit(dItemNo_Randomizer_MIRROR_PIECE_4_e);
#endif
    return -1;
}

#if TARGET_PC
int item_getcheck_func_MIRROR_CHAMBER_PORTAL() {
    return dComIfGs_isStageSwitch(0xA, 0x28); // Unlock MC Portal
}

int item_getcheck_func_SNOWPEAK_PORTAL() {
    return dComIfGs_isStageSwitch(0x8, 0x15); // Unlock Snowpeak Portal
}
#endif

int item_getcheck_func_SMELL_YELIA_POUCH() {
    return dComIfGs_getCollectSmell() == dItemNo_SMELL_YELIA_POUCH_e ? TRUE : FALSE;
}

int item_getcheck_func_SMELL_PUMPKIN() {
    return -1;
}

int item_getcheck_func_SMELL_POH() {
    return dComIfGs_getCollectSmell() == dItemNo_SMELL_POH_e ? TRUE : FALSE;
}

int item_getcheck_func_SMELL_FISH() {
    return dComIfGs_getCollectSmell() == dItemNo_SMELL_FISH_e ? TRUE : FALSE;
}

int item_getcheck_func_SMELL_CHILDREN() {
    return dComIfGs_getCollectSmell() == dItemNo_SMELL_CHILDREN_e ? TRUE : FALSE;
}

int item_getcheck_func_SMELL_MEDICINE() {
    return dComIfGs_getCollectSmell() == dItemNo_SMELL_MEDICINE_e ? TRUE : FALSE;
}

#if TARGET_PC
int item_getcheck_func_SACRED_GROVE_PORTAL() {
    return dComIfGs_isStageSwitch(0x7, 0x64); // Unlock Grove Portal
}
#endif

int item_getcheck_func_M_BEETLE() {
    return dComIfGs_isItemFirstBit(dItemNo_M_BEETLE_e);
}

int item_getcheck_func_F_BEETLE() {
    return dComIfGs_isItemFirstBit(dItemNo_F_BEETLE_e);
}

int item_getcheck_func_M_BUTTERFLY() {
    return dComIfGs_isItemFirstBit(dItemNo_M_BUTTERFLY_e);
}

int item_getcheck_func_F_BUTTERFLY() {
    return dComIfGs_isItemFirstBit(dItemNo_F_BUTTERFLY_e);
}

int item_getcheck_func_M_STAG_BEETLE() {
    return dComIfGs_isItemFirstBit(dItemNo_M_STAG_BEETLE_e);
}

int item_getcheck_func_F_STAG_BEETLE() {
    return dComIfGs_isItemFirstBit(dItemNo_F_STAG_BEETLE_e);
}

int item_getcheck_func_M_GRASSHOPPER() {
    return dComIfGs_isItemFirstBit(dItemNo_M_GRASSHOPPER_e);
}

int item_getcheck_func_F_GRASSHOPPER() {
    return dComIfGs_isItemFirstBit(dItemNo_F_GRASSHOPPER_e);
}

int item_getcheck_func_M_NANAFUSHI() {
    return dComIfGs_isItemFirstBit(dItemNo_M_NANAFUSHI_e);
}

int item_getcheck_func_F_NANAFUSHI() {
    return dComIfGs_isItemFirstBit(dItemNo_F_NANAFUSHI_e);
}

int item_getcheck_func_M_DANGOMUSHI() {
    return dComIfGs_isItemFirstBit(dItemNo_M_DANGOMUSHI_e);
}

int item_getcheck_func_F_DANGOMUSHI() {
    return dComIfGs_isItemFirstBit(dItemNo_F_DANGOMUSHI_e);
}

int item_getcheck_func_M_MANTIS() {
    return dComIfGs_isItemFirstBit(dItemNo_M_MANTIS_e);
}

int item_getcheck_func_F_MANTIS() {
    return dComIfGs_isItemFirstBit(dItemNo_F_MANTIS_e);
}

int item_getcheck_func_M_LADYBUG() {
    return dComIfGs_isItemFirstBit(dItemNo_M_LADYBUG_e);
}

int item_getcheck_func_F_LADYBUG() {
    return dComIfGs_isItemFirstBit(dItemNo_F_LADYBUG_e);
}

int item_getcheck_func_M_SNAIL() {
    return dComIfGs_isItemFirstBit(dItemNo_M_SNAIL_e);
}

int item_getcheck_func_F_SNAIL() {
    return dComIfGs_isItemFirstBit(dItemNo_F_SNAIL_e);
}

int item_getcheck_func_M_DRAGONFLY() {
    return dComIfGs_isItemFirstBit(dItemNo_M_DRAGONFLY_e);
}

int item_getcheck_func_F_DRAGONFLY() {
    return dComIfGs_isItemFirstBit(dItemNo_F_DRAGONFLY_e);
}

int item_getcheck_func_M_ANT() {
    return dComIfGs_isItemFirstBit(dItemNo_M_ANT_e);
}

int item_getcheck_func_F_ANT() {
    return dComIfGs_isItemFirstBit(dItemNo_F_ANT_e);
}

int item_getcheck_func_M_MAYFLY() {
    return dComIfGs_isItemFirstBit(dItemNo_M_MAYFLY_e);
}

int item_getcheck_func_F_MAYFLY() {
    return dComIfGs_isItemFirstBit(dItemNo_F_MAYFLY_e);
}

#if TARGET_PC
int item_getcheck_func_FUSED_SHADOW_1() {
    return dComIfGs_isItemFirstBit(dItemNo_Randomizer_FUSED_SHADOW_1_e);
}

int item_getcheck_func_FUSED_SHADOW_2() {
    return dComIfGs_isItemFirstBit(dItemNo_Randomizer_FUSED_SHADOW_2_e);
}

int item_getcheck_func_FUSED_SHADOW_3() {
    return dComIfGs_isItemFirstBit(dItemNo_Randomizer_FUSED_SHADOW_3_e);
}

int item_getcheck_func_MIRROR_PIECE_1() {
    return dComIfGs_isItemFirstBit(dItemNo_Randomizer_MIRROR_PIECE_1_e);
}
#endif

int item_getcheck_func_POU_SPIRIT() {
    return dComIfGs_getPohSpiritNum();
}

#if TARGET_PC
int item_getcheck_func_ENDING_BLOW() {
    return dComIfGs_isItemFirstBit(dItemNo_Randomizer_ENDING_BLOW_e);
}

int item_getcheck_func_SHIELD_ATTACK() {
    return dComIfGs_isItemFirstBit(dItemNo_Randomizer_SHIELD_ATTACK_e);
}

int item_getcheck_func_BACK_SLICE() {
    return dComIfGs_isItemFirstBit(dItemNo_Randomizer_BACK_SLICE_e);
}

int item_getcheck_func_HELM_SPLITTER() {
    return dComIfGs_isItemFirstBit(dItemNo_Randomizer_HELM_SPLITTER_e);
}

int item_getcheck_func_MORTAL_DRAW() {
    return dComIfGs_isItemFirstBit(dItemNo_Randomizer_MORTAL_DRAW_e);
}

int item_getcheck_func_JUMP_STRIKE() {
    return dComIfGs_isItemFirstBit(dItemNo_Randomizer_JUMP_STRIKE_e);
}

int item_getcheck_func_GREAT_SPIN() {
    return dComIfGs_isItemFirstBit(dItemNo_Randomizer_GREAT_SPIN_e);
}

int item_getcheck_func_ELDIN_BRIDGE_PORTAL() {
    return dComIfGs_isStageSwitch(0x6, 0x63); // Unlock Eldin Bridge Portal
}
#endif

int item_getcheck_func_ANCIENT_DOCUMENT() {
    return dComIfGs_getItem(SLOT_22, true) == dItemNo_ANCIENT_DOCUMENT_e ? TRUE : FALSE;
}

int item_getcheck_func_AIR_LETTER() {
    return dComIfGs_getItem(SLOT_22, true) == dItemNo_AIR_LETTER_e ? TRUE : FALSE;
}

int item_getcheck_func_ANCIENT_DOCUMENT2() {
    return dComIfGs_getItem(SLOT_22, true) == dItemNo_ANCIENT_DOCUMENT2_e ? TRUE : FALSE;
}

int item_getcheck_func_LV7_DUNGEON_EXIT() {
    return dComIfGs_getItem(SLOT_18, true) == dItemNo_LV7_DUNGEON_EXIT_e ? TRUE : FALSE;
}

int item_getcheck_func_LINKS_SAVINGS() {
    return -1;
}

int item_getcheck_func_SMALL_KEY2() {
    return -1;
}

int item_getcheck_func_POU_FIRE1() {
    return -1;
}

int item_getcheck_func_POU_FIRE2() {
    return -1;
}

int item_getcheck_func_POU_FIRE3() {
    return -1;
}

int item_getcheck_func_POU_FIRE4() {
    return -1;
}

int item_getcheck_func_BOSSRIDER_KEY() {
#if TARGET_PC
    if (randomizer_IsActive())
        return dComIfGs_isItemFirstBit(dItemNo_Randomizer_BOSSRIDER_KEY_e);
#endif
    return -1;
}

int item_getcheck_func_TOMATO_PUREE() {
    return dComIfGs_isItemFirstBit(dItemNo_TOMATO_PUREE_e);
}

int item_getcheck_func_TASTE() {
    return dComIfGs_isItemFirstBit(dItemNo_TASTE_e);
}

int item_getcheck_func_LV5_BOSS_KEY() {
    return dComIfGs_isDungeonItemBossKey();
}

int item_getcheck_func_SURFBOARD() {
    return -1;
}

int item_getcheck_func_KANTERA2() {
    return -1;
}

int item_getcheck_func_L2_KEY_PIECES1() {
    return dComIfGs_isItemFirstBit(dItemNo_L2_KEY_PIECES1_e);
}

int item_getcheck_func_L2_KEY_PIECES2() {
    return dComIfGs_isItemFirstBit(dItemNo_L2_KEY_PIECES2_e);
}

int item_getcheck_func_L2_KEY_PIECES3() {
    return dComIfGs_isItemFirstBit(dItemNo_L2_KEY_PIECES3_e);
}

int item_getcheck_func_KEY_OF_CARAVAN() {
    return dComIfGs_isItemFirstBit(dItemNo_KEY_OF_CARAVAN_e);
}

int item_getcheck_func_LV2_BOSS_KEY() {
    return dComIfGs_isDungeonItemBossKey();
}

int item_getcheck_func_KEY_OF_FILONE() {
    return dComIfGs_getKeyNum();
}

int isBomb(u8 i_itemNo) {
    int is_bomb = false;

    if (i_itemNo == dItemNo_BOMB_5_e || i_itemNo == dItemNo_BOMB_10_e || i_itemNo == dItemNo_BOMB_20_e || i_itemNo == dItemNo_BOMB_30_e ||
        i_itemNo == dItemNo_NORMAL_BOMB_e | i_itemNo == dItemNo_WATER_BOMB_e || i_itemNo == dItemNo_POKE_BOMB_e)
    {
        is_bomb = true;
    }

    return is_bomb;
}

int isArrow(u8 i_itemNo) {
    int is_arrow = false;

    if (i_itemNo == dItemNo_ARROW_1_e || i_itemNo == dItemNo_ARROW_10_e || i_itemNo == dItemNo_ARROW_20_e || i_itemNo == dItemNo_ARROW_30_e)
    {
        is_arrow = true;
    }

    return is_arrow;
}

BOOL isBottleItem(u8 i_itemNo) {
    switch (i_itemNo) {
    case dItemNo_OIL_BOTTLE3_e:
    case dItemNo_EMPTY_BOTTLE_e:
    case dItemNo_RED_BOTTLE_e:
    case dItemNo_GREEN_BOTTLE_e:
    case dItemNo_BLUE_BOTTLE_e:
    case dItemNo_MILK_BOTTLE_e:
    case dItemNo_HALF_MILK_BOTTLE_e:
    case dItemNo_OIL_BOTTLE_e:
    case dItemNo_WATER_BOTTLE_e:
    case dItemNo_OIL_BOTTLE_2_e:
    case dItemNo_RED_BOTTLE_2_e:
    case dItemNo_UGLY_SOUP_e:
    case dItemNo_HOT_SPRING_e:
    case dItemNo_FAIRY_e:
    case dItemNo_FAIRY_DROP_e:
    case dItemNo_WORM_e:
    case dItemNo_BEE_CHILD_e:
    case dItemNo_CHUCHU_RARE_e:
    case dItemNo_CHUCHU_RED_e:
    case dItemNo_CHUCHU_BLUE_e:
    case dItemNo_CHUCHU_GREEN_e:
    case dItemNo_CHUCHU_YELLOW_e:
    case dItemNo_CHUCHU_PURPLE_e:
    case dItemNo_LV1_SOUP_e:
    case dItemNo_LV2_SOUP_e:
    case dItemNo_LV3_SOUP_e:
    case dItemNo_CHUCHU_BLACK_e:
    case dItemNo_POU_FIRE1_e:
    case dItemNo_POU_FIRE2_e:
    case dItemNo_POU_FIRE3_e:
    case dItemNo_POU_FIRE4_e:
        return TRUE;
    default:
        return FALSE;
    }
}

BOOL isHeart(u8 i_itemNo) {
    BOOL is_heart = false;

    if (i_itemNo == dItemNo_HEART_e || i_itemNo == dItemNo_TRIPLE_HEART_e) {
        is_heart = true;
    }

    return is_heart;
}

#if TARGET_PC
BOOL isRupee(u8 i_itemNo) {
    switch (i_itemNo) {
    case dItemNo_GREEN_RUPEE_e:
    case dItemNo_BLUE_RUPEE_e:
    case dItemNo_YELLOW_RUPEE_e:
    case dItemNo_RED_RUPEE_e:
    case dItemNo_PURPLE_RUPEE_e:
    case dItemNo_ORANGE_RUPEE_e:
    case dItemNo_SILVER_RUPEE_e:
        return true;
    default:
        break;
    }

    return false;
}
#endif

BOOL isInsect(u8 i_itemNo) {
    BOOL is_insect = false;

    switch (i_itemNo) {
    case dItemNo_M_BEETLE_e:
    case dItemNo_F_BEETLE_e:
    case dItemNo_M_BUTTERFLY_e:
    case dItemNo_F_BUTTERFLY_e:
    case dItemNo_M_STAG_BEETLE_e:
    case dItemNo_F_STAG_BEETLE_e:
    case dItemNo_M_GRASSHOPPER_e:
    case dItemNo_F_GRASSHOPPER_e:
    case dItemNo_M_NANAFUSHI_e:
    case dItemNo_F_NANAFUSHI_e:
    case dItemNo_M_DANGOMUSHI_e:
    case dItemNo_F_DANGOMUSHI_e:
    case dItemNo_M_MANTIS_e:
    case dItemNo_F_MANTIS_e:
    case dItemNo_M_LADYBUG_e:
    case dItemNo_F_LADYBUG_e:
    case dItemNo_M_SNAIL_e:
    case dItemNo_F_SNAIL_e:
    case dItemNo_M_DRAGONFLY_e:
    case dItemNo_F_DRAGONFLY_e:
    case dItemNo_M_ANT_e:
    case dItemNo_F_ANT_e:
    case dItemNo_M_MAYFLY_e:
    case dItemNo_F_MAYFLY_e:
        is_insect = true;
    }

    return is_insect;
}

u8 check_itemno(int i_itemNo) {
    if (!dComIfGs_isGetMagicUseFlag() && (i_itemNo == dItemNo_S_MAGIC_e || i_itemNo == dItemNo_L_MAGIC_e)) {
        return dItemNo_GREEN_RUPEE_e;
    }

    if (i_itemNo == dItemNo_ARROW_1_e) {
        if (!dComIfGs_isItemFirstBit(dItemNo_BOW_e)) {
            return dItemNo_GREEN_RUPEE_e;
        }
    } else {
        if (isArrow(i_itemNo)) {
            if (!dComIfGs_isItemFirstBit(dItemNo_BOW_e)) {
                return dItemNo_GREEN_RUPEE_e;
            }

            if (g_dComIfG_gameInfo.play.getLayerNo(0) == 0xD ||
                g_dComIfG_gameInfo.play.getLayerNo(0) == 0xE)
            {
                const char* stage_name = dComIfGp_getStartStageName();
                // D_MN08: Palace of Twilight
                if (strncmp(stage_name, "D_MN08", 6)) {
                    return dItemNo_GREEN_RUPEE_e;
                }
            }
        }
    }

    if (!dComIfGs_isItemFirstBit(dItemNo_BOMB_BAG_LV1_e) && isBomb(i_itemNo)) {
        return dItemNo_GREEN_RUPEE_e;
    } else {
        if (i_itemNo == dItemNo_TRIPLE_HEART_e) {
            i_itemNo = dItemNo_HEART_e;
        }
        if (!checkItemGet(dItemNo_PACHINKO_e, 1) && i_itemNo == dItemNo_PACHINKO_SHOT_e) {
            i_itemNo = dItemNo_GREEN_RUPEE_e;
        }
        if (i_itemNo == dItemNo_S_MAGIC_e || i_itemNo == dItemNo_L_MAGIC_e) {
            i_itemNo = dItemNo_GREEN_RUPEE_e;
        }
    }
    return i_itemNo;
}

int addBombCount(u8 i_bombType, u8 i_addNum) {
    u8 bombType[3];
    int bombNum[3];

    for (u8 i = 0; i < 3; i++) {
        bombType[i] = dComIfGs_getItem(i + SLOT_15, false);

        if (bombType[i] == dItemNo_BOMB_BAG_LV1_e) {
            bombNum[i] = 0;
        } else if (bombType[i] == i_bombType) {
            bombNum[i] = dComIfGs_getBombNum(i);
        } else {
            bombNum[i] = -1;
        }
    }

    for (u8 i = 0; i < 3; i++) {
        int bombIdx = -1;
        int var_r22 = -1;

        for (u8 j = 0; j < 3; j++) {
            if (bombNum[j] == 0) {
                bombIdx = j;
                var_r22 = 0;
            }
        }

        for (u8 k = 0; k < 3; k++) {
            if (bombNum[k] > 0 && bombNum[k] > var_r22 &&
                bombNum[k] != dComIfGs_getBombMax(bombType[k]))
            {
                bombIdx = k;
                var_r22 = bombNum[k];
            }
        }

        if (bombIdx == -1) {
            return i_addNum;
        } else if (var_r22 == 0) {
            if (dComIfGs_getBombMax(i_bombType) >= i_addNum) {
                dComIfGs_setEmptyBombBagItemIn(i_bombType, i_addNum, true);
                return 0;
            } else {
                dComIfGs_setEmptyBombBagItemIn(i_bombType, i_addNum, true);
                i_addNum = i_addNum - dComIfGs_getBombMax(i_bombType);
            }
        } else {
            if (dComIfGs_getBombMax(bombType[bombIdx]) >= var_r22 + i_addNum) {
                dComIfGp_setItemBombNumCount(bombIdx, i_addNum);
                return 0;
            } else {
                dComIfGp_setItemBombNumCount(bombIdx, i_addNum);
                i_addNum = i_addNum - (dComIfGs_getBombMax(bombType[bombIdx]) - var_r22);
            }
        }

        bombNum[bombIdx] = dComIfGs_getBombMax(bombType[bombIdx]);
    }

    return i_addNum;
}

u8* dEnemyItem_c::mData;
