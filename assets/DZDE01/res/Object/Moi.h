#ifndef RES_MOI_H
#define RES_MOI_H

enum dRes_INDEX_MOI {
    /* BCK  */
    dRes_INDEX_MOI_BCK_MOI_F_TALK_A_e=0x6,
    dRes_INDEX_MOI_BCK_MOI_STEP_e=0x7,
    dRes_INDEX_MOI_BCK_MOI_WAIT_A_e=0x8,
    /* BMDR */
    dRes_INDEX_MOI_BMD_MOI_e=0xB,
    dRes_INDEX_MOI_BMD_MOI_SAYA_e=0xC,
    dRes_INDEX_MOI_BMD_MOI_SWA_e=0xD,
    /* BTK  */
    dRes_INDEX_MOI_BTK_MOI_e=0x10,
    /* BTP  */
    dRes_INDEX_MOI_BTP_MOI_e=0x13,
    dRes_INDEX_MOI_BTP_MOI_F_TALK_A_e=0x14,
};

enum dRes_ID_MOI {
    /* BCK  */
    dRes_ID_MOI_BCK_MOI_F_TALK_A_e=0x6,
    dRes_ID_MOI_BCK_MOI_STEP_e=0x7,
    dRes_ID_MOI_BCK_MOI_WAIT_A_e=0x8,
    /* BMDR */
    dRes_ID_MOI_BMD_MOI_e=0xB,
    dRes_ID_MOI_BMD_MOI_SAYA_e=0xC,
    dRes_ID_MOI_BMD_MOI_SWA_e=0xD,
    /* BTK  */
    dRes_ID_MOI_BTK_MOI_e=0x10,
    /* BTP  */
    dRes_ID_MOI_BTP_MOI_e=0x13,
    dRes_ID_MOI_BTP_MOI_F_TALK_A_e=0x14,
};

enum MOI_JNT {
    MOI_JNT_CENTER_e=0x0,
    MOI_JNT_BACKBONE1_e=0x1,
    MOI_JNT_BACKBONE2_e=0x2,
    MOI_JNT_NECK_e=0x3,
    MOI_JNT_HEAD_e=0x4,
    MOI_JNT_CHIN_e=0x5,
    MOI_JNT_MAYU_L_e=0x6,
    MOI_JNT_MAYU_R_e=0x7,
    MOI_JNT_MOUTH_e=0x8,
    MOI_JNT_SHOULDERL_e=0x9,
    MOI_JNT_ARML1_e=0xA,
    MOI_JNT_ARML2_e=0xB,
    MOI_JNT_HANDL_e=0xC,
    MOI_JNT_FINGERL_e=0xD,
    MOI_JNT_SHOULDERR_e=0xE,
    MOI_JNT_ARMR1_e=0xF,
    MOI_JNT_ARMR2_e=0x10,
    MOI_JNT_HANDR_e=0x11,
    MOI_JNT_FINGERR_e=0x12,
    MOI_JNT_WAIST_e=0x13,
    MOI_JNT_LEGL1_e=0x14,
    MOI_JNT_LEGL2_e=0x15,
    MOI_JNT_FOOTL_e=0x16,
    MOI_JNT_LEGR1_e=0x17,
    MOI_JNT_LEGR2_e=0x18,
    MOI_JNT_FOOTR_e=0x19,
};

enum MOI_SAYA_JNT {
    MOI_SAYA_JNT_MOI_SAYA_MODEL_e=0x0,
};

enum MOI_SWA_JNT {
    MOI_SWA_JNT_MOI_SWA_e=0x0,
};

#endif /* !RES_MOI_H */