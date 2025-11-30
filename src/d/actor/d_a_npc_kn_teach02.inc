#include "d/actor/d_a_npc_kn.h"
#include "d/actor/d_a_obj_knBullet.h"

u32 daNpc_Kn_c::setParamTeach02() {
    s16 talk_distance = mpHIO->m.common.talk_distance;
    s16 talk_angle = mpHIO->m.common.talk_angle;
    s16 attention_distance = mpHIO->m.common.attention_distance;
    s16 attention_angle = mpHIO->m.common.attention_angle;

    switch (mActionMode) {
    case 7:
    case 6:
        attention_info.distances[fopAc_attn_LOCK_e] = daNpcT_getDistTableIdx(attention_distance, attention_angle);
        attention_info.distances[fopAc_attn_BATTLE_e] = 0xe4;
        attention_info.distances[fopAc_attn_TALK_e] = attention_info.distances[fopAc_attn_LOCK_e];
        attention_info.distances[fopAc_attn_SPEAK_e] = daNpcT_getDistTableIdx(talk_distance, talk_angle);
        return 4;
    case 5: {
        attention_info.distances[fopAc_attn_LOCK_e] = daNpcT_getDistTableIdx(attention_distance, attention_angle);
        attention_info.distances[fopAc_attn_BATTLE_e] = 0xe4;
        attention_info.distances[fopAc_attn_TALK_e] = attention_info.distances[fopAc_attn_LOCK_e];
        attention_info.distances[fopAc_attn_SPEAK_e] = daNpcT_getDistTableIdx(talk_distance, talk_angle);
        cXyz pos;
        mDoMtx_stack_c::copy(mpModelMorf[0]->getModel()->getAnmMtx(getBackboneJointNo()));
        mDoMtx_stack_c::multVecZero(&pos);

        if ((daPy_getPlayerActorClass()->current.pos - pos).absXZ() < 255.0f) {
            return fopAc_AttnFlag_UNK_0x200000 | fopAc_AttnFlag_BATTLE_e;
        } else {
            return fopAc_AttnFlag_BATTLE_e;
        }
    }
    case 4:
        attention_info.distances[fopAc_attn_LOCK_e] = daNpcT_getDistTableIdx(attention_distance, attention_angle);
        attention_info.distances[fopAc_attn_BATTLE_e] = 0xe4;
        attention_info.distances[fopAc_attn_TALK_e] = attention_info.distances[fopAc_attn_LOCK_e];
        attention_info.distances[fopAc_attn_SPEAK_e] = daNpcT_getDistTableIdx(talk_distance, talk_angle);
        mAcchCir.SetWallR(mpHIO->m.common.width);
        mAcchCir.SetWallH(mpHIO->m.common.knee_length);
        return 0;
    }

    return (fopAc_AttnFlag_SPEAK_e | fopAc_AttnFlag_TALK_e);
}

void daNpc_Kn_c::setCollisionTeach02() {
    switch (mActionMode) {
    case 4:
        break;
    case 5:
        mCylCc.SetTgType(-1);
        mCylCc.SetTgHitMark(CcG_Tg_UNK_MARK_2);
        mCylCc.SetTgSPrm(0x13);
        mCylCc.OnTgShield();
        mCylCc.OffTgNoAtHitInfSet();
        mCylCc.OnTgNoConHit();
        break;
    case 6:
        mCylCc.SetTgType(-1);
        mCylCc.SetTgHitMark(CcG_Tg_UNK_MARK_1);
        mCylCc.SetTgSPrm(0x13);
        mCylCc.OffTgShield();
        mCylCc.OffTgNoAtHitInfSet();
        mCylCc.OnTgNoConHit();
        break;
    case 7:
        mCylCc.SetTgType(-1);
        mCylCc.SetTgHitMark(CcG_Tg_UNK_MARK_2);
        mCylCc.SetTgSPrm(0x13);
        mCylCc.OffTgShield();
        mCylCc.OnTgNoConHit();
        break;
    }
}

