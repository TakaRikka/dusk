#ifndef RES_GRZ_H
#define RES_GRZ_H

enum dRes_INDEX_GRZ {
    /* BCK  */
    dRes_INDEX_GRZ_BCK_GRZ_F_MUKIDASHI_e=0x6,
    dRes_INDEX_GRZ_BCK_GRZ_F_TALK_A_e=0x7,
    dRes_INDEX_GRZ_BCK_GRZ_FH_TALK_A_e=0x8,
    dRes_INDEX_GRZ_BCK_GRZ_STEP_e=0x9,
    dRes_INDEX_GRZ_BCK_GRZ_TALK_A_e=0xA,
    dRes_INDEX_GRZ_BCK_GRZ_WAIT_A_e=0xB,
    /* BMDR */
    dRes_INDEX_GRZ_BMD_GRZ_e=0xE,
    /* BTK  */
    dRes_INDEX_GRZ_BTK_GRZ_e=0x11,
    /* BTP  */
    dRes_INDEX_GRZ_BTP_GRZ_e=0x14,
};

enum dRes_ID_GRZ {
    /* BCK  */
    dRes_ID_GRZ_BCK_GRZ_F_MUKIDASHI_e=0x6,
    dRes_ID_GRZ_BCK_GRZ_F_TALK_A_e=0x7,
    dRes_ID_GRZ_BCK_GRZ_FH_TALK_A_e=0x8,
    dRes_ID_GRZ_BCK_GRZ_STEP_e=0x9,
    dRes_ID_GRZ_BCK_GRZ_TALK_A_e=0xA,
    dRes_ID_GRZ_BCK_GRZ_WAIT_A_e=0xB,
    /* BMDR */
    dRes_ID_GRZ_BMD_GRZ_e=0xE,
    /* BTK  */
    dRes_ID_GRZ_BTK_GRZ_e=0x11,
    /* BTP  */
    dRes_ID_GRZ_BTP_GRZ_e=0x14,
};

enum GRZ_JNT {
    GRZ_JNT_CENTER_e=0x0,
    GRZ_JNT_BACKBONE1_e=0x1,
    GRZ_JNT_BACKBONE2_e=0x2,
    GRZ_JNT_NECK_e=0x3,
    GRZ_JNT_HEAD_e=0x4,
    GRZ_JNT_CHIN_e=0x5,
    GRZ_JNT_LIP2_e=0x6,
    GRZ_JNT_LIP1_e=0x7,
    GRZ_JNT_MAYUL_e=0x8,
    GRZ_JNT_MAYUR_e=0x9,
    GRZ_JNT_MOUTH_e=0xA,
    GRZ_JNT_SHOULDERL_e=0xB,
    GRZ_JNT_ARML1_e=0xC,
    GRZ_JNT_ARML2_e=0xD,
    GRZ_JNT_HANDL_e=0xE,
    GRZ_JNT_FINGERL1_e=0xF,
    GRZ_JNT_THAMBL_e=0x10,
    GRZ_JNT_SHOULDERR_e=0x11,
    GRZ_JNT_ARMR1_e=0x12,
    GRZ_JNT_ARMR2_e=0x13,
    GRZ_JNT_HANDR_e=0x14,
    GRZ_JNT_FINGERR1_e=0x15,
    GRZ_JNT_THAMBR_e=0x16,
    GRZ_JNT_WAIST_e=0x17,
    GRZ_JNT_LEGL1_e=0x18,
    GRZ_JNT_LEGL2_e=0x19,
    GRZ_JNT_FOOTL_e=0x1A,
    GRZ_JNT_LEGR1_e=0x1B,
    GRZ_JNT_LEGR2_e=0x1C,
    GRZ_JNT_FOOTR_e=0x1D,
    GRZ_JNT_SKIRT_e=0x1E,
};

#endif /* !RES_GRZ_H */