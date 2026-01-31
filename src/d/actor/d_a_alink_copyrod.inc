/**
 * d_a_alink_copyrod.inc
 * Player Dominion Rod action handling
 */

#include "d/actor/d_a_alink.h"
#include "d/actor/d_a_crod.h"
#include "d/actor/d_a_cstatue.h"

BOOL daAlink_c::checkLv6BossRoom() {
    return checkStageName("D_MN06A");
}

f32 daAlink_c::getCopyRodBallSpeed() const {
    return mpHIO->mItem.mCopyRod.m.mBallSpeed;
}

f32 daAlink_c::getCopyRodBallReturnSpeed() const {
    return mpHIO->mItem.mCopyRod.m.mBallReturnSpeed;
}

f32 daAlink_c::getCopyRodBallDisMax() const {
    if (checkLv6BossRoom()) {
        return mpHIO->mItem.mCopyRod.m.mBossBallMaxDistance;
    }

    return mpHIO->mItem.mCopyRod.m.mBallMaxDistance;
}

fopAc_ac_c* daAlink_c::getCopyRodControllActor() {
    if (mCopyRodAcKeep.getActor() != NULL) {
        return ((daCrod_c*)mCopyRodAcKeep.getActor())->getControllActor();
    }

    return NULL;
}

fopAc_ac_c* daAlink_c::getCopyRodCameraActor() {
    if (mCopyRodAcKeep.getActor() != NULL) {
        return ((daCrod_c*)mCopyRodAcKeep.getActor())->getCameraActor();
    }

    return NULL;
}

void daAlink_c::initCopyRodUpperAnimeSpeed(BOOL param_0) {
    if (!checkCopyRodThrowAnime()) {
        if (param_0) {
            mUpperFrameCtrl[2].setRate(0.0f);
            mUpperFrameCtrl[2].setFrame(0.0f);
            getNowAnmPackUpper(UPPER_2)->setFrame(0.0f);
        } else {
            mUpperFrameCtrl[2].setRate(mpHIO->mItem.mBoomerang.m.mIdleAnmSpeed);
        }
    }
}

BOOL daAlink_c::checkForestOldCentury() {
    return checkStageName("F_SP117") && dComIfGp_roomControl_getStayNo() == 2;
}

BOOL daAlink_c::checkCopyRodTopUse() {
    return (dComIfGs_isEventBit(dSv_event_flag_c::F_0302) && !(mProcID == PROC_UNEQUIP && mProcVar3.field_0x300e != 0))
            || dStage_stagInfo_GetSaveTbl(dComIfGp_getStage()->getStagInfo()) == dStage_SaveTbl_LV6
            || checkForestOldCentury();
}

BOOL daAlink_c::checkCopyRodAnime() const {
    return checkCopyRodReadyAnime() || checkCopyRodThrowAnime();
}

void daAlink_c::setCopyRodControllAnime() {
    if (checkNoUpperAnime() && getCopyRodControllActor()) {
        setUpperAnimeBaseSpeed(dRes_ID_ALANM_BCK_RODD_e, 0.0f, 3.0f);
        field_0x2f96 = 2;
        field_0x2f97 = 5;
        field_0x33e8 = 0.0f;
    }
}

void daAlink_c::setCopyRodControllUpperSpeedRate() {
    f32 max_speed;
    if (checkAttentionLock()) {
        max_speed = 1.0f;
    } else {
        if (mProcID == PROC_STEP_MOVE) {
            max_speed = fabsf(field_0x3478 / mpHIO->mMove.m.mMaxSpeed);
        } else {
            max_speed = fabsf(mNormalSpeed / mpHIO->mMove.m.mMaxSpeed);
        }

        if (checkHeavyStateOn(TRUE, TRUE)) {
            max_speed *= 1.0f / (mHeavySpeedMultiplier * mHeavySpeedMultiplier);
        }
    }

    if (max_speed > 1.0f) {
        max_speed = 1.0f;
    }

    cLib_chaseF(&field_0x33e8, max_speed, 0.15f);

    daPy_frameCtrl_c* frame_ctrl = &mUpperFrameCtrl[2];
    frame_ctrl->setFrame(field_0x33e8 * frame_ctrl->getEnd());
    getNowAnmPackUpper(UPPER_2)->setFrame(frame_ctrl->getFrame());
}

