/**
 * @file d_a_npc_kn.cpp
 *
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "Z2AudioLib/Z2Instances.h"
#include "d/actor/d_a_npc.h"
#include "d/actor/d_a_npc_gwolf.h"
#include "d/actor/d_a_npc_kn.h"
#include "d/actor/d_a_obj_knBullet.h"
#include "d/d_meter2_info.h"
#if DEBUG
#include "JSystem/JHostIO/JORFile.h"
#include "d/d_debug_viewer.h"
#endif

dCcD_SrcCyl daNpc_Kn_c::mCcDCyl = {
    mCcDObjData,
    {
        {{0.0f, 0.0f, 0.0f}, 0.0f, 0.0f},
    }
};

dCcD_SrcSph daNpc_Kn_c::mCcDSph = {
    mCcDObjData,
    {
        {{0.0f, 0.0f, 0.0f}, 0.0f}  // mSphCc
    },
};

s16 daNpc_Kn_c::mSrchName;

fopAc_ac_c* daNpc_Kn_c::mFindActorPtrs[50];

int daNpc_Kn_c::mFindCount;

void* dummy_srchActor(void* i_actor1, void* i_actor2) {
    // Fake function (though a similar one likely existed and got stripped out).
    // daNpc_Kn_c::setSwordChargePtcl has issues where the ...rodata and ...bss pools are loaded in
    // reverse order, which in turn also causes regalloc in that function.
    // Fixing this requires a function early on in this TU to also use ...bss pooling, which for
    // some reason fixes later pool loads.
    // The unused member bss variables mSrchName/mFindActorPtrs/mFindCount existing in the maps
    // indicates that a function using them originally existed, but was stripped out.
    // Additionally, the bss members must be defined above this function for bss pooling to be used.
    fopAc_ac_c* actor2 = (fopAc_ac_c*)i_actor2;
    void* foundActor = NULL;
    if (daNpc_Kn_c::mFindCount < 50 && fopAcM_IsActor(i_actor1) && i_actor1 != i_actor2) {
        if (daNpc_Kn_c::mSrchName == fopAcM_GetName((fopAc_ac_c*)i_actor1)) {
            foundActor = (fopAc_ac_c*)i_actor1;
            daNpc_Kn_c::mFindActorPtrs[daNpc_Kn_c::mFindCount] = (fopAc_ac_c*)foundActor;
            daNpc_Kn_c::mFindCount++;
        }
    }

    return NULL;
}

const dCcD_SrcGObjInf daNpc_Kn_c::mCcDObjData = {
    {0, {{0, 0, 0}, {0, 0}, {0x79}}},
    {dCcD_SE_NONE, 0, 0, 0, 0},
    {dCcD_SE_NONE, 0, 0, 0, 0},
    {0},
};

#include "d/actor/d_a_npc_kn_base.inc"

const daNpc_Kn_HIOParam daNpc_Kn_Param_c::m = {
    50.0f,
    -5.0f,
    1.0f,
    1250.0f,
    245.0f,
    270.0f,
    0.0f,
    55.0f,
    0.0f,
    0.0f,
    40.0f,
    -40.0f,
    30.0f,
    -30.0f,
    60.0f,
    -60.0f,
    0.6f,
    12.0f,
    6,
    6,
    8,
    6,
    0.0f,
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
    255,
    300,
    90,
    100.0f,
    240.0f,
    1100.0f,
    27.0f,
    50.0f,
    16,
    0x200,
    120,
    90,
    2.0f,
};

#if DEBUG
daNpc_Kn_HIO_c::daNpc_Kn_HIO_c() {
    m = daNpc_Kn_Param_c::m;
}

void daNpc_Kn_HIO_c::listenPropertyEvent(const JORPropertyEvent* event) {
    char msg_buffer[2000];

    JORReflexible::listenPropertyEvent(event);

    JORFile aJStack_910;
    int len;
    switch ((u32)event->id) {
    case 0x40000002:
        if (aJStack_910.open(6, "すべてのファイル(*.*)\0*.*\0", NULL, NULL, NULL) != 0) {
            memset(msg_buffer, 0, sizeof(msg_buffer));
            len = 0;
            daNpcT_cmnListenPropertyEvent(msg_buffer, &len, &m.common);
            sprintf(msg_buffer + len, "%d,   \t//  アルファ\n", m.alpha);
            len = strlen(msg_buffer);
            sprintf(msg_buffer + len, "%d,   \t//  攻撃され待ちタイム\n", m.attack_wait_time);
            len = strlen(msg_buffer);
            sprintf(msg_buffer + len, "%d,   \t//  追い討ち待ちタイム\n", m.followup_wait_time);
            len = strlen(msg_buffer);
            sprintf(msg_buffer + len, "%3.3f,   \t//  小滑り距離\n", m.small_slide_distance);
            len = strlen(msg_buffer);
            sprintf(msg_buffer + len, "%3.3f,   \t//  大滑り距離\n", m.big_slide_distance);
            len = strlen(msg_buffer);
            sprintf(msg_buffer + len, "%3.3f,   \t//  ワープ距離\n", m.warp_distance);
            len = strlen(msg_buffer);
            sprintf(msg_buffer + len, "%3.3f,   \t//  攻撃吹っ飛び速度横\n", m.attack_disappear_speed_h);
            len = strlen(msg_buffer);
            sprintf(msg_buffer + len, "%3.3f,   \t//  攻撃吹っ飛び速度縦\n", m.attack_disappear_speed_v);
            len = strlen(msg_buffer);
            sprintf(msg_buffer + len, "%d,   \t//  回転分割数\n", m.rotation_num);
            len = strlen(msg_buffer);
            sprintf(msg_buffer + len, "%d,   \t//  回転角速度\n", m.rotation_speed);
            len = strlen(msg_buffer);
            sprintf(msg_buffer + len, "%d,   \t//  攻撃開始最小時間\n", m.attack_start_min_time);
            len = strlen(msg_buffer);
            sprintf(msg_buffer + len, "%d,   \t//  攻撃開始時間幅\n", m.attack_start_time_range);
            len = strlen(msg_buffer);
            sprintf(msg_buffer + len, "%3.3f,   \t//  移動速度\n", m.move_speed);
            len = strlen(msg_buffer);
            aJStack_910.writeData(msg_buffer, len);
            aJStack_910.close();
            OS_REPORT("write append success!::%6d\n", len);
        } else {
            OS_REPORT("write append failure!\n");
        }
        break;
    }
}

void daNpc_Kn_HIO_c::genMessage(JORMContext* ctext) {
    daNpcT_cmnGenMessage(ctext, &m.common);
    // alpha
    ctext->genSlider("アルファ        ", &m.alpha, 0, 0xFF, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
    ctext->genSlider("攻撃され待ち時間    ", &m.attack_wait_time, 0, 10000, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
    ctext->genSlider("追い討ち待ち時間", &m.followup_wait_time, 0, 10000, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
    ctext->genSlider("小滑り距離      ", &m.small_slide_distance, 0.0f, 1000.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
    ctext->genSlider("大滑り距離      ", &m.big_slide_distance, 0.0f, 1000.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
    ctext->genSlider("ワープ距離      ", &m.warp_distance, 0.0f, 10000.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
    ctext->genSlider("攻撃吹飛び速度横", &m.attack_disappear_speed_h, 0.0f, 1000.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
    ctext->genSlider("攻撃吹飛び速度縦", &m.attack_disappear_speed_v, 0.0f, 1000.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
    ctext->genSlider("回転分割数      ", &m.rotation_num, 0, 100, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
    ctext->genSlider("回転角速度      ", &m.rotation_speed, 0, 0x7FFF, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
    ctext->genSlider("攻撃開始最小時間", &m.attack_start_min_time, 0, 10000, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
    ctext->genSlider("攻撃開始時間幅  ", &m.attack_start_time_range, 0, 10000, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
    ctext->genSlider("移動速度        ", &m.move_speed, 0.0f, 1000.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
    // export file:
    ctext->genButton("ファイル書き出し", 0x40000002, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
}
#endif

static int l_bmdData[3][2] = {
    {47, 1},
    {48, 1},
    {49, 1},
};

static daNpcT_evtData_c l_evtList[26] = {
    {"", 0},
    {"NONE_EQUIP_CHANGE_TALK", 1},
    {"NONE_EQUIP_CHANGE_TALK_WAIT", 1},
    {"NO_EQ_CHNGE_TLK_STD", 1},
    {"LARGE_DAMAGE_TALK", 1},
    {"FIRST_ENCOUNT", 2},
    {"ATTACK_FAILED", 2},
    {"FIRST_SKILL_GET", 2},
    {"SECOND_ENCOUNT", 1},
    {"SECOND_SKILL_EXPLAIN", 3},
    {"REFLECT_EXPLAIN", 3},
    {"SECOND_SKILL_GET", 3},
    {"THIRD_SKILL_EXPLAIN", 4},
    {"THIRD_SKILL_FIRST", 1},
    {"THIRD_SKILL_GET", 4},
    {"FOURTH_SKILL_EXPLAIN", 5},
    {"FOURTH_SKILL_GET", 5},
    {"FIFTH_SKILL_EXPLAIN", 6},
    {"FIFTH_SKILL_FIRST", 1},
    {"FIFTH_SKILL_GET", 6},
    {"SIXTH_SKILL_EXPLAIN", 7},
    {"SIXTH_SKILL_GET", 7},
    {"SIXTH_SKILL_GET_STAND", 7},
    {"SEVENTH_SKILL_EXPLAIN", 8},
    {"SEVENTH_SKILL_GET", 8},
    {"SEVENTH_SKILL_GET_STAND", 8},
};

static char* l_resNameList[9] = {
    "",          "KN_a",      "KN_tch01D", "KN_tch02D", "KN_tch03D",
    "KN_tch04D", "KN_tch05D", "KN_tch06D", "KN_tch07D",
};

static s8 l_loadResPtrnTeach01[3] = {
    1,
    2,
    -1,
};

static s8 l_loadResPtrnTeach02[4] = {
    1,
    2,
    3,
    -1,
};

static s8 l_loadResPtrnTeach03[4] = {
    1,
    3,
    4,
    -1,
};

static s8 l_loadResPtrnTeach04[4] = {
    1,
    4,
    5,
    -1,
};

static s8 l_loadResPtrnTeach05[4] = {
    1,
    5,
    6,
    -1,
};

static s8 l_loadResPtrnTeach06[4] = {
    1,
    6,
    7,
    -1,
};

static s8 l_loadResPtrnTeach07[3] = {
    1,
    8,
    -1,
};

static s8 l_loadResPtrn0[2] = {
    1,
    -1,
};

static s8* l_loadResPtrnList[8] = {
    l_loadResPtrnTeach01, l_loadResPtrnTeach02, l_loadResPtrnTeach03, l_loadResPtrnTeach04,
    l_loadResPtrnTeach05, l_loadResPtrnTeach06, l_loadResPtrnTeach07, l_loadResPtrn0,
};

static daNpcT_faceMotionAnmData_c l_faceMotionAnmData[2] = {
    {-1, 0, 0, -1, 0, 0, 0},
    {21, 0, 1, -1, 0, 0, 0},
};

static int l_motionAnmData[245] = {
    43, 2, 1, -1, 0, 0, 0, 38, 2, 1, -1, 0, 0, 0, 39, 2, 1, -1, 0, 0, 0, 41, 0, 1, -1, 0, 0, 0,
    16, 0, 1, -1, 0, 0, 0, 11, 0, 1, -1, 0, 0, 0, 7,  0, 1, -1, 0, 0, 0, 8,  0, 1, -1, 0, 0, 0,
    9,  2, 1, -1, 0, 0, 0, 14, 0, 1, -1, 0, 0, 0, 13, 2, 1, -1, 0, 0, 0, 12, 0, 1, -1, 0, 0, 0,
    30, 0, 1, -1, 0, 0, 0, 17, 2, 1, -1, 0, 0, 0, 26, 0, 1, -1, 0, 0, 0, 28, 2, 1, -1, 0, 0, 0,
    35, 0, 1, -1, 0, 0, 0, 36, 2, 1, -1, 0, 0, 0, 10, 0, 1, -1, 0, 0, 0, 19, 0, 1, -1, 0, 0, 0,
    40, 0, 1, -1, 0, 0, 0, 37, 0, 1, -1, 0, 0, 0, 5,  0, 1, -1, 0, 0, 0, 18, 2, 1, -1, 0, 0, 0,
    27, 0, 1, -1, 0, 0, 0, 29, 2, 1, -1, 0, 0, 0, 20, 0, 1, -1, 0, 0, 0, 15, 0, 1, -1, 0, 0, 0,
    24, 0, 1, -1, 0, 0, 0, 25, 2, 1, -1, 0, 0, 0, 44, 2, 1, -1, 0, 0, 0, 42, 0, 1, -1, 0, 0, 0,
    22, 0, 1, -1, 0, 0, 0, 23, 2, 1, -1, 0, 0, 0, 6,  0, 1, -1, 0, 0, 0,
};

static int l_podBckData[5][2] = {
    {-1, 1},
    {34, 1},
    {33, 1},
    {31, 1},
    {32, 1},
};

static daNpcT_MotionSeqMngr_c::sequenceStepData_c l_faceMotionSequenceData[8] = {
    {1, -1, 0},
    {-1, 0, 0},
    {-1, 0, 0},
    {-1, 0, 0},
    {0, -1, 0},
    {-1, 0, 0},
    {-1, 0, 0},
    {-1, 0, 0},
};

static daNpcT_MotionSeqMngr_c::sequenceStepData_c l_motionSequenceData[132] = {
    {0, -1, 0},
    {-1, 0, 0},
    {-1, 0, 0},
    {-1, 0, 0},
    {3, -1, 1},
    {0, -1, 0},
    {-1, 0, 0},
    {-1, 0, 0},
    {15, -1, 0},
    {-1, 0, 0},
    {-1, 0, 0},
    {-1, 0, 0},
    {30, -1, 0},
    {-1, 0, 0},
    {-1, 0, 0},
    {-1, 0, 0},
    {31, -1, 1},
    {30, -1, 0},
    {-1, 0, 0},
    {-1, 0, 0},
    {-1, -1, 0},
    {-1, 0, 0},
    {-1, 0, 0},
    {-1, 0, 0},
    {29, 0, 0},
    {-1, 0, 0},
    {-1, 0, 0},
    {-1, 0, 0},
    {33, 0, 0},
    {-1, 0, 0},
    {-1, 0, 0},
    {-1, 0, 0},
    {5, 0, 1},
    {4, 3, 1},
    {0, -1, 0},
    {-1, 0, 0},
    {2, -1, 0},
    {-1, 0, 0},
    {-1, 0, 0},
    {-1, 0, 0},
    {34, 3, 0},
    {-1, 0, 0},
    {-1, 0, 0},
    {-1, 0, 0},
    {7, 0, 1},
    {8, 0, 0},
    {-1, 0, 0},
    {-1, 0, 0},
    {32, 3, 0},
    {-1, 0, 0},
    {-1, 0, 0},
    {-1, 0, 0},
    {28, 3, 0},
    {-1, 0, 0},
    {-1, 0, 0},
    {-1, 0, 0},
    {23, 6, 0},
    {-1, 0, 0},
    {-1, 0, 0},
    {-1, 0, 0},
    {24, 3, 1},
    {25, 0, 0},
    {-1, 0, 0},
    {-1, 0, 0},
    {26, -1, 1},
    {0, 0, 0},
    {-1, 0, 0},
    {-1, 0, 0},
    {20, 3, 1},
    {21, 3, 1},
    {22, 3, 1},
    {0, 3, 0},
    {13, 6, 0},
    {-1, 0, 0},
    {-1, 0, 0},
    {-1, 0, 0},
    {14, 3, 1},
    {15, 0, 0},
    {-1, 0, 0},
    {-1, 0, 0},
    {16, -1, 1},
    {17, 0, 0},
    {-1, 0, 0},
    {-1, 0, 0},
    {18, -1, 1},
    {15, 0, 0},
    {-1, 0, 0},
    {-1, 0, 0},
    {19, -1, 1},
    {0, 0, 0},
    {-1, 0, 0},
    {-1, 0, 0},
    {5, 0, 1},
    {0, -1, 0},
    {-1, 0, 0},
    {-1, 0, 0},
    {27, -1, 1},
    {2, -1, 0},
    {-1, 0, 0},
    {-1, 0, 0},
    {12, 3, 1},
    {2, -1, 0},
    {-1, 0, 0},
    {-1, 0, 0},
    {4, 3, 1},
    {0, -1, 0},
    {-1, 0, 0},
    {-1, 0, 0},
    {5, 0, 1},
    {2, -1, 0},
    {-1, 0, 0},
    {-1, 0, 0},
    {6, 0, 1},
    {0, -1, 0},
    {-1, 0, 0},
    {-1, 0, 0},
    {7, 0, 1},
    {0, -1, 0},
    {-1, 0, 0},
    {-1, 0, 0},
    {7, 0, 1},
    {2, -1, 0},
    {-1, 0, 0},
    {-1, 0, 0},
    {7, 0, 1},
    {1, -1, 0},
    {-1, 0, 0},
    {-1, 0, 0},
    {1, 3, 0},
    {-1, 0, 0},
    {-1, 0, 0},
    {-1, 0, 0},
};

char* daNpc_Kn_c::mCutNameList[21] = {
    "",
    "NONE_EQUIP_CHANGE_TALK",
    "NO_EQ_CHNGE_TLK_STD",
    "LARGE_DAMAGE_TALK",
    "FIRST_ENCOUNT",
    "ATTACK_FAILED",
    "FIRST_SKILL_GET",
    "SECOND_ENCOUNT",
    "SECOND_SKILL_EXPLAIN",
    "REFLECT_EXPLAIN",
    "SECOND_SKILL_GET",
    "THIRD_SKILL_EXPLAIN",
    "THIRD_SKILL_GET",
    "FOURTH_SKILL_EXPLAIN",
    "FOURTH_SKILL_GET",
    "FIFTH_SKILL_EXPLAIN",
    "FIFTH_SKILL_GET",
    "SIXTH_SKILL_EXPLAIN",
    "SIXTH_SKILL_GET",
    "SEVENTH_SKILL_EXPLAIN",
    "SEVENTH_SKILL_GET",
};

daNpc_Kn_c::cutFunc daNpc_Kn_c::mCutList[21] = {
    NULL,
    &daNpc_Kn_c::ECut_noneEquipChangeTalk,
    &daNpc_Kn_c::ECut_noneEquipChangeTalkStand,
    &daNpc_Kn_c::ECut_largeDamageTalk,
    &daNpc_Kn_c::ECut_firstEncount,
    &daNpc_Kn_c::ECut_attackFailed,
    &daNpc_Kn_c::ECut_firstSkillGet,
    &daNpc_Kn_c::ECut_secondEncount,
    &daNpc_Kn_c::ECut_secondSkillExplain,
    &daNpc_Kn_c::ECut_reflectExplain,
    &daNpc_Kn_c::ECut_secondSkillGet,
    &daNpc_Kn_c::ECut_thirdSkillExplain,
    &daNpc_Kn_c::ECut_thirdSkillGet,
    &daNpc_Kn_c::ECut_fourthSkillExplain,
    &daNpc_Kn_c::ECut_fourthSkillGet,
    &daNpc_Kn_c::ECut_fifthSkillExplain,
    &daNpc_Kn_c::ECut_fifthSkillGet,
    &daNpc_Kn_c::ECut_sixthSkillExplain,
    &daNpc_Kn_c::ECut_sixthSkillGet,
    &daNpc_Kn_c::ECut_seventhSkillExplain,
    &daNpc_Kn_c::ECut_seventhSkillGet,
};

static NPC_KN_HIO_CLASS l_HIO;

daNpc_Kn_c::~daNpc_Kn_c() {
    // destruct
    OS_REPORT("|%06d:%x|daNpc_Kn_c -> デストラクト\n", g_Counter.mCounter0, this);
    if (heap != NULL) {
        mpModelMorf[0]->stopZelAnime();
        mpModelMorf[1]->stopZelAnime();
    }

#if DEBUG
    if (mpHIO) {
        mpHIO->removeHIO();
    }
#endif

    deleteRes(l_loadResPtrnList[mType], (const char**)l_resNameList);
}

int daNpc_Kn_c::create() {
    daNpcT_ct(this, daNpc_Kn_c, l_faceMotionAnmData,
                       (daNpcT_motionAnmData_c*)l_motionAnmData, l_faceMotionSequenceData, 4,
                       l_motionSequenceData, 4, l_evtList, l_resNameList);

    mType = getType();
    mDivideNo = getDivideNo();
    mFlowNodeNo = getFlowNodeNo();
    mTwilight = FALSE;

    OS_REPORT("\t(%s:%d) flowNo:%d, PathNo:%02x<%08x> ", fopAcM_getProcNameString(this), mType, mFlowNodeNo, getPath(), fopAcM_GetParam(this));

    if (isDelete()) {
        OS_REPORT("===>isDelete:TRUE\n");
        return cPhs_ERROR_e;
    }

    OS_REPORT("\n");

    int phase_state = loadRes(l_loadResPtrnList[mType], (const char**)l_resNameList);
    if (phase_state == cPhs_COMPLEATE_e) {
        static int const heapSize[8] = {0x4B10, 0x4B20, 0x4B10, 0x4B10, 0x5A00, 0x4B10, 0x4B20, 0x0000};
        if (!fopAcM_entrySolidHeap(this, createHeapCallBack, heapSize[mType])) {
            return cPhs_ERROR_e;
        }

        J3DModelData* model_data = mpModelMorf[0]->getModel()->getModelData();
        fopAcM_SetMtx(this, mpModelMorf[0]->getModel()->getBaseTRMtx());
        fopAcM_setCullSizeBox(this, -300.0f, -50.0f, -300.0f, 300.0f, 450.0f, 300.0f);
        fopAcM_OnStatus(this, fopAcM_STATUS_UNK_0x8000000);

        mSound.init(&current.pos, &eyePos, 3, 1);
        dKy_tevstr_init(&mTevStr, fopAcM_GetRoomNo(this), 0xFF);
#if DEBUG
        mpHIO = &l_HIO;
        // Spirit of the brave:
        mpHIO->entryHIO("勇者の霊");
#endif

        mAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1,
                  &mAcchCir, fopAcM_GetSpeed_p(this), fopAcM_GetAngle_p(this),
                  fopAcM_GetShapeAngle_p(this));
        mAcch.CrrPos(dComIfG_Bgsp());
        mGndChk = mAcch.m_gnd;
        mGroundHeight = mAcch.GetGroundH();

        setEnvTevColor();
        setRoomNo();

        mCcStts.Init(mpHIO->m.common.weight, 0, this);
        resetCol();
        reset();

        mCreating = TRUE;
        Execute();
        mCreating = FALSE;
    }

    return phase_state;
}

int daNpc_Kn_c::CreateHeap() {
    J3DModelData* mdlData_p = NULL;
    J3DModel* model_p = NULL;
    int bmd_offset = 1;
    int arc_idx = l_bmdData[bmd_offset][1];
    int res_idx = l_bmdData[bmd_offset][0];

    mdlData_p = static_cast<J3DModelData*>(dComIfG_getObjectRes(l_resNameList[arc_idx], res_idx));
    JUT_ASSERT(1259, NULL != mdlData_p);

    u32 sp2C = 0x11020284;
    mpModelMorf[0] = new mDoExt_McaMorfSO(mdlData_p, NULL, NULL, NULL, -1, 1.0f, 0, -1, &mSound, 0x80000, sp2C);
    if (mpModelMorf[0] != NULL && mpModelMorf[0]->getModel() == NULL) {
        mpModelMorf[0]->stopZelAnime();
        mpModelMorf[0] = NULL;
    }

    if (mpModelMorf[0] == NULL) {
        return 0;
    }

    model_p = mpModelMorf[0]->getModel();

    for (u16 i = 0; i < mdlData_p->getJointNum(); i++) {
        mdlData_p->getJointNodePointer(i)->setCallBack(ctrlJointCallBack);
    }

    model_p->setUserArea((uintptr_t)this);

    if (mInvisModel.create(mpModelMorf[0]->getModel(), 1) == FALSE) {
        return 0;
    }

    bmd_offset = 0;
    arc_idx = l_bmdData[bmd_offset][1];
    res_idx = l_bmdData[bmd_offset][0];
    mdlData_p = static_cast<J3DModelData*>(dComIfG_getObjectRes(l_resNameList[arc_idx], res_idx));
    JUT_ASSERT(1310, NULL != mdlData_p);

    sp2C = 0x11020284;
    mpModelMorf[1] = new mDoExt_McaMorfSO(mdlData_p, NULL, NULL, NULL, -1, 1.0f, 0, -1, &mSound, 0x80000, sp2C);

    if (mpModelMorf[1] != NULL && mpModelMorf[1]->getModel() == NULL) {
        mpModelMorf[1] = NULL;
    }

    if (mpModelMorf[1] == NULL) {
        if (mpModelMorf[0] != NULL) {
            mpModelMorf[0]->stopZelAnime();
        }
        return 0;
    }

    model_p = mpModelMorf[1]->getModel();

    for (u16 i = 0; i < mdlData_p->getJointNum(); i++) {
        mdlData_p->getJointNodePointer(i)->setCallBack(ctrlJointCallBack);
    }

    model_p->setUserArea((uintptr_t)this);

    if (mType == 4) {
        J3DModelData* modelData = NULL;
        bmd_offset = 2;
        arc_idx = l_bmdData[bmd_offset][1];
        res_idx = l_bmdData[bmd_offset][0];
        modelData = static_cast<J3DModelData*>(dComIfG_getObjectRes(l_resNameList[arc_idx], res_idx));

        if (modelData != NULL) {
            mpPodModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
            if (mpPodModel == NULL) {
                return 0;
            }
        } else {
            return 0;
        }
    }

    if (setFaceMotionAnm(1, false)) {
        if (mType == 4) {
            J3DAnmTransform* bck_p = NULL;
            bck_p = getTrnsfrmKeyAnmP(l_resNameList[l_podBckData[1][1]], l_podBckData[1][0]);

            if (bck_p != NULL && mPodBck.init(bck_p, 1, 2, 1.0f, 0, -1, false))
            {
                mPodAnmFlags |= 0x41;
                return 1;
            }

            if (bck_p == NULL && l_podBckData[1][0] != -1) {
                return 0;
            }
        }

        if (setMotionAnm(0, 0.0f, 0)) {
            return 1;
        }
    }

    return 0;
}

int daNpc_Kn_c::Delete() {
    OS_REPORT("|%06d:%x|daNpc_Kn_c -> Delete\n", g_Counter.mCounter0, this);
    fopAcM_RegisterDeleteID(this, "NPC_KN");
    this->~daNpc_Kn_c();
    return 1;
}

int daNpc_Kn_c::Execute() {
    int rt = execute();
    setPrtcl();
    calcMagicBallPos();
    setSwordChargePtcl();
    setSe();
    return rt;
}

int daNpc_Kn_c::Draw() {
    if (mpMatAnm) {
        J3DModelData* modelData = mpModelMorf[0]->getModel()->getModelData();
        modelData->getMaterialNodePointer(getEyeballMaterialNo())->setMaterialAnm(mpMatAnm);
    }

    GXColorS10 tevcolor;
    tevcolor.r = 0;
    tevcolor.g = 0;
    tevcolor.b = 0;
    tevcolor.a = s16(mpHIO->m.alpha);
    return draw(
#if DEBUG
        chkAction(&daNpc_Kn_c::test),
#else
        FALSE,
#endif
        FALSE, mpHIO->m.common.real_shadow_size, &tevcolor, 0.0f, TRUE, FALSE, FALSE);
}

int daNpc_Kn_c::createHeapCallBack(fopAc_ac_c* i_this) {
    daNpc_Kn_c* a_this = ((daNpc_Kn_c*)i_this);
    return a_this->CreateHeap();
}

int daNpc_Kn_c::ctrlJointCallBack(J3DJoint* i_joint, int param_1) {
    if (param_1 == 0) {
        J3DModel* model = j3dSys.getModel();
        daNpc_Kn_c* a_this = (daNpc_Kn_c*)model->getUserArea();

        if (a_this != NULL) {
            a_this->ctrlJoint(i_joint, model);
        }
    }

    return 1;
}

u8 daNpc_Kn_c::getType() {
    u8 prm = fopAcM_GetParam(this);
    switch (prm & 0xFF) {
    case 1:
        return 0;
    case 2:
        return 1;
    case 3:
        return 2;
    case 4:
        return 3;
    case 5:
        return 4;
    case 6:
        return 5;
    case 7:
        return 6;
    default:
        return 7;
    };
}

u8 daNpc_Kn_c::getDivideNo() {
    u8 prm = (fopAcM_GetParam(this) & 0xFF00) >> 8;
    return prm;
}

u32 daNpc_Kn_c::getFlowNodeNo() {
    u16 node_no = home.angle.x;
    return (node_no == 0xFFFF) ? 0xFFFFFFFF : node_no;
}

u8 daNpc_Kn_c::getPath() {
    return (fopAcM_GetParam(this) & 0xFF00) >> 8;
}

BOOL daNpc_Kn_c::isDelete() {
    static s16 l_appearFlag[7] = {
        -1, 0x0153, 0x0152, 0x0154, 0x0155, 0x0156, 0x0157,
    };
    static s16 l_delFlag[7] = {
        0x0153, 0x0152, 0x0154, 0x0155, 0x0156, 0x0157, 0x0158,
    };

    switch (mType) {
    case 0:
        if (!daNpcT_chkEvtBit(0x153)) {
            return 0;
        }
        break;
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
        if (daNpcT_chkEvtBit(l_appearFlag[mType * 1]) && !daNpcT_chkEvtBit(l_delFlag[mType])) {
            return 0;
        }
    }

    return 1;
}

void daNpc_Kn_c::resetCol() {
    mCylCc.Set(mCcDCyl);
    mCylCc.SetStts(&mCcStts);
    mCylCc.SetTgHitCallback(tgHitCallBack);

    for (int i = 0; i < 2; i++) {
        mSphCc[i].Set(mCcDSph);
        mSphCc[i].SetStts(&mCcStts);
        mSphCc[i].SetAtSe(dCcD_SE_HARD_BODY);
        mSphCc[i].SetAtSpl(dCcG_At_Spl_UNK_1);
        mSphCc[i].SetAtType(AT_TYPE_800);
        mSphCc[i].OnAtSPrmBit(0xc);
        mSphCc[i].OffAtNoConHit();
        mSphCc[i].OffCoSetBit();
    }
}

void daNpc_Kn_c::reset() {
    initialize();
    memset(&mpTeachAction, 0, (intptr_t)&field_0x1754 - (intptr_t)&mpTeachAction);

    if (mpMatAnm != NULL) {
        mpMatAnm->initialize();
    }

    if (getPath() != 0xFF) {
        mPath.initialize();
        mPath.setPathInfo(getPath(), fopAcM_GetRoomNo(this), 0);
    }

    mStaffId = -1;
    setAngle(home.angle.y);

    field_0x15bd = 0;
    field_0x15be = 0;
    field_0x15cc = 0;
    field_0x15cd = 0;
    field_0x15ce = 0;
    field_0x15bc = 0;
    field_0x15af = 1;

    for (int i = 0; i < 3; i++) {
        mParticleMngr[i].mManager.init(&mAcch, 0.0f, 0.0f);
    }

    for (int i = 0; i < 3; i++) {
        field_0x15d4[i] = -1;
    }

    field_0x170c = 0;
    field_0x16f4.set(1.0f, 1.0f, 1.0f);
    field_0x170d = 1;
    field_0x170e = 0;

    field_0x15c0 = current.pos;

    if (mType <= 6 && (mDivideNo == 0 || mDivideNo > 2)) {
        field_0xe2c = 1;
        parentActorID =
            fopAcM_createChild(PROC_NPC_GWOLF, fopAcM_GetID(this), 0xFFFFFF02, &current.pos,
                               fopAcM_GetRoomNo(this), &current.angle, 0, -1, 0);
    }

    resetType();
}

void daNpc_Kn_c::resetType() {
    switch (mType) {
    case 0:
        mActionMode = 0;
        return;
    case 1:
        mActionMode = 4;
        return;
    case 2:
        mActionMode = 4;
        return;
    case 3:
        mActionMode = 4;
        return;
    case 4:
        mActionMode = 4;
        return;
    case 5:
        if (mDivideNo != 0 && mDivideNo <= 2) {
            mActionMode = 14;
            field_0x15af = 0;
        } else {
            mActionMode = 4;
        }
        break;
    case 6:
        if (mDivideNo != 0 && mDivideNo <= 2) {
            mActionMode = 20;
            field_0x15af = 0;
        } else {
            mActionMode = 4;
        }
        break;
    default:
        mActionMode = -1;
    }
}

void daNpc_Kn_c::setParam() {
    u32 new_flags = fopAc_AttnFlag_SPEAK_e | fopAc_AttnFlag_TALK_e;
    selectAction();
    srchActors();

    s16 talk_distance = mpHIO->m.common.talk_distance;
    s16 talk_angle = mpHIO->m.common.talk_angle;
    s16 attention_distance = mpHIO->m.common.attention_distance;
    s16 attention_angle = mpHIO->m.common.attention_angle;

    switch (mType) {
    case 0:
        attention_info.flags = setParamTeach01();
        mAcchCir.SetWallR(135.0f);
        mAcchCir.SetWallH(mpHIO->m.common.knee_length);
        break;
    case 1:
        if (mActionMode < 4) {
            attention_info.flags = setParamTeach01();
            mAcchCir.SetWallR(135.0f);
            mAcchCir.SetWallH(mpHIO->m.common.knee_length);
        } else {
            attention_info.flags = setParamTeach02();
        }
        break;
    case 2:
        if (mActionMode < 8) {
            attention_info.flags = setParamTeach02();
        } else {
            attention_info.flags = setParamTeach03();
        }
        break;
    case 3:
        if (mActionMode < 9) {
            attention_info.flags = setParamTeach03();
        } else {
            attention_info.flags = setParamTeach04();
        }
        break;
    case 4:
        if (mActionMode < 12) {
            attention_info.flags = setParamTeach04();
        } else {
            attention_info.flags = setParamTeach05();
        }
        break;
    case 5:
        attention_info.flags = setParamTeach06();
        break;
    case 6:
        attention_info.flags = setParamTeach07();
        break;
    default:
        attention_info.distances[fopAc_attn_LOCK_e] = daNpcT_getDistTableIdx(attention_distance, attention_angle);
        attention_info.distances[fopAc_attn_BATTLE_e] = daNpcT_getDistTableIdx(attention_distance, attention_angle);
        attention_info.distances[fopAc_attn_TALK_e] = attention_info.distances[fopAc_attn_LOCK_e];
        attention_info.distances[fopAc_attn_SPEAK_e] = daNpcT_getDistTableIdx(talk_distance, talk_angle);
        attention_info.flags = new_flags;

        mAcchCir.SetWallR(mpHIO->m.common.width);
        mAcchCir.SetWallH(mpHIO->m.common.knee_length);
    }

    scale.set(mpHIO->m.common.scale * field_0x16f4.x,
              mpHIO->m.common.scale * field_0x16f4.y,
              mpHIO->m.common.scale * field_0x16f4.z);

    mCcStts.SetWeight(mpHIO->m.common.weight);
    field_0xe0c = mpHIO->m.common.height;
    mRadius = mpHIO->m.common.width;
    gravity = mpHIO->m.common.gravity;
    mExpressionMorfFrame = mpHIO->m.common.expression_morf_frame;
    mMorfFrames = mpHIO->m.common.morf_frame;
};

u32 daNpc_Kn_c::setParamTeach03() {
    s16 talk_distance = mpHIO->m.common.talk_distance;
    s16 talk_angle = mpHIO->m.common.talk_angle;
    s16 attention_distance = mpHIO->m.common.attention_distance;
    s16 attention_angle = mpHIO->m.common.attention_angle;

    attention_info.distances[fopAc_attn_LOCK_e] = daNpcT_getDistTableIdx(attention_distance, attention_angle);
    attention_info.distances[fopAc_attn_BATTLE_e] = 228;
    attention_info.distances[fopAc_attn_TALK_e] = attention_info.distances[fopAc_attn_LOCK_e];
    attention_info.distances[fopAc_attn_SPEAK_e] = daNpcT_getDistTableIdx(talk_distance, talk_angle);

    mAcchCir.SetWallR(135.0f);
    mAcchCir.SetWallH(mpHIO->m.common.knee_length);
    return fopAc_AttnFlag_BATTLE_e;
}

u32 daNpc_Kn_c::setParamTeach04() {
    s16 talk_distance = mpHIO->m.common.talk_distance;
    s16 talk_angle = mpHIO->m.common.talk_angle;
    s16 attention_distance = mpHIO->m.common.attention_distance;
    s16 attention_angle = mpHIO->m.common.attention_angle;

    attention_info.distances[fopAc_attn_LOCK_e] = daNpcT_getDistTableIdx(attention_distance, attention_angle);
    attention_info.distances[fopAc_attn_BATTLE_e] = 228;
    attention_info.distances[fopAc_attn_TALK_e] = attention_info.distances[fopAc_attn_LOCK_e];
    attention_info.distances[fopAc_attn_SPEAK_e] = daNpcT_getDistTableIdx(talk_distance, talk_angle);

    if (mActionMode == 10 || mActionMode == 11) {
        mAcchCir.SetWallR(135.0f);
        mAcchCir.SetWallH(mpHIO->m.common.knee_length);
        return fopAc_AttnFlag_BATTLE_e;
    }

    return (fopAc_AttnFlag_UNK_0x200000 | fopAc_AttnFlag_BATTLE_e);
}

u32 daNpc_Kn_c::setParamTeach05() {
    s16 talk_distance = mpHIO->m.common.talk_distance;
    s16 talk_angle = mpHIO->m.common.talk_angle;
    s16 attention_distance = mpHIO->m.common.attention_distance;
    s16 attention_angle = mpHIO->m.common.attention_angle;

    attention_info.distances[fopAc_attn_LOCK_e] = daNpcT_getDistTableIdx(attention_distance, attention_angle);
    attention_info.distances[fopAc_attn_BATTLE_e] = 228;
    attention_info.distances[fopAc_attn_TALK_e] = attention_info.distances[fopAc_attn_LOCK_e];
    attention_info.distances[fopAc_attn_SPEAK_e] = daNpcT_getDistTableIdx(talk_distance, talk_angle);

    mAcchCir.SetWallR(135.0f);
    mAcchCir.SetWallH(mpHIO->m.common.knee_length);
    return fopAc_AttnFlag_BATTLE_e;
}

u32 daNpc_Kn_c::setParamTeach06() {
    return setParamTeach05();
}

u32 daNpc_Kn_c::setParamTeach07() {
    return setParamTeach05();
}

void daNpc_Kn_c::setAfterTalkMotion() {
    int index = 0;
    mFaceMotionSeqMngr.getNo();
    index = 1;
    mFaceMotionSeqMngr.setNo(index, -1.0, 0, 0);
}

void daNpc_Kn_c::srchActors() {
    switch (mType) {
        break;
    }
}

bool daNpc_Kn_c::evtTalk() {
    if (chkAction(&daNpc_Kn_c::talk)) {
        (this->*mpActionFunc)(NULL);
    } else {
        if (dComIfGp_event_chkTalkXY() == FALSE || dComIfGp_evmng_ChkPresentEnd()) {
            setAction(&daNpc_Kn_c::talk);
        }
    }

    return true;
}

BOOL daNpc_Kn_c::evtCutProc() {
    if (mDivideNo != 0 && mDivideNo <= 2) {
        return false;
    }

    BOOL rt = false;
    int staff_id = dComIfGp_getEventManager().getMyStaffId("Kn", this, -1);
    int act_idx;
    if (staff_id != -1) {
        mStaffId = staff_id;
        act_idx = dComIfGp_getEventManager().getMyActIdx(mStaffId, &mCutNameList[0], 21, 0, 0);

        if ((this->*mCutList[act_idx])(mStaffId)) {
            dComIfGp_getEventManager().cutEnd(mStaffId);
        }

        rt = true;
    }

    return rt;
}

void daNpc_Kn_c::action() {
    if (mpTeachAction != NULL) {
        if (mpActionFunc == mpTeachAction) {
            (this->*mpActionFunc)(NULL);
        } else {
            setAction(mpTeachAction);
        }
    }
}

void daNpc_Kn_c::beforeMove() {
    if (checkHide() || mNoDraw) {
        attention_info.flags = 0;
    }
}

void daNpc_Kn_c::afterMoved() {
    if (mGroundHeight != -G_CM3D_F_INF) {
        mTevStr.YukaCol = dComIfG_Bgsp().GetPolyColor(mGndChk);
        mTevStr.room_no = dComIfG_Bgsp().GetRoomId(mGndChk);
    }
}

void daNpc_Kn_c::setAttnPos() {
    cXyz pos(30.0f, 0.0f, 0.0f);

    mJntAnm.setParam(
        this, mpModelMorf[0]->getModel(), &pos, getBackboneJointNo(), getNeckJointNo(), getHeadJointNo(),
        mpHIO->m.common.body_angleX_min, mpHIO->m.common.body_angleX_max,
        mpHIO->m.common.body_angleY_min, mpHIO->m.common.body_angleY_max,
        mpHIO->m.common.head_angleX_min, mpHIO->m.common.head_angleX_max,
        mpHIO->m.common.head_angleY_min, mpHIO->m.common.head_angleY_max,
        mpHIO->m.common.neck_rotation_ratio, 0.0f, 0);

    f32 rad_val = cM_s2rad(mCurAngle.y - field_0xDB2.y);
    mJntAnm.calcJntRad(0.2f, 1.0f, rad_val);

    setMtx();

    J3DModel* model = mpModelMorf[1]->getModel();
    J3DModelData* modelData = model->getModelData();    // unused

    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::ZXYrotM(mCurAngle);
    mDoMtx_stack_c::scaleM(scale);
    model->setBaseTRMtx(mDoMtx_stack_c::get());
    model->setUserArea((uintptr_t)this);
    mpModelMorf[1]->modelCalc();

    mDoMtx_stack_c::copy(mpModelMorf[0]->getModel()->getAnmMtx(getHeadJointNo()));
    mDoMtx_stack_c::multVec(&pos, &eyePos);
    mJntAnm.setEyeAngleX(eyePos, 1.0f, 0);
    mJntAnm.setEyeAngleY(eyePos, mCurAngle.y, 1, 1.0f, 0);

    mDoMtx_stack_c::copy(mpModelMorf[0]->getModel()->getAnmMtx(getHeadJointNo()));
    mDoMtx_stack_c::multVecZero(&attention_info.position);
    attention_info.position.y += mpHIO->m.common.attention_offset;
    {
        // The following is to force dbg asm to use r31 for stack reg:
        int unused;
    }
}

void daNpc_Kn_c::setCollision() {
    cXyz centerPos;

    f32 height = 0.0f;
    f32 radius = 0.0f;
    if (field_0xe2c == 0 && field_0x15af != 0) {
        mCylCc.SetCoSPrm((mTwilight == TRUE) ? 0x69 : 0x79);

        switch (mType) {
        case 0:
            setCollisionTeach01();
            break;
        case 1:
            if (mActionMode < 4) {
                setCollisionTeach01();
            } else {
                setCollisionTeach02();
            }
            break;
        case 2:
            if (mActionMode < 8) {
                setCollisionTeach02();
            } else {
                setCollisionTeach03();
            }
            break;
        case 3:
            if (mActionMode < 9) {
                setCollisionTeach03();
            } else {
                setCollisionTeach04();
            }
            break;
        case 4:
            if (mActionMode < 12) {
                setCollisionTeach04();
            } else {
                setCollisionTeach05();
            }
            break;
        case 5:
            setCollisionTeach06();
            break;
        case 6:
            setCollisionTeach07();
        }

        mDoMtx_stack_c::copy(mpModelMorf[0]->getModel()->getAnmMtx(getBackboneJointNo()));
        mDoMtx_stack_c::multVecZero(&centerPos);
        centerPos.y = current.pos.y;

        height = mpHIO->m.common.height;
        radius = mpHIO->m.common.width;
        mCylCc.SetH(height);
        mCylCc.SetR(radius);
        mCylCc.SetC(centerPos);
        dComIfG_Ccsp()->Set(&mCylCc);
    }

    mCylCc.ClrCoHit();
    mCylCc.ClrTgHit();
    setCollisionSword();
}

void daNpc_Kn_c::setCollisionSword() {
    static f32 l_swordOffset[2] = {60.0f, 120.0f};

    if (mMotionSeqMngr.getNo() == 0x19 && mMotionSeqMngr.getStepNo() == 0 &&
        mMotionSeqMngr.checkEntryNewMotion() == 0)
    {
        field_0x15ce = 1;
    } else {
        field_0x15ce = 0;
    }

    for (int i = 0; i < 2; i++) {
        if (field_0xe2c == 0) {
            if (field_0x15ce && mpModelMorf[0]->getFrame() >= 30.0f && mpModelMorf[0]->getFrame() <= 40.0f) {
                cXyz pos(l_swordOffset[i], 0.0f, 0.0f);
                mSphCc[i].OnAtSetBit();

                mDoMtx_stack_c::copy(mpModelMorf[0]->getModel()->getAnmMtx(13));
                mDoMtx_stack_c::scaleM(scale);
                mDoMtx_stack_c::multVec(&pos, &pos);

                mSphCc[i].SetC(pos);
                mSphCc[i].SetR(30.0f);
                dComIfG_Ccsp()->Set(&mSphCc[i]);
            } else {
                mSphCc[i].OffAtSetBit();
            }
        }

        mSphCc[i].ClrAtHit();
    }
}

BOOL daNpc_Kn_c::checkCollisionSword() {
    int hit_type = false;

    for (int i = 0; i < 2; i++) {
        if (mSphCc[i].ChkAtShieldHit()) {
            hit_type = 1;
            break;
        }

        if (mSphCc[i].ChkAtHit()) {
            hit_type = 2;
        }
    }

    // If shield was hit, clear hit
    if (hit_type == 1) {
        for (int i = 0; i < 2; i++) {
            mSphCc[i].ClrAtHit();
        }
    }

    return hit_type == 2;
}

void daNpc_Kn_c::setCollisionTeach03() {
    mCylCc.SetTgType(0xFFFFFFFF);
    mCylCc.SetTgSPrm(0x13);
    mCylCc.OffTgNoAtHitInfSet();
    mCylCc.OnTgNoConHit();
    mCylCc.OnCoSetBit();
}

void daNpc_Kn_c::setCollisionTeach04() {
    switch (mActionMode) {
    case 9:
        if (dComIfGp_event_runCheck() && eventInfo.checkCommandDemoAccrpt()) {
            mCylCc.OffTgSetBit();
            mCylCc.OffCoSetBit();
        } else {
            mCylCc.SetTgType(0xFFFFFFFF);
            mCylCc.SetTgHitMark(CcG_Tg_UNK_MARK_2);
            mCylCc.SetTgSPrm(0x13);
            mCylCc.OnTgShield();
            mCylCc.OffTgNoAtHitInfSet();
            mCylCc.OnTgNoConHit();
        }
        break;
    case 11:
    case 10:
        mCylCc.SetTgType(0xFFFFFFFF);
        mCylCc.SetTgHitMark(CcG_Tg_UNK_MARK_1);
        mCylCc.SetTgSPrm(0x13);
        mCylCc.OffTgShield();
        mCylCc.OffTgNoAtHitInfSet();
        mCylCc.OnTgNoConHit();
    }
}

void daNpc_Kn_c::setCollisionTeach05() {
    mCylCc.SetTgType(0xFFFFFFFF);
    mCylCc.SetTgSPrm(0x13);
    mCylCc.OffTgNoAtHitInfSet();
    mCylCc.OnTgNoConHit();
    mCylCc.OnCoSetBit();
}

void daNpc_Kn_c::setCollisionTeach06() {
    setCollisionTeach05();
}

void daNpc_Kn_c::setCollisionTeach07() {
    setCollisionTeach05();
}

int daNpc_Kn_c::drawDbgInfo() {
#if DEBUG
    if (mpHIO->m.common.debug_info_ON) {
        dDbVw_drawSphereXlu(eyePos, 18.0f, (GXColor){0x80, 0x80, 0x80, 0xA0}, 1);
        if (mPath.getPathInfo() != NULL) {
            mPath.drawDbgInfo(attention_info.position.y, 4);
        }
    }
#endif

    return false;
}

void daNpc_Kn_c::drawOtherMdl() {
    J3DModel* model = mpModelMorf[1]->getModel();
    J3DModelData* model_data = model->getModelData();

    mTevStr.TevColor.r = 0;
    mTevStr.TevColor.g = 0;
    mTevStr.TevColor.b = 0;
    mTevStr.TevColor.a = 0;

    if (mTimerStart != 0 && mTimer != 0) {
        f32 col_strength = (f32)mTimer / (f32)mTimerStart;
        if (cM3d_IsZero(col_strength) == false) {
            mTevStr.TevColor.r = col_strength * 20.0f;
        }
    } else {
#if DEBUG
        if (chkAction(&daNpc_Kn_c::test)) {
            mTevStr.TevColor.g = 20;
        }
#endif
    }

    g_env_light.settingTevStruct(0, &current.pos, &mTevStr);
    g_env_light.setLightTevColorType_MAJI(model, &mTevStr);
    mpModelMorf[1]->entryDL();

    mShadowKey = dComIfGd_setShadow(mShadowKey, 1, model, &current.pos,
                                    mpHIO->m.common.real_shadow_size, 20.0f,
                                    current.pos.y, mGroundHeight, mGndChk, &mTevStr, 0, 1.0f,
                                    dDlst_shadowControl_c::getSimpleTex());

    if (field_0x15cd && mpPodModel != NULL) {
        mDoMtx_stack_c::copy(mpModelMorf[0]->getModel()->getAnmMtx(22));
        mDoMtx_stack_c::scaleM(scale);
        mpPodModel->setBaseTRMtx(mDoMtx_stack_c::get());

        g_env_light.setLightTevColorType_MAJI(mpPodModel, &mTevStr);
        J3DModelData* mdlData_p = mpPodModel->getModelData();

        if (mPodAnmFlags & 0x40) {
            mPodBck.entry(mdlData_p);
        }

        mDoExt_modelUpdateDL(mpPodModel);

        if (mPodAnmFlags & 0x40) {
            mPodBck.remove(mdlData_p);
        }

        dComIfGd_addRealShadow(mShadowKey, mpPodModel);
    }
}

void daNpc_Kn_c::drawGhost() {
    J3DModel* model = mpModelMorf[0]->getModel();
    g_env_light.settingTevStruct(7, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(model, &tevStr);

    if (dKy_darkworld_check()) {
        dComIfGd_setListDark();
        mInvisModel.entryDL(0);
        dComIfGd_setList();
    } else {
        mInvisModel.entryDL(0);
    }
}

bool daNpc_Kn_c::afterSetMotionAnm(int param_0, int i_mode, f32 param_2, int param_3) {
    (void) param_2;
    (void) param_3;
    mPodAnmFlags &= 0xFFFFFC30;
    J3DAnmTransform* bck_p = NULL;
    int i = 0;

    switch (param_0) {
    case 30:
        i = 1;
        break;
    case 31:
        i = 2;
        break;
    case 32:
        i = 3;
        break;
    case 33:
        i = 4;
        break;
    }

    if (l_podBckData[i][0] != -1) {
        bck_p = (J3DAnmTransform*)getTrnsfrmKeyAnmP(l_resNameList[l_podBckData[i][1]], l_podBckData[i][0]);
    }

    if (bck_p && mPodBck.init(bck_p, 1, i_mode, 1.0f, 0, -1, true)) {
        mPodAnmFlags |= 0x41;
        return true;
    }

    if (bck_p == NULL && l_podBckData[i][0] != -1) {
        return false;
    }

    return true;
}

int daNpc_Kn_c::selectAction() {
    mpTeachAction = NULL;

    if (mpHIO->m.common.debug_mode_ON) {
        mpTeachAction = &daNpc_Kn_c::test;
    } else {
        switch (mType) {
        case 0:
            teach01_selectAction();
            break;
        case 1:
            if (mActionMode < 4) {
                teach01_selectAction();
            } else {
                teach02_selectAction();
            }
            break;
        case 2:
            if (mActionMode < 8) {
                teach02_selectAction();
            } else {
                teach03_selectAction();
            }
            break;
        case 3:
            if (mActionMode == 4) {
                mpTeachAction = &daNpc_Kn_c::teach02_start;
            } else if (mActionMode < 9) {
                teach03_selectAction();
            } else {
                teach04_selectAction();
            }
            break;
        case 4:
            if (mActionMode == 4) {
                mpTeachAction = &daNpc_Kn_c::teach02_start;
            } else if (mActionMode < 12) {
                teach04_selectAction();
            } else {
                teach05_selectAction();
            }
            break;
        case 5:
            if (mActionMode == 4) {
                mpTeachAction = &daNpc_Kn_c::teach02_start;
            } else if (mActionMode < 13) {
                teach05_selectAction();
            } else {
                teach06_selectAction();
            }
            break;
        case 6:
            if (mActionMode == 4) {
                mpTeachAction = &daNpc_Kn_c::teach02_start;
            } else if (mActionMode < 19) {
                teach06_selectAction();
            } else {
                teach07_selectAction();
            }
            break;
        default:
            mpTeachAction = &daNpc_Kn_c::wait;
        }
    }

    return 1;
}

void daNpc_Kn_c::teach03_selectAction() {
    switch (mActionMode) {
    case 8:
        mpTeachAction = &daNpc_Kn_c::teach03_attackWait;
        return;
    }

    mpTeachAction = &daNpc_Kn_c::wait;
}

void daNpc_Kn_c::teach04_selectAction() {
    switch (mActionMode) {
    case 9:
        mpTeachAction = &daNpc_Kn_c::teach04_attackWait;
        return;
    case 10:
        mpTeachAction = &daNpc_Kn_c::teach04_headBreakWait;
        return;
    case 11:
        mpTeachAction = &daNpc_Kn_c::teach04_finishWait;
        return;
    }

    mpTeachAction = &daNpc_Kn_c::wait;
}

void daNpc_Kn_c::teach05_selectAction() {
    switch (mActionMode) {
    case 12:
        mpTeachAction = &daNpc_Kn_c::teach05_surpriseAttackWait;
        return;
    }

    mpTeachAction = &daNpc_Kn_c::wait;
}

void daNpc_Kn_c::teach06_selectAction() {
    switch (mActionMode) {
    case 13:
        mpTeachAction = &daNpc_Kn_c::teach06_superJumpWait;
        return;
    case 14:
        mpTeachAction = &daNpc_Kn_c::teach06_divideMove;
        return;
    case 15:
        mpTeachAction = &daNpc_Kn_c::teach06_superJumpWaitDivide;
        return;
    case 16:
        mpTeachAction = &daNpc_Kn_c::teach06_superJumpedDivide;
        return;
    case 17:
        mpTeachAction = &daNpc_Kn_c::teach06_warpDelete;
        return;
    case 18:
        mpTeachAction = &daNpc_Kn_c::teach06_waitDivide;
        return;
    }

    mpTeachAction = &daNpc_Kn_c::wait;
}

void daNpc_Kn_c::teach07_selectAction() {
    switch (mActionMode) {
    case 19:
        mpTeachAction = &daNpc_Kn_c::teach07_superTurnAttackWait;
        return;
    case 20:
        mpTeachAction = &daNpc_Kn_c::teach07_divideMove;
        return;
    case 21:
        mpTeachAction = &daNpc_Kn_c::teach07_superTurnAttackWaitDivide;
        return;
    case 22:
        mpTeachAction = &daNpc_Kn_c::teach07_superTurnAttackedDivide;
        return;
    case 23:
        mpTeachAction = &daNpc_Kn_c::teach07_warpDelete;
        return;
    case 24:
        mpTeachAction = &daNpc_Kn_c::teach07_waitDivide;
        return;
    }

    mpTeachAction = &daNpc_Kn_c::wait;
}

BOOL daNpc_Kn_c::chkAction(daNpc_Kn_c::actionFunc i_action) {
    return mpActionFunc == i_action;
}

BOOL daNpc_Kn_c::setAction(daNpc_Kn_c::actionFunc i_action) {
    mMode = 3;
    if (mpActionFunc) {
        (this->*mpActionFunc)(NULL);
    }

    mMode = 0;
    mpActionFunc = i_action;

    if (mpActionFunc) {
        (this->*mpActionFunc)(NULL);
    }

    return TRUE;
}

int daNpc_Kn_c::wait(void* param_0) {
    switch (mMode) {
    case 0:
    case 1:
        mFaceMotionSeqMngr.setNo(1, -1.0f, 0, 0);
        mMotionSeqMngr.setNo(0, -1.0f, 0, 0);
        mMode = 2;
    case 2:
        if (mTwilight == 0 && srchPlayerActor()) {
            mJntAnm.lookPlayer(0);
        } else {
            if (home.angle.y != mCurAngle.y && step(home.angle.y, 1, 0x20, 20, 0)) {
                mMode = 1;
                mFaceMotionSeqMngr.setNo(1, -1.0f, 0, 0);
                mMotionSeqMngr.setNo(0, -1.0f, 0, 0);
            }
            mJntAnm.lookNone(0);
        }
    case 3:
    default:
        return 1;
    }
}

int daNpc_Kn_c::talk(void* param_0) {
    int rt = 0;
    BOOL var_r30 = FALSE;

    switch (mMode) {
    case 0:
    case 1:
        initTalk(mFlowNodeNo, 0);
        mMode = 2;
    case 2:
        if (!mTwilight) {
            mJntAnm.lookPlayer(0);

            if (mCurAngle.y == fopAcM_searchPlayerAngleY(this)) {
                var_r30 = 1;
            } else {
                if (step(fopAcM_searchPlayerAngleY(this), 1, 0x20, 20, 0)) {
                    var_r30 = true;
                    mFaceMotionSeqMngr.setNo(1, -1.0f, 0, 0);
                    mMotionSeqMngr.setNo(0, -1.0f, 0, 0);
                }
            }
        } else {
            var_r30 = 1;
        }

        if (var_r30 && talkProc(0, 0, NULL, 0)) {
            mActorMngr1.entry(daPy_getPlayerActorClass());
            dComIfGp_event_reset();
            mMode = 3;
        }
        break;
    case 3:
    default:
        break;
    }

    return rt;
}

int daNpc_Kn_c::test(void* param_0) {
    int rt = 0;

    switch (mMode) {
    case 0:
    case 1:
        speedF = 0.0f;
        speed.setall(0.0f);
        mMode = 2;
    case 2:
        mFaceMotionSeqMngr.setNo(mpHIO->m.common.face_expression, -1.0f, 0, 0);
        mMotionSeqMngr.setNo(mpHIO->m.common.motion, -1.0f, 0, 0);
        mJntAnm.lookNone(0);
        attention_info.flags = 0;
        break;
    case 3:
    default:
        break;
    }

    return rt;
}

void daNpc_Kn_c::setSceneChange(int param_0) {
    for (int i = 2; i <= 7; i++) {
        if (daNpcT_chkTmpBit(i + 90)) {
            dStage_changeScene(i, 0.0, 0, fopAcM_GetRoomNo(this), 0, -1);

            if (param_0 != 0) {
                daNpcT_offTmpBit(i + 90);
                daNpcT_onEvtBit(i + 490);
            }

            return;
        }
    }

    dStage_changeScene(1, 0.0, 0, fopAcM_GetRoomNo(this), 0, -1);
}

int daNpc_Kn_c::ECut_noneEquipChangeTalk(int i_idx) {
    dEvent_manager_c* event_manager = &dComIfGp_getEventManager();
    int reg_r26 = 0;
    int rt = 0;
    int* prm_p = NULL;
    int prm = -1;

    prm_p = dComIfGp_evmng_getMyIntegerP(i_idx, "prm");

    if (prm_p != NULL) {
        prm = *prm_p;
    }

    if (event_manager->getIsAddvance(i_idx)) {
        switch (prm) {
        case 0:
            mJntAnm.lookPlayer(0);
            speed.zero();
            speedF = 0.0f;
            break;
        case 10:
            mStepMode = 2;
            initTalk(mFlowNodeNo, NULL);
            break;
        }
    }

    switch (prm) {
    case 0:
        if (mCurAngle.y != fopAcM_searchPlayerAngleY(this)) {
            if (step(fopAcM_searchPlayerAngleY(this), 1, 0x20, 20, 0)) {
                rt = 1;
                mFaceMotionSeqMngr.setNo(1, -1.0f, 0, 0);
                mMotionSeqMngr.setNo(0, -1.0f, 0, 0);
            }
        } else {
            rt = 1;
        }
        break;
    case 10:
        if (talkProc(NULL, 0, NULL, 0)) {
            rt = 1;
        }
        break;
    default:
        rt = 1;
    }

    return rt;
}

int daNpc_Kn_c::ECut_noneEquipChangeTalkStand(int i_idx) {
    dEvent_manager_c* event_manager = &dComIfGp_getEventManager();
    int reg_r26 = 0;
    int rt = 0;
    int* prm_p = NULL;
    int prm = -1;

    prm_p = dComIfGp_evmng_getMyIntegerP(i_idx, "prm");

    if (prm_p != NULL) {
        prm = *prm_p;
    }

    if (event_manager->getIsAddvance(i_idx)) {
        switch (prm) {
        case 0:
            mJntAnm.lookNone(0);
            mFaceMotionSeqMngr.setNo(1, -1.0f, 0, 0);
            mMotionSeqMngr.setNo(22, -1.0f, 0, 0);
            speed.zero();
            speedF = 0.0f;
            break;
        case 10:
            mJntAnm.lookPlayer(0);
            break;
        case 20:
            mStepMode = 2;
            initTalk(mFlowNodeNo, NULL);
            break;
        }
    }

    switch (prm) {
    case 0:
        if (mMotionSeqMngr.getNo() != 22) {
            break;
        }

        if (mMotionSeqMngr.getStepNo() > 0) {
            rt = 1;
        }
        break;
    case 10:
        if (mCurAngle.y != fopAcM_searchPlayerAngleY(this)) {
            if (step(fopAcM_searchPlayerAngleY(this), 1, 0x20, 20, 0)) {
                rt = 1;
                mFaceMotionSeqMngr.setNo(1, -1.0f, 0, 0);
                mMotionSeqMngr.setNo(0, -1.0f, 0, 0);
            }
        } else {
            rt = 1;
        }
        break;
    case 20:
        if (talkProc(NULL, 0, NULL, 0)) {
            rt = 1;
        }
        break;
    default:
        rt = 1;
    }

    return rt;
}

int daNpc_Kn_c::ECut_largeDamageTalk(int i_idx) {
    dEvent_manager_c* event_manager = &dComIfGp_getEventManager();
    int reg_r26 = 0;
    int rt = 0;
    int* prm_p = NULL;
    int prm = -1;

    prm_p = dComIfGp_evmng_getMyIntegerP(i_idx, "prm");

    if (prm_p != NULL) {
        prm = *prm_p;
    }

    if (event_manager->getIsAddvance(i_idx)) {
        switch (prm) {
        case 0:
            mJntAnm.lookPlayer(0);
            speed.zero();
            speedF = 0.0f;
            break;
        case 10:
            break;
        case 20:
            mStepMode = 2;
            initTalk(mFlowNodeNo, NULL);
            break;
        }
    }

    switch (prm) {
    case 0:
        if (!daPy_getPlayerActorClass()->checkPlayerFly()) {
            rt = 1;
        }

        daPy_getPlayerActorClass()->onLargeDamageUpStop();
        break;
    case 10:
        if (mCurAngle.y != fopAcM_searchPlayerAngleY(this)) {
            if (step(fopAcM_searchPlayerAngleY(this), 1, 0x20, 20, 0)) {
                rt = 1;
                mFaceMotionSeqMngr.setNo(1, -1.0f, 0, 0);
                mMotionSeqMngr.setNo(0, -1.0f, 0, 0);
            }
        } else {
            rt = 1;
        }

        daPy_getPlayerActorClass()->onLargeDamageUpStop();
        break;
    case 20:
        if (talkProc(NULL, 0, NULL, 0)) {
            rt = 1;
        }

        cLib_addCalcAngleS2(&mCurAngle.y, fopAcM_searchPlayerAngleY(this), 2, 0x800);
        setAngle(mCurAngle.y);
        daPy_getPlayerActorClass()->onLargeDamageUpStop();
        break;
    default:
        rt = 1;
    }

    return rt;
}

int daNpc_Kn_c::teach02_start(void* param_0) {
    switch (mMode) {
    case 0:
    case 1:
        mFaceMotionSeqMngr.setNo(1, -1.0f, 0, 0);
        mMotionSeqMngr.setNo(0, -1.0f, 0, 0);
        mJntAnm.lookPlayer(0);
        mMode = 2;
    case 2:
        mEvtNo = 8;
        field_0xe39 = 1;
        cLib_addCalcAngleS2(&mCurAngle.y, fopAcM_searchPlayerAngleY(this), 2, 0x800);
        setAngle(mCurAngle.y);
        break;
    case 3:
    default:
        break;
    }

    return 1;
}

int daNpc_Kn_c::teach03_attackWait(void* param_0) {
    switch (mMode) {
    case 0:
    case 1:
        mFaceMotionSeqMngr.setNo(1, -1.0f, 0, 0);
        mMotionSeqMngr.setNo(9, -1.0f, 0, 0);
        mJntAnm.lookPlayer(0);
        field_0xdec = mpHIO->m.attack_wait_time;
        mTargetPos = current.pos;
        field_0x15bc = 0;
        field_0x15d0 = cLib_getRndValue(mpHIO->m.attack_start_min_time, mpHIO->m.attack_start_time_range);
        speedF = 0;
        speed.zero();
        field_0x15cc = 0;
        mMode = 2;
        // fallthrough
    case 2: {
        mCcStts.Move();

        int cut_type = daPy_getPlayerActorClass()->getCutType();
        if (cut_type == daPy_py_c::CUT_TYPE_TWIRL) {
            mCylCc.SetTgHitMark(CcG_Tg_UNK_MARK_1);
            mCylCc.OffTgShield();
        } else {
            mCylCc.SetTgHitMark(CcG_Tg_UNK_MARK_2);
            mCylCc.OnTgShield();
        }

        if (mCylCc.ChkTgHit()) {
            if (cut_type == daPy_py_c::CUT_TYPE_TWIRL) {
                if (mType == 2) {
                    mEvtNo = 14;
                } else if (mType == 3) {
                    mEvtNo = 15;
                }

                s16 srch_ply_angle = fopAcM_searchPlayerAngleY(this);
                s16 angle = srch_ply_angle - current.angle.y;

                if ((angle < 0 ? -angle : angle) < 0x4000) {
                    mFaceMotionSeqMngr.setNo(1, -1.0f, 0, 0);
                    mMotionSeqMngr.setNo(0x12, -1.0f, 0, 0);

                    setAngle(fopAcM_searchPlayerAngleY(this));
                    speedF = -mpHIO->m.attack_disappear_speed_h;
                    mSound.startCreatureVoice(Z2SE_KN_V_DAMAGE_L, -1);
                } else {
                    mFaceMotionSeqMngr.setNo(1, -1.0f, 0, 0);
                    mMotionSeqMngr.setNo(0xe, -1.0f, 0, 0);

                    setAngle(fopAcM_searchPlayerAngleY(this) + 0x8000);
                    speedF = mpHIO->m.attack_disappear_speed_h;
                    mSound.startCreatureVoice(Z2SE_KN_V_DAMAGE_L, -1);
                }

                mJntAnm.lookNone(0);
                speed.y = mpHIO->m.attack_disappear_speed_v;
                mSound.startCollisionSE(Z2SE_HIT_SWORD, 0x1f);
            } else {
                setAngle(fopAcM_searchPlayerAngleY(this));
                mFaceMotionSeqMngr.setNo(1, -1.0f, 0, 0);
                mMotionSeqMngr.setNo(0x1b, -1.0f, 1, 0);

                if (cut_type == daPy_py_c::CUT_TYPE_GUARD_ATTACK) {
                    mSound.startCollisionSE(Z2SE_HIT_SHIELD_ATTACK, 0x28);
                } else {
                    mSound.startCollisionSE(Z2SE_HIT_SWORD, 0x28);
                }
            }
        } else {
            if (daPy_getPlayerActorClass()->checkSideStep()) {
                field_0x15cc = 1;
            }

            if (!cLib_calcTimer(&field_0xdec) || checkCollisionSword()) {
                mTargetPos = current.pos;
                field_0x15bc = 0;

                BOOL is_lockon = FALSE;
                dAttention_c* attention_p = dComIfGp_getAttention();
                if (attention_p->LockonTruth()) {
                    is_lockon = TRUE;
                }

                if (checkCollisionSword()) {
                    mEvtNo = 4;
                } else {
                    mEvtNo = 1;
                }

                mFaceMotionSeqMngr.setNo(1, -1.0f, 0, 0);
                mMotionSeqMngr.setNo(0, -1.0f, 1, 0);

                if (field_0x15cc) {
                    mFlowNodeNo = 0x2e4;
                } else if (is_lockon) {
                    mFlowNodeNo = 0x2e2;
                } else {
                    mFlowNodeNo = 0x2e3;
                }

                break;
            } else {
                calcSwordAttackMove(0);
            }
        }
        break;
    }
    case 3:
    default:
        break;
    }

    return 1;
}

int daNpc_Kn_c::teach04_attackWait(void* param_0) {
    switch (mMode) {
    case 0:
    case 1:
        mFaceMotionSeqMngr.setNo(1, -1.0f, 0, 0);
        mMotionSeqMngr.setNo(9, -1.0f, 0, 0);
        mJntAnm.lookPlayer(0);
        field_0xdec = mpHIO->m.attack_wait_time;
        mTargetPos = current.pos;
        field_0x15bc = 0;
        field_0x15d0 = cLib_getRndValue(mpHIO->m.attack_start_min_time, mpHIO->m.attack_start_time_range);
        speedF = 0;
        speed.zero();
        mMode = 2;
    case 2:
        calcSlip();
        mCcStts.Move();

        if (mCylCc.ChkTgShieldHit()) {
            if (daPy_getPlayerActorClass()->getCutType() == daPy_py_c::CUT_TYPE_GUARD_ATTACK) {
                fopAc_ac_c* aTgHitActor_p = mCylCc.GetTgHitAc();
                JUT_ASSERT(3715, NULL != aTgHitActor_p);

                s16 angle = cLib_targetAngleY(&aTgHitActor_p->current.pos, &current.pos);
                cXyz offset(0.0f, 0.0f, mpHIO->m.small_slide_distance);
                mDoMtx_stack_c::YrotS(angle);
                mDoMtx_stack_c::multVec(&offset, &mTargetPos);
                mTargetPos += current.pos;

                field_0x15bc = 1;
                mCylCc.ClrTgHit();
                mSound.startCollisionSE(Z2SE_HIT_SHIELD_ATTACK, 0x28);
                mActionMode = 10;
                break;
            }
        }

        if (cLib_calcTimer(&field_0xdec) == 0 || checkCollisionSword()) {
            BOOL is_lockon = FALSE;
            dAttention_c* attention_p = dComIfGp_getAttention();
            if (attention_p->LockonTruth()) {
                is_lockon = TRUE;
            }

            if (checkCollisionSword()) {
                mEvtNo = 4;
            } else {
                mEvtNo = 1;
            }

            mFaceMotionSeqMngr.setNo(1, -1.0f, 0, 0);
            mMotionSeqMngr.setNo(0, -1.0f, 1, 0);

            if (is_lockon) {
                mFlowNodeNo = 0x2f3;
            } else {
                mFlowNodeNo = 0x2f4;
            }

            break;
        } else {
            if (mCylCc.ChkTgHit()) {
                if (!mCylCc.GetTgHitObj()->ChkAtType(AT_TYPE_SHIELD_ATTACK)) {
                    mFaceMotionSeqMngr.setNo(1, -1.0f, 0, 0);
                    mMotionSeqMngr.setNo(0x1b, -1.0f, 1, 0);
                    mSound.startCollisionSE(Z2SE_HIT_SWORD, 0x28);
                    break;
                }
            }
            calcSwordAttackMove(1);
        }
        break;
    case 3:
    default:
        break;
    }

    return 1;
}

int daNpc_Kn_c::teach04_headBreakWait(void* param_0) {
    s16 angle;
    switch (mMode) {
    case 0:
    case 1:
        mFaceMotionSeqMngr.setNo(1, -1.0f, 0, 0);
        mMotionSeqMngr.setNo(0xb, -1.0f, 1, 0);
        mJntAnm.lookNone(0);
        field_0xdec = mpHIO->m.followup_wait_time;
        onHeadLockFlg();
        mMode = 2;
        // fallthrough
    case 2: {
        calcSlip();
        field_0x15c0.set(10.0f, -10.0f, 0.0f);

        int head_joint_no = getHeadJointNo();
        mDoMtx_stack_c::copy(mpModelMorf[0]->getModel()->getAnmMtx(head_joint_no));
        mDoMtx_stack_c::multVec(&field_0x15c0, &field_0x15c0);
        setHeadLockPos(&field_0x15c0);

        mCcStts.Move();

        if (mCylCc.ChkTgHit() && daPy_getPlayerActorClass()->getCutType() == daPy_py_c::CUT_TYPE_HEAD_JUMP) {
            angle = cLib_targetAngleY(&daPy_getPlayerActorClass()->current.pos, &attention_info.position);
            cXyz offset(0.0f, 0.0f, mpHIO->m.small_slide_distance);
            mDoMtx_stack_c::YrotS(angle);
            mDoMtx_stack_c::multVec(&offset, &mTargetPos);
            mTargetPos += current.pos;

            field_0x15bc = 1;
            mCylCc.ClrTgHit();
            mSound.startCollisionSE(Z2SE_HIT_SWORD, 0x1e);
            mActionMode = 11;
            break;
        } else {
            if (cLib_calcTimer(&field_0xdec) == 0) {
                mEvtNo = 1;
                mFaceMotionSeqMngr.setNo(1, -1.0f, 0, 0);
                mMotionSeqMngr.setNo(0, -1.0f, 1, 0);
                mFlowNodeNo = 0x2f2;
                mActionMode = 9;
            }
        }
        break;
    }
    case 3:
        offHeadLockFlg();
        break;
    default:
        break;
    }

    return 1;
}

int daNpc_Kn_c::teach04_finishWait(void* param_0) {
    switch (mMode) {
    case 0:
    case 1:
        mFaceMotionSeqMngr.setNo(1, -1.0f, 0, 0);
        mMotionSeqMngr.setNo(0xb, -1.0f, 1, 0);
        mJntAnm.lookNone(0);
        field_0xdec = mpHIO->m.followup_wait_time;
        mMode = 2;
    case 2:
        calcSlip();
        mCcStts.Move();

        if (mCylCc.ChkTgHit() && daPy_getPlayerActorClass()->getCutType() != daPy_py_c::CUT_TYPE_GUARD_ATTACK) {
            if (mType == 3) {
                mEvtNo = 16;
            } else {
                mEvtNo = 17;
            }

            s16 angle =
                (mCylCc.GetTgHitPosP() ? cLib_targetAngleY(&current.pos, mCylCc.GetTgHitPosP()) : fopAcM_searchPlayerAngleY(this))
                - current.angle.y;

            if ((angle < 0 ? -angle : angle) < 0x4000) {
                mFaceMotionSeqMngr.setNo(1, -1.0f, 0, 0);
                mMotionSeqMngr.setNo(0x12, -1.0f, 0, 0);

                setAngle(fopAcM_searchPlayerAngleY(this));
                speedF = -mpHIO->m.attack_disappear_speed_h;
                mSound.startCreatureVoice(Z2SE_KN_V_DAMAGE_L, -1);
            } else {
                mFaceMotionSeqMngr.setNo(1, -1.0f, 0, 0);
                mMotionSeqMngr.setNo(0xe, -1.0f, 0, 0);
                setAngle(fopAcM_searchPlayerAngleY(this) + 0x8000);
                speedF = mpHIO->m.attack_disappear_speed_h;
                mSound.startCreatureVoice(Z2SE_KN_V_DAMAGE_L, -1);
            }

            mSound.startCollisionSE(Z2SE_HIT_SWORD, 0x1f);
            mJntAnm.lookNone(0);
            speed.y = mpHIO->m.attack_disappear_speed_v;
            break;
        } else {
            if (cLib_calcTimer(&field_0xdec) == 0) {
                mEvtNo = 1;
                mFlowNodeNo = 0x306;
                mActionMode = 9;
                mFaceMotionSeqMngr.setNo(1, -1.0f, 0, 0);
                mMotionSeqMngr.setNo(0, -1.0f, 1, 0);
            }
        }
        break;
    case 3:
    default:
        break;
    }

    return 1;
}

int daNpc_Kn_c::teach05_surpriseAttackWait(void* param_0) {
    switch (mMode) {
    case 0:
    case 1:
        mFaceMotionSeqMngr.setNo(1, -1.0f, 0, 0);
        mMotionSeqMngr.setNo(9, -1.0f, 0, 0);
        mJntAnm.lookPlayer(0);
        field_0xdec = mpHIO->m.attack_wait_time;
        mTargetPos = current.pos;
        field_0x15bc = 0;
        field_0x15d0 = cLib_getRndValue(mpHIO->m.attack_start_min_time, mpHIO->m.attack_start_time_range);
        speedF = 0.0f;
        speed.zero();
        mMode = 2;
    case 2: {
        calcSlip();
        mCcStts.Move();

        int cut_type = daPy_getPlayerActorClass()->getCutType();
        if (cut_type == daPy_py_c::CUT_TYPE_MORTAL_DRAW_B ||
            cut_type == daPy_py_c::CUT_TYPE_MORTAL_DRAW_A)
        {
            mCylCc.SetTgHitMark(CcG_Tg_UNK_MARK_1);
            mCylCc.OffTgShield();
        } else {
            mCylCc.SetTgHitMark(CcG_Tg_UNK_MARK_2);
            mCylCc.OnTgShield();
        }

        if (mCylCc.ChkTgHit()) {
            if (cut_type == daPy_py_c::CUT_TYPE_MORTAL_DRAW_B ||
                cut_type == daPy_py_c::CUT_TYPE_MORTAL_DRAW_A)
            {
                if (mType == 4) {
                    mEvtNo = 0x13;
                } else {
                    mEvtNo = 0x14;
                }

                s16 srch_ply_angle = fopAcM_searchPlayerAngleY(this);
                s16 angle = srch_ply_angle - current.angle.y;

                if ((angle < 0 ? -angle : angle) < 0x4000) {
                    mFaceMotionSeqMngr.setNo(1, -1.0f, 0, 0);
                    mMotionSeqMngr.setNo(0x12, -1.0f, 0, 0);
                    setAngle(fopAcM_searchPlayerAngleY(this));

                    speedF = -mpHIO->m.attack_disappear_speed_h;
                    mSound.startCreatureVoice(Z2SE_KN_V_DAMAGE_L, -1);
                } else {
                    mFaceMotionSeqMngr.setNo(1, -1.0f, 0, 0);
                    mMotionSeqMngr.setNo(14, -1.0f, 0, 0);
                    setAngle(fopAcM_searchPlayerAngleY(this) + 0x8000);

                    speedF = mpHIO->m.attack_disappear_speed_h;
                    mSound.startCreatureVoice(Z2SE_KN_V_DAMAGE_L, -1);
                }

                mSound.startCollisionSE(Z2SE_HIT_SWORD, 0x1f);
                mJntAnm.lookNone(0);
                speed.y = mpHIO->m.attack_disappear_speed_v;
            } else {
                setAngle(fopAcM_searchPlayerAngleY(this));
                mFaceMotionSeqMngr.setNo(1, -1.0f, 0, 0);
                mMotionSeqMngr.setNo(0x1b, -1.0f, 1, 0);

                if (cut_type == daPy_py_c::CUT_TYPE_GUARD_ATTACK) {
                    mSound.startCollisionSE(Z2SE_HIT_SHIELD_ATTACK, 0x28);
                } else {
                    mSound.startCollisionSE(Z2SE_HIT_SWORD, 0x28);
                }
                break;
            }
        } else {
            if (daPy_getPlayerActorClass()->checkSideStep()) {
                field_0x15cc = 1;
            }

            calcSwordAttackMove(1);

            if (cLib_calcTimer(&field_0xdec) == 0 || checkCollisionSword()) {
                mTargetPos = current.pos;
                field_0x15bc = 0;

                BOOL is_lockon = FALSE;
                dAttention_c* attention_p = dComIfGp_getAttention();
                if (attention_p->LockonTruth()) {
                    is_lockon = TRUE;
                }

                if (checkCollisionSword()) {
                    mEvtNo = 4;
                } else {
                    mEvtNo = 1;
                }

                mFaceMotionSeqMngr.setNo(1, -1.0f, 0, 0);
                mMotionSeqMngr.setNo(0, -1.0f, 1, 0);

                if (is_lockon != FALSE) {
                    mFlowNodeNo = 0x2ec;

                } else {
                    if (!daPy_getPlayerActorClass()->checkItemSwordEquip()) {
                        mFlowNodeNo = 0x2ea;
                    } else {
                        mFlowNodeNo = 0x2eb;
                    }
                }
            }
        }
        break;
    }
    case 3:
    default:
        break;
    }

    return 1;
}

static void* s_sub6(void* i_actor, void* i_other) {
    if (fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_NPC_KN && i_actor != i_other) {
        ((daNpc_Kn_c*)i_actor)->setActionMode(18);
    }

    return NULL;
}

static void* s_subEnd6(void* i_actor, void* i_other) {
    if (fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_NPC_KN && i_actor != i_other &&
        ((daNpc_Kn_c*)i_actor)->getActionMode() != 17)
    {
        ((daNpc_Kn_c*)i_actor)->setActionMode(17);
    }

    return NULL;
}

int daNpc_Kn_c::teach06_superJumpWait(void* param_0) {
    switch (mMode) {
    case 0:
    case 1:
        mFaceMotionSeqMngr.setNo(1, -1.0f, 0, 0);
        mMotionSeqMngr.setNo(9, -1.0f, 0, 0);
        mJntAnm.lookPlayer(0);
        field_0xdec = mpHIO->m.attack_wait_time;
        mTargetPos = current.pos;
        field_0x15bc = 0;
        field_0x15d0 = cLib_getRndValue(mpHIO->m.attack_start_min_time, mpHIO->m.attack_start_time_range);
        speedF = 0.0f;
        speed.zero();
        field_0x15bd = 0;
        mMode = 2;
    case 2: {
        calcSlip();
        mCcStts.Move();

        int cut_type = daPy_getPlayerActorClass()->getCutType();
        if (cut_type == daPy_py_c::CUT_TYPE_LARGE_JUMP_INIT ||
            cut_type == daPy_py_c::CUT_TYPE_LARGE_JUMP ||
            cut_type == daPy_py_c::CUT_TYPE_LARGE_JUMP_FINISH)
        {
            mCylCc.SetTgHitMark(CcG_Tg_UNK_MARK_1);
            mCylCc.OffTgShield();
        } else {
            mCylCc.SetTgHitMark(CcG_Tg_UNK_MARK_2);
            mCylCc.OnTgShield();
        }

        if (mCylCc.ChkTgHit()) {
            if (cut_type == daPy_py_c::CUT_TYPE_LARGE_JUMP || cut_type == daPy_py_c::CUT_TYPE_LARGE_JUMP_FINISH) {
                if (mType == 5) {
                    mEvtNo = 0x15;
                } else {
                    mEvtNo = 0x17;
                }

                s16 srch_ply_angle = fopAcM_searchPlayerAngleY(this);
                s16 angle = srch_ply_angle - current.angle.y;

                if ((angle < 0 ? -angle : angle) < 0x4000) {
                    mFaceMotionSeqMngr.setNo(1, -1.0f, 0, 0);
                    mMotionSeqMngr.setNo(0x12, -1.0f, 0, 0);

                    setAngle(fopAcM_searchPlayerAngleY(this));
                    speedF = -mpHIO->m.attack_disappear_speed_h;
                    mSound.startCreatureVoice(Z2SE_KN_V_DAMAGE_L, -1);
                } else {
                    mFaceMotionSeqMngr.setNo(1, -1.0f, 0, 0);
                    mMotionSeqMngr.setNo(0xe, -1.0f, 0, 0);
                    setAngle(fopAcM_searchPlayerAngleY(this) + 0x8000);
                    speedF = mpHIO->m.attack_disappear_speed_h;
                    mSound.startCreatureVoice(Z2SE_KN_V_DAMAGE_L, -1);
                }
                mJntAnm.lookNone(0);
                speed.y = mpHIO->m.attack_disappear_speed_v;
                mSound.startCollisionSE(Z2SE_HIT_SWORD, 0x1f);
            } else if (cut_type == daPy_py_c::CUT_TYPE_LARGE_JUMP_INIT) {
                fopAc_ac_c* aTgHitActor_p = mCylCc.GetTgHitAc();
                JUT_ASSERT(4286, NULL != aTgHitActor_p);

                s16 angle = cLib_targetAngleY(&aTgHitActor_p->current.pos, &current.pos);
                cXyz offset(0.0f, 0.0f, mpHIO->m.small_slide_distance);
                mDoMtx_stack_c::YrotS(angle);
                mDoMtx_stack_c::multVec(&offset, &mTargetPos);
                mTargetPos += current.pos;

                field_0x15bc = 1;
                mCylCc.ClrTgHit();
                mFaceMotionSeqMngr.setNo(1, -1.0f, 0, 0);
                mMotionSeqMngr.setNo(0x1E, -1.0f, 1, 0);
                mSound.startCollisionSE(Z2SE_HIT_SWORD, 0x1E);
            } else {
                setAngle(fopAcM_searchPlayerAngleY(this));
                mFaceMotionSeqMngr.setNo(1, -1.0f, 0, 0);
                mMotionSeqMngr.setNo(0x1B, -1.0f, 1, 0);

                if (cut_type == daPy_py_c::CUT_TYPE_GUARD_ATTACK) {
                    mSound.startCollisionSE(Z2SE_HIT_SHIELD_ATTACK, 0x28);
                } else {
                    mSound.startCollisionSE(Z2SE_HIT_SWORD, 0x28);
                }
            }
        } else {
            if (field_0x15bd == 2 &&
                !(cut_type == daPy_py_c::CUT_TYPE_LARGE_JUMP || cut_type == daPy_py_c::CUT_TYPE_LARGE_JUMP_FINISH))
            {
                mEvtNo = 22;
                fpcM_Search(s_subEnd6, this);
            } else {
                calcSwordAttackMove(1);
                if (field_0x15bd != 2 && (cLib_calcTimer(&field_0xdec) == 0 || checkCollisionSword() || field_0x15bd == 1)) {
                    mTargetPos = current.pos;
                    field_0x15bc = 0;

                    BOOL is_lockon = FALSE;
                    dAttention_c* attention_p = dComIfGp_getAttention();
                    if (attention_p->LockonTruth()) {
                        is_lockon = TRUE;
                    }

                    if (checkCollisionSword() || field_0x15bd == 1) {
                        mEvtNo = 4;
                    } else {
                        mEvtNo = 1;
                    }

                    mFaceMotionSeqMngr.setNo(1, -1.0f, 0, 0);
                    mMotionSeqMngr.setNo(0, -1.0f, 1, 0);
                    fpcM_Search(s_sub6, this);
                } else if (!daPy_getPlayerActorClass()->checkDamageImpact()) {
                    BOOL is_lockon = FALSE;
                    dAttention_c* attention_p = dComIfGp_getAttention();
                    if (attention_p->LockonTruth()) {
                        is_lockon = TRUE;
                    }

                    if (daPy_getPlayerActorClass()->checkCutLargeJumpCharge()) {
                        mFlowNodeNo = 0x2fb;
                    } else {
                        if (is_lockon) {
                            mFlowNodeNo = 0x2fc;
                        } else {
                            mFlowNodeNo = 0x2fd;
                        }
                    }
                }
            }
        }
        break;
    }
    case 3:
    default:
        break;
    }

    return 1;
}

int daNpc_Kn_c::teach06_divideMove(void* param_0) {
    switch (mMode) {
    case 0:
    case 1: {
        mFaceMotionSeqMngr.setNo(1, -1.0f, 0, 0);
        mMotionSeqMngr.setNo(9, -1.0f, 0, 0);
        mJntAnm.lookPlayer(0);

        cXyz pos(0.0f, 0.0f, 180.0f);
        s16 angle;
        if (mDivideNo == 1) {
            angle = home.angle.y - 0x1555;
        } else {
            angle = home.angle.y + 0x1555;
        }

        mDoMtx_stack_c::transS(home.pos);
        mDoMtx_stack_c::YrotM(angle);
        mDoMtx_stack_c::multVec(&pos, &pos);
        mTargetPos = pos;
        mMode = 2;
    }
    case 2: {
        cXyz pos(current.pos);
        cLib_chasePos(&pos, mTargetPos, 6.0f);
        setPos(pos);

        cLib_addCalcAngleS2(&mCurAngle.y, fopAcM_searchPlayerAngleY(this), 2, 0x800);
        setAngle(mCurAngle.y);

        if (!dComIfGp_event_runCheck()) {
            mActionMode = 15;
        }
    }
    case 3:
    default:
        break;
    }

    return 1;
}

int daNpc_Kn_c::teach06_waitDivide(void* param_0) {
    switch (mMode) {
    case 0:
    case 1:
        mFaceMotionSeqMngr.setNo(1, -1.0f, 0, 0);
        mMotionSeqMngr.setNo(0, -1.0f, 0, 0);
        mJntAnm.lookPlayer(0);
        speedF = 0.0f;
        speed.zero();
        mMode = 2;
        break;
    case 2:
        cLib_addCalcAngleS2(&mCurAngle.y, fopAcM_searchPlayerAngleY(this), 2, 0x800);
        setAngle(mCurAngle.y);
        if (!dComIfGp_event_runCheck()) {
            mActionMode = 15;
        }
        break;
    case 3:
    default:
        break;
    }

    return 1;
}

int daNpc_Kn_c::teach06_superJumpWaitDivide(void* param_0) {
    switch (mMode) {
    case 0:
    case 1:
        mFaceMotionSeqMngr.setNo(1, -1.0f, 0, 0);
        mMotionSeqMngr.setNo(9, -1.0f, 0, 0);
        mJntAnm.lookPlayer(0);
        field_0x15af = 1;
        mTargetPos = current.pos;
        field_0x15bc = 0;
        field_0x15d0 = cLib_getRndValue<s16>(mpHIO->m.attack_start_min_time, mpHIO->m.attack_start_time_range);
        speedF = 0.0f;
        speed.zero();
        mMode = 2;
    case 2: {
        calcSlip();
        mCcStts.Move();

        int cut_type = daPy_getPlayerActorClass()->getCutType();
        if (cut_type == daPy_py_c::CUT_TYPE_LARGE_JUMP_INIT || cut_type == daPy_py_c::CUT_TYPE_LARGE_JUMP || cut_type == daPy_py_c::CUT_TYPE_LARGE_JUMP_FINISH) {
            mCylCc.SetTgHitMark(CcG_Tg_UNK_MARK_1);
            mCylCc.OffTgShield();
        } else {
            mCylCc.SetTgHitMark(CcG_Tg_UNK_MARK_2);
            mCylCc.OnTgShield();
        }

        if (mCylCc.ChkTgHit()) {
            if (cut_type == daPy_py_c::CUT_TYPE_LARGE_JUMP || cut_type == daPy_py_c::CUT_TYPE_LARGE_JUMP_FINISH) {
                mActionMode = 16;
                
                daNpc_Kn_c* parent_p = (daNpc_Kn_c*)fpcM_SearchByID(parentActorID);
                if (parent_p != NULL) {
                    parent_p->setTalkFlag(2);
                }

                s16 srch_ply_angle = fopAcM_searchPlayerAngleY(this);
                s16 angle = srch_ply_angle - current.angle.y;

                if ((angle < 0 ? -angle : angle) < 0x4000) {
                    mFaceMotionSeqMngr.setNo(1, -1.0f, 0, 0);
                    mMotionSeqMngr.setNo(0x12, -1.0f, 0, 0);
                    setAngle(fopAcM_searchPlayerAngleY(this));
                    speedF = -mpHIO->m.attack_disappear_speed_h;
                    mSound.startCreatureVoice(Z2SE_KN_V_DAMAGE_L, -1);
                } else {
                    mFaceMotionSeqMngr.setNo(1, -1.0f, 0, 0);
                    mMotionSeqMngr.setNo(0xE, -1.0f, 0, 0);
                    setAngle(fopAcM_searchPlayerAngleY(this) + 0x8000);
                    speedF = mpHIO->m.attack_disappear_speed_h;
                    mSound.startCreatureVoice(Z2SE_KN_V_DAMAGE_L, -1);
                }

                mJntAnm.lookNone(0);
                speed.y = mpHIO->m.attack_disappear_speed_v;
                mSound.startCollisionSE(Z2SE_HIT_SWORD, 0x1F);
            } else if (cut_type == daPy_py_c::CUT_TYPE_LARGE_JUMP_INIT) {
                fopAc_ac_c* aTgHitActor_p = mCylCc.GetTgHitAc();
                JUT_ASSERT(4619, NULL != aTgHitActor_p);

                s16 angle = cLib_targetAngleY(&aTgHitActor_p->current.pos, &current.pos);
                cXyz offset(0.0f, 0.0f, mpHIO->m.small_slide_distance);
                mDoMtx_stack_c::YrotS(angle);
                mDoMtx_stack_c::multVec(&offset, &mTargetPos);
                mTargetPos += current.pos;

                field_0x15bc = 1;
                mCylCc.ClrTgHit();
                mFaceMotionSeqMngr.setNo(1, -1.0f, 0, 0);
                mMotionSeqMngr.setNo(0x1E, -1.0f, 1, 0);
                mSound.startCollisionSE(Z2SE_HIT_SWORD, 0x1E);
            } else {
                setAngle(fopAcM_searchPlayerAngleY(this));
                mFaceMotionSeqMngr.setNo(1, -1.0f, 0, 0);
                mMotionSeqMngr.setNo(0x1B, -1.0f, 1, 0);

                if (cut_type == daPy_py_c::CUT_TYPE_GUARD_ATTACK) {
                    mSound.startCollisionSE(Z2SE_HIT_SHIELD_ATTACK, 0x28);
                } else {
                    mSound.startCollisionSE(Z2SE_HIT_SWORD, 0x28);
                }
            }
        } else {
            if (dComIfGp_event_runCheck()) {
                speedF = 0.0f;
                speed.zero();
            } else {
                calcSwordAttackMove(1);
            }

            if (checkCollisionSword()) {
                daNpc_Kn_c* parent_p = (daNpc_Kn_c*)fpcM_SearchByID(parentActorID);
                if (parent_p != NULL) {
                    parent_p->setTalkFlag(1);
                    parent_p->mFlowNodeNo = mFlowNodeNo;
                } else {
                    break;
                }
            } else if (!daPy_getPlayerActorClass()->checkDamageImpact()) {
                BOOL is_lockon = FALSE;
                dAttention_c* attention_p = dComIfGp_getAttention();
                if (attention_p->LockonTruth()) {
                    is_lockon = TRUE;
                }

                if (daPy_getPlayerActorClass()->checkCutLargeJumpCharge()) {
                    mFlowNodeNo = 0x2FB;
                } else if (is_lockon) {
                    mFlowNodeNo = 0x2FC;
                } else {
                    mFlowNodeNo = 0x2FD;
                }
            }
        }
        break;
    }
    case 3:
    default:
        break;
    }

    return 1;
}

int daNpc_Kn_c::teach06_superJumpedDivide(void* param_0) {
    switch (mMode) {
    case 0:
    case 1:
        mTargetPos = current.pos;
        field_0x15bc = 0;
        mMode = 2;
    case 2:
        if (mMotionSeqMngr.getNo() == 19 || mMotionSeqMngr.getNo() == 15) {
            if (mMotionSeqMngr.getStepNo() <= 0) {
                if (mpModelMorf[0]->getFrame() >= 13.0f && mpModelMorf[0]->getFrame() < 14.0f) {
                    setLandingPrtcl();
                }
            }
        } else {
            if (speed.y <= 0.0f && mAcch.ChkGroundHit()) {
                speedF = 0.0f;
                speed.zero();

                if (mMotionSeqMngr.getNo() == 18) {
                    mMotionSeqMngr.setNo(0x13, -1.0f, 0, 0);
                } else {
                    mMotionSeqMngr.setNo(15, -1.0f, 0, 0);
                }

                mFaceMotionSeqMngr.setNo(1, -1.0f, 0, 0);
                setLandingPrtcl();
            }
        }
        break;
    case 3:
    default:
        break;
    }

    return 1;
}

int daNpc_Kn_c::teach06_warpDelete(void* param_0) {
    switch (mMode) {
    case 0:
    case 1:
        field_0x170c = 1;
        mMode = 2;
    case 2:
        field_0x170c = 1;
        ctrlWarp();

        if (field_0x170c == 2) {
            fopAcM_delete(this);
        }
    case 3:
    default:
        break;
    }

    return 1;
}

static void* s_sub7(void* i_actor, void* i_other) {
    if (fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_NPC_KN && i_actor != i_other) {
        ((daNpc_Kn_c*)i_actor)->setActionMode(24);
    }

    return NULL;
}

static void* s_subEnd7(void* i_actor, void* i_other) {
    if (fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_NPC_KN && i_actor != i_other &&
        ((daNpc_Kn_c*)i_actor)->getActionMode() != 23)
    {
        ((daNpc_Kn_c*)i_actor)->setActionMode(23);
    }

    return NULL;
}

int daNpc_Kn_c::teach07_superTurnAttackWait(void* param_0) {
    switch (mMode) {
    case 0:
    case 1:
        mFaceMotionSeqMngr.setNo(1, -1.0f, 0, 0);
        mMotionSeqMngr.setNo(9, -1.0f, 0, 0);
        mJntAnm.lookPlayer(0);
        field_0xdec = mpHIO->m.attack_wait_time;
        mTargetPos = current.pos;
        field_0x15bc = 0;
        field_0x15d0 = cLib_getRndValue<s16>(mpHIO->m.attack_start_min_time, mpHIO->m.attack_start_time_range);
        speedF = 0.0f;
        speed.zero();
        field_0x15bd = 0;
        mMode = 2;
    case 2: {
        calcSlip();
        mCcStts.Move();

        int cut_type = daPy_getPlayerActorClass()->getCutType();
        if (cut_type == daPy_py_c::CUT_TYPE_LARGE_TURN_LEFT || cut_type == daPy_py_c::CUT_TYPE_LARGE_TURN_RIGHT) {
            mCylCc.SetTgHitMark(CcG_Tg_UNK_MARK_1);
            mCylCc.OffTgShield();
        } else {
            mCylCc.SetTgHitMark(CcG_Tg_UNK_MARK_2);
            mCylCc.OnTgShield();
        }

        if (mCylCc.ChkTgHit()) {
            if (cut_type == daPy_py_c::CUT_TYPE_LARGE_TURN_LEFT || cut_type == daPy_py_c::CUT_TYPE_LARGE_TURN_RIGHT) {
                if (mType == 6) {
                    mEvtNo = 0x18;
                }

                s16 srch_ply_angle = fopAcM_searchPlayerAngleY(this);
                s16 angle = srch_ply_angle - current.angle.y;

                if ((angle < 0 ? -angle : angle) < 0x4000) {
                    mFaceMotionSeqMngr.setNo(1, -1.0f, 0, 0);
                    mMotionSeqMngr.setNo(0x12, -1.0f, 0, 0);
                    setAngle(fopAcM_searchPlayerAngleY(this));
                    speedF = -mpHIO->m.attack_disappear_speed_h;
                    mSound.startCreatureVoice(Z2SE_KN_V_DAMAGE_L, -1);
                } else {
                    mFaceMotionSeqMngr.setNo(1, -1.0f, 0, 0);
                    mMotionSeqMngr.setNo(0xE, -1.0f, 0, 0);
                    setAngle(fopAcM_searchPlayerAngleY(this) + 0x8000);
                    speedF = mpHIO->m.attack_disappear_speed_h;
                    mSound.startCreatureVoice(Z2SE_KN_V_DAMAGE_L, -1);
                }

                mJntAnm.lookNone(0);
                speed.y = mpHIO->m.attack_disappear_speed_v;
                mSound.startCollisionSE(Z2SE_HIT_SWORD, 0x1F);
                break;
            } else {
                setAngle(fopAcM_searchPlayerAngleY(this));
                mFaceMotionSeqMngr.setNo(1, -1.0f, 0, 0);
                mMotionSeqMngr.setNo(0x1B, -1.0f, 1, 0);

                if (cut_type == daPy_py_c::CUT_TYPE_GUARD_ATTACK) {
                    mSound.startCollisionSE(Z2SE_HIT_SHIELD_ATTACK, 0x28);
                } else {
                    mSound.startCollisionSE(Z2SE_HIT_SWORD, 0x28);
                }
            }
        } else if (field_0x15bd == 2 &&
            !(cut_type == daPy_py_c::CUT_TYPE_LARGE_TURN_LEFT || cut_type == daPy_py_c::CUT_TYPE_LARGE_TURN_RIGHT))
        {
            mEvtNo = 0x19;
            fpcM_Search(s_subEnd7, this);
            break;
        }

        calcSwordAttackMove(1);

        if (field_0x15bd != 2 && (cLib_calcTimer(&field_0xdec) == 0 || checkCollisionSword() || field_0x15bd == 1)) {
            mTargetPos = current.pos;
            field_0x15bc = 0;

            BOOL is_lockon = FALSE;
            dAttention_c* attention_p = dComIfGp_getAttention();
            if (attention_p->LockonTruth()) {
                is_lockon = TRUE;
            }

            if (checkCollisionSword() || field_0x15bd == 1) {
                mEvtNo = 4;
            } else {
                mEvtNo = 1;
            }

            mFaceMotionSeqMngr.setNo(1, -1.0f, 0, 0);
            mMotionSeqMngr.setNo(0, -1.0f, 1, 0);
            mFlowNodeNo = 0x303;
            fpcM_Search(s_sub7, this);
        }
        break;
    }
    case 3:
    default:
        break;
    }

    return 1;
}

int daNpc_Kn_c::teach07_divideMove(void* param_0) {
    switch (mMode) {
    case 0:
    case 1:
        mFaceMotionSeqMngr.setNo(1, -1.0f, 0, 0);
        mMotionSeqMngr.setNo(9, -1.0f, 0, 0);
        mJntAnm.lookPlayer(0);

        if (mDivideNo == 1) {
            field_0x1710 = home.angle.y - 0x2AAA;
        } else {
            field_0x1710 = home.angle.y + 0x2AAA;
        }

        field_0x1712 = home.angle.y + 0x8000;

        mTargetPos = current.pos;
        field_0x15bc = 0;
        speedF = 0.0f;
        speed.zero();
        mMode = 2;
    case 2: {
        cLib_chaseAngleS(&field_0x1712, field_0x1710, 0x100);

        cXyz offset(0.0f, 0.0f, 375.0f);
        mDoMtx_stack_c::transS(daPy_getPlayerActorClass()->current.pos);
        mDoMtx_stack_c::YrotM(field_0x1712);
        mDoMtx_stack_c::multVec(&offset, &offset);
        setPos(offset);

        cLib_addCalcAngleS2(&mCurAngle.y, fopAcM_searchPlayerAngleY(this), 2, 0x800);
        setAngle(mCurAngle.y);

        if (!dComIfGp_event_runCheck()) {
            mActionMode = 21;
        }
    }
    case 3:
    default:
        break;
    }

    return 1;
}

int daNpc_Kn_c::teach07_waitDivide(void* param_0) {
    switch (mMode) {
    case 0:
    case 1:
        mFaceMotionSeqMngr.setNo(1, -1.0f, 0, 0);
        mMotionSeqMngr.setNo(0, -1.0f, 0, 0);
        mJntAnm.lookPlayer(0);
        speedF = 0.0f;
        speed.zero();
        mMode = 2;
        break;
    case 2:
        cLib_addCalcAngleS2(&mCurAngle.y, fopAcM_searchPlayerAngleY(this), 2, 0x800);
        setAngle(mCurAngle.y);

        if (!dComIfGp_event_runCheck()) {
            mActionMode = 21;
        }
    case 3:
    default:
        break;
    }

    return 1;
}

int daNpc_Kn_c::teach07_superTurnAttackWaitDivide(void* param_0) {
    switch (mMode) {
    case 0:
    case 1:
        mFaceMotionSeqMngr.setNo(1, -1.0f, 0, 0);
        mMotionSeqMngr.setNo(9, -1.0f, 0, 0);
        mJntAnm.lookPlayer(0);
        field_0x15af = 1;
        mTargetPos = current.pos;
        field_0x15bc = 0;
        field_0x15d0 = cLib_getRndValue<s16>(mpHIO->m.attack_start_min_time, mpHIO->m.attack_start_time_range);
        speedF = 0.0f;
        speed.zero();
        mMode = 2;
    case 2: {
        calcSlip();
        mCcStts.Move();

        int cut_type = daPy_getPlayerActorClass()->getCutType();
        if (cut_type == daPy_py_c::CUT_TYPE_LARGE_TURN_LEFT || cut_type == daPy_py_c::CUT_TYPE_LARGE_TURN_RIGHT) {
            mCylCc.SetTgHitMark(CcG_Tg_UNK_MARK_1);
            mCylCc.OffTgShield();
        } else {
            mCylCc.SetTgHitMark(CcG_Tg_UNK_MARK_2);
            mCylCc.OnTgShield();
        }

        if (mCylCc.ChkTgHit()) {
            if (cut_type == daPy_py_c::CUT_TYPE_LARGE_TURN_LEFT || cut_type == daPy_py_c::CUT_TYPE_LARGE_TURN_RIGHT) {
                mActionMode = 22;
                
                daNpc_Kn_c* parent_p = (daNpc_Kn_c*)fpcM_SearchByID(parentActorID);
                if (parent_p != NULL) {
                    parent_p->setTalkFlag(2);
                }

                s16 srch_ply_angle = fopAcM_searchPlayerAngleY(this);
                s16 angle = srch_ply_angle - current.angle.y;

                if ((angle < 0 ? -angle : angle) < 0x4000) {
                    mFaceMotionSeqMngr.setNo(1, -1.0f, 0, 0);
                    mMotionSeqMngr.setNo(0x12, -1.0f, 0, 0);
                    setAngle(fopAcM_searchPlayerAngleY(this));
                    speedF = -mpHIO->m.attack_disappear_speed_h;
                    mSound.startCreatureVoice(Z2SE_KN_V_DAMAGE_L, -1);
                } else {
                    mFaceMotionSeqMngr.setNo(1, -1.0f, 0, 0);
                    mMotionSeqMngr.setNo(0xE, -1.0f, 0, 0);
                    setAngle(fopAcM_searchPlayerAngleY(this) + 0x8000);
                    speedF = mpHIO->m.attack_disappear_speed_h;
                    mSound.startCreatureVoice(Z2SE_KN_V_DAMAGE_L, -1);
                }

                mJntAnm.lookNone(0);
                speed.y = mpHIO->m.attack_disappear_speed_v;
                mSound.startCollisionSE(Z2SE_HIT_SWORD, 0x1F);
                break;
            } else {
                setAngle(fopAcM_searchPlayerAngleY(this));
                mFaceMotionSeqMngr.setNo(1, -1.0f, 0, 0);
                mMotionSeqMngr.setNo(0x1B, -1.0f, 1, 0);

                if (cut_type == daPy_py_c::CUT_TYPE_GUARD_ATTACK) {
                    mSound.startCollisionSE(Z2SE_HIT_SHIELD_ATTACK, 0x28);
                } else {
                    mSound.startCollisionSE(Z2SE_HIT_SWORD, 0x28);
                }
            }
        }

        if (dComIfGp_event_runCheck()) {
            speedF = 0.0f;
            speed.zero();
        } else {
            calcSwordAttackMove(1);
        }

        if (checkCollisionSword()) {
            daNpc_Kn_c* parent_p = (daNpc_Kn_c*)fpcM_SearchByID(parentActorID);
            if (parent_p != NULL) {
                parent_p->setTalkFlag(1);
            }
            break;
        }
    }
    case 3:
    default:
        break;
    }

    return 1;
}

int daNpc_Kn_c::teach07_superTurnAttackedDivide(void* param_0) {
    switch (mMode) {
    case 0:
    case 1:
        mTargetPos = current.pos;
        field_0x15bc = 0;
        mMode = 2;
    case 2:
        if (mMotionSeqMngr.getNo() == 19 || mMotionSeqMngr.getNo() == 15) {
            if (mMotionSeqMngr.getStepNo() <= 0) {
                if (mpModelMorf[0]->getFrame() >= 13.0f && mpModelMorf[0]->getFrame() < 14.0f) {
                    setLandingPrtcl();
                }
            }
        } else {
            if (speed.y <= 0.0f && mAcch.ChkGroundHit()) {
                speedF = 0.0f;
                speed.zero();

                if (mMotionSeqMngr.getNo() == 18) {
                    mMotionSeqMngr.setNo(0x13, -1.0f, 0, 0);
                } else {
                    mMotionSeqMngr.setNo(15, -1.0f, 0, 0);
                }

                mFaceMotionSeqMngr.setNo(1, -1.0f, 0, 0);
                setLandingPrtcl();
            }
        }
        break;
    case 3:
    default:
        break;
    }

    return 1;
}

int daNpc_Kn_c::teach07_warpDelete(void* param_0) {
    switch (mMode) {
    case 0:
    case 1:
        field_0x170c = 1;
        mMode = 2;
    case 2:
        field_0x170c = 1;
        ctrlWarp();
        if (field_0x170c == 2) {
            fopAcM_delete(this);
        }
    case 3:
    default:
        break;
    }

    return 1;
}

int daNpc_Kn_c::ECut_secondEncount(int i_idx) {
    dEvent_manager_c* event_manager = &dComIfGp_getEventManager();
    daNpc_GWolf_c* gwolf_p = NULL;
    int rt = 0;
    int* prm_p = NULL;
    int prm = -1;

    prm_p = dComIfGp_evmng_getMyIntegerP(i_idx, "prm");
    if (prm_p != NULL) {
        prm = *prm_p;
    }

    if (event_manager->getIsAddvance(i_idx)) {
        switch (prm) {
        case 0:
            mFaceMotionSeqMngr.setNo(1, -1.0f, 0, 0);
            mMotionSeqMngr.setNo(0, -1.0f, 0, 0);
            break;
        case 2: {
            gwolf_p = (daNpc_GWolf_c*)fpcM_SearchByID(parentActorID);
            if (fopAcM_GetName(gwolf_p) == PROC_NPC_GWOLF) {
                gwolf_p->setMotion(4, -1.0f, FALSE);
            }
            break;
        }
        case 5: {
            gwolf_p = (daNpc_GWolf_c*)fpcM_SearchByID(parentActorID);
            if (gwolf_p != NULL) {
                fopAcM_delete(gwolf_p);
            }

            field_0xe2c = 0;
            break;
        }
        case 10:
            switch (mType) {
            case 1:
                initTalk(0x2bd, NULL);
                break;
            case 2:
                initTalk(0x2de, NULL);
                break;
            case 3:
                initTalk(0x2ee, NULL);
                break;
            case 4:
                initTalk(0x2e6, NULL);
                break;
            case 5:
                initTalk(0x2f7, NULL);
                break;
            case 6:
                initTalk(0x2ff, NULL);
                break;
            }
            break;
        case 20:
            field_0xdec = 30;
            break;
        }
    }

    switch (prm) {
    case 0:
        rt = 1;
        break;
    case 2:
        rt = 1;
        break;
    case 5:
        rt = 1;
        break;
    case 10:
        if (talkProc(0, 0, NULL, 0)) {
            int choiceNo = mFlow.getChoiceNo();
            if (choiceNo == 0) {
                int sp8 = 0;
                switch (mType) {
                case 1:
                    if (mFlow.getEventId(&sp8) == 0x1A && sp8 == 0) {
                        rt = 1;
                    } else {
                        setSceneChange(0);
                    }
                    break;
                case 2:
                    if (mFlow.getEventId(&sp8) == 0x1A) {
                        if (sp8 == 1) {
                            rt = 1;
                        } else if (sp8 == 2) {
                            mEvtNo = 0xD;
                            evtChange();
                        }
                    } else {
                        setSceneChange(0);
                    }
                    break;
                case 3:
                    if (mFlow.getEventId(&sp8) == 0x1A && sp8 == 3) {
                        rt = 1;
                    } else {
                        setSceneChange(0);
                    }
                    break;
                case 4:
                    if (mFlow.getEventId(&sp8) == 0x1A) {
                        if (sp8 == 4) {
                            rt = 1;
                        } else if (sp8 == 5) {
                            mEvtNo = 0x12;
                            evtChange();
                        }
                    } else {
                        setSceneChange(0);
                    }
                    break;
                default:
                    rt = 1;
                    break;
                }
            } else {
                setSceneChange(0);
            }
        }
        break;
    case 20:
        if (cLib_calcTimer(&field_0xdec) == 0) {
            rt = 1;
            switch (mType) {
            case 1:
                mActionMode = 2;
                break;
            case 2:
                mActionMode = 5;
                break;
            case 3:
                mActionMode = 8;
                break;
            case 4:
                mActionMode = 9;
                break;
            case 5:
                mActionMode = 12;
                break;
            case 6:
                mActionMode = 13;
                break;
            }
        }
        break;
    default:
        rt = 1;
    }

    return rt;
}

int daNpc_Kn_c::ECut_thirdSkillExplain(int i_idx) {
    dEvent_manager_c* event_manager = &dComIfGp_getEventManager();

    void* _ = NULL;
    int rt = 0;
    int* prm_p = NULL;
    int prm = -1;

    prm_p = dComIfGp_evmng_getMyIntegerP(i_idx, "prm");
    if (prm_p != NULL) {
        prm = *prm_p;
    }

    if (event_manager->getIsAddvance(i_idx)) {
        switch (prm) {
        case 0:
            mFaceMotionSeqMngr.setNo(1, -1.0f, 0, 0);
            mMotionSeqMngr.setNo(28, -1.0f, 1, 0);
            mJntAnm.lookPlayer(0);
            field_0xdec = 0x1e;
            break;
        case 10:
            mStepMode = 2;
            initTalk(0x2df, 0);
            break;
        case 20:
            Z2GetAudioMgr()->bgmStart(Z2BGM_OUGI_TRAINING, 0, 0);
        case 65: {
            setPos(home.pos);
            setAngle(home.angle.y);

            cXyz pos(0.0f, 0.0f, 375.0f);
            mDoMtx_stack_c::transS(home.pos);
            mDoMtx_stack_c::YrotM(home.angle.y);
            mDoMtx_stack_c::multVec(&pos, &pos);

            cXyz pos2 = pos;
            pos2.y += 500.0f;
            if (fopAcM_gc_c::gndCheck(&pos2)) {
                pos.y = fopAcM_gc_c::getGroundY();
            }

            daPy_getPlayerActorClass()->setPlayerPosAndAngle(&pos, fopAcM_searchPlayerAngleY(this) + 0x8000, 0);
            daPy_getPlayerActorClass()->onTraningCutBack();
            break;
        }
        case 30:
            initTalk(0x2e0, NULL);
            break;
        case 40:
            daNpcT_offTmpBit(0xb);
            initTalk(0x2e1, NULL);
            break;
        case 50:
            mFaceMotionSeqMngr.setNo(1, -1.0f, 0, 0);
            mMotionSeqMngr.setNo(17, -1.0f, 1, 0);
            break;
        case 55:
            field_0xdec = 30;
            break;
        case 60:
            initTalk(0x2e1, NULL);
            break;
        case 70:
            mFaceMotionSeqMngr.setNo(1, -1.0f, 0, 0);
            mMotionSeqMngr.setNo(24, -1.0f, 1, 0);
            mJntAnm.lookNone(0);
            break;
        }
    }

    switch (prm) {
    case 0:
        if (cLib_calcTimer(&field_0xdec) == 0) {
            if (mCurAngle.y == fopAcM_searchPlayerAngleY(this)) {
                rt = 1;
                mTargetPos = current.pos;
                field_0x15bc = 0;
                speedF = 0.0f;
                speed.zero();
            } else {
                if (step(fopAcM_searchPlayerAngleY(this), 1, 0x20, 20, 0)) {
                    mFaceMotionSeqMngr.setNo(1, -1.0f, 0, 0);
                    mMotionSeqMngr.setNo(0, -1.0f, 0, 0);
                }
            }
        } else {
            calcSlip();
        }
        break;
    case 6:
        if (mCurAngle.y != fopAcM_searchPlayerAngleY(this)) {
            if (step(fopAcM_searchPlayerAngleY(this), 1, 0x20, 20, 0)) {
                rt = 1;
                mFaceMotionSeqMngr.setNo(1, -1.0f, 0, 0);
                mMotionSeqMngr.setNo(0, -1.0f, 0, 0);
            }
            break;
        }
        rt = 1;
        break;
    case 10:
        if (talkProc(0, 0, NULL, 0)) {
            rt = 1;
        }
        break;
    case 20:
    case 65: {
        rt = 1;
        setPos(home.pos);
        setAngle(home.angle.y);

        cXyz pos(0.0f, 0.0f, 375.0f);
        mDoMtx_stack_c::transS(home.pos);
        mDoMtx_stack_c::YrotM(home.angle.y);
        mDoMtx_stack_c::multVec(&pos, &pos);

        cXyz pos2(pos);
        pos2.y += 500.0f;
        if (fopAcM_gc_c::gndCheck(&pos2)) {
            pos.y = fopAcM_gc_c::getGroundY();
        }

        daPy_getPlayerActorClass()->setPlayerPosAndAngle(&pos, fopAcM_searchPlayerAngleY(this) + 0x8000, 0);
        break;
    }
    case 30:
    case 40:
        if (talkProc(0, 0, NULL, 0)) {
            rt = 1;
        }
        break;
    case 50:
        if (mMotionSeqMngr.getNo() == 17) {
            if (mMotionSeqMngr.getStepNo() >= 3) {
                rt = 1;
            } else {
                cXyz pos(0.0f, 0.0f, 0.0f);
                s16 local_98 = 0;

                if (mMotionSeqMngr.getStepNo() == 0 && !(mpModelMorf[0]->getFrame() <= 4) &&
                    mpModelMorf[0]->getFrame() <= 14)
                {
                    pos.set(13.0f, 0.0f, 20.0f);

                    local_98 = cM_deg2s(-3.8f);
                }

                if (mMotionSeqMngr.getStepNo() == 1) {
                    if (mpModelMorf[0]->getFrame() <= 10) {
                        pos.set(3.77f, 0.0f, 19.05f);
                        local_98 = cM_deg2s(-6.76f);
                    } else {
                        pos.set(-13.66f, 0.0f, 19.05f);
                        local_98 = cM_deg2s(-6.76f);
                    }
                }

                mDoMtx_stack_c::YrotS(home.angle.y);
                mDoMtx_stack_c::multVec(&pos, &pos);
                pos += current.pos;
                setPos(pos);
                setAngle(current.angle.y + local_98);
            }
        }
        break;
    case 60:
        if (talkProc(0, 0, NULL, 0)) {
            rt = 1;
        }
        break;
    case 70:
        if (mMotionSeqMngr.getNo() == 24 && mMotionSeqMngr.getStepNo() > 0) {
            rt = 1;
            mActionMode = 8;
        }
        break;
    default:
        rt = 1;
    }

    return rt;
}

int daNpc_Kn_c::ECut_thirdSkillGet(int i_idx) {
    dEvent_manager_c* event_manager = &dComIfGp_getEventManager();
    int sp8 = 0;

    int rt = 0;
    int* prm_p = NULL;
    int prm = -1;

    prm_p = dComIfGp_evmng_getMyIntegerP(i_idx, "prm");
    if (prm_p != NULL) {
        prm = *prm_p;
    }

    if (event_manager->getIsAddvance(i_idx)) {
        switch (prm) {
        case 0: 
            daNpcT_offTmpBit(0xB);
            daNpcT_offTmpBit(0xC);
            break;
        case 4: 
            mFaceMotionSeqMngr.setNo(1, -1.0f, 0, 0);
            if (mMotionSeqMngr.getNo() == 0x13) {
                mMotionSeqMngr.setNo(0x16, -1.0f, 0, 0);
            } else {
                mMotionSeqMngr.setNo(0x10, -1.0f, 0, 0);
            }
            break;
        case 6: 
            mJntAnm.lookNone(0);
            break;
        case 10: {
            mStepMode = 2;

            cXyz sp1C(0.0f, 0.0f, 700.0f);
            mDoMtx_stack_c::transS(current.pos);
            mDoMtx_stack_c::YrotM(cLib_targetAngleY(&current.pos, &home.pos));
            mDoMtx_stack_c::multVec(&sp1C, &sp1C);

            cXyz sp10(sp1C);
            sp10.y += 500.0f;
            if (fopAcM_gc_c::gndCheck(&sp10)) {
                sp1C.y = fopAcM_gc_c::getGroundY();
            }

            daPy_getPlayerActorClass()->setPlayerPosAndAngle(&sp1C, (s16)cLib_targetAngleY(&home.pos, &current.pos), 0);
            initTalk(0x2E5, NULL);
            break;
        }
        case 12:
            setAngle(fopAcM_searchPlayerAngleY(this));
            break;
        case 15:
            Z2GetAudioMgr()->bgmStreamPrepare(0x2000038);
            Z2GetAudioMgr()->bgmStreamPlay();
            break;
        case 20:
            initTalk(0x2E5, NULL);
            break;
        case 30:
            initTalk(0x2E5, NULL);
            break;
        }
    }
    switch (prm) {
    case 0: 
        if (speed.y <= 0.0f && mAcch.ChkGroundHit()) {
            speedF = 0.0f;
            speed.zero();

            if (mMotionSeqMngr.getNo() == 0x12) {
                mMotionSeqMngr.setNo(0x13, -1.0f, 0, 0);
            } else {
                mMotionSeqMngr.setNo(0xF, -1.0f, 0, 0);
            }

            mFaceMotionSeqMngr.setNo(1, -1.0f, 0, 0);
            setLandingPrtcl();
            rt = 1;
        }
        break;
    case 2: 
        if (mMotionSeqMngr.getNo() == 0x13 || mMotionSeqMngr.getNo() == 0xF) {
            if (mMotionSeqMngr.getStepNo() > 0) {
                rt = 1;
            } else if (mpModelMorf[0]->getFrame() >= 13.0f && mpModelMorf[0]->getFrame() < 14.0f) {
                setLandingPrtcl();
            }
        }
        break;
    case 4: 
        if ((mMotionSeqMngr.getNo() == 0x16 || mMotionSeqMngr.getNo() == 0x10) && mMotionSeqMngr.getStepNo() > 0) {
            rt = 1;
        }
        break;
    case 6: 
        if (mCurAngle.y != fopAcM_searchPlayerAngleY(this)) {
            if (step(fopAcM_searchPlayerAngleY(this), 1, 0x20, 20, 0)) {
                rt = 1;
                mFaceMotionSeqMngr.setNo(1, -1.0f, 0, 0);
                mMotionSeqMngr.setNo(0, -1.0f, 0, 0);
            }
        } else {
            rt = 1;
        }
        break;
    case 10:
        if (talkProc(NULL, 0, NULL, 0)) {
            rt = 1;
            if (field_0x170e == 0) {
                field_0x170e = 1;
                Z2GetAudioMgr()->bgmStop(0x32, 0);
            }
        }
        break;
    case 20:
        if (talkProc(NULL, 0, NULL, 0)) {
            rt = 1;
        }
        cLib_addCalcAngleS2(&mCurAngle.y, fopAcM_searchPlayerAngleY(this), 2, 0x800);
        setAngle(mCurAngle.y);
        break;
    case 30:
        if (talkProc(NULL, 0, NULL, 0)) {
            setSceneChange(1);
        }
        cLib_addCalcAngleS2(&mCurAngle.y, fopAcM_searchPlayerAngleY(this), 2, 0x800);
        setAngle(mCurAngle.y);
        break;
    default:
        rt = 1;
        break;
    }

    return rt;
}

int daNpc_Kn_c::ECut_fourthSkillExplain(int i_idx) {
    dEvent_manager_c* event_manager = &dComIfGp_getEventManager();
    int sp8 = 0;

    int rt = 0;
    int* prm_p = NULL;
    int prm = -1;

    prm_p = dComIfGp_evmng_getMyIntegerP(i_idx, "prm");
    if (prm_p != NULL) {
        prm = *prm_p;
    }

    if (event_manager->getIsAddvance(i_idx)) {
        switch (prm) {
        case 0: 
            mActionMode = 9;
            break;
        case 4: 
            mFaceMotionSeqMngr.setNo(1, -1.0f, 0, 0);
            if (mMotionSeqMngr.getNo() == 0x13) {
                mMotionSeqMngr.setNo(0x16, -1.0f, 0, 0);
            } else {
                mMotionSeqMngr.setNo(16, -1.0f, 0, 0);
            }
            break;
        case 6: 
            mJntAnm.lookPlayer(0);
            break;
        case 10: 
            mStepMode = 2;
            initTalk(0x2EF, NULL);
            break;
        case 20:
            Z2GetAudioMgr()->bgmStart(Z2BGM_OUGI_TRAINING, 0, 0);
            /* fallthrough */
        case 65: {
            setPos(home.pos);
            setAngle(home.angle.y);
    
            cXyz sp64(-10.0f, 0.0f, 375.0f);
            mDoMtx_stack_c::transS(home.pos);
            mDoMtx_stack_c::YrotM(home.angle.y);
            mDoMtx_stack_c::multVec(&sp64, &sp64);

            cXyz sp58(sp64);
            sp58.y += 500.0f;
            if (fopAcM_gc_c::gndCheck(&sp58)) {
                sp64.y = fopAcM_gc_c::getGroundY();
            }

            if (prm == 20) {
                daPy_getPlayerActorClass()->setPlayerPosAndAngle(&sp64, fopAcM_searchPlayerAngleY(this) + 0x7254, 0);
                daPy_getPlayerActorClass()->onTraningCutHead();
            } else {
                daPy_getPlayerActorClass()->setPlayerPosAndAngle(&sp64, fopAcM_searchPlayerAngleY(this) + 0x8000, 0);
                mFaceMotionSeqMngr.setNo(1, -1.0f, 0, 0);
                mMotionSeqMngr.setNo(0, -1.0f, 1, 0);
            }
            void(0);
            break;
        }
        case 30:
            initTalk(0x2F0, NULL);
            break;
        case 40:
            daNpcT_offTmpBit(0xB);
            initTalk(0x2F1, NULL);
            break;
        case 50:
            mFaceMotionSeqMngr.setNo(1, -1.0f, 0, 0);
            mMotionSeqMngr.setNo(13, -1.0f, 1, 0);
            mJntAnm.lookNone(0);
            break;
        case 60:
            initTalk(0x2F1, NULL);
            break;
        case 70:
            mFaceMotionSeqMngr.setNo(1, -1.0f, 0, 0);
            mMotionSeqMngr.setNo(24, -1.0f, 1, 0);
            mJntAnm.lookNone(0);
            break;
        }
    }

    switch (prm) {
    case 0:
        if (speed.y <= 0.0f && mAcch.ChkGroundHit()) {
            speedF = 0.0f;
            speed.zero();

            if (mMotionSeqMngr.getNo() == 0x12) {
                mMotionSeqMngr.setNo(19, -1.0f, 0, 0);
            } else {
                mMotionSeqMngr.setNo(15, -1.0f, 0, 0);
            }

            mFaceMotionSeqMngr.setNo(1, -1.0f, 0, 0);
            setLandingPrtcl();
            rt = 1;
        }
        break;
    case 2:
        if (mMotionSeqMngr.getNo() == 0x13 || mMotionSeqMngr.getNo() == 0xF) {
            if (mMotionSeqMngr.getStepNo() > 0) {
                rt = 1;
            } else if (mpModelMorf[0]->getFrame() >= 13.0f && mpModelMorf[0]->getFrame() < 14.0f) {
                setLandingPrtcl();
            }
        }
        break;
    case 4:
        if ((mMotionSeqMngr.getNo() == 0x16 || mMotionSeqMngr.getNo() == 0x10) && mMotionSeqMngr.getStepNo() > 0) {
            rt = 1;
        }
        break;
    case 6:
        if (mCurAngle.y != fopAcM_searchPlayerAngleY(this)) {
            if (step(fopAcM_searchPlayerAngleY(this), 1, 0x20, 20, 0)) {
                rt = 1;
                mFaceMotionSeqMngr.setNo(1, -1.0f, 0, 0);
                mMotionSeqMngr.setNo(0, -1.0f, 0, 0);
            }
        } else {
            rt = 1;
        }
        break;
    case 10:
        if (talkProc(NULL, 0, NULL, 0)) {
            rt = 1;
        }
        break;
    case 20:
    case 65: {
        setPos(home.pos);
        setAngle(home.angle.y);

        cXyz sp4C(-10.0f, 0.0f, 375.0f);
        mDoMtx_stack_c::transS(home.pos);
        mDoMtx_stack_c::YrotM(home.angle.y);
        mDoMtx_stack_c::multVec(&sp4C, &sp4C);

        cXyz sp40(sp4C);
        sp40.y += 500.0f;
        if (fopAcM_gc_c::gndCheck(&sp40)) {
            sp4C.y = fopAcM_gc_c::getGroundY();
        }

        if (prm == 20) {
            daPy_getPlayerActorClass()->setPlayerPosAndAngle(&sp4C, fopAcM_searchPlayerAngleY(this) + 0x7254, 0);
        } else {
            daPy_getPlayerActorClass()->setPlayerPosAndAngle(&sp4C, fopAcM_searchPlayerAngleY(this) + 0x8000, 0);
        }
        rt = 1;
        break;
    }
    case 25:
        rt = 1;
        break;
    case 30:
    case 40:
        if (talkProc(NULL, 0, NULL, 0)) {
            rt = 1;
        }
        break;
    case 50:
        if (mMotionSeqMngr.getNo() == 0xD && !mMotionSeqMngr.checkEntryNewMotion()) {
            if (mpModelMorf[0]->isStop()) {
                cXyz sp34(0.0f, 0.0f, 594.0f);
                mDoMtx_stack_c::YrotS(home.angle.y);
                mDoMtx_stack_c::multVec(&sp34, &sp34);
                sp34 += home.pos;

                setPos(sp34);
                setAngle(home.angle.y + 0x8000);
                mFaceMotionSeqMngr.setNo(1, -1.0f, 0, 0);
                mMotionSeqMngr.setNo(6, -1.0f, 1, 0);
            }
        } else if (mMotionSeqMngr.getNo() == 6 && !mMotionSeqMngr.checkEntryNewMotion()) {
            rt = 1;
        }
        break;
    case 60:
        if (talkProc(NULL, 0, NULL, 0)) {
            rt = 1;
        }
        break;
    case 70:
        if (mMotionSeqMngr.getNo() == 0x18 && mMotionSeqMngr.getStepNo() > 0) {
            rt = 1;
        }
        break;
    default: 
        rt = 1;
        break;
    }

    return rt;
}

