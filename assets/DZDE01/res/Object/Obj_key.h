#ifndef RES_OBJ_KEY_H
#define RES_OBJ_KEY_H

enum dRes_INDEX_OBJ_KEY {
    /* BMDR */
    dRes_INDEX_OBJ_KEY_BMD_K_KEY00_e=0x3,
    dRes_INDEX_OBJ_KEY_BMD_K_KUSARI00_e=0x4,
    dRes_INDEX_OBJ_KEY_BMD_RK_e=0x5,
};

enum dRes_ID_OBJ_KEY {
    /* BMDR */
    dRes_ID_OBJ_KEY_BMD_K_KEY00_e=0x3,
    dRes_ID_OBJ_KEY_BMD_K_KUSARI00_e=0x4,
    dRes_ID_OBJ_KEY_BMD_RK_e=0x5,
};

enum K_KEY00_JNT {
    K_KEY00_JNT_T_GD_KYE_e=0x0,
};

enum K_KUSARI00_JNT {
    K_KUSARI00_JNT_K_KUSARI00_e=0x0,
};

enum RK_JNT {
    RK_JNT_RING_KEY_MODEL_e=0x0,
};

#endif /* !RES_OBJ_KEY_H */