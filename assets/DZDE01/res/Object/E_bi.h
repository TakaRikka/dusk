#ifndef RES_E_BI_H
#define RES_E_BI_H

enum dRes_INDEX_E_BI {
    /* BCK  */
    dRes_INDEX_E_BI_BCK_BI_APPEAR_e=0x4,
    dRes_INDEX_E_BI_BCK_BI_ATTACK_e=0x5,
    dRes_INDEX_E_BI_BCK_BI_BOMBPOSE_e=0x6,
    dRes_INDEX_E_BI_BCK_BI_MOVE_e=0x7,
    dRes_INDEX_E_BI_BCK_BI_WAIT01_e=0x8,
    dRes_INDEX_E_BI_BCK_BI_WAIT01TO02_e=0x9,
    dRes_INDEX_E_BI_BCK_BI_WAIT02_e=0xA,
    dRes_INDEX_E_BI_BCK_BI_WAIT02TO01_e=0xB,
    /* BMDR */
    dRes_INDEX_E_BI_BMD_BI_e=0xE,
    dRes_INDEX_E_BI_BMD_BI_HOME_e=0xF,
};

enum dRes_ID_E_BI {
    /* BCK  */
    dRes_ID_E_BI_BCK_BI_APPEAR_e=0x4,
    dRes_ID_E_BI_BCK_BI_ATTACK_e=0x5,
    dRes_ID_E_BI_BCK_BI_BOMBPOSE_e=0x6,
    dRes_ID_E_BI_BCK_BI_MOVE_e=0x7,
    dRes_ID_E_BI_BCK_BI_WAIT01_e=0x8,
    dRes_ID_E_BI_BCK_BI_WAIT01TO02_e=0x9,
    dRes_ID_E_BI_BCK_BI_WAIT02_e=0xA,
    dRes_ID_E_BI_BCK_BI_WAIT02TO01_e=0xB,
    /* BMDR */
    dRes_ID_E_BI_BMD_BI_e=0xE,
    dRes_ID_E_BI_BMD_BI_HOME_e=0xF,
};

enum BI_JNT {
    BI_JNT_BOMB_e=0x0,
    BI_JNT_ARML1_e=0x1,
    BI_JNT_ARML2_e=0x2,
    BI_JNT_ARML3_e=0x3,
    BI_JNT_ARMR1_e=0x4,
    BI_JNT_ARMR2_e=0x5,
    BI_JNT_ARMR3_e=0x6,
    BI_JNT_LEGL1_e=0x7,
    BI_JNT_LEGL2_e=0x8,
    BI_JNT_LEGL3_e=0x9,
    BI_JNT_LEGR1_e=0xA,
    BI_JNT_LEGR2_e=0xB,
    BI_JNT_LEGR3_e=0xC,
};

enum BI_HOME_JNT {
    BI_HOME_JNT_BI_HOUSE_1_e=0x0,
};

#endif /* !RES_E_BI_H */