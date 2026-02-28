#ifndef RES_E_SW_H
#define RES_E_SW_H

enum dRes_INDEX_E_SW {
    /* BCK  */
    dRes_INDEX_E_SW_BCK_SW_ATTACK_e=0x4,
    dRes_INDEX_E_SW_BCK_SW_ATTACK_END_e=0x5,
    dRes_INDEX_E_SW_BCK_SW_ATTACK_MIDDLE_e=0x6,
    dRes_INDEX_E_SW_BCK_SW_ATTACK_START_e=0x7,
    dRes_INDEX_E_SW_BCK_SW_CHANCE_e=0x8,
    dRes_INDEX_E_SW_BCK_SW_DAMAGE_e=0x9,
    dRes_INDEX_E_SW_BCK_SW_DIE_e=0xA,
    dRes_INDEX_E_SW_BCK_SW_DIVE_e=0xB,
    dRes_INDEX_E_SW_BCK_SW_RUN_e=0xC,
    dRes_INDEX_E_SW_BCK_SW_WAIT_e=0xD,
    /* BMDR */
    dRes_INDEX_E_SW_BMD_SW_e=0x10,
};

enum dRes_ID_E_SW {
    /* BCK  */
    dRes_ID_E_SW_BCK_SW_ATTACK_e=0x4,
    dRes_ID_E_SW_BCK_SW_ATTACK_END_e=0x5,
    dRes_ID_E_SW_BCK_SW_ATTACK_MIDDLE_e=0x6,
    dRes_ID_E_SW_BCK_SW_ATTACK_START_e=0x7,
    dRes_ID_E_SW_BCK_SW_CHANCE_e=0x8,
    dRes_ID_E_SW_BCK_SW_DAMAGE_e=0x9,
    dRes_ID_E_SW_BCK_SW_DIE_e=0xA,
    dRes_ID_E_SW_BCK_SW_DIVE_e=0xB,
    dRes_ID_E_SW_BCK_SW_RUN_e=0xC,
    dRes_ID_E_SW_BCK_SW_WAIT_e=0xD,
    /* BMDR */
    dRes_ID_E_SW_BMD_SW_e=0x10,
};

enum SW_JNT {
    SW_JNT_WORLD_ROOT_e=0x0,
    SW_JNT_HEAD_e=0x1,
    SW_JNT_FANG_1_e=0x2,
    SW_JNT_FANG_2_e=0x3,
    SW_JNT_FANG_3_e=0x4,
    SW_JNT_TAIL_1_e=0x5,
    SW_JNT_TAIL_2_e=0x6,
    SW_JNT_TAIL_3_e=0x7,
    SW_JNT_TAIL_4_e=0x8,
    SW_JNT_TAIL_5_e=0x9,
};

#endif /* !RES_E_SW_H */