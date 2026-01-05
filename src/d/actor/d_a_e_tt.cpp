/**
 * @file d_a_e_tt.cpp
 *
 */

#include "d/dolzel_rel.h"  // IWYU pragma: keep

#include "Z2AudioLib/Z2Instances.h"
#include "d/actor/d_a_e_tt.h"
#include "d/d_com_inf_game.h"
#include "f_op/f_op_actor_enemy.h"
#include "f_op/f_op_camera_mng.h"

class daE_TT_HIO_c : public JORReflexible {
public:
    daE_TT_HIO_c();
    virtual ~daE_TT_HIO_c() {}

    void genMessage(JORMContext*);

    /* 0x04 */ s8 id;
    /* 0x08 */ f32 model_size;
    /* 0x0C */ f32 player_detection_range;
    /* 0x10 */ f32 player_attack_distance;
    /* 0x14 */ f32 attack_angle;
};

enum daE_TT_Action {
    /*  0 */ ACTION_WAIT,
    /*  1 */ ACTION_CHASE,
    /*  2 */ ACTION_ATTACK,
    /*  3 */ ACTION_DAMAGE,
    /*  4 */ ACTION_DEATH,
    /*  5 */ ACTION_OUT_RANGE,
    /*  6 */ ACTION_FIRST_ATTACK,
};

namespace {
static dCcD_SrcSph cc_tt_src = {
    {
        {0x0, {{0x400, 0x1, 0xD}, {0xD8FBFDFF, 0x43}, 0x75}},
        {dCcD_SE_METAL, 0x0, 0x0, 0x0, 0x0},
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x2},
        {0x0},
    },
    {{{0.0f, 0.0f, 0.0f}, 40.0f}},

};
static dCcD_SrcSph cc_tt_eye_src = {
    {
        {0x0, {{0x0, 0x1, 0x0}, {0x2000, 0x43}, 0x0}},
        {dCcD_SE_METAL, 0x0, 0x0, 0x0, 0x0},
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x2},
        {0x0},
    },
    {{{0.0f, 0.0f, 0.0f}, 40.0f}},
};
}  // namespace

daE_TT_HIO_c::daE_TT_HIO_c() {
    id = -1;
    model_size = 0.6f;
    player_detection_range = 1300.0f;
    player_attack_distance = 400.0f;
    attack_angle = 8192.0f;
}

int daE_TT_c::ctrlJoint(J3DJoint* i_joint, J3DModel* i_model) {
    s32 jntNo = i_joint->getJntNo();
    mDoMtx_stack_c::copy(i_model->getAnmMtx(jntNo));

    switch (jntNo) {
    case 0x4:
        mDoMtx_stack_c::ZrotM(mFootJoints[0]);
        break;

    case 0x8:
        mDoMtx_stack_c::ZrotM(mFootJoints[1]);
        break;

    case 0xD:
        mDoMtx_stack_c::ZrotM(mFootJoints[2]);
        break;

    case 0x11:
        mDoMtx_stack_c::ZrotM(mFootJoints[3]);
        break;
    }

    i_model->setAnmMtx(jntNo, mDoMtx_stack_c::get());
    MTXCopy(mDoMtx_stack_c::get(), J3DSys::mCurrentMtx);

    return 1;
}

int daE_TT_c::JointCallBack(J3DJoint* i_joint, int param_1) {
    if (param_1 == 0) {
        J3DModel* model = j3dSys.getModel();
        daE_TT_c* kk_p = (daE_TT_c*)model->getUserArea();

        if (kk_p != NULL) {
            kk_p->ctrlJoint(i_joint, model);
        }
    }
    return 1;
}

