/**
 * d_a_alink_demo.inc
 * Player Cutscene / Event action handling
 */

#include "d/actor/d_a_alink.h"
#include "d/d_com_inf_game.h"
#include "d/d_demo.h"
#include "d/d_gameover.h"
#include "f_op/f_op_msg_mng.h"
#include "f_op/f_op_overlap_mng.h"
#include "d/actor/d_a_kytag04.h"
#include "d/actor/d_a_tag_mhint.h"
#include "d/actor/d_a_tag_mist.h"
#include "d/actor/d_a_tag_mstop.h"
#include "d/actor/d_a_tag_mwait.h"
#include "d/actor/d_a_canoe.h"
#include "d/actor/d_a_horse.h"
#include "d/actor/d_a_midna.h"
#include "d/actor/d_a_e_wb.h"
#include "d/actor/d_a_npc_zra.h"
#include "d/actor/d_a_obj_tks.h"
#include "d/actor/d_a_npc_tkc.h"

BOOL daAlink_c::checkEventRun() const {
    return dComIfGp_event_runCheck() || checkPlayerDemoMode();
}

void daAlink_c::createNpcTks(cXyz* i_basePos, int i_roomNo, u32 i_parameters) {
    cXyz pos(i_basePos->x + cM_ssin(shape_angle.y) * 200.0f, i_basePos->y,
             i_basePos->z + cM_scos(shape_angle.y) * 200.0f);
    csXyz angle(0, shape_angle.y + 0x8000, 0);
    fopAcM_create(PROC_NPC_TKS, i_parameters, &pos, i_roomNo, &angle, NULL, -1);
}


bool daAlink_c::checkSetNpcTks(cXyz* i_basePos, int i_roomNo, int param_2) {
    u32 scene_mode = getLastSceneMode();

    if (param_2 != 0 && checkDungeon() &&
        dStage_stagInfo_GetSaveTbl(dComIfGp_getStage()->getStagInfo()) != 0x17 &&
        dStage_stagInfo_GetSaveTbl(dComIfGp_getStage()->getStagInfo()) != 0x18 &&
        (checkItemGet(fpcNm_ITEM_TKS_LETTER, 1) ||
         (checkItemGet(fpcNm_ITEM_DUNGEON_BACK, 1) && !checkLv7DungeonShop())) &&
        dComIfGs_isDungeonItemWarp())
    {
        if (!dComIfGs_isStageBossEnemy()) {
            cXyz pos;
            csXyz angle;

            u32 parameters = 5;
            if (checkStageName("D_MN01")) {
                daNpcF_getPlayerInfoFromPlayerList(1, 0, pos, angle);
                i_basePos = &pos;
                if (scene_mode != 0xC) {
                    parameters = 8;
                }
            } else if ((checkStageName("D_MN06") != 0 || checkStageName("D_MN05")) &&
                       scene_mode != 0xC)
            {
                parameters = 9;
            }

            createNpcTks(i_basePos, i_roomNo, parameters);
            return true;
        }
    }

    return false;
}

int daAlink_c::checkDemoAction() {
    if (dComIfGp_getEvent().isOrderOK()) {
        if (!dComIfGp_event_runCheck()) {
            offNoResetFlg0(FLG0_UNK_200000);
        }
        return 0;
    }

    u32 demoMode = mDemo.getDemoMode();
    JUT_ASSERT(155,
               (demoMode < daPy_demo_c::DEMO_LAST_e) || (demoMode == daPy_demo_c::DEMO_NEW_ANM0_e));

    if (demoMode != daPy_demo_c::DEMO_UNK_6_e && demoMode != daPy_demo_c::DEMO_UNK_8_e) {
        offNoResetFlg0(FLG0_UNK_400);
    }

    if (demoMode != daPy_demo_c::DEMO_SUMOU_SHIKO_e && mSpecialMode == 1) {
        return procSumouReadyInit();
    }

    if (mProcID == PROC_WOLF_GIANT_PUZZLE) {
        return 0;
    }

    if (!checkModeFlg(0x400) &&
        demoMode != daPy_demo_c::DEMO_NEW_ANM0_e &&
        demoMode != daPy_demo_c::DEMO_METAMORPHOSE_ONLY_UNK1_e &&
        demoMode != daPy_demo_c::DEMO_METAMORPHOSE_ONLY_UNK2_e &&
        demoMode != daPy_demo_c::DEMO_METAMORPHOSE_UNK1_e &&
        demoMode != daPy_demo_c::DEMO_METAMORPHOSE_UNK2_e &&
        demoMode != daPy_demo_c::DEMO_UNK_9_e &&
        demoMode != daPy_demo_c::DEMO_UNK_17_e &&
        demoMode != daPy_demo_c::DEMO_UNK_24_e &&
        demoMode != daPy_demo_c::DEMO_UNK_7_e &&
        demoMode != daPy_demo_c::DEMO_MONKEY_MOVE_e &&
        demoMode != daPy_demo_c::DEMO_UNK_4_e &&
        demoMode != daPy_demo_c::DEMO_WOLF_CARGO_CARRY_e &&
        demoMode != daPy_demo_c::DEMO_ZORA_MOVE_e &&
        demoMode != daPy_demo_c::DEMO_OPEN_TREASURE_e &&
        demoMode != daPy_demo_c::DEMO_UNK_45_e &&
        demoMode != daPy_demo_c::DEMO_DOOR_OPEN_e &&
        demoMode != daPy_demo_c::DEMO_FM_CHAIN_STRONG_PULL_e &&
        demoMode != daPy_demo_c::DEMO_FOG_DEAD_e &&
        (demoMode != daPy_demo_c::DEMO_GET_ITEM_e || !checkModeFlg(0x10040)) &&
        !dComIfGp_checkPlayerStatus0(0, 0x100000) &&
        !checkFlyAtnWait() &&
        ((!mLinkAcch.ChkGroundHit() && !checkMagneBootsOn()) || checkModeFlg(0x70C52)))
    {
        return 0;
    }

    if (checkSpinnerRide()) {
        current.pos.x = field_0x37a4.x;
        current.pos.z = field_0x37a4.z;
    }

    if (demoMode != daPy_demo_c::DEMO_NEW_ANM0_e) {
        endHighModel();
    }

    if (demoMode == daPy_demo_c::DEMO_NEW_ANM0_e) {
        onNoResetFlg0(FLG0_UNK_100000);
        return procCoToolDemoInit();
    }

    if (m_demoInitTable[demoMode] != NULL) {
        onNoResetFlg0(FLG0_UNK_100000);
        return (this->*m_demoInitTable[demoMode])();
    }

    if (demoMode == daPy_demo_c::DEMO_UNK_6_e || demoMode == daPy_demo_c::DEMO_UNK_8_e) {
        if (mDemo.getParam1() != 1 && !checkNoResetFlg0(FLG0_UNK_400)) {
            seStartSystem(Z2SE_SY_TALK_START);
            onNoResetFlg0(FLG0_UNK_400);
        }

        if (mProcID == PROC_SPINNER_WAIT) {
            return procFallInit(1, mpHIO->mAutoJump.m.mFallInterpolation);
        }

        if (checkModeFlg(0x4000000) || checkModeFlg(0x40000) || checkHorseRide() ||
            checkBoarRide() || checkCanoeRide())
        {
            if (demoMode == daPy_demo_c::DEMO_UNK_6_e && (checkHorseRide() || checkBoarRide())) {
                if (mEquipItem != fpcNm_ITEM_NONE && mEquipItem != fpcNm_ITEM_KANTERA && !checkEquipAnime()) {
                    allUnequip(1);
                }
            }

            if (checkHorseRide()) {
                if (fopAcM_getTalkEventPartner(this) == daPy_py_c::getMidnaActor()) {
                    daHorse_c* horse_p = dComIfGp_getHorseActor();

                    if (!horse_p->checkOriginalDemo()) {
                        horse_p->changeOriginalDemo();
                        horse_p->changeDemoMode(14, 0);
                    }
                }
            }

            field_0x3000 = 0;
            return 1;
        }

        onNoResetFlg0(FLG0_UNK_100000);
        if (mGrabItemAcKeep.getActor() != NULL) {
            if (fopAcM_getTalkEventPartner(this) != NULL &&
                checkGrabTalkActor(fopAcM_getTalkEventPartner(this)))
            {
                return procGrabWaitInit();
            } else {
                return commonGrabPutInit();
            }
        }

        return procCoTalkInit();
    } else if (demoMode == daPy_demo_c::DEMO_UNK_4_e) {
        int var_r28;
        if (checkWolf()) {
            var_r28 = procWolfWaitInit();
        } else {
            if (mDemo.getParam0() != 0) {
                freeGrabItem();

                if (mDemo.getParam0() == 1 && checkSwordGet()) {
                    if (mEquipItem != 0x103) {
                        deleteEquipItem(FALSE, TRUE);
                        if (checkEquipAnime()) {
                            resetUpperAnime(UPPER_2, -1.0f);
                        }

                        setSwordModel();
                    }
                } else if (mDemo.getParam0() == 2) {
                    deleteEquipItem(FALSE, TRUE);
                } else if (mDemo.getParam0() == 4) {
                    if (!checkFishingRodItem(mEquipItem)) {
                        deleteEquipItem(FALSE, TRUE);
                        mEquipItem = fpcNm_ITEM_FISHING_ROD_1;
                        setGroundFishingRodActor();
                    }
                } else if (mDemo.getParam0() == 3) {
                    if (!checkCanoeRide()) {
                        daCanoe_c* canoe_p =
                            (daCanoe_c*)fopAcIt_Judge((fopAcIt_JudgeFunc)daAlink_searchCanoe, NULL);
                        fpc_ProcID rodID =
                            fopAcM_create(PROC_MG_ROD, 0xD, &mLeftHandPos, -1, NULL, NULL, -1);
                        canoe_p->onRodID(rodID);
                        return procCanoeJumpRideInit(NULL);
                    }
                    return 1;
                } else if (mDemo.getParam0() == 5) {
                    if (!checkBoardRide()) {
                        fopAc_ac_c* board_p = (fopAc_ac_c*)fopAcIt_Judge(
                            (fopAcIt_JudgeFunc)daAlink_searchIceLeaf, NULL);
                        if (board_p != NULL) {
                            mGndPolySpecialCode = dBgW_SPCODE_LIGHT_SNOW;
                            return procBoardWaitInit(board_p);
                        }
                    } else {
                        return 1;
                    }
                } else if (mDemo.getParam0() == 6 && checkHorseRide()) {
                    mDemo.setParam0(0);
                    return 1;
                }

                mDemo.setParam0(0);
            }

            var_r28 = procWaitInit();
        }

        if (var_r28 != 0) {
            field_0x2060->initOldFrameMorf(0.0f, 0, field_0x30c6);
        }
        return var_r28;
    } else if (demoMode == daPy_demo_c::DEMO_UNK_9_e) {
        onNoResetFlg0(FLG0_UNK_100000);
        if (mProcID == PROC_LARGE_DAMAGE_UP || mProcID == PROC_WOLF_LARGE_DAMAGE_UP ||
            mProcID == PROC_LARGE_DAMAGE_WALL)
        {
            return 0;
        }

        if (checkModeFlg(0x40000)) {
            dComIfGp_evmng_cutEnd(mAlinkStaffId);
            return 0;
        }

        return procCoLargeDamageInit(-2, 1, 0, 0, NULL, 0);
    } else if (demoMode == daPy_demo_c::DEMO_UNK_36_e) {
        onNoResetFlg0(FLG0_UNK_100000);
        int prm0 = mDemo.getParam0();
        if (mProcID == PROC_CUT_NORMAL || mProcID == PROC_CUT_FINISH || mProcID == PROC_CUT_TURN) {
            return 0;
        }

        if (prm0 < 5) {
            return procCutNormalInit(prm0);
        }

        if (prm0 - 20 >= 0 && prm0 - 20 < 6) {
            return procCutFinishInit(prm0 - 20);
        } else {
            return procCutTurnInit(1, 1);
        }
    } else if (demoMode == daPy_demo_c::DEMO_UNK_82_e) {
        onNoResetFlg0(FLG0_UNK_100000);
        if (mProcID == PROC_TURN_MOVE || mProcID == PROC_CUT_FINISH_JUMP_UP ||
            mProcID == PROC_CUT_FINISH_JUMP_UP_LAND)
        {
            return 1;
        }

        return procTurnMoveInit(2);
    } else if (demoMode == daPy_demo_c::DEMO_UNK_84_e) {
        onNoResetFlg0(FLG0_UNK_100000);
        if (mProcID != PROC_CUT_FINISH) {
            onNoResetFlg0(FLG0_UNK_1000000);
            changeCutFast();
        }

        return 1;
    } else if (demoMode == daPy_demo_c::DEMO_UNK_43_e) {
        onNoResetFlg0(FLG0_UNK_100000);
        int prm0 = mDemo.getParam0();

        if (mProcID == PROC_WOLF_WAIT_ATTACK) {
            return 0;
        }

        if (prm0 >= 4) {
            return procWolfWaitAttackInit(0);
        } else {
            return procWolfWaitAttackInit(prm0);
        }
    } else if (demoMode == daPy_demo_c::DEMO_UNK_27_e) {
        onNoResetFlg0(FLG0_UNK_100000);

        if (mDemo.getDemoType() == daPy_demo_c::DEMO_TYPE_TOOL_e) {
            return commonLargeDamageUpInit(-2, 1, 0, 0);
        } else if (mDemo.getParam0() == 1) {
            return commonLargeDamageUpInit(-5, 1, 0, 0);
        } else {
            return commonLargeDamageUpInit(-3, 1, 0, 0);
        }
    } else if (demoMode == daPy_demo_c::DEMO_UNK_15_e) {
        onNoResetFlg0(FLG0_UNK_100000);
        return procWolfSitInit(1);
    } else if (demoMode == daPy_demo_c::DEMO_UNK_21_e) {
        onNoResetFlg0(FLG0_UNK_100000);
        return procWolfHowlInit(0);
    } else if (demoMode == daPy_demo_c::DEMO_UNK_22_e) {
        onNoResetFlg0(FLG0_UNK_100000);
        return procWolfHowlInit(1);
    } else if (demoMode == daPy_demo_c::DEMO_UNK_45_e) {
        onNoResetFlg0(FLG0_UNK_100000);
        return procCoWarpInit(mDemo.getParam0(), mDemo.getParam1());
    } else if (demoMode == daPy_demo_c::DEMO_UNK_24_e) {
        onNoResetFlg0(FLG0_UNK_100000);
        return procAutoJumpInit(1);
    } else if (demoMode == daPy_demo_c::DEMO_UNK_91_e) {
        if (mProcID == PROC_CUT_TURN_MOVE || mProcID == PROC_CUT_TURN_CHARGE) {
            return 1;
        }

        onNoResetFlg0(FLG0_UNK_100000);
        return procCutTurnChargeInit();
    } else if (demoMode == daPy_demo_c::DEMO_UNK_94_e) {
        if (mProcID == PROC_WOLF_SERVICE_WAIT) {
            return 0;
        }

        if (checkModeFlg(0x40000)) {
            return 0;
        }

        onNoResetFlg0(FLG0_UNK_100000);
        return procWolfServiceWaitInit(0);
    } else if (demoMode == daPy_demo_c::DEMO_UNK_92_e) {
        if (mProcID == PROC_CUT_LARGE_JUMP_CHARGE || mProcID == PROC_CUT_TURN_MOVE) {
            return 1;
        }

        onNoResetFlg0(FLG0_UNK_100000);
        return procCutLargeJumpChargeInit();
    } else if (demoMode == daPy_demo_c::DEMO_UNK_73_e) {
        onNoResetFlg0(FLG0_UNK_100000);
        return procSideRollInit(mDemo.getParam0() ? 3 : 2);
    } else if (demoMode == daPy_demo_c::DEMO_UNK_16_e) {
        onNoResetFlg0(FLG0_UNK_100000);
        if (checkWolf()) {
            if (mProcID == PROC_WOLF_BACKJUMP_LAND || mProcID == PROC_WOLF_SIDESTEP_LAND ||
                mProcID == PROC_WOLF_BACKJUMP || mProcID == PROC_WOLF_SIDESTEP)
            {
                return 0;
            }

            field_0x2f98 = mDemo.getParam0();
            if (field_0x2f98 == 1) {
                field_0x2fcc = 1;
            }

            return procWolfSideStepInit(0);
        }

        if (mProcID == PROC_BACK_JUMP_LAND || mProcID == PROC_SIDESTEP_LAND ||
            mProcID == PROC_BACK_JUMP || mProcID == PROC_SIDESTEP)
        {
            return 0;
        }

        if (mDemo.getParam0() == 0) {
            mDemo.setParam0(1);
        }

        return procSideStepInit(mDemo.getParam0());
    } else if (demoMode == daPy_demo_c::DEMO_UNK_44_e) {
        onNoResetFlg0(FLG0_UNK_100000);
        return procWolfLieMoveInit(0);
    } else if (demoMode == daPy_demo_c::DEMO_UNK_17_e) {
        onNoResetFlg0(FLG0_UNK_100000);
        return commonFallInit(1);
    } else if (demoMode == daPy_demo_c::DEMO_UNK_42_e) {
        onNoResetFlg0(FLG0_UNK_100000);
        return procBottleDrinkInit(mDemo.getParam0());
    } else if (demoMode == daPy_demo_c::DEMO_UNK_31_e) {
        onNoResetFlg0(FLG0_UNK_100000);
        if (!checkModeFlg(0x4070C52) &&
            (checkUpperReadyThrowAnime() || field_0x2f8c == 2 || field_0x2f8c == 1 ||
             field_0x2f8c == 3 || field_0x2f8c == 8 || mProcID == PROC_GRAB_WAIT ||
             mProcID == PROC_TALK || field_0x2f8c == 9))
        {
            return procCoLookWaitInit();
        }

        dComIfGp_evmng_cutEnd(mAlinkStaffId);
        return 1;
    } else if (demoMode == daPy_demo_c::DEMO_UNK_7_e) {
        offNoResetFlg0(FLG0_UNK_100000);
        if (checkUpperReadyThrowAnime() || checkGrabAnime()) {
            resetUpperAnime(UPPER_2, -1.0f);
        }

        if (!checkModeFlg(0x400)) {
            initForceRideHorse();
        }

        return checkNextActionHorse();
    } else if (checkFlyAtnWait()) {
        return procAtnActorWaitInit();
    } else if ((checkNoResetFlg0(FLG0_UNK_100000) || checkUpperReadyThrowAnime() ||
                checkModeFlg(0x400) || mProcID == PROC_SWIM_SUBJECTIVITY ||
                mProcID == PROC_FISHING_CAST || mProcID == PROC_SUBJECTIVITY) &&
               (demoMode == daPy_demo_c::DEMO_UNK_1_e || checkDemoMoveMode(demoMode) || demoMode == daPy_demo_c::DEMO_UNK_23_e ||
                demoMode == daPy_demo_c::DEMO_UNK_31_e || demoMode == daPy_demo_c::DEMO_UNK_18_e ||
                (checkSpinnerRide() &&
                 (demoMode == daPy_demo_c::DEMO_UNK_26_e || (demoMode == daPy_demo_c::DEMO_UNK_14_e && mDemo.getParam0() == 0)))))
    {
        offNoResetFlg0(FLG0_UNK_100000);
        if (checkUpperReadyThrowAnime()) {
            resetUpperAnime(UPPER_2, -1.0f);
        }

        if (checkReinRide()) {
            if (mDemo.getDemoMode() == daPy_demo_c::DEMO_UNK_23_e && !checkEquipAnime() && mEquipItem != 0x103 &&
                (mDemo.getParam1() == 2 || mDemo.getParam1() == 3))
            {
                swordEquip(0);
            }

            if (checkHorseGetOffMode() || checkHorseRideOn()) {
                fopAc_ac_c* rideAc_p = mRideAcKeep.getActor();
                if (rideAc_p == NULL) {
                    return commonCheckNextAction(0);
                }

                commonInitForceRideRein();

                if (rideAc_p == dComIfGp_getHorseActor()) {
                    static_cast<daHorse_c*>(rideAc_p)->onRideFlg();
                    mZ2Link.setRiding(true);
                } else {
                    ((e_wb_class*)rideAc_p)->setPlayerRide();
                }
            }

            return checkNextActionHorse();
        } else if (checkCanoeRide()) {
            return checkNextActionCanoe();
        } else if (checkBoardRide()) {
            return checkNextActionBoard();
        } else if (checkSpinnerRide()) {
            return procFallInit(1, mpHIO->mAutoJump.m.mFallInterpolation);
        }

        return commonCheckNextAction(0);
    }

    return 0;
}


bool daAlink_c::checkDemoMoveMode(u32 i_mode) const {
    return i_mode == daPy_demo_c::DEMO_UNK_2_e || i_mode == daPy_demo_c::DEMO_UNK_3_e || i_mode == daPy_demo_c::DEMO_UNK_38_e;
}

