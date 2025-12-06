/**
 * d_a_alink_bow.inc
 * Player Bow and Arrow action handling
 */

#include "d/actor/d_a_alink.h"
#include "d/d_com_inf_game.h"
#include "d/actor/d_a_arrow.h"
#include "d/actor/d_a_horse.h"

void daAlink_c::getArrowFlyData(f32* o_distance, f32* o_speed, int i_forceCharge) const {
    if (mEquipItem == fpcNm_ITEM_PACHINKO) {
        *o_distance = mpHIO->mItem.mBow.m.mSlingshotDistance;
        *o_speed = mpHIO->mItem.mBow.m.mSlingshotSpeed;
    } else if (dComIfGp_checkPlayerStatus0(0, 0x200000)) {
        *o_distance = mpHIO->mItem.mBow.m.mScopeArrowDistance;
        *o_speed = mpHIO->mItem.mBow.m.mScopeArrowSpeed;
    } else if (field_0x301c == 0 || i_forceCharge) {
        if (field_0x301e == 1) {
            *o_distance = mpHIO->mItem.mBow.m.mChargeBombArrowDistance;
            *o_speed = mpHIO->mItem.mBow.m.mChargeBombArrowSpeed;
        } else {
            *o_distance = mpHIO->mItem.mBow.m.mChargeArrowDistance;
            *o_speed = mpHIO->mItem.mBow.m.mChargeArrowSpeed;
        }
    } else if (field_0x301e == 1) {
        *o_distance = mpHIO->mItem.mBow.m.mBombArrowDistance;
        *o_speed = mpHIO->mItem.mBow.m.mBombArrowSpeed;
    } else {
        *o_distance = mpHIO->mItem.mBow.m.mArrowDistance;
        *o_speed = mpHIO->mItem.mBow.m.mArrowSpeed;
    }
}

f32 daAlink_c::getArrowIncAtR() const {
    return mpHIO->mItem.mBow.m.mArrowAttackRadius;
}

s16 daAlink_c::getBombArrowFlyExplodeTime() const {
    return mpHIO->mItem.mBow.m.mBombArrowFlyExplodeTime;
}

f32 daAlink_c::getArrowIncAtMaxStart() const {
    return mpHIO->mItem.mBow.m.mArrowIncAttackMaxStart;
}

f32 daAlink_c::getArrowIncAtMax() const {
    return mpHIO->mItem.mBow.m.mArrowIncAttackMax;
}

BOOL daAlink_c::checkBowAndSlingItem(int i_itemNo) {
    return checkBowItem(i_itemNo) || i_itemNo == fpcNm_ITEM_PACHINKO;
}

void daAlink_c::setSlingModel() {
    J3DAnmTransform* bck = (J3DAnmTransform*)mAnmHeap9.loadDataIdx(0x1F9);
    JKRHeap* prev_heap = setItemHeap();

    J3DModelData* model_data = loadAramBmd(0x319, 0x2C00);
    mHeldItemModel = initModel(model_data, 0);
    mItemBck.init(bck, 0, 2, 1.0f, 0, -1, false);

    mDoExt_setCurrentHeap(prev_heap);
    field_0x2f95 = 5;
    field_0x2f94 = 103;
}

cXyz* daAlink_c::checkBowCameraArrowPosP(s16* param_0, s16* param_1) {
    if (!checkBowAnime() || mEquipItem == fpcNm_ITEM_PACHINKO) {
        return NULL;
    }

    s16 var_28;
    if (checkHorseRide() && checkBoarSingleBattle() && dComIfGp_getHorseActor() != NULL) {
        var_28 = dComIfGp_getHorseActor()->shape_angle.y;
    } else if (checkMagneBootsOn()) {
        var_28 = field_0x3118;
    } else {
        var_28 = shape_angle.y;
    }

    var_28 += mBodyAngle.y;

    mDoMtx_stack_c::transS(field_0x3834);
    concatMagneBootMtx();
    mDoMtx_stack_c::ZXYrotM(mBodyAngle.x, var_28, 0);

    static Vec const localPos = {3.45f, 45.32f, -7.0f};
    mDoMtx_stack_c::multVec(&localPos, &mHeldItemRootPos);

    *param_0 = mBodyAngle.x;
    *param_1 = var_28;
    return &mHeldItemRootPos;
}

bool daAlink_c::checkArrowChargeEnd() const {
    return checkBowChargeWaitAnime() && field_0x301c == 0;
}

BOOL daAlink_c::checkBowReadyAnime() const {
    return checkBowReloadAnime() || checkBowChargeWaitAnime() || checkBowWaitAnime();
}

