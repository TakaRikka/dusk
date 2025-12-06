/**
 * d_a_alink_link.inc
 * Player Swim action handling
 */

#include "d/actor/d_a_alink.h"
#include "d/d_com_inf_game.h"
#include "f_op/f_op_kankyo_mng.h"

BOOL daAlink_c::checkZoraWearMaskDraw() {
    return checkZoraWearAbility() && !checkNoResetFlg0(FLG0_SWIM_UP);
}

BOOL daAlink_c::checkAcceptUseItemInWater(u16 i_itemNo) const {
    return i_itemNo == fpcNm_ITEM_HVY_BOOTS || i_itemNo == fpcNm_ITEM_WATER_BOMB || checkHookshotItem(i_itemNo);
}

void daAlink_c::swimDeleteItem() {
    if (!checkHookshotItem(mEquipItem) && (mEquipItem != 0x103 || !checkBootsOrArmorHeavy())) {
        deleteEquipItem(1, 1);
    }

    if (checkNoResetFlg2(FLG2_UNK_1)) {
        offKandelaarModel();
    }
}

bool daAlink_c::getZoraSwim() const {
    return checkUnderMove0BckNoArc(ANM_SWIM_C) || checkUnderMove0BckNoArc(ANM_SWIM_ZORA_START);
}

f32 daAlink_c::getSwimFrontMaxSpeed() const {
    if (checkBootsOrArmorHeavy()) {
        return mpHIO->mSwim.m.mUnderwaterFallMaxSpeed;
    } else if (checkZoraWearAbility()) {
        return 14.0f;
    } else if (checkSwimDashMode()) {
        return mpHIO->mSwim.m.mDashMaxSpeed;
    } else {
        return mpHIO->mSwim.m.mForwardMaxSpeed;
    }
}

f32 daAlink_c::getSwimMaxFallSpeed() const {
    if (mProcID == PROC_DIVE_JUMP) {
        return -mpHIO->mSwim.m.mUnderwaterMaxSpeed;
    } else if (checkBootsOrArmorHeavy()) {
        return mpHIO->mSwim.m.mBootsMaxFallSpeed;
    } else if (getZoraSwim()) {
        return mpHIO->mSwim.m.mMaxFallSpeed;
    } else {
        return mpHIO->mSwim.m.mStandingMaxFallSpeed;
    }
}

void daAlink_c::checkOxygenTimer() {
    BOOL hide_timer;

    if (!checkNoResetFlg0(FLG0_SWIM_UP) ||
        (checkModeFlg(MODE_SWIMMING) && mWaterY > 5.0f + current.pos.y))
    {
        hide_timer = false;
    } else {
        hide_timer = true;
    }

    if (dComIfGp_getOxygenShowFlag()) {
        if (checkZoraWearAbility()) {
            offOxygenTimer();
        } else if (hide_timer) {
            s32 max = dComIfGp_getMaxOxygen();
            dComIfGp_setOxygenCount(max);

            if (field_0x2fbe < 90) {
                field_0x2fbe++;
            } else {
                offOxygenTimer();
            }
        } else if (!checkEventRun()) {
            dComIfGp_setOxygenCount(-1);
        }
    } else if (!hide_timer && !checkZoraWearAbility()) {
        if (field_0x2fbe != 0) {
            field_0x2fbe--;
        } else {
            dComIfGp_onOxygenShowFlag();
            dComIfGp_setOxygen(dComIfGp_getMaxOxygen());
        }
    }
}

void daAlink_c::offOxygenTimer() {
    dComIfGp_offOxygenShowFlag();
    s32 max = dComIfGp_getMaxOxygen();
    dComIfGp_setOxygen(max);

    field_0x2fbe = 90;
}

BOOL daAlink_c::checkPossibleWaterInMode() const {
    return checkModeFlg(MODE_VINE_CLIMB) || dComIfGp_checkPlayerStatus1(0, 0x2000000) ||
           mProcID == PROC_HOOKSHOT_FLY;
}

void daAlink_c::setWaterInAnmRate(daPy_frameCtrl_c* i_frameCtrl, f32 i_rate) {
    if (checkNoResetFlg0(FLG0_UNDERWATER)) {
        if (checkZoraWearAbility()) {
            i_rate *= mpHIO->mItem.mIronBoots.m.mZoraWaterAnmSpeed;
        } else {
            i_rate *= mpHIO->mItem.mIronBoots.m.mWaterStartWalkAnmRate;
        }
    }

    i_frameCtrl->setRate(i_rate);
}

void daAlink_c::swimBgCheck(f32 param_0) {
    offNoResetFlg1(FLG1_UNK_20000000);

    if (!getZoraSwim() && mProcID != PROC_METAMORPHOSE) {
        return;
    }

    cXyz start_pos(current.pos);
    cXyz end_pos;

    s16 var_r29;
    if (getZoraSwim()) {
        var_r29 = field_0x3080;
    } else {
        var_r29 = mProcVar2.field_0x300c;
        start_pos.y += mpHIO->mWolf.mWlWallHang.m.mAutoWalkHeight;
    }

    f32 var_f28 = param_0 * cM_scos(var_r29);

    end_pos.set(start_pos.x + (var_f28 * cM_ssin(shape_angle.y)),
                start_pos.y - param_0 * cM_ssin(var_r29),
                start_pos.z + (var_f28 * cM_scos(shape_angle.y)));

    if (!commonLineCheck(&start_pos, &end_pos)) {
        return;
    }

    cM3dGPla tri;
    dComIfG_Bgsp().GetTriPla(mLinkLinChk, &tri);

    if (cBgW_CheckBWall(tri.mNormal.y) &&
        (mProcID != PROC_METAMORPHOSE || dComIfG_Bgsp().GetWallCode(mLinkLinChk) != 7))
    {
        cXyz sp14 = end_pos - mLinkLinChk.GetCross();
        f32 temp_f1 = sp14.absXZ();

        s16 var_r28 = sp14.atan2sX_Z();
        f32 var_f26 = cM_scos(var_r28 - cM_atan2s(-tri.mNormal.x, -tri.mNormal.z));
        f32 var_f29 = -temp_f1 * var_f26;

        f32 var_f31 = var_f29 * tri.mNormal.x;
        f32 var_f30 = var_f29 * tri.mNormal.z;

        if (mProcID == PROC_METAMORPHOSE ||
            (getZoraSwim() && cLib_distanceAngleS(cM_atan2s(tri.mNormal.y, tri.mNormal.absXZ()),
                                                  field_0x3080) < 0x1000))
        {
            onNoResetFlg1(FLG1_UNK_20000000);
        }

        start_pos.x -= var_f31;
        start_pos.z -= var_f30;
        end_pos.x -= var_f31;
        end_pos.z -= var_f30;

        if (commonLineCheck(&start_pos, &end_pos)) {
            dComIfG_Bgsp().GetTriPla(mLinkLinChk, &tri);

            if (cBgW_CheckBWall(tri.mNormal.y)) {
                sp14 = end_pos - mLinkLinChk.GetCross();
                temp_f1 = sp14.absXZ();

                var_r28 = sp14.atan2sX_Z();
                var_f26 = cM_scos(var_r28 - cM_atan2s(-tri.mNormal.x, -tri.mNormal.z));
                var_f29 = -temp_f1 * var_f26;

                var_f31 += var_f29 * tri.mNormal.x;
                var_f30 += var_f29 * tri.mNormal.z;
            }
        }

        current.pos.x -= var_f31;
        current.pos.z -= var_f30;
        field_0x3092 = cM_atan2s(-var_f31, -var_f30);
    }
}

