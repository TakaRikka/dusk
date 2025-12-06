/**
 * d_a_alink_damage.inc
 * Player Damage action handling
 */

#include "d/actor/d_a_alink.h"
#include "d/d_com_inf_game.h"
#include "d/actor/d_a_horse.h"
#include "d/actor/d_a_crod.h"
#include "d/d_msg_object.h"

s16 daAlink_c::getFreezeR() const {
    return mpHIO->mDamage.m.mFreezeR;
}

s16 daAlink_c::getFreezeG() const {
    return mpHIO->mDamage.m.mFreezeG;
}

s16 daAlink_c::getFreezeB() const {
    return mpHIO->mDamage.m.mFreezeB;
}

bool daAlink_c::checkMiddleBossGoronRoom() {
    return checkStageName("D_MN04B");
}

void daAlink_c::setDkCaught(fopAc_ac_c* param_0) {
    if (!checkNoResetFlg1(FLG1_DK_CAUGHT)) {
        onNoResetFlg1(FLG1_DK_CAUGHT);
        field_0x28f8 = fopAcM_GetID(param_0);
    }
}

void daAlink_c::freezeTimerDamage() {
    mProcVar0.field_0x3008--;
    mProcVar1.field_0x300a++;

    if (mProcVar1.field_0x300a % 45 == 0) {
        setDamagePoint(1, 1, 0, 0);
    }

    if (escapeTrigger()) {
        mProcVar0.field_0x3008 -= 2;
    }

    if (checkInputOnR() && abs((s16)(mStickAngle - mPrevStickAngle)) > 0x1000) {
        mProcVar0.field_0x3008 -= 2;
    }

    if (mProcVar0.field_0x3008 < 0) {
        mProcVar0.field_0x3008 = 0;
    }
}

void daAlink_c::onPressedDamage(cXyz const& param_0, s16 param_1) {
    onNoResetFlg2(FLG2_PRESSED_DAMAGE);
    field_0x3104 = param_1;
    field_0x3744 = param_0;
}

BOOL daAlink_c::checkNoLandDamageSlidePolygon() {
    if (field_0x2fbb == 3) {
        return true;
    }

    if (dComIfG_Bgsp().ChkPolySafe(mLinkAcch.m_gnd)) {
        cM3dGPla poly;
        dComIfG_Bgsp().GetTriPla(mLinkAcch.m_gnd, &poly);

        if (poly.mNormal.y < field_0x3470) {
            return true;
        }
    }

    return false;
}


void daAlink_c::checkCutLandDamage() {
    if (!checkEventRun()) {
        f32 var_f31 = (mFallHeight - current.pos.y) * 0.0099999998f;

        if (var_f31 >= mpHIO->mDamage.mDamFall.m.mSmallDmgHeight && !checkNoLandDamageSlidePolygon())
        {
            if (var_f31 >= mpHIO->mDamage.mDamFall.m.mBigDmgHeight) {
                setLandDamagePoint(8);
            } else {
                setLandDamagePoint(4);
            }

            onModeFlg(8);
        }
    }
}

BOOL daAlink_c::checkCaughtEscapeCutTurn() {
    if (mLinkAcch.ChkGroundHit() && (checkSwordGet() || checkWolf()) &&
        checkCutTurnInputTrigger())
    {
        if (checkWolf()) {
            return procWolfRollAttackInit(2, 0);
        }

        if (mEquipItem != 0x103) {
            deleteEquipItem(0, 0);
            setSwordModel();
        }

        return procCutTurnInit(1, 2);
    }

    return 0;
}

bool daAlink_c::setThrowDamage(s16 param_0, f32 param_1, f32 param_2, int param_3, int param_4,
                               int param_5) {
    field_0x3408 = param_1;
    field_0x340c = param_2;
    field_0x318c = param_3;
    field_0x2ffe = param_0;

    if (param_4) {
        onEndResetFlg0(ERFLG0_UNK_40);
    } else {
        onEndResetFlg0(ERFLG0_UNK_80);
    }

    if (param_5 == 1) {
        onEndResetFlg0(ERFLG0_UNK_40000000);
    } else if (param_5 == 2) {
        onEndResetFlg1(ERFLG1_UNK_80);
    }

    if (getSumouMode()) {
        cancelSumouMode();
    }

    return true;
}

f32 daAlink_c::damageMagnification(int i_checkZoraMag, int param_1) {
    f32 base_mag;

    if (param_1 == 0 && checkNoResetFlg3(FLG3_UNK_40000000) &&
        !checkEndResetFlg2(ERFLG2_UNK_40))
    {
        base_mag = 1.5f;
    } else {
        base_mag = 1.0f;
    }

    if (checkWolf() && !checkCargoCarry() && param_1 == 0) {
        return 2.0f * base_mag;
    }

    if (checkZoraWearAbility() && i_checkZoraMag) {
        return 10.0f * base_mag;
    }

    return base_mag;
}

int daAlink_c::setDamagePoint(int i_dmgAmount, int i_checkZoraMag, int i_setDmgTimer, int param_3) {
    if (i_dmgAmount <= 0) {
        dComIfGp_setItemLifeCount(-i_dmgAmount, 0);
        return 0;
    }

    f32 magnified_dmgF = (f32)i_dmgAmount * damageMagnification(i_checkZoraMag, param_3);
    i_dmgAmount = magnified_dmgF;
    if ((int)(magnified_dmgF * 10.0f) % 10 != 0) {
        i_dmgAmount++;
    }

    if (checkWolf()) {
        offWolfEyeUp();
    }

    if (checkMagicArmorNoDamage()) {
        dComIfGp_setItemRupeeCount(-i_dmgAmount * 10);
    } else {
        dComIfGp_setItemLifeCount(-i_dmgAmount, 0);
    }

    onResetFlg1(RFLG1_DAMAGE_IMPACT);
    mSwordUpTimer = 0;

    if (i_setDmgTimer) {
        mDamageTimer = mpHIO->mDamage.m.mInvincibleTime;
        setDamageColorTime();
    }

    mIceDamageWaitTimer = 0;
    return 0;
}

void daAlink_c::setDamagePointNormal(int i_dmgAmount) {
    setDamagePoint(i_dmgAmount, FALSE, TRUE, 0);
}

void daAlink_c::setLandDamagePoint(int i_dmgAmount) {
    onEndResetFlg2(ERFLG2_UNK_40);
    setDamagePoint(i_dmgAmount, FALSE, TRUE, 0);
    offEndResetFlg2(ERFLG2_UNK_40);
}

cXyz* daAlink_c::getDamageVec(dCcD_GObjInf* i_hitObj) {
    cXyz* vec = i_hitObj->GetTgRVecP();
    cXyz sp40;

    f32 var_f31 = vec->abs2XZ();

    if (checkResetFlg1(RFLG1_UNK_2)) {
        sp40.set(cM_ssin(shape_angle.y) * -10.0f, 0.0f, cM_scos(shape_angle.y) * -10.0f);
        i_hitObj->SetTgRVec(sp40);
    } else if (var_f31 < 0.1f) {
        if (i_hitObj->GetTgHitAc() != NULL) {
            sp40 = current.pos - i_hitObj->GetTgHitAc()->current.pos;
        } else {
            sp40 = current.pos - *i_hitObj->GetTgHitPosP();
        }

        var_f31 = sp40.abs2();
        if (var_f31 < 0.1f) {
            sp40.x = cM_ssin(shape_angle.y) * -10.0f;
            sp40.y = 0.0f;
            sp40.z = cM_scos(shape_angle.y) * -10.0f;
        } else {
            sp40 *= 10.0f / JMAFastSqrt(var_f31);
        }

        i_hitObj->SetTgRVec(sp40);
    }

    multVecMagneBootInvMtx(vec);
    return vec;
}

void daAlink_c::setDashDamage() {
    setUpperAnimeParam(0xAD, UPPER_2, &mpHIO->mDamage.m.mDashDmgAnm);
    setFacePriBck(0x121);
    setFacePriTexture(FTANM_UNK_27);
    voiceStart(Z2SE_AL_V_DAMAGE_S);
    seStartOnlyReverb(Z2SE_AL_DAMAGE_NORMAL);
    freeGrabItem();
}

BOOL daAlink_c::checkIcePolygonDamage(cBgS_PolyInfo* i_poly) {
    if (dComIfG_Bgsp().GetPolyAtt0(*i_poly) == 8 && dKy_pol_argument_get(i_poly) & 0x60) {
        return TRUE;
    }

    return FALSE;
}

BOOL daAlink_c::checkMagicArmorNoDamage() {
    return checkMagicArmorWearAbility() && !checkMagicArmorHeavy();
}

int daAlink_c::checkPolyDamage() {
    if (dComIfGs_PolyDamageOff_Check()) {
        return 0;
    }

    if (checkEndResetFlg1(ERFLG1_UNK_4000)) {
        return 4;
    }

    int var_r29;
    if (!checkBoardRide() && mLinkAcch.ChkGroundHit()) {
        if (dComIfG_Bgsp().ChkPolySafe(mLinkAcch.m_gnd) && (dKy_pol_argument_get(&mLinkAcch.m_gnd) & 0x60)) {
            var_r29 = 1;
            if (checkIcePolygonDamage(&mLinkAcch.m_gnd)) {
                var_r29 |= 0x80;
            }

            cM3dGPla sp38;
            dComIfG_Bgsp().GetTriPla(mLinkAcch.m_gnd, &sp38);
            field_0x311e = sp38.mNormal.atan2sX_Z();
            return var_r29;
        }
    }

    if (!checkBoardRide() && ((checkWaterPolygonUnder() && dComIfG_Bgsp().ChkPolySafe(mLinkAcch.m_wtr) && ((dKy_pol_argument_get(&mLinkAcch.m_wtr) & 0x60) || field_0x2fbc == 6)) || (checkModeFlg(0x40) && field_0x33bc < field_0x33b8 && -G_CM3D_F_INF != field_0x33b8 && ((field_0x2fc6 & 0x60) || field_0x2fc5 == 6) && (field_0x2fc5 != 6 || !(field_0x33b8 > field_0x33d8 + mpHIO->mBasic.m.mLavaDeathDepth))))) {
        return 2;
    }

    if (mLinkAcch.ChkWallHit()) {
        dBgS_AcchCir* acch_cir = mAcchCir;
        
        for (int i = 0; i < 3; i++) {
            if (acch_cir->ChkWallHit()) {
                if (dComIfG_Bgsp().ChkPolySafe(*acch_cir) && (dKy_pol_argument_get(acch_cir) & 0x60)) {
                    var_r29 = 3;

                    field_0x311e = acch_cir->GetWallAngleY();
                    if (checkIcePolygonDamage(acch_cir)) {
                        var_r29 |= 0x80;
                    }

                    if (dKy_pol_argument_get(acch_cir) & 0x20) {
                        var_r29 |= 0x40;
                    }

                    return var_r29;
                }
            }
            acch_cir++;
        }
    }

    return field_0x2fca;
}

