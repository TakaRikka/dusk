#ifndef RES_DOC_H
#define RES_DOC_H

enum dRes_INDEX_DOC {
    /* BCK  */
    dRes_INDEX_DOC_BCK_DOC_F_TALK_A_e=0x6,
    dRes_INDEX_DOC_BCK_DOC_GETDOWN_A_e=0x7,
    dRes_INDEX_DOC_BCK_DOC_GETUP_A_e=0x8,
    dRes_INDEX_DOC_BCK_DOC_TALK_A_e=0x9,
    dRes_INDEX_DOC_BCK_DOC_TALK_B_e=0xA,
    dRes_INDEX_DOC_BCK_DOC_WAIT_A_e=0xB,
    /* BMDR */
    dRes_INDEX_DOC_BMD_DOC_e=0xE,
    /* BTK  */
    dRes_INDEX_DOC_BTK_DOC_e=0x11,
    /* BTP  */
    dRes_INDEX_DOC_BTP_DOC_e=0x14,
    dRes_INDEX_DOC_BTP_DOC_TALK_B_e=0x15,
};

enum dRes_ID_DOC {
    /* BCK  */
    dRes_ID_DOC_BCK_DOC_F_TALK_A_e=0x6,
    dRes_ID_DOC_BCK_DOC_GETDOWN_A_e=0x7,
    dRes_ID_DOC_BCK_DOC_GETUP_A_e=0x8,
    dRes_ID_DOC_BCK_DOC_TALK_A_e=0x9,
    dRes_ID_DOC_BCK_DOC_TALK_B_e=0xA,
    dRes_ID_DOC_BCK_DOC_WAIT_A_e=0xB,
    /* BMDR */
    dRes_ID_DOC_BMD_DOC_e=0xE,
    /* BTK  */
    dRes_ID_DOC_BTK_DOC_e=0x11,
    /* BTP  */
    dRes_ID_DOC_BTP_DOC_e=0x14,
    dRes_ID_DOC_BTP_DOC_TALK_B_e=0x15,
};

enum DOC_JNT {
    DOC_JNT_CENTER_e=0x0,
    DOC_JNT_BACKBORN1_e=0x1,
    DOC_JNT_BACKBORN2_e=0x2,
    DOC_JNT_NECK_e=0x3,
    DOC_JNT_HEAD_e=0x4,
    DOC_JNT_CHIN_e=0x5,
    DOC_JNT_MAYU_L_e=0x6,
    DOC_JNT_MAYU_R_e=0x7,
    DOC_JNT_MOUTH_e=0x8,
    DOC_JNT_SHOULDER_L_e=0x9,
    DOC_JNT_ARM1_L_e=0xA,
    DOC_JNT_ARM2_L_e=0xB,
    DOC_JNT_HAND_L_e=0xC,
    DOC_JNT_FINGER_L_e=0xD,
    DOC_JNT_THUMB_L_e=0xE,
    DOC_JNT_SHOULDER_R_e=0xF,
    DOC_JNT_ARM1_R_e=0x10,
    DOC_JNT_ARM2_R_e=0x11,
    DOC_JNT_HAND_R_e=0x12,
    DOC_JNT_FINGER_R_e=0x13,
    DOC_JNT_THUMB_R_e=0x14,
    DOC_JNT_WAIST_e=0x15,
    DOC_JNT_LEG1_L_e=0x16,
    DOC_JNT_LEG2_L_e=0x17,
    DOC_JNT_FOOT_L_e=0x18,
    DOC_JNT_LEG1_R_e=0x19,
    DOC_JNT_LEG2_R_e=0x1A,
    DOC_JNT_FOOT_R_e=0x1B,
};

#endif /* !RES_DOC_H */