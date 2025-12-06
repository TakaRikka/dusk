/**
 * d_a_alink_cut.inc
 * Player Sword action handling
 */

#include "d/actor/d_a_alink.h"
#include "d/actor/d_a_b_gnd.h"
#include "SSystem/SComponent/c_math.h"

static fopAc_ac_c* daAlink_searchNightStalker(fopAc_ac_c* i_actor, void*) {
    if (fopAcM_GetName(i_actor) == PROC_E_S1) {
        daPy_py_c* player_p = daPy_getLinkPlayerActorClass();
        if (player_p->current.pos.abs2XZ(i_actor->current.pos) < 250000.0f &&
            fabsf(player_p->current.pos.y - i_actor->current.pos.y) < 300.0f)
        {
            return i_actor;
        }
    } else if (fopAcM_GetName(i_actor) == PROC_B_GND &&
               static_cast<b_gnd_class*>(i_actor)->checkAttackChance())
    {
        return i_actor;
    }

    return NULL;
}

void daAlink_c::checkLightSwordMtrl() {
    if (checkLightMasterSwordEquip()) {
        stage_stag_info_class* stag_info = dComIfGp_getStageStagInfo();

        if (dStage_stagInfo_GetSaveTbl(stag_info) == 0x17 ||
            fopAcIt_Judge((fopAcIt_JudgeFunc)daAlink_searchNightStalker, NULL))
        {
            onNoResetFlg3(FLG3_UNK_100000);
            return;
        }
    }

    offNoResetFlg3(FLG3_UNK_100000);
}

BOOL daAlink_c::checkSwordEquipAnime() const {
    return !checkNoResetFlg0(FLG0_UNK_1000000) &&
           (checkUpperAnime(0x255) || checkUpperAnime(0x261));
}

BOOL daAlink_c::checkCutDashAnime() const {
    return checkUpperAnime(0x80) || checkUpperAnime(0x82) || checkUpperAnime(0x81);
}

BOOL daAlink_c::checkCutDashEnemyHit(dCcD_GObjInf& obj) {
    return obj.ChkAtHit() && checkEnemyGroup(obj.GetAtHitAc());
}

u32 daAlink_c::getSwordAtType() {
    return checkMasterSwordEquip() ? 0x4000002 : 2;
}

void daAlink_c::initCutTurnAt(f32 param_0, int param_1) {
    field_0xFB8.ResetAtHit();
    field_0xFB8.OnAtSetBit();
    field_0xFB8.SetAtType(getSwordAtType());
    field_0xFB8.SetAtHitMark(3);
    field_0xFB8.SetAtSe(1);
    field_0xFB8.SetAtAtp(param_1);

    u8 mtrl;
    if (!checkNoResetFlg3(FLG3_UNK_100000)) {
        mtrl = dCcD_MTRL_NONE;
    } else {
        mtrl = dCcD_MTRL_LIGHT;
    }
    field_0xFB8.SetAtMtrl(mtrl);
    field_0xFB8.SetR(param_0);
    field_0x2fd0 = 1;
}

bool daAlink_c::checkCutFinishJumpUp() {
    if (checkCutBackState() && !checkNotBattleStage() && swordSwingTrigger()) {
        return true;
    }

    return false;
}

void daAlink_c::changeCutFast() {
    deleteEquipItem(FALSE, FALSE);
    setSwordModel();
    mComboCutCount = 3;
    checkCutAction();
    onNoResetFlg0(FLG0_UNK_80000);
}

bool daAlink_c::checkCutFastReady() {
    if (mProcID == PROC_CUT_FAST_READY) {
        return true;
    }

    if (checkNotBattleStage()) {
        return false;
    }

    fopAc_ac_c* target_p = mAttention->LockonTarget(0);

    if (mEquipItem == fpcNm_ITEM_NONE && (dComIfGs_isEventBit(0x2A80) || checkNoResetFlg3(FLG3_UNK_80)))
    {
        if (checkModeFlg(1) && !checkRideOn() && checkSwordGet() && checkModeFlg(4) &&
            !checkModeFlg(0x40000) && mTargetedActor == NULL && checkEnemyGroup(target_p))
        {
            f32 xz_dist = target_p->eyePos.abs2XZ(current.pos);
            f32 y_dist = fabsf(target_p->eyePos.y - current.pos.y);

            if ((xz_dist < 160000.0f && y_dist < 300.0f) ||
                (checkNoResetFlg0(FLG0_UNK_1000000) && xz_dist < 202500.0f && y_dist < 350.0f))
            {
                return true;
            }
        }
    }

    return false;
}


void daAlink_c::setSwordModel() {
    field_0x2fde = 0xFF;
    mEquipItem = 0x103;

    mDoAud_bgmSetSwordUsing(1);
    field_0x2f94 = 0;

    if (checkWoodSwordEquip()) {
        mSwordModel->getModelData()->getMaterialNodePointer(1)->getShape()->hide();
    } else {
        mSwordModel->getModelData()->getMaterialNodePointer(0)->getShape()->show();
    }
}

void daAlink_c::offSwordModel() {
    mDoAud_bgmSetSwordUsing(0);

    if (mSwordModel == mpSwMModel || mSwordModel == mpSwAModel) {
        mSwordModel->getModelData()->getMaterialNodePointer(0)->getShape()->hide();
    } else if (mSwordModel == mWoodSwordModel) {
        mWoodSwordModel->getModelData()->getMaterialNodePointer(1)->getShape()->show();
    }
}

BOOL daAlink_c::checkCutTypeNoBlur() const {
    return mCutType == CUT_TYPE_NM_STAB || mCutType == CUT_TYPE_COMBO_STAB ||
           mCutType == CUT_TYPE_FINISH_STAB || checkNoResetFlg0(FLG0_UNDERWATER);
}

bool daAlink_c::checkCutTurnInput() const {
    return 0xF800 < abs(field_0x3180);
}

// Debug version is correct, but breaks other versions due no longer inlining
int daAlink_c::getCutTurnDirection() const {
#if VERSION == VERSION_SHIELD_DEBUG
    if (field_0x3180 < 0) {
        return 1;
    } else {
        return 0;
    }
#else
    return field_0x3180 < 0;
#endif
}

void daAlink_c::resetCombo(int param_0) {
    mComboCutCount = 0;
    offNoResetFlg0(FLG0_UNK_8000);
    if (param_0 != 0) {
        mRunCutComboCount = 0;
    }
}

void daAlink_c::checkComboCnt() {
    if ((mEquipItem == 0x103 && mComboCutCount == 2) &&
        (checkAttentionLock() &&
         (mAtCps[0].ChkAtHit() || mAtCps[1].ChkAtHit() || mAtCps[2].ChkAtHit())))
    {
        onNoResetFlg0(FLG0_UNK_8000);
    }

    if (field_0x307e > 0) {
        field_0x307e--;
    } else {
        resetCombo(1);
    }
}

void daAlink_c::setCutType(u8 type) {
    mCutType = type;
    offResetFlg0(RFLG0_UNK_8000000);
}

void daAlink_c::setCylAtParam(u32 i_AtType, dCcG_At_Spl i_spl, u8 i_hitMark, u8 i_AtSe, int i_atp,
                              f32 i_radius, f32 i_height) {
    if (checkWoodSwordEquip() && (i_AtSe == 14 || i_AtSe == 1 || i_AtSe == 16)) {
        i_AtSe = 5;
    }

    u8 mtrl;
    if (i_AtType & 2) {
        if (checkNoResetFlg3(FLG3_UNK_100000)) {
            mtrl = dCcD_MTRL_LIGHT;
        } else {
            mtrl = dCcD_MTRL_NONE;
        }
    } else if (mEquipItem == fpcNm_ITEM_WATER_BOTTLE) {
        mtrl = dCcD_MTRL_UNK_6;
    } else {
        mtrl = dCcD_MTRL_NONE;
    }

    mAtCyl.SetAtType(i_AtType);
    mAtCyl.SetAtSpl(i_spl);
    mAtCyl.SetAtHitMark(i_hitMark);
    mAtCyl.SetAtSe(i_AtSe);
    mAtCyl.SetAtAtp(i_atp);
    mAtCyl.SetR(i_radius);
    mAtCyl.SetH(i_height);
    mAtCyl.SetAtMtrl(mtrl);

    if (mProcID == PROC_GUARD_ATTACK) {
        mAtCyl.OnAtNoHitMark();
    } else {
        mAtCyl.OffAtNoHitMark();
    }

    if (checkWolf()) {
        dCcD_Cyl* cyl = field_0x850;
        for (int i = 0; i < 3; i++) {
            cyl->SetAtType(i_AtType);
            cyl->SetAtSpl(i_spl);
            cyl->SetAtHitMark(i_hitMark);
            cyl->SetAtSe(i_AtSe);
            cyl->SetAtAtp(i_atp);
            cyl++;
        }
    }
}

void daAlink_c::setSwordAtParam(dCcG_At_Spl i_spl, u8 i_hitMark, u8 i_AtSe, int i_atp, f32 param_4,
                                f32 i_radius) {
    dCcD_Cps* atCps = mAtCps;
    u32 atType = getSwordAtType();
    mAtCps[0].OffAtNoHitMark();

    if (checkWoodSwordEquip() && (i_AtSe == 14 || i_AtSe == 1 || i_AtSe == 16)) {
        i_AtSe = 5;
    }

    if (i_spl == 0) {
        field_0x2fd0 = 0;
    } else {
        field_0x2fd0 = 1;
    }

    u8 mtrl;
    if (checkNoResetFlg3(FLG3_UNK_100000)) {
        mtrl = dCcD_MTRL_LIGHT;
    } else {
        mtrl = dCcD_MTRL_NONE;
    }

    for (int i = 0; i < 3; i++) {
        atCps->SetAtSpl(i_spl);
        atCps->SetAtHitMark(i_hitMark);
        atCps->SetAtAtp(i_atp);
        atCps->SetR(i_radius);
        atCps->SetAtSe(i_AtSe);
        atCps->SetAtMtrl(mtrl);
        atCps->SetAtType(atType);
        atCps++;
    }

    field_0x33d0 = param_4;
}

BOOL daAlink_c::notSwordHitVibActor(fopAc_ac_c* p_actor) {
    if (p_actor != NULL && fopAcM_GetGroup(p_actor) == 4) {
        s16 name = fopAcM_GetName(p_actor);

        if (!checkSpecialNpc(p_actor) && name != PROC_NI && name != PROC_NPC_BLUENS) {
            return true;
        }
    }

    return false;
}

BOOL daAlink_c::setSwordHitVibration(dCcD_GObjInf* i_gobj) {
    if (i_gobj->ChkAtHit()) {
        if (notSwordHitVibActor(i_gobj->GetAtHitAc())) {
            return true;
        }

        int var_r4;
        if (checkWolf()) {
            var_r4 = 3;
        } else if (i_gobj->ChkAtShieldHit()) {
            if (field_0x2fd0 == 1) {
                var_r4 = 5;
            } else if (field_0x2fd0 == 2) {
                var_r4 = 2;
            } else {
                var_r4 = 3;
            }
        } else if (field_0x2fd0 == 2 ||
                   (i_gobj->GetAtHitAc() != NULL && fopAcM_GetGroup(i_gobj->GetAtHitAc()) == 3))
        {
            var_r4 = 2;
        } else if (field_0x2fd0 == 1) {
            var_r4 = 4;
        } else {
            var_r4 = 2;
        }

        dComIfGp_getVibration().StartShock(var_r4, 31, cXyz(0.0f, 1.0f, 0.0f));
        return true;
    }

    return false;
}

BOOL daAlink_c::checkAtShieldHit(dCcD_GObjInf& param_0) {
    return param_0.ChkAtHit() && param_0.ChkAtShieldHit();
}

BOOL daAlink_c::checkCutReverseAt(dCcD_GObjInf* param_0) {
    param_0->GetAtHitAc();
    return checkAtShieldHit(*param_0) ? TRUE : FALSE;
}

