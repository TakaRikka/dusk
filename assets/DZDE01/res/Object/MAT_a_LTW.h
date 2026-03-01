#ifndef RES_MAT_A_LTW_H
#define RES_MAT_A_LTW_H

enum dRes_INDEX_MAT_A_LTW {
    /* BMDR */
    dRes_INDEX_MAT_A_LTW_BMD_MAT_A_L_TW_e=0x3,
};

enum dRes_ID_MAT_A_LTW {
    /* BMDR */
    dRes_ID_MAT_A_LTW_BMD_MAT_A_L_TW_e=0x3,
};

enum MAT_A_L_TW_JNT {
    MAT_A_L_TW_JNT_WORLD_ROOT_e=0x0,
    MAT_A_L_TW_JNT_BACKBONE_e=0x1,
    MAT_A_L_TW_JNT_ARML1_e=0x2,
    MAT_A_L_TW_JNT_ARML2_e=0x3,
    MAT_A_L_TW_JNT_ARMR1_e=0x4,
    MAT_A_L_TW_JNT_ARMR2_e=0x5,
    MAT_A_L_TW_JNT_HEAD_e=0x6,
    MAT_A_L_TW_JNT_WAIST_e=0x7,
    MAT_A_L_TW_JNT_LEGL1_e=0x8,
    MAT_A_L_TW_JNT_LEGL2_e=0x9,
    MAT_A_L_TW_JNT_LEGR1_e=0xA,
    MAT_A_L_TW_JNT_LEGR2_e=0xB,
};

#endif /* !RES_MAT_A_LTW_H */