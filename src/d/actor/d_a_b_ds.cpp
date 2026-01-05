/**
 * d_a_b_ds.cpp
 * Boss - Stallord
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_b_ds.h"
#include "d/actor/d_a_player.h"
#include "d/d_s_play.h"
#include "f_op/f_op_actor_mng.h"
#include "f_op/f_op_camera_mng.h"
#include "f_op/f_op_msg.h"
#include "f_op/f_op_msg_mng.h"
#include "d/actor/d_a_spinner.h"
#include "JSystem/J3DGraphBase/J3DMaterial.h"
#include "SSystem/SComponent/c_math.h"
#include "c/c_damagereaction.h"
#include "f_op/f_op_actor_enemy.h"
#include "Z2AudioLib/Z2Instances.h"

enum daB_DS_Joint {
    DS_JNT_BACKBONE1,
    DS_JNT_BACKBONE2,
    DS_JNT_BACKBONE3,
    DS_JNT_BACKBONE4,
    DS_JNT_NECK1,
    DS_JNT_NECK2,
    DS_JNT_HEAD,
    DS_JNT_JAW,
    DS_JNT_SHOULDER_BL,
    DS_JNT_ARM_L1,
    DS_JNT_ARM_L2,
    DS_JNT_HAND_L,
    DS_JNT_LYUBI_A1,
    DS_JNT_LYUBI_A2,
    DS_JNT_LYUBI_A3,
    DS_JNT_LYUBI_B1,
    DS_JNT_LYUBI_B2,
    DS_JNT_LYUBI_B3,
    DS_JNT_LYUBI_C1,
    DS_JNT_LYUBI_C2,
    DS_JNT_LYUBI_C3,
    DS_JNT_LYUBI_D1,
    DS_JNT_LYUBI_D2,
    DS_JNT_LYUBI_D3,
    DS_JNT_LYUBI_E1,
    DS_JNT_LYUBI_E2,
    DS_JNT_LYUBI_E3,
    DS_JNT_SHOULDER_BR,
    DS_JNT_ARM_R1,
    DS_JNT_ARM_R2,
    DS_JNT_HAND_R,
    DS_JNT_RYUBI_A1,
    DS_JNT_RYUBI_A2,
    DS_JNT_RYUBI_A3,
    DS_JNT_RYUBI_B1,
    DS_JNT_RYUBI_B2,
    DS_JNT_RYUBI_B3,
    DS_JNT_RYUBI_C1,
    DS_JNT_RYUBI_C2,
    DS_JNT_RYUBI_C3,
    DS_JNT_RYUBI_D1,
    DS_JNT_RYUBI_D2,
    DS_JNT_RYUBI_D3,
    DS_JNT_RYUBI_E1,
    DS_JNT_RYUBI_E2,
    DS_JNT_RYUBI_E3,
    DS_JNT_SHOULDER_L,
    DS_JNT_SHOULDER_R,
};

enum daB_DS_head_Joint {
    DS_HEAD_JNT_HEAD,
    DS_HEAD_JNT_FUR_B,
    DS_HEAD_JNT_FUR_L1,
    DS_HEAD_JNT_FUR_L2,
    DS_HEAD_JNT_FUR_R1,
    DS_HEAD_JNT_FUR_R2,
    DS_HEAD_JNT_JAW,
};

namespace {
static dCcD_SrcSph cc_ds_week_src = {
    {
        0,
        {{0, 0, 0}, {0x00fbfdff, 0x43}, {0x75}},  // mObj
        {0, 0, 0, 0, 0},                          // mGObjAt
        {0, 2, 0, 0, 0x303},                      // mGObjTg
        {0},                                      // mGObjCo
    },                                            // mObjInf
    {{
        {0.0f, 0.0f, 0.0f},  // mCenter
        40.0f,               // mRadius
    }}                       // mSph
};

static dCcD_SrcSph cc_ds_head_src = {
    {
        0,
        {{0, 0, 0}, {0xd8fbfdff, 3}, {0x55}},  // mObj
        {0xa, 0, 0, 0, 0},                     // mGObjAt
        {0, 2, 0, 0, 0x303},                   // mGObjTg
        {0},                                   // mGObjCo
    },                                         // mObjInf
    {{
        {0.0f, 0.0f, 0.0f},  // mCenter
        40.0f,               // mRadius
    }}                       // mSph
};

static dCcD_SrcSph cc_ds_body_src = {
    {
        0,
        {{0, 0, 0}, {0xd8fbfdff, 0x43}, {0x75}},  // mObj
        {0, 0, 0, 0, 0},                          // mGObjAt
        {0xa, 2, 0, 0, 0x303},                    // mGObjTg
        {0},                                      // mGObjCo
    },                                            // mObjInf
    {{
        {0.0f, 0.0f, 0.0f},  // mCenter
        40.0f,               // mRadius
    }}                       // mSph
};

static dCcD_SrcCyl cc_ds_backbone_src = {
    {
        0,
        {{0, 0, 0}, {0xd8fbfdff, 0x43}, {0x55}},  // mObj
        {0, 0, 0, 0, 0},                          // mGObjAt
        {0xa, 2, 0, 0, 0x307},                    // mGObjTg
        {0},                                      // mGObjCo
    },                                            // mObjInf
    {
        {
            {0.0f, 0.0f, 0.0f},  // mCenter
            30.0f,               // mRadius
            130.0f,              // mHeight
        }                        // mCyl
    }                            // mCylAttr
};

static dCcD_SrcCyl cc_ds_hand_at_cyl_src = {
    {
        0,
        {{0x100000, 4, 0x1f}, {0xd8fbfdff, 3}, {0x75}},  // mObj
        {0xa, 0, 0xe, 0, 0},                             // mGObjAt
        {0, 2, 0, 0, 0x307},                             // mGObjTg
        {0},                                             // mGObjCo
    },                                                   // mObjInf
    {
        {
            {0.0f, 0.0f, 0.0f},  // mCenter
            30.0f,               // mRadius
            130.0f,              // mHeight
        }                        // mCyl
    }                            // mCylAttr
};

static dCcD_SrcSph cc_ds_breath_at_src = {
    {
        0,
        {{0x100000, 3, 0xd}, {0, 0}, {0}},  // mObj
        {0xd, 0, 0xd, 0, 0},                // mGObjAt
        {0, 0, 0, 0, 0},                    // mGObjTg
        {0},                                // mGObjCo
    },                                      // mObjInf
    {{
        {0.0f, 0.0f, 0.0f},  // mCenter
        40.0f,               // mRadius
    }}                       // mSph
};
};  // namespace

daB_DS_HIO_c::daB_DS_HIO_c() {
    field_0x04 = -1;
    mModelSize = 1.0f;
    mHandRange = 2000.0f;
    mBreathRange = 3900.0f;
    mGuardSpawnRange = 2000.0f;
    mNoSearchRange = 2500.0f;
    mP2MoveAxis = 2800.0f;
    mP2AttackHeight = 700.0f;
    mP2ModelSize = 5.0f;
    mP2FallTime = 250;
    mPedestalFallTime = 200;
    mSandFallWaitTime = 50;
    mP2BulletFireTime = 5;
    mP2OuterWallAttackTime = 850;
    mP2TrapCreateWaitTime1 = 0;
    mP2TrapCreateWaitTime2 = 300;
    mP2TrapCreateID1 = 70;
    mP2TrapCreateID2 = 10;
    mHintTime1 = 600;
    mHintTime2 = 600;
    mP2OuterWallTrapSpeed = 12;
    mTowerTrapSpeed = 14;
    mP2ApproachAccel = 5;
    mP2ApproachSpeedMax = 100;
    mP2ApproachAfterBullet = 0x400;
    mP2ApproachDist = 0x1800;
    mP2Health = 1080;
    mP2HealthDebugOn = false;
}

static bool hio_set;

static daB_DS_HIO_c l_HIO;

static s16 handL_ang;
static s16 handR_ang;

static s16 handX_ang;
static u8 breathTimerBase;

int daB_DS_c::ctrlJoint(J3DJoint* i_joint, J3DModel* i_model) {
    cXyz sp40;
    cXyz sp4C;
    csXyz rot;

    u16 joint_no = i_joint->getJntNo();
    mDoMtx_stack_c::copy(i_model->getAnmMtx(joint_no));

    if (mBossPhase == 0) {
        switch (joint_no) {
        case DS_JNT_NECK1:
            rot.x = 0;
            rot.y = FAST_DIV(mHeadAngle.y, 2);
            rot.z = mHeadAngle.x;
            mDoMtx_stack_c::ZXYrotM(rot);
            break;
        case DS_JNT_HEAD:
            rot.x = 0;
            rot.y = FAST_DIV(mHeadAngle.y, 2);
            rot.z = mHeadAngle.x;
            mDoMtx_stack_c::ZXYrotM(rot);
            break;
        case DS_JNT_ARM_L1:
            mDoMtx_stack_c::YrotM(handL_ang);
            mDoMtx_stack_c::XrotM(handX_ang);
            break;
        case DS_JNT_ARM_L2:
            mDoMtx_stack_c::YrotM(-handL_ang);
            break;
        case DS_JNT_ARM_R1:
            mDoMtx_stack_c::YrotM(-handR_ang);
            mDoMtx_stack_c::XrotM(-handX_ang);
            break;
        case DS_JNT_ARM_R2:
            mDoMtx_stack_c::YrotM(handR_ang);
            break;
        }
    }

    i_model->setAnmMtx(joint_no, mDoMtx_stack_c::get());
    MTXCopy(mDoMtx_stack_c::get(), J3DSys::mCurrentMtx);
    return 1;
}

int daB_DS_c::JointCallBack(J3DJoint* i_joint, int param_1) {
    if (param_1 == 0) {
        daB_DS_c* a_this = (daB_DS_c*)j3dSys.getModel()->getUserArea();
        if (a_this != NULL) {
            a_this->ctrlJoint(i_joint, j3dSys.getModel());
        }
    }

    return 1;
}

int daB_DS_c::draw() {
    if (arg0 == TYPE_BULLET_A || arg0 == TYPE_BULLET_B || arg0 == TYPE_BULLET_C ||
        mBossPhase == 100)
    {
        return 1;
    }

    J3DModel* model = mpMorf->getModel();
    g_env_light.settingTevStruct(0, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(model->mModelData, &tevStr);
    J3DModelData* model_data = model->getModelData();

    if (arg0 == TYPE_BATTLE_2) {
        if (!mDead) {
            if (model_data->getMaterialNodePointer(2) != NULL) {
                model_data->getMaterialNodePointer(2)->getTevColor(2)->a = (u8)mEyeColorAlpha;
                model_data->getMaterialNodePointer(1)->getTevKColor(1)->a = mCrackAlpha;
            }

            mpMorf->entryDL();

            cXyz shadow_pos;
            shadow_pos.set(current.pos.x, current.pos.y + 120.0f, current.pos.z);
            tevStr.field_0x344 = field_0x7f8;
            mShadowKey = dComIfGd_setShadow(mShadowKey, 0, model, &shadow_pos, 6000.0f, 0.0f,
                                            current.pos.y, mAcch.GetGroundH(), mAcch.m_gnd, &tevStr,
                                            0, 1.0f, dDlst_shadowControl_c::getSimpleTex());
        }

        if (!mNoDrawSword) {
            J3DModel* sword_model = mpSwordMorf->getModel();
            g_env_light.settingTevStruct(0, &mSwordPos, &tevStr);
            g_env_light.setLightTevColorType_MAJI(sword_model->mModelData, &tevStr);
            mpSwordBrkAnm->entry(sword_model->getModelData());
            mpSwordMorf->entryDL();
        }

        if (mPlayPatternAnm) {
            mpPatternBrkAnm->entry(mpPatternModel->getModelData());
            mpPatternBtkAnm->entry(mpPatternModel->getModelData());
            mDoExt_modelUpdateDL(mpPatternModel);
        }

        return 1;
    } else {
        if (model_data->getMaterialNodePointer(2) != NULL) {
            model_data->getMaterialNodePointer(2)->getTevColor(2)->a = (u8)mEyeColorAlpha;
            model_data->getMaterialNodePointer(1)->getTevKColor(1)->a = mCrackAlpha;
        }

        u8 backbone_hide_mat = 0;
        if (mBossPhase != 0) {
            backbone_hide_mat = 6;
        } else if (mBackboneLevel == 1) {
            backbone_hide_mat = 4;
        } else if (mBackboneLevel == 2) {
            backbone_hide_mat = 5;
        }

        if (mBackboneCrackAlpha[mBackboneLevel] != 255.0f) {
            J3DMaterial* material =
                model->getModelData()->getMaterialNodePointer((u8)(mBackboneLevel + 4));
            if (material != NULL) {
                material->getTevKColor(1)->a = mBackboneCrackAlpha[mBackboneLevel];
            }
        }

        if (mBackboneLevel != 0 && mBackboneLevel < 3) {
            for (int i = 4; i < backbone_hide_mat + 1; i++) {
                J3DShape* shape =
                    mpMorf->getModel()->getModelData()->getMaterialNodePointer((u8)i)->getShape();
                if (shape != NULL) {
                    shape->hide();
                }
            }
        } else {
            for (int i = 4; i < 7; i++) {
                J3DShape* shape =
                    mpMorf->getModel()->getModelData()->getMaterialNodePointer((u8)i)->getShape();
                if (shape != NULL) {
                    shape->show();
                }
            }
        }

        mpMorf->entryDL();

        if (!mNoDrawSword) {
            J3DModel* sword_model = mpSwordMorf->getModel();
            g_env_light.settingTevStruct(0, &mSwordPos, &tevStr);
            g_env_light.setLightTevColorType_MAJI(sword_model->mModelData, &tevStr);
            mpSwordBrkAnm->entry(sword_model->getModelData());
            mpSwordMorf->entryDL();
        }

        if (mPlayPatternAnm) {
            mpOpPatternBrkAnm->entry(mpOpPatternModel->getModelData());
            mpOpPatternBtkAnm->entry(mpOpPatternModel->getModelData());
            mDoExt_modelUpdateDL(mpOpPatternModel);
        }

        if (mDrawZant) {
            J3DModel* zant_model = mpZantMorf->getModel();
            g_env_light.settingTevStruct(3, &mSwordPos, &tevStr);
            g_env_light.setLightTevColorType_MAJI(zant_model->mModelData, &tevStr);

            J3DShape* shape =
                mpZantMorf->getModel()->getModelData()->getMaterialNodePointer(2)->getShape();
            if (shape != NULL) {
                if (!mDrawZantSword) {
                    shape->hide();
                } else {
                    shape->show();
                }
            }
            mpZantMorf->entryDL();
        }

        cXyz shadow_pos;
        shadow_pos.set(current.pos.x, current.pos.y + 1000.0f, current.pos.z);
        model = mpMorf->getModel();
        tevStr.field_0x344 = field_0x7f8;
        mShadowKey = dComIfGd_setShadow(mShadowKey, 0, model, &shadow_pos, 7000.0f, 0.0f,
                                        current.pos.y, mAcch.GetGroundH(), mAcch.m_gnd, &tevStr, 0,
                                        1.0f, dDlst_shadowControl_c::getSimpleTex());

        return 1;
    }
}

static int daB_DS_Draw(daB_DS_c* i_this) {
    return i_this->draw();
}

void daB_DS_c::setBck(int i_anmID, u8 i_attr, f32 i_morf, f32 i_rate) {
    J3DAnmTransform* anm = static_cast<J3DAnmTransform*>(dComIfG_getObjectRes("B_DS", i_anmID));
    mpMorf->setAnm(anm, i_attr, i_morf, i_rate, 0.0f, -1.0f);
    mAnmID = i_anmID;
}

void daB_DS_c::setActionMode(int i_action, int i_mode) {
    mAction = i_action;
    mMode = i_mode;
}

static u16 eff_smoke_id[2] = {0x8BC4, 0x8BC5};

void daB_DS_c::mSmokeSet() {
    if (mSmokeCount >= 20) {
        return;
    }

    if (mAcch.ChkGroundHit()) {
        if (field_0x84d == 0 && mAcch.GetGroundH() != -G_CM3D_F_INF) {
            cXyz particle_scale(1.0f, 1.0f, 1.0f);
            cXyz particle_pos(current.pos);
            csXyz particle_angle(shape_angle);

            particle_pos.y = mAcch.GetGroundH();
            particle_angle.z = 0;
            particle_angle.x = 0;

            mSmokeParticleKey[mSmokeCount] =
                dComIfGp_particle_set(mSmokeParticleKey[mSmokeCount], eff_smoke_id[1],
                                      &particle_pos, &particle_angle, &particle_scale);

            mSoundPos = current.pos;
            mSound.startCreatureSound(Z2SE_EN_DS_H_COL, 0, -1);

            field_0x84d = 1;
            mSmokeCount++;
        }
    } else {
        field_0x84d = 0;
    }
}

void daB_DS_c::mHeadAngle_Clear() {
    cLib_addCalcAngleS2(&mHeadAngle.x, 0, 20, 0x100);
    cLib_addCalcAngleS2(&mHeadAngle.y, 0, 20, 0x100);
    cLib_addCalcAngleS2(&mHeadAngle.z, 0, 20, 0x100);
}

void daB_DS_c::HandHitSoundSet(bool i_isLeft) {
    csXyz hit_angle;
    cXyz sp18, hit_pos;

    if (!i_isLeft) {
        sp18 = mHandPos[1] - *mHandAtRCyl.GetTgHitPosP();
        hit_pos = *mHandAtRCyl.GetTgHitPosP();
        hit_pos.y += 100.0f;
        mSoundPos = mHandPos[1];
    } else {
        sp18 = mHandPos[0] - *mHandAtLCyl.GetTgHitPosP();
        hit_pos = *mHandAtLCyl.GetTgHitPosP();
        hit_pos.y += 100.0f;
        mSoundPos = mHandPos[0];
    }

    hit_angle.x = 0;
    hit_angle.y = sp18.atan2sX_Z();
    hit_angle.z = 0;

    def_se_set(&mSound, mAtInfo.mpCollider, 2, NULL);
    dComIfGp_setHitMark(2, this, &hit_pos, &hit_angle, NULL, 0);
}

void daB_DS_c::handSPosSet(int i_hand) {
    cXyz chk_pos, particle_pos;
    csXyz particle_angle;
    dBgS_GndChk gnd_chk;

    chk_pos = mFingerPos[i_hand];
    chk_pos.y += 800.0f;
    gnd_chk.SetPos(&chk_pos);

    chk_pos.y = dComIfG_Bgsp().GroundCross(&gnd_chk);
    if (chk_pos.y == -G_CM3D_F_INF) {
        chk_pos.y = mFingerPos[i_hand].y;
    }
    particle_pos = chk_pos - mHandPos[i_hand];

    mDoMtx_YrotS(*calc_mtx, (s16)particle_pos.atan2sX_Z());

    chk_pos.x = 0.0f;
    chk_pos.y = 200.0f;
    chk_pos.z = 400.0f;
    MtxPosition(&chk_pos, &particle_pos);

    particle_pos += mHandPos[i_hand];
    particle_pos.y += 800.0f;
    gnd_chk.SetPos(&particle_pos);

    particle_pos.y = dComIfG_Bgsp().GroundCross(&gnd_chk);
    if (particle_pos.y == -G_CM3D_F_INF) {
        particle_pos.y = mHandPos[i_hand].y;
    }
    chk_pos = particle_pos - mHandPos[i_hand];

    particle_angle.x = chk_pos.atan2sY_XZ();
    particle_angle.y = particle_pos.atan2sX_Z();
    particle_angle.z = 0;

    cXyz particle_scale(1.0f, 1.0f, 1.0f);
    for (int i = 0; i < 2; i++) {
        mHandSmokeParticleKey[(i_hand << 1) + i] =
            dComIfGp_particle_set(mHandSmokeParticleKey[(i_hand << 1) + i], eff_smoke_id[i],
                                  &particle_pos, &particle_angle, &particle_scale);
    }

    dComIfGp_getVibration().StartShock(2, 0x4F, cXyz(0.0f, 1.0f, 0.0f));
}

void daB_DS_c::hand_smokeSet(u8 i_hand) {
    if (i_hand == 0 || i_hand == 1) {
        handSPosSet(0);
    }

    if (i_hand == 0 || i_hand == 2) {
        handSPosSet(1);
    }
}

void daB_DS_c::mZsMoveChk() {
    cXyz offset, zs_pos;

    fopAc_ac_c* staltroop;
    int indices[15];
    int count = 0;
    int i;

    for (i = 0; i < 15; i++) {
        indices[count] = 0;
        if (mStaltroopID[i] != 0) {
            if (fopAcM_SearchByID(mStaltroopID[i], &staltroop) == 0) {
                mStaltroopID[i] = 0;
            } else {
                indices[count] = i;
                count++;
            }
        }
    }

    if (count == 0) {
        return;
    }

    dBgS_GndChk gnd_chk;
    s16 angle_y = fopAcM_searchPlayerAngleY(this);

    for (i = 0; i < count; i++) {
        if (indices[i] == 0) {
            continue;
        }

        mDoMtx_YrotS(*calc_mtx, angle_y);

        offset.x = 0.0f;
        offset.y = 0.0f;
        offset.z = 1500.0f;
        offset.z += cM_rndF(500.0f);
        MtxPosition(&offset, &zs_pos);

        zs_pos += current.pos;
        zs_pos.y += 2000.0f;
        gnd_chk.SetPos(&zs_pos);

        if (zs_pos.y != -G_CM3D_F_INF) {
            zs_pos.y = dComIfG_Bgsp().GroundCross(&gnd_chk);
        } else {
            zs_pos.y = current.pos.y += 800.0f;
        }

        if (mStaltroopID[indices[i]] != 0 &&
            fopAcM_SearchByID(mStaltroopID[indices[i]], &staltroop) != 0 && staltroop != NULL)
        {
            staltroop->home.pos.x = zs_pos.x;
            staltroop->home.pos.y = zs_pos.y;
            staltroop->home.pos.z = zs_pos.z;
        }

        angle_y = fopAcM_searchPlayerAngleY(this);
        angle_y += (s16)cM_rndFX(0x2000);
    }

    if (!daPy_getPlayerActorClass()->checkSpinnerRide() || mAction == ACT_DAMAGE ||
        fopAcM_searchPlayerDistance(this) > l_HIO.mBreathRange)
    {
        mIsAppear = false;
        return;
    }

    mIsAppear = true;
}

void daB_DS_c::mZsMoveChk_Guard() {
    static s16 mGuardDt[5] = {-0x1800, 0xC00, -0xC00, 0, 0x1800};

    cXyz offset, zs_pos;
    s16 angle_to_player = fopAcM_searchPlayerAngleY(this);
    fopAc_ac_c* staltroop;
    dBgS_GndChk gnd_chk;

    s16 angle_y[5];

    int i = 0;
    int index = cM_rndF(5.0f);
    for (; i < 5; i++, index++) {
        if (index > 4) {
            index = 0;
        }

        angle_y[i] = angle_to_player;

        if (mGuardDt[index] != 0) {
            angle_y[i] += mGuardDt[index];
        }
    }

    for (int i = 0; i < 5; i++) {
        mDoMtx_YrotS(*calc_mtx, angle_y[i]);

        offset.x = 0.0f;
        offset.y = 0.0f;
        offset.z = 500.0f;
        MtxPosition(&offset, &zs_pos);

        zs_pos += current.pos;
        zs_pos.y += 2000.0f;
        gnd_chk.SetPos(&zs_pos);

        if (zs_pos.y != -G_CM3D_F_INF) {
            zs_pos.y = dComIfG_Bgsp().GroundCross(&gnd_chk);
        } else {
            zs_pos.y = current.pos.y += 800.0f;
        }

        if (mStaltroop2ID[i] != 0 && fopAcM_SearchByID(mStaltroop2ID[i], &staltroop) != 0 &&
            staltroop != NULL)
        {
            staltroop->home.pos.x = zs_pos.x;
            staltroop->home.pos.y = zs_pos.y;
            staltroop->home.pos.z = zs_pos.z;
        }
    }

    if (!daPy_getPlayerActorClass()->checkSpinnerRide() || mAction == ACT_DAMAGE ||
        fopAcM_searchPlayerDistance(this) > l_HIO.mBreathRange)
    {
        mIsAppearG = false;
        return;
    }

    if (fopAcM_searchPlayerDistance(this) < l_HIO.mGuardSpawnRange) {
        mIsAppearG = true;
    }
}

void daB_DS_c::mTrapScale() {
    f32 target_scale = 1.0f;
    if (mBossPhase != 0) {
        target_scale = 2.0f;
    } else if (mTrapID[0] == 0) {
        return;
    }

    for (int i = 0; i < 20; i++) {
        fopAc_ac_c* trap_actor;
        if (fopAcM_SearchByID(mTrapID[i], &trap_actor) != 0 && trap_actor != NULL) {
            cLib_addCalc2(&trap_actor->scale.x, target_scale, 0.7f, 0.5f);
            cLib_addCalc2(&trap_actor->scale.y, target_scale, 0.8f, 2.0f);
            cLib_addCalc2(&trap_actor->scale.z, target_scale, 0.7f, 0.5f);

            if (fabsf(trap_actor->scale.y - target_scale) < 0.1f) {
                trap_actor->scale.set(target_scale, target_scale, target_scale);
            }
        }
    }
}

void daB_DS_c::mClearTrap(bool i_delete) {
    for (int i = 0; i < 20; i++) {
        if (mTrapID[i] == 0) {
            continue;
        }

        fopAc_ac_c* trap_actor;
        if (fopAcM_SearchByID(mTrapID[i], &trap_actor) != 0 && trap_actor != NULL) {
            if (i_delete) {
                fopAcM_delete(trap_actor);
            } else {
                trap_actor->health = 0;
            }

            mTrapID[i] = 0;
        }
    }
}

void daB_DS_c::mCreateTrap(bool param_0) {
    daPy_py_c* pla = daPy_getPlayerActorClass();
    if (mBossPhase == 0) {
        if (cLib_calcTimer(&mBirthTrapTimerF) != 0 || mPedestalFallTimer != 0 ||
            mSandFallTimer != 0 || mAction == ACT_DAMAGE || mAction == ACT_OPENING_DEMO)
        {
            mClearTrap(true);
            return;
        }
    } else if (pla->current.pos.y < -1300.0f || pla->current.pos.y > l_HIO.mP2AttackHeight ||
               (mAction == ACT_CIRCLE && mMode >= 3 && mMode <= 4) || mOutTimer == 0 ||
               daPy_getPlayerActorClass()->getDamageWaitTimer() != 0 || mAction == ACT_WAIT ||
               mAction == ACT_DAMAGE || mAction == ACT_ETC_DAMAGE || mAction == ACT_HAND_ATTACK)
    {
        mTrapCreate = false;
        mClearTrap(false);
        return;
    }

    mTrapScale();

    if (mBossPhase == 0 && !param_0 && (fopAcM_searchPlayerAngleY(this) & 0xFFF) == 0) {
        return;
    }

    cXyz pos, vec;
    cXyz trap_scale(1.0f, 1.0f, 1.0f);
    u32 params;

    if (mBossPhase != 0 && health <= (int)l_HIO.mP2Health / 3 * 2 && mTrapCreate) {
        int i = 6;
        int trap_count = 0;
        for (; i < 20 && trap_count < 2; i++) {
            if (mTrapID[i] != 0) {
                fopAc_ac_c* actor;
                if (fopAcM_SearchByID(mTrapID[i], &actor) != 0 && actor != NULL) {
                    continue;
                }
                mTrapID[i] = 0;
            }

            params = 0x1F5003;
            params |= (int)l_HIO.mTowerTrapSpeed << 8;

            int trap_create_id;
            if (trap_count == 0) {
                trap_create_id = l_HIO.mP2TrapCreateID1;
                params |= l_HIO.mP2TrapCreateID1 << 0x18;
            } else {
                trap_create_id = l_HIO.mP2TrapCreateID2;
                params |= l_HIO.mP2TrapCreateID2 << 0x18;
            }

            dPath* path = dPath_GetRoomPath(3, fopAcM_GetRoomNo(this));
            if (path != NULL) {
                dPnt& point = path->m_points[trap_create_id];
                pos = point.m_position;
                vec = pos - current.pos;
                if (vec.abs() <= 200.0f) {
                    continue;
                }

                csXyz angle;
                angle.x = 0;
                angle.y = 0;
                angle.z = 0;
                mTrapID[i] = fopAcM_createChild(PROC_Obj_Lv6TogeTrap, fopAcM_GetID(this), params,
                                                &current.pos, fopAcM_GetRoomNo(this), &angle,
                                                &trap_scale, -1, NULL);
                trap_count++;
            }
        }
        mTrapCreate = false;
    }

    trap_scale.set(0.0f, 7.0f, 0.0f);
    pos.zero();
    csXyz angle;
    angle.y = 0;
    angle.z = 0;
    angle.x = 0;
    pos.y = 1708.0f;

    static s16 mBirthAngle01_dt[4] = {0x0000, 0x4000, -0x8000, -0x4000};
    static s16 mBirthAngle02_dt[3] = {0x0000, 0x5555, -0x5556};
    static f32 mBirthYpos02_dt[3] = {1150.0f, 350.0f, -450.0f};

    if (mBossPhase == 0) {
        int trap_max = 6;
        if (mBackboneLevel < 2) {
            trap_max = 4;
        }

        for (int i = 0; i < trap_max; i++) {
            if (mBackboneLevel < 2) {
                angle.y = mBirthAngle01_dt[i];
            }

            if (mTrapID[i] != 0) {
                continue;
            }

            mTrapID[i] =
                fopAcM_createChild(PROC_Obj_Lv6TogeTrap, fopAcM_GetID(this), 0x27104DFF, &pos,
                                   fopAcM_GetRoomNo(this), &angle, &trap_scale, -1, NULL);
            if (mBackboneLevel == 2) {
                angle.y += 0x2AAA;
            }
        }
    } else {
        pos.zero();

        if (health <= (int)l_HIO.mP2Health / 3) {
            params = 0x271040FF;
            params |= (int)l_HIO.mP2OuterWallTrapSpeed << 8;

            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 2; j++) {
                    int idx = (i << 1) + j;
                    if (mTrapID[idx] != 0) {
                        continue;
                    }

                    angle.y = mBirthAngle02_dt[i] + mBirthAngle01_dt[j * 2];
                    pos.y = mBirthYpos02_dt[i];
                    mTrapID[idx] =
                        fopAcM_createChild(PROC_Obj_Lv6TogeTrap, fopAcM_GetID(this), params, &pos,
                                           fopAcM_GetRoomNo(this), &angle, &trap_scale, -1, NULL);
                }
            }
        }
    }
}

void daB_DS_c::mChangeVer2() {
    if (bitSw != 0xFF) {
        fopAcM_offSwitch(this, bitSw);
    }

    mCrackAlpha = 0.0f;
    shape_angle.x = 0;
    mBossPhase = 1;
    gravity = 0.0f;
    mDead = false;

    mHandAtLCyl.OffTgSetBit();
    mHandAtRCyl.OffTgSetBit();
    mHandAtLCyl.OffCoSetBit();
    mHandAtRCyl.OffCoSetBit();
    mHandAtLCyl.OffAtSetBit();
    mHandAtRCyl.OffAtSetBit();

    mWeakSph.OffTgSetBit();
    mWeakSph.OffCoSetBit();
    mWeakSph.OffAtSetBit();
    mWeakSph.OnTgNoHitMark();
    mWeakSph.OffTgShield();
    mWeakSph.OffTgNoConHit();
    mWeakSph.OnTgStopNoConHit();
    mWeakSph.SetTgHitMark(CcG_Tg_UNK_MARK_0);

    mBackboneCyl.OffTgSetBit();
    mBackboneCyl.OffCoSetBit();

    for (int i = 0; i < 5; i++) {
        mHeadSph[i].SetTgType(0xd8fbfdff);
        mHeadSph[i].SetTgHitMark(CcG_Tg_UNK_MARK_2);
        mHeadSph[i].OnTgShield();
        mHeadSph[i].OffTgIronBallRebound();
        mHeadSph[i].OffTgSpinnerReflect();
        mHeadSph[i].OnTgNoHitMark();
    }

    dComIfGs_onZoneSwitch(6, fopAcM_GetRoomNo(this));
    dComIfGs_onZoneSwitch(8, fopAcM_GetRoomNo(this));

    mIsOpeningDemo = true;
    mBackboneLevel = 0;
    field_0x560 = health = l_HIO.mP2Health;
}

void daB_DS_c::damage_check() {
    if (health <= 1 || mBossPhase == 100) {
        return;
    }

    if (cLib_calcTimer(&mDamageTimer) != 0) {
        return;
    }

    if (mHandAtRCyl.ChkTgHit()) {
        mDamageTimer = 8;
        mAtInfo.mpCollider = mHandAtRCyl.GetTgHitObj();
        HandHitSoundSet(false);

        if (mAction != ACT_OPENING_DEMO && mHandAtRCyl.GetTgHitObj()->ChkAtType(AT_TYPE_SPINNER) &&
            (mAction != ACT_ETC_DAMAGE || mHitLocation != 0))
        {
            mAction = ACT_ETC_DAMAGE;
            mHitLocation = 0;
            mMode = 0;
        }

        mHandAtRCyl.ClrTgHit();
        return;
    }

    if (mHandAtLCyl.ChkTgHit()) {
        mDamageTimer = 8;
        mAtInfo.mpCollider = mHandAtLCyl.GetTgHitObj();
        HandHitSoundSet(true);

        if (mAction != ACT_OPENING_DEMO && mHandAtLCyl.GetTgHitObj()->ChkAtType(AT_TYPE_SPINNER) &&
            (mAction != ACT_ETC_DAMAGE || mHitLocation != 1))
        {
            mAction = ACT_ETC_DAMAGE;
            mHitLocation = 1;
            mMode = 1;
        }

        mHandAtLCyl.ClrTgHit();
        return;
    }

    for (int i = 0; i < 18; i++) {
        if (mEtcSph[i].ChkTgHit()) {
            mDamageTimer = 8;
            mAtInfo.mpCollider = mEtcSph[i].GetTgHitObj();
            mSoundPos = *mEtcSph[i].GetTgHitPosP();
            def_se_set(&mSound, mAtInfo.mpCollider, 2, NULL);
            mEtcSph[i].ClrTgHit();
            return;
        }
    }

    for (int i = 0; i < 5; i++) {
        if (mHeadSph[i].ChkTgHit()) {
            mDamageTimer = 8;
            mAtInfo.mpCollider = mHeadSph[i].GetTgHitObj();
            mSoundPos = mHeadPos;
            def_se_set(&mSound, mAtInfo.mpCollider, 2, NULL);

            if (mAction != ACT_OPENING_DEMO && (mAction != ACT_ETC_DAMAGE || mHitLocation != 2)) {
                mAction = ACT_ETC_DAMAGE;
                mHitLocation = 2;
                mMode = 2;
            }

            mHeadSph[i].ClrTgHit();
            return;
        }
    }

    if (mWeakSph.ChkTgHit()) {
        mDamageTimer = 8;
        mAtInfo.mpCollider = mWeakSph.GetTgHitObj();
        mSoundPos = mHeadPos;
        def_se_set(&mSound, mAtInfo.mpCollider, 2, NULL);

        if (mAction != ACT_OPENING_DEMO && (mAction != ACT_ETC_DAMAGE || mHitLocation != 2)) {
            mAction = ACT_ETC_DAMAGE;
            mHitLocation = 2;
            mMode = 2;
        }

        mWeakSph.ClrTgHit();
        return;
    }

    if (mAction == ACT_OPENING_DEMO) {
        return;
    }

    daPy_getPlayerActorClass()->onBossRoomWait();
    mCcStts.Move();

    if (mBackboneCyl.ChkTgHit()) {
        mAtInfo.mpCollider = mBackboneCyl.GetTgHitObj();
        cXyz vec = current.pos - *mBackboneCyl.GetTgHitPosP();
        cXyz hit_pos = *mBackboneCyl.GetTgHitPosP();
        csXyz hit_angle;
        hit_angle.x = 0;
        hit_angle.y = vec.atan2sX_Z();
        hit_angle.z = 0;
        mSoundPos = current.pos;

        if (mBackboneCyl.GetTgHitObj()->ChkAtType(AT_TYPE_SPINNER) &&
            daPy_getPlayerActorClass()->checkSpinnerTriggerAttack())
        {
            u8 se = static_cast<dCcD_GObjInf*>(mBackboneCyl.GetTgHitObj())->GetAtSe();
            mSound.startCollisionSE(dCcD_GObjInf::getHitSeID(se, 0), mAtInfo.field_0x18);
            dComIfGp_getVibration().StopQuake(0x4f);

            mBackboneCyl.SetTgHitMark(CcG_Tg_UNK_MARK_3);
            mHandAtLCyl.OffAtSetBit();
            mHandAtRCyl.OffAtSetBit();
            mHandAtLCyl.OffTgShield();
            mHandAtRCyl.OffTgShield();

            mModeTimer = 0;
            if (bitSw3 != 0xFF) {
                fopAcM_onSwitch(this, bitSw3);
            }

            def_se_set(&mSound, mAtInfo.mpCollider, 0x1f, NULL);
            dComIfGp_setHitMark(3, this, &hit_pos, &hit_angle, NULL, 0);

            mDamageTimer = 50;
            setActionMode(ACT_DAMAGE, 0);

            if (mBackboneLevel >= 2) {
                Z2GetAudioMgr()->bgmStop(0x1e, 0);
            }
        } else {
            def_se_set(&mSound, mAtInfo.mpCollider, 2, NULL);
            dComIfGp_setHitMark(2, this, &hit_pos, &hit_angle, NULL, 0);
        }
    }
}

void daB_DS_c::neck_set() {
    static f32 HeadAngleDt[3] = {-850.0f, -350.0f, -50.0f};
    cXyz mae;
    csXyz angl;
    angl.z = 0;
    angl.y = 0;
    angl.x = 0;

    if (mAction != ACT_OPENING_DEMO && mAction != ACT_DAMAGE) {
        daPy_py_c* pla = daPy_getPlayerActorClass();
        mae.x = pla->current.pos.x - mHeadPos.x;
        mae.z = pla->current.pos.z - mHeadPos.z;
        mae.y = pla->current.pos.y - (mHeadPos.y + HeadAngleDt[mBackboneLevel]);

        angl.x = -mae.atan2sY_XZ() * 2.0f;
        if (mAction == ACT_BREATH_ATTACK) {
            angl.x = mBh2AttackAngleF;
            angl.x += (s16)(mBackboneLevel * 200);
        }

        if (angl.x > 0x2000) {
            angl.x = 0x2000;
        } else if (angl.x < -0x400) {
            angl.x = -0x400;
        }

        cLib_addCalcAngleS2(&mHeadAngle.x, -angl.x, 20, 0x100);

        angl.y = shape_angle.y - mae.atan2sX_Z();
        if (angl.y > 0x3000) {
            angl.y = 0x3000;
        } else if (angl.y < -0x3000) {
            angl.y = -0x3000;
        }

        if (WREG_S(1)) {
            OS_REPORT("\n\n\n\n");
            OS_REPORT("mHeadPos.y %f\n", mHeadPos.y);
            OS_REPORT("angl.sx %x\n", angl.x);
            OS_REPORT("angl.sy %x\n", angl.y);
            // clang-format off
            OS_REPORT("(short)( shape_angle.sy - mae.atan2sX_Z() ) %x\n", (short)( shape_angle.y - mae.atan2sX_Z() ));
            // clang-format on
            OS_REPORT("\n\n\n\n");
        }

        if (abs((s16)(mHeadAngle.y - angl.y)) > 0x100) {
            if (abs((s16)(fopAcM_searchPlayerAngleY(this) - shape_angle.y)) < 0x4000 &&
                fopAcM_searchPlayerDistance(this) > l_HIO.mNoSearchRange)
            {
                cLib_addCalcAngleS2(&mHeadAngle.y, angl.y, 20, 0x200);
            } else {
                mHeadAngle_Clear();
            }
        }
    }
}

bool daB_DS_c::mCutTypeCheck() {
    daPy_py_c* pla = (daPy_py_c*)dComIfGp_getPlayer(0);

    if (pla->getCutType() == daPy_py_c::CUT_TYPE_TURN_RIGHT ||
        pla->getCutType() == daPy_py_c::CUT_TYPE_JUMP ||
        pla->getCutType() == daPy_py_c::CUT_TYPE_TWIRL ||
        pla->getCutType() == daPy_py_c::CUT_TYPE_TURN_LEFT)
    {
        return true;
    }

    if (pla->getCutType() == daPy_py_c::CUT_TYPE_LARGE_JUMP ||
        pla->getCutType() == daPy_py_c::CUT_TYPE_LARGE_JUMP_FINISH ||
        pla->getCutType() == daPy_py_c::CUT_TYPE_LARGE_TURN_LEFT ||
        pla->getCutType() == daPy_py_c::CUT_TYPE_LARGE_TURN_RIGHT ||
        pla->getCutType() == daPy_py_c::CUT_TYPE_MORTAL_DRAW_A ||
        pla->getCutType() == daPy_py_c::CUT_TYPE_MORTAL_DRAW_B)
    {
        return true;
    }

    if (pla->getCutType() == daPy_py_c::CUT_TYPE_HEAD_JUMP ||
        pla->getCutType() == daPy_py_c::CUT_TYPE_TWIRL)
    {
        return true;
    }

    return false;
}

bool daB_DS_c::startDemoCheck() {
    camera_class* camera = dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0));

    if (daPy_py_c::checkNowWolf()) {
        return false;
    }

    if (!eventInfo.checkCommandDemoAccrpt()) {
        fopAcM_orderPotentialEvent(this, 2, 0xFFFF, 0);
        eventInfo.onCondition(2);
        mCameraEye = dCam_getBody()->Center();
        mCameraCenter = dCam_getBody()->Eye();
        mCameraFovy = dCam_getBody()->Fovy();
        return false;
    } else {
        camera->mCamera.Stop();
        camera->mCamera.SetTrimSize(3);
        daPy_getPlayerActorClass()->changeOriginalDemo();
        return true;
    }
}

void daB_DS_c::setYoMessage(int i_msgIdx) {
    mMsgIdx = i_msgIdx;
    mMsgPcID = fopMsgM_messageSet(i_msgIdx, 1000);
}

bool daB_DS_c::doYoMessage() {
    if (mpMsg != NULL) {
        if (mpMsg->mode == 0xE) {
            mpMsg->mode = 0x10;
        } else if (mpMsg->mode == 0x12) {
            mpMsg->mode = 0x13;
            mMsgPcID = 0xFFFFFFFF;
            return true;
        }
    } else {
        mpMsg = fopMsgM_SearchByID(mMsgPcID);
    }

    return false;
}

void daB_DS_c::executeOpeningDemo() {
    camera_class* camera = dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0));
    daPy_py_c* pla = daPy_getPlayerActorClass();

    dBgS_GndChk gnd_chk;
    cXyz sp298, sp28C, sp280, sp274, sp268;
    csXyz particle_angle;
    sp280.zero();
    sp274.zero();

    static cXyz mOpPlayerDt[3] = {
        cXyz(0.0f, 1800.0f, 4900.0f),
        cXyz(342.0f, 690.0f, 1753.0f),
        cXyz(-20.0f, 1270.0f, 3083.0f),
    };

    static cXyz mOpCenterDt[15] = {
        cXyz(0.0f, 1910.0f, 4935.0f),    cXyz(-5.0f, 2011.0f, 4664.0f),
        cXyz(615.0f, 2410.0f, 2140.0f),  cXyz(131.0f, 1105.0f, 1092.0f),
        cXyz(-110.0f, 1160.0f, 1200.0f), cXyz(-50.0f, 1095.0f, 1300.0f),
        cXyz(-85.0f, 1150.0f, 1250.0f),  cXyz(10.0f, 1145.0f, 1420.0f),
        cXyz(0.0f, 0.0f, 0.0f),          cXyz(10.0f, 1060.0f, 1420.0f),
        cXyz(45.0f, 1065.0f, 1475.0f),   cXyz(235.0f, 1110.0f, 1835.0f),
        cXyz(205.0f, 795.0f, 1770.0f),   cXyz(45.0f, 905.0f, 1480.0f),
        cXyz(650.0f, 2800.0f, 2350.0f),
    };

    static cXyz mOpCenterBodyDt[2] = {
        cXyz(320.0f, 730.0f, 1270.0f),
        cXyz(-235.0f, 1345.0f, 2090.0f),
    };

    static cXyz mOpEyeDt[15] = {
        cXyz(0.0f, 1980.0f, 5300.0f),   cXyz(0.0f, 1934.0f, 5036.0f),
        cXyz(655.0f, 2810.0f, 2360.0f), cXyz(181.0f, 1190.0f, 844.0f),
        cXyz(25.0f, 1125.0f, 1450.0f),  cXyz(75.0f, 1010.0f, 1540.0f),
        cXyz(40.0f, 1030.0f, 1480.0f),  cXyz(150.0f, 1115.0f, 1670.0f),
        cXyz(0.0f, 0.0f, 0.0f),         cXyz(160.0f, 1000.0f, 1685.0f),
        cXyz(180.0f, 1110.0f, 1730.0f), cXyz(360.0f, 1220.0f, 2080.0f),
        cXyz(340.0f, 755.0f, 2020.0f),  cXyz(170.0f, 800.0f, 1710.0f),
        cXyz(695.0f, 3200.0f, 2570.0f),
    };

    static cXyz mOpEyeBodyDt[2] = {
        cXyz(600.0f, 710.0f, 1300.0f),
        cXyz(125.0f, 1500.0f, 3320.0f),
    };

    static u16 eff_znt_Disapp_id[2] = {0x8BBA, 0x8BBB};
    static u16 eff_znt_App_id[4] = {0x8BBC, 0x8BBD, 0x8BBE, 0x8BBF};
    static u16 eff_znt_SASI_id[3] = {0x8BC0, 0x8BC1, 0x8BC2};

    static const struct {
        u16 eff_id;
        int joint_no;
    } KIDOU_EFF_DT[16] = {
        {0x8BC6, 8},  {0x8BC7, 8},  {0x8BC8, 9}, {0x8BC9, 9}, {0x8BCA, 27}, {0x8BCB, 27},
        {0x8BCC, 28}, {0x8BCD, 28}, {0x8BCE, 3}, {0x8BCF, 3}, {0x8BD0, 11}, {0x8BD1, 11},
        {0x8BD2, 30}, {0x8BD3, 30}, {0x8BD4, 6}, {0x8BD5, 6},
    };

    mHintTimer1 = l_HIO.mHintTime1;

    switch (mMode) {
    case 0:
        for (int i = 0; i < 16; i++) {
            mStartingParticleKey[i] = 0;
        }
        setBck(ANM_OPDEMO, 0, 1.0f, 0.0f);
        mMode++;
        // fallthrough
    case 1:
    case 10:
        if (!startDemoCheck()) {
            break;
        }

        Z2GetAudioMgr()->setDemoName("force_start");

        if (mMode == 10) {
            dComIfGp_getEvent()->startCheckSkipEdge(this);

            sp298.set(mOpPlayerDt[1]);
            daPy_getPlayerActorClass()->changeDemoMode(4, 2, 0, 0);

            sp298.set(pla->current.pos);
            sp28C = mZantPos - pla->current.pos;
            daPy_getPlayerActorClass()->setPlayerPosAndAngle(&sp298, sp28C.atan2sX_Z(), 0);

            mCameraCenter.set(mOpCenterDt[14]);
            mCameraEye.set(mOpEyeDt[14]);
            mMode++;
        } else {
            daPy_getPlayerActorClass()->changeDemoMode(20, 0, 0, 0);

            sp298.set(mOpPlayerDt[0]);
            daPy_getPlayerActorClass()->setPlayerPosAndAngle(&sp298, -0x8000, 0);

            sp280.set(sp298.x, sp298.y, 4700.0f);
            pla->changeDemoPos0(&sp280);

            mCameraCenter.set(mOpCenterDt[0]);
            mCameraEye.set(mOpEyeDt[0]);
            mModeTimer = 10;
            mMode++;
        }
        break;
    case 2: {
        f32 calc_center = cLib_addCalcPos(&mCameraCenter, mOpCenterDt[1], 0.3f, 2.0f, 1.0f);
        f32 calc_eye = cLib_addCalcPos(&mCameraEye, mOpEyeDt[1], 0.3f, 2.0f, 1.0f);
        if (calc_center > 2.0f || calc_eye > 2.0f || cLib_calcTimer(&mModeTimer) != 0) {
            break;
        }

        mCameraCenter.set(mOpCenterDt[1]);
        mCameraEye.set(mOpEyeDt[1]);
        sp280.set(0.0f, 1900.0f, 4727.0f);
        sp274.set(0.0f, 1981.0f, 5018.0f);
        camera->mCamera.Reset(sp280, sp274);
        camera->mCamera.Start();
        camera->mCamera.SetTrimSize(0);

        dComIfGp_event_reset();
        dComIfGs_onZoneSwitch(0, fopAcM_GetRoomNo(this));
        fopAcM_OffStatus(this, 0x4000);
        mMode++;
        // fallthrough
    }
    case 3:
        mPedestalFallTimer = l_HIO.mPedestalFallTime;

        // supposed to be daPy_py_c::checkNowWolf, but not getting inlined. fix later
        if (!dComIfGp_getLinkPlayer()->checkWolf() && pla->current.pos.z > 1800.0f &&
            pla->current.pos.z < 2200.0f && pla->current.pos.y > 780.0f &&
            pla->current.pos.x > -420.0f && pla->current.pos.x < 350.0f)
        {
            dComIfGs_onOneZoneSwitch(8, fopAcM_GetRoomNo(this));
            mMode = 10;
        }
        break;
    case 11:
        daPy_getPlayerActorClass()->changeDemoMode(20, 0, 0, 0);
        Z2GetAudioMgr()->seStart(Z2SE_EN_ZAN_L4_V, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
        mModeTimer = 150;
        mMode++;
        // fallthrough
    case 12:
        if (cLib_calcTimer(&mModeTimer) != 0) {
            cLib_addCalcPos(&mCameraCenter, mOpCenterDt[2], 0.3f, 4.0f, 2.0f);
            cLib_addCalcPos(&mCameraEye, mOpEyeDt[2], 0.3f, 4.0f, 2.0f);
            break;
        }
        daPy_getPlayerActorClass()->changeDemoMode(1, 0, 0, 0);
        mModeTimer = 60;
        mMode++;
        // fallthrough
    case 13:
        mCameraCenter.set(mOpCenterDt[3]);
        mCameraEye.set(mOpEyeDt[3]);
        if (cLib_calcTimer(&mModeTimer) != 0) {
            break;
        }

        mpZantMorf->setAnm(static_cast<J3DAnmTransform*>(dComIfG_getObjectRes("B_DS", 66)),
                           J3DFrameCtrl::EMode_LOOP, 1.0f, 1.0f, 0.0f, -1.0f);
        mZantScale.set(0.0f, 5.0f, 0.0f);
        mZantEyePos.set(mZantPos);
        mZantEyePos.y += 200.0f;

        daPy_getPlayerActorClass()->changeDemoMode(47, 1, 0, 0);

        particle_angle.x = 0;
        particle_angle.y = field_0x7ca + 5000;
        particle_angle.z = 0;
        dComIfGp_particle_set(0x8bb9, &mZantPos, &particle_angle, NULL);

        mSoundPos.set(current.pos);
        mSound.startCreatureSound(Z2SE_EN_ZAN_L4_WARP_IN, 0, -1);

        mDrawZant = true;
        mModeTimer = 10;
        mMode++;
        break;
    case 14:
        cLib_addCalc2(&mZantScale.x, 1.0f, 0.7f, 0.1f);
        cLib_addCalc2(&mZantScale.y, 1.0f, 0.7f, 0.7f);
        cLib_addCalc2(&mZantScale.z, 1.0f, 0.7f, 0.1f);
        if (cLib_calcTimer(&mModeTimer) != 0) {
            break;
        }

        mZantScale.set(1.0f, 1.0f, 1.0f);
        daPy_getPlayerActorClass()->changeDemoMode(23, 1, 2, 0);
        mModeTimer = 100;
        mMode++;
        // fallthrough
    case 15:
        if (cLib_calcTimer(&mModeTimer) != 0) {
            if (mModeTimer == 86) {
                Z2GetAudioMgr()->bgmStreamPrepare(0x2000047);
                Z2GetAudioMgr()->bgmStreamPlay();
            }

            if (mModeTimer == 1) {
                setYoMessage(0x1F41);
            }
        } else if (doYoMessage()) {
            mMode++;
        }
        break;
    case 16:
        setYoMessage(0x1F42);
        mMode++;
        break;
    case 17:
        if (!doYoMessage()) {
            break;
        }

        mCameraCenter.set(mOpCenterDt[4]);
        mCameraEye.set(mOpEyeDt[4]);
        mModeTimer = 30;
        mMode++;
        // fallthrough
    case 18:
        if (cLib_calcTimer(&mModeTimer) != 0) {
            if (mModeTimer == 1) {
                setYoMessage(0x1F43);
            }
        } else {
            if (doYoMessage()) {
                mMode = 20;
            }
        }
        break;
    case 20:
        mCameraCenter.set(mOpCenterDt[4]);
        mCameraEye.set(mOpEyeDt[4]);

        mpZantMorf->setAnm(static_cast<J3DAnmTransform*>(dComIfG_getObjectRes("B_DS", 64)),
                           J3DFrameCtrl::EMode_NONE, 1.0f, 1.0f, 0.0f, -1.0f);
        mpSwordMorf->setAnm(static_cast<J3DAnmTransform*>(dComIfG_getObjectRes("B_DS", 63)),
                            J3DFrameCtrl::EMode_NONE, 1.0f, 1.0f, 0.0f, -1.0f);
        mpSwordBrkAnm->init(mpSwordMorf->getModel()->getModelData(),
                            static_cast<J3DAnmTevRegKey*>(dComIfG_getObjectRes("B_DS", 81)), TRUE,
                            J3DFrameCtrl::EMode_NONE, 1.0f, 0, -1);

        particle_angle.x = 0;
        particle_angle.y = field_0x7ca + 5000;
        particle_angle.z = 0;

        for (int i = 0; i < 4; i++) {
            dComIfGp_particle_set(eff_znt_App_id[i], &mZantPos, &particle_angle, NULL);
        }

        for (int i = 0; i < 3; i++) {
            dComIfGp_particle_set(eff_znt_SASI_id[i], &mZantPos, &particle_angle, NULL);
        }

        mSound.startCreatureSound(Z2SE_EN_ZAN_L4_1, 0, -1);

        mNoDrawSword = true;
        mDrawZantSword = false;
        mMode++;
        // fallthrough
    case 21:
        cLib_addCalcPos(&mCameraCenter, mOpCenterDt[5], 1.0f, 20.0f, 10.0f);
        cLib_addCalcPos(&mCameraEye, mOpEyeDt[5], 1.0f, 20.0f, 10.0f);

        if (mpZantMorf->checkFrame(9.0f)) {
            mDrawZantSword = true;
        }

        if ((int)mpZantMorf->getFrame() < 50) {
            break;
        }
        mMode++;
        // fallthrough
    case 22:
        cLib_addCalcPos(&mCameraCenter, mOpCenterDt[6], 0.3f, 1.0f, 0.5f);
        cLib_addCalcPos(&mCameraEye, mOpEyeDt[6], 0.3f, 1.0f, 0.5f);

        if ((int)mpZantMorf->getFrame() < 191) {
            break;
        }

        mModeTimer = 30;
        mNoDrawSword = false;
        mMode++;
        // fallthrough
    case 23:
        cLib_addCalcPos(&mCameraCenter, mOpCenterDt[7], 0.7f, 30.0f, 20.0f);
        cLib_addCalcPos(&mCameraEye, mOpEyeDt[7], 0.7f, 30.0f, 20.0f);
        if (cLib_calcTimer(&mModeTimer) != 0) {
            break;
        }

        mMode++;
        // fallthrough
    case 24:
        if ((int)mpZantMorf->getFrame() >= 250) {
            cLib_addCalcPos(&mCameraCenter, mOpCenterDt[9], 0.7f, 4.0f, 1.0f);
            cLib_addCalcPos(&mCameraEye, mOpEyeDt[9], 0.7f, 4.0f, 1.0f);
        }

        if ((int)mpZantMorf->getFrame() < 340) {
            break;
        }

        mMode++;
        // fallthrough
    case 25:
        cLib_addCalcPos(&mCameraCenter, mOpCenterDt[10], 0.7f, 30.0f, 20.0f);
        cLib_addCalcPos(&mCameraEye, mOpEyeDt[10], 0.7f, 30.0f, 20.0f);
        if ((int)mpZantMorf->getFrame() < 346) {
            break;
        }

        field_0x85e = false;
        mChkHigh = 0.0f;
        mModeTimer = 100;
        dComIfGp_getVibration().StartShock(4, 0x1f, cXyz(0.0f, 1.0f, 0.0f));
        mMode++;
        // fallthrough
    case 26: {
        if (!field_0x85e && mpZantMorf->isStop()) {
            mpZantMorf->setAnm(static_cast<J3DAnmTransform*>(dComIfG_getObjectRes("B_DS", 67)),
                               J3DFrameCtrl::EMode_LOOP, 1.0f, 1.0f, 0.0f, -1.0f);
            field_0x85e = true;
        }

        cLib_addCalc0(&mCrackAlpha, 0.7f, 20.0f);
        cLib_addCalc2(&mChkHigh, 1.0f, 0.1f, 0.01f);

        sp298 = mCameraCenter - mOpCenterDt[11];
        f32 step = mChkHigh * sp298.abs();
        cLib_addCalcPos(&mCameraCenter, mOpCenterDt[11], mChkHigh, step, step);

        sp298 = mCameraEye - mOpEyeDt[11];
        step = mChkHigh * sp298.abs();
        cLib_addCalcPos(&mCameraEye, mOpEyeDt[11], mChkHigh, step, step);

        if (mModeTimer == 100) {
            mpOpPatternBrkAnm->init(mpOpPatternModel->getModelData(),
                                    static_cast<J3DAnmTevRegKey*>(dComIfG_getObjectRes("B_DS", 79)),
                                    TRUE, J3DFrameCtrl::EMode_NONE, 1.0f, 0, -1);

            mpOpPatternBtkAnm->init(
                mpOpPatternModel->getModelData(),
                static_cast<J3DAnmTextureSRTKey*>(dComIfG_getObjectRes("B_DS", 85)), TRUE,
                J3DFrameCtrl::EMode_NONE, 1.0f, 0, -1);
            mPlayPatternAnm = true;
        }

        if (cLib_calcTimer(&mModeTimer) != 0) {
            if (mModeTimer == 30) {
                mPlayPatternAnm = false;
            }
            break;
        }

        mpZantMorf->setAnm(static_cast<J3DAnmTransform*>(dComIfG_getObjectRes("B_DS", 65)),
                           J3DFrameCtrl::EMode_NONE, 1.0f, 1.0f, 0.0f, -1.0f);
        mSound.startCreatureSound(Z2SE_EN_ZAN_L4_2, 0, -1);
        mMode = 30;
        field_0x85e = false;
        // fallthrough
    }
    case 30:
        mCameraCenter.set(mOpCenterDt[12]);
        mCameraEye.set(mOpEyeDt[12]);
        if ((int)mpZantMorf->getFrame() < 65) {
            break;
        }
        mMode++;
        // fallthrough
    case 31:
        if (mpZantMorf->checkFrame(76.0f)) {
            mDrawZantSword = false;
        }

        cLib_addCalcPos(&mCameraCenter, mOpCenterDt[13], 0.7f, 10.0f, 5.0f);
        cLib_addCalcPos(&mCameraEye, mOpEyeDt[13], 0.7f, 10.0f, 5.0f);
        if (!mpZantMorf->isStop()) {
            break;
        }

        mSound.startCreatureSound(Z2SE_EN_ZAN_L4_WARP_OUT, 0, -1);
        mMode++;
        // fallthrough
    case 32:
        cLib_addCalc2(&mZantScale.x, 0.0f, 0.7f, 0.1f);
        cLib_addCalc2(&mZantScale.y, 5.0f, 0.7f, 0.7f);
        cLib_addCalc2(&mZantScale.z, 0.0f, 0.7f, 0.1f);
        if (mZantScale.y < 4.9f) {
            break;
        }

        particle_angle.x = 0;
        particle_angle.y = field_0x7ca + 5000;
        particle_angle.z = 0;
        for (int i = 0; i < 2; i++) {
            dComIfGp_particle_set(eff_znt_Disapp_id[i], &mZantPos, &particle_angle, NULL);
        }

        mDrawZant = false;
        mModeTimer = 50;
        mMode++;
        // fallthrough
    case 33:
        if (cLib_calcTimer(&mModeTimer) != 0) {
            break;
        }

        Z2GetAudioMgr()->subBgmStart(Z2BGM_HARAGIGANT_D01);
        mSound.startCreatureSound(Z2SE_EN_DS_OPDEMO, 0, -1);
        mModeTimer = 50;
        mMode = 40;
        // fallthrough
    case 40:
        mCameraCenter.set(mOpCenterBodyDt[0]);
        mCameraEye.set(mOpEyeBodyDt[0]);
        if (cLib_calcTimer(&mModeTimer) != 0) {
            break;
        }

        mModeTimer = 100;
        dComIfGp_getVibration().StartQuake(4, 0x1f, cXyz(0.0f, 1.0f, 0.0f));
        mMode++;
        // fallthrough
    case 41:
        if (cLib_calcTimer(&mModeTimer) != 0) {
            break;
        }
        mMode++;
        // fallthrough
    case 42:
        cLib_addCalc2(&mEyeColorAlpha, 255.0f, 0.7f, 10.0f);
        if (mEyeColorAlpha < 254.0f) {
            break;
        }

        field_0x7d2 = 0;
        mModeTimer = 0;
        mEyeColorAlpha = 255.0f;
        setBck(ANM_OPDEMO, 0, 1.0f, 1.0f);
        dComIfGp_particle_set(0x8bd6, &current.pos, &shape_angle, NULL);
        mMode++;
        // fallthrough
    case 43:
        sp280.set(mSwordPos);
        sp280.y += -100.0f;
        cLib_addCalcPos(&mCameraCenter, sp280, 1.0f, 20.0f, 10.0f);
        if ((int)mpMorf->getFrame() < 330) {
            break;
        }

        mIsOpeningDemo = false;
        sp28C = current.pos - pla->current.pos;
        shape_angle.y = -700;
        daPy_getPlayerActorClass()->setPlayerPosAndAngle(&mOpPlayerDt[2],
                                                         sp28C.atan2sX_Z() + 0x1000, 0);
        mMode++;
        // fallthrough

    case 44:
        sp274.set(mOpEyeBodyDt[1]);
        sp280.set(mOpCenterBodyDt[1]);
        sp280.y = mSwordPos.y;

        f32 max_step = 10.0f;
        f32 min_step = 5.0f;
        if ((int)mpMorf->getFrame() > 460) {
            max_step = 40.0f;
            min_step = 20.0f;
            sp280.y -= 200.0f;
        }

        if (sp280.y > 1835.0f) {
            sp280.y = 1835.0f;
        } else if (sp280.y < 1250.0f) {
            sp280.y = 1250.0f;
        }

        cLib_addCalcPos(&mCameraCenter, sp280, 1.0f, max_step, min_step);
        cLib_addCalcPos(&mCameraEye, sp274, 1.0f, 40.0f, 20.0f);

        if (mpMorf->checkFrame(517.0f)) {
            dComIfGp_getVibration().StopQuake(0x1f);
        }

        if (mpMorf->checkFrame(518.0f)) {
            hand_smokeSet(0);
        }

        if (mpMorf->checkFrame(670.0f)) {
            dCam_getBody()->StartBlure(40, this, 0.8f, 1.2f);
            fopMsgM_messageSetDemo(0x482);
            dComIfGp_getVibration().StartQuake(4, 0x1f, cXyz(0.0f, 1.0f, 0.0f));
            pla->changeDemoMode(34, 0, 0, 0);
        }

        if (mpMorf->checkFrame(770.0f)) {
            dComIfGp_getVibration().StopQuake(0x1f);
        }

        if (mpMorf->isStop()) {
            dKy_change_colpat(1);

            camera->mCamera.Start();
            camera->mCamera.SetTrimSize(0);
            dComIfGp_event_reset();

            attention_info.distances[fopAc_attn_BATTLE_e] = 0;
            attention_info.flags = fopAc_AttnFlag_BATTLE_e;
            fopAcM_SetGroup(this, 2);
            fopAcM_OnStatus(this, 0);

            Z2GetAudioMgr()->bgmStart(Z2BGM_HARAGIGANT_BTL01, 0, 0);
            Z2GetAudioMgr()->setDemoName("force_end");

            dComIfGs_onZoneSwitch(5, fopAcM_GetRoomNo(this));
            field_0x7f8 = 0.5f;
            mSwordTimer = 3;
            setActionMode(ACT_WAIT, 0);
            return;
        }
    }

    if (mMode > 41) {
        cLib_addCalc(&mColBlend, 1.0f, 0.01f, 0.01f, 0.001f);
        dKy_custom_colset(0, 1, mColBlend);
        cLib_addCalc2(&field_0x7f8, 0.5f, 0.1f, 0.01f);
    }

    mZantEyePos.set(mZantPos);
    mZantEyePos.y += 200.0f;

    if (mMode != 0 && mMode != 3 && mMode != 4) {
        camera->mCamera.Set(mCameraCenter, mCameraEye);
    }

    if (mMode < 11) {
        return;
    }

    if (mMode >= 43) {
        sp268.set(1.0f, 1.0f, 1.0f);
        for (int i = 0; i < 16; i++) {
            mStartingParticleKey[i] = dComIfGp_particle_set(
                mStartingParticleKey[i], KIDOU_EFF_DT[i].eff_id, &current.pos, &shape_angle, NULL);
            JPABaseEmitter* emitter = dComIfGp_particle_getEmitter(mStartingParticleKey[i]);
            if (emitter != NULL) {
                emitter->setGlobalSRTMatrix(
                    mpMorf->getModel()->getAnmMtx(KIDOU_EFF_DT[i].joint_no));
            }
        }
    }

    if (dComIfGp_getEvent()->checkSkipEdge()) {
        cDmr_SkipInfo = 1;
        dStage_changeScene(2, 0.0f, 0, fopAcM_GetRoomNo(this), 0, -1);
        dComIfGs_onZoneSwitch(5, fopAcM_GetRoomNo(this));

        Z2GetAudioMgr()->setDemoName("force_end");
        Z2GetAudioMgr()->bgmStreamStop(0);
        Z2GetAudioMgr()->subBgmStop();
    }
}

bool daB_DS_c::mHandBreathChk() {
    daPy_py_c* pla = daPy_getPlayerActorClass();
    if (daPy_getPlayerActorClass()->getDamageWaitTimer() > 10) {
        return false;
    }

    if (daPy_getPlayerActorClass()->checkSpinnerRide()) {
        if (abs((s16)(fopAcM_searchPlayerAngleY(this) - shape_angle.y)) < 0x4000) {
            cXyz hand_l_dist = mHandPos[0] - pla->current.pos;
            cXyz hand_r_dist = mHandPos[1] - pla->current.pos;

            if (hand_l_dist.abs() < l_HIO.mHandRange) {
                mAttackingHand = 1;
                setActionMode(ACT_HAND_ATTACK, 0);
                return true;
            }

            if (hand_r_dist.abs() < l_HIO.mHandRange) {
                mAttackingHand = 0;
                setActionMode(ACT_HAND_ATTACK, 0);
                return true;
            }
        }
    }

    if (cLib_calcTimer(&mModeTimer) == 0 &&
        fopAcM_searchPlayerDistance(this) > l_HIO.mBreathRange)
    {
        if ((s16)(shape_angle.y - fopAcM_searchPlayerAngleY(this)) < 0x2000) {
            setActionMode(ACT_BREATH_ATTACK, 0);
        } else {
            setActionMode(ACT_BREATH_SEARCH, 0);
        }
        return true;
    }

    return false;
}

void daB_DS_c::executeWait() {
    static int Ds_wait_id[3] = {
        ANM_WAIT01_A,
        ANM_WAIT01_B,
        ANM_WAIT01_C,
    };

    switch (mMode) {
    case 0:
        if (mModeTimer == 0) {
            mModeTimer = (int)cM_rndF(60.0f) + 120;

            if (mBackboneLevel == 1) {
                mModeTimer = FAST_DIV(mModeTimer, 2);
            } else if (mBackboneLevel == 2) {
                mModeTimer = FAST_DIV(mModeTimer, 4);
            }
        }

        if (mAnmID != Ds_wait_id[mBackboneLevel]) {
            setBck(Ds_wait_id[mBackboneLevel], 2, 10.0f, 1.0f);
        }
        mMode = 1;
        // fallthrough
    case 1:
        if (fopAcM_searchPlayerDistance(this) > l_HIO.mNoSearchRange ||
            daPy_getPlayerActorClass()->checkSpinnerRide())
        {
            s16 angle_to_player = shape_angle.y - fopAcM_searchPlayerAngleY(this);
            if (abs(angle_to_player) > 0x1800) {
                setActionMode(ACT_CIRCLE, 0);
            }
        }
    }

    mHandBreathChk();
}

void daB_DS_c::executeCircle() {
    static int Ds_circle_id[6] = {
        ANM_CIRCLE_L_A, ANM_CIRCLE_L_B, ANM_CIRCLE_L_C,
        ANM_CIRCLE_R_A, ANM_CIRCLE_R_B, ANM_CIRCLE_R_C,
    };
    s16 angle_to_player = shape_angle.y - fopAcM_searchPlayerAngleY(this);

    switch (mMode) {
    case 0:
        if (mModeTimer == 0) {
            mModeTimer = (int)cM_rndF(60.0f) + 240;
        }

        if (angle_to_player < 0) {
            setBck(Ds_circle_id[mBackboneLevel], 2, 10.0f, 1.0f);
        } else {
            setBck(Ds_circle_id[mBackboneLevel + 3], 2, 10.0f, 1.0f);
        }
        mMode = 1;
        // fallthrough
    case 1:
        angle_to_player = shape_angle.y - fopAcM_searchPlayerAngleY(this);
        if (abs(angle_to_player) < 0x200) {
            setActionMode(ACT_WAIT, 0);
        }
    }

    cLib_addCalcAngleS2(&current.angle.y, fopAcM_searchPlayerAngleY(this), 2, 100);
    shape_angle.y = current.angle.y;
    mHandBreathChk();
}

void daB_DS_c::mBreakBoneSUB() {
    if (bitSw3 != 0xFF) {
        fopAcM_offSwitch(this, bitSw3);
        mSoundPos = current.pos;
        mSound.startCollisionSE(Z2SE_HIT_SWORD, 0x1F);
    }
}

void daB_DS_c::damageSet() {
    if (mBossPhase == 0) {
        dScnPly_c::setPauseTimer(5);
    }

    field_0x85e = true;
    mBreakBoneSUB();
    dComIfGp_getVibration().StartShock(3, 0x4f, cXyz(0.0f, 1.0f, 0.0f));
}

void daB_DS_c::damageHitCamera() {
    camera_class* camera = dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0));
    cXyz offset, pos;

    mDoMtx_YrotS(*calc_mtx, field_0x7d0);

    offset.zero();
    offset.y = 300.0f;
    MtxPosition(&offset, &pos);
    pos.x += current.pos.x;
    if (mAcch.GetGroundH() != -G_CM3D_F_INF) {
        pos.y += mAcch.GetGroundH();
    }
    pos.z += current.pos.z;
    mCameraCenter = pos;

    offset.zero();
    offset.z = 700.0f;
    MtxPosition(&offset, &pos);
    pos.x += current.pos.x;
    if (mAcch.GetGroundH() != -G_CM3D_F_INF) {
        pos.y += mAcch.GetGroundH();
    }
    pos.z += current.pos.z;
    mCameraEye = pos;

    camera->mCamera.Set(mCameraCenter, mCameraEye);
}

void daB_DS_c::damageDownCheck() {
    cXyz particle_scale(1.0f, 1.0f, 1.0f);
    csXyz particle_angle = shape_angle;

    if (mAnmID == ANM_DAMAGE_A || mAnmID == ANM_DAMAGE_B) {
        int ptcl_check_frame = 18;

        if (mBrokenBone == 0) {
            if (mpMorf->checkFrame(30.0f)) {
                hand_smokeSet(1);
            }

            if (mpMorf->checkFrame(50.0f)) {
                hand_smokeSet(2);
            }

            ptcl_check_frame = 35;
        } else {
            if (mpMorf->checkFrame(13.0f)) {
                hand_smokeSet(1);
            }

            if (mpMorf->checkFrame(29.0f)) {
                hand_smokeSet(2);
            }
        }

        if (mpMorf->checkFrame(ptcl_check_frame)) {
            for (int i = 0; i < 2; i++) {
                mHitParticleKey[i] =
                    dComIfGp_particle_set(mHitParticleKey[i], eff_smoke_id[i], &current.pos,
                                          &particle_angle, &particle_scale);
            }
        }
    }
}

void daB_DS_c::executeDamage() {
    static cXyz down_center_dt[5] = {
        cXyz(345.0f, 2100.0f, -1575.0f),   cXyz(1490.0f, 1160.0f, -2050.0f),
        cXyz(1465.0f, 1135.0f, -2110.0f),  cXyz(-320.0f, 30.0f, -2355.0f),
        cXyz(-320.0f, -1900.0f, -2355.0f),
    };

    static cXyz down_eye_dt[5] = {
        cXyz(-180.0f, 1390.0f, 3080.0f),  cXyz(1675.0f, 1120.0f, -2310.0f),
        cXyz(1690.0f, 1210.0f, -2315.0f), cXyz(1400.0f, 2215.0f, 1480.0f),
        cXyz(1400.0f, 280.0f, 1480.0f),
    };

    static struct {
        u16 eff_id;
        int joint_no;
    } const BROKENBORN_EFF_DT[12] = {
        {0x8BDB, 0}, {0x8BDC, 0}, {0x8BDD, 0}, {0x8BDE, 0}, {0x8BDF, 1}, {0x8BE0, 1},
        {0x8BE1, 1}, {0x8BE2, 1}, {0x8BE3, 2}, {0x8BE4, 2}, {0x8BE5, 2}, {0x8BE6, 2},
    };

    static int Ds_damage_wait_id[3] = {ANM_DAMAGE_A_WAIT, ANM_DAMAGE_B_WAIT, ANM_LASTDAMAGE_WAIT};
    static int Ds_damage_id[3] = {ANM_DAMAGE_A, ANM_DAMAGE_B, ANM_LASTDAMAGE};
    static u16 eff_spHit_id[2] = {0x8BD9, 0x8BDA};
    static u16 eff_LastSmoke_id[4] = {0x8C25, 0x8C26, 0x8C27, 0x8C28};

    camera_class* camera = dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0));
    daPy_py_c* pla = daPy_getPlayerActorClass();
    csXyz angle = shape_angle;
    cXyz sp1BC, sp1B0;
    cXyz particle_scale(1.0f, 1.0f, 1.0f);

    mHeadAngle_Clear();
    mDamageTimer = 50;
    int index;

    switch (mMode) {
    case 0:
        mHintTimer1 = 0;

        mHandAtLCyl.OffAtSetBit();
        mHandAtRCyl.OffAtSetBit();

        if (!startDemoCheck()) {
            break;
        }

        mColBlend = 0.0f;
        for (int i = 0; i < 2; i++) {
            dComIfGp_particle_set(eff_spHit_id[i], &current.pos, &shape_angle, NULL);
        }

        field_0x84d = 0;
        mClearTrap(true);

        daPy_getPlayerActorClass()->changeOriginalDemo();
        daPy_getPlayerActorClass()->changeDemoMode(14, 1, 0, 0);
        daPy_getPlayerActorClass()->setPlayerPosAndAngle(&pla->current.pos,
                                                         fopAcM_searchPlayerAngleY(this), 0);

        dComIfGp_getVibration().StartShock(5, 0x1f, cXyz(0.0f, 1.0f, 0.0f));
        field_0x7d0 = fopAcM_searchPlayerAngleY(this);
        setBck(Ds_damage_wait_id[mBackboneLevel], 2, 3.0f, 1.0f);
        mIsDemo = true;
        field_0x85e = false;

        if (mMode == 0) {
            mModeTimer = 5;
        } else {
            mModeTimer = 30;
        }
        mMode++;
        break;
    case 1:
        if (cLib_calcTimer(&mModeTimer) != 0) {
            break;
        }

        daPy_getPlayerActorClass()->changeDemoMode(14, 2, 0, 0);

        mModeTimer = 41;
        mIsOpeningDemo = false;

        if (mBackboneLevel < 2) {
            mBrokenBone = mBackboneLevel;
            mMode++;
        } else {
            mDead = true;
            mIsOpeningDemo = true;
            mMode = 10;
        }
        break;
    case 2:
        if (cLib_calcTimer(&mModeTimer) == 1 &&
            (mAnmID == ANM_DAMAGE_A_WAIT || mAnmID == ANM_DAMAGE_B_WAIT))
        {
            setBck(Ds_damage_id[mBrokenBone], 0, 3.0f, 1.0f);
        }

        if (mModeTimer == 0 && mAnmID != ANM_DAMAGE_A_WAIT && mAnmID != ANM_DAMAGE_B_WAIT &&
            ((mBrokenBone == 0 && mpMorf->checkFrame(24.0f)) ||
             (mBrokenBone != 0 && mpMorf->checkFrame(7.0f))))
        {
            int index = mBackboneLevel * 4;
            for (int i = 0; i < 4; i++) {
                JPABaseEmitter* emitter = dComIfGp_particle_set(BROKENBORN_EFF_DT[index + i].eff_id,
                                                                &current.pos, &shape_angle, NULL);
                if (emitter != NULL) {
                    emitter->setGlobalSRTMatrix(
                        mpMorf->getModel()->getAnmMtx(BROKENBORN_EFF_DT[index + i].joint_no));
                }
            }

            mSoundPos = current.pos;
            mSound.startCreatureSound(Z2SE_EN_DS_BONE_BRK, 0, -1);
            damageSet();
            mBackboneLevel++;

            s16 angle_to_player = fopAcM_searchPlayerAngleY(this);
            for (int i = 0; i < 15; i++) {
                mDoMtx_YrotS(*calc_mtx, angle_to_player);

                sp1BC.x = 0.0f;
                sp1BC.y = 0.0f;
                sp1BC.z = 1000.0f;
                MtxPosition(&sp1BC, &sp1B0);
                sp1B0 += current.pos;
                if (mAcch.GetGroundH() != -G_CM3D_F_INF) {
                    sp1B0.y = mAcch.GetGroundH();
                }

                angle = shape_angle;
                angle.y = fopAcM_searchPlayerAngleY(this);

                fopAc_ac_c* actor;
                if (mStaltroopID[i] != 0 && !fopAcM_SearchByID(mStaltroopID[i], &actor)) {
                    mStaltroopID[i] = 0;
                }

                if (mStaltroopID[i] == 0) {
                    mStaltroopID[i] =
                        fopAcM_createChild(PROC_E_ZS, fopAcM_GetID(this), 1, &sp1B0,
                                           fopAcM_GetRoomNo(this), &angle, NULL, -1, NULL);
                }
            }

            if (mBackboneLevel == 2) {
                for (int i = 0; i < 5; i++) {
                    if (mStaltroop2ID[i] == 0) {
                        mStaltroop2ID[i] =
                            fopAcM_createChild(PROC_E_ZS, fopAcM_GetID(this), 2, &current.pos,
                                               fopAcM_GetRoomNo(this), NULL, NULL, -1, NULL);
                    }
                }
            }
        }

        mBackboneCrackAlpha[mBrokenBone] -= 12.75f;
        if (mBackboneCrackAlpha[mBrokenBone] < 0.0f) {
            mBackboneCrackAlpha[mBrokenBone] = 0.0f;
        }

        damageHitCamera();
        damageDownCheck();

        if (field_0x85e && mBackboneCrackAlpha[mBrokenBone] < 2.0f) {
            mModeTimer = 30;
            mBackboneCrackAlpha[mBrokenBone] = 0.0f;
            mMode = 3;
        }
        break;
    case 3:
        damageDownCheck();
        mIsDemo = false;
        if (cLib_calcTimer(&mModeTimer) != 0) {
            break;
        }

        mDoMtx_YrotS(*calc_mtx, shape_angle.y);

        sp1BC.zero();
        sp1BC.y = 1100.0f;
        sp1BC.z = -1500.0f;
        MtxPosition(&sp1BC, &sp1B0);
        sp1B0 += current.pos;
        cLib_addCalcPos(&mCameraCenter, sp1B0, 1.0f, 2000.0f, 300.0f);

        sp1BC.zero();
        sp1BC.y = 1600.0f;
        sp1BC.z = 4200.0f;
        MtxPosition(&sp1BC, &sp1B0);
        sp1B0 += current.pos;
        cLib_addCalcPos(&mCameraEye, sp1B0, 1.0f, 2000.0f, 300.0f);

        camera->mCamera.Set(mCameraCenter, mCameraEye);

        if (mpMorf->isStop()) {
            mBrokenBone = 0;

            f32 dist = fopAcM_searchPlayerDistance(this);
            mDoMtx_YrotS(*calc_mtx, dist + 0x8000);
            sp1BC.x = 0.0f;
            sp1BC.y = 0.0f;
            sp1BC.z = -1000.0f;
            MtxPosition(&sp1BC, &sp1B0);

            sp1B0 += pla->current.pos;
            sp1BC = pla->current.pos;
            camera->mCamera.Reset(pla->current.pos, sp1B0);
            camera->mCamera.Start();
            camera->mCamera.SetTrimSize(0);
            dComIfGp_event_reset();

            if (bitSw3 != 0xff) {
                fopAcM_offSwitch(this, bitSw3);
            }

            mBirthTrapTimerF = 30;
            shape_angle.y = current.angle.y = fopAcM_searchPlayerAngleY(this) + 0xC000;
            setActionMode(ACT_BREATH_ATTACK, 0);
        }
        break;
    case 10:
        mBackboneCrackAlpha[mBackboneLevel] -= 12.75f;
        if (mBackboneCrackAlpha[mBackboneLevel] < 0.0f) {
            mBackboneCrackAlpha[mBackboneLevel] = 0.0f;
        }
        damageHitCamera();

        if (cLib_calcTimer(&mModeTimer) == 0) {
            if (!mBackboneCrackAlpha[mBackboneLevel]) {
                if (bitSw != 0xff) {
                    fopAcM_offSwitch(this, bitSw);
                }

                setBck(Ds_damage_id[mBackboneLevel], 0, 3.0f, 1.0f);

                for (int i = 0; i < 4; i++) {
                    dComIfGp_particle_set(eff_LastSmoke_id[i], &current.pos, &home.angle, NULL);
                }

                sp1BC.x = 0.0f;
                sp1BC.y = -1600.0f;
                sp1BC.z = 2315.0f;
                daPy_getPlayerActorClass()->changeDemoMode(4, 2, 0, 0);
                daPy_getPlayerActorClass()->setPlayerPosAndAngle(&sp1BC, -0x8000, 0);
                mMode++;
            }
        }
        break;
    case 11:
        cLib_addCalcAngleS2(&shape_angle.x, 2000, 8, 0x100);
        cLib_addCalc0(&field_0x7f8, 0.1f, 0.01f);

        if (mpMorf->checkFrame(120.0f)) {
            hand_smokeSet(1);
            dComIfGp_getVibration().StartShock(1, 0x1f, cXyz(0.0f, 1.0f, 0.0f));
            mSound.startCreatureSound(Z2SE_EN_DS_DMG_HAND, 0, -1);
            mBossPhase = 1;
            damageSet();

            int index = 8;
            for (int i = 0; i < 4; i++) {
                JPABaseEmitter* emitter = dComIfGp_particle_set(BROKENBORN_EFF_DT[index + i].eff_id,
                                                                &current.pos, &shape_angle, NULL);
                if (emitter != NULL) {
                    emitter->setGlobalSRTMatrix(
                        mpMorf->getModel()->getAnmMtx(BROKENBORN_EFF_DT[index + i].joint_no));
                }
            }

            mSoundPos = current.pos;
            mSound.startCreatureSound(Z2SE_EN_DS_BONE_BRK, 0, -1);
        }

        if (mpMorf->checkFrame(241.0f)) {
            hand_smokeSet(2);
            dComIfGp_getVibration().StartShock(5, 0x1f, cXyz(0.0f, 1.0f, 0.0f));
            mSoundPos = current.pos;
            mSound.startCreatureSound(Z2SE_EN_DS_DMG_HAND, 0, -1);
        }

        if (mpMorf->checkFrame(145.0f)) {
            for (int i = 0; i < 2; i++) {
                mHitParticleKey[i] = dComIfGp_particle_set(mHitParticleKey[i], eff_smoke_id[i],
                                                           &current.pos, &angle, &particle_scale);
            }
            mSoundPos = current.pos;
            dComIfGp_getVibration().StartShock(2, 0x4f, cXyz(0.0f, 1.0f, 0.0f));
            mSound.startCreatureSound(Z2SE_EN_DS_DMG_HAND, 0, -1);
        }

        index = 0;
        if ((int)mpMorf->getFrame() > 130) {
            index = 1;
        }

        if ((int)mpMorf->getFrame() < 140) {
            shape_angle.y = home.angle.y;
            current.angle.y = shape_angle.y;
            mCameraCenter = down_center_dt[index];
            mCameraEye = down_eye_dt[index];
        } else {
            cLib_addCalcPos(&mCameraCenter, down_center_dt[2], 0.8f, 1.0f, 0.5f);
            cLib_addCalcPos(&mCameraEye, down_eye_dt[2], 0.8f, 1.0f, 0.5f);
        }
        camera->mCamera.Set(mCameraCenter, mCameraEye);

        if (mpMorf->checkFrame(215.0f)) {
            dComIfGp_getVibration().StartShock(4, 0x4f, cXyz(0.0f, 1.0f, 0.0f));
            mSoundPos = current.pos;
            mSound.startCreatureSound(Z2SE_EN_DS_DMG_HAND, 0, -1);
        }

        if (!mpMorf->isStop()) {
            break;
        }
        mModeTimer = 30;
        mMode = 20;
        // fallthrough
    case 20:
        if (cLib_calcTimer(&mModeTimer) != 0) {
            if (mModeTimer == 1) {
                mSoundPos = current.pos;
                mSound.startCreatureSound(Z2SE_EN_DS_EYE_OFF, 0, -1);
            }
            break;
        }

        cLib_addCalc0(&mEyeColorAlpha, 0.7f, 3.0f);
        if (mEyeColorAlpha > 2.0f) {
            break;
        }

        mModeTimer = 30;
        mEyeColorAlpha = 0.0f;
        mMode++;
        // fallthrough
    case 21:
        if (cLib_calcTimer(&mModeTimer) != 0) {
            break;
        }

        mCameraCenter = down_center_dt[3];
        mCameraEye = down_eye_dt[3];
        camera->mCamera.Set(mCameraCenter, mCameraEye);
        dComIfGp_getVibration().StartQuake(5, 0x1f, cXyz(0.0f, 1.0f, 0.0f));

        mModeTimer = 30;
        mMode++;
        // fallthrough
    case 22:
        if (cLib_calcTimer(&mModeTimer) != 0) {
            break;
        }

        mColBlend = 0.0f;
        mMode = 100;
        mSound.startCreatureSound(Z2SE_EN_DS_MDEMO_FALL, 0, -1);
        // fallthrough
    case 100: {
        gravity = -0.2f;
        if (current.pos.y > -1600.0f) {
            if (current.pos.y <= -300.0f) {
                dComIfGs_onZoneSwitch(6, fopAcM_GetRoomNo(this));
            }
            break;
        }

        mBossPhase = 100;
        u32 param = fopAcM_GetParam(this);
        csXyz p2_angle;
        p2_angle = shape_angle;
        p2_angle.z = 0;
        p2_angle.y = 0;
        p2_angle.x = 0;

        fopAcM_create(PROC_B_DS, param | TYPE_BATTLE_2, &current.pos, fopAcM_GetRoomNo(this),
                      &p2_angle, NULL, 0xff);
        mMode = 101;
        // fallthrough
    }
    case 101:
        cLib_addCalc2(&mCameraCenter.y, down_center_dt[4].y, 0.7f, 10.0f);
        cLib_addCalc2(&mCameraEye.y, down_eye_dt[4].y, 0.7f, 10.0f);
        camera->mCamera.Set(mCameraCenter, mCameraEye);
        if (fabsf(mCameraCenter.y - down_center_dt[4].y) > 2.0f ||
            fabsf(mCameraEye.y - down_eye_dt[4].y) > 2.0f)
        {
            break;
        }
        dComIfGp_getVibration().StopQuake(0x1f);
        mModeTimer = 100;
        mMode = 102;
        // fallthrough
    case 102:
        if (cLib_calcTimer(&mModeTimer) != 0) {
            break;
        }

        dKy_change_colpat(3);

        sp1BC.set(0.0f, -1510.0f, 2315.0f);
        sp1B0.set(0.0f, -1435.0f, 2525.0f);
        camera->mCamera.Reset(sp1BC, sp1B0);
        camera->mCamera.Start();
        camera->mCamera.SetTrimSize(0);

        dComIfGp_event_reset();
        fopAcM_delete(this);
    }

    if (mMode >= 100) {
        cLib_addCalc(&mColBlend, 1.0f, 0.02f, 0.02f, 0.001f);
        dKy_custom_colset(1, 3, mColBlend);
    }
}

void daB_DS_c::executeEtcDamage() {
    static int Ds_Etc_damage_id[9] = {
        ANM_NODAMAGE_01_A, ANM_NODAMAGE_01_B, ANM_NODAMAGE_01_C,
        ANM_NODAMAGE_02_A, ANM_NODAMAGE_02_B, ANM_NODAMAGE_02_C,
        ANM_NODAMAGE_03_A, ANM_NODAMAGE_03_B, ANM_NODAMAGE_03_C,
    };

    static int smokeSet_dt[3] = {32, 29, 29};

    switch (mMode) {
    case 0:
        dComIfGp_getVibration().StartShock(7, 0x1f, cXyz(0.0f, 1.0f, 0.0f));
        setBck(Ds_Etc_damage_id[mBackboneLevel], 0, 3.0f, 1.0f);
        mColBlend = 0.0f;
        mMode = 3;
        break;
    case 1:
        dComIfGp_getVibration().StartShock(7, 0x1f, cXyz(0.0f, 1.0f, 0.0f));
        setBck(Ds_Etc_damage_id[mBackboneLevel + 3], 0, 3.0f, 1.0f);
        mColBlend = 0.0f;
        mMode = 4;
        break;
    case 2:
        setBck(Ds_Etc_damage_id[mBackboneLevel + 6], 0, 3.0f, 1.0f);
        mColBlend = 0.0f;
        mMode = 5;
        break;
    case 3:
    case 4:
        if ((int)mpMorf->getFrame() == smokeSet_dt[mBackboneLevel]) {
            if (mMode == 4) {
                hand_smokeSet(1);
            } else {
                hand_smokeSet(2);
            }
        }
        // fallthrough
    case 5:
        if (mpMorf->isStop()) {
            setActionMode(ACT_WAIT, 0);
        }
    }
}

void daB_DS_c::breath_smokeSet() {
    static u16 effId[3] = {0x85F6, 0x89B0, 0x89B1};
    cXyz particle_scale(1.0f, 1.0f, 1.0f);

    cLib_addCalc2(&mBreathTimerBase, 50.0f, 0.7f, 4.0f);
    breathTimerBase = (int)mBreathTimerBase;

    for (int i = 0; i < 3; i++) {
        mBreathSmokeParticleKey[i] = dComIfGp_particle_set(mBreathSmokeParticleKey[i], effId[i],
                                                           &mHeadPos, &mHeadAngle, &particle_scale);
        JPABaseEmitter* emitter = dComIfGp_particle_getEmitter(mBreathSmokeParticleKey[i]);
        if (emitter != NULL) {
            emitter->setGlobalSRTMatrix(mpMorf->getModel()->getAnmMtx(DS_JNT_HEAD));
        }
    }

    Z2GetAudioMgr()->seStartLevel(Z2SE_OBJ_DS_FIRE_CLMN, &mHeadPos, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f,
                                  0);

    mBreathTimeCount++;
    mBreathTimeCount &= 3;
    if (mBreathTimeCount == 0) {
        cXyz vec = mMouthPos - mBulletPos;
        csXyz bullet_angle;
        bullet_angle.x = -vec.atan2sY_XZ();
        bullet_angle.y = vec.atan2sX_Z();
        bullet_angle.z = 0;

        cXyz bullet_pos = mBulletPos;

        fopAcM_create(PROC_B_DS, TYPE_BULLET_A, &bullet_pos, fopAcM_GetRoomNo(this), &bullet_angle,
                      NULL, 0xff);
    }
}

void daB_DS_c::executeBreathAttack() {
    static int Ds_breath_id[9] = {
        ANM_ATTACK02_A_A, ANM_ATTACK02_B_A, ANM_ATTACK02_C_A, ANM_ATTACK02_A_B, ANM_ATTACK02_B_B,
        ANM_ATTACK02_C_B, ANM_ATTACK02_A_C, ANM_ATTACK02_B_C, ANM_ATTACK02_C_C,
    };

    switch (mMode) {
    case 0:
        mColBlend = 0.0f;
        field_0x840 = 1;
        mBh2AttackAngleF = 0;
        setBck(Ds_breath_id[mBackboneLevel * 3], 0, 10.0f, 1.0f);
        mMode = 1;
        break;
    case 1:
        if (mpMorf->checkFrame(82.0f)) {
            hand_smokeSet(0);
            mSound.startCreatureSound(Z2SE_EN_DS_HANDS, 0, -1);
        }

        if ((int)mpMorf->getFrame() > 82) {
            cLib_addCalcAngleS2(&mBh2AttackAngleF, 10000, 10, 0x100);
        }

        if (mpMorf->isStop()) {
            setBck(Ds_breath_id[mBackboneLevel * 3 + 1], 2, 3.0f, 1.0f);
            mModeTimer = 185;
            mBreathTimerBase = 0.0f;
            breathTimerBase = 0;
            mMode = 2;
        }
        break;
    case 2:
        if (mModeTimer == 0 || daPy_getPlayerActorClass()->getDamageWaitTimer() > 10) {
            setBck(Ds_breath_id[mBackboneLevel * 3 + 2], 0, 3.0f, 1.0f);
            mColBlend = 0.0f;
            mMode = 3;
        } else if (cLib_calcTimer(&mModeTimer) != 0) {
            cLib_addCalcAngleS2(&mBh2AttackAngleF, 5000, 10, 0x100);
            cLib_addCalc(&mColBlend, 1.0f, 0.02f, 0.02f, 0.04f);
            dKy_custom_colset(1, 5, mColBlend);
            field_0x840 = 5;
            breath_smokeSet();
        }
        break;
    case 3:
        if (mpMorf->isStop()) {
            setActionMode(ACT_WAIT, 0);
        }
    }
}

void daB_DS_c::executeHandAttack() {
    static int Ds_HandAt_id[6] = {
        ANM_ATTACK_R_A, ANM_ATTACK_R_B, ANM_ATTACK_R_C,
        ANM_ATTACK_L_A, ANM_ATTACK_L_B, ANM_ATTACK_L_C,
    };

    switch (mMode) {
    case 0:
        mChkHigh = 0.0f;
        setBck(Ds_HandAt_id[mBackboneLevel + mAttackingHand * 3], 0, 10.0f, 1.0f);
        mMode++;
        break;
    case 1:
        if ((int)mpMorf->getFrame() > 90 && (int)mpMorf->getFrame() < 110) {
            bool bvar1 = false;

            int ivar3 = (int)((mpMorf->getFrame() - 90.0f) / 4.0f) + 7;
            if ((int)mChkHigh < ivar3) {
                mChkHigh = ivar3;
                bvar1 = true;
            }

            if (mAttackingHand == 0) {
                mHandAtRCyl.OnAtSetBit();
                if ((int)mpMorf->getFrame() > 100 && bvar1) {
                    hand_smokeSet(2);
                }
            } else {
                mHandAtLCyl.OnAtSetBit();
                if ((int)mpMorf->getFrame() > 100 && bvar1) {
                    hand_smokeSet(1);
                }
            }
        } else {
            mHandAtLCyl.OffAtSetBit();
            mHandAtRCyl.OffAtSetBit();
        }

        if (mpMorf->isStop()) {
            setActionMode(ACT_WAIT, 0);
        }
    }
}

void daB_DS_c::executeBreathSearch() {
    static int Ds_BS_id[3] = {ANM_ATTACK01_A, ANM_ATTACK01_B, ANM_ATTACK01_C};
    mHeadAngle_Clear();

    switch (mMode) {
    case 0:
        setBck(Ds_BS_id[mBackboneLevel], 0, 10.0f, 1.0f);
        mMode++;
        // fallthrough
    case 1:
        if ((int)mpMorf->getFrame() >= 177 && (int)mpMorf->getFrame() <= 195) {
            mHandAtRCyl.OnAtSetBit();
            mHandAtLCyl.OnAtSetBit();
            hand_smokeSet(0);
        } else {
            mHandAtLCyl.OffAtSetBit();
            mHandAtRCyl.OffAtSetBit();
        }

        if (mpMorf->isStop() && !mHandBreathChk()) {
            setActionMode(ACT_WAIT, 0);
        }
        break;
    }

    if ((int)mpMorf->getFrame() > 70 && (int)mpMorf->getFrame() < 150) {
        s16 angle = fopAcM_searchPlayerAngleY(this);
        cLib_addCalcAngleS2(&current.angle.y, angle, 40, 0x200);
        shape_angle.y = current.angle.y;
    }
}

void daB_DS_c::executeBattle2OpeningDemo() {
    camera_class* camera = dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0));
    daPy_py_c* pla = daPy_getPlayerActorClass();
    cXyz vec, center, eye, unused;
    center.zero();

    static cXyz mOp2PlayerDt[3] = {
        cXyz(315.0f, 1800.0f, 1460.0f),
        cXyz(415.0f, 1800.0f, 1880.0f),
        cXyz(465.0f, 1400.0f, 1980.0f),
    };

    static cXyz mOp2CenterDt[7] = {
        cXyz(-255.0f, -1460.0f, 3320.0f), cXyz(-255.0f, 3070.0f, 3340.0f),
        cXyz(145.0f, 1940.0f, 1520.0f),   cXyz(165.0f, 1930.0f, 1515.0f),
        cXyz(180.0f, 2083.0f, 1490.0f),   cXyz(450.0f, 1880.0f, 1955.0f),
        cXyz(1815.0f, 2370.0f, 1660.0f),
    };

    static cXyz mOp2EyeDt[7] = {
        cXyz(-290.0f, -1560.0f, 3690.0f), cXyz(-290.0f, 3230.0f, 3690.0f),
        cXyz(-15.0f, 2005.0f, 1180.0f),   cXyz(180.0f, 1840.0f, 1885.0f),
        cXyz(45.0f, 2260.0f, 1250.0f),    cXyz(600.0f, 1770.0f, 2225.0f),
        cXyz(2145.0f, 2410.0f, 1635.0f),
    };

    static u16 eff_Sand_id[2] = {0x8BF6, 0x8BF7};

    unused.set(1.0f, 1.0f, 1.0f);
    mHintTimer2 = l_HIO.mHintTime1;

    switch (mMode) {
    case 0:
        mGroundUpY = 150.0f;
        shape_angle.z = -8500;
        if (!mIsDemo) {
            break;
        }

        mMode++;
        // fallthrough
    case 1:
        if (!startDemoCheck()) {
            break;
        }

        mCameraCenter = mOp2CenterDt[0];
        mCameraEye = mOp2EyeDt[0];
        Z2GetAudioMgr()->subBgmStart(Z2BGM_HARAGIGANT_D02);
        mMode++;
        // fallthrough
    case 2:
        if (pla->current.pos.y < -745.0f) {
            break;
        }

        mCameraCenter = mOp2CenterDt[1];
        mCameraEye = mOp2EyeDt[1];
        mMode++;
        // fallthrough
    case 3:
        if (mIsDemo) {
            break;
        }

        mModeTimer = 30;
        mMode++;
        // fallthrough
    case 4:
        if (cLib_calcTimer(&mModeTimer) != 0) {
            break;
        }

        mMode++;
        // fallthrough
    case 5:
        vec = mOp2PlayerDt[0];
        daPy_getPlayerActorClass()->changeDemoMode(4, 2, 0, 0);
        daPy_getPlayerActorClass()->setPlayerPosAndAngle(&vec, 0x570, 0);

        mCameraCenter = mOp2CenterDt[2];
        mCameraEye = mOp2EyeDt[2];

        mSoundPos = current.pos;
        mSound.startCreatureSound(Z2SE_EN_DS_MDEMO_REBOOT, 0, -1);

        mGroundUpY = 0.0f;
        mModeTimer = 50;
        mMode++;
        // fallthrough
    case 6:
        if (cLib_calcTimer(&mModeTimer) != 0) {
            break;
        }

        mModeTimer = 18;
        daPy_getPlayerActorClass()->changeDemoMode(25, 0, 0, 0);
        mMode++;
        // fallthrough
    case 7:
        if (cLib_calcTimer(&mModeTimer) != 0) {
            break;
        }

        gravity = 0.0f;
        speed.y = 0.0f;
        mSound.startCreatureSoundLevel(Z2SE_EN_DS_H_FLOAT, 0, -1);

        daPy_getPlayerActorClass()->changeDemoMode(23, 1, 0, 0);
        daPy_getPlayerActorClass()->setPlayerPosAndAngle(&mOp2PlayerDt[0], -0x8000, 0);

        vec = mOp2PlayerDt[0];
        mCameraCenter = mOp2CenterDt[3];
        mCameraEye = mOp2EyeDt[3];

        current.pos.y = 2050.0f;
        current.pos.x = -140.0f;
        current.pos.z = 360.0f;

        shape_angle.x = 0;
        shape_angle.y = 0x1000;
        current.angle.y = 0x1000;
        shape_angle.z = 0;

        cLib_addCalc2(&mEyeColorAlpha, 255.0f, 0.7f, 5.0f);
        daPy_getPlayerActorClass()->changeDemoMode(23, 1, 2, 0);
        mMode++;
        // fallthrough
    case 8:
        cLib_addCalc2(&mEyeColorAlpha, 255.0f, 0.7f, 5.0f);
        for (int i = 0; i < 2; i++) {
            mSandParticleKey[i] = dComIfGp_particle_set(mSandParticleKey[i], eff_Sand_id[i],
                                                        &current.pos, &shape_angle, NULL);
            JPABaseEmitter* emitter = dComIfGp_particle_getEmitter(mSandParticleKey[i]);
            if (emitter != NULL) {
                emitter->setGlobalSRTMatrix(mpMorf->getModel()->getAnmMtx(DS_HEAD_JNT_HEAD));
            }
        }

        vec = pla->current.pos - mMouthPos;
        current.angle.x = vec.atan2sY_XZ();
        cLib_addCalcAngleS2(&shape_angle.x, current.angle.x, 20, 0x200);
        cLib_addCalc2(&field_0x7f8, 0.5f, 0.1f, 0.01f);
        cLib_addCalc2(&current.pos.y, 2400.0f, 0.8f, 5.0f);

        mSound.startCreatureSoundLevel(Z2SE_EN_DS_H_FLOAT, 0, -1);

        center = mSwordPos;
        center.y += -500.0f;
        if (center.y < mOp2CenterDt[3].y) {
            center.y = mOp2CenterDt[3].y;
        }

        if (center.y > mCameraCenter.y) {
            cLib_addCalc2(&mCameraCenter.y, center.y, 0.8f, 5.0f);
        }

        if (fabsf(current.pos.y - 2400.0f) > 2.0f) {
            break;
        }

        mModeTimer = 50;
        current.pos.y = 2400.0f;
        mMode = 10;
        // fallthrough
    case 10:
        vec = pla->current.pos - mMouthPos;
        current.angle.x = vec.atan2sY_XZ();
        cLib_addCalcAngleS2(&shape_angle.x, current.angle.x, 20, 0x200);

        if (cLib_calcTimer(&mModeTimer) != 0) {
            break;
        }

        vec = mOp2PlayerDt[1];
        daPy_getPlayerActorClass()->setPlayerPosAndAngle(&vec, -0x664a, 0);
        mModeTimer = 30;
        mMode++;
        // fallthrough

    case 11:
        current.pos.x = -140.0f;
        current.pos.z = 760.0f;

        mCameraCenter = mOp2CenterDt[4];
        mCameraEye = mOp2EyeDt[4];

        vec = pla->current.pos - mMouthPos;
        current.angle.x = vec.atan2sY_XZ();
        cLib_addCalcAngleS2(&shape_angle.x, current.angle.x, 20, 0x200);

        if (cLib_calcTimer(&mModeTimer) != 0) {
            break;
        }

        field_0x6f4 = current.pos;
        daPy_getPlayerActorClass()->changeDemoMode(25, 0, 0, 0);
        mModeTimer = 30;
        mMode++;
        break;
    case 12:
        if (cLib_calcTimer(&mModeTimer) != 0) {
            break;
        }

        mModeTimer = 30;
        daPy_getPlayerActorClass()->changeDemoMode(23, 1, 2, 0);
        mMode++;
        // fallthrough
    case 13:
        if (cLib_calcTimer(&mModeTimer) != 0) {
            break;
        }

        current.angle.y = shape_angle.y = fopAcM_searchPlayerAngleY(this);
        field_0x790.zero();
        mCameraCenter = mOp2CenterDt[5];
        mCameraEye = mOp2EyeDt[5];

        mModeTimer = 30;
        mMode++;
        // fallthrough
    case 14:
        if (cLib_calcTimer(&mModeTimer) != 0) {
            break;
        }

        setBck(ANM_HEAD_FATTACK_A, 0, 3.0f, 1.0f);
        field_0x718 = pla->current.pos;
        mMode++;
        // fallthrough
    case 15:
        if ((int)mpMorf->getFrame() < 30) {
            break;
        }

        if (mpMorf->checkFrame(35.0f)) {
            mSound.startCreatureVoice(Z2SE_EN_DS_H_V_ATK, -1);
        }

        daPy_getPlayerActorClass()->changeDemoMode(9, 1, 0, 0);
        speedF = 150.0f;

        dCam_getBody()->StartBlure(40, this, 0.8f, 1.2f);
        dComIfGp_getVibration().StartShock(6, 0x1f, cXyz(0.0f, 1.0f, 0.0f));
        mCameraCenter = mOp2CenterDt[6];
        mCameraEye = mOp2EyeDt[6];

        mModeTimer = 50;
        mMode++;
        // fallthrough
    case 16:
        cLib_addCalc0(&speedF, 0.7f, 10.0f);

        if (cLib_calcTimer(&mModeTimer) != 0) {
            if (mModeTimer == 20) {
                center.set(644.0f, -1495.0f, 2194.0f);
                eye.set(968.0f, -1421.0f, 2169.0f);
                camera->mCamera.Reset(center, eye);
                camera->mCamera.Start();
                camera->mCamera.SetTrimSize(0);
                dComIfGp_event_reset();
                dComIfGs_onOneZoneSwitch(6, fopAcM_GetRoomNo(this));
            }
        } else {
            vec.set(644.0f, -1600.0f, 2195.0f);
            dComIfGs_setRestartRoom(vec, -0x664A, 50);

            attention_info.distances[fopAc_attn_BATTLE_e] = 0;
            attention_info.flags = fopAc_AttnFlag_BATTLE_e;
            fopAcM_SetGroup(this, 2);

            Z2GetAudioMgr()->bgmStart(Z2BGM_HARAGIGANT_BTL02, 0, 0);
            mSetFirstPos();
            setActionMode(ACT_B2_WAIT, 0);
        }
    }

    if (mMode > 6) {
        cLib_addCalc(&mColBlend, 1.0f, 0.01f, 0.01f, 0.001f);
        dKy_custom_colset(3, 2, mColBlend);
    }

    if (mMode >= 10) {
        mSandPos.y = current.pos.y;
        mFuwafuwaSet(false);
    }

    if (mMode >= 2) {
        camera->mCamera.Set(mCameraCenter, mCameraEye);
    }
}

void daB_DS_c::mFlyBMove(f32 param_0) {
    cLib_addCalc2(&mChkHigh, param_0, 0.8f, 30.0f);
    mSandPos.y += mChkHigh;

    f32 step = fabsf(mSandPos.y - current.pos.y);
    if (step < 1.0f) {
        step = 1.0f;
    } else if (step > 30.0f) {
        step = 30.0f;
    }

    cLib_addCalc2(&current.pos.y, mSandPos.y, 0.8f, step);
}

void daB_DS_c::mFuwafuwaSet(bool param_0) {
    if (fabsf(current.pos.y - mSandPos.y) < 20.0f) {
        field_0x82c += 2.0f;
        cLib_addCalc2(&field_0x790.y, cM_ssin(field_0x82c * 1000.0f) * 50.0f, 0.8f, 10.0f);
        cLib_addCalc2(&field_0x790.x, cM_ssin(field_0x82c * 500.0f) * 50.0f, 0.5f, field_0x804);
        cLib_addCalc2(&field_0x790.z, cM_scos(field_0x82c * 500.0f) * 50.0f, 0.5f, field_0x804);
        cLib_addCalc2(&field_0x804, 4.0f, 0.8f, 0.3f);
    }

    mSoundPos = current.pos;
    mSound.startCreatureSoundLevel(Z2SE_EN_DS_H_FLOAT, 0, -1);

    if (param_0) {
        daPy_py_c* pla = daPy_getPlayerActorClass();
        dBgS_GndChk gnd_chk;
        f32 cHigh = 1000.0f;

        mSandPos.y = pla->current.pos.y;

        if (field_0x7ce > 0x2800) {
            field_0x7ce = 0x2800;
        }

        if (mAction != ACT_B2_TIRED) {
            if (daPy_getPlayerActorClass()->checkSpinnerPathMove() ||
                (daPy_getPlayerActorClass()->getSpinnerActor() != NULL &&
                 !daPy_getPlayerActorClass()->getSpinnerActor()->checkGroundHit()))
            {
                cLib_calcTimer(&mOutTimer);

                if (pla->current.pos.y < l_HIO.mP2AttackHeight && mOutTimer != 0) {
                    mBh2AttackAngleF = 0x4800;
                    field_0x7ce = 0x4800;
                    mCreateFireBreath = false;

                    if (mParticleKey2 != 0) {
                        JPABaseEmitter* emitter = dComIfGp_particle_getEmitter(mParticleKey2);
                        if (emitter != NULL) {
                            emitter->stopDrawParticle();
                            mParticleKey2 = 0;
                        }
                    }
                } else {
                    mOutTimer = 0;
                    mBirthTrapTimerF = 0;
                }
                cHigh = 300.0f;
            }
        } else {
            cHigh = 300.0f;
        }

        cLib_addCalcAngleS2(&mBh2AttackAngleF, field_0x7ce, 20, 0x200);

        if (WREG_S(2)) {
            OS_REPORT("mBh2AttackAngleF %x\n", (s16)mBh2AttackAngleF);
            OS_REPORT("mSandPos.y %f\n", mSandPos.y);
            OS_REPORT("pla->current.pos.y %f\n", pla->current.pos.y);
            OS_REPORT("cHigh %f\n", cHigh);
        }

        mFlyBMove(cHigh);
    }
}

void daB_DS_c::mBattle2BreathFireSet(u32 i_params) {
    fopAcM_createChild(PROC_B_DS, fopAcM_GetID(this), i_params, &mMouthPos, fopAcM_GetRoomNo(this),
                       &shape_angle, NULL, -1, NULL);
    mCreateFireBreath = false;
}

bool daB_DS_c::mNeckAngleSet() {
    s16 svar5 = 0;
    cXyz vec = daPy_getPlayerActorClass()->current.pos - mMouthPos;
    if (daPy_getPlayerActorClass()->checkSpinnerRide() && !mRailInOutCheck()) {
        svar5 = -0x52C;
    }

    cLib_addCalcAngleS2(&current.angle.y, svar5 + vec.atan2sX_Z(), 2, 0x1000);
    cLib_addCalcAngleS2(&current.angle.x, (s16)vec.atan2sY_XZ(), 8, 0x200);
    shape_angle = current.angle;
    return abs((s16)(shape_angle.y - (svar5 + vec.atan2sX_Z()))) <= 0x200;
}

void daB_DS_c::mSetFirstPos() {
    daPy_py_c* pla = daPy_getPlayerActorClass();
    home.pos.set(0.0f, 1900.0f, 0.0f);

    cXyz offset, vec2;
    vec2 = pla->current.pos - home.pos;
    vec2.y = 0.0f;

    mDoMtx_YrotS(*calc_mtx, vec2.atan2sX_Z() - 0x4800);

    offset.x = 0.0f;
    offset.y = 0.0f;
    offset.z = l_HIO.mP2MoveAxis;
    MtxPosition(&offset, &current.pos);
    current.pos += home.pos;

    mEyeColorAlpha = 255.0f;
    shape_angle.z = 0;
}

bool daB_DS_c::mChkScreenIn() {
    camera_class* camera = dComIfGp_getCamera(0);
    cXyz vec1, vec2;

    vec2 = camera->lookat.eye - camera->lookat.center;
    vec1 = camera->lookat.eye - current.pos;
    return abs((s16)(vec1.atan2sX_Z() - vec2.atan2sX_Z())) < 0x3000;
}

bool daB_DS_c::mRailInOutCheck() {
    cXyz vec = daPy_getPlayerActorClass()->current.pos - home.pos;
    vec.y = 0.0f;
    return !(vec.abs() < l_HIO.mP2MoveAxis);
}

void daB_DS_c::executeBattle2Wait() {
    switch (mMode) {
    case 0:
        mBh2AttackAngleF = 0x4800;
        field_0x7ce = 0x4800;
        mOutTimer = l_HIO.mP2OuterWallAttackTime;
        mPlayPatternAnm = false;
        field_0x82c = 0;
        field_0x808 = 0.0f;
        mCreateFireBreath = false;
        mModeTimer = 0;
        mWallR = 500.0f;

        setBck(ANM_HEAD_FWAIT, 2, 10.0f, 1.0f);
        mBirthTrapTimerF = l_HIO.mP2TrapCreateWaitTime1;
        mTrapCreate = false;
        setActionMode(ACT_B2_F_MOVE, 0);
        break;
    }

    mNeckAngleSet();
    mFuwafuwaSet(true);
}

bool daB_DS_c::mBattle2MoveFSet() {
    daPy_py_c* pla = daPy_getPlayerActorClass();
    f32 move_axis = l_HIO.mP2MoveAxis;
    bool cvar6 = mRailInOutCheck();

    cXyz offset, ato;
    ato = field_0x718 - current.pos;
    ato.y = 0.0f;

    if (mBh2AttackAngleF <= 0x2880 && ato.abs() < 500.0f) {
        if (!cvar6) {
            move_axis += 600.0f;
        } else if (mAction != ACT_B2_TIRED) {
            move_axis += 700.0f;
        } else {
            move_axis -= 700.0f;
        }
    }

    cLib_addCalc2(&field_0x80c, move_axis, 1.0f, 20.0f);
    bool ret = false;

    ato = pla->current.pos - home.pos;
    ato.y = 0.0f;
    mDoMtx_YrotS(*calc_mtx, ato.atan2sX_Z() - mBh2AttackAngleF);
    offset.x = 0.0f;
    offset.y = 0.0f;
    offset.z = field_0x80c;
    MtxPosition(&offset, &field_0x718);

    ato = field_0x718 - current.pos;
    ato.y = 0.0f;
    f32 spdF = ato.abs() - field_0x80c;
    if (spdF < 10.0f || !mChkScreenIn()) {
        spdF = 10.0f;
    } else if (spdF > 30.0f) {
        spdF = 30.0f;
    }
    spdF += pla->speedF * 2.0f;

    ato = pla->current.pos - home.pos;
    ato.y = 0.0f;
    if (ato.abs() > 2000.0f) {
        cLib_addCalc2(&field_0x808, spdF, 1.0f, 10.0f);
        cLib_addCalc2(&current.pos.x, field_0x718.x, 1.0f, field_0x808);
        cLib_addCalc2(&current.pos.z, field_0x718.z, 1.0f, field_0x808);

        ato = field_0x718 - current.pos;
        ato.y = 0.0f;
        if (mOutTimer == 0 || ato.abs() < 200.0f) {
            ret = true;
        }

        if (cLib_calcTimer(&mBirthTrapTimerF) == 0) {
            if (mTrapID[6] != 0) {
                mBirthTrapTimerF = l_HIO.mP2TrapCreateWaitTime2;
            } else {
                mBirthTrapTimerF = l_HIO.mP2TrapCreateWaitTime1;
            }

            mTrapCreate = true;
        }
    }

    if (WREG_S(1)) {
        OS_REPORT("ato.abs() %f\n", ato.abs());
        OS_REPORT("mBirthTrapTimerF %d\n", mBirthTrapTimerF);
        OS_REPORT("spdF %f\n", spdF);
        OS_REPORT("pla->speedF %f\n\n", pla->getSpeedF());
    }

    if (daPy_getPlayerActorClass()->checkSpinnerPathMove()) {
        field_0x6f4 = pla->current.pos;
    }

    return ret;
}

void daB_DS_c::executeBattle2FMove() {
    daPy_py_c* pla = daPy_getPlayerActorClass();
    bool mMvFlag = mBattle2MoveFSet();
    bool mAnF = false;
    cXyz vec;

    if (!mMvFlag && daPy_getPlayerActorClass()->checkSpinnerRide()) {
        vec = field_0x718 - current.pos;
        cLib_addCalcAngleS2(&current.angle.y, vec.atan2sX_Z(), 1, 0x400);
        shape_angle.y = current.angle.y;
    } else {
        mAnF = mNeckAngleSet();
    }

    switch (mMode) {
    case 0:
        mModeTimer = l_HIO.mP2BulletFireTime;
        mCreateFireBreath = false;
        setBck(ANM_HEAD_FWAIT, 2, 10.0f, 1.0f);

        if (!daPy_getPlayerActorClass()->checkSpinnerRide()) {
            field_0x808 = 0.0f;
            mOutTimer = l_HIO.mP2OuterWallAttackTime;
        }
        mMode++;
        // fallthrough
    case 1:
    case 2:
        vec = pla->current.pos - home.pos;
        vec.y = 0.0f;
        if (cLib_calcTimer(&mModeTimer) != 0) {
            if (mModeTimer > 5) {
                if (vec.abs() < 2000.0f) {
                    mModeTimer = 5;
                }
            }
        } else {
            if (vec.abs() > 2000.0f && (!mMvFlag || !mAnF || mBh2AttackAngleF > 0x2880)) {
                if (WREG_S(3)) {
                    OS_REPORT("mMvFlag %d\n", mMvFlag);
                    OS_REPORT("mAnF %d\n", mAnF);
                    OS_REPORT("mBh2AttackAngleF %x\n", (s16)mBh2AttackAngleF);
                }
                break;
            }

            if (daPy_getPlayerActorClass()->getDamageWaitTimer() != 0) {
                mOutTimer = l_HIO.mP2OuterWallAttackTime;
                mTrapCreate = false;
            } else {
                if (mMode == 1) {
                    if (field_0x7ce > 0x2800) {
                        field_0x7ce = 0x2800;
                    }
                } else {
                    setBck(ANM_HEAD_FATTACK_A, 0, 3.0f, 1.0f);
                }

                mMode++;
            }
        }
        break;
    case 3:
        if (!mpMorf->isStop()) {
            if (mpMorf->checkFrame(35.0f)) {
                mSound.startCreatureVoice(Z2SE_EN_DS_H_V_ATK, -1);
            }
            break;
        }

        setBck(ANM_HEAD_FATTACK_B, 2, 3.0f, 1.0f);
        mCreateFireBreath = true;
        mMode++;
        // fallthrough
    case 4:
        if (mCreateFireBreath) {
            field_0x6a8 = 3;
            break;
        }

        if (daPy_getPlayerActorClass()->checkSpinnerRide() &&
            daPy_getPlayerActorClass()->getDamageWaitTimer() == 0)
        {
            field_0x7ce -= l_HIO.mP2ApproachAfterBullet;
            if (field_0x7ce <= l_HIO.mP2ApproachDist) {
                field_0x6a8 = 2;
                setActionMode(ACT_B2_TIRED, 0);
                break;
            }
        } else if (!daPy_getPlayerActorClass()->checkSpinnerRide() ||
                   (daPy_getPlayerActorClass()->getSpinnerActor() != NULL &&
                    daPy_getPlayerActorClass()->getSpinnerActor()->checkGroundHit()))
        {
            setBck(ANM_HEAD_FATTACK_C, 0, 3.0f, 1.0f);
            mMode = 100;
            break;
        }

        setBck(ANM_HEAD_FATTACK_C, 0, 3.0f, 1.0f);
        mMode = 10;
        break;
    case 10:
    case 100:
        if (mpMorf->isStop()) {
            if (mMode == 10) {
                mMode = 0;
            } else {
                setActionMode(ACT_B2_WAIT, 0);
            }
        }
        break;
    }

    if (cLib_calcTimer(&field_0x6a8) == 1 && mParticleKey2 != 0) {
        JPABaseEmitter* emitter = dComIfGp_particle_getEmitter(mParticleKey2);
        if (emitter != NULL) {
            emitter->stopDrawParticle();
        }
    }

    mFuwafuwaSet(true);
}

void daB_DS_c::executeBattle2Tired() {
    switch (mMode) {
    case 0:
        setBck(ANM_HEAD_FATTACK_A, 0, 3.0f, 1.0f);
        mTimerCount = 0;
        mMode++;
        break;
    case 1:
        if (mAnmID == ANM_HEAD_FATTACK_A) {
            if (mpMorf->checkFrame(35.0f)) {
                mSound.startCreatureVoice(Z2SE_EN_DS_H_V_ATK, -1);
            }
            if (mpMorf->isStop()) {
                setBck(ANM_HEAD_FATTACK_B, 2, 3.0f, 1.0f);
            }
        }

        if (abs((s16)(mBh2AttackAngleF - 0x800)) > 0x100) {
            break;
        }

        mCreateFireBreath = true;
        mMode++;
        // fallthrough
    case 2:
        if (!mCreateFireBreath) {
            setBck(ANM_HEAD_FATTACK_C, 0, 3.0f, 1.0f);
            mMode++;
        }
        break;
    case 3:
        if (mpMorf->isStop()) {
            mMode = 777;
        }
        break;
    }

    if (!daPy_getPlayerActorClass()->checkSpinnerRide() ||
        daPy_getPlayerActorClass()->getDamageWaitTimer() != 0 || mMode == 777 ||
        (daPy_getPlayerActorClass()->getSpinnerActor() != NULL &&
         daPy_getPlayerActorClass()->getSpinnerActor()->checkGroundHit()))
    {
        setActionMode(ACT_B2_WAIT, 0);
    }

    cLib_addCalcAngleS2(&mBh2AttackAngleF, 0x800, l_HIO.mP2ApproachAccel,
                        l_HIO.mP2ApproachSpeedMax);

    mSandPos.y = daPy_getPlayerActorClass()->current.pos.y;
    mNeckAngleSet();
    mFlyBMove(100.0f);
    mBattle2MoveFSet();
    mFuwafuwaSet(false);
}

void daB_DS_c::executeBattle2Damage() {
    camera_class* camera = dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0));
    daPy_py_c* pla = daPy_getPlayerActorClass();

    JPABaseEmitter* emitter;
    dBgS_LinChk lin_chk;
    dBgS_GndChk gnd_chk;
    cXyz vec1, vec2, chk_pos;
    cXyz particle_scale(1.5f, 1.5f, 1.5f);
    csXyz particle_angle;

    chk_pos = current.pos;
    chk_pos.y += 300.0f;
    gnd_chk.SetPos(&chk_pos);
    chk_pos.y = dComIfG_Bgsp().GroundCross(&gnd_chk);
    if (chk_pos.y == -G_CM3D_F_INF) {
        chk_pos.y = current.pos.y - 200.0f;
    }

    switch (mMode) {
    case 0:
        if (!startDemoCheck()) {
            break;
        }

        vec2 = pla->current.pos - home.pos;
        vec2.y = 0.0f;

        field_0x7d2 = vec2.atan2sX_Z();
        mDoMtx_YrotS(*calc_mtx, field_0x7d2);

        vec1.x = 0.0f;
        vec1.y = 0.0f;
        vec1.z = l_HIO.mP2MoveAxis;
        MtxPosition(&vec1, &vec2);
        vec2.y = pla->current.pos.y;
        daPy_getPlayerActorClass()->setPlayerPosAndAngle(&vec2, -vec2.atan2sX_Z(), 0);

        mHintTimer2 = 0;

        emitter = dComIfGp_particle_getEmitter(mParticleKey1);
        if (emitter != NULL) {
            emitter->stopDrawParticle();
        }

        shape_angle.x = 0;
        current.angle.x = 0;
        setBck(ANM_HEAD_DAMAGE, 0, 3.0f, 1.0f);

        for (int i = 0; i < 5; i++) {
            mHeadSph[i].OnTgSpinnerReflect();
            mHeadSph[i].OnTgIronBallRebound();
            mHeadSph[i].OffTgShield();
            mHeadSph[i].SetTgType(0);
            mHeadSph[i].SetTgHitMark(CcG_Tg_UNK_MARK_0);
        }

        shape_angle.x = 0;
        mTimerCount = 0;
        daPy_getPlayerActorClass()->changeOriginalDemo();
        daPy_getPlayerActorClass()->changeDemoMode(14, 0, 0, 0);

        vec2 = field_0x6f4 - home.pos;
        vec2.y = 0.0f;
        vec1 = home.pos - field_0x6f4;
        vec1.y = 0.0f;
        current.angle.y = vec1.atan2sX_Z();

        if (vec2.abs() < l_HIO.mP2MoveAxis) {
            mAttackingHand = 0;
            // "Player hit from right \n"
            OS_REPORT("右からプレイヤ当り \n");
            current.angle.y += 0x8000;
            field_0x7d2 = vec2.atan2sX_Z() + 0x800;
        } else {
            mAttackingHand = 1;
            // "Player hit from left \n"
            OS_REPORT("左からプレイヤ当り \n");
            field_0x7d2 = vec2.atan2sX_Z() + 0x1800;
        }

        mDoMtx_YrotS(*calc_mtx, current.angle.y);

        vec1.x = 0.0f;
        vec1.y = 0.0f;
        vec1.z = 4000.0f;
        MtxPosition(&vec1, &field_0x79c);

        mWallR = 300.0f;
        field_0x79c += current.pos;
        if (field_0x79c.y > 1400.0f) {
            field_0x79c.y = 1400.0f;
            current.pos.y = 1400.0f;
        }

        mSmokeCount = 0;
        mMode++;
        break;
    case 1:
        mDoMtx_YrotS(*calc_mtx, field_0x7d2);

        vec1.x = 0.0f;
        vec1.y = 200.0f;
        vec1.z = l_HIO.mP2MoveAxis;
        MtxPosition(&vec1, &mCameraEye);
        mCameraEye.y += current.pos.y;
        mCameraCenter = current.pos;

        vec2 = mCameraEye - mCameraCenter;
        shape_angle.y = vec2.atan2sX_Z();
        cLib_addCalcPos(&current.pos, field_0x79c, 0.8f, 50.0f, 7.0f);

        if (!mAcch.ChkWallHit()) {
            break;
        }

        mSound.startCreatureSound(Z2SE_EN_DS_H_COL_WALL, 0, -1);

        particle_angle = shape_angle;
        particle_angle.x = 0x4000;
        particle_angle.z = 0;
        field_0x6d0 = mMouthPos;
        mHitParticleKey[0] = dComIfGp_particle_set(mHitParticleKey[0], eff_smoke_id[1],
                                                   &field_0x6d0, &particle_angle, &particle_scale);

        dScnPly_c::setPauseTimer(5);
        dComIfGp_getVibration().StartShock(7, 0x1f, cXyz(0.0f, 1.0f, 0.0f));

        current.angle.y = shape_angle.y;
        speedF = 0.0f;
        field_0x7d0 = 0;
        gravity = -3.0f;
        mEyeColorAlpha = 0.0f;
        mMode++;
        // fallthrough
    case 2:
        mCameraCenter = current.pos;
        if (!mAcch.ChkGroundHit() && current.pos.y - 100.0f > chk_pos.y) {
            break;
        }

        if (mAnmID != ANM_HEAD_DOWN) {
            setBck(ANM_HEAD_DOWN, 0, 3.0f, 1.0f);
        }

        gravity = 0.0f;
        speed.y = 0.0f;

        particle_angle = shape_angle;
        particle_angle.z = 0;
        particle_angle.x = 0;
        vec2 = current.pos;
        vec2.y = mAcch.GetGroundH();
        mHitParticleKey[1] = dComIfGp_particle_set(mHitParticleKey[1], eff_smoke_id[1], &vec2,
                                                   &particle_angle, &particle_scale);

        mSound.startCreatureSound(Z2SE_EN_DS_H_COL, 0, -1);
        dComIfGp_getVibration().StartShock(3, 0x4f, cXyz(0.0f, 1.0f, 0.0f));
        mModeTimer = 50;
        mMode++;
        // fallthrough
    case 3:
        if (cLib_calcTimer(&mModeTimer) != 0) {
            break;
        }

        camera->mCamera.Reset(mCameraCenter, mCameraEye);
        camera->mCamera.Start();
        camera->mCamera.SetTrimSize(0);
        dComIfGp_event_reset();

        attention_info.distances[fopAc_attn_BATTLE_e] = 0x16;
        mWeakSph.OnTgSetBit();
        mP2FallTimer = l_HIO.mP2FallTime;
        mMode++;
        // fallthrough
    case 4:
        if (mBattle2_damage_check()) {
            return;
        }

        mWeakSph.SetC(mSwordPos);
        mWeakSph.SetR(140.0f);
        dComIfG_Ccsp()->Set(&mWeakSph);

        if (cLib_calcTimer(&mP2FallTimer) != 0) {
            mParticleKey1 =
                dComIfGp_particle_set(mParticleKey1, 0x8bf5, &current.pos, &shape_angle, NULL);
            emitter = dComIfGp_particle_getEmitter(mParticleKey1);
            if (emitter != NULL) {
                emitter->setGlobalSRTMatrix(mpMorf->getModel()->getAnmMtx(DS_HEAD_JNT_HEAD));
            }

            Z2GetAudioMgr()->seStartLevel(Z2SE_EN_DS_SMOKE_2, &mSwordPos, 0, 0, 1.0f, 1.0f, -1.0f,
                                          -1.0f, 0);
            break;
        }

        emitter = dComIfGp_particle_getEmitter(mParticleKey1);
        if (emitter != NULL) {
            emitter->stopDrawParticle();
        }

        attention_info.distances[fopAc_attn_BATTLE_e] = 0;
        setBck(ANM_HEAD_DOWN_UP, 0, 3.0f, 1.0f);
        Z2GetAudioMgr()->changeBgmStatus(1);
        mWeakSph.OffTgSetBit();
        mMode++;
        // fallthrough
    case 5:
        mDamageTimer = 100;

        if (!mChkScreenIn()) {
            mSetFirstPos();
            for (int i = 0; i < 5; i++) {
                mHeadSph[i].OnTgShield();
                mHeadSph[i].SetTgType(0xd8fbfdff);
                mHeadSph[i].SetTgHitMark(CcG_Tg_UNK_MARK_2);
                mHeadSph[i].OffTgSpinnerReflect();
                mHeadSph[i].OffTgIronBallRebound();
            }
            mDamageTimer = 100;
            setActionMode(ACT_B2_WAIT, 0);
            return;
        }

        if ((int)mpMorf->getFrame() >= 20) {
            if (mpMorf->checkFrame(20.0f)) {
                mSound.startCreatureSound(Z2SE_EN_DS_H_DOWN_UP, 0, -1);
            }

            cLib_addCalc2(&mWallR, 500.0f, 0.7f, 30.0f);
            cLib_addCalcAngleS2(&shape_angle.z, 0, 4, 0x400);
            cLib_addCalc2(&mEyeColorAlpha, 255.0f, 0.7f, 10.0f);
            mFuwafuwaSet(true);

            if (mEyeColorAlpha > 254.0f && shape_angle.z < 0x100) {
                mEyeColorAlpha = 255.0f;
                shape_angle.z = 0;
                for (int i = 0; i < 5; i++) {
                    mHeadSph[i].OnTgShield();
                    mHeadSph[i].SetTgType(0xd8fbfdff);
                    mHeadSph[i].SetTgHitMark(CcG_Tg_UNK_MARK_2);
                    mHeadSph[i].OffTgSpinnerReflect();
                    mHeadSph[i].OffTgIronBallRebound();
                }
                setActionMode(ACT_B2_WAIT, 0);
                return;
            }
        }
        break;
    }

    if (mMode >= 1 && mMode <= 3) {
        camera->mCamera.Set(mCameraCenter, mCameraEye);
        cLib_addCalc0(&mEyeColorAlpha, 0.7f, 10.0f);
    }

    if (mMode >= 2 && mMode <= 3) {
        s16 target_z = 13000;
        if (mAttackingHand == 0) {
            target_z = -13000;
        }
        cLib_addCalcAngleS2(&shape_angle.z, target_z, 4, 0x400);
    }

    if (mP2FallTimer != 0) {
        if (mP2FallTimer > l_HIO.mP2FallTime / 3.0f) {
            return;
        }

        if ((mP2FallTimer & 32) == 0) {
            cLib_addCalc0(&mEyeColorAlpha, 0.8f, 10.0f);
        } else {
            cLib_addCalc2(&mEyeColorAlpha, 255.0f, 0.8f, 10.0f);
        }
    }
}

bool daB_DS_c::mDeadMove() {
    mTimerCount++;
    cXyz vec1;
    cXyz vec2 = home.pos - (current.pos + field_0x790);
    vec2.y = 0.0f;

    if (mTimerCount < 80) {
        if (mAttackingHand != 0) {
            shape_angle.z -= 0x80;
        } else {
            shape_angle.z += 0x80;
        }

        field_0x83c++;
        cLib_addCalc2(&field_0x790.x, cM_ssin(field_0x83c * -300.0f) * 1000.0f, 0.8f, 50.0f);
        cLib_addCalc2(&field_0x790.z, cM_scos(field_0x83c * -100.0f) * -200.0f, 0.8f, 50.0f);

        vec2 = home.pos - (current.pos + field_0x790);
        vec2.y = (home.pos.y + 8000.0f) - current.pos.y;
        if (mTimerCount == 1) {
            current.angle.y = vec2.atan2sX_Z();
            speedF = 40.0f;
        }
        current.angle.x = vec2.atan2sY_XZ();

        mDoMtx_YrotS(*calc_mtx, current.angle.y);
        mDoMtx_XrotM(*calc_mtx, current.angle.x);
        vec1.x = 0.0f;
        vec1.y = 0.0f;
        vec1.z = 80.0f;
        MtxPosition(&vec1, &vec2);
        speed.y = vec2.y;

        field_0x861 = false;
        mModeTimer = 30;
    } else {
        if (mAnmID != ANM_HEAD_DAMAGE) {
            setBck(ANM_HEAD_DAMAGE, 0, 3.0f, 1.0f);
        }

        if (cLib_calcTimer(&mModeTimer) == 0) {
            field_0x861 = true;
            if (mAttackingHand != 0) {
                shape_angle.z -= 0x80;
                cLib_addCalc2(&speedF, 20.0f, 0.8f, 3.0f);
            } else {
                shape_angle.z += 0x80;
                cLib_addCalc2(&speedF, 17.0f, 0.8f, 3.0f);
            }

            vec2 = field_0x718 - (current.pos + field_0x790);
            field_0x7ba.z = 0;
            current.angle.x = vec2.atan2sY_XZ();
            current.angle.y = vec2.atan2sX_Z();

            mDoMtx_YrotS(*calc_mtx, current.angle.y);
            mDoMtx_XrotM(*calc_mtx, current.angle.x);
            vec1.x = 0.0f;
            vec1.y = 0.0f;
            vec1.z = 40.0f;
            MtxPosition(&vec1, &vec2);
            speed.y = vec2.y;
        } else {
            speed.y = 0.0f;
            cLib_addCalc0(&speedF, 0.7f, 2.0f);
        }
        cLib_addCalc0(&mEyeColorAlpha, 0.7f, 3.0f);
    }

    s16 angle_scale, angle_step;
    if (!field_0x861) {
        angle_scale = 8;
        angle_step = 0x200;
    } else {
        angle_scale = 40;
        angle_step = 0x100;
    }

    cLib_addCalcAngleS2(&shape_angle.x, current.angle.x, angle_scale, angle_step);
    cLib_addCalcAngleS2(&shape_angle.y, current.angle.y, angle_scale, angle_step);

    vec2 = current.pos + field_0x790;
    cLib_addCalcPos(&mCameraCenter, vec2, 1.0f, 100.0f, 20.0f);

    if (mTimerCount < 80 || home.pos.y + 700.0f < current.pos.y) {
        return false;
    } else {
        return true;
    }
}

void daB_DS_c::executeBattle2Dead() {
    static cXyz mEd2CenterDt[2] = {
        cXyz(70.0f, 1890.0f, -1190.0f),
        cXyz(-90.0f, 1890.0f, -2200.0f),
    };
    static cXyz mEd22EyeDt[2] = {
        cXyz(105.0f, 1880.0f, -945.0f),
        cXyz(110.0f, 1880.0f, -885.0f),
    };
    static u16 eff_Demo_sasi_id[2] = {0x8BF8, 0x8BF9};

    camera_class* camera = dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0));
    csXyz particle_angle = shape_angle;
    cXyz mae, ato;
    cXyz particle_scale(1.5f, 1.5f, 1.5f);
    cXyz item_scale;

    switch (mMode) {
    case 0:
        if (!startDemoCheck()) {
            break;
        }

        mSmokeCount = 0;
        field_0x6f4 = current.pos;
        home.angle = shape_angle;
        field_0x7c0 = current.angle;
        dComIfGp_getVibration().StartShock(5, 0x4f, cXyz(0.0f, 1.0f, 0.0f));
        // fallthrough
    case 100:
        field_0x790.zero();
        setBck(ANM_HEAD_DIEB, 2, 3.0f, 1.0f);

        mEyeColorAlpha = 255.0f;
        mTimerCount = 0;
        mCameraFovy = 50.0f;
        mWallR = 500.0f;
        mGroundUpY = 500.0f;

        ato = current.pos - home.pos;
        if (mAttackingHand != 0) {
            field_0x7d2 = ato.atan2sX_Z() + 0x2000;
        } else {
            field_0x7d2 = ato.atan2sX_Z() - 0x8000;
        }

        field_0x83c = 0;
        speedF = 0.0f;
        speed.y = 0.0f;
        gravity = 0.0f;
        current.angle.y = field_0x7d2;
        daPy_getPlayerActorClass()->changeDemoMode(4, 1, 0, 0);

        mDoMtx_YrotS(*calc_mtx, field_0x7d2);

        mae.x = 0.0f;
        mae.y = 0.0f;
        mae.z = 1500.0f;
        MtxPosition(&mae, &ato);
        ato += field_0x6f4;
        mCameraEye = ato;
        mMode = 1;
        // fallthrough
    case 1:
        shape_angle.z += 0x100;
        if (current.pos.y < 1700.0f) {
            mCameraCenter = current.pos;
            ato = current.pos;
            ato.y = 2500.0f;
            cLib_addCalcPos(&current.pos, ato, 1.0f, 90.0f, 45.0f);
            cLib_addCalcAngleS2(&shape_angle.x, -0x4000, 4, 0x400);
            break;
        }

        speedF = 0.0f;
        mMode++;
        // fallthrough
    case 2:
        mWallR = 650.0f;
        ato = current.pos - home.pos;
        field_0x80c = 3400.0f;

        mDoMtx_YrotS(*calc_mtx, ato.atan2sX_Z());

        mae.x = 0.0f;
        mae.y = 0.0f;
        mae.z = field_0x80c;
        MtxPosition(&mae, &ato);
        ato += home.pos;

        current.pos.x = ato.x;
        current.pos.y = 2500.0f;
        current.pos.z = ato.z;

        ato = home.pos - current.pos;
        ato.y = 0.0f;
        field_0x80c = 3600.0f;

        mDoMtx_YrotS(*calc_mtx, ato.atan2sX_Z() + 10000);

        mae.x = 0.0f;
        mae.y = 0.0f;
        mae.z = field_0x80c;
        MtxPosition(&mae, &ato);
        ato += home.pos;
        ato.y = 3400.0f;
        mCameraEye = ato;

        field_0x80c = 3600.0f;
        ato = home.pos - current.pos;
        mDoMtx_YrotS(*calc_mtx, ato.atan2sX_Z());
        mDoMtx_XrotM(*calc_mtx, ato.atan2sY_XZ());
        mae.x = 0.0f;
        mae.y = 100.0f;
        mae.z = field_0x80c;
        MtxPosition(&mae, &ato);
        field_0x718 = ato + home.pos;
        ato = field_0x718 - current.pos;
        field_0x7ba.x = ato.atan2sY_XZ();
        field_0x7ba.y = ato.atan2sX_Z();
        field_0x7ba.z = 0;
        current.angle = field_0x7ba;

        mCameraCenter = current.pos;
        mMode++;
        // fallthrough
    case 3:
        if (!mDeadMove()) {
            break;
        }

        speedF = 0.0f;
        speed.y = 0.0f;
        gravity = 0.0f;
        mEyeColorAlpha = 0.0f;
        field_0x790.zero();
        field_0x7d2 -= 0x1000;

        setBck(ANM_HEAD_DIE, 2, 3.0f, 1.0f);
        speedF = 0.0f;
        mMode++;
        // fallthrough
    case 4:
        current.pos = field_0x718;
        shape_angle.z = 0x5277;
        shape_angle.x = 0x4a4;
        current.angle.x = 0x4a4;

        ato = home.pos - current.pos;
        ato.y = 0.0f;
        mDoMtx_YrotS(*calc_mtx, ato.atan2sX_Z() - 40000);
        mae.x = 0.0f;
        mae.y = 0.0f;
        mae.z = 3000.0f;
        MtxPosition(&mae, &ato);
        ato += home.pos;
        ato.y -= 3000.0f;
        mCameraEye = ato;

        ato = home.pos - current.pos;
        mae = field_0x718 - current.pos;

        if (BREG_S(1)) {
            OS_REPORT("ato.abs() %f\n", ato.abs());
            OS_REPORT("mae.abs() %f\n", mae.abs());
        }

        mSound.startCreatureSound(Z2SE_EN_DS_END_COL, 0, -1);

        gravity = 0.0f;
        speed.y = 0.0f;
        particle_angle = shape_angle;
        particle_angle.x = 0x4000;
        particle_angle.z = 0;
        mDeathParticleKey = dComIfGp_particle_set(mDeathParticleKey, eff_smoke_id[1], &field_0x718,
                                                  &particle_angle, &particle_scale);

        current.angle.y = shape_angle.y;
        ato = mCameraEye - current.pos;
        ato.y = mCameraEye.y - current.pos.y;
        current.angle.y = ato.atan2sX_Z() - 0x800;
        current.angle.x = ato.atan2sY_XZ();

        mModeTimer = 10;
        dComIfGp_getVibration().StartShock(6, 0x4f, cXyz(0.0f, 1.0f, 0.0f));
        mMode++;
        break;
    case 5:
        speedF = 50.0f;
        mGroundUpY = 400.0f;

        mDoMtx_YrotS(*calc_mtx, current.angle.y);
        mDoMtx_XrotM(*calc_mtx, current.angle.x);
        mae.x = 0.0f;
        mae.y = 0.0f;
        mae.z = speedF;
        MtxPosition(&mae, &ato);
        speed.y = ato.y;

        cLib_addCalcAngleS2(&shape_angle.y, current.angle.y, 20, 0x100);
        cLib_addCalcAngleS2(&shape_angle.x, current.angle.x, 20, 0x100);
        if (cLib_calcTimer(&mModeTimer) == 0) {
            mMode++;
        }
        break;
    case 6:
        if (mAcch.ChkGroundHit()) {
            mGroundUpY = 500.0f;
            dComIfGp_getVibration().StartShock(5, 0x4f, cXyz(0.0f, 1.0f, 0.0f));
            speedF = 50.0f;
            speed.y = 70.0f;
            gravity = -5.0f;
            mMode++;
        }
        break;
    case 7:
        ato.zero();
        ato.y = mAcch.GetGroundH() - current.pos.y;
        current.angle.x = ato.atan2sY_XZ();
        cLib_addCalcAngleS2(&shape_angle.x, 0, 8, 0x200);
        cLib_addCalcAngleS2(&shape_angle.z, -0x4000, 10, 0x200);

        if (mAcch.ChkGroundHit()) {
            dComIfGp_getVibration().StartShock(4, 0x4f, cXyz(0.0f, 1.0f, 0.0f));
            current.angle.y -= 0x1830;
            mGroundUpY = 400.0f;
            speedF = 120.0f;
            speed.y = 50.0f;
            gravity = -5.0f;
            mMode = 10;
        }
        break;
    case 10:
    case 12:
    case 14:
        cLib_addCalcAngleS2(&shape_angle.z, -0x4000, 10, 0x200);
        if (mAcch.ChkGroundHit()) {
            dComIfGp_getVibration().StartShock(3, 0x4f, cXyz(0.0f, 1.0f, 0.0f));
            mMode++;
        }
        break;
    case 11:
    case 13:
    case 15:
        if (mMode != 15) {
            speedF = 180.0f;
            speed.y = 20.0f;
            gravity = -5.0f;
        } else {
            mModeTimer = 40;
        }
        mMode++;
        break;
    case 16:
        if (cLib_calcTimer(&mModeTimer) != 0) {
            break;
        }

        gravity = 0.0f;
        speed.y = 0.0f;
        speedF = 0.0f;

        ato = current.pos - home.pos;
        mDoMtx_YrotS(*calc_mtx, ato.atan2sX_Z() + 0x2000);
        mae.x = 0.0f;
        mae.y = 0.0f;
        mae.z = ato.abs() - 2500.0f;
        MtxPosition(&mae, &ato);
        ato += home.pos;
        ato.y = -1600.0f;

        fopAcM_createDisappear(this, &ato, 49, 0, 0xff);

        dComIfGp_getVibration().StartShock(7, 0x4f, cXyz(0.0f, 1.0f, 0.0f));
        mSound.startCreatureSound(Z2SE_EN_DS_END_COL_LAST, 0, -1);
        daPy_getPlayerActorClass()->changeDemoMode(4, 1, 0, 0);
        mModeTimer = 80;
        mMode++;
        // fallthrough
    case 17:
        if (cLib_calcTimer(&mModeTimer) != 0) {
            break;
        }

        mDead = true;
        shape_angle.x = 0;
        shape_angle.z = 0;
        ato.set(0.0f, 1800.0f, -900.0f);
        daPy_getPlayerActorClass()->changeDemoMode(2, 0, 0, 0);
        daPy_getPlayerActorClass()->setPlayerPosAndAngle(&ato, 0x8000, 0);

        mae.set(0.0f, 1800.0f, -1140.0f);
        ato = mae - home.pos;
        ato.y = 0.0f;
        mDoMtx_YrotS(*calc_mtx, (s16)ato.atan2sX_Z());
        mae.x = 0.0f;
        mae.y = -500.0f;
        mae.z = l_HIO.mP2MoveAxis - 600.0f;
        MtxPosition(&mae, &current.pos);
        current.pos += home.pos;

        shape_angle.y = 0x4000;
        mCameraCenter = current.pos;
        mCameraCenter.y = mEd2CenterDt[0].y;
        ato = mEd22EyeDt[0];
        mCameraEye = ato;
        mCameraFovy = 68.0f;
        current.angle.x = -0x4000;

        mae.set(0.0f, 1800.0f, -1140.0f);
        ato = mae - home.pos;
        ato.y = 0.0f;
        mDoMtx_YrotS(*calc_mtx, (s16)ato.atan2sX_Z());
        mae.x = 0.0f;
        mae.y = 0.0f;
        mae.z = l_HIO.mP2MoveAxis - 1600.0f;
        MtxPosition(&mae, &field_0x718);
        field_0x718 += home.pos;

        ato = field_0x718 - current.pos;
        current.angle.y = ato.atan2sX_Z();
        mModeTimer = 80;
        mMode = 20;
        // fallthrough
    case 20:
        ato.set(0.0f, 1800.0f, -1140.0f);
        daPy_getPlayerActorClass()->changeDemoPos0(&ato);
        if (cLib_calcTimer(&mModeTimer) != 0) {
            break;
        }

        Z2GetAudioMgr()->seStart(Z2SE_OBJ_DS_END_ZAN_SWD, &current.pos, 0, 0, 1.0f, 1.0f, -1.0f,
                                 -1.0f, 0);
        mMode++;
        // fallthrough
    case 21:
        shape_angle.z += 0x2000;
        cLib_addCalcAngleS2(&current.angle.x, 0x4000, 100, 200);

        mDoMtx_YrotS(*calc_mtx, current.angle.y);
        mDoMtx_XrotM(*calc_mtx, current.angle.x);
        mae.x = 0.0f;
        mae.y = 0.0f;
        mae.z = 30.0f;
        MtxPosition(&mae, &ato);
        speed.y = ato.y;
        if (speed.y < 0.0f) {
            speed.y *= 4.0f;
        }

        if (abs(current.angle.x) > 0x2000) {
            cLib_addCalc2(&speedF, 9.0f, 0.1f, 0.3f);
        }

        if (speed.y < 0.0f && current.pos.y <= 1955.0f) {
            mMode++;
        }
        break;
    case 22:
        if ((s16)shape_angle.z != 0) {
            shape_angle.z += 0x2000;
        }

        cLib_addCalc2(&speedF, 9.0f, 0.1f, 0.3f);
        if (current.pos.y > 1850.0f) {
            break;
        }

        speedF = 0.0f;
        speed.y = 0.0f;
        gravity = 0.0f;
        current.pos.y = 1850.0f;
        ato = current.pos;
        ato.x += 85.0f;
        ato.y = 1800.0f;

        dComIfGp_getVibration().StartShock(2, 0x4f, cXyz(0.0f, 1.0f, 0.0f));
        for (int i = 0; i < 2; i++) {
            dComIfGp_particle_set(eff_Demo_sasi_id[i], &ato, &shape_angle, NULL);
        }

        mModeTimer = 30;
        mMode++;
        // fallthrough
    case 23:
        if (cLib_calcTimer(&mModeTimer) == 0) {
            mpSwordBrkAnm->init(mpSwordMorf->getModel()->getModelData(),
                                static_cast<J3DAnmTevRegKey*>(dComIfG_getObjectRes("B_DS", 80)), 1,
                                1, 1.0f, 0, -1);
            mColBlend = 0.0f;
            field_0x840 = 4;
            mMode++;
        }
        break;
    case 24:
        if (mpSwordMorf->isStop()) {
            ato = current.pos;
            ato.x += 85.0f;
            ato.y = 1800.0f;
            dComIfGp_particle_set(0x8c20, &ato, &shape_angle, NULL);

            fopAcM_createDisappear(this, &ato, 4, 1, 0xff);

            mNoDrawSword = true;
            mModeTimer = 80;
            mMode++;
        }
        break;
    case 25:
        if (cLib_calcTimer(&mModeTimer) != 0) {
            if (mModeTimer == 70) {
                item_scale.set(1.0f, 1.0f, 1.0f);
                ato = current.pos;
                ato.x += 85.0f;
                ato.y = 1800.0f;
                fopAcM_createItemForBoss(&ato, 0x22, fopAcM_GetRoomNo(this), &shape_angle,
                                         &item_scale, 0.0f, 0.0f, -1);

                daPy_getPlayerActorClass()->changeDemoMode(30, 0, 0, 0);
            }

            cLib_addCalcPos(&mCameraCenter, mEd2CenterDt[1], 0.3f, 50.0f, 10.0f);
            cLib_addCalcPos(&mCameraEye, mEd22EyeDt[1], 0.3f, 50.0f, 10.0f);
            break;
        }

        daPy_getPlayerActorClass()->changeDemoMode(1, 0, 0, 0);
        mModeTimer = 80;
        mMode++;
        // fallthrough
    case 26:
        if (cLib_calcTimer(&mModeTimer) != 0) {
            break;
        }

        fopAcM_onSwitch(this, 0x70);
        mModeTimer = 140;
        mMode++;
        // fallthrough
    case 27:
        mCameraCenter.set(-280.0f, 1950.0f, -4150.0f);
        mCameraEye.set(-340.0f, 2075.0f, -4505.0f);
        mCameraFovy = 56.0f;

        if (cLib_calcTimer(&mModeTimer) != 0) {
            break;
        }

        fopAcM_onSwitch(this, 0x7b);
        mModeTimer = 140;
        mMode++;
        // fallthrough
    case 28:
        mCameraCenter.set(410.0f, 1890.0f, -3835.0f);
        mCameraEye.set(535.0f, 1725.0f, -3515.0f);
        mCameraFovy = 59.0f;

        if (cLib_calcTimer(&mModeTimer) == 0) {
            camera->mCamera.Start();
            camera->mCamera.SetTrimSize(0);
            dComIfGp_event_reset();
            dComIfGs_onStageBossEnemy(0x13);
            /* dSv_event_flag_c::F_0265 - Arbiter's Grounds - Arbiter's Grounds clear */
            dComIfGs_onEventBit(0x2010);
            fopAcM_delete(this);
        }
    }

    if (mMode >= 20 && mMode <= 23) {
        ato = current.pos;
        if (mEd2CenterDt[0].y < current.pos.y) {
            cLib_addCalc2(&mCameraCenter.y, current.pos.y, 0.8f, 100.0f);
        }

        ato.x += 70.0f;
        cLib_addCalc2(&mCameraCenter.x, ato.x, 0.8f, 100.0f);
        cLib_addCalc2(&mCameraCenter.z, ato.z, 0.8f, 100.0f);
        eyePos = current.pos;
    }

    if (mMode >= 5 && mMode <= 15) {
        cLib_addCalcPos(&mCameraCenter, current.pos, 0.8f, 100.0f, 40.0f);
    }

    if (mMode >= 5 && mMode <= 16) {
        mSmokeSet();
    }

    if (mMode >= 25 && mMode != 100) {
        cLib_addCalc(&mColBlend, 1.0f, 0.01f, 0.01f, 0.001f);
        dKy_custom_colset(2, 4, mColBlend);
    }

    if (mMode != 0) {
        camera->mCamera.Set(mCameraCenter, mCameraEye, mCameraFovy, 0);
    }
}

