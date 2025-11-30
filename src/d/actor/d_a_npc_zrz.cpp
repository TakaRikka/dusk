/**
 * @file d_a_npc_zrz.cpp
 * NPC - Rutela
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_npc_zrz.h"
#include "d/actor/d_a_npc.h"
#include "SSystem/SComponent/c_math.h"
#include "JSystem/J3DGraphBase/J3DMaterial.h"
#include "d/actor/d_a_obj_grave_stone.h"
#include "d/actor/d_a_obj_zra_rock.h"
#include "Z2AudioLib/Z2Instances.h"

static daNpc_zrZ_Param_c l_HIO;

daNpc_zrZ_Param_c::param const daNpc_zrZ_Param_c::m = {
    700.0f,   // mAttnOffsetY
    0.0f,     // mGravity
    1.0f,     // mScale
    1400.0f,  // mShadowDepth
    255.0f,   // mCcWeight
    700.0f,   // mCylH
    0.0f,     // mWallH
    30.0f,    // mWallR
    0.0f,     // mBodyUpAngle
    0.0f,     // mBodyDownAngle
    10.0f,    // mBodyLeftAngle
    -10.0f,   // mBodyRightAngle
    30.0f,    // mHeadUpAngle
    -10.0f,   // mHeadDownAngle
    45.0f,    // mHeadLeftAngle
    -45.0f,   // mHeadRightAngle
    0.6f,     // mNeckAngleScl
    12.0f,    // mMorfFrames
    3,        // mSpeakDistIdx
    6,        // mSpeakAngleIdx
    5,        // mTalkDistIdx
    6,        // mTalkAngleIdx
    80.0f,    // mAttnFovY
    500.0f,   // mAttnRadius
    300.0f,   // mAttnUpperY
    -300.0f,  // mAttnLowerY
    60,
    8,        // mDamageTimer
    0,        // mTestExpression
    0,        // mTestMotion
    0,        // mTestLookMode
    false,    // mTest
    1200.0f,
    1000.0f,  // mFollowDst
    3000.0f,  // mRestoreDst
    25.0f,    // mMaxSpeed
    400.0f,   // mClothesGetDst
    200.0f,
};

static daNpc_GetParam1 l_bmdGetParamList[1] = {
    {9, 0},
};

static daNpc_GetParam1 l_bmdGTGetParamList[1] = {
    {13, 1},  // gt
};

static daNpc_GetParam1 l_bckGetParamList[5] = {
    {-1, 0},
    {-1, 0},
    {6, 0},
    {0, 0},
    {0, 0},
};

static daNpc_GetParam1 l_bckGTGetParamList[5] = {
    {-1, 1},
    {8, 1},   // f_talk_a
    {10, 1},  // wait_gt_a
    {9, 1},   // lead
    {7, 1},   // comeon
};

static daNpc_GetParam1 l_btpGetParamList[1] = {
    {15, 0},
};

static daNpc_GetParam1 l_btpGTGetParamList[1] = {
    {19, 1},  // gt
};

static daNpc_GetParam1 l_btkGetParamList[1] = {
    {12, 0},
};

static daNpc_GetParam1 l_btkGTGetParamList[1] = {
    {16, 1},  // gt
};

static daNpc_GetParam1 l_evtGetParamList[8] = {
    {0, 0},
    {1, 1},
    {2, 1},
    {3, 1},
    {4, 1},
    {5, 1},
    {6, 1},
    {7, 1},
};

static int l_loadRes_ZRZa[3] = {0, -1, -1};

static int l_loadRes_ZRZ_GT[3] = {1, -1, -1};

static int l_loadRes_ZRZ0[3] = {0, -1, -1};

static int* l_loadRes_list[3] = {
    l_loadRes_ZRZa,
    l_loadRes_ZRZ_GT,
    l_loadRes_ZRZ0,
};

static char* l_resNames[2] = {
    "zrZ",
    "zrZ_GT",
};

static char* l_evtNames[8] = {
    NULL,
    "HELP_PRINCE",
    "COME_HERE",
    "RESTORE_LINK",
    "CLOTHES_GET",
    "GET_AFTER",
    "SEAL_RELEASE",
    "SR_SKIP",
};

static char* l_myName = "zrZ";

char* daNpc_zrZ_c::mEvtCutNameList[8] = {
    "",
    "HELP_PRINCE",
    "COME_HERE",
    "RESTORE_LINK",
    "CLOTHES_GET",
    "GET_AFTER",
    "SEAL_RELEASE",
    "SR_SKIP",
};

daNpc_zrZ_c::EventFn daNpc_zrZ_c::mEvtCutList[8] = {
    NULL,
    &daNpc_zrZ_c::ECut_helpPrince,
    &daNpc_zrZ_c::ECut_comeHere,
    &daNpc_zrZ_c::ECut_restoreLink,
    &daNpc_zrZ_c::ECut_clothesGet,
    &daNpc_zrZ_c::ECut_getAfter,
    &daNpc_zrZ_c::ECut_sealRelease,
    &daNpc_zrZ_c::ECut_srSkip,
};

daNpc_zrZ_c::daNpc_zrZ_c() {
    /* empty function */
}

daNpc_zrZ_c::~daNpc_zrZ_c() {
    for (int i = 0; l_loadRes_list[mType][i] >= 0; i++) {
        dComIfG_resDelete(&mPhase[i], l_resNames[l_loadRes_list[mType][i]]);
    }

    if (heap != NULL) {
        mAnm_p->stopZelAnime();
    }
}

cPhs__Step daNpc_zrZ_c::create() {
    fopAcM_ct(this, daNpc_zrZ_c);

    mType = getTypeFromParam();
    mDemoMode = getDemoMode();
    mSwitch1 = home.angle.z & 0xff;
    mSwitch2 = (home.angle.z >> 8) & 0xff;
    mSwitch3 = fopAcM_GetParam(this) >> 0x18;
    if (home.angle.x != 0xffff) {
        mFlowID = home.angle.x;
    } else {
        mFlowID = -1;
    }

    if (isDelete()) {
        return cPhs_ERROR_e;
    }

    int res_count = 0;
    int i;
    for (i = 0; l_loadRes_list[mType][i] >= 0; i++) {
        cPhs__Step step = (cPhs__Step)dComIfG_resLoad(&mPhase[i],
                                                      l_resNames[l_loadRes_list[mType][i]]);
        if (step == cPhs_ERROR_e || step == cPhs_UNK3_e) {
            return cPhs_ERROR_e;
        }
        if (step == cPhs_COMPLEATE_e) {
            res_count++;
        }
    }

    if (res_count == i) {
        if (!fopAcM_entrySolidHeap(this, createHeapCallBack, 0x5230)) {
            return cPhs_ERROR_e;
        }

        fopAcM_SetMtx(this, mAnm_p->getModel()->getBaseTRMtx());
        fopAcM_setCullSizeBox(this, -300.0f, -50.0f, -300.0f, 300.0f, 800.0f, 300.0f);
        mCreatureSound.init(&current.pos, &eyePos, 3, 1);
        mAcchCir.SetWall(daNpc_zrZ_Param_c::m.mWallR, daNpc_zrZ_Param_c::m.mWallH);
        mAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1, &mAcchCir,
                  fopAcM_GetSpeed_p(this), fopAcM_GetAngle_p(this), fopAcM_GetShapeAngle_p(this));
        mCcStts.Init(daNpc_zrZ_Param_c::m.mCcWeight, 0, this);
        mCcCyl.Set(mCcDCyl);
        mCcCyl.SetStts(&mCcStts);
        mCcCyl.SetTgHitCallback(tgHitCallBack);
        mAcch.CrrPos(dComIfG_Bgsp());
        mGndChk = mAcch.m_gnd;
        mGroundH = mAcch.GetGroundH();

        setEnvTevColor();
        setRoomNo();
        reset();
        Execute();

        return cPhs_COMPLEATE_e;
    } else {
        return cPhs_INIT_e;
    }
}

int daNpc_zrZ_c::CreateHeap() {
    J3DModelData* model_data = NULL;
    if (mType == 1) {
        if (l_bmdGTGetParamList[0].fileIdx >= 0) {
            model_data = static_cast<J3DModelData*>(
                dComIfG_getObjectRes(l_resNames[l_bmdGTGetParamList[0].arcIdx],
                                     l_bmdGTGetParamList[0].fileIdx));
        }
    } else {
        if (l_bmdGetParamList[0].fileIdx >= 0) {
            model_data = static_cast<J3DModelData*>(
                dComIfG_getObjectRes(l_resNames[l_bmdGetParamList[0].arcIdx],
                                     l_bmdGetParamList[0].fileIdx));
        }
    }

    mAnm_p = new mDoExt_McaMorfSO(model_data, NULL, NULL, NULL, -1, 1.0f, 0, -1,
                                  &mCreatureSound, 0x80000, 0x11020284);
    if (mAnm_p != NULL && mAnm_p->getModel() == NULL) {
        mAnm_p->stopZelAnime();
        mAnm_p = NULL;
    }
    if (mAnm_p == NULL) {
        return 0;
    }

    if (!mInvisibleModel.create(mAnm_p->getModel(), 1)) {
        return 0;
    }
    
    J3DModel* model = mAnm_p->getModel();
    for (u16 i = 0; i < model_data->getJointNum(); i++) {
        model_data->getJointNodePointer(i)->setCallBack(ctrlJointCallBack);
    }
    model->setUserArea((uintptr_t)this);

    mpMatAnm = new daNpcF_MatAnm_c();
    if (mpMatAnm == NULL) {
        return 0;
    }

    if (!setExpressionAnm(ANM_F_TALK_A, false)) {
        return 0;
    }
    setMotionAnm(ANM_WAIT_GT_A, 0.0f);

    return 1;
}

int daNpc_zrZ_c::Delete() {
    this->~daNpc_zrZ_c();
    return 1;
}

int daNpc_zrZ_c::Execute() {
    return execute();
}

int daNpc_zrZ_c::Draw() {
    mAnm_p->getModel()->getModelData()->getMaterialNodePointer(1)->setMaterialAnm(mpMatAnm);
    if (mType == 1) {
        return draw(chkAction(&daNpc_zrZ_c::test), false, daNpc_zrZ_Param_c::m.mShadowDepth, NULL, false);
    } else {
        return daNpcF_c::draw(chkAction(&daNpc_zrZ_c::test), false, daNpc_zrZ_Param_c::m.mShadowDepth,
                              NULL, false);
    }
}

