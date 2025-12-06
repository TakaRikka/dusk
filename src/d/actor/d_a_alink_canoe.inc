/**
 * d_a_alink_canoe.inc
 * Player Canoe riding handling
 */

#include "d/actor/d_a_alink.h"
#include "d/actor/d_a_canoe.h"
#include "d/actor/d_a_mg_rod.h"
#include "SSystem/SComponent/c_math.h"

f32 daAlink_c::getCanoeMaxSpeed() const {
    if (checkCanoeSlider()) {
        return mpHIO->mCanoe.m.mMaxSpeed_RiverRide;
    } else {
        return mpHIO->mCanoe.m.mMaxSpeed;
    }
}

f32 daAlink_c::getCanoeBackMaxSpeed() const {
    return mpHIO->mCanoe.m.mBackMaxSpeed;
}

f32 daAlink_c::getCanoeCres() const {
    if (checkCanoeSlider()) {
        return mpHIO->mCanoe.m.mDeceleration_RiverRide;
    } else {
        return mpHIO->mCanoe.m.mDeceleration;
    }
}

f32 daAlink_c::getCanoeSpeedRate() const {
    if (checkCanoeSlider()) {
        return mpHIO->mCanoe.m.mSpeedRate_RiverRide;
    } else {
        return mpHIO->mCanoe.m.mSpeedRate;
    }
}

s16 daAlink_c::getCanoeMaxRotSpeed() const {
    s16 var_r30;
    if (checkCanoeSlider()) {
        var_r30 = mpHIO->mCanoe.m.mMaxTurnAngle_RiverRide;
    } else {
        var_r30 = mpHIO->mCanoe.m.mMaxTurnAngle;
    }

    return var_r30 * (((mUnderFrameCtrl[0].getRate() - mpHIO->mCanoe.m.mStickRowAnmSpeed) * 0.5f) + 1.0f);
}

cXyz* daAlink_c::getCanoeLocalPaddleTop() {
    static Vec const localPaddleTop = {0.0f, 0.0f, -158.0f};
    return (cXyz*)&localPaddleTop;
}

static fopAc_ac_c* daAlink_searchCanoe(fopAc_ac_c* i_actor, void* i_data) {
    if (fopAcM_GetName(i_actor) == PROC_CANOE && ((daCanoe_c*)i_actor)->getModel() != NULL) {
        return i_actor;
    }

    return NULL;
}

BOOL daAlink_c::checkCanoeRideTandem() {
    daCanoe_c* canoe = (daCanoe_c*)mRideAcKeep.getActor();

    if (checkCanoeRide() && canoe != NULL && canoe->checkTandem()) {
        return true;
    }

    return false;
}

BOOL daAlink_c::checkFishingRodAndLureItem() const {
    return checkFisingRodLure() || checkFishingRodItem(mEquipItem);
}

void daAlink_c::initFishingRodHand() {
    field_0x2f94 = 0x65;
    field_0x2f95 = 0x65;
    mItemVar0.field_0x3018 = 1;

    if (mEquipItem == fpcNm_ITEM_JEWEL_WORM_ROD || mEquipItem == fpcNm_ITEM_WORM_ROD) {
        mItemMode = 0x74;
    } else if (mEquipItem == fpcNm_ITEM_JEWEL_BEE_ROD || mEquipItem == fpcNm_ITEM_BEE_ROD) {
        mItemMode = 0x76;
    } else {
        mItemMode = 0xFF;
    }
}

MtxP daAlink_c::getCanoePaddleMatrix() {
    mDoMtx_stack_c::copy(getRightItemMatrix());
    mDoMtx_stack_c::transM(0.0f, 0.0f, 25.0f);
    return mDoMtx_stack_c::get();
}

BOOL daAlink_c::checkFishingRodGrab(fopAc_ac_c* i_actor) const {
    return checkFishingRodAndLureItem() && mItemAcKeep.getID() == fopAcM_GetID(i_actor);
}

BOOL daAlink_c::checkCanoePaddleGrab(fopAc_ac_c* i_actor) const {
    return mEquipItem == 0x10B && checkCanoeRideOwn(i_actor);
}

BOOL daAlink_c::checkCanoeFishingRodGrabOrPut() const {
    return checkFisingRodLure() && checkCanoeRide() &&
           (checkEquipAnime() || mProcID == PROC_CANOE_PADDLE_PUT ||
            mProcID == PROC_CANOE_PADDLE_GRAB || mProcID == PROC_CANOE_ROD_GRAB);
}

void daAlink_c::initCanoeRide() {
    mProcVar5.field_0x3012 = 1;
    field_0x3588 = l_waitBaseAnime;
    field_0x384c = (cXyz*)&l_canoeBaseAnime;
    mRideStatus = 3;
    mProcVar4.field_0x3010 = 0;
}

BOOL daAlink_c::checkCanoeJumpRide() {
    daCanoe_c* canoe = NULL;

    if (field_0x850[0].ChkCoHit()) {
        canoe = (daCanoe_c*)field_0x850[0].GetCoHitAc();
    } else if (field_0x850[1].ChkCoHit()) {
        canoe = (daCanoe_c*)field_0x850[1].GetCoHitAc();
    }

    if (speed.y < 0.0f && canoe != NULL && fopAcM_GetName(canoe) == PROC_CANOE &&
        canoe->checkJumpRideFlg() && !checkDeadHP() &&
        (mProcID != PROC_FALL || mProcVar4.field_0x3010 != 0)) {
        return procCanoeJumpRideInit(canoe);
    }

    return 0;
}

void daAlink_c::setArmReelAnime() {
    setUpperAnimeBaseSpeed(0x1FB, 1.0f, -1.0f);
    mUpperFrameCtrl[2].setRate(0.0f);
    mNowAnmPackUpper[2].setRatio(1.0f);
    field_0x2f97 = 8;
    offNoResetFlg1(FLG1_UNK_8000);
}

void daAlink_c::setFishingWaitAction() {
    daPy_frameCtrl_c* frame_ctrl0 = &mUnderFrameCtrl[0];
    daPy_frameCtrl_c* frame_ctrl1 = &mUnderFrameCtrl[1];

    f32 sp10 = frame_ctrl0->getFrame();
    f32 spC = frame_ctrl1->getFrame();
    f32 sp8 = mNowAnmPackUnder[0].getRatio();

    if (checkResetFlg0(RFLG0_UNK_80000)) {
        mUpperFrameCtrl[2].setRate(1.0f);
    } else {
        mUpperFrameCtrl[2].setRate(0.0f);
    }

    f32 var_f29, var_f28, var_f27, var_f26, var_f31, var_f30;
    dmg_rod_class* mg_rod = (dmg_rod_class*)mItemAcKeep.getActor();
    if (mg_rod != NULL && !checkEventRun()) {
        var_f31 = mg_rod->getRodStickX();
        var_f30 = mg_rod->getRodStickY();
    } else {
        var_f31 = 0.0f;
        var_f30 = 0.0f;
    }

    var_f29 = JMAFastSqrt(var_f31 * var_f31 + var_f30 * var_f30);
    int var_r27 = abs(cM_atan2s(-var_f31, var_f30));

    if (var_f29 > 0.05f) {
        if (var_f30 > 0.0f) {
            var_f28 = ((1.0f - (var_f29 - fabsf(var_f31))) * frame_ctrl0->getEnd()) * 0.2f;
            var_f26 = (0x4000 - var_r27) * 0.000061035156f;
        } else {
            var_f28 = frame_ctrl0->getEnd() - (((1.0f - (var_f29 - fabsf(var_f31))) * frame_ctrl0->getEnd()) * 0.8f);
            var_f26 = (var_r27 - 0x4000) * 0.000061035156f;
        }

        if (var_f31 < 0.0f) {
            var_f27 = ((1.0f - (var_f29 - fabsf(var_f30))) * frame_ctrl1->getEnd()) * 0.5f;
        } else {
            var_f27 = frame_ctrl1->getEnd() - (((1.0f - (var_f29 - fabsf(var_f30))) * frame_ctrl1->getEnd()) * 0.5f);
        }
    } else {
        var_f28 = frame_ctrl0->getEnd() * 0.2f;
        var_f27 = frame_ctrl1->getEnd() * 0.5f;
        var_f26 = 0.5f;
    }

    cLib_addCalc(&sp10, var_f28, 0.3f, 0.8f, 0.01f);
    cLib_addCalc(&spC, var_f27, 0.3f, 0.8f, 0.01f);
    frame_ctrl0->setFrame(sp10);
    frame_ctrl1->setFrame(spC);

    cLib_chaseF(&sp8, var_f26, 0.1f);
    setDoubleAnimeBlendRatio(1.0f - sp8);
}

