#ifndef RES_E_MD_H
#define RES_E_MD_H

enum dRes_INDEX_E_MD {
    /* BCK  */
    dRes_INDEX_E_MD_BCK_MD_BREAKOUT_e=0x4,
    dRes_INDEX_E_MD_BCK_MD_WAIT_e=0x5,
    dRes_INDEX_E_MD_BCK_YARI_BREAKOUT_e=0x6,
    /* BMDR */
    dRes_INDEX_E_MD_BMD_LV9_MD_e=0x9,
    dRes_INDEX_E_MD_BMD_LV9_YARI_e=0xA,
    dRes_INDEX_E_MD_BMD_MD_e=0xB,
    dRes_INDEX_E_MD_BMD_MD_TB_e=0xC,
    dRes_INDEX_E_MD_BMD_MD_YARI_e=0xD,
};

enum dRes_ID_E_MD {
    /* BCK  */
    dRes_ID_E_MD_BCK_MD_BREAKOUT_e=0x4,
    dRes_ID_E_MD_BCK_MD_WAIT_e=0x5,
    dRes_ID_E_MD_BCK_YARI_BREAKOUT_e=0x6,
    /* BMDR */
    dRes_ID_E_MD_BMD_LV9_MD_e=0x9,
    dRes_ID_E_MD_BMD_LV9_YARI_e=0xA,
    dRes_ID_E_MD_BMD_MD_e=0xB,
    dRes_ID_E_MD_BMD_MD_TB_e=0xC,
    dRes_ID_E_MD_BMD_MD_YARI_e=0xD,
};

enum LV9_MD_JNT {
    LV9_MD_JNT_CENTER_e=0x0,
    LV9_MD_JNT_ARM_L_1_e=0x1,
    LV9_MD_JNT_ARM_L_2_e=0x2,
    LV9_MD_JNT_ARM_R_1_e=0x3,
    LV9_MD_JNT_ARM_R_2_e=0x4,
    LV9_MD_JNT_BACKBONE_1_e=0x5,
    LV9_MD_JNT_BACKBONE_2_e=0x6,
    LV9_MD_JNT_HAND_L_e=0x7,
    LV9_MD_JNT_HAND_R_e=0x8,
    LV9_MD_JNT_HEAD_e=0x9,
    LV9_MD_JNT_LEG_L_1_e=0xA,
    LV9_MD_JNT_LEG_L_2_e=0xB,
    LV9_MD_JNT_LEG_R_1_e=0xC,
    LV9_MD_JNT_LEG_R_2_e=0xD,
    LV9_MD_JNT_SHOLDER_L_e=0xE,
    LV9_MD_JNT_SHOLDER_R_e=0xF,
    LV9_MD_JNT_WAIST_e=0x10,
};

enum LV9_YARI_JNT {
    LV9_YARI_JNT_LV9_MD_YARI_e=0x0,
};

enum MD_JNT {
    MD_JNT_CENTER_e=0x0,
    MD_JNT_ARM_L_1_e=0x1,
    MD_JNT_ARM_L_2_e=0x2,
    MD_JNT_ARM_R_1_e=0x3,
    MD_JNT_ARM_R_2_e=0x4,
    MD_JNT_BACKBONE_1_e=0x5,
    MD_JNT_BACKBONE_2_e=0x6,
    MD_JNT_HAND_L_e=0x7,
    MD_JNT_HAND_R_e=0x8,
    MD_JNT_HEAD_e=0x9,
    MD_JNT_LEG_L_1_e=0xA,
    MD_JNT_LEG_L_2_e=0xB,
    MD_JNT_LEG_R_1_e=0xC,
    MD_JNT_LEG_R_2_e=0xD,
    MD_JNT_SHOLDER_L_e=0xE,
    MD_JNT_SHOLDER_R_e=0xF,
    MD_JNT_WAIST_e=0x10,
};

enum MD_TB_JNT {
    MD_TB_JNT_MB_TB_MODEL_e=0x0,
};

enum MD_YARI_JNT {
    MD_YARI_JNT_MD_YARI_MODEL_e=0x0,
};

#endif /* !RES_E_MD_H */