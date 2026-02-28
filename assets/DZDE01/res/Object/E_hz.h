#ifndef RES_E_HZ_H
#define RES_E_HZ_H

enum dRes_INDEX_E_HZ {
    /* BCK  */
    dRes_INDEX_E_HZ_BCK_HZ_BACK_e=0x5,
    dRes_INDEX_E_HZ_BCK_HZ_DAMAGE_e=0x6,
    dRes_INDEX_E_HZ_BCK_HZ_DEATH_e=0x7,
    dRes_INDEX_E_HZ_BCK_HZ_DEATH_STAND_e=0x8,
    dRes_INDEX_E_HZ_BCK_HZ_DROWNED_A_e=0x9,
    dRes_INDEX_E_HZ_BCK_HZ_DROWNED_B_e=0xA,
    dRes_INDEX_E_HZ_BCK_HZ_FAINT_e=0xB,
    dRes_INDEX_E_HZ_BCK_HZ_GALE_e=0xC,
    dRes_INDEX_E_HZ_BCK_HZ_JUMP_e=0xD,
    dRes_INDEX_E_HZ_BCK_HZ_LANDING_e=0xE,
    dRes_INDEX_E_HZ_BCK_HZ_STAND_e=0xF,
    dRes_INDEX_E_HZ_BCK_HZ_WAIT_e=0x10,
    dRes_INDEX_E_HZ_BCK_HZ_WALK_e=0x11,
    dRes_INDEX_E_HZ_BCK_HZ_WALK_D_R_e=0x12,
    dRes_INDEX_E_HZ_BCK_HZ_WIND_e=0x13,
    /* BMDR */
    dRes_INDEX_E_HZ_BMD_HZ_e=0x16,
    dRes_INDEX_E_HZ_BMD_HZ_HOLE_e=0x17,
    /* DZB  */
    dRes_INDEX_E_HZ_DZB_K_HZP00_e=0x1A,
};

enum dRes_ID_E_HZ {
    /* BCK  */
    dRes_ID_E_HZ_BCK_HZ_BACK_e=0x5,
    dRes_ID_E_HZ_BCK_HZ_DAMAGE_e=0x6,
    dRes_ID_E_HZ_BCK_HZ_DEATH_e=0x7,
    dRes_ID_E_HZ_BCK_HZ_DEATH_STAND_e=0x8,
    dRes_ID_E_HZ_BCK_HZ_DROWNED_A_e=0x9,
    dRes_ID_E_HZ_BCK_HZ_DROWNED_B_e=0xA,
    dRes_ID_E_HZ_BCK_HZ_FAINT_e=0xB,
    dRes_ID_E_HZ_BCK_HZ_GALE_e=0xC,
    dRes_ID_E_HZ_BCK_HZ_JUMP_e=0xD,
    dRes_ID_E_HZ_BCK_HZ_LANDING_e=0xE,
    dRes_ID_E_HZ_BCK_HZ_STAND_e=0xF,
    dRes_ID_E_HZ_BCK_HZ_WAIT_e=0x10,
    dRes_ID_E_HZ_BCK_HZ_WALK_e=0x11,
    dRes_ID_E_HZ_BCK_HZ_WALK_D_R_e=0x12,
    dRes_ID_E_HZ_BCK_HZ_WIND_e=0x13,
    /* BMDR */
    dRes_ID_E_HZ_BMD_HZ_e=0x16,
    dRes_ID_E_HZ_BMD_HZ_HOLE_e=0x17,
    /* DZB  */
    dRes_ID_E_HZ_DZB_K_HZP00_e=0x1A,
};

enum HZ_JNT {
    HZ_JNT_WORLD_ROOT_e=0x0,
    HZ_JNT_BACK_BONE1_e=0x1,
    HZ_JNT_BACK_BONE2_e=0x2,
    HZ_JNT_BACK_BONE3_e=0x3,
    HZ_JNT_HEAD_e=0x4,
    HZ_JNT_JAW_L_e=0x5,
    HZ_JNT_JAW_R_e=0x6,
    HZ_JNT_PLATE_e=0x7,
    HZ_JNT_LEG_L1A_e=0x8,
    HZ_JNT_LEG_L1B_e=0x9,
    HZ_JNT_LEG_L2A_e=0xA,
    HZ_JNT_LEG_L2B_e=0xB,
    HZ_JNT_LEG_R1A_e=0xC,
    HZ_JNT_LEG_R1B_e=0xD,
    HZ_JNT_LEG_R2A_e=0xE,
    HZ_JNT_LEG_R2B_e=0xF,
    HZ_JNT_WAIST1_e=0x10,
    HZ_JNT_WAIST2_e=0x11,
    HZ_JNT_WAIST3_e=0x12,
    HZ_JNT_WAIST4_e=0x13,
    HZ_JNT_TAIL1_e=0x14,
    HZ_JNT_TAIL2_e=0x15,
    HZ_JNT_TAIL3_e=0x16,
};

enum HZ_HOLE_JNT {
    HZ_HOLE_JNT_HOLE_MODEL_e=0x0,
};

#endif /* !RES_E_HZ_H */