int daNpc_Kn_c::ECut_fourthSkillGet(int i_idx) {
    dEvent_manager_c* event_manager = &dComIfGp_getEventManager();
    int sp8 = 0;

    int rt = 0;
    int* prm_p = NULL;
    int prm = -1;

    prm_p = dComIfGp_evmng_getMyIntegerP(i_idx, "prm");
    if (prm_p != NULL) {
        prm = *prm_p;
    }

    if (event_manager->getIsAddvance(i_idx)) {
        switch (prm) {
        case 0:
            daNpcT_offTmpBit(0xB);
            daNpcT_offTmpBit(0xC);
            break;
        case 4:
            mFaceMotionSeqMngr.setNo(1, -1.0f, 0, 0);
            if (mMotionSeqMngr.getNo() == 0x13) {
                mMotionSeqMngr.setNo(0x16, -1.0f, 0, 0);
            } else {
                mMotionSeqMngr.setNo(16, -1.0f, 0, 0);
            }
            break;
        case 6:
            mJntAnm.lookNone(0);
            break;
        case 10: {
            mStepMode = 2;

            cXyz sp1C(0.0f, 0.0f, 700.0f);
            mDoMtx_stack_c::transS(current.pos);
            mDoMtx_stack_c::YrotM(cLib_targetAngleY(&current.pos, &home.pos));
            mDoMtx_stack_c::multVec(&sp1C, &sp1C);

            cXyz sp10(sp1C);
            sp10.y += 500.0f;
            if (fopAcM_gc_c::gndCheck(&sp10)) {
                sp1C.y = fopAcM_gc_c::getGroundY();
            }

            daPy_getPlayerActorClass()->setPlayerPosAndAngle(&sp1C, (s16)cLib_targetAngleY(&home.pos, &current.pos), 0);
            initTalk(0x2F5, NULL);
            break;
        }
        case 12:
            setAngle(fopAcM_searchPlayerAngleY(this));
            break;
        case 15:
            Z2GetAudioMgr()->bgmStreamPrepare(0x2000038);
            Z2GetAudioMgr()->bgmStreamPlay();
            break;
        case 20:
            initTalk(0x2F5, NULL);
            break;
        case 30:
            initTalk(0x2F5, NULL);
            break;
        }
    }

    switch (prm) {
    case 0:
        if (speed.y <= 0.0f && mAcch.ChkGroundHit()) {
            speedF = 0.0f;
            speed.zero();

            if (mMotionSeqMngr.getNo() == 0x12) {
                mMotionSeqMngr.setNo(19, -1.0f, 0, 0);
            } else {
                mMotionSeqMngr.setNo(15, -1.0f, 0, 0);
            }

            mFaceMotionSeqMngr.setNo(1, -1.0f, 0, 0);
            setLandingPrtcl();
            rt = 1;
        }
        break;
    case 2:
        if (mMotionSeqMngr.getNo() == 0x13 || mMotionSeqMngr.getNo() == 0xF) {
            if (mMotionSeqMngr.getStepNo() > 0) {
                rt = 1;
            } else if (mpModelMorf[0]->getFrame() >= 13.0f && mpModelMorf[0]->getFrame() < 14.0f) {
                setLandingPrtcl();
            }
        }
        break;
    case 4:
        if ((mMotionSeqMngr.getNo() == 0x16 || mMotionSeqMngr.getNo() == 0x10) && mMotionSeqMngr.getStepNo() > 0) {
            rt = 1;
        }
        break;
    case 6:
        if (mCurAngle.y != fopAcM_searchPlayerAngleY(this)) {
            if (step(fopAcM_searchPlayerAngleY(this), 1, 0x20, 20, 0)) {
                rt = 1;
                mFaceMotionSeqMngr.setNo(1, -1.0f, 0, 0);
                mMotionSeqMngr.setNo(0, -1.0f, 0, 0);
            }
        } else {
            rt = 1;
        }
        break;
    case 10:
        if (talkProc(NULL, 0, NULL, 0)) {
            rt = 1;
            if (field_0x170e == 0) {
                field_0x170e = 1;
                Z2GetAudioMgr()->bgmStop(0x32, 0);
            }
        }
        break;
    case 20:
        if (talkProc(NULL, 0, NULL, 0)) {
            rt = 1;
        }
        cLib_addCalcAngleS2(&mCurAngle.y, fopAcM_searchPlayerAngleY(this), 2, 0x800);
        setAngle(mCurAngle.y);
        break;
    case 30:
        if (talkProc(NULL, 0, NULL, 0)) {
            setSceneChange(1);
        }
        cLib_addCalcAngleS2(&mCurAngle.y, fopAcM_searchPlayerAngleY(this), 2, 0x800);
        setAngle(mCurAngle.y);
        break;
    default:
        rt = 1;
        break;
    }

    return rt;
}