BOOL daAlink_c::checkElecReturnDamage(dCcD_GObjInf& param_0, fopAc_ac_c** param_1) {
    if (param_0.ChkAtHit() && param_0.GetAtHitGObj() != NULL) {
        dCcD_GObjInf* gobj_p = (dCcD_GObjInf*)param_0.GetAtHitGObj();

        if (gobj_p->GetTgSpl() == dCcG_Tg_Spl_UNK_1) {
            *param_1 = param_0.GetAtHitAc();
            return true;
        }
    }

    return false;
}

void daAlink_c::damageTimerCount() {
    if (!checkModeFlg(8)) {
        if (mDamageColorTime != 0) {
            mDamageColorTime--;
        }
        mDamageTimer--;

        if (mProcID != PROC_DK_CAUGHT && checkNoResetFlg1(FLG1_DK_CAUGHT)) {
            offNoResetFlg1(FLG1_DK_CAUGHT);
        }

        if (!checkDkCaught2Anime() && checkNoResetFlg0(FLG0_DK_CAUGHT)) {
            offNoResetFlg0(FLG0_DK_CAUGHT);
        }
    } else if (mDamageColorTime != 0) {
        mDamageColorTime--;

        if ((mDamageColorTime + mDamageTimer) % 16 == 0) {
            setDamageColorTime();
        }
    }
}

bool daAlink_c::checkHugeAttack(int param_0) const {
    return param_0 == 2 || param_0 == 7 || param_0 == 11 || param_0 == 14;
}

bool daAlink_c::checkLargeAttack(int param_0) const {
    return param_0 == 1 || param_0 == 6 || param_0 == 10 || param_0 == 13;
}

