#ifndef RES_TKJ2_H
#define RES_TKJ2_H

enum dRes_INDEX_TKJ2 {
    /* BCK  */
    dRes_INDEX_TKJ2_BCK_TKJ_FLY_e=0x6,
    dRes_INDEX_TKJ2_BCK_TKJ_FRY_FALL_e=0x7,
    dRes_INDEX_TKJ2_BCK_TKJ_F_TALK_A_e=0x8,
    dRes_INDEX_TKJ2_BCK_TKJ_MOGAKU_e=0x9,
    dRes_INDEX_TKJ2_BCK_TKJ_WAIT_A_e=0xA,
    dRes_INDEX_TKJ2_BCK_TKJ_WALK_A_e=0xB,
    /* BMDR */
    dRes_INDEX_TKJ2_BMD_TKJ_e=0xE,
    /* BTK  */
    dRes_INDEX_TKJ2_BTK_TJK_e=0x11,
    /* BTP  */
    dRes_INDEX_TKJ2_BTP_TKJ_e=0x14,
};

enum dRes_ID_TKJ2 {
    /* BCK  */
    dRes_ID_TKJ2_BCK_TKJ_FLY_e=0x6,
    dRes_ID_TKJ2_BCK_TKJ_FRY_FALL_e=0x7,
    dRes_ID_TKJ2_BCK_TKJ_F_TALK_A_e=0x8,
    dRes_ID_TKJ2_BCK_TKJ_MOGAKU_e=0x9,
    dRes_ID_TKJ2_BCK_TKJ_WAIT_A_e=0xA,
    dRes_ID_TKJ2_BCK_TKJ_WALK_A_e=0xB,
    /* BMDR */
    dRes_ID_TKJ2_BMD_TKJ_e=0xE,
    /* BTK  */
    dRes_ID_TKJ2_BTK_TJK_e=0x11,
    /* BTP  */
    dRes_ID_TKJ2_BTP_TKJ_e=0x14,
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

#endif /* !RES_TKJ2_H */