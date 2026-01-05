/**
 * @file d_a_npc_chat.cpp
 * 
*/

#include "d/dolzel_rel.h"
#include "d/actor/d_a_npc_chat.h"
#include "Z2AudioLib/Z2Instances.h"

enum MAN_a_RES_File_ID {
    /* BMDR */
    /* 0x3 */ BMDR_MAN_A = 0x3,
};

enum MAN_a_TW_RES_File_ID {
    /* BMDR */
    /* 0x3 */ BMDR_MAN_A_TW = 0x3,
};

enum MAD_a_RES_File_ID {
    /* BMDR */
    /* 0x3 */ BMDR_MAD_A = 0x3,
};

enum MAD_a_TW_RES_File_ID {
    /* BMDR */
    /* 0x3 */ BMDR_MAD_A_TW = 0x3,
};

enum MCN_a_RES_File_ID {
    /* BMDR */
    /* 0x4 */ BMDR_MCN_A = 0x4,

    /* BTP */
    /* 0x7 */ BTP_MCN_A = 0x7,
};

enum MCN_a_TW_RES_File_ID {
    /* BMDR */
    /* 0x4 */ BMDR_MCN_A_TW = 0x4,

    /* BTP */
    /* 0x7 */ BTP_MCN_A_TW = 0x7,
};

enum MON_a_RES_File_ID {
    /* BMDR */
    /* 0x4 */ BMDR_MON_A = 0x4,

    /* BTP */
    /* 0x7 */ BTP_MON_A = 0x7,
};

enum MON_a_TW_RES_File_ID {
    /* BMDR */
    /* 0x4 */ BMDR_MON_A_TW = 0x4,

    /* BTP */
    /* 0x7 */ BTP_MON_A_TW = 0x7,
};

enum MAN_b_RES_File_ID {
    /* BMDR */
    /* 0x3 */ BMDR_MAN_B = 0x3,
};

enum MAN_b_TW_RES_File_ID {
    /* BMDR */
    /* 0x3 */ BMDR_MAN_B_TW = 0x3,
};

enum MAN_c_RES_File_ID {
    /* BMDR */
    /* 0x4 */ BMDR_MAN_C = 0x4,

    /* BTP */
    /* 0x7 */ BTP_MAN_C = 0x7,
};

enum MAN_c_TW_RES_File_ID {
    /* BMDR */
    /* 0x4 */ BMDR_MAN_C_TW = 0x4,

    /* BTP */
    /* 0x7 */ BTP_MAN_C_TW = 0x7,
};

enum MAS_a_RES_File_ID {
    /* BMDR */
    /* 0x4 */ BMDR_MAS_A = 0x4,

    /* BTP */
    /* 0x7 */ BTP_MAS_A = 0x7,
};

enum MAS_a_TW_RES_File_ID {
    /* BMDR */
    /* 0x4 */ BMDR_MAS_A_TW = 0x4,

    /* BTP */
    /* 0x7 */ BTP_MAS_A_TW = 0x7,
};

enum MBN_a_RES_File_ID {
    /* BMDR */
    /* 0x4 */ BMDR_MBN_A = 0x4,

    /* BTP */
    /* 0x7 */ BTP_MBN_A = 0x7,
};

enum MBN_a_TW_RES_File_ID {
    /* BMDR */
    /* 0x4 */ BMDR_MBN_A_TW = 0x4,

    /* BTP */
    /* 0x7 */ BTP_MBN_A_TW = 0x7,
};

enum MAN_a2_RES_File_ID {
    /* BMDR */
    /* 0x4 */ BMDR_MAN_A2 = 0x4,

    /* BTP */
    /* 0x7 */ BTP_MAN_A2 = 0x7,
};

enum MAN_a2_TW_RES_File_ID {
    /* BMDR */
    /* 0x4 */ BMDR_MAN_A2_TW = 0x4,

    /* BTP */
    /* 0x7 */ BTP_MAN_A2_TW = 0x7,
};

enum MAD_a2_RES_File_ID {
    /* BMDR */
    /* 0x4 */ BMDR_MAD_A2 = 0x4,

    /* BTP */
    /* 0x7 */ BTP_MAD_A2 = 0x7,
};

enum MAD_a2_TW_RES_File_ID {
    /* BMDR */
    /* 0x3 */ BMDR_MAD_A2_TW = 0x3,
};

enum MCN_a2_RES_File_ID {
    /* BMDR */
    /* 0x4 */ BMDR_MCN_A2 = 0x4,

    /* BTP */
    /* 0x7 */ BTP_MCN_A2 = 0x7,
};

enum MCN_a2_TW_RES_File_ID {
    /* BMDR */
    /* 0x4 */ BMDR_MCN_A2_TW = 0x4,

    /* BTP */
    /* 0x7 */ BTP_MCN_A2_TW = 0x7,
};

enum MON_a2_RES_File_ID {
    /* BMDR */
    /* 0x4 */ BMDR_MON_A2 = 0x4,

    /* BTP */
    /* 0x7 */ BTP_MON_A2 = 0x7,
};

enum MON_a2_TW_RES_File_ID {
    /* BMDR */
    /* 0x4 */ BMDR_MON_A2_TW = 0x4,

    /* BTP */
    /* 0x7 */ BTP_MON_A2_TW = 0x7,
};

enum MAN_b2_RES_File_ID {
    /* BMDR */
    /* 0x3 */ BMDR_MAN_B2 = 0x3,
};

enum MAN_b2_TW_RES_File_ID {
    /* BMDR */
    /* 0x3 */ BMDR_MAN_B2_TW = 0x3,
};

enum MAN_c2_RES_File_ID {
    /* BMDR */
    /* 0x4 */ BMDR_MAN_C2 = 0x4,

    /* BTP */
    /* 0x7 */ BTP_MAN_C2 = 0x7,
};

enum MAN_c2_TW_RES_File_ID {
    /* BMDR */
    /* 0x4 */ BMDR_MAN_C2_TW = 0x4,

    /* BTP */
    /* 0x7 */ BTP_MAN_C2_TW = 0x7,
};

enum MAS_a2_RES_File_ID {
    /* BMDR */
    /* 0x4 */ BMDR_MAS_A2 = 0x4,

    /* BTP */
    /* 0x7 */ BTP_MAS_A2 = 0x7,
};

enum MAS_a2_TW_RES_File_ID {
    /* BMDR */
    /* 0x4 */ BMDR_MAS_A2_TW = 0x4,

    /* BTP */
    /* 0x7 */ BTP_MAS_A2_TW = 0x7,
};

enum MBN_a2_RES_File_ID {
    /* BMDR */
    /* 0x4 */ BMDR_MBN_A2 = 0x4,

    /* BTP */
    /* 0x7 */ BTP_MBN_A2 = 0x7,
};

enum MBN_a2_TW_RES_File_ID {
    /* BMDR */
    /* 0x4 */ BMDR_MBN_A2_TW = 0x4,

    /* BTP */
    /* 0x7 */ BTP_MBN_A2_TW = 0x7,
};

enum WAN_a_RES_File_ID {
    /* BMDR */
    /* 0x4 */ BMDR_WAN_A = 0x4,

    /* BTP */
    /* 0x7 */ BTP_WAN_A = 0x7,
};

enum WAN_a_TW_RES_File_ID {
    /* BMDR */
    /* 0x4 */ BMDR_WAN_A_TW = 0x4,

    /* BTP */
    /* 0x7 */ BTP_WAN_A_TW = 0x7,
};

enum WAD_a_RES_File_ID {
    /* BMDR */
    /* 0x4 */ BMDR_WAD_A = 0x4,

    /* BTP */
    /* 0x7 */ BTP_WAD_A = 0x7,
};

enum WAD_a_TW_RES_File_ID {
    /* BMDR */
    /* 0x4 */ BMDR_WAD_A_TW = 0x4,

    /* BTP */
    /* 0x7 */ BTP_WAD_A_TW = 0x7,
};

enum MAT_a_RES_File_ID {
    /* BMDR */
    /* 0x3 */ BMDR_MAT_A = 0x3,
};

enum MAT_a_TW_RES_File_ID {
    /* BMDR */
    /* 0x3 */ BMDR_MAT_A_TW = 0x3,
};

enum WCN_a_RES_File_ID {
    /* BMDR */
    /* 0x4 */ BMDR_WCN_A = 0x4,

    /* BTP */
    /* 0x7 */ BTP_WCN_A = 0x7,
};

enum WCN_a_TW_RES_File_ID {
    /* BMDR */
    /* 0x4 */ BMDR_WCN_A_TW = 0x4,

    /* BTP */
    /* 0x7 */ BTP_WCN_A_TW = 0x7,
};

enum WON_a_RES_File_ID {
    /* BMDR */
    /* 0x4 */ BMDR_WON_A = 0x4,

    /* BTP */
    /* 0x7 */ BTP_WON_A = 0x7,
};

enum WON_a_TW_RES_File_ID {
    /* BMDR */
    /* 0x4 */ BMDR_WON_A_TW = 0x4,

    /* BTP */
    /* 0x7 */ BTP_WON_A_TW = 0x7,
};

enum WGN_a_RES_File_ID {
    /* BMDR */
    /* 0x4 */ BMDR_WGN_A = 0x4,

    /* BTP */
    /* 0x7 */ BTP_WGN_A = 0x7,
};

enum WGN_a_TW_RES_File_ID {
    /* BMDR */
    /* 0x4 */ BMDR_WGN_A_TW = 0x4,

    /* BTP */
    /* 0x7 */ BTP_WGN_A_TW = 0x7,
};

enum WAN_b_RES_File_ID {
    /* BMDR */
    /* 0x4 */ BMDR_WAN_B = 0x4,

    /* BTP */
    /* 0x7 */ BTP_WAN_B = 0x7,
};

enum WAN_b_TW_RES_File_ID {
    /* BMDR */
    /* 0x4 */ BMDR_WAN_B_TW = 0x4,

    /* BTP */
    /* 0x7 */ BTP_WAN_B_TW = 0x7,
};

enum WAN_a2_RES_File_ID {
    /* BMDR */
    /* 0x4 */ BMDR_WAN_A2 = 0x4,

    /* BTP */
    /* 0x7 */ BTP_WAN_A2 = 0x7,
};

enum WAN_a2_TW_RES_File_ID {
    /* BMDR */
    /* 0x4 */ BMDR_WAN_A2_TW = 0x4,

    /* BTP */
    /* 0x7 */ BTP_WAN_A2_TW = 0x7,
};

enum WAD_a2_RES_File_ID {
    /* BMDR */
    /* 0x4 */ BMDR_WAD_A2 = 0x4,

    /* BTP */
    /* 0x7 */ BTP_WAD_A2 = 0x7,
};

enum WAD_a2_TW_RES_File_ID {
    /* BMDR */
    /* 0x4 */ BMDR_WAD_A2_TW = 0x4,

    /* BTP */
    /* 0x7 */ BTP_WAD_A2_TW = 0x7,
};

enum MAT_a2_RES_File_ID {
    /* BMDR */
    /* 0x3 */ BMDR_MAT_A2 = 0x3,
};

enum MAT_a2_TW_RES_File_ID {
    /* BMDR */
    /* 0x3 */ BMDR_MAT_A2_TW = 0x3,
};

enum WCN_a2_RES_File_ID {
    /* BMDR */
    /* 0x4 */ BMDR_WCN_A2 = 0x4,

    /* BTP */
    /* 0x7 */ BTP_WCN_A2 = 0x7,
};

enum WCN_a2_TW_RES_File_ID {
    /* BMDR */
    /* 0x4 */ BMDR_WCN_A2_TW = 0x4,

    /* BTP */
    /* 0x7 */ BTP_WCN_A2_TW = 0x7,
};

enum WON_a2_RES_File_ID {
    /* BMDR */
    /* 0x4 */ BMDR_WON_A2 = 0x4,

    /* BTP */
    /* 0x7 */ BTP_WON_A2 = 0x7,
};

enum WON_a2_TW_RES_File_ID {
    /* BMDR */
    /* 0x4 */ BMDR_WON_A2_TW = 0x4,

    /* BTP */
    /* 0x7 */ BTP_WON_A2_TW = 0x7,
};

enum WGN_a2_RES_File_ID {
    /* BMDR */
    /* 0x4 */ BMDR_WGN_A2 = 0x4,

    /* BTP */
    /* 0x7 */ BTP_WGN_A2 = 0x7,
};

enum WGN_a2_TW_RES_File_ID {
    /* BMDR */
    /* 0x4 */ BMDR_WGN_A2_TW = 0x4,

    /* BTP */
    /* 0x7 */ BTP_WGN_A2_TW = 0x7,
};

enum WAN_b2_RES_File_ID {
    /* BMDR */
    /* 0x4 */ BMDR_WAN_B2 = 0x4,

    /* BTP */
    /* 0x7 */ BTP_WAN_B2 = 0x7,
};

enum WAN_b2_TW_RES_File_ID {
    /* BMDR */
    /* 0x4 */ BMDR_WAN_B2_TW = 0x4,

    /* BTP */
    /* 0x7 */ BTP_WAN_B2_TW = 0x7,
};

enum object_RES_File_ID {
    /* BMDR */
    /* 0x03 */ BMDR_B_BROOM = 0x3,
    /* 0x04 */ BMDR_B_CHAIR_M,
    /* 0x05 */ BMDR_B_CHAIR_W,
    /* 0x06 */ BMDR_B_FLUTE,
    /* 0x07 */ BMDR_B_MILKTUBO,
    /* 0x08 */ BMDR_B_SPEAR,
    /* 0x09 */ BMDR_B_TUBO,
    /* 0x0A */ BMDR_CELEB_BAG_M,
    /* 0x0B */ BMDR_CELEB_BAG_W,
    /* 0x0C */ BMDR_J_GUITAR,
    /* 0x0D */ BMDR_K_KAGOM,
    /* 0x0E */ BMDR_K_KAGOW,
    /* 0x0F */ BMDR_MARO_BAG_M,
    /* 0x10 */ BMDR_MARO_BAG_W,
    /* 0x11 */ BMDR_SOL_SHE,
};

enum objectTW_RES_File_ID {
    /* BMDR */
    /* 0x03 */ BMDR_B_BROOM_TW = 0x3,
    /* 0x04 */ BMDR_B_CHAIR_M_TW,
    /* 0x05 */ BMDR_B_CHAIR_W_TW,
    /* 0x06 */ BMDR_B_FLUTE_TW,
    /* 0x07 */ BMDR_B_MILKTUBO_TW,
    /* 0x08 */ BMDR_B_SPEAR_TW,
    /* 0x09 */ BMDR_B_TUBO_TW,
    /* 0x0A */ BMDR_CELEB_BAG_M_TW,
    /* 0x0B */ BMDR_CELEB_BAG_W_TW,
    /* 0x0C */ BMDR_J_GUITAR_TW,
    /* 0x0D */ BMDR_K_KAGOM_TW,
    /* 0x0E */ BMDR_K_KAGOW_TW,
};

enum Mgeneral_RES_File_ID {
    /* BCK */
    /* 0x03 */ BCK_M_BROWSE_A = 0x3,
    /* 0x04 */ BCK_M_BROWSE_B,
    /* 0x05 */ BCK_M_LOOK_A,
    /* 0x06 */ BCK_M_LOOK_B,
    /* 0x07 */ BCK_M_MARO_DANCE,
    /* 0x08 */ BCK_M_RUN_A,
    /* 0x09 */ BCK_M_RUN_B,
    /* 0x0A */ BCK_M_SING,
    /* 0x0B */ BCK_M_SIT_TO_WOLF_A,
    /* 0x0C */ BCK_M_SIT_TO_WOLF_B,
    /* 0x0D */ BCK_M_SITTALK_A,
    /* 0x0E */ BCK_M_SITTALK_A_B,
    /* 0x0F */ BCK_M_SITTALK_B,
    /* 0x10 */ BCK_M_SITTALK_B_B,
    /* 0x11 */ BCK_M_SITWAIT_A,
    /* 0x12 */ BCK_M_SITWAIT_B,
    /* 0x13 */ BCK_M_SURPRISE,
    /* 0x14 */ BCK_M_TALK_A,
    /* 0x15 */ BCK_M_TALK_B,
    /* 0x16 */ BCK_M_TALK_B_WALL,
    /* 0x17 */ BCK_M_TALK_C,
    /* 0x18 */ BCK_M_TALK_WALL,
    /* 0x19 */ BCK_M_TO_WOLF,
    /* 0x1A */ BCK_M_WAIT_A,
    /* 0x1B */ BCK_M_WAIT_B,
    /* 0x1C */ BCK_M_WAIT_WALL,
    /* 0x1D */ BCK_M_WALK_A,
    /* 0x1E */ BCK_M_WALK_B,
};

enum Mspecial_RES_File_ID {
    /* BCK */
    /* 0x03 */ BCK_M_KAMAE = 0x3,
    /* 0x04 */ BCK_M_KAMAE_C,
    /* 0x05 */ BCK_M_KAMAE_STEP,
    /* 0x06 */ BCK_M_KAMAE_WAIT_B,
    /* 0x07 */ BCK_M_PLAYM,
    /* 0x08 */ BCK_M_RUN_KAGO,
    /* 0x09 */ BCK_M_RUN_KAMAE,
    /* 0x0A */ BCK_M_RUN_STICK,
    /* 0x0B */ BCK_M_RUN_TUBO,
    /* 0x0C */ BCK_M_TALK_B_KAGO,
    /* 0x0D */ BCK_M_TALK_B_STICK,
    /* 0x0E */ BCK_M_TALK_B_TUBO,
    /* 0x0F */ BCK_M_TALK_KAGO,
    /* 0x10 */ BCK_M_TALK_STICK,
    /* 0x11 */ BCK_M_TALK_TUBO,
    /* 0x12 */ BCK_M_WAIT_KAGO,
    /* 0x13 */ BCK_M_WAIT_STICK,
    /* 0x14 */ BCK_M_WAIT_TUBO,
    /* 0x15 */ BCK_M_WALK_KAGO,
    /* 0x16 */ BCK_M_WALK_STICK,
    /* 0x17 */ BCK_M_WALK_TUBO,
};

