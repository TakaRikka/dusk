#ifndef RES_E_KC_H
#define RES_E_KC_H

enum dRes_INDEX_E_KC {
    /* BCK  */
    dRes_INDEX_E_KC_BCK_KC_CRASH_e=0x4,
    dRes_INDEX_E_KC_BCK_KC_CRASH2_e=0x5,
    dRes_INDEX_E_KC_BCK_KC_FLY_e=0x6,
    dRes_INDEX_E_KC_BCK_KC_FLY_BRAKE_e=0x7,
    dRes_INDEX_E_KC_BCK_KC_FLY_DASH_WL_e=0x8,
    dRes_INDEX_E_KC_BCK_KC_FLY_GLIDE_e=0x9,
    dRes_INDEX_E_KC_BCK_KC_FLY_GLIDE_WL_e=0xA,
    dRes_INDEX_E_KC_BCK_KC_FLY_LIMIT_WL_e=0xB,
    dRes_INDEX_E_KC_BCK_KC_FLY_WL_e=0xC,
    dRes_INDEX_E_KC_BCK_KC_HOVERING_e=0xD,
    /* BMDR */
    dRes_INDEX_E_KC_BMD_KC_e=0x10,
};

enum dRes_ID_E_KC {
    /* BCK  */
    dRes_ID_E_KC_BCK_KC_CRASH_e=0x4,
    dRes_ID_E_KC_BCK_KC_CRASH2_e=0x5,
    dRes_ID_E_KC_BCK_KC_FLY_e=0x6,
    dRes_ID_E_KC_BCK_KC_FLY_BRAKE_e=0x7,
    dRes_ID_E_KC_BCK_KC_FLY_DASH_WL_e=0x8,
    dRes_ID_E_KC_BCK_KC_FLY_GLIDE_e=0x9,
    dRes_ID_E_KC_BCK_KC_FLY_GLIDE_WL_e=0xA,
    dRes_ID_E_KC_BCK_KC_FLY_LIMIT_WL_e=0xB,
    dRes_ID_E_KC_BCK_KC_FLY_WL_e=0xC,
    dRes_ID_E_KC_BCK_KC_HOVERING_e=0xD,
    /* BMDR */
    dRes_ID_E_KC_BMD_KC_e=0x10,
};

enum KC_JNT {
    KC_JNT_WORLD_ROOT_e=0x0,
    KC_JNT_BACKBONE1_e=0x1,
    KC_JNT_BACKBONE2_e=0x2,
    KC_JNT_NECK1_e=0x3,
    KC_JNT_MIDONA_LOC_e=0x4,
    KC_JNT_NECK2_e=0x5,
    KC_JNT_HEAD_e=0x6,
    KC_JNT_MOUTH1_e=0x7,
    KC_JNT_MOUTH2_e=0x8,
    KC_JNT_THROAT_e=0x9,
    KC_JNT_SHOULDERL_1_e=0xA,
    KC_JNT_SHOULDERL_2_e=0xB,
    KC_JNT_HANDL_1_e=0xC,
    KC_JNT_HANDL_2_e=0xD,
    KC_JNT_HANDL_3_e=0xE,
    KC_JNT_SHOULDERR_1_e=0xF,
    KC_JNT_SHOULDERR_2_e=0x10,
    KC_JNT_HANDR_1_e=0x11,
    KC_JNT_HANDR_2_e=0x12,
    KC_JNT_HANDR_3_e=0x13,
    KC_JNT_WAIST_e=0x14,
    KC_JNT_LEGL_1_e=0x15,
    KC_JNT_LEGL_2_e=0x16,
    KC_JNT_LEGL_3_e=0x17,
    KC_JNT_TUMEL_B_e=0x18,
    KC_JNT_TUMEL_F_e=0x19,
    KC_JNT_LEGR_1_e=0x1A,
    KC_JNT_LEGR_2_e=0x1B,
    KC_JNT_LEGR_3_e=0x1C,
    KC_JNT_TUMER_B_e=0x1D,
    KC_JNT_TUMER_F_e=0x1E,
    KC_JNT_TAIL1_e=0x1F,
    KC_JNT_TAIL2_e=0x20,
    KC_JNT_TAIL3_e=0x21,
    KC_JNT_TAIL4_e=0x22,
};

#endif /* !RES_E_KC_H */