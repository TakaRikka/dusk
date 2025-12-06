/**
 * d_a_alink_hook.inc
 * Player Clawshot action handling
 */

#include "d/actor/d_a_alink.h"
#include "d/actor/d_a_b_dr.h"
#include "d/actor/d_a_obj_swhang.h"
#include "d/actor/d_a_obj_chandelier.h"
#include "JSystem/J3DGraphBase/J3DMaterial.h"

enum {
    HS_MODE_NONE_e,
    HS_MODE_READY_e,
    HS_MODE_SHOOT_e = 3,
    HS_MODE_FLY_e,
    HS_MODE_RETURN_e = 6,
};

void daAlink_c::hsChainShape_c::draw() {
    daAlink_c* temp_r3 = (daAlink_c*)getUserArea();
    J3DModelData* sp60 = temp_r3->getItemModelData();
    J3DMaterial* sp5C = sp60->getMaterialNodePointer(0);
    daAlink_hsChainLight_c* temp_r29 = (daAlink_hsChainLight_c*)&temp_r3->tevStr;

    j3dSys.setVtxPos(sp60->getVtxPosArray());
    j3dSys.setVtxNrm(sp60->getVtxNrmArray());
    j3dSys.setVtxCol(sp60->getVtxColorArray(0));
    J3DShape::resetVcdVatCache();

    sp5C->loadSharedDL();
    sp5C->getShape()->loadPreDrawSetting();

    GXColor sp58;
    sp58.r = temp_r29->AmbCol.r;
    sp58.g = temp_r29->AmbCol.g;
    sp58.b = temp_r29->AmbCol.b;
    sp58.a = temp_r29->AmbCol.a;

    GXSetChanAmbColor(GX_COLOR0A0, sp58);
    GXSetChanMatColor(GX_COLOR0A0, g_whiteColor);

    dKy_setLight_again();
    dKy_GxFog_tevstr_set(temp_r29);
    GXLoadLightObjImm(temp_r29->getLightObj(), GX_LIGHT0);

    if (temp_r3->checkIronBallEquip()) {
        cXyz* sp54 = temp_r3->getIronBallChainPos();
        csXyz* sp50 = temp_r3->getIronBallChainAngle();
        sp54++;
        sp50++;

        for (int i = 0; i < 101; i++, sp54++, sp50++) {
            mDoMtx_stack_c::copy(j3dSys.getViewMtx());
            mDoMtx_stack_c::transM(*sp54);
            mDoMtx_stack_c::ZXYrotM(*sp50);
            mDoMtx_stack_c::scaleM(2.0f, 2.0f, 2.0f);

            GXLoadPosMtxImm(mDoMtx_stack_c::get(), GX_PNMTX0);
            GXLoadNrmMtxImm(mDoMtx_stack_c::get(), GX_PNMTX0);

            sp5C->getShape()->simpleDrawCache();
        }

        cXyz spC8;
        cXyz spBC;

        for (int i = 0; i < 2; i++) {
            cXyz* sp48;
            csXyz* sp44;

            s16 sp14;
            s16 sp12;
            s16 sp10;
            s16 spE;

            if (i == 0) {
                sp48 = temp_r3->getIronBallChainPos() + temp_r3->getIronBallHandChainNum();
                spC8 = temp_r3->getIronBallChainHandRootPos() - *sp48;

                sp44 = temp_r3->getIronBallChainAngle() + temp_r3->getIronBallHandChainNum();
                spE = 0x3000;
            } else {
                sp48 = temp_r3->getIronBallChainPos();
                mDoMtx_stack_c::transS(*(temp_r3->getIronBallChainPos() + 1));
                mDoMtx_stack_c::ZXYrotM(*(temp_r3->getIronBallChainAngle() + 1));
                mDoMtx_stack_c::multVec(&l_ironBallChainVec, &spC8);
                spC8 -= *sp48;

                sp44 = temp_r3->getIronBallChainAngle() + 1;
                spE = -0x3000;
            }

            sp10 = sp44->y;
            sp14 = sp44->z + spE;
            f32 var_f29 = spC8.abs();

            spBC = *sp48;

            if (var_f29 > 5.0f) {
                if (spC8.absXZ() >= 1.0f) {
                    sp10 = spC8.atan2sX_Z();
                }

                sp12 = spC8.atan2sY_XZ();
                cXyz spB0 = spC8 * (10.0f / var_f29);

                while (var_f29 > 5.0f) {
                    mDoMtx_stack_c::copy(j3dSys.getViewMtx());
                    mDoMtx_stack_c::transM(spBC);
                    mDoMtx_stack_c::ZXYrotM(sp12, sp10, sp14);
                    mDoMtx_stack_c::scaleM(2.0f, 2.0f, 2.0f);

                    GXLoadPosMtxImm(mDoMtx_stack_c::get(), GX_PNMTX0);
                    GXLoadNrmMtxImm(mDoMtx_stack_c::get(), GX_PNMTX0);

                    sp5C->getShape()->simpleDrawCache();

                    spBC += spB0;
                    var_f29 -= 10.0f;
                    sp14 += spE;
                }
            }
        }
    } else {
        const cXyz& chainRootPos = temp_r3->getHsChainRootPos();
        const cXyz& chainTopPos = temp_r3->getHsChainTopPos();
        cXyz maxDistance = chainRootPos - chainTopPos;

        f32 maxDistanceF = maxDistance.abs();
        f32 var_f30;
        cXyz sp98;
        csXyz sp6C;

        if (maxDistanceF > 1.0f) {
            maxDistance *= (1.0f / maxDistanceF);
            var_f30 = 0.0f;

            sp98 = chainTopPos;
            sp6C.set(maxDistance.atan2sY_XZ(), maxDistance.atan2sX_Z(), 0);
            sp98 = chainTopPos;

            csXyz sp64(sp6C);

            f32 sp34 = M_PI / maxDistanceF;
            
            f32 temp_f27;
            f32 var_f26 = 0.0f;
            f32 var_f28;

            var_f28 = 2.5f * temp_r3->getHookshotStopTime();
            if (temp_r3->getHookshotStopTime() & 1) {
                var_f28 *= -1.0f;
            }

            while (maxDistanceF > var_f30) {
                temp_f27 = var_f28 * cM_fsin(sp34 * var_f30);
                s16 spC = cM_atan2s(temp_f27 - var_f26, 5.0f);
                sp64.x = sp6C.x + spC;

                mDoMtx_stack_c::transS(sp98);
                mDoMtx_stack_c::ZXYrotM(sp64);

                static const Vec hsVec = {0.0f, 0.0f, 5.0f};
                mDoMtx_stack_c::multVec(&hsVec, &sp98);

                mDoMtx_stack_c::revConcat(j3dSys.getViewMtx());

                GXLoadPosMtxImm(mDoMtx_stack_c::get(), GX_PNMTX0);
                GXLoadNrmMtxImm(mDoMtx_stack_c::get(), GX_PNMTX0);

                sp5C->getShape()->simpleDrawCache();

                sp64.z += 0x3000;

                var_f26 = temp_f27;
                var_f30 += fabsf(cM_scos(spC)) * 5.0f;
            }
        }

        const cXyz& sp30 = temp_r3->getHsSubChainRootPos();
        const cXyz& sp2C = temp_r3->getHsSubChainTopPos();
        maxDistance = sp30 - sp2C;

        f32 temp_f1 = maxDistance.abs();
        if (temp_f1 > 1.0f) {
            maxDistance *= (1.0f / temp_f1);
            var_f30 = 0.0f;
            
            sp98 = sp2C;
            sp6C.set(maxDistance.atan2sY_XZ(), maxDistance.atan2sX_Z(), 0);

            while (temp_f1 > var_f30) {
                mDoMtx_stack_c::copy(j3dSys.getViewMtx());
                mDoMtx_stack_c::transM(sp98);
                mDoMtx_stack_c::ZXYrotM(sp6C);

                GXLoadPosMtxImm(mDoMtx_stack_c::get(), GX_PNMTX0);
                GXLoadNrmMtxImm(mDoMtx_stack_c::get(), GX_PNMTX0);

                sp5C->getShape()->simpleDrawCache();

                sp98 += maxDistance * 5.0f;
                sp6C.z += 0x3000;
                var_f30 += 5.0f;
            }
        }
    }
}

void daAlink_c::hookshotAtHitCallBack(dCcD_GObjInf* i_atObjInf, fopAc_ac_c* i_tgActor,
                                      dCcD_GObjInf* i_tgObjInf) {
    if (i_tgActor != NULL && fopAcM_IsActor(i_tgActor) && !i_tgObjInf->ChkTgHookshotThrough()) {
        cXyz* hit_pos_p = i_atObjInf->GetAtHitPosP();
        f32 dist_to_hitpos = getHsChainTopPos().abs(*hit_pos_p);

        if (dist_to_hitpos < mSearchBallScale) {
            mSearchBallScale = dist_to_hitpos;

            if (checkHookshotWait()) {
                if (fopAcM_CheckStatus(i_tgActor, 0x280000)) {
                    mHookTargetAcKeep.setData(i_tgActor);
                } else {
                    field_0x3494 = 1.0f;
                }
            } else if (!i_tgObjInf->ChkTgShield()) {
                mHookTargetAcKeep.setData(i_tgActor);
                field_0x381c = *i_atObjInf->GetAtHitPosP();
            } else {
                field_0x3494 = 1.0f;
            }
        }
    }
}

static void daAlink_hookshotAtHitCallBack(fopAc_ac_c* i_atActor, dCcD_GObjInf* i_atObjInf,
                                          fopAc_ac_c* i_tgActor, dCcD_GObjInf* i_tgObjInf) {
    static_cast<daAlink_c*>(i_atActor)->hookshotAtHitCallBack(i_atObjInf, i_tgActor, i_tgObjInf);
}

cXyz* daAlink_c::getHookshotTopPos() {
    if (checkHookshotItem(mEquipItem)) {
        return &mHookshotTopPos;
    }

    return NULL;
}

bool daAlink_c::checkHookshotReturnMode() const {
    return checkHookshotItem(mEquipItem) &&
           (mItemMode == HS_MODE_FLY_e || mItemMode == 5 || mItemMode == HS_MODE_RETURN_e);
}

bool daAlink_c::checkHookshotShootReturnMode() const {
    return (checkHookshotItem(mEquipItem) && mItemMode == HS_MODE_SHOOT_e) ||
           checkHookshotReturnMode();
}