void daAlink_c::setFishGetFace() {
    if (field_0x2fc8 == 1) {
        setFacePriBck(0x140);
    } else if (field_0x2fc8 == 2) {
        setFacePriTexture(FTANM_UNK_7A);
        setFacePriBck(0x143);
    } else if (field_0x2fc8 == 3) {
        setFacePriTexture(FTANM_UNK_7B);
        setFacePriBck(0x146);
    } else if (field_0x2fc8 == 4) {
        setFacePriTexture(FTANM_UNK_7C);
        setFacePriBck(0x149);
    } else {
        resetFacePriAnime();
    }
}

void daAlink_c::setRideCanoeBasePos(fopAc_ac_c* param_0) {
    field_0x37c8.x = 0.0f;
    field_0x37c8.y = 25.0f;
    field_0x37c8.z = -35.0f;

    if (((daCanoe_c*)param_0)->checkTandem()) {
        field_0x37c8.z += 175.0f;
    }
}

void daAlink_c::setSyncCanoePos() {
    daCanoe_c* canoe = (daCanoe_c*)mRideAcKeep.getActor();
    if (canoe->checkTandem()) {
        mDoMtx_stack_c::copy(canoe->getModelMtx());
        mDoMtx_stack_c::transM(0.0f, 0.0f, 175.0f);
        mDoMtx_stack_c::multVec(field_0x384c, &current.pos);
    } else {
        mDoMtx_multVec(canoe->getModelMtx(), field_0x384c, &current.pos);
    }

    field_0x2f99 = 0x60;
    mNormalSpeed = fabsf(canoe->speedF);

    s16 var_r28;
    if (mProcID == PROC_CANOE_SUBJECTIVITY || mProcID == PROC_CANOE_BOW_SUBJECT || mProcID == PROC_CANOE_BOOMERANG_SUBJECT || mProcID == PROC_CANOE_HOOKSHOT_SUBJECT) {
        var_r28 = 0;
        shape_angle.y = canoe->shape_angle.y;
        cLib_addCalcAngleS(&shape_angle.x, 0, 5, 0x1000, 0x200);
        cLib_addCalcAngleS(&shape_angle.z, 0, 5, 0x1000, 0x200);
    } else {
        var_r28 = -FAST_DIV(shape_angle.z, 2);
        shape_angle = canoe->shape_angle;
    }

    current.angle.y = shape_angle.y;
    cLib_addCalcAngleS(&mBodyAngle.z, var_r28, 5, 0x40, 0x10);

    s16 var_r29;
    f32 var_f31;
    if ((canoe->speedF < 0.0f && mProcID == PROC_CANOE_WAIT) || (mProcID == PROC_CANOE_ROW && mProcVar3.field_0x300e == 1)) {
        if (mProcVar5.field_0x3012 == 0) {
            var_r29 = 0x5000;
            var_f31 = 1.0f;
        } else {
            var_r29 = -0x5000;
            var_f31 = -1.0f;
        }
    } else {
        if (!checkNoUpperAnime()) {
            var_r29 = 0;
            var_f31 = 0.0f;
        } else {
            var_r29 = 0;
            var_f31 = (s16)(shape_angle.y - field_0x2fe6) * 0.005f;
        }
    }

    daPy_addCalcShort(&mProcVar4.field_0x3010, var_r29, 3, 0x1000, 0x100);
    field_0x30a2 = mProcVar4.field_0x3010;
    cLib_chaseF(&field_0x33f0, var_f31, 0.2f);
}

int daAlink_c::canoeCommon() {
    if (mRideAcKeep.getActor() == NULL) {
        return checkNextAction(0);
    }

    setSyncCanoePos();

    if (((daCanoe_c*)mRideAcKeep.getActor())->checkPaddleChangeWater()) {
        f32 var_f31;
        if (getNowAnmPackUnder(UNDER_0) != getNowAnmPackUpper(UPPER_0)) {
            var_f31 = mUpperFrameCtrl[0].getRate();
        } else {
            var_f31 = mUnderFrameCtrl[0].getRate();
        }

        u32 var_r29;
        if ((mUpperAnmHeap[0].getIdx() == getMainBckData(ANM_CANOE_ROW_LEFT_BACK)->m_upperID) || (mUpperAnmHeap[0].getIdx() == getMainBckData(ANM_CANOE_ROW_RIGHT_BACK)->m_upperID)) {
            var_r29 = 0x20022;
        } else {
            var_r29 = 0x20021;
        }

        int var_r27 = cLib_minMaxLimit<int>(((fabsf(var_f31) - mpHIO->mCanoe.m.mStickRowAnmSpeed) / (mpHIO->mCanoe.m.mBtnRowStartAnmSpeed - mpHIO->mCanoe.m.mStickRowAnmSpeed)) * 127.0f, 0, 0x7F);
        mZ2Link.startLinkSwordSound(var_r29, var_r27, mVoiceReverbIntensity);
    }

    return 0;
}

int daAlink_c::checkNextActionCanoe() {
    if ((!checkFishingRodAndLureItem() || mItemAcKeep.getActor() == NULL ||
        ((dmg_rod_class*)mItemAcKeep.getActor())->checkRodEquipPermission()) &&
        checkNextActionFromButton()) {
        return 1;
    }

    if (checkBowAnime()) {
        return checkNextActionBow();
    } else if (checkBoomerangAnimeAndReturnWait()) {
        return checkNextActionBoomerang();
    } else if (checkHookshotAnime()) {
        return checkNextActionHookshot();
    } else if (checkGrabAnimeAndThrow()) {
        return procCanoeGrabMoveInit();
    } else if (checkCanoeFishingWaitAnime()) {
        return procCanoeFishingWaitInit();
    } else {
        return procCanoeWaitInit(0);
    }
}

int daAlink_c::procCanoeRideInit() {
    commonProcInit(PROC_CANOE_RIDE);
    setHeavyBoots(0);
    mRideAcKeep.setData(field_0x27f4);
    deleteEquipItem(0, 0);
    daCanoe_c* canoe = (daCanoe_c*)field_0x27f4;

    setSingleAnime(ANM_HANG_CLIMB_TOP, mpHIO->mWallHang.mWallCatch.m.mClimbAnm.mSpeed,
                   mpHIO->mWallHang.mWallCatch.m.mClimbStartFrame,
                   mpHIO->mWallHang.mWallCatch.m.mClimbAnm.mEndFrame,
                   mpHIO->mWallHang.mWallCatch.m.mClimbAnm.mInterpolation);
    setSpecialGravity(0.0f, maxFallSpeed, 0);

    mNormalSpeed = 0.0f;
    speed.y = 0.0f;
    setRideCanoeBasePos(canoe);

    if ((s16)(fopAcM_searchActorAngleY(canoe, this) - canoe->shape_angle.y) > 0) {
        shape_angle.y = canoe->shape_angle.y - 0x4000;
        mProcVar0.field_0x3008 = 0;
        field_0x37c8.x = 55.0f;
    } else {
        shape_angle.y = canoe->shape_angle.y + 0x4000;
        mProcVar0.field_0x3008 = 1;
        field_0x37c8.x = -55.0f;
    }

    mDoMtx_multVec(canoe->getModelMtx(), &field_0x37c8, &current.pos);
    current.angle.y = shape_angle.y;

    initCanoeRide();
    mProcVar3.field_0x300e = 0x80;
    swimOutAfter(1);
    return 1;
}

int daAlink_c::procCanoeRide() {
    daCanoe_c* canoe = (daCanoe_c*)mRideAcKeep.getActor();
    if (canoe == NULL) {
        return checkNextAction(0);
    }

    mDoMtx_multVec(canoe->getModelMtx(), &field_0x37c8, &current.pos);

    daPy_frameCtrl_c* frameCtrl = mUnderFrameCtrl;
    if (frameCtrl->checkAnmEnd()) {
        procCanoeWaitInit(1);
    } else if (frameCtrl->getFrame() < 9.0f) {
        mProcVar3.field_0x300e += 0x180;

        if (mProcVar0.field_0x3008 == 0) {
            canoe->incShapeAngleZ(-mProcVar3.field_0x300e);
        } else {
            canoe->incShapeAngleZ(mProcVar3.field_0x300e);
        }
        canoe->incShapeOffsetY(-4.0f);
    }

    return 1;
}

