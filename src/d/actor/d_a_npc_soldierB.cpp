/**
 * @file d_a_npc_soldierB.cpp
 *
 */

#include "d/dolzel_rel.h"  // IWYU pragma: keep

#include "d/actor/d_a_npc_soldierB.h"
#include "d/actor/d_a_npc.h"
#include "d/actor/d_a_npc_soldierA.h"
#include "Z2AudioLib/Z2Instances.h"

enum Joint {
    /* 0x00 */ JNT_CENTER,
    /* 0x01 */ JNT_BACKBONE1,
    /* 0x02 */ JNT_BACKBONE2,
    /* 0x03 */ JNT_NECK,
    /* 0x04 */ JNT_HEAD,
    /* 0x05 */ JNT_SHOULDERL,
    /* 0x06 */ JNT_ARM1L,
    /* 0x07 */ JNT_ARM2L,
    /* 0x08 */ JNT_HANDL,
    /* 0x09 */ JNT_FINGERL,
    /* 0x0A */ JNT_THUMBL,
    /* 0x0B */ JNT_SHOULDERR,
    /* 0x0C */ JNT_ARM1R,
    /* 0x0D */ JNT_ARM2R,
    /* 0x0E */ JNT_HANDR,
    /* 0x0F */ JNT_FINGERR,
    /* 0x10 */ JNT_THUMBR,
    /* 0x11 */ JNT_WAIST,
    /* 0x12 */ JNT_LEG1L,
    /* 0x13 */ JNT_LEG2L,
    /* 0x14 */ JNT_FOOTL,
    /* 0x15 */ JNT_LEG1R,
    /* 0x16 */ JNT_LEG2R,
    /* 0x17 */ JNT_FOOTR,
    /* 0x18 */ JNT_SAYA
};

enum Animation {
    /* 0x0 */ ANM_NONE,
    /* 0x1 */ ANM_WAIT_A,
    /* 0x2 */ ANM_FEAR_WAIT,
    /* 0x3 */ ANM_FEAR_TALK,
    /* 0x4 */ ANM_WAIT,
    /* 0x5 */ ANM_TALK_A,
    /* 0x6 */ ANM_TALK_B,
};

enum Motion {
    /* 0x0 */ MOT_WAIT,
    /* 0x1 */ MOT_TALK_B,
    /* 0x2 */ MOT_TALK_A,
    /* 0x3 */ MOT_FEAR_WAIT,
    /* 0x4 */ MOT_FEAR_TALK,
    /* 0x5 */ MOT_WAIT_A,
};

enum Event {
    /* 0x0 */ EVT_NONE,
    /* 0x1 */ EVT_TALK_LAKE,
};

enum Event_Cut_Nums {
    /* 0x3 */ NUM_EVT_CUTS_e = 0x3,
};

static NPC_SOLDIERB_HIO_CLASS l_HIO;

static daNpc_GetParam1 l_bmdGetParamList[2] = {{10, 0}, {9, 0}};

static daNpc_GetParam1 l_bckGetParamList[4] = {
    {-1, 0},
    {4, 0},
    {6, 0},
    {5, 0},
};

static daNpc_GetParam1 l_evtGetParamList[1] = {
    {0, 0},
};

static char* l_evtNames[1] = {
    NULL,
};

static int l_loadRes_SOLDIERba[3] = {
    0,
    -1,
    -1,
};

static int l_loadRes_SOLDIERb0[3] = {
    0,
    -1,
    -1,
};

static int* l_loadRes_list[2] = {
    l_loadRes_SOLDIERba,
    l_loadRes_SOLDIERb0,
};

static char* l_resNames[1] = {"chtSolB"};

static char* l_myName = "chtSolB";

char* daNpc_SoldierB_c::mEvtCutNameList[2] = {
    "",
    "LISTEN_LAKE",
};

daNpc_SoldierB_c::cutFunc daNpc_SoldierB_c::mEvtCutList[2] = {
    NULL,
    &daNpc_SoldierB_c::ECut_listenLake,
};

daNpc_SoldierB_c::daNpc_SoldierB_c() {}

daNpc_SoldierB_c::~daNpc_SoldierB_c() {
    for (int i = 0; l_loadRes_list[mType][i] >= 0; i++) {
        dComIfG_resDelete(&mPhases[i], l_resNames[l_loadRes_list[mType][i]]);
    }

    if (heap != NULL) {
        mAnm_p->stopZelAnime();
    }
}