void daAlink_c::resetHookshotMode() {
    mItemMode = HS_MODE_NONE_e;
    initLockAt();
    mSearchBallScale = FLT_MAX;
}

BOOL daAlink_c::setEnemyBombHookshot(fopAc_ac_c* i_actor) {
    if (mHookTargetAcKeep.getActor() != NULL) {
        cancelHookshotCarry();
        mHookTargetAcKeep.setData(i_actor);
        fopAcM_setHookCarryNow(i_actor);
        return true;
    }
    return false;
}

bool daAlink_c::checkLv7BossRoom() {
    return checkStageName("D_MN07A");
}

BOOL daAlink_c::checkHookshotStickBG(cBgS_PolyInfo& i_poly) {
    if (dComIfG_Bgsp().ChkPolyHSStick(i_poly)) {
        dBgW_Base* bgw_p = dComIfG_Bgsp().GetBgWBasePointer(i_poly);
        if (bgw_p != NULL && bgw_p->ChkPushPullOk()) {
            return true;
        }
    }

    return false;
}

void daAlink_c::cancelHookshotCarry() {
    if (mHookTargetAcKeep.getActor() != NULL) {
        fopAcM_cancelHookCarryNow(mHookTargetAcKeep.getActor());
        mHookTargetAcKeep.clearData();
    }
}

void daAlink_c::changeHookshotDrawModel() {
    if (mEquipItem == fpcNm_ITEM_HOOKSHOT && field_0x3020 == 1) {
        J3DModel* old_item = mHeldItemModel;
        mHeldItemModel = field_0x0710;
        field_0x0710 = old_item;

        J3DModel* old_item2 = mpHookTipModel;
        mpHookTipModel = field_0x0714;
        field_0x0714 = old_item2;
    }
}

BOOL daAlink_c::checkHookshotRoofLv7Boss() {
    return mCargoCarryAcKeep.getActor() != NULL &&
           fopAcM_GetName(mCargoCarryAcKeep.getActor()) == PROC_B_DR;
}

BOOL daAlink_c::checkChaseHookshot() {
    if (field_0x2804 == NULL) {
        if (mTargetedActor != NULL) {
            field_0x2804 = mTargetedActor;
        } else {
            field_0x2804 = this;
        }
    } else if (field_0x2804 != mTargetedActor) {
        field_0x2804 = this;
    }

    if (mTargetedActor != NULL) {
        s16 actorName = fopAcM_GetName(mTargetedActor);
        return field_0x2804 == mTargetedActor &&
               (checkBossOctaIealRoom() || actorName == PROC_Obj_FallObj ||
                actorName == PROC_B_DR || actorName == PROC_E_PH);
    }

    return false;
}

BOOL daAlink_c::checkOctaIealSpecialCollect() {
    return mProcID == PROC_HOOKSHOT_FLY && checkHookshotItem(mEquipItem) && checkBossOctaIealRoom();
}

BOOL daAlink_c::checkBossOctaIealRoom() {
    return checkStageName("D_MN01A");
}

BOOL daAlink_c::checkHookshotWait() const {
    return mItemMode == HS_MODE_NONE_e || mItemMode == HS_MODE_READY_e;
}

void daAlink_c::setHookshotCatchNow() {
    field_0x3026 = 5;
    dComIfGp_getVibration().StartShock(4, 1, cXyz(0.0f, 1.0f, 0.0f));
}


bool daAlink_c::setHookshotCarryOffset(fpc_ProcID i_carryActorID, cXyz const* i_posOffset) {
    if (dComIfGp_checkPlayerStatus1(0, 0x10000)) {
        fopAc_ac_c* carryActor_p = mCargoCarryAcKeep.getActor();

        if (carryActor_p != NULL && fopAcM_checkHookCarryNow(carryActor_p) &&
            i_carryActorID == mCargoCarryAcKeep.getID())
        {
            field_0x37c8 = *i_posOffset;
            return true;
        }
    }

    fopAc_ac_c* hookTargetAc_p = mHookTargetAcKeep.getActor();
    if (hookTargetAc_p != NULL && fopAcM_checkHookCarryNow(hookTargetAc_p) &&
        i_carryActorID == mHookTargetAcKeep.getID())
    {
        mIronBallCenterPos = *i_posOffset;
        return true;
    }

    return false;
}

void daAlink_c::setHookshotModel() {
    J3DAnmTransform* bck = (J3DAnmTransform*)mAnmHeap9.loadDataIdx(0x17E);

    JKRHeap* heap = setItemHeap();
    mItemBck.init(bck, 0, 2, 1.0f, 0, -1, false);

    J3DModelData* hookModelData = loadAramBmd(0x316, 0x5C00);
    mHeldItemModel = initModel(hookModelData, 0x80000, 0);

    J3DModelData* hookTipModelData = loadAramBmd(0x318, 0x3800);
    mpHookTipModel = initModel(hookTipModelData, 0x80000, 0);
    field_0x0710 = initModel(mHeldItemModel->getModelData(), 0x80000, 0);
    field_0x0714 = initModel(mpHookTipModel->getModelData(), 0x80000, 0);

    mpItemModelData = loadAramBmd(0x317, 0x1000);
    mpHookChain = new hsChainShape_c();
    mpHookSound = new Z2SoundObjSimple();

    mpHookshotLinChk = new dBgS_ObjLinChk();
    mpHookshotLinChk->OffFullGrp();
    mpHookshotLinChk->OnWaterGrp();

    J3DAnmTransform* bck2 = (J3DAnmTransform*)loadAram(dRes_ID_ALANM_BCK_HS_TIP_OPEN_e, 0x800);
    mHookTipBck.init(bck2, 0, 2, 1.0f, 0, -1, false);
    mDoExt_setCurrentHeap(heap);

    resetHookshotMode();
    mpHookChain->setUserArea((uintptr_t)this);
    field_0x2f94 = 1;
    if (mEquipItem == fpcNm_ITEM_W_HOOKSHOT) {
        field_0x2f95 = 6;
    }

    field_0x3020 = 0;
    field_0x3024 = 0;

    mAtCps[0].SetAtSpl(dCcG_At_Spl_UNK_0);
    mAtCps[0].OffAtNoHitMark();
    mAtCps[0].SetAtAtp(0);
    mAtCps[0].SetR(5.0f);
    mAtCps[0].SetAtSe(dCcD_SE_HOOKSHOT_STICK);
    mAtCps[0].SetAtType(AT_TYPE_HOOKSHOT);
    mAtCps[0].SetAtHitMark(1);
    mAtCps[0].OnAtSetBit();
    mAtCps[0].SetAtHitCallback(daAlink_hookshotAtHitCallBack);
    mAtCps[0].SetAtMtrl(dCcD_MTRL_NONE);
    mpHookSound->init(&mHookshotTopPos, 1);
}

void daAlink_c::setHookshotSight() {
    cXyz sight_pos;
    f32 length;
    if (checkLv7BossRoom()) {
        length = mpHIO->mItem.mHookshot.m.mBossMaxLength;
    } else {
        length = mpHIO->mItem.mHookshot.m.mMaxLength;
    }

    BOOL line_cross = checkSightLine(length, &sight_pos);

    if (mHookTargetAcKeep.getActor() != NULL) {
        mSight.setPos(&mHookTargetAcKeep.getActor()->eyePos);
    } else {
        mSight.setPos(&sight_pos);
    }

    if (mItemMode == HS_MODE_READY_e) {
        mSight.onDrawFlg();

        if ((line_cross && field_0x3494 < 0.0f && checkHookshotStickBG(mRopeLinChk)) ||
            mHookTargetAcKeep.getActor() != NULL)
        {
            mSight.onLockFlg();

            if (mHookTargetAcKeep.getActor() != NULL &&
                fopAcM_GetName(mHookTargetAcKeep.getActor()) == PROC_B_DR)
            {
                daB_DR_c* dr_p = (daB_DR_c*)mHookTargetAcKeep.getActor();
                dr_p->onTarget();
            }
        } else {
            mSight.offLockFlg();
        }
    } else {
        mSight.offDrawFlg();
    }

    mHookTargetAcKeep.clearData();
    field_0x3494 = -1.0f;
}


void daAlink_c::cancelHookshotShot() {
    if (checkHookshotItem(mEquipItem) &&
        (mItemMode == HS_MODE_SHOOT_e || mItemMode == 5 || mItemMode == HS_MODE_FLY_e))
    {
        if (mProcID != PROC_HOOKSHOT_MOVE && mProcID != PROC_HOOKSHOT_FLY &&
            mProcID != PROC_HOOKSHOT_SUBJECT)
        {
            mItemMode = HS_MODE_RETURN_e;
        }
    }
}

bool daAlink_c::cancelHookshotMove() {
    if (mFastShotTime == 0 && mItemMode == HS_MODE_NONE_e) {
        if (checkHookshotAnime() &&
            (mTargetedActor == NULL && !checkAttentionLock() || !itemButton()))
        {
            resetUpperAnime(UPPER_2, -1.0f);
            return 1;
        }
    }

    return 0;
}


BOOL daAlink_c::checkHookshotReadyMaterialOffMode() const {
    return mProcID == PROC_HOOKSHOT_WALL_SHOOT || mProcID == PROC_HORSE_HOOKSHOT_SUBJECT ||
           mProcID == PROC_SWIM_HOOKSHOT_SUBJECT || mProcID == PROC_HOOKSHOT_SUBJECT;
}

void daAlink_c::setHookshotReadyMaterial() {
    if (checkNoResetFlg2(FLG2_UNK_80000) || checkCasualWearFlg()) {
        mFallVoiceInit = 0;
    } else if (checkZoraWearFlg()) {
        field_0x32cc = 0x37B;
        mFallVoiceInit = 14;
    } else if (checkMagicArmorWearFlg()) {
        field_0x32cc = 0xC80;
        mFallVoiceInit = 13;
    } else {
        field_0x32cc = 0x11C47;
        mFallVoiceInit = 18;
    }
}

int daAlink_c::initHookshotUpperAnimeSpeed(int param_0) {
    if (checkHookshotWait()) {
        if (param_0) {
            f32 tmp_0 = 0.0f;
            mUpperFrameCtrl[2].setRate(tmp_0);
            mUpperFrameCtrl[2].setFrame(tmp_0);
            getNowAnmPackUpper(UPPER_2)->setFrame(tmp_0);
        } else {
            setWaterInAnmRate(&mUpperFrameCtrl[2], mpHIO->mItem.mHookshot.m.mWaitAnmSpeed);
        }

        return 1;
    }

    return 0;
}

