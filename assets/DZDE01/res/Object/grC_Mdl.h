#ifndef RES_GRC_MDL_H
#define RES_GRC_MDL_H

enum dRes_INDEX_GRC_MDL {
    /* BMDR */
    dRes_INDEX_GRC_MDL_BMD_GRC_A_e=0x3,
};

enum dRes_ID_GRC_MDL {
    /* BMDR */
    dRes_ID_GRC_MDL_BMD_GRC_A_e=0x3,
};

enum GRC_A_JNT {
    GRC_A_JNT_CENTER_e=0x0,
    GRC_A_JNT_BACKBONE1_e=0x1,
    GRC_A_JNT_BACKBONE2_e=0x2,
    GRC_A_JNT_NECK_e=0x3,
    GRC_A_JNT_HEAD_e=0x4,
    GRC_A_JNT_CHIN_e=0x5,
    GRC_A_JNT_MAYU_e=0x6,
    GRC_A_JNT_MOUTH_e=0x7,
    GRC_A_JNT_SHOULDERL_e=0x8,
    GRC_A_JNT_ARML1_e=0x9,
    GRC_A_JNT_ARML2_e=0xA,
    GRC_A_JNT_HANDL_e=0xB,
    GRC_A_JNT_SHOULDERR_e=0xC,
    GRC_A_JNT_ARMR1_e=0xD,
    GRC_A_JNT_ARMR2_e=0xE,
    GRC_A_JNT_HANDR_e=0xF,
    GRC_A_JNT_WAIST_e=0x10,
    GRC_A_JNT_LEGL1_e=0x11,
    GRC_A_JNT_LEGL2_e=0x12,
    GRC_A_JNT_FOOTL_e=0x13,
    GRC_A_JNT_LEGR1_e=0x14,
    GRC_A_JNT_LEGR2_e=0x15,
    GRC_A_JNT_FOOTR_e=0x16,
};

#endif /* !RES_GRC_MDL_H */