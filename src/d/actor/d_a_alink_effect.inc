/**
 * d_a_alink_effect.inc
 * Player Particle Effect handling
 */

#include "JSystem/J2DGraph/J2DAnimation.h"
#include "JSystem/J2DGraph/J2DScreen.h"
#include "d/actor/d_a_alink.h"
#include "d/d_com_inf_game.h"
#include "d/d_k_wmark.h"

EffParamProc daAlink_c::m_fEffParamProc[] = {
    &daAlink_c::setEffectFrontRollParam,
    &daAlink_c::setEffectSlipParam,
    &daAlink_c::setEffectSmallLandParam,
    &daAlink_c::setEffectRunParam,
    &daAlink_c::setEffectLandParam,
    &daAlink_c::setEffectSumouParam,
};

JPABaseEmitter* daAlink_c::setEmitter(u32* i_emitterId, u16 i_effName, cXyz const* i_pos,
                                      csXyz const* i_rotation) {
    *i_emitterId = dComIfGp_particle_set(*i_emitterId, i_effName, i_pos, &tevStr, i_rotation, NULL, 0xFF, NULL,
                                     0xFF, NULL, NULL, NULL);
    dComIfGp_particle_levelEmitterOnEventMove(*i_emitterId);
    return dComIfGp_particle_getEmitter(*i_emitterId);
}

JPABaseEmitter* daAlink_c::setEmitterPolyColor(u32* i_emitterId, u16 i_effName, cBgS_PolyInfo& i_polyinfo,
                                               cXyz const* param_3, csXyz const* param_4) {
    *i_emitterId = dComIfGp_particle_setPolyColor(*i_emitterId, i_effName, i_polyinfo, param_3, &tevStr, param_4,
                                              NULL, 0, NULL, -1, NULL);
    dComIfGp_particle_levelEmitterOnEventMove(*i_emitterId);

    return dComIfGp_particle_getEmitter(*i_emitterId);
}

JPABaseEmitter* daAlink_c::setEmitterColor(u32* i_emitterId, u16 i_effName, cXyz const* i_pos,
                                           csXyz const* i_rotation) {
    *i_emitterId = dComIfGp_particle_setColor(*i_emitterId, i_effName, i_pos, &tevStr, NULL, NULL, 0.0f, -1,
                                          i_rotation, NULL, NULL, -1, NULL);
    dComIfGp_particle_levelEmitterOnEventMove(*i_emitterId);

    return dComIfGp_particle_getEmitter(*i_emitterId);
}

void daAlink_c::stopDrawParticle(u32 i_emitterId) {
    JPABaseEmitter* emitter = dComIfGp_particle_getEmitter(i_emitterId);

    if (emitter != NULL) {
        emitter->stopDrawParticle();
    }
}

void daAlink_c::setEffectFrontRollParam() {
    int j, i;
    JPABaseEmitter* emitter_p;

    for (i = 0; i < 5; i++) {
        for (j = 0; j < 2; j++) {
            if (field_0x2e54.getTypeFour(i, j) == 0) {
                emitter_p = field_0x2e54.getEmitterFour(i, j, 0);

                if (emitter_p != NULL) {
                    emitter_p->setRate(2.0f);
                }
            }
        }
    }
}

void daAlink_c::setEffectSlipParam() {
    static Vec const smokeParticleScale = {0.69999999f, 0.69999999f, 0.69999999f};
    static Vec const wolfWaterCenterScale = {1.2f, 1.2f, 1.2f};

    JGeometry::TVec3<f32> sp30;
    JPABaseEmitter* emitter;
    u8 type;

    int k, j, i;
    for (i = 0; i < 5; i++) {
        for (j = 0; j < 2; j++) {
            type = field_0x2e54.getTypeFour(i, j);

            if (type == 0) {
                emitter = field_0x2e54.getEmitterFour(i, j, 0);
                if (emitter != NULL) {
                    if (checkWolf()) {
                        emitter->setRate(0.5f);
                    }

                    emitter->setAwayFromCenterSpeed(0.0f);
                    emitter->setRandomDirectionSpeed(0.0f);
                    emitter->setSpread(0.1f);
                    emitter->setGlobalParticleScale(smokeParticleScale);
                }
            } else if (type == 1) {
                emitter = field_0x2e54.getEmitterFour(i, j, 0);
                if (emitter != NULL) {
                    emitter->setRate(0.5f);
                    emitter->setDirectionalSpeed(12.0f);
                }
            } else if (type == 16) {
                for (k = 0; k < 4; k++) {
                    emitter = field_0x2e54.getEmitterFour(i, j, k);
                    if (emitter == NULL) {
                        break;
                    }

                    if (checkWolf()) {
                        emitter->setGlobalScale(wolfWaterCenterScale);
                    } else {
                        emitter->getGlobalTranslation(&sp30);
                        sp30.x += cM_ssin(current.angle.y) * 35.0f;
                        sp30.z += cM_scos(current.angle.y) * 35.0f;

                        emitter->setGlobalTranslation(sp30);
                    }
                }
            } else if (type == 3) {
                emitter = field_0x2e54.getEmitterFour(i, j, 0);
                if (emitter != NULL) {
                    emitter->setRate(1.0f);
                }
            } else if (type == 4) {
                for (k = 0; k < 4; k++) {
                    emitter = field_0x2e54.getEmitterFour(i, j, k);
                    if (emitter != NULL) {
                        emitter->setRate(2.0f);
                    }
                }
            }
        }
    }
}

void daAlink_c::setEffectRunParam() {
    static Vec const waterScale = {0.57999998f, 0.57999998f, 0.57999998f};
    static Vec const waterCenterScale = {1.2f, 1.2f, 1.2f};
    static Vec const waterSmokeScale = {0.69999999f, 0.69999999f, 0.69999999f};

    JPABaseEmitter* emitter;
    u8 type;

    int k, j, i;
    for (i = 0; i < 5; i++) {
        for (j = 0; j < 2; j++) {
            type = field_0x2e54.getTypeFour(i, j);

            if (type == 0) {
                emitter = field_0x2e54.getEmitterFour(i, j, 0);
                if (emitter != NULL) {
                    emitter->setRate(3.0f);
                }
            } else if (type == 1) {
                emitter = field_0x2e54.getEmitterFour(i, j, 0);
                if (emitter != NULL) {
                    emitter->setRate(4.0f);
                }
            } else if (type == 2) {
                for (k = 0; k < 4; k++) {
                    emitter = field_0x2e54.getEmitterFour(i, j, k);
                    if (emitter == NULL) {
                        break;
                    }

                    emitter->setGlobalScale(waterScale);
                }
            } else if (type == 5) {
                emitter = field_0x2e54.getEmitterFour(i, j, 0);
                if (emitter != NULL) {
                    emitter->setRate(10.0f);
                    emitter->setAwayFromAxisSpeed(1.0f);
                    emitter->setGlobalScale(waterSmokeScale);
                }
            } else if (type == 16) {
                for (k = 0; k < 4; k++) {
                    emitter = field_0x2e54.getEmitterFour(i, j, k);
                    if (emitter == NULL) {
                        break;
                    }

                    emitter->setGlobalScale(waterCenterScale);
                }
            } else if (type == 3) {
                emitter = field_0x2e54.getEmitterFour(i, j, 0);
                if (emitter != NULL) {
                    if (checkWolf()) {
                        emitter->setRate(2.0f);
                        emitter->setDirectionalSpeed(8.0f);
                        emitter->setSpread(0.1f);
                    } else {
                        emitter->setRate(5.0f);
                    }
                }
            }
        }
    }
}

void daAlink_c::setEffectSmallLandParam() {
    static Vec const smokeParticleScale = {0.8f, 0.8f, 0.8f};
    static Vec const waterScale = {1.25f, 1.25f, 1.25f};
    static Vec const wolfSmokeScale = {0.69999999f, 0.69999999f, 0.69999999f};

    JPABaseEmitter* emitter;
    u8 type;

    int k, j, i;
    for (i = 0; i < 5; i++) {
        for (j = 0; j < 2; j++) {
            type = field_0x2e54.getTypeFour(i, j);

            if (type == 0) {
                emitter = field_0x2e54.getEmitterFour(i, j, 0);
                if (emitter != NULL) {
                    if (checkWolf()) {
                        emitter->setGlobalScale(wolfSmokeScale);
                    } else {
                        emitter->setRate(15.0f);
                        emitter->setVolumeSize(25);
                        emitter->setDirectionalSpeed(0.0f);
                        emitter->setGlobalParticleScale(smokeParticleScale);
                    }
                }
            } else if (type == 1) {
                emitter = field_0x2e54.getEmitterFour(i, j, 0);
                if (emitter != NULL) {
                    if (checkWolf()) {
                        emitter->setGlobalScale(wolfSmokeScale);
                    } else {
                        emitter->setRate(8.0f);
                        emitter->setVolumeSize(25);
                        emitter->setDirectionalSpeed(0.0f);
                        emitter->setAwayFromAxisSpeed(8.0f);
                    }
                }
            } else if (type == 2) {
                if (checkWolf()) {
                    for (k = 0; k < 4; k++) {
                        emitter = field_0x2e54.getEmitterFour(i, j, k);
                        if (emitter == NULL) {
                            break;
                        }

                        emitter->setGlobalScale(waterScale);
                    }
                }
            }
        }
    }
}

void daAlink_c::setEffectLandParam() {}

void daAlink_c::setEffectSumouParam() {
    static Vec const shikoScale = {0.69999999f, 0.69999999f, 0.69999999f};
    static Vec const pushedSlip = {0.8f, 0.8f, 0.8f};
    static Vec const loseScale = {1.1f, 1.1f, 1.1f};

    JPABaseEmitter* emitter;
    u8 type;

    int j, i;    
    GXColor sp3C;
    GXColor sp40;
    u8 sp48;
    f32 sp44;
    
    for (i = 0; i < 5; i++) {
        for (j = 0; j < 2; j++) {
            type = field_0x2e54.getTypeFour(i, j);

            if (type == 0) {
                emitter = field_0x2e54.getEmitterFour(i, j, 0);
                if (emitter != NULL) {
                    if (mProcID == PROC_SUMOU_SHIKO) {
                        emitter->setGlobalScale(shikoScale);
                    } else if (mProcID == PROC_SUMOU_WIN_LOSE) {
                        emitter->setGlobalScale(loseScale);
                    } else if ((mProcID == PROC_SUMOU_SIDE_MOVE || mProcID == PROC_SUMOU_ACTION) &&
                               (field_0x3198 == 0x143 || field_0x3198 == 0x142))
                    {
                        
                        dPa_control_c::getPolyColor(mLinkAcch.m_gnd, j, &sp3C, &sp40, &sp48, &sp44);

                        if (sp48 == emitter->getGlobalAlpha()) {
                            emitter->setGlobalAlpha(sp48 >> 1);
                        }
                    } else {
                        emitter->setGlobalScale(pushedSlip);
                    }
                }
            }
        }
    }
}

void daAlink_c::setFootEffectProcType(int param_0) {
    if (param_0 != field_0x2f9f) {
        field_0x2e54.clearFourAllID();
    }

    mEffProc = param_0;
    field_0x2f9f = mEffProc;
}