void daAlink_c::initHookshotReady() {
    mItemMode = HS_MODE_READY_e;

    f32 tmp_0 = 0.0f;
    field_0x33e0 = tmp_0;

    dComIfGp_getVibration().StartShock(1, 1, cXyz(tmp_0, 1.0f, tmp_0));

    mItemVar0.field_0x3018 = 3;
    field_0x3494 = -1.0f;
}

void daAlink_c::setHookshotReadyAnime() {
    f32 speed;
    if (!checkAttentionLock() && mFastShotTime == 0) {
        speed = 0.0f;
    } else {
        speed = mpHIO->mItem.mHookshot.m.mWaitAnmSpeed;
    }

    setUpperAnimeBaseSpeed(dRes_INDEX_ALANM_BCK_HSWAIT_e, speed, 3.0f);
    field_0x3020 = 0;
    initHookshotReady();
}

int daAlink_c::checkUpperItemActionHookshot() {
    if (checkHookshotWait()) {
        mSearchBallScale = FLT_MAX;
    }

    if (mItemMode == HS_MODE_READY_e) {
        if (!itemButton() && mItemVar0.field_0x3018 == 0 && checkReadyItem()) {
            mItemMode = 2;
            field_0x3026 = 0;
            field_0x3494 = -1.0f;
            mHookTargetAcKeep.clearData();
            cancelLockAt();
            field_0x2804 = NULL;

            if (mProcID == PROC_HOOKSHOT_ROOF_SHOOT || mProcID == PROC_HOOKSHOT_WALL_SHOOT) {
                daAlink_ANM anmID;
                if (mProcID == PROC_HOOKSHOT_ROOF_SHOOT) {
                    if (field_0x3020 == 0) {
                        anmID = ANM_HOOKSHOT_HANG_SHOOT_RIGHT;
                    } else {
                        anmID = ANM_HOOKSHOT_HANG_SHOOT_LEFT;
                    }
                } else if (field_0x3020 == 0) {
                    anmID = ANM_HOOKSHOT_WALL_SHOOT_RIGHT;
                } else {
                    anmID = ANM_HOOKSHOT_WALL_SHOOT_LEFT;
                }

                setSingleAnimeBaseMorf(anmID, 0.0f);
            } else if (checkModeFlg(0x40400)) {
                setUpperAnimeParam(0x18C, UPPER_2, &mpHIO->mItem.mHookshot.m.mShootAnm);

                if (checkModeFlg(0x40000)) {
                    mNormalSpeed = 0.0f;
                }
            } else {
                f32 tmp_0 = 0.0f;
                mUpperFrameCtrl[2].setRate(tmp_0);
                mUpperFrameCtrl[2].setFrame(tmp_0);
                getNowAnmPackUpper(UPPER_2)->setFrame(tmp_0);

                mNormalSpeed = tmp_0;
                onModeFlg(1);
                mSpeedModifier = tmp_0;
                mNowAnmPackUpper[2].setRatio(tmp_0);

                setSingleAnimeParam(ANM_HOOKSHOT_SHOOT, &mpHIO->mItem.mHookshot.m.mShootAnm);
            }

            if (!checkNoResetFlg3(FLG3_UNK_4)) {
                f32 tmp_0 = 0.0f;
                dComIfGp_getVibration().StartQuake(1, 1, cXyz(tmp_0, 1.0f, tmp_0));
                onNoResetFlg3(FLG3_UNK_4);
            }

            return 1;
        }
    } else if (mItemMode == HS_MODE_NONE_e) {
        if (checkModeFlg(0x400) && checkAttentionLock() && mTargetedActor == NULL) {
            resetUpperAnime(UPPER_2, 3.0f);
            dComIfGp_clearPlayerStatus0(0, 0x4000);
            return 1;
        }

        if (checkReadyItem() && itemButton()) {
            initHookshotReady();
        }
    } else if (mItemMode == HS_MODE_SHOOT_e) {
        if (checkReadyItem() && itemTrigger()) {
            mItemMode = HS_MODE_RETURN_e;
        }

        checkNextActionHookshot();
        return 1;
    } else if (mItemMode == HS_MODE_RETURN_e) {
        checkNextActionHookshot();
        return 1;
    } else if (mItemMode == 5 || mItemMode == HS_MODE_FLY_e) {
        if (field_0x3026 != 0) {
            field_0x3026--;
            setHookshotTopPosFly();
            return 1;
        }

        return procHookshotFlyInit();
    }

    if (mItemVar0.field_0x3018 > 0) {
        mItemVar0.field_0x3018--;
    }

    return cancelUpperItemReadyAnime(0);
}

int daAlink_c::checkNextActionHookshot() {
    if (mProcID == PROC_HOOKSHOT_ROOF_SHOOT || mProcID == PROC_HOOKSHOT_WALL_SHOOT) {
        return 0;
    }

    if (mFastShotTime != 0 && checkHookshotWait()) {
        mFastShotTime--;
    }

    if (!checkHookshotAnime()) {
        setHookshotReadyAnime();
        setFastShotTimer();

        if (!checkAttentionLock()) {
            field_0x2fe4 = shape_angle.y;
        }
    }

    if (!checkAttentionLock() && mFastShotTime == 0) {
        if (checkModeFlg(0x400)) {
            if (checkCanoeRide()) {
                return procCanoeHookshotSubjectInit();
            } else {
                return procHorseHookshotSubjectInit();
            }
        } else if (checkModeFlg(0x40000)) {
            return procSwimHookshotSubjectInit();
        } else {
            return procHookshotSubjectInit();
        }
    } else if (checkModeFlg(0x400)) {
        if (checkCanoeRide()) {
            return procCanoeHookshotMoveInit();
        } else {
            return procHorseHookshotMoveInit();
        }
    } else if (checkModeFlg(0x40000)) {
        return procSwimHookshotMoveInit();
    } else {
        return procHookshotMoveInit();
    }
}

void daAlink_c::setHookshotReturnEnd() {
    resetHookshotMode();
    cancelItemUseQuake(0);

    seStartOnlyReverb(Z2SE_LK_HS_WIND_UP_FIN);

    if (checkHookshotReadyAnime()) {
        mNowAnmPackUpper[2].setRatio(1.0f);
        field_0x2060->initOldFrameMorf(3.0f, field_0x30a8, field_0x30aa);
    } else if (checkHookshotShootAnime()) {
        setUpperAnimeBaseSpeed(
            0x18D, checkAttentionLock() ? mpHIO->mItem.mHookshot.m.mWaitAnmSpeed : 0.0f, 3.0f);
    }

    mFastShotTime = 0;
}

int daAlink_c::setHookshotHangMoveBGCollect() {
    cBgS_PolyInfo* poly;
    if (mItemMode == 4) {
        poly = &mPolyInfo3;
    } else {
        poly = &mPolyInfo2;
    }

    fopAc_ac_c* carry_actor = mCargoCarryAcKeep.getActor();

    if (carry_actor != NULL) {
        cXyz sp28(mIronBallBgChkPos);
        mDoMtx_stack_c::transS(carry_actor->current.pos);
        mDoMtx_stack_c::ZXYrotM(carry_actor->shape_angle);
        mDoMtx_stack_c::multVec(&field_0x37c8, &mIronBallBgChkPos);

        current.pos += mIronBallBgChkPos - sp28;
        shape_angle.y += (s16)(mProcVar0.field_0x3008 - carry_actor->shape_angle.y);
        current.angle.y = shape_angle.y;
        mProcVar0.field_0x3008 = carry_actor->shape_angle.y;
    } else if (dComIfG_Bgsp().ChkPolySafe(*poly)) {
        if (!checkHookshotStickBG(*poly)) {
            return 0;
        }

        if (dComIfG_Bgsp().ChkMoveBG(*poly)) {
            dComIfG_Bgsp().MoveBgTransPos(*poly, 1, &current.pos, &current.angle, &shape_angle);
            
            csXyz sp8(0, field_0x3022, 0);
            dComIfG_Bgsp().MoveBgTransPos(*poly, 1, &mIronBallBgChkPos, NULL, &sp8);
            field_0x3022 = sp8.y;

            s16 movebg_name = getMoveBGActorName(*poly, 1);
            if (movebg_name == PROC_Obj_SwHang) {
                daObjSwHang_c* var_r26 = (daObjSwHang_c*)dComIfG_Bgsp().GetActorPointer(*poly);
                var_r26->setHangPlayer();

                f32 prev_y = mIronBallBgChkPos.y;
                mIronBallBgChkPos.y = var_r26->getHangPos().y;
                current.pos.y += mIronBallBgChkPos.y - prev_y;
            } else if (movebg_name == PROC_Obj_Chandelier) {
                ((daObjChandelier_c*)dComIfG_Bgsp().GetActorPointer(*poly))->moveHookOn();
            }
        }
    } else {
        return 0;
    }

    return 1;
}

void daAlink_c::setHookshotTopPosFly() {
    fopAc_ac_c* targetAc_p = mHookTargetAcKeep.getActor();

    if (mItemMode == 5) {
        if (targetAc_p != NULL && fopAcM_checkHookCarryNow(targetAc_p)) {
            mDoMtx_stack_c::transS(targetAc_p->current.pos);
            mDoMtx_stack_c::ZXYrotM(targetAc_p->shape_angle);
            mDoMtx_stack_c::multVec(&mIronBallCenterPos, &mHookshotTopPos);
        } else {
            mItemMode = HS_MODE_RETURN_e;
        }
    } else if (!dComIfG_Bgsp().ChkPolySafe(mPolyInfo2) || !checkHookshotStickBG(mPolyInfo2) ||
               (targetAc_p != NULL && !fopAcM_checkHookCarryNow(targetAc_p)))
    {
        mItemMode = HS_MODE_RETURN_e;
    } else if (dComIfG_Bgsp().ChkMoveBG(mPolyInfo2)) {
        csXyz angle(field_0x301c, field_0x301e, 0);
        dComIfG_Bgsp().MoveBgTransPos(mPolyInfo2, true, &mHookshotTopPos, NULL, &angle);
        field_0x301c = angle.x;
        field_0x301e = angle.y;
    }
}

static cXyz l_hookSnowSandHitScale(0.5f, 0.5f, 0.5f);

