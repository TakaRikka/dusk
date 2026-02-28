#ifndef RES_OBJ_BKL_H
#define RES_OBJ_BKL_H

enum dRes_INDEX_OBJ_BKL {
    /* BCK  */
    dRes_INDEX_OBJ_BKL_BCK_HL_SHAKE1_e=0x4,
    /* BMDR */
    dRes_INDEX_OBJ_BKL_BMD_HL_e=0x7,
    dRes_INDEX_OBJ_BKL_BMD_K_HB00_e=0x8,
    dRes_INDEX_OBJ_BKL_BMD_K_HB01_e=0x9,
};

enum dRes_ID_OBJ_BKL {
    /* BCK  */
    dRes_ID_OBJ_BKL_BCK_HL_SHAKE1_e=0x4,
    /* BMDR */
    dRes_ID_OBJ_BKL_BMD_HL_e=0x7,
    dRes_ID_OBJ_BKL_BMD_K_HB00_e=0x8,
    dRes_ID_OBJ_BKL_BMD_K_HB01_e=0x9,
};

enum HL_JNT {
    HL_JNT_CENTER_e=0x0,
    HL_JNT_LEAF_1_e=0x1,
    HL_JNT_LEAF_2_e=0x2,
};

enum K_HB00_JNT {
    K_HB00_JNT_WORLD_ROOT_e=0x0,
};

enum K_HB01_JNT {
    K_HB01_JNT_MPM_TUBO_e=0x0,
};

#endif /* !RES_OBJ_BKL_H */