int daNpc_Kn_c::ECut_fifthSkillExplain(int i_idx) {
    dEvent_manager_c* event_manager = &dComIfGp_getEventManager();
    int sp8 = 0;

    int rt = 0;
    int* prm_p = NULL;
    int prm = -1;

    prm_p = dComIfGp_evmng_getMyIntegerP(i_idx, "prm");
    if (prm_p != NULL) {
        prm = *prm_p;
    }

    if (event_manager->getIsAddvance(i_idx)) {
        switch (prm) {
        case 0:
            break;
        case 4: 
            mFaceMotionSeqMngr.setNo(1, -1.0f, 0, 0);
            if (mMotionSeqMngr.getNo() == 0x13) {
                mMotionSeqMngr.setNo(0x16, -1.0f, 0, 0);
            } else {
                mMotionSeqMngr.setNo(16, -1.0f, 0, 0);
            }
            break;
        case 6: 
            mJntAnm.lookPlayer(0);
            break;
        case 10: 
            mStepMode = 2;
            initTalk(0x2E7, NULL);
            break;
        case 20:
            Z2GetAudioMgr()->bgmStart(Z2BGM_OUGI_TRAINING, 0, 0);
            /* fallthrough */
        case 65: {
            mFaceMotionSeqMngr.setNo(1, -1.0f, 0, 0);
            if (prm == 20) {
                mMotionSeqMngr.setNo(3, -1.0f, 0, 0);
                field_0x15cd = 1;
            } else {
                mMotionSeqMngr.setNo(0, -1.0f, 0, 0);
                field_0x15cd = 0;
            }

            setPos(home.pos);
            setAngle(home.angle.y);
    
            cXyz sp64(0.0f, 0.0f, 375.0f);
            mDoMtx_stack_c::transS(home.pos);
            mDoMtx_stack_c::YrotM(home.angle.y);
            mDoMtx_stack_c::multVec(&sp64, &sp64);

            cXyz sp58(sp64);
            sp58.y += 500.0f;
            if (fopAcM_gc_c::gndCheck(&sp58)) {
                sp64.y = fopAcM_gc_c::getGroundY();
            }

            daPy_getPlayerActorClass()->setPlayerPosAndAngle(&sp64, fopAcM_searchPlayerAngleY(this) + 0x8000, 0);
            daPy_getPlayerActorClass()->onTraningCutFast();
            break;
        }
        case 30:
            initTalk(0x2E8, NULL);
            break;
        case 40:
            daNpcT_offTmpBit(0xB);
            initTalk(0x2E9, NULL);
            break;
        case 50:
            mFaceMotionSeqMngr.setNo(1, -1.0f, 0, 0);
            mMotionSeqMngr.setNo(12, -1.0f, 1, 0);
            mJntAnm.lookNone(0);
            break;
        case 60:
            initTalk(0x2E9, NULL);
            break;
        case 70:
            mFaceMotionSeqMngr.setNo(1, -1.0f, 0, 0);
            mMotionSeqMngr.setNo(24, -1.0f, 1, 0);
            mJntAnm.lookNone(0);
            break;
        }
    }

    switch (prm) {
    case 0:
        if (speed.y <= 0.0f && mAcch.ChkGroundHit()) {
            speedF = 0.0f;
            speed.zero();

            if (mMotionSeqMngr.getNo() == 0x12) {
                mMotionSeqMngr.setNo(19, -1.0f, 0, 0);
            } else {
                mMotionSeqMngr.setNo(15, -1.0f, 0, 0);
            }

            mFaceMotionSeqMngr.setNo(1, -1.0f, 0, 0);
            setLandingPrtcl();
            rt = 1;
        }
        break;
    case 2:
        if (mMotionSeqMngr.getNo() == 0x13 || mMotionSeqMngr.getNo() == 0xF) {
            if (mMotionSeqMngr.getStepNo() > 0) {
                rt = 1;
            } else if (mpModelMorf[0]->getFrame() >= 13.0f && mpModelMorf[0]->getFrame() < 14.0f) {
                setLandingPrtcl();
            }
        }
        break;
    case 4:
        if ((mMotionSeqMngr.getNo() == 0x16 || mMotionSeqMngr.getNo() == 0x10) && mMotionSeqMngr.getStepNo() > 0) {
            rt = 1;
        }
        break;
    case 6:
        if (mCurAngle.y != fopAcM_searchPlayerAngleY(this)) {
            if (step(fopAcM_searchPlayerAngleY(this), 1, 0x20, 20, 0)) {
                rt = 1;
                mFaceMotionSeqMngr.setNo(1, -1.0f, 0, 0);
                mMotionSeqMngr.setNo(0, -1.0f, 0, 0);
            }
        } else {
            rt = 1;
        }
        break;
    case 10:
        if (talkProc(NULL, 0, NULL, 0)) {
            rt = 1;
        }
        break;
    case 20:
    case 65: {
        rt = 1;
        setPos(home.pos);
        setAngle(home.angle.y);

        cXyz sp4C(0.0f, 0.0f, 375.0f);
        mDoMtx_stack_c::transS(home.pos);
        mDoMtx_stack_c::YrotM(home.angle.y);
        mDoMtx_stack_c::multVec(&sp4C, &sp4C);

        cXyz sp40(sp4C);
        sp40.y += 500.0f;
        if (fopAcM_gc_c::gndCheck(&sp40)) {
            sp4C.y = fopAcM_gc_c::getGroundY();
        }

        daPy_getPlayerActorClass()->setPlayerPosAndAngle(&sp4C, fopAcM_searchPlayerAngleY(this) + 0x8000, 0);
        break;
    }
    case 30:
    case 40:
        if (talkProc(NULL, 0, NULL, 0)) {
            rt = 1;
        }
        break;
    case 50:
        if (mMotionSeqMngr.getNo() == 0xC && !mMotionSeqMngr.checkEntryNewMotion()) {
            if (mpModelMorf[0]->isStop()) {
                cXyz sp34(0.0f, 0.0f, 165.0f);
                mDoMtx_stack_c::YrotS(home.angle.y);
                mDoMtx_stack_c::multVec(&sp34, &sp34);
                sp34 += home.pos;

                setPos(sp34);
                setAngle((s16) home.angle.y);
                mFaceMotionSeqMngr.setNo(1, -1.0f, 0, 0);
                mMotionSeqMngr.setNo(7, -1.0f, 1, 0);
            } else if (mpModelMorf[0]->getFrame() >= 8.0f && mpModelMorf[0]->getFrame() < 9.0f) {
                mpPodModel->getModelData()->getMaterialNodePointer(0)->getShape()->hide();
            }
        } else if (mMotionSeqMngr.getNo() == 7 && !mMotionSeqMngr.checkEntryNewMotion()) {
            rt = 1;
        }
        break;
    case 60:
        if (talkProc(NULL, 0, NULL, 0)) {
            rt = 1;
        }
        break;
    case 70:
        if (mMotionSeqMngr.getNo() == 0x18 && mMotionSeqMngr.getStepNo() > 0) {
            rt = 1;
            mActionMode = 12;
        }
        break;
    default: 
        rt = 1;
        break;
    }

    return rt;
}

