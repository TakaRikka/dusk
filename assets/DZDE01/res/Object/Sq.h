#ifndef RES_SQ_H
#define RES_SQ_H

enum dRes_INDEX_SQ {
    /* BCK  */
    dRes_INDEX_SQ_BCK_SQ_DEMO_e=0x5,
    dRes_INDEX_SQ_BCK_SQ_DROP_e=0x6,
    dRes_INDEX_SQ_BCK_SQ_RUN_e=0x7,
    dRes_INDEX_SQ_BCK_SQ_RUN_CLIMB_e=0x8,
    dRes_INDEX_SQ_BCK_SQ_WAIT1_e=0x9,
    dRes_INDEX_SQ_BCK_SQ_WAIT2_01_e=0xA,
    dRes_INDEX_SQ_BCK_SQ_WAIT2_02_e=0xB,
    dRes_INDEX_SQ_BCK_SQ_WAIT2_03_e=0xC,
    dRes_INDEX_SQ_BCK_SQ_WAIT3_e=0xD,
    dRes_INDEX_SQ_BCK_SQ_WAIT4_e=0xE,
    /* BMDR */
    dRes_INDEX_SQ_BMD_SQ_e=0x11,
    /* BTK  */
    dRes_INDEX_SQ_BTK_SQ_e=0x14,
};

enum dRes_ID_SQ {
    /* BCK  */
    dRes_ID_SQ_BCK_SQ_DEMO_e=0x5,
    dRes_ID_SQ_BCK_SQ_DROP_e=0x6,
    dRes_ID_SQ_BCK_SQ_RUN_e=0x7,
    dRes_ID_SQ_BCK_SQ_RUN_CLIMB_e=0x8,
    dRes_ID_SQ_BCK_SQ_WAIT1_e=0x9,
    dRes_ID_SQ_BCK_SQ_WAIT2_01_e=0xA,
    dRes_ID_SQ_BCK_SQ_WAIT2_02_e=0xB,
    dRes_ID_SQ_BCK_SQ_WAIT2_03_e=0xC,
    dRes_ID_SQ_BCK_SQ_WAIT3_e=0xD,
    dRes_ID_SQ_BCK_SQ_WAIT4_e=0xE,
    /* BMDR */
    dRes_ID_SQ_BMD_SQ_e=0x11,
    /* BTK  */
    dRes_ID_SQ_BTK_SQ_e=0x14,
};

enum SQ_JNT {
    SQ_JNT_CENTER_e=0x0,
    SQ_JNT_BACKBONE_e=0x1,
    SQ_JNT_HAND_L_e=0x2,
    SQ_JNT_HAND_R_e=0x3,
    SQ_JNT_NECK_e=0x4,
    SQ_JNT_LEG_L1_e=0x5,
    SQ_JNT_LEG_L2_e=0x6,
    SQ_JNT_LEG_R1_e=0x7,
    SQ_JNT_LEG_R2_e=0x8,
    SQ_JNT_TAIL_e=0x9,
    SQ_JNT_TAIL_2_e=0xA,
};

#endif /* !RES_SQ_H */