/**
 * @file d_a_npc_ykm.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "SSystem/SComponent/c_counter.h"
#include "Z2AudioLib/Z2Instances.h"
#include "d/actor/d_a_midna.h"
#include "d/actor/d_a_npc_ykm.h"

#include "JSystem/JHostIO/JORFile.h"
#include "d/actor/d_a_obj_gadget.h"
#include "d/actor/d_a_tag_evtarea.h"
#include "d/d_debug_viewer.h"
#include "d/d_item.h"
#include "d/d_meter2_info.h"
#include "d/d_timer.h"
#include "d/d_tresure.h"


enum ykM_RES_File_ID {
    /* BCK */
    /* 0x06 */ BCK_YKM_F_ASK = 0x6,
    /* 0x07 */ BCK_YKM_F_LAUGH,
    /* 0x08 */ BCK_YKM_F_REGRET,
    /* 0x09 */ BCK_YKM_F_TALK_A,
    /* 0x0A */ BCK_YKM_F_TALK_B,
    /* 0x0B */ BCK_YKM_F_TALK_C,
    /* 0x0C */ BCK_YKM_FH_LAUGH,
    /* 0x0D */ BCK_YKM_FH_REGRET,
    /* 0x0E */ BCK_YKM_FUNWAIT,
    /* 0x0F */ BCK_YKM_STEP,
    /* 0x10 */ BCK_YKM_TALK_B,
    /* 0x11 */ BCK_YKM_TALK_C,
    /* 0x12 */ BCK_YKM_WAIT_A,
    /* 0x13 */ BCK_YKM_WALK,

    /* BMDR */
    /* 0x16 */ BMDR_YKM = 0x16,

    /* BTK */
    /* 0x19 */ BTK_YKM = 0x19,

    /* BTP */
    /* 0x1C */ BTP_YKM = 0x1C,
    /* 0x1D */ BTP_YKM_F_ASK,
    /* 0x1E */ BTP_YKM_F_LAUGH,
    /* 0x1F */ BTP_YKM_F_REGRET,
    /* 0x20 */ BTP_YKM_FH_LAUGH,
    /* 0x21 */ BTP_YKM_FH_REGRET,
};

enum ykM1_RES_File_ID {
    /* BCK */
    /* 0x07 */ BCK_YKM_DENY = 0x7,
    /* 0x08 */ BCK_YKM_F_HAPPY,
    /* 0x09 */ BCK_YKM_F_SHOUT,
    /* 0x0A */ BCK_YKM_F_SHUT,
    /* 0x0B */ BCK_YKM_F_TALK_D,
    /* 0x0C */ BCK_YKM_F_TALK_X,
    /* 0x0D */ BCK_YKM_F_TALK_Y,
    /* 0x0E */ BCK_YKM_FH_HAPPY,
    /* 0x0F */ BCK_YKM_HOLD,
    /* 0x10 */ BCK_YKM_HOLD_B,
    /* 0x11 */ BCK_YKM_PICKUP,
    /* 0x12 */ BCK_YKM_POUR,
    /* 0x13 */ BCK_YKM_PUSH,
    /* 0x14 */ BCK_YKM_RUN,
    /* 0x15 */ BCK_YKM_STOP,
    /* 0x16 */ BCK_YKM_TALK_D,
    /* 0x17 */ BCK_YKM_TALK_E,
    /* 0x18 */ BCK_YKM_WAIT_B,
    /* 0x19 */ BCK_YKM_WAIT_C,
    /* 0x1A */ BCK_YKM_WAIT_X,
    /* 0x1B */ BCK_YKM_WAITHOLD,
    /* 0x1C */ BCK_YKM_WAITHOLD_B_B,
    /* 0x1D */ BCK_YKM_WAITLIE,
    /* 0x1E */ BCK_YKM_WAITWAKE,
    /* 0x1F */ BCK_YKM_WAKE,

    /* BMDR */
    /* 0x22 */ BMDR_CHEESE = 0x22,
    /* 0x23 */ BMDR_TOMATO,
    /* 0x24 */ BMDR_YKM_CHEESE,
    /* 0x25 */ BMDR_YKM_TOMATO,

    /* BTK */
    /* 0x28 */ BTK_YKM_PICKUP = 0x28,
    /* 0x29 */ BTK_YKM_TALK_D,
    /* 0x2A */ BTK_YKM_WAIT_B,

    /* BTP */
    /* 0x2D */ BTP_YKM_F_HAPPY = 0x2D,
    /* 0x2E */ BTP_YKM_F_SHUT,
    /* 0x2F */ BTP_YKM_FH_HAPPY,

    /* EVT */
    /* 0x32 */ EVT_YKM1_EVENT_LIST = 0x32,
};

enum ykM2_RES_File_ID {
    /* BCK */
    /* 0x06 */ BCK_YKM_ASK = 0x6,
    /* 0x07 */ BCK_YKM_LAUGH,
    /* 0x08 */ BCK_YKM_REGRET,
    /* 0x09 */ BCK_YKM_REGRETWAIT,
    /* 0x0A */ BCK_YKM_SB_DAMAGE,
    /* 0x0B */ BCK_YKM_SB_GOAL,
    /* 0x0C */ BCK_YKM_SB_GOALWAIT,
    /* 0x0D */ BCK_YKM_SB_JUMP_A,
    /* 0x0E */ BCK_YKM_SB_JUMP_B,
    /* 0x0F */ BCK_YKM_SB_JUMP_C,
    /* 0x10 */ BCK_YKM_SB_TURNL,
    /* 0x11 */ BCK_YKM_SB_TURNR,
    /* 0x12 */ BCK_YKM_SB_WAIT,

    /* BMDR */
    /* 0x15 */ BMDR_YKM_LEAF = 0x15,

    /* BTK */
    /* 0x18 */ BTK_YKM_REGRET = 0x18,
    /* 0x19 */ BTK_YKM_REGRETWAIT,
    /* 0x1A */ BTK_YKM_SB_DAMAGE,

    /* EVT */
    /* 0x1D */ EVT_YKM2_EVENT_LIST,
};

enum ykM3_RES_File_ID {
    /* BCK */
    /* 0x06 */ BCK_FISH_ATTACK = 0x6,
    /* 0x07 */ BCK_FISH_JUMP,
    /* 0x08 */ BCK_FISH_SB_WAIT,
    /* 0x09 */ BCK_FISH_WAIT_A,
    /* 0x0A */ BCK_LEAF_FALL,
    /* 0x0B */ BCK_LEAF_WAIT_A,
    /* 0x0C */ BCK_LEAF_WAIT_B,
    /* 0x0D */ BCK_YKM_FISH_ASK,
    /* 0x0E */ BCK_YKM_FISH_ATTACK,
    /* 0x0F */ BCK_YKM_FISH_JUMP,
    /* 0x10 */ BCK_YKM_FISH_LAUGH,
    /* 0x11 */ BCK_YKM_FISH_LOOK,
    /* 0x12 */ BCK_YKM_FISH_LOOKWAIT,
    /* 0x13 */ BCK_YKM_FISH_REGRET,
    /* 0x14 */ BCK_YKM_FISH_REGRETWAIT,
    /* 0x15 */ BCK_YKM_FISH_TALK_B,
    /* 0x16 */ BCK_YKM_FISH_TALK_C,
    /* 0x17 */ BCK_YKM_FISH_WAIT_A,
    /* 0x18 */ BCK_YKM_FISH_WAIT_B,

    /* BMDR */
    /* 0x1B */ BMDR_FISH = 0x1B,
    /* 0x1C */ BMDR_LEAF,

    /* BTK */
    /* 0x1F */ BTK_YKM_FISH_ATTACK = 0x1F,
    /* 0x20 */ BTK_YKM_FISH_JUMP,
    /* 0x21 */ BTK_YKM_FISH_LOOK,
    /* 0x22 */ BTK_YKM_FISH_LOOKWAIT,
    /* 0x23 */ BTK_YKM_FISH_REGRET,
    /* 0x24 */ BTK_YKM_FISH_REGRETWAIT,
    /* 0x25 */ BTK_YKM_FISH_WAIT_B,

    /* EVT */
    /* 0x28 */ EVT_YKM3_EVENT_LIST = 0x28,
};

enum RES_Name {
    /* 0x1 */ YKM = 0x1,
    /* 0x2 */ YKM1,
    /* 0x3 */ YKM2,
    /* 0x4 */ YKM3,
};

daNpc_ykM_HIOParam const daNpc_ykM_Param_c::m = {
    500.0f,
    -4.0f,
    1.0f,
    1600.0f,
    255.0f,
    170.0f,
    30.0f,
    120.0f,
    0.0f,
    -15.0f,
    25.0f,
    -25.0f,
    30.0f,
    -15.0f,
    20.0f,
    -20.0f,
    0.8f,
    12.0f,
    7,
    6,
    9,
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
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    30,
    500.0f,
    300.0f,
    18.0f,
    50.0f,
    3.0f,
    20.0f,
    50.0f,
    56.0f,
    0.8f,
    52.0f,
    0x78,
    28.0f,
    1,
    5,
    0.6f,
};

#if DEBUG
daNpc_ykM_HIO_c::daNpc_ykM_HIO_c() {
    m = daNpc_ykM_Param_c::m;
}

void daNpc_ykM_HIO_c::listenPropertyEvent(const JORPropertyEvent* event) {
    size_t len;
    char buf[0x7d0];

    JORReflexible::listenPropertyEvent(event);
    JORFile file;
    switch ((int)event->id) {
    case 0x40000002:
        if (file.open(6, "すべてのファイル(*.*)\0*.*\0", NULL, NULL, NULL)) {
            memset(buf, 0, sizeof(buf));
            len = 0;
            daNpcT_cmnListenPropertyEvent(buf, (int*)&len, &m.common);
            sprintf(buf + len, "%d,   	//  ステップの速さ\n", m.step_speed);
            len = strlen(buf);
            sprintf(buf + len, "%.3ff,	//  注目距離\n", m.attention_dist);
            len = strlen(buf);
            sprintf(buf + len, "%.3ff,	//  会話距離\n", m.conversation_dist);
            len = strlen(buf);
            sprintf(buf + len, "%.3ff,	//  飛び出し速度\n", m.jump_speed);
            len = strlen(buf);
            sprintf(buf + len, "%.3ff,	//  飛び出し角度\n", m.push_out_angle);
            len = strlen(buf);
            sprintf(buf + len, "%.3ff,	//  飛ばしフレ－ム\n", m.skip_frame);
            len = strlen(buf);
            sprintf(buf + len, "%.3ff,	//  飛ばし速度\n", m.fly_speed);
            len = strlen(buf);
            sprintf(buf + len, "%.3ff,	//  飛ばし角度\n", m.fly_angle);
            len = strlen(buf);
            sprintf(buf + len, "%.3ff,	//  ジャンプ初速\n", m.jump_init_speed);
            len = strlen(buf);
            sprintf(buf + len, "%.3ff,	//  滑り加速\n", m.slide_acceleration);
            len = strlen(buf);
            sprintf(buf + len, "%.3ff,	//  滑り速度\n", m.slide_speed);
            len = strlen(buf);
            sprintf(buf + len, "%d,   	//  ウエイト時間\n", m.wait_time);
            len = strlen(buf);
            sprintf(buf + len, "%.3ff,	//  走り速度\n", m.run_speed);
            len = strlen(buf);
            sprintf(buf + len, "%d,   	//  競争パラメ－タａ\n", m.competition_prm_a);
            len = strlen(buf);
            sprintf(buf + len, "%d,   	//  競争パラメ－タｂ\n", m.competition_prm_b);
            len = strlen(buf);
            sprintf(buf + len, "%.3ff,	//  競争パラメ－タｃ\n", m.competition_prm_c);
            len = strlen(buf);
            file.writeData(buf, len);
            file.close();
            OS_REPORT("write append success!::%6d\n", len);
        } else {
            OS_REPORT("write append failure!\n");
        }
        break;
    }
}