void daAlink_c::setSpeedAndAngleSwim() {
    if (!checkBootsOrArmorHeavy() && dComIfGp_getDoStatus() == 0 && (!checkHookshotAnime() || (checkHookshotItem(mEquipItem) && (mItemMode == 0 || mItemMode == 1)))) {
        if (checkZoraWearAbility()) {
            if (checkNoResetFlg0(FLG0_SWIM_UP)) {
                if (field_0x2fcd == 0) {
                    setDoStatus(0x41);
                }
            } else {
                setDoStatus(0x4C);
            }
        } else if (!checkNoResetFlg0(FLG0_SWIM_UP)) {
            setDoStatus(0x97);
        } else if (mProcID == PROC_SWIM_MOVE && checkUnderMove0BckNoArc(ANM_SWIM_A) && field_0x30d0 == 0 && field_0x30d2 == 0) {
            setDoStatus(9);
        }
    }

    f32 var_f31;
    f32 var_f30;
    if (mProcID == PROC_SWIM_HOOKSHOT_MOVE && !checkHookshotWait()) {
        var_f30 = 0.0f;
    } else if (checkZoraSwimMove()) {
        s16 temp_r29 = shape_angle.y;

        if (mTargetedActor != NULL && current.pos.abs2XZ(mTargetedActor->eyePos) > 22500.0f) {
            setShapeAngleToAtnActor(0);
        } else if (checkInputOnR()) {
            s16 var_r28;
            if (checkEventRun()) {
                var_r28 = field_0x2fe2;
            } else {
                var_r28 = shape_angle.y + (16384.0f * cM_ssin(mStickAngle));
            }

            cLib_addCalcAngleS(&shape_angle.y, var_r28, mpHIO->mSwim.m.mUnderwaterTurnRate, mpHIO->mSwim.m.mUnderwaterMaxTurn, mpHIO->mSwim.m.mUnderwaterMinTurn);
        }

        current.angle.y = shape_angle.y;
        var_f30 = 3.0f * cM_scos((shape_angle.y - temp_r29));
    } else if (!checkAttentionLock()) {
        if (checkInputOnR()) {
            s16 temp_r29_2 = shape_angle.y;
            if (checkNoResetFlg0(FLG0_SWIM_UP)) {
                cLib_addCalcAngleS(&shape_angle.y, field_0x2fe2, mpHIO->mSwim.m.mTurnRate, mpHIO->mSwim.m.mMaxTurn, mpHIO->mSwim.m.mMinTurn);
            } else {
                cLib_addCalcAngleS(&shape_angle.y, field_0x2fe2, mpHIO->mSwim.m.mUnderwaterTurnRate, mpHIO->mSwim.m.mUnderwaterMaxTurn, mpHIO->mSwim.m.mUnderwaterMinTurn);
            }

            f32 var_f29;
            if (checkUnderMove0BckNoArc(ANM_SWIM_DASH)) {
                var_f29 = 1.0f;
            } else {
                var_f29 = field_0x33a8;
            }

            var_f30 = 3.0f * var_f29 * cM_scos((shape_angle.y - temp_r29_2));
            cLib_addCalcAngleS(&current.angle.y, shape_angle.y, 2, 0x2000, 0x1000);
        } else {
            var_f30 = 0.0f;
        }

        if (checkZeroSpeedF()) {
            current.angle.y = shape_angle.y;
        }
    } else {
        setShapeAngleToAtnActor(0);

        if (checkInputOnR()) {
            if (getDirectionFromCurrentAngle() == DIR_BACKWARD) {
                current.angle.y += 0x8000;
                mNormalSpeed *= -1.0f;
            }

            if (checkZeroSpeedF()) {
                current.angle.y = field_0x2fe2;
            }

            s16 temp_r29_3 = current.angle.y;
            if (checkNoResetFlg0(FLG0_SWIM_UP)) {
                cLib_addCalcAngleS(&current.angle.y, field_0x2fe2, mpHIO->mSwim.m.mTurnRate, mpHIO->mSwim.m.mMaxTurn, mpHIO->mSwim.m.mMinTurn);
            } else {
                cLib_addCalcAngleS(&current.angle.y, field_0x2fe2, mpHIO->mSwim.m.mUnderwaterTurnRate, mpHIO->mSwim.m.mUnderwaterMaxTurn, mpHIO->mSwim.m.mUnderwaterMinTurn);
            }

            var_f30 = mpHIO->mSwim.m.mAcceleration * field_0x33a8 * cM_scos((s16)(current.angle.y - temp_r29_3));
        } else {
            var_f30 = 0.0f;
        }
    }

    f32 var_f28;
    if (mpHIO->mSwim.m.mForwardMaxSpeed < mNormalSpeed && !checkZoraWearAbility()) {
        var_f28 = mpHIO->mSwim.m.mDashDeceleration;
    } else {
        var_f28 = mpHIO->mSwim.m.mDeceleration;
    }

    setNormalSpeedF(var_f30, var_f28);

    if (dComIfGp_checkPlayerStatus0(0, 0x10)) {
        mNormalSpeed = 0.0f;
    }

    if (checkSwimNeckUpDown()) {
        s16 sp14;
        if (speed.y >= 0.0f) {
            var_f31 = -speed.y / mpHIO->mSwim.m.mMaxFloatUpSpeed;
            if (var_f31 < -1.0f) {
                var_f31 = -1.0f;
            }
            sp14 = 6144.0f * var_f31;
        } else {
            var_f31 = speed.y / maxFallSpeed;
            if (var_f31 > 1.0f) {
                var_f31 = 1.0f;
            }
            sp14 = 8192.0f * var_f31;
        }

        cLib_addCalcAngleS(&field_0x30a0, sp14, 5, 0x1000, 0x400);
    } else {
        field_0x30a0 = 0;
    }
}

int daAlink_c::checkNextActionSwim() {
    if (checkSwimButtonMove() && getZoraSwim()) {
        field_0x3000 = 30;
    }

    if ((checkZoraWearAbility() && !checkHeavyStateOn(1, 1)) || checkNoResetFlg0(FLG0_SWIM_UP)) {
        setTalkStatus();
    }

    if (orderTalk(1)) {
        return 1;
    }

    if (mAttList != NULL && mAttList->mType == fopAc_attn_ETC_e && checkNoResetFlg0(FLG0_SWIM_UP) &&
        fopAcM_GetName(field_0x27f4) == PROC_CANOE)
    {
        setDoStatus(0x17);

        if (doTrigger()) {
            return procCanoeRideInit();
        }
    } else if (checkSwimButtonMove() && !checkZoraSwimMove()) {
        if (checkSwimButtonAccept()) {
            return procSwimDiveInit();
        }

        field_0x3000 = 30;
        return procSwimMoveInit();
    }

    return checkNextActionFromButton();
}

