/**
 * d_a_alink_sumou.inc
 * Player Sumo action handling
 */

#include "d/actor/d_a_alink.h"
#include "d/d_com_inf_game.h"
#include "d/actor/d_a_npc_wrestler.h"
#include "d/actor/d_a_boomerang.h"

BOOL daAlink_c::checkSumouVsActor() {
    mCargoCarryAcKeep.setActor();

    if (mCargoCarryAcKeep.getActor() == NULL) {
        cancelSumouMode();
        procWaitInit();
        return false;
    }

    if (mSpecialMode == 0xD || mSpecialMode == 0xB || mSpecialMode == 0xC || mSpecialMode == 0xE) {
        procSumouWinLoseInit();
        return false;
    }

    return true;
}

void daAlink_c::cancelSumouMode() {
    mCargoCarryAcKeep.clearData();
    mSpecialMode = 0;
}

BOOL daAlink_c::sumouPunchTrigger() {
    return itemTriggerCheck(BTN_B);
}

void daAlink_c::setSumouPunchStatus() {
    setBStatus(ACTION_STR_SLAP);
}

int daAlink_c::procSumouReadyInit() {
    if (!commonProcInitNotSameProc(PROC_SUMOU_READY)) {
        return 0;
    }

    setSingleAnimeBaseMorf(ANM_SUMOU_MIAU, 0.0f);
    mNormalSpeed = 0.0f;
    current.angle.y = shape_angle.y;
    field_0x3588 = l_waitBaseAnime;
    field_0x34d4 = field_0x3588;
    field_0x2f99 = 4;
    mSpecialMode = 1;
    mProcVar2.field_0x300c = 0;
    mProcVar3.field_0x300e = 0;
    deleteEquipItem(FALSE, TRUE);

    if (mThrowBoomerangAcKeep.getActor() != NULL) {
        static_cast<daBoomerang_c*>(mThrowBoomerangAcKeep.getActor())->onForceDelete();
    }

    return 1;
}

int daAlink_c::procSumouReady() {
    field_0x2f99 = 4;

    if (!checkSumouVsActor()) {
        return 1;
    }

    if (mSpecialMode == 2 || mSpecialMode == 3 || mSpecialMode == 4) {
        setSumouPunchStatus();
        setDoStatusEmphasys(0x15);
        dComIfGp_setAdvanceDirection(5);

        if (doTrigger()) {
            mProcVar2.field_0x300c = 1;
        }

        if (sumouPunchTrigger()) {
            mProcVar3.field_0x300e = 1;
        }
    }

    if (mUnderAnmHeap[0].getIdx() == getMainBckData(ANM_SUMOU_HAKEYOI)->m_underID &&
        mUnderFrameCtrl[0].checkPass(7.0f))
    {
        field_0x2f92 = 0xFE;
        field_0x2f93 = 0xFE;
    }

    if (mSpecialMode == 3) {
        setSingleAnimeBase(ANM_SUMOU_HAKEYOI);
        field_0x2f99 = 12;
        mSpecialMode = 4;
        dComIfGp_clearPlayerStatus0(0, 0x2000);
    } else if (checkAnmEnd(mUnderFrameCtrl)) {
        if (checkInputOnR()) {
            s16 temp_r29 = (s16)(field_0x2fe2 - shape_angle.y);
            int var_r30 = abs(temp_r29);
            cLib_distanceAngleS(field_0x2fe2, shape_angle.y);

            if (var_r30 > 0x800 && var_r30 < 0x7800) {
                mNormalSpeed = 1.0f;

                if (temp_r29 > 0) {
                    field_0x2f98 = 2;
                } else {
                    field_0x2f98 = 3;
                }
            }
        }

        if (mSpecialMode == 0x1C || mSpecialMode == 0x1B || mProcVar2.field_0x300c != 0 ||
            mProcVar3.field_0x300e != 0 ||
            (static_cast<daNpcWrestler_c*>(mCargoCarryAcKeep.getActor())->getWrestlerAction() ==
                 3 &&
             !checkZeroSpeedF()))
        {
            procSumouActionInit(mProcVar2.field_0x300c, mProcVar3.field_0x300e, 0);
        } else {
            procSumouSideMoveInit();
        }
    }

    return 1;
}