void daAlink_c::setDemoMoveData(u32* o_mode, cXyz const* i_goal) {
    cXyz sp20;
    sp20 = *i_goal - current.pos;

    f32 temp_f30;
    f32 temp_f29;
    f32 temp_f28;
    f32 var_f27;
    if (checkWolf()) {
        var_f27 = mpHIO->mWolf.mWlMoveNoP.m.mMaxSpeed / mpHIO->mMove.m.mMaxSpeed;
        var_f27 *= var_f27;
    } else {
        var_f27 = 1.0f;
    }

    temp_f30 = var_f27 * 100.0f;
    temp_f29 = var_f27 * 400.0f;
    temp_f28 = var_f27 * 2500.0f;

    f32 dist_to_pos = sp20.abs2XZ();
    if (dist_to_pos < var_f27 * 25.0f) {
        *o_mode = daPy_demo_c::DEMO_UNK_1_e;

        if (!checkModeFlg(0x2)) {
            mNormalSpeed = 0.0f;
        }
    } else if (dist_to_pos < temp_f30 || (dist_to_pos < temp_f28 && checkZeroSpeedF())) {
        *o_mode = daPy_demo_c::DEMO_UNK_1_e;
    } else if ((*o_mode == daPy_demo_c::DEMO_UNK_2_e && dist_to_pos < temp_f29) || dist_to_pos < temp_f28) {
        *o_mode = daPy_demo_c::DEMO_UNK_2_e;
        mDemo.setStick(0.35000002f);
    } else if (mDemo.getDemoType() == daPy_demo_c::DEMO_TYPE_ORIGINAL_e) {
        mDemo.setStick(1.0f);
    }

    mDemo.setMoveAngle(sp20.atan2sX_Z());
}

void daAlink_c::setNoDrawSwordShield(int param_0, u16 param_1) {
    if (param_0 == 5) {
        if (param_1 == 2) {
            offNoResetFlg2(FLG2_UNK_2000000);
            onNoResetFlg3(FLG3_UNK_80000000);
        } else if (param_1 == 1) {
            onNoResetFlg2(FLG2_UNK_2000000);
            offNoResetFlg3(FLG3_UNK_80000000);
        } else {
            offNoResetFlg2(FLG2_UNK_2000000);
            offNoResetFlg3(FLG3_UNK_80000000);
        }
    } else if (param_0 == 6) {
        if (param_1 != 0) {
            onNoResetFlg2(FLG2_UNK_4000000);
        } else {
            offNoResetFlg2(FLG2_UNK_4000000);
        }
    } else if (param_0 == 3) {
        if (param_1 == 1) {
            onEndResetFlg0(ERFLG0_UNK_800000);
        } else if (param_1 == 2) {
            onEndResetFlg1(ERFLG1_UNK_80000);
        } else if (param_1 == 3) {
            onEndResetFlg1(ERFLG1_UNK_400000);
        }
    } else {
        if (param_0 == 7) {
            if (param_1 == 1) {
                onNoResetFlg0(FLG0_PLAYER_NO_DRAW);
            } else {
                offNoResetFlg0(FLG0_PLAYER_NO_DRAW);
            }
        } else if (param_0 == 8) {
            if (param_1 == 1) {
                onNoResetFlg3(FLG3_UNK_1000000);
            } else {
                offNoResetFlg3(FLG3_UNK_1000000);
                
            }
        } else if (param_0 == 9) {
            if (param_1 == 1) {
                onNoResetFlg2(FLG2_PLAYER_SHADOW);
            } else {
                offNoResetFlg2(FLG2_PLAYER_SHADOW);
            }
        }
    }
}

void daAlink_c::setDemoData() {
    if (dComIfGp_getEvent().isOrderOK()) {
        if (checkPlayerDemoMode() && mClothesChangeWaitTimer == 0) {
            endDemoMode();
        }

        if (mMidnaTalkDelayTimer != 0) {
            mMidnaTalkDelayTimer--;
        }
    } else {
        dDemo_actor_c* demo_actor_p = dDemo_c::getActor(demoActorID);
        cXyz* pos_p = NULL;
        s16 angle;
        u32 demo_mode = daPy_demo_c::DEMO_UNK_1_e;
        int* prm0_p = NULL;
        int* prm1_p = NULL;

        dComIfGp_clearPlayerStatus0(0, 0x600000);
        offNoResetFlg3(FLG3_COPY_ROD_THROW_AFTER);
        field_0x30d0 = 0;

        if (mProcID == PROC_SUBJECTIVITY) {
            if (checkWolf()) {
                procWolfWaitInit();
            } else {
                procWaitInit();
            }
        } else if (mProcID == PROC_SWIM_SUBJECTIVITY) {
            if (checkWolf()) {
                procWolfSwimWaitInit(0);
            } else {
                procSwimWaitInit(0);
            }
        }

        if (mDemo.getDemoType() == daPy_demo_c::DEMO_TYPE_START_e && !dComIfGp_evmng_checkStartDemo()) {
            mDemo.setSystemDemoType();
        }

        if (demo_actor_p != NULL) {
            demo_actor_p->setModel(mpLinkModel);
            mAlinkStaffId = -1;

            if (mDemo.getDemoType() != daPy_demo_c::DEMO_TYPE_TOOL_e) {
                mDemo.setToolDemoType();
                mDemo.setDemoMode(daPy_demo_c::DEMO_UNK_1_e);
                freeGrabItem();

                if (dComIfGp_evmng_startCheck("R22-opening")) {
                    if (mEquipItem == fpcNm_ITEM_IRONBALL) {
                        deleteEquipItem(FALSE, FALSE);
                    }
                } else {
                    deleteEquipItem(FALSE, TRUE);
                }

                if (checkIronBallWaitAnime()) {
                    resetUpperAnime(UPPER_2, -1.0f);
                }

                if (checkWolf()) {
                    procWolfWaitInit();
                } else {
                    procWaitInit();
                }
            }

            if (demo_actor_p->checkEnable(0x20)) {
                demo_mode = demo_actor_p->getAnmId();
            } else {
                demo_mode = mDemo.getDemoMode();
            }

            if (demo_actor_p->checkEnable(0x2)) {
                pos_p = &demo_actor_p->getTrans();
            } else {
                pos_p = &current.pos;
            }

            if (demo_actor_p->checkEnable(0x8)) {
                angle = demo_actor_p->getRatate().y;
            } else {
                angle = shape_angle.y;
            }

            int sp14;
            int sp10;
            int spC;
            u16 sp8;
            while (demo_actor_p->getDemoIDData(&sp14, &sp10, &spC, &sp8, NULL) != 0) {
                if (sp14 == 0 && sp10 == 0) {
                    setNoDrawSwordShield(spC, sp8);
                }
            }
        } else {
            BOOL var_r28 = FALSE;
            if (checkPlayerDemoMode() == 0) {
                mDemo.setSystemDemoType();
                var_r28 = TRUE;

                if (mDemo.getDemoMode() == daPy_demo_c::DEMO_UNK_0_e) {
                    demo_mode = daPy_demo_c::DEMO_UNK_65_e;
                }
            }

            if (mAlinkStaffId != -1) {
                if (mProcID == PROC_DUNGEON_WARP_READY) {
                    mDemo.setSystemDemoType();
                }

                char* cut_name = dComIfGp_getPEvtManager()->getMyNowCutName(mAlinkStaffId);
                if (cut_name != NULL) {
                    // Take the first 3 chars in cut_name, convert them to integers, and calculate
                    // demo_mode
                    demo_mode = ((cut_name[0] - '0') * 100) + ((cut_name[1] - '0') * 10) +
                                (cut_name[2] - '0');
                    if (mDemo.getDemoType() != daPy_demo_c::DEMO_TYPE_START_e && (demo_mode == daPy_demo_c::DEMO_UNK_1_e || demo_mode == daPy_demo_c::DEMO_UNK_23_e) &&
                        mLinkAcch.ChkGroundHit() && !checkModeFlg(0x70C52))
                    {
                        mNormalSpeed = 0.0f;
                        speedF = 0.0f;
                    }

                    if (demo_mode == daPy_demo_c::DEMO_WOLF_MIDNA_RIDE_SHOCK_e && !daMidna_c::checkMidnaRealBody()) {
                        demo_mode = daPy_demo_c::DEMO_UNK_1_e;
                    }

                    pos_p = dComIfGp_evmng_getMyXyzP(mAlinkStaffId, "pos");
                    if (pos_p == NULL) {
                        pos_p = dComIfGp_evmng_getGoal();
                    }

                    int* angle_p = dComIfGp_evmng_getMyIntegerP(mAlinkStaffId, "angle");
                    if (angle_p != NULL) {
                        angle = *angle_p;
                    } else if (demo_mode == daPy_demo_c::DEMO_WAIT_TURN_e) {
                        angle = mDemo.getMoveAngle();
                    } else {
                        angle = shape_angle.y;
                    }

                    prm0_p = dComIfGp_evmng_getMyIntegerP(mAlinkStaffId, "prm0");
                    prm1_p = dComIfGp_evmng_getMyIntegerP(mAlinkStaffId, "prm1");

                    if (mDemo.getDemoType() == daPy_demo_c::DEMO_TYPE_SYSTEM_e) {
                        f32* stick_p = dComIfGp_evmng_getMyFloatP(mAlinkStaffId, "stick");

                        if (stick_p != NULL) {
                            mDemo.setStick(*stick_p);
                        } else {
                            mDemo.setStick(1.0f);
                        }
                    }

                    if ((demo_mode == daPy_demo_c::DEMO_UNK_2_e || demo_mode == daPy_demo_c::DEMO_UNK_3_e) && prm1_p != NULL && *prm1_p == 1) {
                        cXyz goal_pos;
                        mDoMtx_stack_c::transS(current.pos);
                        mDoMtx_stack_c::YrotM(shape_angle.y);
                        mDoMtx_stack_c::multVec(pos_p, &goal_pos);
                        dComIfGp_evmng_setGoal(&goal_pos);
                        dComIfGp_evmng_cutEnd(mAlinkStaffId);
                    }
                }
            }

            if ((mEquipItem == fpcNm_ITEM_IRONBALL || checkFishingRodItem(mEquipItem)) && demo_mode != daPy_demo_c::DEMO_UNK_1_e &&
                demo_mode != daPy_demo_c::DEMO_UNK_31_e && (mAlinkStaffId != -1 || mDemo.getDemoType() == daPy_demo_c::DEMO_TYPE_ORIGINAL_e) &&
                (demo_mode != daPy_demo_c::DEMO_UNK_6_e || (var_r28 == 0 && !checkEquipAnime())))
            {
                deleteEquipItem(FALSE, TRUE);
                if (checkIronBallWaitAnime()) {
                    resetUpperAnime(UPPER_2, 3.0f);
                }
            }
        }

        if (mDemo.getDemoType() == daPy_demo_c::DEMO_TYPE_START_e) {
            if (mDemo.getDemoMode() == daPy_demo_c::DEMO_UNK_14_e) {
                if (mDemo.getTimer() != 0) {
                    mDemo.decTimer();

                    cXyz tmp_sp = current.pos - home.pos;
                    if (tmp_sp.abs2XZ() > 90000.0f) {
                        mDemo.setTimer(0);
                        dComIfGp_evmng_cutEnd(mAlinkStaffId);
                    }
                } else {
                    dComIfGp_evmng_cutEnd(mAlinkStaffId);
                }
            }
        } else {
            if (mDemo.getDemoType() == daPy_demo_c::DEMO_TYPE_TOOL_e || (mDemo.getDemoType() == daPy_demo_c::DEMO_TYPE_SYSTEM_e && mAlinkStaffId != -1)) {
                if (prm0_p != NULL) {
                    mDemo.setParam0(*prm0_p);
                } else {
                    mDemo.setParam0(0);
                }

                if (prm1_p != NULL) {
                    mDemo.setParam1(*prm1_p);
                } else {
                    mDemo.setParam1(0);
                }

                if (demo_mode == daPy_demo_c::DEMO_UNK_4_e) {
                    mNormalSpeed = 0.0f;
                    speedF = 0.0f;
                    field_0x814.ClrCcMove();
                    setPlayerPosAndAngle(pos_p, angle, 0);
                    mDemo.setMoveAngle(angle);
                } else if (checkDemoMoveMode(demo_mode)) {
                    setDemoMoveData(&demo_mode, pos_p);
                } else if (demo_mode == daPy_demo_c::DEMO_WAIT_TURN_e) {
                    mDemo.setMoveAngle(angle);
                }

                mDemo.setDemoMode(demo_mode);
            } else if (mDemo.getDemoType() == daPy_demo_c::DEMO_TYPE_ORIGINAL_e) {
                demo_mode = mDemo.getDemoMode();

                if (checkDemoMoveMode(demo_mode)) {
                    setDemoMoveData(&demo_mode, mDemo.getPos0());
                    mDemo.setDemoMode(demo_mode);
                } else if (demo_mode == daPy_demo_c::DEMO_UNK_65_e) {
                    mDemo.setDemoMode(daPy_demo_c::DEMO_UNK_1_e);
                }
            } else {
                mDemo.setDemoMode(demo_mode);
            }

            if (checkModeFlg(0x80000) || mDemo.getDemoMode() == daPy_demo_c::DEMO_UNK_6_e || mDemo.getDemoMode() == daPy_demo_c::DEMO_UNK_8_e) {
                dComIfGp_setPlayerStatus0(0, 0x10);
                mMidnaTalkDelayTimer = 30;
            } else {
                dComIfGp_clearPlayerStatus0(0, 0x10);
            }

            if (mAlinkStaffId != -1 &&
                (demo_mode == daPy_demo_c::DEMO_UNK_1_e || demo_mode == daPy_demo_c::DEMO_UNK_4_e || demo_mode == daPy_demo_c::DEMO_UNK_23_e || demo_mode == daPy_demo_c::DEMO_UNK_14_e ||
                 demo_mode == daPy_demo_c::DEMO_UNK_17_e || demo_mode == daPy_demo_c::DEMO_UNK_24_e || demo_mode == daPy_demo_c::DEMO_UNK_7_e || demo_mode == daPy_demo_c::DEMO_UNK_15_e ||
                 demo_mode == daPy_demo_c::DEMO_UNK_21_e || demo_mode == daPy_demo_c::DEMO_UNK_22_e || demo_mode == daPy_demo_c::DEMO_UNK_6_e || demo_mode == daPy_demo_c::DEMO_UNK_8_e ||
                 demo_mode == daPy_demo_c::DEMO_CROUCH_e || demo_mode == daPy_demo_c::DEMO_UNK_18_e))
            {
                dComIfGp_evmng_cutEnd(mAlinkStaffId);
            }

            s16 prm2 = mDemo.getParam2();
            if (prm2 != 0) {
                if (!checkWolf()) {
                    if (prm2 == 1) {
                        setFacePriTexture(FTANM_UNK_0);
                        setFacePriBck(0x15A);
                    } else if (prm2 == 2) {
                        setFacePriBtp(0x3E1);
                        field_0x2154->setFrame(field_0x2154->getFrameMax());

                        setFacePriBtk(0x37B);
                        mpFaceBtk->setFrame(mpFaceBtk->getFrameMax());

                        setFacePriBck(0x149);
                    }

                    offNoResetFlg1(FLG1_UNK_2000);
                    onNoResetFlg1(FLG1_UNK_200);
                }
            } else if (checkNoResetFlg1(FLG1_UNK_200)) {
                offNoResetFlg1(FLG1_UNK_200);
                resetFacePriAnime();
            }
        }
    }
}


void daAlink_c::resetDemoBck() {
    mFaceBckHeap.resetArcNo();
    mFaceBtpHeap.resetArcNo();
    mFaceBtkHeap.resetArcNo();
    mUnderAnmHeap[0].resetArcNo();
    mUnderAnmHeap[0].resetIdx();
    mFaceBckHeap.resetIdx();
    mFaceBtpHeap.resetIdx();
    mFaceBtkHeap.resetIdx();

    if (checkWolf()) {
        setSingleAnimeWolfBaseSpeed(WANM_WAIT, mpHIO->mWolf.mWlMoveNoP.m.mIdleAnmSpeed,
                                    mpHIO->mWolf.mWlMove.m.mNormalInterpolation);
        setFaceBasicAnime(ANM_WAIT);
    } else {
        setSingleAnimeBaseSpeed(ANM_WAIT, mpHIO->mMove.m.mWaitAnmSpeed,
                                mpHIO->mBasic.m.mBasicInterpolation);
    }
}

void daAlink_c::endHighModel() {
    if (field_0x068c != NULL) {
        mpDemoFCTongueModel->getModelData()->removeTexMtxAnimator(field_0x068c);
    }

    offNoResetFlg1(FLG1_UNK_10);
    field_0x0698 = NULL;
    field_0x068c = NULL;
}

void daAlink_c::resetSpecialEvent() {
    dComIfGp_event_reset();
    dCam_getBody()->EndEventCamera(fopAcM_GetID(this));
    endDemoMode();
}

void daAlink_c::endDemoMode() {
    BOOL temp_r30 = checkFlyAtnWait();
    endHighModel();
    offNoResetFlg0(FLG0_UNK_10400);
    field_0x06f4 = NULL;

    if (field_0x06f8 != NULL) {
        if (mpDemoHLTmpModel != NULL) {
            mpDemoHLTmpModel->getModelData()->removeTevRegAnimator(field_0x06f8);
        }
        field_0x06f8 = NULL;
    }

    if (mpDemoHDTmpBck != NULL && mpLinkHatModel != NULL && mClothesChangeWaitTimer == 0) {
        mpDemoHDTmpBck->remove(mpLinkHatModel->getModelData());
    }

    offNoResetFlg0(FLG0_UNK_100000);
    dComIfGp_clearPlayerStatus0(0, 0x10);

    BOOL temp_r29 = mDemo.getDemoType() == daPy_demo_c::DEMO_TYPE_START_e;
    BOOL var_r0 = mDemo.getDemoMode() == daPy_demo_c::DEMO_UNK_14_e || mDemo.getDemoMode() == daPy_demo_c::DEMO_UNK_26_e;

    mDemo.resetDemoType();
    mDemo.setDemoMode(0);
    mDemo.setParam0(0);
    mDemo.setParam1(0);

    if (checkNoResetFlg1(FLG1_UNK_200)) {
        offNoResetFlg1(FLG1_UNK_200);
        resetFacePriAnime();
    }

    mDemo.setParam2(0);
    mDemo.setStick(1.0f);
    mAlinkStaffId = -1;

    if (checkReinRide()) {
        field_0x2fab &= ~0x8;
        if (mProcID != PROC_HORSE_COMEBACK) {
            procHorseWaitInit();
        }
    } else if (!checkSpinnerRide() && mProcID != PROC_FISHING_CAST) {
        if (temp_r30) {
            if (checkSwimAction(1)) {
                if (checkWolf()) {
                    procWolfSwimMoveInit();
                } else {
                    procSwimMoveInit();
                }
            } else {
                checkWaitAction();
            }
        } else if ((mLinkAcch.ChkGroundHit() && (!checkUpperReadyThrowAnime() || var_r0 == 0) &&
                    !checkModeFlg(0x4070C52) && mProcID != PROC_LARGE_DAMAGE_UP &&
                    mProcID != PROC_WOLF_LARGE_DAMAGE_UP) ||
                   checkMagneBootsOn())
        {
            if (temp_r29 && (mProcID == PROC_MOVE || mProcID == PROC_WOLF_MOVE)) {
                field_0x33a8 = mDoCPd_c::getStickValue(PAD_1);
                mStickAngle = mDoCPd_c::getStickAngle(PAD_1) + 0x8000;
                field_0x2fe2 =
                    mStickAngle + dCam_getControledAngleY(dComIfGp_getCamera(field_0x317c));
                mNormalSpeed = speedF;
                commonCheckNextAction(0);
            } else if (mProcID == PROC_HAWK_CATCH || mProcID == PROC_HAWK_SUBJECT) {
                procHawkSubjectInit();
            } else if (mProcID == PROC_GRASS_WHISTLE_WAIT || (mDemo.getDemoMode() != daPy_demo_c::DEMO_UNK_45_e && mProcID == PROC_WARP)) {
                return;
            } else if (mProcID == PROC_TALK && dComIfGp_checkPlayerStatus0(0, 0x8000000)) {
                procWolfLieMoveInit(0);
            } else {
                checkWaitAction();
            }
        } else if (mProcID == PROC_CAUGHT) {
            if (checkModeFlg(0x40000)) {
                if (checkWolf()) {
                    procWolfSwimWaitInit(0);
                } else {
                    procSwimWaitInit(0);
                }
            } else {
                checkWaitAction();
            }
        } else if (mProcID == PROC_MONKEY_MOVE) {
            procFallInit(1, mpHIO->mAutoJump.m.mFallInterpolation);
        } else if (checkCanoeRide()) {
            procCanoeWaitInit(0);
        }
    }
}

fopAc_ac_c* daAlink_c::getDemoLookActor() {
    if (mDemo.getParam0() == 1) {
        return dComIfGp_event_getPt1();
    } else if (mDemo.getParam0() == 2) {
        return dComIfGp_event_getPt2();
    } else if (mDemo.getParam0() == 3) {
        return fopAcM_getTalkEventPartner(this);
    } else if (mDemo.getParam0() == 4) {
        return getMidnaActor();
    }

    return NULL;
}

