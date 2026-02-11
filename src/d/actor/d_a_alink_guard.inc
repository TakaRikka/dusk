/**
 * d_a_alink_guard.inc
 * Player Shield action handling
 */

#include "d/actor/d_a_alink.h"
#include "d/d_com_inf_game.h"
#include "d/actor/d_a_obj_gra2.h"
#include "d/actor/d_a_npc_kn.h"

BOOL daAlink_c::checkEnemyGroup(fopAc_ac_c* i_actor) {
    return i_actor != NULL
            && (fopAcM_GetGroup(i_actor) == fopAc_ENEMY_e || fopAcM_GetName(i_actor) == PROC_E_ZH || checkSpecialNpc(i_actor));
}

BOOL daAlink_c::checkSpecialNpc(fopAc_ac_c* i_actor) {
    s16 name = fopAcM_GetName(i_actor);

    return name == PROC_NPC_KAKASHI ||
          (name == PROC_NPC_KN && static_cast<daNpc_Kn_c*>(i_actor)->checkBattleMode()) ||
          (name == PROC_OBJ_GRA && !static_cast<daObj_GrA_c*>(i_actor)->checkNpcObjGra());
}

BOOL daAlink_c::checkShieldAttackEmphasys() {
    return mTargetedActor != NULL && (mTargetedActor->attention_info.flags & fopAc_AttnFlag_UNK_0x200000) &&
           !checkCutHeadState() &&
           (fopAcM_GetGroup(mTargetedActor) != fopAc_ENEMY_e ||
            !static_cast<fopEn_enemy_c*>(mTargetedActor)->checkDownFlg());
}

BOOL daAlink_c::checkGuardActionChange() {
    return checkAttentionLock() && !checkIronBallWaitAnime() && !checkGrabAnime() &&
           !checkCopyRodControllAnime() && !((mTargetedActor != NULL && !checkSpecialNpc(mTargetedActor)) &&
           (fopAcM_GetGroup(mTargetedActor) == 3 || fopAcM_GetGroup(mTargetedActor) == fopAc_NPC_e));
}

void daAlink_c::stickArrowIncrement(int param_0) {
    if (!checkNoResetFlg2(FLG2_STATUS_WINDOW_DRAW)) {
        s16 arrow_num = 0;

        for (int i = 0; i < 16; i++) {
            if (mShieldArrowIDs[i] != fpcM_ERROR_PROCESS_ID_e) {
                if (field_0x2f7c[i] == 0 || param_0) {
                    arrow_num++;
                    mShieldArrowIDs[i] = fpcM_ERROR_PROCESS_ID_e;
                    field_0x2f7c[i] = 0;
                } else {
                    field_0x2f7c[i]--;
                }
            }
        }

        if (arrow_num != 0) {
            if (checkShieldGet()
                #if !PLATFORM_SHIELD
                && checkItemGet(fpcNm_ITEM_BOW, true)
                #endif
                )
            {
                int arrow_type;
                if (arrow_num == 1) {
                    arrow_type = fpcNm_ITEM_ARROW_1;
                } else {
                    arrow_type = fpcNm_ITEM_ARROW_10;
                }

                dComIfGp_setItemArrowNumCount(arrow_num);
                fopAcM_createItemForSimpleDemo(&current.pos, arrow_type, -1, NULL, NULL, 0.0f,
                                               0.0f);
            }

            onResetFlg0(RFLG0_STICK_ARROW_RESET);
        }
    }
}