void daAlink_c::setCopyRodModel() {
    JKRHeap* heap = setItemHeap();
    mHeldItemModel = initModel(loadAramBmd(dRes_ID_ALANM_BMD_AL_CROD_e, 0x5400), 0x1000000);
    field_0x0724 = loadAramItemBrk(dRes_ID_ALANM_BRK_AL_CROD_CHANGE_COLOR_e, mHeldItemModel);
    mDoExt_setCurrentHeap(heap);

    field_0x0724->setFrame(0.0f);
    field_0x2f94 = 2;

    mAtCps[0].SetAtSpl(dCcG_At_Spl_UNK_0);
    mAtCps[0].SetAtAtp(0);
    mAtCps[0].SetAtSe(dCcD_SE_COPYROD);
    mAtCps[0].SetAtHitMark(1);
    mAtCps[0].OnAtSetBit();
    mAtCps[0].SetAtHitCallback(NULL);
    mAtCps[0].SetAtMtrl(dCcD_MTRL_NONE);

    mHeldItemModel->setBaseTRMtx(mpLinkModel->getAnmMtx(mLeftItemJntNo));
    mHeldItemModel->calc();
}

void daAlink_c::setCopyRodReadyAnime() {
    f32 speed;
    if (!checkAttentionLock()) {
        speed = 0.0f;
    } else {
        speed = mpHIO->mItem.mBoomerang.m.mIdleAnmSpeed;
    }

    setUpperAnimeBaseSpeed(dRes_ID_ALANM_BCK_BOOMWAIT_e, speed, 3.0f);

    if (checkCopyRodTopUse()) {
        seStartSwordCut(Z2SE_AL_COPYROD_READY);
    } else {
        seStartSwordCut(Z2SE_AL_COPYROD_READY_OFF);
    }

    mAtCps[0].SetR(20.0f);
    mAtCps[0].SetAtType(0x1000000);
    initLockAt();
}

void daAlink_c::throwCopyRod() {
    daCrod_c* copy_rod = ((daCrod_c*)mItemAcKeep.getActor());
    onNoResetFlg1(FLG1_UNK_10000);

    if (checkCopyRodTopUse()) {
        copy_rod->setThrow();
        mCopyRodAcKeep = mItemAcKeep;
        onNoResetFlg3(FLG3_COPY_ROD_THROW_AFTER);
        mItemAcKeep.clearData();
    }

    dComIfGp_clearPlayerStatus0(0, 0x80);
    mFastShotTime = -1;
    voiceStart(Z2SE_AL_V_ATTACK_S);
    seStartSwordCut(Z2SE_AL_COPYROD_SWING);

    if (mLinkAcch.ChkGroundHit()) {
        if (checkZeroSpeedF()) {
            field_0x2f98 = 3;
            procAtnActorWaitInit();
        } else {
            procAtnActorMoveInit();
        }
        field_0x2060->initOldFrameMorf(0.0f, 0, 35);
    }
}

int daAlink_c::returnCopyRod() {
    offNoResetFlg3(FLG3_COPY_ROD_THROW_AFTER);

    BOOL rt;
    if (mEquipItem == fpcNm_ITEM_COPY_ROD) {
        mItemAcKeep = mCopyRodAcKeep;
        onNoResetFlg0(FLG0_UNK_20);
        rt = TRUE;

        dComIfGp_getVibration().StartShock(VIBMODE_S_POWER1, 1, cXyz(0.0f, 1.0f, 0.0f));
    } else {
        rt = FALSE;
    }

    mCopyRodAcKeep.clearData();
    return rt;
}