enum Wgeneral_RES_File_ID {
    /* BCK */
    /* 0x03 */ BCK_W_2LADYTALK_A = 0x3,
    /* 0x04 */ BCK_W_2LADYTALK_B,
    /* 0x05 */ BCK_W_2NORMALTALK_A,
    /* 0x06 */ BCK_W_2NORMALTALK_B,
    /* 0x07 */ BCK_W_BROWSE_A,
    /* 0x08 */ BCK_W_BROWSE_B,
    /* 0x09 */ BCK_W_CELLME,
    /* 0x0A */ BCK_W_LOOK_A,
    /* 0x0B */ BCK_W_LOOK_B,
    /* 0x0C */ BCK_W_MARO_DANCE,
    /* 0x0D */ BCK_W_RUN_A,
    /* 0x0E */ BCK_W_RUN_B,
    /* 0x0F */ BCK_W_SING,
    /* 0x10 */ BCK_W_SIT_TO_WOLF_A,
    /* 0x11 */ BCK_W_SIT_TO_WOLF_B,
    /* 0x12 */ BCK_W_SITTALK_A,
    /* 0x13 */ BCK_W_SITTALK_A_B,
    /* 0x14 */ BCK_W_SITTALK_B,
    /* 0x15 */ BCK_W_SITTALK_B_B,
    /* 0x16 */ BCK_W_SITWAIT_A,
    /* 0x17 */ BCK_W_SITWAIT_B,
    /* 0x18 */ BCK_W_SURPRISE,
    /* 0x19 */ BCK_W_TALK_A,
    /* 0x1A */ BCK_W_TALK_B,
    /* 0x1B */ BCK_W_TALK_B_WALL,
    /* 0x1C */ BCK_W_TALK_C,
    /* 0x1D */ BCK_W_TALK_WALL,
    /* 0x1E */ BCK_W_TO_WOLF,
    /* 0x1F */ BCK_W_WAIT_A,
    /* 0x20 */ BCK_W_WAIT_B,
    /* 0x21 */ BCK_W_WAIT_WALL,
    /* 0x22 */ BCK_W_WALK_A,
    /* 0x23 */ BCK_W_WALK_B,
};

enum Wspecial_RES_File_ID {
    /* 0x03 */ BCK_W_PLAYM = 0x3,
    /* 0x04 */ BCK_W_RUN_KAGO,
    /* 0x05 */ BCK_W_RUN_STICK,
    /* 0x06 */ BCK_W_RUN_TUBO,
    /* 0x07 */ BCK_W_TALK_B_KAGO,
    /* 0x08 */ BCK_W_TALK_B_STICK,
    /* 0x09 */ BCK_W_TALK_B_TUBO,
    /* 0x0A */ BCK_W_TALK_KAGO,
    /* 0x0B */ BCK_W_TALK_STICK,
    /* 0x0C */ BCK_W_TALK_TUBO,
    /* 0x0D */ BCK_W_WAIT_KAGO,
    /* 0x0E */ BCK_W_WAIT_STICK,
    /* 0x0F */ BCK_W_WAIT_TUBO,
    /* 0x10 */ BCK_W_WALK_KAGO,
    /* 0x11 */ BCK_W_WALK_STICK,
    /* 0x12 */ BCK_W_WALK_TUBO,
};

enum Animation {
    /* 0x00 */ ANM_WAIT_A,
    /* 0x01 */ ANM_WAIT_B,
    /* 0x02 */ ANM_WALK_A,
    /* 0x03 */ ANM_WALK_B,
    /* 0x04 */ ANM_RUN_A,
    /* 0x05 */ ANM_RUN_B,
    /* 0x06 */ ANM_TALK_A,
    /* 0x07 */ ANM_TALK_B,
    /* 0x08 */ ANM_TALK_C,
    /* 0x09 */ ANM_LOOK_A,
    /* 0x0A */ ANM_LOOK_B,
    /* 0x0B */ ANM_WAIT_WALL,
    /* 0x0C */ ANM_TALK_WALL,
    /* 0x0D */ ANM_TALK_B_WALL,
    /* 0x0E */ ANM_SITWAIT_A,
    /* 0x0F */ ANM_SITTALK_A,
    /* 0x10 */ ANM_SITTALK_A_B,
    /* 0x11 */ ANM_SITWAIT_B,
    /* 0x12 */ ANM_SITTALK_B,
    /* 0x13 */ ANM_SITTALK_B_B,
    /* 0x14 */ ANM_BROWSE_A,
    /* 0x15 */ ANM_BROWSE_B,
    /* 0x16 */ ANM_WAIT_KAGO,
    /* 0x17 */ ANM_TALK_KAGO,
    /* 0x18 */ ANM_TALK_B_KAGO,
    /* 0x19 */ ANM_WALK_KAGO,
    /* 0x1A */ ANM_WAIT_STICK,
    /* 0x1B */ ANM_TALK_STICK,
    /* 0x1C */ ANM_TALK_B_STICK,
    /* 0x1D */ ANM_WALK_STICK,
    /* 0x1E */ ANM_WAIT_TUBO,
    /* 0x1F */ ANM_TALK_TUBO,
    /* 0x20 */ ANM_TALK_B_TUBO,
    /* 0x21 */ ANM_WALK_TUBO,
    /* 0x22 */ ANM_PLAYM,
    /* 0x23 */ ANM_RUN_KAGO,
    /* 0x24 */ ANM_RUN_STICK,
    /* 0x25 */ ANM_RUN_TUBO,
    /* 0x26 */ ANM_TO_WOLF,
    /* 0x27 */ ANM_SIT_TO_WOLF_A,
    /* 0x28 */ ANM_SIT_TO_WOLF_B,
    /* 0x29 */ ANM_SURPRISE,
    /* 0x2A */ ANM_KAMAE,
    /* 0x2B */ ANM_KAMAE_WAIT_B,
    /* 0x2C */ ANM_KAMAE_STEP,
    /* 0x2D */ ANM_RUN_KAMAE,
    /* 0x2E */ ANM_KAMAE_C,
    /* 0x2F */ ANM_MARO_DANCE,
    /* 0x30 */ ANM_2LADYTALK_A,
    /* 0x31 */ ANM_2LADYTALK_B,
    /* 0x32 */ ANM_2NORMALTALK_A,
    /* 0x33 */ ANM_2NORMALTALK_B,
    /* 0x34 */ ANM_CELLME,
    /* 0x35 */ ANM_SING,
};

enum Motion {
    /* 0x00 */ MOT_TALK_A,
    /* 0x01 */ MOT_WAIT_A,
    /* 0x02 */ MOT_WAIT_B,
    /* 0x04 */ MOT_TALK_B = 0x4,
    /* 0x05 */ MOT_TALK_A_B_A,
    /* 0x06 */ MOT_TALK_C,
    /* 0x07 */ MOT_TALK_B_A_C,
    /* 0x08 */ MOT_TALK_A_B_C,
    /* 0x09 */ MOT_BROWSE,
    /* 0x0A */ MOT_BROWSE_TALK_A_B,
    /* 0x0B */ MOT_BROWSE_TALK_A_B_C,
    /* 0x0C */ MOT_2LADYTALK_A,
    /* 0x0D */ MOT_2LADYTALK_B,
    /* 0x0E */ MOT_TALK_A_2LADYTALK_A,
    /* 0x0F */ MOT_WAIT_A_2LADYTALK_B,
    /* 0x10 */ MOT_TALK_A_2LADYTALK_A_2NORMALTALK_A,
    /* 0x11 */ MOT_WAIT_A_2LADYTALK_B_2NORMALTALK_B,
    /* 0x12 */ MOT_2LADYTALK_A_B_2NORMALTALK_A_B,
    /* 0x13 */ MOT_LOOK_A,
    /* 0x14 */ MOT_LOOK_B,
    /* 0x15 */ MOT_LOOK_A_B,
    /* 0x16 */ MOT_WAIT_WALL,
    /* 0x17 */ MOT_TALK_WALL,
    /* 0x18 */ MOT_SITWAIT_A,
    /* 0x19 */ MOT_SITTALK_SITWAIT_A,
    /* 0x1A */ MOT_SITWAIT_B,
    /* 0x1B */ MOT_SITTALK_SITWAIT_B,
    /* 0x1C */ MOT_SING,
    /* 0x1D */ MOT_SITTALK_A,
    /* 0x1E */ MOT_SITTALK_A_B,
    /* 0x1F */ MOT_SITTALK_A_A_B,
    /* 0x20 */ MOT_SITTALK_B,
    /* 0x21 */ MOT_SITTALK_B_B,
    /* 0x22 */ MOT_SITTALK_B_B_B,
    /* 0x23 */ MOT_KAMAE,
    /* 0x24 */ MOT_KAMAE_C,
    /* 0x25 */ MOT_KAMAE_STEP,
    /* 0x26 */ MOT_SURPRISE,
    /* 0x27 */ MOT_TO_WOLF,
};

enum Object {
    /* 0x00 */ OBJ_TUBO,
    /* 0x01 */ OBJ_TUBO2,
    /* 0x02 */ OBJ_SPEAR,
    /* 0x03 */ OBJ_BROOM,
    /* 0x04 */ OBJ_KAGOM,
    /* 0x05 */ OBJ_KAGOW,
    /* 0x06 */ OBJ_GUITAR,
    /* 0x07 */ OBJ_FLUTE,
    /* 0x08 */ OBJ_MILKTUBO,
    /* 0x09 */ OBJ_CELEB_BAG_M,
    /* 0x0A */ OBJ_CELEB_BAG_W,
    /* 0x0B */ OBJ_MARO_BAG_M,
    /* 0x0C */ OBJ_MARO_BAG_W,
};

enum Joint_NUM {
    /* 0x13 */ JntM_NUM_e = 0x13,
    /* 0x15 */ JntW_NUM_e = 0x15,
};

enum Joint_M {
    /* 0x00 */ JNTM_CENTER,
    /* 0x01 */ JNTM_BACKBONE,
    /* 0x02 */ JNTM_NECK,
    /* 0x03 */ JNTM_HEAD,
    /* 0x04 */ JNTM_SHOULDERL,
    /* 0x05 */ JNTM_ARML1,
    /* 0x06 */ JNTM_ARML2,
    /* 0x07 */ JNTM_HANDL,
    /* 0x08 */ JNTM_SHOULDERR,
    /* 0x09 */ JNTM_ARMR1,
    /* 0x0A */ JNTM_ARMR2,
    /* 0x0B */ JNTM_HANDR,
    /* 0x0C */ JNTM_WAIST,
    /* 0x0D */ JNTM_LEGL1,
    /* 0x0E */ JNTM_LEGL2,
    /* 0x0F */ JNTM_FOOTL,
    /* 0x10 */ JNTM_LEGR1,
    /* 0x11 */ JNTM_LEGR2,
    /* 0x12 */ JNTM_FOOTR,
};

enum Joint_W {
    /* 0x00 */ JNTW_CENTER,
    /* 0x01 */ JNTW_BACKBONE,
    /* 0x02 */ JNTW_NECK,
    /* 0x03 */ JNTW_HEAD,
    /* 0x04 */ JNTW_SHOULDERL,
    /* 0x05 */ JNTW_ARML1,
    /* 0x06 */ JNTW_ARML2,
    /* 0x07 */ JNTW_HANDL,
    /* 0x08 */ JNTW_SHOULDERR,
    /* 0x09 */ JNTW_ARMR1,
    /* 0x0A */ JNTW_ARMR2,
    /* 0x0B */ JNTW_HANDR,
    /* 0x0C */ JNTW_WAIST,
    /* 0x0D */ JNTW_LEGL1,
    /* 0x0E */ JNTW_LEGL2,
    /* 0x0F */ JNTW_FOOTL,
    /* 0x10 */ JNTW_SKIRTL,
    /* 0x11 */ JNTW_LEGR1,
    /* 0x12 */ JNTW_LEGR2,
    /* 0x13 */ JNTW_FOOTR,
    /* 0x14 */ JNTW_SKIRTR,
};

enum Type {
    /* 0x00 */ MdlMANa_e,
    /* 0x01 */ MdlMADa_e,
    /* 0x02 */ MdlMCNa_e,
    /* 0x03 */ MdlMONa_e,
    /* 0x04 */ MdlMANb_e,
    /* 0x05 */ MdlMANc_e,
    /* 0x06 */ MdlMASa_e,
    /* 0x07 */ MdlMBNa_e,
    /* 0x08 */ MdlMANa2_e,
    /* 0x09 */ MdlMADa2_e,
    /* 0x0A */ MdlMCNa2_e,
    /* 0x0B */ MdlMONa2_e,
    /* 0x0C */ MdlMANb2_e,
    /* 0x0D */ MdlMANc2_e,
    /* 0x0E */ MdlMASa2_e,
    /* 0x0F */ MdlMBNa2_e,
    /* 0x10 */ MdlWANa_e,
    /* 0x11 */ MdlWADa_e,
    /* 0x12 */ MdlMATa_e,
    /* 0x13 */ MdlWCNa_e,
    /* 0x14 */ MdlWONa_e,
    /* 0x15 */ MdlWGNa_e,
    /* 0x16 */ MdlWANb_e,
    /* 0x17 */ MdlWANa2_e,
    /* 0x18 */ MdlWADa2_e,
    /* 0x19 */ MdlMATa2_e,
    /* 0x1A */ MdlWCNa2_e,
    /* 0x1B */ MdlWONa2_e,
    /* 0x1C */ MdlWGNa2_e,
    /* 0x1D */ MdlWANb2_e,
};

static char* const l_resMANa[6] = {
    "MAN_a",
    "MAN_a_TW",
    "Mgeneral",
    "Mspecial",
    "object",
    "objectTW",
};

static char* const l_resMADa[6] = {
    "MAD_a",
    "MAD_a_TW",
    "Mgeneral",
    "Mspecial",
    "object",
    "objectTW",
};

static char* const l_resMCNa[6] = {
    "MCN_a",
    "MCN_a_TW",
    "Mgeneral",
    "Mspecial",
    "object",
    "objectTW",
};

static char* const l_resMONa[6] = {
    "MON_a",
    "MON_a_TW",
    "Mgeneral",
    "Mspecial",
    "object",
    "objectTW",
};

static char* const l_resMANb[6] = {
    "MAN_b",
    "MAN_b_TW",
    "Mgeneral",
    "Mspecial",
    "object",
    "objectTW",
};

static char* const l_resMANc[6] = {
    "MAN_c",
    "MAN_c_TW",
    "Mgeneral",
    "Mspecial",
    "object",
    "objectTW",
};

static char* const l_resMASa[6] = {
    "MAS_a",
    "MAS_a_TW",
    "Mgeneral",
    "Mspecial",
    "object",
    "objectTW",
};

static char* const l_resMBNa[6] = {
    "MBN_a",
    "MBN_a_TW",
    "Mgeneral",
    "Mspecial",
    "object",
    "objectTW",
};

static char* const l_resMANa2[6] = {
    "MAN_a2",
    "MAN_a2_TW",
    "Mgeneral",
    "Mspecial",
    "object",
    "objectTW",
};

static char* const l_resMADa2[6] = {
    "MAD_a2",
    "MAD_a2_TW",
    "Mgeneral",
    "Mspecial",
    "object",
    "objectTW",
};

static char* const l_resMCNa2[6] = {
    "MCN_a2",
    "MCN_a2_TW",
    "Mgeneral",
    "Mspecial",
    "object",
    "objectTW",
};

static char* const l_resMONa2[6] = {
    "MON_a2",
    "MON_a2_TW",
    "Mgeneral",
    "Mspecial",
    "object",
    "objectTW",
};

static char* const l_resMANb2[6] = {
    "MAN_b2",
    "MAN_b2_TW",
    "Mgeneral",
    "Mspecial",
    "object",
    "objectTW",
};

static char* const l_resMANc2[6] = {
    "MAN_c2",
    "MAN_c2_TW",
    "Mgeneral",
    "Mspecial",
    "object",
    "objectTW",
};

static char* const l_resMASa2[6] = {
    "MAS_a2",
    "MAS_a2_TW",
    "Mgeneral",
    "Mspecial",
    "object",
    "objectTW",
};

static char* const l_resMBNa2[6] = {
    "MBN_a2",
    "MBN_a2_TW",
    "Mgeneral",
    "Mspecial",
    "object",
    "objectTW",
};

static char* const l_resWANa[6] = {
    "WAN_a",
    "WAN_a_TW",
    "Wgeneral",
    "Wspecial",
    "object",
    "objectTW",
};

static char* const l_resWADa[6] = {
    "WAD_a",
    "WAD_a_TW",
    "Wgeneral",
    "Wspecial",
    "object",
    "objectTW",
};

static char* const l_resMATa[6] = {
    "MAT_a",
    "MAT_a_TW",
    "Wgeneral",
    "Wspecial",
    "object",
    "objectTW",
};

static char* const l_resWCNa[6] = {
    "WCN_a",
    "WCN_a_TW",
    "Wgeneral",
    "Wspecial",
    "object",
    "objectTW",
};

static char* const l_resWONa[6] = {
    "WON_a",
    "WON_a_TW",
    "Wgeneral",
    "Wspecial",
    "object",
    "objectTW",
};

static char* const l_resWGNa[6] = {
    "WGN_a",
    "WGN_a_TW",
    "Wgeneral",
    "Wspecial",
    "object",
    "objectTW",
};

static char* const l_resWANb[6] = {
    "WAN_b",
    "WAN_b_TW",
    "Wgeneral",
    "Wspecial",
    "object",
    "objectTW",
};

