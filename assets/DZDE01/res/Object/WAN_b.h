#ifndef RES_WAN_B_H
#define RES_WAN_B_H

enum dRes_INDEX_WAN_B {
    /* BMDR */
    dRes_INDEX_WAN_B_BMD_WAN_B_e=0x4,
    /* BTP  */
    dRes_INDEX_WAN_B_BTP_WAN_B_e=0x7,
};

enum dRes_ID_WAN_B {
    /* BMDR */
    dRes_ID_WAN_B_BMD_WAN_B_e=0x4,
    /* BTP  */
    dRes_ID_WAN_B_BTP_WAN_B_e=0x7,
};

enum WAN_B_JNT {
    WAN_B_JNT_CENTER_e=0x0,
    WAN_B_JNT_BACKBONE_e=0x1,
    WAN_B_JNT_NECK_e=0x2,
    WAN_B_JNT_HEAD_e=0x3,
    WAN_B_JNT_SHOULDERL_e=0x4,
    WAN_B_JNT_ARML1_e=0x5,
    WAN_B_JNT_ARML2_e=0x6,
    WAN_B_JNT_HANDL_e=0x7,
    WAN_B_JNT_SHOULDERR_e=0x8,
    WAN_B_JNT_ARMR1_e=0x9,
    WAN_B_JNT_ARMR2_e=0xA,
    WAN_B_JNT_HANDR_e=0xB,
    WAN_B_JNT_WAIST_e=0xC,
    WAN_B_JNT_LEGL1_e=0xD,
    WAN_B_JNT_LEGL2_e=0xE,
    WAN_B_JNT_FOOTL_e=0xF,
    WAN_B_JNT_SKIRTL_e=0x10,
    WAN_B_JNT_LEGR1_e=0x11,
    WAN_B_JNT_LEGR2_e=0x12,
    WAN_B_JNT_FOOTR_e=0x13,
    WAN_B_JNT_SKIRTR_e=0x14,
};

#endif /* !RES_WAN_B_H */