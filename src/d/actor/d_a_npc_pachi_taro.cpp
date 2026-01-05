/**
 * @file d_a_npc_pachi_taro.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_npc_pachi_taro.h"
#include "JSystem/JHostIO/JORFile.h"
#include "d/actor/d_a_tag_pachi.h"
#include "d/actor/d_a_npc.h"
#include "d/actor/d_a_npc4.h"
#include "d/actor/d_a_npc_pachi_maro.h"
#include "d/actor/d_a_npc_pachi_besu.h"
#include "d/d_debug_viewer.h"

enum Taro_RES_File_ID {
    /* BMDR */
    /* 0x4 */ BMDR_TARO = 0x4,

    /* BTP */
    /* 0x7 */ BTP_TARO_F_NORMAL = 0x7,
};

enum Taro_TW_RES_File_ID {
    /* BCK */
    /* 0x06 */ BCK_TARO_DEMO_WAIT = 0x6,
    /* 0x07 */ BCK_TARO_F_DEMO_WAIT,

    /* BMDR */
    /* 0x0A */ BMDR_TARO_TW = 0xA,

    /* BTK */
    /* 0x0D */ BTK_TARO_DEMO_WAIT = 0xD,

    /* BTP */
    /* 0x10 */ BTP_TARO_F_DEMO_WAIT = 0x10,
};

enum Taro0_RES_File_ID {
    /* BCK */
    /* 0x5 */ BCK_TARO_DEFENSE_A = 0x5,
    /* 0x6 */ BCK_TARO_F_CLOSE,
    /* 0x7 */ BCK_TARO_F_NORMAL,
    /* 0x8 */ BCK_TARO_STEP,
    /* 0x9 */ BCK_TARO_WAIT_A,

    /* BTK */
    /* 0xC */ BTK_TARO = 0xC,

    /* BTP */
    /* 0xF */ BTP_TARO_F_CLOSE = 0xF,
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

enum Taro3_RES_File_ID {
    /* BCK */
    /* 0x4 */ BCK_TARO_F_ANGER = 0x4,
    /* 0x5 */ BCK_TARO_F_SMILE,
    /* 0x6 */ BCK_TARO_FH_ANGER,
    /* 0x7 */ BCK_TARO_FH_SMILE,
    /* 0x8 */ BCK_TARO_TO_WAIT_C,
    /* 0x9 */ BCK_TARO_VICTORY_A,
    /* 0xA */ BCK_TARO_WAIT_C,

    /* BTP */
    /* 0xD */ BTP_TARO_F_ANGER = 0xD,
    /* 0xE */ BTP_TARO_F_SMILE,
    /* 0xF */ BTP_TARO_FH_SMILE,
};

enum Taro4_RES_File_ID {
    /* BCK */
    /* 0x4 */ BCK_TARO_F_SURPRISEB = 0x4,
    /* 0x5 */ BCK_TARO_FH_SURPRISEB,
    /* 0x6 */ BCK_TARO_SURPRISEB,
    /* 0x7 */ BCK_TARO_SURPRISEB_TALK,

    /* BTP */
    /* 0xA */ BTP_TARO_F_SURPRISEB = 0xA,
    /* 0xB */ BTP_TARO_FH_SURPRISEB,
};

enum Taro5_RES_File_ID {
    /* BCK */
    /* 0x06 */ BCK_TARO_CRY = 0x6,
    /* 0x07 */ BCK_TARO_F_CRY,
    /* 0x08 */ BCK_TAROB_F_MISERABLE,
    /* 0x09 */ BCK_TAROB_F_SAVED,
    /* 0x0A */ BCK_TAROB_F_SCARED,
    /* 0x0B */ BCK_TAROB_FH_MISERABLE,
    /* 0x0C */ BCK_TAROB_FH_SCARED,
    /* 0x0D */ BCK_TAROB_SAD_TALK_A,
    /* 0x0E */ BCK_TAROB_SAD_UTUMUKI,
    /* 0x0F */ BCK_TAROB_SAD_WAIT,
    /* 0x10 */ BCK_TAROB_SAD_WALK,
    /* 0x11 */ BCK_TAROB_SAVED,
    /* 0x12 */ BCK_TAROB_SAVED_WAIT,
    /* 0x13 */ BCK_TAROB_SCARED,

    /* BTK */
    /* 0x16 */ BTK_TARO_CRY = 0x16,

    /* BTP */
    /* 0x19 */ BTP_TARO_F_CRY = 0x19,
    /* 0x1A */ BTP_TAROB_F_MISERABLE,
    /* 0x1B */ BTP_TAROB_F_SAVED,
    /* 0x1C */ BTP_TAROB_F_SCARED,
    /* 0x1D */ BTP_TAROB_FH_MISERABLE,
    /* 0x1E */ BTP_TAROB_FH_SCARED,

    /* EVT */
    /* 0x21 */ EVT_TARO5_EVENT_LIST = 0x21,
};

enum TaroB_RES_File_ID {
    /* BMDR */
    /* 0x4 */ BMDR_GM_TARO = 0x4,