daNpc_SoldierB_HIOParam const daNpc_SoldierB_Param_c::m = {
    30.0f,    // attention_offset
    -4.0f,    // gravity
    1.0f,     // scale
    600.0f,   // real_shadow_size
    255.0f,   // weight
    240.0f,   // height
    35.0f,    // knee_length
    45.0f,    // width
    0.0f,     // body_angleX_max
    0.0f,     // body_angleX_min
    10.0f,    // body_angleY_max
    -10.0f,   // body_angleY_min
    30.0f,    // head_angleX_max
    -30.0f,   // head_angleX_min
    45.0f,    // head_angleY_max
    -45.0f,   // head_angleY_min
    0.6f,     // neck_rotation_ratio
    12.0f,    // morf_frame
    5,        // talk_distance
    6,        // talk_angle
    7,        // attention_distance
    6,        // attention_angle
    180.0f,   // fov
    500.0f,   // search_distance
    300.0f,   // search_height
    -300.0f,  // search_depth
    60,       // attention_time
    8,        // damage_time
    0,        // face_expression
    0,        // motion
    0,        // look_mode
    false,    // debug_mode_ON
    false,    // debug_info_ON
};

cPhs__Step daNpc_SoldierB_c::create() {
    mTwilight = dKy_darkworld_check();

    fopAcM_ct(this, daNpc_SoldierB_c);

    mType = getTypeFromParam();

    if (home.angle.x != 0xFFFF) {
        mMsgNo = home.angle.x;
    } else {
        mMsgNo = -1;
    }

    if (isDelete()) {
        return cPhs_ERROR_e;
    }

    int res_count = 0;
    int i = 0;
    for (; l_loadRes_list[mType][i] >= 0; i++) {
        cPhs__Step phase =
            (cPhs__Step)dComIfG_resLoad(&mPhases[i], l_resNames[l_loadRes_list[mType][i]]);
        if (phase == cPhs_ERROR_e || phase == cPhs_UNK3_e) {
            return cPhs_ERROR_e;
        }

        if (phase == cPhs_COMPLEATE_e) {
            res_count++;
        }
    }

    if (res_count == i) {
        if (!fopAcM_entrySolidHeap(this, createHeapCallBack, 0x2CE0)) {
            return cPhs_ERROR_e;
        }

        J3DModelData* modelData = mAnm_p->getModel()->getModelData();
        fopAcM_SetMtx(this, mAnm_p->getModel()->getBaseTRMtx());
        fopAcM_setCullSizeBox(this, -300.0f, -50.0f, -300.0f, 300.0f, 450.0f, 300.0f);
        mSound.init(&current.pos, &eyePos, 3, 1);

        mAcchCir.SetWall(mHIO->m.common.width, mHIO->m.common.knee_length);
        mAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1, &mAcchCir,
                  fopAcM_GetSpeed_p(this), fopAcM_GetAngle_p(this), fopAcM_GetShapeAngle_p(this));
        mCcStts.Init(mHIO->m.common.weight, 0, this);
        mCyl.Set(mCcDCyl);
        mCyl.SetStts(&mCcStts);
        mCyl.SetTgHitCallback(tgHitCallBack);
        mAcch.CrrPos(dComIfG_Bgsp());
        mGndChk = mAcch.m_gnd;
        mGroundH = mAcch.GetGroundH();

        setEnvTevColor();
        setRoomNo();
        reset();
        Execute();

        return cPhs_COMPLEATE_e;
    }

    return cPhs_INIT_e;
}

