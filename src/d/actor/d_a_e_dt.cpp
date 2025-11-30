/**
 * @file d_a_e_dt.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_e_dt.h"
#include "JSystem/J3DGraphBase/J3DMaterial.h"
#include "SSystem/SComponent/c_math.h"
#include "Z2AudioLib/Z2Instances.h"
#include "c/c_damagereaction.h"
#include "d/actor/d_a_e_ot.h"
#include "d/actor/d_a_obj_amiShutter.h"
#include "d/actor/d_a_player.h"
#include "d/d_camera.h"
#include "d/d_com_inf_game.h"
#include "d/d_bomb.h"
#include "f_op/f_op_actor_mng.h"
#include "f_op/f_op_camera_mng.h"
#include "m_Do/m_Do_graphic.h"
#include "m_Do/m_Do_lib.h"

class daE_DT_HIO_c {
public:
    daE_DT_HIO_c();
    virtual ~daE_DT_HIO_c() {}

    /* 0x04 */ s8 field_0x4;
    /* 0x08 */ f32 mScale;
    /* 0x0C */ f32 mWalkSpeed;
    /* 0x10 */ f32 field_0x10;
    /* 0x14 */ f32 field_0x14;
    /* 0x18 */ f32 field_0x18;
    /* 0x1C */ f32 mStunTimer;
    /* 0x20 */ f32 mPressTimer;
    /* 0x24 */ f32 mMaxFallSpeed;
    /* 0x28 */ f32 field_0x28;
    /* 0x2C */ f32 field_0x2c;
};

enum Action {
    /* 0x0 */ ACT_WAIT,
    /* 0x1 */ ACT_CRY,
    /* 0x2 */ ACT_SHAKE,
    /* 0x3 */ ACT_WALK,
    /* 0x4 */ ACT_JUMP,
    /* 0x5 */ ACT_DOWN,
    /* 0x6 */ ACT_SEARCH,
    /* 0x7 */ ACT_DAMAGE,
    /* 0x8 */ ACT_BOMB_DAMAGE,
    /* 0x9 */ ACT_DEATH,
    /* 0xA */ ACT_OPENING,
};

enum Animation {
    /* 0x04 */ ANM_CROAK = 4,
    /* 0x05 */ ANM_CROAK_DAMAGE,
    /* 0x06 */ ANM_DEAD,
    /* 0x07 */ ANM_DOWN_DAMAGE,
    /* 0x08 */ ANM_DOWN_DAMAGE_2,
    /* 0x09 */ ANM_DOWN_RETURN,
    /* 0x0A */ ANM_DOWN_RETURN_2,
    /* 0x0B */ ANM_DOWN_WAIT,
    /* 0x0C */ ANM_JUMP_END,
    /* 0x0D */ ANM_JUMP_START,
    /* 0x0E */ ANM_JUMP_TOP,
    /* 0x0F */ ANM_KYORO_2,
    /* 0x10 */ ANM_OP_DEMO,
    /* 0x11 */ ANM_OP_DEMO_FALL,
    /* 0x12 */ ANM_OP_DEMO_SWING,
    /* 0x13 */ ANM_OP_DEMO_WAIT,
    /* 0x14 */ ANM_SMALL_DAMAGE,
    /* 0x15 */ ANM_STEP,
    /* 0x16 */ ANM_SWING,
    /* 0x17 */ ANM_WAIT_01,
    /* 0x18 */ ANM_WALK_01,
    /* 0x19 */ ANM_WALK_02,
};

enum Joint {
    /* 0x00 */ JNT_WORLD_ROOT,
    /* 0x01 */ JNT_BACKBONE_1,
    /* 0x02 */ JNT_BACKBONE_2,
    /* 0x03 */ JNT_ARM_L1,
    /* 0x04 */ JNT_ARM_L2,
    /* 0x05 */ JNT_HAND_L,
    /* 0x06 */ JNT_FINGER_L,
    /* 0x07 */ JNT_ARM_R1,
    /* 0x08 */ JNT_ARM_R2,
    /* 0x09 */ JNT_HAND_R,
    /* 0x0A */ JNT_FINGER_R,
    /* 0x0B */ JNT_HEAD,
    /* 0x0C */ JNT_ANTENNA_L1,
    /* 0x0D */ JNT_ANTENNA_L2,
    /* 0x0E */ JNT_ANTENNA_L3,
    /* 0x0F */ JNT_ANTENNA_L4,
    /* 0x10 */ JNT_ANTENNA_L5,
    /* 0x11 */ JNT_ANTENNA_R1,
    /* 0x12 */ JNT_ANTENNA_R2,
    /* 0x13 */ JNT_ANTENNA_R3,
    /* 0x14 */ JNT_ANTENNA_R4,
    /* 0x15 */ JNT_ANTENNA_R5,
    /* 0x16 */ JNT_CHEEK,
    /* 0x17 */ JNT_CHIN_1,
    /* 0x18 */ JNT_CHIN_2,
    /* 0x19 */ JNT_THROAT_1,
    /* 0x1A */ JNT_TONGUE_1,
    /* 0x1B */ JNT_TONGUE_2,
    /* 0x1C */ JNT_TONGUE_3,
    /* 0x1D */ JNT_TONGUE_4,
    /* 0x1E */ JNT_TONGUE_5,
    /* 0x1F */ JNT_BELLY_1,
    /* 0x20 */ JNT_WAIST,
    /* 0x21 */ JNT_LEG_L1,
    /* 0x22 */ JNT_LEG_L2,
    /* 0x23 */ JNT_FOOT_L1,
    /* 0x24 */ JNT_FOOT_L2,
    /* 0x25 */ JNT_LEG_R1,
    /* 0x26 */ JNT_LEG_R2,
    /* 0x27 */ JNT_FOOT_R1,
    /* 0x28 */ JNT_FOOT_R2,
    /* 0x29 */ JNT_TAIL_1,
    /* 0x2A */ JNT_TAIL_2,
};

f32 E_DT_n::eDt_ShakeFrame[10] = {
    20.0f, 22.0f, 24.0f, 26.0f, 28.0f, 30.0f, 32.0f, 34.0f, 36.0f, 38.0f,
};

f32 E_DT_n::eDt_ShakeFrameDemo[10] = {
    310.0f, 312.0f, 314.0f, 316.0f, 318.0f, 320.0f, 322.0f, 324.0f, 326.0f, 328.0f,
};

dCcD_SrcSph E_DT_n::cc_dt_body_src = {
    {
        {0x0, {{AT_TYPE_CSTATUE_SWING, 0x1, 0x0}, {0xd8fbfdff, 0x43}, 0x75}}, // mObj
        {dCcD_SE_METAL, 0x0, 0x0, 0x0, 0x0}, // mGObjAt
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x5}, // mGObjTg
        {0x0}, // mGObjCo
    }, // mObjInf
    {
        {{0.0f, 0.0f, 0.0f}, 40.0f} // mSph
    } // mSphAttr
};

dCcD_SrcSph E_DT_n::cc_dt_tongue_src = {
    {
        {0x0, {{AT_TYPE_CSTATUE_SWING, 0x1, 0x0}, {0xd8fbfdff, 0x43}, 0x75}}, // mObj
        {dCcD_SE_METAL, 0x0, 0x0, 0x0, 0x0}, // mGObjAt
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x2}, // mGObjTg
        {0x0}, // mGObjCo
    }, // mObjInf
    {
        {{0.0f, 0.0f, 0.0f}, 40.0f} // mSph
    } // mSphAttr
};

/* 806B629D 0003+00 data_806B629D None */
static bool hioInit;

static daE_DT_HIO_c l_HIO;

daE_DT_HIO_c::daE_DT_HIO_c() {
    field_0x4 = -1;
    mScale = 1.5f;
    mWalkSpeed = 8.0f;
    field_0x10 = 170.0f;
    field_0x14 = 1536.0f;
    field_0x2c = 25.0f;
    field_0x18 = 3.0f;
    mStunTimer = 300.0f;
    mPressTimer = 120.0f;
    mMaxFallSpeed = -55.0f;
    field_0x28 = 105.0f;
}

int daE_DT_c::ctrlJoint(J3DJoint* i_joint, J3DModel* i_model) {
    u16 jnt_no = i_joint->getJntNo();
    mDoMtx_stack_c::copy(i_model->getAnmMtx(jnt_no));
    f32 scl = field_0x748 * cM_ssin(field_0x74c) + 1.0f;
    s16 ang1 = field_0x74e * cM_ssin(field_0x750);
    s16 ang2 = field_0x752 * cM_ssin(field_0x754);

    switch (jnt_no) {
    case JNT_BACKBONE_1:
        mDoMtx_stack_c::scaleM(1.0f, scl, 1.0f);
        mDoMtx_stack_c::scaleM(1.0f, 1.0f, scl);
        break;
    case JNT_BACKBONE_2:
        mDoMtx_stack_c::scaleM(1.0f, 1.0f / scl, 1.0f);
        mDoMtx_stack_c::scaleM(1.0f, 1.0f, 1.0f / scl);
        break;
    case JNT_LEG_L1:
        mDoMtx_stack_c::YrotM(ang1);
        break;
    case JNT_LEG_L2:
        mDoMtx_stack_c::YrotM(-ang1);
        break;
    case JNT_LEG_R1:
        mDoMtx_stack_c::YrotM(ang2);
        break;
    case JNT_LEG_R2:
        mDoMtx_stack_c::YrotM(-ang2);
        break;
    case JNT_TONGUE_4:
        mDoMtx_stack_c::ZrotM(field_0x734);
        break;
    case JNT_HEAD:
        mDoMtx_stack_c::YrotM(field_0x744);
        mDoMtx_stack_c::ZrotM(field_0x742);
        break;
    }

    i_model->setAnmMtx(jnt_no, mDoMtx_stack_c::get());
    MTXCopy(mDoMtx_stack_c::get(), J3DSys::mCurrentMtx);
    return 1;
}

int daE_DT_c::JointCallBack(J3DJoint* i_joint, int param_1) {
    if (param_1 == 0) {
        J3DModel* model = j3dSys.getModel();
        daE_DT_c* _this = (daE_DT_c*)model->getUserArea();
        if (_this != NULL) {
            _this->ctrlJoint(i_joint, model);
        }
    }
    return 1;
}

int daE_DT_c::ctrlJointTuba(J3DJoint* i_joint, J3DModel* i_model) {
    u16 jnt_no = i_joint->getJntNo();
    mDoMtx_stack_c::copy(i_model->getAnmMtx(jnt_no));

    switch (jnt_no) {
    case 0:
        break;

    case 1:
        mDoMtx_stack_c::YrotM(mSpitAngle[1].x - mSpitAngle[0].x);
        mDoMtx_stack_c::ZrotM(mSpitAngle[1].z);
        mDoMtx_stack_c::scaleM(mSpitScaleX, 1.0f / mSpitScale[0], 1.0f / mSpitScale[0]);
        mDoMtx_stack_c::scaleM(mSpitScaleX, mSpitScale[1], mSpitScale[1]);
        break;

    case 2:
        mDoMtx_stack_c::YrotM(mSpitAngle[2].x - mSpitAngle[1].x);
        mDoMtx_stack_c::scaleM(1.0f, 1.0f / mSpitScale[1], 1.0f / mSpitScale[1]);
        mDoMtx_stack_c::scaleM(1.0f, mSpitScale[2], mSpitScale[2]);
        break;
    }

    i_model->setAnmMtx(jnt_no, mDoMtx_stack_c::get());
    MTXCopy(mDoMtx_stack_c::get(), J3DSys::mCurrentMtx);
    return 1;
}

int daE_DT_c::JointCallBackTuba(J3DJoint* i_joint, int param_1) {
    if (param_1 == 0) {
        J3DModel* model = j3dSys.getModel();
        daE_DT_c* _this = (daE_DT_c*)model->getUserArea();
        if (_this != NULL) {
            _this->ctrlJointTuba(i_joint, model);
        }
    }
    return 1;
}

