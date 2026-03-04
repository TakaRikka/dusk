#ifndef RES_WARPBUG_H
#define RES_WARPBUG_H

enum dRes_INDEX_WARPBUG {
    /* BMDR */
    dRes_INDEX_WARPBUG_BMD_KISEI_e=0x5,
    /* BRK  */
    dRes_INDEX_WARPBUG_BRK_KISEI_e=0x8,
    /* BTK  */
    dRes_INDEX_WARPBUG_BTK_KISEI_e=0xB,
};

enum dRes_ID_WARPBUG {
    /* BMDR */
    dRes_ID_WARPBUG_BMD_KISEI_e=0x5,
    /* BRK  */
    dRes_ID_WARPBUG_BRK_KISEI_e=0x8,
    /* BTK  */
    dRes_ID_WARPBUG_BTK_KISEI_e=0xB,
};

enum KISEI_JNT {
    KISEI_JNT_WORLD_ROOT_e=0x0,
    KISEI_JNT_KINSHI_e=0x1,
    KISEI_JNT_YM_MODEL1_e=0x2,
};

#endif /* !RES_WARPBUG_H */