BOOL daAlink_c::checkFlyAtnWait() {
    return mDemo.getDemoMode() == daPy_demo_c::DEMO_UNK_23_e && (mDemo.getParam1() == 4 || mDemo.getParam1() == 5);
}

void daAlink_c::setGetItemFace(u16 param_0) {
    if (param_0 == 7 || param_0 == 6 || param_0 == 0x21 || param_0 == 0x22 || param_0 == 0x31 ||
        param_0 == 0x36 || (param_0 >= 0x40 && param_0 <= 0x47) || param_0 == 0x4A ||
        param_0 == 0x56 || param_0 == 0xED || param_0 == 0x73 || param_0 == 0x75 ||
        param_0 == 0xEB || param_0 == 0xA1 || param_0 == 0xA2 || param_0 == 0xA3)
    {
        setFaceBasicBck(0x143);
    } else if (param_0 == 0x25 || param_0 == 0xEC) {
        setFaceBasicTexture(FTANM_UNK_7C);
        setFaceBasicBck(0x149);
    } else if (param_0 == 0xF4 || param_0 == 0xF5) {
        setFaceBasicBck(0x146);
    }
}

BOOL daAlink_c::checkGrabTalkActor(fopAc_ac_c* i_actor) {
    s16 name = fopAcM_GetName(i_actor);
    return name == PROC_NPC_KYURY || name == PROC_MYNA || name == PROC_OBJ_SSITEM ||
           name == PROC_TAG_MSG;
}

int daAlink_c::setTalkStartBack(cXyz* param_0) {
    fopAc_ac_c* partner_p = fopAcM_getTalkEventPartner(this);
    if (partner_p != NULL) {
        s16 partner_name = fopAcM_GetName(partner_p);
        if (partner_name == PROC_MIDNA || partner_name == PROC_Tag_Wljump ||
            partner_name == PROC_Tag_Mhint || partner_name == PROC_Tag_Mstop ||
            partner_name == PROC_Tag_Mwait)
        {
            return 0;
        }

        cXyz sp24 = current.pos - partner_p->current.pos;
        f32 var_f1 = sp24.absXZ();
        if (var_f1 < 100.0f && var_f1 > 1.0f) {
            f32 temp_f2_2 = 100.0f / var_f1;
            param_0->x = partner_p->current.pos.x + temp_f2_2 * sp24.x;
            param_0->y = current.pos.y + l_autoUpHeight;
            param_0->z = partner_p->current.pos.z + temp_f2_2 * sp24.z;
            mLinkGndChk.SetPos(param_0);

            if (dComIfG_Bgsp().GroundCross(&mLinkGndChk) - current.pos.y >= l_autoDownHeight) {
                current.angle.y = sp24.atan2sX_Z();
                mNormalSpeed = 5.0f;
                setSingleAnimeBaseSpeed(ANM_WALK, -mpHIO->mMove.m.mWalkAnmSpeed, 3.0f);
                return 1;
            }
        }
    }

    return 0;
}


void daAlink_c::setShapeAngleToTalkActor() {
    fopAc_ac_c* partner_p = fopAcM_getTalkEventPartner(this);
    if (partner_p != NULL) {
        s16 partner_name = fopAcM_GetName(partner_p);

        if (partner_name != PROC_MYNA &&
            (partner_name != PROC_Tag_Mwait ||
             !static_cast<daTagMwait_c*>(partner_p)->checkEndMessage()) &&
            (partner_name != PROC_Tag_Mhint ||
             !static_cast<daTagMhint_c*>(partner_p)->checkNoAttention()) &&
            (partner_name != PROC_Tag_Mstop ||
             !static_cast<daTagMstop_c*>(partner_p)->checkNoAttention()) &&
            partner_name != PROC_MIDNA)
        {
            s16 target;
            if (partner_name == PROC_Tag_Mhint || partner_name == PROC_Tag_Mstop) {
                target = cLib_targetAngleY(&current.pos, &partner_p->eyePos);
            } else {
                target = fopAcM_searchActorAngleY(this, partner_p);
            }

            cLib_addCalcAngleS(&shape_angle.y, target, 4, 0x1000, 0x200);
        }
    }
}

void daAlink_c::setTalkAnime() {
    if (mDemo.getDemoMode() == daPy_demo_c::DEMO_UNK_8_e || mEquipItem == fpcNm_ITEM_KANTERA ||
        (checkNoResetFlg2(FLG2_UNK_1) && field_0x2fde == 0x48))
    {
        setDoubleAnime(0.0f, mpHIO->mMove.m.mWaitAnmSpeed,
                       mpHIO->mMove.m.mWaitAnmSpeed, ANM_WAIT, ANM_WAIT, 2, 3.0f);
    } else {
        setSingleAnimeBase(ANM_TALK);
    }
}

u8 daAlink_c::setTradeItemAnime() {
    setSingleAnimeBase(ANM_TRADE_ITEM_PULL_OUT);
    offModeFlg(0x100);

    if (mDemo.getParam0() != 1) {
        int item_no;
        if (mDemo.getParam0() == 2) {
            item_no = mDemo.getParam1();
        } else {
            item_no = dComIfGp_event_getPreItemNo();
        }

        fpc_ProcID partner_id = fopAcM_createItemForPresentDemo(
            &current.pos, item_no, 3, -1, fopAcM_GetRoomNo(this), &shape_angle, &scale);
        dComIfGp_event_setItemPartnerId(partner_id);
    }

    keepItemData();
    return 1;
}

void daAlink_c::setTradeItemOutHand() {
    daPy_frameCtrl_c* frame_ctrl = mUnderFrameCtrl;

    if (frame_ctrl->getFrame() >= 59.0f) {
        field_0x2f92 = 4;
    } else if (frame_ctrl->getFrame() >= 20.0f) {
        field_0x2f92 = 0;
    }
}

BOOL daAlink_c::checkEndMessage(u32 param_0) {
    if (mMsgClassID == fpcM_ERROR_PROCESS_ID_e) {
        mMsgClassID = fopMsgM_messageSet(param_0, 1000);
    } else {
        msg_class* msg = fopMsgM_SearchByID(mMsgClassID);

        if (msg != NULL) {
            if (msg->mode == 14) {
                msg->mode = 16;
            } else if (msg->mode == 0x12) {
                msg->mode = 0x13;
                return 1;
            }
        }
    }

    return 0;
}

u8 daAlink_c::setDemoRightHandIndex(u16 param_0) {
    if (param_0 == 0xFD) {
        mProcVar4.field_0x3010 = 1;
        return 6;
    } else if (param_0 == 0xFC) {
        field_0x2fab |= 0x10;
        return 5;
    } else if (param_0 == 0xFB) {
        if (mpDemoHRTmpModel != NULL) {
            return 0xFB;
        } else {
            return 0xFE;
        }
    } else if (param_0 != 0xFE) {
        return param_0 + 5;
    } else {
        return param_0 & 0xFF;
    }
}

u8 daAlink_c::setDemoLeftHandIndex(u16 param_0) {
    if (param_0 == 0xFD) {
        if (mEquipItem != 0x103) {
            deleteEquipItem(FALSE, TRUE);
            setSwordModel();
        }
        return 0;
    } else if (param_0 == 0xFC) {
        field_0x2fab |= 0x8;
        return 0;
    } else if (param_0 == 0xFB) {
        if (mpDemoHLTmpModel != NULL) {
            return 0xFB;
        } else {
            return 0xFE;
        }
    } else {
        return param_0 & 0xFF;
    }
}

void daAlink_c::setDemoRide(u16 param_0) {
    daHorse_c* horse_p = dComIfGp_getHorseActor();

    if (param_0 == 1) {
        if (horse_p != NULL && !checkModeFlg(0x400)) {
            onModeFlg(0x400);
            horse_p->onRideFlg();
            field_0x384c = (cXyz*)&l_horseBaseAnime;
            mRideStatus = 1;
        }
    } else {
        if (horse_p != NULL) {
            horse_p->offRideFlg();
        }

        mRideStatus = 0;
        offModeFlg(0x400);
    }
}

void daAlink_c::setDemoBodyBck(dDemo_actor_c* param_0, u16 param_1) {
    J3DAnmTransform* bck = (J3DAnmTransform*)mUnderAnmHeap[0].loadDataDemoRID(param_1, 0);
    if (bck != NULL) {
        setFrameCtrl(mUnderFrameCtrl, bck->getAttribute(), 0, bck->getFrameMax(), 1.0f, 0.0f);
        mNowAnmPackUnder[0].setAnmTransform(bck);
        mNowAnmPackUpper[0].setAnmTransform(bck);

        dDemo_actor_c* demo_actor_p = dDemo_c::getActor(demoActorID);
        if (demo_actor_p->checkEnable(0x80)) {
            field_0x2060->initOldFrameMorf(demo_actor_p->getPrm_Morf(), 0, field_0x30c6);
        }

        field_0x2f8c = 0;
        param_0->setAnmFrameMax(bck->getFrameMax());
        setBasAnime(UNDER_0);
    }
}


BOOL daAlink_c::checkFinalBattle() {
    return checkStageName("D_MN09B") && dComIfGs_isSaveDunSwitch(1);
}

BOOL daAlink_c::checkRestartDead(int i_dmgAmount, int i_checkZoraMag) {
    i_dmgAmount *= (int)damageMagnification(i_checkZoraMag, 1);

    if (mProcID != PROC_DEAD && i_dmgAmount >= dComIfGs_getLife() && !checkMagicArmorNoDamage() &&
        !dComIfGs_checkBottle(fpcNm_ITEM_FAIRY))
    {
        onNoResetFlg2(FLG2_FORCE_GAMEOVER);
        procCoDeadInit(0);
        return 1;
    }

    return 0;
}

void daAlink_c::setDeadRideSyncPos() {
    if (checkModeFlg(MODE_RIDING) && mRideAcKeep.getActor() != NULL) {
        if (checkReinRide()) {
            setSyncRidePos();
        } else if (checkSpinnerRide()) {
            s16 old_angle = shape_angle.y;
            setSpinnerSyncPos();
            shape_angle.y = old_angle;
        }
    }
}

BOOL daAlink_c::checkDeadHP() {
    return ((dComIfGs_getLife() == 0 && !checkResetFlg0(RFLG0_UNK_400)) && !dComIfGs_checkBottle(fpcNm_ITEM_FAIRY)) ||
           checkNoResetFlg2(FLG2_FORCE_GAMEOVER) ||
           (dComIfGp_getOxygenShowFlag() && dComIfGp_getNowOxygen() == 0);
}

BOOL daAlink_c::checkDeadAction(int param_0) {
    if (!checkEventRun()) {
        if (checkDeadHP() && mProcID != PROC_HORSE_RIDE && mProcID != PROC_HORSE_GETOFF && mProcID != PROC_LARGE_DAMAGE && mProcID != PROC_LARGE_DAMAGE_WALL && (param_0 || (mProcID != PROC_LARGE_DAMAGE_UP && mProcID != PROC_WOLF_LARGE_DAMAGE_UP))) {
            if (mLinkAcch.ChkGroundHit() || checkModeFlg(0x40000) || checkBoarRide() || (checkHorseRide() && !dComIfGp_getHorseActor()->checkJump())) { 
                return procCoDeadInit(1);
            }

            if (!checkHorseRide()) {
                if (checkMagneBootsOn() && !cancelMagneBootsOn()) {
                    return procCoDeadInit(1);
                }

                offCargoCarry();
    
                int var_r29;
                if (dComIfG_Bgsp().ChkPolySafe(mLinkAcch.m_gnd) && -G_CM3D_F_INF != mLinkAcch.GetGroundH()) {
                    var_r29 = dComIfG_Bgsp().GetExitId(mLinkAcch.m_gnd);
                } else {
                    var_r29 = 0x3F;
                }

                if (((var_r29 != 0x3F || mExitID != 0x3F) && field_0x3174 == 5 && field_0x33c8 - current.pos.y > 500.0f) || ((mExitID & 0x8000) && checkModeFlg(2))) {
                    onNoResetFlg2(FLG2_FORCE_GAMEOVER);
                    return procCoDeadInit(0);
                }

                return commonFallInit(1);
            }
        } else if (dComIfGs_getLife() == 0 && !checkResetFlg0(RFLG0_UNK_400) && dComIfGs_checkBottle(fpcNm_ITEM_FAIRY)) {
            makeFairy(&current.pos, 0);
            dComIfGs_setBottleItemIn(fpcNm_ITEM_FAIRY, fpcNm_ITEM_EMPTY_BOTTLE);
        }
    }

    return 0;
}

void daAlink_c::setHighModelBck(mDoExt_bckAnm* i_bck, u16 i_anmID) {
    if (i_bck != NULL) {
        J3DAnmTransform* bck = (J3DAnmTransform*)dComIfG_getObjectIDRes(
            dStage_roomControl_c::getDemoArcName(), i_anmID);

        if (bck != NULL && i_bck->getBckAnm() != bck) {
            if (field_0x06b4 == bck) {
                mpDemoHDTmpBck->changeBckOnly(NULL);
            } else {
                i_bck->init(bck, TRUE, -1, 1.0f, 0, -1, true);
            }
        }
    }
}

void daAlink_c::setHighModelFaceBtk(u16 i_anmID) {
    J3DAnmTextureSRTKey* btk = static_cast<J3DAnmTextureSRTKey*>(dComIfG_getObjectIDRes(
        dStage_roomControl_c::getDemoArcName(), i_anmID));

    if (btk && field_0x068c != btk) {
        btk->searchUpdateMaterialID(mpDemoFCTongueModel->getModelData());
        mpDemoFCTongueModel->getModelData()->entryTexMtxAnimator(btk);

        btk->setFrame(0.0f);
        field_0x068c = btk;
    }
}

void daAlink_c::setDemoBrk(J3DAnmTevRegKey** param_0, J3DModel* param_1, u16 i_anmID) {
    J3DAnmTevRegKey* brk = static_cast<J3DAnmTevRegKey*>(dComIfG_getObjectIDRes(
        dStage_roomControl_c::getDemoArcName(), i_anmID));

    if (brk != NULL && *param_0 != brk) {
        brk->searchUpdateMaterialID(param_1->getModelData());
        param_1->getModelData()->entryTevRegAnimator(brk);

        brk->setFrame(0.0f);
        *param_0 = brk;
    }
}

f32 daAlink_c::setStickAnmData(J3DAnmBase* i_anm, int param_1, int param_2, u16 param_3,
                               int param_4) {
    dDemo_actor_c* demo_actor_p = dDemo_c::getActor(demoActorID);

    f32 var_f31;
    if (i_anm != NULL) {
        var_f31 = i_anm->getFrame();
    } else {
        var_f31 = 0.0f;
    }

    if (param_4 == 0 || (checkInputOnR() && param_4 == getDirectionFromAngle(mStickAngle) + 1)) {
        if (param_4 != 0) {
            dDemo_c::setBranchId(1, 1);
        }

        if (param_1 == 1) {
            if (param_2 == 2) {
                setDemoBodyBck(demo_actor_p, param_3);

                if (param_4 == 0) {
                    if (demo_actor_p->checkEnable(0x40)) {
                        var_f31 = demo_actor_p->getAnmFrame();
                    } else {
                        var_f31 = 0.0f;
                    }
                }
            }
        } else if (param_1 == 9) {
            if (param_2 == 2) {
                if (checkNoResetFlg1(FLG1_UNK_10)) {
                    setHighModelBck(mpDemoFCTmpBck, param_3);
                    return 0.0f;
                }

                setFaceDemoBck(param_3);
            } else if (param_2 == 3) {
                if (checkNoResetFlg1(FLG1_UNK_10)) {
                    setHighModelFaceBtk(param_3);
                    return 0.0f;
                }

                setFaceDemoBtk(param_3);
            } else if (param_2 == 5) {
                setFaceDemoBtp(param_3);
            }

            if (param_4 != 0 && i_anm != NULL) {
                for (; var_f31 > i_anm->getFrameMax(); var_f31 -= i_anm->getFrameMax()) {
                }
                i_anm->setFrame(var_f31);
            }
        } else if (param_1 == 5) {
            setHighModelBck(mpDemoHRTmpBck, param_3);
        } else if (param_1 == 6) {
            setHighModelBck(mpDemoHLTmpBck, param_3);
        } else if (param_1 == 4) {
            setHighModelBck(mpDemoHDTmpBck, param_3);
        }
    }

    if (param_4 != 0 && i_anm != NULL) {
        for (; var_f31 > i_anm->getFrameMax(); var_f31 -= i_anm->getFrameMax()) {
        }
    }

    return var_f31;
}

static int daAlink_c_getDemoIDData(dDemo_actor_c* param_0, int* param_1, int* param_2,
                                       int* param_3, u16* param_4, int* param_5, int* param_6) {
    JStudio::stb::TParseData_fixed<51, TValueIterator_misaligned<u32> > sp30(param_0->getPrm()->getData());

    static JStudio::stb::TParseData_fixed<51, TValueIterator_misaligned<u32> > dummy;
    static JGadget::binary::TValueIterator_misaligned<u32> it(dummy.begin());

    if (it == sp30.end()) {
        it = dummy.begin();
        return 0;
    }

    if (it == dummy.begin()) {
        it = sp30.begin();
    }

    u32 var_r31 = *it;
    *param_1 = var_r31 >> 0x1E;
    *param_2 = (var_r31 >> 0x18) & 0xF;
    *param_6 = (var_r31 >> 0x17) & 1;
    *param_3 = (var_r31 >> 0x10) & 0xF;
    *param_4 = var_r31 & 0xFFFF;
    *param_5 = (var_r31 >> 0x14) & 7;

    it++;
    return 1;
}

int daAlink_c::procDemoCommon() {
    dComIfGp_evmng_cutEnd(mAlinkStaffId);
    return 1;
}

int daAlink_c::procCoToolDemoInit() {
    if (!commonProcInitNotSameProc(PROC_TOOL_DEMO)) {
        return 1;
    }

    mNormalSpeed = 0.0f;
    speedF = 0.0f;
    speed.y = 0.0f;

    mUnderAnmHeap[0].resetArcNo();
    mUnderAnmHeap[0].resetIdx();
    mUpperAnmHeap[0].resetIdx();
    mUnderAnmHeap[1].resetIdx();
    mUpperAnmHeap[1].resetIdx();

    setDoubleAnimeBlendRatio(0.0f);
    mNowAnmPackUnder[1].setAnmTransform(NULL);
    mNowAnmPackUpper[1].setAnmTransform(NULL);

    field_0x3198 = 0;
    mProcVar4.field_0x3010 = 0;
    field_0x2fab = 0;
    deleteEquipItem(FALSE, TRUE);
    mUnderAnmHeap[0].setBufferSize(0x10800);
    mProcVar2.field_0x300c = 0;
    field_0x3478 = 0.0f;
    field_0x37c8 = current.pos;

    onEndResetFlg1(ERFLG1_GANON_FINISH);
    return 1;
}

