/**
 * @file d_a_npc_besu.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_npc_besu.h"
#include "d/actor/d_a_npc_len.h"
#include "d/actor/d_a_tag_push.h"
#include "d/d_com_static.h"
#include "d/d_item.h"

enum Besu_RES_File_ID {
    /* BMDR */
    /* 0x3 */ BMDR_BESU = 0x3,
};

enum Besu_TW_RES_File_ID {
    /* BCK */
    /* 0x5 */ BCK_BESU_DEMO_FEAR = 0x5,
    /* 0x6 */ BCK_BESU_DEMO_WAIT,
    /* 0x7 */ BCK_BESU_F_DEMO_FEAR,
    /* 0x8 */ BCK_BESU_F_DEMO_WAIT,

    /* BMDR */
    /* 0xB */ BMDR_BESU_TW = 0xB,

    /* BTP */
    /* 0xE */ BTP_BESU_F_DEMO_FEAR = 0xE,
    /* 0xF */ BTP_BESU_F_DEMO_WAIT,
};

enum Besu0_RES_File_ID {
    /* BCK */
    /* 0x05 */ BCK_BESU_BODY_TALK = 0x5,
    /* 0x06 */ BCK_BESU_BODY_WAIT,
    /* 0x07 */ BCK_BESU_F_TALK_C,
    /* 0x08 */ BCK_BESU_F_YOKERU,
    /* 0x09 */ BCK_BESU_FH_BIKKURI,
    /* 0x0A */ BCK_BESU_MAD,
    /* 0x0B */ BCK_BESU_MAD_WAIT,
    /* 0x0C */ BCK_BESU_NORMAL_STEP,
    /* 0x0D */ BCK_BESU_NORMAL_TALK,
    /* 0x0E */ BCK_BESU_NORMAL_TALK_B,
    /* 0x0F */ BCK_BESU_NORMAL_WAIT,
    /* 0x10 */ BCK_BESU_STEP,
    /* 0x11 */ BCK_BESU_WAIT_A,
    /* 0x12 */ BCK_BESU_WAIT_C,
    /* 0x13 */ BCK_BESU_YOKERU,

    /* BTK */
    /* 0x16 */ BTK_BESU = 0x16,

    /* BTP */
    /* 0x19 */ BTP_BESU = 0x19,
    /* 0x1A */ BTP_BESU_F_YOKERU,
    /* 0x1B */ BTP_BESU_FH_BIKKURI,
};

enum Besu1_RES_File_ID {
    /* BCK */
    /* 0x05 */ BCK_BESU_F_MADTALK = 0x5,
    /* 0x06 */ BCK_BESU_F_TALK_A,
    /* 0x07 */ BCK_BESU_F_TALK_B,
    /* 0x08 */ BCK_BESU_F_WAIT_B,
    /* 0x09 */ BCK_BESU_FH_MADTALK,
    /* 0x0A */ BCK_BESU_FH_TALK_A,
    /* 0x0B */ BCK_BESU_FH_TALK_B,
    /* 0x0C */ BCK_BESU_RUN,
    /* 0x0D */ BCK_BESU_TALK_A,
    /* 0x0E */ BCK_BESU_TALK_B,
    /* 0x0F */ BCK_BESU_WAIT_B,

    /* BTP */
    /* 0x12 */ BTP_BESU_F_MADTALK = 0x12,
    /* 0x13 */ BTP_BESU_F_TALK_A,
    /* 0x14 */ BTP_BESU_F_TALK_B,
    /* 0x15 */ BTP_BESU_F_WAIT_B,
    /* 0x16 */ BTP_BESU_FH_MADTALK,
    /* 0x17 */ BTP_BESU_FH_TALK_A,
    /* 0x18 */ BTP_BESU_FH_TALK_B,

    /* EVT */
    /* 0x1B */ BESU1_EVT_EVENT_LIST = 0x1B,
};

enum Besu2_RES_File_ID {
    /* BCK */
    /* 0x06 */ BCK_BESU_AMAZED_A = 0x6,
    /* 0x07 */ BCK_BESU_AMAZED_B,
    /* 0x08 */ BCK_BESU_AMAZED_C,
    /* 0x09 */ BCK_BESU_CUP,
    /* 0x0A */ BCK_BESU_CUP_A,
    /* 0x0B */ BCK_BESU_CUP_B,
    /* 0x0C */ BCK_BESU_F_AMAZED_A,
    /* 0x0D */ BCK_BESU_F_AMAZED_B,
    /* 0x0E */ BCK_BESU_F_AMAZED_C,
    /* 0x0F */ BCK_BESU_F_CUP_A,
    /* 0x10 */ BCK_BESU_F_CUP_B,
    /* 0x11 */ BCK_BESU_F_GLARE_A,
    /* 0x12 */ BCK_BESU_F_GLARE_WAIT,
    /* 0x13 */ BCK_BESU_F_MOP_A,
    /* 0x14 */ BCK_BESU_F_MOP_B,
    /* 0x15 */ BCK_BESU_F_MOP_C,
    /* 0x16 */ BCK_BESU_F_NURSE_A,
    /* 0x17 */ BCK_BESU_F_TALK_N_A,
    /* 0x18 */ BCK_BESU_F_TALK_N_B,
    /* 0x19 */ BCK_BESU_F_TALK_N_C,
    /* 0x1A */ BCK_BESU_GLARE_A,
    /* 0x1B */ BCK_BESU_GLARE_WAIT,
    /* 0x1C */ BCK_BESU_MOP_A,
    /* 0x1D */ BCK_BESU_MOP_B,
    /* 0x1E */ BCK_BESU_MOP_C,
    /* 0x1F */ BCK_BESU_NURSE_A,
    /* 0x20 */ BCK_BESU_TALK_N_A,
    /* 0x21 */ BCK_BESU_TALK_N_B,
    /* 0x22 */ BCK_BESU_TALK_N_C,

    /* BMDR */
    /* 0x25 */ BMDR_BESU_CLOTH = 0x25,
    /* 0x26 */ BMDR_BESU_CUP,
    /* 0x27 */ BMDR_BESUB,

    /* BTP */
    /* 0x2A */ BTP_BESU_F_AMAZED_A = 0x2A,
    /* 0x2B */ BTP_BESU_F_AMAZED_B,
    /* 0x2C */ BTP_BESU_F_AMAZED_C,
    /* 0x2D */ BTP_BESU_F_CUP_A,
    /* 0x2E */ BTP_BESU_F_CUP_B,
    /* 0x2F */ BTP_BESU_F_GLARE_A,
    /* 0x30 */ BTP_BESU_F_GLARE_WAIT,
    /* 0x31 */ BTP_BESU_F_MOP_A,
    /* 0x32 */ BTP_BESU_F_MOP_B,
    /* 0x33 */ BTP_BESU_F_MOP_C,
    /* 0x34 */ BTP_BESU_F_NURSE_A,
    /* 0x35 */ BTP_BESU_F_TALK_N_A,
    /* 0x36 */ BTP_BESU_F_TALK_N_B,
    /* 0x37 */ BTP_BESU_F_TALK_N_C,

    /* EVT */
    /* 0x3A */ BESU2_EVT_EVENT_LIST = 0x3A,
};

enum Besu3_RES_file_ID {
    /* BCK */
    /* 0x07 */ BCK_BESU3_F_WORRY = 0x7,
    /* 0x08 */ BCK_BESU3_FH_WORRY,
    /* 0x09 */ BCK_BESU_F_SURPRISE,
    /* 0x0A */ BCK_BESU_FH_SURPRISE,
    /* 0x0B */ BCK_BESU_SURPRISE,

    /* BMDR */
    /* 0x0E */ BMDR_BESU3 = 0xE,

    /* BTK */
    /* 0x11 */ BTK_BESU_SURPRISE = 0x11,

    /* BTP */
    /* 0x14 */ BTP_BESU3 = 0x14,
    /* 0x15 */ BTP_BESU3_F_SURPRISE,
    /* 0x16 */ BTP_BESU3_F_WORRY,
    /* 0x17 */ BTP_BESU3_FH_SURPRISE,
    /* 0x18 */ BTP_BESU3_FH_WORRY,

    /* EVT */
    /* 0x1B */ BESU3_EVT_EVENT_LIST = 0x1B,
};

enum Besu_p1_RES_File_ID {
    /* BCK */
    /* 0x05 */ BESU_P1_BCK_BESU_F_MADTALK = 0x5,
    /* 0x06 */ BESU_P1_BCK_BESU_F_TALK_A,
    /* 0x07 */ BESU_P1_BCK_BESU_F_TALK_C,
    /* 0x08 */ BESU_P1_BCK_BESU_F_YOKERU,
    /* 0x09 */ BESU_P1_BCK_BESU_FH_MADTALK,
    /* 0x0A */ BESU_P1_BCK_BESU_NORMAL_TALK_B,
    /* 0x0B */ BESU_P1_BCK_BESU_STEP,
    /* 0x0C */ BESU_P1_BCK_BESU_WAIT_A,
    /* 0x0D */ BESU_P1_BCK_BESU_YOKERU,

    /* BTK */
    /* 0x10 */ BESU_P1_BTK_BESU = 0x10,

    /* BTP */
    /* 0x13 */ BESU_P1_BTP_BESU = 0x13,
    /* 0x14 */ BESU_P1_BTP_BESU_F_MADTALK,
    /* 0x15 */ BESU_P1_BTP_BESU_F_YOKERU,
    /* 0x16 */ BESU_P1_BTP_BESU_FH_MADTALK,
};

enum RES_Name {
    /* 0x1 */ BESU = 0x1,
    /* 0x2 */ BESU_TW,
    /* 0x3 */ BESU0,
    /* 0x4 */ BESU1,
    /* 0x5 */ BESU2,
    /* 0x6 */ BESU3,
};

enum FaceMotion {
    /* 0x00 */ FACE_TALK_C,
    /* 0x01 */ FACE_TALK_A,
    /* 0x02 */ FACE_TALK_B,
    /* 0x03 */ FACE_TALK_N_B,
    /* 0x04 */ FACE_MOP_A_C,
    /* 0x05 */ FACE_MOP_A_B,
    /* 0x06 */ FACE_MOP_C,
    /* 0x07 */ FACE_NURSE_A,
    /* 0x08 */ FACE_MADTALK,
    /* 0x09 */ FACE_CUP_A,
    /* 0x0A */ FACE_SURPISE,
    /* 0x0B */ FACE_WORRY,
    /* 0x0C */ FACE_YOKERU,
    /* 0x0D */ FACE_H_TALK_A,
    /* 0x0E */ FACE_H_TALK_B,
    /* 0x0F */ FACE_H_MADTALK,
    /* 0x10 */ FACE_TALK_N_A,
    /* 0x11 */ FACE_TALK_N_C,
    /* 0x12 */ FACE_GLARE_A,
    /* 0x13 */ FACE_H_SURPRISE,
    /* 0x14 */ FACE_AMAZED_A,
    /* 0x15 */ FACE_AMAZED_C,
    /* 0x16 */ FACE_CUP_B,
    /* 0x17 */ FACE_DEMO_WAIT,
    /* 0x18 */ FACE_DEMO_FEAR,
    /* 0x19 */ FACE_GLARE_WAIT,
    /* 0x1A */ FACE_H_WORRY,
    /* 0x1B */ FACE_H_BIKKURI,
    /* 0x1C */ FACE_NONE,
};

enum Motion {
    /* 0x00 */ MOT_WAIT_A,
    /* 0x01 */ MOT_WAIT_B,
    /* 0x02 */ MOT_TALK_A,
    /* 0x03 */ MOT_TALK_A_2,
    /* 0x04 */ MOT_TALK_N_B,
    /* 0x05 */ MOT_CUP_A,
    /* 0x06 */ MOT_SURPRISE,
    /* 0x07 */ MOT_WAIT_C,
    /* 0x08 */ MOT_NORMAL_WAIT,
    /* 0x09 */ MOT_NORMAL_TALK,
    /* 0x0A */ MOT_MAD,
    /* 0x0B */ MOT_NORMAL_TALK_B,
    /* 0x0C */ MOT_BODY_TALK,
    /* 0x0D */ MOT_NURSE_A,
    /* 0x0E */ MOT_MOP_A_C,
    /* 0x0F */ MOT_MOP_A_B,
    /* 0x10 */ MOT_MOP_C,
    /* 0x11 */ MOT_STEP,
    /* 0x12 */ MOT_YOKERU,
    /* 0x13 */ MOT_TALK_N_A,
    /* 0x14 */ MOT_TALK_N_C,
    /* 0x15 */ MOT_GLARE_A,
    /* 0x16 */ MOT_AMAZED_A,
    /* 0x17 */ MOT_AMAZED_C,
    /* 0x18 */ MOT_CUP_B,
    /* 0x19 */ MOT_DEMO_WAIT,
    /* 0x1A */ MOT_DEMO_FEAR,
    /* 0x1B */ MOT_GLARE_WAIT,
    /* 0x1C */ MOT_RUN,
    /* 0x1D */ MOT_NORMAL_STEP,
};

enum Event {
    /* 0x0 */ EVENT_NONE,
    /* 0x1 */ EVENT_NO_RESPONSE,
    /* 0x2 */ EVENT_CONVERSATION_ABOUT_SARU01,
    /* 0x3 */ EVENT_CONVERSATION_ABOUT_SARU02,
    /* 0x4 */ EVENT_CONVERSATION_ABOUT_PACHINKO1,
    /* 0x5 */ EVENT_CONVERSATION_ABOUT_PACHINKO2,
    /* 0x6 */ EVENT_CONVERSATION_ABOUT_PACHINKO3,
    /* 0x7 */ EVENT_CACARICO_CONVERSATION,
    /* 0x8 */ EVENT_DEMO13_STB,
    /* 0x9 */ EVENT_SURPRISE,
    /* 0xA */ EVENT_CONVERSATION_ABOUT_DEATHMT,
    /* 0xB */ EVENT_NURSE,
    /* 0xC */ EVENT_CONVERSATION_ABOUT_ZORA,
};

#if !DEBUG
#endif