int daAlink_c::procCanoeJumpRideInit(fopAc_ac_c* param_0) {
    BOOL var_r31;
    daCanoe_c* canoe = (daCanoe_c*)param_0;

    commonProcInit(PROC_CANOE_JUMP_RIDE);

    if (canoe == NULL) {
        canoe = (daCanoe_c*)fopAcIt_Judge((fopAcIt_JudgeFunc)daAlink_searchCanoe, NULL);
        var_r31 = true;
    } else {
        var_r31 = false;
    }

    setHeavyBoots(0);
    setSingleAnimeParam(ANM_JUMP_LAND, &mpHIO->mAutoJump.m.mLandAnm);
    deleteEquipItem(0, 0);

    mRideAcKeep.setData(canoe);
    setSpecialGravity(0.0f, maxFallSpeed, 0);

    mNormalSpeed = 0.0f;
    speed.y = 0.0f;
    setRideCanoeBasePos(canoe);
    mDoMtx_multVec(canoe->getModelMtx(), &field_0x37c8, &current.pos);
    current.angle.y = shape_angle.y;

    initCanoeRide();
    if (var_r31) {
        procCanoeWaitInit(2);
    }

    return 1;
}

int daAlink_c::procCanoeJumpRide() {
    daCanoe_c* canoe = (daCanoe_c*)mRideAcKeep.getActor();
    if (canoe == NULL) {
        return checkNextAction(0);
    }

    mDoMtx_multVec(canoe->getModelMtx(), &field_0x37c8, &current.pos);

    if (mUnderFrameCtrl[0].checkAnmEnd()) {
        procCanoeWaitInit(1);
    }

    return 1;
}

int daAlink_c::procCanoeGetOffInit() {
    commonProcInit(PROC_CANOE_GETOFF);
    daCanoe_c* canoe = (daCanoe_c*)mRideAcKeep.getActor();

    field_0x2f99 = 0;
    mNormalSpeed = 0.0f;

    setSingleAnimeBaseSpeed(ANM_WAIT, mpHIO->mMove.m.mWaitAnmSpeed, 6.0f);
    setSpecialGravity(0.0f, maxFallSpeed, 0);
    mNormalSpeed = 0.0f;
    speed.y = 0.0f;

    J3DTransformInfo* var_r28 = field_0x2060->getOldFrameTransInfo(0);

    cXyz sp4C(canoe->current.pos.x - cM_scos(shape_angle.y) * 150.0f, canoe->current.pos.y + 100.0f, canoe->current.pos.z + cM_ssin(shape_angle.y) * 150.0f);
    cXyz sp58(canoe->current.pos.x, sp4C.y, canoe->current.pos.z);

    BOOL var_r27;
    if (commonLineCheck(&sp58, &sp4C)) {
        var_r27 = false;
    } else {
        sp4C.x = sp58.x * 2.0f - sp4C.x;
        sp4C.z = sp58.z * 2.0f - sp4C.z;

        if (commonLineCheck(&sp58, &sp4C)) {
            var_r27 = true;
        } else {
            mLinkGndChk.SetPos(&sp4C);
            if (dComIfG_Bgsp().GroundCross(&mLinkGndChk) > canoe->current.pos.y) {
                var_r27 = false;
            } else {
                var_r27 = true;
            }
        }
    }

    if (var_r27) {
        shape_angle.y -= 0x4000;
        setOldRootQuaternion(0, 0x4000, 0);
        var_r28->mTranslate.x -= 100.0f;
    } else {
        shape_angle.y += 0x4000;
        setOldRootQuaternion(0, -0x4000, 0);
        var_r28->mTranslate.x += 100.0f;
    }

    var_r28->mTranslate.z += 70.0f;
    current.angle.y = shape_angle.y;

    setRideCanoeBasePos(canoe);
    field_0x37c8.z -= -35.0f;

    cMtx_multVec(canoe->getModelMtx(), &field_0x37c8, &current.pos);
    mProcVar3.field_0x300e = 0;
    return 1;
}

int daAlink_c::procCanoeGetOff() {
    if (checkCanoeRide()) {
        daCanoe_c* canoe = (daCanoe_c*)mRideAcKeep.getActor();
        if (canoe == NULL) {
            return checkNextAction(0);
        }

        cMtx_multVec(canoe->getModelMtx(), &field_0x37c8, &current.pos);
    }

    if (checkModeFlg(2)) {
        if (mLinkAcch.ChkGroundHit()) {
            checkLandAction(0);
        } else if (speed.y < -gravity) {
            procFallInit(2, mpHIO->mWallHang.mSmallJump.m.mFallInterpolation);
        } else {
            field_0x2f99 = 7;
        }
    } else {
        if (mProcVar3.field_0x300e == 0) {
            if (field_0x2060->getOldFrameRate() < 0.01f) {
                mProcVar3.field_0x300e = 1;
                setSingleAnimeParam(ANM_VJUMP_START, &mpHIO->mWallHang.mSmallJump.m.mSmallJumpAnm);
                field_0x33b0 = l_waitBaseAnime.y;
                field_0x3588 = l_waitBaseAnime;
            }
        } else if (mUnderFrameCtrl[0].checkPass(6)) {
            rideGetOff();
            offModeFlg(0x400);
            setJumpMode();
            setSpecialGravity(mpHIO->mAutoJump.m.mGravity, maxFallSpeed, 0);
            mNormalSpeed = 15.0f;
            speed.y = 20.0f;
        }
    }

    return 1;
}

int daAlink_c::procCanoeWaitInit(int param_0) {
    if (mProcID == PROC_CANOE_FISHING_WAIT) {
        mProcVar5.field_0x3012 = 1;
        return procCanoePaddleGrabInit();
    }

    BOOL temp_r26 = mProcID == PROC_CANOE_GRAB_MOVE;

    if (!commonProcInitNotSameProc(PROC_CANOE_WAIT)) {
        return 0;
    }

    f32 var_f31;
    daAlink_ANM var_r27;

    if (param_0) {
        cXyz sp8;
        daCanoe_c* canoe = (daCanoe_c*)mRideAcKeep.getActor();

        setOldRootQuaternion(0, (s16)(shape_angle.y - canoe->shape_angle.y), 0);
        J3DTransformInfo* temp_r3_2 = field_0x2060->getOldFrameTransInfo(0);

        mDoMtx_stack_c::YrotS(-canoe->shape_angle.y);
        mDoMtx_stack_c::transM(-canoe->current.pos.x, -canoe->current.pos.y, -canoe->current.pos.z);
        mDoMtx_stack_c::multVec(&current.pos, &sp8);

        temp_r3_2->mTranslate.x += sp8.x;
        temp_r3_2->mTranslate.y += sp8.y;
        temp_r3_2->mTranslate.z += sp8.z - 15.0f;

        if (canoe->checkTandem()) {
            temp_r3_2->mTranslate.z -= 175.0f;
        }

        if (param_0 == 2) {
            var_f31 = -1.0f;
            mProcVar0.field_0x3008 = 0;
            mEquipItem = 0x10B;
            var_r27 = ANM_WAIT_CANOE_RIGHT;
            canoe->startInitPaddleWater();
        } else {
            var_f31 = 10.0f;
            mProcVar0.field_0x3008 = 10;
            var_r27 = ANM_WAIT_SIT;
        }
    } else {
        var_f31 = 3.0f;
        mProcVar0.field_0x3008 = 0;

        if (mEquipItem == fpcNm_ITEM_NONE || (mEquipItem != 0x10B && !checkFisingRodLure())) {
            var_r27 = ANM_WAIT_SIT;
        } else if (mProcVar5.field_0x3012 == 0) {
            var_r27 = ANM_WAIT_CANOE_LEFT;
        } else {
            var_r27 = ANM_WAIT_CANOE_RIGHT;
        }
    }

    setSyncCanoePos();
    setSingleAnimeBaseMorf(var_r27, var_f31);

    if (param_0 && mEquipItem != 0x10B && mEquipItem != fpcNm_ITEM_NONE && !checkFisingRodLure()) {
        allUnequip(0);
    }

    mProcVar1.field_0x300a = temp_r26;
    mNormalSpeed = 0.0f;
    speed.y = 0.0f;
    setSpecialGravity(0.0f, maxFallSpeed, 0);
    return 1;
}