void daAlink_c::setHookshotPos() {
    mDoMtx_stack_c::copy(mpLinkModel->getAnmMtx(field_0x30c0));
    mDoMtx_stack_c::transM(-2.0f, 1.0f, 1.0f);
    mDoMtx_stack_c::XYZrotM(cM_deg2s(5.7f), cM_deg2s(162.0f), 0);

    J3DModel* var_r27;
    if (field_0x3020 == 0) {
        var_r27 = mHeldItemModel;
    } else {
        var_r27 = field_0x0710;
    }
    var_r27->setBaseTRMtx(mDoMtx_stack_c::get());

    mDoMtx_stack_c::copy(mpLinkModel->getAnmMtx(field_0x30c2));
    mDoMtx_stack_c::transM(-2.0f, 0.0f, 1.0f);
    mDoMtx_stack_c::XYZrotM(cM_deg2s(-78.0f), cM_deg2s(182.0f), cM_deg2s(-99.0f));

    J3DModel* var_r27_2;
    if (field_0x3020 == 0) {
        var_r27_2 = field_0x0710;
    } else {
        var_r27_2 = mHeldItemModel;
    }
    var_r27_2->setBaseTRMtx(mDoMtx_stack_c::get());

    cXyz sp134;

    if (mItemMode == 2 || mItemMode == HS_MODE_SHOOT_e) {
        field_0x33dc += 1.0f;

        if (field_0x33dc >= mItemBck.getBckAnm()->getFrameMax()) {
            field_0x33dc -= mItemBck.getBckAnm()->getFrameMax();
        }

        field_0x33e0 = mHookTipBck.getBckAnm()->getFrameMax();
    } else if (mItemMode == HS_MODE_FLY_e || mItemMode == 5 || mItemMode == HS_MODE_RETURN_e) {
        field_0x33dc -= 1.0f;
        if (field_0x33dc < 0.0f) {
            field_0x33dc += mItemBck.getBckAnm()->getFrameMax();
        }

        if (mItemMode == HS_MODE_RETURN_e && mHookTargetAcKeep.getActor() == NULL) {
            field_0x33e0 = 0.0f;
        } else {
            field_0x33e0 = 14.0f;
        }
    } else if (mItemMode == HS_MODE_READY_e) {
        if (!checkHookshotAnime() && mProcID != PROC_HOOKSHOT_WALL_SHOOT &&
            mProcID != PROC_HOOKSHOT_ROOF_SHOOT)
        {
            resetHookshotMode();
            field_0x33e0 = 0.0f;
        } else {
            if (field_0x33e0 < 0.1f) {
                seStartOnlyReverb(Z2SE_AL_HS_OPEN);
            }

            field_0x33e0 += 1.0f;
            if (field_0x33e0 > mHookTipBck.getBckAnm()->getFrameMax()) {
                field_0x33e0 = mHookTipBck.getBckAnm()->getFrameMax();
            }
        }
    } else {
        field_0x33e0 = 0.0f;
    }

    mItemBck.entry(field_0x0710->getModelData(), 0.0f);
    field_0x0710->calc();

    static Vec const hookRoot = {0.0f, 0.0f, 23.5f};
    mDoMtx_multVec(mHeldItemModel->getBaseTRMtx(), &hookRoot, &mHeldItemRootPos);
    mDoMtx_multVec(field_0x0710->getBaseTRMtx(), &hookRoot, &field_0x3810);

    mpHookSound->framework(0, mVoiceReverbIntensity);

    fopAc_ac_c* targetAc_p = mHookTargetAcKeep.getActor();
    f32 return_speed;
    f32 shoot_speed;
    f32 length;
    if (checkLv7BossRoom()) {
        return_speed = mpHIO->mItem.mHookshot.m.mBossReturnSpeed;
        shoot_speed = mpHIO->mItem.mHookshot.m.mBossShootSpeed;
        length = mpHIO->mItem.mHookshot.m.mBossMaxLength;
    } else {
        return_speed = mpHIO->mItem.mHookshot.m.mReturnSpeed;
        shoot_speed = mpHIO->mItem.mHookshot.m.mShootSpeed;
        length = mpHIO->mItem.mHookshot.m.mMaxLength;
    }

    if (mItemMode == HS_MODE_RETURN_e) {
        if (targetAc_p != NULL) {
            if (checkLv7BossRoom()) {
                return_speed = mpHIO->mItem.mHookshot.m.mBossStickReturnSpeed;
            } else {
                return_speed = mpHIO->mItem.mHookshot.m.mStickReturnSpeed;
            }
        }

        if (checkModeFlg(0x400)) {
            return_speed += current.pos.abs(field_0x3798);
        }

        if (field_0x3026 != 0) {
            field_0x3026--;
        } else if (mProcID != PROC_ELEC_DAMAGE || !checkHookshotAnime()) {
            if (cLib_chasePos(&mHookshotTopPos, mHeldItemRootPos, return_speed)) {
                setHookshotReturnEnd();
            } else {
                cXyz sp1AC = mHookshotTopPos - mHeldItemRootPos;
                field_0x301c = sp1AC.atan2sY_XZ();
                field_0x301e = sp1AC.atan2sX_Z();
                seStartOnlyReverbLevel(Z2SE_LK_HS_WIND_UP);
            }
        }

        if (targetAc_p != NULL) {
            if (fopAcM_checkHookCarryNow(targetAc_p)) {
                targetAc_p->current.pos = mHookshotTopPos - mIronBallCenterPos;

                if (mItemMode == HS_MODE_NONE_e) {
                    cancelHookshotCarry();
                }
            } else {
                mHookTargetAcKeep.clearData();
            }
        }
    }

    if (checkHookshotWait() || mItemMode == 2) {
        csXyz* var_r28;
        if (mProcID == PROC_HOOKSHOT_WALL_SHOOT || mProcID == PROC_HOOKSHOT_ROOF_SHOOT) {
            var_r28 = (csXyz*)&mProcVar3.field_0x300e;
        } else {
            var_r28 = &mBodyAngle;
        }

        if (mProcID == PROC_HOOKSHOT_WALL_SHOOT) {
            field_0x301e = mProcVar4.field_0x3010;
        } else {
            field_0x301e = (s16)(shape_angle.y + mBodyAngle.y);
        }

        if (mTargetedActor != NULL && mItemMode == 2) {
            s16 var_r25 = getBodyAngleXAtnActor(0);
            if (cLib_distanceAngleS(var_r25, var_r28->x) < 0x3000) {
                cXyz sp1A0;
                getBodyAngleXBasePos(&sp1A0);
                mDoMtx_stack_c::transS(sp1A0);
                mDoMtx_stack_c::ZXYrotM(var_r25, field_0x301e, 0);
                mDoMtx_stack_c::XrotM(-var_r28->x);
                mDoMtx_stack_c::YrotM(-field_0x301e);
                mDoMtx_stack_c::transM(-sp1A0.x, -sp1A0.y, -sp1A0.z);
                mDoMtx_stack_c::multVec(&mHookshotTopPos, &mHookshotTopPos);
                var_r28->x = var_r25;
            }
        }

        field_0x301c = var_r28->x;
        mDoMtx_stack_c::ZXYrotS(field_0x301c, field_0x301e, 0);
        mDoMtx_stack_c::multVec(&cXyz::BaseZ, &mIronBallCenterPos);

        if (mItemMode == 2) {
            seStartOnlyReverb(Z2SE_LK_HS_SHOOT);
            if (mTargetedActor != NULL) {
                field_0x3028 = 1;
            } else {
                field_0x3028 = 0;
            }
        } else {
            field_0x3828 = mHeldItemRootPos;
        }

        mDoMtx_stack_c::copy(mHeldItemModel->getBaseTRMtx());

        mDoMtx_stack_c::transM(cXyz(hookRoot));
        if (mTargetedActor != NULL || mItemMode != 2) {
            mDoMtx_stack_c::multVecZero(&mHookshotTopPos);
        }

        if (mItemMode == 2) {
            mItemMode = HS_MODE_SHOOT_e;
        }
    } else {
        cXyz sp194(mHookshotTopPos);

        if (mItemMode != HS_MODE_RETURN_e && mItemMode == HS_MODE_SHOOT_e) {
            if (mAtCps[0].ChkAtHit() &&
                (mHookTargetAcKeep.getActor() != NULL || field_0x3494 > 0.0f))
            {
                if (mHookTargetAcKeep.getActor() != NULL) {
                    mHookshotTopPos = field_0x381c;

                    fopAc_ac_c* targetAc_p = mHookTargetAcKeep.getActor();
                    if (targetAc_p != NULL && (targetAc_p->actor_status & 0x280000)) {
                        mHookTargetAcKeep.setData(targetAc_p);
                        mIronBallCenterPos = mHookshotTopPos - targetAc_p->current.pos;
                        setHookshotCatchNow();

                        if (targetAc_p->actor_status & 0x200000) {
                            mItemMode = 5;
                            field_0x316c.set(field_0x301c, field_0x301e, 0);
                            mDoMtx_stack_c::ZrotS(-targetAc_p->shape_angle.z);
                            mDoMtx_stack_c::XrotM(-targetAc_p->shape_angle.x);
                            mDoMtx_stack_c::YrotM(-targetAc_p->shape_angle.y);
                            mDoMtx_stack_c::multVecSR(&mIronBallCenterPos, &mIronBallCenterPos);
                        } else {
                            mItemMode = HS_MODE_RETURN_e;
                        }

                        fopAcM_setHookCarryNow(targetAc_p);
                    } else {
                        mHookTargetAcKeep.clearData();
                        mItemMode = HS_MODE_RETURN_e;
                        dComIfGp_getVibration().StartShock(1, 1, cXyz(0.0f, 1.0f, 0.0f));
                    }
                } else {
                    mItemMode = HS_MODE_RETURN_e;
                    dComIfGp_getVibration().StartShock(1, 1, cXyz(0.0f, 1.0f, 0.0f));
                }
            } else {
                if (checkChaseHookshot()) {
                    cXyz sp188 = mTargetedActor->eyePos - mHookshotTopPos;
                    if (sp188.inprod(mIronBallCenterPos) >= 0.0f) {
                        mIronBallCenterPos = sp188;
                        mIronBallCenterPos.normalizeZP();
                        field_0x301c = mIronBallCenterPos.atan2sY_XZ();
                        field_0x301e = mIronBallCenterPos.atan2sX_Z();
                    }
                }

                mHookshotTopPos += mIronBallCenterPos * shoot_speed;

                if (checkModeFlg(0x400)) {
                    mHookshotTopPos += current.pos - field_0x3798;
                }

                cXyz sp17C = mHookshotTopPos - mHeldItemRootPos;
                f32 temp_f1 = sp17C.abs();
                sp17C *= 1.0f / temp_f1;

                if (temp_f1 >= length - 15.0f) {
                    mHookshotTopPos = mHeldItemRootPos + (sp17C * (length - 15.0f));
                    mItemMode = HS_MODE_RETURN_e;
                }

                cXyz sp170;
                if (field_0x3828.abs2(mHeldItemRootPos) > 400.0f ||
                    current.pos.abs2(field_0x3798) > 1.0f || shape_angle.y != field_0x2fe6)
                {
                    field_0x3028 = 1;
                }

                if (field_0x3028 != 0) {
                    sp170 = mHeldItemRootPos;
                } else {
                    sp170 = field_0x3828;
                }

                sp170 -= mIronBallCenterPos * 100.0f;
                cXyz sp164 = mHookshotTopPos + (mIronBallCenterPos * 15.0f);
                mRopeLinChk.Set(&sp170, &sp164, this);

                if (dComIfG_Bgsp().LineCross(&mRopeLinChk)) {
                    u32 hit_se;

                    if (checkHookshotStickBG(mRopeLinChk)) {
                        setHookshotCatchNow();
                        mItemMode = HS_MODE_FLY_e;
                        hit_se = Z2SE_HIT_HOOKSHOT_STICK;

                        if (dComIfGp_checkPlayerStatus1(0, 0x2010000) != 0 &&
                            mCargoCarryAcKeep.getActor() == NULL)
                        {
                            mPolyInfo3.SetPolyInfo(mPolyInfo2);
                        } else {
                            mPolyInfo3.ClearPi();
                        }

                        mPolyInfo2.SetPolyInfo(mRopeLinChk);
                        if (dComIfG_Bgsp().ChkMoveBG_NoDABg(mRopeLinChk)) {
                            fopAc_ac_c* target_ac = dComIfG_Bgsp().GetActorPointer(mRopeLinChk);
                            mHookTargetAcKeep.setData(target_ac);
                            fopAcM_setHookCarryNow(target_ac);
                        }
                    } else {
                        int poly_att0 = dComIfG_Bgsp().GetPolyAtt0(mRopeLinChk);
                        dComIfGp_getVibration().StartShock(1, 1, cXyz(0.0f, 1.0f, 0.0f));
                        mItemMode = HS_MODE_RETURN_e;
                        hit_se = Z2SE_HIT_HOOKSHOT_REBOUND;

                        cM3dGPla poly;
                        dComIfG_Bgsp().GetTriPla(mRopeLinChk, &poly);

                        csXyz sp30;
                        if (poly_att0 == 0xD || poly_att0 == 3) {
                            u16 particle_id;
                            if (poly_att0 == 0xD) {
                                particle_id = 0x881F;
                            } else {
                                particle_id = 0x881E;
                            }

                            sp30.set(cM_atan2s(poly.mNormal.absXZ(), poly.mNormal.y),
                                     poly.mNormal.atan2sX_Z(), 0);

                            dComIfGp_particle_setPolyColor(
                                particle_id, mRopeLinChk, mRopeLinChk.GetCrossP(), &tevStr, &sp30,
                                (cXyz*)&l_hookSnowSandHitScale, 0, NULL, -1, NULL);
                            if (poly_att0 == 0xD) {
                                dComIfGp_particle_setPolyColor(
                                    0x8820, mRopeLinChk, mRopeLinChk.GetCrossP(), &tevStr, &sp30,
                                    (cXyz*)&l_hookSnowSandHitScale, 0, NULL, -1, NULL);
                            }
                        } else {
                            sp30.set(cM_atan2s(poly.mNormal.y, poly.mNormal.absXZ()),
                                     cM_atan2s(-poly.mNormal.x, -poly.mNormal.z), 0);
                            dComIfGp_setHitMark(9, NULL, mRopeLinChk.GetCrossP(), &sp30, NULL, 0);
                        }
                    }

                    mHookshotTopPos = mRopeLinChk.GetCross() - (mIronBallCenterPos * 15.0f);

                    cM3dGPla poly;
                    dComIfG_Bgsp().GetTriPla(mRopeLinChk, &poly);
                    field_0x316c.set(cM_atan2s(poly.mNormal.y, poly.mNormal.absXZ()),
                                     cM_atan2s(-poly.mNormal.x, -poly.mNormal.z), 0);

                    mZ2Link.startHitItemSE(hit_se, dKy_pol_sound_get(&mRopeLinChk), mpHookSound,
                                           -1.0f);
                } else {
                    seStartOnlyReverbLevel(Z2SE_LK_HS_CHAIN);
                }
            }
        }

        mDoMtx_stack_c::transS(mHookshotTopPos);
        mDoMtx_stack_c::ZXYrotM(field_0x301c, field_0x301e, 0);
        mpHookshotLinChk->Set(&sp194, &mHookshotTopPos, this);

        if (dComIfG_Bgsp().LineCross(mpHookshotLinChk) &&
            dComIfG_Bgsp().GetPolyAtt0(*mpHookshotLinChk) != 6)
        {
            fopKyM_createWpillar(mpHookshotLinChk->GetCrossP(), 0.5f, 0);
            mDoAud_seStart(Z2SE_CM_BODYFALL_WATER_S, mpHookshotLinChk->GetCrossP(), 0,
                           mVoiceReverbIntensity);
        }
    }

    mHookTipBck.entry(mpHookTipModel->getModelData(), field_0x33e0);
    mpHookTipModel->setBaseTRMtx(mDoMtx_stack_c::get());
    mpHookTipModel->calc();

    f32 bck_frame;
    if (dComIfGp_checkPlayerStatus1(0, 0x10000)) {
        mDoMtx_stack_c::transS(mIronBallBgChkPos);
        mDoMtx_stack_c::ZXYrotM(-0x4000, field_0x3022, 0);
        bck_frame = 14.0f;
    } else if (dComIfGp_checkPlayerStatus1(0, 0x2000000)) {
        mDoMtx_stack_c::transS(mIronBallBgChkPos);
        mDoMtx_stack_c::ZXYrotM(0, field_0x3022, 0);
        bck_frame = 14.0f;
    } else {
        if (field_0x3024 != 0) {
            cLib_chasePos(&mIronBallBgChkPos, field_0x3810, 2.0f * return_speed);
            cXyz sp158 = mIronBallBgChkPos - field_0x3810;

            if (sp158.abs2() < 1.0f) {
                field_0x3024 = 0;
            } else {
                mDoMtx_stack_c::transS(mIronBallBgChkPos);
                mDoMtx_stack_c::ZXYrotM(sp158.atan2sY_XZ(), sp158.atan2sX_Z(), 0);
            }
        }

        if (field_0x3024 == 0) {
            mDoMtx_stack_c::copy(field_0x0710->getBaseTRMtx());

            mDoMtx_stack_c::transM(cXyz(hookRoot));

            mIronBallBgChkPos = field_0x3810;
        }
        bck_frame = 0.0f;
    }

    mHookTipBck.entry(field_0x0714->getModelData(), bck_frame);
    field_0x0714->setBaseTRMtx(mDoMtx_stack_c::get());
    field_0x0714->calc();
}

