#ifndef RES_E_DK_H
#define RES_E_DK_H

enum dRes_INDEX_E_DK {
    /* BCK  */
    dRes_INDEX_E_DK_BCK_DK_ATTACK_e=0x6,
    dRes_INDEX_E_DK_BCK_DK_CHARGE_e=0x7,
    dRes_INDEX_E_DK_BCK_DK_C_ATTACK_e=0x8,
    dRes_INDEX_E_DK_BCK_DK_C_CHARGE_e=0x9,
    dRes_INDEX_E_DK_BCK_DK_C_DEAD_e=0xA,
    dRes_INDEX_E_DK_BCK_DK_C_GUARD_e=0xB,
    dRes_INDEX_E_DK_BCK_DK_C_WAIT_e=0xC,
    dRes_INDEX_E_DK_BCK_DK_GUARD_e=0xD,
    dRes_INDEX_E_DK_BCK_DK_SHELLDEAD_e=0xE,
    dRes_INDEX_E_DK_BCK_DK_WAIT_e=0xF,
    /* BMDR */
    dRes_INDEX_E_DK_BMD_DK_e=0x12,
    dRes_INDEX_E_DK_BMD_DK_CORE_e=0x13,
    /* BRK  */
    dRes_INDEX_E_DK_BRK_DK_ATTACK_e=0x16,
    dRes_INDEX_E_DK_BRK_DK_WAIT_e=0x17,
    /* BTK  */
    dRes_INDEX_E_DK_BTK_DK_ATTACK_e=0x1A,
    dRes_INDEX_E_DK_BTK_DK_WAIT_e=0x1B,
};

enum dRes_ID_E_DK {
    /* BCK  */
    dRes_ID_E_DK_BCK_DK_ATTACK_e=0x6,
    dRes_ID_E_DK_BCK_DK_CHARGE_e=0x7,
    dRes_ID_E_DK_BCK_DK_C_ATTACK_e=0x8,
    dRes_ID_E_DK_BCK_DK_C_CHARGE_e=0x9,
    dRes_ID_E_DK_BCK_DK_C_DEAD_e=0xA,
    dRes_ID_E_DK_BCK_DK_C_GUARD_e=0xB,
    dRes_ID_E_DK_BCK_DK_C_WAIT_e=0xC,
    dRes_ID_E_DK_BCK_DK_GUARD_e=0xD,
    dRes_ID_E_DK_BCK_DK_SHELLDEAD_e=0xE,
    dRes_ID_E_DK_BCK_DK_WAIT_e=0xF,
    /* BMDR */
    dRes_ID_E_DK_BMD_DK_e=0x12,
    dRes_ID_E_DK_BMD_DK_CORE_e=0x13,
    /* BRK  */
    dRes_ID_E_DK_BRK_DK_ATTACK_e=0x16,
    dRes_ID_E_DK_BRK_DK_WAIT_e=0x17,
    /* BTK  */
    dRes_ID_E_DK_BTK_DK_ATTACK_e=0x1A,
    dRes_ID_E_DK_BTK_DK_WAIT_e=0x1B,
};

enum DK_JNT {
    DK_JNT_WORLD_ROOT_e=0x0,
    DK_JNT_BACKBONE_e=0x1,
    DK_JNT_ARM_A_1_e=0x2,
    DK_JNT_ARM_A_2_e=0x3,
    DK_JNT_ARM_A_3_e=0x4,
    DK_JNT_ARM_B_1_e=0x5,
    DK_JNT_ARM_B_2_e=0x6,
    DK_JNT_ARM_B_3_e=0x7,
    DK_JNT_ARM_C_1_e=0x8,
    DK_JNT_ARM_C_2_e=0x9,
    DK_JNT_ARM_C_3_e=0xA,
    DK_JNT_ARM_D_1_e=0xB,
    DK_JNT_ARM_D_2_e=0xC,
    DK_JNT_ARM_D_3_e=0xD,
    DK_JNT_HEAD_1_e=0xE,
    DK_JNT_UMBRELLA_1_e=0xF,
    DK_JNT_UMBRELLA_2_e=0x10,
    DK_JNT_UMBRELLA_3_e=0x11,
    DK_JNT_TAIL_2_e=0x12,
    DK_JNT_TAIL_3_e=0x13,
    DK_JNT_TAIL_4_e=0x14,
};

enum DK_CORE_JNT {
    DK_CORE_JNT_LOCATOR1_e=0x0,
    DK_CORE_JNT_CORE1_e=0x1,
    DK_CORE_JNT_CORE2_e=0x2,
};

#endif /* !RES_E_DK_H */