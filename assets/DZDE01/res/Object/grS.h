#ifndef RES_GRS_H
#define RES_GRS_H

enum dRes_INDEX_GRS {
    /* BCK  */
    dRes_INDEX_GRS_BCK_GRS_F_INTENS_e=0x7,
    dRes_INDEX_GRS_BCK_GRS_F_TALK_A_e=0x8,
    dRes_INDEX_GRS_BCK_GRS_FH_INTENS_e=0x9,
    dRes_INDEX_GRS_BCK_GRS_STEP_e=0xA,
    dRes_INDEX_GRS_BCK_GRS_TALK_A_e=0xB,
    dRes_INDEX_GRS_BCK_GRS_TALK_B_e=0xC,
    dRes_INDEX_GRS_BCK_GRS_WAIT_A_e=0xD,
    /* BMDR */
    dRes_INDEX_GRS_BMD_GRS_e=0x10,
    dRes_INDEX_GRS_BMD_GRS_STICK_e=0x11,
    /* BTK  */
    dRes_INDEX_GRS_BTK_GRS_e=0x14,
    /* BTP  */
    dRes_INDEX_GRS_BTP_GRS_e=0x17,
    dRes_INDEX_GRS_BTP_GRS_F_INTENS_e=0x18,
    dRes_INDEX_GRS_BTP_GRS_FH_INTENS_e=0x19,
    /* EVT  */
    dRes_INDEX_GRS_DAT_EVENT_LIST_e=0x1C,
};

enum dRes_ID_GRS {
    /* BCK  */
    dRes_ID_GRS_BCK_GRS_F_INTENS_e=0x7,
    dRes_ID_GRS_BCK_GRS_F_TALK_A_e=0x8,
    dRes_ID_GRS_BCK_GRS_FH_INTENS_e=0x9,
    dRes_ID_GRS_BCK_GRS_STEP_e=0xA,
    dRes_ID_GRS_BCK_GRS_TALK_A_e=0xB,
    dRes_ID_GRS_BCK_GRS_TALK_B_e=0xC,
    dRes_ID_GRS_BCK_GRS_WAIT_A_e=0xD,
    /* BMDR */
    dRes_ID_GRS_BMD_GRS_e=0x10,
    dRes_ID_GRS_BMD_GRS_STICK_e=0x11,
    /* BTK  */
    dRes_ID_GRS_BTK_GRS_e=0x14,
    /* BTP  */
    dRes_ID_GRS_BTP_GRS_e=0x17,
    dRes_ID_GRS_BTP_GRS_F_INTENS_e=0x18,
    dRes_ID_GRS_BTP_GRS_FH_INTENS_e=0x19,
    /* EVT  */
    dRes_ID_GRS_DAT_EVENT_LIST_e=0x1C,
};

enum GRS_JNT {
    GRS_JNT_CENTER_e=0x0,
    GRS_JNT_BACKBONE1_e=0x1,
    GRS_JNT_BACKBONE2_e=0x2,
    GRS_JNT_NECK_e=0x3,
    GRS_JNT_HEAD_e=0x4,
    GRS_JNT_CHIN_e=0x5,
    GRS_JNT_BEARD_e=0x6,
    GRS_JNT_MAYU_e=0x7,
    GRS_JNT_MOUTH_e=0x8,
    GRS_JNT_SHOULDERL_e=0x9,
    GRS_JNT_ARML1_e=0xA,
    GRS_JNT_ARML2_e=0xB,
    GRS_JNT_HANDL_e=0xC,
    GRS_JNT_SHOULDERR_e=0xD,
    GRS_JNT_ARMR1_e=0xE,
    GRS_JNT_ARMR2_e=0xF,
    GRS_JNT_HANDR_e=0x10,
    GRS_JNT_WAIST_e=0x11,
    GRS_JNT_LEGL1_e=0x12,
    GRS_JNT_LEGL2_e=0x13,
    GRS_JNT_FOOTL_e=0x14,
    GRS_JNT_LEGR1_e=0x15,
    GRS_JNT_LEGR2_e=0x16,
    GRS_JNT_FOOTR_e=0x17,
};

enum GRS_STICK_JNT {
    GRS_STICK_JNT_GRS_STICK_e=0x0,
};

#endif /* !RES_GRS_H */