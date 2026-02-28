#ifndef RES_MG_F_KS_H
#define RES_MG_F_KS_H

enum dRes_INDEX_MG_F_KS {
    /* BMDR */
    dRes_INDEX_MG_F_KS_BMD_KS_e=0x4,
    /* BRK  */
    dRes_INDEX_MG_F_KS_BRK_KS_e=0x7,
};

enum dRes_ID_MG_F_KS {
    /* BMDR */
    dRes_ID_MG_F_KS_BMD_KS_e=0x4,
    /* BRK  */
    dRes_ID_MG_F_KS_BRK_KS_e=0x7,
};

enum KS_JNT {
    KS_JNT_WORLD_ROOT_e=0x0,
    KS_JNT_BACKBONE01_e=0x1,
    KS_JNT_BACKBONE02_e=0x2,
    KS_JNT_BACKBONE03_e=0x3,
    KS_JNT_BACKBONE04_e=0x4,
    KS_JNT_TAIL01_e=0x5,
    KS_JNT_TAIL02_e=0x6,
    KS_JNT_M_HIREL_e=0x7,
    KS_JNT_M_HIRER_e=0x8,
    KS_JNT_HEAD_e=0x9,
    KS_JNT_AGO_e=0xA,
};

#endif /* !RES_MG_F_KS_H */