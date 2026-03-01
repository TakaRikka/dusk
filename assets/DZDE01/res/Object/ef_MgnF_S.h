#ifndef RES_EF_MGNF_S_H
#define RES_EF_MGNF_S_H

enum dRes_INDEX_EF_MGNF_S {
    /* BMDE */
    dRes_INDEX_EF_MGNF_S_BMD_EF_MGNFIELD_S_e=0x5,
    /* BRK  */
    dRes_INDEX_EF_MGNF_S_BRK_EF_MGNFIELD_S_e=0x8,
    /* BTK  */
    dRes_INDEX_EF_MGNF_S_BTK_EF_MGNFIELD_S_e=0xB,
    dRes_INDEX_EF_MGNF_S_BTK_EF_MGNFIELD_S_02_e=0xC,
};

enum dRes_ID_EF_MGNF_S {
    /* BMDE */
    dRes_ID_EF_MGNF_S_BMD_EF_MGNFIELD_S_e=0x5,
    /* BRK  */
    dRes_ID_EF_MGNF_S_BRK_EF_MGNFIELD_S_e=0x8,
    /* BTK  */
    dRes_ID_EF_MGNF_S_BTK_EF_MGNFIELD_S_e=0xB,
    dRes_ID_EF_MGNF_S_BTK_EF_MGNFIELD_S_02_e=0xC,
};

enum EF_MGNFIELD_S_JNT {
    EF_MGNFIELD_S_JNT_WORLD_ROOT_e=0x0,
};

#endif /* !RES_EF_MGNF_S_H */