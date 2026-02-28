#ifndef RES_E_TK_H
#define RES_E_TK_H

enum dRes_INDEX_E_TK {
    /* BCK  */
    dRes_INDEX_E_TK_BCK_TK_APPEAR_e=0x4,
    dRes_INDEX_E_TK_BCK_TK_ATTACK_e=0x5,
    dRes_INDEX_E_TK_BCK_TK_DAMAGE_e=0x6,
    dRes_INDEX_E_TK_BCK_TK_HIDE_e=0x7,
    dRes_INDEX_E_TK_BCK_TK_KYORO2_e=0x8,
    dRes_INDEX_E_TK_BCK_TK_SWIM_e=0x9,
    dRes_INDEX_E_TK_BCK_TK_WAIT01_e=0xA,
    /* BMDR */
    dRes_INDEX_E_TK_BMD_EF_TKBALL_e=0xD,
    dRes_INDEX_E_TK_BMD_TK_e=0xE,
};

enum dRes_ID_E_TK {
    /* BCK  */
    dRes_ID_E_TK_BCK_TK_APPEAR_e=0x4,
    dRes_ID_E_TK_BCK_TK_ATTACK_e=0x5,
    dRes_ID_E_TK_BCK_TK_DAMAGE_e=0x6,
    dRes_ID_E_TK_BCK_TK_HIDE_e=0x7,
    dRes_ID_E_TK_BCK_TK_KYORO2_e=0x8,
    dRes_ID_E_TK_BCK_TK_SWIM_e=0x9,
    dRes_ID_E_TK_BCK_TK_WAIT01_e=0xA,
    /* BMDR */
    dRes_ID_E_TK_BMD_EF_TKBALL_e=0xD,
    dRes_ID_E_TK_BMD_TK_e=0xE,
};

enum EF_TKBALL_JNT {
    EF_TKBALL_JNT_EF_TKBALL_A_e=0x0,
};

enum TK_JNT {
    TK_JNT_WORLD_ROOT_e=0x0,
    TK_JNT_BACKBONE_e=0x1,
    TK_JNT_HEAD_e=0x2,
    TK_JNT_AGO_e=0x3,
    TK_JNT_AGO2_e=0x4,
    TK_JNT_EARL1_e=0x5,
    TK_JNT_EARL2_e=0x6,
    TK_JNT_EARR1_e=0x7,
    TK_JNT_EARR2_e=0x8,
    TK_JNT_WAIST_e=0x9,
    TK_JNT_LEGL1_e=0xA,
    TK_JNT_LEGL2_e=0xB,
    TK_JNT_FOOTL1_e=0xC,
    TK_JNT_FOOTL2_e=0xD,
    TK_JNT_LEGR1_e=0xE,
    TK_JNT_LEGR2_e=0xF,
    TK_JNT_FOOTR1_e=0x10,
    TK_JNT_FOOTR2_e=0x11,
    TK_JNT_TAIL1_e=0x12,
    TK_JNT_TAIL2_e=0x13,
    TK_JNT_TAIL3_e=0x14,
    TK_JNT_TAIL4_e=0x15,
};

#endif /* !RES_E_TK_H */