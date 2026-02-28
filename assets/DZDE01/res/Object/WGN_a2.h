#ifndef RES_WGN_A2_H
#define RES_WGN_A2_H

enum dRes_INDEX_WGN_A2 {
    /* BMDR */
    dRes_INDEX_WGN_A2_BMD_WGN_A2_e=0x4,
    /* BTP  */
    dRes_INDEX_WGN_A2_BTP_WGN_A2_e=0x7,
};

enum dRes_ID_WGN_A2 {
    /* BMDR */
    dRes_ID_WGN_A2_BMD_WGN_A2_e=0x4,
    /* BTP  */
    dRes_ID_WGN_A2_BTP_WGN_A2_e=0x7,
};

enum WGN_A2_JNT {
    WGN_A2_JNT_CENTER_e=0x0,
    WGN_A2_JNT_BACKBONE_e=0x1,
    WGN_A2_JNT_NECK_e=0x2,
    WGN_A2_JNT_HEAD_e=0x3,
    WGN_A2_JNT_SHOULDERL_e=0x4,
    WGN_A2_JNT_ARML1_e=0x5,
    WGN_A2_JNT_ARML2_e=0x6,
    WGN_A2_JNT_HANDL_e=0x7,
    WGN_A2_JNT_SHOULDERR_e=0x8,
    WGN_A2_JNT_ARMR1_e=0x9,
    WGN_A2_JNT_ARMR2_e=0xA,
    WGN_A2_JNT_HANDR_e=0xB,
    WGN_A2_JNT_WAIST_e=0xC,
    WGN_A2_JNT_LEGL1_e=0xD,
    WGN_A2_JNT_LEGL2_e=0xE,
    WGN_A2_JNT_FOOTL_e=0xF,
    WGN_A2_JNT_SKIRTL_e=0x10,
    WGN_A2_JNT_LEGR1_e=0x11,
    WGN_A2_JNT_LEGR2_e=0x12,
    WGN_A2_JNT_FOOTR_e=0x13,
    WGN_A2_JNT_SKIRTR_e=0x14,
};

#endif /* !RES_WGN_A2_H */