int daNpc_SoldierB_c::CreateHeap() {
    J3DModelData* mdlData_p = NULL;
    J3DModel* model = NULL;

    if (l_bmdGetParamList[0].fileIdx >= 0) {
        mdlData_p = (J3DModelData*)dComIfG_getObjectRes(l_resNames[l_bmdGetParamList[0].arcIdx],
                                                        l_bmdGetParamList[0].fileIdx);
    }

    u32 uVar1 = 0x11020284;
    mAnm_p = new mDoExt_McaMorfSO(mdlData_p, NULL, NULL, NULL, -1, 1.0f, 0, -1, &mSound,
                                  J3DMdlFlag_DifferedDLBuffer, uVar1);
    if (mAnm_p != NULL && mAnm_p->getModel() == NULL) {
        mAnm_p->stopZelAnime();
        mAnm_p = NULL;
    }

    if (mAnm_p == NULL) {
        return 0;
    }

    model = mAnm_p->getModel();
    for (u16 i = 0; i < mdlData_p->getJointNum(); i++) {
        mdlData_p->getJointNodePointer(i)->setCallBack(ctrlJointCallBack);
    }
    model->setUserArea((uintptr_t)this);

    setMotionAnm(ANM_FEAR_WAIT, 0.0f);

    int arcIdx = 0;
    arcIdx = l_bmdGetParamList[1].arcIdx;
    mdlData_p =
        (J3DModelData*)dComIfG_getObjectRes(l_resNames[arcIdx], l_bmdGetParamList[1].fileIdx);
    if (mdlData_p != NULL) {
        mSpearModel = mDoExt_J3DModel__create(mdlData_p, J3DMdlFlag_DifferedDLBuffer, 0x11000084);
        if (mSpearModel == NULL) {
            return 0;
        }
    } else {
        return 0;
    }

    return 1;
}

int daNpc_SoldierB_c::Delete() {
    fpc_ProcID id = fopAcM_GetID(this);
    this->~daNpc_SoldierB_c();
    return 1;
}

int daNpc_SoldierB_c::Execute() {
    return execute();
}

int daNpc_SoldierB_c::Draw() {
    return draw(chkAction(&daNpc_SoldierB_c::test), FALSE, mHIO->m.common.real_shadow_size, NULL,
                FALSE);
}

int daNpc_SoldierB_c::ctrlJoint(J3DJoint* i_joint, J3DModel* i_model) {
    int jntNo = i_joint->getJntNo();
    int i_jointList[3] = {JNT_BACKBONE1, JNT_NECK, JNT_HEAD};

    if (jntNo == JNT_CENTER) {
        mDoMtx_stack_c::copy(mAnm_p->getModel()->getAnmMtx(JNT_BACKBONE1));
        mDoMtx_stack_c::multVecZero(&mLookatPos[0]);
        mDoMtx_stack_c::copy(mAnm_p->getModel()->getAnmMtx(JNT_NECK));
        mDoMtx_stack_c::multVecZero(&mLookatPos[1]);
        mDoMtx_stack_c::copy(mAnm_p->getModel()->getAnmMtx(JNT_HEAD));
        mDoMtx_stack_c::multVecZero(&mLookatPos[2]);
    }

    mDoMtx_stack_c::copy(i_model->getAnmMtx(jntNo));

    switch (jntNo) {
    case JNT_BACKBONE1:
    case JNT_NECK:
    case JNT_HEAD:
        setLookatMtx(jntNo, i_jointList, mHIO->m.common.neck_rotation_ratio);
        break;
    }

    if (jntNo == JNT_BACKBONE1) {
        mDoMtx_stack_c::YrotM(-field_0x908[0].z);
        mDoMtx_stack_c::ZrotM(field_0x908[0].x);
    } else if (jntNo == JNT_HEAD) {
        mDoMtx_stack_c::YrotM(-field_0x908[2].z);
        mDoMtx_stack_c::ZrotM(field_0x908[2].x);
    }

    i_model->setAnmMtx(jntNo, mDoMtx_stack_c::get());
    MTXCopy(mDoMtx_stack_c::get(), J3DSys::mCurrentMtx);

    return 1;
}

int daNpc_SoldierB_c::createHeapCallBack(fopAc_ac_c* i_this) {
    daNpc_SoldierB_c* actor = (daNpc_SoldierB_c*)i_this;
    return actor->CreateHeap();
}

int daNpc_SoldierB_c::ctrlJointCallBack(J3DJoint* i_joint, int param_2) {
    if (param_2 == 0) {
        J3DModel* model = j3dSys.getModel();
        daNpc_SoldierB_c* actor = (daNpc_SoldierB_c*)model->getUserArea();
        if (actor != NULL) {
            actor->ctrlJoint(i_joint, model);
        }
    }

    return 1;
}

