#ifndef RES_E_OC2_H
#define RES_E_OC2_H

enum dRes_INDEX_E_OC2 {
    /* BMDR */
    dRes_INDEX_E_OC2_BMD_OC2_e=0x3,
    dRes_INDEX_E_OC2_BMD_OC2_WEPON_e=0x4,
};

enum dRes_ID_E_OC2 {
    /* BMDR */
    dRes_ID_E_OC2_BMD_OC2_e=0x3,
    dRes_ID_E_OC2_BMD_OC2_WEPON_e=0x4,
};

enum OC2_JNT {
    OC2_JNT_WORLD_ROOT_e=0x0,
    OC2_JNT_BACK_BONE1_e=0x1,
    OC2_JNT_BACK_BONE2_e=0x2,
    OC2_JNT_ARM_L1_e=0x3,
    OC2_JNT_ARM_L2_e=0x4,
    OC2_JNT_ARM_L3_e=0x5,
    OC2_JNT_HAND_L1_e=0x6,
    OC2_JNT_FINGER_L1_e=0x7,
    OC2_JNT_THUMB_L1_e=0x8,
    OC2_JNT_ARM_R1_e=0x9,
    OC2_JNT_ARM_R2_e=0xA,
    OC2_JNT_ARM_R3_e=0xB,
    OC2_JNT_HAND_R1_e=0xC,
    OC2_JNT_FINGER_R1_e=0xD,
    OC2_JNT_THUMB_R1_e=0xE,
    OC2_JNT_NECK1_e=0xF,
    OC2_JNT_NECK2_e=0x10,
    OC2_JNT_HEAD_e=0x11,
    OC2_JNT_HEAR1_e=0x12,
    OC2_JNT_JAW1_e=0x13,
    OC2_JNT_WAIST_1_e=0x14,
    OC2_JNT_BAG1_e=0x15,
    OC2_JNT_BODY1_e=0x16,
    OC2_JNT_LEG_L1_e=0x17,
    OC2_JNT_LEG_L2_e=0x18,
    OC2_JNT_LEG_L3_e=0x19,
    OC2_JNT_LEG_R1_e=0x1A,
    OC2_JNT_LEG_R2_e=0x1B,
    OC2_JNT_LEG_R3_e=0x1C,
};

enum OC2_WEPON_JNT {
    OC2_WEPON_JNT_WEPON2_MODEL_e=0x0,
};

#endif /* !RES_E_OC2_H */