int daE_DT_c::draw() {
    g_env_light.settingTevStruct(0, &current.pos, &tevStr);

    if (mChestMode != 0) {
        g_env_light.setLightTevColorType_MAJI(mpSpitModel, &tevStr);
        J3DModelData* model_data = mpSpitModel->getModelData();
        J3DMaterial* material0 = model_data->getMaterialNodePointer(0);
        material0->getTevColor(1)->r = 24.0f - mSpitFade[0] * 18.0f;
        material0->getTevColor(1)->g = 8.0f - mSpitFade[0] * 6.0f;
        material0->getTevColor(2)->r = 8.0f - mSpitFade[0] * 6.0f;
        material0->getTevColor(2)->g = 24.0f - mSpitFade[0] * 18.0f;
        material0->getTevKColor(3)->a = mSpitFade[0] * 255.0f;
        J3DMaterial* material1 = model_data->getMaterialNodePointer(1);
        material1->getTevColor(1)->r = 16.0f - mSpitFade[0] * 10.0f;
        material1->getTevColor(1)->g = 8.0f - mSpitFade[0] * 8.0f;
        material1->getTevColor(2)->r = 16.0f - mSpitFade[0] * 12.0f;
        material1->getTevColor(2)->g = 24.0f - mSpitFade[0] * 16.0f;
        if (mSpitFade[0] == 1.0f) {
            material1->setMaterialMode(1);
            material1->setZCompLoc(GX_TRUE);
            material1->getZMode()->setUpdateEnable(GX_TRUE);
            material1->getBlend()->setType(0);
        } else {
            material1->getZMode()->setUpdateEnable(GX_FALSE);
            material1->setZCompLoc(GX_FALSE);
            material1->setMaterialMode(4);
            material1->getBlend()->setType(1);
        }
        material1->getTevKColor(3)->a = mSpitFade[0] * 255.0f;
        fopAcM_setEffectMtx(this, model_data);
        mDoExt_modelUpdateDL(mpSpitModel);

        g_env_light.setLightTevColorType_MAJI(mpSpitBModel, &tevStr);
        model_data = mpSpitBModel->getModelData();
        for (u16 i = 0; i < model_data->getMaterialNum(); i++) {
            J3DMaterial* material = model_data->getMaterialNodePointer(i);
            material->getTevColor(0)->r = mSpitFade[i + 1] * -20.0f;
            material->getTevColor(0)->g = mSpitFade[i + 1] * -30.0f;
            material->getTevColor(0)->b = mSpitFade[i + 1] * -10.0f;
            material->getTevKColor(3)->a = mSpitFade[i + 1] * 255.0f;
        }
        fopAcM_setEffectMtx(this, model_data);
        mDoExt_modelUpdateDL(mpSpitBModel);
    }

    if (mDead == 2) {
        return 1;
    }

    J3DModel* model = mpMorf->getModel();
    g_env_light.setLightTevColorType_MAJI(model, &tevStr);
    if (field_0x72c) {
        J3DModelData* model_data = model->getModelData();
        for (u16 i = 0; i < model_data->getMaterialNum(); i++) {
            J3DMaterial* material = model_data->getMaterialNodePointer(i);
            material->getTevColor(0)->r = field_0x72c;
            material->getTevColor(0)->g = field_0x72c;
            material->getTevColor(0)->b = field_0x72c;
        }
    }
    fopAcM_setEffectMtx(this, model->getModelData());
    mpMorf->entryDL();

    cXyz pos;
    pos.set(current.pos.x, current.pos.y + 100.0f, current.pos.z);
    tevStr.mLightPosWorld = current.pos;
    tevStr.mLightPosWorld.y += 1000.0f;
    f32 tmp = 2000.0f - current.pos.y;
    if (tmp < 0.0f) {
        tmp = 0.0f;
    }
    tevStr.field_0x344 = l_HIO.field_0x18 * tmp / 2000.0f;
    if (tevStr.field_0x344 >= 1.0f) {
        tevStr.field_0x344 = 1.0f;
    }
    mShadowKey = dComIfGd_setShadow(mShadowKey, 0, model, &pos, 4000.0f, 0.0f, current.pos.y,
                                    mAcch.GetGroundH(), mAcch.m_gnd, &tevStr, 0, 1.0f,
                                    dDlst_shadowControl_c::getSimpleTex());
    
    cXyz unused = current.pos;
    unused.y += 100.0f;

    if (mChestMode != 0) {
        g_env_light.settingTevStruct(0x10, &mChestPos, &tevStr);
        g_env_light.setLightTevColorType_MAJI(mpChestModel, &tevStr);
        mDoExt_modelUpdateDL(mpChestModel);
    }

    if (mBlurMode == 1) {
        mDoGph_gInf_c::setBlureRate(mBlurRate);
        mDoGph_gInf_c::onBlure();
    } else if (mBlurMode == 2) {
        mDoGph_gInf_c::offBlure();
    }

    return 1;
}

static int daE_DT_Draw(daE_DT_c* i_this) {
    return i_this->draw();
}

void daE_DT_c::demo_skip(int param_0) {
    cDmr_SkipInfo = 1;
    dStage_changeScene(2, 0.0f, 0, fopAcM_GetRoomNo(this), 0, -1);
}

int daE_DT_c::DemoSkipCallBack(void* i_this, int param_1) {
    if (i_this != NULL) {
        static_cast<daE_DT_c*>(i_this)->demo_skip(param_1);
        return 1;
    } else {
        return 0;
    }
}

void daE_DT_c::setBck(int i_anm, u8 i_attr, f32 i_morf, f32 i_rate) {
    J3DAnmTransform* bck = (J3DAnmTransform*)dComIfG_getObjectRes("E_DT", i_anm);
    mpMorf->setAnm(bck, i_attr, i_morf, i_rate, 0.0f, -1.0f);
}

bool daE_DT_c::checkBck(int i_anm) {
    J3DAnmTransform* bck = (J3DAnmTransform*)dComIfG_getObjectRes("E_DT", i_anm);
    return mpMorf->getAnm() == bck;
}

void daE_DT_c::setActionMode(int i_action, int i_mode) {
    mAction = i_action;
    mMode = i_mode;
}

void daE_DT_c::damage_check() {
    daPy_py_c* player = daPy_getPlayerActorClass();

    if (mBodyDamageTimer == 0 && (mBodyCcSph[0].ChkTgHit() || mBodyCcSph[1].ChkTgHit())) {
        field_0x748 = -0.3f;
        field_0x74c = 0;
        mBodyDamageTimer = 10;
        mSound.startCreatureSound(Z2SE_EN_DT_SMALL_DAMAGE, 0, -1);
    }

    cXyz pos;

    if (mLegLDamageTimer == 0 && mLegCcSph[0].ChkTgHit()) {
        field_0x74e = 0x600;
        field_0x750 = 0;
        mLegLDamageTimer = 10;
        mDoMtx_stack_c::copy(mpMorf->getModel()->getAnmMtx(JNT_LEG_L1));
        mDoMtx_stack_c::transM(130.0f, 0.0f, 0.0f);
        mDoMtx_stack_c::multVecZero(&pos);
        mDoAud_seStart(Z2SE_EN_DT_NODAMAGE_FOOT, &pos, 0, 0);
    }

    if (mLegRDamageTimer == 0 && mLegCcSph[1].ChkTgHit()) {
        field_0x752 = 0x600;
        field_0x754 = 0;
        mLegRDamageTimer = 10;
        mDoMtx_stack_c::copy(mpMorf->getModel()->getAnmMtx(JNT_LEG_R1));
        mDoMtx_stack_c::transM(130.0f, 0.0f, 0.0f);
        mDoMtx_stack_c::multVecZero(&pos);
        mDoAud_seStart(Z2SE_EN_DT_NODAMAGE_FOOT, &pos, 0, 0);
    }

    if (checkCutDownHitFlg()) {
        offCutDownHitFlg();
        health = 0;
        setActionMode(ACT_DEATH, 0);
    } else if (mDamageTimer == 0) {
        mCcStts.Move();
        if (mTongueCcSph[0].ChkTgHit()) {
            mAtInfo.mpCollider = mTongueCcSph[0].GetTgHitObj();
            dCc_GetAc(mAtInfo.mpCollider->GetAc());
            cc_at_check(this, &mAtInfo);
            if (mAtInfo.mpCollider->ChkAtType(AT_TYPE_UNK)) {
                mDamageTimer = 20;
            } else {
                mDamageTimer = 10;
            }
            if (mAtInfo.mAttackPower <= 1) {
                mDamageTimer = 10;
            }
            mTongueCcSph[0].ClrTgHit();

            BOOL bVar1 = false;
            if (player->getSwordAtUpTime() != 0 || player->getCutCount() >= 4
                || mAtInfo.mpCollider->GetAtAtp() >= 4)
            {
                bVar1 = true;
            } else if (static_cast<dCcD_GObjInf*>(mAtInfo.mpCollider)->GetAtSpl() == 1) {
                field_0x774++;
                if (field_0x774 >= 2) {
                    bVar1 = true;
                }
            }

            if (player->getCutType() == daPy_py_c::CUT_TYPE_JUMP
                && player->checkCutJumpCancelTurn())
            {
                mDamageTimer = 3;
                setActionMode(ACT_DAMAGE, 0);
            } else if (bVar1) {
                field_0x774 = 0;
                setActionMode(ACT_DOWN, 2);
            } else {
                setActionMode(ACT_DAMAGE, 0);
            }
            if (health <= 0) {
                setActionMode(ACT_DEATH, 2);
            }
        }
    }
}

void daE_DT_c::setStabPos() {
    cXyz pos;
    mDoMtx_stack_c::copy(mpMorf->getModel()->getAnmMtx(JNT_TONGUE_5));
    mDoMtx_stack_c::transM(10.0f, 0.0f, 0.0f);
    mDoMtx_stack_c::multVecZero(&pos);
    setDownPos(&pos);
}

bool daE_DT_c::checkBeforeBg(f32 i_dist) {
    dBgS_LinChk lin_chk;
    cM3dGPla plane;
    cXyz start, end;
    start = current.pos;
    start.y += 100.0f;
    end = start;
    end.x += i_dist * cM_ssin(shape_angle.y);
    end.z += i_dist * cM_scos(shape_angle.y);
    lin_chk.Set(&start, &end, this);
    if (dComIfG_Bgsp().LineCross(&lin_chk)) {
        dComIfG_Bgsp().GetTriPla(lin_chk, &plane);
        if ((s16)(cM_atan2s(plane.mNormal.x, plane.mNormal.z) - shape_angle.y) > 0) {
            mTargetAngleY = shape_angle.y + 0x2000;
        } else {
            mTargetAngleY = shape_angle.y - 0x2000;
        }
        return TRUE;
    } else {
        return FALSE;
    }
}

void daE_DT_c::checkTongueBg() {
    if (mAction != ACT_DOWN && mAction != ACT_DAMAGE && mAction != ACT_JUMP) {
        cLib_chaseAngleS(&field_0x734, 0, 0x200);
        return;
    }

    cXyz start, end;
    J3DModel* model = mpMorf->getModel();
    mDoMtx_stack_c::copy(model->getAnmMtx(JNT_TONGUE_4));
    mDoMtx_stack_c::multVecZero(&start);
    mDoMtx_stack_c::copy(model->getAnmMtx(JNT_TONGUE_5));
    mDoMtx_stack_c::transM(15.0f, -10.0f, 0.0f);
    mDoMtx_stack_c::multVecZero(&end);
    dBgS_LinChk lin_chk;
    end.y = start.y;
    lin_chk.Set(&start, &end, this);
    if (dComIfG_Bgsp().LineCross(&lin_chk)) {
        cLib_chaseAngleS(&field_0x734, 15000, 0x400);
        field_0x730 = 0;
    } else if (mAction != ACT_DOWN) {
        cLib_chaseAngleS(&field_0x734, 0, 0x200);
    }
}

void daE_DT_c::checkWaterSurface() {
    dBgS_ObjGndChk_Spl wtr_chk;
    Vec pos;
    pos = current.pos;
    pos.y += 300.0f;
    wtr_chk.SetPos(&pos);
    f32 surface = dComIfG_Bgsp().GroundCross(&wtr_chk);
    if (surface != -1.0e9f) {
        mWaterSurface = surface;
        if (current.pos.y < mWaterSurface) {
            if (checkBck(ANM_DOWN_WAIT) || checkBck(ANM_DOWN_DAMAGE_2)) {
                setHamonEffect(1);
            } else {
                setHamonEffect(0);
            }
        }
    }
}

void daE_DT_c::setSmallWaterEffect() {
    cXyz scl(mScale, mScale, mScale);
    static u16 w_eff_id[5] = {0x8572, 0x8573, 0x8574, 0x8575, 0x8576};
    for (int i = 0; i < 5; i++) {
        dComIfGp_particle_set(w_eff_id[i], &current.pos, &tevStr, &shape_angle, &scl);
    }
}

void daE_DT_c::setLargeWaterEffect() {
    cXyz scl(mScale, mScale, mScale);
    static u16 w_eff_id[5] = {0x856d, 0x856e, 0x856f, 0x8570, 0x8571};
    for (int i = 0; i < 5; i++) {
        dComIfGp_particle_set(w_eff_id[i], &current.pos, &tevStr, &shape_angle, &scl);
    }
}

void daE_DT_c::setHamonEffect(int param_0) {
    if (param_0 >= 2) {
        return;
    }
    cXyz scl(mScale, mScale, mScale);
    cXyz pos(current.pos.x, mWaterSurface, current.pos.z);
    static u16 w_eff_id[4] = {0x8563, 0x8564, 0x8561, 0x8562};
    for (int i = 0; i < 2; i++) {
        mRippleKey[i] = dComIfGp_particle_set(mRippleKey[i], w_eff_id[(param_0 << 1) + i], &pos,
                                              &tevStr, &shape_angle, &scl, 0xff, NULL, -1,
                                              NULL, NULL, NULL);
    }
}

void daE_DT_c::setJumpEffect() {
    cXyz scl(mScale, mScale, mScale);
    static u16 w_eff_id[5] = {0x8565, 0x8566, 0x8567, 0x8568, 0x8569};
    for (int i = 0; i < 5; i++) {
        dComIfGp_particle_set(w_eff_id[i], &current.pos, &tevStr, &shape_angle, &scl);
    }
}

