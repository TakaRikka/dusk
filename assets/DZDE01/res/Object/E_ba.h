#ifndef RES_E_BA_H
#define RES_E_BA_H

enum dRes_INDEX_E_BA {
    /* BCK  */
    dRes_INDEX_E_BA_BCK_BA_APPEAR_e=0x4,
    dRes_INDEX_E_BA_BCK_BA_DEAD_e=0x5,
    dRes_INDEX_E_BA_BCK_BA_FLY_e=0x6,
    dRes_INDEX_E_BA_BCK_BA_FURA2_e=0x7,
    dRes_INDEX_E_BA_BCK_BA_HOLDWAIT_e=0x8,
    dRes_INDEX_E_BA_BCK_BA_HOVERING_e=0x9,
    dRes_INDEX_E_BA_BCK_BA_WAIT_e=0xA,
    /* BMDR */
    dRes_INDEX_E_BA_BMD_BA_e=0xD,
};

enum dRes_ID_E_BA {
    /* BCK  */
    dRes_ID_E_BA_BCK_BA_APPEAR_e=0x4,
    dRes_ID_E_BA_BCK_BA_DEAD_e=0x5,
    dRes_ID_E_BA_BCK_BA_FLY_e=0x6,
    dRes_ID_E_BA_BCK_BA_FURA2_e=0x7,
    dRes_ID_E_BA_BCK_BA_HOLDWAIT_e=0x8,
    dRes_ID_E_BA_BCK_BA_HOVERING_e=0x9,
    dRes_ID_E_BA_BCK_BA_WAIT_e=0xA,
    /* BMDR */
    dRes_ID_E_BA_BMD_BA_e=0xD,
};

enum BA_JNT {
    BA_JNT_WORLD_ROOT_e=0x0,
    BA_JNT_BACKBONE_e=0x1,
    BA_JNT_HEAD_e=0x2,
    BA_JNT_CHIN_e=0x3,
    BA_JNT_EARL_e=0x4,
    BA_JNT_EARR_e=0x5,
    BA_JNT_TONGUE1_e=0x6,
    BA_JNT_TONGUE2_e=0x7,
    BA_JNT_TONGUE3_e=0x8,
    BA_JNT_TONGUE4_e=0x9,
    BA_JNT_WINGL1_e=0xA,
    BA_JNT_WINGL2_e=0xB,
    BA_JNT_WINGL3_e=0xC,
    BA_JNT_WINGL4_e=0xD,
    BA_JNT_WINGR1_e=0xE,
    BA_JNT_WINGR2_e=0xF,
    BA_JNT_WINGR3_e=0x10,
    BA_JNT_WINGR4_e=0x11,
    BA_JNT_WAIST_e=0x12,
    BA_JNT_LEGL_e=0x13,
    BA_JNT_FOOTL_e=0x14,
    BA_JNT_LEGR_e=0x15,
    BA_JNT_FOOTR_e=0x16,
    BA_JNT_TAIL1_e=0x17,
    BA_JNT_TAIL2_e=0x18,
    BA_JNT_TAIL3_e=0x19,
    BA_JNT_TAIL4_e=0x1A,
};

#endif /* !RES_E_BA_H */