int daAlink_c::procSumouMoveInit() {
    daNpcWrestler_c* wrestler_p = (daNpcWrestler_c*)mCargoCarryAcKeep.getActor();
    commonProcInit(PROC_SUMOU_MOVE);

    field_0x2f99 = 12;
    field_0x3478 = 1000.0f;

    daAlink_ANM anm;
    if (mSpecialMode == 15 || mSpecialMode == 16) {
        if (wrestler_p->getWrestlerType() != 0) {
            anm = ANM_SUMOU_TACKLE_WAIT_GORON;
        } else {
            anm = ANM_SUMOU_TACKLE_WAIT;
        }

        setSingleAnimeBase(anm);
        mSpeedModifier = 1.0f;
        mSpecialMode = 7;
        mProcVar2.field_0x300c = 0xFFFF;
        mProcVar0.field_0x3008 = 10;
        field_0x3478 = 5.0f;
        field_0x32cc = Z2SE_AL_V_SUMO_PUSH;
    } else {
        if (wrestler_p->getWrestlerType() != 0) {
            anm = ANM_SUMOU_TACKLE_STAGGER_GORON;
        } else {
            anm = ANM_SUMOU_TACKLE_STAGGER;
        }

        setSingleAnimeBase(anm);
        mSpeedModifier = -1.0f;
        mSpecialMode = 8;
        mProcVar2.field_0x300c = 10;
        mProcVar0.field_0x3008 = 15;
        field_0x2f9d = 0x60;
    }

    field_0x3198 = anm;
    current.angle.y = shape_angle.y;
    mProcVar3.field_0x300e = 0;
    mProcVar4.field_0x3010 = 0;
    mNormalSpeed = 0.0f;
    mProcVar1.field_0x300a = 0x4B;
    setFootEffectProcType(5);
    return 1;
}


int daAlink_c::procSumouMove() {
    field_0x2f99 = 4;

    if (!checkSumouVsActor()) {
        return 1;
    }

    daNpcWrestler_c* wrestler_p = (daNpcWrestler_c*)mCargoCarryAcKeep.getActor();
    daPy_frameCtrl_c* frameCtrl_p = mUnderFrameCtrl;
    f32 sp8;

    if (frameCtrl_p->checkPass(field_0x3478)) {
        voiceStart(field_0x32cc);
    }

    if (mSpecialMode == 9 || mSpecialMode == 10) {
        if (mProcVar1.field_0x300a != 0) {
            mProcVar1.field_0x300a--;
        }

        sp8 = frameCtrl_p->getRate();
        if (mProcVar0.field_0x3008 != 0) {
            if (doTrigger()) {
                mProcVar0.field_0x3008--;
            }

            f32 var_f1;
            if (mSpecialMode == 10 && mProcVar1.field_0x300a == 0) {
                var_f1 = 1.5f;
            } else {
                var_f1 = 0.2f;
            }

            cLib_chaseF(&sp8, var_f1, 0.3f);
            frameCtrl_p->setRate(sp8);
        } else if (mSpecialMode == 9) {
            cLib_chaseF(&sp8, 1.5f, 0.3f);
        } else {
            setSingleAnimeBase(ANM_SUMOU_PUSH_BACK);
            mSpecialMode = 0x23;
            mSpeedModifier = 0.0f;
            field_0x3478 = 13.0f;
            field_0x32cc = Z2SE_AL_V_SUMO_HOLD_BACK;
            field_0x3198 = ANM_SUMOU_PUSH_BACK;
        }
    } else if (mProcVar3.field_0x300e == 0) {
        if (mProcVar2.field_0x300c > 0 && doTrigger()) {
            mProcVar2.field_0x300c--;
        }

        setDoStatusContinuation(0x64);
        if (mSpeedModifier > 0.0f) {
            sp8 = frameCtrl_p->getRate();

            if (doTrigger()) {
                cLib_chaseF(&sp8, 1.5f, 0.1f);
            } else {
                cLib_addCalc(&sp8, 1.0f, 0.5f, 0.05f, 0.02f);
            }

            frameCtrl_p->setRate(sp8);
        }

        dMeter2Info_onBlinkButton(1);

        if (mSpecialMode == 0x24 || (mSpeedModifier < 0.0f && mProcVar2.field_0x300c == 0)) {
            mProcVar3.field_0x300e = 1;
            field_0x2f99 = 12;

            if (mSpeedModifier > 0.0f) {
                daAlink_ANM anm;
                if (wrestler_p->getWrestlerType() != 0) {
                    anm = ANM_SUMOU_PULL_BACK_GORON;
                } else {
                    anm = ANM_SUMOU_PULL_BACK;
                }

                setSingleAnimeBase(anm);
                mSpecialMode = 0x24;
                field_0x3478 = 13.0f;
                field_0x32cc = Z2SE_AL_V_SUMO_PUSHED_BACK;
                field_0x3198 = anm;
            } else {
                setSingleAnimeBase(ANM_SUMOU_PUSH_BACK);
                mSpecialMode = 0x23;
                field_0x3478 = 13.0f;
                field_0x32cc = Z2SE_AL_V_SUMO_HOLD_BACK;
                field_0x3198 = ANM_SUMOU_PUSH_BACK;
            }

            mSpeedModifier = 0.0f;
        } else {
            s16 var_r31 = 0;

            if (mSpeedModifier > 0.0f && checkInputOnR()) {
                int direction = getDirectionFromAngle(mStickAngle);
                if (direction == DIR_LEFT || direction == DIR_RIGHT) {
                    var_r31 = field_0x33ac * (mpHIO->mPushpull.m.mSumoRotationAngle * field_0x33ac);

                    if (direction == DIR_RIGHT) {
                        var_r31 = -var_r31;
                    }
                }
            } else if (mSpeedModifier < 0.0f) {
                var_r31 = mProcVar4.field_0x3010;
                mProcVar4.field_0x3010 = 0;
            }

            if (var_r31 != 0) {
                f32 temp_f3 = current.pos.x + cM_ssin(shape_angle.y) * 40.0f;
                f32 temp_f1 = current.pos.z + cM_scos(shape_angle.y) * 40.0f;

                shape_angle.y += var_r31;
                current.angle.y = shape_angle.y;
                current.pos.x = temp_f3 - (cM_ssin(shape_angle.y) * 40.0f);
                current.pos.z = temp_f1 - (cM_scos(shape_angle.y) * 40.0f);
            }
        }
    } else if (checkAnmEnd(frameCtrl_p)) {
        return procSumouSideMoveInit();
    }

    if (field_0x3198 == ANM_SUMOU_TACKLE_STAGGER ||
        field_0x3198 == ANM_SUMOU_TACKLE_STAGGER_GORON ||
        field_0x3198 == ANM_SUMOU_TACKLE_WAIT_GORON || field_0x3198 == ANM_SUMOU_TACKLE_WAIT)
    {
        field_0x2f9d = 0x60;
    } else if ((field_0x3198 == ANM_SUMOU_PULL_BACK || field_0x3198 == ANM_SUMOU_PULL_BACK_GORON) &&
               frameCtrl_p->getFrame() >= 16.0f && frameCtrl_p->getFrame() < 28.0f)
    {
        field_0x2f9d = 0x60;
    }

    return 1;
}


