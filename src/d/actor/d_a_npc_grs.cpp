/**
 * @file d_a_npc_grs.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_npc_grs.h"
#include "d/d_debug_viewer.h"
#include "d/actor/d_a_tag_push.h"
#include "Z2AudioLib/Z2Instances.h"

enum Event_Cut_Nums {
    NUM_EVT_CUTS_e = 2,
};

enum Animation {
    /* 0x00 */ ANM_NONE,
    /* 0x01 */ ANM_UNK_1,
    /* 0x02 */ ANM_UNK_2,
    /* 0x03 */ ANM_UNK_3,
    /* 0x04 */ ANM_UNK_4,
    /* 0x05 */ ANM_UNK_5,
    /* 0x06 */ ANM_UNK_6,
    /* 0x07 */ ANM_UNK_7,
};

enum LookMode {
    /* -1   */ LOOK_MODE_NONE = -1,
    /* 0x00 */ LOOK_MODE_UNK_0,
    /* 0x01 */ LOOK_MODE_SNAP,
    /* 0x02 */ LOOK_MODE_UNK_2,
    /* 0x03 */ LOOK_MODE_UNK_3,
    /* 0x04 */ LOOK_MODE_UNK_4,
};

static daNpc_GetParam1 l_bmdGetParamList[2] = {
    {16, 0},
    {17, 0},
};

static daNpc_GetParam1 l_bckGetParamList[8] = {
    {-1, 0},
    {8, 0},
    {7, 0},
    {9, 0},
    {13, 0},
    {11, 0},
    {12, 0},
    {10, 0},
};

static daNpc_GetParam1 l_btpGetParamList[3] = {
    {23, 0},
    {24, 0},
    {25, 0},
};

static daNpc_GetParam1 l_btkGetParamList[1] = {
    {20, 0},
};

static daNpc_GetParam1 l_evtGetParamList[2] = {
    {0, 0},
    {1, 0},
};

static int l_loadRes_GRSa[3] = {0, -1, -1};

static int l_loadRes_GRS0[3] = {0, -1, -1};

static int* l_loadRes_list[2] = {
    (int*)&l_loadRes_GRSa,
    (int*)&l_loadRes_GRS0,
};

static char* l_resNames[1] = {
    "grS",
};

static char* l_evtNames[2] = {
    NULL,
    "PUSHOUT",
};

static char* l_myName = "grS";

static NPC_GRS_HIO_CLASS l_HIO;

char* daNpc_grS_c::mEvtCutNameList[2] = {
    "",
    "PUSHOUT",
};

daNpc_grS_c::cutFunc daNpc_grS_c::mEvtCutList[2] = {
    NULL,
    &daNpc_grS_c::cutPushOut,
};

daNpc_grS_c::daNpc_grS_c() {
}

daNpc_grS_c::~daNpc_grS_c() {
    for (int i = 0; l_loadRes_list[mType][i] >= 0; i++) {
        dComIfG_resDelete(&mPhases[i], l_resNames[l_loadRes_list[mType][i]]);
    }

    if (heap != NULL) {
        mAnm_p->stopZelAnime();
    }

#if DEBUG
    if (mpHIO != NULL) {
        mpHIO->removeHIO();
    }
#endif
}

const daNpc_grS_HIOParam daNpc_grS_Param_c::m = {
    120.0f, -3.0f, 1.0f, 600.0f, 255.0f, 120.0f, 35.0f, 60.0f,
    0.0f, 0.0f, 10.0f, -10.0f, 30.0f, -30.0f, 45.0f, -45.0f,
    0.6f, 12.0f,
    3, 6, 5, 6,
    180.0f, 500.0f, 300.0f, -300.0f,
    60, 8, 0, 0, 0,
    FALSE, FALSE,
};

