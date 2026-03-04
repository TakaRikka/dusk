#ifndef RES_I_ARI_H
#define RES_I_ARI_H

enum dRes_INDEX_I_ARI {
    /* BCK  */
    dRes_INDEX_I_ARI_BCK_ARI_WALK_e=0x6,
    /* BMDR */
    dRes_INDEX_I_ARI_BMD_ARI_M_e=0x9,
    dRes_INDEX_I_ARI_BMD_ARI_O_e=0xA,
    /* BRK  */
    dRes_INDEX_I_ARI_BRK_ARI_M_e=0xD,
    dRes_INDEX_I_ARI_BRK_ARI_O_e=0xE,
    /* BTK  */
    dRes_INDEX_I_ARI_BTK_ARI_M_e=0x11,
    dRes_INDEX_I_ARI_BTK_ARI_O_e=0x12,
};

enum dRes_ID_I_ARI {
    /* BCK  */
    dRes_ID_I_ARI_BCK_ARI_WALK_e=0x6,
    /* BMDR */
    dRes_ID_I_ARI_BMD_ARI_M_e=0x9,
    dRes_ID_I_ARI_BMD_ARI_O_e=0xA,
    /* BRK  */
    dRes_ID_I_ARI_BRK_ARI_M_e=0xD,
    dRes_ID_I_ARI_BRK_ARI_O_e=0xE,
    /* BTK  */
    dRes_ID_I_ARI_BTK_ARI_M_e=0x11,
    dRes_ID_I_ARI_BTK_ARI_O_e=0x12,
};

enum ARI_M_JNT {
    ARI_M_JNT_CENTER_e=0x0,
    ARI_M_JNT_LEG1_e=0x1,
    ARI_M_JNT_LEG2_e=0x2,
};

enum ARI_O_JNT {
    ARI_O_JNT_CENTER_e=0x0,
    ARI_O_JNT_LEG1_e=0x1,
    ARI_O_JNT_LEG2_e=0x2,
};

#endif /* !RES_I_ARI_H */