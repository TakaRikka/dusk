/**
 * d_a_alink_boom.inc
 * Player Boomerang action handling
 */

#include "d/actor/d_a_alink.h"
#include "d/actor/d_a_boomerang.h"

BOOL daAlink_c::checkBoomerangLockAccept() {
    return mSight.getDrawFlg() && dComIfGp_checkPlayerStatus0(0, 0x80000);
}

f32 daAlink_c::getBoomSpeed() {
    f32 speed;
    if (checkBoomerangChargeEndWait()) {
        speed = mpHIO->mItem.mBoomerang.m.mChargeFlySpeed;
    } else {
        speed = mpHIO->mItem.mBoomerang.m.mFlySpeed;
    }

    if (checkModeFlg(0x400)) {
        fopAc_ac_c* ride_actor = mRideAcKeep.getActor();
        if (ride_actor != NULL && ride_actor->speedF > 0.0f) {
            speed += ride_actor->speedF;
        }
    }

    return speed;
}

f32 daAlink_c::getBoomCatchSpeed() const {
    return mpHIO->mItem.mBoomerang.m.mCatchSpeed;
}

f32 daAlink_c::getBoomFlyMax() const {
    if (checkModeFlg(0x400) && !checkCanoeRide()) {
        return mpHIO->mItem.mBoomerang.m.mHorsebackFlyDistMax;
    } else {
        return mpHIO->mItem.mBoomerang.m.mFlyDistMax;
    }
}

f32 daAlink_c::getBoomLockMax() {
    f32 fly_max = getBoomFlyMax();

    if (checkBossBabaRoom()) {
        return 2600.0f;
    } else if (mpHIO->mItem.mBoomerang.m.mLockDistMax > fly_max) {
        return mpHIO->mItem.mBoomerang.m.mLockDistMax;
    } else {
        return fly_max;
    }
}

s16 daAlink_c::getBoomBgThroughTime() const {
    return mpHIO->mItem.mBoomerang.m.mBgThroughTime;
}

bool daAlink_c::checkBossBabaRoom() {
    return checkStageName("D_MN05A");
}

void daAlink_c::cancelBoomerangLock(fopAc_ac_c* i_actor) {
    daBoomerang_c* boomerang = (daBoomerang_c*)getBoomerangActor();

    if (boomerang != NULL) {
        boomerang->cancelLockActor(i_actor);
    }
}

fopAc_ac_c* daAlink_c::getBoomerangActor() {
    if (mThrowBoomerangAcKeep.getID() != fpcM_ERROR_PROCESS_ID_e) {
        return mThrowBoomerangAcKeep.getActor();
    }

    if (mEquipItem == fpcNm_ITEM_BOOMERANG) {
        return mItemAcKeep.getActor();
    }

    return NULL;
}

bool daAlink_c::checkBoomerangChargeEnd() {
    if (checkBoomerangChargeEndWait()) {
        return true;
    }

    daBoomerang_c* boomerang = (daBoomerang_c*)getBoomerangActor();
    if (boomerang != NULL && boomerang->checkCharge()) {
        return true;
    }

    return false;
}

BOOL daAlink_c::checkBoomerangCarry(fopAc_ac_c* i_grabActor) {
    if (checkNoResetFlg0(FLG0_UNK_20) && !checkEventRun() && !checkWolf() &&
        checkModeFlg(0x10000000) &&
        (checkHorseNoUpperAnime() || checkHorseTurnAnime() || checkBoomerangThrowAnime())) {
        deleteEquipItem(FALSE, FALSE);
        seStartOnlyReverb(JA_SE_LK_BOOM_CATCH);
        offNoResetFlg0(FLG0_UNK_20);

        setGrabItemActor(i_grabActor);
        field_0x33e4 = 38.0f;
        setGrabUpperAnime(mpHIO->mBasic.m.mBasicInterpolation);
        return 1;
    }

    return 0;
}