BOOL daAlink_c::checkBowAnime() const {
    return checkBowReadyAnime() || checkBowShootAnime();
}

void daAlink_c::makeArrow() {
    if (dComIfGs_getArrowNum() != 0) {
        if (field_0x301e == 1 && dComIfGp_getSelectItemNum(mSelectItemId) == 0) {
            field_0x301e = 0;
        }

        fopAc_ac_c* arrow_p = daArrow_c::makeArrow(this, field_0x301e);
        mItemAcKeep.setData(arrow_p);
    }

    mItemVar0.field_0x3018 = 0;
}

void daAlink_c::deleteArrow() {
    daArrow_c* arrow_p = (daArrow_c*)mItemAcKeep.getActor();

    if (arrow_p != NULL && fopAcM_GetName(arrow_p) == PROC_ARROW) {
        if (arrow_p->checkBombArrow() &&
            (mDamageTimer == mpHIO->mDamage.m.mInvincibleTime || checkModeFlg(8)))
        {
            arrow_p->setBombArrowExplode();
            dComIfGp_addSelectItemNum(mSelectItemId, -1);
            dComIfGp_setItemArrowNumCount(-1);
        } else {
            fopAcM_delete(arrow_p);
        }
        mItemAcKeep.clearData();
    }
}

void daAlink_c::setBowOrSlingStatus() {
    if (mEquipItem == fpcNm_ITEM_PACHINKO) {
        dComIfGp_setPlayerStatus0(0, 0x40);
    } else {
        dComIfGp_setPlayerStatus0(0, 0x1000);
    }
}

void daAlink_c::changeArrowType() {
    if (checkCanoeSlider() || (mEquipItem == fpcNm_ITEM_BOW && field_0x301e == 0) ||
        (mEquipItem == fpcNm_ITEM_BOMB_ARROW && field_0x301e == 0 &&
         dComIfGp_getSelectItemNum(mSelectItemId) == 0) ||
        (mEquipItem == fpcNm_ITEM_HAWK_ARROW && checkAttentionLock()))
    {
        return;
    }

    setItemActionButtonStatus(0x11);

    if (arrowChangeTrigger()) {
        if (mEquipItem == fpcNm_ITEM_HAWK_ARROW) {
            if (dComIfGp_checkPlayerStatus0(0, 0x200000)) {
                dComIfGp_clearPlayerStatus0(0, 0x200000);
                seStartSystem(Z2SE_AL_HAWK_EYE_PUTOFF);
            } else {
                dComIfGp_setPlayerStatus0(0, 0x200000);
                seStartSystem(Z2SE_AL_HAWK_EYE_PUTON);
            }
        } else {
            if (field_0x301e == 1) {
                field_0x301e = 0;
            } else {
                field_0x301e = 1;
            }

            if (mItemAcKeep.getActor() != NULL) {
                deleteArrow();
                makeArrow();
                setBowReloadAnime();
            }
        }
    }
}


BOOL daAlink_c::cancelBowMoveRideNotAtn() {
    return checkModeFlg(0x400) && checkAttentionLock() && mTargetedActor == NULL;
}

BOOL daAlink_c::cancelBowMove() {
    if (mTargetedActor == NULL && !checkAttentionLock() && mFastShotTime == 0 &&
        !checkBowReadyAnime() && (!checkBowShootAnime() || mUpperFrameCtrl[2].checkAnmEnd()))
    {
        resetUpperAnime(UPPER_2, -1.0f);
        return true;
    }

    return false;
}

void daAlink_c::setBowReadyAnime() {
    if (!checkBowAnime()) {
        if (mEquipItem == fpcNm_ITEM_BOMB_ARROW) {
            if (dComIfGp_getSelectItemNum(mSelectItemId) != 0) {
                field_0x301e = 1;
            } else {
                field_0x301e = 0;
            }
        } else {
            field_0x301e = 0;
        }
    }

    setUpperAnimeBase(0xD);

    if (checkResetFlg0(RFLG0_UNK_80)) {
        mItemMode = 0;
    } else {
        mItemMode = 3;
    }

    field_0x2f97 = 5;
    field_0x2f96 = 4;

    if (mEquipItem == fpcNm_ITEM_PACHINKO) {
        changeItemBck(0x1F1, -1.0f);
    } else {
        changeItemBck(0x43, -1.0f);
    }
}

