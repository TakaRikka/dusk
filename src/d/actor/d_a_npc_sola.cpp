/**
 * @file d_a_npc_sola.cpp
 *
 */

#include "d/dolzel_rel.h"  // IWYU pragma: keep

#include "d/actor/d_a_npc_sola.h"

enum Bans_RES_File_ID {
    /* BCK */
    /* 0x04 */ BCK_SOLA_WAIT_A = 0x4,

    /* BMDE */
    /* 0x07 */ BMDR_SOLA = 0x7,
};
enum RES_Name {
    /* 0x0 */ NONE,
    /* 0x1 */ SOLA,
};

static int l_bmdData[1][2] = {
    {BMDR_SOLA, SOLA},
};

static daNpcT_evtData_c l_evtList[2] = {
    {"", 0},
    {"NO_RESPONSE", 0},
};

static char* l_resNameList[2] = {
    "",
    "solA",
};

static s8 l_loadResPtrn0[2] = {1, -1};

static s8* l_loadResPtrnList[1] = {l_loadResPtrn0};

static daNpcT_faceMotionAnmData_c l_faceMotionAnmData[1] = {
    {-1, J3DFrameCtrl::EMode_NONE, NONE, -1, J3DFrameCtrl::EMode_NONE, NONE, FALSE},
};

static daNpcT_motionAnmData_c l_motionAnmData[1] = {
    {BCK_SOLA_WAIT_A, J3DFrameCtrl::EMode_LOOP, SOLA, -1, J3DFrameCtrl::EMode_NONE, NONE, 0, 0},
};

static daNpcT_MotionSeqMngr_c::sequenceStepData_c l_faceMotionSequenceData[4] = {
    {0, -1, 0},
    {-1, 0, 0},
    {-1, 0, 0},
    {-1, 0, 0},
};

static daNpcT_MotionSeqMngr_c::sequenceStepData_c l_motionSequenceData[4] = {
    {0, -1, 0},
    {-1, 0, 0},
    {-1, 0, 0},
    {-1, 0, 0},
};

char* daNpc_solA_c::mCutNameList[1] = {""};

daNpc_solA_c::cutFunc daNpc_solA_c::mCutList[1] = {
    NULL,
};

daNpc_solA_c::~daNpc_solA_c() {
    if (heap != NULL) {
        mpMorf[0]->stopZelAnime();
    }

    deleteRes(l_loadResPtrnList[field_0xf80], (const char**)l_resNameList);
}

daNpc_solA_HIOParam const daNpc_solA_Param_c::m = {
    220.0f, -3.0f, 1.0f,   400.0f, 255.0f, 200.0f, 35.0f, 30.0f, 0.0f, 0.0f,  10.0f,
    -10.0f, 30.0f, -10.0f, 45.0f,  -45.0f, 0.6f,   12.0f, 3,     6,    5,     6,
    0.0f,   0.0f,  0.0f,   0.0f,   60,     8,      0,     0,     0,    false, false,
    4.0f,   0.0f,  0.0f,   0.0f,   0.0f,   0.0f,   0.0f,  0.0f,
};

int daNpc_solA_c::create() {
    daNpcT_ct(this, daNpc_solA_c, l_faceMotionAnmData, l_motionAnmData, l_faceMotionSequenceData, 4,
              l_motionSequenceData, 4, l_evtList, l_resNameList);

    field_0xf80 = getType();
    mFlowNodeNo = getFlowNodeNo();
    mTwilight = false;

    int rv = loadRes(l_loadResPtrnList[field_0xf80], (const char**)l_resNameList);
    if (rv == cPhs_COMPLEATE_e) {
        if (!fopAcM_entrySolidHeap(this, createHeapCallBack, 0)) {
            return cPhs_ERROR_e;
        }

        if (isDelete()) {
            return cPhs_ERROR_e;
        }

        fopAcM_SetMtx(this, mpMorf[0]->getModel()->getBaseTRMtx());
        fopAcM_setCullSizeBox(this, -300.0f, -50.0f, -300.0f, 300.0f, 450.0f, 300.0f);
        mSound.init(&current.pos, &eyePos, 3, 1);
        mAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1, &mAcchCir,
                  fopAcM_GetSpeed_p(this), fopAcM_GetAngle_p(this), fopAcM_GetShapeAngle_p(this));
        mAcch.CrrPos(dComIfG_Bgsp());
        mGndChk = mAcch.m_gnd;
        mGroundH = mAcch.GetGroundH();
        setEnvTevColor();
        setRoomNo();

        mCcStts.Init(daNpc_solA_Param_c::m.common.weight, 0, this);
        mCyl.Set(mCcDCyl);
        mCyl.SetStts(&mCcStts);
        mCyl.SetTgHitCallback(tgHitCallBack);
        reset();
        mCreating = true;
        Execute();
        mCreating = false;
    }

    return rv;
}