int daAlink_c::procCanoeWait() {
    if (canoeCommon() != 0) {
        return 1;
    }

    if (mProcVar0.field_0x3008 > 0) {
        mProcVar0.field_0x3008--;
    } else if (!checkNextActionCanoe()) {
        if (checkNoUpperAnime()) {
            if (mNormalSpeed < 3.0f && !checkFisingRodLure() && !checkCanoeSlider()) {
                if (!setTalkStatus() && !checkStageName("F_SP127")) {
                    setDoStatusEmphasys(0x16);
                }

                if (orderTalk(0) != 0) {
                    return 1;
                }

                if (doTrigger() && dComIfGp_getDoStatus() == 0x16) {
                    if (mEquipItem == 0x10B) {
                        return procCanoePaddlePutInit(1);
                    }

                    if (mEquipItem != fpcNm_ITEM_NONE) {
                        allUnequip(0);
                    }

                    return procCanoeGetOffInit();
                }
            }

            if (checkFisingRodLure()) {
                procCanoeFishingWaitInit();
            } else if (mEquipItem == fpcNm_ITEM_NONE && ((mThrowBoomerangAcKeep.getActor() == NULL && mProcVar1.field_0x300a == 0) || checkInputOnR())) {
                field_0x2fde = 0xFF;

                if (mThrowBoomerangAcKeep.getActor() != NULL || mProcVar1.field_0x300a != 0) {
                    if ((s16)(field_0x2fe2 - shape_angle.y) >= 0) {
                        mProcVar5.field_0x3012 = 1;
                    } else {
                        mProcVar5.field_0x3012 = 0;
                    }
                }

                procCanoePaddleGrabInit();
            } else if (checkInputOnR() && mEquipItem == 0x10B) {
                procCanoeRowInit(-1);
            } else if (mEquipItem != fpcNm_ITEM_NONE && mEquipItem != 0x10B) {
                if (!checkUnderMove0BckNoArc(ANM_WAIT_SIT) && getNowAnmPackUnder(UNDER_0) != getNowAnmPackUpper(UPPER_0)) {
                    setSingleAnimeBase(ANM_WAIT_SIT);
                } else if (checkInputOnR()) {
                    allUnequip(0);

                    if ((s16)(field_0x2fe2 - shape_angle.y) >= 0) {
                        mProcVar5.field_0x3012 = 1;
                    } else {
                        mProcVar5.field_0x3012 = 0;
                    }
                }
            }
        } else if (checkEquipAnime()) {
            if (mEquipItem == 0x10B) {
                procCanoePaddlePutInit(0);
            } else if (field_0x2fde == 0x105 && ((daCanoe_c*)mRideAcKeep.getActor())->getRodID() != fpcM_ERROR_PROCESS_ID_e) {
                procCanoeRodGrabInit();
            }
        }
    }

    return 1;
}

// NONMATCHING - regalloc, equivalent
int daAlink_c::procCanoeRowInit(int param_0) {
    fopAc_ac_c* temp_r26 = mRideAcKeep.getActor();
    field_0x3198 = -2;

    int var_r29;
    if (param_0 < 0) {
        if (checkInputOnR()) {
            s16 sp8 = (s16)(field_0x2fe2 - shape_angle.y);
            int temp_r3 = abs(sp8);

            BOOL spC;
            if (temp_r3 >= 0x6800) {
                spC = 1;
            } else {
                spC = 0;
            }

            int var_r28;
            if (temp_r3 < 0x40 || temp_r3 > 0x7F80) {
                if (param_0 == -3) {
                    if (mProcVar5.field_0x3012 == 0) {
                        var_r28 = 2;
                    } else {
                        var_r28 = 3;
                    }
                } else {
                    if (param_0 == -2) {
                        field_0x3198 = -3;
                    }

                    if (mProcVar5.field_0x3012 == 0) {
                        var_r28 = 3;
                    } else {
                        var_r28 = 2;
                    }
                }
            } else if (sp8 > 0) {
                var_r28 = 2;
            } else {
                var_r28 = 3;
            }

            if (spC == 1) {
                if (var_r28 == 2) {
                    var_r29 = ANM_CANOE_ROW_RIGHT_BACK;
                } else {
                    var_r29 = ANM_CANOE_ROW_LEFT_BACK;
                }

                if ((var_r29 == ANM_CANOE_ROW_RIGHT_BACK && mProcVar5.field_0x3012 == 0) || (var_r29 == ANM_CANOE_ROW_LEFT_BACK && mProcVar5.field_0x3012 == 1)) {
                    return procCanoePaddleShiftInit(var_r29);
                }
            } else {
                if (var_r28 == 2) {
                    var_r29 = ANM_CANOE_ROW_RIGHT;
                } else {
                    var_r29 = ANM_CANOE_ROW_LEFT;
                }

                if ((var_r29 == ANM_CANOE_ROW_RIGHT && mProcVar5.field_0x3012 == 0) || (var_r29 == ANM_CANOE_ROW_LEFT && mProcVar5.field_0x3012 == 1)) {
                    return procCanoePaddleShiftInit(var_r29);
                }
            }
        } else {
            if (param_0 == -3) {
                if (mProcVar5.field_0x3012 == 1) {
                    return procCanoePaddleShiftInit(ANM_CANOE_ROW_LEFT);
                }
                return procCanoePaddleShiftInit(ANM_CANOE_ROW_RIGHT);
            } else if (param_0 == -2) {
                field_0x3198 = -3;
            }

            if (mProcVar5.field_0x3012 == 1) {
                var_r29 = ANM_CANOE_ROW_RIGHT;
            } else {
                var_r29 = ANM_CANOE_ROW_LEFT;
            }
        }

        // Need something here that generates no code in order for debug branches to match.
        (void)0;
    } else {
        var_r29 = param_0;
    }

    commonProcInit(PROC_CANOE_ROW);
    f32 var_f31 = mpHIO->mCanoe.m.mBtnRowAnmSpeed;
    BOOL var_r24 = 0;

    if (var_r29 == ANM_CANOE_ROW_LEFT_BACK || var_r29 == ANM_CANOE_ROW_RIGHT_BACK) {
        mProcVar3.field_0x300e = 1;
        if (temp_r26->speedF > 0.5f * (-var_f31 * mpHIO->mCanoe.m.mBackSpeedRate)) {
            var_r24 = 1;
        }

        field_0x3478 = 32.0f;
        mProcVar0.field_0x3008 = 0;
    } else {
        mProcVar3.field_0x300e = 0;
        if (temp_r26->speedF < 0.5f * (var_f31 * getCanoeSpeedRate())) {
            var_r24 = 1;
        }
    
        field_0x3478 = 26.0f;

        if (param_0 == -1) {
            mProcVar0.field_0x3008 = 1;
        } else {
            mProcVar0.field_0x3008 = 0;
        }
    }

    if (var_r24 != 0) {
        var_f31 = mpHIO->mCanoe.m.mBtnRowStartAnmSpeed;
    }

    setSingleAnime((daAlink_ANM)var_r29, var_f31, 0.0f, 35, 3.0f);
    setSyncCanoePos();
    return 1;
}

