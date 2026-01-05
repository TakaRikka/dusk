/**
 * @file d_a_npc_grc.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_npc_grc.h"
#include "d/actor/d_a_npc.h"
#include "Z2AudioLib/Z2Instances.h"

enum grC_RES_File_ID {
    /* BCK */
    /* 0x05 */ BCK_GRC_F_CHEERFUL = 0x5,
    /* 0x06 */ BCK_GRC_F_CHEERFUL_T,
    /* 0x07 */ BCK_GRC_GRUMPY,
    /* 0x08 */ BCK_GRC_GRUMPY_T,
    /* 0x09 */ BCK_GRC_F_SAD_TALK,
    /* 0x0A */ BCK_GRC_F_TALK_A,
    /* 0x0B */ BCK_GRC_F_WAIT_A,
    /* 0x0C */ BCK_GRC_FH_CLOSEEYE,
    /* 0x0D */ BCK_GRC_FH_SAD_TALK,
    /* 0x0E */ BCK_GRC_GET_UP,
    /* 0x0F */ BCK_GRC_SAD_TALK,
    /* 0x10 */ BCK_GRC_SAD_WAIT,
    /* 0x11 */ BCK_GRC_SIT_A,
    /* 0x12 */ BCK_GRC_STEP,
    /* 0x13 */ BCK_GRC_TALK_A,
    /* 0x14 */ BCK_GRC_TALK_B,
    /* 0x15 */ BCK_GRC_TOSIT_A,
    /* 0x16 */ BCK_GRC_WAIT_A,

    /* BTK */
    /* 0x19 */ BTK_GRC_A = 0x19,

    /* BTP */
    /* 0x1C */ BTP_GRC_A = 0x1C,
    /* 0x1D */ BTP_GRC_F_CHEERFUL_T,
    /* 0x1E */ BTP_GRC_F_GRUMPY_T,
    /* 0x1F */ BTP_GRC_F_SAD_TALK,
    /* 0x20 */ BTP_GRC_FH_CLOSEEYE,
    /* 0x21 */ BTP_GRC_FH_SAD_TALK,
};

enum grC_Mdl_RES_File_ID {
    /* BMDR */
    /* 0x3 */ BMDR_GRC_A = 0x3,
};

enum grC_TW_RES_File_ID {
    /* BMDR */
    /* 0x3 */ BMDR_GRC_TW = 0x3,
};

enum RES_Name {
    /* 0x0 */ GRC,
    /* 0x1 */ GRC_MDL,
    /* 0x2 */ GRC_TW,
};

enum Joint {
    /* 0x00 */ JNT_CENTER,
    /* 0x01 */ JNT_BACKBONE1,
    /* 0x02 */ JNT_BACKBONE2,
    /* 0x03 */ JNT_NECK,
    /* 0x04 */ JNT_HEAD,
    /* 0x05 */ JNT_CHIN,
    /* 0x06 */ JNT_MAYU,
    /* 0x07 */ JNT_MOUTH,
    /* 0x08 */ JNT_SHOULDERL,
    /* 0x09 */ JNT_ARML1,
    /* 0x0A */ JNT_ARML2,
    /* 0x0B */ JNT_HANDL,
    /* 0x0C */ JNT_SHOULDERR,
    /* 0x0D */ JNT_ARMR1,
    /* 0x0E */ JNT_ARMR2,
    /* 0x0F */ JNT_HANDR,
    /* 0x10 */ JNT_WAIST,
    /* 0x11 */ JNT_LEGL1,
    /* 0x12 */ JNT_LEGL2,
    /* 0x13 */ JNT_FOOTL,
    /* 0x14 */ JNT_LEGR1,
    /* 0x15 */ JNT_LEGR2,
    /* 0x16 */ JNT_FOOTR,
};

enum Animation {
    /* 0x00 */ ANM_NONE,
    /* 0x01 */ ANM_F_WAIT_A,
    /* 0x02 */ ANM_GRUMPY,
    /* 0x03 */ ANM_F_CHEERFUL,
    /* 0x04 */ ANM_FH_SAD_TALK,
    /* 0x05 */ ANM_F_TALK_A,
    /* 0x06 */ ANM_GRUMPY_T,
    /* 0x07 */ ANM_F_CHEERFUL_T,
    /* 0x08 */ ANM_F_SAD_TALK,
    /* 0x09 */ ANM_FH_CLOSEEYE,
    /* 0x0A */ ANM_WAIT_A,
    /* 0x0B */ ANM_TALK_A,
    /* 0x0C */ ANM_TALK_B,
    /* 0x0D */ ANM_SAD_TALK,
    /* 0x0E */ ANM_SIT_A,
    /* 0x0F */ ANM_TOSIT_A,
    /* 0x10 */ ANM_GET_UP,
    /* 0x11 */ ANM_SAD_WAIT,
    /* 0x12 */ ANM_STEP,
};

enum Expression {
    /* 0x0 */ EXPR_TALK_A,
    /* 0x1 */ EXPR_GRUMPY_T,
    /* 0x2 */ EXPR_CHEERFUL_T,
    /* 0x9 */ EXPR_SAD_TALK = 0x9,
    /* 0xA */ EXPR_CLOSEEYE,
    /* 0xB */ EXPR_GRUMPY,
    /* 0xC */ EXPR_CHEERFUL,
    /* 0xD */ EXPR_H_SAD_TALK,
    /* 0xE */ EXPR_NONE,
};

enum Expression_BTP {
    /* 0x0 */ EXPR_BTP_GRC_A,
    /* 0x1 */ EXPR_BTP_F_CHEERFUL_T,
    /* 0x2 */ EXPR_BTP_F_GRUMPY_T,
    /* 0x3 */ EXPR_BTP_F_SAD_TALK,
    /* 0x4 */ EXPR_BTP_FH_SAD_TALK,
    /* 0x5 */ EXPR_BTP_FH_CLOSEEYE,
};

enum Motion {
    /* 0x0 */ MOT_WAIT_A,
    /* 0x1 */ MOT_GET_UP,
    /* 0x2 */ MOT_TALK_A,
    /* 0x3 */ MOT_TALK_B,
    /* 0x4 */ MOT_TOSIT_A,
    /* 0x5 */ MOT_SIT_A,
    /* 0x6 */ MOT_SAD_TALK,
    /* 0x7 */ MOT_SAD_WAIT,
    /* 0x8 */ MOT_STEP,
};

