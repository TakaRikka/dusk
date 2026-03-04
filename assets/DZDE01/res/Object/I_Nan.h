#ifndef RES_I_NAN_H
#define RES_I_NAN_H

enum dRes_INDEX_I_NAN {
    /* BCK  */
    dRes_INDEX_I_NAN_BCK_NAN_WALK_e=0x6,
    /* BMDE */
    dRes_INDEX_I_NAN_BMD_NAN_e=0x9,
    /* BRK  */
    dRes_INDEX_I_NAN_BRK_NAN_M_e=0xC,
    dRes_INDEX_I_NAN_BRK_NAN_O_e=0xD,
    /* BTK  */
    dRes_INDEX_I_NAN_BTK_NAN_M_e=0x10,
    dRes_INDEX_I_NAN_BTK_NAN_O_e=0x11,
};

enum dRes_ID_I_NAN {
    /* BCK  */
    dRes_ID_I_NAN_BCK_NAN_WALK_e=0x6,
    /* BMDE */
    dRes_ID_I_NAN_BMD_NAN_e=0x9,
    /* BRK  */
    dRes_ID_I_NAN_BRK_NAN_M_e=0xC,
    dRes_ID_I_NAN_BRK_NAN_O_e=0xD,
    /* BTK  */
    dRes_ID_I_NAN_BTK_NAN_M_e=0x10,
    dRes_ID_I_NAN_BTK_NAN_O_e=0x11,
};

enum NAN_JNT {
    NAN_JNT_CENTER_e=0x0,
    NAN_JNT_LEG1_e=0x1,
    NAN_JNT_LEG2_e=0x2,
};

#endif /* !RES_I_NAN_H */