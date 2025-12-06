#include "d/actor/d_a_alink.h"
#include "d/d_com_inf_game.h"
#include "d/actor/d_a_obj_carry.h"
#include "d/d_bg_w.h"
#include "d/actor/d_a_cow.h"
#include "d/actor/d_a_nbomb.h"
#include "d/actor/d_a_obj_swchain.h"
#include "d/actor/d_a_obj_movebox.h"
#include "d/actor/d_a_obj_gra2.h"

static bool daAlink_checkLightBallA(fopAc_ac_c* p_actor) {
    if (fopAcM_GetName(p_actor) == PROC_Obj_Carry &&
        static_cast<daObjCarry_c*>(p_actor)->getType() == daObjCarry_c::TYPE_BALL_S)
    {
        return true;
    }
    return false;
}

static bool daAlink_checkLightBallB(fopAc_ac_c* p_actor) {
    if (fopAcM_GetName(p_actor) == PROC_Obj_Carry &&
        static_cast<daObjCarry_c*>(p_actor)->getType() == daObjCarry_c::TYPE_BALL_S_2)
    {
        return true;
    }
    return false;
}

static fopAc_ac_c* daAlink_searchLightBall(fopAc_ac_c* p_actor, void* param_1) {
    if ((daPy_py_c::checkCarryStartLightBallA() && daAlink_checkLightBallA(p_actor)) ||
        (daPy_py_c::checkCarryStartLightBallB() && daAlink_checkLightBallB(p_actor)))
    {
        return p_actor;
    }
    return NULL;
}

BOOL daAlink_c::checkGrabLineCheck() {
    cXyz sp18(field_0x27f4->attention_info.position.x, field_0x27f4->attention_info.position.y + 10.0f, field_0x27f4->attention_info.position.z);
    if (fopAcM_lc_c::lineCheck(&attention_info.position, &sp18, field_0x27f4) && fopAcM_lc_c::checkWallHit()) {
        return true;
    }

    return false;
}

void daAlink_c::setGrabCollisionOffset(f32 param_0, f32 param_1, cBgS_PolyInfo* param_2) {
    if (param_2 != NULL) {
        mPolyInfo4.SetPolyInfo(*param_2);
    } else {
        mPolyInfo4.ClearPi();
    }

    field_0x342c = 0.8f * param_0;
    field_0x3430 = 0.8f * param_1;
}

BOOL daAlink_c::exchangeGrabActor(fopAc_ac_c* p_actor) {
    fopAc_ac_c* actor = mGrabItemAcKeep.getActor();
    if (actor == NULL) {
        return false;
    }

    fopAcM_cancelCarryNow(actor);
    actor->shape_angle.z = 0;
    actor->shape_angle.x = 0;

    if (checkGrabCarryActor()) {
        actor->current.angle.z = 0;
        actor->current.angle.x = 0;
    }

    setGrabItemActor(p_actor);
    return true;
}

BOOL daAlink_c::setForceGrab(fopAc_ac_c* p_actor, int param_1, int param_2) {
    if (((param_1 != 0 || !checkEventRun()) && !checkWolf()) &&
        (((mEquipItem == fpcNm_ITEM_NONE || (param_2 != 0 && checkHookshotItem(mEquipItem)))) &&
         ((checkModeFlg(MODE_UNK_10000000) &&
           (checkHorseNoUpperAnime() || checkHorseTurnAnime())) ||
          param_2 != 0 && checkHookshotAnime())))
    {
        deleteEquipItem(TRUE, FALSE);
        setGrabItemActor(p_actor);
        field_0x33e4 = 38.0f;
        setGrabUpperAnime(mpHIO->mBasic.m.mBasicInterpolation);
        return true;
    }

    return false;
}

f32 daAlink_c::getGrabThrowRate() {
    if (checkHeavyStateOn(1, 1)) {
        return 0.6f * mHeavySpeedMultiplier;
    }

    return 0.6f;
}

BOOL daAlink_c::checkGrabThrowAnime() const {
    return checkGrabUpThrowAnime() || checkGrabSideThrowAnime() || checkGrabHeavyThrowAnime();
}

BOOL daAlink_c::checkGrabAnime() const {
    return checkGrabAnimeUp() || checkGrabAnimeSide() || checkGrabAnimeCarry();
}

BOOL daAlink_c::checkGrabAnimeAndThrow() const {
    return checkGrabAnime() || checkGrabThrowAnime();
}

BOOL daAlink_c::checkGrabCarryActor() {
    return mGrabItemAcKeep.getActor() != NULL &&
           fopAcM_CheckCarryType(mGrabItemAcKeep.getActor(), fopAcM_CARRY_TYPE_8);
}

BOOL daAlink_c::checkGrabSlowMoveActor() {
    daObjCarry_c* var_r31 = (daObjCarry_c*)mGrabItemAcKeep.getActor();
    return var_r31 != NULL &&
          (fopAcM_CheckCarryType(var_r31, fopAcM_CARRY_HEAVY) ||
          (fopAcM_GetName(var_r31) == PROC_Obj_Carry && (var_r31->getType() == 1 || var_r31->getType() == 10 || var_r31->getType() == 4)));
}

BOOL daAlink_c::checkGrabHeavyActor() {
    return mGrabItemAcKeep.getActor() != NULL &&
           fopAcM_CheckCarryType(mGrabItemAcKeep.getActor(), fopAcM_CARRY_HEAVY);
}

BOOL daAlink_c::checkGrabSideActor() {
    return mGrabItemAcKeep.getActor() != NULL &&
           fopAcM_CheckCarryType(mGrabItemAcKeep.getActor(), fopAcM_CARRY_SIDE);
}

void daAlink_c::setGrabUpperAnime(f32 param_0) {
    if (param_0 > 0.0f && checkNoResetFlg0(FLG0_UNDERWATER)) {
        if (checkZoraWearAbility()) {
            param_0 *= 1.0f / mpHIO->mItem.mIronBoots.m.mZoraWaterAnmSpeed;
        } else {
            param_0 *= 1.0f / mpHIO->mItem.mIronBoots.m.mWaterWalkAnmRate;
        }
    }

    field_0x33e8 = 0.0f;

    if (checkGrabCarryActor()) {
        setUpperAnimeBaseSpeed(0x60, 0.0f, param_0);
        setFacePriBck(0xFA);
    } else if (checkGrabSideActor() != 0) {
        setUpperAnimeBaseSpeed(0x50, 0.0f, param_0);
        setFacePriBck(0x13D);
        field_0x33ec = 0.0f;
    } else {
        setUpperAnimeBaseSpeed(0x16C, 0.0f, param_0);
        setFacePriBck(0x13D);
        field_0x33ec = 0.0f;
    }

    field_0x2f96 = 0xFE;
    field_0x2f97 = 0xFE;
}

BOOL daAlink_c::checkGrabRooster() {
    if (mGrabItemAcKeep.getActor() != NULL &&
        (fopAcM_GetName(mGrabItemAcKeep.getActor()) == PROC_NI ||
         fopAcM_GetName(mGrabItemAcKeep.getActor()) == PROC_NPC_TKJ2))
    {
        return true;
    }

    return false;
}

void daAlink_c::setGrabItemPos() {
    static cXyz grabCarryOffset0(0.0f, -30.0f, 25.0f);
    static cXyz grabCarryOffset1(0.0f, 20.0f, -5.0f);
    static cXyz grabCarryOffset2(-3.0f, 30.0f, -30.0f);
    static cXyz grabCarryOffset3(-4.5f, 26.0f, -33.0f);

    fopAc_ac_c* temp_r3 = mGrabItemAcKeep.getActor();
    if (temp_r3 != NULL) {
        daPy_frameCtrl_c* sp1C = &mUnderFrameCtrl[0];

        s8 sp8 = fopAcM_GetRoomNo(this);
        temp_r3->tevStr.room_no = sp8;
        temp_r3->tevStr.YukaCol = tevStr.YukaCol;
        fopAcM_SetRoomNo(temp_r3, sp8);
    
        BOOL sp18 = checkGrabCarryActor();
        s16 spE;
        s16 spC;
        if (sp18) {
            spE = (shape_angle.y + 0x8000);
            spC = -field_0x2fee;
        } else {
            spE = shape_angle.y;
            spC = (field_0x2fee + field_0x3082);
        }
    
        if (checkGrabSideThrowAnime()) {
            mDoMtx_multVecZero(mpLinkModel->getAnmMtx(0xA), &temp_r3->current.pos);
            temp_r3->shape_angle.y = spE;
            temp_r3->shape_angle.z = spC;
            temp_r3->shape_angle.x = shape_angle.x;
        } else if (mProcID == PROC_GRAB_READY || mProcID == PROC_GRAB_MISS || mProcID == PROC_PICK_UP || (mProcID == PROC_WOLF_GRAB_UP && mProcVar3.field_0x300e == 0)) {
            mDoMtx_stack_c::transS(current.pos);
            mDoMtx_stack_c::YrotM((shape_angle.y - mProcVar2.field_0x300c));
            mDoMtx_stack_c::multVec(&field_0x37c8, &temp_r3->current.pos);
            if (sp18 && mProcID == PROC_GRAB_READY) {
                cLib_chaseAngleS(&temp_r3->shape_angle.y, spE, 0x1000);
                temp_r3->current.angle.y = temp_r3->shape_angle.y;
            }
        } else if (checkWolf()) {
            mDoMtx_stack_c::copy(mpLinkModel->getAnmMtx(4));

            if (!fopAcM_CheckCarryType(temp_r3, fopAcM_CARRY_ITEM)) {
                mDoMtx_stack_c::transM(78.0f, 42.0f, 0.0f);
                mDoMtx_stack_c::ZrotM(cM_deg2s(138.0f));
            } else {
                if (fopAcM_GetName(temp_r3) == PROC_OBJ_FOOD) {
                    mDoMtx_stack_c::transM(32.0f, 7.0f, 0.0f);
                } else {
                    mDoMtx_stack_c::transM(30.0f, 12.0f, 0.0f);
                }

                mDoMtx_stack_c::XYZrotM(cM_deg2s(132.0f), cM_deg2s(-90.0f), 0);
            }

            mDoMtx_stack_c::multVecZero(&temp_r3->current.pos);
            mDoMtx_MtxToRot(mDoMtx_stack_c::get(), &temp_r3->shape_angle);
        } else {
            BOOL sp14 = fopAcM_GetName(temp_r3) == PROC_NPC_TKJ2;

            if (mProcID == PROC_GRAB_UP) {
                cLib_chaseAngleS(&temp_r3->shape_angle.y, spE, 0x1000);
            } else {
                temp_r3->shape_angle.y = spE;
                temp_r3->shape_angle.z = spC;
                temp_r3->shape_angle.x = shape_angle.x;
            }

            cXyz sp140;
            cXyz sp134;
            if (mProcID == PROC_GRAB_UP) {
                sp134 = temp_r3->current.pos;
            }

            if (sp18) {
                s16 spA = shape_angle.x + (field_0x33e8 * cM_deg2s(30.0f));

                cXyz sp128;
                cXyz sp11C;
                mDoMtx_multVecZero(mFootData2[0].field_0x14[2], &sp128);
                mDoMtx_multVecZero(mFootData2[1].field_0x14[2], &sp11C);

                temp_r3->current.pos = (sp128 + sp11C) * 0.5f;
                mDoMtx_stack_c::ZXYrotS(spA, shape_angle.y, shape_angle.z);

                if (mProcID == PROC_GRAB_UP || mProcID == PROC_GRAB_PUT) {
                    if (sp1C->getFrame() < field_0x3478) {
                        f32 temp_f31 = (sp1C->getFrame() - 9.0f) / (field_0x3478 - 9.0f);
                        sp140 = (grabCarryOffset0 * (1.0f - temp_f31)) + (grabCarryOffset1 * temp_f31);
                        mDoMtx_stack_c::multVec(&sp140, &sp140);
                    } else {
                        f32 temp_f31 = (sp1C->getFrame() - field_0x3478) / (sp1C->getEnd() - field_0x3478);
                        sp140 = (grabCarryOffset1 * (1.0f - temp_f31)) + (grabCarryOffset2 * temp_f31);
                        mDoMtx_stack_c::multVec(&sp140, &sp140);
                    }
                } else {
                    sp140 = (grabCarryOffset2 * (1.0f - field_0x33e8)) + (grabCarryOffset3 * field_0x33e8);
                    mDoMtx_stack_c::multVec(&sp140, &sp140);
                }

                temp_r3->current.pos += sp140;
                temp_r3->shape_angle.x = -spA;

                temp_r3->current.angle.x = temp_r3->shape_angle.x;
                temp_r3->current.angle.y = temp_r3->shape_angle.y;
                temp_r3->current.angle.z = temp_r3->shape_angle.z;
            } else {
                sp140 = (mLeftHandPos + mRightHandPos) * 0.5f;
                mDoMtx_stack_c::transS(sp140);

                concatMagneBootMtx();
                mDoMtx_stack_c::ZXYrotM(shape_angle.x, spE, spC);

                sp140.set(0.0f, -3.0f, -8.0f);
                if (sp14) {
                    sp140.y += -15.0f;
                }

                mDoMtx_stack_c::multVec(&sp140, &temp_r3->current.pos);
    
                if (checkGrabAnimeSide()) {
                    static Vec const localSidePos = {2.3f, -15.5f, 15.6f};
                    mDoMtx_multVec(mpLinkModel->getAnmMtx(0xA), &localSidePos, &sp140);
                    temp_r3->current.pos = (temp_r3->current.pos * (1.0f - field_0x33e8)) + (sp140 * field_0x33e8);
                }
            }

            if (checkUnderMove0BckNoArc(ANM_GRAB_UP)) {
                f32 temp_f31_3 = 1.0f - ((sp1C->getFrame() - sp1C->getStart()) / (sp1C->getEnd() - sp1C->getStart()));

                sp140.x = 0.0f;
                sp140.z = field_0x33e4 * temp_f31_3;

                f32 var_f30;
                if (sp14) {
                    var_f30 = -18.0f;
                } else {
                    var_f30 = -3.0f;
                }

                if (mProcID == 0x70) {
                    sp140.y = temp_f31_3 * (-13.789083f - var_f30);
                } else {
                    sp140.y = temp_f31_3 * (-13.825373f - var_f30);
                }

                mDoMtx_stack_c::multVecSR(&sp140, &sp140);
                temp_r3->current.pos += sp140;
            }

            if (mProcID == PROC_GRAB_PUT) {
                if (checkMagneBootsOn()) {
                    cXyz sp110 = temp_r3->current.pos - current.pos;
                    mDoMtx_multVecSR(mMagneBootInvMtx, &sp110, &sp110);

                    sp110.y = field_0x598;
                    mDoMtx_multVecSR(mMagneBootMtx, &sp110, &sp110);

                    sp110 += current.pos;

                    cXyz sp104;
                    mDoMtx_multVecSR(mMagneBootMtx, &cXyz::BaseY, &sp104);

                    sp104 = sp110 - (sp104 * field_0x598);

                    if (grabLineCheck(&sp110, &sp104)) {
                        if (temp_r3->current.pos.abs(sp110) > mObjLinChk.GetCrossP()->abs(sp110)) {
                            temp_r3->speedF = 0.0f;
                            temp_r3->current.pos = mObjLinChk.GetCross();
                            freeGrabItem();
                            return;
                        }
                    }
                } else {
                    cXyz spF8(temp_r3->current.pos.x, current.pos.y + field_0x598, temp_r3->current.pos.z);
                    mObjGndChk.SetPos(&spF8);

                    f32 temp_f1 = dComIfG_Bgsp().GroundCross(&mObjGndChk);
                    if (temp_f1 > temp_r3->current.pos.y) {
                        temp_r3->speedF = 0.0f;
                        temp_r3->current.pos.y = temp_f1;
                        freeGrabItem();
                        return;
                    }
                }
            }

            if (mProcID == PROC_GRAB_UP && mUnderFrameCtrl[0].getFrame() < 6.0f && temp_r3->current.pos.y < sp134.y) {
                temp_r3->current.pos = sp134;
            }
        }
        return;
    }

    if (mEquipItem == 0x102) {
        fopAc_ac_c* sp10 = mItemAcKeep.getActor();
        if (sp10 != NULL) {
            mDoMtx_stack_c::copy(mpLinkModel->getAnmMtx(field_0x30c0));
            mDoMtx_stack_c::transM(7.0f, -2.0f, 1.0f);
            mDoMtx_stack_c::XYZrotM(cM_deg2s(71.0f), cM_deg2s(-6.6f), cM_deg2s(3.2f));
            mDoMtx_stack_c::multVecZero(&sp10->current.pos);
            mDoMtx_MtxToRot(mDoMtx_stack_c::get(), &sp10->shape_angle);
        }
    }
}

