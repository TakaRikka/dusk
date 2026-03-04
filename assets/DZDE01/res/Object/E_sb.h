#ifndef RES_E_SB_H
#define RES_E_SB_H

enum dRes_INDEX_E_SB {
    /* BCK  */
    dRes_INDEX_E_SB_BCK_SB_ATTACK_e=0x4,
    dRes_INDEX_E_SB_BCK_SB_BIND_e=0x5,
    dRes_INDEX_E_SB_BCK_SB_DEATH_e=0x6,
    dRes_INDEX_E_SB_BCK_SB_DEFENCE_e=0x7,
    dRes_INDEX_E_SB_BCK_SB_JUMP_A_e=0x8,
    dRes_INDEX_E_SB_BCK_SB_JUMP_B_e=0x9,
    dRes_INDEX_E_SB_BCK_SB_JUMP_C_e=0xA,
    dRes_INDEX_E_SB_BCK_SB_SHIELD_e=0xB,
    dRes_INDEX_E_SB_BCK_SB_WAIT_e=0xC,
    /* BMDR */
    dRes_INDEX_E_SB_BMD_SB_e=0xF,
};

enum dRes_ID_E_SB {
    /* BCK  */
    dRes_ID_E_SB_BCK_SB_ATTACK_e=0x4,
    dRes_ID_E_SB_BCK_SB_BIND_e=0x5,
    dRes_ID_E_SB_BCK_SB_DEATH_e=0x6,
    dRes_ID_E_SB_BCK_SB_DEFENCE_e=0x7,
    dRes_ID_E_SB_BCK_SB_JUMP_A_e=0x8,
    dRes_ID_E_SB_BCK_SB_JUMP_B_e=0x9,
    dRes_ID_E_SB_BCK_SB_JUMP_C_e=0xA,
    dRes_ID_E_SB_BCK_SB_SHIELD_e=0xB,
    dRes_ID_E_SB_BCK_SB_WAIT_e=0xC,
    /* BMDR */
    dRes_ID_E_SB_BMD_SB_e=0xF,
};

enum SB_JNT {
    SB_JNT_WORLD_ROOT_e=0x0,
    SB_JNT_SHELL_TOP01_e=0x1,
    SB_JNT_SHELL_TOP02_e=0x2,
    SB_JNT_SHELL_UNDER01_e=0x3,
    SB_JNT_SHELL_UNDER02_e=0x4,
};

#endif /* !RES_E_SB_H */