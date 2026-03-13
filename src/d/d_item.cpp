/**
 * d_item.cpp
 * Item Get Handling
 */

#include "d/dolzel.h" // IWYU pragma: keep

#include "d/d_item.h"
#include "d/d_com_inf_game.h"
#include "d/d_meter2_info.h"
#include <cstring>

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

inline void getItemFunc(u8 i_itemNo) {
    dComIfGs_onItemFirstBit(i_itemNo);
    item_func_ptr[i_itemNo]();
}

void execItemGet(u8 i_itemNo) {
    getItemFunc(i_itemNo);
}

static int (*item_getcheck_func_ptr[256])() = {
    item_getcheck_func_HEART,
    item_getcheck_func_GREEN_RUPEE,
    item_getcheck_func_BLUE_RUPEE,
    item_getcheck_func_YELLOW_RUPEE,
    item_getcheck_func_RED_RUPEE,
    item_getcheck_func_PURPLE_RUPEE,
    item_getcheck_func_ORANGE_RUPEE,
    item_getcheck_func_SILVER_RUPEE,
    item_getcheck_func_S_MAGIC,
    item_getcheck_func_L_MAGIC,
    item_getcheck_func_BOMB_5,
    item_getcheck_func_BOMB_10,
    item_getcheck_func_BOMB_20,
    item_getcheck_func_BOMB_30,
    item_getcheck_func_ARROW_10,
    item_getcheck_func_ARROW_20,
    item_getcheck_func_ARROW_30,
    item_getcheck_func_ARROW_1,
    item_getcheck_func_PACHINKO_SHOT,
    item_getcheck_func_noentry,
    item_getcheck_func_noentry,
    item_getcheck_func_noentry,
    item_getcheck_func_WATER_BOMB_5,
    item_getcheck_func_WATER_BOMB_10,
    item_getcheck_func_WATER_BOMB_20,
    item_getcheck_func_WATER_BOMB_30,
    item_getcheck_func_BOMB_INSECT_5,
    item_getcheck_func_BOMB_INSECT_10,
    item_getcheck_func_BOMB_INSECT_20,
    item_getcheck_func_BOMB_INSECT_30,
    item_getcheck_func_RECOVER_FAILY,
    item_getcheck_func_TRIPLE_HEART,
    item_getcheck_func_SMALL_KEY,
    item_getcheck_func_KAKERA_HEART,
    item_getcheck_func_UTUWA_HEART,
    item_getcheck_func_MAP,
    item_getcheck_func_COMPUS,
    item_getcheck_func_DUNGEON_EXIT,
    item_getcheck_func_BOSS_KEY,
    item_getcheck_func_DUNGEON_BACK,
    item_getcheck_func_SWORD,
    item_getcheck_func_MASTER_SWORD,
    item_getcheck_func_WOOD_SHIELD,
    item_getcheck_func_SHIELD,
    item_getcheck_func_HYLIA_SHIELD,
    item_getcheck_func_TKS_LETTER,
    item_getcheck_func_WEAR_CASUAL,
    item_getcheck_func_WEAR_KOKIRI,
    item_getcheck_func_ARMOR,
    item_getcheck_func_WEAR_ZORA,
    item_getcheck_func_MAGIC_LV1,
    item_getcheck_func_DUNGEON_EXIT_2,
    item_getcheck_func_WALLET_LV1,
    item_getcheck_func_WALLET_LV2,
    item_getcheck_func_WALLET_LV3,
    item_getcheck_func_noentry,
    item_getcheck_func_noentry,
    item_getcheck_func_noentry,
    item_getcheck_func_noentry,
    item_getcheck_func_noentry,
    item_getcheck_func_noentry,
    item_getcheck_func_ZORAS_JEWEL,
    item_getcheck_func_HAWK_EYE,
    item_getcheck_func_WOOD_STICK,
    item_getcheck_func_BOOMERANG,
    item_getcheck_func_SPINNER,
    item_getcheck_func_IRONBALL,
    item_getcheck_func_BOW,
    item_getcheck_func_HOOKSHOT,
    item_getcheck_func_HVY_BOOTS,
    item_getcheck_func_COPY_ROD,
    item_getcheck_func_W_HOOKSHOT,
    item_getcheck_func_KANTERA,
    item_getcheck_func_LIGHT_SWORD,
    item_getcheck_func_FISHING_ROD_1,
    item_getcheck_func_PACHINKO,
    item_getcheck_func_COPY_ROD_2,
    item_getcheck_func_noentry,
    item_getcheck_func_noentry,
    item_getcheck_func_BOMB_BAG_LV2,
    item_getcheck_func_BOMB_BAG_LV1,
    item_getcheck_func_BOMB_IN_BAG,
    item_getcheck_func_noentry,
    item_getcheck_func_LIGHT_ARROW,
    item_getcheck_func_ARROW_LV1,
    item_getcheck_func_ARROW_LV2,
    item_getcheck_func_ARROW_LV3,
    item_getcheck_func_noentry,
    item_getcheck_func_LURE_ROD,
    item_getcheck_func_BOMB_ARROW,
    item_getcheck_func_HAWK_ARROW,
    item_getcheck_func_BEE_ROD,
    item_getcheck_func_JEWEL_ROD,
    item_getcheck_func_WORM_ROD,
    item_getcheck_func_JEWEL_BEE_ROD,
    item_getcheck_func_JEWEL_WORM_ROD,
    item_getcheck_func_EMPTY_BOTTLE,
    item_getcheck_func_RED_BOTTLE,
    item_getcheck_func_GREEN_BOTTLE,
    item_getcheck_func_BLUE_BOTTLE,
    item_getcheck_func_MILK_BOTTLE,
    item_getcheck_func_HALF_MILK_BOTTLE,
    item_getcheck_func_OIL_BOTTLE,
    item_getcheck_func_WATER_BOTTLE,
    item_getcheck_func_OIL_BOTTLE2,
    item_getcheck_func_RED_BOTTLE2,
    item_getcheck_func_UGLY_SOUP,
    item_getcheck_func_HOT_SPRING,
    item_getcheck_func_FAIRY_BOTTLE,
    item_getcheck_func_HOT_SPRING2,
    item_getcheck_func_OIL2,
    item_getcheck_func_OIL,
    item_getcheck_func_NORMAL_BOMB,
    item_getcheck_func_WATER_BOMB,
    item_getcheck_func_POKE_BOMB,
    item_getcheck_func_FAIRY_DROP,
    item_getcheck_func_WORM,
    item_getcheck_func_DROP_BOTTLE,
    item_getcheck_func_BEE_CHILD,
    item_getcheck_func_CHUCHU_RARE,
    item_getcheck_func_CHUCHU_RED,
    item_getcheck_func_CHUCHU_BLUE,
    item_getcheck_func_CHUCHU_GREEN,
    item_getcheck_func_CHUCHU_YELLOW,
    item_getcheck_func_CHUCHU_PURPLE,
    item_getcheck_func_LV1_SOUP,
    item_getcheck_func_LV2_SOUP,
    item_getcheck_func_LV3_SOUP,
    item_getcheck_func_LETTER,
    item_getcheck_func_BILL,
    item_getcheck_func_WOOD_STATUE,
    item_getcheck_func_IRIAS_PENDANT,
    item_getcheck_func_HORSE_FLUTE,
    item_getcheck_func_noentry,
    item_getcheck_func_noentry,
    item_getcheck_func_noentry,
    item_getcheck_func_noentry,
    item_getcheck_func_noentry,
    item_getcheck_func_noentry,
    item_getcheck_func_noentry,
    item_getcheck_func_noentry,
    item_getcheck_func_noentry,
    item_getcheck_func_noentry,
    item_getcheck_func_noentry,
    item_getcheck_func_RAFRELS_MEMO,
    item_getcheck_func_ASHS_SCRIBBLING,
    item_getcheck_func_noentry,
    item_getcheck_func_noentry,
    item_getcheck_func_noentry,
    item_getcheck_func_noentry,
    item_getcheck_func_noentry,
    item_getcheck_func_noentry,
    item_getcheck_func_noentry,
    item_getcheck_func_noentry,
    item_getcheck_func_noentry,
    item_getcheck_func_noentry,
    item_getcheck_func_CHUCHU_YELLOW2,
    item_getcheck_func_OIL_BOTTLE3,
    item_getcheck_func_SHOP_BEE_CHILD,
    item_getcheck_func_CHUCHU_BLACK,
    item_getcheck_func_LIGHT_DROP,
    item_getcheck_func_DROP_CONTAINER,
    item_getcheck_func_DROP_CONTAINER02,
    item_getcheck_func_DROP_CONTAINER03,
    item_getcheck_func_FILLED_CONTAINER,
    item_getcheck_func_MIRROR_PIECE_2,
    item_getcheck_func_MIRROR_PIECE_3,
    item_getcheck_func_MIRROR_PIECE_4,
    item_getcheck_func_noentry,
    item_getcheck_func_noentry,
    item_getcheck_func_noentry,
    item_getcheck_func_noentry,
    item_getcheck_func_noentry,
    item_getcheck_func_noentry,
    item_getcheck_func_noentry,
    item_getcheck_func_noentry,
    item_getcheck_func_SMELL_YELIA_POUCH,
    item_getcheck_func_SMELL_PUMPKIN,
    item_getcheck_func_SMELL_POH,
    item_getcheck_func_SMELL_FISH,
    item_getcheck_func_SMELL_CHILDREN,
    item_getcheck_func_SMELL_MEDICINE,
    item_getcheck_func_noentry,
    item_getcheck_func_noentry,
    item_getcheck_func_noentry,
    item_getcheck_func_noentry,
    item_getcheck_func_noentry,
    item_getcheck_func_noentry,
    item_getcheck_func_noentry,
    item_getcheck_func_noentry,
    item_getcheck_func_noentry,
    item_getcheck_func_noentry,
    item_getcheck_func_M_BEETLE,
    item_getcheck_func_F_BEETLE,
    item_getcheck_func_M_BUTTERFLY,
    item_getcheck_func_F_BUTTERFLY,
    item_getcheck_func_M_STAG_BEETLE,
    item_getcheck_func_F_STAG_BEETLE,
    item_getcheck_func_M_GRASSHOPPER,
    item_getcheck_func_F_GRASSHOPPER,
    item_getcheck_func_M_NANAFUSHI,
    item_getcheck_func_F_NANAFUSHI,
    item_getcheck_func_M_DANGOMUSHI,
    item_getcheck_func_F_DANGOMUSHI,
    item_getcheck_func_M_MANTIS,
    item_getcheck_func_F_MANTIS,
    item_getcheck_func_M_LADYBUG,
    item_getcheck_func_F_LADYBUG,
    item_getcheck_func_M_SNAIL,
    item_getcheck_func_F_SNAIL,
    item_getcheck_func_M_DRAGONFLY,
    item_getcheck_func_F_DRAGONFLY,
    item_getcheck_func_M_ANT,
    item_getcheck_func_F_ANT,
    item_getcheck_func_M_MAYFLY,
    item_getcheck_func_F_MAYFLY,
    item_getcheck_func_noentry,
    item_getcheck_func_noentry,
    item_getcheck_func_noentry,
    item_getcheck_func_noentry,
    item_getcheck_func_noentry,
    item_getcheck_func_noentry,
    item_getcheck_func_noentry,
    item_getcheck_func_noentry,
    item_getcheck_func_POU_SPIRIT,
    item_getcheck_func_noentry,
    item_getcheck_func_noentry,
    item_getcheck_func_noentry,
    item_getcheck_func_noentry,
    item_getcheck_func_noentry,
    item_getcheck_func_noentry,
    item_getcheck_func_noentry,
    item_getcheck_func_noentry,
    item_getcheck_func_ANCIENT_DOCUMENT,
    item_getcheck_func_AIR_LETTER,
    item_getcheck_func_ANCIENT_DOCUMENT2,
    item_getcheck_func_LV7_DUNGEON_EXIT,
    item_getcheck_func_LINKS_SAVINGS,
    item_getcheck_func_SMALL_KEY2,
    item_getcheck_func_POU_FIRE1,
    item_getcheck_func_POU_FIRE2,
    item_getcheck_func_POU_FIRE3,
    item_getcheck_func_POU_FIRE4,
    item_getcheck_func_BOSSRIDER_KEY,
    item_getcheck_func_TOMATO_PUREE,
    item_getcheck_func_TASTE,
    item_getcheck_func_LV5_BOSS_KEY,
    item_getcheck_func_SURFBOARD,
    item_getcheck_func_KANTERA2,
    item_getcheck_func_L2_KEY_PIECES1,
    item_getcheck_func_L2_KEY_PIECES2,
    item_getcheck_func_L2_KEY_PIECES3,
    item_getcheck_func_KEY_OF_CARAVAN,
    item_getcheck_func_LV2_BOSS_KEY,
    item_getcheck_func_KEY_OF_FILONE,
    item_getcheck_func_noentry,
};