static char* const l_resWANa2[6] = {
    "WAN_a2",
    "WAN_a2_TW",
    "Wgeneral",
    "Wspecial",
    "object",
    "objectTW",
};

static char* const l_resWADa2[6] = {
    "WAD_a2",
    "WAD_a2_TW",
    "Wgeneral",
    "Wspecial",
    "object",
    "objectTW",
};

static char* const l_resMATa2[6] = {
    "MAT_a2",
    "MAT_a2_TW",
    "Wgeneral",
    "Wspecial",
    "object",
    "objectTW",
};

static char* const l_resWCNa2[6] = {
    "WCN_a2",
    "WCN_a2_TW",
    "Wgeneral",
    "Wspecial",
    "object",
    "objectTW",
};

static char* const l_resWONa2[6] = {
    "WON_a2",
    "WON_a2_TW",
    "Wgeneral",
    "Wspecial",
    "object",
    "objectTW",
};

static char* const l_resWGNa2[6] = {
    "WGN_a2",
    "WGN_a2_TW",
    "Wgeneral",
    "Wspecial",
    "object",
    "objectTW",
};

static char* const l_resWANb2[6] = {
    "WAN_b2",
    "WAN_b2_TW",
    "Wgeneral",
    "Wspecial",
    "object",
    "objectTW",
};

static char* const* l_resNameTbl[30] = {
    l_resMANa,  l_resMADa,  l_resMCNa,  l_resMONa,
    l_resMANb,  l_resMANc,  l_resMASa,  l_resMBNa,
    l_resMANa2, l_resMADa2, l_resMCNa2, l_resMONa2,
    l_resMANb2, l_resMANc2, l_resMASa2, l_resMBNa2,
    l_resWANa,  l_resWADa,  l_resMATa,  l_resWCNa,
    l_resWONa,  l_resWGNa,  l_resWANb,  l_resWANa2,
    l_resWADa2, l_resMATa2, l_resWCNa2, l_resWONa2,
    l_resWGNa2, l_resWANb2,
};

struct anmTblPrm {
    char* arc_name;
    int resource_index;
};

 static anmTblPrm l_bmdTbl[30] = {
    l_resMANa[0], BMDR_MAN_A,
    l_resMADa[0], BMDR_MAD_A,
    l_resMCNa[0], BMDR_MCN_A,
    l_resMONa[0], BMDR_MON_A,
    l_resMANb[0], BMDR_MAN_B,
    l_resMANc[0], BMDR_MAN_C,
    l_resMASa[0], BMDR_MAS_A,
    l_resMBNa[0], BMDR_MBN_A,
    l_resMANa2[0], BMDR_MAN_A2,
    l_resMADa2[0], BMDR_MAD_A2,
    l_resMCNa2[0], BMDR_MCN_A2,
    l_resMONa2[0], BMDR_MON_A2,
    l_resMANb2[0], BMDR_MAN_B2,
    l_resMANc2[0], BMDR_MAN_C2,
    l_resMASa2[0], BMDR_MAS_A2,
    l_resMBNa2[0], BMDR_MBN_A2,
    l_resWANa[0], BMDR_WAN_A,
    l_resWADa[0], BMDR_WAD_A,
    l_resMATa[0], BMDR_MAT_A,
    l_resWCNa[0], BMDR_WCN_A,
    l_resWONa[0], BMDR_WON_A,
    l_resWGNa[0], BMDR_WGN_A,
    l_resWANb[0], BMDR_WAN_B,
    l_resWANa2[0], BMDR_WAN_A2,
    l_resWADa2[0], BMDR_WAD_A2,
    l_resMATa2[0], BMDR_MAT_A2,
    l_resWCNa2[0], BMDR_WCN_A2,
    l_resWONa2[0], BMDR_WON_A2,
    l_resWGNa2[0], BMDR_WGN_A2,
    l_resWANb2[0], BMDR_WAN_B2,
};

static anmTblPrm l_bmdTWTbl[30] = {
    l_resMANa[1], BMDR_MAN_A_TW,
    l_resMADa[1], BMDR_MAD_A_TW,
    l_resMCNa[1], BMDR_MCN_A_TW,
    l_resMONa[1], BMDR_MON_A_TW,
    l_resMANb[1], BMDR_MAN_B_TW,
    l_resMANc[1], BMDR_MAN_C_TW,
    l_resMASa[1], BMDR_MAS_A_TW,
    l_resMBNa[1], BMDR_MBN_A_TW,
    l_resMANa2[1], BMDR_MAN_A2_TW,
    l_resMADa2[1], BMDR_MAD_A2_TW,
    l_resMCNa2[1], BMDR_MCN_A2_TW,
    l_resMONa2[1], BMDR_MON_A2_TW,
    l_resMANb2[1], BMDR_MAN_B2_TW,
    l_resMANc2[1], BMDR_MAN_C2_TW,
    l_resMASa2[1], BMDR_MAS_A2_TW,
    l_resMBNa2[1], BMDR_MBN_A2_TW,
    l_resWANa[1], BMDR_WAN_A_TW,
    l_resWADa[1], BMDR_WAD_A_TW,
    l_resMATa[1], BMDR_MAT_A_TW,
    l_resWCNa[1], BMDR_WCN_A_TW,
    l_resWONa[1], BMDR_WON_A_TW,
    l_resWGNa[1], BMDR_WGN_A_TW,
    l_resWANb[1], BMDR_WAN_B_TW,
    l_resWANa2[1], BMDR_WAN_A2_TW,
    l_resWADa2[1], BMDR_WAD_A2_TW,
    l_resMATa2[1], BMDR_MAT_A2_TW,
    l_resWCNa2[1], BMDR_WCN_A2_TW,
    l_resWONa2[1], BMDR_WON_A2_TW,
    l_resWGNa2[1], BMDR_WGN_A2_TW,
    l_resWANb2[1], BMDR_WAN_B2_TW,
};

static anmTblPrm l_btpTbl[30] = {
    l_resMANa[0], -1,
    l_resMADa[0], -1,
    l_resMCNa[0], BTP_MCN_A,
    l_resMONa[0], BTP_MON_A,
    l_resMANb[0], -1,
    l_resMANc[0], BTP_MAN_C,
    l_resMASa[0], BTP_MAS_A,
    l_resMBNa[0], BTP_MBN_A,
    l_resMANa2[0], BTP_MAN_A2,
    l_resMADa2[0], BTP_MAD_A2,
    l_resMCNa2[0], BTP_MCN_A2,
    l_resMONa2[0], BTP_MON_A2,
    l_resMANb2[0], -1,
    l_resMANc2[0], BTP_MAN_C2,
    l_resMASa2[0], BTP_MAS_A2,
    l_resMBNa2[0], BTP_MBN_A2,
    l_resWANa[0], BTP_WAN_A,
    l_resWADa[0], BTP_WAD_A,
    l_resMATa[0], -1,
    l_resWCNa[0], BTP_WCN_A,
    l_resWONa[0], BTP_WON_A,
    l_resWGNa[0], BTP_WGN_A,
    l_resWANb[0], BTP_WAN_B,
    l_resWANa2[0], BTP_WAN_A2,
    l_resWADa2[0], BTP_WAD_A2,
    l_resMATa2[0], -1,
    l_resWCNa2[0], BTP_WCN_A2,
    l_resWONa2[0], BTP_WON_A2,
    l_resWGNa2[0], BTP_WGN_A2,
    l_resWANb2[0], BTP_WAN_B2,
};

static anmTblPrm l_btpTWTbl[30] = {
    l_resMANa[1], -1,
    l_resMADa[1], -1,
    l_resMCNa[1], BTP_MCN_A_TW,
    l_resMONa[1], BTP_MON_A_TW,
    l_resMANb[1], -1,
    l_resMANc[1], BTP_MAN_C_TW,
    l_resMASa[1], BTP_MAS_A_TW,
    l_resMBNa[1], BTP_MBN_A_TW,
    l_resMANa2[1], BTP_MAN_A2_TW,
    l_resMADa2[1], -1,
    l_resMCNa2[1], BTP_MCN_A2_TW,
    l_resMONa2[1], BTP_MON_A2_TW,
    l_resMANb2[1], -1,
    l_resMANc2[1], BTP_MAN_C2_TW,
    l_resMASa2[1], BTP_MAS_A2_TW,
    l_resMBNa2[1], BTP_MBN_A2_TW,
    l_resWANa[1], BTP_WAN_A_TW,
    l_resWADa[1], BTP_WAD_A_TW,
    l_resMATa[1], -1,
    l_resWCNa[1], BTP_WCN_A_TW,
    l_resWONa[1], BTP_WON_A_TW,
    l_resWGNa[1], BTP_WGN_A_TW,
    l_resWANb[1], BTP_WAN_B_TW,
    l_resWANa2[1], BTP_WAN_A2_TW,
    l_resWADa2[1], BTP_WAD_A2_TW,
    l_resMATa2[1], -1,
    l_resWCNa2[1], BTP_WCN_A2_TW,
    l_resWONa2[1], BTP_WON_A2_TW,
    l_resWGNa2[1], BTP_WGN_A2_TW,
    l_resWANb2[1], BTP_WAN_B2_TW,
};

static anmTblPrm l_bckTbl_M[54] = {
    {"Mgeneral",  BCK_M_WAIT_A},
    {"Mgeneral",  BCK_M_WAIT_B},
    {"Mgeneral",  BCK_M_WALK_A},
    {"Mgeneral",  BCK_M_WALK_B},
    {"Mgeneral",  BCK_M_RUN_A},
    {"Mgeneral",  BCK_M_RUN_B},
    {"Mgeneral",  BCK_M_TALK_A},
    {"Mgeneral",  BCK_M_TALK_B},
    {"Mgeneral",  BCK_M_TALK_C},
    {"Mgeneral",  BCK_M_LOOK_A},
    {"Mgeneral",  BCK_M_LOOK_B},
    {"Mgeneral",  BCK_M_WAIT_WALL},
    {"Mgeneral",  BCK_M_TALK_WALL},
    {"Mgeneral",  BCK_M_TALK_B_WALL},
    {"Mgeneral",  BCK_M_SITWAIT_A},
    {"Mgeneral",  BCK_M_SITTALK_A},
    {"Mgeneral",  BCK_M_SITTALK_A_B},
    {"Mgeneral",  BCK_M_SITWAIT_B},
    {"Mgeneral",  BCK_M_SITTALK_B},
    {"Mgeneral",  BCK_M_SITTALK_B_B},
    {"Mgeneral",  BCK_M_BROWSE_A},
    {"Mgeneral",  BCK_M_BROWSE_B},
    {"Mspecial", BCK_M_WAIT_KAGO},
    {"Mspecial", BCK_M_TALK_KAGO},
    {"Mspecial", BCK_M_TALK_B_KAGO},
    {"Mspecial", BCK_M_WALK_KAGO},
    {"Mspecial", BCK_M_WAIT_STICK},
    {"Mspecial", BCK_M_TALK_STICK},
    {"Mspecial", BCK_M_TALK_B_STICK},
    {"Mspecial", BCK_M_WALK_STICK},
    {"Mspecial", BCK_M_WAIT_TUBO},
    {"Mspecial", BCK_M_TALK_TUBO},
    {"Mspecial", BCK_M_TALK_B_TUBO},
    {"Mspecial", BCK_M_WALK_TUBO},
    {"Mspecial", BCK_M_PLAYM},
    {"Mspecial", BCK_M_RUN_KAGO},
    {"Mspecial", BCK_M_RUN_STICK},
    {"Mspecial", BCK_M_RUN_TUBO},
    {"Mgeneral",  BCK_M_TO_WOLF},
    {"Mgeneral",  BCK_M_SIT_TO_WOLF_A},
    {"Mgeneral",  BCK_M_SIT_TO_WOLF_B},
    {"Mgeneral",  BCK_M_SURPRISE},
    {"Mspecial", BCK_M_KAMAE},
    {"Mspecial", BCK_M_KAMAE_WAIT_B},
    {"Mspecial", BCK_M_KAMAE_STEP},
    {"Mspecial", BCK_M_RUN_KAMAE},
    {"Mspecial", BCK_M_KAMAE_C},
    {"Mgeneral",  BCK_M_MARO_DANCE},
    {"Mgeneral",  -1},
    {"Mgeneral",  -1},
    {"Mgeneral",  -1},
    {"Mgeneral",  -1},
    {"Mgeneral",  -1},
    {"Mgeneral",  BCK_M_SING},
};

static anmTblPrm l_bckTbl_W[54] = {
    {"Wgeneral", BCK_W_WAIT_A},
    {"Wgeneral", BCK_W_WAIT_B},
    {"Wgeneral", BCK_W_WALK_A},
    {"Wgeneral", BCK_W_WALK_B},
    {"Wgeneral", BCK_W_RUN_A},
    {"Wgeneral", BCK_W_RUN_B},
    {"Wgeneral", BCK_W_TALK_A},
    {"Wgeneral", BCK_W_TALK_B},
    {"Wgeneral", BCK_W_TALK_C},
    {"Wgeneral", BCK_W_LOOK_A},
    {"Wgeneral", BCK_W_LOOK_B},
    {"Wgeneral", BCK_W_WAIT_WALL},
    {"Wgeneral", BCK_W_TALK_WALL},
    {"Wgeneral", BCK_W_TALK_B_WALL},
    {"Wgeneral", BCK_W_SITWAIT_A},
    {"Wgeneral", BCK_W_SITTALK_A},
    {"Wgeneral", BCK_W_SITTALK_A_B},
    {"Wgeneral", BCK_W_SITWAIT_B},
    {"Wgeneral", BCK_W_SITTALK_B},
    {"Wgeneral", BCK_W_SITTALK_B_B},
    {"Wgeneral", BCK_W_BROWSE_A},
    {"Wgeneral", BCK_W_BROWSE_B},
    {"Wspecial", BCK_W_WAIT_KAGO},
    {"Wspecial", BCK_W_TALK_KAGO},
    {"Wspecial", BCK_W_TALK_B_KAGO},
    {"Wspecial", BCK_W_WALK_KAGO},
    {"Wspecial", BCK_W_WAIT_STICK},
    {"Wspecial", BCK_W_TALK_STICK},
    {"Wspecial", BCK_W_TALK_B_STICK},
    {"Wspecial", BCK_W_WALK_STICK},
    {"Wspecial", BCK_W_WAIT_TUBO},
    {"Wspecial", BCK_W_TALK_TUBO},
    {"Wspecial", BCK_W_TALK_B_TUBO},
    {"Wspecial", BCK_W_WALK_TUBO},
    {"Wspecial", BCK_W_PLAYM},
    {"Wspecial", BCK_W_RUN_KAGO},
    {"Wspecial", BCK_W_RUN_STICK},
    {"Wspecial", BCK_W_RUN_TUBO},
    {"Wgeneral", BCK_W_TO_WOLF},
    {"Wgeneral", BCK_W_SIT_TO_WOLF_A},
    {"Wgeneral", BCK_W_SIT_TO_WOLF_B},
    {"Wgeneral", BCK_W_SURPRISE},
    {"Wspecial", -1},
    {"Wspecial", -1},
    {"Wspecial", -1},
    {"Wspecial", -1},
    {"Wspecial", -1},
    {"Wgeneral", BCK_W_MARO_DANCE},
    {"Wgeneral", BCK_W_2LADYTALK_A},
    {"Wgeneral", BCK_W_2LADYTALK_B},
    {"Wgeneral", BCK_W_2NORMALTALK_A},
    {"Wgeneral", BCK_W_2NORMALTALK_B},
    {"Wgeneral", BCK_W_CELLME},
    {"Wgeneral", BCK_W_SING},
};

struct jnt {
    /* 0x00 */ f32 jntT[3];
    /* 0X0C */ s16 jntR[3];
    /* 0x12 */ u8 field_0x12;
    /* 0x13 */ u8 field_0x13;
};

struct jntTblMPrm {
    /* 0x0 */ jnt jntPrm[19];
};

struct jntTblWPrm {
    /* 0x0 */ jnt jntPrm[21];
};

static jntTblMPrm a_jntTbl_M[16] = {
    {
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    },

    {
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0x14, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0x14, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0xB, 0, -0x0012, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    },

    {
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    },
    
    {
    0.0f, -0.3f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, -0x0014, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, -0x0032, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, -0x0041, 0, 0,
    0.0f, 0.0f, 0.0f, 0, -0x000A, 5, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0x14, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0x46, 0xA, 0x1E, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0x32, -0x0005, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0xA, 5, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0x14, 0, 0, 0,
    0.0f, 0.0f, 0.0f, -0x0046, 0xA, -0x001E, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0x32, 5, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0xF, 0, 0,
    0.0f, 0.0f, 0.0f, 0xA, 0, -0x001E, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0xA, 0xB, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 1, 0, 0,
    0.0f, 0.0f, 0.0f, 0xA, 0, -0x001E, 0, 0,
    0.0f, 0.0f, 0.0f, 0, -0x000A, 0xB, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 1, 0, 0,
    },
    
    {
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    },
    
    {
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    },
    
    {
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0xA, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0xA, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    },
    
    {
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    },
    
    {
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    },
    
    {
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0x14, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0x14, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0xB, 0, -0x0012, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    },
    
    {
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    },
    
    {
    0.0f, -0.3f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, -0x0014, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, -0x0032, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, -0x0041, 0, 0,
    0.0f, 0.0f, 0.0f, 0, -0x000A, 5, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0x14, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0x46, 0xA, 0x1E, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0x32, -0x0005, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0xA, 5, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0x14, 0, 0, 0,
    0.0f, 0.0f, 0.0f, -0x0046, 0xA, -0x001E, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0x32, 5, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0xF, 0, 0,
    0.0f, 0.0f, 0.0f, 0xA, 0, -0x001E, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0xA, 0xB, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 1, 0, 0,
    0.0f, 0.0f, 0.0f, 0xA, 0, -0x001E, 0, 0,
    0.0f, 0.0f, 0.0f, 0, -0x000A, 0xB, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 1, 0, 0,
    },
    
    {
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    },
    
    {
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    },
    
    {
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0xA, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0xA, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    },
    
    {
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    },
};

