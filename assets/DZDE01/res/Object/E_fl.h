#ifndef RES_E_FL_H
#define RES_E_FL_H

enum dRes_INDEX_E_FL {
    /* BCK  */
    dRes_INDEX_E_FL_BCK_FL_BLIZZARD_e=0x5,
    dRes_INDEX_E_FL_BCK_FL_BLIZZARD_LOOP_e=0x6,
    dRes_INDEX_E_FL_BCK_FL_DAMAGE_S_e=0x7,
    dRes_INDEX_E_FL_BCK_FL_WAIT_e=0x8,
    /* BMDE */
    dRes_INDEX_E_FL_BMD_FL_MODEL_e=0xB,
    /* BRK  */
    dRes_INDEX_E_FL_BRK_FL_MODEL_e=0xE,
};

enum dRes_ID_E_FL {
    /* BCK  */
    dRes_ID_E_FL_BCK_FL_BLIZZARD_e=0x5,
    dRes_ID_E_FL_BCK_FL_BLIZZARD_LOOP_e=0x6,
    dRes_ID_E_FL_BCK_FL_DAMAGE_S_e=0x7,
    dRes_ID_E_FL_BCK_FL_WAIT_e=0x8,
    /* BMDE */
    dRes_ID_E_FL_BMD_FL_MODEL_e=0xB,
    /* BRK  */
    dRes_ID_E_FL_BRK_FL_MODEL_e=0xE,
};

enum FL_MODEL_JNT {
    FL_MODEL_JNT_CENTER_e=0x0,
    FL_MODEL_JNT_WEST_e=0x1,
    FL_MODEL_JNT_HEAD_e=0x2,
    FL_MODEL_JNT_CHIN_e=0x3,
};

#endif /* !RES_E_FL_H */