int daNpc_Kn_c::ECut_fifthSkillGet(int i_idx) {
    dEvent_manager_c* event_manager = &dComIfGp_getEventManager();
    int sp8 = 0;

    int rt = 0;
    int* prm_p = NULL;
    int prm = -1;

    prm_p = dComIfGp_evmng_getMyIntegerP(i_idx, "prm");
    if (prm_p != NULL) {
        prm = *prm_p;
    }

    if (event_manager->getIsAddvance(i_idx)) {
        switch (prm) {
        case 0:
            daNpcT_offTmpBit(0xB);
            daNpcT_offTmpBit(0xC);
            break;
        case 4:
            mFaceMotionSeqMngr.setNo(1, -1.0f, 0, 0);
            if (mMotionSeqMngr.getNo() == 0x13) {
                mMotionSeqMngr.setNo(0x16, -1.0f, 0, 0);
            } else {
                mMotionSeqMngr.setNo(16, -1.0f, 0, 0);
            }
            break;
        case 6:
            mJntAnm.lookNone(0);
            break;
        case 10: {
            mStepMode = 2;

            cXyz sp1C(0.0f, 0.0f, 700.0f);
            mDoMtx_stack_c::transS(current.pos);
            mDoMtx_stack_c::YrotM(cLib_targetAngleY(&current.pos, &home.pos));
            mDoMtx_stack_c::multVec(&sp1C, &sp1C);

            cXyz sp10(sp1C);
            sp10.y += 500.0f;
            if (fopAcM_gc_c::gndCheck(&sp10)) {
                sp1C.y = fopAcM_gc_c::getGroundY();
            }

            daPy_getPlayerActorClass()->setPlayerPosAndAngle(&sp1C, (s16)cLib_targetAngleY(&home.pos, &current.pos), 0);
            initTalk(0x2ED, NULL);
            break;
        }
        case 12:
            setAngle(fopAcM_searchPlayerAngleY(this));
            break;
        case 15:
            Z2GetAudioMgr()->bgmStreamPrepare(0x2000038);
            Z2GetAudioMgr()->bgmStreamPlay();
            break;
        case 20:
            initTalk(0x2ED, NULL);
            break;
        case 30:
            initTalk(0x2ED, NULL);
            break;
        }
    }

    switch (prm) {
    case 0:
        if (speed.y <= 0.0f && mAcch.ChkGroundHit()) {
            speedF = 0.0f;
            speed.zero();

            if (mMotionSeqMngr.getNo() == 0x12) {
                mMotionSeqMngr.setNo(19, -1.0f, 0, 0);
            } else {
                mMotionSeqMngr.setNo(15, -1.0f, 0, 0);
            }

            mFaceMotionSeqMngr.setNo(1, -1.0f, 0, 0);
            setLandingPrtcl();
            rt = 1;
        }
        break;
    case 2:
        if (mMotionSeqMngr.getNo() == 0x13 || mMotionSeqMngr.getNo() == 0xF) {
            if (mMotionSeqMngr.getStepNo() > 0) {
                rt = 1;
            } else if (mpModelMorf[0]->getFrame() >= 13.0f && mpModelMorf[0]->getFrame() < 14.0f) {
                setLandingPrtcl();
            }
        }
        break;
    case 4:
        if ((mMotionSeqMngr.getNo() == 0x16 || mMotionSeqMngr.getNo() == 0x10) && mMotionSeqMngr.getStepNo() > 0) {
            rt = 1;
        }
        break;
    case 6:
        if (mCurAngle.y != fopAcM_searchPlayerAngleY(this)) {
            if (step(fopAcM_searchPlayerAngleY(this), 1, 0x20, 20, 0)) {
                rt = 1;
                mFaceMotionSeqMngr.setNo(1, -1.0f, 0, 0);
                mMotionSeqMngr.setNo(0, -1.0f, 0, 0);
            }
        } else {
            rt = 1;
        }
        break;
    case 10:
        if (talkProc(NULL, 0, NULL, 0)) {
            rt = 1;
            if (field_0x170e == 0) {
                field_0x170e = 1;
                Z2GetAudioMgr()->bgmStop(0x32, 0);
            }
        }
        break;
    case 20:
        if (talkProc(NULL, 0, NULL, 0)) {
            rt = 1;
        }
        cLib_addCalcAngleS2(&mCurAngle.y, fopAcM_searchPlayerAngleY(this), 2, 0x800);
        setAngle(mCurAngle.y);
        break;
    case 30:
        if (talkProc(NULL, 0, NULL, 0)) {
            setSceneChange(1);
        }
        cLib_addCalcAngleS2(&mCurAngle.y, fopAcM_searchPlayerAngleY(this), 2, 0x800);
        setAngle(mCurAngle.y);
        break;
    default:
        rt = 1;
        break;
    }

    return rt;
}