void daAlink_c::setArrowShieldActor(fopAc_ac_c* i_actor, int param_1) {
    fpc_ProcID new_arrow_id = fopAcM_GetID(i_actor);

    int arrow_num = 0;
    for (; arrow_num < 16; arrow_num++) {
        if (mShieldArrowIDs[arrow_num] == fpcM_ERROR_PROCESS_ID_e) {
            mShieldArrowIDs[arrow_num] = new_arrow_id;
            field_0x2f7c[arrow_num] = 10;
            break;
        }
    }

    if (arrow_num == 16) {
        fopAc_ac_c* first_arrow = fopAcM_SearchByID(mShieldArrowIDs[0]);
        if (first_arrow != NULL) {
            fopAcM_delete(first_arrow);
        }

        int i = 0;
        for (; i < 15; i++) {
            mShieldArrowIDs[i] = mShieldArrowIDs[i + 1];
            field_0x2f7c[i] = field_0x2f7c[i + 1];
        }

        mShieldArrowIDs[i] = new_arrow_id;
        field_0x2f7c[i] = 10;

        dComIfGp_setItemArrowNumCount(1);
        fopAcM_createItemForSimpleDemo(&current.pos, fpcNm_ITEM_ARROW_10, -1, NULL, NULL, 0.0f, 0.0f);
    }

    if (param_1 && field_0x2fcb == 0 && !checkMagicArmorNoDamage()) {
        field_0x2fcb = 120;
    }
}

BOOL daAlink_c::checkWoodShieldEquipNotIronBall() const {
    return checkWoodShieldEquip() && !checkIronBallEquip();
}

void daAlink_c::getArrowShieldOffset(const cXyz* param_0, const csXyz* param_1, cXyz* param_2,
                                     cXyz* param_3) const {
    cXyz sp54;
    mDoMtx_stack_c::ZXYrotS(param_1->x + cM_rndFX(0x1000), param_1->y + cM_rndFX(0x1000), 0);
    mDoMtx_stack_c::multVec(&cXyz::BaseZ, &sp54);
    mDoMtx_stack_c::copy(getShieldMtx());
    mDoMtx_stack_c::inverse();
    mDoMtx_stack_c::multVecSR(&sp54, param_3);

    static const Vec localCenter = {-15.0f, 0.0f, 18.0f};
    cXyz sp60;
    mDoMtx_multVec(getShieldMtx(), &localCenter, &sp60);

    cM3dGPla plane(&field_0x351c, -field_0x351c.inprod(sp60));

    cXyz sp6C;
    cXyz sp78 = *param_0 + sp54;
    plane.crossInfLin(*param_0, sp78, sp6C);

    if (sp6C.abs(sp60) > 20.0f) {
        sp6C = sp6C - sp60;
        sp6C.normalize();
        sp6C *= 20.0f;
        sp6C += sp60;
    }

    mDoMtx_stack_c::multVec(&sp6C, param_2);
}

void daAlink_c::setArrowShieldPos(cXyz* param_0, csXyz* param_1, const cXyz* param_2,
                                  const cXyz* param_3) const {
    cXyz sp1C;

    mDoMtx_multVecSR(getShieldMtx(), param_3, &sp1C);
    param_1->x = sp1C.atan2sY_XZ();
    param_1->y = sp1C.atan2sX_Z();

    mDoMtx_multVec(getShieldMtx(), param_2, param_0);
}

BOOL daAlink_c::checkUpperGuardAnime() const {
    return checkNoResetFlg2(FLG2_UNK_8000000) &&
           !checkEventRun() &&
           !checkNoResetFlg0(FLG0_UNK_2) &&
           mProcID != PROC_GUARD_BREAK &&
           mProcID != PROC_CUT_REVERSE &&
           mProcID != PROC_GUARD_ATTACK;
}

BOOL daAlink_c::checkPlayerGuard() const {
    return (checkSmallUpperGuardAnime() || (checkShieldGet() && checkHorseLieAnime())) ||
           checkUpperGuardAnime();
}

BOOL daAlink_c::checkPlayerGuardAndAttack() const {
    return checkPlayerGuard() || mProcID == PROC_GUARD_ATTACK;
}

BOOL daAlink_c::checkGuardAccept() {
    return checkModeFlg(0x80) &&
           checkShieldGet() &&
           !checkFmChainGrabAnime() &&
           !checkNotBattleStage();
}

void daAlink_c::setUpperGuardAnime(f32 param_0) {
    UNUSED(param_0);
    onNoResetFlg2(FLG2_UNK_8000000);
}