    /* BTP */
    /* 0x7 */ BMDR_TAROB = 0x7,
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

enum evt_pachi_RES_File_ID {
    /* EVT */
    /* 0x3 */ EVT_EVT_PACHI_EVENT_LIST = 0x3,
};

enum RES_Name {
    /* 0x0 */ NONE,
    /* 0x1 */ TARO,
    /* 0x2 */ TARO_TW,
    /* 0x3 */ TARO0,
    /* 0x4 */ TARO1,
    /* 0x5 */ TARO2,
    /* 0x6 */ TARO3,
    /* 0x7 */ TARO4,
    /* 0x8 */ TARO5,
    /* 0x9 */ TAROB,
    /* 0xA */ LEN1,
    /* 0xB */ EVT_PACHI,
};

enum Face_Motion_Animation {
    /* 0x00 */ FACE_MOT_ANM_NONE,
    /* 0x01 */ FACE_MOT_ANM_NORMAL,
    /* 0x02 */ FACE_MOT_ANM_SAD,
    /* 0x03 */ FACE_MOT_ANM_SMILE,
    /* 0x04 */ FACE_MOT_ANM_SURPRISE,
    /* 0x05 */ FACE_MOT_ANM_PANIC,
    /* 0x06 */ FACE_MOT_ANM_ANGER,
    /* 0x07 */ FACE_MOT_ANM_SHOUT,
    /* 0x08 */ FACE_MOT_ANM_H_SAD,
    /* 0x09 */ FACE_MOT_ANM_H_SMILE,
    /* 0x0A */ FACE_MOT_ANM_H_SURPRISE,
    /* 0x0B */ FACE_MOT_ANM_H_PANIC,
    /* 0x0C */ FACE_MOT_ANM_H_ANGER,
    /* 0x0D */ FACE_MOT_ANM_CLOSE,
    /* 0x0E */ FACE_MOT_ANM_S_GET,
    /* 0x0F */ FACE_MOT_ANM_CALL,
    /* 0x10 */ FACE_MOT_ANM_WAVE,
    /* 0x11 */ FACE_MOT_ANM_WATCH,
    /* 0x12 */ FACE_MOT_ANM_MARVELOUS,
    /* 0x13 */ FACE_MOT_ANM_POINT,
    /* 0x14 */ FACE_MOT_ANM_HI,
    /* 0x15 */ FACE_MOT_ANM_SURPRISEB,
    /* 0x16 */ FACE_MOT_ANM_H_SURPRISEB,
    /* 0x17 */ FACE_MOT_ANM_DEMO_WAIT,
    /* 0x18 */ FACE_MOT_ANM_SCARED,
    /* 0x19 */ FACE_MOT_ANM_H_SCARED,
    /* 0x1A */ FACE_MOT_ANM_MISERABLE,
    /* 0x1B */ FACE_MOT_ANM_H_MISERABLE,
    /* 0x1C */ FACE_MOT_ANM_SAVED,
    /* 0x1D */ FACE_MOT_ANM_CRY,
};

enum Motion_Animation {
    /* 0x00 */ MOT_ANM_WAIT_A,
    /* 0x01 */ MOT_ANM_WAIT_B,
    /* 0x02 */ MOT_ANM_WAIT_C,
    /* 0x03 */ MOT_ANM_WAIT_D,
    /* 0x04 */ MOT_ANM_WAIT_E,
    /* 0x05 */ MOT_ANM_WAIT_F,
    /* 0x06 */ MOT_ANM_TO_WAIT_C,
    /* 0x07 */ MOT_ANM_NONE,
    /* 0x08 */ MOT_ANM_RUN,
    /* 0x09 */ MOT_ANM_SAD_WALK,
    /* 0x0A */ MOT_ANM_S_RUN,
    /* 0x0B */ MOT_ANM_TALK_A,
    /* 0x0C */ MOT_ANM_TALK_B,
    /* 0x0D */ MOT_ANM_VICTORY_A,
    /* 0x0E */ MOT_ANM_CHOP_A,
    /* 0x0F */ MOT_ANM_CHOP_B,
    /* 0x10 */ MOT_ANM_DEFENSE_A,
    /* 0x11 */ MOT_ANM_S_GET,
    /* 0x12 */ MOT_ANM_S_WAIT,
    /* 0x13 */ MOT_ANM_S_CHOP_A,
    /* 0x14 */ MOT_ANM_S_CHOP_B,
    /* 0x15 */ MOT_ANM_CALL,
    /* 0x16 */ MOT_ANM_WAVE,
    /* 0x17 */ MOT_ANM_WATCH,
    /* 0x18 */ MOT_ANM_WATCHING,
    /* 0x19 */ MOT_ANM_MARVELOUS,
    /* 0x1A */ MOT_ANM_POINT,
    /* 0x1B */ MOT_ANM_LOOKBACK,
    /* 0x1C */ MOT_ANM_HI,
    /* 0x1D */ MOT_ANM_HI_WAIT,
    /* 0x1E */ MOT_ANM_TO_WAIT_C2,
    /* 0x1F */ MOT_ANM_SURPRISEB,
    /* 0x20 */ MOT_ANM_SURPRISEB_TALK,
    /* 0x21 */ MOT_ANM_DEMO_WAIT,
    /* 0x22 */ MOT_ANM_STEP,
    /* 0x23 */ MOT_ANM_SCARED,
    /* 0x24 */ MOT_ANM_SAD_WAIT,
    /* 0x25 */ MOT_ANM_SAD_TALK_A,
    /* 0x26 */ MOT_ANM_SAD_UTUMUKI,
    /* 0x27 */ MOT_ANM_CRY,
    /* 0x28 */ MOT_ANM_SAVED,
};

enum Face_Motion {
    /* 0x00 */ FACE_MOT_NORMAL,
    /* 0x01 */ FACE_MOT_SAD,
    /* 0x02 */ FACE_MOT_SMILE,
    /* 0x03 */ FACE_MOT_SURPRISE,
    /* 0x04 */ FACE_MOT_PANIC,
    /* 0x05 */ FACE_MOT_ANGER,
    /* 0x06 */ FACE_MOT_CALL,
    /* 0x07 */ FACE_MOT_WAVE,
    /* 0x08 */ FACE_MOT_MARVELOUS,
    /* 0x09 */ FACE_MOT_S_GET,
    /* 0x0A */ FACE_MOT_POINT,
    /* 0x0B */ FACE_MOT_HI,
    /* 0x0C */ FACE_MOT_SURPRISEB,
    /* 0x0D */ FACE_MOT_SCARED,
    /* 0x0E */ FACE_MOT_MISERABLE,
    /* 0x0F */ FACE_MOT_SMILE2,
    /* 0x10 */ FACE_MOT_SHOUT,
    /* 0x11 */ FACE_MOT_CLOSE,
    /* 0x12 */ FACE_MOT_H_SAD,
    /* 0x13 */ FACE_MOT_H_SMILE,
    /* 0x14 */ FACE_MOT_H_SURPRISE,
    /* 0x15 */ FACE_MOT_H_PANIC,
    /* 0x16 */ FACE_MOT_H_ANGER,
    /* 0x17 */ FACE_MOT_WATCH,
    /* 0x18 */ FACE_MOT_H_SURPRISEB,
    /* 0x19 */ FACE_MOT_DEMO_WAIT,
    /* 0x1A */ FACE_MOT_H_SCARED,
    /* 0x1B */ FACE_MOT_H_MISERABLE,
    /* 0x1C */ FACE_MOT_SAVED,
    /* 0x1D */ FACE_MOT_SCARED2,
    /* 0x1E */ FACE_MOT_CRY,
    /* 0x1F */ FACE_MOT_NONE,
};

enum Motion {
    /* 0x00 */ MOT_WAIT_A,
    /* 0x01 */ MOT_CALL,
    /* 0x02 */ MOT_WAVE,
    /* 0x03 */ MOT_WAIT_B,
    /* 0x04 */ MOT_S_GET,
    /* 0x05 */ MOT_WATCHING,
    /* 0x06 */ MOT_MARVELOUS,
    /* 0x07 */ MOT_POINT,
    /* 0x08 */ MOT_WAIT_C,
    /* 0x09 */ MOT_TO_WAIT_C,
    /* 0x0A */ MOT_LOOKBACK,
    /* 0x0B */ MOT_WAIT_D,
    /* 0x0C */ MOT_HI,
    /* 0x0D */ MOT_TALK_A,
    /* 0x0E */ MOT_TALK_B,
    /* 0x0F */ MOT_VICTORY_A,
    /* 0x10 */ MOT_HI_WAIT,
    /* 0x11 */ MOT_TO_WAIT_C2,
    /* 0x12 */ MOT_SURPRISEB_TALK,
    /* 0x13 */ MOT_SURPRISEB,
    /* 0x14 */ MOT_WAIT_F,
    /* 0x15 */ MOT_SCARED,
    /* 0x16 */ MOT_SAD_WAIT,
    /* 0x17 */ MOT_SAD_TALK_A,
    /* 0x18 */ MOT_SAD_UTUMUKI,
    /* 0x19 */ MOT_S_WAIT,
    /* 0x1A */ MOT_CRY,
    /* 0x1B */ MOT_CHOP_A,
    /* 0x1C */ MOT_CHOP_B,
    /* 0x1D */ MOT_UNK_29,
    /* 0x1E */ MOT_UNK_30,
    /* 0x1F */ MOT_S_CHOP_A,
    /* 0x20 */ MOT_S_CHOP_B,
    /* 0x21 */ MOT_DEFENSE_A,
    /* 0x22 */ MOT_WATCH,
    /* 0x23 */ MOT_DEMO_WAIT,
    /* 0x24 */ MOT_STEP,
    /* 0x25 */ MOT_RUN,
    /* 0x26 */ MOT_S_RUN,
    /* 0x27 */ MOT_SAD_WALK,
    /* 0x28 */ MOT_SAVED,
};

enum Type {
    /* 0x0 */ TYPE_0,
};

enum Event {
    /* 0x0 */ EVT_NONE,
    /* 0x1 */ EVT_TUTRIAL_BEGIN,
    /* 0x2 */ EVT_TUTRIAL_BEGIN_SKIP,
    /* 0x3 */ EVT_TUTRIAL_CLEAR,
    /* 0x4 */ EVT_TUTRIAL_TALK,
    /* 0x5 */ EVT_TUTRIAL_TALK2,
    /* 0x6 */ EVT_HIT_KAKASI_BODY,
    /* 0x7 */ EVT_TUTRIAL_SELECT_GIVEUP,
    /* 0x8 */ EVT_TUTRIAL_CONTINUE,
    /* 0x9 */ EVT_TUTRIAL_GIVEUP,
    /* 0xA */ EVT_TUTRIAL_CAUTION,
};

#if DEBUG
daNpc_Pachi_Taro_HIO_c::daNpc_Pachi_Taro_HIO_c() { m = daNpc_Pachi_Taro_Param_c::m; }

void daNpc_Pachi_Taro_HIO_c::listenPropertyEvent(const JORPropertyEvent* event) {
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

void daNpc_Pachi_Taro_HIO_c::genMessage(JORMContext* ctx) {
    daNpcT_cmnGenMessage(ctx, &m.common);
    ctx->genButton("ファイル書き出し", 0x40000002, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
}
#endif

static int l_bmdData[5][2] = {
    {BMDR_TARO, TARO},
    {BMDR_TARO_STICK_A, TARO1},
    {BMDR_TARO_SWB, TARO1},
    {BMDR_TARO_TW, TARO_TW},
    {BMDR_GM_TARO, TAROB},
};

static daNpcT_evtData_c l_evtList[11] = {
    {"", NONE},
    {"TUTRIAL_BEGIN", EVT_PACHI},
    {"TUTRIAL_BEGIN_SKIP", EVT_PACHI},
    {"TUTRIAL_CLEAR", EVT_PACHI},
    {"TUTRIAL_TALK", EVT_PACHI},
    {"TUTRIAL_TALK2", EVT_PACHI},
    {"HIT_KAKASI_BODY", EVT_PACHI},
    {"TUTRIAL_SELECT_GIVEUP", EVT_PACHI},
    {"TUTRIAL_CONTINUE", EVT_PACHI},
    {"TUTRIAL_GIVEUP", EVT_PACHI},
    {"TUTRIAL_CAUTION", EVT_PACHI},
};

static char* l_resNameList[12] = {
    "",
    "Taro",
    "Taro_TW",
    "Taro0",
    "Taro1",
    "Taro2",
    "Taro3",
    "Taro4",
    "Taro5",
    "TaroB",
    "Len1",
    "evt_pachi",
};

static s8 l_loadResPtrn0[7] = {
    TARO, TARO0, TARO1, TARO2, TARO3, EVT_PACHI, -1,
};

static s8 l_loadResPtrn4[8] = {
    TARO, TARO_TW, TARO0, TARO1, TARO2, TARO3, TARO4, -1,
};

static s8* l_loadResPtrnList[2] = {
    l_loadResPtrn0,
    l_loadResPtrn4,
};

static daNpcT_faceMotionAnmData_c l_faceMotionAnmData[30] = {
    {-1, J3DFrameCtrl::EMode_NONE, NONE, BTP_TARO_F_NORMAL, J3DFrameCtrl::EMode_LOOP, TARO, 1},
    {BCK_TARO_F_NORMAL, J3DFrameCtrl::EMode_NONE, TARO0,BTP_TARO_F_NORMAL, J3DFrameCtrl::EMode_LOOP, TARO, 1},
    {BCK_TARO_F_SAD, J3DFrameCtrl::EMode_NONE, TARO1, BTP_TARO_F_SAD, J3DFrameCtrl::EMode_NONE, TARO1, 0},
    {BCK_TARO_F_SMILE, J3DFrameCtrl::EMode_NONE, TARO3, BTP_TARO_F_SMILE, J3DFrameCtrl::EMode_NONE, TARO3, 0},
    {BCK_TARO_F_SURPRISE, J3DFrameCtrl::EMode_NONE, TARO1, BTP_TARO_F_SURPRISE, J3DFrameCtrl::EMode_NONE, TARO1, 0},
    {BCK_TARO_F_PANIC, J3DFrameCtrl::EMode_NONE, TARO1, BTP_TARO_F_PANIC, J3DFrameCtrl::EMode_NONE, TARO1, 0},
    {BCK_TARO_F_ANGER, J3DFrameCtrl::EMode_NONE, TARO3, BTP_TARO_F_ANGER, J3DFrameCtrl::EMode_NONE, TARO3, 0},
    {BCK_TARO_F_SHOUT, J3DFrameCtrl::EMode_NONE, TARO1, BTP_TARO_F_SHOUT, J3DFrameCtrl::EMode_NONE, TARO1, 0},
    {BCK_TARO_FH_SAD, J3DFrameCtrl::EMode_LOOP, TARO1, BTP_TARO_FH_SAD, J3DFrameCtrl::EMode_LOOP, TARO1, 0},
    {BCK_TARO_FH_SMILE, J3DFrameCtrl::EMode_LOOP, TARO3, BTP_TARO_FH_SMILE, J3DFrameCtrl::EMode_LOOP, TARO3, 0},
    {BCK_TARO_FH_SURPRISE, J3DFrameCtrl::EMode_LOOP, TARO1, BTP_TARO_FH_SURPRISE, J3DFrameCtrl::EMode_LOOP, TARO1, 0},
    {BCK_TARO_FH_PANIC, J3DFrameCtrl::EMode_LOOP, TARO1, BTP_TARO_FH_PANIC, J3DFrameCtrl::EMode_LOOP, TARO1, 0},
    {BCK_TARO_FH_ANGER, J3DFrameCtrl::EMode_LOOP, TARO3, BTP_TARO_F_NORMAL, J3DFrameCtrl::EMode_LOOP, TARO, 1},
    {BCK_TARO_F_CLOSE, J3DFrameCtrl::EMode_NONE, TARO0, BTP_TARO_F_CLOSE, J3DFrameCtrl::EMode_NONE, TARO0, 0},
    {BCK_TARO_F_S_GET, J3DFrameCtrl::EMode_NONE, TARO1, BTP_TARO_F_S_GET, J3DFrameCtrl::EMode_NONE, TARO1, 0},
    {BCK_TARO_F_CALL, J3DFrameCtrl::EMode_NONE, TARO2, BTP_TARO_F_CALL, J3DFrameCtrl::EMode_NONE, TARO2, 0},
    {BCK_TARO_F_WAVE, J3DFrameCtrl::EMode_NONE, TARO2, BTP_TARO_F_WAVE, J3DFrameCtrl::EMode_NONE, TARO2, 0},
    {BCK_TARO_F_WATCH, J3DFrameCtrl::EMode_NONE, TARO2, BTP_TARO_F_WATCH, J3DFrameCtrl::EMode_NONE, TARO2, 0},
    {BCK_TARO_F_MARVELOUS, J3DFrameCtrl::EMode_NONE, TARO2, BTP_TARO_F_MARVELOUS, J3DFrameCtrl::EMode_NONE, TARO2, 0},
    {BCK_TARO_F_POINT, J3DFrameCtrl::EMode_NONE, TARO2, BTP_TARO_F_POINT, J3DFrameCtrl::EMode_NONE, TARO2, 0},
    {BCK_TARO_F_HI, J3DFrameCtrl::EMode_NONE, TARO2, BTP_TARO_F_HI, J3DFrameCtrl::EMode_NONE, TARO2, 0},
    {BCK_TARO_F_SURPRISEB, J3DFrameCtrl::EMode_NONE, TARO4, BTP_TARO_F_SURPRISEB, J3DFrameCtrl::EMode_NONE, TARO4, 0},
    {BCK_TARO_FH_SURPRISEB, J3DFrameCtrl::EMode_LOOP, TARO4, BTP_TARO_FH_SURPRISEB, J3DFrameCtrl::EMode_LOOP, TARO4, 0},
    {BCK_TARO_F_DEMO_WAIT, J3DFrameCtrl::EMode_LOOP, TARO_TW, BTP_TARO_F_DEMO_WAIT, J3DFrameCtrl::EMode_LOOP, TARO_TW, 0},
    {BCK_TAROB_F_SCARED, J3DFrameCtrl::EMode_NONE, TARO5, BTP_TAROB_F_SCARED, J3DFrameCtrl::EMode_NONE, TARO5, 0},
    {BCK_TAROB_FH_SCARED, J3DFrameCtrl::EMode_LOOP, TARO5, BTP_TAROB_FH_SCARED, J3DFrameCtrl::EMode_LOOP, TARO5, 0},
    {BCK_TAROB_F_MISERABLE, J3DFrameCtrl::EMode_NONE, TARO5, BTP_TAROB_F_MISERABLE, J3DFrameCtrl::EMode_NONE, TARO5, 0},
    {BCK_TAROB_FH_MISERABLE, J3DFrameCtrl::EMode_LOOP, TARO5, BTP_TAROB_FH_MISERABLE, J3DFrameCtrl::EMode_LOOP, TARO5, 0},
    {BCK_TAROB_F_SAVED, J3DFrameCtrl::EMode_NONE, TARO5, BTP_TAROB_F_SAVED, J3DFrameCtrl::EMode_NONE, TARO5, 0},
    {BCK_TARO_F_CRY, J3DFrameCtrl::EMode_LOOP, TARO5, BTP_TARO_F_CRY, J3DFrameCtrl::EMode_LOOP, TARO5, 0},
};

static daNpcT_motionAnmData_c l_motionAnmData[41] = {
    {BCK_TARO_WAIT_A, J3DFrameCtrl::EMode_LOOP, TARO0,BTK_TARO, J3DFrameCtrl::EMode_NONE, TARO0, 1, 0},
    {BCK_TARO_WAIT_B, J3DFrameCtrl::EMode_LOOP, TARO1, BTK_TARO, J3DFrameCtrl::EMode_NONE, TARO0, 1, 0},
    {BCK_TARO_WAIT_C, J3DFrameCtrl::EMode_LOOP, TARO3, BTK_TARO, J3DFrameCtrl::EMode_NONE, TARO0, 1, 0},
    {BCK_TARO_WAIT_D, J3DFrameCtrl::EMode_LOOP, TARO1, BTK_TARO, J3DFrameCtrl::EMode_NONE, TARO0, 1, 0},
    {BCK_TARO_WAIT_E, J3DFrameCtrl::EMode_LOOP, TARO1, BTK_TARO, J3DFrameCtrl::EMode_NONE, TARO0, 1, 0},
    {BCK_TARO_WAIT_F, J3DFrameCtrl::EMode_LOOP, TARO1, BTK_TARO, J3DFrameCtrl::EMode_NONE, TARO0, 1, 0},
    {BCK_TARO_TO_WAIT_C, J3DFrameCtrl::EMode_NONE, TARO3, BTK_TARO, J3DFrameCtrl::EMode_NONE, TARO0, 1, 0},
    {-1, J3DFrameCtrl::EMode_NONE, NONE, -1, J3DFrameCtrl::EMode_NONE, NONE, 0, 0},
    {BCK_TARO_RUN, J3DFrameCtrl::EMode_LOOP, TARO1, BTK_TARO, J3DFrameCtrl::EMode_NONE, TARO0, 1, 0},
    {BCK_TAROB_SAD_WALK, J3DFrameCtrl::EMode_LOOP, TARO5, BTK_TARO, J3DFrameCtrl::EMode_NONE, TARO0, 1, 0},
    {BCK_TARO_S_RUN, J3DFrameCtrl::EMode_LOOP, TARO1, BTK_TARO, J3DFrameCtrl::EMode_NONE, TARO0, 1, 0},
    {BCK_TARO_TALK_A, J3DFrameCtrl::EMode_NONE, TARO1, BTK_TARO, J3DFrameCtrl::EMode_NONE, TARO0, 1, 0},
    {BCK_TARO_TALK_B, J3DFrameCtrl::EMode_LOOP, TARO1, BTK_TARO, J3DFrameCtrl::EMode_NONE, TARO0, 1, 0},
    {BCK_TARO_VICTORY_A, J3DFrameCtrl::EMode_NONE, TARO3, BTK_TARO, J3DFrameCtrl::EMode_NONE, TARO0, 1, 0},
    {BCK_TARO_CHOP_A, J3DFrameCtrl::EMode_NONE, TARO1, BTK_TARO, J3DFrameCtrl::EMode_NONE, TARO0, 1, 0},
    {BCK_TARO_CHOP_B, J3DFrameCtrl::EMode_NONE, TARO1, BTK_TARO, J3DFrameCtrl::EMode_NONE, TARO0, 1, 0},
    {BCK_TARO_DEFENSE_A, J3DFrameCtrl::EMode_NONE, TARO0, BTK_TARO, J3DFrameCtrl::EMode_NONE, TARO0, 1, 0},
    {BCK_TARO_S_GET, J3DFrameCtrl::EMode_NONE, TARO1, BTK_TARO_S_GET, J3DFrameCtrl::EMode_NONE, TARO1, 0, 0},
    {BCK_TARO_S_WAIT, J3DFrameCtrl::EMode_LOOP, TARO1, BTK_TARO, J3DFrameCtrl::EMode_NONE, TARO0, 1, 0},
    {BCK_TARO_S_CHOP_A, J3DFrameCtrl::EMode_LOOP, TARO1, BTK_TARO, J3DFrameCtrl::EMode_NONE, TARO0, 1, 0},
    {BCK_TARO_S_CHOP_B, J3DFrameCtrl::EMode_LOOP, TARO1, BTK_TARO, J3DFrameCtrl::EMode_NONE, TARO0, 1, 0},
    {BCK_TARO_CALL, J3DFrameCtrl::EMode_NONE, TARO2, BTK_TARO, J3DFrameCtrl::EMode_NONE, TARO0, 1, 0},
    {BCK_TARO_WAVE, J3DFrameCtrl::EMode_NONE, TARO2, BTK_TARO, J3DFrameCtrl::EMode_NONE, TARO0, 1, 0},
    {BCK_TARO_WATCH, J3DFrameCtrl::EMode_NONE, TARO2, BTK_TARO, J3DFrameCtrl::EMode_NONE, TARO0, 1, 0},
    {BCK_TARO_WATCHING, J3DFrameCtrl::EMode_LOOP, TARO2, BTK_TARO, J3DFrameCtrl::EMode_NONE, TARO0, 1, 0},
    {BCK_TARO_MARVELOUS, J3DFrameCtrl::EMode_NONE, TARO2, BTK_TARO, J3DFrameCtrl::EMode_NONE, TARO0, 1, 0},
    {BCK_TARO_POINT, J3DFrameCtrl::EMode_NONE, TARO2, BTK_TARO, J3DFrameCtrl::EMode_NONE, TARO0, 1, 0},
    {BCK_TARO_LOOKBACK, J3DFrameCtrl::EMode_NONE, TARO2, BTK_TARO_LOOKBACK, J3DFrameCtrl::EMode_NONE, TARO2, 0, 0},
    {BCK_TARO_HI, J3DFrameCtrl::EMode_NONE, TARO2, BTK_TARO, J3DFrameCtrl::EMode_NONE, TARO0, 1, 0},
    {BCK_TARO_HI_WAIT, J3DFrameCtrl::EMode_LOOP, TARO2, BTK_TARO, J3DFrameCtrl::EMode_NONE, TARO0, 1, 0},
    {BCK_TARO_TO_WAIT_C2, J3DFrameCtrl::EMode_NONE, TARO2, BTK_TARO, J3DFrameCtrl::EMode_NONE, TARO0, 1, 0},
    {BCK_TARO_SURPRISEB, J3DFrameCtrl::EMode_LOOP, TARO4, BTK_TARO, J3DFrameCtrl::EMode_NONE, TARO0, 1, 0},
    {BCK_TARO_SURPRISEB_TALK, J3DFrameCtrl::EMode_NONE, TARO4, BTK_TARO, J3DFrameCtrl::EMode_NONE, TARO0, 1, 0},
    {BCK_TARO_DEMO_WAIT, J3DFrameCtrl::EMode_LOOP, TARO_TW, BTK_TARO_DEMO_WAIT, J3DFrameCtrl::EMode_LOOP, TARO_TW, 0, 0},
    {BCK_TARO_STEP, J3DFrameCtrl::EMode_NONE, TARO0, BTK_TARO, J3DFrameCtrl::EMode_NONE, TARO0, 1, 0},
    {BCK_TAROB_SCARED, J3DFrameCtrl::EMode_LOOP, TARO5, BTK_TARO, J3DFrameCtrl::EMode_NONE, TARO0, 1, 0},
    {BCK_TAROB_SAD_WAIT, J3DFrameCtrl::EMode_LOOP, TARO5, BTK_TARO, J3DFrameCtrl::EMode_NONE, TARO0, 1, 0},
    {BCK_TAROB_SAD_TALK_A, J3DFrameCtrl::EMode_NONE, TARO5, BTK_TARO, J3DFrameCtrl::EMode_NONE, TARO0, 1, 0},
    {BCK_TAROB_SAD_UTUMUKI, J3DFrameCtrl::EMode_LOOP, TARO5, BTK_TARO, J3DFrameCtrl::EMode_NONE, TARO0, 1, 0},
    {BCK_TARO_CRY, J3DFrameCtrl::EMode_LOOP, TARO5, BTK_TARO_CRY, J3DFrameCtrl::EMode_LOOP, TARO5, 0, 0},
    {BCK_TAROB_SAVED, J3DFrameCtrl::EMode_NONE, TARO5, BTK_TARO, J3DFrameCtrl::EMode_NONE, TARO0, 1, 0},
};

static daNpcT_MotionSeqMngr_c::sequenceStepData_c l_faceMotionSequenceData[128] = {
    {FACE_MOT_ANM_NORMAL, -1, 1}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {FACE_MOT_ANM_SAD, -1, 1}, {8, -1, 0}, {-1, 0, 0}, {-1, 0, 0},
    {FACE_MOT_ANM_SMILE, -1, 1}, {9, -1, 0}, {-1, 0, 0}, {-1, 0, 0},
    {FACE_MOT_ANM_SURPRISE, -1, 1}, {10, -1, 0}, {-1, 0, 0}, {-1, 0, 0},
    {FACE_MOT_ANM_PANIC, -1, 1}, {11, -1, 0}, {-1, 0, 0}, {-1, 0, 0},
    {FACE_MOT_ANM_ANGER, -1, 1}, {12, -1, 0}, {-1, 0, 0}, {-1, 0, 0},
    {FACE_MOT_ANM_CALL, -1, 1}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {FACE_MOT_ANM_WAVE, -1, 1}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {FACE_MOT_ANM_MARVELOUS, -1, 1}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {FACE_MOT_ANM_S_GET, -1, 1}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {FACE_MOT_ANM_POINT, -1, 1}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {FACE_MOT_ANM_HI, -1, 1}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {FACE_MOT_ANM_SURPRISEB, -1, 1}, {22, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {FACE_MOT_ANM_SCARED, -1, 1}, {25, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {FACE_MOT_ANM_MISERABLE, -1, 1}, {27, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {FACE_MOT_ANM_SMILE, -1, 1}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {FACE_MOT_ANM_SHOUT, -1, 1}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {FACE_MOT_ANM_CLOSE, -1, 1}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {FACE_MOT_ANM_H_SAD, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {FACE_MOT_ANM_H_SMILE, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {FACE_MOT_ANM_H_SURPRISE, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {FACE_MOT_ANM_H_PANIC, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {FACE_MOT_ANM_H_ANGER, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {FACE_MOT_ANM_WATCH, -1, 1}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {FACE_MOT_ANM_H_SURPRISEB, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {FACE_MOT_ANM_DEMO_WAIT, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {FACE_MOT_ANM_H_SCARED, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {FACE_MOT_ANM_H_MISERABLE, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {FACE_MOT_ANM_SAVED, -1, 1}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {FACE_MOT_ANM_SCARED, -1, 1}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {FACE_MOT_ANM_CRY, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {FACE_MOT_ANM_NONE, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
};

static daNpcT_MotionSeqMngr_c::sequenceStepData_c l_motionSequenceData[164] = {
    {MOT_ANM_WAIT_A, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {MOT_ANM_CALL, -1, 1}, {0, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {MOT_ANM_WAVE, -1, 1}, {0, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {MOT_ANM_WAIT_B, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {MOT_ANM_S_GET, -1, 1}, {0, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {MOT_ANM_WATCHING, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {MOT_ANM_MARVELOUS, -1, 1}, {0, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {MOT_ANM_POINT, -1, 1}, {0, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {MOT_ANM_WAIT_C, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {MOT_ANM_TO_WAIT_C, 0, 1}, {2, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {MOT_ANM_LOOKBACK, -1, 1}, {0, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {MOT_ANM_WAIT_D, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {MOT_ANM_HI, -1, 1}, {29, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {MOT_ANM_TALK_A, -1, 1}, {0, -1, 0}, {-1, 0, 0}, {-1, 0, 0},
    {MOT_ANM_TALK_B, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {MOT_ANM_VICTORY_A, -1, 1}, {0, -1, 0}, {-1, 0, 0}, {-1, 0, 0},
    {MOT_ANM_HI_WAIT, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {MOT_ANM_TO_WAIT_C2, 10, 1}, {2, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {MOT_ANM_SURPRISEB_TALK, -1, 1}, {31, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {MOT_ANM_SURPRISEB, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {MOT_ANM_WAIT_F, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {MOT_ANM_SCARED, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {MOT_ANM_SAD_WAIT, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {MOT_ANM_SAD_TALK_A, -1, 1}, {36, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {MOT_ANM_SAD_UTUMUKI, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {MOT_ANM_S_WAIT, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {MOT_ANM_CRY, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {MOT_ANM_CHOP_A, -1, 1}, {0, -1, 0}, {-1, 0, 0}, {-1, 0, 0},
    {MOT_ANM_CHOP_B, -1, 1}, {0, -1, 0}, {-1, 0, 0}, {-1, 0, 0},
    {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {MOT_ANM_S_CHOP_A, -1, 1}, {18, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {MOT_ANM_S_CHOP_B, -1, 1}, {18, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {MOT_ANM_DEFENSE_A, -1, 1}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {MOT_ANM_WATCH, -1, 1}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {MOT_ANM_DEMO_WAIT, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {MOT_ANM_STEP, 4, 1}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {MOT_ANM_RUN, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {MOT_ANM_S_RUN, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {MOT_ANM_SAD_WALK, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {MOT_ANM_SAVED, -1, 1}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
};

char* daNpc_Pachi_Taro_c::mCutNameList[11] = {
    "",
    "TUTRIAL_BEGIN",
    "TUTRIAL_BEGIN_SKIP",
    "TUTRIAL_CLEAR",
    "TUTRIAL_TALK",
    "TUTRIAL_TALK2",
    "HIT_KAKASI_BODY",
    "TUTRIAL_SELECT_GIVEUP",
    "TUTRIAL_CONTINUE",
    "TUTRIAL_GIVEUP",
    "TUTRIAL_CAUTION",
};

daNpc_Pachi_Taro_c::cutFunc daNpc_Pachi_Taro_c::mCutList[11] = {
    NULL,
    &daNpc_Pachi_Taro_c::cutTutrialBegin,
    &daNpc_Pachi_Taro_c::cutTutrialBegin_Skip,
    &daNpc_Pachi_Taro_c::cutTutrialClear,
    &daNpc_Pachi_Taro_c::cutTalk,
    &daNpc_Pachi_Taro_c::cutTalk2,
    &daNpc_Pachi_Taro_c::cut1stHit_KakasiBody,
    &daNpc_Pachi_Taro_c::cutTutrialSelectGiveUp,
    &daNpc_Pachi_Taro_c::cutTutrialContinue,
    &daNpc_Pachi_Taro_c::cutTutrialGiveUp,
    &daNpc_Pachi_Taro_c::cutTutrialCaution,
};

static NPC_PACHI_TARO_HIO_CLASS l_HIO;

daNpc_Pachi_Taro_c::~daNpc_Pachi_Taro_c() {
    OS_REPORT("|%06d:%x|daNpc_Pachi_Taro_c -> デストラクト\n", g_Counter.mCounter0, this);

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

daNpc_Pachi_Taro_HIOParam const daNpc_Pachi_Taro_Param_c::m = {
    140.0f,
    -3.0f,
    1.0f,
    400.0f,
    255.0f,
    120.0f,
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
    -10.0f,
    15.0f,
    30.0f,
    10.0f,
    55.0f,
};

cPhs__Step daNpc_Pachi_Taro_c::create() {
    daNpcT_ct(this, daNpc_Pachi_Taro_c, l_faceMotionAnmData, l_motionAnmData,
                       l_faceMotionSequenceData, 4, l_motionSequenceData, 4,
                       l_evtList, l_resNameList);

    mType = getType();
    mFlowNodeNo = getFlowNodeNo();
    mTwilight = false;

    cPhs__Step phase = (cPhs__Step)loadRes(l_loadResPtrnList[mType], (const char**)l_resNameList);
    if (phase == cPhs_COMPLEATE_e) {
        if (!fopAcM_entrySolidHeap(this, createHeapCallBack, 0x48E0)) {
            return cPhs_ERROR_e;
        }

        OS_REPORT("\t(%s:%d) flowNo:%d, PathID:%02x, BitSW:%02x<%08x> ", fopAcM_getProcNameString(this), mType, mFlowNodeNo, getPathID(), getBitSW(), fopAcM_GetParam(this));

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
        mpHIO->entryHIO("パチチュー：タロ");
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

int daNpc_Pachi_Taro_c::CreateHeap() {
    J3DModelData* modelData = NULL;
    J3DModel* model = NULL;
    
    int bmdIdx = mTwilight == true ? TARO0 : NONE;
    modelData = static_cast<J3DModelData*>(dComIfG_getObjectRes(l_resNameList[l_bmdData[bmdIdx][1]], l_bmdData[bmdIdx][0]));
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

    static int const bmdTypeList[2] = {
        1, 2,
    };
    
    for (int i = 0; i < 2; i++) {
        if (l_bmdData[bmdTypeList[i]][0] >= 0) {
            modelData = (J3DModelData*)dComIfG_getObjectRes(l_resNameList[l_bmdData[bmdTypeList[i]][1]], l_bmdData[bmdTypeList[i]][0]);
        } else {
            modelData = NULL;
        }

        if (modelData != NULL) {
            mpModels[i] = mDoExt_J3DModel__create(modelData, J3DMdlFlag_DifferedDLBuffer, 0x11000084);
        } else {
            mpModels[i] = NULL;
        }
    }

    if (mTwilight) {
        if (setFaceMotionAnm(FACE_MOT_ANM_DEMO_WAIT, false) && setMotionAnm(MOT_ANM_DEMO_WAIT, 0.0f, FALSE)) {
            return 1;
        }

        return 0;
    }
    
    if (setFaceMotionAnm(FACE_MOT_ANM_NORMAL, false) && setMotionAnm(MOT_ANM_WAIT_A, 0.0f, FALSE)) {
        return 1;
    }

    return 0;
}

int daNpc_Pachi_Taro_c::Delete() {
    OS_REPORT("|%06d:%x|daNpc_Pachi_Taro_c -> Delete\n", g_Counter.mCounter0, this);
    fpc_ProcID id = fopAcM_GetID(this);
    this->~daNpc_Pachi_Taro_c();
    return 1;
}

int daNpc_Pachi_Taro_c::Execute() {
    return execute();
}

int daNpc_Pachi_Taro_c::Draw() {
    if (mpMatAnm[0] != NULL) {
        J3DModelData* modelData = mpMorf[0]->getModel()->getModelData();
        modelData->getMaterialNodePointer(getEyeballMaterialNo())->setMaterialAnm(mpMatAnm[0]);
    }

    return draw(
#if DEBUG
        chkAction(&daNpc_Pachi_Taro_c::test),
#else
        FALSE,
#endif
        FALSE,
        mRealShadowSize, NULL, 100.0f, FALSE, FALSE, FALSE
    );
}

int daNpc_Pachi_Taro_c::createHeapCallBack(fopAc_ac_c* i_this) {
    daNpc_Pachi_Taro_c* actor = (daNpc_Pachi_Taro_c*)i_this;
    return actor->CreateHeap();
}

int daNpc_Pachi_Taro_c::ctrlJointCallBack(J3DJoint* i_joint, int param_2) {
    if (param_2 == 0) {
        J3DModel* model = j3dSys.getModel();
        daNpc_Pachi_Taro_c* actor = (daNpc_Pachi_Taro_c*)model->getUserArea();
        if (actor != 0) {
            actor->ctrlJoint(i_joint, model);
        }
    }

    return 1;
}

u8 daNpc_Pachi_Taro_c::getType() {
    u8 param = fopAcM_GetParam(this) & 0xFF;
    param = 0;

    switch (param) {
        case 0:
            return TYPE_0;

        default:
            return 1;
    }
}

BOOL daNpc_Pachi_Taro_c::isDelete() {
    switch (mType) {
        case TYPE_0:
            return FALSE;

        default:
            return FALSE;
    }
}

void daNpc_Pachi_Taro_c::reset() {
    int size = (u8*)&field_0x1008 - (u8*)&mNextAction;

    if (mpMatAnm[0] != NULL) {
        mpMatAnm[0]->initialize();
    }

    initialize();

    for (int i = 0; i < 6; i++) {
        mActorMngrs[i].initialize();
    }

    memset(&mNextAction, 0, size);

    switch (mType) {
        case TYPE_0:
            field_0xfd4 = 2;
            field_0xfde = 0;
            field_0xfdf = 0;
            field_0xfe0 = 0;
            field_0xfe2 = 1;
            field_0xfe3 = 1;
            mMesPat = 0;
            dComIfGs_setTmpReg(0xF4FF, 0);
            mPrevEvtNo = EVT_NONE;
            field_0x1004 = 0;
            break;
    }

    setAngle(home.angle.y);
}

void daNpc_Pachi_Taro_c::afterJntAnm(int i_joint) {
    if (i_joint == JNT_BACKBONE1) {
        mDoMtx_stack_c::YrotM(-mStagger.getAngleZ(1));
        mDoMtx_stack_c::ZrotM(mStagger.getAngleX(1));
    } else if (i_joint == JNT_HEAD) {
        mDoMtx_stack_c::YrotM(-mStagger.getAngleZ(0));
        mDoMtx_stack_c::ZrotM(mStagger.getAngleX(0));
    }
}

void daNpc_Pachi_Taro_c::setParam() {
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

BOOL daNpc_Pachi_Taro_c::checkChangeEvt() {
    switch (mType) {
        default:
            break;
    }
    
    return FALSE;
}

void daNpc_Pachi_Taro_c::setAfterTalkMotion() {
    int idx = FACE_MOT_NONE;

    switch (mFaceMotionSeqMngr.getNo()) {
        case FACE_MOT_SAD:
            idx = FACE_MOT_H_SAD;
            break;

        case FACE_MOT_SMILE:
            idx = FACE_MOT_H_SMILE;
            break;

        case FACE_MOT_SURPRISE:
            idx = FACE_MOT_H_SURPRISE;
            break;

        case FACE_MOT_PANIC:
            idx = FACE_MOT_H_PANIC;
            break;

        case FACE_MOT_ANGER:
            idx = FACE_MOT_H_ANGER;
            break;

        case FACE_MOT_SURPRISEB:
            idx = FACE_MOT_H_SURPRISEB;
            break;
            
        case FACE_MOT_SCARED:
            idx = FACE_MOT_H_SCARED;
            break;

        case FACE_MOT_MISERABLE:
            idx = FACE_MOT_H_MISERABLE;
            break;
    }

    mFaceMotionSeqMngr.setNo(idx, -1.0f, FALSE, 0);
}

void daNpc_Pachi_Taro_c::srchActors() {
    switch (mType) {
        case TYPE_0:
            if (mActorMngrs[0].getActorP() == NULL) {
                mActorMngrs[0].entry(getNearestActorP(PROC_NPC_PACHI_MARO));
            }

            if (mActorMngrs[1].getActorP() == NULL) {
                mActorMngrs[1].entry(getNearestActorP(PROC_NPC_PACHI_BESU));
            }

            if (mActorMngrs[2].getActorP() == NULL) {
                mActorMngrs[2].entry(srchDistTag1());
            }

            if (mActorMngrs[3].getActorP() == NULL) {
                mActorMngrs[3].entry(srchDistTag2());
            }

            if (mActorMngrs[4].getActorP() == NULL) {
                mActorMngrs[4].entry(srchEscapeTag1());
            }

            if (mActorMngrs[5].getActorP() == NULL) {
                mActorMngrs[5].entry(srchEscapeTag2());
            }
            break;
    }
}

fopAc_ac_c* daNpc_Pachi_Taro_c::srchDistTag1() {
    return (fopAc_ac_c*)fpcM_Search(_srch_DistTag1_main, this);
}

void* daNpc_Pachi_Taro_c::_srch_DistTag1_main(void* i_actor, void* i_data) {
    daTagPati_c* actor = (daTagPati_c*)i_actor;
    
    if (!fopAcM_IsActor(actor)) {
        return NULL;
    } 
    
    if (fopAcM_GetName(actor) != PROC_TAG_PATI) {
        return NULL;
    } 
    
    if (!actor->isDistChkTag1()) {
        return NULL;
    }

    return actor;
}

fopAc_ac_c* daNpc_Pachi_Taro_c::srchDistTag2() {
    return (fopAc_ac_c*)fpcM_Search(_srch_DistTag2_main, this);
}

void* daNpc_Pachi_Taro_c::_srch_DistTag2_main(void* i_actor, void* i_data) {
    daTagPati_c* actor = (daTagPati_c*)i_actor;
    
    if (!fopAcM_IsActor(actor)) {
        return NULL;
    } 
    
    if (fopAcM_GetName(actor) != PROC_TAG_PATI) {
        return NULL;
    } 
    
    if (!actor->isDistChkTag2()) {
        return NULL;
    }

    return actor;
}

fopAc_ac_c* daNpc_Pachi_Taro_c::srchEscapeTag1() {
    return (fopAc_ac_c*)fpcM_Search(_srch_EscapeTag1_main, this);
}

void* daNpc_Pachi_Taro_c::_srch_EscapeTag1_main(void* i_actor, void* i_data) {
    daTagPati_c* actor = (daTagPati_c*)i_actor;
    
    if (!fopAcM_IsActor(actor)) {
        return NULL;
    } 
    
    if (fopAcM_GetName(actor) != PROC_TAG_PATI) {
        return NULL;
    } 
    
    if (!actor->isEscapeChkTag1()) {
        return NULL;
    }

    return actor;
}

fopAc_ac_c* daNpc_Pachi_Taro_c::srchEscapeTag2() {
    return (fopAc_ac_c*)fpcM_Search(_srch_EscapeTag2_main, this);
}

void* daNpc_Pachi_Taro_c::_srch_EscapeTag2_main(void* i_actor, void* i_data) {
    daTagPati_c* actor = (daTagPati_c*)i_actor;
    
    if (!fopAcM_IsActor(actor)) {
        return NULL;
    } 
    
    if (fopAcM_GetName(actor) != PROC_TAG_PATI) {
        return NULL;
    } 
    
    if (!actor->isEscapeChkTag2()) {
        return NULL;
    }

    return actor;
}

BOOL daNpc_Pachi_Taro_c::evtTalk() {
    if (chkAction(&daNpc_Pachi_Taro_c::talk)) {
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

        setAction(&daNpc_Pachi_Taro_c::talk);
    }

    return TRUE;
}

BOOL daNpc_Pachi_Taro_c::evtCutProc() {
    int staffId = dComIfGp_getEventManager().getMyStaffId("PA_Taro", this, -1);

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

void daNpc_Pachi_Taro_c::action() {
    fopAc_ac_c* actor_p = NULL;

    if (!mTwilight) {
        actor_p = hitChk(&mCyl, 0xFFFFFFFF);
    }

    if (actor_p != NULL) {
        mStagger.setParam(this, actor_p, mCurAngle.y);
        setDamage(0, 17, 33);
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

void daNpc_Pachi_Taro_c::beforeMove() {
    if (checkHide() || mNoDraw) {
        attention_info.flags = 0;
    }
}

void daNpc_Pachi_Taro_c::setAttnPos() {
    cXyz sp38(0.0f, 30.0f, 0.0f);

    BOOL bVar1 = 0;
    if (mMotionSeqMngr.getNo() == MOT_DEFENSE_A && !mMotionSeqMngr.checkEndSequence()) {
        bVar1 = TRUE;
    }
    
    mStagger.calc(bVar1);
    f32 fVar1 = cM_s2rad(mCurAngle.y - field_0xd7e.y);

    mJntAnm.setParam(this, mpMorf[0]->getModel(), &sp38, getBackboneJointNo(), getNeckJointNo(), getHeadJointNo(),
                     mpHIO->m.common.body_angleX_min, mpHIO->m.common.body_angleX_max,
                     mpHIO->m.common.body_angleY_min, mpHIO->m.common.body_angleY_max,
                     mpHIO->m.common.head_angleX_min, mpHIO->m.common.head_angleX_max,
                     mpHIO->m.common.head_angleY_min, mpHIO->m.common.head_angleY_max,
                     mpHIO->m.common.neck_rotation_ratio, 0.0f, NULL);
    mJntAnm.calcJntRad(0.2f, 1.0f, fVar1);
    setMtx();

    mDoMtx_stack_c::copy(mpMorf[0]->getModel()->getAnmMtx(getHeadJointNo()));
    mDoMtx_stack_c::multVec(&sp38, &eyePos);
    mJntAnm.setEyeAngleX(eyePos, 1.0f, 0);
    mJntAnm.setEyeAngleY(eyePos, mCurAngle.y, TRUE, 1.0f, 0);

    attention_info.position = current.pos;
    attention_info.position.y += mpHIO->m.common.attention_offset;
}

void daNpc_Pachi_Taro_c::setCollision() {
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

int daNpc_Pachi_Taro_c::drawDbgInfo() {
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

void daNpc_Pachi_Taro_c::drawOtherMdl() {
    J3DModel* model = mpMorf[0]->getModel();

    static int const jointNo[2] = {
        JNT_HANDR, JNT_HANDR,
    };

    for (int i = 0; i < 2; i++) {
        if (mpModels[i] != NULL && ((i == 0 && field_0x1005 != 0) || (i == 1 && field_0x1006 != 0))) {
            g_env_light.setLightTevColorType_MAJI(mpModels[i], &tevStr);
            mDoMtx_stack_c::copy(model->getAnmMtx(jointNo[i]));
            Mtx mtx;
            MTXCopy(mDoMtx_stack_c::get(), mtx);
            mpModels[i]->setBaseTRMtx(mtx);
            mDoExt_modelUpdateDL(mpModels[i]);
            dComIfGd_addRealShadow(mShadowKey, mpModels[i]);
        }
    }
}

BOOL daNpc_Pachi_Taro_c::selectAction() {
    mNextAction = NULL;

    #if DEBUG
    if (mpHIO->m.common.debug_mode_ON) {
        mNextAction = &daNpc_Pachi_Taro_c::test;
        return TRUE;
    }
    #endif

    switch (mType) {
        case TYPE_0:
            mNextAction = &daNpc_Pachi_Taro_c::wait;
            break;

        default:
            mNextAction = &daNpc_Pachi_Taro_c::wait;
            break;
    }

    return TRUE;
}

BOOL daNpc_Pachi_Taro_c::chkAction(actionFunc action) {
    return mAction == action;
}

BOOL daNpc_Pachi_Taro_c::setAction(actionFunc action) {
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

int daNpc_Pachi_Taro_c::wait(void* param_1) {
    int unused = 0;
    
    switch (mMode) {
        case MODE_ENTER:
        case MODE_INIT:
            if (!mStagger.checkStagger()) {
                mMesPat = -1;
                mFaceMotionSeqMngr.setNo(FACE_MOT_NONE, -1.0f, FALSE, 0);
                mMotionSeqMngr.setNo(MOT_WAIT_A, -1.0f, FALSE, 0);
                mMode = MODE_RUN;
            }
            // fallthrough
        case MODE_RUN:
            if (!mStagger.checkStagger()) {
                if (mPlayerActorMngr.getActorP() != NULL) {
                    if (field_0xfe3 != 0) {
                        mJntAnm.lookPlayer(0);

                        if (field_0xfe2 == 0 || !chkActorInSight(mPlayerActorMngr.getActorP(), mAttnFovY, mCurAngle.y)) {
                            mJntAnm.lookNone(0);
                        }
                    } else {
                        field_0xfe3 = 1;
                    }

                    if (!srchPlayerActor() && home.angle.y == mCurAngle.y) {
                        mMode = MODE_INIT;
                    }
                } else {
                    mJntAnm.lookNone(0);

                    if (home.angle.y != mCurAngle.y) {
                        if (field_0xe34 != 0) {
                            if (field_0xfe2 != 0 && step(home.angle.y, 31, 36, 15, 0)) {
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

                if (field_0x1004 == 0) {
                    mEvtNo = EVT_TUTRIAL_BEGIN;
                    mPrevEvtNo = mEvtNo;
                    dComIfGp_getEvent()->setSkipProc(this, dEv_defaultSkipProc, 0);
                } else if (field_0x1004 == 1) {
                    watch_tutrial();
                }
            }
            break;

        case MODE_EXIT:
            break;
    }

    return 1;
}

void daNpc_Pachi_Taro_c::setMesPat() {
    mMesPat = 0;

    if (fopAcM_isSwitch(this, getBitSW())) {
        if (!fopAcM_isSwitch(this, getBitSW2())) {
            // dSv_event_flag_c::F_0613 - Ordon Village - Slingshot Tutorial - Spoke before getting 2 scarecrow heards
            if (daNpcT_chkEvtBit(613)) {
                mMesPat = 3;
            } else {
                mMesPat = 2;
            }
        } else {
            mMesPat = 4;
        }
    } else if (
        // dSv_event_flag_c::F_0612 - Ordon Village - Pachinco Tutorial - Spoke before hitting target
        daNpcT_chkEvtBit(612)
    ) {
        mMesPat = 1;
    } else {
        mMesPat = 0;
    }
}

BOOL daNpc_Pachi_Taro_c::_turn_to_link(s16 i_step) {
    daPy_py_c* player = daPy_getPlayerActorClass();
    cXyz pos(*fopAcM_GetPosition_p(player));
    return _turn_pos(pos, i_step);
}

BOOL daNpc_Pachi_Taro_c::_turn_pos(cXyz const& i_pos, s16 i_step) {
    BOOL rv = cLib_chaseAngleS(&mCurAngle.y, cLib_targetAngleY(&current.pos, &i_pos), i_step);
    current.angle.y = mCurAngle.y;
    shape_angle.y = mCurAngle.y;
    return rv;
}

BOOL daNpc_Pachi_Taro_c::_turn_pos(cXyz const& i_pos) {
    s16 angleY = cLib_targetAngleY(&current.pos, &i_pos);
    mCurAngle.y = angleY;
    current.angle.y = angleY;
    shape_angle.y = angleY;

    return TRUE;
}

void daNpc_Pachi_Taro_c::talk_cancel() {
    daNpc_Pachi_Maro_c* maro_p = (daNpc_Pachi_Maro_c*)mActorMngrs[0].getActorP();
    if (maro_p != NULL) {
        maro_p->talkOff();
    }
    
    daNpc_Pachi_Besu_c* besu_p = (daNpc_Pachi_Besu_c*)mActorMngrs[1].getActorP();
    if (besu_p != NULL) {
        besu_p->talkOff();
    }
}

void daNpc_Pachi_Taro_c::watch_tutrial() {
    u16 evtNo = 0;
    evtNo = chkDistTag();
    if (evtNo) {
        mEvtNo = evtNo;
        mPrevEvtNo = mEvtNo;
    } else {
        evtNo = chkEscapeTag();
        if (evtNo) {
            mEvtNo = evtNo;
            mPrevEvtNo = mEvtNo;
        } else {
            evtNo = chkTarget();
            if (evtNo) {
                mEvtNo = evtNo;
                mPrevEvtNo = mEvtNo;
                return;
            }
        }
    }
}

u16 daNpc_Pachi_Taro_c::chkDistTag() {
    fopAc_ac_c* actor_p = NULL;
    actor_p = mActorMngrs[2].getActorP();
    if (actor_p != NULL && ((daTagPati_c*)actor_p)->is_CoHit()) {
        OS_REPORT("---- tag hit1!!\n");
        return EVT_TUTRIAL_CAUTION;
    }

    actor_p = mActorMngrs[3].getActorP();
    if (actor_p != NULL && ((daTagPati_c*)actor_p)->is_CoHit()) {
        OS_REPORT("---- tag hit2!!\n");
        return EVT_TUTRIAL_CAUTION;
    }

    return EVT_NONE;
}

u16 daNpc_Pachi_Taro_c::chkEscapeTag() {
    fopAc_ac_c* actor_p = NULL;
    actor_p = mActorMngrs[4].getActorP();
    if (actor_p != NULL && ((daTagPati_c*)actor_p)->is_CoHit()) {
        cXyz tagPos(-428.2985f, 800.0f, -1607.351f);
        cXyz lookPos(640.3426f, 900.0f, -1783.4485f);
        allTagPosSet(tagPos);
        allLookPosSet(lookPos);
        return EVT_TUTRIAL_SELECT_GIVEUP;
    }
    
    actor_p = mActorMngrs[5].getActorP();
    if (actor_p != NULL && ((daTagPati_c*)actor_p)->is_CoHit()) {
        cXyz tagPos(780.17f, 800.0f, -2589.7766f);
        cXyz lookPos(781.49524f, 900.0f, -2103.6f);
        allTagPosSet(tagPos);
        allLookPosSet(lookPos);
        return EVT_TUTRIAL_SELECT_GIVEUP;
    }

    return EVT_NONE;
}

u16 daNpc_Pachi_Taro_c::chkTarget() {
    s8 sVar1 = -1;
    u8 tmpReg = dComIfGs_getTmpReg(0xF3FF);

    if (tmpReg != 0) {
        if (tmpReg & 1) {
            if (tmpReg & 128) {
                sVar1 = 1;
            } else {
                sVar1 = 2;
            }

            field_0xfd4--;
            if (field_0xfd4 == 0) {
                OS_REPORT("-------- KAKASI W BREAK!!\n");
                fopAcM_onSwitch(this, getBitSW2());
            }
        }

        if (tmpReg & 2) {
            sVar1 = 0;
        }

        if (tmpReg & 4) {
            if (tmpReg & 128) {
                sVar1 = 1;
            } else {
                sVar1 = 2;
            }
        }
    }

    dComIfGs_setTmpReg(0xF3FF, 0);

    u16 rv = EVT_NONE;

    if (field_0xfdf == 0) {
        switch (sVar1) {
            case 0:
                if (field_0xfe0 == 0) {
                    field_0xfe0 = 1;
                    rv = EVT_HIT_KAKASI_BODY;
                    mHitMsgIdx = sVar1;
                }
                break;

            case 1:
            case 2:
                field_0xfdf = 1;
                rv = EVT_HIT_KAKASI_BODY;
                mHitMsgIdx = sVar1;
                break;

            default:
                field_0xfdf = 0;
                break;
        }
    }

    field_0xfde = dComIfGs_getTmpReg(0xF4FF);
    if (field_0xfde == 7) {
        rv = EVT_TUTRIAL_CLEAR;
    } else if (field_0xfde != 0) {
        fopAcM_onSwitch(this, getBitSW());
    }

    return rv;
}

void daNpc_Pachi_Taro_c::clr_tagHit() {
    fopAc_ac_c* actor_p = NULL;
    for (int i = 0; i < 4; i++) {
        actor_p = mActorMngrs[i + 2].getActorP();
        if (actor_p != NULL) {
            ((daTagPati_c*)actor_p)->clr_CoHit();
        }
    }
}

void daNpc_Pachi_Taro_c::allTagPosSet(cXyz const& i_pos) {
    fopAc_ac_c* pachiMaroP = mActorMngrs[0].getActorP();
    fopAc_ac_c* pachBesuP = mActorMngrs[1].getActorP();
    
    setTagPos(i_pos);

    if (pachiMaroP != NULL) {
        ((daNpc_Pachi_Maro_c*)pachiMaroP)->setTagPos(i_pos);
    }

    if (pachBesuP != NULL) {
        ((daNpc_Pachi_Besu_c*)pachBesuP)->setTagPos(i_pos);
    }
}

void daNpc_Pachi_Taro_c::allLookPosSet(cXyz const& i_pos) {
    fopAc_ac_c* pachiMaroP = mActorMngrs[0].getActorP();
    fopAc_ac_c* pachBesuP = mActorMngrs[1].getActorP();
    
    setLookPos(i_pos);

    if (pachiMaroP != NULL) {
        ((daNpc_Pachi_Maro_c*)pachiMaroP)->setLookPos(i_pos);
    }

    if (pachBesuP != NULL) {
        ((daNpc_Pachi_Besu_c*)pachBesuP)->setLookPos(i_pos);
    }
}

int daNpc_Pachi_Taro_c::talk(void* param_1) {
    switch (mMode) {
        case MODE_ENTER:
        case MODE_INIT:
            if (!mStagger.checkStagger()) {
                clrMesPat();
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

int daNpc_Pachi_Taro_c::test(void* param_1) {
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

BOOL daNpc_Pachi_Taro_c::evtEndProc() {
    BOOL rv = TRUE;
    fopAc_ac_c* actor_p = mActorMngrs[0].getActorP();

    talk_cancel();
    clr_tagHit();

    switch (mPrevEvtNo) {
        case EVT_TUTRIAL_SELECT_GIVEUP:
            field_0xfe2 = 0;
            field_0xfe3 = 0;

            if (mFlow.getChoiceNo() == 0) {
                mEvtNo = EVT_TUTRIAL_GIVEUP;
            } else {
                mEvtNo = EVT_TUTRIAL_CONTINUE;
            }

            mPrevEvtNo = mEvtNo;
            evtChange();
            break;

        case EVT_TUTRIAL_TALK:
        case EVT_HIT_KAKASI_BODY:
        case EVT_TUTRIAL_GIVEUP:
            ((daNpc_Pachi_Maro_c*)actor_p)->setFMotion_LookNone();
            break;

        case EVT_TUTRIAL_BEGIN:
            daNpcT_onEvtBit(608);
            // fallthrough
        default:
            field_0xfe2 = 1;
            mPrevEvtNo = EVT_NONE;
            break;
    }

    return rv;
}

BOOL daNpc_Pachi_Taro_c::cutTutrialBegin(int i_staffId) {
    field_0x1004 = 1;

    dComIfGp_getEvent()->setSkipZev(this, "TUTRIAL_BEGIN_SKIP");
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

BOOL daNpc_Pachi_Taro_c::_cutTutrialBegin_Init(int const& i_cutId) {
    fopAc_ac_c* actors[3] = {this, mActorMngrs[0].getActorP(), mActorMngrs[1].getActorP()};

    switch (i_cutId) {
        case 10:
            field_0xfda = 4;
            initTalk(0x47, actors);
            break;
    }

    return TRUE;
}

BOOL daNpc_Pachi_Taro_c::_cutTutrialBegin_Main(int const& i_cutId) {
    fopAc_ac_c* actors[3] = {this, mActorMngrs[0].getActorP(), mActorMngrs[1].getActorP()};
    BOOL rv = FALSE;

    switch (i_cutId) {
        case 10:
            if (talkProc(NULL, FALSE, actors, FALSE)) {
                rv = TRUE;
            } else {
                u32 nowMsgNo = mFlow.getNowMsgNo();
                if (nowMsgNo == 0x1688 || nowMsgNo == 0x166B) {
                    if (--field_0xfda == 0) {
                        fopAc_ac_c* actor_p = mActorMngrs[1].getActorP();
                        if (actor_p != NULL) {
                            ((daNpc_Pachi_Besu_c*)actor_p)->setFMotion_LookBros();
                        }
                    }
                } else if (nowMsgNo == 0x1689 || nowMsgNo == 0x166C) {
                    fopAc_ac_c* actor_p = mActorMngrs[1].getActorP();
                    if (actor_p != NULL) {
                        ((daNpc_Pachi_Besu_c*)actor_p)->setFMotion_LookNone();
                    }
                }
            }
            break;
    }

    return rv;
}

BOOL daNpc_Pachi_Taro_c::cutTutrialBegin_Skip(int i_staffId) {
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

BOOL daNpc_Pachi_Taro_c::_cutTutrialBegin_Skip_Init(int const& i_cutId) {
    switch (i_cutId) {
        case 10:
            // dSv_event_flag_c::F_0608 - Ordon Village - Began Slingshot tutorial
            daNpcT_onEvtBit(608);
            break;
    }

    return TRUE;
}

BOOL daNpc_Pachi_Taro_c::_cutTutrialBegin_Skip_Main(int const& i_cutId) {
    BOOL rv = FALSE;

    switch (i_cutId) {
        case 10:
            rv = TRUE;
            break;
    }

    return rv;
}

BOOL daNpc_Pachi_Taro_c::cutTutrialClear(int i_staffId) {
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

BOOL daNpc_Pachi_Taro_c::_cutTutrialClear_Init(int const& i_cutId) {
    fopAc_ac_c* actors[3] = {this, mActorMngrs[0].getActorP(), mActorMngrs[1].getActorP()};

    switch (i_cutId) {
        case 5:
            mJntAnm.lookNone(0);
            break;
        
        case 10:
            break;

        case 20:
            initTalk(0x4C, actors);
            break;
    }

    return TRUE;
}

BOOL daNpc_Pachi_Taro_c::_cutTutrialClear_Main(int const& i_cutId) {
    fopAc_ac_c* actors[3] = {this, mActorMngrs[0].getActorP(), mActorMngrs[1].getActorP()};
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
            if (talkProc(NULL, FALSE, actors, FALSE)) {
                rv = TRUE;
            }
            break;
    }

    return rv;
}

BOOL daNpc_Pachi_Taro_c::cutTalk(int i_staffId) {
    dEvent_manager_c& eventManager = dComIfGp_getEventManager();
    int* cutId = NULL;
    cutId = dComIfGp_evmng_getMyIntegerP(i_staffId, "cutId");
    if (cutId == NULL) {
        return TRUE;
    }

    char* mesNos[5] = {
        "MesNo0",
        "MesNo1",
        "MesNo2",
        "MesNo3",
        "MesNo4",
    };

    if (mMesPat < 0) {
        setMesPat();
    }

    int mesNo = 0;
    int* piVar2 = dComIfGp_evmng_getMyIntegerP(i_staffId, mesNos[mMesPat]);
    if (piVar2 != NULL) {
        mesNo = *piVar2;
    }

    if (eventManager.getIsAddvance(i_staffId)) {
        _cutTalk_Init(*cutId);
    }

    return _cutTalk_Main(*cutId, mesNo);
}

BOOL daNpc_Pachi_Taro_c::_cutTalk_Init(int const& i_cutId) {
    fopAc_ac_c* actors[3] = {this, mActorMngrs[0].getActorP(), mActorMngrs[1].getActorP()};

    switch (i_cutId) {
        case 5:
            break;
        
        case 10:
            initTalk(0x4B, actors);
            break;

        case 20:
            break;

        case 25:
            break;

        case 30:
            break;
    }

    return TRUE;
}

BOOL daNpc_Pachi_Taro_c::_cutTalk_Main(int const& i_cutId, int const& i_mesNo) {
    fopAc_ac_c* actors[3] = {this, mActorMngrs[0].getActorP(), mActorMngrs[1].getActorP()};
    fopAc_ac_c* actor_p = mActorMngrs[0].getActorP();
    fopAc_ac_c* actor_p2 = mActorMngrs[1].getActorP();
    int mesNo[2] = {-1, -1};
    BOOL rv = FALSE;

    switch (i_cutId) {
        case 5:
            if (_turn_to_link(0x800)) {
                rv = TRUE;
            }
            break;

       case 10:
            if (i_mesNo == 0) {
                JUT_ASSERT(3563, FALSE);
            }

            mesNo[0] = i_mesNo;

            if (talkProc(mesNo, FALSE, actors, FALSE)) {
                rv = TRUE;
            }
            break;

        case 20:
            if (i_mesNo == 0) {
                JUT_ASSERT(3574, FALSE);
            }

            mesNo[0] = i_mesNo;

            if (talkProc(mesNo, FALSE, actors, FALSE)) {
                rv = TRUE;
            }
            break;

        case 30:
            if (talkProc(NULL, FALSE, actors, FALSE)) {
                rv = 1;
            } else {
                u32 nowMsgNo = mFlow.getNowMsgNo();
                if (nowMsgNo == 0x1698 || nowMsgNo == 0x167C) {
                    ((daNpc_Pachi_Maro_c*)actor_p)->setFMotion_Niramu_to_Besu();
                }
            }
            break;
        
        default:
            rv = TRUE;
            break;
    }

    return rv;
}

BOOL daNpc_Pachi_Taro_c::cutTalk2(int i_staffId) {
    dEvent_manager_c& eventManager = dComIfGp_getEventManager();
    int* cutId = NULL;
    cutId = dComIfGp_evmng_getMyIntegerP(i_staffId, "cutId");
    if (cutId == NULL) {
        return TRUE;
    }

    char* mesNos[5] = {
        "MesNo0",
        "MesNo1",
        "MesNo2",
        "MesNo3",
        "MesNo4",
    };

    int* piVar2 = NULL;
    int mesNo = 0;
    if (mMesPat < 0) {
        setMesPat();
    }

    piVar2 = dComIfGp_evmng_getMyIntegerP(i_staffId, mesNos[mMesPat]);
    if (piVar2 != NULL) {
        mesNo = *piVar2;
    }

    if (eventManager.getIsAddvance(i_staffId)) {
        _cutTalk2_Init(*cutId);
    }

    return _cutTalk2_Main(*cutId, mesNo);
}

BOOL daNpc_Pachi_Taro_c::_cutTalk2_Init(int const& i_cutId) {
    fopAc_ac_c* actors[3] = {this, mActorMngrs[0].getActorP(), mActorMngrs[1].getActorP()};

    switch (i_cutId) {
        case 5:
            break;
        
        case 10:
            initTalk(0x4B, actors);
            break;

        case 20:
            break;

        case 25:
            break;

        case 30:
            break;
    }

    return TRUE;
}

BOOL daNpc_Pachi_Taro_c::_cutTalk2_Main(int const& i_cutId, int const& i_mesNo) {
    fopAc_ac_c* actors[3] = {this, mActorMngrs[0].getActorP(), mActorMngrs[1].getActorP()};
    fopAc_ac_c* actor_p = mActorMngrs[0].getActorP();
    fopAc_ac_c* actor_p2 = mActorMngrs[1].getActorP();
    int mesNo[2] = {-1, -1};
    BOOL rv = FALSE;

    switch (i_cutId) {
        case 5:
            if (_turn_to_link(0x800)) {
                rv = TRUE;
            }
            break;

       case 10:
            if (i_mesNo == 0) {
                JUT_ASSERT(3749, FALSE);
            }

            mesNo[0] = i_mesNo;

            if (talkProc(mesNo, FALSE, actors, FALSE)) {
                rv = TRUE;
            }
            break;

        case 20:
            if (i_mesNo == 0) {
                JUT_ASSERT(3760, FALSE);
            }

            mesNo[0] = i_mesNo;

            if (talkProc(mesNo, FALSE, actors, FALSE)) {
                rv = TRUE;
            }
            break;

        case 30:
            if (talkProc(NULL, FALSE, actors, FALSE)) {
                rv = 1;
            } else {
                u32 nowMsgNo = mFlow.getNowMsgNo();
                if (nowMsgNo == 0x1698 || nowMsgNo == 0x167C) {
                    ((daNpc_Pachi_Maro_c*)actor_p)->setFMotion_Niramu_to_Besu();
                }
            }
            break;
        
        default:
            rv = TRUE;
            break;
    }

    return rv;
}

BOOL daNpc_Pachi_Taro_c::cut1stHit_KakasiBody(int i_staffId) {
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

BOOL daNpc_Pachi_Taro_c::_cut1stHit_KakasiBody_Init(int const& i_cutId) {
    fopAc_ac_c* actors[3] = {this, mActorMngrs[0].getActorP(), mActorMngrs[1].getActorP()};

    static int const sHitMsg[3] = {
        0x48, 0x4A, 0x49,
    };
    
    switch (i_cutId) {
        case 5:
            mJntAnm.lookNone(0);
            break;

        case 10:
            break;

        case 20:
            if (sHitMsg[mHitMsgIdx] != 0x48) {
                fopAc_ac_c* actor_p = mActorMngrs[1].getActorP();
                ((daNpc_Pachi_Besu_c*)actor_p)->setFMotion_Hit();
            }

            field_0xfda = 30;
            initTalk(sHitMsg[mHitMsgIdx], actors);
            break;
    }

    return TRUE;
}

BOOL daNpc_Pachi_Taro_c::_cut1stHit_KakasiBody_Main(int const& i_cutId) {
    fopAc_ac_c* actors[3] = {this, mActorMngrs[0].getActorP(), mActorMngrs[1].getActorP()};
    daNpc_Pachi_Besu_c* besu_p = (daNpc_Pachi_Besu_c*)mActorMngrs[1].getActorP();
    daNpc_Pachi_Maro_c* maro_p = (daNpc_Pachi_Maro_c*)mActorMngrs[0].getActorP();
    BOOL rv = FALSE;

    switch (i_cutId) {
        case 5:
            rv = TRUE;
            break;
        
        case 10: {
            cXyz pos(894.916f, 903.411, -2040.36);
            if (_turn_pos(pos, 0x1000)) {
                rv = TRUE;
            }
        }
        break;

        case 20:
            if (talkProc(NULL, FALSE, actors, FALSE)) {
                rv = TRUE;
            } else {
                u32 nowMsgNo = mFlow.getNowMsgNo();
                if (nowMsgNo == 0x168F) {
                    if (--field_0xfda <= 0 && besu_p != NULL) {
                        besu_p->setFMotion_Mad();
                    }
                } else if ((nowMsgNo == 0x1675 || nowMsgNo == 0x168C || nowMsgNo == 0x166F || nowMsgNo == 0x1672) && maro_p != NULL) {
                    maro_p->setFMotion_Niramu_to_Besu();
                }
            }
            break;
    }

    return rv;
}

BOOL daNpc_Pachi_Taro_c::cutTutrialSelectGiveUp(int i_staffId) {
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

BOOL daNpc_Pachi_Taro_c::_cutTutrialSelectGiveUp_Init(int const& i_cutId) {
    fopAc_ac_c* actors[3] = {this, mActorMngrs[0].getActorP(), mActorMngrs[1].getActorP()};

    switch (i_cutId) {        
        case 10:
        case 11:
            break;
        
        case 20:
            field_0xfda = 4;
            daNpcF_offTmpBit(11);
            OS_REPORT("------------------------off tmp bit : 11!!\n");
            initTalk(0x4D, actors);
            break;
    }

    return TRUE;
}

BOOL daNpc_Pachi_Taro_c::_cutTutrialSelectGiveUp_Main(int const& i_cutId) {
    fopAc_ac_c* actors[3] = {this, mActorMngrs[0].getActorP(), mActorMngrs[1].getActorP()};
    fopAc_ac_c* actor_p = mActorMngrs[1].getActorP();
    fopAc_ac_c* actor_p2 = mActorMngrs[0].getActorP();
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
            if (talkProc(NULL, FALSE, actors, FALSE)) {
                rv = TRUE;
            } else {
                u32 nowMsgNo = mFlow.getNowMsgNo();
                if (nowMsgNo == 0x1682) {
                    if (--field_0xfda == 0 && actor_p != NULL) {
                        ((daNpc_Pachi_Besu_c*)actor_p)->setFMotion_LookBros();
                    }
                } else if (nowMsgNo == 0x1683) {
                    if (actor_p != NULL) {
                        ((daNpc_Pachi_Besu_c*)actor_p)->setFMotion_LookNone();
                    }

                    if (actor_p2 != NULL) {
                        ((daNpc_Pachi_Maro_c*)actor_p2)->setFMotion_Niramu_to_Besu();
                    }
                }
            }
            break;
    }

    return rv;
}

BOOL daNpc_Pachi_Taro_c::cutTutrialGiveUp(int i_staffId) {
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

BOOL daNpc_Pachi_Taro_c::_cutTutrialGiveUp_Init(int const& i_cutId) {
    fopAc_ac_c* actors[3] = {this, mActorMngrs[0].getActorP(), mActorMngrs[1].getActorP()};

    switch (i_cutId) {
        case 5:
        case 10:
            break;
        
        case 20:
            field_0xfda = -1;
            initTalk(0x4D, actors);
            break;
    }

    return TRUE;
}

BOOL daNpc_Pachi_Taro_c::_cutTutrialGiveUp_Main(int const& i_cutId) {
    fopAc_ac_c* actors[3] = {this, mActorMngrs[0].getActorP(), mActorMngrs[1].getActorP()};
    fopAc_ac_c* actor_p = mActorMngrs[1].getActorP();
    fopAc_ac_c* actor_p2 = mActorMngrs[0].getActorP();
    BOOL rv = FALSE;

    switch (i_cutId) {
        case 5:
            rv = TRUE;
            break;
        
        case 10:
            rv = TRUE;
            break;

        case 20:
            if (talkProc(NULL, FALSE, actors, FALSE)) {
                rv = TRUE;
            } else {
                u32 nowMsgNo = mFlow.getNowMsgNo();
                if (nowMsgNo == 0x169A) {
                    if (field_0xfda < 0) {
                        field_0xfda = 4;
                    } else if (field_0xfda > 0 && --field_0xfda == 0 && actor_p != NULL) {
                        ((daNpc_Pachi_Besu_c*)actor_p)->setFMotion_LookBros();
                    }
                } else if (nowMsgNo == 0x169B) {
                    if (actor_p != NULL) {
                        ((daNpc_Pachi_Besu_c*)actor_p)->setFMotion_LookNone();
                    }
                } else if (nowMsgNo == 0x1685 && actor_p2 != NULL) {
                    ((daNpc_Pachi_Maro_c*)actor_p2)->setFMotion_Niramu_to_Besu();
                }
            }
            break;
    }

    return rv;
}

BOOL daNpc_Pachi_Taro_c::cutTutrialCaution(int i_staffId) {
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

BOOL daNpc_Pachi_Taro_c::_cutTutrialCaution_Init(int const& i_cutId) {
    switch (i_cutId) {
        case 5:
            break;
        
        case 10:
            initTalk(0x4E, NULL);
            break;
    }

    return TRUE;
}

BOOL daNpc_Pachi_Taro_c::_cutTutrialCaution_Main(int const& i_cutId) {
    BOOL rv = FALSE;

    switch (i_cutId) {
        case 5:
            if (_turn_to_link(0xC00)) {
                rv = TRUE;
            }
            break;

        case 10:
            if (talkProc(NULL, FALSE, NULL, FALSE)) {
                rv = TRUE;
            }
            break;
    }

    return rv;
}

BOOL daNpc_Pachi_Taro_c::cutTutrialContinue(int i_staffId) {
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

BOOL daNpc_Pachi_Taro_c::_cutTutrialContinue_Init(int const& i_cutId) {
    switch (i_cutId) {
        case 10:
            break;
        
        case 20:
            break;
    }
    
    return TRUE;
}

BOOL daNpc_Pachi_Taro_c::_cutTutrialContinue_Main(int const& i_cutId) {
    BOOL rv = FALSE;

    switch (i_cutId) {
        case 10:
            if (_turn_pos(mTagPos, 0x1000)) {
                field_0xfe2 = 1;
                rv = TRUE;
            }
            break;
        
        case 20:
            BOOL chaseAngleFlag = cLib_chaseAngleS(&mCurAngle.y, (s16)home.angle.y, 0x800);
            current.angle.y = mCurAngle.y;
            shape_angle.y = mCurAngle.y;

            if (chaseAngleFlag) {
                daNpc_Pachi_Maro_c* maro_p = (daNpc_Pachi_Maro_c*)mActorMngrs[0].getActorP();
                maro_p->setFMotion_LookNone();
                rv = TRUE;
            }
    }

    return rv;
}

static int daNpc_Pachi_Taro_Create(void* i_this) {
    return static_cast<daNpc_Pachi_Taro_c*>(i_this)->create();
}

static int daNpc_Pachi_Taro_Delete(void* i_this) {
    return static_cast<daNpc_Pachi_Taro_c*>(i_this)->Delete();
}

static int daNpc_Pachi_Taro_Execute(void* i_this) {
    return static_cast<daNpc_Pachi_Taro_c*>(i_this)->Execute();
}

static int daNpc_Pachi_Taro_Draw(void* i_this) {
    return static_cast<daNpc_Pachi_Taro_c*>(i_this)->Draw();
}

static int daNpc_Pachi_Taro_IsDelete(void* i_this) {
    return 1;
}

static actor_method_class daNpc_Pachi_Taro_MethodTable = {
    (process_method_func)daNpc_Pachi_Taro_Create,
    (process_method_func)daNpc_Pachi_Taro_Delete,
    (process_method_func)daNpc_Pachi_Taro_Execute,
    (process_method_func)daNpc_Pachi_Taro_IsDelete,
    (process_method_func)daNpc_Pachi_Taro_Draw,
};

actor_process_profile_definition g_profile_NPC_PACHI_TARO = {
  fpcLy_CURRENT_e,               // mLayerID
  7,                             // mListID
  fpcPi_CURRENT_e,               // mListPrio
  PROC_NPC_PACHI_TARO,           // mProcName
  &g_fpcLf_Method.base,         // sub_method
  sizeof(daNpc_Pachi_Taro_c),    // mSize
  0,                             // mSizeOther
  0,                             // mParameters
  &g_fopAc_Method.base,          // sub_method
  369,                           // mPriority
  &daNpc_Pachi_Taro_MethodTable, // sub_method
  0x00044107,                    // mStatus
  fopAc_NPC_e,                   // mActorType
  fopAc_CULLBOX_CUSTOM_e,        // cullType
};
