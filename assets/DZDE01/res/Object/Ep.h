#ifndef RES_EP_H
#define RES_EP_H

enum dRes_INDEX_EP {
    /* BMDR */
    dRes_INDEX_EP_BMD_KSYOK00_e=0x3,
    dRes_INDEX_EP_BMD_KSYOK01_e=0x4,
    dRes_INDEX_EP_BMD_KSYOK02_e=0x5,
    dRes_INDEX_EP_BMD_KSYOK03_e=0x6,
    dRes_INDEX_EP_BMD_KSYOKP00_e=0x7,
};

enum dRes_ID_EP {
    /* BMDR */
    dRes_ID_EP_BMD_KSYOK00_e=0x3,
    dRes_ID_EP_BMD_KSYOK01_e=0x4,
    dRes_ID_EP_BMD_KSYOK02_e=0x5,
    dRes_ID_EP_BMD_KSYOK03_e=0x6,
    dRes_ID_EP_BMD_KSYOKP00_e=0x7,
};

enum KSYOK00_JNT {
    KSYOK00_JNT_WORLD_ROOT_e=0x0,
};

enum KSYOK01_JNT {
    KSYOK01_JNT_KSYOK01_e=0x0,
};

enum KSYOK02_JNT {
    KSYOK02_JNT_KSYOK02_e=0x0,
};

enum KSYOK03_JNT {
    KSYOK03_JNT_KSYOK03_e=0x0,
};

enum KSYOKP00_JNT {
    KSYOKP00_JNT_WORLD_ROOT_e=0x0,
};

#endif /* !RES_EP_H */