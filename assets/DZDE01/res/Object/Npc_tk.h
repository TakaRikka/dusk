#ifndef RES_NPC_TK_H
#define RES_NPC_TK_H

enum dRes_INDEX_NPC_TK {
    /* BCK  */
    dRes_INDEX_NPC_TK_BCK_TK_BREAKING_e=0x5,
    dRes_INDEX_NPC_TK_BCK_TK_FLY_e=0x6,
    dRes_INDEX_NPC_TK_BCK_TK_HOVERING_e=0x7,
    dRes_INDEX_NPC_TK_BCK_TK_KAKKU_e=0x8,
    dRes_INDEX_NPC_TK_BCK_TK_LAND_e=0x9,
    dRes_INDEX_NPC_TK_BCK_TK_PICK_A_e=0xA,
    dRes_INDEX_NPC_TK_BCK_TK_PICK_B_e=0xB,
    dRes_INDEX_NPC_TK_BCK_TK_WAIT_A_e=0xC,
    /* BMDR */
    dRes_INDEX_NPC_TK_BMD_TK_e=0xF,
    /* EVT  */
    dRes_INDEX_NPC_TK_DAT_EVENT_LIST_e=0x12,
};

enum dRes_ID_NPC_TK {
    /* BCK  */
    dRes_ID_NPC_TK_BCK_TK_BREAKING_e=0x5,
    dRes_ID_NPC_TK_BCK_TK_FLY_e=0x6,
    dRes_ID_NPC_TK_BCK_TK_HOVERING_e=0x7,
    dRes_ID_NPC_TK_BCK_TK_KAKKU_e=0x8,
    dRes_ID_NPC_TK_BCK_TK_LAND_e=0x9,
    dRes_ID_NPC_TK_BCK_TK_PICK_A_e=0xA,
    dRes_ID_NPC_TK_BCK_TK_PICK_B_e=0xB,
    dRes_ID_NPC_TK_BCK_TK_WAIT_A_e=0xC,
    /* BMDR */
    dRes_ID_NPC_TK_BMD_TK_e=0xF,
    /* EVT  */
    dRes_ID_NPC_TK_DAT_EVENT_LIST_e=0x12,
};

enum TK_JNT {
    TK_JNT_HARA_e=0x0,
    TK_JNT_ASHIAL_e=0x1,
    TK_JNT_ASHIBL_e=0x2,
    TK_JNT_FOOTL_e=0x3,
    TK_JNT_ASHIAR_e=0x4,
    TK_JNT_ASHIBR_e=0x5,
    TK_JNT_FOOTR_e=0x6,
    TK_JNT_KATAL_e=0x7,
    TK_JNT_KANEAL_e=0x8,
    TK_JNT_HANEBL_e=0x9,
    TK_JNT_HANECL_e=0xA,
    TK_JNT_KATAR_e=0xB,
    TK_JNT_HANEAR_e=0xC,
    TK_JNT_HANEBR_e=0xD,
    TK_JNT_HANECR_e=0xE,
    TK_JNT_KUBIA_e=0xF,
    TK_JNT_HEAD_e=0x10,
    TK_JNT_KUCHI_e=0x11,
    TK_JNT_TK_TUBASA_e=0x12,
};

#endif /* !RES_NPC_TK_H */