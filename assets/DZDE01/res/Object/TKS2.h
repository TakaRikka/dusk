#ifndef RES_TKS2_H
#define RES_TKS2_H

enum dRes_INDEX_TKS2 {
    /* BCK  */
    dRes_INDEX_TKS2_BCK_TKS_F_TALK_A_e=0x7,
    dRes_INDEX_TKS2_BCK_TKS_FRY_e=0x8,
    dRes_INDEX_TKS2_BCK_TKS_JUMP_E_e=0x9,
    dRes_INDEX_TKS2_BCK_TKS_JUMP_S_e=0xA,
    dRes_INDEX_TKS2_BCK_TKS_WAIT_A_e=0xB,
    /* BMDR */
    dRes_INDEX_TKS2_BMD_TKS_e=0xE,
    /* BTK  */
    dRes_INDEX_TKS2_BTK_TKS_e=0x11,
    /* BTP  */
    dRes_INDEX_TKS2_BTP_TKS_e=0x14,
    /* EVT  */
    dRes_INDEX_TKS2_DAT_EVENT_LIST_e=0x17,
};

enum dRes_ID_TKS2 {
    /* BCK  */
    dRes_ID_TKS2_BCK_TKS_F_TALK_A_e=0x7,
    dRes_ID_TKS2_BCK_TKS_FRY_e=0x8,
    dRes_ID_TKS2_BCK_TKS_JUMP_E_e=0x9,
    dRes_ID_TKS2_BCK_TKS_JUMP_S_e=0xA,
    dRes_ID_TKS2_BCK_TKS_WAIT_A_e=0xB,
    /* BMDR */
    dRes_ID_TKS2_BMD_TKS_e=0xE,
    /* BTK  */
    dRes_ID_TKS2_BTK_TKS_e=0x11,
    /* BTP  */
    dRes_ID_TKS2_BTP_TKS_e=0x14,
    /* EVT  */
    dRes_ID_TKS2_DAT_EVENT_LIST_e=0x17,
};

enum TKS_JNT {
    TKS_JNT_CENTER_e=0x0,
    TKS_JNT_ARML1_e=0x1,
    TKS_JNT_ARML2_e=0x2,
    TKS_JNT_HANDL_e=0x3,
    TKS_JNT_TUBASAL_e=0x4,
    TKS_JNT_ARMR1_e=0x5,
    TKS_JNT_ARMR2_e=0x6,
    TKS_JNT_HANDR_e=0x7,
    TKS_JNT_TUBASAR_e=0x8,
    TKS_JNT_LEGL_e=0x9,
    TKS_JNT_FOOTL1_e=0xA,
    TKS_JNT_FOOTL2_e=0xB,
    TKS_JNT_LEGR_e=0xC,
    TKS_JNT_FOOTR1_e=0xD,
    TKS_JNT_FOOTR2_e=0xE,
    TKS_JNT_NECK1_e=0xF,
    TKS_JNT_NECK2_e=0x10,
    TKS_JNT_HEAD_e=0x11,
    TKS_JNT_CHIN_e=0x12,
    TKS_JNT_MOUTH_e=0x13,
};

#endif /* !RES_TKS2_H */