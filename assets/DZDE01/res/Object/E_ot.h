#ifndef RES_E_OT_H
#define RES_E_OT_H

enum dRes_INDEX_E_OT {
    /* BCK  */
    dRes_INDEX_E_OT_BCK_OT_BORN_e=0x6,
    dRes_INDEX_E_OT_BCK_OT_DAMAGE_e=0x7,
    dRes_INDEX_E_OT_BCK_OT_SWIM_e=0x8,
    /* BMDE */
    dRes_INDEX_E_OT_BMD_DT_EGG_e=0xB,
    /* BMDR */
    dRes_INDEX_E_OT_BMD_OT_e=0xE,
    /* BTK  */
    dRes_INDEX_E_OT_BTK_EGG_BEAT_e=0x11,
};

enum dRes_ID_E_OT {
    /* BCK  */
    dRes_ID_E_OT_BCK_OT_BORN_e=0x6,
    dRes_ID_E_OT_BCK_OT_DAMAGE_e=0x7,
    dRes_ID_E_OT_BCK_OT_SWIM_e=0x8,
    /* BMDE */
    dRes_ID_E_OT_BMD_DT_EGG_e=0xB,
    /* BMDR */
    dRes_ID_E_OT_BMD_OT_e=0xE,
    /* BTK  */
    dRes_ID_E_OT_BTK_EGG_BEAT_e=0x11,
};

enum DT_EGG_JNT {
    DT_EGG_JNT_DT_EGG_MODEL_e=0x0,
};

enum OT_JNT {
    OT_JNT_CENTER_e=0x0,
    OT_JNT_TAIL1_e=0x1,
    OT_JNT_TAIL2_e=0x2,
    OT_JNT_TAIL3_e=0x3,
};

#endif /* !RES_E_OT_H */