void daAlink_c::freeGrabItem() {
    fopAc_ac_c* temp_r3 = mGrabItemAcKeep.getActor();

    if (temp_r3 != NULL) {
        if (dComIfGp_event_runCheck() || mDemo.getDemoType() == daPy_demo_c::DEMO_TYPE_SPECIAL_e) {
            fopAcM_OnStatus(temp_r3, 0x800);
        }

        fopAcM_cancelCarryNow(temp_r3);

        temp_r3->shape_angle.z = 0;
        temp_r3->shape_angle.x = 0;

        if (checkGrabCarryActor()) {
            temp_r3->current.angle.z = 0;
            temp_r3->current.angle.x = 0;
        }

        if (checkModeFlg(0x400)) {
            fopAc_ac_c* temp_r3_2 = mRideAcKeep.getActor();
            if (temp_r3_2 != NULL) {
                if (temp_r3_2->speedF > 0.0f) {
                    temp_r3->speedF += temp_r3_2->speedF;
                }
                temp_r3->current.angle.y = temp_r3_2->shape_angle.y;
            }
        } else {
            cXyz sp14(current.pos.x, field_0x34e0.y, current.pos.z);

            if (grabLineCheck(&sp14, &temp_r3->current.pos)) {
                temp_r3->current.pos = mObjLinChk.GetCross();

                cM3dGPla sp20;
                dComIfG_Bgsp().GetTriPla(mObjLinChk, &sp20);
                if (cBgW_CheckBWall(sp20.mNormal.y)) {
                    temp_r3->current.pos += sp20.mNormal * 10.0f;
                }
            }
        }
    }

    mGrabItemAcKeep.clearData();

    if (checkGrabAnime() || checkWolfGrabAnime()) {
        resetUpperAnime(UPPER_2, 3.0f);
    }

    for (int i = 0; i < 2; i++) {
        field_0x312a[i] = csXyz::Zero;
        field_0x3136[i] = csXyz::Zero;
    }
}

void daAlink_c::setGrabUpperSpeedRate() {
    f32 var_f31;

    if (checkModeFlg(0x400)) {
        if (checkCanoeRide() != 0) {
            var_f31 = fabsf(mNormalSpeed / getCanoeMaxSpeed());
        } else {
            var_f31 = fabsf(0.02f * mNormalSpeed);
        }
    } else if (checkAttentionLock() && !checkGrabCarryActor()) {
        var_f31 = 1.0f;
    } else {
        if (mProcID == PROC_STEP_MOVE) {
            var_f31 = fabsf(field_0x3478 / mpHIO->mMove.m.mMaxSpeed);
        } else {
            var_f31 = fabsf(mNormalSpeed / mpHIO->mMove.m.mMaxSpeed);
        }

        if (checkHeavyStateOn(1, 1)) {
            var_f31 *= 1.0f / (mHeavySpeedMultiplier * mHeavySpeedMultiplier);
        }
    }

    if (var_f31 > 1.0f) {
        var_f31 = 1.0f;
    }

    cLib_chaseF(&field_0x33e8, var_f31, 0.15f);
    
    daPy_frameCtrl_c* temp_r30 = &mUpperFrameCtrl[2];
    temp_r30->setFrame(field_0x33e8 * temp_r30->getEnd());
    getNowAnmPackUpper(UPPER_2)->setFrame(temp_r30->getFrame());
}

void daAlink_c::setCarryArmAngle(f32 param_0, f32 param_1) {
    if (0.0f != field_0x33ec) {
        param_1 *= field_0x33ec;
        s16 temp_r29 = -1500.0f * param_0 * field_0x33ec;

        field_0x312a[0].set(0, temp_r29, temp_r29);
        field_0x3136[0].set(0, 0, temp_r29);

        field_0x312a[1].set(0, temp_r29 + (2000.0f * param_1), temp_r29 + (2000.0f * param_1));
        field_0x3136[1].set(0, 0, temp_r29 - (3500.0f * param_1));
    
        if (param_1 < 0.0f) {
            field_0x312a[0].z += (s16)(2500.0f * param_1);
            field_0x3136[0].y += (s16)(2000.0f * param_1);
        } else {
            field_0x3136[0].y += (s16)(4000.0f * param_1);
        }
    } else if (checkGrabRooster()) {
        field_0x3136[0].y = -5000.0f * param_1;
        field_0x3136[1].y = 5000.0f * param_1;
    }
}

BOOL daAlink_c::checkGrabNotThrow() {
    return checkGrabCarryActor();
}

BOOL daAlink_c::checkNextActionGrab() {
    fopAc_ac_c* temp_r3 = mGrabItemAcKeep.getActor();
    if (temp_r3 == NULL) {
        freeGrabItem();
        return 0;
    }

    setCarryArmAngle(0.0f, 1.0f);

    if (mTargetedActor == NULL && mAttList != NULL && mAttList->mType == fopAc_attn_DOOR_e && fopAcM_CheckStatus(temp_r3, 0x2000000)) {
        setDoStatus(6);
    } else if ((field_0x27f4 == NULL) || !checkGrabTalkActor(field_0x27f4) || !setTalkStatus()) {
        if (checkModeFlg(0x400)) {
            setDoStatus(0x13);
        } else if ((!checkAttentionState() && field_0x33a8 <= getGrabThrowRate()) || checkGrabNotThrow()) {
            setDoStatus(0x14);
        } else {
            setDoStatus(0x13);
        }
    }

    if (doTrigger() && dComIfGp_getDoStatus() == 6) {
        fopAcM_orderDoorEvent(this, field_0x27f4, 0, 0);
        return 1;
    }

    if (orderTalk(1)) {
        return 1;
    }

    if (doTrigger() || (fopAcM_GetName(temp_r3) == PROC_NBOMB && ((daNbomb_c*)temp_r3)->checkPlayerMake() && (checkSetItemTrigger(0x50) || (((daNbomb_c*)temp_r3)->checkWaterBomb() && checkSetItemTrigger(0x71)) || (!((daNbomb_c*)temp_r3)->checkWaterBomb() && checkSetItemTrigger(0x70))))) {
        if (dComIfGp_getDoStatus() == 0x14) {
            return procGrabPutInit();
        }

        if (dComIfGp_getDoStatus() == 0x13) {
            return procGrabThrowInit(0);
        }
    }

    return 0;
}

void daAlink_c::initGrabNextMode() {
    if (!checkGrabAnime()) {
        setGrabUpperAnime(-1.0f);
    }

    checkNextAction(0);
}

void daAlink_c::setGrabItemThrow() {
    fopAc_ac_c* temp_r3 = mGrabItemAcKeep.getActor();
    if (temp_r3 != NULL) {
        cXyz sp14;
        temp_r3->current.angle.y = shape_angle.y;

        if (checkWolf()) {
            temp_r3->speedF = mpHIO->mItem.mBomb.m.mWolfThrowSpeedH;
            temp_r3->speed.y = mpHIO->mItem.mBomb.m.mWolfThrowSpeedV;
        } else {
            f32 var_f31;
            f32 var_f30;
            if (checkNoResetFlg0(FLG0_UNDERWATER)) {
                var_f31 = mpHIO->mItem.mBomb.m.mWaterThrowSpeedH;
                var_f30 = mpHIO->mItem.mBomb.m.mWaterThrowSpeedV;
            } else {
                var_f31 = mpHIO->mItem.mBomb.m.mThrowSpeedH;
                var_f30 = mpHIO->mItem.mBomb.m.mThrowSpeedV;
            }

            if (checkMagneBootsOn()) {
                sp14.set(0.0f, var_f30, var_f31);
                mDoMtx_stack_c::copy(mMagneBootMtx);
                mDoMtx_stack_c::YrotM(shape_angle.y);
                mDoMtx_stack_c::multVecSR(&sp14, &sp14);

                temp_r3->speedF = sp14.absXZ();
                temp_r3->speed.y = sp14.y;
                temp_r3->current.angle.y = sp14.atan2sX_Z();

                sp14.set(0.0f, 15.0f, 90.0f);
                mDoMtx_stack_c::multVecSR(&sp14, &sp14);
                temp_r3->current.pos += sp14;
            } else {
                temp_r3->speedF = var_f31;
                temp_r3->speed.y = var_f30;
                temp_r3->current.angle.x = getBodyAngleXAtnActor(0);

                temp_r3->current.pos.x += 90.0f * cM_ssin(shape_angle.y);
                temp_r3->current.pos.y += 15.0f;
                temp_r3->current.pos.z += 90.0f * cM_scos(shape_angle.y);
            }

            if (mTargetedActor != NULL && checkGrabSideThrowAnime()) {
                sp14 = mTargetedActor->eyePos - temp_r3->current.pos;
                multVecMagneBootInvMtx(&sp14);
                temp_r3->current.angle.y = sp14.atan2sX_Z();
                temp_r3->shape_angle.y = temp_r3->current.angle.y;
            }

            if (fopAcM_CheckCarryType(temp_r3, fopAcM_CARRY(fopAcM_CARRY_UNK_40 | fopAcM_CARRY_HEAVY))) {
                voiceStart(Z2SE_AL_V_THROW_L);
            } else if (dBomb_c::checkBombActor(temp_r3)) {
                voiceStart(Z2SE_AL_V_ATTACK_S);
            } else {
                voiceStart(Z2SE_AL_V_THROW_S);
            }
        }

        onResetFlg0(RFLG0_GRAB_THROW);
        freeGrabItem();
    }
}

