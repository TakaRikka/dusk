/**
 * d_a_alink_spinner.inc
 * Player Spinner action handling
 */

#include "d/actor/d_a_alink.h"
#include "d/actor/d_a_spinner.h"

f32 daAlink_c::getSpinnerGravity() const {
    return mpHIO->mItem.mSpinner.m.mGravity;
}

f32 daAlink_c::getSpinnerMaxFallSpeed() const {
    return mpHIO->mItem.mSpinner.m.mMaxFallSpeed;
}

f32 daAlink_c::getSpinnerJumpRate() const {
    return mpHIO->mItem.mSpinner.m.mJumpRate;
}

s16 daAlink_c::getSpinnerRideMoveTime() {
    if (checkLv4BossRoom()) {
        return mpHIO->mItem.mSpinner.m.mBossRideMoveTime;
    }

    return mpHIO->mItem.mSpinner.m.mRideMoveTime;
}

f32 daAlink_c::getSpinnerRideSpeedF() {
    if (checkLv4BossRoom()) {
        return mpHIO->mItem.mSpinner.m.mBossRideSpeed;
    }

    return mpHIO->mItem.mSpinner.m.mRideSpeed;
}

f32 daAlink_c::getSpinnerRideDecSpeedMax() const {
    return mpHIO->mItem.mSpinner.m.mDecelSpeedMax;
}

f32 daAlink_c::getSpinnerRideDecSpeedMin() const {
    return mpHIO->mItem.mSpinner.m.mDecelSpeedMin;
}

f32 daAlink_c::getSpinnerRideDecSpeedRate() const {
    return mpHIO->mItem.mSpinner.m.mDecelRate;
}

s16 daAlink_c::getSpinnerRideRotAngleMax() const {
    return mpHIO->mItem.mSpinner.m.mRideRotAngleMax;
}

s16 daAlink_c::getSpinnerRideRotAngleMin() const {
    return mpHIO->mItem.mSpinner.m.mRideRotAngleMin;
}

void daAlink_c::onSpinnerPathForceRemove() {
    if (checkSpinnerRide() && mRideAcKeep.getActor() != NULL) {
        ((daSpinner_c*)mRideAcKeep.getActor())->onPathForceRemove();
    }
}

bool daAlink_c::checkSpinnerPathMove() {
    if (checkSpinnerRide() && mRideAcKeep.getActor() != NULL) {
        if (((daSpinner_c*)mRideAcKeep.getActor())->checkPathMoveNow() != NULL) {
            return true;
        }
    }

    return false;
}

bool daAlink_c::checkLv4BossRoom() {
    return checkStageName("D_MN10A");
}

bool daAlink_c::checkSpinnerTriggerAttack() {
    if (checkSpinnerRide() && mRideAcKeep.getActor() != NULL) {
        if (((daSpinner_c*)mRideAcKeep.getActor())->reflectAccept()) {
            return true;
        }
    }

    return false;
}


void daAlink_c::setSpinnerSyncPos() {
    daSpinner_c* spinner = (daSpinner_c*)mRideAcKeep.getActor();

    mDoMtx_multVecZero(spinner->getModelMtx(), &current.pos);
    mDoMtx_stack_c::ZXYrotS(spinner->shape_angle);
    mDoMtx_stack_c::YrotM(shape_angle.y - spinner->shape_angle.y);
    mDoMtx_MtxToRot(mDoMtx_stack_c::get(), &shape_angle);
    field_0x37a4 = spinner->current.pos;
}

int daAlink_c::procSpinnerReadyInit() {
    fopAc_ac_c* spinner = (fopAc_ac_c*)fopAcM_fastCreate(
        PROC_SPINNER, daSpinner_c::getWaitArg(), &current.pos, fopAcM_GetRoomNo(this),
        &shape_angle, NULL, -1, NULL, NULL);

    if (spinner == NULL) {
        return 0;
    }

    commonProcInit(PROC_SPINNER_READY);
    mRideAcKeep.setData(spinner);
    setSingleAnime(ANM_VJUMP_START, mpHIO->mWallHang.mSmallJump.m.mSmallJumpAnm.mSpeed, 6.0f,
                   mpHIO->mWallHang.mSmallJump.m.mSmallJumpAnm.mEndFrame,
                   mpHIO->mWallHang.mSmallJump.m.mSmallJumpAnm.mInterpolation);
    mNormalSpeed = 0.0f;
    setHeavyBoots(0);

    speed.y = 26.0f;
    voiceStart(Z2SE_AL_V_JUMP_S);
    seStartOnlyReverb(Z2SE_AL_SPINNER_START);

    field_0x2f99 = 15;
    deleteEquipItem(TRUE, TRUE);

    current.angle.y = shape_angle.y;
    field_0x3588 = l_waitBaseAnime;
    field_0x33b0 = field_0x3588.y;
    field_0x34d4 = l_waitBaseAnime;
    mProcVar2.field_0x300c = 8;

    return 1;
}

