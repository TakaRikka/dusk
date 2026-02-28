#ifndef RES_E_HM_H
#define RES_E_HM_H

enum dRes_INDEX_E_HM {
    /* BCK  */
    dRes_INDEX_E_HM_BCK_HM_ATACK_S_e=0x6,
    dRes_INDEX_E_HM_BCK_HM_DIE_1_e=0x7,
    dRes_INDEX_E_HM_BCK_HM_DIE_2_S_e=0x8,
    dRes_INDEX_E_HM_BCK_HM_FALL_e=0x9,
    dRes_INDEX_E_HM_BCK_HM_FALL_LIVE_e=0xA,
    dRes_INDEX_E_HM_BCK_HM_GUARD_e=0xB,
    dRes_INDEX_E_HM_BCK_HM_SIPPU_e=0xC,
    dRes_INDEX_E_HM_BCK_HM_WAIT_e=0xD,
    dRes_INDEX_E_HM_BCK_HM_WALK_S_e=0xE,
    /* BMDR */
    dRes_INDEX_E_HM_BMD_HM_e=0x11,
    /* BRK  */
    dRes_INDEX_E_HM_BRK_HM_e=0x14,
    /* BTK  */
    dRes_INDEX_E_HM_BTK_HM_e=0x17,
};

enum dRes_ID_E_HM {
    /* BCK  */
    dRes_ID_E_HM_BCK_HM_ATACK_S_e=0x6,
    dRes_ID_E_HM_BCK_HM_DIE_1_e=0x7,
    dRes_ID_E_HM_BCK_HM_DIE_2_S_e=0x8,
    dRes_ID_E_HM_BCK_HM_FALL_e=0x9,
    dRes_ID_E_HM_BCK_HM_FALL_LIVE_e=0xA,
    dRes_ID_E_HM_BCK_HM_GUARD_e=0xB,
    dRes_ID_E_HM_BCK_HM_SIPPU_e=0xC,
    dRes_ID_E_HM_BCK_HM_WAIT_e=0xD,
    dRes_ID_E_HM_BCK_HM_WALK_S_e=0xE,
    /* BMDR */
    dRes_ID_E_HM_BMD_HM_e=0x11,
    /* BRK  */
    dRes_ID_E_HM_BRK_HM_e=0x14,
    /* BTK  */
    dRes_ID_E_HM_BTK_HM_e=0x17,
};

enum HM_JNT {
    HM_JNT_WORLD_ROOT_e=0x0,
    HM_JNT_HEAD_e=0x1,
    HM_JNT_EYE_L_e=0x2,
    HM_JNT_EYE_R_e=0x3,
    HM_JNT_TAIL_1_e=0x4,
    HM_JNT_TAIL_2_e=0x5,
    HM_JNT_TAIL_3_e=0x6,
    HM_JNT_TAIL_4_e=0x7,
    HM_JNT_TAIL_END_e=0x8,
};

#endif /* !RES_E_HM_H */