BOOL daAlink_c::checkUpperItemActionCopyRod() {
    if (mItemVar0.field_0x3018 != 0) {
        mItemVar0.field_0x3018--;
    }

    if (checkReadyItem() && !itemButton() && checkCopyRodReadyAnime()) {
        setUpperAnimeParam(0x53, UPPER_2, &mpHIO->mItem.mBoomerang.m.mThrowAnm);
        mSearchBallScale = getCopyRodBallDisMax();

        if (!mSight.getLockFlg()) {
            mSearchBallScale -= 500.0f;
        }

        return true;
    }

    return cancelUpperItemReadyAnime(0);
}

void daAlink_c::checkUpperItemActionCopyRodFly() {
    if (mCopyRodAcKeep.getActor() == NULL) {
        if (checkCopyRodReadyAnime()) {
            checkUpperItemActionCopyRod();
        } else if (checkReadyItem() && itemTrigger()) {
            setCopyRodReadyAnime();
        }
    }
}

int daAlink_c::checkNextActionCopyRod() {
    if ((checkBoomerangCatchAnime() &&
         mUpperFrameCtrl[2].getFrame() <= mpHIO->mItem.mBoomerang.m.mCatchAnm.mCancelFrame) ||
        checkNoResetFlg1(FLG1_UNK_10000))
    {
        return 0;
    }

    if (mFastShotTime != 0) {
        mFastShotTime--;
    }

    if (!checkCopyRodAnime()) {
        setCopyRodReadyAnime();
        setFastShotTimer();

        if (!checkAttentionLock()) {
            field_0x2fe4 = shape_angle.y;
        }
    }

    if (!checkAttentionLock() && mFastShotTime == 0) {
        return procCopyRodSubjectInit();
    }

    return procCopyRodMoveInit();
}

void daAlink_c::setCopyRodSight() {
    if (mItemAcKeep.getActor() && !checkCopyRodThrowAnime()) {
        cXyz sight_pos;
        checkSightLine(getCopyRodBallDisMax(), &sight_pos);
        mSight.setPos(&sight_pos);
        mSight.onDrawFlg();

        if (mAtCps[0].ChkAtHit()) {
            mSight.onLockFlg();
        } else {
            mSight.offLockFlg();
        }
    }
}

int daAlink_c::procCopyRodSubjectInit() {
    if (!commonProcInitNotSameProc(PROC_COPY_ROD_SUBJECT)) {
        return 0;
    }

    mNormalSpeed = 0.0f;
    initCopyRodUpperAnimeSpeed(1);
    setSingleAnimeBaseSpeed(ANM_ATN_WAIT_RIGHT, 0.0f, mpHIO->mItem.mBoomerang.m.mStartInterpolation);

    dComIfGp_setPlayerStatus0(0, 0x80);
    current.angle.y = shape_angle.y;
    return 1;
}

int daAlink_c::procCopyRodSubject() {
    if (!checkItemActorPointer()) {
        return 1;
    }

    if (checkCopyRodReadyAnime()) {
        setDoStatus(BUTTON_STATUS_BACK);
    }

    setShapeAngleToAtnActor(0);

    if (!checkNextAction(0)) {
        if (setBodyAngleToCamera()) {
            setCopyRodSight();
        }
    } else {
        mSight.offDrawFlg();
    }

    return 1;
}

int daAlink_c::procCopyRodMoveInit() {
    if (!commonProcInitNotSameProc(PROC_COPY_ROD_MOVE)) {
        return 0;
    }

    if (checkZeroSpeedF()) {
        onModeFlg(1);
    }

    initCopyRodUpperAnimeSpeed(0);
    setBlendAtnMoveAnime(mpHIO->mItem.mBoomerang.m.mStartInterpolation);

    dComIfGp_setPlayerStatus0(0, 0x80);
    return 1;
}