void daAlink_c::setBowReloadAnime() {
    setUpperAnimeParam(9, UPPER_2, &mpHIO->mItem.mBow.m.mLoadAnm);
    setFacePriTexture(FTANM_UNK_8);
    setFacePriBck(0xE5);

    if (mEquipItem == fpcNm_ITEM_PACHINKO) {
        changeItemBck(0x1F0, mpHIO->mItem.mBow.m.mLoadAnm.mStartFrame);
        field_0x2f97 = 5;
        seStartOnlyReverb(Z2SE_AL_PACHINKO_DRAW);
    } else {
        changeItemBck(0x41, mpHIO->mItem.mBow.m.mLoadAnm.mStartFrame);
        field_0x2f97 = 7;
        seStartOnlyReverb(Z2SE_AL_DRAW_BOW);
    }

    field_0x2f96 = 0;
    field_0x301c = mpHIO->mItem.mBow.m.mChargeArrowTime;
}

BOOL daAlink_c::checkUpperItemActionBow() {
    daPy_frameCtrl_c* frameCtrl_p = &mUpperFrameCtrl[2];

    if (mEquipItem == fpcNm_ITEM_PACHINKO) {
        static Vec const slingLocalPos = {10.0f, 10.0f, 0.0f};
        mDoMtx_multVec(getLeftItemMatrix(), &slingLocalPos, &mHeldItemRootPos);
    }

    if (checkBowChargeWaitAnime()) {
        daArrow_c* arrow_p = (daArrow_c*)mItemAcKeep.getActor();

        if (mEquipItem != fpcNm_ITEM_PACHINKO) {
            if (field_0x301c > 0) {
                field_0x301c--;

                if (field_0x301c <= frameCtrl_p->getEnd() && frameCtrl_p->getFrame() < 0.1f &&
                    frameCtrl_p->getRate() < 0.1f)
                {
                    frameCtrl_p->setRate(1.0f);
                }
            } else {
                onResetFlg0(RFLG0_UNK_10);
                cancelItemUseQuake(0);
            }
        }

        if (checkReadyItem()) {
            if (mItemVar0.field_0x3018 != 0) {
                setUpperAnimeParam(0xC, UPPER_2, &mpHIO->mItem.mBow.m.mShootAnm);
                setFacePriBck(0xE8);
                field_0x2f96 = 0;

                u16 var_r29;
                if (mEquipItem == fpcNm_ITEM_PACHINKO) {
                    var_r29 = 0x1F1;
                    if (dComIfGs_getPachinkoNum() != 0 &&
                        daArrow_c::makeSlingStone(this, &mHeldItemRootPos) != NULL)
                    {
                        dComIfGp_setItemPachinkoNumCount(-1);
                    }
                } else if (field_0x301c != 0) {
                    var_r29 = 0x43;
                } else {
                    var_r29 = 0x44;
                }

                changeItemBck(var_r29, mpHIO->mItem.mBow.m.mShootAnm.mStartFrame);
                mItemVar0.field_0x3018 = 0;
            } else if (mEquipItem == fpcNm_ITEM_PACHINKO) {
                if (!itemButton()) {
                    dComIfGp_getVibration().StartShock(1, 1, cXyz(0.0f, 1.0f, 0.0f));
                    seStartOnlyReverb(Z2SE_AL_PACHINKO_SHOT);
                    mItemVar0.field_0x3018 = 1;
                    resetFacePriAnime();
                }
            } else if (!itemButton()) {
                if (arrow_p != NULL) {
                    u32 sound;
                    int shock_strength;

                    if (field_0x301c != 0) {
                        arrow_p->setShoot();
                        shock_strength = 1;

                        if (arrow_p->checkBombArrow()) {
                            sound = Z2SE_OBJ_ARROWBOMB_SHOT;
                        } else {
                            sound = Z2SE_OBJ_ARROW_SHOT_NORMAL;
                        }
                    } else {
                        arrow_p->setChargeShoot();
                        shock_strength = 2;

                        if (arrow_p->checkBombArrow()) {
                            sound = Z2SE_OBJ_ARROWBOMB_SHOTGAIN;
                        } else {
                            sound = Z2SE_OBJ_ARROW_SHOT_GAIN;
                        }
                    }

                    dComIfGp_getVibration().StartShock(shock_strength, 1, cXyz(0.0f, 1.0f, 0.0f));
                    seStartOnlyReverb(sound);

                    if (field_0x301e == 1) {
                        dComIfGp_addSelectItemNum(mSelectItemId, -1);
                    }

                    dComIfGp_setItemArrowNumCount(-1);
                    mItemAcKeep.clearData();
                    onResetFlg0(RFLG0_UNK_20000000);
                    field_0x30a4 = mpHIO->mItem.mBow.m.mBombArrowHoldTime;
                } else {
                    seStartOnlyReverb(Z2SE_AL_ARROW_EMPTY);
                }

                mItemVar0.field_0x3018 = 1;
                resetFacePriAnime();

                if (mEquipItem == fpcNm_ITEM_BOW) {
                    field_0x301e = 0;
                }
            } else {
                changeArrowType();
            }
        }
    } else if (checkBowReloadAnime()) {
        if (checkAnmEnd(frameCtrl_p)) {
            setUpperAnimeBaseSpeed(10, 0.0f, -1.0f);
            setFacePriTexture(FTANM_UNK_9);
            setFacePriBck(0xE6);

            if (mEquipItem != fpcNm_ITEM_PACHINKO) {
                changeItemBck(0x42, 0.0f);
                dComIfGp_getVibration().StartQuake(1, 1, cXyz(0.0f, 1.0f, 0.0f));
                onNoResetFlg3(FLG3_UNK_4);
            }
        }
    } else if (checkBowWaitAnime()) {
        if ((mItemVar0.field_0x3018 == 0 && mFastShotTime != 0) ||
            (mItemMode == 0 && !itemButton() && mItemVar0.field_0x3018 == 0 &&
             cancelBowMoveRideNotAtn()))
        {
            resetUpperAnime(UPPER_2, 3.0f);
            dComIfGp_clearPlayerStatus0(0, 0x1040);
            return 1;
        }

        if (mItemMode == 0 && (itemButton() || mItemVar0.field_0x3018 != 0) && checkReadyItem() &&
            (field_0x30a4 == 0 || field_0x301e != 1))
        {
            setBowReloadAnime();

            if (mEquipItem == fpcNm_ITEM_PACHINKO) {
                mItemVar0.field_0x3018 = 0;
            } else {
                makeArrow();
            }
        } else if (mEquipItem == fpcNm_ITEM_HAWK_ARROW) {
            changeArrowType();
        }

        if (mItemMode != 0) {
            mItemMode--;
        }
    } else if (checkBowShootAnime()) {
        if (mUpperFrameCtrl[2].getFrame() >= 6.0f) {
            field_0x2f97 = 5;
        } else if (mUpperFrameCtrl[2].getFrame() >= 1.0f) {
            field_0x2f96 = 4;
        }

        if (checkAnmEnd(&mUpperFrameCtrl[2]) && cancelBowMoveRideNotAtn()) {
            resetUpperAnime(UPPER_2, 3.0f);
            dComIfGp_clearPlayerStatus0(0, 0x1040);
            return 1;
        }

        if (checkAnmEnd(&mUpperFrameCtrl[2]) ||
            (mUpperFrameCtrl[2].getFrame() > mpHIO->mItem.mBow.m.mShootAnm.mCancelFrame &&
             itemButton()))
        {
            setBowReadyAnime();
        }

        if (mItemMode != 0) {
            mItemMode--;
        }
    }

    if (checkBowWaitAnime() || (mEquipItem == fpcNm_ITEM_PACHINKO && checkBowChargeWaitAnime())) {
        field_0x33dc += mpHIO->mItem.mBow.m.mShootAnm.mSpeed;
    } else {
        field_0x33dc = mUpperFrameCtrl[2].getFrame();
    }

    return cancelUpperItemReadyAnime(0);
}


