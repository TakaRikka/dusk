#ifndef RES_NPC_INKO_H
#define RES_NPC_INKO_H

enum dRes_INDEX_NPC_INKO {
    /* BCK  */
    dRes_INDEX_NPC_INKO_BCK_INKO_FLY_e=0x4,
    dRes_INDEX_NPC_INKO_BCK_INKO_WAIT_e=0x5,
    /* BMDR */
    dRes_INDEX_NPC_INKO_BMD_INKO_e=0x8,
};

enum dRes_ID_NPC_INKO {
    /* BCK  */
    dRes_ID_NPC_INKO_BCK_INKO_FLY_e=0x4,
    dRes_ID_NPC_INKO_BCK_INKO_WAIT_e=0x5,
    /* BMDR */
    dRes_ID_NPC_INKO_BMD_INKO_e=0x8,
};

enum INKO_JNT {
    INKO_JNT_CENTER_e=0x0,
    INKO_JNT_HEAD_e=0x1,
    INKO_JNT_BILL_e=0x2,
    INKO_JNT_LEG_L_e=0x3,
    INKO_JNT_LEG_R_e=0x4,
    INKO_JNT_WING_L_e=0x5,
    INKO_JNT_WING_R_e=0x6,
};

#endif /* !RES_NPC_INKO_H */