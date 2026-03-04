#ifndef RES_I_TEN_H
#define RES_I_TEN_H

enum dRes_INDEX_I_TEN {
    /* BCK  */
    dRes_INDEX_I_TEN_BCK_TEN_FLY_e=0x6,
    dRes_INDEX_I_TEN_BCK_TEN_WALK_e=0x7,
    /* BMDR */
    dRes_INDEX_I_TEN_BMD_TEN_e=0xA,
    /* BRK  */
    dRes_INDEX_I_TEN_BRK_TEN_M_e=0xD,
    dRes_INDEX_I_TEN_BRK_TEN_O_e=0xE,
    /* BTK  */
    dRes_INDEX_I_TEN_BTK_TEN_M_e=0x11,
    dRes_INDEX_I_TEN_BTK_TEN_O_e=0x12,
};

enum dRes_ID_I_TEN {
    /* BCK  */
    dRes_ID_I_TEN_BCK_TEN_FLY_e=0x6,
    dRes_ID_I_TEN_BCK_TEN_WALK_e=0x7,
    /* BMDR */
    dRes_ID_I_TEN_BMD_TEN_e=0xA,
    /* BRK  */
    dRes_ID_I_TEN_BRK_TEN_M_e=0xD,
    dRes_ID_I_TEN_BRK_TEN_O_e=0xE,
    /* BTK  */
    dRes_ID_I_TEN_BTK_TEN_M_e=0x11,
    dRes_ID_I_TEN_BTK_TEN_O_e=0x12,
};

enum TEN_JNT {
    TEN_JNT_CENTER_e=0x0,
    TEN_JNT_LEG1_e=0x1,
    TEN_JNT_LEG2_e=0x2,
    TEN_JNT_WING1L_e=0x3,
    TEN_JNT_WING1R_e=0x4,
    TEN_JNT_WING2_e=0x5,
};

#endif /* !RES_I_TEN_H */