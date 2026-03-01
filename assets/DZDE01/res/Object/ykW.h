#ifndef RES_YKW_H
#define RES_YKW_H

enum dRes_INDEX_YKW {
    /* BCK  */
    dRes_INDEX_YKW_BCK_YKW_F_TALK_A_e=0x6,
    dRes_INDEX_YKW_BCK_YKW_STEP_e=0x7,
    dRes_INDEX_YKW_BCK_YKW_WAIT_A_e=0x8,
    /* BMDR */
    dRes_INDEX_YKW_BMD_YKW_e=0xB,
    /* BTK  */
    dRes_INDEX_YKW_BTK_YKW_e=0xE,
    /* BTP  */
    dRes_INDEX_YKW_BTP_YKW_e=0x11,
};

enum dRes_ID_YKW {
    /* BCK  */
    dRes_ID_YKW_BCK_YKW_F_TALK_A_e=0x6,
    dRes_ID_YKW_BCK_YKW_STEP_e=0x7,
    dRes_ID_YKW_BCK_YKW_WAIT_A_e=0x8,
    /* BMDR */
    dRes_ID_YKW_BMD_YKW_e=0xB,
    /* BTK  */
    dRes_ID_YKW_BTK_YKW_e=0xE,
    /* BTP  */
    dRes_ID_YKW_BTP_YKW_e=0x11,
};

enum YKW_JNT {
    YKW_JNT_CENTER_e=0x0,
    YKW_JNT_BB1_e=0x1,
    YKW_JNT_BB2_e=0x2,
    YKW_JNT_BB3_e=0x3,
    YKW_JNT_NECK_e=0x4,
    YKW_JNT_HEAD_e=0x5,
    YKW_JNT_CHIN_e=0x6,
    YKW_JNT_MAYUL_e=0x7,
    YKW_JNT_MAYUR_e=0x8,
    YKW_JNT_MOUTH_e=0x9,
    YKW_JNT_WAIST_e=0xA,
    YKW_JNT_LEGL1_e=0xB,
    YKW_JNT_LEGL2_e=0xC,
    YKW_JNT_FOOTL_e=0xD,
    YKW_JNT_LEGR1_e=0xE,
    YKW_JNT_LEGR2_e=0xF,
    YKW_JNT_FOOTR_e=0x10,
};

#endif /* !RES_YKW_H */