int daAlink_c::procSpinnerReady() {
    daSpinner_c* spinner = (daSpinner_c*)mRideAcKeep.getActor();

    if (spinner == NULL) {
        return checkNextAction(0);
    }

    current.pos.x = spinner->current.pos.x;
    current.pos.z = spinner->current.pos.z;

    mProcVar2.field_0x300c--;
    field_0x2f99 = 15;

    if ((mProcVar2.field_0x300c < 0 && current.pos.y <= spinner->getModelMtx()[1][3]) ||
        mProcVar2.field_0x300c < -30) {
        current.pos.y = spinner->getModelMtx()[1][3];
        procSpinnerWaitInit();
    }

    return 1;
}

void daAlink_c::setSpinnerStatus(u8 status, u8 flag) {
    dComIfGp_setDoStatus(status, flag);
}

int daAlink_c::procSpinnerWaitInit() {
    if (!commonProcInitNotSameProc(PROC_SPINNER_WAIT)) {
        return 0;
    }

    mRideStatus = 5;
    setBgCheckParam();
    setSingleAnimeBaseSpeed(ANM_RIDE_CROUCH, mpHIO->mItem.mBoard.m.mSitAnmSpeed, 4.0f);
    speed.y = 0.0f;
    setSpecialGravity(0.0f, maxFallSpeed, 0);
    mNormalSpeed = 0.0f;
    mProcVar2.field_0x300c = 0;

    mProcVar3.field_0x300e = shape_angle.y;
    mProcVar4.field_0x3010 = 0;
    field_0x3198 = ANM_RIDE_CROUCH;
    field_0x33b0 = 70.0f;
    mProcVar5.field_0x3012 = 0;
    field_0x37a4 = mRideAcKeep.getActor()->current.pos;

    return 1;
}