int daAlink_c::procCanoeRow() {
    daCanoe_c* temp_r29 = (daCanoe_c*)mRideAcKeep.getActor();
    if (canoeCommon()) {
        return 1;
    }

    daPy_frameCtrl_c* temp_r27 = &mUpperFrameCtrl[0];

    if (!checkItemChangeFromButton()) {
        if (checkEquipAnime()) {
            procCanoePaddlePutInit(0);
        } else if (temp_r27->checkPass(0.0f)) {
            if (checkInputOnR()) {
                procCanoeRowInit(field_0x3198);
            } else {
                procCanoeWaitInit(0);
            }
        } else {
            f32 var_f30 = mpHIO->mCanoe.m.mBtnRowAnmSpeed;
            if ((mProcVar3.field_0x300e == 1 && temp_r29->speedF > 0.5f * (-var_f30 * mpHIO->mCanoe.m.mBackSpeedRate)) || (mProcVar3.field_0x300e == 0 && temp_r29->speedF < 0.5f * (var_f30 * getCanoeSpeedRate()))) {
                var_f30 = mpHIO->mCanoe.m.mBtnRowStartAnmSpeed;
            }

            f32 sp8 = temp_r27->getRate();
            cLib_chaseF(&sp8, var_f30, 0.1f);
            temp_r27->setRate(sp8);

            if (mProcVar0.field_0x3008 == 0 || temp_r27->getFrame() >= 19.0f) {
                s16 var_r28;
                if (checkInputOnR()) {
                    s16 temp_r0 = field_0x2fe2 - shape_angle.y;
                    s16 var_r25;
                    if (mProcVar3.field_0x300e == 1) {
                        var_r25 = field_0x2fe2 + 0x8000;
                    } else {
                        var_r25 = field_0x2fe2;
                    }

                    if (temp_r0 == -0x8000 || (temp_r0 > 0 && mProcVar5.field_0x3012 == 1) || (temp_r0 < 0 && mProcVar5.field_0x3012 == 0)) {
                        temp_r29->setAimAngle(var_r25);
                    }
                    var_r28 = mStickAngle;
                } else {
                    var_r28 = 0x4000;
                }

                f32 var_f31 = temp_r27->getRate();

                if (checkCanoeSlider()) {
                    int temp_r3 = abs(var_r28);
                    if (temp_r3 < 0x6000 && temp_r3 > 0x2000) {
                        var_f31 *= cM_ssin(abs((temp_r3 - 0x4000) * 2));
                    }
                } else {
                    var_f31 *= fabsf(cM_scos(var_r28));
                }

                if (mProcVar3.field_0x300e == 0) {
                    temp_r29->setAimSpeed(var_f31 * getCanoeSpeedRate());
                } else {
                    temp_r29->setAimSpeed(-var_f31 * mpHIO->mCanoe.m.mBackSpeedRate);
                }
            }

            if (temp_r27->checkPass(field_0x3478)) {
                if (mProcVar5.field_0x3012 == 1) {
                    temp_r29->setPaddleOffsetRight();
                } else {
                    temp_r29->setPaddleOffsetLeft();
                }
            }
        }
    }

    return 1;
}

int daAlink_c::procCanoePaddleShiftInit(int param_0) {
    commonProcInit(PROC_CANOE_PADDLE_SHIFT);

    f32 speed;
    if (mProcVar5.field_0x3012 == 0) {
        speed = mpHIO->mCanoe.m.mLeftRightChangeAnm.mSpeed;
    } else {
        speed = -mpHIO->mCanoe.m.mLeftRightChangeAnm.mSpeed;
    }

    setSingleAnime(ANM_CANOE_CHANGE_HAND, speed, mpHIO->mCanoe.m.mLeftRightChangeAnm.mStartFrame, mpHIO->mCanoe.m.mLeftRightChangeAnm.mEndFrame, mpHIO->mCanoe.m.mLeftRightChangeAnm.mInterpolation);

    if (mProcVar5.field_0x3012 == 1) {
        field_0x2f92 = 0;
        field_0x2f93 = 0xFE;
    }

    field_0x3198 = param_0;
    setSyncCanoePos();
    return 1;
}

int daAlink_c::procCanoePaddleShift() {
    if (canoeCommon()) {
        return 1;
    }

    daPy_frameCtrl_c* frame_ctrl = mUpperFrameCtrl;

    if (!checkItemChangeFromButton()) {
        if (checkEquipAnime()) {
            procCanoePaddlePutInit(0);
        } else if (checkAnmEnd(frame_ctrl)) {
            procCanoeRowInit(field_0x3198);
        } else if (frame_ctrl->checkPass(11)) {
            if (mProcVar5.field_0x3012 == 0) {
                field_0x2f92 = 0;
                field_0x2f93 = 0xFE;
                mProcVar5.field_0x3012 = 1;
            } else {
                field_0x2f92 = 0xFE;
                field_0x2f93 = 5;
                mProcVar5.field_0x3012 = 0;
            }
        }
    }

    return 1;
}

int daAlink_c::procCanoePaddlePutInit(int param_0) {
    if (checkCanoeSlider() && mEquipItem == 0x10B) {
        if (checkEquipAnime() && checkBowItem(field_0x2fde)) {
            mEquipItem = fpcNm_ITEM_NONE;
            field_0x2f93 = 0xFE;
            field_0x2f92 = 0xFE;
            return procCanoeWaitInit(0);
        }
    }

    commonProcInit(PROC_CANOE_PADDLE_PUT);

    f32 var_f31;
    if (mProcVar5.field_0x3012 == 0) {
        var_f31 = mpHIO->mCanoe.m.mLeftRightChangeAnm.mSpeed;
    } else {
        var_f31 = -mpHIO->mCanoe.m.mLeftRightChangeAnm.mSpeed;
    }

    setSingleAnime(ANM_CANOE_CHANGE_HAND, var_f31, mpHIO->mCanoe.m.mLeftRightChangeAnm.mStartFrame, mpHIO->mCanoe.m.mLeftRightChangeAnm.mEndFrame, mpHIO->mCanoe.m.mLeftRightChangeAnm.mInterpolation);

    if (mProcVar5.field_0x3012 == 1) {
        field_0x2f92 = 0;
        field_0x2f93 = 0xFE;
    }

    mProcVar3.field_0x300e = param_0;

    setSyncCanoePos();
    ((daCanoe_c*)mRideAcKeep.getActor())->setAimSpeed(0.0f);

    if (param_0 == 0 && field_0x2fde == 0x105) {
        mItemAcKeep.setID(((daCanoe_c*)mRideAcKeep.getActor())->getRodID());
        mItemAcKeep.setActor();
    }

    return 1;
}

int daAlink_c::procCanoePaddlePut() {
    if (canoeCommon()) {
        return 1;
    }

    if (mUpperAnmHeap[0].getIdx() == getMainBckData(ANM_CANOE_CHANGE_HAND)->m_upperID) {
        if (mUpperFrameCtrl[0].checkPass(11)) {
            setSingleAnimeBaseSpeed(ANM_CANOE_PADDLE_TO_ROD, -1.0f, 3.0f);
            field_0x2f92 = 0;
            field_0x2f93 = 0xFE;
        }
    } else {
        daPy_frameCtrl_c* temp_r29 = &mUnderFrameCtrl[0];

        if (checkAnmEnd(temp_r29)) {
            if (mProcVar3.field_0x300e != 0) {
                return procCanoeGetOffInit();
            }

            if (mItemAcKeep.getActor() != NULL) {
                initFishingRodHand();
                procCanoeFishingWaitInit();
            } else {
                itemEquip(field_0x2fde);
                procCanoeWaitInit(0);
            }
        } else if (temp_r29->getFrame() < 5.0f) {
            if (mItemAcKeep.getActor() != NULL) {
                mEquipItem = 0x105;
                field_0x2f93 = 5;
            } else {
                mEquipItem = fpcNm_ITEM_NONE;
                field_0x2f93 = 0xFE;
            }

            field_0x2f92 = 0xFE;
        }
    }

    return 1;
}

int daAlink_c::procCanoePaddleGrabInit() {
    commonProcInit(PROC_CANOE_PADDLE_GRAB);
    setSingleAnimeBase(ANM_CANOE_PADDLE_TO_ROD);
    setSyncCanoePos();
    return 1;
}