void daNpc_SoldierB_c::setParam() {
    actionFunc action = mNextAction;

    selectAction();

    if (mNextAction != action) {
        /* Bug??? - Loop never actually runs */
        for (int i = 3; i < 3; i++) {
            mActorMngrs[i].initialize();
        }
    }

    field_0xe00 = 0;
    field_0xe04 = 0;

    s16 talk_distance, talk_angle, attention_distance, attention_angle;
    attention_distance = 3;
    attention_angle = 6;
    talk_distance = 5;
    talk_angle = 6;

    attention_info.distances[fopAc_attn_LOCK_e] = getDistTableIdx(talk_distance, talk_angle);
    attention_info.distances[fopAc_attn_TALK_e] = attention_info.distances[fopAc_attn_LOCK_e];
    attention_info.distances[fopAc_attn_SPEAK_e] =
        getDistTableIdx(attention_distance, attention_angle);
    attention_info.flags = 0;
    scale.set(mHIO->m.common.scale, mHIO->m.common.scale, mHIO->m.common.scale);
    mAcchCir.SetWallR(mHIO->m.common.width);
    mAcchCir.SetWallH(mHIO->m.common.knee_length);

    if (!dComIfGp_event_runCheck()) {
        gravity = mHIO->m.common.gravity;
    }
}

BOOL daNpc_SoldierB_c::main() {
    if (!doEvent()) {
        doNormalAction(1);
    }

    if (checkHide()) {
        attention_info.flags = 0;
    }

    if (!mHIO->m.common.debug_mode_ON) {
        if (!dComIfGp_event_runCheck() || (mOrderNewEvt && dComIfGp_getEvent().isOrderOK())) {
            if (mOrderEvtNo != EVT_NONE) {
                eventInfo.setArchiveName(l_resNames[l_evtGetParamList[mOrderEvtNo].arcIdx]);
            }

            orderEvent(mOrderSpeakEvt, l_evtNames[l_evtGetParamList[mOrderEvtNo].fileIdx], 0xFFFF,
                       0x28, 0xFF, 1);
        }
    }

    if (field_0x9ee) {
        mExpressionMorfOverride = 0.0f;
        mMotionMorfOverride = 0.0f;
        field_0x9ee = false;
    }

    playMotion();

    return TRUE;
}

BOOL daNpc_SoldierB_c::ctrlBtk() {
    return FALSE;
}

void daNpc_SoldierB_c::setAttnPos() {
    static cXyz eyeOffset(0.0f, 20.0f, 0.0f);
    cXyz sp74, sp80, sp8c, sp98;
    f32 attn_offset = mHIO->m.common.attention_offset;

    mDoMtx_stack_c::YrotS(field_0x990);
    cLib_addCalc2(&field_0x984[0], 0.0f, 0.1f, 125.0f);
    cLib_addCalc2(&field_0x984[2], 0.0f, 0.1f, 125.0f);

    for (int i = 0; i < 3; i++) {
        sp8c.set(0.0f, 0.0f, field_0x984[i] * cM_ssin(field_0x992));
        mDoMtx_stack_c::multVec(&sp8c, &sp98);
        field_0x908[i].x = -sp98.z;
        field_0x908[i].z = -sp98.x;
    }

    cLib_chaseS(&field_0x992, 0, 0x555);

    if (mLookMode == LOOK_RESET) {
        for (int i = 0; i < 3; i++) {
            mLookatAngle[i].setall(0);
        }
    }

    setMtx();
    lookat();

    mDoMtx_stack_c::copy(mAnm_p->getModel()->getAnmMtx(JNT_HEAD));
    mDoMtx_stack_c::multVecZero(&mHeadPos);
    mDoMtx_stack_c::multVec(&eyeOffset, &eyePos);
    mDoMtx_stack_c::multVec(&eyeOffset, &sp8c);
    mHeadAngle.x = cLib_targetAngleX(&mHeadPos, &sp8c);
    mHeadAngle.y = cLib_targetAngleY(&mHeadPos, &sp8c);

    cXyz* attn_pos = mLookat.getAttnPos();
    if (attn_pos != NULL) {
        sp80 = *attn_pos - eyePos;
        mEyeAngle.y = -(mLookatAngle[2].y + mCurAngle.y);
        mEyeAngle.y += cM_atan2s(sp80.x, sp80.z);
        mEyeAngle.x = -cM_atan2s(sp80.y, sp80.absXZ());
        mEyeAngle.x += mHeadAngle.x;
    } else {
        mEyeAngle.y = 0;
        mEyeAngle.x = 0;
    }

    f32 fVar2 = 0.0f;
    f32 fVar3 = 0.0f;
    f32 fVar4 = 0.0f;
    sp80.set(0.0f, 0.0f, 0.0f);
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::ZXYrotM(mCurAngle.x, mCurAngle.y, mCurAngle.z);
    mDoMtx_stack_c::multVec(&sp80, &sp74);
    attention_info.position.set(mHeadPos.x, mHeadPos.y + attn_offset, mHeadPos.z);

    if (!mHide) {
        if (!mIsDamaged) {
            mCyl.SetTgType(0xD8FBFDFF);
            mCyl.SetTgSPrm(0x1F);
            mCyl.OnTgNoHitMark();
        } else {
            mCyl.SetTgType(0);
            mCyl.SetTgSPrm(0);
        }

        mCyl.SetC(sp74);
        mCyl.SetH(mHIO->m.common.height + fVar3);
        mCyl.SetR(mHIO->m.common.width + fVar4);
        dComIfG_Ccsp()->Set(&mCyl);
    }

    mCyl.ClrTgHit();
}

