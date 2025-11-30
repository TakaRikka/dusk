/**
 * @file d_a_npc_yelia.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_npc_yelia.h"
#include "d/actor/d_a_demo_item.h"

static daNpc_GetParam1 l_bmdData[3] = {
    {3, 1},
    {3, 4},
    {8, 7},
};

static daNpcT_evtData_c l_evtList[6] = {
    {"", 0},
    {"NO_RESPONSE", 0},
    {"TW_RESISTANCE1", 7},
    {"TW_RESISTANCE2", 7},
    {"TAKE_WOODSTATUE", 6},
    {"THANK_YOU", 6},
};

static char* l_resNameList[8] = {
    "",
    "Yelia",
    "Yelia0",
    "Yelia1",
    "Yelia2",
    "Yelia3",
    "Yelia4",
    "yelB_TW",
};

static s8 l_loadResPtrn0[4] = {1, 2, 3, -1};

static s8 l_loadResPtrn1[3] = {2, 4, -1};

static s8 l_loadResPtrn2[4] = {1, 2, 5, -1};

static s8 l_loadResPtrn3[4] = {2, 4, 6, -1};

static s8 l_loadResPtrn4[4] = {2, 7, 6, -1};

static s8 l_loadResPtrn5[5] = {2, 4, 7, 6, -1};

static s8 l_loadResPtrn9[8] = {1, 2, 3, 4, 5, 6, 7, -1};

static s8* l_loadResPtrnList[10] = {
    l_loadResPtrn0, l_loadResPtrn1, l_loadResPtrn2, l_loadResPtrn3, l_loadResPtrn4,
    l_loadResPtrn5, l_loadResPtrn3, l_loadResPtrn3, l_loadResPtrn9, l_loadResPtrn0,
};

static daNpcT_faceMotionAnmData_c l_faceMotionAnmData[] = {
    {-1, J3DFrameCtrl::EMode_NONE, 0, 19, J3DFrameCtrl::EMode_LOOP, 2, TRUE},
    {7, J3DFrameCtrl::EMode_NONE, 2, 19, J3DFrameCtrl::EMode_LOOP, 2, TRUE},
    {-1, J3DFrameCtrl::EMode_NONE, 0, -1, J3DFrameCtrl::EMode_NONE, 0, FALSE},
    {-1, J3DFrameCtrl::EMode_NONE, 0, -1, J3DFrameCtrl::EMode_NONE, 0, FALSE},
    {5, J3DFrameCtrl::EMode_NONE, 3, 18, J3DFrameCtrl::EMode_NONE, 3, FALSE},
    {6, J3DFrameCtrl::EMode_NONE, 3, 19, J3DFrameCtrl::EMode_NONE, 3, FALSE},
    {6, J3DFrameCtrl::EMode_LOOP, 5, 20, J3DFrameCtrl::EMode_LOOP, 5, FALSE},
    {5, J3DFrameCtrl::EMode_NONE, 5, 19, J3DFrameCtrl::EMode_NONE, 5, FALSE},
    {7, J3DFrameCtrl::EMode_LOOP, 5, 19, J3DFrameCtrl::EMode_LOOP, 2, TRUE},
    {8, J3DFrameCtrl::EMode_NONE, 5, 19, J3DFrameCtrl::EMode_LOOP, 2, TRUE},
    {14, J3DFrameCtrl::EMode_NONE, 6, 49, J3DFrameCtrl::EMode_NONE, 6, FALSE},
    {16, J3DFrameCtrl::EMode_NONE, 6, 51, J3DFrameCtrl::EMode_NONE, 6, FALSE},
    {15, J3DFrameCtrl::EMode_NONE, 6, 50, J3DFrameCtrl::EMode_NONE, 6, FALSE},
    {12, J3DFrameCtrl::EMode_NONE, 6, 47, J3DFrameCtrl::EMode_NONE, 6, FALSE},
    {13, J3DFrameCtrl::EMode_NONE, 6, 48, J3DFrameCtrl::EMode_NONE, 6, FALSE},
    {10, J3DFrameCtrl::EMode_NONE, 6, 45, J3DFrameCtrl::EMode_NONE, 6, FALSE},
    {21, J3DFrameCtrl::EMode_LOOP, 6, 56, J3DFrameCtrl::EMode_LOOP, 6, FALSE},
    {22, J3DFrameCtrl::EMode_LOOP, 6, 57, J3DFrameCtrl::EMode_LOOP, 6, FALSE},
    {18, J3DFrameCtrl::EMode_LOOP, 6, 53, J3DFrameCtrl::EMode_LOOP, 6, FALSE},
    {20, J3DFrameCtrl::EMode_LOOP, 6, 55, J3DFrameCtrl::EMode_LOOP, 6, FALSE},
    {9, J3DFrameCtrl::EMode_NONE, 6, 44, J3DFrameCtrl::EMode_NONE, 6, FALSE},
    {17, J3DFrameCtrl::EMode_LOOP, 6, 52, J3DFrameCtrl::EMode_LOOP, 6, FALSE},
    {11, J3DFrameCtrl::EMode_NONE, 6, 46, J3DFrameCtrl::EMode_NONE, 6, FALSE},
    {19, J3DFrameCtrl::EMode_LOOP, 6, 54, J3DFrameCtrl::EMode_LOOP, 6, FALSE},
    {5, J3DFrameCtrl::EMode_NONE, 2, 20, J3DFrameCtrl::EMode_NONE, 2, FALSE},
    {8, J3DFrameCtrl::EMode_LOOP, 2, 22, J3DFrameCtrl::EMode_LOOP, 2, FALSE},
    {6, J3DFrameCtrl::EMode_LOOP, 2, 21, J3DFrameCtrl::EMode_LOOP, 2, FALSE},
    {9, J3DFrameCtrl::EMode_LOOP, 2, 23, J3DFrameCtrl::EMode_LOOP, 2, FALSE},
};

static daNpcT_motionAnmData_c l_motionAnmData[] = {
    {13, J3DFrameCtrl::EMode_LOOP, 2, 16, J3DFrameCtrl::EMode_NONE, 2, TRUE},
    {-1, J3DFrameCtrl::EMode_NONE, 0, -1, J3DFrameCtrl::EMode_NONE, 0, FALSE},
    {-1, J3DFrameCtrl::EMode_NONE, 0, -1, J3DFrameCtrl::EMode_NONE, 0, FALSE},
    {-1, J3DFrameCtrl::EMode_NONE, 0, -1, J3DFrameCtrl::EMode_NONE, 0, FALSE},
    {10, J3DFrameCtrl::EMode_LOOP, 3, 15, J3DFrameCtrl::EMode_LOOP, 3, FALSE},
    {-1, J3DFrameCtrl::EMode_NONE, 0, -1, J3DFrameCtrl::EMode_NONE, 0, FALSE},
    {11, J3DFrameCtrl::EMode_LOOP, 3, 16, J3DFrameCtrl::EMode_NONE, 2, TRUE},
    {9, J3DFrameCtrl::EMode_NONE, 3, 16, J3DFrameCtrl::EMode_NONE, 2, TRUE},
    {-1, J3DFrameCtrl::EMode_NONE, 0, -1, J3DFrameCtrl::EMode_NONE, 0, FALSE},
    {-1, J3DFrameCtrl::EMode_NONE, 0, -1, J3DFrameCtrl::EMode_NONE, 0, FALSE},
    {7, J3DFrameCtrl::EMode_NONE, 3, 16, J3DFrameCtrl::EMode_NONE, 2, TRUE},
    {8, J3DFrameCtrl::EMode_NONE, 3, 14, J3DFrameCtrl::EMode_NONE, 3, FALSE},
    {10, J3DFrameCtrl::EMode_LOOP, 5, 15, J3DFrameCtrl::EMode_LOOP, 5, FALSE},
    {9, J3DFrameCtrl::EMode_NONE, 5, 14, J3DFrameCtrl::EMode_NONE, 5, FALSE},
    {11, J3DFrameCtrl::EMode_LOOP, 5, 16, J3DFrameCtrl::EMode_LOOP, 5, FALSE},
    {35, J3DFrameCtrl::EMode_LOOP, 6, 16, J3DFrameCtrl::EMode_NONE, 2, TRUE},
    {34, J3DFrameCtrl::EMode_LOOP, 6, 41, J3DFrameCtrl::EMode_LOOP, 6, FALSE},
    {33, J3DFrameCtrl::EMode_LOOP, 6, 40, J3DFrameCtrl::EMode_LOOP, 6, FALSE},
    {31, J3DFrameCtrl::EMode_NONE, 6, 16, J3DFrameCtrl::EMode_NONE, 2, TRUE},
    {30, J3DFrameCtrl::EMode_LOOP, 6, 16, J3DFrameCtrl::EMode_NONE, 2, TRUE},
    {27, J3DFrameCtrl::EMode_NONE, 6, 16, J3DFrameCtrl::EMode_NONE, 2, TRUE},
    {28, J3DFrameCtrl::EMode_LOOP, 6, 16, J3DFrameCtrl::EMode_NONE, 2, TRUE},
    {25, J3DFrameCtrl::EMode_NONE, 6, 38, J3DFrameCtrl::EMode_NONE, 6, FALSE},
    {26, J3DFrameCtrl::EMode_LOOP, 6, 39, J3DFrameCtrl::EMode_LOOP, 6, FALSE},
    {6, J3DFrameCtrl::EMode_NONE, 6, 16, J3DFrameCtrl::EMode_NONE, 2, TRUE},
    {8, J3DFrameCtrl::EMode_LOOP, 6, 16, J3DFrameCtrl::EMode_NONE, 2, TRUE},
    {12, J3DFrameCtrl::EMode_NONE, 2, 16, J3DFrameCtrl::EMode_NONE, 2, TRUE},
    {5, J3DFrameCtrl::EMode_LOOP, 7, 16, J3DFrameCtrl::EMode_NONE, 2, TRUE},
    {32, J3DFrameCtrl::EMode_NONE, 6, 16, J3DFrameCtrl::EMode_NONE, 2, FALSE},
    {29, J3DFrameCtrl::EMode_LOOP, 6, 16, J3DFrameCtrl::EMode_NONE, 2, TRUE},
    {7, J3DFrameCtrl::EMode_NONE, 6, 16, J3DFrameCtrl::EMode_NONE, 2, FALSE},
    {23, J3DFrameCtrl::EMode_NONE, 6, 16, J3DFrameCtrl::EMode_NONE, 2, FALSE},
    {24, J3DFrameCtrl::EMode_LOOP, 6, 16, J3DFrameCtrl::EMode_NONE, 2, FALSE},
    {10, J3DFrameCtrl::EMode_NONE, 2, 16, J3DFrameCtrl::EMode_NONE, 2, TRUE},
    {11, J3DFrameCtrl::EMode_NONE, 2, 16, J3DFrameCtrl::EMode_NONE, 2, TRUE},
};

static daNpcT_MotionSeqMngr_c::sequenceStepData_c l_faceMotionSequenceData[] = {
    {1, -1, 1}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {2, -1, 1}, {3, -1, 0}, {-1, 0, 0}, {-1, 0, 0},
    {7, -1, 1}, {9, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {10, -1, 1}, {16, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {11, -1, 1}, {17, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {12, -1, 1}, {17, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {13, -1, 1}, {18, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {14, -1, 1}, {19, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {15, -1, 1}, {18, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {20, -1, 1}, {21, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {24, -1, 1}, {25, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {26, -1, 1}, {27, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {3, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {4, -1, 1}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {5, -1, 1}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {9, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {6, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {16, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {17, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {18, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {19, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {21, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {25, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {22, -1, 0}, {23, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {23, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {27, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {0, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
};

static daNpcT_MotionSeqMngr_c::sequenceStepData_c l_motionSequenceData[] = {
    {0, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {2, -1, 1}, {0, -1, 0}, {-1, 0, 0}, {-1, 0, 0},
    {3, -1, 1}, {1, -1, 0}, {-1, 0, 0}, {-1, 0, 0},
    {12, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {7, -1, 1}, {6, -1, 0}, {-1, 0, 0}, {-1, 0, 0},
    {6, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {13, -1, 1}, {14, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {15, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {16, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {17, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {18, -1, 1}, {19, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {19, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {20, -1, 1}, {21, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {21, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {22, -1, 1}, {23, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {23, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {24, -1, 1}, {25, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {25, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {27, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {29, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {33, -1, 1}, {0, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {34, -1, 1}, {0, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {4, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {5, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {8, -1, 1}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {9, -1, 1}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {10, -1, 1}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {11, -1, 1}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {14, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {28, -1, 0}, {15, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {26, 4, 1}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {30, 4, 1}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {31, -1, 0}, {32, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {32, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
};

char* daNpc_Yelia_c::mCutNameList[6] = {
    "",
    "CONVERSATION_ABOUT_LOOPHOLE",
    "TW_RESISTANCE",
    "TAKE_WOODSTATUE",
    "CLOTH_TRY",
    "THANK_YOU",
};

BOOL (daNpc_Yelia_c::*daNpc_Yelia_c::mCutList[6])(int) = {
    NULL,
    &daNpc_Yelia_c::cutConversationAboutLoopHole,
    &daNpc_Yelia_c::cutTWResistance,
    &daNpc_Yelia_c::cutTakeWoodStatue,
    &daNpc_Yelia_c::cutClothTry,
    &daNpc_Yelia_c::cutThankYou,
};

enum Type {
    /* 0 */ TYPE_HORSE,
    /* 1 */ TYPE_1,
    /* 2 */ TYPE_REMINISCENCE,
    /* 3 */ TYPE_KAKARIKO,
    /* 4 */ TYPE_TWILIGHT,
    /* 5 */ TYPE_AFTER_ESCORT,
    /* 6 */ TYPE_WOOD_STATUE,
    /* 7 */ TYPE_MEMORIES_RESTORED,
    /* 8 */ TYPE_8,
};

