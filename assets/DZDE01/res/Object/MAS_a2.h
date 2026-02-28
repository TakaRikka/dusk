#ifndef RES_MAS_A2_H
#define RES_MAS_A2_H

enum dRes_INDEX_MAS_A2 {
    /* BMDR */
    dRes_INDEX_MAS_A2_BMD_MAS_A2_e=0x4,
    /* BTP  */
    dRes_INDEX_MAS_A2_BTP_MAS_A2_e=0x7,
};

enum dRes_ID_MAS_A2 {
    /* BMDR */
    dRes_ID_MAS_A2_BMD_MAS_A2_e=0x4,
    /* BTP  */
    dRes_ID_MAS_A2_BTP_MAS_A2_e=0x7,
};

enum MAS_A2_JNT {
    MAS_A2_JNT_CENTER_e=0x0,
    MAS_A2_JNT_BACKBONE_e=0x1,
    MAS_A2_JNT_NECK_e=0x2,
    MAS_A2_JNT_HEAD_e=0x3,
    MAS_A2_JNT_SHOULDERL_e=0x4,
    MAS_A2_JNT_ARML1_e=0x5,
    MAS_A2_JNT_ARML2_e=0x6,
    MAS_A2_JNT_HANDL_e=0x7,
    MAS_A2_JNT_SHOULDERR_e=0x8,
    MAS_A2_JNT_ARMR1_e=0x9,
    MAS_A2_JNT_ARMR2_e=0xA,
    MAS_A2_JNT_HANDR_e=0xB,
    MAS_A2_JNT_WAIST_e=0xC,
    MAS_A2_JNT_LEGL1_e=0xD,
    MAS_A2_JNT_LEGL2_e=0xE,
    MAS_A2_JNT_FOOTL_e=0xF,
    MAS_A2_JNT_LEGR1_e=0x10,
    MAS_A2_JNT_LEGR2_e=0x11,
    MAS_A2_JNT_FOOTR_e=0x12,
};

#endif /* !RES_MAS_A2_H */