BOOL daAlink_c::changeCutReverseProc(daAlink_c::daAlink_ANM param_0) {
    if (checkCutReverseAt(&mAtCps[0]) || (mEquipItem != fpcNm_ITEM_COPY_ROD && (checkCutReverseAt(&mAtCps[1]) || checkCutReverseAt(&mAtCps[2])))) {
        return procCutReverseInit(param_0);
    }

    if (checkNoResetFlg0(FLG0_UNK_40) || mEquipItem == fpcNm_ITEM_COPY_ROD) {
        cXyz sp28;
        Vec sp1C;

        if (checkMagneBootsOn()) {
            sp1C.x = 0.0f;
            sp1C.y = 30.0f;
            sp1C.z = 0.0f;
            mDoMtx_multVecSR(mMagneBootMtx, &sp1C, &sp28);

            sp28 += current.pos;
        } else {
            sp28.x = current.pos.x;
            sp28.y = current.pos.y + 30.0f;
            sp28.z = current.pos.z;
        }

        if (commonLineCheck(&sp28, &field_0x3720) && !dComIfG_Bgsp().GetPolyAttackThrough(mLinkLinChk) && dComIfG_Bgsp().GetWallCode(mLinkLinChk) != 7) {
            cM3dGPla sp34;
            dComIfG_Bgsp().GetTriPla(mLinkLinChk, &sp34);

            f32 var_f31;
            if (checkMagneBootsOn()) {
                mDoMtx_multVecSR(mMagneBootInvMtx, &sp34.mNormal, &sp1C);
                var_f31 = sp1C.y;
            } else {
                var_f31 = sp34.mNormal.y;
            }

            if (!cBgW_CheckBGround(var_f31) && !checkCutDashAnime()) {
                int temp_r3 = dComIfG_Bgsp().GetPolyAtt0(mLinkLinChk);

                if (!checkWoodSwordEquip() && (temp_r3 == 2 || temp_r3 == 5 || temp_r3 == 0xE || temp_r3 == 8 || temp_r3 == 0xA)) {
                    csXyz sp8(cM_atan2s(sp34.mNormal.y, sp34.mNormal.absXZ()), cM_atan2s(-sp34.mNormal.x, -sp34.mNormal.z), 0);
    
                    u16 var_r28;
                    if (temp_r3 == 5 || temp_r3 == 0xE) {
                        var_r28 = 0x752;
                    } else if (temp_r3 == 8) {
                        var_r28 = 0xAE3;
                    } else {
                        var_r28 = 0x32E;
                        dKy_SordFlush_set(*mLinkLinChk.GetCrossP(), 2);
                    }

                    dComIfGp_particle_set(var_r28, mLinkLinChk.GetCrossP(), &sp8, NULL);
                }

                u32 var_r27;
                if (mAtCps[0].GetAtSe() == 14) {
                    var_r27 = 0x40000;
                } else {
                    var_r27 = dCcD_GObjInf::getHitSeID(mAtCps[0].GetAtSe(), 0);
                }

                mZ2Link.startHitItemSE(var_r27, dKy_pol_sound_get(&mLinkLinChk), NULL, -1.0f);
                return procCutReverseInit(param_0);
            }
        }
    }

    return 0;
}

void daAlink_c::setCutDash(int param_0, int param_1) {
    if (!param_0 && checkNoResetFlg1(FLG1_UNK_4000000)) {
        checkCutAction();
    } else {
        field_0x2fce = 5;
        resetCombo(0);
        offNoResetFlg2(FLG2_UNK_2);

        setSwordAtParam(dCcG_At_Spl_UNK_0, 7, 16, 1, mpHIO->mCut.m.mRunCutLength,
                        mpHIO->mCut.m.mRunCutRadius);
        field_0x2fd0 = 2;
        mRunCutComboCount++;

        if (param_1) {
            setUpperAnimeParam(0x82, UPPER_2, &mpHIO->mCut.mCutDashCharge.m.mCutAnm);

            if (checkBoardRide()) {
                setCutType(13);
            } else {
                setCutType(0x38);
            }

            if (mRunCutComboCount == 3) {
                mRunCutComboCount = 1;
            }
        } else if (mRunCutComboCount == 1 || mRunCutComboCount == 3) {
            setUpperAnimeParam(0x80, UPPER_2, &mpHIO->mCut.mCutDashLeft.m.mCutAnm);

            if (checkBoardRide()) {
                setCutType(11);
            } else if (mProcID == PROC_SIDESTEP) {
                setCutType(CUT_TYPE_AIR);
            } else {
                setCutType(0x25);
            }

            mRunCutComboCount = 1;
            setFacePriTexture(FTANM_UNK_13);
        } else {
            setUpperAnimeParam(0x81, UPPER_2, &mpHIO->mCut.mCutDashRight.m.mCutAnm);

            if (checkBoardRide()) {
                setCutType(12);
            } else if (mProcID == PROC_SIDESTEP) {
                setCutType(0x11);
            } else {
                setCutType(0x26);
            }

            setFacePriTexture(FTANM_UNK_14);
        }

        setFacePriBck(0x104);
        field_0x2f96 = 2;
        field_0x307e = mpHIO->mCut.m.mComboDuration;

        setSwordVoiceSe(Z2SE_AL_V_ATTACK_RUN);
        setCylAtParam(getSwordAtType(), dCcG_At_Spl_UNK_0, 7, 16, 1, 50.0f, 130.0f);
        setCutWaterDropEffect();
        field_0x2fb7 = 0;
    }
}

BOOL daAlink_c::checkForceSwordSwing() {
    return mEquipItem == 0x103 && swordSwingTrigger();
}

void daAlink_c::setComboReserb() {
    if (checkWolf()) {
        field_0x307e = mpHIO->mWolf.mWlAttack.m.mComboDuration;
    } else {
        field_0x307e = mpHIO->mCut.m.mComboDuration;
    }

    if (mComboCutCount != 4 && field_0x2fce == 0 && swordSwingTrigger()) {
        onNoResetFlg2(FLG2_UNK_2);
    }
}

BOOL daAlink_c::checkComboReserb() {
    return checkNoResetFlg2(FLG2_UNK_2) && !checkEventRun();
}

int daAlink_c::commonCutAction() {
    if (mComboCutCount == 4) {
        resetCombo(1);
    }

    if (dComIfGp_getDoStatus() == 4) {
        setDoStatus(0);
    }
    mComboCutCount++;

    if (checkReinRide()) {
        procHorseCutInit();
        if (mComboCutCount > 1) {
            mComboCutCount = 1;
        }
        return 1;
    }

    return 0;
}

void daAlink_c::setSwordVoiceSe(u32 i_seID) {
    if (!checkNoResetFlg0(FLG0_UNDERWATER)) {
        voiceStart(i_seID);
    }
}

void daAlink_c::setSwordChargeVoiceSe() {
    if (mProcVar2.field_0x300c != 0) {
        mProcVar2.field_0x300c--;

        if (mProcVar2.field_0x300c == 0) {
            setSwordVoiceSe(Z2SE_AL_V_TAME);
        }
    }
}

void daAlink_c::setSwordComboVoice() {
    if (mProcID == PROC_CUT_JUMP || mComboCutCount == 4) {
        setSwordVoiceSe(Z2SE_AL_V_ATTACK_L);
    } else if (mComboCutCount == 1) {
        setSwordVoiceSe(Z2SE_AL_V_ATTACK_S);
    } else {
        setSwordVoiceSe(Z2SE_AL_V_ATTACK_M);
    }
}

bool daAlink_c::checkCutTurnInputTrigger() {
    return swordSwingTrigger() && checkCutTurnInput();
}

int daAlink_c::checkCutAction() {
    static int const atnNormalType1[5] = {4, 1, 1, 2, 0};
    static int const atnNormalType2[5] = {3, 0, 0, 1, 1};
    static int const atnNormalType3[5] = {4, 1, 1, 0, 0};
    static int const atnFinishType[5] = {2, 1, 0, 5, 1};
    static int const hitType[5] = {4, 0, 5, 0, 1};
    static int const hitFinishType[5] = {2, 1, 0, 5, 0};
    static int const normalType1[5] = {0, 2, 1, 2, 1};
    static int const normalType2[5] = {1, 1, 2, 1, 2};
    static int const normalType3[5] = {4, 0, 1, 2, 1};
    static int const finishType[5] = {2, 0, 0, 5, 1};

    if (commonCutAction()) {
        return 1;
    }

    int cutDir = getCutDirection();

    if (mComboCutCount == 4) {
        if (checkNoResetFlg0(FLG0_UNK_1000000)) {
            if (checkInputOnR() && (cutDir == DIR_FORWARD || cutDir == DIR_BACKWARD)) {
                procCutFinishInit(3);
            } else {
                procCutFinishInit(4);
            }
        } else if (checkNoResetFlg0(FLG0_UNK_8000)) {
            procCutFinishInit(hitFinishType[cutDir]);
        } else if (checkAttentionLock()) {
            procCutFinishInit(atnFinishType[cutDir]);
        } else {
            procCutFinishInit(finishType[cutDir]);
        }
    } else if (mComboCutCount == 3) {
        if (checkNoResetFlg0(FLG0_UNK_8000)) {
            if (cutDir == 0) {
                procCutNormalInit(hitType[cutDir]);
            } else {
                procCutFinishInit(hitType[cutDir]);
            }
        } else if (checkAttentionLock()) {
            procCutNormalInit(atnNormalType3[cutDir]);
        } else {
            procCutNormalInit(normalType3[cutDir]);
        }
    } else if (mProcID == PROC_CUT_NORMAL && mProcVar1.field_0x300a != 4) {
        if (mProcVar1.field_0x300a == 2) {
            procCutNormalInit(1);
        } else {
            procCutNormalInit(2);
        }
    } else if (checkAttentionLock()) {
        if (mComboCutCount == 1) {
            procCutNormalInit(atnNormalType1[cutDir]);
        } else {
            procCutNormalInit(atnNormalType2[cutDir]);
        }
    } else if (mComboCutCount == 1) {
        procCutNormalInit(normalType1[cutDir]);
    } else {
        procCutNormalInit(normalType2[cutDir]);
    }

    return 1;
}

void daAlink_c::checkCutTurnCharge() {
    if (swordButton()) {
        if (mProcVar5.field_0x3012 < 3) {
            mProcVar5.field_0x3012++;
        } else {
            onResetFlg0(RFLG0_UNK_40);
        }
    } else {
        mProcVar5.field_0x3012 = 0;
    }
}

int daAlink_c::getCutDirection() {
    s16 angle;

    if (!checkInputOnR()) {
        return DIR_NONE;
    }

    if (mTargetedActor != NULL) {
        angle = field_0x2fe2 - fopAcM_searchActorAngleY(this, mTargetedActor);
    } else {
        angle = field_0x2fe2 - field_0x2fe6;
    }

    return getDirectionFromAngle(angle);
}

BOOL daAlink_c::checkCutCancelNextMode(int param_0) {
    f32 old_speed = mNormalSpeed;
    u8 old_2f98 = field_0x2f98;

    mNormalSpeed = 0.0f;
    field_0x2f98 = param_0;
    onModeFlg(4);

    if (checkNextAction(1)) {
        return 1;
    }

    mNormalSpeed = old_speed;
    field_0x2f98 = old_2f98;
    return 0;
}

BOOL daAlink_c::checkDoCutAction() {
    // event flag 0x2A40: Learned Jump Strike
    if (doButton() && (dComIfGs_isEventBit(0x2A40) || checkNoResetFlg3(FLG3_UNK_200))) {
        return procCutLargeJumpChargeInit();
    } else if (checkDownAttackState()) {
        return procCutDownInit();
    } else if (checkCutHeadState()) {
        return procCutHeadInit();
    } else {
        return procCutJumpInit(0);
    }
}

BOOL daAlink_c::checkCutBackState() {
    // event flag 0x2902: Learned Backslice
    return dComIfGs_isEventBit(0x2902) || checkNoResetFlg3(FL3_TRANING_CUT_BACK);
}

