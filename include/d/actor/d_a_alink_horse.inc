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

static void* daAlink_searchBoar(fopAc_ac_c* i_actor, void*) {
    if (fopAcM_GetName(i_actor) == PROC_E_WB) {
        e_wb_class* e_wb_p = (e_wb_class*)i_actor;

        if (e_wb_p->getModel() != NULL) {
            return i_actor;
        }
    }

    return NULL;
}

static fopAc_ac_c* daAlink_searchCoach(fopAc_ac_c* i_actor, void*) {
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
    if (dComIfGp_getHorseActor() != NULL) {
        daHoZelda_c* zelda_p = dComIfGp_getHorseActor()->getZeldaActor();
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

    fopAc_ac_c* temp_r4 = field_0x27f4;
    const Vec* offset_p;
    if (fopAcM_GetName(temp_r4) == PROC_HORSE) {
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

    cXyz sp2C;
    cXyz sp38(temp_r4->current.pos.x, temp_r4->current.pos.y, temp_r4->current.pos.z);

    mDoMtx_stack_c::transS(field_0x27f4->current.pos);
    mDoMtx_stack_c::YrotM(field_0x27f4->shape_angle.y);
    mDoMtx_stack_c::multVec(offset_p, &sp2C);

    dBgS_AcchCir* acchcir_p = mAcchCir;
    for (int i = 0; i < 6; i++) {
        sp38.y = sp2C.y = current.pos.y + acchcir_p->GetWallH();

        if (commonLineCheck(&sp2C, &sp38)) {
            return 0;
        }

        if (i == 2) {
            acchcir_p = mAcchCir;
            sp2C.x = current.pos.x;
            sp2C.z = current.pos.z;
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
    e_wb_class* e_wb = (e_wb_class*)fopAcIt_Judge((fopAcIt_JudgeFunc)daAlink_searchBoar, NULL);
    fopAc_ac_c* e_wb_actor = (fopAc_ac_c*)e_wb;

    if (e_wb == NULL) {
        return 0;
    }

    commonInitForceRideRein();
    e_wb_actor->current.pos = current.pos;
    e_wb_actor->shape_angle.y = shape_angle.y;
    e_wb_actor->current.angle.y = shape_angle.y;
    mRideStatus = 2;

    mRideAcKeep.setData(e_wb_actor);
    field_0x384c = (cXyz*)&l_boarBaseAnime;
    e_wb->setPlayerRide();
    return 1;
}

void daAlink_c::initForceRideHorse() {
    commonInitForceRideRein();
    mRideStatus = 1;
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
    offNoResetFlg1(FLG1_UNK_1800);
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
    setUpperAnimeBaseSpeed(0xCC, mpHIO->mHorse.m.mSwordUpAnmSpeed,
                           mpHIO->mHorse.m.mSwordUpInterpolation);
    setFaceBck(0xE8, 1, 0xFFFF);
}

void daAlink_c::setHorseTurnUpperAnime(int i_useLAnime) {
    if (checkHorseNoUpperAnime() || (i_useLAnime == FALSE && checkHorseTurnRAnime()) ||
        (i_useLAnime == TRUE && checkHorseTurnLAnime()))
    {
        setUpperAnimeBaseMorf(!i_useLAnime ? (u16)0x24C : (u16)0x24D, 4.0f);
    }
}

BOOL daAlink_c::checkHorseNoUpperAnime() const {
    return checkHorseTiredAnime() || checkNoUpperAnime() || checkHorseSwordUpAnime();
}

void daAlink_c::getHorseReinHandPos(cXyz* param_0, cXyz* param_1) {
    int hand_type = getReinHandType();
    if (hand_type == 2) {
        mDoMtx_multVec(getRightHandMatrix(), &m_handRightInSidePos, param_0);
        mDoMtx_multVec(getRightHandMatrix(), &m_handRightOutSidePos, param_1);
    } else if (hand_type == 1) {
        mDoMtx_multVec(getLeftHandMatrix(), &m_handLeftOutSidePos, param_0);
        mDoMtx_multVec(getLeftHandMatrix(), &m_handLeftInSidePos, param_1);
    } else {
        mDoMtx_multVec(getLeftHandMatrix(), &m_handLeftOutSidePos, param_0);
        mDoMtx_multVec(getRightHandMatrix(), &m_handRightOutSidePos, param_1);
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
    daHorse_c* temp_r3 = dComIfGp_getHorseActor();
    if (temp_r3 == NULL || (!checkHorseRide() && (mProcID != PROC_TOOL_DEMO || field_0x2fab == 0))) {
        return;
    }

    if (field_0x2fab & 1) {
        mDoMtx_stack_c::copy(mpLinkModel->getAnmMtx(field_0x30bc));
        mDoMtx_stack_c::transM(-2.0f, -11.0f, 1.5f);       
        mDoMtx_stack_c::ZXYrotM(NULL, -0x8000, 0x4000);
        mDoMtx_copy(mDoMtx_stack_c::get(), temp_r3->getLeftStirrupMtx());
    }

    if (field_0x2fab & 2) {
        mDoMtx_stack_c::copy(mpLinkModel->getAnmMtx(field_0x30be));
        mDoMtx_stack_c::transM(-2.0f, 11.0f, 1.5f);     
        mDoMtx_stack_c::ZrotM(-0x4000);
        mDoMtx_copy(mDoMtx_stack_c::get(), temp_r3->getRightStirrupMtx());
    }

    if (field_0x2fab & 3) {
        temp_r3->calcWeightEnvMtx();
    }

    int temp_r3_4 = getReinHandType();
    if (temp_r3_4 == 0) {
        temp_r3->setReinPosNormal();
    } else if (temp_r3_4 != -1) {
        temp_r3->setReinPosHand(temp_r3_4);
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
    e_wb_class* e_wb_p = (e_wb_class*)mRideAcKeep.getActor();
    if (e_wb_p == NULL) {
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
        if (e_wb_p == NULL || e_wb_p->checkDownDamage()) {
            boarForceGetOff();
            return 0;
        }

        if (!e_wb_p->checkNormalRideMode() && mProcID != PROC_BOAR_RUN) {
            procBoarRunInit();
            return 0;
        }
    }

    MtxP ride_mtx = e_wb_p->getRideMtx();
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
    e_wb_class* e_wb_p = (e_wb_class*)mRideAcKeep.getActor();
    if (e_wb_p == NULL || e_wb_p->checkDownDamage()) {
        boarForceGetOff();
        return 0;
    }

    if (e_wb_p->checkNormalRideMode()) {
        procHorseWaitInit();
        return 0;
    }

    MtxP ride_mtx = e_wb_p->getRideMtx();
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
        mUnderFrameCtrl[0].setFrame(e_wb_p->nowAnimeFrame());
        getNowAnmPackUnder(UNDER_0)->setFrame(e_wb_p->nowAnimeFrame());
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
    daHorse_c* temp_r3 = dComIfGp_getHorseActor();
    u16 temp_r3_2 = temp_r3->getAnmIdx(0);

    daAlink_ANM var_r31;
    if (temp_r3_2 == 0xC) {
        var_r31 = ANM_HORSE_JUMP_START;
    } else if (temp_r3_2 == 0xB) {
        var_r31 = ANM_HORSE_JUMP;
    } else if (temp_r3_2 == 0xA) {
        var_r31 = ANM_HORSE_JUMP_LAND;
    } else if (temp_r3_2 == 0x15) {
        var_r31 = ANM_HORSE_STOP;
    } else if (temp_r3_2 == 0x14) {
        var_r31 = ANM_HORSE_STAND;
    } else {
        var_r31 = ANM_HORSE_STOP_TO_STAND;
    }

    setSingleAnimeBaseMorf(var_r31, temp_r3->getMorfFrame());

    f32 temp_f1 = temp_r3->getAnmFrame(0);
    mUnderFrameCtrl[0].setFrame(temp_f1);
    getNowAnmPackUnder(UNDER_0)->setFrame(temp_f1);

    if (getNowAnmPackUnder(UNDER_0) != getNowAnmPackUpper(UPPER_0)) {
        mUpperFrameCtrl[0].setFrame(temp_f1);
        getNowAnmPackUpper(UPPER_0)->setFrame(temp_f1);
    }
}

void daAlink_c::getBaseHorseAnime(daAlink_c::daAlink_ANM* param_0) {
    daHorse_c* temp_r27 = dComIfGp_getHorseActor();

    if (checkHorseNoUpperAnime() || checkUpperHorseLieAnime()) {
        if (!dComIfGp_checkPlayerStatus0(0, 0x2000)) {
            if (checkShieldGet()) {
                setRStatus(0xB);
            } else {
                setRStatus(0x3E);
            }

            if (spActionButton()) {
                resetUpperAnime(UPPER_2, -1.0f);
                param_0[0] = ANM_HORSE_CROUCH;
                param_0[1] = ANM_HORSE_CROUCH;
                return;
            }
        }
    }

    for (int i = 0; i < 3; i++, param_0++) {
        u16 temp_r3_2 = temp_r27->getAnmIdx(i);
        if (temp_r3_2 == 7 || temp_r27->speedF < 0.0f) {
            if (i == 1) {
                param_0[0] = ANM_HORSE_WALK_B;
            } else {
                param_0[0] = ANM_HORSE_TURN_B;
            }
        } else if (temp_r3_2 == 0x12) {
            param_0[0] = ANM_HORSE_DASH_B;
        } else if (temp_r3_2 == 0x22) {
            param_0[0] = ANM_HORSE_WALK_B;
        } else if (temp_r3_2 == 0x11) {
            param_0[0] = ANM_HORSE_DASH_START;
        } else if (temp_r3_2 == 0x13) {
            if (checkHorseZeldaBowMode() ) {
                param_0[0] = ANM_HORSE_DASH_B;
            } else {
                param_0[0] = ANM_HORSE_DASH_A;
            }
        } else if (temp_r3_2 == 0x23) {
            param_0[0] = ANM_HORSE_WALK_A;
        } else if (temp_r3_2 == 0x17 || temp_r3_2 == 0x18) {
            if (i == 1) {
                param_0[0] = ANM_HORSE_WALK_B;
            } else {
                param_0[0] = ANM_HORSE_TURN_LEFT;
            }
        } else if (temp_r3_2 == 0x19 || temp_r3_2 == 0x1A) {
            if (i == 1) {
                param_0[0] = ANM_HORSE_WALK_B;
            } else {
                param_0[0] = ANM_HORSE_TURN_RIGHT;
            }
        } else if (temp_r3_2 == 0xFFFF && i == 1) {
            param_0[0] = param_0[-1];
        } else {
            param_0[0] = ANM_HORSE_WAIT;
        }
    }
}

int daAlink_c::checkHorseSpecialProc() {
    daHorse_c* horse_p = dComIfGp_getHorseActor();

    if (!checkHorseRide()) {
        return 0;
    }

    if (checkEndResetFlg2(ERFLG2_UNK_2)) {
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
    daPy_frameCtrl_c* sp24 = &mUnderFrameCtrl[0];
    daPy_frameCtrl_c* sp20 = &mUnderFrameCtrl[2];
    BOOL mode_1 = checkModeFlg(1) ? TRUE : FALSE;

    setSyncHorsePos();
    if (checkHorseSpecialProc()) {
        return 0;
    }

    daAlink_ANM sp8[3];
    getBaseHorseAnime(sp8);

    if (fabsf(horse_p->speedF) < 0.001f) {
        if (!mode_1) {
            field_0x3004 = 0;
        } else if (checkInputOnR() && horse_p->checkWait() && mProcID != PROC_HORSE_SUBJECTIVITY &&
                   mProcID != PROC_HORSE_CUT_TURN && sp8[0] != ANM_HORSE_TURN_LEFT &&
                   sp8[0] != ANM_HORSE_TURN_RIGHT && field_0x3004 == 0) {
            s32 dir = getDirectionFromShapeAngle();
            if (dir == DIR_FORWARD)
            {
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
        horse_p->getAimNeckAngleY() == 0 && sp8[0] != ANM_HORSE_TURN_LEFT &&
        sp8[0] != ANM_HORSE_TURN_RIGHT &&
        (field_0x2f8c == 10 &&
             (checkHorseNoUpperAnime() || checkEquipAnime() || checkUpperGuardAnime()) ||
         (mProcID == PROC_HORSE_CUT || mProcID == PROC_HORSE_CUT_TURN ||
          mProcID == PROC_HORSE_CUT_CHARGE_READY)))
    {
        if (checkCowGame()) {
            if (checkHorseNoUpperAnime()) {
                setDoStatus(40);

                if (doTrigger()) {
                    onResetFlg0(RFLG0_COW_GAME_LEASH);
                    voiceStart(Z2SE_AL_V_RUSH_HORSE);
                    field_0x30a6 = mpHIO->mHorse.m.mWhipWaitTime;
                    setHorseSwordUpAnime();
                    field_0x3002 = -mpHIO->mHorse.m.mWhipWaitTime;
                }
            }
        } else if (checkHorseDashAccept()) {
            setDoStatus(9);

            if (doTrigger()) {
                field_0x3004 = mpHIO->mHorse.m.mWalkOutProhibitionTime;
                daAlink_ANM anm;
                const daAlinkHIO_anm_c* anmparam_p = &mpHIO->mHorse.m.mWhipAnm;
                anm = ANM_HORSE_LASH;

                if (mProcID != PROC_HORSE_CUT && mProcID != PROC_HORSE_CUT_TURN &&
                    mProcID != PROC_HORSE_CUT_CHARGE_READY)
                {
                    setSingleAnimeParam(anm, anmparam_p);
                }

                setUnderAnimeParam(getMainBckData(anm)->m_underID, UNDER_2,
                                   &mpHIO->mHorse.m.mWhipAnm);
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
         sp24->getFrame() > mpHIO->mHorse.m.mHorseWalkStartFrame) ||
        (checkHorseWaitLashAnime() && sp20->getFrame() > 13.0f))
    {
        horse_p->onMoveAccept();
    }

    if (checkNoResetFlg1(FLG1_UNK_1000) &&
        ((checkHorseUnderDashStartAnime() && sp20->getFrame() > 13.0f) ||
         (checkHorseUnderLashAnime() && sp20->getFrame() > 13.0f)))
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

    BOOL var_r28 =
        checkAnmEnd(mUnderFrameCtrl) ||
        (checkHorseServiceWaitAnime() &&
         (!checkNoUpperAnime() || horse_p->getAimNeckAngleY() != 0 || !checkModeFlg(1)));

    if (((var_r28 || sp8[0] == ANM_HORSE_TURN_LEFT || sp8[0] == ANM_HORSE_TURN_RIGHT ||
          sp8[0] == ANM_HORSE_DASH_START ||
          (spActionButton() && !checkHorseWalkStartAnm() && !checkHorseWaitLashAnime())) &&
         mProcID != PROC_HORSE_CUT_TURN) ||
        field_0x2f8c == 10)
    {
        if ((var_r28 || field_0x3004 != mpHIO->mHorse.m.mWalkOutProhibitionTime) && field_0x3004 != 0) {
            field_0x3004--;
        }

        f32 var_f31;
        if (var_r28 && horse_p->getMorfFrame() < 4.0f) {
            var_f31 = 4.0f;
        } else {
            var_f31 = horse_p->getMorfFrame();
        }

        f32 var_f30;
        if (sp8[0] == ANM_HORSE_TURN_LEFT || sp8[0] == ANM_HORSE_TURN_B ||
            sp8[0] == ANM_HORSE_TURN_RIGHT)
        {
            var_f30 = 0.0f;
        } else {
            var_f30 = horse_p->getBlendRate();
        }

        if (sp8[0] == ANM_HORSE_DASH_START && !checkUnderMove0BckNoArc(ANM_HORSE_DASH_START)) {
            voiceStart(Z2SE_AL_V_RUSH_HORSE);
        }

        setDoubleAnime(var_f30, 1.0f, 1.0f, sp8[0], sp8[1], 10, var_f31);
        setBaseHorseAnimeFrame();
        if (var_r28) {
            setHorseSwordUp(0);
        }
    }

    return 1;
}

int daAlink_c::setSyncBoar(int param_0) {
    e_wb_class* e_wb_p = (e_wb_class*)mRideAcKeep.getActor();
    fopAc_ac_c* e_wb_actor = (fopAc_ac_c*)e_wb_p;
    BOOL mode_1 = checkModeFlg(1);

    if (!setSyncBoarPos()) {
        return 0;
    }

    if (fabsf(e_wb_actor->speedF) < 0.001f && mode_1 && checkInputOnR() && e_wb_p->checkWait() &&
        mProcID != PROC_HORSE_SUBJECTIVITY && field_0x3004 == 0)
    {
        int direction = getDirectionFromShapeAngle();
        if (direction == DIR_FORWARD || direction == DIR_BACKWARD) {
            field_0x3004 = mpHIO->mHorse.m.mWalkOutProhibitionTime;
            setSingleAnimeParam(ANM_WSTARTH, &mpHIO->mHorse.m.mWalkOutAnm);
        }
    }

    if (param_0 && e_wb_p->getWaitRollAngle() == 0 && checkHorseNoUpperAnime()) {
        setDoStatus(9);
        if (doTrigger()) {
            changeBoarRunRide();
        }
    }

    if ((checkAnmEnd(mUnderFrameCtrl) && mProcID != PROC_HORSE_CUT_TURN) ||
        (checkHorseServiceWaitAnime() && (!checkNoUpperAnime() || !checkModeFlg(1))))
    {
        setBaseBoarAnime();
    } else if (mUnderAnmHeap[0].getIdx() == 0x1B3) {
        if (mUnderFrameCtrl[0].checkPass(10.0f)) {
            changeBoarRunRide();
        }
    } else if (!checkHorseServiceWaitAnime() && mProcID != PROC_HORSE_CUT_TURN) {
        setBaseBoarAnime();
    }

    if (mUnderAnmHeap[0].getIdx() != 0x1B3) {
        if (e_wb_p->getWaitRollAngle() > 0) {
            setHorseTurnUpperAnime(FALSE);
        } else if (e_wb_p->getWaitRollAngle() < 0) {
            setHorseTurnUpperAnime(TRUE);
        }
    }

    if (checkHorseTurnAnime()) {
        if (e_wb_p->getWaitRollAngle() == 0) {
            resetUpperAnime(UPPER_2, 4.0f);
            field_0x33f0 = 0.0f;
        } else {
            field_0x33f0 = cLib_minMaxLimit(e_wb_p->getWaitRollAngle() * 0.002f, -1.0f, 1.0f);
            // supposed to be std::abs
            mNowAnmPackUpper[2].setRatio(fabsf(field_0x33f0));
        }
    } else {
        field_0x33f0 = 0.0f;
    }

    return 1;
}

int daAlink_c::setSyncRide(int param_0) {
    BOOL var_r31;
    if (param_0 && mMidnaTalkDelayTimer == 0 && dComIfGp_getDoStatus() == 0 &&
        (!checkHorseRide() || dComIfGp_getHorseActor()->getAnmIdx(0) != 7))
    {
        var_r31 = TRUE;
    } else {
        var_r31 = FALSE;
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
        return setSyncHorse(var_r31);
    } else {
        return setSyncBoar(var_r31);
    }
}

void daAlink_c::setBaseHorseAnimeFrame() {
    daHorse_c* temp_r29 = dComIfGp_getHorseActor();
    BOOL var_r27 = 0;

    f32 var_f31;
    if (mUnderAnmHeap[0].checkNoSetArcNo()) {
        u16 temp_r28 = mUnderAnmHeap[0].getIdx();
        if (temp_r28 == getMainBckData(ANM_HORSE_WAIT)->m_underID) {
            var_f31 = mUnderFrameCtrl[0].getFrame();
        } else if ((temp_r28 == getMainBckData(ANM_HORSE_TURN_B)->m_underID && temp_r29->getAnmIdx(1) == 6) || ((temp_r28 == getMainBckData(ANM_HORSE_TURN_LEFT)->m_underID || temp_r28 == getMainBckData(ANM_HORSE_TURN_RIGHT)->m_underID) && (temp_r29->getAnmIdx(0) == 0x17 || temp_r29->getAnmIdx(0) == 0x19))) {
            var_f31 = mUnderFrameCtrl[0].getEnd();
            mUnderFrameCtrl[0].onEndFlg();
            var_r27 = 1;
        } else {
            field_0x3460 = 0.0f;
            var_f31 = temp_r29->getAnmFrame(0);
        }
    } else {
        var_f31 = temp_r29->getAnmFrame(0);
    }

    getNowAnmPackUnder(UNDER_0)->setFrame(var_f31);
    mUnderFrameCtrl[0].setFrame(var_f31);

    if (var_r27) {
        getNowAnmPackUnder(UNDER_1)->setFrame(field_0x3460);
        simpleAnmPlay(getNowAnmPackUnder(UNDER_1));
        field_0x3460 = getNowAnmPackUnder(UNDER_1)->getFrame();
    } else if (temp_r29->getAnmIdx(1) == 0xFFFF) {
        getNowAnmPackUnder(UNDER_1)->setFrame(var_f31);
    } else {
        getNowAnmPackUnder(UNDER_1)->setFrame(temp_r29->getAnmFrame(1));
    }

    getNowAnmPackUpper(UPPER_0)->setFrame(var_f31);
    getNowAnmPackUpper(UPPER_1)->setFrame(getNowAnmPackUnder(UNDER_1)->getFrame());
}

void daAlink_c::setBaseBoarAnime() {
    e_wb_class* temp_r30 = (e_wb_class*)mRideAcKeep.getActor();

    f32 var_f31;
    if (checkEventRun() && fopAcM_getTalkEventPartner(this) == (fopAc_ac_c*)getMidnaActor()) {
        var_f31 = 0.0f;
    } else {
        var_f31 = temp_r30->rideSpeedRate();
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
        daAlink_ANM sp8[3];
        getBaseHorseAnime(sp8);
        setDoubleAnime(horse_p->getBlendRate(), 1.0f, 1.0f, sp8[0], sp8[1], 10,
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

void daAlink_c::setHorseSwordUp(int param_0) {
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
    check_y[2] = ride_actor_p->current.pos.y + l_autoUpHeight;

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
    procCoLargeDamageInit(-4, 1, 0, 0, NULL, 0);
}

void daAlink_c::horseGetOffEnd() {
    if (field_0x2fc0 == 0) {
        shape_angle.y -= 0x4000;
        setOldRootQuaternion(0, 0x4000, 0);
        mDoMtx_stack_c::YrotS(-0x4000);
    } else {
        shape_angle.y += 0x4000;
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
    if ((!checkGrabAnimeAndThrow() || mProcID == PROC_HORSE_GRAB_MOVE) &&
        checkNextActionFromButton())
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

// NONMATCHING - regalloc, equivalent
BOOL daAlink_c::checkHorseGetOff() {
    return (checkHorseRide() && dComIfGp_getHorseActor() != NULL &&
            dComIfGp_getHorseActor()->checkGetOff()) ||
           (checkBoarRide() && ((e_wb_class*)mRideAcKeep.getActor()) != NULL &&
            ((e_wb_class*)mRideAcKeep.getActor())->checkGetOff());
}

int daAlink_c::checkHorseGetOffAndSetDoStatus() {
    daHorse_c* temp_r29 = dComIfGp_getHorseActor();
    int var_r30 = 0;
    BOOL temp_r28 = checkHorseGetOff();

    if (!(fabsf(mRideAcKeep.getActor()->speedF) < 30.0f) || !setTalkStatus()) {
        if (checkHorseLieAnime()) {
            if (!checkSpecialHorseRide()) {
                setDoStatus(0x32);
            } else {
                setDoStatus(0x8C);
            }
        } else if ((checkHorseNoUpperAnime() || checkEquipAnime()) && mMidnaTalkDelayTimer == 0 && !checkInputOnR() && temp_r28) {
            if (mEquipItem == fpcNm_ITEM_NONE) {
                var_r30 = checkHorseGetOffDirection();
                if ((var_r30 != DIR_LEFT || (checkHorseRide() && temp_r29->checkEnemySearch())) && field_0x2fb4 == 0) {
                    if (!checkSpecialHorseRide()) {
                        setDoStatus(0x32);
                    } else {
                        setDoStatus(0x8C);
                    }
                }
            } else if (checkHorseRide() && !checkSpecialHorseRide() && field_0x2fb4 == 0 && temp_r29->checkEnemySearch()) {
                setDoStatus(0x32);
            } else {
                setDoStatus(4);
            }
        }
    }

    return var_r30;
}

int daAlink_c::setHorseGetOff(int param_0) {
    daHorse_c* horse_p = dComIfGp_getHorseActor();

    if (dComIfGp_getDoStatus() == 0x32 && doTrigger()) {
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
    BOOL var_r25 = 0;

    if (mEquipItem != fpcNm_ITEM_NONE) {
        if (mEquipItem == 0x103) {
            var_r25 = 1;
        }
        deleteEquipItem(TRUE, TRUE);
    }

    mRideAcKeep.setData(field_0x27f4);

    daAlink_ANM var_r29;
    MtxP var_r28;
    Vec* var_r27;
    u32 var_r24;
    if (fopAcM_GetName(field_0x27f4) == PROC_HORSE) {
        daHorse_c* temp_r26 = (daHorse_c*)field_0x27f4;
        temp_r26->onRideStartFlg();
        var_r28 = temp_r26->getRootMtx();
        mRideStatus = 1;
        var_r27 = &l_localHorseRidePos;
        field_0x384c = &l_horseBaseAnime;
        var_r24 = temp_r26->checkRodeoMode();
        mZ2Link.setRiding(true);
    } else {
        e_wb_class* temp_r23 = (e_wb_class*)field_0x27f4;
        temp_r23->setPlayerRideNow();
        var_r28 = temp_r23->getRideMtx();
        mRideStatus = 2;
        var_r27 = &l_localBoarRidePos;
        field_0x384c = &l_boarBaseAnime;
        var_r24 = 0;
    }

    commonProcInit(PROC_HORSE_RIDE);

    field_0x3588 = l_waitBaseAnime;
    f32 var_f30 = 1.0f;
    f32 var_f31;
    s16 var_r22 = -1;
    mProcVar2.field_0x300c = 0;
    setHeavyBoots(0);

    int temp_r21 = getReinRideDirection();
    if (var_r24 || temp_r21 == 1) {
        var_r29 = ANM_HORSE_MOUNT_BACK;
        field_0x2fc0 = 2;
        mProcVar2.field_0x300c = 1;
    } else {
        if (checkHorseRide() && ((daHorse_c*)field_0x27f4)->checkEnemySearch()) {
            var_f30 *= 1.2f;
            mProcVar2.field_0x300c = 1;
            var_r22 = 0x30;
        }
    
        if (temp_r21 == 2) {
            if (checkHorseRide()) {
                var_r29 = ANM_HORSE_MOUNT_LEFT;
            } else {
                var_r29 = ANM_HORSE_RIDE_LEFT;
            }
            field_0x2fc0 = 0;
        } else {
            if (checkHorseRide()) {
                var_r29 = ANM_HORSE_MOUNT_RIGHT;
            } else {
                var_r29 = ANM_HORSE_RIDE_RIGHT;
            }
            field_0x2fc0 = 1;
        }
    
        if (mDemo.getDemoMode() != 7) {
            setOldRootQuaternion(0, (s16)(shape_angle.y - field_0x27f4->shape_angle.y), 0);
        }
    }

    mNormalSpeed = 0.0f;

    cXyz sp2C;
    cXyz sp20;

    mDoMtx_multVec(var_r28, var_r27, &sp2C);
    mDoMtx_stack_c::transS(field_0x27f4->current.pos);
    mDoMtx_stack_c::YrotM(field_0x27f4->shape_angle.y);
    mDoMtx_stack_c::XrotM(-field_0x27f4->shape_angle.x);
    mDoMtx_stack_c::YrotM(-field_0x27f4->shape_angle.y);
    mDoMtx_stack_c::transM(-field_0x27f4->current.pos.x, -field_0x27f4->current.pos.y, -field_0x27f4->current.pos.z);
    mDoMtx_stack_c::concat(var_r28);
    mDoMtx_stack_c::multVec(var_r27, &sp20);

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

    setSingleAnime(var_r29, var_f30, 0.0f, var_r22, 4.0f);

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
    daPy_frameCtrl_c* temp_r29 = &mUnderFrameCtrl[0];

    fopAc_ac_c* temp_r3 = mRideAcKeep.getActor();
    if (temp_r3 == NULL) {
        return checkNextAction(0);
    }

    e_wb_class* sp8 = (e_wb_class*)temp_r3;
    daHorse_c* var_r26 = (daHorse_c*)temp_r3;
    
    field_0x2f99 = 7;
    current.pos += temp_r3->current.pos - field_0x37c8;
    field_0x37c8 = temp_r3->current.pos;

    if (field_0x2fc0 == 2) {
        if (temp_r29->getFrame() >= 25.0f) {
            field_0x2f93 = 5;
            field_0x2fab = 0x13;
        } else if (temp_r29->getFrame() >= 14.0f) {
            field_0x2f92 = 4;
        }

        if (var_r26->checkRodeoMode()) {
            if (temp_r29->getFrame() > 15.0f) {
                return procHorseRunInit();
            } else if (temp_r29->getFrame() >= 4.0f) {
                setSyncHorsePos();
            }
        } else if (temp_r29->getFrame() >= 4.0f && temp_r29->getFrame() < 8.0f) {
            current.pos.y -= field_0x3478 * (0.25f * temp_r29->getRate());
        } else if (temp_r29->getFrame() > 8.0f) {
            setSyncHorsePos();
            var_r26->onRideRunFlg();
            var_r26->onMoveAccept();
        }
    } else {
        if (temp_r29->getFrame() >= 46.0f) {
            field_0x2f92 = 4;

            if (!checkBoarRide() || (field_0x2fc0 == 0) || temp_r29->getFrame() >= 53.0f) {
                field_0x2f93 = 5;
                field_0x2fab = 0x13;
            } else {
                field_0x2fab = 3;
            }

            if (checkBoarRide()) {
                csXyz spC;
                mDoMtx_MtxToRot(sp8->getRideMtx(), &spC);
                cLib_addCalcAngleS(&shape_angle.x, -spC.z, 4, 2000, 400);
            } else {
                cLib_addCalcAngleS(&shape_angle.x, temp_r3->shape_angle.x, 4, 2000, 400);
            }
        } else {
            if (temp_r29->getFrame() >= 17.0f) {
                field_0x2fab |= (u8)(1 << field_0x2fc0);

                if (field_0x2fc0 == 0) {
                    if (temp_r29->getFrame() >= 41.0f && checkHorseRide()) {
                        field_0x2f92 = 4;
                    }
                } else if (temp_r29->getFrame() >= 38.0f && checkHorseRide()) {
                    field_0x2f93 = 5;
                    field_0x2fab |= 0x10;
                }
            }
        }

        if (temp_r29->getFrame() >= 11.0f && temp_r29->getFrame() < 16.0f) {
            current.pos.y -= field_0x3478 * (0.2f * temp_r29->getRate());
        }

        if (temp_r29->getFrame() >= 17.0f && checkHorseRide() && mProcVar2.field_0x300c != 0) {
            setSyncHorsePos();
            var_r26->onRideRunFlg();
            var_r26->onMoveAccept();
        }
    }

    if (checkAnmEnd(temp_r29)) {
        J3DTransformInfo* temp_r28 = field_0x2060->getOldFrameTransInfo(0);

        if (checkHorseRide()) {
            var_r26->onRideFlg();
    
            if (mProcVar2.field_0x300c == 0) {
                temp_r28->mTranslate.x += l_horseBaseAnime.x;
                temp_r28->mTranslate.y += l_horseBaseAnime.y;
                temp_r28->mTranslate.z += l_horseBaseAnime.z;
            }
        } else {
            sp8->setPlayerRide();
            temp_r28->mTranslate.x += l_boarBaseAnime.x;
            temp_r28->mTranslate.y += l_boarBaseAnime.y;
            temp_r28->mTranslate.z += l_boarBaseAnime.z;
        }

        field_0x2f92 = 4;
        field_0x2f93 = 5;
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
                var_r26->onPlayerBackRideLash();
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
    cXyz spC;
    commonProcInit(PROC_HORSE_GETOFF);
    resetUnderAnime(UNDER_2, -1.0f);

    if (mEquipItem != fpcNm_ITEM_NONE) {
        allUnequip(0);
    }

    fopAc_ac_c* temp_r30 = mRideAcKeep.getActor();
    if (checkHorseRide()) {
        ((daHorse_c*)temp_r30)->offRideFlg();
    } else {
        ((e_wb_class*)temp_r30)->setPlayerRideNow();
    }

    field_0x37d4 = temp_r30->current.pos;
    mProcVar3.field_0x300e = temp_r30->shape_angle.y;
    field_0x2fc0 = param_0;

    mDoMtx_stack_c::transS(temp_r30->current.pos.x, temp_r30->current.pos.y, temp_r30->current.pos.z);
    mDoMtx_stack_c::YrotM(temp_r30->shape_angle.y);

    daAlink_ANM var_r29;
    if (field_0x2fc0 == 0) {
        if (checkHorseRide()) {
            var_r29 = ANM_HORSE_GETOFF_LEFT;
        } else {
            var_r29 = ANM_HORSE_FALL_LEFT;
        }

        static Vec const leftOffset = {75.0f, 102.0f, 6.224f};
        mDoMtx_stack_c::multVec(&leftOffset, &spC);
    } else {
        if (checkHorseRide()) {
            var_r29 = ANM_HORSE_GETOFF_RIGHT;
        } else {
            var_r29 = ANM_HORSE_FALL_RIGHT;
        }

        static Vec const rightOffset = {-75.0f, 102.0f, 6.221f};
        mDoMtx_stack_c::multVec(&rightOffset, &spC);
    }

    setSingleAnimeBaseMorf(var_r29, 4.0f);

    mLinkGndChk.SetPos(&spC);

    spC.y = dComIfG_Bgsp().GroundCross(&mLinkGndChk);
    field_0x3478 = spC.y - temp_r30->current.pos.y;
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
    daPy_frameCtrl_c* temp_r29 = &mUnderFrameCtrl[0];

    fopAc_ac_c* temp_r3 = mRideAcKeep.getActor();
    if (temp_r3 != NULL) {
        current.pos += temp_r3->current.pos - field_0x37d4;
        field_0x37d4 = temp_r3->current.pos;
        shape_angle.y += (s16)(mProcVar3.field_0x300e - temp_r3->shape_angle.y);
        mProcVar3.field_0x300e = temp_r3->shape_angle.y;
    }

    cLib_addCalcAngleS(&shape_angle.z, 0, 4, 2000, 400);

    if (temp_r29->getFrame() >= 35.0f) {
        field_0x2fab = 0;
    } else if (checkHorseRide()) {
        if (temp_r29->getFrame() >= 14.0f) {
            field_0x2fab &= ~(1 << field_0x2fc0);
        } else if (temp_r29->getFrame() >= 4.0f) {
            if (field_0x2fc0 == 0) {
                field_0x2fab &= ~0x2;
            } else {
                field_0x2fab &= (u8)~0x1;
            }
        }
    } else if (!(temp_r29->getFrame() >= 24.0f)) {
        if (temp_r29->getFrame() >= 21.0f) {
            field_0x2fab &= ~(1 << field_0x2fc0);
        } else if (temp_r29->getFrame() >= 3.0f) {
            if (field_0x2fc0 == 0) {
                field_0x2fab &= ~0x2;
            } else {
                field_0x2fab &= (u8)~0x1;
            }
        }
    }

    if (checkAnmEnd(temp_r29)) {
        horseGetOffEnd();
        checkNextAction(0);
    } else if (temp_r29->getFrame() >= 32.0f) {
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

        if (temp_r29->getFrame() < 27.0f) {
            f32 temp_f31 = temp_r29->getFrame() / 27.0f;
            field_0x3588.x = (l_waitBaseAnime.x * (1.0f - temp_f31)) + (field_0x37c8.x * temp_f31);
            field_0x3588.z = (l_waitBaseAnime.z * (1.0f - temp_f31)) + (field_0x37c8.z * temp_f31);
        } else if (temp_r29->getFrame() < 32.0f) {
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
        setUnderAnime(0x26E, UNDER_2, 1.0f, 0.0f, -1, -1.0f);
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

    daHorse_c* temp_r26 = (daHorse_c*)mRideAcKeep.getActor();
    if (checkHorseRide() && temp_r26->checkRodeoMode() && !checkEventRun()) {
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
            fopAc_ac_c* temp_r3 = (fopAc_ac_c*)fopAcIt_Judge((fopAcIt_JudgeFunc)daAlink_searchBoar, NULL);
            if (temp_r3 != NULL) {
                int temp_r28 = abs((s16)(cLib_targetAngleY(&temp_r3->current.pos, &current.pos) - temp_r3->shape_angle.y));
                f32 temp_f31 = temp_r3->current.pos.abs2XZ(current.pos);

                if (temp_r28 > 0x800 && temp_r28 < 0x5000 && temp_f31 < 360000.0f) {
                    setBStatus(0x2E);
                } else if (checkEndResetFlg0(ERFLG0_SINGLE_BOAR_AVOID)) {
                    set3DStatus(0x2D, 5);
                    setBStatus(0x2E);
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
    daHorse_c* temp_r29 = dComIfGp_getHorseActor();
    if (!setSyncRidePos()) {
        return 1;
    }

    if (mUnderFrameCtrl[0].getFrame() < 13.0f && mEquipItem == 0x103 && !checkBootsOrArmorHeavy() && checkCutTurnInputTrigger()) {
        temp_r29->onCutTurnCancel();
        return procHorseCutTurnInit();
    }

    if (!temp_r29->checkStop() && !temp_r29->checkTurn()) {
        mUnderFrameCtrl[0].onEndFlg();
        procHorseWaitInit();
        field_0x30ca = 0;
    } else if (temp_r29->checkTurnCancelFrame() || temp_r29->checkStopCancelFrame()) {
        onModeFlg(4);
        temp_r29->onTurnCancelKeep();

        if (checkHorseNoUpperAnime()) {
            if (checkShieldGet()) {
                setRStatus(0xB);
            } else {
                setRStatus(0x3E);
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
                temp_r29->offCutTurnCancel();
            }
            return 1;
        }

        temp_r29->offTurnCancelKeep();
    } else {
        if (checkBootsOrArmorHeavy() && !checkEventRun() && !checkBoarSingleBattle()) {
            if (temp_r29->getAnmFrame(0) >= 30.0f) {
                return procCoLargeDamageInit(-4, 1, 0, 0, NULL, 0);
            }
        } else if (spActionButton() && !checkSpecialHorseRide()) {
            setDoStatus(0x32);

            if (doTrigger()) {
                resetUpperAnime(UPPER_2, -1.0f);

                if (temp_r29->checkTurnStand()) {
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
    s16 old_shape_angle = shape_angle.y;
    s16 old_angle = current.angle.y;
    int old_3190 = field_0x3190;

    if (!setSyncRide(0)) {
        return 1;
    }

    shape_angle.y = old_shape_angle;
    if (checkSubjectEnd(1)) {
        procHorseWaitInit();
    } else if (checkNextActionFromButton()) {
        if (checkUpperReadyThrowAnime()) {
            field_0x3190 = old_3190;
            mBodyAngle.y = field_0x3190;
            field_0x310a = mBodyAngle.x;
            field_0x310c = shape_angle.y + field_0x3190;
        }
        return 1;
    } else {
        setRideSubjectAngle(old_angle);
    }

    return 1;
}

int daAlink_c::procHorseCutInit() {
    commonProcInit(PROC_HORSE_CUT);
    BOOL var_r26 = 0;

    if (checkHorseUnderItemAnime()) {
        resetUnderAnime(UNDER_2, -1.0f);
    }

    setSyncRide(1);

    fopAc_ac_c* var_r25;
    if (mTargetedActor != NULL) {
        var_r25 = mTargetedActor;
    } else {
        var_r25 = mAttention->LockonTarget(0);
    }

    if (var_r25 != NULL) {
        s16 sp8 = (s16)(cLib_targetAngleY(&current.pos, &var_r25->eyePos) - shape_angle.y);
        if (sp8 > 0x800) {
            var_r26 = 2;
        } else if (sp8 < -0x800) {
            var_r26 = 3;
        }
    }

    int var_r28;
    if (mComboCutCount == 1) {
        if (var_r26 == 3) {
            var_r28 = 2;
        } else {
            var_r28 = 0;
        }
    } else if (var_r26 == 3) {
        if (cM_rnd() < 0.5f) {
            var_r28 = 2;
        } else {
            var_r28 = 3;
        }
    } else if (var_r26 == 2) {
        if (cM_rnd() < 0.5f) {
            var_r28 = 0;
        } else {
            var_r28 = 1;
        }
    } else {
        var_r28 = (int)cM_rndF(4.0f) % 4;
    }

    static daAlink_cutHorseParamTbl const cutParamTable[] = {
        {
            daAlink_c::FTANM_UNK_20,
            0x91,
            0x10C,
            0x00,
            0x0E,
            0x22,
        },
        {
            daAlink_c::FTANM_UNK_21,
            0x92,
            0x10D,
            0x00,
            0x13,
            0x23,
        },
        {
            daAlink_c::FTANM_UNK_22,
            0x93,
            0x10E,
            0x00,
            0x13,
            0x2A,
        },
        {
            daAlink_c::FTANM_UNK_23,
            0x94,
            0x10F,
            0x00,
            0x00,
            0x2A,
        },
    };

    const daAlinkHIO_hoCut_c1* var_r29;
    const daAlink_cutHorseParamTbl* temp_r27 = &cutParamTable[var_r28];
    field_0x2f96 = 2;
    
    if (var_r28 == 0) {
        var_r29 = &daAlinkHIO_hoCutLA_c0::m;
    } else if (var_r28 == 1) {
        var_r29 = &daAlinkHIO_hoCutLB_c0::m;
    } else if (var_r28 == 2) {
        var_r29 = &daAlinkHIO_hoCutRA_c0::m;
    } else {
        var_r29 = &daAlinkHIO_hoCutRB_c0::m;
    }

    setUpperAnimeParam(temp_r27->m_upperAnm, UPPER_2, &var_r29->mCutAnm);
    setFacePriTexture(temp_r27->m_faceanm);
    setFacePriBck(temp_r27->m_faceBtk);
    setCutType(temp_r27->m_cutType);

    field_0x3478 = var_r29->mAttackStartFrame;
    field_0x347c = var_r29->mAttackEndFrame;
    field_0x3480 = var_r29->mAfterCutMorf;
    field_0x3484 = var_r29->mCutAnm.mCancelFrame;

    if (checkBoarSingleBattle()) {
        setSwordAtParam(dCcG_At_Spl_UNK_0, 1, 1, 2, mpHIO->mCut.m.mSwordLengthHorsebackFight, mpHIO->mCut.m.mSwordRadiusHorsebackFight);
    } else {
        setSwordAtParam(dCcG_At_Spl_UNK_0, 1, 1, 2, mpHIO->mCut.m.mSwordLengthHorseback, mpHIO->mCut.m.mSwordRadiusHorseback);
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
    daPy_frameCtrl_c* temp_r30 = &mUpperFrameCtrl[2];

    if (setSyncRide(1) == 0) {
        return 1;
    }

    setComboReserb();
    checkCutTurnCharge();

    if (checkAnmEnd(temp_r30)) {
        if (checkNoResetFlg2(FLG2_UNK_2)) {
            checkCutAction();
        } else {
            resetUpperAnime(UPPER_2, 4.0f);
            procHorseWaitInit();
            field_0x2060->initOldFrameMorf(field_0x3480, 0, 0x23);
        }
    } else if (temp_r30->getFrame() > field_0x3484 && (checkNoResetFlg2(FLG2_UNK_2) || checkResetFlg0(RFLG0_UNK_40) || spActionButton())) {
        if (checkResetFlg0(RFLG0_UNK_40)) {
            procHorseCutChargeReadyInit();
        } else if (checkNoResetFlg2(FLG2_UNK_2)) {
            checkCutAction();
        } else {
            if (checkShieldGet()) {
                setRStatus(0xB);
            } else {
                setRStatus(0x3E);
            }

            resetUpperAnime(UPPER_2, -1.0f);
            setSingleAnimeBase(ANM_HORSE_CROUCH);
            procHorseWaitInit();
        }
    } else {
        if (temp_r30->getFrame() > field_0x3484) {
            if (checkShieldGet()) {
                setRStatus(0xB);
            } else {
                setRStatus(0x3E);
            }
            resetCombo(1);
        }
        
        if (temp_r30->getFrame() >= field_0x3478 && temp_r30->getFrame() < field_0x347c) {
            if (!checkNoResetFlg0(FLG0_UNK_40)) {
                seStartSwordCut(Z2SE_AL_SWORD_SWING_S);
            }
            onResetFlg0(RFLG0_UNK_2);
        }
    }

    return 1;
}

int daAlink_c::procHorseCutChargeReadyInit() {
    commonProcInit(PROC_HORSE_CUT_CHARGE_READY);
    setUpperAnimeParam(0x96, UPPER_2, &mpHIO->mCut.mHorseCutCharge.m.mChargeAnm);
    setFacePriBck(0xE8);
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
    daPy_frameCtrl_c* temp_r30 = &mUpperFrameCtrl[2];

    if (!setSyncRide(1)) {
        return 1;
    }

    setSwordChargeVoiceSe();
    BOOL var_r30 = 0;

    if (checkAnmEnd(temp_r30)) {
        if (!checkWoodSwordEquip()) {
            simpleAnmPlay(m_nSwordBtk);
        } else {
            mProcVar0.field_0x3008++;

            if (mProcVar0.field_0x3008 >= 14.0f) {
                mProcVar0.field_0x3008 = 100;
            }
        }

        if (m_nSwordBtk->getFrame() >= 14.0f || mProcVar0.field_0x3008 >= 14.0f) {
            var_r30 = 1;
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
    setCutType(43);
    setSyncRide(1);
    field_0x3478 = mpHIO->mCut.mHorseCutCharge.m.mSpinAttackRadius;
    setCylAtParam(getSwordAtType(), dCcG_At_Spl_UNK_1, 3, 1, 3, field_0x3478 * 0.5f, 200.0f);
    mProcVar4.field_0x3010 = mpHIO->mCut.mHorseCutCharge.m.mSpinStopTime;
    voiceStart(Z2SE_AL_V_KAITEN);
    setCutWaterDropEffect();
    setHorseZeldaDamage();
    return 1;
}

int daAlink_c::procHorseCutTurn() {
    daPy_frameCtrl_c* temp_r30 = &mUnderFrameCtrl[0];

    if (!setSyncRide(1)) {
        return 1;
    }

    if (checkAnmEnd(temp_r30)) {
        procHorseWaitInit();
        field_0x2060->initOldFrameMorf(4.0f, 0, 0x23);
    } else {
        if (temp_r30->getFrame() > mpHIO->mCut.mHorseCutCharge.m.mSpinAnm.mCancelFrame) {
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

        if (temp_r30->getFrame() >= mpHIO->mCut.mHorseCutCharge.m.mSpinAttackStartFrame && temp_r30->getFrame() < mpHIO->mCut.mHorseCutCharge.m.mSpinAttackEndFrame) {
            if (!checkNoResetFlg0(FLG0_UNK_40)) {
                seStartSwordCut(Z2SE_AL_KAITENGIRI);
            }

            onResetFlg0(RFLG0_UNK_2);
            cLib_chaseF(mAtCyl.GetRP(), field_0x3478, 20.0f);
        }
    }

    return 1;
}

int daAlink_c::procHorseDamageInit(dCcD_GObjInf* param_0) {
    commonProcInit(PROC_HORSE_DAMAGE);

    if (checkHorseRide()) {
        resetUnderAnime(UNDER_2, -1.0f);
    }

    setSyncRide(0);

    static u16 const anmIdx[] = {
        0x00BA,
        0x00B8,
        0x00BB,
        0x00BC,
        0x00B9,
    };

    if (checkUnderMove0BckNoArc(ANM_HORSE_STOP) || checkUnderMove0BckNoArc(ANM_HORSE_STOP_TO_STAND) || checkUnderMove0BckNoArc(ANM_HORSE_STAND) || checkUnderMove0BckNoArc(ANM_HORSE_DASH_B) || checkUnderMove0BckNoArc(ANM_HORSE_DASH_A) || checkUnderMove0BckNoArc(ANM_HORSE_DASH_START)) {
        mProcVar4.field_0x3010 = 0;
        mProcVar5.field_0x3012 = 0;
        setUpperAnimeParam(anmIdx[4], UPPER_2, &mpHIO->mDamage.mDamHorse.m.mNoDirectionAnm);
    } else {
        cXyz* temp_r29 = getDamageVec(param_0);
        
        f32 temp_f31 = cM_ssin(shape_angle.y);
        f32 temp_f1 = cM_scos(shape_angle.y);
        cXyz spC((temp_r29->z * -temp_f31) + (temp_r29->x * temp_f1), temp_r29->y, (temp_r29->z * temp_f1) + (temp_r29->x * temp_f31));

        mProcVar4.field_0x3010 = cLib_minMaxLimit<s16>(cM_atan2s(spC.z, spC.y), -mpHIO->mDamage.mDamHorse.m.mFrontBackBodyMaxAngle, mpHIO->mDamage.mDamHorse.m.mFrontBackBodyMaxAngle);
        mProcVar5.field_0x3012 = cLib_minMaxLimit<s16>(cM_atan2s(spC.x, -JMAFastSqrt((spC.y * spC.y) + (spC.z * spC.z))), -mpHIO->mDamage.mDamHorse.m.mLeftRightBodyMaxAngle, mpHIO->mDamage.mDamHorse.m.mLeftRightBodyMaxAngle);
        
        int direction = getDirectionFromAngle(cM_atan2s(-spC.x, -spC.z));
        setUpperAnimeParam(anmIdx[direction], UPPER_2, &mpHIO->mDamage.mDamHorse.m.mWithDirectionAnm);
    }

    setFacePriBck(0xEA);
    setFacePriTexture((daAlink_c::daAlink_FTANM) 0x27);
    voiceStart(Z2SE_AL_V_DAMAGE_S);
    seStartOnlyReverb(Z2SE_AL_DAMAGE_NORMAL);
    
    daPy_frameCtrl_c* temp_r28 = &mUpperFrameCtrl[2];
    field_0x347c = M_PI / (temp_r28->getEnd() - temp_r28->getStart());
    setHorseZeldaDamage();
    onNoResetFlg1(FLG1_UNK_10000000);
    return 1;
}

int daAlink_c::procHorseDamage() {
    if (!setSyncRide(0)) {
        return 1;
    }

    daPy_frameCtrl_c* temp_r30 = &mUpperFrameCtrl[2];
    f32 temp_f1 = cM_fsin(field_0x347c * (temp_r30->getFrame() - temp_r30->getStart()));

    mBodyAngle.x = mProcVar4.field_0x3010 * temp_f1;
    mBodyAngle.z = -mProcVar5.field_0x3012 * temp_f1;
    mBodyAngle.y = 0;

    if (checkAnmEnd(temp_r30)) {
        resetUpperAnime(UPPER_2, 4.0f);
        procHorseWaitInit();
    }
    return 1;
}

int daAlink_c::procHorseBowSubjectInit() {
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
    setDoStatus(0x12);

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
    setDoStatus(0x12);

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
        setDoStatus(0x12);
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
        setDoStatus(0x12);
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
    setHookshotReadyMaterial();
    dComIfGp_setPlayerStatus0(0, 0x4000);
    return 1;
}

int daAlink_c::procHorseHookshotSubject() {
    if (checkHookshotWait()) {
        setDoStatus(0x12);
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
        setDoStatus(0x12);
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

int daAlink_c::procHorseBottleDrinkInit(u16 param_0) {
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
        daHorse_c* temp_r3 = dComIfGp_getHorseActor();
        temp_r3->changeOriginalDemo();
        temp_r3->changeDemoMode(0xE, 0);
    } else {
        mRideAcKeep.getActor()->speedF = 0.0f;
    }

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

int daAlink_c::procHorseBottleDrink() {
    dComIfGp_getHorseActor();

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
        daHorse_c* temp_r3 = dComIfGp_getHorseActor();
        temp_r3->changeOriginalDemo();
        temp_r3->changeDemoMode(1, 0);
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
        field_0x2fab |= 2;
    } else if (frameCtrl_p->getFrame() >= 75.0f) {
        field_0x2f93 = 5;
        field_0x2fab |= 0x10;
    } else if (frameCtrl_p->getFrame() >= 68.0f) {
        field_0x2fab |= 1;
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
    daHorse_c* temp_r29 = dComIfGp_getHorseActor();
    if (!setSyncHorsePos()) {
        mDoAud_changeSubBgmStatus(1);
        return 1;
    }

    if (field_0x32cc != 0) {
        onSceneChangeArea(17, 0xFF, NULL);
        return 1;
    }

    if (temp_r29->speedF > 0.0f) {
        if (mProcVar3.field_0x300e != 0) {
            mProcVar3.field_0x300e--;
        }

        if (temp_r29->checkRodeoLeft()) {
            if (mProcVar3.field_0x300e != 0) {
                mProcVar5.field_0x3012 -= 150;
            } else {
                mProcVar5.field_0x3012 -= 600;
            }

            if (mProcVar2.field_0x300c == 0) {
                set3DStatus(0x7A, 4);
            }
        } else {
            if (mProcVar3.field_0x300e != 0) {
                mProcVar5.field_0x3012 += 150;
            } else {
                mProcVar5.field_0x3012 += 600;
            }

            if (mProcVar2.field_0x300c == 0) {
                set3DStatus(0x7A, 1);
            }
        }

        if (checkInputOnR()) {
            int temp_r3 = getDirectionFromAngle(mStickAngle);
            s16 temp_r26 = mProcVar5.field_0x3012;

            if (temp_r3 == DIR_LEFT && !temp_r29->checkRodeoLeft()) {
                mProcVar5.field_0x3012 -= (600.0f + (256.0f * field_0x33a8));
            } else if (temp_r3 == DIR_RIGHT && temp_r29->checkRodeoLeft()) {
                mProcVar5.field_0x3012 += (600.0f + (256.0f * field_0x33a8));
            }

            if (temp_r26 * mProcVar5.field_0x3012 <= 0) {
                mProcVar5.field_0x3012 = 0;
            }
        }

        daPy_frameCtrl_c* temp_r28 = &mUnderFrameCtrl[0];
        f32 temp_f31 = temp_r28->getFrame() / temp_r28->getEnd();
    
        mProcVar4.field_0x3010 = (field_0x3478 * (1.0f + cM_fsin((M_PI*2) * (temp_f31 - 0.69999999f)))) + (4000.0f * (1.0f - fabsf(0.000099999997f * mProcVar5.field_0x3012)));
        field_0x3088 = field_0x3478 * (1.0f + cM_fsin((M_PI*2) * (temp_f31 - 1.0f)));
    
        if (temp_r28->checkPass(0.0f)) {
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
    } else if (temp_r29->getRodeoPointCnt() && temp_r29->checkTurnStand()) {
        setDoStatus(0x6F);

        if (doTrigger()) {
            field_0x32cc = 1;
            onSceneChangeArea(17, 0xFF, 0);
            temp_r29->offRodeoMode();
        } else {
            mProcVar2.field_0x300c = 1;
        }
    }

    return 1;
}

int daAlink_c::procHorseHangInit(dCcD_GObjInf* param_0, int param_1) {
    if (checkBoarRide()) {
        return procHorseDamageInit(param_0);
    }

    commonProcInit(PROC_HORSE_HANG);

    daAlink_ANM var_r30;
    if ((s16)(getDamageVec(param_0)->atan2sX_Z() - shape_angle.y) > 0) {
        var_r30 = ANM_HORSE_DMG_LEFT;
        field_0x32cc = 0x58;
        mProcVar4.field_0x3010 = 1;
        mProcVar5.field_0x3012 = 2;
    } else {
        var_r30 = ANM_HORSE_DMG_RIGHT;
        field_0x32cc = 0x59;
        mProcVar4.field_0x3010 = 2;
        mProcVar5.field_0x3012 = 1;
    }

    field_0x2fab &= ~mProcVar5.field_0x3012;
    setSingleAnimeBase(var_r30);
    mProcVar3.field_0x300e = 0;
    field_0x3004 = 0;
    field_0x3002 = 0;
    resetUnderAnime(UNDER_2, -1.0f);

    setSyncHorsePos();
    setHorseZeldaDamage();

    if (param_1) {
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
            field_0x2fab |= 3;
        } else {
            boarForceGetOff();
        }

        return 1;
    }

    daPy_frameCtrl_c* temp_r30 = &mUnderFrameCtrl[0];

    if (checkAnmEnd(temp_r30)) {
        if (mProcVar3.field_0x300e == 0) {
            mProcVar3.field_0x300e = 1;
            voiceStart(Z2SE_AL_V_CLING_HORSE);
            setSingleAnimeBase((daAlink_c::daAlink_ANM)field_0x32cc);
        } else {
            checkNextActionHorse();
        }
    } else if (mProcVar3.field_0x300e == 0) {
        if (temp_r30->checkPass(7.0f)) {
            field_0x2fab &= ~mProcVar4.field_0x3010;
        } else if (temp_r30->checkPass(36.0f)) {
            field_0x2fab |= mProcVar4.field_0x3010;
        }
    } else if (temp_r30->checkPass(7.0f)) {
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
        field_0x2f92 = 254;
        field_0x2f93 = 254;
        mProcVar2.field_0x300c = 1;
    } else if (frameCtrl_p->getFrame() >= 12.0f) {
        field_0x2f92 = 2;
    } else if (frameCtrl_p->getFrame() >= 5.0f) {
        field_0x2f92 = 3;
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
    e_wb_class* temp_r30 = (e_wb_class*)mRideAcKeep.getActor();
    fopAc_ac_c* temp_r29 = (fopAc_ac_c*)temp_r30;

    if (mProcVar3.field_0x300e == 0 && checkAnmEnd(&mUnderFrameCtrl[0])) {
        setSingleAnimeBaseSpeed(ANM_HORSE_TAME_WAIT_B, 0.0f, -1.0f);
        mProcVar3.field_0x300e = 1;
    }

    if ((mProcVar3.field_0x300e != 0 && !setSyncBoarRunPos()) || (mProcVar3.field_0x300e == 0 && !setSyncBoarPos())) {
        return 1;
    }

    if (temp_r29->speedF > 0.0f && mProcVar3.field_0x300e != 0) {
        mProcVar4.field_0x3010 = (field_0x3478 * (1.0f + cM_fsin((M_PI*2) * (temp_r30->getAnimeFrameRate() - 0.2f)))) - 2000.0f;
        field_0x3088 = field_0x3478 * (1.0f + cM_fsin((M_PI*2) * (temp_r30->getAnimeFrameRate() - 0.4f)));
        
        if (temp_r30->checkAnmLoopFrame()) {
            field_0x3478 = 3000.0f - cM_rndF(1500.0f);
        }
    } else if (mProcVar2.field_0x300c == 0) {
        if (mUnderFrameCtrl[0].getFrame() >= 14.0f) {
            field_0x2f92 = 0xFE;
            field_0x2f93 = 0xFE;
            mProcVar2.field_0x300c = 1;
        }
    }

    return 1;
}