void daNpc_Kn_c::teach02_selectAction() {
    switch (mActionMode) {
    case 4:
        mpTeachAction = &daNpc_Kn_c::teach02_start;
        return;
    case 5:
        mpTeachAction = &daNpc_Kn_c::teach02_shieldBashWait;
        return;
    case 6:
        mpTeachAction = &daNpc_Kn_c::teach02_finishWait;
        return;
    case 7:
        mpTeachAction = &daNpc_Kn_c::teach02_shieldReflectWait;
        return;
    }

    mpTeachAction = &daNpc_Kn_c::wait;
    return;
}

int daNpc_Kn_c::teach02_shieldBashWait(void* param_0) {
    switch (mMode) {
    case 0:
    case 1:
        mFaceMotionSeqMngr.setNo(1, -1.0f, 0, 0);
        mMotionSeqMngr.setNo(9, -1.0f, 0, 0);
        mJntAnm.lookPlayer(0);
        field_0xdec = mpHIO->m.attack_wait_time;
        mTargetPos = current.pos;
        field_0x15bc = 0;
        field_0x15d0 =
            cLib_getRndValue(mpHIO->m.attack_start_min_time, mpHIO->m.attack_start_time_range);
        speedF = 0.0f;
        speed.zero();
        mMode = 2;
    case 2:
        mCcStts.Move();

        if (mCylCc.ChkTgShieldHit()) {
            if (daPy_getPlayerActorClass()->getCutType() == daPy_py_c::CUT_TYPE_GUARD_ATTACK) {
                s16 angle = cLib_targetAngleY(&daPy_getPlayerActorClass()->current.pos,
                                              &attention_info.position);
                cXyz pos(0.0f, 0.0f, mpHIO->m.small_slide_distance);

                mDoMtx_stack_c::YrotS(angle);
                mDoMtx_stack_c::multVec(&pos, &mTargetPos);
                mTargetPos += current.pos;
                field_0x15bc = 1;
                mCylCc.ClrTgHit();
                mActionMode = 6;
                mSound.startCollisionSE(Z2SE_HIT_SHIELD_ATTACK, 0x28);
                break;
            }
        }

        if (!cLib_calcTimer(&field_0xdec) || checkCollisionSword()) {
            BOOL is_lockon = FALSE;
            dAttention_c* attention_p = dComIfGp_getAttention();
            if (attention_p->LockonTruth()) {
                is_lockon = TRUE;
            }

            if (checkCollisionSword()) {
                mEvtNo = 4;
            } else {
                mEvtNo = 1;
            }

            mFaceMotionSeqMngr.setNo(1, -1.0f, 0, 0);
            mMotionSeqMngr.setNo(0, -1.0f, 1, 0);

            if (is_lockon) {
                mFlowNodeNo = 0x2c0;
            } else {
                mFlowNodeNo = 0x2bf;
            }

            break;
        } else {
            if (mCylCc.ChkTgHit()) {
                if (!mCylCc.GetTgHitObj()->ChkAtType(0x10)) {
                    mFaceMotionSeqMngr.setNo(1, -1.0f, 0, 0);
                    mMotionSeqMngr.setNo(0x1b, -1.0f, 1, 0);
                    mSound.startCollisionSE(Z2SE_HIT_SWORD, 0x28);
                    break;
                }
            }

            calcSwordAttackMove(1);
        }
    case 3:
    default:
        break;
    }

    return 1;
}