int daAlink_c::procCoToolDemo() {
    dDemo_actor_c* demo_actor_p = dDemo_c::getActor(demoActorID);
    BOOL var_r29 = 0;

    onEndResetFlg1(ERFLG1_GANON_FINISH);
    field_0x3198 = 0;
    mProcVar4.field_0x3010 = 0;
    field_0x2fab = 0;

    if (demo_actor_p != NULL) {
        f32 var_f31 = 0.0f;

        if (demo_actor_p->checkEnable(2)) {
            current.pos = demo_actor_p->getTrans();
        }

        if (demo_actor_p->checkEnable(8)) {
            shape_angle = demo_actor_p->getRatate();
            current.angle = shape_angle;
        }

        if (demo_actor_p->checkEnable(0x40)) {
            var_f31 = demo_actor_p->getAnmFrame();
        }

        if (demo_actor_p->checkEnable(1)) {
            int sp1C;
            int sp18;
            int sp14;
            int sp10;
            int spC;
            u16 sp8;
            while (daAlink_c_getDemoIDData(demo_actor_p, &sp1C, &sp18, &sp14, &sp8, &sp10, &spC)) {
                if (sp10 != 0 && dDemo_c::getBranchId() == -1) {
                    dDemo_c::setBranchId(1, 0);
                }

                if (sp1C == 0) {
                    if (sp18 == 5) {
                        field_0x2f93 = setDemoRightHandIndex(sp8);
                    } else if (sp18 == 6) {
                        field_0x2f92 = setDemoLeftHandIndex(sp8);
                    } else if (sp18 == 9) {
                        if (sp8 == 1 && mpDemoFCBlendModel != NULL && mpDemoFCTongueModel != NULL) {
                            onNoResetFlg1(FLG1_UNK_10);
                            var_r29 = 1;
                        } else {
                            endHighModel();
                        }
                    } else if (sp18 == 7) {
                        if (sp8 == 1) {
                            field_0x2fab |= 2;
                        }
                    } else if (sp18 == 8) {
                        if (sp8 == 1) {
                            field_0x2fab |= 1;
                        }
                    } else if (sp18 == 0) {
                        if (sp14 == 1) {
                            field_0x3198 = sp8;
                        } else if (sp14 == 2) {
                            setDemoRide(sp8);
                        } else if (sp14 == 4 &&
                                   (mProcVar2.field_0x300c == 0 || (sp8 != 0 && mProcVar2.field_0x300c == 1) ||
                                    (sp8 == 0 && mProcVar2.field_0x300c == 2)))
                        {
                            if (mProcVar2.field_0x300c == 0) {
                                changeWarpMaterial(WARP_MAT_MODE_0);
                            }

                            if (sp8 != 0) {
                                field_0x3484 = 1.0f;
                                field_0x3480 = -0.5f;

                                if (checkWolf()) {
                                    field_0x347c = 3.5f;
                                } else {
                                    field_0x347c = 5.5f;
                                }

                                mProcVar2.field_0x300c = 2;
                            } else {
                                field_0x3484 = 0.0f;
                                tevStr.field_0x344 = 0.0f;
                                field_0x347c = -0.5f;

                                if (checkWolf()) {
                                    field_0x3480 = 3.5f;
                                } else {
                                    field_0x3480 = 5.5f;
                                }

                                mProcVar2.field_0x300c = 1;
                            }
                        }
                    }
                } else if (sp1C == 1) {
                    JUT_ASSERT(2345, FALSE);
                } else if (sp1C == 2) {
                    if (sp18 == 1) {
                        if (sp14 == 2) {
                            var_f31 =
                                setStickAnmData(getNowAnmPackUnder(UNDER_0), sp18, sp14, sp8, sp10);
                        } else if (sp14 == 4) {
                            J3DAnmTevRegKey* btk_p = (J3DAnmTevRegKey*)dComIfG_getObjectIDRes(
                                dStage_roomControl_c::getDemoArcName(), sp8);
                            if (btk_p != NULL && field_0x06f4 != btk_p) {
                                btk_p->setFrame(0.0f);
                                field_0x06f4 = btk_p;
                            }

                            if (btk_p != NULL && spC == 0) {
                                btk_p->setFrame(var_f31);
                            }
                        }
                    } else if (sp18 == 4) {
                        if (sp14 == 2) {
                            setStickAnmData(NULL, sp18, sp14, sp8, sp10);

                            if (mpDemoHDTmpBck != NULL && spC == 0) {
                                mpDemoHDTmpBck->setFrame(var_f31);
                            }
                        }
                    } else if (sp18 == 6) {
                        if (sp14 == 2) {
                            setStickAnmData(NULL, sp18, sp14, sp8, sp10);

                            if (mpDemoHLTmpBck != NULL && spC == 0) {
                                mpDemoHLTmpBck->setFrame(var_f31);
                            }
                        } else if (sp14 == 4 && field_0x2f92 == 0xFB) {
                            setDemoBrk(&field_0x06f8, mpDemoHLTmpModel, sp8);
                            if (field_0x06f8 != NULL && spC == 0) {
                                field_0x06f8->setFrame(var_f31);
                            }
                        }
                    } else if (sp18 == 5) {
                        if (sp14 == 2) {
                            setStickAnmData(NULL, sp18, sp14, sp8, sp10);

                            if (mpDemoHRTmpBck != NULL && spC == 0) {
                                mpDemoHRTmpBck->setFrame(var_f31);
                            }
                        }
                    } else if (sp18 == 9) {
                        if (sp14 == 2) {
                            setStickAnmData(field_0x2164.getBckAnm(), sp18, sp14, sp8, sp10);

                            if (mpDemoFCTmpBck != NULL && spC == 0) {
                                mpDemoFCTmpBck->setFrame(var_f31);
                            }
                        } else if (sp14 == 3) {
                            setStickAnmData(mpFaceBtk, sp18, sp14, sp8, sp10);

                            if (mpFaceBtk != NULL && spC == 0) {
                                mpFaceBtk->setFrame(var_f31);
                            }
                        } else if (sp14 == 5) {
                            setStickAnmData(field_0x2154, sp18, sp14, sp8, sp10);

                            if (field_0x2154 != NULL && spC == 0) {
                                field_0x2154->setFrame(var_f31);
                            }
                        } else if (sp14 == 7) {
                            J3DAnmCluster* blk_p = (J3DAnmCluster*)dComIfG_getObjectIDRes(
                                dStage_roomControl_c::getDemoArcName(), sp8);
                            if (field_0x0698 != blk_p) {
                                field_0x0698 = blk_p;
                                field_0x069c->init(mpDemoFCTmpBls, field_0x0698, FALSE,
                                                   J3DFrameCtrl::EMode_LOOP, 1.0f, 0, -1);
                                field_0x069c->getBlkAnm()->setFrame(0.0f);
                            }

                            if (spC == 0) {
                                if (field_0x069c != NULL && field_0x069c->getBlkAnm() != NULL) {
                                    field_0x069c->getBlkAnm()->setFrame(var_f31);
                                }
                            }
                        }
                    }
                }
            }
        }

        if (demo_actor_p->checkEnable(0x40)) {
            getNowAnmPackUnder(UNDER_0)->setFrame(var_f31);
            demo_actor_p->setAnmFrameMax(getNowAnmPackUnder(UNDER_0)->getFrameMax());
            mUnderFrameCtrl[0].setFrame(var_f31);
        }
    }

    if (mProcVar2.field_0x300c != 0) {
        warpModelTexScroll();
    }

    if (var_r29 == 0) {
        endHighModel();
    }

    if (demo_actor_p == NULL) {
        checkNextAction(0);
    }

    return 1;
}


int daAlink_c::procCoTalkInit() {
    if (mProcID == PROC_TALK) {
        return 0;
    }

    int var_r30;
    if (mProcID == PROC_WOLF_LIE_START) {
        var_r30 = 1;
    } else if (mProcID == PROC_WOLF_LIE_MOVE) {
        var_r30 = 2;
    } else {
        var_r30 = 0;
    }

    commonProcInit(PROC_TALK);

    if (mDemo.getDemoMode() == daPy_demo_c::DEMO_UNK_6_e) {
        if (mEquipItem != fpcNm_ITEM_KANTERA && mEquipItem != fpcNm_ITEM_NONE) {
            allUnequip(1);
        }
    }

    if (checkWolf()) {
        field_0x3198 = 0;
        mNormalSpeed = 0.0f;

        if (var_r30 == 0) {
            setBlendWolfMoveAnime(mpHIO->mWolf.mWlMove.m.mIdleInterpolation);
        } else {
            if (var_r30 == 1) {
                field_0x2f99 = 13;
                setSingleAnimeWolfBaseSpeed(WANM_CROUCH_WALK, 0.0f,
                                            mpHIO->mWolf.mWlLie.m.mProneMoveInterp);
            } else {
                field_0x2f99 = 13;
                daPy_frameCtrl_c* frameCtrl_p = mUnderFrameCtrl;

                if (fabsf(frameCtrl_p->getFrame() - 15.0f) / 15.0f < 0.5f) {
                    frameCtrl_p->setFrame(15.0f);
                } else {
                    frameCtrl_p->setFrame(0.0f);
                }

                frameCtrl_p->setRate(0.0f);
                getNowAnmPackUnder(UNDER_0)->setFrame(frameCtrl_p->getFrame());
                field_0x2060->initOldFrameMorf(3.0f, 0, 40);
            }

            dComIfGp_setPlayerStatus0(0, 0x8000000);
        }
    } else {
        field_0x3198 = setTalkStartBack(&field_0x37c8);
        if (!field_0x3198) {
            setTalkAnime();
            mNormalSpeed = 0.0f;
        }
    }

    mProcVar0.field_0x3008 = 30;
    dComIfGp_setPlayerStatus0(0, 0x10);
    return 1;
}


int daAlink_c::procCoTalk() {
    if (dComIfGp_checkPlayerStatus0(0, 0x8000000)) {
        field_0x2f99 = 13;
    }

    setShapeAngleToTalkActor();

    if (field_0x3198 != 0) {
        cXyz sp24 = field_0x37c8 - current.pos;
        int var_r30 = 0;

        if (mLinkAcch.ChkWallHit()) {
            dBgS_AcchCir* acchCir_p = mAcchCir;
            for (int i = 0; i < 3; i++) {
                if (acchCir_p->ChkWallHit()) {
                    var_r30 = cLib_distanceAngleS(acchCir_p->GetWallAngleY(), current.angle.y);
                    break;
                }
                acchCir_p++;
            }
        }

        if (sp24.absXZ() < 0.001f ||
            cLib_distanceAngleS(sp24.atan2sX_Z(), current.angle.y) > 0x4000 || var_r30 > 0x4000)
        {
            current.pos.x = field_0x37c8.x;
            current.pos.z = field_0x37c8.z;
            field_0x3198 = 0;
            setTalkAnime();
            mNormalSpeed = 0.0f;
        } else if (mProcVar0.field_0x3008 != 0) {
            mProcVar0.field_0x3008--;
        } else {
            field_0x3198 = 0;
            setTalkAnime();
            mNormalSpeed = 0.0f;
        }
    } else {
        if (mEquipItem == fpcNm_ITEM_KANTERA) {
            if (mUpperAnmHeap[0].getIdx() != getMainBckData(ANM_WAIT)->m_upperID) {
                setSingleAnimeBaseSpeed(ANM_WAIT, mpHIO->mMove.m.mWaitAnmSpeed, 3.0f);
            }
        }

        current.angle.y = shape_angle.y;
    }

    return 1;
}


int daAlink_c::procCoOpenTreasureInit() {
    if (!commonProcInitNotSameProc(PROC_OPEN_TREASURE)) {
        return 1;
    }

    f32 var_f31;
    if (checkWolf()) {
        setSingleAnimeWolfBase(WANM_TRES_OPEN);
        if (mDemo.getParam0() != 1) {
            mUnderFrameCtrl[0].setEnd(21);
        }

        var_f31 = 130.0f;
        field_0x3588 = l_wolfBaseAnime;
    } else {
        daAlink_ANM open_anm;
        if (mDemo.getParam0() == 2) {
            open_anm = ANM_TRES_OPEN_BIG;
            var_f31 = 111.0f;
            mDoAud_subBgmStart(Z2BGM_OPEN_BOX);
        } else {
            var_f31 = 88.0f;

            if (mDemo.getParam0() == 1) {
                open_anm = ANM_TRES_OPEN_KICK;
            } else {
                open_anm = ANM_TRES_OPEN_SMALL;
            }
        }

        setSingleAnimeBase(open_anm);
        field_0x3588 = l_waitBaseAnime;
    }

    mNormalSpeed = 0.0f;

    fopAc_ac_c* partner_p = fopAcM_getEventPartner(this);
    if (partner_p != NULL) {
        shape_angle.y = partner_p->shape_angle.y + 0x8000;
        current.angle.y = shape_angle.y;

        current.pos.x = partner_p->current.pos.x - var_f31 * cM_ssin(shape_angle.y);
        current.pos.z = partner_p->current.pos.z - var_f31 * cM_scos(shape_angle.y);
        current.pos.y = partner_p->current.pos.y;
    }

    if (!checkWolf()) {
        deleteEquipItem(FALSE, FALSE);
        mDoMtx_stack_c::transS(current.pos);
        mDoMtx_stack_c::YrotM(shape_angle.y);
        mDoMtx_stack_c::transM(60.0f, 45.0f, 30.0f);
        mpKanteraModel->setBaseTRMtx(mDoMtx_stack_c::get());
    }

    mProcVar2.field_0x300c = 0;
    mLinkAcch.SetWallNone();
    mLinkAcch.OffLineCheck();
    onEndResetFlg1(ERFLG1_UNK_8);
    return 1;
}


int daAlink_c::procCoOpenTreasure() {
    onEndResetFlg1(ERFLG1_UNK_8);

    if (mProcVar2.field_0x300c == 0) {
        mProcVar2.field_0x300c = 1;
    } else {
        field_0x2f99 = 4;
    }

    if (checkAnmEnd(mUnderFrameCtrl)) {
        dComIfGp_evmng_cutEnd(mAlinkStaffId);
        mLinkAcch.ClrWallNone();
        mLinkAcch.OnLineCheck();
    }

    return 1;
}

int daAlink_c::procCoUnequipInit() {
    if (!commonProcInitNotSameProc(PROC_UNEQUIP)) {
        return 0;
    }

    mNormalSpeed = 0.0f;
    mProcVar2.field_0x300c = 0;
    mProcVar3.field_0x300e = 0;

    if (checkWolf()) {
        setBlendWolfMoveAnime(mpHIO->mWolf.mWlMove.m.mNormalInterpolation);
    } else {
        setBlendMoveAnime(mpHIO->mBasic.m.mBasicInterpolation);

        if (mDemo.getParam1() == 0) {
            if (mEquipItem != fpcNm_ITEM_NONE) {
                allUnequip(0);
            }
        } else if (mDemo.getParam1() == 1) {
            if (mEquipItem != 0x103) {
                swordEquip(0);
            }
        } else if (mDemo.getParam1() == 2) {
            mProcVar2.field_0x300c = 1;
            mProcVar3.field_0x300e = 1;
            setSingleAnime(ANM_TRADE_ITEM_PULL_OUT, 1.0f, 0.0f, 39, 3.0f);
        }
    }

    return 1;
}


int daAlink_c::procCoUnequip() {
    if (mProcVar2.field_0x300c != 0) {
        if (checkAnmEnd(mUnderFrameCtrl)) {
            setBlendMoveAnime(3.0f);
            itemEquip(fpcNm_ITEM_COPY_ROD);

            f32 tmp_6 = 6.0f;
            mUpperFrameCtrl[2].setFrame(tmp_6);
            getNowAnmPackUpper(UPPER_2)->setFrame(tmp_6);
            commonChangeItem();
            mProcVar2.field_0x300c = 0;
        }
    } else {
        if (!checkWolf()) {
            setBlendMoveAnime(-1.0f);
        }

        if (checkNoUpperAnime()) {
            dComIfGp_evmng_cutEnd(mAlinkStaffId);
        }
    }

    return 1;
}

void daAlink_c::setGetSubBgm(int i_seTypeIdx) {
    static u8 const getSeType[255] = {
        0x08, 0x03, 0x03, 0x03, 0x03, 0x03, 0x02, 0x02, 0x08, 0x08, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
        0x03, 0x03, 0x03, 0x08, 0x08, 0x08, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x02, 0x08,
        0x02, 0x02, 0x00, 0x01, 0x01, 0x02, 0x01, 0x08, 0x01, 0x08, 0x01, 0x01, 0x01, 0x08, 0x08, 0x08,
        0x01, 0x01, 0x08, 0x02, 0x08, 0x01, 0x01, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x01, 0x01, 0x02,
        0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x08, 0x08, 0x01,
        0x01, 0x01, 0x08, 0x08, 0x08, 0x01, 0x01, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,
        0x01, 0x02, 0x02, 0x02, 0x02, 0x01, 0x02, 0x08, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,
        0x08, 0x08, 0x08, 0x01, 0x02, 0x01, 0x02, 0x01, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,
        0x02, 0x02, 0x02, 0x02, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,
        0x02, 0x02, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x02, 0x01, 0x02, 0x02,
        0x08, 0x01, 0x01, 0x01, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,
        0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,
        0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04,
        0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,
        0x06, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x02, 0x02, 0x01, 0x02, 0x02, 0x02, 0x08,
        0x08, 0x08, 0x08, 0x02, 0x03, 0x03, 0x01, 0x08, 0x03, 0x02, 0x02, 0x02, 0x02, 0x01, 0x02,
    };

    static u32 const bgmLabel[8] = {
        Z2BGM_HEART_GET,       Z2BGM_ITEM_GET,       Z2BGM_ITEM_GET_MINI, Z2BGM_ITEM_GET_ME,
        Z2BGM_ITEM_GET_INSECT, Z2BGM_ITEM_GET_SMELL, Z2BGM_ITEM_GET_POU,  Z2BGM_ITEM_GET_ME_S,
    };

    u32 se_type = getSeType[i_seTypeIdx];

    if (se_type == 3 && mProcVar4.field_0x3010 == 0) {
        se_type = 7;
    }

    if (se_type != 8) {
        mDoAud_subBgmStart(bgmLabel[se_type]);
        dComIfGp_setMesgBgmOn();
    }
}

BOOL daAlink_c::checkTreasureRupeeReturn(int i_itemNo) const {
    if (i_itemNo == fpcNm_ITEM_LINKS_SAVINGS) {
        i_itemNo = fpcNm_ITEM_PURPLE_RUPEE;
    }

    i_itemNo -= 4;
    if (i_itemNo < 0 || i_itemNo >= 4) {
        return false;
    }

    static int const rupeeCount[4] = {20, 50, 100, 200};
    return dComIfGs_getRupeeMax() < rupeeCount[i_itemNo] + dComIfGs_getRupee();
}

int daAlink_c::procCoGetItemInit() {
    BOOL var_r31 = 0;
    BOOL var_r30 = 0;
    if (mProcID == PROC_GET_ITEM || mProcID == PROC_INSECT_CATCH ||
        (mProcID == PROC_PREACTION_UNEQUIP && !checkNoUpperAnime()))
    {
        return 1;
    }

    if (mProcID == PROC_OPEN_TREASURE || mProcID == PROC_GET_READY_SIT) {
        if (mProcID == PROC_OPEN_TREASURE) {
            var_r31 = 1;
        }
        var_r30 = 1;
    }

    mLinkAcch.ClrWallNone();

    if (!checkWolf() && mEquipItem != fpcNm_ITEM_NONE && var_r30 == 0) {
        if (checkModeFlg(0x400) || (checkStageName("D_MN05B") && mEquipItem == fpcNm_ITEM_BOOMERANG &&
                                      !dComIfGs_isItemFirstBit(fpcNm_ITEM_BOOMERANG)))
        {
            deleteEquipItem(0, 0);
        } else {
            return procPreActionUnequipInit(PROC_GET_ITEM, NULL);
        }
    }

    commonProcInit(PROC_GET_ITEM);
    mNormalSpeed = 0.0f;
    mProcVar1.field_0x300a = 0;
    field_0x3478 = 0.0f;

    if (mDemo.getParam0() != 0) {
        int item_no;
        if (mDemo.getParam0() != 0x100) {
            item_no = mDemo.getParam0();
        } else {
            item_no = dComIfGp_event_getGtItm();
        }

        fpc_ProcID item_partner_id = fopAcM_createItemForPresentDemo(&current.pos, item_no, 0, -1,
                                                              fopAcM_GetRoomNo(this), NULL, NULL);
        if (item_partner_id != fpcM_ERROR_PROCESS_ID_e) {
            dComIfGp_event_setItemPartnerId(item_partner_id);
        }

        mProcVar2.field_0x300c = item_no;
    } else {
        daItemBase_c* item_partner_p = (daItemBase_c*)fopAcM_getItemEventPartner(this);
        if (item_partner_p != NULL) {
            mProcVar2.field_0x300c = item_partner_p->getItemNo();
        } else {
            mProcVar2.field_0x300c = -1;
        }
    }

    if (checkWolf()) {
        if (var_r30 != 0) {
            setSingleAnimeWolfBase(WANM_GET_A);
        } else {
            setSingleAnimeWolf(WANM_GET_A, 1.0f, 10.0f, -1, 3.0f);
        }

        field_0x2f99 = 12;
        field_0x3588 = l_wolfBaseAnime;
        field_0x347c = 10.0f;
        field_0x3480 = 19.0f;
        field_0x3484 = 0.11111111f;
    } else if (checkModeFlg(0x400)) {
        setSingleAnimeBase(ANM_HORSE_GET_ITEM);

        if (mProcVar2.field_0x300c != -1) {
            setGetItemFace(mProcVar2.field_0x300c);
        }

        if (checkReinRide()) {
            setSyncRidePos();
            if (checkBoarRide()) {
                setUnderAnime(0x26E, UNDER_2, 1.0f, 0.0f, -1, -1.0f);
            }
        }
        field_0x3478 = 9.0f;
        mProcVar1.field_0x300a = 1;
    } else {
        if (var_r30 != 0) {
            setSingleAnimeBase(ANM_GET_A);
        } else {
            setSingleAnimeBase(ANM_GET_A_WAIT);

            if (mProcVar2.field_0x300c != -1) {
                setGetItemFace(mProcVar2.field_0x300c);
            }
            mProcVar1.field_0x300a = -7;
        }

        field_0x347c = 9.0f;
        field_0x3480 = 16.0f;
        field_0x3484 = 0.14285715f;
        field_0x2f99 = 0xC;
        field_0x3588 = l_waitBaseAnime;
    }

    mProcVar4.field_0x3010 = 0;

    if (var_r31 != 0) {
        mProcVar3.field_0x300e = -0x8000;
        mProcVar4.field_0x3010 = 1;
    } else if (mDemo.getParam1() == 2) {
        mProcVar3.field_0x300e = 0;
    } else {
        mProcVar3.field_0x300e =
            shape_angle.y - (fopCamM_GetAngleY(dComIfGp_getCamera(field_0x317c)) + 0x8000);
        s16 temp_r28 = mProcVar3.field_0x300e + 0x16C1;
        s16 temp_r31 = mProcVar3.field_0x300e - 0x16C1;

        if (abs(temp_r28) > abs(temp_r31)) {
            mProcVar3.field_0x300e = temp_r31;
        } else {
            mProcVar3.field_0x300e = temp_r28;
        }
    }

    current.angle.y = shape_angle.y;
    mMsgClassID = fpcM_ERROR_PROCESS_ID_e;
    mProcVar0.field_0x3008 = 0;
    field_0x3198 = 0;
    field_0x32cc = 0;
    mProcVar5.field_0x3012 = 0;
    dKy_Itemgetcol_chg_on();

    dComIfGp_setPlayerStatus1(0, 0x4000000);
    if (mProcVar4.field_0x3010 != 0) {
        onEndResetFlg1(ERFLG1_UNK_8);
    }

    return 1;
}