BOOL daAlink_c::checkUpperGrabItemThrow(f32 param_0) {
    if (checkAnmEnd(&mUpperFrameCtrl[2])) {
        resetUpperAnime(UPPER_2, param_0);
        return 1;
    }

    if (mUpperFrameCtrl[2].checkPass(field_0x343c)) {
        setGrabItemThrow();
    }

    return 0;
}

void daAlink_c::putObjLineCheck(dBgS_LinChk& param_0, cXyz* param_1, fopAc_ac_c* param_2) {
    if (!checkNoCollisionCorret()) {
        param_0.Set(&field_0x37c8, param_1, param_2);

        if (dComIfG_Bgsp().LineCross(&param_0) && dBgS_CheckBWallPoly(param_0)) {
            cXyz sp8 = *param_1 - param_0.GetCross();
            f32 temp_f31 = sp8.abs();
            f32 temp_f1 = sp8.absXZ();

            if (temp_f1 > 0.01f) {
                f32 temp_f29 = temp_f31 / temp_f1;
                sp8.x *= temp_f29;
                sp8.z *= temp_f29;
            } else {
                sp8.x *= temp_f31;
                sp8.z *= temp_f31;
            }

            current.pos.x -= sp8.x;
            current.pos.z -= sp8.z;
            param_2->current.pos.x -= sp8.x;
            param_2->current.pos.z -= sp8.z;
        }
    }
}

bool daAlink_c::grabLineCheck(cXyz* i_start, cXyz* i_end) {
    mObjLinChk.Set(i_start, i_end, mGrabItemAcKeep.getActor());
    return dComIfG_Bgsp().LineCross(&mObjLinChk);
}

void daAlink_c::setGrabItemActor(fopAc_ac_c* param_0) {
    fopAcM_setCarryNow(param_0, 1);
    mGrabItemAcKeep.setData(param_0);

    mObjLinChk.ClrObj();
    mObjLinChk.ClrBomb();
    mObjLinChk.ClrStatue();
    mObjLinChk.ClrLink();
    mObjGndChk.ClrObj();
    mObjGndChk.ClrBomb();
    mObjGndChk.ClrStatue();
    mObjGndChk.ClrLink();

    s16 temp_r3 = fopAcM_GetName(param_0);
    if (dBomb_c::checkBombActor(param_0) || temp_r3 == PROC_Obj_Stone) {
        mObjLinChk.SetBomb();
        mObjGndChk.SetBomb();
    } else if (temp_r3 == PROC_CSTATUE) {
        mObjLinChk.SetStatue();
        mObjGndChk.SetStatue();
    } else if (temp_r3 == PROC_Obj_Carry && ((daObjCarry_c*)param_0)->prm_chk_type_ironball()) {
        mObjLinChk.SetLink();
        mObjGndChk.SetLink();
    } else {
        mObjLinChk.SetObj();
        mObjGndChk.SetObj();
    }
}

int daAlink_c::procGrabReadyInit() {
    if (fopAcM_checkCarryNow(field_0x27f4)) {
        return checkWaitAction();
    }

    if (mEquipItem != fpcNm_ITEM_NONE) {
        return procPreActionUnequipInit(PROC_GRAB_READY, field_0x27f4);
    }

    commonProcInit(PROC_GRAB_READY);
    setGrabItemActor(field_0x27f4);

    if (fopAcM_searchActorDistanceXZ2(this, field_0x27f4) > 1.0f) {
        shape_angle.y = fopAcM_searchActorAngleY(this, field_0x27f4);
    }

    current.angle.y = shape_angle.y;
    mProcVar2.field_0x300c = shape_angle.y;
    field_0x37c8 = field_0x27f4->current.pos - field_0x3510;
    mNormalSpeed = 0.0f;

    if (checkEquipAnime()) {
        resetUpperAnime(UPPER_2, -1.0f);
    }

    if (checkGrabCarryActor()) {
        setSingleAnime(ANM_CARRY, mpHIO->mGrab.m.mFailAnm.mSpeed, mpHIO->mGrab.m.mFailAnm.mStartFrame, 9, mpHIO->mGrab.m.mFailAnm.mInterpolation);
        mProcVar3.field_0x300e = 1;
        field_0x347c = 1.0f / (9.0f - mpHIO->mGrab.m.mFailAnm.mStartFrame);

        f32 var_f31;
        if (fopAcM_GetName(mGrabItemAcKeep.getActor()) == PROC_DO) {
            var_f31 = 0.93f;
        } else {
            var_f31 = 1.0f;
        }

        field_0x33ec = 4.0f * ((var_f31 * mGrabItemAcKeep.getActor()->scale.x) - 1.0f);
    } else {
        field_0x33ec = 0.0f;
        setSingleAnimeParam(ANM_GRAB_UP_START, &mpHIO->mGrab.m.mPrepareAnm);
        field_0x347c = 1.0f / (mpHIO->mGrab.m.mPrepareAnm.mEndFrame - mpHIO->mGrab.m.mPrepareAnm.mStartFrame);
        mProcVar3.field_0x300e = 0;
    }

    if (fopAcM_CheckCarryType(field_0x27f4, fopAcM_CARRY(fopAcM_CARRY_UNK_40 | fopAcM_CARRY_HEAVY | fopAcM_CARRY_TYPE_1))) {
        field_0x3198 = 1;
    } else {
        field_0x3198 = 0;
    }

    return 1;
}

int daAlink_c::procGrabReady() {
    if (mGrabItemAcKeep.getActor() == NULL) {
        return checkNextAction(0);
    }

    if (checkAnmEnd(&mUnderFrameCtrl[0])) {
        if (field_0x3198 != 0) {
            setSingleAnime(ANM_PICK_UP_LARGE_START, mpHIO->mGrab.m.mRecoilAnm.mSpeed, mpHIO->mGrab.m.mRecoilAnm.mStartFrame, mpHIO->mGrab.m.mRecoilAnm.mCancelFrame, mpHIO->mGrab.m.mRecoilAnm.mInterpolation);
            field_0x3198 = 0;
        } else {
            procGrabUpInit();
        }
    } else if (mProcVar3.field_0x300e != 0) {
        setCarryArmAngle(field_0x347c * (mUnderFrameCtrl[0].getFrame() - mpHIO->mGrab.m.mFailAnm.mStartFrame), 0.0f);
    } else {
        setCarryArmAngle(0.0f, field_0x347c * (mUnderFrameCtrl[0].getFrame() - mpHIO->mGrab.m.mPrepareAnm.mStartFrame));
    }

    return 1;
}

int daAlink_c::procGrabUpInit() {
    fopAc_ac_c* temp_r30 = mGrabItemAcKeep.getActor();
    commonProcInit(PROC_GRAB_UP);

    if (checkGrabCarryActor()) {
        setSingleAnime(ANM_CARRY, mpHIO->mGrab.m.mFailAnm.mSpeed, 9.0f + mpHIO->mGrab.m.mFailAnm.mSpeed, mpHIO->mGrab.m.mFailAnm.mEndFrame, 0.0f);
        mProcVar3.field_0x300e = 1;
        setCarryArmAngle(1.0f, 0.0f);
    } else {
        setSingleAnimeParam(ANM_GRAB_UP, &mpHIO->mGrab.m.mLiftBackAnm);
        mProcVar3.field_0x300e = 0;
        setCarryArmAngle(0.0f, 1.0f);
    }

    cXyz sp8 = temp_r30->current.pos - field_0x3510;
    field_0x33e4 = (sp8.absXZ() - 46.051205f) - -8.0f;

    if (fopAcM_CheckCarryType(temp_r30, fopAcM_CARRY(fopAcM_CARRY_UNK_40 | fopAcM_CARRY_HEAVY))) {
        voiceStart(Z2SE_AL_V_LIFTUP_L);
    } else {
        voiceStart(Z2SE_AL_V_LIFTUP_S);
    }

    fopAcM_setStageLayer(temp_r30);
    onResetFlg0(RFLG0_UNK_8000);
    field_0x3478 = 18.0f;
    return 1;
}

int daAlink_c::procGrabUp() {
    daPy_frameCtrl_c* temp_r29 = &mUnderFrameCtrl[0];

    fopAc_ac_c* temp_r3 = mGrabItemAcKeep.getActor();
    if (temp_r3 == NULL) {
        return checkNextAction(0);
    }

    if (checkAnmEnd(temp_r29) || (checkInputOnR() && (temp_r29->getFrame() > mpHIO->mGrab.m.mLiftBackAnm.mCancelFrame))) {
        onResetFlg0(RFLG0_GRAB_UP_END);

        if (fopAcM_CheckCarryType(temp_r3, fopAcM_CARRY(fopAcM_CARRY_UNK_40 | fopAcM_CARRY_HEAVY | fopAcM_CARRY_TYPE_1))) {
            procGrabReboundInit(0);
        } else {
            initGrabNextMode();
        }
    } else if (mProcVar3.field_0x300e != 0) {
        f32 var_f31;
        if (temp_r29->getFrame() < field_0x3478) {
            var_f31 = 0.0f;
        } else {
            var_f31 = (temp_r29->getFrame() - field_0x3478) / (temp_r29->getEnd() - field_0x3478);
        }

        setCarryArmAngle(1.0f - ((temp_r29->getFrame() - 9.0f) / (temp_r29->getEnd() - 9.0f)), var_f31);
    }

    return 1;
}

int daAlink_c::procGrabMiss() {
    if (mGrabItemAcKeep.getActor() == NULL) {
        checkNextAction(0);
    } else if (checkAnmEnd(&mUnderFrameCtrl[0])) {
        if (mProcVar0.field_0x3008 > 0) {
            mProcVar0.field_0x3008--;
        } else {
            procGrabStandInit();
        }
    }

    return 1;
}

int daAlink_c::procGrabThrowInit(int param_0) {
    if (checkModeFlg(0x400)) {
        if (checkGrabHeavyActor()) {
            setUpperAnimeParam(0x17B, UPPER_2, &mpHIO->mGrab.m.mIronBallThrowAnm);
            field_0x343c = 14.0f;
        } else if (checkGrabAnimeSide()) {
            setUpperAnimeParam(0x51, UPPER_2, &mpHIO->mGrab.m.mCarryPlaceReverseAnm);
            field_0x343c = 4.0f;
        } else {
            setUpperAnimeParam(0x170, UPPER_2, &mpHIO->mGrab.m.mLiftAnm);
            field_0x343c = 5.0f;
        }

        onNoResetFlg1(FLG1_UNK_10000000);
        return 1;
    }

    if (!commonProcInitNotSameProc(PROC_GRAB_THROW)) {
        return 0;
    }

    if (checkGrabHeavyActor()) {
        setSingleAnimeParam(ANM_THROW_HEAVY, &mpHIO->mGrab.m.mIronBallThrowAnm);
        field_0x343c = 14.0f;
        field_0x347c = mpHIO->mGrab.m.mIronBallThrowAnm.mCancelFrame;
        mProcVar2.field_0x300c = 2;
    } else if (checkGrabSideActor()) {
        f32 var_f31;
        if (mTargetedActor != NULL) {
            var_f31 = mpHIO->mAtnMove.m.mWaitAnmSpeed;
        } else {
            var_f31 = mpHIO->mNoActAtnMove.m.mWaitAnmSpeed;
        }

        setSingleAnimeBaseSpeed(ANM_ATN_WAIT_RIGHT, var_f31, mpHIO->mBasic.m.mBasicInterpolation);
        setUpperAnimeParam(0x51, UPPER_0, &mpHIO->mGrab.m.mCarryPlaceReverseAnm);
        field_0x343c = 4.0f;
        field_0x347c = mpHIO->mGrab.m.mCarryPlaceReverseAnm.mCancelFrame;
        mProcVar2.field_0x300c = 1;
    } else {
        setSingleAnimeParam(ANM_THROW, &mpHIO->mGrab.m.mLiftAnm);
        field_0x343c = 5.0f;
        field_0x347c = mpHIO->mGrab.m.mLiftAnm.mCancelFrame;
        mProcVar2.field_0x300c = 0;
    }

    field_0x3198 = param_0;
    field_0x3588 = l_halfAtnWaitBaseAnime;
    return 1;
}

