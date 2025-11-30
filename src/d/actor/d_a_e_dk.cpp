/**
 * @file d_a_e_dk.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_e_dk.h"
#include "d/actor/d_a_player.h"
#include "d/d_com_inf_game.h"
#include "d/d_s_play.h"
#include <cmath.h>

#define ACTION_MODE_WAIT 0
#define ACTION_MODE_CHASE 1
#define ACTION_MODE_ATTACK 2
#define ACTION_MODE_HURT 3
#define ACTION_MODE_DIE 4

enum E_DK_RES_FILE_ID {
    /* BCK */
    /* 0x06 */ BCK_DK_ATTACK = 6,
    /* 0x07 */ BCK_DK_CHARGE,
    /* 0x08 */ BCK_DK_C_ATTACK,
    /* 0x09 */ BCK_DK_C_CHARGE,
    /* 0x0A */ BCK_DK_C_DEAD,
    /* 0x0B */ BCK_DK_C_GUARD,
    /* 0x0C */ BCK_DK_C_WAIT,
    /* 0x0D */ BCK_DK_GUARD,
    /* 0x0E */ BCK_DK_SHELLDEAD,
    /* 0x0F */ BCK_DK_WAIT,

    /* BMDR */
    /* 0x12 */ BMDR_DK = 0x12,
    /* 0x13 */ BMDR_DK_CORE,

    /* BRK */
    /* 0x16 */ BRK_DK_ATTACK = 0x16,
    /* 0x17 */ BRK_DK_WAIT,

    /* BTK */
    /* 0x1A */ BTK_DK_ATTACK = 0x1A,
    /* 0x1B */ BTK_DK_WAIT,
};


static dCcD_SrcSph cc_dk_src = {
    { // mObjInf
        { 0x0, { { 0, 0, { 0 } }, { (s32)0xd8fbbddf, { 3 } }, { { 0x75 } } } }, // mObj
        { 0x09, 0, 0, 0, { 0 } }, // mGObjAt
        { 0x00, 0x00, 0x00, 0x00, { 5 } }, // mGObjTg
        { { 0 } }, // mGObjCo
    },
    { { { 0.0f, 0.0f, 0.0f }, 40.0f, } } // mSphAttr
};

static dCcD_SrcSph cc_dk_at_src = {
    { // mObjInf
        { 0x0, { { 0x400, 2, { 0xc } }, { 0, { 0 } }, { { 0 } } } }, // mObj
        { 0x09, 0, 0, 0, { 0 } }, // mGObjAt
        { 0x00, 0x00, 0x00, 0x00, { 2 } }, // mGObjTg
        { { 0 } }, // mGObjCo
    },
    { { { 0.0f, 0.0f, 0.0f }, 40.0f, } } // mSphAttr
};

static dCcD_SrcSph cc_dk_core_src = {
    { // mObjInf
        { 0x0, { { 0, 0, { 0 } }, { 0x4020, { 3 } }, { { 0 } } } }, // mObj
        { 0x09, 0, 0, 0, { 0 } }, // mGObjAt
        { 0x00, 0x00, 0x00, 0x00, { 2 } }, // mGObjTg
        { { 0 } }, // mGObjCo
    },
    { { { 0.0f, 0.0f, 0.0f }, 40.0f, } } // mSphAttr
};

daE_DK_HIO_c::daE_DK_HIO_c() {
    field_0x4 = -1;
    model_size = 2.0f;
    player_detection_range = 700.0f;
    field_0x10 = 1000.0f;
    first_attack_range = 1000.0f;
}

static u32 const dk_brk_name[2] = { BRK_DK_WAIT, BRK_DK_ATTACK };
static u32 const dk_btk_name[2] = { BTK_DK_WAIT, BTK_DK_ATTACK };

