#ifndef RES_E_MG_H
#define RES_E_MG_H

enum dRes_INDEX_E_MG {
    /* BCK  */
    dRes_INDEX_E_MG_BCK_MD_DAMAGE_e=0x4,
    dRes_INDEX_E_MG_BCK_MD_DEATH_e=0x5,
    dRes_INDEX_E_MG_BCK_MD_DEATH_PRESS_e=0x6,
    dRes_INDEX_E_MG_BCK_MD_JUMP_e=0x7,
    dRes_INDEX_E_MG_BCK_MD_PIKU_e=0x8,
    dRes_INDEX_E_MG_BCK_MD_UP_e=0x9,
    dRes_INDEX_E_MG_BCK_MD_WALK_e=0xA,
    /* BMDV */
    dRes_INDEX_E_MG_BMD_MD_e=0xD,
};

enum dRes_ID_E_MG {
    /* BCK  */
    dRes_ID_E_MG_BCK_MD_DAMAGE_e=0x4,
    dRes_ID_E_MG_BCK_MD_DEATH_e=0x5,
    dRes_ID_E_MG_BCK_MD_DEATH_PRESS_e=0x6,
    dRes_ID_E_MG_BCK_MD_JUMP_e=0x7,
    dRes_ID_E_MG_BCK_MD_PIKU_e=0x8,
    dRes_ID_E_MG_BCK_MD_UP_e=0x9,
    dRes_ID_E_MG_BCK_MD_WALK_e=0xA,
    /* BMDV */
    dRes_ID_E_MG_BMD_MD_e=0xD,
};

enum MD_JNT {
    MD_JNT_WORLD_ROOT_e=0x0,
    MD_JNT_BELLY_e=0x1,
    MD_JNT_HEAD_e=0x2,
    MD_JNT_AGOL_e=0x3,
    MD_JNT_AGOR_e=0x4,
    MD_JNT_LEGL1A_e=0x5,
    MD_JNT_LEGL1B_e=0x6,
    MD_JNT_LEGL1C_e=0x7,
    MD_JNT_LEGL2A_e=0x8,
    MD_JNT_LEGL2B_e=0x9,
    MD_JNT_LEGL2C_e=0xA,
    MD_JNT_LEGR1A_e=0xB,
    MD_JNT_LEGR1B_e=0xC,
    MD_JNT_LEGR1C_e=0xD,
    MD_JNT_LEGR2A_e=0xE,
    MD_JNT_LEGR2B_e=0xF,
    MD_JNT_LEGR2C_e=0x10,
};

#endif /* !RES_E_MG_H */