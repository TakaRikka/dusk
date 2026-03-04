#ifndef RES_E_MS_H
#define RES_E_MS_H

enum dRes_INDEX_E_MS {
    /* BCK  */
    dRes_INDEX_E_MS_BCK_MS_BITE_DIE_e=0x4,
    dRes_INDEX_E_MS_BCK_MS_DAMAGE_e=0x5,
    dRes_INDEX_E_MS_BCK_MS_DIE_e=0x6,
    dRes_INDEX_E_MS_BCK_MS_DIE_FLOAT_e=0x7,
    dRes_INDEX_E_MS_BCK_MS_FIND_e=0x8,
    dRes_INDEX_E_MS_BCK_MS_GNAW_e=0x9,
    dRes_INDEX_E_MS_BCK_MS_JUMP_END_e=0xA,
    dRes_INDEX_E_MS_BCK_MS_JUMP_MIDDLE_e=0xB,
    dRes_INDEX_E_MS_BCK_MS_JUMP_START_e=0xC,
    dRes_INDEX_E_MS_BCK_MS_KYOROKYORO_e=0xD,
    dRes_INDEX_E_MS_BCK_MS_RUN_e=0xE,
    dRes_INDEX_E_MS_BCK_MS_SWIM_e=0xF,
    dRes_INDEX_E_MS_BCK_MS_WAIT_e=0x10,
    dRes_INDEX_E_MS_BCK_MS_WALK_e=0x11,
    /* BMDR */
    dRes_INDEX_E_MS_BMD_MS_e=0x14,
};

enum dRes_ID_E_MS {
    /* BCK  */
    dRes_ID_E_MS_BCK_MS_BITE_DIE_e=0x4,
    dRes_ID_E_MS_BCK_MS_DAMAGE_e=0x5,
    dRes_ID_E_MS_BCK_MS_DIE_e=0x6,
    dRes_ID_E_MS_BCK_MS_DIE_FLOAT_e=0x7,
    dRes_ID_E_MS_BCK_MS_FIND_e=0x8,
    dRes_ID_E_MS_BCK_MS_GNAW_e=0x9,
    dRes_ID_E_MS_BCK_MS_JUMP_END_e=0xA,
    dRes_ID_E_MS_BCK_MS_JUMP_MIDDLE_e=0xB,
    dRes_ID_E_MS_BCK_MS_JUMP_START_e=0xC,
    dRes_ID_E_MS_BCK_MS_KYOROKYORO_e=0xD,
    dRes_ID_E_MS_BCK_MS_RUN_e=0xE,
    dRes_ID_E_MS_BCK_MS_SWIM_e=0xF,
    dRes_ID_E_MS_BCK_MS_WAIT_e=0x10,
    dRes_ID_E_MS_BCK_MS_WALK_e=0x11,
    /* BMDR */
    dRes_ID_E_MS_BMD_MS_e=0x14,
};

enum MS_JNT {
    MS_JNT_WORLD_ROOT_e=0x0,
    MS_JNT_BACKBONE_1_e=0x1,
    MS_JNT_BACKBONE_2_e=0x2,
    MS_JNT_F_L_LEG_1_e=0x3,
    MS_JNT_F_L_LEG_2_e=0x4,
    MS_JNT_F_L_LEG_3_e=0x5,
    MS_JNT_F_R_LEG_1_e=0x6,
    MS_JNT_F_R_LEG_2_e=0x7,
    MS_JNT_F_R_LEG_3_e=0x8,
    MS_JNT_HEAD_e=0x9,
    MS_JNT_MOUTH_e=0xA,
    MS_JNT_WAIST_e=0xB,
    MS_JNT_B_L_LEG_2_e=0xC,
    MS_JNT_B_L_LEG_3_e=0xD,
    MS_JNT_B_L_LEG_4_e=0xE,
    MS_JNT_B_R_LEG_2_e=0xF,
    MS_JNT_B_R_LEG_3_e=0x10,
    MS_JNT_B_R_LEG_4_e=0x11,
    MS_JNT_TAIL_1_e=0x12,
    MS_JNT_TAIL_2_e=0x13,
    MS_JNT_TAIL_3_e=0x14,
    MS_JNT_TAIL_4_e=0x15,
};

#endif /* !RES_E_MS_H */