void daAlink_c::setHookshotRoofWaitAnime() {
    daAlink_ANM anm;
    if (checkHookshotRoofLv7Boss()) {
        if (field_0x3020 == 0) {
            anm = ANM_DRAGON_HANG_RIGHT;
        } else {
            anm = ANM_DRAGON_HANG_LEFT;
        }
    } else {
        if (field_0x3020 == 0) {
            anm = ANM_HOOKSHOT_HANG_RIGHT;
        } else {
            anm = ANM_HOOKSHOT_HANG_LEFT;
        }

        onModeFlg(0x100);
    }

    setSingleAnimeBaseMorf(anm, 2.0f);
}

void daAlink_c::setHookshotWallWaitAnime() {
    daAlink_ANM anm;
    if (field_0x3020 == 0) {
        anm = ANM_HOOKSHOT_WALL_RIGHT;
    } else {
        anm = ANM_HOOKSHOT_WALL_LEFT;
    }

    setSingleAnimeBaseMorf(anm, 2.0f);
    onModeFlg(0x100);
}

void daAlink_c::hookshotRoofTurn() {
    BOOL play_sound = false;
    if (!dComIfGp_checkPlayerStatus0(0, 0x2000)) {
        f32 max_rise_y = (mIronBallBgChkPos.y + 15.0f) - 1.5f;
        f32 min_descend_y = mIronBallBgChkPos.y - mpHIO->mItem.mHookshot.m.mMaxLength;

        if (checkInputOnR()) {
            int stick_direction = getDirectionFromAngle(mStickAngle);
            int angle = (int)(1024.0f * field_0x33ac * field_0x33ac);

            if (stick_direction == DIR_LEFT) {
                shape_angle.y += angle;
            } else if (stick_direction == DIR_RIGHT) {
                shape_angle.y -= angle;
            }
            current.angle.y = shape_angle.y;

            if (stick_direction == DIR_FORWARD) {
                current.pos.y += mpHIO->mItem.mHookshot.m.mRoofHangRiseSpeed * field_0x33a8;
                if (current.pos.y > max_rise_y) {
                    current.pos.y = max_rise_y;
                } else {
                    seStartOnlyReverbLevel(Z2SE_AL_HS_HANGING_UP);
                    play_sound = true;
                }
            } else if (stick_direction == DIR_BACKWARD) {
                f32 descend_speed = mpHIO->mItem.mHookshot.m.mRoofHangDecendSpeed * field_0x33ac;
                if (checkBootsOrArmorHeavy()) {
                    descend_speed *= 1.5f;
                }
                current.pos.y -= descend_speed;

                if (min_descend_y > current.pos.y) {
                    current.pos.y = min_descend_y;
                } else {
                    seStartOnlyReverbLevel(Z2SE_AL_HS_HANGING_DOWN);
                    play_sound = true;
                }
            }
        }

        u8 status_dir = 0;
        if (current.pos.y < max_rise_y) {
            status_dir |= 8;
        }

        if (current.pos.y > min_descend_y) {
            status_dir |= 2;
        }

        if (!checkEventRun()) {
            dComIfGp_set3DStatusForce(0x78, status_dir, 0);
        }
    }

    if (play_sound) {
        if (!checkNoResetFlg3(FLG3_UNK_4)) {
            dComIfGp_getVibration().StartQuake(1, 1, cXyz(0.0f, 1.0f, 0.0f));
            onNoResetFlg3(FLG3_UNK_4);
        }
    } else {
        cancelItemUseQuake(0);
    }
}