int daAlink_c::procCanoePaddleGrab() {
    if (canoeCommon()) {
        return 1;
    }

    if (checkUnderMove0BckNoArc(ANM_CANOE_PADDLE_TO_ROD) != 0) {
        daPy_frameCtrl_c* temp_r29 = &mUnderFrameCtrl[0];
        
        if (checkAnmEnd(temp_r29) != 0) {
            f32 var_f31;
            if (mProcVar5.field_0x3012 == 0) {
                var_f31 = -mpHIO->mCanoe.m.mLeftRightChangeAnm.mSpeed;
            } else {
                var_f31 = mpHIO->mCanoe.m.mLeftRightChangeAnm.mSpeed;
            }

            setSingleAnime(ANM_CANOE_CHANGE_HAND, var_f31, mpHIO->mCanoe.m.mLeftRightChangeAnm.mStartFrame, mpHIO->mCanoe.m.mLeftRightChangeAnm.mEndFrame, mpHIO->mCanoe.m.mLeftRightChangeAnm.mInterpolation);
            mUpperFrameCtrl[0].setFrame(11.0f);
            getNowAnmPackUpper(UPPER_0)->setFrame(11.0f);

            if (mProcVar5.field_0x3012 == 1) {
                field_0x2f92 = 0;
                field_0x2f93 = 0xFE;
            } else {
                field_0x2f92 = 0xFE;
                field_0x2f93 = 5;
            }
        } else if (temp_r29->getFrame() >= 5.0f) {
            if (checkFisingRodLure()) {
                ((daCanoe_c*)mRideAcKeep.getActor())->onRodID(mItemAcKeep.getID());
                mItemAcKeep.clearData();
                deleteEquipItem(0, 0);
            }

            field_0x2f92 = 0xFE;

            if (field_0x2fde == 0xFF) {
                mEquipItem = 0x10B;
            } else {
                itemEquip(field_0x2fde);
                return procCanoeWaitInit(0);
            }
        }
    } else if (checkAnmEnd(mUpperFrameCtrl)) {
        procCanoeWaitInit(0);
    }

    return 1;
}

int daAlink_c::procCanoeRodGrabInit() {
    commonProcInit(PROC_CANOE_ROD_GRAB);

    if (mEquipItem != fpcNm_ITEM_NONE) {
        allUnequip(0);
        mProcVar0.field_0x3008 = 1;
    } else {
        setSingleAnimeBase(ANM_CANOE_PADDLE_TO_ROD);
        mProcVar0.field_0x3008 = 0;
    }

    setSyncCanoePos();
    return 1;
}

int daAlink_c::procCanoeRodGrab() {
    if (canoeCommon()) {
        return 1;
    }

    if (mProcVar0.field_0x3008 != 0) {
        if (checkNoUpperAnime()) {
            mProcVar0.field_0x3008 = 0;
            setSingleAnimeBase(ANM_CANOE_PADDLE_TO_ROD);
        }
    } else if (mUnderFrameCtrl[0].getFrame() >= 5.0f) {
        mItemAcKeep.setID(((daCanoe_c*)mRideAcKeep.getActor())->getRodID());
        mItemAcKeep.setActor();

        if (mItemAcKeep.getActor() != NULL) {
            initFishingRodHand();
            mEquipItem = 0x105;
            procCanoeFishingWaitInit();
        } else {
            procCanoeWaitInit(0);
        }
    }

    return 1;
}

int daAlink_c::procCanoeFishingWaitInit() {
    if (!commonProcInitNotSameProc(PROC_CANOE_FISHING_WAIT)) {
        field_0x311a = 0;
        return 0;
    }

    setSingleAnimeBaseSpeed(ANM_CANOE_REEL, 0.0f, 3.0f);
    setUpperAnimeBaseSpeed(0x5D, 0.0f, -1.0f);

    f32 temp_f31 = (1.0f - ((mBodyAngle.y + 0x4000) / 32768.0f)) * mUnderFrameCtrl[0].getEnd();
    mUnderFrameCtrl[0].setFrame(temp_f31);
    getNowAnmPackUnder(UNDER_0)->setFrame(temp_f31);
    mUpperFrameCtrl[2].setFrame(temp_f31);
    getNowAnmPackUpper(UPPER_2)->setFrame(temp_f31);

    mItemVar0.field_0x3018 = 1;
    setSyncCanoePos();

    mProcVar2.field_0x300c = 0;
    mProcVar3.field_0x300e = 0;
    mProcVar0.field_0x3008 = 0;
    mProcVar3.field_0x300e = cLib_minMaxLimit<s16>((s16)(field_0x311a - shape_angle.y), -0x3800, 0x3800);
    field_0x311a = 0;
    return 1;
}

int daAlink_c::procCanoeFishingWait() {
    if (canoeCommon()) {
        return 1;
    }

    if (checkFisingRodLure() && mItemAcKeep.getActor() == NULL) {
        return 1;
    }

    daPy_frameCtrl_c* temp_r29 = &mUpperFrameCtrl[2];
    daPy_frameCtrl_c* temp_r28 = &mUnderFrameCtrl[0];

    if (mProcVar2.field_0x300c != 0) {
        if (checkNoResetFlg1(FLG1_UNK_8000)) {
            mProcVar2.field_0x300c = 0;
            setUpperAnimeBase(0x68);
        }
    } else if (checkExchangeRodAnime()) {
        if (checkAnmEnd(temp_r29)) {
            procCanoeFishingReelInit();
        } else if (temp_r29->getFrame() >= 8.0f) {
            mItemVar0.field_0x3018 = 0;
        }
    } else if (!checkNextActionCanoe() && checkCanoeFishingWaitAnime()) {
        if (mProcVar0.field_0x3008 == 0) {
            if (daPy_addCalcShort(&mBodyAngle.y, mProcVar3.field_0x300e, 2, 0x800, 0x200) == 0) {
                mProcVar0.field_0x3008 = 1;
            }
        } else if (checkInputOnR() && ((mStickAngle > 0x2000 && mStickAngle < 0x6000) || (mStickAngle < -0x2000 && mStickAngle > -0x6000))) {
            s16 var_r27;
            if (mStickAngle > 0x2000 && mStickAngle < 0x6000) {
                var_r27 = 0x3800;
            } else {
                var_r27 = -0x3800;
            }

            daPy_addCalcShort(&mBodyAngle.y, var_r27, 5, field_0x33a8 * 1024.0f, 1);
        }

        f32 temp_f31 = (1.0f - ((mBodyAngle.y + 0x4000) / 32768.0f)) * temp_r28->getEnd();
        temp_r28->setFrame(temp_f31);
        temp_r29->setFrame(temp_f31);
    }
    return 1;
}

int daAlink_c::procCanoeFishingReelInit() {
    commonProcInit(PROC_CANOE_FISHING_REEL);
    setDoubleAnime(0.0f, 0.0f, 0.0f, ANM_REEL, ANM_REEL_LEFT_RIGHT, 2, 5.0f);
    mUnderFrameCtrl[1].setFrame(0.5f * mUnderFrameCtrl[1].getEnd());
    setUnderAnime(0x25B, UNDER_2, 1.0f, 0.0f, -1, -1.0f);
    setArmReelAnime();
    setSyncCanoePos();
    return 1;
}

int daAlink_c::procCanoeFishingReel() {
    if (canoeCommon()) {
        mItemVar0.field_0x3018 = 1;
        return 1;
    }

    setFishGetFace();

    if (checkEndResetFlg0(ERFLG0_FISHING_ROD_GET_FISH)) {
        procCanoeFishingGetInit();
    } else if (checkEndResetFlg0(ERFLG0_UNK_20000)) {
        resetUpperAnime(UPPER_2, -1.0f);
        procCanoeFishingWaitInit();
    } else if (checkEndResetFlg0(ERFLG0_UNK_10000)) {
        setUpperAnimeBase(0x17C);
    } else if (checkUpperAnime(0x17C)) {
        if (mUpperFrameCtrl[2].checkAnmEnd()) {
            setArmReelAnime();
            field_0x2060->initOldFrameMorf(3.0f, field_0x30a8, field_0x30aa);
        }
    } else {
        if (((dmg_rod_class*)mItemAcKeep.getActor())->checkReelSpin()) {
            onResetFlg0(RFLG0_UNK_80000);
        }

        setFishingWaitAction();

        fopAc_ac_c* temp_r3 = mItemAcKeep.getActor();
        if (temp_r3 != NULL) {
            daPy_addCalcShort(&mBodyAngle.y, cLib_minMaxLimit<s16>((cLib_targetAngleY(&field_0x3834, &temp_r3->eyePos) - shape_angle.y), -0x3800, 0x3800), 5, 0x400, 1);
        }
    }

    return 1;
}

int daAlink_c::procCanoeFishingGetInit() {
    commonProcInit(PROC_CANOE_FISHING_GET);

    daAlink_ANM var_r30;
    if (mBodyAngle.y >= 0) {
        var_r30 = ANM_CANOE_FISH_LEFT;
        field_0x3478 = 24.0f;
        field_0x347c = 45.0f;
        mProcVar3.field_0x300e = 0;
    } else {
        var_r30 = ANM_CANOE_FISH_RIGHT;
        field_0x3478 = 14.0f;
        field_0x347c = 30.0f;
        mProcVar3.field_0x300e = 1;
    }

    setSyncCanoePos();
    setSingleAnimeBaseMorf(var_r30, 5.0f);
    return 1;
}

