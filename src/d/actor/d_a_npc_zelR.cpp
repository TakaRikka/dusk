/**
 * @file d_a_npc_zelR.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_npc_zelR.h"

enum RES_Name {
    /* 0x0 */ NONE,
    /* 0x1 */ ZELRF,
};

static daNpc_GetParam1 l_bmdData[1] = {
    {10, 1}
};

static daNpcT_evtData_c l_evtList[2] = {
    {"", 0},
    {"NO_RESPONSE", 0}
};

static char* l_resNameList[2] = {
    "",
    "zelRf"
};

static s8 l_loadResPtrn0[2] = {
    1, 0xFF
};

static s8* l_loadResPtrnList[2] = {
    l_loadResPtrn0,
    l_loadResPtrn0,
};

static daNpcT_faceMotionAnmData_c l_faceMotionAnmData[2] = {
    {-1, 0, 0, 16, 2, 1, 1},
    {6, 0, 1, 16, 2, 1, 1}
};

static daNpcT_motionAnmData_c l_motionAnmData = {
    7, 2, 1, 13, 0, 1, 1, 0
};

static daNpcT_MotionSeqMngr_c::sequenceStepData_c l_faceMotionSequenceData[8] = {
    {1, -1, 1}, {-1, 0, 0}, 
    {-1, 0, 0}, {-1, 0, 0}, 
    {0, -1, 0}, {-1, 0, 0}, 
    {-1, 0, 0}, {-1, 0, 0}
};

static daNpcT_MotionSeqMngr_c::sequenceStepData_c l_motionSequenceData[4] = {
    {0, -1, 0}, {-1, 0, 0}, 
    {-1, 0, 0}, {-1, 0, 0}
};

char* daNpc_ZelR_c::mCutNameList = "";

daNpc_ZelR_c::EventFn daNpc_ZelR_c::mCutList[1] = {
    NULL
};

daNpc_ZelR_c::~daNpc_ZelR_c() {
    OS_REPORT("|%06d:%x|daNpc_ZelR_c -> デストラクト\n", g_Counter.mCounter0, this);
    if (heap) {
        mpMorf[0]->stopZelAnime();
    }
    deleteRes(l_loadResPtrnList[mType], (char const**)l_resNameList);
}

static daNpc_ZelR_Param_c l_HIO;

daNpc_ZelR_HIOParam const daNpc_ZelR_Param_c::m = {
    190.0f,
    -3.0f,
    1.0f,
    400.0f,
    255.0f,
    170.0f,
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
    6,
    5,
    6,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0x3C,
    8,
    0,
    0,
    0,
    false,
    false,
    4.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
};

int daNpc_ZelR_c::create() {
    daNpcT_ct(this, daNpc_ZelR_c, l_faceMotionAnmData,
        &l_motionAnmData, l_faceMotionSequenceData, 4,
        l_motionSequenceData, 4, l_evtList, l_resNameList);
    mType = getType();
    mFlowNodeNo = getFlowNodeNo();
    mTwilight = false;

    int phaseState = loadRes(l_loadResPtrnList[mType], (const char**)l_resNameList);
    if (phaseState == cPhs_COMPLEATE_e) {
        if (!fopAcM_entrySolidHeap(this, createHeapCallBack, 0x5cd0)) {
            OS_REPORT("===>isDelete:TRUE\n");
            return cPhs_ERROR_e;
        }

        OS_REPORT("\t(%s:%d) flowNo:%d, PathID:%02x<%08x> ", fopAcM_getProcNameString(this), mType,
        mFlowNodeNo, getPath(), fopAcM_GetParam(this));
        if (isDelete()) {
            OS_REPORT("===>isDelete:TRUE\n");
            return cPhs_ERROR_e;
        }

        J3DModelData* modelData = mpMorf[0]->getModel()->getModelData();
        fopAcM_SetMtx(this, mpMorf[0]->getModel()->getBaseTRMtx());
        fopAcM_setCullSizeBox(this, -300.0f, -50.0f, -300.0f, 300.0f, 450.0f, 300.0f);
        fopAcM_OnStatus(this, 0x8000000);

        mSound.init(&current.pos, &eyePos, 3, 1);

        mAcch.Set(&current.pos, &old.pos, this, 1, &mAcchCir, &speed, &current.angle, &shape_angle);
        mAcch.CrrPos(dComIfG_Bgsp());
        mGndChk = mAcch.m_gnd;
        mGroundH = mAcch.m_ground_h;

        setEnvTevColor();
        setRoomNo();

        mCcStts.Init(daNpc_ZelR_Param_c::m.common.weight, 0, this);
        mCyl.Set(mCcDCyl);
        mCyl.SetStts(&mCcStts);
        mCyl.SetTgHitCallback(tgHitCallBack);
        
        reset();
        mCreating = true;
        Execute();
        mCreating = false;
    }

    return phaseState;
}

