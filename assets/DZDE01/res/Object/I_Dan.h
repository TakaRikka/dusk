#ifndef RES_I_DAN_H
#define RES_I_DAN_H

enum dRes_INDEX_I_DAN {
    /* BCK  */
    dRes_INDEX_I_DAN_BCK_DAN_WAIT_A_e=0x6,
    dRes_INDEX_I_DAN_BCK_DAN_WAIT_B_e=0x7,
    dRes_INDEX_I_DAN_BCK_DAN_WALK_e=0x8,
    /* BMDE */
    dRes_INDEX_I_DAN_BMD_DAN_e=0xB,
    /* BRK  */
    dRes_INDEX_I_DAN_BRK_DAN_M_e=0xE,
    dRes_INDEX_I_DAN_BRK_DAN_O_e=0xF,
    /* BTK  */
    dRes_INDEX_I_DAN_BTK_DAN_M_e=0x12,
    dRes_INDEX_I_DAN_BTK_DAN_O_e=0x13,
};

enum dRes_ID_I_DAN {
    /* BCK  */
    dRes_ID_I_DAN_BCK_DAN_WAIT_A_e=0x6,
    dRes_ID_I_DAN_BCK_DAN_WAIT_B_e=0x7,
    dRes_ID_I_DAN_BCK_DAN_WALK_e=0x8,
    /* BMDE */
    dRes_ID_I_DAN_BMD_DAN_e=0xB,
    /* BRK  */
    dRes_ID_I_DAN_BRK_DAN_M_e=0xE,
    dRes_ID_I_DAN_BRK_DAN_O_e=0xF,
    /* BTK  */
    dRes_ID_I_DAN_BTK_DAN_M_e=0x12,
    dRes_ID_I_DAN_BTK_DAN_O_e=0x13,
};

enum DAN_JNT {
    DAN_JNT_CENTER_e=0x0,
    DAN_JNT_BBF_e=0x1,
    DAN_JNT_LEGF1_e=0x2,
    DAN_JNT_LEGF2_e=0x3,
    DAN_JNT_BBR_e=0x4,
    DAN_JNT_LEGR1_e=0x5,
    DAN_JNT_LEGR2_e=0x6,
};

#endif /* !RES_I_DAN_H */