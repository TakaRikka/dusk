#ifndef RES_LEN_H
#define RES_LEN_H

enum dRes_INDEX_LEN {
    /* BCK  */
    dRes_INDEX_LEN_BCK_LEN_F_TALK_A_e=0x6,
    dRes_INDEX_LEN_BCK_LEN_STEP_e=0x7,
    dRes_INDEX_LEN_BCK_LEN_WAIT_A_e=0x8,
    /* BMDR */
    dRes_INDEX_LEN_BMD_LEN_e=0xB,
    /* BTK  */
    dRes_INDEX_LEN_BTK_LEN_e=0xE,
    /* BTP  */
    dRes_INDEX_LEN_BTP_LEN_e=0x11,
};

enum dRes_ID_LEN {
    /* BCK  */
    dRes_ID_LEN_BCK_LEN_F_TALK_A_e=0x6,
    dRes_ID_LEN_BCK_LEN_STEP_e=0x7,
    dRes_ID_LEN_BCK_LEN_WAIT_A_e=0x8,
    /* BMDR */
    dRes_ID_LEN_BMD_LEN_e=0xB,
    /* BTK  */
    dRes_ID_LEN_BTK_LEN_e=0xE,
    /* BTP  */
    dRes_ID_LEN_BTP_LEN_e=0x11,
};

enum LEN_JNT {
    LEN_JNT_CENTER_e=0x0,
    LEN_JNT_BACKBONE1_e=0x1,
    LEN_JNT_BACKBONE2_e=0x2,
    LEN_JNT_NECK_e=0x3,
    LEN_JNT_HEAD_e=0x4,
    LEN_JNT_CHIN_e=0x5,
    LEN_JNT_HAIR_e=0x6,
    LEN_JNT_MAYU_L_e=0x7,
    LEN_JNT_MAYU_R_e=0x8,
    LEN_JNT_MOUTH_e=0x9,
    LEN_JNT_SHOULDERL_e=0xA,
    LEN_JNT_ARML1_e=0xB,
    LEN_JNT_ARML2_e=0xC,
    LEN_JNT_HANDL_e=0xD,
    LEN_JNT_FINGERL_e=0xE,
    LEN_JNT_THAMBL_e=0xF,
    LEN_JNT_SLEEVEL_e=0x10,
    LEN_JNT_FRINGEL_e=0x11,
    LEN_JNT_SHOULDERR_e=0x12,
    LEN_JNT_ARMR1_e=0x13,
    LEN_JNT_ARMR2_e=0x14,
    LEN_JNT_HANDR_e=0x15,
    LEN_JNT_FINGERR_e=0x16,
    LEN_JNT_THAMBR_e=0x17,
    LEN_JNT_SLEEVER_e=0x18,
    LEN_JNT_FRINGER_e=0x19,
    LEN_JNT_WAIST_e=0x1A,
    LEN_JNT_LEGL1_e=0x1B,
    LEN_JNT_LEGL2_e=0x1C,
    LEN_JNT_FOOTL_e=0x1D,
    LEN_JNT_LEGR1_e=0x1E,
    LEN_JNT_LEGR2_e=0x1F,
    LEN_JNT_FOOTR_e=0x20,
    LEN_JNT_SKIRT_B_e=0x21,
    LEN_JNT_SKIRT_F_e=0x22,
};

#endif /* !RES_LEN_H */