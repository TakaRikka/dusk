#ifndef D_D_ITEM_DATA_H
#define D_D_ITEM_DATA_H

#include <types.h>

struct dItem_itemInfo {
    /* 0x0 */ u8 mShadowSize;
    /* 0x1 */ u8 mH;
    /* 0x2 */ u8 mR;
    /* 0x3 */ u8 mFlag;
};

struct dItem_itemResource {
    f32 getTexScale() { return mTexScale; }

    /* 0x00 */ const char* mArcName;
    /* 0x04 */ s16 mBmdName;
    /* 0x06 */ s16 mBtkName;
    /* 0x08 */ s16 mBckName;
    /* 0x0A */ s16 mBrkName;
    /* 0x0C */ s16 mBtpName;
    /* 0x0E */ s8 mTevFrm;
    /* 0x0F */ s8 mBtpFrm;
    /* 0x10 */ s16 mTexture;
    /* 0x12 */ u8 mTexScale;
    /* 0x14 */ s16 field_0x14;
};  // Size: 0x18

struct dItem_fieldItemResource {
    /* 0x0 */ const char* mFieldArc;
    /* 0x4 */ s16 mItemBmdName;
    /* 0x6 */ s16 mItemBckName;
    /* 0x8 */ s16 mItemBrkName;
    /* 0xA */ u8 field_0xa;
    /* 0xC */ u16 mHeapSize;
};  // Size: 0x10

struct dItem_data {
    static const char* getArcName(u8 index) { return item_resource[index].mArcName; }

    static s16 getBtpName(u8 index) { return item_resource[index].mBtpName; }

    static s16 getBrkName(u8 index) { return item_resource[index].mBrkName; }

    static s16 getBxaName(u8 index) { return -1; }

    static s16 getBckName(u8 index) { return item_resource[index].mBckName; }

    static s16 getBpkName(u8 index) { return -1; }

    static s16 getBtkName(u8 index) { return item_resource[index].mBtkName; }

    static s16 getBmdName(u8 index) { return item_resource[index].mBmdName; }

    static s8 getTevFrm(u8 index) { return item_resource[index].mTevFrm; }

    static s8 getBtpFrm(u8 index) { return item_resource[index].mBtpFrm; }

    static s16 getTexture(u8 index) { return item_resource[index].mTexture; }

    static u8 getTexScale (u8 index) { return item_resource[index].mTexScale; }

    static const char* getFieldArc(u8 index) { return field_item_res[index].mFieldArc; }

    static s16 getItemBmdName(u8 index) { return field_item_res[index].mItemBmdName; }

    static s16 getItemBtkName(u8 index) { return -1; }

    static s16 getItemBpkName(u8 index) { return -1; }

    static s16 getItemBckName(u8 index) { return field_item_res[index].mItemBckName; }

    static s16 getItemBxaName(u8 index) { return -1; }

    static s16 getItemBrkName(u8 index) { return field_item_res[index].mItemBrkName; }

    static s16 getItemBtpName(u8 index) { return -1; }

    static BOOL chkFlag(u8 index, int flag) { return item_info[index].mFlag & flag; }

    static u8 getShadowSize(u8 index) { return item_info[index].mShadowSize; }

    static const u8 getH(u8 index) { return item_info[index].mH; }

    static const u8 getR(u8 index) { return item_info[index].mR; }

    static u16 getFieldHeapSize(u8 index) { return field_item_res[index].mHeapSize; } 

    static dItem_itemResource item_resource[255];
    static dItem_fieldItemResource field_item_res[255];
    static dItem_itemInfo item_info[255];
};

