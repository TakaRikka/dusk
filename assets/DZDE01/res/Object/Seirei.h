#ifndef RES_SEIREI_H
#define RES_SEIREI_H

enum dRes_INDEX_SEIREI {
    /* BCK  */
    dRes_INDEX_SEIREI_BCK_SEIA_WAIT_A_e=0x6,
    dRes_INDEX_SEIREI_BCK_SEIA_WAIT_B_e=0x7,
    /* BMDE */
    dRes_INDEX_SEIREI_BMD_SEIA_e=0xA,
    /* BRK  */
    dRes_INDEX_SEIREI_BRK_SEIA_e=0xD,
    /* BTK  */
    dRes_INDEX_SEIREI_BTK_SEIA_e=0x10,
};

enum dRes_ID_SEIREI {
    /* BCK  */
    dRes_ID_SEIREI_BCK_SEIA_WAIT_A_e=0x6,
    dRes_ID_SEIREI_BCK_SEIA_WAIT_B_e=0x7,
    /* BMDE */
    dRes_ID_SEIREI_BMD_SEIA_e=0xA,
    /* BRK  */
    dRes_ID_SEIREI_BRK_SEIA_e=0xD,
    /* BTK  */
    dRes_ID_SEIREI_BTK_SEIA_e=0x10,
};

enum SEIA_JNT {
    SEIA_JNT_CENTER_e=0x0,
    SEIA_JNT_BACKBONE_1_e=0x1,
    SEIA_JNT_BACKBONE_2_e=0x2,
    SEIA_JNT_FL_1_e=0x3,
    SEIA_JNT_FL_2_e=0x4,
    SEIA_JNT_FL_FOOT_1_e=0x5,
    SEIA_JNT_FL_FOOT_2_e=0x6,
    SEIA_JNT_FR_1_e=0x7,
    SEIA_JNT_FR_2_e=0x8,
    SEIA_JNT_FR_FOOT_1_e=0x9,
    SEIA_JNT_FR_FOOT_2_e=0xA,
    SEIA_JNT_NECK_e=0xB,
    SEIA_JNT_HEAD_e=0xC,
    SEIA_JNT_CHIN_1_e=0xD,
    SEIA_JNT_SIZUKU_e=0xE,
    SEIA_JNT_WAIST_e=0xF,
    SEIA_JNT_BL_1_e=0x10,
    SEIA_JNT_BL_2_e=0x11,
    SEIA_JNT_BL_3_e=0x12,
    SEIA_JNT_BL_FOOT_e=0x13,
    SEIA_JNT_BR_1_e=0x14,
    SEIA_JNT_BR_2_e=0x15,
    SEIA_JNT_BR_3_e=0x16,
    SEIA_JNT_BR_FOOT_e=0x17,
    SEIA_JNT_TAIL_1_e=0x18,
    SEIA_JNT_TAIL_2_e=0x19,
    SEIA_JNT_TAIL_3_e=0x1A,
    SEIA_JNT_TAIL_4_e=0x1B,
};

#endif /* !RES_SEIREI_H */