int daNpc_zrZ_c::draw(int i_isTest, int param_1, f32 i_shadowDepth, _GXColorS10* i_fogColor,
                      int i_hideDamage) {
    f32 damage_ratio, frame;
    J3DModel* model = mAnm_p->getModel();
    J3DModelData* modelData = model->getModelData();
    field_0x9f3 = 1;

    if (!mHide) {
        if (!i_hideDamage && mDamageTimer != 0 && mTotalDamageTimer != 0) {
            damage_ratio = (f32)mDamageTimer / (f32)mTotalDamageTimer;
        } else {
            damage_ratio = 0.0f;
        }

        if (cM3d_IsZero(damage_ratio) == FALSE) {
            tevStr.TevColor.r = (s16)(damage_ratio * 20.0f);
            tevStr.TevColor.g = 0;
        } else if (i_isTest) {
            tevStr.TevColor.g = 20;
            tevStr.TevColor.r = 0;
        } else if (i_fogColor != NULL) {
            tevStr.TevColor.r = i_fogColor->r;
            tevStr.TevColor.g = i_fogColor->g;
            tevStr.TevColor.b = i_fogColor->b;
            tevStr.TevColor.a = i_fogColor->a;
        } else {
            tevStr.TevColor.g = 0;
            tevStr.TevColor.r = 0;
        }

        if (tevStr.TevColor.a == 0) {
            return 1;
        }

        g_env_light.settingTevStruct(7, &current.pos, &tevStr);
        g_env_light.setLightTevColorType_MAJI(model->getModelData(), &tevStr);

        if (!drawDbgInfo()) {
            if (mAnmFlags & ANM_PLAY_BTP) {
                mBtpAnm.entry(&modelData->getMaterialTable(), (s16)mBtpAnm.getFrame());
            }
            if (mAnmFlags & ANM_PLAY_BTK) {
                frame = mBtkAnm.getFrame();
                mBtkAnm.entry(&modelData->getMaterialTable(), frame);
            }
            if (mAnmFlags & ANM_PLAY_BRK) {
                frame = mBrkAnm.getFrame();
                mBrkAnm.entry(&modelData->getMaterialTable(), frame);
            }

            if (param_1) {
                fopAcM_setEffectMtx(this, modelData);
            }

            if (dKy_darkworld_check()) {
                dComIfGd_setListDark();
                mInvisibleModel.entryDL(NULL);
                dComIfGd_setList();
            } else {
                mInvisibleModel.entryDL(NULL);
            }

            if (mAnmFlags & ANM_PLAY_BTP) {
                mBtpAnm.remove(modelData);
            }
            if (mAnmFlags & ANM_PLAY_BTK) {
                mBtkAnm.remove(modelData);
            }
            if (mAnmFlags & ANM_PLAY_BRK) {
                mBrkAnm.remove(modelData);
            }

            drawOtherMdls();
        }
    }
    return 1;
}

int daNpc_zrZ_c::ctrlJoint(J3DJoint* i_joint, J3DModel* i_model) {
    int jnt_no = i_joint->getJntNo();
    int lookat_joints[3] = {1, 3, 4};
    Mtx base_mtx, inv_rot;
    cXyz pos;

    if (jnt_no == 0) {
        mDoMtx_stack_c::copy(mAnm_p->getModel()->getAnmMtx(1));
        mDoMtx_stack_c::multVecZero(&mLookatPos[0]);
        mDoMtx_stack_c::copy(mAnm_p->getModel()->getAnmMtx(3));
        mDoMtx_stack_c::multVecZero(&mLookatPos[1]);
        mDoMtx_stack_c::copy(mAnm_p->getModel()->getAnmMtx(4));
        mDoMtx_stack_c::multVecZero(&mLookatPos[2]);
    }

    mDoMtx_stack_c::copy(i_model->getAnmMtx(jnt_no));

    switch (jnt_no) {
    case 1:   // backbone1
    case 3:   // neck
    case 4:   // head
        setLookatMtx(jnt_no, lookat_joints, daNpc_zrZ_Param_c::m.mNeckAngleScl);
        break;
    
    case 14:  // armL1
    case 22:  // armR1
    case 29:  // waist
        if (jnt_no == 29) {
            himoCalc();
        }

        MTXCopy(mDoMtx_stack_c::get(), base_mtx);
        pos.set(base_mtx[0][3], base_mtx[1][3], base_mtx[2][3]);
        base_mtx[0][3] = base_mtx[1][3] = base_mtx[2][3] = 0.0f;

        mDoMtx_stack_c::ZXYrotS(mCurAngle);
        mDoMtx_stack_c::inverse();
        MTXCopy(mDoMtx_stack_c::get(), inv_rot);

        mDoMtx_stack_c::transS(pos);
        mDoMtx_stack_c::ZXYrotM(mCurAngle);
        mDoMtx_stack_c::ZXYrotM(mLimbAngle);
        mDoMtx_stack_c::concat(inv_rot);
        mDoMtx_stack_c::concat(base_mtx);
        break;
    }

    if (jnt_no == 1) {
        mDoMtx_stack_c::YrotM(field_0x908[0].z);
        mDoMtx_stack_c::ZrotM(-field_0x908[0].x);
    } else if (jnt_no == 4) {
        mDoMtx_stack_c::YrotM(field_0x908[2].z);
        mDoMtx_stack_c::ZrotM(field_0x908[2].x);
    }

    i_model->setAnmMtx(jnt_no, mDoMtx_stack_c::get());
    MTXCopy(mDoMtx_stack_c::get(), J3DSys::mCurrentMtx);

    if ((jnt_no == 4 || jnt_no == 12) && (mAnmFlags & ANM_PLAY_BCK)) {
        J3DAnmTransform* bck_anm = mBckAnm.getBckAnm();
        mBckAnm.changeBckOnly(mAnm_p->getAnm());
        mAnm_p->changeAnm(bck_anm);
    }

    return 1;
}

int daNpc_zrZ_c::createHeapCallBack(fopAc_ac_c* i_this) {
    return static_cast<daNpc_zrZ_c*>(i_this)->CreateHeap();
}

int daNpc_zrZ_c::ctrlJointCallBack(J3DJoint* i_joint, int param_1) {
    if (param_1 == 0) {
        daNpc_zrZ_c* _this = (daNpc_zrZ_c*)j3dSys.getModel()->getUserArea();
        if (_this != NULL) {
            _this->ctrlJoint(i_joint, j3dSys.getModel());
        }
    }
    return 1;
}

static void* s_sub(void* i_proc, void* i_this) {
    if (fopAc_IsActor(i_proc) && fopAcM_GetName(i_proc) == PROC_Obj_GraveStone) {
        return i_proc;
    }
    return NULL;
}

static void* s_subCloth(void* i_proc, void* i_this) {
    if (fopAc_IsActor(i_proc) && fopAcM_GetName(i_proc) == PROC_Obj_ZoraCloth) {
        return i_proc;
    }
    return NULL;
}

static void* s_subRock(void* i_proc, void* i_this) {
    if (fopAc_IsActor(i_proc) && fopAcM_GetName(i_proc) == PROC_Obj_ZraRock) {
        return i_proc;
    }
    return NULL;
}

void daNpc_zrZ_c::setParam() {
    ActionFn action = mpNextActionFn;
    u32 attn_flags = 10;
    selectAction();

    if (!mTwilight && daPy_py_c::checkNowWolf()) {
        attn_flags = 0;
    }

    if (mpNextActionFn != action) {
        for (int i = 2; i < 2; i++) {
            mActorMngr[i].initialize();
        }
    }

    field_0x1434 = 0;
    field_0x1438 = 0;

    attention_info.distances[fopAc_attn_LOCK_e] = getDistTableIdx(5, 6);
    attention_info.distances[fopAc_attn_TALK_e] = attention_info.distances[fopAc_attn_LOCK_e];
    attention_info.distances[fopAc_attn_SPEAK_e] = getDistTableIdx(3, 6);
    attention_info.flags = attn_flags;

    scale.set(daNpc_zrZ_Param_c::m.mScale,
              daNpc_zrZ_Param_c::m.mScale,
              daNpc_zrZ_Param_c::m.mScale);
    
    mAcchCir.SetWallR(daNpc_zrZ_Param_c::m.mWallR);
    mAcchCir.SetWallH(daNpc_zrZ_Param_c::m.mWallH);
    gravity = daNpc_zrZ_Param_c::m.mGravity;

    u32 uvar4 = cLib_minMaxLimit<u32>(tevStr.TevColor.a * 100 / 0xff, 1, 100);
    mCreatureSound.startCreatureSoundLevel(Z2SE_ZRZ_MV, uvar4, -1);
}

BOOL daNpc_zrZ_c::main() {
    if (!doEvent()) {
        doNormalAction(1);
    }

    attention_info.flags = 0;

    if (!daNpc_zrZ_Param_c::m.mTest
        && (!dComIfGp_event_runCheck() || (mOrderNewEvt && dComIfGp_getEvent().isOrderOK())))
    {
        if (mOrderEvtNo != EVT_NONE) {
            eventInfo.setArchiveName(l_resNames[l_evtGetParamList[mOrderEvtNo].arcIdx]);
        }
        orderEvent(mOrderSpeakEvt, l_evtNames[l_evtGetParamList[mOrderEvtNo].fileIdx],
                   0xffff, 4, 0xff, 1);
    }

    if (field_0x9ee) {
        mExpressionMorfOverride = 0.0f;
        mMotionMorfOverride = 0.0f;
        field_0x9ee = false;
    }

    playExpression();
    playMotion();

    mAcch.SetGrndNone();
    mAcch.SetWallNone();
    gravity = 0.0f;
    speed.setall(0.0f);
    speedF = 0.0f;

    mModulationOffset = 8.5f - cM_scos(mModulationParam * 0x10000 / 90) * 8.5f;
    mModulationParam--;
    if (mModulationParam <= 0) {
        mModulationParam = 90;
        mModulationOffset = 0.0f;
    }

    return TRUE;
}