enum Event {
    /* 0 */ EVENT_NONE,
    /* 1 */ EVENT_NO_RESPONSE,
    /* 2 */ EVENT_TW_RESISTANCE1,
    /* 3 */ EVENT_TW_RESISTANCE2,
    /* 4 */ EVENT_TAKE_WOODSTATUE,
    /* 5 */ EVENT_THANK_YOU,
};

enum FaceMotion {
    /* 0x00 */ FACE_TALK_A,
    /* 0x01 */ FACE_1,
    /* 0x02 */ FACE_KAISOU_TALK,
    /* 0x03 */ FACE_SAD,
    /* 0x04 */ FACE_TO_THINK,
    /* 0x05 */ FACE_THINK,
    /* 0x06 */ FACE_REMEMBER,
    /* 0x07 */ FACE_REMEMBERING,
    /* 0x08 */ FACE_INTENSE,
    /* 0x09 */ FACE_ASK,
    /* 0x0A */ FACE_HAPPY_TALK,
    /* 0x0B */ FACE_LISTEN,
    /* 0x0C */ FACE_12,
    /* 0x0D */ FACE_HORSE_LOOKBACK,
    /* 0x0E */ FACE_HORSE_RETURN,
    /* 0x0F */ FACE_H_KAISOU_TALK,
    /* 0x10 */ FACE_KAISOU_WAIT_A,
    /* 0x11 */ FACE_H_SAD,
    /* 0x12 */ FACE_H_THINK,
    /* 0x13 */ FACE_H_INTENSE,
    /* 0x14 */ FACE_H_REMEMBERING,
    /* 0x15 */ FACE_H_ASK,
    /* 0x16 */ FACE_H_HAPPY_TALK,
    /* 0x17 */ FACE_KIZUKU,
    /* 0x18 */ FACE_H_KIZUKU,
    /* 0x19 */ FACE_H_LISTEN,
    /* 0x1A */ FACE_NONE,
};

