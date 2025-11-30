/**
 * @file d_a_npc_clerka.cpp
 *
 */

#include "d/dolzel_rel.h"  // IWYU pragma: keep

#include "d/actor/d_a_npc_clerka.h"
#include "d/d_msg_object.h"

const daNpc_clerkA_HIOParam daNpc_clerkA_Param_c::m = {
    220.0f,  // attention_offset
    -3.0f,   // gravity
    1.0f,    // scale
    400.0f,  // real_shadow_size
    255.0f,  // weight
    200.0f,  // height
    35.0f,   // knee_length
    30.0f,   // width
    0.0f,    // body_angleX_max
    0.0f,    // body_angleX_min
    10.0f,   // body_angleY_max
    -10.0f,  // body_angleY_min
    30.0f,   // head_angleX_max
    -10.0f,  // head_angleX_min
    60.0f,   // head_angleY_max
    -60.0f,  // head_angleY_min
    0.6f,    // neck_rotation_ratio
    12.0f,   // morf_frame
    3,       // talk_distance
    6,       // talk_angle
    5,       // attention_distance
    6,       // attention_angle
    110.0f,  // fov
    0.0f,    // search_distance
    0.0f,    // search_height
    0.0f,    // search_depth
    60,      // attention_time
    8,       // damage_time
    0,       // face_expression
    0,       // motion
    0,       // look_mode
    0,       // debug_mode_ON
    0,       // debug_info_ON
    4.0f,    // expression_morf_frame
    -20.0f,  // box_min_x
    0.0f,    // box_min_y
    -20.0f,  // box_min_z
    20.0f,   // box_max_x
    40.0f,   // box_max_y
    20.0f,   // box_max_z
    110.0f,  // box_offset
    120,     // field_0x8c
    0,       // field_0x8f
};

#if DEBUG
daNpc_clerkA_HIO_c::daNpc_clerkA_HIO_c() {
    m = daNpc_clerkA_Param_c::m;
}

void daNpc_clerkA_HIO_c::listenPropertyEvent(const JORPropertyEvent* event) {
    // TODO
}

void daNpc_clerkA_HIO_c::genMessage(JORMContext* ctext) {
    // TODO
}
#endif

static int l_bmdData[1][2] = {
    {19, 1},
};

static daNpcT_evtData_c l_evtList[3] = {
    {"", 0},
    {"DEFAULT_GETITEM", 0},
    {"NO_RESPONSE", 0},
};

static char* l_resNameList[2] = {
    "",
    "clerkA",
};

static s8 l_loadResPtrn0[2] = {1, -1};

static s8* l_loadResPtrnList[2] = {
    l_loadResPtrn0,
    l_loadResPtrn0,
};

static daNpcT_faceMotionAnmData_c l_faceMotionAnmData[5] = {
    {-1, 0, 0, 25, 2, 1, 1}, {6, 0, 1, 26, 0, 1, 0}, {7, 0, 1, 27, 0, 1, 0},
    {8, 0, 1, 28, 0, 1, 0},  {9, 2, 1, 29, 2, 1, 0},
};

static daNpcT_motionAnmData_c l_motionAnmData[7] = {
    {15, 2, 1, 22, 0, 1, 1, 0}, {16, 2, 1, 22, 0, 1, 1, 0}, {10, 0, 1, 22, 0, 1, 1, 0},
    {11, 0, 1, 22, 0, 1, 1, 0}, {12, 0, 1, 22, 0, 1, 1, 0}, {13, 0, 1, 22, 0, 1, 1, 0},
    {14, 0, 1, 22, 0, 1, 1, 0},
};

