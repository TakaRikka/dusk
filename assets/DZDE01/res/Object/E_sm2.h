#ifndef RES_E_SM2_H
#define RES_E_SM2_H

enum dRes_INDEX_E_SM2 {
    /* BMDE */
    dRes_INDEX_E_SM2_BMD_SM_PIECE_e=0x5,
    /* BMDV */
    dRes_INDEX_E_SM2_BMD_SM2_e=0x8,
    /* BTK  */
    dRes_INDEX_E_SM2_BTK_SM2_SCALE_e=0xB,
};

enum dRes_ID_E_SM2 {
    /* BMDE */
    dRes_ID_E_SM2_BMD_SM_PIECE_e=0x5,
    /* BMDV */
    dRes_ID_E_SM2_BMD_SM2_e=0x8,
    /* BTK  */
    dRes_ID_E_SM2_BTK_SM2_SCALE_e=0xB,
};

enum SM_PIECE_JNT {
    SM_PIECE_JNT_SM2_PIECEMODEL_e=0x0,
};

enum SM2_JNT {
    SM2_JNT_BODY1_e=0x0,
    SM2_JNT_BODY2_e=0x1,
    SM2_JNT_BODY3_e=0x2,
    SM2_JNT_BODY4_e=0x3,
    SM2_JNT_BODY5_e=0x4,
    SM2_JNT_BODY6_e=0x5,
    SM2_JNT_BODY7_e=0x6,
    SM2_JNT_BODY8_e=0x7,
};

#endif /* !RES_E_SM2_H */