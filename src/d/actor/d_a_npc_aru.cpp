/**
 * @file d_a_npc_aru.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_npc_aru.h"
#include "d/actor/d_a_cow.h"
#include "d/actor/d_a_obj_udoor.h"
#include "d/d_timer.h"
#include "d/d_s_play.h"
#include "d/actor/d_a_tag_evtarea.h"
#include "d/d_meter2_info.h"
#include "d/actor/d_a_horse.h"
#include "Z2AudioLib/Z2Instances.h"

enum Aru_RES_File_ID {
    /* BCK */
    /* 0x06 */ BCK_ARU_F_NORMAL = 0x6,
    /* 0x07 */ BCK_ARU_FH_NORMAL,
    /* 0x08 */ BCK_ARU_STEP_R,
    /* 0x09 */ BCK_ARU_WAIT_A,

    /* BMDR */
    /* 0x0C */ BMDR_ARU = 0xC,

    /* BTK */
    /* 0x0F */ BTK_ARU = 0xF,

    /* BTP */
    /* 0x12 */ BTP_ARU = 0x12,
    /* 0x13 */ BTP_ARU_FH_NORMAL,
};

enum Aru1_RES_File_ID {
    /* BCK */
    /* 0x06 */ BCK_ARU_F_BITTERSMILE = 0x6,
    /* 0x07 */ BCK_ARU_F_GRUMPY,
    /* 0x08 */ BCK_ARU_F_SERIOUS,
    /* 0x09 */ BCK_ARU_F_SMILE,
    /* 0x0A */ BCK_ARU_F_WORRY,
    /* 0x0B */ BCK_ARU_FH_BITTERSMILE,
    /* 0x0C */ BCK_ARU_FH_GRUMPY,
    /* 0x0D */ BCK_ARU_FH_SERIOUS,
    /* 0x0E */ BCK_ARU_FH_SMILE,
    /* 0x0F */ BCK_ARU_FH_WORRY,
    /* 0x10 */ BCK_ARU_HI,
    /* 0x11 */ BCK_ARU_LOOKBACK,
    /* 0x12 */ BCK_ARU_LOOKOVER,
    /* 0x13 */ BCK_ARU_RUN,
    /* 0x14 */ BCK_ARU_TALK_A,
    /* 0x15 */ BCK_ARU_TALK_B,
    /* 0x16 */ BCK_ARU_TALK_C,
    /* 0x17 */ BCK_ARU_TALK_D,
    /* 0x18 */ BCK_ARU_TALK_E,
    /* 0x19 */ BCK_ARU_WAIT_C,
    /* 0x1A */ BCK_ARU_WAIT_D,

    /* BTK */
    /* 0x1D */ BTK_ARU_LOOKBACK = 0x1D,
    /* 0x1E */ BTK_ARU_LOOKOVER,
    /* 0x1F */ BTK_ARU_TALK_C,
    /* 0x20 */ BTK_ARU_TALK_D,

    /* BTP */
    /* 0x23 */ BTP_ARU_F_BITTERSMILE = 0x23,
    /* 0x24 */ BTP_ARU_F_GRUMPY,
    /* 0x25 */ BTP_ARU_F_SERIOUS,
    /* 0x26 */ BTP_ARU_F_SMILE,
    /* 0x27 */ BTP_ARU_F_WORRY,
    /* 0x28 */ BTP_ARU_FH_BITTERSMILE,
    /* 0x29 */ BTP_ARU_FH_GRUMPY,
    /* 0x2A */ BTP_ARU_FH_SERIOUS,
    /* 0x2B */ BTP_ARU_FH_SMILE,
    /* 0x2C */ BTP_ARU_FH_WORRY,
};

enum Face_Motion {
    /* 0x0 */ FACE_NORMAL,
    /* 0x1 */ FACE_GRUMPY,
    /* 0x2 */ FACE_WORRY,
    /* 0x3 */ FACE_BITTERSMILE,
    /* 0x4 */ FACE_SMILE,
    /* 0x5 */ FACE_SERIOUS,
    /* 0x6 */ FACE_H_NORMAL,
    /* 0x7 */ FACE_H_GRUMPY,
    /* 0x8 */ FACE_H_WORRY,
    /* 0x9 */ FACE_H_BITTERSMILE,
    /* 0xA */ FACE_H_SMILE,
    /* 0xB */ FACE_H_SERIOUS,
    /* 0xC */ FACE_NONE,
};

enum Motion {
    /* 0x0 */ MOT_WAIT_A,
    /* 0x1 */ MOT_LOOKBACK,
    /* 0x2 */ MOT_LOOKOVER,
    /* 0x3 */ MOT_WAIT_C,
    /* 0x4 */ MOT_TALK_B,
    /* 0x5 */ MOT_TALK_A,
    /* 0x6 */ MOT_TALK_C,
    /* 0x7 */ MOT_HI,
    /* 0x8 */ MOT_TALK_D,
    /* 0x9 */ MOT_TALK_E,
    /* 0xA */ MOT_RUN,
    /* 0xB */ MOT_STEP_R,
};

enum Event {
    /* 0x0 */ EVT_NONE,
    /* 0x1 */ EVT_DEFAULT_GETITEM,
    /* 0x2 */ EVT_NO_RESPONSE,
    /* 0x3 */ EVT_RIDEON_HORSE,
    /* 0x4 */ EVT_GOTO_BULLRUNNING_STAGE,
    /* 0x5 */ EVT_GOTO_BULLRUNNING_STAGE_VER2,
    /* 0x6 */ EVT_END_BULLRUNNING,
    /* 0x7 */ EVT_END_BULLRUNNING2,
    /* 0x8 */ EVT_END_BULLRUNNING3,
    /* 0x9 */ EVT_GOTO_SKIPFENCE_STAGE,
    /* 0xA */ EVT_SPEAKTO,
    /* 0xB */ EVT_NO_ENTRANCE,
};

enum RES_Name {
    /* 0x0 */ NONE,
    /* 0x1 */ ARU,
    /* 0x2 */ ARU1,
};

enum Type {
    /* 0x0 */ TYPE_0,
    /* 0x1 */ TYPE_1,
    /* 0x2 */ TYPE_2,
    /* 0x3 */ TYPE_3,
    /* 0x4 */ TYPE_4,
};

static int l_bmdData[1][2] = {
    {BMDR_ARU, ARU},
};

static daNpcT_evtData_c l_evtList[12] = {
    {"", 0},
    {"DEFAULT_GETITEM", 0},
    {"NO_RESPONSE", 0},
    {"RIDEON_HORSE", 2},
    {"GOTO_BULLRUNNING_STAGE", 2},
    {"GOTO_BULLRUNNING_STAGE_VER2", 2},
    {"END_BULLRUNNING", 2},
    {"END_BULLRUNNING2", 2},
    {"END_BULLRUNNING3", 2},
    {"GOTO_SKIPFENCE_STAGE", 2},
    {"SPEAKTO", 2},
    {"NO_ENTRANCE", 2},
};

static char* l_resNameList[3] = {
    "",
    "Aru",
    "Aru1",
};

static s8 l_loadResPtrn0[2] = {
    ARU, -1
};

static s8 l_loadResPtrn9[3] = {
    ARU, ARU1, -1,
};

static s8* l_loadResPtrnList[6] = {
    l_loadResPtrn9, l_loadResPtrn9, l_loadResPtrn9,
    l_loadResPtrn9, l_loadResPtrn9, l_loadResPtrn9,
};

static daNpcT_faceMotionAnmData_c l_faceMotionAnmData[13] = {
    {-1, J3DFrameCtrl::EMode_NONE, 0, BTP_ARU, J3DFrameCtrl::EMode_LOOP, ARU, 1},
    {BCK_ARU_F_NORMAL, J3DFrameCtrl::EMode_NONE, ARU, BTP_ARU, J3DFrameCtrl::EMode_LOOP, ARU, 1},
    {BCK_ARU_F_GRUMPY, J3DFrameCtrl::EMode_NONE, ARU1, BTP_ARU_F_GRUMPY, J3DFrameCtrl::EMode_NONE, ARU1, 0},
    {BCK_ARU_F_WORRY, J3DFrameCtrl::EMode_NONE, ARU1, BTP_ARU_F_WORRY, J3DFrameCtrl::EMode_NONE, ARU1, 0},
    {BCK_ARU_F_BITTERSMILE, J3DFrameCtrl::EMode_NONE, ARU1, BTP_ARU_F_BITTERSMILE, J3DFrameCtrl::EMode_NONE, ARU1, 0},
    {BCK_ARU_F_SMILE, J3DFrameCtrl::EMode_NONE, ARU1, BTP_ARU_F_SMILE, J3DFrameCtrl::EMode_NONE, ARU1, 0},
    {BCK_ARU_F_SERIOUS, J3DFrameCtrl::EMode_NONE, ARU1, BTP_ARU_F_SERIOUS, J3DFrameCtrl::EMode_NONE, ARU1, 0},
    {BCK_ARU_FH_GRUMPY, J3DFrameCtrl::EMode_LOOP, ARU1, BTP_ARU_FH_GRUMPY, J3DFrameCtrl::EMode_LOOP, ARU1, 0},
    {BCK_ARU_FH_WORRY, J3DFrameCtrl::EMode_LOOP, ARU1, BTP_ARU_FH_WORRY, J3DFrameCtrl::EMode_LOOP, ARU1, 0},
    {BCK_ARU_FH_BITTERSMILE, J3DFrameCtrl::EMode_LOOP, ARU1, BTP_ARU_FH_BITTERSMILE, J3DFrameCtrl::EMode_LOOP, ARU1, 0},
    {BCK_ARU_FH_SERIOUS, J3DFrameCtrl::EMode_LOOP, ARU1, BTP_ARU_FH_SERIOUS, J3DFrameCtrl::EMode_LOOP, ARU1, 0},
    {BCK_ARU_FH_SMILE, J3DFrameCtrl::EMode_LOOP, ARU1, BTP_ARU_FH_SMILE, J3DFrameCtrl::EMode_LOOP, ARU1, 0},
    {BCK_ARU_FH_NORMAL, J3DFrameCtrl::EMode_LOOP, ARU, BTP_ARU_FH_NORMAL, J3DFrameCtrl::EMode_LOOP, ARU, 0},
};

