#ifndef RES_ARU_H
#define RES_ARU_H

enum dRes_INDEX_ARU {
    /* BCK  */
    dRes_INDEX_ARU_BCK_ARU_F_NORMAL_e=0x6,
    dRes_INDEX_ARU_BCK_ARU_FH_NORMAL_e=0x7,
    dRes_INDEX_ARU_BCK_ARU_STEP_R_e=0x8,
    dRes_INDEX_ARU_BCK_ARU_WAIT_A_e=0x9,
    /* BMDR */
    dRes_INDEX_ARU_BMD_ARU_e=0xC,
    /* BTK  */
    dRes_INDEX_ARU_BTK_ARU_e=0xF,
    /* BTP  */
    dRes_INDEX_ARU_BTP_ARU_e=0x12,
    dRes_INDEX_ARU_BTP_ARU_FH_NORMAL_e=0x13,
};

enum dRes_ID_ARU {
    /* BCK  */
    dRes_ID_ARU_BCK_ARU_F_NORMAL_e=0x6,
    dRes_ID_ARU_BCK_ARU_FH_NORMAL_e=0x7,
    dRes_ID_ARU_BCK_ARU_STEP_R_e=0x8,
    dRes_ID_ARU_BCK_ARU_WAIT_A_e=0x9,
    /* BMDR */
    dRes_ID_ARU_BMD_ARU_e=0xC,
    /* BTK  */
    dRes_ID_ARU_BTK_ARU_e=0xF,
    /* BTP  */
    dRes_ID_ARU_BTP_ARU_e=0x12,
    dRes_ID_ARU_BTP_ARU_FH_NORMAL_e=0x13,
};

enum ARU_JNT {
    ARU_JNT_CENTER_e=0x0,
    ARU_JNT_BACKBONE1_e=0x1,
    ARU_JNT_BACKBONE2_e=0x2,
    ARU_JNT_NECK_e=0x3,
    ARU_JNT_HEAD_e=0x4,
    ARU_JNT_CHIN_e=0x5,
    ARU_JNT_MAYU_L_e=0x6,
    ARU_JNT_MAYU_R_e=0x7,
    ARU_JNT_MOUTH_e=0x8,
    ARU_JNT_SHOULDERL_e=0x9,
    ARU_JNT_ARML1_e=0xA,
    ARU_JNT_ARML2_e=0xB,
    ARU_JNT_HANDL_e=0xC,
    ARU_JNT_FINGERL1_e=0xD,
    ARU_JNT_FINGERL2_e=0xE,
    ARU_JNT_THAMBL_e=0xF,
    ARU_JNT_SHOULDERR_e=0x10,
    ARU_JNT_ARMR1_e=0x11,
    ARU_JNT_ARMR2_e=0x12,
    ARU_JNT_HANDR_e=0x13,
    ARU_JNT_FINGERR1_e=0x14,
    ARU_JNT_FINGERR2_e=0x15,
    ARU_JNT_THAMBR_e=0x16,
    ARU_JNT_WAIST_e=0x17,
    ARU_JNT_LEGL1_e=0x18,
    ARU_JNT_LEGL2_e=0x19,
    ARU_JNT_FOOTL_e=0x1A,
    ARU_JNT_SKIRTL_e=0x1B,
    ARU_JNT_LEGR1_e=0x1C,
    ARU_JNT_LEGR2_e=0x1D,
    ARU_JNT_FOOTR_e=0x1E,
    ARU_JNT_SKIRTR_e=0x1F,
    ARU_JNT_SKIRT1_e=0x20,
    ARU_JNT_SKIRT2_e=0x21,
};

#endif /* !RES_ARU_H */