BOOL daAlink_c::checkCutHeadState() {
    fopEn_enemy_c* temp_r27 = (fopEn_enemy_c*)mTargetedActor;

    // event flag 0x2901: Learned Helm Splitter
    return ((!checkBootsOrArmorHeavy() && (temp_r27 != NULL) &&
             (dComIfGs_isEventBit(0x2901) || checkNoResetFlg3(FLG3_UNK_40))) &&
            fopAcM_GetGroup(temp_r27) == 2) &&
           temp_r27->checkHeadLockFlg();
}

BOOL daAlink_c::checkDownAttackState() {
    fopEn_enemy_c* enemy = (fopEn_enemy_c*)mTargetedActor;

    if (enemy != NULL &&
        (fopAcM_GetGroup(enemy) == 2 && enemy->checkDownFlg() &&
         ((!checkWolf() && (dComIfGs_isEventBit(0x2904) || checkNoResetFlg3(FLG3_UNK_10)) &&
           enemy->getDownPos().abs2(current.pos) < 640000.0f) ||
          (checkWolf() &&
           (fopAcM_GetName(mTargetedActor) == PROC_E_PO ||
            fopAcM_GetName(mTargetedActor) == PROC_E_HP) &&
           enemy->getDownPos().abs2(current.pos) < 640000.0f))))
    {
        return true;
    }

    return false;
}

BOOL daAlink_c::checkCutLargeTurnState() const {
    // event flag 0x2A20: Learned Great Spin
    return ((dComIfGs_isEventBit(0x2A20) || checkNoResetFlg3(FLG3_UNK_100)) &&
            dComIfGs_getLife() == dComIfGs_getMaxLifeGauge()) ||
           (mDemo.getDemoMode() == 0x24 && mDemo.getParam0() == 0x33);
}

void daAlink_c::cancelCutCharge() {
    if (mComboCutCount == 4) {
        resetCombo(1);
        checkNextAction(0);
    } else {
        mComboCutCount++;

        if (mComboCutCount <= 3) {
            procCutNormalInit(2);
        } else {
            procCutFinishInit(1);
        }
    }
}

void daAlink_c::initCutAtnActorSearch() {
    if (cLib_distanceAngleS(getShapeAngleYAtnActor(), shape_angle.y) < 0x800) {
        mProcVar4.field_0x3010 = 1;
    } else {
        mProcVar4.field_0x3010 = 0;
    }

    field_0x32cc = (uintptr_t)mTargetedActor;
}

void daAlink_c::checkCutAtnActorChange() {
    if (field_0x32cc != (uintptr_t)mTargetedActor) {
        mProcVar4.field_0x3010 = 0;
        field_0x32cc = (uintptr_t)mTargetedActor;
    }
}

void daAlink_c::setCutJumpSpeed(int i_airAt) {
    if (checkNoResetFlg0(FLG0_UNDERWATER)) {
        mNormalSpeed *= mpHIO->mItem.mIronBoots.m.mWaterVelocityX;
        speed.y *= mpHIO->mItem.mIronBoots.m.mWaterVelocityY;
    } else if (checkHeavyStateOn(1, 1)) {
        speed.y *= 1.35f;
    }

    if (mTargetedActor != NULL && !i_airAt) {
        shape_angle.y = cLib_targetAngleY(&current.pos, &mTargetedActor->eyePos);
        cXyz targetPos(mTargetedActor->eyePos.x - (cM_ssin(shape_angle.y) * 70.0f),
                       mTargetedActor->eyePos.y + 50.0f,
                       mTargetedActor->eyePos.z - (cM_scos(shape_angle.y) * 70.0f));

        if (fopAcM_gc_c::gndCheck(&targetPos)) {
            targetPos.y = fopAcM_gc_c::getGroundY();
        } else {
            targetPos.y = mTargetedActor->eyePos.y;
        }

        f32 speed_y_delta = speed.y + gravity; // 23.6
        f32 inverse_gravity = 1.0f / gravity; // -0.29
        f32 fvar1 = targetPos.y - (current.pos.y - (inverse_gravity * (speed_y_delta * speed_y_delta) * 0.5f));

        if (fvar1 > 0.0f) {
            fvar1 = 0.0f;
        }
        f32 sqrt = JMAFastSqrt(fvar1 * 2.0f * inverse_gravity);
        f32 speedDiv = sqrt - (speed_y_delta * inverse_gravity);
        f32 squareDist = current.pos.absXZ(targetPos);

        if (squareDist > 500.0f) {
            squareDist = 500.0f;
        }
        mNormalSpeed = squareDist / speedDiv;
    }
}


int daAlink_c::procCutNormalInit(int i_type) {
    static daAlink_cutParamTbl const cutParamTable[5] = {
        {
            daAlink_c::ANM_CUT_NM_VERTICAL,
            0x6F,
            daAlink_c::CUT_TYPE_NM_VERTICAL,
            1,
            0,
            0,
            6.0f,
        },
        {
            daAlink_c::ANM_CUT_NM_LEFT,
            0x6F,
            daAlink_c::CUT_TYPE_NM_LEFT,
            1,
            0,
            0,
            5.0f,
        },
        {
            daAlink_c::ANM_CUT_NM_RIGHT,
            0x6F,
            daAlink_c::CUT_TYPE_NM_RIGHT,
            1,
            0,
            0,
            5.0f,
        },
        {
            daAlink_c::ANM_CUT_COMBO_STAB,
            0x6F,
            daAlink_c::CUT_TYPE_COMBO_STAB,
            14,
            0,
            0,
            6.0f,
        },
        {
            daAlink_c::ANM_CUT_NM_STAB,
            0x70,
            daAlink_c::CUT_TYPE_NM_STAB,
            14,
            0,
            0,
            6.0f,
        },
    };
    const daAlink_cutParamTbl* cutParams = &cutParamTable[i_type];

    commonProcInit(PROC_CUT_NORMAL);
    setCutType(cutParams->m_cutType);
    field_0x3198 = cutParams->field_0x4;

    const daAlinkHIO_cutNormal_c1* cutData;
    if (i_type == 1) {
        cutData = &mpHIO->mCut.mCutLeft.m;
    } else if (i_type == 2) {
        cutData = &mpHIO->mCut.mCutRight.m;
    } else if (i_type == 4) {
        cutData = &mpHIO->mCut.mCutRightStab.m;
    } else if (i_type == 3) {
        cutData = &mpHIO->mCut.mCutLeftStab.m;
    } else {
        cutData = &mpHIO->mCut.mCutVertical.m;
    }

    field_0x3478 = cutData->mAttackStartFrame;
    field_0x347c = cutData->mAttackEndFrame;
    field_0x3480 = cutData->mSpeed;

    if (checkHeavyStateOn(1, 1)) {
        field_0x3480 *= mHeavySpeedMultiplier;
    }

    field_0x3484 = cutData->mCutAnm.mCancelFrame;

    f32 morf;
    if (!checkZoraWearAbility() && checkNoResetFlg0(FLG0_UNDERWATER)) {
        morf = cutParams->m_morf;
    } else {
        morf = cutData->mCutAnm.mInterpolation;
    }

    setSingleAnime(cutParams->m_anmID, cutData->mCutAnm.mSpeed, cutData->mCutAnm.mStartFrame,
                   cutData->mCutAnm.mEndFrame, morf);
    current.angle.y = shape_angle.y;

    if (!checkAttentionLock() && checkInputOnR()) {
        mProcVar2.field_0x300c = field_0x2fe2;
    } else {
        mProcVar2.field_0x300c = shape_angle.y;
    }

    if (mComboCutCount != 1) {
        field_0x2f99 = 12;
    }

    field_0x3588 = l_halfAtnWaitBaseAnime;
    field_0x307e = mpHIO->mCut.m.mComboDuration;

    setSwordAtParam(dCcG_At_Spl_UNK_0, 1, cutParams->m_atSe, 2, mpHIO->mCut.m.mSwordLength,
                    mpHIO->mCut.m.mSwordRadius);
    setSwordComboVoice();
    mProcVar5.field_0x3012 = 0;

    dComIfGp_setPlayerStatus0(0, 0x8000);
    setCutWaterDropEffect();

    int cutDir = getCutDirection();
    if (mComboCutCount == 1 && (cutDir == DIR_LEFT || cutDir == DIR_RIGHT)) {
        mProcVar3.field_0x300e = 1;
    } else {
        mProcVar3.field_0x300e = 0;
    }

    mProcVar1.field_0x300a = 4;
    initCutAtnActorSearch();
    return 1;
}

int daAlink_c::procCutNormal() {
    if (checkGroundSpecialMode()) {
        return 1;
    }
    setBodyAngleXReadyAnime(1);

    daPy_frameCtrl_c* frameCtrl = mUnderFrameCtrl;
    setComboReserb();
    checkCutTurnCharge();
    checkCutAtnActorChange();

    cLib_chaseF(&mNormalSpeed, 0.0f, mpHIO->mMove.m.mDeceleration);
    onEndResetFlg0(ERFLG0_UNK_8000000);
    field_0x2f99 = 4;

    if (mProcVar3.field_0x300e != 0 && checkNoResetFlg2(FLG2_UNK_2)) {
        int cutDir = getCutDirection();
        if ((cutDir == DIR_RIGHT && mCutType == CUT_TYPE_NM_LEFT &&
             frameCtrl->getFrame() > 12.0f) ||
            (cutDir == DIR_LEFT && mCutType == CUT_TYPE_NM_RIGHT &&
             frameCtrl->getFrame() > 12.0f))
        {
            mProcVar1.field_0x300a = cutDir;
        }
    }

    if (frameCtrl->checkAnmEnd()) {
        resetCombo(1);

        if (mDemo.getDemoMode() == 0x24 || mDemo.getDemoMode() == 0x54) {
            dComIfGp_evmng_cutEnd(mAlinkStaffId);
        } else {
            checkNextAction(0);
        }
    } else if ((!(frameCtrl->getFrame() > field_0x3484) && mProcVar1.field_0x300a == 4) ||
               !checkCutCancelNextMode(3))
    {
        if (frameCtrl->getFrame() > field_0x3484) {
            resetCombo(1);
        }

        if (changeCutReverseProc((daAlink_ANM)field_0x3198)) {
            return 1;
        }

        if (setShapeAngleToAtnActor(mProcVar4.field_0x3010)) {
            mProcVar2.field_0x300c = shape_angle.y;

            if (cLib_distanceAngleS(getShapeAngleYAtnActor(), shape_angle.y) < 0x800) {
                mProcVar4.field_0x3010 = 1;
            }
        } else {
            cLib_addCalcAngleS(&shape_angle.y, mProcVar2.field_0x300c, 2, 0x2000, 0x800);
        }

        current.angle.y = shape_angle.y;

        if (frameCtrl->getFrame() >= field_0x3478 && frameCtrl->getFrame() < field_0x347c) {
            if (!checkNoResetFlg0(FLG0_UNK_40)) {
                seStartSwordCut(Z2SE_AL_SWORD_SWING_S);
                mNormalSpeed = field_0x3480;
            }

            onResetFlg0(RFLG0_UNK_2);
        }

        if (mCutType == CUT_TYPE_NM_LEFT) {
            if (frameCtrl->getFrame() >= 24.0f) {
                field_0x2f92 = 0;
            } else if (frameCtrl->getFrame() >= 4.0f) {
                field_0x2f92 = 100;
            }
        } else if (mCutType == CUT_TYPE_NM_RIGHT) {
            if (frameCtrl->getFrame() >= 26.0f) {
                field_0x2f92 = 0;
            } else if (frameCtrl->getFrame() >= 4.0f) {
                field_0x2f92 = 100;
            }
        } else if (frameCtrl->getFrame() >= 24.0f) {
            field_0x2f92 = 0;
        }
    }

    return 1;
}