int daNpc_Kn_c::teach02_finishWait(void* param_0) {
    switch (mMode) {
    case 0:
    case 1:
        mFaceMotionSeqMngr.setNo(1, -1.0f, 0, 0);
        mMotionSeqMngr.setNo(11, -1.0f, 1, 0);
        mJntAnm.lookPlayer(0);
        field_0xdec = mpHIO->m.followup_wait_time;
        mMode = 2;
    case 2:
        calcSlip();
        mCcStts.Move();

        if (mCylCc.ChkTgHit()) {
            if (daPy_getPlayerActorClass()->getCutType() != daPy_py_c::CUT_TYPE_GUARD_ATTACK) {
                if (mType == 1) {
                    mEvtNo = 10;
                } else if (mType == 2) {
                    mEvtNo = 12;
                }

                cXyz pos(0.0f, 0.0f, mpHIO->m.big_slide_distance);
                s16 tgt_ang = cLib_targetAngleY(&daPy_getPlayerActorClass()->current.pos,
                                                &attention_info.position);
                mDoMtx_stack_c::YrotS(tgt_ang);
                mDoMtx_stack_c::multVec(&pos, &mTargetPos);
                mTargetPos += current.pos;
                field_0x15bc = 1;
                mSound.startCollisionSE(Z2SE_HIT_SWORD, 0x1e);
                break;
            }
        }

        if (!cLib_calcTimer(&field_0xdec)) {
            mEvtNo = 1;
            mFlowNodeNo = 0x2c1;
            mActionMode = 5;

            mFaceMotionSeqMngr.setNo(1, -1.0f, 0, 0);
            mMotionSeqMngr.setNo(0, -1.0f, 1, 0);
        }
    case 3:
    default:
        break;
    }

    return 1;
}

int daNpc_Kn_c::teach02_shieldReflectWait(void* param_0) {

    switch (mMode) {
    case 0:
    case 1:
        mFaceMotionSeqMngr.setNo(1, -1.0f, 0, 0);
        mMotionSeqMngr.setNo(32, -1.0f, 0, 0);
        mJntAnm.lookPlayer(0);
        mMode = 2;
    case 2:
        mCcStts.Move();

        if (field_0x15bd != 0) {
            if (!daPy_getPlayerActorClass()->checkPlayerFly()) {
                mEvtNo = 2;
                if (field_0x15bd == 1) {
                    mFlowNodeNo = 0x2C4;
                } else if (field_0x15bd == 2) {
                    mFlowNodeNo = 0x2C3;
                }
                field_0x15bd = 0;
            }
        } else {
            if (parentActorID == fpcM_ERROR_PROCESS_ID_e || fpcM_SearchByID(parentActorID) == NULL) {
                if (mMotionSeqMngr.getNo() == 0x19 && mMotionSeqMngr.getStepNo() == 0) {
                    if (mpModelMorf[0]->getFrame() >= 2.0f && mpModelMorf[0]->getFrame() < 3.0f) {
                        cXyz sp20(150.0f, 0.0f, 0.0f);
                        mDoMtx_stack_c::copy(mpModelMorf[0]->getModel()->getAnmMtx(0xD));
                        mDoMtx_stack_c::multVec(&sp20, &sp20);
                        parentActorID = fopAcM_createChild(PROC_KN_BULLET, fopAcM_GetID(this), 0, &sp20, fopAcM_GetRoomNo(this), &current.angle, NULL, -1, NULL);
                    }
                } else {
                    mMotionSeqMngr.setNo(0x19, -1.0f, 1, 0);
                    mFaceMotionSeqMngr.setNo(1, -1.0f, 0, 0);
                }
            } else {
                fopAc_ac_c* parent_p;
                if (fopAcM_SearchByID(parentActorID, &parent_p) && parent_p != NULL && fopAcM_GetName(parent_p) == PROC_KN_BULLET && ((daObjKnBullet_c*)parent_p)->getActionMode() == 0) {
                    if (mpModelMorf[0]->getFrame() >= 34.0f && mpModelMorf[0]->getFrame() < 35.0f) {
                        ((daObjKnBullet_c*)parent_p)->setActionMode(1);
                        parent_p->current.angle.y = mCurAngle.y;
                        parent_p->shape_angle.y = mCurAngle.y;
                    }
                }
            }

            if (mCylCc.ChkTgHit()) {
                fopAc_ac_c* aTgHitActor_p = mCylCc.GetTgHitAc();
                JUT_ASSERT(437, NULL != aTgHitActor_p);

                if (fopAcM_GetName(aTgHitActor_p) == PROC_KN_BULLET) {
                    mEvtNo = 0xB;

                    s16 var_r27 = cLib_targetAngleY(&aTgHitActor_p->current.pos, &current.pos);
                    cXyz sp14(0.0f, 0.0f, mpHIO->m.big_slide_distance);
                    mDoMtx_stack_c::YrotS(var_r27);
                    mDoMtx_stack_c::multVec(&sp14, &mTargetPos);
                    mTargetPos += current.pos;
                    field_0x15bc = 1;
                    mSound.startCollisionSE(Z2SE_HIT_HARD_BODY, 0x1E);
                }
            }
            cLib_addCalcAngleS2(&mCurAngle.y, fopAcM_searchPlayerAngleY(this), 2, 0x800);
            setAngle(mCurAngle.y);
        }
        break;
    case 3:
    default:
        break;
    }

    return 1;
}