struct prmTbl {
    /* 0x00 */ f32 field_0x0;
    /* 0x04 */ u8 field_0x4[0x18 - 0x4];
    /* 0x18 */ f32 field_0x18;
    /* 0x1C */ s16 field_0x1c;
    /* 0x1E */ u8 field_0x1e[0x20 - 0x1e];
    /* 0x20 */ f32 field_0x20;
    /* 0x24 */ f32 field_0x24;
    /* 0x28 */ f32 field_0x28;
    /* 0x2C */ f32 field_0x2c;
    /* 0x30 */ f32 field_0x30;
    /* 0x34 */ f32 field_0x34;
    /* 0x38 */ f32 field_0x38;
    /* 0x3C */ f32 field_0x3c;
};

static prmTbl a_prmTbl_M[16] = {
    -852593.0f, 0xB9, 0xB0, 0xD8, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x0F, 0xA0, 0x3A, 0x98, 0xE0, 0xC0, 0xC5, 0x68, 1.0f, 0xC8, 0x00, 0x00,
    1.5f, 1.8f, 16.0f, 6.0f, 180.0f, 30.0f, 50.1f, 35.0f,

    -852593.0f, 0xB9, 0xB0, 0xD8, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x0F, 0xA0, 0x3A, 0x98, 0xE0, 0xC0, 0xC5, 0x68, 1.4f, 0xF0, 0x00, 0x00,
    1.4f, 1.8f, 14.0f, 6.0f, 180.0f, 30.0f, 50.1f, 35.0f,

    -852593.0f, 0xB9, 0xB0, 0xD8, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x0F, 0xA0, 0x3A, 0x98, 0xE0, 0xC0, 0xC5, 0x68, 0.7f, 0x8C, 0x00, 0x00,
    1.7f, 2.0f, 12.0f, 4.5f, 180.0f, 30.0f, 50.1f, 35.0f,

    -852593.0f, 0xB9, 0xB0, 0xD8, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x0F, 0xA0, 0x3A, 0x98, 0xE0, 0xC0, 0xC5, 0x68, 0.9f, 0xC8, 0x00, 0x00,
    1.3f, 1.2f, 12.0f, 3.7f, 180.0f, 30.0f, 50.1f, 35.0f,

    -852593.0f, 0xB9, 0xB0, 0xD8, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x0F, 0xA0, 0x3A, 0x98, 0xE0, 0xC0, 0xC5, 0x68, 1.0f, 0xC8, 0x00, 0x00,
    1.5f, 1.5f, 16.0f, 5.5f, 180.0f, 30.0f, 50.1f, 35.0f,

    -852593.0f, 0xB9, 0xB0, 0xD8, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x0F, 0xA0, 0x3A, 0x98, 0xE0, 0xC0, 0xC5, 0x68, 1.0f, 0xC8, 0x00, 0x00,
    1.5f, 1.5f, 16.0f, 5.5f, 180.0f, 30.0f, 50.1f, 35.0f,

    -852593.0f, 0xB9, 0xB0, 0xD8, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x0F, 0xA0, 0x3A, 0x98, 0xE0, 0xC0, 0xC5, 0x68, 1.0f, 0xC8, 0x00, 0x00,
    1.5f, 1.8f, 15.0f, 5.0f, 180.0f, 30.0f, 50.1f, 35.0f,

    -852593.0f, 0xB9, 0xB0, 0xD8, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x0F, 0xA0, 0x3A, 0x98, 0xE0, 0xC0, 0xC5, 0x68, 1.0f, 0xC8, 0x00, 0x00,
    1.5f, 1.8f, 16.0f, 6.0f, 180.0f, 30.0f, 50.1f, 35.0f,

    -852593.0f, 0xB9, 0xB0, 0xD8, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x0F, 0xA0, 0x3A, 0x98, 0xE0, 0xC0, 0xC5, 0x68, 1.0f, 0xC8, 0x00, 0x00,
    1.5f, 1.8f, 16.0f, 6.0f, 180.0f, 30.0f, 50.1f, 35.0f,

    -852593.0f, 0xB9, 0xB0, 0xD8, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x0F, 0xA0, 0x3A, 0x98, 0xE0, 0xC0, 0xC5, 0x68, 1.4f, 0xF0, 0x00, 0x00,
    1.4f, 1.8f, 14.0f, 6.0f, 180.0f, 30.0f, 50.1f, 35.0f,

    -852593.0f, 0xB9, 0xB0, 0xD8, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x0F, 0xA0, 0x3A, 0x98, 0xE0, 0xC0, 0xC5, 0x68, 0.7f, 0x8C, 0x00, 0x00,
    1.7f, 2.0f, 12.0f, 4.5f, 180.0f, 30.0f, 50.1f, 35.0f,

    -852593.0f, 0xB9, 0xB0, 0xD8, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x0F, 0xA0, 0x3A, 0x98, 0xE0, 0xC0, 0xC5, 0x68, 0.9f, 0xC8, 0x00, 0x00,
    1.3f, 1.2f, 12.0f, 3.7f, 180.0f, 30.0f, 50.1f, 35.0f,

    -852593.0f, 0xB9, 0xB0, 0xD8, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x0F, 0xA0, 0x3A, 0x98, 0xE0, 0xC0, 0xC5, 0x68, 1.0f, 0xC8, 0x00, 0x00,
    1.5f, 1.5f, 16.0f, 5.5f, 180.0f, 30.0f, 50.1f, 35.0f,

    -852593.0f, 0xB9, 0xB0, 0xD8, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x0F, 0xA0, 0x3A, 0x98, 0xE0, 0xC0, 0xC5, 0x68, 1.0f, 0xC8, 0x00, 0x00,
    1.5f, 1.5f, 16.0f, 5.5f, 180.0f, 30.0f, 50.1f, 35.0f,

    -852593.0f, 0xB9, 0xB0, 0xD8, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x0F, 0xA0, 0x3A, 0x98, 0xE0, 0xC0, 0xC5, 0x68, 1.0f, 0xC8, 0x00, 0x00,
    1.5f, 1.8f, 15.0f, 5.0f, 180.0f, 30.0f, 50.1f, 35.0f,

    -852593.0f, 0xB9, 0xB0, 0xD8, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x0F, 0xA0, 0x3A, 0x98, 0xE0, 0xC0, 0xC5, 0x68, 1.0f, 0xC8, 0x00, 0x00,
    1.5f, 1.8f, 16.0f, 6.0f, 180.0f, 30.0f, 50.1f, 35.0f,
};

static jntTblWPrm a_jntTbl_W[14] = {
    {
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    },

    {
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0x14, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0x14, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0xB, 8, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    },
    
    {
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    },
    
    {
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0xA, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0xA, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    },
    
    {
    0.0f, -0.3f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, -0x0014, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, -0x0032, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, -0x0041, 0, 0,
    0.0f, 0.0f, 0.0f, 0, -0x000A, 5, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0xA, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 8, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0xA, 5, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 8, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0xF, 0, 0,
    0.0f, 0.0f, 0.0f, 0xA, 0, -0x001E, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0xA, 0xB, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 1, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0xA, 0, -0x001E, 0, 0,
    0.0f, 0.0f, 0.0f, 0, -0x000A, 0xB, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 1, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    },
    
    {
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    },
    
    {
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    },
    
    {
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    },
    
    {
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0x14, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0x14, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0xB, 8, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    },
    
    {
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    },
    
    {
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0xA, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0xA, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    },
    
    {
    0.0f, -0.3f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, -0x0014, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, -0x0032, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, -0x0041, 0, 0,
    0.0f, 0.0f, 0.0f, 0, -0x000A, 5, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0xA, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 8, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0xA, 5, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 8, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0xF, 0, 0,
    0.0f, 0.0f, 0.0f, 0xA, 0, -0x01E, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0xA, 0xB, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 1, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0xA, 0, -0x001E, 0, 0,
    0.0f, 0.0f, 0.0f, 0, -0x000A, 0xB, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 1, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    },
    
    {
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    },
    
    {
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0,
    },
};

static prmTbl a_prmTbl_W[14] = {
    -852593.0f, 0xB9, 0xB0, 0xD8, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x0F, 0xA0, 0x3A, 0x98, 0xE0, 0xC0, 0xC5, 0x68, 1.0f, 0xC8, 0x00, 0x00,
    1.4f, 1.5f, 10.0f, 4.0f, 180.0f, 30.0f, 50.1f, 35.0f,

    -852593.0f, 0xB9, 0xB0, 0xD8, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x0F, 0xA0, 0x3A, 0x98, 0xE0, 0xC0, 0xC5, 0x68, 1.0f, 0xC8, 0x00, 0x00,
    1.5f, 1.5f, 10.0f, 3.5f, 180.0f, 30.0f, 50.1f, 35.0f,

    -852593.0f, 0xB9, 0xB0, 0xD8, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x0F, 0xA0, 0x3A, 0x98, 0xE0, 0xC0, 0xC5, 0x68, 1.0f, 0xC8, 0x00, 0x00,
    1.3f, 1.2f, 10.0f, 3.5f, 180.0f, 30.0f, 50.1f, 35.0f,

    -852593.0f, 0xB9, 0xB0, 0xD8, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x0F, 0xA0, 0x3A, 0x98, 0xE0, 0xC0, 0xC5, 0x68, 0.6f, 0xC8, 0x00, 0x00,
    1.8f, 1.5f, 9.0f, 3.2f, 180.0f, 30.0f, 50.1f, 35.0f,

    -852593.0f, 0xB9, 0xB0, 0xD8, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x0F, 0xA0, 0x3A, 0x98, 0xE0, 0xC0, 0xC5, 0x68, 0.8f, 0xC8, 0x00, 0x00,
    1.3f, 1.2f, 8.0f, 2.8f, 180.0f, 30.0f, 50.1f, 35.0f,

    -852593.0f, 0xB9, 0xB0, 0xD8, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x0F, 0xA0, 0x3A, 0x98, 0xE0, 0xC0, 0xC5, 0x68, 1.0f, 0xC8, 0x00, 0x00,
    1.4f, 1.3f, 10.0f, 2.6f, 180.0f, 30.0f, 50.1f, 35.0f,

    -852593.0f, 0xB9, 0xB0, 0xD8, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x0F, 0xA0, 0x3A, 0x98, 0xE0, 0xC0, 0xC5, 0x68, 1.0f, 0xC8, 0x00, 0x00,
    1.5f, 1.6f, 10.0f, 4.4f, 180.0f, 30.0f, 50.1f, 35.0f,

    -852593.0f, 0xB9, 0xB0, 0xD8, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x0F, 0xA0, 0x3A, 0x98, 0xE0, 0xC0, 0xC5, 0x68, 1.0f, 0xC8, 0x00, 0x00,
    1.4f, 1.5f, 10.0f, 4.0f, 180.0f, 30.0f, 50.1f, 35.0f,

    -852593.0f, 0xB9, 0xB0, 0xD8, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x0F, 0xA0, 0x3A, 0x98, 0xE0, 0xC0, 0xC5, 0x68, 1.0f, 0xC8, 0x00, 0x00,
    1.5f, 1.5f, 10.0f, 3.5f, 180.0f, 30.0f, 50.1f, 35.0f,

    -852593.0f, 0xB9, 0xB0, 0xD8, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x0F, 0xA0, 0x3A, 0x98, 0xE0, 0xC0, 0xC5, 0x68, 1.0f, 0xC8, 0x00, 0x00,
    1.3f, 1.2f, 10.0f, 3.5f, 180.0f, 30.0f, 50.1f, 35.0f,

    -852593.0f, 0xB9, 0xB0, 0xD8, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x0F, 0xA0, 0x3A, 0x98, 0xE0, 0xC0, 0xC5, 0x68, 0.6f, 0xC8, 0x00, 0x00,
    1.8f, 1.5f, 9.0f, 3.2f, 180.0f, 30.0f, 50.1f, 35.0f,

    -852593.0f, 0xB9, 0xB0, 0xD8, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x0F, 0xA0, 0x3A, 0x98, 0xE0, 0xC0, 0xC5, 0x68, 0.8f, 0xC8, 0x00, 0x00,
    1.3f, 1.2f, 8.0f, 2.8f, 180.0f, 30.0f, 50.1f, 35.0f,

    -852593.0f, 0xB9, 0xB0, 0xD8, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x0F, 0xA0, 0x3A, 0x98, 0xE0, 0xC0, 0xC5, 0x68, 1.0f, 0xC8, 0x00, 0x00,
    1.4f, 1.3f, 10.0f, 2.6f, 180.0f, 30.0f, 50.1f, 35.0f,

    -852593.0f, 0xB9, 0xB0, 0xD8, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x0F, 0xA0, 0x3A, 0x98, 0xE0, 0xC0, 0xC5, 0x68, 1.0f, 0xC8, 0x00, 0x00,
    1.5f, 1.6f, 10.0f, 4.4f, 180.0f, 30.0f, 50.1f, 35.0f,
};

static char* l_evtNames[1] = {
    NULL,
};

static char* l_myName = "Chat";

static daNpcChat_Param_c l_HIO;

daNpcChat_c::eventFunc daNpcChat_c::mEvtSeqList[1] = {
    NULL,
};

daNpcChat_c::daNpcChat_c() {}

daNpcChat_c::~daNpcChat_c() {
    removeResrc(mType, mObjNum);

    if (heap != NULL) {
        mAnm_p->stopZelAnime();
    }
}

static anmTblPrm const l_objTbl[13] = {
    {"object", BMDR_B_TUBO},
    {"object", BMDR_B_TUBO},
    {"object", BMDR_B_SPEAR},
    {"object", BMDR_B_BROOM},
    {"object", BMDR_K_KAGOM},
    {"object", BMDR_K_KAGOW},
    {"object", BMDR_J_GUITAR},
    {"object", BMDR_B_FLUTE},
    {"object", BMDR_B_MILKTUBO},
    {"object", BMDR_CELEB_BAG_M},
    {"object", BMDR_CELEB_BAG_W},
    {"object", BMDR_MARO_BAG_M},
    {"object", BMDR_MARO_BAG_W},
};

static anmTblPrm const l_objTWTbl[13] = {
    {"objectTW", BMDR_B_TUBO_TW},
    {"objectTW", BMDR_B_TUBO_TW},
    {"objectTW", BMDR_B_SPEAR_TW},
    {"objectTW", BMDR_B_BROOM_TW},
    {"objectTW", BMDR_K_KAGOM_TW},
    {"objectTW", BMDR_K_KAGOW_TW},
    {"objectTW", BMDR_J_GUITAR_TW},
    {"objectTW", BMDR_B_FLUTE_TW},
    {"objectTW", BMDR_B_MILKTUBO_TW},
    {"objectTW", BMDR_CELEB_BAG_M_TW},
    {"objectTW", BMDR_CELEB_BAG_W_TW},
    {"objectTW", -1},
    {"objectTW", -1},
};

daNpcChat_HIOParam const daNpcChat_Param_c::m = {
    55.0f,
    -3.0f,
    1.0f,
    500.0f,
    255.0f,
    140.0f,
    35.0f,
    30.0f,
    0.0f,
    0.0f,
    10.0f,
    -10.0f,
    30.0f,
    -10.0f,
    45.0f,
    -45.0f,
    0.6f,
    12.0f,
    2,
    6,
    3,
    6,
    60.0f,
    500.0f,
    300.0f,
    -300.0f,
    60,
    0,
    0,
    0,
    0,
    false,
    false,
};

BOOL daNpcChat_c::NpcCreate(int type) {
    J3DModelData* a_mdlData_p = getNpcMdlDataP(type);

    JUT_ASSERT(185, NULL != a_mdlData_p);

    J3DAnmTexPattern* texAnmP = getTexAnmP(type);
    u32 uVar1 = texAnmP != NULL ? 0x11020084 : 0x11000084;

    mAnm_p = new mDoExt_McaMorfSO(a_mdlData_p, NULL, NULL, NULL, -1, 1.0f, 0, -1, &mSound, 0x80000, uVar1);
    if (mAnm_p != NULL && mAnm_p->getModel() == NULL) {
        mAnm_p->stopZelAnime();
        mAnm_p = NULL;
    }

    if (mAnm_p == NULL) {
        return FALSE;
    }

    static Vec const a_transScaleTbl[30] = {
        1.0f, 1.0f, 1.0f,
        1.0f, 1.1983438f, 2.173357f,
        1.0f, 0.5581154f, 1.0f,
        1.0f, 0.95f, 1.0f,
        1.0f, 1.1642542f, 1.0f,
        1.0f, 1.0820773f, 1.0f,
        1.0f, 0.78816104f, 1.0f,
        1.0f, 0.91999996f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.1983438f, 2.173357f,
        1.0f, 0.5581154f, 1.0f,
        1.0f, 0.95f, 1.0f,
        1.0f, 1.1642542f, 1.0f,
        1.0f, 1.0820773f, 1.0f,
        1.0f, 0.78816104f, 1.0f,
        1.0f, 0.91999996f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 0.95f, 1.0f,
        1.0f, 1.0999999f, 1.0f,
        1.0f, 0.5656486f, 1.0f,
        1.0f, 0.9f, 1.0f,
        1.0f, 0.8630768f, 1.0f,
        1.0f, 1.0529536f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 0.95f, 1.0f,
        1.0f, 1.0999999f, 1.0f,
        1.0f, 0.5656486f, 1.0f,
        1.0f, 0.9f, 1.0f,
        1.0f, 0.8630768f, 1.0f,
        1.0f, 1.0529536f, 1.0f,
    };

    cXyz i_scale(a_transScaleTbl[type]);
    mAnm_p->offTranslate();
    mAnm_p->setTranslateScale(i_scale);
    J3DJointCallBack jointCallBack = a_mdlData_p->getJointNodePointer(0)->getCallBack();
    uintptr_t userArea = mAnm_p->getModel()->getUserArea();
    for (u16 i = 0; i < a_mdlData_p->getJointNum(); i++) {
        a_mdlData_p->getJointNodePointer(i)->setCallBack(ctrlJointCallBack);
    }
    mAnm_p->getModel()->setUserArea((uintptr_t)this);

    if (!setExpressionBtp(type)) {
        return FALSE;
    }

    setMotionAnm(0, 0.0f);

    for (u16 i = 0; i < a_mdlData_p->getJointNum(); i++) {
        a_mdlData_p->getJointNodePointer(i)->setCallBack(jointCallBack);
    }
    mAnm_p->getModel()->setUserArea(userArea);

    return TRUE;
}

