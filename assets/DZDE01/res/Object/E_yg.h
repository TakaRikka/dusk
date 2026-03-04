#ifndef RES_E_YG_H
#define RES_E_YG_H

enum dRes_INDEX_E_YG {
    /* BCK  */
    dRes_INDEX_E_YG_BCK_YG_BITE_DIE_e=0x4,
    dRes_INDEX_E_YG_BCK_YG_DAMAGE_e=0x5,
    dRes_INDEX_E_YG_BCK_YG_DIE_e=0x6,
    dRes_INDEX_E_YG_BCK_YG_DIE_FLOAT_e=0x7,
    dRes_INDEX_E_YG_BCK_YG_FIND_e=0x8,
    dRes_INDEX_E_YG_BCK_YG_GNAW_e=0x9,
    dRes_INDEX_E_YG_BCK_YG_JUMP_END_e=0xA,
    dRes_INDEX_E_YG_BCK_YG_JUMP_MIDDLE_e=0xB,
    dRes_INDEX_E_YG_BCK_YG_JUMP_START_e=0xC,
    dRes_INDEX_E_YG_BCK_YG_RUN_e=0xD,
    dRes_INDEX_E_YG_BCK_YG_SWIM_e=0xE,
    dRes_INDEX_E_YG_BCK_YG_WAIT_e=0xF,
    dRes_INDEX_E_YG_BCK_YG_WALK_e=0x10,
    /* BMDR */
    dRes_INDEX_E_YG_BMD_YG_e=0x13,
};

enum dRes_ID_E_YG {
    /* BCK  */
    dRes_ID_E_YG_BCK_YG_BITE_DIE_e=0x4,
    dRes_ID_E_YG_BCK_YG_DAMAGE_e=0x5,
    dRes_ID_E_YG_BCK_YG_DIE_e=0x6,
    dRes_ID_E_YG_BCK_YG_DIE_FLOAT_e=0x7,
    dRes_ID_E_YG_BCK_YG_FIND_e=0x8,
    dRes_ID_E_YG_BCK_YG_GNAW_e=0x9,
    dRes_ID_E_YG_BCK_YG_JUMP_END_e=0xA,
    dRes_ID_E_YG_BCK_YG_JUMP_MIDDLE_e=0xB,
    dRes_ID_E_YG_BCK_YG_JUMP_START_e=0xC,
    dRes_ID_E_YG_BCK_YG_RUN_e=0xD,
    dRes_ID_E_YG_BCK_YG_SWIM_e=0xE,
    dRes_ID_E_YG_BCK_YG_WAIT_e=0xF,
    dRes_ID_E_YG_BCK_YG_WALK_e=0x10,
    /* BMDR */
    dRes_ID_E_YG_BMD_YG_e=0x13,
};

enum YG_JNT {
    YG_JNT_WORLD_ROOT_e=0x0,
    YG_JNT_BODY_e=0x1,
    YG_JNT_TAIL_01_e=0x2,
    YG_JNT_TAIL_02_e=0x3,
    YG_JNT_TAIL_03_e=0x4,
    YG_JNT_TAIL_04_e=0x5,
};

#endif /* !RES_E_YG_H */