void daAlink_c::setWolfFootOn(int param_0) {
    daPy_frameCtrl_c* framectrl = &mUnderFrameCtrl[1];

    if (framectrl->checkPass(l_wolfFootOnFrame[param_0][0])) {
        onResetFlg1(RFLG1_UNK_4);
    } else if (framectrl->checkPass(l_wolfFootOnFrame[param_0][1])) {
        onResetFlg1(RFLG1_UNK_8);
    } else if (framectrl->checkPass(l_wolfFootOnFrame[param_0][2])) {
        onResetFlg1(RFLG1_UNK_10);
    } else if (framectrl->checkPass(l_wolfFootOnFrame[param_0][3])) {
        onResetFlg1(RFLG1_UNK_20);
    }
}

void daAlink_c::setFootMark(cXyz* i_pos, u16 i_mtxNo, int param_2) {
    cXyz sp8;
    mDoMtx_multVecSR(mpLinkModel->getAnmMtx(i_mtxNo), &cXyz::BaseX, &sp8);
    dkWmark_c::setFootMark(i_pos, sp8.atan2sX_Z(), param_2);
}

void daAlink_c::setEffect() {
    if (mProcID == PROC_CUT_TURN || mProcID == PROC_BOARD_CUT_TURN) {
        setCutTurnEffect();
    } else if (mProcID == PROC_HORSE_CUT_TURN) {
        setHorseCutTurnEffect();
    } else if (mProcID == PROC_WOLF_ROLL_ATTACK) {
        setWolfRollAttackEffect();
    } else if (mProcID == PROC_ELEC_DAMAGE) {
        setElecDamageEffect();
    }

    if (checkMagneBootsOn() || (checkEquipHeavyBoots() && mLinkAcch.ChkGroundHit() &&
                                dComIfG_Bgsp().GetMagnetCode(mLinkAcch.m_gnd) == 2))
    {
        if (!checkMagneBootsOn() && !checkNoResetFlg3(FLG3_UNK_1)) {
            mZ2Link.setMagnetized(true);
            onNoResetFlg3(FLG3_UNK_1);
        }
        setMagneBootsEffect();
    } else if (checkNoResetFlg3(FLG3_UNK_1)) {
        offNoResetFlg3(FLG3_UNK_1);
        mZ2Link.setMagnetized(false);
    }

    if (getSumouMode() != 0) {
        setSumouEffect();
    } else {
        setWaterfallEffect();
    }

    setSwordChargeEffect();

    if (checkWolf()) {
        setWolfLockAttackEffect();
        setWolfJumpAttackEffect();
    } else {
        setBootsLightEffect();
    }

    setLightningSwordEffect();

    if (mLinkAcch.ChkWallHit()) {
        cXyz spD0;
        cXyz spC4;

        dBgS_AcchCir* acchcir = mAcchCir;
        for (int i = 0; i < 3; i++, acchcir++) {
            if (acchcir->ChkWallHit() && checkWolfBarrierWallHit(*acchcir)) {
                spD0.set(current.pos.x, current.pos.y + acchcir->GetWallH(), current.pos.z);
                spC4.set(spD0.x - (acchcir->GetWallR() + 5.0f) * cM_ssin(acchcir->GetWallAngleY()),
                         spD0.y,
                         spD0.z - (acchcir->GetWallR() + 5.0f) * cM_scos(acchcir->GetWallAngleY()));

                if (commonLineCheck(&spD0, &spC4) && checkWolfBarrierWallHit(mLinkLinChk)) {
                    setWolfBarrierHitEffect(mLinkLinChk);
                    break;
                }
            }
        }
    }

    setWaterDropEffect();
    setSwordUpColorEffect();
    setSwordCutSplash();
    setWoodShieldBurnEffect();
    setFreezeEffect();

    if (mProcID == PROC_METAMORPHOSE) {
        setMetamorphoseEffect();
    } else {
        setRunSplash();
    }

    setBottleEffect();

    if (checkEndResetFlg0(ERFLG0_UNK_20000000) || 0.0f != field_0x346c) {
        if (checkEndResetFlg0(ERFLG0_UNK_20000000) && field_0x346c < 0.0f) {
            field_0x346c *= -1.0f;
        }

        field_0x346c += 0.0625f;

        if (field_0x346c > 1.0f) {
            if (checkNoResetFlg3(FLG3_UNK_200000) &&
                dComIfGp_getNeedLightDropNum() ==
                    dComIfGs_getLightDropNum(dComIfGp_getStartStageDarkArea()))
            {
                field_0x346c = 1.0f;

                if (checkEventRun()) {
                    changeOriginalDemo();
                    changeDemoMode(daPy_demo_c::DEMO_UNK_94_e, 0, 0, 0);
                }
            } else {
                field_0x346c = -1.0f;
            }
        }
    }

    cXyz spB8(current.pos);
    cXyz spAC(current.pos);
    Vec spA0;

    if (checkModeFlg(0x40000)) {
        if (checkWolf()) {
            spB8.x += 40.0f * cM_ssin(current.angle.y);
            spB8.z += 40.0f * cM_scos(current.angle.y);
            spAC.y -= mpHIO->mWolf.mWlSwim.m.mStartHeight;
        } else {
            spAC.y -= mpHIO->mSwim.m.mStartHeight;
        }

        spB8.y = mWaterY;
        if (checkNoResetFlg0(FLG0_SWIM_UP) && mNormalSpeed > 8.0f) {
            setEmitterPolyColor(&field_0x31c0, ID_ZI_J_HIKINAMI_A, mLinkAcch.m_wtr, &spB8, &current.angle);
        }
        spB8.y = spAC.y;
    } else if (checkModeFlg(0x10000) && mProcVar4.field_0x3010 != 0 &&
               0.0f != mUnderFrameCtrl[0].getRate() && dComIfG_Bgsp().ChkPolySafe(mPolyInfo1) &&
               (dKy_pol_efftype_get(&mPolyInfo1) == 1 || dKy_pol_efftype_get(&mPolyInfo1) == 1))
    {
        int sp44;
        if (dKy_pol_efftype_get(&mPolyInfo1) == 1) {
            sp44 = 0;
        } else {
            sp44 = 1;
        }

        field_0x31c0 =
            dComIfGp_particle_setPolyColor(field_0x31c0, ID_ZI_J_TSUTANOBORI_A, mPolyInfo1, &current.pos, &tevStr,
                                           &shape_angle, NULL, sp44, NULL, -1, NULL);
        dComIfGp_particle_levelEmitterOnEventMove(field_0x31c0);
    }

    if (mProcID == PROC_DEAD && (field_0x2f9d & 4)) {
        mDoMtx_multVecZero(mpLinkModel->getAnmMtx(mProcVar4.field_0x3010), &spAC);
        mLinkGndChk.SetPos(&spAC);

        f32 temp_f1 = dComIfG_Bgsp().GroundCross(&mLinkGndChk);
        if (fabsf(current.pos.y - temp_f1) < 50.0f) {
            spAC.y = temp_f1;
        }
    }

    if (!checkWolf()) {
        mDoMtx_multVecZero(mpLinkModel->getAnmMtx(field_0x30b4), &spA0);

        if (spA0.y < (mWaterY - 50.0f)) {
            JPABaseEmitter* sp40 =
                setEmitterPolyColor(&field_0x31bc, ID_ZI_J_LK_ABUKU_A, mLinkAcch.m_wtr, (cXyz*)&spA0, &shape_angle);
            if (sp40 != NULL) {
                sp40->setParticleCallBackPtr(dPa_control_c::getWaterBubblePcallBack());
            }
        }

        if (dComIfGp_checkPlayerStatus0(0, 8)) {
            spB8.x -= 30.0f * cM_ssin(shape_angle.y);
            spB8.z -= 30.0f * cM_scos(shape_angle.y);
            spAC.y = mLeftFootPos.y;
            spB8.y = spAC.y;
        } else if (dComIfGp_checkPlayerStatus1(0, 0x02000000)) {
            spB8.x += 60.0f * cM_ssin(shape_angle.y);
            spB8.z += 60.0f * cM_scos(shape_angle.y);

            if (mRightFootPos.y < mLeftFootPos.y) {
                spAC.y = mRightFootPos.y;
            } else {
                spAC.y = mLeftFootPos.y;
            }

            spB8.y = spAC.y;
        } else if (dComIfGp_checkPlayerStatus1(0, 0x10000)) {
            spAC.y = mLeftFootPos.y;
            spB8.y = spAC.y;
        }
    }

    u32 var_r28 = 0;
    u32 var_r29 = 0;

    cXyz sp94(mRightFootPos);
    cXyz sp88(mLeftFootPos);
    cXyz sp7C(mRightHandPos);
    cXyz sp70(mLeftHandPos);
    cXyz sp64(spAC);

    if (mEffProc == EFFPROC_SUMOU) {
        if (mProcID == PROC_SUMOU_SHIKO || mProcID == PROC_SUMOU_WIN_LOSE) {
            var_r28 = 1;
            sp88.y = current.pos.y;
        }
    } else if (mEffProc == EFFPROC_LAND) {
        var_r28 |= 0x3B;
    } else if (mEffProc == EFFPROC_FRONT_ROLL) {
        if (mProcID == PROC_SLIDE) {
            var_r29 |= 4;
        }

        var_r28 |= 4;
        var_r29 |= 0x10000;
    } else if (mEffProc == EFFPROC_SLIP) {
        if (checkWolf()) {
            sp94.y -= 10.0f;
            sp88.y -= 10.0f;
            sp7C.y -= 10.0f;
            sp70.y -= 10.0f;
            sp64.y -= 10.0f;
        }
        var_r29 |= 0x10000;
    } else if (mEffProc == EFFPROC_RUN) {
        if (checkWolf() && field_0x2f9d != 0) {
            var_r29 |= 0x10000;
        }
    } else if (mEffProc == EFFPROC_SMALL_LAND) {
        var_r28 |= 0x38;
        if (checkWolf()) {
            var_r28 |= 3;
        }
    }

    if (checkCanoeRide() || checkModeFlg(0x10000)) {
        var_r29 |= 0x40000;
    }

    BOOL sp3C = FALSE;
    if (mProcID == PROC_SUMOU_MOVE &&
        (field_0x3198 == 0x14C || field_0x3198 == 0x14D || field_0x3198 == 0x155))
    {
        sp3C = TRUE;
        current.angle.y += 0x8000;
    }

    f32 temp_f30 = field_0x3798.absXZ(current.pos);
    if ((!checkModeFlg(0x70C52) && (mLinkAcch.ChkGroundHit() || checkMagneBootsOn())) ||
        mProcID == PROC_SUMOU_WIN_LOSE || (checkBoardRide() && !checkModeFlg(2)) ||
        mProcID == PROC_WOLF_TAG_JUMP)
    {
        field_0x2e54.setEffectFour(
            &tevStr, &spB8, var_r28, var_r29, (field_0x2f9d & 0x04) ? &sp64 : NULL,
            (field_0x2f9d & 0x08) ? &sp70 : NULL, (field_0x2f9d & 0x10) ? &sp7C : NULL,
            (field_0x2f9d & 0x20) ? &sp88 : NULL, (field_0x2f9d & 0x40) ? &sp94 : NULL,
            &current.angle, NULL, fopAcM_GetRoomNo(this), field_0x3420, temp_f30);

        if (checkBoardRide() && temp_f30 > 3.0f && mGndPolyAtt0 == 0xD) {
            JPABaseEmitter* sp38 = setEmitterPolyColor(&field_0x31c0, dPa_RM(ID_ZI_S_LK_SB_A), mLinkAcch.m_gnd,
                                                       &current.pos, &shape_angle);
            if (sp38 != NULL) {
                cXyz sp58;
                f32 var_f31 = temp_f30 / mpHIO->mItem.mBoard.m.mEffectMaxSpeed;
                if (var_f31 > 1.0f) {
                    var_f31 = 1.0f;
                }
                sp38->setRate(1.0f + (2.0f * var_f31));
                sp38->setAwayFromCenterSpeed(1.0f + (4.0f * var_f31));

                sp58.x = 0.7f + (0.3f * var_f31);
                sp58.y = sp58.x;
                sp58.z = sp58.x;
                sp38->setGlobalParticleScale(sp58);
                sp38->setGlobalAlpha(50.0f + (205.0f * var_f31));
            }
        }
    } else {
        field_0x2e54.setEffectFour(&tevStr, &spB8, var_r28, var_r29, NULL, NULL, NULL, NULL, NULL,
                                   &current.angle, NULL, fopAcM_GetRoomNo(this), field_0x3420,
                                   temp_f30);
    }

    if (mEffProc != EFFPROC_NONE) {
        (this->*m_fEffParamProc[mEffProc])();
    }

    if (mWaterY > current.pos.y) {
        field_0x2fc4 = 0x96;
    }

    int var_r26;
    if ((checkResetFlg1(daPy_RFLG1(RFLG1_UNK_20 | RFLG1_UNK_10 | RFLG1_UNK_8 | RFLG1_UNK_4)) ||
         (field_0x2fc1 != 6 && field_0x2f8c != 2)) &&
        current.pos.y > mWaterY &&
        (field_0x2fc4 != 0 || (mSinkShapeOffset < 0.0f && mSinkShapeOffset >= -15.0f)))
    {
        if (mSinkShapeOffset < 0.0f && mSinkShapeOffset >= -15.0f) {
            if (mGndPolyAtt0 == 3) {
                var_r26 = 1;
            } else {
                var_r26 = 2;
            }
        } else {
            var_r26 = 0;
        }

        if (checkWolf() && field_0x2fc1 != 6) {
            setWolfFootOn(field_0x2fc1);

            if (field_0x2fc1 == 4) {
                setWolfFootOn(5);
            }
        }

        if (checkResetFlg1(RFLG1_UNK_10)) {
            setFootMark(&mLeftFootPos, field_0x30bc, var_r26);
        }

        if (checkResetFlg1(RFLG1_UNK_20)) {
            setFootMark(&mRightFootPos, field_0x30be, var_r26);
        }

        if (checkResetFlg1(RFLG1_UNK_4)) {
            setFootMark(&mLeftHandPos, mLeftHandJntNo, var_r26);
        }

        if (checkResetFlg1(RFLG1_UNK_8)) {
            setFootMark(&mRightHandPos, mRightHandJntNo, var_r26);
        }
    }

    if (sp3C) {
        current.angle.y += 0x8000;
    }

    setFirePointDamageEffect();
}

