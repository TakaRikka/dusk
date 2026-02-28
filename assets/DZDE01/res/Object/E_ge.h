#ifndef RES_E_GE_H
#define RES_E_GE_H

enum dRes_INDEX_E_GE {
    /* BCK  */
    dRes_INDEX_E_GE_BCK_GE_CAW_e=0x4,
    dRes_INDEX_E_GE_BCK_GE_CAW_TREE_e=0x5,
    dRes_INDEX_E_GE_BCK_GE_DESCENT_e=0x6,
    dRes_INDEX_E_GE_BCK_GE_FLY_e=0x7,
    dRes_INDEX_E_GE_BCK_GE_FLY_AWAY_e=0x8,
    dRes_INDEX_E_GE_BCK_GE_FURA2_e=0x9,
    dRes_INDEX_E_GE_BCK_GE_GLIDE_e=0xA,
    dRes_INDEX_E_GE_BCK_GE_LANDING_e=0xB,
    dRes_INDEX_E_GE_BCK_GE_STAY_e=0xC,
    /* BMDR */
    dRes_INDEX_E_GE_BMD_GE_MODEL_e=0xF,
};

enum dRes_ID_E_GE {
    /* BCK  */
    dRes_ID_E_GE_BCK_GE_CAW_e=0x4,
    dRes_ID_E_GE_BCK_GE_CAW_TREE_e=0x5,
    dRes_ID_E_GE_BCK_GE_DESCENT_e=0x6,
    dRes_ID_E_GE_BCK_GE_FLY_e=0x7,
    dRes_ID_E_GE_BCK_GE_FLY_AWAY_e=0x8,
    dRes_ID_E_GE_BCK_GE_FURA2_e=0x9,
    dRes_ID_E_GE_BCK_GE_GLIDE_e=0xA,
    dRes_ID_E_GE_BCK_GE_LANDING_e=0xB,
    dRes_ID_E_GE_BCK_GE_STAY_e=0xC,
    /* BMDR */
    dRes_ID_E_GE_BMD_GE_MODEL_e=0xF,
};

enum GE_MODEL_JNT {
    GE_MODEL_JNT_WORLD_ROOT_e=0x0,
    GE_MODEL_JNT_BODY1_e=0x1,
    GE_MODEL_JNT_NECK_e=0x2,
    GE_MODEL_JNT_HEAD_e=0x3,
    GE_MODEL_JNT_JAW_e=0x4,
    GE_MODEL_JNT_WING_L1_e=0x5,
    GE_MODEL_JNT_WING_L2_e=0x6,
    GE_MODEL_JNT_WING_R1_e=0x7,
    GE_MODEL_JNT_WING_R2_e=0x8,
    GE_MODEL_JNT_TAIL_e=0x9,
    GE_MODEL_JNT_LEG_e=0xA,
};

#endif /* !RES_E_GE_H */