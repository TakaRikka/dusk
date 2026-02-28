#ifndef RES_E_SH_H
#define RES_E_SH_H

enum dRes_INDEX_E_SH {
    /* BCK  */
    dRes_INDEX_E_SH_BCK_SH_APPEAR_e=0x4,
    dRes_INDEX_E_SH_BCK_SH_ATTACK01_A_e=0x5,
    dRes_INDEX_E_SH_BCK_SH_ATTACK01_B_e=0x6,
    dRes_INDEX_E_SH_BCK_SH_ATTACK01_C_e=0x7,
    dRes_INDEX_E_SH_BCK_SH_ATTACK02_e=0x8,
    dRes_INDEX_E_SH_BCK_SH_DAMAGE_e=0x9,
    dRes_INDEX_E_SH_BCK_SH_DASH_e=0xA,
    dRes_INDEX_E_SH_BCK_SH_DEAD_e=0xB,
    dRes_INDEX_E_SH_BCK_SH_DISAPPEAR_e=0xC,
    dRes_INDEX_E_SH_BCK_SH_STEP_e=0xD,
    dRes_INDEX_E_SH_BCK_SH_WAIT_e=0xE,
    dRes_INDEX_E_SH_BCK_SH_WAIT02_e=0xF,
    dRes_INDEX_E_SH_BCK_SH_WAIT03_e=0x10,
    dRes_INDEX_E_SH_BCK_SH_WAIT04_e=0x11,
    dRes_INDEX_E_SH_BCK_SH_WALK_e=0x12,
    /* BMDR */
    dRes_INDEX_E_SH_BMD_E_EYE_e=0x15,
    dRes_INDEX_E_SH_BMD_SH_e=0x16,
};

enum dRes_ID_E_SH {
    /* BCK  */
    dRes_ID_E_SH_BCK_SH_APPEAR_e=0x4,
    dRes_ID_E_SH_BCK_SH_ATTACK01_A_e=0x5,
    dRes_ID_E_SH_BCK_SH_ATTACK01_B_e=0x6,
    dRes_ID_E_SH_BCK_SH_ATTACK01_C_e=0x7,
    dRes_ID_E_SH_BCK_SH_ATTACK02_e=0x8,
    dRes_ID_E_SH_BCK_SH_DAMAGE_e=0x9,
    dRes_ID_E_SH_BCK_SH_DASH_e=0xA,
    dRes_ID_E_SH_BCK_SH_DEAD_e=0xB,
    dRes_ID_E_SH_BCK_SH_DISAPPEAR_e=0xC,
    dRes_ID_E_SH_BCK_SH_STEP_e=0xD,
    dRes_ID_E_SH_BCK_SH_WAIT_e=0xE,
    dRes_ID_E_SH_BCK_SH_WAIT02_e=0xF,
    dRes_ID_E_SH_BCK_SH_WAIT03_e=0x10,
    dRes_ID_E_SH_BCK_SH_WAIT04_e=0x11,
    dRes_ID_E_SH_BCK_SH_WALK_e=0x12,
    /* BMDR */
    dRes_ID_E_SH_BMD_E_EYE_e=0x15,
    dRes_ID_E_SH_BMD_SH_e=0x16,
};

enum E_EYE_JNT {
    E_EYE_JNT_ENEMY_EYE_e=0x0,
};

enum SH_JNT {
    SH_JNT_WORLD_ROOT_e=0x0,
    SH_JNT_BACKBONE1_e=0x1,
    SH_JNT_BACKBONE2_e=0x2,
    SH_JNT_ARML1_e=0x3,
    SH_JNT_ARML2_e=0x4,
    SH_JNT_ARML3_e=0x5,
    SH_JNT_HANDL_e=0x6,
    SH_JNT_ARMR1_e=0x7,
    SH_JNT_ARMR2_e=0x8,
    SH_JNT_ARMR3_e=0x9,
    SH_JNT_HANDR_e=0xA,
    SH_JNT_NECK_e=0xB,
    SH_JNT_BONE_e=0xC,
    SH_JNT_HEAD_e=0xD,
    SH_JNT_CHIN_e=0xE,
    SH_JNT_WAIST_e=0xF,
    SH_JNT_LEGL1_e=0x10,
    SH_JNT_LEGL2_e=0x11,
    SH_JNT_LEGL3_e=0x12,
    SH_JNT_FOOTL_e=0x13,
    SH_JNT_LEGR1_e=0x14,
    SH_JNT_LEGR2_e=0x15,
    SH_JNT_LEGR3_e=0x16,
    SH_JNT_FOOTR_e=0x17,
    SH_JNT_TAIL1_e=0x18,
    SH_JNT_TAIL2_e=0x19,
};

#endif /* !RES_E_SH_H */