enum Motion {
    /* 0x00 */ MOTION_WAIT_A,
    /* 0x01 */ MOTION_1,
    /* 0x02 */ MOTION_2,
    /* 0x03 */ MOTION_KAISOU_WAIT_A,
    /* 0x04 */ MOTION_HORSE_TALK_A,
    /* 0x05 */ MOTION_HORSE_WAIT_C,
    /* 0x06 */ MOTION_KAISOU_TALK,
    /* 0x07 */ MOTION_WAIT_WORRY,
    /* 0x08 */ MOTION_WAIT_WONDER,
    /* 0x09 */ MOTION_WAIT_UTUMUKU,
    /* 0x0A */ MOTION_TO_THINK,
    /* 0x0B */ MOTION_THINK,
    /* 0x0C */ MOTION_REMEMBER,
    /* 0x0D */ MOTION_REMEMBERING,
    /* 0x0E */ MOTION_LOOK,
    /* 0x0F */ MOTION_LOOKING,
    /* 0x10 */ MOTION_ASK,
    /* 0x11 */ MOTION_ASKING,
    /* 0x12 */ MOTION_DEMO_WAIT_SIT,
    /* 0x13 */ MOTION_SITTALK_A,
    /* 0x14 */ MOTION_HAPPYTALK,
    /* 0x15 */ MOTION_LISTEN,
    /* 0x16 */ MOTION_HORSE_WAIT_A,
    /* 0x17 */ MOTION_17,
    /* 0x18 */ MOTION_18,
    /* 0x19 */ MOTION_19,
    /* 0x1A */ MOTION_HORSE_LOOKBACK,
    /* 0x1B */ MOTION_HORSE_RETURN,
    /* 0x1C */ MOTION_KAISOU_WAIT_B,
    /* 0x1D */ MOTION_TO_WORRY,
    /* 0x1E */ MOTION_STEP,
    /* 0x1F */ MOTION_ASK_STEP,
    /* 0x20 */ MOTION_KIZUKU,
    /* 0x21 */ MOTION_KIZUKU_WAIT,
};

daNpc_Yelia_Param_c::param const daNpc_Yelia_Param_c::m = {
    170.0f,   // mAttnOffsetY
    -3.0f,    // mGravity
    1.0f,     // mScale
    400.0f,   // mShadowDepth
    255.0f,   // mCcWeight
    160.0f,   // mCylH
    35.0f,    // mWallH
    30.0f,    // mWallR
    0.0f,     // mBodyUpAngle
    0.0f,     // mBodyDownAngle
    10.0f,    // mBodyLeftAngle
    -10.0f,   // mBodyRightAngle
    30.0f,    // mHeadUpAngle
    -10.0f,   // mHeadDownAngle
    45.0f,    // mHeadLeftAngle
    -45.0f,   // mHeadRightAngle
    0.6f,     // mNeckAngleScl
    12.0f,    // mMorfFrames
    3,        // mSpeakDistIdx
    6,        // mSpeakAngleIdx
    5,        // mTalkDistIdx
    6,        // mTalkAngleIdx
    110.0f,   // mAttnFovY
    0.0f,     // mAttnRadius
    0.0f,     // mAttnUpperY
    0.0f,     // mAttnLowerY
    60,
    8,        // mDamageTimer
    0,        // mTestExpression
    0,        // mTestMotion
    0,        // mTestLookMode
    false,    // mTest
    4.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
};

static daNpc_Yelia_Param_c l_HIO;

daNpc_Yelia_c::~daNpc_Yelia_c() {
    if (mpMorf[0] != NULL) {
        mpMorf[0]->stopZelAnime();
    }
    deleteRes(l_loadResPtrnList[mType], (const char**)l_resNameList);
}

cPhs__Step daNpc_Yelia_c::create() {
    if (!fopAcM_CheckCondition(this, fopAcCnd_INIT_e)) {
        new (this) daNpc_Yelia_c(l_faceMotionAnmData, l_motionAnmData, l_faceMotionSequenceData, 4,
                                 l_motionSequenceData, 4, l_evtList, l_resNameList);
        fopAcM_OnCondition(this, fopAcCnd_INIT_e);
    }

    mType = getType();
    mFlowNodeNo = getFlowNodeNo();
    mTwilight = false;
    if (mType == TYPE_TWILIGHT) {
        mTwilight = true;
    }

    cPhs__Step step = (cPhs__Step)loadRes(l_loadResPtrnList[mType], (const char**)l_resNameList);

    if (step == cPhs_COMPLEATE_e) {
        if (isDelete()) {
            return cPhs_ERROR_e;
        }

        static const int heapSize[] = {0x44d0, 0, 0x44d0, 0x44d0, 0x4410,
                                       0x44d0, 0x44d0, 0x44d0, 0x44d0, 0};
        if (!fopAcM_entrySolidHeap(this, createHeapCallBack, heapSize[mType])) {
            return cPhs_ERROR_e;
        }

        fopAcM_SetMtx(this, mpMorf[0]->getModel()->getBaseTRMtx());
        fopAcM_setCullSizeBox(this, -200.0f, -100.0f, -200.0f, 200.0f, 300.0, 200.0f);
        mSound.init(&current.pos, &eyePos, 3, 1);
        reset();
        mAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1, &mAcchCir,
                  fopAcM_GetSpeed_p(this), fopAcM_GetAngle_p(this), fopAcM_GetShapeAngle_p(this));
        mCcStts.Init(l_HIO.m.mCcWeight, 0, this);
        mCcCyl.Set(mCcDCyl);
        mCcCyl.SetStts(&mCcStts);
        mCcCyl.SetTgHitCallback(tgHitCallBack);
        if (mType == TYPE_TWILIGHT || mType == TYPE_AFTER_ESCORT) {
            mAcch.SetGrndNone();
            mAcch.SetWallNone();
        }
        mAcch.CrrPos(dComIfG_Bgsp());
        mGndChk = mAcch.m_gnd;
        mGroundH = mAcch.GetGroundH();
        if (mGroundH != -1.0e9f) {
            setEnvTevColor();
            setRoomNo();
        }
        reset();
        mCreating = true;
        Execute();
        mCreating = false;
    }

    return step;
}

int daNpc_Yelia_c::CreateHeap() {
    int bmd_idx = 1;
    if (mTwilight) {
        bmd_idx = 2;
    } else if (mType == TYPE_HORSE || mType == TYPE_REMINISCENCE || mType == TYPE_8) {
        bmd_idx = 0;
    }

    J3DModelData* model_data = static_cast<J3DModelData*>(
        dComIfG_getObjectRes(l_resNameList[l_bmdData[bmd_idx].arcIdx], l_bmdData[bmd_idx].fileIdx));
    if (model_data == NULL) {
        return 0;
    }
    
    mpMorf[0] = new mDoExt_McaMorfSO(model_data, NULL, NULL, NULL, -1, 1.0f, 0, -1,
                                     &mSound, 0x80000, 0x11020284);
    if (mpMorf[0] == NULL || mpMorf[0]->getModel() == NULL) {
        return 0;
    }

    J3DModel* model = mpMorf[0]->getModel();
    for (u16 i = 0; i < model_data->getJointNum(); i++) {
        model_data->getJointNodePointer(i)->setCallBack(ctrlJointCallBack);
    }
    model->setUserArea((uintptr_t)this);

    mpMatAnm[0] = new daNpcT_MatAnm_c();
    if (mpMatAnm[0] == NULL) {
        return 0;
    }

    if (mTwilight) {
        if (setFaceMotionAnm(1, false) && setMotionAnm(0, 0.0f, 0)) {
            return 1;
        } else {
            return 0;
        }
    } else {
        if (setFaceMotionAnm(1, false) && setMotionAnm(0, 0.0f, 0)) {
            return 1;
        } else {
            return 0;
        }
    }
}

int daNpc_Yelia_c::Delete() {
    this->~daNpc_Yelia_c();
    return 1;
}

int daNpc_Yelia_c::Execute() {
    return execute();
}

int daNpc_Yelia_c::Draw() {
    if (mpMatAnm[0] != NULL) {
        J3DModelData* model_data = mpMorf[0]->getModel()->getModelData();
        model_data->getMaterialNodePointer(getEyeballMaterialNo())->setMaterialAnm(mpMatAnm[0]);
    }
    return draw(FALSE, FALSE, mRealShadowSize, NULL, 100.0f, FALSE, FALSE, FALSE);
}

int daNpc_Yelia_c::createHeapCallBack(fopAc_ac_c* i_this) {
    return static_cast<daNpc_Yelia_c*>(i_this)->CreateHeap();
}

