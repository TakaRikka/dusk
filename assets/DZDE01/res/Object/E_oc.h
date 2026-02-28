#ifndef RES_E_OC_H
#define RES_E_OC_H

enum dRes_INDEX_E_OC {
    /* BMDR */
    dRes_INDEX_E_OC_BMD_OC_e=0x3,
    dRes_INDEX_E_OC_BMD_OC_WEPON_e=0x4,
};

enum dRes_ID_E_OC {
    /* BMDR */
    dRes_ID_E_OC_BMD_OC_e=0x3,
    dRes_ID_E_OC_BMD_OC_WEPON_e=0x4,
};

enum OC_JNT {
    OC_JNT_WORLD_ROOT_e=0x0,
    OC_JNT_BACK_BONE1_e=0x1,
    OC_JNT_BACK_BONE2_e=0x2,
    OC_JNT_ARM_L1_e=0x3,
    OC_JNT_ARM_L2_e=0x4,
    OC_JNT_ARM_L3_e=0x5,
    OC_JNT_HAND_L1_e=0x6,
    OC_JNT_FINGER_L1_e=0x7,
    OC_JNT_THUMB_L1_e=0x8,
    OC_JNT_ARM_R1_e=0x9,
    OC_JNT_ARM_R2_e=0xA,
    OC_JNT_ARM_R3_e=0xB,
    OC_JNT_HAND_R1_e=0xC,
    OC_JNT_FINGER_R1_e=0xD,
    OC_JNT_THUMB_R1_e=0xE,
    OC_JNT_NECK1_e=0xF,
    OC_JNT_NECK2_e=0x10,
    OC_JNT_HEAD_e=0x11,
    OC_JNT_HEAR1_e=0x12,
    OC_JNT_JAW1_e=0x13,
    OC_JNT_WAIST_1_e=0x14,
    OC_JNT_BAG1_e=0x15,
    OC_JNT_BODY1_e=0x16,
    OC_JNT_LEG_L1_e=0x17,
    OC_JNT_LEG_L2_e=0x18,
    OC_JNT_LEG_L3_e=0x19,
    OC_JNT_LEG_R1_e=0x1A,
    OC_JNT_LEG_R2_e=0x1B,
    OC_JNT_LEG_R3_e=0x1C,
};

enum OC_WEPON_JNT {
    OC_WEPON_JNT_WEPON_MODEL_e=0x0,
};

#endif /* !RES_E_OC_H */