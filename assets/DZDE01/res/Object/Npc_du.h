#ifndef RES_NPC_DU_H
#define RES_NPC_DU_H

enum dRes_INDEX_NPC_DU {
    /* BCK  */
    dRes_INDEX_NPC_DU_BCK_DU_SWIM_e=0x5,
    dRes_INDEX_NPC_DU_BCK_DU_WAIT_e=0x6,
    dRes_INDEX_NPC_DU_BCK_DU_WALK_e=0x7,
    /* BMDR */
    dRes_INDEX_NPC_DU_BMD_DU_e=0xA,
    /* BTP  */
    dRes_INDEX_NPC_DU_BTP_DU_e=0xD,
};

enum dRes_ID_NPC_DU {
    /* BCK  */
    dRes_ID_NPC_DU_BCK_DU_SWIM_e=0x5,
    dRes_ID_NPC_DU_BCK_DU_WAIT_e=0x6,
    dRes_ID_NPC_DU_BCK_DU_WALK_e=0x7,
    /* BMDR */
    dRes_ID_NPC_DU_BMD_DU_e=0xA,
    /* BTP  */
    dRes_ID_NPC_DU_BTP_DU_e=0xD,
};

enum DU_JNT {
    DU_JNT_WORLD_ROOT_e=0x0,
    DU_JNT_BACKBONE_e=0x1,
    DU_JNT_ARML_1_e=0x2,
    DU_JNT_ARML_2_e=0x3,
    DU_JNT_ARML_3_e=0x4,
    DU_JNT_ARMR_1_e=0x5,
    DU_JNT_ARMR_2_e=0x6,
    DU_JNT_ARMR_3_e=0x7,
    DU_JNT_NECK1_e=0x8,
    DU_JNT_NECK2_e=0x9,
    DU_JNT_HEAD_e=0xA,
    DU_JNT_JAW_e=0xB,
    DU_JNT_WAIST_e=0xC,
    DU_JNT_LEGL_e=0xD,
    DU_JNT_FOOTL_e=0xE,
    DU_JNT_LEGR1_e=0xF,
    DU_JNT_FOOTR_e=0x10,
    DU_JNT_TAIL_e=0x11,
};

#endif /* !RES_NPC_DU_H */