void daB_DS_c::executeBullet() {
    static u16 effId[4] = {0x89AC, 0x89AD, 0x89AE, 0x89AF};
    cXyz sp38;
    cXyz sp44;

    switch (mMode) {
    case 0:
        if (arg0 == TYPE_BULLET_A) {
            speedF = -100.0f;
            mBulletRadius = 100.0f;
            mModeTimer = breathTimerBase;
            mMode = 2;
            break;
        } else {
            mBulletDistance = 500.0f;
            mBulletRadius = 80.0f;
            mModeTimer = 100;
            mMode++;
        }
        // fallthrough
    case 1:
        if (!daPy_getPlayerActorClass()->checkSpinnerPathMove() ||
            (s16)(current.angle.y - home.angle.y) > 0x2000)
        {
            speedF = 100.0f;
            mModeTimer = 100;
            mMode = 2;
        } else {
            fopAc_ac_c* parent_actor;
            if (fopAcM_SearchByID(parentActorID, &parent_actor) && parent_actor != NULL) {
                daB_DS_c* parent = static_cast<daB_DS_c*>(parent_actor);
                sp38 = parent->field_0x6d0 - parent->mMouthPos;
                current.angle.x = sp38.atan2sY_XZ();
                current.angle.y = sp38.atan2sX_Z();
                mDoMtx_YrotS(*calc_mtx, current.angle.y);
                mDoMtx_XrotM(*calc_mtx, current.angle.x);
                sp38.x = 0.0f;
                sp38.y = 0.0f;
                sp38.z = mBulletDistance;
                MtxPosition(&sp38, &sp44);
                current.pos = sp44 + parent->mMouthPos;
                mBulletDistance += 100.0f;
            }

            mBulletRadius += 10.0f;
            if (mBulletRadius > 240.0f) {
                mBulletRadius = 240.0f;
            }
        }
        break;

    case 2:
        mDoMtx_YrotS(*calc_mtx, current.angle.y);

        if (arg0 == TYPE_BULLET_A) {
            mDoMtx_XrotM(*calc_mtx, -current.angle.x);
        } else {
            mDoMtx_XrotM(*calc_mtx, current.angle.x);
        }

        sp38.x = 0.0f;
        sp38.y = 0.0f;
        sp38.z = speedF;
        MtxPosition(&sp38, &speed);

        if (cLib_calcTimer(&mModeTimer) == 0) {
            fopAcM_delete(this);
        }
        break;
    }

    if (mAcch.ChkWallHit() || mAcch.ChkGroundHit()) {
        fopAcM_delete(this);
        return;
    }

    mBreathAtSph.SetC(current.pos);
    mBreathAtSph.SetR(mBulletRadius);
    dComIfG_Ccsp()->Set(&mBreathAtSph);

    if (arg0 == TYPE_BULLET_B || arg0 == TYPE_BULLET_C) {
        cXyz scale(l_HIO.mP2ModelSize, l_HIO.mP2ModelSize, l_HIO.mP2ModelSize);
        field_0x724 = current.pos - old.pos;
        VECScale(&field_0x724, &field_0x724, 0.8f);

        Z2GetAudioMgr()->seStartLevel(Z2SE_OBJ_DS_H_FIRE, &current.pos, 0, 0, 1.0f, 1.0f, -1.0f,
                                      -1.0f, 0);

        for (int i = 0; i < 4; i++) {
            mBulletParticleKey[i] = dComIfGp_particle_set(mBulletParticleKey[i], effId[i],
                                                          &current.pos, &current.angle, &scale);
            JPABaseEmitter* emitter = dComIfGp_particle_getEmitter(mBulletParticleKey[i]);
            if (emitter != NULL) {
                emitter->setGlobalTranslation(JGeometry::TVec3<f32>(current.pos));
                emitter->setParticleCallBackPtr(dPa_control_c::getParticleTracePCB());
                emitter->setUserWork((uintptr_t)&field_0x724);
            }
        }
    }
}

