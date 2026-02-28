#ifndef RES_E_YD_H
#define RES_E_YD_H

enum dRes_INDEX_E_YD {
    /* BCK  */
    dRes_INDEX_E_YD_BCK_HB_APPEAR_e=0x6,
    dRes_INDEX_E_YD_BCK_HB_ATTACK_e=0x7,
    dRes_INDEX_E_YD_BCK_HB_BITE_e=0x8,
    dRes_INDEX_E_YD_BCK_HB_CHANCE1_e=0x9,
    dRes_INDEX_E_YD_BCK_HB_CHANCE2_e=0xA,
    dRes_INDEX_E_YD_BCK_HB_CHANCE3_e=0xB,
    dRes_INDEX_E_YD_BCK_HB_DAMAGE_e=0xC,
    dRes_INDEX_E_YD_BCK_HB_DAMAGE2_e=0xD,
    dRes_INDEX_E_YD_BCK_HB_DEFAULTPOSE_e=0xE,
    dRes_INDEX_E_YD_BCK_HB_DIE_e=0xF,
    dRes_INDEX_E_YD_BCK_HB_ESCAPE_e=0x10,
    dRes_INDEX_E_YD_BCK_HB_FREE_e=0x11,
    dRes_INDEX_E_YD_BCK_HB_REVIVE_e=0x12,
    dRes_INDEX_E_YD_BCK_HB_WAIT_e=0x13,
    dRes_INDEX_E_YD_BCK_HL_SHAKE1_e=0x14,
    dRes_INDEX_E_YD_BCK_HL_SHAKE2_e=0x15,
    /* BMDR */
    dRes_INDEX_E_YD_BMD_YD_e=0x18,
    dRes_INDEX_E_YD_BMD_YL_e=0x19,
    dRes_INDEX_E_YD_BMD_YT_e=0x1A,
    /* BRK  */
    dRes_INDEX_E_YD_BRK_YD_e=0x1D,
    /* TEXT */
    dRes_INDEX_E_YD_BTI_YD_STALK_e=0x20,
};

enum dRes_ID_E_YD {
    /* BCK  */
    dRes_ID_E_YD_BCK_HB_APPEAR_e=0x6,
    dRes_ID_E_YD_BCK_HB_ATTACK_e=0x7,
    dRes_ID_E_YD_BCK_HB_BITE_e=0x8,
    dRes_ID_E_YD_BCK_HB_CHANCE1_e=0x9,
    dRes_ID_E_YD_BCK_HB_CHANCE2_e=0xA,
    dRes_ID_E_YD_BCK_HB_CHANCE3_e=0xB,
    dRes_ID_E_YD_BCK_HB_DAMAGE_e=0xC,
    dRes_ID_E_YD_BCK_HB_DAMAGE2_e=0xD,
    dRes_ID_E_YD_BCK_HB_DEFAULTPOSE_e=0xE,
    dRes_ID_E_YD_BCK_HB_DIE_e=0xF,
    dRes_ID_E_YD_BCK_HB_ESCAPE_e=0x10,
    dRes_ID_E_YD_BCK_HB_FREE_e=0x11,
    dRes_ID_E_YD_BCK_HB_REVIVE_e=0x12,
    dRes_ID_E_YD_BCK_HB_WAIT_e=0x13,
    dRes_ID_E_YD_BCK_HL_SHAKE1_e=0x14,
    dRes_ID_E_YD_BCK_HL_SHAKE2_e=0x15,
    /* BMDR */
    dRes_ID_E_YD_BMD_YD_e=0x18,
    dRes_ID_E_YD_BMD_YL_e=0x19,
    dRes_ID_E_YD_BMD_YT_e=0x1A,
    /* BRK  */
    dRes_ID_E_YD_BRK_YD_e=0x1D,
    /* TEXT */
    dRes_ID_E_YD_BTI_YD_STALK_e=0x20,
};

enum YD_JNT {
    YD_JNT_CENTER_e=0x0,
    YD_JNT_NECK_e=0x1,
    YD_JNT_MOUTH_1_e=0x2,
    YD_JNT_TONGUE_1_e=0x3,
    YD_JNT_TONGUE_2_e=0x4,
    YD_JNT_TONGUE_3_e=0x5,
    YD_JNT_MOUTH_2_e=0x6,
};

enum YL_JNT {
    YL_JNT_CENTER_e=0x0,
    YL_JNT_LEAF_1_e=0x1,
    YL_JNT_LEAF_2_e=0x2,
};

enum YT_JNT {
    YT_JNT_YT_e=0x0,
};

#endif /* !RES_E_YD_H */