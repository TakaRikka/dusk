/**
 * d_a_alink_link.inc
 * General Player Human action handling
 */

#include "JSystem/J3DGraphBase/J3DDrawBuffer.h"
#include "d/actor/d_a_alink.h"
#include "d/actor/d_a_canoe.h"
#include "d/actor/d_a_kytag05.h"
#include "d/actor/d_a_tag_mstop.h"
#include "d/actor/d_a_tag_mhint.h"

bool daAlink_c::checkNoSubjectModeCamera() {
    return dCam_getBody()->Type() == dCam_getBody()->GetCameraTypeFromCameraName("Rotary") ||
           dCam_getBody()->Type() == dCam_getBody()->GetCameraTypeFromCameraName("Rampart2") ||
           dCam_getBody()->Type() == dCam_getBody()->GetCameraTypeFromCameraName("Allay") ||
           dCam_getBody()->Type() == dCam_getBody()->GetCameraTypeFromCameraName("AllayR") ||
           dCam_getBody()->Type() == dCam_getBody()->GetCameraTypeFromCameraName("AllayR2") ||
           dCam_getBody()->Type() == dCam_getBody()->GetCameraTypeFromCameraName("AllayS") ||
           checkCargoCarry() ||
           dCam_getBody()->Type() == dCam_getBody()->GetCameraTypeFromCameraName("StreetP") ||
           dCam_getBody()->Type() == dCam_getBody()->GetCameraTypeFromCameraName("Street");
}

bool daAlink_c::acceptSubjectModeChange() {
    return dCam_getBody()->ChangeModeOK(4) &&
           !checkNoSubjectModeCamera() &&
           !checkAttentionLock() &&
           !checkHorseLieAnime() &&
           !checkEventRun() &&
           !checkModeFlg(0x310040) &&
           (checkNoUpperAnime() || checkHorseTiredAnime() || checkGrabAnime() || checkWolfGrabAnime()) &&
           !(checkWolf() && checkWolfWaitSlipPolygon() && mLinkAcch.ChkGroundHit());
}

int daAlink_c::checkSubjectAction() {
    if (acceptSubjectModeChange()) {
        onResetFlg0(RFLG0_UNK_4000000);

        if (dComIfGp_checkCameraAttentionStatus(field_0x317c, 0x1000)) {
            if (checkModeFlg(0x400)) {
                if (checkCanoeRide()) {
                    return procCanoeSubjectivityInit();
                } else if (checkBoardRide()) {
                    return procBoardSubjectivityInit();
                } else {
                    return procHorseSubjectivityInit();
                }
            }

            if (checkModeFlg(0x20000)) {
                return procWolfRopeSubjectivityInit();
            }

            if (checkModeFlg(0x40000)) {
                return procCoSwimSubjectivityInit();
            }

            return procCoSubjectivityInit();
        }
    }

    return 0;
}

s16 daAlink_c::checkBodyAngleX(s16 param_0) {
    if (checkWolf() || mProcID == PROC_PEEP_SUBJECTIVITY || mProcID == PROC_HOOKSHOT_WALL_SHOOT) {
        return param_0;
    }

    Vec sp2C = lit_5908;
    sp2C.y = 0.5f * field_0x598;

    cXyz sp20;
    mDoMtx_multVecZero(mpLinkModel->getAnmMtx(1), &sp20);
    cXyz sp14;

    if (param_0 >= 0) {
        sp2C.z = 25.0f;
    } else {
        sp2C.z = -25.0f;
    }

    mDoMtx_stack_c::transS(sp20);
    concatMagneBootMtx();
    mDoMtx_stack_c::ZXYrotM(param_0, shape_angle.y, 0);
    mDoMtx_stack_c::multVec(&sp2C, &sp14);

    if (commonLineCheck(&sp20, &sp14)) {
        cXyz sp8 = sp14 - mLinkLinChk.GetCross();
        multVecMagneBootInvMtx(&sp8);

        current.pos.x -= sp8.x;
        current.pos.z -= sp8.z;
    }

    return param_0;
}

