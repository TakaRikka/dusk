/**
 * @file d_a_npc_grd.cpp
 *
 */

#include "d/dolzel_rel.h"  // IWYU pragma: keep

#include "d/actor/d_a_npc_grd.h"
#include "d/actor/d_a_npc.h"
#include "Z2AudioLib/Z2Instances.h"

enum Animation {
    /* 0x00 */ ANM_UNK_0,
    /* 0x01 */ ANM_UNK_1,
    /* 0x02 */ ANM_UNK_2,
    /* 0x03 */ ANM_UNK_3,
    /* 0x04 */ ANM_UNK_4,
    /* 0x05 */ ANM_UNK_5,
    /* 0x06 */ ANM_UNK_6,
    /* 0x07 */ ANM_UNK_7,
    /* 0x08 */ ANM_UNK_8,
    /* 0x09 */ ANM_UNK_9,
    /* 0x0A */ ANM_UNK_10,
    /* 0x0B */ ANM_UNK_11,
};

enum Expression {
    /* 0x0 */ EXPR_UNK_0 = 0,
    /* 0x3 */ EXPR_UNK_3 = 3,
};

enum Expression_BTP {
    /* 0x0 */ EXPR_BTP_UNK_0,
    /* 0x1 */ EXPR_BTP_UNK_1,
    /* 0x2 */ EXPR_BTP_UNK_2,
    /* 0x3 */ EXPR_BTP_UNK_3,
};

enum Motion {
    /* 0x0 */ MOT_UNK_0 = 0,
    /* 0x1 */ MOT_UNK_1 = 1,
    /* 0x3 */ MOT_UNK_4 = 4,
};

enum Type {
    /* 0x0 */ TYPE_0,
    /* 0x1 */ TYPE_1,
    /* 0x2 */ TYPE_2,
};

static NPC_GRD_HIO_CLASS l_HIO;

static daNpc_GetParam1 l_bmdGetParamList[1] = {
    {11, 0},
};

static daNpc_GetParam1 l_bckGetParamList[12] = {
    {-1, 0}, {6, 0},  {5, 1},  {6, 1}, {4, 1}, {8, 0},
    {7, 0},  {10, 1}, {11, 1}, {8, 1}, {7, 1}, {9, 1},
};

static daNpc_GetParam1 l_btpGetParamList[4] = {
    {17, 0},
    {15, 1},
    {16, 1},
    {14, 1},
};

static daNpc_GetParam1 l_btkGetParamList[1] = {
    {14, 0},
};

static daNpc_GetParam1 l_evtGetParamList[1] = {
    {0, 0},
};

static int l_loadRes_GRD_YELIA[3] = {
    0,
    1,
    -1,
};

static int l_loadRes_GRDa[3] = {
    0,
    1,
    -1,
};

static int l_loadRes_GRD0[3] = {
    0,
    -1,
    -1,
};

static int* l_loadRes_list[3] = {
    l_loadRes_GRD_YELIA,
    l_loadRes_GRDa,
    l_loadRes_GRD0,
};

static char* l_resNames[2] = {
    "grD",
    "grD1",
};

static char* l_evtNames[1] = {
    NULL,
};

static char* l_myName = "grD";

char* daNpc_Grd_c::mEvtCutNameList[2] = {
    "",
    "NOD_TO_GRZ",
};

daNpc_Grd_c::cutFunc daNpc_Grd_c::mEvtCutList[2] = {
    NULL,
    &daNpc_Grd_c::ECut_nodToGrz,
};

daNpc_Grd_c::daNpc_Grd_c() {}

daNpc_Grd_c::~daNpc_Grd_c() {
    for (int i = 0; l_loadRes_list[mType][i] >= 0; i++) {
        dComIfG_resDelete(&mPhase[i], l_resNames[l_loadRes_list[mType][i]]);
    }

    if (heap != NULL) {
        mAnm_p->stopZelAnime();
    }
}

const daNpc_Grd_HIOParam daNpc_Grd_Param_c::m = {
    230.0f,   // attention_offset
    -3.0f,    // gravity
    1.0f,     // scale
    600.0f,   // real_shadow_size
    255.0f,   // weight
    240.0f,   // height
    35.0f,    // knee_length
    100.0f,   // width
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
    0,        // debug_mode_ON
    0,        // debug_info_ON
};