inline int getCheckItemFunc(u8 i_itemNo) {
    return item_getcheck_func_ptr[i_itemNo]();
}

int checkItemGet(u8 i_itemNo, int i_default) {
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
}

void item_func_WALLET_LV3() {
    dComIfGs_setWalletSize(GIANT_WALLET);
}

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
    dComIfGs_setItem(SLOT_8, dItemNo_COPY_ROD_e);
}

void item_func_BOMB_BAG_LV2() {}

void item_func_BOMB_BAG_LV1() {
    dComIfGs_setEmptyBombBag(dItemNo_NORMAL_BOMB_e, 30);
}

void item_func_BOMB_IN_BAG() {
    dComIfGs_setEmptyBombBag(dItemNo_NORMAL_BOMB_e, 30);
}

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
    dComIfGs_setItem(SLOT_21, dItemNo_LETTER_e);
}

void item_func_BILL() {
    dComIfGs_setItem(SLOT_21, dItemNo_BILL_e);
}

void item_func_WOOD_STATUE() {
    /* dSv_event_flag_c::F_283 - Hyrule Field - Get wood carving */
    dComIfGs_onEventBit(dSv_event_flag_c::saveBitLabels[283]);
    dComIfGs_setItem(SLOT_21, dItemNo_WOOD_STATUE_e);
}