int daNpcChat_c::getObjNum() {
    int objNum;
    u32 param = (fopAcM_GetParam(this) >> 8) & 0xF;

    if (isM_()) {
        switch (param) {
            case 0:
                objNum = OBJ_TUBO;
                break;

            case 1:
                objNum = OBJ_TUBO2;
                break;

            case 2:
                objNum = OBJ_SPEAR;
                break;

            case 3:
                objNum = OBJ_KAGOM;
                break;

            case 4:
                objNum = OBJ_GUITAR;
                break;

            case 5:
                objNum = OBJ_TUBO;
                break;

            case 6:
                objNum = OBJ_TUBO;
                break;

            case 7:
                objNum = OBJ_CELEB_BAG_M;
                break;

            default:
                objNum = OBJ_TUBO;
                break;
        }

        // dSv_event_flag_c::F_0281 - Shop - Malo Mart opens in Castle Town
        if (dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[281]) &&
            objNum == OBJ_CELEB_BAG_M) {
            objNum = OBJ_MARO_BAG_M;
        }
    } else {
        switch (param) {
            case 0:
                objNum = OBJ_TUBO;
                break;

            case 1:
                objNum = OBJ_MILKTUBO;
                break;

            case 2:
                objNum = OBJ_BROOM;
                break;

            case 3:
                objNum = OBJ_KAGOW;
                break;

            case 4:
                objNum = OBJ_FLUTE;
                break;

            case 5:
                objNum = OBJ_TUBO;
                break;

            case 6:
                objNum = OBJ_TUBO;
                break;

            case 7:
                objNum = OBJ_CELEB_BAG_W;
                break;

            default:
                objNum = OBJ_TUBO;
                break;
        }

        // dSv_event_flag_c::F_0281 - Shop - Malo Mart opens in Castle Town
        if (dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[281]) &&
            objNum == OBJ_CELEB_BAG_W) {
            objNum = OBJ_MARO_BAG_W;
        }
    }

    return objNum;
}

J3DModel* daNpcChat_c::ObjCreate(int param_1) {
    J3DModelData* a_mdlData_p = getObjMdlDataP(param_1);
    if (a_mdlData_p != NULL) {
        return mDoExt_J3DModel__create(a_mdlData_p, 0x80000, 0x11000084);
    }

    return NULL;
}

J3DModel* daNpcChat_c::ChairCreate(f32 i_scale) {
    const char* i_arcName = mTwilight ? "objectTW" : "object";
    int i_index = isM_() ? ((mTwilight != 0) ? 4 : 4) : ((mTwilight != 0) ? 5 : 5);
    
    J3DModelData* a_mdlData_p = (J3DModelData*)dComIfG_getObjectRes(i_arcName, i_index);
    if (a_mdlData_p != NULL) {
        J3DModel* model = mDoExt_J3DModel__create(a_mdlData_p, 0x80000, 0x11000084);
        if (model == NULL) {
            return NULL;
        }

        Vec scale = {i_scale, i_scale, i_scale};
        mDoMtx_stack_c::transS(current.pos);
        mDoMtx_stack_c::ZXYrotM(shape_angle);
        model->setBaseScale(scale);
        model->setBaseTRMtx(mDoMtx_stack_c::get());

        return model;
    }
    
    return NULL;
}

bool daNpcChat_c::isM_() {
    if (mAnm_p == NULL) {
        return mType < 16;
    }

    u16 a_jntNum = mAnm_p->getModel()->getModelData()->getJointNum();
    JUT_ASSERT(738, (a_jntNum == JntM_NUM_e) || (a_jntNum == JntW_NUM_e));
    return a_jntNum == JntM_NUM_e;
}

cPhs__Step daNpcChat_c::loadResrc(int idx, int param_2) {
    cPhs__Step rv = cPhs_COMPLEATE_e;
    if (mTwilight) {
        rv = (cPhs__Step)dComIfG_resLoad(&mPhase1, l_resNameTbl[idx][1]);
        if (rv != cPhs_COMPLEATE_e) {
            return rv;
        }
    } else {
        rv = (cPhs__Step)dComIfG_resLoad(&mPhase1, l_resNameTbl[idx][0]);
        if (rv != cPhs_COMPLEATE_e) {
            return rv;
        }
    }

    rv = (cPhs__Step)dComIfG_resLoad(&mPhase2, l_resNameTbl[idx][2]);
    if (rv != cPhs_COMPLEATE_e) {
        return rv;
    }
    
    if (param_2 != 0 || field_0xe51 == 1) {
        rv = (cPhs__Step)dComIfG_resLoad(&mPhase3, l_resNameTbl[idx][3]);
        if (rv != cPhs_COMPLEATE_e) {
            return rv;
        }

        if (mTwilight) {
            rv = (cPhs__Step)dComIfG_resLoad(&mPhase4, l_resNameTbl[idx][5]);
            if (rv != cPhs_COMPLEATE_e) {
                return rv;
            }
        } else {
            rv = (cPhs__Step)dComIfG_resLoad(&mPhase4, l_resNameTbl[idx][4]);
            if (rv != cPhs_COMPLEATE_e) {
                return rv;
            }
        }
    }

    return rv;
}

J3DModelData* daNpcChat_c::getNpcMdlDataP(int i_index) {
    if (i_index < 0 || i_index >= 30) {
        return NULL;
    }

    J3DModelData* model_data;
    const anmTblPrm* def = mTwilight ? &l_bmdTWTbl[i_index] : &l_bmdTbl[i_index];
    model_data = (J3DModelData*)dComIfG_getObjectRes(def->arc_name, def->resource_index);
    return model_data;
}

J3DModelData* daNpcChat_c::getObjMdlDataP(int i_index) {
    J3DModelData* model_data = NULL;
    const anmTblPrm* def = mTwilight ? &l_objTWTbl[i_index] : &l_objTbl[i_index];

    if (def->resource_index > 0) {
        model_data = (J3DModelData*)dComIfG_getObjectRes(def->arc_name, def->resource_index);
    }

    return model_data;
}

J3DAnmTexPattern* daNpcChat_c::getTexAnmP(int i_index) {
    const anmTblPrm* def;
    if (mTwilight) {
        def = &l_btpTWTbl[i_index];
    } else {
        def = &l_btpTbl[i_index];
    }
    if (def->resource_index != -1) {
        return (J3DAnmTexPattern*)dComIfG_getObjectRes(def->arc_name, def->resource_index);
    }
    return NULL;
}

BOOL daNpcChat_c::removeResrc(int idx, int param_2) {
    if (mTwilight) {
        dComIfG_resDelete(&mPhase1, l_resNameTbl[idx][1]);
    } else {
        dComIfG_resDelete(&mPhase1, l_resNameTbl[idx][0]);
    }

    dComIfG_resDelete(&mPhase2, l_resNameTbl[idx][2]);

    if (param_2 != 0 || field_0xe51 == 1) {
        dComIfG_resDelete(&mPhase3, l_resNameTbl[idx][3]);

        if (mTwilight) {
            dComIfG_resDelete(&mPhase4, l_resNameTbl[idx][5]);
        } else {
            dComIfG_resDelete(&mPhase4, l_resNameTbl[idx][4]);
        }
    }

    return TRUE;
}

inline f32 AtnOfs(int param_1) { return param_1 < 16 ? a_prmTbl_M[param_1].field_0x1c : a_prmTbl_W[param_1 - 16].field_0x1c; }

BOOL daNpcChat_c::setAttention(int param_1) {
    static cXyz a_eyeOfsTbl[30] = {
        cXyz(0.0f, 10.0f, 0.0f),
        cXyz(0.0f, 10.0f, 0.0f),
        cXyz(0.0f, 10.0f, 0.0f),
        cXyz(0.0f, 10.0f, 0.0f),
        cXyz(0.0f, 10.0f, 0.0f),
        cXyz(0.0f, 10.0f, 0.0f),
        cXyz(0.0f, 10.0f, 0.0f),
        cXyz(0.0f, 10.0f, 0.0f),
        cXyz(0.0f, 10.0f, 0.0f),
        cXyz(0.0f, 10.0f, 0.0f),
        cXyz(0.0f, 10.0f, 0.0f),
        cXyz(0.0f, 10.0f, 0.0f),
        cXyz(0.0f, 10.0f, 0.0f),
        cXyz(0.0f, 10.0f, 0.0f),
        cXyz(0.0f, 10.0f, 0.0f),
        cXyz(0.0f, 10.0f, 0.0f),
        cXyz(0.0f, 10.0f, 0.0f),
        cXyz(0.0f, 10.0f, 0.0f),
        cXyz(0.0f, 10.0f, 0.0f),
        cXyz(0.0f, 10.0f, 0.0f),
        cXyz(0.0f, 10.0f, 0.0f),
        cXyz(0.0f, 10.0f, 0.0f),
        cXyz(0.0f, 10.0f, 0.0f),
        cXyz(0.0f, 10.0f, 0.0f),
        cXyz(0.0f, 10.0f, 0.0f),
        cXyz(0.0f, 10.0f, 0.0f),
        cXyz(0.0f, 10.0f, 0.0f),
        cXyz(0.0f, 10.0f, 0.0f),
        cXyz(0.0f, 10.0f, 0.0f),
        cXyz(0.0f, 10.0f, 0.0f),
    };

    int jointNo = isM_() ? JNTM_HEAD : JNTW_HEAD;
    mDoMtx_stack_c::copy(mAnm_p->getModel()->getAnmMtx(jointNo));
    mDoMtx_stack_c::multVec(&a_eyeOfsTbl[param_1], &eyePos);
    mBaseAttnPos.set(current.pos.x, current.pos.y + AtnOfs(param_1), current.pos.z);

    if (field_0xe38 == 2) {
        if (mTalkNo == 0) {
            attention_info.position = mBaseAttnPos;
            if (mTalkMemberNum > 1) {
                for (int i = 1; i < mTalkMemberNum; i++) {
                    if (mTalkMembers[i] != NULL) {
                        attention_info.position += mTalkMembers[i]->getBaseAttnPos();
                    }
                }

                attention_info.position = attention_info.position / mTalkMemberNum;

                for (int i = 1; i < mTalkMemberNum; i++) {
                    if (mTalkMembers[i] != NULL) {
                        mTalkMembers[i]->attention_info.position = attention_info.position;
                    }
                }
            }
        }
    } else {
        attention_info.position = mBaseAttnPos;
    }

    return TRUE;
}

inline f32 ChkWallR(int type) {
    return (f32)(type < 16 ? a_prmTbl_M[type].field_0x3c : a_prmTbl_W[type - 16].field_0x3c);
}

inline f32 ChkWallH(int type) {
    return (f32)(type < 16 ? a_prmTbl_M[type].field_0x38 : a_prmTbl_W[type - 16].field_0x38);
}

inline f32 CylR(int type) {
    return (f32)(type < 16 ? a_prmTbl_M[type].field_0x34 : a_prmTbl_W[type - 16].field_0x34);
}

inline f32 CylH(int type) {
    return (f32)(type < 16 ? a_prmTbl_M[type].field_0x30 : a_prmTbl_W[type - 16].field_0x30);
}

cPhs__Step daNpcChat_c::Create() {
    fopAcM_ct(this, daNpcChat_c);

    mTwilight = dKy_darkworld_check();
    mType = getType();
    mObjNum = getObjNum();
    mTalkGroupNo = getTalkGroupNoParam();
    mTalkMemberNum = getTalkMenberNumParam();
    mTalkNo = getTalkNoParam();
    mTalkIconType = getTalkIconTypeParam();
    mTalkMotionNo = getTalkMotionNo();
    mLookPlayerCheck = getLookPlayerCheck();

    if (mTalkMotionNo == MOT_SITWAIT_A || mTalkMotionNo == MOT_SITTALK_SITWAIT_A || mTalkMotionNo == MOT_SITTALK_A || mTalkMotionNo == MOT_SITTALK_A_B || mTalkMotionNo == MOT_SITTALK_A_A_B) {
        field_0xe51 = 1;
    } else if (mTalkMotionNo == MOT_SITWAIT_B || mTalkMotionNo == MOT_SITTALK_SITWAIT_B || mTalkMotionNo == MOT_SITTALK_B || mTalkMotionNo == MOT_SITTALK_B_B || mTalkMotionNo == MOT_SITTALK_B_B_B) {
        field_0xe51 = 2;
    } else {
        field_0xe51 = 0;
    }

    JUT_ASSERT(645, mTalkMemberNum<=5);
    JUT_ASSERT(646, mTalkNo<5);

    mMsgNo = getMessageNo();

    cPhs__Step phase = loadResrc(mType, mObjNum);
    if (phase == cPhs_COMPLEATE_e) {
        if (!fopAcM_entrySolidHeap(this, createHeapCallBack, 0x800022E0)) {
            return cPhs_ERROR_e;
        }

        J3DModelData* mdlData_p = mAnm_p->getModel()->getModelData();
        fopAcM_SetMtx(this, mAnm_p->getModel()->getBaseTRMtx());
        fopAcM_setCullSizeBox(this, -40.0f, -10.0f, -30.0f, 30.0f, 190.0f, 40.0f);
        mSound.init(&current.pos, &eyePos, 3, 1);
        mSound.setMdlType(mType, false, mTwilight & 0xFF);

        #if DEBUG
        // mHIO = l_HIO;
        mHIO->entryHIO("多人数会話NPC");
        #endif

        mAcchCir.SetWall(ChkWallH(mType), ChkWallR(mType));
        mAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1, &mAcchCir,
                  fopAcM_GetSpeed_p(this), fopAcM_GetAngle_p(this), fopAcM_GetShapeAngle_p(this));
        mAcch.CrrPos(dComIfG_Bgsp());
        mCcStts.Init(daNpcChat_Param_c::m.common.weight, 0, this);
        mCyl.Set(mCcDCyl);
        mCyl.SetStts(&mCcStts);
        mCyl.SetTgType(0);
        mCyl.SetTgSPrm(0);
        mCyl.SetR(CylR(mType));
        mCyl.SetH(CylH(mType));
        mGndChk = mAcch.m_gnd;
        mGroundH = mAcch.GetGroundH();

        setEnvTevColor();
        setRoomNo();
        attention_info.distances[fopAc_attn_SPEAK_e] = fopAc_AttnFlag_ETC_e | fopAc_AttnFlag_JUEL_e | fopAc_AttnFlag_CARRY_e | fopAc_AttnFlag_SPEAK_e | fopAc_AttnFlag_LOCK_e;
        attention_info.distances[fopAc_attn_TALK_e] = fopAc_AttnFlag_ETC_e | fopAc_AttnFlag_JUEL_e | fopAc_AttnFlag_CARRY_e | fopAc_AttnFlag_SPEAK_e | fopAc_AttnFlag_LOCK_e;

        reset();
        Execute();
    }

    return phase;
}

inline f32 ObjScale(int type) {
    return (f32)(type < 16 ? a_prmTbl_M[type].field_0x18 : a_prmTbl_W[type - 16].field_0x18);
}

BOOL daNpcChat_c::CreateHeap() {
    J3DModel* model;
    BOOL rv = NpcCreate(mType);
    if (rv) {
        if (mObjNum != OBJ_TUBO) {
            model = ObjCreate(mObjNum);
            mObjModel = model;
            rv = model != NULL;
            if (!rv) {
                mAnm_p->stopZelAnime();
            }
        } else if (field_0xe51 == 1) {
            model = ChairCreate(ObjScale(mType));
            mObjModel = model;
            rv = model != NULL;
            if (!rv) {
                mAnm_p->stopZelAnime();
            }
        } else {
            mObjModel = NULL;
            rv = TRUE;
        }
    }

    setMotion(MOT_WAIT_A, -1.0f, 0);
    return rv;
}

int daNpcChat_c::Delete() {
    fpc_ProcID id = fopAcM_GetID(this);
    this->~daNpcChat_c();
    return 1;
}

int daNpcChat_c::Execute() {
    if (!appearCheck()) {
        return 1;
    }

    J3DModelData* a_mdlData_p = mAnm_p->getModel()->getModelData();
    J3DJointCallBack callBack = a_mdlData_p->getJointNodePointer(0)->getCallBack();
    u32 userArea = mAnm_p->getModel()->getUserArea();
    for (u16 i = 0; i < a_mdlData_p->getJointNum(); i++) {
        a_mdlData_p->getJointNodePointer(i)->setCallBack(ctrlJointCallBack);
    }
    mAnm_p->getModel()->setUserArea((uintptr_t)this);
    execute();

    for (u16 i = 0; i < a_mdlData_p->getJointNum(); i++) {
        a_mdlData_p->getJointNodePointer(i)->setCallBack(callBack);
    }
    mAnm_p->getModel()->setUserArea(userArea);

    return 1;
}

int daNpcChat_c::Draw() {
    draw(0, 0, daNpcChat_Param_c::m.common.real_shadow_size, NULL, 0);
    return 1;
}