int daNpc_Kn_c::ECut_sixthSkillExplain(int i_idx) {
    dEvent_manager_c* event_manager = &dComIfGp_getEventManager();
    int sp8 = 0;

    int rt = 0;
    int* prm_p = NULL;
    int prm = -1;

    prm_p = dComIfGp_evmng_getMyIntegerP(i_idx, "prm");
    if (prm_p != NULL) {
        prm = *prm_p;
    }

    if (event_manager->getIsAddvance(i_idx)) {
        switch (prm) {
        case 0:
            break;
        case 4: 
            mFaceMotionSeqMngr.setNo(1, -1.0f, 0, 0);
            if (mMotionSeqMngr.getNo() == 0x13) {
                mMotionSeqMngr.setNo(0x16, -1.0f, 0, 0);
            } else {
                mMotionSeqMngr.setNo(16, -1.0f, 0, 0);
            }
            break;
        case 6: 
            mJntAnm.lookPlayer(0);
            break;
        case 10: 
            mStepMode = 2;
            initTalk(0x2F8, NULL);
            break;
        case 20:
            Z2GetAudioMgr()->bgmStart(Z2BGM_OUGI_TRAINING, 0, 0);
            /* fallthrough */
        case 65: {
            setPos(home.pos);
            setAngle(home.angle.y);
    
            cXyz sp64(0.0f, 0.0f, 375.0f);
            mDoMtx_stack_c::transS(home.pos);
            mDoMtx_stack_c::YrotM(home.angle.y);
            mDoMtx_stack_c::multVec(&sp64, &sp64);

            cXyz sp58(sp64);
            sp58.y += 500.0f;
            if (fopAcM_gc_c::gndCheck(&sp58)) {
                sp64.y = fopAcM_gc_c::getGroundY();
            }

            daPy_getPlayerActorClass()->setPlayerPosAndAngle(&sp64, fopAcM_searchPlayerAngleY(this) + 0x8000, 0);
            daPy_getPlayerActorClass()->onTraningCutLargeJump();
            break;
        }
        case 30:
            initTalk(0x2F9, NULL);
            break;
        case 40:
            daNpcT_offTmpBit(0xB);
            initTalk(0x2FA, NULL);
            break;
        case 50:
            mFaceMotionSeqMngr.setNo(1, -1.0f, 0, 0);
            mMotionSeqMngr.setNo(10, -1.0f, 1, 0);
            mJntAnm.lookNone(0);
            break;
        case 60:
            initTalk(0x2FA, NULL);
            break;
        case 70:
            mFaceMotionSeqMngr.setNo(1, -1.0f, 0, 0);
            mMotionSeqMngr.setNo(24, -1.0f, 1, 0);
            mJntAnm.lookNone(0);
            break;
        case 80:
            field_0xdec = 0x78;
            break;
        }
    }

    switch (prm) {
    case 0:
        if (speed.y <= 0.0f && mAcch.ChkGroundHit()) {
            speedF = 0.0f;
            speed.zero();

            if (mMotionSeqMngr.getNo() == 0x12) {
                mMotionSeqMngr.setNo(19, -1.0f, 0, 0);
            } else {
                mMotionSeqMngr.setNo(15, -1.0f, 0, 0);
            }

            mFaceMotionSeqMngr.setNo(1, -1.0f, 0, 0);
            setLandingPrtcl();
            rt = 1;
        }
        break;
    case 2:
        if (mMotionSeqMngr.getNo() == 0x13 || mMotionSeqMngr.getNo() == 0xF) {
            if (mMotionSeqMngr.getStepNo() > 0) {
                rt = 1;
            } else if (mpModelMorf[0]->getFrame() >= 13.0f && mpModelMorf[0]->getFrame() < 14.0f) {
                setLandingPrtcl();
            }
        }
        break;
    case 4:
        if ((mMotionSeqMngr.getNo() == 0x16 || mMotionSeqMngr.getNo() == 0x10) && mMotionSeqMngr.getStepNo() > 0) {
            rt = 1;
        }
        break;
    case 6:
        if (mCurAngle.y != fopAcM_searchPlayerAngleY(this)) {
            if (step(fopAcM_searchPlayerAngleY(this), 1, 0x20, 20, 0)) {
                rt = 1;
                mFaceMotionSeqMngr.setNo(1, -1.0f, 0, 0);
                mMotionSeqMngr.setNo(0, -1.0f, 0, 0);
            }
        } else {
            rt = 1;
        }
        break;
    case 10:
        if (talkProc(NULL, 0, NULL, 0)) {
            rt = 1;
        }
        break;
    case 20:
    case 65: {
        setPos(home.pos);
        setAngle(home.angle.y);

        cXyz sp4C(0.0f, 0.0f, 375.0f);
        mDoMtx_stack_c::transS(home.pos);
        mDoMtx_stack_c::YrotM(home.angle.y);
        mDoMtx_stack_c::multVec(&sp4C, &sp4C);

        cXyz sp40(sp4C);
        sp40.y += 500.0f;
        if (fopAcM_gc_c::gndCheck(&sp40)) {
            sp4C.y = fopAcM_gc_c::getGroundY();
        }

        daPy_getPlayerActorClass()->setPlayerPosAndAngle(&sp4C, fopAcM_searchPlayerAngleY(this) + 0x8000, 0);
        rt = 1;
        break;
    }
    case 30:
    case 40:
        if (talkProc(NULL, 0, NULL, 0)) {
            rt = 1;
        }
        break;
    case 50:
        if (mMotionSeqMngr.getNo() == 10 && !mMotionSeqMngr.checkEntryNewMotion()) {
            if (mpModelMorf[0]->isStop()) {
                cXyz sp34(0.0f, 0.0f, 138.64f);
                mDoMtx_stack_c::YrotS(home.angle.y);
                mDoMtx_stack_c::multVec(&sp34, &sp34);
                sp34 += home.pos;

                setPos(sp34);
                setAngle((s16) home.angle.y);
                mFaceMotionSeqMngr.setNo(1, -1.0f, 0, 0);
                mMotionSeqMngr.setNo(0, 0.0f, 1, 0);
            }
        } else if (mMotionSeqMngr.getNo() == 0 && !mMotionSeqMngr.checkEntryNewMotion()) {
            rt = 1;
        }
        break;
    case 60:
        if (talkProc(NULL, 0, NULL, 0)) {
            rt = 1;
        }
        break;
    case 70:
        if (mMotionSeqMngr.getNo() == 0x18 && mMotionSeqMngr.getStepNo() > 0) {
            rt = 1;
            for (int i = 0; i < 2; i++) {
                fopAcM_createChild(PROC_NPC_KN, fopAcM_GetID(this), ((i + 1) << 8) + 6, &current.pos, fopAcM_GetRoomNo(this), &current.angle, NULL, -1, NULL);
            }
        }
        break;
    case 80:
        if (cLib_calcTimer(&field_0xdec) == 0) {
            rt = 1;
            mActionMode = 13;
        }
        break;
    default: 
        rt = 1;
        break;
    }

    return rt;
}