void item_func_IRIAS_PENDANT() {
    dComIfGs_setItem(SLOT_21, dItemNo_IRIAS_PENDANT_e);
}

void item_func_HORSE_FLUTE() {
    dComIfGs_setItem(SLOT_21, dItemNo_HORSE_FLUTE_e);
}

void item_func_RAFRELS_MEMO() {
    dComIfGs_setItem(SLOT_19, dItemNo_RAFRELS_MEMO_e);
}

void item_func_ASHS_SCRIBBLING() {
    dComIfGs_setItem(SLOT_19, dItemNo_ASHS_SCRIBBLING_e);
}

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
}

void item_func_FILLED_CONTAINER() {}

void item_func_MIRROR_PIECE_2() {}

void item_func_MIRROR_PIECE_3() {}

void item_func_MIRROR_PIECE_4() {}

void item_func_SMELL_YELIA_POUCH() {}

void item_func_SMELL_PUMPKIN() {}

void item_func_SMELL_POH() {}

void item_func_SMELL_FISH() {}

void item_func_SMELL_CHILDREN() {}

void item_func_SMELL_MEDICINE() {}

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

void item_func_POU_SPIRIT() {}

void item_func_ANCIENT_DOCUMENT() {
    dComIfGs_setItem(SLOT_22, dItemNo_ANCIENT_DOCUMENT_e);
}

