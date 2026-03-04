#ifndef RES_WAN_A2_H
#define RES_WAN_A2_H

enum dRes_INDEX_WAN_A2 {
    /* BMDR */
    dRes_INDEX_WAN_A2_BMD_WAN_A2_e=0x4,
    /* BTP  */
    dRes_INDEX_WAN_A2_BTP_WAN_A2_e=0x7,
};

enum dRes_ID_WAN_A2 {
    /* BMDR */
    dRes_ID_WAN_A2_BMD_WAN_A2_e=0x4,
    /* BTP  */
    dRes_ID_WAN_A2_BTP_WAN_A2_e=0x7,
};

enum WAN_A2_JNT {
    WAN_A2_JNT_CENTER_e=0x0,
    WAN_A2_JNT_BACKBONE_e=0x1,
    WAN_A2_JNT_NECK_e=0x2,
    WAN_A2_JNT_HEAD_e=0x3,
    WAN_A2_JNT_SHOULDERL_e=0x4,
    WAN_A2_JNT_ARML1_e=0x5,
    WAN_A2_JNT_ARML2_e=0x6,
    WAN_A2_JNT_HANDL_e=0x7,
    WAN_A2_JNT_SHOULDERR_e=0x8,
    WAN_A2_JNT_ARMR1_e=0x9,
    WAN_A2_JNT_ARMR2_e=0xA,
    WAN_A2_JNT_HANDR_e=0xB,
    WAN_A2_JNT_WAIST_e=0xC,
    WAN_A2_JNT_LEGL1_e=0xD,
    WAN_A2_JNT_LEGL2_e=0xE,
    WAN_A2_JNT_FOOTL_e=0xF,
    WAN_A2_JNT_SKIRTL_e=0x10,
    WAN_A2_JNT_LEGR1_e=0x11,
    WAN_A2_JNT_LEGR2_e=0x12,
    WAN_A2_JNT_FOOTR_e=0x13,
    WAN_A2_JNT_SKIRTR_e=0x14,
};

#endif /* !RES_WAN_A2_H */