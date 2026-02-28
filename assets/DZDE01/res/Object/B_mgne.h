#ifndef RES_B_MGNE_H
#define RES_B_MGNE_H

enum dRes_INDEX_B_MGNE {
    /* BCK  */
    dRes_INDEX_B_MGNE_BCK_EF_GDGATE_START_e=0x6,
    /* BMDR */
    dRes_INDEX_B_MGNE_BMD_EF_GDGATE_e=0x9,
    /* BRK  */
    dRes_INDEX_B_MGNE_BRK_EF_GDGATE_APPEAR_e=0xC,
    dRes_INDEX_B_MGNE_BRK_EF_GDGATE_START_e=0xD,
    /* BTK  */
    dRes_INDEX_B_MGNE_BTK_EF_GDGATE_e=0x10,
};

enum dRes_ID_B_MGNE {
    /* BCK  */
    dRes_ID_B_MGNE_BCK_EF_GDGATE_START_e=0x6,
    /* BMDR */
    dRes_ID_B_MGNE_BMD_EF_GDGATE_e=0x9,
    /* BRK  */
    dRes_ID_B_MGNE_BRK_EF_GDGATE_APPEAR_e=0xC,
    dRes_ID_B_MGNE_BRK_EF_GDGATE_START_e=0xD,
    /* BTK  */
    dRes_ID_B_MGNE_BTK_EF_GDGATE_e=0x10,
};

enum EF_GDGATE_JNT {
    EF_GDGATE_JNT_WORLD_ROOT_e=0x0,
};

#endif /* !RES_B_MGNE_H */