int daAlink_c::procGrabThrow() {
    daPy_frameCtrl_c* var_r30;
    if (mProcVar2.field_0x300c == 1) {
        var_r30 = &mUpperFrameCtrl[0];
    } else {
        if (mProcVar2.field_0x300c == 2) {
            field_0x2f99 = 4;
            onEndResetFlg0(ERFLG0_UNK_8000000);
        }
        var_r30 = &mUnderFrameCtrl[0];
    }

    cLib_chaseF(&mNormalSpeed, 0.0f, mpHIO->mMove.m.mDeceleration);

    if (var_r30->checkPass(field_0x343c)) {
        setGrabItemThrow();
    }

    if (checkAnmEnd(var_r30)) {
        if (field_0x3198 != 0) {
            field_0x3198 = 0;
            swordEquip(0);
        }

        checkNextAction(0);
    } else if (var_r30->getFrame() > field_0x347c) {
        onModeFlg(4);

        if (field_0x3198 != 0) {
            field_0x3198 = 0;
            swordEquip(0);
            checkNextAction(0);
        } else {
            checkNextAction(1);
        }
    }

    return 1;
}

int daAlink_c::procGrabPutInit() {
    if (mProcID == PROC_GRAB_STAND) {
        return 0;
    }

    if (!commonProcInitNotSameProc(PROC_GRAB_PUT)) {
        return 0;
    }

    mNormalSpeed = 0.0f;
    field_0x33e8 = 0.0f;
    field_0x3478 = 23.0f;

    if (checkGrabCarryActor()) {
        field_0x3198 = 0;
        setSingleAnime(ANM_CARRY, mpHIO->mGrab.m.mCarryLiftAnm.mSpeed, 9.0f, mpHIO->mGrab.m.mCarryLiftAnm.mEndFrame, mpHIO->mGrab.m.mCarryLiftAnm.mInterpolation);
        mProcVar3.field_0x300e = 1;
        field_0x347c = 1.0f / (mpHIO->mGrab.m.mCarryLiftAnm.mEndFrame - 9.0f);
        field_0x3480 = 1.0f / (mpHIO->mGrab.m.mCarryLiftAnm.mEndFrame - field_0x3478);
    } else {
        mProcVar3.field_0x300e = 0;
        field_0x3198 = 0;
        setSingleAnimeParam(ANM_GRAB_UP, &mpHIO->mGrab.m.mThrowAnm);
        field_0x347c = 1.0f / (mpHIO->mGrab.m.mThrowAnm.mEndFrame - mpHIO->mGrab.m.mThrowAnm.mStartFrame);
    }

    if (mGrabItemAcKeep.getActor() != NULL) {
        field_0x37c8 = mGrabItemAcKeep.getActor()->current.pos;
    } else {
        field_0x37c8 = current.pos;
    }

    if (checkEndResetFlg1(ERFLG1_UNK_2000)) {
        mProcVar4.field_0x3010 = cLib_targetAngleY(&current.pos, &mForcePutPos);
    } else {
        mProcVar4.field_0x3010 = shape_angle.y;
    }

    onResetFlg0(RFLG0_GRAB_PUT_START);
    return 1;
}

int daAlink_c::procGrabPut() {
    fopAc_ac_c* temp_r29 = mGrabItemAcKeep.getActor();

    cLib_addCalcAngleS(&shape_angle.y, mProcVar4.field_0x3010, 2, 0x2000, 0x800);
    current.angle.y = shape_angle.y;

    if (temp_r29 != NULL) {
        cXyz sp8(temp_r29->current.pos.x + (field_0x33e4 * cM_ssin(shape_angle.y)), temp_r29->current.pos.y, temp_r29->current.pos.z + (field_0x33e4 * cM_scos(shape_angle.y)));
        putObjLineCheck(mObjLinChk, &sp8, temp_r29);
        field_0x37c8 = temp_r29->current.pos;
    }

    daPy_frameCtrl_c* temp_r28 = &mUnderFrameCtrl[0];

    if (checkAnmEnd(temp_r28)) {
        if (temp_r29 != NULL) {
            temp_r29->speedF = 0.0f;
        }
        freeGrabItem();
        procGrabStandInit();
    } else if (mProcVar3.field_0x300e) {
        f32 var_f31;
        if (temp_r28->getFrame() < field_0x3478) {
            var_f31 = 0.0f;
        } else {
            var_f31 = field_0x3480 * (temp_r28->getFrame() - field_0x3478);
        }
        setCarryArmAngle(1.0f - (field_0x347c * (temp_r28->getFrame() - 9.0f)), var_f31);
    } else {
        setCarryArmAngle(0.0f, field_0x347c * (temp_r28->getFrame() - mpHIO->mGrab.m.mThrowAnm.mStartFrame));
    }

    return 1;
}

int daAlink_c::procGrabWaitInit() {
    if (checkEndResetFlg2(ERFLG2_UNK_8)) {
        return procGrabReboundInit(1);
    }

    BOOL var_r30;
    if (!checkModeFlg(0x100000) && checkNoResetFlg0(FLG0_UNDERWATER)) {
        var_r30 = 1;
    } else {
        var_r30 = 0;
    }

    if (!commonProcInitNotSameProc(PROC_GRAB_WAIT)) {
        return 0;
    }

    f32 var_f31 = mpHIO->mBasic.m.mBasicInterpolation;
    if (!checkGrabAnime()) {
        setGrabUpperAnime(-1.0f);
    } else {
        setWaterInAnmRate(&mUpperFrameCtrl[2], 1.0f);
    }

    if (var_r30) {
        if (checkZoraWearAbility() ) {
            var_f31 *= 1.0f / mpHIO->mItem.mIronBoots.m.mZoraWaterAnmSpeed;
        } else {
            var_f31 *= 1.0f / mpHIO->mItem.mIronBoots.m.mWaterWalkAnmRate;
        }
    }

    if (checkAttentionLock() && (checkGrabAnimeUp() || checkGrabAnimeSide())) {
        setBlendAtnMoveAnime(mpHIO->mBasic.m.mBasicInterpolation);
    } else {
        setBlendMoveAnime(var_f31);
    }

    current.angle.y = shape_angle.y;
    mNormalSpeed = 0.0f;
    return 1;
}

int daAlink_c::procGrabWait() {
    if (mGrabItemAcKeep.getActor() == NULL) {
        if (checkGrabAnime()) {
            resetUpperAnime(UPPER_2, 3.0f);
        }
        return checkNextAction(0);
    }

    s16 temp_r29 = shape_angle.y;

    if (!checkNextAction(0)) {
        field_0x33f0 = 0.005f * (s16)(shape_angle.y - temp_r29);

        if (checkAttentionLock() && (checkGrabAnimeUp() || checkGrabAnimeSide())) {
            setBlendAtnMoveAnime(-1.0f);
        } else {
            setBlendMoveAnime(-1.0f);
        }
    } else if (checkGrabAnime()) {
        mUpperFrameCtrl[2].setRate(0.0f);
        mUpperFrameCtrl[2].setFrame(0.0f);
    }

    return 1;
}

int daAlink_c::procGrabReboundInit(int param_0) {
    commonProcInit(PROC_GRAB_REBOUND);
    setSingleAnimeParam(ANM_PICK_UP_LARGE, &mpHIO->mGrab.m.mCarryAnm);

    mNormalSpeed = 0.0f;
    field_0x3198 = param_0;

    if (param_0) {
        mUnderFrameCtrl[0].setRate(0.0f);
    }

    dComIfGp_getVibration().StartShock(1, 0xF, cXyz(0.0f, 1.0f, 0.0f));
    return 1;
}

int daAlink_c::procGrabRebound() {
    daPy_frameCtrl_c* temp_r30 = &mUnderFrameCtrl[0];

    if (mGrabItemAcKeep.getActor() == NULL) {
        resetUpperAnime(UPPER_2, -1.0f);
        return checkNextAction(0);
    }

    if (field_0x3198 != 0) {
        if (field_0x2060->getOldFrameRate() < 0.1f) {
            setWaterInAnmRate(temp_r30, mpHIO->mGrab.m.mCarryAnm.mSpeed);
            field_0x3198 = 0;
        }
    } else if (checkAnmEnd(temp_r30) || (checkInputOnR() && (temp_r30->getFrame() > mpHIO->mGrab.m.mCarryAnm.mCancelFrame))) {
        initGrabNextMode();
    }

    return 1;
}

int daAlink_c::procGrabStandInit() {
    commonProcInit(PROC_GRAB_STAND);

    if (checkUnderMove0BckNoArc(ANM_CARRY)) {
        setSingleAnime(ANM_CARRY, mpHIO->mGrab.m.mCarryLiftAnm.mSpeed, mpHIO->mGrab.m.mCarryLiftAnm.mStartFrame, (9.0f + mpHIO->mGrab.m.mCarryLiftAnm.mSpeed), 0.0f);
        field_0x3478 = mpHIO->mGrab.m.mCarryLiftAnm.mCancelFrame;
        field_0x347c = 1.0f / (9.0f - mpHIO->mGrab.m.mCarryLiftAnm.mStartFrame);
        mProcVar3.field_0x300e = 1;
        setCarryArmAngle(1.0f, 0.0f);
    } else {
        setSingleAnimeParam(ANM_GRAB_UP_START, &mpHIO->mGrab.m.mStandReverseAnm);
        field_0x3478 = mpHIO->mGrab.m.mStandReverseAnm.mCancelFrame;
        mProcVar3.field_0x300e = 0;
    }

    return 1;
}

int daAlink_c::procGrabStand() {
    daPy_frameCtrl_c* temp_r30 = &mUnderFrameCtrl[0];

    if (checkAnmEnd(temp_r30)) {
        if (mDemo.getDemoMode() == 0xD) {
            dComIfGp_evmng_cutEnd(mAlinkStaffId);
        } else {
            checkNextAction(0);
        }
    } else if (temp_r30->getFrame() < field_0x3478) {
        onModeFlg(4);

        if (!checkNextAction(1) && mProcVar3.field_0x300e != 0) {
            setCarryArmAngle(field_0x347c * (mUnderFrameCtrl[0].getFrame() - mpHIO->mGrab.m.mCarryLiftAnm.mStartFrame), 0.0f);
        }
    }

    return 1;
}

BOOL daAlink_c::checkInsectActorName(fopAc_ac_c* insectActor) {
    s16 insectName = fopAcM_GetName(insectActor);

    for (int i = 0; i < 12; i++) {
        if (insectName == l_insectNameList[i]) {
            return true;
        }
    }
    return false;
}

int daAlink_c::procInsectCatchInit() {
    if (fopAcM_checkCarryNow(field_0x27f4)) {
        return checkWaitAction();
    }

    if (mEquipItem != fpcNm_ITEM_NONE) {
        return procPreActionUnequipInit(PROC_INSECT_CATCH, field_0x27f4);
    }

    commonProcInit(PROC_INSECT_CATCH);
    mNormalSpeed = 0.0f;

    if (field_0x27f4->attention_info.position.y > field_0x3834.y - 30.0f) {
        setSingleAnimeParam(ANM_BOTTLE_SWING, &mpHIO->mItem.mBottle.m.mSwingSideAnm);
        field_0x3478 = 4.0f;
    } else {
        setSingleAnimeParam(ANM_BOTTLE_SWING_DOWN, &mpHIO->mItem.mBottle.m.mSwingDownAnm);
        field_0x3478 = 8.0f;
    }

    field_0x280c.setData(field_0x27f4);
    return 1;
}

int daAlink_c::procInsectCatch() {
    daPy_frameCtrl_c* frame_ctrl = &mUnderFrameCtrl[0];

    if (frame_ctrl->checkAnmEnd()) {
        checkNextAction(0);
    } else {
        if (frame_ctrl->checkPass(field_0x3478)) {
            fopAc_ac_c* actor = field_0x280c.getActor();
            if (actor != NULL) {
                fopAcM_setCarryNow(actor, 0);
            }
        }
    }
    return true;
}

