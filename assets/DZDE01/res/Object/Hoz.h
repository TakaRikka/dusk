#ifndef RES_HOZ_H
#define RES_HOZ_H

enum dRes_INDEX_HOZ {
    /* BCK  */
    dRes_INDEX_HOZ_BCK_HOZ_F_HAIRWAIT_e=0x7,
    dRes_INDEX_HOZ_BCK_HOZ_F_TALK_A_e=0x8,
    dRes_INDEX_HOZ_BCK_HOZ_TALK_A_e=0x9,
    dRes_INDEX_HOZ_BCK_HOZ_WAIT_A_e=0xA,
    /* BMDR */
    dRes_INDEX_HOZ_BMD_HOZ_e=0xD,
    /* BTK  */
    dRes_INDEX_HOZ_BTK_HOZ_e=0x10,
    /* BTP  */
    dRes_INDEX_HOZ_BTP_HOZ_e=0x13,
    /* EVT  */
    dRes_INDEX_HOZ_DAT_EVENT_LIST_e=0x16,
};

enum dRes_ID_HOZ {
    /* BCK  */
    dRes_ID_HOZ_BCK_HOZ_F_HAIRWAIT_e=0x7,
    dRes_ID_HOZ_BCK_HOZ_F_TALK_A_e=0x8,
    dRes_ID_HOZ_BCK_HOZ_TALK_A_e=0x9,
    dRes_ID_HOZ_BCK_HOZ_WAIT_A_e=0xA,
    /* BMDR */
    dRes_ID_HOZ_BMD_HOZ_e=0xD,
    /* BTK  */
    dRes_ID_HOZ_BTK_HOZ_e=0x10,
    /* BTP  */
    dRes_ID_HOZ_BTP_HOZ_e=0x13,
    /* EVT  */
    dRes_ID_HOZ_DAT_EVENT_LIST_e=0x16,
};

enum HOZ_JNT {
    HOZ_JNT_CENTER_e=0x0,
    HOZ_JNT_BACKBORN1_e=0x1,
    HOZ_JNT_BACKBORN2_e=0x2,
    HOZ_JNT_NECK_e=0x3,
    HOZ_JNT_HEAD_e=0x4,
    HOZ_JNT_CHIN_e=0x5,
    HOZ_JNT_HAIR_e=0x6,
    HOZ_JNT_MAYU_L_e=0x7,
    HOZ_JNT_MAYU_R_e=0x8,
    HOZ_JNT_MOUTH_e=0x9,
    HOZ_JNT_SHOULDER_L_e=0xA,
    HOZ_JNT_ARM1_L_e=0xB,
    HOZ_JNT_ARM2_L_e=0xC,
    HOZ_JNT_HAND_L_e=0xD,
    HOZ_JNT_FINGER_L_e=0xE,
    HOZ_JNT_THUMB_L_e=0xF,
    HOZ_JNT_SHOULDER_R_e=0x10,
    HOZ_JNT_ARM1_R_e=0x11,
    HOZ_JNT_ARM2_R_e=0x12,
    HOZ_JNT_HAND_R_e=0x13,
    HOZ_JNT_FINGER_R_e=0x14,
    HOZ_JNT_THUMB_R_e=0x15,
    HOZ_JNT_SUSPENDER_e=0x16,
    HOZ_JNT_WAIST_e=0x17,
    HOZ_JNT_LEG1_L_e=0x18,
    HOZ_JNT_LEG2_L_e=0x19,
    HOZ_JNT_FOOT_L_e=0x1A,
    HOZ_JNT_LEG1_R_e=0x1B,
    HOZ_JNT_LEG2_R_e=0x1C,
    HOZ_JNT_FOOT_R_e=0x1D,
};

#endif /* !RES_HOZ_H */