BOOL daNpc_zrZ_c::ctrlBtk() {
    if (mpMatAnm != NULL) {
        J3DAnmTextureSRTKey* btk_anm = NULL;
        if (mType == 1) {
            if (l_btkGTGetParamList[0].fileIdx >= 0) {
                btk_anm = getTexSRTKeyAnmP(l_resNames[l_btkGTGetParamList[0].arcIdx],
                                           l_btkGTGetParamList[0].fileIdx);
            }
        } else {
            if (l_btkGetParamList[0].fileIdx >= 0) {
                btk_anm = getTexSRTKeyAnmP(l_resNames[l_btkGetParamList[0].arcIdx],
                                           l_btkGetParamList[0].fileIdx);
            }
        }

        if (btk_anm == mBtkAnm.getBtkAnm()) {
            mpMatAnm->setNowOffsetX(cM_ssin(mEyeAngle.y) * 0.2f * -1.0f);
            mpMatAnm->setNowOffsetY(cM_ssin(mEyeAngle.x) * 0.2f);
            mpMatAnm->onEyeMoveFlag();
            return TRUE;
        }
        mpMatAnm->offEyeMoveFlag();
    }

    return FALSE;
}

void daNpc_zrZ_c::setAttnPos() {
    static cXyz eyeOffset(-10.0f, 10.0f, 0.0f);
    f32 offset = daNpc_zrZ_Param_c::m.mAttnOffsetY;
    cXyz center, vec2, vec3, vec4;

    mDoMtx_stack_c::YrotS(field_0x990);

    cLib_addCalc2(&field_0x984[0], 0.0f, 0.1f, 125.0f);
    cLib_addCalc2(&field_0x984[2], 0.0f, 0.1f, 125.0f);

    for (int i = 0; i < 3; i++) {
        vec3.set(0.0f, 0.0f, field_0x984[i] * cM_ssin(field_0x992));
        mDoMtx_stack_c::multVec(&vec3, &vec4);
        field_0x908[i].x = -vec4.z;
        field_0x908[i].z = -vec4.x;
    }

    cLib_chaseS(&field_0x992, 0, 0x555);

    if (mLookMode == LOOK_RESET) {
        for (int i = 0; i < 3; i++) {
            mLookatAngle[i].setall(0);
        }
    }

    J3DModelData* model_data = mAnm_p->getModel()->getModelData();

    setMtx();
    lookat();

    mDoMtx_stack_c::copy(mAnm_p->getModel()->getAnmMtx(4));
    mDoMtx_stack_c::multVecZero(&mHeadPos);
    mDoMtx_stack_c::multVec(&eyeOffset, &eyePos);
    eyeOffset.y = 0.0f;
    mDoMtx_stack_c::multVec(&eyeOffset, &vec3);
    mHeadAngle.x = cLib_targetAngleX(&mHeadPos, &vec3);
    mHeadAngle.y = cLib_targetAngleY(&mHeadPos, &vec3);

    cXyz* attn_pos = mLookat.getAttnPos();
    if (attn_pos != NULL) {
        vec2 = *attn_pos - eyePos;
        mEyeAngle.y = -(mLookatAngle[2].y + mCurAngle.y);
        mEyeAngle.y += cM_atan2s(vec2.x, vec2.z);
        mEyeAngle.x = -cM_atan2s(vec2.y, vec2.absXZ());
        mEyeAngle.x += mHeadAngle.x;
    } else {
        mEyeAngle.x = mEyeAngle.y = 0;
    }

    f32 extra_height = 0.0f;
    f32 extra_radius = 0.0f;
    vec2.set(0.0f, 0.0f, 0.0f);
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::ZXYrotM(mCurAngle.x, mCurAngle.y, mCurAngle.z);
    mDoMtx_stack_c::multVec(&vec2, &center);
    attention_info.position.set(center.x, center.y + offset, center.z);

    if (!mHide && !mTwilight) {
        if (!mIsDamaged) {
            mCcCyl.SetTgType(0xd8fbfdff);
            mCcCyl.SetTgSPrm(0x1f);
            mCcCyl.OnTgNoHitMark();
        } else {
            mCcCyl.SetTgType(0);
            mCcCyl.SetTgSPrm(0);
        }

        mCcCyl.SetC(center);
        mCcCyl.SetH(daNpc_zrZ_Param_c::m.mCylH + extra_height);
        mCcCyl.SetR(daNpc_zrZ_Param_c::m.mWallR + extra_radius);
        dComIfG_Ccsp()->Set(&mCcCyl);
        mCcCyl.SetCoSPrm(0x69);
    }
}

void daNpc_zrZ_c::setMtx() {
    J3DModel* model = mAnm_p->getModel();
    cXyz pos = current.pos;
    pos.y += mModulationOffset;
    mDoMtx_stack_c::transS(pos);
    mDoMtx_stack_c::ZXYrotM(mCurAngle);
    mDoMtx_stack_c::scaleM(scale);
    model->setBaseTRMtx(mDoMtx_stack_c::get());
    model->setUserArea((uintptr_t)this);

    if (mAnmFlags & ANM_PLAY_BCK) {
        mBckAnm.getBckAnm()->setFrame(mBckAnm.getFrame());
        mAnm_p->modelCalc();
    } else {
        mAnm_p->modelCalc();
    }
}

bool daNpc_zrZ_c::setExpressionAnm(int i_idx, bool i_modify) {
    J3DAnmTransform* bck_anm = NULL;
    mAnmFlags &= ~ANM_EXPRESSION_FLAGS;

    if (mType == 1) {
        if (l_bckGTGetParamList[i_idx].fileIdx >= 0) {
            bck_anm = getTrnsfrmKeyAnmP(l_resNames[l_bckGTGetParamList[i_idx].arcIdx],
                                        l_bckGTGetParamList[i_idx].fileIdx);
        }
    } else {
        if (l_bckGetParamList[i_idx].fileIdx >= 0) {
            bck_anm = getTrnsfrmKeyAnmP(l_resNames[l_bckGetParamList[i_idx].arcIdx],
                                        l_bckGetParamList[i_idx].fileIdx);
        }
    }

    bool res = false;
    switch (i_idx) {
    case 0:
    case 1:
        res = setExpressionBtp(0);
        break;
    default:
        bck_anm = NULL;
        break;
    }

    if (!res) {
        return false;
    }

    if (bck_anm == NULL) {
        return true;
    }

    if (setBckAnm(bck_anm, 1.0f, J3DFrameCtrl::EMode_NONE, 0, -1, i_modify)) {
        mAnmFlags |= ANM_PLAY_BCK | ANM_PAUSE_BCK;
        mExpressionLoops = 0;
        return true;
    }

    return false;
}

bool daNpc_zrZ_c::setExpressionBtp(int i_idx) {
    J3DAnmTexPattern* btp_anm = NULL;
    int attr = J3DFrameCtrl::EMode_NONE;
    mAnmFlags &= ~(ANM_PLAY_BTP | ANM_PAUSE_BTP | ANM_FLAG_800);

    if (mType == 1) {
        if (l_btpGTGetParamList[i_idx].fileIdx >= 0) {
            btp_anm = getTexPtrnAnmP(l_resNames[l_btpGTGetParamList[i_idx].arcIdx],
                                     l_btpGTGetParamList[i_idx].fileIdx);
        }
    } else {
        if (l_btpGetParamList[i_idx].fileIdx >= 0) {
            btp_anm = getTexPtrnAnmP(l_resNames[l_btpGetParamList[i_idx].arcIdx],
                                     l_btpGetParamList[i_idx].fileIdx);
        }
    }

    switch (i_idx) {
    case 0:
        attr = J3DFrameCtrl::EMode_LOOP;
        break;
    default:
        btp_anm = NULL;
        break;
    }

    if (btp_anm == NULL) {
        return true;
    }

    if (setBtpAnm(btp_anm, mAnm_p->getModel()->getModelData(), 1.0f, attr)) {
        mAnmFlags |= ANM_PLAY_BTP | ANM_PAUSE_BTP;
        if (i_idx == 0) {
            mAnmFlags |= ANM_FLAG_800;
        }
        return true;
    }

    return false;
}

void daNpc_zrZ_c::setExpression(int i_expression, f32 i_morf) {
    if (i_expression >= 0 && i_expression < 2) {
        mExpression = i_expression;
        mExpressionMorfOverride = i_morf;
        mExpressionPrevPhase = -1;
        mExpressionPhase = 0;
    }
}

void daNpc_zrZ_c::setMotionAnm(int i_idx, f32 i_morf) {
    J3DAnmTransformKey* bck_anm = NULL;
    J3DAnmTextureSRTKey* btk_anm = NULL;
    int attr = J3DFrameCtrl::EMode_LOOP;
    mAnmFlags &= ~ANM_MOTION_FLAGS;

    if (mType == 1) {
        if (l_bckGTGetParamList[i_idx].fileIdx >= 0) {
            bck_anm = getTrnsfrmKeyAnmP(l_resNames[l_bckGTGetParamList[i_idx].arcIdx],
                                        l_bckGTGetParamList[i_idx].fileIdx);
        }
    } else {
        if (l_bckGetParamList[i_idx].fileIdx >= 0) {
            bck_anm = getTrnsfrmKeyAnmP(l_resNames[l_bckGetParamList[i_idx].arcIdx],
                                        l_bckGetParamList[i_idx].fileIdx);
        }
    }

    switch (i_idx) {
    case 2:
    case 3:
        break;
    case 4:
        attr = J3DFrameCtrl::EMode_NONE;
        break;
    default:
        bck_anm = NULL;
        btk_anm = NULL;
        break;
    }

    if (mType == 1) {
        if (l_btkGTGetParamList[0].fileIdx >= 0) {
            btk_anm = getTexSRTKeyAnmP(l_resNames[l_btkGTGetParamList[0].arcIdx],
                                       l_btkGTGetParamList[0].fileIdx);
        }
    } else {
        if (l_btkGetParamList[0].fileIdx >= 0) {
            btk_anm = getTexSRTKeyAnmP(l_resNames[l_btkGetParamList[0].arcIdx],
                                       l_btkGetParamList[0].fileIdx);
        }
    }

    if (btk_anm != NULL && setBtkAnm(btk_anm, mAnm_p->getModel()->getModelData(), 1.0f, 2)) {
        mAnmFlags |= ANM_PLAY_BTK | ANM_PAUSE_BTK;
    }

    if (bck_anm != NULL && setMcaMorfAnm(bck_anm, 1.0f, i_morf, attr, 0, -1)) {
        mAnmFlags |= ANM_PLAY_MORF | ANM_PAUSE_MORF;
        mMotionLoops = 0;
    }
}