int daNpc_solA_c::CreateHeap() {
    J3DModel* model;
    J3DModelData* mdlData_p;
    enum XXX { x0 = 0, x1 = 1 };
    int bmd_get_idx = mTwilight == 1 ? x0 : x0;
    int res_name_idx = l_bmdData[bmd_get_idx][1];
    int sp_0x2c = l_bmdData[bmd_get_idx][0];

    mdlData_p = (J3DModelData*)dComIfG_getObjectRes(l_resNameList[res_name_idx], sp_0x2c);

    mpMorf[0] = new mDoExt_McaMorfSO(mdlData_p, NULL, NULL, NULL, -1, 1.0f, 0, -1, &mSound, 0x80000,
                                     0x11020284);
    if (mpMorf[0] && mpMorf[0]->getModel() == NULL) {
        mpMorf[0]->stopZelAnime();
        mpMorf[0] = NULL;
    }

    if (mpMorf[0] == NULL) {
        return 0;
    } else {
        model = mpMorf[0]->getModel();
        for (u16 jointNo = 0; jointNo < mdlData_p->getJointNum(); jointNo++) {
            mdlData_p->getJointNodePointer(jointNo)->setCallBack(ctrlJointCallBack);
        }

        model->setUserArea((uintptr_t)this);
        if (setFaceMotionAnm(0, false) && setMotionAnm(0, 0.0f, FALSE)) {
            return 1;
        }
    }
    return 0;
}

int daNpc_solA_c::Delete() {
    fopAcM_GetID(this);
    this->~daNpc_solA_c();
    return 1;
}

int daNpc_solA_c::Execute() {
    execute();
}

void daNpc_solA_c::Draw() {
    if (mpMatAnm[0] != NULL) {
        J3DModelData* mdlData_p = mpMorf[0]->getModel()->getModelData();
        mdlData_p->getMaterialNodePointer(getEyeballMaterialNo())->setMaterialAnm(mpMatAnm[0]);
    }
    draw(FALSE, FALSE, daNpc_solA_Param_c::m.common.real_shadow_size, NULL, 100.0f, FALSE, FALSE,
         FALSE);
    return;
}

BOOL daNpc_solA_c::createHeapCallBack(fopAc_ac_c* a_this) {
    return static_cast<daNpc_solA_c*>(a_this)->CreateHeap();
}

BOOL daNpc_solA_c::ctrlJointCallBack(J3DJoint* i_joint, int param_1) {
    daNpc_solA_c* i_this;
    J3DModel* model;
    if (param_1 == 0) {
        model = j3dSys.getModel();
        i_this = (daNpc_solA_c*)model->getUserArea();
        if (i_this != NULL) {
            i_this->ctrlJoint(i_joint, model);
        }
    }
    return true;
}

bool daNpc_solA_c::getType() {
    return false;
}

int daNpc_solA_c::getFlowNodeNo() {
    u16 nodeNo = home.angle.x;
    if (nodeNo == 0xffff) {
        return -1;
    }
    return nodeNo;
}

int daNpc_solA_c::isDelete() {
    return field_0xf80 ? 1 : 0;
}

void daNpc_solA_c::reset() {
    initialize();
    int size = (u8*)&field_0xf9c - (u8*)&mNextAction;
    memset(&mNextAction, 0, size);

    if (mpMatAnm[0] != NULL) {
        mpMatAnm[0]->initialize();
    }

    setAngle(home.angle.y);
}

