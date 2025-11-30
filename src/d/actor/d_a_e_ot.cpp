/**
 * @file d_a_e_ot.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_e_ot.h"
#include "Z2AudioLib/Z2Instances.h"
#include "f_op/f_op_actor_enemy.h"

class daE_OT_HIO_c {
public:
    daE_OT_HIO_c();
    virtual ~daE_OT_HIO_c() {}

    /* 0x04 */ s8 field_0x4;
    /* 0x08 */ f32 mAttackSpeed;
    /* 0x0C */ f32 mHeartDropRate;
    /* 0x10 */ f32 field_0x10;
    /* 0x14 */ f32 field_0x14;
    /* 0x18 */ f32 field_0x18;
    /* 0x1C */ f32 field_0x1c;
};

enum Animation {
    /* 0x6 */ ANM_BORN = 6,
    /* 0x7 */ ANM_DAMAGE,
    /* 0x8 */ ANM_SWIM,
};

namespace {

static f32 rand_speed[4] = {-4.0f, -2.0f, 2.0f, 4.0f};

static s16 rand_angle[4] = {-0x800, 0x400, -0x400, 0x800};

static s16 rand_angle2[4] = {-0x3000, 0x3000, -0x5000, 0x5000};

static Vec egg_pos[20] = {
    {0.0f, 130.0f, 0.0f},
    {15.0f, 60.0f, 150.0f},
    {70.0f, 140.0f, -10.0f},
    {30.0f, 130.0f, -55.0f},
    {-75.0f, 90.0f, 55.0f},
    {80.0f, 80.0f, 140.0f},
    {-40.0f, 55.0f, 130.0f},
    {30.0f, 100.0f, 110.0f},
    {-40.0f, 120.0f, 20.0f},
    {15.0f, 70.0f, -150.0f},
    {-55.0f, 65.0f, -110.0f},
    {-100.0f, 95.0f, -10.0f},
    {30.0f, 130.0f, 50.0f},
    {-30.0f, 100.0f, 70.0f},
    {0.0f, 100.0f, -100.0f},
    {-70.0f, 90.0f, -60.0f},
    {80.0f, 135.0f, -65.0f},
    {70.0f, 90.0f, -135.0f},
    {70.0f, 125.0f, 80.0f},
    {-30.0f, 120.0f, -40.0f},
};

static dCcD_SrcSph cc_ot_src = {
    {
        {0x0, {{AT_TYPE_CSTATUE_SWING, 0x1, 0x10}, {0xd8fafdff, 0x3}, 0x74}}, // mObj
        {dCcD_SE_SOFT_BODY, 0x0, 0x0, 0x0, 0x0}, // mGObjAt
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x2}, // mGObjTg
        {0x0}, // mGObjCo
    }, // mObjInf
    {
        {{0.0f, 0.0f, 0.0f}, 40.0f} // mSph
    } // mSphAttr
};

static dCcD_SrcSph cc_ot_at_src = {
    {
        {0x0, {{AT_TYPE_CSTATUE_SWING, 0x1, 0x0c}, {0x0, 0x0}, 0x0}}, // mObj
        {dCcD_SE_SOFT_BODY, 0x0, 0x0, 0x0, 0x0}, // mGObjAt
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x2}, // mGObjTg
        {0x0}, // mGObjCo
    }, // mObjInf
    {
        {{0.0f, 0.0f, 0.0f}, 40.0f} // mSph
    } // mSphAttr
};

}  // namespace

/* 8073D2AD 0003+00 data_8073D2AD None */
static bool hioInit;

static daE_OT_HIO_c l_HIO;

/* 8073D2DC 0001+00 data_8073D2DC None */
/* 8073D2DD 0003+00 data_8073D2DD None */
static u8 data_8073D2DC;
static u8 data_8073D2DD;

daE_OT_HIO_c::daE_OT_HIO_c() {
    field_0x4 = -1;
    mAttackSpeed = 13.0f;
    mHeartDropRate = 0.1f;
    field_0x10 = 0.0f;
    field_0x14 = 0.0f;
    field_0x18 = 0.0f;
    field_0x1c = 20.0f;
}