int daAlink_c::checkSwimAction(int param_0) {
    f32 var_f31;
    if (checkWolf()) {
        if (getMoveBGActorName(mLinkAcch.m_gnd, NULL) == PROC_Obj_ITA) {
            var_f31 = 200.0f;
        } else {
            var_f31 = mpHIO->mWolf.mWlSwim.m.mStartHeight;
        }
    } else {
        var_f31 = mpHIO->mSwim.m.mStartHeight;
    }

    if (checkNoResetFlg0(FLG0_UNK_80) && !checkNoResetFlg0(FLG0_UNDERWATER) && !checkModeFlg(0x40000) && mProcID != PROC_MAGNE_BOOTS_FLY && ((mProcID != PROC_WOLF_LOCK_ATTACK && mProcID != PROC_WOLF_LOCK_ATTACK_TURN) || mProcVar5.field_0x3012 == 0) && mDemo.getDemoMode() != 0x11) {
        f32 var_f28;
        f32 var_f29;
        f32 var_f30;
        
        if (checkWolf()) {
            if (checkWolfDashMode()) {
                var_f29 = mpHIO->mWolf.mWlSwim.m.mMaxSpeed;
            } else {
                var_f29 = mpHIO->mWolf.mWlSwim.m.mMaxSpeedWeak;
            }

            var_f28 = mpHIO->mWolf.mWlSwim.m.mMaxFallSpeed;
        } else {
            var_f29 = getSwimFrontMaxSpeed();
            var_f28 = getSwimMaxFallSpeed();
        }

        if (dComIfGp_checkPlayerStatus1(0, 0x10000)) {
            var_f30 = mLeftFootPos.y;
        } else if (dComIfGp_checkPlayerStatus1(0, 0x2000000)) {
            var_f30 = (-65.0f + current.pos.y) - var_f31;
        } else {
            var_f30 = current.pos.y;
        }

        if (mWaterY - var_f30 > var_f31 || (param_0 != 0 && mWaterY >= var_f30 && mWaterY - mLinkAcch.GetGroundH() > var_f31)) {
            cXyz sp14;
            offNoResetFlg0(FLG0_SWIM_UP);
            mZ2Link.setInWater(true);

            field_0x594 = var_f29;

            if (!param_0) {
                mNormalSpeed *= 0.75f;
            }

            if (mNormalSpeed >= field_0x594) {
                mNormalSpeed = field_0x594;
            }

            swimDeleteItem();

            if (checkEquipAnime()) {
                resetUpperAnime(UPPER_2, 3.0f);
            }

            field_0x3080 = 0;

            if (param_0) {
                if (current.pos.y - mWaterY > -5.0f) {
                    onNoResetFlg0(FLG0_SWIM_UP);
                    mZ2Link.setInWater(false);
                    return 0;
                }
                return 1;
            }

            if (checkModeFlg(2) && !checkPossibleWaterInMode()) {
                dComIfGp_getVibration().StartShock(2, 1, cXyz(0.0f, 1.0f, 0.0f));
                current.pos.y = var_f30 + var_f31;

                speed.y = 0.75f * field_0x3528.y;
                if (speed.y < var_f28) {
                    speed.y = var_f28;
                } else if (speed.y > 0.0f) {
                    speed.y = 0.0f;
                }

                if (checkWolf()) {
                    if (checkNoResetFlg2(FLG2_UNK_8)) {
                        daAlink_WANM var_r29;
                        if (cM_rnd() < 0.5f) {
                            var_r29 = WANM_FLING_LEFT;
                            onNoResetFlg2(FLG2_WOLF_ENEMY_LEFT_THROW);
                        } else {
                            var_r29 = WANM_FLING_RIGHT;
                            offNoResetFlg2(FLG2_WOLF_ENEMY_LEFT_THROW);
                        }
                        setWolfEnemyThrowUpperAnime(var_r29, 0.0f);
                    }

                    return procWolfSwimWaitInit(1);
                }

                if (mProcID == PROC_DIVE_JUMP) {
                    seStartOnlyReverb(Z2SE_AL_JUMP_DIVE_WATER);

                    if (checkZoraWearAbility()) {
                        field_0x3000 = 30;
                    }
                    return procSwimMoveInit();
                }

                return procSwimWaitInit(1);
            }

            if (checkBootsOrArmorHeavy() || checkPossibleWaterInMode()) {
                onNoResetFlg0(FLG0_UNDERWATER);
                return 0;
            }

            if (checkWolf()) {
                return procWolfSwimUpInit();
            }

            return procSwimUpInit();
        }
    } else if (!checkNoResetFlg0(FLG0_SWIM_UP) && !checkBootsOrArmorHeavy() && !checkPossibleWaterInMode() && !checkModeFlg(0x40000)) {
        if (checkNoResetFlg0(FLG0_UNK_80) && mWaterY - current.pos.y > var_f31) {
            if (checkWolf()) {
                return procWolfSwimWaitInit(0);
            }

            return procSwimWaitInit(0);
        }

        swimOutAfter(0);

        if (checkWolf()) {
            return procWolfWaitInit();
        }

        return procWaitInit();
    } else if (checkModeFlg(0x40000) && mProcID != PROC_CAUGHT && mProcID != PROC_DEAD) {
        return checkSwimOutAction();
    }

    return 0;
}

int daAlink_c::checkSwimUpAction() {
    if (checkNoResetFlg0(FLG0_SWIM_UP)) {
        if (!checkHookshotAnime()) {
            return checkSubjectAction();
        }
        return 0;
    }

    if (!getZoraSwim()) {
        if (checkWolf()) {
            field_0x3480 = mpHIO->mWolf.mWlSwim.m.mStartHeight;
        } else {
            cLib_addCalc(&field_0x3480, mpHIO->mSwim.m.mStartHeight, 0.5f, 15.0f, 1.0f);
        }
    } else {
        field_0x3480 = 65.0f;
    }

    f32 var_f31;
    if (checkWolf()) {
        var_f31 = mpHIO->mWolf.mWlSwim.m.mSurfacingHeight;
    } else {
        var_f31 = mpHIO->mSwim.m.mFloatUpHeight;
    }

    if (!checkBootsOrArmorHeavy() && !checkNoResetFlg0(FLG0_SWIM_UP)) {
        if (speed.y >= 0.0f && mWaterY - current.pos.y < var_f31) {
            if (checkWolf()) {
                return procWolfSwimUpInit();
            } else {
                return procSwimUpInit();
            }
        }
    }

    if (current.pos.y - field_0x3480 < mLinkAcch.GetGroundH()) {
        if (!checkBootsOrArmorHeavy()) {
            current.pos.y = field_0x3480 + mLinkAcch.GetGroundH();
            field_0x3798.y = current.pos.y;
            mProcVar3.field_0x300e = getGroundAngle(&mLinkAcch.m_gnd, shape_angle.y);
            mProcVar4.field_0x3010 = 2;
    
            if (speed.y < -1.0f) {
                speed.y = -1.0f;
            }
        } else {
            current.pos.y = mLinkAcch.GetGroundH();
            mLinkAcch.SetGroundHit();
            onNoResetFlg0(FLG0_UNDERWATER);
            procLandInit(0.0f);
            field_0x2f99 = 0x90;
            return 1;
        }
    } else if (mLinkAcch.ChkRoofHit()) {
        mProcVar3.field_0x300e = getRoofAngle(&mLinkAcch.m_roof, shape_angle.y);
        mProcVar4.field_0x3010 = 2;

        if (speed.y > 1.0f) {
            speed.y = 1.0f;
        }
    }

    return 0;
}

