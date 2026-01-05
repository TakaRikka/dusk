/**
 * @file d_a_npc_pachi_maro.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_npc_pachi_maro.h"
#include "JSystem/JHostIO/JORFile.h"
#include "d/d_debug_viewer.h"
#include "d/actor/d_a_npc_pachi_taro.h"

enum Maro_RES_File_ID {
    /* BCK */
    /* 0x06 */ BCK_MARO_F_TALK_A = 0x6,
    /* 0x07 */ BCK_MARO_STEP,
    /* 0x08 */ BCK_MARO_WAIT_A,

    /* BMDR */
    /* 0x0B */ BMDR_MARO = 0xB,
    
    /* BTK */
    /* 0x0E */ BTK_MARO = 0xE,

    /* BTP */
    /* 0x11 */ BTP_MARO = 0x11,
    /* 0x12 */ BTP_MARO_F_TALK_A,
};

enum Maro_TW_RES_File_ID {
    /* BMDR */
    /* 0x5 */ BMDR_MARO_TW = 0x5,

    /* BTK */
    /* 0x8 */ BTK_MARO_TW = 0x8,

    /* BTP */
    /* 0xB */ BTP_MARO_TW = 0xB,
};

enum Maro1_RES_File_ID {
    /* BCK */
    /* 0x06 */ BCK_MARO_F_SURPRISE = 0x6,
    /* 0x07 */ BCK_MARO_F_TALK_B,
    /* 0x08 */ BCK_MARO_F_TALK_C,
    /* 0x09 */ BCK_MARO_FH_SURPRISE,
    /* 0x0A */ BCK_MARO_RUN,
    /* 0x0B */ BCK_MARO_STOP,
    /* 0x0C */ BCK_MARO_TALK_A,
    /* 0x0D */ BCK_MARO_TALK_B,
    /* 0x0E */ BCK_MARO_TALK_C,

    /* BTK */
    /* 0x11 */ BTK_MARO_TALK_B = 0x11,
    /* 0x12 */ BTK_MARO_TALK_C,

    /* BTP */
    /* 0x15 */ BTP_MARO_F_SURPRISE = 0x15,
    /* 0x16 */ BTP_MARO_F_TALK_B,
    /* 0x17 */ BTP_MARO_F_TALK_C,
    /* 0x18 */ BTP_MARO_FH_SURPRISE,

    /* EVT */
    /* 0x1B */ EVT_MARO1_EVENT_LIST = 0x1B,
};

enum Maro2_RES_File_ID {
    /* BCK */
    /* 0x5 */ BCK_MARO_CROUCH = 0x5,
    /* 0x6 */ BCK_MARO_F_GLARE_TALK,
    /* 0x7 */ BCK_MARO_FH_GLARE_TALK,

    /* BTP */
    /* 0xA */ BTP_MARO_F_GLARE_TALK = 0xA,
    /* 0xB */ BTP_MARO_FH_GLARE_TALK,

    /* EVT */
    /* 0xE */ EVT_MARO2_EVENT_LIST = 0xE,
};

enum Maro3_RES_File_ID {
    /* BCK */
    /* 0x4 */ BCK_MARO_ASK = 0x4,
    /* 0x5 */ BCK_MARO_ASKING,
    /* 0x6 */ BCK_MARO_F_BESUTALK,
    /* 0x7 */ BCK_MARO_FH_BESUTALK,

    /* BTP */
    /* 0xA */ BTP_MARO_F_BESUTALK = 0xA,
    /* 0xB */ BTP_MARO_FH_BESUTALK,
};

enum Taro1_RES_File_ID {
    /* BCK */
    /* 0x07 */ BCK_TARO_CHOP_A = 0x7,
    /* 0x08 */ BCK_TARO_CHOP_B,
    /* 0x09 */ BCK_TARO_F_PANIC,
    /* 0x0A */ BCK_TARO_F_S_GET,
    /* 0x0B */ BCK_TARO_F_SAD,
    /* 0x0C */ BCK_TARO_F_SHOUT,
    /* 0x0D */ BCK_TARO_F_SURPRISE,
    /* 0x0E */ BCK_TARO_FH_PANIC,
    /* 0x0F */ BCK_TARO_FH_SAD,
    /* 0x10 */ BCK_TARO_FH_SURPRISE,
    /* 0x11 */ BCK_TARO_RUN,
    /* 0x12 */ BCK_TARO_S_CHOP_A,
    /* 0x13 */ BCK_TARO_S_CHOP_B,
    /* 0x14 */ BCK_TARO_S_GET,
    /* 0x15 */ BCK_TARO_S_RUN,
    /* 0x16 */ BCK_TARO_S_WAIT,
    /* 0x17 */ BCK_TARO_TALK_A,
    /* 0x18 */ BCK_TARO_TALK_B,
    /* 0x19 */ BCK_TARO_WAIT_B,
    /* 0x1A */ BCK_TARO_WAIT_D,
    /* 0x1B */ BCK_TARO_WAIT_E,
    /* 0x1C */ BCK_TARO_WAIT_F,

    /* BMDR */
    /* 0x1F */ BMDR_TARO_STICK_A = 0x1F,
    /* 0x20 */ BMDR_TARO_SWB,

    /* BTK */
    /* 0x23 */ BTK_TARO_S_GET = 0x23,
    /* 0x24 */ BTK_TARO_TALK_C,
    /* 0x25 */ BTK_TARO_TALK_E,
    /* 0x26 */ BTK_TARO_TARK_D,

    /* BTP */
    /* 0x29 */ BTP_TARO_F_PANIC = 0x29,
    /* 0x2A */ BTP_TARO_F_S_GET,
    /* 0x2B */ BTP_TARO_F_SAD,
    /* 0x2C */ BTP_TARO_F_SHOUT,
    /* 0x2D */ BTP_TARO_F_SURPRISE,
    /* 0x2E */ BTP_TARO_FH_PANIC,
    /* 0x2F */ BTP_TARO_FH_SAD,
    /* 0x30 */ BTP_TARO_FH_SURPRISE,

    /* EVT */
    /* 0x33 */ EVT_TARO1_EVENT_LIST = 0x33,
};

enum Taro2_RES_File_ID {
    /* BCK */
    /* 0x06 */ BCK_TARO_CALL = 0x6,
    /* 0x07 */ BCK_TARO_F_CALL,
    /* 0x08 */ BCK_TARO_F_HI,
    /* 0x09 */ BCK_TARO_F_MARVELOUS,
    /* 0x0A */ BCK_TARO_F_POINT,
    /* 0x0B */ BCK_TARO_F_WATCH,
    /* 0x0C */ BCK_TARO_F_WAVE,
    /* 0x0D */ BCK_TARO_HI,
    /* 0x0E */ BCK_TARO_HI_WAIT,
    /* 0x0F */ BCK_TARO_LOOKBACK,
    /* 0x10 */ BCK_TARO_MARVELOUS,
    /* 0x11 */ BCK_TARO_POINT,
    /* 0x12 */ BCK_TARO_TO_WAIT_C2,
    /* 0x13 */ BCK_TARO_WATCH,
    /* 0x14 */ BCK_TARO_WATCHING,
    /* 0x15 */ BCK_TARO_WAVE,

