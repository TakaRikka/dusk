#ifndef RES_FR_H
#define RES_FR_H

enum dRes_INDEX_FR {
    /* BCK  */
    dRes_INDEX_FR_BCK_FR_JUMP01_e=0x5,
    dRes_INDEX_FR_BCK_FR_JUMP02_e=0x6,
    dRes_INDEX_FR_BCK_FR_MOVE01_e=0x7,
    dRes_INDEX_FR_BCK_FR_MOVE02_e=0x8,
    dRes_INDEX_FR_BCK_FR_MOVE03_e=0x9,
    dRes_INDEX_FR_BCK_FR_SWIM_e=0xA,
    dRes_INDEX_FR_BCK_FR_WAIT_e=0xB,
    /* BMDR */
    dRes_INDEX_FR_BMD_FR_e=0xE,
    /* BTK  */
    dRes_INDEX_FR_BTK_FR_e=0x11,
};

enum dRes_ID_FR {
    /* BCK  */
    dRes_ID_FR_BCK_FR_JUMP01_e=0x5,
    dRes_ID_FR_BCK_FR_JUMP02_e=0x6,
    dRes_ID_FR_BCK_FR_MOVE01_e=0x7,
    dRes_ID_FR_BCK_FR_MOVE02_e=0x8,
    dRes_ID_FR_BCK_FR_MOVE03_e=0x9,
    dRes_ID_FR_BCK_FR_SWIM_e=0xA,
    dRes_ID_FR_BCK_FR_WAIT_e=0xB,
    /* BMDR */
    dRes_ID_FR_BMD_FR_e=0xE,
    /* BTK  */
    dRes_ID_FR_BTK_FR_e=0x11,
};

enum FR_JNT {
    FR_JNT_WAIST_e=0x0,
    FR_JNT_ARM_e=0x1,
    FR_JNT_ARM_END_e=0x2,
    FR_JNT_LEG_L1_e=0x3,
    FR_JNT_LEG_L2_e=0x4,
    FR_JNT_LEG_LEND_e=0x5,
    FR_JNT_LEG_R_e=0x6,
    FR_JNT_LEG_R2_e=0x7,
    FR_JNT_LEG_REND_e=0x8,
    FR_JNT_NECK_e=0x9,
    FR_JNT_WAIST_END_e=0xA,
};

#endif /* !RES_FR_H */