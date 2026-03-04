#ifndef RES_E_SG_H
#define RES_E_SG_H

enum dRes_INDEX_E_SG {
    /* BMDR */
    dRes_INDEX_E_SG_BMD_SG_e=0x3,
};

enum dRes_ID_E_SG {
    /* BMDR */
    dRes_ID_E_SG_BMD_SG_e=0x3,
};

enum SG_JNT {
    SG_JNT_CENTER_e=0x0,
    SG_JNT_BACKBONE1_e=0x1,
    SG_JNT_BACKBONE2_e=0x2,
    SG_JNT_BACKBONE3_e=0x3,
    SG_JNT_HEAD1_e=0x4,
    SG_JNT_ANDER_HEAD1_e=0x5,
};

#endif /* !RES_E_SG_H */