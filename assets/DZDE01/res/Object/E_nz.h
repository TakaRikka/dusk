#ifndef RES_E_NZ_H
#define RES_E_NZ_H

enum dRes_INDEX_E_NZ {
    /* BCK  */
    dRes_INDEX_E_NZ_BCK_NZ_DIE_e=0x4,
    dRes_INDEX_E_NZ_BCK_NZ_HOLD_WAIT_e=0x5,
    dRes_INDEX_E_NZ_BCK_NZ_JUMP_MIDDLE_e=0x6,
    dRes_INDEX_E_NZ_BCK_NZ_JUMP_START_e=0x7,
    dRes_INDEX_E_NZ_BCK_NZ_KYOROKYORO_e=0x8,
    dRes_INDEX_E_NZ_BCK_NZ_RUN_e=0x9,
    dRes_INDEX_E_NZ_BCK_NZ_WAIT_e=0xA,
    /* BMDR */
    dRes_INDEX_E_NZ_BMD_NZ_e=0xD,
};

enum dRes_ID_E_NZ {
    /* BCK  */
    dRes_ID_E_NZ_BCK_NZ_DIE_e=0x4,
    dRes_ID_E_NZ_BCK_NZ_HOLD_WAIT_e=0x5,
    dRes_ID_E_NZ_BCK_NZ_JUMP_MIDDLE_e=0x6,
    dRes_ID_E_NZ_BCK_NZ_JUMP_START_e=0x7,
    dRes_ID_E_NZ_BCK_NZ_KYOROKYORO_e=0x8,
    dRes_ID_E_NZ_BCK_NZ_RUN_e=0x9,
    dRes_ID_E_NZ_BCK_NZ_WAIT_e=0xA,
    /* BMDR */
    dRes_ID_E_NZ_BMD_NZ_e=0xD,
};

enum NZ_JNT {
    NZ_JNT_WORLD_ROOT_e=0x0,
    NZ_JNT_BACKBONE_1_e=0x1,
    NZ_JNT_F_L_LEG_1_e=0x2,
    NZ_JNT_F_L_LEG_2_e=0x3,
    NZ_JNT_F_R_LEG_1_e=0x4,
    NZ_JNT_F_R_LEG_2_e=0x5,
    NZ_JNT_NECK_e=0x6,
    NZ_JNT_WAIST_e=0x7,
    NZ_JNT_B_L_LEG_1_e=0x8,
    NZ_JNT_B_L_LEG_2_e=0x9,
    NZ_JNT_B_R_LEG_1_e=0xA,
    NZ_JNT_B_R_LEG_2_e=0xB,
    NZ_JNT_TAIL_1_e=0xC,
    NZ_JNT_TAIL_2_e=0xD,
    NZ_JNT_TAIL_3_e=0xE,
    NZ_JNT_TAIL_4_e=0xF,
};

#endif /* !RES_E_NZ_H */