int daAlink_c::procCoGetItem() {
    if (mProcVar4.field_0x3010 != 0) {
        onEndResetFlg1(ERFLG1_UNK_8);
    }

    if (field_0x3198 != 0) {
        if (checkModeFlg(0x400) && checkReinRide()) {
            setSyncRidePos();
        }

        dComIfGp_evmng_cutEnd(mAlinkStaffId);
        return 1;
    }

    daPy_frameCtrl_c* frameCtrl_p = mUnderFrameCtrl;

    if (checkModeFlg(0x400)) {
        if (checkReinRide()) {
            setSyncRidePos();
        }
    } else if (field_0x2f99 != 12) {
        field_0x2f99 = 4;
    }

    if (mProcVar1.field_0x300a == 0 && !(frameCtrl_p->getFrame() < field_0x347c)) {
        if (frameCtrl_p->getFrame() < field_0x3480) {
            shape_angle.y =
                current.angle.y -
                (field_0x3484 * (mProcVar3.field_0x300e * (frameCtrl_p->getFrame() - field_0x347c)));

            if (mProcVar4.field_0x3010 != 0 && checkWolf()) {
                current.pos.x -= cM_ssin(current.angle.y) * 3.8888888f;
                current.pos.z -= cM_scos(current.angle.y) * 3.8888888f;
            }
        } else {
            shape_angle.y = current.angle.y - mProcVar3.field_0x300e;
        }
    }

    daItemBase_c* item_partner_p = (daItemBase_c*)fopAcM_getItemEventPartner(this);
#ifdef DEBUG
    if (item_partner_p != NULL && fopAcM_GetName(item_partner_p) != PROC_ITEM &&
        fopAcM_GetName(item_partner_p) != PROC_Demo_Item)
    {
        // "Get Item is not an item!!! %d\n"
        OSReport("ゲットするアイテムがアイテムじゃない！！！　%d\n",
                 fopAcM_GetName(item_partner_p));
        JUT_ASSERT(3415, FALSE);
    }
#endif

    if (mProcVar2.field_0x300c == -1 && item_partner_p != NULL) {
        mProcVar2.field_0x300c = item_partner_p->getItemNo();

        if (!checkWolf() && (checkUnderMove0BckNoArc(ANM_GET_A_WAIT) || checkModeFlg(0x400))) {
            setGetItemFace(mProcVar2.field_0x300c);
        }
    }

    field_0x3478 += 1.0f;
    if (field_0x3478 > 100.0f) {
        field_0x3478 = 100.0f;
    }

    if (field_0x3478 >= 9.0f && mProcVar0.field_0x3008 == 0 && mProcVar2.field_0x300c != -1) {
        mProcVar0.field_0x3008 = 1;

        int get_bgm_type = mProcVar2.field_0x300c;
        if (mProcVar2.field_0x300c == 33 && (dComIfGs_getMaxLife() % 5) == 4) {
            get_bgm_type = 34;
        }

        setGetSubBgm(get_bgm_type);
    }

    BOOL var_r27 = true;
    if (((mProcVar1.field_0x300a == 0 && frameCtrl_p->getFrame() >= (f32)frameCtrl_p->getEnd() - 5.0f) ||
         mProcVar1.field_0x300a != 0) &&
        item_partner_p != NULL)
    {
        field_0x280c.setData(item_partner_p);
        item_partner_p->show();
        var_r27 = false;
    }

    if (checkAnmEnd(frameCtrl_p)) {
        mProcVar1.field_0x300a = 1;

        if (checkWolf()) {
            setSingleAnimeWolfBase(WANM_GET);
        } else {
            setSingleAnimeBase(ANM_GET_A_WAIT);
            setGetItemFace(mProcVar2.field_0x300c);
        }

        field_0x2f99 = 12;
        if (mDemo.getParam1() == 1) {
            dComIfGp_evmng_cutEnd(mAlinkStaffId);
        }
    } else if (mProcVar1.field_0x300a != 0) {
        if (mDemo.getParam1() == 1 && checkModeFlg(0x400)) {
            dComIfGp_evmng_cutEnd(mAlinkStaffId);
        }

        if (mProcVar1.field_0x300a < 0) {
            mProcVar1.field_0x300a++;

            if (mProcVar1.field_0x300a == 0) {
                mProcVar1.field_0x300a = 1;
                shape_angle.y = current.angle.y - mProcVar3.field_0x300e;

                if (mDemo.getParam1() == 1) {
                    dComIfGp_evmng_cutEnd(mAlinkStaffId);
                }
            } else {
                shape_angle.y =
                    current.angle.y - (mProcVar3.field_0x300e * (mProcVar1.field_0x300a + 7)) * (1.0f / 7.0f);
            }
        }

        if (item_partner_p != NULL) {
            if (!fpcM_IsCreating(fpcM_GetID(item_partner_p))) {
                if (field_0x32cc == 0 && mProcVar2.field_0x300c != -1) {
                    if (mProcVar2.field_0x300c == 0x70 && checkStageName("F_SP115")) {
                        field_0x32cc = 0x6E;
                    } else if (mProcVar2.field_0x300c == 0x23 && checkStageName("D_MN11")) {
                        field_0x32cc = 0x5C0;
                    } else if (mProcVar2.field_0x300c == 0xE0) {
                        if (dComIfGs_getPohSpiritNum() == 20) {
                            field_0x32cc = 0x4CF;
                        } else if (dComIfGs_getPohSpiritNum() == 60) {
                            field_0x32cc = 0x4D0;
                        } else {
                            field_0x32cc = mProcVar2.field_0x300c + 0x65;
                        }
                    } else if (mProcVar2.field_0x300c == 0x21) {
                        static u32 const heartPieceMessage[5] = {0x86, 0x9C, 0x9D, 0x9E, 0x9F};
                        field_0x32cc = heartPieceMessage[dComIfGs_getMaxLife() % 5];
                    } else if (mProcVar2.field_0x300c == 0x33 && checkStageName("D_MN07")) {
                        field_0x32cc = 0x151;
                    } else {
                        field_0x32cc = mProcVar2.field_0x300c + 0x65;
                    }
                }

                if (field_0x32cc != 0 && checkEndMessage(field_0x32cc)) {
                    if (field_0x32cc < 0x164) {
                        if (mProcVar2.field_0x300c >= 0xC0 && mProcVar2.field_0x300c <= 0xD7 &&
                            dComIfGs_isItemFirstBit(mProcVar2.field_0x300c))
                        {
                            field_0x32cc += 2000;
                            mMsgClassID = fpcM_ERROR_PROCESS_ID_e;
                            return 1;
                        }

                        if (mProcVar4.field_0x3010 != 0 && checkTreasureRupeeReturn(mProcVar2.field_0x300c)) {
                            if (mProcVar2.field_0x300c == 0xED) {
                                field_0x32cc = 0x6A;
                            }

                            field_0x32cc += 0x5DC;
                            mMsgClassID = fpcM_ERROR_PROCESS_ID_e;
                            return 1;
                        }
                    }

                    dComIfGp_evmng_cutEnd(mAlinkStaffId);
                    item_partner_p->dead();
                    field_0x3198 = 1;
                    field_0x2f99 = 12;

                    if (checkWolf()) {
                        setBlendWolfMoveAnime(mpHIO->mWolf.mWlMove.m.mNormalInterpolation);
                    } else {
                        if (mEquipItem == fpcNm_ITEM_KANTERA) {
                            field_0x2f94 = 0;
                        }

                        current.angle.y = shape_angle.y;
                        if (checkModeFlg(0x400)) {
                            setDoubleAnime(0.0f, 1.0f, 1.0f, ANM_HORSE_WAIT, ANM_HORSE_WAIT, 10,
                                           3.0f);
                            field_0x2f99 = 0x60;
                        } else {
                            setBlendMoveAnime(mpHIO->mBasic.m.mBasicInterpolation);
                        }
                    }
                }
            }
        }
    } else if (var_r27 && item_partner_p != NULL) {
        item_partner_p->hide();
    }

    return 1;
}


int daAlink_c::procCoTurnBackInit() {
    if (!commonProcInitNotSameProc(PROC_TURN_BACK)) {
        return 0;
    }

    mNormalSpeed = 0.0f;
    if (checkWolf()) {
        setSingleAnimeWolfBase(WANM_TURN_BACK);
    } else {
        setSingleAnimeBase(ANM_TURN_BACK);
    }

    mProcVar2.field_0x300c = 0;
    return 1;
}

int daAlink_c::procCoTurnBack() {
    daPy_frameCtrl_c* frameCtrl_p = mUnderFrameCtrl;

    if (checkAnmEnd(frameCtrl_p)) {
        if (mProcVar2.field_0x300c == 0) {
            if (checkWolf()) {
                setBlendWolfMoveAnime(5.0f);
            } else {
                setDoubleAnime(0.0f, mpHIO->mNoActAtnMove.m.mWaitAnmSpeed,
                               mpHIO->mNoActAtnMove.m.mWaitAnmSpeed, ANM_WAIT_B, ANM_WAIT_B,
                               2, 3.0f);
            }

            mProcVar2.field_0x300c = 1;
        }

        dComIfGp_evmng_cutEnd(mAlinkStaffId);
    } else if (mProcVar2.field_0x300c != 0) {
        dComIfGp_evmng_cutEnd(mAlinkStaffId);
    } else if (!checkWolf() && frameCtrl_p->getFrame() >= 29.0f) {
        field_0x2f92 = 1;
        field_0x2f93 = 6;
    }

    return 1;
}


int daAlink_c::procDoorOpenInit() {
    if (!commonProcInitNotSameProc(PROC_DOOR_OPEN)) {
        return 1;
    }

    daAlink_ANM open_anm;
    f32 start_f = 0.0f;
    s16 end_f = -1;

    field_0x3478 = mpHIO->mBasic.m.mBasicInterpolation;
    mProcVar0.field_0x3008 = 0;

    if (mDemo.getParam0() == 4) {
        open_anm = ANM_DOOR_OPEN_ROLL;
        mProcVar2.field_0x300c = 1;
        mProcVar3.field_0x300e = 1;
        mProcVar4.field_0x3010 = 0;
    } else if (mDemo.getParam0() == 5) {
        open_anm = ANM_DOOR_OPEN_SLIDE;
        mProcVar2.field_0x300c = 1;
        mProcVar3.field_0x300e = 1;
        mProcVar4.field_0x3010 = 0;
    } else {
        mProcVar3.field_0x300e = 0;

        if (mDemo.getParam0() & 1) {
            open_anm = ANM_DOOR_OPEN_RIGHT;
            field_0x347c = 42.0f;
        } else {
            open_anm = ANM_DOOR_OPEN_LEFT;
            field_0x347c = 46.0f;
        }

        if (mDemo.getParam0() & 4) {
            field_0x3478 = 10.0f;
            mProcVar0.field_0x3008 = 10;

            if (open_anm == ANM_DOOR_OPEN_LEFT) {
                end_f = 14;
            } else {
                end_f = 18;
            }
        } else if (mDemo.getParam0() & 2) {
            start_f = 38.0f;
            field_0x2f99 = 5;
            field_0x34d4 = l_waitBaseAnime;
        }

        mProcVar4.field_0x3010 = 1;
        mProcVar2.field_0x300c = 0;
    }

    setSingleAnime(open_anm, 1.0f, start_f, end_f, 0.0f);
    mNormalSpeed = 0.0f;

    mLinkAcch.SetWallNone();
    mLinkAcch.OnLineCheckNone();
    field_0x3588 = l_waitBaseAnime;
    field_0x3198 = 0;
    mProcVar5.field_0x3012 = 0;
    return 1;
}


int daAlink_c::procDoorOpen() {
    daPy_frameCtrl_c* frameCtrl_p = mUnderFrameCtrl;

    if (field_0x3198 == 0) {
        if (mProcVar2.field_0x300c == 0 && mProcVar4.field_0x3010 != 0) {
            if (frameCtrl_p->getFrame() >= 9.0f && frameCtrl_p->getFrame() < field_0x347c) {
                field_0x2f92 = 3;
            } else {
                field_0x2f92 = 4;
            }
        } else if (frameCtrl_p->checkPass(18.0f) && mProcVar3.field_0x300e != 0) {
            voiceStart(Z2SE_AL_V_PUSH_ROCK);
            mProcVar3.field_0x300e = 0;
        }
    }

    if (checkAnmEnd(frameCtrl_p)) {
        if (mProcVar0.field_0x3008 != 0) {
            daAlink_ANM open_anm;
            if (mDemo.getParam0() & 1) {
                open_anm = ANM_DOOR_OPEN_LOCK_RIGHT;
            } else {
                open_anm = ANM_DOOR_OPEN_LOCK_LEFT;
            }

            seStartMapInfo(Z2SE_OBJ_KNOB_DOOR_LOCKED);
            setSingleAnimeBaseMorf(open_anm, 0.0f);
            mProcVar0.field_0x3008 = 0;
            field_0x2f99 = 5;
            mProcVar4.field_0x3010 = 0;
        } else {
            mLinkAcch.ClrWallNone();
            mLinkAcch.OffLineCheckNone();
            dComIfGp_evmng_cutEnd(mAlinkStaffId);
            onModeFlg(1);
            offModeFlg(0x4000);

            if (mProcVar2.field_0x300c == 0) {
                setBlendMoveAnime(field_0x3478);
            }
            field_0x3198 = 1;
        }
    } else if (field_0x3198 == 0) {
        if (mProcVar2.field_0x300c == 0) {
            if (mProcVar5.field_0x3012 == 0) {
                mProcVar5.field_0x3012 = 1;
            } else {
                field_0x2f99 = 5;
            }
        }
    } else {
        dComIfGp_evmng_cutEnd(mAlinkStaffId);
    }

    return 1;
}


int daAlink_c::procCoLookWaitInit() {
    if (!commonProcInitNotSameProc(PROC_LOOK_WAIT)) {
        return 0;
    }

    onNoResetFlg0(FLG0_UNK_100000);

    if (mGrabItemAcKeep.getActor() != NULL && (checkGrabAnime() || checkWolfGrabAnime())) {
        onModeFlg(0x100000);

        if (!checkWolf()) {
            setWaterInAnmRate(&mUpperFrameCtrl[2], 1.0f);
        }
    }

    mNormalSpeed = 0.0f;

    if (checkWolf()) {
        setBlendWolfMoveAnime(mpHIO->mWolf.mWlMove.m.mNormalInterpolation);
    } else {
        setBlendMoveAnime(mpHIO->mBasic.m.mBasicInterpolation);
    }

    current.angle.y = shape_angle.y;
    field_0x2f98 = 4;
    return 1;
}


int daAlink_c::procCoLookWait() {
    fopAc_ac_c* look_actor_p = getDemoLookActor();

    if (look_actor_p != NULL) {
        s16 target_angle;
        s16 svar1 = cLib_targetAngleY(&field_0x34e0, &look_actor_p->eyePos) - shape_angle.y;

        if (svar1 > 0x6000) {
            target_angle = shape_angle.y + 0x6000;
        } else if (svar1 < -0x6000) {
            target_angle = shape_angle.y + -0x6000;
        } else {
            target_angle = shape_angle.y;
        }

        cLib_addCalcAngleS(&shape_angle.y, target_angle, 2, 0x800, 0x100);
    }

    dComIfGp_evmng_cutEnd(mAlinkStaffId);
    return 1;
}

int daAlink_c::procCoDemoPushPullWaitInit() {
    if (!commonProcInitNotSameProc(PROC_DEMO_PUSH_PULL_WAIT)) {
        return 0;
    }

    if (checkWolf()) {
        setSingleAnimeWolfBase(WANM_WAIT_PP);
    } else {
        setSingleAnimeBaseSpeed(ANM_WAIT_PUSH_PULL, mpHIO->mPushpull.m.mStandbyASpeed,
                                mpHIO->mPushpull.m.mStandbyInterpolation);
    }

    mNormalSpeed = 0.0f;
    return 1;
}

int daAlink_c::procCoDemoPushMoveInit() {
    if (!commonProcInitNotSameProc(PROC_DEMO_PUSH_MOVE)) {
        return 0;
    }

    if (checkWolf()) {
        if (mDemo.getParam0() != 0) {
            setSingleAnimeWolfBase(WANM_PUSH);
        } else {
            setSingleAnimeWolf(WANM_PUSH_LIGHT, mpHIO->mWolf.mWlPush.m.mPushAnmSpeed,
                               0.0f, 24, mpHIO->mWolf.mWlPush.m.mPushInterp);
        }
    } else {
        if (mDemo.getParam0() != 0) {
            setSingleAnimeBase(ANM_PUSH);
        } else {
            setSingleAnimeBaseSpeed(ANM_PUSH_LIGHT, mpHIO->mPushpull.m.mPushASpeed,
                                    mpHIO->mPushpull.m.mPushInterpolation);
        }
    }

    mNormalSpeed = 0.0f;
    return 1;
}

void daAlink_c::setMonkeyMoveAnime() {
    daPy_frameCtrl_c* frame_ctrl = mUnderFrameCtrl;

    if (mDemo.getParam0() == 0) {
        field_0x2f92 = 0xFE;
        field_0x2f93 = 0xFE;
        frame_ctrl->setFrame(0.0f);
    } else {
        field_0x2f92 = 2;
        field_0x2f93 = 5;
        frame_ctrl->setFrame(1.0f);
    }

    getNowAnmPackUnder(UNDER_0)->setFrame(frame_ctrl->getFrame());
    field_0x3088 = mDemo.getParam1();
    shape_angle.x = field_0x3088 >> 1;  // divide by 2
}

int daAlink_c::procMonkeyMoveInit() {
    if (!commonProcInitNotSameProc(PROC_MONKEY_MOVE)) {
        return 0;
    }

    current.angle.y = shape_angle.y;
    setSpecialGravity(0.0f, maxFallSpeed, 0);

    speed.y = 0.0f;
    mNormalSpeed = 0.0f;
    deleteEquipItem(FALSE, FALSE);

    if (!checkWolf()) {
        setSingleAnimeBaseSpeed(ANM_ROPE_SWING, 0.0f, 3.0f);
        setMonkeyMoveAnime();
    }

    return 1;
}

int daAlink_c::procMonkeyMove() {
    dComIfGp_evmng_cutEnd(mAlinkStaffId);

    if (!checkWolf()) {
        setMonkeyMoveAnime();
    }
    return 1;
}

int daAlink_c::procDemoBoomerangCatchInit() {
    if (!commonProcInitNotSameProc(PROC_DEMO_BOOMERANG_CATCH)) {
        return 0;
    }

    mNormalSpeed = 0.0f;
    deleteEquipItem(FALSE, FALSE);
    mEquipItem = fpcNm_ITEM_BOOMERANG;
    setItemActor();
    setSingleAnimeParam(ANM_BOOMERANG_CATCH, &mpHIO->mItem.mBoomerang.m.mCatchAnm);
    return 1;
}

int daAlink_c::procDemoBoomerangCatch() {
    dComIfGp_evmng_cutEnd(mAlinkStaffId);

    if (checkAnmEnd(mUnderFrameCtrl)) {
        setBlendMoveAnime(mpHIO->mBasic.m.mBasicInterpolation);
    }
    return 1;
}

int daAlink_c::procCoDeadInit(int param_0) {
    if (!commonProcInitNotSameProc(PROC_DEAD)) {
        return 1;
    }

    dComIfGp_event_compulsory(this, NULL, 0xFFFF);
    mDemo.setSpecialDemoType();
    mNormalSpeed = 0.0f;
    mDamageTimer = 0;

    if (checkWolf()) {
        mProcVar4.field_0x3010 = 2;
    } else {
        mProcVar4.field_0x3010 = 2;
    }

    setDeadRideSyncPos();

    if (checkNoResetFlg2(FLG2_FORCE_GAMEOVER)) {
        if (param_0) {
            if (checkWolf()) {
                setSingleAnimeWolfBaseSpeed(WANM_WAIT, mpHIO->mWolf.mWlMove.m.mIdleAnmSpeed, 3.0f);
            } else {
                setSingleAnimeBaseSpeed(ANM_WAIT, mpHIO->mMove.m.mWaitAnmSpeed, 3.0f);
            }
        }

        field_0x3478 = 0.38f;
    } else {
        if (checkWolf()) {
            if (checkModeFlg(0x40000)) {
                setSingleAnimeWolfBase(WANM_SWIM_DIE);
                setFaceBasicTexture(FTANM_UNK_9D);
                dComIfGp_setPlayerStatus0(0, 0x100000);
                voiceStart(Z2SE_WL_V_DIE_WATER);
            } else {
                setSingleAnimeWolfBase(WANM_DIE);
                setFaceBasicTexture(FTANM_UNK_9C);
                voiceStart(Z2SE_WL_V_DIE);
            }
        } else {
            daAlink_ANM die_anm;

            if (checkModeFlg(0x400)) {
                if (checkReinRide()) {
                    die_anm = ANM_HORSE_DIE;

                    if (checkHorseUnderItemAnime()) {
                        resetUnderAnime(UNDER_2, -1.0f);
                    }

                    if (checkHorseRide()) {
                        daHorse_c* horse_p = dComIfGp_getHorseActor();
                        horse_p->changeOriginalDemo();
                        horse_p->changeDemoMode(5, 0);
                    } else {
                        mRideAcKeep.getActor()->speedF = 0.0f;
                    }
                }

                voiceStart(Z2SE_AL_V_DIE_SHORT);
            } else if (checkModeFlg(0x40000)) {
                die_anm = ANM_SWIM_DIE;
                dComIfGp_setPlayerStatus0(0, 0x100000);
                voiceStart(Z2SE_AL_V_DIE_SHORT);

                if (speed.y < 0.0f) {
                    speed.y = 0.0f;
                }
            } else {
                die_anm = ANM_DIE;
                onModeFlg(0x2000000);
                field_0x33cc = 0.0f;
                voiceStart(Z2SE_AL_V_DIE);
            }

            setSingleAnimeBase(die_anm);
        }

        field_0x3478 = 1.0f;
    }

    mProcVar3.field_0x300e = 0;
    mProcVar0.field_0x3008 = 0;
    dComIfGp_setPlayerStatus0(0, 0x20000000);
    mMsgClassID = 0;
    setFootEffectProcType(4);
    mProcVar2.field_0x300c = 0;
    field_0x3080 = 0;
    mProcVar5.field_0x3012 = 0x3F;
    field_0x3198 = -1;
    return 1;
}