int daAlink_c::procCutFinishInit(int i_type) {
    static daAlink_cutParamTbl const cutParamTable[6] = {
        {
            daAlink_c::ANM_CUT_FINISH_LEFT,
            0x6F,
            daAlink_c::CUT_TYPE_FINISH_LEFT,
            1,
            100,
            100,
            5.0f,
        },
        {
            daAlink_c::ANM_CUT_FINISH_VERTICAL,
            0x6F,
            daAlink_c::CUT_TYPE_FINISH_VERTICAL,
            1,
            100,
            100,
            8.0f,
        },
        {
            daAlink_c::ANM_CUT_FINISH_STAB,
            0x6F,
            daAlink_c::CUT_TYPE_FINISH_STAB,
            14,
            100,
            100,
            10.0f,
        },
        {
            daAlink_c::ANM_CUT_MORTAL_DRAW_A,
            0x6F,
            daAlink_c::CUT_TYPE_MORTAL_DRAW_A,
            1,
            5,
            12,
            10.0f,
        },
        {
            daAlink_c::ANM_CUT_MORTAL_DRAW_B,
            0x6F,
            daAlink_c::CUT_TYPE_MORTAL_DRAW_B,
            1,
            7,
            14,
            10.0f,
        },
        {
            daAlink_c::ANM_CUT_FINISH_RIGHT,
            0x6F,
            daAlink_c::CUT_TYPE_FINISH_RIGHT,
            1,
            100,
            100,
            10.0f,
        },
    };

    const daAlinkHIO_cutFinish_c1* cutData;
    const daAlink_cutParamTbl* cutParams = &cutParamTable[i_type];
    BOOL front_roll = mProcID == PROC_FRONT_ROLL;

    commonProcInit(PROC_CUT_FINISH);
    setCutType(cutParams->m_cutType);
    field_0x3198 = cutParams->field_0x4;
    mProcVar2.field_0x300c = cutParams->field_0xa;
    mProcVar3.field_0x300e = cutParams->field_0xb;

    BOOL var_r30 = FALSE;
    if (i_type == 0) {
        cutData = &mpHIO->mCut.mCutFinishLeft.m;
    } else if (i_type == 1) {
        cutData = &mpHIO->mCut.mCutFinishVertical.m;
    } else if (i_type == 2) {
        cutData = &mpHIO->mCut.mCutFinishStab.m;
    } else if (i_type == 3) {
        cutData = &mpHIO->mCut.mCutFinishSweep.m;
        var_r30 = TRUE;
    } else if (i_type == 4) {
        cutData = &mpHIO->mCut.mCutFinishSlash.m;
        var_r30 = TRUE;
    } else {
        cutData = &mpHIO->mCut.mCutFinishRight.m;
    }

    f32 var_f31;
    if (front_roll) {
        var_f31 = cutData->mAttackStartFrame;
    } else if (mComboCutCount == 3) {
        var_f31 = cutData->mComboStartFrame;
    } else {
        var_f31 = cutData->mCutAnm.mStartFrame;
    }

    f32 morf;
    if (!checkZoraWearAbility() && checkNoResetFlg0(FLG0_UNDERWATER)) {
        morf = cutParams->m_morf;
    } else {
        morf = cutData->mCutAnm.mInterpolation;
    }

    setSingleAnime(cutParams->m_anmID, cutData->mCutAnm.mSpeed, var_f31, cutData->mCutAnm.mEndFrame,
                   morf);

    field_0x3478 = cutData->mAttackStartFrame;
    field_0x347c = cutData->mAttackEndFrame;
    field_0x3480 = cutData->mSpeed;

    if (checkHeavyStateOn(1, 1)) {
        field_0x3480 *= mHeavySpeedMultiplier;
    }

    current.angle.y = shape_angle.y;
    field_0x307e = mpHIO->mCut.m.mComboDuration;

    if (mComboCutCount == 3) {
        field_0x3484 = cutData->mComboCheckFrame;
        mProcVar0.field_0x3008 = cutData->mComboStopTime;
        setSwordAtParam(dCcG_At_Spl_UNK_0, 1, cutParams->m_atSe, 2,
                        mpHIO->mCut.m.mSwordLength, mpHIO->mCut.m.mSwordRadius);
        field_0x32d0 = Z2SE_AL_SWORD_SWING_S;
    } else {
        field_0x3484 = cutData->mCutAnm.mCancelFrame;
        mProcVar0.field_0x3008 = cutData->mStopTime;
        setSwordAtParam(dCcG_At_Spl_UNK_1, 3, cutParams->m_atSe, (var_r30 ? TRUE : FALSE) + 3,
                        mpHIO->mCut.m.mSwordLength, mpHIO->mCut.m.mSwordRadius);

        if (i_type == 2) {
            field_0x32d0 = Z2SE_AL_SWORD_THRUST;
        } else {
            field_0x32d0 = Z2SE_AL_SWORD_SWING_L;
        }
    }

    if (var_r30) {
        setSwordVoiceSe(Z2SE_AL_V_IAIGIRI);
    } else {
        setSwordComboVoice();
    }

    field_0x2f99 = 12;
    field_0x3588 = l_halfAtnWaitBaseAnime;

    if (mCutType == CUT_TYPE_MORTAL_DRAW_A || mCutType == CUT_TYPE_FINISH_STAB) {
        field_0x2f99 = 13;
    }

    mProcVar5.field_0x3012 = 0;
    dComIfGp_setPlayerStatus0(0, 0x8000);
    setCutWaterDropEffect();
    initCutAtnActorSearch();

    if (mAttention->LockonTarget(0) != NULL && var_r30) {
        field_0x280c.setData(mAttention->LockonTarget(0));
    } else {
        field_0x280c.clearData();
    }

    if (mCutType == CUT_TYPE_MORTAL_DRAW_A || mCutType == CUT_TYPE_MORTAL_DRAW_B) {
        field_0x2fd0 = 1;
    } else {
        field_0x2fd0 = 0;
    }

    return 1;
}

int daAlink_c::procCutFinish() {
    if (checkGroundSpecialMode()) {
        return 1;
    }

    daPy_frameCtrl_c* frameCtrl_p = mUnderFrameCtrl;

    setBodyAngleXReadyAnime(1);
    cLib_chaseF(&mNormalSpeed, 0.0f, mpHIO->mMove.m.mDeceleration);
    onEndResetFlg0(ERFLG0_UNK_8000000);
    setComboReserb();

    checkCutTurnCharge();
    checkCutAtnActorChange();

    if (mCutType == CUT_TYPE_MORTAL_DRAW_A || mCutType == CUT_TYPE_FINISH_STAB) {
        field_0x2f99 = 5;
    } else {
        field_0x2f99 = 4;
    }

    if (frameCtrl_p->getFrame() >= mProcVar3.field_0x300e) {
        onModeFlg(0x100);
    } else if (frameCtrl_p->getFrame() >= mProcVar2.field_0x300c) {
        offModeFlg(0x100);
    }

    if (checkAnmEnd(frameCtrl_p)) {
        resetCombo(1);

        if (mProcVar0.field_0x3008 > 0) {
            if (!(frameCtrl_p->getFrame() > field_0x3484) || !checkCutCancelNextMode(3)) {
                mProcVar0.field_0x3008--;
            }
        } else if (mDemo.getDemoMode() == 0x24 || mDemo.getDemoMode() == 0x54) {
            dComIfGp_evmng_cutEnd(mAlinkStaffId);
        } else {
            mNormalSpeed = 0.0f;
            field_0x2f98 = 2;
            checkNextAction(0);
        }
    } else if (!(frameCtrl_p->getFrame() > field_0x3484) || !checkCutCancelNextMode(3)) {
        if (frameCtrl_p->getFrame() > field_0x3484) {
            resetCombo(1);
        }

        if (changeCutReverseProc((daAlink_ANM)field_0x3198)) {
            return 1;
        }

        setShapeAngleToAtnActor(mProcVar4.field_0x3010);

        if (cLib_distanceAngleS(getShapeAngleYAtnActor(), shape_angle.y) < 0x800) {
            mProcVar4.field_0x3010 = 1;
        }

        current.angle.y = shape_angle.y;

        if (frameCtrl_p->getFrame() >= field_0x3478 && frameCtrl_p->getFrame() < field_0x347c) {
            if (!checkNoResetFlg0(FLG0_UNK_40)) {
                seStartSwordCut(field_0x32d0);
                mNormalSpeed = field_0x3480;
            }

            onResetFlg0(RFLG0_UNK_2);
        }
    }

    return 1;
}


int daAlink_c::procCutFinishJumpUpInit() {
    BOOL side_roll = mProcID == PROC_SIDE_ROLL;

    commonProcInit(PROC_CUT_FINISH_JUMP_UP);

    if (mEquipItem != 0x103) {
        deleteEquipItem(0, 0);
        setSwordModel();
    }

    setCutType(CUT_TYPE_TWIRL);
    speed.y = mpHIO->mCut.mCutFinishJumpUppercut.m.mSpeedV;
    mNormalSpeed = mpHIO->mCut.mCutFinishJumpUppercut.m.mSpeedH;

    if (checkNoResetFlg0(FLG0_UNDERWATER)) {
        mNormalSpeed *= mpHIO->mItem.mIronBoots.m.mWaterVelocityX;
        speed.y *= mpHIO->mItem.mIronBoots.m.mWaterVelocityY;
    } else if (checkHeavyStateOn(1, 1)) {
        speed.y *= 1.5f;
    }

    if (side_roll) {
        if (field_0x2f98 == 2) {
            current.angle.y = shape_angle.y + 0x4000;
        } else {
            current.angle.y = shape_angle.y - 0x4000;
        }
    } else {
        current.angle.y = shape_angle.y;
    }

    field_0x307e = mpHIO->mCut.m.mComboDuration;
    f32 var_f31 = mpHIO->mCut.mCutFinishJumpUppercut.m.mCutAnm.mStartFrame;

    setCylAtParam(getSwordAtType(), dCcG_At_Spl_UNK_1, 3, 1, 3,
                  mpHIO->mCut.mCutFinishJumpUppercut.m.mAttackRadius, 10.0f);
    field_0x32cc = Z2SE_AL_SWORD_SWING_L;

    setSingleAnime(ANM_CUT_TWIRL, mpHIO->mCut.mCutFinishJumpUppercut.m.mCutAnm.mSpeed, var_f31,
                   mpHIO->mCut.mCutFinishJumpUppercut.m.mCutAnm.mEndFrame,
                   mpHIO->mCut.mCutFinishJumpUppercut.m.mCutAnm.mInterpolation);
    setSwordVoiceSe(Z2SE_AL_V_SOTOMO_ATK);
    mProcVar5.field_0x3012 = 0;
    field_0x3478 = current.pos.y;
    dComIfGp_setPlayerStatus0(0, 0x8000);
    setCutWaterDropEffect();
    field_0x2fd0 = 1;
    return 1;
}

int daAlink_c::procCutFinishJumpUp() {
    daPy_frameCtrl_c* frameCtrl = mUnderFrameCtrl;

    if (frameCtrl->getFrame() >= 12.0f) {
        offModeFlg(0x100);
    }

    if (frameCtrl->getFrame() >= 3.0f) {
        field_0x2f92 = 100;
    }

    if (mLinkAcch.ChkGroundHit()) {
        return procCutFinishJumpUpLandInit();
    }

    if (mDemo.getDemoMode() != 0x52 &&
        current.pos.y < mLastJumpPos.y - mpHIO->mCut.mCutFinishJumpUppercut.m.mFallHeight)
    {
        return procFallInit(2, mpHIO->mCut.mCutFinishJumpUppercut.m.mFallInterpolation);
    }

    if (frameCtrl->getFrame() >= mpHIO->mCut.mCutFinishJumpUppercut.m.mAttackStartFrame &&
        frameCtrl->getFrame() < mpHIO->mCut.mCutFinishJumpUppercut.m.mAttackEndFrame)
    {
        if (!checkNoResetFlg0(FLG0_UNK_40)) {
            seStartSwordCut(field_0x32cc);
        }

        onResetFlg0(RFLG0_UNK_2);
    }

    return 1;
}