void daAlink_c::initHookshotRoofWaitActor(fopAc_ac_c* i_actor) {
    if (i_actor != NULL) {
        mCargoCarryAcKeep.setData(i_actor);
        field_0x37c8 = mIronBallBgChkPos - i_actor->current.pos;

        mDoMtx_stack_c::XrotS(-i_actor->shape_angle.x);
        mDoMtx_stack_c::YrotM(-i_actor->shape_angle.y);
        mDoMtx_stack_c::multVecSR(&field_0x37c8, &field_0x37c8);
        mProcVar0.field_0x3008 = i_actor->shape_angle.y;

        fopAcM_setHookCarryNow(i_actor);
        mProcVar4.field_0x3010 = 1;
    } else {
        mCargoCarryAcKeep.clearData();
        mProcVar4.field_0x3010 = 0;
    }
}

int daAlink_c::checkNextHookPoint() {
    if (field_0x3026 != 0) {
        fopAc_ac_c* targetAc_p = mHookTargetAcKeep.getActor();

        if ((mItemMode == 5 && targetAc_p != NULL && fopAcM_checkHookCarryNow(targetAc_p)) ||
            (mItemMode == HS_MODE_FLY_e && dComIfG_Bgsp().ChkPolySafe(mPolyInfo2) &&
             checkHookshotStickBG(mPolyInfo2) &&
             (targetAc_p == NULL || fopAcM_checkHookCarryNow(targetAc_p))))
        {
            field_0x3026 = 0;
            return procHookshotFlyInit();
        }
    } else if (dComIfGp_checkPlayerStatus1(0, 0x10000) && checkHookshotRoofLv7Boss()) {
        if (!fopAcM_checkHookCarryNow(mCargoCarryAcKeep.getActor())) {
            field_0x3102 = shape_angle.y;
            return procCoLargeDamageInit(-6, 1, 0, 0, NULL, 0);
        }
    }

    return procFallInit(1, 5.0f);
}

int daAlink_c::checkLandHookshotHang() {
    mLinkGndChk.SetPos(&field_0x3834);

    f32 ground_cross = dComIfG_Bgsp().GroundCross(&mLinkGndChk);
    if (!setHookshotHangMoveBGCollect() || ground_cross > mLeftFootPos.y ||
        ground_cross > mRightFootPos.y)
    {
        return checkNextHookPoint();
    }

    if (mProcID != PROC_HOOKSHOT_ROOF_BOOTS &&
        ((mProcID != PROC_HOOKSHOT_ROOF_SHOOT && mProcID != PROC_HOOKSHOT_WALL_SHOOT) ||
         checkHookshotWait()) &&
        (!checkHookshotRoofLv7Boss() || !checkEquipHeavyBoots()) &&
        checkSetItemTrigger(fpcNm_ITEM_HVY_BOOTS))
    {
        if (dComIfGp_checkPlayerStatus1(0, 0x10000)) {
            return procHookshotRoofBootsInit(mCargoCarryAcKeep.getActor());
        }
        setHeavyBoots(1);
    }

    return 0;
}

int daAlink_c::commonHookshotRoofWait() {
    setJumpMode();
    if (mProcVar4.field_0x3010 != 0) {
        mCargoCarryAcKeep.setActor();
        fopAc_ac_c* carryAc_p = mCargoCarryAcKeep.getActor();

        if (carryAc_p == NULL || !fopAcM_checkHookCarryNow(carryAc_p)) {
            return checkNextHookPoint();
        }
    }

    if (mLinkAcch.ChkWallHit()) {
        return checkNextHookPoint();
    }

    cXyz start_pos = field_0x3810 + (current.pos - field_0x3798);
    mRopeLinChk.Set(&start_pos, &mIronBallBgChkPos, this);

    if (dComIfG_Bgsp().LineCross(&mRopeLinChk)) {
        return checkNextHookPoint();
    }

    if (checkLandHookshotHang()) {
        return 1;
    }

    return 0;
}

int daAlink_c::commonHookshotWallWait() {
    setJumpMode();
    return checkLandHookshotHang() ? TRUE : FALSE;
}

int daAlink_c::procHookshotSubjectInit() {
    if (!commonProcInitNotSameProc(PROC_HOOKSHOT_SUBJECT)) {
        return 0;
    }

    mNormalSpeed = 0.0f;

    if (initHookshotUpperAnimeSpeed(1)) {
        setSingleAnimeBaseSpeed(ANM_ATN_WAIT_LEFT, 0.0f,
                                mpHIO->mItem.mHookshot.m.mStartInterpolation);
    }

    dComIfGp_setPlayerStatus0(0, 0x4000);
    current.angle.y = shape_angle.y;
    setHookshotReadyMaterial();
    return 1;
}


