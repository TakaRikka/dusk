#ifndef RES_URI_H
#define RES_URI_H

enum dRes_INDEX_URI {
    /* BCK  */
    dRes_INDEX_URI_BCK_URI_F_TALK_A_e=0x6,
    dRes_INDEX_URI_BCK_URI_K_F_HAPPY_e=0x7,
    dRes_INDEX_URI_BCK_URI_K_FH_HAPPY_e=0x8,
    dRes_INDEX_URI_BCK_URI_WAIT_A_e=0x9,
    /* BMDR */
    dRes_INDEX_URI_BMD_URI_e=0xC,
    /* BTK  */
    dRes_INDEX_URI_BTK_URI_e=0xF,
    /* BTP  */
    dRes_INDEX_URI_BTP_URI_e=0x12,
    dRes_INDEX_URI_BTP_URI_F_TALK_A_e=0x13,
    dRes_INDEX_URI_BTP_URI_K_F_HAPPY_e=0x14,
    dRes_INDEX_URI_BTP_URI_K_FH_HAPPY_e=0x15,
};

enum dRes_ID_URI {
    /* BCK  */
    dRes_ID_URI_BCK_URI_F_TALK_A_e=0x6,
    dRes_ID_URI_BCK_URI_K_F_HAPPY_e=0x7,
    dRes_ID_URI_BCK_URI_K_FH_HAPPY_e=0x8,
    dRes_ID_URI_BCK_URI_WAIT_A_e=0x9,
    /* BMDR */
    dRes_ID_URI_BMD_URI_e=0xC,
    /* BTK  */
    dRes_ID_URI_BTK_URI_e=0xF,
    /* BTP  */
    dRes_ID_URI_BTP_URI_e=0x12,
    dRes_ID_URI_BTP_URI_F_TALK_A_e=0x13,
    dRes_ID_URI_BTP_URI_K_F_HAPPY_e=0x14,
    dRes_ID_URI_BTP_URI_K_FH_HAPPY_e=0x15,
};

enum URI_JNT {
    URI_JNT_CENTER_e=0x0,
    URI_JNT_BACKBONE1_e=0x1,
    URI_JNT_BACKBONE2_e=0x2,
    URI_JNT_NECK_e=0x3,
    URI_JNT_HEAD_e=0x4,
    URI_JNT_CHIN_e=0x5,
    URI_JNT_MAYU_L_e=0x6,
    URI_JNT_MAYU_R_e=0x7,
    URI_JNT_MOUTH_e=0x8,
    URI_JNT_SHOULDERL_e=0x9,
    URI_JNT_ARML1_e=0xA,
    URI_JNT_ARML2_1_e=0xB,
    URI_JNT_HANDL_1_e=0xC,
    URI_JNT_FINGERL1_e=0xD,
    URI_JNT_SHOULDERR_e=0xE,
    URI_JNT_ARMR1_e=0xF,
    URI_JNT_ARMR2_1_e=0x10,
    URI_JNT_HANDR_1_e=0x11,
    URI_JNT_FINGERR1_e=0x12,
    URI_JNT_SKIRTB1_e=0x13,
    URI_JNT_SKIRTF1_e=0x14,
    URI_JNT_SKIRTF2_e=0x15,
    URI_JNT_WAIST_e=0x16,
    URI_JNT_LEGL1_e=0x17,
    URI_JNT_LEGL2_e=0x18,
    URI_JNT_FOOTL_e=0x19,
    URI_JNT_LEGR1_e=0x1A,
    URI_JNT_LEGR2_e=0x1B,
    URI_JNT_FOOTR_e=0x1C,
};

#endif /* !RES_URI_H */