BOOL daAlink_c::checkDamageAction() {
    s16 spE = mIceDamageWaitTimer;
    mIceDamageWaitTimer = 0;

    u8 spA = field_0x2fca;
    field_0x2fca = 0;

    if (checkAnmEnd(&mUpperFrameCtrl[2])) {
        if (checkDashDamageAnime()) {
            resetUpperAnime(UPPER_2, mpHIO->mDamage.m.mDashDmgAnm.mInterpolation);
        } else if (checkWolfHeadDamageAnime()) {
            resetUpperAnime(UPPER_2, 3.0f);
        } else if (checkSmallUpperGuardAnime()) {
            resetUpperAnime(UPPER_2, 3.0f);
            setUpperGuardAnime(-1.0f);
        } else if (checkZoraSwimDamageAnime()) {
            resetUpperAnime(UPPER_2, 3.0f);
        }
    }

    if (checkEndResetFlg0(ERFLG0_UNK_40)) {
        if (getSumouMode()) {
            cancelSumouMode();
        }
    
        if (checkEndResetFlg1(ERFLG1_UNK_80)) {
            return procOctaIealSpitInit();
        }
    
        if (checkEndResetFlg0(ERFLG0_UNK_40000000)) {
            return commonFallInit(4);
        }
    
        if (mProcID == PROC_WOLF_CARGO_CARRY && mProcVar2.field_0x300c == 0) {
            offNoResetFlg0(FLG0_UNK_100000);
        }
    
        return procCoLargeDamageInit(-3, 1, 0, 0, NULL, 0);
    }

    if (checkEventRun()) {
        offNoResetFlg1(FLG1_DK_CAUGHT);
        offNoResetFlg0(FLG0_DK_CAUGHT);
        return 0;
    }

    if (checkNoResetFlg1(FLG1_DK_CAUGHT)) {
        if (!checkModeFlg(0x70C52) && !checkWolf() && procDkCaughtInit(field_0x28f8)) {
            return 1;
        }

        if (mProcID != PROC_DK_CAUGHT) {
            offNoResetFlg1(FLG1_DK_CAUGHT);
        }
    }

    if (checkNoResetFlg0(FLG0_DK_CAUGHT)) {
        if ((!checkModeFlg(0x70C52)) && (!checkWolf()) && (!checkDkCaught2Anime())) {
            setUpperAnimeBase(getMainBckData(ANM_HORSE_WAIT_D_B)->m_underID);
            setFacePriAnime(ANM_HORSE_WAIT_D_B);
            field_0x30f6 = mpHIO->mDamage.mDamCaught.m.mGrabDuration;
            field_0x2f96 = 1;
            field_0x2f97 = 6;
            onNoResetFlg1(FLG1_UNK_100000);
            return procWaitInit();
        }

        if (!checkDkCaught2Anime()) {
            offNoResetFlg0(FLG0_DK_CAUGHT);
        }
    }

    if (checkEndResetFlg0(ERFLG0_UNK_80)) {
        if (checkEndResetFlg1(ERFLG1_UNK_80)) {
            return procOctaIealSpitInit();
        }

        if (checkEndResetFlg0(ERFLG0_UNK_40000000)) {
            return commonFallInit(4);
        }

        return procCoLargeDamageInit(-3, 1, 0, 0, NULL, 0);
    }

    if (checkEndResetFlg1(ERFLG1_UNK_400)) {
        return procCoSandWallHitInit();
    }

    if (checkNoResetFlg2(FLG2_PRESSED_DAMAGE)) {
        if (commonLargeDamageUpInit(-4, 1, 0, 0)) {
            current.pos = field_0x3744;
            shape_angle.y = field_0x3104;
            current.angle.y = shape_angle.y;
            return 1;
        }
        return 0;
    }

    if (mDamageTimer != 0) {
        return checkWolfBarrierHitReverse();
    }

    dCcD_GObjInf* var_r29 = NULL;
    for (int i = 0; i < 3; i++) {
        if (field_0x850[i].ChkTgHit() && (field_0x2fb8 == 0 || (1 << i) == 0)) {
            var_r29 = &field_0x850[i];
            break;
        }
    }

    if (var_r29 == NULL && checkWolf()) {
        var_r29 = &field_0xFB8;
    }

    if (checkEndResetFlg1(ERFLG1_UNK_8000) && checkWolf()) {
        return procWolfCargoCarryInit();
    }

    BOOL armor_no_dmg = checkMagicArmorNoDamage();
    fopAc_ac_c* sp30 = NULL;
    field_0x2fca = spA;
    int sp2C = checkPolyDamage();
    field_0x2fca = 0;
    mIceDamageWaitTimer = spE;

    if (sp2C & 0x80) {
        mIceDamageWaitTimer++;
        if (mIceDamageWaitTimer < mpHIO->mDamage.m.mFreezeTime) {
            sp2C = 0;
        }
    } else if (mIceDamageWaitTimer > 3) {
        mIceDamageWaitTimer -= 3;
    } else {
        mIceDamageWaitTimer = 0;
    }

    if (sp2C != 0) {
        int sp28 = (sp2C & 7);
        setDamagePoint(1, sp2C & 0x80, TRUE, 0);

        if (checkWolf()) {
            mDamageTimer = mpHIO->mDamage.m.mWolfFloorInvincibleTime;
            setDamageColorTime();
        }

        if (checkModeFlg(0x4000000)) {
            dComIfGp_getVibration().StartShock(2, 0x1F, cXyz(0.0f, 1.0f, 0.0f));

            if (checkWolf()) {
                setWolfHeadDamage();
            } else {
                voiceStart(Z2SE_AL_V_DAMAGE_S);
                seStartOnlyReverb(Z2SE_AL_DAMAGE_NORMAL);
            }
        } else {
            if (!checkModeFlg(0x40000) && ((sp2C & 0x40) || (!mLinkAcch.ChkGroundHit() && (sp28 == 3 || sp28 == 4)))) {
                dComIfGp_getVibration().StartShock(2, 0x1F, cXyz(0.0f, 1.0f, 0.0f));

                int sp24;
                if (sp2C & 0x80) {
                    sp24 = 1;
                } else {
                    sp24 = 0;
                }

                if (sp28 == 4) {
                    field_0x311e = shape_angle.y;
                }

                return procCoLargeDamageInit(-5, 1, 0, 0, NULL, sp24);
            }

            dComIfGp_getVibration().StartShock(2, 0x1F, cXyz(0.0f, 1.0f, 0.0f));

            if (checkWolf()) {
                if (checkModeFlg(0x40000)) {
                    setWolfHeadDamage();
                } else if (sp2C & 0x80) {
                    return procWolfDamageInit(NULL);
                } else {
                    return procCoPolyDamageInit();
                }
            } else if (checkModeFlg(0x40000)) {
                return procSwimDamageInit(NULL);
            } else if (sp2C & 0x80) {
                return procDamageInit(NULL, 0);
            } else {
                return procCoPolyDamageInit();
            }
        }
    } else if ((mEquipItem == 0x103 || (checkHookshotItem(mEquipItem) && !mAtCps[0].ChkAtSPrm(0x20))) && (checkElecReturnDamage(mAtCps[0], &sp30) || checkElecReturnDamage(mAtCps[1], &sp30) || checkElecReturnDamage(mAtCps[2], &sp30))) {
        setDamagePointNormal(2);
        onResetFlg1(RFLG1_UNK_2);
        dComIfGp_getVibration().StartShock(4, 0x1F, cXyz(0.0f, 1.0f, 0.0f));

        if (checkModeFlg(0x4000000)) {
            if (checkWolf()) {
                setWolfHeadDamage();
            } else {
                voiceStart(Z2SE_AL_V_DAMAGE_S);
                seStartOnlyReverb(Z2SE_AL_DAMAGE_NORMAL);
            }
        } else {
            return procCoElecDamageInit(sp30, NULL, 0);
        }
    } else if (var_r29 != NULL && var_r29->ChkTgHit() && mProcID != PROC_GUARD_SLIP) {
        fopAc_ac_c* tghit_ac = var_r29->GetTgHitAc();
        s16 tghit_ac_name;
        if (tghit_ac != NULL) {
            tghit_ac_name = fopAcM_GetName(tghit_ac);
        } else {
            tghit_ac_name = PROC_ALINK;
        }

        dCcD_GObjInf* tghit_gobj = var_r29->GetTgHitGObj();
        int at_spl;
        u8 at_mtrl;
        if (tghit_gobj != NULL) {
            at_spl = tghit_gobj->GetAtSpl();
            at_mtrl = tghit_gobj->GetAtMtrl();
        } else {
            at_spl = field_0x814.GetAtSpl();
            at_mtrl = dCcD_MTRL_NONE;
        }
    
        if (at_spl == 8 && checkWolf()) {
            at_spl = 1;
        }
    
        if (var_r29->ChkTgShieldHit()) {
            setGuardSe(var_r29);

            if (checkHugeAttack(at_spl)) {
                dComIfGp_getVibration().StartShock(5, 1, cXyz(0.0f, 1.0f, 0.0f));
            } else if (checkLargeAttack(at_spl)) {
                dComIfGp_getVibration().StartShock(4, 1, cXyz(0.0f, 1.0f, 0.0f));
            } else if (at_spl == 8) {
                dComIfGp_getVibration().StartShock(2, 1, cXyz(0.0f, 1.0f, 0.0f));
            } else {
                dComIfGp_getVibration().StartShock(3, 1, cXyz(0.0f, 1.0f, 0.0f));
            }
    
            if (!armor_no_dmg && at_mtrl == dCcD_MTRL_FIRE && checkWoodShieldEquip() && field_0x2fcb == 0 && !field_0x2e44.checkPassNum(15)) {
                field_0x2fcb = 120;
                seStartOnlyReverb(Z2SE_AL_BURN_START);
            }

            if ((mProcID != PROC_GUARD_ATTACK || (tghit_ac_name != PROC_E_TK_BALL && tghit_ac_name != PROC_KN_BULLET)) && !checkModeFlg(0x4000000)) {
                if (at_spl == 10 || at_spl == 11 || at_spl == 9) {
                    return procGuardBreakInit();
                }

                if (!armor_no_dmg) {
                    if (var_r29->ChkTgShield()) {
                        if (checkHorseRide()) {
                            if ((checkLargeAttack(at_spl) || checkHugeAttack(at_spl)) && (!checkSpecialHorseRide())) {
                                return procHorseHangInit(var_r29, 0);
                            }
                        } else if (at_spl == 8 || checkModeFlg(0x70C52)) {
                            if (!checkModeFlg(0x400) && mEquipItem != fpcNm_ITEM_IRONBALL) {
                                setSmallGuard(var_r29);
                            }
                        } else {
                            return procGuardSlipInit(at_spl, var_r29);
                        }
                    } else if (var_r29->ChkTgSpShield()) {
                        return procGuardSlipInit(at_spl, var_r29);
                    }
                }
            }
        } else {
            int dmg = field_0x814.GetDmg();
            if (var_r29->ChkTgHit() && tghit_gobj != NULL && tghit_gobj->GetAtType() == AT_TYPE_BOMB) {
                if (checkNoResetFlg3(FLG3_UNK_40000000)) {
                    dmg = 1;
                } else {
                    dmg = 2;
                }
            }

            if (at_mtrl == dCcD_MTRL_ELECTRIC) {
                dComIfGp_getVibration().StartShock(4, 0x1F, cXyz(0.0f, 1.0f, 0.0f));
            } else if (checkHugeAttack(at_spl)) {
                dComIfGp_getVibration().StartShock(8, 0x1F, cXyz(0.0f, 1.0f, 0.0f));
            } else if (checkLargeAttack(at_spl)) {
                dComIfGp_getVibration().StartShock(4, 0x1F, cXyz(0.0f, 1.0f, 0.0f));
            } else {
                dComIfGp_getVibration().StartShock(2, 0x1F, cXyz(0.0f, 1.0f, 0.0f));
            }

            setDamagePoint(dmg, at_mtrl == dCcD_MTRL_FIRE || at_mtrl == dCcD_MTRL_ICE, TRUE, 0);

            if (armor_no_dmg && at_mtrl != dCcD_MTRL_ELECTRIC && at_mtrl != dCcD_MTRL_ICE) {
                setGuardSe(var_r29);
            } else if (at_mtrl == dCcD_MTRL_FIRE) {
                seStartOnlyReverb(Z2SE_AL_BURN_START);
                if (at_spl == 8) {
                    initFirePointDamageEffect(var_r29->GetTgHitPosP(), var_r29);
                } else {
                    initFirePointDamageEffectAll();
                }
            }

            if (checkModeFlg(0x4000000)) {
                if (checkWolf()) {
                    setWolfHeadDamage();
                } else if (!armor_no_dmg) {
                    voiceStart(Z2SE_AL_V_DAMAGE_S);
                    seStartOnlyReverb(Z2SE_AL_DAMAGE_NORMAL);
                }
            } else if (at_mtrl == dCcD_MTRL_ELECTRIC && !checkSpecialHorseRide()) {
                return procCoElecDamageInit(var_r29->GetTgHitAc(), var_r29, at_spl);
            } else if (checkReinRide()) {
                if (at_mtrl == dCcD_MTRL_ELECTRIC) {
                    return procCoLargeDamageInit(-1, 1, 0, 0, var_r29, 0);
                }

                if (!checkBoarSingleBattle() && (mProcID == PROC_HORSE_HANG || mProcID == PROC_BOAR_RUN || mProcID == PROC_HORSE_RUN || checkLargeAttack(at_spl) || checkHugeAttack(at_spl))) {
                    setHorseZeldaDamage();
                    procCoLargeDamageInit(-1, 1, 0, 0, var_r29, 0);
                    onModeFlg(0x2000);
                    return 1;
                }

                if (!armor_no_dmg) {
                    if (checkHorseNotDamageReaction()) {
                        voiceStart(Z2SE_AL_V_DAMAGE_S);
                        seStartOnlyReverb(Z2SE_AL_DAMAGE_NORMAL);
                    } else if (at_spl == 8 || checkHorseZelda()) {
                        return procHorseDamageInit(var_r29);
                    } else {
                        return procHorseHangInit(var_r29, 1);
                    }
                }
            } else if (checkModeFlg(0x40000)) {
                if (checkWolf()) {
                    setWolfHeadDamage();
                } else if (!armor_no_dmg) {
                    return procSwimDamageInit(var_r29);
                }
            } else if (dmg == 0 && !checkWolf() && (at_spl == 10 || at_spl == 11 || at_spl == 9) && mLinkAcch.ChkGroundHit() && !checkModeFlg(0x70C52)) {
                return procGuardBreakInit();
            } else if (checkHugeAttack(at_spl)) {
                return procCoLargeDamageInit(-1, 0, 0, 0, var_r29, 0);
            } else if (checkModeFlg(0x70C52) || checkLargeAttack(at_spl)) {
                if (checkModeFlg(0x10000) && !checkLargeAttack(at_spl)) {
                    voiceStart(Z2SE_AL_V_DAMAGE_S);
                    seStartOnlyReverb(Z2SE_AL_DAMAGE_NORMAL);
                    procFallInit(1, mpHIO->mAutoJump.m.mFallInterpolation);
                    onModeFlg(8);
                    return 1;
                }

                if (at_spl == 8 && (mProcID == PROC_WOLF_ROPE_MOVE || mProcID == PROC_WOLF_ROPE_SUBJECTIVITY || mProcID == PROC_WOLF_ROPE_STAGGER)) {
                    cXyz* sp14 = getDamageVec(var_r29);
                    if ((s16)(sp14->atan2sX_Z() - shape_angle.y) >= 0) {
                        return procWolfRopeHangInit(2);
                    }
                    return procWolfRopeHangInit(3);
                }

                return procCoLargeDamageInit(-1, 1, 0, 0, var_r29, 0);
            } else if (!armor_no_dmg || at_mtrl == dCcD_MTRL_ICE) {
                if (checkDashAnime() && at_mtrl != dCcD_MTRL_ICE) {
                    setDashDamage();
                } else if (mProcID == PROC_WOLF_MOVE && at_mtrl != dCcD_MTRL_ICE && mNormalSpeed > field_0x594 * 0.7f) {
                    setWolfHeadDamage();
                } else if (checkWolf()) {
                    return procWolfDamageInit(var_r29);
                } else {
                    return procDamageInit(var_r29, 0);
                }
            }
        }
    }

    if (checkHorseRide()) {
        daHorse_c* horsep = dComIfGp_getHorseActor();
        if (horsep->checkCowHit() != 0) {
            current.angle.y = horsep->getCowHitAngle();

            if (checkBoarSingleBattle()) {
                dComIfGp_getVibration().StartShock(8, 0x1F, cXyz(0.0f, 1.0f, 0.0f));
                return procCoLargeDamageInit(-1, 0, 0, 0, NULL, 0);
            }
    
            dComIfGp_getVibration().StartShock(4, 0x1F, cXyz(0.0f, 1.0f, 0.0f));
            return procCoLargeDamageInit(-1, 1, 0, 0, NULL, 0);
        }
    }

    field_0x814.ClrTg();
    field_0x814.ClrAt();
    return checkWolfBarrierHitReverse();
}

