#ifndef RES_E_YK_H
#define RES_E_YK_H

enum dRes_INDEX_E_YK {
    /* BCK  */
    dRes_INDEX_E_YK_BCK_YK_DEAD_e=0x4,
    dRes_INDEX_E_YK_BCK_YK_FLY_e=0x5,
    dRes_INDEX_E_YK_BCK_YK_FURA2_e=0x6,
    dRes_INDEX_E_YK_BCK_YK_HOLDWAIT_e=0x7,
    dRes_INDEX_E_YK_BCK_YK_HOVERING_e=0x8,
    dRes_INDEX_E_YK_BCK_YK_WAIT_e=0x9,
    /* BMDR */
    dRes_INDEX_E_YK_BMD_YK_e=0xC,
};

enum dRes_ID_E_YK {
    /* BCK  */
    dRes_ID_E_YK_BCK_YK_DEAD_e=0x4,
    dRes_ID_E_YK_BCK_YK_FLY_e=0x5,
    dRes_ID_E_YK_BCK_YK_FURA2_e=0x6,
    dRes_ID_E_YK_BCK_YK_HOLDWAIT_e=0x7,
    dRes_ID_E_YK_BCK_YK_HOVERING_e=0x8,
    dRes_ID_E_YK_BCK_YK_WAIT_e=0x9,
    /* BMDR */
    dRes_ID_E_YK_BMD_YK_e=0xC,
};

enum YK_JNT {
    YK_JNT_WORLD_ROOT_e=0x0,
    YK_JNT_BACKBONE_e=0x1,
    YK_JNT_HEAD_e=0x2,
    YK_JNT_WINGL1_e=0x3,
    YK_JNT_WINGL2_e=0x4,
    YK_JNT_WINGL3_e=0x5,
    YK_JNT_WINGL4_e=0x6,
    YK_JNT_WINGR1_e=0x7,
    YK_JNT_WINGR2_e=0x8,
    YK_JNT_WINGR3_e=0x9,
    YK_JNT_WINGR4_e=0xA,
    YK_JNT_WAIST_e=0xB,
    YK_JNT_LEGL_e=0xC,
    YK_JNT_FOOTL_e=0xD,
    YK_JNT_LEGR_e=0xE,
    YK_JNT_FOOTR_e=0xF,
    YK_JNT_TAIL1_e=0x10,
    YK_JNT_TAIL2_e=0x11,
    YK_JNT_TAIL3_e=0x12,
    YK_JNT_TAIL_4_e=0x13,
};

#endif /* !RES_E_YK_H */