int daNpc_Grd_c::create() {
    fopAcM_ct(this, daNpc_Grd_c);

    mType = getTypeFromParam();

    if (home.angle.x != 0xffff) {
        mFlowID = home.angle.x;
    } else {
        mFlowID = -1;
    }

    if (isDelete()) {
        return cPhs_ERROR_e;
    }

    int phase;
    int i = 0, j = 0;
    for (; l_loadRes_list[mType][j] >= 0; j++) {
        phase = dComIfG_resLoad(&mPhase[j], l_resNames[l_loadRes_list[mType][j]]);
        if (phase == cPhs_ERROR_e || phase == cPhs_UNK3_e) {
            return cPhs_ERROR_e;
        }
        if (phase == cPhs_COMPLEATE_e) {
            i++;
        }
    }

    if (i == j) {
        if (!fopAcM_entrySolidHeap(this, &daNpc_Grd_c::createHeapCallBack, 0x3A50)) {
            return cPhs_ERROR_e;
        }

        fopAcM_SetMtx(this, mAnm_p->getModel()->getBaseTRMtx());
        fopAcM_setCullSizeBox(this, -300.0f, -50.0f, -300.0f, 300.0f, 450.0f, 300.0f);
        mSound.init(&current.pos, &eyePos, 3, 1);

        mAcchCir.SetWall(mpHIO->m.common.width, mpHIO->m.common.knee_length);

        mAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1, &mAcchCir,
                  fopAcM_GetSpeed_p(this), fopAcM_GetAngle_p(this), fopAcM_GetShapeAngle_p(this));
        mCcStts.Init(mpHIO->m.common.weight, 0, this);

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

int daNpc_Grd_c::CreateHeap() {
    J3DModelData* mdlData_p = NULL;

    if (l_bmdGetParamList[0].fileIdx >= 0) {
        mdlData_p = (J3DModelData*)dComIfG_getObjectRes(l_resNames[l_bmdGetParamList[0].arcIdx],
                                                        l_bmdGetParamList[0].fileIdx);
    }

    mAnm_p = new mDoExt_McaMorfSO(mdlData_p, NULL, NULL, NULL, -1, 1.0f, 0, -1, &mSound, 0x80000,
                                  0x11020284);
    if (mAnm_p != NULL && mAnm_p->getModel() == NULL) {
        mAnm_p->stopZelAnime();
        mAnm_p = NULL;
    }

    if (mAnm_p == NULL) {
        return 0;
    }

    J3DModel* model = mAnm_p->getModel();
    for (u16 i = 0; i < mdlData_p->getJointNum(); i++) {
        mdlData_p->getJointNodePointer(i)->setCallBack(ctrlJointCallBack);
    }
    model->setUserArea((uintptr_t)this);

    mpMatAnm = new daNpcF_MatAnm_c();
    if (mpMatAnm == NULL) {
        return false;
    }

    if (!setExpressionAnm(ANM_UNK_1, false)) {
        return 0;
    }

    setMotionAnm(ANM_UNK_5, 0.0f);

    return 1;
}

int daNpc_Grd_c::Delete() {
    this->~daNpc_Grd_c();
    return 1;
}

int daNpc_Grd_c::Execute() {
    return execute();
}

int daNpc_Grd_c::Draw() {
    mAnm_p->getModel()->getModelData()->getMaterialNodePointer(1)->setMaterialAnm(mpMatAnm);
    return draw(chkAction(&daNpc_Grd_c::test), FALSE, mpHIO->m.common.real_shadow_size, NULL,
                FALSE);
}

int daNpc_Grd_c::ctrlJoint(J3DJoint* i_joint, J3DModel* i_model) {
    int joint_no = i_joint->getJntNo();
    int lookat_joints[3] = {1, 3, 4};

    if (joint_no == 0) {
        mDoMtx_stack_c::copy(mAnm_p->getModel()->getAnmMtx(1));
        mDoMtx_stack_c::multVecZero(&mLookatPos[0]);
        mDoMtx_stack_c::copy(mAnm_p->getModel()->getAnmMtx(3));
        mDoMtx_stack_c::multVecZero(&mLookatPos[1]);
        mDoMtx_stack_c::copy(mAnm_p->getModel()->getAnmMtx(4));
        mDoMtx_stack_c::multVecZero(&mLookatPos[2]);
    }

    mDoMtx_stack_c::copy(i_model->getAnmMtx(joint_no));
    switch (joint_no) {
    case 1:
    case 3:
    case 4:
        setLookatMtx(joint_no, lookat_joints, mpHIO->m.common.neck_rotation_ratio);
        break;
    }

    if (joint_no == 1) {
        mDoMtx_stack_c::YrotM(-field_0x908[0].z);
        mDoMtx_stack_c::ZrotM(field_0x908[0].x);
    } else if (joint_no == 4) {
        mDoMtx_stack_c::YrotM(-field_0x908[2].z);
        mDoMtx_stack_c::ZrotM(field_0x908[2].x);
    }

    i_model->setAnmMtx(joint_no, mDoMtx_stack_c::get());
    cMtx_copy(mDoMtx_stack_c::get(), J3DSys::mCurrentMtx);

    if ((joint_no == 4 || joint_no == 7) && (mAnmFlags & ANM_PLAY_BCK)) {
        J3DAnmTransform* bckAnm = mBckAnm.getBckAnm();
        mBckAnm.changeBckOnly(mAnm_p->getAnm());
        mAnm_p->changeAnm(bckAnm);
    }

    return 1;
}

int daNpc_Grd_c::createHeapCallBack(fopAc_ac_c* a_this) {
    daNpc_Grd_c* i_this = (daNpc_Grd_c*)a_this;
    return i_this->CreateHeap();
}

int daNpc_Grd_c::ctrlJointCallBack(J3DJoint* i_joint, int param_2) {
    if (param_2 == 0) {
        J3DModel* model = j3dSys.getModel();
        daNpc_Grd_c* i_this = (daNpc_Grd_c*)model->getUserArea();

        if (i_this != NULL) {
            i_this->ctrlJoint(i_joint, model);
        }
    }

    return 1;
}

void daNpc_Grd_c::setParam() {
    actionFunc action = mNextAction;
    u32 uVar1 = (fopAc_AttnFlag_SPEAK_e | fopAc_AttnFlag_TALK_e);
    selectAction();

    if (!mTwilight && daPy_py_c::checkNowWolf()) {
        uVar1 = 0;
    }

    if (mNextAction != action) {
        for (int i = 3; i < 3; i++) {
            mActorMngr[i].initialize();
        }
    }

    field_0xe00 = 0;
    field_0xe04 = 0;

    s16 talk_distance = mpHIO->m.common.talk_distance;
    s16 talk_angle = mpHIO->m.common.talk_angle;

    attention_info.distances[fopAc_attn_LOCK_e] =
        getDistTableIdx(mpHIO->m.common.attention_distance, mpHIO->m.common.attention_angle);
    attention_info.distances[fopAc_attn_TALK_e] = attention_info.distances[fopAc_attn_LOCK_e];
    attention_info.distances[fopAc_attn_SPEAK_e] = getDistTableIdx(talk_distance, talk_angle);
    attention_info.flags = uVar1;

    scale.set(mpHIO->m.common.scale, mpHIO->m.common.scale, mpHIO->m.common.scale);
    mAcchCir.SetWallR(mpHIO->m.common.width);
    mAcchCir.SetWallH(mpHIO->m.common.knee_length);
    gravity = mpHIO->m.common.gravity;
}

BOOL daNpc_Grd_c::main() {
    if (!doEvent()) {
        doNormalAction(1);
    }

    if (checkHide()) {
        attention_info.flags = 0;
    }

    if (!mpHIO->m.common.debug_mode_ON &&
        (!dComIfGp_event_runCheck() || (mOrderNewEvt && dComIfGp_getEvent().isOrderOK())))
    {
        if (mOrderEvtNo != 0) {
            eventInfo.setArchiveName(l_resNames[l_evtGetParamList[mOrderEvtNo].arcIdx]);
        }

        orderEvent(mOrderSpeakEvt, l_evtNames[l_evtGetParamList[mOrderEvtNo].fileIdx], 0xFFFF, 40,
                   0xFF, 1);
    }

    if (field_0x9ee) {
        mExpressionMorfOverride = 0.0f;
        mMotionMorfOverride = 0.0f;
        field_0x9ee = false;
    }

    playExpression();
    playMotion();

    return TRUE;
}

BOOL daNpc_Grd_c::ctrlBtk() {
    if (mpMatAnm != NULL) {
        J3DAnmTextureSRTKey* btk = NULL;

        if (l_btkGetParamList[0].fileIdx >= 0) {
            btk = getTexSRTKeyAnmP(l_resNames[l_btkGetParamList[0].arcIdx],
                                   l_btkGetParamList[0].fileIdx);
        }

        if (btk == mBtkAnm.getBtkAnm()) {
            mpMatAnm->setNowOffsetX(cM_ssin(mEyeAngle.y) * 0.02f * -1.0f);
            mpMatAnm->setNowOffsetY(cM_ssin(mEyeAngle.x) * 0.02f);
            mpMatAnm->onEyeMoveFlag();

            return TRUE;
        }

        mpMatAnm->offEyeMoveFlag();
    }

    return FALSE;
}

void daNpc_Grd_c::setAttnPos() {
    static cXyz eyeOffset(20.0f, 30.0f, 0.0f);

    cXyz* attnPos = NULL;
    cXyz sp7c, sp88, sp94, spa0;
    f32 attention_offset = mpHIO->m.common.attention_offset;
    f32 fVar1 = 0.0f;
    f32 fVar2 = 0.0f;

    mDoMtx_stack_c::YrotS(field_0x990);
    cLib_addCalc2(&field_0x984[0], 0.0f, 0.1f, 125.0f);
    cLib_addCalc2(&field_0x984[2], 0.0f, 0.1f, 125.0f);

    for (int i = 0; i < 3; i++) {
        sp94.set(0.0f, 0.0f, field_0x984[i] * cM_ssin(field_0x992));
        mDoMtx_stack_c::multVec(&sp94, &spa0);
        field_0x908[i].x = -spa0.z;
        field_0x908[i].z = -spa0.x;
    }

    cLib_chaseS(&field_0x992, 0, 0x555);

    if (mLookMode == LOOK_RESET) {
        for (int i = 0; i < 3; i++) {
            mLookatAngle[i].setall(0);
        }
    }

    J3DModelData* modelData = mAnm_p->getModel()->getModelData();
    for (u16 i = 0; i < modelData->getJointNum(); i++) {
        modelData->getJointNodePointer(i)->setCallBack(ctrlJointCallBack);
    }

    setMtx();
    lookat();

    mDoMtx_stack_c::copy(mAnm_p->getModel()->getAnmMtx(4));
    mDoMtx_stack_c::multVecZero(&mHeadPos);
    mDoMtx_stack_c::multVec(&eyeOffset, &eyePos);
    eyeOffset.x = 0.0f;
    mDoMtx_stack_c::multVec(&eyeOffset, &sp94);

    mHeadAngle.x = cLib_targetAngleX(&mHeadPos, &sp94);
    mHeadAngle.y = cLib_targetAngleY(&mHeadPos, &sp94);

    attnPos = mLookat.getAttnPos();
    if (attnPos != NULL) {
        sp88 = *attnPos - eyePos;
        mEyeAngle.y = -mLookatAngle[2].y - mCurAngle.y;
        mEyeAngle.y += cM_atan2s(sp88.x, sp88.z);
        mEyeAngle.x = -cM_atan2s(sp88.y, sp88.absXZ());
    } else {
        mEyeAngle.y = 0;
        mEyeAngle.x = 0;
    }

    sp88.set(0.0f, 0.0f, 0.0f);
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::ZXYrotM(mCurAngle.x, mCurAngle.y, mCurAngle.z);
    mDoMtx_stack_c::multVec(&sp88, &sp7c);

    attention_info.position.set(sp7c.x, sp7c.y + attention_offset, sp7c.z);

    if (!mHide && !mTwilight) {
        if (!mIsDamaged) {
            mCyl.SetTgType(0xD8FBFDFF);
            mCyl.SetTgSPrm(0x1F);
            mCyl.OnTgNoHitMark();
        } else {
            mCyl.SetTgType(0);
            mCyl.SetTgSPrm(0);
        }

        mCyl.SetC(sp7c);
        mCyl.SetH(mpHIO->m.common.height + fVar1);
        mCyl.SetR(mpHIO->m.common.width + fVar2);
        dComIfG_Ccsp()->Set(&mCyl);
    }

    mCyl.ClrTgHit();
}

bool daNpc_Grd_c::setExpressionAnm(int i_idx, bool i_modify) {
    J3DAnmTransform* anm = NULL;
    int attr = J3DFrameCtrl::EMode_NONE;

    mAnmFlags &= ~ANM_EXPRESSION_FLAGS;

    if (l_bckGetParamList[i_idx].fileIdx >= 0) {
        anm = getTrnsfrmKeyAnmP(l_resNames[l_bckGetParamList[i_idx].arcIdx],
                                l_bckGetParamList[i_idx].fileIdx);
    }

    bool bVar1 = false;
    switch (i_idx) {
    case ANM_UNK_0:
        bVar1 = setExpressionBtp(EXPR_BTP_UNK_0);
        break;
    case ANM_UNK_1:
        bVar1 = setExpressionBtp(EXPR_BTP_UNK_0);
        break;
    case ANM_UNK_2:
        bVar1 = setExpressionBtp(EXPR_BTP_UNK_1);
        break;
    case ANM_UNK_3:
        bVar1 = setExpressionBtp(EXPR_BTP_UNK_2);
        attr = J3DFrameCtrl::EMode_LOOP;
        break;
    case ANM_UNK_4:
        bVar1 = setExpressionBtp(EXPR_BTP_UNK_3);
        break;
    default:
        anm = NULL;
        break;
    }

    if (!bVar1) {
        return false;
    }

    if (anm == NULL) {
        return true;
    }

    if (setBckAnm(anm, 1.0f, attr, 0, -1, i_modify)) {
        mAnmFlags |= ANM_PAUSE_BCK | ANM_PLAY_BCK;
        mExpressionLoops = 0;
        return true;
    }

    return false;
}

bool daNpc_Grd_c::setExpressionBtp(int i_idx) {
    J3DAnmTexPattern* anm = NULL;
    int attr = J3DFrameCtrl::EMode_NONE;

    mAnmFlags &= ~(ANM_PLAY_BTP | ANM_PAUSE_BTP | ANM_FLAG_800);

    if (l_btpGetParamList[i_idx].fileIdx >= 0) {
        anm = getTexPtrnAnmP(l_resNames[l_btpGetParamList[i_idx].arcIdx],
                             l_btpGetParamList[i_idx].fileIdx);
    }

    switch (i_idx) {
    case EXPR_BTP_UNK_0:
        attr = J3DFrameCtrl::EMode_LOOP;
        break;
    case EXPR_BTP_UNK_1:
        break;
    case EXPR_BTP_UNK_2:
        attr = J3DFrameCtrl::EMode_LOOP;
        break;
    case EXPR_BTP_UNK_3:
        break;
    default:
        anm = NULL;
        break;
    }

    if (anm == NULL) {
        return true;
    }

    if (setBtpAnm(anm, mAnm_p->getModel()->getModelData(), 1.0f, attr)) {
        mAnmFlags |= ANM_PAUSE_BTP | ANM_PLAY_BTP;

        if (i_idx == 0) {
            mAnmFlags |= ANM_FLAG_800;
        }

        return true;
    }

    return false;
}

void daNpc_Grd_c::setExpression(int i_expression, f32 i_morf) {
    if (i_expression >= 0 && i_expression < 4) {
        mExpression = i_expression;
        mExpressionMorfOverride = i_morf;
        mExpressionPrevPhase = -1;
        mExpressionPhase = 0;
    }
}

void daNpc_Grd_c::setMotionAnm(int i_idx, f32 i_morf) {
    J3DAnmTransformKey* anm = NULL;
    J3DAnmTextureSRTKey* btk = NULL;
    int attr = J3DFrameCtrl::EMode_LOOP;

    mAnmFlags &= ~ANM_MOTION_FLAGS;

    if (l_bckGetParamList[i_idx].fileIdx >= 0) {
        anm = getTrnsfrmKeyAnmP(l_resNames[l_bckGetParamList[i_idx].arcIdx],
                                l_bckGetParamList[i_idx].fileIdx);
    }

    switch (i_idx) {
    case ANM_UNK_5:
    case ANM_UNK_8:
        break;
    case ANM_UNK_6:
    case ANM_UNK_7:
    case ANM_UNK_9:
    case ANM_UNK_10:
    case ANM_UNK_11:
        attr = J3DFrameCtrl::EMode_NONE;
        break;
    default:
        anm = NULL;
        btk = NULL;
        break;
    }

    if (l_btkGetParamList[0].fileIdx >= 0) {
        btk =
            getTexSRTKeyAnmP(l_resNames[l_btkGetParamList[0].arcIdx], l_btkGetParamList[0].fileIdx);
    }

    if (btk != NULL &&
        setBtkAnm(btk, mAnm_p->getModel()->getModelData(), 1.0f, J3DFrameCtrl::EMode_LOOP))
    {
        mAnmFlags |= ANM_PAUSE_BTK | ANM_PLAY_BTK;
    }

    if (anm != NULL && setMcaMorfAnm(anm, 1.0f, i_morf, attr, 0, -1)) {
        mAnmFlags |= ANM_PLAY_MORF | ANM_PAUSE_MORF;
        mMotionLoops = 0;
    }
}

void daNpc_Grd_c::setMotion(int param_0, f32 param_1, int param_2) {
    s16 var_r31 = param_0;
    if ((param_2 != 0 || mMotion != var_r31) && param_0 >= 0 && param_0 < 6) {
        mMotion = var_r31;
        mMotionMorfOverride = param_1;
        mMotionPrevPhase = -1;
        mMotionPhase = 0;
    }
}

BOOL daNpc_Grd_c::drawDbgInfo() {
    return FALSE;
}

void daNpc_Grd_c::drawOtherMdls() {
    /* empty function */
}

u8 daNpc_Grd_c::getTypeFromParam() {
    switch (fopAcM_GetParam(this) & 0xFF) {
    case 0:
        return TYPE_0;
    }
    return TYPE_1;
}

int daNpc_Grd_c::isDelete() {
    if (mType == TYPE_2 || mType == TYPE_1) {
        return FALSE;
    }
    if (mType == TYPE_0 &&
        /* dSv_event_flag_c::F_0267 - Temple of Time - Temple of Time clear */
        daNpcF_chkEvtBit(0x10B) &&
        /* dSv_event_flag_c::F_288 - [cutscene: ] Ilia gets her memories back */
        !daNpcF_chkEvtBit(0x120))
    {
        return FALSE;
    }

    return TRUE;
}

void daNpc_Grd_c::reset() {
    initialize();
    mpMatAnm->initialize();
    mLookat.initialize();

    for (int i = 0; i < 3; i++) {
        mActorMngr[i].initialize();
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

void daNpc_Grd_c::playExpression() {
    daNpcF_anmPlayData dat0 = {ANM_UNK_1, mpHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat0[1] = {&dat0};
    daNpcF_anmPlayData dat1a = {ANM_UNK_2, mpHIO->m.common.morf_frame, 1};
    daNpcF_anmPlayData dat1b = {ANM_UNK_3, mpHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat1[2] = {&dat1a, &dat1b};
    daNpcF_anmPlayData dat2 = {ANM_UNK_4, mpHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat2[1] = {&dat2};
    daNpcF_anmPlayData dat3 = {ANM_UNK_0, mpHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat3[1] = {&dat3};

    daNpcF_anmPlayData** ppDat[] = {
        pDat0,
        pDat1,
        pDat2,
        pDat3,
    };

    if (mExpression >= 0 && mExpression < 4) {
        playExpressionAnm(ppDat);
    }
}

void daNpc_Grd_c::playMotion() {
    daNpcF_anmPlayData dat0 = {ANM_UNK_5, mpHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat0[1] = {&dat0};
    daNpcF_anmPlayData dat1a = {ANM_UNK_7, mpHIO->m.common.morf_frame, 1};
    daNpcF_anmPlayData dat1b = {ANM_UNK_8, mpHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat1[2] = {&dat1a, &dat1b};
    daNpcF_anmPlayData dat2a = {ANM_UNK_9, mpHIO->m.common.morf_frame, 1};
    daNpcF_anmPlayData dat2b = {ANM_UNK_5, mpHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat2[2] = {&dat2a, &dat2b};
    daNpcF_anmPlayData dat3a = {ANM_UNK_10, mpHIO->m.common.morf_frame, 1};
    daNpcF_anmPlayData dat3b = {ANM_UNK_5, mpHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat3[2] = {&dat3a, &dat3b};
    daNpcF_anmPlayData dat4a = {ANM_UNK_11, mpHIO->m.common.morf_frame, 1};
    daNpcF_anmPlayData dat4b = {ANM_UNK_5, mpHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat4[2] = {&dat4a, &dat4b};
    daNpcF_anmPlayData dat5 = {ANM_UNK_6, 3.0f, 0};
    daNpcF_anmPlayData* pDat5[1] = {&dat5};

    daNpcF_anmPlayData** ppDat[6] = {
        pDat0, pDat1, pDat2, pDat3, pDat4, pDat5,
    };

    if (mMotion >= 0 && mMotion < 6) {
        playMotionAnm(ppDat);
    }
}

int daNpc_Grd_c::chkAction(actionFunc i_action) {
    return mAction == i_action;
}

int daNpc_Grd_c::setAction(actionFunc i_action) {
    mMode = 3;

    if (mAction) {
        (this->*mAction)(NULL);
    }

    mMode = 0;
    mAction = i_action;

    if (mAction) {
        (this->*mAction)(NULL);
    }

    return TRUE;
}

int daNpc_Grd_c::selectAction() {
    mNextAction = NULL;

    if (mpHIO->m.common.debug_mode_ON) {
        mNextAction = &daNpc_Grd_c::test;
    } else {
        mNextAction = &daNpc_Grd_c::wait;
    }

    return TRUE;
}

void daNpc_Grd_c::doNormalAction(int param_1) {
    if (param_1 != 0 && hitChk2(&mCyl, TRUE, FALSE)) {
        int timer;
        if (mCutType == daPy_py_c::CUT_TYPE_TURN_RIGHT) {
            timer = 20;
        } else {
            timer = mpHIO->m.common.damage_time;
        }

        setDamage(timer, 3, 0);
        setLookMode(LOOK_RESET);
    } else if (mIsDamaged && mDamageTimer == 0) {
        mMode = 0;
        mIsDamaged = false;
    }

    mOrderEvtNo = 0;

    if (mNextAction) {
        if (mAction == mNextAction) {
            (this->*mAction)(NULL);
        } else {
            setAction(mNextAction);
        }
    }
}

BOOL daNpc_Grd_c::doEvent() {
    BOOL rv = FALSE;

    if (dComIfGp_event_runCheck() != 0) {
        dEvent_manager_c& eventManager = dComIfGp_getEventManager();

        if (eventInfo.mCommand == 1 || eventInfo.mCommand == 2) {
            mOrderNewEvt = false;
        }

        if (eventInfo.checkCommandTalk()) {
            if (chkAction(&daNpc_Grd_c::talk)) {
                (this->*mAction)(NULL);
            } else if (dComIfGp_event_chkTalkXY() == 0 || dComIfGp_evmng_ChkPresentEnd()) {
                setAction(&daNpc_Grd_c::talk);
            }

            rv = TRUE;
        } else {
            int staffId = eventManager.getMyStaffId(l_myName, NULL, 0);
            if (staffId != -1) {
                mStaffID = staffId;

                int evtCutNo = eventManager.getMyActIdx(staffId, &mEvtCutNameList[0], 2, 0, 0);

                if ((this->*mEvtCutList[evtCutNo])(staffId)) {
                    eventManager.cutEnd(staffId);
                }

                rv = TRUE;
            }

            if (eventInfo.checkCommandDemoAccrpt() && mEventIdx != -1 &&
                eventManager.endCheck(mEventIdx) != 0)
            {
                dComIfGp_event_reset();
                mOrderEvtNo = 0;
                mEventIdx = -1;
            }
        }

        int msg_timer, expression, motion;
        msg_timer = mMsgTimer;

        if (ctrlMsgAnm(expression, motion, this, FALSE) != 0) {
            if (!field_0x9eb) {
                setExpression(expression, -1.0f);
                setMotion(motion, -1.0f, 0);
            }
        } else if (msg_timer != 0 && !field_0x9eb) {
            setExpressionTalkAfter();
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

void daNpc_Grd_c::setLookMode(int i_lookMode) {
    if (i_lookMode >= 0 && i_lookMode < 6 && i_lookMode != mLookMode) {
        mLookMode = i_lookMode;
    }
}

void daNpc_Grd_c::lookat() {
    daPy_py_c* player = NULL;
    J3DModel* model = mAnm_p->getModel();
    BOOL snap = FALSE;
    f32 body_angleX_min = mpHIO->m.common.body_angleX_min;
    f32 body_angleX_max = mpHIO->m.common.body_angleX_max;
    f32 body_angleY_min = mpHIO->m.common.body_angleY_min;
    f32 body_angleY_max = mpHIO->m.common.body_angleY_max;
    f32 head_angleX_min = mpHIO->m.common.head_angleX_min;
    f32 head_angleX_max = mpHIO->m.common.head_angleX_max;
    f32 head_angleY_min = mpHIO->m.common.head_angleY_min;
    f32 head_angleY_max = mpHIO->m.common.head_angleY_max;
    s16 angle_delta = mCurAngle.y - mOldAngle.y;
    cXyz lookatPos[3] = {mLookatPos[0], mLookatPos[1], mLookatPos[2]};
    csXyz* lookatAngle[3] = {&mLookatAngle[0], &mLookatAngle[1], &mLookatAngle[2]};
    cXyz spe8;

    switch (mLookMode) {
    case LOOK_NONE:
        break;
    case LOOK_RESET:
        snap = TRUE;
        break;
    case LOOK_PLAYER:
    case LOOK_PLAYER_TALK:
        player = daPy_getPlayerActorClass();

        if (mLookMode != LOOK_PLAYER_TALK) {
            break;
        }

        head_angleY_min = -80.0f;
        head_angleY_max = 80.0f;
        break;
    case LOOK_ACTOR:
        player = (daPy_py_c*)mActorMngr[1].getActorP();
        break;
    case LOOK_ATTN:
        player = (daPy_py_c*)mActorMngr[2].getActorP();
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
    mLookat.calc(this, model->getBaseTRMtx(), lookatAngle, snap, angle_delta, FALSE);
}

void daNpc_Grd_c::setExpressionTalkAfter() {
    setExpression(EXPR_UNK_3, -1.0f);
}

int daNpc_Grd_c::wait(void* param_1) {
    switch (mMode) {
    case 0:
        setExpression(EXPR_UNK_3, -1.0f);
        setMotion(MOT_UNK_0, -1.0f, 0);

        setLookMode(LOOK_NONE);
        mTurnMode = 0;
        mMode = 2;
        // fallthrough
    case 2:
        if (!mIsDamaged) {
            BOOL bVar1 = mActorMngr[0].getActorP() != NULL;
            if (chkFindPlayer2(bVar1, shape_angle.y)) {
                if (!bVar1) {
                    mActorMngr[0].entry(daPy_getPlayerActorClass());
                    mTurnMode = 0;
                }
            } else if (bVar1) {
                mActorMngr[0].remove();
                mTurnMode = 0;
            }

            if (mActorMngr[0].getActorP() != NULL) {
                setLookMode(LOOK_PLAYER);
            } else {
                setLookMode(LOOK_NONE);

                if (home.angle.y != mCurAngle.y && step(home.angle.y, 3, 5, 15)) {
                    mMode = 0;
                }
            }

            if (home.angle.y == mCurAngle.y) {
                fopAc_ac_c* actor_p = getAttnActorP(
                    NULL != mActorMngr[0].getActorP(), srchAttnActor1,
                    mpHIO->m.common.search_distance, mpHIO->m.common.search_height,
                    mpHIO->m.common.search_depth, mpHIO->m.common.fov, shape_angle.y, 120, 1);
                if (actor_p != NULL) {
                    mActorMngr[1].entry(actor_p);
                    setLookMode(LOOK_ACTOR);
                }
            } else {
                mAttnChangeTimer = 0;
            }
        }
        break;

    case 3:
        break;
    }

    return TRUE;
}

int daNpc_Grd_c::talk(void* param_1) {
    BOOL rv = FALSE;
    BOOL bVar1 = FALSE;

    switch (mMode) {
    case 0:
        if (mIsDamaged) {
            break;
        }

        initTalk(mFlowID, NULL);
        mTurnMode = 0;
        mMode = 2;
        // fallthrough
    case 2:
        if (field_0x9ea) {
            bVar1 = TRUE;
        } else {
            setLookMode(LOOK_PLAYER_TALK);
            mActorMngr[0].entry(daPy_getPlayerActorClass());

            if (mCurAngle.y == fopAcM_searchPlayerAngleY(this)) {
                bVar1 = TRUE;
            } else if (step(fopAcM_searchPlayerAngleY(this), 3, 5, 15)) {
                setExpression(EXPR_UNK_3, -1.0f);
                setMotion(MOT_UNK_0, -1.0f, 0);
                mTurnMode = 0;
            }
        }

        if (bVar1 && talkProc(NULL, TRUE, NULL)) {
            rv = TRUE;
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

int daNpc_Grd_c::test(void* param_1) {
    switch (mMode) {
    case 0:
        speedF = 0.0f;
        speed.setall(0.0f);
        mMode = 2;
        // fallthrough
    case 2:
        if (mpHIO->m.common.face_expression != mExpression) {
            setExpression(mpHIO->m.common.face_expression, mpHIO->m.common.morf_frame);
        }

        setMotion(mpHIO->m.common.motion, mpHIO->m.common.morf_frame, 0);
        setLookMode(mpHIO->m.common.look_mode);
        mOrderEvtNo = 0;
        attention_info.flags = 0;
        break;
    case 3:
        break;
    }

    return TRUE;
}

static void* s_sub1(void* i_actor, void* i_data) {
    if (fopAc_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_NPC_GRZ) {
        return i_actor;
    }

    return NULL;
}

static void* s_sub2(void* i_actor, void* i_data) {
    if (fopAc_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_NPC_YELIA) {
        return i_actor;
    }

    return NULL;
}

BOOL daNpc_Grd_c::ECut_nodToGrz(int i_staffId) {
    fopAc_ac_c* actor_p;
    dEvent_manager_c& eventManager = dComIfGp_getEventManager();
    daPy_py_c* player = daPy_getPlayerActorClass();
    int rv = 0;
    int prm = -1;
    int timer = 0;
    int* piVar1 = NULL;

    piVar1 = dComIfGp_evmng_getMyIntegerP(i_staffId, "prm");
    if (piVar1 != NULL) {
        prm = *piVar1;
    }

    piVar1 = dComIfGp_evmng_getMyIntegerP(i_staffId, "timer");
    if (piVar1 != NULL) {
        timer = *piVar1;
    }

    if (eventManager.getIsAddvance(i_staffId)) {
        switch (prm) {
        case 0:
        case 20:
            actor_p = (fopAc_ac_c*)fpcM_Search(s_sub2, this);
            mActorMngr[2].entry(actor_p);
            setLookMode(LOOK_NONE);
            mEventTimer = timer;
            break;
        case 10:
            actor_p = (fopAc_ac_c*)fpcM_Search(s_sub1, this);
            mActorMngr[2].entry(actor_p);
            setLookMode(LOOK_ATTN);
            break;
        case 11:
            break;
        case 30:
        case 99:
            setExpression(EXPR_UNK_3, 0.0f);
            setMotion(MOT_UNK_0, 0.0f, 1);
            setLookMode(LOOK_RESET);
            setAngle(home.angle.y);
            break;
        }
    }

    s16 angleY;

    switch (prm) {
    case 0:
    case 20:
        if (cLib_calcTimer(&mEventTimer) == 0) {
            actor_p = mActorMngr[2].getActorP();
            if (prm == 20) {
                actor_p = daPy_getPlayerActorClass();
            }
            setLookMode(LOOK_ATTN);
            if (prm == 20) {
                setLookMode(LOOK_PLAYER);
            }
            angleY = fopAcM_searchActorAngleY(this, actor_p);
            if (angleY == mCurAngle.y) {
                mTurnMode = 0;
                rv = 1;
            } else if (step(angleY, 3, 5, 15)) {
                mTurnMode = 0;
                setMotion(MOT_UNK_0, -1.0f, 0);
                setExpression(EXPR_UNK_3, -1.0f);
            }
            if (prm == 20) {
                shape_angle.y = angleY;
            }
        }
        break;
    case 10:
        actor_p = mActorMngr[2].getActorP();
        angleY = fopAcM_searchActorAngleY(this, actor_p);
        if (angleY == mCurAngle.y) {
            mTurnMode = 0;
            if (mMotion == 4 && mMotionPhase > 0) {
                rv = 1;
            }
        } else if (step(angleY, 3, 5, 15)) {
            mTurnMode = 0;
            setExpression(EXPR_UNK_3, -1.0f);
            setMotion(MOT_UNK_4, -1.0f, 0);
        }
        shape_angle.y = angleY;
        break;
    case 11:
    case 30:
    case 99:
        rv = 1;
        break;
    default:
        rv = 1;
        break;
    }

    return rv;
}

static int daNpc_Grd_Create(void* i_this) {
    return ((daNpc_Grd_c*)i_this)->create();
}

static int daNpc_Grd_Delete(void* i_this) {
    return ((daNpc_Grd_c*)i_this)->Delete();
}

static int daNpc_Grd_Execute(void* i_this) {
    return ((daNpc_Grd_c*)i_this)->Execute();
}

static int daNpc_Grd_Draw(void* i_this) {
    return ((daNpc_Grd_c*)i_this)->Draw();
}

static int daNpc_Grd_IsDelete(void* i_this) {
    return 1;
}

static actor_method_class daNpc_Grd_MethodTable = {
    (process_method_func)daNpc_Grd_Create,  (process_method_func)daNpc_Grd_Delete,
    (process_method_func)daNpc_Grd_Execute, (process_method_func)daNpc_Grd_IsDelete,
    (process_method_func)daNpc_Grd_Draw,
};

extern actor_process_profile_definition g_profile_NPC_GRD = {
    fpcLy_CURRENT_e,         // mLayerID
    7,                       // mListID
    fpcPi_CURRENT_e,         // mListPrio
    PROC_NPC_GRD,            // mProcName
    &g_fpcLf_Method.base,    // sub_method
    sizeof(daNpc_Grd_c),     // mSize
    0,                       // mSizeOther
    0,                       // mParameters
    &g_fopAc_Method.base,    // sub_method
    308,                     // mPriority
    &daNpc_Grd_MethodTable,  // sub_method
    0x00044100,              // mStatus
    fopAc_NPC_e,             // mActorType
    fopAc_CULLBOX_CUSTOM_e,  // cullType
};

AUDIO_INSTANCES;