int daAlink_c::procCutFinishJumpUpLandInit() {
    commonProcInit(PROC_CUT_FINISH_JUMP_UP_LAND);
    checkCutLandDamage();

    setSingleAnimeParam(ANM_CUT_TWIRL_LAND, &mpHIO->mCut.mCutFinishJumpUppercut.m.mLandAnm);
    mNormalSpeed = 0.0f;
    setCutType(CUT_TYPE_TWIRL);

    field_0x2f9d = 4;
    setFootEffectProcType(2);
    onResetFlg1(RFLG1_UNK_30);

    if (mComboCutCount == 3) {
        field_0x3484 = mpHIO->mCut.mCutFinishJumpUppercut.m.mComboCheckFrame;
        mProcVar0.field_0x3008 = mpHIO->mCut.mCutFinishJumpUppercut.m.mComboStopTime;
    } else {
        field_0x3484 = mpHIO->mCut.mCutFinishJumpUppercut.m.mLandAnm.mCancelFrame;
        mProcVar0.field_0x3008 = mpHIO->mCut.mCutFinishJumpUppercut.m.mStopTime;
    }

    mProcVar5.field_0x3012 = 0;
    field_0x3588 = l_halfAtnWaitBaseAnime;
    initCutAtnActorSearch();
    return 1;
}

int daAlink_c::procCutFinishJumpUpLand() {
    if (checkGroundSpecialMode()) {
        return 1;
    }

    daPy_frameCtrl_c* frameCtrl = mUnderFrameCtrl;
    onEndResetFlg0(ERFLG0_UNK_8000000);

    setComboReserb();
    checkCutTurnCharge();
    field_0x2f99 = 4;
    checkCutAtnActorChange();

    if (frameCtrl->checkAnmEnd()) {
        if (mDemo.getDemoMode() == 0x52) {
            dComIfGp_evmng_cutEnd(mAlinkStaffId);
        } else if (mProcVar0.field_0x3008 > 0) {
            if (!(frameCtrl->getFrame() > field_0x3484) || !checkCutCancelNextMode(3)) {
                mProcVar0.field_0x3008--;
            }
        } else {
            mNormalSpeed = 0.0f;
            field_0x2f98 = 2;
            checkNextAction(0);
        }
    } else if (!(frameCtrl->getFrame() > field_0x3484) || !checkCutCancelNextMode(3)) {
        if (frameCtrl->getFrame() > field_0x3484) {
            resetCombo(1);
        }

        setShapeAngleToAtnActor(mProcVar4.field_0x3010);

        if (cLib_distanceAngleS(getShapeAngleYAtnActor(), shape_angle.y) < 0x800) {
            mProcVar4.field_0x3010 = 1;
        }

        current.angle.y = shape_angle.y;

        if (frameCtrl->getFrame() >= 9.0f) {
            field_0x2f92 = 0;
        }
    }

    return 1;
}

int daAlink_c::procCutReverseInit(daAlink_c::daAlink_ANM i_anmID) {
    dKy_Sound_set(current.pos, 100, fopAcM_GetID(this), 5);

    if (i_anmID == ANM_CUT_JUMP) {
        return 1;
    }

    commonProcInit(PROC_CUT_REVERSE);

    if (i_anmID == ANM_SHIELD_ATTACK_RECOIL) {
        setSingleAnimeParam(i_anmID, &mpHIO->mGuard.m.mRecoilAnm);
        mProcVar2.field_0x300c = 1;
        field_0x3478 = mpHIO->mGuard.m.mRecoilAnm.mCancelFrame;
        field_0x2f98 = 2;
        setUpperGuardAnime(-1.0f);
    } else {
        setSingleAnimeParam(i_anmID, &mpHIO->mCut.m.mRecoilAnm);
        mProcVar2.field_0x300c = 0;
        field_0x3478 = mpHIO->mCut.m.mRecoilAnm.mCancelFrame;
        field_0x2f98 = 4;
    }

    mNormalSpeed = mpHIO->mCut.m.mRecoilSpeed;
    current.angle.y = shape_angle.y - -0x8000;
    field_0x307e = 0;
    mProcVar5.field_0x3012 = 0;

    int temp_r3;
    if (mCutType == CUT_TYPE_MORTAL_DRAW_A || mCutType == CUT_TYPE_MORTAL_DRAW_B ||
        mCutType == CUT_TYPE_JUMP)
    {
        temp_r3 = 5;
    } else {
        temp_r3 = 3;
    }

    f32 tmp_0 = 0.0f;
    dComIfGp_getVibration().StartShock(temp_r3, 31, cXyz(tmp_0, 1.0f, tmp_0));
    return 1;
}

int daAlink_c::procCutReverse() {
    if (checkGroundSpecialMode()) {
        return 1;
    }

    daPy_frameCtrl_c* frameCtrl = mUnderFrameCtrl;
    cLib_chaseF(&mNormalSpeed, 0.0f, mpHIO->mCut.m.mRecoilDeceleration);

    if (mProcVar2.field_0x300c == 0) {
        checkCutTurnCharge();
        onEndResetFlg0(ERFLG0_UNK_8000000);
    }

    if (mProcVar2.field_0x300c != 0) {
        setUpperGuardAnime(-1.0f);
    }

    if (frameCtrl->checkAnmEnd()) {
        current.angle.y = shape_angle.y;
        checkNextAction(0);
    } else if (frameCtrl->getFrame() > field_0x3478) {
        current.angle.y = shape_angle.y;

        if (!checkNextAction(1)) {
            current.angle.y = shape_angle.y - -0x8000;
        }
    }

    return 1;
}

int daAlink_c::procCutJumpInit(int i_airCut) {
    commonProcInit(PROC_CUT_JUMP);
    setSingleAnimeParam(ANM_CUT_JUMP, &mpHIO->mCut.mCutJump.m.mCutAnm);

    if (i_airCut) {
        mNormalSpeed = mpHIO->mCut.mCutJump.m.mAirJumpSpeedH;
        speed.y = mpHIO->mCut.mCutJump.m.mAirJumpSpeedV;
    } else {
        mNormalSpeed = mpHIO->mCut.mCutJump.m.mBaseJumpSpeedH;
        speed.y = mpHIO->mCut.mCutJump.m.mBaseJumpSpeedV;
    }

    setCutJumpSpeed(i_airCut);
    current.angle.y = shape_angle.y;
    setSwordAtParam(dCcG_At_Spl_UNK_1, 3, 1, 3, mpHIO->mCut.m.mSwordLength,
                    mpHIO->mCut.m.mSwordRadius);
    setCutType(CUT_TYPE_JUMP);
    setSwordComboVoice();
    dComIfGp_setPlayerStatus0(0, 0x8000);
    field_0x3198 = 2;
    return 1;
}

int daAlink_c::procCutJump() {
    if (current.angle.y == shape_angle.y && changeCutReverseProc(ANM_CUT_JUMP)) {
        current.angle.y += 0x8000;
        mNormalSpeed = 27.0f;

        f32 tmp_0 = 0.0f;
        dComIfGp_getVibration().StartShock(5, 31, cXyz(tmp_0, 1.0f, tmp_0));
    } else if (current.angle.y != shape_angle.y) {
        cLib_chaseF(&mNormalSpeed, 5.0f, 1.0f);
    }

    if (checkCutTurnInput()) {
        field_0x3198 = getCutTurnDirection();
    }

    if (mLinkAcch.ChkGroundHit()) {
        procCutJumpLandInit(field_0x3198);
    } else if (mUnderFrameCtrl[0].getFrame() >= mpHIO->mCut.mCutJump.m.mStartAttackFrame) {
        if (!checkNoResetFlg0(FLG0_UNK_40)) {
            seStartSwordCut(Z2SE_AL_SWORD_SWING_L);
        }

        onResetFlg0(RFLG0_UNK_2);
    }

    return 1;
}

int daAlink_c::procCutJumpLandInit(int param_0) {
    commonProcInit(PROC_CUT_JUMP_LAND);
    checkCutLandDamage();
    setSingleAnimeParam(ANM_CUT_JUMP_LAND, &mpHIO->mCut.mCutJump.m.mLandAnm);

    if (checkNoResetFlg0(FLG0_UNDERWATER)) {
        field_0x2060->initOldFrameMorf(5.0f * mpHIO->mCut.mCutJump.m.mLandAnm.mInterpolation,
                                       0, 0x23);
    }

    if (!checkNoResetFlg0(FLG0_UNK_40)) {
        seStartSwordCut(Z2SE_AL_SWORD_SWING_L);
        m_swordBlur.initBlur(0.0f, 0, &mSwordTopPos, &field_0x3498, &field_0x34a4);
        setSwordAtParam(dCcG_At_Spl_UNK_1, 3, 1, 3, mpHIO->mCut.m.mSwordLength,
                        mpHIO->mCut.m.mSwordRadius);
    }

    onResetFlg0(RFLG0_UNK_2);
    mNormalSpeed = 0.0f;
    setCutType(CUT_TYPE_JUMP);
    field_0x2f9d = 4;
    setFootEffectProcType(2);

    onResetFlg1(RFLG1_UNK_30);
    field_0x3588 = l_halfAtnWaitBaseAnime;
    field_0x3198 = param_0;
    setStepLandVibration();
    return 1;
}

int daAlink_c::procCutJumpLand() {
    if (checkGroundSpecialMode()) {
        return 1;
    }

    daPy_frameCtrl_c* frameCtrl = mUnderFrameCtrl;

    if (changeCutReverseProc(ANM_CUT_RECOIL_B)) {
        return 1;
    }

    field_0x2f99 = 4;
    onEndResetFlg0(ERFLG0_UNK_8000000);

    if (field_0x3198 != 2 && checkCutTurnInput()) {
        field_0x3198 = getCutTurnDirection();
    }

    if (frameCtrl->checkAnmEnd()) {
        checkNextAction(0);
    } else if (frameCtrl->getFrame() > mpHIO->mCut.mCutJump.m.mJumpSpinDelay && field_0x3198 != 2)
    {
        procCutTurnInit(1, field_0x3198);
    } else {
        if (frameCtrl->getFrame() > mpHIO->mCut.mCutJump.m.mLandAnm.mCancelFrame) {
            onModeFlg(4);

            if (checkNextAction(1)) {
                return 1;
            }
        }

        if (frameCtrl->getFrame() < mpHIO->mCut.mCutJump.m.mEndAttackFrame) {
            onResetFlg0(RFLG0_UNK_2);
        }
    }

    return 1;
}

