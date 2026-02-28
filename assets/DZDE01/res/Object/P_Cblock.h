#ifndef RES_P_CBLOCK_H
#define RES_P_CBLOCK_H

enum dRes_INDEX_P_CBLOCK {
    /* BMDR */
    dRes_INDEX_P_CBLOCK_BMD_P_CBLOCK_e=0x4,
    dRes_INDEX_P_CBLOCK_BMD_P_CHAIN_e=0x5,
    dRes_INDEX_P_CBLOCK_BMD_P_HANDLE_e=0x6,
    /* DZB  */
    dRes_INDEX_P_CBLOCK_DZB_P_CBLOCK_e=0x9,
};

enum dRes_ID_P_CBLOCK {
    /* BMDR */
    dRes_ID_P_CBLOCK_BMD_P_CBLOCK_e=0x4,
    dRes_ID_P_CBLOCK_BMD_P_CHAIN_e=0x5,
    dRes_ID_P_CBLOCK_BMD_P_HANDLE_e=0x6,
    /* DZB  */
    dRes_ID_P_CBLOCK_DZB_P_CBLOCK_e=0x9,
};

enum P_CBLOCK_JNT {
    P_CBLOCK_JNT_WORLD_ROOT_e=0x0,
};

enum P_CHAIN_JNT {
    P_CHAIN_JNT_WORLD_ROOT_e=0x0,
};

enum P_HANDLE_JNT {
    P_HANDLE_JNT_WORLD_ROOT_e=0x0,
};

#endif /* !RES_P_CBLOCK_H */