#ifndef RES_MAD_A2_H
#define RES_MAD_A2_H

enum dRes_INDEX_MAD_A2 {
    /* BMDR */
    dRes_INDEX_MAD_A2_BMD_MAD_A2_e=0x4,
    /* BTP  */
    dRes_INDEX_MAD_A2_BTP_MAD_A2_e=0x7,
};

enum dRes_ID_MAD_A2 {
    /* BMDR */
    dRes_ID_MAD_A2_BMD_MAD_A2_e=0x4,
    /* BTP  */
    dRes_ID_MAD_A2_BTP_MAD_A2_e=0x7,
};

enum MAD_A2_JNT {
    MAD_A2_JNT_CENTER_e=0x0,
    MAD_A2_JNT_BACKBONE_e=0x1,
    MAD_A2_JNT_NECK_e=0x2,
    MAD_A2_JNT_HEAD_e=0x3,
    MAD_A2_JNT_SHOULDERL_e=0x4,
    MAD_A2_JNT_ARML1_e=0x5,
    MAD_A2_JNT_ARML2_e=0x6,
    MAD_A2_JNT_HANDL_e=0x7,
    MAD_A2_JNT_SHOULDERR_e=0x8,
    MAD_A2_JNT_ARMR1_e=0x9,
    MAD_A2_JNT_ARMR2_e=0xA,
    MAD_A2_JNT_HANDR_e=0xB,
    MAD_A2_JNT_WAIST_e=0xC,
    MAD_A2_JNT_LEGL1_e=0xD,
    MAD_A2_JNT_LEGL2_e=0xE,
    MAD_A2_JNT_FOOTL_e=0xF,
    MAD_A2_JNT_LEGR1_e=0x10,
    MAD_A2_JNT_LEGR2_e=0x11,
    MAD_A2_JNT_FOOTR_e=0x12,
};

#endif /* !RES_MAD_A2_H */