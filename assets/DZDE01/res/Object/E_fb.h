#ifndef RES_E_FB_H
#define RES_E_FB_H

enum dRes_INDEX_E_FB {
    /* BCK  */
    dRes_INDEX_E_FB_BCK_BA_APPEAR_e=0x4,
    dRes_INDEX_E_FB_BCK_BA_DEAD_e=0x5,
    dRes_INDEX_E_FB_BCK_BA_FLY_e=0x6,
    dRes_INDEX_E_FB_BCK_BA_FURA2_e=0x7,
    dRes_INDEX_E_FB_BCK_BA_HOLDWAIT_e=0x8,
    dRes_INDEX_E_FB_BCK_BA_HOVERING_e=0x9,
    dRes_INDEX_E_FB_BCK_BA_WAIT_e=0xA,
    /* BMDR */
    dRes_INDEX_E_FB_BMD_FB_e=0xD,
};

enum dRes_ID_E_FB {
    /* BCK  */
    dRes_ID_E_FB_BCK_BA_APPEAR_e=0x4,
    dRes_ID_E_FB_BCK_BA_DEAD_e=0x5,
    dRes_ID_E_FB_BCK_BA_FLY_e=0x6,
    dRes_ID_E_FB_BCK_BA_FURA2_e=0x7,
    dRes_ID_E_FB_BCK_BA_HOLDWAIT_e=0x8,
    dRes_ID_E_FB_BCK_BA_HOVERING_e=0x9,
    dRes_ID_E_FB_BCK_BA_WAIT_e=0xA,
    /* BMDR */
    dRes_ID_E_FB_BMD_FB_e=0xD,
};

enum FB_JNT {
    FB_JNT_WORLD_ROOT_e=0x0,
    FB_JNT_BACKBONE_e=0x1,
    FB_JNT_HEAD_e=0x2,
    FB_JNT_CHIN_e=0x3,
    FB_JNT_EARL_e=0x4,
    FB_JNT_EARR_e=0x5,
    FB_JNT_TONGUE1_e=0x6,
    FB_JNT_TONGUE2_e=0x7,
    FB_JNT_TONGUE3_e=0x8,
    FB_JNT_TONGUE4_e=0x9,
    FB_JNT_WINGL1_e=0xA,
    FB_JNT_WINGL2_e=0xB,
    FB_JNT_WINGL3_e=0xC,
    FB_JNT_WINGL4_e=0xD,
    FB_JNT_WINGR1_e=0xE,
    FB_JNT_WINGR2_e=0xF,
    FB_JNT_WINGR3_e=0x10,
    FB_JNT_WINGR4_e=0x11,
    FB_JNT_WAIST_e=0x12,
    FB_JNT_LEGL_e=0x13,
    FB_JNT_FOOTL_e=0x14,
    FB_JNT_LEGR_e=0x15,
    FB_JNT_FOOTR_e=0x16,
    FB_JNT_TAIL1_e=0x17,
    FB_JNT_TAIL2_e=0x18,
    FB_JNT_TAIL3_e=0x19,
    FB_JNT_TAIL4_e=0x1A,
};

#endif /* !RES_E_FB_H */