int daAlink_c::procSumouSideMoveInit() {
    commonProcInit(PROC_SUMOU_SIDE_MOVE);
    mSpecialMode = 0x16;
    field_0x2f98 = 3;
    current.angle.y = shape_angle.y + -0x4000;
    field_0x594 = mpHIO->mAtnMove.m.mMaxSpeed;
    setSingleAnimeBase(ANM_SUMOU_FIGHT_WAIT);

    field_0x3198 = 0x141;
    setSumouPunchStatus();
    setDoStatusEmphasys(0x15);
    dComIfGp_setAdvanceDirection(5);

    field_0x2f99 = 12;
    setFootEffectProcType(5);
    mProcVar0.field_0x3008 = 15;

    return 1;
}

int daAlink_c::procSumouSideMove() {
    field_0x2f99 = 4;

    if (!checkSumouVsActor()) {
        return 1;
    }

    if (mProcVar0.field_0x3008 != 0) {
        mProcVar0.field_0x3008--;
    }

    cLib_addCalcAngleS(&shape_angle.y,
                       cLib_targetAngleY(&current.pos, &mCargoCarryAcKeep.getActor()->current.pos),
                       2, 0x2000, 0x800);

    if (field_0x2f98 == 3) {
        current.angle.y = shape_angle.y - 0x4000;
    } else {
        current.angle.y = shape_angle.y + 0x4000;
    }

    int temp_r30_2 = cLib_distanceAngleS(field_0x2fe2, shape_angle.y);
    if (mSpecialMode == 0x1C || mSpecialMode == 0x1B || doTrigger() || sumouPunchTrigger() ||
        (static_cast<daNpcWrestler_c*>(mCargoCarryAcKeep.getActor())->getWrestlerAction() == 3 &&
         !checkZeroSpeedF()))
    {
        return procSumouActionInit(doTrigger(), sumouPunchTrigger(), mProcVar0.field_0x3008);
    }

    setSumouPunchStatus();
    setDoStatusEmphasys(0x15);
    dComIfGp_setAdvanceDirection(5);

    f32 var_f1;
    if (checkInputOnR() && temp_r30_2 > 0x800 && temp_r30_2 < 0x7800) {
        if (abs((s16)(field_0x2fe2 - current.angle.y)) > 0x4000) {
            current.angle.y += 0x8000;
            mNormalSpeed *= -1.0f;

            if (field_0x2f98 == 3) {
                field_0x2f98 = 2;
            } else {
                field_0x2f98 = 3;
            }
        }

        if (field_0x33ac > (1.0f - fabsf(mNormalSpeed / field_0x594)) * 0.3f) {
            var_f1 = mpHIO->mAtnMove.m.mAcceleration * field_0x33ac;
        } else {
            var_f1 = 0.0f;
        }
    } else {
        var_f1 = 0.0f;
    }

    setNormalSpeedF(var_f1, mpHIO->mAtnMove.m.mDeceleration);

    if (checkZeroSpeedF()) {
        onModeFlg(1);
        mSpecialMode = 0x16;
        field_0x2f98 = 3;
        current.angle.y = shape_angle.y - 0x4000;

        if (mUnderAnmHeap[0].getIdx() != (getMainBckData(ANM_SUMOU_FIGHT_WAIT)->m_underID & 0xFFF))
        {
            setSingleAnimeBase(ANM_SUMOU_FIGHT_WAIT);
            mSpeedModifier = 0.0f;
            field_0x3198 = 0x141;
        }
    } else {
        offModeFlg(1);

        daAlink_ANM anm;
        if (field_0x2f98 == 3) {
            anm = ANM_SUMOU_FIGHT_STEP_RIGHT;
        } else {
            anm = ANM_SUMOU_FIGHT_STEP_LEFT;
        }

        if (mUnderAnmHeap[0].getIdx() != (getMainBckData(anm)->m_underID & 0xFFF)) {
            setSingleAnime(anm, 1.0f, 0.0f, 24, 3.0f);
            field_0x3198 = anm;
            mSpeedModifier = 1.0f;
        }

        if (field_0x2f98 == 3) {
            mSpecialMode = 0x18;
        } else {
            mSpecialMode = 0x17;
        }

        mUnderFrameCtrl[0].setRate(mNormalSpeed / field_0x594);
        initBasAnime();
        field_0x2f9d = 0x60;
    }

    return 1;
}


