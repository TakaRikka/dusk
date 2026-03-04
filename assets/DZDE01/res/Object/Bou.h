#ifndef RES_BOU_H
#define RES_BOU_H

enum dRes_INDEX_BOU {
    /* BCK  */
    dRes_INDEX_BOU_BCK_BOU_F_TALK_A_e=0x6,
    dRes_INDEX_BOU_BCK_BOU_STEP_e=0x7,
    dRes_INDEX_BOU_BCK_BOU_WAIT_A_e=0x8,
    /* BMDR */
    dRes_INDEX_BOU_BMD_BOU_e=0xB,
    /* BTK  */
    dRes_INDEX_BOU_BTK_BOU_e=0xE,
    /* BTP  */
    dRes_INDEX_BOU_BTP_BOU_e=0x11,
};

enum dRes_ID_BOU {
    /* BCK  */
    dRes_ID_BOU_BCK_BOU_F_TALK_A_e=0x6,
    dRes_ID_BOU_BCK_BOU_STEP_e=0x7,
    dRes_ID_BOU_BCK_BOU_WAIT_A_e=0x8,
    /* BMDR */
    dRes_ID_BOU_BMD_BOU_e=0xB,
    /* BTK  */
    dRes_ID_BOU_BTK_BOU_e=0xE,
    /* BTP  */
    dRes_ID_BOU_BTP_BOU_e=0x11,
};

enum BOU_JNT {
    BOU_JNT_CENTER_e=0x0,
    BOU_JNT_BACKBONE1_e=0x1,
    BOU_JNT_BACKBONE2_e=0x2,
    BOU_JNT_NECK_e=0x3,
    BOU_JNT_HEAD_e=0x4,
    BOU_JNT_CHIN_e=0x5,
    BOU_JNT_MAYU_L_e=0x6,
    BOU_JNT_MAYU_R_e=0x7,
    BOU_JNT_MOUTH_e=0x8,
    BOU_JNT_SHOULDERL_e=0x9,
    BOU_JNT_ARML1_e=0xA,
    BOU_JNT_ARML2_e=0xB,
    BOU_JNT_HANDL_e=0xC,
    BOU_JNT_FINGERL1_e=0xD,
    BOU_JNT_THAMBL_e=0xE,
    BOU_JNT_SHOULDERR_e=0xF,
    BOU_JNT_ARMR1_e=0x10,
    BOU_JNT_ARMR2_e=0x11,
    BOU_JNT_HANDR_e=0x12,
    BOU_JNT_FINGERR1_e=0x13,
    BOU_JNT_THAMBR_e=0x14,
    BOU_JNT_SKIRT1_e=0x15,
    BOU_JNT_SKIRT2_e=0x16,
    BOU_JNT_SKIRTL1_e=0x17,
    BOU_JNT_SKIRTL2_e=0x18,
    BOU_JNT_SKIRTR1_e=0x19,
    BOU_JNT_SKIRTR2_e=0x1A,
    BOU_JNT_WAIST_e=0x1B,
    BOU_JNT_LEGL1_e=0x1C,
    BOU_JNT_LEGL2_e=0x1D,
    BOU_JNT_FOOTL_e=0x1E,
    BOU_JNT_LEGR1_e=0x1F,
    BOU_JNT_LEGR2_e=0x20,
    BOU_JNT_FOOTR_e=0x21,
};

#endif /* !RES_BOU_H */