int daNpc_ZelR_c::CreateHeap() {
    int bmdIdx = mTwilight == true ? NONE : NONE;
    J3DModelData* mdlData_p = (J3DModelData*)(dComIfG_getObjectRes(
        l_resNameList[l_bmdData[bmdIdx].arcIdx], l_bmdData[bmdIdx].fileIdx
    ));
    
    
    JUT_ASSERT(0x1b0, NULL != mdlData_p);

    mpMorf[0] = new mDoExt_McaMorfSO(mdlData_p, NULL, NULL, NULL, -1, 1.0f, 0, -1, &mSound, 0, 0x11020284);
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
    for (int i = 0; i < 2; i++) {
        mpMatAnm[i] = new daNpcT_MatAnm_c();

        if (mpMatAnm[i] == NULL) {
            return 0;
        }
    }

    if (setFaceMotionAnm(1, false) && setMotionAnm(0, 0.0f, 0) != 0) {
        return 1;
    }
    
    return 0;
}

int daNpc_ZelR_c::Delete() {
    fopAcM_GetID(this);
    this->~daNpc_ZelR_c();
    return 1;
}

int daNpc_ZelR_c::Execute() {
    return execute();
}

int daNpc_ZelR_c::Draw() {
    J3DModel* model = mpMorf[0]->getModel();
    J3DModelData* modelData = model->getModelData();

    if (mpMatAnm[0]) {
        modelData->getMaterialNodePointer(getEyeballLMaterialNo())->setMaterialAnm(mpMatAnm[0]);
    }

    if (mpMatAnm[1]) {
        modelData->getMaterialNodePointer(getEyeballRMaterialNo())->setMaterialAnm(mpMatAnm[1]);
    }
    
    return daNpcT_c::draw(0, 1, mRealShadowSize, NULL, 100.0f, 0, 0, 0);
}

int daNpc_ZelR_c::createHeapCallBack(fopAc_ac_c* a_this) {
    daNpc_ZelR_c* i_this = (daNpc_ZelR_c*)a_this;
    return i_this->CreateHeap();

}

int daNpc_ZelR_c::ctrlJointCallBack(J3DJoint* i_joint, int param_2) {
    if (!param_2) {
        daNpc_ZelR_c* i_this = (daNpc_ZelR_c*)j3dSys.getModel()->getUserArea();
        if (i_this) {
            i_this->ctrlJoint(i_joint, j3dSys.getModel());
        }
    }

    return 1;
}

u8 daNpc_ZelR_c::getType() {
    switch (fopAcM_GetParam(this) & 0xff) {
        case 0:
            return TYPE_0;
        default:
            return TYPE_1;
    }
}

u32 daNpc_ZelR_c::getFlowNodeNo() {
    u16 nodeNo = home.angle.x;
    if (nodeNo == 0xffff) {
        return -1;
    }
    return nodeNo;
}

u8 daNpc_ZelR_c::getPath() {
    return (fopAcM_GetParam(this) & 0xff00) >> 8;
}

int daNpc_ZelR_c::isDelete() {
    if (mType == TYPE_1) {
        return 0;
    }
    
    switch (mType) {
        case TYPE_0:
            return daNpcT_chkEvtBit(0x2d) != 0;
        default:
            return 1;
    }
}

