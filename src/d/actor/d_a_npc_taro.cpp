/**
 * @file d_a_npc_taro.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_npc_taro.h"
#include "JSystem/JUtility/JUTAssert.h"
#include "d/actor/d_a_npc_len.h"
#include "d/actor/d_a_npc_maro.h"
#include "Z2AudioLib/Z2Instances.h"
#include "d/actor/d_a_tag_evtarea.h"
#include "d/actor/d_a_tag_push.h"
#include "d/actor/d_a_horse.h"
#include "d/d_com_static.h"
#include "d/d_meter2_info.h"
#include "d/d_msg_object.h"
#include "f_op/f_op_actor.h"
#include "f_op/f_op_camera_mng.h"
#include "types.h"

daNpc_Maro_c::actionFunc dummy_lit_3931() {
    return &daNpc_Maro_c::choccai;
}

enum TaroEvent {
    /* 0x8 */ EVENT_8 = 0x8,
    /* 0x17 */ EVENT_17 = 0x17,
    /* 0x18 */ EVENT_18 = 0x18,
};

static int l_bmdData[5][2] = {
    4, 1, 31, 4, 32, 4, 10, 2, 4, 9,
};

static daNpcT_evtData_c l_evtList[27] = {
    {"", 0},
    {"NO_RESPONSE", 0},
    {"HAIL", 4},
    {"FIND_MONKEY", 4},
    {"HELP_ME", 8},
    {"THANX_FOR_YOURHELP", 8},
    {"APPEARANCE_MOI", 8},
    {"GIVEME_WOODSWD1", 4},
    {"GIVEME_WOODSWD2", 4},
    {"GET_WOODSWD", 4},
    {"CONVERSATION_WITH_MARO1", 4},
    {"CONVERSATION_WITH_MARO2", 4},
    {"DEMO13_STB", 0},
    {"BEGIN_ARROWTUTORIAL", 5},
    {"BEGIN_ARROWTUTORIAL_NO_ARROW", 5},
    {"RETRY_ARROWTUTORIAL1", 5},
    {"RETRY_ARROWTUTORIAL1_NO_ARROW", 5},
    {"RETRY_ARROWTUTORIAL2", 5},
    {"RETRY_ARROWTUTORIAL2_NO_ARROW", 5},
    {"RETRY_ARROWTUTORIAL3", 5},
    {"RETRY_ARROWTUTORIAL3_NO_ARROW", 5},
    {"CAUTION", 5},
    {"TAG_PUSH1", 4},
    {"NOT_GONNA_LET_1", 4},
    {"NOT_GONNA_LET_2", 4},
    {"NOT_GONNA_LET_3", 4},
    {"TAG_PUSH4", 4},
};

static char* l_resNameList[11] = {
    "", "Taro", "Taro_TW", "Taro0", "Taro1", "Taro2", "Taro3", "Taro4", "Taro5", "TaroB", "Len1",
};

static s8 l_loadResPtrn0[5] = {
    0x01, 0x03, 0x04, 0x06, -1,
};

static s8 l_loadResPtrn1[4] = {0x01, 0x03, 0x02, -1};

static s8 l_loadResPtrn2[6] = {
    0x01, 0x03, 0x05, 0x06, 0x07, -1,
};

static s8 l_loadResPtrn3[5] = {
    0x01, 0x03, 0x04, 0x07, -1,
};

static s8 l_loadResPtrn4[4] = {0x01, 0x03, 0x08, -1};

static s8 l_loadResPtrn5[5] = {
    0x09, 0x03, 0x04, 0x08, -1,
};

static s8 l_loadResPtrn6[6] = {
    0x01, 0x03, 0x04, 0x05, 0x06, -1,
};

static s8 l_loadResPtrn9[9] = {
    0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, -1,
};

static s8* l_loadResPtrnList[17] = {
    l_loadResPtrn6, l_loadResPtrn6, l_loadResPtrn0, l_loadResPtrn1,
    l_loadResPtrn0, l_loadResPtrn0, l_loadResPtrn3, l_loadResPtrn6,
    l_loadResPtrn2, l_loadResPtrn0, l_loadResPtrn6, l_loadResPtrn6,
    l_loadResPtrn6, l_loadResPtrn4, l_loadResPtrn5, l_loadResPtrn6,
    l_loadResPtrn9,
};

static daNpcT_faceMotionAnmData_c l_faceMotionAnmData[30] = {
    {-1, 0, 0, 7, 2, 1, 1},
    {7, 0, 3, 7, 2, 1, 1},
    {11, 0, 4, 43, 0, 4, 0},
    {5, 0, 6, 14, 0, 6, 0},
    {13, 0, 4, 45, 0, 4, 0},
    {9, 0, 4, 41, 0, 4, 0},
    {4, 0, 6, 13, 0, 6, 0},
    {12, 0, 4, 44, 0, 4, 0},
    {15, 2, 4, 47, 2, 4, 0},
    {7, 2, 6, 15, 2, 6, 0},
    {16, 2, 4, 48, 2, 4, 0},
    {14, 2, 4, 46, 2, 4, 0},
    {6, 2, 6, 7, 2, 1, 1},
    {6, 0, 3, 15, 0, 3, 0},
    {10, 0, 4, 42, 0, 4, 0},
    {7, 0, 5, 27, 0, 5, 0},
    {12, 0, 5, 32, 0, 5, 0},
    {11, 0, 5, 31, 0, 5, 0},
    {9, 0, 5, 29, 0, 5, 0},
    {10, 0, 5, 30, 0, 5, 0},
    {8, 0, 5, 28, 0, 5, 0},
    {4, 0, 7, 10, 0, 7, 0},
    {5, 2, 7, 11, 2, 7, 0},
    {7, 2, 2, 16, 2, 2, 0},
    {10, 0, 8, 28, 0, 8, 0},
    {12, 2, 8, 30, 2, 8, 0},
    {8, 0, 8, 26, 0, 8, 0},
    {11, 2, 8, 29, 2, 8, 0},
    {9, 0, 8, 27, 0, 8, 0},
    {7, 2, 8, 25, 2, 8, 0},
};

static daNpcT_motionAnmData_c l_motionAnmData[42] = {
    9,  2, 3, 12, 0, 3, 1, 0, 25, 2, 4, 12, 0, 3, 1, 0, 10, 2, 6, 12, 0, 3, 1, 0,
    26, 2, 4, 12, 0, 3, 1, 0, 27, 2, 4, 12, 0, 3, 1, 0, 28, 2, 4, 12, 0, 3, 1, 0,
    8,  0, 6, 12, 0, 3, 1, 0, -1, 0, 0, -1, 0, 0, 0, 0, 17, 2, 4, 12, 0, 3, 1, 0,
    16, 2, 8, 12, 0, 3, 1, 0, 21, 2, 4, 12, 0, 3, 1, 0, 23, 0, 4, 12, 0, 3, 1, 0,
    24, 2, 4, 12, 0, 3, 1, 0, 9,  0, 6, 12, 0, 3, 1, 0, 7,  0, 4, 12, 0, 3, 1, 0,
    8,  0, 4, 12, 0, 3, 1, 0, 5,  0, 3, 12, 0, 3, 1, 0, 20, 0, 4, 35, 0, 4, 0, 0,
    22, 2, 4, 12, 0, 3, 1, 0, 18, 2, 4, 12, 0, 3, 1, 0, 19, 2, 4, 12, 0, 3, 1, 0,
    6,  0, 5, 12, 0, 3, 1, 0, 21, 0, 5, 12, 0, 3, 1, 0, 19, 0, 5, 12, 0, 3, 1, 0,
    20, 2, 5, 12, 0, 3, 1, 0, 16, 0, 5, 12, 0, 3, 1, 0, 17, 0, 5, 12, 0, 3, 1, 0,
    15, 0, 5, 24, 0, 5, 0, 0, 13, 0, 5, 12, 0, 3, 1, 0, 14, 2, 5, 12, 0, 3, 1, 0,
    18, 0, 5, 12, 0, 3, 1, 0, 6,  2, 7, 12, 0, 3, 1, 0, 7,  0, 7, 12, 0, 3, 1, 0,
    6,  2, 2, 13, 2, 2, 0, 0, 8,  0, 3, 12, 0, 3, 1, 0, 19, 2, 8, 12, 0, 3, 1, 0,
    15, 2, 8, 12, 0, 3, 1, 0, 13, 0, 8, 12, 0, 3, 1, 0, 14, 2, 8, 12, 0, 3, 1, 0,
    6,  2, 8, 22, 2, 8, 0, 0, 17, 0, 8, 12, 0, 3, 1, 0, 18, 2, 8, 12, 0, 3, 1, 0,
};