int daNpc_Yelia_c::ctrlJointCallBack(J3DJoint* i_joint, int param_1) {
    if (param_1 == 0) {
        J3DModel* model = j3dSys.getModel();
        daNpc_Yelia_c* _this = (daNpc_Yelia_c*)model->getUserArea();
        if (_this != NULL) {
            _this->ctrlJoint(i_joint, model);
        }
    }
    return 1;
}

u8 daNpc_Yelia_c::getType() {
    switch (fopAcM_GetParam(this) & 0xff) {
    case 0:
        return TYPE_HORSE;
    case 1:
        return TYPE_1;
    case 2:
        return TYPE_REMINISCENCE;
    case 3:
        return TYPE_KAKARIKO;
    case 4:
        return TYPE_TWILIGHT;
    case 5:
        return TYPE_AFTER_ESCORT;
    case 6:
        return TYPE_WOOD_STATUE;
    case 7:
        return TYPE_MEMORIES_RESTORED;
    case 8:
        return TYPE_8;
    default:
        return TYPE_REMINISCENCE;
    }
}

BOOL daNpc_Yelia_c::isDelete() {
    switch (mType) {
    case TYPE_HORSE:
        return false;
    case TYPE_1:
        return false;
    case TYPE_REMINISCENCE:
        return false;
    case TYPE_KAKARIKO:
        return !daNpcT_chkEvtBit(0x108) || daNpcT_chkEvtBit(0x11f);
    case TYPE_TWILIGHT:
        return false;
    case TYPE_AFTER_ESCORT:
        return !daNpcT_chkEvtBit(0x44) || daNpcT_chkEvtBit(0x108);
    case TYPE_WOOD_STATUE:
        return !daNpcT_chkEvtBit(0x11f) || daNpcT_chkEvtBit(0x120);
    case TYPE_MEMORIES_RESTORED:
        return !daNpcT_chkEvtBit(0x120);
    case TYPE_8:
        return false;
    default:
        return false;
    }
}

void daNpc_Yelia_c::reset() {
    u32 size = (uintptr_t)&field_0xfec - (uintptr_t)&mpNextAction;
    if (mpMatAnm[0] != NULL) {
        mpMatAnm[0]->initialize();
    }
    initialize();
    for (int i = 0; i < 4; i++) {
        mActorMngr[i].initialize();
    }
    memset(&mpNextAction, 0, size);
    mItemId = -1;
    setAngle(home.angle.y);
}

void daNpc_Yelia_c::afterJntAnm(int i_jointNo) {
    if (i_jointNo == 1) {
        mDoMtx_stack_c::YrotM(-mStagger.getAngleZ(1));
        mDoMtx_stack_c::ZrotM(mStagger.getAngleX(1));
    } else if (i_jointNo == 4) {
        mDoMtx_stack_c::YrotM(-mStagger.getAngleZ(0));
        mDoMtx_stack_c::ZrotM(mStagger.getAngleX(0));
    }
}

void daNpc_Yelia_c::setParam() {
    selectAction();
    srchActors();

    s16 speak_dist_idx = l_HIO.m.mSpeakDistIdx;
    s16 speak_angle_idx = l_HIO.m.mSpeakAngleIdx;
    s16 talk_dist_idx = l_HIO.m.mTalkDistIdx;
    s16 talk_angle_idx = l_HIO.m.mTalkAngleIdx;
    if (mType == TYPE_8) {
        speak_dist_idx = 16;
        speak_angle_idx = 6;
        talk_dist_idx = 19;
        talk_angle_idx = 6;
    } else if (mType == TYPE_KAKARIKO && !daNpcT_chkEvtBit(0x11f)) {
        speak_angle_idx = 3;
        talk_angle_idx = 3;
    }
    if (daPy_getPlayerActorClass()->checkHorseRide()) {
        if (speak_dist_idx < 7) {
            speak_dist_idx = 7;
        }
        if (talk_dist_idx < 9) {
            talk_dist_idx = 9;
        }
    }
    attention_info.distances[fopAc_attn_LOCK_e] =
        daNpcT_getDistTableIdx(talk_dist_idx, talk_angle_idx);
    attention_info.distances[fopAc_attn_TALK_e] = attention_info.distances[fopAc_attn_LOCK_e];
    attention_info.distances[fopAc_attn_SPEAK_e] =
        daNpcT_getDistTableIdx(speak_dist_idx, speak_angle_idx);
    attention_info.flags = fopAc_AttnFlag_SPEAK_e | fopAc_AttnFlag_TALK_e;

    scale.set(l_HIO.m.mScale, l_HIO.m.mScale, l_HIO.m.mScale);
    mCcStts.SetWeight(l_HIO.m.mCcWeight);
    mCylH = l_HIO.m.mCylH;
    mWallR = l_HIO.m.mWallR;
    mAttnFovY = l_HIO.m.mAttnFovY;
    if (mType == TYPE_8) {
        mAttnFovY = 135.0f;
    }
    mAcchCir.SetWallR(mWallR);
    mAcchCir.SetWallH(l_HIO.m.mWallH);
    mRealShadowSize = l_HIO.m.mShadowDepth;
    gravity = l_HIO.m.mGravity;
    mExpressionMorfFrame = l_HIO.m.field_0x6c;
    mMorfFrames = l_HIO.m.mMorfFrames;
    if (mType == TYPE_TWILIGHT || mType == TYPE_AFTER_ESCORT) {
        mAcch.SetGrndNone();
        mAcch.SetWallNone();
        gravity = 0.0f;
    }
}

BOOL daNpc_Yelia_c::checkChangeEvt() {
    if (!chkAction(&daNpc_Yelia_c::talk)) {
        mPreItemNo = 0;
        if (dComIfGp_event_chkTalkXY()) {
            if (dComIfGp_evmng_ChkPresentEnd()) {
                mPreItemNo = dComIfGp_event_getPreItemNo();
                if (mType == TYPE_WOOD_STATUE && daNpcT_chkEvtBit(0x11f)
                    && mPreItemNo == fpcNm_ITEM_IRIAS_PENDANT)
                {
                    if (mPendantTimer >= 70) {
                        dStage_changeScene(10, 0.0f, 0, fopAcM_GetRoomNo(this), 0,
                                           dComIfG_play_c::getLayerNo(0));
                    } else {
                        if (mPendantTimer == 0) {
                            mSound.startCreatureVoice(Z2SE_YELIA_V_LOOK_KIBORI, -1);
                        } else if (mPendantTimer == 8) {
                            mFaceMotionSeqMngr.setNo(FACE_KIZUKU, -1.0f, FALSE, 0);
                            mMotionSeqMngr.setNo(MOTION_KIZUKU, -1.0f, FALSE, 0);
                        }
                        mJntAnm.lookPlayer(false);
                        cLib_addCalcAngleS2(&current.angle.y, fopAcM_searchPlayerAngleY(this),
                                            4, 0x800);
                        shape_angle.y = current.angle.y;
                        mCurAngle.y = shape_angle.y;
                    }
                    mPendantTimer++;
                }
                else if (mType == TYPE_KAKARIKO && !daNpcT_chkEvtBit(0x11f)
                    && mPreItemNo == fpcNm_ITEM_WOOD_STATUE)
                {
                    mEvtNo = EVENT_TAKE_WOODSTATUE;
                    evtChange();
                } else {
                    mEvtNo = EVENT_NO_RESPONSE;
                    evtChange();
                }
            }
            return TRUE;
        }

        switch (mType) {
        case TYPE_TWILIGHT:
            if (daNpcT_chkEvtBit(0x115)) {
                mEvtNo = 3;
            } else {
                mEvtNo = 2;
            }
            evtChange();
            return TRUE;
        case TYPE_AFTER_ESCORT:
            mEvtNo = EVENT_THANK_YOU;
            evtChange();
            return TRUE;
        case TYPE_HORSE:
        case TYPE_1:
        case TYPE_REMINISCENCE:
        case TYPE_KAKARIKO:
        case TYPE_WOOD_STATUE:
        case TYPE_MEMORIES_RESTORED:
        case TYPE_8:
            break;
        }
    }

    return FALSE;
}

