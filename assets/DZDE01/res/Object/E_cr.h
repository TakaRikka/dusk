#ifndef RES_E_CR_H
#define RES_E_CR_H

enum dRes_INDEX_E_CR {
    /* BCK  */
    dRes_INDEX_E_CR_BCK_CR_CHANCE_e=0x4,
    dRes_INDEX_E_CR_BCK_CR_CHANCE_WAIT_e=0x5,
    dRes_INDEX_E_CR_BCK_CR_DAMAGE_e=0x6,
    dRes_INDEX_E_CR_BCK_CR_DIE1_e=0x7,
    dRes_INDEX_E_CR_BCK_CR_DIE2_e=0x8,
    dRes_INDEX_E_CR_BCK_CR_RUN_e=0x9,
    /* BMDR */
    dRes_INDEX_E_CR_BMD_CE_e=0xC,
    dRes_INDEX_E_CR_BMD_CR_e=0xD,
};

enum dRes_ID_E_CR {
    /* BCK  */
    dRes_ID_E_CR_BCK_CR_CHANCE_e=0x4,
    dRes_ID_E_CR_BCK_CR_CHANCE_WAIT_e=0x5,
    dRes_ID_E_CR_BCK_CR_DAMAGE_e=0x6,
    dRes_ID_E_CR_BCK_CR_DIE1_e=0x7,
    dRes_ID_E_CR_BCK_CR_DIE2_e=0x8,
    dRes_ID_E_CR_BCK_CR_RUN_e=0x9,
    /* BMDR */
    dRes_ID_E_CR_BMD_CE_e=0xC,
    dRes_ID_E_CR_BMD_CR_e=0xD,
};

enum CE_JNT {
    CE_JNT_CE_e=0x0,
};

enum CR_JNT {
    CR_JNT_CENTER_e=0x0,
    CR_JNT_BACKBONE_e=0x1,
    CR_JNT_HEAD_e=0x2,
    CR_JNT_MOUTH_L_e=0x3,
    CR_JNT_MOUTH_R_e=0x4,
    CR_JNT_LEG_L_1_e=0x5,
    CR_JNT_LEG_L_2_e=0x6,
    CR_JNT_LEG_L_3_e=0x7,
    CR_JNT_LEG_R_1_e=0x8,
    CR_JNT_LEG_R_2_e=0x9,
    CR_JNT_LEG_R_3_e=0xA,
};

#endif /* !RES_E_CR_H */