void daAlink_c::swimOutAfter(int param_0) {
    onNoResetFlg0(FLG0_SWIM_UP);
    onEndResetFlg1(ERFLG1_UNK_100);
    mZ2Link.setInWater(false);

    if (checkNoResetFlg0(FLG0_UNDERWATER)) {
        offNoResetFlg0(FLG0_UNDERWATER);
    } else {
        field_0x2b98 = 0.0f;
        field_0x2f98 = 4;
    }

    field_0x3750 = cXyz::Zero;
    field_0x3000 = 0;
    offOxygenTimer();

    if (checkSwimMoveHandAnime() || checkZoraSwimDamageAnime()) {
        resetUpperAnime(UPPER_2, 3.0f);
    }
}

BOOL daAlink_c::checkSwimFall() {
    if (!checkNoResetFlg0(FLG0_UNK_80) || current.pos.y > mWaterY + l_autoUpHeight) {
        return true;
    }

    return false;
}

int daAlink_c::checkSwimOutAction() {
    f32 var_f31;
    if (checkWolf()) {
        var_f31 = mpHIO->mWolf.mWlSwim.m.mStartHeight;
    } else {
        var_f31 = mpHIO->mSwim.m.mStartHeight;
    }

    if (checkSwimFall()) {
        swimOutAfter(1);

        cXyz sp8(field_0x3594.x + (mNormalSpeed * cM_ssin(current.angle.y)), 0.0f, field_0x3594.z + (mNormalSpeed * cM_scos(current.angle.y)));
        mNormalSpeed = sp8.absXZ();
        current.angle.y = sp8.atan2sX_Z();

        if (checkWolf()) {
            if (mNormalSpeed > mpHIO->mWolf.mWlAutoJump.m.mWeakJumpMaxSpeed) {
                mNormalSpeed = mpHIO->mWolf.mWlAutoJump.m.mWeakJumpMaxSpeed;
            }
            return procWolfFallInit(2, mpHIO->mWolf.mWlAutoJump.m.mNormalFallInterp);
        } else {
            if (mNormalSpeed > mpHIO->mAutoJump.m.mMaxJumpSpeed) {
                mNormalSpeed = mpHIO->mAutoJump.m.mMaxJumpSpeed;
            }
            return procFallInit(2, mpHIO->mAutoJump.m.mFallInterpolation);
        }
    }

    if (!checkNoResetFlg0(FLG0_UNK_80) || mWaterY - mLinkAcch.GetGroundH() <= var_f31 - 5.0f) {
        current.pos.y = mWaterY;
        swimOutAfter(1);
        return commonCheckNextAction(0);
    }

    if (checkMagicArmorHeavy() && checkNoResetFlg0(FLG0_SWIM_UP)) {
        offNoResetFlg0(FLG0_SWIM_UP);
        mZ2Link.setInWater(true);
    }

    return 0;
}

void daAlink_c::setSwimMoveAnime() {
    daPy_frameCtrl_c* temp_r28 = &mUnderFrameCtrl[0];
    daPy_frameCtrl_c* temp_r27 = &mUpperFrameCtrl[2];

    f32 var_f31;
    f32 temp_f25 = temp_r28->getEnd();
    f32 var_f26 = temp_r28->getFrame() * temp_f25;

    if (field_0x3000 != 0 && !checkHookshotAnime()) {
        field_0x2f98 = 4;
    } else if (checkHookshotAnime() || (checkAttentionLock() && !getZoraSwim())) {
        field_0x3000 = 0;
        if (checkInputOnR()) {
            field_0x2f98 = getDirectionFromShapeAngle();
        } else if (field_0x2f98 >= 4 || mNormalSpeed < 3.0f) {
            field_0x2f98 = 0;
        }
    } else if (checkInputOnR() || field_0x2f98 >= 4 || mNormalSpeed < 3.0f) {
        field_0x2f98 = 0;
    }

    daAlink_ANM var_r29;
    BOOL var_r26 = 1;
    BOOL var_r25 = 0;

    if (field_0x2f98 == 4) {
        f32 temp_f29 = mpHIO->mSwim.m.mUnderwaterMaxSpeed + mpHIO->mSwim.m.mUnderwaterButtonAdditionalSpeed;

        field_0x3478 = mpHIO->mSwim.m.mUnderwaterMinAnmSpeed;
        field_0x347c = mpHIO->mSwim.m.mUnderwaterMaxAnmSpeed;

        if (field_0x594 < mpHIO->mSwim.m.mUnderwaterMaxSpeed) {
            field_0x594 = mpHIO->mSwim.m.mUnderwaterMaxSpeed;
        } else if (doTrigger()) {
            field_0x594 += 0.75f;
            if (field_0x594 > temp_f29) {
                field_0x594 = temp_f29;
            }

            if (!checkSwimMoveHandAnime() && !checkZoraSwimDamageAnime()) {
                f32 var_f28;
                if (field_0x2060->getOldFrameMorfCounter() > 5.0f) {
                    var_f28 = -1.0f;
                } else {
                    var_f28 = 3.0f;
                }

                setUpperAnime(0x23F, UPPER_2, 1.0f, 0.0f, 0x28, var_f28);
                mProcVar5.field_0x3012 = 0;
                var_r25 = 1;
            } else if (temp_r27->getFrame() > 25.0f) {
                mProcVar5.field_0x3012 = 1;
            }
        } else if (!checkSwimMoveHandAnime()) {
            cLib_chaseF(&field_0x594, mpHIO->mSwim.m.mUnderwaterMaxSpeed, 0.4f);
        }

        var_f31 = 1.0f / mpHIO->mSwim.m.mUnderwaterMaxSpeed;

        if (checkZoraWearAbility() && (field_0x3198 == ANM_SWIM_DIVE || (field_0x3198 == ANM_SWIM_ZORA_START && !checkAnmEnd(temp_r28)))) {
            var_r29 = ANM_SWIM_ZORA_START;
        } else {
            var_r29 = ANM_SWIM_C;

            if (field_0x3198 == ANM_SWIM_ZORA_START) {
                var_r26 = 0;
                if (mProcVar4.field_0x3010 == 1) {
                    mProcVar4.field_0x3010 = 0;
                }
            }
        }
    } else if (field_0x2f98 == 0) {
        if (checkSwimDashMode() && checkUnderMove0BckNoArc(ANM_SWIM_DASH) && !checkAnmEnd(temp_r28)) {
            var_r29 = ANM_SWIM_DASH;
        } else if (mNormalSpeed < 3.0f || checkHookshotAnime()) {
            var_r29 = ANM_SWIM_WAIT;
            field_0x30d0 = 0;
        } else {
            var_r29 = ANM_SWIM_A;
        }

        if (checkSwimDashMode()) {
            field_0x3478 = mpHIO->mSwim.m.mDashMinAnmSpeed;
            field_0x347c = mpHIO->mSwim.m.mDashMaxAnmSpeed;
            field_0x594 = mpHIO->mSwim.m.mDashMaxSpeed;
        } else {
            field_0x3478 = mpHIO->mSwim.m.mForwardMinAnmSpeed;
            field_0x347c = mpHIO->mSwim.m.mForwardMaxAnmSpeed;
            field_0x594 = getSwimFrontMaxSpeed();
        }

        var_f31 = 1.0f / field_0x594;
    } else if (field_0x2f98 == 1) {
        var_r29 = ANM_ATN_SWIM_BACK;
        field_0x3478 = mpHIO->mSwim.m.mBackwardMinAnmSpeed;
        field_0x347c = mpHIO->mSwim.m.mBackwardMaxAnmSpeed;

        if (checkZoraWearAbility()) {
            field_0x594 = 10.0f;
        } else {
            field_0x594 = mpHIO->mSwim.m.mBackwardMaxSpeed;
        }

        var_f31 = 1.0f / field_0x594;
    } else {
        if (field_0x2f98 == 2) {
            var_r29 = ANM_ATN_SWIM_LEFT;
        } else {
            var_r29 = ANM_ATN_SWIM_RIGHT;
        }

        field_0x3478 = mpHIO->mSwim.m.mStrafeMinAnmSpeed;
        field_0x347c = mpHIO->mSwim.m.mStrafeMaxAnmSpeed;

        if (checkZoraWearAbility()) {
            field_0x594 = 12.0f;
        } else {
            field_0x594 = mpHIO->mSwim.m.mStrafeMaxSpeed;
        }

        var_f31 = 1.0f / field_0x594;
    }

    field_0x347c -= field_0x3478;

    if (field_0x2f98 != 0) {
        field_0x30d0 = 0;
    }

    if (checkBootsOrArmorHeavy()) {
        var_r29 = ANM_SWIM_WAIT;
        field_0x594 = mpHIO->mSwim.m.mUnderwaterFallMaxSpeed;
        if (mEquipItem != fpcNm_ITEM_NONE && !checkHookshotAnime()) {
            setDoStatus(4);
        }
    }

    if (var_r29 != field_0x3198) {
        if (field_0x3198 == ANM_SWIM_ZORA_START) {
            speed.y = 0.0f;
        }

        field_0x3198 = var_r29;

        if (checkUnderMove0BckNoArc(ANM_SWIM_DASH)) {
            field_0x30d2 = mpHIO->mSwim.m.field_0x5e;
        }

        if (var_r29 == ANM_SWIM_WAIT) {
            setSingleAnimeBaseSpeed(ANM_SWIM_WAIT, mpHIO->mSwim.m.mWaitAnmSpeed, mpHIO->mSwim.m.mWaitInterpolation);
        } else {
            f32 var_f27;
            if (var_r26 != 0) {
                var_f27 = mpHIO->mSwim.m.mMoveInterpolation;
            } else {
                var_f27 = 0.0f;
            }
            setSingleAnimeBaseMorf(var_r29, var_f27);
        }
    }

    if (var_r29 != ANM_SWIM_WAIT && var_r29 != ANM_SWIM_DASH) {
        f32 temp_f30 = var_f31 * fabsf(mNormalSpeed);
        if (temp_f30 > 1.0f && field_0x2f98 == 4) {
            temp_r28->setRate(mpHIO->mSwim.m.mUnderwaterMaxAnmSpeed + ((temp_f30 - 1.0f) * mpHIO->mSwim.m.mUnderwaterAdditionalAnmSpeed));
        } else {
            temp_r28->setRate(field_0x3478 + (field_0x347c * temp_f30));
        }
    }

    if (checkSwimMoveHandAnime()) {
        if (temp_r27->checkPass(17.0f)) {
            seStartOnlyReverb(Z2SE_AL_WATER_STROKE_L);
        }
    
        if (var_r29 == ANM_SWIM_C) {
            temp_r27->setRate(temp_r28->getRate());

            if (var_r25 == 0 && temp_r27->checkPass(0.0f)) {
                if (mProcVar5.field_0x3012 != 0) {
                    mProcVar5.field_0x3012 = 0;
                } else {
                    resetUpperAnime(UPPER_2, 3.0f);
                }
            }
        } else {
            resetUpperAnime(UPPER_2, -1.0f);
        }
    }

    maxFallSpeed = getSwimMaxFallSpeed();

    if (var_r29 == ANM_SWIM_C || var_r29 == ANM_SWIM_ZORA_START) {
        if (!checkZoraWearAbility()) {
            if (field_0x3080 != -0x3800) {
                setOldRootQuaternion(0x4000, 0, 0);
            }
            field_0x3080 = -0x3800;
        } else if (mProcVar4.field_0x3010 == 1) {
            cLib_addCalcAngleS(&field_0x3080, mProcVar3.field_0x300e, 5, 0x1000, 0x100);
        } else if (mProcVar4.field_0x3010 == 2) {
            cLib_addCalcAngleS(&field_0x3080, mProcVar3.field_0x300e, 5, 0x1000, 0x100);
            mProcVar4.field_0x3010 = 0;
        } else {
            s16 var_r24;
            if (checkInputOnR() && !checkEventRun()) {
                var_r24 = 13653.0f * cM_scos(mStickAngle);
            } else {
                var_r24 = 0;
            }

            cLib_addCalcAngleS(&field_0x3080, var_r24, 5, 0x200, 0x80);
        }
    } else if (field_0x3080 != 0) {
        setOldRootQuaternion(field_0x3080, 0, 0);
        field_0x3080 = 0;
        mProcVar4.field_0x3010 = 0;

        if (!checkZoraWearAbility()) {
            mNormalSpeed = 0.0f;
        }
    }
}

