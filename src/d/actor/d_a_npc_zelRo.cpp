/**
 * @file d_a_npc_zelRo.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_npc_zelRo.h"

enum zelRo_RES_File_ID {
    /* BCK */
    /* 0x06 */ BCK_ZELRO_F_TALK_A = 0x6,
    /* 0x07 */ BCK_ZELRO_WAIT_A,

    /* BMDR */
    /* 0x0A */ BMDR_ZELRO = 0xA,

    /* BTK */
    /* 0x0D */ BTK_ZELRO = 0xD,

    /* BTP */
    /* 0x10 */ BTP_ZELRO = 0x10,
};

enum RES_Name {
    /* 0x0 */ NONE,
    /* 0x1 */ ZELRO,
};

enum Face_Motion {
    /* 0x0 */ FACE_TALK_A,
    /* 0x1 */ FACE_NONE,
};

enum Motion {
    /* 0x0 */ MOT_WAIT_A,
};

enum Type {
    /* 0x0 */ TYPE_0,
    /* 0x1 */ TYPE_1,
};

static int l_bmdData[1][2] = {
    {BMDR_ZELRO, ZELRO},
};

static daNpcT_evtData_c l_evtList[2] = {
    {"", 0},
    {"NO_RESPONSE", 0},
};

static char* l_resNameList[2] = {
    "",
    "zelRo",
};

static s8 l_loadResPtrn0[2] = {
    1, -1
};

static s8* l_loadResPtrnList[2] = {
    l_loadResPtrn0,
    l_loadResPtrn0,
};

static daNpcT_faceMotionAnmData_c l_faceMotionAnmData[2] = {
    {-1, J3DFrameCtrl::EMode_NONE, NONE, 16, J3DFrameCtrl::EMode_LOOP, ZELRO, 1},
    {BCK_ZELRO_F_TALK_A, J3DFrameCtrl::EMode_NONE, ZELRO, 16, J3DFrameCtrl::EMode_LOOP, ZELRO, 1},
};

static daNpcT_motionAnmData_c l_motionAnmData[1] = {
    {BCK_ZELRO_WAIT_A, J3DFrameCtrl::EMode_LOOP, ZELRO, BTK_ZELRO, J3DFrameCtrl::EMode_NONE, ZELRO, 1, 0},
};

