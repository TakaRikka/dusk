#ifndef RES_E_GB_H
#define RES_E_GB_H

enum dRes_INDEX_E_GB {
    /* BCK  */
    dRes_INDEX_E_GB_BCK_GB_ATTACK_e=0x5,
    dRes_INDEX_E_GB_BCK_GB_ATTACK_BEAT_e=0x6,
    dRes_INDEX_E_GB_BCK_GB_ATTACK_BEAT_SIGN_e=0x7,
    dRes_INDEX_E_GB_BCK_GB_ATTACK_DASH_e=0x8,
    dRes_INDEX_E_GB_BCK_GB_BOUND_e=0x9,
    dRes_INDEX_E_GB_BCK_GB_BREATH_e=0xA,
    dRes_INDEX_E_GB_BCK_GB_DAMAGE_A_e=0xB,
    dRes_INDEX_E_GB_BCK_GB_DAMAGE_B_e=0xC,
    dRes_INDEX_E_GB_BCK_GB_DAMAGE_WAIT_e=0xD,
    dRes_INDEX_E_GB_BCK_GB_DEMO_DROPKEY_e=0xE,
    dRes_INDEX_E_GB_BCK_GB_DIE_e=0xF,
    dRes_INDEX_E_GB_BCK_GB_REVIVE_e=0x10,
    dRes_INDEX_E_GB_BCK_GB_WAIT_e=0x11,
    dRes_INDEX_E_GB_BCK_GF_BOUND_e=0x12,
    dRes_INDEX_E_GB_BCK_GF_BOUND_CLOSE_e=0x13,
    dRes_INDEX_E_GB_BCK_GF_BURST_e=0x14,
    dRes_INDEX_E_GB_BCK_GF_CLOSE_e=0x15,
    dRes_INDEX_E_GB_BCK_GF_DIE_e=0x16,
    dRes_INDEX_E_GB_BCK_GF_EAT_e=0x17,
    dRes_INDEX_E_GB_BCK_GF_OPEN_e=0x18,
    dRes_INDEX_E_GB_BCK_GF_WAIT_e=0x19,
    dRes_INDEX_E_GB_BCK_GF_WAIT_CLOSE_e=0x1A,
    /* BMDR */
    dRes_INDEX_E_GB_BMD_GB_e=0x1D,
    dRes_INDEX_E_GB_BMD_GB_KEY_e=0x1E,
    dRes_INDEX_E_GB_BMD_GF_e=0x1F,
    dRes_INDEX_E_GB_BMD_GS_e=0x20,
    /* BRK  */
    dRes_INDEX_E_GB_BRK_GF_DIE_e=0x23,
};

enum dRes_ID_E_GB {
    /* BCK  */
    dRes_ID_E_GB_BCK_GB_ATTACK_e=0x5,
    dRes_ID_E_GB_BCK_GB_ATTACK_BEAT_e=0x6,
    dRes_ID_E_GB_BCK_GB_ATTACK_BEAT_SIGN_e=0x7,
    dRes_ID_E_GB_BCK_GB_ATTACK_DASH_e=0x8,
    dRes_ID_E_GB_BCK_GB_BOUND_e=0x9,
    dRes_ID_E_GB_BCK_GB_BREATH_e=0xA,
    dRes_ID_E_GB_BCK_GB_DAMAGE_A_e=0xB,
    dRes_ID_E_GB_BCK_GB_DAMAGE_B_e=0xC,
    dRes_ID_E_GB_BCK_GB_DAMAGE_WAIT_e=0xD,
    dRes_ID_E_GB_BCK_GB_DEMO_DROPKEY_e=0xE,
    dRes_ID_E_GB_BCK_GB_DIE_e=0xF,
    dRes_ID_E_GB_BCK_GB_REVIVE_e=0x10,
    dRes_ID_E_GB_BCK_GB_WAIT_e=0x11,
    dRes_ID_E_GB_BCK_GF_BOUND_e=0x12,
    dRes_ID_E_GB_BCK_GF_BOUND_CLOSE_e=0x13,
    dRes_ID_E_GB_BCK_GF_BURST_e=0x14,
    dRes_ID_E_GB_BCK_GF_CLOSE_e=0x15,
    dRes_ID_E_GB_BCK_GF_DIE_e=0x16,
    dRes_ID_E_GB_BCK_GF_EAT_e=0x17,
    dRes_ID_E_GB_BCK_GF_OPEN_e=0x18,
    dRes_ID_E_GB_BCK_GF_WAIT_e=0x19,
    dRes_ID_E_GB_BCK_GF_WAIT_CLOSE_e=0x1A,
    /* BMDR */
    dRes_ID_E_GB_BMD_GB_e=0x1D,
    dRes_ID_E_GB_BMD_GB_KEY_e=0x1E,
    dRes_ID_E_GB_BMD_GF_e=0x1F,
    dRes_ID_E_GB_BMD_GS_e=0x20,
    /* BRK  */
    dRes_ID_E_GB_BRK_GF_DIE_e=0x23,
};

enum GB_JNT {
    GB_JNT_CENTER_e=0x0,
    GB_JNT_NECK_e=0x1,
    GB_JNT_MOUTH_2_e=0x2,
    GB_JNT_MOUTH_1_e=0x3,
};

enum GB_KEY_JNT {
    GB_KEY_JNT_T_GD_KYE_e=0x0,
};

enum GF_JNT {
    GF_JNT_CENTER_e=0x0,
    GF_JNT_FLOWER_1_e=0x1,
    GF_JNT_FLOWER_2_e=0x2,
    GF_JNT_FLOWER_3_e=0x3,
    GF_JNT_FLOWER_4_e=0x4,
    GF_JNT_FLOWER_5_e=0x5,
    GF_JNT_PETAL_A_1_e=0x6,
    GF_JNT_PETAL_A_2_e=0x7,
    GF_JNT_PETAL_B_1_e=0x8,
    GF_JNT_PETAL_B_2_e=0x9,
    GF_JNT_PETAL_C_1_e=0xA,
    GF_JNT_PETAL_C_2_e=0xB,
    GF_JNT_PETAL_D_1_e=0xC,
    GF_JNT_PETAL_D_2_e=0xD,
    GF_JNT_PETAL_E_1_e=0xE,
    GF_JNT_PETAL_E_2_e=0xF,
    GF_JNT_PETAL_F_1_e=0x10,
    GF_JNT_PETAL_F_2_e=0x11,
};

enum GS_JNT {
    GS_JNT_GS_MODEL_e=0x0,
};

#endif /* !RES_E_GB_H */