void daAlink_c::setSumouEffect() {
    static Vec const releaeScale = {0.8f, 0.8f, 0.8f};

    daPy_frameCtrl_c* framectrl = &mUnderFrameCtrl[0];
    JPABaseEmitter* emitter;
    cXyz pos;

    if (mProcID == PROC_SUMOU_ACTION) {
        if ((field_0x3198 == 0x14F) || (field_0x3198 == 0x151) || (field_0x3198 == 0x150)) {
            emitter = setEmitter(&field_0x31c8, dPa_RM(ID_ZF_S_BSUMOUHARITE02LINK_TRACE), &current.pos, NULL);
            if (emitter != NULL) {
                emitter->setGlobalRTMatrix(mpLinkModel->getAnmMtx(9));
                if (field_0x3198 == 0x151) {
                    emitter->setLifeTime(5);
                }
            }

            if (field_0x3198 != 0x150) {
                if (framectrl->getFrame() >= 12.0f) {
                    mDoMtx_multVecZero(mpLinkModel->getAnmMtx(9), &pos);
                    setEmitter(&field_0x31cc, dPa_RM(ID_ZF_S_BSUMOUSWEAT00_SAND), &pos, NULL);
                    setEmitter(&field_0x31d0, dPa_RM(ID_ZF_S_BSUMOUSWEAT01_SWEAT), &pos, &shape_angle);
                }
            }
        } else if (field_0x3198 == 0x145 || field_0x3198 == 0x146 || field_0x3198 == 0x14E ||
                   field_0x3198 == 0x147)
        {
            if (field_0x3198 != 0x14E &&
                (field_0x3198 == 0x145 || field_0x3198 == 0x146 || framectrl->getFrame() >= 29.0f))
            {
                emitter = setEmitter(&field_0x31c8, dPa_RM(ID_ZF_S_BSUMOUTACKLELCENTER_SHOCKR), &field_0x3834, &shape_angle);
                if (emitter != NULL) {
                    emitter->setGlobalRTMatrix(mpLinkModel->getAnmMtx(0));
                }

                emitter = setEmitter(&field_0x31cc, dPa_RM(ID_ZF_S_BSUMOUTACKLELCENTER_SHOCKL), &field_0x3834, &shape_angle);
                if (emitter != NULL) {
                    emitter->setGlobalRTMatrix(mpLinkModel->getAnmMtx(0));
                }
            }

            if (framectrl->getFrame() >= 35.0f ||
                (field_0x3198 == 0x14E && framectrl->getFrame() >= 3.0f) ||
                ((field_0x3198 == 0x145 || field_0x3198 == 0x146) && framectrl->getFrame() >= 7.0f))
            {
                f32 var_f31;
                if (field_0x3198 == 0x147) {
                    var_f31 = 40.0f;
                } else {
                    var_f31 = 30.0f;
                }

                pos.set(field_0x3834.x + (var_f31 * cM_ssin(shape_angle.y)), field_0x3834.y,
                         field_0x3834.z + (var_f31 * cM_scos(shape_angle.y)));
                setEmitter(&field_0x31d0, dPa_RM(ID_ZF_S_BSUMOUSWEAT00_SAND), &pos, NULL);
                setEmitter(&field_0x31d4, dPa_RM(ID_ZF_S_BSUMOUSWEAT01_SWEAT), &pos, &shape_angle);
            }
        }
    } else if (mProcID == PROC_SUMOU_MOVE) {
        if (field_0x3198 == 0x154) {
            emitter = setEmitter(&field_0x31c8, dPa_RM(ID_ZF_S_BSUMOUSWEATPUSH_SWEAT), &field_0x3834, &shape_angle);
            if (emitter != NULL) {
                mDoMtx_stack_c::copy(mpLinkModel->getAnmMtx(0));
                mDoMtx_stack_c::transM(36.0f, 84.0f, 0.0f);
                emitter->setGlobalRTMatrix(mDoMtx_stack_c::get());
            }
        } else if (field_0x3198 == 0x155) {
            if (framectrl->getFrame() >= 17.0f) {
                emitter = setEmitter(&field_0x31c8, dPa_RM(ID_ZF_S_BSUMOUSWEAT01_SWEAT), &field_0x3834, &shape_angle);
                if (emitter != NULL) {
                    mDoMtx_stack_c::copy(mpLinkModel->getAnmMtx(0));
                    mDoMtx_stack_c::XrotM(0x7FFF);
                    emitter->setGlobalRTMatrix(mDoMtx_stack_c::get());
                    emitter->setGlobalScale(releaeScale);
                }
            }
        }
    }
}

void daAlink_c::setWaterfallEffect(const cXyz* i_pos, u32* i_emitterId) {
    setEmitter(i_emitterId, dPa_RM(ID_ZI_S_LK_TAKISHIBUKI_A), i_pos, NULL);
    i_emitterId++;
    setEmitter(i_emitterId, dPa_RM(ID_ZI_S_LK_TAKISHIBUKI_B), i_pos, NULL);
}

void daAlink_c::setWaterfallEffect() {
    Vec pos;
    if (checkEndResetFlg1(ERFLG1_WATERFALL_FRONT_HIT)) {
        if (checkWolf()) {
            mDoMtx_multVecZero(mpLinkModel->getAnmMtx(3), &pos);
            pos.y += 5.0f;
        } else {
            mDoMtx_multVecZero(mpLinkModel->getAnmMtx(3), &pos);
        }

        setWaterfallEffect((cXyz*)&pos, &field_0x31c8);
    }

    if (checkWolf() && checkEndResetFlg1(ERFLG1_UNK_1000)) {
        mDoMtx_multVecZero(mpLinkModel->getAnmMtx(26), &pos);
        pos.y -= 15.0f;
        setWaterfallEffect((cXyz*)&pos, &field_0x31d0);
    }

    if (checkEndResetFlg1(daPy_ERFLG1(ERFLG1_UNK_1000 | ERFLG1_WATERFALL_FRONT_HIT))) {
        seStartOnlyReverbLevel(Z2SE_AL_IN_WATER_FALL);
    }
}

void daAlink_c::setMagneBootsEffect() {
    static const u16 effName[] = {
        ID_ZI_J_HB_MAGNE_A,
        ID_ZI_J_HB_MAGNE_B,
        ID_ZI_J_HB_MAGNE_C,
    };

    int i, j;
    u32* emitterId;
    JPABaseEmitter* emitter;
    cXyz sp20;

    for (i = 0; i < 2; i++) {
        if (i == 0) {
            sp20 = mLeftFootPos - current.pos;
        } else {
            sp20 = mRightFootPos - current.pos;
        }

        if (checkMagneBootsOn()) {
            mDoMtx_multVecSR(mMagneBootInvMtx, &sp20, &sp20);
        }

        if (sp20.y < 6.0f) {
            emitterId = field_0x3228[i];

            for (j = 0; j < 3; j++, emitterId++) {
                emitter = setEmitter(emitterId, effName[j], &current.pos, NULL);
                if (emitter != NULL) {
                    if (j == 1) {
                        emitter->setGlobalRTMatrix(mpLinkBootModels[i]->getAnmMtx(1));
                    } else {
                        emitter->setGlobalRTMatrix(mpLinkBootModels[i]->getAnmMtx(2));
                    }
                }
            }
        }
    }
}