    /* BTK */
    /* 0x18 */ BTK_TARO_LOOKBACK = 0x18,

    /* BTP */
    /* 0x1B */ BTP_TARO_F_CALL = 0x1B,
    /* 0x1C */ BTP_TARO_F_HI,
    /* 0x1D */ BTP_TARO_F_MARVELOUS,
    /* 0x1E */ BTP_TARO_F_POINT,
    /* 0x1F */ BTP_TARO_F_WATCH,
    /* 0x20 */ BTP_TARO_F_WAVE,

    /* EVT */
    /* 0x23 */ EVT_TARO2_EVENT_LIST = 0x23,
};

enum Len1_RES_File_ID {
    /* BCK */
    /* 0x06 */ BCK_LEN_DISLIKE_A = 0x6,
    /* 0x07 */ BCK_LEN_DISLIKE_B,
    /* 0x08 */ BCK_LEN_EVADE_A,
    /* 0x09 */ BCK_LEN_EVADE_B,
    /* 0x0A */ BCK_LEN_EVADE_C,
    /* 0x0B */ BCK_LEN_F_DISLIKE_A,
    /* 0x0C */ BCK_LEN_F_DISLIKE_B,
    /* 0x0D */ BCK_LEN_F_EVADE_A,
    /* 0x0E */ BCK_LEN_F_TALK_B,
    /* 0x0F */ BCK_LEN_F_TALK_C,
    /* 0x10 */ BCK_LEN_F_TALK_D,
    /* 0x11 */ BCK_LEN_F_WAIT_BC_CONNECT,
    /* 0x12 */ BCK_LEN_F_WAIT_C,
    /* 0x13 */ BCK_LEN_F_WAIT_D,
    /* 0x14 */ BCK_LEN_F_WAIT_E,
    /* 0x15 */ BCK_LEN_FH_DISLIKE_B,
    /* 0x16 */ BCK_LEN_FH_EVADE_B,
    /* 0x17 */ BCK_LEN_FH_TALK_C,
    /* 0x18 */ BCK_LEN_FH_TALK_D,
    /* 0x19 */ BCK_LEN_FH_WAIT_C,
    /* 0x1A */ BCK_LEN_FH_WAIT_D,
    /* 0x1B */ BCK_LEN_TALK_A,
    /* 0x1C */ BCK_LEN_TALK_C,
    /* 0x1D */ BCK_LEN_WAIT_B,
    /* 0x1E */ BCK_LEN_WAIT_BC_CONNECT,
    /* 0x1F */ BCK_LEN_WAIT_C,
    /* 0x20 */ BCK_LEN_WAIT_D,
    /* 0x21 */ BCK_LEN_WAIT_E,
    /* 0x22 */ BCK_LEN_WALK_A,

    /* BTK */
    /* 0x25 */ BTK_LEN_F_DISLIKE = 0x25,
    /* 0x26 */ BTK_LEN_F_DISLIKE_B,
    /* 0x27 */ BTK_LEN_FH_DISLIKE_B,

    /* BTP */
    /* 0x2A */ BTP_LEN_F_DISLIKE_A = 0x2A,
    /* 0x2B */ BTP_LEN_F_DISLIKE_B,
    /* 0x2C */ BTP_LEN_F_EVADE_A,
    /* 0x2D */ BTP_LEN_F_TALK_C,
    /* 0x2E */ BTP_LEN_F_TALK_D,
    /* 0x2F */ BTP_LEN_F_WAIT_C,
    /* 0x30 */ BTP_LEN_FH_TALK_C,
    /* 0x31 */ BTP_LEN_FH_TALK_D,
    /* 0x32 */ BTP_LEN_FH_WAIT_C,

