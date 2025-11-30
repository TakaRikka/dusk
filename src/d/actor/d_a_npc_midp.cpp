/**
 * @file d_a_npc_midp.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_npc_midp.h"

enum midP_RES_File_ID {
    /* BCK */
    /* 0x6 */ BCK_MIDP_WAIT_A = 0x6,

    /* BMDR */
    /* 0x9 */ BMDR_MIDP = 0x9,

    /* BTK */
    /* 0xC */ BTK_MIDP = 0xC,

    /* BTP */
    /* 0xF */ BTP_MIDP = 0xF,
};

static int l_bmdData[1][2] = {
    {BMDR_MIDP, 1},
};

static daNpcT_evtData_c l_evtList[2] = {
    {"", 0},
    {"NO_RESPONSE", 0},
};

static char* l_resNameList[2] = {
    "",
    "midP"
};

static s8 l_loadResPtrn0[1 + 1 /* padding */] = {
    1,
    -1
};

static s8* l_loadResPtrnList[2] = {
    l_loadResPtrn0,
    l_loadResPtrn0,
};

static daNpcT_faceMotionAnmData_c l_faceMotionAnmData = {
    -1, 0, 0, 15, 2, 1, 1
};

static daNpcT_motionAnmData_c l_motionAnmData = {
    6, 2, 1, 12, 0, 1, 1, 0
};

static daNpcT_MotionSeqMngr_c::sequenceStepData_c l_faceMotionSequenceData[4] = {
    {0, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}
};

static daNpcT_MotionSeqMngr_c::sequenceStepData_c l_motionSequenceData[4] = {
    {0, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}
};

char* daNpc_midP_c::mCutNameList = "";

daNpc_midP_c::cutFunc daNpc_midP_c::mCutList[1] = { 0 };

daNpc_midP_c::~daNpc_midP_c() {
    OS_REPORT("|%06d:%x|daNpc_midP_c -> デストラクト\n", g_Counter.mCounter0, this);
    if (heap != 0) {
        mpMorf[0]->stopZelAnime();
    }
    deleteRes(l_loadResPtrnList[mType], (char const**)l_resNameList);
}

daNpc_midP_HIOParam const daNpc_midP_Param_c::m = {
    210.0f, 
    -3.0f,
    1.0f,
    500.0f, 
    255.0f,
    190.0f,
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
    0.0f, 
    0.0f, 
    0.0f, 
    0.0f, 
    0.0f,
    0.0f,
    0.0f,
};

int daNpc_midP_c::create() {
    daNpcT_ct(this, daNpc_midP_c, &l_faceMotionAnmData, &l_motionAnmData, l_faceMotionSequenceData, 4,
        l_motionSequenceData, 4, l_evtList, l_resNameList);
    
    mType = getType();
    mFlowNodeNo = getFlowNodeNo();
    mTwilight = 0;

    int rv = loadRes(l_loadResPtrnList[mType], (const char**)l_resNameList);
    if (rv == cPhs_COMPLEATE_e) {
        if (!fopAcM_entrySolidHeap(this, createHeapCallBack, 0)) {
            OS_REPORT("===>isDelete:TRUE\n");
            return cPhs_ERROR_e;
        }
        OS_REPORT("\n");

        OS_REPORT("\t(%s:%d) flowNo:%d, PathID:%02x<%08x> ", fopAcM_getProcNameString(this), mType,
        mFlowNodeNo, fopAcM_GetParam(this));
        if (isDelete()) {
            OS_REPORT("===>isDelete:TRUE\n");
            return cPhs_ERROR_e;
        }

        J3DModelData* modelData = mpMorf[0]->getModel()->getModelData();
        fopAcM_SetMtx(this, mpMorf[0]->getModel()->getBaseTRMtx());
        fopAcM_setCullSizeBox(this, -300.0f, -50.0f, -300.0f, 300.0f, 450.0f, 300.0f);
        fopAcM_OnStatus(this, 0x8000000);

        mSound.init(&current.pos, &eyePos, 3, 1);

        mAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1, &mAcchCir,
            fopAcM_GetSpeed_p(this), fopAcM_GetAngle_p(this), fopAcM_GetShapeAngle_p(this));
        mCcStts.Init(daNpc_midP_Param_c::m.common.weight, 0, this);
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

    return rv;
}