void daE_DT_c::setJumpSizukuEffect() {
    mParticleKey[0] = dComIfGp_particle_set(mParticleKey[0], 0x856a, &current.pos, &tevStr,
                                            &shape_angle, NULL, 0xff, NULL, -1, NULL, NULL, NULL);
    JPABaseEmitter* emitter = dComIfGp_particle_getEmitter(mParticleKey[0]);
    if (emitter != NULL) {
        emitter->setGlobalSRTMatrix(mpMorf->getModel()->getAnmMtx(JNT_BACKBONE_2));
    }
}

void daE_DT_c::setDamageShibukiEffect(int param_0) {
    cXyz scl(mScale, mScale, mScale);
    static u16 w_eff_id[2] = {0x855d, 0x855f};
    dComIfGp_particle_set(w_eff_id[param_0], &current.pos, &tevStr, &shape_angle, &scl);
}

void daE_DT_c::setDamageYodareEffect(int param_0) {
    static u16 w_eff_id[2] = {0x855e, 0x8560};
    mParticleKey[0] = dComIfGp_particle_set(mParticleKey[0], w_eff_id[param_0], &current.pos,
                                            &tevStr, &shape_angle, NULL, 0xff, NULL, -1,
                                            NULL, NULL, NULL);
    JPABaseEmitter* emitter = dComIfGp_particle_getEmitter(mParticleKey[0]);
    if (emitter != NULL) {
        emitter->setGlobalSRTMatrix(mpMorf->getModel()->getAnmMtx(JNT_CHEEK));
    }
}

void daE_DT_c::setSwingEffect() {
    cXyz scl(mScale, mScale, mScale);
    dComIfGp_particle_set(0x856c, &current.pos, &tevStr, &shape_angle, &scl);
}

void daE_DT_c::setStepEffect() {
    cXyz scl(mScale, mScale, mScale);
    if (!checkBck(ANM_STEP)) {
        return;
    }
    if (mpMorf->checkFrame(0.0f)) {
        dComIfGp_particle_set(0x856b, &current.pos, &tevStr, &shape_angle, &scl);
    }
    if (mpMorf->checkFrame(29.0f)) {
        mSound.startCreatureSound(Z2SE_EN_DT_STEP, 0, -1);
        mSound.startCreatureSound(Z2SE_EN_DT_FOOTNOTE_L, 0, -1);
    }
    if (mpMorf->checkFrame(59.0f)) {
        mSound.startCreatureSound(Z2SE_EN_DT_FOOTNOTE_R, 0, -1);
    }
}

void daE_DT_c::setBombEffect() {
    static u16 w_eff_id[4] = {0x8552, 0x8553, 0x8554, 0x8555};
    for (int i = 0; i < 4; i++) {
        mParticleKey[i] = dComIfGp_particle_set(mParticleKey[i], w_eff_id[i], &current.pos,
                                                &tevStr, &shape_angle, NULL, 0xff, NULL, -1,
                                                NULL, NULL, NULL);
        JPABaseEmitter* emitter = dComIfGp_particle_getEmitter(mParticleKey[i]);
        if (emitter != NULL) {
            emitter->setGlobalSRTMatrix(mpMorf->getModel()->getAnmMtx(JNT_CHEEK));
        }
    }
}

void daE_DT_c::setBombSmokeEffect() {
    mSmokeKey = dComIfGp_particle_set(mSmokeKey, 0x8556, &current.pos, &tevStr, &shape_angle,
                                      NULL, 0xff, NULL, -1, NULL, NULL, NULL);
    JPABaseEmitter* emitter = dComIfGp_particle_getEmitter(mSmokeKey);
    if (emitter != NULL) {
        emitter->setGlobalSRTMatrix(mpMorf->getModel()->getAnmMtx(JNT_CHEEK));
    }
}

void daE_DT_c::setDeadShibukiEffect() {
    cXyz scl(mScale, mScale, mScale);
    static u16 w_eff_id[2] = {0x8557, 0x8558};
    for (int i = 0; i < 2; i++) {
        dComIfGp_particle_set(w_eff_id[i], &current.pos, &tevStr, &shape_angle, &scl);
    }
}

void daE_DT_c::setDeadYodareEffect() {
    static u16 w_eff_id[4] = {0x8559, 0x855a, 0x855b, 0x855c};
    for (int i = 0; i < 4; i++) {
        mParticleKey[i] = dComIfGp_particle_set(mParticleKey[i], w_eff_id[i], &current.pos,
                                                &tevStr, &shape_angle, NULL, 0xff, NULL, -1,
                                                NULL, NULL, NULL);
        JPABaseEmitter* emitter = dComIfGp_particle_getEmitter(mParticleKey[i]);
        if (emitter != NULL) {
            emitter->setGlobalSRTMatrix(mpMorf->getModel()->getAnmMtx(JNT_CHEEK));
        }
    }
}

void daE_DT_c::setBoxTubaEffect() {
    mSpitKey = dComIfGp_particle_set(mSpitKey, 0x8598, &current.pos, &tevStr, &shape_angle,
                                     NULL, 0xff, NULL, -1, NULL, NULL, NULL);
    JPABaseEmitter* emitter = dComIfGp_particle_getEmitter(mSpitKey);
    if (emitter != NULL) {
        emitter->setGlobalSRTMatrix(mpMorf->getModel()->getAnmMtx(JNT_CHEEK));
    }
}

void daE_DT_c::setBoxTubaLandEffect() {
    cXyz pos;
    mDoMtx_stack_c::copy(mpSpitModel->getAnmMtx(0));
    mDoMtx_stack_c::multVecZero(&pos);
    static u16 w_eff_id[2] = {0x8599, 0x859a};
    for (int i = 0; i < 2; i++) {
        dComIfGp_particle_set(w_eff_id[i], &pos, &tevStr, NULL, NULL);
    }
}

int E_DT_n::mDt_OtamaNum;

int E_DT_n::mDt_OtamaNo[20];

static void* s_otama_todo(void* i_proc, void* i_this) {
    if (fopAcM_IsActor(i_proc) && !fpcM_IsCreating(fopAcM_GetID(i_proc))
        && fopAcM_GetName(i_proc) == PROC_E_OT)
    {
        daE_OT_c* otama = static_cast<daE_OT_c*>(i_proc);
        E_DT_n::mDt_OtamaNum++;
        if (otama->getChildNo() < 20) {
            E_DT_n::mDt_OtamaNo[otama->getChildNo()] = 1;
        }
    }
    return NULL;
}

int daE_DT_c::getNumberOfOtama() {
    E_DT_n::mDt_OtamaNum = 0;
    for (int i = 0; i < 20; i++) {
        E_DT_n::mDt_OtamaNo[i] = 0;
    }
    fpcM_Search(s_otama_todo, this);
    return E_DT_n::mDt_OtamaNum;
}

int E_DT_n::m_fall_no;

static void* s_fall_otama(void* i_proc, void* i_this) {
    if (fopAcM_IsActor(i_proc) && !fpcM_IsCreating(fopAcM_GetID(i_proc))
        && fopAcM_GetName(i_proc) == PROC_E_OT)
    {
        daE_OT_c* otama = static_cast<daE_OT_c*>(i_proc);
        if (E_DT_n::m_fall_no == otama->getChildNo()
            && otama->getActionMode() == daE_OT_c::ACT_EGG)
        {
            return i_proc;
        }
    }
    return NULL;
}

void daE_DT_c::setStayOtamaFall() {
    for (int i = field_0x704 * 2; i < (field_0x704 + 1) * 2; i++) {
        E_DT_n::m_fall_no = 19 - i;
        daE_OT_c* otama = (daE_OT_c*)fpcM_Search(s_fall_otama, this);
        if (otama != NULL) {
            otama->setShakeEgg();
        }
    }
}

void daE_DT_c::setStayOtamaDelete() {
    for (int i = 0; i < 20; i++) {
        E_DT_n::m_fall_no = i;
        daE_OT_c* otama = (daE_OT_c*)fpcM_Search(s_fall_otama, this);
        if (otama != NULL) {
            otama->setDeleteEgg();
        }
    }
}

static void* s_demo_otama(void* i_proc, void* i_this) {
    if (fopAcM_IsActor(i_proc) && !fpcM_IsCreating(fopAcM_GetID(i_proc))
        && fopAcM_GetName(i_proc) == PROC_E_OT)
    {
        daE_OT_c* otama = static_cast<daE_OT_c*>(i_proc);
        fopAcM_OffStatus(otama, 0x8000);
    }
    return NULL;
}

cXyz E_DT_n::m_tongue_pos;

static void* s_bomb_search(void* i_proc, void* i_this) {
    if (fopAcM_IsActor(i_proc) && !fpcM_IsCreating(fopAcM_GetID(i_proc))) {
        fopAc_ac_c* actor = static_cast<fopAc_ac_c*>(i_proc);
        if (dBomb_c::checkBombActor(actor) && !fopAcM_checkCarryNow(actor)
            && fopAcM_GetPosition_p(actor)->abs(E_DT_n::m_tongue_pos) < 200.0f)
        {
            return actor;
        }
    }
    return NULL;
}

bool daE_DT_c::checkBombEat() {
    mDoMtx_stack_c::copy(mpMorf->getModel()->getAnmMtx(JNT_TONGUE_3));
    mDoMtx_stack_c::multVecZero(&E_DT_n::m_tongue_pos);
    dBomb_c* bomb = (dBomb_c*)fpcM_Search(s_bomb_search, this);
    if (bomb != NULL) {
        bomb->deleteBombAndEffect();
        return true;
    }
    return false;
}

bool daE_DT_c::isShutterOpen() {
    daAmiShutter_c* shutter;
    fopAcM_SearchByName(PROC_Obj_AmiShutter, (fopAc_ac_c**)&shutter);
    if (shutter != NULL && shutter->isShutterOpen()) {
        return true;
    } else {
        return false;
    }
}

void daE_DT_c::executeWait() {
    switch (mMode) {
    case 0:
        mTimer = 0;
        // fallthrough

    case 1:
        if (checkBck(ANM_KYORO_2) && (mpMorf->checkFrame(0.0f) || mpMorf->checkFrame(45.0f))) {
            mSound.startCreatureVoice(Z2SE_EN_DT_V_KYORO, -1);
        }
        if (mTimer == 0) {
            setBck(ANM_WAIT_01, J3DFrameCtrl::EMode_LOOP, 3.0f, 1.0f);
            mMode = 2;
            mTimer = (u8)(150.0f + cM_rndFX(30.0f));
        }
        break;

    case 2:
        if (mTimer == 0) {
            setBck(ANM_KYORO_2, J3DFrameCtrl::EMode_LOOP, 3.0f, 1.0f);
            mMode = 1;
            mTimer = 180;
        }
        break;
    }

    setActionMode(ACT_CRY, 0);
}

void daE_DT_c::executeCry() {
    switch (mMode) {
    case 0:
        if (checkBeforeBg(1200.0f)) {
            mMode = 1;
            mTimer = 30;
            setBck(ANM_STEP, J3DFrameCtrl::EMode_LOOP, 10.0f, 1.0f);
            if ((s16)(shape_angle.y - fopAcM_searchPlayerAngleY(this)) < 0) {
                mTargetAngleY = shape_angle.y + 0x4000;
            } else {
                mTargetAngleY = shape_angle.y - 0x4000;
            }
        } else {
            setBck(ANM_CROAK, J3DFrameCtrl::EMode_NONE, 3.0f, 1.0f);
            field_0x704 = 0;
            mMode = 2;
        }
        break;

    case 1:
        cLib_chaseAngleS(&shape_angle.y, mTargetAngleY, 0x200);
        if (mTimer == 0) {
            if (checkBeforeBg(1200.0f)) {
                mTimer = 30;
            } else {
                setBck(ANM_CROAK, J3DFrameCtrl::EMode_NONE, 3.0f, 1.0f);
                field_0x704 = 0;
                mMode = 2;
            }
        }
        break;

    case 2:
        if (mpMorf->checkFrame(27.0f)) {
            mSound.startCreatureVoice(Z2SE_EN_DT_V_CROAK, -1);
        }
        if (35.0f <= mpMorf->getFrame() && mpMorf->getFrame() < 70.0f) {
            mEyePosMode = 2;
            if (checkBombEat()) {
                setActionMode(ACT_BOMB_DAMAGE, 0);
                return;
            }
        }
        if (mpMorf->checkFrame(30.0f)) {
            mSound.startCreatureSound(Z2SE_EN_DT_SPAWN, 0, -1);
            getNumberOfOtama();
            for (int i = 0; i < 20; i++) {
                if (E_DT_n::mDt_OtamaNo[i] == 0) {
                    fopAcM_create(PROC_E_OT, i, &current.pos, fopAcM_GetRoomNo(this),
                                  &shape_angle, NULL, 0xff);
                }
            }
        }
        if (mpMorf->isStop()) {
            setActionMode(ACT_SHAKE, 0);
        }
        break;
    }
}

