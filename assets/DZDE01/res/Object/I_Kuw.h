#ifndef RES_I_KUW_H
#define RES_I_KUW_H

enum dRes_INDEX_I_KUW {
    /* BCK  */
    dRes_INDEX_I_KUW_BCK_KUW_FLY_e=0x6,
    dRes_INDEX_I_KUW_BCK_KUW_WALK_e=0x7,
    /* BMDR */
    dRes_INDEX_I_KUW_BMD_KUW_M_e=0xA,
    dRes_INDEX_I_KUW_BMD_KUW_O_e=0xB,
    /* BRK  */
    dRes_INDEX_I_KUW_BRK_KUW_M_e=0xE,
    dRes_INDEX_I_KUW_BRK_KUW_O_e=0xF,
    /* BTK  */
    dRes_INDEX_I_KUW_BTK_KUW_M_e=0x12,
    dRes_INDEX_I_KUW_BTK_KUW_O_e=0x13,
};

enum dRes_ID_I_KUW {
    /* BCK  */
    dRes_ID_I_KUW_BCK_KUW_FLY_e=0x6,
    dRes_ID_I_KUW_BCK_KUW_WALK_e=0x7,
    /* BMDR */
    dRes_ID_I_KUW_BMD_KUW_M_e=0xA,
    dRes_ID_I_KUW_BMD_KUW_O_e=0xB,
    /* BRK  */
    dRes_ID_I_KUW_BRK_KUW_M_e=0xE,
    dRes_ID_I_KUW_BRK_KUW_O_e=0xF,
    /* BTK  */
    dRes_ID_I_KUW_BTK_KUW_M_e=0x12,
    dRes_ID_I_KUW_BTK_KUW_O_e=0x13,
};

enum KUW_M_JNT {
    KUW_M_JNT_CENTER_e=0x0,
    KUW_M_JNT_LEG1_e=0x1,
    KUW_M_JNT_LEG2_e=0x2,
    KUW_M_JNT_WING1_e=0x3,
    KUW_M_JNT_WING2_e=0x4,
};

enum KUW_O_JNT {
    KUW_O_JNT_CENTER_e=0x0,
    KUW_O_JNT_LEG1_e=0x1,
    KUW_O_JNT_LEG2_e=0x2,
    KUW_O_JNT_WING1_e=0x3,
    KUW_O_JNT_WING2_e=0x4,
};

#endif /* !RES_I_KUW_H */