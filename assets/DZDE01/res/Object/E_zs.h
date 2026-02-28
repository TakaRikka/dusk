#ifndef RES_E_ZS_H
#define RES_E_ZS_H

enum dRes_INDEX_E_ZS {
    /* BCK  */
    dRes_INDEX_E_ZS_BCK_ZS_APPEAR_e=0x4,
    dRes_INDEX_E_ZS_BCK_ZS_DAMAGE_e=0x5,
    dRes_INDEX_E_ZS_BCK_ZS_DEAD_e=0x6,
    dRes_INDEX_E_ZS_BCK_ZS_WAIT01_e=0x7,
    dRes_INDEX_E_ZS_BCK_ZS_WAIT02_e=0x8,
    dRes_INDEX_E_ZS_BCK_ZS_WAIT03_e=0x9,
    /* BMDE */
    dRes_INDEX_E_ZS_BMD_ZS_e=0xC,
};

enum dRes_ID_E_ZS {
    /* BCK  */
    dRes_ID_E_ZS_BCK_ZS_APPEAR_e=0x4,
    dRes_ID_E_ZS_BCK_ZS_DAMAGE_e=0x5,
    dRes_ID_E_ZS_BCK_ZS_DEAD_e=0x6,
    dRes_ID_E_ZS_BCK_ZS_WAIT01_e=0x7,
    dRes_ID_E_ZS_BCK_ZS_WAIT02_e=0x8,
    dRes_ID_E_ZS_BCK_ZS_WAIT03_e=0x9,
    /* BMDE */
    dRes_ID_E_ZS_BMD_ZS_e=0xC,
};

enum ZS_JNT {
    ZS_JNT_WAIST_e=0x0,
    ZS_JNT_BACKBONE_e=0x1,
    ZS_JNT_ARML_e=0x2,
    ZS_JNT_ARMR_e=0x3,
    ZS_JNT_HEAD_e=0x4,
};

#endif /* !RES_E_ZS_H */