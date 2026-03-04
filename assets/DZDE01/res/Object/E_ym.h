#ifndef RES_E_YM_H
#define RES_E_YM_H

enum dRes_INDEX_E_YM {
    /* BCK  */
    dRes_INDEX_E_YM_BCK_YM_ATTACK_e=0x5,
    dRes_INDEX_E_YM_BCK_YM_DEATH_e=0x6,
    dRes_INDEX_E_YM_BCK_YM_DEFENSE_e=0x7,
    dRes_INDEX_E_YM_BCK_YM_DIG_e=0x8,
    dRes_INDEX_E_YM_BCK_YM_ELECTRIC_e=0x9,
    dRes_INDEX_E_YM_BCK_YM_JUMP_A_e=0xA,
    dRes_INDEX_E_YM_BCK_YM_JUMP_B_e=0xB,
    dRes_INDEX_E_YM_BCK_YM_JUMP_C_e=0xC,
    dRes_INDEX_E_YM_BCK_YM_JUST_e=0xD,
    dRes_INDEX_E_YM_BCK_YM_STEP_e=0xE,
    dRes_INDEX_E_YM_BCK_YM_WAIT_e=0xF,
    dRes_INDEX_E_YM_BCK_YM_WALK_e=0x10,
    /* BMDR */
    dRes_INDEX_E_YM_BMD_YM_TW_e=0x13,
    /* BRK  */
    dRes_INDEX_E_YM_BRK_YM_MARK_WAIT_e=0x16,
};

enum dRes_ID_E_YM {
    /* BCK  */
    dRes_ID_E_YM_BCK_YM_ATTACK_e=0x5,
    dRes_ID_E_YM_BCK_YM_DEATH_e=0x6,
    dRes_ID_E_YM_BCK_YM_DEFENSE_e=0x7,
    dRes_ID_E_YM_BCK_YM_DIG_e=0x8,
    dRes_ID_E_YM_BCK_YM_ELECTRIC_e=0x9,
    dRes_ID_E_YM_BCK_YM_JUMP_A_e=0xA,
    dRes_ID_E_YM_BCK_YM_JUMP_B_e=0xB,
    dRes_ID_E_YM_BCK_YM_JUMP_C_e=0xC,
    dRes_ID_E_YM_BCK_YM_JUST_e=0xD,
    dRes_ID_E_YM_BCK_YM_STEP_e=0xE,
    dRes_ID_E_YM_BCK_YM_WAIT_e=0xF,
    dRes_ID_E_YM_BCK_YM_WALK_e=0x10,
    /* BMDR */
    dRes_ID_E_YM_BMD_YM_TW_e=0x13,
    /* BRK  */
    dRes_ID_E_YM_BRK_YM_MARK_WAIT_e=0x16,
};

enum YM_TW_JNT {
    YM_TW_JNT_BODY_e=0x0,
    YM_TW_JNT_HEAD_e=0x1,
    YM_TW_JNT_LEG_L1_A_e=0x2,
    YM_TW_JNT_LEG_L1_B_e=0x3,
    YM_TW_JNT_LEG_L1_C_e=0x4,
    YM_TW_JNT_LEG_L2_A_e=0x5,
    YM_TW_JNT_LEG_L2_B_e=0x6,
    YM_TW_JNT_LEG_L2_C_e=0x7,
    YM_TW_JNT_LEG_L3_A_e=0x8,
    YM_TW_JNT_LEG_L3_B_e=0x9,
    YM_TW_JNT_LEG_L3_C_e=0xA,
    YM_TW_JNT_LEG_R1_A_e=0xB,
    YM_TW_JNT_LEG_R1_B_e=0xC,
    YM_TW_JNT_LEG_R1_C_e=0xD,
    YM_TW_JNT_LEG_R2_A_e=0xE,
    YM_TW_JNT_LEG_R2_B_e=0xF,
    YM_TW_JNT_LEG_R2_C_e=0x10,
    YM_TW_JNT_LEG_R3_A_e=0x11,
    YM_TW_JNT_LEG_R3_B_e=0x12,
    YM_TW_JNT_LEG_R3_C_e=0x13,
    YM_TW_JNT_TAIL_e=0x14,
};

#endif /* !RES_E_YM_H */