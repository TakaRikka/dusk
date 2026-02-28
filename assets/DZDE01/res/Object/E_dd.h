#ifndef RES_E_DD_H
#define RES_E_DD_H

enum dRes_INDEX_E_DD {
    /* BCK  */
    dRes_INDEX_E_DD_BCK_DD_DAMAGE_L_e=0x5,
    dRes_INDEX_E_DD_BCK_DD_DAMAGE_R_e=0x6,
    dRes_INDEX_E_DD_BCK_DD_DIE_e=0x7,
    dRes_INDEX_E_DD_BCK_DD_DIE_ARROW_e=0x8,
    dRes_INDEX_E_DD_BCK_DD_DIE_BOMB_e=0x9,
    dRes_INDEX_E_DD_BCK_DD_DIE_FALL_A_1_e=0xA,
    dRes_INDEX_E_DD_BCK_DD_DIE_FALL_A_2_e=0xB,
    dRes_INDEX_E_DD_BCK_DD_DIE_FALL_B_e=0xC,
    dRes_INDEX_E_DD_BCK_DD_DIE_FALL_C_e=0xD,
    dRes_INDEX_E_DD_BCK_DD_FIRE_A_e=0xE,
    dRes_INDEX_E_DD_BCK_DD_FIRE_B_e=0xF,
    dRes_INDEX_E_DD_BCK_DD_FIRE_C_e=0x10,
    dRes_INDEX_E_DD_BCK_DD_GUARD_e=0x11,
    dRes_INDEX_E_DD_BCK_DD_RUN_e=0x12,
    dRes_INDEX_E_DD_BCK_DD_TURN_L_e=0x13,
    dRes_INDEX_E_DD_BCK_DD_TURN_R_e=0x14,
    dRes_INDEX_E_DD_BCK_DD_WAIT_e=0x15,
    dRes_INDEX_E_DD_BCK_DD_WALK_e=0x16,
    /* BMDR */
    dRes_INDEX_E_DD_BMD_DD_e=0x19,
    /* BRK  */
    dRes_INDEX_E_DD_BRK_DD_e=0x1C,
    dRes_INDEX_E_DD_BRK_DD_DEAD_e=0x1D,
    dRes_INDEX_E_DD_BRK_DD_FIRE_A_e=0x1E,
    dRes_INDEX_E_DD_BRK_DD_FIRE_B_e=0x1F,
    dRes_INDEX_E_DD_BRK_DD_FIRE_C_e=0x20,
};

enum dRes_ID_E_DD {
    /* BCK  */
    dRes_ID_E_DD_BCK_DD_DAMAGE_L_e=0x5,
    dRes_ID_E_DD_BCK_DD_DAMAGE_R_e=0x6,
    dRes_ID_E_DD_BCK_DD_DIE_e=0x7,
    dRes_ID_E_DD_BCK_DD_DIE_ARROW_e=0x8,
    dRes_ID_E_DD_BCK_DD_DIE_BOMB_e=0x9,
    dRes_ID_E_DD_BCK_DD_DIE_FALL_A_1_e=0xA,
    dRes_ID_E_DD_BCK_DD_DIE_FALL_A_2_e=0xB,
    dRes_ID_E_DD_BCK_DD_DIE_FALL_B_e=0xC,
    dRes_ID_E_DD_BCK_DD_DIE_FALL_C_e=0xD,
    dRes_ID_E_DD_BCK_DD_FIRE_A_e=0xE,
    dRes_ID_E_DD_BCK_DD_FIRE_B_e=0xF,
    dRes_ID_E_DD_BCK_DD_FIRE_C_e=0x10,
    dRes_ID_E_DD_BCK_DD_GUARD_e=0x11,
    dRes_ID_E_DD_BCK_DD_RUN_e=0x12,
    dRes_ID_E_DD_BCK_DD_TURN_L_e=0x13,
    dRes_ID_E_DD_BCK_DD_TURN_R_e=0x14,
    dRes_ID_E_DD_BCK_DD_WAIT_e=0x15,
    dRes_ID_E_DD_BCK_DD_WALK_e=0x16,
    /* BMDR */
    dRes_ID_E_DD_BMD_DD_e=0x19,
    /* BRK  */
    dRes_ID_E_DD_BRK_DD_e=0x1C,
    dRes_ID_E_DD_BRK_DD_DEAD_e=0x1D,
    dRes_ID_E_DD_BRK_DD_FIRE_A_e=0x1E,
    dRes_ID_E_DD_BRK_DD_FIRE_B_e=0x1F,
    dRes_ID_E_DD_BRK_DD_FIRE_C_e=0x20,
};

enum DD_JNT {
    DD_JNT_WORLD_ROOT_e=0x0,
    DD_JNT_BACKBONE_1_e=0x1,
    DD_JNT_BACKBONE_2_e=0x2,
    DD_JNT_BACKBONE_3_e=0x3,
    DD_JNT_ARM_L_1_e=0x4,
    DD_JNT_ARM_L_2_e=0x5,
    DD_JNT_HAND_L_e=0x6,
    DD_JNT_ARM_R_1_e=0x7,
    DD_JNT_ARM_R_2_e=0x8,
    DD_JNT_HAND_R_e=0x9,
    DD_JNT_NECK_e=0xA,
    DD_JNT_CHIN_e=0xB,
    DD_JNT_WAIST_e=0xC,
    DD_JNT_LEG_L_1_e=0xD,
    DD_JNT_LEG_L_2_e=0xE,
    DD_JNT_FOOT_L_e=0xF,
    DD_JNT_LEG_R_1_e=0x10,
    DD_JNT_LEG_R_2_e=0x11,
    DD_JNT_FOOT_R_e=0x12,
    DD_JNT_TAIL_1_e=0x13,
    DD_JNT_TAIL_2_e=0x14,
    DD_JNT_TAIL3_e=0x15,
    DD_JNT_TAIL_4_e=0x16,
};

#endif /* !RES_E_DD_H */