int daAlink_c::procSumouActionInit(BOOL i_doTrigger, BOOL i_punchTrigger, int unused) {
    commonProcInit(PROC_SUMOU_ACTION);

    daNpcWrestler_c* wrestler_p = (daNpcWrestler_c*)mCargoCarryAcKeep.getActor();
    int wrestler_action = wrestler_p->getWrestlerAction();
    mProcVar5.field_0x3012 = 0;

    daAlink_ANM anm;
    if (mSpecialMode == 0x21) {
        mSpecialMode = 0x22;
        anm = ANM_SUMOU_PUNCH_SHOCK;
    } else if (mSpecialMode == 0x1C) {
        i_doTrigger = FALSE;
        i_punchTrigger = FALSE;
        mSpecialMode = 0x1E;

        if (!checkZeroSpeedF()) {
            daAlink_ANM var_r0;
            if (field_0x2f98 == 2) {
                var_r0 = ANM_SUMOU_FIGHT_STEP_LEFT;
            } else {
                var_r0 = ANM_SUMOU_FIGHT_STEP_RIGHT;
            }

            anm = var_r0;
            mProcVar5.field_0x3012 = 1;
        } else {
            anm = ANM_SUMOU_PUNCH_SHOCK;
        }
    } else if (mSpecialMode == 0x1B) {
        i_doTrigger = FALSE;
        i_punchTrigger = FALSE;
        mSpecialMode = 0x1D;
        anm = ANM_SUMOU_TACKLE_SHOCK;
    } else if (wrestler_action == 2) {
        if (i_doTrigger) {
            mSpecialMode = 0x10;
            anm = ANM_SUMOU_DODGE_TACKLE;
        } else if (i_punchTrigger) {
            mSpecialMode = 0x15;
            anm = ANM_SUMOU_PUNCH_MISS_SHOCK_RETURN;
        } else {
            mSpecialMode = 0x1E;

            if (!checkZeroSpeedF()) {
                daAlink_ANM var_r0_2;
                if (field_0x2f98 == 2) {
                    var_r0_2 = ANM_SUMOU_FIGHT_STEP_LEFT;
                } else {
                    var_r0_2 = ANM_SUMOU_FIGHT_STEP_RIGHT;
                }

                anm = var_r0_2;
                mProcVar5.field_0x3012 = 1;
            } else {
                anm = ANM_SUMOU_PUNCH_SHOCK;
            }
        }
    } else if (wrestler_action == 3) {
        if (i_doTrigger) {
            mSpecialMode = 0x12;
            anm = ANM_SUMOU_TACKLE_SHOCK_RETURN;
        } else if (i_punchTrigger) {
            mSpecialMode = 0x14;
            anm = ANM_SUMOU_PUNCH_MISS_SHOCK;
        } else if (!checkZeroSpeedF()) {
            if (field_0x2f98 == 3) {
                mSpecialMode = 0x1A;
                anm = ANM_SUMOU_FIGHT_STEP_RIGHT;
            } else {
                mSpecialMode = 0x19;
                anm = ANM_SUMOU_FIGHT_STEP_LEFT;
            }
        } else {
            mSpecialMode = 0x1D;
            anm = ANM_SUMOU_TACKLE_SHOCK;
        }
    } else if (wrestler_action == 1) {
        if (i_doTrigger) {
            mSpecialMode = 0x11;
            anm = ANM_SUMOU_TACKLE_MISS;
        } else {
            mSpecialMode = 0x13;
            anm = ANM_SUMOU_PUNCH;
        }
    } else if (wrestler_p->getWrestlerType() != 0 && !checkEquipHeavyBoots()) {
        if (i_doTrigger) {
            mSpecialMode = 0x12;
            anm = ANM_SUMOU_TACKLE_SHOCK_RETURN;
        } else {
            mSpecialMode = 0x15;
            anm = ANM_SUMOU_PUNCH_MISS_SHOCK_RETURN;
        }
    } else if (i_doTrigger) {
        mSpecialMode = 0xF;
        if (wrestler_p->getWrestlerType() == 0) {
            anm = ANM_SUMOU_TACKLE;
        } else {
            anm = ANM_SUMOU_TACKLE_GORON;
        }
    } else {
        mSpecialMode = 0x13;
        anm = ANM_SUMOU_PUNCH;
    }

    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::YrotM(shape_angle.y);
    mProcVar3.field_0x300e = 0x82;

    if (mSpecialMode == 0x19) {
        dComIfGp_setAdvanceDirection(1);
        mProcVar4.field_0x3010 = shape_angle.y - 0x2800;
        field_0x2f98 = 2;
    } else if (mSpecialMode == 0x1A) {
        field_0x2f98 = 3;
        dComIfGp_setAdvanceDirection(4);
        mProcVar4.field_0x3010 = shape_angle.y + 0x2800;
    } else if (i_doTrigger) {
        setDoStatusEmphasys(0x15);
        mProcVar3.field_0x300e = 0x15;
        field_0x2f98 = 4;
    } else if (i_punchTrigger) {
        setSumouPunchStatus();
        mProcVar3.field_0x300e = 0x44;
        field_0x2f98 = 4;
    } else if (!checkZeroSpeedF()) {
        if (field_0x2f98 == 2) {
            mProcVar4.field_0x3010 = shape_angle.y - 0x2800;
        } else {
            mProcVar4.field_0x3010 = shape_angle.y + 0x2800;
        }
    } else {
        field_0x2f98 = 4;
    }

    f32 var_f31;
    if ((anm == ANM_SUMOU_FIGHT_STEP_RIGHT || anm == ANM_SUMOU_FIGHT_STEP_LEFT) &&
        mUnderAnmHeap[0].getIdx() != (getMainBckData(anm)->m_underID & 0xFFF))
    {
        var_f31 = mUnderFrameCtrl[0].getFrame();
    } else {
        var_f31 = 0.0f;
    }

    mProcVar2.field_0x300c = 0;
    field_0x3198 = anm;

    int end_frame;
    if (anm == ANM_SUMOU_FIGHT_STEP_LEFT) {
        end_frame = 24;
    } else {
        end_frame = -1;
    }

    setSingleAnime(anm, 1.0f, 0.0f, end_frame, 3.0f);
    mUnderFrameCtrl[0].setFrame(var_f31);
    getNowAnmPackUnder(UNDER_0)->setFrame(var_f31);

    cLib_addCalcAngleS(&shape_angle.y,
                       cLib_targetAngleY(&current.pos, &mCargoCarryAcKeep.getActor()->current.pos),
                       2, 0x2000, 0x800);
    current.angle.y = shape_angle.y;
    mNormalSpeed = 0.0f;

    field_0x3478 = current.pos.absXZ(wrestler_p->current.pos);
    field_0x37c8 = wrestler_p->current.pos;
    field_0x2f99 = 0xC;
    field_0x347c = 1000.0f;
    field_0x3480 = 1000.0f;

    if (anm == ANM_SUMOU_PUNCH_SHOCK || anm == ANM_SUMOU_PUNCH_MISS_SHOCK_RETURN) {
        field_0x347c = 12.0f;
        field_0x32cc = Z2SE_AL_V_SUMO_HARITE_DMG;
    } else if (anm == ANM_SUMOU_PUNCH_MISS_SHOCK) {
        field_0x347c = 11.0f;
        field_0x32cc = Z2SE_AL_V_SUMO_HARITE_ATK;
        field_0x3480 = 35.0f;
    } else if (anm == ANM_SUMOU_PUNCH) {
        field_0x347c = 11.0f;
        field_0x32cc = Z2SE_AL_V_SUMO_HARITE_ATK;
    } else if (anm == ANM_SUMOU_TACKLE || anm == ANM_SUMOU_TACKLE_GORON ||
               anm == ANM_SUMOU_TACKLE_MISS)
    {
        field_0x347c = 2.0f;
        field_0x32cc = Z2SE_AL_V_SUMO_TUCKLE_ATK;
    } else if (anm == ANM_SUMOU_DODGE_TACKLE) {
        field_0x347c = 32.0f;
        field_0x32cc = Z2SE_AL_V_SUMO_TUCKLE_ATK;
    } else if (anm == ANM_SUMOU_TACKLE_SHOCK || anm == ANM_SUMOU_TACKLE_SHOCK_RETURN) {
        field_0x347c = 7.0f;
        field_0x32cc = Z2SE_AL_V_SUMO_HOLDED;
    }

    setFootEffectProcType(5);

    if (field_0x3198 == ANM_SUMOU_FIGHT_STEP_LEFT || field_0x3198 == ANM_SUMOU_FIGHT_STEP_RIGHT) {
        field_0x2f9d = 0x60;
    }

    return 1;
}