int daAlink_c::procCutTurnInit(int param_0, int param_1) {
    const daAlinkHIO_cutTurn_c1* cutData = &mpHIO->mCut.mCutTurn.m;
    const daAlinkHIO_anm_c* anm_data;
    daAlink_ANM anmID;

    commonProcInit(PROC_CUT_TURN);

    if (param_1 == 2) {
        param_1 = getCutTurnDirection();
    }

    f32 var_f31;
    if (param_1 == 1) {
        anmID = ANM_CUT_TURN_RIGHT;
        anm_data = &cutData->mRightTurnAnm;

        if (param_0) {
            var_f31 = cutData->mRightTurnInputStartFrame;
        } else {
            var_f31 = anm_data->mStartFrame;
        }

        field_0x3484 = cutData->mRightAttackStartFrame;
        field_0x3488 = cutData->mRightAttackEndFrame;
        mProcVar4.field_0x3010 = 1;
        mProcVar1.field_0x300a = 6;
    } else {
        anmID = ANM_CUT_TURN_LEFT;
        anm_data = &cutData->mLeftTurnAnm;

        if (param_0) {
            var_f31 = cutData->mLeftTurnInputStartFrame;
        } else {
            var_f31 = anm_data->mStartFrame;
        }

        field_0x3484 = cutData->mLeftAttackStartFrame;
        field_0x3488 = cutData->mLeftAttackEndFrame;
        mProcVar4.field_0x3010 = 0;
        mProcVar1.field_0x300a = 8;
    }

    field_0x3480 = anm_data->mCancelFrame;
    resetCombo(1);

    f32 morf;
    if (!checkZoraWearAbility() && checkNoResetFlg0(FLG0_UNDERWATER)) {
        morf = 10.0f;
    } else {
        morf = anm_data->mInterpolation;
    }

    setSingleAnime(anmID, anm_data->mSpeed, var_f31, anm_data->mEndFrame, morf);

    int sp08;
    if (checkCutLargeTurnState()) {
        sp08 = 4;
        field_0x32cc = Z2SE_AL_KAITEN_L_SLASH;
        setSwordVoiceSe(Z2SE_AL_V_KAITENGIRI_L);
        field_0x3478 = cutData->mLargeAttackRadius;
        field_0x348c = cutData->mLargeAttackAccel;

        if (anmID == ANM_CUT_TURN_RIGHT) {
            setCutType(CUT_TYPE_LARGE_TURN_RIGHT);
        } else {
            setCutType(CUT_TYPE_LARGE_TURN_LEFT);
        }
    } else {
        sp08 = 3;
        field_0x32cc = Z2SE_AL_KAITENGIRI;
        setSwordVoiceSe(Z2SE_AL_V_KAITEN);

        if (anmID == ANM_CUT_TURN_RIGHT) {
            setCutType(CUT_TYPE_TURN_RIGHT);
        } else {
            setCutType(CUT_TYPE_TURN_LEFT);
        }

        if (checkNoResetFlg3(FLG3_UNK_100000)) {
            field_0x3478 = cutData->mLightAttackRadius;
            field_0x348c = cutData->mLightAttackRadiusAccel;
        } else {
            field_0x3478 = cutData->mAttackRadius;
            field_0x348c = cutData->mAttackRadiusAccel;
        }
    }

    field_0x347c = 130.0f;
    field_0x3588 = l_halfAtnWaitBaseAnime;
    mProcVar0.field_0x3008 = cutData->mStopTime;
    current.angle.y = shape_angle.y;
    field_0x3180 = 0;
    dComIfGp_setPlayerStatus0(0, 0x8000);
    mNormalSpeed = 0.0f;
    mProcVar5.field_0x3012 = 0;
    setCutWaterDropEffect();
    initCutTurnAt(field_0x347c, sp08);
    return 1;
}

int daAlink_c::procCutTurn() {
    if (checkGroundSpecialMode()) {
        return 1;
    }

    daPy_frameCtrl_c* frameCtrl = mUnderFrameCtrl;
    field_0x2f99 = 4;
    cLib_chaseF(&mNormalSpeed, 0.0f, mpHIO->mMove.m.mDeceleration);
    onEndResetFlg0(ERFLG0_UNK_8000000);

    if (mComboCutCount != 0) {
        field_0x307e = mpHIO->mCut.m.mComboDuration;
    }

    if (frameCtrl->getFrame() >= 44.0f) {
        field_0x2f92 = 0;
    }

    if (frameCtrl->checkAnmEnd()) {
        if (mProcVar0.field_0x3008 > 0) {
            if (!(frameCtrl->getFrame() > field_0x3480) || !checkCutCancelNextMode(3)) {
                mProcVar0.field_0x3008--;
            }
        } else if (mDemo.getDemoMode() == 0x24) {
            dComIfGp_evmng_cutEnd(mAlinkStaffId);
        } else {
            field_0x2f98 = 3;
            checkNextAction(0);
        }
    } else if (!(frameCtrl->getFrame() > field_0x3480) || !checkCutCancelNextMode(3)) {
        if (frameCtrl->getFrame() >= field_0x3484 && frameCtrl->getFrame() < field_0x3488) {
            if (!checkNoResetFlg0(FLG0_UNK_40)) {
                seStartSwordCut(field_0x32cc);
                mNormalSpeed = mpHIO->mCut.mCutTurn.m.mSpeed;

                if (checkHeavyStateOn(1, 1)) {
                    mNormalSpeed *= mHeavySpeedMultiplier;
                }
            }

            onResetFlg0(RFLG0_UNK_2);
            cLib_chaseF(&field_0x347c, field_0x3478, field_0x348c);
            field_0xFB8.SetR(field_0x347c);
        }
    }

    return 1;
}

int daAlink_c::procCutTurnChargeInit() {
    if (mComboCutCount == 0 && checkDashAnime()) {
        setUpperAnimeBase(0x83);
        return 1;
    }

    commonProcInit(PROC_CUT_TURN_CHARGE);
    setSingleAnimeParam(ANM_CUT_TURN_CHARGE_START, &mpHIO->mCut.mCutTurn.m.mChargeAnm);
    mNormalSpeed = 0.0f;
    current.angle.y = shape_angle.y;
    mProcVar2.field_0x300c = mpHIO->mCut.m.mNormalSwingDuration;
    return 1;
}

int daAlink_c::procCutTurnCharge() {
    if (checkGroundSpecialMode()) {
        return 1;
    }

    setSwordChargeVoiceSe();
    field_0x307e = mpHIO->mCut.m.mComboDuration;

    if (!swordButton() && mDemo.getDemoMode() != 0x5B) {
        if (mProcVar2.field_0x300c != 0) {
            checkCutAction();
        } else {
            cancelCutCharge();
        }
    } else if (mUnderFrameCtrl[0].checkAnmEnd()) {
        procCutTurnMoveInit(0);
    } else if (mUnderFrameCtrl[0].getFrame() >= 6.0f) {
        field_0x2f92 = 100;
    }

    return 1;
}

int daAlink_c::procCutTurnMoveInit(int param_0) {
    commonProcInit(PROC_CUT_TURN_MOVE);

    if (checkZeroSpeedF()) {
        onModeFlg(1);
    }

    if (param_0) {
        setUpperAnimeBaseSpeed(getMainBckData(ANM_CUT_JUMP_LARGE_CHARGE)->m_underID,
                               0.0f, -1.0f);

        f32 frame = mUnderFrameCtrl[0].getFrame();
        mUpperFrameCtrl[2].setFrame(frame);
        getNowAnmPackUpper(UPPER_2)->setFrame(frame);
        setFacePriAnime(ANM_CUT_JUMP_LARGE_CHARGE);
    }

    setSingleAnimeBaseSpeed(ANM_CUT_TURN_CHARGE, 0.0f,
                            mpHIO->mCut.mCutTurn.m.mMoveFBInterpolation);
    field_0x2f98 = 0;
    field_0x594 = mpHIO->mCut.mCutTurn.m.mMaxSpeed;

    if (!checkWoodSwordEquip()) {
        simpleAnmPlay(m_nSwordBtk);
    } else {
        mProcVar0.field_0x3008 = 1;
    }

    mProcVar2.field_0x300c = param_0;
    onNoResetFlg1(FLG1_UNK_10000000);
    dComIfGp_setPlayerStatus0(0, 0x40000000);
    return 1;
}

int daAlink_c::procCutTurnMove() {
    setShapeAngleToAtnActor(0);

    if (!checkWoodSwordEquip()) {
        simpleAnmPlay(m_nSwordBtk);
    } else {
        mProcVar0.field_0x3008++;
        if (mProcVar0.field_0x3008 >= 14.0f) {
            mProcVar0.field_0x3008 = 100;
        }
    }

    field_0x2fe4 = shape_angle.y;
    onNoResetFlg1(FLG1_UNK_10000000);

    if (checkGroundSpecialMode()) {
        return 1;
    }

    BOOL var_r4 = m_nSwordBtk->getFrame() >= 14.0f || mProcVar0.field_0x3008 >= 14.0f;

    if (mProcVar2.field_0x300c != 0) {
        if (checkDownAttackState() && !var_r4) {
            setDoStatusEmphasys(0x30);
        } else if (checkCutHeadState() && !var_r4) {
            setDoStatusEmphasys(0x77);
        } else {
            setDoStatus(0x86);
        }
    } else {
        field_0x307e = mpHIO->mCut.m.mComboDuration;
    }

    if (mDemo.getDemoMode() == 0x5B || mDemo.getDemoMode() == 0x5C) {
        if (var_r4) {
            dComIfGp_evmng_cutEnd(mAlinkStaffId);
        }
    } else if ((mProcVar2.field_0x300c == 0 && swordButton()) || (mProcVar2.field_0x300c != 0 && doButton())) {
        f32 var_f30 = 0.0f;
        onResetFlg0(RFLG0_UNK_10);

        if (checkInputOnR()) {
            int direction = getDirectionFromAngle(field_0x2fe2 - field_0x2fe4);
            daAlink_ANM anm = ANM_MAX;

            cLib_addCalcAngleS(&current.angle.y, field_0x2fe2, 4, 12000, 0x2000);

            f32 var_f31;
            if (field_0x2f98 == 0 || field_0x2f98 == 1) {
                if (direction == DIR_LEFT || direction == DIR_RIGHT) {
                    anm = ANM_CUT_TURN_CHARGE_MOVE;
                    var_f31 = mpHIO->mCut.mCutTurn.m.mMoveLRInterpolation;
                    mNormalSpeed *= 0.5f;
                } else if (field_0x2f98 != direction) {
                    mNormalSpeed *= -0.5f;
                    current.angle.y = field_0x2fe2;
                }
            } else if (direction == DIR_FORWARD || direction == DIR_BACKWARD) {
                anm = ANM_CUT_TURN_CHARGE;
                var_f31 = mpHIO->mCut.mCutTurn.m.mMoveFBInterpolation;
                mNormalSpeed *= 0.5f;
            } else if (field_0x2f98 != direction) {
                mNormalSpeed *= -0.5f;
                current.angle.y = field_0x2fe2;
            }

            if (field_0x2f98 == direction) {
                var_f30 = mpHIO->mCut.mCutTurn.m.mChargeMoveAccel * field_0x33ac;
            } else {
                field_0x2f98 = direction;
            }

            if (anm != ANM_MAX) {
                setSingleAnimeBaseSpeed(anm, 0.0f, var_f31);
            }
        }

        setNormalSpeedF(var_f30, mpHIO->mCut.mCutTurn.m.mChargeMoveDecel);

        f32 var_f31_2;
        if (field_0x2f98 == DIR_FORWARD) {
            var_f31_2 = mpHIO->mCut.mCutTurn.m.mMoveFBAnmSpeed;
        } else if (field_0x2f98 == DIR_BACKWARD) {
            var_f31_2 = -mpHIO->mCut.mCutTurn.m.mMoveFBAnmSpeed;
        } else if (field_0x2f98 == DIR_RIGHT) {
            var_f31_2 = -mpHIO->mCut.mCutTurn.m.mMoveLRAnmSpeed;
        } else {
            var_f31_2 = mpHIO->mCut.mCutTurn.m.mMoveLRAnmSpeed;
        }

        daPy_frameCtrl_c* frameCtrl_p = mUnderFrameCtrl;

        f32 temp_f0 = mNormalSpeed / field_0x594;
        setWaterInAnmRate(frameCtrl_p, temp_f0 * var_f31_2);

        if (var_f31_2 >= 0.0f) {
            frameCtrl_p->setLoop(frameCtrl_p->getStart());
        } else {
            frameCtrl_p->setLoop(frameCtrl_p->getEnd());
        }

        initBasAnime();

        if (checkZeroSpeedF()) {
            onModeFlg(1);
            mSpeedModifier = 0.0f;
        } else {
            offModeFlg(1);
            mSpeedModifier = 1.0f;
        }
    } else if (var_r4) {
        if (mProcVar2.field_0x300c != 0) {
            procCutLargeJumpInit();
        } else {
            procCutTurnInit(0, 1);
        }
    } else if (mProcVar2.field_0x300c != 0) {
        checkDoCutAction();
    } else {
        cancelCutCharge();
    }

    return 1;
}