void daAlink_c::setShieldGuard() {
    f32 var_f31 = mNowAnmPackUpper[2].getRatio();

    if ((mProcID == PROC_GUARD_SLIP && mEquipItem != fpcNm_ITEM_IRONBALL) || checkSmallUpperGuardAnime() ||
        (checkGuardAccept() && !checkGrabAnime() && !checkUpperReadyThrowAnime() &&
         !checkDkCaught2Anime() && !checkKandelaarSwingAnime() && !checkCutDashAnime() &&
         !checkCutDashChargeAnime() && (!checkEquipAnime() || checkUpperGuardAnime()) &&
         !checkRideOn() && checkGuardActionChange()))
    {
        onNoResetFlg2(FLG2_UNK_8000000);
    } else {
        offNoResetFlg2(FLG2_UNK_8000000);
    }
}

void daAlink_c::setGuardSe(dCcD_GObjInf* i_objinf) {
    int var_r31;
    BOOL isRebound;

    if (checkWoodShieldEquipNotIronBall() && !checkMagicArmorNoDamage()) {
        var_r31 = 0x29;
        isRebound = FALSE;
    } else {
        var_r31 = 0x28;
        isRebound = TRUE;
    }

    u32 hit_se;
    if (i_objinf->GetTgHitGObj() != NULL && i_objinf->GetTgHitGObj()->GetAtType() == AT_TYPE_BOMB) {
        hit_se = Z2SE_HIT_METAL_WEAPON;
    } else {
        hit_se = i_objinf->GetTgHitObjHitSeID(isRebound);
    }

    mZ2Link.startCollisionSE(hit_se, var_r31);
}

void daAlink_c::setSmallGuard(dCcD_GObjInf* i_objinf) {
    setUpperAnimeBase(dRes_ID_ALANM_BCK_ATDEFS_e);
    cXyz* dmg_vec = getDamageVec(i_objinf);

    mBodyAngle.y = (dmg_vec->atan2sX_Z() + 0x8000) - shape_angle.y;
    if (abs(mBodyAngle.y) > 0x7000) {
        mBodyAngle.y = 0;
    } else {
        mBodyAngle.y =
            cLib_minMaxLimit<s16>((s16)mBodyAngle.y, -mpHIO->mGuard.m.mSmallGuardLRAngleMax,
                                  mpHIO->mGuard.m.mSmallGuardLRAngleMax);
    }

    mBodyAngle.x = cLib_minMaxLimit<s16>(cM_atan2s(dmg_vec->y, dmg_vec->absXZ()), -mpHIO->mGuard.m.mSmallGuardFBAngleMax, mpHIO->mGuard.m.mSmallGuardFBAngleMax);
}