void daNpc_SoldierB_c::setMotionAnm(int i_idx, f32 i_morf) {
    J3DAnmTransformKey* anm = NULL;
    int attr = J3DFrameCtrl::EMode_LOOP;

    mAnmFlags &= ~ANM_MOTION_FLAGS;

    if (l_bckGetParamList[i_idx].fileIdx >= 0) {
        anm = getTrnsfrmKeyAnmP(l_resNames[l_bckGetParamList[i_idx].arcIdx],
                                l_bckGetParamList[i_idx].fileIdx);
    }

    mAnm = i_idx;

    switch (i_idx) {
    case ANM_WAIT_A:
    case ANM_FEAR_WAIT:
        break;

    case ANM_FEAR_TALK:
        attr = J3DFrameCtrl::EMode_NONE;
        break;

    default:
        anm = NULL;
        break;
    }

    if (anm != NULL && setMcaMorfAnm(anm, 1.0f, i_morf, attr, 0, -1)) {
        mAnmFlags |= ANM_PLAY_MORF | ANM_PAUSE_MORF;
        mMotionLoops = 0;
    }
}

void daNpc_SoldierB_c::setMotion(int i_motion, f32 i_morf, int param_3) {
    s16 motion = i_motion;
    if ((param_3 != 0 || mMotion != motion) && i_motion >= 0 && i_motion < 3) {
        mMotion = motion;
        mMotionMorfOverride = i_morf;
        mMotionPrevPhase = -1;
        mMotionPhase = 0;
    }
}

int daNpc_SoldierB_c::drawDbgInfo() {
    return 0;
}

void daNpc_SoldierB_c::drawOtherMdls() {
    g_env_light.setLightTevColorType_MAJI(mSpearModel, &tevStr);
    mDoMtx_stack_c::copy(mAnm_p->getModel()->getAnmMtx(JNT_HANDR));
    mDoMtx_stack_c::scaleM(scale);
    mSpearModel->setBaseTRMtx(mDoMtx_stack_c::get());
    J3DModelData* mdlData_p = mSpearModel->getModelData();
    fopAcM_setEffectMtx(this, mdlData_p);
    mDoExt_modelUpdateDL(mSpearModel);
    dComIfGd_addRealShadow(mShadowKey, mSpearModel);
}

u8 daNpc_SoldierB_c::getTypeFromParam() {
    u8 param = fopAcM_GetParam(this) & 0xFF;
    switch (param) {
    case 0:
        return 0;
    default:
        return 1;
    }
}

BOOL daNpc_SoldierB_c::isDelete() {
    if (mType == 0 || mType == 1) {
        return FALSE;
    }

    return TRUE;
}

void daNpc_SoldierB_c::reset() {
    initialize();
    mLookat.initialize();
    for (int i = 0; i < 3; i++) {
        mActorMngrs[i].initialize();
    }

    mNextAction = NULL;
    mAction = NULL;
    field_0xe00 = 0;
    field_0xe04 = 0;
    field_0xe08 = 0;
    field_0xe0c = 0;
    mLookMode = -1;
    mMode = 0;
    current.pos = home.pos;
    old.pos = current.pos;
    current.angle.set(0, home.angle.y, 0);
    old.angle = current.angle;
    shape_angle = current.angle;
    mCurAngle = current.angle;
    mOldAngle = mCurAngle;
    speedF = 0.0f;
    speed.setall(0.0f);
    field_0x9ee = true;
}