int daAlink_c::procCutDownInit() {
    if (mDemo.getDemoMode() == 0x51 && (mProcID == PROC_CUT_DOWN || mProcID == PROC_CUT_DOWN_LAND))
    {
        return 1;
    }

    commonProcInit(PROC_CUT_DOWN);
    resetCombo(1);

    cXyz sp2C;
    mProcVar3.field_0x300e = 0;

    if (mTargetedActor != NULL) {
        sp2C = static_cast<fopEn_enemy_c*>(mTargetedActor)->getDownPos();
        field_0x280c.setData(mTargetedActor);

        if (fopAcM_GetName(mTargetedActor) == PROC_B_GND) {
            mProcVar3.field_0x300e = 1;
        }
    } else {
        sp2C.set(current.pos.x + cM_ssin(shape_angle.y) * 100.0f, current.pos.y,
                 current.pos.z + cM_scos(shape_angle.y) * 100.0f);
        field_0x280c.clearData();
    }

    if (mProcVar3.field_0x300e != 0) {
        mNormalSpeed = 0.0f;
        speed.y = 0.0f;
        offModeFlg(2);
    } else {
        setSingleAnimeParam(ANM_CUT_FINISHING_BLOW_JUMP, &mpHIO->mCut.mCutDown.m.mJumpAnm);
        sp2C.y += 50.0f;

        if (fopAcM_gc_c::gndCheck(&sp2C)) {
            sp2C.y = fopAcM_gc_c::getGroundY();
        } else {
            sp2C.y -= 50.0f;
        }

        shape_angle.y = cLib_targetAngleY(&current.pos, &sp2C);
        current.angle.y = shape_angle.y;

        sp2C.x -= cM_ssin(shape_angle.y) * 95.0f;
        sp2C.z -= cM_scos(shape_angle.y) * 95.0f;

        f32 var_f31 = current.pos.absXZ(sp2C);
        if (var_f31 > 800.0f) {
            var_f31 = 800.0f;
        }

        speed.y = mpHIO->mCut.mCutDown.m.mRecoverSpeedH;

        if (checkNoResetFlg0(FLG0_UNDERWATER)) {
            mNormalSpeed *= mpHIO->mItem.mIronBoots.m.mWaterVelocityX;
            speed.y *= mpHIO->mItem.mIronBoots.m.mWaterVelocityY;
        } else if (checkHeavyStateOn(1, 1)) {
            speed.y *= 1.5f;
        }

        f32 var_f4 = speed.y + gravity;
        f32 var_f5 = 1.0f / gravity;

        f32 temp_f1 = sp2C.y - (current.pos.y - var_f5 * (var_f4 * var_f4) * 0.5f);
        if (temp_f1 > 0.0f) {
            temp_f1 = 0.0f;
        }

        temp_f1 = JMAFastSqrt(temp_f1 * 2.0f * var_f5);
        mNormalSpeed = var_f31 / (temp_f1 - var_f4 * var_f5);
        mProcVar2.field_0x300c = 0;
    }

    setSwordVoiceSe(Z2SE_AL_V_TODOME_JUMP);
    setCutType(CUT_TYPE_DOWN);
    dComIfGp_setPlayerStatus1(0, 0x400000);
    return 1;
}


int daAlink_c::procCutDown() {
    if (mProcVar3.field_0x300e != 0) {
        return 1;
    }

    if (mLinkAcch.ChkGroundHit() && speed.y <= 0.0f) {
        procCutDownLandInit((fopEn_enemy_c*)field_0x280c.getActor());
    } else if (mUnderFrameCtrl[0].checkAnmEnd() && mProcVar2.field_0x300c == 0) {
        mProcVar2.field_0x300c = 1;
        setSingleAnimeParam(ANM_CUT_FINISHING_BLOW_FALL, &mpHIO->mCut.mCutDown.m.mFallAnm);
    }

    return 1;
}

int daAlink_c::procCutDownLandInit(fopEn_enemy_c* i_enemy) {
    commonProcInit(PROC_CUT_DOWN_LAND);
    checkCutLandDamage();
    setSingleAnimeParam(ANM_CUT_FINISHING_BLOW_STAB, &mpHIO->mCut.mCutDown.m.mLandAnm);

    u32 var_r30;
    if (mDemo.getDemoMode() == 0x51 || (i_enemy != NULL && i_enemy->checkDownFlg() &&
                                        mSwordTopPos.abs2XZ(i_enemy->getDownPos()) < 10000.0f &&
                                        fabsf(i_enemy->current.pos.y - current.pos.y) < 100.0f))
    {
        mProcVar3.field_0x300e = 1;
        if (i_enemy != NULL) {
            i_enemy->onCutDownHitFlg();

            csXyz angle(0x4000, shape_angle.y, 0);
            dComIfGp_setHitMark(3, NULL, &i_enemy->getDownPos(), &angle, NULL, 0);
        }

        var_r30 = 0x20;
        mProcVar0.field_0x3008 = mpHIO->mCut.mCutDown.m.mSuccessStopTime;
        dComIfGp_getVibration().StartShock(4, 31, cXyz(0.0f, 1.0f, 0.0f));
    } else {
        var_r30 = mPolySound;
        mProcVar3.field_0x300e = 0;
        mProcVar0.field_0x3008 = mpHIO->mCut.mCutDown.m.mFailStopTime;
    }

    mZ2Link.startCollisionSE(Z2SE_HIT_SWORD_STAB, var_r30);
    mProcVar2.field_0x300c = 0;
    mNormalSpeed = 0.0f;

    offResetFlg0(RFLG0_UNK_8000000);
    field_0x2f9d = 4;

    setFootEffectProcType(2);
    onResetFlg1(RFLG1_UNK_30);
    mProcVar4.field_0x3010 = 0;

    dComIfGp_setPlayerStatus1(0, 0x400000);
    field_0x3588 = l_halfAtnWaitBaseAnime;
    field_0x2f99 = 4;
    return 1;
}


int daAlink_c::procCutDownLand() {
    if (checkGroundSpecialMode()) {
        return 1;
    }

    daPy_frameCtrl_c* frameCtrl_p = mUnderFrameCtrl;
    field_0x2f99 = 4;

    if (mProcVar2.field_0x300c != 0) {
        onEndResetFlg0(ERFLG0_UNK_8000000);
    }

    if (checkAnmEnd(frameCtrl_p)) {
        if (mProcVar2.field_0x300c == 0) {
            if (mProcVar0.field_0x3008 != 0) {
                mProcVar0.field_0x3008--;
            } else if (mProcVar3.field_0x300e != 0) {
                if (mDemo.getDemoMode() == 0x51) {
                    dComIfGp_evmng_cutEnd(mAlinkStaffId);
                } else {
                    procBackJumpInit(1);
                }
            } else {
                setSingleAnimeParam(ANM_CUT_FINISHING_BLOW_MISS,
                                    &mpHIO->mCut.mCutDown.m.mMissAnm);
                mProcVar2.field_0x300c = 1;
            }
        } else {
            checkNextAction(0);
        }
    } else if (mProcVar2.field_0x300c != 0) {
        if (frameCtrl_p->getFrame() >= 37.0f) {
            mProcVar4.field_0x3010 = 0;
        } else if (frameCtrl_p->getFrame() >= 30.0f) {
            mProcVar4.field_0x3010 = 254;
        }

        if (frameCtrl_p->checkPass(54.0f)) {
            seStartSwordCut(Z2SE_SWORD_STICK_PULLOUT);
        }

        if (frameCtrl_p->getFrame() > mpHIO->mCut.mCutDown.m.mMissAnm.mCancelFrame) {
            onModeFlg(4);
            checkNextAction(1);
        }
    }

    return 1;
}


int daAlink_c::procCutHeadInit() {
    fopEn_enemy_c* enemy_p = (fopEn_enemy_c*)mTargetedActor;

    if (mDemo.getDemoMode() == 0x53 && (mProcID == PROC_CUT_HEAD || mProcID == PROC_CUT_HEAD_LAND))
    {
        return 1;
    }

    commonProcInit(PROC_CUT_HEAD);
    resetCombo(1);
    setSingleAnimeParam(ANM_CUT_HEAD_JUMP, &mpHIO->mCut.mCutHead.m.mJumpAnm);
    setSpecialGravity(mpHIO->mCut.mCutHead.m.mGravity, maxFallSpeed, 0);

    cXyz sp2C;
    if (enemy_p != NULL) {
        sp2C = enemy_p->getHeadLockPos() - current.pos;
        field_0x280c.setData(enemy_p);
    } else {
        sp2C.set(cM_ssin(shape_angle.y) * 200.0f, 200.0f, cM_scos(shape_angle.y) * 200.0f);
        field_0x280c.clearData();
    }

    f32 fvar9 = cLib_minMaxLimit<f32>(sp2C.y, 30.0f, mpHIO->mCut.mCutHead.m.mMaxHeight);
    f32 fvar10 = JMAFastSqrt((fvar9 * -2.0f) / gravity);
    speed.y = fvar10 * -gravity;

    f32 fvar5 = sp2C.absXZ();
    if (fvar5 > mpHIO->mCut.mCutHead.m.mMaxDistance) {
        fvar5 = mpHIO->mCut.mCutHead.m.mMaxDistance;
    }

    field_0x3478 = (fvar5 * 2.0f) / (fvar10 * 3.0f * fvar10);
    mNormalSpeed = field_0x3478 * fvar10 * 2.0f;
    mProcVar3.field_0x300e = 0;

    current.angle.y = sp2C.atan2sX_Z();
    shape_angle.y = current.angle.y;

    setSwordVoiceSe(Z2SE_AL_V_KABUTO_JUMP);
    setSwordAtParam(dCcG_At_Spl_UNK_1, 3, 1, 3, mpHIO->mCut.mCutHead.m.mSwordLength,
                    mpHIO->mCut.mCutHead.m.mSwordRadius);
    setCutType(5);

    return 1;
}

int daAlink_c::procCutHead() {
    if (mProcVar3.field_0x300e == 0) {
        cLib_chaseF(&mNormalSpeed, 0.0f, field_0x3478);

        if (speed.y <= 0.0f) {
            setSingleAnimeParam(ANM_CUT_HEAD, &mpHIO->mCut.mCutHead.m.mCutAnm);
            f32 tmp_0 = 0.0f;
            mNormalSpeed = tmp_0;
            speed.y = tmp_0;
            setSpecialGravity(tmp_0, maxFallSpeed, 0);
            mProcVar3.field_0x300e = 1;
            setCutWaterDropEffect();
            setSwordVoiceSe(Z2SE_AL_V_KABUTO_ATK);
        }
    } else {
        daPy_frameCtrl_c* frameCtrl = mUnderFrameCtrl;
        mNormalSpeed *= 0.99f;

        if (frameCtrl->checkPass(7.0f)) {
            speed.y = mpHIO->mCut.mCutHead.m.mAddSpeedV;
            mNormalSpeed = mpHIO->mCut.mCutHead.m.mAddSpeedH;
            initGravity();
        }

        if (frameCtrl->getFrame() >= mpHIO->mCut.mCutHead.m.mAttackStartFrame &&
            frameCtrl->getFrame() < mpHIO->mCut.mCutHead.m.mAttackEndFrame)
        {
            if (!checkNoResetFlg0(FLG0_UNK_40)) {
                seStartSwordCut(Z2SE_AL_SWORD_SWING_L);
            }

            onResetFlg0(RFLG0_UNK_2);
        }

        if (mLinkAcch.ChkGroundHit()) {
            procCutHeadLandInit();
        }
    }

    return 1;
}

int daAlink_c::procCutHeadLandInit() {
    commonProcInit(PROC_CUT_HEAD_LAND);
    checkCutLandDamage();
    setSingleAnimeParam(ANM_CUT_HEAD_LAND, &mpHIO->mCut.mCutHead.m.mLandAnm);
    mNormalSpeed = 0.0f;
    return 1;
}

int daAlink_c::procCutHeadLand() {
    onEndResetFlg0(ERFLG0_UNK_8000000);
    daPy_frameCtrl_c* frameCtrl = mUnderFrameCtrl;

    if (frameCtrl->checkAnmEnd()) {
        if (mDemo.getDemoMode() == 0x53) {
            dComIfGp_evmng_cutEnd(mAlinkStaffId);
        } else {
            checkNextAction(0);
        }
    } else if (frameCtrl->getFrame() > mpHIO->mCut.mCutHead.m.mLandAnm.mCancelFrame) {
        checkNextAction(1);
    }

    return 1;
}

