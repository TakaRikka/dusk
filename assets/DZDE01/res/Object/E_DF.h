#ifndef RES_E_DF_H
#define RES_E_DF_H

enum dRes_INDEX_E_DF {
    /* BCK  */
    dRes_INDEX_E_DF_BCK_DF_BOUND_e=0x4,
    dRes_INDEX_E_DF_BCK_DF_BURST_e=0x5,
    dRes_INDEX_E_DF_BCK_DF_DIE_e=0x6,
    dRes_INDEX_E_DF_BCK_DF_EAT_e=0x7,
    dRes_INDEX_E_DF_BCK_DF_EAT_WAIT_e=0x8,
    dRes_INDEX_E_DF_BCK_DF_SPITOUT_e=0x9,
    dRes_INDEX_E_DF_BCK_DF_WAIT_e=0xA,
    /* BMDR */
    dRes_INDEX_E_DF_BMD_DF_e=0xD,
};

enum dRes_ID_E_DF {
    /* BCK  */
    dRes_ID_E_DF_BCK_DF_BOUND_e=0x4,
    dRes_ID_E_DF_BCK_DF_BURST_e=0x5,
    dRes_ID_E_DF_BCK_DF_DIE_e=0x6,
    dRes_ID_E_DF_BCK_DF_EAT_e=0x7,
    dRes_ID_E_DF_BCK_DF_EAT_WAIT_e=0x8,
    dRes_ID_E_DF_BCK_DF_SPITOUT_e=0x9,
    dRes_ID_E_DF_BCK_DF_WAIT_e=0xA,
    /* BMDR */
    dRes_ID_E_DF_BMD_DF_e=0xD,
};

enum DF_JNT {
    DF_JNT_CENTER_e=0x0,
    DF_JNT_FLOWER_1_e=0x1,
    DF_JNT_FLOWER_2_e=0x2,
    DF_JNT_FLOWER_3_e=0x3,
    DF_JNT_FLOWER_4_e=0x4,
    DF_JNT_FLOWER_5_e=0x5,
    DF_JNT_STALK_L_1_e=0x6,
    DF_JNT_STALK_L_2_e=0x7,
    DF_JNT_STALK_R_1_e=0x8,
    DF_JNT_STALK_R_2_e=0x9,
};

#endif /* !RES_E_DF_H */