void daE_DT_c::executeShake() {
    switch (mMode) {
    case 0:
        setBck(ANM_WAIT_01, J3DFrameCtrl::EMode_LOOP, 3.0f, 1.0f);
        mTimer = 30;
        mMode = 1;
        break;

    case 1:
        if (mTimer == 0) {
            setSwingEffect();
            setBck(ANM_SWING, J3DFrameCtrl::EMode_LOOP, 3.0f, 1.0f);
            mMode = 2;
            field_0x704 = 0;
            mTimer = 60;
        }
        break;

    case 2:
        if (mpMorf->checkFrame(12.0f)) {
            mSound.startCreatureSound(Z2SE_EN_DT_SWING, 0, -1);
        }
        if (field_0x704 < 10) {
            if (60 - mTimer == E_DT_n::eDt_ShakeFrame[field_0x704]) {
                setStayOtamaFall();
                field_0x704++;
            }
        }
        if (mTimer == 0) {
            setActionMode(ACT_WALK, 0);
        }
        break;
    }
}

void daE_DT_c::executeWalk() {
    cXyz offset;

    if (mMode >= 5 && (mBodyCcSph[0].ChkTgHit() || mBodyCcSph[1].ChkTgHit()
                    || mLegCcSph[0].ChkTgHit() || mLegCcSph[1].ChkTgHit()))
    {
        mMode = 7;
        setBck(ANM_SMALL_DAMAGE, J3DFrameCtrl::EMode_NONE, 3.0f, 1.0f);
        mSound.startCreatureSound(Z2SE_EN_DT_SMALL_DAMAGE, 0, -1);
    }

    switch (mMode) {
    case 0:
        setBck(ANM_WAIT_01, J3DFrameCtrl::EMode_LOOP, 3.0f, 1.0f);
        mTimer = 30;
        mMode = 1;
        mWalkTimer = 1200;
        // fallthrough

    case 1:
        if (mTimer == 0) {
            setBck(ANM_STEP, J3DFrameCtrl::EMode_LOOP, 10.0f, 1.0f);
            offset.set(0.0f, 0.0f, 1500.0f);
            s16 angle = cM_rndFX(0x2000) + fopAcM_searchPlayerAngleY(this) + 0x8000;
            cLib_offsetPos(&mTargetPos, &home.pos, angle, &offset);
            mTargetAngleY = cLib_targetAngleY(&current.pos, &mTargetPos);
            mMode = 2;
        }
        break;

    case 2:
        if (cLib_chaseAngleS(&shape_angle.y, mTargetAngleY, 0x100)) {
            current.angle.y = shape_angle.y;
            mTimer = 300;
            setBck(ANM_WALK_01, J3DFrameCtrl::EMode_LOOP, 10.0f, 1.0f);
            mMode = 3;
        }
        break;

    case 3:
        if (mpMorf->checkFrame(0.0f) || mpMorf->checkFrame(45.0f)) {
            mSound.startCreatureSound(Z2SE_EN_DT_WALK, 0, -1);
        }
        if (mpMorf->checkFrame(23.0f)) {
            mSound.startCreatureSound(Z2SE_EN_DT_FOOTNOTE_L, 0, -1);
        }
        if (mpMorf->checkFrame(65.0f)) {
            mSound.startCreatureSound(Z2SE_EN_DT_FOOTNOTE_R, 0, -1);
        }
        cLib_chaseF(&speedF, l_HIO.mWalkSpeed, 1.0f);
        if (mTimer == 0 || mTargetPos.absXZ(current.pos) < 300.0f) {
            setBck(ANM_STEP, J3DFrameCtrl::EMode_LOOP, 10.0f, 1.0f);
            mTargetAngleY = cLib_targetAngleY(&current.pos, &home.pos);
            mMode = 4;
            speedF = 0.0f;
        }
        break;

    case 4:
        if (cLib_chaseAngleS(&shape_angle.y, mTargetAngleY, 0x100)) {
            current.angle.y = shape_angle.y;
            setBck(ANM_WAIT_01, J3DFrameCtrl::EMode_LOOP, 10.0f, 1.0f);
            mMode = 5;
            field_0x704 = 0;
        }
        break;

    case 5:
        if (abs((s16)(shape_angle.y - fopAcM_searchPlayerAngleY(this))) < 0x3000) {
            field_0x73e = fopAcM_searchPlayerAngleX(this);
            field_0x740 = fopAcM_searchPlayerAngleY(this) - shape_angle.y;
            field_0x704 = 0;
        } else {
            field_0x704++;
            if (field_0x704 >= 150) {
                mMode = 6;
                setBck(ANM_KYORO_2, J3DFrameCtrl::EMode_NONE, 10.0f, 1.0f);
                mSound.startCreatureVoice(Z2SE_EN_DT_V_KYORO, -1);
            }
        }
        break;

    case 6:
        if (mpMorf->checkFrame(45.0f)) {
            mSound.startCreatureVoice(Z2SE_EN_DT_V_KYORO, -1);
        }
        if (mpMorf->isStop()) {
            setBck(ANM_WAIT_01, J3DFrameCtrl::EMode_LOOP, 10.0f, 1.0f);
            mMode = 5;
            field_0x704 = 0;
        }
        break;

    case 7:
        if (mpMorf->isStop()) {
            setBck(ANM_WAIT_01, J3DFrameCtrl::EMode_LOOP, 3.0f, 1.0f);
            mMode = 5;
            field_0x704 = 0;
        }
        break;
    }

    if (getNumberOfOtama() < 1 || (getNumberOfOtama() < 20 && mWalkTimer == 0)) {
        setActionMode(ACT_JUMP, 0);
    }
}

void daE_DT_c::executeJump() {
    s16 player_angle = fopAcM_searchPlayerAngleY(this);
    cXyz pos, offset, press_pos;
    offset.set(0.0f, 0.0f, 160.0f);
    cLib_offsetPos(&pos, &current.pos, shape_angle.y, &offset);
    if (!mLinkPressed) {
        mTongueCcSph[0].OnCoSetBit();
        mTongueCcSph[1].OnCoSetBit();
    }
    cXyz player_offset;
    cXyz player_pos = daPy_getPlayerActorClass()->current.pos;
    s16 angle_to_player = (s16)cLib_targetAngleY(&pos, &player_pos);
    s16 press_angle;
    mDoMtx_stack_c::YrotS(daPy_getPlayerActorClass()->current.angle.y);
    player_offset.set(0.0f, 0.0f, daPy_getPlayerActorClass()->getSpeedF() * 20.0f);
    mDoMtx_stack_c::multVec(&player_offset, &player_offset);

    switch (mMode) {
    case 0:
        setBck(ANM_STEP, J3DFrameCtrl::EMode_LOOP, 10.0f, 1.0f);
        speed.y = 0.0f;
        speedF = 0.0f;
        gravity = -5.0f;
        mMode = 1;
        mDemoMode = 0;
        field_0x736 = l_HIO.field_0x14;
        break;

    case 1:
        cLib_addCalcAngleS(&shape_angle.y, player_angle, 0x10, 0x100, 0x100);
        if (abs((s16)(player_angle - shape_angle.y)) < 0x1000
            || fopAcM_searchPlayerDistance(this) < 700.0f)
        {
            mMode = 2;
            setBck(ANM_JUMP_START, J3DFrameCtrl::EMode_NONE, 3.0f, 1.0f);
            mSound.startCreatureVoice(Z2SE_EN_DT_V_JUMP, -1);
            setJumpEffect();
            if (!field_0x783) {
                mDemoMode = 1;
            }
            current.angle.y = shape_angle.y;
        }
        break;

    case 2:
        setJumpSizukuEffect();
        if (mpMorf->checkFrame(87.0f)) {
            mSound.startCreatureSound(Z2SE_EN_DT_JUMP, 0, -1);
        }
        if (mpMorf->isStop()) {
            speedF = 30.0f;
            speed.y = 150.0f;
            mMode = 3;
            mTimer = 60;
            current.angle.y = cLib_targetAngleY(&current.pos, &player_pos);
            setBck(ANM_JUMP_TOP, J3DFrameCtrl::EMode_NONE, 20.0f, 1.0f);
        }
        break;

    case 3:
        setJumpSizukuEffect();
        Z2GetAudioMgr()->changeSubBgmStatus(2);
        if (speed.y <= 0.0f) {
            cLib_chaseF(&current.pos.x, player_pos.x + player_offset.x, 70.0f);
            cLib_chaseF(&current.pos.z, player_pos.z + player_offset.z, 70.0f);
            speedF = 0.0f;
            gravity = 0.0f;
            speed.y = 0.0f;
            if (dComIfGp_checkPlayerStatus0(0, 0x2000) || mTimer == 0) {
                mMode = 4;
                maxFallSpeed = -50.0f;
                gravity = -5.0f;
                mTimer = 30;
                field_0x704 = 0;
            }
        }
        if (current.pos.y > 2000.0f) {
            cLib_chaseF(&current.pos.x, player_pos.x + player_offset.x, 70.0f);
            cLib_chaseF(&current.pos.z, player_pos.z + player_offset.z, 70.0f);
        }
        break;

    case 4:
        mSound.startCreatureSoundLevel(Z2SE_EN_DT_FALLING, 0, -1);
        mFlyingAttack = true;
        if (!dComIfGp_checkPlayerStatus0(0, 0x2000)) {
            current.pos.x = player_pos.x + player_offset.x;
            current.pos.z = player_pos.z + player_offset.z;
        }
        maxFallSpeed = l_HIO.mMaxFallSpeed;
        gravity = -5.0f;
        mMode = 5;
        field_0x704 = 0;
        mTimer = 10;
        shape_angle.y = cLib_targetAngleY(&current.pos, &home.pos) + cM_rndFX(0x1000);
        break;

    case 5:
        mSound.startCreatureSoundLevel(Z2SE_EN_DT_FALLING, 0, -1);
        // fallthrough

    case 6:
        mFlyingAttack = true;

        switch (field_0x704) {
        case 0:
            if (mBodyCcSph[0].ChkCoHit()) {
                if (fopAcM_GetName(dCc_GetAc(mBodyCcSph[0].GetCoHitObj()->GetAc())) == PROC_ALINK)
                {
                    field_0x704 = 2;
                    mLinkPressed = true;
                }
            }
            if (mBodyCcSph[1].ChkCoHit()) {
                if (fopAcM_GetName(dCc_GetAc(mBodyCcSph[1].GetCoHitObj()->GetAc())) == PROC_ALINK)
                {
                    field_0x704 = 2;
                    mLinkPressed = true;
                }
            }
            break;

        case 2:
            if (mLinkPressed == true) {
                s16 angle_diff = shape_angle.y - angle_to_player;
                if (abs(angle_diff) < 0x2000) {
                    if (angle_diff < 0) {
                        angle_to_player += 0x2000;
                    } else {
                        angle_to_player += -0x2000;
                    }
                }
                offset.set(0.0f, 0.0f, 470.0f);
                cLib_offsetPos(&press_pos, &pos, angle_to_player, &offset);
                press_angle = angle_to_player;
            } else {
                mDoMtx_stack_c::copy(mpMorf->getModel()->getAnmMtx(JNT_TONGUE_3));
                mDoMtx_stack_c::multVecZero(&press_pos);
                if (cM_rnd() < 0.5f) {
                    press_angle = shape_angle.y + 0x4000;
                } else {
                    press_angle = shape_angle.y - 0x4000;
                }
            }
            press_pos.y = mAcch.GetGroundH();
            daPy_getPlayerActorClass()->onPressedDamage(press_pos, press_angle);
            daPy_py_c::setPlayerDamage(4, 1);
            field_0x704 = 3;
            Z2GetAudioMgr()->changeSubBgmStatus(1);
            break;

        case 3:
            mDemoMode = 3;
            field_0x704 = 4;
            break;
        }

        if (mMode == 5) {
            if (mAcch.ChkGroundHit()) {
                setLargeWaterEffect();
                mFlyingAttackLand = true;
                dComIfGp_getVibration().StartQuake(8, 0x1f, cXyz(0.0f, 1.0f, 0.0f));
                mQuakeTimer = 30;
                setBck(ANM_JUMP_END, J3DFrameCtrl::EMode_NONE, 3.0f, 1.0f);
                mMode = 6;
                mSound.startCreatureSound(Z2SE_EN_DT_SPLASH, 0, -1);
            }
        } else if (mpMorf->getFrame() >= 5.0f) {
            if (field_0x704 == 1 || field_0x704 >= 4) {
                mMode = 7;
            } else if (field_0x704 == 0) {
                field_0x704 = 1;
                Z2GetAudioMgr()->changeSubBgmStatus(3);
                break;
            }
        }
        break;

    case 7:
        if (mpMorf->isStop()) {
            setActionMode(ACT_DOWN, 0);
        }
        break;
    }

    f32 tmp = 2000.0f - current.pos.y;
    if (tmp < 0.0f) {
        tmp = 0.0f;
    }
    if (speed.y < 0.0f) {
        if (tmp > 2000.0f) {
            tmp = 2000.0f;
        }
        mScale = tmp / 2000.0f * 1.2f + 0.3f;
    } else {
        if (tmp > 2000.0f) {
            tmp = 2000.0f;
        }
        mScale = tmp / 2000.0f + 0.5f;
    }
}

