#ifndef RES_DMIDNA_H
#define RES_DMIDNA_H

enum dRes_INDEX_DMIDNA {
    /* BCK  */
    dRes_INDEX_DMIDNA_BCK_D_MD_DOWN_WAITA_e=0x4,
    dRes_INDEX_DMIDNA_BCK_D_MD_DOWN_WAITB_e=0x5,
    /* BMDV */
    dRes_INDEX_DMIDNA_BMD_D_MD_e=0x8,
};

enum dRes_ID_DMIDNA {
    /* BCK  */
    dRes_ID_DMIDNA_BCK_D_MD_DOWN_WAITA_e=0x4,
    dRes_ID_DMIDNA_BCK_D_MD_DOWN_WAITB_e=0x5,
    /* BMDV */
    dRes_ID_DMIDNA_BMD_D_MD_e=0x8,
};

enum D_MD_JNT {
    D_MD_JNT_WORLD_ROOT_e=0x0,
    D_MD_JNT_BACKBONE1_e=0x1,
    D_MD_JNT_BACKBONE2_e=0x2,
    D_MD_JNT_NECK_e=0x3,
    D_MD_JNT_HEAD_e=0x4,
    D_MD_JNT_CHIN_e=0x5,
    D_MD_JNT_HAIR_e=0x6,
    D_MD_JNT_MOUTH_e=0x7,
    D_MD_JNT_SHOULDER_L_e=0x8,
    D_MD_JNT_ARM_L1_e=0x9,
    D_MD_JNT_ARM_L2_e=0xA,
    D_MD_JNT_HAND_L_e=0xB,
    D_MD_JNT_SHOULDER_R_e=0xC,
    D_MD_JNT_ARM_R1_e=0xD,
    D_MD_JNT_ARM_R2_e=0xE,
    D_MD_JNT_HAND_R_e=0xF,
    D_MD_JNT_WAIST_e=0x10,
    D_MD_JNT_CLOTCHL_e=0x11,
    D_MD_JNT_LEG_L1_e=0x12,
    D_MD_JNT_LEG_L2_e=0x13,
    D_MD_JNT_FOOT_L_e=0x14,
    D_MD_JNT_CLOTCHR_e=0x15,
    D_MD_JNT_LEG_R1_e=0x16,
    D_MD_JNT_LEG_R2_e=0x17,
    D_MD_JNT_FOOT_R_e=0x18,
};

#endif /* !RES_DMIDNA_H */