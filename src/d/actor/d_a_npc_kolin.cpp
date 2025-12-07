/**
 * @file d_a_npc_kolin.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_npc_kolin.h"
#include "Z2AudioLib/Z2Instances.h"
#include "d/actor/d_a_npc_len.h"
#include "d/actor/d_a_horse.h"
#include "d/actor/d_a_tag_evtarea.h"

enum Kolin_RES_File_ID {
    /* BCK */
    /* 0x06 */ BCK_KOLIN_F_TALK_A = 0x6,
    /* 0x07 */ BCK_KOLIN_STEP,
    /* 0x08 */ BCK_KOLIN_WAIT_A,

    /* BMDR */
    /* 0x0B */ BMDR_KOLIN = 0xB,

    /* BTK */
    /* 0x0E */ BTK_KOLIN = 0xE,

    /* BTP */
    /* 0x11 */ BTP_KOLIN = 0x11,
    /* 0x12 */ BTP_KOLIN_F_TALK_A,
};

enum Kolin_TW_RES_File_ID {
    /* BCK */
    /* 0x06 */ BCK_KOLIN_DEMO_FEAR = 0x6,
    /* 0x07 */ BCK_KOLIN_DEMO_WAIT,
    /* 0x08 */ BCK_KOLIN_F_DEMO_FEAR,
    /* 0x09 */ BCK_KOLIN_F_DEMO_WAIT,

    /* BMDR */
    /* 0x0C */ BMDR_KOLIN_TW = 0xC,

    /* BTK */
    /* 0x0F */ BTK_KOLIN_DEMO_FEAR = 0xF,
    /* 0x10 */ BTK_KOLIN_DEMO_WAIT,

    /* BTP */
    /* 0x13 */ BTP_KOLIN_F_DEMO_FEAR = 0x13,
    /* 0x14 */ BTP_KOLIN_F_DEMO_WAIT,
};

enum Kolin1_RES_File_ID {
    /* BCK */ 
    /* 0x06 */ BCK_KOLIN_CLUP = 0x6,
    /* 0x07 */ BCK_KOLIN_F_CLUP,
    /* 0x08 */ BCK_KOLIN_F_HAPPY_TALK,
    /* 0x09 */ BCK_KOLIN_F_KIZUKU,
    /* 0x0A */ BCK_KOLIN_F_NORMAL_TALK,
    /* 0x0B */ BCK_KOLIN_F_STONE,
    /* 0x0C */ BCK_KOLIN_F_WALK_A,
    /* 0x0D */ BCK_KOLIN_FH_KIZUKU,
    /* 0x0E */ BCK_KOLIN_FH_NORMAL_TALK,
    /* 0x0F */ BCK_KOLIN_FH_WAIT_D,
    /* 0x10 */ BCK_KOLIN_HAIHAI,
    /* 0x11 */ BCK_KOLIN_HAPPY_TALK,
    /* 0x12 */ BCK_KOLIN_HAPPY_WAIT,
    /* 0x13 */ BCK_KOLIN_NORMAL_TALK,
    /* 0x14 */ BCK_KOLIN_NOZOKU,
    /* 0x15 */ BCK_KOLIN_RUN,
    /* 0x16 */ BCK_KOLIN_STONE,
    /* 0x17 */ BCK_KOLIN_TALK_A,
    /* 0x18 */ BCK_KOLIN_TURN,
    /* 0x19 */ BCK_KOLIN_WAIT_B,
    /* 0x1A */ BCK_KOLIN_WAIT_C,
    /* 0x1B */ BCK_KOLIN_WAIT_D,
    /* 0x1C */ BCK_KOLIN_WALK_A,
    /* 0x1D */ BCK_KOLIN_WALK_B,

    /* BTK */
    /* 0x20 */ BTK_KOLIN_CLUP = 0x20,
    /* 0x21 */ BTK_KOLIN_NOZOKU,
    /* 0x22 */ BTK_KOLIN_STONE,
    /* 0x23 */ BTK_KOLIN_WALK_A,
    
    /* BTP */
    /* 0x26 */ BTP_KOLIN_F_CLUP = 0x26,
    /* 0x27 */ BTP_KOLIN_F_KIZUKU,
    /* 0x28 */ BTP_KOLIN_F_STONE,
    /* 0x29 */ BTP_KOLIN_F_WALK_A,
    /* 0x2A */ BTP_KOLIN_FH_WAIT_D,

    /* EVT */
    /* 0x2D */ EVT_KOLIN1_EVENT_LIST = 0x2D,
};

enum Kolin2_RES_File_ID {
    /* BCK */
    /* 0x06 */ BCK_KOLIN_CLOTH_TRY = 0x6,
    /* 0x07 */ BCK_KOLIN_CLOTH_WAIT,
    /* 0x08 */ BCK_KOLIN_F_CLOTH_TRY,

    /* BMDR */
    /* 0x0B */ BMDR_KOLIN_CLOTH = 0xB,

    /* BTK */
    /* 0x0E */ BTK_KOLIN_CLOTH_TRY = 0xE,

    /* BTP */
    /* 0x11 */ BTP_KOLIN_F_CLOTH_TRY = 0x11,
};

enum FaceMotion {
    /* 0x0 */ FACE_TALK_A,
    /* 0x1 */ FACE_STONE,
    /* 0x2 */ FACE_WALK_A,
    /* 0x3 */ FACE_NORMAL_TALK,
    /* 0x4 */ FACE_HAPPY_TALK,
    /* 0x5 */ FACE_CLUP,
    /* 0x6 */ FACE_H_WAIT_D,
    /* 0x7 */ FACE_KIZUKU,
    /* 0x8 */ FACE_H_NORMAL_TALK,
    /* 0x9 */ FACE_H_KIZUKU,
    /* 0xA */ FACE_DEMO_WAIT,
    /* 0xB */ FACE_DEMO_FEAR,
    /* 0xC */ FACE_CLOTH_TRY,
    /* 0xD */ FACE_NONE,
};

enum Motion {
    /* 0x01 */ MOT_WAIT_A = 0x1,
    /* 0x02 */ MOT_WAIT_B,
    /* 0x03 */ MOT_WAIT_C,
    /* 0x04 */ MOT_TALK_A,
    /* 0x05 */ MOT_STONE,
    /* 0x06 */ MOT_NORMAL_TALK,
    /* 0x07 */ MOT_HAPPY_TALK,
    /* 0x08 */ MOT_CLUP,
    /* 0x09 */ MOT_CLOTH_WAIT,
    /* 0x0A */ MOT_STEP,
    /* 0x0B */ MOT_NOZOKU,
    /* 0x0C */ MOT_TURN,
    /* 0x0D */ MOT_WALK_A,
    /* 0x0E */ MOT_WALK_B,
    /* 0x0F */ MOT_RUN,
    /* 0x10 */ MOT_HAIHAI,
    /* 0x11 */ MOT_DEMO_WAIT,
    /* 0x12 */ MOT_DEMO_FEAR,
    /* 0x13 */ MOT_CLOTH_TRY,
};

enum RES_Name {
    /* 0x1 */ KOLIN = 0x1,
    /* 0x2 */ KOLIN_TW,
    /* 0x3 */ KOLIN1,
    /* 0x4 */ KOLIN2,
    /* 0x5 */ KOLIN_P1,
};

enum Event {
    /* 0x0 */ EVENT_NONE,
    /* 0x1 */ EVENT_NO_RESPONSE,
    /* 0x2 */ EVENT_NO_RIDE_L,
    /* 0x3 */ EVENT_NO_RIDE_R,
    /* 0x4 */ EVENT_HAIL,
    /* 0x5 */ EVENT_CONVERSATION_ABOUT_LOOPHOLE1,
    /* 0x6 */ EVENT_CONVERSATION_ABOUT_LOOPHOLE2,
    /* 0x7 */ EVENT_DEMO13_STB,
    /* 0x8 */ EVENT_CLOTH_TRY,
};

static int l_bmdData[3][2] = {
    {BMDR_KOLIN, KOLIN},
    {BMDR_KOLIN_TW, KOLIN_TW},
    {BMDR_KOLIN_CLOTH, KOLIN2},
};

static daNpcT_evtData_c l_evtList[9] = {
    {"", 0},
    {"NO_RESPONSE", 0},
    {"NO_RIDE_L", 3},
    {"NO_RIDE_R", 3},
    {"HAIL", 3},
    {"CONVERSATION_ABOUT_LOOPHOLE1", 3},
    {"CONVERSATION_ABOUT_LOOPHOLE2", 3},
    {"DEMO13_STB", 0},
    {"CLOTH_TRY", 3},
};

static char* l_resNameList[8] = {
    "",
    "Kolin",
    "Kolin_TW",
    "Kolin1",
    "Kolin2",
    "Kolin_p1",
    "Len1",
    "Taro1",
};

static s8 l_loadResPtrn0[3] = {
    KOLIN, KOLIN1, -1,
};

static s8 l_loadResPtrn1[3] = {
    KOLIN, KOLIN_TW, -1,
};

static s8 l_loadResPtrn2[3] = {
    KOLIN, KOLIN2, -1,
};

static s8 l_loadResPtrn3[5] = {
    KOLIN, KOLIN1, KOLIN2, KOLIN_TW, -1,
};

static s8 l_loadResPtrn4[3] = {
    KOLIN, KOLIN_P1, -1,
};

static s8 l_loadResPtrn9[5] = {
    KOLIN, KOLIN1, KOLIN2, KOLIN_TW, -1,
};

static s8* l_loadResPtrnList[15] = {
    l_loadResPtrn0, l_loadResPtrn0, l_loadResPtrn0, l_loadResPtrn0,
    l_loadResPtrn1, l_loadResPtrn0, l_loadResPtrn0, l_loadResPtrn0,
    l_loadResPtrn0, l_loadResPtrn0, l_loadResPtrn3, l_loadResPtrn0,
    l_loadResPtrn0, l_loadResPtrn4, l_loadResPtrn9,
};

