#ifndef RES_TOMBO_H
#define RES_TOMBO_H

enum dRes_INDEX_TOMBO {
    /* BCK  */
    dRes_INDEX_TOMBO_BCK_TON_FLY_e=0x6,
    /* BMDR */
    dRes_INDEX_TOMBO_BMD_TON_e=0x9,
    /* BRK  */
    dRes_INDEX_TOMBO_BRK_TON_M_e=0xC,
    dRes_INDEX_TOMBO_BRK_TON_O_e=0xD,
    /* BTK  */
    dRes_INDEX_TOMBO_BTK_TON_M_e=0x10,
    dRes_INDEX_TOMBO_BTK_TON_O_e=0x11,
};

enum dRes_ID_TOMBO {
    /* BCK  */
    dRes_ID_TOMBO_BCK_TON_FLY_e=0x6,
    /* BMDR */
    dRes_ID_TOMBO_BMD_TON_e=0x9,
    /* BRK  */
    dRes_ID_TOMBO_BRK_TON_M_e=0xC,
    dRes_ID_TOMBO_BRK_TON_O_e=0xD,
    /* BTK  */
    dRes_ID_TOMBO_BTK_TON_M_e=0x10,
    dRes_ID_TOMBO_BTK_TON_O_e=0x11,
};

enum TON_JNT {
    TON_JNT_CENTER_e=0x0,
    TON_JNT_WING1_e=0x1,
    TON_JNT_WING2_e=0x2,
};

#endif /* !RES_TOMBO_H */