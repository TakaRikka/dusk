/**
 * d_a_alink_horse.inc
 * Player Epona riding handling
 */

#include "d/actor/d_a_alink.h"
#include "d/actor/d_a_horse.h"
#include "d/actor/d_a_hozelda.h"
#include "d/actor/d_a_e_wb.h"
#include "d/actor/d_a_obj_iceleaf.h"
#include "d/actor/d_a_spinner.h"

static void* daAlink_searchBoar(fopAc_ac_c* i_actor, void* i_data) {
    UNUSED(i_data);

    if (fopAcM_GetName(i_actor) == PROC_E_WB) {
        if (((e_wb_class*)i_actor)->getModel() != NULL) {
            return i_actor;
        }
    }

    return NULL;
}

static fopAc_ac_c* daAlink_searchCoach(fopAc_ac_c* i_actor, void* i_data) {
    UNUSED(i_data);

    if (fopAcM_GetName(i_actor) == PROC_NPC_COACH) {
        return i_actor;
    }

    return NULL;
}

BOOL daAlink_c::checkHorseZeldaBowMode() {
    if (dComIfGp_getHorseActor() == NULL) {
        return false;
    }

    daHoZelda_c* zelda_p = dComIfGp_getHorseActor()->getZeldaActor();
    return zelda_p != NULL && zelda_p->checkBowMode();
}

void daAlink_c::setHorseZeldaDamage() {
    daHoZelda_c* zelda_p;

    if (dComIfGp_getHorseActor() != NULL) {
        zelda_p = dComIfGp_getHorseActor()->getZeldaActor();
        if (zelda_p != NULL) {
            zelda_p->setDamageInit();
        }
    }
}

BOOL daAlink_c::checkHorseDashAccept() {
    return !checkStageName("F_SP103");
}

BOOL daAlink_c::checkCowGame() {
    return checkStageName("F_SP00") &&
           (dComIfGp_getStartStageLayer() == 4 || dComIfGp_getStartStageLayer() == 5);
}

int daAlink_c::getReinRideDirection() {
    s16 angle = fopAcM_searchActorAngleY(field_0x27f4, this) - field_0x27f4->shape_angle.y;

    if (fopAcM_GetName(field_0x27f4) == PROC_HORSE && abs(angle) > 0x6800) {
        return DIR_BACKWARD;
    } else if (angle > 0) {
        return DIR_LEFT;
    }

    return DIR_RIGHT;
}

Vec const daAlink_c::m_handLeftOutSidePos = {
    9.0f, -5.0f, 3.0f
};

Vec const daAlink_c::m_handRightOutSidePos = {
    9.0f, -5.0f, -3.0f
};

Vec const daAlink_c::m_handLeftInSidePos = {
    9.0f, 5.0f, 3.0f
};

Vec const daAlink_c::m_handRightInSidePos = {
    9.0f, 5.0f, -3.0f
};

int daAlink_c::checkReinRideBgCheck() {
    static const Vec horseLocalLeft = {20.0f, 0.0f, 0.0f};
    static const Vec horseLocalRight = {-20.0f, 0.0f, 0.0f};
    static const Vec horseLocalBack = {0.0f, 0.0f, -200.0f};
    static const Vec boarLocalLeft = {200.0f, 0.0f, -18.0f};
    static const Vec boarLocalRight = {-200.0f, 0.0f, -18.0f};

    int direction = getReinRideDirection();

    const Vec* offset_p;
    if (fopAcM_GetName(field_0x27f4) == PROC_HORSE) {
        if (direction == DIR_BACKWARD) {
            offset_p = &horseLocalBack;
        } else if (direction == DIR_LEFT) {
            offset_p = &horseLocalLeft;
        } else {
            offset_p = &horseLocalRight;
        }
    } else if (direction == DIR_LEFT) {
        offset_p = &boarLocalLeft;
    } else {
        offset_p = &boarLocalRight;
    }

    cXyz line_start_pos;
    cXyz line_end_pos(field_0x27f4->current.pos.x, field_0x27f4->current.pos.y, field_0x27f4->current.pos.z);

    mDoMtx_stack_c::transS(field_0x27f4->current.pos);
    mDoMtx_stack_c::YrotM(field_0x27f4->shape_angle.y);
    mDoMtx_stack_c::multVec(offset_p, &line_start_pos);

    dBgS_AcchCir* acchcir_p = mAcchCir;
    for (int i = 0; i < 6; i++) {
        line_start_pos.y = current.pos.y + acchcir_p->GetWallH();
        line_end_pos.y = line_start_pos.y;

        if (commonLineCheck(&line_start_pos, &line_end_pos)) {
            return 0;
        }

        if (i == 2) {
            acchcir_p = mAcchCir;
            line_start_pos.x = current.pos.x;
            line_start_pos.z = current.pos.z;
        } else {
            acchcir_p++;
        }
    }

    return 1;
}

void daAlink_c::commonInitForceRideRein() {
    setHeavyBoots(0);
    onModeFlg(0x400);
    field_0x2fc0 = 0;
    field_0x2fab = 0x13;
    field_0x3002 = 0;
    field_0x3004 = 0;
    field_0x30a6 = 0;

    initServiceWaitTime();
    attention_info.field_0xa = 0x46;
    mUnderFrameCtrl[0].onEndFlg();
}

int daAlink_c::initForceRideBoar() {
    e_wb_class* boar = (e_wb_class*)fopAcIt_Judge((fopAcIt_JudgeFunc)daAlink_searchBoar, NULL);
    fopAc_ac_c* boar_actor = (fopAc_ac_c*)boar;

    if (boar == NULL) {
        return 0;
    }

    commonInitForceRideRein();
    boar_actor->current.pos = current.pos;
    boar_actor->shape_angle.y = shape_angle.y;
    boar_actor->current.angle.y = shape_angle.y;
    mRideStatus = RIDETYPE_BOAR;

    mRideAcKeep.setData(boar_actor);
    field_0x384c = (cXyz*)&l_boarBaseAnime;
    boar->setPlayerRide();
    return 1;
}

void daAlink_c::initForceRideHorse() {
    commonInitForceRideRein();
    mRideStatus = RIDETYPE_HORSE;
    mRideAcKeep.setData(dComIfGp_getHorseActor());
    field_0x384c = (cXyz*)&l_horseBaseAnime;
    dComIfGp_getHorseActor()->onRideFlg();
    mZ2Link.setRiding(true);
}

void daAlink_c::rideGetOff() {
    if (checkHorseRide()) {
        daHorse_c* horse_p = dComIfGp_getHorseActor();
        if (horse_p != NULL) {
            horse_p->offRideFlg();
        }

        mZ2Link.setRiding(false);
    } else if (checkBoarRide()) {
        e_wb_class* boar_p = (e_wb_class*)mRideAcKeep.getActor();
        if (boar_p != NULL) {
            boar_p->getOff();
        }
    } else if (checkSpinnerRide()) {
        daSpinner_c* spinner_p = (daSpinner_c*)mRideAcKeep.getActor();
        if (spinner_p != NULL) {
            spinner_p->forceDelete();
        }

        seStartOnlyReverb(Z2SE_AL_SPINNER_END);
    } else if (checkCanoeRide()) {
        if (mEquipItem == 0x10B) {
            mEquipItem = fpcNm_ITEM_NONE;
        }
    } else if (checkBoardRide()) {
        static_cast<daObjIceLeaf_c*>(mRideAcKeep.getActor())->offRide();

        if (checkEventRun()) {
            static_cast<daObjIceLeaf_c*>(mRideAcKeep.getActor())->setBreakEffect();
        }
    }

    if (!checkBoardRide() || checkEventRun()) {
        mRideAcKeep.clearData();
    }

    mRideStatus = 0;
    offNoResetFlg1(daPy_FLG1(FLG1_UNK_1000 | FLG1_UNK_800));
    attention_info.field_0xa = 10;
    shape_angle.x = 0;
    shape_angle.z = 0;
}

BOOL daAlink_c::checkHorseNotDamageReaction() const {
    return mProcID == PROC_HORSE_TURN || mProcID == PROC_HORSE_JUMP || mProcID == PROC_HORSE_HANG ||
           mProcID == PROC_HORSE_LAND;
}

BOOL daAlink_c::checkHorseWaitLashAnime() const {
    return checkHorseUnderDashStartAnime() && checkModeFlg(1);
}

BOOL daAlink_c::checkHorseReinLeftOnly() const {
    return (field_0x2fab & 8) && !(field_0x2fab & 0x10);
}

int daAlink_c::getReinHandType() const {
    if (mProcID == PROC_TOOL_DEMO) {
        if ((field_0x2fab & 8) && (field_0x2fab & 0x10)) {
            return 3;
        } else if (field_0x2fab & 8) {
            return 1;
        } else if (field_0x2fab & 0x10) {
            return 2;
        } else if (checkReinRide()) {
            return 0;
        }
    } else if (checkReinRide()) {
        if (checkHorseNotGrab()) {
            return 0;
        } else if (checkHorseReinLeftOnly()) {
            return 1;
        } else {
            return 2;
        }
    }

    return -1;
}

BOOL daAlink_c::checkHorseLieAnime() const {
    return checkUnderMove0BckNoArc(ANM_HORSE_CROUCH) || checkUpperHorseLieAnime();
}

BOOL daAlink_c::checkHorseSubjectivity() const {
    return dComIfGp_checkPlayerStatus0(0, 0x2000) || mProcID == PROC_HORSE_BOW_SUBJECT ||
           mProcID == PROC_HORSE_HOOKSHOT_SUBJECT || mProcID == PROC_HORSE_BOOMERANG_SUBJECT;
}

void daAlink_c::setHorseSwordUpAnime() {
    setUpperAnimeBaseSpeed(dRes_ID_ALANM_BCK_DASHHBSUP_e, mpHIO->mHorse.m.mSwordUpAnmSpeed,
                           mpHIO->mHorse.m.mSwordUpInterpolation);
    setFacePriBck(dRes_ID_ALANM_BCK_FAT_e);
}

void daAlink_c::setHorseTurnUpperAnime(BOOL i_isTurnL) {
    if (checkHorseNoUpperAnime() || (!i_isTurnL && checkHorseTurnRAnime()) ||
        (i_isTurnL == TRUE && checkHorseTurnLAnime()))
    {
        setUpperAnimeBaseMorf(!i_isTurnL ? (u16)dRes_ID_ALANM_BCK_TURNLS_e : (u16)dRes_ID_ALANM_BCK_TURNRS_e, 4.0f);
    }
}

BOOL daAlink_c::checkHorseNoUpperAnime() const {
    return checkHorseTiredAnime() || checkNoUpperAnime() || checkHorseSwordUpAnime();
}

void daAlink_c::getHorseReinHandPos(cXyz* o_handPosA, cXyz* o_handPosB) {
    int hand_type = getReinHandType();
    if (hand_type == 2) {
        mDoMtx_multVec(getRightHandMatrix(), &m_handRightInSidePos, o_handPosA);
        mDoMtx_multVec(getRightHandMatrix(), &m_handRightOutSidePos, o_handPosB);
    } else if (hand_type == 1) {
        mDoMtx_multVec(getLeftHandMatrix(), &m_handLeftOutSidePos, o_handPosA);
        mDoMtx_multVec(getLeftHandMatrix(), &m_handLeftInSidePos, o_handPosB);
    } else {
        mDoMtx_multVec(getLeftHandMatrix(), &m_handLeftOutSidePos, o_handPosA);
        mDoMtx_multVec(getRightHandMatrix(), &m_handRightOutSidePos, o_handPosB);
    }
}

BOOL daAlink_c::checkHorseNotGrab() const {
    return checkBowAnime() ||
           mProcID == PROC_HORSE_BOTTLE_DRINK ||
           mProcID == PROC_HORSE_KANDELAAR_POUR ||
           mProcID == PROC_HORSE_RUN ||
           mProcID == PROC_HORSE_HANG ||
           mProcID == PROC_DEAD ||
           mProcID == PROC_GET_ITEM ||
           (mProcID == PROC_HORSE_GET_KEY && mProcVar2.field_0x300c != 0) ||
           checkUpperGuardAnime() ||
           checkTwoHandItemEquipAnime() ||
           checkGrabAnimeAndThrow() ||
           checkBoomerangThrowAnime() ||
           checkBoomerangCatchAnime() ||
           checkKandelaarSwingAnime() ||
           checkHookshotAnime() ||
           !(field_0x2fab & 0x18) ||
           (mProcID == PROC_BOAR_RUN && mProcVar2.field_0x300c != 0);
}

void daAlink_c::setHorseStirrup() {
    daHorse_c* horse = dComIfGp_getHorseActor();
    if (horse == NULL || (!checkHorseRide() && (mProcID != PROC_TOOL_DEMO || field_0x2fab == 0))) {
        return;
    }

    if (field_0x2fab & 1) {
        mDoMtx_stack_c::copy(mpLinkModel->getAnmMtx(field_0x30bc));
        mDoMtx_stack_c::transM(-2.0f, -11.0f, 1.5f);       
        mDoMtx_stack_c::ZXYrotM(NULL, -0x8000, 0x4000);
        mDoMtx_copy(mDoMtx_stack_c::get(), horse->getLeftStirrupMtx());
    }

    if (field_0x2fab & 2) {
        mDoMtx_stack_c::copy(mpLinkModel->getAnmMtx(field_0x30be));
        mDoMtx_stack_c::transM(-2.0f, 11.0f, 1.5f);     
        mDoMtx_stack_c::ZrotM(-0x4000);
        mDoMtx_copy(mDoMtx_stack_c::get(), horse->getRightStirrupMtx());
    }

    if (field_0x2fab & 3) {
        horse->calcWeightEnvMtx();
    }

    int hand_type = getReinHandType();
    if (hand_type == 0) {
        horse->setReinPosNormal();
    } else if (hand_type != -1) {
        horse->setReinPosHand(hand_type);
    }
}

