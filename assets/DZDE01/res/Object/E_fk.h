#ifndef RES_E_FK_H
#define RES_E_FK_H

enum dRes_INDEX_E_FK {
    /* BCK  */
    dRes_INDEX_E_FK_BCK_FK_ATTACK_L_e=0x4,
    dRes_INDEX_E_FK_BCK_FK_ATTACK_R_e=0x5,
    dRes_INDEX_E_FK_BCK_FK_DAMAGE_L_e=0x6,
    dRes_INDEX_E_FK_BCK_FK_DAMAGE_R_e=0x7,
    dRes_INDEX_E_FK_BCK_FK_DIE_1_e=0x8,
    dRes_INDEX_E_FK_BCK_FK_DIE_2_e=0x9,
    dRes_INDEX_E_FK_BCK_FK_RUN_e=0xA,
    dRes_INDEX_E_FK_BCK_FK_SPIN_1_e=0xB,
    dRes_INDEX_E_FK_BCK_FK_SPIN_2_e=0xC,
    /* BMDR */
    dRes_INDEX_E_FK_BMD_FK_e=0xF,
    dRes_INDEX_E_FK_BMD_FK_EFFECT_e=0x10,
};

enum dRes_ID_E_FK {
    /* BCK  */
    dRes_ID_E_FK_BCK_FK_ATTACK_L_e=0x4,
    dRes_ID_E_FK_BCK_FK_ATTACK_R_e=0x5,
    dRes_ID_E_FK_BCK_FK_DAMAGE_L_e=0x6,
    dRes_ID_E_FK_BCK_FK_DAMAGE_R_e=0x7,
    dRes_ID_E_FK_BCK_FK_DIE_1_e=0x8,
    dRes_ID_E_FK_BCK_FK_DIE_2_e=0x9,
    dRes_ID_E_FK_BCK_FK_RUN_e=0xA,
    dRes_ID_E_FK_BCK_FK_SPIN_1_e=0xB,
    dRes_ID_E_FK_BCK_FK_SPIN_2_e=0xC,
    /* BMDR */
    dRes_ID_E_FK_BMD_FK_e=0xF,
    dRes_ID_E_FK_BMD_FK_EFFECT_e=0x10,
};

enum FK_JNT {
    FK_JNT_CENTER_e=0x0,
    FK_JNT_H_NECK_e=0x1,
    FK_JNT_H_HEAD_e=0x2,
    FK_JNT_HIRA_B_1_e=0x3,
    FK_JNT_HIRA_B_2_e=0x4,
    FK_JNT_HIRA_B_3_e=0x5,
    FK_JNT_HIRA_L_1_e=0x6,
    FK_JNT_HIRA_L_2_e=0x7,
    FK_JNT_HIRA_R_1_e=0x8,
    FK_JNT_HIRA_R_2_e=0x9,
    FK_JNT_K_WEAST_e=0xA,
    FK_JNT_K_BACKBONE_e=0xB,
    FK_JNT_K_NECK_e=0xC,
    FK_JNT_K_HEAD_e=0xD,
    FK_JNT_SHOULDER_L_e=0xE,
    FK_JNT_ARM_L_1_e=0xF,
    FK_JNT_ARM_L_2_e=0x10,
    FK_JNT_ARM_L_3_e=0x11,
    FK_JNT_HAND_L_e=0x12,
    FK_JNT_SHOULDER_R_e=0x13,
    FK_JNT_ARM_R_1_e=0x14,
    FK_JNT_ARM_R_2_e=0x15,
    FK_JNT_ARM_R_3_e=0x16,
    FK_JNT_HAND_R_e=0x17,
    FK_JNT_LANCER_e=0x18,
};

enum FK_EFFECT_JNT {
    FK_EFFECT_JNT_FK_EFFECT_e=0x0,
};

#endif /* !RES_E_FK_H */