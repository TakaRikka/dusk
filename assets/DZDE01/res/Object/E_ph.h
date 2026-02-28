#ifndef RES_E_PH_H
#define RES_E_PH_H

enum dRes_INDEX_E_PH {
    /* BCK  */
    dRes_INDEX_E_PH_BCK_PH_APPER_e=0x4,
    dRes_INDEX_E_PH_BCK_PH_DAMAGE_e=0x5,
    dRes_INDEX_E_PH_BCK_PH_DAMAGE_ARROW_e=0x6,
    dRes_INDEX_E_PH_BCK_PH_DEAD_e=0x7,
    dRes_INDEX_E_PH_BCK_PH_FALL_e=0x8,
    dRes_INDEX_E_PH_BCK_PH_FALL_WAIT_e=0x9,
    dRes_INDEX_E_PH_BCK_PH_HANG_EN_e=0xA,
    dRes_INDEX_E_PH_BCK_PH_HANG_ST_e=0xB,
    dRes_INDEX_E_PH_BCK_PH_HANG_WAIT_e=0xC,
    dRes_INDEX_E_PH_BCK_PH_LEAVE_e=0xD,
    dRes_INDEX_E_PH_BCK_PH_WAIT_e=0xE,
    dRes_INDEX_E_PH_BCK_PH_WAKE_e=0xF,
    dRes_INDEX_E_PH_BCK_PH_WIND_e=0x10,
    dRes_INDEX_E_PH_BCK_PH_WIND_FALL_e=0x11,
    /* BMDR */
    dRes_INDEX_E_PH_BMD_PH_e=0x14,
};

enum dRes_ID_E_PH {
    /* BCK  */
    dRes_ID_E_PH_BCK_PH_APPER_e=0x4,
    dRes_ID_E_PH_BCK_PH_DAMAGE_e=0x5,
    dRes_ID_E_PH_BCK_PH_DAMAGE_ARROW_e=0x6,
    dRes_ID_E_PH_BCK_PH_DEAD_e=0x7,
    dRes_ID_E_PH_BCK_PH_FALL_e=0x8,
    dRes_ID_E_PH_BCK_PH_FALL_WAIT_e=0x9,
    dRes_ID_E_PH_BCK_PH_HANG_EN_e=0xA,
    dRes_ID_E_PH_BCK_PH_HANG_ST_e=0xB,
    dRes_ID_E_PH_BCK_PH_HANG_WAIT_e=0xC,
    dRes_ID_E_PH_BCK_PH_LEAVE_e=0xD,
    dRes_ID_E_PH_BCK_PH_WAIT_e=0xE,
    dRes_ID_E_PH_BCK_PH_WAKE_e=0xF,
    dRes_ID_E_PH_BCK_PH_WIND_e=0x10,
    dRes_ID_E_PH_BCK_PH_WIND_FALL_e=0x11,
    /* BMDR */
    dRes_ID_E_PH_BMD_PH_e=0x14,
};

enum PH_JNT {
    PH_JNT_WORLD_ROOT_e=0x0,
    PH_JNT_BODY_e=0x1,
    PH_JNT_HEAD_e=0x2,
    PH_JNT_WIN_D_e=0x3,
    PH_JNT_WING_A_e=0x4,
    PH_JNT_WING_B_e=0x5,
    PH_JNT_WINGC_e=0x6,
    PH_JNT_TAIL_e=0x7,
};

#endif /* !RES_E_PH_H */