int daAlink_c::procPickUpInit() {
    if (fopAcM_checkCarryNow(field_0x27f4)) {
        return checkWaitAction();
    }

    if (mEquipItem != fpcNm_ITEM_NONE) {
        return procPreActionUnequipInit(PROC_PICK_UP, field_0x27f4);
    }

    s16 temp_r3 = fopAcM_GetName(field_0x27f4);
    if (temp_r3 == PROC_NPC_KS && !dComIfGp_event_compulsory(this, NULL, 0xFFFF)) {
        return checkWaitAction();
    }

    commonProcInit(PROC_PICK_UP);
    mGrabItemAcKeep.setData(field_0x27f4);

    if (fopAcM_searchActorDistanceXZ2(this, field_0x27f4) > 1.0f) {
        shape_angle.y = fopAcM_searchActorAngleY(this, field_0x27f4);
    }

    current.angle.y = shape_angle.y;
    mProcVar2.field_0x300c = shape_angle.y;
    field_0x37c8 = field_0x27f4->current.pos - field_0x3510;

    mGrabItemAcKeep.getActor();
    mNormalSpeed = 0.0f;

    if (checkEquipAnime()) {
        resetUpperAnime(UPPER_2, -1.0f);
    }

    int var_r29;
    if (temp_r3 == PROC_Obj_Kantera) {
        setSingleAnimeParam(ANM_BOTTLE_SWING, &mpHIO->mItem.mBottle.m.mSwingSideAnm);
        mProcVar3.field_0x300e = 1;
        field_0x3478 = 7.0f;
        mProcVar4.field_0x3010 = 0;
        var_r29 = 0;
    } else if (temp_r3 == PROC_NPC_KS) {
        setSingleAnimeParam(ANM_BOTTLE_SWING_DOWN, &mpHIO->mItem.mBottle.m.mSwingDownAnm);
        mProcVar3.field_0x300e = 1;
        field_0x3478 = 7.0f;
        mProcVar4.field_0x3010 = 1;
        fopAcM_onSwitch(field_0x27f4, 0x3F);
        var_r29 = 0;
    } else {
        setSingleAnimeParam(ANM_PICK_UP, &mpHIO->mItem.mPickUp.m.mGrabAnm);
        mProcVar3.field_0x300e = 0;
        mProcVar4.field_0x3010 = 0;
        var_r29 = 1;
    }

    fopAcM_setCarryNow(field_0x27f4, var_r29);
    return 1;
}

int daAlink_c::procPickUp() {
    if (mProcVar4.field_0x3010 == 0 && mGrabItemAcKeep.getActor() == NULL && mItemAcKeep.getActor() == NULL) {
        return checkNextAction(0);
    }

    daPy_frameCtrl_c* temp_r30 = &mUnderFrameCtrl[0];

    if (mProcVar3.field_0x300e != 0) {
        if (checkAnmEnd(temp_r30)) {
            if (mProcVar4.field_0x3010 != 0) {
                procBottleGetInit(0);
            } else {
                checkNextAction(0);
            }
        } else if (temp_r30->checkPass(field_0x3478)) {
            onResetFlg0(RFLG0_UNK_8000);

            if (mProcVar4.field_0x3010 != 0) {
                mEquipItem = fpcNm_ITEM_KANTERA;
                setKandelaarModel();
            }
        }
    } else if (checkAnmEnd(temp_r30)) {
        checkNextAction(0);
    } else if (temp_r30->getFrame() > mpHIO->mItem.mPickUp.m.mGrabAnm.mCancelFrame) {
        onModeFlg(4);
        checkNextAction(1);
    } else if (temp_r30->checkPass(9.0f)) {
        mEquipItem = 0x102;
        mItemAcKeep = mGrabItemAcKeep;
        mGrabItemAcKeep.clearData();
        field_0x2f94 = 3;
        onResetFlg0(RFLG0_UNK_8000);
    }

    return 1;
}

int daAlink_c::procPickPutInit(int param_0) {
    if (param_0 != 0 && mEquipItem != fpcNm_ITEM_NONE) {
        return procPreActionUnequipInit(PROC_PICK_PUT, NULL);
    }

    commonProcInit(PROC_PICK_PUT);
    mNormalSpeed = 0.0f;
    setSingleAnimeParam(ANM_PICK_UP, &mpHIO->mItem.mPickUp.m.mPlaceAnm);

    if (mItemAcKeep.getActor() != NULL) {
        field_0x37c8 = mItemAcKeep.getActor()->current.pos;
    } else {
        field_0x37c8 = current.pos;
    }

    field_0x3198 = 1;
    mProcVar2.field_0x300c = param_0;

    if (mProcVar2.field_0x300c != 0) {
        mEquipItem = fpcNm_ITEM_POKE_BOMB;
    } else {
        onResetFlg0(RFLG0_GRAB_PUT_START);
    }

    return 1;
}

int daAlink_c::procPickPut() {
    fopAc_ac_c* temp_r3 = mItemAcKeep.getActor();
    if (temp_r3 == NULL && mProcVar2.field_0x300c == 0 && field_0x3198 != 0) {
        return checkNextAction(0);
    }

    if (temp_r3 != NULL) {
        cXyz sp8(temp_r3->current.pos.x, temp_r3->current.pos.y, temp_r3->current.pos.z);
        putObjLineCheck(*fopAcM_lc_c::getLineCheck(), &sp8, temp_r3);
        field_0x37c8 = temp_r3->current.pos;
    }

    daPy_frameCtrl_c* temp_r28 = &mUnderFrameCtrl[0];

    if (checkAnmEnd(temp_r28)) {
        checkNextAction(0);
    } else if (temp_r28->getFrame() < mpHIO->mItem.mPickUp.m.mPlaceAnm.mCancelFrame) {
        onModeFlg(4);
        checkNextAction(1);
    } else if (temp_r28->checkPass(9.0f)) {
        if (mProcVar2.field_0x300c != 0) {
            if (checkReadyItem() && dBomb_c::createInsectBombPlayer(&current.pos)) {
                dComIfGp_addSelectItemNum(mSelectItemId, -1);
                field_0x2fcf++;
            }
            mEquipItem = fpcNm_ITEM_NONE;
        } else {
            field_0x3198 = 0;
            temp_r3->speedF = 0.0f;
            deleteEquipItem(0, 0);
        }
    }

    return 1;
}

BOOL daAlink_c::checkSetChainPullAnime(s16 param_0) {
    return checkInputOnR() && checkFmChainGrabAnime() && (field_0x3400 != 0.0f || field_0x3404 != 0.0f) && (checkBossRoom() || abs((s16)(field_0x2fe2 - param_0)) >= 0x3800);
}

s16 daAlink_c::getChainStickAngleY(s16 param_0) const {
    s16 angle = field_0x2fe2 - param_0;

    if (angle >= 0x3800 && angle < 0x7F80) {
        return param_0 + 0x7F80;
    } else if (angle <= -0x3800 && angle > -0x7F80) {
        return param_0 - 0x7F80;
    }

    return field_0x2fe2;
}

u8 daAlink_c::checkChainEmphasys() {
    fopAc_ac_c* temp_r3 = field_0x2844.getActor();
    if (temp_r3 != NULL && fopAcM_GetName(temp_r3) == PROC_Obj_SwChain && !((daObjSwChain_c*)temp_r3)->checkDispEmphasis()) {
        return 0;
    }

    return 2;
}

BOOL daAlink_c::searchFmChainPos() {
    if (field_0x2844.getActor() == NULL) {
        field_0x2fa3 = 0;
        return 0;
    }

    cXyz* var_r30 = field_0x354c;

    for (int i = 0; i < 4; i++, var_r30++) {
        if (current.pos.abs2XZ(*var_r30) < 14400.0f && fabsf(current.pos.y - var_r30->y) < 100.0f) {
            return i + 1;
        }
    }

    return 0;
}

BOOL daAlink_c::setFmChainPosFromOut(fopAc_ac_c* param_0, cXyz* param_1, int param_2) {
    field_0x354c[param_2] = *param_1;

    BOOL var_r29 = field_0x2fa3 - 1 == param_2 &&
                   (checkFmChainGrabAnime() ||
                   (mProcID == PROC_CHAIN_UP && mUnderFrameCtrl[0].getFrame() >= 8.0f) ||
                   (mProcID == PROC_WOLF_CHAIN_UP && mUnderFrameCtrl[0].getFrame() >= 7.0f));

    if (field_0x2fa3 == 0 || var_r29) {
        field_0x2844.setData(param_0);
    }

    return var_r29;
}

bool daAlink_c::checkChainBlockPushPull() {
    if (checkFmChainGrabAnime()) {
        fopAc_ac_c* actor = field_0x2844.getActor();
        if (actor != NULL) {
            s16 name = fopAcM_GetName(actor);
            if (name == PROC_Obj_ChainBlock || name == PROC_Obj_SwChain ||
                name == PROC_Obj_ChainWall)
            {
                return true;
            }
        }
    }
    return false;
}

int daAlink_c::procFmChainUpInit() {
    if (mEquipItem != fpcNm_ITEM_NONE) {
        return procPreActionUnequipInit(PROC_CHAIN_UP, NULL);
    }

    field_0x2fa3 = searchFmChainPos();

    if (field_0x2fa3 == 0) {
        return procWaitInit();
    }

    commonProcInit(PROC_CHAIN_UP);
    setSingleAnimeParam(ANM_CHAIN_PICK_UP, &mpHIO->mItem.mFmChain.m.mGripAnm);
    mNormalSpeed = 0.0f;
    return 1;
}

int daAlink_c::procFmChainUp() {
    daPy_frameCtrl_c* temp_r29 = &mUnderFrameCtrl[0];

    setGrabStatus(0x96, 2);
    setShapeAngleToAtnActor(0);

    if (checkAnmEnd(temp_r29) || (checkInputOnR() && temp_r29->getFrame() > mpHIO->mItem.mFmChain.m.mGripAnm.mCancelFrame)) {
        u8 temp_r27 = field_0x2fa3;
        fopAc_ac_c* temp_r28 = field_0x2844.getActor();

        procWaitInit();
        setUpperAnimeBaseSpeed(getMainBckData(ANM_CHAIN_PULL)->m_upperID, 0.0f, 3.0f);
        field_0x2f96 = 3;
        field_0x2f97 = 9;

        if (temp_r28 != NULL) {
            field_0x2fa3 = temp_r27;
            field_0x2844.setData(temp_r28);
        }
    } else if (temp_r29->getFrame() >= 9.0f) {
        field_0x2f92 = 3;
        field_0x2f93 = 9;
    } else {
        field_0x2f92 = 0xFE;
        field_0x2f93 = 0xFE;
    }

    return 1;
}

int daAlink_c::procFmChainStrongPullInit() {
    if (!commonProcInitNotSameProc(PROC_CHAIN_STRONG_PULL)) {
        return 0;
    }

    setSingleAnimeBase(ANM_CHAIN_PULL_FYRUS);
    mNowAnmPackUpper[2].setRatio(0.0f);
    mNormalSpeed = 0.0f;
    voiceStart(Z2SE_AL_V_MAGNET_CAUGHT);
    return 1;
}

int daAlink_c::procFmChainStrongPull() {
    setGrabStatus(0x96, 2);

    if (checkAnmEnd(&mUnderFrameCtrl[0])) {
        if (mDemo.getDemoMode() == 0x43) {
            dComIfGp_evmng_cutEnd(mAlinkStaffId);
        } else {
            mNowAnmPackUpper[2].setRatio(1.0f);
            checkNextAction(0);
        }
    }

    return 1;
}

void daAlink_c::setWallGrabStatus(u8 param_0, u8 param_1) {
    setRStatus(param_0, param_1);
}

int daAlink_c::getWallGrabStatus() {
    return dComIfGp_getRStatus();
}

BOOL daAlink_c::wallGrabTrigger() {
    return itemTriggerCheck(BTN_R);
}

BOOL daAlink_c::wallGrabButton() {
    return itemButtonCheck(BTN_R);
}

int daAlink_c::setPushPullKeepData(dBgW_Base::PushPullLabel i_label, int param_1) {
    if (i_label != dBgW::PPLABEL_NONE) {
        if (checkPowerGloveGet()) {
            cLib_onBit<dBgW::PushPullLabel>(i_label, dBgW::PPLABEL_HEAVY);
        }

        if (field_0x3198) {
            cLib_onBit<dBgW::PushPullLabel>(i_label, dBgW::PPLABEL_4);
        }
    }

    fopAc_ac_c* pp_actor = dComIfG_Bgsp().PushPullCallBack(mPolyInfo1, this, shape_angle.y, i_label);
    if (pp_actor == NULL) {
        return 0;
    }

    cXyz unused;
    if (i_label != dBgW::PPLABEL_NONE || param_1) {
        if (pp_actor->shape_angle.y != mProcVar2.field_0x300c) {
            s16 var_r28 = pp_actor->shape_angle.y - mProcVar2.field_0x300c;
            mProcVar3.field_0x300e += var_r28;

            current.pos.x = field_0x37c8.x + field_0x347c * cM_ssin(mProcVar3.field_0x300e);
            current.pos.z = field_0x37c8.z + field_0x347c * cM_scos(mProcVar3.field_0x300e);
            shape_angle.y += var_r28;
            current.angle.y += var_r28;
        }

        current.pos.x += pp_actor->current.pos.x - field_0x37c8.x;
        current.pos.z += pp_actor->current.pos.z - field_0x37c8.z;
    }

    field_0x37c8 = pp_actor->current.pos;
    mProcVar2.field_0x300c = pp_actor->shape_angle.y;
    return 1;
}