BOOL daAlink_c::setBodyAngleToCamera() {
    if (dComIfGp_checkCameraAttentionStatus(field_0x317c, 0x10)) {
        field_0x310e = field_0x310a;
        field_0x3110 = field_0x310c;
        onEndResetFlg2(ERFLG2_UNK_20);

        s16 sp8;
        if (checkInputOnR()) {
            f32 var_f31 = 512.0f * field_0x33ac * field_0x33ac;
            if (checkWolfEyeUp()) {
                var_f31 *= 0.6f;
            }

            if (dComIfGp_checkPlayerStatus0(0, 0x200000)) {
                var_f31 /= dComIfGp_getCameraZoomScale(field_0x317c);
            }

            shape_angle.y = shape_angle.y + (var_f31 * cM_ssin(mStickAngle));
            sp8 = mBodyAngle.x + (var_f31 * cM_scos(mStickAngle));

            if (checkNotItemSinkLimit() && sp8 > 0 && sp8 > mBodyAngle.x) {
                sp8 = mBodyAngle.x;
            }
        } else {
            sp8 = mBodyAngle.x;
        }

        if (checkNotItemSinkLimit() && sp8 > 0) {
            cLib_addCalcAngleS(&sp8, 0, 5, 0x1000, 0x400);
        }

        s16 var_r29;
        if (!dComIfGp_checkPlayerStatus0(0, 0x202000) && mEquipItem == fpcNm_ITEM_IRONBALL) {
            var_r29 = -10000;
        } else {
            var_r29 = mpHIO->mItem.m.mItemFPUpMaxUnk;
        }

        mBodyAngle.x = cLib_minMaxLimit<s16>((s16)sp8, var_r29, mpHIO->mItem.m.mItemFPMaxUnk);
        checkBodyAngleX(mBodyAngle.x);
        field_0x310a = mBodyAngle.x;
        field_0x310c = shape_angle.y;
        return 1;
    }

    return 0;
}

void daAlink_c::setSubjectMode() {
    dComIfGp_setPlayerStatus0(0, 0x2000);
    seStartSystem(Z2SE_SUBJ_VIEW_IN);
}

BOOL daAlink_c::subjectCancelTrigger() {
    if (checkWolf() || checkNotBattleStage() || !checkSwordGet() ||
        (checkModeFlg(0x40000) && !checkEquipHeavyBoots()))
    {
        return itemTriggerCheck(BTN_B);
    }

    return false;
}

int daAlink_c::checkSubjectEnd(BOOL i_playSound) {
    setDoStatus(0x12);

    if (checkEventRun() || checkEquipAnime() || doTrigger() || checkSetItemTrigger(fpcNm_ITEM_HAWK_EYE) || subjectCancelTrigger() || checkEndResetFlg0(ERFLG0_FORCE_SUBJECT_CANCEL) || dComIfGp_checkCameraAttentionStatus(field_0x317c, 0x2000)) {
        if (i_playSound) {
            seStartSystem(Z2SE_SUBJ_VIEW_OUT);
        }

        onResetFlg0(RFLG0_UNK_10000000);
        return true;
    }

    return false;
}

void daAlink_c::searchPeepObj(fopAc_ac_c* i_actor, void* i_data) {
    if (fopAcM_GetName(i_actor) == PROC_KYTAG05) {
        f32 var_f31 = current.pos.abs2(i_actor->current.pos);
        if (var_f31 < field_0x3478) {
            field_0x3478 = var_f31;
            *(fopAc_ac_c**)i_data = i_actor;
        }
    }
}

static int daAlink_searchPeepObj(fopAc_ac_c* i_actor, void* i_data) {
    daAlink_getAlinkActorClass()->searchPeepObj(i_actor, i_data);
    return 0;
}

int daAlink_c::procCoSubjectivityInit() {
    commonProcInit(PROC_SUBJECTIVITY);
    mNormalSpeed = 0.0f;

    setSubjectMode();
    field_0x2fe6 = shape_angle.y;

    if (checkWolf()) {
        setSingleAnimeWolfBaseSpeed(WANM_WAIT, 0.0f, 3.0f);
    } else {
        setSingleAnimeBaseSpeed(ANM_WAIT, 0.0f, 3.0f);
    }

    return 1;
}

int daAlink_c::procCoSubjectivity() {
    onResetFlg0(RFLG0_UNK_4000000);
    setTalkStatus();

    if (orderTalk(1)) {
        return 1;
    }

    if ((checkWolf() && checkWolfGroundSpecialMode()) || (!checkWolf() && checkGroundSpecialMode())) {
        return 1;
    }

    if (checkSubjectEnd(FALSE) || (dComIfGp_checkPlayerStatus0(0, 0x200000) && checkSetItemTrigger(fpcNm_ITEM_HAWK_EYE))) {
        seStartSystem(Z2SE_SUBJ_VIEW_OUT);
        checkWaitAction();
    } else if (!checkWolf() && (checkItemAction() || checkItemChangeFromButton())) {
        if (checkUpperReadyThrowAnime()) {
            field_0x310a = mBodyAngle.x;
        }

        return 1;
    } else {
        setBodyAngleToCamera();
    }

    return 1;
}

