#ifndef RES_E_GS_H
#define RES_E_GS_H

enum dRes_INDEX_E_GS {
    /* BCK  */
    dRes_INDEX_E_GS_BCK_GHO_GT_POINT_e=0x4,
    dRes_INDEX_E_GS_BCK_GHO_GT_POINTING_e=0x5,
    dRes_INDEX_E_GS_BCK_GHO_GT_WAIT_A_e=0x6,
    /* BMDR */
    dRes_INDEX_E_GS_BMD_GHO_GT_e=0x9,
};

enum dRes_ID_E_GS {
    /* BCK  */
    dRes_ID_E_GS_BCK_GHO_GT_POINT_e=0x4,
    dRes_ID_E_GS_BCK_GHO_GT_POINTING_e=0x5,
    dRes_ID_E_GS_BCK_GHO_GT_WAIT_A_e=0x6,
    /* BMDR */
    dRes_ID_E_GS_BMD_GHO_GT_e=0x9,
};

enum GHO_GT_JNT {
    GHO_GT_JNT_CENTER_e=0x0,
    GHO_GT_JNT_BACKBONE1_e=0x1,
    GHO_GT_JNT_BACKBONE2_e=0x2,
    GHO_GT_JNT_NECK_e=0x3,
    GHO_GT_JNT_HEAD_e=0x4,
    GHO_GT_JNT_SHOULDERL_e=0x5,
    GHO_GT_JNT_ARM1L_e=0x6,
    GHO_GT_JNT_ARM2L_e=0x7,
    GHO_GT_JNT_HANDL_e=0x8,
    GHO_GT_JNT_SHOULDERR_e=0x9,
    GHO_GT_JNT_ARM1R_e=0xA,
    GHO_GT_JNT_ARM2R_e=0xB,
    GHO_GT_JNT_HANDR_e=0xC,
    GHO_GT_JNT_WAIST_e=0xD,
    GHO_GT_JNT_LEG1L_e=0xE,
    GHO_GT_JNT_LEG2L_e=0xF,
    GHO_GT_JNT_FOOTL_e=0x10,
    GHO_GT_JNT_LEG1R_e=0x11,
    GHO_GT_JNT_LEG2R_e=0x12,
    GHO_GT_JNT_FOOTR_e=0x13,
};

#endif /* !RES_E_GS_H */