void daNpc_ZelR_c::reset() {
    initialize();
    memset(&field_0xf84, 0, ((u8*)&field_0xfc4 - (u8*)&field_0xf84));
    for (int i = 0; i < 2; i++) {
        if (mpMatAnm[i]) {
            mpMatAnm[i]->initialize();
        }
    }

    if (getPath() != 0xff) {
        mPath.initialize();
        mPath.setPathInfo(getPath(), fopAcM_GetRoomNo(this), 0);
    }

    setAngle(home.angle.y);
}

void daNpc_ZelR_c::afterJntAnm(int param_1) {
    if (param_1 == 1) {
        mDoMtx_stack_c::YrotM(mStagger.getAngleZ(1));
        mDoMtx_stack_c::ZrotM(-mStagger.getAngleX(1));
    } else if (param_1 == 3) {
        mDoMtx_stack_c::YrotM(mStagger.getAngleZ(0));
        mDoMtx_stack_c::ZrotM(-mStagger.getAngleX(0));
    }
}

BOOL daNpc_ZelR_c::ctrlBtk() {
    if (mpMatAnm[0] && mpMatAnm[1]) {
        if (field_0xe29 && mBtkAnm.getBtkAnm()) {
            mpMatAnm[0]->setNowOffsetX(cM_ssin(mJntAnm.getEyeAngleY()) * 0.2f * -1.0f);
            mpMatAnm[0]->setNowOffsetY(cM_ssin(mJntAnm.getEyeAngleX()) * 0.2f);

            mpMatAnm[1]->setNowOffsetX(cM_ssin(mJntAnm.getEyeAngleY()) * 0.2f);
            mpMatAnm[1]->setNowOffsetY(cM_ssin(mJntAnm.getEyeAngleX()) * 0.2f);

            mpMatAnm[0]->onEyeMoveFlag();
            mpMatAnm[1]->onEyeMoveFlag();
            return TRUE;
        }

        if (field_0xe2a) {
            mpMatAnm[0]->setMorfFrm(field_0xe2a);
            mpMatAnm[1]->setMorfFrm(field_0xe2a);
            field_0xe2a = 0;
        }

        mpMatAnm[0]->offEyeMoveFlag();
        mpMatAnm[1]->offEyeMoveFlag();

    }

    return FALSE;
}

void daNpc_ZelR_c::setParam() {
    selectAction();
    srchActors();
    s16 sVar1 = l_HIO.m.common.talk_distance;
    s16 sVar2 = l_HIO.m.common.talk_angle;
    s16 sVar3 = l_HIO.m.common.attention_distance;
    s16 sVar4 = l_HIO.m.common.attention_angle;
    attention_info.distances[0] = daNpcT_getDistTableIdx(sVar3, sVar4);
    attention_info.distances[1] = attention_info.distances[0];
    attention_info.distances[3] = daNpcT_getDistTableIdx(sVar1, sVar2);
    attention_info.flags = fopAc_AttnFlag_SPEAK_e | fopAc_AttnFlag_TALK_e;
    scale.setall(l_HIO.m.common.scale);
    mCcStts.SetWeight(l_HIO.m.common.weight);
    mCylH = l_HIO.m.common.height;
    mWallR = l_HIO.m.common.width;
    mAcchCir.SetWallR(mWallR);
    mAcchCir.SetWallH(l_HIO.m.common.knee_length);
    mRealShadowSize = l_HIO.m.common.real_shadow_size;
    gravity = l_HIO.m.common.gravity;
    mExpressionMorfFrame = l_HIO.m.common.expression_morf_frame;
    mMorfFrames = l_HIO.m.common.morf_frame;
}

void daNpc_ZelR_c::setAfterTalkMotion() {
    mFaceMotionSeqMngr.setNo(1, -1.0f, 0, 0);
}

void daNpc_ZelR_c::srchActors() {
    /* empty function */
}

BOOL daNpc_ZelR_c::evtTalk() {
    if (chkAction(&daNpc_ZelR_c::talk)) {
        (this->*field_0xf90)(NULL);
    } else {
        mPreItemNo = 0;
        if (dComIfGp_event_chkTalkXY()) {
            if (!dComIfGp_evmng_ChkPresentEnd()) {
                return TRUE;
            } else {
                mEvtNo = 1;
                evtChange();
                return TRUE;
            }
        } else {
            setAction(&daNpc_ZelR_c::talk);
        }
    }

    return TRUE;
}

