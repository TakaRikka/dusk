#ifndef RES_BD_H
#define RES_BD_H

enum dRes_INDEX_BD {
    /* BCK  */
    dRes_INDEX_BD_BCK_BD_FLY_e=0x5,
    dRes_INDEX_BD_BCK_BD_KAKKU_e=0x6,
    dRes_INDEX_BD_BCK_BD_KAKKU1_e=0x7,
    dRes_INDEX_BD_BCK_BD_PITA_DOWN_e=0x8,
    dRes_INDEX_BD_BCK_BD_PITA_LEFTUP_e=0x9,
    dRes_INDEX_BD_BCK_BD_PITA_RIGHT_e=0xA,
    dRes_INDEX_BD_BCK_BD_PYON_e=0xB,
    /* BMDR */
    dRes_INDEX_BD_BMD_BD_e=0xE,
    /* BTK  */
    dRes_INDEX_BD_BTK_BD_e=0x11,
};

enum dRes_ID_BD {
    /* BCK  */
    dRes_ID_BD_BCK_BD_FLY_e=0x5,
    dRes_ID_BD_BCK_BD_KAKKU_e=0x6,
    dRes_ID_BD_BCK_BD_KAKKU1_e=0x7,
    dRes_ID_BD_BCK_BD_PITA_DOWN_e=0x8,
    dRes_ID_BD_BCK_BD_PITA_LEFTUP_e=0x9,
    dRes_ID_BD_BCK_BD_PITA_RIGHT_e=0xA,
    dRes_ID_BD_BCK_BD_PYON_e=0xB,
    /* BMDR */
    dRes_ID_BD_BMD_BD_e=0xE,
    /* BTK  */
    dRes_ID_BD_BTK_BD_e=0x11,
};

enum BD_JNT {
    BD_JNT_CENTER_e=0x0,
    BD_JNT_FOOT_e=0x1,
    BD_JNT_HANE_L_e=0x2,
    BD_JNT_HANE_R_e=0x3,
    BD_JNT_SHIPO_e=0x4,
};

#endif /* !RES_BD_H */