int daAlink_c::procDamageInit(dCcD_GObjInf* param_0, int param_1) {
    BOOL freeze_player;

    if (!param_1 && (param_0 == NULL || (param_0->GetTgHitGObj() != NULL && param_0->GetTgHitGObj()->GetAtMtrl() == dCcD_MTRL_ICE))) {
        freeze_player = true;
        voiceStart(Z2SE_AL_V_DAMAGE_FREEZE);
        seStartOnlyReverb(Z2SE_AL_FREEZE);
    } else {
        seStartOnlyReverb(Z2SE_AL_DAMAGE_NORMAL);
        freeze_player = false;
        
        if (!param_1) {
            voiceStart(Z2SE_AL_V_DAMAGE_S);
        }
    }

    if (checkIronBallWaitAnime() && !freeze_player && !param_1) {
        return 1;
    }

    commonProcInit(PROC_DAMAGE);

    cXyz sp7C;
    f32 var_f31;
    f32 var_f30 = cM_ssin(shape_angle.y);
    f32 var_f29 = cM_scos(shape_angle.y);

    cXyz* var_r29;
    if (param_0 != NULL) {
        var_r29 = getDamageVec(param_0);   
    } else if (param_1) {
        s16 var_r24 = field_0x3102 + 0x8000;
        sp7C.set(cM_ssin(var_r24), 0.0f, cM_scos(var_r24));
        var_r29 = &sp7C;
        offModeFlg(8);
    } else {
        sp7C.set(cM_ssin(field_0x311e), 0.0f, cM_scos(field_0x311e));
        var_r29 = &sp7C;
    }

    cXyz sp88(var_r29->z * -var_f30 + var_r29->x * var_f29,
                var_r29->y,
                var_r29->z * var_f29 + var_r29->x * var_f30);
    
    mProcVar2.field_0x300c = cLib_minMaxLimit<s16>(cM_atan2s(sp88.z, sp88.y), -mpHIO->mDamage.mDamNormal.m.mFrontBackBodyMaxAngle, mpHIO->mDamage.mDamNormal.m.mFrontBackBodyMaxAngle);
    mProcVar3.field_0x300e = cLib_minMaxLimit<s16>(cM_atan2s(sp88.x, -JMAFastSqrt(sp88.y * sp88.y + sp88.z * sp88.z)), -mpHIO->mDamage.mDamNormal.m.mLeftRightBodyMaxAngle, mpHIO->mDamage.mDamNormal.m.mLeftRightBodyMaxAngle);

    if (freeze_player) {
        var_f31 = 0.0f;
    } else if (mTargetedActor != NULL) {
        var_f31 = mpHIO->mAtnMove.m.mWaitAnmSpeed;
    } else {
        var_f31 = mpHIO->mNoActAtnMove.m.mWaitAnmSpeed;
    }

    if (checkAtnLeftAnime()) {
        setUnderAnime(0x1A, UNDER_2, var_f31, 0.0f, -1, -1.0f);
    } else if (checkAtnRightAnime()) {
        setUnderAnime(0x1C, UNDER_2, var_f31, 0.0f, -1, -1.0f);
    }

    int direction = getDirectionFromAngle(cM_atan2s(-sp88.x, -sp88.z));
    if (direction == DIR_FORWARD) {
        setSingleAnimeParam(ANM_DMG_SMALL_A, &mpHIO->mDamage.mDamNormal.m.mFrontAnm);
        field_0x3478 = mpHIO->mDamage.mDamNormal.m.mFrontAnm.mCancelFrame;
    } else if (direction == DIR_BACKWARD) {
        setSingleAnimeParam(ANM_DMG_SMALL_B, &mpHIO->mDamage.mDamNormal.m.mRearAnm);
        field_0x3478 = mpHIO->mDamage.mDamNormal.m.mRearAnm.mCancelFrame;
    } else if (direction == DIR_LEFT) {
        setSingleAnimeParam(ANM_DMG_SMALL_LEFT, &mpHIO->mDamage.mDamNormal.m.mLeftAnm);
        field_0x3478 = mpHIO->mDamage.mDamNormal.m.mLeftAnm.mCancelFrame;
    } else {
        setSingleAnimeParam(ANM_DMG_SMALL_RIGHT, &mpHIO->mDamage.mDamNormal.m.mRightAnm);
        field_0x3478 = mpHIO->mDamage.mDamNormal.m.mRightAnm.mCancelFrame;
    }

    current.angle.y = var_r29->atan2sX_Z();
    mProcVar4.field_0x3010 = current.angle.y;

    mNormalSpeed = var_r29->absXZ() * mpHIO->mDamage.mDamNormal.m.mAttackSpeedRate + mpHIO->mDamage.mDamNormal.m.mInitialSpeed;
    if (mNormalSpeed > mpHIO->mMove.m.mMaxSpeed) {
        mNormalSpeed = mpHIO->mMove.m.mMaxSpeed;
    }

    daPy_frameCtrl_c* frame_ctrl = mUnderFrameCtrl;
    field_0x347c = M_PI / (frame_ctrl->getEnd() - frame_ctrl->getStart());

    if (freeze_player) {
        frame_ctrl->setRate(mpHIO->mDamage.mDamNormal.m.mIceDamageASpeed);
        frame_ctrl->setEnd(7);
        mNormalSpeed = 0.0f;
        mProcVar0.mIceFreezeTimer = 90;
        onNoResetFlg1(FLG1_ICE_FREEZE);
    } else {
        mProcVar0.mIceFreezeTimer = 0;
    }

    mProcVar1.field_0x300a = 0;
    return 1;
}

int daAlink_c::procDamage() {
    daPy_frameCtrl_c* frame_ctrl = mUnderFrameCtrl;
    f32 var_f30 = frame_ctrl->getFrame() - frame_ctrl->getStart();
    f32 var_f31 = cM_fsin(field_0x347c * var_f30);

    mBodyAngle.x = mProcVar2.field_0x300c * var_f31;
    mBodyAngle.z = -mProcVar3.field_0x300e * var_f31;
    mBodyAngle.y = 0;

    cLib_chaseF(&mNormalSpeed, 0.0f, mpHIO->mDamage.mDamNormal.m.mDeceleration);

    if (checkFreezeDamage()) {
        freezeTimerDamage();
    }

    if (checkAnmEnd(frame_ctrl)) {
        current.angle.y = shape_angle.y;

        if (!checkFreezeDamage()) {
            checkNextAction(0);
        } else if (mProcVar0.mIceFreezeTimer == 0) {
            seStartOnlyReverb(Z2SE_AL_FREEZE_RECOVER);
            procStEscapeInit();
            onModeFlg(8);
        }
    } else if (frame_ctrl->getFrame() > field_0x3478) {
        current.angle.y = shape_angle.y;
        onModeFlg(4);

        if (!checkNextAction(1)) {
            current.angle.y = mProcVar4.field_0x3010;
        }
    }

    return 1;
}