int daAlink_c::procSumouAction() {
    field_0x2f99 = 4;

    if (!checkSumouVsActor()) {
        return 1;
    }

    if (field_0x2f98 == 2) {
        dComIfGp_setAdvanceDirection(1);
    } else if (field_0x2f98 == 3) {
        dComIfGp_setAdvanceDirection(4);
    }

    mProcVar2.field_0x300c++;

    if (mSpecialMode == 0x1C || mSpecialMode == 0x1B) {
        return procSumouActionInit(FALSE, FALSE, 0);
    }

    if (mSpecialMode == 0x19 || mSpecialMode == 0x1A) {
        cLib_addCalcAngleS(&shape_angle.y, mProcVar4.field_0x3010, 2, 0x400, 0x40);
        current.angle.y = shape_angle.y;

        current.pos.set(field_0x37c8.x - field_0x3478 * cM_ssin(current.angle.y), field_0x37c8.y,
                        field_0x37c8.z - field_0x3478 * cM_scos(current.angle.y));
        if (mProcVar2.field_0x300c >= 29 || shape_angle.y == mProcVar4.field_0x3010) {
            procSumouSideMoveInit();
        }
    } else {
        if (mSpecialMode == 0x13) {
            cLib_addCalcAngleS(
                &shape_angle.y,
                cLib_targetAngleY(&current.pos, &mCargoCarryAcKeep.getActor()->current.pos), 2,
                0x2000, 0x800);
            current.angle.y = shape_angle.y;
        } else if (mSpecialMode == 0x1E && mProcVar5.field_0x3012 != 0) {
            cLib_addCalcAngleS(&shape_angle.y, mProcVar4.field_0x3010, 2, 0x400, 0x40);
            current.angle.y = shape_angle.y;

            current.pos.set(field_0x37c8.x - field_0x3478 * cM_ssin(current.angle.y),
                            field_0x37c8.y,
                            field_0x37c8.z - field_0x3478 * cM_scos(current.angle.y));
        } else if (mSpecialMode == 0x1E || mSpecialMode == 0x1D) {
            cLib_addCalcAngleS(
                &shape_angle.y,
                cLib_targetAngleY(&current.pos, &mCargoCarryAcKeep.getActor()->current.pos), 2,
                0x600, 0x40);
            current.angle.y = shape_angle.y;
        }

        if (mProcVar3.field_0x300e == 0x15) {
            setDoStatusEmphasys(0x15);
        } else if (mProcVar3.field_0x300e == 0x44) {
            setSumouPunchStatus();
        }

        daPy_frameCtrl_c* frameCtrl_p = mUnderFrameCtrl;
        if (checkAnmEnd(frameCtrl_p)) {
            if (mSpecialMode == 0x1E) {
                procSumouStaggerInit();
            } else if (mSpecialMode == 0x12 || mSpecialMode == 0x22 || mSpecialMode == 0x13 ||
                       mSpecialMode == 0x1A || mSpecialMode == 0x19 || mSpecialMode == 0x11 ||
                       mSpecialMode == 0x15)
            {
                procSumouSideMoveInit();
            } else {
                procSumouMoveInit();
            }
        } else if (mProcVar5.field_0x3012 != 0) {
            if (mProcVar2.field_0x300c > 12.0f) {
                setSingleAnime(ANM_SUMOU_PUNCH_SHOCK, 1.0f, mProcVar2.field_0x300c, -1, 3.0f);
                field_0x2f99 = 12;
                mProcVar5.field_0x3012 = 0;
                voiceStart(Z2SE_AL_V_SUMO_HARITE_DMG);
                field_0x3198 = 0x152;
            }
        } else if (frameCtrl_p->checkPass(field_0x347c)) {
            voiceStart(field_0x32cc);
        } else if (frameCtrl_p->checkPass(field_0x3480)) {
            voiceStart(Z2SE_AL_V_SUMO_HOLDED);
        }
    }

    if (field_0x3198 == 0x142 || field_0x3198 == 0x143) {
        field_0x2f9d = 0x60;
    }

    return 1;
}