static daNpcT_MotionSeqMngr_c::sequenceStepData_c l_faceMotionSequenceData[20] = {
    {1, -1, 1}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {2, -1, 1}, {0, -1, 0}, {-1, 0, 0},
    {-1, 0, 0}, {3, -1, 1}, {4, 0, 0},  {-1, 0, 0}, {-1, 0, 0}, {4, -1, 0}, {-1, 0, 0},
    {-1, 0, 0}, {-1, 0, 0}, {0, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
};

static daNpcT_MotionSeqMngr_c::sequenceStepData_c l_motionSequenceData[28] = {
    {0, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {3, -1, 1}, {0, 0, 0},  {-1, 0, 0},
    {-1, 0, 0}, {4, -1, 1}, {0, 0, 0},  {-1, 0, 0}, {-1, 0, 0}, {5, -1, 1}, {1, 0, 0},
    {-1, 0, 0}, {-1, 0, 0}, {1, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {2, -1, 1},
    {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {6, -9, 1}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
};

char* daNpc_clerkA_c::mCutNameList[1] = {""};

daNpc_clerkA_c::cutFunc daNpc_clerkA_c::mCutList[1] = {NULL};

daNpc_clerkA_c::~daNpc_clerkA_c() {
    deleteObject();

    if (heap != NULL) {
        mpMorf[0]->stopZelAnime();
    }

    deleteRes(l_loadResPtrnList[mType], (const char**)l_resNameList);
}

int daNpc_clerkA_c::create() {
    daNpcT_ct(this, daNpc_clerkA_c, l_faceMotionAnmData, l_motionAnmData, l_faceMotionSequenceData,
              4, l_motionSequenceData, 4, l_evtList, l_resNameList);

    mType = getType();
    mFlowNodeNo = getFlowNodeNo();
    mTwilight = dKy_darkworld_check();

    int phase = loadRes(l_loadResPtrnList[mType], (const char**)l_resNameList);
    if (phase == cPhs_COMPLEATE_e) {
        if (!fopAcM_entrySolidHeap(this, createHeapCallBack, 0x39F0)) {
            return cPhs_ERROR_e;
        }

        if (isDelete()) {
            return cPhs_ERROR_e;
        }

        J3DModelData* modelData = mpMorf[0]->getModel()->getModelData();
        fopAcM_SetMtx(this, mpMorf[0]->getModel()->getBaseTRMtx());
        fopAcM_setCullSizeBox(this, -300.0f, -50.0f, -300.0f, 300.0f, 450.0f, 300.0f);
        mSound.init(&current.pos, &eyePos, 3, 1);

        mAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1, &mAcchCir,
                  fopAcM_GetSpeed_p(this), fopAcM_GetAngle_p(this), fopAcM_GetShapeAngle_p(this));
        mCcStts.Init(mpHIO->m.common.weight, 0, this);
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

        reset();
        mCreating = true;
        Execute();
        mCreating = false;
    }
    return phase;
}

int daNpc_clerkA_c::CreateHeap() {
    int res_idx = l_bmdData[0][1];
    int bmd_idx = l_bmdData[0][0];
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes(l_resNameList[res_idx], bmd_idx);

    mpMorf[0] =
        new mDoExt_McaMorfSO(modelData, NULL, NULL, NULL, -1, 1.0f, 0, -1, &mSound, 0, 0x11020284);
    if (mpMorf[0] != NULL && mpMorf[0]->getModel() == NULL) {
        mpMorf[0]->stopZelAnime();
        mpMorf[0] = NULL;
    }
    if (mpMorf[0] == NULL) {
        return 0;
    }

    J3DModel* model = mpMorf[0]->getModel();
    for (u16 i = 0; i < modelData->getJointNum(); i++) {
        modelData->getJointNodePointer(i)->setCallBack(ctrlJointCallBack);
    }
    model->setUserArea((uintptr_t)this);

    mpMatAnm[0] = new daNpcT_MatAnm_c();
    if (mpMatAnm[0] == NULL) {
        return 0;
    }

    if (setFaceMotionAnm(1, false) && setMotionAnm(0, 0.0f, FALSE)) {
        return 1;
    }

    return 0;
}

int daNpc_clerkA_c::Delete() {
    fpc_ProcID id = fopAcM_GetID(this);
    this->~daNpc_clerkA_c();
    return 1;
}

int daNpc_clerkA_c::Execute() {
    if (!mCreating && !checkShopOpen() && mType == TYPE_SHOP && mShopFlag == 0) {
        initShopSystem();
        setSellItemMax(getMaxNumItem());
        field_0xf60 = -1;
        mShopCamAction.setCamDataIdx(0);
        mShopCamAction.setCamAction(NULL);
        mShopFlag = 1;
    }

    execute();

    if (mShopFlag != 0 && searchItemActor()) {
        mShopCamAction.move();
    }
    return 1;
}

int daNpc_clerkA_c::Draw() {
    if (mpMatAnm[0] != NULL) {
        J3DModelData* modelData = mpMorf[0]->getModel()->getModelData();
        modelData->getMaterialNodePointer(getEyeballMaterialNo())->setMaterialAnm(mpMatAnm[0]);
    }
    return draw(FALSE, FALSE, mRealShadowSize, NULL, 100.0f, FALSE, FALSE, FALSE);
}

int daNpc_clerkA_c::createHeapCallBack(fopAc_ac_c* i_this) {
    return ((daNpc_clerkA_c*)i_this)->CreateHeap();
}

int daNpc_clerkA_c::ctrlJointCallBack(J3DJoint* i_joint, int param_2) {
    if (param_2 == 0) {
        J3DModel* model = j3dSys.getModel();
        daNpc_clerkA_c* i_this = (daNpc_clerkA_c*)model->getUserArea();
        if (i_this != NULL) {
            i_this->ctrlJoint(i_joint, model);
        }
    }

    return 1;
}

u8 daNpc_clerkA_c::getType() {
    switch ((fopAcM_GetParam(this) & 0xFF)) {
    case 0:
        return TYPE_SHOP;
    }
    return TYPE_1;
}

int daNpc_clerkA_c::isDelete() {
    if (mType == TYPE_1) {
        return 0;
    }
    switch (mType) {
    case TYPE_SHOP:
        return 0;
    }
    return 1;
}

void daNpc_clerkA_c::reset() {
    initialize();

    int size = (u8*)&field_0x10f0 - (u8*)&mNextAction;

    memset(&mNextAction, 0, size);

    if (mpMatAnm[0] != NULL) {
        mpMatAnm[0]->initialize();
    }

    for (int i = 0; i < 1; i++) {
        mActorMngr[i].initialize();
    }

    setAngle(home.angle.y);
}

void daNpc_clerkA_c::afterJntAnm(int param_1) {
    if (param_1 == 1) {
        mDoMtx_stack_c::YrotM(mStagger.getAngleZ(1));
        mDoMtx_stack_c::ZrotM(-mStagger.getAngleX(1));
    } else if (param_1 == 4) {
        mDoMtx_stack_c::YrotM(mStagger.getAngleZ(0));
        mDoMtx_stack_c::ZrotM(mStagger.getAngleX(0));
    }
}

void daNpc_clerkA_c::setParam() {
    if (field_0x10ee != 0) {
        if (mShopProcess == 2) {
            mShopCamAction.Reset();
        } else {
            mShopCamAction.EventRecoverNotime();
        }

        field_0x10ee = 0;
    }
    selectAction();
    srchActors();

    u32 flags = fopAc_AttnFlag_SPEAK_e | fopAc_AttnFlag_TALK_e;
    s16 talk_distance = mpHIO->m.common.talk_distance;
    s16 talk_angle = mpHIO->m.common.talk_angle;
    s16 attention_distance = mpHIO->m.common.attention_distance;
    s16 attention_angle = mpHIO->m.common.attention_angle;

    attention_info.distances[fopAc_attn_LOCK_e] =
        daNpcT_getDistTableIdx(attention_distance, attention_angle);
    attention_info.distances[fopAc_attn_TALK_e] = attention_info.distances[fopAc_attn_LOCK_e];
    attention_info.distances[fopAc_attn_SPEAK_e] =
        daNpcT_getDistTableIdx(talk_distance, talk_angle);
    attention_info.flags = flags;

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
}

void daNpc_clerkA_c::setAfterTalkMotion() {
    int idx;

    switch (mFaceMotionSeqMngr.getNo()) {
    case FACE_MOT_UNK_1:
        return;
    case FACE_MOT_UNK_2:
        idx = FACE_MOT_UNK_3;
        break;
    default:
        idx = FACE_MOT_UNK_4;
        break;
    }

    mFaceMotionSeqMngr.setNo(idx, -1.0f, FALSE, 0);
}

void daNpc_clerkA_c::srchActors() {
    switch (mType) {
    case TYPE_SHOP:
        if (mActorMngr[0].getActorP() == NULL) {
            mActorMngr[0].entry(getShopItemTagP());
        }
        break;
    }
}

BOOL daNpc_clerkA_c::evtTalk() {
    if (mShopFlag) {
        if (chkAction(&daNpc_clerkA_c::shop)) {
            (this->*mAction)(NULL);
        } else {
            mPreItemNo = 0;
            if (dComIfGp_event_chkTalkXY()) {
                if (!dComIfGp_evmng_ChkPresentEnd()) {
                    return TRUE;
                }
                mEvtNo = 2;
                evtChange();
                return TRUE;
            } else {
                mShopCamAction.shop_cam_action_init();
                setAction(&daNpc_clerkA_c::shop);
            }
        }
    } else if (chkAction(&daNpc_clerkA_c::talk)) {
        (this->*mAction)(NULL);
    } else {
        mPreItemNo = 0;
        if (dComIfGp_event_chkTalkXY()) {
            if (!dComIfGp_evmng_ChkPresentEnd()) {
                return TRUE;
            }
            mEvtNo = 2;
            evtChange();
            return TRUE;
        } else {
            setAction(&daNpc_clerkA_c::talk);
        }
    }

    return TRUE;
}

BOOL daNpc_clerkA_c::evtCutProc() {
    BOOL rv = FALSE;

    s32 staffId = dComIfGp_getEventManager().getMyStaffId("clerkA", this, -1);
    if (staffId != -1) {
        mStaffId = staffId;
        int actIdx =
            dComIfGp_getEventManager().getMyActIdx(mStaffId, mCutNameList, 1, FALSE, FALSE);
        if ((this->*mCutList[actIdx])(mStaffId)) {
            dComIfGp_getEventManager().cutEnd(mStaffId);
        }

        rv = TRUE;
    }

    return rv;
}

void daNpc_clerkA_c::action() {
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

void daNpc_clerkA_c::beforeMove() {
    if (checkHide() || mNoDraw) {
        attention_info.flags = 0;
    }
}

void daNpc_clerkA_c::setAttnPos() {
    cXyz cStack_3c(-30.0f, 15.0f, 0.0f);
    cXyz cStack_48(0.0f, 15.0f, 0.0f);

    mStagger.calc(FALSE);
    f32 rad = cM_s2rad((s16)(mCurAngle.y - field_0xd7e.y));

    mJntAnm.setParam(this, mpMorf[0]->getModel(), &cStack_3c, getBackboneJointNo(),
                     getNeckJointNo(), getHeadJointNo(), 0.0f, 0.0f, 0.0f, 0.0f,
                     mpHIO->m.common.head_angleX_min, mpHIO->m.common.head_angleX_max,
                     mpHIO->m.common.head_angleY_min, mpHIO->m.common.head_angleY_max,
                     mpHIO->m.common.neck_rotation_ratio, rad, &cStack_48);

    mJntAnm.calcJntRad(0.2f, 1.0f, rad);
    setMtx();
    mDoMtx_stack_c::copy(mpMorf[0]->getModel()->getAnmMtx(getHeadJointNo()));
    mDoMtx_stack_c::multVec(&cStack_3c, &eyePos);

    if (mMotionSeqMngr.getNo() == MOT_UNK_3) {
        mJntAnm.setEyeAngleX(eyePos, 1.0f, 0);
        mJntAnm.setEyeAngleY(eyePos, 0x1555, FALSE, 1.0f, 0);
    } else {
        mJntAnm.setEyeAngleX(eyePos, 1.0f, 0);
        mJntAnm.setEyeAngleY(eyePos, mCurAngle.y, TRUE, 1.0f, 0);
    }

    attention_info.position = current.pos;
    attention_info.position.y += mpHIO->m.common.attention_offset;
}

void daNpc_clerkA_c::setCollision() {
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
        mCyl.SetCoSPrm(0x79);
        mCyl.SetTgType(tgType);
        mCyl.SetTgSPrm(tgSPrm);
        mCyl.OnTgNoHitMark();
        mCyl.SetH(cylH);
        mCyl.SetR(wallR);
        mCyl.SetC(pos);
        dComIfG_Ccsp()->Set(&mCyl);
    }

    mCyl.ClrCoHit();
    mCyl.ClrTgHit();
}

int daNpc_clerkA_c::drawDbgInfo() {
    return 0;
}

void daNpc_clerkA_c::drawOtherMdl() {
    if (mShopFlag) {
        cXyz pos;
        itemRotate();
        pos.set(0.0f, 0.0f, 0.0f);
        itemZoom(&pos);
        drawCursor();
    }
}

int daNpc_clerkA_c::selectAction() {
    mNextAction = NULL;

    switch (mType) {
    case TYPE_SHOP:
        mNextAction = &daNpc_clerkA_c::tend;
        break;
    default:
        mNextAction = &daNpc_clerkA_c::wait;
        break;
    }

    return TRUE;
}

int daNpc_clerkA_c::chkAction(actionFunc action) {
    return mAction == action;
}

int daNpc_clerkA_c::setAction(actionFunc action) {
    mMode = MODE_EXIT;
    if (mAction != NULL) {
        (this->*(mAction))(NULL);
    }

    mMode = MODE_ENTER;
    mAction = action;
    if (mAction != NULL) {
        (this->*(mAction))(NULL);
    }

    return 1;
}

int daNpc_clerkA_c::wait(void* param_0) {
    switch (mMode) {
    case MODE_ENTER:
    case MODE_INIT:
        if (!mStagger.checkStagger()) {
            mFaceMotionSeqMngr.setNo(FACE_MOT_UNK_4, -1.0f, FALSE, 0);
            mMotionSeqMngr.setNo(MOT_UNK_0, -1.0f, FALSE, 0);
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
                    } else if (step(home.angle.y, 4, 5, 15, 0)) {
                        mMode = MODE_INIT;
                    }
                    attention_info.flags = 0;
                } else if (!mTwilight) {
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

int daNpc_clerkA_c::tend(void* param_0) {
    int sVar1 = mpHIO->m.field_0x8c;

    switch (mMode) {
    case MODE_ENTER:
    case MODE_INIT:
        if (!mStagger.checkStagger()) {
            mFaceMotionSeqMngr.setNo(FACE_MOT_UNK_4, -1.0f, FALSE, 0);
            mMotionSeqMngr.setNo(MOT_UNK_0, -1.0f, FALSE, 0);
            int tmp = sVar1 * (cM_rnd() - 0.5f);
            field_0x10e4 = sVar1 + tmp;
            field_0x10ec = 0;
            mMode = MODE_RUN;
        }
    case MODE_RUN:
        if (mShopFlag != 0 && mShopProcess == 2) {
            mSpeakEvent = true;
            field_0xe33 = true;
        }
        if (!mStagger.checkStagger()) {
            mJntAnm.lookNone(0);
            if (cLib_calcTimer(&field_0x10e4) == 0) {
                if (field_0x10ec != 0) {
                    if (mMotionSeqMngr.checkEndSequence()) {
                        field_0x10ec--;
                        if (field_0x10ec != 0) {
                            mMotionSeqMngr.setNo(MOT_UNK_6, 0.0f, TRUE, 0);
                        } else {
                            mMode = MODE_INIT;
                        }
                    }
                } else {
                    mMotionSeqMngr.setNo(MOT_UNK_6, -1.0f, FALSE, 0);
                    field_0x10ec = 1;
                    if (field_0x10ec == 0) {
                        field_0x10ec = 1;
                    }
                }
            }
        }
        break;
    case MODE_EXIT:
        break;
    }

    return 1;
}

int daNpc_clerkA_c::talk(void* param_0) {
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
                step(fopAcM_searchPlayerAngleY(this), 4, 5, 15, 0);
            }
        }
        break;
    case MODE_EXIT:
        break;
    }

    return 0;
}

int daNpc_clerkA_c::shop(void* param_0) {
    cXyz cam_ctr_pos;
    cXyz cStack_3c;

    fopAcM_searchPlayerAngleY(this);

    switch (mMode) {
    case MODE_ENTER:
    case MODE_INIT:
        if (!mStagger.checkStagger()) {
            if (mShopProcess == 2) {
                shop_init(true);
            } else {
                mShopCamAction.Save();
                initTalk(mFlowNodeNo, NULL);
                shop_init(false);
                field_0x10e8 = 1;
                field_0xe26 = false;
            }

            mJntAnm.lookNone(1);
            if (checkStep()) {
                mStepMode = 0;
            }
            mMode = MODE_RUN;
        }
    case MODE_RUN:
        if (!mStagger.checkStagger()) {
            mShopProcess = shop_process(this, &mFlow);
            if (mShopProcess != 0) {
                mPlayerActorMngr.entry(daPy_getPlayerActorClass());
                dComIfGp_event_reset();
                mMode = MODE_EXIT;
                field_0x10ee = 1;
            }
            if (chkExplainItem() && getCursorPos()) {
                if (mMotionSeqMngr.getNo() == MOT_UNK_3) {
                    mJntAnm.lookCamera(0);
                } else {
                    field_0xd6c = mItemCtrl.getCurrentPos(getCursorPos() - 1);
                    cStack_3c.set(0.0f, 0.0f, 60.0f);
                    mDoMtx_stack_c::transS(field_0xd6c);
                    mDoMtx_stack_c::YrotM(mCurAngle.y);
                    mDoMtx_stack_c::multVec(&cStack_3c, &field_0xd6c);
                    mJntAnm.lookPos(&field_0xd6c, 0);
                }
            } else {
                u16 status = dMsgObject_c::getStatus();
                if (field_0x10e8 != 0) {
                    mJntAnm.lookNone(0);
                    if (dMsgObject_isMouthCheck() || status == 15 || status == 16) {
                        field_0x10e8 = 0;
                        field_0xe26 = true;
                    }
                } else {
                    mJntAnm.lookCamera(0);
                    if (mMotionSeqMngr.getNo() == MOT_UNK_2 && mMotionSeqMngr.getStepNo() > 0) {
                        mJntAnm.lookNone(0);
                    }
                }
            }
        }

        cam_ctr_pos.set(-110.0f, 100.0f, 0.0f);
        mDoMtx_stack_c::YrotS(home.angle.y);
        mDoMtx_stack_c::multVec(&cam_ctr_pos, &cam_ctr_pos);
        cam_ctr_pos += current.pos;
        mShopCamAction.setMasterCamCtrPos(&cam_ctr_pos);
        break;
    case MODE_EXIT:
        break;
    }

    return 0;
}

static int daNpc_clerkA_Create(void* i_this) {
    return static_cast<daNpc_clerkA_c*>(i_this)->create();
}

static int daNpc_clerkA_Delete(void* i_this) {
    return static_cast<daNpc_clerkA_c*>(i_this)->Delete();
}

static int daNpc_clerkA_Execute(void* i_this) {
    return static_cast<daNpc_clerkA_c*>(i_this)->Execute();
}

static int daNpc_clerkA_Draw(void* i_this) {
    return static_cast<daNpc_clerkA_c*>(i_this)->Draw();
}

static int daNpc_clerkA_IsDelete(void* i_this) {
    return 1;
}

NPC_CLERKA_HIO_CLASS l_HIO;

static actor_method_class daNpc_clerkA_MethodTable = {
    (process_method_func)daNpc_clerkA_Create,  (process_method_func)daNpc_clerkA_Delete,
    (process_method_func)daNpc_clerkA_Execute, (process_method_func)daNpc_clerkA_IsDelete,
    (process_method_func)daNpc_clerkA_Draw,
};

extern actor_process_profile_definition g_profile_NPC_CLERKA = {
    fpcLy_CURRENT_e,            // mLayerID
    7,                          // mListID
    fpcPi_CURRENT_e,            // mListPrio
    PROC_NPC_CLERKA,            // mProcName
    &g_fpcLf_Method.base,       // sub_method
    sizeof(daNpc_clerkA_c),     // mSize
    0,                          // mSizeOther
    0,                          // mParameters
    &g_fopAc_Method.base,       // sub_method
    326,                        // mPriority
    &daNpc_clerkA_MethodTable,  // sub_method
    0x00044107,                 // mStatus
    fopAc_NPC_e,                // mActorType
    fopAc_CULLBOX_CUSTOM_e,     // cullType
};