static int l_bmdData[6][2] = {
    {3, 1},
    {11, 2},
    {37, 5},
    {39, 5},
    {14, 6},
    {38, 5},
};

static daNpcT_evtData_c l_evtList[13] = {
    {"", 0},
    {"NO_RESPONSE", 0},
    {"CONVERSATION_ABOUT_SARU01", 11},
    {"CONVERSATION_ABOUT_SARU02", 11},
    {"CONVERSATION_ABOUT_PACHINKO1", 4},
    {"CONVERSATION_ABOUT_PACHINKO2", 4},
    {"CONVERSATION_ABOUT_PACHINKO3", 4},
    {"CACARICO_CONVERSATION", 9},
    {"DEMO13_STB", 0},
    {"SURPRISE", 6},
    {"CONVERSATION_ABOUT_DEATHMT", 5},
    {"NURSE", 10},
    {"CONVERSATION_ABOUT_ZORA", 5},
};

static char* l_resNameList[12] = {
    "",
    "Besu",
    "Besu_TW",
    "Besu0",
    "Besu1",
    "Besu2",
    "Besu3",
    "Besu_p1",
    "Len1",
    "Taro1",
    "Lud2",
    "Sera",
};

static s8 l_loadResPtrn0[4] = {
    1, 3, 4, -1
};

static s8 l_loadResPtrn1[3] = {
    3, 2, -1,
};

static s8 l_loadResPtrn2[3] = {
    3, 5, -1,
};

static s8 l_loadResPtrn3[3] = {
    3, 6, -1,
};

static s8 l_loadResPtrn4[4] = {3, 4, 6, -1};

static s8 l_loadResPtrn5[3] = {
    1, 7, -1,
};

static s8 l_loadResPtrn9[7] = {
    1, 2, 3, 4, 5, 6, -1,
};

static s8* l_loadResPtrnList[19] = {
    l_loadResPtrn0, l_loadResPtrn0, l_loadResPtrn1, l_loadResPtrn4,
    l_loadResPtrn0, l_loadResPtrn3, l_loadResPtrn2, l_loadResPtrn2,
    l_loadResPtrn2, l_loadResPtrn0, l_loadResPtrn0, l_loadResPtrn0,
    l_loadResPtrn0, l_loadResPtrn0, l_loadResPtrn0, l_loadResPtrn4,
    l_loadResPtrn0, l_loadResPtrn5, l_loadResPtrn9,
};

static daNpcT_faceMotionAnmData_c l_faceMotionAnmData[30] = {
    {-1, J3DFrameCtrl::EMode_NONE, 0, BTP_BESU, J3DFrameCtrl::EMode_LOOP, BESU0, 1},
    {BCK_BESU_F_TALK_A, J3DFrameCtrl::EMode_NONE, BESU1, BTP_BESU_F_TALK_A, J3DFrameCtrl::EMode_NONE, BESU1, 0},
    {BCK_BESU_F_TALK_B, J3DFrameCtrl::EMode_NONE, BESU1, BTP_BESU_F_TALK_B, J3DFrameCtrl::EMode_NONE, BESU1, 0},
    {BCK_BESU_F_TALK_C, J3DFrameCtrl::EMode_NONE, BESU0, BTP_BESU, J3DFrameCtrl::EMode_LOOP, BESU0, 1},
    {BCK_BESU_F_MADTALK, J3DFrameCtrl::EMode_NONE, BESU1, BTP_BESU_F_MADTALK, J3DFrameCtrl::EMode_NONE, BESU1, 0},
    {BCK_BESU_FH_TALK_A, J3DFrameCtrl::EMode_LOOP, BESU1, BTP_BESU_FH_TALK_A, J3DFrameCtrl::EMode_LOOP, BESU1, 0},
    {BCK_BESU_FH_TALK_B, J3DFrameCtrl::EMode_LOOP, BESU1, BTP_BESU_FH_TALK_B, J3DFrameCtrl::EMode_LOOP, BESU1, 0},
    {BCK_BESU_FH_MADTALK, J3DFrameCtrl::EMode_LOOP, BESU1, BTP_BESU_FH_MADTALK, J3DFrameCtrl::EMode_LOOP, BESU1, 0},
    {BCK_BESU_F_YOKERU, J3DFrameCtrl::EMode_NONE, BESU0, BTP_BESU_F_YOKERU, J3DFrameCtrl::EMode_NONE, BESU0, 0},
    {BCK_BESU_F_NURSE_A, J3DFrameCtrl::EMode_LOOP, BESU2, BTP_BESU_F_NURSE_A, J3DFrameCtrl::EMode_LOOP, BESU2, 0},
    {BCK_BESU_F_MOP_A, J3DFrameCtrl::EMode_NONE, BESU2, BTP_BESU_F_MOP_A, J3DFrameCtrl::EMode_NONE, BESU2, 0},
    {BCK_BESU_F_MOP_B, J3DFrameCtrl::EMode_LOOP, BESU2, BTP_BESU_F_MOP_B, J3DFrameCtrl::EMode_LOOP, BESU2, 0},
    {BCK_BESU_F_MOP_C, J3DFrameCtrl::EMode_NONE, BESU2, BTP_BESU_F_MOP_C, J3DFrameCtrl::EMode_NONE, BESU2, 0},
    {BCK_BESU_F_TALK_N_A, J3DFrameCtrl::EMode_NONE, BESU2, BTP_BESU_F_TALK_N_A, J3DFrameCtrl::EMode_NONE, BESU2, 0},
    {BCK_BESU_F_TALK_N_B, J3DFrameCtrl::EMode_LOOP, BESU2, BTP_BESU_F_TALK_N_B, J3DFrameCtrl::EMode_LOOP, BESU2, 0},
    {BCK_BESU_F_TALK_N_C, J3DFrameCtrl::EMode_NONE, BESU2, BTP_BESU_F_TALK_N_C, J3DFrameCtrl::EMode_NONE, BESU2, 0},
    {BCK_BESU_F_GLARE_A, J3DFrameCtrl::EMode_NONE, BESU2, BTP_BESU_F_GLARE_A, J3DFrameCtrl::EMode_NONE, BESU2, 0},
    {BCK_BESU_F_CUP_A, J3DFrameCtrl::EMode_LOOP, BESU2, BTP_BESU_F_CUP_A, J3DFrameCtrl::EMode_LOOP, BESU2, 0},
    {BCK_BESU_F_CUP_B, J3DFrameCtrl::EMode_LOOP, BESU2, BTP_BESU_F_CUP_B, J3DFrameCtrl::EMode_LOOP, BESU2, 0},
    {BCK_BESU_F_SURPRISE, J3DFrameCtrl::EMode_NONE, BESU3, BTP_BESU3_F_SURPRISE, J3DFrameCtrl::EMode_NONE, BESU3, 0},
    {BCK_BESU_FH_SURPRISE, J3DFrameCtrl::EMode_LOOP, BESU3, BTP_BESU3_FH_SURPRISE, J3DFrameCtrl::EMode_LOOP, BESU3, 0},
    {BCK_BESU3_F_WORRY, J3DFrameCtrl::EMode_NONE, BESU3, BTP_BESU3_F_WORRY, J3DFrameCtrl::EMode_NONE, BESU3, 0},
    {BCK_BESU3_FH_WORRY, J3DFrameCtrl::EMode_LOOP, BESU3, BTP_BESU3_FH_WORRY, J3DFrameCtrl::EMode_LOOP, BESU3, 0},
    {BCK_BESU_F_AMAZED_A, J3DFrameCtrl::EMode_NONE, BESU2, BTP_BESU_F_AMAZED_A, J3DFrameCtrl::EMode_NONE, BESU2, 0},
    {BCK_BESU_F_AMAZED_B, J3DFrameCtrl::EMode_LOOP, BESU2, BTP_BESU_F_AMAZED_B, J3DFrameCtrl::EMode_LOOP, BESU2, 0},
    {BCK_BESU_F_AMAZED_C, J3DFrameCtrl::EMode_NONE, BESU2, BTP_BESU_F_AMAZED_C, J3DFrameCtrl::EMode_NONE, BESU2, 0},
    {BCK_BESU_F_DEMO_WAIT, J3DFrameCtrl::EMode_LOOP, BESU_TW, BTP_BESU_F_DEMO_WAIT, J3DFrameCtrl::EMode_LOOP, BESU_TW, 0},
    {BCK_BESU_F_DEMO_FEAR, J3DFrameCtrl::EMode_LOOP, BESU_TW, BTP_BESU_F_DEMO_FEAR, J3DFrameCtrl::EMode_LOOP, BESU_TW, 0},
    {BCK_BESU_F_GLARE_WAIT, J3DFrameCtrl::EMode_LOOP, BESU2, BTP_BESU_F_GLARE_WAIT, J3DFrameCtrl::EMode_LOOP, BESU2, 0},
    {BCK_BESU_FH_BIKKURI, J3DFrameCtrl::EMode_LOOP, BESU0, BTP_BESU_FH_BIKKURI, J3DFrameCtrl::EMode_LOOP, BESU0, 0},
};

static daNpcT_motionAnmData_c l_motionAnmData[33] = {
    {BCK_BESU_WAIT_A, J3DFrameCtrl::EMode_LOOP, BESU0, BTK_BESU, J3DFrameCtrl::EMode_NONE, BESU0, 1, 0},
    {BCK_BESU_WAIT_B, J3DFrameCtrl::EMode_LOOP, BESU1, BTK_BESU, J3DFrameCtrl::EMode_NONE, BESU0, 1, 0},
    {BCK_BESU_WAIT_C, J3DFrameCtrl::EMode_LOOP, BESU0, BTK_BESU, J3DFrameCtrl::EMode_NONE, BESU0, 1, 0},
    {BCK_BESU_NORMAL_WAIT, J3DFrameCtrl::EMode_LOOP, BESU0, BTK_BESU, J3DFrameCtrl::EMode_NONE, BESU0, 1, 0},
    {BCK_BESU_TALK_A, J3DFrameCtrl::EMode_NONE, BESU1, BTK_BESU, J3DFrameCtrl::EMode_NONE, BESU0, 1, 0},
    {BCK_BESU_TALK_B, J3DFrameCtrl::EMode_NONE, BESU1, BTK_BESU, J3DFrameCtrl::EMode_NONE, BESU0, 1, 0},
    {BCK_BESU_NORMAL_TALK, J3DFrameCtrl::EMode_NONE, BESU0, BTK_BESU, J3DFrameCtrl::EMode_NONE, BESU0, 1, 0},
    {BCK_BESU_YOKERU, J3DFrameCtrl::EMode_NONE, BESU0, BTK_BESU, J3DFrameCtrl::EMode_NONE, BESU0, 1, 0},
    {BCK_BESU_STEP, J3DFrameCtrl::EMode_NONE, BESU0, BTK_BESU, J3DFrameCtrl::EMode_NONE, BESU0, 1, 0},
    {BCK_BESU_NORMAL_STEP, J3DFrameCtrl::EMode_NONE, BESU0, BTK_BESU, J3DFrameCtrl::EMode_NONE, BESU0, 1, 0},
    {BCK_BESU_NURSE_A, J3DFrameCtrl::EMode_LOOP, BESU2, BTK_BESU, J3DFrameCtrl::EMode_NONE, BESU0, 1, 0},
    {BCK_BESU_MOP_A, J3DFrameCtrl::EMode_NONE, BESU2, BTK_BESU, J3DFrameCtrl::EMode_NONE, BESU0, 0, 0},
    {BCK_BESU_MOP_B, J3DFrameCtrl::EMode_LOOP, BESU2, BTK_BESU, J3DFrameCtrl::EMode_NONE, BESU0, 0, 0},
    {BCK_BESU_MOP_C, J3DFrameCtrl::EMode_NONE, BESU2, BTK_BESU, J3DFrameCtrl::EMode_NONE, BESU0, 0, 0},
    {BCK_BESU_TALK_N_A, J3DFrameCtrl::EMode_NONE, BESU2, BTK_BESU, J3DFrameCtrl::EMode_NONE, BESU0, 0, 0},
    {BCK_BESU_TALK_N_B, J3DFrameCtrl::EMode_LOOP, BESU2, BTK_BESU, J3DFrameCtrl::EMode_NONE, BESU0, 0, 0},
    {BCK_BESU_TALK_N_C, J3DFrameCtrl::EMode_NONE, BESU2, BTK_BESU, J3DFrameCtrl::EMode_NONE, BESU0, 0, 0},
    {BCK_BESU_GLARE_A, J3DFrameCtrl::EMode_NONE, BESU2, BTK_BESU, J3DFrameCtrl::EMode_NONE, BESU0, 0, 0},
    {BCK_BESU_CUP_A, J3DFrameCtrl::EMode_LOOP, BESU2, BTK_BESU, J3DFrameCtrl::EMode_NONE, BESU0, 0, 0},
    {BCK_BESU_CUP_B, J3DFrameCtrl::EMode_LOOP, BESU2, BTK_BESU, J3DFrameCtrl::EMode_NONE, BESU0, 0, 0},
    {BCK_BESU_SURPRISE, J3DFrameCtrl::EMode_LOOP, BESU3, BTK_BESU_SURPRISE, J3DFrameCtrl::EMode_LOOP, BESU3, 0, 0},
    {BCK_BESU_AMAZED_A, J3DFrameCtrl::EMode_NONE, BESU2, BTK_BESU, J3DFrameCtrl::EMode_NONE, BESU0, 0, 0},
    {BCK_BESU_AMAZED_B, J3DFrameCtrl::EMode_LOOP, BESU2, BTK_BESU, J3DFrameCtrl::EMode_NONE, BESU0, 0, 0},
    {BCK_BESU_AMAZED_C, J3DFrameCtrl::EMode_NONE, BESU2, BTK_BESU, J3DFrameCtrl::EMode_NONE, BESU0, 0, 0},
    {BCK_BESU_DEMO_WAIT, J3DFrameCtrl::EMode_LOOP, BESU_TW, -1, J3DFrameCtrl::EMode_NONE, 0, 0, 0},
    {BCK_BESU_DEMO_FEAR, J3DFrameCtrl::EMode_LOOP, BESU_TW, -1, J3DFrameCtrl::EMode_NONE, 0, 0, 0},
    {BCK_BESU_GLARE_WAIT, J3DFrameCtrl::EMode_LOOP, BESU2, BTK_BESU, J3DFrameCtrl::EMode_NONE, BESU0, 0, 0},
    {BCK_BESU_RUN, J3DFrameCtrl::EMode_LOOP, BESU1, BTK_BESU, J3DFrameCtrl::EMode_NONE, BESU0, 0, 0},
    {BCK_BESU_MAD, J3DFrameCtrl::EMode_NONE, BESU0, BTK_BESU, J3DFrameCtrl::EMode_NONE, BESU0, 1, 0},
    {BCK_BESU_MAD_WAIT, J3DFrameCtrl::EMode_LOOP, BESU0, BTK_BESU, J3DFrameCtrl::EMode_NONE, BESU0, 1, 0},
    {BCK_BESU_NORMAL_TALK_B, J3DFrameCtrl::EMode_NONE, BESU0, BTK_BESU, J3DFrameCtrl::EMode_NONE, BESU0, 1, 0},
    {BCK_BESU_BODY_TALK, J3DFrameCtrl::EMode_NONE, BESU0, BTK_BESU, J3DFrameCtrl::EMode_NONE, BESU0, 1, 0},
    {BCK_BESU_BODY_WAIT, J3DFrameCtrl::EMode_LOOP, BESU0, BTK_BESU, J3DFrameCtrl::EMode_NONE, BESU0, 1, 0},
};