static daNpcT_faceMotionAnmData_c l_faceMotionAnmData[14] = {
    {-1, J3DFrameCtrl::EMode_NONE, 0, BTP_KOLIN, J3DFrameCtrl::EMode_LOOP, KOLIN, 1},
    {BCK_KOLIN_F_WALK_A, J3DFrameCtrl::EMode_LOOP, KOLIN1, BTP_KOLIN_F_WALK_A, J3DFrameCtrl::EMode_LOOP, KOLIN1, 0},
    {BCK_KOLIN_F_STONE, J3DFrameCtrl::EMode_NONE, KOLIN1, BTP_KOLIN_F_STONE, J3DFrameCtrl::EMode_NONE, KOLIN1, 0},
    {BCK_KOLIN_FH_NORMAL_TALK, J3DFrameCtrl::EMode_LOOP, KOLIN1, BTP_KOLIN, J3DFrameCtrl::EMode_LOOP, KOLIN, 1},
    {BCK_KOLIN_F_KIZUKU, J3DFrameCtrl::EMode_NONE, KOLIN1, BTP_KOLIN_F_KIZUKU, J3DFrameCtrl::EMode_NONE, KOLIN1, 0},
    {BCK_KOLIN_FH_KIZUKU, J3DFrameCtrl::EMode_LOOP, KOLIN1, BTP_KOLIN, J3DFrameCtrl::EMode_LOOP, KOLIN, 1},
    {BCK_KOLIN_FH_WAIT_D, J3DFrameCtrl::EMode_LOOP, KOLIN1, BTP_KOLIN_FH_WAIT_D, J3DFrameCtrl::EMode_LOOP, KOLIN1, 0},
    {BCK_KOLIN_F_DEMO_WAIT, J3DFrameCtrl::EMode_LOOP, KOLIN_TW, BTP_KOLIN_F_DEMO_WAIT, J3DFrameCtrl::EMode_LOOP, KOLIN_TW, 0},
    {BCK_KOLIN_F_DEMO_FEAR, J3DFrameCtrl::EMode_LOOP, KOLIN_TW, BTP_KOLIN_F_DEMO_FEAR, J3DFrameCtrl::EMode_LOOP, KOLIN_TW, 0},
    {BCK_KOLIN_F_CLOTH_TRY, J3DFrameCtrl::EMode_NONE, KOLIN2, BTP_KOLIN_F_CLOTH_TRY, J3DFrameCtrl::EMode_NONE, KOLIN2, 0},
    {BCK_KOLIN_F_TALK_A, J3DFrameCtrl::EMode_NONE, KOLIN, BTP_KOLIN_F_TALK_A, J3DFrameCtrl::EMode_NONE, KOLIN, 0},
    {BCK_KOLIN_F_NORMAL_TALK, J3DFrameCtrl::EMode_NONE, KOLIN1, BTP_KOLIN, J3DFrameCtrl::EMode_LOOP, KOLIN, 1},
    {BCK_KOLIN_F_HAPPY_TALK, J3DFrameCtrl::EMode_NONE, KOLIN1, BTP_KOLIN, J3DFrameCtrl::EMode_LOOP, KOLIN, 1},
    {BCK_KOLIN_F_CLUP, J3DFrameCtrl::EMode_NONE, KOLIN1, BTP_KOLIN_F_CLUP, J3DFrameCtrl::EMode_NONE, KOLIN1, 0},
};

static daNpcT_motionAnmData_c l_motionAnmData[21] = {
    {BCK_KOLIN_WAIT_A, J3DFrameCtrl::EMode_LOOP, KOLIN, BTK_KOLIN, J3DFrameCtrl::EMode_NONE, KOLIN, 1, 0},
    {BCK_KOLIN_WAIT_B, J3DFrameCtrl::EMode_LOOP, KOLIN1, BTK_KOLIN, J3DFrameCtrl::EMode_NONE, KOLIN, 1, 0},
    {BCK_KOLIN_WAIT_C, J3DFrameCtrl::EMode_LOOP, KOLIN1, BTK_KOLIN, J3DFrameCtrl::EMode_NONE, KOLIN, 1, 0},
    {BCK_KOLIN_WAIT_D, J3DFrameCtrl::EMode_LOOP, KOLIN1, BTK_KOLIN, J3DFrameCtrl::EMode_NONE, KOLIN, 1, 0},
    {BCK_KOLIN_HAPPY_WAIT, J3DFrameCtrl::EMode_LOOP, KOLIN1, BTK_KOLIN, J3DFrameCtrl::EMode_NONE, KOLIN, 1, 0},
    {BCK_KOLIN_TALK_A, J3DFrameCtrl::EMode_NONE, KOLIN1, BTK_KOLIN, J3DFrameCtrl::EMode_NONE, KOLIN, 1, 0},
    {BCK_KOLIN_NORMAL_TALK, J3DFrameCtrl::EMode_NONE, KOLIN1, BTK_KOLIN, J3DFrameCtrl::EMode_NONE, KOLIN, 1, 0},
    {BCK_KOLIN_HAPPY_TALK, J3DFrameCtrl::EMode_NONE, KOLIN1, BTK_KOLIN, J3DFrameCtrl::EMode_NONE, KOLIN, 1, 0},
    {BCK_KOLIN_WALK_A, J3DFrameCtrl::EMode_LOOP, KOLIN1, BTK_KOLIN_WALK_A, J3DFrameCtrl::EMode_LOOP, KOLIN1, 0, 0},
    {BCK_KOLIN_WALK_B, J3DFrameCtrl::EMode_LOOP, KOLIN1, BTK_KOLIN, J3DFrameCtrl::EMode_NONE, KOLIN, 1, 0},
    {BCK_KOLIN_RUN, J3DFrameCtrl::EMode_LOOP, KOLIN1, BTK_KOLIN, J3DFrameCtrl::EMode_NONE, KOLIN, 1, 0},
    {BCK_KOLIN_HAIHAI, J3DFrameCtrl::EMode_LOOP, KOLIN1, BTK_KOLIN, J3DFrameCtrl::EMode_NONE, KOLIN, 1, 0},
    {BCK_KOLIN_TURN, J3DFrameCtrl::EMode_NONE, KOLIN1, BTK_KOLIN, J3DFrameCtrl::EMode_NONE, KOLIN, 1, 0},
    {BCK_KOLIN_STONE, J3DFrameCtrl::EMode_NONE, KOLIN1, BTK_KOLIN_STONE, J3DFrameCtrl::EMode_NONE, KOLIN1, 0, 0},
    {BCK_KOLIN_CLUP, J3DFrameCtrl::EMode_NONE, KOLIN1, BTK_KOLIN_CLUP, J3DFrameCtrl::EMode_NONE, KOLIN1, 0, 0},
    {BCK_KOLIN_NOZOKU, J3DFrameCtrl::EMode_NONE, KOLIN1, BTK_KOLIN_NOZOKU, J3DFrameCtrl::EMode_NONE, KOLIN1, 0, 0},
    {BCK_KOLIN_STEP, J3DFrameCtrl::EMode_NONE, KOLIN, BTK_KOLIN, J3DFrameCtrl::EMode_NONE, KOLIN, 1, 0},
    {BCK_KOLIN_DEMO_WAIT, J3DFrameCtrl::EMode_LOOP, KOLIN_TW, BTK_KOLIN_DEMO_WAIT, J3DFrameCtrl::EMode_LOOP, KOLIN_TW, 0, 0},
    {BCK_KOLIN_DEMO_FEAR, J3DFrameCtrl::EMode_LOOP, KOLIN_TW, BTK_KOLIN_DEMO_FEAR, J3DFrameCtrl::EMode_LOOP, KOLIN_TW, 0, 0},
    {BCK_KOLIN_CLOTH_WAIT, J3DFrameCtrl::EMode_LOOP, KOLIN2, BTK_KOLIN, J3DFrameCtrl::EMode_LOOP, KOLIN, 1, 0},
    {BCK_KOLIN_CLOTH_TRY, J3DFrameCtrl::EMode_NONE, KOLIN2, BTK_KOLIN_CLOTH_TRY, J3DFrameCtrl::EMode_NONE, KOLIN2, 0, 0},
};

