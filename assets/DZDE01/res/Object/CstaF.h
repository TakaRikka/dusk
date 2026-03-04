#ifndef RES_CSTAF_H
#define RES_CSTAF_H

enum dRes_INDEX_CSTAF {
    /* BCK  */
    dRes_INDEX_CSTAF_BCK_CS_F_A_FALL_e=0x7,
    dRes_INDEX_CSTAF_BCK_CS_F_A_SWING_REACTION_e=0x8,
    dRes_INDEX_CSTAF_BCK_CS_F_A_WALK_e=0x9,
    /* BMDR */
    dRes_INDEX_CSTAF_BMD_CS_F_A_e=0xC,
    /* BRK  */
    dRes_INDEX_CSTAF_BRK_CS_F_A_START_e=0xF,
    dRes_INDEX_CSTAF_BRK_CS_F_A_STOP_e=0x10,
    /* BTK  */
    dRes_INDEX_CSTAF_BTK_CS_F_A_START_e=0x13,
    /* DZB  */
    dRes_INDEX_CSTAF_DZB_CS_F_A_e=0x16,
};

enum dRes_ID_CSTAF {
    /* BCK  */
    dRes_ID_CSTAF_BCK_CS_F_A_FALL_e=0x7,
    dRes_ID_CSTAF_BCK_CS_F_A_SWING_REACTION_e=0x8,
    dRes_ID_CSTAF_BCK_CS_F_A_WALK_e=0x9,
    /* BMDR */
    dRes_ID_CSTAF_BMD_CS_F_A_e=0xC,
    /* BRK  */
    dRes_ID_CSTAF_BRK_CS_F_A_START_e=0xF,
    dRes_ID_CSTAF_BRK_CS_F_A_STOP_e=0x10,
    /* BTK  */
    dRes_ID_CSTAF_BTK_CS_F_A_START_e=0x13,
    /* DZB  */
    dRes_ID_CSTAF_DZB_CS_F_A_e=0x16,
};

enum CS_F_A_JNT {
    CS_F_A_JNT_CS_F_A_e=0x0,
};

#endif /* !RES_CSTAF_H */