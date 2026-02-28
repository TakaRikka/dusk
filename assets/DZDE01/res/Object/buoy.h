#ifndef RES_BUOY_H
#define RES_BUOY_H

enum dRes_INDEX_BUOY {
    /* BMDR */
    dRes_INDEX_BUOY_BMD_BUOY_A_e=0x3,
    dRes_INDEX_BUOY_BMD_BUOY_B_e=0x4,
    dRes_INDEX_BUOY_BMD_BUOY_C_e=0x5,
    dRes_INDEX_BUOY_BMD_BUOY_D_e=0x6,
    dRes_INDEX_BUOY_BMD_BUOY_PIECE_A_e=0x7,
    dRes_INDEX_BUOY_BMD_BUOY_PIECE_B_e=0x8,
};

enum dRes_ID_BUOY {
    /* BMDR */
    dRes_ID_BUOY_BMD_BUOY_A_e=0x3,
    dRes_ID_BUOY_BMD_BUOY_B_e=0x4,
    dRes_ID_BUOY_BMD_BUOY_C_e=0x5,
    dRes_ID_BUOY_BMD_BUOY_D_e=0x6,
    dRes_ID_BUOY_BMD_BUOY_PIECE_A_e=0x7,
    dRes_ID_BUOY_BMD_BUOY_PIECE_B_e=0x8,
};

enum BUOY_A_JNT {
    BUOY_A_JNT_BUOY_A_e=0x0,
    BUOY_A_JNT_BALL_e=0x1,
    BUOY_A_JNT_BUOY_A_1_e=0x2,
};

enum BUOY_B_JNT {
    BUOY_B_JNT_BUOY_B_e=0x0,
    BUOY_B_JNT_BALL_e=0x1,
    BUOY_B_JNT_BUOY_B_1_e=0x2,
};

enum BUOY_C_JNT {
    BUOY_C_JNT_BUOY_C_e=0x0,
    BUOY_C_JNT_BALL_e=0x1,
    BUOY_C_JNT_BUOY_C_1_e=0x2,
};

enum BUOY_D_JNT {
    BUOY_D_JNT_BUOY_D_e=0x0,
    BUOY_D_JNT_BALL_e=0x1,
    BUOY_D_JNT_BUOY_D_1_e=0x2,
};

enum BUOY_PIECE_A_JNT {
    BUOY_PIECE_A_JNT_BUOY_PIECE_A_e=0x0,
};

enum BUOY_PIECE_B_JNT {
    BUOY_PIECE_B_JNT_BUOY_PIECE_B_e=0x0,
};

#endif /* !RES_BUOY_H */