static daNpcT_MotionSeqMngr_c::sequenceStepData_c l_faceMotionSequenceData[56] = {
    {10, -1, 1}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {2, -1, 1}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {1, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {11, -1, 1}, {3, -1, 0}, {-1, 0, 0}, {-1, 0, 0},
    {12, -1, 1}, {3, -1, 0}, {-1, 0, 0}, {-1, 0, 0},
    {13, -1, 1}, {5, -1, 0}, {-1, 0, 0}, {-1, 0, 0},
    {6, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {4, -1, 1}, {5, -1, 0}, {-1, 0, 0}, {-1, 0, 0},
    {3, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {5, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {7, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {8, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {9, -1, 1}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {0, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
};

static daNpcT_MotionSeqMngr_c::sequenceStepData_c l_motionSequenceData[80] = {
    {0, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {0, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {1, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {2, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {5, -1, 1}, {1, -1, 0}, {-1, 0, 0}, {-1, 0, 0},
    {13, -1, 1}, {1, -1, 0}, {-1, 0, 0}, {-1, 0, 0},
    {6, -1, 1}, {0, -1, 0}, {-1, 0, 0}, {-1, 0, 0},
    {7, -1, 1}, {4, -1, 0}, {-1, 0, 0}, {-1, 0, 0},
    {14, -1, 1}, {3, -1, 0}, {-1, 0, 0}, {-1, 0, 0},
    {19, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {16, 3, 1}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {15, -1, 1}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {12, -1, 1}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {8, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {9, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {10, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {11, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {17, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {18, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {20, -1, 1}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
};

char* daNpc_Kolin_c::mCutNameList[11] = {
    "",
    "NO_RIDE",
    "HAIL",
    "GIVEME_WOODSWD",
    "GET_WOODSWD",
    "CONVERSATION_ABOUT_LOOPHOLE",
    "CACARICO_CONVERSATION",
    "CONVERSATION_ABOUT_DEATHMT",
    "CONVERSATION_ABOUT_GORON",
    "CLOTH_TRY",
    "THANK_YOU",
};

daNpc_Kolin_c::cutFunc daNpc_Kolin_c::mCutList[11] = {
    NULL,
    &daNpc_Kolin_c::cutNoRide,
    &daNpc_Kolin_c::cutHail,
    &daNpc_Kolin_c::cutGiveMeWoodSwd,
    &daNpc_Kolin_c::cutGetWoodSwd,
    &daNpc_Kolin_c::cutConversationAboutLoopHole,
    &daNpc_Kolin_c::cutCacaricoConversation,
    &daNpc_Kolin_c::cutConversationAboutDeathMt,
    &daNpc_Kolin_c::cutConversationAboutGoron,
    &daNpc_Kolin_c::cutClothTry,
    &daNpc_Kolin_c::cutThankYou
};

daNpc_Kolin_c::~daNpc_Kolin_c() {
    if (mpMorf[0] != NULL) {
        mpMorf[0]->stopZelAnime();
    }

    deleteRes(l_loadResPtrnList[mType], (const char**)l_resNameList);
}

daNpc_Kolin_HIOParam const daNpc_Kolin_Param_c::m = {
    140.0f,
    -3.0f,
    1.0f,
    500.0f,
    255.0f,
    130.0f,
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
    false,
    false,
    4.0f,
    -15.0f,
    0.0f,
    -15.0f,
    15.0f,
    40.0f,
    15.0f,
    60.0f,
    250.0f,
    200.0f,
    8.0f,
    0x168,
    0xB4,
    2.0f,
};

cPhs__Step daNpc_Kolin_c::create() {
    daNpcT_ct(this, daNpc_Kolin_c, l_faceMotionAnmData, l_motionAnmData, l_faceMotionSequenceData, 4, l_motionSequenceData, 4, l_evtList, l_resNameList);

    mType = getType();
    mFlowNodeNo = getFlowNodeNo();
    mTwilight = dKy_darkworld_check();

    if (mType == 9) {
        mTwilight = false;
    }

    cPhs__Step phase = (cPhs__Step)loadRes(l_loadResPtrnList[mType], (const char**)l_resNameList);
    if (phase == cPhs_COMPLEATE_e) {
        OS_REPORT("\t(%s:%d) flowNo:%d, PathID:%02x, BitSW:%02x<%08x> ", fopAcM_getProcNameString(this), mType, mFlowNodeNo,
                  (getPathID() >> 32) & 0xFF, getBitSW() & 0xFF, fopAcM_GetParam(this));
        
        if (isDelete()) {
            OS_REPORT("===>isDelete:TRUE\n");
            return cPhs_ERROR_e;
        }

        OS_REPORT("\n");

        static u32 const heapSize[15] = {
            0x39C0, 0, 0, 0x39C0, 0x3900, 0x39D0, 0x39D0, 0x39C0,
            0, 0x39C0, 0x41D0, 0x39C0, 0x39C0, 0x39C0, 0,
        };
        if (!fopAcM_entrySolidHeap(this, createHeapCallBack, heapSize[mType])) {
            return cPhs_ERROR_e;
        }

        J3DModel* model = mpMorf[0]->getModel();
        fopAcM_SetMtx(this, mpMorf[0]->getModel()->getBaseTRMtx());
        fopAcM_setCullSizeBox(this, -200.0f, -100.0f, -200.0f, 200.0f, 300.0f, 200.0f);

        mSound.init(&current.pos, &eyePos, 3, 1);
        field_0x9c0.init(&mAcch, 0.0f, 0.0f);

        reset();

        mAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1, &mAcchCir, fopAcM_GetSpeed_p(this),
                  fopAcM_GetAngle_p(this), fopAcM_GetShapeAngle_p(this));
        mCcStts.Init(daNpc_Kolin_Param_c::m.common.weight, 0, this);

        field_0xe48.Set(mCcDCyl);
        field_0xe48.SetStts(&mCcStts);
        field_0xe48.SetTgHitCallback(tgHitCallBack);

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

    return phase;
}

int daNpc_Kolin_c::CreateHeap() {
    int bmdIdx;

    if (mTwilight == true) {
        bmdIdx = 1;
    } else {
        bmdIdx = 0;
    }

    J3DModelData* modelData = static_cast<J3DModelData*>(dComIfG_getObjectRes(l_resNameList[l_bmdData[bmdIdx][1]], l_bmdData[bmdIdx][0]));
    if (modelData == NULL) {
        return 0;
    }

    mpMorf[0] = new mDoExt_McaMorfSO(modelData, NULL, NULL, NULL, -1, 1.0f, 0, -1, &mSound, 0x80000, 0x11020284);
    if (mpMorf[0] == NULL || mpMorf[0]->getModel() == NULL) {
        return 0;
    }

    J3DModel* model = mpMorf[0]->getModel();
    for (u16 i = 0; i < modelData->getJointNum(); i++) {
        modelData->getJointNodePointer(i)->setCallBack(ctrlJointCallBack);
    }
    model->setUserArea((uintptr_t)this);

    mpMatAnm[0] = new daNpcT_MatAnm_c();
    if (mpMatAnm[0] == NULL) {
        return 0;
    }

    if (mType == 10) {
        if (l_bmdData[2][0] >= 0) {
            modelData = (J3DModelData*)dComIfG_getObjectRes(l_resNameList[l_bmdData[2][1]], l_bmdData[2][0]);
        } else {
            modelData = NULL;
        }

        if (modelData != NULL) {
            mpClothModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
        }

        if (mpClothModel == NULL) {
            return 0;
        }
    }

    if (mTwilight) {
        if (setFaceMotionAnm(7, false) && setMotionAnm(0x11, 0.0f, FALSE)) {
            return 1;
        }

        return 0;
    }

    if (setFaceMotionAnm(10, false) && setMotionAnm(0, 0.0f, FALSE)) {
        return 1;
    }

    return 0;
}

int daNpc_Kolin_c::Delete() {
    this->~daNpc_Kolin_c();
    return 1;
}

int daNpc_Kolin_c::Execute() {
    return execute();
}

int daNpc_Kolin_c::Draw() {
    if (mpMatAnm[0] != NULL) {
        J3DModelData* modelData = mpMorf[0]->getModel()->getModelData();
        modelData->getMaterialNodePointer(getEyeballMaterialNo())->setMaterialAnm(mpMatAnm[0]);
    }

    return draw(FALSE, FALSE, mRealShadowSize, NULL, 100.0f, FALSE, FALSE, FALSE);
}

int daNpc_Kolin_c::createHeapCallBack(fopAc_ac_c* a_this) {
    daNpc_Kolin_c* i_this = (daNpc_Kolin_c*)a_this;
    return i_this->CreateHeap();
}

int daNpc_Kolin_c::ctrlJointCallBack(J3DJoint* i_joint, int param_2) {
    if (param_2 == 0) {
        daNpc_Kolin_c* i_this = (daNpc_Kolin_c*)j3dSys.getModel()->getUserArea();
        if (i_this != NULL) {
            i_this->ctrlJoint(i_joint, j3dSys.getModel());
        }
    }

    return 1;
}

u8 daNpc_Kolin_c::getType() {
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
            return 10;

        case 11:
            return 11;

        case 12:
            return 12;

        case 13:
            return 13;

        default:
            return 14;
    }
}

BOOL daNpc_Kolin_c::isDelete() {
    switch (mType) {
        case 0:
                   /* dSv_event_flag_c::F_0026 - Ordon Village - gave wooden sword to talo on 3rd day */
            return daNpcT_chkEvtBit(31);

        case 1:
                    /* dSv_event_flag_c::F_0044 - Ordon Village - Accepted sword tutorial first time */
            return !daNpcT_chkEvtBit(142) ||
                    /* dSv_event_flag_c::F_0009 - Ordon Woods - Approach secret entrance with Colin */
                    daNpcT_chkEvtBit(13);

        case 2:
            return FALSE;

        case 3:
                   /* dSv_event_flag_c::F_0070 - Ordon Woods - Colin went deep into the woods */
            return daNpcT_chkEvtBit(168);

        case 4:
            return FALSE;

        case 5:
                    /* dSv_event_flag_c::M_028 - Cutscene - [cutscene: 14] restore mountain spirit - Reuinion with Colin et al. */
            return !daNpcT_chkEvtBit(61) ||
                    /* dSv_event_flag_c::F_0066 - Death Mountain - First saw Goron cutscene on mountain path */
                    daNpcT_chkEvtBit(164);

        case 6:
                    /* dSv_event_flag_c::F_0066 - Death Mountain - First saw Goron cutscene on mountain path */
            return !daNpcT_chkEvtBit(164) ||
                    /* dSv_event_flag_c::M_020 - Cutscene - [cutscene: ] Colin kidnapped : ON once watched */
                    daNpcT_chkEvtBit(53);

        case 7:
                    /* dSv_event_flag_c::F_0264 - Cutscene - Get master sword */
            return !daNpcT_chkEvtBit(264);

        case 8:
            return TRUE;

        case 9:
            return FALSE;

        case 10:
                    /* dSv_event_flag_c::M_035 - Cutscene - [cutscene: 35] after carriage guarding event */
            return !daNpcT_chkEvtBit(68) ||
                    /* dSv_event_flag_c::F_0264 - Cutscene - Get master sword */
                    daNpcT_chkEvtBit(264);
            break;

        case 11:
                   /* dSv_event_flag_c::F_0014 - Ordon Village - sword tutorial ends */
            return daNpcT_chkEvtBit(18);

        case 12:
                    /* dSv_event_flag_c::F_0014 - Ordon Village - sword tutorial ends */
            return !daNpcT_chkEvtBit(18);

        case 13:
            return FALSE;

        default:
            return FALSE;
    }
}

void daNpc_Kolin_c::reset() {
    csXyz angle;
    int iVar1 = (u8*)&field_0x101c - (u8*)&mNextAction;

    if (mpMatAnm[0] != NULL) {
        mpMatAnm[0]->initialize();
    }

    initialize();

    for (int i = 0; i < 5; i++) {
        mActorMngr[i].initialize();
    }

    if (getPathID() != 0xFF) {
        mPath.initialize();
        mPath.setPathInfo(getPathID(), fopAcM_GetRoomNo(this), 0);
    }

    memset(&mNextAction, 0, iVar1);

    angle.setall(0);
    angle.y = home.angle.y;

    switch (mType) {
        case 10:
            field_0x1017 = 1;
            break;
    
        case 9:
        case 11:
        default:
            break;
    }

    setAngle(angle);
        /* dSv_event_flag_c::F_0026 - Ordon Village - gave wooden sword to talo on 3rd day */
    if (daNpcT_chkEvtBit(31) ||
          /* dSv_event_flag_c::F_0010 - Ordon Village - First convo with Colin blocking path (forced) */
        (!daNpcT_chkEvtBit(14) &&
          /* dSv_event_flag_c::F_0016 - Ordon Village - On 3rd day, start following Colin who is blocking path */
         !daNpcT_chkEvtBit(20))) {
        /* dSv_event_tmp_flag_c::T_0021 - Ordon Village - Colin follows to Link's house */
        daNpcT_offTmpBit(22);        
    }

    field_0x1010 = cM_rnd();
}

void daNpc_Kolin_c::afterJntAnm(int param_1) {
    if (param_1 == 1) {
        mDoMtx_stack_c::YrotM(-mStagger.getAngleZ(1));
        mDoMtx_stack_c::ZrotM(mStagger.getAngleX(1));
    } else if (param_1 == 4) {
        mDoMtx_stack_c::YrotM(-mStagger.getAngleZ(0));
        mDoMtx_stack_c::ZrotM(mStagger.getAngleX(0));
    }
}

void daNpc_Kolin_c::setParam() {
    selectAction();
    srchActors();

    s16 talk_distance = daNpc_Kolin_Param_c::m.common.talk_distance;
    s16 talk_angle = daNpc_Kolin_Param_c::m.common.talk_angle;
    s16 attention_distance = daNpc_Kolin_Param_c::m.common.attention_distance;
    s16 attention_angle = daNpc_Kolin_Param_c::m.common.attention_angle;

    if (mType == 3) {
        talk_distance = 7;
        talk_angle = 6;
        attention_distance = 9;
        attention_angle = 6;
    } else {
        if (&daNpc_Kolin_c::follow == mNextAction) {
            talk_distance = 1;
            talk_angle = 6;
            attention_distance = 3;
            attention_angle = 6;
        }
    }

    if (daPy_getPlayerActorClass()->checkHorseRide()) {
        if (talk_distance < 7) {
            talk_distance = 7;
        }

        if (attention_distance < 9) {
            attention_distance = 9;
        }
    }

    attention_info.distances[fopAc_attn_LOCK_e] = daNpcT_getDistTableIdx(attention_distance, attention_angle);
    attention_info.distances[fopAc_attn_TALK_e] = attention_info.distances[fopAc_attn_LOCK_e];
    attention_info.distances[fopAc_attn_SPEAK_e] = daNpcT_getDistTableIdx(talk_distance, talk_angle);
    attention_info.flags = fopAc_AttnFlag_SPEAK_e | fopAc_AttnFlag_TALK_e;

    if (mType == 4) {
        fopAcM_OffStatus(this, fopAcM_STATUS_UNK_0x100);
    }

    scale.set(daNpc_Kolin_Param_c::m.common.scale, daNpc_Kolin_Param_c::m.common.scale, daNpc_Kolin_Param_c::m.common.scale);
    mCcStts.SetWeight(daNpc_Kolin_Param_c::m.common.weight);

    if (&daNpc_Kolin_c::follow == mNextAction) {
        mCcStts.SetWeight(109);
    }

    mCylH = daNpc_Kolin_Param_c::m.common.height;
    mWallR = daNpc_Kolin_Param_c::m.common.width;
    mAttnFovY = daNpc_Kolin_Param_c::m.common.fov;
    mAcchCir.SetWallR(mWallR);
    mAcchCir.SetWallH(daNpc_Kolin_Param_c::m.common.knee_length);
    mRealShadowSize = daNpc_Kolin_Param_c::m.common.real_shadow_size;
    mExpressionMorfFrame = daNpc_Kolin_Param_c::m.common.expression_morf_frame;
    mMorfFrames = daNpc_Kolin_Param_c::m.common.morf_frame;
    gravity = daNpc_Kolin_Param_c::m.common.gravity;
}

BOOL daNpc_Kolin_c::checkChangeEvt() {
    if (!chkAction(&daNpc_Kolin_c::talk)) {
        mPreItemNo = 0;

        if (dComIfGp_event_chkTalkXY()) {
            if (dComIfGp_evmng_ChkPresentEnd()) {
                mEvtNo = 1;
                evtChange();
            }

            return TRUE;
        }

        switch (mType) {
            case 2:
                break;
                
            case 3:
                    /* dSv_event_flag_c::F_0019 - Ordon Woods - Spoke with Ilia (Colin is there too) at the spring */
                if (daNpcT_chkEvtBit(23)) {
                    mEvtNo = 6;
                } else {
                    mEvtNo = 5;
                }

                evtChange();
                return TRUE;

            case 10:
                mEvtNo = 8;
                evtChange();
                return TRUE;

            case 11:
                break;
        }
    }

    return FALSE;
}

void daNpc_Kolin_c::setAfterTalkMotion() {
    int i_index = FACE_NONE;
    
    switch (mFaceMotionSeqMngr.getNo()) {
        case FACE_NORMAL_TALK:
            i_index = FACE_H_NORMAL_TALK;
            break;

        case FACE_HAPPY_TALK:
            i_index = FACE_H_NORMAL_TALK;
            break;

        case FACE_H_KIZUKU:
            return;
    }

    mFaceMotionSeqMngr.setNo(i_index, -1.0f, FALSE, 0);
}

void daNpc_Kolin_c::srchActors() {
    switch (mType) {
        case 0:
            if (mActorMngr[0].getActorP() == NULL) {
                mActorMngr[0].entry(getEvtAreaTagP(7, 0));
            }
            break;

        case 1:
            if (mActorMngr[0].getActorP() == NULL) {
                mActorMngr[0].entry(getEvtAreaTagP(8, 0));
            }

            if (mActorMngr[1].getActorP() == NULL) {
                mActorMngr[1].entry(getEvtAreaTagP(9, 0));
            }

            if (mActorMngr[2].getActorP() == NULL) {
                mActorMngr[2].entry(getNearestActorP(PROC_Obj_Hfuta));
            }
            break;

        case 3:
            if (mActorMngr[3].getActorP() == NULL) {
                mActorMngr[3].entry(getNearestActorP(PROC_NPC_YELIA));
            }
            break;

        case 4:
            if (mActorMngr[4].getActorP() == NULL) {
                mActorMngr[4].entry(getNearestActorP(PROC_NPC_LEN));
            }
            break;

        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
            break;
    }
}

BOOL daNpc_Kolin_c::evtTalk() {
    if (chkAction(&daNpc_Kolin_c::talk)) {
        (this->*mAction)(NULL);
    } else {
        setAction(&daNpc_Kolin_c::talk);
    }

    return TRUE;
}

BOOL daNpc_Kolin_c::evtCutProc() {
    int staffId = dComIfGp_getEventManager().getMyStaffId("Kolin", this, -1);
    if (staffId != -1) {
        mStaffId = staffId;
        int actIdx = dComIfGp_getEventManager().getMyActIdx(mStaffId, mCutNameList, 11, 0, 0);
        if ((this->*mCutList[actIdx])(mStaffId)) {
            dComIfGp_getEventManager().cutEnd(mStaffId);
        }

        return TRUE;
    }

    return FALSE;
}

void daNpc_Kolin_c::action() {
    fopAc_ac_c* actor_p = hitChk(&field_0xe48, -1);
    if (actor_p) {
        if (mType != 10 && field_0xe48.GetTgHitObj()->ChkAtType(AT_TYPE_THROW_OBJ)) {
            mStagger.setParam(this, actor_p, mCurAngle.y);
            setDamage(0, 13, 1);
            mDamageTimerStart = 0;
            mJntAnm.lookNone(1);
        }
    }

    if (mStagger.checkRebirth()) {
        mStagger.initialize();
        mMode = 1;
    }

    if (mNextAction) {
        if (mAction == mNextAction) {
            (this->*mAction)(NULL);
        } else {
            setAction(mNextAction);
        }
    }
}

void daNpc_Kolin_c::beforeMove() {
    fopAcM_OffStatus(this, fopAcM_STATUS_UNK_0x8000000);

    if (checkHide()) {
        fopAcM_OnStatus(this, fopAcM_STATUS_UNK_0x8000000);
    }

    if (checkHide() || mNoDraw) {
        attention_info.flags = 0;
    }
}

static daNpc_Kolin_Param_c l_HIO;

void daNpc_Kolin_c::setAttnPos() {
    cXyz sp3c(5.0f, 30.0f, 0.0f);

    if (field_0x1015 != 0) {
        daPy_getPlayerActorClass()->onWolfEyeKeep();
    }

    mStagger.calc(FALSE);
    f32 rad_val = cM_s2rad(mCurAngle.y - field_0xd7e.y);
    mJntAnm.setParam(this, mpMorf[0]->getModel(), &sp3c, getBackboneJointNo(), getNeckJointNo(), getHeadJointNo(),
                     daNpc_Kolin_Param_c::m.common.body_angleX_min, daNpc_Kolin_Param_c::m.common.body_angleX_max,
                     daNpc_Kolin_Param_c::m.common.body_angleY_min, daNpc_Kolin_Param_c::m.common.body_angleY_max,
                     daNpc_Kolin_Param_c::m.common.head_angleX_min, daNpc_Kolin_Param_c::m.common.head_angleX_max,
                     daNpc_Kolin_Param_c::m.common.head_angleY_min, daNpc_Kolin_Param_c::m.common.head_angleY_max,
                     daNpc_Kolin_Param_c::m.common.neck_rotation_ratio, rad_val, NULL);
    mJntAnm.calcJntRad(0.2f, 1.0f, rad_val);
    
    setMtx();
    mDoMtx_stack_c::copy(mpMorf[0]->getModel()->getAnmMtx(getHeadJointNo()));
    mDoMtx_stack_c::multVec(&sp3c, &eyePos);
    mJntAnm.setEyeAngleX(eyePos, 1.0f, 0);
    mJntAnm.setEyeAngleY(eyePos, mCurAngle.y, FALSE, 1.0f, 0);

    sp3c.set(0.0f, 0.0f, 0.0f);
    sp3c.y = daNpc_Kolin_Param_c::m.common.attention_offset;
    if (mType == 10) {
        sp3c.set(26.54f, 0.0f, -97.77f);
        sp3c.y = daNpc_Kolin_Param_c::m.common.attention_offset;
    }

    mDoMtx_stack_c::YrotS(mCurAngle.y);
    mDoMtx_stack_c::multVec(&sp3c, &sp3c);
    attention_info.position = current.pos + sp3c;

    static cXyz prtclScl(1.0f, 1.0f, 1.0f);
    setFootPos();
    if (3.0f < speedF) {
        setFootPrtcl(&prtclScl, 9.0f, 0.0f);
    }
}

void daNpc_Kolin_c::setCollision() {
    cXyz pos;

    if (!mHide) {
        u32 tgType = 0xD8FBFDFF;
        u32 tgSPrm = 0x1F;

        if (mTwilight) {
            tgType = 0;
            tgSPrm = 0;
        } else if (mStagger.checkStagger()) {
            tgType = 0;
            tgSPrm = 0;
        }

        field_0xe48.SetCoSPrm(0x79);
        field_0xe48.SetTgType(tgType);
        field_0xe48.SetTgSPrm(tgSPrm);
        field_0xe48.OnTgNoHitMark();

        pos.set(0.0f, 0.0f, 0.0f);
        f32 cyl_h = mCylH;
        f32 wall_r = mWallR;

        if (mType == 10) {
            pos.set(26.54f, 0.0f, -97.77f);
        } else if (mType == 4) {
            pos.set(-63.45f, 0.0f, 157.33f);
        }

        mDoMtx_stack_c::YrotS(mCurAngle.y);
        mDoMtx_stack_c::multVec(&pos, &pos);
        pos += current.pos;

        field_0xe48.SetH(cyl_h);
        field_0xe48.SetR(wall_r);
        field_0xe48.SetC(pos);
        dComIfG_Ccsp()->Set(&field_0xe48);
    }

    field_0xe48.ClrCoHit();
    field_0xe48.ClrTgHit();
}

int daNpc_Kolin_c::drawDbgInfo() {
    return false;
}

void daNpc_Kolin_c::drawOtherMdl() {
    J3DModel* model = mpMorf[0]->getModel();

    for (int i = 0; i < 1; i++) {
        if (mpClothModel != NULL && i == 0 && field_0x1017 != 0) {
            g_env_light.setLightTevColorType_MAJI(mpClothModel, &tevStr);
            static int const jointNo[1] = {0x12};
            mDoMtx_stack_c::copy(model->getAnmMtx(jointNo[0]));
            Mtx mtx;
            MTXCopy(mDoMtx_stack_c::get(), mtx);
            mpClothModel->setBaseTRMtx(mtx);
            mDoExt_modelUpdateDL(mpClothModel);
            dComIfGd_addRealShadow(mShadowKey, mpClothModel);
        }
    }
}

void daNpc_Kolin_c::changeAnm(int* param_1, int* param_2) {
    if (mType == 13 && *param_1 == 19) {
        *param_1 = 5;
        *param_2 = 5;
    }
}

void daNpc_Kolin_c::changeBck(int* param_1, int* param_2) {
    if (mType == 13) {
        switch (*param_1) {
            case 8:
                *param_1 = 3;
                *param_2 = 5;
                break;

            case 14:
                *param_1 = 4;
                *param_2 = 5;
                break;
        }
    }
}

int daNpc_Kolin_c::selectAction() {
    mNextAction = NULL;

#if DEBUG
    if (daNpc_Kolin_Param_c::m.common.debug_mode_ON) {
        mNextAction = &daNpc_Kolin_c::test;
        return 1;
    }
#endif

    switch (mType) {
        case 0:
            mNextAction = &daNpc_Kolin_c::timidWalk;
                /* dSv_event_tmp_flag_c::T_0021 - Ordon Village - Colin follows to Link's house */
            if (daNpcT_chkTmpBit(22)) {
                mNextAction = &daNpc_Kolin_c::follow;
            }
            break;
            
        case 10:
            mNextAction = &daNpc_Kolin_c::clothWait;
            break;

        default:
            mNextAction = &daNpc_Kolin_c::wait;
            break;
    }

    return 1;
}

BOOL daNpc_Kolin_c::chkAction(actionFunc action) {
    return mAction == action;
}

int daNpc_Kolin_c::setAction(actionFunc action) {
    mMode = 3;

    if (mAction) {
        (this->*mAction)(NULL);
    }

    mMode = 0;

    mAction = action;
    if (mAction) {
        (this->*mAction)(NULL);
    }

    return 1;
}

void daNpc_Kolin_c::calcFollowSpeedAndAngle(fopAc_ac_c* actor, int param_2, int param_3) {
    cXyz sp4c, sp58;
    f32 actor_distance = fopAcM_searchActorDistanceXZ(this, actor);

    if (param_2 == 0) {
        cLib_addCalcAngleS2(&current.angle.y, fopAcM_searchActorAngleY(this, actor), 6, 0x800);
        shape_angle.y = current.angle.y;
        mCurAngle.y = current.angle.y;

        if (param_3 != 0) {
            f32 fVar1;
            if (daNpc_Kolin_Param_c::m.follow_distance <= actor_distance) {
                fVar1 = actor_distance - daNpc_Kolin_Param_c::m.follow_distance;
            } else {
                fVar1 = 0.0f;
            }

            sp4c.set(0.0f, 0.0f, fVar1 * 0.1f);
            mDoMtx_stack_c::YrotS(fopAcM_searchActorAngleY(this, actor));
            mDoMtx_stack_c::multVec(&sp4c, &sp58);
            current.pos += sp58;
            old.pos = current.pos;
        }

        if (mMotionSeqMngr.getNo() == MOT_RUN) {
            cLib_chaseF(&speedF, daNpc_Kolin_Param_c::m.run_speed, 0.5f);
        } else if (mFootLOffset.y < mFootROffset.y) {
            speedF = (mFootLOffset - mOldFootLOffset).absXZ();
        } else {
            speedF = (mFootROffset - mOldFootROffset).absXZ();
        }
    } else if (mMotionSeqMngr.getNo() == MOT_RUN) {
        cLib_chaseF(&speedF, 0.0f, 1.0f);
    } else {
        speedF = 0.0f;
    }
}

void daNpc_Kolin_c::followPlayer(int param_1) {
    cXyz attnPos = getAttnPos(daPy_getPlayerActorClass());
    BOOL bVar1 = mPlayerActorMngr.getActorP() != NULL;

    int dist_index = bVar1 == FALSE ? attention_info.distances[fopAc_attn_SPEAK_e] : attention_info.distances[fopAc_attn_TALK_e];
    int iVar1 = 0;
    f32 dist_max = bVar1 == FALSE ? dComIfGp_getAttention()->getDistTable(dist_index).mDistMax : dComIfGp_getAttention()->getDistTable(dist_index).mDistMax;
    f32 lower_y = dComIfGp_getAttention()->getDistTable(dist_index).mLowerY * -1.0f;
    f32 upper_y = dComIfGp_getAttention()->getDistTable(dist_index).mUpperY * -1.0f;

    if (chkPointInArea(attnPos, attention_info.position, dist_max, lower_y, upper_y, 0)) {
        if (!bVar1) {
            mPlayerActorMngr.entry(daPy_getPlayerActorClass());
            bVar1 = TRUE;
        }
    } else if (bVar1) {
        mPlayerActorMngr.remove();
        bVar1 = FALSE;
    }

    if (bVar1 != FALSE) {
        iVar1 = 1;
    }

    if (param_1 == 0 && iVar1 == 0) {
        cXyz start, end;

        start = current.pos;
        start.y += mAcchCir.GetWallH();
        end.setall(0.0f);
        end.z = 50.0f;

        mDoMtx_stack_c::YrotS(fopAcM_searchPlayerAngleY(this));
        mDoMtx_stack_c::multVec(&end, &end);
        end += start;
        mLinChk.Set(&start, &end, this);

        if (dComIfG_Bgsp().LineCross(&mLinChk)) {
            iVar1 = 1;
        }
    }

    calcFollowSpeedAndAngle(daPy_getPlayerActorClass(), iVar1, param_1);

    cXyz sp98 = attnPos - attention_info.position;

    if (300.0f < fabsf(sp98.y)) {
        if (mMotionSeqMngr.getNo() == MOT_RUN) {
            cLib_chaseF(&speedF, 0.0f, 1.0f);
        } else {
            speedF = 0.0f;
        }
    }

    if (cM3d_IsZero(speedF)) {
        if (mMotionSeqMngr.getNo() != MOT_WAIT_A) {
            mMotionSeqMngr.setNo(MOT_WAIT_A, -1.0f, FALSE, 0);
        }
    } else if (mMotionSeqMngr.getNo() != MOT_RUN) {
        if (fopAcM_searchActorDistanceXZ(this, daPy_getPlayerActorClass()) < daNpc_Kolin_Param_c::m.start_distance) {
            if (mMotionSeqMngr.getNo() != MOT_WALK_B) {
                mMotionSeqMngr.setNo(MOT_WALK_B, 4.0f, FALSE, 0);
            }
        } else {
            mMotionSeqMngr.setNo(MOT_RUN, 4.0f, FALSE, 0);
        }
    }

    mJntAnm.lookPlayer(0);
}

void daNpc_Kolin_c::lookup(u8 i_direct) {
    cXyz pos;
    pos.set(0.0f, 30.0f, 200.0f);
    mDoMtx_stack_c::transS(attention_info.position);
    mDoMtx_stack_c::YrotM(mCurAngle.y);
    mDoMtx_stack_c::multVec(&pos, &field_0xd6c);
    mJntAnm.lookPos(&field_0xd6c, i_direct);
}

int daNpc_Kolin_c::cutNoRide(int i_cutIndex) {
    int rv = 0;
    int prm = -1;

    int* piVar1 = dComIfGp_evmng_getMyIntegerP(i_cutIndex, "prm");
    if (piVar1 != NULL) {
        prm = *piVar1;
    }

    if (dComIfGp_getEventManager().getIsAddvance(i_cutIndex)) {
        switch (prm) {
            case 0:
                mPlayerActorMngr.entry(daPy_getPlayerActorClass());
                initTalk(4, NULL);
                setAngle(mPlayerAngle);
                break;

            case 1:
                break;
        }
    }

    switch (prm) {
        case 0:
            mJntAnm.lookPlayer(0);

            if (talkProc(NULL, FALSE, NULL, FALSE) && mFlow.checkEndFlow()) {
                rv = 1;
            }
            break;

        case 1:
            rv = 1;
            break;
    }

    return rv;
}

int daNpc_Kolin_c::cutHail(int i_cutIndex) {
    cXyz work;
    int rv = 0;
    int prm = -1;
    int msgNo = 0;

    int* piVar1 = dComIfGp_evmng_getMyIntegerP(i_cutIndex, "prm");
    if (piVar1 != NULL) {
        prm = *piVar1;
    }

    piVar1 = dComIfGp_evmng_getMyIntegerP(i_cutIndex, "msgNo");
    if (piVar1 != NULL) {
        msgNo = *piVar1;
    }

    if (dComIfGp_getEventManager().getIsAddvance(i_cutIndex)) {
        switch (prm) {
            case 0: {
                mFaceMotionSeqMngr.setNo(FACE_NONE, 0.0f, FALSE, 0);
                mMotionSeqMngr.setNo(MOT_WAIT_B, 0.0f, FALSE, 0);
                mJntAnm.lookNone(1);
                initTalk(mFlowNodeNo, NULL);
                speedF = 0.0f;
                speed.setall(0.0f);
                setAngle(cM_deg2s(-25.0f));
                work.set(0.0f, 500.0f, 150.0f);

                s16 angle = fopAcM_searchActorAngleY(daPy_getPlayerActorClass(), this);

                mDoMtx_stack_c::YrotS(mCurAngle.y);
                mDoMtx_stack_c::multVec(&work, &work);
                work += current.pos;
                mGndChk.SetPos(&work);
                work.y = dComIfG_Bgsp().GroundCross(&mGndChk);

                JUT_ASSERT(2646, -G_CM3D_F_INF != work.y);

                daPy_getPlayerActorClass()->setPlayerPosAndAngle(&work, angle, 0);
                break;
            }

            case 1:
            case 2:
                break;
        }
    }

    int iVar1[2] = {-1, -1};
    switch (prm) {
        case 0:
        case 1:
        case 2:
            if (prm == 1) {
                work.set(-100.0f, -100.0f, -100.0f);
                mDoMtx_stack_c::YrotS(mCurAngle.y);
                mDoMtx_stack_c::multVec(&work, &work);
                field_0xd6c = attention_info.position + work;
                mJntAnm.lookPos(&field_0xd6c, 0);
            } else {
                mJntAnm.lookNone(0);
            }

            iVar1[0] = msgNo;
            
            if (talkProc(iVar1, FALSE, NULL, FALSE)) {
                if (msgNo == 0) {
                    if (mFlow.checkEndFlow()) {
                        rv = 1;
                    }
                } else {
                    rv = 1;
                }
            }

            shape_angle.y = 0;
            break;
    }

    return rv;
}

int daNpc_Kolin_c::cutGiveMeWoodSwd(int i_cutIndex) {
    cXyz work;
    csXyz angle;
    int rv = 0;
    int prm = -1;

    int* piVar1 = dComIfGp_evmng_getMyIntegerP(i_cutIndex, "prm");
    if (piVar1 != NULL) {
        prm = *piVar1;
    }

    if (dComIfGp_getEventManager().getIsAddvance(i_cutIndex)) {
        switch (prm) {
            case 0:
                break;

            case 1:
                mFaceMotionSeqMngr.setNo(FACE_NONE, 0.0f, FALSE, 0);
                mMotionSeqMngr.setNo(MOT_WAIT_B, 0.0f, FALSE, 0);
                mJntAnm.lookNone(1);
                speedF = 0.0f;
                speed.setall(0.0f);
                work.set(-613.0f, 1306.0f, -2048.0f);
                setPos(work);
                setAngle(cM_deg2s(-130.0f));
                break;

            case 2:
                mFaceMotionSeqMngr.setNo(FACE_NONE, 0.0f, FALSE, 0);
                mMotionSeqMngr.setNo(MOT_WAIT_B, 0.0f, FALSE, 0);
                mJntAnm.lookNone(1);
                work.set(153.0f, 1300.0f, -1278.0f);
                setPos(work);
                setAngle(cM_deg2s(-130.0f));
                break;
        }
    }

    switch (prm) {
        case 0:
            action();
            rv = 1;
            break;

        case 1:
        case 2:
            rv = 1;
            break;
    }

    return rv;
}

int daNpc_Kolin_c::cutGetWoodSwd(int i_cutIndex) {
    cXyz work;
    csXyz angle;
    int rv = 0;
    int prm = -1;
    int timer = 0;
    int msgNo = 0;

    int* piVar1 = dComIfGp_evmng_getMyIntegerP(i_cutIndex, "prm");
    if (piVar1 != NULL) {
        prm = *piVar1;
    }

    piVar1 = dComIfGp_evmng_getMyIntegerP(i_cutIndex, "timer");
    if (piVar1 != NULL) {
        timer = *piVar1;
    }

    piVar1 = dComIfGp_evmng_getMyIntegerP(i_cutIndex, "msgNo");
    if (piVar1 != NULL) {
        msgNo = *piVar1;
    }

    if (dComIfGp_getEventManager().getIsAddvance(i_cutIndex)) {
        switch (prm) {
            case 0:
                break;

            case 1:
                mFaceMotionSeqMngr.setNo(FACE_NONE, 0.0f, FALSE, 0);
                mMotionSeqMngr.setNo(MOT_WAIT_B, 0.0f, FALSE, 0);
                mJntAnm.lookNone(1);
                speedF = 0.0f;
                speed.setall(0.0f);
                work.set(-613.0f, 1306.0f, -2048.0f);
                setPos(work);
                setAngle(cM_deg2s(-130.0f));
                mHide = true;
                break;

            case 2:
                mEventTimer = timer;
                break;

            case 3:
                work.set(0.0f, 0.0f, 100.0f);
                angle.y = daPy_getPlayerActorClass()->shape_angle.y + cM_deg2s(20.0f);
                mDoMtx_stack_c::YrotS(angle.y);
                mDoMtx_stack_c::multVec(&work, &work);
                work += daPy_getPlayerActorClass()->current.pos;
                setPos(work);
                setAngle(cLib_targetAngleY(&work, &daPy_getPlayerActorClass()->current.pos));
                daNpcT_offTmpBit(11);
                mHide = false;
                break;

            case 4:
                initTalk(48, NULL);
                break;

            case 5:
                mFaceMotionSeqMngr.setNo(FACE_NONE, -1.0f, FALSE, 0);
                mMotionSeqMngr.setNo(MOT_RUN, -1.0f, FALSE, 0);
                home.pos = current.pos;
                home.angle.y = mCurAngle.y;
                mEventTimer = timer;
                break;

            case 6:
                speedF = 0.0f;
                speed.setall(0.0f);
                mHide = true;
                break;

            case 7:
                break;
        }
    }

    int iVar1[3] = {-1, -1, -1};
    switch (prm) {
        case 0:
            action();
            rv = 1;
            break;

        case 1:
            rv = 1;
            break;

        case 2:
            if (cLib_calcTimer<int>(&mEventTimer) == 0) {
                rv = 1;
            }
            break;

        case 3:
            rv = 1;
            break;

        case 4:
        case 7:
            mJntAnm.lookPlayer(0);
            iVar1[0] = msgNo;

            if (talkProc(iVar1, FALSE, NULL, FALSE)) {
                if (msgNo == 0) {
                    if (mFlow.checkEndFlow()) {
                        rv = 1;
                    }
                } else {
                    rv = 1;
                }
            }
            break;

        case 5:
            mJntAnm.lookNone(0);
            work.set(0.0f, 0.0f, 1600.0f);
            mDoMtx_stack_c::YrotS(home.angle.y + cM_deg2s(-160.0f));
            mDoMtx_stack_c::multVec(&work, &work);
            work += home.pos;

            if (50.0f < current.pos.absXZ(work)) {
                current.angle.y = cLib_targetAngleY(&current.pos, &work);
                cLib_addCalcAngleS2(&shape_angle.y, current.angle.y, 4, 0x800);
                mCurAngle.y = shape_angle.y;
                cLib_chaseF(&speedF, 10.0f, 0.5f);
            }

            if (cLib_calcTimer<int>(&mEventTimer) == 0) {
                rv = 1;
            }
            break;

        case 6:
            /* dSv_event_flag_c::F_0026 - Ordon Village - gave wooden sword to talo on 3rd day */
            daNpcT_onEvtBit(31);
            rv = 1;
            break;
    }

    return rv;
}

int daNpc_Kolin_c::cutConversationAboutLoopHole(int i_cutIndex) {
    int rv = 0;
    int prm = -1;
    int msgNo = 0;
    int timer = 0;

    int* piVar1 = dComIfGp_evmng_getMyIntegerP(i_cutIndex, "prm");
    if (piVar1 != NULL) {
        prm = *piVar1;
    }

    piVar1 = dComIfGp_evmng_getMyIntegerP(i_cutIndex, "msgNo");
    if (piVar1 != NULL) {
        msgNo = *piVar1;
    }

    piVar1 = dComIfGp_evmng_getMyIntegerP(i_cutIndex, "timer");
    if (piVar1 != NULL) {
        timer = *piVar1;
    }

    fopAc_ac_c* speakers[2] = {mActorMngr[3].getActorP(), this};
    dComIfGp_setMesgCameraInfoActor(speakers[0], speakers[1], NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

    if (dComIfGp_getEventManager().getIsAddvance(i_cutIndex)) {
        switch (prm) {
            case 0:
                daNpcT_offTmpBit(11);
                daNpcT_offTmpBit(12);
                initTalk(64, (fopAc_ac_c**)&speakers[0]);
                break;

            case 2:
            case 3:
                initTalk(64, (fopAc_ac_c**)&speakers[0]);
                break;

            case 4:
                mEventTimer = timer;
                break;
        }
    }

    int iVar1[3] = {-1, -1, -1};
    switch (prm) {
        case 0:
            if (mPlayerAngle != mCurAngle.y) {
                mJntAnm.lookNone(0);

                if (step(mPlayerAngle, 13, 10, 15, 0)) {
                    mFaceMotionSeqMngr.setNo(FACE_NONE, -1.0f, FALSE, 0);
                    mMotionSeqMngr.setNo(MOT_WAIT_A, -1.0f, FALSE, 0);
                }

                shape_angle.y = mPlayerAngle;
            } else {
                lookup(0);
                rv = 1;
            }
            break;

        case 1:
        case 2:
        case 3:
            lookup(0);
            iVar1[0] = msgNo;

            if (talkProc(iVar1, FALSE, (fopAc_ac_c**)&speakers[0], FALSE)) {
                if (msgNo == 0) {
                    if (mFlow.checkEndFlow()) {
                        rv = 1;
                    }
                } else {
                    rv = 1;
                }
            }
            break;

        case 4:
            if (cLib_calcTimer<int>(&mEventTimer) == 0) {
                rv = 1;
            }
            break;
    }

    return rv;
}

int daNpc_Kolin_c::cutCacaricoConversation(int i_cutIndex) {
    int rv = 0;
    int prm = -1;

    int* piVar1 = dComIfGp_evmng_getMyIntegerP(i_cutIndex, "prm");
    if (piVar1 != NULL) {
        prm = *piVar1;
    }

    if (dComIfGp_getEventManager().getIsAddvance(i_cutIndex)) {
        switch (prm) {
            case 0:
                mPlayerAngle = fopAcM_searchPlayerAngleY(this);

                if (checkStep()) {
                    mStepMode = 0;
                }
                break;

            case 1:
                break;
        }
    }

    switch (prm) {
        case 0:
            mJntAnm.lookPlayer(0);

            if (mPlayerAngle != mCurAngle.y) {
                if (step(mPlayerAngle, 13, 10, 15, 0)) {
                    mFaceMotionSeqMngr.setNo(FACE_NONE, -1.0f, FALSE, 0);
                    mMotionSeqMngr.setNo(MOT_WAIT_A, -1.0f, FALSE, 0);
                }
            } else {
                rv = 1;
            }
            break;
        
        case 1:
            mJntAnm.lookPlayer(0);
            rv = 1;
            break;
    }

    return rv;
}

int daNpc_Kolin_c::cutConversationAboutDeathMt(int i_cutIndex) {
    int prm = -1;

    int* piVar1 = dComIfGp_evmng_getMyIntegerP(i_cutIndex, "prm");
    if (piVar1 != NULL) {
        prm = *piVar1;
    }

    dComIfGp_getEventManager().getIsAddvance(i_cutIndex);

    switch (prm) {
        case 0:
            action();
            break;
    }

    return 0;
}

int daNpc_Kolin_c::cutConversationAboutGoron(int i_cutIndex) {
    int prm = -1;

    int* piVar1 = dComIfGp_evmng_getMyIntegerP(i_cutIndex, "prm");
    if (piVar1 != NULL) {
        prm = *piVar1;
    }

    dComIfGp_getEventManager().getIsAddvance(i_cutIndex);

    switch (prm) {
        case 0:
            action();
            break;
    }

    return 0;
}

int daNpc_Kolin_c::cutClothTry(int i_cutIndex) {
    int rv = 0;
    int prm = -1;

    int* piVar1 = dComIfGp_evmng_getMyIntegerP(i_cutIndex, "prm");
    if (piVar1 != NULL) {
        prm = *piVar1;
    }

    if (dComIfGp_getEventManager().getIsAddvance(i_cutIndex)) {
        switch (prm) {
            case 0:
                initTalk(mFlowNodeNo, NULL);
                break;

            case 1:
                break;
        }
    }

    switch (prm) {
        case 0:
            mJntAnm.lookNone(0);

            if (talkProc(NULL, FALSE, NULL, FALSE) && mFlow.checkEndFlow()) {
                rv = 1;
            }
            break;
        
        case 1:
            rv = 1;
            break;
    }

    return rv;
}

int daNpc_Kolin_c::cutThankYou(int i_cutIndex) {
    int prm = -1;

    int* piVar1 = dComIfGp_evmng_getMyIntegerP(i_cutIndex, "prm");
    if (piVar1 != NULL) {
        prm = *piVar1;
    }

    dComIfGp_getEventManager().getIsAddvance(i_cutIndex);

    switch (prm) {
        case 0:
            action();
            return 1;
    }

    return 1;
}

static int const padding[2] = {-1, -1};

int daNpc_Kolin_c::wait(void* param_1) {
    fopAc_ac_c* actor_p;

    switch (mMode) {
        case 0:
        case 1:
            if (!mStagger.checkStagger()) {
                switch (mType) {
                    case 0:
                        mFaceMotionSeqMngr.setNo(FACE_H_NORMAL_TALK, -1.0f, FALSE, 0);
                        mMotionSeqMngr.setNo(MOT_WAIT_A, -1.0f, FALSE, 0);
                        break;

                    case 4:
                        if (getBitSW() != 0xFF) {
                            if (dComIfGs_isSwitch(getBitSW(), fopAcM_GetRoomNo(this))) {
                                mFaceMotionSeqMngr.setNo(FACE_DEMO_FEAR, -1.0f, FALSE, 0);
                                mMotionSeqMngr.setNo(MOT_DEMO_FEAR, -1.0f, FALSE, 0);
                                break;
                            }
                        }

                        mFaceMotionSeqMngr.setNo(FACE_DEMO_WAIT, -1.0f, FALSE, 0);
                        mMotionSeqMngr.setNo(MOT_DEMO_WAIT, -1.0f, FALSE, 0);
                        break;

                    default:
                        mFaceMotionSeqMngr.setNo(FACE_NONE, -1.0f, FALSE, 0);
                        mMotionSeqMngr.setNo(MOT_WAIT_A, -1.0f, FALSE, 0);
                        break;
                }

                field_0xffc = 0;
                field_0x1000 = 0;
                mMode = 2;
            }
            // fallthrough
        case 2:
            if (mType == 4) {
                actor_p = mActorMngr[4].getActorP();
                if (actor_p != NULL &&
                    ((daNpc_Len_c*)actor_p)->checkStartDemo13StbEvt(this, daNpc_Kolin_Param_c::m.common.box_min_x, daNpc_Kolin_Param_c::m.common.box_min_y,
                                                    daNpc_Kolin_Param_c::m.common.box_min_z, daNpc_Kolin_Param_c::m.common.box_max_x,
                                                    daNpc_Kolin_Param_c::m.common.box_max_y, daNpc_Kolin_Param_c::m.common.box_max_z,
                                                    daNpc_Kolin_Param_c::m.common.box_offset)) {
                    mEvtNo = 7;
                    field_0x1015 = 1;
                }

                if (field_0x1015 != 0 &&
                    /* dSv_event_flag_c::M_027 - Cutscene - [cutscene: 13] kids in the church (beast eyes) */
                    daNpcT_chkEvtBit(60) &&
                    !dComIfGp_event_runCheck()) {
                    field_0x1015 = 0;
                }
            }

            if (!mStagger.checkStagger()) {
                if (mType == 4) {
                    mJntAnm.lookNone(0);
                    
                    if (getBitSW() != 0xFF) {
                        if (dComIfGs_isSwitch(getBitSW(), fopAcM_GetRoomNo(this)) && mMotionSeqMngr.getNo() != MOT_DEMO_FEAR) {
                            mMode = 1;
                        }
                    }

                    attention_info.flags = 0;
                } else {
                    switch (mType) {
                        case 5:
                        case 6:
                            mPlayerActorMngr.remove();

                            if (dComIfGs_isTmpBit(0xD02)) {
                                mPlayerActorMngr.entry(daPy_getPlayerActorClass());
                            }

                            mJntAnm.lookNone(0);
                            break;

                        case 9:
                            mPlayerActorMngr.remove();
                            break;
                    }

                    if (mPlayerActorMngr.getActorP() != NULL && !mTwilight && field_0x1018 == 0) {
                        if (mType != 5 && mType != 6) {
                            mJntAnm.lookPlayer(0);

                            if (!chkActorInSight(mPlayerActorMngr.getActorP(), mAttnFovY, mCurAngle.y)) {
                                mJntAnm.lookNone(0);
                                
                                if (mType == 11) {
                                    field_0x1018 = 1;
                                }
                            }

                            if (!srchPlayerActor() && home.angle.y == mCurAngle.y) {
                                mMode = 1;
                            }
                        }
                    } else {
                        mJntAnm.lookNone(0);

                        if (home.angle.y != mCurAngle.y) {
                            if (field_0xe34 != 0) {
                                if (step(home.angle.y, 13, 10, 15, 0)) {
                                    mMode = 1;
                                }
                            } else {
                                setAngle(home.angle.y);
                                mMode = 1;
                            }

                            attention_info.flags = 0;
                        } else {
                            if (!mTwilight && mType != 5 && mType != 6) {
                                srchPlayerActor();
                            }

                            field_0x1018 = 0;
                        }
                    }
                }

                switch (mJntAnm.getMode()) {
                    case 0:
                        switch (mType) {
                            case 3:
                                actor_p = mActorMngr[3].getActorP();
                                
                                if (actor_p != NULL) {
                                    mJntAnm.lookActor(actor_p, 0.0f, 0);
                                }
                                break;

                            case 5:
                            case 6:
                                if (dComIfGs_isTmpBit(0xE40)) {
                                    mJntAnm.lookPlayer(0);
                                }
                                break;

                            case 11:
                                daHorse_c* horse_actor = dComIfGp_getHorseActor();
                                
                                if (horse_actor != NULL) {
                                    mJntAnm.lookActor(horse_actor, 60.0f, 0);
                                }
                        }
                        break;
                }

                switch (mType) {
                    case 5:
                    case 6:
                        attention_info.flags = 0;
                        break;
                }
            }
            break;

        case 3:
            break;
    }

    return 1;
}

int daNpc_Kolin_c::timidWalk(void* param_1) {
    fopAc_ac_c* actor_p;
    cXyz work;
    int shy_walk_time = daNpc_Kolin_Param_c::m.shy_walk_time;
    int sulk_time = daNpc_Kolin_Param_c::m.sulk_time;

    switch (mMode) {
        case 0:
        case 1:
            if (!mStagger.checkStagger()) {
                mFaceMotionSeqMngr.setNo(FACE_NONE, -1.0f, FALSE, 0);
                mMotionSeqMngr.setNo(MOT_WAIT_A, -1.0f, FALSE, 0);
                field_0x1016 = 1;
                field_0x1008 = 0;
                field_0x1004 = 0;
                mMode = 2;
            }
            // fallthrough
        case 2:;
            JUT_ASSERT(4359, NULL != mPath.getPathInfo());

                 /* dSv_event_flag_c::F_0010 - Ordon Village - First convo with Colin blocking path (forced) */
            if (!daNpcT_chkEvtBit(14)) {
                actor_p = mActorMngr[0].getActorP();

                if (actor_p != NULL) {
                    if (((daTag_EvtArea_c*)actor_p)->chkPointInArea(daPy_getPlayerActorClass()->current.pos)) {
                        mEvtNo = 4;
                    }
                }
            }

            if (!mStagger.checkStagger()) {
                if (mPlayerActorMngr.getActorP() != NULL) {
                    mJntAnm.lookPlayer(0);

                    if (!chkActorInSight(mPlayerActorMngr.getActorP(), mAttnFovY, mCurAngle.y)) {
                        mJntAnm.lookNone(0);
                    }

                    if (!srchPlayerActor()) {
                        mMode = 1;
                    }
                } else {
                    mJntAnm.lookNone(0);

                    if (field_0x1016 != 0) {
                        if (field_0x1008 == 0) {
                            mFaceMotionSeqMngr.setNo(FACE_NONE, -1.0f, FALSE, 0);
                            mMotionSeqMngr.setNo(MOT_WALK_A, -1.0f, FALSE, 0);
                            field_0x1004 = cLib_getRndValue<int>(shy_walk_time / 2, shy_walk_time);
                            field_0x1016 = 0;
                        } else {
                            field_0x1008--;
                        }
                    } else {
                        mPath.getDstPosH(current.pos, &work, mPath.getNumPnts(), 4);
                        calcSpeedAndAngle(work, field_0x1004 == 0, 4, 0x200);

                        if (field_0x1004 == 0) {
                            if (cM3d_IsZero(speedF)) {
                                mFaceMotionSeqMngr.setNo(FACE_NONE, -1.0f, FALSE, 0);

                                if (field_0x1010 <= cM_rnd()) {
                                    mMotionSeqMngr.setNo(MOT_STONE, 8.0f, FALSE, 0);
                                    field_0x1010 = 0.7f;
                                } else {
                                    mMotionSeqMngr.setNo(MOT_WAIT_B, 8.0f, FALSE, 0);
                                    field_0x1010 -= 0.1f;
                                }

                                field_0x1008 = cLib_getRndValue<int>(sulk_time / 2, sulk_time);
                                field_0x1016 = 1;
                            }
                        } else {
                            field_0x1004--;
                        }
                    }

                    if (!mTwilight) {
                        srchPlayerActor();
                    }
                }

                mJntAnm.getMode();
            }
            break;

        case 3:
            break;
    }

    return 1;
}

int daNpc_Kolin_c::follow(void* param_1) {
    cXyz work;

    switch (mMode) {
        case 0:
        case 1:
            if (!mStagger.checkStagger()) {
                mFaceMotionSeqMngr.setNo(FACE_NONE, -1.0f, FALSE, 0);
                mMotionSeqMngr.setNo(MOT_WAIT_A, -1.0f, FALSE, 0);
                field_0x1016 = 1;
                field_0x1008 = 0;
                field_0x1004 = 0;
                mMode = 2;
            }
            // fallthrough
        case 2:
            if (mType == 0 && mHide) {
                fopAcM_delete(this);
                return 1;
            }

            if (!mStagger.checkStagger()) {
                mJntAnm.lookNone(0);
                followPlayer(0);
            }
            break;

        case 3:
            break;
    }

    return 1;
}

int daNpc_Kolin_c::clothWait(void* param_1) {
    int sulk_time = daNpc_Kolin_Param_c::m.sulk_time;

    switch (mMode) {
        case 0:
        case 1:
            if (!mStagger.checkStagger()) {
                mFaceMotionSeqMngr.setNo(FACE_NONE, -1.0f, FALSE, 0);
                mMotionSeqMngr.setNo(MOT_CLOTH_WAIT, -1.0f, FALSE, 0);
                field_0x1008 = sulk_time + (int)(sulk_time * (cM_rnd() - 0.5f));
                mMode = 2;
            }
            // fallthrough
        case 2:
            if (!mStagger.checkStagger()) {
                mJntAnm.lookNone(0);

                if (home.angle.y != mCurAngle.y) {
                    if (field_0xe34 == 0) {
                        setAngle(home.angle.y);
                        mMode = 1;
                    } else if (step(home.angle.y, 13, 10, 15, 0)) {
                        mMode = 1;
                    }

                    attention_info.flags = 0;
                } else if (field_0x1008 != 0) {
                    if (cLib_calcTimer<int>(&field_0x1008) == 0) {
                        mFaceMotionSeqMngr.setNo(FACE_CLOTH_TRY, -1.0f, FALSE, 0);
                        mMotionSeqMngr.setNo(MOT_CLOTH_TRY, -1.0f, FALSE, 0);
                    }
                } else if (mMotionSeqMngr.checkEndSequence() != 0) {
                    mMode = 1;
                }
            }
            break;

        case 3:
            break;
    }

    return 1;
}

int daNpc_Kolin_c::talk(void* param_1) {
    switch (mMode) {
        case 0:
        case 1:
            if (!mStagger.checkStagger()) {
                if (mMotionSeqMngr.getNo() == MOT_RUN) {
                    mMotionSeqMngr.setNo(MOT_WAIT_A, -1.0f, FALSE, 0);
                }

                initTalk(mFlowNodeNo, NULL);
                mMode = 2;
            }
            // fallthrough
        case 2:
            if (!mStagger.checkStagger()) {
                if (mTwilight || mPlayerAngle == mCurAngle.y || mType == 10) {
                    if (talkProc(NULL, FALSE, NULL, FALSE) && mFlow.checkEndFlow()) {
                        mPlayerActorMngr.entry(daPy_getPlayerActorClass());
                        dComIfGp_event_reset();
                        mMode = 3;
                    }

                    mJntAnm.lookPlayer(0);

                    if (mTwilight || mType == 10) {
                        mJntAnm.lookNone(0);
                    }
                } else {
                    mJntAnm.lookPlayer(0);
                    step(mPlayerAngle, 13, 10, 15, 0);
                }
            }
            break;

        case 3:
            break;
    }

    return 0;
}

static int daNpc_Kolin_Create(void* a_this) {
    daNpc_Kolin_c* i_this = (daNpc_Kolin_c*)a_this;
    return i_this->create();
}

static int daNpc_Kolin_Delete(void* a_this) {
    daNpc_Kolin_c* i_this = (daNpc_Kolin_c*)a_this;
    return i_this->Delete();
}

static int daNpc_Kolin_Execute(void* a_this) {
    daNpc_Kolin_c* i_this = (daNpc_Kolin_c*)a_this;
    return i_this->Execute();
}

static int daNpc_Kolin_Draw(void* a_this) {
    daNpc_Kolin_c* i_this = (daNpc_Kolin_c*)a_this;
    return i_this->Draw();
}

static int daNpc_Kolin_IsDelete(void* a_this) {
    return 1;
}

AUDIO_INSTANCES;

static actor_method_class daNpc_Kolin_MethodTable = {
    (process_method_func)daNpc_Kolin_Create,
    (process_method_func)daNpc_Kolin_Delete,
    (process_method_func)daNpc_Kolin_Execute,
    (process_method_func)daNpc_Kolin_IsDelete,
    (process_method_func)daNpc_Kolin_Draw,
};

extern actor_process_profile_definition g_profile_NPC_KOLIN = {
  fpcLy_CURRENT_e,          // mLayerID
  7,                        // mListID
  fpcPi_CURRENT_e,          // mListPrio
  PROC_NPC_KOLIN,           // mProcName
  &g_fpcLf_Method.base,    // sub_method
  sizeof(daNpc_Kolin_c),    // mSize
  0,                        // mSizeOther
  0,                        // mParameters
  &g_fopAc_Method.base,     // sub_method
  350,                      // mPriority
  &daNpc_Kolin_MethodTable, // sub_method
  0x00040107,               // mStatus
  fopAc_NPC_e,              // mActorType
  fopAc_CULLBOX_CUSTOM_e,   // cullType
};