void daNpc_Yelia_c::setAfterTalkMotion() {
    int no = FACE_NONE;
    switch (mFaceMotionSeqMngr.getNo()) {
    case FACE_1:
        no = FACE_12;
        break;
    case FACE_KAISOU_TALK:
        no = FACE_H_KAISOU_TALK;
        break;
    case FACE_SAD:
        no = FACE_H_SAD;
        break;
    case FACE_TO_THINK:
        no = FACE_H_THINK;
        break;
    case FACE_THINK:
        no = FACE_H_THINK;
        break;
    case FACE_REMEMBER:
        no = FACE_H_INTENSE;
        break;
    case FACE_REMEMBERING:
        no = FACE_H_REMEMBERING;
        break;
    case FACE_INTENSE:
        no = FACE_H_INTENSE;
        break;
    case FACE_ASK:
        no = FACE_H_ASK;
        break;
    case FACE_HAPPY_TALK:
        no = FACE_H_HAPPY_TALK;
        break;
    case FACE_KIZUKU:
        no = FACE_H_KIZUKU;
        break;
    case FACE_LISTEN:
        no = FACE_H_LISTEN;
        break;
    }
    mFaceMotionSeqMngr.setNo(no, -1.0f, FALSE, 0);
}

void daNpc_Yelia_c::srchActors() {
    switch (mType) {
    case TYPE_KAKARIKO:
        if (mActorMngr[2].getActorP() == NULL) {
            mActorMngr[2].entry(getNearestActorP(PROC_NPC_GRD));
        }
        if (mActorMngr[3].getActorP() == NULL) {
            mActorMngr[3].entry(getNearestActorP(PROC_NPC_GRZ));
        }
        break;
    case TYPE_TWILIGHT:
        if (mActorMngr[0].getActorP() == NULL) {
            mActorMngr[0].entry(getNearestActorP(PROC_NPC_THE));
        }
        if (mActorMngr[1].getActorP() == NULL) {
            mActorMngr[1].entry(getNearestActorP(PROC_NPC_ZRC));
        }
        break;
    case TYPE_AFTER_ESCORT:
        if (mActorMngr[1].getActorP() == NULL) {
            mActorMngr[1].entry(getNearestActorP(PROC_NPC_KOLINB));
        }
        break;
    case TYPE_HORSE:
    case TYPE_1:
    case TYPE_REMINISCENCE:
    case TYPE_WOOD_STATUE:
    case TYPE_MEMORIES_RESTORED:
    case TYPE_8:
        break;
    }
}

BOOL daNpc_Yelia_c::evtTalk() {
    if (chkAction(&daNpc_Yelia_c::talk)) {
        (this->*mpAction)(NULL);
    } else {
        setAction(&daNpc_Yelia_c::talk);
    }
    return TRUE;
}

BOOL daNpc_Yelia_c::evtCutProc() {
    int staff_id = dComIfGp_getEventManager().getMyStaffId("Yelia", this, -1);
    if (staff_id != -1) {
        mStaffId = staff_id;
        int act_idx = dComIfGp_getEventManager().getMyActIdx(mStaffId, mCutNameList, 6, 0, 0);
        if ((this->*mCutList[act_idx])(mStaffId)) {
            dComIfGp_getEventManager().cutEnd(mStaffId);
        }
        return TRUE;
    } else {
        return FALSE;
    }
}

void daNpc_Yelia_c::action() {
    hitChk(&mCcCyl, -1);
    if (mStagger.checkRebirth()) {
        mStagger.initialize();
        mMode = MODE_INIT;
    }
    if (mpNextAction != NULL) {
        if (mpAction == mpNextAction) {
            (this->*mpAction)(NULL);
        } else {
            setAction(mpNextAction);
        }
    }
}

void daNpc_Yelia_c::beforeMove() {
    fopAcM_OffStatus(this, 0x8000000);
    if (checkHide()) {
        fopAcM_OnStatus(this, 0x8000000);
    }
    if (checkHide() || mNoDraw) {
        attention_info.flags = 0;
    }
}

void daNpc_Yelia_c::setAttnPos() {
    cXyz eye_offset(30.0f, -5.0f, 0.0f);
    cXyz vec(0.0f, -5.0f, 0.0f);
    mStagger.calc(FALSE);

    f32 angle_diff = cM_s2rad(mCurAngle.y - field_0xd7e.y);
    if (mType == TYPE_TWILIGHT || mType == TYPE_AFTER_ESCORT) {
        mJntAnm.setParam(this, mpMorf[0]->getModel(), &eye_offset, getBackboneJointNo(),
                         getNeckJointNo(), getHeadJointNo(), 0.0f, 0.0f, 0.0f, 0.0f,
                         l_HIO.m.mHeadDownAngle, l_HIO.m.mHeadUpAngle, l_HIO.m.mHeadRightAngle,
                         l_HIO.m.mHeadLeftAngle, l_HIO.m.mNeckAngleScl, angle_diff, &vec);
    } else {
        mJntAnm.setParam(this, mpMorf[0]->getModel(), &eye_offset, getBackboneJointNo(),
                         getNeckJointNo(), getHeadJointNo(), l_HIO.m.mBodyDownAngle,
                         l_HIO.m.mBodyUpAngle, l_HIO.m.mBodyRightAngle, l_HIO.m.mBodyLeftAngle,
                         l_HIO.m.mHeadDownAngle, l_HIO.m.mHeadUpAngle, l_HIO.m.mHeadRightAngle,
                         l_HIO.m.mHeadLeftAngle, l_HIO.m.mNeckAngleScl, angle_diff, &vec);
    }
    mJntAnm.calcJntRad(0.2f, 1.0f, angle_diff);

    setMtx();
    mDoMtx_stack_c::copy(mpMorf[0]->getModel()->getAnmMtx(getHeadJointNo()));
    mDoMtx_stack_c::multVec(&eye_offset, &eyePos);
    mJntAnm.setEyeAngleX(eyePos, 1.0f, -0x1000);
    mJntAnm.setEyeAngleY(eyePos, mCurAngle.y, TRUE, 1.0f, 0);

    if (mType == TYPE_TWILIGHT || mType == TYPE_AFTER_ESCORT) {
        vec.set(0.0f, 150.0f, 20.0f);
    } else {
        vec.set(0.0f, l_HIO.m.mAttnOffsetY, 10.0f);
    }
    mDoMtx_stack_c::YrotS(mCurAngle.y);
    mDoMtx_stack_c::multVec(&vec, &vec);
    attention_info.position = vec + current.pos;
}

void daNpc_Yelia_c::setCollision() {
    cXyz vec;

    if (!mHide) {
        u32 tg_type = 0xd8fbfdff;
        u32 tg_sprm = 0x1f;
        if (mTwilight) {
            tg_type = 0;
            tg_sprm = 0;
        } else if (mStagger.checkStagger()) {
            tg_type = 0;
            tg_sprm = 0;
        }
        mCcCyl.SetCoSPrm(0x79);
        mCcCyl.SetTgType(tg_type);
        mCcCyl.SetTgSPrm(tg_sprm);
        mCcCyl.OnTgNoHitMark();

        vec.set(0.0f, 0.0f, 0.0f);
        f32 height = mCylH;
        f32 radius = mWallR;
        mDoMtx_stack_c::YrotS(mCurAngle.y);
        mDoMtx_stack_c::multVec(&vec, &vec);
        vec += current.pos;
        mCcCyl.SetH(height);
        mCcCyl.SetR(radius);
        mCcCyl.SetC(vec);
        dComIfG_Ccsp()->Set(&mCcCyl);
    }

    mCcCyl.ClrCoHit();
    mCcCyl.ClrTgHit();
}

BOOL daNpc_Yelia_c::drawDbgInfo() {
    return FALSE;
}

BOOL daNpc_Yelia_c::selectAction() {
    mpNextAction = NULL;
    switch (mType) {
    case TYPE_HORSE:
        mpNextAction = &daNpc_Yelia_c::horseWait;
        break;
    case TYPE_REMINISCENCE:
        mpNextAction = &daNpc_Yelia_c::reminiscence;
        break;
    default:
        mpNextAction = &daNpc_Yelia_c::wait;
        break;
    }
    return TRUE;
}

BOOL daNpc_Yelia_c::chkAction(BOOL (daNpc_Yelia_c::*i_action)(void*)) {
    return mpAction == i_action;
}

