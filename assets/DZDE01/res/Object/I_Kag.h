#ifndef RES_I_KAG_H
#define RES_I_KAG_H

enum dRes_INDEX_I_KAG {
    /* BCK  */
    dRes_INDEX_I_KAG_BCK_KAG_FLY_e=0x6,
    dRes_INDEX_I_KAG_BCK_KAG_WALK_e=0x7,
    /* BMDE */
    dRes_INDEX_I_KAG_BMD_KAG_e=0xA,
    /* BRK  */
    dRes_INDEX_I_KAG_BRK_KAG_M_e=0xD,
    dRes_INDEX_I_KAG_BRK_KAG_O_e=0xE,
    /* BTK  */
    dRes_INDEX_I_KAG_BTK_KAG_M_e=0x11,
    dRes_INDEX_I_KAG_BTK_KAG_O_e=0x12,
};

enum dRes_ID_I_KAG {
    /* BCK  */
    dRes_ID_I_KAG_BCK_KAG_FLY_e=0x6,
    dRes_ID_I_KAG_BCK_KAG_WALK_e=0x7,
    /* BMDE */
    dRes_ID_I_KAG_BMD_KAG_e=0xA,
    /* BRK  */
    dRes_ID_I_KAG_BRK_KAG_M_e=0xD,
    dRes_ID_I_KAG_BRK_KAG_O_e=0xE,
    /* BTK  */
    dRes_ID_I_KAG_BTK_KAG_M_e=0x11,
    dRes_ID_I_KAG_BTK_KAG_O_e=0x12,
};

enum KAG_JNT {
    KAG_JNT_CENTER_e=0x0,
    KAG_JNT_LEG1_e=0x1,
    KAG_JNT_LEG2_e=0x2,
    KAG_JNT_WING_e=0x3,
};

#endif /* !RES_I_KAG_H */