int daE_OT_c::draw() {
    J3DModel* model;
    if (mHatched) {
        model = mpMorf->getModel();
    } else {
        model = mpEggModel;
    }

    g_env_light.settingTevStruct(0, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(model, &tevStr);
    
    if (mHatched) {
        mpMorf->entryDL();
    } else {
        mpEggAnm->entry(model->getModelData());
        fopAcM_setEffectMtx(this, model->getModelData());
        mDoExt_modelUpdateDL(model);
    }

    cXyz pos;
    pos.set(current.pos.x, current.pos.y + 100.0f, current.pos.z);
    mShadowKey = dComIfGd_setShadow(mShadowKey, 1, model, &pos, 300.0f, 0.0f, current.pos.y,
                                    mAcch.GetGroundH(), mAcch.m_gnd, &tevStr, 0, 1.0f,
                                    dDlst_shadowControl_c::getSimpleTex());
    
    return 1;
}

static int daE_OT_Draw(daE_OT_c* i_this) {
    return i_this->draw();
}

void daE_OT_c::setBck(int i_anm, u8 i_attr, f32 i_morf, f32 i_rate) {
    J3DAnmTransform* bck = (J3DAnmTransform*)dComIfG_getObjectRes("E_OT", i_anm);
    mpMorf->setAnm(bck, i_attr, i_morf, i_rate, 0.0f, -1.0f);
}

void daE_OT_c::setActionMode(int i_action, int i_mode) {
    if (mAction != i_action) {
        mAction = i_action;
        mMode = i_mode;
    }
}

void daE_OT_c::damage_check() {
    if (mAction == ACT_DAMAGE) {
        return;
    }

    mCcStts.Move();

    if (mAtSph.ChkAtShieldHit()) {
        mAtSph.OffAtShieldHit();
        setActionMode(ACT_PANIC, 1);
    }

    if (mCcSph.ChkCoHit()) {
        fopAc_ac_c* hit_actor = dCc_GetAc(mCcSph.GetCoHitObj()->GetAc());
        if (fopAcM_GetName(dCc_GetAc(hit_actor)) == PROC_E_DT
            && static_cast<daE_DT_c*>(hit_actor)->isFlyingAttack())
        {
            setActionMode(ACT_DAMAGE, 10);
            return;
        }
    }

    if (mpToadActor->isFlyingAttackLand() && mpToadActor->current.pos.absXZ(current.pos) < 500.0f) {
        setActionMode(ACT_DAMAGE, 10);
        return;
    }

    if (mCcSph.ChkTgHit()) {
        mAtInfo.mpCollider = mCcSph.GetTgHitObj();
        fopAc_ac_c* ac = dCc_GetAc(mAtInfo.mpCollider->GetAc());
        cc_at_check(this, &mAtInfo);
        dScnPly_c::setPauseTimer(0);
        if (mAtInfo.mpCollider->ChkAtType(AT_TYPE_SHIELD_ATTACK)) {
            setActionMode(ACT_PANIC, 0);
        } else if (mAtInfo.mpCollider->ChkAtType(AT_TYPE_BOOMERANG)) {
            mCcSph.SetTgType(0xd8fafdff);
            setActionMode(ACT_PANIC, 5);
        } else {
            setActionMode(ACT_DAMAGE, 0);
        }
    }
}

void daE_OT_c::checkWaterSurface() {
    dBgS_ObjGndChk_Spl wtr_chk;
    Vec pos;
    pos = current.pos;
    pos.y += 100.0f;
    wtr_chk.SetPos(&pos);
    mWaterSurface = dComIfG_Bgsp().GroundCross(&wtr_chk);
}

void daE_OT_c::setWaterEffect() {
    cXyz pos(current.pos.x, mWaterSurface, current.pos.z);
    static cXyz sc(0.5f, 0.5f, 0.5f);
    static u16 w_eff_id[4] = {0x1b8, 0x1b9, 0x1ba, 0x1bb};
    for (int i = 0; i < 4; i++) {
        mWaterEffectKey[i] = dComIfGp_particle_set(mWaterEffectKey[i], w_eff_id[i], &pos, &tevStr,
                                                   NULL, &sc, 0xff, NULL, -1, NULL, NULL, NULL);
        JPABaseEmitter* emitter = dComIfGp_particle_getEmitter(mWaterEffectKey[i]);
        if (emitter != NULL) {
            emitter->setRateStep(2);
        }
    }
    mSound.startCreatureSound(Z2SE_CM_BODYFALL_ASASE_S, 0, -1);
}

void daE_OT_c::setEggBreakEffect() {
    static u16 w_eff_id[2] = {0x8550, 0x8551};
    for (int i = 0; i < 2; i++) {
        dComIfGp_particle_set(w_eff_id[i], &current.pos, &tevStr, NULL, NULL);
    }
}

void daE_OT_c::setDeathLightEffect() {
    static u16 w_eff_id[4] = {0x854c, 0x854d, 0x854e, 0x854f};
    for (int i = 0; i < 4; i++) {
        dComIfGp_particle_set(w_eff_id[i], &current.pos, &tevStr, NULL, NULL);
    }
    fopAcM_seStart(this, JA_SE_CM_MONS_EXPLODE, 0);
}

void daE_OT_c::setEggStay() {
    if (mpToadActor != NULL) {
        cXyz pos;
        mDoMtx_stack_c::copy(mpToadActor->getBackMtx());
        pos = mEggPos;
        pos.y += (mScale - 1.0f) * 15.0f + 20.0f;
        mDoMtx_stack_c::transM(pos);
        mDoMtx_stack_c::multVecZero(&pos);
        current.pos = pos;
    }
}

void daE_OT_c::executeEgg() {
    switch (mMode) {
    case 0:
    case 1:
    case 2:
        mCcSph.OnAtSetBit();
        setEggStay();
        if (mMode == 0) {
            if (cLib_chaseF(&mScale, 0.4f, 0.01f)) {
                mMode++;
            }
        } else if (mMode == 1) {
            if (cLib_chaseF(&mScale, 1.4f, 0.1f)) {
                mMode = 2;
            }
        } else {
            cLib_chaseF(&mScale, 1.0f, 0.05f);
        }
        break;

    case 4:
    case 6:
    case 8:
        mCcSph.OnCoSetBit();
        mScale = 1.0f;

        if (mMode == 8) {
            speedF = 5.0f;
            speed.y = 0.0f;
            current.angle.y = mpToadActor->shape_angle.y + -0x8000 + cM_rndFX(0x1000);
        } else if (mDemoMode == 3 && fopAcM_GetID(this) == mpToadActor->getDemoId()) {
            speedF = 20.0f;
            speed.y = 50.0f;
            current.angle.y += 0x4000;
        } else {
            speedF = 30.0f + cM_rndFX(10.0f);
            speed.y = 50.0f + cM_rndFX(10.0f);
            current.angle.y = mpToadActor->shape_angle.y;
            if (mChildNo < 5 || (mChildNo >= 10 && mChildNo < 15)) {
                current.angle.y += (s16)(0x4000 + cM_rndFX(0x1000));
            } else {
                current.angle.y += (s16)(-0x4000 + cM_rndFX(0x1000));
            }
        }

        mMode++;
        gravity = -5.0f;
        mCcSph.OffAtSetBit();
        attention_info.flags = 0;
        // fallthrough

    case 5:
    case 7:
    case 9:
        if (mMode == 9) {
            cLib_chaseF(&speedF, 0.0f, 0.5f);
        }

        if (mDemoMode != 0 && mpToadActor->isDemoJump()) {
            fopAcM_delete(this);
        } else {
            if (mAcch.ChkWallHit()) {
                speedF *= 0.3f;
                current.angle.y += 0x8000;
            }

            if (mAcch.ChkGroundHit() || mWaterSurface >= current.pos.y) {
                setEggBreakEffect();
                if (mWaterSurface >= current.pos.y) {
                    setWaterEffect();
                }
                attention_info.flags = fopAc_AttnFlag_BATTLE_e;
                if (mMode == 5) {
                    setActionMode(ACT_BORN, 0);
                } else {
                    fopAcM_delete(this);
                }
            }
        }
        break;
    }
}

void daE_OT_c::executeBorn() {
    if (mDemoMode != 0 && mpToadActor->isDemoJump()) {
        fopAcM_delete(this);
        return;
    }

    switch (mMode) {
    case 0:
        speedF = 0.0f;
        mHatched = true;
        mMode = 1;
        setBck(ANM_BORN, J3DFrameCtrl::EMode_NONE, 3.0f, 1.0f);
        mMode = 1;
        mSound.startCreatureSound(Z2SE_EN_OT_BORN, 0, -1);
        if (mDemoMode == 3) {
            shape_angle.y = current.angle.y = 0;
        } else {
            shape_angle.y = current.angle.y = cM_rndFX(0x8000);
        }
        mCcSph.SetTgType(0xd8fbfdff);
        break;

    case 1:
        if (mpMorf->isStop()) {
            setActionMode(ACT_ATTACK, 0);
        }
        break;
    }
}

void daE_OT_c::executeAttack() {
    if (mDemoMode != 0 && mpToadActor->isDemoJump()) {
        fopAcM_delete(this);
        return;
    }

    if (mpToadActor->isFlyingAttack() || mpToadActor->isDead()) {
        mAtSph.OffAtSetBit();
        attention_info.flags = 0;
    } else {
        mAtSph.OnAtSetBit();
    }

    switch (mMode) {
    case 0:
        setBck(ANM_SWIM, J3DFrameCtrl::EMode_LOOP, 3.0f, 1.0f);
        // fallthrough

    case 10:
        mMode = 1;
        mTimer1 = 0;
        mTimer2 = 180.0f + cM_rndFX(30.0f);
        // fallthrough

    case 1:
        if (mTimer2 == 0) {
            mTimer2 = 180.0f + cM_rndFX(30.0f);
        }

        if (mpMorf->checkFrame(0.0f)) {
            mSound.startCreatureSound(Z2SE_EN_OT_SWIM, 0, -1);
        }

        cLib_chaseF(&speedF, l_HIO.mAttackSpeed + rand_speed[mChildNo & 3], 2.0f);
        mpMorf->setPlaySpeed(speedF / 13.0f);

        if (mpToadActor->isDead()) {
            if (mTimer1 == 0) {
                mTargetAngleY = fopAcM_searchPlayerAngleY(this) + 0x8000 + cM_rndFX(0x4000);
                mTimer1 = 30;
            }
            cLib_addCalcAngleS(&shape_angle.y, mTargetAngleY, 0x10, 0x800, 0x100);
        } else if (mTimer2 > 30 && (daPy_getPlayerActorClass()->getDamageWaitTimer() == 0
                                        || fopAcM_searchPlayerDistance(this) > 500.0f)) {
            if (mDemoMode == 1) {
                s16 target_angle = fopAcM_searchPlayerAngleY(this);
                cLib_addCalcAngleS(&shape_angle.y, target_angle, 0xc, 0x800, 0x100);
            } else {
                s16 target_angle = fopAcM_searchPlayerAngleY(this)
                                        + rand_angle[mChildNo >> 2 & 3];
                cLib_addCalcAngleS(&shape_angle.y, target_angle, 0xc, 0x800, 0x100);
            }
        } else {
            s16 target_angle = fopAcM_searchPlayerAngleY(this)
                                    + rand_angle2[mChildNo >> 2 & 3];
            cLib_addCalcAngleS(&shape_angle.y, target_angle, 0x10, 0x800, 0x100);
        }
        break;
    }

    current.angle.y = shape_angle.y;
}

void daE_OT_c::executePanic() {
    switch (mMode) {
    case 0:
    case 1:
        if (mMode == 0) {
            speed.y = 30.0f + cM_rndFX(5.0f);
            speedF = 20.0f + cM_rndFX(5.0f);
            current.angle.y = daPy_getPlayerActorClass()->shape_angle.y + cM_rndFX(0x2000);
        } else {
            speed.y = 25.0f + cM_rndFX(5.0f);
            speedF += cM_rndF(3.0f);
            current.angle.y += 0x8000 + cM_rndFX(0x2000);
        }
        gravity = -5.0f;
        mMode = 2;
        setBck(ANM_BORN, J3DFrameCtrl::EMode_NONE, 3.0f, 1.0f);
        break;

    case 2:
        if (mAcch.ChkGroundHit() || mWaterSurface >= current.pos.y) {
            if (mWaterSurface >= current.pos.y) {
                setWaterEffect();
            }
            mMode = 3;
        }
        break;

    case 3:
        if (mpMorf->isStop()) {
            speedF = 20.0f;
            mMode = 4;
            setBck(ANM_SWIM, J3DFrameCtrl::EMode_LOOP, 3.0f, 1.0f);
        }
        break;

    case 4:
        if (mpMorf->checkFrame(0.0f)) {
            mSound.startCreatureSound(Z2SE_EN_OT_SWIM, 0, -1);
        }
        cLib_addCalcAngleS(&shape_angle.y, fopAcM_searchPlayerAngleY(this)
                                            + rand_angle[mChildNo >> 2 & 3], 0x10, 0x800, 0x100);
        cLib_addCalcAngleS(&current.angle.y, shape_angle.y + 0x8000, 0x10, 0x800, 0x100);
        if (cLib_chaseF(&speedF, 0.0f, 1.0f)) {
            setActionMode(ACT_ATTACK, 10);
        }
        break;

    case 5:
        mTimer1 = 11.0f + cM_rndFX(3.0f);
        mMode = 6;
        setBck(ANM_BORN, J3DFrameCtrl::EMode_NONE, 3.0f, 1.0f);
        gravity = 0.0f;
        speedF = 0.0f;
        // fallthrough

    case 6:
        speed.y += 1.5f;
        speedF = cM_rndF(5.0f);
        current.angle.y = cM_rndFX(0x8000);
        if (mTimer1 == 0) {
            gravity = -5.0f;
            mMode = 7;
        }
        break;

    case 7:
        if (mAcch.ChkGroundHit() || mWaterSurface >= current.pos.y) {
            if (mWaterSurface >= current.pos.y) {
                setWaterEffect();
            }
            mMode = 8;
            speedF = 0.0f;
        }
        break;

    case 8:
        if (mpMorf->isStop()) {
            mCcSph.SetTgType(0xd8fbfdff);
            setActionMode(ACT_ATTACK, 0);
        }
        break;
    }
}

void daE_OT_c::executeDamage() {
    cXyz pos, vec;

    switch (mMode) {
    case 0:
        attention_info.flags = 0;
        mAtSph.OffAtSetBit();
        mCcSph.OffTgSetBit();
        setBck(ANM_DAMAGE, J3DFrameCtrl::EMode_LOOP, 3.0f, 1.0f);
        mSound.startCreatureVoice(Z2SE_EN_OT_V_DEATH, -1);
        speedF = 20.0f + cM_rndFX(5.0f);
        speed.y = 40.0f + cM_rndFX(5.0f);
        mMode = 1;
        mAnmSpeed = 1.0f;
        gravity = -5.0f;
        current.angle.y = mAtInfo.mHitDirection.y + 0x8000 + cM_rndFX(0x2000);
        break;

    case 1:
        cLib_addCalcAngleS2(&shape_angle.x, 0x8000, 8, 0x400);
        if (mAcch.ChkGroundHit() || mWaterSurface >= current.pos.y) {
            if (mWaterSurface >= current.pos.y) {
                setWaterEffect();
            }
            if (mHatched) {
                setDeathLightEffect();
            }
            fopAcM_delete(this);
            if (cM_rnd() < l_HIO.mHeartDropRate) {
                fopAcM_createItem(&current.pos, fpcNm_ITEM_HEART, -1, -1, NULL, NULL, 0);
            }
        }
        break;

    case 2:
        if (mAcch.ChkGroundHit() || mWaterSurface >= current.pos.y) {
            speed.y = 0.0f;
            speedF = 0.0;
            mMode = 3;
            mTimer1 = 60;
        }
        break;

    case 3:
        cLib_chaseF(&mAnmSpeed, 0.0f, 0.02f);
        mpMorf->setPlaySpeed(mAnmSpeed);
        if (mTimer1 == 0) {
            if (mHatched) {
                setDeathLightEffect();
            }
            fopAcM_delete(this);
        }
        break;

    case 10:
        vec.set(0.0f, 0.0f, 160.0f);
        cLib_offsetPos(&pos, &mpToadActor->current.pos, mpToadActor->shape_angle.y, &vec);
        int angle = cLib_targetAngleY(&pos, &current.pos);
        vec.set(0.0f, 0.0f, 470.0f);
        cLib_offsetPos(&current.pos, &pos, angle, &vec);
        data_8073D2DC++;
        if (data_8073D2DC < 5) {
            setDeathLightEffect();
        }
        fopAcM_delete(this);
        if (cM_rnd() < l_HIO.mHeartDropRate) {
            fopAcM_createItem(&current.pos, fpcNm_ITEM_HEART, -1, -1, NULL, NULL, 0);
        }
        break;
    }
}

void daE_OT_c::action() {
    fopAcM_SearchByName(PROC_E_DT, (fopAc_ac_c**)&mpToadActor);

    if (mpToadActor == NULL || fpcM_IsCreating(fopAcM_GetID(mpToadActor))
        || mpToadActor->isDead() == 2)
    {
        fopAcM_delete(this);
        return;
    }

    if (mpToadActor->isFlyingAttack()) {
        if (data_8073D2DD == 0) {
            data_8073D2DC = 0;
            data_8073D2DD = 1;
        }
    } else {
        data_8073D2DD = 0;
    }
    
    damage_check();

    switch (mAction) {
    case ACT_EGG:
        executeEgg();
        break;
    case ACT_BORN:
        executeBorn();
        break;
    case ACT_ATTACK:
        executeAttack();
        break;
    case ACT_PANIC:
        executePanic();
        break;
    case ACT_DAMAGE:
        executeDamage();
        break;
    }

    fopAcM_posMoveF(this, mCcStts.GetCCMoveP());

    if (mWaterSurface >= current.pos.y) {
        speed.y = 0.0f;
        current.pos.y = mWaterSurface;
        cXyz pos(current.pos.x, mWaterSurface, current.pos.z);
        fopAcM_effHamonSet(&mRippleKey, &pos, 0.4f, 0.5f);
    }

    mAcch.CrrPos(dComIfG_Bgsp());

    mpMorf->play(0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
    if (!mHatched) {
        mpEggAnm->play();
    }
}

void daE_OT_c::mtx_set() {
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y - 20.0f, current.pos.z);
    mDoMtx_stack_c::ZXYrotM(shape_angle);
    mDoMtx_stack_c::scaleM(mScale, mScale, mScale);
    mpMorf->getModel()->setBaseTRMtx(mDoMtx_stack_c::get());
    mpMorf->modelCalc();
    mpEggModel->setBaseTRMtx(mDoMtx_stack_c::get());
}

void daE_OT_c::cc_set() {
    cXyz vec, center;
    J3DModel* model = mpMorf->getModel();

    mDoMtx_stack_c::copy(model->getAnmMtx(0));
    vec.set(30.0f, 0.0f, 0.0f);
    mDoMtx_stack_c::multVec(&vec, &eyePos);
    attention_info.position = eyePos;
    attention_info.position.y += 25.0f;

    mDoMtx_stack_c::copy(model->getAnmMtx(0));
    vec.set(0.0f, 20.0f, 0.0f);
    mDoMtx_stack_c::multVec(&vec, &center);
    mCcSph.SetC(center);
    mCcSph.SetR(50.0f);
    dComIfG_Ccsp()->Set(&mCcSph);

    mDoMtx_stack_c::copy(model->getAnmMtx(0));
    vec.set(40.0f, 20.0f, 0.0f);
    mDoMtx_stack_c::multVec(&vec, &center);
    mAtSph.SetC(center);
    mAtSph.SetR(30.0f);
    dComIfG_Ccsp()->Set(&mAtSph);
}

int daE_OT_c::execute() {
    if (mTimer1 != 0) {
        mTimer1--;
    }
    if (mTimer2 != 0) {
        mTimer2--;
    }

    action();
    mtx_set();
    cc_set();

    cXyz pos = current.pos;
    cXyz size(0.5f, 0.5f, 0.5f);
    setMidnaBindEffect(this, &mSound, &pos, &size);

    return 1;
}

static int daE_OT_Execute(daE_OT_c* i_this) {
    return i_this->execute();
}

static int daE_OT_IsDelete(daE_OT_c* i_this) {
    return 1;
}

int daE_OT_c::_delete() {
    dComIfG_resDelete(&mPhase, "E_OT");

    if (mHIOInit) {
        hioInit = false;
    }

    if (heap != NULL) {
        mSound.deleteObject();
    }

    return 1;
}

static int daE_OT_Delete(daE_OT_c* i_this) {
    return i_this->_delete();
}

int daE_OT_c::CreateHeap() {
    J3DModelData* model_data = (J3DModelData*)dComIfG_getObjectRes("E_OT", 14);
    mpMorf = new mDoExt_McaMorfSO(model_data, NULL, NULL,
                                  (J3DAnmTransform*)dComIfG_getObjectRes("E_OT", ANM_BORN),
                                  J3DFrameCtrl::EMode_NONE, 1.0f, 0, -1, &mSound,
                                  0x80000, 0x11000084);
    if (mpMorf == NULL || mpMorf->getModel() == NULL) {
        return 0;
    }

    model_data = (J3DModelData*)dComIfG_getObjectRes("E_OT", 11);
    mpEggModel = mDoExt_J3DModel__create(model_data, 0, 0x11000284);
    if (mpEggModel == NULL) {
        return 0;
    }

    mpEggAnm = new mDoExt_btkAnm();
    if (mpEggAnm == NULL) {
        return 0;
    }
    J3DAnmTextureSRTKey* btk = (J3DAnmTextureSRTKey*)dComIfG_getObjectRes("E_OT", 17);
    if (!mpEggAnm->init(mpEggModel->getModelData(), btk, TRUE,
                        J3DFrameCtrl::EMode_NONE, 1.0f, 0, -1)) {
        return 0;
    }

    return 1;
}

static int useHeapInit(fopAc_ac_c* i_this) {
    return static_cast<daE_OT_c*>(i_this)->CreateHeap();
}

cPhs__Step daE_OT_c::create() {
    fopAcM_ct(this, daE_OT_c);

    cPhs__Step step = (cPhs__Step)dComIfG_resLoad(&mPhase, "E_OT");
    if (step == cPhs_COMPLEATE_e) {
        if (!fopAcM_entrySolidHeap(this, useHeapInit, 0x1f40)) {
            return cPhs_ERROR_e;
        }

        if (!hioInit) {
            hioInit = true;
            mHIOInit = true;
            l_HIO.field_0x4 = -1;
        }

        attention_info.flags = fopAc_AttnFlag_BATTLE_e;
        fopAcM_SetMtx(this, mpMorf->getModel()->getBaseTRMtx());
        fopAcM_SetMin(this, -200.0f, -200.0f, -200.0f);
        fopAcM_SetMax(this, 200.0f, 200.0f, 200.0f);
        mAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1, &mAcchCir,
                  fopAcM_GetSpeed_p(this), NULL, NULL);
        mAcchCir.SetWall(10.0f, 60.0f);
        
        health = 10;
        field_0x560 = 10;

        mCcStts.Init(100, 0, this);
        mCcSph.Set(cc_ot_src);
        mCcSph.SetStts(&mCcStts);
        mAtSph.Set(cc_ot_at_src);
        mAtSph.SetStts(&mCcStts);
        
        mSound.init(&current.pos, &eyePos, 3, 1);
        mAtInfo.mpSound = &mSound;
        mAtInfo.mPowerType = 1;
        gravity = 0.0f;
        fopAcM_SearchByName(PROC_E_DT, (fopAc_ac_c**)&mpToadActor);

        mChildNo = fopAcM_GetParam(this) & 0xffff;
        mDemoMode = (fopAcM_GetParam(this) >> 0x10) & 0xff;
        if (mDemoMode == 0xff) {
            mDemoMode = 0;
        }

        switch (mDemoMode) {
        case 3:
            fopAcM_OnStatus(this, 0x8000);
            // fallthrough

        case 0:
            mEggPos.x = egg_pos[mChildNo].x;
            mEggPos.y = egg_pos[mChildNo].y;
            mEggPos.z = egg_pos[mChildNo].z;
            break;

        case 1:
            mScale = 1.0f;
            mMode = 5;
            mCcSph.OnCoSetBit();
            gravity = -5.0f;
            speedF = 15.0f;
            if (mpToadActor != NULL) {
                shape_angle.y = current.angle.y
                              = mpToadActor->shape_angle.y + -0x8000 + cM_rndFX(0x1000);
            }
            break;

        case 2:
            mScale = 1.0f;
            mMode = 7;
            mCcSph.OnCoSetBit();
            gravity = -5.0f;
            break;
        }

        shape_angle.x = shape_angle.z = 0;
        attention_info.distances[fopAc_attn_BATTLE_e] = 4;
        checkWaterSurface();
        daE_OT_Execute(this);
    }

    return step;
}

static cPhs__Step daE_OT_Create(daE_OT_c* i_this) {
    return i_this->create();
}

static actor_method_class l_daE_OT_Method = {
    (process_method_func)daE_OT_Create,
    (process_method_func)daE_OT_Delete,
    (process_method_func)daE_OT_Execute,
    (process_method_func)daE_OT_IsDelete,
    (process_method_func)daE_OT_Draw,
};

extern actor_process_profile_definition g_profile_E_OT = {
  fpcLy_CURRENT_e,        // mLayerID
  7,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_E_OT,              // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(daE_OT_c),       // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  203,                    // mPriority
  &l_daE_OT_Method,       // sub_method
  0x00050100,             // mStatus
  fopAc_ENEMY_e,          // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};

AUDIO_INSTANCES