int daAlink_c::procGuardSlipInit(int i_atSpl, dCcD_GObjInf* i_objinf) {
    if (mProcID == PROC_GUARD_SLIP) {
        return 0;
    }

    field_0x2fd5++;
    if (field_0x2fd5 == 4) {
        field_0x2fd5 = 0;
        return procGuardBreakInit();
    }

    commonProcInit(PROC_GUARD_SLIP);
    field_0x2fd4 = 3;
    
    cXyz* dmg_vec = getDamageVec(i_objinf);
    current.angle.y = dmg_vec->atan2sX_Z();

    const daAlinkHIO_anm_c* anmParams;
    daAlink_ANM anm;
    if (mEquipItem == fpcNm_ITEM_IRONBALL) {
        setSingleAnimeBase(ANM_IRONBALL_DEF);
        setIronBallWaitUpperAnime(1);
        mNowAnmPackUpper[2].setRatio(0.0f);

        mProcVar1.field_0x300a = 0;
        mProcVar2.field_0x300c = 0;
        mProcVar3.field_0x300e = 0;
    } else {
        anmParams = &mpHIO->mGuard.m.mGuardHitAnm;
        if (field_0x2f98 == 2) {
            anm = ANM_GUARD_LEFT;
        } else {
            anm = ANM_GUARD_RIGHT;
        }

        setSingleAnimeParam(anm, anmParams);
        setUpperGuardAnime(-1.0f);

        mProcVar2.field_0x300c = (current.angle.y + 0x8000) - shape_angle.y;
        if (abs(mProcVar2.field_0x300c) > 0x7000) {
            mProcVar2.field_0x300c = 0;
        } else {
            mProcVar2.field_0x300c = cLib_minMaxLimit<s16>((s16)mProcVar2.field_0x300c, -mpHIO->mGuard.m.mGuardLRAngleMax, mpHIO->mGuard.m.mGuardLRAngleMax);
        }

        mProcVar3.field_0x300e = cLib_minMaxLimit<s16>(cM_atan2s(dmg_vec->y, dmg_vec->absXZ()), -mpHIO->mGuard.m.mGuardFBAngleMax, mpHIO->mGuard.m.mGuardFBAngleMax);
        mProcVar1.field_0x300a = mpHIO->mGuard.m.mGuardBodyInterpolation + 1;
        mProcVar2.field_0x300c = (mProcVar2.field_0x300c - mBodyAngle.y) / mProcVar1.field_0x300a;
        mProcVar3.field_0x300e = (mProcVar3.field_0x300e - mBodyAngle.x) / mProcVar1.field_0x300a;
        mProcVar1.field_0x300a--;

        mBodyAngle.y += mProcVar2.field_0x300c;
        mBodyAngle.x += mProcVar3.field_0x300e;
    }

    mProcVar4.field_0x3010 = 0;
    mProcVar5.field_0x3012 = 0;

    if (i_objinf != NULL && i_objinf->GetTgHitAc() != NULL && fopAcM_GetName(i_objinf->GetTgHitAc()) == PROC_E_MM_MT && i_objinf->GetTgHitAc()->argument == 1) {
        if (checkBootsOrArmorHeavy()) {
            mNormalSpeed = mpHIO->mGuard.m.mMagneHvyGuardSpeed;
            mProcVar5.field_0x3012 = 1;
        } else {
            mNormalSpeed = mpHIO->mGuard.m.mMagneGuardSpeed;
            mProcVar5.field_0x3012 = 0;
        }

        mProcVar4.field_0x3010 = 1;
    } else if (checkHugeAttack(i_atSpl)) {
        if ((!checkBootsOrArmorHeavy() && checkMiddleBossGoronRoom()) || checkStageName("D_MN07B")) {
            mNormalSpeed = 40.0f;
        } else {
            mNormalSpeed = mpHIO->mGuard.m.mGuardSpeedHuge;
        }
    } else if (checkLargeAttack(i_atSpl)) {
        mNormalSpeed = mpHIO->mGuard.m.mGuardSpeedLarge;
    } else {
        mNormalSpeed = mpHIO->mGuard.m.mGuardSpeedNormal;
    }

    field_0x2f9d = 0x60;
    setFootEffectProcType(1);
    return 1;
}

int daAlink_c::procGuardSlip() {
    if (mEquipItem != fpcNm_ITEM_IRONBALL) {
        setUpperGuardAnime(-1.0f);
    }

    setShapeAngleToAtnActor(0);

    if (mProcVar1.field_0x300a != 0) {
        mProcVar1.field_0x300a -= (s16)1;
        mBodyAngle.y += mProcVar2.field_0x300c;
        mBodyAngle.x += mProcVar3.field_0x300e;
    }

    if (mNormalSpeed > 2.5f) {
        field_0x2f9d = 0x60;
    }

    f32 speed_step;
    if (checkBootsOrArmorHeavy() && mProcVar4.field_0x3010 == 0) {
        speed_step = 2.5f;
    } else {
        speed_step = 1.25f;
    }

    if (mProcVar4.field_0x3010 != 0 && mProcVar5.field_0x3012 != checkBootsOrArmorHeavy()) {
        mNormalSpeed = 0.0f;
    }

    if (cLib_chaseF(&mNormalSpeed, 0.0f, speed_step) != 0 && checkAnmEnd(&mUnderFrameCtrl[0])) {
        mCcStts.SetTgApid(0);

        if (!checkGuardActionChange()) {
            onEndResetFlg1(ERFLG1_UNK_20);
        }

        if (mEquipItem == fpcNm_ITEM_IRONBALL) {
            mNowAnmPackUpper[2].setRatio(1.0f);
        }

        checkNextAction(0);
    }

    return 1;
}

