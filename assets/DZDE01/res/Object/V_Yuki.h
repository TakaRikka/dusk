#ifndef RES_V_YUKI_H
#define RES_V_YUKI_H

enum dRes_INDEX_V_YUKI {
    /* BCK  */
    dRes_INDEX_V_YUKI_BCK_V_YUKI_e=0x6,
    /* BMDR */
    dRes_INDEX_V_YUKI_BMD_V_YUKI_e=0x9,
    /* DZB  */
    dRes_INDEX_V_YUKI_DZB_AFTER_e=0xC,
    dRes_INDEX_V_YUKI_DZB_BEFORE_e=0xD,
    /* EVT  */
    dRes_INDEX_V_YUKI_DAT_EVENT_LIST_e=0x10,
};

enum dRes_ID_V_YUKI {
    /* BCK  */
    dRes_ID_V_YUKI_BCK_V_YUKI_e=0x6,
    /* BMDR */
    dRes_ID_V_YUKI_BMD_V_YUKI_e=0x9,
    /* DZB  */
    dRes_ID_V_YUKI_DZB_AFTER_e=0xC,
    dRes_ID_V_YUKI_DZB_BEFORE_e=0xD,
    /* EVT  */
    dRes_ID_V_YUKI_DAT_EVENT_LIST_e=0x10,
};

enum V_YUKI_JNT {
    V_YUKI_JNT_DIS_e=0x0,
    V_YUKI_JNT_AFTER_e=0x1,
    V_YUKI_JNT_AFTER_00_e=0x2,
    V_YUKI_JNT_BEFORE_e=0x3,
    V_YUKI_JNT_BEFORE_00_e=0x4,
};

#endif /* !RES_V_YUKI_H */