void daNpc_SoldierB_c::playMotion() {
    daNpcF_anmPlayData dat0 = {ANM_FEAR_WAIT, mHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat0[1] = {&dat0};
    daNpcF_anmPlayData dat1a = {ANM_FEAR_TALK, mHIO->m.common.morf_frame, 1};
    daNpcF_anmPlayData dat1b = {ANM_FEAR_WAIT, 0.0f, 0};
    daNpcF_anmPlayData* pDat1[2] = {&dat1a, &dat1b};
    daNpcF_anmPlayData dat2 = {ANM_WAIT_A, mHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat2[1] = {&dat2};

    daNpcF_anmPlayData** ppDat[3] = {pDat0, pDat1, pDat2};

    if (mMotion >= 0 && mMotion < 3) {
        playMotionAnm(ppDat);
    }
}

BOOL daNpc_SoldierB_c::chkAction(actionFunc action) {
    return mAction == action;
}

BOOL daNpc_SoldierB_c::setAction(actionFunc action) {
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

BOOL daNpc_SoldierB_c::selectAction() {
    mNextAction = NULL;

    if (mHIO->m.common.debug_mode_ON) {
        mNextAction = &daNpc_SoldierB_c::test;
    } else {
        mNextAction = &daNpc_SoldierB_c::wait;
    }

    return TRUE;
}

void daNpc_SoldierB_c::doNormalAction(int param_1) {
    if (param_1 != 0 && hitChk2(&mCyl, TRUE, FALSE)) {
        int damage_time =
            mCutType == daPy_py_c::CUT_TYPE_TURN_RIGHT ? 20 : mHIO->m.common.damage_time;
        setDamage(damage_time, -1, 0);
        setLookMode(LOOK_RESET);
    } else if (mIsDamaged && mDamageTimer == 0) {
        mMode = 0;
        mIsDamaged = false;
    }

    mOrderEvtNo = EVT_NONE;
    if (mNextAction) {
        if (mAction == mNextAction) {
            (this->*mAction)(NULL);
        } else {
            setAction(mNextAction);
        }
    }
}

BOOL daNpc_SoldierB_c::doEvent() {
    BOOL rv = FALSE;

    if (dComIfGp_event_runCheck() != FALSE) {
        dEvent_manager_c& event_manager = dComIfGp_getEventManager();

        if (eventInfo.checkCommandTalk() || eventInfo.checkCommandDemoAccrpt()) {
            mOrderNewEvt = false;
        }

        if (eventInfo.checkCommandTalk()) {
            if (chkAction(&daNpc_SoldierB_c::talk)) {
                (this->*mAction)(NULL);
            } else if (dComIfGp_event_chkTalkXY() == FALSE || dComIfGp_evmng_ChkPresentEnd()) {
                setAction(&daNpc_SoldierB_c::talk);
            }

            rv = TRUE;
        } else {
            int staffId = event_manager.getMyStaffId(l_myName, NULL, 0);
            if (staffId != -1) {
                mStaffID = staffId;
                int evtCutNo = event_manager.getMyActIdx(staffId, mEvtCutNameList, 2, FALSE, FALSE);

                if ((this->*mEvtCutList[evtCutNo])(staffId)) {
                    event_manager.cutEnd(staffId);
                }

                rv = TRUE;
            }

            if (eventInfo.checkCommandDemoAccrpt() && mEventIdx != -1 &&
                event_manager.endCheck(mEventIdx))
            {
                dComIfGp_event_reset();
                mOrderEvtNo = EVT_NONE;
                mEventIdx = -1;
            }
        }

        int i_expression, i_motion;
        int msgTimer = mMsgTimer;
        if (ctrlMsgAnm(i_expression, i_motion, this, FALSE)) {
            if (!field_0x9eb) {
                setMotion(i_motion, -1.0f, 0);
            }
        }
    } else {
        mMsgTimer = 0;

        if (mStaffID != -1) {
            mAction = NULL;
            mStaffID = -1;
        }
    }

    return rv;
}

void daNpc_SoldierB_c::setLookMode(int i_lookMode) {
    if (i_lookMode >= 0 && i_lookMode < 6 && i_lookMode != mLookMode) {
        mLookMode = i_lookMode;
    }
}

void daNpc_SoldierB_c::lookat() {
    daPy_py_c* player = NULL;
    J3DModel* model = mAnm_p->getModel();
    BOOL i_snap = FALSE;
    f32 body_angleX_min = mHIO->m.common.body_angleX_min;
    f32 body_angleX_max = mHIO->m.common.body_angleX_max;
    f32 body_angleY_min = mHIO->m.common.body_angleY_min;
    f32 body_angleY_max = mHIO->m.common.body_angleY_max;
    f32 head_angleX_min = mHIO->m.common.head_angleX_min;
    f32 head_angleX_max = mHIO->m.common.head_angleX_max;
    f32 head_angleY_min = mHIO->m.common.head_angleY_min;
    f32 head_angleY_max = mHIO->m.common.head_angleY_max;
    s16 angle_delta = mCurAngle.y - mOldAngle.y;
    cXyz lookatPos[3] = {mLookatPos[0], mLookatPos[1], mLookatPos[2]};
    csXyz* lookatAngle[3] = {&mLookatAngle[0], &mLookatAngle[1], &mLookatAngle[2]};
    cXyz spe8;

    switch (mLookMode) {
    case LOOK_NONE:
        break;

    case LOOK_RESET:
        i_snap = TRUE;
        break;

    case LOOK_PLAYER:
    case LOOK_PLAYER_TALK:
        player = daPy_getPlayerActorClass();

        if (mLookMode == LOOK_PLAYER_TALK) {
            head_angleY_min = -80.0f;
            head_angleY_max = 80.0f;
        }
        break;

    case LOOK_ACTOR:
        player = (daPy_py_c*)mActorMngrs[1].getActorP();
        break;

    case LOOK_ATTN:
        player = (daPy_py_c*)mActorMngrs[2].getActorP();
        break;
    }

    if (player != NULL) {
        mLookPos = player->attention_info.position;

        if (mLookMode != LOOK_PLAYER && mLookMode != LOOK_PLAYER_TALK && mLookMode != LOOK_ACTOR) {
            mLookPos.y -= 40.0f;
        }

        mLookat.setAttnPos(&mLookPos);
    } else {
        mLookat.setAttnPos(NULL);
    }

    mLookat.setParam(body_angleX_min, body_angleX_max, body_angleY_min, body_angleY_max, 0.0f, 0.0f,
                     0.0f, 0.0f, head_angleX_min, head_angleX_max, head_angleY_min, head_angleY_max,
                     mCurAngle.y, lookatPos);
    mLookat.calc(this, model->getBaseTRMtx(), lookatAngle, i_snap, angle_delta, FALSE);
}

static void* s_sub(void* i_actor, void* i_data) {
    if (fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_NPC_SOLDIERa &&
        ((daNpc_SoldierA_c*)i_actor)->getType() == 0)
    {
        return i_actor;
    }

    return NULL;
}

int daNpc_SoldierB_c::wait(void* param_1) {
    switch (mMode) {
    case 0:
        setMotion(MOT_WAIT, -1.0f, 0);
        mTurnMode = 0;
        field_0x9ea = true;
        mMode = 2;
        // fallthrough
    case 2:
        if (mActorMngrs[2].getActorP() == NULL) {
            fopAc_ac_c* actor_p = (fopAc_ac_c*)fpcM_Search(s_sub, this);
            if (actor_p != NULL) {
                mActorMngrs[2].entry(actor_p);
                setLookMode(LOOK_ATTN);
            }
        }
        break;

    case 3:
        break;
    }

    return 1;
}

int daNpc_SoldierB_c::talk(void* param_1) {
    int rv = 0;
    BOOL unkFlag = FALSE;

    switch (mMode) {
    case 0: {
        if (mIsDamaged) {
            break;
        }

        int msgNo = mMsgNo;
        mOrderSpeakEvt = false;
        initTalk(msgNo, NULL);
        mTurnMode = 0;
        mMode = 2;
        // fallthrough
    }
    case 2:
        if (field_0x9ea) {
            unkFlag = TRUE;
        } else {
            setLookMode(LOOK_PLAYER_TALK);
            mActorMngrs[0].entry(daPy_getPlayerActorClass());

            if (mCurAngle.y == fopAcM_searchPlayerAngleY(this)) {
                unkFlag = TRUE;
            } else if (step(fopAcM_searchPlayerAngleY(this), -1, -1, 15)) {
                setMotion(MOT_WAIT, -1.0f, 0);
                mTurnMode = 0;
            }
        }

        if (unkFlag && talkProc(NULL, TRUE, NULL)) {
            rv = 1;
        }

        if (rv) {
            mMode = 3;

            if (!field_0x9ec) {
                dComIfGp_event_reset();
            }

            field_0x9ec = false;
        }
        break;

    case 3:
        break;
    }

    return rv;
}

int daNpc_SoldierB_c::ECut_listenLake(int i_staffId) {
    dEvent_manager_c& eventManager = dComIfGp_getEventManager();
    daPy_py_c* player = daPy_getPlayerActorClass();
    int rv = 0;
    int* piVar1 = NULL;
    int prm = -1;

    piVar1 = dComIfGp_evmng_getMyIntegerP(i_staffId, "prm");
    if (piVar1 != NULL) {
        prm = *piVar1;
    }

    if (eventManager.getIsAddvance(i_staffId)) {
        switch (prm) {
        case 0:
            if (mActorMngrs[2].getActorP() == NULL) {
                fopAc_ac_c* actor_p = (fopAc_ac_c*)fpcM_Search(s_sub, this);
                if (actor_p != NULL) {
                    mActorMngrs[2].entry(actor_p);
                    setLookMode(LOOK_ATTN);
                }
            }
            break;

        case 10:
            setMotion(MOT_TALK_B, -1.0f, 0);
            break;

        case 20:
            break;
        }
    }

    switch (prm) {
    case 0:
        rv = 1;
        break;

    case 10:
        if (mMotionPhase > 0) {
            rv = 1;
        }
        break;

    case 20:
        rv = 1;
        break;

    default:
        rv = 1;
        break;
    }

    return rv;
}

int daNpc_SoldierB_c::test(void* param_1) {
    switch (mMode) {
    case 0:
        speedF = 0.0f;
        speed.setall(0.0f);
        mMode = 2;
        // fallthrough
    case 2:
        setMotion(mHIO->m.common.motion, mHIO->m.common.morf_frame, 0);
        setLookMode(mHIO->m.common.look_mode);
        mOrderEvtNo = EVT_NONE;
        attention_info.flags = 0;
        break;

    case 3:
        break;
    }

    return 1;
}

static int daNpc_SoldierB_Create(void* i_this) {
    return static_cast<daNpc_SoldierB_c*>(i_this)->create();
}

static int daNpc_SoldierB_Delete(void* i_this) {
    return static_cast<daNpc_SoldierB_c*>(i_this)->Delete();
}

static int daNpc_SoldierB_Execute(void* i_this) {
    return static_cast<daNpc_SoldierB_c*>(i_this)->Execute();
}

static int daNpc_SoldierB_Draw(void* i_this) {
    return static_cast<daNpc_SoldierB_c*>(i_this)->Draw();
}

static int daNpc_SoldierB_IsDelete(void* i_this) {
    return 1;
}

static actor_method_class daNpc_SoldierB_MethodTable = {
    (process_method_func)daNpc_SoldierB_Create,  (process_method_func)daNpc_SoldierB_Delete,
    (process_method_func)daNpc_SoldierB_Execute, (process_method_func)daNpc_SoldierB_IsDelete,
    (process_method_func)daNpc_SoldierB_Draw,
};

extern actor_process_profile_definition g_profile_NPC_SOLDIERb = {
    fpcLy_CURRENT_e,              // mLayerID
    7,                            // mListID
    fpcPi_CURRENT_e,              // mListPrio
    PROC_NPC_SOLDIERb,            // mProcName
    &g_fpcLf_Method.base,         // sub_method
    sizeof(daNpc_SoldierB_c),     // mSize
    0,                            // mSizeOther
    0,                            // mParameters
    &g_fopAc_Method.base,         // sub_method
    400,                          // mPriority
    &daNpc_SoldierB_MethodTable,  // sub_method
    0x00044100,                   // mStatus
    fopAc_NPC_e,                  // mActorType
    fopAc_CULLBOX_CUSTOM_e,       // cullType
};

AUDIO_INSTANCES;
