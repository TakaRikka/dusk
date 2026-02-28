#ifndef RES_ASH_H
#define RES_ASH_H

enum dRes_INDEX_ASH {
    /* BCK  */
    dRes_INDEX_ASH_BCK_ASH_F_TALK_A_e=0x7,
    dRes_INDEX_ASH_BCK_ASH_WAIT_A_e=0x8,
    /* BMDR */
    dRes_INDEX_ASH_BMD_ASH_e=0xB,
    /* BTK  */
    dRes_INDEX_ASH_BTK_ASH_e=0xE,
    /* BTP  */
    dRes_INDEX_ASH_BTP_ASH_e=0x11,
    /* EVT  */
    dRes_INDEX_ASH_DAT_EVENT_LIST_e=0x14,
};

enum dRes_ID_ASH {
    /* BCK  */
    dRes_ID_ASH_BCK_ASH_F_TALK_A_e=0x7,
    dRes_ID_ASH_BCK_ASH_WAIT_A_e=0x8,
    /* BMDR */
    dRes_ID_ASH_BMD_ASH_e=0xB,
    /* BTK  */
    dRes_ID_ASH_BTK_ASH_e=0xE,
    /* BTP  */
    dRes_ID_ASH_BTP_ASH_e=0x11,
    /* EVT  */
    dRes_ID_ASH_DAT_EVENT_LIST_e=0x14,
};

enum ASH_JNT {
    ASH_JNT_CENTER_e=0x0,
    ASH_JNT_BB1_e=0x1,
    ASH_JNT_BB2_e=0x2,
    ASH_JNT_NECK_e=0x3,
    ASH_JNT_HEAD_e=0x4,
    ASH_JNT_CHIN_e=0x5,
    ASH_JNT_HAIRL1_e=0x6,
    ASH_JNT_HAIRL2_e=0x7,
    ASH_JNT_HAIRR1_e=0x8,
    ASH_JNT_HAIRR2_e=0x9,
    ASH_JNT_MAYUL_e=0xA,
    ASH_JNT_MAYUR_e=0xB,
    ASH_JNT_MOUTH_e=0xC,
    ASH_JNT_SHOULDERL_e=0xD,
    ASH_JNT_ARML1_e=0xE,
    ASH_JNT_ARML2_e=0xF,
    ASH_JNT_HANDL_e=0x10,
    ASH_JNT_FINGERL_e=0x11,
    ASH_JNT_THUMBL_e=0x12,
    ASH_JNT_ARMOR_e=0x13,
    ASH_JNT_SHOULDERR_e=0x14,
    ASH_JNT_ARMR1_e=0x15,
    ASH_JNT_ARMR2_e=0x16,
    ASH_JNT_HANDR_e=0x17,
    ASH_JNT_FINGERR_e=0x18,
    ASH_JNT_THUMBR_e=0x19,
    ASH_JNT_WAIST_e=0x1A,
    ASH_JNT_LEGL1_e=0x1B,
    ASH_JNT_LEGL2_e=0x1C,
    ASH_JNT_FOOTL_e=0x1D,
    ASH_JNT_LEGR1_e=0x1E,
    ASH_JNT_LEGR2_e=0x1F,
    ASH_JNT_FOOTR_e=0x20,
    ASH_JNT_SWORD_e=0x21,
};

#endif /* !RES_ASH_H */