enum Type {
    /* 0x0 */ TYPE_0,
    /* 0x1 */ TYPE_TWILIGHT,
    /* 0x2 */ TYPE_SPA,
    /* 0x3 */ TYPE_3,
    /* 0x4 */ TYPE_BUYER,
    /* 0x5 */ TYPE_5,
    /* 0x6 */ TYPE_6,
};

enum Event_Cut_Nums {
    /* 0x1 */ NUM_EVT_CUTS_e = 0x1,
};

static daNpc_grC_Param_c l_HIO;

static daNpc_GetParam1 l_bmdGetParamList[2] = {
    {BMDR_GRC_A, GRC_MDL},
    {BMDR_GRC_TW, GRC_TW},
};

static daNpc_GetParam1 l_bckGetParamList[19] = {
    {-1, GRC},
    {BCK_GRC_F_WAIT_A, 0},
    {BCK_GRC_GRUMPY, GRC},
    {BCK_GRC_F_CHEERFUL, GRC},
    {BCK_GRC_FH_SAD_TALK, GRC},
    {BCK_GRC_F_TALK_A, GRC},
    {BCK_GRC_GRUMPY_T, GRC},
    {BCK_GRC_F_CHEERFUL_T, GRC},
    {BCK_GRC_F_SAD_TALK, GRC},
    {BCK_GRC_FH_CLOSEEYE, GRC},
    {BCK_GRC_WAIT_A, GRC},
    {BCK_GRC_TALK_A, GRC},
    {BCK_GRC_TALK_B, GRC},
    {BCK_GRC_SAD_TALK, GRC},
    {BCK_GRC_SIT_A, GRC},
    {BCK_GRC_TOSIT_A, GRC},
    {BCK_GRC_GET_UP, GRC},
    {BCK_GRC_SAD_WAIT, GRC},
    {BCK_GRC_STEP, GRC},
};

static daNpc_GetParam1 l_btpGetParamList[6] = {
    {BTP_GRC_A, GRC}, 
    {BTP_GRC_F_CHEERFUL_T, GRC},
    {BTP_GRC_F_GRUMPY_T, GRC},
    {BTP_GRC_F_SAD_TALK, GRC},
    {BTP_GRC_FH_SAD_TALK, GRC},
    {BTP_GRC_FH_CLOSEEYE, GRC},
};

static daNpc_GetParam1 l_btkGetParamList[1] = {
    {-1, GRC},
};

static daNpc_GetParam1 l_evtGetParamList[1] = {
    {0, GRC},
};

static int l_loadRes_GRCa[3] = {
    GRC, GRC_MDL, -1,
};

static int l_loadRes_GRCa_TW[3] = {
    GRC, GRC_TW, -1,
};

static int l_loadRes_GRC0[3] = {
    GRC, GRC_MDL, -1,
};

static int* l_loadRes_list[7] = {
    l_loadRes_GRCa, l_loadRes_GRCa_TW, l_loadRes_GRCa,
    l_loadRes_GRCa, l_loadRes_GRCa,    l_loadRes_GRCa,
    l_loadRes_GRC0,
};

static char* l_resNames[3] = {
    "grC",
    "grC_Mdl",
    "grC_TW",
};

static char* l_evtNames[1] = {
    NULL
};

static char* l_myName = "grC";

char* daNpc_grC_c::mEvtCutNameList = "";

daNpc_grC_c::EventFn daNpc_grC_c::mEvtCutList[1] = {NULL};

daNpc_grC_c::daNpc_grC_c() {}

daNpc_grC_c::~daNpc_grC_c() {
    for (int i = 0; l_loadRes_list[mType][i] >= 0; i++) {
        dComIfG_resDelete(&mPhases[i], l_resNames[l_loadRes_list[mType][i]]);
    }

    if (heap != NULL) {
        mAnm_p->stopZelAnime();
    }
}

daNpc_grC_HIOParam const daNpc_grC_Param_c::m = {
    40.0f,
    -3.0f,
    1.0f,
    400.0f,
    255.0f,
    140.0f,
    35.0f,
    50.0f,
    0.0f,
    0.0f,
    10.0f,
    -10.0f,
    30.0f,
    -30.0f,
    45.0f,
    -45.0f,
    0.6f,
    12.0f,
    4,
    6,
    6,
    6,
    180.0f,
    500.0f,
    300.0f,
    -300.0f,
    60,
    8,
    0,
    0,
    0,
    false,
    false,
};