int daNpc_midP_c::CreateHeap() {
    J3DModelData* modelData = static_cast<J3DModelData*>(dComIfG_getObjectRes(
        l_resNameList[l_bmdData[0][1]], l_bmdData[0][0]));

    mpMorf[0] = new mDoExt_McaMorfSO(modelData, NULL, NULL, NULL, -1, 1.0f, 0, -1, &mSound,
        0x80000, 0x11020284);
    if (mpMorf[0] == NULL || mpMorf[0]->getModel() == NULL) {
        if (mpMorf[0] != NULL) {
            mpMorf[0]->stopZelAnime();
        }
        return 0;
    }

    J3DModel* model = mpMorf[0]->getModel();
    for (u16 i = 0; i < modelData->getJointNum(); i++) {
        modelData->getJointNodePointer(i)->setCallBack(ctrlJointCallBack);
    }

    model->setUserArea((uintptr_t)this);
    for (int i = 0; i < 2; i++) {
        mpMatAnm[i] = new daNpcT_MatAnm_c();

        if (mpMatAnm[i] == NULL) {
            return 0;
        }
    }

    if (setFaceMotionAnm(0, false) && setMotionAnm(0, 0.0f, 0) != 0) {
        return 1;
    } else {
        return 0;
    }
}

int daNpc_midP_c::Delete() {
    OS_REPORT("|%06d:%x|daNpc_midP_c -> Delete\n", g_Counter.mCounter0, this);
    fopAcM_GetID(this);
    this->~daNpc_midP_c();
    return 1;
}

int daNpc_midP_c::Execute() {
    return execute();
}

int daNpc_midP_c::Draw() {
    J3DModel* model = mpMorf[0]->getModel();
    J3DModelData* modelData = model->getModelData();
    if (mpMatAnm[0]) {
        modelData->getMaterialNodePointer(getEyeballLMaterialNo())->setMaterialAnm(mpMatAnm[0]);
    }
    if (mpMatAnm[1]) {
        modelData->getMaterialNodePointer(getEyeballRMaterialNo())->setMaterialAnm(mpMatAnm[1]);
    }

    return daNpcT_c::draw(0, 0, mRealShadowSize, NULL, 0.0f, 1, 0, 0);
}

int daNpc_midP_c::createHeapCallBack(fopAc_ac_c* a_this) {
    daNpc_midP_c* i_this = (daNpc_midP_c*)a_this;
    return i_this->CreateHeap();
}

int daNpc_midP_c::ctrlJointCallBack(J3DJoint* param_1, int param_2) {
    if (param_2 == 0) {
        J3DModel* model = j3dSys.getModel();
        daNpc_midP_c* i_this = (daNpc_midP_c*)model->getUserArea();
        if (i_this != 0) {
            i_this->ctrlJoint(param_1, model);
        }
    }

    return 1;
}

u8 daNpc_midP_c::getType() {
    switch ((u8)fopAcM_GetParam(this)) {
        case 0:
            return TYPE_0;
        default:
            return TYPE_1;
    }
}

int daNpc_midP_c::isDelete() {
    switch (mType) {
        case 0:
            return FALSE;
        default:
            return FALSE;
    }
}

void daNpc_midP_c::reset() {
    int iVar1 = (u8*)&field_0xf9c - (u8*)&field_0xf84;
    for (int i = 0; i < 2; i++) {
        if (mpMatAnm[i]) {
            mpMatAnm[i]->initialize();
        }
    }
    initialize();

    memset(&field_0xf84, 0, iVar1);
    setAngle(home.angle.y);
}

