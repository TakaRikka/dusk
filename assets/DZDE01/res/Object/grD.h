#ifndef RES_GRD_H
#define RES_GRD_H

enum dRes_INDEX_GRD {
    /* BCK  */
    dRes_INDEX_GRD_BCK_GRD_F_TALK_A_e=0x6,
    dRes_INDEX_GRD_BCK_GRD_STEP_e=0x7,
    dRes_INDEX_GRD_BCK_GRD_WAIT_A_e=0x8,
    /* BMDR */
    dRes_INDEX_GRD_BMD_GRD_e=0xB,
    /* BTK  */
    dRes_INDEX_GRD_BTK_GRD_e=0xE,
    /* BTP  */
    dRes_INDEX_GRD_BTP_GRD_e=0x11,
};

enum dRes_ID_GRD {
    /* BCK  */
    dRes_ID_GRD_BCK_GRD_F_TALK_A_e=0x6,
    dRes_ID_GRD_BCK_GRD_STEP_e=0x7,
    dRes_ID_GRD_BCK_GRD_WAIT_A_e=0x8,
    /* BMDR */
    dRes_ID_GRD_BMD_GRD_e=0xB,
    /* BTK  */
    dRes_ID_GRD_BTK_GRD_e=0xE,
    /* BTP  */
    dRes_ID_GRD_BTP_GRD_e=0x11,
};

enum GRD_JNT {
    GRD_JNT_CENTER_e=0x0,
    GRD_JNT_BACKBONE1_e=0x1,
    GRD_JNT_BACKBONE2_e=0x2,
    GRD_JNT_NECK_e=0x3,
    GRD_JNT_HEAD_e=0x4,
    GRD_JNT_CHIN_e=0x5,
    GRD_JNT_MAYU_e=0x6,
    GRD_JNT_MOUTH_e=0x7,
    GRD_JNT_SHOULDERL_e=0x8,
    GRD_JNT_ARML1_e=0x9,
    GRD_JNT_ARML2_e=0xA,
    GRD_JNT_HANDL_e=0xB,
    GRD_JNT_FINGERL_e=0xC,
    GRD_JNT_THAMBL_e=0xD,
    GRD_JNT_SHOULDERR_e=0xE,
    GRD_JNT_ARMR1_e=0xF,
    GRD_JNT_ARMR2_e=0x10,
    GRD_JNT_HANDR_e=0x11,
    GRD_JNT_FINGERR_e=0x12,
    GRD_JNT_THAMBR_e=0x13,
    GRD_JNT_WAIST_e=0x14,
    GRD_JNT_FUNDOSHI_e=0x15,
    GRD_JNT_LEGL1_e=0x16,
    GRD_JNT_LEGL2_e=0x17,
    GRD_JNT_FOOTL_e=0x18,
    GRD_JNT_LEGR1_e=0x19,
    GRD_JNT_LEGR2_e=0x1A,
    GRD_JNT_FOOTR_e=0x1B,
};

#endif /* !RES_GRD_H */