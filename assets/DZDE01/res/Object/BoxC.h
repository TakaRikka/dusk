#ifndef RES_BOXC_H
#define RES_BOXC_H

enum dRes_INDEX_BOXC {
    /* BCK  */
    dRes_INDEX_BOXC_BCK_K_TAKARAC_e=0x8,
    /* BMDR */
    dRes_INDEX_BOXC_BMD_EF_BOXC_e=0xB,
    /* BMDV */
    dRes_INDEX_BOXC_BMD_BOXC_e=0xE,
    /* BRK  */
    dRes_INDEX_BOXC_BRK_EF_BOXC_e=0x11,
    /* DZB  */
    dRes_INDEX_BOXC_DZB_BOXCC_e=0x14,
    dRes_INDEX_BOXC_DZB_BOXCO_e=0x15,
    /* EVT  */
    dRes_INDEX_BOXC_DAT_EVENT_LIST_e=0x18,
};

enum dRes_ID_BOXC {
    /* BCK  */
    dRes_ID_BOXC_BCK_K_TAKARAC_e=0x8,
    /* BMDR */
    dRes_ID_BOXC_BMD_EF_BOXC_e=0xB,
    /* BMDV */
    dRes_ID_BOXC_BMD_BOXC_e=0xE,
    /* BRK  */
    dRes_ID_BOXC_BRK_EF_BOXC_e=0x11,
    /* DZB  */
    dRes_ID_BOXC_DZB_BOXCC_e=0x14,
    dRes_ID_BOXC_DZB_BOXCO_e=0x15,
    /* EVT  */
    dRes_ID_BOXC_DAT_EVENT_LIST_e=0x18,
};

enum EF_BOXC_JNT {
    EF_BOXC_JNT_EF_BOXB_e=0x0,
};

enum BOXC_JNT {
    BOXC_JNT_WORLD_ROOT_e=0x0,
    BOXC_JNT_BOXC_e=0x1,
    BOXC_JNT_COVER_JNT_e=0x2,
};

#endif /* !RES_BOXC_H */