void daAlink_c::changeBoarRunRide() {
    ((e_wb_class*)mRideAcKeep.getActor())->setRunRideMode();
}

int daAlink_c::setSyncHorsePos() {
    daHorse_c* horse_p = dComIfGp_getHorseActor();

    if (horse_p == NULL) {
        if (mProcID == PROC_DEAD || mProcID == PROC_GET_ITEM) {
            return 1;
        }

        rideGetOff();
        checkWaitAction();
        return 0;
    }

    field_0x2f99 = 0x60;

    if (mProcID == PROC_HORSE_RUN || mProcID == PROC_HORSE_HANG ||
        checkUnderMove0BckNoArc(ANM_HORSE_CROUCH) || checkUnderMove0BckNoArc(ANM_HORSE_TURN_LEFT) ||
        checkUnderMove0BckNoArc(ANM_HORSE_TURN_RIGHT))
    {
        if (mProcID == PROC_HORSE_RUN) {
            static const Vec localHorseRun = {15.0f, 0.0f, -45.0f};
            mDoMtx_multVec(horse_p->getSaddleMtx(), &localHorseRun, &current.pos);

            if (!horse_p->checkTurn() && !horse_p->checkStop()) {
                f32 frame = horse_p->getAnmFrame(0) / horse_p->getAnmFrameMax(0) + 0.5f;
                if (frame > 1.0f) {
                    frame -= 1.0f;
                }

                f32 var_f0 = frame * (f32)mUnderFrameCtrl[0].getEnd();
                mUnderFrameCtrl[0].setFrame(var_f0);
                getNowAnmPackUnder(UNDER_0)->setFrame(var_f0);
            }
        } else {
            static const Vec localHorseRun = {30.0f, 0.0f, -10.0f};
            mDoMtx_multVec(horse_p->getSaddleMtx(), &localHorseRun, &current.pos);
        }

        csXyz sp58;
        mDoMtx_MtxToRot(horse_p->getSaddleMtx(), &sp58);
        shape_angle.x = sp58.x;
        shape_angle.y = sp58.y;
        shape_angle.z = sp58.z + -0x4000;
    } else {
        if (dComIfGp_checkCameraAttentionStatus(field_0x317c, 0x20)) {
            current.pos.set(horse_p->current.pos.x, horse_p->current.pos.y + 230.0f,
                            horse_p->current.pos.z);
        } else {
            mDoMtx_multVec(horse_p->getRootMtx(), &l_localHorseRidePos, &current.pos);
        }

        shape_angle = horse_p->shape_angle;
    }

    current.angle.y = shape_angle.y;
    mNormalSpeed = fabsf(horse_p->speedF);

    if (checkZeroSpeedF()) {
        onModeFlg(1);
    } else {
        offModeFlg(1);
    }

    return 1;
}

int daAlink_c::setSyncBoarPos() {
    e_wb_class* boar = (e_wb_class*)mRideAcKeep.getActor();
    if (boar == NULL) {
        if (mProcID == PROC_DEAD || mProcID == PROC_GET_ITEM) {
            return 1;
        }

        rideGetOff();
        checkWaitAction();
        return 0;
    }

    if (fabsf(mRideAcKeep.getActor()->speedF) < 0.0010000000474974513f) {
        onModeFlg(1);
    } else {
        offModeFlg(1);
    }

    if (mProcID != PROC_DEAD && mProcID != PROC_GET_ITEM) {
        if (boar == NULL || boar->checkDownDamage()) {
            boarForceGetOff();
            return 0;
        }

        if (!boar->checkNormalRideMode() && mProcID != PROC_BOAR_RUN) {
            procBoarRunInit();
            return 0;
        }
    }

    MtxP ride_mtx = boar->getRideMtx();
    field_0x2f99 = 0x60;

    mDoMtx_multVec(ride_mtx, &l_localBoarRidePos, &current.pos);

    csXyz sp18;
    mDoMtx_MtxToRot(ride_mtx, &sp18);
    shape_angle.x = -sp18.z;
    shape_angle.y = sp18.y + 0x4000;
    shape_angle.z = sp18.x;
    current.angle.y = shape_angle.y;

    mNormalSpeed = mRideAcKeep.getActor()->speedF;
    return 1;
}

int daAlink_c::setSyncBoarRunPos() {
    e_wb_class* boar = (e_wb_class*)mRideAcKeep.getActor();
    fopAc_ac_c* boar_actor = &boar->mEnemy;

    if (boar == NULL || boar->checkDownDamage()) {
        boarForceGetOff();
        return 0;
    }

    if (boar->checkNormalRideMode()) {
        procHorseWaitInit();
        return 0;
    }

    MtxP ride_mtx = boar->getRideMtx();
    field_0x2f99 = 0x60;

    static const Vec localOffset = {-30.0f, -18.0f, 0.0f};
    mDoMtx_multVec(ride_mtx, &localOffset, &current.pos);

    csXyz sp28;
    mDoMtx_MtxToRot(ride_mtx, &sp28);
    shape_angle.x = -sp28.z;
    shape_angle.y = sp28.y + 0x4000;
    shape_angle.z = sp28.x;
    current.angle.y = shape_angle.y;

    if (mProcID == PROC_BOAR_RUN && mProcVar3.field_0x300e != 0) {
        mUnderFrameCtrl[0].setFrame(boar->nowAnimeFrame());
        getNowAnmPackUnder(UNDER_0)->setFrame(boar->nowAnimeFrame());
    }

    return 1;
}

BOOL daAlink_c::setSyncRidePos() {
    if (checkHorseRide()) {
        return setSyncHorsePos();
    } else {
        return setSyncBoarPos();
    }
}

void daAlink_c::setHorseTurnAnime() {
    daHorse_c* horse = dComIfGp_getHorseActor();
    u16 anm_idx = horse->getAnmIdx(0);

    daAlink_ANM anm;
    if (anm_idx == 12) {
        anm = ANM_HORSE_JUMP_START;
    } else if (anm_idx == 11) {
        anm = ANM_HORSE_JUMP;
    } else if (anm_idx == 10) {
        anm = ANM_HORSE_JUMP_LAND;
    } else if (anm_idx == 21) {
        anm = ANM_HORSE_STOP;
    } else if (anm_idx == 20) {
        anm = ANM_HORSE_STAND;
    } else {
        anm = ANM_HORSE_STOP_TO_STAND;
    }

    setSingleAnimeBaseMorf(anm, horse->getMorfFrame());

    f32 anm_frame = horse->getAnmFrame(0);
    mUnderFrameCtrl[0].setFrame(anm_frame);
    getNowAnmPackUnder(UNDER_0)->setFrame(anm_frame);

    if (getNowAnmPackUnder(UNDER_0) != getNowAnmPackUpper(UPPER_0)) {
        mUpperFrameCtrl[0].setFrame(anm_frame);
        getNowAnmPackUpper(UPPER_0)->setFrame(anm_frame);
    }
}

void daAlink_c::getBaseHorseAnime(daAlink_c::daAlink_ANM* i_anmList) {
    daHorse_c* horse = dComIfGp_getHorseActor();

    if (checkHorseNoUpperAnime() || checkUpperHorseLieAnime()) {
        if (!dComIfGp_checkPlayerStatus0(0, 0x2000)) {
            if (checkShieldGet()) {
                setRStatus(BUTTON_STATUS_DEFEND);
            } else {
                setRStatus(BUTTON_STATUS_COVER);
            }

            if (spActionButton()) {
                resetUpperAnime(UPPER_2, -1.0f);
                i_anmList[0] = ANM_HORSE_CROUCH;
                i_anmList[1] = ANM_HORSE_CROUCH;
                return;
            }
        }
    }

    int i;
    u16 anm_idx;
    for (i = 0; i < 3; i++, i_anmList++) {
        anm_idx = horse->getAnmIdx(i);
        if (anm_idx == 7 || horse->speedF < 0.0f) {
            if (i == 1) {
                i_anmList[0] = ANM_HORSE_WALK_B;
            } else {
                i_anmList[0] = ANM_HORSE_TURN_B;
            }
        } else if (anm_idx == 0x12) {
            i_anmList[0] = ANM_HORSE_DASH_B;
        } else if (anm_idx == 0x22) {
            i_anmList[0] = ANM_HORSE_WALK_B;
        } else if (anm_idx == 0x11) {
            i_anmList[0] = ANM_HORSE_DASH_START;
        } else if (anm_idx == 0x13) {
            if (checkHorseZeldaBowMode() ) {
                i_anmList[0] = ANM_HORSE_DASH_B;
            } else {
                i_anmList[0] = ANM_HORSE_DASH_A;
            }
        } else if (anm_idx == 0x23) {
            i_anmList[0] = ANM_HORSE_WALK_A;
        } else if (anm_idx == 0x17 || anm_idx == 0x18) {
            if (i == 1) {
                i_anmList[0] = ANM_HORSE_WALK_B;
            } else {
                i_anmList[0] = ANM_HORSE_TURN_LEFT;
            }
        } else if (anm_idx == 0x19 || anm_idx == 0x1A) {
            if (i == 1) {
                i_anmList[0] = ANM_HORSE_WALK_B;
            } else {
                i_anmList[0] = ANM_HORSE_TURN_RIGHT;
            }
        } else if (anm_idx == 0xFFFF && i == 1) {
            i_anmList[0] = i_anmList[-1];
        } else {
            i_anmList[0] = ANM_HORSE_WAIT;
        }
    }
}

int daAlink_c::checkHorseSpecialProc() {
    daHorse_c* horse_p = dComIfGp_getHorseActor();

    if (!checkHorseRide()) {
        return 0;
    }

    if (checkEndResetFlg2(ERFLG2_FORCE_HORSE_GETOFF)) {
        current.pos = horse_p->current.pos;
        return procWaitInit();
    }

    if ((horse_p->checkStop() || horse_p->checkTurn()) && !horse_p->checkCutTurnCancel() &&
        !horse_p->checkTurnCancelKeep())
    {
        return procHorseTurnInit();
    }

    if (horse_p->checkJump()) {
        return procHorseJumpInit();
    }

    if (mProcID != PROC_HORSE_WAIT && mProcID != PROC_HORSE_CUT &&
        mProcID != PROC_HORSE_CUT_CHARGE_READY)
    {
        horse_p->onMoveAccept();
    }

    return 0;
}

BOOL daAlink_c::checkHorseServiceWaitAnime() {
    return checkUnderMove0BckNoArc(ANM_HORSE_WAIT_A) || checkUnderMove0BckNoArc(ANM_HORSE_WAIT_B);
}