void daB_DS_c::mBulletAction() {
    executeBullet();
    fopAcM_posMove(this, mCcStts.GetCCMoveP());

    if (arg0 == TYPE_BULLET_B || arg0 == TYPE_BULLET_C) {
        mAcchCir.SetWall(0.0f, 100.0f);
        mAcch.CrrPos(dComIfG_Bgsp());
    }
}

void daB_DS_c::mSoundSE_Set() {
    static JAISoundID mWaitDt[3] = {
        Z2SE_EN_DS_V_WAIT_A,
        Z2SE_EN_DS_V_WAIT_B,
        Z2SE_EN_DS_V_WAIT_C,
    };

    switch (mAnmID) {
    case ANM_WAIT01_A:
    case ANM_WAIT01_B:
    case ANM_WAIT01_C:
        if (mpMorf->checkFrame(29.0f)) {
            mSoundPos = current.pos;
            mSound.startCreatureVoice(mWaitDt[mAnmID - ANM_WAIT01_A], -1);
        }
        break;
    case ANM_NODAMAGE_01_A:
    case ANM_NODAMAGE_01_B:
    case ANM_NODAMAGE_01_C:
        if (mpMorf->checkFrame(25.0f)) {
            mSoundPos = current.pos;
            mSound.startCreatureVoice(Z2SE_EN_DS_V_NODMG, -1);
        }
        if (mpMorf->checkFrame(39.0f)) {
            mSoundPos = current.pos;
            mSound.startCreatureSound(Z2SE_EN_DS_HAND_1, 0, -1);
        }
        break;
    case ANM_NODAMAGE_02_A:
    case ANM_NODAMAGE_02_B:
    case ANM_NODAMAGE_02_C:
        if (mpMorf->checkFrame(28.0f)) {
            mSoundPos = current.pos;
            mSound.startCreatureVoice(Z2SE_EN_DS_V_NODMG, -1);
        }
        if (mpMorf->checkFrame(51.0f)) {
            mSoundPos = current.pos;
            mSound.startCreatureSound(Z2SE_EN_DS_HAND_2, 0, -1);
        }
        break;
    case ANM_NODAMAGE_03_A:
    case ANM_NODAMAGE_03_B:
    case ANM_NODAMAGE_03_C:
        if (mpMorf->checkFrame(42.0f)) {
            mSoundPos = current.pos;
            mSound.startCreatureVoice(Z2SE_EN_DS_V_NODMG, -1);
        }
        break;
    case ANM_ATTACK_L_A:
    case ANM_ATTACK_L_B:
    case ANM_ATTACK_L_C:
    case ANM_ATTACK_R_A:
    case ANM_ATTACK_R_B:
    case ANM_ATTACK_R_C:
        if (mpMorf->checkFrame(32.0f)) {
            mSoundPos = current.pos;
            mSound.startCreatureVoice(Z2SE_EN_DS_V_ATK_1, -1);
        }

        if (mpMorf->checkFrame(97.0f)) {
            mSoundPos = current.pos;
            mSound.startCreatureVoice(Z2SE_EN_DS_V_ATK_2, -1);
        }

        if (mpMorf->checkFrame(105.0f)) {
            mSoundPos = current.pos;
            mSound.startCreatureSound(Z2SE_EN_DS_ATK, 0, -1);
        }
        break;
    case ANM_ATTACK02_A_A:
    case ANM_ATTACK02_A_B:
    case ANM_ATTACK02_A_C:
        if (mpMorf->checkFrame(83.0f)) {
            mSoundPos = current.pos;
            mSound.startCreatureVoice(Z2SE_EN_DS_V_ATK_FIRE, -1);
        }
        break;
    case ANM_CIRCLE_L_A:
    case ANM_CIRCLE_L_B:
    case ANM_CIRCLE_L_C:
    case ANM_CIRCLE_R_A:
    case ANM_CIRCLE_R_B:
    case ANM_CIRCLE_R_C:
        if (mpMorf->checkFrame(32.0f)) {
            mSoundPos = current.pos;
            mSound.startCreatureSound(Z2SE_EN_DS_HAND_1, 0, -1);
        }

        if (mpMorf->checkFrame(54.0f)) {
            mSoundPos = current.pos;
            mSound.startCreatureSound(Z2SE_EN_DS_HAND_2, 0, -1);
        }

        if (mpMorf->checkFrame(92.0f)) {
            mSoundPos = current.pos;
            mSound.startCreatureSound(Z2SE_EN_DS_HAND_1, 0, -1);
        }

        if (mpMorf->checkFrame(114.0f)) {
            mSoundPos = current.pos;
            mSound.startCreatureSound(Z2SE_EN_DS_HAND_2, 0, -1);
        }
        break;
    case ANM_ATTACK01_A:
        if (mpMorf->checkFrame(272.0f)) {
            mSoundPos = current.pos;
            mSound.startCreatureSound(Z2SE_EN_DS_HAND_1, 0, -1);
        }

        if (mpMorf->checkFrame(314.0f)) {
            mSoundPos = current.pos;
            mSound.startCreatureSound(Z2SE_EN_DS_HAND_2, 0, -1);
        }
        // fallthrough
    case ANM_ATTACK01_B:
    case ANM_ATTACK01_C:
        if (mpMorf->checkFrame(10.0f)) {
            mSoundPos = current.pos;
            mSound.startCreatureVoice(Z2SE_EN_DS_V_RTT, -1);
        }
        if (mpMorf->checkFrame(184.0f)) {
            mSoundPos = current.pos;
            mSound.startCreatureSound(Z2SE_EN_DS_HANDS, 0, -1);
        }
        break;
    case ANM_DAMAGE_A_WAIT:
    case ANM_DAMAGE_B_WAIT:
    case ANM_LASTDAMAGE_WAIT:
        mSoundPos = current.pos;
        mSound.startCreatureSound(Z2SE_EN_DS_DMG_WAIT, 0, -1);
        break;
    case ANM_DAMAGE_A:
        if (mpMorf->checkFrame(0.0f)) {
            mSoundPos = current.pos;
            mSound.startCreatureVoice(Z2SE_EN_DS_V_DMG_A, -1);
        }

        if (mpMorf->checkFrame(20.0f)) {
            mSoundPos = current.pos;
            mSound.startCreatureSound(Z2SE_EN_DS_DMG_A_FALL, 0, -1);
        }

        if (mpMorf->checkFrame(28.0f)) {
            mSoundPos = current.pos;
            mSound.startCreatureSound(Z2SE_EN_DS_DMG_HAND, 0, -1);
        }

        if (mpMorf->checkFrame(54.0f)) {
            mSoundPos = current.pos;
            mSound.startCreatureSound(Z2SE_EN_DS_DMG_HAND, 0, -1);
        }
        break;

    case ANM_DAMAGE_B:
        if (mpMorf->checkFrame(0.0f)) {
            mSoundPos = current.pos;
            mSound.startCreatureVoice(Z2SE_EN_DS_V_DMG_B, -1);
        }

        if (mpMorf->checkFrame(6.0f)) {
            mSoundPos = current.pos;
            mSound.startCreatureSound(Z2SE_EN_DS_DMG_B_FALL, 0, -1);
        }

        if (mpMorf->checkFrame(33.0f)) {
            mSoundPos = current.pos;
            mSound.startCreatureSound(Z2SE_EN_DS_DMG_HAND, 0, -1);
        }

        if (mpMorf->checkFrame(45.0f)) {
            mSoundPos = current.pos;
            mSound.startCreatureSound(Z2SE_EN_DS_DMG_HAND, 0, -1);
        }
        break;
    case ANM_LASTDAMAGE:
        if (mpMorf->checkFrame(0.0f)) {
            mSoundPos = current.pos;
            mSound.startCreatureVoice(Z2SE_EN_DS_V_DMG_L, -1);
        }

        if (mpMorf->checkFrame(124.0f)) {
            mSoundPos = current.pos;
            mSound.startCreatureSound(Z2SE_EN_DS_DMG_L_FALL, 0, -1);
        }

        if (mpMorf->checkFrame(119.0f)) {
            mSoundPos = current.pos;
            mSound.startCreatureSound(Z2SE_EN_DS_DMG_HAND, 0, -1);
        }

        if (mpMorf->checkFrame(236.0f)) {
            mSoundPos = current.pos;
            mSound.startCreatureSound(Z2SE_EN_DS_DMG_BODY, 0, -1);
        }

        if (mpMorf->checkFrame(240.0f)) {
            mSoundPos = current.pos;
            mSound.startCreatureSound(Z2SE_EN_DS_DMG_HAND, 0, -1);
        }
        break;
    }
}