BOOL daAlink_c::checkPushPullTurnBlock() {
    s16 name = getMoveBGActorName(mPolyInfo1, 1);
    return name == PROC_Obj_SwTurn || name == PROC_Obj_Lv6SwTurn || name == PROC_Obj_PushDoor || name == PROC_PushDoor || name == PROC_Obj_Ytaihou;
}

BOOL daAlink_c::checkPullBehindWall() {
    cXyz sp14;
    cXyz sp8;

    f32 temp_f31 = cM_ssin(shape_angle.y);
    f32 temp_f30 = cM_scos(shape_angle.y);

    sp14.x = current.pos.x;
    sp14.y = current.pos.y + l_autoUpHeight;
    sp14.z = current.pos.z;

    sp8.x = sp14.x - (75.0f * temp_f31);
    sp8.y = sp14.y;
    sp8.z = sp14.z - (75.0f * temp_f30);
    if (commonLineCheck(&sp14, &sp8)) {
        return 1;
    }

    sp14.y = 129.99f + current.pos.y;
    sp8.y = sp14.y;
    if (commonLineCheck(&sp14, &sp8)) {
        return 1;
    }

    sp8.x += 34.0f * temp_f31;
    sp8.z += 34.0f * temp_f30;
    mLinkGndChk.SetPos(&sp8);
    if (dComIfG_Bgsp().GroundCross(&mLinkGndChk) - current.pos.y < l_autoDownHeight) {
        return 1;
    }

    return 0;
}

void daAlink_c::offGoatStopGame() {
    if (mSpecialMode == SMODE_GOAT_STOP) {
        mSpecialMode = 0;
    }
}

BOOL daAlink_c::checkGoatCatchActor(fopAc_ac_c* param_0) {
    s16 name = fopAcM_GetName(param_0);
    return name == PROC_E_GOB || (name == PROC_OBJ_GRA && checkSpecialNpc(param_0)) ||
           name == PROC_COW;
}

f32 daAlink_c::getGoatCatchDistance2() {
    if (fopAcM_GetName(field_0x27f4) == PROC_B_MGN) {
        return 1000000.0f;
    }

    return 90000.0f;
}

int daAlink_c::endPushPull() {
    return checkWaitAction();
}

f32 daAlink_c::getPushPullAnimeSpeed() {
    f32 var_f31;
    if ((getMoveBGActorName(mPolyInfo1, 0) == PROC_Obj_Movebox && ((daObjMovebox::Act_c*)dComIfG_Bgsp().GetActorPointer(mPolyInfo1))->getType() != 0) ||
        getMoveBGActorName(mPolyInfo1, 0) == PROC_Obj_Bemos)
    {
        if (checkWolf()) {
            var_f31 = mpHIO->mWolf.mWlPush.m.mHeavyPushAnmSpeed;
        } else {
            var_f31 = mpHIO->mPushpull.m.mPushASpeedHeavy;
        }
    } else if (checkWolf()) {
        var_f31 = mpHIO->mWolf.mWlPush.m.mPushAnmSpeed;
    } else {
        var_f31 = mpHIO->mPushpull.m.mPushASpeed;
    }

    return var_f31;
}

int daAlink_c::procCoPushPullWaitInit(int param_0) {
    if (mEquipItem != fpcNm_ITEM_NONE) {
        return procPreActionUnequipInit(PROC_PUSH_PULL_WAIT, NULL);
    }

    commonProcInit(PROC_PUSH_PULL_WAIT);
    mProcVar4.field_0x3010 = 0;
    field_0x3198 = param_0;
    mNormalSpeed = 0.0f;

    f32 var_f31;
    if (checkWolf()) {
        setSingleAnimeWolfBaseSpeed(WANM_WAIT_PP, mpHIO->mWolf.mWlPush.m.mIdleAnmSpeed, mpHIO->mWolf.mWlPush.m.mIdleInterp);
        var_f31 = 50.0f;
    } else {
        if (checkEquipAnime()) {
            resetUpperAnime(UPPER_2, -1.0f);
        }

        setSingleAnimeBaseSpeed(ANM_WAIT_PUSH_PULL, mpHIO->mPushpull.m.mStandbyASpeed, mpHIO->mPushpull.m.mStandbyInterpolation);
        var_f31 = 50.0f;
    }

    if (param_0 != 0) {
        mProcVar2.field_0x300c = field_0x306e + 0x8000;

        field_0x37c8.x = field_0x34ec.x + (var_f31 * cM_ssin(field_0x306e));
        field_0x37c8.y = current.pos.y;
        field_0x37c8.z = field_0x34ec.z + (var_f31 * cM_scos(field_0x306e));
    }

    dComIfGp_setPlayerStatus0(0, 0x4000000);
    return 1;
}

int daAlink_c::procCoPushPullWait() {
    if (mProcVar4.field_0x3010 == 0) {
        setWallGrabStatus(0x15, 2);
    }

    setFrontWallType();

    if (mProcVar4.field_0x3010 != 0 || (wallGrabButton() && checkResetFlg0(RFLG0_UNK_8))) {
        if (field_0x3198 != 0) {
            s16 temp_r28 = cLib_addCalcAngleS(&shape_angle.y, mProcVar2.field_0x300c, 3, 0x800, 0x100);
            current.angle.y = shape_angle.y;

            f32 temp_f31 = cLib_addCalc(&current.pos.x, field_0x37c8.x, 0.5f, 10.0f, 1.0f);
            temp_f31 += cLib_addCalc(&current.pos.z, field_0x37c8.z, 0.5f, 10.0f, 1.0f);
    
            if (temp_r28 == 0 && temp_f31 < 5.0f && checkNoUpperAnime()) {
                current.pos.x = field_0x37c8.x;
                current.pos.z = field_0x37c8.z;
                field_0x3198 = 0;
            }
        }

        if (field_0x3198 == 0) {
            if (mProcVar4.field_0x3010 != 0) {
                procCoPushMoveInit(0, 1);
            } else if (checkInputOnR()) {
                int temp_r3 = getDirectionFromShapeAngle();
                if (temp_r3 == DIR_FORWARD) {
                    procCoPushMoveInit(0, getMoveBGActorName(mPolyInfo1, 1) == PROC_Obj_IceBlock);
                } else if (!checkWolf() && temp_r3 == DIR_BACKWARD) {
                    procPullMoveInit(0);
                }
            }
        }
    } else {
        endPushPull();
    }

    return 1;
}

int daAlink_c::procCoPushMoveInit(int param_0, int param_1) {
    if (!setPushPullKeepData(dBgW_Base::PPLABEL_NONE, param_0)) {
        return 0;
    }

    commonProcInit(PROC_PUSH_MOVE);
    mProcVar0.field_0x3008 = checkPushPullTurnBlock();
    f32 var_f31 = getPushPullAnimeSpeed();

    if (checkWolf()) {
        if (mProcVar0.field_0x3008 != 0) {
            setSingleAnimeWolfBase(WANM_PUSH_START);
            var_f31 = 1.0f;
        } else {
            setSingleAnimeWolf(WANM_PUSH_LIGHT, var_f31, 0.0f, 24, mpHIO->mWolf.mWlPush.m.mPushInterp);
        }

        field_0x3478 = 17.0f;
    } else {
        if (mProcVar0.field_0x3008 != 0) {
            setSingleAnimeBase(ANM_PUSH_START);
            var_f31 = 1.0f;
        } else {
            setSingleAnimeBaseSpeed(ANM_PUSH_LIGHT, var_f31, mpHIO->mPushpull.m.mPushInterpolation);
        }

        field_0x3478 = 15.0f;
    }

    mUnderFrameCtrl[0].setRate(var_f31);
    field_0x3198 = 1;
    mProcVar4.field_0x3010 = param_1;
    mProcVar5.field_0x3012 = mProcVar0.field_0x3008;

    fopAc_ac_c* temp_r3 = dComIfG_Bgsp().GetActorPointer(mPolyInfo1);
    mProcVar3.field_0x300e = fopAcM_searchActorAngleY(temp_r3, this);
    field_0x347c = temp_r3->current.pos.absXZ(current.pos);
    dComIfGp_setPlayerStatus0(0, 0x04000000);
    return 1;
}

int daAlink_c::procCoPushMove() {
    daPy_frameCtrl_c* temp_r29 = &mUnderFrameCtrl[0];
    int var_r28 = 0x160;

    if (mProcVar4.field_0x3010 != 0) {
        if (temp_r29->checkPass(0.0f)) {
            return endPushPull();
        }

        if (temp_r29->checkPass(field_0x3478)) {
            if (!checkWolf()) {
                voiceStart(Z2SE_AL_V_PUSH_ROCK);
            }

            setPushPullKeepData(dBgW::PPLABEL_PUSH, 0);
        }
    } else {
        setWallGrabStatus(0x15, 2);

        if (temp_r29->checkPass(0.0f) || field_0x3198 != 0 || mProcVar5.field_0x3012 != 0) {
            if (!checkNoResetFlg0(FLG0_PUSH_PULL_KEEP)) {
                if (!wallGrabButton()) {
                    return endPushPull();
                }

                if (checkInputOnR()) {
                    int temp_r3 = getDirectionFromShapeAngle();
                    if (temp_r3 == DIR_FORWARD) {
                        setPushPullKeepData(dBgW::PPLABEL_PUSH, 0);
                        if (mProcVar0.field_0x3008 != 0) {
                            if (mProcVar5.field_0x3012 == 0) {
                                mProcVar5.field_0x3012 = 1;

                                if (checkWolf()) {
                                    setSingleAnimeWolf(WANM_PUSH_START, 1.0f, 12.0f, -1, 3.0f);
                                } else {
                                    setSingleAnime(ANM_PUSH_START, 1.0f, 12.0f, -1, 3.0f);
                                }

                                temp_r29->setRate(1.0f);
                            } else if (checkAnmEnd(temp_r29)) {
                                if (checkWolf()) {
                                    setSingleAnimeWolfBase(WANM_PUSH);
                                } else {
                                    setSingleAnimeBase(ANM_PUSH);
                                }

                                temp_r29->setRate(1.0f);
                            }
                        }
                    } else if (!checkWolf() && temp_r3 == DIR_BACKWARD) {
                        var_r28 = 0x39;
                    } else {
                        var_r28 = 0x143;
                    }
                } else {
                    setPushPullKeepData(dBgW::PPLABEL_NONE, 1);
                    var_r28 = 0x143;
                }
            } else {
                if (field_0x3198 != 0 && !checkWolf()) {
                    voiceStart(Z2SE_AL_V_PUSH_ROCK);
                }

                field_0x3198 = 0;
                if (mProcVar5.field_0x3012 != 0) {
                    mProcVar5.field_0x3012 = 0;
                    if (checkWolf()) {
                        setSingleAnimeWolfBase(WANM_PUSH);
                    } else {
                        setSingleAnimeBase(ANM_PUSH);
                    }
                }
                setPushPullKeepData(dBgW::PPLABEL_PUSH, 0);
            }
        } else {
            setPushPullKeepData(dBgW::PPLABEL_PUSH, 0);
        }
    }

    setFrontWallType();

    if (!checkResetFlg0(RFLG0_UNK_8)) {
        endPushPull();
    } else if (var_r28 == 0x39) {
        procPullMoveInit(1);
    } else if (var_r28 == 0x143) {
        procCoPushPullWaitInit(0);
    }

    return 1;
}

int daAlink_c::procPullMoveInit(int param_0) {
    if (!setPushPullKeepData(dBgW::PPLABEL_NONE, param_0)) {
        return 0;
    }

    commonProcInit(PROC_PULL_MOVE);

    mProcVar0.field_0x3008 = checkPushPullTurnBlock();
    mProcVar5.field_0x3012 = mProcVar0.field_0x3008;

    f32 var_f31;
    if (mProcVar0.field_0x3008 != 0) {
        var_f31 = 1.0f;
        setSingleAnimeBase(ANM_PULL_START);
    } else {
        var_f31 = getPushPullAnimeSpeed();
        setSingleAnimeBaseSpeed(ANM_PULL_LIGHT, var_f31, mpHIO->mPushpull.m.mPullInterpolation);
    }

    mUnderFrameCtrl[0].setRate(var_f31);
    field_0x3198 = 1;

    fopAc_ac_c* temp_r3 = dComIfG_Bgsp().GetActorPointer(mPolyInfo1);
    mProcVar3.field_0x300e = fopAcM_searchActorAngleY(temp_r3, this);
    field_0x347c = temp_r3->current.pos.absXZ(current.pos);
    dComIfGp_setPlayerStatus0(0, 0x4000000);
    return 1;
}

