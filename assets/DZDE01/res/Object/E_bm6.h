#ifndef RES_E_BM6_H
#define RES_E_BM6_H

enum dRes_INDEX_E_BM6 {
    /* BCK  */
    dRes_INDEX_E_BM6_BCK_BM6_DOWN_e=0x8,
    dRes_INDEX_E_BM6_BCK_BM6_UP_e=0x9,
    dRes_INDEX_E_BM6_BCK_EF_BIMOL6_e=0xA,
    /* BMDE */
    dRes_INDEX_E_BM6_BMD_BM6_e=0xD,
    /* BMDR */
    dRes_INDEX_E_BM6_BMD_EF_BIMOL6_e=0x10,
    /* BRK  */
    dRes_INDEX_E_BM6_BRK_BM6_LOOK_e=0x13,
    dRes_INDEX_E_BM6_BRK_BM6_SERCH_e=0x14,
    dRes_INDEX_E_BM6_BRK_BM6_TURN_e=0x15,
    dRes_INDEX_E_BM6_BRK_BM6_UP_e=0x16,
    /* BTK  */
    dRes_INDEX_E_BM6_BTK_EF_BIMOL6_e=0x19,
    dRes_INDEX_E_BM6_BTK_EF_BIMOL6_OFF_e=0x1A,
    dRes_INDEX_E_BM6_BTK_EF_BIMOL6_ON_e=0x1B,
    /* DZB  */
    dRes_INDEX_E_BM6_DZB_BM6_e=0x1E,
};

enum dRes_ID_E_BM6 {
    /* BCK  */
    dRes_ID_E_BM6_BCK_BM6_DOWN_e=0x8,
    dRes_ID_E_BM6_BCK_BM6_UP_e=0x9,
    dRes_ID_E_BM6_BCK_EF_BIMOL6_e=0xA,
    /* BMDE */
    dRes_ID_E_BM6_BMD_BM6_e=0xD,
    /* BMDR */
    dRes_ID_E_BM6_BMD_EF_BIMOL6_e=0x10,
    /* BRK  */
    dRes_ID_E_BM6_BRK_BM6_LOOK_e=0x13,
    dRes_ID_E_BM6_BRK_BM6_SERCH_e=0x14,
    dRes_ID_E_BM6_BRK_BM6_TURN_e=0x15,
    dRes_ID_E_BM6_BRK_BM6_UP_e=0x16,
    /* BTK  */
    dRes_ID_E_BM6_BTK_EF_BIMOL6_e=0x19,
    dRes_ID_E_BM6_BTK_EF_BIMOL6_OFF_e=0x1A,
    dRes_ID_E_BM6_BTK_EF_BIMOL6_ON_e=0x1B,
    /* DZB  */
    dRes_ID_E_BM6_DZB_BM6_e=0x1E,
};

enum BM6_JNT {
    BM6_JNT_CENTER_e=0x0,
    BM6_JNT_COGRED_e=0x1,
    BM6_JNT_HEAD_e=0x2,
    BM6_JNT_COGBLUE_e=0x3,
    BM6_JNT_COGGREEN_e=0x4,
    BM6_JNT_COGYELLOW_e=0x5,
};

enum EF_BIMOL6_JNT {
    EF_BIMOL6_JNT_WORLD_ROOT_e=0x0,
    EF_BIMOL6_JNT_BIMOL6BEAM1_e=0x1,
    EF_BIMOL6_JNT_BIMOL6PLANE_e=0x2,
};

#endif /* !RES_E_BM6_H */