cPhs__Step daNpc_grC_c::create() {
    fopAcM_ct(this, daNpc_grC_c);

    mTwilight = dKy_darkworld_check();
    mType = getTypeFromParam();

    if (home.angle.x != 0xFFFF) {
        mFlowID = home.angle.x;
    } else {
        mFlowID = -1;
    }

    if (isDelete()) {
        return cPhs_ERROR_e;
    }

    int res_count = 0;
    int i = 0;
    for (; l_loadRes_list[mType][i] >= 0; i++) {
        cPhs__Step step = (cPhs__Step)dComIfG_resLoad(&mPhases[i], l_resNames[l_loadRes_list[mType][i]]);

        if (step == cPhs_ERROR_e || step == cPhs_UNK3_e) {
            return cPhs_ERROR_e;
        }

        if (step == cPhs_COMPLEATE_e) {
            res_count++;
        }
    }

    if (res_count == i) {
        if (!fopAcM_entrySolidHeap(this, createHeapCallBack, 0x36E0)) {
            return cPhs_ERROR_e;
        }

        fopAcM_SetMtx(this, mAnm_p->getModel()->getBaseTRMtx());
        fopAcM_setCullSizeBox(this, -300.0f, -50.0f, -300.0f, 300.0f, 450.0f, 300.0f);

        mSound.init(&current.pos, &eyePos, 3, 1);

        mAcchCir.SetWall(daNpc_grC_Param_c::m.common.width, daNpc_grC_Param_c::m.common.knee_length);
        mAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1, &mAcchCir,
                  fopAcM_GetSpeed_p(this), fopAcM_GetAngle_p(this), fopAcM_GetShapeAngle_p(this));
        mPaPo.init(&mAcch, daNpc_grC_Param_c::m.common.height, daNpc_grC_Param_c::m.common.height);
        mCcStts.Init(daNpc_grC_Param_c::m.common.weight, 0, this);
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

int daNpc_grC_c::CreateHeap() {
    J3DModelData* mdlData_p = NULL;

    if (mTwilight) {
        if (l_bmdGetParamList[1].fileIdx >= 0) {
            mdlData_p = (J3DModelData*)dComIfG_getObjectRes(l_resNames[l_bmdGetParamList[1].arcIdx], l_bmdGetParamList[1].fileIdx);
        }
    } else if (l_bmdGetParamList[0].fileIdx >= 0) {
        mdlData_p = (J3DModelData*)dComIfG_getObjectRes(l_resNames[l_bmdGetParamList[0].arcIdx], l_bmdGetParamList[0].fileIdx);
    }

    JUT_ASSERT(452, NULL != mdlData_p);

    mAnm_p = new mDoExt_McaMorfSO(mdlData_p, NULL, NULL, NULL, -1, 1.0f, 0, -1, &mSound, 0x80000, 0x11020284);
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

    field_0xbd8 = 0;

    if (!setExpressionAnm(ANM_F_TALK_A, false)) {
        return 0;
    }

    setMotionAnm(ANM_WAIT_A, 0.0f);

    return 1;
}

int daNpc_grC_c::Delete() {
    this->~daNpc_grC_c();
    return 1;
}

int daNpc_grC_c::Execute() {
    return execute();
}

int daNpc_grC_c::Draw() {
    return draw(chkAction(&daNpc_grC_c::test), FALSE, daNpc_grC_Param_c::m.common.real_shadow_size, NULL, FALSE);
}

int daNpc_grC_c::ctrlJoint(J3DJoint* i_joint, J3DModel* i_model) {
    int jntNo = i_joint->getJntNo();
    int i_jointList[3] = {JNT_BACKBONE1, JNT_NECK, JNT_HEAD};

    if (jntNo == 0) {
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
            setLookatMtx(jntNo, i_jointList, daNpc_grC_Param_c::m.common.neck_rotation_ratio);
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

    if ((jntNo == JNT_HEAD || jntNo == JNT_MOUTH) && (mAnmFlags & ANM_PLAY_BCK) != 0) {
        J3DAnmTransform* anm = mBckAnm.getBckAnm();
        mBckAnm.changeBckOnly(mAnm_p->getAnm());
        mAnm_p->changeAnm(anm);
    }

    return 1;
}

int daNpc_grC_c::createHeapCallBack(fopAc_ac_c* a_this) {
    daNpc_grC_c* i_this = (daNpc_grC_c*)a_this;
    return i_this->CreateHeap();
}

int daNpc_grC_c::ctrlJointCallBack(J3DJoint* i_joint, int param_2) {
    if (param_2 == 0) {
        J3DModel* model = j3dSys.getModel();
        daNpc_grC_c* i_this = (daNpc_grC_c*)model->getUserArea();

        if (i_this != NULL) {
            i_this->ctrlJoint(i_joint, model);
        }
    }

    return 1;
}

void daNpc_grC_c::setParam() {
    ActionFn action = mNextAction;
    u32 uVar1 = (fopAc_AttnFlag_SPEAK_e | fopAc_AttnFlag_TALK_e);
    selectAction();

    if (!mTwilight && daPy_py_c::checkNowWolf()) {
        uVar1 = 0;
    }

    if (mNextAction != action) {
        for (int i = 2; i < 2; i++) {
            mActorMngr[i].initialize();
        }
    }

    field_0xe30 = 0;
    field_0xe34 = 0;

    s16 talk_distance = daNpc_grC_Param_c::m.common.talk_distance;
    s16 talk_angle = daNpc_grC_Param_c::m.common.talk_angle;

    attention_info.distances[fopAc_attn_LOCK_e] = getDistTableIdx(daNpc_grC_Param_c::m.common.attention_distance, daNpc_grC_Param_c::m.common.attention_angle);
    attention_info.distances[fopAc_attn_TALK_e] = attention_info.distances[fopAc_attn_LOCK_e];
    attention_info.distances[fopAc_attn_SPEAK_e] = getDistTableIdx(talk_distance, talk_angle);
    attention_info.flags = uVar1;

    scale.set(daNpc_grC_Param_c::m.common.scale, daNpc_grC_Param_c::m.common.scale, daNpc_grC_Param_c::m.common.scale);
    mAcchCir.SetWallR(daNpc_grC_Param_c::m.common.width);
    mAcchCir.SetWallH(daNpc_grC_Param_c::m.common.knee_length);
    gravity = daNpc_grC_Param_c::m.common.gravity;
}

BOOL daNpc_grC_c::main() {
    if (!doEvent()) {
        doNormalAction(1);
    }

    if (checkHide()) {
        attention_info.flags = 0;
    }

    if (!daNpc_grC_Param_c::m.common.debug_mode_ON && (!dComIfGp_event_runCheck() || (mOrderNewEvt && dComIfGp_getEvent()->isOrderOK()))) {
        if (mOrderEvtNo != 0) {
            eventInfo.setArchiveName(l_resNames[l_evtGetParamList[mOrderEvtNo].arcIdx]);
        }

        orderEvent(field_0xe51, l_evtNames[l_evtGetParamList[mOrderEvtNo].fileIdx], 0xFFFF, 40, 0xFF, 1);
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

BOOL daNpc_grC_c::ctrlBtk() {
    return FALSE;
}

void daNpc_grC_c::setAttnPos() {
    static cXyz const eyeOffset(16.0f, 60.0f, 0.0f);

    cXyz sp7c, sp88, sp94, spa0;
    f32 attention_offset = daNpc_grC_Param_c::m.common.attention_offset;

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

    J3DModelData* mdlData_p = mAnm_p->getModel()->getModelData();
    for (u16 i = 0; i < mdlData_p->getJointNum(); i++) {
        mdlData_p->getJointNodePointer(i)->setCallBack(ctrlJointCallBack);
    }

    setMtx();
    lookat();
    setPrtcl();

    mDoMtx_stack_c::copy(mAnm_p->getModel()->getAnmMtx(4));
    mDoMtx_stack_c::multVecZero(&mHeadPos);
    mDoMtx_stack_c::multVec(&eyeOffset, &eyePos);
    mDoMtx_stack_c::multVec(&eyeOffset, &sp94);

    mHeadAngle.x = cLib_targetAngleX(&mHeadPos, &sp94);
    mHeadAngle.y = cLib_targetAngleY(&mHeadPos, &sp94);

    cXyz* attnPos = mLookat.getAttnPos();
    if (attnPos != NULL) {
        sp88 = *attnPos - eyePos;
        mEyeAngle.y = -(mLookatAngle[2].y + mCurAngle.y);
        mEyeAngle.y += cM_atan2s(sp88.x, sp88.z);
        mEyeAngle.x = -cM_atan2s(sp88.y, sp88.absXZ());
    } else {
        mEyeAngle.y = 0;
        mEyeAngle.x = 0;
    }

    f32 fVar1 = 0.0f;
    f32 fVar2 = 0.0f;

    attention_info.position = mHeadPos;
    attention_info.position.y += attention_offset;

    if (!mHide) {
        if (!mIsDamaged) {
            mCyl.SetTgType(0xD8FBFDFF);
            mCyl.SetTgSPrm(0x1F);
            mCyl.OnTgNoHitMark();
        } else {
            mCyl.SetTgType(0);
            mCyl.SetTgSPrm(0);
        }

        mCyl.SetCoSPrm(0x79);
        mDoMtx_stack_c::copy(mAnm_p->getModel()->getAnmMtx(1));
        mDoMtx_stack_c::multVecZero(&sp7c);
        sp7c.y = current.pos.y;
        mCyl.SetC(sp7c);
        mCyl.SetH(daNpc_grC_Param_c::m.common.height + fVar1);
        mCyl.SetR(daNpc_grC_Param_c::m.common.width + fVar2);
        dComIfG_Ccsp()->Set(&mCyl);
    }

    mCyl.ClrTgHit();
}

bool daNpc_grC_c::setExpressionAnm(int i_idx, bool i_modify) {
    J3DAnmTransform* anm = NULL;
    int i_attr = J3DFrameCtrl::EMode_NONE;

    mAnmFlags &= ~ANM_EXPRESSION_FLAGS;

    if (l_bckGetParamList[i_idx].fileIdx >= 0) {
        anm = getTrnsfrmKeyAnmP(l_resNames[l_bckGetParamList[i_idx].arcIdx], l_bckGetParamList[i_idx].fileIdx);
    }

    bool bVar1 = false;
    switch (i_idx) {
        case ANM_NONE:
            bVar1 = setExpressionBtp(EXPR_BTP_GRC_A);
            break;

        case ANM_F_WAIT_A:
            bVar1 = setExpressionBtp(EXPR_BTP_GRC_A);
            i_attr = J3DFrameCtrl::EMode_LOOP;
            break;

        case ANM_GRUMPY:
            bVar1 = setExpressionBtp(EXPR_BTP_GRC_A);
            i_attr = J3DFrameCtrl::EMode_LOOP;
            break;

        case ANM_F_CHEERFUL:
            bVar1 = setExpressionBtp(EXPR_BTP_GRC_A);
            i_attr = J3DFrameCtrl::EMode_LOOP;
            break;

        case ANM_FH_SAD_TALK:
            bVar1 = setExpressionBtp(EXPR_BTP_FH_SAD_TALK);
            i_attr = J3DFrameCtrl::EMode_LOOP;
            break;

        case ANM_F_TALK_A:
            bVar1 = setExpressionBtp(EXPR_BTP_GRC_A);
            i_attr = J3DFrameCtrl::EMode_LOOP;
            break;

        case ANM_GRUMPY_T:
            // BTP idx may have mistakenly been switched with the case that follows
            bVar1 = setExpressionBtp(EXPR_BTP_F_CHEERFUL_T);
            break;

        case ANM_F_CHEERFUL_T:
            // BTP idx may have mistakenly been switched with the case that preceeds
            bVar1 = setExpressionBtp(EXPR_BTP_F_GRUMPY_T);
            break;

        case ANM_F_SAD_TALK:
            bVar1 = setExpressionBtp(EXPR_BTP_F_SAD_TALK);
            break;

        case ANM_FH_CLOSEEYE:
            bVar1 = setExpressionBtp(EXPR_BTP_FH_CLOSEEYE);
            i_attr = J3DFrameCtrl::EMode_LOOP;
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

    if (setBckAnm(anm, 1.0f, i_attr, 0, -1, i_modify)) {
        mAnmFlags |= ANM_PAUSE_BCK | ANM_PLAY_BCK;
        mExpressionLoops = 0;
        return true;
    }

    return false;
}

bool daNpc_grC_c::setExpressionBtp(int i_idx) {
    J3DAnmTexPattern* anm = NULL;
    int i_attr = J3DFrameCtrl::EMode_NONE;

    mAnmFlags &= ~(ANM_PLAY_BTP | ANM_PAUSE_BTP | ANM_FLAG_800);

    if (l_btpGetParamList[i_idx].fileIdx >= 0) {
        anm = getTexPtrnAnmP(l_resNames[l_btpGetParamList[i_idx].arcIdx], l_btpGetParamList[i_idx].fileIdx);
    }

    switch (i_idx) {
        case EXPR_BTP_GRC_A:
            i_attr = J3DFrameCtrl::EMode_LOOP;
            break;

        case EXPR_BTP_F_CHEERFUL_T:
        case EXPR_BTP_F_GRUMPY_T:
        case EXPR_BTP_F_SAD_TALK:
            break;
        
        case EXPR_BTP_FH_SAD_TALK:
            i_attr = J3DFrameCtrl::EMode_LOOP;
            break;

        case EXPR_BTP_FH_CLOSEEYE:
            i_attr = J3DFrameCtrl::EMode_LOOP;
            break;

        default:
            anm = NULL;
            break;
    }

    if (anm == NULL) {
        return true;
    }

    if (setBtpAnm(anm, mAnm_p->getModel()->getModelData(), 1.0f, i_attr)) {
        mAnmFlags |= ANM_PAUSE_BTP | ANM_PLAY_BTP;

        if (i_idx == EXPR_BTP_GRC_A) {
            mAnmFlags |= ANM_FLAG_800;
        }

        return true;
    }

    return false;
}

void daNpc_grC_c::setExpression(int i_expression, f32 i_morf) {
    if (i_expression >= 0 && i_expression < 0xF) {
        mExpression = i_expression;
        mExpressionMorfOverride = i_morf;
        mExpressionPrevPhase = -1;
        mExpressionPhase = 0;
    }
}

void daNpc_grC_c::setMotionAnm(int i_idx, f32 i_morf) {
    J3DAnmTransformKey* anm = NULL;
    J3DAnmTextureSRTKey* i_btk = NULL;
    int i_attr = J3DFrameCtrl::EMode_LOOP;

    mAnmFlags &= ~ANM_MOTION_FLAGS;

    if (l_bckGetParamList[i_idx].fileIdx >= 0) {
        anm = getTrnsfrmKeyAnmP(l_resNames[l_bckGetParamList[i_idx].arcIdx], l_bckGetParamList[i_idx].fileIdx);
    }

    switch (i_idx) {
        case ANM_WAIT_A:
        case ANM_TALK_A:
        case ANM_SIT_A:
        case ANM_SAD_WAIT:
            break;
        
        case ANM_TALK_B:
            i_attr = J3DFrameCtrl::EMode_NONE;
            break;

        case ANM_SAD_TALK:
            i_attr = J3DFrameCtrl::EMode_NONE;
            break;

        case ANM_TOSIT_A:
            i_attr = J3DFrameCtrl::EMode_NONE;
            break;

        case ANM_GET_UP:
            i_attr = J3DFrameCtrl::EMode_NONE;
            break;

        case ANM_STEP:
            i_attr = J3DFrameCtrl::EMode_NONE;
            break;

        default:
            anm = NULL;
            i_btk = NULL;
            break;
    }

    if (l_btkGetParamList[0].fileIdx >= 0) {
        i_btk = getTexSRTKeyAnmP(l_resNames[l_btkGetParamList[0].arcIdx], l_btkGetParamList[0].fileIdx);
    }

    if (i_btk != NULL && setBtkAnm(i_btk, mAnm_p->getModel()->getModelData(), 1.0f, J3DFrameCtrl::EMode_LOOP)) {
        mAnmFlags |= ANM_PAUSE_BTK | ANM_PLAY_BTK;
    }

    if (anm != NULL && setMcaMorfAnm(anm, 1.0f, i_morf, i_attr, 0, -1)) {
        mAnmFlags |= ANM_PLAY_MORF | ANM_PAUSE_MORF;
        mMotionLoops = 0;
    }
}

void daNpc_grC_c::setMotion(int i_motion, f32 i_morf, int param_3) {
    s16 motion = i_motion;
    if ((param_3 != 0 || mMotion != motion) && i_motion >= 0 && i_motion < 9) {
        mMotion = motion;
        mMotionMorfOverride = i_morf;
        mMotionPrevPhase = -1;
        mMotionPhase = 0;
    }
}

int daNpc_grC_c::drawDbgInfo() {
    return 0;
}

void daNpc_grC_c::drawOtherMdls() {
    /* empty function */
}

u8 daNpc_grC_c::getTypeFromParam() {
    switch (fopAcM_GetParam(this) & 0xFF) {
        case 0:
        default:
            return mTwilight ? TYPE_TWILIGHT : TYPE_0;
            
        case 1:
            return TYPE_SPA;

        case 2:
            if (mTwilight) {
                return TYPE_TWILIGHT;
            }

            return TYPE_3;

        case 3:
            return TYPE_BUYER;

        case 4:
            return TYPE_5;
    }
}

BOOL daNpc_grC_c::isDelete() {
    if (mType == TYPE_6 || mType == TYPE_0 || mType == TYPE_TWILIGHT || mType == TYPE_SPA &&
        /* dSv_event_flag_c::M_031 - Goron Mines - Goron Mines clear */
        daNpcF_chkEvtBit(64) ||
        mType == TYPE_3 || mType == TYPE_BUYER || mType == TYPE_5) {
        return FALSE;
    }

    return TRUE;
}

void daNpc_grC_c::reset() {
    initialize();
    mLookat.initialize();

    for (int i = 0; i < 2; i++) {
        mActorMngr[i].initialize();
    }

    mNextAction = NULL;
    mAction = NULL;
    field_0xe30 = 0;
    field_0xe34 = 0;
    field_0xe38 = 0;
    field_0xe3c = 0;
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
    field_0xe51 = false;
    mIsSpringWaterShopClosed = false;
    mItemID = fpcM_ERROR_PROCESS_ID_e;

    if (mType == TYPE_3) {
        /* dSv_event_flag_c::F_0377 - Castle Town - Goron spring water shop open! */
        if (!daNpcF_chkEvtBit(377)) {
            mIsSpringWaterShopClosed = true;
        }
    } else if (mType == TYPE_BUYER) {
        setExpression(EXPR_NONE, -1.0f);
        setMotion(MOT_SIT_A, -1.0f, 0);
    }
}

void daNpc_grC_c::playExpression() {
    daNpcF_anmPlayData dat0a = {ANM_F_TALK_A, daNpc_grC_Param_c::m.common.morf_frame, 1};
    daNpcF_anmPlayData dat0b = {ANM_F_WAIT_A, daNpc_grC_Param_c::m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat0[2] = {&dat0a, &dat0b};
    daNpcF_anmPlayData dat1a = {ANM_GRUMPY_T, daNpc_grC_Param_c::m.common.morf_frame, 1};
    daNpcF_anmPlayData dat1b = {ANM_GRUMPY, daNpc_grC_Param_c::m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat1[2] = {&dat1a, &dat1b};
    daNpcF_anmPlayData dat2a = {ANM_F_CHEERFUL_T, daNpc_grC_Param_c::m.common.morf_frame, 1};
    daNpcF_anmPlayData dat2b = {ANM_F_CHEERFUL, daNpc_grC_Param_c::m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat2[2] = {&dat2a, &dat2b};
    daNpcF_anmPlayData dat3a = {ANM_F_SAD_TALK, daNpc_grC_Param_c::m.common.morf_frame, 1};
    daNpcF_anmPlayData dat3b = {ANM_FH_SAD_TALK, daNpc_grC_Param_c::m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat3[2] = {&dat3a, &dat3b};
    daNpcF_anmPlayData dat4 = {ANM_FH_CLOSEEYE, daNpc_grC_Param_c::m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat4[1] = {&dat4};
    daNpcF_anmPlayData dat5 = {ANM_GRUMPY, daNpc_grC_Param_c::m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat5[1] = {&dat5};
    daNpcF_anmPlayData dat6 = {ANM_F_CHEERFUL, daNpc_grC_Param_c::m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat6[1] = {&dat6};
    daNpcF_anmPlayData dat7 = {ANM_FH_SAD_TALK, daNpc_grC_Param_c::m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat7[1] = {&dat7};
    daNpcF_anmPlayData dat8 = {ANM_NONE, daNpc_grC_Param_c::m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat8[1] = {&dat8};

    daNpcF_anmPlayData** ppDat[15] = {
        pDat0,
        pDat1,
        pDat2,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        pDat3,
        pDat4,
        pDat5,
        pDat6,
        pDat7,
        pDat8,
    };

    if (mExpression >= 0 && mExpression < 0xF) {
        playExpressionAnm(ppDat);
    }
}

void daNpc_grC_c::playMotion() {
    daNpcF_anmPlayData dat0 = {ANM_WAIT_A, daNpc_grC_Param_c::m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat0[1] = {&dat0};
    daNpcF_anmPlayData dat1a = {ANM_GET_UP, daNpc_grC_Param_c::m.common.morf_frame, 1};
    daNpcF_anmPlayData dat1b = {ANM_WAIT_A, 0.0f, 0};
    daNpcF_anmPlayData* pDat1[2] = {&dat1a, &dat1b};
    daNpcF_anmPlayData dat2 = {ANM_TALK_A, daNpc_grC_Param_c::m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat2[1] = {&dat2};
    daNpcF_anmPlayData dat3a = {ANM_TALK_B, daNpc_grC_Param_c::m.common.morf_frame, 1};
    daNpcF_anmPlayData dat3b = {ANM_WAIT_A, 0.0f, 0};
    daNpcF_anmPlayData* pDat3[2] = {&dat3a, &dat3b};
    daNpcF_anmPlayData dat4a = {ANM_TOSIT_A, daNpc_grC_Param_c::m.common.morf_frame, 1};
    daNpcF_anmPlayData dat4b = {ANM_SIT_A, 0.0f, 0};
    daNpcF_anmPlayData* pDat4[2] = {&dat4a, &dat4b};
    daNpcF_anmPlayData dat5 = {ANM_SIT_A, daNpc_grC_Param_c::m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat5[1] = {&dat5};
    daNpcF_anmPlayData dat6a = {ANM_SAD_TALK, daNpc_grC_Param_c::m.common.morf_frame, 1};
    daNpcF_anmPlayData dat6b = {ANM_SAD_WAIT, 0.0f, 0};
    daNpcF_anmPlayData* pDat6[2] = {&dat6a, &dat6b};
    daNpcF_anmPlayData dat7 = {ANM_SAD_WAIT, daNpc_grC_Param_c::m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat7[1] = {&dat7};
    daNpcF_anmPlayData dat8 = {ANM_STEP, daNpc_grC_Param_c::m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat8[1] = {&dat8};

    daNpcF_anmPlayData** ppDat[9] = {
        pDat0,
        pDat1,
        pDat2,
        pDat3,
        pDat4,
        pDat5,
        pDat6,
        pDat7,
        pDat8,
    };

    if (mMotion >= 0 && mMotion < 9) {
        playMotionAnm(ppDat);
    }
}

BOOL daNpc_grC_c::chkAction(ActionFn action) {
    return mAction == action;
}

BOOL daNpc_grC_c::setAction(ActionFn action) {
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

BOOL daNpc_grC_c::selectAction() {
    mNextAction = NULL;

    if (daNpc_grC_Param_c::m.common.debug_mode_ON) {
        mNextAction = &daNpc_grC_c::test;
    } else {
        switch (mType) {
            case TYPE_TWILIGHT:
                mNextAction = &daNpc_grC_c::waitTW;
                break;

            case TYPE_SPA:
                mNextAction = &daNpc_grC_c::waitSpa;
                break;

            case TYPE_BUYER:
                mNextAction = &daNpc_grC_c::waitBuyer;
                break;

            default:
                mNextAction = &daNpc_grC_c::wait;
                break;
        }
    }

    return TRUE;
}

void daNpc_grC_c::doNormalAction(int param_1) {
    if (param_1 != 0 && hitChk2(&mCyl, TRUE, FALSE)) {
        int i_timer;
        if (mCutType == daPy_py_c::CUT_TYPE_TURN_RIGHT) {
            i_timer = 20;
        } else {
            i_timer = daNpc_grC_Param_c::m.common.damage_time;
        }

        setDamage(i_timer, 14, 0);
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

BOOL daNpc_grC_c::doEvent() {
    BOOL rv = FALSE;

    if (dComIfGp_event_runCheck()) {
        dEvent_manager_c& eventManager = dComIfGp_getEventManager();

        if (!field_0xe51) {
            mOrderNewEvt = false;
        } else {
            mOrderNewEvt = true;
        }

        if (eventInfo.checkCommandTalk()) {
            if (chkAction(&daNpc_grC_c::talk)) {
                (this->*mAction)(NULL);
            } else if (dComIfGp_event_chkTalkXY() == 0 || dComIfGp_evmng_ChkPresentEnd()) {
                setAction(&daNpc_grC_c::talk);
            }

            rv = TRUE;
        } else {
            if (mItemID != fpcM_ERROR_PROCESS_ID_e) {
                dComIfGp_event_setItemPartnerId(mItemID);
                mItemID = fpcM_ERROR_PROCESS_ID_e;
            }

            int staffId = eventManager.getMyStaffId(l_myName, NULL, 0);
            if (staffId != -1) {
                mStaffID = staffId;
                int evtCutNo = eventManager.getMyActIdx(staffId, &mEvtCutNameList, 1, 0, 0);

                JUT_ASSERT(1774, (0 <= evtCutNo) && (evtCutNo < NUM_EVT_CUTS_e));
                JUT_ASSERT(1775, NULL != mEvtCutList[evtCutNo]);

                if ((this->*mEvtCutList[evtCutNo])(staffId)) {
                    eventManager.cutEnd(staffId);
                }

                rv = TRUE;
            }

            if (eventInfo.checkCommandDemoAccrpt() && mEventIdx != -1 && eventManager.endCheck(mEventIdx) != 0) {
                dComIfGp_event_reset();
                mOrderEvtNo = 0;
                mEventIdx = -1;
            }
        }

        int i_msgTimer, i_expression, i_motion;
        i_msgTimer = mMsgTimer;

        if (ctrlMsgAnm(i_expression, i_motion, this, FALSE) != 0) {
            if (!field_0x9eb) {
                setExpression(i_expression, -1.0f);
                setMotion(i_motion, -1.0f, 0);
            }
        } else if (i_msgTimer != 0 && !field_0x9eb) {
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

void daNpc_grC_c::setLookMode(int i_lookMode) {
    if (i_lookMode >= 0 && i_lookMode < 5 && i_lookMode != mLookMode) {
        mLookMode = i_lookMode;
    }
}

void daNpc_grC_c::lookat() {
    daPy_py_c* player = NULL;
    J3DModel* model = mAnm_p->getModel();
    BOOL i_snap = FALSE;
    f32 body_angleX_min = daNpc_grC_Param_c::m.common.body_angleX_min;
    f32 body_angleX_max = daNpc_grC_Param_c::m.common.body_angleX_max;
    f32 body_angleY_min = daNpc_grC_Param_c::m.common.body_angleY_min;
    f32 body_angleY_max = daNpc_grC_Param_c::m.common.body_angleY_max;
    f32 head_angleX_min = daNpc_grC_Param_c::m.common.head_angleX_min;
    f32 head_angleX_max = daNpc_grC_Param_c::m.common.head_angleX_max;
    f32 head_angleY_min = daNpc_grC_Param_c::m.common.head_angleY_min;
    f32 head_angleY_max = daNpc_grC_Param_c::m.common.head_angleY_max;
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

            if (mLookMode != LOOK_PLAYER_TALK) {
                break;
            }

            head_angleY_min = -80.0f;
            head_angleY_max = 80.0f;
            break;

        case LOOK_ACTOR:
            player = (daPy_py_c*)mActorMngr[1].getActorP();
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
    
    mLookat.setParam(body_angleX_min, body_angleX_max, body_angleY_min, body_angleY_max, 0.0f, 0.0f, 0.0f, 0.0f, 
                     head_angleX_min, head_angleX_max, head_angleY_min, head_angleY_max, mCurAngle.y, lookatPos);
    mLookat.calc(this, model->getBaseTRMtx(), lookatAngle, i_snap, angle_delta, FALSE);
}

BOOL daNpc_grC_c::chkFindPlayer() {
    BOOL rv;

    if (abs((s16)(fopAcM_searchPlayerAngleY(this) - current.angle.y)) < 0x3C00) {
        if (mActorMngr[0].getActorP() == NULL) {
            rv = chkActorInAttnArea(daPy_getPlayerActorClass(), this, getDistTableIdx(10, 6));
        } else {
            rv = chkActorInAttnArea(daPy_getPlayerActorClass(), this, getDistTableIdx(12, 6));
        }
    } else {
        if (mActorMngr[0].getActorP() == NULL) {
            rv = chkPlayerInSpeakArea(this);
        } else {
            rv = chkPlayerInTalkArea(this);
        }
    }

    if (rv) {
        mActorMngr[0].entry(daPy_getPlayerActorClass());
    } else {
        mActorMngr[0].remove();
    }

    return rv;
}

void daNpc_grC_c::setExpressionTalkAfter() {
    switch (mExpression) {
        case EXPR_GRUMPY_T:
            setExpression(EXPR_GRUMPY, -1.0f);
            break;
        
        case EXPR_CHEERFUL_T:
            setExpression(EXPR_CHEERFUL, -1.0f);
            break;
        
        case EXPR_SAD_TALK:
            setExpression(EXPR_H_SAD_TALK, -1.0f);
            break;

        case EXPR_CLOSEEYE:
            break;

        default:
            setExpression(EXPR_NONE, -1.0f);
            break;
    }
}

BOOL daNpc_grC_c::wait(void* param_1) {
    switch (mMode) {
        case 0:
            if (mIsSpringWaterShopClosed) {
                setExpression(EXPR_H_SAD_TALK, -1.0f);
                setMotion(MOT_SAD_WAIT, -1.0f, 0);
                field_0x9ea = true;
            } else {
                setExpression(EXPR_NONE, -1.0f);
                setMotion(MOT_WAIT_A, -1.0f, 0);
            }

            setLookMode(LOOK_NONE);
            mTurnMode = 0;
            mMode = 2;
            // fallthrough
        case 2:
            if (!mIsDamaged) {
                if (mActorMngr[0].getActorP() != NULL) {
                    if (!chkFindPlayer()) {
                        mTurnMode = 0;
                    }
                } else if (chkFindPlayer()) {
                    mTurnMode = 0;
                }

                if (mActorMngr[0].getActorP() != NULL) {
                    setLookMode(LOOK_PLAYER);
                } else {
                    setLookMode(LOOK_NONE);

                    if (home.angle.y != mCurAngle.y && step(home.angle.y, 14, 8, 15)) {
                        mMode = 0;
                    }
                }
            }
            break;

        case 3:
            break;
    }

    return TRUE;
}

BOOL daNpc_grC_c::waitTW(void* param_1) {
    switch (mMode) {
        case 0:
            setExpression(EXPR_NONE, -1.0f);
            setMotion(MOT_WAIT_A, -1.0f, 0);
            setLookMode(LOOK_NONE);
            mTurnMode = 0;
            mMode = 2;
            // fallthrough
        case 2:
            if (!mIsDamaged && home.angle.y != mCurAngle.y && step(home.angle.y, 14, 8, 15)) {
                mMode = 0;
            }
            break;

        case 3:
            break;
    }

    return TRUE;
}

BOOL daNpc_grC_c::waitSpa(void* param_1) {
    switch (mMode) {
        case 0:
            if (strcmp(dComIfGp_getStartStageName(), "F_SP110") == 0) {
                setExpression(EXPR_CLOSEEYE, -1.0f);
            } else {
                setExpression(EXPR_NONE, -1.0f);
            }

            setMotion(MOT_SIT_A, -1.0f, 0);
            setLookMode(LOOK_NONE);
            mTurnMode = 0;
            mMode = 2;
            field_0x9ea = true;
            // fallthrough
        case 2:
            if (!mIsDamaged) {
                if (mActorMngr[0].getActorP() != NULL) {
                    if (!chkFindPlayer()) {
                        mTurnMode = 0;
                    }
                } else if (chkFindPlayer()) {
                    mTurnMode = 0;
                }

                if (mActorMngr[0].getActorP() != NULL) {
                    setLookMode(LOOK_PLAYER);
                } else {
                    setLookMode(LOOK_NONE);
                }

                if (home.angle.y == mCurAngle.y) {
                    fopAc_ac_c* actor = getAttnActorP(mActorMngr[0].getActorP() != NULL, srchAttnActor1, daNpc_grC_Param_c::m.common.search_distance,
                                                      daNpc_grC_Param_c::m.common.search_height, daNpc_grC_Param_c::m.common.search_depth, daNpc_grC_Param_c::m.common.fov,
                                                      shape_angle.y, 120, TRUE);
                    if (actor != NULL) {
                        mActorMngr[1].entry(actor);
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

BOOL daNpc_grC_c::waitBuyer(void* param_1) {
    switch (mMode) {
        case 0:
            if (!field_0xe51 && mMotion != MOT_SIT_A) {
                setExpression(EXPR_NONE, -1.0f);
                setMotion(MOT_TOSIT_A, -1.0f, 0);
            }

            mTurnMode = 0;
            field_0x9ea = true;
            mMode = 2;
            field_0x9ea = true;
            // fallthrough
        case 2:
            if (!mIsDamaged) {
                if (mActorMngr[0].getActorP() != NULL) {
                    if (!chkFindPlayer()) {
                        mTurnMode = 0;
                    }
                } else if (chkFindPlayer()) {
                    mTurnMode = 0;
                }

                if (mActorMngr[0].getActorP() != NULL) {
                    setLookMode(LOOK_PLAYER);
                } else {
                    setLookMode(LOOK_NONE);
                }
            }
            break;

        case 3:
            break;
    }

    return TRUE;
}

BOOL daNpc_grC_c::talk(void* param_1) {
    BOOL rv = FALSE;
    BOOL bVar1 = FALSE;
    int i_itemNo = 0;

    switch (mMode) {
        case 0:
            if (mIsDamaged) {
                break;
            }

            initTalk(mFlowID, NULL);
            mTurnMode = 0;

            if ((mType == TYPE_BUYER || mType == TYPE_5) && !field_0xe51) {
                daNpcF_offTmpBit(0xB);
                daNpcF_offTmpBit(0xC);
            }

            field_0xe51 = false;
            mMode = 2;
            // fallthrough
        case 2:
            if (field_0x9ea || mTwilight) {
                bVar1 = TRUE;
            } else {
                setLookMode(LOOK_PLAYER_TALK);
                mActorMngr[0].entry(daPy_getPlayerActorClass());

                if (mCurAngle.y == fopAcM_searchPlayerAngleY(this)) {
                    bVar1 = TRUE;
                } else if (step(fopAcM_searchPlayerAngleY(this), 14, 8, 15)) {
                    setExpression(EXPR_NONE, -1.0f);
                    setMotion(MOT_WAIT_A, -1.0f, 0);
                    mTurnMode = 0;
                }
            }

            if (bVar1 && talkProc(NULL, TRUE, NULL)) {
                rv = TRUE;

                if (mFlow.getEventId(&i_itemNo) == 1) {
                    mItemID = fopAcM_createItemForPresentDemo(&current.pos, i_itemNo, 0, -1, -1, NULL, NULL);

                    if (mItemID != fpcM_ERROR_PROCESS_ID_e) {
                        s16 i_eventID = dComIfGp_getEventManager().getEventIdx(this, "DEFAULT_GETITEM", 0xFF);
                        dComIfGp_getEvent()->reset(this);
                        fopAcM_orderChangeEventId(this, i_eventID, 1, 0xFFFF);
                        field_0x9ec = true;
                        rv = TRUE;
                        mOrderNewEvt = true;
                        field_0xe51 = true;
                    }
                }
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

BOOL daNpc_grC_c::test(void* param_1) {
    switch (mMode) {
        case 0:
            speedF = 0.0f;
            speed.setall(0.0f);
            mMode = 2;
            // fallthrough
        case 2:
            if (daNpc_grC_Param_c::m.common.face_expression != mExpression) {
                setExpression(daNpc_grC_Param_c::m.common.face_expression, daNpc_grC_Param_c::m.common.morf_frame);
            }

            setMotion(daNpc_grC_Param_c::m.common.motion, daNpc_grC_Param_c::m.common.morf_frame, 0);
            setLookMode(daNpc_grC_Param_c::m.common.look_mode);
            mOrderEvtNo = 0;
            attention_info.flags = 0;
            break;

        case 3:
            break;
    }

    return TRUE;
}

void daNpc_grC_c::setPrtcl() {
    cXyz sp34(1.0f, 1.0f, 1.0f);
    f32 fVar1 = 1.0f;
    cXyz sp40(mHeadPos);

    sp40.y = current.pos.y;
    mPaPo.setEffectCenter(&tevStr, &sp40, 1, 0x40002, NULL, NULL, &sp34, fopAcM_GetRoomNo(this), fVar1, speedF);
}

static int daNpc_grC_Create(void* a_this) {
    daNpc_grC_c* i_this = (daNpc_grC_c*)a_this;
    return i_this->create();
}

static int daNpc_grC_Delete(void* a_this) {
    daNpc_grC_c* i_this = (daNpc_grC_c*)a_this;
    return i_this->Delete();
}

static int daNpc_grC_Execute(void* a_this) {
    daNpc_grC_c* i_this = (daNpc_grC_c*)a_this;
    return i_this->Execute();
}

static int daNpc_grC_Draw(void* a_this) {
    daNpc_grC_c* i_this = (daNpc_grC_c*)a_this;
    return i_this->Draw();
}

static int daNpc_grC_IsDelete(void* a_this) {
    return 1;
}

AUDIO_INSTANCES;

static actor_method_class daNpc_grC_MethodTable = {
    (process_method_func)daNpc_grC_Create,
    (process_method_func)daNpc_grC_Delete,
    (process_method_func)daNpc_grC_Execute,
    (process_method_func)daNpc_grC_IsDelete,
    (process_method_func)daNpc_grC_Draw,
};

actor_process_profile_definition g_profile_NPC_GRC = {
  fpcLy_CURRENT_e,        // mLayerID
  7,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_NPC_GRC,           // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(daNpc_grC_c),    // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  307,                    // mPriority
  &daNpc_grC_MethodTable, // sub_method
  0x00044100,             // mStatus
  fopAc_NPC_e,            // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};
