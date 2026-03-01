#ifndef RES_B_GOS_H
#define RES_B_GOS_H

enum dRes_INDEX_B_GOS {
    /* BCK  */
    dRes_INDEX_B_GOS_BCK_GRA_RUN_A_e=0x4,
    dRes_INDEX_B_GOS_BCK_GRA_TO_STONE_NORMAL_e=0x5,
    dRes_INDEX_B_GOS_BCK_GRA_WAIT_A_e=0x6,
    /* BMDR */
    dRes_INDEX_B_GOS_BMD_GRA_A_e=0x9,
};

enum dRes_ID_B_GOS {
    /* BCK  */
    dRes_ID_B_GOS_BCK_GRA_RUN_A_e=0x4,
    dRes_ID_B_GOS_BCK_GRA_TO_STONE_NORMAL_e=0x5,
    dRes_ID_B_GOS_BCK_GRA_WAIT_A_e=0x6,
    /* BMDR */
    dRes_ID_B_GOS_BMD_GRA_A_e=0x9,
};

enum GRA_A_JNT {
    GRA_A_JNT_CENTER_e=0x0,
    GRA_A_JNT_BACKBONE1_e=0x1,
    GRA_A_JNT_BACKBONE2_e=0x2,
    GRA_A_JNT_NECK_e=0x3,
    GRA_A_JNT_HEAD_e=0x4,
    GRA_A_JNT_CHIN_e=0x5,
    GRA_A_JNT_MAYU_e=0x6,
    GRA_A_JNT_MOUTH_e=0x7,
    GRA_A_JNT_SHOULDERL_e=0x8,
    GRA_A_JNT_ARML1_e=0x9,
    GRA_A_JNT_ARML2_e=0xA,
    GRA_A_JNT_HANDL_e=0xB,
    GRA_A_JNT_FINGERL1_e=0xC,
    GRA_A_JNT_THUMBL_e=0xD,
    GRA_A_JNT_SHOULDERR_e=0xE,
    GRA_A_JNT_ARMR1_e=0xF,
    GRA_A_JNT_ARMR2_e=0x10,
    GRA_A_JNT_HANDR_e=0x11,
    GRA_A_JNT_FINGER_R1_e=0x12,
    GRA_A_JNT_THUMBR_e=0x13,
    GRA_A_JNT_WAIST_e=0x14,
    GRA_A_JNT_LEGL1_e=0x15,
    GRA_A_JNT_LEGL2_e=0x16,
    GRA_A_JNT_FOOTL_e=0x17,
    GRA_A_JNT_LEGR1_e=0x18,
    GRA_A_JNT_LEGR2_e=0x19,
    GRA_A_JNT_FOOTR_e=0x1A,
};

#endif /* !RES_B_GOS_H */