int daAlink_c::procCanoeFishingGet() {
    daPy_frameCtrl_c* temp_r30 = &mUnderFrameCtrl[0];

    if (canoeCommon()) {
        mItemVar0.field_0x3018 = 1;
        return 1;
    }

    setFishGetFace();

    if (checkUnderMove0BckNoArc(ANM_CANOE_RELEASE)) {
        if (checkAnmEnd(temp_r30)) {
            procCanoeFishingWaitInit();
        } else if (temp_r30->getFrame() >= 65.0f) {
            mItemVar0.field_0x3018 = 1;
            field_0x2f92 = 4;
            field_0x2f93 = 5;
        } else if (temp_r30->getFrame() >= 29.0f) {
            field_0x2f93 = 0xFE;
        }
    } else if (checkAnmEnd(temp_r30)) {
        if (checkEndResetFlg0(ERFLG0_UNK_4000000)) {
            setSingleAnimeBaseMorf(ANM_CANOE_RELEASE, 3.0f);
            field_0x2fc8 = 0;
            offModeFlg(0x100);
        } else if (checkEndResetFlg2(ERFLG2_UNK_1)) {
            field_0x2fc8 = 0;
            procCanoeFishingWaitInit();
        }
    } else if (temp_r30->checkPass(field_0x3478)) {
        field_0x2f93 = 5;
    } else if (temp_r30->checkPass(field_0x347c)) {
        onModeFlg(0x100);
    }

    return 1;
}

int daAlink_c::procCanoeSubjectivityInit() {
    commonProcInit(PROC_CANOE_SUBJECTIVITY);
    setSyncCanoePos();
    setSubjectMode();
    return 1;
}

int daAlink_c::procCanoeSubjectivity() {
    onResetFlg0(RFLG0_UNK_4000000);

    s16 temp_r30 = shape_angle.y;
    s16 temp_r29 = current.angle.y;
    int temp_r28 = field_0x3190;

    if (canoeCommon()) {
        return 1;
    }

    shape_angle.y = temp_r30;

    if (checkSubjectEnd(1)) {
        procCanoeWaitInit(0);
    } else if (checkNextActionFromButton()) {
        if (checkUpperReadyThrowAnime()) {
            field_0x3190 = temp_r28;
            mBodyAngle.y = field_0x3190;
            field_0x310a = mBodyAngle.x;
            field_0x310c = shape_angle.y + field_0x3190;
        }
        return 1;
    } else {
        setRideSubjectAngle(temp_r29);
    }

    return 1;
}

int daAlink_c::procCanoeBowSubjectInit() {
    if (!commonProcInitNotSameProc(PROC_CANOE_BOW_SUBJECT)) {
        return 0;
    }

    setSyncCanoePos();
    setBowOrSlingStatus();
    return 1;
}

int daAlink_c::procCanoeBowSubject() {
    setDoStatus(0x12);

    s16 temp_r30 = shape_angle.y;
    s16 temp_r29 = current.angle.y;

    if (canoeCommon()) {
        return 1;
    }

    shape_angle.y = temp_r30;

    if (!checkNextActionCanoe() && setRideSubjectAngle(temp_r29)) {
        setBowSight();
    }

    return 1;
}

int daAlink_c::procCanoeBowMoveInit() {
    if (!commonProcInitNotSameProc(PROC_CANOE_BOW_MOVE)) {
        return 0;
    }

    setSyncCanoePos();
    setBowOrSlingStatus();
    return 1;
}

int daAlink_c::procCanoeBowMove() {
    setDoStatus(0x12);

    if (canoeCommon()) {
        return 1;
    }

    cancelBowMove();

    if (!checkNextActionCanoe()) {
        setBodyAngleRideReadyAnime();
    }

    return 1;
}

int daAlink_c::procCanoeGrabMoveInit() {
    if (!commonProcInitNotSameProc(PROC_CANOE_GRAB_MOVE)) {
        return 0;
    }

    setSyncCanoePos();
    return 1;
}

int daAlink_c::procCanoeGrabMove() {
    if (canoeCommon()) {
        return 1;
    }

    if (mGrabItemAcKeep.getActor() == NULL && checkGrabAnime()) {
        resetUpperAnime(UPPER_2, 3.0f);
    }

    if (checkGrabThrowAnime()) {
        if (checkUpperGrabItemThrow(3.0f)) {
            procCanoeWaitInit(0);
        }
    } else {
        checkNextActionCanoe();
    }

    return 1;
}

int daAlink_c::procCanoeBoomerangSubjectInit() {
    if (!commonProcInitNotSameProc(PROC_CANOE_BOOMERANG_SUBJECT)) {
        return 0;
    }

    setSyncCanoePos();
    initBoomerangUpperAnimeSpeed(1);
    dComIfGp_setPlayerStatus0(0, 0x80000);
    return 1;
}

int daAlink_c::procCanoeBoomerangSubject() {
    if (!checkItemActorPointer()) {
        return 1;
    }

    if (checkBoomerangReadyAnime()) {
        setDoStatus(0x12);
    }

    s16 temp_r30 = shape_angle.y;
    s16 temp_r29 = current.angle.y;

    if (canoeCommon()) {
        return 1;
    }

    shape_angle.y = temp_r30;

    if (!checkNextActionCanoe()) {
        if (setRideSubjectAngle(temp_r29)) {
            setBoomerangSight();
        }
    } else {
        mSight.offDrawFlg();
    }

    return 1;
}

int daAlink_c::procCanoeBoomerangMoveInit() {
    if (!commonProcInitNotSameProc(PROC_CANOE_BOOMERANG_MOVE)) {
        return 0;
    }

    setSyncCanoePos();
    initBoomerangUpperAnimeSpeed(0);
    dComIfGp_setPlayerStatus0(0, 0x80000);
    return 1;
}

int daAlink_c::procCanoeBoomerangMove() {
    if (!checkItemActorPointer()) {
        return 1;
    }

    if (checkBoomerangReadyAnime()) {
        setDoStatus(0x12);
    }

    if (canoeCommon()) {
        return 1;
    }

    if (!checkNextActionCanoe()) {
        setBodyAngleRideReadyAnime();
    }

    return 1;
}

int daAlink_c::procCanoeHookshotSubjectInit() {
    if (!commonProcInitNotSameProc(PROC_CANOE_HOOKSHOT_SUBJECT)) {
        return 0;
    }

    setSyncCanoePos();
    initHookshotUpperAnimeSpeed(1);
    dComIfGp_setPlayerStatus0(0, 0x4000);
    return 1;
}

int daAlink_c::procCanoeHookshotSubject() {
    if (checkHookshotWait()) {
        setDoStatus(0x12);
    }

    s16 temp_r30 = shape_angle.y;
    s16 temp_r29 = current.angle.y;

    if (canoeCommon()) {
        return 1;
    }

    shape_angle.y = temp_r30;

    if (!checkNextActionCanoe()) {
        if (setRideSubjectAngle(temp_r29)) {
            setHookshotSight();
        }
    } else {
        mSight.offDrawFlg();
    }

    return 1;
}

int daAlink_c::procCanoeHookshotMoveInit() {
    if (!commonProcInitNotSameProc(PROC_CANOE_HOOKSHOT_MOVE)) {
        return 0;
    }

    setSyncCanoePos();
    initHookshotUpperAnimeSpeed(0);
    dComIfGp_setPlayerStatus0(0, 0x4000);
    return 1;
}

int daAlink_c::procCanoeHookshotMove() {
    cancelHookshotMove();

    if (checkHookshotWait()) {
        setDoStatus(0x12);
    }

    if (canoeCommon()) {
        return 1;
    }

    if (!checkNextActionCanoe()) {
        setBodyAngleRideReadyAnime();
    }

    return 1;
}

