#ifndef RES_B_BH_H
#define RES_B_BH_H

enum dRes_INDEX_B_BH {
    /* BCK  */
    dRes_INDEX_B_BH_BCK_BH_ATTACK_e=0x5,
    dRes_INDEX_B_BH_BCK_BH_ATTACKWAIT_e=0x6,
    dRes_INDEX_B_BH_BCK_BH_DAMAGE_e=0x7,
    dRes_INDEX_B_BH_BCK_BH_EAT_e=0x8,
    dRes_INDEX_B_BH_BCK_BH_EATWAIT_e=0x9,
    dRes_INDEX_B_BH_BCK_BH_NODAMAGE_e=0xA,
    dRes_INDEX_B_BH_BCK_BH_ROUT_e=0xB,
    dRes_INDEX_B_BH_BCK_BH_WAIT_e=0xC,
    dRes_INDEX_B_BH_BCK_BH_WAIT01_e=0xD,
    dRes_INDEX_B_BH_BCK_BH_WAIT02_e=0xE,
    /* BMDR */
    dRes_INDEX_B_BH_BMD_BH_KUKI_e=0x11,
    dRes_INDEX_B_BH_BMD_BH_LEAF_e=0x12,
    /* BMDV */
    dRes_INDEX_B_BH_BMD_BH_e=0x15,
};

enum dRes_ID_B_BH {
    /* BCK  */
    dRes_ID_B_BH_BCK_BH_ATTACK_e=0x5,
    dRes_ID_B_BH_BCK_BH_ATTACKWAIT_e=0x6,
    dRes_ID_B_BH_BCK_BH_DAMAGE_e=0x7,
    dRes_ID_B_BH_BCK_BH_EAT_e=0x8,
    dRes_ID_B_BH_BCK_BH_EATWAIT_e=0x9,
    dRes_ID_B_BH_BCK_BH_NODAMAGE_e=0xA,
    dRes_ID_B_BH_BCK_BH_ROUT_e=0xB,
    dRes_ID_B_BH_BCK_BH_WAIT_e=0xC,
    dRes_ID_B_BH_BCK_BH_WAIT01_e=0xD,
    dRes_ID_B_BH_BCK_BH_WAIT02_e=0xE,
    /* BMDR */
    dRes_ID_B_BH_BMD_BH_KUKI_e=0x11,
    dRes_ID_B_BH_BMD_BH_LEAF_e=0x12,
    /* BMDV */
    dRes_ID_B_BH_BMD_BH_e=0x15,
};

enum BH_KUKI_JNT {
    BH_KUKI_JNT_KUKI_MODEL_e=0x0,
};

enum BH_LEAF_JNT {
    BH_LEAF_JNT_LEAF_MODEL_e=0x0,
};

enum BH_JNT {
    BH_JNT_CENTER_e=0x0,
    BH_JNT_HA1_e=0x1,
    BH_JNT_HA2_e=0x2,
    BH_JNT_HA3_e=0x3,
    BH_JNT_HB1_e=0x4,
    BH_JNT_HB2_e=0x5,
    BH_JNT_HB3_e=0x6,
    BH_JNT_HC1_e=0x7,
    BH_JNT_HC2_e=0x8,
    BH_JNT_HC3_e=0x9,
    BH_JNT_HEAD1_e=0xA,
    BH_JNT_JAWD1_e=0xB,
    BH_JNT_JAWD2_e=0xC,
    BH_JNT_JAWD3_e=0xD,
    BH_JNT_JAWU1_e=0xE,
    BH_JNT_JAWU2_e=0xF,
    BH_JNT_JAWU3_e=0x10,
    BH_JNT_TONGUE1_e=0x11,
    BH_JNT_TONGUE2_e=0x12,
    BH_JNT_TONGUE3_e=0x13,
    BH_JNT_TONGUE4_e=0x14,
    BH_JNT_TONGUE5_e=0x15,
    BH_JNT_TONGUE6_e=0x16,
    BH_JNT_LEAFA_e=0x17,
    BH_JNT_LEAFB_e=0x18,
};

#endif /* !RES_B_BH_H */