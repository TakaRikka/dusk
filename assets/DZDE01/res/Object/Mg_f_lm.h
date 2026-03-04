#ifndef RES_MG_F_LM_H
#define RES_MG_F_LM_H

enum dRes_INDEX_MG_F_LM {
    /* BCK  */
    dRes_INDEX_MG_F_LM_BCK_LM_CLOSE_e=0x4,
    dRes_INDEX_MG_F_LM_BCK_LM_MOUTHOPEN_e=0x5,
    /* BMDR */
    dRes_INDEX_MG_F_LM_BMD_LM_e=0x8,
};

enum dRes_ID_MG_F_LM {
    /* BCK  */
    dRes_ID_MG_F_LM_BCK_LM_CLOSE_e=0x4,
    dRes_ID_MG_F_LM_BCK_LM_MOUTHOPEN_e=0x5,
    /* BMDR */
    dRes_ID_MG_F_LM_BMD_LM_e=0x8,
};

enum LM_JNT {
    LM_JNT_WORLD_ROOT_e=0x0,
    LM_JNT_BACKBONE1_e=0x1,
    LM_JNT_BACKBONE2_e=0x2,
    LM_JNT_BACKBONE3_e=0x3,
    LM_JNT_FIN_e=0x4,
    LM_JNT_FINL_e=0x5,
    LM_JNT_FINR1_e=0x6,
    LM_JNT_HEAD_e=0x7,
    LM_JNT_MOUTH1_e=0x8,
};

#endif /* !RES_MG_F_LM_H */