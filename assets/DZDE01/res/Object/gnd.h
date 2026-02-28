#ifndef RES_GND_H
#define RES_GND_H

enum dRes_INDEX_GND {
    /* BCK  */
    dRes_INDEX_GND_BCK_GND_WAIT_A_e=0x8,
    /* BMDR */
    dRes_INDEX_GND_BMD_GND_e=0xB,
    /* BPK  */
    dRes_INDEX_GND_BPK_GND_e=0xE,
    /* BRK  */
    dRes_INDEX_GND_BRK_GND_e=0x11,
    /* BTK  */
    dRes_INDEX_GND_BTK_GND_e=0x14,
    /* BTP  */
    dRes_INDEX_GND_BTP_GND_e=0x17,
};

enum dRes_ID_GND {
    /* BCK  */
    dRes_ID_GND_BCK_GND_WAIT_A_e=0x8,
    /* BMDR */
    dRes_ID_GND_BMD_GND_e=0xB,
    /* BPK  */
    dRes_ID_GND_BPK_GND_e=0xE,
    /* BRK  */
    dRes_ID_GND_BRK_GND_e=0x11,
    /* BTK  */
    dRes_ID_GND_BTK_GND_e=0x14,
    /* BTP  */
    dRes_ID_GND_BTP_GND_e=0x17,
};

enum GND_JNT {
    GND_JNT_CENTER_e=0x0,
    GND_JNT_BACKBONE1_e=0x1,
    GND_JNT_BACKBONE2_e=0x2,
    GND_JNT_NECK_e=0x3,
    GND_JNT_HEAD_e=0x4,
    GND_JNT_CHIN_e=0x5,
    GND_JNT_MOUTHL2_e=0x6,
    GND_JNT_MOUTHR2_e=0x7,
    GND_JNT_TOOTH_e=0x8,
    GND_JNT_TUSKL_e=0x9,
    GND_JNT_TUSKR_e=0xA,
    GND_JNT_MAYUL1_e=0xB,
    GND_JNT_MAYUL2_e=0xC,
    GND_JNT_MAYUR1_e=0xD,
    GND_JNT_MAYUR2_e=0xE,
    GND_JNT_MOUTHL1_e=0xF,
    GND_JNT_MOUTHR1_e=0x10,
    GND_JNT_SHOULDERL_e=0x11,
    GND_JNT_ARML1_e=0x12,
    GND_JNT_ARML2_e=0x13,
    GND_JNT_HANDL_e=0x14,
    GND_JNT_FINGERL1_e=0x15,
    GND_JNT_FINGERL2_e=0x16,
    GND_JNT_THAMBL_e=0x17,
    GND_JNT_WEAPONL_e=0x18,
    GND_JNT_PADL_e=0x19,
    GND_JNT_SHOULDERR_e=0x1A,
    GND_JNT_ARMR1_e=0x1B,
    GND_JNT_ARMR2_e=0x1C,
    GND_JNT_HANDR_e=0x1D,
    GND_JNT_FINGERR1_e=0x1E,
    GND_JNT_FINGERR2_e=0x1F,
    GND_JNT_THAMBR_e=0x20,
    GND_JNT_WEAPONR_e=0x21,
    GND_JNT_PADR_e=0x22,
    GND_JNT_WAIST_e=0x23,
    GND_JNT_LEGL1_e=0x24,
    GND_JNT_LEGL2_e=0x25,
    GND_JNT_FOOTL_e=0x26,
    GND_JNT_TOEL_e=0x27,
    GND_JNT_LEGR1_e=0x28,
    GND_JNT_LEGR2_e=0x29,
    GND_JNT_FOOTR_e=0x2A,
    GND_JNT_TOER_e=0x2B,
};

#endif /* !RES_GND_H */