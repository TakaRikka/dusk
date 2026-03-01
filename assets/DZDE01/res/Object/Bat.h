#ifndef RES_BAT_H
#define RES_BAT_H

enum dRes_INDEX_BAT {
    /* BCK  */
    dRes_INDEX_BAT_BCK_BAT_FLY_e=0x6,
    dRes_INDEX_BAT_BCK_BAT_JUMP_e=0x7,
    dRes_INDEX_BAT_BCK_BAT_WAIT_e=0x8,
    /* BMDR */
    dRes_INDEX_BAT_BMD_BAT_e=0xB,
    /* BRK  */
    dRes_INDEX_BAT_BRK_BAT_M_e=0xE,
    dRes_INDEX_BAT_BRK_BAT_O_e=0xF,
    /* BTK  */
    dRes_INDEX_BAT_BTK_BAT_M_e=0x12,
    dRes_INDEX_BAT_BTK_BAT_O_e=0x13,
};

enum dRes_ID_BAT {
    /* BCK  */
    dRes_ID_BAT_BCK_BAT_FLY_e=0x6,
    dRes_ID_BAT_BCK_BAT_JUMP_e=0x7,
    dRes_ID_BAT_BCK_BAT_WAIT_e=0x8,
    /* BMDR */
    dRes_ID_BAT_BMD_BAT_e=0xB,
    /* BRK  */
    dRes_ID_BAT_BRK_BAT_M_e=0xE,
    dRes_ID_BAT_BRK_BAT_O_e=0xF,
    /* BTK  */
    dRes_ID_BAT_BTK_BAT_M_e=0x12,
    dRes_ID_BAT_BTK_BAT_O_e=0x13,
};

enum BAT_JNT {
    BAT_JNT_CENTER_e=0x0,
    BAT_JNT_LEG_e=0x1,
    BAT_JNT_WING1_e=0x2,
    BAT_JNT_WING2_e=0x3,
};

#endif /* !RES_BAT_H */