void daNpc_zrZ_c::setMotion(int i_motion, f32 i_morf, BOOL i_restart) {
    s16 motion = i_motion;
    if (i_restart || mMotion != motion) {
        if (i_motion >= 0 && i_motion < 3) {
            mMotion = motion;
            mMotionMorfOverride = i_morf;
            mMotionPrevPhase = -1;
            mMotionPhase = 0;
        }
    }
}

BOOL daNpc_zrZ_c::drawDbgInfo() {
    return false;
}

void daNpc_zrZ_c::drawOtherMdls() {
    /* empty function */
}

u8 daNpc_zrZ_c::getTypeFromParam() {
    switch (fopAcM_GetParam(this) & 0xff) {
    case 0:
        return 1;
    case 1:
        return 0;
    default:
        return 1;
    }
}

BOOL daNpc_zrZ_c::isDelete() {
    if (((mDemoMode == DEMO_COME_HERE || mDemoMode == DEMO_WAIT)
                    && dComIfGs_isSwitch(mSwitch1, fopAcM_GetRoomNo(this)))
        || (mDemoMode == DEMO_COME_HERE_2 && (!dComIfGs_isSwitch(mSwitch1, fopAcM_GetRoomNo(this))
                                          || dComIfGs_isSwitch(mSwitch2, fopAcM_GetRoomNo(this)))))
    {
        return true;
    } else {
        return false;
    }
}

void daNpc_zrZ_c::reset() {
    initialize();
    mpMatAnm->initialize();
    mPath.initialize();
    if (mPath.setPathInfo(getPathNoFromParam(), fopAcM_GetRoomNo(this), 0)) {
        mPath.setRange(100.0f);
    }
    mLookat.initialize();
    for (int i = 0; i < 2; i++) {
        mActorMngr[i].initialize();
    }

    mpNextActionFn = NULL;
    mpActionFn = NULL;
    field_0x1434 = 0;
    field_0x1438 = 0;
    field_0x143c = 0;
    field_0x1440 = 0;
    mLookMode = -1;
    mMode = 0;
    mHide = false;

    if (mDemoMode == DEMO_WAIT) {
        mIsLeading = false;
        tevStr.TevColor.a = 0;
        mpNextActionFn = &daNpc_zrZ_c::wait;
    } else if (mDemoMode == DEMO_COME_HERE || mDemoMode == DEMO_COME_HERE_2) {
        mIsLeading = false;
        tevStr.TevColor.a = 0xff;
    } else {
        mIsLeading = true;
        tevStr.TevColor.a = 0xff;
    }

    setLookMode(LOOK_NONE);
    current.pos = home.pos;
    old.pos = current.pos;
    current.angle.set(0, home.angle.y, 0);
    old.angle = current.angle;
    shape_angle = current.angle;
    mCurAngle = current.angle;
    mOldAngle = mCurAngle;
    speedF = 0.0f;
    speed.setall(0.0f);
    mItemID = -1;
    mModulationParam = 90;
    mModulationOffset = 0.0f;
    mIsMoving = false;
    mSpeed = 0.0f;
    mMoveAngle.x = current.angle.x;
    mMoveAngle.y = current.angle.y + 0x8000;
    mMoveAngle.z = current.angle.z;
    mpGravestoneActor = NULL;
    mpClothActor = NULL;
    mpRockActor = NULL;
    mLimbCalcPos = current.pos;
    mLimbCalcPos.y -= daNpc_zrZ_Param_c::m.field_0x80;
    mLimbAngle.set(0, 0, 0);
    mClothesObtained = false;
    mMusicSet = false;
    mSealReleased = false;
    field_0x9ee = true;
}

void daNpc_zrZ_c::playExpression() {
    daNpcF_anmPlayData dat0a = {ANM_F_TALK_A, daNpc_zrZ_Param_c::m.mMorfFrames, 1};
    daNpcF_anmPlayData dat0b = {ANM_NONE, daNpc_zrZ_Param_c::m.mMorfFrames, 0};
    daNpcF_anmPlayData* pDat0[2] = {&dat0a, &dat0b};
    daNpcF_anmPlayData dat1 = {ANM_NONE, daNpc_zrZ_Param_c::m.mMorfFrames, 0};
    daNpcF_anmPlayData* pDat1[1] = {&dat1};
    daNpcF_anmPlayData** ppDat[2] = {
        pDat0, pDat1,
    };
    if (mExpression >= 0 && mExpression < 2) {
        playExpressionAnm(ppDat);
    }
}

void daNpc_zrZ_c::playMotion() {
    daNpcF_anmPlayData dat0 = {ANM_WAIT_GT_A, daNpc_zrZ_Param_c::m.mMorfFrames, 0};
    daNpcF_anmPlayData* pDat0[1] = {&dat0};
    daNpcF_anmPlayData dat1a = {ANM_COMEON, daNpc_zrZ_Param_c::m.mMorfFrames, 1};
    daNpcF_anmPlayData dat1b = {ANM_WAIT_GT_A, daNpc_zrZ_Param_c::m.mMorfFrames, 0};
    daNpcF_anmPlayData* pDat1[2] = {&dat1a, &dat1b};
    daNpcF_anmPlayData dat2 = {ANM_LEAD, daNpc_zrZ_Param_c::m.mMorfFrames, 0};
    daNpcF_anmPlayData* pDat2[1] = {&dat2};
    daNpcF_anmPlayData** ppDat[3] = {
        pDat0, pDat1, pDat2,
    };
    if (mMotion >= 0 && mMotion < 3) {
        playMotionAnm(ppDat);
    }
}

BOOL daNpc_zrZ_c::chkAction(ActionFn i_action) {
    return mpActionFn == i_action;
}

BOOL daNpc_zrZ_c::setAction(ActionFn i_action) {
    mMode = 3;
    if (mpActionFn != NULL) {
        (this->*mpActionFn)(NULL);
    }
    mMode = 0;
    mpActionFn = i_action;
    if (mpActionFn != NULL) {
        (this->*mpActionFn)(NULL);
    }
    return true;
}

BOOL daNpc_zrZ_c::selectAction() {
    mpNextActionFn = NULL;
    if (daNpc_zrZ_Param_c::m.mTest) {
        mpNextActionFn = &daNpc_zrZ_c::test;
    } else {
        switch (mDemoMode) {
        case DEMO_COME_HERE:
            mpNextActionFn = &daNpc_zrZ_c::comeHere;
            break;
        case DEMO_COME_HERE_2:
            mpNextActionFn = &daNpc_zrZ_c::comeHere2;
            break;
        default:
            mpNextActionFn = &daNpc_zrZ_c::wait;
            break;
        }
    }
    return true;
}

void daNpc_zrZ_c::doNormalAction(BOOL param_0) {
    if (param_0 && hitChk2(&mCcCyl, true, false)) {
        int timer;
        if (mCutType == daPy_py_c::CUT_TYPE_TURN_RIGHT) {
            timer = 20;
        } else {
            timer = daNpc_zrZ_Param_c::m.mDamageTimer;
        }
        setDamage(timer, EXPR_NONE, MOT_WAIT_GT_A);
        setLookMode(LOOK_RESET);
    } else {
        if (mIsDamaged && mDamageTimer == 0) {
            mMode = 0;
            mIsDamaged = false;
        }
    }

    mOrderEvtNo = EVT_NONE;
    if (mpNextActionFn != NULL) {
        if (mpActionFn == mpNextActionFn) {
            (this->*mpActionFn)(NULL);
        } else {
            setAction(mpNextActionFn);
        }
    }
}

BOOL daNpc_zrZ_c::doEvent() {
    BOOL ret = false;

    if (dComIfGp_event_runCheck() != false) {
        dEvent_manager_c& event_manager = dComIfGp_getEventManager();

        if (eventInfo.checkCommandTalk() || eventInfo.checkCommandDemoAccrpt()) {
            mOrderNewEvt = false;
        }

        if (eventInfo.checkCommandTalk()) {
            if (chkAction(&daNpc_zrZ_c::talk)) {
                (this->*mpActionFn)(NULL);
            } else if (dComIfGp_event_chkTalkXY() == false || dComIfGp_evmng_ChkPresentEnd()) {
                setAction(&daNpc_zrZ_c::talk);
            }
            ret = true;
        } else {
            if (mItemID != -1) {
                dComIfGp_event_setItemPartnerId(mItemID);
                mItemID = -1;
            }

            int staff_id = event_manager.getMyStaffId(l_myName, NULL, 0);
            if (staff_id != -1) {
                mStaffID = staff_id;
                int act_idx = event_manager.getMyActIdx(staff_id, mEvtCutNameList,
                                                        ARRAY_SIZEU(mEvtCutNameList), 0, 0);
                if ((this->*mEvtCutList[act_idx])(staff_id)) {
                    event_manager.cutEnd(staff_id);
                }
                ret = true;
            }

            if (eventInfo.checkCommandDemoAccrpt()) {
                if (mEventIdx != -1 && event_manager.endCheck(mEventIdx)) {
                    switch (mOrderEvtNo) {
                    case EVT_GET_AFTER:
                        dComIfGp_event_reset();
                        mOrderEvtNo = EVT_NONE;
                        mEventIdx = -1;
                        dComIfGs_onSwitch(mSwitch2, fopAcM_GetRoomNo(this));
                        fopAcM_delete(this);
                        break;

                    case EVT_CLOTHES_GET:
                        dComIfGp_event_reset();
                        mClothesObtained = true;
                        mOrderEvtNo = EVT_CLOTHES_GET;
                        mEventIdx = -1;
                        dComIfGs_onSwitch(mSwitch2, fopAcM_GetRoomNo(this));
                        fopAcM_delete(this);
                        break;

                    case EVT_HELP_PRINCE:
                        dComIfGp_event_reset();
                        mOrderEvtNo = EVT_NONE;
                        mEventIdx = -1;
                        fopAcM_delete(this);
                        break;

                    case EVT_SEAL_RELEASE:
                        dComIfGp_event_reset();
                        mOrderEvtNo = EVT_NONE;
                        mEventIdx = -1;
                        if (mpRockActor != NULL) {
                            fopAcM_delete(mpRockActor);
                        }
                        break;

                    default:
                        dComIfGp_event_reset();
                        mOrderEvtNo = EVT_NONE;
                        mEventIdx = -1;
                        break;
                    }
                } else {
                    switch (mOrderEvtNo) {
                    case EVT_SEAL_RELEASE:
                        setSkipZev(EVT_SEAL_RELEASE, EVT_SR_SKIP);
                        dComIfGp_getEvent().onSkipFade();
                        break;
                    }
                }
            }
        }

        int expression, motion;
        int prev_msg_timer = mMsgTimer;
        if (ctrlMsgAnm(expression, motion, this, false) != 0) {
            if (!field_0x9eb) {
                setExpression(expression, -1.0f);
                setMotion(motion, -1.0f, false);
            }
        } else if (prev_msg_timer != 0 && !field_0x9eb) {
            setExpressionTalkAfter();
        }
    } else {
        mMsgTimer = 0;
        if (mStaffID != -1) {
            mpActionFn = NULL;
            mStaffID = -1;
        }
    }

    return ret;
}

