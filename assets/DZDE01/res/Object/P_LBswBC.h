#ifndef RES_P_LBSWBC_H
#define RES_P_LBSWBC_H

enum dRes_INDEX_P_LBSWBC {
    /* BMDR */
    dRes_INDEX_P_LBSWBC_BMD_P_LBSWB_e=0x5,
    dRes_INDEX_P_LBSWBC_BMD_P_LBSWC_e=0x6,
    /* BTK  */
    dRes_INDEX_P_LBSWBC_BTK_P_LBSWB_e=0x9,
    dRes_INDEX_P_LBSWBC_BTK_P_LBSWC_e=0xA,
    /* EVT  */
    dRes_INDEX_P_LBSWBC_DAT_EVENT_LIST_e=0xD,
};

enum dRes_ID_P_LBSWBC {
    /* BMDR */
    dRes_ID_P_LBSWBC_BMD_P_LBSWB_e=0x5,
    dRes_ID_P_LBSWBC_BMD_P_LBSWC_e=0x6,
    /* BTK  */
    dRes_ID_P_LBSWBC_BTK_P_LBSWB_e=0x9,
    dRes_ID_P_LBSWBC_BTK_P_LBSWC_e=0xA,
    /* EVT  */
    dRes_ID_P_LBSWBC_DAT_EVENT_LIST_e=0xD,
};

enum P_LBSWB_JNT {
    P_LBSWB_JNT_WORLD_ROOT_e=0x0,
};

enum P_LBSWC_JNT {
    P_LBSWC_JNT_WORLD_ROOT_e=0x0,
};

#endif /* !RES_P_LBSWBC_H */