#ifndef RES_E_GI_H
#define RES_E_GI_H

enum dRes_INDEX_E_GI {
    /* BCK  */
    dRes_INDEX_E_GI_BCK_GI_APPEARANCE_e=0x4,
    dRes_INDEX_E_GI_BCK_GI_ATACK_e=0x5,
    dRes_INDEX_E_GI_BCK_GI_DAMAGE_L_e=0x6,
    dRes_INDEX_E_GI_BCK_GI_DAMAGE_R_e=0x7,
    dRes_INDEX_E_GI_BCK_GI_DIE_e=0x8,
    dRes_INDEX_E_GI_BCK_GI_GET_UP_e=0x9,
    dRes_INDEX_E_GI_BCK_GI_SHOUT_e=0xA,
    dRes_INDEX_E_GI_BCK_GI_WAIT_e=0xB,
    dRes_INDEX_E_GI_BCK_GI_WALK_e=0xC,
    dRes_INDEX_E_GI_BCK_GI_WOLF_DAMAGE_e=0xD,
    dRes_INDEX_E_GI_BCK_GI_WOLF_IMPACT_e=0xE,
    dRes_INDEX_E_GI_BCK_GI_WOLF_RETURN_e=0xF,
    dRes_INDEX_E_GI_BCK_GI_WOLF_WAIT_e=0x10,
    /* BMDR */
    dRes_INDEX_E_GI_BMD_GI_e=0x13,
    dRes_INDEX_E_GI_BMD_GI_SWORD_e=0x14,
};

enum dRes_ID_E_GI {
    /* BCK  */
    dRes_ID_E_GI_BCK_GI_APPEARANCE_e=0x4,
    dRes_ID_E_GI_BCK_GI_ATACK_e=0x5,
    dRes_ID_E_GI_BCK_GI_DAMAGE_L_e=0x6,
    dRes_ID_E_GI_BCK_GI_DAMAGE_R_e=0x7,
    dRes_ID_E_GI_BCK_GI_DIE_e=0x8,
    dRes_ID_E_GI_BCK_GI_GET_UP_e=0x9,
    dRes_ID_E_GI_BCK_GI_SHOUT_e=0xA,
    dRes_ID_E_GI_BCK_GI_WAIT_e=0xB,
    dRes_ID_E_GI_BCK_GI_WALK_e=0xC,
    dRes_ID_E_GI_BCK_GI_WOLF_DAMAGE_e=0xD,
    dRes_ID_E_GI_BCK_GI_WOLF_IMPACT_e=0xE,
    dRes_ID_E_GI_BCK_GI_WOLF_RETURN_e=0xF,
    dRes_ID_E_GI_BCK_GI_WOLF_WAIT_e=0x10,
    /* BMDR */
    dRes_ID_E_GI_BMD_GI_e=0x13,
    dRes_ID_E_GI_BMD_GI_SWORD_e=0x14,
};

enum GI_JNT {
    GI_JNT_WORLD_ROOT_e=0x0,
    GI_JNT_BACKBONE_1_e=0x1,
    GI_JNT_BACKBONE_2_e=0x2,
    GI_JNT_NECK_e=0x3,
    GI_JNT_HEAD_e=0x4,
    GI_JNT_CHIN_e=0x5,
    GI_JNT_SHOLDER_L_e=0x6,
    GI_JNT_ARM_L_1_e=0x7,
    GI_JNT_ARM_L_2_e=0x8,
    GI_JNT_ARM_L_3_e=0x9,
    GI_JNT_CLOTH_L_1_e=0xA,
    GI_JNT_CLOTH_L_2_e=0xB,
    GI_JNT_HAND_L_e=0xC,
    GI_JNT_SHOLDER_R_e=0xD,
    GI_JNT_ARM_R_1_e=0xE,
    GI_JNT_ARM_R_2_e=0xF,
    GI_JNT_ARM_R_3_e=0x10,
    GI_JNT_CLOTH_R_1_e=0x11,
    GI_JNT_CLOTH_R_2_e=0x12,
    GI_JNT_HAND_R_e=0x13,
    GI_JNT_SILK_e=0x14,
    GI_JNT_HIP_e=0x15,
    GI_JNT_LEG_L_1_e=0x16,
    GI_JNT_LEG_L_2_e=0x17,
    GI_JNT_FOOT_L_e=0x18,
    GI_JNT_LEG_R_1_e=0x19,
    GI_JNT_LEG_R_2_e=0x1A,
    GI_JNT_FOOT_R_e=0x1B,
};

enum GI_SWORD_JNT {
    GI_SWORD_JNT_SWORD_e=0x0,
};

#endif /* !RES_E_GI_H */