BOOL daNpc_zrZ_c::setSkipZev(int i_idx1, int i_idx2) {
    if (!strcmp(dComIfGp_getEventManager().getRunEventName(), l_evtNames[i_idx1])) {
        dComIfGp_getEvent().setSkipZev(this, l_evtNames[i_idx2]);
        return true;
    }
    return false;
}

void daNpc_zrZ_c::setLookMode(int i_lookMode) {
    if (i_lookMode >= 0 && i_lookMode < 5 && i_lookMode != mLookMode) {
        mLookMode = i_lookMode;
    }
}

void daNpc_zrZ_c::lookat() {
    fopAc_ac_c* attn_actor = NULL;
    J3DModel* model = mAnm_p->getModel();
    BOOL snap = false;
    f32 body_down_angle = daNpc_zrZ_Param_c::m.mBodyDownAngle;
    f32 body_up_angle = daNpc_zrZ_Param_c::m.mBodyUpAngle;
    f32 body_right_angle = daNpc_zrZ_Param_c::m.mBodyRightAngle;
    f32 body_left_angle = daNpc_zrZ_Param_c::m.mBodyLeftAngle;
    f32 head_down_angle = daNpc_zrZ_Param_c::m.mHeadDownAngle;
    f32 head_up_angle = daNpc_zrZ_Param_c::m.mHeadUpAngle;
    f32 head_right_angle = daNpc_zrZ_Param_c::m.mHeadRightAngle;
    f32 head_left_angle = daNpc_zrZ_Param_c::m.mHeadLeftAngle;
    s16 angle_delta = mCurAngle.y - mOldAngle.y;
    cXyz lookat_pos[3] = {mLookatPos[0], mLookatPos[1], mLookatPos[2]};
    csXyz* lookat_angle[3] = {&mLookatAngle[0], &mLookatAngle[1], &mLookatAngle[2]};

    switch (mLookMode) {
    case LOOK_NONE:
        break;
    case LOOK_RESET:
        snap = true;
        break;
    case LOOK_PLAYER:
    case LOOK_PLAYER_TALK:
        attn_actor = daPy_getPlayerActorClass();
        if (mLookMode == LOOK_PLAYER_TALK) {
            head_right_angle = -80.0f;
            head_left_angle = 80.0f;
        }
        break;
    case LOOK_ACTOR:
        attn_actor = mActorMngr[1].getActorP();
        break;
    }

    if (attn_actor != NULL) {
        mLookPos = attn_actor->attention_info.position;
        if (mLookMode != LOOK_PLAYER && mLookMode != LOOK_PLAYER_TALK && mLookMode != LOOK_ACTOR) {
            mLookPos.y -= 40.0f;
        }
        mLookat.setAttnPos(&mLookPos);
    } else {
        mLookat.setAttnPos(NULL);
    }

    mLookat.setParam(body_down_angle, body_up_angle, body_right_angle, body_left_angle,
                     0.0f, 0.0f, 0.0f, 0.0f,
                     head_down_angle, head_up_angle, head_right_angle, head_left_angle,
                     mCurAngle.y, lookat_pos);
    mLookat.calc(this, model->getBaseTRMtx(), lookat_angle, snap, angle_delta, false);
}

void daNpc_zrZ_c::setExpressionTalkAfter() {
    setExpression(EXPR_NONE, -1.0f);
}

void daNpc_zrZ_c::lightReady() {
    setLightPos();
    mLight.mColor.r = 0;
    mLight.mColor.g = 0;
    mLight.mColor.b = 0;
    mLight.mPow = 0.0f;
    mLight.mFluctuation = 0.0f;
    mAllcolRatio = 1.0f;
    mUseLightEffect = false;
}

void daNpc_zrZ_c::setLightPos() {
    fopAc_ac_c* gravestone = (fopAc_ac_c*)fpcM_Search(s_sub, this);
    if (gravestone != NULL) {
        cXyz offset(0.0f, 0.0f, 0.0f);
        cXyz pos;
        cLib_offsetPos(&pos, &gravestone->current.pos, gravestone->current.angle.y, &offset);
        mLight.mPosition.set(pos.x, pos.y, pos.z);
    }
}

void daNpc_zrZ_c::lightPowerCalc(int param_0) {
    if (param_0 == 30) {
        mLight.mPow = (50 - mEventTimer) * 70.0f / 50.0f;
    } else if (mLightEffectFrame > 200) {
        cLib_addCalc0(&mLight.mPow, 0.05f, 5.0f);
    } else {
        mLight.mPow = cM_ssin(mLightEffectFrame * 3500) * 14.0f + 70.0f;
    }
}

void daNpc_zrZ_c::lightColorProc() {
    static int const key_frame[4] = {
        0, 50, 190, 220,
    };

    static GXColorS10 const key_color[4] = {
        {0x00, 0x00, 0x00, 0xFF},
        {0xFF, 0xC3, 0x8A, 0xFF},
        {0xFF, 0xC3, 0x8A, 0xFF},
        {0x00, 0x00, 0x00, 0xFF},
    };

    if (mLightEffectFrame >= 220) {
        return;
    }

    int prev_key_frame, next_key_frame, next, i;
    for (i = 0; i < 4; i++) {
        if (key_frame[i] <= mLightEffectFrame && key_frame[i + 1] > mLightEffectFrame) {
            // The following is present to cause dbg asm to use r31 for stack reg:
            int nested_var;
            prev_key_frame = key_frame[i];
            next_key_frame = key_frame[i + 1];
            next = i + 1;
            break;
        }
    }

    int frames = next_key_frame - prev_key_frame;
    GXColorS10 color;
    color.r = std::fabs(key_color[i + 1].r - key_color[i].r) / frames;
    color.g = std::fabs(key_color[i + 1].g - key_color[i].g) / frames;
    color.b = std::fabs(key_color[i + 1].b - key_color[i].b) / frames;
    ++color.r;
    ++color.g;
    ++color.b;
    cLib_chaseS(&mLight.mColor.r, key_color[next].r, color.r);
    cLib_chaseS(&mLight.mColor.g, key_color[next].g, color.g);
    cLib_chaseS(&mLight.mColor.b, key_color[next].b, color.b);
}

static u8 const lit_5782[8] = {
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
};

BOOL daNpc_zrZ_c::ECut_helpPrince(int i_staffID) {
    dEvent_manager_c& event_manager = dComIfGp_getEventManager();
    daPy_py_c* player = daPy_getPlayerActorClass();
    BOOL ret = false;
    int prm = -1;
    int timer = 0;
    csXyz angle;
    int* prm_p = dComIfGp_evmng_getMyIntegerP(i_staffID, "prm");
    if (prm_p != NULL) {
        prm = *prm_p;
    }
    int* timer_p = dComIfGp_evmng_getMyIntegerP(i_staffID, "timer");
    if (timer_p != NULL) {
        timer = *timer_p;
    }

    if (event_manager.getIsAddvance(i_staffID)) {
        switch (prm) {
        case 11:
            initTalk(0x3e9, NULL);
            setLookMode(LOOK_NONE);
            Z2GetAudioMgr()->muteSceneBgm(90, 0.0f);
            mIsLeading = true;
            daNpcF_offTmpBit(0xb);
            daNpcF_offTmpBit(0xc);
            daNpcF_offTmpBit(0xd);
            daNpcF_offTmpBit(0xe);
            daNpcF_offTmpBit(0xf);
            daNpcF_offTmpBit(0x33);
            daNpcF_offTmpBit(0x34);
            break;

        case 20:
            mEventTimer = timer;
            break;

        case 40:
            mEventTimer = timer;
            break;

        case 50: {
            initTalk(0x3e9, NULL);
            angle = player->current.angle;
            angle.y = fopAcM_searchPlayerAngleY(this) + 0x8000;
            cXyz pos(-898.02f, 14.565f, -1002.517f);
            player->setPlayerPosAndAngle(&pos, &angle);
            break;
        }

        case 51:
            break;

        case 60:
            initTalk(0x3e9, NULL);
            setAngle(fopAcM_searchPlayerAngleY(this));
            setLookMode(LOOK_PLAYER);
            break;

        case 70:
            break;

        case 71:
            initTalk(0x3e9, NULL);
            break;

        case 80:
            initTalk(0x3e9, NULL);
            break;

        case 81:
            break;

        case 82:
            break;

        case 83:
            break;

        case 90:
            initTalk(0x3e9, NULL);
            break;

        case 91:
            break;

        case 92:
            break;

        case 93:
            break;

        case 100:
            initTalk(0x3e9, NULL);
            break;

        case 101:
            break;

        case 102:
            break;

        case 103:
            mEventTimer = timer;
            Z2GetAudioMgr()->subBgmStop();
            Z2GetAudioMgr()->unMuteSceneBgm(90);
            break;

        case 110:
            initTalk(0x3e9, NULL);
            break;

        case 111:
            break;

        case 112:
            break;

        case 113:
            break;
        }
    }

    switch (prm) {
    case 11:
        if (talkProc(NULL, true, NULL)) {
            ret = true;
            if (!mMusicSet) {
                mMusicSet = true;
                Z2GetAudioMgr()->subBgmStart(Z2BGM_LUTERA1);
            }
        }
        setAngle(fopAcM_searchPlayerAngleY(this));
        break;

    case 50:
        ret = true;
        break;

    case 51:
        if (talkProc(NULL, true, NULL)) {
            ret = true;
        }
        break;

    case 60:
        if (talkProc(NULL, true, NULL)) {
            ret = true;
        }
        break;

    case 71:
        if (talkProc(NULL, true, NULL)) {
            ret = true;
        }
        break;

    case 80:
        ret = true;
        break;

    case 81:
        ret = true;
        break;

    case 82:
        if (talkProc(NULL, true, NULL)) {
            ret = true;
        }
        break;

    case 90:
        ret = true;
        break;

    case 91:
        ret = true;
        break;

    case 92:
        ret = true;
        break;

    case 93:
        if (talkProc(NULL, true, NULL)) {
            ret = true;
        }
        break;

    case 100:
        ret = true;
        break;

    case 101:
        ret = true;
        break;

    case 102:
        if (talkProc(NULL, true, NULL)) {
            ret = true;
        }
        break;

    case 110:
        ret = true;
        break;

    case 111:
        ret = true;
        break;

    case 112:
        ret = true;
        break;

    case 113:
        if (talkProc(NULL, true, NULL)) {
            if (!dComIfGs_isSwitch(mSwitch1, fopAcM_GetRoomNo(this))) {
                dComIfGs_onSwitch(mSwitch1, fopAcM_GetRoomNo(this));
            }
            ret = true;
        }
        break;

    case 20:
    case 70:
        if (cLib_calcTimer(&mEventTimer) == 0) {
            ret = true;
        }
        break;

    case 40:
        if (cLib_calcTimer(&mEventTimer) == 0) {
            ret = true;
            tevStr.TevColor.a = 0xff;
        } else {
            tevStr.TevColor.a = (u8)((60 - mEventTimer) / 60.0f * 255.0f);
        }
        break;

    case 103:
        if (cLib_calcTimer(&mEventTimer) == 0) {
            ret = true;
            tevStr.TevColor.a = 0;
        } else if (mEventTimer > 30) {
            tevStr.TevColor.a = (u8)((mEventTimer - 30) / 30.0f * 255.0f);
        } else {
            tevStr.TevColor.a = 0;
        }
        break;

    default:
        ret = true;
        break;
    }

    return ret;
}

