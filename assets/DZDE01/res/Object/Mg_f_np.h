#ifndef RES_MG_F_NP_H
#define RES_MG_F_NP_H

enum dRes_INDEX_MG_F_NP {
    /* BMDR */
    dRes_INDEX_MG_F_NP_BMD_NP_e=0x3,
};

enum dRes_ID_MG_F_NP {
    /* BMDR */
    dRes_ID_MG_F_NP_BMD_NP_e=0x3,
};

enum NP_JNT {
    NP_JNT_WORLD_ROOT_e=0x0,
    NP_JNT_BACKBONE01_e=0x1,
    NP_JNT_BACKBONE02_e=0x2,
    NP_JNT_BACKBONE03_e=0x3,
    NP_JNT_BACKBONE04_e=0x4,
    NP_JNT_TAIL01_e=0x5,
    NP_JNT_TAIL02_e=0x6,
    NP_JNT_M_HIREL_e=0x7,
    NP_JNT_M_HIRER_e=0x8,
    NP_JNT_HEAD_e=0x9,
    NP_JNT_AGO_e=0xA,
};

#endif /* !RES_MG_F_NP_H */