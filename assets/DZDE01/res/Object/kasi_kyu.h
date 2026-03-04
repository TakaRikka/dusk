#ifndef RES_KASI_KYU_H
#define RES_KASI_KYU_H

enum dRes_INDEX_KASI_KYU {
    /* BMDR */
    dRes_INDEX_KASI_KYU_BMD_KYU_e=0x4,
    /* BTP  */
    dRes_INDEX_KASI_KYU_BTP_KYU_e=0x7,
};

enum dRes_ID_KASI_KYU {
    /* BMDR */
    dRes_ID_KASI_KYU_BMD_KYU_e=0x4,
    /* BTP  */
    dRes_ID_KASI_KYU_BTP_KYU_e=0x7,
};

enum KYU_JNT {
    KYU_JNT_CENTER_e=0x0,
    KYU_JNT_BACKBONE_e=0x1,
    KYU_JNT_NECK_e=0x2,
    KYU_JNT_HEAD_e=0x3,
    KYU_JNT_SHOULDERL_e=0x4,
    KYU_JNT_ARML1_e=0x5,
    KYU_JNT_ARML2_e=0x6,
    KYU_JNT_HANDL_e=0x7,
    KYU_JNT_SHOULDERR_e=0x8,
    KYU_JNT_ARMR1_e=0x9,
    KYU_JNT_ARMR2_e=0xA,
    KYU_JNT_HANDR_e=0xB,
    KYU_JNT_WAIST_e=0xC,
    KYU_JNT_LEGL1_e=0xD,
    KYU_JNT_LEGL2_e=0xE,
    KYU_JNT_FOOTL_e=0xF,
    KYU_JNT_SKIRTL_e=0x10,
    KYU_JNT_LEGR1_e=0x11,
    KYU_JNT_LEGR2_e=0x12,
    KYU_JNT_FOOTR_e=0x13,
    KYU_JNT_SKIRTR_e=0x14,
};

#endif /* !RES_KASI_KYU_H */