void daAlink_c::setSwordChargeEffect() {
    static Vec const lightAParticleScale = {1.0f, 1.171f, 1.0f};
    static Vec const lightALocalTrans = {68.0f, 0.0f, 0.0f};

    JPABaseEmitter* emitter;

    f32 var_f30;
    BOOL isMasterSwordEquip = checkMasterSwordEquip();
    if (isMasterSwordEquip) {
        var_f30 = l_swordTopLocalM.x;
    } else {
        var_f30 = l_swordTopLocalN.x;
    }

    f32 var_f31 = m_nSwordBtk->getFrame();
    if (var_f31 > 0.0f) {
        cXyz sp50;
        if (!checkWoodSwordEquip()) {
            emitter = setEmitter(&field_0x321c, ID_ZI_J_SWA_KIRARI_A, &current.pos, NULL);
            if (emitter != NULL) {
                emitter->setGlobalRTMatrix(mSwordModel->getBaseTRMtx());

                if (isMasterSwordEquip) {
                    emitter->setGlobalParticleScale(lightAParticleScale);
                    emitter->setLocalTranslation(lightALocalTrans);
                }
            }
        }

        cXyz sp44;
        mDoMtx_multVecSR(mSwordModel->getBaseTRMtx(), &cXyz::BaseX, &sp50);
        mDoMtx_multVecZero(mSwordModel->getBaseTRMtx(), &sp44);

        if (var_f31 >= 14.0f) {
            var_f31 = 14.0f;
        }

        sp50 = sp44 + ((sp50 * (var_f31 + 1.0f)) * (var_f30 / 15.0f));
        setEmitter(&field_0x3220, ID_ZI_J_SWA_KIRARI_B, &sp50, NULL);

        if (var_f31 >= 14.0f) {
            setEmitter(&field_0x3224, ID_ZI_J_SWA_KIRARI_C, &sp50, NULL);
        } else {
            if (checkWolf() || checkWoodSwordEquip()) {
                seStartSwordCut(Z2SE_WOLF_POWER_COME);
            } else {
                seStartSwordCut(Z2SE_SWORD_POWER_COME);
            }
        }
    } else {
        emitter = dComIfGp_particle_getEmitter(field_0x321c);
        if (emitter != NULL) {
            u8 alpha = emitter->getGlobalAlpha();
            if (alpha > 17) {
                emitter = setEmitter(&field_0x321c, ID_ZI_J_SWA_KIRARI_A, &current.pos, NULL);
                if (emitter != NULL) {
                    emitter->setGlobalRTMatrix(mSwordModel->getBaseTRMtx());
                    emitter->setGlobalAlpha(alpha - 17);
                }
            } else {
                emitter->setGlobalAlpha(0);
            }
        }

        stopDrawParticle(field_0x3220);
        stopDrawParticle(field_0x3224);
    }
}

void daAlink_c::setElecDamageEffect() {
    static Vec const localOffset = {0.0f, -10.0f, -10.0f};
    static Vec const localScale0 = {0.69999999f, 0.85f, 1.5f};
    static Vec const localScale1 = {1.0f, 0.85f, 1.5f};

    static const u16 effName0[] = {
        dPa_RM(ID_ZI_S_LK_BIRIBIRIA_A),
        dPa_RM(ID_ZI_S_LK_BIRIBIRIA_B),
        dPa_RM(ID_ZI_S_LK_BIRIBIRIA_C),
    };

    static const u16 effName1[] = {
        dPa_RM(ID_ZI_S_LK_BIRIBIRIC_A),
        dPa_RM(ID_ZI_S_LK_BIRIBIRIC_B),
        dPa_RM(ID_ZI_S_LK_BIRIBIRIC_C),
    };

    cXyz pos;

    const u16* effNameList;
    if (checkStageName("D_MN09A")) {
        effNameList = effName1;
    } else {
        effNameList = effName0;
    }

    JPABaseEmitter* emitter;
    int i;
    if (checkWolf()) {
        mDoMtx_multVec(mpLinkModel->getAnmMtx(2), &localOffset, &pos);

        for (i = 0; i < 3; i++, effNameList++) {
            emitter = setEmitter(&field_0x31d8[i], *effNameList, &pos, &shape_angle);
            if (emitter != NULL) {
                if (i == 2) {
                    emitter->setLocalScale(localScale1);
                } else {
                    emitter->setLocalScale(localScale0);
                }
            }
        }
    } else {
        mDoMtx_multVecZero(mpLinkModel->getAnmMtx(1), &pos);

        for (i = 0; i < 3; i++, effNameList++) {
            setEmitter(&field_0x31d8[i], *effNameList, &pos, &shape_angle);
        }

        if (checkHookshotItem(mEquipItem) && mItemMode == 6 && checkStageName("D_MN01")) {
            pos = (mHeldItemRootPos + mHookshotTopPos) * 0.5f;

            cXyz sp48 = mHookshotTopPos - mHeldItemRootPos;
            csXyz sp10(sp48.atan2sY_XZ(), sp48.atan2sX_Z(), 0);

            emitter = setEmitter(&field_0x31e4, dPa_RM(ID_ZI_S_LK_BIRIBIRIFS_A), &pos, &sp10);
            if (emitter != NULL) {
                emitter->setVolumeSize(sp48.abs());
            }
        }
    }
}

void daAlink_c::clearCutTurnEffectID() {
    for (int i = 0; i < 6; i++) {
        stopDrawParticle(field_0x3204[i]);
        field_0x3204[i] = 0;
    }
}

void daAlink_c::setCutTurnEffect() {
    static const u16 effNameNormal[] = {
        ID_ZI_J_KAITENGIRI_A,
        ID_ZI_J_KAITENGIRI_B,
    };

    static const u16 effNameLight[] = {
        ID_ZI_J_KAITENGIRIL_A,
        ID_ZI_J_KAITENGIRIL_B,
        ID_ZI_J_KAITENGIRIL_C,
        ID_ZI_J_KAITENGIRIL_D,
    };

    static const u16 effNameWater[] = {
        ID_ZI_J_KAITENGIRI_INWTR_A,
        ID_ZI_J_KAITENGIRI_INWTR_B,
    };

    static Vec const waterEffScale = {1.5f, 1.5f, 1.5f};

    static Vec const leftTransNormal[] = {
        {0.0f, 0.0f, 0.0f},
        {0.0f, 30.0f, 0.0f},
    };
    
    static Vec const leftTransLight[] = {
        {0.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 0.0f},
        {0.0f, 35.0f, 0.0f},
        {0.0f, 0.0f, 0.0f},
    };
    
    static u16 const effNameLarge[] = {
        ID_ZI_J_KAITENGIRID_A,
        ID_ZI_J_KAITENGIRID_B,
        ID_ZI_J_KAITENGIRID_C,
        ID_ZI_J_KAITENGIRID_D,
        ID_ZI_J_KAITENGIRID_E,
        ID_ZI_J_KAITENGIRID_F,
    };
    
    static Vec const leftTransLarge[] = {
        {0.0f, 0.0f, 0.0f},
        {0.0f, 35.0f, 0.0f},
        {0.0f, 0.0f, 0.0f},
        {0.0f, 45.0f, 0.0f},
        {0.0f, 30.0f, 0.0f},
        {0.0f, 50.0f, 0.0f},
    };

    static Vec const leftTransWater[] = {
        {0.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 0.0f},
    };

    if (!(mUnderFrameCtrl[0].getFrame() < mProcVar1.field_0x300a)) {
        static s16 leftRotNormal[] = {
            cM_deg2s(180), cM_deg2s(45), cM_deg2s(13), cM_deg2s(180), cM_deg2s(45), cM_deg2s(13),
        };

        static s16 leftRotLight[] = {
            cM_deg2s(180), cM_deg2s(100), cM_deg2s(13), cM_deg2s(180), cM_deg2s(80), cM_deg2s(13),
            cM_deg2s(180), cM_deg2s(80),  cM_deg2s(13), cM_deg2s(180), cM_deg2s(80), cM_deg2s(13),
        };

        static s16 leftRotLarge[] = {
            cM_deg2s(180), cM_deg2s(45), cM_deg2s(13), cM_deg2s(180), cM_deg2s(45), cM_deg2s(13),
            cM_deg2s(180), cM_deg2s(60), cM_deg2s(13), cM_deg2s(180), cM_deg2s(60), cM_deg2s(13),
            cM_deg2s(180), cM_deg2s(60), cM_deg2s(13), cM_deg2s(180), cM_deg2s(60), cM_deg2s(13),
        };

        static s16 leftRotWater[] = {
            cM_deg2s(-90), cM_deg2s(0), cM_deg2s(180), cM_deg2s(0), cM_deg2s(0), cM_deg2s(180),
        };

        u32* emitterId = field_0x3204;
        const u16* effNameList;
        JGeometry::TVec3<s16>* rot;
        JGeometry::TVec3<f32>* trans;
        int effNum;
        BOOL sp18 = FALSE;

        if (checkNoResetFlg0(FLG0_WATER_IN_MOVE)) {
            if (checkZoraWearAbility()) {
                effNameList = effNameWater;
                rot = (JGeometry::TVec3<s16>*)leftRotWater;
                trans = (JGeometry::TVec3<f32>*)leftTransWater;
                effNum = 2;
                if (mCutType == CUT_TYPE_LARGE_TURN_RIGHT || mCutType == CUT_TYPE_LARGE_TURN_LEFT) {
                    sp18 = TRUE;
                }
            } else {
                return;
            }
        } else if (mCutType == CUT_TYPE_LARGE_TURN_RIGHT || mCutType == CUT_TYPE_LARGE_TURN_LEFT) {
            effNameList = effNameLarge;
            rot = (JGeometry::TVec3<s16>*)leftRotLarge;
            trans = (JGeometry::TVec3<f32>*)leftTransLarge;
            effNum = 6;
        } else if (checkNoResetFlg3(FLG3_UNK_100000)) {
            effNameList = effNameLight;
            rot = (JGeometry::TVec3<s16>*)leftRotLight;
            trans = (JGeometry::TVec3<f32>*)leftTransLight;
            effNum = 4;
        } else {
            effNameList = effNameNormal;
            rot = (JGeometry::TVec3<s16>*)leftRotNormal;
            trans = (JGeometry::TVec3<f32>*)leftTransNormal;
            effNum = 2;
        }

        cXyz pos;
        mDoMtx_multVecZero(mpLinkModel->getAnmMtx(1), &pos);

        if (checkMagneBootsOn()) {
            mDoMtx_stack_c::transS(pos);
            mDoMtx_stack_c::concat(mMagneBootMtx);
            mDoMtx_stack_c::ZXYrotM(shape_angle);
        }

        for (int i = 0; i < effNum; i++, emitterId++, effNameList++, rot++, trans++) {
            JPABaseEmitter* emitter = setEmitter(emitterId, *effNameList, &pos, &shape_angle);
            if (emitter != NULL) {
                if (checkMagneBootsOn()) {
                    emitter->setGlobalRTMatrix(mDoMtx_stack_c::get());
                }

                if (mProcVar4.field_0x3010 == 0) {
                    emitter->setLocalRotation(*rot);
                    if (trans->y > 1.0f) {
                        emitter->setLocalTranslation(*trans);
                    }
                }

                if (sp18) {
                    if (i == 0) {
                        emitter->setGlobalParticleScale(waterEffScale);
                    } else {
                        emitter->setVolumeSize(225);
                        emitter->setAwayFromAxisSpeed(15.0f);
                    }
                }
            }
        }
    }
}

void daAlink_c::setHorseCutTurnEffect() {
    cXyz pos;
    csXyz rot(0, shape_angle.y, shape_angle.z);

    mDoMtx_multVecZero(mpLinkModel->getAnmMtx(1), &pos);
    setEmitter(&field_0x3204[0], ID_ZI_J_KAITENGIRIH_A, &pos, &rot);
    setEmitter(&field_0x3204[1], ID_ZI_J_KAITENGIRIH_B, &pos, &rot);
}