void daNpc_midP_c::afterJntAnm(int param_1) {
    if (param_1 == 2) {
        mDoMtx_stack_c::YrotM(mStagger.getAngleZ(1));
        mDoMtx_stack_c::ZrotM(-mStagger.getAngleX(1));
    } else if (param_1 == 4) {
        mDoMtx_stack_c::YrotM(mStagger.getAngleZ(0));
        mDoMtx_stack_c::ZrotM(-mStagger.getAngleX(0));
    }
}

BOOL daNpc_midP_c::ctrlBtk() {
    if (mpMatAnm[0] && mpMatAnm[1]) {
        if (field_0xe29 && mBtkAnm.getBtkAnm()) {
            mpMatAnm[0]->setNowOffsetX(field_0xde4 * cM_ssin(mJntAnm.getEyeAngleY()) * -1.0f);
            mpMatAnm[0]->setNowOffsetY(field_0xde0 * cM_ssin(mJntAnm.getEyeAngleX()));
            mpMatAnm[1]->setNowOffsetX(field_0xde4 * cM_ssin(mJntAnm.getEyeAngleY()));
            mpMatAnm[1]->setNowOffsetY(field_0xde0 * cM_ssin(mJntAnm.getEyeAngleX()));
            
            if (field_0xe2a) {
                mpMatAnm[0]->setMorfFrm(field_0xe2a);
                mpMatAnm[1]->setMorfFrm(field_0xe2a);
                field_0xe2a = 0;
            }

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

void daNpc_midP_c::setParam() {
    selectAction();
    srchActors();

    s16 sVar1 = daNpc_midP_Param_c::m.common.talk_distance;
    s16 sVar2 = daNpc_midP_Param_c::m.common.talk_angle;
    s16 sVar3 = daNpc_midP_Param_c::m.common.attention_distance;
    s16 sVar4 = daNpc_midP_Param_c::m.common.attention_angle;

    attention_info.distances[0] = daNpcT_getDistTableIdx(sVar3, sVar4);
    attention_info.distances[1] = attention_info.distances[0];
    attention_info.distances[3] = daNpcT_getDistTableIdx(sVar1, sVar2);
    attention_info.flags = fopAc_AttnFlag_SPEAK_e | fopAc_AttnFlag_TALK_e;

    scale.setall(daNpc_midP_Param_c::m.common.scale);
    mCcStts.SetWeight(daNpc_midP_Param_c::m.common.weight);
    mCylH = daNpc_midP_Param_c::m.common.height;
    mWallR = daNpc_midP_Param_c::m.common.width;
    mAttnFovY = daNpc_midP_Param_c::m.common.fov;
    mAcchCir.SetWallR(mWallR);
    mAcchCir.SetWallH(daNpc_midP_Param_c::m.common.knee_length);
    mRealShadowSize = daNpc_midP_Param_c::m.common.real_shadow_size;
    mExpressionMorfFrame = daNpc_midP_Param_c::m.common.expression_morf_frame;
    mMorfFrames = daNpc_midP_Param_c::m.common.morf_frame;
    gravity = daNpc_midP_Param_c::m.common.gravity;
}

void daNpc_midP_c::setAfterTalkMotion() {
    mFaceMotionSeqMngr.setNo(0, -1.0f, 0, 0);
}

void daNpc_midP_c::srchActors() {
    /* empty function */
}

BOOL daNpc_midP_c::evtTalk() {
    if (chkAction(&daNpc_midP_c::talk)) {
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
            setAction(&daNpc_midP_c::talk);
        }
    }

    return TRUE;
}

BOOL daNpc_midP_c::evtCutProc() {
    int staffId = dComIfGp_getEventManager().getMyStaffId("midP", this, -1);
    if (staffId != -1) {
        mStaffId = staffId;
        int actIdx = dComIfGp_getEventManager().getMyActIdx(mStaffId, &mCutNameList, 1, 0, 0);
        if ((this->*(mCutList[actIdx]))(mStaffId) != 0) {
            dComIfGp_getEventManager().cutEnd(mStaffId);
        }
        return TRUE;
    } else {
        return FALSE;
    }
}

void daNpc_midP_c::action() {
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

void daNpc_midP_c::beforeMove() {
    if (checkHide() || mNoDraw) {
        attention_info.flags = 0;
    }
}

void daNpc_midP_c::setAttnPos() {
    cXyz sp3c(10.0f, -30.0f, 0.0f);
    cXyz sp48(10.0f, 0.0f, 0.0f);

    mStagger.calc(FALSE);
    f32 dVar5 = cM_s2rad(mCurAngle.y - field_0xd7e.y);
    J3DModel* model = mpMorf[0]->getModel();
    mJntAnm.setParam(this, model, &sp3c, getBackboneJointNo(), getNeckJointNo(),
        getHeadJointNo(), daNpc_midP_Param_c::m.common.body_angleX_min, daNpc_midP_Param_c::m.common.body_angleX_max,
        daNpc_midP_Param_c::m.common.body_angleY_min, daNpc_midP_Param_c::m.common.body_angleY_max,
        daNpc_midP_Param_c::m.common.head_angleX_min, daNpc_midP_Param_c::m.common.head_angleX_max,
        daNpc_midP_Param_c::m.common.head_angleY_min, daNpc_midP_Param_c::m.common.head_angleY_max,
        daNpc_midP_Param_c::m.common.neck_rotation_ratio, dVar5, &sp48);
    mJntAnm.calcJntRad(0.2f, 1.0f, dVar5);
    setMtx();

    mDoMtx_stack_c::copy(mpMorf[0]->getModel()->getAnmMtx(getHeadJointNo()));
    mDoMtx_stack_c::multVec(&sp3c, &eyePos);
    mJntAnm.setEyeAngleX(eyePos, 1.0f, 0);
    mJntAnm.setEyeAngleY(eyePos, mCurAngle.y, 0, 1.0f, 0);

    attention_info.position = current.pos;
    attention_info.position.y += daNpc_midP_Param_c::m.common.attention_offset;
}

void daNpc_midP_c::setCollision() {
    if (!mHide) {
        u32 tgType = 0xd8fbfdff;
        u32 tgSPrm = 0x1f;
        if (dComIfGp_event_runCheck()) {
            tgType = 0;
            tgSPrm = 0;
        } else {
            if (mTwilight) {
                tgType = 0;
                tgSPrm = 0;
            } else {
                if (mStagger.checkStagger()) {
                    tgType = 0;
                    tgSPrm = 0;
                }
            }
        }

        mCyl.SetCoSPrm(0x79);
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

int daNpc_midP_c::drawDbgInfo() {
    return 0;
}

void daNpc_midP_c::drawGhost() {
    J3DModel* model = mpMorf[0]->getModel();
    g_env_light.settingTevStruct(3, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(model, &tevStr);
    mpMorf[0]->entryDL();
}

int daNpc_midP_c::selectAction() {
    field_0xf84 = NULL;
    field_0xf84 = &daNpc_midP_c::wait;
    return 1;
}

int daNpc_midP_c::chkAction(int (daNpc_midP_c::*action)(void*)) {
    return field_0xf90 == action;
}

int daNpc_midP_c::setAction(int (daNpc_midP_c::*action)(void*)) {
    mMode = 3;
    if (field_0xf90) {
        (this->*field_0xf90)(NULL);
    }
    
    mMode = 0;
    field_0xf90 = action;
    if (field_0xf90) {
        (this->*field_0xf90)(NULL);
    }

    return 1;
}

int daNpc_midP_c::wait(void* param_1) {
    switch (mMode) {
        case 0:
        case 1:
            if (!mStagger.checkStagger()) {
                mFaceMotionSeqMngr.setNo(0, -1.0f, 0, 0);
                mMotionSeqMngr.setNo(0, -1.0f, 0, 0);
                mMode = 2;
            }
        case 2:
            if (!mStagger.checkStagger()) {
                if (mPlayerActorMngr.getActorP()) {
                    mJntAnm.lookPlayer(0);
                    if (!chkActorInSight(mPlayerActorMngr.getActorP(), mAttnFovY, mCurAngle.y)) {
                        mJntAnm.lookNone(0);
                    }
                
                    if (!srchPlayerActor() && home.angle.y == mCurAngle.y) {
                        mMode = 1;
                    }
                } else {
                    mJntAnm.lookNone(0);
                    if (home.angle.y != mCurAngle.y) {
                        if (field_0xe34) {
                            if (step(home.angle.y, -1, -1, 15, 0)) {
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
        case 3:
        default:
            return 1;
    }
}

int daNpc_midP_c::talk(void* param_1) {
    switch (mMode) {
        case 0:
        case 1:
            if (!mStagger.checkStagger()) {
                initTalk(mFlowNodeNo, NULL);
                mPlayerAngle = fopAcM_searchPlayerAngleY(this);
                if (checkStep()) {
                    mStepMode = 0;
                }

                speedF = 0.0f;
                speed.setall(0.0f);
                mMode = 2;
            }
        case 2:
            if (!mStagger.checkStagger()) {
                if (mTwilight || mPlayerAngle == mCurAngle.y) {
                    if (talkProc(NULL, 0, NULL, 0) && mFlow.checkEndFlow()) {
                        mPlayerActorMngr.entry(daPy_getPlayerActorClass());
                        dComIfGp_event_reset();
                        mMode = 3;
                    }
                    
                    mJntAnm.lookPlayer(0);
                    if (mTwilight) {
                        mJntAnm.lookNone(0);
                    }
                } else {
                    mJntAnm.lookPlayer(0);
                    step(mPlayerAngle, -1, -1, 15, 0);
                }
            }
        case 3:
        default:
            return 0;
    }
}

static int daNpc_midP_Create(void* i_this) {
    return static_cast<daNpc_midP_c*>(i_this)->create();
}

static int daNpc_midP_Delete(void* i_this) {
    return static_cast<daNpc_midP_c*>(i_this)->Delete();
}

static int daNpc_midP_Execute(void* i_this) {
    return static_cast<daNpc_midP_c*>(i_this)->Execute();
}

static int daNpc_midP_Draw(void* i_this) {
    return static_cast<daNpc_midP_c*>(i_this)->Draw();
}

static int daNpc_midP_IsDelete(void* i_this) {
    return 1;
}

static daNpc_midP_Param_c l_HIO;

static actor_method_class daNpc_midP_MethodTable = {
    (process_method_func)daNpc_midP_Create,
    (process_method_func)daNpc_midP_Delete,
    (process_method_func)daNpc_midP_Execute,
    (process_method_func)daNpc_midP_IsDelete,
    (process_method_func)daNpc_midP_Draw,
};

extern actor_process_profile_definition g_profile_NPC_MIDP = {
  fpcLy_CURRENT_e,         // mLayerID
  7,                       // mListID
  fpcPi_CURRENT_e,         // mListPrio
  PROC_NPC_MIDP,           // mProcName
  &g_fpcLf_Method.base,   // sub_method
  sizeof(daNpc_midP_c),    // mSize
  0,                       // mSizeOther
  0,                       // mParameters
  &g_fopAc_Method.base,    // sub_method
  354,                     // mPriority
  &daNpc_midP_MethodTable, // sub_method
  0x00044108,              // mStatus
  fopAc_NPC_e,             // mActorType
  fopAc_CULLBOX_CUSTOM_e,  // cullType
};
