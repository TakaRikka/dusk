/**
 * @file d_a_npc_pachi_besu.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_npc_pachi_besu.h"
#include "d/actor/d_a_npc_pachi_taro.h"
#include "Z2AudioLib/Z2Instances.h"

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

enum evt_pachi_RES_File_ID {
    /* EVT */
    /* 0x3 */ EVT_EVT_PACHI_EVENT_LIST = 0x3,
};

enum RES_Name {
    /* 0x0 */ NONE,
    /* 0x1 */ BESU,
    /* 0x2 */ BESU_TW,
    /* 0x3 */ BESU0,
    /* 0x4 */ BESU1,
    /* 0x5 */ BESU2,
    /* 0x6 */ BESU3,
    /* 0x7 */ EVT_PACHI,
};

enum Face_Motion {
    /* 0x00 */ FACE_MOT_TALK_C,
    /* 0x01 */ FACE_MOT_TALK_A,
    /* 0x02 */ FACE_MOT_TALK_B,
    /* 0x03 */ FACE_MOT_TALK_N_B,
    /* 0x04 */ FACE_MOT_MOP_A_C,
    /* 0x05 */ FACE_MOT_MOP_A_B,
    /* 0x06 */ FACE_MOT_MOP_C,
    /* 0x07 */ FACE_MOT_NURSE_A,
    /* 0x08 */ FACE_MOT_MADTALK,
    /* 0x09 */ FACE_MOT_CUP_A,
    /* 0x0A */ FACE_MOT_SURPRISE,
    /* 0x0B */ FACE_MOT_WORRY,
    /* 0x0C */ FACE_MOT_YOKERU,
    /* 0x0D */ FACE_MOT_H_TALK_A,
    /* 0x0E */ FACE_MOT_H_TALK_B,
    /* 0x0F */ FACE_MOT_H_MADTALK,
    /* 0x10 */ FACE_MOT_TALK_N_A,
    /* 0x11 */ FACE_MOT_TALK_N_C,
    /* 0x12 */ FACE_MOT_GLARE_A,
    /* 0x13 */ FACE_MOT_H_SURPRISE,
    /* 0x14 */ FACE_MOT_AMAZED_A_B,
    /* 0x15 */ FACE_MOT_AMAZED_C,
    /* 0x16 */ FACE_MOT_CUP_B,
    /* 0x17 */ FACE_MOT_DEMO_WAIT,
    /* 0x18 */ FACE_MOT_DEMO_FEAR,
    /* 0x19 */ FACE_MOT_GLARE_WAIT,
    /* 0x1A */ FACE_MOT_H_WORRY,
    /* 0x1B */ FACE_MOT_H_BIKKURI,
    /* 0x1C */ FACE_MOT_NONE,
};