void daE_DT_c::executeDown() {
    switch (mMode) {
    case 0:
        if (mLinkPressed) {
            mTimer = l_HIO.mPressTimer;
        } else {
            mTimer = l_HIO.mStunTimer;
        }
        field_0x774 = 0;
        // fallthrough

    case 1:
        setBck(ANM_DOWN_WAIT, J3DFrameCtrl::EMode_LOOP, 3.0f, 1.0f);
        speed.y = 0.0f;
        speedF = 0.0f;
        mMode = 3;
        if (mHitCounter >= 2) {
            onDownFlg();
            setStabPos();
        }
        break;

    case 2:
        setDamageShibukiEffect(1);
        setDamageYodareEffect(1);
        Z2GetAudioMgr()->changeSubBgmStatus(1);
        mTimer = 0;
        mSound.startCreatureVoice(Z2SE_EN_DT_V_DOWN_DAMAGE2, -1);
        setBck(ANM_DOWN_DAMAGE_2, J3DFrameCtrl::EMode_NONE, 3.0f, 1.0f);
        speed.y = 0.0f;
        speedF = 0.0f;
        mMode = 4;
        mEyePosMode = 1;
        mHitCounter++;
        if (mHitCounter >= 3) {
            setActionMode(ACT_DEATH, 2);
        }
        break;

    case 3:
        if (checkBck(ANM_DOWN_WAIT) && mpMorf->checkFrame(56.0f)) {
            mSound.startCreatureVoice(Z2SE_EN_DT_V_DOWN_WAIT, -1);
        }
        if (mTimer == 0) {
            offDownFlg();
            if (mLinkPressed) {
                daPy_getPlayerActorClass()->offPressedDamage();
                mDemoMode = 6;
                mLinkPressed = false;
            } else {
                Z2GetAudioMgr()->changeSubBgmStatus(1);
            }
            setBck(ANM_DOWN_RETURN_2, J3DFrameCtrl::EMode_NONE, 3.0f, 1.0f);
            mSound.startCreatureVoice(Z2SE_EN_DT_V_RETURN, -1);
            mMode = 5;
        } else {
            if (mHitCounter >= 2) {
                setStabPos();
            }
            if (daPy_getPlayerActorClass()->getCutType() == daPy_py_c::CUT_TYPE_DOWN) {
                mCcStts.ClrCcMove();
                mTimer = 30;
            } else if (!mLinkPressed) {
                mTongueCcSph[0].OnTgSetBit();
                mTongueCcSph[0].OnCoSetBit();
                mTongueCcSph[1].OnCoSetBit();
            }
            mEyePosMode = 1;
        }
        break;

    case 4:
        setDamageYodareEffect(1);
        if (mpMorf->checkFrame(50.0f)) {
            mSound.startCreatureSound(Z2SE_CM_BODYFALL_ASASE_L, 0, -1);
            setSmallWaterEffect();
        }
        // fallthrough

    case 5:
        if (mpMorf->isStop()) {
            setActionMode(ACT_SEARCH, 0);
        }
        break;
    }
}

void daE_DT_c::executeSearch() {
    switch (mMode) {
    case 0:
        setBck(ANM_WAIT_01, J3DFrameCtrl::EMode_LOOP, 3.0f, 1.0f);
        mTimer = 30;
        mMode = 1;
        break;

    case 1:
        if (mTimer == 0) {
            setActionMode(ACT_WAIT, 0);
        }
        break;
    }
}

void daE_DT_c::executeDamage() {
    if (mTimer != 0) {
        if (!mLinkPressed) {
            mTongueCcSph[0].OnTgSetBit();
            mTongueCcSph[0].OnCoSetBit();
            mTongueCcSph[1].OnCoSetBit();
        }
        mEyePosMode = 1;
    }

    switch (mMode) {
    case 0:
        setDamageShibukiEffect(0);
        setDamageYodareEffect(0);
        if (mTimer > 120) {
            mTimer = 120;
        }
        mSound.startCreatureVoice(Z2SE_EN_DT_V_DOWN_DAMAGE, -1);
        setBck(ANM_DOWN_DAMAGE, J3DFrameCtrl::EMode_NONE, 3.0f, 1.0f);
        mMode = 1;
        break;

    case 1:
        setDamageYodareEffect(0);
        if (mpMorf->isStop()) {
            setActionMode(ACT_DOWN, 1);
        }
        break;
    }
}

void daE_DT_c::executeBombDamage() {
    mTongueCcSph[0].OnCoSetBit();
    mTongueCcSph[1].OnCoSetBit();

    switch (mMode) {
    case 0:
        setBombEffect();
        setBombSmokeEffect();
        mSound.startCreatureVoice(Z2SE_EN_DT_CROAK_DAMAGE, -1);
        setBck(ANM_CROAK_DAMAGE, J3DFrameCtrl::EMode_NONE, 3.0f, 1.0f);
        mMode = 1;
        break;

    case 1:
        setBombEffect();
        setBombSmokeEffect();
        if (mpMorf->checkFrame(119.0f)) {
            mSound.startCreatureSound(Z2SE_EN_DT_CROAK_BOMB, 0, -1);
            setStayOtamaDelete();
        }
        if (mpMorf->checkFrame(135.0f)) {
            mSound.startCreatureSound(Z2SE_EN_DT_SPLASH, 0, -1);
            setSmallWaterEffect();
        }
        if (mpMorf->getFrame() >= 150.0f) {
            if (mpMorf->checkFrame(150.0f)) {
                Z2GetAudioMgr()->changeSubBgmStatus(3);
            }
            mTimer = l_HIO.mStunTimer;
            mEyePosMode = 1;
            mTongueCcSph[0].OnTgSetBit();
            mTongueCcSph[0].OnCoSetBit();
            mTongueCcSph[1].OnCoSetBit();
        }
        if (mpMorf->isStop()) {
            setActionMode(ACT_DOWN, 0);
        }
        break;
    }
}

void daE_DT_c::executeDeath() {
    cXyz player_pos(0.0f, 0.0f, 450.0f);
    camera_class* camera = dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0));
    cXyz offset, disappear_pos;
    daPy_py_c* player = daPy_getPlayerActorClass();

    switch (mMode) {
    case 0:
        mSound.startCreatureVoice(Z2SE_EN_DT_V_TODOME, -1);
        attention_info.flags = 0;
        setBck(ANM_DOWN_WAIT, J3DFrameCtrl::EMode_LOOP, 3.0f, 1.0f);
        mTimer = 30;
        mMode = 1;
        mDead = 1;
        field_0x781 = true;
        speed.y = 0.0f;
        gravity = 0.0f;
        break;

    case 1:
        if (player->getCutType() == daPy_py_c::CUT_TYPE_DOWN) {
            mTimer = 10;
        }
        if (mTimer == 0) {
            mMode = 2;
        }
        break;

    case 2:
        if (!eventInfo.checkCommandDemoAccrpt()) {
            fopAcM_orderPotentialEvent(this, 2, 0xffff, 0);
            eventInfo.onCondition(dEvtCnd_CANDEMO_e);
        } else {
            setDeadShibukiEffect();
            setDeadYodareEffect();
            camera->mCamera.Stop();
            mMode = 3;
            mDead = 1;
            current.pos.set(0.0f, 0.0f, -1000.0f);
            current.angle.y = 0;
            shape_angle.y = 0;
            shape_angle.z = 0;
            shape_angle.x = 0;
            player->changeOriginalDemo();
            player->setPlayerPosAndAngle(&player_pos, 0x8000, 0);
            player->changeDemoMode(4, 1, 0, 0);
            mCamEye.set(150.0f, 100.0f, 800.0f);
            mCamCenter = eyePos;
            mCamFovy = 55.0f;
            camera->mCamera.SetTrimSize(3);
            attention_info.flags = 0;
            setBck(ANM_DEAD, J3DFrameCtrl::EMode_NONE, 3.0f, 1.0f);
            mSound.startCreatureVoice(Z2SE_EN_DT_V_DEAD, -1);
            mMode = 3;
            mTimer = 300;
            Z2GetAudioMgr()->subBgmStop();
            Z2GetAudioMgr()->muteSceneBgm(0, 0.0f);
            dComIfGs_onStageMiddleBoss();
        }
        break;

    case 3:
        player->setPlayerPosAndAngle(&player_pos, 0x8000, 0);
        setBoxTubaEffect();
        if (mpMorf->checkFrame(70.0f) || mpMorf->checkFrame(110.0f) || mpMorf->checkFrame(125.0f)) {
            setDeadShibukiEffect();
        }
        if (mpMorf->checkFrame(212.0f)) {
            mSound.startCreatureSound(Z2SE_CM_BODYFALL_ASASE_L, 0, -1);
            setSmallWaterEffect();
        }
        setDeadYodareEffect();
        if (mpMorf->checkFrame(176.0f)) {
            mDoMtx_stack_c::copy(mpMorf->getModel()->getAnmMtx(JNT_TONGUE_3));
            mDoMtx_stack_c::multVecZero(&mChestPos);
            mChestSpeed.y = 50.0f;
            mChestSpeed.z = 26.2f;
            mChestMode = 1;
            s16 ang_x = 0x4000 - cM_atan2s(mChestSpeed.y, mChestSpeed.absXZ());
            mSpitAngle[2].x = ang_x;
            mSpitAngle[1].x = ang_x;
            mSpitAngle[0].x = ang_x;
            player->changeDemoMode(23, 1, 0, 0);
        }

        if (mpMorf->getFrame() < 30.0f) {
            cLib_addCalc2(&mBlurRate, 180.0f, 1.0f, 8.0f);
            mBlurMode = 1;
        } else if (mBlurRate) {
            cLib_addCalc2(&mBlurRate, 0.0f, 1.0f, 1.0f);
            if (!mBlurRate) {
                mBlurMode = 2;
            }
        } else {
            mBlurMode = 0;
        }

        if (mTimer < 120) {
            cLib_chasePos(&mCamEye, cXyz(200.0f, 100.0f, 700.0f), 5.0f);
            cLib_chaseF(&field_0x6e4, 0.3f, 0.03f);
            mCamCenter = eyePos * field_0x6e4 + mChestPos * (1.0f - field_0x6e4);
        } else {
            field_0x6e4 = 1.0f;
            if (mTimer < 270) {
                cLib_chasePos(&mCamEye, cXyz(200.0f, 100.0f, 600.0f), 5.0f);
            }
            mCamCenter = eyePos;
        }
        camera->mCamera.Set(mCamCenter, mCamEye, mCamFovy, 0);

        if (mpMorf->getFrame() > 176.0f) {
            cLib_addCalc2(&field_0x72c, -50.0f, 1.0f, 0.5f);
        }
        if (mpMorf->isStop()) {
            mTimer = 20;
            mMode = 4;
        }
        break;

    case 4:
        mEyePosMode = 3;
        cLib_addCalc2(&field_0x72c, -50.0f, 1.0f, 0.5f);
        if (mTimer == 0) {
            offset.set(0.0f, 0.0f, 160.0f);
            cLib_offsetPos(&disappear_pos, &current.pos, shape_angle.y, &offset);
            fopAcM_createDisappear(this, &disappear_pos, 40, 0, 0xff);
            mDead = 2;
            mTimer = 10;
            mMode = 5;
        }
        break;

    case 5:
        mEyePosMode = 3;
        if (mTimer == 0) {
            mTimer = 120;
            mMode = 6;
        }
        break;

    case 6:
        mEyePosMode = 3;
        cLib_addCalcPos2(&mCamCenter, cXyz(0.0f, 100.0f, 0.0f), 0.1f, 5.0f);
        camera->mCamera.Set(mCamCenter, mCamEye, mCamFovy, 0);
        if (mTimer == 0) {
            camera->mCamera.Reset(mCamCenter, mCamEye, mCamFovy, 0);
            camera->mCamera.Start();
            camera->mCamera.SetTrimSize(0);
            dComIfGp_event_reset();
            Z2GetAudioMgr()->unMuteSceneBgm(45);
            fopAcM_delete(this);
        }
        break;
    }
}

bool daE_DT_c::pointInSight(cXyz* i_point) {
    cXyz proj;
    u32 idx = fopCamM_GetParam(dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0)));
    scissor_class* scissor = dComIfGp_getWindow(dComIfGp_getCameraWinID(idx))->getScissor();
    f32 width = scissor->width;
    f32 height = scissor->height;
    mDoLib_project(i_point, &proj);
    if (proj.x > 0.0f && proj.x < width && proj.y > 30.0f && -30.0f < height) {
        return true;
    } else {
        return false;
    }
}

