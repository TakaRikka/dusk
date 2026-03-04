#ifndef RES_E_TM_H
#define RES_E_TM_H

enum dRes_INDEX_E_TM {
    /* BCK  */
    dRes_INDEX_E_TM_BCK_TM_ATTACK_A_e=0x5,
    dRes_INDEX_E_TM_BCK_TM_ATTACK_B_e=0x6,
    dRes_INDEX_E_TM_BCK_TM_ATTACK_C_e=0x7,
    dRes_INDEX_E_TM_BCK_TM_DEATH_e=0x8,
    dRes_INDEX_E_TM_BCK_TM_G_WAIT_e=0x9,
    dRes_INDEX_E_TM_BCK_TM_G_WALK_e=0xA,
    dRes_INDEX_E_TM_BCK_TM_K_FLY_e=0xB,
    dRes_INDEX_E_TM_BCK_TM_TAKE_FLY_e=0xC,
    dRes_INDEX_E_TM_BCK_TM_TAKE_OFF_e=0xD,
    dRes_INDEX_E_TM_BCK_TM_WAIT_e=0xE,
    /* BMDR */
    dRes_INDEX_E_TM_BMD_TM_TW_e=0x11,
    /* BRK  */
    dRes_INDEX_E_TM_BRK_TM_MARK_e=0x14,
};

enum dRes_ID_E_TM {
    /* BCK  */
    dRes_ID_E_TM_BCK_TM_ATTACK_A_e=0x5,
    dRes_ID_E_TM_BCK_TM_ATTACK_B_e=0x6,
    dRes_ID_E_TM_BCK_TM_ATTACK_C_e=0x7,
    dRes_ID_E_TM_BCK_TM_DEATH_e=0x8,
    dRes_ID_E_TM_BCK_TM_G_WAIT_e=0x9,
    dRes_ID_E_TM_BCK_TM_G_WALK_e=0xA,
    dRes_ID_E_TM_BCK_TM_K_FLY_e=0xB,
    dRes_ID_E_TM_BCK_TM_TAKE_FLY_e=0xC,
    dRes_ID_E_TM_BCK_TM_TAKE_OFF_e=0xD,
    dRes_ID_E_TM_BCK_TM_WAIT_e=0xE,
    /* BMDR */
    dRes_ID_E_TM_BMD_TM_TW_e=0x11,
    /* BRK  */
    dRes_ID_E_TM_BRK_TM_MARK_e=0x14,
};

enum TM_TW_JNT {
    TM_TW_JNT_BODY_e=0x0,
    TM_TW_JNT_HEAD_e=0x1,
    TM_TW_JNT_LEG_L1A_e=0x2,
    TM_TW_JNT_LEG_L1B_e=0x3,
    TM_TW_JNT_LEG_L1C_e=0x4,
    TM_TW_JNT_LEG_R1A_e=0x5,
    TM_TW_JNT_LEG_R1B_e=0x6,
    TM_TW_JNT_LEG_R1C_e=0x7,
    TM_TW_JNT_TAIL_e=0x8,
    TM_TW_JNT_INER_LA_IN_e=0x9,
    TM_TW_JNT_INER_LB_IN_e=0xA,
    TM_TW_JNT_INER_RA_IN_e=0xB,
    TM_TW_JNT_INER_RB_IN_e=0xC,
    TM_TW_JNT_LEG_L2A_e=0xD,
    TM_TW_JNT_LEG_L2B_e=0xE,
    TM_TW_JNT_LEG_L2C_e=0xF,
    TM_TW_JNT_LEG_L3A_e=0x10,
    TM_TW_JNT_LEG_L3B_e=0x11,
    TM_TW_JNT_LEG_L3C_e=0x12,
    TM_TW_JNT_LEG_R2A_e=0x13,
    TM_TW_JNT_LEG_R2B_e=0x14,
    TM_TW_JNT_LEG_R2C_e=0x15,
    TM_TW_JNT_LEG_R3A_e=0x16,
    TM_TW_JNT_LEG_R3B_e=0x17,
    TM_TW_JNT_LEG_R3C_e=0x18,
    TM_TW_JNT_WING_L_e=0x19,
    TM_TW_JNT_WING_R_e=0x1A,
};

#endif /* !RES_E_TM_H */