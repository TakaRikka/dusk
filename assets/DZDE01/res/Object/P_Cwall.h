#ifndef RES_P_CWALL_H
#define RES_P_CWALL_H

enum dRes_INDEX_P_CWALL {
    /* BMDR */
    dRes_INDEX_P_CWALL_BMD_P_CHAIN_e=0x4,
    dRes_INDEX_P_CWALL_BMD_P_CWALL_e=0x5,
    dRes_INDEX_P_CWALL_BMD_P_HANDLE_e=0x6,
    /* DZB  */
    dRes_INDEX_P_CWALL_DZB_P_CWALL_e=0x9,
};

enum dRes_ID_P_CWALL {
    /* BMDR */
    dRes_ID_P_CWALL_BMD_P_CHAIN_e=0x4,
    dRes_ID_P_CWALL_BMD_P_CWALL_e=0x5,
    dRes_ID_P_CWALL_BMD_P_HANDLE_e=0x6,
    /* DZB  */
    dRes_ID_P_CWALL_DZB_P_CWALL_e=0x9,
};

enum P_CHAIN_JNT {
    P_CHAIN_JNT_WORLD_ROOT_e=0x0,
};

enum P_CWALL_JNT {
    P_CWALL_JNT_WORLD_ROOT_e=0x0,
};

enum P_HANDLE_JNT {
    P_HANDLE_JNT_WORLD_ROOT_e=0x0,
};

#endif /* !RES_P_CWALL_H */