int daNpc_grS_c::create() {
    J3DModelData* modelData;
    int loadResult;
    int loadedCount;
    int i;
    int i_copy;

    fopAcM_ct(this, daNpc_grS_c);

    mType = getTypeFromParam();

    if (home.angle.x != 0xffff) {
        field_0xe0c = home.angle.x;
    } else {
        field_0xe0c = -1;
    }

    if (isDelete()) {
        return cPhs_ERROR_e;
    }

    loadedCount = 0;
    i = 0;
    i_copy = i;
    while (l_loadRes_list[mType][i] >= 0) {
        // Declaring a dummy variable inside the loop scope forces r31 to be used for the stack
        // pointer instead of r1. Ideally we'd declare loadedResult here instead, but that causes a
        // regswap with loadedCount.
        int dummy;
        loadResult = dComIfG_resLoad(&mPhases[i], l_resNames[l_loadRes_list[mType][i]]);
        if (loadResult == cPhs_ERROR_e || loadResult == cPhs_UNK3_e) {
            return cPhs_ERROR_e;
        }
        if (loadResult == cPhs_COMPLEATE_e) {
            loadedCount++;
        }

        i++;
    }

    if (loadedCount == i) {
        if (!fopAcM_entrySolidHeap(this, createHeapCallBack, 0x3e30)) {
            return cPhs_ERROR_e;
        }

        modelData = mAnm_p->getModel()->getModelData();
        fopAcM_SetMtx(this, mAnm_p->getModel()->getBaseTRMtx());
        fopAcM_setCullSizeBox(this, -300.0f, -50.0f, -300.0f, 300.0f, 450.0f, 300.0f);

        mSound.init(&current.pos, &eyePos, 3, 1);

#if DEBUG
        mpHIO = &l_HIO;
        // "Little elder Goron"
        mpHIO->entryHIO("チビ長老ゴロン");
#endif

        mAcchCir.SetWall(mpHIO->m.common.width, mpHIO->m.common.knee_length);

        mAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1, &mAcchCir,
                  fopAcM_GetSpeed_p(this), fopAcM_GetAngle_p(this),
                  fopAcM_GetShapeAngle_p(this));
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

int daNpc_grS_c::CreateHeap() {
    J3DModelData* mdlData_p = NULL;
    J3DModel* model = NULL;

    if (l_bmdGetParamList[0].fileIdx >= 0) {
        mdlData_p = (J3DModelData*)dComIfG_getObjectRes(l_resNames[l_bmdGetParamList[0].arcIdx],
                                                        l_bmdGetParamList[0].fileIdx);
    }
    JUT_ASSERT(420, NULL != mdlData_p);
    u32 differedDlistFlag = 0x11020284;
    mAnm_p = new mDoExt_McaMorfSO(mdlData_p, NULL, NULL, NULL, -1, 1.0f, 0, -1, &mSound, 0x80000,
                                  differedDlistFlag);

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
    mpMatAnm = new daNpcF_MatAnm_c();

    if (mpMatAnm == NULL) {
        return 0;
    }

    mdlData_p = (J3DModelData*)dComIfG_getObjectRes(
        l_resNames[l_bmdGetParamList[1].arcIdx], l_bmdGetParamList[1].fileIdx);
    if (mdlData_p != NULL) {
        mpModel = mDoExt_J3DModel__create(mdlData_p, 0x80000, 0x11000084);
    } else {
        mpModel = NULL;
    }

    if (!setExpressionAnm(1, 0)) {
        return 0;
    }

    setMotionAnm(4, 0.0f);
    return 1;
}

int daNpc_grS_c::Delete() {
    fpc_ProcID unusedId = fopAcM_GetID(this);
    this->~daNpc_grS_c();
    return 1;
}

int daNpc_grS_c::Execute() {
    return execute();
}

int daNpc_grS_c::Draw() {
    J3DModelData* modelData = mAnm_p->getModel()->getModelData();
    modelData->getMaterialNodePointer(2)->setMaterialAnm(mpMatAnm);
    return draw(chkAction(&daNpc_grS_c::test),0, mpHIO->m.common.real_shadow_size,NULL,0);
}

int daNpc_grS_c::ctrlJoint(J3DJoint* i_joint, J3DModel* i_model) {
    J3DJoint* joint = i_joint;
    int mtxIdx = joint->getJntNo();
    int local_24[3] = {1, 3, 4};

    if (mtxIdx == 0) {
        mDoMtx_stack_c::copy(mAnm_p->getModel()->getAnmMtx(1));
        mDoMtx_stack_c::multVecZero(&mLookatPos[0]);
        mDoMtx_stack_c::copy(mAnm_p->getModel()->getAnmMtx(3));
        mDoMtx_stack_c::multVecZero(&mLookatPos[1]);
        mDoMtx_stack_c::copy(mAnm_p->getModel()->getAnmMtx(4));
        mDoMtx_stack_c::multVecZero(&mLookatPos[2]);
    }

    mDoMtx_stack_c::copy(i_model->getAnmMtx(mtxIdx));

    switch (mtxIdx) {
    case 1:
    case 3:
    case 4:
        setLookatMtx(mtxIdx, local_24, mpHIO->m.common.neck_rotation_ratio);
        break;
    }
    if (mtxIdx == 1) {
        mDoMtx_stack_c::YrotM(-field_0x908[0].z);
        mDoMtx_stack_c::ZrotM(field_0x908[0].x);
    } else if (mtxIdx == 4) {
        mDoMtx_stack_c::YrotM(-field_0x908[2].z);
        mDoMtx_stack_c::ZrotM(field_0x908[2].x);
    }

    i_model->setAnmMtx(mtxIdx, mDoMtx_stack_c::get());
    cMtx_copy(mDoMtx_stack_c::get(), J3DSys::mCurrentMtx);

    if ((mtxIdx == 4 || mtxIdx == 8) && (mAnmFlags & 0x100)) {
        J3DAnmTransform* bckTransform = mBckAnm.getBckAnm();
        mBckAnm.changeBckOnly(mAnm_p->getAnm());
        mAnm_p->changeAnm(bckTransform);
    }
    return 1;
}

int daNpc_grS_c::createHeapCallBack(fopAc_ac_c* i_this) {
    daNpc_grS_c* grS = (daNpc_grS_c*)i_this;
    return grS->CreateHeap();
}

int daNpc_grS_c::ctrlJointCallBack(J3DJoint* i_joint, int param_1) {
    if (param_1 == 0) {
        J3DModel* model = j3dSys.getModel();
        daNpc_grS_c* grS = (daNpc_grS_c*)model->getUserArea();
        if (grS != NULL) {
            grS->ctrlJoint(i_joint, model);
        }
    }

    return 1;
}

void daNpc_grS_c::setParam() {
    actionFunc oldAction = mpNextActionFn;
    u32 flags = fopAc_AttnFlag_TALK_e | fopAc_AttnFlag_SPEAK_e;

    selectAction();

    if (!mTwilight && daPy_py_c::checkNowWolf()) {
        flags = 0;
    }

    if (mpNextActionFn != oldAction) {
        // BUG: i is initialized incorrectly; the loop never actually runs.
        for (int i = 2; i < 2; i++) {
            mActorMngrs[i].initialize();
        }
    }

    field_0xdfc = 0;
    field_0xe00 = 0;

    s16 unkInt1;
    s16 unkInt2;
    s16 unkInt3 = 3;
    s16 unkInt4 = 6;
    unkInt1 = 5;
    unkInt2 = 6;
    attention_info.distances[fopAc_attn_LOCK_e] = getDistTableIdx(unkInt1, unkInt2);
    attention_info.distances[fopAc_attn_TALK_e] = attention_info.distances[0];
    attention_info.distances[fopAc_attn_SPEAK_e] = getDistTableIdx(unkInt3, unkInt4);
    attention_info.flags = flags;

    scale.set(mpHIO->m.common.scale, mpHIO->m.common.scale,
                mpHIO->m.common.scale);

    mAcchCir.SetWallR(mpHIO->m.common.width);
    mAcchCir.SetWallH(mpHIO->m.common.knee_length);

    gravity = mpHIO->m.common.gravity;
}

int daNpc_grS_c::main() {
    u16 temp = 0xffff;

    if (!doEvent()) {
        doNormalAction(1);
    }

    if (checkHide()) {
        attention_info.flags = 0;
    }

    if (!mpHIO->m.common.debug_mode_ON) {
        if (!dComIfGp_event_runCheck() || (mOrderNewEvt && dComIfGp_getEvent()->isOrderOK())) {
            if (mOrderEvtNo != 0) {
                eventInfo.setArchiveName(l_resNames[l_evtGetParamList[mOrderEvtNo].arcIdx]);
            }

            orderEvent(mIsSpeaking, l_evtNames[l_evtGetParamList[mOrderEvtNo].fileIdx], temp, 40,
                       0xff, 1);
        }
    }

    if (field_0x9ee) {
        mExpressionMorfOverride = 0.0f;
        mMotionMorfOverride = 0.0f;
        field_0x9ee = false;
    }

    playExpression();
    playMotion();

    return 1;
}

int daNpc_grS_c::ctrlBtk() {
    if (mpMatAnm != NULL) {
        J3DAnmTextureSRTKey* iVar2 = 0;
        if (l_btkGetParamList[0].fileIdx >= 0) {
            iVar2 = getTexSRTKeyAnmP(l_resNames[l_btkGetParamList[0].arcIdx],
                                                          l_btkGetParamList[0].fileIdx);
        }

        if (iVar2 == mBtkAnm.getBtkAnm()) {
            mpMatAnm->setNowOffsetX(cM_ssin(mEyeAngle.y) * 0.02f * -1.0f);
            mpMatAnm->setNowOffsetY(cM_ssin(mEyeAngle.x) * 0.02f);
            mpMatAnm->onEyeMoveFlag();
            return 1;
        }

        mpMatAnm->offEyeMoveFlag();
    }
    return 0;
}

void daNpc_grS_c::setAttnPos() {
    static cXyz eyeOffset(16.0f, 20.0f, 0.0f);

    cXyz* this_01 = NULL;
    int unusedInt1 = 0;
    cXyz local_74;
    cXyz local_80;
    cXyz cStack_8c;
    cXyz local_98;

    f32 attnOffset = mpHIO->m.common.attention_offset;
    f32 newZ = 0.0f;
    f32 heightOff = 0.0f;
    f32 widthOff = 0.0f;

    mDoMtx_stack_c::YrotS(field_0x990);
    cLib_addCalc2(&field_0x984[0], 0.0f, 0.1f, 125.0f);
    cLib_addCalc2(&field_0x984[2], 0.0f, 0.1f, 125.0f);

    for (int i = 0; i < 3; i++) {
        cStack_8c.set(0.0f, 0.0f, field_0x984[i] * cM_ssin(field_0x992));
        mDoMtx_stack_c::multVec(&cStack_8c, &local_98);
        field_0x908[i].x = -local_98.z;
        field_0x908[i].z = -local_98.x;
    }

    cLib_chaseS(&field_0x992, 0, 0x555);

    if (mLookMode == LOOK_MODE_SNAP) {
        for (int i = 0; i < 3; i++) {
            mLookatAngle[i].setall(0);
        }
    }

    setMtx();
    lookat();
    setPrtcl();

    mDoMtx_stack_c::copy(mAnm_p->getModel()->getAnmMtx(4));
    mDoMtx_stack_c::multVecZero(&mHeadPos);
    mDoMtx_stack_c::multVec(&eyeOffset, &eyePos);
    eyeOffset.x = 0.0f;
    mDoMtx_stack_c::multVec(&eyeOffset, &cStack_8c);
    mHeadAngle.x = cLib_targetAngleX(&mHeadPos, &cStack_8c);
    mHeadAngle.y = cLib_targetAngleY(&mHeadPos, &cStack_8c);

    this_01 = mLookat.getAttnPos();
    if (this_01 != NULL) {
        local_80 = *this_01 - eyePos;
        mEyeAngle.y = -mLookatAngle[2].y - mCurAngle.y;
        mEyeAngle.y += cM_atan2s(local_80.x, local_80.z);
        mEyeAngle.x = -cM_atan2s(local_80.y, local_80.absXZ());
    } else {
        mEyeAngle.y = 0;
        mEyeAngle.x = 0;
    }

    newZ = 0.0f;
    heightOff = 0.0f;
    widthOff = 0.0f;

    local_80.set(0.0f, 0.0f, newZ);
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::ZXYrotM(mCurAngle.x, mCurAngle.y, mCurAngle.z);
    mDoMtx_stack_c::multVec(&local_80, &local_74);
    attention_info.position.set(local_74.x, local_74.y + attnOffset, local_74.z);

    if (!mHide && mTwilight == 0) {
        if (!mIsDamaged) {
            mCyl.SetTgType(0xd8fbfdff);
            mCyl.SetTgSPrm(31);
            mCyl.OnTgNoHitMark();
        } else {
            mCyl.SetTgType(0);
            mCyl.SetTgSPrm(0);
        }
        mCyl.SetC(local_74);
        mCyl.SetH(mpHIO->m.common.height + heightOff);
        mCyl.SetR(mpHIO->m.common.width + widthOff);
        dComIfG_Ccsp()->Set(&mCyl);
    }

    mCyl.ClrTgHit();
}

bool daNpc_grS_c::setExpressionAnm(int param_0, bool param_1) {
    J3DAnmTransform* bck = NULL;
    s32 iVar2 = 0;
    mAnmFlags = mAnmFlags & ~0x1fc0;
    if (l_bckGetParamList[param_0].fileIdx >= 0) {
        bck = (J3DAnmTransform*)getTrnsfrmKeyAnmP(l_resNames[l_bckGetParamList[param_0].arcIdx],
                                                     l_bckGetParamList[param_0].fileIdx);
    }

    bool exprResult = false;
    switch (param_0) {
    case 0:
        exprResult = setExpressionBtp(0);
        break;
    case 1:
        exprResult = setExpressionBtp(0);
        break;
    case 2:
        exprResult = setExpressionBtp(1);
        break;
    case 3:
        exprResult = setExpressionBtp(2);
        iVar2 = 2;
        break;
    default:
        bck = NULL;
        break;
    }

    if (!exprResult) {
        return false;
    }

    if (bck == NULL) {
        return true;
    }

    if (setBckAnm(bck, 1.0f, iVar2, 0, -1, param_1)) {
        mAnmFlags |= 0x140;
        mExpressionLoops = 0;
        return true;
    }

    // "Failed to register facial expression Bck animation!"
    OS_REPORT("%s: 表情Bckアニメーションの登録に失敗しました！\n", __FILE__);
    return false;
}

bool daNpc_grS_c::setExpressionBtp(int param_0) {
    J3DAnmTexPattern* anmTexPattern = NULL;
    s32 btpAnmAttr = 0;

    mAnmFlags = mAnmFlags & ~0xa80;

    if (l_btpGetParamList[param_0].fileIdx >= 0) {
        anmTexPattern = getTexPtrnAnmP(l_resNames[l_btpGetParamList[param_0].arcIdx],
            l_btpGetParamList[param_0].fileIdx);
    }

    switch (param_0) {
    case 1:
        break;
    case 0:
    case 2:
        btpAnmAttr = 2;
        break;
    default:
        anmTexPattern = NULL;
        break;
    }

    if (anmTexPattern == NULL) {
        return true;
    }

    if (setBtpAnm(anmTexPattern, mAnm_p->getModel()->getModelData(), 1.0f, btpAnmAttr)) {
        mAnmFlags |= 0x280;
        if (param_0 == 0) {
            mAnmFlags |= 0x800;
        }

        return true;
    }

    // "Failed to register facial BTP animation!"
    OS_REPORT("%s: 表情Btpアニメーションの登録に失敗しました！\n", __FILE__);
    return false;
}

void daNpc_grS_c::setExpression(int param_0, f32 param_1) {
    if (param_0 >= 0 && param_0 < 4) {
        mExpression = param_0;
        mExpressionMorfOverride = param_1;
        mExpressionPrevPhase = -1;
        mExpressionPhase = 0;
    }
}

void daNpc_grS_c::setMotionAnm(int param_0, f32 param_1) {
    J3DAnmTransformKey* anmTransformKey = NULL;
    J3DAnmTextureSRTKey* anmTexSRTKey = NULL;
    s32 bckIdx = 0;
    s32 anmMode = 2;
    s32 btkAnmMode = 2;
    mAnmFlags = mAnmFlags & ~0x3f;
    if (l_bckGetParamList[param_0].fileIdx >= 0) {
        anmTransformKey = getTrnsfrmKeyAnmP(l_resNames[l_bckGetParamList[param_0].arcIdx],
                                   l_bckGetParamList[param_0].fileIdx);
    }

    switch (param_0) {
    case 5:
    case 6:
    case 7:
        anmMode = 0;
        break;
    case 4:
        break;
    default:
        anmTransformKey = NULL;
        anmTexSRTKey = NULL;
        break;
    }

    if (l_btkGetParamList[bckIdx].fileIdx >= 0) {
        anmTexSRTKey = getTexSRTKeyAnmP(l_resNames[l_btkGetParamList[bckIdx].arcIdx],
                                        l_btkGetParamList[bckIdx].fileIdx);
    }

    if (anmTexSRTKey != NULL) {
        if (setBtkAnm(anmTexSRTKey, mAnm_p->getModel()->getModelData(), 1.0f, btkAnmMode)) {
            mAnmFlags |= 0x12;
        }
    }

    if (anmTransformKey != NULL && setMcaMorfAnm(anmTransformKey, 1.0f, param_1, anmMode, 0, -1)) {
        mAnmFlags |= 0x9;
        mMotionLoops = 0;
    }
}

void daNpc_grS_c::setMotion(int param_0, f32 param_1, int param_2) {
    s16 param_0_copy = param_0;
    if ((param_2 != 0 || mMotion != param_0_copy) && param_0 >= 0 && param_0 < 4) {
        mMotion = param_0_copy;
        mMotionMorfOverride = param_1;
        mMotionPrevPhase = -1;
        mMotionPhase = 0;
    }
}

int daNpc_grS_c::drawDbgInfo() {
#if DEBUG
    if (mpHIO->m.common.debug_info_ON) {
        cXyz acStack_48;

        f32 distMax1 = dComIfGp_getAttention()
                           ->getDistTable(attention_info.distances[fopAc_attn_SPEAK_e])
                           .mDistMax;
        f32 distMax2 = dComIfGp_getAttention()
                           ->getDistTable(attention_info.distances[fopAc_attn_TALK_e])
                           .mDistMax;
        s16 targetAngleX = cLib_targetAngleX(&current.pos, &eyePos);
        s16 targetAngleY = cLib_targetAngleY(&current.pos, &eyePos);
        dDbVw_drawCircleOpa(attention_info.position, distMax1,
                            (GXColor){0x0, 0xc8, 0x0, 0xff}, 1, 12);
        dDbVw_drawCircleOpa(attention_info.position, distMax2,
                            (GXColor){0xc8, 0x0, 0x0, 0xff}, 1, 12);

        if (mpHIO->m.common.fov != 180.0f) {
            cXyz cStack_54(0.0f, 0.0f, mpHIO->m.common.search_distance);
            mDoMtx_stack_c::transS(attention_info.position);
            mDoMtx_stack_c::YrotM(mHeadAngle.y);
            mDoMtx_stack_c::YrotM(cM_deg2s(-mpHIO->m.common.fov));
            mDoMtx_stack_c::multVec(&cStack_54, &acStack_48);
            dDbVw_drawLineOpa(attention_info.position, acStack_48, (GXColor){0x0, 0x0, 0xc8, 0xff},
                              1, 12);

            mDoMtx_stack_c::transS(attention_info.position);
            mDoMtx_stack_c::YrotM(mHeadAngle.y);
            mDoMtx_stack_c::YrotM(cM_deg2s(mpHIO->m.common.fov));
            mDoMtx_stack_c::multVec(&cStack_54, &acStack_48);
            dDbVw_drawLineOpa(attention_info.position, acStack_48, (GXColor){0x0, 0x0, 0xc8, 0xff},
                              1, 12);
        }

        dDbVw_drawCircleOpa(attention_info.position, mpHIO->m.common.search_distance,
                            (GXColor){0x0, 0x0, 0xc8, 0xff}, 1, 12);
        dDbVw_drawSphereXlu(eyePos, 18.0f, (GXColor){0x80, 0x80, 0x80, 0xa0}, 1);
        dDbVw_drawSphereXlu(mLookPos, 18.0f, (GXColor){0x80, 0x80, 0x80, 0xa0}, 1);

        int idx = getDistTableIdx(field_0xdfc, field_0xe00);
        distMax1 = dComIfGp_getAttention()->getDistTable(idx).mDistMax;
        dDbVw_drawCircleOpa(attention_info.position, distMax1,
                            (GXColor){0xc8, 0xc8, 0xc8, 0xff}, 1, 12);
    }
#endif

    return 0;
}

void daNpc_grS_c::drawOtherMdls() {
    Mtx mtx;

    J3DModel* model = mAnm_p->getModel();
    if (mpModel != NULL) {
        g_env_light.setLightTevColorType_MAJI(mpModel, &tevStr);
        mDoMtx_stack_c::copy(model->getAnmMtx(12));
        cMtx_copy(mDoMtx_stack_c::get(), mtx);
        mpModel->setBaseTRMtx(mtx);

        mDoExt_modelUpdateDL(mpModel);
        dComIfGd_addRealShadow(mShadowKey, mpModel);
    }
}

u8 daNpc_grS_c::getTypeFromParam() {
    int temp = (u8)fopAcM_GetParam(this);
    switch ((u8)temp) {
    case 0:
        return 0;
    default:
        return 0;
    }
}

int daNpc_grS_c::isDelete() {
    if (strcmp(dComIfGp_getStartStageName(), "D_MN04") == 0 && daNpcF_chkEvtBit(0x40)) {
        return 1;
    }

    if (mType == 1 || mType == 0) {
        return 0;
    }

    return 1;
}

void daNpc_grS_c::reset() {
    initialize();
    mpMatAnm->initialize();
    mLookat.initialize();

    for (int i = 0; i < 2; i++) {
        mActorMngrs[i].initialize();
    }

    mpNextActionFn = NULL;

    mpActionFn = NULL;
    field_0xdfc = 0;
    field_0xe00 = 0;
    field_0xe04 = 0;
    field_0xe08 = 0;
    mLookMode = LOOK_MODE_NONE;
    mActionState = 0;
    mPresentItemId = fpcM_ERROR_PROCESS_ID_e;
    for (int i = 0; i < ARRAY_SIZE(mParticleIds); i++) {
        mParticleIds[i] = fpcM_ERROR_PROCESS_ID_e;
    }

    current.pos = home.pos;
    old.pos = current.pos;

    current.angle.set(0, home.angle.y, 0);
    old.angle = current.angle;
    shape_angle = current.angle;
    mCurAngle = current.angle;
    mOldAngle = mCurAngle;

    speedF = 0.0f;
    speed.setall(0.0f);

    mIsSpeaking = false;
    field_0x9ee = true;
}

void daNpc_grS_c::playExpression() {
    daNpcF_anmPlayData dat0a = {ANM_UNK_1, mpHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat0[1] = {&dat0a};
    daNpcF_anmPlayData dat1a = {ANM_UNK_2, mpHIO->m.common.morf_frame, 1};
    daNpcF_anmPlayData dat1b = {ANM_UNK_3, mpHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat1[2] = {&dat1a, &dat1b};
    daNpcF_anmPlayData dat2a = {ANM_UNK_3, mpHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat2[1] = {&dat2a};
    daNpcF_anmPlayData dat3a = {ANM_NONE, mpHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat3[1] = {&dat3a};

    daNpcF_anmPlayData** ppDat[4] = {
        pDat0, pDat1, pDat2, pDat3
    };
    if (mExpression >= 0 && mExpression < ARRAY_SIZE(ppDat)) {
        playExpressionAnm(ppDat);
    }
}

void daNpc_grS_c::playMotion() {
    daNpcF_anmPlayData dat0a = {ANM_UNK_4, mpHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat0[1] = {&dat0a};
    daNpcF_anmPlayData dat1a = {ANM_UNK_5, mpHIO->m.common.morf_frame, 1};
    daNpcF_anmPlayData dat1b = {ANM_UNK_4, mpHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat1[2] = {&dat1a, &dat1b};
    daNpcF_anmPlayData dat2a = {ANM_UNK_6, mpHIO->m.common.morf_frame, 1};
    daNpcF_anmPlayData dat2b = {ANM_UNK_4, 0.0f, 0};
    daNpcF_anmPlayData* pDat2[2] = {&dat2a, &dat2b};
    daNpcF_anmPlayData dat3a = {ANM_UNK_7, mpHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat3[1] = {&dat3a};

    daNpcF_anmPlayData** ppDat[4] = {
        pDat0,
        pDat1,
        pDat2,
        pDat3,
    };

    if (mMotion >= 0 && mMotion < ARRAY_SIZE(ppDat)) {
        playMotionAnm(ppDat);
    }
}

int daNpc_grS_c::chkAction(actionFunc i_func) {
    return mpActionFn == i_func;
}

int daNpc_grS_c::setAction(actionFunc i_func) {
    mActionState = 3;
    if (mpActionFn != NULL) {
        (this->*mpActionFn)(NULL);
    }
    mActionState = 0;
    mpActionFn = i_func;
    if (mpActionFn != NULL) {
        (this->*mpActionFn)(NULL);
    }
    return 1;
}

int daNpc_grS_c::selectAction() {
    mpNextActionFn = NULL;
    if (mpHIO->m.common.debug_mode_ON) {
        mpNextActionFn = &daNpc_grS_c::test;
    }
    else {
        (int)mType;
        mpNextActionFn = &daNpc_grS_c::wait;
    }

    return 1;
}

void daNpc_grS_c::doNormalAction(int param_0) {
    if (param_0 != 0 && hitChk2(&mCyl, 1, 0)) {
        int damageTime = mCutType == 8 ? 20 : mpHIO->m.common.damage_time;

        setDamage(damageTime, 3, 0);
        setLookMode(1);
    } else {
        if (mIsDamaged && mDamageTimer == 0) {
            mActionState = 0;
            mIsDamaged = false;
        }
    }

    if (field_0x9ef == 8) {
        mOrderEvtNo = 1;
    } else {
        mOrderEvtNo = 0;
    }

    if (mpNextActionFn != NULL) {
        if (mpActionFn == mpNextActionFn) {
            (this->*mpActionFn)(NULL);
        } else {
            setAction(mpNextActionFn);
        }
    }
}

int daNpc_grS_c::doEvent() {
    dEvent_manager_c* eventManager = NULL;
    int result = 0;
    int evtCutNo;
    int staffId;

    if (dComIfGp_event_runCheck()) {
        eventManager = &dComIfGp_getEventManager();
        if ((eventInfo.checkCommandTalk() || eventInfo.checkCommandDemoAccrpt()) &&
            !mIsSpeaking)
        {
            mOrderNewEvt = false;
        }
        if (eventInfo.checkCommandTalk()) {
            if (chkAction(&daNpc_grS_c::talk)) {
                (this->*mpActionFn)(NULL);
            } else {
                if (dComIfGp_event_chkTalkXY() == FALSE || dComIfGp_evmng_ChkPresentEnd()) {
                    setAction(&daNpc_grS_c::talk);
                }
            }
            result = 1;
        } else {
            if (mPresentItemId != fpcM_ERROR_PROCESS_ID_e) {
                dComIfGp_event_setItemPartnerId(mPresentItemId);
                mPresentItemId = fpcM_ERROR_PROCESS_ID_e;
            }

            staffId = eventManager->getMyStaffId(l_myName, NULL, 0);
            if (staffId != -1) {
                mStaffID = staffId;

                evtCutNo = eventManager->getMyActIdx(staffId, mEvtCutNameList, 2, 0, 0);

                JUT_ASSERT(0x63f, (0 <= evtCutNo) && (evtCutNo < NUM_EVT_CUTS_e));
                JUT_ASSERT(0x640, NULL != mEvtCutList[evtCutNo]);
                if ((this->*mEvtCutList[evtCutNo])(staffId)) {
                    eventManager->cutEnd(staffId);
                }
                result = 1;
            }

            if (eventInfo.checkCommandDemoAccrpt() && mEventIdx != -1 &&
                eventManager->endCheck(mEventIdx))
            {
                (int)mOrderEvtNo;
                dComIfGp_event_reset();
                mOrderEvtNo = 0;
                mEventIdx = -1;
            }
        }

        int expression;
        int motion;
        int msgTimerOld = mMsgTimer;
        if (ctrlMsgAnm(expression, motion, this, 0)) {
            if (!field_0x9eb) {
                setExpression(expression, -1.0f);
                setMotion(motion, -1.0f, 0);
            }
        } else {
            if (msgTimerOld != 0 && !field_0x9eb) {
                setExpressionTalkAfter();
            }
        }
    } else {
        mMsgTimer = 0;
        if (mStaffID != -1) {
            mpActionFn = NULL;
            mStaffID = -1;
        }
    }
    return result;
}

void daNpc_grS_c::setLookMode(int i_mode) {
    if (i_mode >= 0 && i_mode < 5 && i_mode != mLookMode) {
        mLookMode = i_mode;
    }
}

void daNpc_grS_c::lookat() {
    daPy_py_c* player = NULL;
    J3DModel* model = mAnm_p->getModel();
    s32 snap = 0;

    f32 bodyAngleXMin = mpHIO->m.common.body_angleX_min;
    f32 bodyAngleXMax = mpHIO->m.common.body_angleX_max;
    f32 bodyAngleYMin = mpHIO->m.common.body_angleY_min;
    f32 bodyAngleYMax = mpHIO->m.common.body_angleY_max;
    f32 headAngleXMin = mpHIO->m.common.head_angleX_min;
    f32 headAngleXMax = mpHIO->m.common.head_angleX_max;
    f32 headAngleYMin = mpHIO->m.common.head_angleY_min;
    f32 headAngleYMax = mpHIO->m.common.head_angleY_max;

    s16 yawDiff = mCurAngle.y - mOldAngle.y;

    cXyz lookatPositions[3] = {mLookatPos[0], mLookatPos[1], mLookatPos[2]};
    csXyz* lookatAngles[3] = {&mLookatAngle[0], &mLookatAngle[1], &mLookatAngle[2]};
    cXyz unusedXyz1;

    switch (mLookMode) {
    case LOOK_MODE_UNK_0:
        break;
    case LOOK_MODE_SNAP:
        snap = 1;
        break;
    case LOOK_MODE_UNK_2:
    case LOOK_MODE_UNK_3:
        player = daPy_getPlayerActorClass();
        if (mLookMode == 3) {
            headAngleYMin = -80.0f;
            headAngleYMax = 80.0f;
        }
        break;
    case LOOK_MODE_UNK_4:
        player = (daPy_py_c*)mActorMngrs[1].getActorP();
        break;
    }

    if (player != NULL) {
        mLookPos = player->attention_info.position;
        if (mLookMode != LOOK_MODE_UNK_2 && mLookMode != LOOK_MODE_UNK_3 &&
            mLookMode != LOOK_MODE_UNK_4)
        {
            mLookPos.y = mLookPos.y - 40.0f;
        }
        mLookat.setAttnPos(&mLookPos);
    } else {
        mLookat.setAttnPos(0);
    }

    mLookat.setParam(bodyAngleXMin, bodyAngleXMax, bodyAngleYMin, bodyAngleYMax, 0.0f, 0.0f, 0.0f,
                     0.0f, headAngleXMin, headAngleXMax, headAngleYMin, headAngleYMax, mCurAngle.y,
                     lookatPositions);
    mLookat.calc(this, model->getBaseTRMtx(), lookatAngles, snap, yawDiff, 0);
}

void daNpc_grS_c::setExpressionTalkAfter() {
    // possible bug? first case always falls through to default
    switch (mExpression) {
    case 1:
        setExpression(2, -1.0f);
    default:
        setExpression(3, -1.0f);
    }
}

int daNpc_grS_c::cutPushOut(int param_0) {
    daTag_Push_c *actor_p = NULL;
    int result = 0;
    int *prm_p = NULL;
    int prm = -1;

    prm_p = dComIfGp_evmng_getMyIntegerP(param_0, "prm");
    if (prm_p != NULL) {
        prm = *prm_p;
    }

    if (dComIfGp_getEventManager().getIsAddvance(param_0)) {
        switch (prm) {
        case 0:
            actor_p = (daTag_Push_c *)field_0x824.getActorP();
            JUT_ASSERT(0x738, NULL != actor_p);
            actor_p->pushBackPlayer(0);
            break;
        case 1:
            setExpression(3, 0.0f);
            setMotion(0, 0.0f, 1);
            setLookMode(1);
            initTalk(mFlowNodeNo,NULL);
            setAngle(fopAcM_searchPlayerAngleY(this));
            break;
        }
    }

    switch (prm) {
    case 0:
        result = 1;
        break;
    case 1:
        if (talkProc(NULL,1,NULL)) {
            result = 1;
        }
        break;
    }

    return result;
}

int daNpc_grS_c::wait(void* param_0) {
    switch (mActionState) {
    case 0:
        setExpression(3, -1.0f);
        setMotion(0, -1.0f, 0);
        setLookMode(0);
        mTurnMode = 0;
        mActionState = 2;
    case 2:
        if (!mIsDamaged) {
            BOOL isActorFound = NULL != mActorMngrs[0].getActorP();
            BOOL isPlayerFound = chkFindPlayer2(isActorFound, shape_angle.y);
            if (isPlayerFound) {
                if (!isActorFound) {
                    mActorMngrs[0].entry(daPy_getPlayerActorClass());
                    mTurnMode = 0;
                }
            } else {
                if (isActorFound) {
                    mActorMngrs[0].remove();
                    mTurnMode = 0;
                }
            }

            if (mActorMngrs[0].getActorP() != NULL) {
                setLookMode(2);
            } else {
                setLookMode(0);

                if (home.angle.y != mCurAngle.y) {
                    if (step(home.angle.y, 3, 3, 15)) {
                        mActionState = 0;
                    }
                }
            }

            if (home.angle.y == mCurAngle.y) {
                fopAc_ac_c* pfVar5 = getAttnActorP(
                    NULL != mActorMngrs[0].getActorP(), srchAttnActor1,
                    mpHIO->m.common.search_distance, mpHIO->m.common.search_height,
                    mpHIO->m.common.search_depth, mpHIO->m.common.fov, shape_angle.y, 0x78, 1);
                if (pfVar5 != NULL) {
                    mActorMngrs[1].entry(pfVar5);
                    setLookMode(4);
                }
            } else {
                mAttnChangeTimer = 0;
            }
        }

        break;
    case 3:
    default:
        break;
    }

    return 1;
}

int daNpc_grS_c::talk(void* param_0) {
    int unusedInt1 = 0;
    BOOL result = 0;
    int unkInt1 = 0;
    BOOL unkFlag1 = FALSE;
    int unkInt2 = 0;

    switch (mActionState) {
    case 0:
        if (mIsDamaged) {
            break;
        }

        if (strcmp(dComIfGp_getStartStageName(), "D_MN04") == 0 && !mIsSpeaking) {
            daNpcF_offTmpBit(11);
        }

        unkInt1 = field_0xe0c;
        mIsSpeaking = false;
        initTalk(unkInt1, NULL);
        mTurnMode = 0;
        mActionState = 2;
    case 2:
        if (field_0x9ea) {
            unkFlag1 = TRUE;
        } else {
            setLookMode(3);
            mActorMngrs[0].entry(daPy_getPlayerActorClass());
            if (mCurAngle.y == fopAcM_searchPlayerAngleY(this)) {
                unkFlag1 = TRUE;
            } else {
                if (step(fopAcM_searchPlayerAngleY(this), 3, 3, 15)) {
                    setExpression(3, -1.0f);
                    setMotion(0, -1.0f, 0);
                    mTurnMode = 0;
                }
            }
        }

        if (unkFlag1 && talkProc(NULL, 1, NULL)) {
            if (mType == 0) {
                if (mFlow.getEventId(&unkInt2) == 1) {
                    mPresentItemId =
                        fopAcM_createItemForPresentDemo(&current.pos, unkInt2, 0, -1, -1, 0, 0);

                    if (mPresentItemId != fpcM_ERROR_PROCESS_ID_e) {
                        s16 eventIdx =
                            dComIfGp_getEventManager().getEventIdx(this, "DEFAULT_GETITEM", 0xff);
                        dComIfGp_getEvent()->reset(this);
                        fopAcM_orderChangeEventId(this, eventIdx, 1, 0xffff);
                        mIsSpeaking = true;
                        field_0x9ec = true;
                        result = TRUE;
                        mOrderNewEvt = true;
                    }
                } else {
                    result = TRUE;
                }
            } else {
                result = TRUE;
            }
        }

        if (result) {
            mActionState = 3;
            if (!field_0x9ec) {
                dComIfGp_event_reset();
            }
            field_0x9ec = false;
        }

        break;
    case 3:
    default:
        break;
    }

    return result;
}

int daNpc_grS_c::test(void* param_0) {
    switch (mActionState) {
    case 0:
        speedF = 0.0f;
        speed.setall(0.0f);
        mActionState = 2;
    case 2:
        if (mpHIO->m.common.face_expression != mExpression) {
            setExpression(mpHIO->m.common.face_expression, mpHIO->m.common.morf_frame);
        }
        setMotion(mpHIO->m.common.motion,mpHIO->m.common.morf_frame,0);
        setLookMode(mpHIO->m.common.look_mode);
        mOrderEvtNo = 0;
        attention_info.flags = 0;

        break;
    case 3:
    default:
        break;
    }

    return 1;
}

void daNpc_grS_c::setPrtcl() {
    static const u16 id[6] = {
        dPa_RM(ID_ZI_S_GRS_SMOKE_A),
        dPa_RM(ID_ZI_S_GRS_SMOKE_B),
        dPa_RM(ID_ZI_S_GRS_SMOKE_C),
        dPa_RM(ID_ZI_S_GRS_SMOKE_D),
        dPa_RM(ID_ZI_S_GRS_SMOKE_E),
        dPa_RM(ID_ZI_S_GRS_SMOKE_F),
    };
    static const int jointNo[6] = {4, 2, 2, 2, 2, 2};

    JPABaseEmitter* emitter = NULL;

    cXyz position;
    cXyz scale(0.7f,0.7f,0.7f);

    for (int i = 0; i < 6; i++) {
        mDoMtx_stack_c::copy(mAnm_p->getModel()->getAnmMtx(jointNo[i]));
        mDoMtx_stack_c::multVecZero(&position);
        mParticleIds[i] = dComIfGp_particle_set(mParticleIds[i], id[i], &position, NULL, &scale);
        emitter = dComIfGp_particle_getEmitter(mParticleIds[i]);
        if (emitter != NULL) {
            emitter->setGlobalRTMatrix(mDoMtx_stack_c::get());
        }
    }
}

static int daNpc_grS_Create(void* i_this) {
    return ((daNpc_grS_c*)i_this)->create();
}

static int daNpc_grS_Delete(void* i_this) {
    return ((daNpc_grS_c*)i_this)->Delete();
}

static int daNpc_grS_Execute(void* i_this) {
    return ((daNpc_grS_c*)i_this)->Execute();
}

static int daNpc_grS_Draw(void* i_this) {
    return ((daNpc_grS_c*)i_this)->Draw();
}

static int daNpc_grS_IsDelete(void* i_this) {
    return TRUE;
}

static actor_method_class daNpc_grS_MethodTable = {
    (process_method_func)daNpc_grS_Create,
    (process_method_func)daNpc_grS_Delete,
    (process_method_func)daNpc_grS_Execute,
    (process_method_func)daNpc_grS_IsDelete,
    (process_method_func)daNpc_grS_Draw,
};

actor_process_profile_definition g_profile_NPC_GRS = {
    fpcLy_CURRENT_e,        // mLayerID
    7,                      // mListID
    fpcPi_CURRENT_e,        // mListPrio
    PROC_NPC_GRS,           // mProcName
    &g_fpcLf_Method.base,  // sub_method
    sizeof(daNpc_grS_c),    // mSize
    0,                      // mSizeOther
    0,                      // mParameters
    &g_fopAc_Method.base,   // sub_method
    313,                    // mPriority
    &daNpc_grS_MethodTable, // sub_method
    0x00044100,             // mStatus
    fopAc_NPC_e,            // mActorType
    fopAc_CULLBOX_CUSTOM_e, // cullType
};

AUDIO_INSTANCES