static daNpcT_MotionSeqMngr_c::sequenceStepData_c l_faceMotionSequenceData[128] = {
    {1, -1, 1}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {2, -1, 1}, {8, -1, 0}, {-1, 0, 0}, {-1, 0, 0},
    {3, -1, 1}, {9, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {4, -1, 1}, {10, -1, 0}, {-1, 0, 0}, {-1, 0, 0},
    {5, -1, 1}, {11, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {6, -1, 1}, {12, -1, 0}, {-1, 0, 0}, {-1, 0, 0},
    {15, -1, 1}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {16, -1, 1}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {18, -1, 1}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {14, -1, 1}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {19, -1, 1}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {20, -1, 1}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {21, -1, 1}, {22, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {24, -1, 1}, {25, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {26, -1, 1}, {27, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {3, -1, 1}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {7, -1, 1}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {13, -1, 1}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {8, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {9, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {10, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {11, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {12, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {17, -1, 1}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {22, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {23, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {25, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {27, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {28, -1, 1}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {24, -1, 1}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {29, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {0, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
};

static daNpcT_MotionSeqMngr_c::sequenceStepData_c l_motionSequenceData[168] = {
    {0, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {21, -1, 1}, {0, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {22, -1, 1}, {0, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {1, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {17, -1, 1}, {0, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {24, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {25, -1, 1}, {0, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {26, -1, 1}, {0, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {2, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {6, 0, 1}, {2, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {27, -1, 1}, {0, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {3, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {28, -1, 1}, {29, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {11, -1, 1}, {0, -1, 0}, {-1, 0, 0}, {-1, 0, 0},
    {12, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {13, -1, 1}, {0, -1, 0}, {-1, 0, 0}, {-1, 0, 0},
    {29, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {30, 10, 1}, {2, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {32, -1, 1}, {31, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {31, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {5, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {35, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {36, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {37, -1, 1}, {36, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {38, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {18, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {39, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {14, -1, 1}, {0, -1, 0}, {-1, 0, 0}, {-1, 0, 0},
    {15, -1, 1}, {0, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {19, -1, 1}, {18, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {20, -1, 1}, {18, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {16, -1, 1}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {23, -1, 1}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {33, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {34, 4, 1}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {8, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {10, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {9, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {40, -1, 1}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {40, -1, 1}, {41, -1, 0}, {-1, 0, 0}, {-1, 0, 0},
};

char* daNpc_Taro_c::mCutNameList[17] = {
    "",
    "CONVERSATION_ABOUT_PACHINKO",
    "CONVERSATION_ABOUT_WOODSWD",
    "SWDTUTORIAL",
    "HAIL",
    "FIND_MONKEY",
    "HELP_ME",
    "APPEARANCE_MOI",
    "GIVEME_WOODSWD",
    "GET_WOODSWD",
    "CONVERSATION_WITH_MARO",
    "CACARICO_CONVERSATION",
    "ARROWTUTORIAL",
    "CAUTION",
    "TAG_PUSH1",
    "NOT_GONNA_LET",
    "TAG_PUSH4",
};

daNpc_Taro_c::cutFunc daNpc_Taro_c::mCutList[17] = {
    NULL,
    &daNpc_Taro_c::cutConversationAboutPachinko,
    &daNpc_Taro_c::cutConversationAboutWoodSwd,
    &daNpc_Taro_c::cutSwdTutorial,
    &daNpc_Taro_c::cutHail,
    &daNpc_Taro_c::cutFindMonkey,
    &daNpc_Taro_c::cutHelpMe,
    &daNpc_Taro_c::cutAppearanceMoi,
    &daNpc_Taro_c::cutGiveMeWoodSwd,
    &daNpc_Taro_c::cutGetWoodSwd,
    &daNpc_Taro_c::cutConversationWithMaro,
    &daNpc_Taro_c::cutCacaricoConversation,
    &daNpc_Taro_c::cutArrowTutorial,
    &daNpc_Taro_c::cutCaution,
    &daNpc_Taro_c::cutTagPush1,
    &daNpc_Taro_c::cutNotGonnaLet,
    &daNpc_Taro_c::cutTagPush4,
};

static daNpc_Taro_Param_c l_HIO;

daNpc_Taro_c::~daNpc_Taro_c() {
    OS_REPORT("|%06d:%x|daNpc_Taro_c -> デストラクト\n", g_Counter.mCounter0, this);
    if (mpMorf[0] != NULL) {
        mpMorf[0]->stopZelAnime();
    }

    #if DEBUG
    if (field_0xe40 != NULL) {
        field_0xe40->removeHIO();
    }
    #endif

    deleteRes((l_loadResPtrnList)[mType], (const char**)l_resNameList);
}

daNpc_Taro_HIOParam const daNpc_Taro_Param_c::m = {
    140.0f, -3.0f,  1.0f,   400.0f, 255.0f, 120.0f, 35.0f, 30.0f, 0.0f, 0.0f,
    10.0f,  -10.0f, 30.0f,  -10.0f, 45.0f,  -45.0f, 0.6f, 12.0f, 3,    6,
    5,      6,      110.0f, 0.0f,   0.0f,   0.0f,   60,    8,     0.0f, 0.0f,
    4.0f,   -15.0f, 0.0f,   -10.0f, 15.0f,  30.0f,  10.0f, 55.0f, 120,  90};

int daNpc_Taro_c::create() {
    daNpcT_ct(this, daNpc_Taro_c, l_faceMotionAnmData,
                       l_motionAnmData, l_faceMotionSequenceData,
                       4, l_motionSequenceData,
                       4, l_evtList, l_resNameList);

    mType = getType();
    mFlowNodeNo = getFlowNodeNo();
    mTwilight = dKy_darkworld_check();
    if (mType == 9) {
        mTwilight = false;
    }
    int rv = loadRes(l_loadResPtrnList[mType], (const char**)l_resNameList);
    if (rv == cPhs_COMPLEATE_e) {
        OS_REPORT("\t(%s:%d) flowNo:%d, PathID:%02x, BitSW:%02x, BitSW2:%02x<%08x> ",
                  fopAcM_getProcNameString(this), mType, mFlowNodeNo, getPathID(), getBitSW(),
                  getBitSW2(), fopAcM_GetParam(this));
        if (isDelete()) {
            OS_REPORT("===>isDelete:TRUE\n");
            return cPhs_ERROR_e;
        }
        OS_REPORT("\n");
        OS_REPORT("Taro: mType=%u\n", mType);

        static int const heapSize[17] = {
            0x40F0, 0x40F0, 0x48F0, 0x3840, 0x3910, 0x3910, 0x48F0,
            0x3910, 0x3910, 0x4100, 0x3900, 0x3900, 0x40F0, 0x3900,
            0x3900, 0x40F0, 0x48F0,
        };

        if (fopAcM_entrySolidHeap(this, createHeapCallBack, heapSize[mType]) == 0) {
            return cPhs_ERROR_e;
        }

        J3DModelData* modelData_p = mpMorf[0]->getModel()->getModelData();
        fopAcM_SetMtx(this, mpMorf[0]->getModel()->getBaseTRMtx());
        fopAcM_setCullSizeBox(this, -200.0f, -100.0f, -200.0f, 200.0f, 300.0f, 200.0f);
        mSound.init(&current.pos, &eyePos, 3, 1);
        field_0x9c0.init(&mAcch, 0.0f, 0.0f);

#if DEBUG
        // I'm unsure exactly how we're supposed to set + use the param ptr in the debug build...
        // field_0xe40 = &l_HIO;
        // field_0xe40->entryHIO("タロ");
#endif

        reset();
        mAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1,
                            &mAcchCir, fopAcM_GetSpeed_p(this), fopAcM_GetAngle_p(this), fopAcM_GetShapeAngle_p(this));
        mCcStts.Init(daNpc_Taro_Param_c::m.mSttsWeight, 0, this);
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

int daNpc_Taro_c::CreateHeap() {
    int bmdIdx = 0;
    if (mTwilight) {
        bmdIdx = 3;
    } else {
        if (mType == 14) {
            bmdIdx = 4;
        }
    }

    J3DModelData* modelData = static_cast<J3DModelData*>(
        dComIfG_getObjectRes(l_resNameList[l_bmdData[bmdIdx][1]], l_bmdData[bmdIdx][0]));
    if (modelData == NULL) {
        return 0;
    }

    mpMorf[0] = new mDoExt_McaMorfSO(modelData, NULL, NULL, NULL, -1, 1.0f, 0, -1, &mSound, 0x80000,
                                     0x11020284);
    if (mpMorf[0] == NULL || mpMorf[0]->getModel() == NULL) {
        return 0;
    }

    J3DModel* model = (J3DModel*)mpMorf[0]->getModel();
    for (u16 i = 0; i < modelData->getJointNum(); i++) {
        modelData->getJointNodePointer(i)->setCallBack(ctrlJointCallBack);
    }
    model->setUserArea((uintptr_t)this);
    mpMatAnm[0] = new daNpcT_MatAnm_c();
    if (mpMatAnm[0] == NULL) {
        return 0;
    }

    if (mType == 12 || mType == 15) {
        if (l_bmdData[1][0] >= 0) {
            modelData = (J3DModelData*)dComIfG_getObjectRes(l_resNameList[l_bmdData[1][1]],
                                                            l_bmdData[1][0]);
        } else {
            modelData = NULL;
        }

        if (modelData != NULL) {
            mModels[0] = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
        }

        if (mModels[0] == NULL) {
            return 0;
        }
    }

    if (mType == 0 || mType == 1 || mType == 2 || mType == 9) {
        if (l_bmdData[2][0] >= 0) {
            modelData = (J3DModelData*)dComIfG_getObjectRes(l_resNameList[l_bmdData[2][1]],
                l_bmdData[2][0]);
        } else {
            modelData = 0;
        }

        if (modelData != NULL) {
            mModels[1] = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
        }

        if (mModels[1] == NULL) {
            return 0;
        }
    }

    if (mTwilight) {
        if (setFaceMotionAnm(23, false) && setMotionAnm(33, 0.0f, 0)) {
            return 1;
        } else {
            return 0;
        }
    } else {
        if (setFaceMotionAnm(1, false) && setMotionAnm(0, 0, 0)) {
            return 1;
        } else {
            return 0;
        }
    }
}

int daNpc_Taro_c::Delete() {
    OS_REPORT("|%06d:%x|daNpc_Taro_c -> Delete\n", g_Counter, this);
    fopAcM_GetID(this);
    this->~daNpc_Taro_c();
    return 1;
}

int daNpc_Taro_c::Execute() {
    return execute();
}

int daNpc_Taro_c::Draw() {
    daNpcT_MatAnm_c* matAnm = mpMatAnm[0];
    if (matAnm != NULL) {
        J3DModelData* modelData = mpMorf[0]->getModel()->getModelData();
        J3DMaterial* material = modelData->getMaterialNodePointer(getEyeballMaterialNo());
        material->setMaterialAnm(matAnm);
    }

    return draw(FALSE, 0, mRealShadowSize, NULL, 100.0f, 0, 0, 0);
}

int daNpc_Taro_c::createHeapCallBack(fopAc_ac_c* i_this) {
    return static_cast<daNpc_Taro_c*>(i_this)->CreateHeap();
}

 int daNpc_Taro_c::ctrlJointCallBack(J3DJoint* param_1, int param_2) {
    if (param_2 == 0) {
        J3DModel* model = j3dSys.getModel();
        daNpc_Taro_c* i_this = (daNpc_Taro_c*)model->getUserArea();
        if (i_this != NULL) {
            i_this->ctrlJoint(param_1, model);
        }
      }
      return 1;
}

void* daNpc_Taro_c::srchArrow(void* param_1, void* param_2) {
    if (mFindCount < 50 && param_1 != NULL && param_1 != param_2) {
        if (fopAcM_IsExecuting(fopAcM_GetID(param_1)) && fopAcM_GetName(param_1) == PROC_ARROW) {
            mFindActorPtrs[mFindCount] = (fopAc_ac_c*)param_1;
            mFindCount++;
        }
    }
    return NULL;
}

void* daNpc_Taro_c::srchNpc(void* param_1, void* param_2) {
    if (mFindCount < 50 && param_1 != NULL && param_1 != param_2) {
        if (fopAcM_IsExecuting(fopAcM_GetID(param_1)) && 
        (fopAcM_GetGroup((fopAc_ac_c*)param_1) == 4 || 
        (fopAcM_GetName(param_1) == PROC_TAG_EVTAREA && ((daTag_EvtArea_c*)param_1)->getType() == 15))) {
            mFindActorPtrs[mFindCount] = (fopAc_ac_c*)param_1;
            mFindCount++;
        }
    }
    return NULL;
}

fopAc_ac_c* daNpc_Taro_c::getArrowP() {
    fopAc_ac_c* pActor = NULL;
    f32 minDist = G_CM3D_F_INF;
    mFindCount = 0;
    fopAcM_Search(srchArrow, this);
    for (int i = 0; i < mFindCount; i++) {
        if (cM3d_IsZero(mFindActorPtrs[i]->speedF) == false &&
            fopAcM_searchActorDistance(this, mFindActorPtrs[i]) < minDist)
        {
            minDist = fopAcM_searchActorDistance(this, mFindActorPtrs[i]);
            pActor = mFindActorPtrs[i];
        }
    }
    return pActor;
}

u8 daNpc_Taro_c::getType() {
    u8 val = fopAcM_GetParam(this) & 0xff;
    switch (val) {
    case TYPE_0:
        return TYPE_0;
    case TYPE_1:
        return TYPE_1;
    case TYPE_2:
        return TYPE_2;
    case TYPE_3:
        return TYPE_3;
    case TYPE_4:
        return TYPE_4;
    case TYPE_5:
        return TYPE_5;
    case TYPE_6:
        return TYPE_6;
    case TYPE_7:
        return TYPE_7;
    case TYPE_8:
        return TYPE_8;
    case TYPE_9:
        return TYPE_9;
    case TYPE_10:
        return TYPE_10;
    case TYPE_11:
        return TYPE_11;
    case TYPE_12:
        return TYPE_12;
    case TYPE_13:
        return TYPE_13;
    case TYPE_14:
        return TYPE_14;
    case TYPE_15:
        return TYPE_15;
    }
    return TYPE_16;
}

BOOL daNpc_Taro_c::isDelete() {
    switch (mType) {
    case TYPE_0:
        return !daNpcT_chkEvtBit(0x12) || daNpcT_chkEvtBit(0x1f);
    case TYPE_1:
        return !daNpcT_chkEvtBit(0x12);
    case TYPE_2:
        return 1;
    case TYPE_3:
        return 0;
    case TYPE_4:
        return !daNpcT_chkEvtBit(0x3d) || daNpcT_chkEvtBit(0xa4);
    case TYPE_5:
        return !daNpcT_chkEvtBit(0xa4) || daNpcT_chkEvtBit(0x35);
    case TYPE_6:
        return !daNpcT_chkEvtBit(0x35) || daNpcT_chkEvtBit(0x55);
    case TYPE_7:
        return !daNpcT_chkEvtBit(0x55) || daNpcT_chkEvtBit(0x40);
    case TYPE_8:
        return !daNpcT_chkEvtBit(0x40);
    case TYPE_9:
        return 0;
    case TYPE_10:
        return daNpcT_chkEvtBit(0x13);
    case TYPE_11:
        return !daNpcT_chkEvtBit(0x13) || daNpcT_chkEvtBit(0x12);
    case TYPE_12:
        return !daNpcT_chkEvtBit(0x12) || daNpcT_chkEvtBit(0x25c);
    case TYPE_13:
        return 0;
    case TYPE_14:
        return !daNpcT_chkEvtBit(0x271) || daNpcT_chkEvtBit(0xb8);
    case TYPE_15:
        return 0;
    }
    return 0;
}

void daNpc_Taro_c::reset() {
    csXyz acStack_20;
    int size = (u8*)&field_0x11a8 - (u8*)&mAction;
    if (mpMatAnm[0] != NULL) {
        mpMatAnm[0]->initialize();
    }
    initialize();
    for (int i = 0; i < 23; i++) {
        mActors[i].initialize();
    }
    memset(&mAction, 0, size);
    acStack_20.setall(0);
    acStack_20.y = home.angle.y;
    switch(mType) {
    case TYPE_0:
        eventInfo.setIdx(0);
        break;
    case TYPE_1:
        mHide = daNpcT_chkEvtBit(0x1f) == 0;
        field_0x11a5 = 1;
        eventInfo.setIdx(1);
        break;
    case TYPE_2:
        field_0x11a5 = 1;
        break;
    case TYPE_4:
        dComIfGs_setTmpReg(0xfbff, 0);
        break;
    case TYPE_9:
        field_0x11a5 = 1;
        break;
    case TYPE_12:
        field_0x11a4 = 1;
        break;
    case TYPE_13:
        field_0x11a2 = 1;
        break;
    case TYPE_15:
        field_0x11a4 = 1;
        break;
    }
    daNpcT_offTmpBit(0x3c);
    daNpcT_offTmpBit(0x3f);
    daNpcT_offTmpBit(0x40);
    daNpcT_offTmpBit(0x41);
    daNpcT_offTmpBit(0x42);
    daNpcT_offTmpBit(100);
    setAngle(acStack_20);
}

void daNpc_Taro_c::afterJntAnm(int param_1) {
    if (param_1 == 1) {
        mDoMtx_stack_c::YrotM(-mStagger.getAngleZ(1));
        mDoMtx_stack_c::ZrotM(mStagger.getAngleX(1));
    } else if (param_1 == 4) {
        mDoMtx_stack_c::YrotM(-mStagger.getAngleZ(0));
        mDoMtx_stack_c::ZrotM(mStagger.getAngleX(0));
    }
}

void daNpc_Taro_c::setParam() {
    selectAction();
    srchActors();
    int attentionFlags = (fopAc_AttnFlag_SPEAK_e | fopAc_AttnFlag_TALK_e);
    s16 sVar7 = daNpc_Taro_Param_c::m.field_0x48;
    s16 sVar5 = daNpc_Taro_Param_c::m.field_0x4a;
    s16 sVar6 = daNpc_Taro_Param_c::m.field_0x4c;
    s16 sVar4 = daNpc_Taro_Param_c::m.field_0x4e;

    if (mType == TYPE_13) {
        sVar5 = 6;
        sVar4 = 6;
    } else {
        if (&daNpc_Taro_c::swdTutorial == mAction) {
            sVar7 = 0xb;
            sVar5 = 6;
            sVar6 = 0xf;
            sVar4 = 6;
            attentionFlags = 0;
        }
        if (&daNpc_Taro_c::talk_withMaro == mAction) {
            sVar7 = 5;
            sVar5 = 4;
            sVar6 = 7;
            sVar4 = 4;
        }
    }

    if (daPy_getPlayerActorClass()->checkHorseRide()) {
        if (sVar7 < 7) {
            sVar7 = 7;
        }
        if (sVar6 < 9) {
            sVar6 = 9;
        }
    }

    attention_info.distances[fopAc_attn_LOCK_e] = daNpcT_getDistTableIdx(sVar6, sVar4);
    attention_info.distances[fopAc_attn_TALK_e] = attention_info.distances[fopAc_attn_LOCK_e];
    attention_info.distances[fopAc_attn_SPEAK_e] = daNpcT_getDistTableIdx(sVar7, sVar5);
    attention_info.flags = attentionFlags;
    scale.set(daNpc_Taro_Param_c::m.mScale, daNpc_Taro_Param_c::m.mScale,
              daNpc_Taro_Param_c::m.mScale);
    mCcStts.SetWeight(daNpc_Taro_Param_c::m.mSttsWeight);
    mCylH = daNpc_Taro_Param_c::m.mCylH;
    mWallR = daNpc_Taro_Param_c::m.mWallR;
    if (mTwilight) {
        mCylH = 100.0f;
    }
    mAttnFovY = daNpc_Taro_Param_c::m.mAttnFovY;
    if (mType == TYPE_13) {
        mAttnFovY = 180.0f;
    }
    mAcchCir.SetWallR(mWallR);
    mAcchCir.SetWallH(daNpc_Taro_Param_c::m.mWallH);
    mRealShadowSize = daNpc_Taro_Param_c::m.field_0x0c;
    if (&daNpc_Taro_c::practice == mAction) {
        mRealShadowSize = 500.0f;
    }
    mExpressionMorfFrame = daNpc_Taro_Param_c::m.field_0x6c;
    mMorfFrames = daNpc_Taro_Param_c::m.mMorfFrames;
    gravity = daNpc_Taro_Param_c::m.mGravity;
}

BOOL daNpc_Taro_c::checkChangeEvt() {
    if (!chkAction(&daNpc_Taro_c::talk)) {
        mPreItemNo = 0;
        if (dComIfGp_event_chkTalkXY()) {
            if (dComIfGp_evmng_ChkPresentEnd()) {
                mEvtNo = 1;
                evtChange();
            }
            return 1;
        }
        switch (mType) {
        case TYPE_1:
            if (daNpcT_chkEvtBit(0x269)) {
                mEvtNo = 11;
            } else {
                mEvtNo = 10;
            }
            evtChange();
            return 1;
        case TYPE_0:
            break;
        case TYPE_2:
            break;
        }
    }
    return 0;
}

BOOL daNpc_Taro_c::evtEndProc() {
    if (field_0x11a6 != 0) {
        dCam_getBody()->CorrectCenter();
    }
    return 1;
}

void daNpc_Taro_c::setAfterTalkMotion() {
    int idx = 31;
    switch (mFaceMotionSeqMngr.getNo()) {
    case 1:
        idx = 18;
        break;
    case 2:
        idx = 19;
        break;
    case 3:
        idx = 20;
        break;
    case 4:
        idx = 21;
        break;
    case 5:
        idx = 22;
        break;
    case 12:
        idx = 24;
        break;
    case 13:
        idx = 26;
        break;
    case 14:
        idx = 27;
        break;
    }
    mFaceMotionSeqMngr.setNo(idx, -1.0f, 0, 0);
}

void daNpc_Taro_c::srchActors() {
    switch (mType) {
    case TYPE_0:
        if (mActors[0].getActorP() == NULL) {
            mActors[0].entry(getNearestActorP(0x260));
        }

        if (mActors[1].getActorP() == NULL) {
            mActors[1].entry(getNearestActorP(0x245));
        }

        if (mActors[2].getActorP() == NULL) {
            mActors[2].entry(getNearestActorP(0x25d));
        }

        if (mActors[3].getActorP() == NULL) {
            mActors[3].entry(getEvtAreaTagP(4, 0));
        }
        break;
    case TYPE_1:
        if (mActors[0].getActorP() == NULL) {
            mActors[0].entry(getNearestActorP(0x260));
        }
        break;
    case TYPE_3:
        if (mActors[22].getActorP() == NULL) {
            mActors[22].entry(getNearestActorP(0x250));
        }
        break;
    case TYPE_4:
    case TYPE_5:
        if (mActors[0].getActorP() == NULL) {
            mActors[0].entry(getNearestActorP(0x260));
        }

        if (mActors[1].getActorP() == NULL) {
            mActors[1].entry(getNearestActorP(0x245));
        }

        if (mActors[2].getActorP() == NULL) {
            mActors[2].entry(getNearestActorP(0x25d));
        }
        break;
    case TYPE_8:
        if (mActors[0].getActorP() == NULL) {
            mActors[0].entry(getNearestActorP(0x260));
        }

        for (int i = 0; i < 16; i++) {
            if (mActors[i + 3].getActorP() == NULL) {
                mActors[i + 3].entry(getEvtAreaTagP(15, i));
            }
        }
        break;
    case TYPE_10:
        if (mActors[1].getActorP() == NULL) {
            mActors[1].entry(getNearestActorP(0x245));
        }
        break;
    case TYPE_12:
        if (mActors[2].getActorP() == NULL) {
            mActors[2].entry(getNearestActorP(0x25d));
        }

        if (mActors[20].getActorP() == 0) {
            mActors[20].entry(getNearestActorP(0x264));
        }
        break;
    case TYPE_13:
        if (mActors[3].getActorP() == NULL) {
            mActors[3].entry(getEvtAreaTagP(24, 0));
        }
        break;
    case TYPE_14:
        if (mActors[21].getActorP() == NULL) {
            mActors[21].entry(getNearestActorP(0x262));
        }
        break;
    case TYPE_15:
        if (mActors[0].getActorP() == NULL) {
            mActors[0].entry(getNearestActorP(0x260));
        }

        if (mActors[1].getActorP() == NULL) {
            mActors[1].entry(getNearestActorP(0x245));
        }
        break;
    }
}

BOOL daNpc_Taro_c::evtTalk() {
    if (chkAction(&daNpc_Taro_c::talk)) {
        (this->*(mAction2))(NULL);
    } else {
        setAction(&daNpc_Taro_c::talk);
    }
    return 1;
}

BOOL daNpc_Taro_c::evtCutProc() {
    int staffId = dComIfGp_getEventManager().getMyStaffId("Taro", this, -1);
    if (staffId != -1) {
        if (mStaffId == -1) {
            if ((strcmp(dComIfGp_getEventManager().getRunEventName(), "BEGIN_ARROWTUTORIAL") == 0) ||
                (strcmp(dComIfGp_getEventManager().getRunEventName(), "BEGIN_ARROWTUTORIAL_NO_ARROW") ==
                0) ||
                (strcmp(dComIfGp_getEventManager().getRunEventName(), "RETRY_ARROWTUTORIAL1") == 0) ||
                (strcmp(dComIfGp_getEventManager().getRunEventName(),
                        "RETRY_ARROWTUTORIAL1_NO_ARROW") == 0) ||
                (strcmp(dComIfGp_getEventManager().getRunEventName(), "RETRY_ARROWTUTORIAL2") == 0) ||
                (strcmp(dComIfGp_getEventManager().getRunEventName(),
                        "RETRY_ARROWTUTORIAL2_NO_ARROW") == 0) ||
                (strcmp(dComIfGp_getEventManager().getRunEventName(), "RETRY_ARROWTUTORIAL3") == 0) ||
                (strcmp(dComIfGp_getEventManager().getRunEventName(),
                        "RETRY_ARROWTUTORIAL3_NO_ARROW") == 0) ||
                (strcmp(dComIfGp_getEventManager().getRunEventName(), "CAUTION") == 0))
            {
                field_0x11a6 = 1;
            }
        }

        mStaffId = staffId;
        int actIdx = dComIfGp_getEventManager().getMyActIdx(mStaffId, (char**)&mCutNameList, 0x11, 0, 0);
        if ((this->*(mCutList[actIdx]))(mStaffId)) {
            dComIfGp_getEventManager().cutEnd(mStaffId);
        }
        return TRUE;
    }

    return FALSE;
}

void daNpc_Taro_c::action() {
    fopAc_ac_c* hitActor = NULL;
    if (!mTwilight) {
        hitActor = hitChk(&mCyl1, 0xffffffff);
    }
    if (hitActor != NULL && mType != TYPE_6) {
        mStagger.setParam(this,
                                        hitActor, mCurAngle.y);
        setDamage(0, 17, 33);

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

        mMode = MODE_INIT;
    }
    if (!dComIfGp_event_runCheck()) {
        field_0x11a6 = 0;
    }
    if (mAction != NULL) {
        if (mAction2 == mAction) {
            (this->*(mAction2))(NULL);
        } else {
            setAction(mAction);
        }
    }
    
    daTag_Push_c* pushTag = (daTag_Push_c*)field_0xba0.getActorP();
    if (pushTag != NULL) {
        switch (pushTag->getId()) {
        case 1:
            mEvtNo = 2;
            break;
        case 4:
            mEvtNo = 0x1a;
            break;
        }
    }
}

void daNpc_Taro_c::beforeMove() {
    fopAcM_OffStatus(this, fopAcM_STATUS_UNK_0x8000000);
    if (checkHide()) {
        fopAcM_OnStatus(this, fopAcM_STATUS_UNK_0x8000000);
    }
    if (checkHide() || mNoDraw) {
        attention_info.flags = 0;
    }
}

void daNpc_Taro_c::setAttnPos() {
    cXyz eyeOffset(0.0f, 30.0f, 0.0f);
    if (field_0x11a1 != 0) {
        daPy_getPlayerActorClass()->onWolfEyeKeep();
    }
    BOOL isStaggering = FALSE;
    if (mMotionSeqMngr.getNo() == 33 && !mMotionSeqMngr.checkEndSequence()) {
        isStaggering = TRUE;
    }
    mStagger.calc(isStaggering);
    f32 dVar8 = cM_s2rad(mCurAngle.y - field_0xd7e.y);
    mJntAnm.setParam(
        this, mpMorf[0]->getModel(), &eyeOffset, getBackboneJointNo(), getNeckJointNo(),
        getHeadJointNo(), daNpc_Taro_Param_c::m.mBodyUpAngle, daNpc_Taro_Param_c::m.mBodyDownAngle,
        daNpc_Taro_Param_c::m.mBodyLeftAngle, daNpc_Taro_Param_c::m.mBodyRightAngle,
        daNpc_Taro_Param_c::m.mHeadUpAngle, daNpc_Taro_Param_c::m.mHeadDownAngle,
        daNpc_Taro_Param_c::m.mHeadLeftAngle, daNpc_Taro_Param_c::m.mHeadRightAngle,
        daNpc_Taro_Param_c::m.field_0x40, 0.0f, NULL);
    mJntAnm.calcJntRad(0.2f, 1.0f, (float)dVar8);
    setMtx();
    mDoMtx_stack_c::copy(mpMorf[0]->getModel()->getAnmMtx(getHeadJointNo()));
    mDoMtx_stack_c::multVec(&eyeOffset, &eyePos);
    mJntAnm.setEyeAngleX(eyePos, 1.0f, 0);
    mJntAnm.setEyeAngleY(eyePos, mCurAngle.y, 1, 1.0f, 0);
    eyeOffset.set(0.0f, 0.0f, 0.0f);
    eyeOffset.y = daNpc_Taro_Param_c::m.mAttentionPosYOffset;
    mDoMtx_stack_c::YrotS(mCurAngle.y);
    mDoMtx_stack_c::multVec(&eyeOffset, &eyeOffset);
    attention_info.position = current.pos + eyeOffset;
    if (!checkHide()) {
        setSwingVoice();
    }

    static cXyz prtclScl(1.0f, 1.0f, 1.0f);
    setFootPos();
    if (mMotionSeqMngr.getNo() == 37) {
        setFootPrtcl(&prtclScl, 10.0f, 0.0f);
    }
}

void daNpc_Taro_c::setCollision() {
    cXyz cStack_48;
    if (!mHide) {
        int tgType = 0xd8fbfdff;
        int tgSPrm = 0x1f;
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
        if (mType == TYPE_3) {
            cStack_48.set(-63.39f, 0.0f, 0.0f);
        }
        mDoMtx_stack_c::YrotS(mCurAngle.y);
        mDoMtx_stack_c::multVec(&cStack_48, &cStack_48);
        cStack_48 += current.pos;
        mCyl1.SetH(cylH);
        mCyl1.SetR(wallR);
        mCyl1.SetC(cStack_48);
        dComIfG_Ccsp()->Set(&mCyl1);
        if (mType == TYPE_1 || mType == TYPE_2 ||
            mType == TYPE_8)
        {
            switch (mType) {
            case TYPE_1:
            case TYPE_2:
                wallR = 140.0f;
                cStack_48.set(60.0f, 0.0f, 40.0f);
                break;
            case TYPE_8:
                wallR = 80.0;
                cStack_48.set(0.0f, 0.0f, 0.0f);
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

int daNpc_Taro_c::drawDbgInfo() {
    return FALSE;
}

void daNpc_Taro_c::drawOtherMdl() {
    J3DModel* model = mpMorf[0]->getModel();
    for (int i = 0; i < 2; i++) {
        if (mModels[i] != NULL && ((i == 0 && field_0x11a4 != 0) || (i == 1 && field_0x11a5 != 0)))
        {
            g_env_light.setLightTevColorType_MAJI(mModels[i], &tevStr);

            static int const jointNo[2] = {
                15, 15,
            };

            mDoMtx_stack_c::copy(model->getAnmMtx(jointNo[i]));
            Mtx jointAnmMtx;
            cMtx_copy(mDoMtx_stack_c::get(), jointAnmMtx);
            mModels[i]->setBaseTRMtx(jointAnmMtx);
            mDoExt_modelUpdateDL(mModels[i]);
            dComIfGd_addRealShadow(mShadowKey, mModels[i]);
        }
    }
}

daNpcT_faceMotionAnmData_c daNpc_Taro_c::getFaceMotionAnm(daNpcT_faceMotionAnmData_c param_0) {
    daNpcT_faceMotionAnmData_c rv = param_0;
    if (mType == TYPE_14 && rv.mBtpFileIdx == 7 && rv.mBtpArcIdx == 1) {
        rv.mBtpFileIdx = 7;
        rv.mBtpArcIdx = 9;
    }
   return rv;
}

BOOL daNpc_Taro_c::selectAction() {
    mAction = NULL;
    switch(mType) {
    case TYPE_7:
        mAction = &daNpc_Taro_c::nurse;
        break;
    case TYPE_15:
        // daNpcKakashi_chkSwdTutorialStage itself doesn't match when it returns bool 
        if ((u8)daNpcKakashi_chkSwdTutorialStage()) {
            mAction = &daNpc_Taro_c::swdTutorial;
        } else {
            mAction = &daNpc_Taro_c::wait;
        }
        break;
    default:
        mAction = &daNpc_Taro_c::wait;
        break;
    }

    return TRUE;
}

BOOL daNpc_Taro_c::chkAction(int (daNpc_Taro_c::*action)(void*)) {
    return mAction2 == action;
}

BOOL daNpc_Taro_c::setAction(int (daNpc_Taro_c::*action)(void*)) {
    mMode = MODE_EXIT;
    if (mAction2 != NULL) {
        (this->*(mAction2))(NULL);
    }
    mMode = MODE_ENTER;
    mAction2 = action;
    if (mAction2 != NULL) {
        (this->*(mAction2))(NULL);
    }
    return 1;
}

void daNpc_Taro_c::setSwingVoice() {
    if (mType != TYPE_3 && mType != TYPE_8 && mType != TYPE_13) {
        J3DAnmTransform* anmTransform = getTrnsfrmAnmP(l_resNameList[l_motionAnmData[19].mBckArcIdx],
            l_motionAnmData[19].mBckFileIdx);
        if (anmTransform == mpMorf[0]->getAnm() &&
            mpMorf[0]->checkFrame(34.0f))
        {
            mSound.startCreatureVoice(Z2SE_TARO_V_SWING_L, -1);
        }

        anmTransform = getTrnsfrmAnmP(l_resNameList[l_motionAnmData[20].mBckArcIdx],
                                      l_motionAnmData[20].mBckFileIdx);
        if (anmTransform == mpMorf[0]->getAnm() && mpMorf[0]->checkFrame(52.0f)) {
            mSound.startCreatureVoice(Z2SE_TARO_V_SWING_S, -1);
        }
    }
}

int daNpc_Taro_c::cutConversationAboutPachinko(int param_1) {
    fopAc_ac_c* actor_p;
    int rv = 0;
    int prm = -1;
    int* pPrm = dComIfGp_evmng_getMyIntegerP(param_1, "prm");
    if (pPrm != NULL) {
        prm = *pPrm;
    }
    if (dComIfGp_getEventManager().getIsAddvance(param_1)) {
        switch(prm) {
        case 0:
            mPlayerAngle = fopAcM_searchPlayerAngleY(this);
            break;
        case 1:
            break;
        case 10:
            break;
        }
    }

    switch (prm) {
    case 0:
        mJntAnm.lookPlayer(0);
        if (mPlayerAngle == mCurAngle.y) {
            rv = TRUE;
        } else {
            step(mPlayerAngle, 0x1f, 0x24, 0xf, 0);
        }
        break;
    case 1: {
        actor_p = mActors[1].getActorP();
        JUT_ASSERT(3282, NULL != actor_p);
        mJntAnm.lookActor(actor_p, -40.0, 0);
        if (home.angle.y == mCurAngle.y) {
            rv = TRUE;
        } else {
            step(home.angle.y, -1, -1, 0xf, 0);
        }
        break;
    }
    case 10:
        rv = TRUE;
        break;
    }
    return rv;
}

int daNpc_Taro_c::cutConversationAboutWoodSwd(int param_1) {
    fopAc_ac_c* actor_p;
    int rv = 0;
    int prm = -1;
    int* pPrm = dComIfGp_evmng_getMyIntegerP(param_1, "prm");
    if (pPrm != NULL) {
        prm = *pPrm;
    }
    if (dComIfGp_getEventManager().getIsAddvance(param_1)) {
        switch(prm) {
        case 0:
            mPlayerAngle = fopAcM_searchPlayerAngleY(this);
            break;
        case 1:
            break;
        }
    }

    switch (prm) {
    case 0:
        mJntAnm.lookPlayer(0);
        if (mPlayerAngle != mCurAngle.y) {
            if (step(mPlayerAngle, 0x1f, 0x24, 0xf, 0) != 0) {
                mFaceMotionSeqMngr.setNo(31, -1.0f, 0, 0);
                mMotionSeqMngr.setNo(0, -1.0f, 0, 0);
            }
        } else {
            rv = TRUE;
        }
        break;
    case 1:
        rv = TRUE;
        break;
    }
    return rv;
}

int daNpc_Taro_c::cutSwdTutorial(int param_1) {
    fopAc_ac_c* actor_p = NULL;
    cXyz acStack_34;
    cXyz cStack_40;
    csXyz cStack_60;
    int uVar12 = 0;
    int local_70 = -1;
    int eventTimer = 0;
    int iVar11 = 0;
    int* pPrm = dComIfGp_evmng_getMyIntegerP(param_1, "prm");
    if (pPrm != NULL) {
        local_70 = *pPrm;
    }
    int* pTimer =
        dComIfGp_evmng_getMyIntegerP(param_1, "timer");
    if (pTimer != NULL) {
        eventTimer = *pTimer;
    }
    int* pMsgNo = dComIfGp_evmng_getMyIntegerP(param_1, "msgNo");
    if (pMsgNo != NULL) {
        iVar11 = *pMsgNo;
    }
    fopAc_ac_c* local_4c[3] = {this, mActors[0].getActorP(), mActors[1].getActorP()};
    dComIfGp_setMesgCameraInfoActor(local_4c[0], local_4c[1], local_4c[2], 0, 0, 0, 0, 0, 0, 0);
    if (dComIfGp_getEventManager().getIsAddvance(param_1)) {
        switch (local_70) {
        case 0:
            mFaceMotionSeqMngr.setNo(0x1f, 0.0, 0, 0);
            mMotionSeqMngr.setNo(0xb, 0.0, 0, 0);
            mEventTimer = eventTimer;
            break;
        case 1:
            initTalk(0x1f5, local_4c);
            break;
        case 10:
            mEventTimer = eventTimer;
            break;
        case 0xb:
            mFaceMotionSeqMngr.setNo(0x1f, 0.0, 0, 0);
            mMotionSeqMngr.setNo(0x14, 0.0, 0, 0);
            mJntAnm.lookNone(1);
            initTalk(0x1f5, local_4c);
            break;
        case 0x15:
            mFaceMotionSeqMngr.setNo(0x10, 0.0, 0, 0);
            mMotionSeqMngr.setNo(0x1b, 0.0, 0, 0);
            mJntAnm.lookNone(1);
            break;
        case 0x16:
            initTalk(0x1f5, local_4c);
            break;
        case 0x1f:
            mFaceMotionSeqMngr.setNo(0x10, 0.0, 0, 0);
            mMotionSeqMngr.setNo(0x1c, 0.0, 0, 0);
            mJntAnm.lookNone(1);
            break;
        case 0x29:
        case 0x3d:
            mFaceMotionSeqMngr.setNo(0x1f, 0.0, 0, 0);
            mMotionSeqMngr.setNo(0xb, 0.0, 0, 0);
            mJntAnm.lookNone(1);
            initTalk(0x1f5, local_4c);
            if (local_70 == 0x3d) {
                actor_p = mActors[0].getActorP();
                JUT_ASSERT(3480, NULL != actor_p);
                shape_angle.y = fopAcM_searchActorAngleY(this, actor_p);
            }
            break;
        case 0x33:
            mFaceMotionSeqMngr.setNo(0x1f, 0.0, 0, 0);
            mMotionSeqMngr.setNo(0xb, 0.0, 0, 0);
            mJntAnm.lookNone(1);
            initTalk(0x1f5, local_4c);
            break;
        case 0x3f:
            shape_angle.y = mCurAngle.y;
            break;
        case 0x47:
            mFaceMotionSeqMngr.setNo(3, 0.0, 0, 0);
            mMotionSeqMngr.setNo(0xf, 0.0, 0, 0);
            setAngle(home.angle.y);
            acStack_34.set(-50.0f, 0.0f, 0.0f);
            mDoMtx_stack_c::YrotS(mCurAngle.y);
            mDoMtx_stack_c::multVec(&acStack_34, &acStack_34);
            acStack_34 += current.pos;
            setPos(acStack_34);
            initTalk(0x1f6, local_4c);
            break;
        case 0x48:
            daNpcT_onEvtBit(0x12);
            break;
        }
    }
    int local_68[2] = {-1, 0xffffffff};
    switch (local_70) {
    case 0:
        mJntAnm.lookPlayer(0);
        if (cLib_calcTimer(&mEventTimer) == 0) {
            uVar12 = 1;
        }
        break;
    case 1:
        if (talkProc(NULL, 0, local_4c, 0) != 0 && mFlow.checkEndFlow()) {
            uVar12 = 1;
        }
        break;
    case 2:
        uVar12 = 1;
        break;
    case 10:
        mDamageTimer = 1;
        action();
        if (cLib_calcTimer(&mEventTimer) == 0) {
            uVar12 = 1;
        }
        break;
    case 0xb:
    case 0x29:
    case 0x2a:
    case 0x3d:
        local_68[0] = iVar11;
        if (talkProc(local_68, 0, local_4c, 0) != 0) {
            if (iVar11 == 0) {
                if (mFlow.checkEndFlow()) {
                    daNpcT_getPlayerInfoFromPlayerList(0x14, fopAcM_GetRoomNo(this), &acStack_34,
                                                       &cStack_60);
                    daPy_getPlayerActorClass()->setPlayerPosAndAngle(&acStack_34, cStack_60.y, 0);
                    uVar12 = 1;
                }
            } else {
                uVar12 = 1;
            }
        }
        break;
    case 0x15:
        switch (mMotionSeqMngr.getNo()) {
        case 27:
            if (mMotionSeqMngr.getStepNo() > 0) {
                mMotionSeqMngr.setNo(0xf, -1.0f, 0, 0);
            } else {
                if (mMotionSeqMngr.getStepNo() == 0 && mpMorf[0]->checkFrame(8.0f)) {
                    mSound.startCreatureVoice(Z2SE_M055_TARO_02, -1);
                }
            }
            break;
        case 15:
            if (mMotionSeqMngr.getStepNo() > 0) {
                uVar12 = 1;
            }
            break;
        }
        break;
    case 0x16:
    case 0x17:
    case 0x33:
    case 0x34:
        local_68[0] = iVar11;
        if (talkProc(local_68, 0, local_4c, 0) != 0) {
            if (iVar11 == 0) {
                if (mFlow.checkEndFlow()) {
                    daNpcT_getPlayerInfoFromPlayerList(0x14, fopAcM_GetRoomNo(this), &acStack_34, &cStack_60);
                    daPy_getPlayerActorClass()->setPlayerPosAndAngle(&acStack_34, cStack_60.y, 0);
                    u8 step = daNpcKakashi_getSwdTutorialStep();
                    step++;
                    if (daNpcKakashi_getSwdTutorialStep() == 2) {
                        step = step + 1;
                    }
                    daNpcKakashi_setSwdTutorialStep(step);
                    daNpcKakashi_clrSuccessCount();
                    uVar12 = 1;
                }
            } else {
                uVar12 = 1;
            }
        }
        break;
    case 0x1f:
        switch (mMotionSeqMngr.getNo()) {
        case 0x1c:
            if (mMotionSeqMngr.getStepNo() > 0) {
                mMotionSeqMngr.setNo(9, -1.0f, 0, 0);
            } else {
                if (mMotionSeqMngr.getStepNo() == 0 && mpMorf[0]->checkFrame(10.0f)) {
                    mSound.startCreatureVoice(Z2SE_M056_TARO_03, -1);
                }
            }
            break;
        case 9:
            if (mMotionSeqMngr.getStepNo() > 0) {
                uVar12 = 1;
            }
            break;
        }
        break;
    case 0x3f:
        uVar12 = 1;
        break;
    case 0x47:
        field_0xe26 = false;
        mJntAnm.lookCamera(0);
        if (talkProc(NULL, 0, local_4c, 0) != 0 && mFlow.checkEndFlow()) {
            uVar12 = 1;
        }
        break;
    case 0x48:
        dStage_changeScene(13, 0.0f, 0, fopAcM_GetRoomNo(this), 0, -1);
        break;
    }
    return uVar12;
}

int daNpc_Taro_c::cutHail(int param_1) {
    int rv = FALSE;
    int prm = -1;
    int* pPrm = (int*)dComIfGp_evmng_getMyIntegerP(param_1, "prm");
    if (pPrm != NULL) {
        prm = *pPrm;
    }

    if (dComIfGp_getEventManager().getIsAddvance(param_1)) {
        switch(prm) {
        case 0: {
            daTag_Push_c* actor_p = (daTag_Push_c*)field_0xba0.getActorP();
            JUT_ASSERT(3723, NULL != actor_p)
            actor_p->pushBackPlayer(0);
            break;
        }
        case 1: {
            mFaceMotionSeqMngr.setNo(0x1f, 0.0f, 0, 0);
            mMotionSeqMngr.setNo(0, 0.0f, 0, 0);
            mJntAnm.lookNone(1);
            daTag_Push_c* actor_p = (daTag_Push_c*)field_0xba0.getActorP();
            JUT_ASSERT(3732, NULL != actor_p)
            initTalk(actor_p->getFlowNodeNo(), NULL);
            setAngle(fopAcM_searchPlayerAngleY(this));
            break;
        }
        }
    }

    switch(prm) {
    case 0:
        action();
        rv = TRUE;
        break;
    case 1:
        if (talkProc(NULL, 0, NULL, 0) != 0 && mFlow.checkEndFlow()) {
            field_0xba0.remove();
            rv = TRUE;
        }
        break;
    }

    return rv;
}

int daNpc_Taro_c::cutFindMonkey(int param_1) {
    cXyz horsePos;
    int rv = FALSE;
    int prm = -1;
    int timer = 0;
    int msgNo = 0;
    int* pPrm = dComIfGp_evmng_getMyIntegerP(param_1, "prm");
    if (pPrm != NULL) {
        prm = *pPrm;
    }

    int* pTimer = dComIfGp_evmng_getMyIntegerP(param_1, "timer");
    if (pTimer != NULL) {
        timer = *pTimer;
    }

    int* piVar2 = dComIfGp_evmng_getMyIntegerP(param_1, "msgNo");
    if (piVar2 != NULL) {
        msgNo = *piVar2;
    }

    if (dComIfGp_getEventManager().getIsAddvance(param_1)) {
        switch (prm) {
        case 0:
            daNpcT_offTmpBit(0xb);
            daNpcT_offTmpBit(0xc);
            initTalk(0x1f7, NULL);
            daNpcT_onEvtBit(0x25c);
            break;
        case 1:
            mEventTimer = timer;
            break;
        case 2:
            break;
        case 3:
            initTalk(0x1f7, NULL);
            break;
        case 4:
            mMotionSeqMngr.setNo(0x25, -1.0f, 0, 0);
            mEventTimer = timer;
            mDoAud_bgmStart(Z2BGM_EVENT05);
            break;
        case 5:
            mHide = 1;
            speedF = 0.0f;
            speed.setall(0.0f);
            mEventTimer = timer;
            break;
        case 6:
            break;
        }
    }

    int msgNos[3] = {-1, -1 ,-1};

    switch (prm) {
    case 0:
        mJntAnm.lookPlayer(0);
        msgNos[0] = msgNo;
        if (talkProc(msgNos, 0, NULL, 0) != 0) {
            if (msgNo == 0) {
                if (mFlow.checkEndFlow()) {
                    rv = TRUE;
                }
            } else {
                rv = TRUE;
            }
        }
        break;
    case 1: {
        if (cLib_calcTimer(&mEventTimer) == 0) {
            fopAc_ac_c* p_actor = mActors[20].getActorP();
            JUT_ASSERT(3870, NULL != p_actor);
            mJntAnm.lookActor(p_actor, -40.0f, 0);
            rv = TRUE;
        } else {
            mJntAnm.lookPlayer(0);
        }
        break;
    }
    case 2: {
        fopAc_ac_c* p_actor = mActors[20].getActorP();
        JUT_ASSERT(3881, NULL != p_actor);
        mJntAnm.lookActor(p_actor, -40.0f, 0);
        rv = TRUE;
        break;
    }
    case 3: {
        fopAc_ac_c* p_actor = mActors[20].getActorP();
        JUT_ASSERT(3888, NULL != p_actor);
        mJntAnm.lookActor(p_actor, -40.0f, 0);
        if (talkProc(msgNos, 0, NULL, 0) != 0 && mFlow.checkEndFlow()) {
            rv = TRUE;
        }
        break;
    }
    case 4: {
        fopAc_ac_c* p_actor = mActors[20].getActorP();
        JUT_ASSERT(3899, NULL != p_actor);
        mJntAnm.lookActor(p_actor, -40.0f, 0);
        p_actor = dComIfGp_getHorseActor();
        JUT_ASSERT(3902, NULL != p_actor);
        if (cLib_calcTimer(&mEventTimer) == 0) {
            rv = TRUE;
        }
        horsePos = p_actor->current.pos;
        if (2500.0f < current.pos.abs2(horsePos)) {
            cLib_addCalcAngleS2(&current.angle.y, cLib_targetAngleY(&current.pos, &horsePos), 4, 0x800);
            shape_angle.y = current.angle.y;
            mCurAngle.y = shape_angle.y;
            cLib_chaseF(&speedF, 16.0f, 0.5f);
        } else {
            rv = TRUE;
        }
        break;
    }
    case 5:
        if (cLib_calcTimer(&mEventTimer) == 0) {
            rv = TRUE;
        }
        break;
    case 6:
        rv = TRUE;
        break;
    }
    return rv;
}

int daNpc_Taro_c::cutHelpMe(int param_1) {
    int rv = FALSE;
    int prm = -1;
    int timer = 0;
    int* pPrm = dComIfGp_evmng_getMyIntegerP(param_1, "prm");
    if (pPrm != NULL) {
        prm = *pPrm;
    }

    int* pTimer = dComIfGp_evmng_getMyIntegerP(param_1, "timer");
    if (pTimer != NULL) {
        timer = *pTimer;
    }

    if (dComIfGp_getEventManager().getIsAddvance(param_1)) {
        switch (prm) {
        case 0:
            mFaceMotionSeqMngr.setNo(0x1e, 0.0f, 0, 0);
            mMotionSeqMngr.setNo(0x1a, 0.0f, 0, 0);
            mEventTimer = timer;
            break;
        case 1:
            mFaceMotionSeqMngr.setNo(0x1c, -1.0, 0, 0);
            mMotionSeqMngr.setNo(0x29, -1.0f, 0, 0);
            mSound.startCreatureVoice(Z2SE_TARO_V_CAGE_HAPPY, -1);
            mDoAud_subBgmStart(Z2BGM_TARO_RESCUE);
            mEventTimer = timer;
            break;
        case 2:
            daNpcT_onEvtBit(0x271);
            break;
        case 3:
            daNpcT_onEvtBit(0x25d);
            break;
        }
    }
    switch (prm) {
    case 0:
        mJntAnm.lookNone(0);
        if (cLib_calcTimer(&mEventTimer) == 0) {
            rv = TRUE;
        }
        break;
    case 1:
        mJntAnm.lookNone(0);
        if (cLib_calcTimer(&mEventTimer) == 0) {
            rv = TRUE;
        }
        break;
    case 2:
        dStage_changeScene(7, 0.0f, 0, fopAcM_GetRoomNo(this), 0, -1);
        break;
    case 3:
        action();
        mJntAnm.lookNone(0);
        rv = TRUE;
        break;
    }

    return rv;
}

int daNpc_Taro_c::cutAppearanceMoi(int param_1) {
    cXyz work;
    csXyz cStack_68;
    int rv = FALSE;
    int prm = -1;
    int timer = 0;
    int* pPrm = dComIfGp_evmng_getMyIntegerP(param_1, "prm");
    if (pPrm != NULL) {
        prm = *pPrm;
    }

    int* pTimer = dComIfGp_evmng_getMyIntegerP(param_1, "timer");
    if (pTimer != NULL) {
        timer = *pTimer;
    }
    fopAc_ac_c* local_70[2] = {this, mActors[21].getActorP()};
    dComIfGp_setMesgCameraInfoActor(local_70[0], local_70[1], 0, 0, 0, 0, 0, 0, 0, 0);
    if (dComIfGp_getEventManager().getIsAddvance(param_1)) {
        switch (prm) {
        case 0:
            work.set(0.0f, 0.0f, 400.0f);
            cStack_68.y = daPy_getPlayerActorClass()->current.angle.y + cM_deg2s(10.0f);
            mDoMtx_stack_c::YrotS(cStack_68.y);
            mDoMtx_stack_c::multVec(&work, &work);
            work += daPy_getPlayerActorClass()->current.pos;
            dComIfGp_evmng_setGoal(&work);
            daPy_getPlayerActorClass()->changeDemoMoveAngle(cStack_68.y);
            daNpcT_offTmpBit(0xb);
            daNpcT_offTmpBit(0xc);
            break;
        case 1:
            mFaceMotionSeqMngr.setNo(0x1f, -1.0f, 0, 0);
            mMotionSeqMngr.setNo(0x27, -1.0f, 0, 0);
            work.set(0.0f, 0.0f, 100.0f);
            mDoMtx_stack_c::YrotS(home.angle.y + cM_deg2s(-10.0f));
            mDoMtx_stack_c::multVec(&work, &work);
            work += home.pos;
            setPos(work);
            initTalk(0x78, local_70);
            work = daPy_getPlayerActorClass()->current.pos;
            cStack_68.y = daPy_getPlayerActorClass()->shape_angle.y;
            dComIfGp_evmng_setGoal(&work);
            daPy_getPlayerActorClass()->changeDemoMoveAngle(cStack_68.y);
            break;
        case 2:
            mFaceMotionSeqMngr.setNo(0x1f, 0.0f, 0, 0);
            mMotionSeqMngr.setNo(0x18, 0.0f, 0, 0);
            setAngle(mPlayerAngle);
            break;
        case 3:
            mFaceMotionSeqMngr.setNo(0x1f, -1.0f, 0, 0);
            mMotionSeqMngr.setNo(0x25, -1.0f, 0, 0);
            home.pos = current.pos;
            mEventTimer = timer;
            break;
        case 4:
            setAngle(home.angle.y + cM_deg2s(-10.0f));
            work.set(0.0f, 0.0f, 1600.0f);
            mDoMtx_stack_c::YrotS(mCurAngle.y);
            mDoMtx_stack_c::multVec(&work, &work);
            work += home.pos;
            setPos(work);
            work.set(0.0f, 500.0f, 300.0f);
            mDoMtx_stack_c::multVec(&work, &work);
            work += home.pos;
            mGndChk.SetPos(&work);
            work.y = dComIfG_Bgsp().GroundCross(&mGndChk);
            JUT_ASSERT(4156, -G_CM3D_F_INF != work.y)
            cStack_68.y = cLib_targetAngleY(&work, &current.pos);
            daPy_getPlayerActorClass()->setPlayerPosAndAngle(&work, cStack_68.y, 0);
            mEventTimer = timer;
            break;
        case 6:
            speedF = 0.0f;
            speed.setall(0.0f);
            mHide = 1;
            break;
        case 10:
            break;
        }
    }
    switch (prm) {
    case 0:
        mJntAnm.lookNone(0);
        work.set(0.0f, 0.0f, 500.0f);
        mDoMtx_stack_c::YrotS(home.angle.y + cM_deg2s(-10.0f));
        mDoMtx_stack_c::multVec(&work, &work);
        work += home.pos;
        calcSpeedAndAngle(work, current.pos.absXZ(work) < 50.0f, 4, 0x800);
        if (current.pos.absXZ(work) < 50.0f && cM3d_IsZero(speedF)) {
            rv = TRUE;
        }
        break;
    case 1:
        mJntAnm.lookNone(0);
        if (cLib_calcTimer(&mEventTimer) == 0) {
            rv = TRUE;
        }
        break;
    case 2:
        mJntAnm.lookPlayer(0);
        if (talkProc(NULL, 0, local_70, 0) != 0 && mFlow.checkEndFlow()) {
            rv = TRUE;
        }
        break;
    case 3:
    case 4:
    case 5:
        mJntAnm.lookNone(0);
        work.set(0.0f, 0.0f, 1600.0f);
        if (prm != 3) {
            work.z += 800.0f;
        }
        mDoMtx_stack_c::YrotS(home.angle.y + cM_deg2s(-10.0f));
        mDoMtx_stack_c::multVec(&work, &work);
        work += home.pos;
        calcSpeedAndAngle(work, current.pos.absXZ(work) < 50.0f, 4, 0x800);
        if (cLib_calcTimer(&mEventTimer) == 0) {
            rv = TRUE;
        }
        break;
    case 6:
        rv = TRUE;
        break;
    case 10:
        if (mCurAngle.y != mPlayerAngle) {
            if (step(mPlayerAngle, -1, 0x24, 0xf, 0) != 0) {
                mFaceMotionSeqMngr.setNo(0x1f, -1.0f, 0, 0);
                mMotionSeqMngr.setNo(0x16, -1.0f, 0, 0);
            }
        } else if (cLib_calcTimer(&mEventTimer) == 0) {
            rv = TRUE;
        }
        break;
    }
    return rv;
}

int daNpc_Taro_c::cutGiveMeWoodSwd(int param_1) {
    cXyz cStack_2c;
    csXyz cStack_58;
    int rv = FALSE;
    int prm = -1;
    int msgNo = 0;
    int msgNo2 = 0;
    int timer = 0;

    int* pPrm = dComIfGp_evmng_getMyIntegerP(param_1, "prm");
    if (pPrm != NULL) {
        prm = *pPrm;
    }

    int* pMsgNo = dComIfGp_evmng_getMyIntegerP(param_1, "msgNo");
    if (pMsgNo != NULL) {
        msgNo = *pMsgNo;
    }

    int* pMsgNo2 = dComIfGp_evmng_getMyIntegerP(param_1, "msgNo2");
    if (pMsgNo2 != NULL) {
        msgNo2 = *pMsgNo2;
    }

    int* pTimer = dComIfGp_evmng_getMyIntegerP(param_1, "timer");
    if (pTimer != NULL) {
        timer = *pTimer;
    }
    fopAc_ac_c* pActors[2] = {this, mActors[0].getActorP()};
    dComIfGp_setMesgCameraInfoActor(pActors[0], pActors[1], 0, 0, 0, 0, 0, 0, 0, 0);
    if (dComIfGp_getEventManager().getIsAddvance(param_1)) {
        switch (prm) {
        case 0:
            mFaceMotionSeqMngr.setNo(0x1f, -1.0f, 0, 0);
            mMotionSeqMngr.setNo(0, -1.0f, 0, 0);
        case 1:
            mEventTimer = timer;
            break;
        case 2:
            daNpcT_getPlayerInfoFromPlayerList(6, fopAcM_GetRoomNo(this), &cStack_2c, &cStack_58);
            daPy_getPlayerActorClass()->setPlayerPosAndAngle(&cStack_2c, cStack_58.y, 0);
            cStack_2c.set(-656.0f, 1338.0f, -2237.0f);
            setPos(cStack_2c);
            setAngle(cM_deg2s(25.0f));
            break;
        case 3:
            initTalk(mFlowNodeNo, pActors);
            break;
        case 7:
            mFaceMotionSeqMngr.setNo(0x1f, 0.0f, 0, 0);
            mMotionSeqMngr.setNo(0, 0.0f, 0, 0);
            daNpcT_getPlayerInfoFromPlayerList(7, fopAcM_GetRoomNo(this), &cStack_2c, &cStack_58);
            daPy_getPlayerActorClass()->setPlayerPosAndAngle(&cStack_2c, cStack_58.y, 0);
            cStack_2c.set(-419.0f, 1375.0f, -2203.0f);
            setPos(cStack_2c);
            setAngle(cM_deg2s(15.0f));
            break;
        case 8:
            initTalk(mFlowNodeNo, pActors);
            break;
        }
    }

    int msgNos[3] = {-1, -1 , -1};

    switch (prm) {
    case 0:
    case 1:
        mJntAnm.lookPlayer(0);
        if (cLib_calcTimer(&mEventTimer) == 0) {
            rv = TRUE;
        }
        break;
    case 2:
        mJntAnm.lookPlayer(0);
        rv = TRUE;
        break;
    case 3:
    case 4:
    case 5:
    case 6:
    case 8: 
    {
        switch(prm) {
        case 5: 
        case 8: {
            fopAc_ac_c* actor_p = mActors[2].getActorP();
            JUT_ASSERT(4405, NULL != actor_p);
            mJntAnm.lookActor(actor_p, -40.0f, 0);
            break;
        }
        case 6: {
            fopAc_ac_c* actor_p = mActors[0].getActorP();
            JUT_ASSERT(4411, NULL != actor_p);
            mJntAnm.lookActor(actor_p, -40.0f, 0);
            break;
        }
        case 7:
        default:
            mJntAnm.lookPlayer(0);
            break;
        }
        msgNos[0] = msgNo;
        msgNos[1] = msgNo2;
        if (talkProc(msgNos, 0, pActors, 0) != 0) {
            if (msgNo == 0 && msgNo2 == 0) {
                int dummyEventId;
                if ((int)mFlow.getEventId(&dummyEventId) == 0x19) {
                    mEvtNo = 9;
                    evtChange();
                } else {
                    if (mFlow.checkEndFlow()) {
                        rv = TRUE;
                    }
                }
            } else {
                rv = TRUE;
            }
        }
        break;
    }
    case 7:
        mJntAnm.lookPlayer(0);
        rv = TRUE;
        break;
    }

    return rv;
}

int daNpc_Taro_c::cutGetWoodSwd(int param_1) {
    cXyz acStack_30;
    csXyz cStack_68;
    int rv = 0;
    int prm = -1;
    int msgNo = 0;
    int msgNo2 = 0;
    int timer = 0;
    int send = 0;

    int* pPrm = dComIfGp_evmng_getMyIntegerP(param_1, "prm");
    if (pPrm != NULL) {
        prm = *pPrm;
    }

    int* pMsgNo = dComIfGp_evmng_getMyIntegerP(param_1, "msgNo");
    if (pMsgNo != NULL) {
        msgNo = *pMsgNo;
    }

    int* pMsgNo2 = dComIfGp_evmng_getMyIntegerP(param_1, "msgNo2");
    if (pMsgNo2 != NULL) {
        msgNo2 = *pMsgNo2;
    }

    int* pTimer = dComIfGp_evmng_getMyIntegerP(param_1, "timer");
    if (pTimer != NULL) {
        timer = *pTimer;
    }
    
    int* pSend = dComIfGp_evmng_getMyIntegerP(param_1, "send");
    if (pSend != NULL) {
        send = *pSend;
    }

    fopAc_ac_c* pActors[2] = {this, mActors[0].getActorP()};
    dComIfGp_setMesgCameraInfoActor(pActors[0], pActors[1], 0, 0, 0, 0, 0, 0, 0, 0);
    if (dComIfGp_getEventManager().getIsAddvance(param_1)) {
        switch (prm) {
        case 0:
        case 1:
            mEventTimer = timer;
            break;
        case 2:
            acStack_30.set(-656.0f, 1338.0f, -2237.0f);
            setPos(acStack_30);
            setAngle(cM_deg2s(25.0f));
            acStack_30.set(0.0f, 0.0f, 200.0f);
            mDoMtx_stack_c::YrotS(mCurAngle.y);
            mDoMtx_stack_c::multVec(&acStack_30, &acStack_30);
            acStack_30 += current.pos;
            cStack_68.y = cLib_targetAngleY(&acStack_30, &current.pos);
            daPy_getPlayerActorClass()->setPlayerPosAndAngle(&acStack_30, cStack_68.y, 0);
            break;
        case 3:
            initTalk(mFlowNodeNo, pActors);
            break;
        case 11:
            mFaceMotionSeqMngr.setNo(9, 0.0f, 0, 0);
            mMotionSeqMngr.setNo(4, 0.0f, 0, 0);
            mJntAnm.lookNone(1);
            field_0x11a5 = 1;
            dMeter2Info_setSword(0xff, 0);
            mEventTimer = timer;
            initTalk(0x5a, NULL);
            break;
        case 12:
            mFaceMotionSeqMngr.setNo(0x1f, -1.0f, 0, 0);
            mMotionSeqMngr.setNo(0x26, -1.0f, 0, 0);
            home.pos = current.pos;
            home.angle.y = mCurAngle.y;
            mEventTimer = timer;
            break;
        case 13:
            mFaceMotionSeqMngr.setNo(0x1f, 0.0f, 0, 0);
            mMotionSeqMngr.setNo(0, 0.0f, 0, 0);
            speedF = 0.0f;
            speed.setall(0.0f);
            mHide = 1;
            break;
        case 21:
            mHide = 0;
            break;
        }
    }
    int msgNos[3] = {-1, -1, -1};

    switch (prm) {
    case 0:
    case 1:
        mJntAnm.lookPlayer(0);
        if (cLib_calcTimer(&mEventTimer) == 0) {
            rv = TRUE;
        }
        break;
    case 2:
        mJntAnm.lookPlayer(0);
        rv = TRUE;
        break;
    case 3:
        mJntAnm.lookNone(0);
        msgNos[0] = msgNo;
        msgNos[1] = msgNo2;
        if (talkProc(msgNos, 0, pActors, 0) != 0) {
            if (msgNo == 0 && msgNo2 == 0) {
                if (mFlow.checkEndFlow()) {
                    rv = TRUE;
                }
            } else {
                rv = TRUE;
            }
        }
        break;
    case 11:
        field_0xe26 = false;
        acStack_30.set(0.0f, 30.0f, 200.0f);
        mDoMtx_stack_c::YrotS(mCurAngle.y);
        mDoMtx_stack_c::multVec(&acStack_30, &acStack_30);
        field_0xd6c = acStack_30 + attention_info.position;
        mJntAnm.lookPos(&field_0xd6c, 0);
        if (mMotionSeqMngr.getStepNo() == 0) {
            if (mpMorf[0]->checkFrame(45.0f) || mpMorf[0]->checkFrame(63.0f)) {
                mSound.startCreatureVoice(Z2SE_M058_TARO_05, -1);
            }
        } else {
            send = 1;
        }
        msgNos[0] = msgNo;
        msgNos[1] = msgNo2;
        if (talkProc(msgNos, send, NULL, 0) != 0) {
            if (msgNo == 0 && msgNo2 == 0) {
                if (mFlow.checkEndFlow()) {
                    rv = TRUE;
                }
            } else {
                rv = TRUE;
            }
        }
        break;
    case 12:
        mJntAnm.lookNone(0);
        acStack_30.set(0.0f, 0.0f, 1600.0f);
        mDoMtx_stack_c::YrotS(home.angle.y + cM_deg2s(86.0f));
        mDoMtx_stack_c::multVec(&acStack_30, &acStack_30);
        acStack_30 += home.pos;
        calcSpeedAndAngle(acStack_30, current.pos.absXZ(acStack_30) < 50.0f, 4, 0x800);
        if (cLib_calcTimer(&mEventTimer) == 0) {
            rv = TRUE;
        }
        break;
    case 13:
        mJntAnm.lookNone(0);
        break;
    case 14:
    case 15:
        if (prm == 14) {
            mDoMtx_stack_c::copy(mpMorf[0]->getModel()->getAnmMtx(15));
            mDoMtx_stack_c::multVecZero(&field_0xd6c);
            mJntAnm.lookPos(&field_0xd6c, 0);
        } else {
            mJntAnm.lookNone(0);
        }
        msgNos[0] = msgNo;
        msgNos[1] = msgNo2;
        if (talkProc(msgNos, send, NULL, 0) != 0) {
            if (msgNo == 0 && msgNo2 == 0) {
                if (mFlow.checkEndFlow() != 0) {
                    rv = TRUE;
                }
            } else {
                rv = TRUE;
            }
        }
        break;
    case 20:
    case 21:
        action();
        rv = TRUE;
        break;
    }
    return rv;
}

int daNpc_Taro_c::cutConversationWithMaro(int param_1) {
    int rv = FALSE;
    int prm = -1;
    int msgNo = 0;
    int msgNo2 = 0;

    int* pPrm = dComIfGp_evmng_getMyIntegerP(param_1, "prm");
    if (pPrm != NULL) {
        prm = *pPrm;
    }

    int* pMsgNo = dComIfGp_evmng_getMyIntegerP(param_1, "msgNo");
    if (pMsgNo != NULL) {
        msgNo = *pMsgNo;
    }

    int* pMsgNo2 = dComIfGp_evmng_getMyIntegerP(param_1, "msgNo2");
    if (pMsgNo2 != NULL) {
        msgNo2 = *pMsgNo2;
    }

    fopAc_ac_c* pActors[2] = {this, mActors[0].getActorP() };
    dComIfGp_setMesgCameraInfoActor(pActors[0], pActors[1], 0, 0, 0, 0, 0, 0, 0, 0);
    if (dComIfGp_getEventManager().getIsAddvance(param_1)) {
        switch (prm) {
        case 0:
            initTalk(mFlowNodeNo, pActors);
            mPlayerAngle = fopAcM_searchActorAngleY(this, pActors[1]);
            break;
        case 4:
            initTalk(mFlowNodeNo, pActors);
            mPlayerAngle = fopAcM_searchActorAngleY(this, pActors[1]);
            break;
        }
    }

    int msgNos[3] = {-1, -1, -1};

    switch(prm) {
    case 0:
        mJntAnm.lookActor(pActors[1], -40.0f, 0);
        if (mPlayerAngle == mCurAngle.y) {
            rv = TRUE;
        } else {
            msgNo2 = step(mPlayerAngle, -1, 0x24, 0xf, 0);
            if (msgNo2 != 0) {
                mMotionSeqMngr.setNo(0x19, -1.0f, 0, 0);
            }
        }
        shape_angle.y = mPlayerAngle;
        break;
    case 1:
    case 2:
    case 3:
    case 4:
        if (prm == 2 || prm == 3 || prm == 4) {
            mJntAnm.lookNone(0);
            if (home.angle.y == mCurAngle.y) {
                if (prm == 3) {
                    rv = TRUE;
                }
            } else {
                if (step(home.angle.y, -1, 0x24, 0xf, 0) != 0) {
                    mMotionSeqMngr.setNo(0x19, -1.0f, 0, 0);
                }
            }
        } else {
            mJntAnm.lookActor(pActors[1], -40.0f, 0);
        }

        if (prm != 3) {
            msgNos[0] = msgNo;
            msgNos[1] = msgNo2;
            if (talkProc(msgNos, 0, pActors, 0) != 0) {
                if (msgNo == 0 && msgNo2 == 0) {
                    if (mFlow.checkEndFlow()) {
                        rv = TRUE;
                    }
                } else {
                    rv = TRUE;
                }
            }
        }
        break;
    }

    return rv;
}

int daNpc_Taro_c::cutCacaricoConversation(int param_1) {
    int rv = FALSE;
    int prm = -1;
    int timer = 0;
    int msgNo = 0;
    int* pPrm = dComIfGp_evmng_getMyIntegerP(param_1, "prm");
    if (pPrm != NULL) {
        prm = *pPrm;
    }

    int* pTimer = dComIfGp_evmng_getMyIntegerP(param_1, "timer");
    if (pTimer != NULL) {
        timer = *pTimer;
    }

    int* piVar2 = dComIfGp_evmng_getMyIntegerP(param_1, "msgNo");
    if (piVar2 != NULL) {
        msgNo = *piVar2;
    }

    fopAc_ac_c* pActors[4] = {this, mActors[1].getActorP(), mActors[2].getActorP(),
                              mActors[0].getActorP()};
    dComIfGp_setMesgCameraInfoActor(pActors[0], pActors[1], pActors[2], pActors[3], 0, 0, 0, 0, 0,
                                    0);
    if (dComIfGp_getEventManager().getIsAddvance(param_1)) {
        switch(prm) {
        case 0:
            initTalk(mFlowNodeNo, pActors);
            mEventTimer = timer;
            break;
        case 1:
            break;
        }
    }

    int msgNos[2] = {-1, -1};
    
    switch (prm) {
    case 0:
        mJntAnm.lookPlayer(0);
        if (mPlayerAngle != mCurAngle.y) {
            if (step(mPlayerAngle, 0x1f, 0x24, 0xf, 0) != 0) {
                mFaceMotionSeqMngr.setNo(0x1f, -1.0f, 0, 0);
                mMotionSeqMngr.setNo(0, -1.0f, 0, 0);
            }
        } else if (cLib_calcTimer(&mEventTimer) == 0) {
            rv = TRUE;
        }
        break;
    case 1:
        mJntAnm.lookPlayer(0);
        msgNos[0] = msgNo;
        if (talkProc(msgNos, 0, pActors, 0) != 0) {
            if (msgNo == 0) {
                if (mFlow.checkEndFlow()) {
                    rv = TRUE;
                }
            } else {
                rv = TRUE;
            }
        }
        break;
    }

    return rv;
}

int daNpc_Taro_c::cutArrowTutorial(int param_1) {
    cXyz cStack_40;
    csXyz cStack_48;
    int rv = FALSE;
    int prm = -1;
    int timer = 0;

    int* pPrm = dComIfGp_evmng_getMyIntegerP(param_1, "prm");
    if (pPrm != NULL) {
        prm = *pPrm;
    }

    int* pTimer = dComIfGp_evmng_getMyIntegerP(param_1, "timer");
    if (pTimer != NULL) {
        timer = *pTimer;
    }

    if (dComIfGp_getEventManager().getIsAddvance(param_1)) {
        switch (prm) {
        case 0:
            mPlayerActorMngr.entry(daPy_getPlayerActorClass());
            break;
        case 1:
            mFaceMotionSeqMngr.setNo(0x1f, 0.0f, 0, 0);
            mMotionSeqMngr.setNo(0, 0.0f, 0, 0);
            setAngle(home.angle.y);
            if (daNpcT_getPlayerInfoFromPlayerList(0x42, current.roomNo, &cStack_40, &cStack_48) !=
                0)
            {
                daPy_getPlayerActorClass()->setPlayerPosAndAngle(&cStack_40, cStack_48.y, 0);
            }

            if (dComIfGp_getHorseActor() != NULL) {
                daHorse_c* horse = dComIfGp_getHorseActor();
                cStack_40 = horse->home.pos;
                horse->setHorsePosAndAngle(&cStack_40, horse->home.angle.y);
            }
            Z2GetAudioMgr()->subBgmStart(Z2BGM_EVENT01);
            daNpcT_onTmpBit(0x3c);
            daNpcT_offTmpBit(0x3f);
            daNpcT_offTmpBit(0x40);
            daNpcT_offTmpBit(0x41);
            break;
        case 3:
            mFaceMotionSeqMngr.setNo(0x1f, 0.0f, 0, 0);
            mMotionSeqMngr.setNo(0, 0.0f, 0, 0);
            setAngle(home.angle.y);

            if (prm == 3) {
                mActors[19].entry(getArrowP());
                mJntAnm.lookNone(1);
            } else {
                mJntAnm.lookNone(0);
            }
            break;
        case 5:
            mFaceMotionSeqMngr.setNo(0x1f, 0.0f, 0, 0);
            mMotionSeqMngr.setNo(0, 0.0f, 0, 0);
            mEventTimer = timer;
            if (!daNpcT_chkEvtBit(0x4c)) {
                camera_class* pCamera = dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0));
                mJntAnm.lookCamera(0);
                mPlayerAngle = cLib_targetAngleY(fopAcM_GetPosition_p(this), fopCamM_GetEye_p(pCamera));
            } else {
                mPlayerAngle = fopAcM_searchPlayerAngleY(this);
            }

            if (checkStep()) {
                mStepMode = 0;
            }
            setAngle(home.angle.y);
            mPlayerActorMngr.entry(daPy_getPlayerActorClass());
            break;
        case 6:
            setAngle(home.angle.y);
            mJntAnm.lookNone(1);
            break;
        case 10:
            mEventTimer = timer;
            break;
        case 7:
            mFaceMotionSeqMngr.setNo(0x1f, 0.0f, 0, 0);
            mMotionSeqMngr.setNo(0, 0.0f, 0, 0);
            mEventTimer = timer;
            setAngle(home.angle.y);
            mPlayerActorMngr.entry(daPy_getPlayerActorClass());
            break;
        }
    }
    switch (prm) {
    case 0:
    case 1:
    case 2:
        action();
        rv = TRUE;
        break;
    case 3:
    case 4:
        if (!dComIfGp_getEventManager().getIsAddvance(param_1)) {
            fopAc_ac_c* actor_p = mActors[19].getActorP();
            if (actor_p != NULL) {
                field_0xd6c = actor_p->attention_info.position;
            }
            mJntAnm.lookPos(&field_0xd6c, 0);
        }
        rv = TRUE;
        break;
    case 5:
        if (daNpcT_chkEvtBit(0x4c)) {
            mJntAnm.lookPlayer(0);
        }
        if (cLib_calcTimer(&mEventTimer) == 0) {
            if (mPlayerAngle != mCurAngle.y) {
                step(mPlayerAngle, 0x1f, 0x24, 0xf, 0);
            } else {
                rv = TRUE;
            }
        }
        break;
    case 6:
        rv = TRUE;
        break;
    case 10:
        if (cLib_calcTimer(&mEventTimer) == 0) {
            rv = TRUE;
        }
        break;
    case 7:
        if (cLib_calcTimer(&mEventTimer) == 0) {
            if (!daNpcT_chkEvtBit(0x4c)) {
                camera_class* pCamera = dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0));
                mJntAnm.lookCamera(0);
                cStack_48.y = cLib_targetAngleY(fopAcM_GetPosition_p(this), fopCamM_GetEye_p(pCamera));
            } else {
                mJntAnm.lookPlayer(0);
                cStack_48.y = fopAcM_searchPlayerAngleY(this);
            }

            if (cStack_48.y != mCurAngle.y) {
                step(cStack_48.y, 0x1f, 0x24, 0xf, 0);
            } else {
                rv = TRUE;
            }
        }
        break;
    }
    return rv;
}

int daNpc_Taro_c::cutCaution(int param_1) {
    cXyz cStack_30;
    csXyz cStack_38;
    int rv = FALSE;
    int prm = -1;
    int timer = 0;

    int* pPrm = dComIfGp_evmng_getMyIntegerP(param_1, "prm");
    if (pPrm != NULL) {
        prm = *pPrm;
    }

    int* pTimer = dComIfGp_evmng_getMyIntegerP(param_1, "timer");
    if (pTimer != NULL) {
        timer = *pTimer;
    }

    if (dComIfGp_getEventManager().getIsAddvance(param_1)) {
        switch (prm) {
        case 0:
            daNpcT_offTmpBit(0xb);
            mFaceMotionSeqMngr.setNo(0x1f, 0.0f, 1, 0);
            mMotionSeqMngr.setNo(5, 0.0f, 1, 0);
            mJntAnm.lookNone(0);
            break;
        case 1:
            mFindCount = 0;
            fopAcM_Search(srchNpc, this);
            for (int i = 0; i < mFindCount; i++) {
                if (mFindActorPtrs[i] != this) {
                    fopAcM_delete(mFindActorPtrs[i]);
                }
            }
            mHide = 1;
            break;
        case 2:
            mEventTimer = timer;
            break;
        case 3:
            initTalk(0x24, NULL);
            break;
        }
    }

    switch (prm) {
    case 0:
    case 1:
        rv = TRUE;
        break;
    case 2:
        if (cLib_calcTimer(&mEventTimer) == 0) {
            rv = TRUE;
        }
        break;
    case 3:
        if (talkProc(NULL, 0, NULL, 0) != 0 && mFlow.checkEndFlow()) {
            rv = TRUE;
        }
        break;
    }
   
    return rv;
}

int daNpc_Taro_c::cutTagPush1(int param_1) {
    daTag_Push_c* pushTag = (daTag_Push_c*)field_0xba0.getActorP();
    int rv = 0;
    int prm = -1;

    int* pPrm = dComIfGp_evmng_getMyIntegerP(param_1, "prm");
    if (pPrm != NULL) {
        prm = *pPrm;
    }

    if (dComIfGp_getEventManager().getIsAddvance(param_1)) {
        switch (prm) {
        case 0:
            pushTag->pushBackPlayer(0);
            break;
        case 1:
            mFaceMotionSeqMngr.setNo(0x1f, 0.0f, 0, 0);
            mMotionSeqMngr.setNo(0, 0.0f, 0, 0);
            mJntAnm.lookNone(1);
            initTalk(pushTag->getFlowNodeNo(), NULL);
            setAngle(fopAcM_searchPlayerAngleY(this));
            break;
        }
    }

    switch (prm) {
    case 0:
        action();
        rv = TRUE;
        break;
    case 1:
        if (talkProc(NULL, 0, NULL, 0) != 0) {
            int dummyId;
            if (mFlow.getEventId(&dummyId) == 2) {
                dComIfGp_setNextStage("F_SP103", 0x14, 1, 8);
            }
            field_0x11a0 = 1;
            field_0xba0.remove();
            rv = TRUE;
        }
        break;
    default:
        rv = TRUE;
        break;
    }

    return rv;
}

int daNpc_Taro_c::cutNotGonnaLet(int param_1) {
    cXyz cStack_30;
    csXyz cStack_50;
    int rv = 0;
    int prm = -1;
    int msgNo = 0;
    int* pPrm = dComIfGp_evmng_getMyIntegerP(param_1, "prm");
    if (pPrm != NULL) {
        prm = *pPrm;
    }

    int* pMsgNo = dComIfGp_evmng_getMyIntegerP(param_1, "msgNo");
    if (pMsgNo != NULL) {
        msgNo = *pMsgNo;
    }

    fopAc_ac_c* pActors[2] = {this, mActors[0].getActorP()};
    dComIfGp_setMesgCameraInfoActor(pActors[0], pActors[1], 0, 0, 0, 0, 0, 0, 0, 0);
    if (dComIfGp_getEventManager().getIsAddvance(param_1)) {
        switch (prm) {
        case 0:
            field_0x11a0 = 1;
            break;
        case 1:
            mFaceMotionSeqMngr.setNo(0x1f, 0.0f, 0, 0);
            mMotionSeqMngr.setNo(0, 0.0f, 0, 0);
            daNpcT_getPlayerInfoFromPlayerList(6, fopAcM_GetRoomNo(this), &cStack_30, &cStack_50);
            daPy_getPlayerActorClass()->setPlayerPosAndAngle(&cStack_30, cStack_50.y, 0);
            dComIfGp_evmng_setGoal(&cStack_30);
            cStack_30.set(-656.0f, 1338.0f, -2237.0f);
            setPos(cStack_30);
            setAngle(cM_deg2s(25.0f));
            break;
        case 40:
            mJntAnm.lookPlayer(1);
            break;
        case 2:
            initTalk(mFlowNodeNo, pActors);
            break;
        case 3: {
            fopAc_ac_c* actor_p = mActors[2].getActorP();
            JUT_ASSERT(5445, NULL != actor_p);
            mJntAnm.lookActor(actor_p, -40.0f, 0);
            break;
        }
        case 4:
            mJntAnm.lookPlayer(0);
            break;
        case 5: {
            fopAc_ac_c* actor_p = mActors[2].getActorP();
            JUT_ASSERT(5456, NULL != actor_p);
            mJntAnm.lookActor(actor_p, -40.0f, 0);
            break;
        }
        case 6: {
            fopAc_ac_c* actor_p = mActors[0].getActorP();
            JUT_ASSERT(5464, NULL != actor_p);
            mJntAnm.lookActor(actor_p, -40.0f, 0);
            dComIfGp_getEvent().setPt2(actor_p);
            break;
        }
        case 7: {
            fopAc_ac_c* actor_p = mActors[2].getActorP();
            JUT_ASSERT(5473, NULL != actor_p);
            mJntAnm.lookActor(actor_p, -40.0f, 0);
            dComIfGp_getEvent().setPt2(this);
            break;
        }
        case 9:
            mJntAnm.lookPlayer(0);
            break;
        case 10:
            mFaceMotionSeqMngr.setNo(0x1f, -1.0f, 0, 0);
            mMotionSeqMngr.setNo(0, -1.0f, 0, 0);
            mJntAnm.lookPlayer(0);
            break;
        case 11:
            mFaceMotionSeqMngr.setNo(0x1f, 0.0f, 0, 0);
            mMotionSeqMngr.setNo(0, 0.0f, 1, 0);
            mJntAnm.lookPlayer(0);
            field_0x11a0 = 0;
            daNpcT_getPlayerInfoFromPlayerList(7, fopAcM_GetRoomNo(this), &cStack_30, &cStack_50);
            daPy_getPlayerActorClass()->setPlayerPosAndAngle(&cStack_30, cStack_50.y, 0);
            dComIfGp_evmng_setGoal(&cStack_30);
            cStack_30.set(-419.0f, 1375.0f, -2203.0f);
            setPos(cStack_30);
            setAngle(cM_deg2s(15.0f));
            break;
        case 20: {
            fopAc_ac_c* actor_p = mActors[2].getActorP();
            JUT_ASSERT(5508, NULL != actor_p);
            mJntAnm.lookActor(actor_p, -40.0f, 0);
            initTalk(mFlowNodeNo, pActors);
            break;
        }
        case 21: {
            fopAc_ac_c* actor_p = mActors[0].getActorP();
            JUT_ASSERT(5517, NULL != actor_p);
            dComIfGp_getEvent().setPt2(actor_p);
            break;
        }
        case 22:
            mJntAnm.lookPlayer(0);
            dComIfGp_getEvent().setPt2(this);
            break;
        case 30:
            daNpcT_getPlayerInfoFromPlayerList(7, fopAcM_GetRoomNo(this), &cStack_30, &cStack_50);
            daPy_getPlayerActorClass()->setPlayerPosAndAngle(&cStack_30, cStack_50.y, 0);
            dComIfGp_evmng_setGoal(&cStack_30);
            break;
        case 31:
            mFaceMotionSeqMngr.setNo(0x1f, 0.0f, 0, 0);
            mMotionSeqMngr.setNo(0, 0.0f, 0, 0);
            mJntAnm.lookPlayer(1);
            initTalk(mFlowNodeNo, pActors);
            setAngle(home.angle.y);
            break;
        }
    }

    int msgNos[2] = {-1, -1};
    switch (prm) {
    case 0:
    case 0x1e:
        action();
        rv = TRUE;
        break;
    case 1:
    case 0x28:
        rv = TRUE;
        break;
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 20:
    case 21:
        msgNos[0] = msgNo;
    case 9:
    case 22:
        if (talkProc(msgNos, 1, pActors, 0) != 0) {
            rv = TRUE;
        }
        break;
    case 10:
    case 11:
        rv = TRUE;
        break;
    case 31:
        if (talkProc(NULL, 1, pActors, 0) != 0) {
            rv = TRUE;
        }
        break;
    default:
        rv = TRUE;
        break;
    }
    return rv;
}

int daNpc_Taro_c::cutTagPush4(int param_1) {
    daTag_Push_c* pushTag = (daTag_Push_c*)field_0xba0.getActorP();
    int rv = FALSE;
    int prm = -1;

    int* pPrm = dComIfGp_evmng_getMyIntegerP(param_1, "prm");
    if (pPrm != NULL) {
        prm = *pPrm;
    }

    if (dComIfGp_getEventManager().getIsAddvance(param_1)) {
        switch (prm) {
        case 0:
            mFaceMotionSeqMngr.setNo(0x1f, -1.0f, 0, 0);
            mMotionSeqMngr.setNo(0, -1.0f, 0, 0);
            mJntAnm.lookNone(1);
            pushTag->pushBackPlayer(0);
            setAngle(fopAcM_searchPlayerAngleY(this));
            mStagger.initialize();
            mDamageTimer = 0;
            break;
        case 1:
            initTalk(pushTag->getFlowNodeNo(), NULL);
            break;
        }
    }

    switch (prm) {
    case 0:
        rv = TRUE;
        break;
    case 1:
        if (talkProc(NULL, 0, NULL, 0) != 0) {
            field_0x11a0 = 1;
            field_0xba0.remove();
            rv = TRUE;
        }
        break;
    default:
        rv = TRUE;
        break;
    }

    return rv;
}

int daNpc_Taro_c::wait(void* param_0) {
    fopAc_ac_c* actor_p;
    int iVar10 = daNpc_Taro_Param_c::m.field_0x8e;
    s16 local_5e = home.angle.y;

    switch (mMode) {
    case MODE_ENTER:
    case MODE_INIT:
        if (!mStagger.checkStagger()) {
            switch (mType) {
            case TYPE_1:
            case TYPE_2:
                mFaceMotionSeqMngr.setNo(0x1f, -1.0, 0, 0);
                mMotionSeqMngr.setNo(0x19, -1.0, 0, 0);
                field_0x119c = cLib_getRndValue(iVar10 / 2, iVar10);
                break;
            case TYPE_3:
                mFaceMotionSeqMngr.setNo(0x19, -1.0f, 0, 0);
                mMotionSeqMngr.setNo(0x23, -1.0f, 0, 0);
                break;
            case TYPE_6:
                mFaceMotionSeqMngr.setNo(0x18, -1.0f, 0, 0);
                mMotionSeqMngr.setNo(0x13, -1.0f, 0, 0);
                break;
            case TYPE_13:
                if (field_0x11a2 != 0) {
                    mFaceMotionSeqMngr.setNo(0x1a, -1.0f, 0, 0);
                    mMotionSeqMngr.setNo(0x15, -1.0f, 0, 0);
                } else {
                    mFaceMotionSeqMngr.setNo(0x1d, -1.0f, 0, 0);
                    mMotionSeqMngr.setNo(0x1a, -1.0f, 0, 0);
                }
                break;
            case TYPE_8:
                if (mPlayerActorMngr.getActorP() != NULL) {
                    mFaceMotionSeqMngr.setNo(0x1f, -1.0f, 0, 0);
                    mMotionSeqMngr.setNo(0, -1.0f, 0, 0);
                } else {
                    mFaceMotionSeqMngr.setNo(0x1f, -1.0f, 0, 0);
                    mMotionSeqMngr.setNo(5, -1.0f, 0, 0);
                    field_0x119c = cLib_getRndValue(iVar10 / 2, iVar10);
                }
                break;
            case TYPE_9:
                mFaceMotionSeqMngr.setNo(0x1f, -1.0f, 0, 0);
                mMotionSeqMngr.setNo(0x19, -1.0f, 0, 0);
                break;
            default:
                mFaceMotionSeqMngr.setNo(0x1f, -1.0f, 0, 0);
                mMotionSeqMngr.setNo(0, -1.0f, 0, 0);
                break;
            }
            mMode = MODE_RUN;
        }
    case MODE_RUN:
        switch (mType) {
        case TYPE_0:
            if (!daNpcT_chkEvtBit(0x1f)) {
                daTag_EvtArea_c* EvtAreaTag = (daTag_EvtArea_c*)mActors[3].getActorP();
                if (EvtAreaTag != NULL && EvtAreaTag->chkPointInArea(daPy_getPlayerActorClass()->current.pos)) {
                    if (daNpcT_chkEvtBit(0x23b)) {
                        mEvtNo = 8;
                    } else {
                        mEvtNo = 7;
                    }
                }
            } else if (mHide) {
                fopAcM_delete(this);
                return 1;
            }
            break;
        case TYPE_3: {
            daNpc_Len_c* pLen = (daNpc_Len_c*)mActors[22].getActorP();
            if (pLen != NULL &&
                pLen->checkStartDemo13StbEvt(
                    this, daNpc_Taro_Param_c::m.field_0x70, daNpc_Taro_Param_c::m.field_0x74,
                    daNpc_Taro_Param_c::m.field_0x78, daNpc_Taro_Param_c::m.field_0x7c,
                    daNpc_Taro_Param_c::m.field_0x80, daNpc_Taro_Param_c::m.field_0x84,
                    daNpc_Taro_Param_c::m.field_0x88))
            {
                mEvtNo = 12;
                field_0x11a1 = 1;
            }
            if (field_0x11a1 != 0 && daNpcT_chkEvtBit(0x3c) && !dComIfGp_event_runCheck()) {
                field_0x11a1 = 0;
            }
            break;
        }
        case TYPE_8:
            if (daNpcT_chkTmpBit(0x3c)) {
                local_5e = fopAcM_searchPlayerAngleY(this);
            }
            if (daPy_py_c::checkNowWolf() && mHide == 0) {
                if (daPy_getPlayerActorClass()->eventInfo.mCondition & 1) {
                    for (int i = 0; i < 16; i++) {
                        daTag_EvtArea_c* evtAreaTag = (daTag_EvtArea_c*)mActors[i+3].getActorP();

                        if (evtAreaTag != NULL && evtAreaTag->chkPointInArea(daPy_getPlayerActorClass()->current.pos)) {
                            mEvtNo = 0x15;
                            break;
                        }
                    }
                }
            }
            break;
        case TYPE_12:
            if (!daNpcT_chkEvtBit(0x25c)) {
                mEvtNo = 3;
                field_0xe33 = 1;
            } else {
                fopAcM_delete(this);
                return 1;
            }
            break;
        case TYPE_13: {
            daTag_EvtArea_c* evtAreaTag = (daTag_EvtArea_c*)mActors[3].getActorP();

            if (evtAreaTag != NULL &&
                evtAreaTag->chkPointInArea(daPy_getPlayerActorClass()->current.pos) &&
                !daNpcT_chkEvtBit(0x25d))
            {
                mEvtNo = 4;
            }
            if (getBitSW() != 0xff) {
                if (dComIfGs_isSwitch(getBitSW(), fopAcM_GetRoomNo(this)) && getBitSW2() != 0xff) {
                    if (dComIfGs_isSwitch(getBitSW2(), fopAcM_GetRoomNo(this))) {
                        mEvtNo = 5;
                    }
                }
            }
            mSound.startCreatureVoiceLevel(Z2SE_TARO_V_CAGE_CRY_LOOP, -1);
            break;
        }
        case TYPE_14:
            if (daNpcT_chkEvtBit(0xb8)) {
                if (mHide) {
                    fopAcM_delete(this);
                    return 1;
                }
            } else {
                mEvtNo = 6;
                field_0xe33 = 1;
            }
            break;
        }
        if (!mStagger.checkStagger()) {
            if (mType == TYPE_3) {
                mJntAnm.lookNone(0);
                attention_info.flags = 0;
            } else {
                u8 local_5f = 0;
                switch (mType) {
                case TYPE_1:
                case TYPE_2:
                case TYPE_9:
                    mPlayerActorMngr.remove();
                    break;
                case TYPE_4:
                case TYPE_5:
                    mPlayerActorMngr.remove();
                    if (dComIfGs_isTmpBit(0xd02)) {
                        mPlayerActorMngr.entry(daPy_getPlayerActorClass());
                    }
                    mJntAnm.lookNone(0);
                    break;
                case TYPE_6:
                case TYPE_10:
                    mPlayerActorMngr.remove();
                    break;
                case TYPE_11:
                    local_5f = dComIfGs_getTmpReg(0xfbff);
                    if (local_5f == 2 || local_5f == 1) {
                        mPlayerActorMngr.entry(daPy_getPlayerActorClass());
                    } else if (local_5f == 0) {
                        mPlayerActorMngr.remove();
                    }
                    break;
                case TYPE_13:
                    mPlayerActorMngr.remove();
                    break;
                case TYPE_8:
                    if (daNpcT_chkEvtBit(0x49)) {
                        mPlayerActorMngr.remove();
                    }
                    break;
                }
                if ((mPlayerActorMngr.getActorP() != NULL || field_0x11a0 != 0) && !mTwilight) {
                    if (mType != TYPE_4 && mType != TYPE_5) {
                        mJntAnm.lookPlayer(0);
                        if (mType == TYPE_11) {
                            if (local_5f == 1) {
                                mJntAnm.lookNone(0);
                            }
                        } else {
                            if (!chkActorInSight(mPlayerActorMngr.getActorP(), mAttnFovY, mCurAngle.y) && field_0x11a0 == 0) {
                                mJntAnm.lookNone(0);
                                if (!srchPlayerActor()) {
                                    if (local_5e == mCurAngle.y) {
                                        mMode = MODE_INIT;
                                    }
                                    field_0x11a0 = 0;
                                } else if (mType == TYPE_8 && mMotionSeqMngr.getNo() != 0)
                                {
                                    mFaceMotionSeqMngr.setNo(0x1f, -1.0f, 0, 0);
                                    mMotionSeqMngr.setNo(0, -1.0f, 0, 0);
                                }
                            }
                        }
                    }
                } else {
                    mJntAnm.lookNone(0);
                    if (local_5e != mCurAngle.y) {
                        if (field_0xe34 != 0) {
                            if (step(local_5e, 0x1f, 0x24, 0xf, 0) != 0) {
                                mMode = MODE_INIT;
                            }
                        } else {
                            setAngle(local_5e);
                            mMode = MODE_INIT;
                        }
                        attention_info.flags = 0;
                    } else {
                        switch (mType) {
                        case TYPE_1:
                        case TYPE_2:
                            if (field_0x119c == 0) {
                                if ((mMotionSeqMngr.getNo() == 0x1f ||
                                    mMotionSeqMngr.getNo() == 0x20) &&
                                    mMotionSeqMngr.getStepNo() > 0)
                                {
                                    mFaceMotionSeqMngr.setNo(0x1f, -1.0f, 0, 0);
                                    mMotionSeqMngr.setNo(0x19, -1.0f, 0, 0);
                                    field_0x119c = cLib_getRndValue(iVar10 / 2, iVar10);
                                }
                            } else {
                                field_0x119c--;
                                if (field_0x119c > 0) {
                                    mFaceMotionSeqMngr.setNo(0x1f, -1.0f, 0, 0);
                                    mMotionSeqMngr.setNo(0x19, -1.0f, 0, 0);
                                } else {
                                    mFaceMotionSeqMngr.setNo(0x10, -1.0f, 0, 0);
                                    if (0.5f < cM_rnd()) {
                                        iVar10 = 0x1f;
                                    } else {
                                        iVar10 = 0x20;
                                    }
                                    mMotionSeqMngr.setNo(iVar10, -1.0, 0, 0);
                                }
                            }
                            break;
                        case TYPE_8:
                            if (field_0x119c != 0) {
                                if (cLib_calcTimer(&field_0x119c) == 0 && !daNpcT_chkTmpBit(0x3c)) {
                                    mFaceMotionSeqMngr.setNo(0x17, -1.0f, 0, 0);
                                    mMotionSeqMngr.setNo(0x22, -1.0f, 0, 0);
                                }
                            } else if (daNpcT_chkTmpBit(0x3c) || mMotionSeqMngr.checkEndSequence()) {
                                mMode = MODE_INIT;
                            }
                            break;
                        case TYPE_4:
                        case TYPE_5:
                        case TYPE_9:
                            break;
                        default:
                            if (!mTwilight) {
                                srchPlayerActor();
                            }
                            break;
                        }
                    }
                }
            }

            switch (mJntAnm.getMode()) {
            case daNpcT_JntAnm_c::LOOK_NONE:
                switch (mType) {
                case TYPE_4:
                case TYPE_5:
                    if (dComIfGs_isTmpBit(0xE40))
                    {
                        mJntAnm.lookPlayer(0);
                    }
                    break;
                case TYPE_10:
                    if (daNpcT_chkTmpBit(0x64)) {
                        field_0x11a7 = 1;
                        home.angle.y = 0x8000;
                    }
                    if (field_0x11a7 != 0) {
                        mJntAnm.lookCamera(0);
                        msg_class* local_48 = dMsgObject_c::getActor();
                        if (local_48 != NULL && local_48->mode == 6) {
                            mFaceMotionSeqMngr.setNo(6, -1.0f, 0, 0);
                            mMotionSeqMngr.setNo(2, -1.0f, 0, 0);
                        }
                    } else {
                        actor_p = mActors[1].getActorP();
                        if (actor_p != NULL) {
                            mJntAnm.lookActor(actor_p, -40.0f, 0);
                        }
                    }
                    break;
                }
            }

            switch (mType) {
            case TYPE_4:
            case TYPE_5:
            case TYPE_10:
            case TYPE_11:
                attention_info.flags = 0;
                break;
            case TYPE_13:
                break;
            }
        }
        break;
    case MODE_EXIT:
        break;
    }
    return 1;
}

int daNpc_Taro_c::swdTutorial(void* param_0) {
    switch (mMode) {
    case MODE_ENTER:
    case MODE_INIT:
        if (!mStagger.checkStagger()) {
            mFaceMotionSeqMngr.setNo(0x1f, -1.0, 0, 0);
            mMotionSeqMngr.setNo(0xb, -1.0, 0, 0);
            mMode = MODE_RUN;
        }
    case MODE_RUN:
        if (!mStagger.checkStagger()) {
            mJntAnm.lookPlayer(0);
            if (home.angle.y != mCurAngle.y) {
                if (field_0xe34 != 0) {
                    if (step(home.angle.y, 0x1f, 0x24, 0xf, 0) != 0) {
                        mMode = 1;
                    }
                } else {
                    setAngle(home.angle.y);
                    mMode = 1;
                }
            }
        }
        attention_info.flags = 0;
        break;
    case MODE_EXIT:
        break;
    }

    return 1;
}

int daNpc_Taro_c::talk_withMaro(void* param_0) {
    daNpc_Maro_c* pMaro = (daNpc_Maro_c*)mActors[0].getActorP();
    fopAc_ac_c* player = daPy_getPlayerActorClass();
    int choccaiTimer = daNpc_Taro_Param_c::m.mChoccaiTimer;

    switch (mMode) {
    case MODE_ENTER:
    case MODE_INIT:
        mFaceMotionSeqMngr.setNo(0x1f, -1.0f, 0, 0);
        mMotionSeqMngr.setNo(0, -1.0f, 0, 0);
        mChoccaiTimer = cLib_getRndValue(choccaiTimer / 2, choccaiTimer);
        mMode = MODE_RUN;
    case MODE_RUN:
        if (!mStagger.checkStagger()) {
            mJntAnm.lookNone(0);
            if (pMaro != NULL) {
                if (field_0x11a0 != 0 ||
                    srchPlayerActor() ||
                    pMaro->srchPlayerActor())
                {
                    pMaro->findPlayer();
                    pMaro->endChoccai();
                    mChoccaiTimer = cLib_getRndValue(choccaiTimer / 2, choccaiTimer);
                    mJntAnm.lookPlayer(0);
                } else {
                    pMaro->lostPlayer();
                    if (pMaro->checkNowMotionIsChoccai()) {
                        if (pMaro->checkEndMotionIsChoccai()) {
                            pMaro->endChoccai();
                            mChoccaiTimer = cLib_getRndValue(choccaiTimer / 2, choccaiTimer);
                        }
                        mJntAnm.lookActor(pMaro, -40.0f, 0);
                    } else {
                        if (cLib_calcTimer(&mChoccaiTimer) == 0) {
                            pMaro->startChoccai();
                        }
                        mJntAnm.lookNone(0);
                    }
                }
            }
            if (home.angle.y != mCurAngle.y && (step(home.angle.y, 0x1f, 0x24, 0xf, 0) != 0)) {
                mMode = MODE_INIT;
            }
            daTag_EvtArea_c* evtAreaTag = (daTag_EvtArea_c*)mActors[3].getActorP();
            if (evtAreaTag != NULL && evtAreaTag->chkPointInArea(player->current.pos) && daNpcT_chkTmpBit(0x16)) {
                enum TaroEvent evtNo;
                if (daNpcT_chkEvtBit(0x13)) {
                    evtNo = EVENT_18;
                } else {
                    evtNo = EVENT_17;
                }
                mEvtNo = evtNo;
            }
        }
        break;
    case MODE_EXIT:
        break;
    }
    return 1;
}

int daNpc_Taro_c::practice(void* param_0) {
    int iVar4 = daNpc_Taro_Param_c::m.field_0x8e;
    int iVar3 = 0;

    switch (mMode) {
    case MODE_ENTER:
    case MODE_INIT:
        mFaceMotionSeqMngr.setNo(0x1f, -1.0f, 0, 0);
        mMotionSeqMngr.setNo(0x19, -1.0f, 0, 0);
        field_0x119c = cLib_getRndValue(iVar4 / 2, iVar4);
        mMode = MODE_RUN;
    case MODE_RUN:
        if (!mStagger.checkStagger()) {
            if (field_0xe34 == 0) {
                field_0x11a0 = 0;
                if (home.angle.y != mCurAngle.y) {
                    setAngle(home.angle.y);
                    mJntAnm.lookNone(1);
                }
            }

            if (field_0x11a0 != 0 || srchPlayerActor()) {
                mFaceMotionSeqMngr.setNo(0x1f, -1.0f, 0, 0);
                mMotionSeqMngr.setNo(0, -1.0f, 0, 0);
                mJntAnm.lookPlayer(0);
                field_0x119c = cLib_getRndValue(iVar4 / 2, iVar4);
                iVar3 = checkStep();
            } else {
                mJntAnm.lookNone(0);
                if (home.angle.y == mCurAngle.y) {
                    if (mType != TYPE_9) {
                        if (field_0x119c == 0) {
                            if ((mMotionSeqMngr.getNo() == 0x1f ||
                                 mMotionSeqMngr.getNo() == 0x20) &&
                                mMotionSeqMngr.getStepNo() > 0)
                            {
                                mFaceMotionSeqMngr.setNo(0x1f, -1.0f, 0, 0);
                                mMotionSeqMngr.setNo(0x19, -1.0f, 0, 0);
                                field_0x119c = cLib_getRndValue(iVar4 / 2, iVar4);
                            }
                        } else {
                            field_0x119c--;
                            if (field_0x119c > 0) {
                                mFaceMotionSeqMngr.setNo(0x1f, -1.0f, 0, 0);
                                mMotionSeqMngr.setNo(0x19, -1.0f, 0, 0);
                            } else {
                                mFaceMotionSeqMngr.setNo(0x10, -1.0f, 0, 0);
                                if (0.5f < cM_rnd()) {
                                    iVar4 = 0x1f;
                                } else {
                                    iVar4 = 0x20;
                                }
                                mMotionSeqMngr.setNo(iVar4, -1.0f, 0, 0);
                            }
                        }
                    }
                } else {
                    iVar3 = 1;
                }
            }

            if (iVar3 != 0L && step(home.angle.y, 0x1f, 0x24, 0xf, 0) != 0) {
                mMode = MODE_INIT;
            }
        }
        break;
    case MODE_EXIT:
        break;
    }

    
    return 1;
}

int daNpc_Taro_c::nurse(void* param_0) {
    switch (mMode) {
    case MODE_ENTER:
    case MODE_INIT:
        mFaceMotionSeqMngr.setNo(0x1f, -1.0f, 0, 0);
        mMotionSeqMngr.setNo(0, -1.0f, 0, 0);
        mMode = MODE_RUN;
    case MODE_RUN:
        if (!mStagger.checkStagger()) {
            BOOL isStep;
            if (srchPlayerActor()) {
                mJntAnm.lookPlayer(0);
                isStep = checkStep();
            } else {
                mJntAnm.lookNone(0);
                isStep = mCurAngle.y - home.angle.y != 0;
            }

            if (isStep && step(home.angle.y, 0x1f, 0x24, 0xf, 0) != 0) {
                mMode = MODE_INIT;
            }
        }
        break;
    case MODE_EXIT:
        break;
    }
    return 1;
}

int daNpc_Taro_c::talk(void* param_1) {
    switch (mMode) {
    case MODE_ENTER:
    case MODE_INIT:
        if (!mStagger.checkStagger()) {
            if (mType == TYPE_8 && !daNpcT_chkEvtBit(0x49)) {
                mFaceMotionSeqMngr.setNo(0x1f, -1.0f, 0, 0);
                mMotionSeqMngr.setNo(0, -1.0f, 0, 0);
            }
            field_0x11a3 = daNpcT_chkEvtBit(0x4a);
            initTalk(mFlowNodeNo, NULL);
            mMode = MODE_RUN;
        }
    case MODE_RUN:
        if (!mStagger.checkStagger()) {
            if (mTwilight || mPlayerAngle == mCurAngle.y ||
                (mType == TYPE_8 && daNpcT_chkEvtBit(0x49)) || mType == TYPE_6 || mType == TYPE_13)
            {
                if (talkProc(NULL, 0, NULL, 0) != 0) {
                    u16 evtNos[] = { 13, 14, 15, 16, 17, 18, 19, 20};

                    int evtId;
                    if ((int)mFlow.getEventId(&evtId) == EVENT_8 && evtId == 0) {
                        int iVar1 = 0;
                        if (field_0x11a3 != 0) {
                            daNpcT_onTmpBit(0x42);
                            iVar1++;
                            if (dComIfGs_isSaveSwitch(0x67)) {
                                iVar1++;
                                if (dComIfGs_isSaveSwitch(0x68)) {
                                    iVar1++;
                                }
                            }
                            iVar1 *= 2;
                            if (dComIfGs_getArrowNum() == 0) {
                                iVar1++;
                            }
                        } else {
                            iVar1 *= 2;
                            if (dComIfGs_getArrowNum() < 3) {
                                iVar1++;
                            }
                        }
                        mEvtNo = evtNos[iVar1];
                        evtChange();
                    } else {
                        if (mFlow.checkEndFlow()) {
                            mPlayerActorMngr.entry(daPy_getPlayerActorClass());
                            dComIfGp_event_reset();
                            mMode = MODE_EXIT;
                        }
                    }
                }

                mJntAnm.lookPlayer(0);
                if (mTwilight || (mType == TYPE_8 && daNpcT_chkEvtBit(0x49)) || mType == TYPE_13 ||
                    mType == TYPE_6)
                {
                    mJntAnm.lookNone(0);
                }
            } else {
                mJntAnm.lookPlayer(0);
                step(mPlayerAngle, 0x1f, 0x24, 0xf, 0);
            }
        }
        break;
    case MODE_EXIT:
        break;
    }

    return 0;
}

static int daNpc_Taro_Create(void* i_this) {
    return static_cast<daNpc_Taro_c*>(i_this)->create();
}

static int daNpc_Taro_Delete(void* i_this) {
    return static_cast<daNpc_Taro_c*>(i_this)->Delete();
}

static int daNpc_Taro_Execute(void* i_this) {
    return static_cast<daNpc_Taro_c*>(i_this)->Execute();
}

static int daNpc_Taro_Draw(void* i_this) {
    return static_cast<daNpc_Taro_c*>(i_this)->Draw();
}

static int daNpc_Taro_IsDelete(void* i_this) {
    return 1;
}

AUDIO_INSTANCES

static actor_method_class daNpc_Taro_MethodTable = {
    (process_method_func)daNpc_Taro_Create,
    (process_method_func)daNpc_Taro_Delete,
    (process_method_func)daNpc_Taro_Execute,
    (process_method_func)daNpc_Taro_IsDelete,
    (process_method_func)daNpc_Taro_Draw,
};

extern actor_process_profile_definition g_profile_NPC_TARO = {
  fpcLy_CURRENT_e,         // mLayerID
  7,                       // mListID
  fpcPi_CURRENT_e,         // mListPrio
  PROC_NPC_TARO,           // mProcName
  &g_fpcLf_Method.base,   // sub_method
  sizeof(daNpc_Taro_c),    // mSize
  0,                       // mSizeOther
  0,                       // mParameters
  &g_fopAc_Method.base,    // sub_method
  367,                     // mPriority
  &daNpc_Taro_MethodTable, // sub_method
  0x00040107,              // mStatus
  fopAc_NPC_e,             // mActorType
  fopAc_CULLBOX_CUSTOM_e,  // cullType
};