void daNpc_solA_c::setParam() {
    selectAction();
    srchActors();

    s16 talk_distance = daNpc_solA_Param_c::m.common.talk_distance;
    s16 talk_angle = daNpc_solA_Param_c::m.common.talk_angle;

    attention_info.distances[fopAc_attn_LOCK_e] =
        daNpcT_getDistTableIdx(daNpc_solA_Param_c::m.common.attention_distance,
                               daNpc_solA_Param_c::m.common.attention_angle);
    attention_info.distances[fopAc_attn_TALK_e] = attention_info.distances[fopAc_attn_LOCK_e];
    attention_info.distances[fopAc_attn_SPEAK_e] =
        daNpcT_getDistTableIdx(talk_distance, talk_angle);
    attention_info.flags = fopAc_AttnFlag_SPEAK_e | fopAc_AttnFlag_TALK_e;

    scale.set(daNpc_solA_Param_c::m.common.scale, daNpc_solA_Param_c::m.common.scale,
              daNpc_solA_Param_c::m.common.scale);
    mAcchCir.SetWallR(daNpc_solA_Param_c::m.common.width);
    mAcchCir.SetWallH(daNpc_solA_Param_c::m.common.knee_length);
    mCcStts.SetWeight(daNpc_solA_Param_c::m.common.weight);
    mCylH = daNpc_solA_Param_c::m.common.height;
    mWallR = daNpc_solA_Param_c::m.common.width;
    gravity = daNpc_solA_Param_c::m.common.gravity;
    mExpressionMorfFrame = daNpc_solA_Param_c::m.common.expression_morf_frame;
    mMorfFrames = daNpc_solA_Param_c::m.common.morf_frame;
}

void daNpc_solA_c::setAfterTalkMotion() {
    mFaceMotionSeqMngr.setNo(0, -1.0f, 0, 0);
}

void daNpc_solA_c::srchActors() {
    /* empty function */
}

BOOL daNpc_solA_c::evtTalk() {
    if (chkAction(&daNpc_solA_c::talk)) {
        (this->*(mAction))(NULL);
    } else {
        mPreItemNo = 0;
        if (dComIfGp_event_chkTalkXY()) {
            if (!dComIfGp_evmng_ChkPresentEnd()) {
                return 1;
            }
            mEvtNo = 1;
            evtChange();
            return 1;
        }
        setAction(&daNpc_solA_c::talk);
    }
    return 1;
}

BOOL daNpc_solA_c::evtCutProc() {
    BOOL rv = 0;
    int staffID = dComIfGp_getEventManager().getMyStaffId("solA", this, -1);
    if (staffID != -1) {
        mStaffId = staffID;
        staffID = dComIfGp_getEventManager().getMyActIdx(
            mStaffId, (char**)&daNpc_solA_c::mCutNameList, 1, 0, 0);
        if ((this->*(mCutList[staffID]))(mStaffId)) {
            dComIfGp_getEventManager().cutEnd(mStaffId);
        }
        rv = 1;
    }

    return rv;
}

void daNpc_solA_c::action() {
    if (mNextAction != NULL) {
        if (mAction == mNextAction) {
            (this->*mAction)(NULL);
        } else {
            setAction(mNextAction);
        }
    }
}

void daNpc_solA_c::beforeMove() {
    if (checkHide() || mNoDraw) {
        attention_info.flags = 0;
    }
}

void daNpc_solA_c::setAttnPos() {
    cXyz local_38(30.0f, 0.0f, 0.0f);
    mJntAnm.setParam(
        this, mpMorf[0]->getModel(), &local_38, getBackboneJointNo(), getNeckJointNo(),
        getHeadJointNo(), daNpc_solA_Param_c::m.common.body_angleX_min,
        daNpc_solA_Param_c::m.common.body_angleX_max, daNpc_solA_Param_c::m.common.body_angleY_min,
        daNpc_solA_Param_c::m.common.body_angleY_max, daNpc_solA_Param_c::m.common.head_angleX_min,
        daNpc_solA_Param_c::m.common.head_angleX_max, daNpc_solA_Param_c::m.common.head_angleY_min,
        daNpc_solA_Param_c::m.common.head_angleY_max,
        daNpc_solA_Param_c::m.common.neck_rotation_ratio, 0.0f, NULL);

    f32 fVar1 = cM_s2rad(mCurAngle.y - field_0xd7e.y);
    mJntAnm.calcJntRad(0.2f, 1.0f, fVar1);
    setMtx();
    mDoMtx_stack_c::copy(mpMorf[0]->getModel()->getAnmMtx(getHeadJointNo()));
    mDoMtx_stack_c::multVec(&local_38, &eyePos);
    mJntAnm.setEyeAngleX(eyePos, 1.0f, 0);
    mJntAnm.setEyeAngleY(eyePos, mCurAngle.y, FALSE, 1.0f, 0);

    attention_info.position = current.pos;
    attention_info.position.y += daNpc_solA_Param_c::m.common.attention_offset;
}

void daNpc_solA_c::setCollision() {
    cXyz pos;
    if (!mHide) {
        int prm = mTwilight == true ? 0x69 : 0x79;
        mCyl.SetCoSPrm(prm);

        pos = current.pos;
        f32 height = daNpc_solA_Param_c::m.common.height;
        f32 width = daNpc_solA_Param_c::m.common.width;
        mCyl.SetH(height);
        mCyl.SetR(width);
        mCyl.SetC(pos);
        dComIfG_Ccsp()->Set(&mCyl);
    }

    mCyl.ClrCoHit();
    mCyl.ClrTgHit();
}