BOOL daNpc_Yelia_c::setAction(BOOL (daNpc_Yelia_c::*i_action)(void*)) {
    mMode = MODE_EXIT;
    if (mpAction != NULL) {
        (this->*mpAction)(NULL);
    }

    mMode = MODE_ENTER;
    mpAction = i_action;
    if (mpAction != NULL) {
        (this->*mpAction)(NULL);
    }

    return TRUE;
}

BOOL daNpc_Yelia_c::cutConversationAboutLoopHole(int i_staffId) {
    BOOL ret = FALSE;
    int prm = -1;
    int* prm_p = dComIfGp_evmng_getMyIntegerP(i_staffId, "prm");
    if (prm_p != NULL) {
        prm = *prm_p;
    }
    dComIfGp_evmng_getMyIntegerP(i_staffId, "msgNo");

    if (dComIfGp_getEventManager().getIsAddvance(i_staffId)) {
        switch (prm) {
        case 0:
            break;

        case 1:
            mFaceMotionSeqMngr.setNo(FACE_HORSE_LOOKBACK, -1.0f, FALSE, 0);
            mMotionSeqMngr.setNo(MOTION_HORSE_LOOKBACK, -1.0f, FALSE, 0);
            break;

        case 2:
            mFaceMotionSeqMngr.setNo(FACE_HORSE_RETURN, -1.0f, FALSE, 0);
            mMotionSeqMngr.setNo(MOTION_HORSE_RETURN, -1.0f, FALSE, 0);
            break;
        }
    }

    switch (prm) {
    case 0:
        ret = TRUE;
        break;

    case 1:
        if (mMotionSeqMngr.getNo() == MOTION_HORSE_LOOKBACK) {
            if (mMotionSeqMngr.checkEndSequence()) {
                mFaceMotionSeqMngr.setNo(FACE_NONE, -1.0f, FALSE, 0);
                mMotionSeqMngr.setNo(MOTION_HORSE_WAIT_C, -1.0f, FALSE, 0);
            }
        } else {
            ret = TRUE;
        }
        break;

    case 2:
        if (mMotionSeqMngr.getNo() == MOTION_HORSE_RETURN) {
            if (mMotionSeqMngr.checkEndSequence()) {
                mFaceMotionSeqMngr.setNo(FACE_NONE, -1.0f, FALSE, 0);
                mMotionSeqMngr.setNo(MOTION_HORSE_WAIT_A, -1.0f, FALSE, 0);
            }
        } else {
            ret = TRUE;
        }
        break;
    }

    return ret;
}

BOOL daNpc_Yelia_c::cutTWResistance(int i_staffId) {
    BOOL ret = FALSE;
    int prm = -1;
    int* prm_p = dComIfGp_evmng_getMyIntegerP(i_staffId, "prm");
    if (prm_p != NULL) {
        prm = *prm_p;
    }

    fopAc_ac_c* actors[2] = {this, mActorMngr[0].getActorP()};
    dComIfGp_setMesgCameraInfoActor(actors[0], actors[1], NULL, NULL, NULL, NULL,
                                    NULL, NULL, NULL, NULL);
    
    if (dComIfGp_getEventManager().getIsAddvance(i_staffId)) {
        switch (prm) {
        case 0:
            break;

        case 1:
            initTalk(mFlowNodeNo, NULL);
            break;
        }
    }

    switch (prm) {
    case 0:
        ret = TRUE;
        break;

    case 1:
        if (talkProc(NULL, TRUE, actors, FALSE) && mFlow.checkEndFlow()) {
            ret = TRUE;
        }
        break;
    }

    return ret;
}

BOOL daNpc_Yelia_c::cutTakeWoodStatue(int i_staffId) {
    daDitem_c* item = NULL;
    cXyz pos;
    csXyz angle;
    char name[0x68];
    BOOL ret = FALSE;
    int prm = -1;
    int timer = 0;
    int msgNo = 0;
    int send = 0;
    int* prm_p = dComIfGp_evmng_getMyIntegerP(i_staffId, "prm");
    if (prm_p != NULL) {
        prm = *prm_p;
    }
    int* timer_p = dComIfGp_evmng_getMyIntegerP(i_staffId, "timer");
    if (timer_p != NULL) {
        timer = *timer_p;
    }
    int* msgNo_p = dComIfGp_evmng_getMyIntegerP(i_staffId, "msgNo");
    if (msgNo_p != NULL) {
        msgNo = *msgNo_p;
    }
    int* send_p = dComIfGp_evmng_getMyIntegerP(i_staffId, "send");
    if (send_p != NULL) {
        send = *send_p;
    }

    fopAc_ac_c* actors[2] = {this, mActorMngr[2].getActorP()};
    dComIfGp_setMesgCameraInfoActor(actors[0], actors[1], NULL, NULL, NULL, NULL,
                                    NULL, NULL, NULL, NULL);
    
    if (dComIfGp_getEventManager().getIsAddvance(i_staffId)) {
        switch (prm) {
        case 0:
            mFaceMotionSeqMngr.setNo(FACE_H_INTENSE, 0.0f, TRUE, 0);
            mMotionSeqMngr.setNo(MOTION_LOOKING, 0.0f, TRUE, 0);
            mSound.startCreatureVoice(Z2SE_YELIA_V_LOOK_KIBORI, -1);
            daNpcT_offTmpBit(0xb);
            daNpcT_offTmpBit(0xc);
            dComIfGs_onSaveSwitch(6, 0x40);
            if (daNpcT_getPlayerInfoFromPlayerList(10, fopAcM_GetRoomNo(this), &pos, &angle)) {
                daPy_getPlayerActorClass()->setPlayerPosAndAngle(&pos, angle.y, 0);
                dComIfGp_evmng_setGoal(&pos);
            }
            if (fopAcM_SearchByID(mItemId, (fopAc_ac_c**)&item)
                                    && item != NULL && fopAc_IsActor(item)) {
                item->scale.setall(0.55f);
                item->show();
            }
            break;

        case 1:
        case 8:
            initTalk(0x26, NULL);
            break;

        case 4:
            mFaceMotionSeqMngr.setNo(FACE_NONE, 0.0f, TRUE, 0);
            mMotionSeqMngr.setNo(MOTION_ASKING, 0.0f, TRUE, 0);
            break;

        case 5:
            if (mFaceMotionSeqMngr.getNo() != FACE_H_ASK) {
                mFaceMotionSeqMngr.setNo(FACE_H_ASK, -1.0f, TRUE, 0);
            }
            if (mMotionSeqMngr.getNo() != MOTION_TO_WORRY) {
                mMotionSeqMngr.setNo(MOTION_TO_WORRY, -1.0f, TRUE, 0);
            }
            break;

        case 7:
            mEventTimer = timer;
            break;

        case 9:
            mFaceMotionSeqMngr.setNo(FACE_H_ASK, 0.0f, TRUE, 0);
            mMotionSeqMngr.setNo(MOTION_ASKING, 0.0f, TRUE, 0);
            mEventTimer = timer;
            break;

        case 11:
        case 99:
            mFaceMotionSeqMngr.setNo(FACE_H_INTENSE, 0.0f, TRUE, 0);
            mMotionSeqMngr.setNo(MOTION_ASKING, 0.0f, TRUE, 0);
            pos = daPy_getPlayerActorClass()->current.pos;
            daPy_getPlayerActorClass()->setPlayerPosAndAngle(&pos, 0, 0);
            dComIfGp_evmng_setGoal(&pos);
            Z2GetAudioMgr()->unMuteSceneBgm(60);
            if (prm == 99) {
                daNpcT_onEvtBit(0x11f);
                daNpcT_onEvtBit(0x17a);
                dComIfGs_setWarashibeItem(0xff);
                daNpcT_offTmpBit(0xb);
                daNpcT_offTmpBit(0xc);
                dComIfGs_onSaveSwitch(6, 0x40);
                dComIfGs_onSaveSwitch(6, 0x1c);
                mFlow.remove();
            }
            break;

        case 13:
            mItemId = fpcM_ERROR_PROCESS_ID_e;
            mEventTimer = timer;
            Z2GetAudioMgr()->muteSceneBgm(90, 0.0f);
            strcpy(name, l_evtList[EVENT_TAKE_WOODSTATUE].eventName);
            strcat(name, "@");
            dComIfGp_getEvent().setSkipZev(this, name);
            dComIfGp_getEvent().onSkipFade();
            break;
        }

        if ((prm == 6 || prm == 99) && mItemId != fpcM_ERROR_PROCESS_ID_e
                                    && fopAcM_IsExecuting(mItemId)
                                    && fopAcM_SearchByID(mItemId, (fopAc_ac_c**)&item)
                                    && item != NULL && fopAc_IsActor(item)) {
            fopAcM_delete(item);
        }
    }

    int msgs[2] = {-1, -1};

    switch (prm) {
    case 0:
        ret = TRUE;
        break;

    case 1:
    case 2:
    case 4:
    case 6:
        msgs[0] = msgNo;
        if (talkProc(msgs, send, actors, FALSE)) {
            if (msgNo == 0) {
                if (mFlow.checkEndFlow()) {
                    ret = TRUE;
                }
            } else {
                ret = TRUE;
            }
        }
        if (prm == 6 && !send) {
            ret = TRUE;
        }
        break;

    case 3:
    case 5:
    case 8:
        if (talkProc(NULL, send, actors, FALSE) && mFlow.checkEndFlow()) {
            ret = TRUE;
        }
        break;

    case 7:
    case 9:
    case 13:
        if (cLib_calcTimer(&mEventTimer) == 0) {
            if (prm == 13) {
                if (mItemId == fpcM_ERROR_PROCESS_ID_e) {
                    mItemId = fopAcM_createItemForPresentDemo(&current.pos,
                        fpcNm_ITEM_WOOD_STATUE, 9, -1, fopAcM_GetRoomNo(this), &mCurAngle, &scale);
                }
                if (fopAcM_SearchByID(mItemId, (fopAc_ac_c**)&item)
                                        && item != NULL && fopAc_IsActor(item)) {
                    pos.set(0.0f, 110.0f, 30.0f);
                    mDoMtx_stack_c::YrotS(mCurAngle.y);
                    mDoMtx_stack_c::multVec(&pos, &pos);
                    pos += current.pos;
                    item->setOffsetPos(pos);
                    item->setMaxScale(0.55f);
                    item->scale.setall(0.0f);
                    item->hide();
                    ret = TRUE;
                }
            } else {
                ret = TRUE;
            }
        }
        break;

    case 10:
    case 11:
    case 99:
        ret = TRUE;
        break;

    case 12:
        ret = TRUE;
        break;
    }

    return ret;
}