    /* EVT */
    /* 0x35 */ EVT_LEN1_EVENT_LIST = 0x35,
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

enum evt_pachi_RES_File_ID {
    /* EVT */
    /* 0x3 */ EVT_EVT_PACHI_EVENT_LIST = 0x3,
};

enum RES_Name {
    /* 0x0 */ NONE,
    /* 0x1 */ MARO,
    /* 0x2 */ MARO_TW,
    /* 0x3 */ MARO1,
    /* 0x4 */ MARO2,
    /* 0x5 */ MARO3,
    /* 0x6 */ TARO1,
    /* 0x7 */ TARO2,
    /* 0x8 */ LEN1,
    /* 0x9 */ BESU1,
    /* 0xA */ EVT_PACHI,
};

enum Face_Motion_Animation {
    /* 0x0 */ FACE_MOT_ANM_NONE,
    /* 0x1 */ FACE_MOT_ANM_TALK_A,
    /* 0x2 */ FACE_MOT_ANM_TALK_B,
    /* 0x3 */ FACE_MOT_ANM_TALK_C,
    /* 0x4 */ FACE_MOT_ANM_SURPRISE,
    /* 0x5 */ FACE_MOT_ANM_GLARE_TALK,
    /* 0x6 */ FACE_MOT_ANM_BESUTALK,
    /* 0x7 */ FACE_MOT_ANM_H_SURPRISE,
    /* 0x8 */ FACE_MOT_ANM_H_GLARE_TALK,
    /* 0x9 */ FACE_MOT_ANM_H_BESUTALK,
};

enum Motion_Animation {
    /* 0x0 */ MOT_ANM_WAIT_A,
    /* 0x1 */ MOT_ANM_CROUCH,
    /* 0x2 */ MOT_ANM_ASKING,
    /* 0x3 */ MOT_ANM_TALK_A,
    /* 0x4 */ MOT_ANM_TALK_B,
    /* 0x5 */ MOT_ANM_TALK_C,
    /* 0x6 */ MOT_ANM_ASK,
    /* 0x7 */ MOT_ANM_STOP,
    /* 0x8 */ MOT_ANM_STEP,
    /* 0x9 */ MOT_ANM_RUN,
};

enum Face_Motion {
    /* 0x0 */ FACE_MOT_TALK_A,
    /* 0x1 */ FACE_MOT_SURPRISE,
    /* 0x2 */ FACE_MOT_TALK_B,
    /* 0x3 */ FACE_MOT_GLARE_TALK,
    /* 0x4 */ FACE_MOT_BESUTALK,
    /* 0x5 */ FACE_MOT_TALK_C,
    /* 0x6 */ FACE_MOT_H_SURPRISE,
    /* 0x7 */ FACE_MOT_H_GLARE_TALK,
    /* 0x8 */ FACE_MOT_H_BESUTALK,
    /* 0x9 */ FACE_MOT_NONE,
};

enum Motion {
    /* 0x0 */ MOT_WAIT_A,
    /* 0x1 */ MOT_TALK_A,
    /* 0x2 */ MOT_CROUCH,
    /* 0x3 */ MOT_ASK,
    /* 0x4 */ MOT_STOP,
    /* 0x5 */ MOT_TALK_B,
    /* 0x6 */ MOT_TALK_C,
    /* 0x7 */ MOT_STEP,
    /* 0x8 */ MOT_RUN,
};

enum Type {
    /* 0x0 */ TYPE_0,
};

enum Event {
    /* 0x0 */ EVT_NONE,
    /* 0x1 */ EVT_TUTRIAL_TALK,
};

#if DEBUG
daNpc_Pachi_Maro_HIO_c::daNpc_Pachi_Maro_HIO_c() { m = daNpc_Pachi_Maro_Param_c::m; }

void daNpc_Pachi_Maro_HIO_c::listenPropertyEvent(const JORPropertyEvent* event) {
    JORReflexible::listenPropertyEvent(event);

    char buffer[2000];
    JORFile jorFile;
    int len;

    switch ((u32)event->id) {
        case 0x40000002:
            if (jorFile.open(6, "すべてのファイル(*.*)\0*.*\0", NULL, NULL, NULL)) {
                memset(buffer, 0, sizeof(buffer));
                len = 0;
                daNpcT_cmnListenPropertyEvent(buffer, &len, &m.common);
                jorFile.writeData(buffer, len);
                jorFile.close();
                OS_REPORT("write append success!::%6d\n", len);
            } else {
                OS_REPORT("write append failure!\n");
            }
            break;
    }
}

void daNpc_Pachi_Maro_HIO_c::genMessage(JORMContext* ctx) {
    daNpcT_cmnGenMessage(ctx, &m.common);
    ctx->genButton("ファイル書き出し", 0x40000002, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
}
#endif

static int l_bmdData[2][2] = {
    {11, 1},
    {5, 2},
};

static daNpcT_evtData_c l_evtList[2] = {
    {"", 0},
    {"TUTRIAL_TALK", 10},
};

static char* l_resNameList[11] = {
    "",
    "Maro",
    "Maro_TW",
    "Maro1",
    "Maro2",
    "Maro3",
    "Taro1",
    "Taro2",
    "Len1",
    "Besu1",
    "evt_pachi",
};

static s8 l_loadResPtrn0[3] = {
    MARO, MARO1, -1,
};

static s8 l_loadResPtrn1[3] = {
    MARO, MARO_TW, -1,
};

static s8 l_loadResPtrn2[6] = {
    MARO, MARO1, MARO2, MARO3, EVT_PACHI, -1,
};

static s8 l_loadResPtrn3[5] = {
    MARO, MARO_TW, MARO1, MARO2, -1,
};

static s8* l_loadResPtrnList[2] = {
    l_loadResPtrn2,
    l_loadResPtrn3,
};

static daNpcT_faceMotionAnmData_c l_faceMotionAnmData[10] = {
    {-1, J3DFrameCtrl::EMode_NONE, NONE, BTP_MARO, J3DFrameCtrl::EMode_LOOP, MARO, 1},
    {BCK_MARO_F_TALK_A, J3DFrameCtrl::EMode_NONE, MARO, BTP_MARO_F_TALK_A, J3DFrameCtrl::EMode_NONE, MARO, 0},
    {BCK_MARO_F_TALK_B, J3DFrameCtrl::EMode_NONE, MARO1, BTP_MARO_F_TALK_B, J3DFrameCtrl::EMode_NONE, MARO1, 0},
    {BCK_MARO_F_TALK_C, J3DFrameCtrl::EMode_NONE, MARO1, BTP_MARO_F_TALK_C, J3DFrameCtrl::EMode_NONE, MARO1, 0},
    {BCK_MARO_F_SURPRISE, J3DFrameCtrl::EMode_NONE, MARO1, BTP_MARO_F_SURPRISE, J3DFrameCtrl::EMode_NONE, MARO1, 0},
    {BCK_MARO_F_GLARE_TALK, J3DFrameCtrl::EMode_NONE, MARO2, BTP_MARO_F_GLARE_TALK, J3DFrameCtrl::EMode_NONE, MARO2, 0},
    {BCK_MARO_F_BESUTALK, J3DFrameCtrl::EMode_NONE, MARO3, BTP_MARO_F_BESUTALK, J3DFrameCtrl::EMode_NONE, MARO3, 0},
    {BCK_MARO_FH_SURPRISE, J3DFrameCtrl::EMode_LOOP, MARO1, BTP_MARO_FH_SURPRISE, J3DFrameCtrl::EMode_LOOP, MARO1, 0},
    {BCK_MARO_FH_GLARE_TALK, J3DFrameCtrl::EMode_LOOP, MARO2, BTP_MARO_FH_GLARE_TALK, J3DFrameCtrl::EMode_LOOP, MARO2, 0},
    {BCK_MARO_FH_BESUTALK, J3DFrameCtrl::EMode_LOOP, MARO3, BTP_MARO_FH_BESUTALK, J3DFrameCtrl::EMode_LOOP, MARO3, 0},
};

static daNpcT_motionAnmData_c l_motionAnmData[10] = {
    {BCK_MARO_WAIT_A, J3DFrameCtrl::EMode_LOOP, MARO, BTK_MARO, J3DFrameCtrl::EMode_NONE, MARO, 1, 0},
    {BCK_MARO_CROUCH, J3DFrameCtrl::EMode_LOOP, MARO2, BTK_MARO, J3DFrameCtrl::EMode_NONE, MARO, 1, 0},
    {BCK_MARO_ASKING, J3DFrameCtrl::EMode_LOOP, MARO3, BTK_MARO, J3DFrameCtrl::EMode_NONE, MARO, 1, 0},
    {BCK_MARO_TALK_A, J3DFrameCtrl::EMode_NONE, MARO1, BTK_MARO, J3DFrameCtrl::EMode_NONE, MARO, 1, 0},
    {BCK_MARO_TALK_B, J3DFrameCtrl::EMode_NONE, MARO1, BTK_MARO_TALK_B, J3DFrameCtrl::EMode_NONE, MARO1, 0, 0},
    {BCK_MARO_TALK_C, J3DFrameCtrl::EMode_NONE, MARO1, BTK_MARO_TALK_C, J3DFrameCtrl::EMode_NONE, MARO1, 0, 0},
    {BCK_MARO_ASK, J3DFrameCtrl::EMode_NONE, MARO3, BTK_MARO, J3DFrameCtrl::EMode_NONE, MARO, 1, 0},
    {BCK_MARO_STOP, J3DFrameCtrl::EMode_LOOP, MARO1, BTK_MARO, J3DFrameCtrl::EMode_NONE, MARO, 1, 0},
    {BCK_MARO_STEP, J3DFrameCtrl::EMode_NONE, MARO, BTK_MARO, J3DFrameCtrl::EMode_NONE, MARO, 1, 0},
    {BCK_MARO_RUN, J3DFrameCtrl::EMode_LOOP, MARO1, BTK_MARO, J3DFrameCtrl::EMode_NONE, MARO, 1, 0},
};

static daNpcT_MotionSeqMngr_c::sequenceStepData_c l_faceMotionSequenceData[40] = {
    {FACE_MOT_ANM_TALK_A, -1, 1}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {FACE_MOT_ANM_SURPRISE, -1, 1}, {FACE_MOT_ANM_H_SURPRISE, -1, 0}, {-1, 0, 0}, {-1, 0, 0},
    {FACE_MOT_ANM_TALK_B, -1, 1}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {FACE_MOT_ANM_GLARE_TALK, -1, 1}, {FACE_MOT_ANM_H_GLARE_TALK, -1, 0}, {-1, 0, 0}, {-1, 0, 0},
    {FACE_MOT_ANM_BESUTALK, -1, 1}, {FACE_MOT_ANM_H_BESUTALK, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {FACE_MOT_ANM_TALK_C, -1, 1}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {FACE_MOT_ANM_H_SURPRISE, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {FACE_MOT_ANM_H_GLARE_TALK, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {FACE_MOT_ANM_H_BESUTALK, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {FACE_MOT_ANM_NONE, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
};

static daNpcT_MotionSeqMngr_c::sequenceStepData_c l_motionSequenceData[36] = {
    {MOT_ANM_WAIT_A, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {MOT_ANM_TALK_A, -1, 1}, {MOT_ANM_WAIT_A, -1, 0}, {-1, 0, 0}, {-1, 0, 0},
    {MOT_ANM_CROUCH, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {MOT_ANM_ASK, 0, 1}, {MOT_ANM_ASKING, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {MOT_ANM_STOP, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {MOT_ANM_TALK_B, -1, 1}, {MOT_ANM_WAIT_A, -1, 0}, {-1, 0, 0}, {-1, 0, 0},
    {MOT_ANM_TALK_C, -1, 1}, {MOT_ANM_WAIT_A, -1, 0}, {-1, 0, 0}, {-1, 0, 0},
    {MOT_ANM_STEP, 4, 1}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {MOT_ANM_RUN, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
};

char* daNpc_Pachi_Maro_c::mCutNameList[11] = {
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

daNpc_Pachi_Maro_c::cutFunc daNpc_Pachi_Maro_c::mCutList[11] = {
    NULL,
    &daNpc_Pachi_Maro_c::cutTutrialBegin,
    &daNpc_Pachi_Maro_c::cutTutrialBegin_Skip,
    &daNpc_Pachi_Maro_c::cutTutrialClear,
    &daNpc_Pachi_Maro_c::cutTalk,
    &daNpc_Pachi_Maro_c::cutTalk2,
    &daNpc_Pachi_Maro_c::cut1stHit_KakasiBody,
    &daNpc_Pachi_Maro_c::cutTutrialSelectGiveUp,
    &daNpc_Pachi_Maro_c::cutTutrialGiveUp,
    &daNpc_Pachi_Maro_c::cutTutrialContinue,
    &daNpc_Pachi_Maro_c::cutTutrialCaution,
};

static NPC_PACHI_MARO_HIO_CLASS l_HIO;

daNpc_Pachi_Maro_c::~daNpc_Pachi_Maro_c() {
    OS_REPORT("|%06d:%x|daNpc_Pachi_Maro_c -> デストラクト\n", g_Counter.mCounter0, this);

    if (mpMorf[0] != NULL) {
        mpMorf[0]->stopZelAnime();
    }

    #if DEBUG
    if (mpHIO != NULL) {
        mpHIO->removeHIO();
    }
    #endif

    deleteRes(l_loadResPtrnList[mType], (const char**)l_resNameList);
}

daNpc_Pachi_Maro_HIOParam const daNpc_Pachi_Maro_Param_c::m = {
    100.0f,
    -3.0f,
    1.0f,
    400.0f,
    255.0f,
    80.0f,
    35.0f,
    30.0f,
    0.0f,
    0.0f,
    10.0f,
    -10.0f,
    30.0f,
    -10.0f,
    30.0f,
    -30.0f,
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
    30.0f,
    0,
};

cPhs__Step daNpc_Pachi_Maro_c::create() {
    daNpcT_ct(this, daNpc_Pachi_Maro_c, l_faceMotionAnmData, l_motionAnmData,
                       l_faceMotionSequenceData, 4, l_motionSequenceData, 4,
                       l_evtList, l_resNameList);

    mType = getType();
    mFlowNodeNo = getFlowNodeNo();
    mTwilight = false;

    cPhs__Step phase = (cPhs__Step)loadRes(l_loadResPtrnList[mType], (const char**)l_resNameList);
    if (phase == cPhs_COMPLEATE_e) {
        if (!fopAcM_entrySolidHeap(this, createHeapCallBack, 0x3310)) {
            return cPhs_ERROR_e;
        }

        OS_REPORT("\t(%s:%d) flowNo:%d, PathID:%02x, BitSW:%02x, MaxItem:%d, group:%d<%08x> ", fopAcM_getProcNameString(this), mType, mFlowNodeNo, getPathID(),
                  getBitSW(), getMaxNumItem(), getGroupId(), fopAcM_GetParam(this));

        if (isDelete()) {
            OS_REPORT("===>isDelete:TRUE\n");
            return cPhs_ERROR_e;
        }

        OS_REPORT("\n");

        J3DModelData* modelData = mpMorf[0]->getModel()->getModelData();
        fopAcM_SetMtx(this, mpMorf[0]->getModel()->getBaseTRMtx());
        fopAcM_setCullSizeBox(this, -300.0f, -50.0f, -300.0f, 300.0f, 450.0f, 300.0f);
        mSound.init(&current.pos, &eyePos, 3, 1);

        #if DEBUG
        mpHIO = &l_HIO;
        mpHIO->entryHIO("パチチュー：マロ");
        #endif

        mAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1, &mAcchCir, fopAcM_GetSpeed_p(this),
                  fopAcM_GetAngle_p(this), fopAcM_GetShapeAngle_p(this));
        mCcStts.Init(mpHIO->m.common.weight, 0, this);
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

int daNpc_Pachi_Maro_c::CreateHeap() {
    J3DModelData* modelData = NULL;
    J3DModel* model = NULL;
    
    int bmdIdx = mTwilight == true ? MARO : NONE;
    int arcIdx = l_bmdData[bmdIdx][1];
    int idx = l_bmdData[bmdIdx][0];
    modelData = static_cast<J3DModelData*>(dComIfG_getObjectRes(l_resNameList[arcIdx], idx));
    if (modelData == NULL) {
        return 1;
    }

    u32 uVar1 = 0x11020284;
    mpMorf[0] = new mDoExt_McaMorfSO(modelData, NULL, NULL, NULL, -1, 1.0f, 0, -1, &mSound, 0x80000, uVar1);

    if (mpMorf[0] == NULL || mpMorf[0]->getModel() == NULL) {
        return 0;
    }

    model = mpMorf[0]->getModel();
    for (u16 i = 0; i < modelData->getJointNum(); i++) {
        modelData->getJointNodePointer(i)->setCallBack(ctrlJointCallBack);
    }
    model->setUserArea((u32)this);

    mpMatAnm[0] = new daNpcT_MatAnm_c();
    if (mpMatAnm[0] == NULL) {
        return 0;
    }

    if (setFaceMotionAnm(FACE_MOT_ANM_TALK_A, false) && setMotionAnm(MOT_ANM_WAIT_A, 0.0f, FALSE)) {
        return 1;
    }

    return 0;
}

int daNpc_Pachi_Maro_c::Delete() {
    OS_REPORT("|%06d:%x|daNpc_Pachi_Maro_c -> Delete\n", g_Counter.mCounter0, this);
    fpc_ProcID id = fopAcM_GetID(this);
    this->~daNpc_Pachi_Maro_c();
    return 1;
}

int daNpc_Pachi_Maro_c::Execute() {
    return execute();
}

int daNpc_Pachi_Maro_c::Draw() {
    if (mpMatAnm[0] != NULL) {
        J3DModelData* modelData = mpMorf[0]->getModel()->getModelData();
        modelData->getMaterialNodePointer(getEyeballMaterialNo())->setMaterialAnm(mpMatAnm[0]);
    }

    return draw(
#if DEBUG
        chkAction(&daNpc_Pachi_Maro_c::test),
#else
        FALSE,
#endif
        FALSE,
        mRealShadowSize, NULL, 100.0f, FALSE, FALSE, FALSE
    );
}

int daNpc_Pachi_Maro_c::createHeapCallBack(fopAc_ac_c* i_this) {
    daNpc_Pachi_Maro_c* actor = (daNpc_Pachi_Maro_c*)i_this;
    return actor->CreateHeap();
}

int daNpc_Pachi_Maro_c::ctrlJointCallBack(J3DJoint* i_joint, int param_2) {
    if (param_2 == 0) {
        J3DModel* model = j3dSys.getModel();
        daNpc_Pachi_Maro_c* actor = (daNpc_Pachi_Maro_c*)model->getUserArea();
        if (actor != 0) {
            actor->ctrlJoint(i_joint, model);
        }
    }

    return 1;
}

u8 daNpc_Pachi_Maro_c::getType() {
    u8 param = fopAcM_GetParam(this) & 0xFF;

    switch (param) {
        case 0:
            return TYPE_0;

        default:
            return 1;
    }
}

BOOL daNpc_Pachi_Maro_c::isDelete() {
    switch (mType) {
        case TYPE_0:
            return FALSE;

        default:
            return FALSE;
    }
}

void daNpc_Pachi_Maro_c::reset() {
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
            field_0xf85 = 0;
            field_0xde0 = 0.07f;
            field_0xde4 = 0.1f;
            break;
    }

    setAngle(home.angle.y);
}

void daNpc_Pachi_Maro_c::afterJntAnm(int i_joint) {
    if (i_joint == JNT_BACKBONE1) {
        mDoMtx_stack_c::YrotM(mStagger.getAngleX(1));
        mDoMtx_stack_c::ZrotM(mStagger.getAngleZ(1));
    } else if (i_joint == JNT_HEAD) {
        mDoMtx_stack_c::YrotM(mStagger.getAngleX(0));
        mDoMtx_stack_c::ZrotM(mStagger.getAngleZ(0));
    }
}

void daNpc_Pachi_Maro_c::setParam() {
    selectAction();
    srchActors();

    u32 flags = fopAc_AttnFlag_SPEAK_e | fopAc_AttnFlag_TALK_e;
    s16 talk_distance = mpHIO->m.common.talk_distance;
    s16 talk_angle = mpHIO->m.common.talk_angle;

    attention_info.distances[fopAc_attn_LOCK_e] = daNpcT_getDistTableIdx(mpHIO->m.common.attention_distance, mpHIO->m.common.attention_angle);
    attention_info.distances[fopAc_attn_TALK_e] = attention_info.distances[fopAc_attn_LOCK_e];
    attention_info.distances[fopAc_attn_SPEAK_e] = daNpcT_getDistTableIdx(talk_distance, talk_angle);
    attention_info.flags = flags;

    scale.set(mpHIO->m.common.scale, mpHIO->m.common.scale, mpHIO->m.common.scale);
    mCcStts.SetWeight(mpHIO->m.common.weight);
    mCylH = mpHIO->m.common.height;
    mWallR = mpHIO->m.common.width;
    mAttnFovY = mpHIO->m.common.fov;
    mAcchCir.SetWallR(mWallR);
    mAcchCir.SetWallH(mpHIO->m.common.knee_length);
    mRealShadowSize = mpHIO->m.common.real_shadow_size;
    mExpressionMorfFrame = mpHIO->m.common.expression_morf_frame;
    mMorfFrames = mpHIO->m.common.morf_frame;
    gravity = mpHIO->m.common.gravity;
}

BOOL daNpc_Pachi_Maro_c::checkChangeEvt() {
    #if DEBUG
    BOOL rv = FALSE;
    switch (mType) {
        case TYPE_0:
            break;
    }
    #endif
    
    return FALSE;
}

void daNpc_Pachi_Maro_c::setAfterTalkMotion() {
    int idx = FACE_MOT_NONE;

    switch (mFaceMotionSeqMngr.getNo()) {
        case FACE_MOT_SURPRISE:
            return;
        
        case FACE_MOT_GLARE_TALK:
            idx = FACE_MOT_H_GLARE_TALK;
            break;

        case FACE_MOT_BESUTALK:
            idx = FACE_MOT_H_BESUTALK;
            break;
    }

    mFaceMotionSeqMngr.setNo(idx, -1.0f, FALSE, 0);
}

void daNpc_Pachi_Maro_c::srchActors() {
    switch (mType) {
        case TYPE_0:
            if (mActorMngrs[0].getActorP() == NULL) {
                mActorMngrs[0].entry(getNearestActorP(PROC_NPC_PACHI_TARO));
            }

            if (mActorMngrs[1].getActorP() == NULL) {
                mActorMngrs[1].entry(getNearestActorP(PROC_NPC_PACHI_BESU));
            }
            break;
    }
}

BOOL daNpc_Pachi_Maro_c::evtTalk() {
    if (chkAction(&daNpc_Pachi_Maro_c::talk)) {
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

        setAction(&daNpc_Pachi_Maro_c::talk);
    }

    return TRUE;
}

BOOL daNpc_Pachi_Maro_c::evtCutProc() {
    int staffId = dComIfGp_getEventManager().getMyStaffId("PA_Maro", this, -1);

    if (staffId != -1) {
        mStaffId = staffId;
        int actIdx = dComIfGp_getEventManager().getMyActIdx(mStaffId, mCutNameList, 11, 0, 0);
        if ((this->*(mCutList[actIdx]))(mStaffId) != 0) {
            dComIfGp_getEventManager().cutEnd(mStaffId);
        }

        return TRUE;
    }

    return FALSE;
}

void daNpc_Pachi_Maro_c::action() {
    fopAc_ac_c* actor_p = NULL;

    if (!mTwilight) {
        actor_p = hitChk(&mCyl, 0xFFFFFFFF);
    }

    if (actor_p != NULL) {
        mStagger.setParam(this, actor_p, mCurAngle.y);
        setDamage(0, 9, 0);
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

void daNpc_Pachi_Maro_c::beforeMove() {
    if (checkHide() || mNoDraw) {
        attention_info.flags = 0;
    }
}

void daNpc_Pachi_Maro_c::afterMoved() {
    fopAc_ac_c* besu_p = mActorMngrs[1].getActorP();
    fopAc_ac_c* taro_p = mActorMngrs[0].getActorP();

    switch (mFMotion) {
        case 1:
            field_0xf85 = 1;

            if (besu_p != NULL) {
                mJntAnm.lookActor(besu_p, 0.0f, 0);
            }
            
            setFMotion_None();
            break;
        
        case 2:
            field_0xf85 = 0;
            mJntAnm.lookNone(0);
            setFMotion_None();
    }
}

void daNpc_Pachi_Maro_c::setAttnPos() {
    cXyz sp38(0.0f, 0.0f, -30.0f);
    
    mStagger.calc(FALSE);
    f32 fVar1 = cM_s2rad(mCurAngle.y - field_0xd7e.y);

    if (field_0xf85 != 0) {
        mJntAnm.setParam(this, mpMorf[0]->getModel(), &sp38, getBackboneJointNo(), getNeckJointNo(), getHeadJointNo(),
                        0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, NULL);
    } else {
        mJntAnm.setParam(this, mpMorf[0]->getModel(), &sp38, getBackboneJointNo(), getNeckJointNo(), getHeadJointNo(),
                        mpHIO->m.common.body_angleX_min, mpHIO->m.common.body_angleX_max,
                        mpHIO->m.common.body_angleY_min, mpHIO->m.common.body_angleY_max,
                        mpHIO->m.common.head_angleX_min, mpHIO->m.common.head_angleX_max,
                        mpHIO->m.common.head_angleY_min, mpHIO->m.common.head_angleY_max,
                        mpHIO->m.common.neck_rotation_ratio, 0.0f, NULL);
    }

    mJntAnm.calcJntRad(0.2f, 1.0f, fVar1);
    setMtx();

    mDoMtx_stack_c::copy(mpMorf[0]->getModel()->getAnmMtx(getHeadJointNo()));
    mDoMtx_stack_c::multVec(&sp38, &eyePos);
    mJntAnm.setEyeAngleX(eyePos, 1.0f, 0);
    mJntAnm.setEyeAngleY(eyePos, mCurAngle.y, FALSE, 1.0f, 0);

    attention_info.position = current.pos;
    attention_info.position.y += mpHIO->m.common.attention_offset;
}

void daNpc_Pachi_Maro_c::setCollision() {
    if (!mHide) {
        u32 coSPrm = 0x79;
        u32 tgType = 0xD8FBFDFF;
        u32 tgSPrm = 0x1F;

        if (dComIfGp_event_runCheck()) {
            tgType = 0;
            tgSPrm = 0;
        } else if (mTwilight) {
            tgType = 0;
            tgSPrm = 0;
        } else if (mStagger.checkStagger()) {
            tgType = 0;
            tgSPrm = 0;
        }

        mCyl.SetCoSPrm(coSPrm);
        mCyl.SetTgType(tgType);
        mCyl.SetTgSPrm(tgSPrm);
        mCyl.OnTgNoHitMark();
        mCyl.SetH(mCylH);
        mCyl.SetR(mWallR);
        mCyl.SetC(current.pos);
        dComIfG_Ccsp()->Set(&mCyl);
    }

    mCyl.ClrCoHit();
    mCyl.ClrTgHit();
}

int daNpc_Pachi_Maro_c::drawDbgInfo() {
    #if DEBUG
    if (mpHIO->m.common.debug_info_ON) {
        f32 radius1 = dComIfGp_getAttention()->getDistTable(attention_info.distances[fopAc_attn_SPEAK_e]).mDistMax;
        f32 radius2 = dComIfGp_getAttention()->getDistTable(attention_info.distances[fopAc_attn_TALK_e]).mDistMax;
        dDbVw_drawCircleOpa(attention_info.position, radius1, (GXColor){0x00, 0xC8, 0x00, 0xFF}, 1, 12);
        dDbVw_drawCircleOpa(attention_info.position, radius2, (GXColor){0xC8, 0x00, 0x00, 0xFF}, 1, 12);
        dDbVw_drawSphereXlu(eyePos, 18.0f, (GXColor){0x80, 0x80, 0x80, 0xA0}, 1);
        dDbVw_drawSphereXlu(attention_info.position, 9.0f, (GXColor){0x80, 0x80, 0x80, 0xA0}, 1);
    }
    #endif
    
    return 0;
}

daNpcT_faceMotionAnmData_c daNpc_Pachi_Maro_c::getFaceMotionAnm(daNpcT_faceMotionAnmData_c param_1) {
    daNpcT_faceMotionAnmData_c rv = param_1;
    if (mTwilight != 0 && rv.mBtpFileIdx == 17) {
        rv.mBtpFileIdx = 11;
        rv.mBtpArcIdx = 2;
    }
    return rv;
}

daNpcT_motionAnmData_c daNpc_Pachi_Maro_c::getMotionAnm(daNpcT_motionAnmData_c param_1) {
    daNpcT_motionAnmData_c rv = param_1;
    if (mTwilight && rv.mBtkFileIdx == 14) {
        rv.mBtkFileIdx = 8;
        rv.mBtkArcIdx = 2;
    }
    return rv;
}

BOOL daNpc_Pachi_Maro_c::selectAction() {
    mNextAction = NULL;

    #if DEBUG
    if (mpHIO->m.common.debug_mode_ON) {
        mNextAction = &daNpc_Pachi_Maro_c::test;
        return TRUE;
    }
    #endif

    switch (mType) {
        case TYPE_0:
            mNextAction = &daNpc_Pachi_Maro_c::wait;
            break;

        default:
            mNextAction = &daNpc_Pachi_Maro_c::wait;
            break;
    }

    return TRUE;
}

BOOL daNpc_Pachi_Maro_c::chkAction(actionFunc action) {
    return mAction == action;
}

BOOL daNpc_Pachi_Maro_c::setAction(actionFunc action) {
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

int daNpc_Pachi_Maro_c::wait(void* param_1) {
    int unused = 0;
    
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
                if (mPlayerActorMngr.getActorP() != NULL) {
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
                            if (step(home.angle.y, 9, 7, 15, 0)) {
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
            
                switch (mJntAnm.getMode()) {
                    case 0:
                    default:
                        break;
                }
            }
            break;

        case MODE_EXIT:
            break;
    }

    return 1;
}

int daNpc_Pachi_Maro_c::talk(void* param_1) {
    switch (mMode) {
        case MODE_ENTER:
        case MODE_INIT:
            if (!mStagger.checkStagger()) {
                daNpc_Pachi_Taro_c* taro_p = (daNpc_Pachi_Taro_c*)mActorMngrs[0].getActorP();
                if (taro_p != NULL) {
                    taro_p->clrMesPat();
                }

                mMode = MODE_RUN;
            }
            // fallthrough
        case MODE_RUN:
            mEvtNo = EVT_TUTRIAL_TALK;
            mPrevEvtNo = mEvtNo;
            evtChange();
            break;

        case MODE_EXIT:
            break;
    }

    return 0;
}

int daNpc_Pachi_Maro_c::test(void* param_1) {
    switch (mMode) {
        case MODE_ENTER:
        case MODE_INIT:
            speedF = 0.0f;
            speed.setall(0.0f);
            mMode = MODE_RUN;
            // fallthrough
        case MODE_RUN:
            mFaceMotionSeqMngr.setNo(mpHIO->m.common.face_expression, -1.0f, FALSE, 0);
            mMotionSeqMngr.setNo(mpHIO->m.common.motion, -1.0f, FALSE, 0);
            mJntAnm.lookNone(0);
            attention_info.flags = 0;
            break;

        case MODE_EXIT:
            break;
    }

    return 1;
}

BOOL daNpc_Pachi_Maro_c::_turn_to_link(s16 i_step) {
    daPy_py_c* player = daPy_getPlayerActorClass();
    cXyz pos(*fopAcM_GetPosition_p(player));
    return _turn_pos(pos, i_step);
}

BOOL daNpc_Pachi_Maro_c::_turn_pos(cXyz const& i_pos, s16 i_step) {
    s16 target = cLib_targetAngleY(&current.pos, &i_pos);
    BOOL rv = cLib_chaseAngleS(&mCurAngle.y, target, i_step);
    current.angle.y = mCurAngle.y;
    shape_angle.y = mCurAngle.y;
    return rv;
}

BOOL daNpc_Pachi_Maro_c::_turn_pos(cXyz const& i_pos) {
    s16 angleY = cLib_targetAngleY(&current.pos, &i_pos);
    mCurAngle.y = angleY;
    current.angle.y = angleY;
    shape_angle.y = angleY;

    return TRUE;
}

BOOL daNpc_Pachi_Maro_c::cutTutrialBegin(int i_staffId) {
    dEvent_manager_c& eventManager = dComIfGp_getEventManager();
    int* cutId = NULL;
    cutId = dComIfGp_evmng_getMyIntegerP(i_staffId, "cutId");
    if (cutId == NULL) {
        OS_REPORT("-----CUT FALSE\n");
        return TRUE;
    }

    if (eventManager.getIsAddvance(i_staffId)) {
        _cutTutrialBegin_Init(*cutId);
    }

    return _cutTutrialBegin_Main(*cutId);
}

BOOL daNpc_Pachi_Maro_c::_cutTutrialBegin_Init(int const& i_cutId) {
    switch (i_cutId) {
        default:
            return TRUE;
    }
}

BOOL daNpc_Pachi_Maro_c::_cutTutrialBegin_Main(int const& i_cutId) {
    BOOL rv = FALSE;

    switch (i_cutId) {
        default:
            return rv;;
    }
}

BOOL daNpc_Pachi_Maro_c::cutTutrialBegin_Skip(int i_staffId) {
    dEvent_manager_c& eventManager = dComIfGp_getEventManager();
    int* cutId = NULL;
    cutId = dComIfGp_evmng_getMyIntegerP(i_staffId, "cutId");
    if (cutId == NULL) {
        OS_REPORT("-----CUT FALSE\n");
        return TRUE;
    }

    if (eventManager.getIsAddvance(i_staffId)) {
        _cutTutrialBegin_Skip_Init(*cutId);
    }

    return _cutTutrialBegin_Skip_Main(*cutId);
}

BOOL daNpc_Pachi_Maro_c::_cutTutrialBegin_Skip_Init(int const& i_cutId) {
    switch (i_cutId) {
        default:
            return TRUE;
    }
}

BOOL daNpc_Pachi_Maro_c::_cutTutrialBegin_Skip_Main(int const& i_cutId) {
    BOOL rv = FALSE;

    switch (i_cutId) {
        default:
            return rv;
    }
}

BOOL daNpc_Pachi_Maro_c::cutTutrialClear(int i_staffId) {
    dEvent_manager_c& eventManager = dComIfGp_getEventManager();
    int* cutId = NULL;
    cutId = dComIfGp_evmng_getMyIntegerP(i_staffId, "cutId");
    if (cutId == NULL) {
        OS_REPORT("-----CUT FALSE\n");
        return TRUE;
    }

    if (eventManager.getIsAddvance(i_staffId)) {
        _cutTutrialClear_Init(*cutId);
    }

    return _cutTutrialClear_Main(*cutId);
}

BOOL daNpc_Pachi_Maro_c::_cutTutrialClear_Init(int const& i_cutId) {
    switch (i_cutId) {
        case 5:
            mJntAnm.lookNone(0);
            break;

        case 10:
            break;

        case 20:
            mMotionSeqMngr.setNo(MOT_WAIT_A, -1.0f, FALSE, 0);
            mFaceMotionSeqMngr.setNo(FACE_MOT_SURPRISE, -1.0f, FALSE, 0);
            break;
    }

    return TRUE;
}

BOOL daNpc_Pachi_Maro_c::_cutTutrialClear_Main(int const& i_cutId) {
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
        }
        break;

        case 20:
            break;
    }

    return rv;
}

BOOL daNpc_Pachi_Maro_c::cutTalk(int i_staffId) {
    dEvent_manager_c& eventManager = dComIfGp_getEventManager();
    int* cutId = NULL;
    cutId = dComIfGp_evmng_getMyIntegerP(i_staffId, "cutId");
    if (cutId == NULL) {
        OS_REPORT("-----CUT FALSE\n");
        return TRUE;
    }

    if (eventManager.getIsAddvance(i_staffId)) {
        _cutTalk_Init(*cutId);
    }

    return _cutTalk_Main(*cutId);
}

BOOL daNpc_Pachi_Maro_c::_cutTalk_Init(int const& i_cutId) {
    switch (i_cutId) {
        case 5:
            break;

        case 10:
            break;
    }

    return TRUE;
}

BOOL daNpc_Pachi_Maro_c::_cutTalk_Main(int const& i_cutId) {
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

BOOL daNpc_Pachi_Maro_c::cutTalk2(int i_staffId) {
    dEvent_manager_c& eventManager = dComIfGp_getEventManager();
    int* cutId = NULL;
    cutId = dComIfGp_evmng_getMyIntegerP(i_staffId, "cutId");
    if (cutId == NULL) {
        OS_REPORT("-----CUT FALSE\n");
        return TRUE;
    }

    if (eventManager.getIsAddvance(i_staffId)) {
        _cutTalk2_Init(*cutId);
    }

    return _cutTalk2_Main(*cutId);
}

BOOL daNpc_Pachi_Maro_c::_cutTalk2_Init(int const& i_cutId) {
    switch (i_cutId) {
        case 5:
            break;

        case 10:
            break;
    }
    
    return TRUE;
}

BOOL daNpc_Pachi_Maro_c::_cutTalk2_Main(int const& i_cutId) {
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

BOOL daNpc_Pachi_Maro_c::cut1stHit_KakasiBody(int i_staffId) {
    dEvent_manager_c& eventManager = dComIfGp_getEventManager();
    int* cutId = NULL;
    cutId = dComIfGp_evmng_getMyIntegerP(i_staffId, "cutId");
    if (cutId == NULL) {
        OS_REPORT("-----CUT FALSE\n");
        return TRUE;
    }

    if (eventManager.getIsAddvance(i_staffId)) {
        _cut1stHit_KakasiBody_Init(*cutId);
    }

    return _cut1stHit_KakasiBody_Main(*cutId);
}

BOOL daNpc_Pachi_Maro_c::_cut1stHit_KakasiBody_Init(int const& i_cutId) {
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

BOOL daNpc_Pachi_Maro_c::_cut1stHit_KakasiBody_Main(int const& i_cutId) {
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
        }
        break;

        case 20:
            break;
    }

    return rv;
}

BOOL daNpc_Pachi_Maro_c::cutTutrialSelectGiveUp(int i_staffId) {
    dEvent_manager_c& eventManager = dComIfGp_getEventManager();
    int* cutId = NULL;
    cutId = dComIfGp_evmng_getMyIntegerP(i_staffId, "cutId");
    if (cutId == NULL) {
        OS_REPORT("-----CUT FALSE\n");
        return TRUE;
    }

    if (eventManager.getIsAddvance(i_staffId)) {
        _cutTutrialSelectGiveUp_Init(*cutId);
    }

    return _cutTutrialSelectGiveUp_Main(*cutId);
}

BOOL daNpc_Pachi_Maro_c::_cutTutrialSelectGiveUp_Init(int const& i_cutId) {
    switch (i_cutId) {
        case 10:
        case 11:
            break;

        case 20:
            break;
    }
    
    return TRUE;
}

BOOL daNpc_Pachi_Maro_c::_cutTutrialSelectGiveUp_Main(int const& i_cutId) {
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

BOOL daNpc_Pachi_Maro_c::cutTutrialGiveUp(int i_staffId) {
    dEvent_manager_c& eventManager = dComIfGp_getEventManager();
    int* cutId = NULL;
    cutId = dComIfGp_evmng_getMyIntegerP(i_staffId, "cutId");
    if (cutId == NULL) {
        OS_REPORT("-----CUT FALSE\n");
        return TRUE;
    }

    if (eventManager.getIsAddvance(i_staffId)) {
        _cutTutrialGiveUp_Init(*cutId);
    }

    return _cutTutrialGiveUp_Main(*cutId);
}

BOOL daNpc_Pachi_Maro_c::_cutTutrialGiveUp_Init(int const& i_cutId) {
    switch (i_cutId) {
        case 5:
            break;

        case 10:    
            break;

        case 20:
            break;
    }
    
    return TRUE;
}

BOOL daNpc_Pachi_Maro_c::_cutTutrialGiveUp_Main(int const& i_cutId) {
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

BOOL daNpc_Pachi_Maro_c::cutTutrialContinue(int i_staffId) {
    dEvent_manager_c& eventManager = dComIfGp_getEventManager();
    int* cutId = NULL;
    cutId = dComIfGp_evmng_getMyIntegerP(i_staffId, "cutId");
    if (cutId == NULL) {
        OS_REPORT("-----CUT FALSE\n");
        return TRUE;
    }

    if (eventManager.getIsAddvance(i_staffId)) {
        _cutTutrialContinue_Init(*cutId);
    }

    return _cutTutrialContinue_Main(*cutId);
}

BOOL daNpc_Pachi_Maro_c::_cutTutrialContinue_Init(int const& i_cutId) {
    switch (i_cutId) {
        default:
            return TRUE;
    }
}

BOOL daNpc_Pachi_Maro_c::_cutTutrialContinue_Main(int const& i_cutId) {
    BOOL rv = FALSE;

    switch (i_cutId) {
        case 10:
            _turn_pos(mTagPos, 0x1000);
            break;
    }

    return rv;
}

BOOL daNpc_Pachi_Maro_c::cutTutrialCaution(int i_staffId) {
    dEvent_manager_c& eventManager = dComIfGp_getEventManager();
    int* cutId = NULL;
    cutId = dComIfGp_evmng_getMyIntegerP(i_staffId, "cutId");
    if (cutId == NULL) {
        OS_REPORT("-----CUT FALSE\n");
        return TRUE;
    }

    if (eventManager.getIsAddvance(i_staffId)) {
        _cutTutrialCaution_Init(*cutId);
    }

    return _cutTutrialCaution_Main(*cutId);
}

BOOL daNpc_Pachi_Maro_c::_cutTutrialCaution_Init(int const& i_cutId) {
    switch (i_cutId) {
        case 5:
            break;

        case 10:
            break;
    }
    
    return TRUE;
}

BOOL daNpc_Pachi_Maro_c::_cutTutrialCaution_Main(int const& i_cutId) {
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

static int daNpc_Pachi_Maro_Create(void* i_this) {
    return static_cast<daNpc_Pachi_Maro_c*>(i_this)->create();
}

static int daNpc_Pachi_Maro_Delete(void* i_this) {
    return static_cast<daNpc_Pachi_Maro_c*>(i_this)->Delete();
}

static int daNpc_Pachi_Maro_Execute(void* i_this) {
    return static_cast<daNpc_Pachi_Maro_c*>(i_this)->Execute();
}

static int daNpc_Pachi_Maro_Draw(void* i_this) {
    return static_cast<daNpc_Pachi_Maro_c*>(i_this)->Draw();
}

static int daNpc_Pachi_Maro_IsDelete(void* i_this) {
    return 1;
}

static actor_method_class daNpc_Pachi_Maro_MethodTable = {
    (process_method_func)daNpc_Pachi_Maro_Create,
    (process_method_func)daNpc_Pachi_Maro_Delete,
    (process_method_func)daNpc_Pachi_Maro_Execute,
    (process_method_func)daNpc_Pachi_Maro_IsDelete,
    (process_method_func)daNpc_Pachi_Maro_Draw,
};

actor_process_profile_definition g_profile_NPC_PACHI_MARO = {
  fpcLy_CURRENT_e,               // mLayerID
  7,                             // mListID
  fpcPi_CURRENT_e,               // mListPrio
  PROC_NPC_PACHI_MARO,           // mProcName
  &g_fpcLf_Method.base,         // sub_method
  sizeof(daNpc_Pachi_Maro_c),    // mSize
  0,                             // mSizeOther
  0,                             // mParameters
  &g_fopAc_Method.base,          // sub_method
  370,                           // mPriority
  &daNpc_Pachi_Maro_MethodTable, // sub_method
  0x00044107,                    // mStatus
  fopAc_NPC_e,                   // mActorType
  fopAc_CULLBOX_CUSTOM_e,        // cullType
};