bool daAlink_c::checkSwimButtonAccept() {
    return dComIfGp_getDoStatus() == 0x41 || dComIfGp_getDoStatus() == 0x4C;
}

bool daAlink_c::checkUpSwimButtonAccept() {
    return dComIfGp_getDoStatus() == 0x97;
}

BOOL daAlink_c::checkSwimButtonMove() {
    return !checkNoResetFlg0(FLG0_SWIM_UP) &&
           (checkSwimButtonAccept() || checkUpSwimButtonAccept()) && doButton();
}

BOOL daAlink_c::checkZoraSwimMove() {
    return checkSwimButtonMove() || field_0x3000 != 0;
}

BOOL daAlink_c::checkSwimNeckUpDown() const {
    return !checkNoResetFlg0(FLG0_SWIM_UP) && checkUnderMove0BckNoArc(ANM_SWIM_WAIT);
}

void daAlink_c::setSwimUpDownOffset() {
    mProcVar2.field_0x300c += (s16)((cM_rndF(0.3f) + 0.85f) * 2330.0f);

    f32 var_f1;
    if (checkWolf()) {
        if (checkHeavyStateOn(1, 1)) {
            var_f1 = mpHIO->mWolf.mWlSwim.m.mHeavyIdleUpDownSwayAmount;
        } else {
            var_f1 = mpHIO->mWolf.mWlSwim.m.mIdleUpDownSwayAmount;
        }
    } else {
        var_f1 = mpHIO->mSwim.m.mWaitUpDownShakeAmount;
    }

    field_0x2b98 = var_f1 * cM_ssin(mProcVar2.field_0x300c);
}

int daAlink_c::procSwimUpInit() {
    commonProcInit(PROC_SWIM_UP);

    if (field_0x3000 != 0 && !checkZoraWearAbility()) {
        mNormalSpeed = 0.0f;
    }

    speed.y = 0.0f;
    current.pos.y = mWaterY;
    field_0x3080 = 0;
    onNoResetFlg0(FLG0_SWIM_UP);
    mZ2Link.setInWater(false);
    setSingleAnimeParam(ANM_SWIM_RESURFACE, &mpHIO->mSwim.m.mFloatUpAnm);
    dComIfGp_setPlayerStatus0(0, 0x100000);
    mProcVar0.field_0x3008 = 0;
    mProcVar4.field_0x3010 = 0;
    field_0x3480 = mpHIO->mSwim.m.mStartHeight;
    field_0x3000 = 0;

    seStartMapInfo(Z2SE_AL_OUTOF_WATER);
    swimDeleteItem();
    field_0x2fcd = 15;
    return 1;
}

