#ifndef RES_E_BS_H
#define RES_E_BS_H

enum dRes_INDEX_E_BS {
    /* BCK  */
    dRes_INDEX_E_BS_BCK_BS_APPEAR_e=0x4,
    dRes_INDEX_E_BS_BCK_BS_ATTACK_e=0x5,
    dRes_INDEX_E_BS_BCK_BS_DAMAGE_e=0x6,
    dRes_INDEX_E_BS_BCK_BS_WAIT01_e=0x7,
    dRes_INDEX_E_BS_BCK_BS_WAIT02_e=0x8,
    dRes_INDEX_E_BS_BCK_BS_WALK_e=0x9,
    dRes_INDEX_E_BS_BCK_SPEAR_WAIT01_e=0xA,
    dRes_INDEX_E_BS_BCK_SPEAR_WAIT02_e=0xB,
    /* BMDR */
    dRes_INDEX_E_BS_BMD_BS_e=0xE,
    dRes_INDEX_E_BS_BMD_BS_SPEAR_e=0xF,
};

enum dRes_ID_E_BS {
    /* BCK  */
    dRes_ID_E_BS_BCK_BS_APPEAR_e=0x4,
    dRes_ID_E_BS_BCK_BS_ATTACK_e=0x5,
    dRes_ID_E_BS_BCK_BS_DAMAGE_e=0x6,
    dRes_ID_E_BS_BCK_BS_WAIT01_e=0x7,
    dRes_ID_E_BS_BCK_BS_WAIT02_e=0x8,
    dRes_ID_E_BS_BCK_BS_WALK_e=0x9,
    dRes_ID_E_BS_BCK_SPEAR_WAIT01_e=0xA,
    dRes_ID_E_BS_BCK_SPEAR_WAIT02_e=0xB,
    /* BMDR */
    dRes_ID_E_BS_BMD_BS_e=0xE,
    dRes_ID_E_BS_BMD_BS_SPEAR_e=0xF,
};

enum BS_JNT {
    BS_JNT_WORLD_ROOT_e=0x0,
    BS_JNT_BACKBONE1_e=0x1,
    BS_JNT_BACKBONE2_e=0x2,
    BS_JNT_ARML1_e=0x3,
    BS_JNT_ARML2_e=0x4,
    BS_JNT_HANDL_e=0x5,
    BS_JNT_ARMR1_e=0x6,
    BS_JNT_ARMR2_e=0x7,
    BS_JNT_HANDR_e=0x8,
    BS_JNT_NECK_e=0x9,
    BS_JNT_HEAD_e=0xA,
    BS_JNT_CHIN_e=0xB,
    BS_JNT_WAIST_e=0xC,
    BS_JNT_LEGL1_e=0xD,
    BS_JNT_LEGL2_e=0xE,
    BS_JNT_FOOTL_e=0xF,
    BS_JNT_LEGR1_e=0x10,
    BS_JNT_LEGR2_e=0x11,
    BS_JNT_FOOTR_e=0x12,
};

enum BS_SPEAR_JNT {
    BS_SPEAR_JNT_SPEAR_LOC_e=0x0,
    BS_SPEAR_JNT_BS_SPEAR_MODEL_e=0x1,
};

#endif /* !RES_E_BS_H */