void daAlink_c::setCutLargeJumpLandEffect() {
    cXyz pos(current.pos.x + cM_ssin(shape_angle.y) * 20.0f, current.pos.y + 50.0f, current.pos.z + cM_scos(shape_angle.y) * 20.0f);
    csXyz rot;
    cM3dGPla tripla;

    mLinkGndChk.SetPos(&pos);
    f32 gnd_height = dComIfG_Bgsp().GroundCross(&mLinkGndChk);
    if (gnd_height < current.pos.y - 50.0f) {
        pos.y = current.pos.y;
        dComIfG_Bgsp().GetTriPla(mLinkAcch.m_gnd, &tripla);
    } else {
        pos.y = gnd_height;
        dComIfG_Bgsp().GetTriPla(mLinkGndChk, &tripla);
    }

    rot.x = cM_atan2s(tripla.mNormal.absXZ(), tripla.mNormal.y);
    rot.y = tripla.mNormal.atan2sX_Z();
    rot.z = 0;

    for (int i = 0; i < 6; i++) {
        static const u16 effName[] = {
            ID_ZI_J_LK_DJGIRI_A,
            ID_ZI_J_LK_DJGIRI_B,
            ID_ZI_J_LK_DJGIRI_C,
            ID_ZI_J_LK_DJGIRI_D,
            ID_ZI_J_LK_DJGIRI_E,
            ID_ZI_J_LK_DJGIRI_F,
        };
        dComIfGp_particle_set(effName[i], &pos, &tevStr, &rot, NULL);
    }
}

void daAlink_c::setBootsLightEffect() {
    static const u16 jointID[] = {
        20,
        19,
        24,
        25
    };

    static const u16 effName[] = {
        dPa_RM(ID_ZF_S_SBOOTS00FOOTL),
        dPa_RM(ID_ZF_S_SBOOTS01LEGL2),
        dPa_RM(ID_ZF_S_SBOOTS02LEGR2),
        dPa_RM(ID_ZF_S_SBOOTS03FOOTR),
    };

    if (checkStageName("F_SP116")) {
        if (dComIfGs_isOneZoneSwitch(0, fopAcM_GetRoomNo(this))) {
            for (int i = 0; i < 4; i++) {
                JPABaseEmitter* emitter = setEmitter(&field_0x326c[i], effName[i], &current.pos, &shape_angle);
                if (emitter != NULL) {
                    emitter->setGlobalRTMatrix(mpLinkModel->getAnmMtx(jointID[i]));
                }
            }

            seStartOnlyReverbLevel(Z2SE_AL_SHOE_PKPK);
        }
    }
}

void daAlink_c::setLightningSwordEffect() {
    static const u16 effName[] = {
        ID_ZI_J_LK_SWL_A,
        ID_ZI_J_LK_SWL_B,
        ID_ZI_J_LK_SWL_C
    };

    JPABaseEmitter* emitter;
    if (checkEndResetFlg2(ERFLG2_LIGHT_SWORD_GET_EFFECT)) {
        emitter = setEmitter(&field_0x3294, dPa_RM(ID_ZI_S_LK_SWL_GET_A), &current.pos, &shape_angle);
        if (emitter != NULL) {
            emitter->setGlobalRTMatrix(mSwordModel->getBaseTRMtx());
        }

        seStartOnlyReverbLevel(Z2SE_AL_LIGHTNING_SW_GET);
    } else {
        stopDrawParticle(field_0x3294);
    }

    int i;
    if (mEquipItem == 0x103 && checkNoResetFlg3(FLG3_UNK_100000)) {
        seStartOnlyReverbLevel(Z2SE_AL_LIGHTNING_SW_GLOW);

        for (i = 0; i < 3; i++) {
            emitter = setEmitter(&field_0x327c[i], effName[i], &current.pos, &shape_angle);
            if (emitter != NULL) {
                emitter->setGlobalRTMatrix(mSwordModel->getBaseTRMtx());
            }
        }
    } else {
        for (i = 0; i < 3; i++) {
            stopDrawParticle(field_0x327c[i]);
        }
    }
}

void daAlink_c::setWolfRollAttackEffect() {
    cXyz pos(current.pos.x, current.pos.y + 80.0f, current.pos.z);
    csXyz rot(shape_angle.x, shape_angle.y, 0);

    if (mProcVar2.field_0x300c == 0) {
        rot.x += 0x8000;
    }

    setEmitter(&field_0x3204[0], ID_ZI_J_WL_KAITENAT_A, &pos, &rot);
    if (mProcVar3.field_0x300e == 0) {
        setEmitter(&field_0x3204[1], ID_ZI_J_WL_KAITENAT_B, &pos, &rot);
    }
}

void daAlink_c::setWolfDigEffect() {
    JPABaseEmitter* emitter;

    if (dComIfG_Bgsp().ChkPolySafe(mPolyInfo2)) {
        f32 var_f31 = 3.0f;
        f32 var_f30;
        csXyz sp18(getGroundAngle(&mPolyInfo2, shape_angle.y), shape_angle.y, 0);

        u16 var_r29, var_r28;
        u16 var_r26;
        if (field_0x3198 == 3) {
            var_r29 = dPa_RM(ID_ZI_S_DIGSAND_A);
            var_r28 = dPa_RM(ID_ZI_S_DASHSAND_A);
        } else if (field_0x3198 == 0xD) {
            var_r29 = dPa_RM(ID_ZI_S_DIGSNOW_A);
            var_r28 = dPa_RM(ID_ZI_S_DASHSNOW_B);
        } else if (field_0x3198 == 1) {
            var_r29 = ID_ZI_J_DIG00_A;
            var_r28 = ID_ZI_J_DASHSMOKE_A;
        } else if (field_0x3198 == 4) {
            var_r29 = ID_ZI_J_DIG00_A;

            if (dKy_pol_efftype_get(&mPolyInfo2) == 1) {
                var_r28 = ID_ZI_J_DASHKUSA_A;
                var_r26 = ID_ZI_J_DASHSMOKE_A;
                var_f30 = 3.0f;
            } else {
                var_r28 = ID_ZI_J_DASHSMOKE_A;
                var_r26 = ID_ZI_J_DASHKUSA_A;
                var_f30 = 15.0f;
                var_f31 = 3.0f;
            }

            field_0x32cc = dComIfGp_particle_setPolyColor(field_0x32cc, var_r26, mPolyInfo2, &field_0x37d4, &tevStr, &sp18, NULL, 1, NULL, -1, NULL);
            emitter = dComIfGp_particle_getEmitter(field_0x32cc);
            if (emitter != NULL) {
                emitter->setAwayFromAxisSpeed(var_f30);
                dComIfGp_particle_levelEmitterOnEventMove(field_0x32cc);
            }
        } else if (field_0x3198 == 7) {
            var_r29 = 0;
            var_r28 = ID_ZI_J_DASHWTRA_C;
            setEmitterPolyColor(&field_0x32cc, ID_ZI_J_DASHWTRA_A, mPolyInfo2, &field_0x37d4, &sp18);
            setEmitterPolyColor(&field_0x31bc, ID_ZI_J_DASHWTRA_B, mPolyInfo2, &field_0x37d4, &sp18);
        } else {
            var_r29 = 0;
            var_r28 = ID_ZI_J_DASHSMOKE_A;
        }

        if (var_r29 != 0) {
            setEmitter(&field_0x31bc, var_r29, &field_0x37d4, &sp18);
        }

        emitter = setEmitterPolyColor(&field_0x31c0, var_r28, mPolyInfo2, &field_0x37d4, &sp18);
        if (emitter != NULL && field_0x3198 != 7) {
            emitter->setAwayFromAxisSpeed(var_f31);
        }
    }
}

void daAlink_c::setWolfSwimEndEffect(JPABaseEmitter** param_0, JPABaseEmitter** param_1) {
    static const u16 name0[] = {
        ID_ZI_J_WL_BURUBURU_A,
        dPa_RM(ID_ZI_S_DIGTHROUGHA_BURU_A),
        dPa_RM(ID_ZI_S_DIGTHROUGHSNOW_BURU_A),
    };
    static const u16 name1[] = {
        ID_ZI_J_WL_BURUBURU_A,
        dPa_RM(ID_ZI_S_DIGTHROUGHA_BURU_B),
        dPa_RM(ID_ZI_S_DIGTHROUGHSNOW_BURU_B),
    };

    int var_r30 = mProcVar3.field_0x300e;
    if (var_r30 >= 3 || var_r30 < 0) {
        var_r30 = 1;
    }

    *param_0 = setEmitterColor(&field_0x32cc, name0[var_r30], &field_0x37c8, &shape_angle);

    if (var_r30 != 0) {
        *param_1 = setEmitterColor(&field_0x31bc, name1[var_r30], &field_0x37c8, &shape_angle);
    }
}

void daAlink_c::setWolfLockAttackEffect() {
    static const u16 effID[] = {
        ID_ZI_J_WL_LOCKATBLUR_A,
        ID_ZI_J_WL_LOCKATBLUR_B,
        ID_ZI_J_WL_LOCKATBLUR_IND,
    };

    JPABaseEmitter* emitter;
    u8 alpha;
    if (dComIfGp_checkPlayerStatus1(0, 0x01000000)) {
        alpha = 0xFF;
    } else {
        emitter = dComIfGp_particle_getEmitter(field_0x31b0[0]);
        if (emitter != NULL) {
            alpha = emitter->getGlobalAlpha();
            if (alpha >= 51) {
                alpha -= (u8)51;
            } else {
                alpha = 0;
            }
        } else {
            alpha = 0;
        }
    }

    if (alpha != 0) {
        Vec pos;
        mDoMtx_multVecZero(mpLinkModel->getAnmMtx(37), &pos);

        for (int i = 0; i < 3; i++) {
            emitter = setEmitter(&field_0x31b0[i], effID[i], (cXyz*)&pos, &shape_angle);
            if (emitter != NULL) {
                emitter->setGlobalAlpha(alpha);
            }
        }
    }
}

void daAlink_c::setWolfJumpAttackEffect() {
    cXyz pos;

    JPABaseEmitter* emitter;
    u8 alpha;
    if (mProcID == PROC_WOLF_JUMP_ATTACK && mProcVar4.field_0x3010 != 0) {
        mDoMtx_multVecZero(mpLinkModel->getAnmMtx(3), &pos);
        setEmitter(&field_0x3288, ID_ZI_J_WL_ATTACKA_A, &pos, &shape_angle);
        setEmitter(&field_0x328c, ID_ZI_J_WL_ATTACKA_B, &pos, &shape_angle);
        alpha = 0xFF;
    } else {
        stopDrawParticle(field_0x3288);
        stopDrawParticle(field_0x328c);

        emitter = dComIfGp_particle_getEmitter(field_0x3290);
        if (emitter != NULL) {
            alpha = emitter->getGlobalAlpha();
            if (alpha >= 51) {
                alpha -= 51;
            } else {
                alpha = 0;
            }
        } else {
            alpha = 0;
        }
    }

    if (alpha != 0) {
        mDoMtx_multVecZero(mpLinkModel->getAnmMtx(37), &pos);

        emitter = setEmitter(&field_0x3290, ID_ZI_J_WL_ATTACKBLURA_A, &pos, &shape_angle);
        if (emitter != NULL) {
            emitter->setGlobalAlpha(alpha);
        }
    }
}

void daAlink_c::setWolfBarrierHitEffect(dBgS_LinChk& i_linchk) {
    static const u16 normalNameID[] = {
        dPa_RM(ID_ZI_S_NSBARRIER_COLL_A),
        dPa_RM(ID_ZI_S_NSBARRIER_COLL_B),
    };
    static const u16 ganonNameID[] = {
        dPa_RM(ID_ZI_S_GW_COLL_A),
        dPa_RM(ID_ZI_S_GW_COLL_B),
    };

    cM3dGPla tripla;
    dComIfG_Bgsp().GetTriPla(i_linchk, &tripla);

    field_0x3102 = cM_atan2s(-tripla.mNormal.x, -tripla.mNormal.z);

    csXyz rot(cM_atan2s(tripla.mNormal.y, tripla.mNormal.absXZ()), field_0x3102, 0);

    const u16* effNames;
    s16 movebg_name = getMoveBGActorName(i_linchk, TRUE);
    if (movebg_name == PROC_Obj_GanonWall2 || movebg_name == PROC_OBJ_GB) {
        effNames = ganonNameID;
    } else {
        effNames = normalNameID;
    }

    for (int i = 0; i < 2; i++, effNames++) {
        dComIfGp_particle_set(*effNames, i_linchk.GetCrossP(), &rot, NULL);
    }

    mDoAud_seStart(Z2SE_OBJ_BARRIER_COLLISION, i_linchk.GetCrossP(), 0, 0);
    field_0x3100 = 3;
}

