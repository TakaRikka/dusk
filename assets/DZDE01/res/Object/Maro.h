#ifndef RES_MARO_H
#define RES_MARO_H

enum dRes_INDEX_MARO {
    /* BCK  */
    dRes_INDEX_MARO_BCK_MARO_F_TALK_A_e=0x6,
    dRes_INDEX_MARO_BCK_MARO_STEP_e=0x7,
    dRes_INDEX_MARO_BCK_MARO_WAIT_A_e=0x8,
    /* BMDR */
    dRes_INDEX_MARO_BMD_MARO_e=0xB,
    /* BTK  */
    dRes_INDEX_MARO_BTK_MARO_e=0xE,
    /* BTP  */
    dRes_INDEX_MARO_BTP_MARO_e=0x11,
    dRes_INDEX_MARO_BTP_MARO_F_TALK_A_e=0x12,
};

enum dRes_ID_MARO {
    /* BCK  */
    dRes_ID_MARO_BCK_MARO_F_TALK_A_e=0x6,
    dRes_ID_MARO_BCK_MARO_STEP_e=0x7,
    dRes_ID_MARO_BCK_MARO_WAIT_A_e=0x8,
    /* BMDR */
    dRes_ID_MARO_BMD_MARO_e=0xB,
    /* BTK  */
    dRes_ID_MARO_BTK_MARO_e=0xE,
    /* BTP  */
    dRes_ID_MARO_BTP_MARO_e=0x11,
    dRes_ID_MARO_BTP_MARO_F_TALK_A_e=0x12,
};

enum MARO_JNT {
    MARO_JNT_CENTER_e=0x0,
    MARO_JNT_BACKBONE1_e=0x1,
    MARO_JNT_BACKBONE2_e=0x2,
    MARO_JNT_NECK_e=0x3,
    MARO_JNT_HEAD_e=0x4,
    MARO_JNT_CHIN_e=0x5,
    MARO_JNT_MOUTH_e=0x6,
    MARO_JNT_SHOULDERL_e=0x7,
    MARO_JNT_ARML1_e=0x8,
    MARO_JNT_ARML2_e=0x9,
    MARO_JNT_HANDL_e=0xA,
    MARO_JNT_FINGERL_e=0xB,
    MARO_JNT_SHOULDERR_e=0xC,
    MARO_JNT_ARMR1_e=0xD,
    MARO_JNT_ARMR2_e=0xE,
    MARO_JNT_HANDR_e=0xF,
    MARO_JNT_FINGERR_e=0x10,
    MARO_JNT_WAIST_e=0x11,
    MARO_JNT_LEGL1_e=0x12,
    MARO_JNT_LEGL2_e=0x13,
    MARO_JNT_FOOTL_e=0x14,
    MARO_JNT_LEGR1_e=0x15,
    MARO_JNT_FOOTR_e=0x16,
};

#endif /* !RES_MARO_H */