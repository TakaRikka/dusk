/**
 * @file d_a_npc_ykw.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "JSystem/JHostIO/JORFile.h"
#include "Z2AudioLib/Z2Instances.h"
#include "d/actor/d_a_b_yo.h"
#include "d/actor/d_a_door_bossL5.h"
#include "d/actor/d_a_npc_ykm.h"
#include "d/actor/d_a_npc_ykw.h"
#include "d/actor/d_a_tag_evtarea.h"
#include "d/d_debug_viewer.h"
#include "d/d_meter2_info.h"
#include "d/d_timer.h"
#include "d/d_tresure.h"
#include "f_op/f_op_camera_mng.h"
#include "m_Do/m_Do_ext.h"

#if DEBUG
class daNpc_ykW_HIO_c : public mDoHIO_entry_c {
public:
    daNpc_ykW_HIO_c() {
        m = daNpc_ykW_Param_c::m;
    }
    virtual ~daNpc_ykW_HIO_c() {}

    void listenPropertyEvent(const JORPropertyEvent*);

    void genMessage(JORMContext*);

    daNpc_ykW_HIOParam m;
};
#endif

static int l_bmdData[1][2] = {
    {11, 1},
};

static daNpcT_evtData_c l_evtList[10] = {
    {"", 0},
    {"DEFAULT_GETITEM", 0},
    {"NO_RESPONSE", 0},
    {"SHOW_DOOR", 2},
    {"GO_INTO_BOSSROOM1", 2},
    {"GO_INTO_BOSSROOM2", 2},
    {"LV5DUNGEON_CLEAR", 2},
    {"FIND_WOLF", 4},
    {"START_SNOWBOARDRACE", 3},
    {"END_SNOWBOARDRACE", 3},
};

static const char* l_resNameList[5] = {
    "",
    "ykW",
    "ykW1",
    "ykW2",
    "ykM2",
};

static s8 l_loadResPtrn0[2] = {0x01, -1};
static s8 l_loadResPtrn1[3] = {0x01, 0x02, -1};
static s8 l_loadResPtrn2[3] = {0x01, 0x03, -1};
static s8 l_loadResPtrn9[4] = {0x01, 0x02, 0x03, -1};
static s8* l_loadResPtrnList[8] = {
    l_loadResPtrn0, l_loadResPtrn1, l_loadResPtrn1, l_loadResPtrn2,
    l_loadResPtrn1, l_loadResPtrn0, l_loadResPtrn0, l_loadResPtrn9,
};

static daNpcT_faceMotionAnmData_c l_faceMotionAnmData[14] = {
    {-1, 0, 0, 17, 2, 1, 1},
    {6, 0, 1, 17, 2, 1, 1},
    {9, 0, 2, 37, 0, 2, 0},
    {11, 0, 2, 39, 0, 2, 0},
    {10, 0, 2, 38, 0, 2, 0},
    {9, 0, 3, 32, 0, 3, 0},
    {10, 0, 3, 33, 0, 3, 0},
    {13, 2, 3, 17, 2, 1, 1},
    {6, 0, 2, 35, 0, 2, 0},
    {8, 0, 2, 17, 2, 1, 1},
    {7, 0, 2, 36, 0, 2, 0},
    {13, 2, 2, 17, 2, 1, 1},
    {12, 2, 2, 40, 2, 2, 0},
    {-1, 0, 0, 41, 2, 2, 0},
};

static daNpcT_motionAnmData_c l_motionAnmData[35] = {
    {8, 2, 1, 14, 0, 1, 1, 0},
    {22, 2, 2, 14, 0, 1, 1, 0},
    {14, 2, 3, 14, 0, 1, 1, 0},
    {12, 2, 3, 14, 0, 1, 1, 0},
    {29, 2, 2, 14, 0, 1, 1, 0},
    {18, 2, 2, 14, 0, 1, 1, 0},
    {7, 0, 1, 14, 0, 1, 1, 0},
    {16, 0, 2, 14, 0, 1, 1, 0},
    {26, 2, 2, 14, 0, 1, 1, 0},
    {28, 0, 2, 14, 0, 1, 1, 0},
    {27, 2, 2, 14, 0, 1, 1, 0},
    {14, 0, 2, 14, 0, 1, 1, 0},
    {24, 2, 2, 14, 0, 1, 1, 0},
    {15, 0, 2, 14, 0, 1, 1, 0},
    {25, 2, 2, 14, 0, 1, 1, 0},
    {25, 2, 3, 14, 0, 1, 1, 0},
    {23, 2, 3, 14, 0, 1, 1, 0},
    {24, 2, 3, 14, 0, 1, 1, 0},
    {18, 0, 3, 14, 0, 1, 1, 0},
    {19, 2, 3, 14, 0, 1, 1, 0},
    {20, 0, 3, 14, 0, 1, 1, 0},
    {22, 0, 3, 14, 0, 1, 1, 0},
    {21, 0, 3, 14, 0, 1, 1, 0},
    {16, 0, 3, 14, 0, 1, 1, 0},
    {17, 2, 3, 14, 0, 1, 1, 0},
    {20, 0, 2, 14, 0, 1, 1, 0},
    {17, 0, 2, 14, 0, 1, 1, 0},
    {19, 0, 2, 14, 0, 1, 1, 0},
    {21, 2, 2, 32, 2, 2, 0, 0},
    {15, 0, 3, 14, 0, 1, 1, 0},
    {7, 0, 3, 14, 0, 1, 1, 0},
    {6, 0, 3, 14, 0, 1, 1, 0},
    {11, 0, 3, 14, 0, 1, 1, 0},
    {8, 0, 3, 14, 0, 1, 1, 0},
    {26, 0, 3, 29, 0, 3, 0, 0},
};

static daNpcT_MotionSeqMngr_c::sequenceStepData_c l_faceMotionSequenceData[56] = {
    {1, -1, 1}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {4, -1, 1}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {5, -1, 1}, {7, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {6, -1, 1}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {2, -1, 1}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {3, -1, 1}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {8, -1, 1}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {9, -1, 1}, {11, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {10, -1, 1}, {12, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {11, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {12, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {7, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {13, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {0, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
};

static daNpcT_MotionSeqMngr_c::sequenceStepData_c l_motionSequenceData[132] = {
    {0, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {1, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {25, -1, 1}, {1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {2, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {29, -1, 1}, {2, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {30, -1, 1}, {0, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {31, -1, 1}, {0, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {33, -1, 1}, {0, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {34, -1, 1}, {0, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {32, -1, 1}, {3, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {26, -1, 1}, {1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {27, -1, 1}, {1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {28, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {8, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {9, -1, 1}, {10, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {7, -1, 1}, {10, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {10, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {11, -1, 1}, {12, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {12, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {14, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {4, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {5, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {6, 4, 1}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {13, -1, 1}, {14, 0, 1}, {-1, 0, 0}, {-1, 0, 0},
    {15, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {18, -1, 1}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {20, -1, 1}, {15, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {16, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {17, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {21, 0, 1}, {15, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {22, 0, 1}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {29, -1, 1}, {2, -1, 2}, {0, -1, 0}, {-1, 0, 0},
    {23, 0, 1}, {24, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
};

const char* daNpc_ykW_c::mCutNameList[8] = {
    "",
    "SHOW_DOOR",
    "GO_INTO_BOSSROOM",
    "LV5DUNGEON_CLEAR",
    "FIND_WOLF",
    "START_SNOWBOARDRACE",
    "END_SNOWBOARDRACE",
    "HUG",
};

daNpc_ykW_c::cutFunc daNpc_ykW_c::mCutList[8] = {
    NULL,
    &daNpc_ykW_c::cutShowDoor,
    &daNpc_ykW_c::cutGoIntoBossRoom,
    &daNpc_ykW_c::cutLv5DungeonClear,
    &daNpc_ykW_c::cutFindWolf,
    &daNpc_ykW_c::cutStartSnowboardRace,
    &daNpc_ykW_c::cutEndSnowboardRace,
    &daNpc_ykW_c::cutHug,
};

#if DEBUG
static daNpc_ykW_HIO_c l_HIO;
#else
static daNpc_ykW_Param_c l_HIO;
#endif

#if DEBUG
void daNpc_ykW_HIO_c::listenPropertyEvent(const JORPropertyEvent* event) {
    char buffer[2000];
    int sp8;

    JORReflexible::listenPropertyEvent(event);
    JORFile sp10;
    switch ((int)event->id) {
    case 0x40000002:
        if (sp10.open(6, "", NULL, NULL, NULL) != 0) {
            memset(&buffer, 0, sizeof(buffer));
            sp8 = 0;

            daNpcT_cmnListenPropertyEvent(buffer, &sp8, &m.common);
            sprintf(buffer + sp8, "%.3ff,\t//  飛ばしフレ－ム\n", m.mSkipFrame);
            sp8 = strlen(buffer);
            sprintf(buffer + sp8, "%.3ff,\t//  飛ばし速度\n", m.mFlySpeed);
            sp8 = strlen(buffer);
            sprintf(buffer + sp8, "%.3ff,\t//  飛ばし角度\n", m.mFlyAngle);
            sp8 = strlen(buffer);
            sprintf(buffer + sp8, "%.3ff,\t//  ジャンプ初速\n", m.mJumpInitialSpeed);
            sp8 = strlen(buffer);
            sprintf(buffer + sp8, "%.3ff,\t//  滑り加速\n", m.mSlidingAccel);
            sp8 = strlen(buffer);
            sprintf(buffer + sp8, "%.3ff,\t//  滑り速度\n", m.mSlidingSpeed);
            sp8 = strlen(buffer);
            sprintf(buffer + sp8, "%d,   \t//  競争パラメ－タａ\n", m.mCompetParamA);
            sp8 = strlen(buffer);
            sprintf(buffer + sp8, "%d,   \t//  競争パラメ－タｂ\n", m.mCompetParamB);
            sp8 = strlen(buffer);
            sprintf(buffer + sp8, "%.3ff,\t//  競争パラメ－タｃ\n", m.mCompetParamC);
            sp8 = strlen(buffer);
            sprintf(buffer + sp8, "%d,   \t//  歩き時間\n", m.mWalkingTime);
            sp8 = strlen(buffer);
            sprintf(buffer + sp8, "%d,   \t//  停止時間\n", m.mDownTime);
            sp8 = strlen(buffer);
            sprintf(buffer + sp8, "%d,   \t//  ステップの速さ\n", m.mStepSpeed);
            sp8 = strlen(buffer);

            sp10.writeData(buffer, sp8);

            sp10.close();
            OSReport("write append success!::%6d\n", sp8);
        } else {
            OSReport("write append failure!\n");
        }
    }
}

void daNpc_ykW_HIO_c::genMessage(JORMContext* ctx) {
    daNpcT_cmnGenMessage(ctx, &m.common);
    ctx->genSlider("飛ばしフレ－ム  ", &m.mSkipFrame, 0.0f, 100.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    ctx->genSlider("飛ばし速度      ", &m.mFlySpeed, 0.0f, 100.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    ctx->genSlider("飛ばし角度      ", &m.mFlyAngle, 0.0f, 90.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    ctx->genSlider("ジャンプ初速    ", &m.mJumpInitialSpeed, 0.0f, 100.0f, 0, NULL, 0xffff,
                   0xffff, 0x200, 0x18);
    ctx->genSlider("滑り加速        ", &m.mSlidingAccel, 0.0f, 8.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    ctx->genSlider("滑り速度        ", &m.mSlidingSpeed, 0.0f, 128.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    ctx->genSlider("競争パラメ－タａ", &m.mCompetParamA, -100, 100, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    ctx->genSlider("競争パラメ－タｂ", &m.mCompetParamB, 1, 100, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    ctx->genSlider("競争パラメ－タｃ", &m.mCompetParamC, 0.0, 10.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    ctx->genSlider("歩き時間        ", &m.mWalkingTime, 0, 10000, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    ctx->genSlider("停止時間        ", &m.mDownTime, 0, 10000, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    ctx->genSlider("ステップの速さ  ", &m.mStepSpeed, 0, 100, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // "File export"
    ctx->genButton("ファイル書き出し", 0x40000002, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
}
#endif

daNpc_ykW_c::~daNpc_ykW_c() {
    // "daNpc_ykW_c -> destruct"
    OS_REPORT("|%06d:%x|daNpc_ykW_c -> デストラクト\n", g_Counter.mCounter0, this);

    if (heap != NULL) {
        mpMorf[0]->stopZelAnime();
    }

#if DEBUG
    if (mpHIO != NULL) {
        mpHIO->removeHIO();
    }
#endif

    deleteRes(l_loadResPtrnList[field_0xf80], l_resNameList);

    if (field_0x106a != 0) {
        mDoAud_subBgmStop();
    }

    daNpcT_offTmpBit(0x55);
}

const daNpc_ykW_HIOParam daNpc_ykW_Param_c::m = {
    {
        250.0f, -3.0f, 1.0f, 500.0f, 255.0f, 230.0f, 35.0f, 60.0f,
        0.0f, 0.0f, 20.0f, -20.0f, 15.0f, -15.0f, 20.0f, -20.0f,
        0.6f, 12.0f,
        3, 6, 5, 6,
        110.0f, 0.0f, 0.0f, 0.0f,
        60, 8, 0, 0, 0,
        0, 0,
        4.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    },
    2.0f, 20.0f, 50.0f, 56.0f, 0.7f, 58.0f,
    3, 5,
    0.6f,
    360, 0, 30,
};

static u32 const heapSize[8] = {
    0x8000, 0x3310, 0x8000, 0x8000,
    0x3310, 0x8000, 0x8000, 0x8000,
};

int daNpc_ykW_c::create() {
    daNpcT_ct(this, daNpc_ykW_c, l_faceMotionAnmData, l_motionAnmData,
                       l_faceMotionSequenceData, 4, l_motionSequenceData, 4, l_evtList,
                       (char**)l_resNameList);
    field_0xf80 = getType();
    mFlowNodeNo = getFlowNodeNo();
    mTwilight = false;

    int loadResult = loadRes(l_loadResPtrnList[field_0xf80], l_resNameList);
    if (loadResult == cPhs_COMPLEATE_e) {
        OS_REPORT("\t(%s:%d) flowNo:%d, PathID:%02x, BitSW:%02x, BitSW2:%02x, BitTRB:%02x<%08x> ",
                  fopAcM_getProcNameString(this), (u32)field_0xf80, mFlowNodeNo, getPathID(),
                  getBitSW(), getBitSW2(), getBitTRB(), fopAcM_GetParam(this));

        if (isDelete()) {
            OS_REPORT("===>isDelete:TRUE\n");
            return cPhs_ERROR_e;
        }

        OS_REPORT("\n");
        OS_REPORT("YKW: %u 0x%04x\n", (u32)field_0xf80, heapSize[field_0xf80]);

        if (!fopAcM_entrySolidHeap(this, createHeapCallBack, heapSize[field_0xf80])) {
            return cPhs_ERROR_e;
        }

        J3DModelData* modelData = mpMorf[0]->getModel()->getModelData();
        fopAcM_SetMtx(this, mpMorf[0]->getModel()->getBaseTRMtx());

        fopAcM_setCullSizeBox(this, -300.0f, -100.0f, -300.0f, 300.0f, 300.0f, 300.0f);

        mSound.init(&current.pos, &eyePos, 3, 1);

        field_0x9c0.init(&mAcch, 0.0f, 0.0f);

#if DEBUG
        this->mpHIO = &l_HIO;
        // "Snow woman"
        mpHIO->entryHIO("雪女");
#endif

        reset();

        mAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1, &mAcchCir,
                  fopAcM_GetSpeed_p(this), fopAcM_GetAngle_p(this), fopAcM_GetShapeAngle_p(this));

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

        mCreating = true;
        Execute();
        mCreating = false;
    }

    return loadResult;
}

f32 dummy_65536f() {
    return 65536.0f;
}

f32 dummy_0_2f() {
    return 0.2f;
}

f32 dummy_1f() {
    return 1.0f;
}

int daNpc_ykW_c::CreateHeap() {
    void* mdlData_p = NULL;
    J3DModel* model = NULL;

    int bmdDataIdx = 0;
    int arcNameIdx = l_bmdData[bmdDataIdx][1];
    int resIndex = l_bmdData[bmdDataIdx][0];
    mdlData_p = dComIfG_getObjectRes(l_resNameList[arcNameIdx], resIndex);
    if (mdlData_p == NULL) {
        return 0;
    }

    int temp1 = 0x11020284;
    mpMorf[0] = new mDoExt_McaMorfSO((J3DModelData*)mdlData_p, NULL, NULL, NULL, -1, 1.0f, 0, -1, &mSound,
                                     0x80000, temp1);

    if (mpMorf[0] == NULL || mpMorf[0]->getModel() == NULL) {
        if (mpMorf[0] != NULL) {
            mpMorf[0]->stopZelAnime();
        }

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

    if (setFaceMotionAnm(1, false) && setMotionAnm(0, 0, 0)) {
        return 1;
    }

    return 0;
}

int daNpc_ykW_c::Delete() {
    OS_REPORT("|%06d:%x|daNpc_ykW_c -> Delete\n", g_Counter.mCounter0, this);
    fpc_ProcID id = fopAcM_GetID(this);
    this->~daNpc_ykW_c();
    return 1;
}

int daNpc_ykW_c::Execute() {
    return execute();
}

int daNpc_ykW_c::Draw() {
    int result = 0;

    if (mpMatAnm[0] != NULL) {
        J3DModelData* modelData = mpMorf[0]->getModel()->getModelData();
        modelData->getMaterialNodePointer(getEyeballMaterialNo())->setMaterialAnm(mpMatAnm[0]);
    }

    result = draw(
#if DEBUG
        chkAction(NULL),
#else
        FALSE,
#endif
        0, mRealShadowSize, NULL, 100.0f, 0, 0, 0
    );

    if (field_0x1069 != 0) {
        fopAc_ac_c* actor = mActorMngrs[3].getActorP();
        if (actor != NULL) {
            dComIfGd_addRealShadow(mShadowKey, ((daNpcT_c*)actor)->mpMorf[0]->getModel());
        }
    }

    return result;
}

int daNpc_ykW_c::createHeapCallBack(fopAc_ac_c* i_this) {
    daNpc_ykW_c* ykw = (daNpc_ykW_c*)i_this;
    return ykw->CreateHeap();
}

int daNpc_ykW_c::ctrlJointCallBack(J3DJoint* i_joint, int param_1) {
    if (param_1 == NULL) {
        J3DModel* model = j3dSys.getModel();
        daNpc_ykW_c* ykw = (daNpc_ykW_c*)model->getUserArea();
        if (ykw != 0) {
            ykw->ctrlJoint(i_joint, model);
        }
    }
    return 1;
}

void* daNpc_ykW_c::srchItemHeart(void* param_0, void* param_1) {
    daNpcT_c* npc = (daNpcT_c*)param_1;

    if (mFindCount < 50) {
        fopAc_ac_c* item = (fopAc_ac_c*)param_0;
        if (item != NULL && item != npc && fopAcM_IsActor(item))
        {
            if (fopAcM_IsExecuting(fopAcM_GetID(item)) && fopAcM_GetName(item) == PROC_ITEM &&
                ((daItemBase_c*)item)->getItemNo() == 0)
            {
                npc->mFindActorPtrs[mFindCount] = item;
                mFindCount++;
            }
        }
    }
    return 0;
}

void* daNpc_ykW_c::srchYkw(void* param_0, void* param_1) {
    fopAc_ac_c* actor1 = (fopAc_ac_c*)param_0;
    fopAc_ac_c* actor2 = (fopAc_ac_c*)param_1;
    if (mFindCount < 50 && param_0 != NULL && actor1 != actor2) {
        if (fopAcM_IsExecuting(fopAcM_GetID(actor1)) && fopAcM_GetName(actor1) == PROC_NPC_YKW) {
            mFindActorPtrs[mFindCount] = actor1;
            mFindCount++;
        }
    }
    return 0;
}

fopAc_ac_c* daNpc_ykW_c::getOtherYkwP(int param_0) {
    fopAc_ac_c* actor = 0;
    f32 unkFloat1 = G_CM3D_F_INF;
    mFindCount = 0;
    fopAcM_Search(srchYkw, this);
    for (int i = 0; i < mFindCount; i++) {

        if (((daNpc_ykW_c*)mFindActorPtrs[i])->getType() == param_0 &&
            fopAcM_searchActorDistance(this, mFindActorPtrs[i]) < unkFloat1)
        {
            unkFloat1 = fopAcM_searchActorDistance(this, mFindActorPtrs[i]);
            actor = mFindActorPtrs[i];
        }
    }
    return actor;
}

void* daNpc_ykW_c::srchYkm(void* param_0, void* param_1) {
    fopAc_ac_c* actor1 = (fopAc_ac_c*)param_0;
    fopAc_ac_c* actor2 = (fopAc_ac_c*)param_1;
    if (mFindCount < 50 && param_0 != NULL && actor1 != actor2) {
        if (fopAcM_IsExecuting(fopAcM_GetID(actor1)) && fopAcM_GetName(actor1) == PROC_NPC_YKM) {
            mFindActorPtrs[mFindCount] = actor1;
            mFindCount++;
        }
    }
    return 0;
}

fopAc_ac_c* daNpc_ykW_c::getOtherYkmP(int param_0) {
    fopAc_ac_c* actor = 0;
    f32 minDist = G_CM3D_F_INF;
    mFindCount = 0;
    fopAcM_Search(srchYkm, this);
    for (int i = 0; i < mFindCount; i++) {
        if (param_0 == ((daNpc_ykM_c*)mFindActorPtrs[i])->getType() &&
            fopAcM_searchActorDistance(this, mFindActorPtrs[i]) < minDist) {
                minDist = fopAcM_searchActorDistance(this, mFindActorPtrs[i]);
                actor = mFindActorPtrs[i];
            }
    }
    return actor;
}

u8 daNpc_ykW_c::getType() {
    u8 param = (u8)(fopAcM_GetParam(this) & 0xf);
    switch (param) {
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
    default:
        return 7;
    }
}

int daNpc_ykW_c::isDelete() {
    switch (field_0xf80) {
    case 0:
        return 0;
    case 1:
        return dComIfGs_isDungeonItemBossKey();
    case 2:
        return !dComIfGs_isDungeonItemBossKey() ||
            (getBitSW2() != 0xff && dComIfGs_isSwitch(getBitSW2(), fopAcM_GetRoomNo(this)));
    case 3:
        return 0;
    case 4:
        return dComIfGs_isStageBossEnemy() && dComIfGs_isStageLife();
    case 5:
        return 0;
    case 6:
        return 0;
    default:
        return 0;
    }
}

void daNpc_ykW_c::reset() {
    cXyz unkXyz1;
    csXyz unkSxyz1;

    u32 memsetSize = (uintptr_t)field_0x1074 - (uintptr_t)&field_0xfd8;

    if (mpMatAnm[0] != NULL) {
        mpMatAnm[0]->initialize();
    }

    initialize();

    for (int i = 0; i < 5; i++) {
        mActorMngrs[i].initialize();
    }

    memset(&field_0xfd8, 0, memsetSize);

    if (getPathID() != 0xff) {
        mPath.initialize();
        mPath.setPathInfo(getPathID(), fopAcM_GetRoomNo(this), 0);
        mpPath = NULL;
        if (field_0xf80 == 3 && mPath.chkNextId()) {
            mpPath = dPath_GetRoomPath(mPath.getPathInfo()->m_nextID, fopAcM_GetRoomNo(this));
        }
    }

    unkSxyz1.setall(0);
    unkSxyz1.y = home.angle.y;

    switch (field_0xf80) {
    case 0:
    case 1:
        break;
    case 2:
        if (daNpcT_chkEvtBit(0x6c)) {
            JUT_ASSERT(0x6bf, mPath.getPathInfo() != NULL);

            unkXyz1 = mPath.getPntPos(mPath.getNumPnts() - 2);
            setPos(unkXyz1);

            field_0x1064 = 1;

            if (getBitSW3() != 0xff) {
                if (!dComIfGs_isSwitch(getBitSW3(), fopAcM_GetRoomNo(this))) {
                    dComIfGs_onSwitch(getBitSW3(), fopAcM_GetRoomNo(this));
                }
            }

            field_0x1066 = 2;
        }

        break;
    case 3:
        eventInfo.setIdx(0);

        break;
    case 4:
        if (dComIfGs_isStageBossEnemy()) {
            field_0x106f = 1;
        }

        mHide = true;
        fopAcM_OnStatus(this, fopAcM_STATUS_UNK_0x8000000);

        break;
    case 5:
    case 6:
        eventInfo.setIdx(1);
        mHide = true;
        fopAcM_OnStatus(this, fopAcM_STATUS_UNK_0x8000000);

        break;
    default:
        break;
    }

    daNpcT_offTmpBit(0x53);
    daNpcT_offTmpBit(0x54);
    daNpcT_offTmpBit(0x55);

    setAngle(unkSxyz1);
}

void daNpc_ykW_c::afterJntAnm(int param_0) {
    if (param_0 == 1) {
        mDoMtx_stack_c::YrotM(-mStagger.getAngleZ(1));
        mDoMtx_stack_c::ZrotM(mStagger.getAngleX(1));
    }
    else if (param_0 == 5) {
        mDoMtx_stack_c::YrotM(-mStagger.getAngleZ(0));
        mDoMtx_stack_c::ZrotM(mStagger.getAngleX(0));
    }
    if (param_0 == 13) {
        mDoMtx_stack_c::ZrotM(field_0xdcc);
    }
    if (param_0 == 16) {
        mDoMtx_stack_c::ZrotM(field_0xdd0);
    }
}

void daNpc_ykW_c::setParam() {
    selectAction();
    srchActors();

    if (field_0xf80 != 5 && field_0xf80 != 6) {
        u32 newAttnFlags = fopAc_AttnFlag_SPEAK_e | fopAc_AttnFlag_TALK_e;
        s16 talkDist = mpHIO->m.common.talk_distance;
        s16 talkAngle = mpHIO->m.common.talk_angle;
        s16 attnDist = mpHIO->m.common.attention_distance;
        s16 attnAngle = mpHIO->m.common.attention_angle;

        if (field_0xf80 == 1) {
            talkAngle = 4;
            attnAngle = 4;
        } else if (field_0xf80 == 3) {
            talkDist = 5;
            attnDist = 7;
        }

        attention_info.distances[fopAc_attn_LOCK_e] = daNpcT_getDistTableIdx(attnDist, attnAngle);
        attention_info.distances[fopAc_attn_TALK_e] = attention_info.distances[fopAc_attn_LOCK_e];
        attention_info.distances[fopAc_attn_SPEAK_e] = daNpcT_getDistTableIdx(talkDist, talkAngle);

        if (daPy_py_c::checkNowWolf()) {
            newAttnFlags |= fopAc_AttnFlag_UNK_0x800000;
        }

        attention_info.flags = newAttnFlags;
    } else {
        attention_info.flags = 0;
    }

    scale.set(mpHIO->m.common.scale,
              mpHIO->m.common.scale,
              mpHIO->m.common.scale);
    mCcStts.SetWeight(mpHIO->m.common.weight);
    mCylH = mpHIO->m.common.height;
    mWallR = mpHIO->m.common.width;
    mAttnFovY = mpHIO->m.common.fov;

    mAcchCir.SetWallR(mWallR);
    mAcchCir.SetWallH(mpHIO->m.common.knee_length);

    mRealShadowSize = mpHIO->m.common.real_shadow_size;

    if (field_0xf80 == 4) {
        fopAcM_OffStatus(this, fopAcM_STATUS_UNK_0x100);
        mRealShadowSize = 1500.0f;
    }

    mExpressionMorfFrame = mpHIO->m.common.expression_morf_frame;
    mMorfFrames = mpHIO->m.common.morf_frame;
    gravity = mpHIO->m.common.gravity;
    field_0x1060 = 0;
    field_0x1061 = 0;
}

int daNpc_ykW_c::checkChangeEvt() {
    if (!chkAction(&daNpc_ykW_c::talk)) {
        mPreItemNo = 0;
        if (dComIfGp_event_chkTalkXY()) {
            if (dComIfGp_evmng_ChkPresentEnd()) {
                mEvtNo = 2;
                evtChange();
            }
            return 1;
        }
        switch (field_0xf80) {
        case 3:
            if (daPy_py_c::checkNowWolf()) {
                mEvtNo = 7;
                evtChange();
                return 1;
            }
        }
    }
    return 0;
}

void daNpc_ykW_c::setAfterTalkMotion() {
    int index = 13;
    switch (mFaceMotionSeqMngr.getNo()) {
    case 7:
        index = 9;
        break;
    case 8:
        index = 10;
        break;
    case 2:
        index = 11;
        break;
    }
    mFaceMotionSeqMngr.setNo(index, -1.0f, 0, 0);
}

void daNpc_ykW_c::srchActors() {
    fopAc_ac_c* actor;

    switch (field_0xf80) {
    case 1:
        if (mActorMngrs[0].getActorP() == NULL) {
            mActorMngrs[0].entry(getEvtAreaTagP(10, 0));
        }
        break;
    case 2:
        if (mActorMngrs[2].getActorP() == NULL) {
            mActorMngrs[2].entry(getNearestActorP(0x22b));
        }
        if (mActorMngrs[1].getActorP() == NULL) {
            mActorMngrs[1].entry(getEvtAreaTagP(12, 0));
        }
        if (mActorMngrs[0].getActorP() == NULL) {
            mActorMngrs[0].entry(getEvtAreaTagP(10, 1));
        }
        break;
    case 3:
        if (mActorMngrs[3].getActorP() == NULL) {
            mActorMngrs[3].entry(getOtherYkmP(4));
        }
        break;
    case 4:
        if (mActorMngrs[3].getActorP() == NULL) {
            mActorMngrs[3].entry(getNearestActorP(0x27b));
        }
        if (mActorMngrs[4].getActorP() == NULL) {
            mActorMngrs[4].entry(getNearestActorP(0x211));
        }
        break;
    }
}

int daNpc_ykW_c::evtTalk() {
    if (chkAction(&daNpc_ykW_c::talk)) {
        (this->*field_0xfe4)(NULL);
    } else {
        setAction(&daNpc_ykW_c::talk);
    }
    return 1;
}

int daNpc_ykW_c::evtCutProc() {
    int staffId = dComIfGp_getEventManager().getMyStaffId("ykW", this, -1);
    if (staffId != -1) {
        mStaffId = staffId;
        int cutIndex = dComIfGp_getEventManager().getMyActIdx(mStaffId, mCutNameList, 8, 0, 0);
        if ((this->*mCutList[cutIndex])(mStaffId)) {
            dComIfGp_getEventManager().cutEnd(mStaffId);
        }

        return 1;
    }

    return 0;
}

void daNpc_ykW_c::action() {
    fopAc_ac_c* unkActor1 = NULL;
    unkActor1 = hitChk(&mCyl, -1);
    if (unkActor1 != NULL && field_0x106a != 0) {
        mStagger.setParam(this, unkActor1, mCurAngle.y);
        mMotionSeqMngr.setNo(30, 0.0f, 0, 0);
        mDamageTimer = mCutType == 8 ? 20 : 8;
        mDamageTimerStart = 0;
        mJntAnm.lookNone(1);
        mSound.startCreatureSound(Z2SE_YW_SNOBO_SPIN, 0, -1);
        mSound.startCreatureVoice(Z2SE_YW_V_SNOBO_SPIN, -1);
    }

    if (mStagger.checkRebirth()) {
        mStagger.initialize();
        mMode = 1;
        if (field_0x106a != 0) {
            mMotionSeqMngr.setNo(24, -1.0f, 0, 0);
            mMode = 2;
        }
    }

    if (field_0xfd8 != NULL) {
        if (field_0xfe4 == field_0xfd8) {
            (this->*field_0xfe4)(NULL);
        } else {
            setAction(field_0xfd8);
        }
    }
}

void daNpc_ykW_c::beforeMove() {
    fopAcM_OffStatus(this, fopAcM_STATUS_UNK_0x8000000);
    if (checkHide()) {
        fopAcM_OnStatus(this, fopAcM_STATUS_UNK_0x8000000);
    }
    if (checkHide() || mNoDraw != 0) {
        attention_info.flags = 0;
    }
}

void daNpc_ykW_c::afterMoved() {
    if (field_0x106a != 0 && field_0x106e == 0) {
        int polyAtt = dComIfG_Bgsp().GetPolyAtt0(mAcch.m_gnd);
        if (polyAtt == 13 && 3.0f < speedF) {
            cXyz unkXyz1 = current.pos;
            unkXyz1.y -= 30.0f;
            field_0x105c =
                dComIfGp_particle_setPolyColor(field_0x105c, 0x8b8f, mAcch.m_gnd, &unkXyz1,
                                               &tevStr, &mCurAngle, NULL, 0, NULL, -1, NULL);

            JPABaseEmitter* emitter = dComIfGp_particle_getEmitter(field_0x105c);
            if (emitter != NULL) {
                cXyz unkXyz2;
                f32 unkFloat1 = speedF / mpHIO->m.mSlidingSpeed;
                if (unkFloat1 > 1.0f) {
                    unkFloat1 = 1.0f;
                }
                emitter->setRate(unkFloat1 * 2.0f + 1.0f);
                emitter->setAwayFromCenterSpeed(unkFloat1 * 4.0f + 1.0f);
                unkXyz2.x = unkFloat1 * 0.3f + 0.7f;
                unkXyz2.y = unkXyz2.x;
                unkXyz2.z = unkXyz2.x;
                emitter->setGlobalParticleScale(unkXyz2);
                emitter->setGlobalAlpha(unkFloat1 * 205.0f + 50.0f);
            }
        }

        f32 unkFloat2 = mGroundH - current.pos.y;

        f32 unkFloat3 = current.pos.absXZ(old.pos) * cM_ssin(mGroundAngle);
        if (unkFloat3 < 0.0f) {
            unkFloat3 = 0.0f;
        }

        if (unkFloat2 <= 1e-06f && unkFloat2 >= -mAcchCir.GetWallH() - unkFloat3) {
            current.pos.y = mGroundH;
            mAcch.SetGroundHit();
            speed.y = 0.0f;
        }
    }
}

void daNpc_ykW_c::setAttnPos() {
    cXyz unkXyz1(0.0f, 50.0f, 0.0f);
    if (field_0x106a != 0) {
        if (mStagger.checkStagger() &&
             (mMotionSeqMngr.getNo() != 30 || mMotionSeqMngr.checkEndSequence() != 0))
        {
            mStagger.setRebirth();
        }
    } else {
        mStagger.calc(0);
    }

    f32 unkFloat1 = cM_s2rad(mCurAngle.y - field_0xd7e.y);
    if (&daNpc_ykW_c::sitWait == field_0xfd8) {
        mJntAnm.setParam(
            this, mpMorf[0]->getModel(), &unkXyz1, getBackboneJointNo(), getNeckJointNo(),
            getHeadJointNo(), 0.0f, 0.0f, 0.0f, 0.0f, mpHIO->m.common.head_angleX_min,
            mpHIO->m.common.head_angleX_max, mpHIO->m.common.head_angleY_min,
            mpHIO->m.common.head_angleY_max, mpHIO->m.common.neck_rotation_ratio,
            unkFloat1, NULL);
    } else {
        mJntAnm.setParam(this, mpMorf[0]->getModel(), &unkXyz1, getBackboneJointNo(),
                         getNeckJointNo(), getHeadJointNo(),
                         field_0x1061 != 0 ? 0.0f : mpHIO->m.common.body_angleX_min,
                         field_0x1061 != 0 ? 0.0f : mpHIO->m.common.body_angleX_max,
                         field_0x1061 != 0 ? 0.0f : mpHIO->m.common.body_angleY_min,
                         field_0x1061 != 0 ? 0.0f : mpHIO->m.common.body_angleY_max,
                         field_0x1060 != 0 ? 0.0f : mpHIO->m.common.head_angleX_min,
                         field_0x1060 != 0 ? 0.0f : mpHIO->m.common.head_angleX_max,
                         field_0x1060 != 0 ? 0.0f : mpHIO->m.common.head_angleY_min,
                         field_0x1060 != 0 ? 0.0f : mpHIO->m.common.head_angleY_max,
                         mpHIO->m.common.neck_rotation_ratio, unkFloat1, NULL);
    }

    mJntAnm.calcJntRad(0.2f, 1.0f, unkFloat1);
    setMtx();
    mDoMtx_stack_c::copy(mpMorf[0]->getModel()->getAnmMtx(getHeadJointNo()));
    mDoMtx_stack_c::multVec(&unkXyz1, &eyePos);

    mJntAnm.setEyeAngleX(eyePos, 1.0f, 0);
    mJntAnm.setEyeAngleY(eyePos, mCurAngle.y, 1, 1.0f, 0);

    unkXyz1.set(0.0f, 0.0f, 0.0f);
    unkXyz1.y = mpHIO->m.common.attention_offset;

    if (&daNpc_ykW_c::sitWait == field_0xfd8) {
        unkXyz1.y -= 50.0f;
    }

    if (field_0x1069 != 0) {
        unkXyz1.set(55.0f, 440.0f, 55.0f);
    }

    mDoMtx_stack_c::YrotS(mCurAngle.y);
    mDoMtx_stack_c::multVec(&unkXyz1, &unkXyz1);

    attention_info.position = current.pos + unkXyz1;

    mFootLPolyAngle = 0;
    mFootRPolyAngle = 0;

    if (&daNpc_ykW_c::sitWait != field_0xfd8 && field_0x106a == 0) {
        static cXyz prtclScl(1.0f, 1.0f, 1.0f);
        setFootPos();
        if (3.0f < speedF) {
            setFootPrtcl(&prtclScl, 10.0f, 0);
        }
    }

    cLib_addCalcAngleS2(&field_0xdcc, mFootLPolyAngle, 5, 0x1000);
    cLib_addCalcAngleS2(&field_0xdd0, mFootRPolyAngle, 5, 0x1000);
}

void daNpc_ykW_c::setCollision() {
    cXyz unkXyz1;
    if (!mHide && field_0x1068 == 0) {
        u32 type = 0xd8fbfdff;
        u32 unkInt1 = 31;
        u32 unkInt2 = 0;
        if (dComIfGp_event_runCheck() != FALSE || field_0x1069 != 0) {
            type = 0;
            unkInt1 = 0;
        } else if (mTwilight) {
                type = 0;
                unkInt1 = 0;
        } else {
            if (mStagger.checkStagger()) {
                type = 0;
                unkInt1 = 0;
            }
            if (field_0x106a != 0) {
                unkInt2 = 19;
            }
        }
        mCyl.SetCoSPrm(0x79);
        mCyl.SetTgType(type);
        mCyl.SetTgSPrm(unkInt1);
        mCyl.SetAtType(AT_TYPE_IRON_BALL);
        mCyl.SetAtSPrm(unkInt2);
        mCyl.OnTgNoHitMark();
        unkXyz1.set(0.0f, 0.0f, 0.0f);
        f32 cylH = mCylH;
        f32 wallR = mWallR;
        if (&daNpc_ykW_c::sitWait == field_0xfd8) {
            unkXyz1.set(0.0f, 0.0f, 10.0f);
            cylH = 180.0f;
            wallR = 70.0f;
        } else {
            if (&daNpc_ykW_c::race == field_0xfd8) {
                unkXyz1.set(0.0f, 0.0f, 0.0f);
                cylH = 120.0f;
                wallR = 100.0f;
            }
        }
        mDoMtx_stack_c::YrotS(mCurAngle.y);
        mDoMtx_stack_c::multVec(&unkXyz1, &unkXyz1);
        unkXyz1 += current.pos;
        mCyl.SetH(cylH);
        mCyl.SetR(wallR);
        mCyl.SetC(unkXyz1);
        dComIfG_Ccsp()->Set(&this->mCyl);
    }

    mCyl.ClrCoHit();
    mCyl.ClrTgHit();
    mCyl.ClrAtHit();
}

int daNpc_ykW_c::drawDbgInfo() {
#if DEBUG
    if (mpHIO->m.common.debug_info_ON != 0) {
        f32 juelDist = dComIfGp_getAttention()
                        ->getDistTable(attention_info.distances[fopAc_attn_JUEL_e])
                        .mDistMax;
        f32 talkDist = dComIfGp_getAttention()
                        ->getDistTable(attention_info.distances[fopAc_attn_TALK_e])
                        .mDistMax;

        GXColor juelCirColor = {0x00, 0xc8, 0x00, 0xff};
        dDbVw_drawCircleOpa(attention_info.position, juelDist, juelCirColor, 1, 12);

        GXColor talkCirColor = {0xc8, 0x00, 0x00, 0xff};
        dDbVw_drawCircleOpa(attention_info.position, talkDist, talkCirColor, 1, 12);

        GXColor sph1Color = {0x80, 0x80, 0x80, 0xa0};
        dDbVw_drawSphereXlu(eyePos, 1.0f, sph1Color, 1);

        GXColor sph2Color = {0x80, 0x80, 0x80, 0xa0};
        dDbVw_drawSphereXlu(attention_info.position, 9.0f, sph2Color, 1);

        if (mPath.getPathInfo() != 0) {
            mPath.drawDbgInfo(attention_info.position.y, 3);
        }
    }

    return 0;
#else
    return false;
#endif
}

int daNpc_ykW_c::selectAction() {
    field_0xfd8 = NULL;

#if DEBUG
    if (mpHIO->m.common.debug_mode_ON != 0) {
        field_0xfd8 = &daNpc_ykW_c::test;
        return 1;
    }
#endif

    switch (field_0xf80) {
    case 1:
        field_0xfd8 = &daNpc_ykW_c::sitWait;

        break;
    case 2:
        field_0xfd8 = &daNpc_ykW_c::wait;
        if (field_0x1063 != 0 && field_0x1064 == 0) {
            field_0xfd8 = &daNpc_ykW_c::walk;
        }

        break;
    case 3:
        field_0xfd8 = &daNpc_ykW_c::wait;
        if (field_0x106a != 0) {
            field_0xfd8 = &daNpc_ykW_c::race;
        }

        break;
    default:
        field_0xfd8 = &daNpc_ykW_c::wait;
        break;
    }

    return 1;
}

BOOL daNpc_ykW_c::chkAction(actionFunc i_func) {
    return field_0xfe4 == i_func;
}

BOOL daNpc_ykW_c::setAction(actionFunc i_func) {
    mMode = 3;
    if (field_0xfe4 != NULL) {
        (this->*field_0xfe4)(0);
    }

    mMode = 0;
    field_0xfe4 = i_func;
    if (field_0xfe4 != NULL) {
        (this->*field_0xfe4)(0);
    }

    return 1;
}

cXyz daNpc_ykW_c::getGoalPos() {
    fopAc_ac_c* actor_p = NULL;
    cXyz result;

    actor_p = mActorMngrs[2].getActorP();
    JUT_ASSERT(0xa16, actor_p != NULL);

    result = daPy_getPlayerActorClass()->current.pos - actor_p->current.pos;

    mDoMtx_stack_c::YrotS(-actor_p->current.angle.y);
    mDoMtx_stack_c::multVec(&result, &result);

    result.x = result.x * 0.5f;
    result.y = 0.0f;
    result.z = -1000.0f;

    mDoMtx_stack_c::transS(actor_p->current.pos);
    mDoMtx_stack_c::YrotM(actor_p->current.angle.y);
    mDoMtx_stack_c::multVec(&result, &result);

    result.y = 0.0f;

    return result;
}

int daNpc_ykW_c::orderGoIntoBossRoomEvt() {
    if (field_0xf80 == 2 &&
        getBitSW2() != 0xff &&
        !dComIfGs_isSwitch(getBitSW2(), fopAcM_GetRoomNo(this)) &&
        getBitSW() != 0xff &&
        dComIfGs_isSwitch(getBitSW(), fopAcM_GetRoomNo(this))) {
        return 1;
    }

    return 0;
}

fopAc_ac_c* daNpc_ykW_c::putUtuwaHeart(cXyz* i_pos, f32 i_speedF, f32 i_speedY, s16 i_yPos,
                                    cXyz* i_scale) {
    fopAc_ac_c* heartContainer = NULL;
    csXyz heartAngle;

    if (!dComIfGs_isStageLife()) {
        cXyz heartPos(211.0f, 0.0f, -900.0f);
        if (i_pos != NULL) {
            heartPos.x = i_pos->x;
            heartPos.y = i_pos->y;
            heartPos.z = i_pos->z;
        }

        if (mItemPartnerId == fpcM_ERROR_PROCESS_ID_e) {
            cXyz scale(1.0f, 1.0f, 1.0f);
            if (i_scale != NULL) {
                scale.x = i_scale->x;
                scale.y = i_scale->y;
                scale.z = i_scale->z;
            }

            heartAngle.setall(0);
            heartAngle.y = i_yPos;
            mItemPartnerId =
                fopAcM_createItemForBoss(&heartPos, 0x22, fopAcM_GetRoomNo(this), &heartAngle,
                                         &scale, i_speedF, i_speedY, -1);
        } else if (fopAcM_SearchByID(mItemPartnerId, &heartContainer) && heartContainer != NULL) {
            if (fopAcM_IsActor(heartContainer) && fopAcM_IsExecuting(mItemPartnerId)) {
                heartContainer->old.pos = heartPos;
                heartContainer->current.pos = heartPos;
                heartContainer->speedF = 0;
                heartContainer->speed.setall(0.0f);
            } else {
                heartContainer = 0;
            }
        }
    }

    return heartContainer;
}

BOOL daNpc_ykW_c::cutShowDoor(int param_0) {
    BOOL retval = FALSE;
    int* param_p = NULL;

    int prm = -1;
    u32 timer = 0;
    int msgNo = 0;
    int send = 0;

    param_p = dComIfGp_evmng_getMyIntegerP(param_0, "prm");
    if (param_p != NULL) {
        prm = *param_p;
    }

    param_p = dComIfGp_evmng_getMyIntegerP(param_0, "timer");
    if (param_p != NULL) {
        timer = *param_p;
    }

    param_p = dComIfGp_evmng_getMyIntegerP(param_0, "msgNo");
    if (param_p != NULL) {
        msgNo = *param_p;
    }

    param_p = dComIfGp_evmng_getMyIntegerP(param_0, "send");
    if (param_p != NULL) {
        send = *param_p;
    }

    if (dComIfGp_getEventManager().getIsAddvance(param_0)) {
        switch (prm) {
        case 0:
            dMeter2Info_setPauseStatus(4);
            mEventTimer = timer;
            break;
        case 1:
            initTalk(mFlowNodeNo, NULL);
            break;
        case 2:
        default:
            break;
        }
    }

    int talkMsgNos[2] = {-1, -1};

    switch (prm) {
    case 0:
        if (cLib_calcTimer(&mEventTimer) == 0) {
            retval = TRUE;
        }

        break;
    case 1:
    case 2:
        talkMsgNos[0] = msgNo;
        if (talkProc(talkMsgNos, send, NULL, 0)) {
            if (msgNo == 0) {
                if (mFlow.checkEndFlow()) {
                    if (prm == 2) {
                        field_0x1062 = 0;
                    }
                    retval = TRUE;
                }
            } else {
                retval = TRUE;
            }
        }

        break;
    default:
        break;
    }

    return retval;
}

int daNpc_ykW_c::cutGoIntoBossRoom(int param_0) {
    char unkStrBuf1[100];

    fopAc_ac_c* bossDoor = NULL;

    cXyz pathPntPos1;
    cXyz pathPntPos2;

    u32 retval = 0;

    s16 targetAngleY;
    f32 unkFloat1;
    f32 unkFloat2;
    f32 unkFloat3;

    int* param_p = NULL;
    int prm = -1;
    u32 timer = 0;
    int skip = 0;
    param_p = dComIfGp_evmng_getMyIntegerP(param_0, "prm");
    if (param_p != NULL) {
        prm = *param_p;
    }
    param_p = dComIfGp_evmng_getMyIntegerP(param_0, "timer");
    if (param_p != NULL) {
        timer = *param_p;
    }
    param_p = dComIfGp_evmng_getMyIntegerP(param_0, "skip");
    if (param_p != NULL) {
        skip = *param_p;
    }

    JUT_ASSERT(0xb15, mPath.getPathInfo() != NULL);
    if (dComIfGp_getEventManager().getIsAddvance(param_0)) {
        switch (prm) {
        case 0:
            bossDoor = mActorMngrs[2].getActorP();
            if (bossDoor != NULL) {
                ((daBdoorL5_c*)bossDoor)->deleteKey();
            }

            pathPntPos1 = mPath.getPntPos(mPath.getNumPnts() - 1);

            targetAngleY = cLib_targetAngleY(fopAcM_GetPosition_p(this), &pathPntPos1);
            setAngle(targetAngleY);

            // fallthrough
        case 10:
            initTalk(mFlowNodeNo, NULL);
            if (getBitSW2() != 0xff) {
                if (!dComIfGs_isSwitch(getBitSW2(), fopAcM_GetRoomNo(this))) {
                    dComIfGs_onSwitch(getBitSW2(), fopAcM_GetRoomNo(this));
                }
            }

            break;
        case 1:
            mFaceMotionSeqMngr.setNo(13, -1.0f, 0, 0);
            mMotionSeqMngr.setNo(0, -1.0f, 0, 0);
            field_0x102c = 0;
            field_0x1028 = 0;
            mEventTimer = timer;
            field_0x1063 = 1;
            field_0x1064 = 0;
            field_0x1066 = 3;
            mPath.setIdx(mPath.getNumPnts() - 2);
            setParam();

            break;
        case 2:
            mEventTimer = timer;
            pathPntPos1 = mPath.getPntPos(mPath.getNumPnts() - 2);
            pathPntPos2 = mPath.getPntPos(mPath.getNumPnts() - 1);
            targetAngleY = cLib_targetAngleY(&pathPntPos1, &pathPntPos2);
            pathPntPos2.set(0.0f, 0.0f, 250.0f);
            mDoMtx_stack_c::transS(pathPntPos1);
            mDoMtx_stack_c::YrotM(targetAngleY);
            mDoMtx_stack_c::multVec(&pathPntPos2, &pathPntPos2);
            setPos(pathPntPos2);

            break;
        case 4:
            mPath.setIdx(mPath.getNumPnts() - 4);
            pathPntPos1 = mPath.getPntPos(mPath.getNumPnts() - 5);
            pathPntPos2 = mPath.getPntPos(mPath.getNumPnts() - 4);
            targetAngleY = cLib_targetAngleY(&pathPntPos1, &pathPntPos2);
            setPos(pathPntPos1);
            setAngle(targetAngleY);

            break;
        case 5:
            break;
        case 99:
            if (getBitSW2() != 0xff) {
                if (!dComIfGs_isSwitch(getBitSW2(), fopAcM_GetRoomNo(this))) {
                    dComIfGs_onSwitch(getBitSW2(), fopAcM_GetRoomNo(this));
                }
            }
            if (getBitSW3() != 0xff) {
                if (!dComIfGs_isSwitch(getBitSW3(), fopAcM_GetRoomNo(this))) {
                    dComIfGs_onSwitch(getBitSW3(), fopAcM_GetRoomNo(this));
                }
            }
            mFlow.remove();

            break;
        }

        if (skip != 0 && (prm == 0 || prm == 4)) {
            if (prm == 0) {
                strcpy(unkStrBuf1, l_evtList[4].eventName);
            } else {
                strcpy(unkStrBuf1, l_evtList[5].eventName);
            }
            strcat(unkStrBuf1, "@");
            dComIfGp_getEvent().setSkipZev(this, unkStrBuf1);
        }
    }

    switch (prm) {
    case 0:
    case 10:
        mJntAnm.lookPlayer(0);

        pathPntPos1 = mPath.getPntPos(mPath.getNumPnts() - 1);
        targetAngleY = cLib_targetAngleY(fopAcM_GetPosition_p(this), &pathPntPos1);
        if (targetAngleY != mCurAngle.y) {
            step(targetAngleY, 13, 22, mpHIO->m.mStepSpeed, 0);
            break;
        }

        if (talkProc(NULL, 0, NULL, 0) == 0) {
            break;
        }

        if (!mFlow.checkEndFlow()) {
            break;
        }

        bossDoor = mActorMngrs[2].getActorP();
        JUT_ASSERT(0xb9e, bossDoor != NULL);

        dComIfGp_getEvent().setPt2(bossDoor);

        retval = 1;
        break;
    case 1:
    case 2:
    case 4:
        if (mEventTimer != 0 || dComIfGp_getEventManager().getIsAddvance(param_0)) {
            if (!cLib_calcTimer(&mEventTimer)) {
                if (prm == 1) {
                    pathPntPos1 = mPath.getPntPos(mPath.getNumPnts() - 2);
                    pathPntPos2 = mPath.getPntPos(mPath.getNumPnts() - 1);

                    if (cM3d_Len2dSqPntAndSegLine(
                        daPy_getPlayerActorClass()->current.pos.x, daPy_getPlayerActorClass()->current.pos.z, pathPntPos1.x,
                        pathPntPos1.z, pathPntPos2.x, pathPntPos2.z, &unkFloat1, &unkFloat2, &unkFloat3)) {
                        unkFloat3 = std::sqrt(unkFloat3);
                        if (unkFloat3 < mWallR * 2.0f)
                        {
                            targetAngleY = cLib_targetAngleY(&pathPntPos1, &pathPntPos2);
                            pathPntPos1.set(unkFloat1, 0.0f, unkFloat2);
                            pathPntPos2.set(daPy_getPlayerActorClass()->current.pos.x, 0.0f, daPy_getPlayerActorClass()->current.pos.z);
                            if ((s16)(cLib_targetAngleY(&pathPntPos1, &pathPntPos2) - targetAngleY) > 0) {
                                targetAngleY = targetAngleY + 0x4000;
                            } else {
                                targetAngleY = targetAngleY + -0x4000;
                            }

                            mDoMtx_stack_c::transS(pathPntPos1);
                            mDoMtx_stack_c::YrotM(targetAngleY);
                            pathPntPos2.set(0.0f, 0.0f, mWallR * 2.0f);
                            mDoMtx_stack_c::multVec(&pathPntPos2, &pathPntPos2);
                            pathPntPos1.x = pathPntPos2.x;
                            pathPntPos1.y = daPy_getPlayerActorClass()->current.pos.y;
                            pathPntPos1.z = pathPntPos2.z;
                            daPy_getPlayerActorClass()->setPlayerPosAndAngle(
                                &pathPntPos1, daPy_getPlayerActorClass()->shape_angle.y, 0);
                        }
                    }

                    pathPntPos1 = mPath.getPntPos(mPath.getNumPnts() - 2);
                    pathPntPos2 = mPath.getPntPos(mPath.getNumPnts() - 1);
                    targetAngleY = cLib_targetAngleY(&pathPntPos1, &pathPntPos2);
                    pathPntPos2.set(0.0f, 0.0f, 250.0f);

                    mDoMtx_stack_c::transS(pathPntPos1);
                    mDoMtx_stack_c::YrotM(targetAngleY);
                    mDoMtx_stack_c::multVec(&pathPntPos2, &pathPntPos2);
                    targetAngleY = cLib_targetAngleY(fopAcM_GetPosition_p(daPy_getPlayerActorClass()), &pathPntPos2);
                    pathPntPos1 = daPy_getPlayerActorClass()->current.pos;
                    daPy_getPlayerActorClass()->setPlayerPosAndAngle(&pathPntPos1, targetAngleY, 0);
                } else if (prm == 2) {
                    cXyz goalPos = getGoalPos();
                    dComIfGp_evmng_setGoal(&goalPos);
                }
            }
        }
        retval = mEventTimer == 0;
    case 3:
    case 5:
        if (prm == 3) {
            dStage_changeScene(getExitNo(), 10.0f, 0, (int)fopAcM_GetRoomNo(this), daPy_getPlayerActorClass()->shape_angle.y, -1);
        }
        mPlayerActorMngr.remove();
        action();
        if (prm == 5 && field_0x1064 != 0) {
            retval = 1;
        }
        mCcStts.SetWeight(0xff);
        if (prm != 4 && prm != 5) {
            gravity = 0.0f;
        }

        break;
    case 99:
        speedF = 0.0f;
        speed.setall(0.0f);
        pathPntPos1 = mPath.getPntPos(mPath.getNumPnts() - 2);
        pathPntPos2 = mPath.getPntPos(mPath.getNumPnts() - 1);
        targetAngleY = cLib_targetAngleY(&pathPntPos1, &pathPntPos2);
        dStage_changeScene(getExitNo(), 10.0f, 0, (int)fopAcM_GetRoomNo(this), targetAngleY, -1);
        break;
    }

    return retval;
}

BOOL daNpc_ykW_c::cutLv5DungeonClear(int param_0) {
    fopAc_ac_c* unkActor1 = NULL;
    camera_class* camera = NULL;

    cXyz newPlayerPos;
    cXyz itemPos;

    int retval = 0;

    int* param_p = NULL;
    int prm = -1;
    int msgNo = 0;
    int send = 0;
    u32 timer = 0;

    u8 unkByte1 = field_0x1070;

    param_p = dComIfGp_evmng_getMyIntegerP(param_0, "prm");
    if (param_p != NULL) {
        prm = *param_p;
    }

    param_p = dComIfGp_evmng_getMyIntegerP(param_0, "msgNo");
    if (param_p != NULL) {
        msgNo = *param_p;
    }

    param_p = dComIfGp_evmng_getMyIntegerP(param_0, "send");
    if (param_p != NULL) {
        send = *param_p;
    }

    param_p = dComIfGp_evmng_getMyIntegerP(param_0, "timer");
    if (param_p != NULL) {
        timer = *param_p;
    }

    fopAc_ac_c* actors[2] = {this, mActorMngrs[3].getActorP()};
    dComIfGp_setMesgCameraInfoActor(actors[0], actors[1], NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

    f32 unkFloat1;
    f32 unkFloat2;
    f32 unkFloat3;
    int unkInt1;
    int unkInt2;

    if (dComIfGp_getEventManager().getIsAddvance(param_0) != 0) {
        switch (prm) {
        case 0:
            mFaceMotionSeqMngr.setNo(12, 0.0f, 0, 0);
            mMotionSeqMngr.setNo(13, 0.0f, 0, 0);

            mItemPartnerId = fpcM_ERROR_PROCESS_ID_e;

            mHide = false;

            fopAcM_OffStatus(this, fopAcM_STATUS_UNK_0x8000000);

            newPlayerPos.setall(0.0f);

            daPy_getPlayerActorClass()->setPlayerPosAndAngle(&newPlayerPos, -0x8000, 0);

            field_0x1070 = 0;
            unkByte1 = field_0x1070;
            field_0x1068 = 1;

            unkActor1 = mActorMngrs[4].getActorP();
            if (unkActor1 != NULL) {
                ((daB_YO_c*)unkActor1)->setDrawOff();
            }

            break;
        case 2:
            daNpcT_offTmpBit(11);
            daNpcT_offTmpBit(12);
            daNpcT_offTmpBit(13);
        case 3:
        case 5:
        case 20:
            initTalk(0x1f9, actors);

            break;
        case 4:
            mFaceMotionSeqMngr.setNo(0, -1.0f, 0, 0);
            mMotionSeqMngr.setNo(15, -1.0f, 0, 0);

            break;
        case 6:
            mFaceMotionSeqMngr.setNo(7, 0.0f, 0, 0);
            mMotionSeqMngr.setNo(16, 0.0f, 0, 0);

            break;
        case 7:
            mFaceMotionSeqMngr.setNo(7, -1.0f, 0, 0);
            mMotionSeqMngr.setNo(17, -1.0f, 0, 0);

            break;
        case 8:
            mFaceMotionSeqMngr.setNo(8, -1.0f, 0, 0);
            mMotionSeqMngr.setNo(23, -1.0f, 0, 0);

            mSound.startCreatureVoice(0x50109, -1);

            break;
        case 11:
        case 14:
            mEventTimer = timer;

            break;
        case 12:
            mFaceMotionSeqMngr.setNo(6, -1.0f, 0, 0);
            mMotionSeqMngr.setNo(14, -1.0f, 0, 0);

            break;
        case 13:
            unkInt2 = dComIfGp_getPlayerCameraID(0);
            camera = dComIfGp_getCamera(unkInt2);
            JUT_ASSERT(0xc9d, camera != NULL);
            field_0x1058 = cLib_targetAngleY(fopCamM_GetCenter_p(camera), fopCamM_GetEye_p(camera));
            mEventTimer = 6;
            field_0x1038 = 0;

            break;
        case 15:
            field_0x1070 = 2;
            field_0x1030 = timer;

            break;
        case 16:
            mDoAud_bgmStreamPrepare(0x200005a);
            mDoAud_bgmStreamPlay();

            mEventTimer = timer;

            break;
        case 17:
            mDoAud_bgmStreamPrepare(0x200005b);
            mDoAud_bgmStreamPlay();

            break;
        case 21:
            field_0x1069 = 1;
            mFaceMotionSeqMngr.setNo(7, 0.0f, 1, 0);

            break;
        case 99:
            mFaceMotionSeqMngr.setNo(8, 0.0f, 0, 0);
            mMotionSeqMngr.setNo(19, 0.0f, 0, 0);

            setPos(home.pos);
            setAngle(home.angle.y);

            newPlayerPos.set(-196.0f, 0.0f, -21.0f);
            daPy_getPlayerActorClass()->setPlayerPosAndAngle(&newPlayerPos, 0x7800, 0);

            dComIfGp_evmng_setGoal(&newPlayerPos);

            field_0x1069 = 1;
            dKy_custom_colset(0, 0, 0.0f);
            field_0x1070 = 0;

            Z2GetAudioMgr()->bgmStreamStop(0);
            mDoAud_bgmStreamPrepare(0x200005b);
            mDoAud_bgmStreamPlay();

            mFlow.remove();

            break;
        case 1:
        case 9:
        case 10:
        case 22:
        // arbitrary extra values to force generation of a jump table
        case 24:
        case 26:
        default:
            break;
        }
    }


    int unkIntArr1[2] = {-1, -1};

    switch (prm) {
    case 0:
        retval = 1;
        break;
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 20:
    case 21:
    case 22:
    case 24:
        unkIntArr1[0] = msgNo;
        if (prm == 3 && mMotionSeqMngr.getStepNo() > 0 || prm == 21)
        {
            send = 1;
        }

        if (talkProc(unkIntArr1, send, actors, 0)) {
            if (msgNo == 0) {
                if (mFlow.checkEndFlow()) {
                    retval = 1;
                }
            } else {
                retval = 1;
            }
        }

        if ((prm == 4 || prm == 6 ||
             prm == 22 || prm == 20 || prm == 24) &&
            send == 0)
        {
            retval = 1;
        }

        break;
    case 7:
    case 8:
        if (mMotionSeqMngr.getStepNo() > 0) {
            if (prm == 8 && field_0x1070 != 1) {
                field_0x1070 = 1;
                field_0x1030 = timer;
            }
            retval = 1;
        }

        break;
    case 10:
        unkInt2 = dComIfGp_getPlayerCameraID(0);
        camera = dComIfGp_getCamera(unkInt2);
        JUT_ASSERT(0xd15, camera != NULL);
        itemPos.set(-70.0f, 50.0f, -30.0f);
        mDoMtx_stack_c::YrotS(-0x8000);
        mDoMtx_stack_c::multVec(&itemPos, &itemPos);
        itemPos += *fopCamM_GetCenter_p(camera);
        unkActor1 = putUtuwaHeart(&itemPos, 22.0f, 40.0f, 0x5000, 0);
        if (unkActor1 != NULL) {
            dComIfGp_getEvent().setPt2(unkActor1);
            mDoAud_seStart(Z2SE_OBJ_YO_HEART_L, &itemPos, 0, 0);
            retval = 1;
        }

        break;
    case 11:
    case 14:
        if (mEventTimer != 0 || dComIfGp_getEventManager().getIsAddvance(param_0)) {
            if (cLib_calcTimer(&mEventTimer) == 0 && prm == 11) {
                mFindCount = 0;
                fpcM_Search(srchItemHeart, this);
                for (unkInt1 = 0; unkInt1 < mFindCount; unkInt1++) {
                    fopAcM_OffStatus(mFindActorPtrs[unkInt1], fopAcM_STATUS_UNK_0x4000);
                }
            }
        }
        retval = mEventTimer == 0;
        if (prm == 14 && field_0x1054 < 1.0f) {
            retval = 0;
        }

        break;
    case 12:
        retval = 1;

        break;
    case 13:
        if (cLib_calcTimer(&mEventTimer) == 0) {
            cXyz unkXyz1(1.0f, 1.0f, 1.0f);
            csXyz unkSxyz;

            unkFloat1 = (cM_rnd() - 0.5f) * 2.0f;
            unkFloat2 = 10.0f;
            unkFloat3 = 40.0f;
            unkFloat2 = unkFloat2 + unkFloat1 * 1.5f;
            unkFloat3 = unkFloat3 - unkFloat1 * 10.0f;

            unkSxyz.setall(0);
            unkSxyz.y = field_0x1058;

            if ((field_0x1038 & 1) != 0) {
                unkSxyz.y += 0x8000;
            }

            field_0x1058 += cM_deg2s(cM_rndFX(5.0f) + -10.0f);

            itemPos = eyePos;
            itemPos.y += 50.0f;

            unkActor1 = fopAcM_fastCreateItem(&itemPos, 0, fopAcM_GetRoomNo(this), &unkSxyz,
                                             &unkXyz1, &unkFloat2, &unkFloat3, -1, NULL, NULL);
            if (unkActor1 != NULL) {
                mDoAud_seStart(Z2SE_OBJ_YO_HEART_S, &itemPos, 0, 0);
                fopAcM_OnStatus(unkActor1, fopAcM_STATUS_UNK_0x4000);
            }
            mEventTimer = (int)(unkFloat1 * 1.8000001f) + 6;
            field_0x1038++;
        }

        retval = 1;

        break;
    case 15:
        retval = field_0x1030 == 0;

        break;
    case 16:
        if (cLib_calcTimer(&mEventTimer) == 0) {
            retval = 1;
        }

        break;
    case 17:
        retval = 1;

        break;
    case 1:
    case 9:
        break;
    case 99:
        if (putUtuwaHeart(0, 0.0f, 0.0f, 0x5000, 0)) {
            retval = 1;
        }

        break;
    }

    switch (field_0x1070) {
    case 0:
        break;
    case 1:
        if (unkByte1 != field_0x1070) {
            field_0x1054 = 0;
        }

        if (cLib_calcTimer(&field_0x1030) == 0) {
            cLib_chaseF(&field_0x1054, 1.0f, 0.02f);
            dKy_custom_colset(0, 6, field_0x1054);
        }

        break;
    case 2:
        if (unkByte1 != field_0x1070) {
            field_0x1054 = 0;
        }

        if (cLib_calcTimer(&field_0x1030) == 0) {
            cLib_chaseF(&field_0x1054, 1.0f, 0.08f);
            dKy_custom_colset(6, 0, field_0x1054);
        }

        break;
    }

    if (prm != 22) {
        unkActor1 = mActorMngrs[3].getActorP();
        if (unkActor1 != NULL && (prm == 3 || prm == 6 || prm == 7 || prm == 8))
        {
            field_0xd6c = unkActor1->eyePos;
            mJntAnm.lookPos(&field_0xd6c, 0);
        } else {
            mJntAnm.lookNone(0);
        }
    }

    field_0xe26 = false;
    field_0x1060 = 1;
    field_0x1061 = 1;

    return retval;
}

int daNpc_ykW_c::cutFindWolf(int param_0) {
    int retval = 0;
    int* param_p = NULL;
    int prm = -1;
    u32 timer = 0;

    param_p = dComIfGp_evmng_getMyIntegerP(param_0, "prm");
    if (param_p != NULL) {
        prm = *param_p;
    }

    param_p = dComIfGp_evmng_getMyIntegerP(param_0, "timer");
    if (param_p != NULL) {
        timer = *param_p;
    }

    if (dComIfGp_getEventManager().getIsAddvance(param_0)) {
        switch (prm) {
        case 0:
            mEventTimer = timer;
            break;
        case 1:
            break;
        }
    }

    switch (prm) {
    case 0:
        if (mEventTimer != 0 || dComIfGp_getEventManager().getIsAddvance(param_0)) {
            if (cLib_calcTimer(&mEventTimer) == 0) {
                mPlayerAngle = fopAcM_searchPlayerAngleY(this);
            }
        } else {
            mJntAnm.lookPlayer(0);

            if (mPlayerAngle != mCurAngle.y) {
                if (step(mPlayerAngle, 0xd, 0x16, mpHIO->m.mStepSpeed, 0)) {
                    mFaceMotionSeqMngr.setNo(13, -1.0, 0, 0);
                    mMotionSeqMngr.setNo(0, -1.0, 0, 0);
                }
            } else {
                retval = 1;
            }
        }
        break;
    case 1:
        retval = 1;
        break;
    }
    return retval;
}

int daNpc_ykW_c::cutStartSnowboardRace(int param_0) {
    fopAc_ac_c* actor_p = NULL;

    cXyz playerPos;
    csXyz playerAngle;

    int retval = 0;
    int* param_p = NULL;
    int prm = -1;
    u32 timer = 0;

    param_p = dComIfGp_evmng_getMyIntegerP(param_0, "prm");
    if (param_p != NULL) {
        prm = *param_p;
    }

    param_p = dComIfGp_evmng_getMyIntegerP(param_0, "timer");
    if (param_p != NULL) {
        timer = *param_p;
    }

    if (dComIfGp_getEventManager().getIsAddvance(param_0)) {
        switch (prm) {
        case 0:
            mEventTimer = timer;

            break;
        case 1:
            home.pos.set(-72610.0f, -54260.0f, 56590.0f);
            home.angle.setall(0);
            home.angle.y = cM_deg2s(60.0f);
            setPos(home.pos);
            setAngle(home.angle.y);

            break;
        case 2:
            mTimer = fpcM_ERROR_PROCESS_ID_e;
            break;
        case 3:
            mFaceMotionSeqMngr.setNo(13, 0.0f, 0, 0);
            mMotionSeqMngr.setNo(0, 0.0f, 0, 0);

            mJntAnm.lookNone(1);

            field_0x1064 = 0;
            field_0x106e = 0;
            field_0x1065 = 0;
            field_0x106b = 0;

            daNpcT_onTmpBit(0x55);

            actor_p = getOtherYkwP(6);
            JUT_ASSERT(0xe4c, actor_p != NULL);

            setPos(actor_p->current.pos);
            setAngle(actor_p->shape_angle.y);

            if (daNpcT_getPlayerInfoFromPlayerList(20, fopAcM_GetRoomNo(this), &playerPos, &playerAngle) != 0) {
                daPy_getPlayerActorClass()->setPlayerPosAndAngle(&playerPos, playerAngle.y, 0);
                dComIfGp_evmng_setGoal(&playerPos);
            }

            break;
        case 4:
            field_0x106a = 1;

            mDoAud_subBgmStart(0x100004b);

            break;
        case 5:
            break;
        }
    }

    switch (prm) {
    case 0:
        if (cLib_calcTimer(&mEventTimer) == 0) {
            retval = 1;
        }

        break;
    case 1:
        retval = 1;
        break;
    case 2:
        if (mTimer != fpcM_ERROR_PROCESS_ID_e) {
            if ((dTimer_c*)fpcM_SearchByID(mTimer) == dComIfG_getTimerPtr()) {
                retval = 1;
            }
        } else if (dComIfG_getTimerPtr() != NULL) {
            dComIfG_TimerDeleteRequest(dComIfG_getTimerMode());
        } else {
            mTimer = dTimer_createTimer(5, 0x989298, 1, 0, 210.0f, 410.0f, 32.0f, 419.0f);
        }
        break;
    case 3:
        retval = 1;
        break;
    case 4:
        if (mTimer != fpcM_ERROR_PROCESS_ID_e) {
            if ((dTimer_c*)fpcM_SearchByID(mTimer) == dComIfG_getTimerPtr() && dComIfG_TimerStart(5, 0) != 0)
            {
                retval = 1;
            }
        }
        break;
    case 5:
        if (mTimer != fpcM_ERROR_PROCESS_ID_e) {
            if ((dTimer_c*)fpcM_SearchByID(mTimer) == dComIfG_getTimerPtr()) {
                if (dComIfG_getTimerPtr()->isStart()) {
                    retval = 1;
                }
            }
        }
        break;
    }
    return retval;
}

int daNpc_ykW_c::cutEndSnowboardRace(int param_0) {
    fopAc_ac_c* actor_p = NULL;

    cXyz playerPos;
    csXyz playerAngle;

    u32 retval = 0;
    int* param_p = NULL;
    int prm = -1;
    u32 timer = 0;

    param_p = dComIfGp_evmng_getMyIntegerP(param_0, "prm");
    if (param_p != NULL) {
        prm = *param_p;
    }
    param_p = dComIfGp_evmng_getMyIntegerP(param_0, "timer");
    if (param_p != NULL) {
        timer = *param_p;
    }

    if (dComIfGp_getEventManager().getIsAddvance(param_0)) {
        switch (prm) {
        case 0:
            break;
        case 1:
            mFaceMotionSeqMngr.setNo(13, 0.0f, 0, 0);
            mMotionSeqMngr.setNo(0, 0.0f, 0, 0);

            actor_p = getOtherYkwP(5);
            JUT_ASSERT(0xed4, actor_p != NULL)
            home.pos = actor_p->current.pos;
            playerAngle.setall(0);
            playerAngle.y = actor_p->shape_angle.y;
            home.angle.y = playerAngle.y;
            setPos(home.pos);
            setAngle(playerAngle);

            break;
        case 2:
            mEventTimer = timer;

            break;
        case 4:
            mFaceMotionSeqMngr.setNo(13, 0.0f, 0, 0);
            mMotionSeqMngr.setNo(0, 0.0f, 0, 0);
            mJntAnm.lookNone(1);

            daNpcT_offTmpBit(0x55);

            field_0x106a = 0;
            field_0x1044.field_0x0 = 0;
            field_0x104c = 0;
            mPath.setIdx(0);

            actor_p = getOtherYkwP(5);
            JUT_ASSERT(0xef0, actor_p != NULL);
            setPos(actor_p->current.pos);
            playerAngle.setall(0);
            playerAngle.y = actor_p->shape_angle.y;
            home.angle.y = playerAngle.y;
            setAngle(playerAngle);

            if (mTimer != fpcM_ERROR_PROCESS_ID_e) {
                if ((dTimer_c*)fpcM_SearchByID(mTimer) == dComIfG_getTimerPtr()) {
                    dComIfG_TimerDeleteRequest(5);
                }
            }

            if (daNpcT_getPlayerInfoFromPlayerList(12, fopAcM_GetRoomNo(this), &playerPos, &playerAngle) != 0) {
                daPy_getPlayerActorClass()->setPlayerPosAndAngle(&playerPos, playerAngle.y, 0);
                dComIfGp_evmng_setGoal(&playerPos);
            }

            mDoAud_subBgmStop();

            speedF = 0.0f;
            speed.setall(0.0f);

            break;
        case 5:
            daNpcT_onTmpBit(0x56);
            if (field_0x106b != 0) {
                daNpcT_offTmpBit(0x56);
            }
            daNpcT_offTmpBit(11);

            mFlowNodeNo = 0x1ff;
            initTalk(mFlowNodeNo, NULL);

            break;
        }
    }

    switch (prm) {
    case 0:
        retval = 1;

        break;
    case 1:
        mJntAnm.lookPlayer(0);

        retval = 1;

        break;
    case 2:
        action();

        if (cLib_calcTimer(&mEventTimer) == 0) {
            retval = 1;
        }

        break;
    case 3:
        action();

        retval = 1;

        break;
    case 4:
        if (!dComIfGp_getEventManager().getIsAddvance(param_0)) {
            mJntAnm.lookPlayer(0);
        }

        retval = 1;

        break;
    case 5: {
        mJntAnm.lookPlayer(0);
        if (talkProc(NULL, 0, NULL, 0) != 0) {
            int itemId;
            u32 eventId = mFlow.getEventId(&itemId);
            field_0x106c = 0;

            switch (eventId) {
            case 1:
                if (mItemPartnerId == fpcM_ERROR_PROCESS_ID_e) {
                    mItemPartnerId = fopAcM_createItemForPresentDemo(&current.pos, itemId, 0,
                                                            -1, -1, 0, 0);
                }

                if (fopAcM_IsExecuting(mItemPartnerId)) {
                    field_0x106c = 1;
                    mEvtNo = 1;

                    evtChange();
                }

                break;
            case 13:
                mEvtNo = 8;

                evtChange();

                break;
            default:
                if (mFlow.checkEndFlow()) {
                    mPlayerActorMngr.entry(daPy_getPlayerActorClass());

                    retval = 1;
                }

                break;
            }
        }

        break;
    }
    }

    return retval;
}

int daNpc_ykW_c::cutHug(int param_0) {
    fopAc_ac_c* itemActor = NULL;

    cXyz heartPos;
    cXyz heartScale;
    csXyz heartAngle;

    f32 heartSpeed;
    f32 heartSpeedY;

    int retval = 0;
    int* i_param = NULL;
    int prm = -1;
    u32 timer = 0;

    fopAc_ac_c* actors[2] = {this, mActorMngrs[3].getActorP()};
    dComIfGp_setMesgCameraInfoActor(actors[0], actors[1], NULL, NULL, NULL, NULL, NULL, NULL, NULL,
                                    NULL);

    i_param = dComIfGp_evmng_getMyIntegerP(param_0, "prm");
    if (i_param != NULL) {
        prm = *i_param;
    }

    i_param = dComIfGp_evmng_getMyIntegerP(param_0, "timer");
    if (i_param != NULL) {
        timer = *i_param;
    }

    if (dComIfGp_getEventManager().getIsAddvance(param_0)) {
        switch (prm) {
        case 0:
            initTalk(0x201, actors);

            break;
        case 1:
            heartAngle.setall(0);
            heartAngle.y = cLib_targetAngleY(&attention_info.position,
                                            &daPy_getPlayerActorClass()->current.pos);
            heartAngle.y += cM_deg2s((cM_rnd() - 0.5f) * 5.0f);
            heartScale.setall(1.0f);
            heartSpeed = cLib_getRndValue(20.0f, 10.0f);
            heartSpeedY = 20.0f;
            heartPos = attention_info.position;
            heartPos.y -= 80.0f;
            itemActor = fopAcM_fastCreateItem(&heartPos, 0, fopAcM_GetRoomNo(this), &heartAngle,
                                          &heartScale, &heartSpeed, &heartSpeedY, -1, 0, 0);
            if (itemActor != NULL) {
                mDoAud_seStart(Z2SE_OBJ_YO_HEART_S, &heartPos, 0, 0);
                fopAcM_OnStatus(itemActor, fopAcM_STATUS_UNK_0x4000);
            }

            mEventTimer = timer;

            break;
        }
    }

    switch (prm) {
    case 0:
        if (talkProc(NULL, 0, actors, 0) != 0 && mFlow.checkEndFlow()) {
            retval = 1;
        }
        break;
    case 1:
        prm = cLib_calcTimer(&mEventTimer);
        if (prm == 0) {
            retval = 1;
        }
        break;
    }
    field_0xe26 = 0;
    return retval;
}

void daNpc_ykW_c::setDialogueMotion() {
    static u32 const motionTbl1[2] = {31, 6};
    static u32 const motionTbl2[2] = {31, 5};

    int unkInt1 = field_0x1024;
    unkInt1 >>= 1;
    if (0.5f < cM_rnd()) {
        if (cM_rnd() - 0.5f < 0.0f) {
            unkInt1--;
        } else {
            unkInt1++;
        }
    }

    if (field_0x1018 != 0 && cLib_calcTimer(&field_0x1018) == 0) {
        unkInt1 &= 1;
        mMotionSeqMngr.setNo(motionTbl1[unkInt1], -1.0f, 0, 0);
        field_0x1020 = 1;
    }

    if (field_0x101c != 0 && cLib_calcTimer(&field_0x101c) == 0) {
        unkInt1 &= 1;
        mMotionSeqMngr.setNo(motionTbl2[unkInt1], -1.0f, 0, 0);
        field_0x1020 = 1;
    }
}

int daNpc_ykW_c::dialogue() {
    if (field_0x1020 != 0) {
        if (mMotionSeqMngr.getNo() == 31 && mMotionSeqMngr.getStepNo() > 1 ||
            mMotionSeqMngr.getNo() != 31 && mMotionSeqMngr.getStepNo() > 0)
        {
            dComIfGs_offTmpBit(0xe80);
            field_0x1020 = 0;
            return 1;
        }
    } else {
        setDialogueMotion();
    }
    return 0;
}

int daNpc_ykW_c::wait(void* param_0) {
    fopAc_ac_c* actor_p = NULL;

    daPy_py_c* player = (daPy_py_c*)daPy_getPlayerActorClass();

    switch (mMode) {
    case 0:
    case 1:
        if (!mStagger.checkStagger()) {
            if (field_0x1069 != 0) {
                mFaceMotionSeqMngr.setNo(10, 0.0f, 0, 0);
                mMotionSeqMngr.setNo(19, -1.0f, 0, 0);
            } else if (field_0xf80 == 4) {
                mFaceMotionSeqMngr.setNo(12, -1.0f, 0, 0);
                mMotionSeqMngr.setNo(13, -1.0f, 0, 0);
            } else if (daNpcT_chkTmpBit(0x54)) {
                mFaceMotionSeqMngr.setNo(13, -1.0f, 0, 0);
                mMotionSeqMngr.setNo(3, -1.0f, 0, 0);
            } else {
                mFaceMotionSeqMngr.setNo(13, -1.0f, 0, 0);
                mMotionSeqMngr.setNo(0, -1.0f, 0, 0);
            }

            if (field_0x106f != 0) {
                mItemPartnerId = fpcM_ERROR_PROCESS_ID_e;
            }

            field_0x1018 = 0;
            field_0x101c = 0;
            field_0x1020 = 0;
            mMode = 2;
        }

        // fallthrough
    case 2:
        if (field_0x106f != 0) {
            mHide = true;
            fopAcM_OnStatus(this, fopAcM_STATUS_UNK_0x8000000);
            if (putUtuwaHeart(0, 0.0f, 0.0f, 0x5000, 0)) {
                fopAcM_delete(this);
            }
        } else if (field_0x1069 != 0) {
            mJntAnm.lookNone(0);
            attention_info.flags = 0;
        } else {
            if (field_0x106c != 0) {
                mSpeakEvent = true;
                field_0xe33 = true;
            } else if (orderGoIntoBossRoomEvt()) {
                if (field_0x1066 >= 2) {
                    mEvtNo = 4;
                } else {
                    mEvtNo = 5;
                }
                field_0xe33 = true;
            } else {
                switch (field_0xf80) {
                case 2:
                    field_0x1067 = 0;
                    actor_p = NULL;
                    if (daPy_py_c::checkNowWolf()) {
                        break;
                    }

                    if (field_0x1066 == 2) {
                        actor_p = mActorMngrs[1].getActorP();
                        field_0x1034 = 8;
                    } else {
                        if (!daNpcT_chkEvtBit(0x6c)) {
                            actor_p = mActorMngrs[0].getActorP();
                            field_0x1067 = 1;
                        }
                    }

                    if (actor_p != NULL && ((daTag_EvtArea_c*)actor_p)->chkPointInArea(player->current.pos)) {
                        mSpeakEvent = true;
                    }

                    break;
                case 3:
                    break;
                case 4:
                    if (strcmp("D_MN11A", dComIfGp_getStartStageName()) == 0) {
                        actor_p = mActorMngrs[4].getActorP();
                        if (dComIfG_play_c::getLayerNo(0) == 4 || dComIfGs_isStageBossEnemy() && actor_p != NULL &&
                                              ((daB_YO_c*)actor_p)->isEnemyDemoEnd())
                        {
                            mEvtNo = 6;
                            field_0xe33 = true;
                        }
                    }

                    break;
                }
            }

            if (!mStagger.checkStagger()) {
                if (field_0x1034 != 0) {
                    mPlayerActorMngr.entry(daPy_getPlayerActorClass());
                    mJntAnm.lookPlayer(0);
                    field_0x1034 = field_0x1034 + -1;
                } else {
                    if (daNpcT_chkTmpBit(0x54)) {
                        mPlayerActorMngr.remove();
                    }

                    if (mPlayerActorMngr.getActorP() != NULL) {
                        mJntAnm.lookPlayer(0);

                        if (field_0xf80 != 3) {
                            if (!chkActorInSight(mPlayerActorMngr.getActorP(), mAttnFovY,
                                                 mCurAngle.y))
                            {
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
                                if (step(home.angle.y, 13, 22, mpHIO->m.mStepSpeed, 0) != 0)
                                {
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
                }

                if (field_0xf80 == 3) {
                    dComIfGs_offTmpBit(0xe40);
                }

                switch (mJntAnm.getMode()) {
                case 1:
                    if (field_0xf80 == 3) {
                        dComIfGs_onTmpBit(0xe40);
                        if (mMotionSeqMngr.getNo() != 0) {
                            mMotionSeqMngr.setNo(0, -1.0f, 0, 0);
                        }
                    }

                    break;
                case 0:
                    if (field_0xf80 == 3) {
                        if (daNpcT_chkTmpBit(0x54)) {
                            actor_p = mActorMngrs[3].getActorP();
                            if (actor_p != NULL) {
                                mJntAnm.lookActor(actor_p, -40.0f, 0);
                            }
                        } else if (dComIfGs_isTmpBit(0xd02)) {
                            mJntAnm.lookPlayer(0);
                        } else {
                            actor_p = mActorMngrs[3].getActorP();
                            if (actor_p != NULL) {
                                mJntAnm.lookActor(actor_p, -40.0f, 0);
                                if (dialogue()) {
                                    mMode = 1;
                                }
                            }
                        }
                    }

                    break;
                }
            }
        }

        break;
    case 3:
        break;
    }

    return 1;
}

int daNpc_ykW_c::sitWait(void* param_0) {
    fopAc_ac_c* actor_p = NULL;

    daPy_py_c* player = daPy_getPlayerActorClass();

    switch (mMode) {
    case 0:
    case 1:
        if (!mStagger.checkStagger()) {
            mFaceMotionSeqMngr.setNo(13, -1.0f, 0, 0);
            mMotionSeqMngr.setNo(1, -1.0f, 0, 0);
            mMode = 2;
        }
    case 2:
        if (field_0x1062 != 0) {
            mEvtNo = 3;
            field_0xe33 = true;
        } else {
            actor_p = mActorMngrs->getActorP();
            if (actor_p != NULL && ((daTag_EvtArea_c*)actor_p)->chkPointInArea(player->current.pos))
            {
                mSpeakEvent = true;
            }
        }

        if (!mStagger.checkStagger()) {
            if (mPlayerActorMngr.getActorP() != NULL) {
                mJntAnm.lookPlayer(0);
                if (!daNpcT_chkEvtBit(0x5b) || daNpcT_chkEvtBit(0x5c) != 0) {
                    if (!chkActorInSight(mPlayerActorMngr.getActorP(), mAttnFovY, mCurAngle.y)) {
                        mJntAnm.lookNone(0);
                    }
                    srchPlayerActor();
                }
            } else {
                mJntAnm.lookNone(0);
                srchPlayerActor();
            }

            switch (mJntAnm.getMode()) {
            case 0:
                break;
            }
        }

        break;
    case 3:
        break;
    }

    return 1;
}

int daNpc_ykW_c::walk(void* param_0) {
    f32 speed;
    int dstPos;
    int dstPos2;

    s16 downTime = mpHIO->m.mWalkingTime;
    s16 walkingTime = mpHIO->m.mDownTime;

    switch (mMode) {
    case 0:
    case 1:
        if (!mStagger.checkStagger()) {
            mFaceMotionSeqMngr.setNo(13, -1.0f, 0, 0);
            mMotionSeqMngr.setNo(0, -1.0f, 0, 0);

            field_0x102c = 0;
            field_0x1028 = 0;
            mMode = 2;
        }

        // fallthrough
    case 2:
        if (orderGoIntoBossRoomEvt()) {
            if (field_0x1066 >= 2) {
                mEvtNo = 4;
            } else {
                mEvtNo = 5;
            }

            field_0xe33 = true;
        }

        if (!mStagger.checkStagger()) {
            cXyz unkXyz1;

            dstPos = 0;
            JUT_ASSERT(0x11b7, mPath.getPathInfo() != NULL);

            if (field_0xf80 == 2 && field_0x1066 == 1) {
                dstPos = mPath.getDstPosH(current.pos, &unkXyz1, mPath.getNumPnts() - 1, 3);
            } else {
                dstPos = mPath.getDstPos(current.pos, &unkXyz1);
            }

            dstPos2 = dstPos;
            if (dComIfGp_event_runCheck() != 0 && mStaffId == -1) {
                dstPos2 = 1;
            }

            speed = speedF;

            calcSpeedAndAngle(unkXyz1, dstPos2, 6, 0x800);

            if (cM3d_IsZero(speedF)) {
                if (cM3d_IsZero(speed) == FALSE) {
                    mFaceMotionSeqMngr.setNo(13, -1.0f, 0, 0);
                    mMotionSeqMngr.setNo(0, -1.0f, 0, 0);
                }
            } else {
                if (mMotionSeqMngr.getNo() == 0) {
                    mMotionSeqMngr.setNo(20, -1.0f, 0, 0);
                }
            }

            if (dstPos != 0 && cM3d_IsZero(speedF)) {
                field_0x1063 = 0;
                field_0x1064 = 1;
                if (field_0x1066 == 1) {
                    if (getBitSW3() != 0xff) {
                        if (!dComIfGs_isSwitch(getBitSW3(), fopAcM_GetRoomNo(this))) {
                            dComIfGs_onSwitch(getBitSW3(), fopAcM_GetRoomNo(this));
                        }
                    }
                    field_0x1066 = 2;
                }
            }

            if (mPlayerActorMngr.getActorP() != NULL) {
                mJntAnm.lookPlayer(0);
                if (!chkActorInSight(mPlayerActorMngr.getActorP(), mAttnFovY, mCurAngle.y)) {
                    mJntAnm.lookNone(0);
                }
            } else {
                mJntAnm.lookNone(0);
            }
            srchPlayerActor();
        }

        break;
    case 3:
        break;
    }

    return 1;
}

int daNpc_ykW_c::race(void* param_0) {
    int unkInt1;
    s16 targetAngleY;
    f32 unkFloat1;
    u16 idx;

    cXyz unkXyz1;

    fopAc_ac_c* actors[2] = {this, daPy_getPlayerActorClass()};

    switch (mMode) {
    case 0:
    case 1:
        if (!mStagger.checkStagger()) {
            mFaceMotionSeqMngr.setNo(13, -1.0f, 0, 0);
            mMotionSeqMngr.setNo(29, -1.0f, 0, 0);
            mMode = 2;
        }
    // fallthrough
    case 2:
        if (daNpcT_chkTmpBit(0x55)) {
            daNpcT_judgeRace(mpPath, actors, &field_0x1044, 2, &field_0x103c);
            field_0xff0.x = mpPath->m_points[mpPath->m_num - 1].m_position.x;
            field_0xff0.y = mpPath->m_points[mpPath->m_num - 1].m_position.y;
            field_0xff0.z = mpPath->m_points[mpPath->m_num - 1].m_position.z;
            targetAngleY = cLib_targetAngleY(&field_0xff0, &daPy_getPlayerActorClass()->current.pos);
            if (field_0x104c < 0 ||
#if PLATFORM_SHIELD
                field_0x1044.field_0x0 >= 0 &&
#endif
                    field_0xff0.abs2(daPy_getPlayerActorClass()->current.pos) < 640000.0f &&
                    (targetAngleY < -0x4000 || targetAngleY > 0x4000))
            {
                field_0x104c = -1;
                mGndChk.SetPos(&daPy_getPlayerActorClass()->current.pos);
                if (dComIfG_Bgsp().GroundCross(&mGndChk) != -G_CM3D_F_INF) {
                    if (mTimer != fpcM_ERROR_PROCESS_ID_e) {
                        if ((dTimer_c*)fpcM_SearchByID(mTimer) == dComIfG_getTimerPtr()) {
                            if (dComIfG_getTimerPtr()->isStart()) {
                                if (field_0x106b != 0) {
                                    mDoAud_subBgmStart(0x1000099);
                                    dMeter2Info_setMeterString(0x539);
                                } else {
                                    mDoAud_subBgmStart(0x1000098);
                                    dMeter2Info_setMeterString(0x538);
                                }
                                dComIfG_TimerEnd(5, -1);
                                if ((dComIfG_getTimerNowTimeMs() < dComIfGs_getRaceGameTime()) ||
                                    dComIfGs_getRaceGameTime() == 0)
                                {
                                    dComIfGs_setRaceGameTime(dComIfG_getTimerNowTimeMs());
                                }
                            }
                        }
                    }
                    mEvtNo = 9;
                }
            }
#if PLATFORM_SHIELD
            else {
#else
            if (field_0x104c >= 0) {
#endif
                if (field_0x1044.field_0x0 < 0) {
                    field_0x106b = 1;
                }
            }
        }
        mJntAnm.lookNone(0);
        if (field_0x1064 != 0) {
            if (field_0x1065 != 0) {
                cLib_addCalc2(&speedF, 0.0f, 0.15f, 6.0f);
                if (speedF <= 0.15f) {
                    field_0x1065 = 0;
                    speedF = 0.0f;
                    speed.setall(0.0f);
                }
            } else {
                mJntAnm.lookNone(0);
            }
        } else {
            if (field_0x106e != 0 && mAcch.ChkGroundHit()) {
                mMotionSeqMngr.setNo(26, -1.0f, 0, 0);
                mSound.startCreatureSound(Z2SE_YW_SNOBO_LAND, 0, -1);
                field_0x106e = 0;
            }

            if (field_0x106e == 0) {
                while (true) {
                    if (mPath.chkPassed1(current.pos, mPath.getNumPnts())) {
                        if (u8(mPath.getArg0()) == 0) {
                            mSound.startCreatureSound(Z2SE_YW_SNOBO_JUMP, 0, -1);
                            mSound.startCreatureVoice(Z2SE_YW_V_SNOBO_JUMP, -1);
                            field_0x106e = 1;
                        }
                        if (mPath.setNextIdx(mPath.getNumPnts()) != 0) {
                            field_0x1064 = 1;
                            field_0x1065 = 1;
                            mMotionSeqMngr.setNo(32, -1.0f, 0, 0);
                        } else {
                            if (field_0x106e == 0) {
                                continue;
                            }

                            if (!mStagger.checkStagger()) {
                                mMotionSeqMngr.setNo(25, -1.0f, 0, 0);
                            }

                            field_0xffc = current.pos;
                            idx = mPath.getIdx();
                            field_0x1008 = mPath.getPntPos(idx);
                            current.angle.y = cLib_targetAngleY(&field_0xffc, &field_0x1008);
                            unkInt1 = (int)((field_0x1008 - field_0xffc).absXZ() / speedF) + 1;
                            speed.y = field_0x1008.y - field_0xffc.y +
                                      (f32)unkInt1 * fabsf(gravity) * (f32)unkInt1 * 0.5f;
                            speed.y /= (f32)unkInt1;
                        }
                    } else {
                        idx = mPath.getIdx();
                        unkXyz1 = mPath.getPntPos(idx);
                    }
                    break;
                }
            }

            if (field_0x1064 == 0 && field_0x106e == 0) {
                targetAngleY = cLib_targetAngleY(&current.pos, &unkXyz1);
                current.angle.y = targetAngleY;
                cLib_addCalcAngleS2(&shape_angle.y, current.angle.y, 4, 0x200);
                mCurAngle.y = shape_angle.y;
                if (!mStagger.checkStagger() && mMotionSeqMngr.getNo() != 29) {
                    targetAngleY -= mCurAngle.y;
                    if (0.0f < cM_sht2d(targetAngleY)) {
                        if (4.0f < cM_sht2d(targetAngleY)) {
                            mMotionSeqMngr.setNo(27, 12.0f, 0, 0);
                        } else {
                            mMotionSeqMngr.setNo(24, 12.0f, 0, 0);
                        }
                    } else {
                        if (cM_sht2d(targetAngleY) < -4.0f) {
                            mMotionSeqMngr.setNo(28, 12.0f, 0, 0);
                        } else {
                            mMotionSeqMngr.setNo(24, 12.0f, 0, 0);
                        }
                    }
                }
                targetAngleY = -fopAcM_getPolygonAngle(mGndChk, (s16)(current.angle.y + 0x4000));
                cLib_addCalcAngleS2(&mCurAngle.x, mGroundAngle, 6, 0x200);
                cLib_addCalcAngleS2(&mCurAngle.z, targetAngleY, 6, 0x200);
                int unkInt2 = field_0x104c - field_0x1044.field_0x0;
                unkFloat1 = mpHIO->m.mSlidingSpeed * cM_scos(mGroundAngle);
                if (mStagger.checkStagger()) {
                    unkFloat1 *= 0.0f;
                } else {
                    unkInt2 += mpHIO->m.mCompetParamA;
                    unkInt2 = cLib_minMaxLimit<int>(unkInt2, 0, mpHIO->m.mCompetParamB);
                    if (unkInt2 > 0) {
                        unkFloat1 *= (f32)abs(unkInt2) * 0.1f * mpHIO->m.mCompetParamC + 1.0f;
                    }
                }
                cLib_chaseF(&speedF, unkFloat1, mpHIO->m.mSlidingAccel);
                mSound.startCreatureSoundLevel(Z2SE_YW_SNOBO_RIDE, speedF, -1);
                mSound.startCreatureVoiceLevel(Z2SE_YW_V_SNOBO_RIDING, -1);
            }
        }
        attention_info.flags = 0;
        break;
    case 3:
        break;
    }
    dTres_c::setNpcYkwPosition(getBitTRB(), &current.pos);
    return 1;
}

int daNpc_ykW_c::talk(void* param_0) {
    int itemNo;

    u32 eventId;
    switch (mMode) {
    case 0:
    case 1:
        if (!mStagger.checkStagger()) {
            dMeter2Info_setMsgTimeMs(dComIfGs_getRaceGameTime());

            if (mFlowNodeNo == 0x1f5 && daNpcT_chkEvtBit(0xa3) == 0) {
                if (!daNpcT_chkEvtBit(0xa2)) {
                    if (!daNpcT_chkEvtBit(0x5c)) {
                        daNpcT_offTmpBit(13);
                    }
                    daNpcT_offTmpBit(11);
                }
                daNpcT_offTmpBit(12);
            }

            initTalk(mFlowNodeNo, NULL);

            mMode = 2;
        }

        // fallthrough
    case 2:
        if (!mStagger.checkStagger()) {
            if (mTwilight || mPlayerAngle == mCurAngle.y || &daNpc_ykW_c::sitWait == field_0xfd8) {
                if (talkProc(NULL, 0, NULL, 0)) {
                    eventId = mFlow.getEventId(&itemNo);
                    field_0x106c = 0;

                    switch (eventId) {
                    case 1:
                        if (mItemPartnerId == fpcM_ERROR_PROCESS_ID_e) {
                            mItemPartnerId =
                                fopAcM_createItemForPresentDemo(&current.pos, itemNo, 0, -1, -1, 0, 0);
                        }

                        if (fopAcM_IsExecuting(mItemPartnerId)) {
                            mEvtNo = 1;
                            field_0x1062 = 1;
                            evtChange();
                        }

                        break;
                    case 13:
                        mEvtNo = 8;

                        evtChange();

                        break;
                    case 9:
                        field_0x1062 = 1;
                    default:
                        if (mFlow.checkEndFlow()) {
                            mPlayerActorMngr.entry(daPy_getPlayerActorClass());

                            dComIfGp_event_reset();

                            if (field_0xf80 == 2 && !daPy_py_c::checkNowWolf()) {
                                if (field_0x1067 != 0) {
                                    field_0x1063 = 1;
                                    field_0x1066 = 1;
                                    field_0x1067 = 0;
                                } else {
                                    if (field_0x1066 == 2) {
                                        field_0x1066 = 3;
                                    }
                                }
                            }

                            mMode = 3;
                        }
                    }
                }

                mJntAnm.lookPlayer(0);

                if (mTwilight) {
                    mJntAnm.lookNone(0);
                }
            } else {
                mJntAnm.lookPlayer(0);
                step(mPlayerAngle, 13, 22, mpHIO->m.mStepSpeed, 0);
                break;
            }
        }
        break;
    case 3:
        break;
    }

    return 0;
}

int daNpc_ykW_c::test(void* param_0) {
    int retval = 0;

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
        break;
    }

    return retval;
}


static int daNpc_ykW_Create(void* i_this) {
    return ((daNpc_ykW_c*)i_this)->create();
}

static int daNpc_ykW_Delete(void* i_this) {
    return ((daNpc_ykW_c*)i_this)->Delete();
}

static int daNpc_ykW_Execute(void* i_this) {
    return ((daNpc_ykW_c*)i_this)->Execute();
}

static int daNpc_ykW_Draw(void* i_this) {
    return ((daNpc_ykW_c*)i_this)->Draw();
}

static int daNpc_ykW_IsDelete(void* i_this) {
    return 1;
}

AUDIO_INSTANCES

static actor_method_class daNpc_ykW_MethodTable = {
    (process_method_func)daNpc_ykW_Create,
    (process_method_func)daNpc_ykW_Delete,
    (process_method_func)daNpc_ykW_Execute,
    (process_method_func)daNpc_ykW_IsDelete,
    (process_method_func)daNpc_ykW_Draw,
};

extern actor_process_profile_definition g_profile_NPC_YKW = {
    fpcLy_CURRENT_e,          // mLayerID
    7,                        // mListID
    fpcPi_CURRENT_e,          // mListPrio
    PROC_NPC_YKW,             // mProcName
    &g_fpcLf_Method.base,    // sub_method
    sizeof(daNpc_ykW_c),      // mSize
    0,                        // mSizeOther
    0,                        // mParameters
    &g_fopAc_Method.base,     // sub_method
    381,                      // mPriority
    &daNpc_ykW_MethodTable,   // sub_method
    0x00040108,               // mStatus
    fopAc_NPC_e,              // mActorType
    fopAc_CULLBOX_CUSTOM_e,   // cullType
  };