int daAlink_c::procCoLargeDamageInit(int param_0, int param_1, s16 param_2, s16 param_3,
                                         dCcD_GObjInf* param_4, int param_5) {
    u32 sp10 = checkReinRide();

    if (!commonProcInitNotSameProc(PROC_LARGE_DAMAGE)) {
        return 0;
    }

    if (mCopyRodAcKeep.getActor() != NULL) {
        ((daCrod_c*)mCopyRodAcKeep.getActor())->offControll();
    }

    field_0x3480 = -1.0f;
    mFallVoiceInit = 0;

    int var_r30;
    if (param_0 == -3) {
        current.angle.y = field_0x2ffe;
        var_r30 = getDirectionFromAngle((current.angle.y - shape_angle.y));
    
        dComIfGp_getVibration().StartShock(6, 0x1F, cXyz(0.0f, 1.0f, 0.0f));
        setDamagePointNormal(field_0x318c);
        onNoResetFlg1(FLG1_THROW_DAMAGE);
    } else if (param_0 == -4) {
        var_r30 = 1;
        current.angle.y = shape_angle.y + 0x8000;
    } else if (param_0 == -2) {
        var_r30 = mDemo.getParam0();

        if (var_r30 == 0) {
            current.angle.y = shape_angle.y;
        } else if (var_r30 == 2) {
            current.angle.y = shape_angle.y + 0x4000;
        } else if (var_r30 == 3) {
            current.angle.y = shape_angle.y - 0x4000;
        } else {
            current.angle.y = shape_angle.y + 0x8000;
        }
    } else if (param_0 == -5) {
        if (param_5 == 1) {
            onNoResetFlg1(FLG1_ICE_FREEZE);
        }

        current.angle.y = field_0x311e;
        var_r30 = getDirectionFromAngle((current.angle.y - shape_angle.y));
    } else if (param_0 == -1) {
        if (param_4 != NULL) {
            cXyz* temp_r3 = getDamageVec(param_4);
            current.angle.y = temp_r3->atan2sX_Z();

            if (param_4->GetTgHitGObj() != NULL) {
                if (param_4->GetTgHitGObj()->GetAtMtrl() == dCcD_MTRL_ICE) {
                    onNoResetFlg1(FLG1_ICE_FREEZE);
                }
            }
        }

        var_r30 = getDirectionFromAngle((current.angle.y - shape_angle.y));
    } else if (param_0 == -6) {
        current.angle.y = field_0x3102 + 0x8000;
        var_r30 = getDirectionFromAngle((current.angle.y - shape_angle.y));
        offModeFlg(8);
        field_0x3480 = 1.0f;
    } else {
        if (checkWolf()) {
            if (param_0 == 0x2D) {
                var_r30 = 1;
            } else if (param_0 == 0x2E) {
                var_r30 = 2;
            } else if (param_0 == 0x2F) {
                var_r30 = 3;
            } else {
                var_r30 = 0;
            }
        } else if (param_0 == 0x88) {
            var_r30 = 1;
        } else if (param_0 == 0x89) {
            var_r30 = 2;
        } else if (param_0 == 0x8A) {
            var_r30 = 3;
        } else {
            var_r30 = 0;
        }
        setOldRootQuaternion(param_2, 0, param_3);
        mFallVoiceInit = 1;
    }

    if (var_r30 == 0) {
        if (checkWolf()) {
            field_0x3198 = 0x29;
            mProcVar2.field_0x300c = 0;
        } else {
            field_0x3198 = 0x84;
            mProcVar2.field_0x300c = 0x3FFF;
        }

        mProcVar3.field_0x300e = 1;
        shape_angle.y = current.angle.y;
    } else if (var_r30 == 3) {
        if (checkWolf()) {
            field_0x3198 = 0x2A;
            mProcVar2.field_0x300c = 0x2000;
        } else {
            field_0x3198 = 0x85;
            mProcVar2.field_0x300c = 0x3FFF;
        }

        mProcVar3.field_0x300e = 0;
        shape_angle.y = current.angle.y + 0x4000;
    } else if (var_r30 == 2) {
        if (checkWolf()) {
            field_0x3198 = 0x2B;
            mProcVar2.field_0x300c = -0x2000;
        } else {
            field_0x3198 = 0x86;
            mProcVar2.field_0x300c = -0x3FFF;
        }

        mProcVar3.field_0x300e = 0;
        shape_angle.y = current.angle.y - 0x4000;
    } else {
        if (checkWolf()) {
            field_0x3198 = 0x28;
            mProcVar2.field_0x300c = -0x3FFF;
        } else {
            field_0x3198 = 0x83;
            mProcVar2.field_0x300c = -0x3FFF;
        }

        mProcVar3.field_0x300e = 1;
        shape_angle.y = current.angle.y + 0x8000;
    }

    const daAlinkHIO_wlDamLaHu_c1* var_r29;
    const daAlinkHIO_damLaHu_c1* var_r28;
    if (checkWolf()) {
        if (param_1 != 0) {
            var_r29 = &mpHIO->mWolf.mWlDamage.mLarge.m;
        } else {
            var_r29 = &mpHIO->mWolf.mWlDamage.mHuge.m;
        }

        setSpecialGravity(var_r29->mGravity, maxFallSpeed, 0);
        mProcVar4.field_0x3010 = var_r29->mBodyRotationSpeed;
        field_0x3478 = var_r29->mBounceSpeed;
        field_0x347c = var_r29->mDeceleration;
        setSingleAnimeWolfBaseMorf((daAlink_c::daAlink_WANM)field_0x3198, var_r29->mDamageInterp);
        setFaceBasicTexture(FTANM_UNK_91);
    } else {
        if (param_1 != 0) {
            var_r28 = &daAlinkHIO_damLarge_c0::m;
        } else {
            var_r28 = &daAlinkHIO_damHuge_c0::m;
        }

        setSpecialGravity(var_r28->mGravity, maxFallSpeed, 0);
        mProcVar4.field_0x3010 = var_r28->mBodyRotateRate;
        field_0x3478 = var_r28->mBounceSpeed;
        field_0x347c = var_r28->mDeceleration;
        setSingleAnimeBaseMorf((daAlink_c::daAlink_ANM)field_0x3198, var_r28->mDamageBlend);
    }

    if (param_0 < 0) {
        if (checkBoarSingleBattle() && dComIfGp_getHorseActor() != NULL && dComIfGp_getHorseActor()->checkCowHit()) {
            mNormalSpeed = 80.0f;
            speed.y = 80.0f;
            onModeFlg(0x2000);
        } else if (param_1 == 0 && checkMiddleBossGoronRoom()) {
            mNormalSpeed = 60.0f;
            speed.y = 30.0f;
        } else if (param_0 == -3) {
            mNormalSpeed = field_0x3408;
            speed.y = field_0x340c;
        } else {
            if (checkWolf()) {
                mNormalSpeed = var_r29->mHorizontalSpeed;
                speed.y = var_r29->mVerticalSpeed;
            } else {
                mNormalSpeed = var_r28->mHorizontalSpeed;
                speed.y = var_r28->mVerticalSpeed;
            }

            if (param_0 == -6) {
                field_0x3478 = 1000.0f;
            }
        }

        if (param_5 != 2) {
            if (param_0 != -6) {
                if (checkWolf()) {
                    voiceStart(Z2SE_WL_V_DAMAGE);
                } else if (checkFreezeDamage()) {
                    voiceStart(Z2SE_AL_V_DAMAGE_FREEZE);
                } else {
                    voiceStart(Z2SE_AL_V_DAMAGE_L);
                }
            }

            if (checkFreezeDamage()) {
                seStartOnlyReverb(Z2SE_AL_FREEZE);
            } else {
                seStartOnlyReverb(Z2SE_AL_DAMAGE_LARGE);
            }
        }
    } else {
        if (checkWolf()) {
            mNormalSpeed = var_r29->mBounceSpeed;
        } else {
            mNormalSpeed = var_r28->mBounceSpeed;
        }

        current.pos.x += 35.0f * cM_ssin(current.angle.y);
        current.pos.z += 35.0f * cM_scos(current.angle.y);
        speed.y = 0.0f;
    }

    mBodyAngle.set(0, 0, 0);

    mProcVar0.field_0x3008 = 0;
    mProcVar1.field_0x300a = param_1;
    mProcVar5.field_0x3012 = 0x14;

    if (checkBoarSingleBattle()) {
        mLinkAcch.SetWallNone();
        mLinkAcch.OffLineCheck();
        onModeFlg(0x4000);
    }

    if (sp10) {
        onModeFlg(0x2000);
        field_0x32cc = 5;
    }

    return 1;
}

int daAlink_c::procCoLargeDamage() {
    if (field_0x32cc != 0) {
        field_0x32cc--;
    } else {
        offModeFlg(0x2000);
    }

    if (checkWolf() || !checkUnderMove0BckNoArc(ANM_FALL)) {
        s16* var_r27;
        if (mProcVar3.field_0x300e != 0) {
            var_r27 = &field_0x3080;
        } else {
            var_r27 = &field_0x3082;
        }

        cLib_chaseAngleS(var_r27, mProcVar2.field_0x300c, mProcVar4.field_0x3010);
    }

    cLib_chaseF(&mNormalSpeed, 0.0f, field_0x347c);

    if (mProcVar0.field_0x3008 == 0) {
        mProcVar0.field_0x3008 = 1;
        return 1;
    }

    offModeFlg(0x2000);

    if (!checkWolf()) {
        setFallVoice();
    }

    if ((mLinkAcch.ChkGroundHit() || checkEndResetFlg2(ERFLG2_UNK_100)) && !checkBoarSingleBattle()) {
        if (checkWolf()) {
            procWolfLargeDamageUpInit(field_0x3198, mProcVar1.field_0x300a, field_0x3080, field_0x3082);
        } else {
            BOOL temp_r26 = field_0x3480 > 0.0f;
            procLargeDamageUpInit(field_0x3198, mProcVar1.field_0x300a, field_0x3080, field_0x3082);

            if (temp_r26) {
                offModeFlg(8);
                checkCutLandDamage();
            }
        }
    } else if (checkBoarSingleBattle()) {
        if (!checkUnderMove0BckNoArc(ANM_FALL) && field_0x2060->getOldFrameRate() < 0.1f) {
            if (mProcVar5.field_0x3012 != 0) {
                mProcVar5.field_0x3012--;
            } else {
                voiceStart(Z2SE_AL_V_FALL);
                setSingleAnimeBaseMorf(ANM_FALL, mpHIO->mDamage.m.mInvertedFallInterpolation);
                setOldRootQuaternion(field_0x3080, 0, field_0x3082);
                field_0x3080 = 0;
                field_0x3082 = 0;
            }
        }
    } else if (mLinkAcch.ChkWallHit() && mNormalSpeed > field_0x3478) {
        cXyz sp14;
        cXyz sp8;
        dBgS_AcchCir* var_r29 = mAcchCir;

        for (int i = 0; i < 3; i++, var_r29++) {
            if (var_r29->ChkWallHit()) {
                sp14.set(current.pos.x, current.pos.y + var_r29->GetWallH(), current.pos.z);
                sp8.set(sp14.x + (cM_ssin(current.angle.y) * (var_r29->GetWallR() + 25.0f)), sp14.y, sp14.z + (cM_scos(current.angle.y) * (var_r29->GetWallR() + 25.0f)));

                if (commonLineCheck(&sp14, &sp8)) {
                    return procCoLargeDamageWallInit(field_0x3198, mProcVar1.field_0x300a, field_0x3080, field_0x3082);
                }
            }
        }
    }

    return 1;
}

int daAlink_c::procLargeDamageUpInit(int param_0, int param_1, s16 param_2, s16 param_3) {
    if (!commonProcInitNotSameProc(PROC_LARGE_DAMAGE_UP)) {
        return 0;
    }

    const daAlinkHIO_damLaHu_c1* var_r30;
    if (param_1 != 0) {
        var_r30 = &daAlinkHIO_damLarge_c0::m;
    } else {
        var_r30 = &daAlinkHIO_damHuge_c0::m;
    }

    mProcVar1.field_0x300a = 0;
    mProcVar2.field_0x300c = 0;
    field_0x3198 = param_0;
    offGoatStopGame();
    mProcVar4.field_0x3010 = 0;

    if (param_0 == -4) {
        onNoResetFlg2(FLG2_PRESSED_DAMAGE);
        mProcVar0.field_0x3008 = 0;
        mProcVar1.field_0x300a = 1;
        mProcVar2.field_0x300c = 1;

        voiceStart(Z2SE_AL_V_DAMAGE_COMIC);
        seStartOnlyReverb(Z2SE_AL_DAMAGE_LARGE);
        setSingleAnimeBase(ANM_DMG_FBW);

        field_0x32cc = 0x88;
        field_0x3478 = var_r30->mBackGetUpAnm.mCancelFrame;
        field_0x3480 = 39.0f;
        field_0x3484 = 1000.0f;
    } else if (param_0 < 0) {
        daAlink_ANM var_r28;
        s16 var_r27;
        f32 var_f29;
        f32 var_f30;
        f32 var_f31;

        if (param_0 == -3) {
            var_r28 = ANM_DMG_AIR_LAND;
            var_f29 = 0.0f;
            var_r27 = -1;
            var_f30 = 5.0f;
            var_f31 = 1.0f;

            mProcVar0.field_0x3008 = -1;
            field_0x3478 = var_r30->mFrontGetUpAnm.mCancelFrame;
            field_0x3480 = 39.0f;
            field_0x2f9d = 4;
            field_0x3484 = 18.0f;
        } else {
            if (param_0 == -2) {
                var_f30 = 0.0f;
            } else {
                var_f30 = mpHIO->mDamage.mDamLarge.m.mBackGetUpAnm.mInterpolation;
            }

            field_0x3484 = 9.0f;
            var_r28 = ANM_DMG_LARGE_LAND;
            var_f29 = 9.0f;
            var_r27 = -1;
            field_0x3478 = var_r30->mBackGetUpAnm.mCancelFrame;

            if (param_0 == -5 && mDemo.getParam1() > 0) {
                mProcVar0.field_0x3008 = mDemo.getParam1();
                var_f31 = 0.0f;
            } else if (param_0 == -2 && mDemo.getDemoType() == daPy_demo_c::DEMO_TYPE_TOOL_e) {
                mProcVar0.field_0x3008 = 0x1E;
                var_f31 = 0.0f;
            } else {
                mProcVar0.field_0x3008 = -1;
                var_f31 = mpHIO->mDamage.m.mRecoverStandAnmSpeed;
            }

            mProcVar1.field_0x300a = 1;
            mProcVar2.field_0x300c = 1;
            field_0x3480 = 39.0f;
        }

        setSingleAnime(var_r28, var_f31, var_f29, var_r27, var_f30);
        field_0x32cc = var_r28;
    } else {
        mProcVar0.field_0x3008 = 0;

        dComIfGp_getVibration().StartShock(6, 0x1F, cXyz(0.0f, 1.0f, 0.0f));
        seStartMapInfo(Z2SE_BODY_FALL_DOWN);
        field_0x2f9d = 4;

        if (param_0 == 0x84) {
            setSingleAnimeParam(ANM_DMG_LARGE_LAND, &var_r30->mBackGetUpAnm);
            field_0x3478 = var_r30->mBackGetUpAnm.mCancelFrame;
            mProcVar1.field_0x300a = 1;
            field_0x3480 = 39.0f;
            field_0x32cc = 0x88;
            field_0x3484 = 9.0f;
        } else if (param_0 == 0x85) {
            setSingleAnimeParam(ANM_DMG_AIR_LEFT_LAND, &var_r30->mLeftGetUpAnm);
            field_0x3478 = var_r30->mLeftGetUpAnm.mCancelFrame;
            field_0x3480 = 29.0f;
            field_0x32cc = 0x89;
            field_0x3484 = 6.0f;
        } else if (param_0 == 0x86) {
            setSingleAnimeParam(ANM_DMG_AIR_RIGHT_LAND, &var_r30->mRightGetUpAnm);
            field_0x3478 = var_r30->mRightGetUpAnm.mCancelFrame;
            field_0x3480 = 29.0f;
            field_0x32cc = 0x8A;
            field_0x3484 = 6.0f;
        } else {
            setSingleAnimeParam(ANM_DMG_AIR_LAND, &var_r30->mFrontGetUpAnm);
            field_0x3478 = var_r30->mFrontGetUpAnm.mCancelFrame;
            field_0x3480 = 39.0f;
            field_0x32cc = 0x87;
            field_0x3484 = 18.0f;
        }

        if (param_1 != 0) {
            mProcVar4.field_0x3010 = 1;
        }
    }

    field_0x33cc = 1.0f;
    field_0x347c = 2.0f / (field_0x3478 - mUnderFrameCtrl[0].getStart());
    setOldRootQuaternion(param_2, 0, param_3);
    mNormalSpeed = 0.0f;
    current.angle.y = shape_angle.y;
    setFootEffectProcType(4);
    return 1;
}

