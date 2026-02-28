#ifndef RES_RI_H
#define RES_RI_H

enum dRes_INDEX_RI {
    /* BMDR */
    dRes_INDEX_RI_BMD_RI_e=0x3,
};

enum dRes_ID_RI {
    /* BMDR */
    dRes_ID_RI_BMD_RI_e=0x3,
};

enum RI_JNT {
    RI_JNT_HEAD_e=0x0,
    RI_JNT_BACKBONE1_e=0x1,
    RI_JNT_BACKBONE2_e=0x2,
    RI_JNT_BACKBONE3_e=0x3,
    RI_JNT_BACKBONE4_e=0x4,
    RI_JNT_TAIL1_e=0x5,
    RI_JNT_TAIL2_e=0x6,
    RI_JNT_FINL_e=0x7,
    RI_JNT_FINR_e=0x8,
    RI_JNT_JAW_e=0x9,
};

#endif /* !RES_RI_H */