static daNpcT_motionAnmData_c l_motionAnmData[13] = {
    {BCK_ARU_WAIT_A, J3DFrameCtrl::EMode_LOOP, ARU, BTK_ARU,J3DFrameCtrl::EMode_NONE, ARU, 1, 0},
    {BCK_ARU_WAIT_C, J3DFrameCtrl::EMode_LOOP, ARU1, BTK_ARU, J3DFrameCtrl::EMode_NONE, ARU, 1, 0},
    {BCK_ARU_WAIT_D, J3DFrameCtrl::EMode_LOOP, ARU1, BTK_ARU, J3DFrameCtrl::EMode_NONE, ARU, 1, 0},
    {BCK_ARU_LOOKBACK, J3DFrameCtrl::EMode_NONE, ARU1, BTK_ARU_LOOKBACK, J3DFrameCtrl::EMode_NONE, ARU1, 0, 0},
    {BCK_ARU_LOOKOVER, J3DFrameCtrl::EMode_NONE, ARU1, BTK_ARU_LOOKOVER, J3DFrameCtrl::EMode_NONE, ARU1, 0, 0},
    {BCK_ARU_TALK_A, J3DFrameCtrl::EMode_NONE, ARU1, BTK_ARU, J3DFrameCtrl::EMode_NONE, ARU, 1, 0},
    {BCK_ARU_TALK_B, J3DFrameCtrl::EMode_NONE, ARU1, BTK_ARU, J3DFrameCtrl::EMode_NONE, ARU, 1, 0},
    {BCK_ARU_HI, J3DFrameCtrl::EMode_NONE, ARU1, BTK_ARU, J3DFrameCtrl::EMode_NONE, ARU, 1, 0},
    {BCK_ARU_TALK_C, J3DFrameCtrl::EMode_LOOP, ARU1, BTK_ARU_TALK_C, J3DFrameCtrl::EMode_LOOP, ARU1, 0, 0},
    {BCK_ARU_TALK_D, J3DFrameCtrl::EMode_LOOP, ARU1, BTK_ARU_TALK_D, J3DFrameCtrl::EMode_LOOP, ARU1, 0, 0},
    {BCK_ARU_TALK_E, J3DFrameCtrl::EMode_NONE, ARU1, BTK_ARU, J3DFrameCtrl::EMode_NONE, ARU, 1, 0},
    {BCK_ARU_STEP_R, J3DFrameCtrl::EMode_NONE, ARU, BTK_ARU, J3DFrameCtrl::EMode_NONE, ARU, 1, 0},
    {BCK_ARU_RUN, J3DFrameCtrl::EMode_LOOP, ARU1, BTK_ARU, J3DFrameCtrl::EMode_NONE, ARU, 1, 0},
};

