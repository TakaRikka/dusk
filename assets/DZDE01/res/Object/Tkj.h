#ifndef RES_TKJ_H
#define RES_TKJ_H

enum dRes_INDEX_TKJ {
    /* BCK  */
    dRes_INDEX_TKJ_BCK_TKJ_F_TALK_A_e=0x6,
    dRes_INDEX_TKJ_BCK_TKJ_MOGAKU_e=0x7,
    dRes_INDEX_TKJ_BCK_TKJ_RUN_e=0x8,
    dRes_INDEX_TKJ_BCK_TKJ_WAIT_A_e=0x9,
    /* BMDR */
    dRes_INDEX_TKJ_BMD_TKJ_e=0xC,
    /* BTK  */
    dRes_INDEX_TKJ_BTK_TKJ_e=0xF,
    /* BTP  */
    dRes_INDEX_TKJ_BTP_TKJ_e=0x12,
};

enum dRes_ID_TKJ {
    /* BCK  */
    dRes_ID_TKJ_BCK_TKJ_F_TALK_A_e=0x6,
    dRes_ID_TKJ_BCK_TKJ_MOGAKU_e=0x7,
    dRes_ID_TKJ_BCK_TKJ_RUN_e=0x8,
    dRes_ID_TKJ_BCK_TKJ_WAIT_A_e=0x9,
    /* BMDR */
    dRes_ID_TKJ_BMD_TKJ_e=0xC,
    /* BTK  */
    dRes_ID_TKJ_BTK_TKJ_e=0xF,
    /* BTP  */
    dRes_ID_TKJ_BTP_TKJ_e=0x12,
};

enum TKJ_JNT {
    TKJ_JNT_CENTER_e=0x0,
    TKJ_JNT_ARML1_e=0x1,
    TKJ_JNT_ARML2_e=0x2,
    TKJ_JNT_HANDL_e=0x3,
    TKJ_JNT_TUBASAL_e=0x4,
    TKJ_JNT_ARMR1_e=0x5,
    TKJ_JNT_ARMR2_e=0x6,
    TKJ_JNT_HANDR_e=0x7,
    TKJ_JNT_TUBASAR_e=0x8,
    TKJ_JNT_LEGL_e=0x9,
    TKJ_JNT_FOOTL1_e=0xA,
    TKJ_JNT_FOOTL2_e=0xB,
    TKJ_JNT_LEGR_e=0xC,
    TKJ_JNT_FOOTR1_e=0xD,
    TKJ_JNT_FOOTR2_e=0xE,
    TKJ_JNT_NECK1_e=0xF,
    TKJ_JNT_NECK2_e=0x10,
    TKJ_JNT_HEAD_e=0x11,
    TKJ_JNT_CHIN_e=0x12,
    TKJ_JNT_MOUTH_e=0x13,
};

#endif /* !RES_TKJ_H */