int daNpcChat_c::draw(int param_1, int param_2, f32 param_3, _GXColorS10* i_color, int param_5) {
    J3DModel* model = mAnm_p->getModel();
    J3DModelData* a_mdlData_p = model->getModelData();

    if (i_color != NULL) {
        tevStr.TevColor.r = i_color->r;
        tevStr.TevColor.g = i_color->g;
        tevStr.TevColor.b = i_color->b;
        tevStr.TevColor.a = i_color->a;
    } else {
        tevStr.TevColor.g = 0;
        tevStr.TevColor.r = 0;
    }

    if (mTwilight) {
        g_env_light.settingTevStruct(4, &current.pos, &tevStr);
    } else {
        g_env_light.settingTevStruct(0, &current.pos, &tevStr);
    }

    g_env_light.setLightTevColorType_MAJI(model, &tevStr);

    if (!checkHide() && !drawDbgInfo()) {
        if ((mAnmFlags & ANM_PLAY_BTP) != 0) {
            mBtpAnm.entry(a_mdlData_p);
        }

        if ((mAnmFlags & ANM_PLAY_BTK) != 0) {
            mBtkAnm.entry(a_mdlData_p);
        }

        if ((mAnmFlags & ANM_PLAY_BRK) != 0) {
            mBrkAnm.entry(a_mdlData_p);
        }

        if (param_2 != 0) {
            fopAcM_setEffectMtx(this, a_mdlData_p);
        }

        if (mTwilight) {
            dComIfGd_setListDark();
            mAnm_p->entryDL();
            dComIfGd_setList();
        } else {
            mAnm_p->entryDL();
        }

        if ((mAnmFlags & ANM_PLAY_BTP) != 0) {
            mBtpAnm.remove(a_mdlData_p);
        }

        if ((mAnmFlags & ANM_PLAY_BTK) != 0) {
            mBtkAnm.remove(a_mdlData_p);
        }

        if ((mAnmFlags & ANM_PLAY_BRK) != 0) {
            mBrkAnm.remove(a_mdlData_p);
        }

        dComIfGd_setSimpleShadow(&current.pos, mAcch.GetGroundH(), 60.0f, mAcch.m_gnd, 0, 1.0f, dDlst_shadowControl_c::getSimpleTex());
    }

    drawOtherMdls();
    return 1;
}

inline f32 JntTX(int type, int jnt_no) {
    return (type < 16 ? a_jntTbl_M[type].jntPrm[jnt_no].jntT[0] : a_jntTbl_W[type - 16].jntPrm[jnt_no].jntT[0]) * 10.0f;
}

inline f32 JntTY(int type, int jnt_no) {
    return (type < 16 ? a_jntTbl_M[type].jntPrm[jnt_no].jntT[1] : a_jntTbl_W[type - 16].jntPrm[jnt_no].jntT[1]) * 10.0f;
}

inline f32 JntTZ(int type, int jnt_no) {
    return (type < 16 ? a_jntTbl_M[type].jntPrm[jnt_no].jntT[2] : a_jntTbl_W[type - 16].jntPrm[jnt_no].jntT[2]) * 10.0f;
}

inline s16 JntRX(int type, int jnt_no) {
    return (type < 16 ? a_jntTbl_M[type].jntPrm[jnt_no].jntR[0] : a_jntTbl_W[type - 16].jntPrm[jnt_no].jntR[0]) * 182.04445f;
}

inline s16 JntRY(int type, int jnt_no) {
    return (type < 16 ? a_jntTbl_M[type].jntPrm[jnt_no].jntR[1] : a_jntTbl_W[type - 16].jntPrm[jnt_no].jntR[1]) * 182.04445f;
}

inline s16 JntRZ(int type, int jnt_no) {
    return (type < 16 ? a_jntTbl_M[type].jntPrm[jnt_no].jntR[2] : a_jntTbl_W[type - 16].jntPrm[jnt_no].jntR[2]) * 182.04445f;
}

int daNpcChat_c::ctrlJoint(J3DJoint* i_joint, J3DModel* i_model) {
    int jntNo = i_joint->getJntNo();
    cXyz jntT(JntTX(mType, jntNo), JntTY(mType, jntNo), JntTZ(mType, jntNo));
    csXyz jntR(JntRX(mType, jntNo), JntRY(mType, jntNo), JntRZ(mType, jntNo));
    mDoMtx_stack_c::copy(i_model->getAnmMtx(jntNo));
    mDoMtx_stack_c::ZXYrotM(jntR);
    mDoMtx_stack_c::transM(jntT);
    i_model->setAnmMtx(jntNo, mDoMtx_stack_c::get());
    MTXCopy(mDoMtx_stack_c::get(), J3DSys::mCurrentMtx);
    return 1;
}

int daNpcChat_c::createHeapCallBack(fopAc_ac_c* a_this) {
    daNpcChat_c* i_this = (daNpcChat_c*)a_this;
    return i_this->CreateHeap();
}

int daNpcChat_c::ctrlJointCallBack(J3DJoint* i_joint, int param_2) {
    if (param_2 == 0) {
        J3DModel* model = j3dSys.getModel();
        daNpcChat_c* i_this = (daNpcChat_c*)model->getUserArea();
        if (i_this != NULL) {
            i_this->ctrlJoint(i_joint, model);
        }
    }

    return 1;
}

static void* s_sub(void* i_actor, void* i_data) {
    if (fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_NPC_CHAT) {
        ((daNpcChat_c*)i_data)->setTalkMember((daNpcChat_c*)i_actor);
    }

    return NULL;
}

BOOL daNpcChat_c::searchGroup() {
    switch (field_0xe38) {
        case 0:
            field_0xe38 = 1;
            return FALSE;
        
        case 1:
            fpcM_Search(s_sub, this);
            int iVar1 = 0;
            for (int i = 0; i < 5; i++) {
                if (mTalkMembers[i] != NULL) {
                    iVar1++;
                } else {
                    break;
                }
            }

            if (iVar1 >= mTalkMemberNum) {
                field_0xe38 = 2;
            }
            return FALSE;
    }

    return TRUE;
}

BOOL daNpcChat_c::appearTimeCheck() {
    int time = dKy_darkworld_check() ? dKy_getDarktime_hour() : dKy_getdaytime_hour();
    BOOL rv = FALSE;

    switch (mTalkGroupNo) {
        case 0:
            if (time >= 8 && time < 17) {
                rv = TRUE;
            }
            break;

        case 1:
            if (time >= 8 && time < 22) {
                rv = TRUE;
            }
            break;

        case 2:
            if (time >= 10 && time < 20) {
                rv = TRUE;
            }
            break;

        case 3:
        case 6:
            if (time >= 7 && time < 22) {
                rv = TRUE;
            }
            break;

        case 4:
            if (time >= 12 && time < 22) {
                rv = TRUE;
            }
            break;

        case 5:
            if (time >= 10) {
                rv = TRUE;
            }
            break;

        case 14:
        case 15:
            if (time >= 21 || time < 5) {
                rv = TRUE;
            }
            break;

        default:
            rv = TRUE;
            break;
    }

    return rv;
}

BOOL daNpcChat_c::appearCheck() {
    if (searchGroup()) {
        if (mTalkNo == 0) {
            if (mHide) {
                if (appearTimeCheck()) {
                    int iVar1 = 0;
                    for (int i = 0; i < mTalkMemberNum; i++) {
                        if (fopAcM_CheckCondition(mTalkMembers[i], fopAcCnd_NODRAW_e)) {
                            iVar1++;
                        } else {
                            break;
                        }
                    }

                    if (iVar1 == mTalkMemberNum) {
                        for (int i = 0; i < mTalkMemberNum; i++) {
                            mTalkMembers[i]->offHide();
                            fopAcM_OffStatus(mTalkMembers[i], fopAcM_STATUS_UNK_0x8000000);
                        }
                    }
                }
            } else if (!appearTimeCheck()) {
                int iVar2 = 0;
                for (int i = 0; i < mTalkMemberNum; i++) {
                    if (fopAcM_CheckCondition(mTalkMembers[i], fopAcCnd_NODRAW_e)) {
                        iVar2++;
                    } else {
                        break;
                    }
                }

                if (iVar2 == mTalkMemberNum) {
                    for (int i = 0; i < mTalkMemberNum; i++) {
                        mTalkMembers[i]->onHide();
                        fopAcM_OnStatus(mTalkMembers[i], fopAcM_STATUS_UNK_0x8000000);
                    }
                }
            }
        }

        if (mHide == true || fopAcM_CheckCondition(this, fopAcCnd_NODRAW_e)) {
            return FALSE;
        }
    }

    return TRUE;
}

void daNpcChat_c::setParam() {
    if (checkHide()) {
        attention_info.flags = 0;
    } else {
        if (mTalkIconType == 0) {
            attention_info.flags = 0xC0000A;
        } else {
            attention_info.flags = fopAc_AttnFlag_UNK_0x400000 | fopAc_AttnFlag_SPEAK_e | fopAc_AttnFlag_TALK_e;
        }
    }

    #if DEBUG
    scale.set(daNpcChat_Param_c::m.common.scale, daNpcChat_Param_c::m.common.scale, daNpcChat_Param_c::m.common.scale);
    mAcchCir.SetWallR(daNpcChat_Param_c::m.common.width);
    mAcchCir.SetWallH(daNpcChat_Param_c::m.common.knee_length);
    gravity = daNpcChat_Param_c::m.common.gravity;
    #endif
}

BOOL daNpcChat_c::main() {
    if (mAction) {
        (this->*mAction)(NULL);
    }

    playMotion();
    return TRUE;
}

void daNpcChat_c::setAttnPos() {
    if (field_0xe1c == 1) {
        for (int i = 0; i < 3; i++) {
            mLookatAngle[i].setall(0);
        }
    }

    daNpcF_c::setMtx();
    cXyz sp28(10.0f, 15.0f, 0.0f);
    mDoMtx_stack_c::copy(mAnm_p->getModel()->getAnmMtx(3));
    mDoMtx_stack_c::multVecZero(&mHeadPos);
    mDoMtx_stack_c::multVec(&sp28, &eyePos);
    sp28.x = 0.0f;
    mDoMtx_stack_c::multVec(&sp28, &sp28);
    mHeadAngle.x = cLib_targetAngleX(&mHeadPos, &sp28);
    mHeadAngle.y = cLib_targetAngleY(&mHeadPos, &sp28);
    setAttention(mType);
    mCyl.SetC(current.pos);
    dComIfG_Ccsp()->Set(&mCyl);
}

bool daNpcChat_c::setExpressionBtp(int i_index) {
    J3DAnmTexPattern* i_btp = getTexAnmP(i_index);
    if (i_btp == NULL) {
        return true;
    }

    J3DModelData* a_mdlData_p = mAnm_p->getModel()->getModelData();
    if (setBtpAnm(i_btp, a_mdlData_p, 1.0f, J3DFrameCtrl::EMode_LOOP)) {
        mAnmFlags |= ANM_FLAG_800 | ANM_PLAY_BTP | ANM_PAUSE_BTP;
        return true;
    }

    OS_REPORT("%s: 表情Btpアニメーションの登録に失敗しました！\n", "d_a_npc_chat.cpp");
    return false;
}

void daNpcChat_c::setMotionAnm(int i_index, f32 i_morf) {
    J3DAnmTransformKey* anm;
    int i_attr = J3DFrameCtrl::EMode_LOOP;
    int index = i_index;
    int objNum = mObjNum;
    mMotionAnm = index;

    switch (index) {
        case ANM_WAIT_A:
        case ANM_WAIT_KAGO:
        case ANM_WAIT_STICK:
        case ANM_WAIT_TUBO:
        case ANM_PLAYM:
            switch (objNum) {
                case OBJ_TUBO2:
                case OBJ_MILKTUBO:
                    index = ANM_WAIT_TUBO;
                    break;

                case OBJ_SPEAR:
                case OBJ_BROOM:
                case OBJ_CELEB_BAG_M:
                case OBJ_CELEB_BAG_W:
                case OBJ_MARO_BAG_M:
                case OBJ_MARO_BAG_W:
                    index = ANM_WAIT_STICK;
                    break;

                case OBJ_KAGOM:
                case OBJ_KAGOW:
                    index = ANM_WAIT_KAGO;
                    break;

                case OBJ_GUITAR:
                case OBJ_FLUTE:
                    index = ANM_PLAYM;
                    break;
            }
            break;

        case ANM_WAIT_B:
            switch (objNum) {
                case OBJ_TUBO2:
                case OBJ_MILKTUBO:
                    index = ANM_WAIT_TUBO;
                    break;

                case OBJ_SPEAR:
                case OBJ_BROOM:
                case OBJ_CELEB_BAG_M:
                case OBJ_CELEB_BAG_W:
                case OBJ_MARO_BAG_M:
                case OBJ_MARO_BAG_W:
                    index = ANM_WAIT_STICK;
                    break;

                case OBJ_KAGOM:
                case OBJ_KAGOW:
                    index = ANM_WAIT_KAGO;
                    break;

                case OBJ_GUITAR:
                case OBJ_FLUTE:
                    index = ANM_PLAYM;
                    break;
            }
            break;

        case ANM_WALK_A:
        case ANM_WALK_KAGO:
        case ANM_WALK_STICK:
        case ANM_WALK_TUBO:
            switch (objNum) {
                case OBJ_TUBO2:
                case OBJ_MILKTUBO:
                    index = ANM_WALK_TUBO;
                    break;

                case OBJ_SPEAR:
                case OBJ_BROOM:
                case OBJ_CELEB_BAG_M:
                case OBJ_CELEB_BAG_W:
                case OBJ_MARO_BAG_M:
                case OBJ_MARO_BAG_W:
                    index = ANM_WALK_STICK;
                    break;

                case OBJ_KAGOM:
                case OBJ_KAGOW:
                    index = ANM_WALK_KAGO;
                    break;

                case OBJ_GUITAR:
                case OBJ_FLUTE:
                    index = ANM_PLAYM;
                    break;
            }
            break;

        case 3:
            switch (objNum) {
                case OBJ_TUBO2:
                case OBJ_MILKTUBO:
                    index = ANM_WALK_TUBO;
                    break;

                case OBJ_SPEAR:
                case OBJ_BROOM:
                case OBJ_CELEB_BAG_M:
                case OBJ_CELEB_BAG_W:
                case OBJ_MARO_BAG_M:
                case OBJ_MARO_BAG_W:
                    index = ANM_WALK_STICK;
                    break;

                case OBJ_KAGOM:
                case OBJ_KAGOW:
                    index = ANM_WALK_KAGO;
                    break;

                case OBJ_GUITAR:
                case OBJ_FLUTE:
                    index = ANM_PLAYM;
                    break;
            }
            break;

        case ANM_TALK_A:
        case ANM_TALK_KAGO:
        case ANM_TALK_STICK:
        case ANM_TALK_TUBO:
            switch (objNum) {
                case OBJ_TUBO2:
                case OBJ_MILKTUBO:
                    index = ANM_TALK_TUBO;
                    break;

                case OBJ_SPEAR:
                case OBJ_BROOM:
                case OBJ_CELEB_BAG_M:
                case OBJ_CELEB_BAG_W:
                case OBJ_MARO_BAG_M:
                case OBJ_MARO_BAG_W:
                    index = ANM_TALK_STICK;
                    break;

                case OBJ_KAGOM:
                case OBJ_KAGOW:
                    index = ANM_TALK_KAGO;
                    break;

                case OBJ_GUITAR:
                case OBJ_FLUTE:
                    index = ANM_PLAYM;
                    break;
            }
            break;

        case ANM_TALK_B:
        case ANM_TALK_B_KAGO:
        case ANM_TALK_B_STICK:
        case ANM_TALK_B_TUBO:
            switch (objNum) {
                case OBJ_TUBO2:
                case OBJ_MILKTUBO:
                    index = ANM_TALK_B_TUBO;
                    break;

                case OBJ_SPEAR:
                case OBJ_BROOM:
                case OBJ_CELEB_BAG_M:
                case OBJ_CELEB_BAG_W:
                case OBJ_MARO_BAG_M:
                case OBJ_MARO_BAG_W:
                    index = ANM_TALK_B_STICK;
                    break;

                case OBJ_KAGOM:
                case OBJ_KAGOW:
                    index = ANM_TALK_B_KAGO;
                    break;

                case OBJ_GUITAR:
                case OBJ_FLUTE:
                    index = ANM_PLAYM;
                    break;
            }
            break;

        case ANM_RUN_A:
        case ANM_RUN_B:
            switch (objNum) {
                case OBJ_TUBO2:
                case OBJ_MILKTUBO:
                    index = ANM_RUN_TUBO;
                    break;

                case OBJ_SPEAR:
                    index = ANM_RUN_KAMAE;
                    break;

                case OBJ_BROOM:
                case OBJ_CELEB_BAG_M:
                case OBJ_CELEB_BAG_W:
                case OBJ_MARO_BAG_M:
                case OBJ_MARO_BAG_W:
                    index = ANM_RUN_STICK;
                    break;

                case OBJ_KAGOM:
                case OBJ_KAGOW:
                    index = ANM_RUN_KAGO;
                    break;

                case OBJ_GUITAR:
                case OBJ_FLUTE:
                    index = ANM_PLAYM;
                    break;
            }
            break;

        case ANM_TALK_C:
        case ANM_LOOK_A:
        case ANM_LOOK_B:
        case ANM_WAIT_WALL:
        case ANM_TALK_WALL:
        case ANM_TALK_B_WALL:
        case ANM_SITWAIT_A:
        case ANM_SITTALK_A:
        case ANM_SITTALK_A_B:
        case ANM_SITWAIT_B:
        case ANM_SITTALK_B:
        case ANM_SITTALK_B_B:
        case ANM_BROWSE_A:
        case ANM_BROWSE_B:
            switch (objNum) {
                case OBJ_TUBO2:
                case OBJ_MILKTUBO:
                    break;

                case OBJ_SPEAR:
                case OBJ_BROOM:
                case OBJ_CELEB_BAG_M:
                case OBJ_CELEB_BAG_W:
                case OBJ_MARO_BAG_M:
                case OBJ_MARO_BAG_W:
                    break;

                case OBJ_KAGOM:
                case OBJ_KAGOW:
                    break;
                
                case OBJ_GUITAR:
                case OBJ_FLUTE:
                    index = ANM_PLAYM;
                    break;
            }
            break;

        case ANM_TO_WOLF:
        case ANM_SIT_TO_WOLF_A:
        case ANM_SIT_TO_WOLF_B:
        case ANM_SURPRISE:
        case ANM_MARO_DANCE:
        case ANM_2LADYTALK_A:
        case ANM_2LADYTALK_B:
        case ANM_2NORMALTALK_A:
        case ANM_2NORMALTALK_B:
        case ANM_CELLME:
        case ANM_SING:
            break;

        case ANM_KAMAE:
        case ANM_KAMAE_WAIT_B:
        case ANM_KAMAE_STEP:
        case ANM_KAMAE_C:
            JUT_ASSERT(1557, mType == MdlMANa_e || mType == MdlMADa_e);
            break;

        case ANM_RUN_KAGO:
        case ANM_RUN_STICK:
        case ANM_RUN_TUBO:
        case ANM_RUN_KAMAE:
        default:
            return;
    }

    switch (index) {
        case ANM_TALK_C:
            mSound.playVoice(3);
            break;

        case ANM_2LADYTALK_B:
            mSound.playVoice(4);
            break;
        
        case ANM_SURPRISE:
            mSound.playVoice(1);
            break;

        case ANM_BROWSE_A:
        case ANM_BROWSE_B:
            mSound.playVoice(5);
            break;
    }

    switch (index) {
        case ANM_SURPRISE:
            i_attr = J3DFrameCtrl::EMode_NONE;
            break;
    }

    int resIdx;
    char* arcName;
    if (isM_()) {
        arcName = l_bckTbl_M[index].arc_name;
        resIdx = l_bckTbl_M[index].resource_index;
    } else {
        arcName = l_bckTbl_W[index].arc_name;
        resIdx = l_bckTbl_W[index].resource_index;
    }

    if (resIdx >= 0) {
        anm = getTrnsfrmKeyAnmP(arcName, resIdx);
        mAnmFlags &= ~(ANM_PLAY_MORF | ANM_PAUSE_MORF);

        if (anm == NULL) {
            OS_REPORT("n\n\n\n\n\nAnime None!!!!!!!!!!!!!!!!!!!! %d\n\n\n\n\n\n", mType);
        } else if (setMcaMorfAnm(anm, 1.0f, i_morf, i_attr, 0, -1)) {
            mAnmFlags |= ANM_PLAY_MORF | ANM_PAUSE_MORF;
            mMotionLoops = 0;
        }
    }
}