int daAlink_c::procGuardAttackInit() {
    if (mDemo.getDemoMode() == daPy_demo_c::DEMO_GUARD_ATTACK_e && mProcID == PROC_GUARD_ATTACK) {
        return 1;
    }

    commonProcInit(PROC_GUARD_ATTACK);
    daAlink_ANM anm = ANM_SHIELD_ATTACK;
    const daAlinkHIO_guardAttack_c1* params = &mpHIO->mGuard.mAtPush.m;
    
    field_0x2f98 = 3;
    field_0x34d4 = l_rWaitBaseAnime;
    field_0x3588 = l_rWaitBaseAnime;

    setCutType(CUT_TYPE_GUARD_ATTACK);
    u8 atSe = dCcD_SE_SHIELD_ATTACK;

    setSingleAnimeParam(anm, &params->mAttackAnm);

    field_0x3478 = params->mAttackStartFrame;
    field_0x347c = params->mAttackEndFrame;
    field_0x3480 = params->mSpeed;

    if (checkHeavyStateOn(TRUE, TRUE)) {
        field_0x3480 *= mHeavySpeedMultiplier;
    }

    if (mDemo.getDemoMode() == daPy_demo_c::DEMO_GUARD_ATTACK_e) {
        field_0x3484 = 1000.0f;
        field_0x3488 = 1000.0f;
    } else {
        field_0x3484 = params->mAttackAnm.mCancelFrame;
        field_0x3488 = params->mSlashCheckFrame;
    }

    mNormalSpeed = 0.0f;
    setSwordVoiceSe(Z2SE_AL_V_TATE_OSHI);

    mGuardAtCps.ResetAtHit();
    mGuardAtCps.SetAtSe(atSe);
    mGuardAtCps.SetR(mpHIO->mGuard.m.mAttackRadius);

    current.angle.y = shape_angle.y;

    setUpperGuardAnime(-1.0f);
    mProcVar2.field_0x300c = 0;
    mProcVar3.field_0x300e = 0;
    mProcVar5.field_0x3012 = 0;
    mProcVar0.field_0x3008 = 0;
    return 1;
}

int daAlink_c::procGuardAttack() {
    daPy_frameCtrl_c* framectrl = &mUnderFrameCtrl[0];

    setUpperGuardAnime(-1.0f);
    field_0x2f99 = 4;
    cLib_chaseF(&mNormalSpeed, 0.0f, mpHIO->mMove.m.mDeceleration);
    setShapeAngleToAtnActor(1);
    current.angle.y = shape_angle.y;

    if (checkForceSwordSwing()) {
        mProcVar2.field_0x300c = 1;
    }

    BOOL var_r29;
    if (mEquipItem == 0x103 && !checkMagneBootsOn()) {
        var_r29 = TRUE;
        if (doTrigger()) {
            mProcVar3.field_0x300e = 1;
        }
    } else {
        var_r29 = FALSE;
    }

    if (mProcVar0.field_0x3008 == 0 && mGuardAtCps.ChkAtHit()) {
        mProcVar0.field_0x3008 = 1;
        dComIfGp_getVibration().StartShock(VIBMODE_S_POWER3, 1, cXyz(0.0f, 1.0f, 0.0f));
    }

    if (mProcVar5.field_0x3012 != 0 && commonLineCheck(&field_0x3834, &mRightHandPos) && dComIfG_Bgsp().GetWallCode(mLinkLinChk) != 7) {
        mZ2Link.startHitItemSE(Z2SE_HIT_SHIELD_ATTACK, dKy_pol_sound_get(&mLinkLinChk), NULL, -1.0f);
        return procCutReverseInit(ANM_SHIELD_ATTACK_RECOIL);
    }

    if (var_r29) {
        if (checkCutHeadState()) {
            setDoStatusEmphasys(BUTTON_STATUS_HELM_SPLITTER);
        } else {
            setDoStatus(BUTTON_STATUS_UNK_134);
        }
    }

    if (checkAnmEnd(framectrl)) {
        if (mDemo.getDemoMode() == daPy_demo_c::DEMO_GUARD_ATTACK_e) {
            dComIfGp_evmng_cutEnd(mAlinkStaffId);
        } else {
            checkNextAction(0);
        }
    } else {
        if (framectrl->getFrame() > field_0x3488) {
            if (mProcVar3.field_0x300e != 0) {
                return checkDoCutAction();
            }

            if (mProcVar2.field_0x300c != 0) {
                return checkCutAction();
            }
        }

        if (framectrl->getFrame() > field_0x3484) {
            onModeFlg(4);
            checkNextAction(1);
        } else if (framectrl->getFrame() >= field_0x3478 && framectrl->getFrame() < field_0x347c) {
            if (mProcVar5.field_0x3012 == 0) {
                mNormalSpeed = field_0x3480;
            }
            mProcVar5.field_0x3012 = 1;
        } else {
            mProcVar5.field_0x3012 = 0;
        }
    }

    return 1;
}

