#ifndef RES_OBJ_RW_H
#define RES_OBJ_RW_H

enum dRes_INDEX_OBJ_RW {
    /* BMDR */
    dRes_INDEX_OBJ_RW_BMD_RW_e=0x3,
};

enum dRes_ID_OBJ_RW {
    /* BMDR */
    dRes_ID_OBJ_RW_BMD_RW_e=0x3,
};

enum RW_JNT {
    RW_JNT_ROOT_e=0x0,
    RW_JNT_NEMOTO_e=0x1,
    RW_JNT_BODY_e=0x2,
    RW_JNT_HEAD_e=0x3,
};

#endif /* !RES_OBJ_RW_H */