int daAlink_c::procSumouStaggerInit() {
    commonProcInit(PROC_SUMOU_STAGGER);

    if (mSpecialMode == 0x11) {
        mSpecialMode = 0x20;
    } else {
        mSpecialMode = 0x1F;
    }

    setSingleAnimeBase(ANM_SUMOU_STAGGER);
    mNormalSpeed = 0.0f;
    field_0x2f99 = 12;
    mProcVar0.field_0x3008 = 30;
    return 1;
}

int daAlink_c::procSumouStagger() {
    field_0x2f99 = 4;

    if (!checkSumouVsActor()) {
        return 1;
    }

    mProcVar0.field_0x3008--;
    if (mSpecialMode == 0x1C || mSpecialMode == 0x21 || mSpecialMode == 0x1B) {
        procSumouActionInit(0, 0, 0);
    } else if (mProcVar0.field_0x3008 == 0) {
        procSumouSideMoveInit();
    }

    return 1;
}

int daAlink_c::procSumouWinLoseInit() {
    if (mSpecialMode == 0xB && mUnderFrameCtrl[0].getAttribute() == 2) {
        cancelSumouMode();
        return procWaitInit();
    }

    daNpcWrestler_c* wrestler_p = (daNpcWrestler_c*)mCargoCarryAcKeep.getActor();
    commonProcInit(PROC_SUMOU_WIN_LOSE);
    field_0x3478 = 1000.0f;

    daAlink_ANM anm = ANM_WAIT;
    if (mSpecialMode == 0xB) {
        mProcVar2.field_0x300c = 1;
    } else if (mSpecialMode == 0xD) {
        field_0x2f99 = 12;
        if (wrestler_p->getWrestlerType() != 0) {
            anm = ANM_SUMOU_WIN_GORON;
        } else {
            anm = ANM_SUMOU_WIN;
        }

        setSingleAnimeBase(anm);
        mProcVar2.field_0x300c = 1;
        field_0x3478 = 10.0f;
        field_0x32cc = Z2SE_AL_V_SUMO_PUSH_LAST;
        field_0x3198 = 13;
    } else {
        f32 start_frame;
        if (mSpecialMode == 12) {
            start_frame = 24.0f;
            voiceStart(Z2SE_AL_V_SUMO_FALL_LOSE);
        } else {
            start_frame = 0.0f;
            field_0x3478 = 23.0f;
            field_0x32cc = Z2SE_AL_V_SUMO_FALL_LOSE;
        }

        if (wrestler_p->getWrestlerType() != 0) {
            anm = ANM_SUMOU_LOSE_GORON;
        } else {
            anm = ANM_SUMOU_LOSE;
        }

        setSingleAnime(anm, 1.0f, start_frame, -1, 3.0f);
        field_0x2f99 = 15;
        mProcVar2.field_0x300c = 0;
        current.pos.y += 70.0f;
    }

    field_0x3198 = anm;
    mNormalSpeed = 0.0f;
    setSpecialGravity(0.0f, maxFallSpeed, 0);
    speed.y = 0.0f;
    setFootEffectProcType(5);
    mProcVar3.field_0x300e = 0;
    field_0x33b0 = 20.0f;
    return 1;
}