int daAlink_c::setSyncHorse(int param_0) {
    daHorse_c* horse_p = dComIfGp_getHorseActor();
    daPy_frameCtrl_c* framectrl1 = &mUnderFrameCtrl[0];
    daPy_frameCtrl_c* framectrl2 = &mUnderFrameCtrl[2];

    BOOL sp1C;
    if (checkModeFlg(1)) {
        sp1C = TRUE;
    } else {
        sp1C = FALSE;
    }

    setSyncHorsePos();
    if (checkHorseSpecialProc()) {
        return 0;
    }

    daAlink_ANM anmList[3];
    getBaseHorseAnime(anmList);

    if (fabsf(horse_p->speedF) < 0.001f) {
        if (!sp1C) {
            field_0x3004 = 0;
        } else if (checkInputOnR() && horse_p->checkWait() && mProcID != PROC_HORSE_SUBJECTIVITY &&
                   mProcID != PROC_HORSE_CUT_TURN && anmList[0] != ANM_HORSE_TURN_LEFT &&
                   anmList[0] != ANM_HORSE_TURN_RIGHT && field_0x3004 == 0) {
            int dir = getDirectionFromShapeAngle();
            if (dir == DIR_FORWARD) {
                field_0x3004 = mpHIO->mHorse.m.mWalkOutProhibitionTime;
                setSingleAnimeParam(ANM_WSTARTH, &mpHIO->mHorse.m.mWalkOutAnm);
                offNoResetFlg1(FLG1_UNK_1000);
            }
        }
    }

    if ((checkHorseUnderDashStartAnime() || checkHorseUnderLashAnime()) &&
        checkAnmEnd(&mUnderFrameCtrl[2]))
    {
        resetUnderAnime(UNDER_2, 4.0f);
    }

    if (checkHorseWalkStartAnm() && spActionButton()) {
        if (checkNoUpperAnime()) {
            setUpperAnimeBase(getMainBckData(ANM_HORSE_CROUCH)->m_upperID);
        }
    } else if (checkUpperHorseLieAnime()) {
        resetUpperAnime(UPPER_2, 3.0f);
    }

    if (param_0 && field_0x30a6 == 0 && horse_p->speedF >= 0.0f &&
        horse_p->getAimNeckAngleY() == 0 && anmList[0] != ANM_HORSE_TURN_LEFT &&
        anmList[0] != ANM_HORSE_TURN_RIGHT &&
        ((field_0x2f8c == 10 && (checkHorseNoUpperAnime() || checkEquipAnime() || checkUpperGuardAnime())) ||
         (mProcID == PROC_HORSE_CUT || mProcID == PROC_HORSE_CUT_TURN || mProcID == PROC_HORSE_CUT_CHARGE_READY)))
    {
        if (checkCowGame()) {
            if (checkHorseNoUpperAnime()) {
                setDoStatus(BUTTON_STATUS_WHOOP);

                if (doTrigger()) {
                    onResetFlg0(RFLG0_COW_GAME_LEASH);
                    voiceStart(Z2SE_AL_V_RUSH_HORSE);
                    field_0x30a6 = mpHIO->mHorse.m.mWhipWaitTime;
                    setHorseSwordUpAnime();
                    field_0x3002 = -mpHIO->mHorse.m.mWhipWaitTime;
                }
            }
        } else if (checkHorseDashAccept()) {
            setDoStatus(BUTTON_STATUS_DASH);

            if (doTrigger()) {
                field_0x3004 = mpHIO->mHorse.m.mWalkOutProhibitionTime;
                daAlink_ANM anm = ANM_HORSE_LASH;
                const daAlinkHIO_anm_c* anmparam_p = &mpHIO->mHorse.m.mWhipAnm;

                if (mProcID != PROC_HORSE_CUT && mProcID != PROC_HORSE_CUT_TURN &&
                    mProcID != PROC_HORSE_CUT_CHARGE_READY)
                {
                    setSingleAnimeParam(anm, anmparam_p);
                }

                setUnderAnimeParam(getMainBckData(anm)->m_underID, UNDER_2, anmparam_p);
                onNoResetFlg1(FLG1_UNK_1000);
                horse_p->offPlayerBackRideLash();
                voiceStart(Z2SE_AL_V_RUSH_HORSE);

                if (horse_p->speedF <= horse_p->getNormalMaxSpeedF()) {
                    onNoResetFlg1(FLG1_UNK_800);
                }

                field_0x30a6 = mpHIO->mHorse.m.mWhipWaitTime;
            }
        }
    }

    if ((checkHorseWalkStartAnm() &&
         framectrl1->getFrame() > mpHIO->mHorse.m.mHorseWalkStartFrame) ||
        (checkHorseWaitLashAnime() && framectrl2->getFrame() > 13.0f))
    {
        horse_p->onMoveAccept();
    }

    if (checkNoResetFlg1(FLG1_UNK_1000) &&
        ((checkHorseUnderDashStartAnime() && framectrl2->getFrame() > 13.0f) ||
         (checkHorseUnderLashAnime() && framectrl2->getFrame() > 13.0f)))
    {
        offNoResetFlg1(FLG1_UNK_1000);
        horse_p->onPlayerLash();
    }

    if (checkHorseSwordUpAnime()) {
        if (checkHorseZeldaBowMode()) {
            resetUpperAnime(UPPER_2, mpHIO->mHorse.m.mSwordUpInterpolation);
            field_0x3002 = 0;
        } else if (field_0x3002 > 0) {
            field_0x3002--;

            if (field_0x3002 == 0 || horse_p->speedF < horse_p->getNormalMaxSpeedF()) {
                resetUpperAnime(UPPER_2, mpHIO->mHorse.m.mSwordUpInterpolation);
                field_0x3002 = 0;
            }
        } else {
            field_0x30ec = mpHIO->mHorse.m.mSwordUpInterpolation;
            field_0x3002++;

            if (field_0x3002 == 0) {
                resetUpperAnime(UPPER_2, mpHIO->mHorse.m.mSwordUpInterpolation);
                field_0x30ec = mpHIO->mHorse.m.mSwordUpInterpolation;
            }
        }
    }

    BOOL var_r28 = checkAnmEnd(mUnderFrameCtrl) ||
        (checkHorseServiceWaitAnime() && (!checkNoUpperAnime() || horse_p->getAimNeckAngleY() != 0 || !checkModeFlg(1)));

    if (((var_r28 || anmList[0] == ANM_HORSE_TURN_LEFT || anmList[0] == ANM_HORSE_TURN_RIGHT ||
          anmList[0] == ANM_HORSE_DASH_START ||
          (spActionButton() && !checkHorseWalkStartAnm() && !checkHorseWaitLashAnime()))
          && mProcID != PROC_HORSE_CUT_TURN
        )
        || field_0x2f8c == 10
        )
    {
        if ((var_r28 || field_0x3004 != mpHIO->mHorse.m.mWalkOutProhibitionTime) && field_0x3004 != 0) {
            field_0x3004--;
        }

        f32 morf_frame;
        if (var_r28 && horse_p->getMorfFrame() < 4.0f) {
            morf_frame = 4.0f;
        } else {
            morf_frame = horse_p->getMorfFrame();
        }

        f32 morf;
        if (anmList[0] == ANM_HORSE_TURN_LEFT || anmList[0] == ANM_HORSE_TURN_B || anmList[0] == ANM_HORSE_TURN_RIGHT) {
            morf = 0.0f;
        } else {
            morf = horse_p->getBlendRate();
        }

        if (anmList[0] == ANM_HORSE_DASH_START && !checkUnderMove0BckNoArc(ANM_HORSE_DASH_START)) {
            voiceStart(Z2SE_AL_V_RUSH_HORSE);
        }

        setDoubleAnime(morf, 1.0f, 1.0f, anmList[0], anmList[1], 10, morf_frame);
        setBaseHorseAnimeFrame();

        if (var_r28) {
            setHorseSwordUp(0);
        }
    }

    return 1;
}

int daAlink_c::setSyncBoar(int param_0) {
    e_wb_class* boar = (e_wb_class*)mRideAcKeep.getActor();
    fopAc_ac_c* boar_actor = (fopAc_ac_c*)boar;

    BOOL var_r27;
    if (checkModeFlg(1)) {
        var_r27 = TRUE;
    } else {
        var_r27 = FALSE;
    }

    if (!setSyncBoarPos()) {
        return 0;
    }

    if (fabsf(boar_actor->speedF) < 0.001f && var_r27 && checkInputOnR() && boar->checkWait() &&
        mProcID != PROC_HORSE_SUBJECTIVITY && field_0x3004 == 0)
    {
        int direction = getDirectionFromShapeAngle();
        if (direction == DIR_FORWARD || direction == DIR_BACKWARD) {
            field_0x3004 = mpHIO->mHorse.m.mWalkOutProhibitionTime;
            setSingleAnimeParam(ANM_WSTARTH, &mpHIO->mHorse.m.mWalkOutAnm);
        }
    }

    if (param_0 && boar->getWaitRollAngle() == 0 && checkHorseNoUpperAnime()) {
        setDoStatus(BUTTON_STATUS_DASH);
        if (doTrigger()) {
            changeBoarRunRide();
        }
    }

    if ((checkAnmEnd(mUnderFrameCtrl) && mProcID != PROC_HORSE_CUT_TURN) ||
        (checkHorseServiceWaitAnime() && (!checkNoUpperAnime() || !checkModeFlg(1))))
    {
        setBaseBoarAnime();
    } else if (mUnderAnmHeap[0].getIdx() == dRes_ID_ALANM_BCK_LASHS_e) {
        if (mUnderFrameCtrl[0].checkPass(10.0f)) {
            changeBoarRunRide();
        }
    } else if (!checkHorseServiceWaitAnime() && mProcID != PROC_HORSE_CUT_TURN) {
        setBaseBoarAnime();
    }

    if (mUnderAnmHeap[0].getIdx() != dRes_ID_ALANM_BCK_LASHS_e) {
        if (boar->getWaitRollAngle() > 0) {
            setHorseTurnUpperAnime(FALSE);
        } else if (boar->getWaitRollAngle() < 0) {
            setHorseTurnUpperAnime(TRUE);
        }
    }

    if (checkHorseTurnAnime()) {
        if (boar->getWaitRollAngle() == 0) {
            resetUpperAnime(UPPER_2, 4.0f);
            field_0x33f0 = 0.0f;
        } else {
            field_0x33f0 = cLib_minMaxLimit(boar->getWaitRollAngle() * 0.002f, -1.0f, 1.0f);
            mNowAnmPackUpper[2].setRatio(std::abs(field_0x33f0));
        }
    } else {
        field_0x33f0 = 0.0f;
    }

    return 1;
}

int daAlink_c::setSyncRide(int param_0) {
    if (param_0 && mMidnaTalkDelayTimer == 0 && dComIfGp_getDoStatus() == BUTTON_STATUS_NONE &&
        (!checkHorseRide() || dComIfGp_getHorseActor()->getAnmIdx(0) != 7))
    {
        param_0 = TRUE;
    } else {
        param_0 = FALSE;
    }

    if (checkServiceWaitMode() && checkHorseRide() && mProcID == PROC_HORSE_WAIT &&
        !checkBowAndSlingItem(mEquipItem) && checkZeroSpeedF() &&
        !checkUnderMove0BckNoArc(ANM_HORSE_TURN_LEFT) &&
        !checkUnderMove0BckNoArc(ANM_HORSE_TURN_RIGHT) && !checkUnderMove0BckNoArc(ANM_WSTARTH) &&
        checkModeFlg(1))
    {
        if (field_0x30ca != 0) {
            field_0x30ca--;
        } else {
            if (cM_rnd() < 0.5f) {
                setSingleAnimeBaseMorf(ANM_HORSE_WAIT_A, 4.0f);
            } else {
                setSingleAnimeBaseMorf(ANM_HORSE_WAIT_B, 4.0f);
            }
            initServiceWaitTime();
        }
    } else {
        initServiceWaitTime();
    }

    if (checkHorseRide()) {
        return setSyncHorse(param_0);
    } else {
        return setSyncBoar(param_0);
    }
}

void daAlink_c::setBaseHorseAnimeFrame() {
    daHorse_c* horse = dComIfGp_getHorseActor();
    BOOL var_r27 = FALSE;

    f32 anm_frame;
    if (mUnderAnmHeap[0].checkNoSetArcNo()) {
        u16 temp_r28 = mUnderAnmHeap[0].getIdx();
        if (temp_r28 == getMainBckData(ANM_HORSE_WAIT)->m_underID) {
            anm_frame = mUnderFrameCtrl[0].getFrame();
        } else if ((temp_r28 == getMainBckData(ANM_HORSE_TURN_B)->m_underID && horse->getAnmIdx(1) == 6) ||
                   ((temp_r28 == getMainBckData(ANM_HORSE_TURN_LEFT)->m_underID || temp_r28 == getMainBckData(ANM_HORSE_TURN_RIGHT)->m_underID) &&
                    (horse->getAnmIdx(0) == 0x17 || horse->getAnmIdx(0) == 0x19)))
        {
            anm_frame = mUnderFrameCtrl[0].getEnd();
            mUnderFrameCtrl[0].onEndFlg();
            var_r27 = TRUE;
        } else {
            field_0x3460 = 0.0f;
            anm_frame = horse->getAnmFrame(0);
        }
    } else {
        anm_frame = horse->getAnmFrame(0);
    }

    getNowAnmPackUnder(UNDER_0)->setFrame(anm_frame);
    mUnderFrameCtrl[0].setFrame(anm_frame);

    if (var_r27) {
        getNowAnmPackUnder(UNDER_1)->setFrame(field_0x3460);
        simpleAnmPlay(getNowAnmPackUnder(UNDER_1));
        field_0x3460 = getNowAnmPackUnder(UNDER_1)->getFrame();
    } else if (horse->getAnmIdx(1) == 0xFFFF) {
        getNowAnmPackUnder(UNDER_1)->setFrame(anm_frame);
    } else {
        getNowAnmPackUnder(UNDER_1)->setFrame(horse->getAnmFrame(1));
    }

    getNowAnmPackUpper(UPPER_0)->setFrame(anm_frame);
    getNowAnmPackUpper(UPPER_1)->setFrame(getNowAnmPackUnder(UNDER_1)->getFrame());
}

void daAlink_c::setBaseBoarAnime() {
    e_wb_class* boar_actor = (e_wb_class*)mRideAcKeep.getActor();

    f32 var_f31;
    if (checkEventRun() && fopAcM_getTalkEventPartner(this) == (fopAc_ac_c*)getMidnaActor()) {
        var_f31 = 0.0f;
    } else {
        var_f31 = boar_actor->rideSpeedRate();
    }

    if (var_f31 < 0.2f) {
        setDoubleAnime(var_f31, 1.0f, 1.0f, ANM_HORSE_WAIT, ANM_HORSE_WALK_A, 0xA, -1.0f);
    } else if (var_f31 < 0.5f) {
        setDoubleAnime(3.3333333f * (var_f31 - 0.2f), 1.0f, 1.0f, ANM_HORSE_WALK_A, ANM_HORSE_WALK_B, 0xA, -1.0f);
    } else {
        setDoubleAnime(2.0f * (var_f31 - 0.5f), 1.0f, 1.0f, ANM_HORSE_WALK_B, ANM_HORSE_DASH_A, 0xA, -1.0f);
    }
}

void daAlink_c::setBaseRideAnime() {
    if (checkHorseRide()) {
        daHorse_c* horse_p = dComIfGp_getHorseActor();
        daAlink_ANM anmList[3];
        getBaseHorseAnime(anmList);
        setDoubleAnime(horse_p->getBlendRate(), 1.0f, 1.0f, anmList[0], anmList[1], 10,
                       horse_p->getMorfFrame());
        setBaseHorseAnimeFrame();
    } else {
        setBaseBoarAnime();
    }
}

bool daAlink_c::checkHorseSwordUpSpped() {
    daHorse_c* horse_p = dComIfGp_getHorseActor();
    return horse_p->speedF > horse_p->getLashMaxSpeedF() * 0.89999998f;
}

void daAlink_c::setHorseSwordUp(BOOL param_0) {
    if (checkHorseRide() && checkNoUpperAnime() && mEquipItem == 0x103 &&
        !checkHorseZeldaBowMode() &&
        ((param_0 && checkHorseSwordUpSpped()) ||
         (!param_0 && checkEnemyGroup(mTargetedActor) && checkNoResetFlg1(FLG1_UNK_800))))
    {
        setHorseSwordUpAnime();
        field_0x3002 = mpHIO->mHorse.m.mSwordUpTime;
    }

    offNoResetFlg1(FLG1_UNK_800);
}