int daNpc_solA_c::drawDbgInfo() {
    return false;
}

void daNpc_solA_c::drawOtherMdl() {
    /* empty function */
}

int daNpc_solA_c::selectAction() {
    mNextAction = NULL;
    mNextAction = &daNpc_solA_c::wait;
    return 1;
}

int daNpc_solA_c::chkAction(int (daNpc_solA_c::*param_0)(void*)) {
    return mAction == param_0;
}

int daNpc_solA_c::setAction(int (daNpc_solA_c::*param_0)(void*)) {
    mMode = MODE_EXIT;
    if (mAction != NULL) {
        (this->*(mAction))(NULL);
    }

    mMode = MODE_ENTER;
    mAction = param_0;
    if (mAction != NULL) {
        (this->*(mAction))(NULL);
    }

    return 1;
}

int daNpc_solA_c::wait(void* param_0) {
    switch (mMode) {
    case MODE_ENTER:
    case MODE_INIT:
        mFaceMotionSeqMngr.setNo(0, -1.0f, FALSE, 0);
        mMotionSeqMngr.setNo(0, -1.0f, FALSE, 0);
        mMode = MODE_RUN;
    case MODE_RUN:
        if (srchPlayerActor()) {
            mJntAnm.lookPlayer(0);
        } else {
            if (home.angle.y != mCurAngle.y) {
                if (step(home.angle.y, -1, -1, 15, 0)) {
                    mMode = MODE_INIT;
                }
            }
            mJntAnm.lookNone(0);
        }
        break;
    case MODE_EXIT:
        break;
    }
    return 1;
}

int daNpc_solA_c::talk(void* param_0) {
    int bVar1 = 0;
    switch (mMode) {
    case MODE_ENTER:
    case MODE_INIT:
        initTalk(mFlowNodeNo, NULL);
        mMode = MODE_RUN;

    case MODE_RUN:
        if (!mTwilight) {
            mJntAnm.lookPlayer(0);
            if (mCurAngle.y == fopAcM_searchPlayerAngleY(this)) {
                bVar1 = TRUE;
            } else {
                if (step(fopAcM_searchPlayerAngleY(this), -1, -1, 15, 0)) {
                    bVar1 = TRUE;
                }
            }
        } else {
            bVar1 = TRUE;
        }

        if (bVar1 && talkProc(NULL, FALSE, NULL, FALSE)) {
            mPlayerActorMngr.entry(daPy_getPlayerActorClass());
            dComIfGp_event_reset();
            mMode = MODE_EXIT;
        }
        break;

    case MODE_EXIT:
        break;
    }
    return 0;
}

static int daNpc_solA_Create(void* param_0) {
    return static_cast<daNpc_solA_c*>(param_0)->create();
}

static int daNpc_solA_Delete(void* param_0) {
    return static_cast<daNpc_solA_c*>(param_0)->Delete();
}

static int daNpc_solA_Execute(void* param_0) {
    static_cast<daNpc_solA_c*>(param_0)->Execute();
}

static void daNpc_solA_Draw(void* param_0) {
    return static_cast<daNpc_solA_c*>(param_0)->Draw();
}

static bool daNpc_solA_IsDelete(void* param_0) {
    return true;
}

static daNpc_solA_Param_c l_HIO;

static actor_method_class daNpc_solA_MethodTable = {
    (process_method_func)daNpc_solA_Create,  (process_method_func)daNpc_solA_Delete,
    (process_method_func)daNpc_solA_Execute, (process_method_func)daNpc_solA_IsDelete,
    (process_method_func)daNpc_solA_Draw,
};

actor_process_profile_definition g_profile_NPC_SOLA = {
    fpcLy_CURRENT_e,          // mLayerID
    7,                        // mListID
    fpcPi_CURRENT_e,          // mListPrio
    PROC_NPC_SOLA,            // mProcName
    &g_fpcLf_Method.base,     // sub_method
    sizeof(daNpc_solA_c),     // mSize
    0,                        // mSizeOther
    0,                        // mParameters
    &g_fopAc_Method.base,     // sub_method
    366,                      // mPriority
    &daNpc_solA_MethodTable,  // sub_method
    0x00044107,               // mStatus
    fopAc_NPC_e,              // mActorType
    fopAc_CULLBOX_CUSTOM_e,   // cullType
};