void daE_DT_c::executeOpening() {
    daPy_py_c* player = daPy_getPlayerActorClass();
    dCamera_c* camera = dCam_getBody();
    cXyz vec1;
    cXyz vec2 = eyePos;
    vec2.y -= 100.0f;
    int i;
    BOOL in_sight, create_otama, bVar1;
    fopAc_ac_c* otama;

    switch (mMode) {
    case 0:
        mMode = 1;
        setBck(ANM_OP_DEMO_WAIT, J3DFrameCtrl::EMode_LOOP, 3.0f, 1.0f);
        attention_info.flags = 0;
        if (getNumberOfOtama() == 0) {
            for (i = 0; i < 20; i++) {
                fopAcM_create(PROC_E_OT, i | 0xff030000, &current.pos, fopAcM_GetRoomNo(this),
                              &shape_angle, NULL, 0xff);
            }
        }
        // fallthrough

    case 1:
    case 2:
        vec1.set(0.0f, 0.0f, 0.0f);
        shape_angle.y = cLib_targetAngleY(&player->current.pos, &vec1);
        shape_angle.x = -0x4000;
        mDoMtx_stack_c::transS(0.0f, 4000.0f, 0.0f);
        mDoMtx_stack_c::YrotM(shape_angle.y);
        mDoMtx_stack_c::transM(0.0f, 0.0f, -100.0f);
        mDoMtx_stack_c::multVecZero(&current.pos);
        field_0x781 = true;
        if (isShutterOpen()) {
            field_0x714 = 690;
            return;
        }

        in_sight = pointInSight(&current.pos);
        bVar1 = FALSE;
        if (dCam_getBody()->Mode() == 4 || dCam_getBody()->Mode() == 7
            || dCam_getBody()->Mode() == 8 || dComIfGp_checkPlayerStatus0(0, 0x200000))
        {
            bVar1 = TRUE;
        }

        if (!bVar1) {
            field_0x704 = 0;
            if (mMode == 1) {
                if (mTimer == 0) {
                    setBck(ANM_OP_DEMO_SWING, J3DFrameCtrl::EMode_NONE, 3.0f, 1.0f);
                    mMode = 2;
                    field_0x775 = cM_rndF(2.9f);
                }
            } else {
                create_otama = FALSE;
                if (mpMorf->checkFrame(14.0f)) {
                    create_otama = TRUE;
                }
                else if (mpMorf->checkFrame(19.0f)) {
                    if (field_0x775 >= 1) {
                        create_otama = TRUE;
                    }
                }
                else if (mpMorf->checkFrame(24.0f)) {
                    if (field_0x775 >= 2) {
                        create_otama = TRUE;
                    }
                }
                if (create_otama) {
                    vec1.set(0.0f, 2000.0f, 0.0f);
                    fopAcM_create(PROC_E_OT, 0xff01ffff, &vec1, fopAcM_GetRoomNo(this),
                                &shape_angle, NULL, 0xff);
                }
                if (mpMorf->isStop() && getNumberOfOtama() <= 20) {
                    setBck(ANM_OP_DEMO_WAIT, J3DFrameCtrl::EMode_LOOP, 3.0f, 1.0f);
                    mMode = 1;
                    mTimer = 300.0f + cM_rndFX(60.0f);
                }
            }
        } else {
            if (field_0x704 != 0) {
                setBck(ANM_OP_DEMO_WAIT, J3DFrameCtrl::EMode_LOOP, 3.0f, 1.0f);
                field_0x704++;
            }
            if (in_sight) {
                camera->Stop();
                mCamEye = camera->Eye();
                mCamCenter = camera->Center();
                mCamFovy = camera->Fovy();
                mCamBank = camera->Bank();
                player->onPlayerNoDraw();
                camera->SetTrimSize(3);
                setBck(ANM_OP_DEMO_WAIT, J3DFrameCtrl::EMode_LOOP, 3.0f, 1.0f);
                mMode = 3;
                Z2GetAudioMgr()->subBgmStart(Z2BGM_DEKUTOAD_D01);
                dComIfGs_onOneZoneSwitch(3, fopAcM_GetRoomNo(this));
            }
        }
        return;

    case 3:
        if (!eventInfo.checkCommandDemoAccrpt()) {
            fopAcM_orderPotentialEvent(this, 2, 0xffff, 0);
            eventInfo.onCondition(dEvtCnd_CANDEMO_e);
        } else {
            mMode = 4;
            mTimer = 60;
            mSound.startCreatureVoice(Z2SE_EN_DT_V_OPDEMO, -1);
        }
        break;

    case 4:
        cLib_chasePos(&mCamCenter, vec2, 5.0f);
        if (mTimer == 0) {
            mMode = 5;
            setBck(ANM_OP_DEMO_SWING, J3DFrameCtrl::EMode_NONE, 3.0f, 1.0f);
            mTimer = 60;
            field_0x704 = 0;
        }
        break;

    case 5:
        cLib_chasePos(&mCamCenter, vec2, 5.0f);
        if (checkBck(ANM_OP_DEMO_SWING) && (mpMorf->checkFrame(10.0f) || mpMorf->checkFrame(15.0f)
                                            || mpMorf->checkFrame(20.0f))) {
            E_DT_n::m_fall_no = mpMorf->getFrame() - 10.0f;
            daE_OT_c* otama = (daE_OT_c*)fpcM_Search(s_fall_otama, this);
            if (otama != NULL) {
                otama->setFallEgg();
                mDemoOtama[field_0x704] = otama;
                field_0x704++;
            }
        }

        if (mpMorf->isStop()) {
            setBck(ANM_OP_DEMO_WAIT, J3DFrameCtrl::EMode_LOOP, 3.0f, 1.0f);
        }

        if (mTimer == 0) {
            mMode = 6;
            mTimer = 60;
            field_0x6e8 = -300.0f;
            mDoMtx_stack_c::copy(mpMorf->getModel()->getAnmMtx(JNT_TONGUE_1));
            mDoMtx_stack_c::multVecZero(&vec2);
            vec2.y += field_0x6e8;
            mCamCenter = vec2;
            mDoMtx_stack_c::YrotS(shape_angle.y + 0x7c00);
            mDoMtx_stack_c::transM(0.0f, 3000.0f, 1700.0f);
            mDoMtx_stack_c::multVecZero(&mCamEye);
            mCamFovy = 55.0f;
            mCamBank = 0;
        }
        break;

    case 6:
        cLib_chaseF(&field_0x6e8, 0.0f, 3.0f);
        mDoMtx_stack_c::YrotS(shape_angle.y + 0x6000);
        mDoMtx_stack_c::transM(0.0f, 3400.0f, 1400.0f);
        mDoMtx_stack_c::multVecZero(&vec1);
        cLib_chasePos(&mCamEye, vec1, 8.0f);
        mDoMtx_stack_c::copy(mpMorf->getModel()->getAnmMtx(JNT_TONGUE_1));
        mDoMtx_stack_c::multVecZero(&vec2);
        vec2.y += field_0x6e8;
        cLib_chasePos(&mCamCenter, vec2, 50.0f);

        if (mTimer == 0) {
            setBck(ANM_OP_DEMO_FALL, J3DFrameCtrl::EMode_NONE, 3.0f, 1.0f);
            mMode = 7;
        }
        break;

    case 7:
        cLib_chaseF(&field_0x6e8, 0.0f, 3.0f);
        mDoMtx_stack_c::YrotS(shape_angle.y + 0x6000);
        mDoMtx_stack_c::transM(0.0f, 3400.0f, 1400.0f);
        mDoMtx_stack_c::multVecZero(&vec1);
        cLib_chasePos(&mCamEye, vec1, 8.0f);
        mDoMtx_stack_c::copy(mpMorf->getModel()->getAnmMtx(JNT_TONGUE_1));
        mDoMtx_stack_c::multVecZero(&vec2);
        vec2.y += field_0x6e8;
        cLib_chasePos(&mCamCenter, vec2, 50.0f);
        
        if (mpMorf->checkFrame(143.0f)) {
            mMode = 8;
            current.angle.y = shape_angle.y - 0x8000;
            gravity = -5.0f;
            maxFallSpeed = -100.0f;
            mTimer = 40;
            field_0x6e8 = 20.0f;
        }
        break;

    case 8:
        mDoMtx_stack_c::YrotS(shape_angle.y + 0x6000);
        mDoMtx_stack_c::transM(0.0f, 3000.0f, 1700.0f);
        mDoMtx_stack_c::multVecZero(&vec1);
        cLib_chasePos(&mCamEye, vec1, 30.0f);
        mDoMtx_stack_c::copy(mpMorf->getModel()->getAnmMtx(JNT_HEAD));
        mDoMtx_stack_c::multVecZero(&vec2);
        cLib_chasePos(&mCamCenter, vec2, field_0x6e8);

        if (mpMorf->isStop()) {
            setBck(ANM_JUMP_TOP, J3DFrameCtrl::EMode_NONE, 20.0f, 1.0f);
        }
        cLib_chaseF(&speedF, 0.0f, 1.0f);
        if (mTimer < 15) {
            cLib_chaseF(&field_0x6e8, 0.0f, 3.0f);
        }

        if (mTimer == 0) {
            mMode = 10;
            field_0x781 = false;
            speedF = 0.0f;
            speed.y = 0.0f;
            maxFallSpeed = -100.0f;
            current.pos.set(0.0f, 2000.0f, 0.0f);
            shape_angle.y = 0;
            shape_angle.x = 0;
            vec1.set(0.0f, 0.0f, 1400.0f);
            player->offPlayerNoDraw();
            player->setPlayerPosAndAngle(&vec1, shape_angle.y + 0x8000, 0);
            mCamCenter.set(0.0f, 3112.0f, 243.0f);
            mCamEye.set(-150.0f, 150.0f, 1600.0f);
            mCamFovy = 115.0f;
            mDemoJump = true;
        }
        break;

    case 10:
        mSound.startCreatureSoundLevel(Z2SE_EN_DT_FALLING, 0, -1);
        mDemoJump = false;
        mCamCenter = vec2;
        cLib_chaseF(&mCamFovy, 70.0f, 1.0f);
        
        if (mAcch.ChkGroundHit()) {
            setLargeWaterEffect();
            mMode = 11;
            setBck(ANM_OP_DEMO, J3DFrameCtrl::EMode_NONE, 3.0f, 1.0f);
            dComIfGp_getVibration().StartQuake(8, 0x1f, cXyz(0.0f, 1.0f, 0.0f));
            mQuakeTimer = 30;
            mSound.startCreatureSound(Z2SE_EN_DT_SPLASH, 0, -1);
            player->changeOriginalDemo();
            player->changeDemoMode(34, 1, 0, 0);
            mTimer = 90;
        }
        break;

    case 11:
        cLib_chasePos(&mCamCenter, vec2, 100.0f);
        vec1.set(-100.0f, 50.0f, 1550.0f);
        cLib_chasePos(&mCamEye, vec1, 100.0f);
        cLib_chaseF(&mCamFovy, 70.0f, 1.5f);
        if (mTimer == 1) {
            player->changeDemoMode(23, 1, 0, 0);
        }

        if (mpMorf->checkFrame(130.0f)) {
            getNumberOfOtama();
            for (int i = 0; i < 20; i++) {
                if (E_DT_n::mDt_OtamaNo[i] == 0) {
                    mDemoID = fopAcM_create(PROC_E_OT, i | 0xff030000, &current.pos,
                                            fopAcM_GetRoomNo(this), &shape_angle, NULL, 0xff);
                }
            }
        }

        if (mpMorf->checkFrame(200.0f)) {
            mMode = 20;
            mCamCenter.set(100.0f, 300.0f, 350.0f);
            mCamEye.set(800.0f, 800.0f, -500.0f);
            mCamFovy = 70.0f;
            field_0x704 = 0;
        }
        break;

    case 20:
        if (field_0x704 < 10 && mpMorf->checkFrame(E_DT_n::eDt_ShakeFrameDemo[field_0x704])) {
            setStayOtamaFall();
            field_0x704++;
        }

        if (mpMorf->checkFrame(290.0f)) {
            setSwingEffect();
        }

        if (mpMorf->checkFrame(330.0f)) {
            mMode = 30;
            if (mDemoID != -1) {
                fopAcM_SearchByID(mDemoID, &otama);
                if (otama != NULL) {
                    vec1 = otama->current.pos;
                    vec1.y += 100.0f;
                    mCamCenter = vec1;
                    mCamEye.set(1000.0f, 300.0f, 1700.0f);
                    mCamFovy = 40.0f;
                }
            }
        }
        break;

    case 30:
        cLib_chaseF(&mCamFovy, 20.0f, 1.0f);
        if (mDemoID != -1) {
            fopAcM_SearchByID(mDemoID, &otama);
            if (otama != NULL) {
                vec1 = otama->current.pos;
                vec1.y += 100.0f;
                cLib_chasePos(&mCamCenter, vec1, 50.0f);
            }
        }

        if (mpMorf->isStop()) {
            setBck(ANM_WAIT_01, J3DFrameCtrl::EMode_LOOP, 3.0f, 1.0f);
            mMode = 32;
            mTimer = 45;
            player->changeOriginalDemo();
            player->changeDemoMode(34, 0, 0, 0);
        }
        break;

    case 32:
        cLib_chaseF(&mCamFovy, 20.0f, 1.0f);
        if (mDemoID != -1) {
            fopAcM_SearchByID(mDemoID, &otama);
            if (otama != NULL) {
                vec1 = otama->current.pos;
                vec1.y += 100.0f;
                cLib_chasePos(&mCamCenter, vec1, 50.0f);
            }
        }

        if (mTimer == 0) {
            setActionMode(ACT_WALK, 0);
            maxFallSpeed = l_HIO.mMaxFallSpeed;
            camera->Reset(mCamCenter, mCamEye);
            camera->Start();
            camera->SetTrimSize(0);
            dComIfGp_event_reset();
            attention_info.flags = fopAc_AttnFlag_BATTLE_e;
            Z2GetAudioMgr()->subBgmStart(Z2BGM_DEKUTOAD);
            fpcM_Search(s_demo_otama, this);
            return;
        }
        break;
    }

    camera->Set(mCamCenter, mCamEye, mCamFovy, mCamBank);
    dComIfGp_getEvent().onSkipFade();
    dComIfGp_getEvent().setSkipProc(this, DemoSkipCallBack, 0);
}