int daAlink_c::setRideSubjectAngle(s16 param_0) {
    shape_angle.y += mBodyAngle.y;
    BOOL temp_r27 = setBodyAngleToCamera();

    s16 var_r30;
    if (mTargetedActor != NULL) {
        var_r30 = 10000;
    } else {
        var_r30 = mpHIO->mHorse.m.mSubjectiveDownwardMaxAngle;
    }

    mBodyAngle.x = cLib_minMaxLimit<s16>((s16)mBodyAngle.x, mpHIO->mHorse.m.mSubjectiveUpwardMaxAngle, var_r30);

    s16 temp_r29 = shape_angle.y - param_0;
    if (checkHorseZelda()) {
        field_0x3190 = cLib_minMaxLimit<s16>((s16)temp_r29, -0x4000, 0x4000);
    } else {
        field_0x3190 = temp_r29;
    }

    mBodyAngle.y = field_0x3190;
    shape_angle.y = param_0;
    current.angle.y = param_0;

    if (!checkNoResetFlg2(FLG2_UNK_100)) {
        field_0x310a = mBodyAngle.x;
        field_0x310c = param_0 + field_0x3190;
    }

    return temp_r27;
}

void daAlink_c::setBodyAngleRideReadyAnime() {
    setBodyAngleXReadyAnime(0);

    s16 angle;
    if (mTargetedActor != NULL) {
        angle = cLib_minMaxLimit<s16>(
            cLib_targetAngleY(&current.pos, &mTargetedActor->eyePos) - shape_angle.y,
            -0x3800, 0x3800);
    } else {
        angle = 0;
    }

    daPy_addCalcShort(&mBodyAngle.y, angle, 4, 0xC00, 0x180);
}

BOOL daAlink_c::checkHorseGetOffWallCheck(cXyz* i_lineStartPos, cXyz* i_lineEndPos, s16 param_2) {
    if (!commonLineCheck(i_lineStartPos, i_lineEndPos)) {
        return FALSE;
    }

    if (dBgS_CheckBWallPoly(mLinkLinChk)) {
        return TRUE;
    }

    if (getGroundAngle(&mLinkLinChk, param_2) < -cM_deg2s(mpHIO->mWolf.mWlSlide.m.mSlidingAngle)) {
        return TRUE;
    }

    return FALSE;
}

int daAlink_c::checkHorseGetOffDirection() {
    fopAc_ac_c* ride_actor_p = mRideAcKeep.getActor();

    cXyz lin_start_pos(ride_actor_p->current.pos.x, current.pos.y, ride_actor_p->current.pos.z);
    cXyz lin_end_pos;

    s16 var_r28;
    if (field_0x2fc0 == 0) {
        var_r28 = shape_angle.y + 0x4000;
    } else {
        var_r28 = shape_angle.y - 0x4000;
    }

    lin_end_pos.set(ride_actor_p->current.pos.x + cM_ssin(var_r28) * 125.0f, lin_start_pos.y,
                    ride_actor_p->current.pos.z + cM_scos(var_r28) * 125.0f);

    f32 check_y[3] = {0.0f, 0.0f, 0.0f};
    check_y[0] = current.pos.y;
    check_y[1] = ride_actor_p->current.pos.y + (current.pos.y - ride_actor_p->current.pos.y) * 0.5f;

    f32 var_f31 = ride_actor_p->current.pos.y + l_autoUpHeight;
    check_y[2] = var_f31;

    int check_no;
    int i;
    for (i = 0; i < 2; i++) {
        for (check_no = 0; check_no < 3; check_no++) {
            lin_start_pos.y = check_y[check_no];
            lin_end_pos.y = lin_start_pos.y;

            if (checkHorseGetOffWallCheck(&lin_start_pos, &lin_end_pos, var_r28)) {
                break;
            }
        }

        if (check_no == 3) {
            if (i == 0) {
                return field_0x2fc0;
            }
            return field_0x2fc0 ^ 1;
        }

        lin_end_pos.x = lin_start_pos.x * 2.0f - lin_end_pos.x;
        lin_end_pos.z = lin_start_pos.z * 2.0f - lin_end_pos.z;
        var_r28 += 0x8000;
    }

    return 2;
}

void daAlink_c::boarForceGetOff() {
    procCoLargeDamageInit(-4, TRUE, 0, 0, NULL, 0);
}

void daAlink_c::horseGetOffEnd() {
    if (field_0x2fc0 == 0) {
        shape_angle.y -= (s16)0x4000;
        setOldRootQuaternion(0, 0x4000, 0);
        mDoMtx_stack_c::YrotS(-0x4000);
    } else {
        shape_angle.y += (s16)0x4000;
        setOldRootQuaternion(0, -0x4000, 0);
        mDoMtx_stack_c::YrotS(0x4000);
    }

    J3DTransformInfo* transinfo_p = field_0x2060->getOldFrameTransInfo(0);
    transinfo_p->mTranslate.x = l_waitBaseAnime.x;
    transinfo_p->mTranslate.z = l_waitBaseAnime.z;
    current.angle.y = shape_angle.y;
    field_0x2fe4 = shape_angle.y;
}

int daAlink_c::checkNextActionHorse() {
    #if PLATFORM_GCN
    if ((!checkGrabAnimeAndThrow() || mProcID == PROC_HORSE_GRAB_MOVE) && checkNextActionFromButton())
    #else
    if (checkGrabAnimeAndThrow())
    #endif
    {
        return 1;
    }

    if (checkBowAnime()) {
        return checkNextActionBow();
    }

    if (checkBoomerangAnimeAndReturnWait()) {
        return checkNextActionBoomerang();
    }

    if (checkHookshotAnime()) {
        return checkNextActionHookshot();
    }

    if (checkGrabAnimeAndThrow()) {
        return procHorseGrabMoveInit();
    }

    if (mProcID == PROC_HORSE_COMEBACK) {
        return procHorseComebackInit();
    }

    if (mProcID == PROC_HORSE_JUMP && dComIfGp_getHorseActor() != NULL &&
        dComIfGp_getHorseActor()->checkJump())
    {
        return 1;
    }

    if (mProcID == PROC_HORSE_TURN || mProcID == PROC_HORSE_CUT_TURN) {
        return 0;
    }

    return procHorseWaitInit();
}

BOOL daAlink_c::checkHorseGetOff() {
    return (checkHorseRide() && dComIfGp_getHorseActor() != NULL && ((daHorse_c*)dComIfGp_getHorseActor())->checkGetOff()) ||
           (checkBoarRide() && ((e_wb_class*)mRideAcKeep.getActor()) != NULL && ((e_wb_class*)mRideAcKeep.getActor())->checkGetOff());
}

int daAlink_c::checkHorseGetOffAndSetDoStatus() {
    daHorse_c* horse = dComIfGp_getHorseActor();
    int off_direction = DIR_FORWARD;
    BOOL isGetOff = checkHorseGetOff();

    if (!(fabsf(mRideAcKeep.getActor()->speedF) < 30.0f) || !setTalkStatus()) {
        if (checkHorseLieAnime()) {
            if (!checkSpecialHorseRide()) {
                setDoStatus(BUTTON_STATUS_DISMOUNT);
            } else {
                setDoStatus(BUTTON_STATUS_UNK_140);
            }
        } else if ((checkHorseNoUpperAnime() || checkEquipAnime()) && mMidnaTalkDelayTimer == 0 && !checkInputOnR() && isGetOff) {
            if (mEquipItem == fpcNm_ITEM_NONE) {
                off_direction = checkHorseGetOffDirection();
                if ((off_direction != DIR_LEFT || (checkHorseRide() && horse->checkEnemySearch())) && field_0x2fb4 == 0) {
                    if (!checkSpecialHorseRide()) {
                        setDoStatus(BUTTON_STATUS_DISMOUNT);
                    } else {
                        setDoStatus(BUTTON_STATUS_UNK_140);
                    }
                }
            } else if (checkHorseRide() && !checkSpecialHorseRide() && field_0x2fb4 == 0 && horse->checkEnemySearch()) {
                setDoStatus(BUTTON_STATUS_DISMOUNT);
            } else {
                setDoStatus(BUTTON_STATUS_PUT_AWAY);
            }
        }
    }

    return off_direction;
}

int daAlink_c::setHorseGetOff(int param_0) {
    daHorse_c* horse_p = dComIfGp_getHorseActor();

    if (dComIfGp_getDoStatus() == BUTTON_STATUS_DISMOUNT && doTrigger()) {
        if ((checkHorseRide() && horse_p->checkEnemySearch()) || checkHorseLieAnime()) {
            resetUpperAnime(UPPER_2, -1.0f);
            procBackJumpInit(0);
            current.pos.y += mpHIO->mBackJump.m.mBackflipSpeedV;
        } else {
            return procHorseGetOffInit(param_0);
        }
    }

    return 0;
}

int daAlink_c::procHorseRideInit() {
    BOOL var_r25 = FALSE;

    if (mEquipItem != fpcNm_ITEM_NONE) {
        if (mEquipItem == 0x103) {
            var_r25 = 1;
        }
        deleteEquipItem(TRUE, TRUE);
    }

    mRideAcKeep.setData(field_0x27f4);

    daAlink_ANM anm;
    MtxP rideMtx;
    Vec* localRidePos_p;
    u32 mode;
    if (fopAcM_GetName(field_0x27f4) == PROC_HORSE) {
        daHorse_c* horse = (daHorse_c*)field_0x27f4;
        horse->onRideStartFlg();
        rideMtx = horse->getRootMtx();
        mRideStatus = RIDETYPE_HORSE;
        localRidePos_p = &l_localHorseRidePos;
        field_0x384c = &l_horseBaseAnime;
        mode = horse->checkRodeoMode();
        mZ2Link.setRiding(true);
    } else {
        e_wb_class* boar = (e_wb_class*)field_0x27f4;
        boar->setPlayerRideNow();
        rideMtx = boar->getRideMtx();
        mRideStatus = RIDETYPE_BOAR;
        localRidePos_p = &l_localBoarRidePos;
        field_0x384c = &l_boarBaseAnime;
        mode = 0;
    }

    commonProcInit(PROC_HORSE_RIDE);

    field_0x3588 = l_waitBaseAnime;
    f32 anm_speed = 1.0f;
    f32 var_f31;
    s16 anm_endF = -1;
    mProcVar2.field_0x300c = 0;
    setHeavyBoots(0);

    int ride_dir = getReinRideDirection();
    if (mode != 0 || ride_dir == DIR_BACKWARD) {
        anm = ANM_HORSE_MOUNT_BACK;
        field_0x2fc0 = 2;
        mProcVar2.field_0x300c = 1;
    } else {
        if (checkHorseRide() && ((daHorse_c*)field_0x27f4)->checkEnemySearch()) {
            anm_speed *= 1.2f;
            mProcVar2.field_0x300c = 1;
            anm_endF = 0x30;
        }
    
        if (ride_dir == DIR_LEFT) {
            if (checkHorseRide()) {
                anm = ANM_HORSE_MOUNT_LEFT;
            } else {
                anm = ANM_HORSE_RIDE_LEFT;
            }
            field_0x2fc0 = 0;
        } else {
            if (checkHorseRide()) {
                anm = ANM_HORSE_MOUNT_RIGHT;
            } else {
                anm = ANM_HORSE_RIDE_RIGHT;
            }
            field_0x2fc0 = 1;
        }
    
        if (mDemo.getDemoMode() != daPy_demo_c::DEMO_UNK_7_e) {
            setOldRootQuaternion(0, (s16)(shape_angle.y - field_0x27f4->shape_angle.y), 0);
        }
    }

    mNormalSpeed = 0.0f;

    cXyz sp2C;
    cXyz sp20;

    mDoMtx_multVec(rideMtx, localRidePos_p, &sp2C);
    mDoMtx_stack_c::transS(field_0x27f4->current.pos);
    mDoMtx_stack_c::YrotM(field_0x27f4->shape_angle.y);
    mDoMtx_stack_c::XrotM(-field_0x27f4->shape_angle.x);
    mDoMtx_stack_c::YrotM(-field_0x27f4->shape_angle.y);
    mDoMtx_stack_c::transM(-field_0x27f4->current.pos.x, -field_0x27f4->current.pos.y, -field_0x27f4->current.pos.z);
    mDoMtx_stack_c::concat(rideMtx);
    mDoMtx_stack_c::multVec(localRidePos_p, &sp20);

    var_f31 = current.pos.y;
    current.pos = field_0x27f4->current.pos + (sp2C - sp20);

    mDoMtx_stack_c::ZXYrotS(field_0x27f4->shape_angle.x, field_0x27f4->shape_angle.y, 0);

    if (checkBoarRide()) {
        static Vec const boarRideOffset = {-0.87f, 3.3299999f, -23.77f};
        mDoMtx_stack_c::multVec(&boarRideOffset, &sp2C);
        current.pos += sp2C;
        var_f31 += sp2C.y;
        mProcVar3.field_0x300e = 0;
    } else {
        mProcVar3.field_0x300e = var_r25;
    }

    field_0x3478 = var_f31 - current.pos.y;
    current.pos.y = var_f31;
    shape_angle.y = field_0x27f4->shape_angle.y;
    current.angle.y = shape_angle.y;
    speed.y = 0.0f;

    setSingleAnime(anm, anm_speed, 0.0f, anm_endF, 4.0f);

    field_0x2f99 = 0x10;
    field_0x2fab = 0;
    field_0x3004 = 0;
    field_0x3002 = 0;
    field_0x30a6 = 0;
    attention_info.field_0xa = 0x46;
    field_0x37c8 = field_0x27f4->current.pos;
    return 1;
}

