#ifndef RES_TKC_H
#define RES_TKC_H

enum dRes_INDEX_TKC {
    /* BCK  */
    dRes_INDEX_TKC_BCK_TKC_F_TALK_A_e=0x6,
    dRes_INDEX_TKC_BCK_TKC_FLY_e=0x7,
    /* BMDR */
    dRes_INDEX_TKC_BMD_TKC_e=0xA,
    /* BTK  */
    dRes_INDEX_TKC_BTK_TKC_e=0xD,
    /* BTP  */
    dRes_INDEX_TKC_BTP_TKC_e=0x10,
};

enum dRes_ID_TKC {
    /* BCK  */
    dRes_ID_TKC_BCK_TKC_F_TALK_A_e=0x6,
    dRes_ID_TKC_BCK_TKC_FLY_e=0x7,
    /* BMDR */
    dRes_ID_TKC_BMD_TKC_e=0xA,
    /* BTK  */
    dRes_ID_TKC_BTK_TKC_e=0xD,
    /* BTP  */
    dRes_ID_TKC_BTP_TKC_e=0x10,
};

enum TKC_JNT {
    TKC_JNT_CENTER_e=0x0,
    TKC_JNT_HEAD_e=0x1,
    TKC_JNT_ARML_e=0x2,
    TKC_JNT_ARMR_e=0x3,
    TKC_JNT_CHIN_e=0x4,
    TKC_JNT_MOUTH_e=0x5,
};

#endif /* !RES_TKC_H */