#ifndef RES_E_HB_H
#define RES_E_HB_H

enum dRes_INDEX_E_HB {
    /* BCK  */
    dRes_INDEX_E_HB_BCK_HB_APPEAR_e=0x5,
    dRes_INDEX_E_HB_BCK_HB_ATTACK_e=0x6,
    dRes_INDEX_E_HB_BCK_HB_BITE_e=0x7,
    dRes_INDEX_E_HB_BCK_HB_CHANCE1_e=0x8,
    dRes_INDEX_E_HB_BCK_HB_CHANCE2_e=0x9,
    dRes_INDEX_E_HB_BCK_HB_CHANCE3_e=0xA,
    dRes_INDEX_E_HB_BCK_HB_DAMAGE_e=0xB,
    dRes_INDEX_E_HB_BCK_HB_DAMAGE2_e=0xC,
    dRes_INDEX_E_HB_BCK_HB_DEFAULTPOSE_e=0xD,
    dRes_INDEX_E_HB_BCK_HB_DIE_e=0xE,
    dRes_INDEX_E_HB_BCK_HB_ESCAPE_e=0xF,
    dRes_INDEX_E_HB_BCK_HB_FREE_e=0x10,
    dRes_INDEX_E_HB_BCK_HB_REVIVE_e=0x11,
    dRes_INDEX_E_HB_BCK_HB_WAIT_e=0x12,
    dRes_INDEX_E_HB_BCK_HL_SHAKE1_e=0x13,
    dRes_INDEX_E_HB_BCK_HL_SHAKE2_e=0x14,
    /* BMDR */
    dRes_INDEX_E_HB_BMD_HB_e=0x17,
    dRes_INDEX_E_HB_BMD_HL_e=0x18,
    dRes_INDEX_E_HB_BMD_HT_e=0x19,
    dRes_INDEX_E_HB_BMD_K_HB00_e=0x1A,
    /* TEXT */
    dRes_INDEX_E_HB_BTI_HB_STALK_e=0x1D,
};

enum dRes_ID_E_HB {
    /* BCK  */
    dRes_ID_E_HB_BCK_HB_APPEAR_e=0x5,
    dRes_ID_E_HB_BCK_HB_ATTACK_e=0x6,
    dRes_ID_E_HB_BCK_HB_BITE_e=0x7,
    dRes_ID_E_HB_BCK_HB_CHANCE1_e=0x8,
    dRes_ID_E_HB_BCK_HB_CHANCE2_e=0x9,
    dRes_ID_E_HB_BCK_HB_CHANCE3_e=0xA,
    dRes_ID_E_HB_BCK_HB_DAMAGE_e=0xB,
    dRes_ID_E_HB_BCK_HB_DAMAGE2_e=0xC,
    dRes_ID_E_HB_BCK_HB_DEFAULTPOSE_e=0xD,
    dRes_ID_E_HB_BCK_HB_DIE_e=0xE,
    dRes_ID_E_HB_BCK_HB_ESCAPE_e=0xF,
    dRes_ID_E_HB_BCK_HB_FREE_e=0x10,
    dRes_ID_E_HB_BCK_HB_REVIVE_e=0x11,
    dRes_ID_E_HB_BCK_HB_WAIT_e=0x12,
    dRes_ID_E_HB_BCK_HL_SHAKE1_e=0x13,
    dRes_ID_E_HB_BCK_HL_SHAKE2_e=0x14,
    /* BMDR */
    dRes_ID_E_HB_BMD_HB_e=0x17,
    dRes_ID_E_HB_BMD_HL_e=0x18,
    dRes_ID_E_HB_BMD_HT_e=0x19,
    dRes_ID_E_HB_BMD_K_HB00_e=0x1A,
    /* TEXT */
    dRes_ID_E_HB_BTI_HB_STALK_e=0x1D,
};

enum HB_JNT {
    HB_JNT_CENTER_e=0x0,
    HB_JNT_NECK_e=0x1,
    HB_JNT_MOUTH_1_e=0x2,
    HB_JNT_TONGUE_1_e=0x3,
    HB_JNT_TONGUE_2_e=0x4,
    HB_JNT_TONGUE_3_e=0x5,
    HB_JNT_MOUTH_2_e=0x6,
};

enum HL_JNT {
    HL_JNT_CENTER_e=0x0,
    HL_JNT_LEAF_1_e=0x1,
    HL_JNT_LEAF_2_e=0x2,
};

enum HT_JNT {
    HT_JNT_DT_e=0x0,
};

enum K_HB00_JNT {
    K_HB00_JNT_WORLD_ROOT_e=0x0,
};

#endif /* !RES_E_HB_H */