void daAlink_c::setCutWaterDropEffect() {
    if (field_0x32c0[0] > 0) {
        Vec pos;
        mDoMtx_multVecZero(mpLinkModel->getAnmMtx(1), &pos);
        dComIfGp_particle_setColor(ID_ZI_J_LK_NURECUT_BACKBONE1, (cXyz*)&pos, &tevStr, NULL, NULL, 0.0f, -1, &shape_angle, NULL,
                                   NULL, -1, NULL);
    }
}

void daAlink_c::setWaterDropEffect() {
    static const u16 effDataTable[] = {
        ID_ZI_J_LK_NUREPOTA_BACKBONE2,
        ID_ZI_J_LK_NUREPOTA_WAIST,
        ID_ZI_J_WL_NUREPOTA_BACKBONE1,
        ID_ZI_J_WL_NUREPOTA_BACKBONE2,
    };

    Vec spC;
    int i;

    if (checkEndResetFlg1(daPy_ERFLG1(ERFLG1_WATER_DROP | ERFLG1_UNK_1000 | ERFLG1_WATERFALL_FRONT_HIT))) {
        for (i = 0; i < 2; i++) {
            field_0x32c0[i] = -1;
        }
    } else if (!checkNoResetFlg0(FLG0_UNK_80) || field_0x2fbc == 6) {
        for (i = 0; i < 2; i++) {
            if (field_0x32c0[i] > 0) {
                field_0x32c0[i]--;
            } else if (field_0x32c0[i] == -1) {
                field_0x32c0[i] = 150;
            }
        }
    } else {
        for (i = 0; i < 2; i++) {
            mDoMtx_multVecZero(mpLinkModel->getAnmMtx(field_0x32c4[i]), &spC);

            if (spC.y < mWaterY - 5.0f) {
                field_0x32c0[i] = -1;
            } else if (spC.y > mWaterY && field_0x32c0[i] == -1) {
                field_0x32c0[i] = 150;
            } else if (field_0x32c0[i] > 0) {
                field_0x32c0[i]--;
            }
        }
    }

    const u16* effNames;
    if (checkWolf()) {
        effNames = &effDataTable[2];
    } else {
        effNames = &effDataTable[0];
    }
    
    JPABaseEmitter* emitter;
    s16 color;
    for (i = 0; i < 2; i++, effNames++) {
        if (field_0x32c0[i] > 0) {
            emitter = setEmitterColor(&field_0x3298[i], *effNames, &current.pos, NULL);
            if (emitter != NULL) {
                emitter->setGlobalRTMatrix(mpLinkModel->getAnmMtx(field_0x32c4[i]));

                if (checkPlayerNoDraw()) {
                    emitter->stopDrawParticle();
                } else {
                    emitter->playDrawParticle();
                }
            }
        }
    
        if (field_0x32c0[i] != 0) {
            if (field_0x32a0[i].a > -20) {
                field_0x32a0[i].a -= 2;

                if (field_0x32a0[i].a < -20) {
                    field_0x32a0[i].a = -20;
                }
            }
        } else if (field_0x32a0[i].a < 0) {
            field_0x32a0[i].a++;
        }

        if (!checkNoResetFlg2(daPy_FLG2(FLG2_UNK_100000 | FLG2_UNK_80000))) {
            color = field_0x32a0[i].a * 0.4f;
        } else {
            color = field_0x32a0[i].a * 0.65f;
        }

        field_0x32a0[i].r = color;
        field_0x32a0[i].g = color;
        field_0x32a0[i].b = color;
    }
}

void daAlink_c::setSwordUpColorEffect() {
    f32 max_intensity;
    if (mSwordUpTimer != 0) {
        max_intensity = 1.0f;
    } else {
        max_intensity = 0.0f;
    }
    cLib_chaseF(&mSwordUpColorIntensity, max_intensity, 0.1f);

    for (int i = 0; i < 2; i++) {
        static GXColorS10 const swordUpColor[2] = {{13, 10, 2, 255}, {28, 16, 4, 255}};
        field_0x32b0[i].r = mSwordUpColorIntensity * swordUpColor[i].r;
        field_0x32b0[i].g = mSwordUpColorIntensity * swordUpColor[i].g;
        field_0x32b0[i].b = mSwordUpColorIntensity * swordUpColor[i].b;
    }
}

void daAlink_c::setSwordCutSplash() {
    static const u16 cutSplashName[] = {
        ID_ZI_J_DOWNWTRA_A,
        ID_ZI_J_DOWNWTRA_B,
        ID_ZI_J_DOWNWTRA_C,
        ID_ZI_J_DOWNWTRA_D,
    };

    static Vec const swordCutSplashScale = {0.6f, 0.6f, 0.6f};

    if (mCutType != 0 && mProcID != PROC_CUT_TURN && getCutAtFlg() && checkNoResetFlg0(FLG0_UNK_80) && mEquipItem == 0x103) {
        if (mSwordTopPos.y <= mWaterY && field_0x34b0.y > mWaterY) {
            cXyz sp14(mSwordTopPos.x, mWaterY, mSwordTopPos.z);

            for (int i = 0; i < 4; i++) {
                JPABaseEmitter* emitter = setEmitterPolyColor(&field_0x31e8[i], cutSplashName[i], mLinkAcch.m_wtr, &sp14, NULL);
                if (emitter != NULL) {
                    emitter->setRateStep(5);
                    emitter->setGlobalScale(swordCutSplashScale);     

                    if (cutSplashName[i] == ID_ZI_J_DOWNWTRA_B) {
                        emitter->setRate(5.0f);
                    } else if (cutSplashName[i] == ID_ZI_J_DOWNWTRA_D) {
                        emitter->setDirectionalSpeed(5.0f);
                    }
                }
            }
        }
    }
}

void daAlink_c::setMetamorphoseEffectStartLink() {
    setEmitter(&field_0x31f8, ID_ZI_J_ATOW_A, &field_0x37c8, NULL);
    setEmitter(&field_0x31fc, ID_ZI_J_ATOW_B, &field_0x37c8, NULL);
    JPABaseEmitter* emitter = setEmitter(&field_0x3200, ID_ZI_J_ATOW_C, &cXyz::Zero, NULL);

    #if VERSION == VERSION_SHIELD_DEBUG
    static Vec effScale = {1.33f, 1.0f, 1.0f};
    if (emitter != NULL) {
        emitter->setGlobalParticleScale(effScale);
    }
    #endif
}

void daAlink_c::setMetamorphoseEffect() {
    JPABaseEmitter* emitter;
    if (mProcVar1.field_0x300a == 0) {
        if (mProcVar5.field_0x3012 != 0) {
            if (checkWolf()) {
                setMetamorphoseEffectStartLink();
            } else {
                setEmitter(&field_0x31fc, ID_ZI_J_WTOA_A, &field_0x37c8, NULL);
            }
        } else if (checkWolf()) {
            emitter = setEmitter(&field_0x31f8, ID_ZI_J_WTOA_B, &current.pos, NULL);
            mDoMtx_multVecZero(mpLinkModel->getAnmMtx(2), &field_0x37c8);

            if (emitter != NULL) {
                emitter->setGlobalRTMatrix(mpLinkModel->getAnmMtx(2));
            }
        } else {
            mDoMtx_multVecZero(mpLinkModel->getAnmMtx(2), &field_0x37c8);
            setMetamorphoseEffectStartLink();
        }
    }
}

void daAlink_c::setRunSplash() {
    JPABaseEmitter* emitter;

    if (mProcID == PROC_MOVE && checkNoResetFlg0(FLG0_UNK_80) && checkDashAnime()) {
        f32 temp_f31 = mWaterY - current.pos.y;
        if (temp_f31 >= mpHIO->mBasic.m.mWaterSurfaceEffectHeight && temp_f31 < mHeight && dKy_pol_efftype_get(&mLinkAcch.m_wtr) == 2) {
            cXyz pos(current.pos.x, mWaterY, current.pos.z);

            emitter = setEmitterPolyColor(&field_0x31f8, ID_ZI_J_DASHWTRA_A, mLinkAcch.m_wtr, &pos, &current.angle);
            if (emitter != NULL) {
                emitter->setLifeTime(10);
                emitter->setDirectionalSpeed(5.0f);
            }

            pos.set(current.pos.x + (70.0f * cM_ssin(shape_angle.y)), 20.0f + mWaterY, current.pos.z + (70.0f * cM_scos(shape_angle.y)));
            setEmitterPolyColor(&field_0x31fc, ID_ZI_J_DASHWTRA_B, mLinkAcch.m_wtr, &pos, &current.angle);
            pos.y = mWaterY;

            emitter = setEmitterPolyColor(&field_0x3200, ID_ZI_J_DASHWTRA_C, mLinkAcch.m_wtr, &pos, &current.angle);
            if (emitter != NULL) {
                emitter->setLifeTime(20);
                emitter->setDirectionalSpeed(4.5f);
            }
        }
    }
}

void dAlink_bottleWaterPcallBack_c::execute(JPABaseEmitter* i_emitter, JPABaseParticle* i_particle) {
    UNUSED(i_emitter);

    JGeometry::TVec3<f32> sp18;
    i_particle->getGlobalPosition(&sp18);

    if (sp18.y < mKeepMinY) {
        mKeepMinY = sp18.y;
    }

    cXyz spC(sp18.x, daAlink_getAlinkActorClass()->getHeadTopPosP()->y, sp18.z);

    BOOL var_r29 = FALSE;
    BOOL var_r30 = FALSE;

    if (fopAcM_gc_c::gndCheck(&spC)) {
        if (sp18.y < fopAcM_gc_c::getGroundY()) {
            mHitFlg = 1;
            mHitPos.set(spC.x, fopAcM_gc_c::getGroundY(), spC.z);

            var_r29 = TRUE;
            var_r30 = TRUE;
        }
    }

    if (fopAcM_wt_c::waterCheck(&spC)) {
        if (sp18.y < fopAcM_wt_c::getWaterY() && (!var_r29 || mHitPos.y < fopAcM_wt_c::getWaterY())) {
            mHitFlg = 1;
            mHitPos.set(spC.x, fopAcM_wt_c::getWaterY(), spC.z);
            var_r30 = TRUE;
        }
    }

    if (var_r30) {
        i_particle->setInvisibleParticleFlag();
        i_particle->setDeleteParticleFlag();
    }
}

void daAlink_c::resetFairyEffect() {
    stopDrawParticle(field_0x3258);
    stopDrawParticle(field_0x325c);
}

