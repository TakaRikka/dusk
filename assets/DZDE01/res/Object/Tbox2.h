#ifndef RES_TBOX2_H
#define RES_TBOX2_H

enum dRes_INDEX_TBOX2 {
    /* BCK  */
    dRes_INDEX_TBOX2_BCK_K_TAKARAA_e=0x5,
    dRes_INDEX_TBOX2_BCK_K_TAKARAB_e=0x6,
    /* BMDR */
    dRes_INDEX_TBOX2_BMD_BOXA_e=0x9,
    dRes_INDEX_TBOX2_BMD_BOXB_e=0xA,
    /* DZB  */
    dRes_INDEX_TBOX2_DZB_BOXAC_e=0xD,
    dRes_INDEX_TBOX2_DZB_BOXAO_e=0xE,
    dRes_INDEX_TBOX2_DZB_BOXBC_e=0xF,
    dRes_INDEX_TBOX2_DZB_BOXBO_e=0x10,
};

enum dRes_ID_TBOX2 {
    /* BCK  */
    dRes_ID_TBOX2_BCK_K_TAKARAA_e=0x5,
    dRes_ID_TBOX2_BCK_K_TAKARAB_e=0x6,
    /* BMDR */
    dRes_ID_TBOX2_BMD_BOXA_e=0x9,
    dRes_ID_TBOX2_BMD_BOXB_e=0xA,
    /* DZB  */
    dRes_ID_TBOX2_DZB_BOXAC_e=0xD,
    dRes_ID_TBOX2_DZB_BOXAO_e=0xE,
    dRes_ID_TBOX2_DZB_BOXBC_e=0xF,
    dRes_ID_TBOX2_DZB_BOXBO_e=0x10,
};

enum BOXA_JNT {
    BOXA_JNT_BOXA_e=0x0,
    BOXA_JNT_COVER_JNT1_e=0x1,
};

enum BOXB_JNT {
    BOXB_JNT_BOXB_e=0x0,
    BOXB_JNT_COVER_JNT_e=0x1,
};

#endif /* !RES_TBOX2_H */