static daNpcT_MotionSeqMngr_c::sequenceStepData_c l_faceMotionSequenceData[8] = {
    {1, -1, 1}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {0, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
};

static daNpcT_MotionSeqMngr_c::sequenceStepData_c l_motionSequenceData[4] = {
    {0, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
};

char* daNpc_ZelRo_c::mCutNameList = "";

daNpc_ZelRo_c::cutFunc daNpc_ZelRo_c::mCutList[1] = {
    NULL
};

daNpc_ZelRo_c::~daNpc_ZelRo_c() {
    if (heap != NULL) {
        mpMorf[0]->stopZelAnime();
    }

    deleteRes(l_loadResPtrnList[mType], (const char**)l_resNameList);
}

const daNpc_ZelRo_HIOParam daNpc_ZelRo_Param_c::m = {
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

cPhs__Step daNpc_ZelRo_c::create() {
    daNpcT_ct(this, daNpc_ZelRo_c, l_faceMotionAnmData, l_motionAnmData, l_faceMotionSequenceData, 4, l_motionSequenceData, 4, l_evtList, l_resNameList);

    mType = getType();
    mFlowNodeNo = getFlowNodeNo();
    mTwilight = false;

    cPhs__Step phase = (cPhs__Step)loadRes(l_loadResPtrnList[mType], (const char**)l_resNameList);
    if (phase == cPhs_COMPLEATE_e) {
        if (!fopAcM_entrySolidHeap(this, createHeapCallBack, 0x71C0)) {
            return cPhs_ERROR_e;
        }

        OS_REPORT("\t(%s:%d) flowNo:%d, PathNo:%02x<%08x> ", fopAcM_getProcNameString(this), mType, mFlowNodeNo,
                  (getPath() >> 32) & 0xFF, fopAcM_GetParam(this));
        
        if (isDelete()) {
            OS_REPORT("===>isDelete:TRUE\n");
            return cPhs_ERROR_e;
        }

        OS_REPORT("\n");


        fopAcM_SetMtx(this, mpMorf[0]->getModel()->getBaseTRMtx());
        fopAcM_setCullSizeBox(this, -300.0f, -50.0f, -300.0f, 300.0f, 450.0f, 300.0f);
        fopAcM_OnStatus(this, fopAcM_STATUS_UNK_0x8000000);

        mSound.init(&current.pos, &eyePos, 3, 1);

        mAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1, &mAcchCir, fopAcM_GetSpeed_p(this),
                  fopAcM_GetAngle_p(this), fopAcM_GetShapeAngle_p(this));

        mAcch.CrrPos(dComIfG_Bgsp());
        mGndChk = mAcch.m_gnd;
        mGroundH = mAcch.GetGroundH();

        setEnvTevColor();
        setRoomNo();

        mCcStts.Init(daNpc_ZelRo_Param_c::m.common.weight, 0, this);
        mCyl.Set(mCcDCyl);
        mCyl.SetStts(&mCcStts);
        mCyl.SetTgHitCallback(tgHitCallBack);

        reset();
        mCreating = true;
        Execute();
        mCreating = false;
    }

    return phase;
}

int daNpc_ZelRo_c::CreateHeap() {
    int bmdIdx = mTwilight == true ? NONE : NONE;
    J3DModelData* mdlData_p = (J3DModelData*)dComIfG_getObjectRes(l_resNameList[l_bmdData[bmdIdx][1]], l_bmdData[bmdIdx][0]);

    JUT_ASSERT(432, NULL != mdlData_p);

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
    model->setUserArea((u32)this);

    for (int i = 0; i < 2; i++) {
        mpMatAnm[i] = new daNpcT_MatAnm_c();
        if (mpMatAnm[i] == NULL) {
            return 0;
        }
    }

    if (setFaceMotionAnm(1, false) && setMotionAnm(0, 0.0f, FALSE)) {
        return 1;
    }

    return 0;
}

int daNpc_ZelRo_c::Delete() {
    this->~daNpc_ZelRo_c();
    return 1;
}

int daNpc_ZelRo_c::Execute() {
    return execute();
}

int daNpc_ZelRo_c::Draw() {
    J3DModelData* mdlData_p = mpMorf[0]->getModel()->getModelData();
    
    if (mpMatAnm[0] != NULL) {
        mdlData_p->getMaterialNodePointer(getEyeballLMaterialNo())->setMaterialAnm(mpMatAnm[0]);
    }

    if (mpMatAnm[1] != NULL) {
        mdlData_p->getMaterialNodePointer(getEyeballRMaterialNo())->setMaterialAnm(mpMatAnm[1]);
    }

    return draw(
#if DEBUG
        chkAction(&daNpc_ZelRo_c::test),
#else
        FALSE,
#endif
        TRUE, mRealShadowSize, NULL, 100.0f, FALSE, FALSE, FALSE
    );
}

int daNpc_ZelRo_c::createHeapCallBack(fopAc_ac_c* a_this) {
    return static_cast<daNpc_ZelRo_c*>(a_this)->CreateHeap();
}

int daNpc_ZelRo_c::ctrlJointCallBack(J3DJoint* i_joint, int param_2) {
    if (param_2 == 0) {
        J3DModel* model = j3dSys.getModel();
        daNpc_ZelRo_c* i_this = (daNpc_ZelRo_c*)model->getUserArea();

        if (i_this != NULL) {
            i_this->ctrlJoint(i_joint, model);
        }
    }

    return 1;
}

u8 daNpc_ZelRo_c::getType() {
    switch (fopAcM_GetParam(this) & 0xFF) {
        case 0:
            return TYPE_0;

        default:
            return TYPE_1;
    }
}

int daNpc_ZelRo_c::getFlowNodeNo() {
    u16 rv = home.angle.x;

    if (rv == 0xFFFF) {
        return -1;
    }

    return rv;
}

u8 daNpc_ZelRo_c::getPath() {
    return (fopAcM_GetParam(this) & 0xFF00) >> 8;
}

BOOL daNpc_ZelRo_c::isDelete() {
    if (mType == TYPE_1) {
        return FALSE;
    }

    switch (mType) {
        case TYPE_0:
            /* dSv_event_flag_c::M_012 - Cutscene - [cutscene: 7] Meet Princess Zelda at castle */
            return daNpcT_chkEvtBit(45) == FALSE;

        default:
            return TRUE;
    }
}

void daNpc_ZelRo_c::reset() {
    initialize();

    memset(&mNextAction, 0, (u8*)&field_0xfc4 - (u8*)&mNextAction);

    for (int i = 0; i < 2; i++) {
        if (mpMatAnm[i] != NULL) {
            mpMatAnm[i]->initialize();
        }
    }

    if (getPath() != 0xFF) {
        mPath.initialize();
        mPath.setPathInfo(getPath(), fopAcM_GetRoomNo(this), 0);
    }

    setAngle(home.angle.y);
}

void daNpc_ZelRo_c::afterJntAnm(int param_1) {
    if (param_1 == 1) {
        mDoMtx_stack_c::YrotM(mStagger.getAngleZ(1));
        mDoMtx_stack_c::ZrotM(-mStagger.getAngleX(1));
    } else if (param_1 == 4) {
        mDoMtx_stack_c::YrotM(mStagger.getAngleZ(0));
        mDoMtx_stack_c::ZrotM(-mStagger.getAngleX(0));
    }
}

BOOL daNpc_ZelRo_c::ctrlBtk() {
    if (mpMatAnm[0] != NULL && mpMatAnm[1] != NULL) {
        if (field_0xe29 != 0 && mBtkAnm.getBtkAnm() != NULL) {
            mpMatAnm[0]->setNowOffsetX(cM_ssin(mJntAnm.getEyeAngleY()) * 0.2f * -1.0f);
            mpMatAnm[0]->setNowOffsetY(cM_ssin(mJntAnm.getEyeAngleX()) * 0.2f);
            mpMatAnm[1]->setNowOffsetX(cM_ssin(mJntAnm.getEyeAngleY()) * 0.2f);
            mpMatAnm[1]->setNowOffsetY(cM_ssin(mJntAnm.getEyeAngleX()) * 0.2f);
            mpMatAnm[0]->onEyeMoveFlag();
            mpMatAnm[1]->onEyeMoveFlag();
            return TRUE;
        }

        if (field_0xe2a != 0) {
            mpMatAnm[0]->setMorfFrm(field_0xe2a);
            mpMatAnm[1]->setMorfFrm(field_0xe2a);
            field_0xe2a = 0;
        }

        mpMatAnm[0]->offEyeMoveFlag();
        mpMatAnm[1]->offEyeMoveFlag();
    }

    return FALSE;
}

void daNpc_ZelRo_c::setParam() {
    selectAction();
    srchActors();

    s16 talk_distance = daNpc_ZelRo_Param_c::m.common.talk_distance;
    s16 talk_angle = daNpc_ZelRo_Param_c::m.common.talk_angle;

    attention_info.distances[fopAc_attn_LOCK_e] = daNpcT_getDistTableIdx(daNpc_ZelRo_Param_c::m.common.attention_distance, daNpc_ZelRo_Param_c::m.common.attention_angle);
    attention_info.distances[fopAc_attn_TALK_e] = attention_info.distances[fopAc_attn_LOCK_e];
    attention_info.distances[fopAc_attn_SPEAK_e] = daNpcT_getDistTableIdx(talk_distance, talk_angle);
    attention_info.flags = fopAc_AttnFlag_SPEAK_e | fopAc_AttnFlag_TALK_e;

    scale.set(daNpc_ZelRo_Param_c::m.common.scale, daNpc_ZelRo_Param_c::m.common.scale, daNpc_ZelRo_Param_c::m.common.scale);
    mCcStts.SetWeight(daNpc_ZelRo_Param_c::m.common.weight);
    mCylH = daNpc_ZelRo_Param_c::m.common.height;
    mWallR = daNpc_ZelRo_Param_c::m.common.width;
    mAcchCir.SetWallR(mWallR);
    mAcchCir.SetWallH(daNpc_ZelRo_Param_c::m.common.knee_length);

    mRealShadowSize = daNpc_ZelRo_Param_c::m.common.real_shadow_size;
    gravity = daNpc_ZelRo_Param_c::m.common.gravity;
    mExpressionMorfFrame = daNpc_ZelRo_Param_c::m.common.expression_morf_frame;
    mMorfFrames = daNpc_ZelRo_Param_c::m.common.morf_frame;
}

void daNpc_ZelRo_c::setAfterTalkMotion() {
    mFaceMotionSeqMngr.getNo();
    mFaceMotionSeqMngr.setNo(FACE_NONE, -1.0f, FALSE, 0);
}

void daNpc_ZelRo_c::srchActors() {
    /* empty function */
}

BOOL daNpc_ZelRo_c::evtTalk() {
    if (chkAction(&daNpc_ZelRo_c::talk)) {
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
        }

        setAction(&daNpc_ZelRo_c::talk);
    }

    return TRUE;
}

BOOL daNpc_ZelRo_c::evtCutProc() {
    BOOL rv = FALSE;
    int staffId = dComIfGp_getEventManager().getMyStaffId("ZelRo", this, -1);

    if (staffId != -1) {
        mStaffId = staffId;
        int actIdx = dComIfGp_getEventManager().getMyActIdx(mStaffId, &mCutNameList, 1, 0, 0);
        if ((this->*mCutList[actIdx])(mStaffId)) {
            dComIfGp_getEventManager().cutEnd(mStaffId);
        }

        rv = TRUE;
    }

    return rv;
}

void daNpc_ZelRo_c::action() {
    fopAc_ac_c* actor_p = hitChk(&mCyl, 0xFFFFFFFF);

    if (actor_p != NULL) {
        mStagger.setParam(this, actor_p, mCurAngle.y);
        setDamage(0, 1, 0);
        mStagger.setPower(0.0f);
        mDamageTimerStart = 0;
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

void daNpc_ZelRo_c::beforeMove() {
    if (checkHide() || mNoDraw) {
        attention_info.flags = 0;
    }
}

void daNpc_ZelRo_c::setAttnPos() {
    cXyz sp48(10.0f, -30.0f, 0.0f);
    
    mStagger.calc(FALSE);
    mJntAnm.setParam(this, mpMorf[0]->getModel(), &sp48, getBackboneJointNo(), getNeckJointNo(), getHeadJointNo(),
                     daNpc_ZelRo_Param_c::m.common.body_angleX_min, daNpc_ZelRo_Param_c::m.common.body_angleX_max,
                     daNpc_ZelRo_Param_c::m.common.body_angleY_min, daNpc_ZelRo_Param_c::m.common.body_angleY_max,
                     daNpc_ZelRo_Param_c::m.common.head_angleX_min, daNpc_ZelRo_Param_c::m.common.head_angleX_max,
                     daNpc_ZelRo_Param_c::m.common.head_angleY_min, daNpc_ZelRo_Param_c::m.common.head_angleY_max,
                     daNpc_ZelRo_Param_c::m.common.neck_rotation_ratio, 0.0f, NULL);
    mJntAnm.calcJntRad(0.2f, 1.0f, cM_s2rad((s16)(mCurAngle.y - field_0xd7e.y)));

    setMtx();
    mDoMtx_stack_c::copy(mpMorf[0]->getModel()->getAnmMtx(getHeadJointNo()));
    mDoMtx_stack_c::multVec(&sp48, &eyePos);
    mJntAnm.setEyeAngleX(eyePos, 1.0f, 0);
    mJntAnm.setEyeAngleY(eyePos, mCurAngle.y, FALSE, 1.0f, 0);
    attention_info.position = current.pos;
    attention_info.position.y += daNpc_ZelRo_Param_c::m.common.attention_offset;
}

void daNpc_ZelRo_c::setCollision() {
    cXyz pos;

    if (!mHide) {
        if (mTwilight == true && !dComIfGp_event_runCheck()) {
            mCyl.SetCoSPrm(0x69);
        } else {
            mCyl.SetCoSPrm(0x79);
        }

        if (mStagger.checkStagger()) {
            mCyl.SetTgType(0);
            mCyl.SetTgSPrm(0);
        } else {
            mCyl.SetTgType(0xD8FBFDFF);
            mCyl.SetTgSPrm(0x1F);
            mCyl.OnTgNoHitMark();
        }

        f32 cylH = mCylH;
        f32 wallR = mWallR;
        pos = current.pos;
        mCyl.SetH(cylH);
        mCyl.SetR(wallR);
        mCyl.SetC(pos);
        dComIfG_Ccsp()->Set(&mCyl);
    }

    mCyl.ClrCoHit();
    mCyl.ClrTgHit();
}

int daNpc_ZelRo_c::drawDbgInfo() {
    return 0;
}

BOOL daNpc_ZelRo_c::selectAction() {
    mNextAction = NULL;
    mNextAction = &daNpc_ZelRo_c::wait;
    return TRUE;
}

BOOL daNpc_ZelRo_c::chkAction(actionFunc action) {
    return mAction == action;
}

BOOL daNpc_ZelRo_c::setAction(actionFunc action) {
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

int daNpc_ZelRo_c::wait(void* param_1) {
    switch (mMode) {
        case MODE_ENTER:
        case MODE_INIT:
            mFaceMotionSeqMngr.setNo(FACE_NONE, -1.0f, FALSE, 0);
            mMotionSeqMngr.setNo(MOT_WAIT_A, -1.0f, FALSE, 0);
            mMode = MODE_RUN;
            // fallthrough
        case MODE_RUN:
            if (!mStagger.checkStagger()) {
                BOOL bVar1;
                if (srchPlayerActor()) {
                    mJntAnm.lookPlayer(0);
                    bVar1 = checkStep();
                } else {
                    mJntAnm.lookNone(0);
                    bVar1 = home.angle.y != mCurAngle.y ? true : false;
                }

                if (bVar1 && step(home.angle.y, -1, -1, 15, 0)) {
                    mMode = MODE_INIT;
                }
            }
            break;

        case MODE_EXIT:
            break;
    }

    return 1;
}

int daNpc_ZelRo_c::talk(void* param_1) {
    BOOL bVar1 = false;

    switch (mMode) {
        case MODE_ENTER:
        case MODE_INIT:
            initTalk(mFlowNodeNo, NULL);
            mMode = MODE_RUN;
            // fallthrough
        case MODE_RUN:
            if (mTwilight) {
                bVar1 = true;
            } else {
                mJntAnm.lookPlayer(0);

                if (mCurAngle.y != fopAcM_searchPlayerAngleY(this)) {
                    step(fopAcM_searchPlayerAngleY(this), -1, -1, 15, 0);
                } else {
                    bVar1 = true;
                }
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

int daNpc_ZelRo_c::test(void* param_1) {
    int rt = 0;
    return rt;
}

static int daNpc_ZelRo_Create(void* a_this) {
    return static_cast<daNpc_ZelRo_c*>(a_this)->create();
}

static int daNpc_ZelRo_Delete(void* a_this) {
    return static_cast<daNpc_ZelRo_c*>(a_this)->Delete();
}

static int daNpc_ZelRo_Execute(void* a_this) {
    return static_cast<daNpc_ZelRo_c*>(a_this)->Execute();
}

static int daNpc_ZelRo_Draw(void* a_this) {
    return static_cast<daNpc_ZelRo_c*>(a_this)->Draw();
}

static int daNpc_ZelRo_IsDelete(void* a_this) {
    return 1;
}

static daNpc_ZelRo_Param_c l_HIO;

static actor_method_class daNpc_ZelRo_MethodTable = {
    (process_method_func)daNpc_ZelRo_Create,
    (process_method_func)daNpc_ZelRo_Delete,
    (process_method_func)daNpc_ZelRo_Execute,
    (process_method_func)daNpc_ZelRo_IsDelete,
    (process_method_func)daNpc_ZelRo_Draw,
};

actor_process_profile_definition g_profile_NPC_ZELRO = {
  fpcLy_CURRENT_e,          // mLayerID
  7,                        // mListID
  fpcPi_CURRENT_e,          // mListPrio
  PROC_NPC_ZELRO,           // mProcName
  &g_fpcLf_Method.base,    // sub_method
  sizeof(daNpc_ZelRo_c),    // mSize
  0,                        // mSizeOther
  0,                        // mParameters
  &g_fopAc_Method.base,     // sub_method
  386,                      // mPriority
  &daNpc_ZelRo_MethodTable, // sub_method
  0x00044108,               // mStatus
  fopAc_NPC_e,              // mActorType
  fopAc_CULLBOX_CUSTOM_e,   // cullType
};
