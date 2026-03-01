#ifndef RES_PPOLAMP_H
#define RES_PPOLAMP_H

enum dRes_INDEX_PPOLAMP {
    /* BCK  */
    dRes_INDEX_PPOLAMP_BCK_EF_GLOW_e=0x4,
    /* BMDR */
    dRes_INDEX_PPOLAMP_BMD_EF_GLOW_e=0x7,
    dRes_INDEX_PPOLAMP_BMD_P_POLAMP_e=0x8,
};

enum dRes_ID_PPOLAMP {
    /* BCK  */
    dRes_ID_PPOLAMP_BCK_EF_GLOW_e=0x4,
    /* BMDR */
    dRes_ID_PPOLAMP_BMD_EF_GLOW_e=0x7,
    dRes_ID_PPOLAMP_BMD_P_POLAMP_e=0x8,
};

enum EF_GLOW_JNT {
    EF_GLOW_JNT_EF_GLOW_e=0x0,
};

enum P_POLAMP_JNT {
    P_POLAMP_JNT_WORLD_ROOT_e=0x0,
};

#endif /* !RES_PPOLAMP_H */