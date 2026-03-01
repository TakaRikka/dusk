#ifndef RES_I_KAM_H
#define RES_I_KAM_H

enum dRes_INDEX_I_KAM {
    /* BCK  */
    dRes_INDEX_I_KAM_BCK_KAM_FLY_e=0x6,
    dRes_INDEX_I_KAM_BCK_KAM_WALK_e=0x7,
    /* BMDR */
    dRes_INDEX_I_KAM_BMD_KAM_e=0xA,
    /* BRK  */
    dRes_INDEX_I_KAM_BRK_KAM_M_e=0xD,
    dRes_INDEX_I_KAM_BRK_KAM_O_e=0xE,
    /* BTK  */
    dRes_INDEX_I_KAM_BTK_KAM_M_e=0x11,
    dRes_INDEX_I_KAM_BTK_KAM_O_e=0x12,
};

enum dRes_ID_I_KAM {
    /* BCK  */
    dRes_ID_I_KAM_BCK_KAM_FLY_e=0x6,
    dRes_ID_I_KAM_BCK_KAM_WALK_e=0x7,
    /* BMDR */
    dRes_ID_I_KAM_BMD_KAM_e=0xA,
    /* BRK  */
    dRes_ID_I_KAM_BRK_KAM_M_e=0xD,
    dRes_ID_I_KAM_BRK_KAM_O_e=0xE,
    /* BTK  */
    dRes_ID_I_KAM_BTK_KAM_M_e=0x11,
    dRes_ID_I_KAM_BTK_KAM_O_e=0x12,
};

enum KAM_JNT {
    KAM_JNT_CENTER_e=0x0,
    KAM_JNT_KAMA_e=0x1,
    KAM_JNT_LEG1_e=0x2,
    KAM_JNT_LEG2_e=0x3,
    KAM_JNT_WING_e=0x4,
};

#endif /* !RES_I_KAM_H */