int daNpc_Kn_c::ECut_sixthSkillGet(int i_idx) {
    dEvent_manager_c* event_manager = &dComIfGp_getEventManager();

    int sp_0x8 = 0;
    int rt = 0;
    int* prm_p = NULL;
    int prm = -1;

    prm_p = dComIfGp_evmng_getMyIntegerP(i_idx, "prm");
    if (prm_p != NULL) {
        prm = *prm_p;
    }

    if (event_manager->getIsAddvance(i_idx)) {
        switch (prm) {
        case 0:
            daNpcT_offTmpBit(0xb);
            daNpcT_offTmpBit(0xc);
            break;
        case 4:
            mFaceMotionSeqMngr.setNo(1, -1.0f, 0, 0);

            if (mMotionSeqMngr.getNo() == 0x13) {
                mMotionSeqMngr.setNo(22, -1.0f, 0, 0);
            } else {
                mMotionSeqMngr.setNo(16, -1.0f, 0, 0);
            }

            fpcM_Search(s_subEnd6, this);
            break;
        case 6:
            mJntAnm.lookNone(0);
            break;
        case 10: {
            mStepMode = 2;

            cXyz pos(0.0f, 0.0f, 1000.0f);
            mDoMtx_stack_c::transS(current.pos);
            mDoMtx_stack_c::YrotM(cLib_targetAngleY(&current.pos, &home.pos));
            mDoMtx_stack_c::multVec(&pos, &pos);

            cXyz pos2 = pos;
            pos2.y += 500.0f;
            if (fopAcM_gc_c::gndCheck(&pos2)) {
                pos.y = fopAcM_gc_c::getGroundY();
            }

            daPy_getPlayerActorClass()->setPlayerPosAndAngle(&pos, (s16)cLib_targetAngleY(&home.pos, &current.pos), 0);
            initTalk(0x2fe, NULL);
            break;
        }
        case 12:
            setAngle(fopAcM_searchPlayerAngleY(this));
            break;
        case 15:
            Z2GetAudioMgr()->bgmStreamPrepare(0x2000038);
            Z2GetAudioMgr()->bgmStreamPlay();
            break;
        case 20:
            initTalk(0x2fe, NULL);
            break;
        case 30:
            initTalk(0x2fe, NULL);
        }
    }

    switch (prm) {
    case 0:
        if (speed.y <= 0.0f && mAcch.ChkGroundHit()) {
            speedF = 0.0f;
            speed.zero();

            if (mMotionSeqMngr.getNo() == 0x12) {
                mMotionSeqMngr.setNo(0x13, -1.0f, 0, 0);
            } else {
                mMotionSeqMngr.setNo(15, -1.0f, 0, 0);
            }

            mFaceMotionSeqMngr.setNo(1, -1.0f, 0, 0);
            setLandingPrtcl();
            rt = 1;
        }
        break;
    case 2:
        if (mMotionSeqMngr.getNo() == 0x13 || mMotionSeqMngr.getNo() == 15) {
            if (mMotionSeqMngr.getStepNo() > 0) {
                rt = 1;
            } else if (mpModelMorf[0]->getFrame() >= 13.0f && mpModelMorf[0]->getFrame() < 14.0f) {
                setLandingPrtcl();
            }
        }
        break;
    case 4:
        if (mMotionSeqMngr.getNo() == 22 || mMotionSeqMngr.getNo() == 16) {
            if (mMotionSeqMngr.getStepNo() > 0) {
                rt = 1;
            }
        }
        break;
    case 6:
        if (mCurAngle.y != fopAcM_searchPlayerAngleY(this)) {
            if (step(fopAcM_searchPlayerAngleY(this), 1, 0x20, 20, 0)) {
                rt = 1;
                mFaceMotionSeqMngr.setNo(1, -1.0f, 0, 0);
                mMotionSeqMngr.setNo(0, -1.0f, 0, 0);
            }
        } else {
            rt = 1;
        }
        break;
    case 10:
        if (talkProc(NULL, 0, NULL, 0)) {
            rt = 1;

            if (field_0x170e == 0) {
                field_0x170e = 1;
                Z2GetAudioMgr()->bgmStop(0x32, 0);
            }
        }
        break;
    case 20:
        if (talkProc(NULL, 0, NULL, 0)) {
            rt = 1;
        }
        cLib_addCalcAngleS2(&mCurAngle.y, fopAcM_searchPlayerAngleY(this), 2, 0x800);
        setAngle(mCurAngle.y);
        break;
    case 30:
        if (talkProc(NULL, 0, NULL, 0)) {
            setSceneChange(1);
        }
        cLib_addCalcAngleS2(&mCurAngle.y, fopAcM_searchPlayerAngleY(this), 2, 0x800);
        setAngle(mCurAngle.y);
        break;
    default:
        rt = 1;
    }

    return rt;
}

