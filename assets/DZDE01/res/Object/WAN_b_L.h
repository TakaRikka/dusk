#ifndef RES_WAN_B_L_H
#define RES_WAN_B_L_H

enum dRes_INDEX_WAN_B_L {
    /* BMDR */
    dRes_INDEX_WAN_B_L_BMD_WAN_B_L_e=0x3,
};

enum dRes_ID_WAN_B_L {
    /* BMDR */
    dRes_ID_WAN_B_L_BMD_WAN_B_L_e=0x3,
};

enum WAN_B_L_JNT {
    WAN_B_L_JNT_WORLD_ROOT_e=0x0,
    WAN_B_L_JNT_BACKBONE_e=0x1,
    WAN_B_L_JNT_ARML1_e=0x2,
    WAN_B_L_JNT_ARML2_e=0x3,
    WAN_B_L_JNT_ARMR1_e=0x4,
    WAN_B_L_JNT_ARMR2_e=0x5,
    WAN_B_L_JNT_HEAD_e=0x6,
    WAN_B_L_JNT_WAIST_e=0x7,
    WAN_B_L_JNT_LEGL1_e=0x8,
    WAN_B_L_JNT_LEGL2_e=0x9,
    WAN_B_L_JNT_LEGR1_e=0xA,
    WAN_B_L_JNT_LEGR2_e=0xB,
};

#endif /* !RES_WAN_B_L_H */