void daE_DT_c::calcCamera() {
    dBgS_LinChk lin_chk;
    cXyz start, end, offset_pos, offset;
    cXyz player_pos = daPy_getPlayerActorClass()->current.pos;
    camera_class* camera = dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0));
    cXyz cam_center, vec, cam_eye;
    s16 angle;

    switch (mDemoMode) {
    case 0:
        break;

    case 1:
    case 2:
        mDoMtx_stack_c::YrotS(fopAcM_searchPlayerAngleY(this));
        vec.set(0.0f, 100.0f, 400.0f);
        mDoMtx_stack_c::multVec(&vec, &cam_eye);
        cam_eye += player_pos;
        lin_chk.Set(&eyePos, &cam_eye, this);
        if (dComIfG_Bgsp().LineCross(&lin_chk)) {
            eyePos.x *= 0.5f;
            eyePos.z *= 0.5f;
            mDoMtx_stack_c::YrotS(fopAcM_searchPlayerAngleY(this));
            vec.set(0.0f, 150.0f, 100.0f);
            mDoMtx_stack_c::multVec(&vec, &cam_eye);
            cam_eye += player_pos;
        }

        if (mDemoMode == 1) {
            camera->mCamera.Stop();
            mDemoTimer = l_HIO.field_0x28;
            mCamEye = cam_eye;
            mCamCenter = eyePos;
            mDemoMode = 2;
            daPy_getPlayerActorClass()->onForceSubjectCancel();
            return;
        }

        cLib_chasePos(&mCamEye, cam_eye, 50.0f);
        cLib_chasePos(&mCamCenter, eyePos, 40.0f);
        if (mDemoTimer != 0) {
            camera->mCamera.U2((s16)cLib_targetAngleY(&mCamEye, &mCamCenter));
            camera->mCamera.Set(mCamCenter, mCamEye);
        } else {
            mDoMtx_stack_c::YrotS(fopAcM_searchPlayerAngleY(this));
            vec.set(0.0f, 150.0f, -500.0f);
            mDoMtx_stack_c::multVec(&vec, &mCamCenter);
            mCamCenter += player_pos;
            camera->mCamera.Reset(mCamCenter, mCamEye);
            camera->mCamera.Start();
            mDemoMode = 0;
        }
        break;

    case 3:
        camera->mCamera.Stop();
        mDemoMode = 4;
        break;

    case 4:
        field_0x73a = 0;
        offset.set(0.0f, 0.0f, 160.0f);
        cLib_offsetPos(&offset_pos, &current.pos, shape_angle.y, &offset);
        angle = (s16)cLib_targetAngleY(&offset_pos, &player_pos);
        mDoMtx_stack_c::YrotS(angle);
        vec.set(200.0f, 1200.0f, 600.0f);
        mDoMtx_stack_c::multVec(&vec, &end);
        end += player_pos;
        mDoMtx_stack_c::YrotS(angle);
        vec.set(0.0f, 0.0f, 100.0f);
        mDoMtx_stack_c::multVec(&vec, &start);
        start += offset_pos;
        lin_chk.Set(&start, &end, this);
        if (dComIfG_Bgsp().LineCross(&lin_chk)) {
            field_0x73a = 0x8000;
        }
        mDemoMode = 5;
        // fallthrough

    case 5:
        offset.set(0.0f, 0.0f, 160.0f);
        cLib_offsetPos(&offset_pos, &current.pos, shape_angle.y, &offset);
        angle = field_0x73a + cLib_targetAngleY(&offset_pos, &player_pos);
        mDoMtx_stack_c::YrotS(angle);
        vec.set(200.0f, 1200.0f, 600.0f);
        mDoMtx_stack_c::multVec(&vec, &cam_eye);
        cam_eye += player_pos;
        mDoMtx_stack_c::YrotS(angle);
        vec.set(0.0f, 0.0f, 100.0f);
        mDoMtx_stack_c::multVec(&vec, &cam_center);
        cam_center += offset_pos;
        camera->mCamera.Set(cam_center, cam_eye, 50.0f, 0);
        break;

    case 6:
        camera->mCamera.Start();
        mDemoMode = 0;
        break;
    }
}

void daE_DT_c::calcBoxAndTuba() {
    if (mChestMode == 0) {
        return;
    }

    mChestSpeed.y += -5.0f;
    if (mChestSpeed.y < -80.0f) {
        mChestSpeed.y = -80.0f;
    }
    mChestPos += mChestSpeed;
    if (mChestPos.y <= 0.0f) {
        mChestPos.y = 0.0f;
    }

    switch (mChestMode) {
    case 1:
        mChestAngle.x += 0x400;
        if (mChestPos.y <= 0.0f) {
            mChestAngle.x = -0x2000;
            mChestSpeed.x = 1.2f;
            mChestSpeed.y = 30.0f;
            mChestSpeed.z = 9.0f;
            mChestMode = 2;
            setBoxTubaLandEffect();
            mDoAud_seStart(Z2SE_CM_BODYFALL_ASASE_M, &mChestPos, 0, 0);
        } else {
            s16 angle = 0x4000 - cM_atan2s(mChestSpeed.y, mChestSpeed.absXZ());
            cLib_addCalcAngleS(&mSpitAngle[0].x, angle, 8, 0x1000, 0x100);
            cLib_addCalcAngleS(&mSpitAngle[1].x, mSpitAngle[0].x, 8, 0x1000, 0x100);
            cLib_addCalcAngleS(&mSpitAngle[2].x, mSpitAngle[1].x, 8, 0x1000, 0x100);
            mSpitAngle[0].y = mSpitAngle[1].y = mSpitAngle[2].y = 0;
            field_0x6dc += 0x800;
            mSpitAngle[0].z = cM_ssin(field_0x6dc) * 0x800;
            mSpitAngle[1].z = cM_ssin(field_0x6dc + 0x4000) * 0x1000;
            mSpitAngle[2].z = cM_ssin(field_0x6dc + 0x8000) * 0x1800;
            field_0x6de += 0x1000;
            mSpitScale[0] = cM_ssin(field_0x6de) * 0.2f + 1.0f;
            mSpitScale[1] = cM_ssin(field_0x6de + 0x4000) * 0.1f + 1.0f;
            mSpitScale[2] = cM_ssin(field_0x6de + 0x8000) * 0.05f + 1.0f;
            mSpitScaleX = 1.0f;
            mSpitOffset = 80.0f;
            mSpitFade[0] = 1.0f;
        }
        break;

    case 2:
        cLib_chaseAngleS(&mChestAngle.x, 0, 0x400);
        if (mChestSpeed.y < 0.0f && mChestPos.y <= 0.0f) {
            mChestAngle.x = 0;
            mChestSpeed.set(0.0f, 0.0f, 0.0f);
            mChestPos.set(0.0f, 0.0f, 0.0f);
            if (mSwBit != 0xff && !dComIfGs_isSwitch(mSwBit, fopAcM_GetRoomNo(this))) {
                dComIfGs_onSwitch(mSwBit, fopAcM_GetRoomNo(this));
            }
            mChestMode = 3;
            mSpitTimer = 50;
        }
        cLib_addCalcAngleS(&mSpitAngle[0].x, 0, 0x10, 0x2000, 0x80);
        cLib_addCalcAngleS(&mSpitAngle[1].x, mSpitAngle[0].x, 0x10, 0x2000, 0x80);
        cLib_addCalcAngleS(&mSpitAngle[2].x, mSpitAngle[1].x, 0x10, 0x2000, 0x80);
        cLib_chaseF(&mSpitScale[0], 2.0f, 0.0f);
        cLib_chaseF(&mSpitScale[1], 3.0f, 0.02f);
        cLib_chaseF(&mSpitScale[2], 1.0f, 0.02f);
        cLib_chaseF(&mSpitOffset, 100.0f, 1.0f);
        break;

    case 3:
    case 4:
        cLib_addCalcAngleS(&mSpitAngle[0].x, 0, 0x10, 0x2000, 0x80);
        cLib_addCalcAngleS(&mSpitAngle[1].x, mSpitAngle[0].x, 0x10, 0x2000, 0x80);
        cLib_addCalcAngleS(&mSpitAngle[2].x, mSpitAngle[1].x, 0x10, 0x2000, 0x80);
        cLib_chaseF(&mSpitScale[0], 5.0f, 0.01f);
        cLib_chaseF(&mSpitScale[1], 5.0f, 0.02f);
        cLib_chaseF(&mSpitScale[2], 1.0f, 0.02f);
        cLib_chaseF(&mSpitOffset, -100.0f, 1.0f);
        if (mChestMode == 3) {
            if (mSpitTimer == 30) {
                mSpitFade[1] = mSpitFade[2] = mSpitFade[3] = 1.0f;
            }
            if (mSpitTimer == 0) {
                mChestMode = 4;
                mSpitTimer = 100;
            }
        } else {
            if (mSpitTimer < 70) {
                cLib_chaseF(&mSpitFade[0], 0.0f, 0.01f);
                if (mSpitTimer < 30) {
                    cLib_chaseF(&mSpitFade[2], 0.0f, 0.02f);
                    if (mSpitTimer < 15) {
                        cLib_chaseF(&mSpitFade[1], 0.0f, 0.02f);
                        if (mSpitTimer == 0) {
                            cLib_chaseF(&mSpitFade[3], 0.0f, 0.02f);
                        }
                    }
                }
            }
        }
        break;
    }
}