BOOL daNpc_ZelR_c::evtCutProc() {
    BOOL rv = FALSE;
    int staffId = dComIfGp_getEventManager().getMyStaffId("ZelR", this, -1);
    if (staffId != -1) {
        mStaffId = staffId;
        int actIdx = dComIfGp_getEventManager().getMyActIdx(mStaffId, &mCutNameList, 1, 0, 0);
        if ((this->*(mCutList[actIdx]))(mStaffId)) {
            dComIfGp_getEventManager().cutEnd(mStaffId);
        }
        rv = TRUE;
    }

    return rv;
}

void daNpc_ZelR_c::action() {
    fopAc_ac_c* hitActor = hitChk(&mCyl, 0xffffffff);
    if (hitActor) {
        mStagger.setParam(this, hitActor, mCurAngle.y);
        setDamage(0, 1, 0);
        mStagger.setPower(0.0f);
        mDamageTimerStart = 0;
    }

    if (mStagger.checkRebirth()) {
        mStagger.initialize();
        mMode = 1;
    }

    if (field_0xf84) {
        if (field_0xf90 == field_0xf84) {
            (this->*field_0xf90)(NULL);
        } else {
            setAction(field_0xf84);
        }
    }
}

void daNpc_ZelR_c::beforeMove() {
    if (checkHide() || mNoDraw) {
        attention_info.flags = 0;
    }
}

void daNpc_ZelR_c::setAttnPos() {
    cXyz sp38(10.0f, -30.0f, 0.0f);

    mStagger.calc(FALSE);

    mJntAnm.setParam(this, mpMorf[0]->getModel(), &sp38, getBackboneJointNo(), getNeckJointNo(), getHeadJointNo(),
        l_HIO.m.common.body_angleX_min, l_HIO.m.common.body_angleX_max, l_HIO.m.common.body_angleY_min, 
        l_HIO.m.common.body_angleY_max, l_HIO.m.common.head_angleX_min, l_HIO.m.common.head_angleX_max, 
        l_HIO.m.common.head_angleY_min, l_HIO.m.common.head_angleY_max, l_HIO.m.common.neck_rotation_ratio,
        0.0f, NULL);

    mJntAnm.calcJntRad(0.2f, 1.0f, cM_s2rad(mCurAngle.y - field_0xd7e.y));

    setMtx();
    mDoMtx_stack_c::copy(mpMorf[0]->getModel()->getAnmMtx(getHeadJointNo()));
    mDoMtx_stack_c::multVec(&sp38, &eyePos);
    
    mJntAnm.setEyeAngleX(eyePos, 1.0f, 0);
    mJntAnm.setEyeAngleY(eyePos, mCurAngle.y, 0, 1.0f, 0);

    attention_info.position = current.pos;
    attention_info.position.y += l_HIO.m.common.attention_offset;
}

void daNpc_ZelR_c::setCollision() {
    if (!mHide) {
        if (mTwilight == 1 && !dComIfGp_event_runCheck()) {
            mCyl.SetCoSPrm(0x69);
        } else {
            mCyl.SetCoSPrm(0x79);
        }

        if (mStagger.checkStagger()) {
            mCyl.SetTgType(0);
            mCyl.SetTgSPrm(0);
        } else {
            mCyl.SetTgType(0xd8fbfdff);
            mCyl.SetTgSPrm(0x1f);
            mCyl.OnTgNoHitMark();
        }

        f32 cylH = mCylH;
        f32 wallR = mWallR;
        cXyz sp28 = current.pos;
        mCyl.SetH(cylH);
        mCyl.SetR(wallR);
        mCyl.SetC(sp28);
        dComIfG_Ccsp()->Set(&mCyl);
    }

    mCyl.ClrCoHit();
    mCyl.ClrTgHit();
}

int daNpc_ZelR_c::drawDbgInfo() {
    return 0;
}

