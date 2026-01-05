/**
 * @file d_a_npc_gnd.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_npc_gnd.h"

static int l_bmdData[1][2] = {
    {11, 1},
};

static daNpcT_evtData_c l_evtList[2] = {
    {"", 0},
    {"NO_RESPONSE", 0},
};

static char* l_resNameList[2] = {
    "",
    "Gnd",
};

static s8 l_loadResPtrn0[2] = {
    1, -1
};

static s8* l_loadResPtrnList[2] = {
    l_loadResPtrn0, l_loadResPtrn0
};

static daNpcT_faceMotionAnmData_c l_faceMotionAnmData[1] = {
    {-1, 0, 0, 23, 2, 1, 1}
};

static daNpcT_motionAnmData_c l_motionAnmData[1] = {
    {8, 2, 1, 20, 0, 1, 1, 0},
};

static daNpcT_MotionSeqMngr_c::sequenceStepData_c l_faceMotionSequenceData[4] = {
    {0, -1, 0}, 
    {-1, 0, 0}, 
    {-1, 0, 0}, 
    {-1, 0, 0},
};

static daNpcT_MotionSeqMngr_c::sequenceStepData_c l_motionSequenceData[4] = {
    {0, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}
};

char* daNpc_Gnd_c::mCutNameList[1] = {""};

daNpc_Gnd_c::cutFunc daNpc_Gnd_c::mCutList[1] = {NULL};

const daNpc_Gnd_HIOParam daNpc_Gnd_Param_c::m = {
    280.0f,
    -3.0f,
    1.0f,
    500.0f,
    255.0f,
    260.0f,
    35.0f,
    50.0f,
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
};

NPC_GND_HIO_CLASS l_HIO;

daNpc_Gnd_c::~daNpc_Gnd_c() {
    OS_REPORT("|%06d:%x|daNpc_Gnd_c -> デストラクト\n", g_Counter.mCounter0, this);
    if (heap != NULL) {
        mpMorf[0]->stopZelAnime();
    }

#if DEBUG
    if (mpHIO != NULL) {
        mpHIO->removeHIO();
    }
#endif

    deleteRes(l_loadResPtrnList[mType], (const char**)l_resNameList);
}

int daNpc_Gnd_c::create() {
    daNpcT_ct(this, daNpc_Gnd_c, l_faceMotionAnmData,
                       l_motionAnmData, l_faceMotionSequenceData, 4,
                       l_motionSequenceData, 4, l_evtList, l_resNameList);

    mType = getType();
    mFlowNodeNo = getFlowNodeNo();
    mTwilight = false;
    int phase_state = loadRes(l_loadResPtrnList[mType], (const char**)l_resNameList);
    if (phase_state == cPhs_COMPLEATE_e) {
        if (!fopAcM_entrySolidHeap(this, createHeapCallBack, 0)) {
            return cPhs_ERROR_e;
        }

        OS_REPORT("\t(%s:%d) flowNo:%d<%08x> ", fopAcM_getProcNameString(this),
                  mType, mFlowNodeNo, fopAcM_GetParam(this));
        if (isDelete()) {
            OS_REPORT("===>isDelete:TRUE\n");
            return cPhs_ERROR_e;
        }

        OS_REPORT("\n");
        J3DModelData* modelData = mpMorf[0]->getModel()->getModelData();
        fopAcM_SetMtx(this, mpMorf[0]->getModel()->getBaseTRMtx());
        fopAcM_setCullSizeBox(this, -300.0f, -50.0f, -300.0f, 300.0f, 450.0f, 300.0f);
        fopAcM_OnStatus(this, fopAcM_STATUS_UNK_0x8000000);
        mSound.init(&current.pos, &eyePos, 3, 1);
#if DEBUG
        mpHIO = &l_HIO;
        // Ganondorf:
        mpHIO->entryHIO("ガノンドロフ");
#endif
        mAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1,
                        &mAcchCir, fopAcM_GetSpeed_p(this), fopAcM_GetAngle_p(this),
                        fopAcM_GetShapeAngle_p(this));
        mCcStts.Init(mpHIO->m.common.weight, 0, this);
        mCyl1.Set(mCcDCyl);
        mCyl1.SetStts(&mCcStts);
        mCyl1.SetTgHitCallback(tgHitCallBack);
        mAcch.CrrPos(dComIfG_Bgsp());
        mGndChk = mAcch.m_gnd;
        mGroundH = mAcch.GetGroundH();
        if (mGroundH != -G_CM3D_F_INF) {
            setEnvTevColor();
            setRoomNo();
        }

        reset();
        mCreating = 1;
        Execute();
        mCreating = 0;
    }

    return phase_state;
}

int daNpc_Gnd_c::CreateHeap() {
    J3DModelData* mdlData_p = NULL;
    J3DModel* model = NULL;
    int bmdIdx = 0;
    int res_name_idx = l_bmdData[bmdIdx][1];
    int my_bmd = l_bmdData[bmdIdx][0];
    mdlData_p = static_cast<J3DModelData*>(dComIfG_getObjectRes(l_resNameList[res_name_idx], my_bmd));
    JUT_ASSERT(433, NULL != mdlData_p);

    u32 sp_0x24 = 0x11020285;
    mpMorf[0] = new mDoExt_McaMorfSO(mdlData_p, NULL, NULL, NULL, -1, 1.0f, 0, -1, &mSound, 0x80000, sp_0x24);
    if (mpMorf[0] != NULL && mpMorf[0]->getModel() == NULL) {
        mpMorf[0]->stopZelAnime();
        mpMorf[0] = NULL;
    }

    if (mpMorf[0] == NULL) {
        return 0;
    }

    model = mpMorf[0]->getModel();
    for (u16 i = 0; i < mdlData_p->getJointNum(); i++) {
        mdlData_p->getJointNodePointer(i)->setCallBack(ctrlJointCallBack);
    }

    model->setUserArea((uintptr_t)this);
    for (int idx = 0; idx < 2; ++idx) {
        mpMatAnm[idx] = new daNpcT_MatAnm_c();
        if (mpMatAnm[idx] == NULL) {
            return 0;
        }
    }

    if (setFaceMotionAnm(0, false) && setMotionAnm(0, 0.0f, FALSE)) {
        return 1;
    }

    return 0;
}

int daNpc_Gnd_c::Delete() {
    fopAcM_GetID(this);
    this->~daNpc_Gnd_c();
    return 1;
}

int daNpc_Gnd_c::Execute() {
    return daNpcT_c::execute();
}

int daNpc_Gnd_c::Draw() {
    J3DModelData* modelData = mpMorf[0]->getModel()->getModelData();
    if (mpMatAnm[0] != NULL) {
        modelData->getMaterialNodePointer(getEyeballLMaterialNo())->setMaterialAnm(mpMatAnm[0]);
    }

    if (mpMatAnm[1] != NULL) {
        modelData->getMaterialNodePointer(getEyeballRMaterialNo())->setMaterialAnm(mpMatAnm[1]);
    }

    return daNpcT_c::draw(0, 1, mRealShadowSize, NULL, 100.0f, 0, 0, 0);
}

int daNpc_Gnd_c::createHeapCallBack(fopAc_ac_c* i_this) {
    return ((daNpc_Gnd_c*)i_this)->CreateHeap();
}

int daNpc_Gnd_c::ctrlJointCallBack(J3DJoint* i_joint, int arg1) {
    if (arg1 == 0) {
        J3DModel* model = j3dSys.getModel();
        daNpc_Gnd_c* i_this = (daNpc_Gnd_c*)model->getUserArea();
        if (i_this != NULL) {
            i_this->ctrlJoint(i_joint, model);
        }
    }

    return 1;
}

u8 daNpc_Gnd_c::getType() {
    switch (fopAcM_GetParam(this) & 0xFF) {
    case 0:
        return TYPE_0;
    default:
        return TYPE_1;
    }
}

int daNpc_Gnd_c::getFlowNodeNo() {
    u16 nodeNo = home.angle.x;
    if (nodeNo == 0xffff) {
        return -1;
    }
    return nodeNo;
}

int daNpc_Gnd_c::isDelete() {
    if (mType == TYPE_1) {
        return 0;
    }

    switch (mType) {
    case TYPE_0:
        return 0;
    default:
        return 1;
    }
}

void daNpc_Gnd_c::reset() {
    initialize();
    memset(&mNextAction, 0, (u8*)&field_0xF9C - (u8*)&mNextAction);

    for (int idx = 0; idx < 2; ++idx) {
        if (mpMatAnm[idx] != NULL) {
            mpMatAnm[idx]->initialize();
        }
    }

    setAngle(home.angle.y);
}

void daNpc_Gnd_c::afterJntAnm(int param_1) {
    if (param_1 == 1) {
        mDoMtx_stack_c::YrotM(-mStagger.getAngleZ(1));
        mDoMtx_stack_c::ZrotM(mStagger.getAngleX(1));
    } else if (param_1 == 4) {
        mDoMtx_stack_c::YrotM(-mStagger.getAngleZ(0));
        mDoMtx_stack_c::ZrotM(mStagger.getAngleX(0));
    }
}

BOOL daNpc_Gnd_c::ctrlBtk() {
    if (mpMatAnm[0] != NULL && mpMatAnm[1] != NULL) {
        if (field_0xe29 != 0 && mBtkAnm.getBtkAnm()) {
            mpMatAnm[0]->setNowOffsetX(-1.0f * (0.2f * cM_ssin(mJntAnm.getEyeAngleY())));
            mpMatAnm[0]->setNowOffsetY(0.2f * cM_ssin(mJntAnm.getEyeAngleX()));
            mpMatAnm[1]->setNowOffsetX(0.2f * cM_ssin(mJntAnm.getEyeAngleY()));
            mpMatAnm[1]->setNowOffsetY(0.2f * cM_ssin(mJntAnm.getEyeAngleX()));

            if (field_0xe2a != 0) {
                mpMatAnm[0]->setMorfFrm(field_0xe2a);
                mpMatAnm[1]->setMorfFrm(field_0xe2a);
                field_0xe2a = 0;
            }

            mpMatAnm[0]->onEyeMoveFlag();
            mpMatAnm[1]->onEyeMoveFlag();
            return 1;
        }

        if (field_0xe2a != 0) {
            mpMatAnm[0]->setMorfFrm(field_0xe2a);
            mpMatAnm[1]->setMorfFrm(field_0xe2a);
            field_0xe2a = 0;
        }

        mpMatAnm[0]->offEyeMoveFlag();
        mpMatAnm[1]->offEyeMoveFlag();
    }

    return 0;
}

void daNpc_Gnd_c::setParam() {
    u32 attn_flags = (fopAc_AttnFlag_SPEAK_e | fopAc_AttnFlag_TALK_e);

    selectAction();
    srchActors();

    s16 talk_distance = mpHIO->m.common.talk_distance;
    s16 talk_angle = mpHIO->m.common.talk_angle;
    s16 attention_distance = mpHIO->m.common.attention_distance;
    s16 attention_angle = mpHIO->m.common.attention_angle;

    attention_info.distances[fopAc_attn_LOCK_e] = daNpcT_getDistTableIdx(attention_distance, attention_angle);
    attention_info.distances[fopAc_attn_TALK_e] = attention_info.distances[fopAc_attn_LOCK_e];
    attention_info.distances[fopAc_attn_SPEAK_e] = daNpcT_getDistTableIdx(talk_distance, talk_angle);
    attention_info.flags = attn_flags;

    scale.set(mpHIO->m.common.scale, mpHIO->m.common.scale, mpHIO->m.common.scale);
    mCcStts.SetWeight(mpHIO->m.common.weight);
    mCylH = mpHIO->m.common.height;
    mWallR = mpHIO->m.common.width;
    mAttnFovY = mpHIO->m.common.fov;
    mAcchCir.SetWallR(mWallR);
    mAcchCir.SetWallH(mpHIO->m.common.knee_length);
    mRealShadowSize = mpHIO->m.common.real_shadow_size;
    gravity = mpHIO->m.common.gravity;
    mExpressionMorfFrame = mpHIO->m.common.expression_morf_frame;
    mMorfFrames = mpHIO->m.common.morf_frame;

    mAcch.SetGrndNone();
    mAcch.SetWallNone();
    gravity = 0.0f;
}

void daNpc_Gnd_c::setAfterTalkMotion() {
    mFaceMotionSeqMngr.getNo();
    mFaceMotionSeqMngr.setNo(0, -1.0f, 0, 0);
}

void daNpc_Gnd_c::srchActors() {}

BOOL daNpc_Gnd_c::evtTalk() {
    if (chkAction(&daNpc_Gnd_c::talk)) {
        (this->*mAction)(NULL);
    } else {
        mPreItemNo = 0;
        if (dComIfGp_event_chkTalkXY()) {
            if (!dComIfGp_evmng_ChkPresentEnd()) {
                return TRUE;
            }
            mEvtNo = 1;
            evtChange();
            return TRUE;
        } else {
            setAction(&daNpc_Gnd_c::talk);
        }
    }

    return TRUE;
}

BOOL daNpc_Gnd_c::evtCutProc() {
    BOOL rt = FALSE;

    s32 staffId = dComIfGp_getEventManager().getMyStaffId("Gnd", this, -1);
    if (staffId != -1) {
        mStaffId = staffId;
        int actIdx = dComIfGp_getEventManager().getMyActIdx(mStaffId, mCutNameList, 1, FALSE, FALSE);
        if ((this->*mCutList[actIdx])(mStaffId)) {
            dComIfGp_getEventManager().cutEnd(mStaffId);
        }

        rt = TRUE;
    }

    return rt;
}

void daNpc_Gnd_c::action() {
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

void daNpc_Gnd_c::beforeMove() {
    if (checkHide() || mNoDraw) {
        attention_info.flags = 0;
    }
}

void daNpc_Gnd_c::setAttnPos() {
    cXyz cStack_3c(10.0f, 30.0f, 0.0f);
    cXyz cStack_48(10.0f, 0.0f, 0.0f);

    mStagger.calc(FALSE);
    f32 rad = cM_s2rad((s16)(mCurAngle.y - field_0xd7e.y));

    mJntAnm.setParam(this, mpMorf[0]->getModel(), &cStack_3c, getBackboneJointNo(),
                     getNeckJointNo(), getHeadJointNo(), mpHIO->m.common.body_angleX_min,
                     mpHIO->m.common.body_angleX_max, mpHIO->m.common.body_angleY_min,
                     mpHIO->m.common.body_angleY_max, mpHIO->m.common.head_angleX_min,
                     mpHIO->m.common.head_angleX_max, mpHIO->m.common.head_angleY_min,
                     mpHIO->m.common.head_angleY_max, mpHIO->m.common.neck_rotation_ratio, rad,
                     &cStack_48);

    mJntAnm.calcJntRad(0.2f, 1.0f, rad);
    setMtx();

    mDoMtx_stack_c::copy(mpMorf[0]->getModel()->getAnmMtx(getHeadJointNo()));
    mDoMtx_stack_c::multVec(&cStack_3c, &eyePos);

    mJntAnm.setEyeAngleX(eyePos, 1.0f, 0);
    mJntAnm.setEyeAngleY(eyePos, mCurAngle.y, FALSE, 1.0f, 0);

    attention_info.position = current.pos;
    attention_info.position.y += mpHIO->m.common.attention_offset;
}

void daNpc_Gnd_c::setCollision() {
    cXyz pos;
    u32 tgType = 0xD8FBFDFF;
    u32 tgSPrm = 0x1F;

    if (!mHide) {
        if (dComIfGp_event_runCheck()) {
            tgType = 0;
            tgSPrm = 0;
        } else {
            if (mTwilight) {
                tgType = 0;
                tgSPrm = 0;
            } else if (mStagger.checkStagger()) {
                tgType = 0;
                tgSPrm = 0;
            }
        }

        f32 cylH = mCylH;
        f32 wallR = mWallR;
        pos = current.pos;
        mCyl1.SetCoSPrm(0x79);
        mCyl1.SetTgType(tgType);
        mCyl1.SetTgSPrm(tgSPrm);
        mCyl1.OnTgNoHitMark();
        mCyl1.SetH(cylH);
        mCyl1.SetR(wallR);
        mCyl1.SetC(pos);
        dComIfG_Ccsp()->Set(&mCyl1);
    }

    mCyl1.ClrCoHit();
    mCyl1.ClrTgHit();
}

int daNpc_Gnd_c::drawDbgInfo() {
    return 0;
}

bool daNpc_Gnd_c::afterSetMotionAnm(int param_0, int param_1, f32 param_2, int param_3) {
    static struct {
        int _0;
        int _4;
    } brkAnmData[] = {
        {0x11, 1},
    };

    static struct {
        int _0;
        int _4;
    } bpkAnmData[] = {
        {0x0E, 1},
    };
    
    J3DAnmTevRegKey* brk = NULL;
    J3DAnmColor* bpk = NULL;

    if (brkAnmData[param_0]._0 != -1) {
        brk = getTevRegKeyAnmP(l_resNameList[brkAnmData[param_0]._4], brkAnmData[param_0]._0);
    }

    if (brk != NULL) {
        if (mBrkAnm.getBrkAnm() == brk) {
            mAnmFlags |= 0x100;
        } else if (setBrkAnm(brk, mpMorf[0]->getModel()->getModelData(), 1.0f, param_1)) {
            mAnmFlags |= 0x104;
        }
    }

    if (brk == NULL && brkAnmData[param_0]._0 != -1) {
        return 0;
    }

    if (bpkAnmData[param_0]._0 != -1) {
        bpk = getColorAnmP(l_resNameList[bpkAnmData[param_0]._4], bpkAnmData[param_0]._0);
    }

    if (bpk != NULL) {
        if (mBpkAnm.getBpkAnm() == bpk) {
            mAnmFlags |= 0x200;
        } else if (setBpkAnm(bpk, mpMorf[0]->getModel()->getModelData(), 1.0f, param_1)) {
            mAnmFlags |= 0x208;
        }
    }

    if (bpk == NULL && bpkAnmData[param_0]._0 != -1) {
        return 0;
    }

    return 1;
}

int daNpc_Gnd_c::selectAction() {
    mNextAction = NULL;
    mNextAction = &daNpc_Gnd_c::wait;
    return 1;
}

int daNpc_Gnd_c::chkAction(int (daNpc_Gnd_c::*i_action)(void*)) {
    return mAction == i_action;
}

int daNpc_Gnd_c::setAction(int (daNpc_Gnd_c::*i_action)(void*)) {
    mMode = MODE_EXIT;
    if (mAction != NULL) {
        (this->*(mAction))(NULL);
    }

    mMode = MODE_ENTER;
    mAction = i_action;
    if (mAction != NULL) {
        (this->*(mAction))(NULL);
    }

    return 1;
}

int daNpc_Gnd_c::wait(void* param_0) {
    switch (mMode) {
    case MODE_ENTER:
    case MODE_INIT:
        if (!mStagger.checkStagger()) {
            mFaceMotionSeqMngr.setNo(0, -1.0f, FALSE, 0);
            mMotionSeqMngr.setNo(0, -1.0f, FALSE, 0);
            mMode = MODE_RUN;
        }
    case MODE_RUN:
        if (!mStagger.checkStagger()) {
            if (mPlayerActorMngr.getActorP()) {
                mJntAnm.lookNone(0);
                if (chkActorInSight(mPlayerActorMngr.getActorP(), mAttnFovY, mCurAngle.y)) {
                    mJntAnm.lookPlayer(0);
                }
                if (!srchPlayerActor() && home.angle.y == mCurAngle.y) {
                    mMode = MODE_INIT;
                }
            } else {
                mJntAnm.lookNone(0);
                if (home.angle.y != mCurAngle.y) {
                    if (field_0xe34 == 0) {
                        setAngle(home.angle.y);
                        mMode = MODE_INIT;
                    } else if (step(home.angle.y, -1, -1, 15, 0)) {
                        mMode = MODE_INIT;
                    }
                    attention_info.flags = 0;
                } else {
                    srchPlayerActor();
                }
            }
        }
        break;
    case MODE_EXIT:
        break;
    }

    return 1;
}

int daNpc_Gnd_c::talk(void* param_0) {
    switch (mMode) {
    case MODE_ENTER:
    case MODE_INIT:
        if (!mStagger.checkStagger()) {
            initTalk(mFlowNodeNo, NULL);
            if (checkStep()) {
                mStepMode = 0;
            }
            mMode = MODE_RUN;
        }
    case MODE_RUN:
        if (!mStagger.checkStagger()) {
            if (mTwilight || mCurAngle.y == fopAcM_searchPlayerAngleY(this)) {
                if (talkProc(NULL, FALSE, NULL, FALSE)) {
                    mPlayerActorMngr.entry(daPy_getPlayerActorClass());
                    dComIfGp_event_reset();
                    mMode = MODE_EXIT;
                }
                mJntAnm.lookPlayer(0);
                if (mTwilight) {
                    mJntAnm.lookNone(0);
                }
            } else {
                mJntAnm.lookPlayer(0);
                step(fopAcM_searchPlayerAngleY(this), -1, -1, 15, 0);
            }
        }
        break;
    case MODE_EXIT:
        break;
    }

    return 0;
}

static int daNpc_Gnd_Create(void* i_this) {
    return ((daNpc_Gnd_c*)i_this)->create();
}

static int daNpc_Gnd_Delete(void* i_this) {
    return ((daNpc_Gnd_c*)i_this)->Delete();
}

static int daNpc_Gnd_Execute(void* i_this) {
    return ((daNpc_Gnd_c*)i_this)->Execute();
}

static int daNpc_Gnd_Draw(void* i_this) {
    return ((daNpc_Gnd_c*)i_this)->Draw();
}

static int daNpc_Gnd_IsDelete(void* i_this) {
    return 1;
}

static actor_method_class daNpc_Gnd_MethodTable = {
    (process_method_func)daNpc_Gnd_Create,
    (process_method_func)daNpc_Gnd_Delete,
    (process_method_func)daNpc_Gnd_Execute,
    (process_method_func)daNpc_Gnd_IsDelete,
    (process_method_func)daNpc_Gnd_Draw,
};

actor_process_profile_definition g_profile_NPC_GND = {
  fpcLy_CURRENT_e,        // mLayerID
  7,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_NPC_GND,           // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(daNpc_Gnd_c),    // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  305,                    // mPriority
  &daNpc_Gnd_MethodTable, // sub_method
  0x00044107,             // mStatus
  fopAc_NPC_e,            // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};
