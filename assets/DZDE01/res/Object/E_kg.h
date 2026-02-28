#ifndef RES_E_KG_H
#define RES_E_KG_H

enum dRes_INDEX_E_KG {
    /* BCK  */
    dRes_INDEX_E_KG_BCK_KG_ATTACK_e=0x5,
    dRes_INDEX_E_KG_BCK_KG_DAMAGE_e=0x6,
    dRes_INDEX_E_KG_BCK_KG_DAMAGEL_e=0x7,
    dRes_INDEX_E_KG_BCK_KG_DAMAGER_e=0x8,
    dRes_INDEX_E_KG_BCK_KG_GUARDWAIT_e=0x9,
    dRes_INDEX_E_KG_BCK_KG_HOLDDAMAGE_e=0xA,
    dRes_INDEX_E_KG_BCK_KG_HOLDWAIT_e=0xB,
    dRes_INDEX_E_KG_BCK_KG_RETURN_e=0xC,
    dRes_INDEX_E_KG_BCK_KG_STEP_e=0xD,
    dRes_INDEX_E_KG_BCK_KG_WAIT01_e=0xE,
    dRes_INDEX_E_KG_BCK_KG_WALK_e=0xF,
    /* BMDR */
    dRes_INDEX_E_KG_BMD_KG_e=0x12,
    /* BTP  */
    dRes_INDEX_E_KG_BTP_KG_MABA_e=0x15,
};

enum dRes_ID_E_KG {
    /* BCK  */
    dRes_ID_E_KG_BCK_KG_ATTACK_e=0x5,
    dRes_ID_E_KG_BCK_KG_DAMAGE_e=0x6,
    dRes_ID_E_KG_BCK_KG_DAMAGEL_e=0x7,
    dRes_ID_E_KG_BCK_KG_DAMAGER_e=0x8,
    dRes_ID_E_KG_BCK_KG_GUARDWAIT_e=0x9,
    dRes_ID_E_KG_BCK_KG_HOLDDAMAGE_e=0xA,
    dRes_ID_E_KG_BCK_KG_HOLDWAIT_e=0xB,
    dRes_ID_E_KG_BCK_KG_RETURN_e=0xC,
    dRes_ID_E_KG_BCK_KG_STEP_e=0xD,
    dRes_ID_E_KG_BCK_KG_WAIT01_e=0xE,
    dRes_ID_E_KG_BCK_KG_WALK_e=0xF,
    /* BMDR */
    dRes_ID_E_KG_BMD_KG_e=0x12,
    /* BTP  */
    dRes_ID_E_KG_BTP_KG_MABA_e=0x15,
};

enum KG_JNT {
    KG_JNT_WORLD_ROOT_e=0x0,
    KG_JNT_HEAD_e=0x1,
    KG_JNT_CHINL_e=0x2,
    KG_JNT_CHINR_e=0x3,
    KG_JNT_HANDL1_e=0x4,
    KG_JNT_HANDL2_e=0x5,
    KG_JNT_HANDL3_e=0x6,
    KG_JNT_HANDR1_e=0x7,
    KG_JNT_HANDR2_e=0x8,
    KG_JNT_HANDR3_e=0x9,
    KG_JNT_LEGL1_e=0xA,
    KG_JNT_LEGL2_e=0xB,
    KG_JNT_LEGL3_e=0xC,
    KG_JNT_LEGR1_e=0xD,
    KG_JNT_LEGR2_e=0xE,
    KG_JNT_LEGR3_e=0xF,
    KG_JNT_WAIST1_e=0x10,
    KG_JNT_WAIST2_e=0x11,
};

#endif /* !RES_E_KG_H */