void daAlink_c::checkUpperItemActionBowFly() {
    if (checkBowAnime()) {
        checkUpperItemActionBow();
        setBodyAngleXReadyAnime(0);
    } else if (checkReadyItem() && itemTrigger()) {
        setBowReadyAnime();
        mItemMode = 0;
    }
}

int daAlink_c::checkNextActionBow() {
    if (mFastShotTime != 0 && checkBowReadyAnime()) {
        mFastShotTime--;
    }

    BOOL var_r31 = false;
    if (!checkBowAnime()) {
        setBowReadyAnime();
        setFastShotTimer();

        if (!checkAttentionLock()) {
            field_0x2fe4 = shape_angle.y;
        }

        mItemMode = 0;
        mItemVar0.field_0x3018 = 1;

        if (mEquipItem == fpcNm_ITEM_HAWK_ARROW) {
            var_r31 = true;
            mFastShotTime = 0;
        }

        if ((!checkAttentionLock() && !checkDashAnime()) || checkModeFlg(0x400)) {
            mFastShotTime = 0;

            if (checkAttentionLock() && checkModeFlg(0x400)) {
                mItemVar0.field_0x3018 = 1;
            } else {
                mItemVar0.field_0x3018 = 0;
            }

            if (!checkResetFlg0(RFLG0_UNK_80)) {
                mItemMode = 3;
            }
        }
    }

    BOOL rt;
    if (!checkAttentionLock() && mFastShotTime == 0) {
        if (checkModeFlg(0x400)) {
            if (checkCanoeRide()) {
                rt = procCanoeBowSubjectInit();
            } else {
                rt = procHorseBowSubjectInit();
            }
        } else {
            rt = procBowSubjectInit();
        }
    } else {
        if (checkModeFlg(0x400)) {
            if (checkCanoeRide()) {
                rt = procCanoeBowMoveInit();
            } else {
                rt = procHorseBowMoveInit();
            }
        } else {
            rt = procBowMoveInit();
        }
    }

    if (rt && var_r31) {
        dComIfGp_setPlayerStatus0(0, 0x200000);
        seStartSystem(Z2SE_AL_HAWK_EYE_PUTON);
    }

    return rt;
}


