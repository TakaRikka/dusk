#ifndef RES_LV5_KEY_H
#define RES_LV5_KEY_H

enum dRes_INDEX_LV5_KEY {
    /* BCK  */
    dRes_INDEX_LV5_KEY_BCK_O_LV5_LOCK_NOMAL_e=0x4,
    /* BMDR */
    dRes_INDEX_LV5_KEY_BMD_O_LV5_LOCK_NOMAL_e=0x7,
};

enum dRes_ID_LV5_KEY {
    /* BCK  */
    dRes_ID_LV5_KEY_BCK_O_LV5_LOCK_NOMAL_e=0x4,
    /* BMDR */
    dRes_ID_LV5_KEY_BMD_O_LV5_LOCK_NOMAL_e=0x7,
};

enum O_LV5_LOCK_NOMAL_JNT {
    O_LV5_LOCK_NOMAL_JNT_LOCATOR1_e=0x0,
    O_LV5_LOCK_NOMAL_JNT_ROOT_e=0x1,
    O_LV5_LOCK_NOMAL_JNT_OPEN_e=0x2,
};

#endif /* !RES_LV5_KEY_H */