#ifndef RES_LUD_H
#define RES_LUD_H

enum dRes_INDEX_LUD {
    /* BCK  */
    dRes_INDEX_LUD_BCK_LUD_F_TALK_A_e=0x6,
    dRes_INDEX_LUD_BCK_LUD_STEP_e=0x7,
    dRes_INDEX_LUD_BCK_LUD_WAIT_A_e=0x8,
    /* BMDR */
    dRes_INDEX_LUD_BMD_LUD_e=0xB,
    /* BTK  */
    dRes_INDEX_LUD_BTK_LUD_e=0xE,
    /* BTP  */
    dRes_INDEX_LUD_BTP_LUD_e=0x11,
};

enum dRes_ID_LUD {
    /* BCK  */
    dRes_ID_LUD_BCK_LUD_F_TALK_A_e=0x6,
    dRes_ID_LUD_BCK_LUD_STEP_e=0x7,
    dRes_ID_LUD_BCK_LUD_WAIT_A_e=0x8,
    /* BMDR */
    dRes_ID_LUD_BMD_LUD_e=0xB,
    /* BTK  */
    dRes_ID_LUD_BTK_LUD_e=0xE,
    /* BTP  */
    dRes_ID_LUD_BTP_LUD_e=0x11,
};

enum LUD_JNT {
    LUD_JNT_CENTER_e=0x0,
    LUD_JNT_BACKBONE1_e=0x1,
    LUD_JNT_BACKBONE2_e=0x2,
    LUD_JNT_NECK_e=0x3,
    LUD_JNT_HEAD_e=0x4,
    LUD_JNT_CHIN_e=0x5,
    LUD_JNT_HAIRL_e=0x6,
    LUD_JNT_HAIRR_e=0x7,
    LUD_JNT_MAYUL_e=0x8,
    LUD_JNT_MAYUR_e=0x9,
    LUD_JNT_MOUTH_e=0xA,
    LUD_JNT_SHOULDERL_e=0xB,
    LUD_JNT_ARML1_e=0xC,
    LUD_JNT_ARML2_e=0xD,
    LUD_JNT_HANDL_e=0xE,
    LUD_JNT_FINGERL_e=0xF,
    LUD_JNT_SHOULDERR_e=0x10,
    LUD_JNT_ARMR1_e=0x11,
    LUD_JNT_ARMR2_e=0x12,
    LUD_JNT_HANDR_e=0x13,
    LUD_JNT_FINGERR_e=0x14,
    LUD_JNT_MANTLE_e=0x15,
    LUD_JNT_WAIST_e=0x16,
    LUD_JNT_LEGL1_e=0x17,
    LUD_JNT_LEGL2_e=0x18,
    LUD_JNT_FOOTL_e=0x19,
    LUD_JNT_LEGR1_e=0x1A,
    LUD_JNT_LEGR2_e=0x1B,
    LUD_JNT_FOOTR_e=0x1C,
};

#endif /* !RES_LUD_H */