int daAlink_c::procSwimUp() {
    daPy_frameCtrl_c* frame_ctrl = mUnderFrameCtrl;

    setNormalSpeedF(0.0f, mpHIO->mSwim.m.mDeceleration);

    if (checkSetItemTrigger(fpcNm_ITEM_HVY_BOOTS)) {
        setHeavyBoots(1);
    }

    if (checkBootsOrArmorHeavy()) {
        if (checkInputOnR()) {
            procSwimMoveInit();
        } else {
            procSwimWaitInit(0);
        }
    } else if (checkAnmEnd(frame_ctrl)) {
        procSwimWaitInit(0);
    } else if (frame_ctrl->getFrame() > mpHIO->mSwim.m.mFloatUpAnm.mCancelFrame &&
               checkInputOnR())
    {
        procSwimMoveInit();
    } else {
        current.pos.y = mWaterY;
    }

    return 1;
}

int daAlink_c::procSwimWaitInit(int param_0) {
    daPy_frameCtrl_c* frame_ctrl = mUnderFrameCtrl;
    u32 mode_40000 = checkModeFlg(0x40000);

    if (checkNoResetFlg0(FLG0_UNDERWATER)) {
        offNoResetFlg0(FLG0_UNDERWATER);
        current.pos.y += mpHIO->mSwim.m.mStartHeight;
    }

    commonProcInit(PROC_SWIM_WAIT);

    if (param_0) {
        seStartMapInfo(Z2SE_AL_INTO_WATER);
        mProcVar4.field_0x3010 = 0;
        field_0x3480 = mpHIO->mSwim.m.mStartHeight;
    } else if (!mode_40000) {
        mProcVar4.field_0x3010 = 0;
        field_0x3480 = mpHIO->mSwim.m.mStartHeight;
    }

    swimDeleteItem();

    if (!checkUnderMove0BckNoArc(ANM_SWIM_WAIT)) {
        setSingleAnimeBaseSpeed(ANM_SWIM_WAIT, mpHIO->mSwim.m.mWaitAnmSpeed,
                                mpHIO->mSwim.m.mWaitInterpolation);
    } else if (frame_ctrl->getRate() < 0.1f) {
        setSingleAnimeBaseSpeed(ANM_SWIM_WAIT, mpHIO->mSwim.m.mWaitAnmSpeed, -1.0f);
    }

    field_0x30d0 = 0;
    field_0x3198 = 0x14;
    field_0x32cc = mEquipItem;

    if (field_0x3080 != 0) {
        setOldRootQuaternion(field_0x3080, 0, 0);
        field_0x3080 = 0;
    }

    dComIfGp_setPlayerStatus0(0, 0x100000);

    if (checkNoResetFlg0(FLG0_SWIM_UP)) {
        current.pos.y = mWaterY;
        speed.y = 0.0f;
    }

    mProcVar2.field_0x300c = 0;
    mProcVar0.field_0x3008 = 0;
    field_0x594 = getSwimFrontMaxSpeed();
    return 1;
}

int daAlink_c::procSwimWait() {
    if (mDemo.getDemoMode() == 6 || mDemo.getDemoMode() == 8) {
        setShapeAngleToTalkActor();
        current.angle.y = shape_angle.y;
    }

    maxFallSpeed = getSwimMaxFallSpeed();
    field_0x594 = getSwimFrontMaxSpeed();

    setSpeedAndAngleSwim();

    if (mEquipItem != field_0x32cc) {
        field_0x32cc = mEquipItem;
        setSingleAnimeBaseSpeed(ANM_SWIM_WAIT, mpHIO->mSwim.m.mWaitAnmSpeed,
                                mpHIO->mSwim.m.mWaitInterpolation);
    }

    setSwimUpDownOffset();

    if (checkEquipHeavyBoots() && mEquipItem != fpcNm_ITEM_NONE) {
        setDoStatus(0x4);
    }

    if (checkSwimUpAction()) {
        return 1;
    }

    if (checkNextActionSwim()) {
        return 1;
    }

    if (checkFrontWallTypeAction()) {
        swimOutAfter(1);
        return 1;
    }

    if (checkSwimButtonAccept() && doTrigger()) {
        procSwimDiveInit();
    } else if ((checkInputOnR() && mNormalSpeed >= mpHIO->mSwim.m.mForwardMinSpeed) ||
               (checkUpSwimButtonAccept() && doButton()))
    {
        if (checkUpSwimButtonAccept() && doButton()) {
            field_0x3000 = 30;
        }

        procSwimMoveInit();
    }

    return 1;
}

int daAlink_c::procSwimMoveInit() {
    u32 mode_40000 = checkModeFlg(0x40000);
    bool proc_dive_jump = mProcID == PROC_DIVE_JUMP;
    bool proc_swim_dive = mProcID == PROC_SWIM_DIVE;

    commonProcInit(PROC_SWIM_MOVE);

    if (checkNoResetFlg0(FLG0_UNDERWATER)) {
        offNoResetFlg0(FLG0_UNDERWATER);
    }

    if (!mode_40000) {
        field_0x3480 = mpHIO->mSwim.m.mStartHeight;
        mProcVar4.field_0x3010 = 0;
    }

    if (proc_swim_dive || proc_dive_jump) {
        field_0x3198 = 199;

        if (proc_dive_jump) {
            field_0x3080 = 0x2800;
            mProcVar3.field_0x300e = field_0x3080;
            mProcVar4.field_0x3010 = 1;
            setOldRootQuaternion(-field_0x3080, 0, 0);
        }
    } else {
        field_0x3198 = -1;
    }

    field_0x2f98 = 5;
    setSwimMoveAnime();

    if (checkNoResetFlg0(FLG0_SWIM_UP)) {
        current.pos.y = mWaterY;
        speed.y = 0.0f;
    }

    swimDeleteItem();
    dComIfGp_setPlayerStatus0(0, 0x100000);
    mProcVar0.field_0x3008 = 0;
    return 1;
}

int daAlink_c::procSwimMove() {
    setSpeedAndAngleSwim();

    if (checkUpSwimButtonAccept() && doButton()) {
        field_0x3000 = 30;
    }

    setSwimMoveAnime();

    if (checkSwimUpAction()) {
        return 1;
    }

    if (checkNextActionSwim()) {
        return 1;
    }

    if (checkInputOnR() && checkFrontWallTypeAction()) {
        swimOutAfter(1);
        return 1;
    }

    if (checkSwimButtonAccept() && doTrigger() && !getZoraSwim()) {
        procSwimDiveInit();
    } else if (dComIfGp_getDoStatus() == 9 && doTrigger()) {
        onNoResetFlg1(FLG1_DASH_MODE);
        field_0x30d0 = mpHIO->mSwim.m.field_0x5c;
        setSingleAnimeParam(ANM_SWIM_DASH, &mpHIO->mSwim.m.mDashAnm);
        field_0x3198 = ANM_SWIM_DASH;
    } else if (!checkZoraSwimMove() && (!checkUpSwimButtonAccept() || !doButton()) &&
               mNormalSpeed < mpHIO->mSwim.m.mForwardMinSpeed)
    {
        procSwimWaitInit(0);
    }

    return 1;
}