enum Motion {
    /* 0x00 */ MOT_WAIT_A,
    /* 0x01 */ MOT_WAIT_B,
    /* 0x02 */ MOT_TALK_A,
    /* 0x03 */ MOT_TALK_N_B,
    /* 0x04 */ MOT_TALK_N_B_2,
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

enum Type {
    /* 0x0 */ TYPE_0,
    /* 0x1 */ TYPE_1,
};

enum Event {
    /* 0x0 */ EVT_NONE,
    /* 0x1 */ EVT_TUTRIAL_TALK2,
};

static int l_bmdData[6][2] = {
    {BMDR_BESU, BESU},
    {BMDR_BESU_TW, BESU_TW},
    {BMDR_BESU_CLOTH, BESU2},
    {BMDR_BESUB, BESU2},
    {BMDR_BESU3, BESU3},
    {BMDR_BESU_CUP, BESU2},
};

static daNpcT_evtData_c l_evtList[2] = {
    {"", 0},
    {"TUTRIAL_TALK2", 7},
};

static char* l_resNameList[8] = {
    "",
    "Besu",
    "Besu_TW",
    "Besu0",
    "Besu1",
    "Besu2",
    "Besu3",
    "evt_pachi",
};

static s8 l_loadResPtrn0[6] = {
    BESU, BESU0, BESU2, BESU1, EVT_PACHI, -1,
};

static s8 l_loadResPtrn1[3] = {
    BESU0, BESU_TW, -1,
};

static s8 l_loadResPtrn2[3] = {
    BESU2, BESU0, -1,
};

static s8 l_loadResPtrn3[3] = {
    BESU0, BESU3, -1,
};

static s8 l_loadResPtrn4[7] = {
    BESU, BESU_TW, BESU0, BESU1, BESU2, BESU3, -1,
};

static s8* l_loadResPtrnList[2] = {
    l_loadResPtrn0,
    l_loadResPtrn4,
};

static daNpcT_faceMotionAnmData_c l_faceMotionAnmData[30] = {
    {-1, J3DFrameCtrl::EMode_NONE, NONE, 25, J3DFrameCtrl::EMode_LOOP, BESU0, 1},
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
    {BCK_BESU_DEMO_WAIT, J3DFrameCtrl::EMode_LOOP, BESU_TW, -1, J3DFrameCtrl::EMode_NONE, NONE, 0, 0},
    {BCK_BESU_DEMO_FEAR, J3DFrameCtrl::EMode_LOOP, BESU_TW, -1, J3DFrameCtrl::EMode_NONE, NONE, 0, 0},
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

char* daNpc_Pachi_Besu_c::mCutNameList[11] = {
    "",
    "TUTRIAL_BEGIN",
    "TUTRIAL_BEGIN_SKIP",
    "TUTRIAL_CLEAR",
    "TUTRIAL_TALK",
    "TUTRIAL_TALK2",
    "HIT_KAKASI_BODY",
    "TUTRIAL_SELECT_GIVEUP",
    "TUTRIAL_GIVEUP",
    "TUTRIAL_CONTINUE",
    "TUTRIAL_CAUTION",
};

daNpc_Pachi_Besu_c::cutFunc daNpc_Pachi_Besu_c::mCutList[11] = {
    NULL,
    &daNpc_Pachi_Besu_c::cutTutrialBegin,
    &daNpc_Pachi_Besu_c::cutTutrialBegin_Skip,
    &daNpc_Pachi_Besu_c::cutTutrialClear,
    &daNpc_Pachi_Besu_c::cutTalk,
    &daNpc_Pachi_Besu_c::cutTalk2,
    &daNpc_Pachi_Besu_c::cut1stHit_KakasiBody,
    &daNpc_Pachi_Besu_c::cutTutrialSelectGiveUp,
    &daNpc_Pachi_Besu_c::cutTutrialGiveUp,
    &daNpc_Pachi_Besu_c::cutTutrialContinue,
    &daNpc_Pachi_Besu_c::cutTutrialCaution,
};

daNpc_Pachi_Besu_c::~daNpc_Pachi_Besu_c() {
    if (mpMorf[0] != NULL) {
        mpMorf[0]->stopZelAnime();
    }

    deleteRes(l_loadResPtrnList[mType], (const char**)l_resNameList);
}

daNpc_Pachi_Besu_HIOParam const daNpc_Pachi_Besu_Param_c::m = {
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
    4,
    5,
    4,
    110.0f,
    0.0f,
    0.0f,
    0.0f,
    60,
    8,
    0,
    0,
    0,
    false,
    false,
    4.0f,
    -15.0f,
    0.0f,
    -15.0f,
    15.0f,
    30.0f,
    15.0f,
    70.0f,
};

cPhs__Step daNpc_Pachi_Besu_c::create() {
    daNpcT_ct(this, daNpc_Pachi_Besu_c, l_faceMotionAnmData, l_motionAnmData,
                       l_faceMotionSequenceData, 4, l_motionSequenceData, 4,
                       l_evtList, l_resNameList);


    mType = getType();
    mFlowNodeNo = getFlowNodeNo();
    mTwilight = false;

    cPhs__Step phase = (cPhs__Step)loadRes(l_loadResPtrnList[mType], (const char**)l_resNameList);
    if (phase == cPhs_COMPLEATE_e) {
        if (!fopAcM_entrySolidHeap(this, createHeapCallBack, 0x35B0)) {
            return cPhs_ERROR_e;
        }

        OS_REPORT("\t(%s:%d) flowNo:%d, PathID:%02x, BitSW:%02x<%08x> ", fopAcM_getProcNameString(this), mType, mFlowNodeNo, (getPathID() >> 32) & 0xFF,
                  getBitSW() & 0xFF, fopAcM_GetParam(this));

        if (isDelete()) {
            OS_REPORT("===>isDelete:TRUE\n");
            return cPhs_ERROR_e;
        }

        fopAcM_SetMtx(this, mpMorf[0]->getModel()->getBaseTRMtx());
        fopAcM_setCullSizeBox(this, -300.0f, -50.0f, -300.0f, 300.0f, 450.0f, 300.0f);
        mSound.init(&current.pos, &eyePos, 3, 1);

        mAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1, &mAcchCir, fopAcM_GetSpeed_p(this),
                  fopAcM_GetAngle_p(this), fopAcM_GetShapeAngle_p(this));
        mCcStts.Init(daNpc_Pachi_Besu_Param_c::m.common.weight, 0, this);
        mCyl.Set(mCcDCyl);
        mCyl.SetStts(&mCcStts);
        mCyl.SetTgHitCallback(tgHitCallBack);
        mAcch.CrrPos(dComIfG_Bgsp());
        mGndChk = mAcch.m_gnd;
        mGroundH = mAcch.GetGroundH();

        if (mGroundH != -G_CM3D_F_INF) {
            setEnvTevColor();
            setRoomNo();
        }

        current.pos = home.pos;
        old.pos = current.pos;

        reset();
        mCreating = true;
        Execute();
        mCreating = false;
    }

    return phase;
}

int daNpc_Pachi_Besu_c::CreateHeap() {
    J3DModelData* modelData = static_cast<J3DModelData*>(dComIfG_getObjectRes(l_resNameList[l_bmdData[0][1]], l_bmdData[0][0]));
    if (modelData == NULL) {
        return 1;
    }

    mpMorf[0] = new mDoExt_McaMorfSO(modelData, NULL, NULL, NULL, -1, 1.0f, 0, -1, &mSound, 0x80000, 0x11020284);

    if (mpMorf[0] == NULL || mpMorf[0]->getModel() == NULL) {
        return 0;
    }

    J3DModel* model = mpMorf[0]->getModel();
    for (u16 i = 0; i < modelData->getJointNum(); i++) {
        modelData->getJointNodePointer(i)->setCallBack(ctrlJointCallBack);
    }
    model->setUserArea((u32)this);

    mpMatAnm[0] = new daNpcT_MatAnm_c();
    if (mpMatAnm[0] == NULL) {
        return 0;
    }

    if (mTwilight) {
        if (setFaceMotionAnm(0x1A, false) && setMotionAnm(0x18, 0.0f, FALSE)) {
            return 1;
        }

        return 0;
    } else {
        if (setFaceMotionAnm(3, false) && setMotionAnm(0, 0.0f, FALSE)) {
            return 1;
        }

        return 0;
    }
}

int daNpc_Pachi_Besu_c::Delete() {
    this->~daNpc_Pachi_Besu_c();
    return 1;
}

int daNpc_Pachi_Besu_c::Execute() {
    return execute();
}

int daNpc_Pachi_Besu_c::Draw() {
    if (mpMatAnm[0] != NULL) {
        J3DModelData* modelData = mpMorf[0]->getModel()->getModelData();
        modelData->getMaterialNodePointer(getEyeballMaterialNo())->setMaterialAnm(mpMatAnm[0]);
    }

    return draw(
#if DEBUG
        chkAction(&daNpc_Pachi_Besu_c::test),
#else
        FALSE,
#endif
        FALSE,
        mRealShadowSize, NULL, 100.0f, FALSE, FALSE, FALSE
    );
}

int daNpc_Pachi_Besu_c::createHeapCallBack(fopAc_ac_c* a_this) {
    return static_cast<daNpc_Pachi_Besu_c*>(a_this)->CreateHeap();
}

int daNpc_Pachi_Besu_c::ctrlJointCallBack(J3DJoint* i_joint, int param_2) {
    if (param_2 == 0) {
        J3DModel* model = j3dSys.getModel();
        daNpc_Pachi_Besu_c* i_this = reinterpret_cast<daNpc_Pachi_Besu_c*>(model->getUserArea());
        if (i_this != 0) {
            i_this->ctrlJoint(i_joint, model);
        }
    }

    return 1;
}

u8 daNpc_Pachi_Besu_c::getType() {
    switch (fopAcM_GetParam(this) & 0xFF) {
        case 0:
            return TYPE_0;

        default:
            return TYPE_1;
    }
}

BOOL daNpc_Pachi_Besu_c::isDelete() {
    switch (mType) {
        case TYPE_0:
            return FALSE;

        default:
            return FALSE;
    }
}

void daNpc_Pachi_Besu_c::reset() {
    int size = (u8*)&field_0xfc8 - (u8*)&mNextAction;

    if (mpMatAnm[0] != NULL) {
        mpMatAnm[0]->initialize();
    }

    initialize();

    for (int i = 0; i < 2; i++) {
        mActorMngrs[i].initialize();
    }

    memset(&mNextAction, 0, size);

    switch (mType) {
        case TYPE_0:
            setFMotion_None();
            break;
    }

    setAngle(home.angle.y);
}

void daNpc_Pachi_Besu_c::afterJntAnm(int param_1) {
    if (param_1 == 1) {
        mDoMtx_stack_c::YrotM(mStagger.getAngleX(1));
        mDoMtx_stack_c::ZrotM(mStagger.getAngleZ(1));
    } else if (param_1 == 4) {
        mDoMtx_stack_c::YrotM(mStagger.getAngleX(0));
        mDoMtx_stack_c::ZrotM(mStagger.getAngleZ(0));
    }
}

void daNpc_Pachi_Besu_c::setParam() {
    selectAction();
    srchActors();

    s16 talk_distance = daNpc_Pachi_Besu_Param_c::m.common.talk_distance;
    s16 talk_angle = daNpc_Pachi_Besu_Param_c::m.common.talk_angle;

    attention_info.distances[fopAc_attn_LOCK_e] = daNpcT_getDistTableIdx(daNpc_Pachi_Besu_Param_c::m.common.attention_distance, daNpc_Pachi_Besu_Param_c::m.common.attention_angle);
    attention_info.distances[fopAc_attn_TALK_e] = attention_info.distances[fopAc_attn_LOCK_e];
    attention_info.distances[fopAc_attn_SPEAK_e] = daNpcT_getDistTableIdx(talk_distance, talk_angle);
    attention_info.flags = fopAc_AttnFlag_SPEAK_e | fopAc_AttnFlag_TALK_e;

    scale.set(daNpc_Pachi_Besu_Param_c::m.common.scale, daNpc_Pachi_Besu_Param_c::m.common.scale, daNpc_Pachi_Besu_Param_c::m.common.scale);
    mCcStts.SetWeight(daNpc_Pachi_Besu_Param_c::m.common.weight);
    mCylH = daNpc_Pachi_Besu_Param_c::m.common.height;
    mWallR = daNpc_Pachi_Besu_Param_c::m.common.width;
    mAttnFovY = daNpc_Pachi_Besu_Param_c::m.common.fov;
    mAcchCir.SetWallR(mWallR);
    mAcchCir.SetWallH(daNpc_Pachi_Besu_Param_c::m.common.knee_length);
    mRealShadowSize = daNpc_Pachi_Besu_Param_c::m.common.real_shadow_size;
    mExpressionMorfFrame = daNpc_Pachi_Besu_Param_c::m.common.expression_morf_frame;
    mMorfFrames = daNpc_Pachi_Besu_Param_c::m.common.morf_frame;
    gravity = daNpc_Pachi_Besu_Param_c::m.common.gravity;
}

static daNpc_Pachi_Besu_Param_c l_HIO;

void daNpc_Pachi_Besu_c::afterMoved() {
    fopAc_ac_c* actor_p1 = mActorMngrs[0].getActorP();
    fopAc_ac_c* actor_p2 = mActorMngrs[1].getActorP();

    switch (mFMotion) {
        case 1:
            mMotionSeqMngr.setNo(MOT_TALK_N_B, -1.0f, FALSE, 0);
            mFaceMotionSeqMngr.setNo(FACE_MOT_TALK_B, -1.0f, FALSE, 0);
            setFMotion_None();
            break;

        case 2:
            mMotionSeqMngr.setNo(MOT_WAIT_A, -1.0f, FALSE, 0);
            mFaceMotionSeqMngr.setNo(FACE_MOT_H_MADTALK, -1.0f, FALSE, 0);

            if (actor_p2 != NULL) {
                mJntAnm.lookActor(actor_p2, 0.0f, 0);
            }

            setFMotion_None();
            break;
        
        case 3:
            if (actor_p1 != NULL) {
                static cXyz center;
                center = *fopAcM_GetPosition_p(actor_p1);
                center.y += 100.0f;
                mJntAnm.lookPos(&center, 0);
            }

            setFMotion_None();
            break;

        case 4:
            mJntAnm.lookNone(0);
            setFMotion_None();
            break;
    }
}

BOOL daNpc_Pachi_Besu_c::checkChangeEvt() {
    return FALSE;
}

void daNpc_Pachi_Besu_c::setAfterTalkMotion() {
    int i_faceMotion = FACE_MOT_NONE;

    switch (mFaceMotionSeqMngr.getNo()) {
        case FACE_MOT_TALK_A:
            i_faceMotion = FACE_MOT_H_TALK_A;
            break;

        case FACE_MOT_TALK_B:
            i_faceMotion = FACE_MOT_H_TALK_B;
            break;

        case FACE_MOT_MADTALK:
            i_faceMotion = FACE_MOT_H_MADTALK;
            break;

        case FACE_MOT_SURPRISE:
            i_faceMotion = FACE_MOT_H_SURPRISE;
            break;

        case FACE_MOT_WORRY:
            i_faceMotion = FACE_MOT_H_WORRY;
            break;
    }

    mFaceMotionSeqMngr.setNo(i_faceMotion, -1.0f, FALSE, 0);
}

void daNpc_Pachi_Besu_c::srchActors() {
    switch (mType) {
        case TYPE_0:
            fopAc_ac_c* actor_p = mActorMngrs[0].getActorP();
            if (actor_p == NULL) {
                mActorMngrs[0].entry(getNearestActorP(PROC_NPC_PACHI_TARO));
            }

            actor_p = mActorMngrs[1].getActorP();
            if (actor_p == NULL) {
                mActorMngrs[1].entry(getNearestActorP(PROC_NPC_PACHI_MARO));
            }
            break;
    }
}

BOOL daNpc_Pachi_Besu_c::evtTalk() {
    if (chkAction(&daNpc_Pachi_Besu_c::talk)) {
        (this->*mAction)(NULL);
    } else {
        mPreItemNo = 0;

        if (dComIfGp_event_chkTalkXY()) {
            if (!dComIfGp_evmng_ChkPresentEnd()) {
                return TRUE;
            }

            dComIfGp_event_reset();
            return TRUE;
        }

        setAction(&daNpc_Pachi_Besu_c::talk);
    }

    return TRUE;
}

BOOL daNpc_Pachi_Besu_c::evtCutProc() {
    BOOL rv;
    int staffId = dComIfGp_getEventManager().getMyStaffId("PA_Besu", this, -1);

    if (staffId != -1) {
        mStaffId = staffId;
        int actIdx = dComIfGp_getEventManager().getMyActIdx(mStaffId, mCutNameList, 11, 0, 0);
        if ((this->*(mCutList[actIdx]))(mStaffId) != 0) {
            dComIfGp_getEventManager().cutEnd(mStaffId);
        }

        rv = TRUE;
    } else {
        rv = FALSE;
    }

    return rv;
}

void daNpc_Pachi_Besu_c::action() {
    fopAc_ac_c* actor_p = NULL;

    if (!mTwilight) {
        actor_p = hitChk(&mCyl, 0xFFFFFFFF);
    }

    if (actor_p != NULL) {
        mStagger.setParam(this, actor_p, mCurAngle.y);
        setDamage(0, 12, 18);
        mStagger.setPower(0.0f);
        mDamageTimerStart = 0;
        mJntAnm.lookNone(1);
    }

    if (mStagger.checkRebirth()) {
        mStagger.initialize();
        mMode = MODE_INIT;
    }

    if (mNextAction) {
        if (mAction == mNextAction) {
            (this->*mAction)(NULL);
        } else {
            setAction(mNextAction);
        }
    }
}

void daNpc_Pachi_Besu_c::beforeMove() {
    if (checkHide() || mNoDraw) {
        attention_info.flags = 0;
    }
}

void daNpc_Pachi_Besu_c::setAttnPos() {
    cXyz sp38(10.0f, 30.0f, 0.0f);

    BOOL bVar1 = 0;
    if (mMotionSeqMngr.getNo() == MOT_YOKERU && !mMotionSeqMngr.checkEndSequence()) {
        bVar1 = TRUE;
    }
    
    mStagger.calc(bVar1);
    f32 fVar1 = cM_s2rad(mCurAngle.y - field_0xd7e.y);
    J3DModel* model = mpMorf[0]->getModel();

    mJntAnm.setParam(this, model, &sp38, getBackboneJointNo(), getNeckJointNo(), getHeadJointNo(),
                     daNpc_Pachi_Besu_Param_c::m.common.body_angleX_min, daNpc_Pachi_Besu_Param_c::m.common.body_angleX_max,
                     daNpc_Pachi_Besu_Param_c::m.common.body_angleY_min, daNpc_Pachi_Besu_Param_c::m.common.body_angleY_max,
                     daNpc_Pachi_Besu_Param_c::m.common.head_angleX_min, daNpc_Pachi_Besu_Param_c::m.common.head_angleX_max,
                     daNpc_Pachi_Besu_Param_c::m.common.head_angleY_min, daNpc_Pachi_Besu_Param_c::m.common.head_angleY_max,
                     daNpc_Pachi_Besu_Param_c::m.common.neck_rotation_ratio, 0.0f, NULL);
    mJntAnm.calcJntRad(0.2f, 1.0f, fVar1);
    setMtx();

    mDoMtx_stack_c::copy(mpMorf[0]->getModel()->getAnmMtx(getHeadJointNo()));
    mDoMtx_stack_c::multVec(&sp38, &eyePos);
    mJntAnm.setEyeAngleX(eyePos, 1.0f, 0);
    mJntAnm.setEyeAngleY(eyePos, mCurAngle.y, TRUE, 1.0f, 0);

    attention_info.position = current.pos;
    attention_info.position.y += daNpc_Pachi_Besu_Param_c::m.common.attention_offset;
}

void daNpc_Pachi_Besu_c::setCollision() {
    cXyz sp28;

    if (!mHide) {
        u32 tgType = 0xD8FBFDFF;
        u32 tgSPrm = 0x1F;

        if (mTwilight) {
            tgType = 0;
            tgSPrm = 0;
        } else {
            if (mStagger.checkStagger()) {
                tgType = 0;
                tgSPrm = 0;
            }
        }

        mCyl.SetCoSPrm(0x79);
        mCyl.SetTgType(tgType);
        mCyl.SetTgSPrm(tgSPrm);
        mCyl.OnTgNoHitMark();
        sp28.setall(0.0f);
        mCyl.SetH(mCylH);
        mCyl.SetR(mWallR);
        mDoMtx_stack_c::YrotS(mCurAngle.y);
        mDoMtx_stack_c::multVec(&sp28, &sp28);
        sp28 += current.pos;
        mCyl.SetC(sp28);
        dComIfG_Ccsp()->Set(&mCyl);
    }

    mCyl.ClrCoHit();
    mCyl.ClrTgHit();
}

int daNpc_Pachi_Besu_c::drawDbgInfo() {
    return 0;
}

BOOL daNpc_Pachi_Besu_c::selectAction() {
    mNextAction = NULL;

    switch (mType) {
        case TYPE_0:
            mNextAction = &daNpc_Pachi_Besu_c::wait;
            break;

        default:
            mNextAction = &daNpc_Pachi_Besu_c::wait;
            break;
    }

    return TRUE;
}

BOOL daNpc_Pachi_Besu_c::chkAction(actionFunc action) {
    return mAction == action;
}

BOOL daNpc_Pachi_Besu_c::setAction(actionFunc action) {
    mMode = MODE_EXIT;

    if (mAction) {
        (this->*mAction)(NULL);
    }

    mMode = MODE_ENTER;
    mAction = action;

    if (mAction) {
        (this->*mAction)(NULL);
    }

    return TRUE;
}

BOOL daNpc_Pachi_Besu_c::wait(void* param_1) {
    switch (mMode) {
        case MODE_ENTER:
        case MODE_INIT:
            mTalk = 0;

            if (!mStagger.checkStagger()) {
                mFaceMotionSeqMngr.setNo(FACE_MOT_NONE, -1.0f, FALSE, 0);
                mMotionSeqMngr.setNo(MOT_WAIT_A, -1.0f, FALSE, 0);
                mMode = MODE_RUN;
            }
            // fallthrough
        case MODE_RUN:
            mTalk = 0;

            if (!mStagger.checkStagger()) {
                fopAc_ac_c* actor_p = mPlayerActorMngr.getActorP();
                if (actor_p != NULL && !mTwilight) {
                    mJntAnm.lookPlayer(0);

                    if (!chkActorInSight(mPlayerActorMngr.getActorP(), mAttnFovY, mCurAngle.y)) {
                        mJntAnm.lookNone(0);
                    }

                    if (!srchPlayerActor() && home.angle.y == mCurAngle.y) {
                        mMode = MODE_INIT;
                    }
                } else {
                    mJntAnm.lookNone(0);

                    if (home.angle.y != mCurAngle.y) {
                        if (field_0xe34 != 0) {
                            if (step(home.angle.y, 0x1C, 0x11, 0xF, 0)) {
                                mMode = MODE_INIT;
                            }
                        } else {
                            setAngle(home.angle.y);
                            mMode = MODE_INIT;
                        }

                        attention_info.flags = 0;
                    } else if (!mTwilight) {
                        srchPlayerActor();
                    }
                }

                mJntAnm.getMode();
            }
            break;

        case MODE_EXIT:
            break;
    }

    return TRUE;
}

BOOL daNpc_Pachi_Besu_c::talk(void* param_1) {
    switch (mMode) {
        case MODE_ENTER:
        case MODE_INIT:
            if (!mStagger.checkStagger()) {
                fopAc_ac_c* actor_p = mActorMngrs[0].getActorP();
                if (actor_p != NULL) {
                    ((daNpc_Pachi_Taro_c*)actor_p)->clrMesPat();
                }

                mMode = MODE_RUN;
            }
            // fallthrough
        case MODE_RUN:
            mEvtNo = EVT_TUTRIAL_TALK2;
            field_0xf9c = mEvtNo;
            evtChange();
            break;

        case MODE_EXIT:
            break;
    }

    return FALSE;
}

BOOL daNpc_Pachi_Besu_c::test(void* param_1) {
    // DEBUG NONMATCHING
}

BOOL daNpc_Pachi_Besu_c::_turn_to_link(s16 param_1) {
    cXyz pos(*fopAcM_GetPosition_p(daPy_getPlayerActorClass()));
    return _turn_pos(pos, param_1);
}

BOOL daNpc_Pachi_Besu_c::_turn_pos(cXyz const& i_pos, s16 param_2) {
    int rv = cLib_chaseAngleS(&mCurAngle.y, cLib_targetAngleY(&current.pos, &i_pos), param_2);
    current.angle.y = mCurAngle.y;
    shape_angle.y = mCurAngle.y;
    return rv;
}

BOOL daNpc_Pachi_Besu_c::_turn_pos(cXyz const& i_pos) {
    s16 sVar1 = cLib_targetAngleY(&current.pos, &i_pos);
    mCurAngle.y = sVar1;
    current.angle.y = sVar1;
    shape_angle.y = sVar1;

    return TRUE;
}

BOOL daNpc_Pachi_Besu_c::cutTutrialBegin(int i_staffId) {
    dEvent_manager_c& eventManager = dComIfGp_getEventManager();
    int* cutId = dComIfGp_evmng_getMyIntegerP(i_staffId, "cutId");

    if (cutId == NULL) {
        OS_REPORT("-----CUT FALSE\n");
        return TRUE;
    }

    if (eventManager.getIsAddvance(i_staffId)) {
        _cutTutrialBegin_Init(*cutId);
    }

    return _cutTutrialBegin_Main(*cutId);
}

BOOL daNpc_Pachi_Besu_c::_cutTutrialBegin_Init(int const& i_cutId) {
    return TRUE;
}

BOOL daNpc_Pachi_Besu_c::_cutTutrialBegin_Main(int const& i_cutId) {
    return FALSE;
}

BOOL daNpc_Pachi_Besu_c::cutTutrialBegin_Skip(int i_staffId) {
    dEvent_manager_c& eventManager = dComIfGp_getEventManager();
    int* cutId = dComIfGp_evmng_getMyIntegerP(i_staffId, "cutId");

    if (cutId == NULL) {
        OS_REPORT("-----CUT FALSE\n");
        return TRUE;
    }

    if (eventManager.getIsAddvance(i_staffId)) {
        _cutTutrialBegin_Skip_Init(*cutId);
    }

    return _cutTutrialBegin_Skip_Main(*cutId);
}

BOOL daNpc_Pachi_Besu_c::_cutTutrialBegin_Skip_Init(int const& i_cutId) {
    return TRUE;
}

BOOL daNpc_Pachi_Besu_c::_cutTutrialBegin_Skip_Main(int const& i_cutId) {
    return FALSE;
}

BOOL daNpc_Pachi_Besu_c::cutTutrialClear(int i_staffId) {
    dEvent_manager_c& eventManager = dComIfGp_getEventManager();
    int* cutId = dComIfGp_evmng_getMyIntegerP(i_staffId, "cutId");

    if (cutId == NULL) {
        OS_REPORT("-----CUT FALSE\n");
        return TRUE;
    }

    if (eventManager.getIsAddvance(i_staffId)) {
        _cutTutrialClear_Init(*cutId);
    }

    return _cutTutrialClear_Main(*cutId);
}

BOOL daNpc_Pachi_Besu_c::_cutTutrialClear_Init(int const& i_cutId) {
    switch (i_cutId) {
        case 5:
            mJntAnm.lookNone(0);
            break;

        case 10:
            break;

        case 20:
            mMotionSeqMngr.setNo(MOT_TALK_N_B, -1.0f, FALSE, 0);
            mFaceMotionSeqMngr.setNo(2, -1.0f, FALSE, 0);
            break;
    }

    return TRUE;
}

BOOL daNpc_Pachi_Besu_c::_cutTutrialClear_Main(int const& i_cutId) {
    BOOL rv = FALSE;

    switch (i_cutId) {
        case 5:
            rv = TRUE;
            break;
            
        case 10: {
            cXyz pos(894.916f, 903.411f, -2040.36f);

            if (_turn_pos(pos, 0x1000)) {
                rv = TRUE;
            }
            break;
        }

        case 20:
            break;
    }

    return rv;
}

BOOL daNpc_Pachi_Besu_c::cutTalk(int i_staffId) {
    dEvent_manager_c& eventManager = dComIfGp_getEventManager();
    int* cutId = dComIfGp_evmng_getMyIntegerP(i_staffId, "cutId");

    if (cutId == NULL) {
        OS_REPORT("-----CUT FALSE\n");
        return TRUE;
    }

    if (eventManager.getIsAddvance(i_staffId)) {
        _cutTalk_Init(*cutId);
    }

    return _cutTalk_Main(*cutId);
}

BOOL daNpc_Pachi_Besu_c::_cutTalk_Init(int const& i_cutId) {
    return TRUE;
}

BOOL daNpc_Pachi_Besu_c::_cutTalk_Main(int const& i_cutId) {
    BOOL rv = FALSE;

    switch (i_cutId) {
        case 5:
            if (_turn_to_link(0xA00)) {
                rv = TRUE;
            }
            break;

        case 10:
            break;
    }

    return rv;
}

BOOL daNpc_Pachi_Besu_c::cutTalk2(int i_staffId) {
    dEvent_manager_c& eventManager = dComIfGp_getEventManager();
    int* cutId = dComIfGp_evmng_getMyIntegerP(i_staffId, "cutId");

    if (cutId == NULL) {
        OS_REPORT("-----CUT FALSE\n");
        return TRUE;
    }

    if (eventManager.getIsAddvance(i_staffId)) {
        _cutTalk2_Init(*cutId);
    }

    return _cutTalk2_Main(*cutId);
}

BOOL daNpc_Pachi_Besu_c::_cutTalk2_Init(int const& i_cutId) {
    return TRUE;
}

BOOL daNpc_Pachi_Besu_c::_cutTalk2_Main(int const& i_cutId) {
    BOOL rv = FALSE;

    switch (i_cutId) {
        case 5:
            if (_turn_to_link(0xA00)) {
                rv = TRUE;
            }
            break;

        case 10:
            break;
    }

    return rv;
}

BOOL daNpc_Pachi_Besu_c::cut1stHit_KakasiBody(int i_staffId) {
    dEvent_manager_c& eventManager = dComIfGp_getEventManager();
    int* cutId = dComIfGp_evmng_getMyIntegerP(i_staffId, "cutId");

    if (cutId == NULL) {
        OS_REPORT("-----CUT FALSE\n");
        return TRUE;
    }

    if (eventManager.getIsAddvance(i_staffId)) {
        _cut1stHit_KakasiBody_Init(*cutId);
    }

    return _cut1stHit_KakasiBody_Main(*cutId);
}

BOOL daNpc_Pachi_Besu_c::_cut1stHit_KakasiBody_Init(int const& i_cutId) {
    switch (i_cutId) {
        case 5:
            mJntAnm.lookNone(0);
            break;

        case 10:
            break;

        case 20:
            break;
    }

    return TRUE;
}

BOOL daNpc_Pachi_Besu_c::_cut1stHit_KakasiBody_Main(int const& i_cutId) {
    BOOL rv = FALSE;

    switch (i_cutId) {
        case 5:
            rv = TRUE;
            break;
        
        case 10: {
            cXyz pos(894.916f, 903.411f, -2040.36f);

            if (_turn_pos(pos, 0x1000)) {
                rv = TRUE;
            }
            break;
        }

        case 20:
            break;
    }

    return rv;
}

BOOL daNpc_Pachi_Besu_c::cutTutrialSelectGiveUp(int i_staffId) {
    dEvent_manager_c& eventManager = dComIfGp_getEventManager();
    int* cutId = dComIfGp_evmng_getMyIntegerP(i_staffId, "cutId");

    if (cutId == NULL) {
        OS_REPORT("-----CUT FALSE\n");
        return TRUE;
    }

    if (eventManager.getIsAddvance(i_staffId)) {
        _cutTutrialSelectGiveUp_Init(*cutId);
    }

    return _cutTutrialSelectGiveUp_Main(*cutId);
}

BOOL daNpc_Pachi_Besu_c::_cutTutrialSelectGiveUp_Init(int const& i_cutId) {
    return TRUE;
}

BOOL daNpc_Pachi_Besu_c::_cutTutrialSelectGiveUp_Main(int const& i_cutId) {
    BOOL rv = FALSE;

    switch (i_cutId) {
        case 10:
            if (_turn_pos(mTagPos)) {
                mJntAnm.lookPos(&mLookPos, 1);
                rv = TRUE;
            }
            break;
        
        case 11:
            rv = TRUE;
            break;

        case 20:
            break;
    }

    return rv;
}

BOOL daNpc_Pachi_Besu_c::cutTutrialGiveUp(int i_staffId) {
    dEvent_manager_c& eventManager = dComIfGp_getEventManager();
    int* cutId = dComIfGp_evmng_getMyIntegerP(i_staffId, "cutId");

    if (cutId == NULL) {
        OS_REPORT("-----CUT FALSE\n");
        return TRUE;
    }

    if (eventManager.getIsAddvance(i_staffId)) {
        _cutTutrialGiveUp_Init(*cutId);
    }

    return _cutTutrialGiveUp_Main(*cutId);
}

BOOL daNpc_Pachi_Besu_c::_cutTutrialGiveUp_Init(int const& i_cutId) {
    return TRUE;
}

BOOL daNpc_Pachi_Besu_c::_cutTutrialGiveUp_Main(int const& i_cutId) {
    BOOL rv = FALSE;

    switch (i_cutId) {
        case 5:
            rv = TRUE;
            break;
        
        case 10:
            rv = TRUE;
            break;

        case 20:
            break;
    }

    return rv;
}

BOOL daNpc_Pachi_Besu_c::cutTutrialContinue(int i_staffId) {
    dEvent_manager_c& eventManager = dComIfGp_getEventManager();
    int* cutId = dComIfGp_evmng_getMyIntegerP(i_staffId, "cutId");

    if (cutId == NULL) {
        OS_REPORT("-----CUT FALSE\n");
        return TRUE;
    }

    if (eventManager.getIsAddvance(i_staffId)) {
        _cutTutrialContinue_Init(*cutId);
    }

    return _cutTutrialContinue_Main(*cutId);
}

BOOL daNpc_Pachi_Besu_c::_cutTutrialContinue_Init(int const& i_cutId) {
    return TRUE;
}

BOOL daNpc_Pachi_Besu_c::_cutTutrialContinue_Main(int const& i_cutId) {
    switch (i_cutId) {
        case 10:
            _turn_pos(mTagPos, 0x1000);
            break;
    }

    return FALSE;
}

BOOL daNpc_Pachi_Besu_c::cutTutrialCaution(int i_staffId) {
    dEvent_manager_c& eventManager = dComIfGp_getEventManager();
    int* cutId = dComIfGp_evmng_getMyIntegerP(i_staffId, "cutId");

    if (cutId == NULL) {
        OS_REPORT("-----CUT FALSE\n");
        return TRUE;
    }

    if (eventManager.getIsAddvance(i_staffId)) {
        _cutTutrialCaution_Init(*cutId);
    }

    return _cutTutrialCaution_Main(*cutId);
}

BOOL daNpc_Pachi_Besu_c::_cutTutrialCaution_Init(int const& i_cutId) {
    return TRUE;
}

BOOL daNpc_Pachi_Besu_c::_cutTutrialCaution_Main(int const& i_cutId) {
    BOOL rv = FALSE;

    switch (i_cutId) {
        case 5:
            if (_turn_to_link(0xD00)) {
                rv = TRUE;
            }
            break;
        
        case 10:
            rv = TRUE;
            break;
    }

    return rv;
}

static int daNpc_Pachi_Besu_Create(void* a_this) {
    return static_cast<daNpc_Pachi_Besu_c*>(a_this)->create();
}

static int daNpc_Pachi_Besu_Delete(void* a_this) {
    return static_cast<daNpc_Pachi_Besu_c*>(a_this)->Delete();
}

static int daNpc_Pachi_Besu_Execute(void* a_this) {
    return static_cast<daNpc_Pachi_Besu_c*>(a_this)->Execute();
}

static int daNpc_Pachi_Besu_Draw(void* a_this) {
    return static_cast<daNpc_Pachi_Besu_c*>(a_this)->Draw();
}

static int daNpc_Pachi_Besu_IsDelete(void* a_this) {
    return 1;
}

AUDIO_INSTANCES;

static actor_method_class daNpc_Pachi_Besu_MethodTable = {
    (process_method_func)daNpc_Pachi_Besu_Create,
    (process_method_func)daNpc_Pachi_Besu_Delete,
    (process_method_func)daNpc_Pachi_Besu_Execute,
    (process_method_func)daNpc_Pachi_Besu_IsDelete,
    (process_method_func)daNpc_Pachi_Besu_Draw,
};

extern actor_process_profile_definition g_profile_NPC_PACHI_BESU = {
  fpcLy_CURRENT_e,               // mLayerID
  7,                             // mListID
  fpcPi_CURRENT_e,               // mListPrio
  PROC_NPC_PACHI_BESU,           // mProcName
  &g_fpcLf_Method.base,         // sub_method
  sizeof(daNpc_Pachi_Besu_c),    // mSize
  0,                             // mSizeOther
  0,                             // mParameters
  &g_fopAc_Method.base,          // sub_method
  368,                           // mPriority
  &daNpc_Pachi_Besu_MethodTable, // sub_method
  0x00044108,                    // mStatus
  fopAc_NPC_e,                   // mActorType
  fopAc_CULLBOX_CUSTOM_e,        // cullType
};