int daAlink_c::procSumouWinLose() {
    if (mProcVar3.field_0x300e == 0) {
        if (mProcVar2.field_0x300c != 0) {
            field_0x2f99 = 4;
        } else {
            field_0x2f99 = 7;
        }
    }

    daPy_frameCtrl_c* frameCtrl_p = mUnderFrameCtrl;

    if (frameCtrl_p->checkPass(field_0x3478)) {
        voiceStart(field_0x32cc);
    }

    if (checkAnmEnd(frameCtrl_p)) {
        if (mProcVar2.field_0x300c != 0 || mProcVar3.field_0x300e != 0) {
            cancelSumouMode();
            procWaitInit();
        } else {
            setSingleAnimeBase(ANM_SUMOU_KNOCKED_DOWN);
            field_0x2f99 = 12;
            field_0x3478 = 1000.0f;
            field_0x3198 = ANM_SUMOU_KNOCKED_DOWN;
        }
    } else if (mProcVar3.field_0x300e == 0 && mSpecialMode == 0x25) {
        setSingleAnimeBase(ANM_SUMOU_GET_UP);
        mProcVar3.field_0x300e = 1;
        field_0x3478 = 1000.0f;
        field_0x3198 = ANM_SUMOU_GET_UP;
    }

    if ((field_0x3198 == ANM_SUMOU_LOSE || field_0x3198 == ANM_SUMOU_LOSE_GORON) &&
        frameCtrl_p->checkPass(37.0f))
    {
        field_0x2f9d = 4;
    }

    return 1;
}