int daAlink_c::procSwimDiveInit() {
    commonProcInit(PROC_SWIM_DIVE);
    setSpecialGravity(0.0f, maxFallSpeed, 0);
    speed.y = 0.0f;

    if (checkNoResetFlg0(FLG0_SWIM_UP)) {
        mProcVar2.field_0x300c = 1;
        setSingleAnimeParam(ANM_SWIM_DIVE, &mpHIO->mSwim.m.mDiveAnm);
        voiceStart(Z2SE_AL_V_DIVING);
    } else {
        mProcVar2.field_0x300c = 0;
        setSingleAnimeParam(ANM_SWIM_DIVE, &mpHIO->mSwim.m.mUnderwaterDiveAnm);
    }

    field_0x2f99 = 0x10;
    field_0x3588 = l_waitBaseAnime;
    field_0x3480 = mpHIO->mSwim.m.mStartHeight;
    mProcVar4.field_0x3010 = 0;

    dComIfGp_setPlayerStatus0(0, 0x100000);
    field_0x3000 = 30;
    field_0x3478 = 0.0f;
    field_0x347c = 1.0f / mpHIO->mSwim.m.mUnderwaterDiveAnm.mEndFrame;
    return 1;
}


int daAlink_c::procSwimDive() {
    daPy_frameCtrl_c* frame_ctrl = mUnderFrameCtrl;

    if (mProcVar2.field_0x300c == 0) {
        setDoStatus(0x4C);
    } else if (checkNoResetFlg0(FLG0_SWIM_UP) && frame_ctrl->getFrame() > 20.0f) {
        offNoResetFlg0(FLG0_SWIM_UP);
        mZ2Link.setInWater(true);
    }

    field_0x2f99 = 7;
    field_0x3000 = 30;
    setNormalSpeedF(0.0f, mpHIO->mSwim.m.mDeceleration);

    field_0x3478 = field_0x347c * frame_ctrl->getFrame();
    if (field_0x3478 > 1.0f) {
        field_0x3478 = 1.0f;
    }

    if (frame_ctrl->checkPass(15.0f) && mProcVar2.field_0x300c != 0) {
        cXyz pos(current.pos.x, mWaterY, current.pos.z);
        fopKyM_createWpillar(&pos, 0.7f, 0);
    }

    if (mLinkAcch.GetGroundH() > current.pos.y - field_0x598 * 0.5f) {
        current.pos.y = field_0x598 * 0.5f + mLinkAcch.GetGroundH();
    }

    if (checkAnmEnd(frame_ctrl)) {
        speed.y = mpHIO->mSwim.m.mMaxFallSpeed;

        if (mProcVar2.field_0x300c != 0) {
            field_0x3080 = 0x2800;
            mProcVar3.field_0x300e = field_0x3080;
            mProcVar4.field_0x3010 = 1;
        }

        setOldRootQuaternion(-field_0x3080, 0, 0);
        return procSwimMoveInit();
    }

    return 1;
}


int daAlink_c::procSwimHookshotSubjectInit() {
    if (!commonProcInitNotSameProc(PROC_SWIM_HOOKSHOT_SUBJECT)) {
        return 0;
    }

    initHookshotUpperAnimeSpeed(1);

    if (!checkUnderMove0BckNoArc(ANM_SWIM_WAIT)) {
        setSingleAnimeBaseSpeed(ANM_SWIM_WAIT, mpHIO->mSwim.m.mWaitAnmSpeed,
                                mpHIO->mItem.mHookshot.m.mStartInterpolation);
    }

    mNormalSpeed = 0.0f;
    current.angle.y = shape_angle.y;

    if (field_0x3080 != 0) {
        setOldRootQuaternion(field_0x3080, 0, 0);
        field_0x3080 = 0;
    }

    if (checkNoResetFlg0(FLG0_SWIM_UP)) {
        current.pos.y = mWaterY;
        speed.y = 0.0f;
    }

    mProcVar2.field_0x300c = 0;
    mProcVar0.field_0x3008 = 0;
    field_0x594 = getSwimFrontMaxSpeed();

    field_0x3480 = mpHIO->mSwim.m.mStartHeight;
    mProcVar4.field_0x3010 = 0;
    field_0x30d0 = 0;

    setHookshotReadyMaterial();
    dComIfGp_setPlayerStatus0(0, 0x104000);
    return 1;
}

int daAlink_c::procSwimHookshotSubject() {
    maxFallSpeed = getSwimMaxFallSpeed();
    field_0x594 = getSwimFrontMaxSpeed();

    if (checkHookshotWait()) {
        setDoStatus(0x12);
    }

    setShapeAngleToAtnActor(0);
    mSight.offDrawFlg();

    if (checkSwimUpAction()) {
        return 1;
    }

    if (checkNextActionFromButton()) {
        return 1;
    }

    if (!checkHookshotAnime()) {
        return procSwimWaitInit(0);
    }

    if (!checkNextActionHookshot()) {
        if (checkHookshotWait()) {
            if (setBodyAngleToCamera()) {
                setHookshotSight();
            }

            dComIfGp_clearPlayerStatus0(0, 0x40000);
        } else {
            dComIfGp_setPlayerStatus0(0, 0x40000);
        }
    }

    return 1;
}

int daAlink_c::procSwimHookshotMoveInit() {
    if (!commonProcInitNotSameProc(PROC_SWIM_HOOKSHOT_MOVE)) {
        return 0;
    }

    if (checkZeroSpeedF()) {
        onModeFlg(1);
    }

    initHookshotUpperAnimeSpeed(0);
    field_0x3198 = -1;
    field_0x2f98 = 5;

    if (mItemVar0.field_0x3018 == 0) {
        setSwimMoveAnime();
    }

    if (checkNoResetFlg0(FLG0_SWIM_UP)) {
        current.pos.y = mWaterY;
        speed.y = 0.0f;
    }

    mProcVar0.field_0x3008 = 0;

    if (field_0x3080 != 0) {
        setOldRootQuaternion(field_0x3080, 0, 0);
        field_0x3080 = 0;
    }

    field_0x3480 = mpHIO->mSwim.m.mStartHeight;
    mProcVar4.field_0x3010 = 0;
    field_0x30d0 = 0;

    dComIfGp_setPlayerStatus0(0, 0x104000);
    return 1;
}

int daAlink_c::procSwimHookshotMove() {
    cancelHookshotMove();

    if (checkHookshotWait() && !checkZoraWearAbility()) {
        setDoStatus(0x12);
    }

    if (mItemVar0.field_0x3018 == 0) {
        setSpeedAndAngleSwim();
        setSwimMoveAnime();
    }

    if (checkSwimUpAction()) {
        return 1;
    }

    if (checkSwimButtonAccept() && doTrigger()) {
        return procSwimDiveInit();
    }

    if (checkUpSwimButtonAccept() && doTrigger()) {
        field_0x3000 = 30;
        return procSwimMoveInit();
    }

    if (checkHookshotWait() || checkBossOctaIealRoom()) {
        setShapeAngleToAtnActor(0);
    }

    if (checkNextActionFromButton()) {
        return 1;
    }

    if (!checkHookshotAnime()) {
        return procSwimWaitInit(0);
    }

    if (!checkNextActionHookshot()) {
        if (checkZeroSpeedF()) {
            onModeFlg(1);
        } else {
            offModeFlg(1);
        }

        if (checkHookshotWait() || (mTargetedActor != NULL && checkBossOctaIealRoom())) {
            setBodyAngleXReadyAnime(0);
        }
    }

    return 1;
}