void item_func_AIR_LETTER() {
    dComIfGs_setItem(SLOT_22, dItemNo_AIR_LETTER_e);
}

void item_func_ANCIENT_DOCUMENT2() {
    dComIfGs_setItem(SLOT_22, dItemNo_ANCIENT_DOCUMENT2_e);
}

void item_func_LV7_DUNGEON_EXIT() {
    dComIfGs_setItem(SLOT_18, dItemNo_LV7_DUNGEON_EXIT_e);
}

void item_func_LINKS_SAVINGS() {
    dComIfGp_setItemRupeeCount(50);
}

void item_func_SMALL_KEY2() {
    dComIfGp_setItemKeyNumCount(1);
}

void item_func_POU_FIRE1() {}

void item_func_POU_FIRE2() {}

void item_func_POU_FIRE3() {}

void item_func_POU_FIRE4() {}

void item_func_BOSSRIDER_KEY() {}

void item_func_TOMATO_PUREE() {}

void item_func_TASTE() {}

void item_func_LV5_BOSS_KEY() {
    dComIfGs_onDungeonItemBossKey();
}

void item_func_SURFBOARD() {}

void item_func_KANTERA2() {}

void item_func_L2_KEY_PIECES1() {}

void item_func_L2_KEY_PIECES2() {}

