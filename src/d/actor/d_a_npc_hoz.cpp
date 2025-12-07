/**
 * @file d_a_npc_hoz.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_npc_hoz.h"
#include "d/actor/d_a_arrow.h"
#include "d/actor/d_a_canoe.h"
#include "d/d_meter2_info.h"

static int l_bmdData[2][2] = {
    {13, 1},
    {3, 2},
};

static daNpcT_evtData_c l_evtList[9] = {
    {"", 0},
    {"BOAT_RACE", 1},
    {"BOAT_RACE_RETURN", 1},
    {"BEFORE_BATTLE", 1},
    {"AFTER_BATTLE", 1},
    {"BEFORE_BLAST", 1},
    {"AFTER_BLAST", 1},
    {"Y_MUSHI", 1},
    {"TALK_BREAK", 1},
};

static char* l_resNameList[7] = {
    "",
    "Hoz",
    "Hoz_TW",
    "Hoz1",
    "Hoz2",
    "Hoz3",
    "Hoz2_3",
};

static s8 l_loadResPtrn0[] = {
    1,
    2,
    -1,
};

static s8 l_loadResPtrnTW[] = {1, 2, 3, -1};

static s8 l_loadResPtrn1[] = {1, 4, 6, -1};

static s8 l_loadResPtrnBattle[] = {
    1,
    4,
    5,
    6,
    -1,
};

static s8* l_loadResPtrnList[7] = {
    l_loadResPtrn0,      l_loadResPtrn1,  l_loadResPtrn1,
    l_loadResPtrnBattle, l_loadResPtrnTW, l_loadResPtrn1,
    l_loadResPtrnTW,
};

static daNpcT_faceMotionAnmData_c l_faceMotionAnmData[39] = {
    {-1, 0, 0, 19, 2, 1, 1},
    {8, 0, 1, 19, 2, 1, 1},
    {7, 2, 1, 19, 2, 1, 1},
    {5, 2, 3, 45, 2, 3, 0},
    {6, 0, 3, 46, 0, 3, 0},
    {7, 0, 3, 47, 0, 3, 0},
    {8, 0, 3, 48, 0, 3, 0},
    {9, 2, 3, 49, 2, 3, 0},
    {12, 0, 3, 52, 0, 3, 0},
    {13, 0, 3, 53, 0, 3, 0},
    {14, 0, 3, 19, 2, 1, 1},
    {15, 0, 3, 54, 0, 3, 0},
    {26, 2, 3, 19, 2, 1, 1},
    {16, 2, 3, 19, 2, 1, 1},
    {17, 0, 3, 19, 2, 1, 1},
    {27, 2, 3, 19, 2, 1, 1},
    {18, 0, 3, 19, 2, 1, 1},
    {10, 0, 3, 50, 0, 3, 0},
    {24, 2, 3, 55, 2, 3, 0},
    {11, 0, 3, 51, 0, 3, 0},
    {4, 0, 6, 10, 0, 6, 0},
    {5, 2, 6, 11, 2, 6, 0},
    {3, 0, 5, 19, 0, 1, 1},
    {4, 0, 5, 19, 0, 1, 1},
    {5, 2, 5, 19, 0, 1, 1},
    {19, 0, 4, 19, 0, 1, 1},
    {16, 0, 4, 43, 0, 4, 0},
    {10, 0, 4, 38, 0, 4, 0},
    {11, 0, 4, 39, 0, 4, 0},
    {12, 0, 4, 40, 0, 4, 0},
    {13, 0, 4, 19, 2, 1, 1},
    {20, 2, 4, 19, 2, 1, 1},
    {18, 0, 4, 44, 0, 4, 0},
    {23, 2, 4, 19, 2, 1, 1},
    {14, 0, 4, 41, 0, 4, 1},
    {21, 2, 4, 45, 2, 4, 1},
    {15, 0, 4, 42, 0, 4, 1},
    {17, 0, 4, 19, 2, 1, 1},
    {22, 2, 4, 19, 2, 1, 1},
};

static daNpcT_motionAnmData_c l_motionAnmData[39] = {
    {10, 2, 1, 16, 0, 1, 1, 0},
    {9, 0, 1, 16, 0, 1, 1, 0},
    {19, 2, 3, 16, 0, 1, 1, 0},
    {20, 0, 3, 40, 0, 3, 0, 0},
    {21, 0, 3, 16, 0, 1, 1, 0},
    {22, 0, 3, 41, 0, 3, 0, 0},
    {23, 2, 3, 42, 2, 3, 0, 0},
    {30, 0, 3, 16, 0, 1, 1, 0},
    {31, 0, 3, 16, 0, 1, 1, 0},
    {32, 2, 3, 16, 0, 1, 1, 0},
    {33, 0, 3, 16, 0, 1, 1, 0},
    {34, 2, 3, 16, 0, 1, 1, 0},
    {35, 0, 3, 16, 0, 1, 1, 0},
    {37, 2, 3, 16, 0, 1, 1, 0},
    {36, 0, 3, 16, 0, 1, 1, 0},
    {28, 0, 3, 16, 0, 1, 1, 0},
    {29, 2, 3, 16, 0, 1, 1, 0},
    {6, 0, 6, 16, 0, 1, 1, 0},
    {7, 2, 6, 16, 0, 1, 1, 0},
    {6, 0, 5, 16, 0, 1, 1, 0},
    {7, 2, 5, 16, 0, 1, 1, 0},
    {29, 0, 4, 16, 0, 1, 1, 0},
    {8, 0, 4, 16, 0, 1, 1, 0},
    {9, 0, 4, 16, 0, 1, 1, 0},
    {24, 0, 4, 16, 0, 1, 1, 0},
    {25, 2, 4, 16, 0, 1, 1, 0},
    {26, 0, 4, 16, 0, 1, 1, 0},
    {30, 0, 4, 16, 0, 1, 1, 0},
    {32, 2, 4, 16, 0, 1, 1, 0},
    {34, 0, 4, 16, 0, 1, 1, 0},
    {31, 0, 4, 16, 0, 1, 1, 0},
    {33, 2, 4, 16, 0, 1, 1, 0},
    {35, 0, 4, 16, 0, 1, 1, 0},
    {27, 0, 4, 16, 0, 1, 1, 0},
    {28, 2, 4, 16, 0, 1, 1, 0},
    {4, 0, 4, 16, 0, 1, 1, 0},
    {5, 2, 4, 16, 0, 1, 1, 0},
    {6, 0, 4, 16, 0, 1, 1, 0},
    {7, 2, 4, 16, 0, 1, 1, 0},
};

static daNpcT_MotionSeqMngr_c::sequenceStepData_c l_faceMotionSequenceData[140] = {
    {1, -1, 1}, {0, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {3, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {4, -1, 1}, {3, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {6, -1, 1}, {7, -1, 0}, {-1, 0, 0}, {-1, 0, 0},
    {7, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {8, -1, 1}, {10, -1, 0}, {-1, 0, 0}, {-1, 0, 0},
    {9, -1, 1}, {10, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {11, -1, 1}, {12, -1, 0}, {-1, 0, 0}, {-1, 0, 0},
    {13, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {14, -1, 1}, {15, -1, 0}, {-1, 0, 0}, {-1, 0, 0},
    {26, -1, 1}, {2, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {27, -1, 1}, {2, -1, 0}, {-1, 0, 0}, {-1, 0, 0},
    {28, -1, 1}, {2, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {29, -1, 1}, {31, -1, 0}, {-1, 0, 0}, {-1, 0, 0},
    {32, -1, 1}, {33, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {34, -1, 1}, {35, -1, 0}, {-1, 0, 0}, {-1, 0, 0},
    {36, -1, 1}, {35, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {20, -1, 1}, {21, -1, 0}, {-1, 0, 0}, {-1, 0, 0},
    {22, -1, 1}, {23, -1, 1}, {24, -1, 0}, {-1, 0, 0}, {23, -1, 1}, {24, -1, 0}, {-1, 0, 0}, {-1, 0, 0},
    {21, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {37, -1, 1}, {38, -1, 0}, {-1, 0, 0}, {-1, 0, 0},
    {30, -1, 1}, {31, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {25, -1, 1}, {2, -1, 0}, {-1, 0, 0}, {-1, 0, 0},
    {17, -1, 1}, {18, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {19, -1, 1}, {18, -1, 0}, {-1, 0, 0}, {-1, 0, 0},
    {18, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {16, -1, 1}, {10, -1, 0}, {-1, 0, 0}, {-1, 0, 0},
    {24, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {35, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {31, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {33, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {12, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {10, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {0, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
};

static daNpcT_MotionSeqMngr_c::sequenceStepData_c l_motionSequenceData[144] = {
    {0, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {1, -1, 1}, {0, -1, 0}, {-1, 0, 0}, {-1, 0, 0},
    {2, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {3, -1, 1}, {2, -1, 0}, {-1, 0, 0}, {-1, 0, 0},
    {5, -1, 1}, {6, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {6, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {7, -1, 1}, {9, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {8, -1, 1}, {9, -1, 0}, {-1, 0, 0}, {-1, 0, 0},
    {10, -1, 1}, {11, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {11, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {12, -1, 1}, {13, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {21, -1, 1}, {0, -1, 0}, {-1, 0, 0}, {-1, 0, 0},
    {22, -1, 1}, {0, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {23, -1, 1}, {0, -1, 0}, {-1, 0, 0}, {-1, 0, 0},
    {24, -1, 1}, {25, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {27, -1, 1}, {28, -1, 0}, {-1, 0, 0}, {-1, 0, 0},
    {25, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {33, -1, 1}, {34, -1, 0}, {-1, 0, 0}, {-1, 0, 0},
    {34, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {17, -1, 1}, {18, -1, 0}, {-1, 0, 0}, {-1, 0, 0},
    {19, -1, 1}, {20, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {20, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {18, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {35, -1, 1}, {36, -1, 0}, {-1, 0, 0}, {-1, 0, 0},
    {36, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {37, -1, 1}, {38, -1, 0}, {-1, 0, 0}, {-1, 0, 0},
    {38, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {35, -1, 1}, {37, -1, 1}, {38, -1, 0}, {-1, 0, 0},
    {30, -1, 1}, {31, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {15, -1, 1}, {16, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {16, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {32, -1, 1}, {0, -1, 0}, {-1, 0, 0}, {-1, 0, 0},
    {14, -1, 1}, {9, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {26, -1, 1}, {0, -1, 0}, {-1, 0, 0}, {-1, 0, 0},
    {29, -1, 1}, {0, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {9, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
};

char* daNpc_Hoz_c::mCutNameList[8] = {
    "",
    "BOAT_RACE",
    "BEFORE_BATTLE",
    "AFTER_BATTLE",
    "BEFORE_BLAST",
    "AFTER_BLAST",
    "Y_MUSHI",
    "TALK_BREAK",
};

daNpc_Hoz_c::cutFunc daNpc_Hoz_c::mCutList[] = {
    NULL,
    &daNpc_Hoz_c::ECut_boatRace,
    &daNpc_Hoz_c::ECut_beforeBattle,
    &daNpc_Hoz_c::ECut_afterBattle,
    &daNpc_Hoz_c::ECut_beforeBlast,
    &daNpc_Hoz_c::ECut_afterBlast,
    &daNpc_Hoz_c::ECut_yMushi,
    &daNpc_Hoz_c::ECut_talkBreak,
};

const daNpc_Hoz_HIOParam daNpc_Hoz_Param_c::m = {
    60.0f,
    -3.0f,
    1.0f,
    500.0f,
    255.0f,
    210.0f,
    35.0f,
    30.0f,
    0.0f,
    0.0f,
    30.0f,
    -30.0f,
    30.0f,
    -10.0f,
    45.0f,
    -45.0f,
    0.6f,
    12.0f,
    4,
    6,
    4,
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
    -20.0f,
    0.0f,
    -20.0f,
    20.0f,
    40.0f,
    20.0f,
    110.0f,
    600.0f,
};

daNpc_Hoz_c::~daNpc_Hoz_c() {
    if (mType == 1) {
        daNpcT_offTmpBit(0x46);
    }

    if (heap != NULL) {
        mpMorf[0]->stopZelAnime();
    }

    deleteRes(l_loadResPtrnList[mType], (const char**)l_resNameList);
}

int daNpc_Hoz_c::create() {
    daNpcT_ct(this, daNpc_Hoz_c,
              l_faceMotionAnmData,
              l_motionAnmData,
              l_faceMotionSequenceData, 4,
              l_motionSequenceData, 4,
              l_evtList,
              l_resNameList);

    mType = getType();
    mFlowNodeNo = getFlowNodeNo();
    mTwilight = dKy_darkworld_check();
    field_0xf88 = (fopAcM_GetParam(this) & 0xFF0000) >> 0x10;
    field_0xf89 = (fopAcM_GetParam(this) & 0xFF000000) >> 0x18;

    int temp_r3 = loadRes(l_loadResPtrnList[mType], (const char**)l_resNameList);
    if (temp_r3 == 4) {
        static int const heapSize[7] = {
            0x0, 0x3F70, 0x3F70, 0x3F80,
            0x3EC0, 0x3F70, 0x3EB0,
        };

        if (!fopAcM_entrySolidHeap(this, createHeapCallBack, heapSize[mType])) {
            return cPhs_ERROR_e;
        }

        //OS_REPORT("\t(%s:%d) flowNo:%d, PathID:%02x<%08x> ", fopAcM_getProcNameString(this), mType, mFlowNodeNo, getPathID() & 0xFF, fopAcM_GetParam(this));

        if (isDelete()) {
            OS_REPORT("===>isDelete:TRUE\n");
            return cPhs_ERROR_e;
        }

        OS_REPORT("\n");

        J3DModelData* sp10 = mpMorf[0]->getModel()->getModelData();
        fopAcM_SetMtx(this, mpMorf[0]->getModel()->getBaseTRMtx());
        fopAcM_setCullSizeBox(this, -300.0f, -50.0f, -300.0f, 300.0f, 450.0f,300.0f);

        mSound.init(&current.pos, &eyePos, 3, 1);

        mAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1, &mAcchCir, fopAcM_GetSpeed_p(this), fopAcM_GetAngle_p(this), fopAcM_GetShapeAngle_p(this));
        mAcch.CrrPos(dComIfG_Bgsp());
        mGndChk = mAcch.m_gnd;
        mGroundH = mAcch.GetGroundH();

        setEnvTevColor();
        setRoomNo();

        mCcStts.Init(daNpc_Hoz_Param_c::m.common.weight, 0, this);
        mCyl.Set(mCcDCyl);
        mCyl.SetStts(&mCcStts);
        mCyl.SetTgHitCallback(tgHitCallBack);
        reset();

        mCreating = true;
        Execute();
        mCreating = false;
    }

    return temp_r3;
}

static f32 dummyFloat(int _) {
    switch (_) {
    case 0: return 0.0f;
    case 1: return 65536.0f;
    case 2: return 0.2f;
    }
}

int daNpc_Hoz_c::CreateHeap() {
    J3DModelData* mdlData_p;
    int bmdIdx;
    int resIdx;

    int idx = 0;
    idx = mTwilight == true ? TRUE : FALSE;
    resIdx = l_bmdData[idx][1];
    bmdIdx = l_bmdData[idx][0];

    mdlData_p = (J3DModelData*)dComIfG_getObjectRes(l_resNameList[resIdx], bmdIdx);
    JUT_ASSERT(1543, NULL != mdlData_p);

    u32 sp1C = 0x11020284;
    mpMorf[0] = new mDoExt_McaMorfSO(mdlData_p, NULL, NULL, NULL, -1, 1.0f, 0, -1, &mSound, J3DMdlFlag_DifferedDLBuffer, sp1C);
    if (mpMorf[0] != NULL && mpMorf[0]->getModel() == NULL) {
        mpMorf[0]->stopZelAnime();
        mpMorf[0] = NULL;
    }

    if (mpMorf[0] == NULL) {
        return 0;
    }

    J3DModel* model = mpMorf[0]->getModel();
    for (u16 i = 0; i < mdlData_p->getJointNum(); i++) {
        mdlData_p->getJointNodePointer(i)->setCallBack(ctrlJointCallBack);
    }
    model->setUserArea((uintptr_t)this);

    mpMatAnm[0] = new daNpcT_MatAnm_c();
    if (mpMatAnm[0] == NULL) {
        return 0;
    }

    if (setFaceMotionAnm(1, false) && setMotionAnm(0, 0.0f, FALSE)) {
        return 1;
    }

    return 0;
}

int daNpc_Hoz_c::Delete() {
    OS_REPORT("|%06d:%x|daNpc_Hoz_c -> Delete\n", g_Counter.mCounter0, this);
    fpc_ProcID id = fopAcM_GetID(this);
    this->~daNpc_Hoz_c();
    return 1;
}

int daNpc_Hoz_c::Execute() {
    return execute();
}

int daNpc_Hoz_c::Draw() {
    if (mpMatAnm[0] != NULL) {
        J3DModelData* modelData = mpMorf[0]->getModel()->getModelData();
        modelData->getMaterialNodePointer(getEyeballMaterialNo())->setMaterialAnm(mpMatAnm[0]);
    }

    return draw(
#if DEBUG
        chkAction(&daNpc_Hoz_c::test),
#else
        FALSE,
#endif
        FALSE, daNpc_Hoz_Param_c::m.common.real_shadow_size, NULL, 100.0f, FALSE, FALSE, FALSE
    );
}

int daNpc_Hoz_c::createHeapCallBack(fopAc_ac_c* actor) {
    daNpc_Hoz_c* i_this = (daNpc_Hoz_c*)actor;
    return i_this->CreateHeap();
}

int daNpc_Hoz_c::ctrlJointCallBack(J3DJoint* i_joint, int param_1) {
    if (param_1 == 0) {
        J3DModel* model = j3dSys.getModel();
        daNpc_Hoz_c* i_this = (daNpc_Hoz_c*)model->getUserArea();
        if (i_this != NULL) {
            i_this->ctrlJoint(i_joint, model);
        }
    }

    return 1;
}

static void* s_sub(void* i_actor, void* i_data) {
    if (fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_START_AND_GOAL) {
        return i_actor;
    }

    return NULL;
}

static void* s_subCanoe(void* i_actor, void* i_data) {
    if (fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_CANOE) {
        return i_actor;
    }

    return NULL;
}

u8 daNpc_Hoz_c::getType() {
    switch (fopAcM_GetParam(this) & 0xFF) {
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
        return 0;
    }
}

int daNpc_Hoz_c::getFlowNodeNo() {
    u16 nodeNo = home.angle.x;

    int rt;
    if (nodeNo == 0xFFFF) {
        rt = -1;
    } else {
        rt = nodeNo;
    }

    return rt;
}

int daNpc_Hoz_c::isDelete() {
    return 0;
}

void daNpc_Hoz_c::reset() {
    initialize();
    memset(&field_0xfac, 0, (u32)&field_0xfc4 - (u32)&field_0xfac);

    if (mpMatAnm[0] != NULL) {
        mpMatAnm[0]->initialize();
    }

    setAngle(home.angle.y);

    field_0xf8a = 0;
    field_0xf8b = 0;
    field_0xf8c = 0;
    mGameStartFlag = 0;
    mPotBreakFlag = 0;

    field_0xfa0.initialize();
    field_0xfa8 = 0;

    if (mType == 4 || mType == 6) {
        mAcch.SetGrndNone();
        mAcch.SetWallNone();
        current.pos = home.pos;
    }
}

static void* s_subBomb(void* i_actor, void* i_data) {
    if (fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_ARROW && ((daArrow_c*)i_actor)->checkBombArrow()) {
        return i_actor;
    }

    return NULL;
}

void daNpc_Hoz_c::setParam() {
    u32 attn_flags = fopAc_AttnFlag_SPEAK_e | fopAc_AttnFlag_TALK_e;

    selectAction();
    srchActors();

    s16 talk_dist = daNpc_Hoz_Param_c::m.common.talk_distance;
    s16 talk_angle = daNpc_Hoz_Param_c::m.common.talk_angle;
    s16 attn_dist = daNpc_Hoz_Param_c::m.common.attention_distance;
    s16 attn_angle = daNpc_Hoz_Param_c::m.common.attention_angle;

    attention_info.distances[fopAc_attn_LOCK_e] = daNpcT_getDistTableIdx(attn_dist, attn_angle);
    attention_info.distances[fopAc_attn_TALK_e] = attention_info.distances[fopAc_attn_LOCK_e];
    attention_info.distances[fopAc_attn_SPEAK_e] = daNpcT_getDistTableIdx(talk_dist, talk_angle);

    if (mType == 1 && field_0xf8b == 0) {
        fopAc_ac_c* var_r3 = (fopAc_ac_c*)fpcM_Search(s_subBomb, this);
        if (var_r3 != NULL) {
            attn_flags = 0;
            field_0xfa8 = 10;
        } else if (cLib_calcTimer<int>(&field_0xfa8)) {
            attn_flags = 0;
        }
    }

    if (daPy_py_c::checkNowWolf()) {
        attention_info.flags = attn_flags | fopAc_AttnFlag_UNK_0x800000;
    } else {
        attention_info.flags = attn_flags;
    }

    scale.set(daNpc_Hoz_Param_c::m.common.scale, daNpc_Hoz_Param_c::m.common.scale, daNpc_Hoz_Param_c::m.common.scale);

    mAcchCir.SetWallR(daNpc_Hoz_Param_c::m.common.width);
    mAcchCir.SetWallH(daNpc_Hoz_Param_c::m.common.knee_length);

    mCcStts.SetWeight(daNpc_Hoz_Param_c::m.common.weight);
    mCylH = daNpc_Hoz_Param_c::m.common.height;
    mWallR = daNpc_Hoz_Param_c::m.common.width;

    if (mType == 4 || mType == 6) {
        gravity = 0.0f;
    } else {
        gravity = daNpc_Hoz_Param_c::m.common.gravity;
    }

    mExpressionMorfFrame = daNpc_Hoz_Param_c::m.common.expression_morf_frame;
    mMorfFrames = daNpc_Hoz_Param_c::m.common.morf_frame;
}

void daNpc_Hoz_c::setAfterTalkMotion() {}

void daNpc_Hoz_c::srchActors() {}

BOOL daNpc_Hoz_c::checkChangeEvt() {
    switch (mType) {
    case 4:
        if (!dComIfGs_isSwitch(field_0xf88, fopAcM_GetRoomNo(this))) {
            field_0xf8c = 7;
            mEvtNo = 7;
            evtChange();
            return 1;
        }
    }

    return FALSE;
}

BOOL daNpc_Hoz_c::evtTalk() {
    if (chkAction(&daNpc_Hoz_c::talk)) {
        (this->*mAction)(NULL);
    } else if (dComIfGp_event_chkTalkXY() == FALSE || dComIfGp_evmng_ChkPresentEnd()) {
        setAction(&daNpc_Hoz_c::talk);
    }

    return TRUE;
}

BOOL daNpc_Hoz_c::evtEndProc() {
    switch (field_0xf8c) {
    case 3:
        field_0xf8a = 1;
        field_0xf8c = 0;
        break;
    case 4:
        field_0xf8b = 1;
        field_0xf8c = 0;
        break;
    case 5: {
        field_0xf8a = 1;
        field_0xf8c = 0;

        int choiceNo = mFlow.getChoiceNo();
        if (choiceNo == 0) {
            dMeter2Info_setMiniGameItem(3);
            dMeter2Info_onMenuInForce(2);
        }
        break;
    }
    case 6:
        field_0xf8b = 1;

        if (daNpcT_chkTmpBit(0x43)) {
            field_0xf8c = 1;
            mEvtNo = 1;
            dMeter2Info_resetMiniGameItem(0);
            evtChange();
        }
        break;
    case 1:
        field_0xf8c = 0;
        mEvtNo = 0;
        break;
    case 8:
        if (mPotBreakFlag) {
            mPotBreakFlag = 0;
        }

        field_0xf8c = 0;
        mEvtNo = 0;
        break;
    default:
        field_0xf8c = 0;
        mEvtNo = 0;
        break;
    }

    return 1;
}

BOOL daNpc_Hoz_c::evtCutProc() {
    BOOL var_r30 = FALSE;

    int staffId = dComIfGp_getEventManager().getMyStaffId("Hoz", this, -1);
    if (staffId != -1) {
        mStaffId = staffId;
        int actIdx = dComIfGp_getEventManager().getMyActIdx(mStaffId, mCutNameList, ARRAY_SIZE(mCutNameList), 0, 0);

        if ((this->*mCutList[actIdx])(mStaffId)) {
            dComIfGp_getEventManager().cutEnd(mStaffId);
        }

        var_r30 = TRUE;
    }

    return var_r30;
}

int daNpc_Hoz_c::evtProc() {
    BOOL ret = FALSE;

#if VERSION != VERSION_SHIELD_DEBUG
        // TODO: gameInfo fake match to force reuse of pointer
        dComIfG_play_c* play = &g_dComIfG_gameInfo.play;
        if (play->getEvent().runCheck())
#else
        if (dComIfGp_event_runCheck())
#endif
        {
        if (eventInfo.checkCommandTalk()) {
            if (!checkChangeEvt()) {
                evtTalk();
            }
            ret = TRUE;
        } else if (eventInfo.checkCommandDemoAccrpt()
                        && dComIfGp_getEventManager().endCheck(mEvtId)) {
            if (evtEndProc()) {
#if VERSION != VERSION_SHIELD_DEBUG
                play->getEvent().reset();
#else
                dComIfGp_event_reset();
#endif
                mEvtId = -1;
            }
        } else {
            if (!strcmp(dComIfGp_getEventManager().getRunEventName(), "DEFAULT_GETITEM")
                                                    && mItemPartnerId != fpcM_ERROR_PROCESS_ID_e) {
                dComIfGp_event_setTalkPartner(NULL);
                dComIfGp_event_setItemPartnerId(mItemPartnerId);
                mItemPartnerId = fpcM_ERROR_PROCESS_ID_e;
            }
            ret = evtCutProc();
        }

        int temp_r28 = field_0xdb4;
        int face_motion, motion;
        if (ctrlMsgAnm(&face_motion, &motion, this, field_0xe2d)) {
            if (field_0xe26) {
                mFaceMotionSeqMngr.setNo(face_motion, -1.0f, TRUE, mFaceMotionSeqMngr.getOffset());
                mMotionSeqMngr.setNo(motion, -1.0f, TRUE, mMotionSeqMngr.getOffset());
            }
        } else if (temp_r28 != 0 && field_0xe26) {
            setAfterTalkMotion();
        }

        mAnmFlags &= 0xFFFFBFFF;
        field_0xe26 = true;
    } else {
        if (mStaffId != -1) {
            mMode = 1;
            mStaffId = -1;
        }

        field_0xdb4 = 0;
        field_0xe26 = true;
    }

    return ret;
}

void daNpc_Hoz_c::action() {
    if (field_0xfac != NULL) {
        if (mAction == field_0xfac) {
            (this->*mAction)(NULL);
        } else {
            setAction(field_0xfac);
        }
    }
}

void daNpc_Hoz_c::beforeMove() {
    if (checkHide() || mNoDraw) {
        attention_info.flags = 0;
    }
}

void daNpc_Hoz_c::setAttnPos() {
    cXyz sp34(0.0f, -30.0f, 0.0f);

    mJntAnm.setParam(this, mpMorf[0]->getModel(), &sp34, getBackboneJointNo(), getNeckJointNo(), getHeadJointNo(),
            daNpc_Hoz_Param_c::m.common.body_angleX_min, daNpc_Hoz_Param_c::m.common.body_angleX_max,
            daNpc_Hoz_Param_c::m.common.body_angleY_min, daNpc_Hoz_Param_c::m.common.body_angleY_max,
            daNpc_Hoz_Param_c::m.common.head_angleX_min, daNpc_Hoz_Param_c::m.common.head_angleX_max,
            daNpc_Hoz_Param_c::m.common.head_angleY_min, daNpc_Hoz_Param_c::m.common.head_angleY_max,
            daNpc_Hoz_Param_c::m.common.neck_rotation_ratio, 0.0f, NULL);

    f32 var_f31 = cM_s2rad(mCurAngle.y - field_0xd7e.y);
    mJntAnm.calcJntRad(0.2f, 1.0f, var_f31);

    setMtx();

    mDoMtx_stack_c::copy(mpMorf[0]->getModel()->getAnmMtx(getHeadJointNo()));
    mDoMtx_stack_c::multVec(&sp34, &eyePos);

    mJntAnm.setEyeAngleX(eyePos, 1.0f, 0);
    mJntAnm.setEyeAngleY(eyePos, mCurAngle.y, 1, 1.0f, 0);

    cXyz sp28;
    mDoMtx_stack_c::copy(mpMorf[0]->getModel()->getAnmMtx(4));
    mDoMtx_stack_c::multVecZero(&sp28);
    attention_info.position.set(sp28.x, sp28.y + daNpc_Hoz_Param_c::m.common.attention_offset, sp28.z);
}

void daNpc_Hoz_c::setCollision() {
    cXyz pos;

    if (!mHide) {
        mCyl.SetCoSPrm(0x79);

        f32 cyl_h;
        f32 cyl_r;
        if (mType == 4) {
            mDoMtx_stack_c::copy(mpMorf[0]->getModel()->getAnmMtx(1));

            if (daNpcT_chkEvtBit(0x50)) {
                cXyz sp8(0.0f, -15.0f, 0.0f);
                mDoMtx_stack_c::multVec(&sp8, &pos);
            } else {
                mDoMtx_stack_c::multVecZero(&pos);
            }

            pos.y -= 30.0f;
            cyl_h = daNpc_Hoz_Param_c::m.common.height;
            cyl_r = daNpc_Hoz_Param_c::m.common.width;
        } else {
            pos = current.pos;
            cyl_h = daNpc_Hoz_Param_c::m.common.height;
            cyl_r = daNpc_Hoz_Param_c::m.common.width;
        }

        mCyl.SetH(cyl_h);
        mCyl.SetR(cyl_r);
        mCyl.SetC(pos);
        dComIfG_Ccsp()->Set(&mCyl);
    }

    mCyl.ClrCoHit();
    mCyl.ClrTgHit();
}

void daNpc_Hoz_c::evtOrder() {
    if (strlen(mpEvtData[mEvtNo].eventName) != 0) {
        if (strlen(mpArcNames[mpEvtData[mEvtNo].num]) != 0) {
            eventInfo.setArchiveName(mpArcNames[mpEvtData[mEvtNo].num]);
            dComIfGp_getEventManager().setObjectArchive(eventInfo.getArchiveName());
        }

        mEvtId = dComIfGp_getEventManager().getEventIdx(this, mpEvtData[mEvtNo].eventName, 0xFF);
        if (mEvtNo == 2) {
            fopAcM_orderOtherEventId(this, mEvtId, 0xFF, 0xFFFF, 4, 3);
        } else {
            fopAcM_orderOtherEventId(this, mEvtId, 0xFF, 0xFFFF, 40, 1);
        }
    } else if ((!mTwilight || daPy_py_c::checkNowWolfEyeUp()) && ((attention_info.flags & fopAc_AttnFlag_SPEAK_e) || (attention_info.flags & fopAc_AttnFlag_TALK_e))) {
        eventInfo.onCondition(dEvtCnd_CANTALK_e);
        if (chkXYItems()) {
            eventInfo.onCondition(dEvtCnd_CANTALKITEM_e);
        }
        if (mSpeakEvent) {
            fopAcM_orderSpeakEvent(this, 0, 0);
        }
    }
}

BOOL daNpc_Hoz_c::drawDbgInfo() {
    return false;
}

int daNpc_Hoz_c::test(void* i_this) {
    // DEBUG NONMATCHING
}

int daNpc_Hoz_c::selectAction() {
    field_0xfac = NULL;

    switch (mType) {
    case 1:
        field_0xfac = &daNpc_Hoz_c::waitBoat;
        break;
    case 2:
        field_0xfac = &daNpc_Hoz_c::waitBoat2;
        break;
    case 3:
        field_0xfac = &daNpc_Hoz_c::waitBattle;
        break;
    case 4:
        field_0xfac = &daNpc_Hoz_c::waitTwilightBattle;
        break;
    case 5:
        field_0xfac = &daNpc_Hoz_c::waitBoat1_5;
        break;
    case 6:
        field_0xfac = &daNpc_Hoz_c::waitTw;
        break;
    default:
        field_0xfac = &daNpc_Hoz_c::wait;
        break;
    }

    return 1;
}

int daNpc_Hoz_c::chkAction(int (daNpc_Hoz_c::*i_action)(void*)) {
    return mAction == i_action;
}

int daNpc_Hoz_c::setAction(int (daNpc_Hoz_c::*i_action)(void*)) {
    mMode = 3;
    if (mAction != NULL) {
        (this->*mAction)(NULL);
    }

    mMode = 0;
    mAction = i_action;
    if (mAction != NULL) {
        (this->*mAction)(NULL);
    }

    return 1;
}

int daNpc_Hoz_c::wait(void* param_0) {
    switch (mMode) {
    case 0:
    case 1:
        mFaceMotionSeqMngr.setNo(0x22, -1.0f, 0, 0);
        mMotionSeqMngr.setNo(0, -1.0f, 0, 0);
        mMode = 2;
    case 2:
        if (!mTwilight) {
            if (srchPlayerActor()) {
                mJntAnm.lookPlayer(0);
            } else {
                if (home.angle.y != mCurAngle.y && step(home.angle.y, -1, -1, 0xF, 0)) {
                    mMode = 1;
                }

                mJntAnm.lookNone(0);
            }
        }
        break;
    case 3:
        break;
    }

    return 1;
}

int daNpc_Hoz_c::waitBattle(void* param_0) {
    switch (mMode) {
    case 0:
    case 1:
        if (field_0xf8a == 0) {
            mFaceMotionSeqMngr.setNo(0x22, -1.0f, 0, 0);
            mMotionSeqMngr.setNo(0, -1.0f, 0, 0);
        } else {
            mFaceMotionSeqMngr.setNo(0x14, -1.0f, 0, 0);
            mMotionSeqMngr.setNo(0x16, -1.0f, 0, 0);
        }
        mMode = 2;
    case 2:
        if (field_0xf8a == 0 && dComIfGs_isSwitch(field_0xf88, fopAcM_GetRoomNo(this))) {
            field_0xf8c = 3;
            mEvtNo = 3;
            field_0xe33 = 1;
        } else if (field_0xf8b == 0 && dComIfGs_isSwitch(field_0xf89, fopAcM_GetRoomNo(this))) {
            field_0xf8c = 4;
            mEvtNo = 4;
            field_0xe33 = 1;
        } else if (srchPlayerActor()) {
            mJntAnm.lookPlayer(0);
        } else {
            if (home.angle.y != mCurAngle.y && step(home.angle.y, -1, -1, 0xF, 0)) {
                mMode = 1;
            }
            mJntAnm.lookNone(0);
        }
        break;
    case 3:
        break;
    }

    return 1;
}

int daNpc_Hoz_c::waitTwilightBattle(void* param_0) {
    switch (mMode) {
    case 0:
    case 1:
        if (daNpcT_chkEvtBit(0x50) != 0) {
            if (mMotionSeqMngr.getNo() == 10) {
                mMotionSeqMngr.setNo(0x20, -1.0f, 0, 0);
            } else {
                mMotionSeqMngr.setNo(0x23, -1.0f, 0, 0);
            }

            if (mFaceMotionSeqMngr.getNo() == 9) {
                mFaceMotionSeqMngr.setNo(0x1B, -1.0f, 0, 0);
            } else {
                mFaceMotionSeqMngr.setNo(0x21, -1.0f, 0, 0);
            }
        } else {
            mFaceMotionSeqMngr.setNo(1, -1.0f, 0, 0);
            mMotionSeqMngr.setNo(2, -1.0f, 0, 0);
        }

        mMode = 2;
    case 2:
        if (!mTwilight) {
            if (srchPlayerActor()) {
                mJntAnm.lookPlayer(0);
            } else {
                if (home.angle.y != mCurAngle.y && step(home.angle.y, -1, -1, 0xF, 0)) {
                    mMode = 1;
                }
                mJntAnm.lookNone(0);
            }
        }
        break;
    case 3:
        break;
    }

    return 1;
}

int daNpc_Hoz_c::waitBoat(void* param_0) {
    switch (mMode) {
    case 0:
    case 1:
        mFaceMotionSeqMngr.setNo(0x22, -1.0f, 0, 0);
        switch (mMotionSeqMngr.getNo()) {
        case 16:
        case 14:
            mMotionSeqMngr.setNo(0x21, -1.0f, 0, 0);
            break;
        case 15:
            mMotionSeqMngr.setNo(0x22, -1.0f, 0, 0);
            break;
        case 28:
            mMotionSeqMngr.setNo(0x1F, -1.0f, 0, 0);
            break;
        default:
            mMotionSeqMngr.setNo(0, -1.0f, 0, 0);
            break;
        }
        mMode = 2;
    case 2:
        if (field_0xf84 == NULL) {
            field_0xf84 = (daStartAndGoal_c*)fpcM_Search(s_sub, this);
            dMeter2Info_setNowCount(0);
            dMeter2Info_setMiniGameCount(0);
        }

        if (field_0xfa0.getActorP() == NULL) {
            fopAc_ac_c* temp_r3_2 = (fopAc_ac_c*)fpcM_Search(s_subCanoe, this);
            if (temp_r3_2 != NULL) {
                field_0xfa0.entry(temp_r3_2);
                field_0xf90 = temp_r3_2->current.pos;
                field_0xf9c = temp_r3_2->current.angle.y;
            }
        }

        if (field_0xf8a == 0) {
            if (strcmp(dComIfGp_getStartStageName(), "F_SP112") == 0 && fopAcM_GetRoomNo(this) == 1 && dComIfGs_getStartPoint() == 2) {
                field_0xf8c = 5;
                mEvtNo = 5;
                field_0xe33 = 1;
                break;
            }
            
            field_0xf8a = 1;
        }

        if (field_0xf8b == 0 && dComIfGs_isSwitch(field_0xf88, fopAcM_GetRoomNo(this))) {
            field_0xf8c = 6;
            mEvtNo = 6;
            field_0xe33 = 1;
        } else if (mGameStartFlag) {
            mJntAnm.lookPlayer(0);
        } else {
            if (srchPlayerActor()) {
                mJntAnm.lookPlayer(0);
            } else {
                if (home.angle.y != mCurAngle.y && step(home.angle.y, -1, -1, 0xF, 0)) {
                    mMode = 1;
                }
                mJntAnm.lookNone(0);
            }

            daCanoe_c* temp_r3_3 = (daCanoe_c*)field_0xfa0.getActorP();
            if (temp_r3_3 != NULL) {
                temp_r3_3->setPosAndAngle(&field_0xf90, field_0xf9c);
            }
        }
        break;
    case 3:
        break;
    }

    return 1;
}

int daNpc_Hoz_c::waitBoat2(void* param_0) {
    switch (mMode) {
    case 0:
    case 1:
        mFaceMotionSeqMngr.setNo(0x22, -1.0f, 0, 0);
        switch (mMotionSeqMngr.getNo()) {
        case 16:
        case 14:
            mMotionSeqMngr.setNo(0x21, -1.0f, 0, 0);
            break;
        case 15:
            mMotionSeqMngr.setNo(0x22, -1.0f, 0, 0);
            break;
        case 28:
            mMotionSeqMngr.setNo(0x1F, -1.0f, 0, 0);
            break;
        default:
            mMotionSeqMngr.setNo(0, -1.0f, 0, 0);
            break;
        }
        mMode = 2;
    case 2:
        if (field_0xf84 == NULL) {
            field_0xf84 = (daStartAndGoal_c*)fpcM_Search(s_sub, this);
            dMeter2Info_setNowCount(0);
            dMeter2Info_setMiniGameCount(0);
        }

        if (dComIfGs_isSaveDunSwitch(0x3E) != 0) {
            field_0xe33 = 1;
            field_0xf8c = 2;
            mEvtNo = 2;
        } else {
            if (field_0xfa0.getActorP() == NULL) {
                fopAc_ac_c* temp_r3_2 = (fopAc_ac_c*)fpcM_Search(s_subCanoe, this);
                if (temp_r3_2 != NULL) {
                    field_0xfa0.entry(temp_r3_2);
                    field_0xf90 = temp_r3_2->current.pos;
                    field_0xf9c = temp_r3_2->current.angle.y;
                }
            }

            if (!daNpcT_chkEvtBit(0x7F)) {
                mSpeakEvent = 1;
            } else if (mPotBreakFlag == true) {
                field_0xf8c = 8;
                mEvtNo = 8;
                break;
            }

            if (mGameStartFlag) {
                mJntAnm.lookPlayer(0);
            } else {
                if (srchPlayerActor()) {
                    mJntAnm.lookPlayer(0);
                } else {
                    if (home.angle.y != mCurAngle.y && step(home.angle.y, -1, -1, 0xF, 0)) {
                        mMode = 1;
                    }
                    mJntAnm.lookNone(0);
                }

                daCanoe_c* temp_r3_3 = (daCanoe_c*)field_0xfa0.getActorP();
                if (temp_r3_3 != NULL) {
                    temp_r3_3->setPosAndAngle(&field_0xf90, field_0xf9c);
                }
            }
        }
        break;
    case 3:
        break;
    }

    return 1;
}

int daNpc_Hoz_c::waitBoat1_5(void* param_0) {
    switch (mMode) {
    case 0:
    case 1:
        mFaceMotionSeqMngr.setNo(0x22, -1.0f, 0, 0);
        switch (mMotionSeqMngr.getNo()) {
        case 16:
        case 14:
            mMotionSeqMngr.setNo(0x21, -1.0f, 0, 0);
            break;
        case 15:
            mMotionSeqMngr.setNo(0x22, -1.0f, 0, 0);
            break;
        case 28:
            mMotionSeqMngr.setNo(0x1F, -1.0f, 0, 0);
            break;
        default:
            mMotionSeqMngr.setNo(0, -1.0f, 0, 0);
            break;
        }
        mMode = 2;
    case 2:
        if (field_0xf84 == NULL) {
            field_0xf84 = (daStartAndGoal_c*)fpcM_Search(s_sub, this);
            dMeter2Info_setNowCount(0);
            dMeter2Info_setMiniGameCount(0);
        }

        if (field_0xfa0.getActorP() == NULL) {
            fopAc_ac_c* temp_r3_2 = (fopAc_ac_c*)fpcM_Search(s_subCanoe, this);
            if (temp_r3_2 != NULL) {
                field_0xfa0.entry(temp_r3_2);
                field_0xf90 = temp_r3_2->current.pos;
                field_0xf9c = temp_r3_2->current.angle.y;
            }
        }

        if (field_0xf8a == 0 && (current.pos - daPy_getPlayerActorClass()->current.pos).absXZ() <= daNpc_Hoz_Param_c::m.field_0x8c) {
            mSpeakEvent = 1;
        }

        if (mGameStartFlag) {
            mJntAnm.lookPlayer(0);
        } else {
            if (srchPlayerActor()) {
                mJntAnm.lookPlayer(0);
            } else {
                if (home.angle.y != mCurAngle.y && step(home.angle.y, -1, -1, 0xF, 0)) {
                    mMode = 1;
                }
                mJntAnm.lookNone(0);
            }

            daCanoe_c* temp_r3_3 = (daCanoe_c*)field_0xfa0.getActorP();
            if (temp_r3_3 != NULL) {
                temp_r3_3->setPosAndAngle(&field_0xf90, field_0xf9c);
            }
        }
        break;
    case 3:
        break;
    }

    return 1;
}

int daNpc_Hoz_c::waitTw(void* param_0) {
    switch (mMode) {
    case 0:
    case 1:
        if (daNpcF_chkEvtBit(0x2C0)) {
            mFaceMotionSeqMngr.setNo(0x1A, -1.0f, 0, 0);
            mMotionSeqMngr.setNo(0x1E, -1.0f, 0, 0);
        } else {
            mFaceMotionSeqMngr.setNo(0x21, -1.0f, 0, 0);
            mMotionSeqMngr.setNo(0x23, -1.0f, 0, 0);
        }
        mMode = 2;
    case 2:
        if (!mTwilight) {
            if (srchPlayerActor()) {
                mJntAnm.lookPlayer(0);
            } else {
                if (home.angle.y != mCurAngle.y && step(home.angle.y, -1, -1, 0xF, 0)) {
                    mMode = 1;
                }

                mJntAnm.lookNone(0);
            }
        }
        break;
    case 3:
        break;
    }

    return 1;
}

int daNpc_Hoz_c::talk(void* param_0) {
    BOOL var_r29 = FALSE;

    switch (mMode) {
    case 0:
    case 1:
        initTalk(mFlowNodeNo, NULL);
        mMode = 2;
    case 2:
        
        if (!mTwilight) {
            mJntAnm.lookPlayer(0);

            if (mCurAngle.y == fopAcM_searchPlayerAngleY(this)) {
                var_r29 = 1;
            } else if (step(fopAcM_searchPlayerAngleY(this), -1, -1, 0xF, 0)) {
                var_r29 = 1;
            }
        } else {
            var_r29 = 1;
        }

        if (var_r29 && talkProc(NULL, 0, NULL, 0)) {
            mFlowNodeNo = getFlowNodeNo();
            mPlayerActorMngr.entry(daPy_getPlayerActorClass());
            dComIfGp_event_reset();
            mMode = 3;

            int choiceNo;
            switch (mType) {
            case 1:
                if (dComIfGs_isSwitch(field_0xf88, fopAcM_GetRoomNo(this))) {
                    choiceNo = mFlow.getChoiceNo();
                    if (choiceNo == 0) {
                        field_0xf8c = 1;
                        mEvtNo = 1;
                        evtChange();
                    }
                } else if (dMeter2Info_getRentalBombBag() == 0xFF) {
                    choiceNo = mFlow.getChoiceNo();
                    if (choiceNo == 0) {
                        dMeter2Info_setMiniGameItem(3);
                        dMeter2Info_onMenuInForce(2);
                    }
                }
                break;
            case 2:
                if (daNpcT_chkTmpBit(0x43)) {
                    choiceNo = mFlow.getChoiceNo();
                    if (choiceNo == 0) {
                        field_0xf8c = 1;
                        mEvtNo = 1;
                        evtChange();
                    }
                }
                break;
            case 5:
                field_0xf8a = 1;
                choiceNo = mFlow.getChoiceNo();
                if (choiceNo == 0) {
                    field_0xf8c = 1;
                    mEvtNo = 1;
                    evtChange();
                }
                break;
            }
        }
        break;
    case 3:
        break;
    }

    return 0;
}

int daNpc_Hoz_c::ECut_boatRace(int i_staffId) {
    dEvent_manager_c& evtmgr = dComIfGp_getEventManager();
    int rt = 0;
    int prm = -1;

    int* data_p = dComIfGp_evmng_getMyIntegerP(i_staffId, "prm");
    if (data_p != NULL) {
        prm = *data_p;
    }

    if (evtmgr.getIsAddvance(i_staffId)) {
        switch (prm) {
        case 40:
        case 0:
            break;
        case 10: {
            daPy_getPlayerActorClass()->setCanoeSlider();
            dMeter2Info_resetMiniGameItem(0);
            dMeter2Info_setMiniGameItem(1);

            setAngle(0);
            home.angle.y = 0;
            cXyz sp18(-5083.76f, 17082.0f, -89740.0f);
            setPos(sp18);
            break;
        }
        case 11:
            current.angle.y = fopAcM_searchPlayerAngleY(this);
            setAngle(current.angle.y);
            mFaceMotionSeqMngr.setNo(0x22, -1.0f, 0, 0);
            mMotionSeqMngr.setNo(0x18, -1.0f, 0, 0);
            break;
        case 20:
            initTalk(mFlowNodeNo, NULL);
            break;
        case 30:
            if (field_0xf84 != NULL) {
                field_0xf84->readyStartTimer();
                mEventTimer = 30;
            }
            break;
        }
    }

    switch (prm) {
    case 0:
        if (field_0xf84 == NULL) {
            field_0xf84 = (daStartAndGoal_c*)fpcM_Search(s_sub, this);
            if (field_0xf84 != NULL) {
                dMeter2Info_setNowCount(0);
                dMeter2Info_setMiniGameCount(0);
            }
        }

        if (field_0xfa0.getActorP() == NULL) {
            fopAc_ac_c* temp_r3_2 = (fopAc_ac_c*)fpcM_Search(s_subCanoe, this);
            if (temp_r3_2 != NULL) {
                field_0xfa0.entry(temp_r3_2);
            }
        }

        if (field_0xf84 != NULL && field_0xfa0.getActorP() != NULL) {
            rt = 1;
        }
        break;
    case 10:
    case 11:
        rt = 1;
        break;
    case 20:
        if (talkProc(NULL, 0, NULL, 0)) {
            rt = 1;
        }
        break;
    case 30:
        if (cLib_calcTimer<int>(&mEventTimer) == 0) {
            mGameStartFlag = 1;
            rt = 1;
        }
    case 40:
        if (field_0xf84 != NULL && field_0xf84->isStartCheck()) {
            mGameStartFlag = 1;
            rt = 1;
            dComIfGs_onSwitch(field_0xf89, fopAcM_GetRoomNo(this));
        }
        break;
    default:
        rt = 1;
    }

    return rt;
}

int daNpc_Hoz_c::ECut_beforeBattle(int i_staffId) {
    dEvent_manager_c& evtmgr = dComIfGp_getEventManager();
    int rt = 0;
    int prm = -1;

    int* data_p = dComIfGp_evmng_getMyIntegerP(i_staffId, "prm");
    if (data_p != NULL) {
        prm = *data_p;
    }

    if (evtmgr.getIsAddvance(i_staffId)) {
        switch (prm) {
        case 0:
            break;
        case 10:
            initTalk(mFlowNodeNo, NULL);
            break;
        }
    }

    switch (prm) {
    case 0:
        rt = 1;
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

int daNpc_Hoz_c::ECut_afterBattle(int i_staffId) {
    dEvent_manager_c& evtmgr = dComIfGp_getEventManager();
    int rt = 0;
    int prm = -1;

    int* data_p = dComIfGp_evmng_getMyIntegerP(i_staffId, "prm");
    if (data_p != NULL) {
        prm = *data_p;
    }

    if (evtmgr.getIsAddvance(i_staffId)) {
        switch (prm) {
        case 0:
            break;
        case 10:
            initTalk(mFlowNodeNo, NULL);
            break;
        }
    }

    switch (prm) {
    case 0:
        if (mCurAngle.y != fopAcM_searchPlayerAngleY(this)) {
            step(fopAcM_searchPlayerAngleY(this), -1, -1, 0xF, 0);
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

int daNpc_Hoz_c::ECut_beforeBlast(int i_staffId) {
    dEvent_manager_c& evtmgr = dComIfGp_getEventManager();
    int rt = 0;
    int prm = -1;

    int* data_p = dComIfGp_evmng_getMyIntegerP(i_staffId, "prm");
    if (data_p != NULL) {
        prm = *data_p;
    }

    if (evtmgr.getIsAddvance(i_staffId)) {
        switch (prm) {
        case 0:
            break;
        case 10:
            initTalk(mFlowNodeNo, NULL);
            break;
        }
    }

    switch (prm) {
    case 0:
        if (mCurAngle.y != fopAcM_searchPlayerAngleY(this)) {
            step(fopAcM_searchPlayerAngleY(this), -1, -1, 0xF, 0);
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

int daNpc_Hoz_c::ECut_afterBlast(int i_staffId) {
    dEvent_manager_c& evtmgr = dComIfGp_getEventManager();
    int rt = 0;
    int prm = -1;

    int* data_p = dComIfGp_evmng_getMyIntegerP(i_staffId, "prm");
    if (data_p != NULL) {
        prm = *data_p;
    }

    if (evtmgr.getIsAddvance(i_staffId)) {
        switch (prm) {
        case 0:
            break;
        case 10:
            initTalk(mFlowNodeNo, NULL);
            break;
        }
    }

    switch (prm) {
    case 0:
        if (mCurAngle.y != fopAcM_searchPlayerAngleY(this)) {
            step(fopAcM_searchPlayerAngleY(this), -1, -1, 0xF, 0);
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

int daNpc_Hoz_c::ECut_yMushi(int i_staffId) {
    dEvent_manager_c& evtmgr = dComIfGp_getEventManager();
    int rt = 0;
    int prm = -1;

    int* data_p = dComIfGp_evmng_getMyIntegerP(i_staffId, "prm");
    if (data_p != NULL) {
        prm = *data_p;
    }

    if (evtmgr.getIsAddvance(i_staffId)) {
        switch (prm) {
        case 0:
            dComIfGs_onSwitch(field_0xf88, fopAcM_GetRoomNo(this));
            break;
        case 10:
            initTalk(mFlowNodeNo, NULL);
            break;
        }
    }

    switch (prm) {
    case 0:
        rt = 1;
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

int daNpc_Hoz_c::ECut_talkBreak(int i_staffId) {
    dEvent_manager_c& evtmgr = dComIfGp_getEventManager();
    int rt = 0;
    int prm = -1;

    int* data_p = dComIfGp_evmng_getMyIntegerP(i_staffId, "prm");
    if (data_p != NULL) {
        prm = *data_p;
    }

    if (evtmgr.getIsAddvance(i_staffId)) {
        switch (prm) {
        case 0:
            break;
        case 10:
            initTalk(0x3E1, NULL);
            break;
        }
    }

    switch (prm) {
    case 0:
        if (mCurAngle.y != fopAcM_searchPlayerAngleY(this)) {
            step(fopAcM_searchPlayerAngleY(this), -1, -1, 0xF, 0);
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

static int daNpc_Hoz_Create(void* i_this) {
    return ((daNpc_Hoz_c*)i_this)->create();
}

static int daNpc_Hoz_Delete(void* i_this) {
    return ((daNpc_Hoz_c*)i_this)->Delete();
}

static int daNpc_Hoz_Execute(void* i_this) {
    return ((daNpc_Hoz_c*)i_this)->Execute();
}

static int daNpc_Hoz_Draw(void* i_this) {
    return ((daNpc_Hoz_c*)i_this)->Draw();
}

static int daNpc_Hoz_IsDelete(void* i_this) {
    return true;
}

static daNpc_Hoz_Param_c l_HIO;

static actor_method_class daNpc_Hoz_MethodTable = {
    (process_method_func)daNpc_Hoz_Create,
    (process_method_func)daNpc_Hoz_Delete,
    (process_method_func)daNpc_Hoz_Execute,
    (process_method_func)daNpc_Hoz_IsDelete,
    (process_method_func)daNpc_Hoz_Draw,
};

extern actor_process_profile_definition g_profile_NPC_HOZ = {
  fpcLy_CURRENT_e,          // mLayerID
  7,                        // mListID
  fpcPi_CURRENT_e,          // mListPrio
  PROC_NPC_HOZ,             // mProcName
  &g_fpcLf_Method.base,    // sub_method
  sizeof(daNpc_Hoz_c),      // mSize
  0,                        // mSizeOther
  0,                        // mParameters
  &g_fopAc_Method.base,     // sub_method
  344,                      // mPriority
  &daNpc_Hoz_MethodTable,   // sub_method
  0x00044108,               // mStatus
  fopAc_NPC_e,              // mActorType
  fopAc_CULLBOX_CUSTOM_e,   // cullType
};
