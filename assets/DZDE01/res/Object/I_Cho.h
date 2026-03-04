#ifndef RES_I_CHO_H
#define RES_I_CHO_H

enum dRes_INDEX_I_CHO {
    /* BCK  */
    dRes_INDEX_I_CHO_BCK_CHO_FLY_e=0x6,
    dRes_INDEX_I_CHO_BCK_CHO_WAIT_e=0x7,
    /* BMDR */
    dRes_INDEX_I_CHO_BMD_CHO_e=0xA,
    /* BRK  */
    dRes_INDEX_I_CHO_BRK_CHO_M_e=0xD,
    dRes_INDEX_I_CHO_BRK_CHO_O_e=0xE,
    /* BTK  */
    dRes_INDEX_I_CHO_BTK_CHO_M_e=0x11,
    dRes_INDEX_I_CHO_BTK_CHO_O_e=0x12,
};

enum dRes_ID_I_CHO {
    /* BCK  */
    dRes_ID_I_CHO_BCK_CHO_FLY_e=0x6,
    dRes_ID_I_CHO_BCK_CHO_WAIT_e=0x7,
    /* BMDR */
    dRes_ID_I_CHO_BMD_CHO_e=0xA,
    /* BRK  */
    dRes_ID_I_CHO_BRK_CHO_M_e=0xD,
    dRes_ID_I_CHO_BRK_CHO_O_e=0xE,
    /* BTK  */
    dRes_ID_I_CHO_BTK_CHO_M_e=0x11,
    dRes_ID_I_CHO_BTK_CHO_O_e=0x12,
};

enum CHO_JNT {
    CHO_JNT_CENTER_e=0x0,
    CHO_JNT_WING1_e=0x1,
    CHO_JNT_WING2_e=0x2,
};

#endif /* !RES_I_CHO_H */