BOOL daNpc_zrZ_c::ECut_comeHere(int i_staffID) {
    dEvent_manager_c& event_manager = dComIfGp_getEventManager();
    daPy_py_c* player = daPy_getPlayerActorClass();
    BOOL ret = false;
    int prm = -1;
    csXyz angle;
    bool facing_player;
    int* prm_p = dComIfGp_evmng_getMyIntegerP(i_staffID, "prm");
    if (prm_p != NULL) {
        prm = *prm_p;
    }

    if (event_manager.getIsAddvance(i_staffID)) {
        switch (prm) {
        case 1:
            mActorMngr[0].entry(daPy_getPlayerActorClass());
            setLookMode(LOOK_PLAYER);
            angle = player->current.angle;
            angle.y = fopAcM_searchPlayerAngleY(this) + 0x8000;
            player->setPlayerPosAndAngle(&player->current.pos, &angle);
            dComIfGp_getEvent().setPtT(this);
            initTalk(8, NULL);
            mIsLeading = true;
            break;
        }
    }

    switch (prm) {
    case 1:
        facing_player = false;
        if (mCurAngle.y == fopAcM_searchPlayerAngleY(this)) {
            facing_player = true;
        } else {
            if (step(fopAcM_searchPlayerAngleY(this), -1, -1, 15)) {
                setExpression(EXPR_NONE, -1.0f);
                setMotion(MOT_WAIT_GT_A, -1.0f, false);
                mTurnMode = 0;
            }
        }
        
        if (facing_player && talkProc(NULL, true, NULL)) {
            setLookMode(LOOK_NONE);
            ret = true;
        }
        break;

    default:
        ret = true;
        break;
    }

    return ret;
}

BOOL daNpc_zrZ_c::ECut_restoreLink(int i_staffID) {
    dEvent_manager_c& event_manager = dComIfGp_getEventManager();
    BOOL ret = false;
    int prm = -1;
    int* prm_p = dComIfGp_evmng_getMyIntegerP(i_staffID, "prm");
    if (prm_p != NULL) {
        prm = *prm_p;
    }

    if (event_manager.getIsAddvance(i_staffID)) {
        switch (prm) {
        case 0:
            pullbackPlayer(daNpc_zrZ_Param_c::m.mRestoreDst - 500.0f);
            setAngle(fopAcM_searchPlayerAngleY(this));
            break;

        case 1:
            initTalk(0xd, NULL);
            break;
        }
    }

    switch (prm) {
    case 0:
        ret = true;
        break;

    case 1:
        if (talkProc(NULL, true, NULL)) {
            ret = true;
        }
        break;

    default:
        ret = true;
        break;
    }

    return ret;
}

BOOL daNpc_zrZ_c::ECut_clothesGet(int i_staffID) {
    dEvent_manager_c& event_manager = dComIfGp_getEventManager();
    BOOL ret = false;
    int prm = -1;
    int item_no;
    int* prm_p = dComIfGp_evmng_getMyIntegerP(i_staffID, "prm");
    if (prm_p != NULL) {
        prm = *prm_p;
    }

    if (event_manager.getIsAddvance(i_staffID)) {
        switch (prm) {
        case 0:
            daNpcF_offTmpBit(0xe);
            daNpcF_offTmpBit(0xf);
            setLookMode(LOOK_NONE);
            lightReady();
            break;

        case 1:
            break;

        case 10:
            initTalk(9, NULL);
            if (mpGravestoneActor != NULL) {
                dComIfGp_getEvent().setPtT(mpGravestoneActor);
                dComIfGp_getEvent().setPt2(mpGravestoneActor);
            }
            break;

        case 11:
            break;

        case 20:
            initTalk(9, NULL);
            break;

        case 30:
            mEventTimer = 50;
            if (mpGravestoneActor != NULL) {
                mpGravestoneActor->speedF = -2.0f;
                mpGravestoneActor->mTimer = 50;
            }
            mAllcolRatio = 1.0f;
            mUseLightEffect = true;
            break;

        case 40:
            mEventTimer = 0;
            break;

        case 50:
            if (mpClothActor != NULL) {
                dComIfGp_getEvent().setPtT(mpClothActor);
            }
            mEventTimer = 20;
            break;

        case 60:
            if (mpClothActor != NULL) {
                fopAcM_delete(mpClothActor);
            }
            item_no = 0;
            if (mFlow.getEventId(&item_no) == 1) {
                mItemID = fopAcM_createItemForPresentDemo(&current.pos, item_no,
                                                          0, -1, -1, NULL, NULL);
            }
            break;

        case 61:
            mEventTimer = 30;
            break;

        case 70:
            dComIfGp_getEvent().setPtT(this);
            break;

        case 71:
            mEventTimer = 30;
            setLookMode(LOOK_NONE);
            break;

        case 80:
            initTalk(9, NULL);
            if (mpGravestoneActor != NULL) {
                mpGravestoneActor->current.pos = mpGravestoneActor->home.pos;
            }
            break;

        case 90:
            mEventTimer = 0x78;
            Z2GetAudioMgr()->bgmStop(90, 0);
            break;
        }
    }

    switch (prm) {
    case 0:
        ret = true;
        break;

    case 10:
        if (talkProc(NULL, true, NULL)) {
            ret = true;
        }
        break;

    case 11:
        ret = true;
        break;

    case 20:
        if (mCurAngle.y != fopAcM_searchPlayerAngleY(this)) {
            if (step(fopAcM_searchPlayerAngleY(this), -1, -1, 15)) {
                mTurnMode = 0;
            }
        } else {
            if (talkProc(NULL, true, NULL)) {
                ret = true;
            }
        }
        break;

    case 30:
        if (cLib_calcTimer(&mEventTimer) == 0) {
            ret = true;
        } else {
            mAllcolRatio = mEventTimer * 0.45f / 50.0f + 0.55f;
            dKy_set_allcol_ratio(mAllcolRatio);
        }
        break;

    case 40:
        if (cLib_calcTimer(&mEventTimer) == 0) {
            ret = true;
        }
        break;

    case 50:
        if (cLib_calcTimer(&mEventTimer) == 0) {
            ret = true;
        }
        break;

    case 60:
        ret = true;
        break;

    case 61:
        mAllcolRatio = (30 - mEventTimer) * 0.45f / 30.0f + 0.55f;
        if (cLib_calcTimer(&mEventTimer) == 0) {
            mUseLightEffect = false;
            dKy_efplight_cut(&mLight);
        }
        ret = true;
        break;

    case 70:
        ret = true;
        break;

    case 71:
        if (cLib_calcTimer(&mEventTimer) == 0) {
            ret = true;
        }
        break;

    case 80:
        if (talkProc(NULL, true, NULL)) {
            ret = true;
        }
        break;

    case 90:
        if (cLib_calcTimer(&mEventTimer) == 0) {
            ret = true;
            tevStr.TevColor.a = 0;
        } else if (mEventTimer > 0) {
            tevStr.TevColor.a = (u8)(mEventTimer / 120.0f * 255.0f);
        } else {
            tevStr.TevColor.a = 0;
        }
        break;

    default:
        ret = true;
        break;
    }

    if (mUseLightEffect) {
        dKy_efplight_set(&mLight);
        lightPowerCalc(prm);
        lightColorProc();
        mLightEffectFrame++;
    }

    if (mAllcolRatio < 1.0f) {
        dKy_set_allcol_ratio(mAllcolRatio);
        cXyz pos(21105.0f, 750.0f, -50.0f);
        GXColor color = {0xff, 0xc3, 0x8a, 0xff};
        dKy_BossLight_set(&pos, &color, (1.0f - mAllcolRatio) * 1.5f, 0);
    }

    return ret;
}

