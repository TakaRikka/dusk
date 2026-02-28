#ifndef RES_NPC_DF_H
#define RES_NPC_DF_H

enum dRes_INDEX_NPC_DF {
    /* BMDG */
    dRes_INDEX_NPC_DF_BMD_DF_e=0x3,
    dRes_INDEX_NPC_DF_BMD_DF_WING_e=0x4,
};

enum dRes_ID_NPC_DF {
    /* BMDG */
    dRes_ID_NPC_DF_BMD_DF_e=0x3,
    dRes_ID_NPC_DF_BMD_DF_WING_e=0x4,
};

enum DF_JNT {
    DF_JNT_WORLD_ROOT_e=0x0,
    DF_JNT_BODY_e=0x1,
    DF_JNT_HEAD_e=0x2,
    DF_JNT_TAIL_e=0x3,
};

enum DF_WING_JNT {
    DF_WING_JNT_DF_HANE_e=0x0,
};

#endif /* !RES_NPC_DF_H */