int daAlink_c::procSumouShikoInit() {
    if (!commonProcInitNotSameProc(PROC_SUMOU_SHIKO)) {
        return 0;
    }

    setSingleAnimeBase(ANM_SUMOU_SHIKO);
    mNormalSpeed = 0.0f;
    field_0x3588 = l_waitBaseAnime;
    field_0x34d4 = field_0x3588;
    field_0x2f99 = 12;
    mProcVar2.field_0x300c = 0;
    mSpecialMode = 0;

    setFootEffectProcType(5);
    deleteEquipItem(0, 1);
    return 1;
}

int daAlink_c::procSumouShiko() {
    daPy_frameCtrl_c* frameCtrl_p = mUnderFrameCtrl;
    if (field_0x2f99 != 12) {
        field_0x2f99 = 4;
    }

    if (mSpecialMode == 1) {
        return procSumouReadyInit();
    }

    if (mProcVar2.field_0x300c == 0) {
        if (checkAnmEnd(frameCtrl_p)) {
            setSingleAnimeBaseMorf(ANM_SUMOU_SHIKO_WAIT, -1.0f);
            field_0x2f99 = 12;
            mProcVar2.field_0x300c = 1;
        } else if (frameCtrl_p->checkPass(127.0f)) {
            field_0x2f9d = 0x20;
        } else if (frameCtrl_p->checkPass(65.0f)) {
            voiceStart(Z2SE_AL_V_SUMO_SHIKO);
        } else if (frameCtrl_p->checkPass(9.0f)) {
            field_0x2f92 = 0xFE;
            field_0x2f93 = 0xFE;
        }
    } else {
        dComIfGp_evmng_cutEnd(mAlinkStaffId);
    }

    return 1;
}