void daAlink_c::setBottleEffect() {
    static Vec const fairyScale = {0.5f, 0.5f, 0.5f};
    static Vec const chuchuLocalOffset = {0.0f, 15.0f, 0.0f};
    static Vec const chuchuScale = {0.8f, 0.8f, 0.8f};

    JPABaseEmitter* emitter;

    if (mEquipItem == fpcNm_ITEM_FAIRY) {
        if (mpHookTipModel != NULL) {
            cXyz pos;
            mDoMtx_multVecZero(mpHookTipModel->getAnmMtx(2), &pos);

            emitter = setEmitter(&field_0x3258, ID_ZF_J_FAIRY00_GLOW, &pos, NULL);
            if (emitter != NULL) {
                emitter->setGlobalScale(fairyScale);
            }

            emitter = setEmitter(&field_0x325c, ID_ZF_J_FAIRYITEM02_STAR, &pos, NULL);
        }
    } else if (mEquipItem == fpcNm_ITEM_CHUCHU_RARE && mProcID == PROC_BOTTLE_GET) {
        if (field_0x072c != NULL && field_0x072c->getFrame() < 2.0f) {
            cXyz pos;
            mDoMtx_multVec(mHeldItemModel->getAnmMtx(0), &chuchuLocalOffset, &pos);
    
            emitter = setEmitter(&field_0x3258, ID_ZI_J_KIRAKIRA_A, &pos, NULL);
            if (emitter != NULL) {
                emitter->setGlobalScale(chuchuScale);
            }
        }
    } else if (mProcID == PROC_BOTTLE_OPEN && mProcVar3.field_0x300e != 0 && mEquipItem == fpcNm_ITEM_WATER_BOTTLE) {
        emitter = setEmitter(&field_0x325c, ID_ZI_J_LK_BINWATERN_A, &current.pos, NULL);
        if (emitter != NULL) {
            emitter->setGlobalRTMatrix(mHeldItemModel->getBaseTRMtx());
            emitter->setParticleCallBackPtr(&field_0x27c8);
        }

        if (!field_0x27c8.getAppearFlg() && field_0x27c8.getHitFlg()) {
            dComIfGp_particle_set(ID_ZI_J_LK_BINWATERN_B, &field_0x27c8.getHitPos(), NULL, NULL);
            dComIfGp_particle_set(ID_ZI_J_LK_BINWATERN_C, &field_0x27c8.getHitPos(), NULL, NULL);
            field_0x27c8.onAppearFlg();
        }
    }
}

void daAlink_c::clearFirePointDamageEffect(int i_effNo) {
    firePointEff_c* fire_eff = &field_0x32d8[i_effNo];
    if (fire_eff->field_0x0 != 0) {
        stopDrawParticle(fire_eff->field_0x4);
        stopDrawParticle(fire_eff->field_0x8);
    }

    fire_eff->field_0x0 = 0;
    fire_eff->field_0x24 = cXyz::Zero;
}

void daAlink_c::initFirePointDamageEffectAll() {
    static u16 const effJoint[2][4] = {
        {7, 12, 16, 34},
        {3, 3, 3, 3},
    };

    static Vec const effOffset[2][4] = {
        {
            {-5.0f, 0.0f, 0.0f},
            {15.0f, 3.0f, 5.0f},
            {4.0f, -10.0f, 15.0f},
            {0.0f, -5.0f, -7.0f},
        },
        {
            {15.0f, 5.0f, -20.0f},
            {0.0f, -25.0f, 5.0f},
            {15.0f, -5.0f, 20.0f},
            {10.0f, -25.0f, -10.0f},
        }
    };

    firePointEff_c* eff = field_0x32d8;

    int var_r29;
    if (checkWolf()) {
        var_r29 = 1;
    } else {
        var_r29 = 0;
    }

    int i;
    for (i = 0; i < 4; i++, eff++) {
        eff->field_0x0 = 1;
        eff->field_0x2 = effJoint[var_r29][i];
        eff->field_0x4 = 0;
        eff->field_0x8 = 0;
        eff->field_0x24 = cXyz::Zero;
        eff->field_0x18 = effOffset[var_r29][i];

        mDoMtx_multVec(mpLinkModel->getAnmMtx(eff->field_0x2), &eff->field_0x18, &eff->field_0xc);
    }
}

void daAlink_c::initFirePointDamageEffect(cXyz const* param_0, dCcD_GObjInf* i_hitObj) {
    firePointEff_c* eff = field_0x32d8;

    int i;
    for (i = 0; i < 4; i++, eff++) {
        if (eff->field_0x0 == 0) {
            break;
        }
    }

    if (i != 4) {
        cXyz* dmg_vec = getDamageVec(i_hitObj);

        csXyz sp38(dmg_vec->atan2sY_XZ(), dmg_vec->atan2sX_Z(), 0);
        csXyz sp40;
        cXyz sp30;

        int var_r30 = field_0x2e44.getHitmarkPosAndAngle(param_0, &sp38, &sp30, &sp40, 0);
        if (var_r30 == -1 || (!checkWolf() && (var_r30 == 1 || var_r30 == 15 || var_r30 == 16))) {
            return;
        }

        eff->field_0x2 = field_0x2e44.getJntNum(var_r30);
        eff->field_0xc = *param_0;
        eff->field_0x24 = cXyz::Zero;
        eff->field_0x0 = 1;
        eff->field_0x4 = 0;

        mDoMtx_stack_c::copy(mpLinkModel->getAnmMtx(eff->field_0x2));
        mDoMtx_stack_c::inverse();
        mDoMtx_stack_c::multVec(&sp30, &eff->field_0x18);
    }
}

void daAlink_c::setFirePointDamageEffect() {
    firePointEff_c* fireEff = field_0x32d8;
    cXyz pos;

    BOOL var_r25 = FALSE;

    JPABaseEmitter* emitter;
    for (int i = 0; i < 4; i++, fireEff++) {
        if (fireEff->field_0x0 != 0) {
            mDoMtx_multVec(mpLinkModel->getAnmMtx(fireEff->field_0x2), &fireEff->field_0x18, &pos);

            if (mWaterY > pos.y) {
                clearFirePointDamageEffect(i);
            } else {
                BOOL var_r26;
                if (fireEff->field_0x4 == 0) {
                    fireEff->field_0x24 = cXyz::Zero;
                    var_r26 = TRUE;
                } else {
                    fireEff->field_0x24 = (pos - fireEff->field_0xc) * 0.9f;
                    var_r26 = FALSE;
                }

                emitter = setEmitter(&fireEff->field_0x4, ID_ZI_J_LK_BURNS_A, &pos, NULL);
                if (emitter != NULL && emitter->isEnableDeleteEmitter()) {
                    clearFirePointDamageEffect(i);
                } else {
                    if (var_r26 && emitter != NULL) {
                        emitter->setParticleCallBackPtr(dPa_control_c::getParticleTracePCB());
                        emitter->setUserWork((uintptr_t)&fireEff->field_0x24);
                    }

                    emitter = setEmitter(&fireEff->field_0x8, ID_ZI_J_LK_BURNS_B, &pos, NULL);
                    if (var_r26 && emitter != NULL) {
                        emitter->setParticleCallBackPtr(dPa_control_c::getParticleTracePCB());
                        emitter->setUserWork((uintptr_t)&fireEff->field_0x24);
                    }

                    fireEff->field_0xc = pos;
                    var_r25 = 1;
                }
            }
        }
    }

    if (var_r25 || field_0x2fcb != 0) {
        seStartOnlyReverbLevel(Z2SE_AL_BURNING);
    }
}

void daAlink_c::setFreezeEffect() {
    static Vec const effScale = {1.0f, 1.8f, 1.0f};
    static Vec const wolfEffScale = {1.0f, 1.0f, 1.5f};
    static Vec const offsetPos = {0.0f, 0.0f, -10.0f};    

    cXyz pos;

    if (checkFreezeDamage()) {
        if (mProcID == PROC_DAMAGE || mProcID == PROC_SWIM_FREEZE_RETURN || mProcID == PROC_WOLF_DAMAGE) {
            const Vec* peffScale;
            if (checkWolf()) {
                mDoMtx_multVec(mpLinkModel->getAnmMtx(2), &offsetPos, &pos);
                peffScale = &wolfEffScale;
            } else {
                mDoMtx_multVecZero(mpLinkModel->getAnmMtx(1), &pos);
                peffScale = &effScale;
            }

            JPABaseEmitter* emitter = setEmitter(&field_0x3268, dPa_RM(ID_ZI_S_LK_FREEZ_A), &pos, &shape_angle);
            if (emitter != NULL) {
                emitter->setLocalScale(*peffScale);
            }
        }

        onNoResetFlg3(FLG3_UNK_800);
    } else if (checkNoResetFlg3(FLG3_UNK_800)) {
        if (checkWolf()) {
            mDoMtx_multVecZero(mpLinkModel->getAnmMtx(2), &pos);
        } else {
            mDoMtx_multVecZero(mpLinkModel->getAnmMtx(1), &pos);
        }

        for (int i = 0; i < 2; i++) {
            static const u16 effName[] = {
                dPa_RM(ID_ZI_S_LK_FREEZEND_A),
                dPa_RM(ID_ZI_S_LK_FREEZEND_B),
            };
            dComIfGp_particle_setColor(effName[i], &pos, &tevStr, NULL, NULL, 0.0f, 0xFF);
        }

        stopDrawParticle(field_0x3268);
        offNoResetFlg3(FLG3_UNK_800);
    }
}

void daAlink_c::setWoodShieldBurnEffect() {
    static const u16 effName[] = {
        ID_ZI_J_LK_SH_BURN_A,
        ID_ZI_J_LK_SH_BURN_B,
    };

    if (field_0x2fcb != 0) {
        for (int i = 0; i < 2; i++) {
            JPABaseEmitter* emitter =
                setEmitter(&field_0x3260[i], effName[i], &current.pos, NULL);

            if (emitter != NULL) {
                emitter->setGlobalRTMatrix(mShieldModel->getBaseTRMtx());
            }
        }
    }
}

void daAlink_c::clearWoodShieldBurnEffect() {
    if (field_0x2fcb != 0) {
        for (int i = 0; i < 2; i++) {
            stopDrawParticle(field_0x3260[i]);
        }

        field_0x2fcb = 0;
    }
}

void daAlink_c::setWoodShieldBurnOutEffect() {
    static const u16 effName[] = {
        ID_ZI_J_LK_SH_BURN_C,
        ID_ZI_J_LK_SH_BURN_D,
    };

    for (int i = 0; i < 2; i++) {
        JPABaseEmitter* emitter = dComIfGp_particle_setColor(effName[i], &current.pos,
                                                             &tevStr, NULL, NULL, 0.0f, -1);

        if (emitter != NULL) {
            emitter->setGlobalRTMatrix(mShieldModel->getBaseTRMtx());
        }
    }

    field_0x2fcb = 1;
    clearWoodShieldBurnEffect();
}

void daAlink_blur_c::initBlur(f32 param_0, int param_1, cXyz const* param_2, cXyz const* param_3,
                              cXyz const* param_4) {
    field_0x24 = param_0;
    field_0x1c = param_1;
    field_0x2c = *param_4;
    field_0x38[0] = *param_2;
    field_0x308[0] = *param_3;
    field_0x38[1] = field_0x38[0];
    field_0x308[1] = field_0x308[0];
    field_0x14 = 0;
}

