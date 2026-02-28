#ifndef RES_E_AI_H
#define RES_E_AI_H

enum dRes_INDEX_E_AI {
    /* BCK  */
    dRes_INDEX_E_AI_BCK_AI_ATTACK_e=0x5,
    dRes_INDEX_E_AI_BCK_AI_ATTACK_DOWN_e=0x6,
    dRes_INDEX_E_AI_BCK_AI_ATTACK_UP_e=0x7,
    dRes_INDEX_E_AI_BCK_AI_DIE_ATTACK_e=0x8,
    dRes_INDEX_E_AI_BCK_AI_MOVE_e=0x9,
    dRes_INDEX_E_AI_BCK_AI_SHOCK_e=0xA,
    /* BMDR */
    dRes_INDEX_E_AI_BMD_AI_e=0xD,
    /* BRK  */
    dRes_INDEX_E_AI_BRK_AI_DAMAGE_e=0x10,
    dRes_INDEX_E_AI_BRK_AI_MOVE_e=0x11,
    dRes_INDEX_E_AI_BRK_AI_START_e=0x12,
    dRes_INDEX_E_AI_BRK_AI_STOP_e=0x13,
};

enum dRes_ID_E_AI {
    /* BCK  */
    dRes_ID_E_AI_BCK_AI_ATTACK_e=0x5,
    dRes_ID_E_AI_BCK_AI_ATTACK_DOWN_e=0x6,
    dRes_ID_E_AI_BCK_AI_ATTACK_UP_e=0x7,
    dRes_ID_E_AI_BCK_AI_DIE_ATTACK_e=0x8,
    dRes_ID_E_AI_BCK_AI_MOVE_e=0x9,
    dRes_ID_E_AI_BCK_AI_SHOCK_e=0xA,
    /* BMDR */
    dRes_ID_E_AI_BMD_AI_e=0xD,
    /* BRK  */
    dRes_ID_E_AI_BRK_AI_DAMAGE_e=0x10,
    dRes_ID_E_AI_BRK_AI_MOVE_e=0x11,
    dRes_ID_E_AI_BRK_AI_START_e=0x12,
    dRes_ID_E_AI_BRK_AI_STOP_e=0x13,
};

enum AI_JNT {
    AI_JNT_CENTER_e=0x0,
    AI_JNT_SHOLDER_e=0x1,
    AI_JNT_HAND_e=0x2,
};

#endif /* !RES_E_AI_H */