int daAlink_c::procCoDead() {
    daPy_frameCtrl_c* frameCtrl_p = mUnderFrameCtrl;

    if (mProcVar0.field_0x3008 == 0) {
        setDeadRideSyncPos();
        mProcVar0.field_0x3008 = 1;
        return 1;
    }

    if (mMsgClassID == 0) {
        mMsgClassID = d_GameOver_Create(0);
        if (mMsgClassID == 0) {
            return 1;
        }

        mDoAud_bgmStart(Z2BGM_GAME_OVER);
    }

    if (!checkNoResetFlg2(FLG2_FORCE_GAMEOVER)) {
        mProcVar3.field_0x300e++;
        if (mProcVar3.field_0x300e == 2) {
            dCam_getBody()->StartEventCamera(9, fopAcM_GetID(this), 0);
        } else if (mProcVar3.field_0x300e > 10) {
            mProcVar3.field_0x300e = 10;
        }

        if (!checkModeFlg(0x400)) {
            if (checkModeFlg(0x40000)) {
                if (checkNoResetFlg0(FLG0_UNK_80) && current.pos.y > mWaterY) {
                    current.pos.y = mWaterY;
                    onNoResetFlg0(FLG0_SWIM_UP);
                }
            } else {
                field_0x33cc = (frameCtrl_p->getFrame() - 101.0f) / 19.0f;

                if ((!checkWolf() && frameCtrl_p->checkPass(122.0f)) ||
                    (checkWolf() && frameCtrl_p->checkPass(114.0f)))
                {
                    field_0x2f9d = 4;
                }
            }
        }
    }

    setDeadRideSyncPos();

    if (checkAnmEnd(frameCtrl_p) || ((checkModeFlg(0x40000) && frameCtrl_p->getFrame() > 70.0f) ||
                                     checkNoResetFlg2(FLG2_FORCE_GAMEOVER)))
    {
        cLib_chaseF(&field_0x3478, 0.0f, 0.01f);
        if (field_0x3478 < 0.38f) {
            d_GameOver_animeStart(mMsgClassID);
        }

        if (mProcVar2.field_0x300c == 0 && d_GameOver_CheckDelete(mMsgClassID)) {
            mProcVar2.field_0x300c = 1;
        }

        if (mProcVar2.field_0x300c != 0 && dComIfGp_getGameoverStatus() == 2 &&
            !checkNoResetFlg0(FLG0_UNK_4000))
        {
            dComIfGp_setItemLifeCount(-(dComIfGs_getLife() - 12), 0);

            u32 var_r31;
            int room_no = fopAcM_GetRoomNo(this);
            if (checkStageName("F_SP102") || (checkStageName("D_MN08D") && room_no == 55)) {
                var_r31 = 0;
            } else {
                var_r31 = 5;
            }

            int var_r30 = -1;
            int var_r3_2;
            if (checkStageName("D_MN09A") && room_no == 50 &&
                (dComIfG_play_c::getLayerNo(0) == 0 || dComIfG_play_c::getLayerNo(0) == 1))
            {
                var_r3_2 = dComIfG_play_c::getLayerNo(0) + 1;
            } else if (checkBossRoom() &&
                       (((room_no == 50 && !dComIfGs_isStageBossEnemy()) ||
                         (room_no == 51 && !dComIfGs_isStageMiddleBoss())) ||
                        (checkStageName("D_MN08C") && !dComIfGs_isEventBit(0x2880))))
            {
                var_r3_2 = 0;
            } else if (mProcVar5.field_0x3012 != 0x3F) {
                var_r3_2 = mProcVar5.field_0x3012;
                var_r30 = field_0x3198;
            } else {
                startRestartRoom(var_r31, 0xC9, 0, 1);
                return 1;
            }

            onNoResetFlg0(FLG0_UNK_4000);
            dStage_changeScene(var_r3_2, 0.0f, var_r31, var_r30, shape_angle.y, -1);
        }
    }

    return 1;
}


int daAlink_c::procCoLookAroundInit() {
    if (!commonProcInitNotSameProc(PROC_LOOK_AROUND)) {
        return 0;
    }

    mProcVar3.field_0x300e = 0;

    if (checkWolf()) {
        setSingleAnimeWolfBase(WANM_ENTRANCE);
        setFaceBasicTexture(FTANM_UNK_A0);
        field_0x3588 = l_wolfBaseAnime;
        voiceStart(Z2SE_WL_V_ENTRANCE);
    } else {
        if (mDemo.getParam0() == 1) {
            setSingleAnimeBase(ANM_ODOROKU);
            field_0x3588 = l_halfAtnWaitBaseAnime;
        } else {
            setSingleAnimeBase(ANM_ENTRANCE);
            field_0x3588 = l_waitBaseAnime;
            mProcVar3.field_0x300e = 1;
        }
    }

    mNormalSpeed = 0.0f;
    mProcVar2.field_0x300c = 0;
    return 1;
}

int daAlink_c::procCoLookAround() {
    daPy_frameCtrl_c* frame_ctrl = mUnderFrameCtrl;

    if (mProcVar2.field_0x300c == 0) {
        mProcVar2.field_0x300c = 1;
    } else {
        field_0x2f99 = 4;
    }

    if (checkAnmEnd(frame_ctrl)) {
        dComIfGp_evmng_cutEnd(mAlinkStaffId);
    } else if (mProcVar3.field_0x300e != 0 && frame_ctrl->checkPass(5.0f)) {
        voiceStart(Z2SE_AL_V_ENTRANCE);
    }

    return 1;
}

int daAlink_c::procBossAtnWaitInit() {
    if (!commonProcInitNotSameProc(PROC_BOSS_ATN_WAIT)) {
        return 0;
    }

    setSingleAnimeBase(ANM_APPEARANCE);
    voiceStart(Z2SE_AL_V_APPEARANCE);
    mNormalSpeed = 0.0f;
    return 1;
}

int daAlink_c::procCoQuakeWaitInit() {
    if (!commonProcInitNotSameProc(PROC_QUAKE_WAIT)) {
        return 0;
    }

    if (checkWolf()) {
        setSingleAnimeWolfBase(WANM_WAIT_WIND);
    } else {
        setSingleAnimeBase(ANM_WAIT_WIND);
    }

    mNormalSpeed = 0.0f;
    return 1;
}

int daAlink_c::procCoCaughtInit() {
    u32 chk_40000 = checkModeFlg(0x40000);
    if (!commonProcInitNotSameProc(PROC_CAUGHT)) {
        return 0;
    }

    if (checkWolf()) {
        setSingleAnimeWolfBaseMorf(WANM_WAIT_START, mpHIO->mWolf.mWlDamage.mCapture.m.mWaitInterp);
    } else {
        setSingleAnimeBaseMorf(ANM_WAIT_START, mpHIO->mDamage.mDamCaught.m.mStandbyInterp);
    }

    if (chk_40000) {
        onModeFlg(0x40000);
    }

    mNormalSpeed = 0.0f;
    speed.y = 0.0f;
    setSpecialGravity(0.0f, maxFallSpeed, 0);
    mBodyAngle.z = 0;
    return 1;
}

int daAlink_c::procLookUpInit() {
    if (!commonProcInitNotSameProc(PROC_LOOK_UP)) {
        return 0;
    }

    setSingleAnimeBase(ANM_GET_A_WAIT2);
    mNormalSpeed = 0.0f;
    field_0x3588 = l_waitBaseAnime;
    field_0x2f99 = 12;
    return 1;
}

int daAlink_c::procLookUp() {
    if (field_0x2f99 != 12) {
        field_0x2f99 = 4;
    }

    dComIfGp_evmng_cutEnd(mAlinkStaffId);
    return 1;
}

int daAlink_c::procLookUpToGetItemInit() {
    if (!commonProcInitNotSameProc(PROC_LOOK_UP_TO_GET_ITEM)) {
        return 0;
    }

    setSingleAnimeBaseMorf(ANM_GET_HOLDOUT, 2.0f);
    mProcVar2.field_0x300c = 0;
    mNormalSpeed = 0.0f;
    field_0x3588 = l_waitBaseAnime;
    field_0x2f99 = 12;
    return 1;
}

int daAlink_c::procLookUpToGetItem() {
    if (field_0x2f99 != 12) {
        field_0x2f99 = 4;
    }

    if (mProcVar2.field_0x300c == 0 && checkAnmEnd(mUpperFrameCtrl)) {
        mProcVar2.field_0x300c = 1;
        setSingleAnimeBase(ANM_GET_A_WAIT);
        setFaceBasicBck(0x133);
        field_0x2f99 = 12;
    } else if (mProcVar2.field_0x300c != 0) {
        dComIfGp_evmng_cutEnd(mAlinkStaffId);
    }

    return 1;
}

int daAlink_c::procHandPatInit() {
    if (!commonProcInitNotSameProc(PROC_HAND_PAT)) {
        return 0;
    }

    setSingleAnime(ANM_COW_THROW_LEFT, 1.0f, 50.0f, -1, 3.0f);
    field_0x2f92 = 0xFE;
    field_0x2f93 = 0xFE;
    field_0x2f99 = 13;
    field_0x3588.set(-l_halfAtnWaitBaseAnime.x, l_halfAtnWaitBaseAnime.y,
                     -l_halfAtnWaitBaseAnime.z);
    mNormalSpeed = 0.0f;

    setOldRootQuaternion(0, -0x8000, 0);
    shape_angle.y += 0x8000;
    mProcVar2.field_0x300c = 0;
    mProcVar3.field_0x300e = 0;
    return 1;
}

int daAlink_c::procHandPat() {
    daPy_frameCtrl_c* frameCtrl_p = mUnderFrameCtrl;

    if (mProcVar2.field_0x300c != 0) {
        if (checkAnmEnd(frameCtrl_p)) {
            dComIfGp_evmng_cutEnd(mAlinkStaffId);

            if (mProcVar3.field_0x300e == 0) {
                setSingleAnimeBaseSpeed(ANM_WAIT, mpHIO->mMove.m.mWaitAnmSpeed, 3.0f);
                mProcVar3.field_0x300e = 1;
            }
        }
    } else {
        if (field_0x2f99 != 13) {
            field_0x2f99 = 5;
        }

        if (checkAnmEnd(frameCtrl_p)) {
            setSingleAnimeBase(ANM_WAIT_B_TO_A);
            mProcVar2.field_0x300c = 1;
            shape_angle.y += 0x8000;
            field_0x2fe6 = shape_angle.y;
            current.angle.y = shape_angle.y;
            setOldRootQuaternion(0, -0x8000, 0);
            field_0x2f99 = 0x30;
        } else if (frameCtrl_p->getFrame() >= 81.0f) {
            field_0x2f92 = 1;
            field_0x2f93 = 6;
        }
    }

    return 1;
}


int daAlink_c::procCoFogDeadInit() {
    if (!commonProcInitNotSameProc(PROC_FOG_DEAD)) {
        return 1;
    }

    mNormalSpeed = 0.0f;
    mDamageTimer = 0;

    if (mLinkAcch.ChkGroundHit()) {
        if (checkWolf()) {
            setSingleAnimeWolfBase(WANM_DIE);
            setFaceBasicTexture(FTANM_UNK_9C);
            voiceStart(Z2SE_WL_V_DIE);
        } else {
            setSingleAnimeBase(ANM_DIE);
            voiceStart(Z2SE_AL_V_DIE);
        }
    } else {
        setJumpMode();

        if (checkWolf()) {
            setSingleAnimeWolfBaseSpeed(WANM_FALL_LAND, 0.0f,
                                        mpHIO->mWolf.mWlDamage.mFall.m.mAirAnmInterp);
        } else {
            setSingleAnimeBaseSpeed(ANM_DMG_FALL, 0.0f,
                                    mpHIO->mDamage.mDamFall.m.mFallAnmMorf);
        }
    }

    field_0x3194 = 0;

    dCam_getBody()->StartEventCamera(0x13, fopAcM_GetID(this), "Type", 1, &field_0x3194, 0);
    mProcVar0.field_0x3008 = 60;
    mProcVar2.field_0x300c = 0;
    return 1;
}

int daAlink_c::procCoFogDead() {
    if (mLinkAcch.ChkGroundHit() && checkModeFlg(0x2)) {
        offModeFlg(0x2);

        if (checkWolf()) {
            setSingleAnimeWolfBase(WANM_DIE);
            setFaceBasicTexture(FTANM_UNK_9C);
            voiceStart(Z2SE_WL_V_DIE);
        } else {
            setSingleAnimeBase(ANM_DIE);
            voiceStart(Z2SE_AL_V_DIE);
        }
    }

    if (mProcVar0.field_0x3008 > 0) {
        mProcVar0.field_0x3008--;
    } else if (mProcVar0.field_0x3008 == 0) {
        if (dStage_changeScene(daTagMist_c::getPlayerNo(), 0.0f, 5, fopAcM_GetRoomNo(this),
                               shape_angle.y, -1))
        {
            seStartSystem(Z2SE_FORCE_BACK);
            mProcVar0.field_0x3008--;
        }
    }

    return 1;
}


int daAlink_c::procWolfSmellWaitInit() {
    if (!commonProcInitNotSameProc(PROC_WOLF_SMELL_WAIT)) {
        return 1;
    }

    setSingleAnimeWolfBaseSpeed(WANM_SMELL, mpHIO->mWolf.mWlChain.m.mSniffAnmSpeed,
                                mpHIO->mWolf.mWlChain.m.mSniffInterp);
    current.angle.y = shape_angle.y;
    mNormalSpeed = 0.0f;
    return 1;
}

int daAlink_c::procCoNodInit() {
    if (!commonProcInitNotSameProc(PROC_NOD)) {
        return 1;
    }

    if (checkWolf()) {
        mProcVar2.field_0x300c = 1;
    } else {
        if (mDemo.getParam0() == 1) {
            setUpperAnimeBase(0x24E);
            setFacePriBck(0x15B);
            setFacePriTexture(FTANM_UNK_8A);
        } else {
            setUpperAnimeBase(0xD1);
            setUpperAnimeBase(0xD1);
            setFacePriTexture(FTANM_UNK_75);
        }

        field_0x2f96 = 4;
        field_0x2f97 = 10;
        onNoResetFlg1(FLG1_UNK_10100000);
        mProcVar2.field_0x300c = 0;
    }

    return 1;
}

int daAlink_c::procCoNod() {
    if (checkAnmEnd(&mUpperFrameCtrl[2])) {
        mProcVar2.field_0x300c = 1;
        resetUpperAnime(UPPER_2, 3.0f);
    }

    if (mProcVar2.field_0x300c != 0) {
        dComIfGp_evmng_cutEnd(mAlinkStaffId);
    }

    return 1;
}

int daAlink_c::procCoEyeAwayInit() {
    if (!commonProcInitNotSameProc(PROC_EYE_AWAY)) {
        return 1;
    }

    if (!checkWolf()) {
        setUpperAnimeBase(0xD3);
        setFacePriTexture(FTANM_UNK_77);
        setFacePriBck(0x129);
        field_0x2f96 = 4;
        field_0x2f97 = 10;
        onNoResetFlg1(FLG1_UNK_10100000);
    }

    return 1;
}

int daAlink_c::procCoGlareInit() {
    if (!commonProcInitNotSameProc(PROC_GLARE)) {
        return 1;
    }

    if (checkWolf()) {
        mProcVar2.field_0x300c = 1;
    } else {
        setUpperAnimeBase(0xD2);
        mUpperFrameCtrl[2].setAttribute(2);
        mUpperFrameCtrl[2].setLoop(45);
        setFacePriTexture(FTANM_UNK_76);
        setFacePriBck(0x128);
        mProcVar2.field_0x300c = 0;
        field_0x3478 = -1.0f;
        field_0x2f96 = 4;
        field_0x2f97 = 10;
        onNoResetFlg1(FLG1_UNK_10100000);
    }

    return 1;
}

int daAlink_c::procCoGlare() {
    daPy_frameCtrl_c* frame_ctrl = &mUpperFrameCtrl[2];

    if (!checkWolf()) {
        if (field_0x3478 > frame_ctrl->getFrame()) {
            mProcVar2.field_0x300c = 1;
        } else {
            field_0x3478 = frame_ctrl->getFrame();
        }
    }

    if (mProcVar2.field_0x300c != 0) {
        dComIfGp_evmng_cutEnd(mAlinkStaffId);
    }

    if (!checkWolf() && frame_ctrl->getFrame() >= 24.0f) {
        field_0x2f96 = 0;
        field_0x2f97 = 5;
    }

    return 1;
}

int daAlink_c::procGoatStopReadyInit() {
    if (!commonProcInitNotSameProc(PROC_GOAT_STOP_READY)) {
        return 1;
    }

    setSingleAnimeBase(ANM_ATN_COW);
    mNormalSpeed = 0.0f;
    mProcVar2.field_0x300c = 0;
    return 1;
}

int daAlink_c::procGoatStopReady() {
    if (checkAnmEnd(mUnderFrameCtrl)) {
        setSingleAnimeBase(ANM_COW_MOVE_LEFT_RIGHT);
        mProcVar2.field_0x300c = 1;
    }

    if (mProcVar2.field_0x300c != 0) {
        dComIfGp_evmng_cutEnd(mAlinkStaffId);
    }

    return 1;
}

int daAlink_c::procCoGetReadySitInit() {
    if (mProcID == PROC_GET_READY_SIT ||
        (mProcID == PROC_PREACTION_UNEQUIP && !checkNoUpperAnime()))
    {
        return 1;
    }

    if (mEquipItem != fpcNm_ITEM_NONE && !checkWolf()) {
        return procPreActionUnequipInit(PROC_GET_READY_SIT, NULL);
    }

    commonProcInit(PROC_GET_READY_SIT);

    if (checkWolf()) {
        setSingleAnimeWolfBaseSpeed(WANM_WAIT, mpHIO->mWolf.mWlMove.m.mIdleAnmSpeed, 3.0f);
        mProcVar2.field_0x300c = 1;
    } else {
        setSingleAnime(ANM_TRES_OPEN_SMALL, 1.0f, 36.0f, -1, 3.0f);
        mProcVar2.field_0x300c = 0;
        field_0x3588 = l_waitBaseAnime;
        field_0x2f99 = 12;
    }

    return 1;
}

int daAlink_c::procCoGetReadySit() {
    if (checkAnmEnd(mUnderFrameCtrl)) {
        mProcVar2.field_0x300c = 1;
    }

    if (!checkWolf() && field_0x2f99 != 12) {
        field_0x2f99 = 4;
    }

    if (mProcVar2.field_0x300c != 0) {
        dComIfGp_evmng_cutEnd(mAlinkStaffId);
    }

    return 1;
}

int daAlink_c::procCoTwGateInit() {
    if (!commonProcInitNotSameProc(PROC_TW_GATE)) {
        return 1;
    }

    if (checkWolf()) {
        setSingleAnimeWolfBase(WANM_TWGATE_PULL);
        field_0x3588 = l_wolfBaseAnime;
        voiceStart(Z2SE_WL_V_TW_PULL);
    } else {
        setSingleAnimeBase(ANM_TWGATE_PULLED_IN);
        field_0x3588 = l_waitBaseAnime;
        voiceStart(Z2SE_AL_V_TW_PULL);
    }

    field_0x33b0 = field_0x3588.y;
    mNormalSpeed = 0.0f;
    setSpecialGravity(0.0f, maxFallSpeed, 0);
    speed.y = 0.0f;
    return 1;
}


int daAlink_c::procCoTwGate() {
    if (mUnderFrameCtrl[0].checkAnmEnd()) {
        dComIfGp_evmng_cutEnd(mAlinkStaffId);
    }

    field_0x2f99 = 7;
    return 1;
}