void daB_DS_c::action() {
    static const struct {
        u16 particle;
        int joint;
    } SEBONE_EFF_DT[6] = {
        {0x8BEF, 0}, {0x8BF0, 0}, {0x8BF1, 1}, {0x8BF2, 1}, {0x8BF3, 2}, {0x8BF4, 2},
    };

    int joint = mBackboneLevel;
    if (mAction != ACT_OPENING_DEMO && joint < 3 && mBossPhase == 0) {
        cXyz pos;

        joint *= 2;
        for (int i = 0; i < 2; i++) {
            mDoMtx_stack_c::copy(model->getAnmMtx(SEBONE_EFF_DT[joint + i].joint));

            mDoMtx_stack_c::multVecZero(&pos);
            mBackboneParticleKey[i] =
                dComIfGp_particle_set(mBackboneParticleKey[i], SEBONE_EFF_DT[joint + i].particle,
                                      &pos, &shape_angle, NULL);
        }

        Z2GetAudioMgr()->seStartLevel(Z2SE_EN_DS_SMOKE_1, &current.pos, 0, 0, 1.0f, 1.0f, -1.0f,
                                      -1.0f, 0);
    }

    if (cLib_calcTimer(&mHintTimer1) != 0 && mHintTimer1 == 1) {
        dComIfGs_onOneZoneSwitch(5, fopAcM_GetRoomNo(this));
    }

    if (mBackboneLevel != 0) {
        mZsMoveChk();
        mZsMoveChk_Guard();
    }

    damage_check();
    mCreateTrap(false);

    if (mAction != ACT_OPENING_DEMO && !dComIfGs_isZoneSwitch(8, fopAcM_GetRoomNo(this)) &&
        cLib_calcTimer(&mPedestalFallTimer) == 0)
    {
        dComIfGs_onZoneSwitch(8, fopAcM_GetRoomNo(this));
        mSandFallTimer = l_HIO.mSandFallWaitTime;
    }

    if (arg0 == TYPE_BATTLE_1 && dComIfGs_isZoneSwitch(8, fopAcM_GetRoomNo(this))) {
        cLib_calcTimer(&mSandFallTimer);
        if (mSandFallTimer >= 1 && mSandFallTimer <= 3 && bitSw != 0xff) {
            fopAcM_onSwitch(this, bitSw);
            mSandFallTimer = 0;
        }
    }

    switch (mAction) {
    case ACT_OPENING_DEMO:
        executeOpeningDemo();
        break;
    case ACT_WAIT:
        executeWait();
        break;
    case ACT_CIRCLE:
        executeCircle();
        break;
    case ACT_DAMAGE:
        executeDamage();
        break;
    case ACT_ETC_DAMAGE:
        executeEtcDamage();
        break;
    case ACT_BREATH_ATTACK:
        executeBreathAttack();
        break;
    case ACT_HAND_ATTACK:
        executeHandAttack();
        break;
    case ACT_BREATH_SEARCH:
        executeBreathSearch();
        break;
    }

    if ((mAction != ACT_BREATH_ATTACK || mMode >= 3) && field_0x840 == 5) {
        cLib_addCalc(&mColBlend, 1.0f, 0.02f, 0.02f, 0.04f);
        if (mColBlend > 0.998f) {
            field_0x840 = 1;
            mColBlend = 1.0f;
        }
        dKy_custom_colset(5, 1, mColBlend);
    }

    mSoundSE_Set();
    fopAcM_posMoveF(this, mCcStts.GetCCMoveP());
    mAcchCir.SetWall(300.0f, 600.0f);

    f32 ground_up_y = -60.0f;
    if (mAction == ACT_OPENING_DEMO) {
        ground_up_y = 0.0f;
    }

    if (mAction != ACT_DAMAGE || mMode < 100) {
        mAcch.SetGroundUpY(ground_up_y);
        mAcch.CrrPos(dComIfG_Bgsp());
    }

    if (mBossPhase != 100) {
        if (daPy_getPlayerActorClass()->checkWolfLock(this)) {
            daPy_getPlayerActorClass()->cancelWolfLock(this);
        }

        mpMorf->play(0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
        mpSwordBrkAnm->play();
        mpSwordMorf->play(0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));

        if (mDrawZant) {
            mpZantMorf->play(0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
        }

        if (mPlayPatternAnm) {
            mpOpPatternBrkAnm->play();
            mpOpPatternBtkAnm->play();
        }
    }
}

void daB_DS_c::mtx_set() {
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::ZXYrotM(shape_angle);
    mDoMtx_stack_c::scaleM(l_HIO.mModelSize, l_HIO.mModelSize, l_HIO.mModelSize);

    if (mPlayPatternAnm) {
        mpOpPatternModel->setBaseTRMtx(mDoMtx_stack_c::get());
    }

    J3DModel* model = mpMorf->getModel();
    model->setBaseTRMtx(mDoMtx_stack_c::get());
    mpMorf->modelCalc();

    mDoMtx_stack_c::copy(model->getAnmMtx(DS_JNT_HEAD));
    mDoMtx_stack_c::multVecZero(&mHeadPos);

    mDoMtx_stack_c::copy(model->getAnmMtx(DS_JNT_JAW));
    cXyz vec(0.0f, 100.0f, 0.0f);
    mDoMtx_stack_c::multVec(&vec, &mMouthPos);

    mDoMtx_stack_c::copy(model->getAnmMtx(DS_JNT_JAW));
    vec.set(100.0f, 20.0f, 0.0f);
    mDoMtx_stack_c::multVec(&vec, &mBulletPos);

    mDoMtx_stack_c::copy(model->getAnmMtx(DS_JNT_HEAD));
    if (mAction == ACT_OPENING_DEMO || cLib_calcTimer(&mSwordTimer) != 0) {
        vec.set(594.0f, -196.0f, 0.0f);
    } else {
        vec.set(584.0f, -136.0f, 0.0f);
    }

    mDoMtx_stack_c::transM(vec.x, vec.y, vec.z);

    if (mAction == ACT_OPENING_DEMO || mSwordTimer != 0) {
        mDoMtx_stack_c::YrotM(-0x4000);
        mDoMtx_stack_c::XrotM(-0x7770);
    } else {
        mDoMtx_stack_c::YrotM(-0x3d44);
        mDoMtx_stack_c::XrotM(-0x7c4c);
    }

    mDoMtx_stack_c::scaleM(mSwordScale.x, mSwordScale.y, mSwordScale.z);
    mDoMtx_stack_c::multVecZero(&mSwordPos);
    mpSwordMorf->getModel()->setBaseTRMtx(mDoMtx_stack_c::get());
    mpSwordMorf->modelCalc();

    vec.set(0.0f, -20.0f, 40.0f);
    mDoMtx_stack_c::multVec(&vec, &eyePos);
    if (mDrawZant == true) {
        eyePos = mZantEyePos;
    }

    attention_info.position = eyePos;
    attention_info.position.y += 100.0f;

    mDoMtx_stack_c::transS(mSwordPos);
    mDoMtx_stack_c::ZXYrotM(shape_angle);
    vec.set(5.0f, 0.0f, 10.0f);
    mDoMtx_stack_c::transM(vec.x, vec.y, vec.z);
    mDoMtx_stack_c::YrotM(field_0x7ca + 5000);
    mDoMtx_stack_c::scaleM(mZantScale.x, mZantScale.y, mZantScale.z);
    mDoMtx_stack_c::multVecZero(&mZantPos);
    mpZantMorf->getModel()->setBaseTRMtx(mDoMtx_stack_c::get());
    mpZantMorf->modelCalc();
}

void daB_DS_c::cc_set() {
    static cXyz head_cc_dt[5] = {
        cXyz(380.0f, 0.0f, 0.0f),  cXyz(50.0f, -50.0f, 0.0f),   cXyz(-300.0f, -70.0f, 0.0f),
        cXyz(640.0f, 80.0f, 0.0f), cXyz(670.0f, -100.0f, 0.0f),
    };

    static f32 head_setRdt[5] = {
        300.0f, 350.0f, 270.0f, 190.0f, 190.0f,
    };

    J3DModel* model = mpMorf->getModel();
    cXyz center_base, center;

    mDoMtx_stack_c::copy(model->getAnmMtx(DS_JNT_HEAD));
    center_base.set(660.0f, -230.0f, 0.0f);
    mDoMtx_stack_c::multVec(&center_base, &center);

    mWeakSph.SetC(center);
    mWeakSph.SetR(70.0f);
    dComIfG_Ccsp()->Set(&mWeakSph);

    for (int i = 0; i < 5; i++) {
        center_base = head_cc_dt[i];
        mDoMtx_stack_c::multVec(&center_base, &center);
        f32 radius = head_setRdt[i];
        mHeadSph[i].SetC(center);
        mHeadSph[i].SetR(radius);
        dComIfG_Ccsp()->Set(&mHeadSph[i]);
    }

    if (mBackboneLevel < 3) {
        mDoMtx_stack_c::copy(model->getAnmMtx(mBackboneLevel));
        center_base.set(-20.0f, 0.0f, 0.0f);
        mDoMtx_stack_c::multVec(&center_base, &center);
        mBackboneCyl.SetC(center);
        mBackboneCyl.SetH(400.0f);
        mBackboneCyl.SetR(120.0f);
        dComIfG_Ccsp()->Set(&mBackboneCyl);
    }

    mDoMtx_stack_c::copy(model->getAnmMtx(DS_JNT_BACKBONE4));
    mDoMtx_stack_c::multVecZero(&mBackbonePos);

    mDoMtx_stack_c::copy(model->getAnmMtx(DS_JNT_HAND_L));
    mDoMtx_stack_c::multVecZero(&mHandPos[0]);

    if (mAction == ACT_OPENING_DEMO) {
        center_base.set(500.0f, -100.0f, 180.0f);
        mDoMtx_stack_c::multVec(&center_base, &center);
        mHandAtLCyl.SetC(center);
        mHandAtLCyl.SetH(300.0f);
        mHandAtLCyl.SetR(440.0f);
    } else {
        center_base.set(400.0f, -100.0f, 100.0f);
        mDoMtx_stack_c::multVec(&center_base, &center);
        mHandAtLCyl.SetC(center);
        mHandAtLCyl.SetH(300.0f);
        mHandAtLCyl.SetR(500.0f);
    }

    dComIfG_Ccsp()->Set(&mHandAtLCyl);

    mDoMtx_stack_c::copy(model->getAnmMtx(DS_JNT_LYUBI_C3));
    mDoMtx_stack_c::multVecZero(&mFingerPos[0]);

    mDoMtx_stack_c::copy(model->getAnmMtx(DS_JNT_HAND_R));
    mDoMtx_stack_c::multVecZero(&mHandPos[1]);

    center_base.set(400.0f, -100.0f, -100.0f);
    mDoMtx_stack_c::multVec(&center_base, &center);
    mHandAtRCyl.SetC(center);
    mHandAtRCyl.SetH(300.0f);
    mHandAtRCyl.SetR(500.0f);
    dComIfG_Ccsp()->Set(&mHandAtRCyl);

    mDoMtx_stack_c::copy(model->getAnmMtx(DS_JNT_RYUBI_C3));
    mDoMtx_stack_c::multVecZero(&mFingerPos[1]);
}

void daB_DS_c::cc_etc_set() {
    static const struct {
        int joint_no;
        Vec center;
        f32 radius;
    } ETC_CC_DT[18] = {
        {0, {2000.0f, 800.0f, 150.0f}, 200.0f},   {0, {1500.0f, 500.0f, 250.0f}, 550.0f},
        {0, {1000.0f, 200.0f, 150.0f}, 440.0f},   {4, {-220.0f, 0.0f, -250.0f}, 150.0f},
        {4, {0.0f, 0.0f, 750.0f}, 150.0f},        {29, {0.0f, -50.0f, 150.0f}, 150.0f},
        {11, {70.0f, 0.0f, 0.0f}, 170.0f},        {11, {-140.0f, -50.0f, -100.0f}, 120.0f},
        {11, {-310.0f, -70.0f, -180.0f}, 100.0f}, {10, {240.0f, 50.0f, 0.0f}, 80.0f},
        {10, {-100.0f, 100.0f, 250.0f}, 120.0f},  {9, {180.0f, 50.0f, -50.0f}, 110.0f},
        {11, {-100.0f, 200.0f, -100.0f}, 200.0f}, {10, {150.0f, 0.0f, 0.0f}, 200.0f},
        {10, {-150.0f, 0.0f, 0.0f}, 200.0f},      {30, {-100.0f, 200.0f, -100.0f}, 200.0f},
        {29, {150.0f, 0.0f, 0.0f}, 200.0f},       {29, {-150.0f, 0.0f, 0.0f}, 200.0f},
    };

    static f32 etc_setRdt[18] = {
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    };

    if (health <= 1 || mBossPhase == 100) {
        return;
    }

    int last = 18;
    int i = 0;
    if (mAction == ACT_OPENING_DEMO) {
        if (mMode == 3) {
            last = 12;
        } else {
            return;
        }
    } else {
        i = 12;
    }

    J3DModel* model = mpMorf->getModel();
    for (; i < last; i++) {
        mDoMtx_stack_c::copy(model->getAnmMtx(ETC_CC_DT[i].joint_no));
        cXyz center_base = ETC_CC_DT[i].center;
        cXyz center;
        mDoMtx_stack_c::multVec(&center_base, &center);
        mEtcSph[i].SetC(center);
        mEtcSph[i].SetR(ETC_CC_DT[i].radius);
        dComIfG_Ccsp()->Set(&mEtcSph[i]);
    }
}

void daB_DS_c::mBattle2Action() {
    mBattle2_spinner_damage_check();

    switch (mAction) {
    case ACT_B2_OPENING_DEMO:
        executeBattle2OpeningDemo();
        break;
    case ACT_B2_WAIT:
        executeBattle2Wait();
        break;
    case ACT_B2_F_MOVE:
        executeBattle2FMove();
        break;
    case ACT_B2_TIRED:
        executeBattle2Tired();
        break;
    case ACT_B2_DAMAGE:
        executeBattle2Damage();
        break;
    case ACT_B2_DEAD:
        executeBattle2Dead();
        break;
    }

    if (cLib_calcTimer(&mHintTimer2) != 0 && mHintTimer2 == 1) {
        dComIfGs_onOneZoneSwitch(7, fopAcM_GetRoomNo(this));
    }

    mCreateTrap(false);

    if (BREG_S(7)) {
        // "Where is process now? %d\n"
        OS_REPORT("今ドコの処理？ %d\n", mAction);

        // "Where is process now? %d\n\n"
        OS_REPORT("今ドコの処理？ %d\n\n", mMode);
    }

    fopAcM_posMoveF(this, mCcStts.GetCCMoveP());

    if (!mDead) {
        mAcchCir.SetWall(200.0f, mWallR);
        mAcch.SetGroundUpY(mGroundUpY);
        mAcch.CrrPos(dComIfG_Bgsp());
        mpMorf->play(0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
    }

    mpSwordBrkAnm->play();
    mpSwordMorf->play(0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));

    if (mPlayPatternAnm) {
        mpPatternBrkAnm->play();
        mpPatternBtkAnm->play();
    }
}

void daB_DS_c::mBattle2_spinner_damage_check() {
    if (cLib_calcTimer(&mDamageTimer) != 0 || mAnmID == ANM_HEAD_RETURN ||
        mAction == ACT_B2_DAMAGE || mAction == ACT_B2_DEAD || health < 1)
    {
        return;
    }

    for (int i = 0; i < 18; i++) {
        if (mEtcSph[i].ChkTgHit()) {
            mDamageTimer = 8;
            mAtInfo.mpCollider = mEtcSph[i].GetTgHitObj();
            mSoundPos = *mEtcSph[i].GetTgHitPosP();
            def_se_set(&mSound, mAtInfo.mpCollider, 2, NULL);
            mEtcSph[i].ClrTgHit();
            return;
        }
    }

    if (mAction == ACT_B2_OPENING_DEMO || mAction == ACT_B2_DAMAGE || mAction == ACT_B2_DEAD) {
        return;
    }

    daPy_getPlayerActorClass()->onBossRoomWait();

    for (int i = 0; i < 5; i++) {
        if (mHeadSph[i].ChkTgHit()) {
            mAtInfo.mpCollider = mHeadSph[i].GetTgHitObj();
            cXyz vec1 = mSwordPos - *mHeadSph[i].GetTgHitPosP();
            cXyz hit_pos = *mHeadSph[i].GetTgHitPosP();

            mDamageTimer = 8;

            csXyz hit_angle;
            hit_angle.x = 0;
            hit_angle.y = vec1.atan2sX_Z();
            hit_angle.z = 0;

            if (mHeadSph[i].GetTgHitObj()->ChkAtType(AT_TYPE_SPINNER)) {
                mSound.startCreatureVoice(Z2SE_EN_DS_H_V_DMG_SPNR, -1);
                mCreateFireBreath = false;

                def_se_set(&mSound, mAtInfo.mpCollider, 0x1f, NULL);
                dComIfGp_setHitMark(3, this, &hit_pos, &hit_angle, NULL, 0);
                Z2GetAudioMgr()->changeBgmStatus(2);

                field_0x790.set(0.0f, 0.0f, 0.0f);
                setBck(ANM_HEAD_FATTACK_A, 0, 3.0f, 1.0f);
                setActionMode(ACT_B2_DAMAGE, 0);
            } else {
                def_se_set(&mSound, mAtInfo.mpCollider, 2, NULL);
                dComIfGp_setHitMark(2, this, &hit_pos, &hit_angle, NULL, 0);
            }

            mHeadSph[i].ClrTgHit();
        }
    }

    mCcStts.Move();
}

bool daB_DS_c::mBattle2_damage_check() {
    if (health < 1) {
        return false;
    }

    if (cLib_calcTimer(&mDamageTimer) != 0) {
        return false;
    }

    if (mWeakSph.ChkTgHit()) {
        mAtInfo.mpCollider = mWeakSph.GetTgHitObj();
        cXyz vec1 = mSwordPos - *mWeakSph.GetTgHitPosP();
        cXyz hit_pos = *mWeakSph.GetTgHitPosP();
        mDamageTimer = 8;

        cc_at_check(this, &mAtInfo);

        csXyz hit_angle;
        hit_angle.x = 0;
        hit_angle.y = vec1.atan2sX_Z();
        hit_angle.z = 0;

        if (health <= 1) {
            dComIfGp_setHitMark(3, this, &eyePos, &hit_angle, NULL, 0);
            mPlayPatternAnm = false;
            mSoundPos = current.pos;
            mSound.startCollisionSE(Z2SE_HIT_SWORD, 0x20);

            JPABaseEmitter* emitter = dComIfGp_particle_getEmitter(mParticleKey1);
            if (emitter != NULL) {
                emitter->stopDrawParticle();
            }

            Z2GetAudioMgr()->bgmStop(0x1e, 0);
            Z2GetAudioMgr()->bgmStreamPrepare(0x2000048);
            Z2GetAudioMgr()->bgmStreamPlay();
            mSound.startCreatureVoice(Z2SE_EN_DS_END_V, -1);

            setBck(ANM_HEAD_DOWN_DAMAGE, 0, 3.0f, 1.0f);
            setActionMode(ACT_B2_DEAD, 0);
            return true;
        }

        if (mWeakSph.GetTgHitObj()->ChkAtType(AT_TYPE_NORMAL_SWORD) ||
            mWeakSph.GetTgHitObj()->ChkAtType(AT_TYPE_8000))
        {
            setBck(ANM_HEAD_DOWN_DAMAGE, 0, 3.0f, 1.0f);
            mSound.startCreatureVoice(Z2SE_EN_DS_H_V_DMG_SWD, -1);

            mpPatternBrkAnm->init(mpPatternModel->getModelData(),
                                  static_cast<J3DAnmTevRegKey*>(dComIfG_getObjectRes("B_DS", 78)),
                                  1, 0, 1.0f, 0, -1);

            mpPatternBtkAnm->init(
                mpPatternModel->getModelData(),
                static_cast<J3DAnmTextureSRTKey*>(dComIfG_getObjectRes("B_DS", 84)), 1, 0, 1.0f, 0,
                -1);

            mPlayPatternAnm = true;
            mSoundPos = current.pos;

            if (mAtInfo.mHitStatus == 0) {
                dComIfGp_setHitMark(1, this, &eyePos, &hit_angle, NULL, 0);
                field_0x7d0 = 0x200;
                mSound.startCollisionSE(Z2SE_HIT_SWORD, 0x1e);
            } else {
                dComIfGp_setHitMark(3, this, &eyePos, &hit_angle, NULL, 0);
                field_0x7d0 = 0x400;
                mSound.startCollisionSE(Z2SE_HIT_SWORD, 0x1f);
            }
        } else {
            dComIfGp_setHitMark(2, this, &eyePos, &hit_angle, NULL, 0);
            def_se_set(&mSound, mAtInfo.mpCollider, 2, NULL);
        }

        mpSwordBrkAnm->init(mpSwordMorf->getModel()->getModelData(),
                            static_cast<J3DAnmTevRegKey*>(dComIfG_getObjectRes("B_DS", 80)), 1, 1,
                            1.0f, 0, -1);

        if (mCutTypeCheck()) {
            mpPatternBrkAnm->init(mpPatternModel->getModelData(),
                                  static_cast<J3DAnmTevRegKey*>(dComIfG_getObjectRes("B_DS", 78)),
                                  1, 0, 1.0f, 0, -1);
            mpPatternBtkAnm->init(
                mpPatternModel->getModelData(),
                static_cast<J3DAnmTextureSRTKey*>(dComIfG_getObjectRes("B_DS", 84)), 1, 0, 1.0f, 0,
                -1);

            mPlayPatternAnm = true;
            mDamageTimer = 12;
            mWeakSph.ClrTgHit();
            mWeakSph.OnTgStopNoConHit();
            return 0;
        }
    }

    mWeakSph.OffTgStopNoConHit();
    mCcStts.Move();
    return 0;
}

void daB_DS_c::mBattle2_mtx_set() {
    cXyz vec1, vec2, vec3;

    J3DModel* model = mpMorf->getModel();
    if (!mDead) {
        mDoMtx_stack_c::transS(current.pos);
        mDoMtx_stack_c::transM(field_0x790.x, field_0x790.y, field_0x790.z);
        mDoMtx_stack_c::ZXYrotM(shape_angle);
        mDoMtx_stack_c::ZXYrotM(field_0x7ae);
        mDoMtx_stack_c::scaleM(l_HIO.mModelSize, l_HIO.mModelSize, l_HIO.mModelSize);
        model->setBaseTRMtx(mDoMtx_stack_c::get());
        mpMorf->modelCalc();

        mDoMtx_stack_c::copy(model->getAnmMtx(DS_HEAD_JNT_HEAD));
        mDoMtx_stack_c::multVecZero(&mHeadPos);

        if (mPlayPatternAnm) {
            mDoMtx_stack_c::copy(model->getAnmMtx(DS_HEAD_JNT_HEAD));
            mpPatternModel->setBaseTRMtx(mDoMtx_stack_c::get());
        }

        mDoMtx_stack_c::copy(model->getAnmMtx(DS_HEAD_JNT_HEAD));
        vec1.set(0.0f, 100.0f, 0.0f);
        mDoMtx_stack_c::multVec(&vec1, &mMouthPos);
    }

    if (!mDead) {
        mDoMtx_stack_c::copy(model->getAnmMtx(DS_HEAD_JNT_HEAD));
        vec1.set(590.0f, 175.0f, 0.0f);
        mDoMtx_stack_c::transM(vec1.x, vec1.y, vec1.z);
        mDoMtx_stack_c::YrotM(0x4000);
        mDoMtx_stack_c::XrotM(7000);
        mDoMtx_stack_c::scaleM(mSwordScale.x, mSwordScale.y, mSwordScale.z);
        mDoMtx_stack_c::multVecZero(&mSwordPos);
    } else {
        mDoMtx_stack_c::transS(current.pos);
        mDoMtx_stack_c::ZXYrotM(shape_angle);
        mDoMtx_stack_c::transM(0.0f, -50.0f, 0.0f);
    }

    mpSwordMorf->getModel()->setBaseTRMtx(mDoMtx_stack_c::get());
    mpSwordMorf->modelCalc();

    vec1.set(0.0f, 30.0f, 80.0f);
    mDoMtx_stack_c::multVec(&vec1, &eyePos);

    attention_info.position = eyePos;
    attention_info.position.y += 100.0f;

    {
        dBgS_LinChk lin_chk;
        csXyz angle;
        angle = shape_angle;
        mDoMtx_YrotS(*calc_mtx, angle.y);
        mDoMtx_XrotM(*calc_mtx, angle.x);
        vec2.set(0.0f, 0.0f, 4000.0f);
        MtxPosition(&vec2, &vec3);
        field_0x6d0 = mMouthPos + vec3;

        lin_chk.Set(&mMouthPos, &field_0x6d0, this);

        if (dComIfG_Bgsp().LineCross(&lin_chk)) {
            field_0x6d0 = lin_chk.GetCross();
        }
    }

    if (mCreateFireBreath) {
        mBattle2BreathFireSet(TYPE_BULLET_C);
    }

    if (mAction == ACT_B2_F_MOVE && mMode >= 3 && mMode <= 4) {
        mParticleKey2 =
            dComIfGp_particle_set(mParticleKey2, 0x8bfa, &current.pos, &shape_angle, NULL);
        JPABaseEmitter* emitter = dComIfGp_particle_getEmitter(mParticleKey2);
        if (emitter != NULL) {
            emitter->setGlobalSRTMatrix(mpMorf->getModel()->getAnmMtx(DS_HEAD_JNT_HEAD));
        }
    }
}

void daB_DS_c::mBattle2_cc_etc_set() {
    static const struct {
        int joint_no;
        Vec center;
        f32 radius;
    } B2_ETC_CC_DT[2] = {
        {0, {60.0f, 300.0f, -120.0f}, 410.0f},
        {0, {-290.0f, 400.0f, -300.0f}, 100.0f},
    };

    if (mAction == ACT_B2_OPENING_DEMO) {
        J3DModel* model = mpMorf->getModel();
        for (int i = 0; i < 2; i++) {
            mDoMtx_stack_c::copy(model->getAnmMtx(B2_ETC_CC_DT[i].joint_no));
            cXyz center_base = B2_ETC_CC_DT[i].center;
            cXyz center;
            mDoMtx_stack_c::multVec(&center_base, &center);
            mEtcSph[i].SetC(center);
            mEtcSph[i].SetR(B2_ETC_CC_DT[i].radius);
            dComIfG_Ccsp()->Set(&mEtcSph[i]);
        }
    }
}

void daB_DS_c::mBattle2_cc_set() {
    cXyz center_base;
    cXyz center;

    static cXyz head2_cc_dt[5] = {
        cXyz(380.0f, 0.0f, 0.0f),  cXyz(-50.0f, 50.0f, 0.0f),   cXyz(-400.0f, 130.0f, 0.0f),
        cXyz(640.0f, 80.0f, 0.0f), cXyz(670.0f, -100.0f, 0.0f),
    };

    static f32 head2_setRdt[5] = {
        300.0f, 350.0f, 270.0f, 190.0f, 190.0f,
    };

    mDoMtx_stack_c::copy(mpMorf->getModel()->getAnmMtx(DS_HEAD_JNT_HEAD));

    csXyz hit_angle;
    cXyz delta_pos;
    cXyz hit_pos;

    cLib_calcTimer(&mHitTimer);

    for (int i = 0; i < 5; i++) {
        center_base.set(head2_cc_dt[i]);
        mDoMtx_stack_c::multVec(&center_base, &center);

        f32 radius = head2_setRdt[i];
        mHeadSph[i].SetC(center);
        mHeadSph[i].SetR(radius);
        dComIfG_Ccsp()->Set(&mHeadSph[i]);

        if (mHeadSph[i].ChkTgHit()) {
            mAtInfo.mpCollider = mHeadSph[i].GetTgHitObj();
            mHeadSph[i].ClrTgHit();

            if (mHitTimer == 0) {
                delta_pos = current.pos - *mHeadSph[i].GetTgHitPosP();
                hit_pos = *mHeadSph[i].GetTgHitPosP();

                hit_angle.x = 0;
                hit_angle.y = delta_pos.atan2sX_Z();
                hit_angle.z = 0;
                dComIfGp_setHitMark(2, this, &hit_pos, &hit_angle, NULL, 0);
                def_se_set(&mSound, mAtInfo.mpCollider, 2, NULL);
                mHitTimer = 8;
            }
        }
    }
}

int daB_DS_c::execute() {
    if (arg0 == TYPE_BULLET_A || arg0 == TYPE_BULLET_B || arg0 == TYPE_BULLET_C) {
        mBulletAction();
        return 1;
    } else if (arg0 == TYPE_BATTLE_2) {
        mBattle2Action();
        mBattle2_mtx_set();
        mBattle2_cc_set();
        mBattle2_cc_etc_set();
        return 1;
    }

    action();

    if (mBossPhase == 100) {
        return 1;
    }

    handR_ang = -(4000.0f + BREG_F(15));
    handL_ang = -(4000.0f + BREG_F(15));

    mtx_set();

    u8 unused_bool = 0;
    cXyz jnt_pos;
    dBgS_GndChk gnd_chk;
    cXyz unused_0, chk_pos, unused_1;

    J3DModel* model = mpMorf->getModel();
    s8 var_r25;
    int i = 0;

    if (!mIsOpeningDemo) {
        s16 hand_x_ang_target = -6000;
        hand_x_ang_target += (s16)(mBackboneLevel * 1000);
        if (handX_ang > -4000) {
            handX_ang = -4000;
        }

        cLib_addCalcAngleS2(&handX_ang, hand_x_ang_target, 20, 0x100);

        // fake match?
        while (true) {
            var_r25 = false;
            mDoMtx_stack_c::copy(model->getAnmMtx(DS_JNT_HAND_L));
            mDoMtx_stack_c::multVecZero(&chk_pos);
            jnt_pos = chk_pos;

            chk_pos.y += 3000.0f;
            gnd_chk.SetPos(&chk_pos);
            chk_pos.y = dComIfG_Bgsp().GroundCross(&gnd_chk);
            if (chk_pos.y == -G_CM3D_F_INF) {
                chk_pos.y = jnt_pos.y;
            }

            if (jnt_pos.y < chk_pos.y) {
                handL_ang += (s16)(fabsf(jnt_pos.y - chk_pos.y) * 10.0f);
                var_r25 = true;

                if (field_0x84d & 1) {
                    if ((int)mpMorf->getFrame() >= 72 && (int)mpMorf->getFrame() < 79) {
                        hand_smokeSet(1);
                        field_0x84d ^= 1;
                    }
                }
            }

            mDoMtx_stack_c::copy(model->getAnmMtx(DS_JNT_HAND_R));
            mDoMtx_stack_c::multVecZero(&chk_pos);
            jnt_pos = chk_pos;

            chk_pos.y += 3000.0f;
            gnd_chk.SetPos(&chk_pos);
            chk_pos.y = dComIfG_Bgsp().GroundCross(&gnd_chk);
            if (chk_pos.y == -G_CM3D_F_INF) {
                chk_pos.y = jnt_pos.y;
            }

            if (jnt_pos.y < chk_pos.y) {
                handR_ang += (s16)(fabsf(jnt_pos.y - chk_pos.y) * 10.0f);
                var_r25 = true;
                if (field_0x84d & 2) {
                    if ((int)mpMorf->getFrame() >= 34 && (int)mpMorf->getFrame() < 41) {
                        hand_smokeSet(2);
                        field_0x84d ^= 2;
                    }
                }
            }

            if (!var_r25) {
                break;
            }

            mpMorf->updateDL();

            i++;
            if (i > 10) {
                break;
            }
        }
    } else {
        handL_ang = 0;
        handR_ang = 0;
        handX_ang = 0;
    }

    if (mBossPhase == 0) {
        neck_set();

        if (BREG_S(7)) {
            OS_REPORT("shape_angle.sx %x\n", shape_angle.x);
            OS_REPORT("mHeadAngle.sx %x\n", mHeadAngle.x);
            OS_REPORT("mHeadAngle.sy %x\n", mHeadAngle.y);
            OS_REPORT("mHeadAngle.sz %x\n", mHeadAngle.z);
            OS_REPORT("\n\n");
        }
    }

    cc_set();
    cc_etc_set();
    return 1;
}

static int daB_DS_Execute(daB_DS_c* i_this) {
    return i_this->execute();
}

static int daB_DS_IsDelete(daB_DS_c* i_this) {
    return 1;
}

int daB_DS_c::_delete() {
    dComIfG_resDelete(&mPhase, "B_DS");

    if (arg0 == TYPE_BULLET_A || arg0 == TYPE_BULLET_B || arg0 == TYPE_BULLET_C) {
        return 1;
    }

    if (mHIOInit) {
        hio_set = false;
    }

    if (heap != NULL) {
        mSound.deleteObject();
    }

    return 1;
}

static int daB_DS_Delete(daB_DS_c* i_this) {
    return i_this->_delete();
}

int daB_DS_c::CreateHeap() {
    if (arg0 == TYPE_BATTLE_1) {
        if (dComIfGs_isZoneSwitch(6, fopAcM_GetRoomNo(this)) &&
            dComIfGs_isZoneSwitch(7, fopAcM_GetRoomNo(this)))
        {
            arg0 = TYPE_BATTLE_2;
            home.pos.set(0.0f, 1900.0f, 0.0f);
            current.pos.set(0.0f, 1900.0f, 0.0f);
        }

        if (BREG_S(9)) {
            arg0 = TYPE_BATTLE_2;
            home.pos.set(0.0f, 1900.0f, 0.0f);
            current.pos.set(0.0f, 1900.0f, 0.0f);

            attention_info.distances[fopAc_attn_BATTLE_e] = 0;
            attention_info.flags = fopAc_AttnFlag_BATTLE_e;

            fopAcM_SetGroup(this, 2);
            fopAcM_OnStatus(this, 0);
        }
    }

    J3DModelData* modelData = static_cast<J3DModelData*>(dComIfG_getObjectRes("B_DS", 70));

    int anm_res = ANM_WAIT01_A;
    if (arg0 == TYPE_BATTLE_2) {
        modelData = static_cast<J3DModelData*>(dComIfG_getObjectRes("B_DS", 71));
        anm_res = ANM_HEAD_FWAIT;
    }

    JUT_ASSERT(0, modelData != NULL);

    mpMorf = new mDoExt_McaMorfSO(
        modelData, NULL, NULL, static_cast<J3DAnmTransform*>(dComIfG_getObjectRes("B_DS", anm_res)),
        0, 1.0f, 0, -1, &mSound, 0, 0x11000084);
    if (mpMorf == NULL || mpMorf->getModel() == NULL) {
        return 0;
    }

    model = mpMorf->getModel();
    model->setUserArea((uintptr_t)this);
    for (u16 i = 0; i < model->getModelData()->getJointNum(); i++) {
        if (i != 0) {
            model->getModelData()->getJointNodePointer(i)->setCallBack(JointCallBack);
        }
    }

    modelData = (J3DModelData*)dComIfG_getObjectRes("B_DS", 74);
    JUT_ASSERT(0, modelData != NULL);

    mpSwordMorf = new mDoExt_McaMorfSO(
        modelData, NULL, NULL, static_cast<J3DAnmTransform*>(dComIfG_getObjectRes("B_DS", 63)), 0,
        1.0f, 0, -1, NULL, 0, 0x11000084);
    if (mpSwordMorf == NULL || mpSwordMorf->getModel() == NULL) {
        return 0;
    }

    mpSwordBrkAnm = new mDoExt_brkAnm();
    if (mpSwordBrkAnm == NULL) {
        return 0;
    }

    if (!mpSwordBrkAnm->init(mpSwordMorf->getModel()->getModelData(),
                             static_cast<J3DAnmTevRegKey*>(dComIfG_getObjectRes("B_DS", 81)), 1, 0,
                             1.0f, 0, -1))
    {
        return 0;
    }

    modelData = (J3DModelData*)dComIfG_getObjectRes("B_DS", 75);
    JUT_ASSERT(0, modelData != NULL);

    mpZantMorf = new mDoExt_McaMorfSO(
        modelData, NULL, NULL, static_cast<J3DAnmTransform*>(dComIfG_getObjectRes("B_DS", 66)), 2,
        1.0f, 0, -1, NULL, 0, 0x11000084);
    if (mpZantMorf == NULL || mpZantMorf->getModel() == NULL) {
        return 0;
    }

    modelData = static_cast<J3DModelData*>(dComIfG_getObjectRes("B_DS", 73));
    JUT_ASSERT(0, modelData != NULL);

    mpOpPatternModel = mDoExt_J3DModel__create(modelData, 0, 0x11000284);
    if (mpOpPatternModel == NULL) {
        return 0;
    }

    mpOpPatternBrkAnm = new mDoExt_brkAnm();
    if (mpOpPatternBrkAnm == NULL) {
        return 0;
    }

    if (!mpOpPatternBrkAnm->init(mpOpPatternModel->getModelData(),
                                 static_cast<J3DAnmTevRegKey*>(dComIfG_getObjectRes("B_DS", 79)), 1,
                                 0, 1.0f, 0, -1))
    {
        return 0;
    }

    mpOpPatternBtkAnm = new mDoExt_btkAnm();
    if (mpOpPatternBtkAnm == NULL) {
        return 0;
    }

    if (!mpOpPatternBtkAnm->init(
            mpOpPatternModel->getModelData(),
            static_cast<J3DAnmTextureSRTKey*>(dComIfG_getObjectRes("B_DS", 85)), 1, 2, 1.0f, 0, -1))
    {
        return 0;
    }

    modelData = static_cast<J3DModelData*>(dComIfG_getObjectRes("B_DS", 72));
    JUT_ASSERT(0, modelData != NULL);

    mpPatternModel = mDoExt_J3DModel__create(modelData, 0, 0x11000284);
    if (mpPatternModel == NULL) {
        return 0;
    }

    mpPatternBrkAnm = new mDoExt_brkAnm();
    if (mpPatternBrkAnm == NULL) {
        return 0;
    }

    if (!mpPatternBrkAnm->init(mpPatternModel->getModelData(),
                               static_cast<J3DAnmTevRegKey*>(dComIfG_getObjectRes("B_DS", 78)), 1,
                               0, 1.0f, 0, -1))
    {
        return 0;
    }

    mpPatternBtkAnm = new mDoExt_btkAnm();
    if (mpPatternBtkAnm == NULL) {
        return 0;
    }

    if (!mpPatternBtkAnm->init(mpPatternModel->getModelData(),
                               static_cast<J3DAnmTextureSRTKey*>(dComIfG_getObjectRes("B_DS", 84)),
                               1, 2, 1.0f, 0, -1))
    {
        return 0;
    }

    return 1;
}

static int useHeapInit(fopAc_ac_c* i_this) {
    return static_cast<daB_DS_c*>(i_this)->CreateHeap();
}

cPhs__Step daB_DS_c::create() {
    fopAcM_ct(this, daB_DS_c);

    cPhs__Step phase_state = (cPhs__Step)dComIfG_resLoad(&mPhase, "B_DS");
    if (phase_state == cPhs_COMPLEATE_e) {
        if (arg0 == TYPE_BATTLE_1 && dComIfGs_isStageBossEnemy()) {
            // "After B_DS defeated, so not re-setting\n"
            OS_REPORT("B_DS やられ後なので再セットしません\n");

            dComIfGs_onZoneSwitch(8, fopAcM_GetRoomNo(this));
            g_env_light.mColpatPrevGather = 4;
            g_env_light.mColpatCurrGather = 4;
            g_env_light.mColPatBlendGather = 1.0f;

            if (!dComIfGs_isStageLife()) {
                cXyz scale(1.0f, 1.0f, 1.0f);
                cXyz pos;
                csXyz angle;
                angle.x = angle.y = angle.z = 0;
                pos.set(-400.0f, 1775.0f, -4132.0f);

                fopAcM_createItemForBoss(&pos, fpcNm_ITEM_UTAWA_HEART, fopAcM_GetRoomNo(this), &angle, &scale,
                                         0.0f, 0.0f, -1);
            }

            return cPhs_ERROR_e;
        }

        arg0 = fopAcM_GetParamBit(this, 0, 8);
        bitSw = fopAcM_GetParamBit(this, 8, 8);
        bitSw2 = fopAcM_GetParamBit(this, 16, 8);
        bitSw3 = fopAcM_GetParamBit(this, 24, 8);

        if (arg0 == 0xFF) {
            arg0 = TYPE_BATTLE_1;
        }

        if (arg0 == TYPE_BATTLE_1 || arg0 == TYPE_BATTLE_2) {
            OS_REPORT("\n\n");
            OS_REPORT("fopAcM_GetParam(this) %x\n", fopAcM_GetParam(this));
            OS_REPORT("arg0 %d\n", arg0);
            OS_REPORT("bitSw %d\n", bitSw);
            OS_REPORT("bitSw2 %d\n", bitSw2);
            OS_REPORT("bitSw3 %d\n", bitSw3);
            OS_REPORT("\n\n");
        }

        if (arg0 == TYPE_BULLET_A || arg0 == TYPE_BULLET_B || arg0 == TYPE_BULLET_C) {
            attention_info.distances[fopAc_attn_BATTLE_e] = 0;
            attention_info.flags &= ~fopAc_AttnFlag_BATTLE_e;

            fopAcM_SetGroup(this, 0);
            fopAcM_OffStatus(this, 0);

            mCcStts.Init(0xff, 0, this);
            gravity = 0.0f;
            mBreathAtSph.Set(cc_ds_breath_at_src);
            mBreathAtSph.SetStts(&mCcStts);

            fopAcM_OffStatus(this, 0);
            attention_info.flags &= ~fopAc_AttnFlag_BATTLE_e;

            if (arg0 == TYPE_BULLET_B || arg0 == TYPE_BULLET_C) {
                mAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1,
                          &mAcchCir, fopAcM_GetSpeed_p(this), NULL, NULL);
                mAcch.CrrPos(dComIfG_Bgsp());
                fopAcM_OnStatus(this, 0x4000);
            }

            setActionMode(8, 0);
        } else {
            if (arg0 != TYPE_BULLET_A && arg0 != TYPE_BULLET_B && arg0 != TYPE_BULLET_C &&
                !fopAcM_entrySolidHeap(this, useHeapInit, 0xfac0))
            {
                return cPhs_ERROR_e;
            }

            OS_REPORT("B_DS PARAM %x\n", fopAcM_GetParam(this));

            field_0x560 = health = l_HIO.mP2Health;

            if (!hio_set) {
                hio_set = 1;
                mHIOInit = true;
                l_HIO.field_0x04 = -1;
            }

            shape_angle.z = 0;
            shape_angle.x = 0;
            shape_angle.y = current.angle.y;

            attention_info.distances[fopAc_attn_BATTLE_e] = 0;
            attention_info.flags = fopAc_AttnFlag_BATTLE_e;

            fopAcM_SetMtx(this, mpMorf->mpModel->getBaseTRMtx());
            fopAcM_SetMin(this, -40000.0f, -40000.0f, -40000.0f);
            fopAcM_SetMax(this, 40000.0f, 40000.0f, 40000.0f);

            mAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1, &mAcchCir,
                      fopAcM_GetSpeed_p(this), NULL, NULL);
            mCcStts.Init(0xff, 0, this);
            mWeakSph.Set(cc_ds_week_src);
            mWeakSph.SetStts(&mCcStts);

            {
                dBgS_GndChk gnd_chk;
                cXyz chk_pos = current.pos;
                chk_pos.y += 800.0f;
                gnd_chk.SetPos(&chk_pos);

                chk_pos.y = dComIfG_Bgsp().GroundCross(&gnd_chk);
                if (chk_pos.y != -G_CM3D_F_INF) {
                    current.pos.y = chk_pos.y;
                }
            }

            for (int i = 0; i < 5; i++) {
                mHeadSph[i].Set(cc_ds_head_src);
                mHeadSph[i].SetStts(&mCcStts);
            }

            for (int i = 0; i < 18; i++) {
                mEtcSph[i].Set(cc_ds_head_src);
                mEtcSph[i].SetStts(&mCcStts);
            }

            mWallR = 500.0f;

            mBackboneCyl.Set(cc_ds_backbone_src);
            mBackboneCyl.SetStts(&mCcStts);

            mHandAtLCyl.Set(cc_ds_hand_at_cyl_src);
            mHandAtLCyl.SetStts(&mCcStts);

            mHandAtRCyl.Set(cc_ds_hand_at_cyl_src);
            mHandAtRCyl.SetStts(&mCcStts);

            mHandAtLCyl.OffAtSetBit();
            mHandAtRCyl.OffAtSetBit();

            mHandAtLCyl.OnTgShield();
            mHandAtRCyl.OnTgShield();

            mSoundPos = current.pos;
            mSound.init(&mSoundPos, &eyePos, 3, 1);
            mAtInfo.mpSound = &mSound;
            mAtInfo.mPowerType = 11;

            mSwordPos = current.pos;
            for (int i = 0; i < 2; i++) {
                mHandPos[i] = current.pos;
            }

            for (int i = 0; i < 3; i++) {
                mBackboneCrackAlpha[i] = 255.0f;
            }

            mEyeColorAlpha = 255.0f;
            mCrackAlpha = 0.0f;
            mpSwordMorf->setStartFrame(379.0f);

            mZantScale.set(0.0f, 0.0f, 0.0f);
            mSwordScale.set(1.0f, 1.0f, 1.0f);

            dComIfGs_onZoneSwitch(8, fopAcM_GetRoomNo(this));
            onWolfNoLock();

            mHintTimer1 = l_HIO.mHintTime1;
            mHintTimer2 = l_HIO.mHintTime1;

            if (arg0 == TYPE_BATTLE_1) {
                gravity = -1.0f;

                if (bitSw != 0xff) {
                    fopAcM_onSwitch(this, bitSw);
                }

                fopAcM_OnStatus(this, 0x4000);

                if (cDmr_SkipInfo == 0 && !dComIfGs_isZoneSwitch(5, fopAcM_GetRoomNo(this))) {
                    mDrawZant = false;
                    mNoDrawSword = true;

                    dComIfGs_offZoneSwitch(8, fopAcM_GetRoomNo(this));
                    mIsOpeningDemo = true;

                    mEyeColorAlpha = 0.0f;
                    mCrackAlpha = 255.0f;

                    if (bitSw != 0xFF) {
                        fopAcM_offSwitch(this, bitSw);
                    }

                    attention_info.distances[fopAc_attn_BATTLE_e] = 0;
                    attention_info.flags &= ~fopAc_AttnFlag_BATTLE_e;

                    fopAcM_SetGroup(this, 0);
                    fopAcM_OffStatus(this, 0);

                    if (!dComIfGs_isZoneSwitch(0, fopAcM_GetRoomNo(this))) {
                        setActionMode(ACT_OPENING_DEMO, 0);
                    } else {
                        setActionMode(ACT_OPENING_DEMO, 3);
                    }

                    setBck(ANM_OPDEMO, 0, 1.0f, 0.0f);
                } else {
                    cDmr_SkipInfo = 0;
                    dComIfGs_onZoneSwitch(5, fopAcM_GetRoomNo(this));
                }

                if (bitSw3 != 0xFF) {
                    fopAcM_offSwitch(this, bitSw3);
                }

                if (mAction != ACT_OPENING_DEMO) {
                    field_0x7f8 = 0.5f;
                    Z2GetAudioMgr()->bgmStart(Z2BGM_HARAGIGANT_BTL01, 0, 0);
                    dKy_change_colpat(1);
                    mCreateTrap(true);
                    setActionMode(ACT_WAIT, 0);
                }
            } else {
                mChangeVer2();
                gravity = -1.0f;

                attention_info.distances[fopAc_attn_BATTLE_e] = 0;
                attention_info.flags &= ~fopAc_AttnFlag_BATTLE_e;

                fopAcM_SetGroup(this, 0);
                fopAcM_OffStatus(this, 0);

                shape_angle.y = -0x500A;
                mEyeColorAlpha = 0.0f;
                current.pos.set(345.0f, -1375.0f, -1040.0f);
                shape_angle.y = 20000;
                shape_angle.z = -15000;

                setActionMode(ACT_B2_OPENING_DEMO, 0);
            }
        }

        daB_DS_Execute(this);
    }

    return phase_state;
}

daB_DS_c::daB_DS_c() {}

static cPhs__Step daB_DS_Create(daB_DS_c* i_this) {
    return i_this->create();
}

static actor_method_class l_daB_DS_Method = {
    (process_method_func)daB_DS_Create,  (process_method_func)daB_DS_Delete,
    (process_method_func)daB_DS_Execute, (process_method_func)daB_DS_IsDelete,
    (process_method_func)daB_DS_Draw,
};

actor_process_profile_definition g_profile_B_DS = {
    fpcLy_CURRENT_e,
    4,
    fpcPi_CURRENT_e,
    PROC_B_DS,
    &g_fpcLf_Method.base,
    sizeof(daB_DS_c),
    0,
    0,
    &g_fopAc_Method.base,
    0xDC,
    &l_daB_DS_Method,
    0x4040000,
    fopAc_ENEMY_e,
    fopAc_CULLBOX_CUSTOM_e,
};

AUDIO_INSTANCES;
