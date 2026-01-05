/**
 * @file d_a_npc_grm.cpp
 *
 */

#include "d/dolzel_rel.h"  // IWYU pragma: keep

#include "d/actor/d_a_npc_grm.h"

enum grA_Base_RES_File_ID {
    /* BCK */
    /* 0x05 */ BCK_GRA_A_F_GETUPTALK = 0x5,
    /* 0x06 */ BCK_GRA_A_FH_GETUP,
    /* 0x07 */ BCK_GRA_F_CHEERFUL,
    /* 0x08 */ BCK_GRA_F_CHEERFUL_T,
    /* 0x09 */ BCK_GRA_F_GRUMPY,
    /* 0x0A */ BCK_GRA_F_GRUMPY_T,
    /* 0x0B */ BCK_GRA_F_RECOVER,
    /* 0x0C */ BCK_GRA_F_TALK_A,
    /* 0x0D */ BCK_GRA_F_TALK_B,
    /* 0x0E */ BCK_GRA_F_WAIT_A,
    /* 0x0F */ BCK_GRA_RECOVER,
    /* 0x10 */ BCK_GRA_ROTATE,
    /* 0x11 */ BCK_GRA_STEP,
    /* 0x12 */ BCK_GRA_TALK_A,
    /* 0x13 */ BCK_GRA_TALK_B,
    /* 0x14 */ BCK_GRA_TO_STONE_NORMAL,
    /* 0x15 */ BCK_GRA_WAIT_A,
    /* 0x16 */ BCK_GRA_WALK_A,

    /* BTK */
    /* 0x19 */ BTK_GRA_A = 0x19,

    /* BTP */
    /* 0x1C */ BTP_GRA_A = 0x1C,
    /* 0x1D */ BTP_RA_A_F_GETUPTALK,
    /* 0x1E */ BTP_RA_A_FH_GETUP,
    /* 0x1F */ BTP_RA_F_CHEERFUL_T,
    /* 0x20 */ BTP_RA_F_GRUMPY_T,
};

enum grA_Mdl_RES_File_ID {
    /* BMDR */
    /* 0x3 */ BMDR_GRA_A = 0x3,
};

enum RES_Name {
    /* 0x0 */ NONE,
    /* 0x1 */ GRA_BASE,
    /* 0x2 */ GRA_MDL,
};

static int l_bmdData[1][2] = {
    {BMDR_GRA_A, GRA_MDL},
};

static daNpcT_evtData_c l_evtList[3] = {
    {"", 0},
    {"DEFAULT_GETITEM", 0},
    {NULL, 0},
};

static char* l_resNameList[3] = {
    "",
    "grA_base",
    "grA_mdl",
};

static s8 l_loadResPtrn0[3] = {2, 1, -1};

static s8* l_loadResPtrnList[1] = {l_loadResPtrn0};

static daNpcT_faceMotionAnmData_c l_faceMotionAnmData[8] = {
    {-1, J3DFrameCtrl::EMode_NONE, NONE, BTP_GRA_A, J3DFrameCtrl::EMode_LOOP, GRA_BASE, TRUE},
    {BCK_GRA_F_WAIT_A, J3DFrameCtrl::EMode_LOOP, GRA_BASE, BTP_GRA_A, J3DFrameCtrl::EMode_LOOP,
     GRA_BASE, TRUE},
    {BCK_GRA_F_TALK_A, J3DFrameCtrl::EMode_NONE, GRA_BASE, BTP_GRA_A, J3DFrameCtrl::EMode_LOOP,
     GRA_BASE, TRUE},
    {BCK_GRA_F_TALK_B, J3DFrameCtrl::EMode_NONE, GRA_BASE, BTP_GRA_A, J3DFrameCtrl::EMode_LOOP,
     GRA_BASE, TRUE},
    {BCK_GRA_F_GRUMPY, J3DFrameCtrl::EMode_LOOP, GRA_BASE, BTP_GRA_A, J3DFrameCtrl::EMode_LOOP,
     GRA_BASE, TRUE},
    {BCK_GRA_F_CHEERFUL_T, J3DFrameCtrl::EMode_NONE, GRA_BASE, BTP_RA_F_CHEERFUL_T,
     J3DFrameCtrl::EMode_NONE, GRA_BASE, TRUE},
    {BCK_GRA_F_GRUMPY, J3DFrameCtrl::EMode_LOOP, GRA_BASE, BTP_GRA_A, J3DFrameCtrl::EMode_LOOP,
     GRA_BASE, TRUE},
    {BCK_GRA_F_GRUMPY_T, J3DFrameCtrl::EMode_NONE, GRA_BASE, BTP_RA_F_GRUMPY_T,
     J3DFrameCtrl::EMode_NONE, GRA_BASE, TRUE},

};