int daAlink_c::procHorseRide() {
    daPy_frameCtrl_c* framectrl = &mUnderFrameCtrl[0];

    fopAc_ac_c* rideActor = mRideAcKeep.getActor();
    if (rideActor == NULL) {
        return checkNextAction(0);
    }

    e_wb_class* boar = (e_wb_class*)rideActor;
    daHorse_c* horse = (daHorse_c*)rideActor;
    
    field_0x2f99 = 7;
    current.pos += rideActor->current.pos - field_0x37c8;
    field_0x37c8 = rideActor->current.pos;

    if (field_0x2fc0 == 2) {
        if (framectrl->getFrame() >= 25.0f) {
            mRightHandIndex = 5;
            field_0x2fab = 0x13;
        } else if (framectrl->getFrame() >= 14.0f) {
            mLeftHandIndex = 4;
        }

        if (horse->checkRodeoMode()) {
            if (framectrl->getFrame() > 15.0f) {
                return procHorseRunInit();
            } else if (framectrl->getFrame() >= 4.0f) {
                setSyncHorsePos();
            }
        } else if (framectrl->getFrame() >= 4.0f && framectrl->getFrame() < 8.0f) {
            current.pos.y -= field_0x3478 * (0.25f * framectrl->getRate());
        } else if (framectrl->getFrame() > 8.0f) {
            setSyncHorsePos();
            horse->onRideRunFlg();
            horse->onMoveAccept();
        }
    } else {
        if (framectrl->getFrame() >= 46.0f) {
            mLeftHandIndex = 4;

            if (!checkBoarRide() || field_0x2fc0 == 0 || framectrl->getFrame() >= 53.0f) {
                mRightHandIndex = 5;
                field_0x2fab = 0x13;
            } else {
                field_0x2fab = 3;
            }

            if (checkBoarRide()) {
                csXyz rot;
                mDoMtx_MtxToRot(boar->getRideMtx(), &rot);
                cLib_addCalcAngleS(&shape_angle.x, -rot.z, 4, 2000, 400);
            } else {
                cLib_addCalcAngleS(&shape_angle.x, rideActor->shape_angle.x, 4, 2000, 400);
            }
        } else {
            if (framectrl->getFrame() >= 17.0f) {
                field_0x2fab |= (u8)(1 << field_0x2fc0);

                if (field_0x2fc0 == 0) {
                    if (framectrl->getFrame() >= 41.0f && checkHorseRide()) {
                        mLeftHandIndex = 4;
                    }
                } else if (framectrl->getFrame() >= 38.0f && checkHorseRide()) {
                    mRightHandIndex = 5;
                    field_0x2fab |= (u8)0x10;
                }
            }
        }

        if (framectrl->getFrame() >= 11.0f && framectrl->getFrame() < 16.0f) {
            current.pos.y -= field_0x3478 * (0.2f * framectrl->getRate());
        }

        if (framectrl->getFrame() >= 17.0f && checkHorseRide() && mProcVar2.field_0x300c != 0) {
            setSyncHorsePos();
            horse->onRideRunFlg();
            horse->onMoveAccept();
        }
    }

    if (checkAnmEnd(framectrl)) {
        J3DTransformInfo* temp_r28 = field_0x2060->getOldFrameTransInfo(0);

        if (checkHorseRide()) {
            horse->onRideFlg();
    
            if (mProcVar2.field_0x300c == 0) {
                temp_r28->mTranslate.x += l_horseBaseAnime.x;
                temp_r28->mTranslate.y += l_horseBaseAnime.y;
                temp_r28->mTranslate.z += l_horseBaseAnime.z;
            }
        } else {
            boar->setPlayerRide();
            temp_r28->mTranslate.x += l_boarBaseAnime.x;
            temp_r28->mTranslate.y += l_boarBaseAnime.y;
            temp_r28->mTranslate.z += l_boarBaseAnime.z;
        }

        mLeftHandIndex = 4;
        mRightHandIndex = 5;
        field_0x2fab = 0x13;

        if (field_0x2fc0 == 2) {
            field_0x2fc0 = 0;

            if (checkHorseDashAccept()) {
                setSingleAnimeParam(ANM_HORSE_LASH, &mpHIO->mHorse.m.mWhipAnm);
                setUnderAnimeParam(getMainBckData(ANM_HORSE_LASH)->m_underID, UNDER_2, &mpHIO->mHorse.m.mWhipAnm);
                onNoResetFlg1(FLG1_UNK_1000);
                voiceStart(Z2SE_AL_V_RUSH_HORSE);

                field_0x30a6 = mpHIO->mHorse.m.mWhipWaitTime;
                field_0x3004 = mpHIO->mHorse.m.mWalkOutProhibitionTime;
                horse->onPlayerBackRideLash();
            }
        } else if (mProcVar2.field_0x300c == 0 && checkHorseRide()) {
            setSingleAnime(ANM_HORSE_WAIT_A, 1.0f, 9.0f, -1, 4.0f);
        }
    
        field_0x2fb4 = 30;

        if (mProcVar3.field_0x300e != 0) {
            swordEquip(0);
        }

        procHorseWaitInit();
    }

    return 1;
}

int daAlink_c::procHorseGetOffInit(int param_0) {
    static const Vec leftOffset = {75.0f, 102.0f, 6.224f};
    static const Vec rightOffset = {-75.0f, 102.0f, 6.221f};

    cXyz spC;
    commonProcInit(PROC_HORSE_GETOFF);
    resetUnderAnime(UNDER_2, -1.0f);

    if (mEquipItem != fpcNm_ITEM_NONE) {
        allUnequip(0);
    }

    fopAc_ac_c* rideActor = mRideAcKeep.getActor();
    if (checkHorseRide()) {
        ((daHorse_c*)rideActor)->offRideFlg();
    } else {
        ((e_wb_class*)rideActor)->setPlayerRideNow();
    }

    field_0x37d4 = rideActor->current.pos;
    mProcVar3.field_0x300e = rideActor->shape_angle.y;
    field_0x2fc0 = param_0;

    mDoMtx_stack_c::transS(rideActor->current.pos.x, rideActor->current.pos.y, rideActor->current.pos.z);
    mDoMtx_stack_c::YrotM(rideActor->shape_angle.y);

    daAlink_ANM anm;
    if (field_0x2fc0 == 0) {
        if (checkHorseRide()) {
            anm = ANM_HORSE_GETOFF_LEFT;
        } else {
            anm = ANM_HORSE_FALL_LEFT;
        }

        mDoMtx_stack_c::multVec(&leftOffset, &spC);
    } else {
        if (checkHorseRide()) {
            anm = ANM_HORSE_GETOFF_RIGHT;
        } else {
            anm = ANM_HORSE_FALL_RIGHT;
        }

        mDoMtx_stack_c::multVec(&rightOffset, &spC);
    }

    setSingleAnimeBaseMorf(anm, 4.0f);

    mLinkGndChk.SetPos(&spC);

    spC.y = dComIfG_Bgsp().GroundCross(&mLinkGndChk);
    field_0x3478 = spC.y - rideActor->current.pos.y;
    if (field_0x3478 < l_autoDownHeight) {
        mProcVar4.field_0x3010 = 1;
        field_0x3478 = 0.0f;
    } else {
        mProcVar4.field_0x3010 = 0;
    }

    field_0x33b0 = 102.0f;
    current.pos.y -= 102.0f;
    field_0x2f99 = 0xF;
    shape_angle.z = 0;
    field_0x2fab = 1 << field_0x2fc0;

    if (field_0x2fc0 == 0) {
        field_0x37c8.x = -l_waitBaseAnime.z;
        field_0x37c8.z = l_waitBaseAnime.x;
    } else {
        field_0x37c8.x = l_waitBaseAnime.z;
        field_0x37c8.z = -l_waitBaseAnime.x;
    }

    field_0x3588 = l_waitBaseAnime;
    setFootEffectProcType(2);
    return 1;
}

int daAlink_c::procHorseGetOff() {
    daPy_frameCtrl_c* framectrl = &mUnderFrameCtrl[0];

    fopAc_ac_c* rideActor = mRideAcKeep.getActor();
    if (rideActor != NULL) {
        current.pos += rideActor->current.pos - field_0x37d4;
        field_0x37d4 = rideActor->current.pos;
        shape_angle.y += (s16)(mProcVar3.field_0x300e - rideActor->shape_angle.y);
        mProcVar3.field_0x300e = rideActor->shape_angle.y;
    }

    cLib_addCalcAngleS(&shape_angle.z, 0, 4, 2000, 400);

    if (framectrl->getFrame() >= 35.0f) {
        field_0x2fab = 0;
    } else if (checkHorseRide()) {
        if (framectrl->getFrame() >= 14.0f) {
            field_0x2fab &= ~(1 << field_0x2fc0);
        } else if (framectrl->getFrame() >= 4.0f) {
            if (field_0x2fc0 == 0) {
                field_0x2fab &= ~0x2;
            } else {
                field_0x2fab &= (u8)~0x1;
            }
        }
    } else if (!(framectrl->getFrame() >= 24.0f)) {
        if (framectrl->getFrame() >= 21.0f) {
            field_0x2fab &= ~(1 << field_0x2fc0);
        } else if (framectrl->getFrame() >= 3.0f) {
            if (field_0x2fc0 == 0) {
                field_0x2fab &= ~0x2;
            } else {
                field_0x2fab &= (u8)~0x1;
            }
        }
    }

    if (checkAnmEnd(framectrl)) {
        horseGetOffEnd();
        checkNextAction(0);
    } else if (framectrl->getFrame() >= 32.0f) {
        field_0x2f99 = 5;

        if (checkModeFlg(0x400)) {
            offModeFlg(0x400);
            rideGetOff();
            onModeFlg(1);

            if (fabsf(current.pos.y - mLinkAcch.GetGroundH()) < l_autoUpHeight) {
                field_0x2f9d = 4;
                dComIfGp_getVibration().StartShock(1, 0xF, cXyz(0.0f, 1.0f, 0.0f));
            }
        }
    } else {
        field_0x2f99 = 7;

        if (mLinkAcch.ChkGroundHit()) {
            horseGetOffEnd();
            return procLandInit(0.0f);
        }

        if (framectrl->getFrame() < 27.0f) {
            f32 temp_f31 = framectrl->getFrame() / 27.0f;
            field_0x3588.x = (l_waitBaseAnime.x * (1.0f - temp_f31)) + (field_0x37c8.x * temp_f31);
            field_0x3588.z = (l_waitBaseAnime.z * (1.0f - temp_f31)) + (field_0x37c8.z * temp_f31);
        } else if (framectrl->getFrame() < 32.0f) {
            current.pos.y += 0.2f * field_0x3478;
            if (mProcVar4.field_0x3010 != 0) {
                procFallInit(1, 4.0f);
            }
        }
    }

    return 1;
}

int daAlink_c::procHorseWaitInit() {
    if (!commonProcInitNotSameProc(PROC_HORSE_WAIT)) {
        return 0;
    }

    if (!checkHorseRide()) {
        setUnderAnime(dRes_ID_ALANM_BCK_WAITWA_e, UNDER_2, 1.0f, 0.0f, -1, -1.0f);
    } else if (checkHorseUnderItemAnime()) {
        resetUnderAnime(UNDER_2, -1.0f);
    }

    initServiceWaitTime();
    checkHorseGetOffAndSetDoStatus();

    setSyncRide((!checkHorseNoUpperAnime() && !checkEquipAnime()) || checkInputOnR() || !checkHorseGetOff() || checkHorseGetOffDirection() == 2);
    field_0x33f0 = 0.0f;
    return 1;
}

int daAlink_c::procHorseWait() {
    BOOL temp_r27 = checkHorseGetOffAndSetDoStatus();
    if (!setSyncRide(1)) {
        return 1;
    }

    daHorse_c* horse = (daHorse_c*)mRideAcKeep.getActor();
    if (checkHorseRide() && horse->checkRodeoMode() && !checkEventRun()) {
        return procHorseRunInit();
    }

    if (orderTalk(1)) {
        return 1;
    }

    if (!checkNextActionHorse()) {
        if (setHorseGetOff(temp_r27)) {
            return 1;
        }

        if (checkBoarSingleBattleFirst()) {
            fopAc_ac_c* boar = (fopAc_ac_c*)fopAcIt_Judge((fopAcIt_JudgeFunc)daAlink_searchBoar, NULL);
            if (boar != NULL) {
                int temp_r28 = abs((s16)(cLib_targetAngleY(&boar->current.pos, &current.pos) - boar->shape_angle.y));
                f32 dist_xz2 = boar->current.pos.abs2XZ(current.pos);

                if (temp_r28 > 0x800 && temp_r28 < 0x5000 && dist_xz2 < SQUARE(600.0f)) {
                    setBStatus(BUTTON_STATUS_CUT);
                } else if (checkEndResetFlg0(ERFLG0_SINGLE_BOAR_AVOID)) {
                    set3DStatus(BUTTON_STATUS_DODGE, 5);
                    setBStatus(BUTTON_STATUS_CUT);
                }
            }
        }
    }

    return 1;
}

int daAlink_c::procHorseTurnInit() {
    commonProcInit(PROC_HORSE_TURN);

    if (checkHorseRide()) {
        resetUnderAnime(UNDER_2, -1.0f);
    }

    setSyncRidePos();
    field_0x3004 = 0;
    setHorseTurnAnime();
    return 1;
}