int daE_DK_c::draw() {
    J3DModel* model = mpMorfSO->getModel();
    J3DModel* modelCore = mpCoreMorfSO->getModel();

    g_env_light.settingTevStruct(0, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(model, &tevStr);
    g_env_light.setLightTevColorType_MAJI(modelCore, &tevStr);

    J3DModelData* modelData = model->getModelData();
    for (u16 i = 0; i < modelData->getMaterialNum(); i++) {
        J3DMaterial* matNodePtr = modelData->getMaterialNodePointer(i);
        if (i == 0) {
            J3DBlend* blend = matNodePtr->getBlend();
            if (this->field_0x692 != 0xff) {
                blend->setType(1);
                blend->setSrcFactor(4);
                blend->setDstFactor(5);
                matNodePtr->getZMode()->setUpdateEnable(0);
                matNodePtr->setZCompLoc(0);
            } else {
                blend->setType(0);
                blend->setSrcFactor(1);
                blend->setDstFactor(0);
                matNodePtr->getZMode()->setUpdateEnable(1);
                matNodePtr->setZCompLoc(1);
            }
            matNodePtr->getTevKColor(3)->a = (u8)field_0x692;
        } else {
            matNodePtr->getTevKColor(3)->a = (u8)field_0x691;
        }
    }

    mpBrkAnm[field_0x6a0]->entry(model->getModelData());
    mpBtkAnm[field_0x6a0]->entry(model->getModelData());

    mpMorfSO->entryDL();

    if (field_0x6a1 == 0) {
        mpCoreMorfSO->entryDL();
    }

    if (field_0x6a1 != 0) {
        return 1;
    }

    cXyz shadowPos;
    shadowPos.set(current.pos.x, current.pos.y + 100.0f, current.pos.z);

    mShadowKey2 = dComIfGd_setShadow(mShadowKey2, 1, modelCore, &shadowPos, 700.0f + nREG_F(19),
        0.0f, current.pos.y, this->mObjAcch.GetGroundH(), mObjAcch.m_gnd,
        &tevStr, 0, 1.0f, dDlst_shadowControl_c::getSimpleTex());

    if (field_0x6a2 == 0) {
        mShadowKey = dComIfGd_setShadow(mShadowKey, 1, model, &shadowPos, 700.0f + nREG_F(19),
            0.0f, current.pos.y, this->mObjAcch.GetGroundH(), mObjAcch.m_gnd,
            &tevStr, 0, 1.0f, dDlst_shadowControl_c::getSimpleTex());
    } else {
        shadowPos.set(field_0x670.x, field_0x670.y + 100.0f, field_0x670.z);

        mShadowKey = dComIfGd_setShadow(mShadowKey, 1, model, &shadowPos, 700.0f + nREG_F(19),
            0.0f, field_0x670.y, this->mObjAcch.GetGroundH(), mObjAcch.m_gnd,
            &tevStr, 0, 1.0f, dDlst_shadowControl_c::getSimpleTex());
    }

    return 1;
}

static int daE_DK_Draw(daE_DK_c* i_this) {
    return i_this->draw();
}

void daE_DK_c::setBck(int i_resIndex, u8 i_attr, f32 i_morf, f32 i_rate) {
    field_0x6a0 = 0;
    if (i_resIndex == BCK_DK_CHARGE || i_resIndex == BCK_DK_ATTACK) {
        field_0x6a0 = 1;
    }

    mpMorfSO->setAnm((J3DAnmTransform*)dComIfG_getObjectRes("E_DK", i_resIndex),
        i_attr, i_morf, i_rate, 0.0f, -1.0f);
}

void daE_DK_c::setBckCore(int i_resIndex, u8 i_attr, f32 i_morf, f32 i_rate) {
    mpCoreMorfSO->setAnm((J3DAnmTransform*)dComIfG_getObjectRes("E_DK", i_resIndex),
        i_attr, i_morf, i_rate, 0.0f, -1.0f);
}

void daE_DK_c::setActionMode(int i_actionMode, int i_moveMode) {
    mActionMode = i_actionMode;
    mMoveMode = i_moveMode;
}

void daE_DK_c::damage_check() {
    if (field_0x694 == 0 && field_0x6a2 == 0) {
        mStts.Move();
        if (mSphere.ChkTgHit() != 0) {
            field_0x694 = 10;

            mAtInfo.mpCollider = mSphere.GetTgHitObj();

            fopAc_ac_c* colliderActor = dCc_GetAc(mAtInfo.mpCollider->GetAc());
            f32 xDiff = current.pos.x - colliderActor->current.pos.x;
            f32 zDiff = current.pos.z - colliderActor->current.pos.z;

            mAtInfo.mHitDirection.y = cM_atan2s(-xDiff, -zDiff);
            if (field_0x6a3 == 0) {
                if (mAtInfo.mpCollider->ChkAtType(0x10) != 0) {
                    setActionMode(3, 1);
                } else {
                    setActionMode(3, 0);
                }
            }

            mSphere.ClrTgHit();
        }
        if (mCoreSphere.ChkTgHit() != 0) {
            mAtInfo.mpCollider = mCoreSphere.GetTgHitObj();

            cc_at_check(this, &mAtInfo);

            field_0x694 = 10;

            if (mAtInfo.mpCollider->ChkAtType(0x20) != 0) {
                setActionMode(4, 1);
            } else {
                if (field_0x6a3 == 0) {
                    setActionMode(4, 0);
                }
            }

            mCoreSphere.ClrTgHit();
        }
    }
}

static u8 data_806AD7F8;

static daE_DK_HIO_c l_HIO;

bool daE_DK_c::checkPlayerSearch() {
    if (current.pos.abs(home.pos) > field_0x6a8) {
        return 0;
    }

    if (daPy_getPlayerActorClass()->current.pos.abs(home.pos) > l_HIO.player_detection_range) {
        return 0;
    }

    if (fopAcM_otherBgCheck(this, daPy_getPlayerActorClass()) != 0) {
        return 0;
    }

    return 1;
}

int daE_DK_c::checkPlayerAttack(f32 param_0) {
    if (field_0x69c == 0) {
        f32 dist = fopAcM_searchPlayerDistance(this);
        if (field_0x690 == 0 && dist < l_HIO.first_attack_range) {
            if (dist > 8.0f) {
                setActionMode(2, 0);
                field_0x690 = 2;
                return 1;
            } else if (dist < l_HIO.first_attack_range - 100.0f) {
                field_0x690 = 1;
            }
        }

        if (param_0 > 30.0f && param_0 < 60.0f && dist < 400.0f) {
            setActionMode(2, 0);
            return 1;
        }
    }

    return 0;
}

void daE_DK_c::checkWaterHeight() {
    dBgS_ObjGndChk_Spl gndChk;
    Vec gndChkPos;
    if (mActionMode != ACTION_MODE_DIE) {
        gndChkPos = current.pos;
        gndChkPos.y += 1000.0f;
        gndChk.SetPos(&gndChkPos);
        field_0x6b8 = dComIfG_Bgsp().GroundCross(&gndChk);
        if (field_0x6b8 != -G_CM3D_F_INF && current.pos.y + 350.0f > field_0x6b8) {
            current.pos.y = field_0x6b8 - 350.0f;
        }
    } else {
        gndChkPos = field_0x670;
        gndChkPos.y += 1000.0f;
        gndChk.SetPos(&gndChkPos);
        field_0x6b8 = dComIfG_Bgsp().GroundCross(&gndChk);
        if (field_0x6b8 != -G_CM3D_F_INF && field_0x670.y + 350.0f > field_0x6b8) {
            field_0x670.y = field_0x6b8 - 350.0f;
        }
    }
}

void daE_DK_c::SphBgcCallBack(dBgS_SphChk* param_0, cBgD_Vtx_t* param_1, int param_2,
    int param_3, int param_4, cM3dGPla* param_5, void* param_6) {
    fopAc_ac_c* actor = (fopAc_ac_c*)param_6;
    f32 speedY = actor->speed.y;
    if (!speedY) {
        return;
    }

    s32 isOutsideRange = FALSE;
    f32 unkFloat1 = (param_1[param_2].y + param_1[param_3].y + param_1[param_4].y) / 3.0f;
    if (actor->speed.y < 0.0f) {
        if (actor->current.pos.y + 150.0f + nREG_F(18) > unkFloat1) {
            isOutsideRange = TRUE;
        }
    } else {
        if (actor->current.pos.y + 150.0f + nREG_F(18) < unkFloat1) {
            isOutsideRange = TRUE;
        }
    }

    if (isOutsideRange) {
        actor->speed.y = 0.0f;
        actor->current.pos.y = actor->old.pos.y;
    }
}

void daE_DK_c::setElectricEffect() {
    static u16 elect_effect_name[6] = {
        0x84BE, 0x84BF, 0x84C0, 0x84C1, 0x84C2, 0x84C3,
    };

    cXyz emitterPos(current.pos);
    emitterPos.y += 100.0f;
    cXyz emitterScale(l_HIO.model_size, l_HIO.model_size, l_HIO.model_size);
    csXyz emitterRot;

    JPABaseEmitter* emitter;

    u8 alpha = 0xff;
    if (dKy_camera_water_in_status_check() == 0) {
        alpha = 0x55;
    }

    for (s32 i = 0; i < 6; i++) {
        emitterRot.set(0, 0, 0);
        mElectricEffects[i] = dComIfGp_particle_set(mElectricEffects[i], elect_effect_name[i],
            &emitterPos, &tevStr, &emitterRot, &emitterScale, alpha, 0, -1, 0, 0, 0);
        if (i == 1 || i == 2 || i == 3) {
            emitter = dComIfGp_particle_getEmitter(mElectricEffects[i]);
            if (emitter != NULL) {
                emitterRot.x = cM_rndFX(32768.0f);
                emitterRot.y = cM_rndFX(32768.0f);
                emitterRot.z = cM_rndFX(32768.0f);
                emitter->setLocalRotation(*(JGeometry::TVec3<s16>*)&emitterRot);
            }
        }
    }

    mCreatureSound.startCreatureSoundLevel(Z2SE_EN_DK_ATTACK, 0, -1);
}

void daE_DK_c::setCoreDeadEffect() {
    cXyz emitterPos;
    cXyz emitterScale(l_HIO.model_size, l_HIO.model_size, l_HIO.model_size);
    csXyz emitterRot(0, 0, 0);

    u8 alpha = 0xff;
    if (dKy_camera_water_in_status_check() == 0) {
        alpha = 0x55;
    }

    J3DModel* model = mpCoreMorfSO->getModel();
    mDoMtx_stack_c::copy(model->getAnmMtx(1));
    mDoMtx_stack_c::multVecZero(&emitterPos);
    dComIfGp_particle_set(0x860c, &emitterPos, &tevStr, &emitterRot, &emitterScale, alpha, NULL,
                          -1, NULL, NULL, NULL);
}

void daE_DK_c::setBodyDeadEffect() {
    static u16 dead_effect_name[2] = {
        0x860D,
        0x860E,
    };

    cXyz emitterPos;
    cXyz emitterScale(l_HIO.model_size, l_HIO.model_size, l_HIO.model_size);
    csXyz emitterRot(0, 0, 0);

    u8 alpha = 0xff;
    if (dKy_camera_water_in_status_check() == 0) {
        alpha = 0x55;
    }

    for (s32 i = 0; i < 2; i++) {
        mDeadEffects[i] = dComIfGp_particle_set(mDeadEffects[i], dead_effect_name[i], &emitterPos, &tevStr,
                                            &emitterRot, &emitterScale, alpha, 0, -1, 0, 0, 0);

        JPABaseEmitter* emitter = dComIfGp_particle_getEmitter(mDeadEffects[i]);
        if (emitter != NULL) {
            emitter->setGlobalSRTMatrix(mpMorfSO->getModel()->getAnmMtx(1));
        }
    }
}

void daE_DK_c::executeWait() {
    cXyz dirFromHome;
    switch (mMoveMode) {
    case 0: {
        setBck(BCK_DK_WAIT, 2, 3.0f, 1.0f);
        setBckCore(BCK_DK_C_WAIT, 2, 3.0f, 1.0f);
        mMoveMode = 1;
        break;
    }
    case 1: {
        if (mpMorfSO->checkFrame(0.0f) != 0 || mpMorfSO->checkFrame(80.0f) != 0) {
            mCreatureSound.startCreatureSound(Z2SE_EN_DK_WAIT, 0, -1);
        }
        f32 fVar4 = mpMorfSO->getFrame();
        if (mpMorfSO->checkFrame(70.0f) != 0 || mpMorfSO->checkFrame(150.0f) != 0) {
            dirFromHome = current.pos - home.pos;
            if (std::abs(dirFromHome.y) < 100.0f) {
                field_0x6ac = 5.0f;
            } else {
                if (dirFromHome.y > 0.0f) {
                    field_0x6ac = 4.0f;
                } else {
                    field_0x6ac = 6.0f;
                }
            }
            if (dirFromHome.absXZ() < 200.0f) {
                field_0x6b0 = 0;
            } else {
                field_0x6b0 = 3.0f + nREG_F(3);
            }

            current.angle.y = cLib_targetAngleY(&current.pos, &home.pos);
        }
        if (fVar4 >= 80.0f) {
            fVar4 -= 80.0f;
        }

        if (70.0f <= fVar4 || fVar4 <= 9.0f) {
            cLib_chaseF(&speed.y, field_0x6ac, 1.0f);
        } else if (fVar4 < 30.0f) {
            cLib_chaseF(&speed.y, 0.0f, 0.2f);
        } else if (fVar4 < 60.0f) {
            cLib_chaseF(&speed.y, -5.0f, 0.3f);
        } else {
            cLib_chaseF(&speed.y, 0.0f, 0.2f);
        }

        if (fVar4 < 60.0f || fVar4 >= 70.0f) {
            cLib_chaseF(&speedF, field_0x6b0, 0.2f);
        } else {
            cLib_chaseF(&speedF, 0.0f, 0.2f);
        }

        cLib_addCalcAngleS(&shape_angle.y, current.angle.y, 0x10, 0x100, 0x10);

        if (checkPlayerSearch()) {
            setActionMode(1, 0);
        } else {
            checkPlayerAttack(fVar4);
        }
    }
    }
}

void daE_DK_c::executeChase() {
    cXyz dirFromHome;
    switch (mMoveMode) {
    case 0: {
        setBck(BCK_DK_WAIT, 2, 3.0f, 1.0f);
        setBckCore(BCK_DK_C_WAIT, 2, 3.0f, 1.0f);
        mMoveMode = 1;
        break;
    }
    case 1: {
        if (mpMorfSO->checkFrame(0.0f) != 0 || mpMorfSO->checkFrame(80.0f) != 0) {
            mCreatureSound.startCreatureSound(Z2SE_EN_DK_WAIT, 0, -1);
        }
        f32 fVar4 = mpMorfSO->getFrame();
        if (mpMorfSO->checkFrame(70.0f) != 0 || mpMorfSO->checkFrame(150.0f) != 0) {
            dirFromHome = current.pos - daPy_getPlayerActorClass()->current.pos;
            if (std::abs(dirFromHome.y) < 50.0f) {
                field_0x6ac = 5.0f;
            } else {
                if (dirFromHome.y > 0.0f) {
                    field_0x6ac = 4.0f;
                } else {
                    field_0x6ac = 6.0f;
                }
            }
            if (dirFromHome.absXZ() < 200.0f) {
                field_0x6b0 = 0;
            } else {
                field_0x6b0 = 3.0f + nREG_F(3);
            }

            current.angle.y = fopAcM_searchActorAngleY(this, daPy_getPlayerActorClass());
        }
        if (fVar4 >= 80.0f) {
            fVar4 -= 80.0f;
        }

        if (70.0f <= fVar4 || fVar4 <= 9.0f) {
            cLib_chaseF(&speed.y, field_0x6ac, 1.0f);
        } else if (fVar4 < 30.0f) {
            cLib_chaseF(&speed.y, 0.0f, 0.2f);
        } else if (fVar4 < 60.0f) {
            cLib_chaseF(&speed.y, -5.0f, 0.3f);
        } else {
            cLib_chaseF(&speed.y, 0.0f, 0.2f);
        }

        if (fVar4 < 60.0f || fVar4 >= 70.0f) {
            cLib_chaseF(&speedF, field_0x6b0, 0.2f);
        } else {
            cLib_chaseF(&speedF, 0.0f, 0.2f);
        }

        cLib_addCalcAngleS(&shape_angle.y, current.angle.y, 0x10, 0x100, 0x10);

        if (!checkPlayerSearch()) {
            setActionMode(0, 0);
        } else {
            checkPlayerAttack(fVar4);
        }
    }
    }
}

void daE_DK_c::executeAttack() {
    switch (mMoveMode) {
    case 0: {
        setBck(BCK_DK_CHARGE, 2, 6.0f, 2.0f);
        setBckCore(BCK_DK_C_CHARGE, 2, 3.0f, 2.0f);
        field_0x698 = 45 + nREG_S(9);
        mMoveMode = 1;
        break;
    }
    case 1: {
        if (mpMorfSO->checkFrame(14.0f) != 0) {
            mCreatureSound.startCreatureSound(Z2SE_EN_DK_CHARGE, 0, -1);
        }
        cLib_chaseF(&speed.y, -3.0f, 0.2f);
        cLib_chaseF(&speedF, 0.0f, 0.2f);
        if (field_0x698 == 0) {
            setBck(BCK_DK_ATTACK, 2, 3.0f, 1.0f);
            setBckCore(BCK_DK_C_ATTACK, 2, 3.0f, 1.0f);
            field_0x698 = 100;
            mMoveMode = 2;
        }
        break;
    }
    case 2:
    case 3: {
        cLib_chaseF(&speed.y, 0.0, 0.2f);
        cLib_chaseF(&speedF, 0.0, 0.2f);
        mAtSphere.OnAtSetBit();
        mAtSphere.SetTgSpl(1);
        mAtSphere.SetAtMtrl(5);
        mSphere.SetTgSpl(1);
        mCoreSphere.SetTgSpl(1);
        setElectricEffect();
        field_0x6a3 = 1;
        fopAcM_OffStatus(this, 0x80000);
        if (mMoveMode == 2) {
            if (field_0x698 == 0) {
                field_0x69c = 150;
                setActionMode(1, 0);
            }
            if (daPy_getPlayerActorClass()->checkElecDamage() != 0) {
                mMoveMode = 3;
            }
        } else {
            if (daPy_getPlayerActorClass()->checkElecDamage() == 0) {
                field_0x69c = 150;
                setActionMode(1, 0);
            }
        }
        break;
    }
    }
}

void daE_DK_c::executeDamage() {
    switch (mMoveMode) {
    case 0:
    case 1: {
        if (mMoveMode == 0) {
            speedF = 8.0f + nREG_F(5);
        } else {
            speedF = 8.0f + nREG_F(5);
        }

        shape_angle.x = 0xf000;

        setBck(BCK_DK_GUARD, 0, 3.0f, 1.0f);
        setBckCore(BCK_DK_C_GUARD, 0, 3.0f, 1.0f);

        shape_angle.y = mAtInfo.mHitDirection.y;
        current.angle.y = mAtInfo.mHitDirection.y + 0x8000;

        mMoveMode = 2;

        mCreatureSound.startCreatureSound(Z2SE_EN_DK_GUARD, 0, -1);

        break;
    }
    case 2: {
        cLib_addCalcAngleS(&shape_angle.x, 0, 0x10, 0x100, 0x10);

        cLib_chaseF(&speed.y, 0.0, 0.2f);
        cLib_chaseF(&speedF, 0.0, 0.2f);

        if (mpMorfSO->isStop() != 0) {
            shape_angle.x = 0;
            setActionMode(1, 0);
        }

        break;
    }
    }
}

void daE_DK_c::BodyDeathMove() {
    cLib_chaseF(&field_0x6ac, -2.0f, 0.2f);
    cLib_chaseF(&field_0x6b0, 0.0f, 0.2f);

    field_0x670.x += field_0x6b0 * cM_ssin(field_0x68c);
    field_0x670.y += field_0x6ac;
    field_0x670.z += field_0x6b0 * cM_scos(field_0x68c);

    dBgS_GndChk gndChk;

    cXyz gndChkPos(field_0x670);
    gndChkPos.y += 100.0f;
    gndChk.SetPos(&gndChkPos);

    f32 crossY = dComIfG_Bgsp().GroundCross(&gndChk);
    if (field_0x670.y + 50.0f < crossY) {
        field_0x6ac = 0;
        field_0x670.y = crossY - 50.0f;
    }
}

void daE_DK_c::executeDeath() {
    cXyz itemPos;
    switch (mMoveMode) {
    case 0: {
        setCoreDeadEffect();
    }
    case 1: {
        setBodyDeadEffect();

        setBck(BCK_DK_SHELLDEAD, 0, 3.0f, 0.0f);
        setBckCore(BCK_DK_C_DEAD, 0, 3.0f, 0.0f);

        field_0x6a2 = 1;

        field_0x670 = current.pos;

        field_0x6b4 = 100.0f;

        mSphere.OffCoSetBit();
        mSphere.OffTgSetBit();
        mCoreSphere.OffTgSetBit();
        fopAcM_OffStatus(this, 0x100);

        field_0x68e = 0;

        attention_info.flags = 0;

        mCreatureSound.startCreatureSound(Z2SE_EN_DK_PULLOUT, 0, -1);

        if (mMoveMode == 0) {
            field_0x68c = mAtInfo.mHitDirection.y + 0x8000;

            field_0x6b0 = 3.0f;
            field_0x6ac = 5.0f;
        } else {
            field_0x68c = mAtInfo.mHitDirection.y + 0x8000;

            field_0x6b0 = 5.0f;
            field_0x6ac = 7.0f;

            current.angle.y = mAtInfo.mHitDirection.y + 0x8000;

            speedF = 10.0f;
            speed.y = 5.0f;

            field_0x68e = 0x1000;
        }
        mMoveMode = 2;
        mCreatureSound.startCreatureSound(Z2SE_EN_DK_BUBBLE, 0, -1);

        break;
    }
    case 2: {
        setBodyDeadEffect();

        shape_angle.y += field_0x68e;
        shape_angle.x += field_0x68e;

        if (mObjAcch.ChkGroundHit()) {
            cLib_chaseAngleS(&field_0x68e, 0, 0x40);
        } else {
            cLib_chaseAngleS(&field_0x68e, 0x100, 0x40);
        }

        BodyDeathMove();

        if (fopAcM_CheckStatus(this, 0x100000) == 0) {
            mMoveMode = 3;

            mpMorfSO->setPlaySpeed(1.0f);
            mpCoreMorfSO->setPlaySpeed(1.0f);

            current.pos.y += 125.0f;

            field_0x6b4 = -25.0f + nREG_F(7);

            field_0x68e = 0x800;
            field_0x69c = 30;
            field_0x698 = 100;
        }

        break;
    }
    case 3: {
        setBodyDeadEffect();

        if (field_0x69c != 0) {
            shape_angle.y += field_0x68e;
            shape_angle.x += field_0x68e;

            if (mObjAcch.ChkGroundHit()) {
                cLib_chaseAngleS(&field_0x68e, 0, 0x40);
            } else {
                cLib_chaseAngleS(&field_0x68e, 0x100, 0x40);
            }

            cLib_chaseF(&speed.y, -2.0f + nREG_F(8), 0.2f);
            cLib_chaseF(&speedF, 0.0f, 0.2f);

            if (field_0x69c == 1) {
                itemPos.set(current.pos.x, current.pos.y + 90.0f, current.pos.z);

                fopAcM_createItem(&itemPos, 0, -1, -1, 0, NULL, 0);
                fopAcM_createDisappear(this, &itemPos, 5, 0, 0x16);

                field_0x6a1 = 1;
            }
        }
        BodyDeathMove();
        if (mpMorfSO->isStop() == 0) {
            cLib_chaseUC(&field_0x691, 120.0f - mpMorfSO->getFrame(), 2);
            cLib_chaseUC(&field_0x692, field_0x691, 3);
        } else {
            cLib_chaseUC(&field_0x691, 0, 2);
            cLib_chaseUC(&field_0x692, field_0x691, 3);

            if (field_0x691 == 0) {
                fopAcM_delete(this);
            }
        }

        break;
    }
    }
}

void daE_DK_c::action() {
    checkWaterHeight();

    mAtSphere.OffAtSetBit();

    damage_check();

    fopAcM_OnStatus(this, 0x80000);

    if (fopAcM_searchPlayerDistance(this) > l_HIO.first_attack_range) {
        field_0x690 = 0x0;
    }

    mAtSphere.SetTgSpl(0);
    mAtSphere.SetAtMtrl(0);
    mSphere.SetTgSpl(0);
    mCoreSphere.SetTgSpl(0);

    field_0x6a3 = 0;

    s32 isFocusedOnPlayer = FALSE;

    switch (mActionMode) {
    case ACTION_MODE_WAIT:
        executeWait();
        break;
    case ACTION_MODE_CHASE:
        isFocusedOnPlayer = TRUE;
        executeChase();
        break;
    case ACTION_MODE_ATTACK:
        isFocusedOnPlayer = TRUE;
        executeAttack();
        break;
    case ACTION_MODE_HURT:
        isFocusedOnPlayer = TRUE;
        executeDamage();
        break;
    case ACTION_MODE_DIE:
        executeDeath();
        break;
    }

    mCreatureSound.setLinkSearch(isFocusedOnPlayer);

    fopAcM_posMoveF(this, mStts.GetCCMoveP());

    mObjAcch.CrrPos(dComIfG_Bgsp());

    if (!mObjAcch.ChkWallHit() && speed.y) {
        cXyz collCenter(current.pos);
        collCenter.y += 150.0f + nREG_F(18);
        mSphereChk.SetC(collCenter);
        mSphereChk.SetR(150.0f + nREG_F(19));
        mSphereChk.SetCallback(SphBgcCallBack);
        dComIfG_Bgsp().SphChk(&mSphereChk, this);
    }

    mpBrkAnm[field_0x6a0]->play();
    mpBtkAnm[field_0x6a0]->play();

    mpMorfSO->play(0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
    mpCoreMorfSO->play(0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
}

void daE_DK_c::mtx_set() {
    if (field_0x6a2 == 0) {
        mDoMtx_stack_c::transS(current.pos.x, current.pos.y + 100.0f, current.pos.z);
        mDoMtx_stack_c::ZXYrotM(shape_angle);
    } else {
        mDoMtx_stack_c::transS(field_0x670.x, field_0x670.y + 100.0f, field_0x670.z);
    }
    mDoMtx_stack_c::scaleM(l_HIO.model_size, l_HIO.model_size, l_HIO.model_size);

    J3DModel* model = mpMorfSO->getModel();
    model->setBaseTRMtx(mDoMtx_stack_c::get());
    mpMorfSO->modelCalc();

    if (!field_0x6a2) {
        mDoMtx_stack_c::copy(model->getAnmMtx(14));
        mDoMtx_stack_c::transS(mDoMtx_stack_c::get()[0][3], mDoMtx_stack_c::get()[1][3],
                               mDoMtx_stack_c::get()[2][3]);
        mDoMtx_stack_c::ZXYrotM(shape_angle);
        mDoMtx_stack_c::transM(0.0f, -50.0f + nREG_F(17), 0.0f);
    } else {
        mDoMtx_stack_c::transS(current.pos.x, current.pos.y + field_0x6b4, current.pos.z);
        mDoMtx_stack_c::transM(0.0f, 90.0f, 0.0f);
        mDoMtx_stack_c::ZXYrotM(shape_angle);
        mDoMtx_stack_c::transM(0.0f, -90.0f, 0.0f);
    }
    mDoMtx_stack_c::scaleM(l_HIO.model_size, l_HIO.model_size, l_HIO.model_size);

    mpCoreMorfSO->getModel()->setBaseTRMtx(mDoMtx_stack_c::get());
    mpCoreMorfSO->modelCalc();
}

void daE_DK_c::cc_set() {
    cXyz sphCenter;

    mDoMtx_stack_c::copy(mpMorfSO->getModel()->getAnmMtx(14));
    mDoMtx_stack_c::transS(mDoMtx_stack_c::get()[0][3], mDoMtx_stack_c::get()[1][3],
                           mDoMtx_stack_c::get()[2][3]);

    mDoMtx_stack_c::multVecZero(&eyePos);
    eyePos.y += 60.0f + nREG_F(14);

    attention_info.position = eyePos;
    attention_info.position.y += 100.0f + nREG_F(15);

    if (field_0x6a2 == 0) {
        mDoMtx_stack_c::copy(mpMorfSO->getModel()->getAnmMtx(0));
        mDoMtx_stack_c::transM(nREG_F(10), 40.0f + nREG_F(0xb), nREG_F(0xc));
        mDoMtx_stack_c::multVecZero(&sphCenter);
        mSphere.SetC(sphCenter);
        mSphere.SetR(90.0f + nREG_F(13));
        dComIfG_Ccsp()->Set(&mSphere);

        mDoMtx_stack_c::copy(mpMorfSO->getModel()->getAnmMtx(0));
        mDoMtx_stack_c::transM(0.0f, 20.0f + nREG_F(16), 0.0f);
        mDoMtx_stack_c::multVecZero(&sphCenter);
        mAtSphere.SetC(sphCenter);
        mAtSphere.SetR(160.0f + nREG_F(14));
        dComIfG_Ccsp()->Set(&mAtSphere);

        mDoMtx_stack_c::copy(mpMorfSO->getModel()->getAnmMtx(0));
        mDoMtx_stack_c::transM(0.0f, 50.0f + nREG_F(16), 0.0f);
        mDoMtx_stack_c::multVecZero(&sphCenter);
        mCoreSphere.SetC(sphCenter);
        mCoreSphere.SetR(70.0f + nREG_F(15));
        dComIfG_Ccsp()->Set(&mCoreSphere);
    }
}

int daE_DK_c::execute() {
    if (field_0x698 != 0) {
        field_0x698--;
    }
    if (field_0x69c != 0) {
        field_0x69c--;
    }
    if (field_0x694) {
        field_0x694--;
    }

    action();
    mtx_set();
    cc_set();

    return 1;
}

static int daE_DK_Execute(daE_DK_c* i_this) {
    return i_this->execute();
}

static int daE_DK_IsDelete(daE_DK_c* i_this) {
    return 1;
}

int daE_DK_c::_delete() {
    dComIfG_resDelete(&mPhaseReq, "E_DK");

    if (mHIOInit != 0) {
        data_806AD7F8 = 0;
        mDoHIO_DELETE_CHILD(data_806AD7F8);
    }

    if (heap != NULL) {
        mCreatureSound.deleteObject();
    }

    return 1;
}

static int daE_DK_Delete(daE_DK_c* i_this) {
    fpc_ProcID unusedId = fopAcM_GetID(i_this); // debug match
    return i_this->_delete();
}

int daE_DK_c::CreateHeap() {
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes("E_DK", BMDR_DK);
    JUT_ASSERT(1488, modelData != NULL);
    mpMorfSO = new mDoExt_McaMorfSO(modelData, NULL, NULL,
                                    (J3DAnmTransform*)dComIfG_getObjectRes("E_DK", BCK_DK_WAIT), 0, 1.0f, 0,
                                    -1, &mCreatureSound, 0x80000, 0x31000284);

    if (mpMorfSO == NULL || mpMorfSO->getModel() == NULL) {
        return 0;
    }

    for (s32 i = 0; i < 2; i++) {
        mpBrkAnm[i] = new mDoExt_brkAnm();
        if (mpBrkAnm[i] == 0) {
            return 0;
        }

        J3DAnmTevRegKey* tevKey = (J3DAnmTevRegKey*)dComIfG_getObjectRes("E_DK", dk_brk_name[i]);
        if (mpBrkAnm[i]->init(mpMorfSO->getModel()->getModelData(), tevKey,
            1, 2, 1.0f, 0, -1) == 0)
        {
            return 0;
        }

        mpBtkAnm[i] = new mDoExt_btkAnm();
        if (mpBtkAnm[i] == 0) {
            return 0;
        }

        J3DAnmTextureSRTKey* texKey =
            (J3DAnmTextureSRTKey*)dComIfG_getObjectRes("E_DK", dk_btk_name[i]);
        if (mpBtkAnm[i]->init(mpMorfSO->getModel()->getModelData(), texKey,
            1, 2, 1.0f, 0, -1) == 0) {
            return 0;
        }
    }

    modelData = (J3DModelData*)dComIfG_getObjectRes("E_DK", BMDR_DK_CORE);
    JUT_ASSERT(1537, modelData != NULL);
    mpCoreMorfSO = new mDoExt_McaMorfSO(modelData, NULL, NULL,
                                        (J3DAnmTransform*)dComIfG_getObjectRes("E_DK", BCK_DK_C_WAIT), 0,
                                        1.0f, 0, -1, &mCreatureSound, 0x80000, 0x11000084);

    if (mpCoreMorfSO == NULL || mpCoreMorfSO->getModel() == NULL) {
        return 0;
    } else {
        return 1;
    }
}

static int useHeapInit(fopAc_ac_c* i_this) {
    daE_DK_c* dk = (daE_DK_c*)i_this;
    return dk->CreateHeap();
}

int daE_DK_c::create() {
    fopAcM_ct(this, daE_DK_c);

    s32 loadRes = dComIfG_resLoad(&mPhaseReq, "E_DK");
    if (loadRes == cPhs_COMPLEATE_e) {
        OS_REPORT("E_DK PARAM %x\n", fopAcM_GetParam(this));
        field_0x6a4 = fopAcM_GetParam(this);

        if (field_0x6a4 != 0xff) {
            if (dComIfGs_isSwitch(field_0x6a4, fopAcM_GetRoomNo(this)) != 0) {
                OS_REPORT("E_DK やられ後なので再セットしません\n");
                return cPhs_ERROR_e;
            }
        }

        u8 param_1 = fopAcM_GetParam(this) >> 8 & 0xff;
        if (param_1 == 0xff) {
            field_0x6a8 = 1000.0f;
        } else {
            field_0x6a8 = param_1 * 100.0f;
            if (field_0x6a8 < 500.0f) {
                field_0x6a8 = 500.0f;
            }
        }

        if (fopAcM_entrySolidHeap(this, useHeapInit, 0x2520) == 0) {
            return cPhs_ERROR_e;
        }

        if (data_806AD7F8 == 0) {
            data_806AD7F8 = 1;
            this->mHIOInit = 1;
            l_HIO.field_0x4 = mDoHIO_CREATE_CHILD("", &l_HIO);
        }

        attention_info.flags = fopAc_AttnFlag_BATTLE_e;

        fopAcM_SetMtx(this, mpMorfSO->getModel()->getBaseTRMtx());
        fopAcM_SetMin(this, -200.0, -200.0, -200.0);
        fopAcM_SetMax(this, 200.0, 200.0, 200.0);

        mObjAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1, &mAcchCir,
                     fopAcM_GetSpeed_p(this), NULL, NULL);

        mAcchCir.SetWall(150.0f + nREG_F(0x12), 150.0f + nREG_F(0x13));

        field_0x560 = health = 100;

        mStts.Init(100, 0, this);
        mSphere.Set(cc_dk_src);
        mSphere.SetStts(&mStts);

        mAtSphere.Set(cc_dk_at_src);
        mAtSphere.SetStts(&mStts);

        mCoreSphere.Set(cc_dk_core_src);
        mCoreSphere.SetStts(&mStts);

        mCreatureSound.init(&current.pos, &eyePos, 3, 1);
        mCreatureSound.setEnemyName("E_dk");

        mAtInfo.mpSound = &mCreatureSound;

        mAtInfo.mPowerType = 0x1;

        field_0x691 = 0x96;
        field_0x692 = 0xff;

        onWolfNoLock();

        daE_DK_Execute(this);
    }
    return loadRes;
}

daE_DK_c::daE_DK_c() {
}

static int daE_DK_Create(daE_DK_c* i_this) {
    return i_this->create();
}

static actor_method_class l_daE_DK_Method = {
    (process_method_func)daE_DK_Create,
    (process_method_func)daE_DK_Delete,
    (process_method_func)daE_DK_Execute,
    (process_method_func)daE_DK_IsDelete,
    (process_method_func)daE_DK_Draw,
};

extern actor_process_profile_definition g_profile_E_DK = {
    fpcLy_CURRENT_e,        // mLayerID
    7,                      // mListID
    fpcPi_CURRENT_e,        // mListPrio
    PROC_E_DK,              // mProcName
    &g_fpcLf_Method.base,  // sub_method
    sizeof(daE_DK_c),       // mSize
    0,                      // mSizeOther
    0,                      // mParameters
    &g_fopAc_Method.base,   // sub_method
    755,                    // mPriority
    &l_daE_DK_Method,       // sub_method
    0x000C0100,             // mStatus
    fopAc_ENEMY_e,          // mActorType
    fopAc_CULLBOX_CUSTOM_e, // cullType
};