void daAlink_c::setBowModel() {
    J3DAnmTransform* bck = (J3DAnmTransform*)mAnmHeap9.loadDataIdx(0x59);
    JKRHeap* old_heap = setItemHeap();

    mHeldItemModel = initModel(loadAramBmd(0x314, 0x4C00), 0);
    mItemBck.init(bck, FALSE, 2, 1.0f, 0, -1, false);

    mDoExt_setCurrentHeap(old_heap);
    field_0x33dc = mItemBck.getBckAnm()->getFrameMax() - 0.001f;
    field_0x2f95 = 5;
    field_0x2f94 = 0x67;
}


bool daAlink_c::checkBowGrabLeftHand() const {
    return checkReinRide() && !checkBowAnime();
}

void daAlink_c::setBowHangAnime() {
    if (checkBowItem(mEquipItem)) {
        changeItemBck(0x58, 0.0f);
    }
}

void daAlink_c::setBowNormalAnime() {
    if (checkBowItem(mEquipItem)) {
        changeItemBck(0x59, -1.0f);
    } else if (mEquipItem == fpcNm_ITEM_PACHINKO) {
        changeItemBck(0x1F9, -1.0f);
    }
}

void daAlink_c::setBowSight() {
    if (checkBowChargeWaitAnime() && !dComIfGp_checkPlayerStatus0(0, 0x200000)) {
        cXyz sight_pos;
        f32 dist;
        f32 speed;

        getArrowFlyData(&dist, &speed, TRUE);
        checkSightLine(dist, &sight_pos);
        mSight.setPos(&sight_pos);
        mSight.offDrawFlg();
    } else {
        mSight.offDrawFlg();
    }
}

int daAlink_c::procBowSubjectInit() {
    if (!commonProcInitNotSameProc(PROC_BOW_SUBJECT)) {
        return 0;
    }

    setSingleAnimeBaseSpeed(ANM_ATN_WAIT_RIGHT, 0.0f,
                            mpHIO->mItem.mBow.m.mStartInterpolation);
    mNormalSpeed = 0.0f;
    setBowOrSlingStatus();
    return 1;
}

int daAlink_c::procBowSubject() {
    setDoStatus(0x12);
    if (!checkNextAction(0) && setBodyAngleToCamera()) {
        setBowSight();
    }

    return 1;
}

int daAlink_c::procBowMoveInit() {
    if (!commonProcInitNotSameProc(PROC_BOW_MOVE)) {
        return 0;
    }

    if (checkZeroSpeedF()) {
        onModeFlg(1);
    }

    setBlendAtnMoveAnime(mpHIO->mItem.mBow.m.mStartInterpolation);
    mProcVar2.field_0x300c = 0;
    setBowOrSlingStatus();
    return 1;
}

int daAlink_c::procBowMove() {
    cancelBowMove();

    if (mFastShotTime == 0 && mProcVar2.field_0x300c != 0 && checkBowWaitAnime() && !itemButton()) {
        resetUpperAnime(UPPER_2, -1.0f);
    }

    mProcVar2.field_0x300c = 1;
    onEndResetFlg0(ERFLG0_UNK_8000000);

    if (!checkNextAction(0)) {
        f32 var_f31 = -1.0f;

        if (checkZeroSpeedF()) {
            onModeFlg(1);

            if (field_0x2f98 != 3) {
                field_0x2f98 = 3;
                var_f31 = mpHIO->mBasic.m.mBasicInterpolation;
            }
            current.angle.y = shape_angle.y - 0x4000;
        } else {
            offModeFlg(1);
        }

        setBlendAtnMoveAnime(var_f31);
        setBodyAngleXReadyAnime(0);
    }

    return 1;
}