int daAlink_c::procWolfSnowEscapeInit() {
    if (!commonProcInitNotSameProc(PROC_WOLF_SNOW_ESCAPE)) {
        return 1;
    }

    current.angle.y = shape_angle.y + 0x8000;
    setSingleAnimeWolf(WANM_TURN, 1.0f, 5.0f, 11, 3.0f);
    voiceStart(Z2SE_WL_V_BREATH_JUMP);
    field_0x2f99 = 13;
    field_0x3588 = l_wolfBaseAnime;
    field_0x3478 = 0.14285715f;
    mProcVar2.field_0x300c = 0;
    mProcVar3.field_0x300e = 0;
    mNormalSpeed = 30.0f;
    return 1;
}

int daAlink_c::procWolfSnowEscape() {
    daPy_frameCtrl_c* frameCtrl_p = mUnderFrameCtrl;

    if (mProcVar3.field_0x300e != 0) {
        dComIfGp_evmng_cutEnd(mAlinkStaffId);
        return 1;
    }

    if (field_0x2f99 != 13) {
        field_0x2f99 = 5;
    }

    f32 var_f1 = (0.5f - field_0x3478 * (frameCtrl_p->getFrame() - frameCtrl_p->getStart())) * 2.0f;
    if (var_f1 < -1.0f) {
        var_f1 = -1.0f;
    }

    field_0x3588.x = l_wolfBaseAnime.x * var_f1;
    field_0x3588.z = l_wolfBaseAnime.z * var_f1;

    if (checkAnmEnd(frameCtrl_p)) {
        setOldRootQuaternion(0, -0x8000, 0);
        shape_angle.y += 0x8000;
        field_0x2fe6 = shape_angle.y;
        current.angle.y = shape_angle.y;

        if (mProcVar2.field_0x300c == 0) {
            mProcVar2.field_0x300c = 1;
            setSingleAnimeWolfParam(WANM_SLIDE_FORWARD_START, &mpHIO->mWolf.mWlSlide.m.mFrontSlideAnm);
            field_0x3588 = l_wolfBaseAnime;
            field_0x3478 = 1.0f / (f32)frameCtrl_p->getEnd();
            field_0x2f99 = 13;
            voiceStart(Z2SE_WL_V_SLIP_ROLL);
        } else {
            setSingleAnimeWolfBaseSpeed(WANM_WAIT, mpHIO->mWolf.mWlMove.m.mIdleAnmSpeed, 3.0f);
            field_0x2f99 = 0x30;
            mProcVar3.field_0x300e = 1;
        }
    } else if (frameCtrl_p->checkPass(12.0f)) {
        mNormalSpeed = 0.0f;
    }

    return 1;
}


int daAlink_c::procZoraMoveInit() {
    if (!commonProcInitNotSameProc(PROC_ZORA_MOVE)) {
        return 1;
    }

    setSingleAnimeBase(ANM_HORSE_TAME_WAIT_B);
    speed.y = 0.0f;
    mNormalSpeed = 0.0f;
    setSpecialGravity(0.0f, maxFallSpeed, 0);

    deleteEquipItem(FALSE, TRUE);
    mProcVar4.field_0x3010 = 5000;
    mProcVar5.field_0x3012 = 0;
    field_0x384c = (cXyz*)&l_boarBaseAnime;
    return 1;
}

int daAlink_c::procZoraMove() {
    dComIfGp_evmng_cutEnd(mAlinkStaffId);
    fopAc_ac_c* temp_r29 = dComIfGp_event_getPt1();    
    field_0x2f99 = 0x60;

    if (temp_r29 != NULL) {
        cXyz sp14;
        cXyz sp8;

        mDoMtx_stack_c::copy(((daNpc_zrA_c*)temp_r29)->getHeadMtx());
        mDoMtx_stack_c::YrotM(-0x4000);
        mDoMtx_stack_c::transM(0.0f, 30.0f, -90.0f);
        mDoMtx_stack_c::multVecZero(&current.pos);
        mDoMtx_stack_c::multVecSR(&cXyz::BaseZ, &sp14);
        mDoMtx_stack_c::multVecSR(&cXyz::BaseY, &sp8);
        shape_angle.y = temp_r29->shape_angle.y;

        if (sp8.y < 0.0f) {
            shape_angle.x = cM_atan2s(-sp14.y, -sp14.absXZ());
        } else {
            shape_angle.x = sp14.atan2sY_XZ();
        }
    }

    return 1;
}

int daAlink_c::procLookAroundTurnInit() {
    if (!commonProcInitNotSameProc(PROC_LOOK_AROUND_TURN)) {
        return 0;
    }

    mNormalSpeed = 0.0f;

    if (mDemo.getParam0() == 3) {
        setSingleAnimeBase(ANM_ASHIMOTO);
    } else if (mDemo.getParam0() == 2) {
        setSingleAnime(ANM_DEMO_MHOP, 1.0f, 0.0f, 89, 3.0f);
    } else if (mDemo.getParam0() == 1) {
        setSingleAnime(ANM_DEMO_MHOP, 1.0f, 0.0f, 93, 3.0f);
        setFaceBasicBtp(0x3DC);
        setFaceBasicBtk(0x340);
        setFaceBasicBck(0x143);
    } else {
        setSingleAnimeBase(ANM_DEMO_MHOP);
    }

    return 1;
}

int daAlink_c::procLookAroundTurn() {
    daPy_frameCtrl_c* frame_ctrl = mUnderFrameCtrl;

    if (checkAnmEnd(frame_ctrl)) {
        dComIfGp_evmng_cutEnd(mAlinkStaffId);
    } else if (frame_ctrl->checkPass(95.0f)) {
        voiceStart(Z2SE_AL_V_D_MHOP);
    }

    return 1;
}

int daAlink_c::procTradeItemOutInit() {
    if (!commonProcInitNotSameProc(PROC_TRADE_ITEM_OUT)) {
        return 0;
    }

    field_0x3198 = setTalkStartBack(&field_0x37c8);

    if (field_0x3198 == 0) {
        mProcVar2.field_0x300c = setTradeItemAnime();
        mNormalSpeed = 0.0f;
    } else {
        mProcVar2.field_0x300c = 0;
    }

    mProcVar3.field_0x300e = 0;
    dComIfGp_setPlayerStatus0(0, 0x10);
    mProcVar4.field_0x3010 = 1;

    if (mDemo.getParam0() != 1) {
        if (mDemo.getParam0() == 2) {
            if (checkLetterItem(mDemo.getParam1())) {
                mProcVar4.field_0x3010 = 0;
            }
        } else {
            if (checkLetterItem(dComIfGp_event_getPreItemNo())) {
                mProcVar4.field_0x3010 = 0;
            }
        }
    }

    mProcVar0.field_0x3008 = 30;
    return 1;
}

int daAlink_c::procTradeItemOut() {
    setShapeAngleToTalkActor();

    if (field_0x3198 != 0) {
        cXyz sp24 = field_0x37c8 - current.pos;
        int var_r30 = 0;

        if (mLinkAcch.ChkWallHit()) {
            dBgS_AcchCir* acchCir_p = mAcchCir;

            for (int i = 0; i < 3; i++) {
                if (acchCir_p->ChkWallHit()) {
                    var_r30 = cLib_distanceAngleS(acchCir_p->GetWallAngleY(), current.angle.y);
                    break;
                }
                acchCir_p++;
            }
        }

        if (sp24.absXZ() < 0.001f ||
            cLib_distanceAngleS(sp24.atan2sX_Z(), current.angle.y) > 0x4000 || var_r30 > 0x4000)
        {
            current.pos.x = field_0x37c8.x;
            current.pos.z = field_0x37c8.z;
            field_0x3198 = 0;
            mProcVar2.field_0x300c = setTradeItemAnime();
            mNormalSpeed = 0.0f;
        } else if (mProcVar0.field_0x3008 != 0) {
            mProcVar0.field_0x3008--;
        } else {
            field_0x3198 = 0;
            mProcVar2.field_0x300c = setTradeItemAnime();
            mNormalSpeed = 0.0f;
        }
    } else {
        current.angle.y = shape_angle.y;

        if (mProcVar3.field_0x300e != 0) {
            if (mDemo.getParam0() == 1) {
                dComIfGp_evmng_cutEnd(mAlinkStaffId);
            } else {
                daItemBase_c* item_partner_p = (daItemBase_c*)fopAcM_getItemEventPartner(this);
                if (item_partner_p != NULL) {
                    if (!fpcM_IsCreating(fpcM_GetID(item_partner_p))) {
                        if (fopAcM_GetName(item_partner_p) == PROC_ITEM ||
                            fopAcM_GetName(item_partner_p) == PROC_Demo_Item)
                        {
                            field_0x280c.setData(item_partner_p);
                            item_partner_p->show();

                            if (mProcVar4.field_0x3010 == 0) {
                                mProcVar4.field_0x3010 = 1;
                                seStartOnlyReverb(Z2SE_AL_OPEN_LETTER);
                            }

                            if (mDemo.getParam0() == 2) {
                                onResetFlg1(RFLG1_UNK_100);
                            }
                        }

                        dComIfGp_evmng_cutEnd(mAlinkStaffId);
                    }
                }
            }
        } else if (checkAnmEnd(mUnderFrameCtrl)) {
            setSingleAnimeBase(ANM_TRADE_ITEM_WAIT);
            onModeFlg(0x100);
            mProcVar3.field_0x300e = 1;
        } else {
            setTradeItemOutHand();
        }
    }

    return 1;
}


BOOL daAlink_c::checkLetterItem(int i_itemNo) {
    return i_itemNo == fpcNm_ITEM_LETTER || i_itemNo == fpcNm_ITEM_BILL || i_itemNo == fpcNm_ITEM_RAFRELS_MEMO ||
           i_itemNo == fpcNm_ITEM_TKS_LETTER || i_itemNo == fpcNm_ITEM_ASHS_SCRIBBLING;
}

int daAlink_c::procNotUseItemInit(int param_0) {
    if (!dComIfGp_event_compulsory(this, NULL, 0xFFFF)) {
        return 0;
    }

    mDemo.setSpecialDemoType();
    commonProcInit(PROC_NOT_USE_ITEM);
    setSingleAnimeBase(ANM_TRADE_ITEM_PULL_OUT);

    mNormalSpeed = 0.0f;
    mMsgClassID = fpcM_ERROR_PROCESS_ID_e;
    field_0x3198 = param_0;
    mProcVar2.field_0x300c = 0;
    mProcVar3.field_0x300e = 0;
    current.angle.y = shape_angle.y;
    field_0x3194 = 3;

    dCam_getBody()->StartEventCamera(0x12, fopAcM_GetID(this), "Type", 1, &field_0x3194, 0);
    keepItemData();
    dComIfGp_setPlayerStatus1(0, 0x4000800);

    if (checkLetterItem(param_0)) {
        mProcVar4.field_0x3010 = 0;
    } else {
        mProcVar4.field_0x3010 = 1;
    }
    return 1;
}


int daAlink_c::procNotUseItem() {
    if (mProcVar2.field_0x300c == 0) {
        fpc_ProcID item_partner_id = fopAcM_createItemForPresentDemo(
            &current.pos, field_0x3198, 5, -1, fopAcM_GetRoomNo(this), &shape_angle, &scale);
        dComIfGp_event_setItemPartnerId(item_partner_id);
        mProcVar2.field_0x300c = 1;
    }

    if (mProcVar3.field_0x300e != 0) {
        daItemBase_c* item_partner_p = (daItemBase_c*)fopAcM_getItemEventPartner(this);
#ifdef DEBUG
        if (item_partner_p != NULL && fopAcM_GetName(item_partner_p) != PROC_ITEM &&
            fopAcM_GetName(item_partner_p) != PROC_Demo_Item)
        {
            // "Present Item is not this Item!!! %d\n"
            OSReport("差し出すアイテムがアイテムじゃない！！！　%d\n",
                     fopAcM_GetName(item_partner_p));
            JUT_ASSERT(5506, FALSE);
        }
#endif
        if (item_partner_p != NULL) {
            if (!fpcM_IsCreating(fpcM_GetID(item_partner_p))) {
                field_0x280c.setData(item_partner_p);
                item_partner_p->show();

                if (mProcVar4.field_0x3010 == 0) {
                    mProcVar4.field_0x3010 = 1;
                    seStartOnlyReverb(Z2SE_AL_OPEN_LETTER);
                }

                if (checkEndMessage(0x531)) {
                    returnKeepItemData();
                    resetSpecialEvent();
                    return 1;
                }
            }
        }
    } else if (checkAnmEnd(mUnderFrameCtrl)) {
        setSingleAnimeBase(ANM_TRADE_ITEM_WAIT);
        onModeFlg(0x100);
        mProcVar3.field_0x300e = 1;
    } else {
        setTradeItemOutHand();
    }

    return 1;
}

int daAlink_c::procSwordReadyInit() {
    if (!commonProcInitNotSameProc(PROC_SWORD_READY)) {
        return 0;
    }

    mNormalSpeed = 0.0f;
    current.angle.y = shape_angle.y;
    setSingleAnimeBase(ANM_DEMO_KAMAE);
    mProcVar2.field_0x300c = 0;
    return 1;
}

int daAlink_c::procSwordReady() {
    daPy_frameCtrl_c* frame_ctrl = mUnderFrameCtrl;

    if (checkAnmEnd(frame_ctrl)) {
        if (mProcVar2.field_0x300c == 0) {
            setDoubleAnime(0.0f, mpHIO->mNoActAtnMove.m.mWaitAnmSpeed,
                           mpHIO->mNoActAtnMove.m.mWaitAnmSpeed, ANM_WAIT_B, ANM_WAIT_B, 2,
                           3.0f);
        }

        mProcVar2.field_0x300c = 1;
        dComIfGp_evmng_cutEnd(mAlinkStaffId);
    } else if (mProcVar2.field_0x300c != 0) {
        dComIfGp_evmng_cutEnd(mAlinkStaffId);
    } else if (frame_ctrl->checkPass(10.0f)) {
        voiceStart(Z2SE_AL_V_OUGI_KAMAE);
    }

    return 1;
}

void daAlink_c::setSwordPushAnime() {
    daAlink_ANM anm_id = (daAlink_ANM)(mDemo.getParam0() + ANM_GANON_CHANCE);
    if (field_0x3198 != anm_id) {
        setSingleAnimeBase(anm_id);
        field_0x2f99 = 13;
        field_0x3198 = anm_id;

        if (anm_id == ANM_GANON_CHANCE_WIN) {
            voiceStart(Z2SE_AL_V_VS_GND_TUBA_WIN);
        } else if (anm_id == ANM_GANON_CHANCE_LOSE) {
            voiceStart(Z2SE_AL_V_VS_GND_TUBA_LOSE);
        }
    }
}

int daAlink_c::procSwordPushInit() {
    if (!commonProcInitNotSameProc(PROC_SWORD_PUSH)) {
        return 1;
    }

    mNormalSpeed = 0.0f;
    current.angle.y = shape_angle.y;
    field_0x3198 = 0;
    setSwordPushAnime();
    field_0x3588 = l_halfAtnWaitBaseAnime;
    return 1;
}

int daAlink_c::procSwordPush() {
    field_0x2f99 = 4;
    onEndResetFlg0(ERFLG0_UNK_8000000);
    setSwordPushAnime();

    if (field_0x3198 == 0x193) {
        voiceStartLevel(Z2SE_AL_V_VS_GND_TUBA_A);
    } else if (field_0x3198 == 0x194) {
        voiceStartLevel(Z2SE_AL_V_VS_GND_TUBA_C);
    } else if (field_0x3198 == 0x192) {
        voiceStartLevel(Z2SE_AL_V_VS_GND_TUBA_B);
    }

    return 1;
}

int daAlink_c::procGanonFinishInit() {
    if (!commonProcInitNotSameProc(PROC_GANON_FINISH)) {
        return 1;
    }

    setSingleAnimeBase(ANM_GANON_FINISH);

    mNormalSpeed = 0.0f;
    speed.y = 0.0f;

    field_0x37c8 = current.pos;
    onEndResetFlg1(ERFLG1_GANON_FINISH);
    return 1;
}

int daAlink_c::procGanonFinish() {
    onEndResetFlg1(ERFLG1_GANON_FINISH);
    current.pos = field_0x37c8;

    if (checkAnmEnd(mUnderFrameCtrl)) {
        setSingleAnimeBaseMorf(ANM_GANON_FINISH_WAIT, -1.0f);
    }

    return 1;
}

int daAlink_c::procCutFastReadyInit() {
    if (!commonProcInitNotSameProc(PROC_CUT_FAST_READY)) {
        return 0;
    }

    mNormalSpeed = 0.0f;
    current.angle.y = shape_angle.y;
    setSingleAnimeBaseSpeed(ANM_WAIT, mpHIO->mMove.m.mWaitAnmSpeed, 3.0f);
    setUpperAnime(0x255, UPPER_2, mpHIO->mCut.m.mEquipAnm.mSpeed, 12.0f,
                  mpHIO->mCut.m.mEquipAnm.mEndFrame,
                  mpHIO->mCut.m.mEquipAnm.mInterpolation);
    onNoResetFlg0(FLG0_UNK_1000000);
    return 1;
}

int daAlink_c::procCutFastReady() {
    if (checkAnmEnd(&mUpperFrameCtrl[2])) {
        dComIfGp_evmng_cutEnd(mAlinkStaffId);
    }

    return 1;
}

int daAlink_c::procMasterSwordStickInit() {
    if (!commonProcInitNotSameProc(PROC_MASTER_SWORD_STICK)) {
        return 0;
    }

    setSingleAnimeBase(ANM_DEMO_MASTER_SWORD_STICK);
    voiceStart(Z2SE_AL_V_MSTR_SW_STICK);
    mNormalSpeed = 0.0f;
    current.angle.y = shape_angle.y;
    mProcVar2.field_0x300c = 0;

    return 1;
}

int daAlink_c::procMasterSwordStick() {
    daPy_frameCtrl_c* frame_ctrl = mUnderFrameCtrl;

    if (mProcVar2.field_0x300c != 0) {
        dComIfGp_evmng_cutEnd(mAlinkStaffId);
    } else if (checkAnmEnd(frame_ctrl)) {
        mProcVar2.field_0x300c = 1;
        setSingleAnimeBaseMorf(ANM_DEMO_MASTER_SWORD_WAIT, -1.0f);
        dComIfGp_evmng_cutEnd(mAlinkStaffId);
    } else if (frame_ctrl->getFrame() >= 32.0f) {
        field_0x2f93 = 5;
    }

    return 1;
}

int daAlink_c::procMasterSwordPullInit() {
    if (!commonProcInitNotSameProc(PROC_MASTER_SWORD_PULL)) {
        return 0;
    }

    setSingleAnimeBase(ANM_DEMO_MASTER_SWORD_PULL);
    voiceStart(Z2SE_AL_V_MSTR_SW_PULLOUT);
    mProcVar4.field_0x3010 = 0;

    return 1;
}

int daAlink_c::procMasterSwordPull() {
    daPy_frameCtrl_c* frame_ctrl = mUnderFrameCtrl;

    if (checkAnmEnd(frame_ctrl)) {
        dComIfGp_evmng_cutEnd(mAlinkStaffId);
    } else if (frame_ctrl->getFrame() >= 36.0f) {
        field_0x2f93 = 10;
    } else if (frame_ctrl->getFrame() >= 9.0f) {
        mProcVar4.field_0x3010 = 0;
    } else if (frame_ctrl->getFrame() >= 1.0f) {
        mProcVar4.field_0x3010 = 4;
    }

    return 1;
}

BOOL daAlink_c::checkLv7DungeonShop() {
    return checkStageName("D_MN07") && dComIfGp_roomControl_getStayNo() == 16;
}

int daAlink_c::procDungeonWarpReadyInit() {
    if (!dComIfGp_event_compulsory(this, NULL, -1)) {
        return 0;
    }

    fpc_ProcID id;
    if (checkItemSetButton(fpcNm_ITEM_DUNGEON_EXIT) != 2) {
        id = fopAcM_create(PROC_OBJ_TKS, 0, &current.pos, fopAcM_GetRoomNo(this), &shape_angle,
                           NULL, -1);
    } else {
        id = fopAcM_create(PROC_NPC_TKC, 2, &current.pos, fopAcM_GetRoomNo(this), &shape_angle,
                           NULL, -1);
    }

    if (id == fpcM_ERROR_PROCESS_ID_e) {
        return 0;
    }

    mDemo.setSpecialDemoType();
    commonProcInit(PROC_DUNGEON_WARP_READY);
    setSingleAnimeBase(ANM_TRADE_ITEM_PULL_OUT);
    mNormalSpeed = 0.0f;
    current.angle.y = shape_angle.y;
    field_0x32cc = id;
    mProcVar2.field_0x300c = 0;
    keepItemData();

    return 1;
}

