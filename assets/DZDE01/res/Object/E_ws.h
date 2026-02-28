#ifndef RES_E_WS_H
#define RES_E_WS_H

enum dRes_INDEX_E_WS {
    /* BCK  */
    dRes_INDEX_E_WS_BCK_WS_ATTACK_e=0x4,
    dRes_INDEX_E_WS_BCK_WS_DAMAGE_e=0x5,
    dRes_INDEX_E_WS_BCK_WS_DEAD_e=0x6,
    dRes_INDEX_E_WS_BCK_WS_MOVE_e=0x7,
    dRes_INDEX_E_WS_BCK_WS_TURN_e=0x8,
    dRes_INDEX_E_WS_BCK_WS_WAIT01_e=0x9,
    dRes_INDEX_E_WS_BCK_WS_WAIT02_e=0xA,
    /* BMDR */
    dRes_INDEX_E_WS_BMD_WS_e=0xD,
};

enum dRes_ID_E_WS {
    /* BCK  */
    dRes_ID_E_WS_BCK_WS_ATTACK_e=0x4,
    dRes_ID_E_WS_BCK_WS_DAMAGE_e=0x5,
    dRes_ID_E_WS_BCK_WS_DEAD_e=0x6,
    dRes_ID_E_WS_BCK_WS_MOVE_e=0x7,
    dRes_ID_E_WS_BCK_WS_TURN_e=0x8,
    dRes_ID_E_WS_BCK_WS_WAIT01_e=0x9,
    dRes_ID_E_WS_BCK_WS_WAIT02_e=0xA,
    /* BMDR */
    dRes_ID_E_WS_BMD_WS_e=0xD,
};

enum WS_JNT {
    WS_JNT_WORLD_ROOT_e=0x0,
    WS_JNT_HEAD_e=0x1,
    WS_JNT_LLEGA_1_e=0x2,
    WS_JNT_LLEGA_2_e=0x3,
    WS_JNT_LLEGA_3_e=0x4,
    WS_JNT_LLEGB_1_e=0x5,
    WS_JNT_LLEGB_2_e=0x6,
    WS_JNT_LLEGB_3_e=0x7,
    WS_JNT_LLEGC_1_e=0x8,
    WS_JNT_LLEGC_2_e=0x9,
    WS_JNT_LLEGC_3_e=0xA,
    WS_JNT_LLEGD_1_e=0xB,
    WS_JNT_LLEGD_2_e=0xC,
    WS_JNT_LLEGD_3_e=0xD,
    WS_JNT_RLEGA_1_e=0xE,
    WS_JNT_RLEGA_2_e=0xF,
    WS_JNT_RLEGA_3_e=0x10,
    WS_JNT_RLEGB_1_e=0x11,
    WS_JNT_RLEGB_2_e=0x12,
    WS_JNT_RLEGB_3_e=0x13,
    WS_JNT_RLEGC_1_e=0x14,
    WS_JNT_RLEGC_2_e=0x15,
    WS_JNT_RLEGC_3_e=0x16,
    WS_JNT_RLEGD_1_e=0x17,
    WS_JNT_RLEGD_2_e=0x18,
    WS_JNT_RLEGD_3_e=0x19,
    WS_JNT_AGOL_e=0x1A,
    WS_JNT_AGOR_e=0x1B,
    WS_JNT_HANDL1_e=0x1C,
    WS_JNT_HANDL2_e=0x1D,
    WS_JNT_HANDR1_e=0x1E,
    WS_JNT_HANDR2_e=0x1F,
    WS_JNT_WEIST_e=0x20,
};

#endif /* !RES_E_WS_H */