void daNpc_ykM_HIO_c::genMessage(JORMContext* ctx) {
    daNpcT_cmnGenMessage(ctx, &m.common);
    ctx->genSlider("ステップの速さ  ", &m.step_speed, 0.0f, 100.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    ctx->genSlider("注目距離        ", &m.attention_dist, 0.0f, 10000.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    ctx->genSlider("会話距離        ", &m.conversation_dist, 0.0f, 10000.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    ctx->genSlider("飛び出し速度    ", &m.jump_speed, 0.0f, 100.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    ctx->genSlider("飛び出し角度    ", &m.push_out_angle, 0.0f, 90.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    ctx->genSlider("飛ばしフレ－ム  ", &m.skip_frame, 0.0f, 100.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    ctx->genSlider("飛ばし速度      ", &m.fly_speed, 0.0f, 100.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    ctx->genSlider("飛ばし角度      ", &m.fly_angle, 0.0f, 90.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    ctx->genSlider("ジャンプ初速    ", &m.jump_init_speed, 0.0f, 100.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    ctx->genSlider("滑り加速        ", &m.slide_acceleration, 0.0f, 8.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    ctx->genSlider("滑り速度        ", &m.slide_speed, 0.0f, 128.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    ctx->genSlider("ウエイト時間    ", &m.wait_time, 0.0f, 10000.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    ctx->genSlider("走り速度        ", &m.run_speed, 0.0f, 64.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    ctx->genSlider("競争パラメ－タａ", &m.competition_prm_a, -100.0f, 100.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    ctx->genSlider("競争パラメ－タｂ", &m.competition_prm_b, 1.0f, 100.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    ctx->genSlider("競争パラメ－タｃ", &m.competition_prm_c, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    ctx->genButton("ファイル書き出し", 0x40000002, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
}
#endif

static int l_bmdData[6][2] = {
    {BMDR_YKM, YKM},
    {BMDR_YKM_TOMATO, YKM1},
    {BMDR_YKM_LEAF, YKM2},
    {BMDR_YKM_CHEESE, YKM1},
    {BMDR_FISH, YKM3},
    {BMDR_LEAF, YKM3},
};

enum BmdIndex {
    BMD_INDEX_YKM,
    BMD_INDEX_YKM_TOMATO,
    BMD_INDEX_YKM_LEAF,
    BMD_INDEX_YKM_CHEESE,
    BMD_INDEX_FISH,
    BMD_INDEX_LEAF,
};

static daNpcT_evtData_c l_evtList[10] = {
    {"", 0},
    {"NO_RESPONSE", 0},
    {"SLIDEDOWN", 4},
    {"MEETING_AGAIN", 2},
    {"GET_TOMATOPUREE", 2},
    {"GET_TASTE", 2},
    {"FIND_WOLF", 3},
    {"START_SNOWBOARDRACE", 3},
    {"END_SNOWBOARDRACE", 3},
    {"HUG", 2},
};

static char* l_resNameList[5] = {
    "",
    "ykM",
    "ykM1",
    "ykM2",
    "ykM3",
};

static s8 l_loadResPtrn0[3] = {YKM, YKM1, -1};

static s8 l_loadResPtrn1[3] = {YKM, YKM2, -1};

static s8 l_loadResPtrn2[5] = {YKM, YKM1, YKM2, YKM3, -1};

static s8 l_loadResPtrn9[5] = {YKM, YKM1, YKM2, YKM3, -1};

static s8* l_loadResPtrnList[8] = {
    l_loadResPtrn0, l_loadResPtrn0, l_loadResPtrn2, l_loadResPtrn0,
    l_loadResPtrn1, l_loadResPtrn1, l_loadResPtrn1, l_loadResPtrn9,
};

static daNpcT_faceMotionAnmData_c l_faceMotionAnmData[16] = {
    {-1, J3DFrameCtrl::EMode_NONE, 0, BTP_YKM, J3DFrameCtrl::EMode_LOOP, YKM, TRUE},
    {BCK_YKM_F_TALK_A, J3DFrameCtrl::EMode_NONE, YKM, BTP_YKM, J3DFrameCtrl::EMode_LOOP, YKM, TRUE},
    {BCK_YKM_F_TALK_B, J3DFrameCtrl::EMode_NONE, YKM, BTP_YKM, J3DFrameCtrl::EMode_LOOP, YKM, TRUE},
    {BCK_YKM_F_TALK_C, J3DFrameCtrl::EMode_NONE, YKM, BTP_YKM, J3DFrameCtrl::EMode_LOOP, YKM, TRUE},
    {BCK_YKM_F_TALK_D, J3DFrameCtrl::EMode_NONE, YKM1, BTP_YKM, J3DFrameCtrl::EMode_LOOP, YKM, TRUE},
    {BCK_YKM_F_REGRET, J3DFrameCtrl::EMode_NONE, YKM, BTP_YKM_F_REGRET, J3DFrameCtrl::EMode_NONE, YKM, FALSE},
    {BCK_YKM_F_LAUGH, J3DFrameCtrl::EMode_NONE, YKM, BTP_YKM_F_LAUGH, J3DFrameCtrl::EMode_NONE, YKM, FALSE},
    {BCK_YKM_F_ASK, J3DFrameCtrl::EMode_NONE, YKM, BTP_YKM_F_ASK, J3DFrameCtrl::EMode_NONE, YKM, FALSE},
    {BCK_YKM_FH_LAUGH, J3DFrameCtrl::EMode_LOOP, YKM, BTP_YKM_FH_LAUGH, J3DFrameCtrl::EMode_LOOP, YKM, FALSE},
    {BCK_YKM_FH_REGRET, J3DFrameCtrl::EMode_LOOP, YKM, BTP_YKM_FH_REGRET, J3DFrameCtrl::EMode_LOOP, YKM, FALSE},
    {BCK_YKM_F_TALK_X, J3DFrameCtrl::EMode_NONE, YKM1, BTP_YKM, J3DFrameCtrl::EMode_LOOP, YKM, TRUE},
    {BCK_YKM_F_TALK_Y, J3DFrameCtrl::EMode_NONE, YKM1, BTP_YKM, J3DFrameCtrl::EMode_LOOP, YKM, TRUE},
    {BCK_YKM_F_SHOUT, J3DFrameCtrl::EMode_NONE, YKM1, BTP_YKM, J3DFrameCtrl::EMode_LOOP, YKM, TRUE},
    {BCK_YKM_F_SHUT, J3DFrameCtrl::EMode_NONE, YKM1, BTP_YKM_F_SHUT, J3DFrameCtrl::EMode_NONE, YKM1, FALSE},
    {BCK_YKM_F_HAPPY, J3DFrameCtrl::EMode_NONE, YKM1, BTP_YKM_F_HAPPY, J3DFrameCtrl::EMode_NONE, YKM1, FALSE},
    {BCK_YKM_FH_HAPPY, J3DFrameCtrl::EMode_LOOP, YKM1, BTP_YKM_FH_HAPPY, J3DFrameCtrl::EMode_LOOP, YKM1, FALSE},
};

static daNpcT_motionAnmData_c l_motionAnmData[51] = {
    {BCK_YKM_WAIT_A, J3DFrameCtrl::EMode_LOOP, YKM, BTK_YKM, J3DFrameCtrl::EMode_NONE, YKM, 1, 0},
    {BCK_YKM_WAIT_B, J3DFrameCtrl::EMode_LOOP, YKM1, BTK_YKM_WAIT_B, J3DFrameCtrl::EMode_NONE, YKM1, 0, 0},
    {BCK_YKM_WAIT_C, J3DFrameCtrl::EMode_LOOP, YKM1, BTK_YKM, J3DFrameCtrl::EMode_NONE, YKM, 1, 0},
    {-1, J3DFrameCtrl::EMode_NONE, 0, -1, J3DFrameCtrl::EMode_NONE, 0, 0, 0},
    {BCK_YKM_WAIT_X, J3DFrameCtrl::EMode_LOOP, YKM1, BTK_YKM, J3DFrameCtrl::EMode_NONE, YKM, 1, 0},
    {BCK_YKM_FUNWAIT, J3DFrameCtrl::EMode_LOOP, YKM, BTK_YKM, J3DFrameCtrl::EMode_NONE, YKM, 1, 0},
    {BCK_YKM_REGRETWAIT, J3DFrameCtrl::EMode_LOOP, YKM2, BTK_YKM_REGRETWAIT, J3DFrameCtrl::EMode_LOOP, YKM2, 0, 0},
    {BCK_YKM_TALK_B, J3DFrameCtrl::EMode_NONE, YKM, BTK_YKM, J3DFrameCtrl::EMode_NONE, YKM, 1, 0},
    {BCK_YKM_TALK_C, J3DFrameCtrl::EMode_NONE, YKM, BTK_YKM, J3DFrameCtrl::EMode_NONE, YKM, 1, 0},
    {BCK_YKM_TALK_D, J3DFrameCtrl::EMode_NONE, YKM1, BTK_YKM_TALK_D, J3DFrameCtrl::EMode_NONE, YKM1, 0, 0},
    {BCK_YKM_TALK_E, J3DFrameCtrl::EMode_NONE, YKM1, BTK_YKM, J3DFrameCtrl::EMode_NONE, YKM, 1, 0},
    {-1, J3DFrameCtrl::EMode_NONE, 0, -1, J3DFrameCtrl::EMode_NONE, 0, 0, 0},
    {BCK_YKM_PUSH, J3DFrameCtrl::EMode_NONE, YKM1, BTK_YKM, J3DFrameCtrl::EMode_NONE, YKM, 1, 0},
    {BCK_YKM_PICKUP, J3DFrameCtrl::EMode_NONE, YKM1, BTK_YKM_PICKUP, J3DFrameCtrl::EMode_NONE, YKM1, 0, 0},
    {BCK_YKM_POUR, J3DFrameCtrl::EMode_NONE, YKM1, BTK_YKM, J3DFrameCtrl::EMode_NONE, YKM, 1, 0},
    {BCK_YKM_REGRET, J3DFrameCtrl::EMode_NONE, YKM2, BTK_YKM_REGRET, J3DFrameCtrl::EMode_NONE, YKM2, 0, 0},
    {BCK_YKM_LAUGH, J3DFrameCtrl::EMode_NONE, YKM2, BTK_YKM, J3DFrameCtrl::EMode_NONE, YKM, 1, 0},
    {BCK_YKM_ASK, J3DFrameCtrl::EMode_NONE, YKM2, BTK_YKM, J3DFrameCtrl::EMode_NONE, YKM, 1, 0},
    {BCK_YKM_WALK, J3DFrameCtrl::EMode_LOOP, YKM, BTK_YKM, J3DFrameCtrl::EMode_NONE, YKM, 1, 0},
    {BCK_YKM_STEP, J3DFrameCtrl::EMode_NONE, YKM, BTK_YKM, J3DFrameCtrl::EMode_NONE, YKM, 1, 0},
    {BCK_YKM_FISH_WAIT_A, J3DFrameCtrl::EMode_LOOP, YKM3, BTK_YKM, J3DFrameCtrl::EMode_NONE, YKM, 1, 0},
    {BCK_YKM_FISH_ATTACK, J3DFrameCtrl::EMode_NONE, YKM3, BTK_YKM_FISH_ATTACK, J3DFrameCtrl::EMode_NONE, YKM3, 0, 0},
    {BCK_YKM_FISH_WAIT_B, J3DFrameCtrl::EMode_LOOP, YKM3, BTK_YKM_FISH_WAIT_B, J3DFrameCtrl::EMode_LOOP, YKM3, 0, 0},
    {BCK_YKM_FISH_JUMP, J3DFrameCtrl::EMode_NONE, YKM3, BTK_YKM_FISH_JUMP, J3DFrameCtrl::EMode_NONE, YKM3, 0, 0},
    {BCK_YKM_FISH_TALK_B, J3DFrameCtrl::EMode_NONE, YKM3, BTK_YKM, J3DFrameCtrl::EMode_NONE, YKM, 1, 0},
    {BCK_YKM_FISH_TALK_C, J3DFrameCtrl::EMode_NONE, YKM3, BTK_YKM, J3DFrameCtrl::EMode_NONE, YKM, 1, 0},
    {BCK_YKM_FISH_REGRET, J3DFrameCtrl::EMode_NONE, YKM3, BTK_YKM_FISH_REGRET, J3DFrameCtrl::EMode_NONE, YKM3, 0, 0},
    {BCK_YKM_FISH_REGRETWAIT, J3DFrameCtrl::EMode_LOOP, YKM3, BTK_YKM_FISH_REGRETWAIT, J3DFrameCtrl::EMode_LOOP, YKM3, 0, 0},
    {BCK_YKM_FISH_LAUGH, J3DFrameCtrl::EMode_NONE, YKM3, BTK_YKM, J3DFrameCtrl::EMode_NONE, YKM, 1, 0},
    {BCK_YKM_FISH_ASK, J3DFrameCtrl::EMode_NONE, YKM3, BTK_YKM, J3DFrameCtrl::EMode_NONE, YKM, 1, 0},
    {BCK_YKM_FISH_LOOK, J3DFrameCtrl::EMode_NONE, YKM3, BTK_YKM_FISH_LOOK, J3DFrameCtrl::EMode_NONE, YKM3, 0, 0},
    {BCK_YKM_FISH_LOOKWAIT, J3DFrameCtrl::EMode_LOOP, YKM3, BTK_YKM_FISH_LOOKWAIT, J3DFrameCtrl::EMode_LOOP, YKM3, 0, 0},
    {BCK_YKM_SB_WAIT, J3DFrameCtrl::EMode_LOOP, YKM2, BTK_YKM, J3DFrameCtrl::EMode_NONE, YKM, 1, 0},
    {BCK_YKM_SB_JUMP_A, J3DFrameCtrl::EMode_NONE, YKM2, BTK_YKM, J3DFrameCtrl::EMode_NONE, YKM, 1, 0},
    {BCK_YKM_SB_JUMP_B, J3DFrameCtrl::EMode_LOOP, YKM2, BTK_YKM, J3DFrameCtrl::EMode_NONE, YKM, 1, 0},
    {BCK_YKM_SB_JUMP_C, J3DFrameCtrl::EMode_NONE, YKM2, BTK_YKM, J3DFrameCtrl::EMode_NONE, YKM, 1, 0},
    {BCK_YKM_SB_TURNL, J3DFrameCtrl::EMode_LOOP, YKM2, BTK_YKM, J3DFrameCtrl::EMode_NONE, YKM, 1, 0},
    {BCK_YKM_SB_TURNR, J3DFrameCtrl::EMode_LOOP, YKM2, BTK_YKM, J3DFrameCtrl::EMode_NONE, YKM, 1, 0},
    {BCK_YKM_SB_DAMAGE, J3DFrameCtrl::EMode_NONE, YKM2, BTK_YKM_SB_DAMAGE, J3DFrameCtrl::EMode_NONE, YKM2, 0, 0},
    {BCK_YKM_SB_GOAL, J3DFrameCtrl::EMode_NONE, YKM2, BTK_YKM, J3DFrameCtrl::EMode_NONE, YKM, 1, 0},
    {BCK_YKM_SB_GOALWAIT, J3DFrameCtrl::EMode_LOOP, YKM2, BTK_YKM, J3DFrameCtrl::EMode_NONE, YKM, 1, 0},
    {BCK_YKM_DENY, J3DFrameCtrl::EMode_NONE, YKM1, BTK_YKM, J3DFrameCtrl::EMode_NONE, YKM, 1, 0},
    {BCK_YKM_RUN, J3DFrameCtrl::EMode_LOOP, YKM1, BTK_YKM, J3DFrameCtrl::EMode_NONE, YKM, 1, 0},
    {BCK_YKM_STOP, J3DFrameCtrl::EMode_NONE, YKM1, BTK_YKM, J3DFrameCtrl::EMode_NONE, YKM, 1, 0},
    {BCK_YKM_WAITLIE, J3DFrameCtrl::EMode_LOOP, YKM1, BTK_YKM, J3DFrameCtrl::EMode_NONE, YKM, 1, 0},
    {BCK_YKM_WAKE, J3DFrameCtrl::EMode_NONE, YKM1, BTK_YKM, J3DFrameCtrl::EMode_NONE, YKM, 1, 0},
    {BCK_YKM_WAITWAKE, J3DFrameCtrl::EMode_LOOP, YKM1, BTK_YKM, J3DFrameCtrl::EMode_NONE, YKM, 1, 3},
    {BCK_YKM_HOLD, J3DFrameCtrl::EMode_NONE, YKM1, BTK_YKM, J3DFrameCtrl::EMode_NONE, YKM, 1, 0},
    {BCK_YKM_WAITHOLD, J3DFrameCtrl::EMode_LOOP, YKM1, BTK_YKM, J3DFrameCtrl::EMode_NONE, YKM, 1, 0},
    {BCK_YKM_HOLD_B, J3DFrameCtrl::EMode_NONE, YKM1, BTK_YKM, J3DFrameCtrl::EMode_NONE, YKM, 1, 0},
    {BCK_YKM_WAITHOLD_B_B, J3DFrameCtrl::EMode_LOOP, YKM1, BTK_YKM, J3DFrameCtrl::EMode_NONE, YKM, 1, 0},
};

static daNpcT_MotionSeqMngr_c::sequenceStepData_c l_faceMotionSequenceData[64] = {
    {1, -1, 1}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {2, -1, 1}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {3, -1, 1}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {4, -1, 1}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {5, -1, 1}, {9, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {6, -1, 1}, {8, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {7, -1, 1}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {0xC, -1, 1}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {0xD, -1, 1}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {0xA, -1, 1}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {0xE, -1, 1}, {0xF, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {0xF, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {0xB, -1, 1}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {9, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {8, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {0, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
};

static daNpcT_MotionSeqMngr_c::sequenceStepData_c l_motionSequenceData[188] = {
    {0, -9, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {1, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {7, -1, 1}, {0, -1, 0}, {-1, 0, 0}, {-1, 0, 0},
    {8, -1, 1}, {0, -1, 0}, {-1, 0, 0}, {-1, 0, 0},
    {9, -1, 1}, {1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {0xA, -1, 1}, {0, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {0xC, -1, 1}, {0, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {0xD, -1, 1}, {0, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {0xD, -1, 1}, {2, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {2, -9, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {5, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {0xF, -1, 1}, {6, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {0x10, -1, 1}, {0, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {0x11, -1, 1}, {0, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {0x14, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {0x18, -1, 1}, {0x14, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {0x19, -1, 1}, {0x14, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {0x1A, -1, 1}, {0x1B, -1, 0}, {-1, 0, 0}, {-1, 0, 0},
    {0x1C, -1, 1}, {0x14, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {0x1D, -1, 1}, {0x14, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {0x1E, -1, 1}, {0x1F, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {0x2C, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {0x2D, -1, 1}, {0x2E, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {0x29, -1, 1}, {0x2E, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {0x2E, -9, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {0x2F, -1, 1}, {0x30, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {0x30, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {4, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {0x32, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {0x12, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {0x13, 4, 1}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {0xE, -1, 1}, {0, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {0x20, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {0x21, 0, 1}, {0x22, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {0x23, 4, 1}, {0x20, 4, 0}, {-1, 0, 0}, {-1, 0, 0},
    {0x2A, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {0x2B, -1, 1}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {0x31, -1, 1}, {0x32, 0, 1}, {-1, 0, 0}, {-1, 0, 0},
    {3, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {0xB, -1, 1}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {0x26, 0, 1}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {0x14, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {0x15, -1, 1}, {0x16, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {0x17, -1, 1}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {0x24, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {0x25, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {0x27, -1, 1}, {0x28, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
};

char* daNpc_ykM_c::mCutNameList[10] = {
    "",
    "SLIDEDOWN",
    "MEETING_AGAIN",
    "GET_TOMATOPUREE",
    "GET_TASTE",
    "LV5DUNGEON_CLEAR",
    "FIND_WOLF",
    "START_SNOWBOARDRACE",
    "END_SNOWBOARDRACE",
    "HUG"
};

daNpc_ykM_c::cutFunc daNpc_ykM_c::mCutList[10] = {
    NULL,
    &daNpc_ykM_c::cutSlideDown,
    &daNpc_ykM_c::cutMeetingAgain,
    &daNpc_ykM_c::cutGetTomatoPuree,
    &daNpc_ykM_c::cutGetTaste,
    &daNpc_ykM_c::cutLv5DungeonClear,
    &daNpc_ykM_c::cutFindWolf,
    &daNpc_ykM_c::cutStartSnowboardRace,
    &daNpc_ykM_c::cutEndSnowboardRace,
    &daNpc_ykM_c::cutHug,
};

static cXyz l_SBRaceStartPos;

static csXyz l_SBRaceStartAngle;

static NPC_YKM_HIO_CLASS l_HIO;

daNpc_ykM_c::~daNpc_ykM_c() {
    OS_REPORT("|%06d:%x|daNpc_ykM_c -> デストラクト\n", g_Counter.mCounter0, this);

    if (mpMorf[0] != NULL) {
        mpMorf[0]->stopZelAnime();
    }

    if (mFishModelMorf != NULL) {
        mFishModelMorf->stopZelAnime();
    }

    if (mLeafModelMorf != NULL) {
        mLeafModelMorf->stopZelAnime();
    }

#if DEBUG
    if (mpHIO != NULL) {
        mpHIO->removeHIO();
    }
#endif

    deleteRes(l_loadResPtrnList[mType], (const char**)l_resNameList);

    if (field_0x157b != 0) {
        mDoAud_subBgmStop();
    }

    daNpcT_offTmpBit(0x54); // dSv_event_tmp_flag_c::T_0084 - Snowpeak mountain - Racing with Yeto
}

cPhs__Step daNpc_ykM_c::create() {
    daNpcT_ct(this, daNpc_ykM_c, l_faceMotionAnmData, l_motionAnmData, l_faceMotionSequenceData,
                                                  4, l_motionSequenceData, 4, l_evtList, l_resNameList);
    mType = getType();
    mFlowNodeNo = getFlowNodeNo();
    mTwilight = false;

    cPhs__Step phase = (cPhs__Step)loadRes(l_loadResPtrnList[mType], (const char**)l_resNameList);
    if (phase == cPhs_COMPLEATE_e) {
        OS_REPORT("\t(%s:%d) flowNo:%d, PathID:%02x, BitTRB:%02x<%08x> ", fopAcM_getProcNameString(this), mType, mFlowNodeNo,
                  getPathID(), getBitTRB() & 0xFF, fopAcM_GetParam(this));

        if (isDelete()) {
            OS_REPORT("===>isDelete:TRUE\n");
            return cPhs_ERROR_e;
        }

        static int const heapSize[8] = {
            0x8000, 0x5460, 0x8000, 0x4460,
            0x8000, 0x8000, 0x8000, 0x8000,
        };
        OS_REPORT("\n");
        OS_REPORT("YKM: %u 0x%04x\n", mType, heapSize[mType]);
        if (!fopAcM_entrySolidHeap(this, createHeapCallBack, heapSize[mType])) {
            return cPhs_ERROR_e;
        }

        J3DModelData* modelData = mpMorf[0]->getModel()->getModelData();
        fopAcM_SetMtx(this, mpMorf[0]->getModel()->getBaseTRMtx());
        fopAcM_setCullSizeBox(this, -400.0f, -100.0f, -500.0f, 400.0f, 600.0f, 500.0f);

        mSound.init(&current.pos, &eyePos, 3, 1);

#if DEBUG
        mpHIO = &l_HIO;
        mpHIO->entryHIO("雪男");
#endif

        reset();
        mAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1, &mAcchCir,
                  fopAcM_GetSpeed_p(this), fopAcM_GetAngle_p(this), fopAcM_GetShapeAngle_p(this));
        mCcStts.Init(mpHIO->m.common.weight, 0, this);

        for (int i = 0; i < 4; i++) {
            field_0xf94[i].Set(mCcDSph);
            field_0xf94[i].SetStts(&mCcStts);
            field_0xf94[i].SetTgHitCallback(tgHitCallBack);
        }

        field_0xe58.Set(mCcDCyl);
        field_0xe58.SetStts(&mCcStts);
        field_0xe58.SetTgHitCallback(tgHitCallBack);

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

int daNpc_ykM_c::CreateHeap() {
    void* mdlData_p = NULL;
    J3DModel* model = NULL;

    int sp40 = mTwilight == 1 ? BMD_INDEX_YKM : BMD_INDEX_YKM;
    int arcNameIdx = l_bmdData[sp40][1];
    int resIndex = l_bmdData[sp40][0];
    mdlData_p = dComIfG_getObjectRes(l_resNameList[arcNameIdx], resIndex);
    if (mdlData_p == NULL) {
        return 1;
    }

    int sp34 = 0x11020284;
    mpMorf[0] = new mDoExt_McaMorfSO((J3DModelData*)mdlData_p, NULL, NULL, NULL, -1, 1.0f, 0, -1, &mSound, 0x80000, sp34);
    if (mpMorf[0] == NULL || mpMorf[0]->getModel() == NULL) {
        return 0;
    }

    model = mpMorf[0]->getModel();
    for (u16 i = 0; i < ((J3DModelData*)mdlData_p)->getJointNum(); i++) {
        ((J3DModelData*)mdlData_p)->getJointNodePointer(i)->setCallBack(ctrlJointCallBack);
    }
    model->setUserArea((uintptr_t)this);

    mpMatAnm[0] = new daNpcT_MatAnm_c();
    if (mpMatAnm[0] == NULL) {
        return 0;
    }

    static BmdIndex const bmdTypeList[3] = {BMD_INDEX_YKM_TOMATO, BMD_INDEX_YKM_LEAF, BMD_INDEX_YKM_CHEESE};
    switch (mType) {
    case TYPE_COOK:
        for (int i = 0; i < 3; i++) {
            if (l_bmdData[bmdTypeList[i]][0] >= 0) {
                mdlData_p = (J3DModelData*)dComIfG_getObjectRes(l_resNameList[l_bmdData[bmdTypeList[i]][1]], l_bmdData[bmdTypeList[i]][0]);
            } else {
                mdlData_p = NULL;
            }

            // field_0xe4c holds a Pumpkin, Leaf, and Cheese model (in that order)
            if (mdlData_p != NULL) {
                field_0xe4c[i] = mDoExt_J3DModel__create((J3DModelData*)mdlData_p, 0x80000, 0x11000084);
            } else {
                field_0xe4c[i] = NULL;
            }
        }
        break;

    case TYPE_2:
        sp40 = BMD_INDEX_FISH;
        arcNameIdx = l_bmdData[sp40][1];
        resIndex = l_bmdData[sp40][0];
        mdlData_p = (J3DModelData*)dComIfG_getObjectRes(l_resNameList[arcNameIdx], resIndex);

        JUT_ASSERT(1595, NULL != mdlData_p);

        sp34 = 0x11000084;
        mFishModelMorf = new mDoExt_McaMorfSO((J3DModelData*)mdlData_p, NULL, NULL, NULL, -1, 1.0f, 0, -1,
                                           NULL, 0x80000, sp34);
        if (mFishModelMorf == NULL || mFishModelMorf->getModel() == NULL) {
            return 0;
        }

        sp40 = BMD_INDEX_LEAF;
        arcNameIdx = l_bmdData[sp40][1];
        resIndex = l_bmdData[sp40][0];
        mdlData_p = (J3DModelData*)dComIfG_getObjectRes(l_resNameList[arcNameIdx], resIndex);

        JUT_ASSERT(1622, NULL != mdlData_p);

        sp34 = 0x11000084;
        mLeafModelMorf = new mDoExt_McaMorfSO((J3DModelData*)mdlData_p, NULL, NULL, NULL, -1, 1.0f, 0, -1,
                                           NULL, 0x80000, sp34);
        if (mLeafModelMorf == NULL || mLeafModelMorf->getModel() == NULL) {
            return 0;
        }
        setLeafAnm(ANM_LEAF_WAIT_A, 0.0f);

        mdlData_p = (J3DModelData*)dComIfG_getObjectRes(l_resNameList[l_bmdData[bmdTypeList[1]][1]], l_bmdData[bmdTypeList[1]][0]);
        if (mdlData_p != NULL) {
            field_0xe4c[1] = mDoExt_J3DModel__create((J3DModelData*)mdlData_p, 0x80000, 0x11000084);
        } else {
            field_0xe4c[1] = NULL;
        }
        break;

    case TYPE_3:
        break;

    case TYPE_4:
        mdlData_p = (J3DModelData*)dComIfG_getObjectRes(l_resNameList[l_bmdData[bmdTypeList[1]][1]], l_bmdData[bmdTypeList[1]][0]);
        if (mdlData_p != NULL) {
            field_0xe4c[1] = mDoExt_J3DModel__create((J3DModelData*)mdlData_p, 0x80000, 0x11000084);
        } else {
            field_0xe4c[1] = NULL;
        }
        break;
    }

    if (setFaceMotionAnm(1, false) && setMotionAnm(0, 0.0f, FALSE)) {
        return 1;
    }

    return 0;
}

int daNpc_ykM_c::Delete() {
    OS_REPORT("|%06d:%x|daNpc_ykM_c -> Delete\n", g_Counter.mCounter0, this);
    fpc_ProcID unusedId = fopAcM_GetID(this);
    this->~daNpc_ykM_c();
    return 1;
}

static void dummy_string() {
    OS_REPORT("Delete -> NPC_YKM(id=%d)\n", 0);
}

int daNpc_ykM_c::Execute() {
    return execute();
}

int daNpc_ykM_c::Draw() {
    J3DModelData* mdlData_p = mpMorf[0]->getModel()->getModelData();
    int rv = 0;

    if (mpMatAnm[0] != NULL) {
        mdlData_p->getMaterialNodePointer(getEyeballMaterialNo())->setMaterialAnm(mpMatAnm[0]);
    }

    if (field_0x157a != 0) {
        mdlData_p->getMaterialNodePointer(3)->getShape()->hide();
    }

    rv = draw(
        #if DEBUG
        chkAction(&daNpc_ykM_c::test),
        #else
        FALSE,
        #endif
        FALSE, mRealShadowSize, NULL, 0.0f, FALSE, field_0x1581 == 1, FALSE
    );

    if (field_0x157a != 0) {
        mdlData_p->getMaterialNodePointer(3)->getShape()->show();
    }

    return rv;
}

int daNpc_ykM_c::createHeapCallBack(fopAc_ac_c* a_this) {
    daNpc_ykM_c* i_this = (daNpc_ykM_c*)a_this;
    return i_this->CreateHeap();
}

int daNpc_ykM_c::ctrlJointCallBack(J3DJoint* i_joint, int param_2) {
    if (param_2 == 0) {
        J3DModel* model = j3dSys.getModel();
        daNpc_ykM_c* i_this = (daNpc_ykM_c*)model->getUserArea();
        if (i_this != NULL) {
            i_this->ctrlJoint(i_joint, model);
        }
    }

    return 1;
}

void* daNpc_ykM_c::srchGadget(void* i_actor, void* i_data) {
    fopAc_ac_c* data = (fopAc_ac_c*)i_data;
    if (mFindCount < 50) {
        fopAc_ac_c* actor = (fopAc_ac_c*)i_actor;
        if (actor != NULL && actor != data) {
            if (fopAcM_IsExecuting(fopAcM_GetID(actor)) && fopAcM_GetName(actor) == PROC_OBJ_GADGET) {
                mFindActorPtrs[mFindCount] = actor;
                mFindCount++;
            }
        }
    }

    return NULL;
}

fopAc_ac_c* daNpc_ykM_c::getTomatoPureeP() {
    fopAc_ac_c* actor = NULL;
    f32 fVar1 = G_CM3D_F_INF;
    mFindCount = 0;

    fopAcM_Search(srchGadget, this);

    for (int i = 0; i < mFindCount; i++) {
        if (((daObj_Gadget_c*)mFindActorPtrs[i])->getType() == 0 && fopAcM_searchActorDistance(this, mFindActorPtrs[i]) < fVar1) {
            fVar1 = fopAcM_searchActorDistance(this, mFindActorPtrs[i]);
            actor = mFindActorPtrs[i];
        }
    }

    return actor;
}

fopAc_ac_c* daNpc_ykM_c::getCheeseP() {
    fopAc_ac_c* actor = NULL;
    f32 fVar1 = G_CM3D_F_INF;
    mFindCount = 0;

    fopAcM_Search(srchGadget, this);

    for (int i = 0; i < mFindCount; i++) {
        if (((daObj_Gadget_c*)mFindActorPtrs[i])->getType() == 1 && fopAcM_searchActorDistance(this, mFindActorPtrs[i]) < fVar1) {
            fVar1 = fopAcM_searchActorDistance(this, mFindActorPtrs[i]);
            actor = mFindActorPtrs[i];
        }
    }

    return actor;
}

void* daNpc_ykM_c::srchYkm(void* i_actor, void* i_data) {
    fopAc_ac_c* data = (fopAc_ac_c*)i_data;
    if (mFindCount < 50) {
        fopAc_ac_c* actor = (fopAc_ac_c*)i_actor;
        if (actor != NULL && actor != data) {
            if (fopAcM_IsExecuting(fopAcM_GetID(actor)) && fopAcM_GetName(actor) == PROC_NPC_YKM) {
                mFindActorPtrs[mFindCount] = actor;
                mFindCount++;
            }
        }
    }

    return NULL;
}

fopAc_ac_c* daNpc_ykM_c::getOtherYkmP(int param_1) {
    fopAc_ac_c* actor = NULL;
    f32 fVar1 = G_CM3D_F_INF;
    mFindCount = 0;

    fopAcM_Search(srchYkm, this);

    for (int i = 0; i < mFindCount; i++) {
        if (param_1 == ((daNpc_ykM_c*)mFindActorPtrs[i])->getType() &&
            fopAcM_searchActorDistance(this, mFindActorPtrs[i]) < fVar1) {
            fVar1 = fopAcM_searchActorDistance(this, mFindActorPtrs[i]);
            actor = mFindActorPtrs[i];
        }
    }

    return actor;
}

void* daNpc_ykM_c::srchYkw(void* i_actor, void* i_data) {
    fopAc_ac_c* data = (fopAc_ac_c*)i_data;
    if (mFindCount < 50) {
        fopAc_ac_c* actor = (fopAc_ac_c*)i_actor;
        if (actor != NULL && actor != data) {
            if (fopAcM_IsExecuting(fopAcM_GetID(actor)) && fopAcM_GetName(actor) == PROC_NPC_YKW) {
                mFindActorPtrs[mFindCount] = actor;
                mFindCount++;
            }
        }
    }

    return NULL;
}

fopAc_ac_c* daNpc_ykM_c::getOtherYkwP(int param_1) {
    fopAc_ac_c* actor = NULL;
    f32 fVar1 = G_CM3D_F_INF;
    mFindCount = 0;

    fopAcM_Search(srchYkw, this);

    for (int i = 0; i < mFindCount; i++) {
        if (param_1 == ((daNpc_ykW_c*)mFindActorPtrs[i])->getType() && fopAcM_searchActorDistance(this, mFindActorPtrs[i]) < fVar1) {
            fVar1 = fopAcM_searchActorDistance(this, mFindActorPtrs[i]);
            actor = mFindActorPtrs[i];
        }
    }

    return actor;
}

u8 daNpc_ykM_c::getType() {
    u8 param = fopAcM_GetParam(this) & 0xFF;
    switch (param) {
        case 0:
            return TYPE_0;

        case 1:
            return TYPE_COOK;

        case 2:
            return TYPE_2;

        case 3:
            return TYPE_3;

        case 4:
            return TYPE_4;

        case 5:
            return TYPE_5;

        case 6:
            return TYPE_6;

        default:
            return TYPE_7;
    }
}

BOOL daNpc_ykM_c::isDelete() {
    switch (mType) {
        case TYPE_0:
            return FALSE;

        case TYPE_COOK:
            return dComIfGs_isDungeonItemBossKey();

        case TYPE_2:
            return daNpcT_chkEvtBit(0x134); // dSv_event_flag_c::F_0308 - Snowpeak mountain - Watched first meeting event with Yeto at top of mountain (human)

        case TYPE_3:
            return dComIfGs_isStageBossEnemy();

        case TYPE_4:
            return FALSE;

        case TYPE_5:
            return FALSE;

        case TYPE_6:
            return FALSE;

        default:
            return TRUE;
    }
}

void daNpc_ykM_c::reset() {
    csXyz angle;
    int iVar1 = (u8*)&field_0x1588 - (u8*)&mpNextAction;

    if (mpMatAnm[0] != NULL) {
        mpMatAnm[0]->initialize();
    }
    initialize();
    for (int i = 0; i < 8; i++) {
        mActorMngr[i].initialize();
    }

    memset(&mpNextAction, 0, iVar1);

    if (getPathID() != 0xFF) {
        mPath.initialize();
        mPath.setPathInfo(getPathID(), fopAcM_GetRoomNo(this), 0);

        mRoomPath = NULL;
        if (mType == TYPE_4 && mPath.chkNextId()) {
            mRoomPath = dPath_GetRoomPath(mPath.getPathInfo()->m_nextID, fopAcM_GetRoomNo(this));
        }
    }

    angle.setall(0);
    angle.y = home.angle.y;

    switch (mType) {
        case TYPE_0:
        case TYPE_COOK:
            break;
        case TYPE_2:
            field_0x1568 = -20.0f;
            field_0x1579 = 1;
            eventInfo.setIdx(0);
            break;

        case TYPE_3:
            mHide = true;
            fopAcM_OnStatus(this, fopAcM_STATUS_UNK_0x8000000);
            break;

        case TYPE_4:
            eventInfo.setIdx(0);
            break;

        case TYPE_5:
            mHide = true;
            fopAcM_OnStatus(this, fopAcM_STATUS_UNK_0x8000000);
            eventInfo.setIdx(1);
            break;

        case TYPE_6:
            mHide = true;
            fopAcM_OnStatus(this, fopAcM_STATUS_UNK_0x8000000);
            eventInfo.setIdx(2);
            break;
    }

    daNpcT_offTmpBit(0x53); // dSv_event_tmp_flag_c::T_0083 - LV5 Dungeon - Speak with Yeta at top of mountain as wolf after clearing Snowpeak Ruins
    daNpcT_offTmpBit(0x54); // dSv_event_tmp_flag_c::T_0084 - Snowpeak mountain - Racing with Yeto
    daNpcT_offTmpBit(0x55); // dSv_event_tmp_flag_c::T_0085 - Snowpeak mountain - Racing with Yeta
    setAngle(angle);
}

void daNpc_ykM_c::afterJntAnm(int param_1) {
    if (param_1 == 1) {
        mDoMtx_stack_c::YrotM(-mStagger.getAngleZ(1));
        mDoMtx_stack_c::ZrotM(mStagger.getAngleX(1));
    } else if (param_1 == 4) {
        mDoMtx_stack_c::YrotM(-mStagger.getAngleZ(0));
        mDoMtx_stack_c::ZrotM(mStagger.getAngleX(0));
    }
}

void daNpc_ykM_c::setParam() {
    selectAction();
    srchActors();

    if (mType != TYPE_5 && mType != TYPE_6) {
        u32 uVar1 = (fopAc_AttnFlag_SPEAK_e | fopAc_AttnFlag_TALK_e);
        dComIfGp_getAttention()->getDistTable(71).mDistMax = mpHIO->m.attention_dist;
        dComIfGp_getAttention()->getDistTable(71).mDistMaxRelease = mpHIO->m.attention_dist;
        dComIfGp_getAttention()->getDistTable(70).mDistMax = mpHIO->m.conversation_dist;
        dComIfGp_getAttention()->getDistTable(70).mDistMaxRelease = mpHIO->m.conversation_dist;
        u32 uVar2 = 4;

        if (mType == TYPE_COOK || mType == TYPE_2) {
            if (mType == TYPE_COOK) {
                dComIfGp_getAttention()->getDistTable(71).mDistMax = 350.0f;
                dComIfGp_getAttention()->getDistTable(71).mDistMaxRelease = 350.0f;
                dComIfGp_getAttention()->getDistTable(70).mDistMax = 350.0f;
                dComIfGp_getAttention()->getDistTable(70).mDistMaxRelease = 350.0f;
            }

            uVar2 |= 0x80;
        } else if (mType == TYPE_4) {
            dComIfGp_getAttention()->getDistTable(71).mDistMax = 800.0f;
            dComIfGp_getAttention()->getDistTable(71).mDistMaxRelease = 900.0f;
            dComIfGp_getAttention()->getDistTable(70).mDistMax = 600.0f;
            dComIfGp_getAttention()->getDistTable(70).mDistMaxRelease = 700.0f;
        }

        dComIfGp_getAttention()->getDistTable(71).mAngleSelect = uVar2;
        dComIfGp_getAttention()->getDistTable(70).mAngleSelect = uVar2;
        attention_info.distances[fopAc_attn_LOCK_e] = 71;
        attention_info.distances[fopAc_attn_TALK_e] = attention_info.distances[fopAc_attn_LOCK_e];
        attention_info.distances[fopAc_attn_SPEAK_e] = 70;

        if (daPy_py_c::checkNowWolf()) {
            uVar1 |= fopAc_AttnFlag_UNK_0x800000;
        }

        attention_info.flags = uVar1;
    } else {
        attention_info.flags = 0;
    }

    scale.set(mpHIO->m.common.scale, mpHIO->m.common.scale, mpHIO->m.common.scale);
    mCcStts.SetWeight(mpHIO->m.common.weight);
    mCylH = mpHIO->m.common.height;
    mWallR = mpHIO->m.common.width;
    mAttnFovY = mpHIO->m.common.fov;
    mAcchCir.SetWallR(mWallR);
    mAcchCir.SetWallH(mpHIO->m.common.knee_length);

    fopAcM_OnStatus(this, fopAcM_STATUS_UNK_0x100);
    mRealShadowSize = mpHIO->m.common.real_shadow_size;

    if (field_0x157b != 0) {
        fopAcM_OffStatus(this, fopAcM_STATUS_UNK_0x100);
    }

    if (mType == TYPE_3) {
        fopAcM_OffStatus(this, fopAcM_STATUS_UNK_0x100);
        mRealShadowSize = 1800.0f;
    }

    mExpressionMorfFrame = mpHIO->m.common.expression_morf_frame;
    mMorfFrames = mpHIO->m.common.morf_frame;
    gravity = mpHIO->m.common.gravity;
    field_0x1574 = 0;
    field_0x1575 = 0;
    field_0xd8a.y = 0;
}

BOOL daNpc_ykM_c::checkChangeEvt() {
    if (!chkAction(&daNpc_ykM_c::talk)) {
        mPreItemNo = 0;

        if (dComIfGp_event_chkTalkXY()) {
            if (dComIfGp_evmng_ChkPresentEnd()) {
                mEvtNo = EVENT_NO_RESPONSE;
                evtChange();
            }

            return TRUE;
        }

        switch (mType) {
            case TYPE_0:
                break;

            case TYPE_COOK:
                if (!daNpcT_chkEvtBit(4) /* dSv_event_flag_c::TEST_004 - Snowpeak Ruins - Handed over secret ingredient */
                    && checkItemGet(fpcNm_ITEM_TASTE, 1)) {
                    mEvtNo = EVENT_GET_TASTE;
                    evtChange();
                    return TRUE;
                }

                if (!daNpcT_chkEvtBit(3) /* dSv_event_flag_c::TEST_003 - Snowpeak Ruins - Handed over tomato puree */ 
                    && checkItemGet(fpcNm_ITEM_TOMATO_PUREE, 1)) {
                    mEvtNo = EVENT_GET_TOMATOPUREE;
                    evtChange();
                    return TRUE;
                }
                break;

            case TYPE_2:
                break;

            case TYPE_3:
                mEvtNo = EVENT_HUG;
                evtChange();
                return TRUE;

            case TYPE_4:
                if (daPy_py_c::checkNowWolf()) {
                    mEvtNo = EVENT_FIND_WOLF;
                    evtChange();
                    return TRUE;
                }
                break;

            case TYPE_5:
            case TYPE_6:
                break;
        }
    }

    return FALSE;
}

void daNpc_ykM_c::setAfterTalkMotion() {
    int i_index = FACE_NONE;

    switch (mFaceMotionSeqMngr.getNo()) {
        case FACE_REGRET:
            i_index = FACE_H_REGRET;
            break;

        case FACE_LAUGH:
            i_index = FACE_H_LAUGH;
            break;

        case FACE_HAPPY:
            i_index = FACE_H_HAPPY;
            break;
    }

    mFaceMotionSeqMngr.setNo(i_index, -1.0f, FALSE, 0);
}

void daNpc_ykM_c::srchActors() {
    switch (mType) {
        case TYPE_0:
            break;
        case TYPE_COOK:
            if (mActorMngr[6].getActorP() == NULL) {
                mActorMngr[6].entry(getEvtAreaTagP(14, 0));
            }

            if (mActorMngr[3].getActorP() == NULL) {
                mActorMngr[3].entry(getTomatoPureeP());
            }

            if (mActorMngr[4].getActorP() == NULL) {
                mActorMngr[4].entry(getCheeseP());
            }
            break;

        case TYPE_2:
            if (mActorMngr[6].getActorP() == NULL) {
                mActorMngr[6].entry(getEvtAreaTagP(14, 0));
            }
            break;

        case TYPE_3:
            if (mActorMngr[5].getActorP() == NULL) {
                mActorMngr[5].entry(getNearestActorP(PROC_NPC_YKW));
            }
            break;

        case TYPE_4:
            if (mActorMngr[6].getActorP() == NULL) {
                mActorMngr[6].entry(getEvtAreaTagP(14, 1));
            }

            if (mActorMngr[7].getActorP() == NULL) {
                mActorMngr[7].entry(getEvtAreaTagP(14, 2));
            }

            if (mActorMngr[5].getActorP() == NULL) {
                mActorMngr[5].entry(getOtherYkwP(3));
            }
            break;

        case TYPE_5:
        case TYPE_6:
            break;
    }
}

BOOL daNpc_ykM_c::evtTalk() {
    if (chkAction(&daNpc_ykM_c::talk)) {
        (this->*mpAction)(NULL);
    } else {
        setAction(&daNpc_ykM_c::talk);
    }

    return TRUE;
}

BOOL daNpc_ykM_c::evtCutProc() {
    s32 staffId = dComIfGp_getEventManager().getMyStaffId("ykM", this, -1);
    if (staffId != -1) {
        mStaffId = staffId;
        int actIdx = dComIfGp_getEventManager().getMyActIdx(mStaffId, mCutNameList, 10, 0, 0);
        if ((this->*mCutList[actIdx])(mStaffId)) {
            dComIfGp_getEventManager().cutEnd(mStaffId);
        }

        return TRUE;
    }

    return FALSE;
}

void daNpc_ykM_c::action() {
    fopAc_ac_c* actor = NULL;

    for (int i = 0; i < 4; i++) {
        actor = hitChk(&field_0xf94[i], -1);
        if (actor != NULL) {
            break;
        }
    }

    if (actor == NULL) {
        actor = hitChk(&field_0xe58, -1);
    }

    if (actor != NULL && field_0x157b != 0) {
        mStagger.setParam(this, actor, mCurAngle.y);
        mMotionSeqMngr.setNo(MOTION_SB_DAMAGE, 0.0f, 0, 0);

        mDamageTimer = mCutType == 8 ? 20 : 8;
        mDamageTimerStart = 0;

        mJntAnm.lookNone(1);

        mSound.startCreatureSound(Z2SE_YM_SNOBO_SPIN, 0, -1);
        mSound.startCreatureVoice(Z2SE_YM_V_SNOBO_SPIN, -1);
    }

    if (mStagger.checkRebirth()) {
        mStagger.initialize();
        mMode = 1;
    }

    if (mpNextAction) {
        if (mpAction == mpNextAction) {
            (this->*mpAction)(NULL);
        } else {
            setAction(mpNextAction);
        }
    }

    if (field_0x1582 != 0 && !dComIfGp_event_runCheck()) {
        fopAcM_delete(this);
    }

    if (field_0x1540 > 0) {
        field_0x1540--;
    } else if (field_0x1540 < 0) {
        field_0x1540 = 0;
    }
}

void daNpc_ykM_c::beforeMove() {
    fopAcM_OffStatus(this, fopAcM_STATUS_UNK_0x8000000);

    if (checkHide()) {
        fopAcM_OnStatus(this, fopAcM_STATUS_UNK_0x8000000);
    }

    if (checkHide() || mNoDraw) {
        attention_info.flags = 0;
    }
}

void daNpc_ykM_c::afterMoved() {
    f32 var_f31;
    f32 var_f30;
    f32 var_f29;
    f32 var_f28;

    if (field_0x157b != 0 && field_0x157e == 0) {
        int polyAtt0 = dComIfG_Bgsp().GetPolyAtt0(mAcch.m_gnd);
        if (polyAtt0 == 13 && 3.0f < speedF) {
            cXyz sp70(current.pos);
            sp70.y -= 30.0f;
            field_0x1570 = dComIfGp_particle_setPolyColor(field_0x1570, dPa_RM(ID_ZI_S_LK_SB_A),
                                                          mAcch.m_gnd, &sp70, &tevStr, &mCurAngle,
                                                          NULL, 0, NULL, -1, NULL);
            JPABaseEmitter* emitter = dComIfGp_particle_getEmitter(field_0x1570);
            if (emitter != NULL) {
                cXyz sp7c;
                var_f31 = speedF / mpHIO->m.slide_speed;

                if (var_f31 > 1.0f) {
                    var_f31 = 1.0f;
                }

                emitter->setRate(var_f31 * 2.0f + 1.0f);
                emitter->setAwayFromCenterSpeed(var_f31 * 4.0f + 1.0f);
                sp7c.x = var_f31 * 0.3f + 0.7f;
                sp7c.y = sp7c.x;
                sp7c.z = sp7c.x;
                emitter->setGlobalParticleScale(sp7c);
                emitter->setGlobalAlpha(var_f31 * 205.0f + 50.0f);
            }
        }

        var_f29 = mGroundH - current.pos.y;
        var_f30 = current.pos.absXZ(old.pos) * cM_ssin(mGroundAngle);

        if (var_f30 < 0.0f) {
            var_f30 = 0.0f;
        }

        if (var_f29 <= 0.000001f && var_f29 >= -mAcchCir.GetWallH() - var_f30) {
            current.pos.y = mGroundH;
            mAcch.SetGroundHit();
            speed.y = 0.0f;
        }
    }
}

void daNpc_ykM_c::setAttnPos() {
    fopAc_ac_c* otherYkmP = NULL;
    cXyz sp104(80.0f, 30.0f, 0.0f);
    cXyz sp110(80.0f, 0.0f, 0.0f);

    if (field_0x157b != 0) {
        if (mStagger.checkStagger() && (mMotionSeqMngr.getNo() != MOTION_SB_DAMAGE || mMotionSeqMngr.checkEndSequence())) {
            mStagger.setRebirth();
        }
    } else {
        mStagger.calc(FALSE);
    }

    f32 fVar1 = cM_s2rad((s16)(mCurAngle.y - field_0xd7e.y));
    mJntAnm.setParam(this, mpMorf[0]->getModel(), &sp104,
        getBackboneJointNo(), getNeckJointNo(), getHeadJointNo(),
        field_0x1575 != 0 ? 0.0f : mpHIO->m.common.body_angleX_min,
        field_0x1575 != 0 ? 0.0f : mpHIO->m.common.body_angleX_max,
        field_0x1575 != 0 ? 0.0f : mpHIO->m.common.body_angleY_min,
        field_0x1575 != 0 ? 0.0f : mpHIO->m.common.body_angleY_max,
        field_0x1574 != 0 ? 0.0f : mpHIO->m.common.head_angleX_min,
        field_0x1574 != 0 ? 0.0f : mpHIO->m.common.head_angleX_max,
        field_0x1574 != 0 ? 0.0f : mpHIO->m.common.head_angleY_min,
        field_0x1574 != 0 ? 0.0f : mpHIO->m.common.head_angleY_max,
        mpHIO->m.common.neck_rotation_ratio, fVar1, &sp110);

    if (mJntAnm.getMode() == daNpcT_JntAnm_c::LOOK_MODE_8) {
        mJntAnm.calcJntRad(0.1f, 1.0f, fVar1);
    } else {
        mJntAnm.calcJntRad(0.2f, 1.0f, fVar1);
    }

    J3DModel* model = mpMorf[0]->getModel();
    J3DModelData* modelData = model->getModelData();
    cXyz sp11c(current.pos);
    sp11c.y += field_0x1568;
    mDoMtx_stack_c::transS(sp11c);
    mDoMtx_stack_c::ZXYrotM(mCurAngle);
    mDoMtx_stack_c::scaleM(scale);
    model->setBaseTRMtx(mDoMtx_stack_c::get());
    model->setUserArea((uintptr_t)this);

    mpMorf[0]->onMorfNone();
    if (cM3d_IsZero(field_0xdfc) != false) {
        mpMorf[0]->offMorfNone();
    }

    if ((mAnmFlags & 0x400) != 0) {
        mBckAnm.getBckAnm()->setFrame(mBckAnm.getFrame());
    }

    mpMorf[0]->modelCalc();

    if (mFishModelMorf != NULL) {
        cXyz sp128;
        mFishModelMorf->play(0, 0);
        mDoMtx_stack_c::copy(mpMorf[0]->getModel()->getAnmMtx(JNT_FINGERR));
        mDoMtx_stack_c::multVecZero(&sp128);

        Mtx mtx;
        cMtx_copy(mDoMtx_stack_c::get(), mtx);
        mFishModelMorf->getModel()->setBaseTRMtx(mtx);
        mFishModelMorf->modelCalc();

        if (field_0x1579 != 0) {
            mDoAud_seStartLevel(Z2SE_YM_FISH, &sp128, 0, 0);
        }
    }

    if (mLeafModelMorf != NULL) {
        mLeafModelMorf->play(0, 0);

        otherYkmP = getOtherYkmP(5);
        if (otherYkmP != NULL) {
            mDoMtx_stack_c::transS(otherYkmP->current.pos);
            mDoMtx_stack_c::ZXYrotM(otherYkmP->shape_angle);
            mDoMtx_stack_c::scaleM(otherYkmP->scale);
            mLeafModelMorf->getModel()->setBaseTRMtx(mDoMtx_stack_c::get());
            mLeafModelMorf->modelCalc();
        }
    }

    mDoMtx_stack_c::copy(mpMorf[0]->getModel()->getAnmMtx(getHeadJointNo()));
    mDoMtx_stack_c::multVec(&sp104, &eyePos);

    mJntAnm.setEyeAngleX(eyePos, 1.0f, 0);
    mJntAnm.setEyeAngleY(eyePos, mCurAngle.y + field_0xd8a.y, TRUE, 1.0f, 0);

    sp104.set(0.0f, 0.0f, 50.0f);
    sp104.y = mpHIO->m.common.attention_offset + field_0x1568;

    if (field_0x1581 != 0) {
        sp104.set(55.0f, 440.0f, 55.0f);
    } else if (mType == TYPE_COOK) {
        sp104.set(0.0f, 480.0f, 50.0f);
    }

    mDoMtx_stack_c::YrotS(mCurAngle.y);
    mDoMtx_stack_c::multVec(&sp104, &sp104);
    attention_info.position = current.pos + sp104;
}

void daNpc_ykM_c::setCollision() {
    cXyz sp28;

    if (!mHide && field_0x1580 == 0) {
        u32 coSPrm = 121;
        u32 tgType = 0xD8FBFDFF;
        u32 tgSPrm = 31;
        int atType = AT_TYPE_IRON_BALL;
        u32 atSPrm = 0;

        if (dComIfGp_event_runCheck() || field_0x1581 != 0) {
            tgType = 0;
            tgSPrm = 0;
        } else if (mTwilight) {
            tgType = 0;
            tgSPrm = 0;
        } else {
            if (mStagger.checkStagger()) {
                tgType = 0;
                tgSPrm = 0;
            }

            if (field_0x157b != 0) {
                atSPrm = 19;
            }
        }

        int jointNo[4] = {JNT_BB2, JNT_HANDL, JNT_HANDR, JNT_TAIL};
        f32 fVar1[4] = {140.0f, 60.0f, 60.0f, 140.0f};
        for (int i = 0; i < 4; i++) {
            field_0xf94[i].SetCoSPrm(coSPrm);
            field_0xf94[i].SetTgType(tgType);
            field_0xf94[i].SetTgSPrm(tgSPrm);
            field_0xf94[i].SetAtType(atType);
            field_0xf94[i].SetAtSPrm(atSPrm);
            field_0xf94[i].OnTgNoHitMark();

            mDoMtx_stack_c::copy(mpMorf[0]->getModel()->getAnmMtx(jointNo[i]));

            if (jointNo[i] == 29) {
                sp28.set(170.0f, 0.0f, 0.0f);
                mDoMtx_stack_c::multVec(&sp28, &sp28);
            } else {
                mDoMtx_stack_c::multVecZero(&sp28);
            }

            field_0xf94[i].SetC(sp28);
            field_0xf94[i].SetR(fVar1[i]);
            dComIfG_Ccsp()->Set(&field_0xf94[i]);
        }

        field_0xe58.SetCoSPrm(coSPrm);
        field_0xe58.SetTgType(tgType);
        field_0xe58.SetTgSPrm(tgSPrm);
        field_0xe58.SetAtType(atType);
        field_0xe58.SetAtSPrm(atSPrm);
        field_0xe58.OnTgNoHitMark();
        field_0xe58.SetH(mCylH);
        field_0xe58.SetR(mWallR);

        mDoMtx_stack_c::copy(mpMorf[0]->getModel()->getAnmMtx(JNT_CENTER));
        mDoMtx_stack_c::multVecZero(&sp28);
        sp28.y = current.pos.y;
        field_0xe58.SetC(sp28);
        dComIfG_Ccsp()->Set(&field_0xe58);
    }

    for (int i = 0; i < 4; i++) {
        field_0xf94[i].ClrCoHit();
    }
    field_0xe58.ClrCoHit();

    for (int i = 0; i < 4; i++) {
        field_0xf94[i].ClrTgHit();
    }
    field_0xe58.ClrTgHit();

    for (int i = 0; i < 4; i++) {
        field_0xf94[i].ClrAtHit();
    }
    field_0xe58.ClrAtHit();
}

int daNpc_ykM_c::drawDbgInfo() {
#if DEBUG
    if (mpHIO->m.common.debug_info_ON != 0) {
        f32 fVar1 = dComIfGp_getAttention()->getDistTable(attention_info.distances[6]).mDistMax;
        f32 fVar2 = dComIfGp_getAttention()->getDistTable(attention_info.distances[1]).mDistMax;
        dDbVw_drawCircleOpa(attention_info.position, fVar1, (GXColor){0x0, 0xc8, 0x0, 0xff}, 1, 12);
        dDbVw_drawCircleOpa(attention_info.position, fVar2, (GXColor){0xc8, 0x0, 0x0, 0xff}, 1, 12);
        dDbVw_drawSphereXlu(eyePos, 18.0f, (GXColor){0x80, 0x80, 0x80, 0xa0}, 1);
        dDbVw_drawSphereXlu(attention_info.position, 9.0f, (GXColor){0x80, 0x80, 0x80, 0xa0}, 1);
        if (mPath.getPathInfo() != 0) {
            mPath.drawDbgInfo(attention_info.position.y, 3);
        }
    }
#endif
    return 0;
}

void daNpc_ykM_c::drawOtherMdl() {
    J3DModel* model = mpMorf[0]->getModel();

    if (mFishModelMorf != NULL) {
        g_env_light.setLightTevColorType_MAJI(mFishModelMorf->getModel(), &tevStr);

        if (field_0x1579 == 0) {
            mFishModelMorf->getModel()->getModelData()->hide();
        } else {
            mFishModelMorf->getModel()->getModelData()->show();
        }

        mFishModelMorf->entryDL();
        dComIfGd_addRealShadow(mShadowKey, mFishModelMorf->getModel());
    }

    if (mLeafModelMorf != NULL) {
        g_env_light.setLightTevColorType_MAJI(mLeafModelMorf->getModel(), &tevStr);

        if (field_0x1576 != 0) {
            mLeafModelMorf->getModel()->getModelData()->hide();
        } else {
            mLeafModelMorf->getModel()->getModelData()->show();
        }

        mLeafModelMorf->entryDL();
    }

    static int const jointNo[3] = {
        JNT_FINGERR, JNT_FOOTR, JNT_FINGERR,
    };
    for (int i = 0; i < 3; i++) {
        if (field_0xe4c[i] != NULL && ((i == 0 && field_0x1577 != 0) || (i == 2 && field_0x1578 != 0) || (i == 1 && field_0x1576 != 0))) {
            g_env_light.setLightTevColorType_MAJI(field_0xe4c[i], &tevStr);
            mDoMtx_stack_c::copy(model->getAnmMtx(jointNo[i]));

            Mtx mtx;
            cMtx_copy(mDoMtx_stack_c::get(), mtx);
            field_0xe4c[i]->setBaseTRMtx(mtx);
            mDoExt_modelUpdateDL(field_0xe4c[i]);
            dComIfGd_addRealShadow(mShadowKey, field_0xe4c[i]);
        }
    }
}

bool daNpc_ykM_c::setFishAnm(int i_index, int i_attr, f32 i_morf) {
    static struct {
        int resIdx;
        int arcIdx;
    } fishAnmData[51] = {
        {-1, 0}, {-1, 0},
        {-1, 0}, {-1, 0},
        {-1, 0}, {-1, 0},
        {-1, 0}, {-1, 0},
        {-1, 0}, {-1, 0},
        {-1, 0}, {-1, 0},
        {-1, 0}, {-1, 0},
        {-1, 0}, {-1, 0},
        {-1, 0}, {-1, 0},
        {-1, 0}, {-1, 0},
        {BCK_FISH_WAIT_A, YKM3}, {BCK_FISH_ATTACK, YKM3},
        {BCK_FISH_WAIT_A, YKM3}, {BCK_FISH_JUMP, YKM3},
        {BCK_FISH_WAIT_A, YKM3}, {BCK_FISH_WAIT_A, YKM3},
        {BCK_FISH_WAIT_A, YKM3}, {BCK_FISH_WAIT_A, YKM3},
        {BCK_FISH_WAIT_A, YKM3}, {BCK_FISH_WAIT_A, YKM3},
        {BCK_FISH_WAIT_A, YKM3}, {BCK_FISH_WAIT_A, YKM3},
        {BCK_FISH_SB_WAIT, YKM3}, {-1, 0},
        {-1, 0}, {-1, 0},
        {-1, 0}, {-1, 0},
        {-1, 0}, {-1, 0},
        {-1, 0}, {-1, 0},
        {-1, 0}, {-1, 0},
        {-1, 0}, {-1, 0},
        {-1, 0}, {-1, 0},
        {-1, 0}, {-1, 0},
        {-1, 0},
    };

    J3DAnmTransform* anmTransform = NULL;

    int index = i_index;
    if (mFishModelMorf != NULL) {
        if (fishAnmData[index].resIdx > 0) {
            anmTransform = getTrnsfrmKeyAnmP(l_resNameList[fishAnmData[index].arcIdx], fishAnmData[index].resIdx);
        }

        if (anmTransform != NULL) {
            mFishModelMorf->setAnm(anmTransform, i_attr, i_morf, 1.0f, 0.0f, -1.0f);
        }
    }

    return true;
}

bool daNpc_ykM_c::setLeafAnm(int i_index, f32 i_morf) {
    static struct {
        int resIdx;
        int attr;
        int arcIdx;
    } leafAnmData[3] = {
        {BCK_LEAF_WAIT_A, J3DFrameCtrl::EMode_LOOP, YKM3},
        {BCK_LEAF_FALL, J3DFrameCtrl::EMode_NONE, YKM3},
        {BCK_LEAF_WAIT_B, J3DFrameCtrl::EMode_LOOP, YKM3},
    };

    J3DAnmTransform* anmTransform = NULL;

    int index = i_index;
    if (mLeafModelMorf != NULL) {
        if (leafAnmData[index].resIdx > 0) {
            anmTransform = getTrnsfrmKeyAnmP(l_resNameList[leafAnmData[index].arcIdx], leafAnmData[index].resIdx);
        }

        if (anmTransform != NULL) {
            mLeafModelMorf->setAnm(anmTransform, leafAnmData[index].attr, i_morf, 1.0f, 0.0f, -1.0f);
        }
    }

    return true;
}

bool daNpc_ykM_c::afterSetMotionAnm(int i_index, int i_attr, f32 i_morf, int param_3) {
    f32 morf = mCreating == true ? 0.0f : i_morf;

    switch (i_index) {
        case 0x23:
            mpMorf[0]->setStartFrame(15.0f);
            break;
    }

    return setFishAnm(i_index, i_attr, morf);
}

int daNpc_ykM_c::selectAction() {
    mpNextAction = NULL;

#if DEBUG
    if (mpHIO->m.common.debug_mode_ON != NULL) {
        mpNextAction = &daNpc_ykM_c::cook;

        return 1;
    }
#endif

    switch (mType) {
    case TYPE_COOK:
        mpNextAction = &daNpc_ykM_c::cook;
        break;

    case TYPE_2:
        if (field_0x157b != 0) {
            mpNextAction = &daNpc_ykM_c::race;
            mAcch.SetWallNone();
        } else {
            mpNextAction = &daNpc_ykM_c::wait;
            mAcch.ClrWallNone();
        }
        break;

    case TYPE_4:
        if (field_0x157b != 0) {
            mpNextAction = &daNpc_ykM_c::race;
            mAcch.SetWallNone();
        } else {
            mpNextAction = &daNpc_ykM_c::wait;
            mAcch.ClrWallNone();
        }
        break;

    default:
        mpNextAction = &daNpc_ykM_c::wait;
        break;
    }

    return 1;
}

BOOL daNpc_ykM_c::chkAction(ActionFn action) {
    return mpAction == action;
}

int daNpc_ykM_c::setAction(ActionFn action) {
    mMode = 3;

    if (mpAction) {
        (this->*mpAction)(NULL);
    }

    mMode = 0;
    mpAction = action;

    if (mpAction) {
        (this->*mpAction)(NULL);
    }

    return 1;
}

BOOL daNpc_ykM_c::chkContinueAttnPlayer() {
    if (mType == TYPE_COOK && daNpcT_chkEvtBit(0xB) /* dSv_event_flag_c::F_0007 - Snowpeak Ruins - Spoke to Yeta while holding cheese */) {
        if (daNpcT_chkEvtBit(4) /* dSv_event_flag_c::TEST_004 - Snowpeak Ruins - Handed over secret ingredient */
            && daNpcT_chkEvtBit(8) /* dSv_event_flag_c::F_0004 - Snowpeak Ruins - Handed over secret ingredient and left room */) {
            return TRUE;
        }

        if (daNpcT_chkEvtBit(3) /* dSv_event_flag_c::TEST_003 - Snowpeak Ruins - Handed over tomato puree */
            && daNpcT_chkEvtBit(7) /* dSv_event_flag_c::F_0003 - Snowpeak Ruins - Handed over tomato puree and left room */) {
            return TRUE;
        }
    }

    return FALSE;
}

BOOL daNpc_ykM_c::chkTouchPlayer() {
    fopAc_ac_c* actor = NULL;
    if (20.0f < speedF) {
        for (int i = 0; i < 4; i++) {
            if (field_0xf94[i].ChkCoHit()) {
                actor = field_0xf94[i].GetCoHitAc();

                if (actor != NULL && fopAcM_GetName(actor) == PROC_ALINK) {
                    return TRUE;
                }
            }
        }

        if (field_0xe58.ChkCoHit()) {
            actor = field_0xe58.GetCoHitAc();

            if (actor != NULL && fopAcM_GetName(actor) == PROC_ALINK) {
                return TRUE;
            }
        }
    }

    return FALSE;
}

int daNpc_ykM_c::cutSlideDown(int i_cutIndex) {
    fopAc_ac_c* actor_p = NULL;
    cXyz work;
    csXyz angle;
    int rv = 0;
    int* piVar1 = NULL;
    int prm = -1;
    int timer = 0;

    piVar1 = dComIfGp_evmng_getMyIntegerP(i_cutIndex, "prm");
    if (piVar1 != NULL) {
        prm = *piVar1;
    }

    piVar1 = dComIfGp_evmng_getMyIntegerP(i_cutIndex, "timer");
    if (piVar1 != NULL) {
        timer = *piVar1;
    }

    if (dComIfGp_getEventManager().getIsAddvance(i_cutIndex) != 0) {
        switch (prm) {
            case 0:
                mEventTimer = timer;
                break;

            case 1:
                mFaceMotionSeqMngr.setNo(FACE_NONE, -1.0f, FALSE, 0);
                mMotionSeqMngr.setNo(MOTION_FISH_WAIT_A_2, -1.0f, FALSE, 0);
                work.set(400.0f, 500.0f, -200.0f);
                angle.y = mCurAngle.y;
                mDoMtx_stack_c::YrotS(angle.y);
                mDoMtx_stack_c::multVec(&work, &work);
                work += current.pos;
                mGndChk.SetPos(&work);
                work.y = dComIfG_Bgsp().GroundCross(&mGndChk);

                JUT_ASSERT(3443, -G_CM3D_F_INF != work.y);

                daPy_getPlayerActorClass()->setPlayerPosAndAngle(&work, angle.y, 0);
                work.set(400.0f, 500.0f, 0.0f);
                angle.y = mCurAngle.y;
                mDoMtx_stack_c::YrotS(angle.y);
                mDoMtx_stack_c::multVec(&work, &work);
                work += current.pos;
                mGndChk.SetPos(&work);
                work.y = dComIfG_Bgsp().GroundCross(&mGndChk);

                JUT_ASSERT(3454, -G_CM3D_F_INF != work.y);

                dComIfGp_evmng_setGoal(&work);
                break;

            case 2:
                initTalk(mFlowNodeNo, NULL);
                mEventTimer = timer;
                break;

            case 3:
                mMotionSeqMngr.setNo(MOTION_FISH_ATTACK, 0.0f, TRUE, 0);
                mJntAnm.lookNone(1);
                break;

            case 4:
                setLeafAnm(ANM_LEAF_FALL, 0.0f);
                break;

            case 5:
                mMotionSeqMngr.setNo(MOTION_FISH_JUMP, 0.0f, TRUE, 0);
                actor_p = getOtherYkmP(5);

                JUT_ASSERT(3475, NULL != actor_p);

                setPos(actor_p->current.pos);
                setAngle(actor_p->shape_angle.y);
                field_0x1568 = 0.0f;
                setLeafAnm(ANM_LEAF_WAIT_B, 0.0f);
                mSound.startCreatureVoice(Z2SE_YKM_V_JUMP_TO_LEAF, -1);
                break;

            case 6:
                field_0x157b = 1;
                field_0x1582 = 1;
                break;
        }
    }

    int unusedIntArr1[2] = {-1, -1};
    switch (prm) {
        case 0:
            if (cLib_calcTimer(&mEventTimer) == 0) {
                rv = 1;
            }
            break;

        case 1:
            rv = 1;
            break;

        case 2:
            mJntAnm.lookPlayer(0);

            if (cLib_calcTimer(&mEventTimer) == 0 && talkProc(NULL, FALSE, NULL, FALSE) && mFlow.checkEndFlow()) {
                rv = 1;
            }
            break;

        case 3:
            mJntAnm.lookNone(0);

            if (mMotionSeqMngr.getStepNo() == 0) {
                if (mpMorf[0]->checkFrame(65.0f)) {
                    rv = 1;
                } else if (mpMorf[0]->checkFrame(50.0f)) {
                    mSound.startCreatureVoice(Z2SE_YKM_V_ATTACK_TREE, -1);
                } else if (mpMorf[0]->checkFrame(62.0f)) {
                    cXyz i_sePos(-14483.0f, 2023.0f, -9768.0f);
                    mDoAud_seStart(Z2SE_YM_ATTACK_TREE, &i_sePos, 0, 0);
                    dComIfGp_getVibration().StartShock(5, 15, cXyz(0.0f, 1.0f, 0.0f));
                }
            }
            break;

        case 4:
            mJntAnm.lookNone(0);

            if (mLeafModelMorf->isStop()) {
                actor_p = getOtherYkmP(5);

                JUT_ASSERT(3565, NULL != actor_p);

                angle.y = fopAcM_searchActorAngleY(daPy_getPlayerActorClass(), actor_p);
                work = daPy_getPlayerActorClass()->current.pos;
                daPy_getPlayerActorClass()->setPlayerPosAndAngle(&work, angle.y, 0);
                rv = 1;
            } else {
                actor_p = getOtherYkmP(5);

                if (actor_p != NULL && mLeafModelMorf->checkFrame(22.0f)) {
                    mDoAud_seStart(Z2SE_OBJ_LEAF_FALL_DOWN, &actor_p->current.pos, 0, 0);
                }
            }
            break;

        case 5:
            if (mMotionSeqMngr.checkEndSequence()) {
                rv = 1;
            } else if (mpMorf[0]->checkFrame(27.0f)) {
                mSound.startCreatureSound(Z2SE_YM_RIDE_LEAF, 0, -1);
            }
            break;

        case 6:
            action();

            if (17 < (int)mPath.getIdx()) {
                rv = 1;
            }
            break;
    }

    return rv;
}

int daNpc_ykM_c::cutMeetingAgain(int i_cutIndex) {
    cXyz work;
    int rv = 0;
    int* piVar1 = NULL;
    int prm = -1;
    int timer = 0;
    int unusedInt1 = 0;
    
    piVar1 = dComIfGp_evmng_getMyIntegerP(i_cutIndex, "prm");
    if (piVar1 != NULL) {
        prm = *piVar1;
    }

    piVar1 = dComIfGp_evmng_getMyIntegerP(i_cutIndex, "timer");
    if (piVar1 != NULL) {
        timer = *piVar1;
    }

    piVar1 = dComIfGp_evmng_getMyIntegerP(i_cutIndex, "msgNo");
    if (piVar1 != NULL) {
        unusedInt1 = *piVar1;
    }

    if (dComIfGp_getEventManager().getIsAddvance(i_cutIndex)) {
        switch (prm) {
            case 0:
                daNpcT_offTmpBit(0xB); // General use - General use temporary flag (flow control) A
                daNpcT_offTmpBit(0xC); // General use - General use temporary flag (flow control) B
                daNpcT_offTmpBit(0xD); // General use - General use temporary flag (flow control) C
                initTalk(mFlowNodeNo, NULL);
                break;

            case 1:
                mEventTimer = timer;
                field_0x156c = fopAcM_searchActorAngleY(daPy_getPlayerActorClass(), this);
                work.set(0.0f, 0.0f, 1000.0f);
                mDoMtx_stack_c::YrotS(field_0x156c);
                mDoMtx_stack_c::multVec(&work, &work);
                field_0x1520 = work + daPy_getPlayerActorClass()->current.pos;
                dComIfGp_evmng_setGoal(&field_0x1520);

                work.set(0.0f, 0.0f, 80.0f);
                mDoMtx_stack_c::YrotS(field_0x156c);
                mDoMtx_stack_c::multVec(&work, &work);
                work += daPy_getPlayerActorClass()->current.pos;
                daPy_getPlayerActorClass()->setPlayerPosAndAngle(&work, field_0x156c, 0);
                break;

            case 2:
                initTalk(mFlowNodeNo, NULL);
                daPy_getPlayerActorClass()->setPlayerPosAndAngle(&field_0x1520, field_0x156c, 0);
                break;

            case 3:
                break;
        }
    }

    int unusedIntArr1[2] = {-1, -1};
    switch (prm) {
        case 0:
            mJntAnm.lookPlayer(0);
            if (talkProc(NULL, FALSE, NULL, FALSE) && mFlow.checkEndFlow()) {
                rv = 1;
            }
            break;

        case 1:
            if (cLib_calcTimer(&mEventTimer) == 0) {
                rv = 1;
            }
            break;

        case 2:
            mJntAnm.lookPlayer(0);
            if (talkProc(NULL, FALSE, NULL, FALSE) && mFlow.checkEndFlow()) {
                rv = 1;
            }
            break;

        case 3:
            rv = 1;
            break;
    }

    return rv;
}

int daNpc_ykM_c::cutGetTomatoPuree(int i_cutIndex) {
    fopAc_ac_c* actor_p = NULL;
    cXyz work;
    csXyz angle;
    int rv = 0;
    int* piVar1 = NULL;
    int prm = -1;
    int timer = 0;
    int msgNo = 0;

    field_0x1508.set(0.0f, 200.0f, 450.0f);
    mDoMtx_stack_c::YrotS(home.angle.y);
    mDoMtx_stack_c::multVec(&field_0x1508, &field_0x1508);
    field_0x1508 += current.pos;

    piVar1 = dComIfGp_evmng_getMyIntegerP(i_cutIndex, "prm");
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
                mJntAnm.lookNone(0);
                daNpcT_offTmpBit(0xB); // General use - General use temporary flag (flow control) A
                daNpcT_offTmpBit(0xC); // General use - General use temporary flag (flow control) B
                daNpcT_offTmpBit(0xD); // General use - General use temporary flag (flow control) C
                initTalk(mFlowNodeNo, NULL);
                break;

            case 1:
                mEventTimer = timer;
                break;

            case 2:
                initTalk(mFlowNodeNo, NULL);
                break;

            case 3:
                mPlayerAngle = fopAcM_searchPlayerAngleY(this);
                break;

            case 4:
                mFaceMotionSeqMngr.setNo(FACE_NONE, -1.0f, FALSE, 0);
                mMotionSeqMngr.setNo(MOTION_PUSH, 0.0f, FALSE, 0);
                mEventTimer = timer;
                break;

            case 5:
                mFaceMotionSeqMngr.setNo(FACE_NONE, -1.0f, FALSE, 0);
                mMotionSeqMngr.setNo(MOTION_PICKUP, -1.0f, FALSE, 0);
                break;

            case 6:
                initTalk(mFlowNodeNo, NULL);
                break;

            case 7:
                mFaceMotionSeqMngr.setNo(FACE_NONE, -1.0f, FALSE, 0);
                mMotionSeqMngr.setNo(MOTION_POUR, -1.0f, FALSE, 0);
                mEventTimer = timer;
                setAngle(home.angle.y);
                break;

            case 8:
                initTalk(mFlowNodeNo, NULL);
                break;
        }
    }

    int iVar2[2] = {-1, -1};
    f32 fVar1;
    int iVar1;
    switch (prm) {
        case 0:
            if (talkProc(NULL, FALSE, NULL, FALSE) && mFlow.checkEndFlow()) {
                rv = 1;
            }
            break;

        case 1:
            if (cLib_calcTimer(&mEventTimer) == 0) {
                rv = 1;
            }
            break;

        case 2:
            iVar2[0] = msgNo;
            if (talkProc(iVar2, FALSE, NULL, FALSE)) {
                if (msgNo == 0) {
                    if (mFlow.checkEndFlow()) {
                        rv = 1;
                    }
                } else {
                    rv = 1;
                }
            }
            break;

        case 3:
            mJntAnm.lookPlayer(0);

            if (mPlayerAngle != mCurAngle.y) {
                step(mPlayerAngle, -1, -1, mpHIO->m.step_speed, 0);
            }

            shape_angle.y = mPlayerAngle;

            if (talkProc(NULL, FALSE, NULL, FALSE) && mFlow.checkEndFlow() && mPlayerAngle == mCurAngle.y) {
                rv = 1;
            }
            break;

        case 4:
            mJntAnm.lookPlayer(0);

            if (mMotionSeqMngr.getNo() != MOTION_PUSH) break;

            if (mEventTimer != 0 || dComIfGp_getEventManager().getIsAddvance(i_cutIndex)) {
                if (cLib_calcTimer(&mEventTimer) == 0) {
                    actor_p = (fopAc_ac_c*)mActorMngr[3].getActorP();
                    if (actor_p != NULL) {
                        ((daObj_Gadget_c*)actor_p)->setCoNone();
                        ((daObj_Gadget_c*)actor_p)->offHide();
                        actor_p->current.angle.y = shape_angle.y + 0x8000;
                        ((daObj_Gadget_c*)actor_p)->popup(mpHIO->m.jump_speed,
                            mpHIO->m.push_out_angle,
                            &daPy_getPlayerActorClass()->attention_info.position);
                    }
                }
            }

            if (mMotionSeqMngr.getStepNo() == 0) {
                if (mpMorf[0]->checkFrame(mpHIO->m.skip_frame)) {
                    mSound.startCreatureVoice(Z2SE_YKM_V_GETFOOD, -1);
                    fVar1 = mpHIO->m.fly_speed;
                    s16 iVar2 = cM_deg2s(mpHIO->m.fly_angle);
                    daPy_getPlayerActorClass()->setThrowDamage(shape_angle.y, fVar1 * cM_scos(iVar2), fVar1 * cM_ssin(iVar2), 0, 1, 0);
                }
            } else {
                rv = 1;
            }
            break;

        case 5:
            if (mMotionSeqMngr.getNo() == MOTION_PICKUP) {
                if (mMotionSeqMngr.getStepNo() == 0) {
                    if (mpMorf[0]->checkFrame(19.0f)) {
                        field_0x1577 = 1;
                        actor_p = (daObj_Gadget_c*)mActorMngr[3].getActorP();
                        if (actor_p != NULL) {
                            ((daObj_Gadget_c*)actor_p)->onHide();
                        }
                    }
                } else {
                    rv = 1;
                }
            }
            break;

        case 7:
            if (mMotionSeqMngr.getNo() == MOTION_POUR) {
                if (mMotionSeqMngr.getStepNo() == 0) {
                    if (mpMorf[0]->checkFrame(18.0f)) {
                        if (field_0x1577 != 0) {
                            mDoAud_seStart(Z2SE_CM_BODYFALL_WATER_S, NULL, 0, 0);
                        }

                        field_0x1577 = 0;
                    }
                } else if (cLib_calcTimer(&mEventTimer) == 0) {
                    rv = 1;
                }
            }
            break;

        case 6:
            if (talkProc(NULL, FALSE, NULL, FALSE) && mFlow.checkEndFlow()) {
                rv = 1;
            }
            break;

        case 8:
            mJntAnm.lookPlayer(0);

            if (talkProc(NULL, FALSE, NULL, FALSE) && mFlow.checkEndFlow()) {
                rv = 1;
            }
            break;
    }

    field_0x1580 = 1;
    if (prm == 8) {
        field_0x1580 = 0;
    }

    if (prm == 5 || prm == 6) {
        if (field_0x1577 != 0) {
            field_0x1575 = 1;
            mDoMtx_stack_c::copy(mpMorf[0]->getModel()->getAnmMtx(JNT_FINGERR));
            mDoMtx_stack_c::multVecZero(&field_0xd6c);
            mJntAnm.lookPos(&field_0xd6c, 0);
        } else {
            actor_p = mActorMngr[3].getActorP();
            if (actor_p == NULL) {
                mJntAnm.lookNone(0);
                return rv;
            }

            field_0xd6c = actor_p->current.pos;
        }
    } else if (prm == 7) {
        mJntAnm.lookPos(&field_0x1508, 0);
        field_0x1575 = 1;
    }

    return rv;
}

int daNpc_ykM_c::cutGetTaste(int i_cutIndex) {
    fopAc_ac_c* actor_p = NULL;
    cXyz work;
    csXyz angle;
    int rv = 0;
    int* piVar1 = NULL;
    int prm = -1;
    int timer = 0;
    int msgNo = 0;

    field_0x1508.set(0.0f, 200.0f, 450.0f);
    mDoMtx_stack_c::YrotS(home.angle.y);
    mDoMtx_stack_c::multVec(&field_0x1508, &field_0x1508);
    field_0x1508 += current.pos;

    piVar1 = dComIfGp_evmng_getMyIntegerP(i_cutIndex, "prm");
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
            case 2:
                mJntAnm.lookNone(0);
                daNpcT_offTmpBit(0xB); // General use - General use temporary flag (flow control) A
                daNpcT_offTmpBit(0xC); // General use - General use temporary flag (flow control) B
                daNpcT_offTmpBit(0xD); // General use - General use temporary flag (flow control) C
                initTalk(mFlowNodeNo, NULL);
                break;

            case 3:
                mPlayerAngle = fopAcM_searchPlayerAngleY(this);
                break;

            case 4:
                mFaceMotionSeqMngr.setNo(FACE_NONE, -1.0f, FALSE, 0);
                mMotionSeqMngr.setNo(MOTION_PUSH, 0.0f, FALSE, 0);
                mEventTimer = timer;
                break;

            case 5:
                mFaceMotionSeqMngr.setNo(FACE_NONE, -1.0f, FALSE, 0);
                mMotionSeqMngr.setNo(MOTION_PICKUP, -1.0f, FALSE, 0);
                break;

            case 6:
                initTalk(mFlowNodeNo, NULL);
                break;

            case 7:
                mFaceMotionSeqMngr.setNo(FACE_NONE, -1.0f, FALSE, 0);
                mMotionSeqMngr.setNo(MOTION_POUR, -1.0f, FALSE, 0);
                mEventTimer = timer;
                setAngle(home.angle.y);
                break;

            case 8:
                initTalk(mFlowNodeNo, NULL);
                break;
        }
    }
    int iVar2[2] = {-1, -1};
    f32 fVar1;
    int iVar1;
    switch (prm) {
        case 2:
            iVar2[0] = msgNo;
            if (talkProc(iVar2, FALSE, NULL, FALSE)) {
                if (msgNo == 0) {
                    if (mFlow.checkEndFlow()) {
                        rv = 1;
                    }
                } else {
                    rv = 1;
                }
            }
            break;

        case 3:
            mJntAnm.lookPlayer(0);

            if (mPlayerAngle != mCurAngle.y) {
                step(mPlayerAngle, -1, -1, mpHIO->m.step_speed, 0);
            }

            shape_angle.y = mPlayerAngle;

            if (talkProc(NULL, FALSE, NULL, FALSE) && mFlow.checkEndFlow() && mPlayerAngle == mCurAngle.y) {
                rv = 1;
            }
            break;

        case 4:
            mJntAnm.lookPlayer(0);

            if (mMotionSeqMngr.getNo() != MOTION_PUSH) break;

            if (mEventTimer != 0 || dComIfGp_getEventManager().getIsAddvance(i_cutIndex)) {
                if (cLib_calcTimer(&mEventTimer) == 0) {
                    actor_p = (daObj_Gadget_c*)mActorMngr[4].getActorP();
                    if (actor_p != NULL) {
                        ((daObj_Gadget_c*)actor_p)->setCoNone();
                        ((daObj_Gadget_c*)actor_p)->offHide();
                        actor_p->current.angle.y = shape_angle.y + 0x8000;
                        ((daObj_Gadget_c*)actor_p)->popup(mpHIO->m.jump_speed, mpHIO->m.push_out_angle, &daPy_getPlayerActorClass()->attention_info.position);
                    }
                }
            }

            if (mMotionSeqMngr.getStepNo() == 0) {
                if (mpMorf[0]->checkFrame(mpHIO->m.skip_frame)) {
                    mSound.startCreatureVoice(Z2SE_YKM_V_GETFOOD, -1);
                    fVar1 = mpHIO->m.fly_speed;
                    s16 iVar3 = cM_deg2s(mpHIO->m.fly_angle);
                    daPy_getPlayerActorClass()->setThrowDamage(shape_angle.y,
                        fVar1 * cM_scos(iVar3), fVar1 * cM_ssin(iVar3), 0, 1, 0);
                }
            } else {
                rv = 1;
            }
            break;

        case 5:
            if (mMotionSeqMngr.getNo() == MOTION_PICKUP) {
                if (mMotionSeqMngr.getStepNo() == 0) {
                    if (mpMorf[0]->checkFrame(19.0f)) {
                        field_0x1578 = 1;
                        actor_p = (daObj_Gadget_c*)mActorMngr[4].getActorP();
                        if (actor_p != NULL) {
                            ((daObj_Gadget_c*)actor_p)->onHide();
                        }
                    }
                } else {
                    rv = 1;
                }
            }
            break;
        case 7:
            if (mMotionSeqMngr.getNo() == MOTION_POUR) {
                if (mMotionSeqMngr.getStepNo() == 0) {
                    if (mpMorf[0]->checkFrame(18.0f)) {
                        if (field_0x1578 != 0) {
                            mDoAud_seStart(Z2SE_CM_BODYFALL_WATER_S, NULL, 0, 0);
                        }

                        field_0x1578 = 0;
                    }
                } else if (cLib_calcTimer(&mEventTimer) == 0) {
                    rv = 1;
                }
            }
            break;

        case 6:
            if (talkProc(NULL, FALSE, NULL, FALSE) && mFlow.checkEndFlow()) {
                rv = 1;
            }
            break;

        case 8:
            mJntAnm.lookPlayer(0);

            if (talkProc(NULL, FALSE, NULL, FALSE) && mFlow.checkEndFlow()) {
                rv = 1;
            }
            break;
    }

    field_0x1580 = 1;
    if (prm == 8) {
        field_0x1580 = 0;
    }

    if (prm == 5 || prm == 6) {
        if (field_0x1578 != 0) {
            field_0x1575 = 1;
            mDoMtx_stack_c::copy(mpMorf[0]->getModel()->getAnmMtx(JNT_FINGERR));
            mDoMtx_stack_c::multVecZero(&field_0xd6c);
            mJntAnm.lookPos(&field_0xd6c, 0);
        } else {
            actor_p = mActorMngr[4].getActorP();
            if (actor_p == NULL) {
                mJntAnm.lookNone(0);
                return rv;
            }

            field_0xd6c = actor_p->current.pos;
        }
    } else if (prm == 7) {
        mJntAnm.lookPos(&field_0x1508, 0);
        field_0x1575 = 1;
    }

    return rv;
}

int daNpc_ykM_c::cutLv5DungeonClear(int i_cutIndex) {
    fopAc_ac_c* actor_p = NULL;
    daPy_py_c* player = (daPy_py_c*)daPy_getPlayerActorClass();
    cXyz work;
    int rv = 0;
    int* piVar1 = NULL;
    int prm = -1;
    int timer = 0;

    piVar1 = dComIfGp_evmng_getMyIntegerP(i_cutIndex, "prm");
    if (piVar1 != NULL) {
        prm = *piVar1;
    }

    piVar1 = dComIfGp_evmng_getMyIntegerP(i_cutIndex, "timer");
    if (piVar1 != NULL) {
        timer = *piVar1;
    }

    s16 angleY;
    if (dComIfGp_getEventManager().getIsAddvance(i_cutIndex)) {
        switch (prm) {
            case 0:
                mFaceMotionSeqMngr.setNo(FACE_NONE, 0.0f, FALSE, 0);
                mMotionSeqMngr.setNo(MOTION_WAIT_X, 0.0f, FALSE, 0);
                mJntAnm.lookNone(1);
                mHide = false;
                fopAcM_OffStatus(this, fopAcM_STATUS_UNK_0x8000000);
                break;

            case 1:
                mMotionSeqMngr.setNo(MOTION_RUN, -1.0f, FALSE, 0);
                mSound.startCreatureVoice(Z2SE_YKM_V_DASH, -1);
                mEventTimer = timer;
                angleY = fopAcM_searchActorAngleY(player, this);
                ((daPy_py_c*)daPy_getPlayerActorClass())->setPlayerPosAndAngle(&player->current.pos, angleY, 0);
                dComIfGp_getEvent()->setPt2(this);
                break;

            case 2:
                mFaceMotionSeqMngr.setNo(FACE_NONE, 0.0f, FALSE, 0);
                mMotionSeqMngr.setNo(MOTION_WAITLIE, 0.0f, FALSE, 0);
                mEventTimer = timer;
                break;

            case 3:
                mMotionSeqMngr.setNo(MOTION_WAKE, -1.0f, FALSE, 0);
                break;

            case 4:
                mFaceMotionSeqMngr.setNo(FACE_SHUT, -1.0f, FALSE, 0);
                mMotionSeqMngr.setNo(MOTION_DENY, -1.0f, FALSE, 0);
                break;

            case 5:
                mFaceMotionSeqMngr.setNo(FACE_TALK_X, 0.0f, FALSE, 0);
                mMotionSeqMngr.setNo(MOTION_WAITWAKE, 0.0f, FALSE, 0);
                break;

            case 6:
                mFaceMotionSeqMngr.setNo(FACE_NONE, -1.0f, FALSE, 0);
                mMotionSeqMngr.setNo(MOTION_HOLD, -1.0f, FALSE, 0);
                break;

            case 7:
                mFaceMotionSeqMngr.setNo(FACE_TALK_X, -1.0f, FALSE, 0);
                break;

            case 8:
                mFaceMotionSeqMngr.setNo(FACE_H_HAPPY, -1.0f, FALSE, 0);
                mMotionSeqMngr.setNo(MOTION_HOLD_B, -1.0f, FALSE, 0);
                mSound.startCreatureVoice(Z2SE_YKM_V_HUG, -1);
                break;

            case 10:
                mEventTimer = timer;
                break;

            case 12:
                mFaceMotionSeqMngr.setNo(FACE_SHOUT, -1.0f, FALSE, 0);
                mEventTimer = timer;
                daPy_py_c::getMidnaActor()->onTagNoHairLead();
                break;

            case 13:
                Z2GetAudioMgr()->bgmStreamStop(0x28);
                break;

            case 20:
                mFaceMotionSeqMngr.setNo(FACE_NONE, 0.0f, FALSE, 0);
                mMotionSeqMngr.setNo(MOTION_STOP, 0.0f, FALSE, 0);
                actor_p = mActorMngr[5].getActorP();

                JUT_ASSERT(4401, NULL != actor_p);

                setPos(actor_p->home.pos);
                setAngle(actor_p->home.angle.y);
                break;

            case 21:
                mFaceMotionSeqMngr.setNo(FACE_TALK_Y, -1.0f, FALSE, 0);
                break;

            case 30:
                field_0x1581 = 1;
                field_0x157a = 1;
                break;

            case 31:
                field_0x157a = 0;
                break;

            case 99:
                mFaceMotionSeqMngr.setNo(FACE_H_HAPPY, 0.0f, FALSE, 0);
                mMotionSeqMngr.setNo(MOTION_WAITHOLD_B_B, 0.0f, FALSE, 0);
                actor_p = mActorMngr[5].getActorP();

                JUT_ASSERT(4424, NULL != actor_p);

                setPos(actor_p->home.pos);
                setAngle(actor_p->home.angle.y);
                speedF = 0.0f;
                speed.setall(0.0f);
                field_0x157a = 0;
                field_0x1580 = 0;
                field_0x1581 = 1;
                break;
        }
    }

    int iVar2[2] = {-1, -1};
    switch (prm) {
        case 0:
            rv = 1;
            break;

        case 11:
        case 1:
            if (mEventTimer != 0 || dComIfGp_getEventManager().getIsAddvance(i_cutIndex)) {
                actor_p = mActorMngr[5].getActorP();

                JUT_ASSERT(4455, NULL != actor_p);

                if (cLib_calcTimer(&mEventTimer) == 0) {
                    speedF = 0.0f;
                } else {
                    current.angle.y = cLib_targetAngleY(&current.pos, &actor_p->current.pos);
                    cLib_addCalcAngleS2(&shape_angle.y, current.angle.y, 4, 0x800);
                    mCurAngle.y = shape_angle.y;
                    speedF = mpHIO->m.run_speed;
                }
            }

            if (prm == 1) {
                for (int i = 0; i < 4; i++) {
                    if (field_0xf94[i].ChkCoHit()) {
                        if (daPy_getPlayerActorClass() == field_0xf94[i].GetCoHitAc()) {
                            rv = 1;
                        }
                    }
                }
                
                if (rv == 0 && field_0xe58.ChkCoHit()) {
                    if (daPy_getPlayerActorClass() == field_0xe58.GetCoHitAc()) {
                        rv = 1;
                    }
                }

                if (rv != 0) {
                    angleY = fopAcM_searchActorAngleY(this, player);
                    angleY += (s16)0x4000;
                    daPy_getPlayerActorClass()->setPlayerPosAndAngle(&player->current.pos, angleY, 0);
                    field_0x1580 = 1;
                }
            }

            if (prm == 11) {
                field_0x1580 = 1;
                rv = mEventTimer == 0;
            }
            break;

        case 2:
        case 5:
        case 6:
        case 7:
        case 8:
        case 21:
        case 30:
        case 31:
        case 99:
            rv = 1;
            break;

        case 3:
            if (mMotionSeqMngr.getStepNo() > 0) {
                rv = 1;
            }
            break;

        case 4:
            if (mMotionSeqMngr.getStepNo() > 0) {
                rv = 1;
            }
            break;

        case 12:
        case 10:
            if (mEventTimer != 0) {
                if (cLib_calcTimer(&mEventTimer) == 0) {
                    rv = 1;
                }
            } else {
                rv = 1;
            }
            break;

        case 13:
            rv = 1;
            break;

        case 20:
            field_0x1580 = 1;

            if (mMotionSeqMngr.checkEndSequence()) {
                daPy_getPlayerActorClass()->changeDemoMoveAngle(0x7800);
                rv = 1;
            }
            break;
    }

    actor_p = mActorMngr[5].getActorP();
    if (actor_p != NULL && (prm == 2 || prm == 3 || prm == 4 || prm == 5 || prm == 6 || prm == 7 || prm == 8 || prm == 20 || prm == 21 || prm == 30 || prm == 31)) {
        field_0xd6c = actor_p->eyePos;
        field_0xd8a.y = 0xC000;
        mJntAnm.lookPos(&field_0xd6c, 0);
    } else {
        mJntAnm.lookNone(0);
    }

    if (prm == 8) {
        field_0x1580 = 0;
    }

    field_0x1575 = 1;
    field_0x1574 = 1;
    field_0xe26 = false;

    return rv;
}

int daNpc_ykM_c::cutFindWolf(int i_cutIndex) {
    int rv = 0;
    int* piVar1 = NULL;
    int prm = -1;

    piVar1 = dComIfGp_evmng_getMyIntegerP(i_cutIndex, "prm");
    if (piVar1 != NULL) {
        prm = *piVar1;
    }

    fopAc_ac_c* actors[2] = {mActorMngr[5].getActorP(), this};
    dComIfGp_setMesgCameraInfoActor(actors[0], actors[1], NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

    if (dComIfGp_getEventManager().getIsAddvance(i_cutIndex)) {
        switch (prm) {
            case 0:
                mPlayerAngle = fopAcM_searchPlayerAngleY(this);
                break;

            case 1:
                mPlayerActorMngr.entry(daPy_getPlayerActorClass());
                initTalk(mFlowNodeNo, NULL);
                break;
        }
    }

    int unusedIntArr1[2] = {-1, -1};
    switch (prm) {
        case 0:
            mJntAnm.lookPlayer(0);

            if (mPlayerAngle != mCurAngle.y) {
                step(mPlayerAngle, 15, 30, mpHIO->m.step_speed, 0);
            } else {
                rv = 1;
            }
            break;

        case 1:
            if (talkProc(NULL, TRUE, &actors[0], FALSE) && mFlow.checkEndFlow()) {
                mFaceMotionSeqMngr.setNo(FACE_NONE, -1.0f, FALSE, 0);
                mMotionSeqMngr.setNo(MOTION_WAIT_A, -1.0f, FALSE, 0);
                rv = 1;
            }
            break;
    }

    return rv;
}

int daNpc_ykM_c::cutStartSnowboardRace(int i_cutIndex) {
    fopAc_ac_c* actor_p = NULL;
    cXyz work;
    csXyz angle;
    int rv = 0;
    int* piVar1 = NULL;
    int prm = -1;
    int timer = 0;

    piVar1 = dComIfGp_evmng_getMyIntegerP(i_cutIndex, "prm");
    if (piVar1 != NULL) {
        prm = *piVar1;
    }

    piVar1 = dComIfGp_evmng_getMyIntegerP(i_cutIndex, "timer");
    if (piVar1 != NULL) {
        timer = *piVar1;
    }

    if (dComIfGp_getEventManager().getIsAddvance(i_cutIndex)) {
        switch (prm) {
            case 0:
                mEventTimer = timer;
                break;

            case 1:
                mFaceMotionSeqMngr.setNo(FACE_NONE, 0.0f, FALSE, 0);
                mMotionSeqMngr.setNo(MOTION_SB_WAIT, 0.0f, FALSE, 0);
                mJntAnm.lookNone(1);
                field_0x1585 = 0;
                field_0x157e = 0;
                field_0x1576 = 1;
                field_0x157c = 0;
                daNpcT_onTmpBit(0x54); // dSv_event_tmp_flag_c::T_0084 - Snowpeak mountain - Racing with Yeto
                field_0x157e = 0;
                actor_p = getOtherYkmP(5);

                JUT_ASSERT(4736, NULL != actor_p);

                setPos(actor_p->current.pos);
                setAngle(actor_p->shape_angle.y);

                if (daNpcT_getPlayerInfoFromPlayerList(20, fopAcM_GetRoomNo(this), &work, &angle)) {
                    daPy_getPlayerActorClass()->setPlayerPosAndAngle(&work, angle.y, 0);
                    dComIfGp_evmng_setGoal(&work);
                }
                break;

            case 2:
                field_0x157b = 1;
                mDoAud_subBgmStart(Z2BGM_SNOW_BOARD);
                break;

            case 4:
                field_0x1534 = -1;
                break;

            case 5:
                home.pos.set(-72610.0f, -54260.0f, 56590.0f);
                home.angle.setall(0);
                home.angle.y = cM_deg2s(60.0f);
                setPos(home.pos);
                setAngle(home.angle.y);
                break;
        }
    }

    switch (prm) {
        case 0:
            if (cLib_calcTimer(&mEventTimer) == 0) {
                rv = 1;
            }
            break;

        case 1:
            rv = 1;
            break;

        case 2:
            if (field_0x1534 != -1) {
                if ((dTimer_c*)fpcM_SearchByID(field_0x1534) == dComIfG_getTimerPtr() && dComIfG_TimerStart(5, 0) != 0) {
                    rv = 1;
                }
            }
            break;

        case 3:
            if (field_0x1534 != -1) {
                if ((dTimer_c*)fpcM_SearchByID(field_0x1534) == dComIfG_getTimerPtr()) {
                    if (dComIfG_getTimerPtr()->isStart()) {
                        rv = 1;
                    }
                }
            }
            break;

        case 4:
            if (field_0x1534 != -1) {
                if ((dTimer_c*)fpcM_SearchByID(field_0x1534) == dComIfG_getTimerPtr()) {
                    rv = 1;
                }
            } else if (dComIfG_getTimerPtr() != NULL) {
                dComIfG_TimerDeleteRequest(dComIfG_getTimerMode());
            } else {
                field_0x1534 = dTimer_createTimer(5, 0x989298, 1, 0, 210.0f, 410.0f, 32.0f, 419.0f);
            }
            break;

        case 5:
            rv = 1;
            break;
    }

    return rv;
}

int daNpc_ykM_c::cutEndSnowboardRace(int i_cutIndex) {
    fopAc_ac_c* actor_p = NULL;
    cXyz work;
    csXyz angle;
    int rv = 0;
    int* piVar1 = NULL;
    int prm = -1;
    int timer = 0;

    piVar1 = dComIfGp_evmng_getMyIntegerP(i_cutIndex, "prm");
    if (piVar1 != NULL) {
        prm = *piVar1;
    }

    piVar1 = dComIfGp_evmng_getMyIntegerP(i_cutIndex, "timer");
    if (piVar1 != NULL) {
        timer = *piVar1;
    }

    if (dComIfGp_getEventManager().getIsAddvance(i_cutIndex)) {
        switch (prm) {
            case 0:
                mEventTimer = timer;
                break;

            case 2:
                mFaceMotionSeqMngr.setNo(FACE_NONE, 0.0f, FALSE, 0);
                mMotionSeqMngr.setNo(MOTION_WAIT_A, 0.0f, FALSE, 0);
                mJntAnm.lookPlayer(0);
                field_0x1576 = 0;
                daNpcT_offTmpBit(0x54); // dSv_event_tmp_flag_c::T_0084 - Snowpeak mountain - Racing with Yeto
                field_0x157b = 0;
                field_0x1558.field_0x0 = 0;
                field_0x1560 = 0;
                mPath.setIdx(0);
                actor_p = getOtherYkmP(6);

                JUT_ASSERT(4900, NULL != actor_p);

                setPos(actor_p->current.pos);
                angle.setall(0);
                angle.y = actor_p->shape_angle.y;
                home.angle.y = angle.y;
                setAngle(angle);

                if (field_0x1534 != -1) {
                    if ((dTimer_c*)fpcM_SearchByID(field_0x1534) == dComIfG_getTimerPtr()) {
                        dComIfG_TimerDeleteRequest(5);
                    }
                }

                if (daNpcT_getPlayerInfoFromPlayerList(12, fopAcM_GetRoomNo(this), &work, &angle)) {
                    angle.y = fopAcM_searchActorAngleY(daPy_getPlayerActorClass(), this);
                    daPy_getPlayerActorClass()->setPlayerPosAndAngle(&work, angle.y, 0);
                    dComIfGp_evmng_setGoal(&work);
                }

                mDoAud_subBgmStop();
                speed.setall(0.0f);
                speedF = 0.0f;
                break;

            case 3:
                daNpcT_onTmpBit(0x56); // dSv_event_tmp_flag_c::T_0086 - Snowpeak mountain - Won race against Yeto/Yeta

                if (field_0x157c != 0) {
                    daNpcT_offTmpBit(0x56); // dSv_event_tmp_flag_c::T_0086 - Snowpeak mountain - Won race against Yeto/Yeta
                }

                mFlowNodeNo = 0x1FE;
                initTalk(mFlowNodeNo, NULL);
                break;

            case 5:
                mFaceMotionSeqMngr.setNo(FACE_NONE, 0.0f, FALSE, 0);
                mMotionSeqMngr.setNo(MOTION_WAIT_A, 0.0f, FALSE, 0);
                actor_p = getOtherYkmP(6);

                JUT_ASSERT(4943, NULL != actor_p);

                home.pos = actor_p->current.pos;
                angle.setall(0);
                angle.y = actor_p->shape_angle.y;
                home.angle.y = angle.y;
                setPos(home.pos);
                setAngle(angle);
        }
    }

    int unusedIntArr1[2] = {-1, -1};
    switch (prm) {
        case 0:
            action();

            if (cLib_calcTimer(&mEventTimer) == 0) {
                rv = 1;
            }
            break;

        case 1:
            action();
            rv = 1;
            break;

        case 2:
            rv = 1;
            break;

        case 3: {
            if (talkProc(NULL, FALSE, NULL, FALSE)) {
                int iVar1;
                int eventId = mFlow.getEventId(&iVar1);
                switch (eventId) {
                case 13:
                    mEvtNo = EVENT_START_SNOWBOARDRACE;
                    evtChange();
                    break;

                default:
                    if (mFlow.checkEndFlow()) {
                        rv = 1;
                    }
                    break;
                }
            }
            break;
        }

        case 4:
            rv = 1;
            break;

        case 5:
            mJntAnm.lookPlayer(0);
            rv = 1;
            break;
    }

    return rv;
}

int daNpc_ykM_c::cutHug(int i_cutIndex) {
    int prm;
    int rv = 0;
    int* piVar1 = NULL;
    prm = -1;

    piVar1 = dComIfGp_evmng_getMyIntegerP(i_cutIndex, "prm");
    if (piVar1 != NULL) {
        prm = *piVar1;
    }

    if (dComIfGp_getEventManager().getIsAddvance(i_cutIndex)) {
        (void)0;
    }
    field_0xe26 = false;

    return rv;
}

void daNpc_ykM_c::setDialogueMotion() {
    int uVar1 = field_0x154c;
    uVar1 >>= 1;
    
    if (0.5f < cM_rnd()) {
        int dummy; // force r31 to be used for stack pointer instead of r1
        if (cM_rnd() - 0.5f < 0.0f) {
            uVar1--;
        } else {
            uVar1++;
        }
    }

    static int const motionTbl2[4] = {
        MOTION_TALK_B, MOTION_TALK_C, MOTION_LAUGH, MOTION_TALK_B,
    };
    static int const motionTbl1[2] = {
        MOTION_TALK_B, MOTION_ASK,
    };

    if ((field_0x154c & 1) != 0) {
        uVar1 &= 0x3;
        mMotionSeqMngr.setNo(motionTbl2[uVar1], -1.0f, FALSE, 0);
    } else {
        uVar1 &= 0x1;
        mMotionSeqMngr.setNo(motionTbl1[uVar1], -1.0f, FALSE, 0);
    }
}

int daNpc_ykM_c::dialogue() {
    daNpc_ykW_c* actor_p = NULL;

    switch (field_0x1548) {
        case 0:
            if (field_0x1544 != 0 && cLib_calcTimer(&field_0x1544) == 0) {
                field_0x1548++;

                if ((field_0x154c & 1) != 0) {
                    dComIfGs_offTmpBit(0xD01);
                    dComIfGs_onTmpBit(0xE80);
                    actor_p = (daNpc_ykW_c*)mActorMngr[5].getActorP();

                    if (actor_p != NULL) {
                        actor_p->setDialogueTmr1(field_0x154c);
                    }
                } else {
                    dComIfGs_onTmpBit(0xD01);
                    dComIfGs_offTmpBit(0xE80);
                    setDialogueMotion();
                }
            }
            break;

        case 1:
        case 2:
            if (dComIfGs_isTmpBit(0xD01)) {
                if (mMotionSeqMngr.getStepNo() > 0) {
                    dComIfGs_offTmpBit(0xD01);
                    dComIfGs_onTmpBit(0xE80);

                    if (field_0x1548 == 1) {
                        actor_p = (daNpc_ykW_c*)mActorMngr[5].getActorP();

                        if (actor_p != NULL) {
                            actor_p->setDialogueTmr2(field_0x154c);
                        }
                    }

                    field_0x1548++;
                }
            } else if (!dComIfGs_isTmpBit(0xE80)) {
                dComIfGs_onTmpBit(0xD01);
                dComIfGs_offTmpBit(0xE80);

                if (field_0x1548 == 1) {
                    setDialogueMotion();
                }

                field_0x1548++;
            }
            break;

        case 3:
            return 1;
    }

    return 0;
}

BOOL daNpc_ykM_c::wait(void* param_1) {
    daTag_EvtArea_c* actor_p = NULL;
    daPy_py_c* player = daPy_getPlayerActorClass();

    switch (mMode) {
        case 0:
        case 1:
            if (!mStagger.checkStagger()) {
                if (field_0x1581 != 0) {
                    mFaceMotionSeqMngr.setNo(FACE_H_HAPPY, 0.0f, FALSE, 0);
                    mMotionSeqMngr.setNo(MOTION_WAITHOLD_B_B, 0.0f, FALSE, 0);
                } else if (mType == TYPE_2) {
                    mFaceMotionSeqMngr.setNo(FACE_NONE, -1.0f, FALSE, 0);
                    mMotionSeqMngr.setNo(MOTION_FISH_WAIT_A_2, -1.0f, FALSE, 0);
                } else if (daNpcT_chkTmpBit(0x55)) { // dSv_event_tmp_flag_c::T_0085 - Snowpeak mountain - Racing with Yeta
                    mFaceMotionSeqMngr.setNo(FACE_NONE, -1.0f, FALSE, 0);
                    mMotionSeqMngr.setNo(MOTION_FUNWAIT, -1.0f, FALSE, 0);
                } else {
                    mFaceMotionSeqMngr.setNo(FACE_NONE, -1.0f, FALSE, 0);
                    mMotionSeqMngr.setNo(MOTION_WAIT_A, -1.0f, FALSE, 0);
                }

                field_0x1544 = cLib_getRndValue(180, 90);
                field_0x1548 = 0;
                field_0x154c++;
                mMode = 2;
            }
            // fallthrough
        case 2:
            if (!daPy_py_c::checkNowWolf() && field_0x1585 == 0) {
                for (int i = 0; i < 2; i++) {
                    actor_p = (daTag_EvtArea_c*)mActorMngr[i + 6].getActorP();

                    if (actor_p != NULL) {
                        if (actor_p->chkPointInArea(player->current.pos)) {
                            if (mType == TYPE_2) {
                                if (!daPy_py_c::checkNowWolf()) {
                                    mEvtNo = EVENT_SLIDEDOWN;
                                }
                            } else {
                                mSpeakEvent = true;
                                field_0x1583 = 1;
                            }
                        }
                    }
                }
            }

            if (field_0x1581 != 0) {
                mJntAnm.lookNone(0);
            } else if (!mStagger.checkStagger()) {
                if (mType == TYPE_4) {
                    if (field_0x1583 != 0) {
                        mPlayerActorMngr.entry(daPy_getPlayerActorClass());
                    }
                } else if (mType == TYPE_2) {
                    if (mPlayerActorMngr.getActorP() != NULL) {
                        if (cLib_calcTimer(&field_0x1538) == 0) {
                            field_0x1538 = cLib_getRndValue(90, 90);
                            field_0x153c ^= 1;
                        }

                        if (field_0x153c != 0) {
                            mPlayerActorMngr.remove();
                        }
                    } else {
                        field_0x1538 = 0;
                        field_0x153c = 0;
                    }
                }

                if (daNpcT_chkTmpBit(0x55)) { // dSv_event_tmp_flag_c::T_0085 - Snowpeak mountain - Racing with Yeta
                    mPlayerActorMngr.remove();
                }

                if (mPlayerActorMngr.getActorP() != NULL) {
                    mJntAnm.lookPlayer(0);

                    if (mType != TYPE_4) {
                        if (!chkActorInSight(mPlayerActorMngr.getActorP(), mAttnFovY, mCurAngle.y)) {
                            mJntAnm.lookNone(0);
                        }
                    }

                    if (!srchPlayerActor() && home.angle.y == mCurAngle.y) {
                        mMode = 1;
                    }
                } else {
                    mJntAnm.lookNone(0);

                    if (home.angle.y != mCurAngle.y) {
                        if (field_0xe34 != 0) {
                            if (step(home.angle.y, 15, 30, mpHIO->m.step_speed, 0)) {
                                mMode = 1;
                            }
                        } else {
                            setAngle(home.angle.y);
                            mMode = 1;
                        }

                        attention_info.flags = 0;
                    } else {
                        srchPlayerActor();
                    }
                }

                if (mType == TYPE_4) {
                    dComIfGs_offTmpBit(0xD02);
                }

                switch (mJntAnm.getMode()) {
                    case daNpcT_JntAnm_c::LOOK_PLAYER:
                        if (mType == TYPE_4) {
                            dComIfGs_onTmpBit(0xD02);

                            if (mMotionSeqMngr.getNo() != MOTION_WAIT_A) {
                                mMotionSeqMngr.setNo(MOTION_WAIT_A, -1.0f, FALSE, 0);
                            }
                        }
                        break;

                    case daNpcT_JntAnm_c::LOOK_NONE:
                        if (mType == TYPE_4) {
                            if (daNpcT_chkTmpBit(0x55)) { // dSv_event_tmp_flag_c::T_0085 - Snowpeak mountain - Racing with Yeta
                                actor_p = (daTag_EvtArea_c*)mActorMngr[5].getActorP();

                                if (actor_p != NULL) {
                                    mJntAnm.lookActor(actor_p, -40.0f, 0);
                                }
                            } else if (dComIfGs_isTmpBit(0xE40)) {
                                mJntAnm.lookPlayer(0);
                            } else {
                                actor_p = (daTag_EvtArea_c*)mActorMngr[5].getActorP();

                                if (actor_p != NULL) {
                                    mJntAnm.lookActor(actor_p, -40.0f, 0);

                                    if (dialogue() != 0) {
                                        mMode = 1;
                                    }
                                }
                            }
                        }
                        break;
                }
            }

        case 3:
            break;
    }

    return TRUE;
}

BOOL daNpc_ykM_c::cook(void* param_1) {
    fopAc_ac_c* actor_p = NULL;
    daPy_py_c* player = daPy_getPlayerActorClass();
    int waitTime = mpHIO->m.wait_time;
    BOOL bVar1 = FALSE;

    field_0x1508.set(0.0f, 200.0f, 450.0f);
    mDoMtx_stack_c::YrotS(home.angle.y);
    mDoMtx_stack_c::multVec(&field_0x1508, &field_0x1508);
    field_0x1508 += current.pos;

    switch (mMode) {
        case 0:
        case 1:
            if (!mStagger.checkStagger()) {
                mFaceMotionSeqMngr.setNo(FACE_NONE, -1.0f, FALSE, 0);
                mMotionSeqMngr.setNo(MOTION_WAIT_A, -1.0f, FALSE, 0);
                field_0x1538 = 0;
                field_0x153c = 1;
                mMode = 2;
            }
            // fallthrough
        case 2: {
            if (!daPy_py_c::checkNowWolf() && !checkItemGet(fpcNm_ITEM_TOMATO_PUREE, 1) && !checkItemGet(fpcNm_ITEM_TASTE, 1) &&
                !daNpcT_chkEvtBit(10) /* dSv_event_flag_c::F_0006 - Snowpeak Ruins - First conversation with Yeto in kitchen */) {
                actor_p = mActorMngr[6].getActorP();
                if (actor_p != NULL &&
                    ((daTag_EvtArea_c*)actor_p)->chkPointInArea(player->current.pos)) {
                    mEvtNo = EVENT_MEETING_AGAIN;
                    field_0xe33 = true;
                    }
                }

            if (!mStagger.checkStagger()) {
                if (daNpcT_chkEvtBit(10) /* dSv_event_flag_c::F_0006 - Snowpeak Ruins - First conversation with Yeto in kitchen */) {
                    if (!chkContinueAttnPlayer()) {
                        mPlayerActorMngr.remove();
                    }

                    if (!daNpcT_chkEvtBit(3) /* dSv_event_flag_c::TEST_003 - Snowpeak Ruins - Handed over tomato puree */) {
                        bVar1 = TRUE;
                    }
                }

                if (mPlayerActorMngr.getActorP() != NULL) {
                    mJntAnm.lookPlayer(0);

                    if (!chkActorInSight(mPlayerActorMngr.getActorP(), mAttnFovY, mCurAngle.y)) {
                        mJntAnm.lookPos(&field_0x1508, 0);
                    }

                    if (!srchPlayerActor() && home.angle.y == mCurAngle.y) {
                        mMode = 1;
                    }
                } else {
                    if (mMotionSeqMngr.getNo() == MOTION_TALK_E && mMotionSeqMngr.getStepNo() > 0) {
                        mJntAnm.lookPos(&field_0x1508, 0);
                    }

                    if (cLib_calcTimer(&field_0x1538) == 0) {
                        if (field_0x153c != 0) {
                            mFaceMotionSeqMngr.setNo(FACE_NONE, -1.0f, FALSE, 0);
                            mMotionSeqMngr.setNo(MOTION_WAIT_A, -1.0f, FALSE, 0);
                            mJntAnm.lookPos(&field_0x1508, 0);
                            field_0x1538 = cLib_getRndValue(waitTime / 2, waitTime);
                            field_0x1538 <<= 1;
                        } else {
                            f32 rnd = cM_rnd();
                            if (bVar1 && 0.6f < rnd) {
                                mJntAnm.lookAround2(0, 0x2D);
                            } else {
                                rnd = cM_rnd();
                                if (0.5f < rnd) {
                                    mFaceMotionSeqMngr.setNo(FACE_NONE, -1.0f, FALSE, 0);
                                    mMotionSeqMngr.setNo(MOTION_TALK_E, -1.0f, FALSE, 0);
                                    mJntAnm.lookNone(0);
                                } else {
                                    mFaceMotionSeqMngr.setNo(FACE_NONE, -1.0f, FALSE, 0);
                                    mMotionSeqMngr.setNo(MOTION_TALK_D, -1.0f, FALSE, 0);
                                    mJntAnm.lookNone(0);
                                }
                            }

                            field_0x1538 = waitTime;
                        }

                        field_0x153c ^= 1;
                    }
                }

                switch (mJntAnm.getMode()) {
                case daNpcT_JntAnm_c::LOOK_NONE:
                    break;

                case daNpcT_JntAnm_c::LOOK_POS:
                case daNpcT_JntAnm_c::LOOK_MODE_8:
                    mSound.startCreatureVoiceLevel(Z2SE_YKM_V_HUMMING, -1);
                }
            }
            break;
        }

        case 3:
            break;
    }

    return TRUE;
}

BOOL daNpc_ykM_c::race(void* param_1) {
    cXyz work;
    fopAc_ac_c* actors[2] = {this, daPy_getPlayerActorClass()};
    s16 angleY;

    switch (mMode) {
        case 0:
        case 1:
            if (!mStagger.checkStagger()) {
                mFaceMotionSeqMngr.setNo(FACE_NONE, -1.0f, FALSE, 0);
                mMotionSeqMngr.setNo(MOTION_SB_WAIT, -1.0f, FALSE, 0);
                field_0x1576 = 1;
                mMode = 2;
            }
            // fallthrough
        case 2:
            if (daNpcT_chkTmpBit(0x54)) {
                daNpcT_judgeRace(mRoomPath, actors, &field_0x1558, 2, &field_0x1550);
                field_0x14fc.x = mRoomPath->m_points[mRoomPath->m_num - 1].m_position.x;
                field_0x14fc.y = mRoomPath->m_points[mRoomPath->m_num - 1].m_position.y;
                field_0x14fc.z = mRoomPath->m_points[mRoomPath->m_num - 1].m_position.z;
                angleY = cLib_targetAngleY(&field_0x14fc, &daPy_getPlayerActorClass()->current.pos);

                if (field_0x1560 < 0 ||
                    (!DEBUG || field_0x1558.field_0x0 >= 0) &&
                    (field_0x14fc.abs2(daPy_getPlayerActorClass()->current.pos) < 640000.0f && (angleY < -0x4000 || angleY > 0x4000))) {
                    field_0x1560 = -1;
                    mGndChk.SetPos(&daPy_getPlayerActorClass()->current.pos);

                    if (dComIfG_Bgsp().GroundCross(&mGndChk) != -G_CM3D_F_INF) {
                        if (field_0x1534 != -1) {
                            if ((dTimer_c*)fpcM_SearchByID(field_0x1534) == dComIfG_getTimerPtr()) {
                                if (dComIfG_getTimerPtr()->isStart()) {
                                    if (field_0x157c != 0) {
                                        mDoAud_subBgmStart(Z2BGM_SNOWBOARD_LOSE);
                                        dMeter2Info_setMeterString(0x539);
                                    } else {
                                        mDoAud_subBgmStart(Z2BGM_SNOWBOARD_WIN);
                                        dMeter2Info_setMeterString(0x538);
                                    }

                                    dComIfG_TimerEnd(5, -1);
                                    if (dComIfG_getTimerNowTimeMs() < dComIfGs_getRaceGameTime() ||
                                        dComIfGs_getRaceGameTime() == 0) {
                                        dComIfGs_setRaceGameTime(dComIfG_getTimerNowTimeMs());
                                    }
                                }
                            }
                        }

                        mEvtNo = EVENT_END_SNOWBOARDRACE;
                    }
                }
#if DEBUG
                else if (field_0x1558.field_0x0 < 0) {
#else
                if (field_0x1560 >= 0 && field_0x1558.field_0x0 < 0) {
#endif
                    field_0x157c = 1;
                }
            }

            mJntAnm.lookNone(0);

            if (field_0x1585 != 0) {
                if (field_0x1576 != 0) {
                    cLib_addCalc2(&speedF, 0.0f, 0.15f, 3.0f);

                    if (speedF <= 0.15f) {
                        field_0x1576 = 0;
                        speedF = 0.0f;
                        speed.setall(0.0f);
                    }
                } else {
                    mJntAnm.lookNone(0);
                }
            } else {
                if (field_0x157e != 0 && mAcch.ChkGroundHit()) {
                    mMotionSeqMngr.setNo(MOTION_SB_JUMP_C, -1.0f, FALSE, 0);
                    mSound.startCreatureSound(Z2SE_YM_SNOBO_LAND, 0, -1);
                    field_0x157e = 0;
                }

                if (field_0x157e == 0) {
                    while (true) {
                        if (mPath.chkPassed1(current.pos, mPath.getNumPnts())) {
                            if (u8(mPath.getArg0()) == 0) {
                                mSound.startCreatureSound(Z2SE_YM_SNOBO_JUMP, 0, -1);
                                mSound.startCreatureVoice(Z2SE_YM_V_SNOBO_JUMP, -1);
                                field_0x157e = 1;
                            }

                            if (mPath.setNextIdx(mPath.getNumPnts()) != 0) {
                                field_0x1585 = 1;
                                mMotionSeqMngr.setNo(MOTION_SB_GOAL, -1.0f, FALSE, 0);
                            } else {
                                if (field_0x157e == 0) {
                                    continue;
                                }

                                if (!mStagger.checkStagger()) {
                                    mMotionSeqMngr.setNo(MOTION_SB_JUMP_A, -1.0f, FALSE, 0);
                                }

                                field_0x1514 = current.pos;
                                field_0x1520 = mPath.getPntPos(mPath.getIdx());
                                current.angle.y = cLib_targetAngleY(&field_0x1514, &field_0x1520);
                                int iVar3 = (int)((field_0x1520 - field_0x1514).absXZ() / speedF) + 1;
                                speed.y = (field_0x1520.y - field_0x1514.y) + fabsf(gravity) * iVar3 * iVar3 * 0.5f;
                                speed.y /= iVar3;
                            }
                        } else {
                            work = mPath.getPntPos(mPath.getIdx());
                        }
                        break;
                    }
                }

                if (field_0x1585 == 0 && field_0x157e == 0) {
                    angleY = cLib_targetAngleY(&current.pos, &work);
                    current.angle.y = angleY;
                    cLib_addCalcAngleS2(&shape_angle.y, current.angle.y, 8, 0x100);
                    mCurAngle.y = shape_angle.y;

                    if (!mStagger.checkStagger() && (mMotionSeqMngr.getNo() != MOTION_SB_JUMP_C || mMotionSeqMngr.checkEndSequence())) {
                        angleY -= mCurAngle.y;
                        if (0.0f < cM_sht2d(angleY)) {
                            if (4.0f < cM_sht2d(angleY)) {
                                mMotionSeqMngr.setNo(MOTION_SB_TURNL, 24.0f, FALSE, 0);
                            } else {
                                mMotionSeqMngr.setNo(MOTION_SB_WAIT, 24.0f, FALSE, 0);
                            }
                        } else {
                            if (cM_sht2d(angleY) < -4.0f) {
                                mMotionSeqMngr.setNo(MOTION_SB_TURNR, 24.0f, FALSE, 0);
                            } else {
                                mMotionSeqMngr.setNo(MOTION_SB_WAIT, 24.0f, FALSE, 0);
                            }
                        }
                    }

                    angleY = -fopAcM_getPolygonAngle(mGndChk, current.angle.y + 0x4000);
                    cLib_addCalcAngleS2(&mCurAngle.x, mGroundAngle, 6, 0x200);
                    cLib_addCalcAngleS2(&mCurAngle.z, angleY, 6, 0x200);
                    int iVar1 = field_0x1560 - field_0x1558.field_0x0;
                    f32 fVar2 = mpHIO->m.slide_speed * cM_scos(mGroundAngle);

                    if (mStagger.checkStagger()) {
                        fVar2 *= 0.0f;
                    } else {
                        if (chkTouchPlayer() && field_0x1540 == 0) {
                            s16 sVar2 = fopAcM_searchPlayerAngleY(this);
                            f32 fVar3 = daPy_getPlayerActorClass()->speedF;
                            s16 iVar3 = cM_deg2s(mpHIO->m.fly_angle);
                            daPy_getPlayerActorClass()->setThrowDamage(sVar2, fVar3 * cM_scos(iVar3), fVar3 * cM_ssin(iVar3), 0, 0, 0);
                            field_0x1540 = 8;
                        }

                        iVar1 += mpHIO->m.competition_prm_a;
                        iVar1 = cLib_minMaxLimit<int>(iVar1, 0, mpHIO->m.competition_prm_b);
                        if (iVar1 > 0) {
                            fVar2 *= abs(iVar1) * 0.1f * mpHIO->m.competition_prm_c + 1.0f;
                        }
                    }

                    cLib_chaseF(&speedF, fVar2, mpHIO->m.slide_acceleration);
                    mSound.startCreatureSoundLevel(Z2SE_YM_SNOBO_RIDE, speedF, -1);
                    mSound.startCreatureVoiceLevel(Z2SE_YM_V_SNOBO_RIDING, -1);
                }
            }

            attention_info.flags = 0;
            break;

        case 3:
            break;
    }

    dTres_c::setNpcYkmPosition(getBitTRB() & 0xFF, &current.pos);
    return TRUE;
}

BOOL daNpc_ykM_c::talk(void* param_1) {
    switch (mMode) {
        case 0:
        case 1:
            if (!mStagger.checkStagger()) {
                initTalk(mFlowNodeNo, NULL);
                field_0x157f = 0;

                if (mType == TYPE_COOK) {
                    mFaceMotionSeqMngr.setNo(FACE_NONE, -1.0f, FALSE, 0);
                    mMotionSeqMngr.setNo(MOTION_WAIT_A, -1.0f, FALSE, 0);

                    if (daNpcT_chkEvtBit(10) /* dSv_event_flag_c::F_0006 - Snowpeak Ruins - First conversation with Yeto in kitchen */ 
                        && !chkContinueAttnPlayer()) {
                        field_0x157f = 1;
                    }
                }

                mMode = 2;
            }
            // fallthrough
        case 2: {
            if (!mStagger.checkStagger()) {
                if (mTwilight || mPlayerAngle == mCurAngle.y || mType == TYPE_COOK || mType == TYPE_2) {
                    if (talkProc(NULL, FALSE, NULL, FALSE)) {
                        field_0x1583 = 0;

                        int iVar1;
                        int eventId = mFlow.getEventId(&iVar1);
                        switch (eventId) {
                        case 13:
                            mEvtNo = EVENT_START_SNOWBOARDRACE;
                            evtChange();
                            break;

                        default:
                            if (mFlow.checkEndFlow()) {
                                mPlayerActorMngr.entry(daPy_getPlayerActorClass());
                                dComIfGp_event_reset();
                                mMode = 3;
                            }
                            break;
                        }
                    }

                    mJntAnm.lookPlayer(0);

                    if (mTwilight || field_0x157f != 0) {
                        mJntAnm.lookNone(0);
                    }
                } else {
                    mJntAnm.lookPlayer(0);
                    step(mPlayerAngle, 15, 30, mpHIO->m.step_speed, 0);
                }
            }
            break;
        }

        case 3:
            break;
    }

    return FALSE;
}

BOOL daNpc_ykM_c::test(void* param_1) {
    int rv = FALSE;
    switch (mMode) {
    case 0:
    case 1:
        speedF = 0.0f;
        speed.setall(0.0f);
        mMode = 2;
        // fallthrough
    case 2:
        mFaceMotionSeqMngr.setNo(mpHIO->m.common.face_expression, -1.0f, FALSE, 0);
        mMotionSeqMngr.setNo(mpHIO->m.common.motion, -1.0f, FALSE, 0);
        mJntAnm.lookNone(0);
        attention_info.flags = 0;
        break;

    case 3:
        break;
    }

    return rv;
}

static int daNpc_ykM_Create(void* i_this) {
    return ((daNpc_ykM_c*)i_this)->create();
}

static int daNpc_ykM_Delete(void* i_this) {
    return ((daNpc_ykM_c*)i_this)->Delete();
}

static int daNpc_ykM_Execute(void* i_this) {
    return ((daNpc_ykM_c*)i_this)->Execute();
}

static int daNpc_ykM_Draw(void* i_this) {
    return ((daNpc_ykM_c*)i_this)->Draw();
}

static int daNpc_ykM_IsDelete(void* a_this) {
    return 1;
}

AUDIO_INSTANCES;

static actor_method_class daNpc_ykM_MethodTable = {
    (process_method_func)daNpc_ykM_Create,
    (process_method_func)daNpc_ykM_Delete,
    (process_method_func)daNpc_ykM_Execute,
    (process_method_func)daNpc_ykM_IsDelete,
    (process_method_func)daNpc_ykM_Draw,
};

actor_process_profile_definition g_profile_NPC_YKM = {
  fpcLy_CURRENT_e,          // mLayerID
  7,                        // mListID
  fpcPi_CURRENT_e,          // mListPrio
  PROC_NPC_YKM,             // mProcName
  &g_fpcLf_Method.base,    // sub_method
  sizeof(daNpc_ykM_c),      // mSize
  0,                        // mSizeOther
  0,                        // mParameters
  &g_fopAc_Method.base,     // sub_method
  380,                      // mPriority
  &daNpc_ykM_MethodTable,   // sub_method
  0x00040107,               // mStatus
  fopAc_NPC_e,              // mActorType
  fopAc_CULLBOX_CUSTOM_e,   // cullType
};