static void setBezierPos(cXyz const* start, cXyz const* control_1, cXyz const* control_2,
                         cXyz const* end, f32 t, cXyz* out_pos) {
    f32 one_minus_t;
    f32 one_minus_t_squared;
    f32 weight_1;
    f32 weight_2;
    f32 t_squared;

    one_minus_t = 1.0f - t;
    one_minus_t_squared = one_minus_t * one_minus_t;
    t_squared = t * t;

    weight_1 = (one_minus_t_squared * 3.0f) * t;
    weight_2 = (t_squared * 3.0f) * one_minus_t;

    one_minus_t_squared *= one_minus_t;
    t_squared *= t;

    *out_pos = (((*start * one_minus_t_squared) + (*control_2 * weight_1)) + (*end * weight_2)) +
               (*control_1 * t_squared);
}

void daAlink_blur_c::copyBlur(cXyz const* param_0, cXyz const* param_1, cXyz const* param_2) {
    s32 var_r29 = 10;

    int i;
    for (i = 59 - var_r29; i >= 0; i--) {
        field_0x38[i + var_r29] = field_0x38[i];
        field_0x308[i + var_r29] = field_0x308[i];
    }

    f32 temp_f30 = 1.0f / var_r29;
    f32 var_f31 = 0.0f;

    cXyz sp4C;
    cXyz sp58;
    cXyz sp64;
    cXyz sp70;

    sp58 = field_0x308[var_r29] + (field_0x2c * 30.0f);
    sp4C = *param_1 + (*param_2 * -30.0f);
    sp70 = field_0x38[var_r29] + (field_0x2c * 60.0f);
    sp64 = *param_0 + (*param_2 * -60.0f);

    for (i = 0; i < var_r29; i++) {
        setBezierPos(param_1, &field_0x308[var_r29], &sp4C, &sp58, var_f31, &field_0x308[i]);
        setBezierPos(param_0, &field_0x38[var_r29], &sp64, &sp70, var_f31, &field_0x38[i]);

        field_0x38[i] += (field_0x38[i] - field_0x308[i]) * field_0x24;
        var_f31 += temp_f30;
    }

    field_0x2c = *param_2;

    field_0x14 += var_r29;
    if (field_0x14 >= 59) {
        field_0x14 = 58;
    }
}

void daAlink_blur_c::traceBlur(cXyz const* param_0, cXyz const* param_1, s16 param_2) {
    mDoMtx_stack_c::transS(*param_0);
    mDoMtx_stack_c::YrotM(param_2);
    mDoMtx_stack_c::transM(-param_1->x, -param_1->y, -param_1->z);

    cXyz sp20;
    for (int i = 0; i < field_0x14; i++) {
        sp20 = field_0x38[i];
        mDoMtx_stack_c::multVec(&sp20, &field_0x38[i]);

        sp20 = field_0x308[i];
        mDoMtx_stack_c::multVec(&sp20, &field_0x308[i]);
    }
}

void daAlink_blur_c::draw() {
    j3dSys.reinitGX();

    static GXTexObj texObj;
    static GXColor nColor0 = {0xFF, 0xFF, 0xFF, 0x14};

    GXSetNumIndStages(0);
    nColor0.a = field_0x20;

    GXInitTexObj(&texObj, (void*)((uintptr_t)m_blurTex + m_blurTex->imageOffset), 16, 4, GX_TF_I4,
                 GX_CLAMP, GX_CLAMP, GX_FALSE);
    GXInitTexObjLOD(&texObj, GX_LINEAR, GX_LINEAR, 0.0f, 0.0f, 0.0f, GX_FALSE, GX_FALSE,
                    GX_ANISO_1);
    GXLoadTexObj(&texObj, GX_TEXMAP0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_CLR_RGBA, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_CLR_RGBA, GX_RGBA4, 8);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetNumChans(0);
    GXSetTevColor(GX_TEVREG0, nColor0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x3C);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_C0);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_A0, GX_CA_TEXA, GX_CA_ZERO);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
    GXLoadPosMtxImm(j3dSys.getViewMtx(), GX_PNMTX0);
    GXSetCurrentMtx(0);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_SET);
    GXSetCullMode(GX_CULL_NONE);
    GXSetZMode(GX_ENABLE, GX_LEQUAL, GX_DISABLE);
    GXSetAlphaCompare(GX_GREATER, 0, GX_AOP_OR, GX_GREATER, 0);

    int temp_r26 = 0xFF / ((field_0x14 >> 1) + 1);
    s16 var_r28 = temp_r26;
    s16 var_r27 = 0;

    GXBegin(GX_QUADS, GX_VTXFMT0, (field_0x14 * 4) + 4);

    for (int i = field_0x14; i >= 0; i--) {
        GXPosition3f32(field_0x38[i].x, field_0x38[i].y, field_0x38[i].z);
        GXTexCoord2s16(var_r28, 0);

        GXPosition3f32(field_0x308[i].x, field_0x308[i].y, field_0x308[i].z);
        GXTexCoord2s16(var_r28, 0xFF);

        GXPosition3f32(field_0x308[i + 1].x, field_0x308[i + 1].y, field_0x308[i + 1].z);
        GXTexCoord2s16(var_r27, 0xFF);

        GXPosition3f32(field_0x38[i + 1].x, field_0x38[i + 1].y, field_0x38[i + 1].z);
        GXTexCoord2s16(var_r27, 0);

        var_r27 = var_r28;
        var_r28 += temp_r26;
    }

    GXEnd();
    J3DShape::resetVcdVatCache();
}

BOOL daAlink_lockCursor_c::create() {
    JKRArchive* arc = dComIfG_getObjectResInfo(l_arcName)->getArchive();

    mLockCursorAngle = 0.0f;
    field_0x4 = false;
    field_0x30 = 0.0f;

    mScrn = new J2DScreen();
    if (mScrn == NULL) {
        return false;
    }

    mScrn->setPriority("zelda_v_cursor_new_yellow.blo", 0x100000, arc);
    dPaneClass_showNullPane(mScrn);

    void* tmpData = JKRGetNameResource("zelda_v_cursor_new_yellow.bpk", arc);
    JUT_ASSERT(3450, tmpData);
    field_0x24 = (J2DAnmColorKey*)J2DAnmLoaderDataBase::load(tmpData);
    if (field_0x24 == NULL) {
        return false;
    }

    tmpData = JKRGetNameResource("zelda_v_cursor_new_yellow.bck", arc);
    JUT_ASSERT(3458, tmpData);
    field_0x28 = (J2DAnmTransformKey*)J2DAnmLoaderDataBase::load(tmpData);
    if (field_0x28 == NULL) {
        return false;
    }

    tmpData = JKRGetNameResource("zelda_v_cursor_new_yellow_02.brk", arc);
    JUT_ASSERT(3467, tmpData);
    field_0x1c = (J2DAnmTevRegKey*)J2DAnmLoaderDataBase::load(tmpData);
    if (field_0x1c == NULL) {
        return false;
    }

    tmpData = JKRGetNameResource("zelda_v_cursor_new_yellow.brk", arc);
    JUT_ASSERT(3475, tmpData);
    field_0x20 = (J2DAnmTevRegKey*)J2DAnmLoaderDataBase::load(tmpData);
    if (field_0x20 == NULL) {
        return false;
    }

    field_0xc = mScrn->search('n_all');
    mCursor0 = mScrn->search('cursor0');
    mCursor1 = mScrn->search('cursor1');
    mCursor2 = mScrn->search('cursor2');

    field_0x28->searchUpdateMaterialID(mScrn);
    field_0xc->setAnimation(field_0x28);
    mCursor0->setAnimation(field_0x28);
    mCursor1->setAnimation(field_0x28);
    mCursor2->setAnimation(field_0x28);
    field_0x28->setFrame(0.0f);

    field_0x24->searchUpdateMaterialID(mScrn);
    mCursor0->setAnimation(field_0x24);
    mCursor1->setAnimation(field_0x24);
    mCursor2->setAnimation(field_0x24);
    J2DPane* sp8 = mScrn->search('flash');
    sp8->setAnimation(field_0x24);
    field_0x24->setFrame(0.0f);

    field_0x1c->searchUpdateMaterialID(mScrn);
    field_0x20->searchUpdateMaterialID(mScrn);
    mCursor0->setAnimation(field_0x1c);
    mCursor1->setAnimation(field_0x1c);
    mCursor2->setAnimation(field_0x1c);
    mCursor0->setAnimation(field_0x20);
    mCursor1->setAnimation(field_0x20);
    mCursor2->setAnimation(field_0x20);
    field_0x1c->setFrame(0.0f);
    field_0x20->setFrame(0.0f);

    mScrn->animation();
    mScrn->setUserInfo('n_43');
    field_0xc->setUserInfo(0x20);
    return true;
}

// void J2DPane::setAnimation(J2DAnmTevRegKey* param_0) {
extern "C" void setAnimation__7J2DPaneFP15J2DAnmTevRegKey() {
    /* empty function */
}

// void J2DPane::setAnimation(J2DAnmColor* param_0) {
extern "C" void setAnimation__7J2DPaneFP11J2DAnmColor() {
    /* empty function */
}

void daAlink_lockCursor_c::update() {
    if (mLockCursorAngle < 21.0f) {
        mLockCursorAngle = 21.0f;
    }

    mLockCursorAngle += 0.9f;

    if (mLockCursorAngle >= 50.0f) {
        mLockCursorAngle += -29.0f;
    }

    field_0x30 += 0.9f;

    if (field_0x30 >= field_0x1c->getFrameMax()) {
        field_0x30 -= field_0x1c->getFrameMax();
    }

    cLib_chaseUC(&field_0x4, 0x80, 30);
}

void daAlink_lockCursor_c::draw() {
    if (field_0x4 == 0) {
        return;
    }

    field_0x28->setFrame(mLockCursorAngle);

    f32 var_f30;
    if (mLockCursorAngle > 21.0f) {
        var_f30 = 21.0f;
    } else {
        var_f30 = mLockCursorAngle;
    }

    field_0x24->setFrame(var_f30);
    field_0x1c->setFrame(field_0x30);

    mScrn->animation();
    field_0xc->scale(0.6f, 0.6f);
    field_0xc->translate(mPosX, mPosY);

    f32 var_f31;
    if (!(mLockCursorAngle < 15.0f)) {
        if (mLockCursorAngle < 21.0f) {
            var_f31 = ((mLockCursorAngle - 15.0f) * 40.0f) * 0.16666667f;
        } else {
            var_f31 = ((field_0x4 * 0.00390625f) + 0.5f) * 40.0f;
        }

        mCursor0->translate(0.0f, -var_f31);
        mCursor1->translate(var_f31 * cM_fcos(2.6179938f), var_f31 * cM_fsin(2.6179938f));
        mCursor2->translate(var_f31 * cM_fcos(0.5235988f), var_f31 * cM_fsin(0.5235988f));
    }

    mScrn->draw(0.0f, 0.0f, dComIfGp_getCurrentGrafPort());
}

BOOL daAlink_sight_c::create() {
    if (!mLockCursor.create()) {
        return false;
    }

    setSightImage((ResTIMG*)dComIfG_getObjectRes(l_arcName, daAlink_c::getSightBti()));
    return true;
}

void daAlink_sight_c::draw() {
    if (mLockFlag) {
        mLockCursor.setPos(mProjMtx[0][3], mProjMtx[1][3]);
        mLockCursor.draw();
    } else {
        daPy_sightPacket_c::draw();
    }
}

void daAlink_sight_c::onLockFlg() {
    if (mLockFlag) {
        return;
    }
    mLockFlag = true;
    mLockCursor.initFrame();
}