BOOL daNpc_zrZ_c::ECut_getAfter(int i_staffID) {
    dEvent_manager_c& event_manager = dComIfGp_getEventManager();
    BOOL ret = false;
    int prm = -1;
    fopAc_ac_c* gravestone;
    int* prm_p = dComIfGp_evmng_getMyIntegerP(i_staffID, "prm");
    if (prm_p != NULL) {
        prm = *prm_p;
    }

    if (event_manager.getIsAddvance(i_staffID)) {
        switch (prm) {
        case 0:
            break;

        case 70:
            mEventTimer = 30;
            setLookMode(LOOK_NONE);
            break;

        case 80:
            initTalk(9, NULL);
            gravestone = (fopAc_ac_c*)fpcM_Search(s_sub, this);
            if (gravestone != NULL) {
                gravestone->current.pos = gravestone->home.pos;
            }
            break;

        case 90:
            mEventTimer = 120;
            break;
        }
    }

    switch (prm) {
    case 0:
        daPy_getPlayerActorClass()->changeDemoMoveAngle(fopAcM_searchPlayerAngleY(this) + 0x8000);
        ret = true;
        break;

    case 70:
        if (cLib_calcTimer(&mEventTimer) == 0) {
            ret = true;
        }
        break;

    case 80:
        if (talkProc(NULL, true, NULL)) {
            ret = true;
        }
        break;

    case 90:
        if (cLib_calcTimer(&mEventTimer) == 0) {
            ret = true;
            tevStr.TevColor.a = 0;
        } else if (mEventTimer > 0) {
            tevStr.TevColor.a = (u8)(mEventTimer / 120.0f * 255.0f);
        } else {
            tevStr.TevColor.a = 0;
        }
        break;

    default:
        ret = true;
        break;
    }

    return ret;
}

