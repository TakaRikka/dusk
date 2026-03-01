#ifndef RES_E_RB_H
#define RES_E_RB_H

enum dRes_INDEX_E_RB {
    /* BCK  */
    dRes_INDEX_E_RB_BCK_RB_ANGRY_e=0x4,
    dRes_INDEX_E_RB_BCK_RB_APPEAR_e=0x5,
    dRes_INDEX_E_RB_BCK_RB_ATTACK_e=0x6,
    dRes_INDEX_E_RB_BCK_RB_DOWN_e=0x7,
    dRes_INDEX_E_RB_BCK_RB_WAIT_e=0x8,
    /* BMDR */
    dRes_INDEX_E_RB_BMD_RB_e=0xB,
};

enum dRes_ID_E_RB {
    /* BCK  */
    dRes_ID_E_RB_BCK_RB_ANGRY_e=0x4,
    dRes_ID_E_RB_BCK_RB_APPEAR_e=0x5,
    dRes_ID_E_RB_BCK_RB_ATTACK_e=0x6,
    dRes_ID_E_RB_BCK_RB_DOWN_e=0x7,
    dRes_ID_E_RB_BCK_RB_WAIT_e=0x8,
    /* BMDR */
    dRes_ID_E_RB_BMD_RB_e=0xB,
};

enum RB_JNT {
    RB_JNT_CENTER_e=0x0,
    RB_JNT_ARM_A1_e=0x1,
    RB_JNT_ARM_A2_e=0x2,
    RB_JNT_ARM_B1_e=0x3,
    RB_JNT_ARM_B2_e=0x4,
    RB_JNT_ARM_C1_e=0x5,
    RB_JNT_ARM_C2_e=0x6,
};

#endif /* !RES_E_RB_H */