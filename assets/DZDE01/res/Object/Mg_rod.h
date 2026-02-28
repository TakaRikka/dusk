#ifndef RES_MG_ROD_H
#define RES_MG_ROD_H

enum dRes_INDEX_MG_ROD {
    /* BMDR */
    dRes_INDEX_MG_ROD_BMD_AL_LROD_e=0x4,
    dRes_INDEX_MG_ROD_BMD_FOOK_e=0x5,
    dRes_INDEX_MG_ROD_BMD_LURE_e=0x6,
    dRes_INDEX_MG_ROD_BMD_PENCIL_e=0x7,
    dRes_INDEX_MG_ROD_BMD_POPPER_e=0x8,
    dRes_INDEX_MG_ROD_BMD_RING_e=0x9,
    dRes_INDEX_MG_ROD_BMD_WS_e=0xA,
    /* BMDV */
    dRes_INDEX_MG_ROD_BMD_SPOON_e=0xD,
};

enum dRes_ID_MG_ROD {
    /* BMDR */
    dRes_ID_MG_ROD_BMD_AL_LROD_e=0x4,
    dRes_ID_MG_ROD_BMD_FOOK_e=0x5,
    dRes_ID_MG_ROD_BMD_LURE_e=0x6,
    dRes_ID_MG_ROD_BMD_PENCIL_e=0x7,
    dRes_ID_MG_ROD_BMD_POPPER_e=0x8,
    dRes_ID_MG_ROD_BMD_RING_e=0x9,
    dRes_ID_MG_ROD_BMD_WS_e=0xA,
    /* BMDV */
    dRes_ID_MG_ROD_BMD_SPOON_e=0xD,
};

enum AL_LROD_JNT {
    AL_LROD_JNT_CENTER_e=0x0,
    AL_LROD_JNT_HANDLE_e=0x1,
    AL_LROD_JNT_ROD_NECK_e=0x2,
    AL_LROD_JNT_SPOOL_e=0x3,
};

enum FOOK_JNT {
    FOOK_JNT_FOOK_MODEL_e=0x0,
};

enum LURE_JNT {
    LURE_JNT_CENTER_e=0x0,
    LURE_JNT_LEG_e=0x1,
};

enum PENCIL_JNT {
    PENCIL_JNT_PENCIL_MODEL_e=0x0,
};

enum POPPER_JNT {
    POPPER_JNT_POPPER_MODEL_e=0x0,
};

enum RING_JNT {
    RING_JNT_RING_MODEL_e=0x0,
};

enum WS_JNT {
    WS_JNT_CENTER_e=0x0,
    WS_JNT_FRONT_e=0x1,
    WS_JNT_REAR_e=0x2,
};

enum SPOON_JNT {
    SPOON_JNT_SPOON_MODEL_e=0x0,
};

#endif /* !RES_MG_ROD_H */