int daAlink_c::procCutLargeJumpChargeInit() {
    commonProcInit(PROC_CUT_LARGE_JUMP_CHARGE);
    setSingleAnimeParam(ANM_CUT_JUMP_LARGE_CHARGE, &mpHIO->mCut.mCutLargeJump.m.mChargeAnm);
    mNormalSpeed = 0.0f;
    current.angle.y = shape_angle.y;
    mProcVar2.field_0x300c = mpHIO->mCut.m.mNormalSwingDuration;
    return 1;
}

int daAlink_c::procCutLargeJumpCharge() {
    if (checkGroundSpecialMode()) {
        return 1;
    }

    if (checkDownAttackState()) {
        setDoStatusEmphasys(0x30);
    } else if (checkCutHeadState()) {
        setDoStatusEmphasys(0x77);
    } else {
        setDoStatus(0x86);
    }

    setSwordChargeVoiceSe();

    if (!doButton() && mDemo.getDemoMode() != 0x5C) {
        checkDoCutAction();
    } else if (mUnderFrameCtrl[0].checkAnmEnd()) {
        procCutTurnMoveInit(1);
    }

    return 1;
}

int daAlink_c::procCutLargeJumpInit() {
    if (mDemo.getDemoMode() == 0x56 &&
        (mProcID == PROC_CUT_LARGE_JUMP || mProcID == PROC_CUT_LARGE_JUMP_LAND))
    {
        return 1;
    }

    commonProcInit(PROC_CUT_LARGE_JUMP);
    setSingleAnimeParam(ANM_CUT_JUMP_LARGE_START, &mpHIO->mCut.mCutLargeJump.m.mChargeMoveAnm);
    field_0x3588 = l_halfAtnWaitBaseAnime;
    mNormalSpeed = 0.0f;
    current.angle.y = shape_angle.y;
    mProcVar2.field_0x300c = 0;

    setSwordAtParam(dCcG_At_Spl_UNK_0, 1, 1, 2, mpHIO->mCut.m.mSwordLength,
                    mpHIO->mCut.m.mSwordRadius);
    setCutType(CUT_TYPE_LARGE_JUMP_INIT);
    setSwordVoiceSe(Z2SE_AL_V_JUMP_ATTACK_L_1);
    setCutWaterDropEffect();
    mProcVar3.field_0x300e = 4;
    mProcVar4.field_0x3010 = 0;
    field_0x3198 = 2;
    return 1;
}

int daAlink_c::procCutLargeJump() {
    daPy_frameCtrl_c* frameCtrl = &mUnderFrameCtrl[0];

    if (mProcVar4.field_0x3010 != 0) {
        field_0x2f99 = 4;
    }

    if (checkModeFlg(2) && checkCutTurnInput()) {
        field_0x3198 = getCutTurnDirection();
    }

    if (mProcVar2.field_0x300c == 0) {
        if (frameCtrl->checkAnmEnd()) {
            field_0x2f99 = 12;
            setSingleAnimeParam(ANM_CUT_JUMP_LARGE, &mpHIO->mCut.mCutLargeJump.m.mCutAnm);
            mProcVar2.field_0x300c = 1;
            setSwordVoiceSe(Z2SE_AL_V_JUMP_ATTACK_L_2);
        } else {
            if (frameCtrl->getFrame() >= 2.0f) {
                mProcVar4.field_0x3010 = 1;
            }

            if (!checkModeFlg(2) && frameCtrl->getFrame() >= 5.0f) {
                setJumpMode();
                mNormalSpeed = mpHIO->mCut.mCutLargeJump.m.mCutSpeedH;
                speed.y = mpHIO->mCut.mCutLargeJump.m.mCutSpeedV;
                setCutJumpSpeed(0);
            }

            if (frameCtrl->getFrame() >= mpHIO->mCut.mCutLargeJump.m.mJumpAttackStartFrame &&
                frameCtrl->getFrame() < mpHIO->mCut.mCutLargeJump.m.mJumpAttackEndFrame)
            {
                if (!checkNoResetFlg0(FLG0_UNK_40)) {
                    seStartSwordCut(Z2SE_AL_SWORD_SWING_S);
                }
                onResetFlg0(RFLG0_UNK_2);
            }
        }
    } else {
        if (mProcVar3.field_0x300e != 0) {
            mProcVar3.field_0x300e--;

            if (mProcVar3.field_0x300e == 0) {
                setSwordAtParam(dCcG_At_Spl_UNK_1, 3, 1, 4, mpHIO->mCut.m.mSwordLength,
                                mpHIO->mCut.m.mSwordRadius);
                setCutType(CUT_TYPE_LARGE_JUMP);
            }
        }

        if (mLinkAcch.ChkGroundHit()) {
            procCutLargeJumpLandInit(field_0x3198);
        } else if (frameCtrl->getFrame() >= mpHIO->mCut.mCutLargeJump.m.mCutInitFrame) {
            if (!checkNoResetFlg0(FLG0_UNK_40)) {
                seStartSwordCut(Z2SE_AL_SWORD_SWING_S);
            }

            onResetFlg0(RFLG0_UNK_2);
        }
    }

    return 1;
}

int daAlink_c::procCutLargeJumpLandInit(int param_0) {
    commonProcInit(PROC_CUT_LARGE_JUMP_LAND);
    checkCutLandDamage();
    setSingleAnimeParam(ANM_CUT_JUMP_LARGE_LAND, &mpHIO->mCut.mCutLargeJump.m.mLandAnm);
    mNormalSpeed = 0.0f;
    field_0x2f99 = 12;
    field_0x3588 = l_halfAtnWaitBaseAnime;
    field_0x2f9d = 4;
    setFootEffectProcType(4);
    onResetFlg1(RFLG1_UNK_30);
    onResetFlg0(RFLG0_UNK_2);
    setCutType(CUT_TYPE_LARGE_JUMP);
    field_0x3478 = mpHIO->mCut.mCutLargeJump.m.mLandAttackRadius;
    field_0x347c = 130.0f;
    mProcVar2.field_0x300c = 0;
    field_0x3198 = param_0;

    f32 tmp_0 = 0.0f;
    dComIfGp_getVibration().StartShock(3, 31, cXyz(tmp_0, 1.0f, tmp_0));
    setCutLargeJumpLandEffect();
    seStartOnlyReverb(Z2SE_AL_JUMP_ATK_L_IMPACT);
    return 1;
}

int daAlink_c::procCutLargeJumpLand() {
    daPy_frameCtrl_c* frameCtrl = mUnderFrameCtrl;

    onEndResetFlg0(ERFLG0_UNK_8000000);
    field_0x2f99 = 4;

    if (field_0x3198 != 2 && checkCutTurnInput()) {
        field_0x3198 = getCutTurnDirection();
    }

    if (frameCtrl->checkAnmEnd()) {
        if (mDemo.getDemoMode() == 0x56) {
            dComIfGp_evmng_cutEnd(mAlinkStaffId);
        } else {
            checkNextAction(0);
        }
    } else if (frameCtrl->getFrame() > mpHIO->mCut.mCutLargeJump.m.mSpinSlashCheckFrame &&
               field_0x3198 != 2)
    {
        procCutTurnInit(1, field_0x3198);
    } else if (frameCtrl->getFrame() > mpHIO->mCut.mCutLargeJump.m.mLandAnm.mCancelFrame) {
        checkNextAction(1);
    } else if (frameCtrl->getFrame() < mpHIO->mCut.mCutLargeJump.m.mLandAttackEndFrame) {
        onResetFlg0(RFLG0_UNK_2);

        if (frameCtrl->getFrame() >= mpHIO->mCut.mCutLargeJump.m.mLandAttackInitFrame) {
            if (mProcVar2.field_0x300c == 0) {
                resetAtCollision(0);
                mProcVar2.field_0x300c = 1;
                setCutType(CUT_TYPE_LARGE_JUMP_FINISH);
                initCutTurnAt(field_0x347c, 4);
            }

            cLib_chaseF(&field_0x347c, field_0x3478, 20.0f);
            field_0xFB8.SetR(field_0x347c);
        }
    }

    return 1;
}

int daAlink_c::procSwordUnequipSpInit() {
    if (!commonProcInitNotSameProc(PROC_SWORD_UNEQUIP_SP)) {
        return 0;
    }

    setSingleAnimeBaseSpeed(ANM_FINISH, mpHIO->mCut.m.mFlourishAnmSpeed, 3.0f);
    mNormalSpeed = 0.0f;
    field_0x3588 = l_halfAtnWaitBaseAnime;
    field_0x3198 = 0;
    mProcVar2.field_0x300c = 0;
    mProcVar3.field_0x300e = (0x1E - mDemo.getDemoMode()) != 0;
    mProcVar4.field_0x3010 = 0;

    return 1;
}

int daAlink_c::procSwordUnequipSp() {
    if (checkGroundSpecialMode()) {
        return 1;
    }

    daPy_frameCtrl_c* frameCtrl = mUnderFrameCtrl;

    if (mProcVar3.field_0x300e != 0) {
        field_0x2f99 = 4;
    } else {
        mProcVar3.field_0x300e = 1;
    }

    onEndResetFlg0(ERFLG0_UNK_8000000);

    if (mProcVar2.field_0x300c != 0) {
        onEndResetFlg1(ERFLG1_GANON_FINISH);
    }

    if (mProcVar4.field_0x3010 == 0 && mDemo.getDemoMode() == 0x1E && mDemo.getParam1() == 1) {
        mProcVar4.field_0x3010 = 1;
        setFaceBasicBck(0x149);
        setFaceBasicBtp(0x3E1);
    }

    if (frameCtrl->checkAnmEnd()) {
        if (field_0x3198 != 0) {
            if (mDemo.getDemoMode() == 0x1E) {
                dComIfGp_evmng_cutEnd(mAlinkStaffId);
            } else {
                checkNextAction(0);
            }
        } else {
            field_0x3198 = 1;
            setSingleAnimeBaseSpeed(ANM_FINISH_END, mpHIO->mCut.m.mFlourishEndAnmSpeed,
                                    3.0f);
        }
    } else if (field_0x3198 != 0) {
        if (!(frameCtrl->getFrame() > mpHIO->mCut.m.mFlourishControlStartFrame) ||
            !checkNextAction(1))
        {
            if (frameCtrl->getFrame() >= 39.0f) {
                field_0x2f92 = 1;
            } else if (frameCtrl->getFrame() >= 36.0f) {
                field_0x2f93 = 6;
            } else if (frameCtrl->getFrame() >= 26.0f) {
                field_0x2f92 = 4;
            } else if (frameCtrl->getFrame() >= 13.0f) {
                if (mDemo.getDemoMode() == 0x1E) {
                    if (mDemo.getParam0() == 1) {
                        frameCtrl->setFrame(13.0f);
                        frameCtrl->setRate(0.0f);
                        dComIfGp_evmng_cutEnd(mAlinkStaffId);
                    } else if (frameCtrl->getRate() < 0.0099999998f) {
                        setWaterInAnmRate(frameCtrl, mpHIO->mCut.m.mFlourishEndAnmSpeed);
                    }
                }
                deleteEquipItem(0, 1);
            } else if (frameCtrl->checkPass(8.0f)) {
                seStartSwordCut(0x20008);
            }
        }
    } else {
        if (frameCtrl->getFrame() >= 9.0f) {
            field_0x2f93 = 0xFE;
            mProcVar2.field_0x300c = 1;
            onEndResetFlg1(ERFLG1_GANON_FINISH);
        }

        if (frameCtrl->checkPass(7.0f) || frameCtrl->checkPass(13.0f)) {
            seStartSwordCut(0x20006);
        } else if (frameCtrl->checkPass(28.0f) || frameCtrl->checkPass(38.0f) ||
                   frameCtrl->checkPass(48.0f))
        {
            seStartSwordCut(0x20007);
        }
    }

    return 1;
}
