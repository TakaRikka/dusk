#ifndef RES_MCN_A2_H
#define RES_MCN_A2_H

enum dRes_INDEX_MCN_A2 {
    /* BMDR */
    dRes_INDEX_MCN_A2_BMD_MCN_A2_e=0x4,
    /* BTP  */
    dRes_INDEX_MCN_A2_BTP_MCN_A2_e=0x7,
};

enum dRes_ID_MCN_A2 {
    /* BMDR */
    dRes_ID_MCN_A2_BMD_MCN_A2_e=0x4,
    /* BTP  */
    dRes_ID_MCN_A2_BTP_MCN_A2_e=0x7,
};

enum MCN_A2_JNT {
    MCN_A2_JNT_CENTER_e=0x0,
    MCN_A2_JNT_BACKBONE_e=0x1,
    MCN_A2_JNT_NECK_e=0x2,
    MCN_A2_JNT_HEAD_e=0x3,
    MCN_A2_JNT_SHOULDERL_e=0x4,
    MCN_A2_JNT_ARML1_e=0x5,
    MCN_A2_JNT_ARML2_e=0x6,
    MCN_A2_JNT_HANDL_e=0x7,
    MCN_A2_JNT_SHOULDERR_e=0x8,
    MCN_A2_JNT_ARMR1_e=0x9,
    MCN_A2_JNT_ARMR2_e=0xA,
    MCN_A2_JNT_HANDR_e=0xB,
    MCN_A2_JNT_WAIST_e=0xC,
    MCN_A2_JNT_LEGL1_e=0xD,
    MCN_A2_JNT_LEGL2_e=0xE,
    MCN_A2_JNT_FOOTL_e=0xF,
    MCN_A2_JNT_LEGR1_e=0x10,
    MCN_A2_JNT_LEGR2_e=0x11,
    MCN_A2_JNT_FOOTR_e=0x12,
};

#endif /* !RES_MCN_A2_H */