void daAlink_c::initBoomerangUpperAnimeSpeed(int param_0) {
    if (!checkBoomerangThrowAnime()) {
        if (param_0 != 0) {
            f32 tmp = 0.0f;
            mUpperFrameCtrl[2].setRate(tmp);
            mUpperFrameCtrl[2].setFrame(tmp);
            getNowAnmPackUpper(UPPER_2)->setFrame(tmp);
        } else {
            mUpperFrameCtrl[2].setRate(mpHIO->mItem.mBoomerang.m.mIdleAnmSpeed);
        }
    }
}

BOOL daAlink_c::checkBoomerangAnime() const {
    return checkBoomerangReadyAnime() || checkBoomerangThrowAnime();
}

BOOL daAlink_c::checkBoomerangThrowAnime() const {
    return (mEquipItem == fpcNm_ITEM_BOOMERANG || checkNoResetFlg1(FLG1_UNK_2) || mEquipItem == 0x102) &&
           checkUpperAnime(0x53);
}

void daAlink_c::setBoomerangReadyQuake() {
    f32 tmp_0 = 0.0f;
    dComIfGp_getVibration().StartQuake(1, 1, cXyz(tmp_0, 1.0f, tmp_0));

    onNoResetFlg3(FLG3_UNK_4);
    mItemVar0.field_0x3018 = mpHIO->mItem.mBoomerang.m.mChargeTime;
}

void daAlink_c::setBoomerangReadyAnime() {
    f32 var_f31;
    if (checkAttentionLock()) {
        var_f31 = mpHIO->mItem.mBoomerang.m.mIdleAnmSpeed;
    } else {
        var_f31 = 0.0f;
    }

    setUpperAnimeBaseSpeed(0x54, var_f31, 3.0f);
    setBoomerangReadyQuake();
}

void daAlink_c::setThrowBoomerangAnime() {
    setUpperAnimeParam(0x53, UPPER_2, &mpHIO->mItem.mBoomerang.m.mThrowAnm);
    cancelItemUseQuake(0);
}

void daAlink_c::setBoomerangCatchAnime() {
    setUpperAnimeParam(0x52, UPPER_2, &mpHIO->mItem.mBoomerang.m.mCatchAnm);
    setFacePriBck(0xE8);
    field_0x2f97 = 254;

    if (mEquipItem != fpcNm_ITEM_COPY_ROD) {
        seStartOnlyReverb(JA_SE_LK_BOOM_CATCH);
    }
}

void daAlink_c::throwBoomerang() {
    daBoomerang_c* item = (daBoomerang_c*)mItemAcKeep.getActor();
    onNoResetFlg1(FLG1_UNK_2);

    //! @bug `item` pointer is being accessed without checking if NULL first
    BOOL boomerang_item = fopAcM_GetName(item) == PROC_BOOMERANG;
    if (boomerang_item) {
        item->setThrow();
        item->current.angle.y = shape_angle.y + mBodyAngle.y;
        item->current.angle.x = -mBodyAngle.x;

        if (mTargetedActor != NULL) {
            item->setAimActor(mTargetedActor);
        }

        item->shape_angle.y = shape_angle.y;
        mThrowBoomerangAcKeep = mItemAcKeep;
        dComIfGp_setPlayerStatus0(0, 0x400000);
        daPy_boomerangMove_c::initDropAngleY();
        dComIfGp_clearPlayerStatus0(0, 0x80000);
        mFastShotTime = -1;
        daPy_boomerangMove_c::offEventKeepFlg();
    } else {
        item->speedF = mpHIO->mItem.mPickUp.m.field_0x28;
        item->speed.y = mpHIO->mItem.mPickUp.m.field_0x2C;
        item->current.angle.y = shape_angle.y;

        fopAcM_cancelCarryNow(item);
        item = NULL;
        mEquipItem = fpcNm_ITEM_NONE;
        field_0x2f94 = 0xFF;
        field_0x2f96 = 0xFE;
    }

    mItemAcKeep.clearData();
    mEquipItem = fpcNm_ITEM_NONE;
    field_0x2f94 = 0xFF;
    field_0x2f96 = 0xFE;

    voiceStart(Z2SE_AL_V_ATTACK_S);

    if (checkModeFlg(MODE_RIDING)) {
        if (checkCanoeRide()) {
            procCanoeWaitInit(0);
        } else {
            procHorseWaitInit();
        }
    } else if (mLinkAcch.ChkGroundHit() && boomerang_item) {
        if (mTargetedActor == NULL && item != NULL) {
            mTargetedActor = item;
            field_0x27f4 = item;
        }

        if (checkZeroSpeedF()) {
            field_0x2f98 = 3;
            procAtnActorWaitInit();
        } else {
            procAtnActorMoveInit();
        }

        field_0x2060->initOldFrameMorf(0.0f, 0, 35);
    }
}