int daNpc_Kn_c::ECut_seventhSkillExplain(int i_idx) {
    dEvent_manager_c* event_manager = &dComIfGp_getEventManager();
    int sp8 = 0;

    int rt = 0;
    int* prm_p = NULL;
    int prm = -1;

    prm_p = dComIfGp_evmng_getMyIntegerP(i_idx, "prm");
    if (prm_p != NULL) {
        prm = *prm_p;
    }

    if (event_manager->getIsAddvance(i_idx)) {
        switch (prm) {
        case 0:
            break;
        case 4: 
            mFaceMotionSeqMngr.setNo(1, -1.0f, 0, 0);
            if (mMotionSeqMngr.getNo() == 0x13) {
                mMotionSeqMngr.setNo(0x16, -1.0f, 0, 0);
            } else {
                mMotionSeqMngr.setNo(16, -1.0f, 0, 0);
            }
            break;
        case 6: 
            mJntAnm.lookPlayer(0);
            break;
        case 10: 
            mStepMode = 2;
            initTalk(0x300, NULL);
            break;
        case 20: {
            Z2GetAudioMgr()->bgmStart(Z2BGM_OUGI_TRAINING, 0, 0);

            setPos(home.pos);
            setAngle(home.angle.y);
    
            cXyz sp64(0.0f, 0.0f, 375.0f);
            mDoMtx_stack_c::transS(home.pos);
            mDoMtx_stack_c::YrotM(home.angle.y);
            mDoMtx_stack_c::multVec(&sp64, &sp64);

            cXyz sp58(sp64);
            sp58.y += 500.0f;
            if (fopAcM_gc_c::gndCheck(&sp58)) {
                sp64.y = fopAcM_gc_c::getGroundY();
            }

            daPy_getPlayerActorClass()->setPlayerPosAndAngle(&sp64, fopAcM_searchPlayerAngleY(this) + 0x8000, 0);
            daPy_getPlayerActorClass()->onTraningCutLargeTurn();
            break;
        }
        case 30:
            initTalk(0x301, NULL);
            break;
        case 40:
            daNpcT_offTmpBit(0xB);
            initTalk(0x302, NULL);
            break;
        case 50:
            dComIfGp_setItemLifeCount(dComIfGs_getMaxLife(), 0);
            dMeter2Info_onLifeGaugeSE();
            dComIfGp_event_offHindFlag(0x10);
            break;
        case 55:
            field_0xdec = 30;
            break;
        case 60:
            initTalk(0x302, NULL);
            break;
        case 70:
            mFaceMotionSeqMngr.setNo(1, -1.0f, 0, 0);
            mMotionSeqMngr.setNo(24, -1.0f, 1, 0);
            mJntAnm.lookNone(0);
            break;
        case 80:
            field_0xdec = 0x78;
            break;
        }
    }

    switch (prm) {
    case 0:
        if (speed.y <= 0.0f && mAcch.ChkGroundHit()) {
            speedF = 0.0f;
            speed.zero();

            if (mMotionSeqMngr.getNo() == 0x12) {
                mMotionSeqMngr.setNo(19, -1.0f, 0, 0);
            } else {
                mMotionSeqMngr.setNo(15, -1.0f, 0, 0);
            }

            mFaceMotionSeqMngr.setNo(1, -1.0f, 0, 0);
            setLandingPrtcl();
            rt = 1;
        }
        break;
    case 2:
        if (mMotionSeqMngr.getNo() == 0x13 || mMotionSeqMngr.getNo() == 0xF) {
            if (mMotionSeqMngr.getStepNo() > 0) {
                rt = 1;
            } else if (mpModelMorf[0]->getFrame() >= 13.0f && mpModelMorf[0]->getFrame() < 14.0f) {
                setLandingPrtcl();
            }
        }
        break;
    case 4:
        if ((mMotionSeqMngr.getNo() == 0x16 || mMotionSeqMngr.getNo() == 0x10) && mMotionSeqMngr.getStepNo() > 0) {
            rt = 1;
        }
        break;
    case 6:
        if (mCurAngle.y != fopAcM_searchPlayerAngleY(this)) {
            if (step(fopAcM_searchPlayerAngleY(this), 1, 0x20, 20, 0)) {
                rt = 1;
                mFaceMotionSeqMngr.setNo(1, -1.0f, 0, 0);
                mMotionSeqMngr.setNo(0, -1.0f, 0, 0);
            }
        } else {
            rt = 1;
        }
        break;
    case 10:
        if (talkProc(NULL, 0, NULL, 0)) {
            rt = 1;
        }
        break;
    case 20: {
        setPos(home.pos);
        setAngle(home.angle.y);

        cXyz sp4C(0.0f, 0.0f, 375.0f);
        mDoMtx_stack_c::transS(home.pos);
        mDoMtx_stack_c::YrotM(home.angle.y);
        mDoMtx_stack_c::multVec(&sp4C, &sp4C);

        cXyz sp40(sp4C);
        sp40.y += 500.0f;
        if (fopAcM_gc_c::gndCheck(&sp40)) {
            sp4C.y = fopAcM_gc_c::getGroundY();
        }

        daPy_getPlayerActorClass()->setPlayerPosAndAngle(&sp4C, fopAcM_searchPlayerAngleY(this) + 0x8000, 0);
        rt = 1;
        break;
    }
    case 30:
    case 40:
        if (talkProc(NULL, 0, NULL, 0)) {
            rt = 1;
        }
        break;
    case 50:
        if (dMeter2Info_getNowLifeGauge() == dComIfGs_getLife()) {
            rt = 1;
        }
        break;
    case 55:
        if (cLib_calcTimer<int>(&field_0xdec) == 0) {
            rt = 1;
        }
        break;
    case 60:
        if (talkProc(NULL, 0, NULL, 0)) {
            rt = 1;
        }
        break;
    case 70:
        if (mMotionSeqMngr.getNo() == 0x18 && mMotionSeqMngr.getStepNo() > 0) {
            rt = 1;
            for (int i = 0; i < 2; i++) {
                fopAcM_createChild(PROC_NPC_KN, fopAcM_GetID(this), ((i + 1) << 8) + 7, &current.pos, fopAcM_GetRoomNo(this), &current.angle, NULL, -1, NULL);
            }
        }
        break;
    case 80:
        if (cLib_calcTimer(&field_0xdec) == 0) {
            rt = 1;
            mActionMode = 19;
        }
        break;
    default: 
        rt = 1;
        break;
    }

    return rt;
}

