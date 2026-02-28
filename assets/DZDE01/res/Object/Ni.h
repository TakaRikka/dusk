#ifndef RES_NI_H
#define RES_NI_H

enum dRes_INDEX_NI {
    /* BCK  */
    dRes_INDEX_NI_BCK_NI_FALL_e=0x5,
    dRes_INDEX_NI_BCK_NI_FLY_e=0x6,
    dRes_INDEX_NI_BCK_NI_IKAKU_e=0x7,
    dRes_INDEX_NI_BCK_NI_MIWATASI_e=0x8,
    dRes_INDEX_NI_BCK_NI_MOGAKU_e=0x9,
    dRes_INDEX_NI_BCK_NI_NAKU_e=0xA,
    dRes_INDEX_NI_BCK_NI_WAIT1_e=0xB,
    dRes_INDEX_NI_BCK_NI_WALK_A_e=0xC,
    dRes_INDEX_NI_BCK_NI_WALK_B_e=0xD,
    /* BMDR */
    dRes_INDEX_NI_BMD_NI_e=0x10,
    /* BTK  */
    dRes_INDEX_NI_BTK_NI_e=0x13,
};

enum dRes_ID_NI {
    /* BCK  */
    dRes_ID_NI_BCK_NI_FALL_e=0x5,
    dRes_ID_NI_BCK_NI_FLY_e=0x6,
    dRes_ID_NI_BCK_NI_IKAKU_e=0x7,
    dRes_ID_NI_BCK_NI_MIWATASI_e=0x8,
    dRes_ID_NI_BCK_NI_MOGAKU_e=0x9,
    dRes_ID_NI_BCK_NI_NAKU_e=0xA,
    dRes_ID_NI_BCK_NI_WAIT1_e=0xB,
    dRes_ID_NI_BCK_NI_WALK_A_e=0xC,
    dRes_ID_NI_BCK_NI_WALK_B_e=0xD,
    /* BMDR */
    dRes_ID_NI_BMD_NI_e=0x10,
    /* BTK  */
    dRes_ID_NI_BTK_NI_e=0x13,
};

enum NI_JNT {
    NI_JNT_WAIST_e=0x0,
    NI_JNT_LEGL_e=0x1,
    NI_JNT_FOOTL_e=0x2,
    NI_JNT_LEGR_e=0x3,
    NI_JNT_FOOTR_e=0x4,
    NI_JNT_NECK_e=0x5,
    NI_JNT_HEAD_e=0x6,
    NI_JNT_WINGL_e=0x7,
    NI_JNT_WINGR_e=0x8,
};

#endif /* !RES_NI_H */