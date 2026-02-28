#ifndef RES_E_BU_H
#define RES_E_BU_H

enum dRes_INDEX_E_BU {
    /* BCK  */
    dRes_INDEX_E_BU_BCK_BU_DIE_e=0x4,
    dRes_INDEX_E_BU_BCK_BU_GABU2_e=0x5,
    dRes_INDEX_E_BU_BCK_BU_TAKEOFF01_e=0x6,
    dRes_INDEX_E_BU_BCK_BU_WAIT01_e=0x7,
    dRes_INDEX_E_BU_BCK_BU_WINGCLOSE_e=0x8,
    /* BMDR */
    dRes_INDEX_E_BU_BMD_BU_e=0xB,
};

enum dRes_ID_E_BU {
    /* BCK  */
    dRes_ID_E_BU_BCK_BU_DIE_e=0x4,
    dRes_ID_E_BU_BCK_BU_GABU2_e=0x5,
    dRes_ID_E_BU_BCK_BU_TAKEOFF01_e=0x6,
    dRes_ID_E_BU_BCK_BU_WAIT01_e=0x7,
    dRes_ID_E_BU_BCK_BU_WINGCLOSE_e=0x8,
    /* BMDR */
    dRes_ID_E_BU_BMD_BU_e=0xB,
};

enum BU_JNT {
    BU_JNT_HEAD_e=0x0,
    BU_JNT_ARML1_e=0x1,
    BU_JNT_ARML2_e=0x2,
    BU_JNT_HANDL_e=0x3,
    BU_JNT_FINGERLA1_e=0x4,
    BU_JNT_FINGERLA2_e=0x5,
    BU_JNT_FINGERLB_e=0x6,
    BU_JNT_FINGERLC_e=0x7,
    BU_JNT_FINGERLD_e=0x8,
    BU_JNT_ARMR1_e=0x9,
    BU_JNT_ARMR2_e=0xA,
    BU_JNT_HANDR_e=0xB,
    BU_JNT_FINGERRA1_e=0xC,
    BU_JNT_FINGERRA2_e=0xD,
    BU_JNT_FINGERRB_e=0xE,
    BU_JNT_FINGERRC_e=0xF,
    BU_JNT_FINGERRD_e=0x10,
    BU_JNT_JAW_e=0x11,
};

#endif /* !RES_E_BU_H */