int daAlink_c::procCanoeBottleDrinkInit(u16 param_0) {
    if (!dComIfGp_event_compulsory(this, NULL, 0xFFFF)) {
        return 0;
    }

    mDemo.setSpecialDemoType();
    commonProcInit(PROC_CANOE_BOTTLE_DRINK);
    setSyncCanoePos();

    daCanoe_c* temp_r3 = (daCanoe_c*)mRideAcKeep.getActor();
    temp_r3->speedF = 0.0f;
    temp_r3->setAimSpeed(0.0f);
    temp_r3->setAimAngle(shape_angle.y);

    setUpperAnimeParam(getMainBckData(ANM_BOTTLE_DRINK_START)->m_upperID, UPPER_2, &mpHIO->mItem.mBottle.m.mStartDrinkAnm);
    setFacePriAnime(ANM_BOTTLE_DRINK_START);
    keepItemData();
    setBottleModel(param_0);
    dComIfGp_setPlayerStatus1(0, 0x2000);

    field_0x319c = 1;
    dCam_getBody()->StartEventCamera(0x12, fopAcM_GetID(this), "Type", 1, &field_0x319c, 0);
    mProcVar2.field_0x300c = 0;
    return 1;
}

int daAlink_c::procCanoeBottleDrink() {
    if (canoeCommon()) {
        resetUpperAnime(UPPER_2, 3.0f);
        returnKeepItemData();
        resetSpecialEvent();
        return 1;
    }

    commonBottleDrink(1);
    return 1;
}

int daAlink_c::procCanoeKandelaarPourInit() {
    if (!dComIfGp_event_compulsory(this, NULL, 0xFBFF)) {
        return 0;
    }

    mDemo.setSpecialDemoType();
    commonProcInit(PROC_CANOE_KANDELAAR_POUR);
    setSyncCanoePos();

    daCanoe_c* canoe = (daCanoe_c*)mRideAcKeep.getActor();
    canoe->speedF = 0.0f;
    canoe->setAimSpeed(0.0f);
    canoe->setAimAngle(shape_angle.y);

    setUpperAnimeParam(getMainBckData(ANM_OIL_BOTTLE_POUR_START)->m_upperID, UPPER_2, &mpHIO->mItem.mLantern.m.mBeginUnkAnm);
    commonKandelaarPourInit();
    return 1;
}

int daAlink_c::procCanoeKandelaarPour() {
    if (canoeCommon()) {
        resetUpperAnime(UPPER_2, 3.0f);
        resetOilBottleModel();
        resetSpecialEvent();
        return 1;
    }

    if (commonKandelaarPour(1)) {
        setUpperAnimeBaseMorf(0x268, -1.0f);
    }

    return 1;
}

void daAlink_c::setGroundFishingRodActor() {
    mItemAcKeep.setID(fopAcM_create(PROC_MG_ROD, 0xFFFF011D, &mLeftHandPos, -1, NULL, NULL, -1));
    initFishingRodHand();
}

int daAlink_c::procFishingCastInit() {
    if (!commonProcInitNotSameProc(PROC_FISHING_CAST)) {
        return 1;
    }

    setSingleAnimeBaseSpeed(ANM_WAIT, mpHIO->mMove.m.mWaitAnmSpeed, 3.0f);
    setUpperAnimeBase(0x260);
    field_0x2f96 = 0xFE;
    field_0x2f97 = 8;
    mNormalSpeed = 0.0f;

    mProcVar2.field_0x300c = 0;
    mProcVar3.field_0x300e = 0;
    mItemVar0.field_0x3018 = 1;

    if (checkFishingRodItem(mEquipItem)) {
        onNoResetFlg2(FLG2_UNK_20000000);
    }

    return 1;
}

int daAlink_c::procFishingCast() {
    setFishGetFace();

    if (checkEndResetFlg0(ERFLG0_UNK_4000000)) {
        setFaceBasicAnime(ANM_WAIT);
        field_0x2fc8 = 0;
    }

    if (checkEndResetFlg0(ERFLG0_UNK_10000) && mItemMode != 0xFF) {
        mItemMode = 0xFF;
        dComIfGs_setBaitItem(0xFF);

        if (mEquipItem == fpcNm_ITEM_JEWEL_WORM_ROD || mEquipItem == fpcNm_ITEM_JEWEL_BEE_ROD) {
            mEquipItem = fpcNm_ITEM_JEWEL_ROD;
        } else {
            mEquipItem = fpcNm_ITEM_FISHING_ROD_1;
        }
    }

    if (!checkNoResetFlg2(FLG2_UNK_20000000)) {
        resetUpperAnime(UPPER_2, -1.0f);
        onEndResetFlg0(ERFLG0_UNK_1000);
        return checkWaitAction();
    }

    if (checkInputOnR()) {
        cLib_addCalcAngleS(&shape_angle.y, field_0x2fe2, mpHIO->mMove.m.mTurnAngleRate, mpHIO->mMove.m.mMaxTurnAngle, mpHIO->mMove.m.mMinTurnAngle);
        current.angle.y = shape_angle.y;
    }

    if (mProcVar3.field_0x300e != 0) {
        mUpperFrameCtrl[2].setRate(0.0f);
        mUpperFrameCtrl[2].setFrame(0.0f);
        getNowAnmPackUpper(UPPER_2)->setFrame(0.0f);

        mUnderFrameCtrl[0].setRate(0.0f);
        mUnderFrameCtrl[0].setFrame(0.0f);
        getNowAnmPackUnder(UNDER_0)->setFrame(0.0f);
    } else if (mUnderFrameCtrl[0].getRate() < 0.01f) {
        mUnderFrameCtrl[0].setRate(mpHIO->mMove.m.mWaitAnmSpeed);
        mUpperFrameCtrl[2].setRate(1.0f);
    }

    return 1;
}

int daAlink_c::procFishingFoodInit() {
    if (!dComIfGp_event_compulsory(this, NULL, 0xFFFF)) {
        return 0;
    }

    mDemo.setSpecialDemoType();
    commonProcInit(PROC_FISHING_FOOD);
    setSingleAnime(ANM_BOTTLE_OPEN, 1.0f, 0.0f, 21, 3.0f);
    mProcVar3.field_0x300e = mSelectItemId;

    if (checkFishingRodItem(mEquipItem)) {
        mProcVar4.field_0x3010 = 0;
        mSelectItemId = checkItemSetButton(mEquipItem);
    } else {
        mProcVar4.field_0x3010 = 1;
        keepItemData();
        deleteEquipItem(0, 0);
        mSelectItemId = checkItemSetButton(0x108);
        mEquipItem = dComIfGp_getSelectItem(mSelectItemId);
        setItemActor();
    }

    mNormalSpeed = 0.0f;
    current.angle.y = shape_angle.y;
    mProcVar2.field_0x300c = 0;
    return 1;
}

int daAlink_c::procFishingFood() {
    if (mUnderFrameCtrl[0].checkAnmEnd()) {
        if (mProcVar2.field_0x300c != 0) {
            if (mProcVar4.field_0x3010 != 0) {
                returnKeepItemData();
            }
            resetSpecialEvent();
        } else {
            setSingleAnime(ANM_BOTTLE_OPEN, -1.0f, 0.0f, mUnderFrameCtrl[0].getEnd(), -1.0f);
            mProcVar2.field_0x300c = 1;

            u32 temp_r3 = dComIfGp_getSelectItem(mProcVar3.field_0x300e);
            if (temp_r3 == fpcNm_ITEM_WORM || dComIfGp_getSelectItemNum(mProcVar3.field_0x300e) == 1) {
                dComIfGs_setEquipBottleItemEmpty(mProcVar3.field_0x300e);
            } else {
                dComIfGp_addSelectItemNum(mProcVar3.field_0x300e, -1);
            }

            if (mItemMode != fpcNm_ITEM_NONE) {
                dComIfGs_setEquipBottleItemIn(mProcVar3.field_0x300e, mItemMode);
                if (mItemMode == fpcNm_ITEM_BEE_CHILD) {
                    dComIfGp_setSelectItemNum(mProcVar3.field_0x300e, 1);
                }
            }

            mItemMode = temp_r3;

            BOOL temp_r28 = checkFisingRodJewl();
            if (temp_r3 == fpcNm_ITEM_WORM) {
                if (temp_r28) {
                    mEquipItem = fpcNm_ITEM_JEWEL_WORM_ROD;
                } else {
                    mEquipItem = fpcNm_ITEM_WORM_ROD;
                }
            } else if (temp_r28) {
                mEquipItem = fpcNm_ITEM_JEWEL_BEE_ROD;
            } else {
                mEquipItem = fpcNm_ITEM_BEE_ROD;
            }

            dComIfGs_setBaitItem(temp_r3);
        }
    }

    return 1;
}