BOOL daNpc_zrZ_c::ECut_sealRelease(int i_staffID) {
    dEvent_manager_c& event_manager = dComIfGp_getEventManager();
    BOOL ret = false;
    int prm = -1;
    int* prm_p = dComIfGp_evmng_getMyIntegerP(i_staffID, "prm");
    if (prm_p != NULL) {
        prm = *prm_p;
    }

    if (event_manager.getIsAddvance(i_staffID)) {
        switch (prm) {
        case 0:
            mEventTimer = 20;
            mSealReleased = true;
            dComIfGp_getEvent().setSkipProc(this, dEv_defaultSkipProc, 0);
            mpRockActor->setDemoStart();
            break;

        case 10:
            break;

        case 20:
            if (mpRockActor != NULL) {
                mpRockActor->mBrkAnm.setPlaySpeed(1.0f);
                Z2GetAudioMgr()->seStart(Z2SE_OBJ_ZORA_STN_VANISH, &mpRockActor->current.pos,
                                         0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
            }
            break;
        }
    }

    switch (prm) {
    case 0:
        if (cLib_calcTimer(&mEventTimer) == 0) {
            ret = true;
        }
        break;

    case 10:
        if (tevStr.TevColor.a <= 0x1e) {
            ret = true;
        }
        break;

    case 20:
        if (mpRockActor->mBrkAnm.isStop()) {
            ret = true;
        }
        break;

    default:
        ret = true;
        break;
    }

    if (prm >= 10) {
        cXyz pos = mPath.getPntPos(mPath.getIdx());
        if (!mPath.chkPassedDst(current.pos)) {
            cLib_chaseS(&tevStr.TevColor.a, 8, 8);
            cLib_addCalc2(&mSpeed, daNpc_zrZ_Param_c::m.mMaxSpeed, 0.1f, 1.0f);
            s16 ang_y = cLib_targetAngleY(&current.pos, &pos);
            s16 ang_x = cLib_targetAngleX(&pos, &current.pos);
            cLib_addCalcAngleS2(&mMoveAngle.y, ang_y, 2, 0x800);
            cLib_addCalcAngleS2(&mMoveAngle.x, ang_x, 2, 0x800);
        } else {
            mSpeed = 0.0f;
            cLib_chaseS(&tevStr.TevColor.a, 0xff, 8);
            mIsMoving = false;
        }
    } else {
        cLib_chaseF(&mSpeed, 0.0f, 3.0f);
        mIsMoving = false;
    }

    cXyz move_speed(0.0f, 0.0f, mSpeed);
    mDoMtx_stack_c::ZXYrotS(mMoveAngle);
    mDoMtx_stack_c::multVec(&move_speed, &move_speed);
    current.pos += move_speed;
    cLib_addCalcAngleS2(&mCurAngle.y, -0x4000, 2, 0x800);
    setAngle(mCurAngle.y);
    return ret;
}

BOOL daNpc_zrZ_c::ECut_srSkip(int i_staffID) {
    dEvent_manager_c& event_manager = dComIfGp_getEventManager();
    int prm = -1;
    int* prm_p = dComIfGp_evmng_getMyIntegerP(i_staffID, "prm");
    if (prm_p != NULL) {
        prm = *prm_p;
    }

    if (event_manager.getIsAddvance(i_staffID)) {
        switch (prm) {
        case 10:
            mSpeed = 0.0f;
            current.pos = mPath.getPntPos(mPath.getIdx());
            old.pos = current.pos;
            setAngle(fopAcM_searchPlayerAngleY(this));
            tevStr.TevColor.a = 0xff;
            mIsMoving = false;
            if (mpRockActor != NULL) {
                fopAcM_delete(mpRockActor);
                mpRockActor = NULL;
            }
            Z2GetAudioMgr()->seStop(Z2SE_OBJ_ZORA_STN_VANISH, 0);
            mSealReleased = true;
            break;
        }
    }

    return true;
}

void daNpc_zrZ_c::pullbackPlayer(f32 param_0) {
    cXyz pos = mPath.getPntPos(mPath.getBeforeIdx());
    pos.y += 1000.0f;
    if (fopAcM_gc_c::gndCheck(&pos)) {
        pos.y = fopAcM_gc_c::getGroundY();
    }
    s16 angle_y = cLib_targetAngleY(&pos, &current.pos);
    daPy_getPlayerActorClass()->setPlayerPosAndAngle(&pos, angle_y, 0);
}

BOOL daNpc_zrZ_c::wait(void* param_0) {
    switch (mMode) {
    case 0:
        setExpression(EXPR_NONE, -1.0f);
        setMotion(MOT_WAIT_GT_A, -1.0f, false);
        setLookMode(LOOK_NONE);
        mTurnMode = 0;
        mMode = 2;
        // fallthrough

    case 2:
        if (!mIsLeading && daPy_getPlayerActorClass()->current.pos.z >= -120.0f
                          && daPy_getPlayerActorClass()->checkSwimUp()) {
            mOrderEvtNo = EVT_HELP_PRINCE;
            mActorMngr[0].entry(daPy_getPlayerActorClass());
            setLookMode(LOOK_PLAYER);
            s16 angle_y = fopAcM_searchPlayerAngleY(this);
            shape_angle.y = angle_y;
            mCurAngle.y = angle_y;
            current.angle.y = angle_y;
            mOrderNewEvt = true;
        } else if (!mIsDamaged) {
            BOOL player_attn = mActorMngr[0].getActorP() != NULL;
            if (chkFindPlayer2(player_attn, shape_angle.y)) {
                if (!player_attn) {
                    mActorMngr[0].entry(daPy_getPlayerActorClass());
                    mTurnMode = 0;
                }
            } else {
                if (player_attn) {
                    mActorMngr[0].remove();
                    mTurnMode = 0;
                }
            }

            if (mActorMngr[0].getActorP() != NULL) {
                setLookMode(LOOK_PLAYER);
            } else {
                setLookMode(LOOK_NONE);
                if (home.angle.y != mCurAngle.y && step(home.angle.y, -1, -1, 15)) {
                    mMode = 0;
                }
            }

            if (home.angle.y == mCurAngle.y) {
                BOOL player_attn = mActorMngr[0].getActorP() != NULL;
                fopAc_ac_c* attn_actor = getAttnActorP(
                    player_attn, srchAttnActor1, daNpc_zrZ_Param_c::m.mAttnRadius,
                    daNpc_zrZ_Param_c::m.mAttnUpperY, daNpc_zrZ_Param_c::m.mAttnLowerY,
                    daNpc_zrZ_Param_c::m.mAttnFovY, shape_angle.y, 120, true
                );
                if (attn_actor != NULL) {
                    mActorMngr[1].entry(attn_actor);
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

    return true;
}

BOOL daNpc_zrZ_c::comeHere(void* param_0) {
    cXyz player_pos = daPy_getPlayerActorClass()->current.pos;
    cXyz pnt_pos = mPath.getPntPos(mPath.getIdx());

    switch (mMode) {
    case 0:
        if (mIsLeading) {
            setExpression(EXPR_NONE, -1.0f);
            setMotion(MOT_LEAD, -1.0f, false);
        } else {
            setExpression(EXPR_NONE, -1.0f);
            setMotion(MOT_WAIT_GT_A, -1.0f, false);
        }

        setLookMode(LOOK_NONE);
        mTurnMode = 0;
        mMode = 2;

        if (mPath.getIdx() == mPath.getBeforeIdx()) {
            mPath.setNextIdxDst(current.pos);
        }
        // fallthrough

    case 2:
        cLib_addCalcAngleS2(&mCurAngle.y, fopAcM_searchPlayerAngleY(this), 2, 0x800);
        setAngle(mCurAngle.y);

        if (!mIsLeading) {
            if ((pnt_pos - player_pos).absXZ() < daNpc_zrZ_Param_c::m.mFollowDst) {
                mActorMngr[0].entry(daPy_getPlayerActorClass());
                setLookMode(LOOK_PLAYER);
                setAngle(fopAcM_searchPlayerAngleY(this));
                setMotion(MOT_LEAD, -1.0f, false);
                mIsLeading = true;
            } else if (!dComIfGs_isSwitch(mSwitch1, fopAcM_GetRoomNo(this))
                && (current.pos - player_pos).absXZ() > daNpc_zrZ_Param_c::m.mRestoreDst)
            {
                mOrderEvtNo = EVT_RESTORE_LINK;
                setAngle(fopAcM_searchPlayerAngleY(this));
            }
        } else {
            if (mPath.chkPassedDst(current.pos)) {
                if ((pnt_pos - player_pos).absXZ() < daNpc_zrZ_Param_c::m.mFollowDst) {
                    mPath.setNextIdxDst(current.pos);
                    pnt_pos = mPath.getPntPos(mPath.getIdx());
                    mIsMoving = true;
                } else {
                    mIsMoving = false;
                }
            } else {
                mIsMoving = true;
            }

            if (mPath.getIdx() == mPath.getNextIdx()) {
                if (!dComIfGs_isSwitch(mSwitch1, fopAcM_GetRoomNo(this))) {
                    dComIfGs_onSwitch(mSwitch1, fopAcM_GetRoomNo(this));
                }
                cLib_chaseS(&tevStr.TevColor.a, 0, 4);
                if (tevStr.TevColor.a == 0) {
                    fopAcM_delete(this);
                }
            }

            if (mIsMoving) {
                cLib_addCalc2(&mSpeed, daNpc_zrZ_Param_c::m.mMaxSpeed, 0.1f, 1.0f);
                s16 angle_y = cLib_targetAngleY(&current.pos, &pnt_pos);
                s16 angle_x = cLib_targetAngleX(&pnt_pos, &current.pos);
                cLib_addCalcAngleS2(&mMoveAngle.y, angle_y, 2, 0x800);
                cLib_addCalcAngleS2(&mMoveAngle.x, angle_x, 2, 0x800);
            } else {
                cLib_chaseF(&mSpeed, 0.0f, 3.0f);
            }

            cXyz move_speed(0.0f, 0.0f, mSpeed);
            mDoMtx_stack_c::ZXYrotS(mMoveAngle);
            mDoMtx_stack_c::multVec(&move_speed, &move_speed);
            current.pos += move_speed;

            if (!dComIfGs_isSwitch(mSwitch1, fopAcM_GetRoomNo(this))
                && (current.pos - player_pos).absXZ() > daNpc_zrZ_Param_c::m.mRestoreDst)
            {
                mOrderEvtNo = EVT_RESTORE_LINK;
                setAngle(fopAcM_searchPlayerAngleY(this));
            }
        }
        break;

    case 3:
        mSpeed = 0.0f;
        break;
    }

    return true;
}

BOOL daNpc_zrZ_c::comeHere2(void* param_0) {
    switch (mMode) {
    case 0: {
        setExpression(EXPR_NONE, -1.0f);
        setMotion(MOT_LEAD, -1.0f, false);
        mTurnMode = 0;
        mMode = 2;
        cXyz pnt_pos = mPath.getPntPos(mPath.getIdx());
        current.angle.y = cLib_targetAngleY(&current.pos, &pnt_pos);
        // fallthrough
    }

    case 2:
        if (mpGravestoneActor == NULL) {
            mpGravestoneActor = (daGraveStone_c*)fpcM_Search(s_sub, this);
        }
        if (mpClothActor == NULL) {
            mpClothActor = (fopAc_ac_c*)fpcM_Search(s_subCloth, this);
        }
        if (mpRockActor == NULL) {
            mpRockActor = (daObjZraRock_c*)fpcM_Search(s_subRock, this);
        }

        cLib_addCalcAngleS2(&mCurAngle.y, fopAcM_searchPlayerAngleY(this), 2, 0x800);
        setAngle(mCurAngle.y);

        if (!mMusicSet && dComIfGs_isSwitch(mSwitch3, fopAcM_GetRoomNo(this))) {
            mMusicSet = true;
            Z2GetAudioMgr()->changeBgmStatus(1);
        }

        if (!mIsLeading) {
            setLookMode(LOOK_PLAYER);
            cXyz player_pos = daPy_getPlayerActorClass()->current.pos;
            cXyz pnt_pos = mPath.getPntPos(mPath.getIdx());

            if (mPath.chkPassedDst(current.pos)) {
                if (mPath.getIdx() == mPath.getNextIdx()) {
                    mIsMoving = false;
                    setMotion(MOT_WAIT_GT_A, -1.0f, false);
                    if ((current.pos - player_pos).absXZ() < daNpc_zrZ_Param_c::m.mClothesGetDst
                        && player_pos.y > 450.0f)
                    {
                        mClothesObtained = true;
                        mOrderEvtNo = EVT_CLOTHES_GET;
                    }
                } else if ((pnt_pos - player_pos).absXZ() < daNpc_zrZ_Param_c::m.mFollowDst) {
                    mPath.setNextIdxDst(current.pos);
                    pnt_pos = mPath.getPntPos(mPath.getIdx());
                    mIsMoving = true;
                } else {
                    mIsMoving = false;
                }
            } else {
                mIsMoving = true;
            }

            if (mIsMoving) {
                if (mPath.getArg0() == 0) {
                    if (!mSealReleased) {
                        mOrderEvtNo = EVT_SEAL_RELEASE;
                        break;
                    } else {
                        cLib_chaseS(&tevStr.TevColor.a, 0x14, 8);
                    }
                } else {
                    cLib_chaseS(&tevStr.TevColor.a, 0xff, 8);
                }

                cLib_addCalc2(&mSpeed, daNpc_zrZ_Param_c::m.mMaxSpeed, 0.1f, 1.0f);
                s16 angle_y = cLib_targetAngleY(&current.pos, &pnt_pos);
                s16 angle_x = cLib_targetAngleX(&pnt_pos, &current.pos);
                cLib_addCalcAngleS2(&mMoveAngle.y, angle_y, 2, 0x800);
                cLib_addCalcAngleS2(&mMoveAngle.x, angle_x, 2, 0x800);
            } else {
                cLib_chaseF(&mSpeed, 0.0f, 3.0f);
                cLib_chaseS(&tevStr.TevColor.a, 0xff, 8);
            }

            cXyz move_speed(0.0f, 0.0f, mSpeed);
            mDoMtx_stack_c::ZXYrotS(mMoveAngle);
            mDoMtx_stack_c::multVec(&move_speed, &move_speed);
            current.pos += move_speed;

            if ((current.pos - player_pos).absXZ() > daNpc_zrZ_Param_c::m.mRestoreDst) {
                mOrderEvtNo = EVT_RESTORE_LINK;
                setAngle(fopAcM_searchPlayerAngleY(this));
            }
        }

        break;

    case 3:
        mSpeed = 0.0f;
        break;
    }

    return true;
}

BOOL daNpc_zrZ_c::talk(void* param_0) {
    BOOL ret = false;
    BOOL talk = false;

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
            talk = true;
        } else {
            setLookMode(LOOK_PLAYER_TALK);
            mActorMngr[0].entry(daPy_getPlayerActorClass());
            if (mCurAngle.y == fopAcM_searchPlayerAngleY(this)) {
                talk = true;
            } else if (step(fopAcM_searchPlayerAngleY(this), -1, -1, 15)) {
                setExpression(EXPR_NONE, -1.0f);
                setMotion(MOT_WAIT_GT_A, -1.0f, false);
                mTurnMode = 0;
            }
        }

        if (talk && talkProc(NULL, true, NULL)) {
            ret = true;
        }

        if (ret) {
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

    return ret;
}

BOOL daNpc_zrZ_c::test(void* param_0) {
    switch (mMode) {
    case 0:
        speedF = 0.0f;
        speed.setall(0.0f);
        mMode = 2;
        // fallthrough

    case 2:
        if (daNpc_zrZ_Param_c::m.mTestExpression != mExpression) {
            setExpression(daNpc_zrZ_Param_c::m.mTestExpression, daNpc_zrZ_Param_c::m.mMorfFrames);
        }
        setMotion(daNpc_zrZ_Param_c::m.mTestMotion, daNpc_zrZ_Param_c::m.mMorfFrames, false);
        setLookMode(daNpc_zrZ_Param_c::m.mTestLookMode);
        mOrderEvtNo = EVT_NONE;
        attention_info.flags = 0;
        break;

    case 3:
        break;
    }

    return true;
}

void daNpc_zrZ_c::himoCalc() {
    cXyz vec1 = current.pos;
    vec1.y += 470.0f;
    cXyz vec2 = mLimbCalcPos - vec1;
    vec2.normalize();
    vec2 = vec2 * daNpc_zrZ_Param_c::m.field_0x80;
    cXyz vec3(0.0f, -daNpc_zrZ_Param_c::m.field_0x80, 0.0f);
    cLib_addCalcPos2(&vec2, vec3, 0.2f, 25.0f);
    mLimbCalcPos = vec2 + vec1;
    
    mDoMtx_stack_c::push();
    mDoMtx_stack_c::copy(mAnm_p->getModel()->getAnmMtx(29));
    mDoMtx_stack_c::inverse();
    mDoMtx_stack_c::multVec(&mLimbCalcPos, &mLimbCalcRelPos);
    mDoMtx_stack_c::pop();

    mLimbAngle.x = cM_atan2s(mLimbCalcRelPos.y, mLimbCalcRelPos.x);
    mLimbAngle.y = 0;
    mLimbAngle.z = cM_atan2s(mLimbCalcRelPos.z, mLimbCalcRelPos.x);
}

static cPhs__Step daNpc_zrZ_Create(void* i_this) {
    return static_cast<daNpc_zrZ_c*>(i_this)->create();
}

static int daNpc_zrZ_Delete(void* i_this) {
    return static_cast<daNpc_zrZ_c*>(i_this)->Delete();
}

static int daNpc_zrZ_Execute(void* i_this) {
    return static_cast<daNpc_zrZ_c*>(i_this)->Execute();
}

static int daNpc_zrZ_Draw(void* i_this) {
    return static_cast<daNpc_zrZ_c*>(i_this)->Draw();
}

static int daNpc_zrZ_IsDelete(void* i_this) {
    return 1;
}

AUDIO_INSTANCES;

static actor_method_class daNpc_zrZ_MethodTable = {
    (process_method_func)daNpc_zrZ_Create,
    (process_method_func)daNpc_zrZ_Delete,
    (process_method_func)daNpc_zrZ_Execute,
    (process_method_func)daNpc_zrZ_IsDelete,
    (process_method_func)daNpc_zrZ_Draw,
};

extern actor_process_profile_definition g_profile_NPC_ZRZ = {
  fpcLy_CURRENT_e,        // mLayerID
  7,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_NPC_ZRZ,           // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(daNpc_zrZ_c),    // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  390,                    // mPriority
  &daNpc_zrZ_MethodTable, // sub_method
  0x08044100,             // mStatus
  fopAc_NPC_e,            // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};
