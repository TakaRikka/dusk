#ifndef RES_GRO_H
#define RES_GRO_H

enum dRes_INDEX_GRO {
    /* BCK  */
    dRes_INDEX_GRO_BCK_GRO_F_TALK_A_e=0x7,
    dRes_INDEX_GRO_BCK_GRO_F_TALK_B_e=0x8,
    dRes_INDEX_GRO_BCK_GRO_F_WAIT_A_e=0x9,
    dRes_INDEX_GRO_BCK_GRO_F_WALK_A_e=0xA,
    dRes_INDEX_GRO_BCK_GRO_STEP_e=0xB,
    dRes_INDEX_GRO_BCK_GRO_TALK_A_e=0xC,
    dRes_INDEX_GRO_BCK_GRO_TALK_B_e=0xD,
    dRes_INDEX_GRO_BCK_GRO_WAIT_A_e=0xE,
    dRes_INDEX_GRO_BCK_GRO_WALK_A_e=0xF,
    /* BMDR */
    dRes_INDEX_GRO_BMD_GRO_A_e=0x12,
    dRes_INDEX_GRO_BMD_GRO_PIPE_e=0x13,
    /* BTK  */
    dRes_INDEX_GRO_BTK_GRO_A_e=0x16,
    /* BTP  */
    dRes_INDEX_GRO_BTP_GRO_A_e=0x19,
    dRes_INDEX_GRO_BTP_GRO_F_TALK_B_e=0x1A,
    /* EVT  */
    dRes_INDEX_GRO_DAT_EVENT_LIST_e=0x1D,
};

enum dRes_ID_GRO {
    /* BCK  */
    dRes_ID_GRO_BCK_GRO_F_TALK_A_e=0x7,
    dRes_ID_GRO_BCK_GRO_F_TALK_B_e=0x8,
    dRes_ID_GRO_BCK_GRO_F_WAIT_A_e=0x9,
    dRes_ID_GRO_BCK_GRO_F_WALK_A_e=0xA,
    dRes_ID_GRO_BCK_GRO_STEP_e=0xB,
    dRes_ID_GRO_BCK_GRO_TALK_A_e=0xC,
    dRes_ID_GRO_BCK_GRO_TALK_B_e=0xD,
    dRes_ID_GRO_BCK_GRO_WAIT_A_e=0xE,
    dRes_ID_GRO_BCK_GRO_WALK_A_e=0xF,
    /* BMDR */
    dRes_ID_GRO_BMD_GRO_A_e=0x12,
    dRes_ID_GRO_BMD_GRO_PIPE_e=0x13,
    /* BTK  */
    dRes_ID_GRO_BTK_GRO_A_e=0x16,
    /* BTP  */
    dRes_ID_GRO_BTP_GRO_A_e=0x19,
    dRes_ID_GRO_BTP_GRO_F_TALK_B_e=0x1A,
    /* EVT  */
    dRes_ID_GRO_DAT_EVENT_LIST_e=0x1D,
};

enum GRO_A_JNT {
    GRO_A_JNT_CENTER_e=0x0,
    GRO_A_JNT_BACKBONE1_e=0x1,
    GRO_A_JNT_BACKBONE2_e=0x2,
    GRO_A_JNT_NECK_e=0x3,
    GRO_A_JNT_HEAD_e=0x4,
    GRO_A_JNT_CHIN_e=0x5,
    GRO_A_JNT_BEARD_e=0x6,
    GRO_A_JNT_HAIRL_e=0x7,
    GRO_A_JNT_HAIRR_e=0x8,
    GRO_A_JNT_MAYU_e=0x9,
    GRO_A_JNT_MOUTH_e=0xA,
    GRO_A_JNT_SHOULDERL_e=0xB,
    GRO_A_JNT_ARML1_e=0xC,
    GRO_A_JNT_ARML2_e=0xD,
    GRO_A_JNT_HANDL_e=0xE,
    GRO_A_JNT_SHOULDERR_e=0xF,
    GRO_A_JNT_ARMR1_e=0x10,
    GRO_A_JNT_ARMR2_e=0x11,
    GRO_A_JNT_HANDR_e=0x12,
    GRO_A_JNT_WAIST_e=0x13,
    GRO_A_JNT_LEGL1_e=0x14,
    GRO_A_JNT_LEGL2_e=0x15,
    GRO_A_JNT_FOOTL_e=0x16,
    GRO_A_JNT_LEGR1_e=0x17,
    GRO_A_JNT_LEGR2_e=0x18,
    GRO_A_JNT_FOOTR_e=0x19,
};

enum GRO_PIPE_JNT {
    GRO_PIPE_JNT_PIPE_e=0x0,
};

#endif /* !RES_GRO_H */