static daNpcT_MotionSeqMngr_c::sequenceStepData_c l_faceMotionSequenceData[52] = {
    {1, -1, 1}, {12, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {2, -1, 1}, {7, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {3, -1, 1}, {8, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {4, -1, 1}, {9, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {5, -1, 1}, {11, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {6, -1, 1}, {10, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {12, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {7, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {8, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {9, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {11, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {10, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {0, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
};

static daNpcT_MotionSeqMngr_c::sequenceStepData_c l_motionSequenceData[48] = {
    {0, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {3, -1, 1}, {0, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {4, -1, 1}, {0, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {1, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {6, -1, 1}, {1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {5, -1, 1}, {0, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {8, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {7, -1, 1}, {0, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {9, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {10, -1, 1}, {2, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {12, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {11, 4, 1}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
};

char* daNpc_Aru_c::mCutNameList[7] = {
    "",
    "RIDEON_HORSE",
    "GOTO_BULLRUNNING_STAGE",
    "END_BULLRUNNING",
    "GOTO_SKIPFENCE_STAGE",
    "SPEAKTO",
    "NO_ENTRANCE",
};

daNpc_Aru_c::cutFunc daNpc_Aru_c::mCutList[7] = {
    NULL,
    &daNpc_Aru_c::cutRideOnHorse,
    &daNpc_Aru_c::cutGotoBullRunningStage,
    &daNpc_Aru_c::cutEndBullRunning,
    &daNpc_Aru_c::cutGotoSkipFenceStage,
    &daNpc_Aru_c::cutSpeakTo,
    &daNpc_Aru_c::cutNoEntrance,
};

daNpc_Aru_c::~daNpc_Aru_c() {
    if (mpMorf[0] != NULL) {
        mpMorf[0]->stopZelAnime();
    }

    deleteRes(l_loadResPtrnList[mType], (const char**)l_resNameList);
}

const daNpc_Aru_HIOParam daNpc_Aru_Param_c::m = {
    220.0f,
    -3.0f,
    1.0f,
    600.0f,
    255.0f,
    210.0f,
    35.0f,
    40.0f,
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
    6,
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
    1500.0f,
    24.0f,
    4.0f,
    10,
    20,
    45.0f,
};

cPhs__Step daNpc_Aru_c::create() {
    daNpcT_ct(this, daNpc_Aru_c, l_faceMotionAnmData, l_motionAnmData,
                       l_faceMotionSequenceData, 4, l_motionSequenceData, 4, l_evtList, l_resNameList);

    mType = getType();
    mFlowNodeNo = getFlowNodeNo();
    mTwilight = false;

    cPhs__Step phase = (cPhs__Step)loadRes(l_loadResPtrnList[mType], (const char**)l_resNameList);
    if (phase == cPhs_COMPLEATE_e) {
        OS_REPORT("\t(%s:%d) flowNo:%d, PathID:%02x<%08x> ", fopAcM_getProcNameString(this), mType, mFlowNodeNo, (getPathID() >> 32) & 0xFF, fopAcM_GetParam(this));

        if (isDelete()) {
            OS_REPORT("===>isDelete:TRUE\n");
            return cPhs_ERROR_e;
        }

        OS_REPORT("\n");

        static int const heapSize[6] = {
            0x3D50, 0x3D50, 0x3D50, 0x3D50,
            0x3D50, 0x0,
        };

        if (!fopAcM_entrySolidHeap(this, createHeapCallBack, heapSize[mType])) {
            return cPhs_ERROR_e;
        }

        fopAcM_SetMtx(this, mpMorf[0]->getModel()->getBaseTRMtx());
        fopAcM_setCullSizeBox(this, -200.0f, -100.0f, -200.0f, 200.0f, 300.0f, 200.0f);

        mSound.init(&current.pos, &eyePos, 3, 1);
        field_0x9c0.init(&mAcch, 0.0f, 0.0f);

        reset();
        mAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1, &mAcchCir, fopAcM_GetSpeed_p(this), fopAcM_GetAngle_p(this), fopAcM_GetShapeAngle_p(this));
        mCcStts.Init(daNpc_Aru_Param_c::m.common.weight, 0, this);
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

        if (mType == TYPE_3 &&
            /* dSv_event_flag_c::F_0573 - Ordon Village - 1st Day - Fado moves to the farm (disappears from in front of Link's house) */
            daNpcT_chkEvtBit(573)) {
            return cPhs_ERROR_e;
        }

        mCreating = true;
        Execute();
        mCreating = false;
    }

    return phase;
}

int daNpc_Aru_c::CreateHeap() {
    J3DModelData* mdlData_p = static_cast<J3DModelData*>(dComIfG_getObjectRes(l_resNameList[l_bmdData[0][1]], l_bmdData[0][0]));
    if (mdlData_p == NULL) {
        return 1;
    }

    mpMorf[0] = new mDoExt_McaMorfSO(mdlData_p, NULL, NULL, NULL, -1, 1.0f, 0, -1, &mSound, 0x80000, 0x11020284);
    if (mpMorf[0] == NULL || mpMorf[0]->getModel() == NULL) {
        return 0;
    }

    J3DModel* model = mpMorf[0]->getModel();
    for (u16 i = 0; i < mdlData_p->getJointNum(); i++) {
        mdlData_p->getJointNodePointer(i)->setCallBack(ctrlJointCallBack);
    }
    model->setUserArea((u32)this);

    mpMatAnm[0] = new daNpcT_MatAnm_c();
    if (mpMatAnm[0] == NULL) {
        return 0;
    }

    if (setFaceMotionAnm(1, false) && setMotionAnm(0, 0.0f, FALSE)) {
        return 1;
    }

    return 0;
}

int daNpc_Aru_c::Delete() {
    this->~daNpc_Aru_c();
    return 1;
}

int daNpc_Aru_c::Execute() {
    return execute();
}

int daNpc_Aru_c::Draw() {
    if (mpMatAnm[0] != NULL) {
        J3DModelData* mdlData_p = mpMorf[0]->getModel()->getModelData();
        mdlData_p->getMaterialNodePointer(getEyeballMaterialNo())->setMaterialAnm(mpMatAnm[0]);
    }

    return draw(
#if DEBUG
        chkAction(&daNpc_Aru_c::test),
#else
        FALSE,
#endif
        FALSE, mRealShadowSize, NULL, 100.0f, FALSE, FALSE, FALSE
    );
}

int daNpc_Aru_c::createHeapCallBack(fopAc_ac_c* a_this) {
    return static_cast<daNpc_Aru_c*>(a_this)->CreateHeap();
}

int daNpc_Aru_c::ctrlJointCallBack(J3DJoint* i_joint, int param_2) {
    if (param_2 == 0) {
        daNpc_Aru_c* i_this = (daNpc_Aru_c*)j3dSys.getModel()->getUserArea();
        if (i_this != NULL) {
            i_this->ctrlJoint(i_joint, j3dSys.getModel());
        }
    }

    return 1;
}

void* daNpc_Aru_c::srchCow(void* i_actor, void* i_data) {
    if (mFindCount < 50) {
        fopAc_ac_c* actor_p = (fopAc_ac_c*)i_actor;
        if (actor_p != NULL && actor_p != i_data) {
            if (fopAcM_IsExecuting(fopAcM_GetID(actor_p)) && fopAcM_GetName(actor_p) == PROC_COW && !((daCow_c*)actor_p)->isGuardFad()) {
                mFindActorPtrs[mFindCount] = actor_p;
                mFindCount++;
            }
        }
    }

    return NULL;
}

void* daNpc_Aru_c::srchUDoor(void* i_actor, void* i_data) {
    if (mFindCount < 50 && i_actor != NULL && i_actor != i_data) {
        if (fopAcM_IsExecuting(fopAcM_GetID(i_actor)) && fopAcM_GetName(i_actor) == PROC_OBJ_UDOOR) {
            mFindActorPtrs[mFindCount] = (fopAc_ac_c*)i_actor;
            mFindCount++;
        }
    }

    return NULL;
}

BOOL daNpc_Aru_c::chkThrust(fopAc_ac_c* actor_p) {
    cM_atan2s(actor_p->speed.x, actor_p->speed.z);
    fopAcM_searchActorAngleY(actor_p, this);
    f32 actorDistance = fopAcM_searchActorDistance(this, actor_p);

    if (actor_p != NULL) {
        if (cM3d_IsZero(actor_p->speedF) <= 0 && (cM3d_IsZero(actor_p->speed.x) <= 0 || cM3d_IsZero(actor_p->speed.z) <= 0)) {
            f32 fVar2 = fopAcM_searchActorDistance(this, actor_p);
            s16 sVar1 = cM_atan2s(actor_p->speed.x, actor_p->speed.z) - fopAcM_searchActorAngleY(actor_p, this);

            if (fVar2 < daNpc_Aru_Param_c::m.warning_range) {
                if (abs(sVar1) < cM_deg2s(35.0f)) {
                    return TRUE;
                }
            }
        }

        if (fopAcM_GetName(actor_p) == PROC_ALINK && daPy_py_c::checkNowWolf() && actorDistance < daNpc_Aru_Param_c::m.warning_range) {
            return TRUE;
        }
    }

    return FALSE;
}

fopAc_ac_c* daNpc_Aru_c::getCowP(int param_1) {
    mFindCount = 0;

    if (daPy_py_c::checkNowWolf()) {
        mFindActorPtrs[mFindCount] = daPy_getPlayerActorClass();

        if (mFindActorPtrs[mFindCount] != NULL) {
            mFindCount++;
        }
    }

    if (param_1 != 0 && dComIfGp_getHorseActor() != NULL) {
        mFindActorPtrs[mFindCount] = (fopAc_ac_c*)dComIfGp_getHorseActor();

        if (mFindActorPtrs[mFindCount] != NULL) {
            mFindCount++;
        }
    }

    field_0xfd2 = false;
    fopAcM_Search(srchCow, this);

    if (field_0xfd2) {
        return NULL;
    }

    for (int i = 0; i < mFindCount; i++) {
        if (chkThrust(mFindActorPtrs[i])) {
            return mFindActorPtrs[i];
        }
    }

    return NULL;
}

fopAc_ac_c* daNpc_Aru_c::getUDoor_l_P() {
    fopAc_ac_c* rv = NULL;
    f32 fVar1 = G_CM3D_F_INF;
    
    mFindCount = 0;
    fopAcM_Search(srchUDoor, this);

    for (int i = 0; i < mFindCount; i++) {
        if (((obj_udoor_class*)mFindActorPtrs[i])->field_0x570 == 1 && fopAcM_searchActorDistance(this, mFindActorPtrs[i]) < fVar1) {
            fVar1 = fopAcM_searchActorDistance(this, mFindActorPtrs[i]);
            rv = mFindActorPtrs[i];
        }
    }

    return rv;
}

fopAc_ac_c* daNpc_Aru_c::getUDoor_r_P() {
    fopAc_ac_c* rv = NULL;
    f32 fVar1 = G_CM3D_F_INF;
    
    mFindCount = 0;
    fopAcM_Search(srchUDoor, this);

    for (int i = 0; i < mFindCount; i++) {
        if (((obj_udoor_class*)mFindActorPtrs[i])->field_0x570 == 0 && fopAcM_searchActorDistance(this, mFindActorPtrs[i]) < fVar1) {
            fVar1 = fopAcM_searchActorDistance(this, mFindActorPtrs[i]);
            rv = mFindActorPtrs[i];
        }
    }

    return rv;
}

u8 daNpc_Aru_c::getType() {
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

        default:
            return 5;
    }
}

BOOL daNpc_Aru_c::isDelete() {
    switch (mType) {
        case TYPE_0:
            /* dSv_event_flag_c::M_002 - Cutscene - [cutscene: 2] Met with Ilia (brings horse to spring) */
            return daNpcT_chkEvtBit(169);

        case TYPE_1:
            return FALSE;
        
        case TYPE_2:
            /* dSv_event_flag_c::M_013 - Misc. - First heard about Twilight gate from Midna */
            return daNpcT_chkEvtBit(45) == FALSE;

        case TYPE_3:
            /* dSv_event_flag_c::F_0573 - Ordon Village - 1st Day - Fado moves to the farm (disappears from in front of Link's house) */
            return daNpcT_chkEvtBit(573);

        case TYPE_4:
            return FALSE;

        default:
            return FALSE;
    }
}

void daNpc_Aru_c::reset() {
    csXyz angle;
    int iVar1 = (u8*)&field_0xfd4 - (u8*)&mNextAction;

    if (mpMatAnm[0] != NULL) {
        mpMatAnm[0]->initialize();
    }

    initialize();

    for (int i = 0; i < 4; i++) {
        mActorMngrs[i].initialize();
    }

    memset(&mNextAction, 0, iVar1);
    angle.setall(0);
    angle.y = home.angle.y;

    switch (mType) {
        case TYPE_0:
            mTimerID = fpcM_ERROR_PROCESS_ID_e;
            mPlayerActorMngr.entry(daPy_getPlayerActorClass());
            mEventTimer = 30;
            break;
        
        case TYPE_2:
            mTimerID = fpcM_ERROR_PROCESS_ID_e;
            mPlayerActorMngr.entry(daPy_getPlayerActorClass());
            mEventTimer = 30;
            break;

        case TYPE_3:
            /* dSv_event_flag_c::F_0573 - Ordon Village - 1st Day - Fado moves to the farm (disappears from in front of Link's house) */
            if (!daNpcT_chkEvtBit(573)) {
                if (daPy_getPlayerActorClass()->checkHorseStart()) {
                    daNpcT_onEvtBit(573);
                }
            }
            break;
            
        case TYPE_4:
            mTimerID = fpcM_ERROR_PROCESS_ID_e;
            mEventTimer = 30;
            break;
    }

    if (!chkBullRunningStage()) {
        if (!chkSkipFenceStage()) {
            if ((strcmp(dComIfGp_getStartStageName(), "F_SP00") != 0 || (dComIfG_play_c::getLayerNo(0) != 2 && dComIfG_play_c::getLayerNo(0) != 13)) &&
                /* dSv_event_flag_c::M_002 - Cutscene - [cutscene: 2] Met with Ilia (brings horse to spring) */
                !daNpcT_chkEvtBit(169)) {
                daNpcT_offTmpBit(91); // dSv_event_tmp_flag_c::T_0091 - Ordon Ranch - Goat herding in second half (for day 1 check)
                daNpcT_offTmpBit(151); // dSv_event_tmp_flag_c::CAT_1 - Ordon Village - 3rd day: Heard results of goat herding
                daNpcT_offTmpBit(152); // dSv_event_tmp_flag_c::CAT_2 - Ordon Village - 3rd day: Spoke with Fado after herding goats
                daNpcT_offTmpBit(153); // dSv_event_tmp_flag_c::CAT_3 - Ordon Village - 3rd day: Fado's first conversation
                daNpcT_offTmpBit(179); // dSv_event_tmp_flag_c::T_0179 - Ordon Ranch - 3rd day - Goat herding START
                daNpcT_offTmpBit(180); // dSv_event_tmp_flag_c::T_0180 - Ordon Ranch - 3rd day - Goat herding END

                /* dSv_event_flag_c::F_0700 - N/A - First day ends */
                if (!daNpcT_chkEvtBit(603)) {
                    daNpcT_offTmpBit(175); // dSv_event_tmp_flag_c::T_0175 - Ordon Ranch - 1st day: Start Fado's goat herding
                    daNpcT_offTmpBit(176); // dSv_event_tmp_flag_c::T_0176 - Ordon Ranch - 1st day: Finish Fado's goat herding
                    daNpcT_offTmpBit(177); // dSv_event_tmp_flag_c::T_0177 - Ordon Ranch - 1st day: Asked to jump gate by Fado
                    daNpcT_offTmpBit(178); // dSv_event_tmp_flag_c::T_0178 - Ordon Ranch - 1st day: Spoke with Fado while jumping over gate
                }
            }
        }
    } else {
        angle.y = fopAcM_searchPlayerAngleY(this);
    }

    setAngle(angle);
}

void daNpc_Aru_c::afterJntAnm(int param_1) {
    if (param_1 == 1) {
        mDoMtx_stack_c::YrotM(mStagger.getAngleZ(1));
        mDoMtx_stack_c::ZrotM(-mStagger.getAngleX(1));
    } else if (param_1 == 4) {
        mDoMtx_stack_c::YrotM(mStagger.getAngleZ(0));
        mDoMtx_stack_c::ZrotM(mStagger.getAngleX(0));
    }
}

void daNpc_Aru_c::setParam() {
    selectAction();
    srchActors();
    u32 attnFlag = fopAc_AttnFlag_SPEAK_e | fopAc_AttnFlag_TALK_e;
    s16 talk_distance = daNpc_Aru_Param_c::m.common.talk_distance;
    s16 talk_angle = daNpc_Aru_Param_c::m.common.talk_angle;
    s16 attention_distance = daNpc_Aru_Param_c::m.common.attention_distance;
    s16 attention_angle = daNpc_Aru_Param_c::m.common.attention_angle;

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

    if (daPy_py_c::checkNowWolf()) {
        attnFlag = fopAc_AttnFlag_LOCK_e;
    }

    attention_info.flags = attnFlag;

    scale.set(daNpc_Aru_Param_c::m.common.scale, daNpc_Aru_Param_c::m.common.scale, daNpc_Aru_Param_c::m.common.scale);
    mCcStts.SetWeight(daNpc_Aru_Param_c::m.common.weight);
    mCylH = daNpc_Aru_Param_c::m.common.height;
    mWallR = daNpc_Aru_Param_c::m.common.width;
    mAttnFovY = daNpc_Aru_Param_c::m.common.fov;
    mAcchCir.SetWallR(mWallR);
    mAcchCir.SetWallH(daNpc_Aru_Param_c::m.common.knee_length);
    mRealShadowSize = daNpc_Aru_Param_c::m.common.real_shadow_size;
    mExpressionMorfFrame = daNpc_Aru_Param_c::m.common.expression_morf_frame;
    mMorfFrames = daNpc_Aru_Param_c::m.common.morf_frame;
    gravity = daNpc_Aru_Param_c::m.common.gravity;
}

BOOL daNpc_Aru_c::checkChangeEvt() {
    if (!chkAction(&daNpc_Aru_c::talk)) {
        mPreItemNo = 0;

        if (dComIfGp_event_chkTalkXY()) {
            if (dComIfGp_evmng_ChkPresentEnd()) {
                mEvtNo = EVT_NO_RESPONSE;
                evtChange();
            }

            return TRUE;
        }
    }

    return FALSE;
}

void daNpc_Aru_c::setAfterTalkMotion() {
    int i_index = FACE_NONE;

    switch (mFaceMotionSeqMngr.getNo()) {
        case FACE_NORMAL:
            i_index = FACE_H_NORMAL;
            break;
        
        case FACE_GRUMPY:
            i_index = FACE_H_GRUMPY;
            break;

        case FACE_WORRY:
            i_index = FACE_H_WORRY;
            break;
        
        case FACE_BITTERSMILE:
            i_index = FACE_H_BITTERSMILE;
            break;

        case FACE_SMILE:
            i_index = FACE_H_SMILE;
            break;

        case FACE_SERIOUS:
            i_index = FACE_H_SERIOUS;
            break;
    }

    mFaceMotionSeqMngr.setNo(i_index, -1.0f, FALSE, 0);
}

void daNpc_Aru_c::srchActors() {
    switch (mType) {
        case TYPE_0:
            if (chkBullRunningStage() && mTimerID == fpcM_ERROR_PROCESS_ID_e) {
                mTimerID = dTimer_createTimer(2, 0x989298, 3, 0, 210.0f, 410.0f, 32.0f, 419.0f);
            }

            if (mActorMngrs[0].getActorP() == NULL) {
                mActorMngrs[0].entry(getUDoor_l_P());
            }

            if (mActorMngrs[1].getActorP() == NULL) {
                mActorMngrs[1].entry(getUDoor_r_P());
            }
            break;
        
        case TYPE_2:
            if (chkBullRunningStage() && mTimerID == fpcM_ERROR_PROCESS_ID_e) {
                mTimerID = dTimer_createTimer(2, 0x989298, 3, 0, 210.0f, 410.0f, 32.0f, 419.0f);
            }

            if (mActorMngrs[0].getActorP() == NULL) {
                mActorMngrs[0].entry(getUDoor_l_P());
            }

            if (mActorMngrs[1].getActorP() == NULL) {
                mActorMngrs[1].entry(getUDoor_r_P());
            }
            break;

        case TYPE_3:
            if (mActorMngrs[3].getActorP() == NULL) {
                mActorMngrs[3].entry(getEvtAreaTagP(23, 0));
            }
            break;

        case TYPE_4:
            if (chkBullRunningStage() && mTimerID == fpcM_ERROR_PROCESS_ID_e) {
                mTimerID = dTimer_createTimer(2, 0x989298, 2, 0, 210.0f, 410.0f, 32.0f, 419.0f);
            }

            if (mActorMngrs[0].getActorP() == NULL) {
                mActorMngrs[0].entry(getUDoor_l_P());
            }

            if (mActorMngrs[1].getActorP() == NULL) {
                mActorMngrs[1].entry(getUDoor_r_P());
            }

            if (mActorMngrs[3].getActorP() == NULL) {
                mActorMngrs[3].entry(getEvtAreaTagP(25, 0));
            }
            break;
    }
}

BOOL daNpc_Aru_c::evtTalk() {
    if (chkAction(&daNpc_Aru_c::talk)) {
        (this->*mAction)(NULL);
    } else {
        setAction(&daNpc_Aru_c::talk);
    }

    return TRUE;
}

BOOL daNpc_Aru_c::evtCutProc() {
    int staffId = dComIfGp_getEventManager().getMyStaffId("Aru", this, -1);

    if (staffId != -1) {
        mStaffId = staffId;
        int actIdx = dComIfGp_getEventManager().getMyActIdx(mStaffId, mCutNameList, 7, 0, 0);

        if ((this->*mCutList[actIdx])(mStaffId)) {
            dComIfGp_getEventManager().cutEnd(mStaffId);
        }

        return TRUE;
    }

    return FALSE;
}

void daNpc_Aru_c::action() {
    fopAc_ac_c* actor_p = hitChk(&mCyl, 0xFFFFFFFF);

    if (actor_p != NULL && mCyl.GetTgHitObj()->ChkAtType(AT_TYPE_THROW_OBJ)) {
        mStagger.setParam(this, actor_p, mCurAngle.y);
        setDamage(0, 12, 0);
        mDamageTimerStart = 0;
        mJntAnm.lookNone(1);
        field_0xfc4 = 0;
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

void daNpc_Aru_c::beforeMove() {
    fopAcM_OffStatus(this, fopAcM_STATUS_UNK_0x8000000);

    if (checkHide()) {
        fopAcM_OnStatus(this, fopAcM_STATUS_UNK_0x8000000);
    }

    if (checkHide() || mNoDraw) {
        attention_info.flags = 0;
    }
}

static daNpc_Aru_Param_c l_HIO;

void daNpc_Aru_c::setAttnPos() {
    cXyz sp40(-30.0f, 10.0f, 0.0f);
    cXyz sp4c(0.0f, 10.0f, 0.0f);

    mStagger.calc(FALSE);
    f32 rad = cM_s2rad((s16)(mCurAngle.y - field_0xd7e.y));

    mJntAnm.setParam(this, mpMorf[0]->getModel(), &sp40, getBackboneJointNo(), getNeckJointNo(), getHeadJointNo(),
                     daNpc_Aru_Param_c::m.common.body_angleX_min, daNpc_Aru_Param_c::m.common.body_angleX_max,
                     daNpc_Aru_Param_c::m.common.body_angleY_min, daNpc_Aru_Param_c::m.common.body_angleY_max,
                     daNpc_Aru_Param_c::m.common.head_angleX_min, daNpc_Aru_Param_c::m.common.head_angleX_max,
                     daNpc_Aru_Param_c::m.common.head_angleY_min, daNpc_Aru_Param_c::m.common.head_angleY_max,
                     daNpc_Aru_Param_c::m.common.neck_rotation_ratio, rad, &sp4c);
    mJntAnm.calcJntRad(0.2f, 1.0f, rad);
    
    setMtx();
    mDoMtx_stack_c::copy(mpMorf[0]->getModel()->getAnmMtx(getHeadJointNo()));
    mDoMtx_stack_c::multVec(&sp40, &eyePos);
    mJntAnm.setEyeAngleX(eyePos, 1.0f, 0);
    mJntAnm.setEyeAngleY(eyePos, mCurAngle.y + field_0xd8a.y, TRUE, 1.0f, 0);

    sp40.set(0.0f, 0.0f, 20.0f);
    sp40.y = daNpc_Aru_Param_c::m.common.attention_offset;
    mDoMtx_stack_c::YrotS(mCurAngle.y);
    mDoMtx_stack_c::multVec(&sp40, &sp40);
    attention_info.position = current.pos + sp40;

    static cXyz prtclScl(1.0f, 1.0f, 1.0f);
    setFootPos();
    setFootPrtcl(&prtclScl, 0.0f, 0.0f);
}

void daNpc_Aru_c::setCollision() {
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

        mCyl.SetCoSPrm(0x79);
        mCyl.SetTgType(tgType);
        mCyl.SetTgSPrm(tgSPrm);
        mCyl.OnTgNoHitMark();

        pos.set(0.0f, 0.0f, 0.0f);
        f32 cylH = mCylH;
        f32 wallR = mWallR;

        mDoMtx_stack_c::YrotS(mCurAngle.y);
        mDoMtx_stack_c::multVec(&pos, &pos);
        pos += current.pos;
        mCyl.SetH(cylH);
        mCyl.SetR(wallR);
        mCyl.SetC(pos);
        dComIfG_Ccsp()->Set(&mCyl);
    }

    mCyl.ClrCoHit();
    mCyl.ClrTgHit();
}

int daNpc_Aru_c::drawDbgInfo() {
    return 0;
}

BOOL daNpc_Aru_c::selectAction() {
    mNextAction = NULL;

    switch (mType) {
        case TYPE_0:
            if (chkSkipFenceStage()) {
                mNextAction = &daNpc_Aru_c::skipFence;
                break;
            }

            if (chkBullRunningStage()) {
                mNextAction = &daNpc_Aru_c::bullRunning;
                break;
            }

            mNextAction = &daNpc_Aru_c::wait;
            break;
        
        case TYPE_2:
            if (chkBullRunningStage()) {
                mNextAction = &daNpc_Aru_c::bullRunning;
            } else {
                mNextAction = &daNpc_Aru_c::wait;
            }
            break;

        case TYPE_4:
            if (chkSkipFenceStage()) {
                mNextAction = &daNpc_Aru_c::skipFence;
                break;
            }

            if (chkBullRunningStage()) {
                mNextAction = &daNpc_Aru_c::bullRunning;
                break;
            }

            mNextAction = &daNpc_Aru_c::wait;
            break;

        default:
            mNextAction = &daNpc_Aru_c::wait;
            break;
    }

    return TRUE;
}

BOOL daNpc_Aru_c::chkAction(actionFunc action) {
    return mAction == action;
}

BOOL daNpc_Aru_c::setAction(actionFunc action) {
    mMode = MODE_EXIT;

    if (mAction) {
        (this->*mAction)(NULL);
    }

    mMode = MODE_ENTER;

    mAction = action;
    if (mAction) {
        (this->*mAction)(NULL);
    }

    return 1;
}

BOOL daNpc_Aru_c::chkBullRunningStage() {
    BOOL rv;

    if (strcmp(dComIfGp_getStartStageName(), "F_SP00") == 0) {
        rv = dComIfG_play_c::getLayerNo(0) == 4 || dComIfG_play_c::getLayerNo(0) == 5;
    } else {
        rv = FALSE;
    }

    return rv;
}

BOOL daNpc_Aru_c::chkSkipFenceStage() {
    BOOL rv;

    if (strcmp(dComIfGp_getStartStageName(), "F_SP00") == 0) {
        rv = dComIfG_play_c::getLayerNo(0) == 6 || dComIfG_play_c::getLayerNo(0) == 7;
    } else {
        rv = FALSE;
    }

    return rv;
}

s16 daNpc_Aru_c::srchActorDirection(fopAc_ac_c* actor_p) {
    s16 rv;
    s16 sVar1 = fopAcM_searchActorAngleY(this, actor_p) - mCurAngle.y;
    s16 sVar2 = abs(sVar1);
    s16 sVar3 = cM_deg2s(daNpc_Aru_Param_c::m.forward_visibility);
    
    if (sVar2 < sVar3) {
        if (sVar1 > 0) {
            rv = 7;
        } else {
            rv = 1;
        }
    } else if ((s16)(0x8000 - sVar3) < sVar2) {
        if (sVar1 > 0) {
            rv = 5;
        } else {
            rv = 3;
        }
    } else if (sVar1 > 0) {
        rv = 6;
    } else {
        rv = 2;
    }

    return (s16)rv;
}

void daNpc_Aru_c::adjustMoveDir() {
    cM3dGPla planes[3];
    cXyz sp84, sp90, sp9c;
    s16 sVar1[3] = {0, -0x2000, 0x2000};
    bool bVar1[3];
    f32 fVar1[3];
    s16 sVar3[3];

    sp84 = current.pos;
    sp84.y += daNpc_Aru_Param_c::m.common.knee_length;

    for (int i = 0; i < 3; i++) {
        bVar1[i] = 0;
        sp90 = sp84;

        if (i != 0) {
            sp90.x += cM_ssin(mCurAngle.y + sVar1[i]) * 280.0f;
            sp90.z += cM_scos(mCurAngle.y + sVar1[i]) * 280.0f;
        } else {
            sp90.x += cM_ssin(mCurAngle.y + sVar1[i]) * 400.0f;
            sp90.z += cM_scos(mCurAngle.y + sVar1[i]) * 400.0f;
        }

        mLinChk.Set(&sp84, &sp90, this);

        if (dComIfG_Bgsp().LineCross(&mLinChk)) {
            bVar1[i] = dComIfG_Bgsp().GetTriPla(mLinChk, &planes[i]);
            f32 fVar2 = cM_ssin(0x6000);
            f32 fVar3 = std::abs(planes[i].mNormal.y);

            if (fVar3 <= fVar2) {
                sp9c = current.pos - mLinChk.GetCross();
                fVar1[i] = sp9c.absXZ();
                sVar3[i] = cM_atan2s(planes[i].mNormal.x, planes[i].mNormal.z);
            } else {
                bVar1[i] = 0;
            }
        }
    }

    if (bVar1[1] != 0 && bVar1[2] != 0) {
        mTimer = daNpc_Aru_Param_c::m.no_turn_time;

        switch (field_0xfca) {
            case 1:
            case 2:
                field_0xfca = 2;
                break;

            case 6:
            case 7:
                field_0xfca = 6;
                break;

            default:
                if (mActorDirection == 1 || mActorDirection == 2 || mActorDirection == 3) {
                    field_0xfca = 2;
                } else {
                    field_0xfca = 6;
                }
                break;
        }
    } else if (bVar1[0] != 0) {
        if (bVar1[1] != 0) {
            if (mActorDirection == 5 || mActorDirection == 3) {
                field_0xfca = 7;
            } else {
                field_0xfca = 6;
            }
        } else if (bVar1[2] != 0) {
            if (mActorDirection == 5 || mActorDirection == 3) {
                field_0xfca = 1;
            } else {
                field_0xfca = 2;
            }
        } else {
            s16 sVar4 = sVar3[0] - mCurAngle.y;

            if (abs(sVar4) > 0x7800) {
                if (mActorDirection == 1 || mActorDirection == 2 || mActorDirection == 3) {
                    field_0xfca = 6;
                } else {
                    field_0xfca = 2;
                }
            } else if (sVar4 > 0) {
                field_0xfca = 6;
            } else {
                field_0xfca = 2;
            }
        }
    } else if (bVar1[1]) {
        switch (mActorDirection) {
            case 1:
            case 7:
                field_0xfca = 6;
                mTimer = daNpc_Aru_Param_c::m.no_turn_time;
                break;
            
            case 2:
                field_0xfca = 7;
                break;

            default:
                field_0xfca = 0;

                if (fVar1[1] < 200.0f) {
                    field_0xfca = 7;
                }
                break;
        }
    } else if (bVar1[2]) {
        switch (mActorDirection) {
            case 1:
            case 7:
                field_0xfca = 2;
                mTimer = daNpc_Aru_Param_c::m.no_turn_time;
                break;

            case 6:
                field_0xfca = 1;
                break;

            default:
                field_0xfca = 0;

                if (fVar1[2] < 200.0f) {
                    field_0xfca = 1;
                }
                break;
        }
    } else if (cLib_calcTimer(&mTimer) == 0) {
        switch (mActorDirection) {
            case 7:
                field_0xfca = 2;
                break;
                
            case 1:
                field_0xfca = 6;
                break;
            
            case 6:
                field_0xfca = 1;
                break;

            case 2:
                field_0xfca = 7;
                break;

            default:
                field_0xfca = 0;
                break;
        }
    }
}

int daNpc_Aru_c::duck(int param_1) {
    fopAc_ac_c* cow_p;
    int iVar1 = daNpc_Aru_Param_c::m.avoid_time;
    cow_p = getCowP(param_1);

    if (cow_p != NULL) {
        if (field_0xfc4 == 0) {
            mFaceMotionSeqMngr.setNo(FACE_NONE, -1.0f, FALSE, 0);
            mMotionSeqMngr.setNo(MOT_RUN, -1.0f, FALSE, 0);
        }

        field_0xfc4 = cLib_getRndValue(iVar1 * 0.5f, iVar1 * 1.5f);
        if (mActorMngrs[2].getActorP() == NULL || (mActorMngrs[2].getActorP() != NULL && cow_p != mActorMngrs[2].getActorP())) {
            mActorMngrs[2].entry(cow_p);
        }
    }

    if (field_0xfc4 != 0) {
        if (cLib_calcTimer(&field_0xfc4) != 0) {
            cow_p = mActorMngrs[2].getActorP();
            if (cow_p != NULL) {
                mActorDirection = srchActorDirection(cow_p);
            }

            adjustMoveDir();
            s16 sVar1 = current.angle.y;

            switch (field_0xfca) {
                case 1:
                    sVar1 -= 0x1000;
                    break;

                case 7:
                    sVar1 += 0x1000;
                    break;
                
                case 2:
                    sVar1 -= 0x4000;
                    break;
                
                case 6:
                    sVar1 += 0x4000;
                    break;
            }

            mJntAnm.lookActor(cow_p, 0.0f, 0);
            cLib_addCalcAngleS2(&current.angle.y, sVar1, MREG_S(0) + 6, MREG_S(1) + 0x800);
            shape_angle.y = current.angle.y;
            mCurAngle.y = shape_angle.y;
            cLib_chaseF(&speedF, daNpc_Aru_Param_c::m.run_speed, 1.5f);
            speedF *= cM_scos(mGroundAngle);

            if (mGroundAngle < 0) {
                speedF *= 0.85f;
            }

            mCcStts.SetWeight(37);
            return 1;
        }

        mFaceMotionSeqMngr.setNo(FACE_NONE, -1.0f, FALSE, 0);
        mMotionSeqMngr.setNo(MOT_WAIT_A, -1.0f, FALSE, 0);
        speedF = 0.0f;
    }

    return 0;
}

int daNpc_Aru_c::lookround(s16 param_1) {
    if (field_0xfd1) {
        cLib_addCalcAngleS2(&current.angle.y, param_1, MREG_S(0) + 4, MREG_S(1) + 0x800);
        shape_angle.y = current.angle.y;
        mCurAngle.y = shape_angle.y;

        if (mMotionSeqMngr.getNo() != MOT_STEP_R) {
            mMotionSeqMngr.setNo(MOT_STEP_R, -1.0f, FALSE, 0);
        } else if (mMotionSeqMngr.checkEndSequence()) {
            if (cM_deg2s(4.0f) >= (s16)abs((s16)(param_1 - mCurAngle.y))) {
                mMotionSeqMngr.setNo(MOT_WAIT_A, -1.0f, FALSE, 0);
                field_0xfd1 = false;
            } else {
                mMotionSeqMngr.setNo(MOT_STEP_R, 0.0f, TRUE, 0);
            }
        }
    } else if (cM_deg2s(90.0f) < (s16)abs((s16)(param_1 - mCurAngle.y))) {
        field_0xfd1 = true;
    }

    return 1;
}

int daNpc_Aru_c::cutRideOnHorse(int i_staffID) {
    int rv = 0;
    int prm = -1;
    int* piVar1 = dComIfGp_evmng_getMyIntegerP(i_staffID, "prm");
    if (piVar1 != NULL) {
        prm = *piVar1;
    }

    if (dComIfGp_getEventManager().getIsAddvance(i_staffID)) {
        switch (prm) {
            case 0:
                initTalk(mFlowNodeNo, NULL);
                break;
            
            case 1:
                mFaceMotionSeqMngr.setNo(FACE_NONE, -1.0f, FALSE, 0);
                mMotionSeqMngr.setNo(MOT_WAIT_A, -1.0f, FALSE, 0);
                break;
        }
    }

    switch (prm) {
        case 0:
            mJntAnm.lookPlayer(0);

            if (mPlayerAngle == mCurAngle.y) {
                rv = 1;
            } else {
                step(mPlayerAngle, 12, 11, 15, 0);
            }
            break;
        
        case 1:
            if (talkProc(NULL, FALSE, NULL, FALSE) && mFlow.checkEndFlow()) {
                rv = 1;
            }
            break;
    }

    return rv;
}

int daNpc_Aru_c::cutGotoBullRunningStage(int i_staffID) {
    int rv = 0;
    int prm = -1;
    int* piVar1 = dComIfGp_evmng_getMyIntegerP(i_staffID, "prm");
    if (piVar1 != NULL) {
        prm = *piVar1;
    }

    if (dComIfGp_getEventManager().getIsAddvance(i_staffID)) {
        switch (prm) {
            case 0:
                mPlayerAngle = fopAcM_searchPlayerAngleY(this);

                if (checkStep()) {
                    mStepMode = 0;
                }
                break;
            
            case 1:
                mFaceMotionSeqMngr.setNo(FACE_NONE, -1.0f, FALSE, 0);
                mMotionSeqMngr.setNo(MOT_WAIT_A, -1.0f, FALSE, 0);
                break;
        }
    }

    switch (prm) {
        case 0:
            mJntAnm.lookPlayer(0);

            if (mPlayerAngle == mCurAngle.y) {
                rv = 1;
            } else {
                step(mPlayerAngle, 12, 11, 15, 0);
            }
            break;

        case 1:
            break;
    }

    return rv;
}

int daNpc_Aru_c::cutEndBullRunning(int i_staffID) {
    int rv = 0;
    int prm = -1;
    int* piVar1 = dComIfGp_evmng_getMyIntegerP(i_staffID, "prm");
    if (piVar1 != NULL) {
        prm = *piVar1;
    }

    if (dComIfGp_getEventManager().getIsAddvance(i_staffID)) {
        switch (prm) {
            case 0:
                break;

            case 1:
                mDoAud_subBgmStop();
                break;
            
            case 2:
                if (daNpcT_chkTmpBit(181)) { // dSv_event_tmp_flag_c::T_0181 - Ordon Ranch - Started Goat herding Ⅱ
                    daNpcT_onTmpBit(182); // dSv_event_tmp_flag_c::T_0182 - Ordon Ranch - Finished Goat herding Ⅱ
                }
                break;

            case 3:
                if (daNpcT_chkTmpBit(175)) { // dSv_event_tmp_flag_c::T_0175 - Ordon Ranch - 1st day: Start Fado's goat herding
                    daNpcT_onEvtBit(176); // dSv_event_tmp_flag_c::T_0176 - Ordon Ranch - 1st day: Finish Fado's goat herding
                }
                break;

            case 4:
                if (!mLastGoatIn) {
                    daNpcT_offTmpBit(181); // dSv_event_tmp_flag_c::T_0181 - Ordon Ranch - Started Goat herding Ⅱ
                    daNpcT_offTmpBit(182); // dSv_event_tmp_flag_c::T_0182 - Ordon Ranch - Finished Goat herding Ⅱ
                } else if (daNpcT_chkTmpBit(181)) { // dSv_event_tmp_flag_c::T_0181 - Ordon Ranch - Started Goat herding Ⅱ
                    daNpcT_onTmpBit(182); // dSv_event_tmp_flag_c::T_0182 - Ordon Ranch - Finished Goat herding Ⅱ
                } else if (daNpcT_chkTmpBit(179)) { // dSv_event_tmp_flag_c::T_0179 - Ordon Ranch - 3rd day - Goat herding START
                    daNpcT_onTmpBit(180); // dSv_event_tmp_flag_c::T_0180 - Ordon Ranch - 3rd day - Goat herding END
                } else if (daNpcT_chkTmpBit(175)) { // dSv_event_tmp_flag_c::T_0175 - Ordon Ranch - 1st day: Start Fado's goat herding
                    daNpcT_onTmpBit(176); // dSv_event_tmp_flag_c::T_0176 - Ordon Ranch - 1st day: Finish Fado's goat herding
                }
                break;
        }
    }

    switch (prm) {
        case 0:
            action();
            rv = 1;
            break;

        case 1:
            if (dComIfG_getTimerPtr() != NULL && dComIfG_getTimerMode() == 2 && !dComIfG_TimerDeleteCheck(2)) {
                dComIfG_TimerDeleteRequest(2);
            }

            action();
            rv = 1;
            break;
        
        case 2:
            action();
            rv = 1;
            break;

        case 3:
            action();
            dStage_changeScene(7, 0.0f, 0, fopAcM_GetRoomNo(this), 0, -1);
            break;

        case 4:
            action();

            if (!mLastGoatIn ||
                daNpcT_chkTmpBit(182)) { // dSv_event_tmp_flag_c::T_0182 - Ordon Ranch - Finished Goat herding Ⅱ
                if (mLastGoatIn) {
                    daNpcT_onTmpBit(91); // dSv_event_tmp_flag_c::T_0091 - Ordon Ranch - Goat herding in second half (for day 1 check)
                }

                dStage_changeScene(12, 0.0f, 0, fopAcM_GetRoomNo(this), 0, -1);
            } else if (daNpcT_chkTmpBit(180)) { // dSv_event_tmp_flag_c::T_0180 - Ordon Ranch - 3rd day - Goat herding END
                dStage_changeScene(10, 0.0f, 0, fopAcM_GetRoomNo(this), 0, -1);
            } else if (daNpcT_chkTmpBit(176)) { // dSv_event_tmp_flag_c::T_0176 - Ordon Ranch - 1st day: Finish Fado's goat herding
                dStage_changeScene(7, 0.0f, 0, fopAcM_GetRoomNo(this), 0, -1);
            }
            break;
    }

    return rv;
}

int daNpc_Aru_c::cutGotoSkipFenceStage(int i_staffID) {
    int rv = 0;
    int prm = -1;
    int* piVar1 = dComIfGp_evmng_getMyIntegerP(i_staffID, "prm");
    if (piVar1 != NULL) {
        prm = *piVar1;
    }

    if (dComIfGp_getEventManager().getIsAddvance(i_staffID)) {
        switch (prm) {
            case 0:
                break;
            
            case 1:
                mDoAud_subBgmStop();
                break;
        }
    }

    switch (prm) {
        case 0:
            action();
            rv = 1;
            break;
        
        case 1:
            if (dComIfG_getTimerPtr() != NULL && dComIfG_getTimerMode() == 2) {
                if (!dComIfG_TimerDeleteCheck(2)) {
                    dComIfG_TimerDeleteRequest(2);
                }
            } else {
                rv = 1;
            }

            action();
            break;
    }

    return rv;
}

int daNpc_Aru_c::cutSpeakTo(int i_staffID) {
    int rv = 0;
    int prm = -1;
    int* piVar1 = dComIfGp_evmng_getMyIntegerP(i_staffID, "prm");
    if (piVar1 != NULL) {
        prm = *piVar1;
    }

    if (dComIfGp_getEventManager().getIsAddvance(i_staffID)) {
        switch (prm) {
            case 0:
                mPlayerAngle = fopAcM_searchPlayerAngleY(this);

                if (checkStep()) {
                    mStepMode = 0;
                }
                break;
            
            case 1:
                mFaceMotionSeqMngr.setNo(FACE_NONE, -1.0f, FALSE, 0);
                mMotionSeqMngr.setNo(MOT_WAIT_A, -1.0f, FALSE, 0);
                initTalk(46, NULL);
                break;
        }
    }

    switch (prm) {
        case 0:
            mJntAnm.lookPlayer(0);

            if (mPlayerAngle == mCurAngle.y) {
                rv = 1;
            } else {
                step(mPlayerAngle, 12, 11, 15, 0);
            }
            break;
        
        case 1:
            if (talkProc(NULL, FALSE, NULL, FALSE)) {
                int itemNo;
                int eventId = mFlow.getEventId(&itemNo);
                field_0xfcf = false;

                switch (eventId) {
                    case 1:
                        if (mItemPartnerId == fpcM_ERROR_PROCESS_ID_e) {
                            mItemPartnerId = fopAcM_createItemForPresentDemo(&current.pos, itemNo, 0, -1, -1, NULL, NULL);
                        }

                        if (fopAcM_IsExecuting(mItemPartnerId)) {
                            field_0xfcf = true;
                            mEvtNo = EVT_DEFAULT_GETITEM;
                            evtChange();
                        }
                        break;

                    case 3:
                        field_0xfd0 = true;
                        mEvtNo = EVT_GOTO_SKIPFENCE_STAGE;
                        evtChange();
                        break;

                    default:
                        if (mFlow.checkEndFlow()) {
                            mPlayerActorMngr.entry(daPy_getPlayerActorClass());
                            rv = 1;
                        }
                        break;
                }
            }
            break;
    }

    return rv;
}

int daNpc_Aru_c::cutNoEntrance(int i_staffID) {
    cXyz work;
    csXyz angle;
    int rv = 0;
    int prm = -1;
    int* piVar1 = dComIfGp_evmng_getMyIntegerP(i_staffID, "prm");
    if (piVar1 != NULL) {
        prm = *piVar1;
    }

    if (dComIfGp_getEventManager().getIsAddvance(i_staffID)) {
        switch (prm) {
            case 0: {
                fopAc_ac_c* actor_p = mActorMngrs[3].getActorP();

                JUT_ASSERT(3279, NULL != actor_p);

                work.set(0.0f, 500.0f, actor_p->scale.z + 100.0f);
                angle.y = fopAcM_searchPlayerAngleY(actor_p) - actor_p->current.angle.y;

                if (angle.y > 0x2000) {
                    angle.y = 0x2000;
                } else if (angle.y < -0x2000) {
                    angle.y = -0x2000;
                }

                angle.y += actor_p->current.angle.y;
                mDoMtx_stack_c::YrotS(angle.y);
                mDoMtx_stack_c::multVec(&work, &work);
                work += actor_p->current.pos;
                mGndChk.SetPos(&work);
                work.y = dComIfG_Bgsp().GroundCross(&mGndChk);

                JUT_ASSERT(3298, -G_CM3D_F_INF != work.y);

                angle.y = fopAcM_searchActorAngleY(daPy_getPlayerActorClass(), this);
                daPy_getPlayerActorClass()->setPlayerPosAndAngle(&work, angle.y, 0);
                dComIfGp_evmng_setGoal(&work);
                mFaceMotionSeqMngr.setNo(FACE_NONE, 0.0f, FALSE, 0);
                mMotionSeqMngr.setNo(MOT_WAIT_A, 0.0f, FALSE, 0);
                mJntAnm.lookNone(1);
                setAngle(cLib_targetAngleY(&current.pos, &work));
                break;
            }
            
            case 1:
                initTalk(54, NULL);
                break;
        }
    }

    switch (prm) {
        case 0:
            rv = 1;
            break;
        
        case 1:
            if (talkProc(NULL, FALSE, NULL, FALSE) && mFlow.checkEndFlow()) {
                rv = 1;
            }
            break;
    }

    return rv;
}

int daNpc_Aru_c::wait(void* param_1) {
    fopAc_ac_c* actor_p;

    switch (mMode) {
        case MODE_ENTER:
        case MODE_INIT:
            if (!mStagger.checkStagger()) {
                mFaceMotionSeqMngr.setNo(FACE_NONE, -1.0f, FALSE, 0);
                mMotionSeqMngr.setNo(MOT_WAIT_A, -1.0f, FALSE, 0);
                field_0xfd1 = false;
                mMode = MODE_RUN;
            }
            // fallthrough
        case MODE_RUN:
            switch (mType) {
                case TYPE_0:
                    if (!daNpcT_chkTmpBit(153)) { // dSv_event_tmp_flag_c::CAT_3 - Ordon Village - 3rd day: Fado's first conversation
                        mSpeakEvent = true;
                        field_0xe33 = true;
                    } else if (!daNpcT_chkTmpBit(180) && // dSv_event_tmp_flag_c::T_0180 - Ordon Ranch - 3rd day - Goat herding END
                               !daPy_py_c::checkNowWolf()) {
                        if (daPy_getPlayerActorClass()->checkHorseRide()) {
                            mSpeakEvent = 1;
                        }
                    }
                    break;
                
                case TYPE_2:
                    if (field_0xfcf) {
                        mSpeakEvent = true;
                        field_0xe33 = true;
                    } else if (daNpcT_chkTmpBit(181)) { // dSv_event_tmp_flag_c::T_0181 - Ordon Ranch - Started Goat herding Ⅱ
                        if (daNpcT_chkTmpBit(182)) { // dSv_event_tmp_flag_c::T_0182 - Ordon Ranch - Finished Goat herding Ⅱ
                            mEvtNo = EVT_SPEAKTO;
                            field_0xe33 = true;
                        } else {
                            mEvtNo = EVT_GOTO_BULLRUNNING_STAGE_VER2;
                            field_0xe33 = true;
                        }
                    }
                    break;

                case TYPE_3:
                    actor_p = mActorMngrs[3].getActorP();
                    if (actor_p != NULL) {
                        if (((daTag_EvtArea_c*)actor_p)->chkPointInArea(daPy_getPlayerActorClass()->current.pos)) {
                            mEvtNo = EVT_NO_ENTRANCE;
                        }
                    }
                    break;

                case TYPE_4:
                    if (!daNpcT_chkEvtBit(23)) { // dSv_event_flag_c::F_0019 - Ordon Woods - Spoke with Ilia (Colin is there too) at the spring
                        if (daPy_getPlayerActorClass()->checkHorseRide()) {
                            actor_p = mActorMngrs[3].getActorP();
                            if (actor_p != NULL) {
                                if (((daTag_EvtArea_c*)actor_p)->chkPointInArea(daPy_getPlayerActorClass()->current.pos)) {
                                    mSpeakEvent = true;
                                    field_0xe33 = true;
                                }
                            }
                        }
                    }

                    if (!daNpcT_chkTmpBit(177) && // dSv_event_tmp_flag_c::T_0177 - Ordon Ranch - 1st day: Asked to jump gate by Fado
                        daNpcT_chkTmpBit(175) && // dSv_event_tmp_flag_c::T_0175 - Ordon Ranch - 1st day: Start Fado's goat herding
                        daNpcT_chkTmpBit(176)) { // dSv_event_tmp_flag_c::T_0176 - Ordon Ranch - 1st day: Finish Fado's goat herding
                        mSpeakEvent = true;
                        field_0xe33 = true;
                    }
                    break;
            }

            if (!mStagger.checkStagger()) {
                if (daNpcT_chkEvtBit(169) && // dSv_event_flag_c::M_002 - Cutscene - [cutscene: 2] Met with Ilia (brings horse to spring)
                    duck(0) != 0) {
                    field_0xfd1 = false;

                    if (checkStep()) {
                        mStepMode = 0;
                    }
                } else if (mType == TYPE_0 || mType == TYPE_2 &&
                           daNpcT_chkTmpBit(182)) { // dSv_event_tmp_flag_c::T_0182 - Ordon Ranch - Finished Goat herding Ⅱ
                    mJntAnm.lookPlayer(0);
                    lookround(fopAcM_searchPlayerAngleY(this));
                } else {
                    if (field_0xfcd) {
                        mPlayerActorMngr.remove();
                    }

                    fopAc_ac_c* actor_p = mPlayerActorMngr.getActorP();
                    if (actor_p != NULL) {
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
                                if (step(home.angle.y, 12, 11, 15, 0)) {
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
            }
            break;

        case 3:
            break;
    }

    return 1;
}

int daNpc_Aru_c::bullRunning(void* param_1) {
    switch (mMode) {
        case MODE_ENTER:
        case MODE_INIT:
            if (!mStagger.checkStagger()) {
                mFaceMotionSeqMngr.setNo(FACE_NONE, -1.0f, FALSE, 0);
                mMotionSeqMngr.setNo(MOT_WAIT_A, -1.0f, FALSE, 0);
                field_0xfd1 = false;
                mMode = MODE_RUN;
            }
            // fallthrough
        case MODE_RUN:
            if (dMeter2Info_getNowCount() == (u32)dMeter2Info_getMaxCount() && field_0xfd3) {
                mDoAud_subBgmStart(Z2BGM_MINIGAME_WIN01);

                if (dComIfG_getTimerPtr() != NULL) {
                    dComIfG_TimerEnd(2, -1);
                }

                field_0xfd3 = false;
            }

            if (mLastGoatIn) {
                mEvtNo = EVT_END_BULLRUNNING;
            } else if (mType == TYPE_4 &&
                       !daNpcT_chkTmpBit(175)) { // dSv_event_tmp_flag_c::T_0175 - Ordon Ranch - 1st day: Start Fado's goat herding
                mSpeakEvent = true;
                field_0xe33 = true;
            } else if (dComIfG_getTimerPtr() != NULL && dComIfG_getTimerMode() == 2 && mEventTimer != 0) {
                if (mEventTimer == 30) {
                    mDoAud_subBgmStart(Z2BGM_COWBOY_GAME);
                    dComIfG_TimerStart(2, -1);
                }

                if (cLib_calcTimer(&mEventTimer) == 0) {
                    dComIfG_TimerReStart(2);
                    field_0xfd3 = 1;
                }
            }

            if (!mStagger.checkStagger()) {
                if (duck(1) != 0) {
                    field_0xfd1 = false;

                    if (checkStep()) {
                        mStepMode = 0;
                    }
                } else {
                    mJntAnm.lookPlayer(0);
                    lookround(fopAcM_searchPlayerAngleY(this));
                }
            }
            break;

        case 3:
            break;
    }

    return 1;
}

int daNpc_Aru_c::skipFence(void* param_1) {
    switch (mMode) {
        case MODE_ENTER:
        case MODE_INIT:
            if (!mStagger.checkStagger()) {
                mFaceMotionSeqMngr.setNo(FACE_NONE, -1.0f, FALSE, 0);
                mMotionSeqMngr.setNo(MOT_WAIT_A, -1.0f, FALSE, 0);
                field_0xfd1 = false;
                mMode = MODE_RUN;
            }
            // fallthrough
        case MODE_RUN:
            switch (mType) {
                case TYPE_0:
                    if (daNpcT_chkTmpBit(180) && // dSv_event_tmp_flag_c::T_0180 - Ordon Ranch - 3rd day - Goat herding END
                        !daNpcT_chkTmpBit(151)) { // dSv_event_tmp_flag_c::CAT_1 - Ordon Village - 3rd day: Heard results of goat herding
                        mSpeakEvent = true;
                        field_0xe33 = true;
                    }
                    break;
                
                case TYPE_4:
                    if (daNpcT_chkTmpBit(177) && // dSv_event_tmp_flag_c::T_0177 - Ordon Ranch - 1st day: Asked to jump gate by Fado
                        !daNpcT_chkTmpBit(178)) { // dSv_event_tmp_flag_c::T_0178 - Ordon Ranch - 1st day: Spoke with Fado while jumping over gate
                        mSpeakEvent = true;
                        field_0xe33 = true;
                    }
                    break;
            }

            if (!mStagger.checkStagger()) {
                mJntAnm.lookPlayer(0);
                lookround(fopAcM_searchPlayerAngleY(this));
            }
            break;

        case 3:
            break;
    }

    return 1;
}

int daNpc_Aru_c::talk(void* param_1) {
    switch (mMode) {
        case MODE_ENTER:
        case MODE_INIT:
            if (!mStagger.checkStagger()) {
                daNpcT_offTmpBit(90); // dSv_event_tmp_flag_c::T_0090 - Ordon Ranch - Epona is at the range (used in goat herding game)
                daHorse_c* horse_p = dComIfGp_getHorseActor();
                if (horse_p != NULL && !horse_p->checkHorseCallWait()) {
                    daNpcT_onTmpBit(90); // dSv_event_tmp_flag_c::T_0090 - Ordon Ranch - Epona is at the range (used in goat herding game)
                }

                initTalk(mFlowNodeNo, NULL);
                field_0xfc4 = 0;
                mMode = MODE_RUN;
            }
            // fallthrough
        case MODE_RUN:
            if (!mStagger.checkStagger()) {
                if (mTwilight || mPlayerAngle == mCurAngle.y || field_0xfcd) {
                    if (talkProc(NULL, FALSE, NULL, FALSE)) {
                        int itemNo;
                        int eventId = mFlow.getEventId(&itemNo);
                        field_0xfcf = false;

                        switch (eventId) {
                            case 3:
                                if (mType == TYPE_4) {
                                    dStage_changeScene(8, 0.0f, 0, fopAcM_GetRoomNo(this), 0, -1);
                                } else {
                                    field_0xfd0 = true;
                                    mEvtNo = EVT_GOTO_SKIPFENCE_STAGE;
                                    evtChange();
                                }
                                break;
                            
                            case 23:
                                if (mType == TYPE_0) {
                                    dStage_changeScene(9, 0.0f, 0, fopAcM_GetRoomNo(this), 0, -1);
                                } else {
                                    dStage_changeScene(6, 0.0f, 0, fopAcM_GetRoomNo(this), 0, -1);
                                }
                                break;

                            default:
                                if (mFlow.checkEndFlow()) {
                                    if (chkBullRunningStage() && mFlow.getChoiceNo() == 0) {
                                        mEvtNo = EVT_END_BULLRUNNING;
                                        evtChange();
                                    } else {
                                        mPlayerActorMngr.entry(daPy_getPlayerActorClass());
                                        dComIfGp_event_reset();
                                        mMode = MODE_EXIT;
                                    }
                                }
                        }
                    }

                    mJntAnm.lookPlayer(0);

                    if (mTwilight || field_0xfcd) {
                        mJntAnm.lookNone(0);
                    }
                } else {
                    mJntAnm.lookPlayer(0);
                    step(mPlayerAngle, 12, 11, 15, 0);
                }
            }
            break;

        case 3:
            break;
    }

    return 0;
}

int daNpc_Aru_c::test(void* param_1) {
    switch(mMode) {
    case 0:
    case 1:
        speedF = 0.0f;
        speed.setall(0.0f);
        mMode = 2;
        // fallthrough
    case 2:
        mFaceMotionSeqMngr.setNo(mHIO->param.common.face_expression, -1.0f, 0, 0);
        mMotionSeqMngr.setNo(mHIO->param.common.motion, -1.0f, 0, 0);
        mJntAnm.lookNone(0);
        attention_info.flags = 0;
        break;
    case 3:
        break;
    }
    return 1;
}

static int daNpc_Aru_Create(void* a_this) {
    return static_cast<daNpc_Aru_c*>(a_this)->create();
}

static int daNpc_Aru_Delete(void* a_this) {
    return static_cast<daNpc_Aru_c*>(a_this)->Delete();
}

static int daNpc_Aru_Execute(void* a_this) {
    return static_cast<daNpc_Aru_c*>(a_this)->Execute();
}

static int daNpc_Aru_Draw(void* a_this) {
    return static_cast<daNpc_Aru_c*>(a_this)->Draw();
}

static int daNpc_Aru_IsDelete(void* a_this) {
    return 1;
}

AUDIO_INSTANCES;

static actor_method_class daNpc_Aru_MethodTable = {
    (process_method_func)daNpc_Aru_Create,
    (process_method_func)daNpc_Aru_Delete,
    (process_method_func)daNpc_Aru_Execute,
    (process_method_func)daNpc_Aru_IsDelete,
    (process_method_func)daNpc_Aru_Draw,
};

actor_process_profile_definition g_profile_NPC_ARU = {
  fpcLy_CURRENT_e,        // mLayerID
  7,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_NPC_ARU,           // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(daNpc_Aru_c),    // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  321,                    // mPriority
  &daNpc_Aru_MethodTable, // sub_method
  0x00040107,             // mStatus
  fopAc_NPC_e,            // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};
