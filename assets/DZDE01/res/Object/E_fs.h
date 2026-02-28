#ifndef RES_E_FS_H
#define RES_E_FS_H

enum dRes_INDEX_E_FS {
    /* BCK  */
    dRes_INDEX_E_FS_BCK_FS_APPEAR_e=0x4,
    dRes_INDEX_E_FS_BCK_FS_ATTACK01_e=0x5,
    dRes_INDEX_E_FS_BCK_FS_DAMAGE_e=0x6,
    dRes_INDEX_E_FS_BCK_FS_DIE_e=0x7,
    dRes_INDEX_E_FS_BCK_FS_MOVE_e=0x8,
    dRes_INDEX_E_FS_BCK_FS_WAIT01_e=0x9,
    /* BMDR */
    dRes_INDEX_E_FS_BMD_FS_e=0xC,
};

enum dRes_ID_E_FS {
    /* BCK  */
    dRes_ID_E_FS_BCK_FS_APPEAR_e=0x4,
    dRes_ID_E_FS_BCK_FS_ATTACK01_e=0x5,
    dRes_ID_E_FS_BCK_FS_DAMAGE_e=0x6,
    dRes_ID_E_FS_BCK_FS_DIE_e=0x7,
    dRes_ID_E_FS_BCK_FS_MOVE_e=0x8,
    dRes_ID_E_FS_BCK_FS_WAIT01_e=0x9,
    /* BMDR */
    dRes_ID_E_FS_BMD_FS_e=0xC,
};

enum FS_JNT {
    FS_JNT_CENTER_e=0x0,
    FS_JNT_BACKBONE_e=0x1,
    FS_JNT_ARML1_e=0x2,
    FS_JNT_ARML2_e=0x3,
    FS_JNT_HANDL_e=0x4,
    FS_JNT_ARMR1_e=0x5,
    FS_JNT_ARMR2_e=0x6,
    FS_JNT_HANDR_e=0x7,
    FS_JNT_CLOAK1_e=0x8,
    FS_JNT_CLOAK2_e=0x9,
    FS_JNT_CLOAK3_e=0xA,
    FS_JNT_HEAD_e=0xB,
    FS_JNT_NECKLESS1_e=0xC,
    FS_JNT_NECKLESS2_e=0xD,
    FS_JNT_WAIST_e=0xE,
    FS_JNT_LEGL1_e=0xF,
    FS_JNT_LEGL2_e=0x10,
    FS_JNT_FOOTL_e=0x11,
    FS_JNT_LEGR1_e=0x12,
    FS_JNT_LEGR2_e=0x13,
    FS_JNT_FOOTR_e=0x14,
};

#endif /* !RES_E_FS_H */