void daNpcChat_c::setMotion(int i_motion, f32 i_morf, int param_3) {
    s16 motion = i_motion;

    if ((param_3 != 0 || mMotion != motion) && i_motion >= 0 && i_motion < 0x28) {
        mMotion = motion;
        mMotionMorfOverride = i_morf;
        mMotionPrevPhase = -1;
        mMotionPhase = 0;
    }
}

int daNpcChat_c::drawDbgInfo() {
    return 0;
}

void daNpcChat_c::drawOtherMdls() {
    static int const a_jntNumTbl[13][2] = {
        -1, -1,
        JNTM_HANDL, -1,
        JNTM_HANDR, -1,
        -1, JNTW_HANDR,
        JNTM_HANDL, -1,
        -1, JNTW_HANDR,
        1, -1,
        -1, JNTW_HANDR,
        -1, JNTW_HANDL,
        JNTM_ARMR2, -1,
        -1, JNTW_HANDR,
        JNTM_ARMR2, -1,
        -1, JNTW_HANDR,
    };

    int x = isM_() ? 0 : 1;
    int jntNo = a_jntNumTbl[mObjNum][x];
    f32 objScale = ObjScale(mType);

    if (field_0xe4f != 0) {
        if (mObjNum != OBJ_TUBO) {
            if (!checkHide()) {
                if (mObjModel != NULL && jntNo >= 0) {
                  if (!chkAction(&daNpcChat_c::fear)) {
                        g_env_light.setLightTevColorType_MAJI(mObjModel, &tevStr);
                        mDoMtx_stack_c::copy(mAnm_p->getModel()->getAnmMtx(jntNo));
                        mDoMtx_stack_c::scaleM(objScale, objScale, objScale);
                        mObjModel->setBaseTRMtx(mDoMtx_stack_c::get());
                        mDoExt_modelUpdateDL(mObjModel);
                    }
                }
            }
        } else if (field_0xe51 == 1 && !mHide && mObjModel != NULL) {
            g_env_light.setLightTevColorType_MAJI(mObjModel, &tevStr);
            mDoExt_modelUpdateDL(mObjModel);
        }
    }
}

u8 daNpcChat_c::getTalkMotionNo() {
    u8 talkNo = home.angle.z & 0xFF;
    if (talkNo >= 35) {
        talkNo = 1;
    }

    return talkNo;
}

bool daNpcChat_c::getLookPlayerCheck() {
    u8 uVar1 = (home.angle.z >> 8) & 0xFF;
    if ((u32)uVar1 == 1 && !mTwilight) {
        return true;
    }

    return false;
}

void daNpcChat_c::reset() {
    initialize();
    mLookat.initialize();

    for (int i = 0; i < 1; i++) {
        mActorMngr[i].initialize();
    }

    field_0xe08 = 0;
    field_0xe0c = 0;
    mAction = NULL;
    field_0xe1c = -1;
    mMode = 0;
    current.pos.set(home.pos);
    old.pos.set(current.pos);
    current.angle.set(0, home.angle.y, 0);
    old.angle = current.angle;
    shape_angle = current.angle;
    mCurAngle = current.angle;
    mOldAngle = mCurAngle;
    speedF = 0.0f;
    speed.setall(0.0f);
    field_0xe04 = fpcM_ERROR_PROCESS_ID_e;
    mOrderEvtNo = 0;
    field_0xe38 = 0;

    for (int i = 0; i < 5; i++) {
        mTalkMembers[i] = NULL;
    }
    mTalkMembers[mTalkNo] = this;

    mFear = false;
    mTalkFlag = false;

    if (mObjModel != NULL) {
        field_0xe4f = true;
    } else {
        field_0xe4f = false;
    }

    if (appearTimeCheck()) {
        mHide = false;
    } else {
        mHide = true;
        fopAcM_OnStatus(this, fopAcM_STATUS_UNK_0x8000000);
    }

    setAction(&daNpcChat_c::wait);
    mMotionMorfOverride = 0.0f;
}

