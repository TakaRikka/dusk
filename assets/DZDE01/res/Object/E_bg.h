#ifndef RES_E_BG_H
#define RES_E_BG_H

enum dRes_INDEX_E_BG {
    /* BCK  */
    dRes_INDEX_E_BG_BCK_BG_BOMPOSE_e=0x5,
    dRes_INDEX_E_BG_BCK_BG_DAMAGE_e=0x6,
    dRes_INDEX_E_BG_BCK_BG_NORMALPOSE_e=0x7,
    /* BMDR */
    dRes_INDEX_E_BG_BMD_BG_e=0xA,
    /* BRK  */
    dRes_INDEX_E_BG_BRK_BG_DEATH_e=0xD,
};

enum dRes_ID_E_BG {
    /* BCK  */
    dRes_ID_E_BG_BCK_BG_BOMPOSE_e=0x5,
    dRes_ID_E_BG_BCK_BG_DAMAGE_e=0x6,
    dRes_ID_E_BG_BCK_BG_NORMALPOSE_e=0x7,
    /* BMDR */
    dRes_ID_E_BG_BMD_BG_e=0xA,
    /* BRK  */
    dRes_ID_E_BG_BRK_BG_DEATH_e=0xD,
};

enum BG_JNT {
    BG_JNT_WORLD_ROOT_e=0x0,
    BG_JNT_HEAD01_e=0x1,
    BG_JNT_TAIL_01_e=0x2,
    BG_JNT_TAIL_02_e=0x3,
    BG_JNT_TAIL_03_e=0x4,
};

#endif /* !RES_E_BG_H */