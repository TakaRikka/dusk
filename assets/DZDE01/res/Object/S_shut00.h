#ifndef RES_S_SHUT00_H
#define RES_S_SHUT00_H

enum dRes_INDEX_S_SHUT00 {
    /* BMDR */
    dRes_INDEX_S_SHUT00_BMD_S_SHUT_ROU_e=0x5,
    /* DZB  */
    dRes_INDEX_S_SHUT00_DZB_S_SHUT_ROU_e=0x8,
    /* EVT  */
    dRes_INDEX_S_SHUT00_DAT_EVENT_LIST_e=0xB,
};

enum dRes_ID_S_SHUT00 {
    /* BMDR */
    dRes_ID_S_SHUT00_BMD_S_SHUT_ROU_e=0x5,
    /* DZB  */
    dRes_ID_S_SHUT00_DZB_S_SHUT_ROU_e=0x8,
    /* EVT  */
    dRes_ID_S_SHUT00_DAT_EVENT_LIST_e=0xB,
};

enum S_SHUT_ROU_JNT {
    S_SHUT_ROU_JNT_WORLD_ROOT_e=0x0,
};

#endif /* !RES_S_SHUT00_H */