int daAlink_c::procGuardBreakInit() {
    commonProcInit(PROC_GUARD_BREAK);
    setSingleAnimeParam(ANM_GUARD_BREAK, &mpHIO->mGuard.m.mGuardBreakAnm);

    if (mEquipItem == fpcNm_ITEM_IRONBALL) {
        deleteEquipItem(FALSE, FALSE);
    }

    mNormalSpeed = 0.0f;
    field_0x3588 = l_halfAtnWaitBaseAnime;
    dComIfGp_getVibration().StartShock(VIBMODE_S_POWER5, 0x1F, cXyz(0.0f, 1.0f, 0.0f));
    voiceStart(Z2SE_AL_V_GUARD_BROKEN);
    return 1;
}

int daAlink_c::procGuardBreak() {
    daPy_frameCtrl_c* framectrl = &mUnderFrameCtrl[0];

    setUpperGuardAnime(-1.0f);
    onEndResetFlg0(ERFLG0_UNK_8000000);
    field_0x2f99 = 4;

    if (checkAnmEnd(framectrl)) {
        if (checkGuardActionChange() == 0) {
            onEndResetFlg1(ERFLG1_UNK_20);
        }
        checkNextAction(0);
    } else if (framectrl->getFrame() > mpHIO->mGuard.m.mGuardBreakAnm.mCancelFrame) {
        if (!checkGuardActionChange()) {
            onEndResetFlg1(ERFLG1_UNK_20);
        }

        onModeFlg(4);
        checkNextAction(1);
    }

    return 1;
}

int daAlink_c::procTurnMoveInit(int param_0) {
    BOOL isUpperGuardAnime = checkUpperGuardAnime();
    fopAc_ac_c* targetedActor = mTargetedActor;

    commonProcInit(PROC_TURN_MOVE);
    mNormalSpeed = 0.0f;

    daAlink_ANM anm;
    if (param_0 == 2) {
        anm = ANM_SIDE_ROLL_LEFT;
    } else if (param_0 == 3) {
        anm = ANM_SIDE_ROLL_RIGHT;
    } else if (checkInputOnR()) {
        if ((s16)(mMoveAngle - shape_angle.y) >= 0) {
            anm = ANM_SIDE_ROLL_LEFT;
        } else {
            anm = ANM_SIDE_ROLL_RIGHT;
        }
    } else {
        anm = ANM_SIDE_ROLL_LEFT;
    }

    f32 anm_speed = mpHIO->mGuard.mTurnMove.m.mTurnAnm.mSpeed;
    mProcVar1.field_0x300a = mpHIO->mGuard.mTurnMove.m.mMaxTurnSpeed;

    if (checkNoResetFlg0(FLG0_WATER_IN_MOVE)) {
        mProcVar1.field_0x300a = mProcVar1.field_0x300a * mpHIO->mItem.mIronBoots.m.mWaterVelocityX;
    }

    setSingleAnime(anm, anm_speed, mpHIO->mGuard.mTurnMove.m.mTurnAnm.mStartFrame, mpHIO->mGuard.mTurnMove.m.mTurnAnm.mEndFrame, mpHIO->mGuard.mTurnMove.m.mTurnAnm.mInterpolation);

    if (anm == ANM_SIDE_ROLL_LEFT) {
        mProcVar2.field_0x300c = -0x7FF0;
        field_0x2f98 = 2;
    } else {
        mProcVar2.field_0x300c = 0x7FF0;
        field_0x2f98 = 3;
    }

    if (targetedActor != NULL) {
        cXyz sp8 = current.pos - targetedActor->current.pos;
        shape_angle.y = fopAcM_searchActorAngleY(this, targetedActor);
        field_0x3478 = sp8.absXZ();
        mProcVar3.field_0x300e = sp8.atan2sX_Z();
    } else {
        field_0x3478 = 200.0f;
        mProcVar3.field_0x300e = shape_angle.y + 0x8000;
    }

    field_0x347c = (field_0x3478 - 150.0f) / mProcVar2.field_0x300c;
    voiceStart(Z2SE_AL_V_SOTOMO_ROLL);

    if (mDemo.getDemoMode() == daPy_demo_c::DEMO_UNK_82_e) {
        mProcVar0.field_0x3008 = 1;
    } else {
        mProcVar0.field_0x3008 = 0;
    }

    setFootEffectProcType(0);
    field_0x2f9d = 4;
    dComIfGp_setPlayerStatus0(0, 4);

    if (isUpperGuardAnime) {
        onNoResetFlg0(FLG0_UNK_2);
    }

    return 1;
}