int daNpc_Kn_c::ECut_seventhSkillGet(int i_idx) {
    dEvent_manager_c* event_manager = &dComIfGp_getEventManager();
    int sp_0x8 = 0;
    int rt = 0;
    int* prm_p = NULL;
    int prm = -1;

    prm_p = dComIfGp_evmng_getMyIntegerP(i_idx, "prm");
    if (prm_p != NULL) {
        prm = *prm_p;
    }

    if (event_manager->getIsAddvance(i_idx)) {
        switch (prm) {
        case 0:
            daNpcT_offTmpBit(0xB);
            daNpcT_offTmpBit(0xC);
            daNpcT_offTmpBit(0xD);
            break;
        case 4:
            mFaceMotionSeqMngr.setNo(1, -1.0f, 0, 0);

            if (mMotionSeqMngr.getNo() == 0x13) {
                mMotionSeqMngr.setNo(22, -1.0f, 0, 0);
            } else {
                mMotionSeqMngr.setNo(16, -1.0f, 0, 0);
            }

            fpcM_Search(s_subEnd7, this);
            break;
        case 6:
            mJntAnm.lookNone(0);
            break;
        case 10: {
            mStepMode = 2;

            cXyz pos(0.0f, 0.0f, 700.0f);
            mDoMtx_stack_c::transS(current.pos);
            mDoMtx_stack_c::YrotM(cLib_targetAngleY(&current.pos, &home.pos));
            mDoMtx_stack_c::multVec(&pos, &pos);

            cXyz pos2 = pos;
            pos2.y += 500.0f;
            if (fopAcM_gc_c::gndCheck(&pos2)) {
                pos.y = fopAcM_gc_c::getGroundY();
            }

            daPy_getPlayerActorClass()->setPlayerPosAndAngle(&pos, (s16)cLib_targetAngleY(&home.pos, &current.pos), 0);
            initTalk(0x305, NULL);
            break;
        }
        case 12:
            setAngle(fopAcM_searchPlayerAngleY(this));
            break;
        case 15:
            Z2GetAudioMgr()->bgmStreamPrepare(0x2000038);
            Z2GetAudioMgr()->bgmStreamPlay();
            break;
        case 20:
            initTalk(0x305, 0);
            break;
        case 30:
            initTalk(0x305, 0);
            break;
        case 40:
            initTalk(0x305, NULL);
            break;
        }
    }

    switch (prm) {
    case 0:
        if (speed.y <= 0.0f && mAcch.ChkGroundHit()) {
            speedF = 0.0f;
            speed.zero();

            if (mMotionSeqMngr.getNo() == 0x12) {
                mMotionSeqMngr.setNo(0x13, -1.0f, 0, 0);
            } else {
                mMotionSeqMngr.setNo(15, -1.0f, 0, 0);
            }

            mFaceMotionSeqMngr.setNo(1, -1.0f, 0, 0);
            setLandingPrtcl();
            rt = 1;
        }
        break;
    case 2:
        if (mMotionSeqMngr.getNo() == 0x13 || mMotionSeqMngr.getNo() == 15) {
            if (mMotionSeqMngr.getStepNo() > 0) {
                rt = 1;
            } else if (mpModelMorf[0]->getFrame() >= 13.0f && mpModelMorf[0]->getFrame() < 14.0f) {
                setLandingPrtcl();
            }
        }
        break;
    case 4:
        if (mMotionSeqMngr.getNo() == 22 || mMotionSeqMngr.getNo() == 16) {
            if (mMotionSeqMngr.getStepNo() > 0) {
                rt = 1;
            }
        }
        break;
    case 6:
        if (mCurAngle.y != fopAcM_searchPlayerAngleY(this)) {
            if (step(fopAcM_searchPlayerAngleY(this), 1, 0x20, 20, 0)) {
                rt = 1;
                mFaceMotionSeqMngr.setNo(1, -1.0f, 0, 0);
                mMotionSeqMngr.setNo(0, -1.0f, 0, 0);
            }
        } else {
            rt = 1;
        }
        break;
    case 10:
        if (talkProc(NULL, 0, NULL, 0)) {
            rt = 1;

            if (field_0x170e == 0) {
                field_0x170e = 1;
                Z2GetAudioMgr()->bgmStop(0x32, 0);
            }
        }
        break;
    case 20:
        if (talkProc(NULL, 0, NULL, 0)) {
            rt = 1;
        }
        cLib_addCalcAngleS2(&mCurAngle.y, fopAcM_searchPlayerAngleY(this), 2, 0x800);
        setAngle(mCurAngle.y);
        break;
    case 30:
        if (talkProc(NULL, 0, NULL, 0)) {
            rt = 1;
        }
        cLib_addCalcAngleS2(&mCurAngle.y, fopAcM_searchPlayerAngleY(this), 2, 0x800);
        setAngle(mCurAngle.y);
        break;
    case 40:
        if (talkProc(NULL, 0, NULL, 0)) {
            setSceneChange(1);
        }
        cLib_addCalcAngleS2(&mCurAngle.y, fopAcM_searchPlayerAngleY(this), 2, 0x800);
        setAngle(mCurAngle.y);
        break;
    default:
        rt = 1;
    }

    return rt;
}

int daNpc_Kn_c::setPrtcl() {
    JPABaseEmitter* emitter = NULL;
    BOOL check = FALSE;
    u32 sp_0x18 = 0;
    for (int i = 0; i < 3; i++) {
        if (i == 2) {
            check = TRUE;
        } else {
            check = FALSE;
        }

        // csXyz* angle = mParticleMngr[i].mpModel == true ? &mParticleMngr[i].mAngle : NULL;
        // cXyz* pos = mParticleMngr[i].mpModel == true ? &mParticleMngr[i].mPos : NULL;

        mParticleMngr[i].mManager.setEffectCenter(&tevStr, &mParticleMngr[i].mPos, check, sp_0x18,
                                                  mParticleMngr[i].mpModel == true ? &mParticleMngr[i].mPos : NULL,
                                                  mParticleMngr[i].mpModel == true ? &mParticleMngr[i].mAngle : NULL,
                                                  0, fopAcM_GetRoomNo(this), 0.0f, speedF);

        if (mParticleMngr[i].mpModel != NULL) {
            emitter = mParticleMngr[i].mManager.getCenterEmitter(0, 0);
            if (emitter != NULL) {
                switch (i) {
                case 0:
                case 1:
                    mParticleMngr[i].mScale.set(1.0f, 1.0f, 1.0f);
                    emitter->setAwayFromCenterSpeed(0.0f);
                    emitter->setRandomDirectionSpeed(0.0f);
                    emitter->setSpread(1.0f);
                    emitter->setGlobalParticleScale(mParticleMngr[i].mScale);
                    break;
                case 2:
                    mParticleMngr[i].mScale.set(2.0f, 2.0f, 2.0f);
                    break;
                }
            }
        }

        mParticleMngr[i].mpModel = NULL;
    }

    return 1;
}

int daNpc_Kn_c::setSlipPrtcl() {
    mDoMtx_stack_c::copy(mpModelMorf[0]->getModel()->getAnmMtx(0x1f));
    mDoMtx_stack_c::multVecZero(&mParticleMngr[0].mPos);
    mParticleMngr[0].mPos.y -= 20.0f;
    mParticleMngr[0].mAngle = current.angle;
    #if DEBUG
    mParticleMngr[0].mAngle.y -= (s16) 0x8000;
    #else
    mParticleMngr[0].mAngle.y -= 0x8000;
    #endif
    mParticleMngr[0].mpModel = true;

    mDoMtx_stack_c::copy(mpModelMorf[0]->getModel()->getAnmMtx(0x1b));
    mDoMtx_stack_c::multVecZero(&mParticleMngr[1].mPos);
    mParticleMngr[1].mPos.y -= 20.0f;
    mParticleMngr[1].mAngle = current.angle;
    #if DEBUG
    mParticleMngr[1].mAngle.y -= (s16) 0x8000;
    #else
    mParticleMngr[1].mAngle.y -= 0x8000;
    #endif
    mParticleMngr[1].mpModel = true;
    return 1;
}

int daNpc_Kn_c::setLandingPrtcl() {
    mParticleMngr[2].mPos = current.pos;
    mParticleMngr[2].mAngle = mCurAngle;
    mParticleMngr[2].mpModel = true;
    return 1;
}

void daNpc_Kn_c::calcMagicBallPos() {
    fopAc_ac_c* actor;

    if (mType == 1 && fopAcM_SearchByID(parentActorID, &actor) && actor &&
        fopAcM_GetName(actor) == PROC_KN_BULLET && ((daObjKnBullet_c*)actor)->getActionMode() == 0)
    {
        cXyz pos(150.0f, 0.0f, 0.0f);
        mDoMtx_stack_c::copy(mpModelMorf[0]->getModel()->getAnmMtx(13));
        mDoMtx_stack_c::multVec(&pos, &pos);
        actor->current.pos = pos;
    }
}

void daNpc_Kn_c::setSwordChargePtcl() {
    JPABaseEmitter* emitter;

    if (mMotionSeqMngr.getNo() == 10 && mMotionSeqMngr.getStepNo() == 0 &&
        !mMotionSeqMngr.checkEntryNewMotion())
    {
        f32 frame = mpModelMorf[0]->getFrame();
        if (frame >= 17.0f && frame < 37.0f) {
            static cXyz lightAParticleScale(1.5223f, 1.3f, 1.3f);
            static cXyz lightALocalTrans(88.399994f, 0.0f, 0.0f);

            f32 temp_f29 = 156.0f;
            f32 var_f30 = frame;

            field_0x15d4[0] = dComIfGp_particle_set(field_0x15d4[0], ID_ZI_J_SWA_KIRARI_A, &current.pos, &tevStr);

            emitter = dComIfGp_particle_getEmitter(field_0x15d4[0]);
            if (emitter != NULL) {
                emitter->setGlobalRTMatrix(mpModelMorf[0]->getModel()->getAnmMtx(0xd));
                emitter->setGlobalParticleScale(lightAParticleScale);
                emitter->setLocalTranslation(lightALocalTrans);
            }

            cXyz sp50;
            cXyz sp44;
            mDoMtx_stack_c::copy(mpModelMorf[0]->getModel()->getAnmMtx(0xd));
            mDoMtx_stack_c::multVecSR(&cXyz::BaseX, &sp50);
            mDoMtx_stack_c::multVecZero(&sp44);

            if (var_f30 >= 31.0f)
                var_f30 = 31.0f;

            sp50 = sp44 + (sp50 * ((var_f30 + 1.0f) - 17.0f)) * (temp_f29 / 15.0f);

            field_0x15d4[1] = dComIfGp_particle_set(field_0x15d4[1], ID_ZI_J_SWA_KIRARI_B, &sp50, &tevStr);
            if (frame >= 31.0f) {
                field_0x15d4[2] = dComIfGp_particle_set(field_0x15d4[2], ID_ZI_J_SWA_KIRARI_C, &sp50, &tevStr);
            }
        } else {
            emitter = dComIfGp_particle_getEmitter(field_0x15d4[0]);
            if (emitter != NULL) {
                u8 temp_r3_3 = emitter->getGlobalAlpha();
                if (temp_r3_3 > 17) {
                    field_0x15d4[0] = dComIfGp_particle_set(field_0x15d4[0], ID_ZI_J_SWA_KIRARI_A, &current.pos, &tevStr);

                    emitter = dComIfGp_particle_getEmitter(field_0x15d4[0]);
                    if (emitter != NULL) {
                        emitter->setGlobalRTMatrix(mpModelMorf[0]->getModel()->getAnmMtx(0xd));
                        emitter->setGlobalAlpha(temp_r3_3 - 17);
                    }
                } else {
                    emitter->setGlobalAlpha(0);
                }
            }

            emitter = dComIfGp_particle_getEmitter(field_0x15d4[1]);
            if (emitter != NULL) {
                emitter->stopDrawParticle();
            }

            emitter = dComIfGp_particle_getEmitter(field_0x15d4[2]);
            if (emitter != NULL) {
                emitter->stopDrawParticle();
            }
        }
    }
}

void daNpc_Kn_c::calcSlip() {
    if (field_0x15bc) {
        if (mAcch.ChkWallHit()) {
            mTargetPos = current.pos;
            field_0x15bc = 0;
            speed.x = 0.0f;
            speed.z = 0.0f;
        } else {
            cXyz pos(current.pos);

            cLib_addCalcPos2(&pos, mTargetPos, 0.25f, 20.0f);
            pos -= current.pos;
            speed.x = pos.x;
            speed.z = pos.z;

            f32 absxz = (mTargetPos - current.pos).absXZ();
            if (absxz > 25.0f) {
                setSlipPrtcl();
            } else if (absxz < 5.0f) {
                field_0x15bc = 0;
                speed.x = 0.0f;
                speed.z = 0.0f;
            }
        }
    }
}

void daNpc_Kn_c::calcSwordAttackMove(int param_0) {
    f32 temp_f31 = (daPy_getPlayerActorClass()->current.pos - current.pos).absXZ();
    if (cLib_calcTimer<s16>(&field_0x15d0) == 1 && field_0x15bc == 0) {
        if (temp_f31 < 250.0f) {
            mFaceMotionSeqMngr.setNo(1, -1.0f, 0, 0);
            mMotionSeqMngr.setNo(0x19, -1.0f, 1, 0);
        } else {
            field_0x15d0 = 2;
        }
    }

    if (field_0x15bc == 0 && field_0x15ce == 0 && (mMotionSeqMngr.getNo() != 0x1B || mMotionSeqMngr.getStepNo()) && temp_f31 > 250.0f) {
        speedF = mpHIO->m.move_speed;
    } else {
        speedF = 0.0f;
        if (field_0x15bc == 0) {
            speed.zero();
        }
    }

    if (field_0x15ce == 0) {
        if (param_0) {
            cLib_addCalcAngleS2(&mCurAngle.y, fopAcM_searchPlayerAngleY(this), 2, 0x800);
        } else {
            cLib_addCalcAngleS2(&mCurAngle.y, fopAcM_searchPlayerAngleY(this), mpHIO->m.rotation_num, mpHIO->m.rotation_speed);
        }

        setAngle(mCurAngle.y);
        mJntAnm.lookPlayer(0);
    } else {
        mJntAnm.lookNone(0);
    }
}

void daNpc_Kn_c::setSe() {
    if (field_0xe2c == 0) {
        switch (field_0x15A8) {
        case 0:
        case 1:
        case 2:
        case 3:
        case 25:
        case 26:
        case 27:
        case 29:
        case 30:
        case 31:
        case 33:
            mSound.startCreatureVoiceLevel(Z2SE_KN_V_BREATH, -1);
            break;
        case 5:
            if (mpModelMorf[0]->getFrame() >= 1.0f && mpModelMorf[0]->getFrame() < 2.0f) {
                mSound.startCreatureVoice(Z2SE_KN_V_GUARD, -1);
            }
            break;
        case 7:
            if (mpModelMorf[0]->getFrame() >= 1.0f && mpModelMorf[0]->getFrame() < 2.0f) {
                mSound.startCreatureVoice(Z2SE_KN_V_DAMAGE_S, -1);
            }
            break;
        case 6:
            if (mpModelMorf[0]->getFrame() >= 1.0f && mpModelMorf[0]->getFrame() < 2.0f) {
                mSound.startCreatureVoice(Z2SE_KN_V_DAMAGE_M, -1);
            }
            break;
        case 16:
            if (mpModelMorf[0]->getFrame() >= 1.0f && mpModelMorf[0]->getFrame() < 2.0f) {
                mSound.startCreatureVoice(Z2SE_KN_V_DAMAGE_L, -1);
            }
            break;
        case 4:
            if (mpModelMorf[0]->getFrame() >= 4.0f && mpModelMorf[0]->getFrame() < 5.0f) {
                mSound.startCreatureVoice(Z2SE_KN_V_ATTACK, -1);
            }
            break;
        case 12:
            if (mpModelMorf[0]->getFrame() >= 1.0f && mpModelMorf[0]->getFrame() < 2.0f) {
                if (mActionMode == 7) {
                    mSound.startCreatureVoice(Z2SE_KN_V_BALL_READY, -1);
                }
            } else if (mpModelMorf[0]->getFrame() >= 27.0f && mpModelMorf[0]->getFrame() < 28.0f) {
                if (mActionMode == 7) {
                    mSound.startCreatureVoice(Z2SE_KN_V_BALL_THROW, -1);
                } else {
                    mSound.startCreatureVoice(Z2SE_KN_V_ATTACK, -1);
                }
            }
            break;
        case 20:
            if (mpModelMorf[0]->getFrame() >= 1.0f && mpModelMorf[0]->getFrame() < 2.0f) {
                mSound.startCreatureVoice(Z2SE_KN_V_SOTOMOGIRI, -1);
            }
            break;
        case 28:
            if (mpModelMorf[0]->getFrame() >= 1.0f && mpModelMorf[0]->getFrame() < 2.0f) {
                mSound.startCreatureVoice(Z2SE_KN_V_KABUTOWARI, -1);
            }
            break;
        case 32:
            if (mpModelMorf[0]->getFrame() >= 1.0f && mpModelMorf[0]->getFrame() < 2.0f) {
                mSound.startCreatureVoice(Z2SE_KN_V_IAIGIRI, -1);
            }
            break;
        case 34:
            if (mpModelMorf[0]->getFrame() >= 1.0f && mpModelMorf[0]->getFrame() < 2.0f) {
                mSound.startCreatureVoice(Z2SE_KN_V_JUMP_ATTACK_L, -1);
            }
            break;
        }
    }
}

static int daNpc_Kn_Create(void* i_this) {
    return ((daNpc_Kn_c*)i_this)->create();
}

static int daNpc_Kn_Delete(void* i_this) {
    return ((daNpc_Kn_c*)i_this)->Delete();
}

static int daNpc_Kn_Execute(void* i_this) {
    return ((daNpc_Kn_c*)i_this)->Execute();
}

static int daNpc_Kn_Draw(void* i_this) {
    return ((daNpc_Kn_c*)i_this)->Draw();
}

static int daNpc_Kn_IsDelete(void* i_this) {
    return 1;
}

#include "d/actor/d_a_npc_kn_teach01.inc"

#include "d/actor/d_a_npc_kn_teach02.inc"

static actor_method_class daNpc_Kn_MethodTable = {
    (process_method_func)daNpc_Kn_Create,  (process_method_func)daNpc_Kn_Delete,
    (process_method_func)daNpc_Kn_Execute, (process_method_func)daNpc_Kn_IsDelete,
    (process_method_func)daNpc_Kn_Draw,
};

actor_process_profile_definition g_profile_NPC_KN = {
    fpcLy_CURRENT_e,         // mLayerID
    7,                       // mListID
    fpcPi_CURRENT_e,         // mListPrio
    PROC_NPC_KN,             // mProcName
    &g_fpcLf_Method.base,    // sub_method
    sizeof(daNpc_Kn_c),      // mSize
    0,                       // mSizeOther
    0,                       // mParameters
    &g_fopAc_Method.base,    // sub_method
    347,                     // mPriority
    &daNpc_Kn_MethodTable,   // sub_method
    0x00044107,              // mStatus
    fopAc_ENEMY_e,           // mActorType
    fopAc_CULLBOX_CUSTOM_e,  // cullType
};

AUDIO_INSTANCES;