int daNpc_Kn_c::ECut_secondSkillExplain(int i_idx) {
    dEvent_manager_c* event_manager = &dComIfGp_getEventManager();
    int sp8 = 0;

    int rt = 0;
    int* prm_p = NULL;
    int prm = -1;

    prm_p = dComIfGp_evmng_getMyIntegerP(i_idx, "prm");
    if (prm_p != NULL) {
        prm = *prm_p;
    }

    if (event_manager->getIsAddvance(i_idx)) {
        switch (prm) {
        case 0:
            field_0xdec = 30;
            break;
        case 2:
            mFaceMotionSeqMngr.setNo(1, -1.0f, 0, 0);
            mMotionSeqMngr.setNo(21, -1.0f, 0, 0);
            break;
        case 4: 
            mFaceMotionSeqMngr.setNo(1, -1.0f, 0, 0);
            mMotionSeqMngr.setNo(22, -1.0f, 0, 0);
            break;
        case 6: 
            mJntAnm.lookNone(0);
            break;
        case 10: 
            mStepMode = 2;
            initTalk(0x2D7, NULL);
            break;
        case 20: {
            Z2GetAudioMgr()->bgmStart(Z2BGM_OUGI_TRAINING, 0, 0);

            setPos(home.pos);
            setAngle(home.angle.y);
    
            cXyz sp64(0.0f, 0.0f, 375.0f);
            mDoMtx_stack_c::transS(home.pos);
            mDoMtx_stack_c::YrotM(home.angle.y);
            mDoMtx_stack_c::multVec(&sp64, &sp64);

            cXyz sp58(sp64);
            sp58.y += 500.0f;
            if (fopAcM_gc_c::gndCheck(&sp58)) {
                sp64.y = fopAcM_gc_c::getGroundY() + 15.0f;
            }

            daPy_getPlayerActorClass()->setPlayerPosAndAngle(&sp64, home.angle.y + 0x8000, 0);
            daPy_getPlayerActorClass()->onTraningShieldAttack();
            break;
        }
        case 30:
            initTalk(0x2C6, NULL);
            break;
        case 40:
            initTalk(0x2BE, NULL);
            break;
        case 50:
            mFaceMotionSeqMngr.setNo(1, -1.0f, 0, 0);
            mMotionSeqMngr.setNo(24, -1.0f, 1, 0);
            mJntAnm.lookNone(0);
            break;
        }
    }

    switch (prm) {
    case 0: 
        if (cLib_calcTimer<int>(&field_0xdec) == 0) {
            rt = 1;
        }
        break;
    case 2: 
        if (mMotionSeqMngr.getNo() == 0x15 && mMotionSeqMngr.getStepNo() > 0 && mMorfLoops != 0) {
            rt = 1;
        }
        break;
    case 4: 
        if (mMotionSeqMngr.getNo() == 0x16 && mMotionSeqMngr.getStepNo() > 0) {
            rt = 1;
        }
        break;
    case 6: 
        if (mCurAngle.y != fopAcM_searchPlayerAngleY(this)) {
            if (step(fopAcM_searchPlayerAngleY(this), 1, 0x20, 20, 0)) {
                rt = 1;
                mFaceMotionSeqMngr.setNo(1, -1.0f, 0, 0);
                mMotionSeqMngr.setNo(0, -1.0f, 0, 0);
            }
        } else {
            rt = 1;
        }
        break;
    case 10:
        if (talkProc(NULL, 0, NULL, 0)) {
            rt = 1;
        }
        break;
    case 20: {
        cXyz sp28(0.0f, 0.0f, 375.0f);
        mDoMtx_stack_c::transS(home.pos);
        mDoMtx_stack_c::YrotM(home.angle.y);
        mDoMtx_stack_c::multVec(&sp28, &sp28);

        cXyz sp1C(sp28);
        sp1C.y += 500.0f;
        if (fopAcM_gc_c::gndCheck(&sp1C)) {
            sp28.y = fopAcM_gc_c::getGroundY();
        }

        daPy_getPlayerActorClass()->setPlayerPosAndAngle(&sp28, home.angle.y + 0x8000, 0);
        rt = 1;
        break;
    }
    case 30:
    case 40:
        if (talkProc(NULL, 0, NULL, 0)) {
            rt = 1;
        }
        break;
    case 50:
        if (mMotionSeqMngr.getNo() == 0x18 && mMotionSeqMngr.getStepNo() > 0) {
            rt = 1;
            mActionMode = 5;
            offDownFlg();
        }
        break;
    default:
        rt = 1;
        break;
    }

    return rt;
}