int daAlink_c::returnBoomerang(int param_0) {
    dComIfGp_clearPlayerStatus0(0, 0x400000);

    if (mEquipItem == fpcNm_ITEM_NONE && !checkMagneBootsOn() && mItemAcKeep.getActor() == NULL &&
        !checkBoardRide() && !checkBoarRide() && !checkWolf() &&
        (mProcID != PROC_HANG_READY || field_0x3198 != 3) && (!checkEventRun() || param_0 != 0) &&
        (checkNoUpperAnime() || checkPlayerGuard() || checkHorseTurnAnime() ||
         checkDashDamageAnime() || checkBoomerangAnime()) &&
        !checkModeFlg(0x1BD0810)) {
        mItemAcKeep = mThrowBoomerangAcKeep;
        mThrowBoomerangAcKeep.clearData();

        mEquipItem = fpcNm_ITEM_BOOMERANG;
        field_0x2f94 = 2;
        onNoResetFlg0(FLG0_UNK_20);

        f32 tmp_0 = 0.0f;
        dComIfGp_getVibration().StartShock(1, 1, cXyz(tmp_0, 1.0f, tmp_0));
        return 1;
    }

    mThrowBoomerangAcKeep.clearData();
    return 0;
}

int daAlink_c::checkUpperItemActionBoomerang() {
    if (mItemVar0.field_0x3018 != 0) {
        mItemVar0.field_0x3018--;

        if (mItemVar0.field_0x3018 == 0) {
            cancelItemUseQuake(0);
        }
    }

    if (checkReadyItem() && !itemButton() && checkBoomerangReadyAnime()) {
        setThrowBoomerangAnime();
        return 1;
    }

    return cancelUpperItemReadyAnime(0);
}

void daAlink_c::checkUpperItemActionBoomerangFly() {
    if (checkBoomerangReadyAnime()) {
        checkUpperItemActionBoomerang();
    } else if (checkReadyItem() && itemTrigger()) {
        setBoomerangReadyAnime();
    }
}

int daAlink_c::checkNextActionBoomerang() {
    if ((checkBoomerangCatchAnime() &&
         mUpperFrameCtrl[2].getFrame() <= mpHIO->mItem.mBoomerang.m.mCatchAnm.mCancelFrame) ||
        checkNoResetFlg1(FLG1_UNK_2)) {
        return 0;
    }

    if (mFastShotTime != 0) {
        mFastShotTime--;
    }

    if (!checkBoomerangAnime()) {
        setBoomerangReadyAnime();
        setFastShotTimer();

        if (!checkAttentionLock()) {
            field_0x2fe4 = shape_angle.y;
        }
    }

    if (!checkAttentionLock() && mFastShotTime == 0) {
        if (checkModeFlg(0x400)) {
            if (checkCanoeRide()) {
                return procCanoeBoomerangSubjectInit();
            } else {
                return procHorseBoomerangSubjectInit();
            }
        } else {
            return procBoomerangSubjectInit();
        }
    } else if (checkModeFlg(0x400)) {
        if (checkCanoeRide()) {
            return procCanoeBoomerangMoveInit();
        } else {
            return procHorseBoomerangMoveInit();
        }
    } else {
        return procBoomerangMoveInit();
    }
}

int daAlink_c::checkBoomerangCatchAction() {
    if (checkNoResetFlg0(FLG0_UNK_20)) {
        if (!checkEventRun() && !checkWolf() && checkModeFlg(0x10000000)) {
            if (checkNoUpperAnime() || checkHorseTurnAnime() || checkBoomerangThrowAnime()) {
                if (checkModeFlg(0x1) && !checkModeFlg(0x400)) {
                    return procBoomerangCatchInit();
                }

                setBoomerangCatchAnime();
            }
        }

        offNoResetFlg0(FLG0_UNK_20);
    }

    return FALSE;
}

