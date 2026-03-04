#ifndef RES_SHOE_H
#define RES_SHOE_H

enum dRes_INDEX_SHOE {
    /* BCK  */
    dRes_INDEX_SHOE_BCK_SHOE_TALK_A_e=0x5,
    /* BMDR */
    dRes_INDEX_SHOE_BMD_SHOE_e=0x8,
    dRes_INDEX_SHOE_BMD_SHOE_CLOTH_e=0x9,
    /* BTP  */
    dRes_INDEX_SHOE_BTP_SHOE_e=0xC,
};

enum dRes_ID_SHOE {
    /* BCK  */
    dRes_ID_SHOE_BCK_SHOE_TALK_A_e=0x5,
    /* BMDR */
    dRes_ID_SHOE_BMD_SHOE_e=0x8,
    dRes_ID_SHOE_BMD_SHOE_CLOTH_e=0x9,
    /* BTP  */
    dRes_ID_SHOE_BTP_SHOE_e=0xC,
};

enum SHOE_JNT {
    SHOE_JNT_CENTER_e=0x0,
    SHOE_JNT_BACKBONE_e=0x1,
    SHOE_JNT_NECK_e=0x2,
    SHOE_JNT_HEAD_e=0x3,
    SHOE_JNT_SHOULDERL_e=0x4,
    SHOE_JNT_ARML1_e=0x5,
    SHOE_JNT_ARML2_e=0x6,
    SHOE_JNT_HANDL_e=0x7,
    SHOE_JNT_SHOULDERR_e=0x8,
    SHOE_JNT_ARMR1_e=0x9,
    SHOE_JNT_ARMR2_e=0xA,
    SHOE_JNT_HANDR_e=0xB,
    SHOE_JNT_WAIST_e=0xC,
    SHOE_JNT_LEGL1_e=0xD,
    SHOE_JNT_LEGL2_e=0xE,
    SHOE_JNT_FOOTL_e=0xF,
    SHOE_JNT_LEGR1_e=0x10,
    SHOE_JNT_LEGR2_e=0x11,
    SHOE_JNT_FOOTR_e=0x12,
};

enum SHOE_CLOTH_JNT {
    SHOE_CLOTH_JNT_SHOE_CLOTH_e=0x0,
};

#endif /* !RES_SHOE_H */