int daAlink_c::procPullMove() {
    setWallGrabStatus(0x15, 2);

    int temp_r28 = getDirectionFromShapeAngle();
    int var_r30 = 0x160;

    daPy_frameCtrl_c* temp_r29 = &mUnderFrameCtrl[0];

    if (temp_r29->checkPass(0.0f) || field_0x3198 != 0 || mProcVar5.field_0x3012 != 0) {
        if (!checkNoResetFlg0(FLG0_PUSH_PULL_KEEP)) {
            if (!wallGrabButton()) {
                return endPushPull();
            }

            if (checkInputOnR()) {
                if (temp_r28 == DIR_BACKWARD) {
                    if (checkPullBehindWall()) {
                        setPushPullKeepData(dBgW::PPLABEL_NONE, 1);
                        field_0x3198 = 1;
                    } else {
                        setPushPullKeepData(dBgW::PPLABEL_PULL, 0);

                        if (mProcVar0.field_0x3008 != 0) {
                            if (mProcVar5.field_0x3012 == 0) {
                                mProcVar5.field_0x3012 = 1;
                                setSingleAnime(ANM_PULL_START, 1.0f, 12.0f, -1, 3.0f);
                                temp_r29->setRate(1.0f);
                            } else if (checkAnmEnd(temp_r29)) {
                                setSingleAnimeBase(ANM_PULL);
                                temp_r29->setRate(1.0f);
                            }
                        }
                    }
                } else if (temp_r28 == DIR_FORWARD) {
                    var_r30 = 0x144;
                } else {
                    var_r30 = 0x143;
                }
            } else {
                setPushPullKeepData(dBgW::PPLABEL_NONE, 1);
                var_r30 = 0x143;
            }
        } else {
            if (field_0x3198 != 0) {
                voiceStart(Z2SE_AL_V_PUSH_ROCK);
                field_0x3198 = 0;
            }

            if (mProcVar5.field_0x3012 != 0) {
                mProcVar5.field_0x3012 = 0;
                setSingleAnimeBase((daAlink_c::daAlink_ANM) 0x3C);
            }

            setPushPullKeepData(dBgW::PPLABEL_PULL, 0);
        }
    } else {
        setPushPullKeepData(dBgW::PPLABEL_PULL, 0);
    }

    setFrontWallType();

    if (!checkResetFlg0(RFLG0_UNK_8)) {
        endPushPull();
    } else if (var_r30 == 0x143) {
        procCoPushPullWaitInit(0);
    } else if (var_r30 == 0x144) {
        procCoPushMoveInit(1, 0);
    }

    return 1;
}

static fopAc_ac_c* daAlink_searchGoat(fopAc_ac_c* i_actor, void*) {
    if (fopAcM_GetName(i_actor) == PROC_COW) {
        return i_actor;
    }

    return NULL;
}

void daAlink_c::cancelGoronThrowEvent() {
    if (mProcID == PROC_GOAT_CATCH && mProcVar5.field_0x3012 == 0) {
        mProcVar5.field_0x3012 = 1;
        dComIfGp_event_reset();
    }
}

void daAlink_c::setGoatStopGameFail(fopAc_ac_c* param_0) {
    s16 temp_r3 = fopAcM_GetName(param_0);
    if (temp_r3 == PROC_OBJ_GRA) {
        ((daObj_GrA_c*)param_0)->setCrazyDash();
    } else if (temp_r3 == PROC_COW) {
        ((daCow_c*)param_0)->setCrazyDash();
    } else {
        ((fopEn_enemy_c*)param_0)->setThrowModeDash();
    }

    if (checkMagneBootsOn()) {
        cancelMagneBootsOn();
        resetSpecialEvent();
        checkAutoJumpAction();
        return;
    }

    current.angle.y = shape_angle.y + 0x8000;

    if (checkGoatStopGame()) {
        onNoResetFlg0(FLG0_UNK_200000);
    } else {
        if (!checkBootsOrArmorHeavy() && temp_r3 == PROC_OBJ_GRA) {
            setDamagePointNormal(2);
        } else if (temp_r3 != PROC_COW) {
            setDamagePointNormal(1);
        }

        resetSpecialEvent();
    }

    procCoLargeDamageInit(-1, 1, 0, 0, NULL, 0);
}

int daAlink_c::procGoatMoveInit() {
    if (!commonProcInitNotSameProc(PROC_GOAT_MOVE)) {
        return 0;
    }

    if (checkZeroSpeedF()) {
        onModeFlg(1);
    }

    mNormalSpeed = 0.0f;
    field_0x37c8 = current.pos;
    field_0x2f98 = 3;
    current.angle.y = shape_angle.y + -0x4000;
    field_0x594 = mpHIO->mAtnMove.m.mMaxSpeed;

    setSingleAnimeBase(ANM_ATN_COW);
    mProcVar2.field_0x300c = 0;

    fopAc_ac_c* goat_p = (fopAc_ac_c*)fopAcIt_Judge((fopAcIt_JudgeFunc)daAlink_searchGoat, NULL);
    if (goat_p != NULL) {
        field_0x280c.setData(goat_p);
    }

    mProcVar3.field_0x300e = 0;
    deleteEquipItem(FALSE, FALSE);
    return 1;
}

int daAlink_c::procGoatMove() {
    if (mProcVar2.field_0x300c == 0) {
        if (checkAnmEnd(&mUnderFrameCtrl[0])) {
            setSingleAnimeBase(ANM_ATN_RIGHT);
            mProcVar2.field_0x300c = 1;
        }
        return 1;
    }

    fopAc_ac_c* temp_r3 = field_0x280c.getActor();

    if (temp_r3 == NULL) {
        offGoatStopGame();
        return procWaitInit();
    }

    cXyz sp14 = temp_r3->current.pos - current.pos;
    mDoMtx_stack_c::YrotS(-shape_angle.y);
    mDoMtx_stack_c::multVecSR(&sp14, &sp14);

    if (sp14.z < 700.0f) {
        if (sp14.z > 0.0f) {
            if (fabsf(sp14.x) < 70.0f) {
                if (doTrigger() || doButton()) {
                    mProcVar3.field_0x300e = 1;
                    ((daCow_c*)temp_r3)->setCrazyBeforeCatch();
                }

                if (sp14.z < 600.0f) {
                    setDoStatusEmphasys(0x15);
                }
            }
        } else if (sp14.z < -100.0f) {
            offGoatStopGame();
            return procWaitInit();
        }
    }

    if (!doButton()) {
        mProcVar3.field_0x300e = 0;
    }

    cXyz sp8 = current.pos - field_0x37c8;
    mDoMtx_stack_c::YrotS(-shape_angle.y);
    mDoMtx_stack_c::multVecSR(&sp8, &sp8);

    if (dComIfGp_getDoStatus() == 0x15 && mProcVar3.field_0x300e != 0) {
        if (sp14.z < 250.0f) {
            return procGoatCatchInit(temp_r3, sp8.x);
        }
    } else if (field_0x850[0].ChkCoHit() && field_0x850[0].GetCoHitAc() == temp_r3 && sp14.z > -50.0f) {
        setGoatStopGameFail(temp_r3);
        return 1;
    }

    f32 var_f31;
    int temp_r27 = abs(field_0x2fe2);
    if (checkInputOnR() && temp_r27 > 0x800 && temp_r27 < 0x7800 && (fabsf(sp8.x) < 250.0f || (sp8.x > 0.0f && field_0x2fe2 < 0) || (sp8.x < 0.0f && field_0x2fe2 > 0))) {
        if (abs((s16)(field_0x2fe2 - current.angle.y)) > 0x4000) {
            current.angle.y += 0x8000;
            mNormalSpeed *= -1.0f;

            if (field_0x2f98 == 3) {
                field_0x2f98 = 2;
            } else {
                field_0x2f98 = 3;
            }
        }

        if (field_0x33a8 > (0.3f * (1.0f - fabsf(mNormalSpeed / field_0x594)))) {
            var_f31 = mpHIO->mAtnMove.m.mAcceleration * field_0x33a8;
        } else {
            var_f31 = 0.0f;
        }
    } else {
        var_f31 = 0.0f;
    }

    setNormalSpeedF(var_f31, mpHIO->mAtnMove.m.mDeceleration);

    if (checkZeroSpeedF()) {
        onModeFlg(1);
        field_0x2f98 = 3;
        current.angle.y = shape_angle.y - 0x4000;

        if (!checkUnderMove0BckNoArc(ANM_ATN_RIGHT)) {
            setSingleAnimeBase(ANM_ATN_RIGHT);
        }
    } else {
        offModeFlg(1);

        if (!checkUnderMove0BckNoArc(ANM_COW_MOVE_LEFT_RIGHT)) {
            setSingleAnimeBase(ANM_COW_MOVE_LEFT_RIGHT);
        }

        daPy_frameCtrl_c* temp_r28 = &mUnderFrameCtrl[0];
        f32 var_f30;
        if (field_0x2f98 == 3) {
            var_f30 = -1.0f;
            temp_r28->setLoop(temp_r28->getEnd());
        } else {
            var_f30 = 1.0f;
            temp_r28->setLoop(temp_r28->getStart());
        }
    
        temp_r28->setRate(var_f30 * (mNormalSpeed / field_0x594));
        initBasAnime();
    }

    return 1;
}

int daAlink_c::procGoatCatchInit(fopAc_ac_c* param_0, f32 param_1) {
    s16 temp_r28 = cLib_targetAngleY(&current.pos, &param_0->current.pos);

    if (!checkGoatStopGame()) {
        cXyz sp18(current.pos.x, 100.0f + current.pos.y, current.pos.z);

        f32 var_f31 = 300.0f;
        if (param_0->speedF > 0.1f) {
            var_f31 += 300.0f;
        }

        cXyz spC(sp18.x - (var_f31 * cM_ssin(temp_r28)), sp18.y, sp18.z - (var_f31 * cM_scos(temp_r28)));
        if (fopAcM_lc_c::lineCheck(&sp18, &spC, param_0)) {
            setDoStatus(0);
            return 0;
        }
    
        deleteEquipItem(1, 0);

        if (!dComIfGp_event_compulsory(this, NULL, 0xFFEF)) {
            return 0;
        }
    
        mDemo.setSpecialDemoType();
        param_0->actor_status |= 0x800;
    }

    commonProcInit(PROC_GOAT_CATCH);
    field_0x280c.setData(param_0);
    mProcVar4.field_0x3010 = 0;

    if (!checkGoatStopGame()) {
        shape_angle.y = temp_r28;

        if (param_0->speedF < 0.1f) {
            mNormalSpeed = 0.1f;
            mProcVar4.field_0x3010 = 1;
        } else {
            mNormalSpeed = 30.0f;
        }
    } else {
        mNormalSpeed = 30.0f;
    }

    if (checkBootsOrArmorHeavy()) {
        mNormalSpeed *= 0.75f;
    }

    if (mProcVar4.field_0x3010 == 0) {
        voiceStart(Z2SE_AL_V_PUSH_ROCK);
        seStartOnlyReverb(Z2SE_AL_BODYATTACK);
    }

    current.angle.y = shape_angle.y + 0x8000;
    field_0x2f98 = 4;

    setSingleAnimeBase(ANM_COW_CATCH_START);

    s16 temp_r3 = fopAcM_GetName(param_0);
    if (temp_r3 == PROC_OBJ_GRA) {
        ((daObj_GrA_c*)param_0)->setCrazyCatch();
    } else if (temp_r3 == PROC_COW) {
        ((daCow_c*)param_0)->setCrazyCatch();
    } else {
        ((fopEn_enemy_c*)param_0)->setThrowModeCatch();
    }

    field_0x37c8 = current.pos;

    mProcVar0.field_0x3008 = 95;
    mProcVar1.field_0x300a = 0;
    mProcVar2.field_0x300c = 0;

    field_0x3198 = -7;
    setFootEffectProcType(1);
    field_0x2f9d = 0x60;
    field_0x3588.set(-l_halfAtnWaitBaseAnime.x, l_halfAtnWaitBaseAnime.y, -l_halfAtnWaitBaseAnime.z);

    if (param_1 >= 0.0f) {
        mProcVar3.field_0x300e = 3;
    } else {
        mProcVar3.field_0x300e = 2;
    }

    mProcVar5.field_0x3012 = 0;
    field_0x3478 = -1.0f;
    return 1;
}