int daNpc_ZelR_c::selectAction() {
    field_0xf84 = NULL;
    field_0xf84 = &daNpc_ZelR_c::wait;
    return 1;
}

int daNpc_ZelR_c::chkAction(int (daNpc_ZelR_c::*param_1)(void*)) {
    return field_0xf90 == param_1;
}

int daNpc_ZelR_c::setAction(int (daNpc_ZelR_c::*param_1)(void*)) {
    mMode = 3;
    if (field_0xf90) {
        (this->*field_0xf90)(NULL);
    }

    mMode = 0;
    field_0xf90 = param_1;
    if (field_0xf90) {
        (this->*field_0xf90)(NULL);
    }

    return 1;
}

int daNpc_ZelR_c::wait(void* param_1) {
    switch (mMode) {
        case 0:
        case 1:
            mFaceMotionSeqMngr.setNo(1, -1.0f, 0, 0);
            mMotionSeqMngr.setNo(0, -1.0f, 0, 0);
            mMode = 2;
        case 2:
            if (!mStagger.checkStagger()) {
                BOOL bVar1;
                if (srchPlayerActor()) {
                    mJntAnm.lookPlayer(0);
                    bVar1 = checkStep();
                } else {
                    mJntAnm.lookNone(0);
                    bVar1 = home.angle.y != mCurAngle.y;
                }

                if (bVar1 && step(home.angle.y, -1, -1, 15, 0)) {
                    mMode = 1;
                }
            }
        case 3:
        default:
            return 1;
    }
}

BOOL daNpc_ZelR_c::talk(void* param_1) {
    BOOL bVar2 = FALSE;
    switch (mMode) {
        case 0:
        case 1:
            initTalk(mFlowNodeNo, NULL);
            mMode = 2;
        case 2:
            if (mTwilight) {
                bVar2 = TRUE;
            } else {
                mJntAnm.lookPlayer(0);
                if (mCurAngle.y != fopAcM_searchPlayerAngleY(this)) {
                    step(fopAcM_searchPlayerAngleY(this), -1, -1, 15, 0);
                } else {
                    bVar2 = TRUE;
                }
            }

            if (bVar2 && talkProc(NULL, 0, NULL, 0)) {
                mPlayerActorMngr.entry(daPy_getPlayerActorClass());
                dComIfGp_event_reset();
                mMode = 3;
            }
        case 3:
        default:
            return FALSE;
    }
}

static int daNpc_ZelR_Create(void* a_this) {
    return static_cast<daNpc_ZelR_c*>(a_this)->create();
}

static int daNpc_ZelR_Delete(void* a_this) {
    return static_cast<daNpc_ZelR_c*>(a_this)->Delete();
}

static int daNpc_ZelR_Execute(void* a_this) {
    return static_cast<daNpc_ZelR_c*>(a_this)->Execute();
}

static int daNpc_ZelR_Draw(void* a_this) {
    return static_cast<daNpc_ZelR_c*>(a_this)->Draw();
}

static int daNpc_ZelR_IsDelete(void* a_this) {
    return 1;
}

static actor_method_class daNpc_ZelR_MethodTable = {
    (process_method_func)daNpc_ZelR_Create,
    (process_method_func)daNpc_ZelR_Delete,
    (process_method_func)daNpc_ZelR_Execute,
    (process_method_func)daNpc_ZelR_IsDelete,
    (process_method_func)daNpc_ZelR_Draw,
};

extern actor_process_profile_definition g_profile_NPC_ZELR = {
  fpcLy_CURRENT_e,         // mLayerID
  7,                       // mListID
  fpcPi_CURRENT_e,         // mListPrio
  PROC_NPC_ZELR,           // mProcName
  &g_fpcLf_Method.base,   // sub_method
  sizeof(daNpc_ZelR_c),    // mSize
  0,                       // mSizeOther
  0,                       // mParameters
  &g_fopAc_Method.base,    // sub_method
  385,                     // mPriority
  &daNpc_ZelR_MethodTable, // sub_method
  0x00044108,              // mStatus
  fopAc_NPC_e,             // mActorType
  fopAc_CULLBOX_CUSTOM_e,  // cullType
};
