/**
 * @file d_a_npc_zanb.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_npc_zanb.h"

enum zanB_RES_File_ID {
    /* BCK */
    /* 0x5 */ BCK_ZANB_SIT = 0x5,
    /* 0x6 */ BCK_ZANB_WAIT_A,

    /* BMDR */
    /* 0x9 */ BMDR_ZANB = 0x9,

    /* BTK */
    /* 0xC */ BTK_ZANB = 0xC,
};

enum RES_Name {
    /* 0x0 */ NONE,
    /* 0x1 */ ZANB,
};

enum Face_Motion {
    /* 0x0 */ FACE_MOT_NONE,
};

enum Motion {
    /* 0x0 */ MOT_WAIT_A,
    /* 0x1 */ MOT_SIT,
};

static int l_bmdData[1][2] = {
    {9, ZANB},
};

static daNpcT_evtData_c l_evtList[2] = {
    {"", 0},
    {"NO_RESPONSE", 0},
};

static char* l_resNameList[2] = {
    "",
    "zanB",
};

static s8 l_loadResPtrn0[2] = {
    1, -1,
};

static s8* l_loadResPtrnList[2] = {
    l_loadResPtrn0, l_loadResPtrn0,
};

static daNpcT_faceMotionAnmData_c l_faceMotionAnmData[1] = {
    {-1, J3DFrameCtrl::EMode_NONE, NONE, -1, J3DFrameCtrl::EMode_NONE, 0, 0},
};

static daNpcT_motionAnmData_c l_motionAnmData[2] = {
    {BCK_ZANB_WAIT_A, J3DFrameCtrl::EMode_LOOP, ZANB, BTK_ZANB, J3DFrameCtrl::EMode_NONE, ZANB, 1, 0},
    {BCK_ZANB_SIT, J3DFrameCtrl::EMode_LOOP, ZANB, BTK_ZANB, J3DFrameCtrl::EMode_NONE, ZANB, 1, 0},
};