void daNpcChat_c::playMotion() {
    daNpcF_anmPlayData dat0 = {ANM_TALK_A, daNpcChat_Param_c::m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat0[1] = {&dat0};
    daNpcF_anmPlayData dat1 = {ANM_WAIT_A, daNpcChat_Param_c::m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat1[1] = {&dat1};
    daNpcF_anmPlayData dat2 = {ANM_WAIT_B, daNpcChat_Param_c::m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat2[1] = {&dat2};
    daNpcF_anmPlayData dat3 = {ANM_TALK_A, daNpcChat_Param_c::m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat3[1] = {&dat3};
    daNpcF_anmPlayData dat4 = {ANM_TALK_B, daNpcChat_Param_c::m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat4[1] = {&dat4};
    daNpcF_anmPlayData dat5a = {ANM_TALK_A, daNpcChat_Param_c::m.common.morf_frame, 1};
    daNpcF_anmPlayData dat5b = {ANM_WAIT_A, daNpcChat_Param_c::m.common.morf_frame, 1};
    daNpcF_anmPlayData dat5c = {ANM_TALK_B, daNpcChat_Param_c::m.common.morf_frame, 1};
    daNpcF_anmPlayData dat5d = {ANM_WAIT_A, daNpcChat_Param_c::m.common.morf_frame, 1};
    daNpcF_anmPlayData dat5e = {ANM_TALK_A, daNpcChat_Param_c::m.common.morf_frame, 1};
    daNpcF_anmPlayData dat5f = {ANM_TALK_B, daNpcChat_Param_c::m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat5[6] = {&dat5a, &dat5b, &dat5c, &dat5d, &dat5e, &dat5f};
    daNpcF_anmPlayData dat6 = {ANM_TALK_C, daNpcChat_Param_c::m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat6[1] = {&dat6};
    daNpcF_anmPlayData dat7a = {ANM_TALK_B, daNpcChat_Param_c::m.common.morf_frame, 1};
    daNpcF_anmPlayData dat7b = {ANM_TALK_A, daNpcChat_Param_c::m.common.morf_frame, 1};
    daNpcF_anmPlayData dat7c = {ANM_TALK_C, daNpcChat_Param_c::m.common.morf_frame, 1};
    daNpcF_anmPlayData dat7d = {ANM_WAIT_A, daNpcChat_Param_c::m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat7[4] = {&dat7a, &dat7b, &dat7c, &dat7d};
    daNpcF_anmPlayData dat8a = {ANM_TALK_A, daNpcChat_Param_c::m.common.morf_frame, 1};
    daNpcF_anmPlayData dat8b = {ANM_WAIT_A, daNpcChat_Param_c::m.common.morf_frame, 1};
    daNpcF_anmPlayData dat8c = {ANM_TALK_B, daNpcChat_Param_c::m.common.morf_frame, 1};
    daNpcF_anmPlayData dat8d = {ANM_WAIT_B, daNpcChat_Param_c::m.common.morf_frame, 1};
    daNpcF_anmPlayData dat8e = {ANM_TALK_C, daNpcChat_Param_c::m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat8[5] = {&dat8a, &dat8b, &dat8c, &dat8d, &dat8e};
    daNpcF_anmPlayData dat9a = {ANM_BROWSE_A, daNpcChat_Param_c::m.common.morf_frame, 1};
    daNpcF_anmPlayData dat9b = {ANM_BROWSE_B, daNpcChat_Param_c::m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat9[2] = {&dat9a, &dat9b};
    daNpcF_anmPlayData dat10a = {ANM_BROWSE_A, daNpcChat_Param_c::m.common.morf_frame, 1};
    daNpcF_anmPlayData dat10b = {ANM_BROWSE_B, daNpcChat_Param_c::m.common.morf_frame, 1};
    daNpcF_anmPlayData dat10c = {ANM_TALK_A, daNpcChat_Param_c::m.common.morf_frame, 1};
    daNpcF_anmPlayData dat10d = {ANM_TALK_B, daNpcChat_Param_c::m.common.morf_frame, 1};
    daNpcF_anmPlayData dat10e = {ANM_WAIT_A, daNpcChat_Param_c::m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat10[5] = {&dat10a, &dat10b, &dat10c, &dat10d, &dat10e};
    daNpcF_anmPlayData dat11a = {ANM_BROWSE_A, daNpcChat_Param_c::m.common.morf_frame, 1};
    daNpcF_anmPlayData dat11b = {ANM_BROWSE_B, daNpcChat_Param_c::m.common.morf_frame, 1};
    daNpcF_anmPlayData dat11c = {ANM_TALK_A, daNpcChat_Param_c::m.common.morf_frame, 1};
    daNpcF_anmPlayData dat11d = {ANM_TALK_B, daNpcChat_Param_c::m.common.morf_frame, 1};
    daNpcF_anmPlayData dat11e = {ANM_TALK_C, daNpcChat_Param_c::m.common.morf_frame, 1};
    daNpcF_anmPlayData dat11f = {ANM_WAIT_A, daNpcChat_Param_c::m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat11[6] = {&dat11a, &dat11b, &dat11c, &dat11d, &dat11e, &dat11f};
    daNpcF_anmPlayData dat12a = {ANM_2LADYTALK_A, daNpcChat_Param_c::m.common.morf_frame, 1};
    daNpcF_anmPlayData dat12b = {ANM_2NORMALTALK_A, daNpcChat_Param_c::m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat12[2] = {&dat12a, &dat12b};
    daNpcF_anmPlayData dat13a = {ANM_2LADYTALK_B, daNpcChat_Param_c::m.common.morf_frame, 1};
    daNpcF_anmPlayData dat13b = {ANM_2NORMALTALK_B, daNpcChat_Param_c::m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat13[2] = {&dat13a, &dat13b};
    daNpcF_anmPlayData dat14a = {ANM_TALK_A, daNpcChat_Param_c::m.common.morf_frame, 1};
    daNpcF_anmPlayData dat14b = {ANM_2LADYTALK_A, daNpcChat_Param_c::m.common.morf_frame, 1};
    daNpcF_anmPlayData dat14c = {ANM_WAIT_A, daNpcChat_Param_c::m.common.morf_frame, 1};
    daNpcF_anmPlayData dat14d = {ANM_TALK_B, daNpcChat_Param_c::m.common.morf_frame, 1};
    daNpcF_anmPlayData dat14e = {ANM_WAIT_A, daNpcChat_Param_c::m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat14[5] = {&dat14a, &dat14b, &dat14c, &dat14d, &dat14e};
    daNpcF_anmPlayData dat15a = {ANM_WAIT_A, daNpcChat_Param_c::m.common.morf_frame, 1};
    daNpcF_anmPlayData dat15b = {ANM_2LADYTALK_B, daNpcChat_Param_c::m.common.morf_frame, 1};
    daNpcF_anmPlayData dat15c = {ANM_TALK_A, daNpcChat_Param_c::m.common.morf_frame, 1};
    daNpcF_anmPlayData dat15d = {ANM_WAIT_A, daNpcChat_Param_c::m.common.morf_frame, 1};
    daNpcF_anmPlayData dat15e = {ANM_TALK_B, daNpcChat_Param_c::m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat15[5] = {&dat15a, &dat15b, &dat15c, &dat15d, &dat15e};
    daNpcF_anmPlayData dat16a = {ANM_TALK_A, daNpcChat_Param_c::m.common.morf_frame, 1};
    daNpcF_anmPlayData dat16b = {ANM_2LADYTALK_A, daNpcChat_Param_c::m.common.morf_frame, 1};
    daNpcF_anmPlayData dat16c = {ANM_WAIT_A, daNpcChat_Param_c::m.common.morf_frame, 1};
    daNpcF_anmPlayData dat16d = {ANM_TALK_B, daNpcChat_Param_c::m.common.morf_frame, 1};
    daNpcF_anmPlayData dat16e = {ANM_WAIT_A, daNpcChat_Param_c::m.common.morf_frame, 1};
    daNpcF_anmPlayData dat16f = {ANM_2NORMALTALK_A, daNpcChat_Param_c::m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat16[6] = {&dat16a, &dat16b, &dat16c, &dat16d, &dat16e, &dat16f};
    daNpcF_anmPlayData dat17a = {ANM_WAIT_A, daNpcChat_Param_c::m.common.morf_frame, 1};
    daNpcF_anmPlayData dat17b = {ANM_2LADYTALK_B, daNpcChat_Param_c::m.common.morf_frame, 1};
    daNpcF_anmPlayData dat17c = {ANM_TALK_A, daNpcChat_Param_c::m.common.morf_frame, 1};
    daNpcF_anmPlayData dat17d = {ANM_WAIT_A, daNpcChat_Param_c::m.common.morf_frame, 1};
    daNpcF_anmPlayData dat17e = {ANM_TALK_B, daNpcChat_Param_c::m.common.morf_frame, 1};
    daNpcF_anmPlayData dat17f = {ANM_2NORMALTALK_B, daNpcChat_Param_c::m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat17[6] = {&dat17a, &dat17b, &dat17c, &dat17d, &dat17e, &dat17f};
    daNpcF_anmPlayData dat18a = {ANM_TALK_A, daNpcChat_Param_c::m.common.morf_frame, 1};
    daNpcF_anmPlayData dat18b = {ANM_2LADYTALK_A, daNpcChat_Param_c::m.common.morf_frame, 1};
    daNpcF_anmPlayData dat18c = {ANM_2LADYTALK_B, daNpcChat_Param_c::m.common.morf_frame, 1};
    daNpcF_anmPlayData dat18d = {ANM_WAIT_A, daNpcChat_Param_c::m.common.morf_frame, 1};
    daNpcF_anmPlayData dat18e = {ANM_TALK_B, daNpcChat_Param_c::m.common.morf_frame, 1};
    daNpcF_anmPlayData dat18f = {ANM_WAIT_A, daNpcChat_Param_c::m.common.morf_frame, 1};
    daNpcF_anmPlayData dat18g = {ANM_2NORMALTALK_A, daNpcChat_Param_c::m.common.morf_frame, 1};
    daNpcF_anmPlayData dat18h = {ANM_2NORMALTALK_B, daNpcChat_Param_c::m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat18[8] = {&dat18a, &dat18b, &dat18c, &dat18d, &dat18e, &dat18f, &dat18g, &dat18h};
    daNpcF_anmPlayData dat19 = {ANM_LOOK_A, daNpcChat_Param_c::m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat19[1] = {&dat19};
    daNpcF_anmPlayData dat20 = {ANM_LOOK_B, daNpcChat_Param_c::m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat20[1] = {&dat20};
    daNpcF_anmPlayData dat21a = {ANM_LOOK_A, daNpcChat_Param_c::m.common.morf_frame, 1};
    daNpcF_anmPlayData dat21b = {ANM_LOOK_B, daNpcChat_Param_c::m.common.morf_frame, 1};
    daNpcF_anmPlayData dat21c = {ANM_WAIT_A, daNpcChat_Param_c::m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat21[3] = {&dat21a, &dat21b, &dat21c};
    daNpcF_anmPlayData dat22 = {ANM_WAIT_WALL, daNpcChat_Param_c::m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat22[1] = {&dat22};
    daNpcF_anmPlayData dat23a = {ANM_TALK_WALL, daNpcChat_Param_c::m.common.morf_frame, 1};
    daNpcF_anmPlayData dat23b = {ANM_WAIT_WALL, daNpcChat_Param_c::m.common.morf_frame, 1};
    daNpcF_anmPlayData dat23c = {ANM_TALK_B_WALL, daNpcChat_Param_c::m.common.morf_frame, 1};
    daNpcF_anmPlayData dat23d = {ANM_WAIT_WALL, daNpcChat_Param_c::m.common.morf_frame, 1};
    daNpcF_anmPlayData dat23e = {ANM_TALK_WALL, daNpcChat_Param_c::m.common.morf_frame, 1};
    daNpcF_anmPlayData dat23f = {ANM_TALK_B_WALL, daNpcChat_Param_c::m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat23[6] = {&dat23a, &dat23b, &dat23c, &dat23d, &dat23e, &dat23f};
    daNpcF_anmPlayData dat24 = {ANM_SITWAIT_A, daNpcChat_Param_c::m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat24[1] = {&dat24};
    daNpcF_anmPlayData dat25a = {ANM_SITTALK_A, daNpcChat_Param_c::m.common.morf_frame, 1};
    daNpcF_anmPlayData dat25b = {ANM_SITWAIT_A, daNpcChat_Param_c::m.common.morf_frame, 1};
    daNpcF_anmPlayData dat25c = {ANM_SITTALK_A_B, daNpcChat_Param_c::m.common.morf_frame, 1};
    daNpcF_anmPlayData dat25d = {ANM_SITWAIT_A, daNpcChat_Param_c::m.common.morf_frame, 1};
    daNpcF_anmPlayData dat25e = {ANM_SITTALK_A, daNpcChat_Param_c::m.common.morf_frame, 1};
    daNpcF_anmPlayData dat25f = {ANM_SITTALK_A_B, daNpcChat_Param_c::m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat25[6] = {&dat25a, &dat25b, &dat25c, &dat25d, &dat25e, &dat25f};
    daNpcF_anmPlayData dat26 = {ANM_SITWAIT_B, daNpcChat_Param_c::m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat26[1] = {&dat26};
    daNpcF_anmPlayData dat27a = {ANM_SITTALK_B, daNpcChat_Param_c::m.common.morf_frame, 1};
    daNpcF_anmPlayData dat27b = {ANM_SITWAIT_B, daNpcChat_Param_c::m.common.morf_frame, 1};
    daNpcF_anmPlayData dat27c = {ANM_SITTALK_B_B, daNpcChat_Param_c::m.common.morf_frame, 1};
    daNpcF_anmPlayData dat27d = {ANM_SITWAIT_B, daNpcChat_Param_c::m.common.morf_frame, 1};
    daNpcF_anmPlayData dat27e = {ANM_SITTALK_B, daNpcChat_Param_c::m.common.morf_frame, 1};
    daNpcF_anmPlayData dat27f = {ANM_SITTALK_B_B, daNpcChat_Param_c::m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat27[6] = {&dat27a, &dat27b, &dat27c, &dat27d, &dat27e, &dat27f};
    daNpcF_anmPlayData dat28 = {ANM_SING, daNpcChat_Param_c::m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat28[1] = {&dat28};
    daNpcF_anmPlayData dat29 = {ANM_SITTALK_A, daNpcChat_Param_c::m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat29[1] = {&dat29};
    daNpcF_anmPlayData dat30 = {ANM_SITTALK_A_B, daNpcChat_Param_c::m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat30[1] = {&dat30};
    daNpcF_anmPlayData dat31a = {ANM_SITTALK_A, daNpcChat_Param_c::m.common.morf_frame, 1};
    daNpcF_anmPlayData dat31b = {ANM_SITTALK_A_B, daNpcChat_Param_c::m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat31[2] = {&dat31a, &dat31b};
    daNpcF_anmPlayData dat32 = {ANM_SITTALK_B, daNpcChat_Param_c::m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat32[1] = {&dat32};
    daNpcF_anmPlayData dat33 = {ANM_SITTALK_B_B, daNpcChat_Param_c::m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat33[1] = {&dat33};
    daNpcF_anmPlayData dat34a = {ANM_SITTALK_B, daNpcChat_Param_c::m.common.morf_frame, 1};
    daNpcF_anmPlayData dat34b = {ANM_SITTALK_B_B, daNpcChat_Param_c::m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat34[2] = {&dat34a, &dat34b};
    daNpcF_anmPlayData dat35 = {ANM_KAMAE, daNpcChat_Param_c::m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat35[1] = {&dat35};
    daNpcF_anmPlayData dat36 = {ANM_KAMAE_C, daNpcChat_Param_c::m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat36[1] = {&dat36};
    daNpcF_anmPlayData dat37 = {ANM_KAMAE_STEP, daNpcChat_Param_c::m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat37[1] = {&dat37};
    daNpcF_anmPlayData dat38 = {ANM_SURPRISE, daNpcChat_Param_c::m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat38[1] = {&dat38};
    daNpcF_anmPlayData dat39 = {ANM_TO_WOLF, daNpcChat_Param_c::m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat39[1] = {&dat39};

    daNpcF_anmPlayData** ppDat[40] = {
        pDat0, pDat1, pDat2, pDat3, pDat4,
        pDat5, pDat6, pDat7, pDat8, pDat9,
        pDat10, pDat11, pDat12, pDat13, pDat14,
        pDat15, pDat16, pDat17, pDat18, pDat19,
        pDat20, pDat21, pDat22, pDat23, pDat24,
        pDat25, pDat26, pDat27, pDat28, pDat29,
        pDat30, pDat31, pDat32, pDat33, pDat34,
        pDat35, pDat36, pDat37, pDat38, pDat39,
    };

    if (mMotion >= 0 && mMotion < 35) {
        playMotionAnmLoop(ppDat);
    } else if (mMotion < 40) {
        playMotionAnm(ppDat);
    }
}

void daNpcChat_c::playMotionAnmLoop(daNpcF_c::daNpcF_anmPlayData*** i_data) {
    daNpcF_anmPlayData* playData = NULL;

    if (i_data[mMotion] != NULL) {
        playData = i_data[mMotion][mMotionPhase];
    }

    if (playData != NULL) {
        if (mMotionPrevPhase == mMotionPhase) {
            if (playData->numLoops > 0 && playData->numLoops <= mMotionLoops) {
                mMotionPhase++;
                playData = i_data[mMotion][mMotionPhase];
            } else if (playData->numLoops == 0 && mMotionLoops >= 1) {
                mMotionPhase = 0;
                playData = i_data[mMotion][mMotionPhase];
                mMotionMorfOverride = playData->morf;
            }
        }

        if (playData != NULL && mMotionPrevPhase != mMotionPhase) {
            setMotionAnm(playData->idx, 0.0f);
            f32 i_morf = playData->morf;

            if (mMotionPhase == 0 && 0.0f <= mMotionMorfOverride) {
                i_morf = mMotionMorfOverride;
            }

            mExpressionMorf = 0.0f;
            mAnm_p->setMorf(i_morf);
        }
    }

    mMotionPrevPhase = mMotionPhase;
}

BOOL daNpcChat_c::setAction(actionFunc action) {
    mMode = 3;

    if (mAction) {
        (this->*mAction)(NULL);
    }

    mMode = 0;
    mAction = action;

    if (mAction) {
        (this->*mAction)(NULL);
    }

    return TRUE;
}

bool daNpcChat_c::step(s16 i_targetAngle, int param_2) {
    if (mTurnMode == 0) {
        mTurnTargetAngle = i_targetAngle;
        mTurnAmount = 0;

        if (mCurAngle.y == mTurnTargetAngle) {
            mTurnMode++;
        }

        current.angle.y = mCurAngle.y;
        shape_angle.y = current.angle.y;
        mTurnMode++;
    } else if (mTurnMode == 1) {
        if (turn(mTurnTargetAngle, 15.0f, 0)) {
            shape_angle.y = current.angle.y;
            mCurAngle.y = current.angle.y;
            mOldAngle.y = current.angle.y;
            mTurnMode++;
        } else {
            shape_angle.y = current.angle.y;
            mCurAngle.y = current.angle.y;
        }
    }

    return mTurnMode > 1;
}

void daNpcChat_c::setTalkMember(daNpcChat_c* actor_p) {
    if (mTalkGroupNo == actor_p->getTalkGroupNo()) {
        int talkNo = actor_p->getTalkNo();
        if (talkNo < 0) {
            return;
        }

        if (talkNo < 5) {
            mTalkMembers[talkNo] = actor_p;
        }
    }
}

bool daNpcChat_c::wait(void* param_1) {
    int staffId;
    int i_expression, i_motion;
    int msgTimer;

    switch (mMode) {
        case 0:
            setMotion(mTalkMotionNo, -1.0f, 1);
            speedF = 0.0f;
            mTalkFlag = false;
            mMode = 2;
            break;

        case 2:
            if (field_0xe38 < 2) {
                return false;
            }

            if (
                dKy_darkworld_check() == 0 && daPy_py_c::checkNowWolf() &&
                fopAcM_searchPlayerDistanceXZ2(this) < std::pow(500.0f, 2.0f) || mFear
            ) {
                setAction(&daNpcChat_c::fear);
                if (!mFear) {
                    for (int i = 0; i < mTalkMemberNum; i++) {
                        if (mTalkMembers[i] != NULL) {
                            mTalkMembers[i]->setFear();
                        } else {
                            break;
                        }
                    }
                }
            } else {
                if (dComIfGp_event_runCheck()) {
                    if (eventInfo.checkCommandTalk()) {
                        if (!dComIfGp_event_chkTalkXY() || dComIfGp_evmng_ChkPresentEnd()) {
                            setAction(&daNpcChat_c::talk);
                        }
                    } else {
                        staffId = dComIfGp_getEventManager().getMyStaffId(l_myName, NULL, 0);
                        if (staffId != -1) {
                            setAction(&daNpcChat_c::demo);
                        }
                    }

                    if (
                        mTalkFlag && mLookPlayerCheck == 1 && mCurAngle.y != fopAcM_searchPlayerAngleY(this) &&
                        step(fopAcM_searchPlayerAngleY(this), 1)
                    ) {
                        mTurnMode = 0;
                    }

                    msgTimer = mMsgTimer;
                    if (ctrlMsgAnm(i_expression, i_motion, this, mTalkFlag)) {
                        setMotion(i_motion, -1.0f, 1);
                    }

                    if (mTalkFlag && dComIfGp_event_getTalkPartner() != this) {
                        if (field_0xe51 == 1) {
                            setMotion(MOT_SITWAIT_A, -1.0f, 0);
                        } else if (field_0xe51 == 2) {
                            setMotion(MOT_SITWAIT_B, -1.0f, 0);
                        } else {
                            setMotion(MOT_WAIT_A, -1.0f, 0);
                        }
                    }
                } else if (mTalkFlag) {
                    mMode = 0;
                } else {
                    if (home.angle.y != mCurAngle.y && step((s16)home.angle.y, 1)) {
                        setMotion(mTalkMotionNo, -1.0f, 1);
                        mTurnMode = 0;
                    }
                    
                    orderEvent(mOrderSpeakEvt, l_evtNames[mOrderEvtNo], 0xFFFF, 0x28, 0xFF, 1);
                }
            }
            break;

        case 3:
            break;

        default:
            JUT_ASSERT(2580, FALSE);
    }

    return true;
}

bool daNpcChat_c::fear(void* param_1) {
    switch (mMode) {
        case 0:
            setAngle(fopAcM_searchPlayerAngleY(this));
            setMotion(MOT_SURPRISE, -1.0f, 0);
            speedF = 0.0f;
            dComIfGs_onSaveDunSwitch(60);
            mMode = 2;
            break;
        
        case 2:
            if (mMotionAnm == ANM_SURPRISE) {
                if (mAnm_p->isStop()) {
                    setMotion(MOT_TO_WOLF, -1.0f, 0);
                    mPlayerAngleY = fopAcM_searchPlayerAngleY(this) + 0x8000;
                }
            } else {
                if (mCurAngle.y != mPlayerAngleY) {
                    cLib_addCalcAngleS2(&shape_angle.y, mPlayerAngleY, 3, 0x600);
                    setAngle(shape_angle.y);
                }

                if (mAnm_p->getFrame() >= 1.0f && mAnm_p->getFrame() < 2.0f) {
                    mSound.playVoice(2);
                }
            }
            break;

        case 3:
            break;

        default:
            JUT_ASSERT(2664, FALSE);
            break;
    }

    return 1;
}

bool daNpcChat_c::talk(void* param_1) {
    bool rv = false;

    switch (mMode) {
        case 0:
            initTalk(mMsgNo, (fopAc_ac_c**)mTalkMembers);
            mMsgTimer = 0;
            mOrderSpeakEvt = false;

            for (int i = 0; i < mTalkMemberNum; i++) {
                if (mTalkMembers[i] != NULL) {
                    mTalkMembers[i]->setTalkFlag();
                } else {
                    break;
                }
            }

            mMode = 2;
            break;
        
        case 2:
            if (mLookPlayerCheck == 1) {
                if (mCurAngle.y == fopAcM_searchPlayerAngleY(this)) {
                    if (talkProc(NULL, FALSE, (fopAc_ac_c**)mTalkMembers)) {
                        setAction(&daNpcChat_c::wait);
                        rv = true;
                    } else {
                        int msgTimer, i_expression, i_motion;
                        msgTimer = mMsgTimer;
                        
                        if (ctrlMsgAnm(i_expression, i_motion, this, FALSE) != 0) {
                            setMotion(i_motion, -1.0f, 0);
                        }

                        if (dComIfGp_event_getTalkPartner() != this && dComIfGp_event_getTalkPartner() != this) {
                            if (field_0xe51 == 1) {
                                setMotion(MOT_SITWAIT_A, -1.0f, 0);
                            } else if (field_0xe51 == 2) {
                                setMotion(MOT_SITWAIT_B, -1.0f, 0);
                            } else {
                                setMotion(MOT_WAIT_A, -1.0f, 0);
                            }
                        }
                    }
                } else {
                    if (step(fopAcM_searchPlayerAngleY(this), 1)) {
                        mTurnMode = 0;
                    }
                }
            } else {
                if (talkProc(NULL, FALSE, (fopAc_ac_c**)mTalkMembers)) {
                    setAction(&daNpcChat_c::wait);
                    rv = true;
                } else {
                    int i_expression, i_motion;
                    if (ctrlMsgAnm(i_expression, i_motion, this, FALSE)) {
                        setMotion(i_motion, -1.0f, 0);
                    }

                    if (dComIfGp_event_getTalkPartner() != this) {
                        if (field_0xe51 == 1) {
                            setMotion(MOT_SITWAIT_A, -1.0f, 0);
                        } else if (field_0xe51 == 2) {
                            setMotion(MOT_SITWAIT_B, -1.0f, 0);
                        } else {
                            setMotion(MOT_WAIT_A, -1.0f, 0);
                        }
                    }
                }
            }
            break;

        case 3:
            if (!field_0x9ec) {
                dComIfGp_event_reset();
            }
            break;

        default:
            JUT_ASSERT(2764, FALSE);
            break;
    }

    return rv;
}

bool daNpcChat_c::demo(void* param_1) {
    dComIfGp_getEventManager();

    switch (mMode) {
        case 0:
            mMode = 2;
            break;
        
        case 2:
            if (dComIfGp_event_runCheck()) {
                if (eventInfo.checkCommandTalk() == false) {
                    dEvent_manager_c& eventManager = dComIfGp_getEventManager();
                    s32 staffId = eventManager.getMyStaffId(l_myName, NULL, 0);
                    if (staffId != -1) {
                        mStaffID = staffId;

                        JUT_ASSERT(2798, NULL != mEvtSeqList[mOrderEvtNo]);

                        if ((this->*mEvtSeqList[mOrderEvtNo])(staffId)) {
                            eventManager.cutEnd(staffId);
                        }
                    }

                    if (eventInfo.checkCommandDemoAccrpt() && mEventIdx != -1 && eventManager.endCheck(mEventIdx)) {
                        dComIfGp_event_reset();
                        mOrderEvtNo = 0;
                        mEventIdx = -1;
                        setAction(&daNpcChat_c::wait);
                    }
                }
            }
            break;

        case 3:
            break;

        default:
            JUT_ASSERT(2826, FALSE);
            break;
    }

    return true;
}

static int daNpcChat_Create(void* a_this) {
    return static_cast<daNpcChat_c*>(a_this)->Create();
}

static int daNpcChat_Delete(void* a_this) {
    return static_cast<daNpcChat_c*>(a_this)->Delete();
}

static int daNpcChat_Execute(void* a_this) {
    return static_cast<daNpcChat_c*>(a_this)->Execute();
}

static int daNpcChat_Draw(void* a_this) {
    return static_cast<daNpcChat_c*>(a_this)->Draw();
}

static int daNpcChat_IsDelete(void* a_this) {
    return 1;
}

AUDIO_INSTANCES;

static actor_method_class daNpcChat_MethodTable = {
    (process_method_func)daNpcChat_Create,
    (process_method_func)daNpcChat_Delete,
    (process_method_func)daNpcChat_Execute,
    (process_method_func)daNpcChat_IsDelete,
    (process_method_func)daNpcChat_Draw,
};

actor_process_profile_definition g_profile_NPC_CHAT = {
  fpcLy_CURRENT_e,        // mLayerID
  7,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_NPC_CHAT,          // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(daNpcChat_c),    // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  398,                    // mPriority
  &daNpcChat_MethodTable, // sub_method
  0x00044107,             // mStatus
  fopAc_NPC_e,            // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};