int daAlink_c::procHorseTurn() {
    daHorse_c* horse = dComIfGp_getHorseActor();
    if (!setSyncRidePos()) {
        return 1;
    }

    if (mUnderFrameCtrl[0].getFrame() < 13.0f && mEquipItem == 0x103 && !checkBootsOrArmorHeavy() && checkCutTurnInputTrigger()) {
        horse->onCutTurnCancel();
        return procHorseCutTurnInit();
    }

    if (!horse->checkStop() && !horse->checkTurn()) {
        mUnderFrameCtrl[0].onEndFlg();
        procHorseWaitInit();
        field_0x30ca = 0;
    } else if (horse->checkTurnCancelFrame() || horse->checkStopCancelFrame()) {
        onModeFlg(4);
        horse->onTurnCancelKeep();

        if (checkHorseNoUpperAnime()) {
            if (checkShieldGet()) {
                setRStatus(BUTTON_STATUS_DEFEND);
            } else {
                setRStatus(BUTTON_STATUS_COVER);
            }

            if (spActionButton()) {
                resetUpperAnime(UPPER_2, -1.0f);
                setSingleAnimeBase(ANM_HORSE_CROUCH);
            } else {
                setHorseTurnAnime();
            }
        } else {
            setHorseTurnAnime();
        }
    
        BOOL temp_r28 = checkHorseGetOffAndSetDoStatus();
        if (orderTalk(1)) {
            return 1;
        }
    
        if (checkNextActionHorse()) {
            return 1;
        }
    
        if (setHorseGetOff(temp_r28)) {
            if (mProcID != PROC_HORSE_GETOFF) {
                horse->offCutTurnCancel();
            }
            return 1;
        }

        horse->offTurnCancelKeep();
    } else {
        if (checkBootsOrArmorHeavy() && !checkEventRun() && !checkBoarSingleBattle()) {
            if (horse->getAnmFrame(0) >= 30.0f) {
                return procCoLargeDamageInit(-4, TRUE, 0, 0, NULL, 0);
            }
        } else if (spActionButton() && !checkSpecialHorseRide()) {
            setDoStatus(BUTTON_STATUS_DISMOUNT);

            if (doTrigger()) {
                resetUpperAnime(UPPER_2, -1.0f);

                if (horse->checkTurnStand()) {
                    return procAutoJumpInit(0);
                } else {
                    return procBackJumpInit(0);
                }
            }
        }

        setHorseTurnAnime();
    }

    return 1;
}

int daAlink_c::procHorseJumpInit() {
    commonProcInit(PROC_HORSE_JUMP);

    if (checkHorseRide()) {
        resetUnderAnime(UNDER_2, -1.0f);
    }

    setSyncRidePos();
    field_0x3004 = 0;
    field_0x3002 = 0;
    setHorseTurnAnime();
    voiceStart(Z2SE_AL_V_JUMP_L);
    return 1;
}

int daAlink_c::procHorseJump() {
    daHorse_c* horse_p = dComIfGp_getHorseActor();

    if (!setSyncRidePos()) {
        return 1;
    }

    if (horse_p->checkLand()) {
        procHorseLandInit();
    } else {
        setHorseTurnAnime();
    }

    return 1;
}

int daAlink_c::procHorseLandInit() {
    commonProcInit(PROC_HORSE_LAND);

    if (checkHorseRide()) {
        resetUnderAnime(UNDER_2, -1.0f);
    }

    setSyncRidePos();
    field_0x3004 = 0;
    field_0x3002 = 0;
    setHorseTurnAnime();
    return 1;
}

int daAlink_c::procHorseLand() {
    daHorse_c* horse_p = dComIfGp_getHorseActor();

    if (!setSyncRidePos()) {
        return 1;
    }

    if (!horse_p->checkLand()) {
        procHorseWaitInit();
    } else {
        setHorseTurnAnime();
    }

    return 1;
}

int daAlink_c::procHorseSubjectivityInit() {
    commonProcInit(PROC_HORSE_SUBJECTIVITY);
    if (checkHorseUnderItemAnime()) {
        resetUnderAnime(UNDER_2, -1.0f);
    }

    setSyncRide(0);
    setSubjectMode();
    return 1;
}

int daAlink_c::procHorseSubjectivity() {
    onResetFlg0(RFLG0_UNK_4000000);
    s16 prev_shape_angle = shape_angle.y;
    s16 prev_angle = current.angle.y;
    int prev_3190 = field_0x3190;

    if (!setSyncRide(0)) {
        return 1;
    }

    shape_angle.y = prev_shape_angle;
    if (checkSubjectEnd(TRUE)) {
        procHorseWaitInit();
    } else if (checkNextActionFromButton()) {
        if (checkUpperReadyThrowAnime()) {
            field_0x3190 = prev_3190;
            mBodyAngle.y = field_0x3190;
            field_0x310a = mBodyAngle.x;
            field_0x310c = shape_angle.y + field_0x3190;
        }
        return 1;
    } else {
        setRideSubjectAngle(prev_angle);
    }

    return 1;
}

int daAlink_c::procHorseCutInit() {
    commonProcInit(PROC_HORSE_CUT);
    int var_r26 = 0;

    if (checkHorseUnderItemAnime()) {
        resetUnderAnime(UNDER_2, -1.0f);
    }

    setSyncRide(1);

    fopAc_ac_c* target_actor;
    if (mTargetedActor != NULL) {
        target_actor = mTargetedActor;
    } else {
        target_actor = mAttention->LockonTarget(0);
    }

    if (target_actor != NULL) {
        s16 sp8 = (s16)(cLib_targetAngleY(&current.pos, &target_actor->eyePos) - shape_angle.y);
        if (sp8 > 0x800) {
            var_r26 = 2;
        } else if (sp8 < -0x800) {
            var_r26 = 3;
        }
    }

    int type;
    if (mComboCutCount == 1) {
        if (var_r26 == 3) {
            type = 2;
        } else {
            type = 0;
        }
    } else if (var_r26 == 3) {
        if (cM_rnd() < 0.5f) {
            type = 2;
        } else {
            type = 3;
        }
    } else if (var_r26 == 2) {
        if (cM_rnd() < 0.5f) {
            type = 0;
        } else {
            type = 1;
        }
    } else {
        type = (int)cM_rndF(4.0f) % 4;
    }

    static daAlink_cutHorseParamTbl const cutParamTable[] = {
        {
            daAlink_c::FTANM_CUTHLA,
            dRes_ID_ALANM_BCK_CUTHLA_e,
            dRes_ID_ALANM_BCK_FCUTHLA_e,
            0x00,
            0x0E,
            CUT_TYPE_HORSE_LEFT_A,
        },
        {
            daAlink_c::FTANM_CUTHLB,
            dRes_ID_ALANM_BCK_CUTHLB_e,
            dRes_ID_ALANM_BCK_FCUTHLB_e,
            0x00,
            0x13,
            CUT_TYPE_HORSE_LEFT_B,
        },
        {
            daAlink_c::FTANM_CUTHRA,
            dRes_ID_ALANM_BCK_CUTHRA_e,
            dRes_ID_ALANM_BCK_FCUTHRA_e,
            0x00,
            0x13,
            CUT_TYPE_HORSE_RIGHT_A,
        },
        {
            daAlink_c::FTANM_CUTHRB,
            dRes_ID_ALANM_BCK_CUTHRB_e,
            dRes_ID_ALANM_BCK_FCUTHRB_e,
            0x00,
            0x00,
            CUT_TYPE_HORSE_RIGHT_A,
        },
    };

    const daAlinkHIO_hoCut_c1* anmParams;
    const daAlink_cutHorseParamTbl* cutParams = &cutParamTable[type];
    field_0x2f96 = 2;
    
    if (type == 0) {
        anmParams = &mpHIO->mCut.mHorseCutLeftA.m;
    } else if (type == 1) {
        anmParams = &mpHIO->mCut.mHorseCutLeftB.m;
    } else if (type == 2) {
        anmParams = &mpHIO->mCut.mHorseCutRightA.m;
    } else {
        anmParams = &mpHIO->mCut.mHorseCutRightB.m;
    }

    setUpperAnimeParam(cutParams->m_upperAnm, UPPER_2, &anmParams->mCutAnm);
    setFacePriTexture(cutParams->m_faceanm);
    setFacePriBck(cutParams->m_faceBtk);
    setCutType(cutParams->m_cutType);

    field_0x3478 = anmParams->mAttackStartFrame;
    field_0x347c = anmParams->mAttackEndFrame;
    field_0x3480 = anmParams->mAfterCutMorf;
    field_0x3484 = anmParams->mCutAnm.mCancelFrame;

    if (checkBoarSingleBattle()) {
        setSwordAtParam(dCcG_At_Spl_UNK_0, 1, dCcD_SE_SWORD, 2, mpHIO->mCut.m.mSwordLengthHorsebackFight, mpHIO->mCut.m.mSwordRadiusHorsebackFight);
    } else {
        setSwordAtParam(dCcG_At_Spl_UNK_0, 1, dCcD_SE_SWORD, 2, mpHIO->mCut.m.mSwordLengthHorseback, mpHIO->mCut.m.mSwordRadiusHorseback);
    }

    voiceStart(Z2SE_AL_V_ATTACK_S);

    field_0x307e = mpHIO->mCut.m.mComboDuration;
    mProcVar5.field_0x3012 = 0;
    field_0x3004 = 0;

    setCutWaterDropEffect();
    onNoResetFlg1(FLG1_UNK_10000000);
    return 1;
}

int daAlink_c::procHorseCut() {
    daPy_frameCtrl_c* framectrl = &mUpperFrameCtrl[2];

    if (setSyncRide(1) == 0) {
        return 1;
    }

    setComboReserb();
    checkCutTurnCharge();

    if (checkAnmEnd(framectrl)) {
        if (checkNoResetFlg2(FLG2_COMBO_RESERB)) {
            checkCutAction();
        } else {
            resetUpperAnime(UPPER_2, 4.0f);
            procHorseWaitInit();
            field_0x2060->initOldFrameMorf(field_0x3480, 0, 0x23);
        }
    } else if (framectrl->getFrame() > field_0x3484 && (checkNoResetFlg2(FLG2_COMBO_RESERB) || checkResetFlg0(RFLG0_UNK_40) || spActionButton())) {
        if (checkResetFlg0(RFLG0_UNK_40)) {
            procHorseCutChargeReadyInit();
        } else if (checkNoResetFlg2(FLG2_COMBO_RESERB)) {
            checkCutAction();
        } else {
            if (checkShieldGet()) {
                setRStatus(BUTTON_STATUS_DEFEND);
            } else {
                setRStatus(BUTTON_STATUS_COVER);
            }

            resetUpperAnime(UPPER_2, -1.0f);
            setSingleAnimeBase(ANM_HORSE_CROUCH);
            procHorseWaitInit();
        }
    } else {
        if (framectrl->getFrame() > field_0x3484) {
            if (checkShieldGet()) {
                setRStatus(BUTTON_STATUS_DEFEND);
            } else {
                setRStatus(BUTTON_STATUS_COVER);
            }
            resetCombo(TRUE);
        }
        
        if (framectrl->getFrame() >= field_0x3478 && framectrl->getFrame() < field_0x347c) {
            if (!checkNoResetFlg0(FLG0_CUT_AT_FLG)) {
                seStartSwordCut(Z2SE_AL_SWORD_SWING_S);
            }
            onResetFlg0(RFLG0_UNK_2);
        }
    }

    return 1;
}

int daAlink_c::procHorseCutChargeReadyInit() {
    commonProcInit(PROC_HORSE_CUT_CHARGE_READY);
    setUpperAnimeParam(dRes_ID_ALANM_BCK_CUTHTP_e, UPPER_2, &mpHIO->mCut.mHorseCutCharge.m.mChargeAnm);
    setFacePriBck(dRes_ID_ALANM_BCK_FAT_e);
    field_0x2f96 = 2;

    setSyncRide(1);
    if (checkHorseUnderItemAnime()) {
        resetUnderAnime(UNDER_2, -1.0f);
    }

    mProcVar2.field_0x300c = mpHIO->mCut.m.mNormalSwingDuration;
    field_0x3004 = 0;
    onNoResetFlg1(FLG1_UNK_10000000);
    return 1;
}

int daAlink_c::procHorseCutChargeReady() {
    daPy_frameCtrl_c* framectrl = &mUpperFrameCtrl[2];

    if (!setSyncRide(1)) {
        return 1;
    }

    setSwordChargeVoiceSe();
    BOOL var_r30 = FALSE;

    if (checkAnmEnd(framectrl)) {
        if (!checkWoodSwordEquip()) {
            simpleAnmPlay(m_nSwordBtk);
        } else {
            mProcVar0.field_0x3008++;

            if (mProcVar0.field_0x3008 >= 14.0f) {
                mProcVar0.field_0x3008 = 100;
            }
        }

        if (m_nSwordBtk->getFrame() >= 14.0f || mProcVar0.field_0x3008 >= 14.0f) {
            var_r30 = TRUE;
        }
    }

    if (!swordButton()) {
        if (mProcVar2.field_0x300c != 0) {
            checkCutAction();
        } else if (var_r30) {
            procHorseCutTurnInit();
        } else {
            checkCutAction();
        }
    }

    return 1;
}

int daAlink_c::procHorseCutTurnInit() {
    commonProcInit(PROC_HORSE_CUT_TURN);

    if (checkHorseUnderItemAnime()) {
        resetUnderAnime(UNDER_2, -1.0f);
    }

    setSingleAnimeParam(ANM_HORSE_CUT_TURN, &mpHIO->mCut.mHorseCutCharge.m.mSpinAnm);
    setCutType(CUT_TYPE_HORSE_TURN);
    setSyncRide(1);
    field_0x3478 = mpHIO->mCut.mHorseCutCharge.m.mSpinAttackRadius;
    setCylAtParam(getSwordAtType(), dCcG_At_Spl_UNK_1, 3, dCcD_SE_SWORD, 3, field_0x3478 * 0.5f, 200.0f);
    mProcVar4.field_0x3010 = mpHIO->mCut.mHorseCutCharge.m.mSpinStopTime;
    voiceStart(Z2SE_AL_V_KAITEN);
    setCutWaterDropEffect();
    setHorseZeldaDamage();
    return 1;
}