static daNpcT_MotionSeqMngr_c::sequenceStepData_c l_faceMotionSequenceData[4] = {
    {0, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
};

static daNpcT_MotionSeqMngr_c::sequenceStepData_c l_motionSequenceData[8] = {
    {0, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {1, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
};

char* daNpc_zanB_c::mCutNameList[1] = {""};

daNpc_zanB_c::cutFunc daNpc_zanB_c::mCutList[1] = {NULL};

daNpc_zanB_c::~daNpc_zanB_c() {
    OS_REPORT("|%06d:%x|daNpc_zanB_c -> デストラクト\n", g_Counter.mCounter0, this);

    if (mpMorf[0] != NULL) {
        mpMorf[0]->stopZelAnime();
    }

    #if DEBUG
    if (mHIO != NULL) {
        mHIO->removeHIO();
    }
    #endif

    deleteRes(l_loadResPtrnList[mType], (const char**)l_resNameList);
}

daNpc_zanB_HIOParam const daNpc_zanB_Param_c::m = {
    270.0f,
    -3.0f,
    1.0f,
    500.0f,
    255.0f,
    250.0f,
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

static NPC_ZANB_HIO_CLASS l_HIO;

cPhs__Step daNpc_zanB_c::create() {
    daNpcT_ct(this, daNpc_zanB_c, l_faceMotionAnmData,
                       l_motionAnmData, l_faceMotionSequenceData, 4,
                       l_motionSequenceData, 4, l_evtList, l_resNameList);

    mType = getType();
    mFlowNodeNo = getFlowNodeNo();
    mTwilight = false;

    cPhs__Step phase = (cPhs__Step)loadRes(l_loadResPtrnList[mType], (const char**)l_resNameList);
    if (phase == cPhs_COMPLEATE_e) {
        OS_REPORT("\t(%s:%d) flowNo:%d<%08x> ", fopAcM_getProcNameString(this), mType, mFlowNodeNo, fopAcM_GetParam(this));

        if (isDelete()) {
            OS_REPORT("===>isDelete:TRUE\n");
            return cPhs_ERROR_e;
        }

        OS_REPORT("\n");

        static int const heapSize[2] = {
            0x6E80, 0x6E80,
        };

        if (!fopAcM_entrySolidHeap(this, createHeapCallBack, heapSize[mType])) {
            return cPhs_ERROR_e;
        }

        J3DModelData* mdlData_p = mpMorf[0]->getModel()->getModelData();
        fopAcM_SetMtx(this, mpMorf[0]->getModel()->getBaseTRMtx());
        fopAcM_setCullSizeBox(this, -200.0f, -100.0f, -200.0f, 200.0f, 300.0f, 200.0f);
        fopAcM_OnStatus(this, fopAcM_STATUS_UNK_0x8000000);
        mSound.init(&current.pos, &eyePos, 3, 1);

        #if DEBUG
        mHIO = &l_HIO;
        mHIO->entryHIO("顔つきザント");
        #endif

        reset();

        mAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1,
                        &mAcchCir, fopAcM_GetSpeed_p(this), fopAcM_GetAngle_p(this),
                        fopAcM_GetShapeAngle_p(this));
        mCcStts.Init(mHIO->m.common.weight, 0, this);
        mCyl.Set(mCcDCyl);
        mCyl.SetStts(&mCcStts);
        mCyl.SetTgHitCallback(tgHitCallBack);

        if (mType == 0) {
            mAcch.SetGrndNone();
            mAcch.SetWallNone();
        }
        
        mAcch.CrrPos(dComIfG_Bgsp());
        mGndChk = mAcch.m_gnd;
        mGroundH = mAcch.GetGroundH();
        if (mGroundH != -G_CM3D_F_INF) {
            setEnvTevColor();
            setRoomNo();
        }

        mCreating = 1;
        Execute();
        mCreating = 0;
    }

    return phase;
}

int daNpc_zanB_c::CreateHeap() {
    J3DModelData* mdlData_p = static_cast<J3DModelData*>(dComIfG_getObjectRes(l_resNameList[l_bmdData[0][1]], l_bmdData[0][0]));
    if (mdlData_p == NULL) {
        return 0;
    }

    mpMorf[0] = new mDoExt_McaMorfSO(mdlData_p, NULL, NULL, NULL, -1, 1.0f, 0, -1, &mSound, J3DMdlFlag_DifferedDLBuffer, 0x11020284);
    if (mpMorf[0] == NULL || mpMorf[0]->getModel() == NULL) {
        return 0;
    }

    J3DModel* mdl_p = mpMorf[0]->getModel();
    for (u16 i = 0; i < mdlData_p->getJointNum(); i++) {
        mdlData_p->getJointNodePointer(i)->setCallBack(ctrlJointCallBack);
    }
    mdl_p->setUserArea((uintptr_t)this);

    mpMatAnm[0] = new daNpcT_MatAnm_c();
    if (mpMatAnm[0] == NULL) {
        return 0;
    }

    if (setFaceMotionAnm(0, false) && setMotionAnm(0, 0.0f, FALSE)) {
        return 1;
    }

    return 0;
}

int daNpc_zanB_c::Delete() {
    OS_REPORT("|%06d:%x|daNpc_zanB_c -> Delete\n", g_Counter.mCounter0, this);
    fpc_ProcID id = fopAcM_GetID(this);
    this->~daNpc_zanB_c();
    return 1;
}

int daNpc_zanB_c::Execute() {
    return execute();
}

int daNpc_zanB_c::Draw() {
    if (mpMatAnm[0] != NULL) {
        J3DModelData* mdlData_p = mpMorf[0]->getModel()->getModelData();
        mdlData_p->getMaterialNodePointer(getEyeballMaterialNo())->setMaterialAnm(mpMatAnm[0]);
    }

    return draw(
#if DEBUG
        chkAction(&daNpc_zanB_c::test),
#else
        FALSE,
#endif
        FALSE, mRealShadowSize, NULL, 0.0f, TRUE, FALSE, FALSE
    );
}

int daNpc_zanB_c::createHeapCallBack(fopAc_ac_c* i_this) {
    return static_cast<daNpc_zanB_c*>(i_this)->CreateHeap();
}

int daNpc_zanB_c::ctrlJointCallBack(J3DJoint* i_joint, int param_2) {
    if (param_2 == 0) {
        J3DModel* model = j3dSys.getModel();
        daNpc_zanB_c* i_this = (daNpc_zanB_c*)model->getUserArea();
        if (i_this != 0) {
            i_this->ctrlJoint(i_joint, model);
        }
    }

    return 1;
}

u8 daNpc_zanB_c::getType() {
    switch (fopAcM_GetParam(this) & 0xFF) {
        case 0:
            return 0;

        default:
            return 1;
    }
}

BOOL daNpc_zanB_c::isDelete() {
    return FALSE;
}

void daNpc_zanB_c::reset() {
    csXyz angle;
    int size = (u8*)&field_0xf9c - (u8*)&mNextAction;

    if (mpMatAnm[0] != NULL) {
        mpMatAnm[0]->initialize();
    }

    initialize();

    memset(&mNextAction, 0, size);

    angle.setall(0);
    angle.y = home.angle.y;
    setAngle(angle);
}

void daNpc_zanB_c::afterJntAnm(int i_joint) {
    if (i_joint == 1) {
        mDoMtx_stack_c::YrotM(mStagger.getAngleZ(1));
        mDoMtx_stack_c::ZrotM(-mStagger.getAngleX(1));
    } else if (i_joint == 3) {
        mDoMtx_stack_c::YrotM(-mStagger.getAngleZ(0));
        mDoMtx_stack_c::ZrotM(mStagger.getAngleX(0));
    }
}

BOOL daNpc_zanB_c::checkChangeEvt() {
    if (!chkAction(&daNpc_zanB_c::talk)) {
        mPreItemNo = 0;

        if (dComIfGp_event_chkTalkXY()) {
            if (dComIfGp_evmng_ChkPresentEnd()) {
                mEvtNo = 1;
                evtChange();
            }

            return TRUE;
        }
    }

    return FALSE;
}

void daNpc_zanB_c::setParam() {
    selectAction();
    srchActors();

    s16 talk_distance = mHIO->m.common.talk_distance;
    s16 talk_angle = mHIO->m.common.talk_angle;
    s16 attention_distance = mHIO->m.common.attention_distance;
    s16 attention_angle = mHIO->m.common.attention_angle;

    attention_info.distances[fopAc_attn_LOCK_e] = daNpcT_getDistTableIdx(attention_distance, attention_angle);
    attention_info.distances[fopAc_attn_TALK_e] = attention_info.distances[fopAc_attn_LOCK_e];
    attention_info.distances[fopAc_attn_SPEAK_e] = daNpcT_getDistTableIdx(talk_distance, talk_angle);
    attention_info.flags = fopAc_AttnFlag_SPEAK_e | fopAc_AttnFlag_TALK_e;

    scale.set(mHIO->m.common.scale, mHIO->m.common.scale, mHIO->m.common.scale);
    mCcStts.SetWeight(mHIO->m.common.weight);
    mCylH = mHIO->m.common.height;
    mWallR = mHIO->m.common.width;
    mAttnFovY = mHIO->m.common.fov;
    mAcchCir.SetWallR(mWallR);
    mAcchCir.SetWallH(mHIO->m.common.knee_length);
    mRealShadowSize = mHIO->m.common.real_shadow_size;
    mExpressionMorfFrame = mHIO->m.common.expression_morf_frame;
    mMorfFrames = mHIO->m.common.morf_frame;
    gravity = mHIO->m.common.gravity;

    if (mType == 0) {
        mAcch.SetGrndNone();
        mAcch.SetWallNone();
        gravity = 0.0f;
    }
}

void daNpc_zanB_c::setAfterTalkMotion() {
    int face_motion = mFaceMotionSeqMngr.getNo();
    mFaceMotionSeqMngr.setNo(FACE_MOT_NONE, -1.0f, FALSE, 0);
}

void daNpc_zanB_c::srchActors() {
    /* empty function */
}

BOOL daNpc_zanB_c::evtTalk() {
    if (chkAction(&daNpc_zanB_c::talk)) {
        (this->*mAction)(NULL);
    } else {
        setAction(&daNpc_zanB_c::talk);
    }

    return TRUE;
}

BOOL daNpc_zanB_c::evtCutProc() {
    s32 staff_id = dComIfGp_getEventManager().getMyStaffId("zanB", this, -1);
    if (staff_id != -1) {
        mStaffId = staff_id;
        int act_idx = dComIfGp_getEventManager().getMyActIdx(mStaffId, mCutNameList, 1, FALSE, FALSE);

        if ((this->*mCutList[act_idx])(mStaffId)) {
            dComIfGp_getEventManager().cutEnd(mStaffId);
        }

        return TRUE;
    }

    return FALSE;
}

void daNpc_zanB_c::action() {
    if (mStagger.checkRebirth()) {
        mStagger.initialize();
        mMode = 1;
    }

    if (mNextAction) {
        if (mAction == mNextAction) {
            (this->*mAction)(NULL);
        } else {
            setAction(mNextAction);
        }
    }
}

void daNpc_zanB_c::beforeMove() {
    if (checkHide() || mNoDraw) {
        attention_info.flags = 0;
    }
}

void daNpc_zanB_c::setAttnPos() {
    cXyz pos(20.0f, 30.0f, 0.0f);

    mStagger.calc(0);
    f32 rad = cM_s2rad(mCurAngle.y - field_0xd7e.y);

    mJntAnm.setParam(this, mpMorf[0]->getModel(), &pos, getBackboneJointNo(),
                     getNeckJointNo(), getHeadJointNo(), mHIO->m.common.body_angleX_min,
                     mHIO->m.common.body_angleX_max, mHIO->m.common.body_angleY_min,
                     mHIO->m.common.body_angleY_max, mHIO->m.common.head_angleX_min,
                     mHIO->m.common.head_angleX_max, mHIO->m.common.head_angleY_min,
                     mHIO->m.common.head_angleY_max, mHIO->m.common.neck_rotation_ratio, 0.0f,
                     NULL);
    mJntAnm.calcJntRad(0.2f, 1.0f, rad);
    setMtx();

    mDoMtx_stack_c::copy(mpMorf[0]->getModel()->getAnmMtx(getHeadJointNo()));
    mDoMtx_stack_c::multVec(&pos, &eyePos);

    mJntAnm.setEyeAngleX(eyePos, 1.0f, 0);
    mJntAnm.setEyeAngleY(eyePos, mCurAngle.y, 0, 1.0f, 0);

    pos.set(0.0f, 0.0f, 0.0f);
    pos.y = mHIO->m.common.attention_offset;

    if (mType == 0) {
        pos.set(0.0f, 220.0f, -40.0f);
    }

    mDoMtx_stack_c::YrotS(mCurAngle.y);
    mDoMtx_stack_c::multVec(&pos, &pos);
    attention_info.position = pos + current.pos;
}

void daNpc_zanB_c::setCollision() {
    cXyz center;

    if (!mHide) {
        u32 tg_type = 0xD8FBFDFF;
        u32 tg_s_prm = 0x1F;

        if (dComIfGp_event_runCheck()) {
            tg_type = 0;
            tg_s_prm = 0;
        } else if (mTwilight) {
            tg_type = 0;
            tg_s_prm = 0;
        } else if (mStagger.checkStagger()) {
            tg_type = 0;
            tg_s_prm = 0;
        }

        mCyl.SetCoSPrm(0x79);
        mCyl.SetTgType(tg_type);
        mCyl.SetTgSPrm(tg_s_prm);
        mCyl.OnTgNoHitMark();

        center.set(0.0f, 0.0f, 0.0f);
        f32 height = mCylH;
        f32 width = mWallR;

        if (mType == 0) {
            center.set(0.0f, 0.0f, -20.0f);
            height = 200.0f;
            width = 60.0f;
        }

        mDoMtx_stack_c::YrotS(mCurAngle.y);
        mDoMtx_stack_c::multVec(&center, &center);
        center += current.pos;

        mCyl.SetH(height);
        mCyl.SetR(width);
        mCyl.SetC(center);
        dComIfG_Ccsp()->Set(&mCyl);
    }

    mCyl.ClrCoHit();
    mCyl.ClrTgHit();
}

int daNpc_zanB_c::drawDbgInfo() {
    return 0;
}

void daNpc_zanB_c::drawGhost() {
    J3DModel* mdl_p = mpMorf[0]->getModel();
    g_env_light.settingTevStruct(3, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mdl_p, &tevStr);
    mpMorf[0]->entryDL();
}

BOOL daNpc_zanB_c::selectAction() {
    mNextAction = NULL;

    #if DEBUG
    if (mHIO->m.common.debug_mode_ON) {
        mNextAction = &daNpc_zanB_c::test;
    } else {
        mNextAction = &daNpc_zanB_c::wait;
    }
    #else
    mNextAction = &daNpc_zanB_c::wait;
    #endif

    return TRUE;
}

BOOL daNpc_zanB_c::chkAction(actionFunc action) {
    return mAction == action;
}

BOOL daNpc_zanB_c::setAction(actionFunc action) {
    mMode = 3;

    if (mAction) {
        (this->*mAction)(NULL);
    }

    mMode = 0;
    mAction = action;

    if (mAction) {
        (this->*mAction)(NULL);
    }

    return TRUE;
}

int daNpc_zanB_c::wait(void* param_1) {
    switch (mMode) {
        case 0:
        case 1:
            if (!mStagger.checkStagger()) {
                if (mType == 0) {
                    mFaceMotionSeqMngr.setNo(FACE_MOT_NONE, -1.0f, FALSE, 0);
                    mMotionSeqMngr.setNo(MOT_SIT, -1.0f, FALSE, 0);
                } else {
                    mFaceMotionSeqMngr.setNo(FACE_MOT_NONE, -1.0f, FALSE, 0);
                    mMotionSeqMngr.setNo(MOT_WAIT_A, -1.0f, FALSE, 0);
                }

                mMode = 2;
            }
            // fallthrough
        case 2:
            if (!mStagger.checkStagger()) {
                if (mType == 0) {
                    mPlayerActorMngr.remove();
                }

                if (mPlayerActorMngr.getActorP() != NULL && !mTwilight) {
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
                        if (field_0xe34 != 0) {
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

                mJntAnm.getMode();
            }
            break;

        case 3:
            break;
    }

    return 1;
}

int daNpc_zanB_c::talk(void* param_1) {
    switch (mMode) {
        case 0:
        case 1:
            if (!mStagger.checkStagger()) {
                initTalk(mFlowNodeNo, NULL);
                mMode = 2;
            }
            // fallthrough
        case 2:
            if (!mStagger.checkStagger()) {
                if (mTwilight || mPlayerAngle == mCurAngle.y || mType == 0) {
                    if (talkProc(NULL, FALSE, NULL, FALSE)) {
                        int event_id;
                        mFlow.getEventId(&event_id);

                        if (mFlow.checkEndFlow()) {
                            mPlayerActorMngr.entry(daPy_getPlayerActorClass());
                            dComIfGp_event_reset();
                            mMode = 3;
                        }
                    }

                    mJntAnm.lookPlayer(0);

                    if (mTwilight || mType == 0) {
                        mJntAnm.lookNone(0);
                    }
                } else {
                    mJntAnm.lookPlayer(0);
                    step(mPlayerAngle, -1, -1, 15, 0);
                }
            }
            break;

        case 3:
            break;
    }

    return 0;
}

int daNpc_zanB_c::test(void* param_1) {
    switch (mMode) {
        case 0:
        case 1:
            speedF = 0.0f;
            speed.setall(0.0f);
            mMode = 2;
            // fallthrough
        case 2:
            mFaceMotionSeqMngr.setNo(mHIO->m.common.face_expression, -1.0f, FALSE, 0);
            mMotionSeqMngr.setNo(mHIO->m.common.motion, -1.0f, FALSE, 0);
            mJntAnm.lookNone(0);
            attention_info.flags = 0;
            break;

        case 3:
            break;
    }

    return 0;
}

static int daNpc_zanB_Create(void* i_this) {
    return static_cast<daNpc_zanB_c*>(i_this)->create();
}

static int daNpc_zanB_Delete(void* i_this) {
    return static_cast<daNpc_zanB_c*>(i_this)->Delete();
}

static int daNpc_zanB_Execute(void* i_this) {
    return static_cast<daNpc_zanB_c*>(i_this)->Execute();
}

static int daNpc_zanB_Draw(void* i_this) {
    return static_cast<daNpc_zanB_c*>(i_this)->Draw();
}

static int daNpc_zanB_IsDelete(void* i_this) {
    return 1;
}

static actor_method_class daNpc_zanB_MethodTable = {
    (process_method_func)daNpc_zanB_Create,
    (process_method_func)daNpc_zanB_Delete,
    (process_method_func)daNpc_zanB_Execute,
    (process_method_func)daNpc_zanB_IsDelete,
    (process_method_func)daNpc_zanB_Draw,
};

extern actor_process_profile_definition g_profile_NPC_ZANB = {
  fpcLy_CURRENT_e,         // mLayerID
  7,                       // mListID
  fpcPi_CURRENT_e,         // mListPrio
  PROC_NPC_ZANB,           // mProcName
  &g_fpcLf_Method.base,   // sub_method
  sizeof(daNpc_zanB_c),    // mSize
  0,                       // mSizeOther
  0,                       // mParameters
  &g_fopAc_Method.base,    // sub_method
  382,                     // mPriority
  &daNpc_zanB_MethodTable, // sub_method
  0x00044107,              // mStatus
  fopAc_NPC_e,             // mActorType
  fopAc_CULLBOX_CUSTOM_e,  // cullType
};