int daAlink_c::procLargeDamageUp() {
    daPy_frameCtrl_c* temp_r30 = &mUnderFrameCtrl[0];
    mProcVar4.field_0x3010 = 0;
    field_0x33cc = field_0x347c * (field_0x3478 - temp_r30->getFrame());
    onEndResetFlg0(ERFLG0_UNK_8000000);

    if (field_0x3198 == -4) {
        if (checkNoResetFlg2(FLG2_PRESSED_DAMAGE)) {
            return 1;
        }

        setSingleAnime(ANM_DMG_LARGE_LAND, mpHIO->mDamage.mDamLarge.m.mBackGetUpAnm.mSpeed, 9.0f, -1, mpHIO->mDamage.mDamLarge.m.mBackGetUpAnm.mInterpolation);
        field_0x3198 = -3;
        field_0x347c = 2.0f / (field_0x3478 - 9.0f);
    }

    if (mProcVar0.field_0x3008 > 0) {
        mProcVar0.field_0x3008--;

        if (mProcVar0.field_0x3008 == 0) {
            mProcVar0.field_0x3008 = -1;
            setWaterInAnmRate(temp_r30, mpHIO->mDamage.m.mRecoverStandAnmSpeed);
            temp_r30->offEndFlg();
        }
    } else {
        if (temp_r30->getFrame() > field_0x3480) {
            field_0x2f92 = 1;
            field_0x2f93 = 6;
        }

        if (checkAnmEnd(temp_r30)) {
            offModeFlg(0x04000000);

            if ((checkEventRun() && mProcVar0.field_0x3008 < 0) || mDemo.getDemoMode() == 0x1B || mDemo.getDemoMode() == 9) {
                dComIfGp_evmng_cutEnd(mAlinkStaffId);
            } else {
                checkNextAction(0);
            }
        } else if (temp_r30->getFrame() > field_0x3478) {
            if (!checkEventRun()) {
                onModeFlg(4);
            }

            checkNextAction(1);
        } else if (mProcVar2.field_0x300c != 0 && temp_r30->checkPass(18.0f)) {
            voiceStart(Z2SE_AL_V_LANDING_FAIL_2);
            mProcVar2.field_0x300c = 0;
        } else if (checkNoResetFlg0(FLG0_UNK_200000)) {
            onModeFlg(0x04000000);

            if (!dMsgObject_isTalkNowCheck() && temp_r30->getFrame() > 19.0f) {
                temp_r30->setFrame(19.0f);
                getNowAnmPackUnder(UNDER_0)->setFrame(19.0f);
            } else if (dMsgObject_isTalkNowCheck()) {
                offNoResetFlg0(FLG0_UNK_200000);
            }
        } else if (checkEndResetFlg1(ERFLG1_LARGE_DAMAGE_UP_STOP) && temp_r30->getFrame() > field_0x3484) {
            temp_r30->setFrame(field_0x3484);
            getNowAnmPackUnder(UNDER_0)->setFrame(field_0x3484);
        }
    }

    return 1;
}

int daAlink_c::procCoLargeDamageWallInit(int param_0, int param_1, s16 param_2, s16 param_3) {
    if (mProcID == PROC_LARGE_DAMAGE_WALL) {
        return 0;
    }

    cM3dGPla sp18;
    dComIfG_Bgsp().GetTriPla(mLinkLinChk, &sp18);
    s16 temp_r26 = sp18.mNormal.atan2sX_Z();

    if (param_0 < 0 || !cBgW_CheckBWall(sp18.mNormal.y) || cLib_distanceAngleS(temp_r26, (current.angle.y - 0x8000)) > cM_deg2s(30.0f)) {
        return 0;
    }

    commonProcInit(PROC_LARGE_DAMAGE_WALL);

    s16 temp_r29 = cM_atan2s(sp18.mNormal.y, sp18.mNormal.absXZ());
    
    current.angle.y = temp_r26;
    current.pos.x = mLinkLinChk.GetCross().x;
    current.pos.z = mLinkLinChk.GetCross().z;

    setOldRootQuaternion(param_2, 0, param_3);

    if (checkWolf()) {
        if (param_0 == 0x29) {
            field_0x3198 = 0x2D;
            setSingleAnimeWolfBase(WANM_DMG_AIR_FRONT);
            field_0x3080 = temp_r29;
        } else if (param_0 == 0x2A) {
            field_0x3198 = 0x2E;
            setSingleAnimeWolf(WANM_DMG_AIR_LEFT_GETUP, 1.0f, 2.0f, 3, 3.0f);
            field_0x3082 = temp_r29 - 0x4000;
        } else if (param_0 == 0x2B) {
            field_0x3198 = 0x2F;
            setSingleAnimeWolf(WANM_DMG_AIR_RIGHT_GETUP, 1.0f, 2.0f, 3, 3.0f);
            field_0x3082 = 0x4000 - temp_r29;
        } else {
            field_0x3198 = 0x2C;
            setSingleAnimeWolfBase(WANM_DMG_AIR_BACK);
            field_0x3080 = -temp_r29;
        }

        setFaceBasicTexture(FTANM_UNK_91);
    } else {
        const daAlinkHIO_damLaHu_c1* var_r28;
        if (param_1 != 0) {
            var_r28 = &daAlinkHIO_damLarge_c0::m;
        } else {
            var_r28 = &daAlinkHIO_damHuge_c0::m;
        }

        if (param_0 == 0x84) {
            field_0x3198 = 0x88;
            setSingleAnimeParam(ANM_DMG_LARGE_LAND, &var_r28->mBackWallHitAnm);
            field_0x3080 = temp_r29 - 0x4000;
        } else if (param_0 == 0x85) {
            field_0x3198 = 0x89;
            setSingleAnimeParam(ANM_DMG_AIR_LEFT_LAND, &var_r28->mLeftWallHitAnm);
            field_0x3082 = temp_r29 - 0x4000;
        } else if (param_0 == 0x86) {
            field_0x3198 = 0x8A;
            setSingleAnimeParam(ANM_DMG_AIR_RIGHT_LAND, &var_r28->mRightWallHitAnm);
            field_0x3082 = 0x4000 - temp_r29;
        } else {
            field_0x3198 = 0x87;
            setSingleAnimeParam(ANM_DMG_AIR_LAND, &var_r28->mFrontWallHitAnm);
            field_0x3080 = 0x4000 - temp_r29;
        }
    }

    mNormalSpeed = 0.0f;
    speed.y = 0.0f;
    setSpecialGravity(0.0f, maxFallSpeed, 0);

    dComIfGp_getVibration().StartShock(6, 0xF, cXyz(0.0f, 1.0f, 0.0f));
    mProcVar0.field_0x3008 = param_1;
    return 1;
}

int daAlink_c::procCoLargeDamageWall() {
    if (mUnderFrameCtrl[0].checkAnmEnd()) {
        procCoLargeDamageInit(field_0x3198, mProcVar0.field_0x3008, field_0x3080, field_0x3082, NULL, 0);
    }

    return 1;
}

int daAlink_c::procCoPolyDamageInit() {
    seStartOnlyReverb(Z2SE_AL_DAMAGE_NORMAL);

    if (!checkWolf()) {
        voiceStart(Z2SE_AL_V_DAMAGE_S);
    }

    if (checkIronBallWaitAnime()) {
        return 1;
    }

    commonProcInit(PROC_POLY_DAMAGE);

    if (checkWolf()) {
        setSingleAnimeWolfParam(WANM_DMG_SMALL, &mpHIO->mWolf.mWlDamage.m.mPolygonAnm);
        voiceStart(Z2SE_WL_V_DAMAGE);
        field_0x3478 = mpHIO->mWolf.mWlDamage.m.mPolygonAnm.mCancelFrame;
    } else {
        setSingleAnimeParam(ANM_DMG, &mpHIO->mDamage.m.mFloorDmgAnm);
        field_0x3478 = mpHIO->mDamage.m.mFloorDmgAnm.mCancelFrame;
    }

    mNormalSpeed = 0.0f;
    return 1;
}