int daAlink_c::procHorseCutTurn() {
    daPy_frameCtrl_c* framectrl = &mUnderFrameCtrl[0];

    if (!setSyncRide(1)) {
        return 1;
    }

    if (checkAnmEnd(framectrl)) {
        procHorseWaitInit();
        field_0x2060->initOldFrameMorf(4.0f, 0, 0x23);
    } else {
        if (framectrl->getFrame() > mpHIO->mCut.mHorseCutCharge.m.mSpinAnm.mCancelFrame) {
            if (orderTalk(1)) {
                return 1;
            }
            if (checkNextActionHorse()) {
                field_0x2060->initOldFrameMorf(4.0f, 0, 0x23);
                return 1;
            }
            if (checkInputOnR()) {
                procHorseWaitInit();
                field_0x2060->initOldFrameMorf(4.0f, 0, 0x23);
                return 1;
            }
        }

        if (framectrl->getFrame() >= mpHIO->mCut.mHorseCutCharge.m.mSpinAttackStartFrame && framectrl->getFrame() < mpHIO->mCut.mHorseCutCharge.m.mSpinAttackEndFrame) {
            if (!checkNoResetFlg0(FLG0_CUT_AT_FLG)) {
                seStartSwordCut(Z2SE_AL_KAITENGIRI);
            }

            onResetFlg0(RFLG0_UNK_2);
            cLib_chaseF(mAtCyl.GetRP(), field_0x3478, 20.0f);
        }
    }

    return 1;
}

int daAlink_c::procHorseDamageInit(dCcD_GObjInf* i_hitObj) {
    commonProcInit(PROC_HORSE_DAMAGE);

    if (checkHorseRide()) {
        resetUnderAnime(UNDER_2, -1.0f);
    }

    setSyncRide(0);

    static const u16 anmIdx[] = {
        dRes_ID_ALANM_BCK_DAMHFS_e,
        dRes_ID_ALANM_BCK_DAMHBS_e,
        dRes_ID_ALANM_BCK_DAMHLS_e,
        dRes_ID_ALANM_BCK_DAMHRS_e,
        dRes_ID_ALANM_BCK_DAMHDS_e,
    };

    u16 var_r26 = mUnderAnmHeap[0].getIdx();

    if (checkUnderMove0BckNoArc(ANM_HORSE_STOP) || checkUnderMove0BckNoArc(ANM_HORSE_STOP_TO_STAND) || checkUnderMove0BckNoArc(ANM_HORSE_STAND) || checkUnderMove0BckNoArc(ANM_HORSE_DASH_B) || checkUnderMove0BckNoArc(ANM_HORSE_DASH_A) || checkUnderMove0BckNoArc(ANM_HORSE_DASH_START)) {
        mProcVar4.field_0x3010 = 0;
        mProcVar5.field_0x3012 = 0;
        setUpperAnimeParam(anmIdx[4], UPPER_2, &mpHIO->mDamage.mDamHorse.m.mNoDirectionAnm);
    } else {
        cXyz* dmg_vec = getDamageVec(i_hitObj);
        
        f32 temp_f31 = cM_ssin(shape_angle.y);
        f32 temp_f1 = cM_scos(shape_angle.y);
        cXyz spC((dmg_vec->z * -temp_f31) + (dmg_vec->x * temp_f1), dmg_vec->y, (dmg_vec->z * temp_f1) + (dmg_vec->x * temp_f31));

        mProcVar4.field_0x3010 = cLib_minMaxLimit<s16>(cM_atan2s(spC.z, spC.y), -mpHIO->mDamage.mDamHorse.m.mFrontBackBodyMaxAngle, mpHIO->mDamage.mDamHorse.m.mFrontBackBodyMaxAngle);
        mProcVar5.field_0x3012 = cLib_minMaxLimit<s16>(cM_atan2s(spC.x, -JMAFastSqrt(SQUARE(spC.y) + SQUARE(spC.z))), -mpHIO->mDamage.mDamHorse.m.mLeftRightBodyMaxAngle, mpHIO->mDamage.mDamHorse.m.mLeftRightBodyMaxAngle);
        
        int direction = getDirectionFromAngle(cM_atan2s(-spC.x, -spC.z));
        setUpperAnimeParam(anmIdx[direction], UPPER_2, &mpHIO->mDamage.mDamHorse.m.mWithDirectionAnm);
    }

    setFacePriBck(dRes_ID_ALANM_BCK_FB_e);
    setFacePriTexture(FTANM_B_A);
    voiceStart(Z2SE_AL_V_DAMAGE_S);
    seStartOnlyReverb(Z2SE_AL_DAMAGE_NORMAL);
    
    daPy_frameCtrl_c* framectrl = &mUpperFrameCtrl[2];
    field_0x347c = M_PI / (framectrl->getEnd() - framectrl->getStart());
    setHorseZeldaDamage();
    onNoResetFlg1(FLG1_UNK_10000000);
    return 1;
}

int daAlink_c::procHorseDamage() {
    if (!setSyncRide(0)) {
        return 1;
    }

    daPy_frameCtrl_c* framectrl = &mUpperFrameCtrl[2];
    f32 temp_f1 = cM_fsin(field_0x347c * (framectrl->getFrame() - framectrl->getStart()));

    mBodyAngle.x = mProcVar4.field_0x3010 * temp_f1;
    mBodyAngle.z = -mProcVar5.field_0x3012 * temp_f1;
    mBodyAngle.y = 0;

    if (checkAnmEnd(framectrl)) {
        resetUpperAnime(UPPER_2, 4.0f);
        procHorseWaitInit();
    }

    return 1;
}

int daAlink_c::procHorseBowSubjectInit() {
    bool var_r30 = mProcID == PROC_HORSE_BOW_MOVE;

    if (!commonProcInitNotSameProc(PROC_HORSE_BOW_SUBJECT)) {
        return 0;
    }

    setSyncRidePos();
    setBaseRideAnime();
    mProcVar2.field_0x300c = 0;
    setBowOrSlingStatus();
    return 1;
}

int daAlink_c::procHorseBowSubject() {
    setDoStatus(BUTTON_STATUS_BACK);

    if (!setSyncRidePos()) {
        return 1;
    }

    if (checkHorseSpecialProc()) {
        return 1;
    }

    setBaseRideAnime();

    if (!checkNextActionHorse() && setRideSubjectAngle(current.angle.y)) {
        setBowSight();
    }

    return 1;
}

int daAlink_c::procHorseBowMoveInit() {
    if (!commonProcInitNotSameProc(PROC_HORSE_BOW_MOVE)) {
        return 0;
    }

    setSyncRidePos();
    setBaseRideAnime();
    setBowOrSlingStatus();
    return 1;
}

int daAlink_c::procHorseBowMove() {
    setDoStatus(BUTTON_STATUS_BACK);

    if (!setSyncRidePos()) {
        return 1;
    }

    if (checkHorseSpecialProc()) {
        return 1;
    }

    setBaseRideAnime();
    cancelBowMove();

    if (!checkNextActionHorse()) {
        setBodyAngleRideReadyAnime();
    }

    return 1;
}

int daAlink_c::procHorseGrabMoveInit() {
    if (!commonProcInitNotSameProc(PROC_HORSE_GRAB_MOVE)) {
        return 0;
    }

    setSyncRidePos();
    setBaseRideAnime();
    field_0x3004 = 0;
    return 1;
}

int daAlink_c::procHorseGrabMove() {
    if (!setSyncRidePos()) {
        return 1;
    }

    if (checkHorseSpecialProc()) {
        return 1;
    }

    if (mGrabItemAcKeep.getActor() == NULL && checkGrabAnime()) {
        resetUpperAnime(UPPER_2, 4.0f);
    }

    setBaseRideAnime();

    if (checkGrabThrowAnime()) {
        if (checkUpperGrabItemThrow(4.0f)) {
            procHorseWaitInit();
        }
    } else {
        checkNextActionHorse();
    }

    return 1;
}

int daAlink_c::procHorseBoomerangSubjectInit() {
    if (!commonProcInitNotSameProc(PROC_HORSE_BOOMERANG_SUBJECT)) {
        return 0;
    }

    setSyncRidePos();
    setBaseRideAnime();
    initBoomerangUpperAnimeSpeed(1);
    dComIfGp_setPlayerStatus0(0, 0x80000);
    return 1;
}

int daAlink_c::procHorseBoomerangSubject() {
    if (!checkItemActorPointer()) {
        return 1;
    }

    if (checkBoomerangReadyAnime()) {
        setDoStatus(BUTTON_STATUS_BACK);
    }

    if (!setSyncRidePos()) {
        return 1;
    }

    if (checkHorseSpecialProc()) {
        return 1;
    }

    setBaseRideAnime();

    if (!checkNextActionHorse()) {
        if (setRideSubjectAngle(current.angle.y)) {
            setBoomerangSight();
        }
    } else {
        mSight.offDrawFlg();
    }

    return 1;
}

int daAlink_c::procHorseBoomerangMoveInit() {
    if (!commonProcInitNotSameProc(PROC_HORSE_BOOMERANG_MOVE)) {
        return 0;
    }

    setSyncRidePos();
    setBaseRideAnime();
    initBoomerangUpperAnimeSpeed(0);
    dComIfGp_setPlayerStatus0(0, 0x80000);
    return 1;
}

int daAlink_c::procHorseBoomerangMove() {
    if (!checkItemActorPointer()) {
        return 1;
    }

    if (checkBoomerangReadyAnime()) {
        setDoStatus(BUTTON_STATUS_BACK);
    }

    if (!setSyncRidePos()) {
        return 1;
    }

    if (checkHorseSpecialProc()) {
        return 1;
    }

    setBaseRideAnime();

    if (!checkNextActionHorse()) {
        setBodyAngleRideReadyAnime();
    }

    return 1;
}

int daAlink_c::procHorseHookshotSubjectInit() {
    if (!commonProcInitNotSameProc(PROC_HORSE_HOOKSHOT_SUBJECT)) {
        return 0;
    }

    setSyncRidePos();
    setBaseRideAnime();
    initHookshotUpperAnimeSpeed(1);

    #if PLATFORM_GCN
    setHookshotReadyMaterial();
    #endif

    dComIfGp_setPlayerStatus0(0, 0x4000);
    return 1;
}

int daAlink_c::procHorseHookshotSubject() {
    if (checkHookshotWait()) {
        setDoStatus(BUTTON_STATUS_BACK);
    }

    if (!setSyncRidePos()) {
        return 1;
    }

    if (checkHorseSpecialProc()) {
        return 1;
    }

    setBaseRideAnime();

    if (!checkNextActionHorse()) {
        if (setRideSubjectAngle(current.angle.y)) {
            setHookshotSight();
        }
    } else {
        mSight.offDrawFlg();
    }

    return 1;
}

int daAlink_c::procHorseHookshotMoveInit() {
    if (!commonProcInitNotSameProc(PROC_HORSE_HOOKSHOT_MOVE)) {
        return 0;
    }

    setSyncRidePos();
    setBaseRideAnime();
    initHookshotUpperAnimeSpeed(0);
    dComIfGp_setPlayerStatus0(0, 0x4000);
    return 1;
}

int daAlink_c::procHorseHookshotMove() {
    cancelHookshotMove();

    if (checkHookshotWait()) {
        setDoStatus(BUTTON_STATUS_BACK);
    }

    if (!setSyncRidePos()) {
        return 1;
    }

    if (checkHorseSpecialProc()) {
        return 1;
    }

    setBaseRideAnime();

    if (!checkNextActionHorse()) {
        setBodyAngleRideReadyAnime();
    }

    return 1;
}

int daAlink_c::procHorseBottleDrinkInit(u16 i_itemNo) {
    if (!dComIfGp_event_compulsory(this, NULL, 0xFFFF)) {
        return 0;
    }

    mDemo.setSpecialDemoType();
    commonProcInit(PROC_HORSE_BOTTLE_DRINK);

    if (checkHorseUnderItemAnime()) {
        resetUnderAnime(UNDER_2, -1.0f);
    }

    setSyncRidePos();
    setBaseRideAnime();

    if (checkHorseRide()) {
        daHorse_c* horse = dComIfGp_getHorseActor();
        horse->changeOriginalDemo();
        horse->changeDemoMode(0xE, 0);
    } else {
        mRideAcKeep.getActor()->speedF = 0.0f;
    }

    setUpperAnimeParam(getMainBckData(ANM_BOTTLE_DRINK_START)->m_upperID, UPPER_2, &mpHIO->mItem.mBottle.m.mStartDrinkAnm);
    setFacePriAnime(ANM_BOTTLE_DRINK_START);

    keepItemData();
    setBottleModel(i_itemNo);
    dComIfGp_setPlayerStatus1(0, 0x2000);

    field_0x319c = 1;
    dCam_getBody()->StartEventCamera(0x12, fopAcM_GetID(this), "Type", 1, &field_0x319c, 0);
    mProcVar2.field_0x300c = 0;
    return 1;
}

int daAlink_c::procHorseBottleDrink() {
    daHorse_c* horse = dComIfGp_getHorseActor();

    if (!setSyncRidePos()) {
        resetUpperAnime(UPPER_2, 3.0f);
        returnKeepItemData();
        resetSpecialEvent();
        return 1;
    }

    setBaseRideAnime();
    commonBottleDrink(1);
    return 1;
}