enum {
    /* 0x00 */ dItemNo_HEART_e,
    /* 0x01 */ dItemNo_GREEN_RUPEE_e,
    /* 0x02 */ dItemNo_BLUE_RUPEE_e,
    /* 0x03 */ dItemNo_YELLOW_RUPEE_e,
    /* 0x04 */ dItemNo_RED_RUPEE_e,
    /* 0x05 */ dItemNo_PURPLE_RUPEE_e,
    /* 0x06 */ dItemNo_ORANGE_RUPEE_e,
    /* 0x07 */ dItemNo_SILVER_RUPEE_e,
    /* 0x08 */ dItemNo_S_MAGIC_e,
    /* 0x09 */ dItemNo_L_MAGIC_e,
    /* 0x0A */ dItemNo_BOMB_5_e,
    /* 0x0B */ dItemNo_BOMB_10_e,
    /* 0x0C */ dItemNo_BOMB_20_e,
    /* 0x0D */ dItemNo_BOMB_30_e,
    /* 0x0E */ dItemNo_ARROW_10_e,
    /* 0x0F */ dItemNo_ARROW_20_e,
    /* 0x10 */ dItemNo_ARROW_30_e,
    /* 0x11 */ dItemNo_ARROW_1_e,
    /* 0x12 */ dItemNo_PACHINKO_SHOT_e,
    /* 0x13 */ dItemNo_NOENTRY_19_e,
    /* 0x14 */ dItemNo_NOENTRY_20_e,
    /* 0x15 */ dItemNo_NOENTRY_21_e,
    /* 0x16 */ dItemNo_WATER_BOMB_5_e,
    /* 0x17 */ dItemNo_WATER_BOMB_10_e,
    /* 0x18 */ dItemNo_WATER_BOMB_20_e,
    /* 0x19 */ dItemNo_WATER_BOMB_30_e,
    /* 0x1A */ dItemNo_BOMB_INSECT_5_e,
    /* 0x1B */ dItemNo_BOMB_INSECT_10_e,
    /* 0x1C */ dItemNo_BOMB_INSECT_20_e,
    /* 0x1D */ dItemNo_BOMB_INSECT_30_e,
    /* 0x1E */ dItemNo_RECOVERY_FAILY_e,
    /* 0x1F */ dItemNo_TRIPLE_HEART_e,
    /* 0x20 */ dItemNo_SMALL_KEY_e,
    /* 0x21 */ dItemNo_KAKERA_HEART_e,
    /* 0x22 */ dItemNo_UTAWA_HEART_e,
    /* 0x23 */ dItemNo_MAP_e,
    /* 0x24 */ dItemNo_COMPUS_e,
    /* 0x25 */ dItemNo_DUNGEON_EXIT_e,
    /* 0x26 */ dItemNo_BOSS_KEY_e,
    /* 0x27 */ dItemNo_DUNGEON_BACK_e,
    /* 0x28 */ dItemNo_SWORD_e,
    /* 0x29 */ dItemNo_MASTER_SWORD_e,
    /* 0x2A */ dItemNo_WOOD_SHIELD_e,
    /* 0x2B */ dItemNo_SHIELD_e,
    /* 0x2C */ dItemNo_HYLIA_SHIELD_e,
    /* 0x2D */ dItemNo_TKS_LETTER_e,
    /* 0x2E */ dItemNo_WEAR_CASUAL_e,
    /* 0x2F */ dItemNo_WEAR_KOKIRI_e,
    /* 0x30 */ dItemNo_ARMOR_e,
    /* 0x31 */ dItemNo_WEAR_ZORA_e,
    /* 0x32 */ dItemNo_MAGIC_LV1_e,
    /* 0x33 */ dItemNo_DUNGEON_EXIT_2_e,
    /* 0x34 */ dItemNo_WALLET_LV1_e,
    /* 0x35 */ dItemNo_WALLET_LV2_e,
    /* 0x36 */ dItemNo_WALLET_LV3_e,
    /* 0x37 */ dItemNo_NOENTRY_55_e,
    /* 0x38 */ dItemNo_NOENTRY_56_e,
    /* 0x39 */ dItemNo_NOENTRY_57_e,
    /* 0x3A */ dItemNo_NOENTRY_58_e,
    /* 0x3B */ dItemNo_NOENTRY_59_e,
    /* 0x3C */ dItemNo_NOENTRY_60_e,
    /* 0x3D */ dItemNo_ZORAS_JEWEL_e,
    /* 0x3E */ dItemNo_HAWK_EYE_e,
    /* 0x3F */ dItemNo_WOOD_STICK_e,
    /* 0x40 */ dItemNo_BOOMERANG_e,
    /* 0x41 */ dItemNo_SPINNER_e,
    /* 0x42 */ dItemNo_IRONBALL_e,
    /* 0x43 */ dItemNo_BOW_e,
    /* 0x44 */ dItemNo_HOOKSHOT_e,
    /* 0x45 */ dItemNo_HVY_BOOTS_e,
    /* 0x46 */ dItemNo_COPY_ROD_e,
    /* 0x47 */ dItemNo_W_HOOKSHOT_e,
    /* 0x48 */ dItemNo_KANTERA_e,
    /* 0x49 */ dItemNo_LIGHT_SWORD_e,
    /* 0x4A */ dItemNo_FISHING_ROD_1_e,
    /* 0x4B */ dItemNo_PACHINKO_e,
    /* 0x4C */ dItemNo_COPY_ROD_2_e,
    /* 0x4D */ dItemNo_NOENTRY_77_e,
    /* 0x4E */ dItemNo_NOENTRY_78_e,
    /* 0x4F */ dItemNo_BOMB_BAG_LV2_e,
    /* 0x50 */ dItemNo_BOMB_BAG_LV1_e,
    /* 0x51 */ dItemNo_BOMB_IN_BAG_e,
    /* 0x52 */ dItemNo_NOENTRY_82_e,
    /* 0x53 */ dItemNo_LIGHT_ARROW_e,
    /* 0x54 */ dItemNo_ARROW_LV1_e,
    /* 0x55 */ dItemNo_ARROW_LV2_e,
    /* 0x56 */ dItemNo_ARROW_LV3_e,
    /* 0x57 */ dItemNo_NOENTRY_87_e,
    /* 0x58 */ dItemNo_LURE_ROD_e,
    /* 0x59 */ dItemNo_BOMB_ARROW_e,
    /* 0x5A */ dItemNo_HAWK_ARROW_e,
    /* 0x5B */ dItemNo_BEE_ROD_e,
    /* 0x5C */ dItemNo_JEWEL_ROD_e,
    /* 0x5D */ dItemNo_WORM_ROD_e,
    /* 0x5E */ dItemNo_JEWEL_BEE_ROD_e,
    /* 0x5F */ dItemNo_JEWEL_WORM_ROD_e,
    /* 0x60 */ dItemNo_EMPTY_BOTTLE_e,
    /* 0x61 */ dItemNo_RED_BOTTLE_e,
    /* 0x62 */ dItemNo_GREEN_BOTTLE_e,
    /* 0x63 */ dItemNo_BLUE_BOTTLE_e,
    /* 0x64 */ dItemNo_MILK_BOTTLE_e,
    /* 0x65 */ dItemNo_HALF_MILK_BOTTLE_e,
    /* 0x66 */ dItemNo_OIL_BOTTLE_e,
    /* 0x67 */ dItemNo_WATER_BOTTLE_e,
    /* 0x68 */ dItemNo_OIL_BOTTLE_2_e,
    /* 0x69 */ dItemNo_RED_BOTTLE_2_e,
    /* 0x6A */ dItemNo_UGLY_SOUP_e,
    /* 0x6B */ dItemNo_HOT_SPRING_e,
    /* 0x6C */ dItemNo_FAIRY_e,
    /* 0x6D */ dItemNo_HOT_SPRING_2_e,
    /* 0x6E */ dItemNo_OIL2_e,
    /* 0x6F */ dItemNo_OIL_e,
    /* 0x70 */ dItemNo_NORMAL_BOMB_e,
    /* 0x71 */ dItemNo_WATER_BOMB_e,
    /* 0x72 */ dItemNo_POKE_BOMB_e,
    /* 0x73 */ dItemNo_FAIRY_DROP_e,
    /* 0x74 */ dItemNo_WORM_e,
    /* 0x75 */ dItemNo_DROP_BOTTLE_e,
    /* 0x76 */ dItemNo_BEE_CHILD_e,
    /* 0x77 */ dItemNo_CHUCHU_RARE_e,
    /* 0x78 */ dItemNo_CHUCHU_RED_e,
    /* 0x79 */ dItemNo_CHUCHU_BLUE_e,
    /* 0x7A */ dItemNo_CHUCHU_GREEN_e,
    /* 0x7B */ dItemNo_CHUCHU_YELLOW_e,
    /* 0x7C */ dItemNo_CHUCHU_PURPLE_e,
    /* 0x7D */ dItemNo_LV1_SOUP_e,
    /* 0x7E */ dItemNo_LV2_SOUP_e,
    /* 0x7F */ dItemNo_LV3_SOUP_e,
    /* 0x80 */ dItemNo_LETTER_e,
    /* 0x81 */ dItemNo_BILL_e,
    /* 0x82 */ dItemNo_WOOD_STATUE_e,
    /* 0x83 */ dItemNo_IRIAS_PENDANT_e,
    /* 0x84 */ dItemNo_HORSE_FLUTE_e,
    /* 0x85 */ dItemNo_NOENTRY_133_e,
    /* 0x86 */ dItemNo_NOENTRY_134_e,
    /* 0x87 */ dItemNo_NOENTRY_135_e,
    /* 0x88 */ dItemNo_NOENTRY_136_e,
    /* 0x89 */ dItemNo_NOENTRY_137_e,
    /* 0x8A */ dItemNo_NOENTRY_138_e,
    /* 0x8B */ dItemNo_NOENTRY_139_e,
    /* 0x8C */ dItemNo_NOENTRY_140_e,
    /* 0x8D */ dItemNo_NOENTRY_141_e,
    /* 0x8E */ dItemNo_NOENTRY_142_e,
    /* 0x8F */ dItemNo_NOENTRY_143_e,
    /* 0x90 */ dItemNo_RAFRELS_MEMO_e,
    /* 0x91 */ dItemNo_ASHS_SCRIBBLING_e,
    /* 0x92 */ dItemNo_NOENTRY_146_e,
    /* 0x93 */ dItemNo_NOENTRY_147_e,
    /* 0x94 */ dItemNo_NOENTRY_148_e,
    /* 0x95 */ dItemNo_NOENTRY_149_e,
    /* 0x96 */ dItemNo_NOENTRY_150_e,
    /* 0x97 */ dItemNo_NOENTRY_151_e,
    /* 0x98 */ dItemNo_NOENTRY_152_e,
    /* 0x99 */ dItemNo_NOENTRY_153_e,
    /* 0x9A */ dItemNo_NOENTRY_154_e,
    /* 0x9B */ dItemNo_NOENTRY_155_e,
    /* 0x9C */ dItemNo_CHUCHU_YELLOW2_e,
    /* 0x9D */ dItemNo_OIL_BOTTLE3_e,
    /* 0x9E */ dItemNo_SHOP_BEE_CHILD_e,
    /* 0x9F */ dItemNo_CHUCHU_BLACK_e,
    /* 0xA0 */ dItemNo_LIGHT_DROP_e,
    /* 0xA1 */ dItemNo_DROP_CONTAINER_e,
    /* 0xA2 */ dItemNo_DROP_CONTAINER02_e,
    /* 0xA3 */ dItemNo_DROP_CONTAINER03_e,
    /* 0xA4 */ dItemNo_FILLED_CONTAINER_e,
    /* 0xA5 */ dItemNo_MIRROR_PIECE_2_e,
    /* 0xA6 */ dItemNo_MIRROR_PIECE_3_e,
    /* 0xA7 */ dItemNo_MIRROR_PIECE_4_e,
    /* 0xA8 */ dItemNo_NOENTRY_168_e,
    /* 0xA9 */ dItemNo_NOENTRY_169_e,
    /* 0xAA */ dItemNo_NOENTRY_170_e,
    /* 0xAB */ dItemNo_NOENTRY_171_e,
    /* 0xAC */ dItemNo_NOENTRY_172_e,
    /* 0xAD */ dItemNo_NOENTRY_173_e,
    /* 0xAE */ dItemNo_NOENTRY_174_e,
    /* 0xAF */ dItemNo_NOENTRY_175_e,
    /* 0xB0 */ dItemNo_SMELL_YELIA_POUCH_e,
    /* 0xB1 */ dItemNo_SMELL_PUMPKIN_e,
    /* 0xB2 */ dItemNo_SMELL_POH_e,
    /* 0xB3 */ dItemNo_SMELL_FISH_e,
    /* 0xB4 */ dItemNo_SMELL_CHILDREN_e,
    /* 0xB5 */ dItemNo_SMELL_MEDICINE_e,
    /* 0xB6 */ dItemNo_NOENTRY_182_e,
    /* 0xB7 */ dItemNo_NOENTRY_183_e,
    /* 0xB8 */ dItemNo_NOENTRY_184_e,
    /* 0xB9 */ dItemNo_NOENTRY_185_e,
    /* 0xBA */ dItemNo_NOENTRY_186_e,
    /* 0xBB */ dItemNo_NOENTRY_187_e,
    /* 0xBC */ dItemNo_NOENTRY_188_e,
    /* 0xBD */ dItemNo_NOENTRY_189_e,
    /* 0xBE */ dItemNo_NOENTRY_190_e,
    /* 0xBF */ dItemNo_NOENTRY_191_e,
    /* 0xC0 */ dItemNo_M_BEETLE_e,
    /* 0xC1 */ dItemNo_F_BEETLE_e,
    /* 0xC2 */ dItemNo_M_BUTTERFLY_e,
    /* 0xC3 */ dItemNo_F_BUTTERFLY_e,
    /* 0xC4 */ dItemNo_M_STAG_BEETLE_e,
    /* 0xC5 */ dItemNo_F_STAG_BEETLE_e,
    /* 0xC6 */ dItemNo_M_GRASSHOPPER_e,
    /* 0xC7 */ dItemNo_F_GRASSHOPPER_e,
    /* 0xC8 */ dItemNo_M_NANAFUSHI_e,
    /* 0xC9 */ dItemNo_F_NANAFUSHI_e,
    /* 0xCA */ dItemNo_M_DANGOMUSHI_e,
    /* 0xCB */ dItemNo_F_DANGOMUSHI_e,
    /* 0xCC */ dItemNo_M_MANTIS_e,
    /* 0xCD */ dItemNo_F_MANTIS_e,
    /* 0xCE */ dItemNo_M_LADYBUG_e,
    /* 0xCF */ dItemNo_F_LADYBUG_e,
    /* 0xD0 */ dItemNo_M_SNAIL_e,
    /* 0xD1 */ dItemNo_F_SNAIL_e,
    /* 0xD2 */ dItemNo_M_DRAGONFLY_e,
    /* 0xD3 */ dItemNo_F_DRAGONFLY_e,
    /* 0xD4 */ dItemNo_M_ANT_e,
    /* 0xD5 */ dItemNo_F_ANT_e,
    /* 0xD6 */ dItemNo_M_MAYFLY_e,
    /* 0xD7 */ dItemNo_F_MAYFLY_e,
    /* 0xD8 */ dItemNo_NOENTRY_216_e,
    /* 0xD9 */ dItemNo_NOENTRY_217_e,
    /* 0xDA */ dItemNo_NOENTRY_218_e,
    /* 0xDB */ dItemNo_NOENTRY_219_e,
    /* 0xDC */ dItemNo_NOENTRY_220_e,
    /* 0xDD */ dItemNo_NOENTRY_221_e,
    /* 0xDE */ dItemNo_NOENTRY_222_e,
    /* 0xDF */ dItemNo_NOENTRY_223_e,
    /* 0xE0 */ dItemNo_POU_SPIRIT_e,
    /* 0xE1 */ dItemNo_NOENTRY_225_e,
    /* 0xE2 */ dItemNo_NOENTRY_226_e,
    /* 0xE3 */ dItemNo_NOENTRY_227_e,
    /* 0xE4 */ dItemNo_NOENTRY_228_e,
    /* 0xE5 */ dItemNo_NOENTRY_229_e,
    /* 0xE6 */ dItemNo_NOENTRY_230_e,
    /* 0xE7 */ dItemNo_NOENTRY_231_e,
    /* 0xE8 */ dItemNo_NOENTRY_232_e,
    /* 0xE9 */ dItemNo_ANCIENT_DOCUMENT_e,
    /* 0xEA */ dItemNo_AIR_LETTER_e,
    /* 0xEB */ dItemNo_ANCIENT_DOCUMENT2_e,
    /* 0xEC */ dItemNo_LV7_DUNGEON_EXIT_e,
    /* 0xED */ dItemNo_LINKS_SAVINGS_e,
    /* 0xEE */ dItemNo_SMALL_KEY2_e,
    /* 0xEF */ dItemNo_POU_FIRE1_e,
    /* 0xF0 */ dItemNo_POU_FIRE2_e,
    /* 0xF1 */ dItemNo_POU_FIRE3_e,
    /* 0xF2 */ dItemNo_POU_FIRE4_e,
    /* 0xF3 */ dItemNo_BOSSRIDER_KEY_e,
    /* 0xF4 */ dItemNo_TOMATO_PUREE_e,
    /* 0xF5 */ dItemNo_TASTE_e,
    /* 0xF6 */ dItemNo_LV5_BOSS_KEY_e,
    /* 0xF7 */ dItemNo_SURFBOARD_e,
    /* 0xF8 */ dItemNo_KANTERA2_e,
    /* 0xF9 */ dItemNo_L2_KEY_PIECES1_e,
    /* 0xFA */ dItemNo_L2_KEY_PIECES2_e,
    /* 0xFB */ dItemNo_L2_KEY_PIECES3_e,
    /* 0xFC */ dItemNo_KEY_OF_CARAVAN_e,
    /* 0xFD */ dItemNo_LV2_BOSS_KEY_e,
    /* 0xFE */ dItemNo_KEY_OF_FILONE_e,
    /* 0xFF */ dItemNo_NONE_e,
};

#endif /* D_D_ITEM_DATA_H */