int daNpc_Kn_c::ECut_reflectExplain(int i_idx) {
    dEvent_manager_c* event_manager = &dComIfGp_getEventManager();
    int sp8 = 0;

    int rt = 0;
    int* prm_p = NULL;
    int prm = -1;

    prm_p = dComIfGp_evmng_getMyIntegerP(i_idx, "prm");
    if (prm_p != NULL) {
        prm = *prm_p;
    }

    if (event_manager->getIsAddvance(i_idx)) {
        switch (prm) {
        case 0:
            daNpcT_offTmpBit(0xB);
            mFaceMotionSeqMngr.setNo(1, -1.0f, 0, 0);
            mMotionSeqMngr.setNo(28, -1.0f, 1, 0);
            mJntAnm.lookPlayer(0);
            field_0xdec = 30;
            break;
        case 10:
            mStepMode = 2;
            initTalk(0x2C2, NULL);
            break;
        case 20: {
            setPos(home.pos);
            setAngle(home.angle.y);

            cXyz sp24(0.0f, 0.0f, 600.0f);
            mDoMtx_stack_c::transS(current.pos);
            mDoMtx_stack_c::YrotM(home.angle.y);
            mDoMtx_stack_c::multVec(&sp24, &sp24);

            cXyz sp18(sp24);
            sp18.y += 500.0f;
            if (fopAcM_gc_c::gndCheck(&sp18)) {
                sp24.y = fopAcM_gc_c::getGroundY();
            }

            daPy_getPlayerActorClass()->setPlayerPosAndAngle(&sp24, home.angle.y + 0x8000, 0);
            break;
        }
        case 30:
            initTalk(0x2C2, NULL);
            break;
        }
    }

    switch (prm) {
    case 0: 
        if (cLib_calcTimer<int>(&field_0xdec) == 0) {
            mTargetPos = current.pos;
            field_0x15bc = 0;
            speedF = 0.0f;
            speed.zero();

            if (mCurAngle.y == fopAcM_searchPlayerAngleY(this)) {
                rt = 1;
            } else if (step(fopAcM_searchPlayerAngleY(this), 1, 0x20, 20, 0)) {
                mFaceMotionSeqMngr.setNo(1, -1.0f, 0, 0);
                mMotionSeqMngr.setNo(0, -1.0f, 0, 0);
            }
        } else {
            calcSlip();
        }
        break;
    case 10:
        if (talkProc(NULL, 0, NULL, 0)) {
            rt = 1;
        }
        break;
    case 20:
        rt = 1;
        break;
    case 30:
        if (talkProc(NULL, 0, NULL, 0)) {
            rt = 1;
            mActionMode = 7;
        }
        break;
    default:
        rt = 1;
        break;
    }

    return rt;
}