int daAlink_c::procHookshotSubject() {
    if (checkHookshotWait()) {
        setDoStatus(0x12);
    }

    setShapeAngleToAtnActor(0);
    mSight.offDrawFlg();

    if (!checkNextAction(0)) {
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

int daAlink_c::procHookshotMoveInit() {
    if (!commonProcInitNotSameProc(PROC_HOOKSHOT_MOVE)) {
        return 0;
    }

    if (checkZeroSpeedF()) {
        onModeFlg(1);
    }

    if (initHookshotUpperAnimeSpeed(0)) {
        setBlendAtnMoveAnime(mpHIO->mItem.mHookshot.m.mStartInterpolation);
    }

    dComIfGp_setPlayerStatus0(0, 0x4000);
    return 1;
}

int daAlink_c::procHookshotMove() {
    cancelHookshotMove();

    if (!checkNextAction(0)) {
        f32 blend = -1.0f;

        if (checkZeroSpeedF()) {
            onModeFlg(1);

            if (field_0x2f98 != 2) {
                field_0x2f98 = 2;
                blend = mpHIO->mBasic.m.mBasicInterpolation;
            }
        } else {
            offModeFlg(1);
        }

        if (checkHookshotWait()) {
            setBlendAtnMoveAnime(blend);
            setBodyAngleXReadyAnime(0);
        } else if (checkChaseHookshot()) {
            setBodyAngleXReadyAnime(0);
        }
    }

    return 1;
}

int daAlink_c::procHookshotFlyInit() {
    commonProcInit(PROC_HOOKSHOT_FLY);

    if (!checkNoResetFlg0(FLG0_SWIM_UP)) {
        onNoResetFlg0(FLG0_UNDERWATER);
    }

    daAlink_ANM anm;
    if (field_0x3020 == 0) {
        anm = ANM_HOOKSHOT_FLY_LEFT;
    } else {
        anm = ANM_HOOKSHOT_FLY_RIGHT;
    }

    setSingleAnimeBase(anm);
    mBodyAngle.x = 0;
    setSpecialGravity(0.0f, maxFallSpeed, 0);
    mNormalSpeed = 0.0f;
    speed.y = 0.0f;

    dComIfGp_clearPlayerStatus0(0, 0x40000);
    dComIfGp_setPlayerStatus1(0, 0x10);

    voiceStart(Z2SE_AL_V_JUMP_L);

    field_0x37c8 = field_0x3798;
    current.pos = field_0x3834;
    field_0x37d4 = cXyz::Zero;

    shape_angle.x = field_0x301c;
    shape_angle.y = field_0x301e;
    current.angle.y = shape_angle.y;

    mProcVar0.field_0x3008 = 1;
    mProcVar1.field_0x300a = 4;
    field_0x2f99 = 0x50;
    mProcVar2.field_0x300c = mItemMode;
    mProcVar5.field_0x3012 = 0;

    procHookshotFly();
    return 1;
}


int daAlink_c::procHookshotFly() {
    fopAc_ac_c* targetAc_p = mHookTargetAcKeep.getActor();

    s16 targetAc_name;
    if (targetAc_p != NULL) {
        targetAc_name = fopAcM_GetName(targetAc_p);
    } else {
        targetAc_name = PROC_ALINK;
    }

    BOOL var_r29 = 0;
    if (targetAc_name == PROC_Obj_SwHang) {
        int swhang_type = static_cast<daObjSwHang_c*>(targetAc_p)->getType();
        if (swhang_type == 3 || swhang_type == 4) {
            var_r29 = 1;
        }
    }

    s16 temp_r24 = field_0x301e;
    cXyz spAC(mHookshotTopPos);
    setHookshotTopPosFly();

    field_0x37d4 = mHookshotTopPos - mHeldItemRootPos;
    if (mProcVar0.field_0x3008 != 0 && cLib_distanceAngleS(field_0x37d4.atan2sX_Z(), temp_r24) > 0x4000) {
        setHookshotReturnEnd();
    } else {
        mProcVar0.field_0x3008 = 0;
        f32 temp_f31 = field_0x37d4.abs();
        f32 temp_f30 = mpHIO->mItem.mHookshot.m.mStickReturnSpeed + spAC.abs(mHookshotTopPos);

        if (temp_f31 < temp_f30 || mProcVar1.field_0x300a == 0) {
            setHookshotReturnEnd();
        } else {
            field_0x37d4 *= temp_f30 / temp_f31;
            seStartOnlyReverbLevel(Z2SE_LK_HS_WIND_UP);
            if (temp_f31 < temp_f30 * 1.5f) {
                mProcVar1.field_0x300a--;
            }
        }
    }

    current.pos += field_0x37d4;
    if (checkSetItemTrigger(fpcNm_ITEM_W_HOOKSHOT) != 0) {
        mProcVar5.field_0x3012 = 1;
    } else if (mProcVar5.field_0x3012 != 0 && !itemButton()) {
        mProcVar5.field_0x3012 = 0;
    }

    if (mItemMode != 5 && mItemMode != HS_MODE_FLY_e) {
        if (targetAc_name == PROC_B_OB ||
            (targetAc_name == PROC_B_DR && static_cast<daB_DR_c*>(targetAc_p)->isBack()))
        {
            dComIfGp_getVibration().StartShock(1, 1, cXyz(0.0f, 1.0f, 0.0f));
            return procBossBodyHangInit(targetAc_p);
        } else {
            cM3dGPla poly;
            BOOL var_r28 = 0;
            BOOL var_r27 = 0;
            BOOL force_fall = checkStageName("D_MN10") && fopAcM_GetRoomNo(this) == 4;

            if (mProcVar2.field_0x300c == 4 && dComIfG_Bgsp().ChkPolySafe(mPolyInfo2)) {
                var_r28 = dComIfG_Bgsp().GetTriPla(mPolyInfo2, &poly);
                var_r27 = cBgW_CheckBRoof(poly.mNormal.y);
                if (!checkHookshotStickBG(mPolyInfo2)) {
                    var_r28 = 0;
                }
            }

            if (!force_fall && !mLinkAcch.ChkGroundHit() && !var_r27 && checkFrontWallTypeAction())
            {
                voiceStart(Z2SE_AL_V_CLIMB);
                dComIfGp_getVibration().StartShock(1, 1, cXyz(0.0f, 1.0f, 0.0f));
                return 1;
            } else {
                cXyz spA0 = current.pos - field_0x37c8;
                if (commonLineCheck(&field_0x37c8, &current.pos)) {
                    current.pos = mLinkLinChk.GetCross();

                    spA0.y = 0.0f;
                    spA0.normalizeZP();

                    current.pos.x -= spA0.x * 35.0f;
                    current.pos.z -= spA0.z * 35.0f;
                }

                if (var_r28 && !var_r27) {
                    cXyz sp94(mHookshotTopPos.x + poly.mNormal.x * 35.0f, mHookshotTopPos.y + 5.0f,
                              mHookshotTopPos.z + poly.mNormal.z * 35.0f);
                    mLinkGndChk.SetPos(&sp94);

                    if (dComIfG_Bgsp().GroundCross(&mLinkGndChk) > mHookshotTopPos.y - 150.0f ||
                        force_fall)
                    {
                        current.pos.x = sp94.x;
                        current.pos.z = sp94.z;
                        force_fall = true;
                    }
                }

                setJumpMode();
                if (mLinkAcch.ChkGroundHit()) {
                    checkNextAction(0);
                } else if (force_fall) {
                    procFallInit(1, 5.0f);
                    field_0x2f99 = 0x70;
                } else if (targetAc_name == PROC_E_PH || targetAc_name == PROC_B_DR || var_r29) {
                    procHookshotRoofWaitInit(1, targetAc_p, mProcVar5.field_0x3012);
                } else if (var_r28 && dComIfG_Bgsp().GetMonkeyBarsCode(mPolyInfo2)) {
                    cXyz sp88;
                    mDoMtx_stack_c::ZXYrotS(field_0x301c, field_0x301e, 0);
                    mDoMtx_stack_c::multVec(&cXyz::BaseZ, &sp88);
                    sp88 = mHookshotTopPos + (sp88 * 15.0f);

                    procRoofHangStartInit(mPolyInfo2, sp88, 0);
                } else if (var_r28 && var_r27) {
                    procHookshotRoofWaitInit(1, NULL, mProcVar5.field_0x3012);
                } else if (var_r28 && field_0x2f91 != 3 && fabsf(poly.mNormal.y) < 0.05f) {
                    procHookshotWallWaitInit(1, poly.mNormal.atan2sX_Z(), mProcVar5.field_0x3012);
                } else {
                    procFallInit(1, 5.0f);
                    field_0x2f99 = 0x70;
                }

                voiceStart(Z2SE_AL_V_CLIMB);
                if (mProcID != PROC_FALL) {
                    dComIfGp_getVibration().StartShock(1, 1, cXyz(0.0f, 1.0f, 0.0f));
                }
            }
        }
    } else {
        cXyz sp7C = mHookshotTopPos - current.pos;
        cLib_addCalcAngleS(&shape_angle.x, sp7C.atan2sY_XZ(), 2, 0x2000, 0x800);
        cLib_addCalcAngleS(&shape_angle.y, sp7C.atan2sX_Z(), 2, 0x2000, 0x800);
        current.angle.y = shape_angle.y;
        field_0x37c8 = field_0x3798;
    }

    return 1;
}


int daAlink_c::procHookshotRoofWaitInit(int param_0, fopAc_ac_c* param_1, int param_2) {
    commonProcInit(PROC_HOOKSHOT_ROOF_WAIT);

    if (param_0) {
        cXyz sp28;
        mDoMtx_stack_c::ZXYrotS(field_0x301c, field_0x301e, 0);
        mDoMtx_stack_c::multVec(&cXyz::BaseZ, &sp28);
        current.pos = mHookshotTopPos + (sp28 * 15.0f);
        current.pos.y -= 1.5f;

        field_0x3022 = field_0x301e;
        mIronBallBgChkPos.set(current.pos.x, current.pos.y - 15.0f, current.pos.z);

        daAlink_ANM anm;
        if (field_0x3020 == 0) {
            field_0x3020 = 1;
            anm = ANM_HOOKSHOT_HANG_END_LEFT;
        } else {
            field_0x3020 = 0;
            anm = ANM_HOOKSHOT_HANG_END_RIGHT;
        }

        mProcVar2.field_0x300c = 0;
        setSingleAnimeParam(anm, &mpHIO->mItem.mHookshot.m.mRoofHangAnm);
    } else {
        setHookshotRoofWaitAnime();
        mProcVar2.field_0x300c = 1;
    }

    resetHookshotMode();
    field_0x3024 = 1;
    setSpecialGravity(0.0f, maxFallSpeed, 0);
    mNormalSpeed = 0.0f;
    speed.y = 0.0f;
    current.angle.y = shape_angle.y;

    initHookshotRoofWaitActor(param_1);
    if (param_0) {
        setHookshotHangMoveBGCollect();
    }

    field_0x2b98 = 6.5f;
    mProcVar5.field_0x3012 = param_2;

    dComIfGp_setPlayerStatus1(0, 0x10000);
    field_0x814.SetWeight(255);
    return 1;
}


int daAlink_c::procHookshotRoofWait() {
    if (dComIfGp_checkCameraAttentionStatus(field_0x317c, 0x80)) {
        if (checkSubjectEnd(1)) {
            dComIfGp_clearPlayerStatus0(0, 0x2000);
            mBodyAngle.y = 0;
            mBodyAngle.x = 0;
            offModeFlg(0x60000000);
        } else if (dComIfGp_checkPlayerStatus0(0, 0x2000)) {
            setBodyAngleToCamera();
        }
    } else if (dCam_getBody()->ChangeModeOK(4)) {
        onResetFlg0(RFLG0_UNK_4000000);

        if (dComIfGp_checkCameraAttentionStatus(field_0x317c, 0x1000)) {
            field_0x310c = shape_angle.y;
            field_0x310a = 0;
            setSubjectMode();
            onModeFlg(0x60000000);
        }
    }

    if (commonHookshotRoofWait()) {
        return 1;
    }

    if (!checkHookshotRoofLv7Boss() || !checkBootsOrArmorHeavy()) {
        setDoStatusEmphasys(1);

        if (doTrigger()) {
            return procFallInit(1, 5.0f);
        }
    }

    setShapeAngleToAtnActor(0);

    if (mProcVar2.field_0x300c == 0) {
        if (checkSetItemTrigger(fpcNm_ITEM_W_HOOKSHOT)) {
            mProcVar5.field_0x3012 = 1;
        } else if (mProcVar5.field_0x3012 != 0 && !itemButton()) {
            mProcVar5.field_0x3012 = 0;
        }

        daPy_frameCtrl_c* frameCtrl_p = mUnderFrameCtrl;
        if (frameCtrl_p->checkAnmEnd() ||
            ((checkInputOnR() || mProcVar5.field_0x3012 != 0 || checkHookshotRoofLv7Boss()) &&
             frameCtrl_p->getFrame() > mpHIO->mItem.mHookshot.m.mRoofHangAnm.mCancelFrame))
        {
            setHookshotRoofWaitAnime();
            mProcVar2.field_0x300c = 1;
        }
    } else if (!checkHookshotRoofLv7Boss()) {
        if (checkSetItemTrigger(fpcNm_ITEM_W_HOOKSHOT) || mProcVar5.field_0x3012 != 0) {
            return procHookshotRoofShootInit(mCargoCarryAcKeep.getActor());
        }
        hookshotRoofTurn();
    }

    return 1;
}


int daAlink_c::procHookshotRoofShootInit(fopAc_ac_c* param_0) {
    commonProcInit(PROC_HOOKSHOT_ROOF_SHOOT);

    daAlink_ANM anm;
    if (field_0x3020 == 0) {
        anm = ANM_HOOKSHOT_HANG_WAIT_RIGHT;
    } else {
        anm = ANM_HOOKSHOT_HANG_WAIT_LEFT;
    }

    setSingleAnimeBaseSpeed(anm, 1.0f, 3.0f);
    setFastShotTimer();
    setSpecialGravity(0.0f, maxFallSpeed, 0);
    mNormalSpeed = 0.0f;
    speed.y = 0.0f;
    field_0x2b98 = 6.5f;

    initHookshotRoofWaitActor(param_0);
    initHookshotReady();

    dComIfGp_setPlayerStatus1(0, 0x10000);
    dComIfGp_setPlayerStatus0(0, 0x4000);

    field_0x814.SetWeight(255);
    mProcVar2.field_0x300c = 0;
    mProcVar3.field_0x300e = mBodyAngle.x;
    return 1;
}


int daAlink_c::procHookshotRoofShoot() {
    if (checkHookshotWait()) {
        setDoStatus(0x12);
    }

    if (commonHookshotRoofWait()) {
        return 1;
    }

    if (checkHookshotWait()) {
        setShapeAngleToAtnActor(0);
    }

    mSight.offDrawFlg();
    dComIfGp_clearPlayerStatus0(0, 0x40000);

    if (mFastShotTime != 0 && checkHookshotWait()) {
        mFastShotTime--;
    }

    if (!checkUpperItemActionHookshot()) {
        mBodyAngle.x = mProcVar3.field_0x300e;

        if (!checkAttentionLock() && mFastShotTime == 0) {
            if (checkHookshotWait()) {
                mUnderFrameCtrl[0].setRate(0.0f);
                mUnderFrameCtrl[0].setFrame(0.0f);
                getNowAnmPackUnder(UNDER_0)->setFrame(0.0f);

                if (mProcVar2.field_0x300c != 0 && setBodyAngleToCamera()) {
                    setHookshotSight();
                }
            } else {
                dComIfGp_setPlayerStatus0(0, 0x40000);
            }

            mProcVar2.field_0x300c = 1;
        } else {
            if (mTargetedActor == NULL && mItemMode == HS_MODE_NONE_e) {
                return procHookshotRoofWaitInit(0, mCargoCarryAcKeep.getActor(), 0);
            }

            if (checkHookshotWait()) {
                mUnderFrameCtrl[0].setRate(1.0f);

                if (mProcVar2.field_0x300c == 0) {
                    setBodyAngleXReadyAnime(0);
                    hookshotRoofTurn();
                } else {
                    cancelItemUseQuake(0);
                }
            }

            mProcVar2.field_0x300c = 0;
        }

        mProcVar3.field_0x300e = mBodyAngle.x;
        mBodyAngle.x = 0;
    }

    return 1;
}


int daAlink_c::procHookshotRoofBootsInit(fopAc_ac_c* param_0) {
    commonProcInit(PROC_HOOKSHOT_ROOF_BOOTS);

    daAlink_ANM anm;
    if (field_0x3020 == 0) {
        anm = ANM_HOOKSHOT_HANG_RIGHT_START;
    } else {
        anm = ANM_HOOKSHOT_HANG_LEFT_START;
    }

    setSingleAnimeBaseMorf(anm, 2.0f);
    resetHookshotMode();
    field_0x3024 = 1;
    setSpecialGravity(0.0f, maxFallSpeed, 0);
    mNormalSpeed = 0.0f;
    speed.y = 0.0f;
    current.angle.y = shape_angle.y;
    initHookshotRoofWaitActor(param_0);
    field_0x2b98 = 6.5f;

    dComIfGp_setPlayerStatus1(0, 0x10000);

    field_0x814.SetWeight(255);
    return 1;
}


int daAlink_c::procHookshotRoofBoots() {
    if (commonHookshotRoofWait()) {
        return 1;
    }

    if (!checkHookshotRoofLv7Boss() || !checkBootsOrArmorHeavy()) {
        setDoStatusEmphasys(1);

        if (doTrigger()) {
            return procFallInit(1, 5.0f);
        }
    }

    daPy_frameCtrl_c* frameCtrl_p = mUnderFrameCtrl;
    if (frameCtrl_p->checkAnmEnd()) {
        return procHookshotRoofWaitInit(0, mCargoCarryAcKeep.getActor(), 0);
    }

    if (frameCtrl_p->checkPass(4.0f)) {
        setHeavyBoots(1);
    } else if (checkNoResetFlg0(FLG0_EQUIP_HVY_BOOTS) && frameCtrl_p->checkPass(10.0f)) {
        dComIfGp_getVibration().StartShock(3, 1, cXyz(0.0f, 1.0f, 0.0f));
    }

    return 1;
}


int daAlink_c::procHookshotWallWaitInit(int param_0, s16 param_1, int param_2) {
    commonProcInit(PROC_HOOKSHOT_WALL_WAIT);

    if (param_0) {
        cXyz sp28;
        setOldRootQuaternion(0, -0x8000, 0);
        mDoMtx_stack_c::ZXYrotS(field_0x301c, field_0x301e, 0);
        mDoMtx_stack_c::multVec(&cXyz::BaseZ, &sp28);
        current.pos = mHookshotTopPos + (sp28 * 15.0f);
        shape_angle.y = param_1;

        current.pos.x += cM_ssin(shape_angle.y) * 1.5f;
        current.pos.z += cM_scos(shape_angle.y) * 1.5f;

        field_0x3022 = param_1 + 0x8000;
        mIronBallBgChkPos.set(current.pos.x + cM_ssin(shape_angle.y) * 15.0f, current.pos.y,
                              current.pos.z + cM_scos(shape_angle.y) * 15.0f);

        daAlink_ANM anm;
        if (field_0x3020 == 0) {
            field_0x3020 = 1;
            anm = ANM_HOOKSHOT_WALL_END_LEFT;
        } else {
            field_0x3020 = 0;
            anm = ANM_HOOKSHOT_WALL_END_RIGHT;
        }

        mProcVar2.field_0x300c = 0;
        setSingleAnimeParam(anm, &mpHIO->mItem.mHookshot.m.mWallHangAnm);
    } else {
        setHookshotWallWaitAnime();
        mProcVar2.field_0x300c = 1;
    }

    dComIfGp_setPlayerStatus1(0, 0x2000000);
    resetHookshotMode();
    field_0x3024 = 1;
    setSpecialGravity(0.0f, maxFallSpeed, 0);
    mNormalSpeed = 0.0f;
    speed.y = 0.0f;
    current.angle.y = shape_angle.y;

    if (param_0) {
        setHookshotHangMoveBGCollect();
    }

    mProcVar5.field_0x3012 = param_2;
    return 1;
}


int daAlink_c::procHookshotWallWait() {
    if (dComIfGp_checkCameraAttentionStatus(field_0x317c, 0x80)) {
        if (checkSubjectEnd(1)) {
            dComIfGp_clearPlayerStatus0(0, 0x2000);
            mBodyAngle.x = 0;
            offModeFlg(0x60000000);
        } else if (dComIfGp_checkPlayerStatus0(0, 0x2000)) {
            s16 old_angle = shape_angle.y;
            shape_angle.y = field_0x310c;
            setBodyAngleToCamera();

            shape_angle.y = old_angle;

            if ((s16)(field_0x310c - shape_angle.y) > 0x4000) {
                field_0x310c = shape_angle.y + 0x4000;
            } else if ((s16)(field_0x310c - shape_angle.y) < -0x4000) {
                field_0x310c = shape_angle.y - 0x4000;
            }
        }
    } else if (dCam_getBody()->ChangeModeOK(4)) {
        onResetFlg0(RFLG0_UNK_4000000);

        if (dComIfGp_checkCameraAttentionStatus(field_0x317c, 0x1000)) {
            field_0x310c = shape_angle.y;
            field_0x310a = 0;
            setSubjectMode();
            onModeFlg(0x60000000);
        }
    }

    if (commonHookshotWallWait()) {
        return 1;
    }

    setDoStatusEmphasys(1);

    if (doTrigger()) {
        return procFallInit(1, 5.0f);
    }

    if (mProcVar2.field_0x300c == 0) {
        if (checkSetItemTrigger(fpcNm_ITEM_W_HOOKSHOT)) {
            mProcVar5.field_0x3012 = 1;
        } else if (mProcVar5.field_0x3012 != 0 && !itemButton()) {
            mProcVar5.field_0x3012 = 0;
        }

        daPy_frameCtrl_c* frameCtrl_p = mUnderFrameCtrl;
        if (frameCtrl_p->checkAnmEnd() ||
            ((checkInputOnR() || mProcVar5.field_0x3012 != 0) &&
             frameCtrl_p->getFrame() > mpHIO->mItem.mHookshot.m.mWallHangAnm.mCancelFrame))
        {
            setHookshotWallWaitAnime();
            mProcVar2.field_0x300c = 1;
        }
    } else if (checkSetItemTrigger(fpcNm_ITEM_W_HOOKSHOT) || mProcVar5.field_0x3012 != 0) {
        return procHookshotWallShootInit();
    }

    return 1;
}


int daAlink_c::procHookshotWallShootInit() {
    commonProcInit(PROC_HOOKSHOT_WALL_SHOOT);

    daAlink_ANM anm;
    if (field_0x3020 == 0) {
        anm = ANM_HOOKSHOT_WALL_WAIT_RIGHT;
    } else {
        anm = ANM_HOOKSHOT_WALL_WAIT_LEFT;
    }

    setSingleAnimeBaseSpeed(anm, 1.0f, 3.0f);
    setFastShotTimer();
    setSpecialGravity(0.0f, maxFallSpeed, 0);
    mNormalSpeed = 0.0f;
    speed.y = 0.0f;

    dComIfGp_setPlayerStatus1(0, 0x2000000);
    initHookshotReady();
    dComIfGp_setPlayerStatus0(0, 0x4000);

    mProcVar2.field_0x300c = 0;
    mProcVar3.field_0x300e = mBodyAngle.x;
    mProcVar4.field_0x3010 = shape_angle.y;
    field_0x310c = mProcVar4.field_0x3010;
    setHookshotReadyMaterial();
    return 1;
}


int daAlink_c::procHookshotWallShoot() {
    if (checkHookshotWait()) {
        setDoStatus(0x12);
    }

    if (commonHookshotWallWait()) {
        return 1;
    }

    mSight.offDrawFlg();
    dComIfGp_clearPlayerStatus0(0, 0x40000);

    if (mFastShotTime != 0 && checkHookshotWait()) {
        mFastShotTime--;
    }

    BOOL var_r31 = false;
    if (!checkUpperItemActionHookshot()) {
        s16 old_angle = shape_angle.y;
        mBodyAngle.x = mProcVar3.field_0x300e;
        shape_angle.y = mProcVar4.field_0x3010;

        if (!checkAttentionLock() && mFastShotTime == 0) {
            if (checkHookshotWait()) {
                mUnderFrameCtrl[0].setRate(0.0f);
                mUnderFrameCtrl[0].setFrame(0.0f);
                getNowAnmPackUnder(UNDER_0)->setFrame(0.0f);

                if (mProcVar2.field_0x300c != 0 && setBodyAngleToCamera()) {
                    var_r31 = true;
                }
            } else {
                dComIfGp_setPlayerStatus0(0, 0x40000);
            }

            mProcVar2.field_0x300c = 1;
        } else {
            if (mTargetedActor == NULL && mItemMode == HS_MODE_NONE_e) {
                shape_angle.y = old_angle;
                return procHookshotWallWaitInit(0, 0, 0);
            }

            if (checkHookshotWait()) {
                setWaterInAnmRate(mUnderFrameCtrl, 1.0f);

                if (mProcVar2.field_0x300c == 0) {
                    setBodyAngleXReadyAnime(0);

                    if (!setShapeAngleToAtnActor(0)) {
                        cLib_addCalcAngleS(&shape_angle.y, old_angle, 4, 0xC00, 0x180);
                    }
                }
            }

            mProcVar2.field_0x300c = 0;
        }

        if ((s16)(shape_angle.y - old_angle) > 0x4000) {
            shape_angle.y = old_angle + 0x4000;
        } else if ((s16)(shape_angle.y - old_angle) < -0x4000) {
            shape_angle.y = old_angle - 0x4000;
        }

        if (var_r31) {
            setHookshotSight();
        }

        mProcVar3.field_0x300e = mBodyAngle.x;
        mBodyAngle.x = 0;
        field_0x310c = shape_angle.y;
        mProcVar4.field_0x3010 = shape_angle.y;
        shape_angle.y = old_angle;
    }

    return 1;
}
