#ifndef RES_WORM_H
#define RES_WORM_H

enum dRes_INDEX_WORM {
    /* BMDR */
    dRes_INDEX_WORM_BMD_WORM_e=0x3,
    dRes_INDEX_WORM_BMD_WORM_HOLE_e=0x4,
};

enum dRes_ID_WORM {
    /* BMDR */
    dRes_ID_WORM_BMD_WORM_e=0x3,
    dRes_ID_WORM_BMD_WORM_HOLE_e=0x4,
};

enum WORM_JNT {
    WORM_JNT_BODY1_e=0x0,
    WORM_JNT_BODY2_e=0x1,
    WORM_JNT_BODY3_e=0x2,
    WORM_JNT_BODY4_e=0x3,
    WORM_JNT_BODY5_e=0x4,
    WORM_JNT_BODY6_e=0x5,
    WORM_JNT_BODY7_e=0x6,
    WORM_JNT_BODY8_e=0x7,
    WORM_JNT_BODY9_e=0x8,
    WORM_JNT_BODY10_e=0x9,
};

enum WORM_HOLE_JNT {
    WORM_HOLE_JNT_WORM_HOLE_MODEL_e=0x0,
};

#endif /* !RES_WORM_H */