int daAlink_c::procSwimDamageInit(dCcD_GObjInf* param_0) {
    seStartOnlyReverb(Z2SE_AL_DAMAGE_NORMAL);
    dComIfGp_getVibration().StartShock(2, 31, cXyz(0.0f, 1.0f, 0.0f));

    BOOL bvar4 =
        checkNoResetFlg0(FLG0_SWIM_UP) || ((param_0 != NULL && param_0->GetTgHitAc() != NULL) &&
                                             fopAcM_GetName(param_0->GetTgHitAc()) == PROC_E_SG);

    if (!getZoraSwim() && bvar4 && mProcID != PROC_SWIM_WAIT) {
        return 1;
    }

    cXyz* temp_r30;
    cXyz sp30;

    if (param_0 != NULL) {
        temp_r30 = (cXyz*)getDamageVec(param_0);
    } else {
        sp30.set(-cM_ssin(current.angle.y), 0.0f, -cM_scos(current.angle.y));
        temp_r30 = &sp30;
    }

    f32 scale =
        temp_r30->abs() * mpHIO->mDamage.mDamSwim.m.mAttackSpeedRate + mpHIO->mDamage.mDamSwim.m.mInitialSpeed;
    if (scale > mpHIO->mDamage.mDamSwim.m.mMaxSpeed) {
        scale = mpHIO->mDamage.mDamSwim.m.mMaxSpeed;
    }

    field_0x3750 = temp_r30->normZP();
    field_0x3750 *= scale;

    if (getZoraSwim()) {
        setUpperAnimeParam(0xBF, UPPER_2, &mpHIO->mDamage.mDamSwim.m.mSinkAnm);
        setFacePriBck(0xEA);
        setFacePriTexture(FTANM_UNK_27);
        return 1;
    }

    commonProcInit(PROC_SWIM_DAMAGE);
    cXyz sp24(temp_r30->z * -cM_ssin(shape_angle.y) + temp_r30->x * cM_scos(shape_angle.y),
              temp_r30->y,
              temp_r30->z * cM_scos(shape_angle.y) + temp_r30->x * cM_ssin(shape_angle.y));

    mProcVar0.field_0x3008 =
        cLib_minMaxLimit(cM_atan2s(sp24.z, sp24.y), (s16)-mpHIO->mDamage.mDamSwim.m.mFrontBackBodyMaxAngle,
                         (s16)mpHIO->mDamage.mDamSwim.m.mFrontBackBodyMaxAngle);

    mProcVar1.field_0x300a = cLib_minMaxLimit(
        cM_atan2s(sp24.x, -JMAFastSqrt(sp24.y * sp24.y + sp24.z * sp24.z)),
        (s16)-mpHIO->mDamage.mDamSwim.m.mLeftRightBodyMaxAngle, (s16)mpHIO->mDamage.mDamSwim.m.mLeftRightBodyMaxAngle);

    int dir = getDirectionFromAngle(cM_atan2s(-sp24.x, -sp24.z));
    if (bvar4) {
        setSingleAnimeParam(ANM_SWIM_DMG_FREEZE, &mpHIO->mDamage.mDamSwim.m.mSurfaceAnm);
        field_0x3478 = mpHIO->mDamage.mDamSwim.m.mSurfaceAnm.mCancelFrame;
        mProcVar0.field_0x3008 = 0;
        mProcVar1.field_0x300a = 0;
    } else if (dir == DIR_FORWARD) {
        setSingleAnimeParam(ANM_SWIM_DMG_FRONT, &mpHIO->mDamage.mDamSwim.m.mFrontAnm);
        field_0x3478 = mpHIO->mDamage.mDamSwim.m.mFrontAnm.mCancelFrame;
    } else if (dir == DIR_BACKWARD) {
        setSingleAnimeParam(ANM_SWIM_DMG_BACK, &mpHIO->mDamage.mDamSwim.m.mRearAnm);
        field_0x3478 = mpHIO->mDamage.mDamSwim.m.mRearAnm.mCancelFrame;
    } else if (dir == DIR_LEFT) {
        setSingleAnimeParam(ANM_SWIM_DMG_LEFT, &mpHIO->mDamage.mDamSwim.m.mLeftAnm);
        field_0x3478 = mpHIO->mDamage.mDamSwim.m.mLeftAnm.mCancelFrame;
    } else {
        setSingleAnimeParam(ANM_SWIM_DMG_RIGHT, &mpHIO->mDamage.mDamSwim.m.mRightAnm);
        field_0x3478 = mpHIO->mDamage.mDamSwim.m.mRightAnm.mCancelFrame;
    }

    mNormalSpeed = 0.0f;
    field_0x347c = M_PI / (mUnderFrameCtrl->getEnd() - mUnderFrameCtrl->getStart());
    field_0x3480 = mpHIO->mSwim.m.mStartHeight;
    current.angle.y = shape_angle.y;

    return 1;
}

int daAlink_c::procSwimDamage() {
    daPy_frameCtrl_c* frame_ctrl = mUnderFrameCtrl;

    f32 fvar8 = cM_fsin(field_0x347c * (frame_ctrl->getFrame() - frame_ctrl->getStart()));
    mBodyAngle.x = mProcVar0.field_0x3008 * fvar8;
    mBodyAngle.z = -mProcVar1.field_0x300a * fvar8;
    mBodyAngle.y = 0;

    if (checkSwimUpAction()) {
        return 1;
    }

    if (checkAnmEnd(frame_ctrl)) {
        if (!checkNextActionSwim()) {
            return procSwimWaitInit(0);
        }
    } else if (frame_ctrl->getFrame() > field_0x3478) {
        onModeFlg(4);

        if (!checkNextActionSwim() && checkInputOnR()) {
            return procSwimMoveInit();
        }
    }

    return 1;
}


int daAlink_c::procOctaIealSpitInit() {
    u32 temp_r30 = checkModeFlg(MODE_SWIMMING);
    commonProcInit(PROC_OCTAIEAL_SPIT);

    if (checkNoResetFlg0(FLG0_UNDERWATER)) {
        offNoResetFlg0(FLG0_UNDERWATER);
    }

    setSingleAnimeBase(ANM_MORPHEEL_SPIT_OUT);
    field_0x3750 = cXyz::Zero;
    field_0x3080 = 0;
    field_0x3000 = 0;
    speed.y = field_0x340c;
    mNormalSpeed = field_0x3408;
    field_0x3478 = field_0x3408 * 0.1f;

    setDamagePointNormal(field_0x318c);

    if (field_0x3478 < 1.0f) {
        field_0x3478 = 1.0f;
    }

    if (temp_r30) {
        field_0x2f99 = 15;
        field_0x33b0 = -45.0f;
    } else {
        field_0x2f99 = 16;
        current.pos.y += 145.0f;
    }

    field_0x3588 = l_waitBaseAnime;
    offNoResetFlg0(FLG0_SWIM_UP);
    current.angle.y = field_0x2ffe;

    if (mNormalSpeed > 0.0f) {
        shape_angle.y = current.angle.y + 0x8000;
    } else {
        shape_angle.y = current.angle.y;
    }

    return 1;
}

static f32 dummyFloat() {
    return -34.0f;
}

int daAlink_c::procOctaIealSpit() {
    if (mDemo.getDemoMode() == 0x2E) {
        cancelOriginalDemo();
    }

    cLib_addCalc(&mNormalSpeed, 0.0f, 0.5f, field_0x3478, 0.1f);
    field_0x2f99 = 7;

    if (mUnderFrameCtrl[0].checkAnmEnd()) {
        procSwimWaitInit(0);
    }

    return 1;
}