int daAlink_c::procCoPolyDamage() {
    if (mUnderFrameCtrl[0].checkAnmEnd()) {
        commonCheckNextAction(0);
    } else if (mUnderFrameCtrl[0].getFrame() > field_0x3478) {
        if (!checkWolf()) {
            onModeFlg(4);
        }

        commonCheckNextAction(1);
    }

    return 1;
}

int daAlink_c::procLandDamageInit(int param_0) {
    f32 var_f31;
    if (param_0 == 0) {
        var_f31 = mpHIO->mDamage.mDamFall.m.mSmallDmgLandStartFrame;
        mProcVar0.field_0x3008 = mpHIO->mAutoJump.m.mSpinJumpLandStopTime;

        if (mProcID == PROC_MAGNE_BOOTS_FLY) {
            dComIfGp_getVibration().StartShock(6, 0x1F, cXyz(0.0f, 1.0f, 0.0f));
        } else {
            dComIfGp_getVibration().StartShock(2, 0xF, cXyz(0.0f, 1.0f, 0.0f));
        }
    } else if (param_0 == 1) {
        if (!checkEventRun()) {
            setLandDamagePoint(4);
        }

        var_f31 = mpHIO->mDamage.mDamFall.m.mSmallDmgLandStartFrame;
        mProcVar0.field_0x3008 = mpHIO->mDamage.mDamFall.m.mSmallStopTime;
        dComIfGp_getVibration().StartShock(4, 0x1F, cXyz(0.0f, 1.0f, 0.0f));
    } else {
        if (!checkEventRun()) {
            setLandDamagePoint(8);
        }

        var_f31 = mpHIO->mDamage.mDamFall.m.mLandAnm.mStartFrame;
        mProcVar0.field_0x3008 = mpHIO->mDamage.mDamFall.m.mBigStopTime;
        dComIfGp_getVibration().StartShock(5, 0x1F, cXyz(0.0f, 1.0f, 0.0f));
    }

    commonProcInit(PROC_LAND_DAMAGE);
    mNormalSpeed = 0.0f;

    if (param_0 == 0) {
        offModeFlg(8);
    } else {
        seStartOnlyReverb(JA_SE_LK_FALL_DAMAGE);
        voiceStart(Z2SE_AL_V_LANDING_FAIL);
    }

    setSingleAnime(ANM_DMG_FALL, mpHIO->mDamage.mDamFall.m.mLandAnm.mSpeed, var_f31, mpHIO->mDamage.mDamFall.m.mLandAnm.mEndFrame, mpHIO->mDamage.mDamFall.m.mLandAnm.mInterpolation);
    field_0x3198 = 0;
    field_0x2f9d = 4;
    setFootEffectProcType(4);
    onResetFlg1(RFLG1_UNK_30);
    return 1;
}

int daAlink_c::procLandDamage() {
    daPy_frameCtrl_c* frameCtrl = mUnderFrameCtrl;

    if (field_0x3198 != 0) {
        if (frameCtrl->checkAnmEnd()) {
            checkNextAction(0);
        } else if (frameCtrl->getFrame() > mpHIO->mDamage.mDamFall.m.mStandAnm.mCancelFrame) {
            onModeFlg(4);
            checkNextAction(1);
        }
    } else {
        if (frameCtrl->checkAnmEnd()) {
            if (mProcVar0.field_0x3008 > 0) {
                mProcVar0.field_0x3008--;
            } else {
                setSingleAnimeParam(ANM_DMG_FALL_RECOVER, &mpHIO->mDamage.mDamFall.m.mStandAnm);
                field_0x3198 = 1;
            }
        }
    }

    return 1;
}

int daAlink_c::procCoElecDamageInit(fopAc_ac_c* param_0, dCcD_GObjInf* param_1, int param_2) {
    if (!dComIfGp_event_compulsory(this, NULL, 0xFFEF)) {
        return 0;
    }

    mDemo.setSpecialDemoType();
    u32 temp_r27 = checkModeFlg(0x70C52);

    if (mCopyRodAcKeep.getActor() != NULL) {
        ((daCrod_c*)mCopyRodAcKeep.getActor())->offControll();
    }

    if (param_0 != NULL) {
        param_0->actor_status |= 0x800;
    }

    commonProcInit(PROC_ELEC_DAMAGE);
    mNormalSpeed = 0.0f;
    mProcVar2.field_0x300c = 0;

    daAlink_ANM var_r29;
    daAlink_WANM var_r28;
    if ((mLinkAcch.ChkGroundHit() && !temp_r27) || checkMagneBootsOn()) {
        if (mEquipItem == fpcNm_ITEM_IRONBALL) {
            setIronBallWaitUpperAnime(1);
        }
        var_r29 = ANM_ELEC_STUN_GND;
        var_r28 = WANM_DMG_ELEC_GROUND;
    } else {
        var_r29 = ANM_ELEC_STUN_AIR;
        var_r28 = WANM_DMG_ELEC_AIR;

        setSpecialGravity(0.0f, maxFallSpeed, 0);
        speed.y = 0.0f;

        if (!checkModeFlg(0x40000)) {
            setJumpMode();
        } else {
            dComIfGp_setPlayerStatus0(0, 0x100000);
            field_0x2f99 = 0xF;
            field_0x3588 = l_waitBaseAnime;
            field_0x33b0 = -50.0f;

            if (getZoraSwim()) {
                current.pos.y += 50.0f;
                field_0x3080 += 0x4000;
            }
        }
    }

    if (!dComIfGp_checkCameraAttentionStatus(field_0x317c, 2) && !dComIfGp_checkCameraAttentionStatus(field_0x317c, 0x20)) {
        dCam_getBody()->Stop();
        mProcVar2.field_0x300c = 1;
    }

    if (checkWolf()) {
        voiceStart(Z2SE_WL_V_DAMAGE_ELEC);
        setSingleAnimeWolfBase(var_r28);
        setFaceBasicTexture(FTANM_UNK_91);
    } else {
        voiceStart(Z2SE_AL_V_DAMAGE_ELEC);
        setSingleAnimeBase(var_r29);
    }

    mProcVar0.field_0x3008 = 0x2D;
    mProcVar3.field_0x300e = 3;

    field_0x3198 = checkLargeAttack(param_2) | checkHugeAttack(param_2);

    if (param_1 != NULL) {
        mProcVar4.field_0x3010 = getDamageVec(param_1)->atan2sX_Z();
    } else {
        mProcVar4.field_0x3010 = shape_angle.y + 0x8000;
    }

    dComIfGp_clearPlayerStatus0(0, 0x4000);
    return 1;
}

int daAlink_c::procCoElecDamage() {
    seStartOnlyReverbLevel(Z2SE_AL_ELEC_DAMAGE);

    if (checkModeFlg(0x40000) && field_0x2f99 != 0xF) {
        field_0x2f99 = 7;
        daPy_addCalcShort(&field_0x3080, 0, 4, 0xC00, 0x180);
    }

    if (mProcVar3.field_0x300e != 0) {
        dComIfGp_getVibration().StartQuake(5, 1, cXyz(0.0f, 1.0f, 0.0f));
        mProcVar3.field_0x300e--;
    }

    if (mProcVar0.field_0x3008 != 0) {
        mProcVar0.field_0x3008--;
    } else {
        if (mProcVar2.field_0x300c != 0) {
            dCam_getBody()->Start();
        }

        resetSpecialEvent();
        dComIfGp_getVibration().StopQuake(0x1F);

        if (checkModeFlg(2) || field_0x3198 != 0) {
            current.angle.y = mProcVar4.field_0x3010;
            procCoLargeDamageInit(-1, 1, 0, 0, NULL, 2);
        } else if (checkModeFlg(0x40000)) {
            if (checkWolf()) {
                procWolfSwimWaitInit(0);
            } else {
                procSwimWaitInit(0);
            }
        } else if (mProcID == PROC_ELEC_DAMAGE) {
            checkWaitAction();
        }
    }

    return 1;
}

int daAlink_c::procStEscapeInit() {
    commonProcInit(PROC_HUMAN_ST_ESCAPE);
    setSingleAnimeParam(ANM_FROZEN_FREE, &mpHIO->mDamage.mDamCaught.m.mEscapeAnm);

    mUnderFrameCtrl[0].setFrame(7.0f);
    getNowAnmPackUnder(UNDER_0)->setFrame(7.0f);
    mNormalSpeed = 0.0f;

    return 1;
}

int daAlink_c::procStEscape() {
    daPy_frameCtrl_c* frameCtrl = mUnderFrameCtrl;
    onEndResetFlg0(ERFLG0_UNK_8000000);

    if (frameCtrl->checkAnmEnd()) {
        checkNextAction(0);
    } else if (frameCtrl->getFrame() > mpHIO->mDamage.mDamCaught.m.mEscapeAnm.mCancelFrame) {
        onModeFlg(4);
        checkNextAction(1);
    }

    return 1;
}

int daAlink_c::procDkCaughtInit(fpc_ProcID param_0) {
    if (!commonProcInitNotSameProc(PROC_DK_CAUGHT)) {
        return 0;
    }

    if (checkZeroSpeedF()) {
        onModeFlg(1);
    }

    setSingleAnimeBase(ANM_WAIT_D_B);
    onNoResetFlg1(FLG1_DK_CAUGHT);

    field_0x28f8 = param_0;
    mNormalSpeed = 0.0f;
    mProcVar0.field_0x3008 = mpHIO->mDamage.mDamCaught.m.mGrabDuration;

    fopAc_ac_c* temp_r3 = fopAcM_SearchByID(field_0x28f8);
    if (temp_r3 != NULL) {
        field_0x3478 = temp_r3->home.pos.abs(current.pos);
    }

    field_0x594 = 13.0f;
    return 1;
}