int daAlink_c::procTurnMove() {
    onEndResetFlg0(ERFLG0_UNK_8000000);
    s16 temp_r28 = mProcVar2.field_0x300c;

    cLib_addCalcAngleS(&mProcVar2.field_0x300c, 0, mpHIO->mGuard.mTurnMove.m.mTurnSpeedRate, mProcVar1.field_0x300a, mpHIO->mGuard.mTurnMove.m.mMinTurnSpeed);

    s16 temp_r29 = temp_r28 - mProcVar2.field_0x300c;
    if (temp_r28 > 0) {
        current.angle.y = mProcVar3.field_0x300e + 0x4000;
    } else {
        current.angle.y = mProcVar3.field_0x300e - 0x4000;
    }

    f32 temp_f31 = current.pos.x - (field_0x3478 * cM_ssin(mProcVar3.field_0x300e));
    f32 temp_f30 = current.pos.z - (field_0x3478 * cM_scos(mProcVar3.field_0x300e));

    mProcVar3.field_0x300e += temp_r29;
    field_0x3478 -= field_0x347c * temp_r29;

    current.pos.x = temp_f31 + (field_0x3478 * cM_ssin(mProcVar3.field_0x300e));
    current.pos.z = temp_f30 + (field_0x3478 * cM_scos(mProcVar3.field_0x300e));

    shape_angle.y = mProcVar3.field_0x300e + 0x8000;
    mProcVar0.field_0x3008 |= checkCutFinishJumpUp();

    daPy_frameCtrl_c* framectrl = &mUnderFrameCtrl[0];

    if (checkNoResetFlg0(FLG0_UNK_2)) {
        setUpperGuardAnime(-1.0f);
    }

    if (checkAnmEnd(framectrl)) {
        if (!checkAttentionLock()) {
            offNoResetFlg2(FLG2_UNK_8000000);
        }
        checkNextAction(0);
    } else if (mProcVar0.field_0x3008 != 0 && !checkNotJumpSinkLimit() && framectrl->getFrame() > mpHIO->mGuard.mTurnMove.m.mTwirlCutDelayF) {
        procCutFinishJumpUpInit();
        dComIfGp_setPlayerStatus0(0, 4);
    } else {
        if (framectrl->getFrame() > mpHIO->mGuard.mTurnMove.m.mTurnAnm.mCancelFrame) {
            onModeFlg(4);

            if (checkNextAction(1)) {
                return 1;
            }
        }

        if (framectrl->checkPass(19.0f)) {
            dComIfGp_getVibration().StartShock(VIBMODE_S_POWER1, 1, cXyz(0.0f, 1.0f, 0.0f));
        }

        if (abs(temp_r29) > 0x100) {
            field_0x2f9d = 4;
        }
    }

    return 1;
}