BOOL daNpc_Yelia_c::cutClothTry(int i_staffId) {
    BOOL ret = 0;
    int prm = -1;
    int* prm_p = dComIfGp_evmng_getMyIntegerP(i_staffId, "prm");
    if (prm_p != NULL) {
        prm = *prm_p;
    }

    dComIfGp_getEventManager().getIsAddvance(i_staffId);

    switch (prm) {
    case 0:
        action();
        ret = TRUE;
        break;
    }

    return ret;
}

BOOL daNpc_Yelia_c::cutThankYou(int i_staffId) {
    BOOL ret = 0;
    int prm = -1;
    int msgNo = 0;
    int msgNo2 = 0;
    int* prm_p = dComIfGp_evmng_getMyIntegerP(i_staffId, "prm");
    if (prm_p != NULL) {
        prm = *prm_p;
    }
    int* msgNo_p = dComIfGp_evmng_getMyIntegerP(i_staffId, "msgNo");
    if (msgNo_p != NULL) {
        msgNo = *msgNo_p;
    }
    int* msgNo2_p = dComIfGp_evmng_getMyIntegerP(i_staffId, "msgNo2");
    if (msgNo2_p != NULL) {
        msgNo2 = *msgNo2_p;
    }

    if (dComIfGp_getEventManager().getIsAddvance(i_staffId)) {
        switch (prm) {
        case 0:
            break;

        case 1:
            mJntAnm.lookPlayer(false);
            initTalk(mFlowNodeNo, NULL);
            break;

        case 2:
            break;
        }
    }

    int msgs[3] = {-1, -1, -1};
    switch (prm) {
    case 0:
        ret = TRUE;
        break;

    case 1:
        msgs[0] = msgNo;
        msgs[1] = msgNo2;
        if (talkProc(msgs, FALSE, NULL, FALSE)) {
            if (msgNo == 0 || msgNo2 == 0) {
                if (mFlow.checkEndFlow()) {
                    ret = TRUE;
                }
            } else {
                ret = TRUE;
            }
        }
        break;

    case 2:
        if (talkProc(NULL, FALSE, NULL, FALSE) && mFlow.checkEndFlow()) {
            ret = TRUE;
        }
        break;
    }

    return ret;
}

BOOL daNpc_Yelia_c::wait(void* param_0) {
    switch (mMode) {
    case MODE_ENTER:
    case MODE_INIT:
        if (!mStagger.checkStagger()) {
            switch (mType) {
            case TYPE_HORSE:
            case TYPE_1:
            case TYPE_REMINISCENCE:
                mFaceMotionSeqMngr.setNo(FACE_NONE, -1.0f, FALSE, 0);
                mMotionSeqMngr.setNo(MOTION_WAIT_A, -1.0f, FALSE, 0);
                break;

            case TYPE_KAKARIKO:
                if (daNpcT_chkEvtBit(0x11f)) {
                    mFaceMotionSeqMngr.setNo(FACE_H_INTENSE, -1.0f, FALSE, 0);
                    mMotionSeqMngr.setNo(MOTION_ASKING, -1.0f, FALSE, 0);
                } else {
                    mFaceMotionSeqMngr.setNo(FACE_NONE, -1.0f, FALSE, 0);
                    mMotionSeqMngr.setNo(MOTION_WAIT_UTUMUKU, -1.0f, FALSE, 0);
                }
                break;

            case TYPE_TWILIGHT:
                mFaceMotionSeqMngr.setNo(FACE_H_SAD, -1.0f, FALSE, 0);
                mMotionSeqMngr.setNo(MOTION_DEMO_WAIT_SIT, -1.0f, FALSE, 0);
                break;

            case TYPE_AFTER_ESCORT:
                mFaceMotionSeqMngr.setNo(FACE_NONE, -1.0f, FALSE, 0);
                mMotionSeqMngr.setNo(MOTION_DEMO_WAIT_SIT, -1.0f, FALSE, 0);
                break;

            case TYPE_WOOD_STATUE:
                mFaceMotionSeqMngr.setNo(FACE_H_INTENSE, -1.0f, FALSE, 0);
                mMotionSeqMngr.setNo(MOTION_ASKING, -1.0f, FALSE, 0);
                break;

            case TYPE_MEMORIES_RESTORED:
                mFaceMotionSeqMngr.setNo(FACE_NONE, -1.0f, FALSE, 0);
                mMotionSeqMngr.setNo(MOTION_WAIT_A, -1.0f, FALSE, 0);
                break;

            default:
                mFaceMotionSeqMngr.setNo(FACE_NONE, -1.0f, FALSE, 0);
                mMotionSeqMngr.setNo(MOTION_WAIT_A, -1.0f, FALSE, 0);
                break;
            }

            mMode = MODE_RUN;
        }
        // fallthrough

    case MODE_RUN:
        if (mType == TYPE_MEMORIES_RESTORED && !daNpcT_chkEvtBit(0x30c)) {
            mSpeakEvent = true;
            field_0xe33 = true;
        }

        if (!mStagger.checkStagger()) {
            if (mType == TYPE_TWILIGHT) {
                fopAc_ac_c* ralis = mActorMngr[1].getActorP();
                if (ralis != NULL) {
                    mJntAnm.lookPos(&ralis->eyePos, false);
                } else {
                    mJntAnm.lookNone(false);
                }
            } else {
                if ((mType == TYPE_KAKARIKO && !daNpcT_chkEvtBit(0x11f))
                                            || mType == TYPE_AFTER_ESCORT) {
                    mPlayerActorMngr.remove();
                }
                if (mPlayerActorMngr.getActorP() != NULL && !mTwilight) {
                    mJntAnm.lookPlayer(false);
                    if (!chkActorInSight(mPlayerActorMngr.getActorP(), mAttnFovY, mCurAngle.y)) {
                        mJntAnm.lookNone(false);
                    }
                    if (!srchPlayerActor() && home.angle.y == mCurAngle.y) {
                        mMode = MODE_INIT;
                    }
                } else {
                    mJntAnm.lookNone(false);
                    if (mType != TYPE_AFTER_ESCORT) {
                        if (home.angle.y != mCurAngle.y) {
                            if (field_0xe34) {
                                if ((mType == TYPE_KAKARIKO && daNpcT_chkEvtBit(0x11f))
                                                            || mType == TYPE_WOOD_STATUE) {
                                    if (step(home.angle.y, FACE_H_ASK, MOTION_ASK_STEP, 15, 0)) {
                                        mMode = MODE_INIT;
                                    }
                                } else if (step(home.angle.y, FACE_NONE, MOTION_STEP, 15, 0)) {
                                    mMode = MODE_INIT;
                                }
                            } else {
                                setAngle(home.angle.y);
                                mMode = MODE_INIT;
                            }
                            attention_info.flags = 0;
                        } else {
                            srchPlayerActor();
                        }
                    }
                }
            }

            switch (mJntAnm.getMode()) {
            case daNpcT_JntAnm_c::LOOK_NONE:
                switch (mType) {
                case TYPE_AFTER_ESCORT:
                    fopAc_ac_c* colin = mActorMngr[1].getActorP();
                    if (colin != NULL) {
                        mJntAnm.lookPos(&colin->eyePos, false);
                    }
                    break;
                }
                break;
            }
        }
        break;

    case MODE_EXIT:
        break;
    }

    return TRUE;
}