void daAlink_c::setBoomerangSight() {
    if (mItemAcKeep.getActor() && !checkBoomerangThrowAnime()) {
        BOOL check_line = checkSightLine(getBoomLockMax(), &mHeldItemRootPos);

        mSight.setPos(&mHeldItemRootPos);
        mSight.onDrawFlg();

        daBoomerang_c* boomerang = (daBoomerang_c*)mItemAcKeep.getActor();
        if (boomerang->getLockReserve() || (check_line && !boomerang->getLockCntMax())) {
            setItemActionButtonStatus(0x10);
            itemActionTrigger();
        }
    }
}

int daAlink_c::procBoomerangSubjectInit() {
    if (!commonProcInitNotSameProc(PROC_BOOMERANG_SUBJECT)) {
        return 0;
    }

    mNormalSpeed = 0.0f;
    initBoomerangUpperAnimeSpeed(1);
    setSingleAnimeBaseSpeed(ANM_ATN_WAIT_RIGHT, 0.0f,
                            mpHIO->mItem.mBoomerang.m.mStartInterpolation);
    dComIfGp_setPlayerStatus0(0, 0x80000);
    current.angle.y = shape_angle.y;

    return 1;
}

int daAlink_c::procBoomerangSubject() {
    if (!checkItemActorPointer()) {
        return 1;
    }

    if (checkBoomerangReadyAnime()) {
        setDoStatus(0x12);
    }

    setShapeAngleToAtnActor(0);

    if (!checkNextAction(0)) {
        if (setBodyAngleToCamera()) {
            setBoomerangSight();
        }
    } else {
        mSight.offDrawFlg();
    }

    return 1;
}

int daAlink_c::procBoomerangMoveInit() {
    if (!commonProcInitNotSameProc(PROC_BOOMERANG_MOVE)) {
        return 0;
    }

    if (checkZeroSpeedF()) {
        onModeFlg(1);
    }

    initBoomerangUpperAnimeSpeed(0);
    setBlendAtnMoveAnime(mpHIO->mItem.mBoomerang.m.mStartInterpolation);

    dComIfGp_setPlayerStatus0(0, 0x80000);
    return 1;
}

int daAlink_c::procBoomerangMove() {
    if (!checkItemActorPointer()) {
        return 1;
    }

    if (!checkNextAction(0)) {
        f32 var_f31 = -1.0f;

        if (checkZeroSpeedF()) {
            onModeFlg(1);

            if (field_0x2f98 != 3) {
                field_0x2f98 = 3;
                var_f31 = mpHIO->mBasic.m.mBasicInterpolation;
            }
        } else {
            offModeFlg(1);

            daBoomerang_c* boomerang = (daBoomerang_c*)mItemAcKeep.getActor();
            if (boomerang != NULL) {
                boomerang->onLockDistanceCancel();
            }
        }

        setBlendAtnMoveAnime(var_f31);
        setBodyAngleXReadyAnime(0);
    }

    return 1;
}

int daAlink_c::procBoomerangCatchInit() {
    if (!commonProcInitNotSameProc(PROC_BOOMERANG_CATCH)) {
        return 0;
    }

    setSingleAnimeParam(ANM_BOOMERANG_CATCH, &mpHIO->mItem.mBoomerang.m.mCatchAnm);
    setBoomerangCatchAnime();
    offNoResetFlg0(FLG0_UNK_20);

    return 1;
}

int daAlink_c::procBoomerangCatch() {
    daPy_frameCtrl_c* frameCtrl = mUnderFrameCtrl;
    onEndResetFlg0(ERFLG0_UNK_8000000);

    if (frameCtrl->getFrame() > mpHIO->mItem.mBoomerang.m.mCatchAnm.mCancelFrame) {
        onModeFlg(4);
    }

    if (frameCtrl->checkAnmEnd()) {
        resetUpperAnime(UPPER_2, -1.0f);
        checkNextAction(0);
    } else {
        checkNextAction(1);
    }

    return 1;
}