int daE_TT_c::draw() {
    J3DModel* model = mpMorfSO->getModel();

    g_env_light.settingTevStruct(0, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(model, &tevStr);

    if (mDeathColor) {
        J3DModelData* modelData = model->getModelData();
        for (u16 i = 0; i < modelData->getMaterialNum(); i++) {
            J3DMaterial* material = modelData->getMaterialNodePointer(i);
            material->getTevColor(0)->r = mDeathColor;
            material->getTevColor(0)->g = mDeathColor;
            material->getTevColor(0)->b = mDeathColor;
        }
    }

    mpMorfSO->entryDL();
    cXyz modified_pos;
    modified_pos.set(current.pos.x, current.pos.y + 100.0f, current.pos.z);
    mShadowKey = dComIfGd_setShadow(mShadowKey, 1, model, &modified_pos, 700.0f, 0.0f,
                                    current.pos.y, mObjAcch.GetGroundH(), mObjAcch.m_gnd, &tevStr,
                                    0, 1.0f, dDlst_shadowControl_c::getSimpleTex());
    return 1;
}

static int daE_TT_Draw(daE_TT_c* i_this) {
    return i_this->draw();
}

void daE_TT_c::setBck(int i_index, u8 i_attr, f32 i_morf, f32 i_rate) {
    J3DAnmTransform* animation = (J3DAnmTransform*)dComIfG_getObjectRes("E_TT", i_index);
    mpMorfSO->setAnm(animation, i_attr, i_morf, i_rate, 0.0f, -1.0f);
}

void daE_TT_c::calcHitGroundSpeed() {
    if (!mTektiteOnWater) {
        speedF = 0.0f;
    } else {
        if (speedF > 10.0f) {
            speedF = 10.0f;
        }
        cLib_chaseF(&speedF, 0.0f, 0.2f);
    }
}

static u8 hio_set;

static daE_TT_HIO_c l_HIO;

static daE_TT_c* m_attack_tt;

void daE_TT_c::setActionMode(int i_action, int i_mode) {
    if (m_attack_tt == this) {
        m_attack_tt = NULL;
    }

    mAction = i_action;
    mMode = i_mode;
    field_0x6fb = 0;
}

int daE_TT_c::getCutType() {
    switch (daPy_getPlayerActorClass()->getCutType()) {
    case daPy_py_c::CUT_TYPE_NM_VERTICAL:
    case daPy_py_c::CUT_TYPE_NM_STAB:
    case daPy_py_c::CUT_TYPE_NM_RIGHT:
    case daPy_py_c::CUT_TYPE_DASH_UNK_26:
        return 0;

    default:
        return 1;
    }
}

void daE_TT_c::damage_check() {
    if (mDamageCooldownTimer == 0) {
        mStts.Move();

        if (mSphere.ChkTgHit()) {
            mAtInfo.mpCollider = mSphere.GetTgHitObj();
            u8 mode = 2;
            s32 iVar = 10;
            if (mAtInfo.mpCollider->ChkAtType(AT_TYPE_ARROW)) {
                iVar = 0x28;
                mode = 5;
            } else if (mAtInfo.mpCollider->ChkAtType(AT_TYPE_IRON_BALL | AT_TYPE_BOMB)) {
                mode = 6;
                iVar = 0x28;
            } else if (mAtInfo.mpCollider->ChkAtType(AT_TYPE_MASTER_SWORD | AT_TYPE_NORMAL_SWORD)) {
                if (daPy_getPlayerActorClass()->getCutCount() >= 4 ||
                    ((dCcD_GObjInf*)mAtInfo.mpCollider)->GetAtSpl() == 1)
                {
                    mode = 6;
                } else {
                    if (mPlayerCutTimer != 0) {
                        mPlayerCutType ^= 1;
                    } else {
                        mPlayerCutType = getCutType();
                    }
                    mode = mPlayerCutType;
                    mPlayerCutTimer = 30;
                }
            } else if (mAtInfo.mpCollider->ChkAtType(AT_TYPE_SHIELD_ATTACK)) {
                mode = 3;
            } else if (mAtInfo.mpCollider->ChkAtType(AT_TYPE_BOOMERANG | AT_TYPE_HOOKSHOT)) {
                mAtInfo.mpCollider->SetAtAtp(0);
                mode = 4;
            }

            if ((s16)mAtInfo.mpCollider->GetAtAtp() <= 0) {
                iVar = 0;
            }

            field_0x6f0 += iVar;
            health = 100;

            if (field_0x6f0 >= 0x28) {
                health = 0;
            }

            cc_at_check(this, &mAtInfo);

            if (mAtInfo.mpCollider->ChkAtType(AT_TYPE_UNK)) {
                mDamageCooldownTimer = 20;
            } else {
                mDamageCooldownTimer = 10;
            }

            if (mAtInfo.mAttackPower <= 1) {
                mDamageCooldownTimer = 10;
            }

            if (mode >= 5) {
                setActionMode(ACTION_DEATH, mode - 5);
            } else {
                if (health <= 0) {
                    setActionMode(ACTION_DEATH, 2);
                } else {
                    setActionMode(ACTION_DAMAGE, mode);
                }
            }
        }
    }
}

void daE_TT_c::checkWaterSurface() {
    mTektiteOnWater = 0;

    if (speed.y <= 0.0f) {
        Vec position = current.pos;
        position.y += 500.0f;

        mGndChk.SetPos(&position);

        f32 ground_cross = dComIfG_Bgsp().GroundCross(&mGndChk);
        if (ground_cross != -G_CM3D_F_INF) {
            if (dComIfG_Bgsp().GetPolyAtt0(mGndChk) == 7 && current.pos.y <= ground_cross) {
                current.pos.y = ground_cross;
                speed.y = 0.0f;
                mTektiteOnWater = 1;
                mObjAcch.SetGroundHit();
            }
        }
    }
}

void daE_TT_c::checkFootGround() {
    static int foot_jnt_no[4] = {
        0x4,
        0x8,
        0xD,
        0x11,
    };

    if (mAction != ACTION_DEATH) {
        cXyz pos1;
        cXyz pos2;
        cXyz pos3;
        cXyz pos4;
        dBgS_LinChk linChk;
        static cXyz sc(0.75f, 0.75f, 0.75f);
        int local_d0[4];
        int local_e0[4];

        for (int i = 0; i < 4; i++) {
            local_d0[i] = 0;
            local_e0[i] = 0;

            mDoMtx_stack_c::copy(mpMorfSO->getModel()->getAnmMtx(foot_jnt_no[i]));
            mDoMtx_stack_c::transM(-20.0f, 0.0f, 0.0f);
            mDoMtx_stack_c::multVecZero(&pos2);
            mDoMtx_stack_c::transM(90.0f, 0.0f, 0.0f);
            mDoMtx_stack_c::multVecZero(&pos3);
            mDoMtx_stack_c::transM(30.0f, 0.0f, 0.0f);
            mDoMtx_stack_c::multVecZero(&pos4);

            pos1.set(pos3.x, pos3.y + 50.0f, pos3.z);

            f32 fVar5 = pos3.y;
            if (fopAcM_wt_c::waterCheck(&pos1)) {
                fVar5 = fopAcM_wt_c::getWaterY();
                if (fabsf(current.pos.y - fVar5) < 40.0f) {
                    local_e0[i] = 1;
                    if (fVar5 - current.pos.y > -10.0f) {
                        local_d0[i] = 1;
                    }
                }
            }

            linChk.Set(&pos2, &pos4, this);
            if (!dComIfG_Bgsp().LineCross(&linChk)) {
                if (local_e0[i] == 1) {
                    pos1.set(pos3.x, fVar5, pos3.z);
                    mPolyColors[i] = dComIfGp_particle_setPolyColor(
                        mPolyColors[i], ID_ZI_J_DOWNWTRA_A, *fopAcM_wt_c::getWaterCheck(), &pos1,
                        &tevStr, &shape_angle, &sc, 0, NULL, -1, NULL);
                    JPABaseEmitter* emitter = dComIfGp_particle_getEmitter(mPolyColors[i]);
                    if (emitter != NULL) {
                        emitter->setRateStep(5);
                    }
                }
            } else {
                local_d0[i] = 1;
            }

            if (mObjAcch.ChkGroundHit() && !mTektiteOnWater) {
                if (local_d0[i] != 0) {
                    cLib_addCalcAngleS(&mFootJoints[i], 0, 8, 0x800, 0x100);
                } else {
                    cLib_addCalcAngleS(&mFootJoints[i], -0x2800, 8, 0x800, 0x100);
                }
            } else {
                mFootJoints[i] = 0;
            }
        }
    }
}

void daE_TT_c::setBodyLandEffect() {
    static u16 w_eff_id[4] = {ID_ZI_J_DOWNWTRA_A, ID_ZI_J_DOWNWTRA_B, ID_ZI_J_DOWNWTRA_C,
                              ID_ZI_J_DOWNWTRA_D};
    static cXyz sc(1.2f, 1.2f, 1.2f);

    if (mTektiteOnWater) {
        for (int i = 0; i < 4; i++) {
            JPABaseEmitter* emitter = dComIfGp_particle_setPolyColor(
                w_eff_id[i], mGndChk, &current.pos, &tevStr, &shape_angle, &sc, 0, NULL, -1, NULL);
            if (emitter != NULL) {
                emitter->setMaxFrame(1);
            }
        }
        mSound.startCreatureSound(Z2SE_EN_TT_LAND_WATER, 0, -1);
    } else {
        dComIfGp_particle_setPolyColor(ID_ZI_J_DOWNSMOKE_A, mObjAcch.m_gnd, &current.pos, &tevStr,
                                       &shape_angle, &sc, 0, NULL, -1, NULL);
        mSound.startCreatureSound(Z2SE_CM_BODYFALL_S, 0, -1);
    }
}

void daE_TT_c::setWaterEffect() {
    static u16 w_eff_id[3] = {
        ID_ZI_J_DOWNWTRA_B,
        ID_ZI_J_DOWNWTRA_C,
        ID_ZI_J_DOWNWTRA_D,
    };
    static s32 foot_jnt_no[4] = {
        0x4,
        0x8,
        0xD,
        0x11,
    };
    static cXyz sc(0.75f, 0.75f, 0.75f);
    cXyz start;
    cXyz end;
    dBgS_CamLinChk_NorWtr camLinChk;

    for (int i = 0; i < 4; i++) {
        mDoMtx_stack_c::copy(mpMorfSO->getModel()->getAnmMtx(foot_jnt_no[i]));
        mDoMtx_stack_c::transM(-20.0f, 0.0f, 0.0f);
        mDoMtx_stack_c::multVecZero(&start);
        mDoMtx_stack_c::transM(120.0f, 0.0f, 0.0f);
        mDoMtx_stack_c::multVecZero(&end);

        camLinChk.Set(&start, &end, this);
        if (dComIfG_Bgsp().LineCross(&camLinChk) && dComIfG_Bgsp().GetPolyAtt0(camLinChk) == 7) {
            cXyz cross(end.x, camLinChk.GetCross().y, end.z);
            if (fabsf(current.pos.y - cross.y) < 20.0f) {
                for (int j = 0; j < 3; j++) {
                    JPABaseEmitter* polyEmitter =
                        dComIfGp_particle_setPolyColor(w_eff_id[j], camLinChk, &cross, &tevStr,
                                                       &shape_angle, &sc, 0, NULL, -1, NULL);
                    if (polyEmitter != NULL) {
                        polyEmitter->setMaxFrame(1);
                    }
                }
            }
        }
    }

    if (mObjAcch.ChkGroundHit()) {
        if (!mTektiteOnWater) {
            mSound.startCreatureSound(Z2SE_EN_TT_LAND, 0, -1);
            sc.set(1.2f, 1.2f, 1.2f);
            dComIfGp_particle_setPolyColor(ID_ZI_J_DOWNSMOKE_A, mObjAcch.m_gnd, &current.pos,
                                           &tevStr, &shape_angle, &sc, 0, NULL, -1, NULL);
        } else {
            mSound.startCreatureSound(Z2SE_EN_TT_LAND_WATER, 0, -1);
        }
    }
}

void daE_TT_c::setDeathEyeEffect() {
    static cXyz sc(0.6f, 0.6f, 0.6f);

    if (!mTektiteType) {
        mDeathParticle = dComIfGp_particle_set(
            mDeathParticle, dPa_RM(ID_ZF_S_TT_DAMAGEARROWRED00_PURPLE), &current.pos, &tevStr,
            &shape_angle, &sc, 0xFF, NULL, -1, NULL, NULL, NULL);
        mDeathParticle2 = dComIfGp_particle_set(
            mDeathParticle2, dPa_RM(ID_ZF_S_TT_DAMAGEARROWRED01_PURPLE), &current.pos, &tevStr,
            &shape_angle, &sc, 0xFF, NULL, -1, NULL, NULL, NULL);
    } else {
        mDeathParticle = dComIfGp_particle_set(
            mDeathParticle, dPa_RM(ID_ZF_S_TT_DAMAGEARROWBLUE00_ORANGE), &current.pos, &tevStr,
            &shape_angle, &sc, 0xFF, NULL, -1, NULL, NULL, NULL);
        mDeathParticle2 = dComIfGp_particle_set(
            mDeathParticle2, dPa_RM(ID_ZF_S_TT_DAMAGEARROWBLUE01_ORANGE), &current.pos, &tevStr,
            &shape_angle, &sc, 0xFF, NULL, -1, NULL, NULL, NULL);
    }

    JPABaseEmitter* emitter = dComIfGp_particle_getEmitter(mDeathParticle);
    if (emitter != NULL) {
        emitter->setGlobalSRTMatrix(mpMorfSO->getModel()->getAnmMtx(9));
    }

    JPABaseEmitter* emitter2 = dComIfGp_particle_getEmitter(mDeathParticle2);
    if (emitter2 != NULL) {
        emitter2->setGlobalSRTMatrix(mpMorfSO->getModel()->getAnmMtx(9));
    }
}

void daE_TT_c::setDeathFootEffect() {
    static cXyz sc(0.6f, 0.6f, 0.6f);

    cXyz unused;
    mDoMtx_stack_c::copy(mpMorfSO->getModel()->getAnmMtx(0));
    mDoMtx_stack_c::multVecZero(&unused);

    csXyz rot;
    mDoMtx_MtxToRot(mpMorfSO->getModel()->getAnmMtx(0), &rot);

    mDeathParticle = dComIfGp_particle_set(
        mDeathParticle, dPa_RM(ID_ZF_S_TT_LEGOFFBACKBONEL00_SPLASH), &current.pos, &tevStr,
        &shape_angle, &sc, 0xFF, NULL, -1, NULL, NULL, NULL);
    mDeathParticle2 = dComIfGp_particle_set(
        mDeathParticle2, dPa_RM(ID_ZF_S_TT_LEGOFFBACKBONER00_SPLASH), &current.pos, &tevStr,
        &shape_angle, &sc, 0xFF, NULL, -1, NULL, NULL, NULL);

    JPABaseEmitter* emitter = dComIfGp_particle_getEmitter(mDeathParticle);
    if (emitter != NULL) {
        emitter->setGlobalSRTMatrix(mpMorfSO->getModel()->getAnmMtx(0));
    }

    JPABaseEmitter* emitter2 = dComIfGp_particle_getEmitter(mDeathParticle2);
    if (emitter2 != NULL) {
        emitter2->setGlobalSRTMatrix(mpMorfSO->getModel()->getAnmMtx(0));
    }
}

bool daE_TT_c::checkPlayerSearch() {
    if (field_0x6d4 && current.pos.absXZ(home.pos) > field_0x6d4 + 300.0f ||
        fopAcM_searchPlayerDistance(this) > l_HIO.player_detection_range + 300.0f ||
        fopAcM_otherBgCheck(this, daPy_getPlayerActorClass()))
    {
        return false;
    }
    return true;
}

void daE_TT_c::executeWait() {
    switch (mMode) {
    case 0:
        gravity = -5.0f;
        setBck(0xE, 2, 3.0f, 1.0f);
        if (field_0x6d4 && home.pos.absXZ(current.pos) < field_0x6d4 - 200.0f) {
            mGenericTimer = (u8)(cM_rndF(120.0f) + 120.0f);
        } else {
            if (cM_rnd() < 0.3f) {
                mGenericTimer = 30;
            } else {
                mGenericTimer = 10;
            }
        }
        mMode = 1;
        if (current.pos.absXZ(home.pos) > 500.0f) {
            field_0x6e0 = cLib_targetAngleY(&current.pos, &home.pos);

            if ((s16)(shape_angle.y - field_0x6e0) > 0x3000) {
                field_0x6e0 = shape_angle.y - 0x3000;
            } else if ((s16)(shape_angle.y - field_0x6e0) < -0x3000) {
                field_0x6e0 = shape_angle.y + 0x3000;
            }
        } else {
            field_0x6e0 = (s16)cM_rndFX(12288.0f);
        }

    case 1:
        if (mpMorfSO->checkFrame(0.0f)) {
            mSound.startCreatureVoice(Z2SE_EN_TT_V_WAIT, -1);
        }
        if (mGenericTimer < 30) {
            cLib_addCalcAngleS(&shape_angle.y, field_0x6e0, 0x8, 0x400, 0x100);
            current.angle.y = shape_angle.y;
        }
        if (mGenericTimer == 0) {
            mMode = 5;
            setBck(0xB, 0, 3.0f, 1.0f);
        } else {
            if (checkPlayerSearch()) {
                setActionMode(ACTION_CHASE, 0);
            }
        }
        break;

    case 5:
        if (mpMorfSO->checkFrame(4.0f)) {
            mSound.startCreatureSound(Z2SE_EN_TT_JUMP, 0, -1);
        }
        if (mpMorfSO->isStop()) {
            setBck(0xC, 0, 3.0f, 1.0f);
            mMode = 6;
            speedF = 15.0f;
            speed.y = 35.0f;
        }
        break;

    case 6:
        mSphere.OnAtSetBit();
        if (mObjAcch.ChkRoofHit()) {
            speed.y = 0.0f;
        }
        if (speed.y <= 0.0f) {
            setBck(0xD, 0, 5.0f, 0.0f);
            mMode = 7;
        }
        break;

    case 7:
        mSphere.OnAtSetBit();
        if (mObjAcch.ChkGroundHit()) {
            mpMorfSO->setPlaySpeed(1.5f);
            mMode = 8;
            setWaterEffect();
            mGenericTimer = 5;
        }
        break;

    case 8:
        if (mGenericTimer != 0) {
            mSphere.OnAtSetBit();
        }
        calcHitGroundSpeed();
        if (!speedF && mpMorfSO->isStop()) {
            mMode = 0;
        }
        break;
    }
}

void daE_TT_c::executeChase() {
    f32 playerDist = fopAcM_searchPlayerDistance(this);

    switch (mMode) {
    case 0:
        mGenericTimer = 30;

    case 10:
        gravity = -5.0f;
        setBck(0x10, 2, 3.0f, 1.0f);
        mMode = 1;

    case 1:
        if (mpMorfSO->checkFrame(0.0f)) {
            mSound.startCreatureVoice(Z2SE_EN_TT_V_WAIT, -1);
            mSound.startCreatureSound(Z2SE_EN_TT_WAIT, 0, -1);
        }
        if (mGenericTimer == 0 && field_0x6d4 && home.pos.absXZ(current.pos) > field_0x6d4) {
            setActionMode(ACTION_OUT_RANGE, 0);
        } else {
            cLib_addCalcAngleS(&shape_angle.y, fopAcM_searchPlayerAngleY(this), 0x8, 0x800, 0x100);
            current.angle.y = shape_angle.y;
            if (playerDist > l_HIO.player_attack_distance) {
                mMode = 5;
                setBck(0xB, 0, 3.0f, 1.0f);
            } else {
                if (!checkPlayerSearch()) {
                    setActionMode(ACTION_WAIT, 0);
                }
                if (mAttackTimer == 0 && !fopAcM_CheckCondition(this, fopAcCnd_NODRAW_e)) {
                    if ((s16)abs((s16)(daPy_getPlayerActorClass()->shape_angle.y -
                                       fopAcM_searchPlayerAngleY(this))) > l_HIO.attack_angle &&
                        m_attack_tt == NULL)
                    {
                        setActionMode(ACTION_ATTACK, 0);
                        m_attack_tt = this;
                    }
                }
            }
        }
        break;

    case 5:
        if (mpMorfSO->checkFrame(4.0f)) {
            mSound.startCreatureSound(Z2SE_EN_TT_JUMP, 0, -1);
        }
        cLib_addCalcAngleS(&shape_angle.y, fopAcM_searchPlayerAngleY(this), 0x8, 0x800, 0x100);
        current.angle.y = shape_angle.y;
        if (mpMorfSO->isStop()) {
            setBck(0xC, 0, 3.0f, 1.0f);
            mMode = 6;
            if (playerDist < l_HIO.player_attack_distance + 250.0f) {
                f32 dist = playerDist - l_HIO.player_attack_distance;
                if (dist > 250.0f) {
                    dist = 250.0f;
                }
                if (dist < 0.0f) {
                    dist = 0.0f;
                }
                speedF = (dist * 15.0f) / 250.0f + 5.0f;
                speed.y = 35.0f;
            } else {
                speedF = 20.0f;
                speed.y = 35.0f;
            }
        }
        break;

    case 6:
        mSphere.OnAtSetBit();
        if (mObjAcch.ChkRoofHit()) {
            speed.y = 0.0f;
        }
        if (speed.y <= 0.0f) {
            setBck(0xD, 0, 5.0f, 0.0f);
            mMode = 7;
        }
        break;

    case 7:
        mSphere.OnAtSetBit();
        if (mObjAcch.ChkGroundHit()) {
            mpMorfSO->setPlaySpeed(1.5f);
            mMode = 8;
            setWaterEffect();
            mGenericTimer = 5;
        }
        break;

    case 8:
        if (mGenericTimer != 0) {
            mSphere.OnAtSetBit();
        }
        calcHitGroundSpeed();
        if (!speedF && mpMorfSO->isStop()) {
            mMode = 10;
        }
        break;
    }
}

void daE_TT_c::executeAttack() {
    cXyz playerPos(daPy_getPlayerActorClass()->current.pos);

    switch (mMode) {
    case 0:
        mSound.startCreatureVoice(Z2SE_EN_TT_V_JUMP, -1);
        setBck(3, 0, 3.0f, 1.0f);
        mMode = 1;
        mAttackTimer = 100;
        gravity = -10.0f;
        break;

    case 1:
        cLib_addCalcAngleS(&shape_angle.y, fopAcM_searchPlayerAngleY(this), 0x8, 0x400, 0x100);
        if (mpMorfSO->checkFrame(35.0f)) {
            mSound.startCreatureSound(Z2SE_EN_TT_JUMP, 0, -1);
        }
        if (mpMorfSO->isStop()) {
            setBck(0xC, 0, 3.0f, 1.0f);
            mMode = 2;
            speed.y = 85.0f;
            f32 playerAbs = playerPos.absXZ(current.pos) / 17.0f;
            if (playerAbs > 50.0f) {
                playerAbs = 50.0f;
            }
            speedF = playerAbs;
            current.angle.y = shape_angle.y;
        }
        break;

    case 2:
        mSphere.OnAtSetBit();
        if (mObjAcch.ChkRoofHit()) {
            speed.y = 0.0f;
        }
        if (speed.y <= 0.0f) {
            setBck(0xD, 0, 5.0f, 0.0f);
            mMode = 3;
        }

    case 3:
        mSphere.OnAtSetBit();
        if (mObjAcch.ChkGroundHit()) {
            field_0x6fb = 1;
            mpMorfSO->setPlaySpeed(1.5f);
            mMode = 4;
            setWaterEffect();
            if (mTektiteOnWater) {
                mTransOffsetVelocity = -3.0f;
            }
            mGenericTimer = 5;
        }
        break;

    case 4:
        if (mGenericTimer != 0) {
            mSphere.OnAtSetBit();
        }
        calcHitGroundSpeed();
        if (mpMorfSO->isStop() && !speedF) {
            if (!checkPlayerSearch()) {
                setActionMode(ACTION_WAIT, 0);
                break;
            }
            setActionMode(ACTION_CHASE, 0);
        }
        break;
    }
}

void daE_TT_c::setDamageInit() {
    mMode = 5;
    current.angle.y = mAtInfo.mHitDirection.y + 0x8000;
    speedF = 15.0f;
}

void daE_TT_c::executeDamage() {
    switch (mMode) {
    case 0:
        mSound.startCreatureVoice(Z2SE_EN_TT_V_DAMAGE, -1);
        setBck(7, 0, 3.0f, 1.0f);
        setDamageInit();
        break;

    case 1:
        mSound.startCreatureVoice(Z2SE_EN_TT_V_DAMAGE, -1);
        setBck(8, 0, 3.0f, 1.0f);
        setDamageInit();
        break;

    case 2:
        mSound.startCreatureVoice(Z2SE_EN_TT_V_DAMAGE_ARROW, -1);
        setBck(6, 0, 3.0f, 1.0f);
        setDamageInit();
        break;

    case 3:
        mSound.startCreatureVoice(Z2SE_EN_TT_V_PUSH, -1);
        setBck(0xF, 0, 3.0f, 1.0f);
        setDamageInit();
        break;

    case 4:
        mSound.startCreatureVoice(Z2SE_EN_TT_V_WIND, -1);
        setBck(0x11, 0, 3.0f, 1.0f);
        setDamageInit();
        break;

    case 5:
        if (!mObjAcch.ChkGroundHit()) {
            break;
        }

    case 6:
        if (mTektiteOnWater) {
            if (speedF > 5.0f) {
                speedF = 5.0f;
            }
            cLib_chaseF(&speedF, 0.0f, 0.2f);
        } else {
            cLib_chaseF(&speedF, 0.0f, 2.0f);
        }
        if (mpMorfSO->isStop()) {
            speedF = 0.0f;
            if (!checkPlayerSearch()) {
                setActionMode(ACTION_WAIT, 0);
            } else {
                setActionMode(ACTION_CHASE, 0);
            }
        }
        break;
    }
}

void daE_TT_c::executeDeath() {
    switch (mMode) {
    case 0:
    case 1:
    case 2:
        mSphere.OffTgSetBit();
        mSphere.ClrTgHit();
        attention_info.flags = 0;
        current.angle.y = mAtInfo.mHitDirection.y + 0x8000;
        speedF = 30.0f;
        mSound.startCreatureVoice(Z2SE_EN_TT_V_EYE_DEATH, -1);
        if (mMode == 0) {
            setBck(9, 0, 3.0f, 1.0f);
            mMode = 3;
            setDeathEyeEffect();
            break;
        }
        if (mMode == 1) {
            mSound.startCreatureSound(Z2SE_EN_TT_LEG_DEATH, 0, -1);
            setBck(0xA, 0, 3.0f, 1.0f);
            mMode = 4;
            setDeathFootEffect();
            break;
        }
        setBck(9, 0, 3.0f, 1.0f);
        mMode = 5;
        break;

    case 3:
    case 4:
    case 5:
        if (mMode == 3) {
            setDeathEyeEffect();
        } else if (mMode == 4) {
            setDeathFootEffect();
        }

        if (mpMorfSO->checkFrame(18.0f)) {
            setBodyLandEffect();
        }

        if (mTektiteOnWater) {
            if (speedF > 10.0f) {
                speedF = 10.0f;
            }
            cLib_chaseF(&speedF, 0.0f, 0.2f);
        } else {
            cLib_chaseF(&speedF, 0.0f, 2.0f);
        }
        cLib_addCalc2(&mDeathColor, -20.0f, 1.0f, 0.4f);
        if (!mpMorfSO->isStop()) {
            break;
        }
        mMode = 6;
        mGenericTimer = 5;
        break;

    case 6:
        field_0x6fb = 1;
        cLib_addCalc2(&mDeathColor, -20.0f, 1.0f, 0.4f);
        if (mTektiteOnWater) {
            if (speedF > 10.0f) {
                speedF = 10.0f;
            }
            cLib_chaseF(&speedF, 0.0f, 0.2f);
        } else {
            cLib_chaseF(&speedF, 0.0f, 2.0f);
        }
        if (mGenericTimer == 0) {
            if (!mTektiteType) {
                fopAcM_createDisappear(this, &current.pos, 0xA, 0, 0x35);
            } else {
                fopAcM_createDisappear(this, &current.pos, 0xA, 0, 0x13);
            }
            fopAcM_delete(this);
            if (mSwitchFlag != 0xFF) {
                if (!dComIfGs_isSwitch(mSwitchFlag, fopAcM_GetRoomNo(this))) {
                    dComIfGs_onSwitch(mSwitchFlag, fopAcM_GetRoomNo(this));
                }
            }
        }
    }
}

void daE_TT_c::executeOutRange() {
    switch (mMode) {
    case 0:
        setBck(0x10, 2, 3.0f, 1.0f);
        mMode = 1;

    case 1:
        if (mpMorfSO->checkFrame(0.0f)) {
            mSound.startCreatureVoice(Z2SE_EN_TT_V_WAIT, -1);
        }
        cLib_addCalcAngleS(&shape_angle.y, fopAcM_searchPlayerAngleY(this), 0x8, 0x800, 0x100);
        current.angle.y = shape_angle.y;
        if (!fopAcM_CheckCondition(this, fopAcCnd_NODRAW_e) &&
            abs((s16)(shape_angle.y - cLib_targetAngleY(&current.pos, &home.pos))) < 0x2000)
        {
            setActionMode(ACTION_CHASE, 0);
            break;
        }
        if (fopAcM_searchPlayerDistanceXZ(this) > 2000.0f) {
            setActionMode(ACTION_WAIT, 0);
            break;
        }
        if (fopAcM_searchPlayerDistanceXZ(this) < 500.0f &&
            !fopAcM_CheckCondition(this, fopAcCnd_NODRAW_e) &&
            (s16)abs((s16)(daPy_getPlayerActorClass()->shape_angle.y -
                           fopAcM_searchPlayerAngleY(this))) > l_HIO.attack_angle &&
            m_attack_tt == NULL)
        {
            setActionMode(ACTION_ATTACK, 0);
            m_attack_tt = this;
        }
        break;
    }
}

void daE_TT_c::executeFirstAttack() {
    daPy_py_c* player = daPy_getPlayerActorClass();
    cXyz playerPos(player->current.pos);

    switch (mMode) {
    case 0:
        gravity = -5.0f;
        setBck(0xE, 2, 3.0f, 1.0f);
        mMode = 1;

    case 1:
        if (mpMorfSO->checkFrame(0.0f)) {
            mSound.startCreatureVoice(Z2SE_EN_TT_V_WAIT, -1);
        }
        cLib_addCalcAngleS(&shape_angle.y, fopAcM_searchPlayerAngleY(this), 0x8, 0x800, 0x100);
        current.angle.y = shape_angle.y;
        if (!fopAcM_CheckCondition(this, fopAcCnd_NODRAW_e)) {
            f32 modifiedPlayerSpeed = player->getSpeedF() * 40.0f + 500.0f;
            if (fopAcM_searchPlayerDistanceXZ(this) < modifiedPlayerSpeed) {
                if ((s16)abs((s16)(player->shape_angle.y - fopAcM_searchPlayerAngleY(this))) >
                    l_HIO.attack_angle)
                {
                    mSound.startCreatureVoice(Z2SE_EN_TT_V_JUMP, -1);
                    setBck(3, 0, 3.0f, 1.0f);
                    mMode = 11;
                    gravity = -10.0f;
                    if (player->checkHorseRide()) {
                        mPlayerOnHorse = true;
                    } else {
                        mPlayerOnHorse = false;
                    }
                }
            }
        }
        break;

    case 11: {
        f32 playerSpeedF = player->getSpeedF() * 17.0f;
        cXyz xyz(playerSpeedF * cM_ssin(player->shape_angle.y), 0.0f,
                 playerSpeedF * cM_scos(player->shape_angle.y));
        xyz += playerPos;
        cLib_addCalcAngleS(&shape_angle.y, cLib_targetAngleY(&current.pos, &xyz), 0x8, 0x800,
                           0x100);
        if (mpMorfSO->checkFrame(35.0f)) {
            mSound.startCreatureSound(Z2SE_EN_TT_JUMP, 0, -1);
        }
        if (mpMorfSO->isStop()) {
            setBck(0xC, 0, 3.0f, 1.0f);
            mMode = 12;
            f32 ttSpeed;
            if (!mPlayerOnHorse) {
                speed.y = 65.0f;
                ttSpeed = xyz.absXZ(current.pos) / 13.0f;
            } else {
                speed.y = 85.0f;
                ttSpeed = xyz.absXZ(current.pos) / 17.0f;
            }
            if (ttSpeed > 80.0f) {
                ttSpeed = 80.0f;
            }
            speedF = ttSpeed;
            current.angle.y = shape_angle.y;
        }
        break;
    }

    case 12:
        mSphere.OnAtSetBit();
        if (mObjAcch.ChkRoofHit()) {
            speed.y = 0.0f;
        }
        if (speed.y <= 0.0f) {
            setBck(0xD, 0, 5.0f, 0.0f);
            mMode = 13;
        }

    case 13:
        mSphere.OnAtSetBit();
        if (mObjAcch.ChkGroundHit()) {
            field_0x6fb = 1;
            mpMorfSO->setPlaySpeed(1.5f);
            mMode = 14;
            setWaterEffect();
            if (mTektiteOnWater) {
                mTransOffsetVelocity = -3.0f;
            }
            mGenericTimer = 5;
        }
        break;

    case 14:
        if (mGenericTimer != 0) {
            mSphere.OnAtSetBit();
        }
        calcHitGroundSpeed();
        if (mpMorfSO->isStop() && !speedF) {
            if (!checkPlayerSearch()) {
                setActionMode(ACTION_WAIT, 0);
            } else {
                setActionMode(ACTION_CHASE, 0);
            }
        }
        break;
    }
}

void daE_TT_c::action() {
    checkWaterSurface();
    if (!field_0x6f7) {
        checkFootGround();
    }
    damage_check();
    mSphere.OffAtSetBit();

    BOOL linkSearch = false;
    switch (mAction) {
    case ACTION_WAIT:
        executeWait();
        break;

    case ACTION_CHASE:
        executeChase();
        linkSearch = true;
        break;

    case ACTION_ATTACK:
        executeAttack();
        linkSearch = true;
        break;

    case ACTION_DAMAGE:
        executeDamage();
        break;

    case ACTION_DEATH:
        executeDeath();
        break;

    case ACTION_OUT_RANGE:
        executeOutRange();
        break;

    case ACTION_FIRST_ATTACK:
        executeFirstAttack();
        break;
    }

    mSound.setLinkSearch(linkSearch);
    if (mAction != ACTION_DEATH && !fopAcM_CheckCondition(this, fopAcCnd_NODRAW_e)) {
        dBgS_LinChk linChk;
        linChk.Set(&dComIfGp_getCamera(0)->lookat.eye, &attention_info.position, this);
        if (dComIfG_Bgsp().LineCross(&linChk)) {
            attention_info.flags = 0;
        } else {
            attention_info.flags = fopAc_AttnFlag_BATTLE_e;
        }
    }
    cLib_chaseF(&mTransOffsetVelocity, 3.0f, 0.2f);
    mTransOffset += mTransOffsetVelocity;
    if (mTransOffset > 0.0f) {
        mTransOffset = 0.0f;
        mTransOffsetVelocity = 0.0f;
    }
    fopAcM_posMoveF(this, mStts.GetCCMoveP());
    mObjAcch.CrrPos(dComIfG_Bgsp());
    mpMorfSO->play(0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
}

void daE_TT_c::mtx_set(int param_0) {
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y + mTransOffset, current.pos.z);
    mDoMtx_stack_c::ZXYrotM(shape_angle);
    mDoMtx_stack_c::scaleM(l_HIO.model_size, l_HIO.model_size, l_HIO.model_size);
    mpMorfSO->getModel()->setBaseTRMtx(mDoMtx_stack_c::get());
    if (param_0 != 0 || !field_0x6f7 || !fopAcM_CheckCondition(this, fopAcCnd_NODRAW_e) ||
        !(fopAcM_searchPlayerDistance(this) > 3000.0f))
    {
        mpMorfSO->modelCalc();
    }
}

void daE_TT_c::cc_set() {
    cXyz sphereC;
    J3DModel* model = mpMorfSO->getModel();

    mDoMtx_stack_c::copy(model->getAnmMtx(9));
    mDoMtx_stack_c::transM(20.0f, 30.0f, 0.0f);
    mDoMtx_stack_c::multVecZero(&eyePos);
    attention_info.position = eyePos;
    attention_info.position.y += 30.0f;

    if (!fopAcM_CheckCondition(this, fopAcCnd_NODRAW_e)) {
        mDoMtx_stack_c::copy(model->getAnmMtx(0));
        mDoMtx_stack_c::transM(40.0f, 20.0f, 0.0f);
        mDoMtx_stack_c::multVecZero(&sphereC);
        mSphere.SetC(sphereC);
        if (mAction == ACTION_FIRST_ATTACK) {
            mSphere.SetR(80.0f);
        } else {
            mSphere.SetR(55.0f);
        }
        dComIfG_Ccsp()->Set(&mSphere);
    }
}

int daE_TT_c::execute() {
    if (mGenericTimer != 0) {
        mGenericTimer--;
    }

    if (mAttackTimer != 0) {
        mAttackTimer--;
    }

    if (mDamageCooldownTimer != 0) {
        mDamageCooldownTimer--;
    }

    if (mPlayerCutTimer != 0) {
        mPlayerCutTimer--;
    }

    action();
    mtx_set(0);
    cc_set();

    cXyz effPos;
    mDoMtx_stack_c::copy(mpMorfSO->getModel()->getAnmMtx(0));
    mDoMtx_stack_c::transM(0.0f, 20.0f, 0.0f);
    mDoMtx_stack_c::multVecZero(&effPos);
    cXyz effSize(1.0f, 1.0f, 1.0f);
    setMidnaBindEffect(this, &mSound, &effPos, &effSize);

    return 1;
}

static int daE_TT_Execute(daE_TT_c* i_this) {
    return i_this->execute();
}

static int daE_TT_IsDelete(daE_TT_c* i_this) {
    return 1;
}

int daE_TT_c::_delete() {
    if (m_attack_tt == this) {
        m_attack_tt = NULL;
    }

    dComIfG_resDelete(&mPhaseReq, mpResName);
    dComIfG_resDelete(&mPhaseReq2, "E_TT");

    if (mHIOInit) {
        hio_set = false;
        mDoHIO_DELETE_CHILD(l_HIO.id);
    }

    if (heap != NULL) {
        mSound.deleteObject();
    }

    return 1;
}

static int daE_TT_Delete(daE_TT_c* i_this) {
    return i_this->_delete();
}

int daE_TT_c::CreateHeap() {
    J3DModelData* modelData;

    if (!mTektiteType) {
        modelData = (J3DModelData*)dComIfG_getObjectRes(mpResName, 3);
    } else {
        modelData = (J3DModelData*)dComIfG_getObjectRes(mpResName, 3);
    }

    JUT_ASSERT(1881, modelData != NULL);

    mpMorfSO = new mDoExt_McaMorfSO(modelData, NULL, NULL,
                                    (J3DAnmTransform*)dComIfG_getObjectRes("E_TT", 0xE), 0, 1.0f, 0,
                                    -1, &mSound, 0x80000, 0x11000084);
    if (mpMorfSO == NULL || mpMorfSO->getModel() == NULL) {
        return 0;
    }

    if (!field_0x6f7) {
        J3DModel* morfModel = mpMorfSO->getModel();
        morfModel->setUserArea((uintptr_t)this);
        for (u16 i = 1; i < morfModel->getModelData()->getJointNum(); i++) {
            morfModel->getModelData()->getJointNodePointer(i)->setCallBack(JointCallBack);
        }
    }
    return 1;
}

static int useHeapInit(fopAc_ac_c* i_this) {
    return static_cast<daE_TT_c*>(i_this)->CreateHeap();
}

int daE_TT_c::create() {
    fopAcM_ct(this, daE_TT_c);

    mSwitchFlag = fopAcM_GetParam(this);

    if (mSwitchFlag != 0xFF && dComIfGs_isSwitch(mSwitchFlag, fopAcM_GetRoomNo(this))) {
        // "E_TT I've been defeated so I won't reset it\n"
        OS_REPORT("E_TT やられ後なので再セットしません\n");
        return cPhs_ERROR_e;
    }

    mTektiteType = fopAcM_GetParam(this) >> 0x8;

    if (mTektiteType == 0xFF) {
        mTektiteType = 0;
    }
    if (mTektiteType == 0) {
        mpResName = "E_TTR";
    } else {
        mpResName = "E_TTB";
    }

    field_0x6f7 = fopAcM_GetParam(this) >> 0x10;

    if (field_0x6f7 != 1) {
        field_0x6f7 = 0;
    }

    u32 param = fopAcM_GetParam(this) >> 0x18;
    if (param == 0xFF) {
        field_0x6d4 = 0.0f;
    } else {
        field_0x6d4 = param * 100.0f;
    }

    int phase = dComIfG_resLoad(&mPhaseReq, mpResName);
    if (phase == cPhs_COMPLEATE_e) {
        phase = dComIfG_resLoad(&mPhaseReq2, "E_TT");
        if (phase == cPhs_COMPLEATE_e) {
            OS_REPORT("E_TT PARAM %x\n", fopAcM_GetParam(this));

            if (!fopAcM_entrySolidHeap(this, useHeapInit, 0x1600)) {
                return cPhs_ERROR_e;
            }

            if (!hio_set) {
                hio_set = true;
                mHIOInit = true;
                l_HIO.id = mDoHIO_CREATE_CHILD("テクタイト", &l_HIO);  // Tektite
            }

            attention_info.flags = 4;
            fopAcM_SetMtx(this, mpMorfSO->getModel()->getBaseTRMtx());
            fopAcM_SetMin(this, -200.0f, -200.0f, -200.0f);
            fopAcM_SetMax(this, 200.0f, 200.0f, 200.0f);
            mObjAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1,
                         &mAcchCir, fopAcM_GetSpeed_p(this), NULL, NULL);
            mAcchCir.SetWall(20.0f, 100.0f);
            mObjAcch.SetRoofCrrHeight(120.0f);
            mObjAcch.OnLineCheck();
            health = 0x50;
            field_0x560 = 0x50;
            mStts.Init(100, 0, this);
            mSphere.Set(cc_tt_src);
            mSphere.SetStts(&mStts);
            mSound.init(&current.pos, &eyePos, 3, 1);
            mSound.setEnemyName("E_tt");
            mAtInfo.mpSound = &mSound;
            mAtInfo.mPowerType = 1;
            gravity = -5.0f;

            if (!field_0x6f7) {
                setActionMode(ACTION_WAIT, 0);
            } else {
                setActionMode(ACTION_FIRST_ATTACK, 0);
            }
            mtx_set(1);
        }
    }
    return phase;
}

static int daE_TT_Create(daE_TT_c* i_this) {
    return i_this->create();
}

static actor_method_class l_daE_TT_Method = {
    (process_method_func)daE_TT_Create,  (process_method_func)daE_TT_Delete,
    (process_method_func)daE_TT_Execute, (process_method_func)daE_TT_IsDelete,
    (process_method_func)daE_TT_Draw,
};

actor_process_profile_definition g_profile_E_TT = {
    fpcLy_CURRENT_e,         // mLayerID
    7,                       // mListID
    fpcPi_CURRENT_e,         // mListPrio
    PROC_E_TT,               // mProcName
    &g_fpcLf_Method.base,    // sub_method
    sizeof(daE_TT_c),        // mSize
    0,                       // mSizeOther
    0,                       // mParameters
    &g_fopAc_Method.base,    // sub_method
    208,                     // mPriority
    &l_daE_TT_Method,        // sub_method
    0x00040100,              // mStatus
    fopAc_ENEMY_e,           // mActorType
    fopAc_CULLBOX_CUSTOM_e,  // cullType
};

AUDIO_INSTANCES;
