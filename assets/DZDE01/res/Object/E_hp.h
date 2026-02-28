#ifndef RES_E_HP_H
#define RES_E_HP_H

enum dRes_INDEX_E_HP {
    /* BCK  */
    dRes_INDEX_E_HP_BCK_EF_GLOW_e=0x4,
    dRes_INDEX_E_HP_BCK_HP_ATTACK_e=0x5,
    dRes_INDEX_E_HP_BCK_HP_DAMAGE_e=0x6,
    dRes_INDEX_E_HP_BCK_HP_DEAD_e=0x7,
    dRes_INDEX_E_HP_BCK_HP_DOWN01_e=0x8,
    dRes_INDEX_E_HP_BCK_HP_DOWN02_e=0x9,
    dRes_INDEX_E_HP_BCK_HP_FOUND_e=0xA,
    dRes_INDEX_E_HP_BCK_HP_RETURN_e=0xB,
    dRes_INDEX_E_HP_BCK_HP_SEARCHMOVE_e=0xC,
    dRes_INDEX_E_HP_BCK_HP_WAIT_e=0xD,
    dRes_INDEX_E_HP_BCK_HP_WAIT02_e=0xE,
    dRes_INDEX_E_HP_BCK_HP_WAIT03_e=0xF,
    /* BMDR */
    dRes_INDEX_E_HP_BMD_EF_GLOW_e=0x12,
    dRes_INDEX_E_HP_BMD_HP_e=0x13,
    dRes_INDEX_E_HP_BMD_HP_ORI_e=0x14,
};

enum dRes_ID_E_HP {
    /* BCK  */
    dRes_ID_E_HP_BCK_EF_GLOW_e=0x4,
    dRes_ID_E_HP_BCK_HP_ATTACK_e=0x5,
    dRes_ID_E_HP_BCK_HP_DAMAGE_e=0x6,
    dRes_ID_E_HP_BCK_HP_DEAD_e=0x7,
    dRes_ID_E_HP_BCK_HP_DOWN01_e=0x8,
    dRes_ID_E_HP_BCK_HP_DOWN02_e=0x9,
    dRes_ID_E_HP_BCK_HP_FOUND_e=0xA,
    dRes_ID_E_HP_BCK_HP_RETURN_e=0xB,
    dRes_ID_E_HP_BCK_HP_SEARCHMOVE_e=0xC,
    dRes_ID_E_HP_BCK_HP_WAIT_e=0xD,
    dRes_ID_E_HP_BCK_HP_WAIT02_e=0xE,
    dRes_ID_E_HP_BCK_HP_WAIT03_e=0xF,
    /* BMDR */
    dRes_ID_E_HP_BMD_EF_GLOW_e=0x12,
    dRes_ID_E_HP_BMD_HP_e=0x13,
    dRes_ID_E_HP_BMD_HP_ORI_e=0x14,
};

enum EF_GLOW_JNT {
    EF_GLOW_JNT_EF_GLOW_e=0x0,
};

enum HP_JNT {
    HP_JNT_WORLD_ROOT_e=0x0,
    HP_JNT_BACKBONE_e=0x1,
    HP_JNT_ARML_01_e=0x2,
    HP_JNT_ARML_02_e=0x3,
    HP_JNT_HANDL_e=0x4,
    HP_JNT_ARMR_01_e=0x5,
    HP_JNT_ARMR_02_e=0x6,
    HP_JNT_HANDR_e=0x7,
    HP_JNT_HEAD_e=0x8,
    HP_JNT_HEADB_e=0x9,
    HP_JNT_WAIST_e=0xA,
    HP_JNT_LEG_01_e=0xB,
    HP_JNT_LEG_02_e=0xC,
    HP_JNT_LEG03_e=0xD,
};

enum HP_ORI_JNT {
    HP_ORI_JNT_ORI_01_e=0x0,
    HP_ORI_JNT_ORI_02_e=0x1,
    HP_ORI_JNT_ORI_03_e=0x2,
};

#endif /* !RES_E_HP_H */