int daAlink_c::procDungeonWarpReady() {
    if (mProcVar2.field_0x300c != 0) {
        fopAc_ac_c* temp_r3 = fopAcM_SearchByID(field_0x32cc);
        if (temp_r3 != NULL) {
            field_0x280c.setData(temp_r3);

            if (fopAcM_GetName(temp_r3) == PROC_OBJ_TKS) {
                ((daObjTks_c*)temp_r3)->setStart();
            } else {
                ((daNpcTkc_c*)temp_r3)->setStart();
            }
        }
    } else if (checkAnmEnd(&mUnderFrameCtrl[0])) {
        setSingleAnimeBase(ANM_TRADE_ITEM_WAIT);
        onModeFlg(0x100);
        mProcVar2.field_0x300c = 1;
    } else {
        setTradeItemOutHand();
    }

    return 1;
}

int daAlink_c::procDungeonWarpInit() {
    if (!commonProcInitNotSameProc(PROC_DUNGEON_WARP)) {
        return 0;
    }

    mProcVar2.field_0x300c = 0;
    mProcVar5.field_0x3012 = 0;
    setSpecialGravity(0.0f, maxFallSpeed, 0);
    mProcVar3.field_0x300e = 0;
    mProcVar4.field_0x3010 = 0;

    return 1;
}

int daAlink_c::procDungeonWarp() {
    mProcVar2.field_0x300c += 0x200;

    if (mProcVar2.field_0x300c > 0x4000) {
        mProcVar2.field_0x300c = 0x4000;
    } else {
        current.pos.y += 1.0f;
    }

    f32 sin = cM_ssin(mProcVar2.field_0x300c);
    shape_angle.y += (s16)(14336.0f * sin);
    mProcVar3.field_0x300e = 8.0f * sin + 24.0f * (1.0f - scale.x);

    if (mProcVar5.field_0x3012 != 0) {
        return 1;
    } else {
        if (mProcVar2.field_0x300c == 0x4000) {
            if (mProcVar4.field_0x3010 == 0) {
                dComIfGp_particle_set(0xA61, &current.pos, &tevStr, NULL, NULL);
                mProcVar4.field_0x3010 = 1;
            }

            cLib_addCalc(&scale.x, 0.0f, 0.5f, 0.4f, 0.005f);
            cLib_addCalc(&scale.y, 2.5f, 0.5f, 0.5f, 0.1f);

            if (scale.x < 0.01f) {
                onNoResetFlg0(FLG0_PLAYER_NO_DRAW);
                if (checkItemGet(fpcNm_ITEM_DUNGEON_EXIT, 1)) {
                    int escape =
                        dStage_stagInfo_GetEscapeWarp(dComIfGp_getStage()->getStagInfo());
                    dStage_changeScene(escape, 0.0f, 12, -1, shape_angle.y, -1);
                    dMeter2Info_warpOutProc();
                } else {
                    dungeonReturnWarp();
                    dMeter2Info_warpInProc();
                }

                mProcVar5.field_0x3012 = 1;
            }
            current.pos.y += 5.0f;
            field_0x2b98 = (1.0f - scale.y) * 100.0f;
        } else if (mProcVar2.field_0x300c >= 0x2000) {
            cLib_chaseF(&scale.x, 0.8f, 0.01f);
        }

        scale.z = scale.x;
        mpLinkModel->setBaseScale(scale);
    }

    return 1;
}


int daAlink_c::procDungeonWarpSceneStartInit() {
    commonProcInit(PROC_DUNGEON_WARP_SCN_START);

    setSpecialGravity(0.0f, maxFallSpeed, 0);
    setSingleAnimeBaseSpeed(ANM_WAIT, mpHIO->mMove.m.mWaitAnmSpeed,
                            mpHIO->mBasic.m.mBasicInterpolation);

    if (checkDungeon() && !checkLv7DungeonShop()) {
        createNpcTks(&current.pos, fopAcM_GetRoomNo(this), 4);
    }

    mProcVar2.field_0x300c = 0x4000;
    mProcVar3.field_0x300e = 0x20;
    mProcVar4.field_0x3010 = 0;

    current.pos.y += 120.0f;
    shape_angle.y += 0x7F7A;

    scale.set(0.0f, 1.5f, 0.0f);
    mpLinkModel->setBaseScale(scale);
    return 1;
}


int daAlink_c::procDungeonWarpSceneStart() {
    if (mProcVar4.field_0x3010 == 0) {
        mProcVar4.field_0x3010 = 1;
        seStartOnlyReverb(Z2SE_TKC_WARP_OUT);
    }

    if (fopOvlpM_IsPeek() == true) {
        return 1;
    }

    BOOL var_r31 = cLib_chaseF(&scale.x, 1.0f, 0.05f);
    scale.z = scale.x;
    cLib_chaseF(&scale.y, 1.0f, 0.025f);
    mpLinkModel->setBaseScale(scale);

    if (!var_r31) {
        current.pos.y -= 5.0f;
    } else {
        mProcVar2.field_0x300c -= 0x200;
        if (mProcVar2.field_0x300c < 0) {
            return checkNextAction(0);
        }

        current.pos.y -= 1.0f;
    }

    f32 sin = cM_ssin(mProcVar2.field_0x300c);
    shape_angle.y += (s16)(sin * 14336.0f);
    mProcVar3.field_0x300e = (sin * 8.0f) + ((1.0f - scale.x) * 24.0f);
    return 1;
}


static fopAc_ac_c* daAlink_searchPortal(fopAc_ac_c* i_actor, void* i_data) {
    if (fopAcM_GetName(i_actor) == PROC_KYTAG04) {
        cXyz* pos = (cXyz*)i_data;
        if (i_actor->current.pos.abs2XZ(*pos) < 1.0f) {
            return i_actor;
        }
    }

    return NULL;
}


bool daAlink_c::checkAcceptWarp() {
    #if VERSION != VERSION_WII_USA_R0
    cM3dGPla plane;
    #endif

    if (mLinkAcch.ChkGroundHit() && !checkModeFlg(MODE_PLAYER_FLY)
        #if VERSION != VERSION_WII_USA_R0
        && !checkNoResetFlg0(FLG0_UNDERWATER)
        #endif
       )
    {
        if (
            #if VERSION != VERSION_WII_USA_R0
            !getSlidePolygon(&plane) &&
            #endif
            !checkForestOldCentury()
           )
        {
            if (checkMidnaRide() || ((daMidna_c*)getMidnaActor())->checkPortalObjRide()) {
                if ((checkField() || checkCastleTown()) && !checkStageName("R_SP161")) {
                    if ((checkWolf() && (checkModeFlg(MODE_UNK_1000) ||
                                           dComIfGp_checkPlayerStatus0(0, 0x10))) ||
                        (!checkWolf() &&
                         (checkEventRun() || getMidnaActor()->checkMetamorphoseEnable()) &&
                         (checkModeFlg(4) || dComIfGp_checkPlayerStatus0(0, 0x10))))
                    {
                        return true;
                    }
                }
            }
        }
    }

    return false;
}


void daAlink_c::dungeonReturnWarp() {
    cXyz pos = dComIfGs_getWarpPlayerPos();
    s16 angle = dComIfGs_getWarpPlayerAngleY();
    s8 room = dComIfGs_getWarpRoomNo();
    char name[8];
    strcpy(name, dComIfGs_getWarpStageName());

    dComIfGs_setRestartRoom(pos, angle, room);
    dComIfGp_setNextStage(name, -1, dComIfGs_getRestartRoomNo(), -1, 0.0f, 12, 0,
                          0, 0, 1, 0);
    u32 data = setParamData(dComIfGs_getRestartRoomNo(), 0, 0xCA, 0);
    dComIfGs_setRestartRoomParam(data);
}

void daAlink_c::skipPortalObjWarp() {
    if (mProcID == PROC_WARP && mProcVar3.field_0x300e <= 0) {
        return;
    }

    if (checkNoResetFlg3(FLG3_WARP_OBJ_DEMO) &&
        (dComIfGp_TargetWarpPt_get() == 9 && dComIfGp_TransportWarp_check()))
    {
        dStage_changeScene(2, 0.0f, 0, fopAcM_GetRoomNo(this), shape_angle.y, -1);
    } else {
        u32 data = setParamData(dMeter2Info_getWarpRoomNo(), 0, 0xCA, 0);
        dComIfGs_setTurnRestart(dMeter2Info_getWarpPos(), dMeter2Info_getWarpPlayerNo(),
                                dMeter2Info_getWarpRoomNo(), data);
        dComIfGp_setNextStage(dMeter2Info_getWarpStageName(), -4, dMeter2Info_getWarpRoomNo(), -1,
                              0.0f, 0, 1, 0, shape_angle.y, 1, 0);
    }

    if (mProcID == PROC_WARP) {
        mProcVar3.field_0x300e = -1;
    }
}

void daAlink_c::checkWarpStart() {
    static char defaultPortal[13] = "POTAL_WARPIN";
    static char kBridgePortal[20] = "PORTAL_WARP_KBRIDGE";
    static char oBridgePortal[20] = "PORTAL_WARP_OBRIDGE";
    static char volcBomPortal[20] = "PORTAL_WARP_BIGVOLC";
    static char cannonPortal[22] = "SKY_CANNON_WARP_START";

    if (dMeter2Info_getWarpStatus() != 0) {
        if (dMeter2Info_getWarpStatus() == WARP_STATUS_DECIDED_e ||
            dComIfGp_event_compulsory(this, 0, 0xFFFF))
        {
            #if VERSION != VERSION_GCN_JPN
            onNoResetFlg0(FLG0_UNK_4000);
            #endif

            if (dMeter2Info_getWarpStatus() == WARP_STATUS_DECIDED_e) {
                const char* portal;
                if (!checkMidnaRide()) {
                    if (dComIfGp_TargetWarpPt_get() == 3) {
                        eventInfo.setArchiveName("Obj_kbrg");
                        portal = kBridgePortal;
                    } else if (dComIfGp_TargetWarpPt_get() == 6) {
                        eventInfo.setArchiveName("Obj_obrg");
                        portal = oBridgePortal;
                    } else if (dComIfGp_TargetWarpPt_get() == 9) {
                        eventInfo.setArchiveName("M_VolcBom");
                        portal = volcBomPortal;
                    } else if (dComIfGp_TargetWarpPt_get() == 8) {
                        eventInfo.setArchiveName("SCanCrs");
                        portal = cannonPortal;
                    } else {
                        portal = defaultPortal;
                    }
                } else {
                    dComIfGp_TargetWarpPt_set(0xFF);
                    portal = defaultPortal;
                }
                fopAcM_orderOtherEvent(this, portal, 0xFFFF, 1, 1);
            } else {
                mDemo.setSpecialDemoType();
                #if VERSION == VERSION_GCN_JPN
                onNoResetFlg0(FLG0_UNK_4000);
                #endif

                if (dMeter2Info_getWarpStatus() == 1) {
                    stage_stag_info_class* stag_info = dComIfGp_getStageStagInfo();
                    u32 exitID = dStage_stagInfo_GetEscapeWarp(stag_info);
                    dStage_changeScene(exitID, 0.0f, 0, -1, shape_angle.y, -1);
                } else {
                    dungeonReturnWarp();
                }
            }
        }
        g_meter2_info.resetWarpStatus();
    }
}

int daAlink_c::warpModelTexScroll() {
    field_0x3478 += 0.15f;
    if (field_0x3478 >= 1.0f) {
        field_0x3478 -= 1.0f;
    }

    int temp_r30 = cLib_chaseF(&field_0x347c, field_0x3480, 0.06f);
    field_0x3484 = cLib_minMaxLimit<f32>(0.5f * field_0x347c, 0.0f, 1.0f);

    dRes_info_c::setWarpSRT(field_0x064C, current.pos, field_0x3478, field_0x347c);
    dRes_info_c::setWarpSRT(mSwordModel->getModelData(), current.pos, field_0x3478, field_0x347c);
    dRes_info_c::setWarpSRT(mShieldModel->getModelData(), current.pos, field_0x3478, field_0x347c);
    dRes_info_c::setWarpSRT(mSheathModel->getModelData(), current.pos, field_0x3478, field_0x347c);

    if (checkWolf()) {
        dRes_info_c::setWarpSRT(mpWlChainModels[0]->getModelData(), current.pos, field_0x3478, field_0x347c);
    } else {
        dRes_info_c::setWarpSRT(mpLinkFaceModel->getModelData(), current.pos, field_0x3478, field_0x347c);
        dRes_info_c::setWarpSRT(mpLinkHatModel->getModelData(), current.pos, field_0x3478, field_0x347c);
        dRes_info_c::setWarpSRT(mpLinkHandModel->getModelData(), current.pos, field_0x3478, field_0x347c);
        dRes_info_c::setWarpSRT(mpLinkBootModels[0]->getModelData(), current.pos, field_0x3478, field_0x347c);
    }

    return temp_r30;
}

int daAlink_c::procCoWarpInit(int param_0, int param_1) {
    if (!commonProcInitNotSameProc(PROC_WARP)) {
        return 0;
    }

    changeWarpMaterial(WARP_MAT_MODE_0);
    mNormalSpeed = 0.0f;
    deleteEquipItem(FALSE, TRUE);

    if (checkWolf()) {
        setSingleAnimeWolfBaseSpeed(WANM_WAIT, mpHIO->mWolf.mWlMove.m.mIdleAnmSpeed, 3.0f);
    } else {
        setSingleAnimeBaseSpeed(ANM_WAIT, mpHIO->mMove.m.mWaitAnmSpeed, 3.0f);
    }

    u32 var_r29;
    BOOL var_r27 = param_1 == 0 && ((checkStageName("F_SP125") && fopAcM_GetRoomNo(this) == 4) ||
                                    (checkStageName("D_MN08") && fopAcM_GetRoomNo(this) == 0));

    field_0x3478 = 0.0f;
    mProcVar2.field_0x300c = param_0;
    mProcVar5.field_0x3012 = 1;
    mProcVar1.field_0x300a = 0;
    mProcVar4.field_0x3010 = 0xFF;
    field_0x3488 = 1.0f;

    if (mProcVar2.field_0x300c == 0) {
        field_0x3480 = -0.5f;
        mProcVar0.field_0x3008 = 0;

        if (checkWolf()) {
            field_0x347c = 3.5f;

            if (var_r27) {
                field_0x32cc = 0x88C8;
                getMidnaActor()->onSideWarp();
                var_r29 = Z2SE_WOLF_WARP_IN_YOKO;
            } else {
                field_0x32cc = 0x9FB;
                var_r29 = Z2SE_WOLF_WARP_IN_TATE;
            }

            #if VERSION == VERSION_GCN_JPN
            if (param_1) {
                onNoResetFlg0(FLG0_UNK_4000);
            }
            #endif

            if (param_1) {
                if (!checkMidnaRide()) {
                    onPlayerNoDraw();
                }
            }
        } else {
            field_0x347c = 5.5f;

            if (var_r27) {
                field_0x32cc = 0x88C7;
                var_r29 = Z2SE_AL_WARP_IN_YOKO;
            } else {
                var_r29 = Z2SE_AL_WARP_IN_TATE;

                if (checkBossRoom() && fopAcM_GetRoomNo(this) == 50) {
                    char sp8[32];
                    strcpy(sp8, dComIfGp_getStartStageName());

                    for (int i = 0; i < 32; i++) {
                        s64 value = sp8[i];

                        if (value == 0) {
                            sp8[i - 1] = 0;
                            break;
                        }
                    }

                    if (checkItemGet(fpcNm_ITEM_DUNGEON_EXIT, 1) ||
                        (checkItemGet(fpcNm_ITEM_DUNGEON_BACK, 1) &&
                         strcmp(sp8, dComIfGs_getWarpStageName()) == 0))
                    {
                        dComIfGs_setItem(SLOT_18, fpcNm_ITEM_NONE);
                        dComIfGs_resetLastWarpAcceptStage();
                    }
                }

                field_0x32cc = 0x9F4;
            }
        }
        field_0x3484 = 1.0f;
    } else {
        field_0x347c = -0.5f;
        mProcVar0.field_0x3008 = 45;

        if (checkWolf()) {
            field_0x3480 = 3.5f;
            mProcVar5.field_0x3012 = 0;
            field_0x3488 = -1.0f;
        } else {
            field_0x3480 = 5.5f;
            var_r29 = Z2SE_AL_WARP_OUT;
            setEmitter(field_0x3240, 0x9F3, &current.pos, &shape_angle);
        }

        field_0x3484 = 0.0f;
        if (!mLinkAcch.ChkGroundHit()) {
            setSpecialGravity(0.0f, maxFallSpeed, 0);
            mProcVar1.field_0x300a = 1;
        }

        kytag04_class* kytag04_p =
            (kytag04_class*)fopAcIt_Judge((fopAcIt_JudgeFunc)daAlink_searchPortal, &current.pos);
        if (kytag04_p != NULL) {
            mProcVar4.field_0x3010 = kytag04_p->mNeedDropNum;
        }
    }

    if (field_0x3488 > 0.0f) {
        seStartOnlyReverb(var_r29);
    }

    mProcVar3.field_0x300e = param_1;
    warpModelTexScroll();
    return 1;
}


int daAlink_c::procCoWarp() {
    if (mProcVar1.field_0x300a != 0 && mLinkAcch.ChkGroundHit()) {
        mProcVar1.field_0x300a = 0;

        f32 var_f1;
        if (checkWolf()) {
            var_f1 = mpHIO->mWolf.mWlAutoJump.m.mGravity;
        } else {
            var_f1 = mpHIO->mAutoJump.m.mGravity;
        }

        setSpecialGravity(var_f1, maxFallSpeed, 1);
    }

    if (mDemo.getDemoMode() == daPy_demo_c::DEMO_UNK_45_e) {
        if (mProcVar5.field_0x3012 == 0) {
            mProcVar5.field_0x3012 = 1;
        }

        if (checkWolf() && mProcVar2.field_0x300c != 0) {
            for (int i = 0; i < 6; i++) {
                static u16 const effName[] = {0x09F5, 0x09F6, 0x09F7, 0x09F8, 0x09F9, 0x09FA};
                setEmitter(&field_0x3240[i], effName[i], &current.pos, &shape_angle);
            }
        }
    }

    if (mProcVar2.field_0x300c != 0 && !checkWolf()) {
        setEmitter(&field_0x3240[0], 0x9F3, &current.pos, &shape_angle);
    }

    if (mProcVar5.field_0x3012 != 0 && field_0x3488 < 0.0f) {
        seStartOnlyReverb(Z2SE_WOLF_WARP_OUT);
        field_0x3488 = 1.0f;
    }

    if (mProcVar5.field_0x3012 != 0) {
        if (mProcVar0.field_0x3008 != 0) {
            mProcVar0.field_0x3008--;
        } else if (warpModelTexScroll()) {
            if (mProcVar2.field_0x300c != 0) {
                if (mDemo.getDemoMode() == daPy_demo_c::DEMO_UNK_45_e) {
                    if (mProcVar1.field_0x300a != 0) {
                        if (field_0x3174 == 4) {
                            dComIfGp_setNextStage(dComIfGp_getStartStageName(), mProcVar4.field_0x3010,
                                                  fopAcM_GetRoomNo(this), -1, 0.0f, 5, 1, 0,
                                                  shape_angle.y, 1, 0);
                            changeOriginalDemo();
                            changeDemoMode(1, 0, 0, 0);
                        } else {
                            resetSpecialEvent();
                        }

                        return commonFallInit(1);
                    }

                    dComIfGp_evmng_cutEnd(mAlinkStaffId);
                } else {
                    return checkWaitAction();
                }
            } else if (mProcVar3.field_0x300e != 0) {
                skipPortalObjWarp();
            } else {
                dComIfGp_evmng_cutEnd(mAlinkStaffId);
            }
        } else if (mProcVar2.field_0x300c == 0 && !checkNoResetFlg0(FLG0_PLAYER_NO_DRAW)) {
            cXyz sp10(current.pos.x, current.pos.y + (field_0x347c - -0.5f) * 30.0f, current.pos.z);
            setEmitter(&field_0x3240[0], field_0x32cc, &sp10, &shape_angle);
        }
    }

    return 1;
}


int daAlink_c::commonWaitTurnInit() {
    if (checkWolf()) {
        return procWolfWaitTurnInit();
    } else {
        return procWaitTurnInit();
    }
}

int daAlink_c::commonGrabPutInit() {
    if (mGrabItemAcKeep.getActor() == NULL) {
        if (mProcID != PROC_GRAB_STAND &&
            (mProcID != PROC_WOLF_GRAB_PUT || !(mUnderFrameCtrl[0].getFrame() < field_0x3478)))
        {
            dComIfGp_evmng_cutEnd(mAlinkStaffId);
            return 1;
        }
    }

    if (checkWolf()) {
        return procWolfGrabPutInit();
    }

    return procGrabPutInit();
}

int daAlink_c::commonLargeDamageUpInit(int param_0, int param_1, s16 param_2, s16 param_3) {
    if (checkWolf()) {
        return procWolfLargeDamageUpInit(param_0, param_1, param_2, param_3);
    } else {
        return procLargeDamageUpInit(param_0, param_1, param_2, param_3);
    }
}

int daAlink_c::commonFallInit(int param_0) {
    if (checkWolf()) {
        return procWolfFallInit(param_0, mpHIO->mWolf.mWlAutoJump.m.mNormalFallInterp);
    } else {
        return procFallInit(param_0, mpHIO->mAutoJump.m.mFallInterpolation);
    }
}