int daAlink_c::procHorseKandelaarPourInit() {
    if (!dComIfGp_event_compulsory(this, NULL, 0xFBFF)) {
        return 0;
    }

    mDemo.setSpecialDemoType();
    commonProcInit(PROC_HORSE_KANDELAAR_POUR);

    if (checkHorseUnderItemAnime()) {
        resetUnderAnime(UNDER_2, -1.0f);
    }

    setSyncRidePos();
    setBaseRideAnime();

    field_0x3478 = mRideAcKeep.getActor()->speedF;

    if (checkHorseRide()) {
        daHorse_c* horse = dComIfGp_getHorseActor();
        horse->changeOriginalDemo();
        horse->changeDemoMode(1, 0);
    } else {
        mRideAcKeep.getActor()->speedF = 0.0f;
    }

    setUpperAnimeParam(getMainBckData(ANM_OIL_BOTTLE_POUR_START)->m_upperID, UPPER_2, &mpHIO->mItem.mLantern.m.mBeginUnkAnm);
    commonKandelaarPourInit();
    return 1;
}

int daAlink_c::procHorseKandelaarPour() {
    if (!setSyncRidePos()) {
        mRideAcKeep.getActor()->speedF = field_0x3478;
    
        if (checkHorseRide() && dComIfGp_getHorseActor() != NULL) {
            dComIfGp_getHorseActor()->cancelOriginalDemo();
        }
    
        resetUpperAnime(UPPER_2, 3.0f);
        resetOilBottleModel();
        resetSpecialEvent();
        return 1;
    }

    setBaseRideAnime();

    if (commonKandelaarPour(1) && checkHorseRide()) {
        mRideAcKeep.getActor()->speedF = field_0x3478;
        dComIfGp_getHorseActor()->cancelOriginalDemo();
    }

    return 1;
}

int daAlink_c::procHorseComebackInit() {
    if (!commonProcInitNotSameProc(PROC_HORSE_COMEBACK)) {
        return 0;
    }

    setSingleAnimeBase(ANM_HORSE_DIE_RECOVER);
    field_0x2fab = 0;
    setSyncRidePos();
    return 1;
}

int daAlink_c::procHorseComeback() {
    daPy_frameCtrl_c* frameCtrl_p = mUnderFrameCtrl;

    setSyncRidePos();
    if (checkAnmEnd(frameCtrl_p)) {
        field_0x2fab = 0x13;
        return procHorseWaitInit();
    } else if (frameCtrl_p->getFrame() >= 78.0f) {
        field_0x2fab |= (u8)2;
    } else if (frameCtrl_p->getFrame() >= 75.0f) {
        mRightHandIndex = 5;
        field_0x2fab |= (u8)0x10;
    } else if (frameCtrl_p->getFrame() >= 68.0f) {
        field_0x2fab |= (u8)1;
    } else if (frameCtrl_p->checkPass(20.0f)) {
        voiceStart(Z2SE_AL_V_LANDING_FAIL_2);
    }

    return 1;
}

int daAlink_c::procHorseRunInit() {
    commonProcInit(PROC_HORSE_RUN);
    initForceRideHorse();
    deleteEquipItem(TRUE, FALSE);
    setSingleAnimeBaseMorf(ANM_HORSE_TAME_WAIT_B, -1.0f);
    setSyncHorsePos();
    field_0x2fab = 0;
    mProcVar4.field_0x3010 = 0;
    mProcVar5.field_0x3012 = 0;

    field_0x3478 = 1000.0f - cM_rndF(500.0f);
    mProcVar2.field_0x300c = 0;
    mProcVar3.field_0x300e = 30;
    field_0x3004 = 0;
    field_0x3002 = 0;
    mDoAud_changeSubBgmStatus(2);
    field_0x32cc = 0;
    return 1;
}

int daAlink_c::procHorseRun() {
    daHorse_c* horse = dComIfGp_getHorseActor();
    if (!setSyncHorsePos()) {
        mDoAud_changeSubBgmStatus(1);
        return 1;
    }

    if (field_0x32cc != 0) {
        onSceneChangeArea(17, 0xFF, NULL);
        return 1;
    }

    if (horse->speedF > 0.0f) {
        if (mProcVar3.field_0x300e != 0) {
            mProcVar3.field_0x300e--;
        }

        if (horse->checkRodeoLeft()) {
            if (mProcVar3.field_0x300e != 0) {
                mProcVar5.field_0x3012 -= (s16)150;
            } else {
                mProcVar5.field_0x3012 -= (s16)600;
            }

            if (mProcVar2.field_0x300c == 0) {
                set3DStatus(BUTTON_STATUS_HOLD_ON, 4);
            }
        } else {
            if (mProcVar3.field_0x300e != 0) {
                mProcVar5.field_0x3012 += (s16)150;
            } else {
                mProcVar5.field_0x3012 += (s16)600;
            }

            if (mProcVar2.field_0x300c == 0) {
                set3DStatus(BUTTON_STATUS_HOLD_ON, 1);
            }
        }

        if (checkInputOnR()) {
            int direction = getDirectionFromAngle(mStickAngle);
            s16 temp_r26 = mProcVar5.field_0x3012;

            if (direction == DIR_LEFT && !horse->checkRodeoLeft()) {
                mProcVar5.field_0x3012 -= (600.0f + (256.0f * mStickValue));
            } else if (direction == DIR_RIGHT && horse->checkRodeoLeft()) {
                mProcVar5.field_0x3012 += (600.0f + (256.0f * mStickValue));
            }

            if (temp_r26 * mProcVar5.field_0x3012 <= 0) {
                mProcVar5.field_0x3012 = 0;
            }
        }

        daPy_frameCtrl_c* framectrl = &mUnderFrameCtrl[0];
        f32 temp_f31 = framectrl->getFrame() / framectrl->getEnd();
    
        mProcVar4.field_0x3010 = (field_0x3478 * (1.0f + cM_fsin((M_PI*2) * (temp_f31 - 0.69999999f)))) + (4000.0f * (1.0f - fabsf(0.000099999997f * mProcVar5.field_0x3012)));
        field_0x3088 = field_0x3478 * (1.0f + cM_fsin((M_PI*2) * (temp_f31 - 1.0f)));
    
        if (framectrl->checkPass(0.0f)) {
            field_0x3478 = 1000.0f - cM_rndF(500.0f);
        }

        if (abs(mProcVar5.field_0x3012) >= 10000 || mProcVar2.field_0x300c != 0) {
            if (mProcVar3.field_0x300e != 0) {
                if (mProcVar5.field_0x3012 >= 10000) {
                    mProcVar5.field_0x3012 = 10000;
                } else if (mProcVar5.field_0x3012 <= -10000) {
                    mProcVar5.field_0x3012 = -10000;
                }
            } else {
                shape_angle.y += mProcVar5.field_0x3012;
                boarForceGetOff();
                mDoAud_changeSubBgmStatus(1);
            }
        }
    } else if (horse->getRodeoPointCnt() && horse->checkTurnStand()) {
        setDoStatus(BUTTON_STATUS_SEIZE);

        if (doTrigger()) {
            field_0x32cc = 1;
            onSceneChangeArea(17, 0xFF, NULL);
            horse->offRodeoMode();
        } else {
            mProcVar2.field_0x300c = 1;
        }
    }

    return 1;
}

int daAlink_c::procHorseHangInit(dCcD_GObjInf* i_hitObj, BOOL i_isPlaySE) {
    if (checkBoarRide()) {
        return procHorseDamageInit(i_hitObj);
    }

    commonProcInit(PROC_HORSE_HANG);

    daAlink_ANM anm;
    cXyz* dmg_vec = getDamageVec(i_hitObj);
    s16 var_r28 = dmg_vec->atan2sX_Z();
    if ((s16)(var_r28 - shape_angle.y) > 0) {
        anm = ANM_HORSE_DMG_LEFT;
        field_0x32cc = 0x58;
        mProcVar4.field_0x3010 = 1;
        mProcVar5.field_0x3012 = 2;
    } else {
        anm = ANM_HORSE_DMG_RIGHT;
        field_0x32cc = 0x59;
        mProcVar4.field_0x3010 = 2;
        mProcVar5.field_0x3012 = 1;
    }

    field_0x2fab &= ~mProcVar5.field_0x3012;
    setSingleAnimeBase(anm);
    mProcVar3.field_0x300e = 0;
    field_0x3004 = 0;
    field_0x3002 = 0;
    resetUnderAnime(UNDER_2, -1.0f);

    setSyncHorsePos();
    setHorseZeldaDamage();

    if (i_isPlaySE) {
        voiceStart(Z2SE_AL_V_DAMAGE_ON_HORSE);
        seStartOnlyReverb(Z2SE_AL_DAMAGE_NORMAL);
    }

    return 1;
}

int daAlink_c::procHorseHang() {
    if (!setSyncHorsePos()) {
        return 1;
    }

    if (checkHorseSpecialProc()) {
        if (checkSpecialHorseRide()) {
            field_0x2fab |= (u8)3;
        } else {
            boarForceGetOff();
        }

        return 1;
    }

    daPy_frameCtrl_c* framectrl = &mUnderFrameCtrl[0];

    if (checkAnmEnd(framectrl)) {
        if (mProcVar3.field_0x300e == 0) {
            mProcVar3.field_0x300e = 1;
            voiceStart(Z2SE_AL_V_CLING_HORSE);
            setSingleAnimeBase((daAlink_c::daAlink_ANM)field_0x32cc);
        } else {
            checkNextActionHorse();
        }
    } else if (mProcVar3.field_0x300e == 0) {
        if (framectrl->checkPass(7.0f)) {
            field_0x2fab &= ~mProcVar4.field_0x3010;
        } else if (framectrl->checkPass(36.0f)) {
            field_0x2fab |= mProcVar4.field_0x3010;
        }
    } else if (framectrl->checkPass(7.0f)) {
        field_0x2fab |= mProcVar5.field_0x3012;
    }

    return 1;
}

int daAlink_c::procHorseGetKeyInit() {
    if (!commonProcInitNotSameProc(PROC_HORSE_GET_KEY)) {
        return 0;
    }

    deleteEquipItem(FALSE, FALSE);
    setSingleAnimeBase(ANM_KEY_CATCH);
    setSyncRidePos();
    mProcVar2.field_0x300c = 0;
    return 1;
}

int daAlink_c::procHorseGetKey() {
    daPy_frameCtrl_c* frameCtrl_p = mUnderFrameCtrl;

    setSyncRidePos();
    if (checkAnmEnd(frameCtrl_p)) {
        dComIfGp_evmng_cutEnd(mAlinkStaffId);
    } else if (frameCtrl_p->getFrame() >= 34.0f) {
        mLeftHandIndex = 254;
        mRightHandIndex = 254;
        mProcVar2.field_0x300c = 1;
    } else if (frameCtrl_p->getFrame() >= 12.0f) {
        mLeftHandIndex = 2;
    } else if (frameCtrl_p->getFrame() >= 5.0f) {
        mLeftHandIndex = 3;
    }

    return 1;
}

int daAlink_c::procHorseLookDownInit() {
    if (!commonProcInitNotSameProc(PROC_HORSE_LOOK_DOWN)) {
        return 0;
    }

    if (mEquipItem != 0x103) {
        deleteEquipItem(FALSE, FALSE);
        setSwordModel();
    }

    setSingleAnimeBase(ANM_GANON_ON_HORSE);
    setSyncRidePos();
    return 1;
}

int daAlink_c::procHorseLookDown() {
    setSyncRidePos();

    if (checkAnmEnd(mUnderFrameCtrl)) {
        dComIfGp_evmng_cutEnd(mAlinkStaffId);
    }

    return 1;
}

int daAlink_c::procBoarRunInit() {
    commonProcInit(PROC_BOAR_RUN);
    resetUpperAnime(UPPER_2, -1.0f);
    resetUnderAnime(UNDER_2, -1.0f);

    mProcVar4.field_0x3010 = 0;
    mProcVar5.field_0x3012 = 0;
    field_0x3478 = 3000.0f - cM_rndF(1500.0f);
    setSingleAnimeBaseSpeed(ANM_HORSE_TAME_WAIT_A_TO_B, 1.0f, 4.0f);
    mProcVar3.field_0x300e = 0;

    setSyncBoarPos();
    mProcVar2.field_0x300c = 0;
    deleteEquipItem(FALSE, FALSE);
    return 1;
}

int daAlink_c::procBoarRun() {
    e_wb_class* boar = (e_wb_class*)mRideAcKeep.getActor();
    fopAc_ac_c* boar_actor = (fopAc_ac_c*)boar;

    if (mProcVar3.field_0x300e == 0 && checkAnmEnd(&mUnderFrameCtrl[0])) {
        setSingleAnimeBaseSpeed(ANM_HORSE_TAME_WAIT_B, 0.0f, -1.0f);
        mProcVar3.field_0x300e = 1;
    }

    if ((mProcVar3.field_0x300e != 0 && !setSyncBoarRunPos()) || (mProcVar3.field_0x300e == 0 && !setSyncBoarPos())) {
        return 1;
    }

    if (boar_actor->speedF > 0.0f && mProcVar3.field_0x300e != 0) {
        mProcVar4.field_0x3010 = (field_0x3478 * (1.0f + cM_fsin((M_PI*2) * (boar->getAnimeFrameRate() - 0.2f)))) - 2000.0f;
        field_0x3088 = field_0x3478 * (1.0f + cM_fsin((M_PI*2) * (boar->getAnimeFrameRate() - 0.4f)));
        
        if (boar->checkAnmLoopFrame()) {
            field_0x3478 = 3000.0f - cM_rndF(1500.0f);
        }
    } else if (mProcVar2.field_0x300c == 0) {
        if (mUnderFrameCtrl[0].getFrame() >= 14.0f) {
            mLeftHandIndex = 0xFE;
            mRightHandIndex = 0xFE;
            mProcVar2.field_0x300c = 1;
        }
    }

    return 1;
}
