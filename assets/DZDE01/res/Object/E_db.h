#ifndef RES_E_DB_H
#define RES_E_DB_H

enum dRes_INDEX_E_DB {
    /* BCK  */
    dRes_INDEX_E_DB_BCK_DB_APPEAR_e=0x5,
    dRes_INDEX_E_DB_BCK_DB_ATTACK_e=0x6,
    dRes_INDEX_E_DB_BCK_DB_BITE_e=0x7,
    dRes_INDEX_E_DB_BCK_DB_CHANCE1_e=0x8,
    dRes_INDEX_E_DB_BCK_DB_CHANCE2_e=0x9,
    dRes_INDEX_E_DB_BCK_DB_CHANCE3_e=0xA,
    dRes_INDEX_E_DB_BCK_DB_DAMAGE_e=0xB,
    dRes_INDEX_E_DB_BCK_DB_DAMAGE2_e=0xC,
    dRes_INDEX_E_DB_BCK_DB_DEFAULTPOSE_e=0xD,
    dRes_INDEX_E_DB_BCK_DB_DIE_e=0xE,
    dRes_INDEX_E_DB_BCK_DB_ESCAPE_e=0xF,
    dRes_INDEX_E_DB_BCK_DB_FREE_e=0x10,
    dRes_INDEX_E_DB_BCK_DB_REVIVE_e=0x11,
    dRes_INDEX_E_DB_BCK_DB_WAIT_e=0x12,
    dRes_INDEX_E_DB_BCK_DL_SHAKE1_e=0x13,
    dRes_INDEX_E_DB_BCK_DL_SHAKE2_e=0x14,
    /* BMDR */
    dRes_INDEX_E_DB_BMD_DB_e=0x17,
    dRes_INDEX_E_DB_BMD_DL_e=0x18,
    dRes_INDEX_E_DB_BMD_DT_e=0x19,
    /* TEXT */
    dRes_INDEX_E_DB_BTI_DB_STALK_e=0x1C,
};

enum dRes_ID_E_DB {
    /* BCK  */
    dRes_ID_E_DB_BCK_DB_APPEAR_e=0x5,
    dRes_ID_E_DB_BCK_DB_ATTACK_e=0x6,
    dRes_ID_E_DB_BCK_DB_BITE_e=0x7,
    dRes_ID_E_DB_BCK_DB_CHANCE1_e=0x8,
    dRes_ID_E_DB_BCK_DB_CHANCE2_e=0x9,
    dRes_ID_E_DB_BCK_DB_CHANCE3_e=0xA,
    dRes_ID_E_DB_BCK_DB_DAMAGE_e=0xB,
    dRes_ID_E_DB_BCK_DB_DAMAGE2_e=0xC,
    dRes_ID_E_DB_BCK_DB_DEFAULTPOSE_e=0xD,
    dRes_ID_E_DB_BCK_DB_DIE_e=0xE,
    dRes_ID_E_DB_BCK_DB_ESCAPE_e=0xF,
    dRes_ID_E_DB_BCK_DB_FREE_e=0x10,
    dRes_ID_E_DB_BCK_DB_REVIVE_e=0x11,
    dRes_ID_E_DB_BCK_DB_WAIT_e=0x12,
    dRes_ID_E_DB_BCK_DL_SHAKE1_e=0x13,
    dRes_ID_E_DB_BCK_DL_SHAKE2_e=0x14,
    /* BMDR */
    dRes_ID_E_DB_BMD_DB_e=0x17,
    dRes_ID_E_DB_BMD_DL_e=0x18,
    dRes_ID_E_DB_BMD_DT_e=0x19,
    /* TEXT */
    dRes_ID_E_DB_BTI_DB_STALK_e=0x1C,
};

enum DB_JNT {
    DB_JNT_CENTER_e=0x0,
    DB_JNT_NECK_e=0x1,
    DB_JNT_MOUTH_1_e=0x2,
    DB_JNT_TONGUE_1_e=0x3,
    DB_JNT_TONGUE_2_e=0x4,
    DB_JNT_TONGUE_3_e=0x5,
    DB_JNT_MOUTH_2_e=0x6,
};

enum DL_JNT {
    DL_JNT_CENTER_e=0x0,
    DL_JNT_LEAF_1_e=0x1,
    DL_JNT_LEAF_2_e=0x2,
};

enum DT_JNT {
    DT_JNT_HT_e=0x0,
};

#endif /* !RES_E_DB_H */