void item_func_L2_KEY_PIECES3() {}

void item_func_KEY_OF_CARAVAN() {}

void item_func_LV2_BOSS_KEY() {
    dComIfGs_onDungeonItemBossKey();
}

void item_func_KEY_OF_FILONE() {
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
    return -1;
}

int item_getcheck_func_DUNGEON_EXIT_2() {
    return dComIfGs_getItem(SLOT_18, true) == dItemNo_DUNGEON_EXIT_e ? TRUE : FALSE;
}

int item_getcheck_func_WALLET_LV1() {
    return -1;
}

int item_getcheck_func_WALLET_LV2() {
    return -1;
}

int item_getcheck_func_WALLET_LV3() {
    return -1;
}

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

int item_getcheck_func_BOMB_BAG_LV2() {
    return dComIfGs_isItemFirstBit(dItemNo_BOMB_BAG_LV2_e);
}

int item_getcheck_func_BOMB_BAG_LV1() {
    return dComIfGs_isItemFirstBit(dItemNo_BOMB_BAG_LV1_e);
}

int item_getcheck_func_BOMB_IN_BAG() {
    return dComIfGs_isItemFirstBit(dItemNo_BOMB_IN_BAG_e);
}

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
    return dComIfGs_getItem(SLOT_21, true) == dItemNo_LETTER_e ? TRUE : FALSE;
}

int item_getcheck_func_BILL() {
    return dComIfGs_getItem(SLOT_21, true) == dItemNo_BILL_e ? TRUE : FALSE;
}

int item_getcheck_func_WOOD_STATUE() {
    return dComIfGs_getItem(SLOT_21, true) == dItemNo_WOOD_STATUE_e ? TRUE : FALSE;
}

int item_getcheck_func_IRIAS_PENDANT() {
    return dComIfGs_getItem(SLOT_21, true) == dItemNo_IRIAS_PENDANT_e ? TRUE : FALSE;
}

int item_getcheck_func_HORSE_FLUTE() {
    return dComIfGs_getItem(SLOT_22, true) == dItemNo_HORSE_FLUTE_e ? TRUE : FALSE;
}

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
    return -1;
}

int item_getcheck_func_MIRROR_PIECE_3() {
    return -1;
}

int item_getcheck_func_MIRROR_PIECE_4() {
    return -1;
}

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

int item_getcheck_func_POU_SPIRIT() {
    return dComIfGs_getPohSpiritNum();
}

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