enum FaceMotion {
    /* 0x0 */ FACE_TALK_A,
    /* 0x4 */ FACE_WAIT_A = 4,
};

enum Motion {
    /* 0x0 */ MOT_WAIT_A,
    /* 0x3 */ MOT_TALK_A = 0x3,
    /* 0x4 */ MOT_TALK_B,
};

static daNpcT_motionAnmData_c l_motionAnmData[3] = {
    {BCK_GRA_WAIT_A, J3DFrameCtrl::EMode_LOOP, GRA_BASE, BTK_GRA_A, J3DFrameCtrl::EMode_NONE,
     GRA_BASE, 0, 0},
    {BCK_GRA_TALK_A, J3DFrameCtrl::EMode_NONE, GRA_BASE, BTK_GRA_A, J3DFrameCtrl::EMode_NONE,
     GRA_BASE, 0, 0},
    {BCK_GRA_TALK_B, J3DFrameCtrl::EMode_NONE, GRA_BASE, BTK_GRA_A, J3DFrameCtrl::EMode_NONE,
     GRA_BASE, 0, 0},
};

static daNpcT_MotionSeqMngr_c::sequenceStepData_c l_faceMotionSequenceData[20] = {
    {2, -1, 1}, {1, 0, 0},  {-1, 0, 0}, {-1, 0, 0}, {3, -1, 1}, {1, 0, 0},  {-1, 0, 0},
    {-1, 0, 0}, {7, -1, 1}, {6, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {5, -1, 1}, {4, -1, 0},
    {-1, 0, 0}, {-1, 0, 0}, {1, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},

};
static daNpcT_MotionSeqMngr_c::sequenceStepData_c l_motionSequenceData[20] = {
    {0, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {0, -1, 0}, {-1, 0, 0}, {-1, 0, 0},
    {-1, 0, 0}, {0, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {1, -1, 1}, {0, 0, 0},
    {-1, 0, 0}, {-1, 0, 0}, {2, -1, 1}, {0, 0, 0},  {-1, 0, 0}, {-1, 0, 0},
};

char* daNpc_grM_c::mCutNameList[2] = {
    "",
    "TALK_SPA",
};

daNpc_grM_c::cutFunc daNpc_grM_c::mCutList[2] = {
    NULL,
    &daNpc_grM_c::cutTalkSpa,
};

daNpc_grM_c::~daNpc_grM_c() {
    deleteObject();

    if (heap != NULL) {
        mpMorf[0]->stopZelAnime();
    }

    deleteRes(l_loadResPtrnList[mType], (const char**)l_resNameList);
}

daNpc_grM_HIOParam const daNpc_grM_Param_c::m = {
    300.0f, -3.0f,  1.0f,   600.0f, 255.0f, 260.0f, 35.0f, 70.0f, 0.0f, 0.0f,  30.0f,
    -30.0f, 30.0f,  -10.0f, 20.0f,  -20.0f, 0.6f,   12.0f, 8,     6,    8,     6,
    0.0f,   0.0f,   0.0f,   0.0f,   60,     8,      0,     0,     0,    false, false,
    4.0f,   -20.0f, 0.0f,   -20.0f, 20.0f,  40.0f,  20.0f, 110.0f};

cPhs__Step daNpc_grM_c::create() {
    daNpcT_ct(this, daNpc_grM_c, l_faceMotionAnmData, l_motionAnmData, l_faceMotionSequenceData, 4,
              l_motionSequenceData, 4, l_evtList, l_resNameList);

    mType = getType();
    mFlowNodeNo = getFlowNodeNo();
    mTwilight = false;

    cPhs__Step phase = (cPhs__Step)loadRes(l_loadResPtrnList[mType], (const char**)l_resNameList);
    if (phase == cPhs_COMPLEATE_e) {
        if (!fopAcM_entrySolidHeap(this, createHeapCallBack, 0x3730)) {
            return cPhs_ERROR_e;
        }

        OS_REPORT("\t(%s:%d) flowNo:%d, MaxItem:%d, group:%d<%08x> ");

        if (isDelete()) {
            OS_REPORT("===>isDelete:TRUE\n");

            return cPhs_ERROR_e;
        }

        OS_REPORT("\n");

        J3DModel* model = mpMorf[0]->getModel();
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
        mCcStts.Init(daNpc_grM_Param_c::m.common.weight, 0, this);
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

int daNpc_grM_c::CreateHeap() {
    int bmdIdx = mTwilight == true ? NONE : NONE;

    J3DModelData* mdlData_p = static_cast<J3DModelData*>(
        dComIfG_getObjectRes(l_resNameList[l_bmdData[bmdIdx][1]], l_bmdData[bmdIdx][0]));

    JUT_ASSERT(701, NULL != mdlData_p);

    mpMorf[0] = new mDoExt_McaMorfSO(mdlData_p, NULL, NULL, NULL, -1, 1.0f, 0, -1, &mSound, 0x80000,
                                     0x11020284);
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

    mpMatAnm[0] = NULL;

    if (setFaceMotionAnm(2, false) && setMotionAnm(0, 0.0f, FALSE)) {
        return 1;
    }

    return 0;
}

int daNpc_grM_c::Delete() {
    this->~daNpc_grM_c();
    return 1;
}

int daNpc_grM_c::Execute() {
    if (!mCreating && !checkShopOpen() && mType == 0 && field_0x10dd == 0) {
        initShopSystem();
        setSellItemMax(getMaxNumItem());
        field_0xf60 = -1;
        mShopCamAction.setCamDataIdx(0);
        mShopCamAction.setCamAction(NULL);
        field_0x10dd = 1;
    }

    execute();

    if (field_0x10dd != 0 && searchItemActor()) {
        mShopCamAction.move();
    }

    return 1;
}

int daNpc_grM_c::Draw() {
    if (mpMatAnm[0] != NULL) {
        J3DModelData* modelData = mpMorf[0]->getModel()->getModelData();
        modelData->getMaterialNodePointer(getEyeballMaterialNo())->setMaterialAnm(mpMatAnm[0]);
    }
    return draw(FALSE, FALSE, mRealShadowSize, NULL, 100.0f, FALSE, FALSE, FALSE);
}

int daNpc_grM_c::createHeapCallBack(fopAc_ac_c* param_0) {
    daNpc_grM_c* i_this = (daNpc_grM_c*)param_0;
    return i_this->CreateHeap();
}

int daNpc_grM_c::ctrlJointCallBack(J3DJoint* i_joint, int param_2) {
    if (param_2 == 0) {
        J3DModel* model = j3dSys.getModel();
        daNpc_grM_c* i_this = (daNpc_grM_c*)model->getUserArea();

        if (i_this != NULL) {
            i_this->ctrlJoint(i_joint, model);
        }
    }

    return 1;
}

bool daNpc_grM_c::getType() {
    return false;
}

int daNpc_grM_c::getFlowNodeNo() {
    u16 rv = home.angle.x;

    if (rv == 0xFFFF) {
        return -1;
    }

    return rv;
}

u8 daNpc_grM_c::getMaxNumItem() {
    return (fopAcM_GetParam(this) & 0xF000000) >> 24;
}

BOOL daNpc_grM_c::isDelete() {
    return mType ? 1 : 0;
}

void daNpc_grM_c::reset() {
    initialize();
    int size = (u8*)&field_0x10e0 - (u8*)&mNextAction;
    memset(&mNextAction, 0, size);
    if (mpMatAnm[0] != NULL) {
        mpMatAnm[0]->initialize();
    }
    setAngle(home.angle.y);
}

void daNpc_grM_c::afterJntAnm(int param_0) {
    if (param_0 == 1) {
        mDoMtx_stack_c::YrotM(-mStagger.getAngleZ(1));
        mDoMtx_stack_c::ZrotM(mStagger.getAngleX(1));
    } else if (param_0 == 4) {
        mDoMtx_stack_c::YrotM(-mStagger.getAngleZ(0));
        mDoMtx_stack_c::ZrotM(mStagger.getAngleX(0));
    }
}

void daNpc_grM_c::setParam() {
    u32 uVar1 = (fopAc_AttnFlag_SPEAK_e | fopAc_AttnFlag_TALK_e);

    if (!mTwilight && daPy_py_c::checkNowWolf()) {
        uVar1 = 0;
    }

    if (field_0x10dc != 0) {
        if (field_0x10d8 == 2) {
            mShopCamAction.Reset();
        } else {
            mShopCamAction.EventRecoverNotime();
        }

        field_0x10dc = 0;
    }

    selectAction();

    srchActors();
    s16 talk_distance = daNpc_grM_Param_c::m.common.talk_distance;
    s16 talk_angle = daNpc_grM_Param_c::m.common.talk_angle;
    s16 attention_distance = daNpc_grM_Param_c::m.common.attention_distance;
    s16 attention_angle = daNpc_grM_Param_c::m.common.attention_angle;

    attention_info.distances[fopAc_attn_LOCK_e] =
        daNpcT_getDistTableIdx(attention_distance, attention_angle);
    attention_info.distances[fopAc_attn_TALK_e] = attention_info.distances[fopAc_attn_LOCK_e];
    attention_info.distances[fopAc_attn_SPEAK_e] =
        daNpcT_getDistTableIdx(talk_distance, talk_angle);
    attention_info.flags = uVar1;

    scale.set(daNpc_grM_Param_c::m.common.scale, daNpc_grM_Param_c::m.common.scale,
              daNpc_grM_Param_c::m.common.scale);
    mCcStts.SetWeight(daNpc_grM_Param_c::m.common.weight);
    mCylH = daNpc_grM_Param_c::m.common.height;
    mWallR = daNpc_grM_Param_c::m.common.width;
    mAcchCir.SetWallR(mWallR);
    mAcchCir.SetWallH(daNpc_grM_Param_c::m.common.knee_length);
    mRealShadowSize = daNpc_grM_Param_c::m.common.real_shadow_size;
    gravity = daNpc_grM_Param_c::m.common.gravity;
    mExpressionMorfFrame = daNpc_grM_Param_c::m.common.expression_morf_frame;
    mMorfFrames = daNpc_grM_Param_c::m.common.morf_frame;
}

void daNpc_grM_c::setAfterTalkMotion() {
    int i_index;
    switch (mFaceMotionSeqMngr.getNo()) {
    case FACE_TALK_A:
        if (mMotionSeqMngr.getNo() == MOT_TALK_A || mMotionSeqMngr.getNo() == MOT_TALK_B) {
            return;
        } else {
            i_index = FACE_WAIT_A;
        }
        break;
    default:
        i_index = FACE_WAIT_A;
        break;
    }
    mFaceMotionSeqMngr.setNo(i_index, -1.0f, FALSE, 0);
}

void daNpc_grM_c::srchActors() {
    /* empty function */
}

BOOL daNpc_grM_c::evtProc() {
    BOOL ret = FALSE;
    if (dComIfGp_event_runCheck()) {
        if (eventInfo.checkCommandTalk()) {
            if (!checkChangeEvt()) {
                evtTalk();
            }
            ret = TRUE;
        } else if (eventInfo.checkCommandDemoAccrpt() &&
                   dComIfGp_getEventManager().endCheck(mEvtId))
        {
            if (evtEndProc()) {
                dComIfGp_event_reset();
                mEvtId = -1;
            }
        } else {
            if (!strcmp(dComIfGp_getEventManager().getRunEventName(), "DEFAULT_GETITEM") &&
                mItemPartnerId != fpcM_ERROR_PROCESS_ID_e)
            {
                dComIfGp_event_setTalkPartner(NULL);
                dComIfGp_event_setItemPartnerId(mItemPartnerId);
                mItemPartnerId = fpcM_ERROR_PROCESS_ID_e;
            }
            ret = evtCutProc();
        }

        int tmp = field_0xdb4;
        int face_motion, motion;
        if (ctrlMsgAnm(&face_motion, &motion, this, field_0xe2d)) {
            if (field_0xe26) {
                mFaceMotionSeqMngr.setNo(face_motion, -1.0f, TRUE, NULL);
                mMotionSeqMngr.setNo(motion, -1.0f, TRUE, NULL);
            }
        } else if (tmp != 0 && field_0xe26) {
            setAfterTalkMotion();
        }
        switch (mFaceMotionSeqMngr.getNo()) {
        case FACE_TALK_A:
            if ((mMotionSeqMngr.getNo() == MOT_TALK_A || mMotionSeqMngr.getNo() == MOT_TALK_B) &&
                (mFaceMotionSeqMngr.getStepNo() == 0 || mMotionSeqMngr.getStepNo() == 0))
            {
                mAnmFlags &= ~ANM_FLAG_4000;
            } else {
            }
            break;
        default:
            break;
        }
    } else {
        if (mStaffId != -1) {
            mMode = MODE_INIT;
            mStaffId = -1;
        }

        field_0xdb4 = 0;
        field_0xe26 = true;
    }

    return ret;
}
BOOL daNpc_grM_c::evtTalk() {
    if (field_0x10dd != 0) {
        if (chkAction(&daNpc_grM_c::shop)) {
            (this->*mAction)(NULL);
        } else if (dComIfGp_event_chkTalkXY() == 0 || dComIfGp_evmng_ChkPresentEnd()) {
            mShopCamAction.shop_cam_action_init();
            setAction(&daNpc_grM_c::shop);
        }
    } else if (chkAction(&daNpc_grM_c::talk)) {
        (this->*mAction)(NULL);
    } else if (dComIfGp_event_chkTalkXY() == 0 || dComIfGp_evmng_ChkPresentEnd()) {
        setAction(&daNpc_grM_c::talk);
    }

    return TRUE;
}

BOOL daNpc_grM_c::evtCutProc() {
    BOOL rv = FALSE;
    int staffId = dComIfGp_getEventManager().getMyStaffId("grM", this, -1);

    if (staffId != -1) {
        mStaffId = staffId;
        int actIdx = dComIfGp_getEventManager().getMyActIdx(mStaffId, mCutNameList, 2, 0, 0);
        if ((this->*mCutList[actIdx])(mStaffId)) {
            dComIfGp_getEventManager().cutEnd(mStaffId);
        }

        rv = TRUE;
    }

    return rv;
}

void daNpc_grM_c::action() {
    if (field_0x10dd == 0) {
        fopAc_ac_c* actor_p = hitChk(&mCyl, -1);
        if (actor_p != NULL) {
            mStagger.setParam(this, actor_p, mCurAngle.y);
            setDamage(0, 4, 0);
            mStagger.setPower(0.0f);
            mDamageTimerStart = 0;
        }
    }

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

void daNpc_grM_c::beforeMove() {
    if (checkHide() || mNoDraw) {
        attention_info.flags = 0;
    }
}

void daNpc_grM_c::setAttnPos() {
    cXyz sp48(0.0f, 30.0f, 0.0f);

    mStagger.calc(FALSE);
    mJntAnm.setParam(
        this, mpMorf[0]->getModel(), &sp48, getBackboneJointNo(), getNeckJointNo(),
        getHeadJointNo(), daNpc_grM_Param_c::m.common.body_angleX_min,
        daNpc_grM_Param_c::m.common.body_angleX_max, daNpc_grM_Param_c::m.common.body_angleY_min,
        daNpc_grM_Param_c::m.common.body_angleY_max, daNpc_grM_Param_c::m.common.head_angleX_min,
        daNpc_grM_Param_c::m.common.head_angleX_max, daNpc_grM_Param_c::m.common.head_angleY_min,
        daNpc_grM_Param_c::m.common.head_angleY_max,
        daNpc_grM_Param_c::m.common.neck_rotation_ratio, 0.0f, NULL);
    mJntAnm.calcJntRad(0.2f, 1.0f, cM_s2rad((s16)(mCurAngle.y - field_0xd7e.y)));

    J3DModelData* modelData = mpMorf[0]->getModel()->getModelData();
    for (u16 i = 0; i < modelData->getJointNum(); i++) {
        modelData->getJointNodePointer(i)->setCallBack(ctrlJointCallBack);
    }

    setMtx();
    mDoMtx_stack_c::copy(mpMorf[0]->getModel()->getAnmMtx(getHeadJointNo()));
    mDoMtx_stack_c::multVec(&sp48, &eyePos);
    mJntAnm.setEyeAngleX(eyePos, 1.0f, 0);
    mJntAnm.setEyeAngleY(eyePos, mCurAngle.y, FALSE, 1.0f, 0);
    attention_info.position = current.pos;
    attention_info.position.y += daNpc_grM_Param_c::m.common.attention_offset;
}

void daNpc_grM_c::setCollision() {
    cXyz pos;
    if (!mHide) {
        if (mTwilight == true && !dComIfGp_event_runCheck()) {
            mCyl.SetCoSPrm(105);
        } else {
            mCyl.SetCoSPrm(121);
        }

        if (mStagger.checkStagger()) {
            mCyl.SetTgType(0);
            mCyl.SetTgSPrm(0);
        } else {
            mCyl.SetTgType(0xD8FBFDFF);
            mCyl.SetTgSPrm(0x1F);
            mCyl.OnTgNoHitMark();
        }

        f32 height = mCylH;
        f32 width = mWallR;
        pos = current.pos;
        mCyl.SetH(height);
        mCyl.SetR(width);
        mCyl.SetC(pos);
        dComIfG_Ccsp()->Set(&mCyl);
    }

    mCyl.ClrCoHit();
    mCyl.ClrTgHit();
}

int daNpc_grM_c::drawDbgInfo() {
    return false;
}

void daNpc_grM_c::drawOtherMdl() {
    if (field_0x10dd != 0) {
        cXyz sp18;
        itemRotate();
        sp18.set(0.0f, 0.0f, 0.0f);
        itemZoom(&sp18);
        drawCursor();
    }
}

BOOL daNpc_grM_c::selectAction() {
    mNextAction = NULL;
    mNextAction = &daNpc_grM_c::tend;
    return TRUE;
}

BOOL daNpc_grM_c::chkAction(actionFunc action) {
    return mAction == action;
}

BOOL daNpc_grM_c::setAction(actionFunc action) {
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

int daNpc_grM_c::cutTalkSpa(int param_1) {
    int uVar4 = 0;
    int iVar3 = -1;
    dEvent_manager_c& manager = dComIfGp_getEventManager();

    int* piVar1 = dComIfGp_evmng_getMyIntegerP(param_1, "prm");

    if (piVar1 != NULL) {
        iVar3 = *piVar1;
    }
    if (manager.getIsAddvance(param_1)) {
        switch (iVar3) {
        case 0:
            break;
        case 1:
            if (daNpcT_chkEvtBit(6) && daNpcT_chkEvtBit(0x3E) == FALSE) {
                mEventTimer = 0x3C;
                mJntAnm.lookPlayer(0);
            } else {
                mEventTimer = 0;
            }
            break;
        }
    }
    switch (iVar3) {
    case 0:
        uVar4 = 1;
        break;
    case 1:
        if (!cLib_calcTimer(&mEventTimer)) {
            uVar4 = 1;
            mJntAnm.lookNone(0);
        }
        break;
    default:
        uVar4 = 1;
        break;
    }
    return uVar4;
}

int daNpc_grM_c::tend(void* param_0) {
    BOOL bVar1;

    switch (mMode) {
    case 0:
    case 1:
        mFaceMotionSeqMngr.setNo(FACE_WAIT_A, -1.0f, FALSE, 0);
        mMotionSeqMngr.setNo(MOT_WAIT_A, -1.0f, FALSE, 0);

        mMode = 2;
        // fallthrough
    case 2:
        if (field_0x10d8 == 2) {
            mSpeakEvent = true;
            field_0xe33 = true;
        }

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
        break;

    case 3:
        break;
    }

    return 1;
}

int daNpc_grM_c::talk(void* param_0) {
    BOOL bVar1 = FALSE;

    switch (mMode) {
    case 0:
    case 1:
        initTalk(mFlowNodeNo, NULL);
        mMode = 2;
        // fallthrough
    case 2:
        if (mTwilight) {
            bVar1 = TRUE;
        } else {
            mJntAnm.lookPlayer(0);

            if (&daNpc_grM_c::tend == mNextAction) {
                bVar1 = TRUE;
            } else if (mCurAngle.y != fopAcM_searchPlayerAngleY(this)) {
                step(fopAcM_searchPlayerAngleY(this), -1, -1, 15, 0);
            } else {
                bVar1 = TRUE;
            }
        }

        if (bVar1 && talkProc(NULL, FALSE, NULL, FALSE)) {
            mPlayerActorMngr.entry(daPy_getPlayerActorClass());
            dComIfGp_event_reset();
            mMode = 3;
        }
        break;

    case 3:
        break;
    }

    return 0;
}

int daNpc_grM_c::shop(void* param_0) {
    switch (mMode) {
    case 0:
    case 1:
        if (field_0x10d8 == 2) {
            shop_init(true);
        } else {
            mShopCamAction.Save();
            initTalk(mFlowNodeNo, NULL);
            shop_init(false);
        }

        mJntAnm.lookCamera(0);
        mMode = 2;
        // fallthrough
    case 2:
        field_0x10d8 = shop_process(this, &mFlow);

        if (field_0x10d8 != 0) {
            mPlayerActorMngr.entry(daPy_getPlayerActorClass());
            dComIfGp_event_reset();
            field_0x10dc = 1;
            mMode = 3;
        }
        break;

    case 3:
        break;
    }

    return 0;
}

static cPhs__Step daNpc_grM_Create(void* param_0) {
    return static_cast<daNpc_grM_c*>(param_0)->create();
}

static int daNpc_grM_Delete(void* param_0) {
    return static_cast<daNpc_grM_c*>(param_0)->Delete();
}

static int daNpc_grM_Execute(void* param_0) {
    return static_cast<daNpc_grM_c*>(param_0)->Execute();
}

static int daNpc_grM_Draw(void* param_0) {
    return static_cast<daNpc_grM_c*>(param_0)->Draw();
}

static BOOL daNpc_grM_IsDelete(void* param_0) {
    return TRUE;
}

static daNpc_grM_Param_c l_HIO;

static actor_method_class daNpc_grM_MethodTable = {
    (process_method_func)daNpc_grM_Create,  (process_method_func)daNpc_grM_Delete,
    (process_method_func)daNpc_grM_Execute, (process_method_func)daNpc_grM_IsDelete,
    (process_method_func)daNpc_grM_Draw,
};

actor_process_profile_definition g_profile_NPC_GRM = {
    fpcLy_CURRENT_e,         // mLayerID
    7,                       // mListID
    fpcPi_CURRENT_e,         // mListPrio
    PROC_NPC_GRM,            // mProcName
    &g_fpcLf_Method.base,    // sub_method
    sizeof(daNpc_grM_c),     // mSize
    0,                       // mSizeOther
    0,                       // mParameters
    &g_fopAc_Method.base,    // sub_method
    309,                     // mPriority
    &daNpc_grM_MethodTable,  // sub_method
    0x00044107,              // mStatus
    fopAc_NPC_e,             // mActorType
    fopAc_CULLBOX_CUSTOM_e,  // cullType
};