int daAlink_c::procCopyRodMove() {
    if (!checkItemActorPointer()) {
        return 1;
    }

    if (!checkNextAction(0)) {
        f32 morf = -1.0f;

        if (checkZeroSpeedF()) {
            onModeFlg(1);

            if (field_0x2f98 != 3) {
                field_0x2f98 = 3;
                morf = mpHIO->mBasic.m.mBasicInterpolation;
            }
        } else {
            offModeFlg(1);
        }

        setBlendAtnMoveAnime(morf);
        setBodyAngleXReadyAnime(0);
    }

    return 1;
}

int daAlink_c::procCopyRodSwingInit() {
    commonProcInit(PROC_COPY_ROD_SWING);
    daCstatue_c* statue = (daCstatue_c*)getCopyRodControllActor();

    if (statue != NULL &&
        ((fopAcM_GetName(statue) == PROC_CSTATUE && statue->checkNotSmallType()) ||
         fopAcM_GetName(statue) != PROC_CSTATUE))
    {
        setSingleAnimeParam(ANM_COPYROD_SWING_LARGE, &mpHIO->mItem.mCopyRod.m.mBigSwingAnm);
        field_0x3478 = 13.0f;
        field_0x347c = 21.0f;
        field_0x3480 = mpHIO->mItem.mCopyRod.m.mBigSwingAnm.mCancelFrame;
        field_0x3484 = 27.0f;
    } else {
        setSingleAnimeParam(ANM_COPYROD_SWING, &mpHIO->mItem.mCopyRod.m.mSwingAnm);
        field_0x3478 = 4.0f;
        field_0x347c = 8.0f;
        field_0x3480 = mpHIO->mItem.mCopyRod.m.mSwingAnm.mCancelFrame;
        field_0x3484 = 14.0f;
    }

    mNormalSpeed = 0.0f;
    current.angle.y = shape_angle.y;
    mProcVar2.field_0x300c = 1;
    mProcVar3.field_0x300e = 0;

    voiceStart(Z2SE_AL_V_ATTACK_S);
    mAtCps[0].SetR(40.0f);
    mAtCps[0].SetAtType(AT_TYPE_COPY_ROD);
    cancelLockAt();
    return 1;
}

int daAlink_c::procCopyRodSwing() {
    daPy_frameCtrl_c* frameCtrl = mUnderFrameCtrl;
    mProcVar2.field_0x300c = 0;

    if (checkAnmEnd(frameCtrl)) {
        setCopyRodControllAnime();
        checkNextAction(0);
    } else if (frameCtrl->getFrame() > field_0x3480 && checkNextAction(1)) {
        if (checkModeFlg(0x1000)) {
            setCopyRodControllAnime();
        }
    } else if (frameCtrl->getFrame() >= field_0x3478 && frameCtrl->getFrame() < field_0x347c) {
        if (mProcVar3.field_0x300e == 0) {
            seStartSwordCut(Z2SE_AL_COPYROD_SWING);
        }

        mProcVar3.field_0x300e = 1;
        if (changeCutReverseProc(ANM_CUT_RECOIL_A)) {
            return 1;
        }
    } else {
        mProcVar3.field_0x300e = 0;
    }

    return 1;
}

int daAlink_c::procCopyRodReviveInit() {
    if (!commonProcInitNotSameProc(PROC_COPY_ROD_REVIVE)) {
        return 1;
    }

    if (mDemo.getParam0() == 0) {
        if (mEquipItem != fpcNm_ITEM_COPY_ROD) {
            deleteEquipItem(FALSE, FALSE);
            mEquipItem = fpcNm_ITEM_COPY_ROD;
            setCopyRodModel();
        }
        field_0x0724->setFrame(0.0f);
    }

    setSingleAnimeBase(ANM_GET_MASTER_SWORD);
    mNormalSpeed = 0.0f;
    return 1;
}

int daAlink_c::procCopyRodRevive() {
    simpleAnmPlay(field_0x0724);

    if (checkAnmEnd(&mUnderFrameCtrl[0])) {
        dComIfGp_evmng_cutEnd(mAlinkStaffId);
    }

    return 1;
}