int daAlink_c::procDkCaught() {
    fopAc_ac_c* temp_r3 = fopAcM_SearchByID(field_0x28f8);
    if (temp_r3 == NULL || !checkNoResetFlg1(FLG1_DK_CAUGHT)) {
        voiceStart(Z2SE_AL_V_ZENTEN_FAIL_2);
        return procFrontRollSuccessInit();
    }

    if (checkCaughtEscapeCutTurn()) {
        return 1;
    }

    cXyz sp38 = temp_r3->home.pos - current.pos;
    multVecMagneBootInvMtx(&sp38);
    cLib_addCalcAngleS(&shape_angle.y, sp38.atan2sX_Z(), 2, 0x2000, 0x800);

    cXyz sp2C = current.pos - temp_r3->home.pos;
    f32 temp_f1 = sp2C.abs();
    if (temp_f1 > field_0x3478) {
        current.pos = temp_r3->home.pos + ((sp2C * field_0x3478) / temp_f1);
    }

    f32 var_f30;
    if (checkInputOnR()) {
        if (getDirectionFromCurrentAngle() == 1) {
            current.angle.y += 0x8000;
            mNormalSpeed *= -1.0f;
        }

        if (checkZeroSpeedF()) {
            current.angle.y = field_0x2fe2;
        }

        s16 temp_r28 = current.angle.y;
        cLib_addCalcAngleS(&current.angle.y, field_0x2fe2, mpHIO->mAtnMove.m.mTurnAngleRate, mpHIO->mAtnMove.m.mMaxTurnAngle, mpHIO->mAtnMove.m.mMinTurnAngle);
        var_f30 = mpHIO->mAtnMove.m.mAcceleration * field_0x33a8 * cM_scos((current.angle.y - temp_r28));
    } else {
        var_f30 = 0.0f;
    }

    setNormalSpeedF(var_f30, mpHIO->mAtnMove.m.mDeceleration);

    if (checkZeroSpeedF()) {
        onModeFlg(1);
    } else {
        offModeFlg(1);
    }

    mProcVar0.field_0x3008--;

    if (escapeTrigger()) {
        mProcVar0.field_0x3008 -= mpHIO->mDamage.mDamCaught.m.mInputFadeTime;
    }

    if (checkInputOnR() && abs((s16)(mStickAngle - mPrevStickAngle)) > 0x1000) {
        mProcVar0.field_0x3008 -= mpHIO->mDamage.mDamCaught.m.mInputFadeTime;
    }

    if (mProcVar0.field_0x3008 < 0) {
        voiceStart(Z2SE_AL_V_ZENTEN_FAIL_2);
        procFrontRollSuccessInit();
    } else {
        voiceStartLevel(Z2SE_AL_V_BITTEN_LOOP);
    }

    return 1;
}

void daAlink_c::setScreamWaitAnime() {
    setSingleAnimeBase(ANM_WAIT_F);
    voiceStart(Z2SE_AL_V_TERRORED);
    mProcVar2.field_0x300c = 1;

    f32 tmp_0 = 0.0f;
    dComIfGp_getVibration().StartQuake(5, 1, cXyz(tmp_0, 1.0f, tmp_0));
}

int daAlink_c::procScreamWaitInit() {
    commonProcInit(PROC_SCREAM_WAIT);

    if (checkEndResetFlg1(ERFLG1_UNK_2)) {
        setScreamWaitAnime();
    } else {
        setSingleAnimeBaseSpeed(ANM_WAIT, mpHIO->mMove.m.mWaitAnmSpeed,
                                mpHIO->mBasic.m.mBasicInterpolation);
        mProcVar2.field_0x300c = 0;
    }

    current.angle.y = shape_angle.y;
    mNormalSpeed = 0.0f;
    return 1;
}

int daAlink_c::procScreamWait() {
    onEndResetFlg0(ERFLG0_UNK_8000000);

    if (!checkEndResetFlg1(ERFLG1_UNK_1)) {
        checkNextAction(0);
    } else if (mProcVar2.field_0x300c == 0 && checkEndResetFlg1(ERFLG1_UNK_2)) {
        setScreamWaitAnime();
    }

    return 1;
}

int daAlink_c::procCoSandWallHitInit() {
    if (!commonProcInitNotSameProc(PROC_SAND_WALL_HIT)) {
        return 1;
    }

    if (checkWolf()) {
        setSingleAnimeWolfBase(WANM_WAIT_WIND);
    } else {
        setSingleAnimeBase(ANM_WAIT_WIND);
    }

    mNormalSpeed = 0.0f;

    return 1;
}

int daAlink_c::procCoSandWallHit() {
    if (mLinkAcch.ChkGroundHit()) {
        offModeFlg(2);
    }

    if (!checkEndResetFlg1(ERFLG1_UNK_400)) {
        setDamagePointNormal(1);
        procCoPolyDamageInit();
    }

    return 1;
}

int daAlink_c::procCoLavaReturnInit(BOOL i_sandReturn) {
    if (!dComIfGp_event_compulsory(this, 0, 0xFFFF)) {
        return 0;
    }

    mDemo.setSpecialDemoType();
    u32 chk_40 = checkModeFlg(0x40);

    if (!commonProcInitNotSameProc(PROC_LAVA_RETURN)) {
        return 1;
    }

    if (chk_40) {
        current.pos.x = field_0x3834.x;
        current.pos.z = field_0x3834.z;
    }

    onNoResetFlg0(FLG0_SWIM_UP);

    if (checkWolf()) {
        setSingleAnimeWolfBase(WANM_SWIM_DROWN);
        setFaceBasicTexture(FTANM_UNK_9E);

        if (i_sandReturn) {
            voiceStart(Z2SE_WL_V_FALL_QUICKSAND);
        } else {
            voiceStart(Z2SE_WL_V_FALL_MAGMA);
        }
    } else {
        setSingleAnimeBase(ANM_SWIM_DROWN);

        if (mEquipItem == 0x103) {
            field_0x2f92 = 100;
        }

        if (i_sandReturn) {
            voiceStart(Z2SE_AL_V_FALL_QUICKSAND);
        } else {
            voiceStart(Z2SE_AL_V_FALL_MAGMA);
        }
    }

    if (i_sandReturn) {
        seStartOnlyReverb(Z2SE_AL_SINK_SAND_PITFALL);
        field_0x2fbd = 0xFF;
        offModeFlg(0x40000);

        field_0x2060->getOldFrameTransInfo(0)->mTranslate.y += field_0x3458;
        mSinkShapeOffset -= field_0x3458;
        field_0x2f99 = 0;
        field_0x32cc = 5;
        field_0x3198 = 4;
    } else {
        seStartOnlyReverb(Z2SE_AL_SINK_MAGMA);
        
        cXyz particle_pos(current.pos.x, mWaterY, current.pos.z);
        dComIfGp_particle_set(0x8757, &particle_pos, &tevStr, NULL, NULL);
        dComIfGp_particle_set(0x8758, &particle_pos, &tevStr, NULL, NULL);
        field_0x32cc = 4;
        field_0x3198 = 8;
    }

    field_0x3080 = 0;
    mDamageTimer = 0;
    mNormalSpeed = 0.0f;
    field_0x3194 = 1;

    u32 id = fopAcM_GetID(this);
    dCam_getBody()->StartEventCamera(9, id, "Type", 1, &field_0x3194, 0);
    return 1;
}

int daAlink_c::procCoLavaReturn() {
    cLib_chaseF(&mSinkShapeOffset, 0.0f, 6.0f);

    if (mUnderFrameCtrl[0].checkAnmEnd()) {
        startRestartRoom(field_0x32cc, 0xC9, field_0x3198, 1);
    }

    return 1;
}

int daAlink_c::procCoSwimFreezeReturnInit() {
    if (!dComIfGp_event_compulsory(this, 0, 0xFFFF)) {
        return 0;
    }

    mDemo.setSpecialDemoType();
    u32 chk_40000 = checkModeFlg(0x40000);

    if (!commonProcInitNotSameProc(PROC_SWIM_FREEZE_RETURN)) {
        return 1;
    }

    onNoResetFlg0(FLG0_SWIM_UP);
    J3DTransformInfo* transInfo = field_0x2060->getOldFrameTransInfo(0);

    f32 var_f31;
    if (checkWolf()) {
        setSingleAnimeWolfBaseSpeed(WANM_SWIM_WAIT, mpHIO->mWolf.mWlDamage.mNormal.m.mIceDamageAnmSpeed,
                                    10.0f);
        mUnderFrameCtrl[0].setEnd(3);
        mUnderFrameCtrl[0].setAttribute(0);
        setFaceBasicTexture(FTANM_UNK_90);
        mMidnaAnm = 1;
        var_f31 = mpHIO->mWolf.mWlSwim.m.mStartHeight;
    } else {
        setSingleAnime(ANM_SWIM_DMG_FREEZE, mpHIO->mDamage.mDamNormal.m.mIceDamageASpeed,
                       0.0f, 5, 10.0f);
        voiceStart(Z2SE_AL_V_DAMAGE_FREEZE);
        var_f31 = mpHIO->mSwim.m.mStartHeight;
    }

    seStartOnlyReverb(Z2SE_AL_FREEZE);

    if (!chk_40000) {
        transInfo->mTranslate.y -= var_f31;
        current.pos.y = mWaterY;
    }

    field_0x3080 = 0;
    mDamageTimer = 0;
    mNormalSpeed = 0.0f;
    onNoResetFlg1(FLG1_ICE_FREEZE);
    field_0x3194 = 1;

    u32 id = fopAcM_GetID(this);
    dCam_getBody()->StartEventCamera(9, id, "Type", 1, &field_0x3194, 0);
    mProcVar2.field_0x300c = 0x2D;
    return 1;
}

int daAlink_c::procCoSwimFreezeReturn() {
    if (mProcVar2.field_0x300c == 0) {
        if (checkRestartDead(4, 1)) {
            onNoResetFlg1(FLG1_ICE_FREEZE);
        } else {
            u32 mode = 4;
            setLastSceneDamage(4, &mode);
            seStartSystem(Z2SE_FORCE_BACK);
            dStage_changeScene(3, 0.0f, mode, fopAcM_GetRoomNo(this), shape_angle.y, -1);
        }
    } else {
        mProcVar2.field_0x300c--;
    }

    return 1;
}