void daE_DT_c::action() {
    mBodyCcSph[1].OnTgShield();
    if (mLinkPressed) {
        mBodyCcSph[0].OffCoSetBit();
        mBodyCcSph[1].OffCoSetBit();
        mLegCcSph[0].OffCoSetBit();
        mLegCcSph[1].OffCoSetBit();
    } else {
        mBodyCcSph[0].OnCoSetBit();
        mBodyCcSph[1].OnCoSetBit();
        mLegCcSph[0].OnCoSetBit();
        mLegCcSph[1].OnCoSetBit();
    }
    mTongueCcSph[0].OffCoSetBit();
    mTongueCcSph[0].OffTgSetBit();
    mTongueCcSph[1].OffCoSetBit();
    mTongueCcSph[1].OffTgSetBit();

    mEyePosMode = 0;
    mFlyingAttack = false;
    mFlyingAttackLand = false;
    field_0x73e = 0;
    field_0x740 = 0;

    checkWaterSurface();
    damage_check();

    cLib_chaseF(&field_0x748, 0.0f, 0.02f);
    field_0x74c += 0x2000;
    cLib_chaseAngleS(&field_0x74e, 0, 0x40);
    field_0x750 += 0x2000;
    cLib_chaseAngleS(&field_0x752, 0, 0x40);
    field_0x754 += 0x2000;

    BOOL boss_room_wait = true;
    switch (mAction) {
    case ACT_WAIT:
        executeWait();
        break;
    case ACT_CRY:
        executeCry();
        break;
    case ACT_SHAKE:
        executeShake();
        break;
    case ACT_WALK:
        executeWalk();
        break;
    case ACT_JUMP:
        executeJump();
        break;
    case ACT_DOWN:
        mBodyCcSph[1].OffTgShield();
        executeDown();
        break;
    case ACT_SEARCH:
        executeSearch();
        break;
    case ACT_DAMAGE:
        mBodyCcSph[1].OffTgShield();
        executeDamage();
        break;
    case ACT_BOMB_DAMAGE:
        executeBombDamage();
        break;
    case ACT_DEATH:
        executeDeath();
        break;
    case ACT_OPENING:
        executeOpening();
        boss_room_wait = false;
    }

    if (boss_room_wait) {
        daPy_getPlayerActorClass()->onBossRoomWait();
    }

    calcCamera();
    setStepEffect();
    fopAcM_posMoveF(this, NULL);
    if (!field_0x781) {
        mAcch.CrrPos(dComIfG_Bgsp());
    }
    mpMorf->play(0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
    calcBoxAndTuba();
    checkTongueBg();
    cLib_chaseAngleS(&field_0x742, field_0x73e, 0x100);
    cLib_chaseAngleS(&field_0x744, field_0x740, 0x100);
}

void daE_DT_c::mtx_set() {
    cXyz spit_pos;

    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::ZXYrotM(shape_angle);
    mDoMtx_stack_c::transM(0.0f, 0.0f, -100.0f);
    mDoMtx_stack_c::scaleM(mScale, mScale, mScale);
    mpMorf->getModel()->setBaseTRMtx(mDoMtx_stack_c::get());
    mpMorf->modelCalc();

    mDoMtx_stack_c::transS(mChestPos);
    mDoMtx_stack_c::ZXYrotM(mChestAngle);
    mpChestModel->setBaseTRMtx(mDoMtx_stack_c::get());
    mpSpitBModel->setBaseTRMtx(mDoMtx_stack_c::get());
    mDoMtx_stack_c::transM(0.0f, mSpitOffset, 0.0f);
    mDoMtx_stack_c::multVecZero(&spit_pos);

    mDoMtx_stack_c::transS(spit_pos);
    mDoMtx_stack_c::ZXYrotM(mSpitAngle[0]);
    mDoMtx_stack_c::scaleM(mSpitScale[0], 1.0f, mSpitScale[0]);
    mpSpitModel->setBaseTRMtx(mDoMtx_stack_c::get());
}

void daE_DT_c::cc_set() {
    cXyz center;
    J3DModel* model = mpMorf->getModel();

    switch (mEyePosMode) {
    case 0:
        mDoMtx_stack_c::copy(model->getAnmMtx(JNT_HEAD));
        mDoMtx_stack_c::transM(0.0f, 100.0f, 0.0f);
        mDoMtx_stack_c::multVecZero(&eyePos);
        break;

    case 1:
        mDoMtx_stack_c::copy(model->getAnmMtx(JNT_TONGUE_4));
        mDoMtx_stack_c::transM(30.0f, 0.0f, 0.0f);
        mDoMtx_stack_c::multVecZero(&eyePos);
        break;

    case 2:
        mDoMtx_stack_c::copy(model->getAnmMtx(JNT_TONGUE_3));
        mDoMtx_stack_c::multVecZero(&eyePos);
        break;

    case 3:
        eyePos = mChestPos;
        break;
    }

    attention_info.position = eyePos;
    attention_info.position.y += 70.0f;

    mDoMtx_stack_c::copy(model->getAnmMtx(JNT_BACKBONE_1));
    mDoMtx_stack_c::transM(110.0f, -100.0f, 0.0f);
    if (mAction == ACT_DOWN || (mAction == ACT_JUMP && mMode >= 7)) {
        if (!mLinkPressed) {
            mBodyCcSph[0].SetR(510.0f);
        } else {
            mBodyCcSph[0].SetR(360.0f);
        }
    } else {
        mBodyCcSph[0].SetR(400.0f);
    }
    mDoMtx_stack_c::multVecZero(&center);
    mBodyCcSph[0].SetC(center);
    dComIfG_Ccsp()->Set(&mBodyCcSph[0]);

    mDoMtx_stack_c::copy(model->getAnmMtx(JNT_BACKBONE_1));
    mDoMtx_stack_c::transM(110.0f, -100.0f, 0.0f);
    if (mAction == ACT_DOWN || (mAction == ACT_JUMP && mMode >= 7)) {
        if (!mLinkPressed) {
            mDoMtx_stack_c::transM(300.0f, -50.0f, 0.0f);
            mBodyCcSph[1].SetR(350.0f);
        } else {
            mDoMtx_stack_c::transM(300.0f, -30.0f, 0.0f);
            mBodyCcSph[1].SetR(370.0f);
        }
    } else {
        mDoMtx_stack_c::transM(220.0f, -20.0f, 0.0f);
        mBodyCcSph[1].SetR(350.0f);
    }
    mDoMtx_stack_c::multVecZero(&center);
    mBodyCcSph[1].SetC(center);
    dComIfG_Ccsp()->Set(&mBodyCcSph[1]);

    mDoMtx_stack_c::copy(model->getAnmMtx(JNT_TONGUE_4));
    mDoMtx_stack_c::transM(30.0f, 0.0f, 0.0f);
    mDoMtx_stack_c::multVecZero(&center);
    mTongueCcSph[0].SetC(center);
    mTongueCcSph[0].SetR(70.0f);
    dComIfG_Ccsp()->Set(&mTongueCcSph[0]);

    mDoMtx_stack_c::copy(model->getAnmMtx(JNT_TONGUE_4));
    mDoMtx_stack_c::transM(-30.0f, 0.0f, 0.0f);
    mDoMtx_stack_c::multVecZero(&center);
    mTongueCcSph[1].SetC(center);
    mTongueCcSph[1].SetR(70.0f);
    dComIfG_Ccsp()->Set(&mTongueCcSph[1]);

    mDoMtx_stack_c::copy(model->getAnmMtx(JNT_LEG_L1));
    mDoMtx_stack_c::transM(130.0f, 0.0f, 0.0f);
    mDoMtx_stack_c::multVecZero(&center);
    mLegCcSph[0].SetC(center);
    mLegCcSph[0].SetR(100.0f);
    dComIfG_Ccsp()->Set(&mLegCcSph[0]);

    mDoMtx_stack_c::copy(model->getAnmMtx(JNT_LEG_R1));
    mDoMtx_stack_c::transM(130.0f, 0.0f, 0.0f);
    mDoMtx_stack_c::multVecZero(&center);
    mLegCcSph[1].SetC(center);
    mLegCcSph[1].SetR(100.0f);
    dComIfG_Ccsp()->Set(&mLegCcSph[1]);
}

int daE_DT_c::execute() {
    if (mTimer != 0) {
        mTimer--;
    }
    if (mWalkTimer != 0) {
        mWalkTimer--;
    }
    if (mDamageTimer != 0) {
        mDamageTimer--;
    }
    if (mDemoTimer != 0) {
        mDemoTimer--;
    }
    if (mQuakeTimer != 0) {
        mQuakeTimer--;
        if (mQuakeTimer == 0) {
            dComIfGp_getVibration().StopQuake(0x1f);
        }
    }
    if (mBodyDamageTimer != 0) {
        mBodyDamageTimer--;
    }
    if (mLegLDamageTimer != 0) {
        mLegLDamageTimer--;
    }
    if (mLegRDamageTimer != 0) {
        mLegRDamageTimer--;
    }
    if (mSpitTimer != 0) {
        mSpitTimer--;
    }
    if (field_0x714 != 0) {
        field_0x714--;
    } else if (!dComIfGs_isOneZoneSwitch(2, fopAcM_GetRoomNo(this))) {
        dComIfGs_onOneZoneSwitch(2, fopAcM_GetRoomNo(this));
    }

    action();
    mtx_set();
    cc_set();
    return 1;
}

static int daE_DT_Execute(daE_DT_c* i_this) {
    return i_this->execute();
}

static int daE_DT_IsDelete(daE_DT_c* i_this) {
    return 1;
}

int daE_DT_c::_delete() {
    dComIfG_resDelete(&mPhase[0], "E_DT");
    dComIfG_resDelete(&mPhase[1], "Dalways");
    dComIfG_resDelete(&mPhase[2], "E_OT");
    if (mHIOInit) {
        hioInit = false;
    }
    if (heap != NULL) {
        mSound.deleteObject();
    }
    return 1;
}

static int daE_DT_Delete(daE_DT_c* i_this) {
    return i_this->_delete();
}

int daE_DT_c::CreateHeap() {
    J3DModelData* model_data = (J3DModelData*)dComIfG_getObjectRes("E_DT", 28);
    mpMorf = new mDoExt_McaMorfSO(model_data, NULL, NULL,
                                  (J3DAnmTransform*)dComIfG_getObjectRes("E_DT", 23),
                                  J3DFrameCtrl::EMode_NONE, 1.0f, 0, -1,
                                  &mSound, 0, 0x11000084);
    if (mpMorf == NULL || mpMorf->getModel() == NULL) {
        return 0;
    }
    J3DModel* model = mpMorf->getModel();
    model->setUserArea((uintptr_t)this);
    for (u16 i = 1; i < model->getModelData()->getJointNum(); i++) {
        model->getModelData()->getJointNodePointer(i)->setCallBack(JointCallBack);
    }

    model_data = (J3DModelData*)dComIfG_getObjectRes("Dalways", "BoxB.bmd");
    mpChestModel = mDoExt_J3DModel__create(model_data, 0x80000, 0x11000084);
    if (mpChestModel == NULL) {
        return 0;
    }

    model_data = (J3DModelData*)dComIfG_getObjectRes("E_DT", 29);
    mpSpitModel = mDoExt_J3DModel__create(model_data, 0, 0x11000084);
    if (mpSpitModel == NULL) {
        return 0;
    }
    mpSpitModel->setUserArea((uintptr_t)this);
    for (u16 i = 1; i < mpSpitModel->getModelData()->getJointNum(); i++) {
        mpSpitModel->getModelData()->getJointNodePointer(i)->setCallBack(JointCallBackTuba);
    }

    model_data = (J3DModelData*)dComIfG_getObjectRes("E_DT", 30);
    mpSpitBModel = mDoExt_J3DModel__create(model_data, 0, 0x11000084);
    if (mpSpitBModel == NULL) {
        return 0;
    }

    return 1;
}

static int useHeapInit(fopAc_ac_c* i_this) {
    return static_cast<daE_DT_c*>(i_this)->CreateHeap();
}

cPhs__Step daE_DT_c::create() {
    fopAcM_ct(this, daE_DT_c);

    cPhs__Step step = (cPhs__Step)dComIfG_resLoad(&mPhase[0], "E_DT");
    if (step != cPhs_COMPLEATE_e) {
        return step;
    }
    step = (cPhs__Step)dComIfG_resLoad(&mPhase[1], "Dalways");
    if (step != cPhs_COMPLEATE_e) {
        return step;
    }
    step = (cPhs__Step)dComIfG_resLoad(&mPhase[2], "E_OT");
    if (step != cPhs_COMPLEATE_e) {
        return step;
    }

    mSwBit = fopAcM_GetParam(this) & 0xff;
    if (dComIfGs_isSwitch(mSwBit, fopAcM_GetRoomNo(this))) {
        return cPhs_ERROR_e;
    }
    if (dComIfGs_isStageMiddleBoss()) {
        return cPhs_ERROR_e;
    }

    if (!fopAcM_entrySolidHeap(this, useHeapInit, 0xa840)) {
        return cPhs_ERROR_e;
    }

    if (!hioInit) {
        hioInit = true;
        mHIOInit = true;
        l_HIO.field_0x4 = -1;
    }

    attention_info.flags = fopAc_AttnFlag_BATTLE_e;
    fopAcM_SetMtx(this, mpMorf->getModel()->getBaseTRMtx());
    fopAcM_SetMin(this, -700.0f, -200.0f, -700.0f);
    fopAcM_SetMax(this, 700.0f, 500.0f, 700.0f);
    mAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1, &mAcchCir,
              fopAcM_GetSpeed_p(this), NULL, NULL);
    mAcchCir.SetWall(50.0f, 500.0f);
    health = 400;
    field_0x560 = 400;

    mCcStts.Init(0xff, 0, this);
    mBodyCcSph[0].Set(E_DT_n::cc_dt_body_src);
    mBodyCcSph[0].SetStts(&mCcStts);
    mBodyCcSph[1].Set(E_DT_n::cc_dt_body_src);
    mBodyCcSph[1].SetStts(&mCcStts);
    mLegCcSph[0].Set(E_DT_n::cc_dt_body_src);
    mLegCcSph[0].SetStts(&mCcStts);
    mLegCcSph[1].Set(E_DT_n::cc_dt_body_src);
    mLegCcSph[1].SetStts(&mCcStts);
    mTongueCcSph[0].Set(E_DT_n::cc_dt_tongue_src);
    mTongueCcSph[0].SetStts(&mCcStts);
    mTongueCcSph[1].Set(E_DT_n::cc_dt_tongue_src);
    mTongueCcSph[1].SetStts(&mCcStts);

    attention_info.distances[fopAc_attn_BATTLE_e] = 4;
    mSound.init(&current.pos, &eyePos, 3, 1);
    mAtInfo.mpSound = &mSound;
    mAtInfo.mPowerType = 1;
    gravity = 0.0f;
    maxFallSpeed = l_HIO.mMaxFallSpeed;
    mScale = l_HIO.mScale;

    if (daPy_getPlayerActorClass()->current.pos.y < -800.0f) {
        cDmr_SkipInfo = 0;
    }
    if (daPy_getPlayerActorClass()->current.pos.z > 3000.0f) {
        cDmr_SkipInfo = 0;
    }

    if (cDmr_SkipInfo != 0) {
        cDmr_SkipInfo = 0;
        gravity = -5.0f;
        setActionMode(ACT_WAIT, 0);
        maxFallSpeed = l_HIO.mMaxFallSpeed;
        attention_info.flags = fopAc_AttnFlag_BATTLE_e;
        current.pos.set(0.0f, 0.0f, -500.0f);
        shape_angle.y = cLib_targetAngleY(&current.pos, &daPy_getPlayerActorClass()->current.pos);
        Z2GetAudioMgr()->subBgmStart(Z2BGM_DEKUTOAD);
        dComIfGs_onOneZoneSwitch(3, fopAcM_GetRoomNo(this));
    } else {
        setActionMode(ACT_OPENING, 0);
        field_0x714 = 600;
    }

    daE_DT_Execute(this);
    return step;
}

static cPhs__Step daE_DT_Create(daE_DT_c* i_this) {
    return i_this->create();
}

static actor_method_class l_daE_DT_Method = {
    (process_method_func)daE_DT_Create,
    (process_method_func)daE_DT_Delete,
    (process_method_func)daE_DT_Execute,
    (process_method_func)daE_DT_IsDelete,
    (process_method_func)daE_DT_Draw,
};

extern actor_process_profile_definition g_profile_E_DT = {
  fpcLy_CURRENT_e,        // mLayerID
  7,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_E_DT,              // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(daE_DT_c),       // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  202,                    // mPriority
  &l_daE_DT_Method,       // sub_method
  0x00040000,             // mStatus
  fopAc_ENEMY_e,          // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};

AUDIO_INSTANCES