static daNpcT_MotionSeqMngr_c::sequenceStepData_c l_faceMotionSequenceData[116] = {
    {3, -1, 1}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, 
    {1, -1, 1}, {5, -1, 0}, {-1, 0, 0}, {-1, 0, 0},
    {2, -1, 1}, {6, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, 
    {14, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {10, 0, 1}, {12, 0, 1}, {-1, 0, 0}, {-1, 0, 0}, 
    {10, 0, 1}, {11, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {12, 0, 1}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, 
    {9, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {4, -1, 1}, {7, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, 
    {17, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {19, -1, 1}, {20, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, 
    {21, -1, 1}, {22, -1, 0}, {-1, 0, 0}, {-1, 0, 0},
    {8, -1, 1}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, 
    {5, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {6, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, 
    {7, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {13, -1, 1}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, 
    {15, -1, 1}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {16, -1, 1}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, 
    {20, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {23, -1, 1}, {24, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, 
    {25, -1, 1}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {18, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, 
    {26, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {27, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, 
    {28, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {22, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, 
    {29, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {0, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
};

static daNpcT_MotionSeqMngr_c::sequenceStepData_c l_motionSequenceData[120] = {
    {0, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {1, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {4, -1, 1}, {1, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, 
    {4, -1, 1}, {1, -1, 0}, {-1, 0, 0}, {-1, 0, 0},
    {15, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, 
    {18, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {20, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, 
    {2, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {3, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, 
    {6, -1, 1}, {3, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {28, -1, 1}, {29, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, 
    {30, -1, 1}, {0, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {31, -1, 1}, {32, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, 
    {10, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {11, 0, 1}, {13, 0, 1}, {-1, 0, 0}, {-1, 0, 0}, 
    {11, 0, 1}, {12, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {13, 0, 1}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, 
    {8, 4, 1}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {7, -1, 1}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, 
    {14, -1, 1}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {16, -1, 1}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, 
    {17, -1, 1}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {21, -1, 1}, {22, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, 
    {23, -1, 1}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {19, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, 
    {24, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {25, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, 
    {26, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {27, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, 
    {9, 4, 1}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
};

char* daNpc_Besu_c::mCutNameList[15] = {
    "",
    "CONVERSATION_ABOUT_SARU",
    "CONVERSATION_ABOUT_PACHINKO",
    "CONVERSATION_ABOUT_WOODSWD",
    "SWDTUTORIAL",
    "FIND_MONKEY",
    "GET_WOODSWD",
    "CACARICO_CONVERSATION",
    "SURPRISE",
    "CONVERSATION_IN_HOTEL",
    "CONVERSATION_ABOUT_DEATHMT",
    "CONVERSATION_ABOUT_GORON",
    "NURSE",
    "CLOTH_TRY",
    "THANK_YOU",
};

daNpc_Besu_c::cutFunc daNpc_Besu_c::mCutList[15] = {
    NULL,
    &daNpc_Besu_c::cutConversationAboutSaru,
    &daNpc_Besu_c::cutConversationAboutPachinko,
    &daNpc_Besu_c::cutConversationAboutWoodSwd,
    &daNpc_Besu_c::cutSwdTutorial,
    &daNpc_Besu_c::cutFindMonkey,
    &daNpc_Besu_c::cutGetWoodSwd,
    &daNpc_Besu_c::cutCacaricoConversation,
    &daNpc_Besu_c::cutSurprise,
    &daNpc_Besu_c::cutConversationInHotel,
    &daNpc_Besu_c::cutConversationAboutDeathMt,
    &daNpc_Besu_c::cutConversationAboutGoron,
    &daNpc_Besu_c::cutNurse,
    &daNpc_Besu_c::cutClothTry,
    &daNpc_Besu_c::cutThankYou,
};

daNpc_Besu_c::~daNpc_Besu_c() {
    // "Destruct":
    OS_REPORT("|%06d:%x|daNpc_Besu_c -> デストラクト\n", g_Counter.mCounter0, this);
    if (mpMorf[0] != NULL) {
        mpMorf[0]->stopZelAnime();
    }

    if (mpCupModelMorf != NULL) {
        mpCupModelMorf->stopZelAnime();
    }

// #if DEBUG
//     if (field_0xe40 != NULL) {
//         field_0xe40->removeHIO();
//     }
// #endif

    deleteRes((l_loadResPtrnList)[mType], (const char**)l_resNameList);
}

const daNpc_Besu_HIOParam daNpc_Besu_Param_c::m = {
    160.0f,
    -3.0f,
    1.0f,
    400.0f,
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
    3,
    6,
    5,
    6,
    110.0f,
    0.0f,
    0.0f,
    0.0f,
    60,
    8,
    0,
    0,
    0,
    0,
    0,
    4.0f,
    -15.0f,
    0.0f,
    -15.0f,
    15.0f,
    30.0f,
    15.0f,
    70.0f,
    1.0f,
};

int daNpc_Besu_c::create() {
    static int const heapSize[19] = {
        0x35C0, 0x35C0, 0x3510, 0x35D0,
        0x35D0, 0x35D0, 0x3DC0, 0x3EC0,
        0x3DD0, 0x35C0, 0x35C0, 0x35D0,
        0x35C0, 0x35C0, 0x35D0, 0x35C0,
        0x35C0, 0x35C0, 0x0,
    };
    daNpcT_ct(this, daNpc_Besu_c, l_faceMotionAnmData, l_motionAnmData,
                       l_faceMotionSequenceData, 4, l_motionSequenceData, 4,
                       l_evtList, l_resNameList
    );
    mType = getType();
    mFlowNodeNo = getFlowNodeNo();
    mTwilight = dKy_darkworld_check();
    if (mType == 9) {
        mTwilight = false;
    }

    cPhs__Step rv = (cPhs__Step) loadRes(l_loadResPtrnList[mType], (const char**)l_resNameList);
    if (rv == cPhs_COMPLEATE_e) {
        OS_REPORT("\t(%s:%d) flowNo:%d, PathID:%02x, BitSW:%02x<%08x> ",
                  fopAcM_getProcNameString(this), mType, mFlowNodeNo, getPathID(),
                  getBitSW(), fopAcM_GetParam(this));
        if (isDelete()) {
            OS_REPORT("===>isDelete:TRUE\n");
            return cPhs_ERROR_e;
        }
        OS_REPORT("\n");

        if (fopAcM_entrySolidHeap(this, createHeapCallBack, heapSize[mType]) == 0) {
            return cPhs_ERROR_e;
        }

        J3DModelData* modelData_p = mpMorf[0]->getModel()->getModelData();
        fopAcM_SetMtx(this, mpMorf[0]->getModel()->getBaseTRMtx());
        fopAcM_setCullSizeBox(this, -200.0f, -100.0f, -200.0f, 200.0f, 300.0f, 200.0f);
        mSound.init(&current.pos, &eyePos, 3, 1);

// #if DEBUG
//         field_0xe40 = &l_HIO;
//         // "Beth":
//         field_0xe40->entryHIO("ベス");
// #endif

        reset();
        mAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1,
                            &mAcchCir, fopAcM_GetSpeed_p(this), fopAcM_GetAngle_p(this), fopAcM_GetShapeAngle_p(this));
        mCcStts.Init(daNpc_Besu_Param_c::m.common.weight, 0, this);
        mCyl1.Set(mCcDCyl);
        mCyl1.SetStts(&mCcStts);
        mCyl1.SetTgHitCallback(tgHitCallBack);
        mCyl2.Set(mCcDCyl);
        mCyl2.SetStts(&mCcStts);
        mAcch.CrrPos(dComIfG_Bgsp());
        mGndChk = mAcch.m_gnd;
        mGroundH = mAcch.GetGroundH();
        if (mGroundH != -G_CM3D_F_INF) {
            setEnvTevColor();
            setRoomNo();
        }
        mCreating = true;
        Execute();
        mCreating = false;
    }

    return rv;
}

int daNpc_Besu_c::CreateHeap() {
    J3DModelData* modelData = NULL;
    J3DModel* model = NULL;
    int bmdIdx = 0;
    if (chkBesu3()) {
        bmdIdx = 4;
    } else if (chkNurse()) {
        bmdIdx = 3;
    }

    if (mTwilight) {
        bmdIdx = 1;
    }

    int resname_idx = l_bmdData[bmdIdx][1];
    int obres_idx = l_bmdData[bmdIdx][0];
    modelData = static_cast<J3DModelData*>(
        dComIfG_getObjectRes(l_resNameList[resname_idx], obres_idx));
    if (modelData == NULL) {
        return 0;
    }

    u32 dbg_0x24 = 0x11020284;
    mpMorf[0] = new mDoExt_McaMorfSO(modelData, NULL, NULL, NULL, -1, 1.0f, 0, -1, &mSound, 0x80000,
                                     dbg_0x24);
    if (mpMorf[0] == NULL || mpMorf[0]->getModel() == NULL) {
        return 0;
    }

    model = (J3DModel*)mpMorf[0]->getModel();
    for (u16 i = 0; i < modelData->getJointNum(); i++) {
        modelData->getJointNodePointer(i)->setCallBack(ctrlJointCallBack);
    }
    model->setUserArea((uintptr_t)this);
    mpMatAnm[0] = new daNpcT_MatAnm_c();
    if (mpMatAnm[0] == NULL) {
        return 0;
    }

    if (mType == 7) {
        bmdIdx = 5;
        resname_idx = l_bmdData[bmdIdx][1];
        obres_idx = l_bmdData[bmdIdx][0];
        modelData = (J3DModelData*)dComIfG_getObjectRes(l_resNameList[resname_idx],
                                                        obres_idx);

        if (modelData == NULL) {
            return 0;
        }

        dbg_0x24 = 0x11000084;
        mpCupModelMorf = new mDoExt_McaMorfSO(modelData, NULL, NULL, NULL, -1, 1.0f,
                                          0, -1, NULL, 0x80000, dbg_0x24);
        if (mpCupModelMorf == NULL || mpCupModelMorf->getModel() == NULL) {
            return 0;
        }
    }

    if (mType == 6 || mType == 8) {
        if (l_bmdData[2][0] >= 0) {
            modelData = (J3DModelData*)dComIfG_getObjectRes(l_resNameList[l_bmdData[2][1]],
                l_bmdData[2][0]);
        } else {
            modelData = 0;
        }

        if (modelData != NULL) {
            mpClothModel[0] = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
        }

        if (mpClothModel[0] == NULL) {
            return 0;
        }
    }

    if (mTwilight) {
        if (setFaceMotionAnm(26, false) && setMotionAnm(24, 0.0f, 0)) {
            return 1;
        } else {
            return 0;
        }
    } else {
        if (setFaceMotionAnm(3, false) && setMotionAnm(0, 0, 0)) {
            return 1;
        } else {
            return 0;
        }
    }
}

int daNpc_Besu_c::Delete() {
    OS_REPORT("|%06d:%x|daNpc_Besu_c -> Delete\n", g_Counter, this);
    fopAcM_GetID(this);
    this->~daNpc_Besu_c();
    return 1;
}

int daNpc_Besu_c::Execute() {
    return execute();
}

int daNpc_Besu_c::Draw() {
    daNpcT_MatAnm_c* matAnm = mpMatAnm[0];
    if (matAnm != NULL) {
        J3DModelData* modelData = mpMorf[0]->getModel()->getModelData();
        J3DMaterial* material = modelData->getMaterialNodePointer(getEyeballMaterialNo());
        material->setMaterialAnm(matAnm);
    }

    return draw(FALSE, 0, mRealShadowSize, NULL, 100.0f, 0, 0, 0);
}

int daNpc_Besu_c::createHeapCallBack(fopAc_ac_c* i_this) {
    return static_cast<daNpc_Besu_c*>(i_this)->CreateHeap();
}

int daNpc_Besu_c::ctrlJointCallBack(J3DJoint* param_0, int param_1) {
    if (param_1 == 0) {
        J3DModel* model = j3dSys.getModel();
        daNpc_Besu_c* i_this = (daNpc_Besu_c*) model->getUserArea();
        if (i_this != NULL) {
            i_this->ctrlJoint(param_0, model);
        }
    }

    return 1;
}

u8 daNpc_Besu_c::getType() {
    switch (fopAcM_GetParam(this) & 0xFF) {
        case 0:
            return 0;
        case 1:
            return 1;
        case 2:
            return 2;
        case 3:
            return 3;
        case 4:
            return 4;
        case 5:
            return 5;
        case 6:
            return 6;
        case 7:
            return 7;
        case 8:
            return 8;
        case 9:
            return 9;
        case 10:
            return 0xA;
        case 11:
            return 0xB;
        case 12:
            return 0xC;
        case 13:
            return 0xD;
        case 14:
            return 0xE;
        case 15:
            return 0xF;
        case 16:
            return 0x10;
        case 17:
            return 0x11;
        default:
            return 0x12;
    }
}

BOOL daNpc_Besu_c::isDelete() {
    switch (mType) {
        case 0:
                    /* dSv_event_flag_c::F_0014 - Ordon Village - sword tutorial ends */
            return !daNpcT_chkEvtBit(0x12) ||
                    /* dSv_event_flag_c::F_0026 - Ordon Village - gave wooden sword to talo on 3rd day */
                    daNpcT_chkEvtBit(0x1F);

        case 1:
                   /* dSv_event_flag_c::F_0014 - Ordon Village - sword tutorial ends */
            return daNpcT_chkEvtBit(0x12) == 0;

        case 2:
            return 0;

        case 3:
                    /* dSv_event_flag_c::M_028 - Cutscene - [cutscene: 14] restore mountain spirit - Reuinion with Colin et al. */
            return !daNpcT_chkEvtBit(0x3D) ||
                    /* dSv_event_flag_c::F_0066 - Death Mountain - First saw Goron cutscene on mountain path */
                    daNpcT_chkEvtBit(0xA4);

        case 4:
                    /* dSv_event_flag_c::F_0066 - Death Mountain - First saw Goron cutscene on mountain path */
            return !daNpcT_chkEvtBit(0xA4) ||
                    /* dSv_event_flag_c::M_020 - Cutscene - [cutscene: ] Colin kidnapped : ON once watched */
                    daNpcT_chkEvtBit(0x35);

        case 5:
                    /* dSv_event_flag_c::M_020 - Cutscene - [cutscene: ] Colin kidnapped : ON once watched */
            return !daNpcT_chkEvtBit(0x35) ||
                    /* dSv_event_flag_c::M_052 - Main Event - Horseback battle clear */
                    daNpcT_chkEvtBit(0x55);

        case 6:
                    /* dSv_event_flag_c::M_052 - Main Event - Horseback battle clear */
            return !daNpcT_chkEvtBit(0x55) ||
                    /* dSv_event_flag_c::M_031 - Goron Mines - Goron Mines clear */
                    daNpcT_chkEvtBit(0x40);

        case 7:
                    /* dSv_event_flag_c::M_031 - Goron Mines - Goron Mines clear */
            return !daNpcT_chkEvtBit(0x40) ||
                    /* dSv_event_flag_c::M_035 - Cutscene - [cutscene: 35] after carriage guarding event */
                    daNpcT_chkEvtBit(0x44);

        case 8:
                    /* dSv_event_flag_c::M_035 - Cutscene - [cutscene: 35] after carriage guarding event */
            return !daNpcT_chkEvtBit(0x44) ||
                    /* dSv_event_flag_c::F_0264 - Cutscene - Get master sword */
                    daNpcT_chkEvtBit(0x108);

        case 9:
            return 0;

        case 10:
                   /* dSv_event_flag_c::F_0264 - Cutscene - Get master sword */
            return daNpcT_chkEvtBit(0x108) == 0;

        case 11:
                   /* dSv_event_flag_c::F_0015 - Ordon Village - Slingshot tutorial ends */
            return daNpcT_chkEvtBit(0x13);

        case 12:
                    /* dSv_event_flag_c::F_0015 - Ordon Village - Slingshot tutorial ends */
            return !daNpcT_chkEvtBit(0x13) ||
                    /* dSv_event_flag_c::F_0014 - Ordon Village - sword tutorial ends */
                    daNpcT_chkEvtBit(0x12);

        case 13:
                    /* dSv_event_flag_c::F_0014 - Ordon Village - sword tutorial ends */
            return !daNpcT_chkEvtBit(0x12) ||
                    /* dSv_event_flag_c::F_0616 - Ordon Village - 3rd Day - Spoke to Talo/Malo after handing over wooden sword (1st time) */
                    daNpcT_chkEvtBit(0x25C);

        case 14:
            return 0;

        case 15:
                    /* dSv_event_flag_c::F_0616 - Ordon Village - 3rd Day - Spoke to Talo/Malo after handing over wooden sword (1st time) */
            return !daNpcT_chkEvtBit(0x25C) ||
                    /* dSv_event_flag_c::M_095 - Faron Woods - First time meeting Coro (obtain lantern) */
                    daNpcT_chkEvtBit(0x80);

        case 16:
                    /* dSv_event_flag_c::M_095 - Faron Woods - First time meeting Coro (obtain lantern) */
            return !daNpcT_chkEvtBit(0x80) ||
                    /* dSv_event_flag_c::F_0565 - N/A - 2nd Day Complete */
                    daNpcT_chkEvtBit(0x235);

        case 17:
            return 0;

        default:
            return 0;
    }
}

void daNpc_Besu_c::reset() {
    csXyz acStack_20;
    int size = (u8*)&field_0x1134 - (u8*)&mNextAction;
    if (mpMatAnm[0] != NULL) {
        mpMatAnm[0]->initialize();
    }
    initialize();
    for (int i = 0; i < 7; i++) {
        mActorMngr[i].initialize();
    }

    memset(&mNextAction, 0, size);
    acStack_20.setall(0);
    acStack_20.y = home.angle.y;
    switch (mType) {
        case 0:
            eventInfo.setIdx(0);
            break;
        case 1:
                    /* dSv_event_flag_c::F_0026 - Ordon Village - gave wooden sword to talo on 3rd day */
            mHide = daNpcT_chkEvtBit(0x1F) == 0;
            eventInfo.setIdx(1);
            break;
        case 6:
            field_0x112d = 1;
            break;
        case 7:
            field_0x112e = 1;
            break;
        case 8:
            field_0x112d = 1;
            break;
        case 16:
            break;
    }

    /* dSv_event_tmp_flag_c::T_0100 - Ordon Village - cutscene at start of 2nd day, start focus on kids Flags */
    daNpcT_offTmpBit(0x64);
    setAngle(acStack_20);
}

BOOL daNpc_Besu_c::chkNurse() {
    return mType == 6 || mType == 7 || mType == 8;
}

BOOL daNpc_Besu_c::chkBesu3() {
    return mType == 3 || mType == 5 || mType == 15;
}

void daNpc_Besu_c::afterJntAnm(int param_1) {
    if (param_1 == 1) {
        mDoMtx_stack_c::YrotM(mStagger.getAngleX(1));
        mDoMtx_stack_c::ZrotM(mStagger.getAngleZ(1));
    } else if (param_1 == 4) {
        mDoMtx_stack_c::YrotM(mStagger.getAngleX(0));
        mDoMtx_stack_c::ZrotM(mStagger.getAngleZ(0));
    }
}

void daNpc_Besu_c::setParam() {
    selectAction();
    srchActors();

    u32 att_flags = (fopAc_AttnFlag_SPEAK_e | fopAc_AttnFlag_TALK_e);
    s16 talk_dist = daNpc_Besu_Param_c::m.common.talk_distance;
    s16 talk_ang = daNpc_Besu_Param_c::m.common.talk_angle;
    s16 att_dist = daNpc_Besu_Param_c::m.common.attention_distance;
    s16 att_ang = daNpc_Besu_Param_c::m.common.attention_angle;
    if (daNpcKakashi_chkSwdTutorialStage() & 0xFF) {
        talk_dist = 11;
        talk_ang = 6;
        att_dist = 15;
        att_ang = 6;
        att_flags = 0;
    } else if (chkNurse()) {
        talk_dist = 1;
        att_dist = 1;
    } else {
        switch (mType) {
            case 3:
            case 4: {
                talk_dist = 5;
                att_dist = 7;
                break;
            }

            case 11: {
                talk_dist = 5;
                att_dist = 5;
                break;
            }
        }
    }

    if (daPy_getPlayerActorClass()->checkHorseRide()) {
        if (talk_dist < 7) {
            talk_dist = 7;
        }

        if (att_dist < 9) {
            att_dist = 9;
        }
    }

    attention_info.distances[0] = daNpcT_getDistTableIdx(att_dist, att_ang);
    attention_info.distances[1] = attention_info.distances[0];
    attention_info.distances[3] = daNpcT_getDistTableIdx(talk_dist, talk_ang);
    attention_info.flags = att_flags;

    scale.set(daNpc_Besu_Param_c::m.common.scale, daNpc_Besu_Param_c::m.common.scale,
            daNpc_Besu_Param_c::m.common.scale);
    mCcStts.SetWeight(daNpc_Besu_Param_c::m.common.weight);
    mCylH = daNpc_Besu_Param_c::m.common.height;
    mWallR = daNpc_Besu_Param_c::m.common.width;
    if (mTwilight) {
        mCylH = 110.0f;
    } else if (mType == 5) {
        mCylH = 70.0f;
        mWallR = 60.0f;
    }

    mAttnFovY = daNpc_Besu_Param_c::m.common.fov;
    if (mType == 3 || mType == 4) {
        mAttnFovY = 180.0f;
    }

    mAcchCir.SetWallR(mWallR);
    mAcchCir.SetWallH(daNpc_Besu_Param_c::m.common.knee_length);
    mRealShadowSize = daNpc_Besu_Param_c::m.common.real_shadow_size;
    if (chkNurse()) {
        mRealShadowSize = 500.0f;
    }

    mExpressionMorfFrame = daNpc_Besu_Param_c::m.common.expression_morf_frame;
    mMorfFrames = daNpc_Besu_Param_c::m.common.morf_frame;
    gravity = daNpc_Besu_Param_c::m.common.gravity;
}

BOOL daNpc_Besu_c::checkChangeEvt() {
    if (chkAction(&daNpc_Besu_c::talk) == 0) {
        mPreItemNo = 0;
        if (dComIfGp_event_chkTalkXY()) {
            if (dComIfGp_evmng_ChkPresentEnd()) {
                mEvtNo = EVENT_NO_RESPONSE;
                evtChange();
            }
            return 1;
        }

        switch (mType) {
            case 0:
                break;

            case 3:
            case 4:
                mEvtNo = EVENT_CACARICO_CONVERSATION;
                evtChange();
                return 1;
            case 6:
                mEvtNo = EVENT_CONVERSATION_ABOUT_DEATHMT;
                evtChange();
                return 1;
            case 7:
                mEvtNo = EVENT_NURSE;
                evtChange();
                return 1;
            case 8:
                mEvtNo = EVENT_CONVERSATION_ABOUT_ZORA;
                evtChange();
                return 1;
            case 11:
                if (checkItemGet(0x4B, 1)) {
                    mEvtNo = EVENT_CONVERSATION_ABOUT_PACHINKO3;
                           /* dSv_event_flag_c::F_0024 - Ordon Village - Spoke with Talo/Malo/Beth (before obtaining slingshot) */
                } else if (daNpcT_chkEvtBit(0x1D)) {
                    mEvtNo = EVENT_CONVERSATION_ABOUT_PACHINKO2;
                } else {
                    mEvtNo = EVENT_CONVERSATION_ABOUT_PACHINKO1;
                }
                evtChange();
                return 1;
            case 17:
                    /* dSv_event_flag_c::F_0601 - Faron Woods - Spoke to imprisoned Talo */
                if (daNpcT_chkEvtBit(0x24D)) {
                    mEvtNo = EVENT_CONVERSATION_ABOUT_SARU02;
                } else {
                    mEvtNo = EVENT_CONVERSATION_ABOUT_SARU01;
                }
                evtChange();
                return 1;
        }
    }

    return 0;
}

void daNpc_Besu_c::setAfterTalkMotion() {
    int idx = FACE_NONE;
    switch (mFaceMotionSeqMngr.getNo()) {
        case FACE_TALK_A:
            idx = FACE_H_TALK_A;
            break;
        case FACE_TALK_B:
            idx = FACE_H_TALK_B;
            break;
        case FACE_MADTALK:
            idx = FACE_H_MADTALK;
            break;
        case FACE_SURPISE:
            idx = FACE_H_SURPRISE;
            break;
        case FACE_WORRY:
            idx = FACE_H_WORRY;
            break;
    }

    mFaceMotionSeqMngr.setNo(idx, -1.0f, 0, 0);
}

void daNpc_Besu_c::srchActors() {
    switch (mType) {
        case 0:
            if (mActorMngr[0].getActorP() == NULL) {
                mActorMngr[0].entry(getNearestActorP(PROC_NPC_TARO));
            }
            if (mActorMngr[1].getActorP() == NULL) {
                mActorMngr[1].entry(getNearestActorP(PROC_NPC_MARO));
                return;
            }
            break;
        case 2:
            if (mActorMngr[3].getActorP() == NULL) {
                // LEN is Renaldo
                mActorMngr[3].entry(getNearestActorP(PROC_NPC_LEN));
                return;
            }
            break;
        case 11:
            if (mActorMngr[0].getActorP() == NULL) {
                mActorMngr[0].entry(getNearestActorP(PROC_NPC_TARO));
            }
            if (mActorMngr[1].getActorP() == NULL) {
                mActorMngr[1].entry(getNearestActorP(PROC_NPC_MARO));
                return;
            }
            break;
        case 12:
            if (mActorMngr[0].getActorP() == NULL) {
                mActorMngr[0].entry(getNearestActorP(PROC_NPC_TARO));
            }
            if (mActorMngr[1].getActorP() == NULL) {
                mActorMngr[1].entry(getNearestActorP(PROC_NPC_MARO));
                return;
            }
            break;
        case 13:
            if (mActorMngr[0].getActorP() == NULL) {
                mActorMngr[0].entry(getNearestActorP(PROC_NPC_TARO));
            }
            if (mActorMngr[2].getActorP() == NULL) {
                mActorMngr[2].entry(getNearestActorP(PROC_NPC_KOLIN));
            }
            if (mActorMngr[4].getActorP() == NULL) {
                mActorMngr[4].entry(getNearestActorP(PROC_NPC_SARU));
                return;
            }
            break;
        case 14:
            if (mActorMngr[0].getActorP() == NULL) {
                mActorMngr[0].entry(getNearestActorP(PROC_NPC_TARO));
            }
            if (mActorMngr[1].getActorP() == NULL) {
                mActorMngr[1].entry(getNearestActorP(PROC_NPC_MARO));
                return;
            }
            break;
        case 17:
            if (mActorMngr[5].getActorP() == NULL) {
                mActorMngr[5].entry(getNearestActorP(PROC_NPC_HANJO));
            }
            if (mActorMngr[6].getActorP() == NULL) {
                mActorMngr[6].entry(getNearestActorP(PROC_NPC_SEIRA));
            }
            break;
    }
}

BOOL daNpc_Besu_c::evtTalk() {
    if (chkAction(&daNpc_Besu_c::talk)) {
        (this->*(mAction))(NULL);
    } else {
        setAction(&daNpc_Besu_c::talk);
    }

    return 1;
}

BOOL daNpc_Besu_c::evtCutProc() {
    int staff_id = dComIfGp_getEventManager().getMyStaffId("Besu", this, -1);
    if (staff_id != -1) {
        mStaffId = staff_id;
        int actIdx = dComIfGp_getEventManager().getMyActIdx(mStaffId, (char**) &daNpc_Besu_c::mCutNameList, 15, 0, 0);
        if (((this->*(mCutList[actIdx]))(mStaffId))) {
            dComIfGp_getEventManager().cutEnd(mStaffId);
        }

        return 1;
    }

    return 0;
}

void daNpc_Besu_c::action() {
    fopAc_ac_c* actor_p = NULL;
    if (mTwilight == 0) {
        actor_p = hitChk(&mCyl1, -1);
    }

    if (actor_p != NULL && mType != 5
#if !DEBUG
        && !chkNurse()
#endif
    ) {
        mStagger.setParam(this, actor_p, mCurAngle.y);
        setDamage(0, 0xC, 0x12);
        // FIXME: Fakematch
#if DEBUG
        mStagger.setPower(0.0f);
#else
        for (int i = 0; i < 2; i++) {
            mStagger.mPower[i] = 0.0f;
        }
#endif
        mDamageTimerStart = 0;
        mJntAnm.lookNone(1);
    }

    if (mStagger.checkRebirth()) {
        // FIXME: Fakematch
#if DEBUG
        mStagger.initialize();
#else
        for (int i = 0; i < 2; i++) {
            mStagger.mAngle[i].setall(0);
            mStagger.mPower[i] = 0.0f;
        }
        mStagger.mStagger = 0;
        mStagger.field_0x16 = 0;
        mStagger.mRebirth = 0;
#endif
        mMode = 1;
    }

    if (mNextAction) {
        if (mAction == mNextAction) {
            (this->*(mAction))(NULL);
        } else {
            setAction(mNextAction);
        }
    }

    actor_p = field_0xba0.getActorP();
    if (actor_p != NULL) {
        switch (((daTag_Push_c*) actor_p)->getId()) {
            case 6: {
                mEvtNo = EVENT_SURPRISE;
            }
        }
    }

    cLib_calcTimer(&field_0x1120);
    cLib_calcTimer(&field_0x1124);
}

void daNpc_Besu_c::beforeMove() {
    fopAcM_OffStatus(this, 0x08000000U);
    if (checkHide()) {
        fopAcM_OnStatus(this, 0x08000000U);
    }

    if (checkHide() || mNoDraw) {
        attention_info.flags = 0;
    }
}

void daNpc_Besu_c::setAttnPos() {
    cXyz eyeOffset(10.0f, 30.0f, 0.0f);
    if (field_0x112f) {
        daPy_getPlayerActorClass()->onWolfEyeKeep();
    }

    BOOL var_r28 = FALSE;
    if (mMotionSeqMngr.getNo() == MOT_YOKERU && !mMotionSeqMngr.checkEndSequence()) {
        var_r28 = TRUE;
    }

    mStagger.calc(var_r28);
    f32 rad_val = cM_s2rad(mCurAngle.y - field_0xd7e.y);
    if (chkNurse()) {
        mJntAnm.setParam(
            this, mpMorf[0]->getModel(), &eyeOffset, getBackboneJointNo(), getNeckJointNo(),
            getHeadJointNo(), daNpc_Besu_Param_c::m.common.body_angleX_min, 0.0f,
            0.0f, 0.0f, -10.0f, daNpc_Besu_Param_c::m.common.head_angleX_max,
            daNpc_Besu_Param_c::m.common.head_angleY_min, daNpc_Besu_Param_c::m.common.head_angleY_max,
            daNpc_Besu_Param_c::m.common.neck_rotation_ratio, 0.0f, NULL);
    } else {
        mJntAnm.setParam(
            this, mpMorf[0]->getModel(), &eyeOffset, getBackboneJointNo(), getNeckJointNo(),
            getHeadJointNo(), daNpc_Besu_Param_c::m.common.body_angleX_min, daNpc_Besu_Param_c::m.common.body_angleX_max,
            daNpc_Besu_Param_c::m.common.body_angleY_min, daNpc_Besu_Param_c::m.common.body_angleY_max,
            daNpc_Besu_Param_c::m.common.head_angleX_min, daNpc_Besu_Param_c::m.common.head_angleX_max,
            daNpc_Besu_Param_c::m.common.head_angleY_min, daNpc_Besu_Param_c::m.common.head_angleY_max,
            daNpc_Besu_Param_c::m.common.neck_rotation_ratio, 0.0f, NULL);
    }

    mJntAnm.calcJntRad(0.2f, 1.0f, rad_val);
    mpMorf[0]->setPlaySpeed(daNpc_Besu_Param_c::m.field_0x8c);
    setMtx();
    if (mpCupModelMorf != NULL) {
        mpCupModelMorf->play(0, 0);
        Mtx jointAnmMtx;
        mDoMtx_stack_c::copy(mpMorf[0]->getModel()->getAnmMtx(10));
        cMtx_copy(mDoMtx_stack_c::get(), jointAnmMtx);
        mpCupModelMorf->getModel()->setBaseTRMtx(jointAnmMtx);
        mpCupModelMorf->modelCalc();
    }

    mDoMtx_stack_c::copy(mpMorf[0]->getModel()->getAnmMtx(getHeadJointNo()));
    mDoMtx_stack_c::multVec(&eyeOffset, &eyePos);
    if (&daNpc_Besu_c::nurse == mNextAction || &daNpc_Besu_c::giveHotWater == mNextAction) {
        mJntAnm.setEyeAngleX(eyePos, 1.0f, 0);
        mJntAnm.setEyeAngleY(eyePos, mCurAngle.y + 0x8000, 1, 1.0f, 0);
    } else if (chkBesu3()) {
        mJntAnm.setEyeAngleX(eyePos, 1.0f, -0x1800);
        mJntAnm.setEyeAngleY(eyePos, mCurAngle.y, 1, 1.0f, 0);
    } else {
        mJntAnm.setEyeAngleX(eyePos, 1.0f, 0);
        mJntAnm.setEyeAngleY(eyePos, mCurAngle.y, 1, 1.0f, 0);
    }

    if (chkNurse()) {
        eyeOffset.set(84.0f, 130.0f, 75.0f);
        mDoMtx_stack_c::copy(mpMorf[0]->getModel()->getBaseTRMtx());
        mDoMtx_stack_c::multVec(&eyeOffset, &attention_info.position);
    } else if (mType == 5) {
        eyeOffset.set(40.0f, 80.0f, 20.0f);
        mDoMtx_stack_c::copy(mpMorf[0]->getModel()->getBaseTRMtx());
        mDoMtx_stack_c::multVec(&eyeOffset, &attention_info.position);
    } else {
        eyeOffset.set(0.0f, 0.0f, 0.0f);
        eyeOffset.y = daNpc_Besu_Param_c::m.common.attention_offset;
        mDoMtx_stack_c::YrotS(mCurAngle.y);
        mDoMtx_stack_c::multVec(&eyeOffset, &eyeOffset);
        attention_info.position = current.pos + eyeOffset;
    }
}

void daNpc_Besu_c::setCollision() {
    cXyz cStack_48;
    if (!mHide) {
        int tgType = 0xD8FBFDFF;
        int tgSPrm = 0x1F;
        if (mTwilight) {
            tgType = 0;
            tgSPrm = 0;
        } else if (mStagger.checkStagger()) {
            tgType = 0;
            tgSPrm = 0;
        }

        mCyl1.SetCoSPrm(0x79);
        mCyl1.SetTgType(tgType);
        mCyl1.SetTgSPrm(tgSPrm);
        mCyl1.OnTgNoHitMark();
        cStack_48.set(0.0f, 0.0f, 0.0f);
        f32 cylH = mCylH;
        f32 wallR = mWallR;
        if (mType == 2) {
            cStack_48.set(62.72f, 0.0f, 0.0f);
        } else if (chkNurse()) {
            cStack_48.set(84.0f, 0.0f, 85.0f);
        }

        mDoMtx_stack_c::YrotS(mCurAngle.y);
        mDoMtx_stack_c::multVec(&cStack_48, &cStack_48);
        cStack_48 += current.pos;
        mCyl1.SetH(cylH);
        mCyl1.SetR(wallR);
        mCyl1.SetC(cStack_48);
        dComIfG_Ccsp()->Set(&mCyl1);
        if ((mType == 11 && !dComIfGp_event_runCheck()) || mType == 3) {
            switch (mType) {
                case 3:
                    wallR = 100.0f;
                    cStack_48.set(0.0f, 0.0f, 20.0f);
                    break;
                case 11:
                    wallR = 120.0f;
                    cStack_48.set(30.0f, 0.0f, 60.0f);
                    break;
                default:
                    wallR = 0.0f;
                    cStack_48.set(0.0f, 0.0f, 0.0f);
                    break;
            }

            if (cM3d_IsZero(wallR) == false) {
                mDoMtx_stack_c::YrotS(mCurAngle.y);
                mDoMtx_stack_c::multVec(&cStack_48, &cStack_48);
                cStack_48 += current.pos;
                mCyl2.SetCoSPrm(0x19);
                mCyl2.SetH(cylH);
                mCyl2.SetR(wallR);
                mCyl2.SetC(cStack_48);
                dComIfG_Ccsp()->Set(&mCyl2);
            }
        }
    }

    mCyl1.ClrCoHit();
    mCyl2.ClrCoHit();
    mCyl1.ClrTgHit();
}

int daNpc_Besu_c::drawDbgInfo() {
    return false;
}

void daNpc_Besu_c::drawOtherMdl() {
    J3DModel* model_p = mpMorf[0]->getModel();
    if (mpCupModelMorf != NULL) {
        g_env_light.setLightTevColorType_MAJI(mpCupModelMorf->getModel(), &tevStr);
        if (field_0x112e == 0) {
            mpCupModelMorf->getModel()->getModelData()->hide();
        } else {
            mpCupModelMorf->getModel()->getModelData()->show();
        }

        mpCupModelMorf->entryDL();
        dComIfGd_addRealShadow(mShadowKey, mpCupModelMorf->getModel());
    }

    for (int i = 0; i < 1; i++) {
        if (mpClothModel[i] != NULL && i == 0 && field_0x112d != 0) {
            g_env_light.setLightTevColorType_MAJI(mpClothModel[i], &tevStr);

            static int const jointNo[1] = {14};

            mDoMtx_stack_c::copy(model_p->getAnmMtx(jointNo[i]));
            Mtx jointAnmMtx;
            cMtx_copy(mDoMtx_stack_c::get(), jointAnmMtx);
            mpClothModel[i]->setBaseTRMtx(jointAnmMtx);
            mDoExt_modelUpdateDL(mpClothModel[i]);
            dComIfGd_addRealShadow(mShadowKey, mpClothModel[i]);
        }
    }
}

bool daNpc_Besu_c::setCupAnm(int arg0, int i_attr, f32 i_morf) {
    static int cupAnmData[33][2] = {
        {-1, 0}, {-1, 0}, {-1, 0}, {-1, 0}, {-1, 0}, {-1, 0}, {-1, 0}, {-1, 0},
        {-1, 0}, {-1, 0}, {-1, 0}, {-1, 0}, {-1, 0}, {-1, 0}, {-1, 0}, {-1, 0},
        {-1, 0}, {-1, 0}, { 9, 5}, { 9, 5}, {-1, 0}, {-1, 0}, {-1, 0}, {-1, 0},
        {-1, 0}, {-1, 0}, {-1, 0}, {-1, 0}, {-1, 0}, {-1, 0}, {-1, 0}, {-1, 0},
        {-1, 0},
    };

    J3DAnmTransform* transform_p = NULL;
    if (mpCupModelMorf != NULL) {
        if (cupAnmData[arg0][0] > 0) {
            transform_p = getTrnsfrmKeyAnmP(l_resNameList[cupAnmData[arg0][1]], cupAnmData[arg0][0]);
        }

        if (transform_p != NULL && transform_p != mpCupModelMorf->getAnm()) {
            mpCupModelMorf->setAnm(transform_p, i_attr, i_morf, 1.0f, 0.0f, -1.0f);
        }
    }

    return true;
}

bool daNpc_Besu_c::afterSetMotionAnm(int arg0, int arg1, f32 arg2, int param_3) {
    f32 var_f30 = (mCreating == 1) ? 0.0f : arg2;
    return setCupAnm(arg0, arg1, var_f30);
}

daNpcT_faceMotionAnmData_c daNpc_Besu_c::getFaceMotionAnm(daNpcT_faceMotionAnmData_c arg0) {
    daNpcT_faceMotionAnmData_c rv = arg0;
    if (chkBesu3() && rv.mBtpFileIdx == 25 && rv.mBtpArcIdx == 3) {
        rv.mBtpFileIdx = 20;
        rv.mBtpArcIdx = 6;
    }

    return rv;
}

void daNpc_Besu_c::changeAnm(int* arg0, int* arg1) {
    if (mType == 0x11) {
        switch (*arg0) {
            case 14:
                *arg0 = 0xA;
                *arg1 = 7;
                return;
            case 16:
                *arg0 = 0xB;
                *arg1 = 7;
                return;
            case 17:
                *arg0 = 0xC;
                *arg1 = 7;
                return;
            case 19:
                *arg0 = 0xD;
                *arg1 = 7;
                break;
        }
    }
}

void daNpc_Besu_c::changeBck(int* arg0, int* arg1) {
    if (mType == 0x11) {
        switch (*arg0) {
            case 5:
                *arg0 = 5;
                *arg1 = 7;
                return;
            case 6:
                *arg0 = 6;
                *arg1 = 7;
                return;
            case 7:
                *arg0 = 7;
                *arg1 = 7;
                return;
            case 8:
                *arg0 = 8;
                *arg1 = 7;
                return;
            case 9:
                *arg0 = 9;
                *arg1 = 7;
                break;
        }
    }
}

void daNpc_Besu_c::changeBtp(int* arg0, int* arg1) {
    if (mType == 0x11) {
        switch (*arg0) {
            case 25:
                *arg0 = 0x13;
                *arg1 = 7;
                return;
            case 18:
                *arg0 = 0x14;
                *arg1 = 7;
                return;
            case 26:
                *arg0 = 0x15;
                *arg1 = 7;
                return;
            case 22:
                *arg0 = 0x16;
                *arg1 = 7;
                break;
        }
    }
}

void daNpc_Besu_c::changeBtk(int* arg0, int* arg1) {
    if (mType == 0x11) {
        if (*arg0 != 0x16) {
            return;
        }

        *arg0 = 0x10;
        *arg1 = 7;
    }
}

int daNpc_Besu_c::selectAction() {
    mNextAction = NULL;
    switch (mType) {
        case 6:
            mNextAction = &daNpc_Besu_c::nurse;
            break;
        case 7:
            mNextAction = &daNpc_Besu_c::giveHotWater;
            break;
        case 8:
            mNextAction = &daNpc_Besu_c::nurse;
            break;
        case 14:
            if (daNpcKakashi_chkSwdTutorialStage() & 0xFF) {
                mNextAction = &daNpc_Besu_c::swdTutorial;
            } else {
                mNextAction = &daNpc_Besu_c::wait;
            }

            break;
        default:
            mNextAction = &daNpc_Besu_c::wait;
            break;
    }

    return 1;
}

int daNpc_Besu_c::chkAction(actionFunc arg0) {
    return mAction == arg0;
}

int daNpc_Besu_c::setAction(actionFunc arg0) {
    mMode = MODE_EXIT;
    if (mAction != NULL) {
        (this->*(mAction))(NULL);
    }

    mMode = MODE_ENTER;
    mAction = arg0;
    if (mAction != NULL) {
        (this->*(mAction))(NULL);
    }

    return 1;
}

void daNpc_Besu_c::mop(int arg0, int arg1) {
    if (field_0x1120) {
        action();
        if (field_0x1120 == 0) {
            mFaceMotionSeqMngr.setNo(FACE_NURSE_A, 4.0f, 0, 0);
            mMotionSeqMngr.setNo(MOT_NURSE_A, 4.0f, 0, 0);
            field_0x1124 = arg1;
        }
    } else if (field_0x1124) {
        action();
        if (field_0x1124 == 0) {
            mFaceMotionSeqMngr.setNo(FACE_MOP_A_C, 4.0f, 0, 0);
            mMotionSeqMngr.setNo(MOT_MOP_A_C, 4.0f, 0, 0);
            field_0x1120 = arg0;
        }
    }
}

int daNpc_Besu_c::cutConversationAboutSaru(int arg0) {
    int retval = 0;
    int* param_p = NULL;
    int prm_val = -1;
    param_p = dComIfGp_evmng_getMyIntegerP(arg0, "prm");
    if (param_p != NULL) {
        prm_val = *param_p;
    }

    if (dComIfGp_getEventManager().getIsAddvance(arg0)) {
        switch (prm_val) {
            case 0: {
                mPlayerAngle = fopAcM_searchPlayerAngleY(this);
                if (checkStep()) {
                    mStepMode = 0;
                }
            }

            case 1: {
                break;
            }
        }
    }

    switch (prm_val) {
        case 0: {
            retval = 1;
            break;
        }

        case 1: {
            mJntAnm.lookPlayer(0U);
            if (mPlayerAngle != mCurAngle.y) {
                step(mPlayerAngle, 0x1C, 0x11, 0xF, 0);
            } else {
                retval = 1;
            }

            break;
        }
    }

    return retval;
}

int daNpc_Besu_c::cutConversationAboutPachinko(int arg0) {
    int retval = 0;
    int* param_p = NULL;
    int prm_val = -1;
    int msgNo_val = 0;
    int msgNo2_val = 0;
    int timer_val = 0;
    param_p = dComIfGp_evmng_getMyIntegerP(arg0, "prm");
    if (param_p != NULL) {
        prm_val = *param_p;
    }

    param_p = dComIfGp_evmng_getMyIntegerP(arg0, "msgNo");
    if (param_p != NULL) {
        msgNo_val = *param_p;
    }

    param_p = dComIfGp_evmng_getMyIntegerP(arg0, "msgNo2");
    if (param_p != NULL) {
        msgNo2_val = *param_p;
    }

    param_p = dComIfGp_evmng_getMyIntegerP(arg0, "timer");
    if (param_p != NULL) {
        timer_val = *param_p;
    }

    fopAc_ac_c* speakers[3] = {mActorMngr[0].getActorP(), mActorMngr[1].getActorP(), this};
    dComIfGp_setMesgCameraInfoActor(speakers[0], speakers[1], speakers[2],
                                    NULL, NULL, NULL, NULL, NULL, NULL, NULL);

    if (dComIfGp_getEventManager().getIsAddvance(arg0)) {
        switch (prm_val) {
            case 0:
            case 2:
            case 3:
            case 4:
            case 5:
            case 6:
            case 7:
            case 8:
            case 9:
            case 11:
            case 12:
            case 13:
            case 14:
            case 15:
            case 16:
            case 17:
            case 18:
            case 19:
            case 21:
                break;

            case 1:
                mEventTimer = timer_val;
                initTalk(mFlowNodeNo, speakers);
                break;

            case 10:
                field_0x112c = 1;
                field_0x1128 = 0;
                initTalk(mFlowNodeNo, speakers);
                break;

            case 20:
                initTalk(mFlowNodeNo, speakers);
                break;
        }
    }

    int my_vals[3] = {-1, -1, -1};

    switch (prm_val) {
        case 0: {
            retval = 1;
            break;
        }

        case 1:
        case 2:
        case 3:
        case 4:
        case 10:
        case 11:
        case 12: {
            switch (prm_val) {
                case 1:
                case 2: {
                    mJntAnm.lookPlayer(0U);
                    if (mPlayerAngle != mCurAngle.y) {
                        if (chkBesu3()) {
                            step(mPlayerAngle, -1, -1, 15, 0);
                        } else {
                            step(mPlayerAngle, -1, -1, 15, 0);
                        }
                    }

                    break;
                }

                case 3: {
                    JUT_ASSERT(0xD84, NULL != speakers[0]);
                    mJntAnm.lookActor(speakers[0], -40.0f, 0U);
                    break;
                }

                case 4: {
                    JUT_ASSERT(0xD89, NULL != speakers[0]);
                    mJntAnm.lookActor(speakers[0], -40.0f, 0U);
                    if (home.angle.y != mCurAngle.y) {
                        if (chkBesu3()) {
                            step(home.angle.y, -1, -1, 15, 0);
                        } else {
                            step(home.angle.y, -1, -1, 15, 0);
                        }
                    }

                    break;
                }

                case 10:
                case 11: {
                    JUT_ASSERT(0xD9F, NULL != speakers[0]);
                    mJntAnm.lookActor(speakers[0], -40.0f, 0U);
                    break;
                }

                case 12: {
                    JUT_ASSERT(0xDA4, NULL != speakers[1]);
                    mJntAnm.lookActor(speakers[1], -40.0f, 0U);
                    break;
                }
            }
        }
        /* fallthrough */
        case 20: {
            my_vals[0] = msgNo_val;
            my_vals[1] = msgNo2_val;
            if (talkProc(my_vals, 0, speakers, 0)) {
                if (msgNo_val == 0 && msgNo2_val == 0) {
                    if (mFlow.checkEndFlow()) {
                        retval = 1;
                    }
                } else {
                    retval = 1;
                }
            }

            break;
        }

        case 21: {
            dStage_changeScene(10, 0.0f, 0U, fopAcM_GetRoomNo(this), 0, -1);
            break;
        }
    }

    return retval;
}

int daNpc_Besu_c::cutConversationAboutWoodSwd(int arg0) {
    int retval = 0;
    int* param_p = NULL;
    int prm_val = -1;
    int timer_val = 0;
    param_p = dComIfGp_evmng_getMyIntegerP(arg0, "prm");
    if (param_p != NULL) {
        prm_val = *param_p;
    }

    param_p = dComIfGp_evmng_getMyIntegerP(arg0, "timer");
    if (param_p != NULL) {
        timer_val = *param_p;
    }

    fopAc_ac_c* speakers[3] = {mActorMngr[0].getActorP(), mActorMngr[1].getActorP(), this};
    dComIfGp_setMesgCameraInfoActor(speakers[0], speakers[1], speakers[2],
                                    NULL, NULL, NULL, NULL, NULL, NULL, NULL);

    if (dComIfGp_getEventManager().getIsAddvance(arg0)) {
        switch (prm_val) {
            case 0: {
                mEventTimer = timer_val;
                initTalk(mFlowNodeNo, speakers);
                break;
            }

            case 1: {
                break;
            }
        }
    }

    switch (prm_val) {
        case 0: {
            mJntAnm.lookPlayer(0);
            if (mPlayerAngle != mCurAngle.y) {
                if (chkBesu3()) {
                    if (step(mPlayerAngle, 0x1C, 0x1D, 0xF, 0)) {
                        mFaceMotionSeqMngr.setNo(FACE_NONE, -1.0f, 0, 0);
                        mMotionSeqMngr.setNo(MOT_WAIT_A, -1.0f, 0, 0);
                    }
                } else if (step(mPlayerAngle, 0x1C, 0x11, 0xF, 0)) {
                    mFaceMotionSeqMngr.setNo(FACE_NONE, -1.0f, 0, 0);
                    mMotionSeqMngr.setNo(MOT_WAIT_A, -1.0f, 0, 0);
                }
            }

            if (cLib_calcTimer(&mEventTimer) == 0) {
                retval = 1;
            }

            break;
        }

        case 1: {
            if (talkProc(NULL, 0, speakers, 0)) {
                int ev_id;
                switch (mFlow.getEventId(&ev_id)) {
                    case 2: {
                        dStage_changeScene(0xC, 0.0f, 0U, fopAcM_GetRoomNo(this), 0, -1);
                        break;
                    }

                    default: {
                        if (mFlow.checkEndFlow()) {
                            retval = 1;
                        }

                        break;
                    }
                }
            }

            break;
        }
    }

    return retval;
}

int daNpc_Besu_c::cutSwdTutorial(int arg0) {
    fopAc_ac_c* actor_p = NULL;
    cXyz my_vec_0;
    cXyz my_vec_1;
    int retval = 0;
    int* param_p = NULL;
    int prm_val = -1;
    param_p = dComIfGp_evmng_getMyIntegerP(arg0, "prm");
    if (param_p != NULL) {
        prm_val = *param_p;
    }

    if (dComIfGp_getEventManager().getIsAddvance(arg0)) {
        switch (prm_val) {
            case 0: {
                break;
            }

            case 1: {
                break;
            }

            case 71: {
                mFaceMotionSeqMngr.setNo(FACE_TALK_A, 0.0f, 1, 0);
                mMotionSeqMngr.setNo(MOT_TALK_A, 0.0f, 1, 0);
                setAngle(home.angle.y);
                my_vec_0.set(50.0f, 0.0f, 0.0f);
                mDoMtx_stack_c::YrotS(mCurAngle.y);
                mDoMtx_stack_c::multVec(&my_vec_0, &my_vec_0);
                my_vec_0 += current.pos;
                setPos(my_vec_0);
                break;
            }
        }
    }

    switch (prm_val) {
        case 0: {
            mDamageTimer = 1;
            action();
            mJntAnm.lookNone(0);
            retval = 1;
            break;
        }

        case 1: {
            mDamageTimer = 1;
            action();
            actor_p = mActorMngr[1].getActorP();
            JUT_ASSERT(0xE7E, NULL != actor_p);
            mJntAnm.lookActor(actor_p, -40.0f, 0);
            retval = 1;
            break;
        }

        case 71: {
            mJntAnm.lookCamera(0);
            break;
        }
    }

    return retval;
}

int daNpc_Besu_c::cutFindMonkey(int arg0) {
    fopAc_ac_c* actor_p = NULL;
    cXyz my_vec_0;
    int retval = 0;
    int* param_p = NULL;
    int prm_val = -1;
    int timer_val = 0;
    param_p = dComIfGp_evmng_getMyIntegerP(arg0, "prm");
    if (param_p != NULL) {
        prm_val = *param_p;
    }

    param_p = dComIfGp_evmng_getMyIntegerP(arg0, "timer");
    if (param_p != NULL) {
        timer_val = *param_p;
    }

    if (dComIfGp_getEventManager().getIsAddvance(arg0)) {
        switch (prm_val) {
            case 0: {
                break;
            }

            case 1: {
                mFaceMotionSeqMngr.setNo(FACE_H_BIKKURI, 0.0f, 0, 0);
                break;
            }

            case 2: {
                break;
            }

            case 3: {
                setAngle(fopAcM_searchActorAngleY(this, (fopAc_ac_c*) dComIfGp_getHorseActor()));
                mJntAnm.lookNone(1);
                mEventTimer = timer_val;
                if (mEventTimer == 0) {
                    mMotionSeqMngr.setNo(MOT_RUN, -1.0f, 0, 0);
                }

                break;
            }

            case 4: {
                mHide = 1;
                speedF = 0.0f;
                speed.setall(0.0f);
                break;
            }
        }
    }

    switch (prm_val) {
        case 0: {
            mJntAnm.lookPlayer(0);
            retval = 1;
            break;
        }

        case 1: {
            actor_p = mActorMngr[4].getActorP();
            JUT_ASSERT(0xED5, NULL != actor_p);
            mJntAnm.lookActor(actor_p, -40.0f, 0U);
            retval = 1;
            break;
        }

        case 2: {
            break;
        }

        case 3: {
            mJntAnm.lookNone(0U);
            if (mEventTimer == 0) {
                actor_p = (fopAc_ac_c*) dComIfGp_getHorseActor();
                JUT_ASSERT(0xEE4, NULL != actor_p);
                my_vec_0 = actor_p->current.pos;
                if (50.0f < current.pos.absXZ(my_vec_0)) {
                    cLib_addCalcAngleS2(&current.angle.y, cLib_targetAngleY(&current.pos, &my_vec_0), 4, 0x800);
                    shape_angle.y = current.angle.y;
                    mCurAngle.y = shape_angle.y;
                    cLib_chaseF(&speedF, 10.0f, 0.5f);
                } else {
                    retval = 1;
                }
            } else if (cLib_calcTimer(&mEventTimer) == 0) {
                mMotionSeqMngr.setNo(MOT_RUN, -1.0f, 0, 0);
            }

            break;
        }

        case 4: {
            retval = 1;
            break;
        }
    }

    return retval;
}

int daNpc_Besu_c::cutGetWoodSwd(int arg0) {
    int retval = 0;
    int* param_p = NULL;
    int prm_val = -1;
    param_p = dComIfGp_evmng_getMyIntegerP(arg0, "prm");
    if (param_p != NULL) {
        prm_val = *param_p;
    }

    if (dComIfGp_getEventManager().getIsAddvance(arg0)) {
        switch (prm_val) {
            case 0: {
                break;
            }

            case 1: {
                mHide = 1;
                break;
            }

            case 10: {
                break;
            }

            case 11: {
                mHide = 0;
                break;
            }
        }
    }

    switch (prm_val) {
        case 0:
        case 1:
        case 10:
        case 11: {
            action();
            retval = 1;
        }
    }

    return retval;
}

int daNpc_Besu_c::cutCacaricoConversation(int arg0) {
    int retval = 0;
    int* param_p = NULL;
    int prm_val = -1;
    param_p = dComIfGp_evmng_getMyIntegerP(arg0, "prm");
    if (param_p != NULL) {
        prm_val = *param_p;
    }

    if (dComIfGp_getEventManager().getIsAddvance(arg0)) {
        switch (prm_val) {
            case 0: {
                mPlayerAngle = fopAcM_searchPlayerAngleY(this);
                if (checkStep()) {
                    mStepMode = 0;
                }

                break;
            }

            case 1: {
                break;
            }
        }
    }

    int lits_6105[2] = {-1, -1};
    (void) lits_6105;
    switch (prm_val) {                              /* irregular */
        case 0: {
            mJntAnm.lookPlayer(0);
            if (mPlayerAngle != mCurAngle.y) {
                if (step(mPlayerAngle, 0x1C, 0x11, 0xF, 0)) {
                    mFaceMotionSeqMngr.setNo(FACE_NONE, -1.0f, 0, 0);
                    mMotionSeqMngr.setNo(MOT_WAIT_A, -1.0f, 0, 0);
                }
            } else {
                retval = 1;
            }

            break;
        }

        case 1: {
            mJntAnm.lookPlayer(0);
            retval = 1;
            break;
        }
    }

    return retval;
}

int daNpc_Besu_c::cutSurprise(int arg0) {
    daTag_Push_c* tagpush_p = (daTag_Push_c*) field_0xba0.getActorP();
    int retval = 0;
    int* param_p = NULL;
    int prm_val = -1;
    param_p = dComIfGp_evmng_getMyIntegerP(arg0, "prm");
    if (param_p != NULL) {
        prm_val = *param_p;
    }

    if (dComIfGp_getEventManager().getIsAddvance(arg0)) {
        switch (prm_val) {
            case 0:
                mFaceMotionSeqMngr.setNo(FACE_H_SURPRISE, 0.0f, 0, 0);
                mMotionSeqMngr.setNo(MOT_SURPRISE, 0.0f, 0, 0);
                mJntAnm.lookNone(1);
                tagpush_p->pushBackPlayer(1);
                break;

            case 1:
                initTalk(tagpush_p->getFlowNodeNo(), NULL);
                mJntAnm.lookPlayer(1U);
                break;
        }
    }

    switch (prm_val) {
        case 0:
            retval = 1;
            break;

        case 1:
            mJntAnm.lookPlayer(0);
            if (talkProc(NULL, 0, NULL, 0) && mFlow.checkEndFlow()) {
                field_0xba0.remove();
                retval = 1;
            }
            break;
    }

    return retval;
}

int daNpc_Besu_c::cutConversationInHotel(int arg0) {
    int retval = 0;
    int* param_p = NULL;
    int prm_val = -1;
    int timer_val = 0;
    int timer1_val = 0;
    int timer2_val = 0;
    param_p = dComIfGp_evmng_getMyIntegerP(arg0, "prm");
    if (param_p != NULL) {
        prm_val = *param_p;
    }

    param_p = dComIfGp_evmng_getMyIntegerP(arg0, "timer");
    if (param_p != NULL) {
        timer_val = *param_p;
    }

    param_p = dComIfGp_evmng_getMyIntegerP(arg0, "timer1");
    if (param_p != NULL) {
        timer1_val = *param_p;
    }

    param_p = dComIfGp_evmng_getMyIntegerP(arg0, "timer2");
    if (param_p != NULL) {
        timer2_val = *param_p;
    }

    if (dComIfGp_getEventManager().getIsAddvance(arg0)) {
        switch (prm_val) {
            case 0:
                field_0x1120 = 1;
                field_0x1124 = 0;
                break;
            case 1:
                mFaceMotionSeqMngr.setNo(FACE_AMAZED_C, 0.0f, 1, 0);
                mMotionSeqMngr.setNo(MOT_AMAZED_C, 0.0f, 1, 0);
                break;
            case 2:
                mEventTimer = timer_val;
                break;
            case 3:
                mFaceMotionSeqMngr.setNo(FACE_NURSE_A, 0.0f, 1, 0);
                mMotionSeqMngr.setNo(MOT_NURSE_A, 0.0f, 1, 0);
                break;
        }
    }

    switch (prm_val) {
        case 0:
            mop(timer1_val, timer2_val);
            retval = 1;
            break;
        case 1:
            retval = 1;
            break;
        case 2:
            if (mEventTimer != 0) {
                if (cLib_calcTimer(&mEventTimer) == 0) {
                    mFaceMotionSeqMngr.setNo(FACE_AMAZED_A, 0.0f, 0, 0);
                    mMotionSeqMngr.setNo(MOT_AMAZED_A, 0.0f, 0, 0);
                    mSound.startCreatureVoice(Z2SE_BESU_V_SHIKARARE, -1);
                } else {
                    mop(timer1_val, timer2_val);
                }
            } else {
                retval = 1;
            }
            break;
        case 3:
            retval = 1;
            break;
    }

    return retval;
}

int daNpc_Besu_c::cutConversationAboutDeathMt(int arg0) {
    int retval = 0;
    int* param_p = NULL;
    int prm_val = -1;
    param_p = dComIfGp_evmng_getMyIntegerP(arg0, "prm");
    if (param_p != NULL) {
        prm_val = *param_p;
    }

    if (dComIfGp_getEventManager().getIsAddvance(arg0)) {
        switch (prm_val) {
            case 0:
                mFaceMotionSeqMngr.setNo(FACE_TALK_N_B, -1.0f, 0, 0);
                mMotionSeqMngr.setNo(MOT_TALK_N_B, -1.0f, 0, 0);
                initTalk(mFlowNodeNo, NULL);
                break;
            case 1:
                mFaceMotionSeqMngr.setNo(FACE_TALK_N_A, -1.0f, 0, 0);
                mMotionSeqMngr.setNo(MOT_TALK_N_A, -1.0f, 0, 0);
                break;
            case 2:
                mFaceMotionSeqMngr.setNo(FACE_TALK_N_C, -1.0f, 0, 0);
                mMotionSeqMngr.setNo(MOT_TALK_N_C, -1.0f, 0, 0);
                break;
        }
    }

    switch (prm_val) {
        case 0: {
            if (talkProc(NULL, 0, NULL, 0) && mFlow.checkEndFlow()) {
                retval = 1;
            }

            break;
        }

        case 1:
        case 2: {
            if (mMotionSeqMngr.checkEndSequence()) {
                retval = 1;
            }

            break;
        }
    }

    return retval;
}

int daNpc_Besu_c::cutConversationAboutGoron(int arg0) {
    int retval = 0;
    int* param_p = NULL;
    int prm_val = -1;
    param_p = dComIfGp_evmng_getMyIntegerP(arg0, "prm");
    if (param_p != NULL) {
        prm_val = *param_p;
    }

    if (dComIfGp_getEventManager().getIsAddvance(arg0)) {
        switch (prm_val) {
            case 0:
                mFaceMotionSeqMngr.setNo(FACE_MOP_A_B, -1.0f, 0, 0);
                mMotionSeqMngr.setNo(MOT_MOP_A_B, -1.0f, 0, 0);
                break;
            case 1:
                mFaceMotionSeqMngr.setNo(FACE_MOP_C, -1.0f, 0, 0);
                mMotionSeqMngr.setNo(MOT_MOP_C, -1.0f, 0, 0);
                field_0x1120 = 0x20;
                break;
            case 2:
                mFaceMotionSeqMngr.setNo(FACE_MOP_A_B, 0.0f, 1, 0);
                mMotionSeqMngr.setNo(MOT_MOP_A_B, 0.0f, 1, 0);
                break;
            case 3:
                mFaceMotionSeqMngr.setNo(FACE_MOP_C, -1.0f, 0, 0);
                mMotionSeqMngr.setNo(MOT_MOP_C, -1.0f, 0, 0);
                field_0x1120 = 0x20;
                break;
        }
    }

    switch (prm_val) {
        case 0:
            action();
            retval = 1;
            break;
        case 1:
            action();
            if (field_0x1120 == 0) {
                retval = 1;
            }
            break;
        case 2:
            retval = 1;
            break;
        case 3:
            if (cLib_calcTimer(&field_0x1120) == 0) {
                retval = 1;
            }
            break;
    }

    return retval;
}

int daNpc_Besu_c::cutNurse(int arg0) {
    int retval = 0;
    int* param_p = NULL;
    int prm_val = -1;
    param_p = dComIfGp_evmng_getMyIntegerP(arg0, "prm");
    if (param_p != NULL) {
        prm_val = *param_p;
    }

    if (dComIfGp_getEventManager().getIsAddvance(arg0)) {
        switch (prm_val) {
            case 0:
                break;
            case 1:
                mFaceMotionSeqMngr.setNo(FACE_CUP_B, -1.0f, 0, 0);
                mMotionSeqMngr.setNo(MOT_CUP_B, -1.0f, 0, 0);
                break;
            case 2:
                mFaceMotionSeqMngr.setNo(FACE_CUP_A, 0.0f, 1, 0);
                mMotionSeqMngr.setNo(MOT_CUP_A, 0.0f, 1, 0);
                break;
        }
    }

    switch (prm_val) {
        case 0:
        case 1:
        case 2: {
            field_0xe26 = false;
            retval = 1;
            break;
        }
    }

    return retval;
}

int daNpc_Besu_c::cutClothTry(int arg0) {
    int retval = 0;
    int* param_p = NULL;
    int prm_val = -1;
    param_p = dComIfGp_evmng_getMyIntegerP(arg0, "prm");
    if (param_p != NULL) {
        prm_val = *param_p;
    }

    if (dComIfGp_getEventManager().getIsAddvance(arg0)) {
        switch (prm_val) {
            case 0:
                mHide = 1;
                break;
            case 1:
                mHide = 0;
                break;
        }
    }

    switch (prm_val) {
        case 0:
        case 1: {
            action();
            retval = 1;
            break;
        }
    }

    return retval;
}

int daNpc_Besu_c::cutThankYou(int arg0) {
    int retval = 0;
    int* param_p = NULL;
    int prm_val = -1;
    param_p = dComIfGp_evmng_getMyIntegerP(arg0, "prm");
    if (param_p != NULL) {
        prm_val = *param_p;
    }

    if (dComIfGp_getEventManager().getIsAddvance(arg0)) {
        switch (prm_val) {
            case 0: {
                break;
            }

            case 1: {
                mFaceMotionSeqMngr.setNo(FACE_GLARE_WAIT, 0.0f, 0, 0);
                mMotionSeqMngr.setNo(MOT_GLARE_WAIT, 0.0f, 0, 0);
                break;
            }
        }
    }

    switch (prm_val) {
        case 0:
            action();
            retval = 1;
            break;
        case 1:
            retval = 1;
            break;
    }

    return retval;
}

int daNpc_Besu_c::wait(void* param_0) {
    fopAc_ac_c* actor_p = NULL;
    switch (mMode) {
        case 0:
        case 1: {
            if (mStagger.checkStagger() == 0) {
                switch (mType) {
                    case 2: {
                        if (getBitSW() != 0xFF && dComIfGs_isSwitch(getBitSW(), fopAcM_GetRoomNo(this))) {
                            mFaceMotionSeqMngr.setNo(FACE_DEMO_FEAR, -1.0f, 0, 0);
                            mMotionSeqMngr.setNo(MOT_DEMO_FEAR, -1.0f, 0, 0);
                        } else {
                            mFaceMotionSeqMngr.setNo(FACE_DEMO_WAIT, -1.0f, 0, 0);
                            mMotionSeqMngr.setNo(MOT_DEMO_WAIT, -1.0f, 0, 0);
                        }

                        break;
                    }

                    case 5: {
                        mFaceMotionSeqMngr.setNo(FACE_H_SURPRISE, -1.0f, 0, 0);
                        mMotionSeqMngr.setNo(MOT_SURPRISE, -1.0f, 0, 0);
                        break;
                    }

                    default: {
                        if (chkBesu3()) {
                            mFaceMotionSeqMngr.setNo(FACE_NONE, -1.0f, 0, 0);
                            mMotionSeqMngr.setNo(MOT_NORMAL_WAIT, -1.0f, 0, 0);
                        } else {
                            mFaceMotionSeqMngr.setNo(FACE_NONE, -1.0f, 0, 0);
                            mMotionSeqMngr.setNo(MOT_WAIT_A, -1.0f, 0, 0);
                        }

                        break;
                    }
                }

                field_0x1118 = 0;
                field_0x111c = 0;
                mMode = 2;
            }
        }

        case 2: {
            switch (mType) {
                case 0: {
                        /* dSv_event_flag_c::F_0026 - Ordon Village - gave wooden sword to talo on 3rd day */
                    if (daNpcT_chkEvtBit(0x1F) && mHide) {
                        fopAcM_delete(this);
                        return 1;
                    }

                    break;
                }

                case 2: {
                    actor_p = (daNpc_Len_c*) mActorMngr[3].getActorP();
                    if (actor_p != NULL &&
                        ((daNpc_Len_c*) actor_p)->checkStartDemo13StbEvt(
                            this, daNpc_Besu_Param_c::m.common.box_min_x, daNpc_Besu_Param_c::m.common.box_min_y,
                            daNpc_Besu_Param_c::m.common.box_min_z, daNpc_Besu_Param_c::m.common.box_max_x,
                            daNpc_Besu_Param_c::m.common.box_max_y, daNpc_Besu_Param_c::m.common.box_max_z,
                            daNpc_Besu_Param_c::m.common.box_offset))
                    {
                        mEvtNo = EVENT_DEMO13_STB;
                        field_0x112f = 1;
                    }

                                        /* dSv_event_flag_c::M_027 - Cutscene - [cutscene: 13] kids in the church (beast eyes) */
                    if (field_0x112f && daNpcT_chkEvtBit(0x3C) && !dComIfGp_event_runCheck()) {
                        field_0x112f = 0;
                    }

                    break;
                }

                case 13: {
                        /* dSv_event_flag_c::F_0616 - Ordon Village - 3rd Day - Spoke to Talo/Malo after handing over wooden sword (1st time) */
                    if (daNpcT_chkEvtBit(0x25C)) {
                        fopAcM_delete(this);
                        return 1;
                    }

                    break;
                }
            }

            if (mStagger.checkStagger() == 0) {
                if (mType == 2) {
                    mJntAnm.lookNone(0);
                    if (getBitSW() != 0xFF) {
                        if (dComIfGs_isSwitch(getBitSW(), fopAcM_GetRoomNo(this))
                            && mMotionSeqMngr.getNo() != MOT_DEMO_FEAR) {
                            mMode = 1;
                        }
                    }

                    attention_info.flags = 0;
                } else {
                    u8 var_r28 = 0;
                    if (daNpcKakashi_chkSwdTutorialStage() & 0xFF) {
                        mPlayerActorMngr.remove();
                    } else {
                        if (mType == 11 || mType == 17) {
                            mPlayerActorMngr.remove();
                        } else if (mType == 12) {
                            var_r28 = dComIfGs_getTmpReg(0xFBFF);
                            if (var_r28 == 2 || var_r28 == 1) {
                                mPlayerActorMngr.entry(daPy_getPlayerActorClass());
                            } else if (var_r28 == 0) {
                                mPlayerActorMngr.remove();
                            }
                        }
                    }

                    if (mPlayerActorMngr.getActorP() != NULL && mTwilight == 0) {
                        if (mType == 3 || mType == 4) {
                            /* dSv_event_tmp_flag_c::YKM_ATTN - Snowpeak Mountain - Player focus while Yeto waits for snowboard race */
                            dComIfGs_onTmpBit(0xD02);
                        }

                        mJntAnm.lookPlayer(0);
                        if (mType == 0xC) {
                            if (var_r28 == 1) {
                                mJntAnm.lookNone(0);
                            }
                        } else {
                            if (chkActorInSight(mPlayerActorMngr.getActorP(), mAttnFovY, mCurAngle.y) == 0) {
                                mJntAnm.lookNone(0);
                            }

                            if (!srchPlayerActor() && home.angle.y == mCurAngle.y) {
                                mMode = 1;
                            }
                        }
                    } else {
                        if (mType == 3 || mType == 4) {
                            /* dSv_event_tmp_flag_c::YKM_ATTN - Snowpeak Mountain - Player focus while Yeto waits for snowboard race */
                            dComIfGs_offTmpBit(0xD02);
                        }

                        mJntAnm.lookNone(0);
                        if (home.angle.y != mCurAngle.y) {
                            if (field_0xe34) {
                                if (chkBesu3()) {
                                    if (step(home.angle.y, 0x1C, 0x1D, 0xF, 0)) {
                                        mMode = 1;
                                    }
                                } else if (step(home.angle.y, 0x1C, 0x11, 0xF, 0)) {
                                    mMode = 1;
                                }
                            } else {
                                setAngle(home.angle.y);
                                mMode = 1;
                            }
                            attention_info.flags = 0;
                        } else if (daNpcKakashi_chkSwdTutorialStage() & 0xFF) {
                            mJntAnm.lookPlayer(0);
                        } else if (mTwilight == 0) {
                            srchPlayerActor();
                        }
                    }
                }

                switch (mJntAnm.getMode()) {
                    case 0: {
                        switch (mType) {
                            case 3:
                            case 4: {
                                /* dSv_event_tmp_flag_c::YKW_ATTN - Snowpeak Mountain - Player focus while Yeta waits for 
                                                                    snowboard race */
                                dComIfGs_offTmpBit(0xE40);
                                break;
                            }

                            case 11: {
                                    /* dSv_event_tmp_flag_c::T_0100 - Ordon Village - cutscene at start of 2nd day, 
                                                                      start focus on kids Flags */
                                if (daNpcT_chkTmpBit(0x64)) {
                                    field_0x1130 = 1;
                                }

                                if (field_0x1130) {
                                    mJntAnm.lookCamera(0);
                                } else {
                                    if (cLib_calcTimer(&field_0x1128) == 0) {
                                        ++field_0x112c;
                                        field_0x1128 = cLib_getRndValue(30, 90);
                                    }

                                    if (field_0x112c & 1) {
                                        actor_p = mActorMngr[1].getActorP();
                                    } else {
                                        actor_p = mActorMngr[0].getActorP();
                                    }

                                    if (actor_p != NULL) {
                                        mJntAnm.lookActor((fopAc_ac_c *) actor_p, -40.0f, 0U);
                                    }
                                }

                                break;
                            }

                            case 17: {
                                actor_p = mActorMngr[5].getActorP();
                                if (actor_p != NULL) {
                                    mJntAnm.lookActor(actor_p, -40.0f, 0);
                                }

                                break;
                            }
                        }

                        break;
                    }

                    case 1: {
                        switch (mType) {
                            case 3:
                            case 4: {
                                /* dSv_event_tmp_flag_c::YKW_ATTN - Snowpeak Mountain - Player focus while Yeta waits for 
                                                                    snowboard race */
                                dComIfGs_onTmpBit(0xE40);
                                break;
                            }
                        }
                        break;
                    }
                }

                switch (mType) {
                    case 12: {
                        attention_info.flags = 0;
                        break;
                    }
                }
            }
        }

        case 3: {
            break;
        }
    }

    return 1;
}

int daNpc_Besu_c::swdTutorial(void* param_0) {
    switch (mMode) {
        case 0:
        case 1: {
            if (!mStagger.checkStagger()) {
                mFaceMotionSeqMngr.setNo(FACE_NONE, -1.0f, 0, 0);
                mMotionSeqMngr.setNo(MOT_WAIT_A, -1.0f, 0, 0);
                mMode = 2;
            }
        }
        /* fallthrough */
        case 2: {
            if (!mStagger.checkStagger()) {
                mJntAnm.lookPlayer(0);
            }

            attention_info.flags = 0;
            break;
        }

        case 3: {
            break;
        }
    }

    return 1;
}

int daNpc_Besu_c::nurse(void* param_0) {
    switch (mMode) {
        case 0:
        case 1: {
            if (mFaceMotionSeqMngr.getNo() != FACE_AMAZED_C) {
                mFaceMotionSeqMngr.setNo(FACE_NURSE_A, -1.0f, 0, 0);
            }

            if (mMotionSeqMngr.getNo() != MOT_AMAZED_C) {
                mMotionSeqMngr.setNo(MOT_NURSE_A, -1.0f, 0, 0);
            }

            mMorfLoops = 0;
            if (mType == 8) {
                field_0x1120 = 1;
                field_0x1124 = 0;
            }

            mMode = 2;
        }
        /* fallthrough */
        case 2: {
            mJntAnm.lookNone(0);
            mAcch.SetWallNone();
            switch (mMotionSeqMngr.getNo()) {
                case MOT_NURSE_A: {
                    if (0 != strcmp(dComIfGp_getEventManager().getRunEventName(), "EVT_CONVERSATION_IN_HOTEL1_e")) {
                        if (0 != strcmp(dComIfGp_getEventManager().getRunEventName(), "EVT_CONVERSATION_IN_HOTEL2_e")
                            && mType != 8 && mMorfLoops >= 2) {
                            mFaceMotionSeqMngr.setNo(FACE_GLARE_A, -1.0f, 0, 0);
                            mMotionSeqMngr.setNo(MOT_GLARE_A, -1.0f, 0, 0);
                        }
                    }

                    break;
                }

                case MOT_GLARE_A:
                case MOT_AMAZED_C: {
                    if (mMotionSeqMngr.checkEndSequence()) {
                        mFaceMotionSeqMngr.setNo(FACE_NURSE_A, -1.0f, 0, 0);
                        mMotionSeqMngr.setNo(MOT_NURSE_A, -1.0f, 0, 0);
                    }

                    break;
                }
            }

            if (mType == 8) {
                if (field_0x1120) {
                    cLib_calcTimer(&field_0x1120);
                    cLib_calcTimer(&field_0x1124);
                    if (field_0x1120 == 0) {
                        mFaceMotionSeqMngr.setNo(FACE_NURSE_A, -1.0f, 0, 0);
                        mMotionSeqMngr.setNo(MOT_NURSE_A, -1.0f, 0, 0);
                        field_0x1124 = 92;
                    }
                } else if (field_0x1124) {
                    cLib_calcTimer(&field_0x1120);
                    cLib_calcTimer(&field_0x1124);
                    if (field_0x1124 == 0) {
                        mFaceMotionSeqMngr.setNo(FACE_MOP_A_C, -1.0f, 0, 0);
                        mMotionSeqMngr.setNo(MOT_MOP_A_C, -1.0f, 0, 0);
                        field_0x1120 = 92;
                    }
                }
            }
            break;
        }

        case 3: {
            break;
        }
    }

    return 1;
}

int daNpc_Besu_c::giveHotWater(void* param_0) {
    switch (mMode) {
        case 0:
        case 1: {
            mFaceMotionSeqMngr.setNo(FACE_CUP_A, -1.0f, 0, 0);
            mMotionSeqMngr.setNo(MOT_CUP_A, -1.0f, 0, 0);
            mMode = 2;
        }
        /* fallthrough */
        case 2: {
            mJntAnm.lookNone(0);
            mAcch.SetWallNone();
            break;
        }

        case 3: {
            break;
        }
    }

    return 1;
}

int daNpc_Besu_c::talk(void* param_0) {
    switch (mMode) {
        case 0:
        case 1: {
            if (!mStagger.checkStagger()) {
                initTalk(mFlowNodeNo, NULL);
                mMode = 2;
            }
        }
        /* fallthrough */
        case 2: {
            if (!mStagger.checkStagger()) {
                if (mTwilight != 0 || mPlayerAngle == mCurAngle.y || mType == 5 || chkNurse()) {
                    if (talkProc(NULL, 0, NULL, 0) && mFlow.checkEndFlow()) {
                        mPlayerActorMngr.entry(daPy_getPlayerActorClass());
                        dComIfGp_event_reset();
                        mMode = 3;
                    }

                    mJntAnm.lookPlayer(0);
                    if ((mTwilight != 0) || (chkNurse())) {
                        mJntAnm.lookNone(0);
                    }
                } else {
                    mJntAnm.lookPlayer(0);
                    if (chkBesu3()) {
                        step(mPlayerAngle, 0x1C, 0x1D, 0xF, 0);
                    } else {
                        step(mPlayerAngle, 0x1C, 0x11, 0xF, 0);
                    }
                }
            }

            break;
        }

        case 3: {
            break;
        }
    }

    return 0;
}

static int daNpc_Besu_Create(void* i_this) {
    return static_cast<daNpc_Besu_c*>(i_this)->create();
}

static int daNpc_Besu_Delete(void* i_this) {
    return static_cast<daNpc_Besu_c*>(i_this)->Delete();
}

static int daNpc_Besu_Execute(void* i_this) {
    return static_cast<daNpc_Besu_c*>(i_this)->Execute();
}

static int daNpc_Besu_Draw(void* i_this) {
    return static_cast<daNpc_Besu_c*>(i_this)->Draw();
}

static int daNpc_Besu_IsDelete(void*) {
    return true;
}

static daNpc_Besu_Param_c l_HIO;


static actor_method_class daNpc_Besu_MethodTable = {
    (process_method_func)daNpc_Besu_Create,
    (process_method_func)daNpc_Besu_Delete,
    (process_method_func)daNpc_Besu_Execute,
    (process_method_func)daNpc_Besu_IsDelete,
    (process_method_func)daNpc_Besu_Draw,
};

actor_process_profile_definition g_profile_NPC_BESU = {
  fpcLy_CURRENT_e,         // mLayerID
  7,                       // mListID
  fpcPi_CURRENT_e,         // mListPrio
  PROC_NPC_BESU,           // mProcName
  &g_fpcLf_Method.base,   // sub_method
  sizeof(daNpc_Besu_c),    // mSize
  0,                       // mSizeOther
  0,                       // mParameters
  &g_fopAc_Method.base,    // sub_method
  323,                     // mPriority
  &daNpc_Besu_MethodTable, // sub_method
  0x00040108,              // mStatus
  fopAc_NPC_e,             // mActorType
  fopAc_CULLBOX_CUSTOM_e,  // cullType
};