int daAlink_c::procSpinnerWait() {
    daSpinner_c* spinner = (daSpinner_c*)mRideAcKeep.getActor();

    if (spinner == NULL || spinner->getDeleteFlg() || checkWindSpeedOnXZ() ||
        checkEndResetFlg1(ERFLG1_UNK_1) || checkEndResetFlg0(ERFLG0_UNK_2)) {
        mNormalSpeed = 0.0f;
        setJumpMode();
        return procFallInit(1, mpHIO->mAutoJump.m.mFallInterpolation);
    }

    if (spinner->checkSpinnerTagEnd()) {
        return procSmallJumpInit(2);
    }

    int itemSetBtn = checkItemSetButton(fpcNm_ITEM_SPINNER);
    if (mProcVar2.field_0x300c == 0) {
        if (!itemButton()) {
            mProcVar2.field_0x300c = 1;
            mNormalSpeed = getSpinnerRideSpeedF();
            spinner->setMove(mNormalSpeed, getSpinnerRideMoveTime());
        } else if (checkInputOnR()) {
            int direction = getDirectionFromAngle(mStickAngle);
            s16 angle = field_0x33ac * 384.0f * field_0x33ac;

            if (direction == DIR_LEFT) {
                shape_angle.y += angle;
            } else if (direction == DIR_RIGHT) {
                shape_angle.y -= angle;
            }

            current.angle.y = shape_angle.y;
            mProcVar3.field_0x300e = shape_angle.y;
        }
    } else {
        if (checkSetItemTrigger(fpcNm_ITEM_SPINNER) || swordSwingTrigger() || itemSetBtn == 2) {
            if (swordSwingTrigger()) {
                swordEquip(0);
            }
            setJumpMode();
            mNormalSpeed = 0.0f;
            return procFallInit(1, mpHIO->mAutoJump.m.mFallInterpolation);
        }

        daPy_frameCtrl_c* frameCtrl = mUnderFrameCtrl;
        dMeter2Info_onDirectUseItem(itemSetBtn);

        if (spinner->checkPathMoveNow() == NULL) {
            seStartOnlyReverbLevel(Z2SE_AL_SPINNER_RIDE);
            setSpinnerStatus(ACTION_STR_ATTACK, ACTION_FLG_DEFAULT);
        } else {
            seStartOnlyReverbLevel(Z2SE_AL_SPINNER_RAIL);
            setSpinnerStatus(ACTION_STR_JUMP2, ACTION_FLG_DEFAULT);
        }

        mNormalSpeed = spinner->speedF;
        current.angle.y = spinner->current.angle.y;

        if (spinner->checkSpinnerTagIntoIncRot()) {
            mProcVar5.field_0x3012 = 1;
            setSpinnerStatus(ACTION_STR_SPIN, ACTION_FLG_CONTINUATION);

            if (field_0x3198 != ANM_RIDE_KICK && field_0x2060->getOldFrameRate() < 0.01f) {
                setSingleAnimeBaseSpeed(ANM_RIDE_CROUCH, mpHIO->mItem.mBoard.m.mSitAnmSpeed,
                                        mpHIO->mItem.mBoard.m.mFastPushInterpolation);
                field_0x3198 = ANM_RIDE_KICK;
                fopAcM_seStartCurrent(spinner, Z2SE_OBJ_SPNR_SW_PUSH, 0);
            }
        } else if (spinner->checkSpinnerTagInto()) {
            if (mProcVar5.field_0x3012 != 0) {
                setSpinnerStatus(ACTION_STR_SPIN, ACTION_FLG_CONTINUATION);
            } else {
                setSpinnerStatus(ACTION_STR_SPIN, ACTION_FLG_DEFAULT);
            }

            if ((field_0x3198 == ANM_RIDE_KICK && field_0x2060->getOldFrameRate() < 0.01f) ||
                (field_0x3198 != ANM_RIDE_KICK && field_0x3198 != ANM_RIDE_WAIT)) {
                setSingleAnimeBaseSpeed(ANM_RIDE_WAIT, mpHIO->mItem.mBoard.m.mStandAnmSpeed,
                                        mpHIO->mItem.mBoard.m.mStandInterpolation);
                field_0x3198 = ANM_RIDE_WAIT;
            }
        } else if (spinner->getButtonJump() && field_0x3198 != ANM_RIDE_JUMP) {
            field_0x3198 = ANM_RIDE_JUMP;
            setSingleAnime(ANM_RIDE_JUMP, 0.0f, mpHIO->mItem.mBoard.m.mAirborneAnm.mStartFrame,
                           mpHIO->mItem.mBoard.m.mAirborneAnm.mEndFrame,
                           mpHIO->mItem.mBoard.m.mAirborneAnm.mInterpolation);
            mProcVar4.field_0x3010 = 0;
        } else if (field_0x3198 == ANM_RIDE_JUMP) {
            if (!spinner->getJumpFlg()) {
                setSingleAnime(ANM_RIDE_JUMP_LAND, mpHIO->mItem.mBoard.m.mLandAnm.mSpeed,
                               mpHIO->mItem.mBoard.m.mLandAnm.mStartFrame, 16,
                               mpHIO->mItem.mBoard.m.mLandAnm.mInterpolation);
                field_0x3198 = ANM_RIDE_JUMP_LAND;
            } else if (mProcVar4.field_0x3010 == 0 && field_0x2060->getOldFrameRate() < 0.01f) {
                setSingleAnime(ANM_RIDE_JUMP, mpHIO->mItem.mBoard.m.mAirborneAnm.mSpeed,
                               mpHIO->mItem.mBoard.m.mAirborneAnm.mStartFrame,
                               mpHIO->mItem.mBoard.m.mAirborneAnm.mEndFrame, -1.0f);
                mProcVar4.field_0x3010 = 1;
            }
        } else if ((field_0x3198 == ANM_RIDE_JUMP_LAND && frameCtrl->checkAnmEnd()) ||
                   (field_0x3198 != ANM_RIDE_JUMP_LAND && field_0x3198 != ANM_RIDE_CROUCH)) {
            setSingleAnimeBaseSpeed(ANM_RIDE_CROUCH, mpHIO->mItem.mBoard.m.mSitAnmSpeed,
                                    mpHIO->mItem.mBoard.m.mSitInterpolation);
            field_0x3198 = ANM_RIDE_CROUCH;
        }
    }

    setSpinnerSyncPos();
    shape_angle.y = mProcVar3.field_0x300e;

    return 1;
}