int daAlink_c::procGoatCatch() {
    daPy_frameCtrl_c* temp_r28 = &mUnderFrameCtrl[0];
    fopAc_ac_c* temp_r3 = field_0x280c.getActor();

    s16 temp_r27 = fopAcM_GetName(temp_r3);
    s16 sp8;
    if (mProcVar2.field_0x300c == 0 || (mProcVar1.field_0x300a == 0 && temp_r28->getFrame() < 29.0f)) {
        sp8 = field_0x2ff0;
    } else {
        sp8 = 0;
    }

    cLib_addCalcAngleS(&shape_angle.x, sp8, 5, 0x2000, 0x400);

    if (temp_r3 == NULL) {
        if (!checkGoatStopGame()) {
            cancelGoronThrowEvent();
            endDemoMode();
        } else {
            offGoatStopGame();
        }

        return procWaitInit();
    }

    if (!mLinkAcch.ChkGroundHit() && mProcVar5.field_0x3012 == 0 && (!checkMagneBootsOn() || !checkNoResetFlg0(FLG0_UNK_2000))) {
        setGoatStopGameFail(temp_r3);
        return 1;
    }

    if (mProcVar2.field_0x300c != 0) {
        if (mProcVar1.field_0x300a == 0) {
            field_0x2f99 = 5;
            onEndResetFlg0(ERFLG0_UNK_8000000);

            if (temp_r28->getFrame() >= 81.0f) {
                field_0x2f92 = 1;
                field_0x2f93 = 6;
            } else if (temp_r28->getFrame() >= 39.0f) {
                onModeFlg(0x100);
            } else if (temp_r28->getFrame() >= 32.0f) {
                field_0x2f92 = 0xFE;
                field_0x2f93 = 0xFE;
                field_0x3478 = 1.0f;
            }

            if (checkAnmEnd(temp_r28)) {
                if (mProcVar5.field_0x3012 == 0) {
                    setOldRootQuaternion(0, -0x8000, 0);
                    shape_angle.y += 0x8000;
                    current.angle.y = shape_angle.y;
                    field_0x2fe4 = shape_angle.y;
                }

                if (temp_r27 == PROC_OBJ_GRA) {
                    field_0x2f99 = 0x30;
                    setSingleAnimeBaseSpeed(ANM_WAIT_B, mpHIO->mMove.m.mWaitAnmSpeed, 3.0f);
                    offModeFlg(0x8000);
                    mProcVar1.field_0x300a = 0x1E;
                    return 1;
                }

                if (!checkGoatStopGame()) {
                    cancelGoronThrowEvent();
                    endDemoMode();
                } else {
                    offGoatStopGame();
                    if (dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[565])) {
                        dComIfGs_onEventBit(dSv_event_flag_c::saveBitLabels[587]);
                    } else {
                        dComIfGs_onEventBit(dSv_event_flag_c::saveBitLabels[178]);
                    }
                }

                return procWaitInit();
            }
        } else {
            mProcVar1.field_0x300a--;
            if (mProcVar1.field_0x300a == 0) {
                onEndResetFlg0(ERFLG0_UNK_8000000);
                cancelGoronThrowEvent();
                endDemoMode();
                return procWaitInit();
            }
        }

        return 1;
    }

    if (!doButton() || (!mLinkAcch.ChkGroundHit() && (!checkMagneBootsOn() || !checkNoResetFlg0(FLG0_UNK_2000))) || field_0x3198 > 0 && mNormalSpeed >= 30.0f) {

        if (checkMagneBootsOn()) {
            cancelMagneBootsOn();
        }
        setGoatStopGameFail(temp_r3);
        return 1;
    }

    setDoStatusEmphasys(0x15);

    int temp_r25 = getDirectionFromShapeAngle();
    if (mNormalSpeed > 1.0f) {
        f32 temp_f30 = temp_r28->getFrame();
        if (checkAnmEnd(temp_r28)) {
            setSingleAnimeBase(ANM_COW_CATCH);
        }
    
        field_0x2f9d = 0x60;
    
        if (field_0x3198 > 0) {
            field_0x3198 += 1;
            cLib_chaseF(&mNormalSpeed, 30.0f, 1.0f);
        } else {
            f32 var_f31 = 1.0f;
            if ((temp_r27 == PROC_OBJ_GRA || temp_r27 == PROC_E_GOB) && !checkBootsOrArmorHeavy()) {
                field_0x3198++;
            } else if (checkInputOnR() && temp_r25 == DIR_FORWARD) {
                var_f31 += 1.0f;
            }

            if (checkBootsOrArmorHeavy()) {
                var_f31 += 0.5f;
            }

            cLib_chaseF(&mNormalSpeed, 0.0f, var_f31);

            if (mNormalSpeed < 0.1f) {
                mNormalSpeed = 0.1f;
            }
        }

        if (!checkUnderMove0BckNoArc(ANM_COW_CATCH)) {
            setSingleAnimeBase(ANM_COW_CATCH);
            temp_r28->setFrame(temp_f30);
            getNowAnmPackUnder(UNDER_0)->setFrame(temp_f30);
        }

        seStartOnlyReverbLevel(Z2SE_FN_LINK_SLIP);
    } else {
        if (mNormalSpeed > 0.0f) {
            mNormalSpeed = 0.0f;
            setSingleAnimeBase(ANM_COW_PRESS);
            voiceStart(Z2SE_AL_V_LIFTUP_L);
        } else if (checkAnmEnd(temp_r28) && checkGoatStopGame()) {
            dComIfGs_onTmpBit(dSv_event_tmp_flag_c::tempBitLabels[35]);
        }

        field_0x2f99 = 4;

        if (checkAnmEnd(temp_r28)) {
            if (temp_r27 == PROC_E_GOB && !checkBootsOrArmorHeavy()) {
                setGoatStopGameFail(temp_r3);
                return 1;
            }

            if (checkInputOnR() && (temp_r25 == DIR_LEFT || temp_r25 == DIR_RIGHT)) {
                mProcVar3.field_0x300e = temp_r25;
            }

            daAlink_ANM var_r26;
            if (mProcVar3.field_0x300e == 2) {
                if (temp_r27 == PROC_OBJ_GRA) {
                    ((daObj_GrA_c*)temp_r3)->setCrazyThrowLeft();
                } else if (temp_r27 == PROC_COW) {
                    ((daCow_c*)temp_r3)->setCrazyThrowLeft();
                } else {
                    ((fopEn_enemy_c*)temp_r3)->setThrowModeThrowLeft();
                }
                var_r26 = ANM_COW_THROW_LEFT;
            } else {
                if (temp_r27 == PROC_OBJ_GRA) {
                    ((daObj_GrA_c*)temp_r3)->setCrazyThrowRight();
                } else if (temp_r27 == PROC_COW) {
                    ((daCow_c*)temp_r3)->setCrazyThrowRight();
                } else {
                    ((fopEn_enemy_c*)temp_r3)->setThrowModeThrowRight();
                }
                var_r26 = ANM_COW_THROW_RIGHT;
            }

            mProcVar2.field_0x300c = 1;
            setSingleAnimeBase(var_r26);
            field_0x2f99 = 0xC;
            voiceStart(Z2SE_AL_V_THROW_GORON);
        }
    }

    return 1;
}

int daAlink_c::procGoatStrokeInit() {
    if (mEquipItem != fpcNm_ITEM_NONE) {
        return procPreActionUnequipInit(PROC_GOAT_STROKE, field_0x27f4);
    }

    commonProcInit(PROC_GOAT_STROKE);
    field_0x280c.setData(field_0x27f4);
    setSingleAnimeBase(ANM_COW_STROKE);

    static_cast<daCow_c*>(field_0x280c.getActor())->setNaderu();
    mNormalSpeed = 0.0f;

    if ((s16)(fopAcM_searchActorAngleY(field_0x27f4, this) - field_0x27f4->shape_angle.y) > 0) {
        shape_angle.y = field_0x27f4->shape_angle.y - 0x4000;
    } else {
        shape_angle.y = field_0x27f4->shape_angle.y + 0x4000;
    }

    current.pos.x = field_0x27f4->current.pos.x - cM_ssin(shape_angle.y) * 100.0f;
    current.pos.z = field_0x27f4->current.pos.z - cM_scos(shape_angle.y) * 100.0f;
    return 1;
}


int daAlink_c::procGoatStroke() {
    if (mUnderFrameCtrl[0].checkAnmEnd()) {
        checkNextAction(0);
    }

    return 1;
}

int daAlink_c::procGoronMoveInit() {
    if (mEquipItem != fpcNm_ITEM_NONE) {
        return procPreActionUnequipInit(PROC_GORON_MOVE, NULL);
    }

    if (!commonProcInitNotSameProc(PROC_GORON_MOVE)) {
        return 0;
    }

    mCargoCarryAcKeep.setActor();

    if (mCargoCarryAcKeep.getActor() == NULL) {
        offGoronSideMove();
        return 0;
    }

    mNormalSpeed = 0.0f;
    field_0x2f98 = 3;
    current.angle.y = shape_angle.y + -0x4000;
    field_0x594 = mpHIO->mAtnMove.m.mMaxSpeed;
    setSingleAnimeBase(ANM_ATN_RIGHT);
    mProcVar2.field_0x300c = 1;
    mProcVar3.field_0x300e = 0;
    return 1;
}

int daAlink_c::procGoronMove() {
    mCargoCarryAcKeep.setActor();

    if (!mCargoCarryAcKeep.getActor() || !checkGoronSideMove()) {
        offGoronSideMove();
        return checkNextAction(0);
    }

    setShapeAngleToAtnActor(0);

    if (field_0x2f98 == 3) {
        current.angle.y = shape_angle.y - 0x4000;
    } else {
        current.angle.y = shape_angle.y + 0x4000;
    }

    if (checkSetItemTrigger(fpcNm_ITEM_HVY_BOOTS)) {
        return procBootsEquipInit();
    }

    int temp_r28 = cLib_distanceAngleS(field_0x2fe2, shape_angle.y);

    if (mAttention->getActionBtnB() != NULL && mAttention->getActionBtnB()->mType == fopAc_attn_CARRY_e) {
        setDoStatus(0x91);

        if (doTrigger()) {
            mProcVar3.field_0x300e = 1;
        } else if (!doButton()) {
            mProcVar3.field_0x300e = 0;
        }
        if (mProcVar3.field_0x300e != 0) {
            if (field_0x27f4->current.pos.abs2XZ(current.pos) < getGoatCatchDistance2()) {
                offGoronSideMove();
                return procGoatCatchInit(field_0x27f4, 0.0f);
            }
        }
    } else {
        mProcVar3.field_0x300e = 0;
    }

    f32 var_f31;
    if (checkInputOnR() && temp_r28 > 0x800 && temp_r28 < 0x7800) {
        if (abs((s16)(field_0x2fe2 - current.angle.y)) > 0x4000) {
            current.angle.y += 0x8000;
            mNormalSpeed *= -1.0f;

            if (field_0x2f98 == 3) {
                field_0x2f98 = 2;
            } else {
                field_0x2f98 = 3;
            }
        }

        if (field_0x33a8 > (0.3f * (1.0f - fabsf(mNormalSpeed / field_0x594)))) {
            var_f31 = mpHIO->mAtnMove.m.mAcceleration * field_0x33a8;
        } else {
            var_f31 = 0.0f;
        }
    } else {
        var_f31 = 0.0f;
    }

    setNormalSpeedF(var_f31, mpHIO->mAtnMove.m.mDeceleration);

    if (checkZeroSpeedF()) {
        onModeFlg(1);
        field_0x2f98 = 3;
        current.angle.y = shape_angle.y - 0x4000;

        if (!checkUnderMove0BckNoArc(ANM_ATN_RIGHT)) {
            setSingleAnimeBase(ANM_ATN_RIGHT);
        }
    } else {
        offModeFlg(1);
        if (!checkUnderMove0BckNoArc(ANM_COW_MOVE_LEFT_RIGHT)) {
            setSingleAnimeBase(ANM_COW_MOVE_LEFT_RIGHT);
        }

        daPy_frameCtrl_c* temp_r29 = &mUnderFrameCtrl[0];
        f32 var_f30;
        if (field_0x2f98 == 3) {
            var_f30 = -1.0f;
            temp_r29->setLoop(temp_r29->getEnd());
        } else {
            var_f30 = 1.0f;
            temp_r29->setLoop(temp_r29->getStart());
        }

        temp_r29->setRate(var_f30 * (mNormalSpeed / field_0x594));
        initBasAnime();
    }

    return 1;
}