int daAlink_c::procCoSwimSubjectivityInit() {
    commonProcInit(PROC_SWIM_SUBJECTIVITY);
    mNormalSpeed = 0.0f;

    setSubjectMode();

    if (checkWolf()) {
        setSingleAnimeWolfBaseSpeed(WANM_SWIM_WAIT, mpHIO->mWolf.mWlSwim.m.mIdleAnmSpeedWeak,
                                    mpHIO->mWolf.mWlSwim.m.mIdleInterp);
    } else {
        setSingleAnimeBaseSpeed(ANM_SWIM_WAIT, mpHIO->mSwim.m.mWaitAnmSpeed,
                                mpHIO->mSwim.m.mWaitInterpolation);
    }

    dComIfGp_setPlayerStatus0(0, 0x100000);
    mProcVar2.field_0x300c = 0;

    return 1;
}

int daAlink_c::procCoSwimSubjectivity() {
    onResetFlg0(RFLG0_UNK_4000000);
    setTalkStatus();

    if (orderTalk(1)) {
        return 1;
    }

    if (checkNoResetFlg0(FLG0_SWIM_UP)) {
        setSwimUpDownOffset();
    }

    if (checkSubjectEnd(FALSE)) {
        seStartSystem(Z2SE_SUBJ_VIEW_OUT);

        if (checkWolf()) {
            procWolfSwimWaitInit(0);
        } else {
            procSwimWaitInit(0);
        }
    } else {
        setBodyAngleToCamera();
    }

    return 1;
}

int daAlink_c::procCoPeepSubjectivityInit() {
    commonProcInit(PROC_PEEP_SUBJECTIVITY);
    mNormalSpeed = 0.0f;

    if (checkWolf()) {
        setSingleAnimeWolfBaseSpeed(WANM_WAIT, 0.0f, 3.0f);
    } else {
        setSingleAnimeBaseSpeed(ANM_WAIT, 0.0f, 3.0f);
    }

    onPlayerNoDraw();
    setSpecialGravity(0.0f, maxFallSpeed, 0);
    speed.y = 0.0f;
    dComIfGp_setPlayerStatus0(0, 0x2000);
    field_0x3478 = 10000000.0f;
    mProcVar2.field_0x300c = shape_angle.y;

    return 1;
}

int daAlink_c::procCoPeepSubjectivity() {
    if (doTrigger() || peepSubjectCancelTrigger()) {
        kytag05_class* peep = NULL;
        fopAcIt_Executor((fopAcIt_ExecutorFunc)daAlink_searchPeepObj, &peep);

        if (peep != NULL) {
            mPeepExitID = peep->getSceneListID();
        }

        if (dComIfGp_event_compulsory(this, 0, 0xFFFF)) {
            startPeepChange();
        }
    }

    return 1;
}

u32 daAlink_c::checkBoardRide() const {
    return mRideStatus == 4;
}

u32 daAlink_c::checkCanoeRide() const {
    return mRideStatus == 3;
}

u32 daAlink_c::checkHorseRide() const {
    return mRideStatus == 1;
}

MtxP daAlink_c::getLeftItemMatrix() {
    return mpLinkModel->getAnmMtx(field_0x30c0);
}

u32 daAlink_c::checkBoarRide() const {
    return mRideStatus == 2;
}

u32 daAlink_c::checkSpinnerRide() const {
    return mRideStatus == 5;
}

MtxP daAlink_c::getLeftHandMatrix() {
    return mpLinkModel->getAnmMtx(field_0x30b8);
}

MtxP daAlink_c::getRightHandMatrix() {
    return mpLinkModel->getAnmMtx(field_0x30ba);
}

void daAlink_c::onSceneChangeArea(u8 exitID, u8 exitDirection, fopAc_ac_c* scexAc) {
    mExitID = exitID;
    mExitDirection = exitDirection;
    mpScnChg = (daScex_c*)scexAc;
}

MtxP daAlink_c::getRightItemMatrix() {
    return mpLinkModel->getAnmMtx(field_0x30c2);
}

u32 daAlink_c::checkPlayerNoDraw() {
    return dComIfGp_checkCameraAttentionStatus(field_0x317c, 2) ||
           checkNoResetFlg0(FLG0_PLAYER_NO_DRAW);
}

daAlink_c::daAlink_c()
    : mAnmHeap3(0), mAnmHeap4(0), mFaceBtpHeap(0x800), mFaceBtkHeap(0x400), mFaceBckHeap(0xC00),
      mAnmHeap9(0x800) {}
