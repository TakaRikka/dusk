#ifndef RES_E_GM_H
#define RES_E_GM_H

enum dRes_INDEX_E_GM {
    /* BCK  */
    dRes_INDEX_E_GM_BCK_GB_APPEAR_e=0x5,
    dRes_INDEX_E_GM_BCK_GB_DAMAGE_e=0x6,
    dRes_INDEX_E_GM_BCK_GB_DEAD_e=0x7,
    dRes_INDEX_E_GM_BCK_GB_MOVE_e=0x8,
    dRes_INDEX_E_GM_BCK_GB_WAIT_e=0x9,
    /* BMDE */
    dRes_INDEX_E_GM_BMD_GOMA_EGG_e=0xC,
    /* BMDV */
    dRes_INDEX_E_GM_BMD_GB_e=0xF,
};

enum dRes_ID_E_GM {
    /* BCK  */
    dRes_ID_E_GM_BCK_GB_APPEAR_e=0x5,
    dRes_ID_E_GM_BCK_GB_DAMAGE_e=0x6,
    dRes_ID_E_GM_BCK_GB_DEAD_e=0x7,
    dRes_ID_E_GM_BCK_GB_MOVE_e=0x8,
    dRes_ID_E_GM_BCK_GB_WAIT_e=0x9,
    /* BMDE */
    dRes_ID_E_GM_BMD_GOMA_EGG_e=0xC,
    /* BMDV */
    dRes_ID_E_GM_BMD_GB_e=0xF,
};

enum GOMA_EGG_JNT {
    GOMA_EGG_JNT_GOMA_EGG_MODEL_e=0x0,
};

enum GB_JNT {
    GB_JNT_WORLD_ROOT_e=0x0,
    GB_JNT_BELLY_e=0x1,
    GB_JNT_HEAD_e=0x2,
    GB_JNT_AGOL_e=0x3,
    GB_JNT_AGOR_e=0x4,
    GB_JNT_LEGL1A_e=0x5,
    GB_JNT_LEGL1B_e=0x6,
    GB_JNT_LEGL1C_e=0x7,
    GB_JNT_LEGL2A_e=0x8,
    GB_JNT_LEGL2B_e=0x9,
    GB_JNT_LEGL2C_e=0xA,
    GB_JNT_LEGR1A_e=0xB,
    GB_JNT_LEGR1B_e=0xC,
    GB_JNT_LEGR1C_e=0xD,
    GB_JNT_LEGR2A_e=0xE,
    GB_JNT_LEGR2B_e=0xF,
    GB_JNT_LEGR2C_e=0x10,
};

#endif /* !RES_E_GM_H */