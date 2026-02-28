#ifndef RES_KAB_M_H
#define RES_KAB_M_H

enum dRes_INDEX_KAB_M {
    /* BCK  */
    dRes_INDEX_KAB_M_BCK_KAB_FLY_e=0x6,
    dRes_INDEX_KAB_M_BCK_KAB_WALK_e=0x7,
    /* BMDR */
    dRes_INDEX_KAB_M_BMD_KAB_M_e=0xA,
    dRes_INDEX_KAB_M_BMD_KAB_O_e=0xB,
    /* BRK  */
    dRes_INDEX_KAB_M_BRK_KAB_M_e=0xE,
    dRes_INDEX_KAB_M_BRK_KAB_O_e=0xF,
    /* BTK  */
    dRes_INDEX_KAB_M_BTK_KAB_M_e=0x12,
    dRes_INDEX_KAB_M_BTK_KAB_O_e=0x13,
};

enum dRes_ID_KAB_M {
    /* BCK  */
    dRes_ID_KAB_M_BCK_KAB_FLY_e=0x6,
    dRes_ID_KAB_M_BCK_KAB_WALK_e=0x7,
    /* BMDR */
    dRes_ID_KAB_M_BMD_KAB_M_e=0xA,
    dRes_ID_KAB_M_BMD_KAB_O_e=0xB,
    /* BRK  */
    dRes_ID_KAB_M_BRK_KAB_M_e=0xE,
    dRes_ID_KAB_M_BRK_KAB_O_e=0xF,
    /* BTK  */
    dRes_ID_KAB_M_BTK_KAB_M_e=0x12,
    dRes_ID_KAB_M_BTK_KAB_O_e=0x13,
};

enum KAB_M_JNT {
    KAB_M_JNT_CENTER_e=0x0,
    KAB_M_JNT_LEG1_e=0x1,
    KAB_M_JNT_LEG2_e=0x2,
    KAB_M_JNT_WING1_e=0x3,
    KAB_M_JNT_WING2_e=0x4,
};

enum KAB_O_JNT {
    KAB_O_JNT_CENTER_e=0x0,
    KAB_O_JNT_LEG1_e=0x1,
    KAB_O_JNT_LEG2_e=0x2,
    KAB_O_JNT_WING1_e=0x3,
    KAB_O_JNT_WING2_e=0x4,
};

#endif /* !RES_KAB_M_H */