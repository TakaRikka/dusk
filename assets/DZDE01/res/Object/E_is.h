#ifndef RES_E_IS_H
#define RES_E_IS_H

enum dRes_INDEX_E_IS {
    /* BCK  */
    dRes_INDEX_E_IS_BCK_IS_ATTACK_e=0x4,
    dRes_INDEX_E_IS_BCK_IS_DAMAGE01_e=0x5,
    dRes_INDEX_E_IS_BCK_IS_DAMAGE02_e=0x6,
    dRes_INDEX_E_IS_BCK_IS_DEAD01_e=0x7,
    dRes_INDEX_E_IS_BCK_IS_DEAD02_e=0x8,
    dRes_INDEX_E_IS_BCK_IS_FALL_e=0x9,
    dRes_INDEX_E_IS_BCK_IS_STARTL_e=0xA,
    dRes_INDEX_E_IS_BCK_IS_STARTR_e=0xB,
    dRes_INDEX_E_IS_BCK_IS_STEP_e=0xC,
    dRes_INDEX_E_IS_BCK_IS_TRAP_e=0xD,
    dRes_INDEX_E_IS_BCK_IS_WAIT02_e=0xE,
    dRes_INDEX_E_IS_BCK_IS_WAIT03_e=0xF,
    dRes_INDEX_E_IS_BCK_IS_WALK_e=0x10,
    /* BMDE */
    dRes_INDEX_E_IS_BMD_IS_e=0x13,
};

enum dRes_ID_E_IS {
    /* BCK  */
    dRes_ID_E_IS_BCK_IS_ATTACK_e=0x4,
    dRes_ID_E_IS_BCK_IS_DAMAGE01_e=0x5,
    dRes_ID_E_IS_BCK_IS_DAMAGE02_e=0x6,
    dRes_ID_E_IS_BCK_IS_DEAD01_e=0x7,
    dRes_ID_E_IS_BCK_IS_DEAD02_e=0x8,
    dRes_ID_E_IS_BCK_IS_FALL_e=0x9,
    dRes_ID_E_IS_BCK_IS_STARTL_e=0xA,
    dRes_ID_E_IS_BCK_IS_STARTR_e=0xB,
    dRes_ID_E_IS_BCK_IS_STEP_e=0xC,
    dRes_ID_E_IS_BCK_IS_TRAP_e=0xD,
    dRes_ID_E_IS_BCK_IS_WAIT02_e=0xE,
    dRes_ID_E_IS_BCK_IS_WAIT03_e=0xF,
    dRes_ID_E_IS_BCK_IS_WALK_e=0x10,
    /* BMDE */
    dRes_ID_E_IS_BMD_IS_e=0x13,
};

enum IS_JNT {
    IS_JNT_WORLD_ROOT_e=0x0,
    IS_JNT_BACKBONE1_e=0x1,
    IS_JNT_BACKBONE2_e=0x2,
    IS_JNT_ARML1_e=0x3,
    IS_JNT_ARML2_e=0x4,
    IS_JNT_HANDL_e=0x5,
    IS_JNT_FINGERL1_e=0x6,
    IS_JNT_FINGERL2_e=0x7,
    IS_JNT_ARMR1_e=0x8,
    IS_JNT_ARMR2_e=0x9,
    IS_JNT_HANDR_e=0xA,
    IS_JNT_FINGERR1_e=0xB,
    IS_JNT_FINGERR2_e=0xC,
    IS_JNT_NECK_e=0xD,
    IS_JNT_HEAD_e=0xE,
    IS_JNT_WAIST_e=0xF,
    IS_JNT_LEGL1_e=0x10,
    IS_JNT_LEGL2_e=0x11,
    IS_JNT_FOOTL_e=0x12,
    IS_JNT_LEGR1_e=0x13,
    IS_JNT_LEGR2_e=0x14,
    IS_JNT_FOOTR_e=0x15,
};

#endif /* !RES_E_IS_H */