int daNpc_Kn_c::ECut_secondSkillGet(int i_idx) {
    dEvent_manager_c* event_manager = &dComIfGp_getEventManager();
    int sp8 = 0;

    int rt = 0;
    int* prm_p = NULL;
    int prm = -1;

    prm_p = dComIfGp_evmng_getMyIntegerP(i_idx, "prm");
    if (prm_p != NULL) {
        prm = *prm_p;
    }

    if (event_manager->getIsAddvance(i_idx)) {
        switch (prm) {
        case 0:
            daNpcT_offTmpBit(0xB);
            daNpcT_offTmpBit(0xC);

            mFaceMotionSeqMngr.setNo(1, -1.0f, 0, 0);
            mMotionSeqMngr.setNo(28, -1.0f, 1, 0);

            mJntAnm.lookPlayer(0);
            field_0xdec = 30;
            break;
        case 10: {
            mStepMode = 2;

            cXyz sp18(0.0f, 0.0f, 500.0f);
            mDoMtx_stack_c::transS(current.pos);
            mDoMtx_stack_c::YrotM(cLib_targetAngleY(&current.pos, &home.pos));
            mDoMtx_stack_c::multVec(&sp18, &sp18);

            cXyz spC(sp18);
            spC.y += 500.0f;
            if (fopAcM_gc_c::gndCheck(&spC)) {
                sp18.y = fopAcM_gc_c::getGroundY();
            }

            daPy_getPlayerActorClass()->setPlayerPosAndAngle(&sp18, (s16)cLib_targetAngleY(&home.pos, &current.pos), 0);
            initTalk(0x2C5, NULL);
            break;
        }
        case 12:
            setAngle(fopAcM_searchPlayerAngleY(this));
            break;
        case 15:
            Z2GetAudioMgr()->bgmStreamPrepare(0x2000038);
            Z2GetAudioMgr()->bgmStreamPlay();
            break;
        case 20:
            initTalk(0x2C5, NULL);
            break;
        case 30:
            initTalk(0x2C5, NULL);
            break;
        }
    }

    switch (prm) {
    case 0: 
        if (cLib_calcTimer<int>(&field_0xdec) == 0) {
            mTargetPos = current.pos;
            field_0x15bc = 0;
            speedF = 0.0f;
            speed.zero();

            if (mCurAngle.y == fopAcM_searchPlayerAngleY(this)) {
                rt = 1;
            } else if (step(fopAcM_searchPlayerAngleY(this), 1, 0x20, 20, 0)) {
                mFaceMotionSeqMngr.setNo(1, -1.0f, 0, 0);
                mMotionSeqMngr.setNo(0, -1.0f, 0, 0);
            }
        } else {
            calcSlip();
        }
        break;
    case 10:
        if (talkProc(NULL, 0, NULL, 0)) {
            rt = 1;
            if (field_0x170e == 0) {
                field_0x170e = 1;
                Z2GetAudioMgr()->bgmStop(0x32, 0);
            }
        }
        break;
    case 20:
        if (talkProc(NULL, 0, NULL, 0)) {
            rt = 1;
        }
        cLib_addCalcAngleS2(&mCurAngle.y, fopAcM_searchPlayerAngleY(this), 2, 0x800);
        setAngle(mCurAngle.y);
        break;
    case 30:
        if (talkProc(NULL, 0, NULL, 0)) {
            setSceneChange(1);
        }
        cLib_addCalcAngleS2(&mCurAngle.y, fopAcM_searchPlayerAngleY(this), 2, 0x800);
        setAngle(mCurAngle.y);
        break;
    default:
        rt = 1;
        break;
    }

    return rt;
}