BOOL daNpc_Yelia_c::horseWait(void* param_0) {
    switch (mMode) {
    case MODE_ENTER:
    case MODE_INIT:
        mMode = MODE_RUN;
        // fallthrough

    case MODE_RUN:
        if (!mStagger.checkStagger()) {
            switch (mMotionSeqMngr.getNo()) {
            case MOTION_HORSE_RETURN:
                if (mMotionSeqMngr.checkEndSequence()) {
                    mFaceMotionSeqMngr.setNo(FACE_NONE, -1.0f, FALSE, 0);
                    mMotionSeqMngr.setNo(MOTION_HORSE_WAIT_A, -1.0f, FALSE, 0);
                }
                break;
            case MOTION_HORSE_WAIT_C:
                mFaceMotionSeqMngr.setNo(FACE_HORSE_RETURN, -1.0f, FALSE, 0);
                mMotionSeqMngr.setNo(MOTION_HORSE_RETURN, -1.0f, FALSE, 0);
                break;
            default:
                if (mMotionSeqMngr.getNo() != MOTION_HORSE_WAIT_A) {
                    mFaceMotionSeqMngr.setNo(FACE_NONE, -1.0f, FALSE, 0);
                    mMotionSeqMngr.setNo(MOTION_HORSE_WAIT_A, -1.0f, FALSE, 0);
                }
                break;
            }
            mJntAnm.lookNone(false);
            attention_info.flags = 0;
        }
        break;

    case MODE_EXIT:
        break;
    }

    return TRUE;
}

BOOL daNpc_Yelia_c::reminiscence(void* param_0) {
    switch (mMode) {
    case MODE_ENTER:
    case MODE_INIT:
        mFaceMotionSeqMngr.setNo(FACE_KAISOU_WAIT_A, -1.0f, FALSE, 0);
        mMotionSeqMngr.setNo(MOTION_KAISOU_WAIT_A, -1.0f, FALSE, 0);
        mMode = MODE_RUN;
        // fallthrough

    case MODE_RUN:
        field_0xe2d = false;
        if (dComIfGp_event_runCheck() != FALSE
            && !strcmp("SMELL_1st", dComIfGp_getEventManager().getRunEventName()))
        {
            dComIfGp_event_setTalkPartner(this);
            field_0xe2d = true;
        }
        if (!mStagger.checkStagger()) {
            mJntAnm.lookNone(false);
        }
        break;

    case MODE_EXIT:
        break;
    }

    return TRUE;
}

BOOL daNpc_Yelia_c::talk(void* param_0) {
    switch (mMode) {
    case MODE_ENTER:
    case MODE_INIT:
        if (!mStagger.checkStagger()) {
            if (&daNpc_Yelia_c::horseWait == mpNextAction) {
                mFaceMotionSeqMngr.setNo(FACE_HORSE_LOOKBACK, -1.0f, FALSE, 0);
                mMotionSeqMngr.setNo(MOTION_HORSE_LOOKBACK, -1.0f, FALSE, 0);
            }
            initTalk(mFlowNodeNo, NULL);
            mMode = 2;
        }
        // fallthrough

    case MODE_RUN:
        if (!mStagger.checkStagger()) {
            if (&daNpc_Yelia_c::horseWait == mpNextAction && mMotionSeqMngr.getNo() == MOTION_HORSE_LOOKBACK) {
                if (mMotionSeqMngr.checkEndSequence()) {
                    mFaceMotionSeqMngr.setNo(FACE_NONE, -1.0f, FALSE, 0);
                    mMotionSeqMngr.setNo(MOTION_HORSE_WAIT_C, -1.0f, FALSE, 0);
                }
                return FALSE;
            } else if (mTwilight || mPlayerAngle == mCurAngle.y || &daNpc_Yelia_c::horseWait == mpNextAction
                || (mType == TYPE_KAKARIKO && !daNpcT_chkEvtBit(0x11f))
                || mType == TYPE_AFTER_ESCORT)
            {
                if (talkProc(NULL, FALSE, NULL, FALSE) && mFlow.checkEndFlow()) {
                    mPlayerActorMngr.entry(daPy_getPlayerActorClass());
                    dComIfGp_event_reset();
                    mMode = MODE_EXIT;
                }
                mJntAnm.lookPlayer(false);
                if (mTwilight || &daNpc_Yelia_c::horseWait == mpNextAction) {
                    mJntAnm.lookNone(false);
                }
            } else {
                mJntAnm.lookPlayer(false);
                if ((mType == TYPE_KAKARIKO && daNpcT_chkEvtBit(0x11f))
                    || mType == TYPE_WOOD_STATUE)
                {
                    step(mPlayerAngle, FACE_H_ASK, MOTION_ASK_STEP, 15, 0);
                } else {
                    step(mPlayerAngle, FACE_NONE, MOTION_STEP, 15, 0);
                }
            }
        }
        break;

    case MODE_EXIT:
        break;
    }

    return FALSE;
}

static cPhs__Step daNpc_Yelia_Create(void* i_this) {
    return static_cast<daNpc_Yelia_c*>(i_this)->create();
}

static int daNpc_Yelia_Delete(void* i_this) {
    return static_cast<daNpc_Yelia_c*>(i_this)->Delete();
}

static int daNpc_Yelia_Execute(void* i_this) {
    return static_cast<daNpc_Yelia_c*>(i_this)->Execute();
}

static int daNpc_Yelia_Draw(void* i_this) {
    return static_cast<daNpc_Yelia_c*>(i_this)->Draw();
}

static int daNpc_Yelia_IsDelete(void* i_this) {
    return 1;
}

static actor_method_class daNpc_Yelia_MethodTable = {
    (process_method_func)daNpc_Yelia_Create,
    (process_method_func)daNpc_Yelia_Delete,
    (process_method_func)daNpc_Yelia_Execute,
    (process_method_func)daNpc_Yelia_IsDelete,
    (process_method_func)daNpc_Yelia_Draw,
};

extern actor_process_profile_definition g_profile_NPC_YELIA = {
  fpcLy_CURRENT_e,          // mLayerID
  7,                        // mListID
  fpcPi_CURRENT_e,          // mListPrio
  PROC_NPC_YELIA,           // mProcName
  &g_fpcLf_Method.base,    // sub_method
  sizeof(daNpc_Yelia_c),    // mSize
  0,                        // mSizeOther
  0,                        // mParameters
  &g_fopAc_Method.base,     // sub_method
  379,                      // mPriority
  &daNpc_Yelia_MethodTable, // sub_method
  0x00040108,               // mStatus
  fopAc_NPC_e,              // mActorType
  fopAc_CULLBOX_CUSTOM_e,   // cullType
};
