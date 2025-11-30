#include "d/actor/d_a_npc_kn.h"

u32 daNpc_Kn_c::setParamTeach01() {
    s16 talk_distance = mpHIO->m.common.talk_distance;
    s16 talk_angle = mpHIO->m.common.talk_angle;
    s16 attention_distance = mpHIO->m.common.attention_distance;
    s16 attention_angle = mpHIO->m.common.attention_angle;

    switch (mActionMode) {
    case 0:
        attention_info.distances[fopAc_attn_LOCK_e] = daNpcT_getDistTableIdx(attention_distance, attention_angle);
        attention_info.distances[fopAc_attn_BATTLE_e] = 0xe4;
        attention_info.distances[fopAc_attn_TALK_e] = attention_info.distances[fopAc_attn_LOCK_e];
        attention_info.distances[fopAc_attn_SPEAK_e] = daNpcT_getDistTableIdx(talk_distance, talk_angle);
        return 0;
    case 1:
        if (field_0x170c != 0) {
            return 0;
        }
    case 2:
        attention_info.distances[fopAc_attn_LOCK_e] = daNpcT_getDistTableIdx(attention_distance, attention_angle);
        attention_info.distances[fopAc_attn_BATTLE_e] = 0xe4;
        attention_info.distances[fopAc_attn_TALK_e] = attention_info.distances[fopAc_attn_LOCK_e];
        attention_info.distances[fopAc_attn_SPEAK_e] = daNpcT_getDistTableIdx(talk_distance, talk_angle);
        mAcchCir.SetWallR(mpHIO->m.common.width);
        mAcchCir.SetWallH(mpHIO->m.common.knee_length);
        return fopAc_AttnFlag_BATTLE_e;
    case 3:
        attention_info.distances[fopAc_attn_LOCK_e] = daNpcT_getDistTableIdx(attention_distance, attention_angle);
        attention_info.distances[fopAc_attn_BATTLE_e] = 0xe4;
        attention_info.distances[fopAc_attn_TALK_e] = attention_info.distances[fopAc_attn_LOCK_e];
        attention_info.distances[fopAc_attn_SPEAK_e] = daNpcT_getDistTableIdx(talk_distance, talk_angle);
        mAcchCir.SetWallR(135.0f);
        mAcchCir.SetWallH(mpHIO->m.common.knee_length);
        return fopAc_AttnFlag_BATTLE_e;
    }

    return (fopAc_AttnFlag_SPEAK_e | fopAc_AttnFlag_TALK_e);
}

void daNpc_Kn_c::setCollisionTeach01() {
    switch (mActionMode) {
    case 0:
        break;
    case 1:
        mCylCc.SetTgType(-1);
        mCylCc.SetTgHitMark(CcG_Tg_UNK_MARK_2);
        mCylCc.SetTgSPrm(0x13);
        mCylCc.OnTgNoConHit();

        if (field_0x170c == 0) {
            mCylCc.OnTgShield();
        } else {
            mCylCc.OffTgShield();
        }
        break;
    case 2:
        mCylCc.SetTgType(-1);
        mCylCc.SetTgHitMark(CcG_Tg_UNK_MARK_1);
        mCylCc.SetTgSPrm(0x13);
        mCylCc.OffTgShield();
        mCylCc.OffTgNoAtHitInfSet();
        mCylCc.OnTgNoConHit();
        mCylCc.OnCoSetBit();
        break;
    case 3:
        mCylCc.OffTgSetBit();
        mCylCc.OffCoSetBit();
        break;
    }
}

void daNpc_Kn_c::teach01_selectAction() {
    switch (mActionMode) {
    case 0:
        mpTeachAction = &daNpc_Kn_c::teach01_start;
        return;
    case 1:
        mpTeachAction = &daNpc_Kn_c::teach01_attackWait;
        return;
    case 2:
        mpTeachAction = &daNpc_Kn_c::teach01_swordAttackWait;
        return;
    case 3:
        mpTeachAction = &daNpc_Kn_c::teach01_swordFinishWait;
        return;
    }

    mpTeachAction = &daNpc_Kn_c::wait;
    return;
}

int daNpc_Kn_c::teach01_start(void* param_0) {
    switch (mMode) {
    case 0:
    case 1:
        mFaceMotionSeqMngr.setNo(1, -1.0f, 0, 0);
        mMotionSeqMngr.setNo(0, -1.0f, 0, 0);
        mJntAnm.lookPlayer(0);
        mMode = 2;
    case 2:
        mEvtNo = 5;
        field_0xe39 = 1;
        cLib_addCalcAngleS2(&mCurAngle.y, fopAcM_searchPlayerAngleY(this), 2, 0x800);
        setAngle(mCurAngle.y);
    case 3:
    default:
        return 1;
    }
}

int daNpc_Kn_c::teach01_attackWait(void* param_0) {
    switch (mMode) {
    case 0:
    case 1:
        mMotionSeqMngr.setNo(9, -1.0f, 1, 0);
        mFaceMotionSeqMngr.setNo(1, -1.0f, 0, 0);
        mJntAnm.lookPlayer(0);
        mMode = 2;
    case 2:
        if (!ctrlWarp()) {
            cLib_addCalcAngleS2(&mCurAngle.y, fopAcM_searchPlayerAngleY(this), 2, 0x800);
            setAngle(mCurAngle.y);
            if (mCylCc.ChkTgHit()) {
                mEvtNo = 6;
                field_0x16f4.set(1.0f, 1.0f, 1.0f);
                field_0x170c = 0;
                field_0x170d = 0;
                mSound.startCollisionSE(Z2SE_HIT_SWORD, 0x28);
            }
        }
    case 3:
    default:
        return 1;
    }
}

int daNpc_Kn_c::teach01_swordAttackWait(void* param_0) {
    switch (mMode) {
    case 0:
    case 1:
        mFaceMotionSeqMngr.setNo(1, -1.0f, 0, 0);
        mMotionSeqMngr.setNo(9, -1.0f, 0, 0);
        mJntAnm.lookPlayer(0);
        field_0xdec = mpHIO->m.attack_wait_time;
        mTargetPos = current.pos;
        field_0x15bc = 0;
        field_0x15d0 = cLib_getRndValue(mpHIO->m.attack_start_min_time, mpHIO->m.attack_start_time_range);
        speedF = 0.0f;
        speed.zero();
        mMode = 2;
    case 2:
        calcSlip();
        mCcStts.Move();

        if (mCylCc.ChkTgHit()) {
            int cut_type = daPy_getPlayerActorClass()->getCutType();
            if (cut_type == daPy_py_c::CUT_TYPE_JUMP ||
                cut_type == daPy_py_c::CUT_TYPE_TURN_RIGHT ||
                cut_type == daPy_py_c::CUT_TYPE_TURN_LEFT ||
                daPy_getPlayerActorClass()->getCutCount() >= 4)
            {
                mActionMode = 3;
                setAngle(fopAcM_searchPlayerAngleY(this));
                speedF = -mpHIO->m.attack_disappear_speed_h;
                speed.y = mpHIO->m.attack_disappear_speed_v;
                mSound.startCollisionSE(Z2SE_HIT_SWORD, 0x1f);
                break;
            } else {
                fopAc_ac_c* aTgHitActor_p = mCylCc.GetTgHitAc();
                JUT_ASSERT(303, NULL != aTgHitActor_p);
                s16 angle = cLib_targetAngleY(&aTgHitActor_p->current.pos, &current.pos);
                cXyz pos(0.0f, 0.0f, mpHIO->m.small_slide_distance);
                mDoMtx_stack_c::YrotS(angle);
                mDoMtx_stack_c::multVec(&pos, &mTargetPos);
                mTargetPos += current.pos;
                field_0x15bc = 1;
                mFaceMotionSeqMngr.setNo(1, -1.0f, 0, 0);
                mMotionSeqMngr.setNo(0x1f, -1.0f, 1, 0);
                mSound.startCollisionSE(Z2SE_HIT_SWORD, 0x1e);
            }
        } else {
            if (cLib_calcTimer(&field_0xdec) == 0 || checkCollisionSword()) {
                mTargetPos = current.pos;

                int is_lockon = FALSE;
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
                    mFlowNodeNo = 0x2da;
                } else {
                    mFlowNodeNo = 0x2db;
                }

                break;
            } else {
                calcSwordAttackMove(1);
            }
        }

        break;

    case 3:
    default:
        break;
    }

    return 1;
}

int daNpc_Kn_c::teach01_swordFinishWait(void* param_0) {
    switch (mMode) {
    case 0:
    case 1:
        mFaceMotionSeqMngr.setNo(1, -1.0f, 0, 0);
        mMotionSeqMngr.setNo(0x12, -1.0f, 0, 0);
        mSound.startCreatureVoice(Z2SE_KN_V_DAMAGE_L, -1);
        mJntAnm.lookNone(0);
        field_0xdec = mpHIO->m.attack_wait_time;
        mMode = 2;
    case 2: {
        mTargetPos = current.pos;
        int no = mMotionSeqMngr.getNo();
        if (no == 0x12) {
            if (speed.y <= 0.0f && mAcch.ChkGroundHit()) {
                speedF = 0.0f;
                speed.zero();
                mFaceMotionSeqMngr.setNo(1, -1.0f, 0, 0);
                mMotionSeqMngr.setNo(0x13, -1.0f, 0, 0);
                setLandingPrtcl();
            }
        } else if (no == 0x13) {
            if (mMotionSeqMngr.getStepNo() == 0) {
                if (mpModelMorf[0]->getFrame() >= 13.0f && mpModelMorf[0]->getFrame() < 14.0f) {
                    setLandingPrtcl();
                }
            } else {
                if (!checkDownFlg()) {
                    cXyz pos(10.0f, 0.0f, 0.0f);
                    onDownFlg();

                    int backbone_joint = getBackboneJointNo();
                    mDoMtx_stack_c::copy(mpModelMorf[0]->getModel()->getAnmMtx(backbone_joint));
                    mDoMtx_stack_c::multVec(&pos, &field_0x15c0);
                    setDownPos(&field_0x15c0);
                } else if (checkCutDownHitFlg()) {
                    mFaceMotionSeqMngr.setNo(1, -1.0f, 0, 0);
                    mMotionSeqMngr.setNo(0x14, -1.0f, 0, 0);

                    if (mType == 0) {
                        mEvtNo = 7;
                    } else if (mType == 1) {
                        mEvtNo = 9;
                    }

                    mSound.startCollisionSE(Z2SE_HIT_SWORD_STAB, 0x1e);
                    break;
                }
            }
        }

        if (cLib_calcTimer(&field_0xdec) == 0) {
            if (daPy_getPlayerActorClass()->getCutType() != daPy_py_c::CUT_TYPE_DOWN) {
                offDownFlg();
                mTargetPos = current.pos;
                mEvtNo = 3;
                mFlowNodeNo = 0x2dc;
                mActionMode = 2;
            }
        }
        break;
    }
    case 3:
    default:
        break;
    }

    return 1;
}

int daNpc_Kn_c::ctrlWarp() {
    int sp28 = 0;

    switch (field_0x170c) {
    case 0: {
        cXyz spEC(daPy_getPlayerActorClass()->current.pos);
    
        bool sp8 = (daPy_getPlayerActorClass()->current.pos - current.pos).absXZ() > mpHIO->m.warp_distance;
        if (sp8) {
            sp28 = 1;
            cXyz spE0(0.0f, 0.0f, 600.0f);
            mDoMtx_stack_c::YrotS(daPy_getPlayerActorClass()->shape_angle.y);
            mDoMtx_stack_c::multVec(&spE0, &spE0);
            spE0 += spEC;
            spE0.y += 1000.0f;

            for (int i = 0; i < 5; i++) {
                static Vec l_checkOffset[] = {
                    {0.0f, 0.0f, 0.0f},
                    {50.0f, 0.0f, 0.0f},
                    {-50.0f, 0.0f, 0.0f},
                    {0.0f, 0.0f, 50.0f},
                    {0.0f, 0.0f, -50.0f},
                };

                cXyz spD4 = spE0 + l_checkOffset[i];
                if (!fopAcM_gc_c::gndCheck(&spD4) || !(fopAcM_gc_c::getGroundY() >= current.pos.y - 100.0f)) {
                    sp28 = 0;
                    break;
                }
            }

            if (sp28 == 1) {
                spE0.y = fopAcM_gc_c::getGroundY();
                field_0x1700 = spE0;
                field_0x170c = 1;
                field_0x170d = 0;
                field_0x16f4.set(1.0f, 1.0f, 1.0f);
            }
        }
        break;
    }
    case 1:
        sp28 = 1;
        field_0x170d++;

        static Vec l_WarpInScaleTable[] = {
            {1.0f, 1.0f, 1.0f},
            {1.594f, 0.824f, 1.67f},
            {1.91f, 0.72f, 1.79f},
            {1.594f, 0.81699997f, 1.67f},
            {0.4f, 1.5f, 1.0f},
            {0.0f, 1.3f, 0.0f},
            {0.0f, 1.0f, 0.0f},
            {0.0f, 0.0f, 0.0f},
        };

        if (field_0x170d < 15) {
            for (int i = 0; i < 7; i++) {
                static u8 l_warpInTimeTable[] = {1, 4, 8, 11, 13, 14, 15, 16};

                int sp1C = l_warpInTimeTable[i];
                int sp18 = l_warpInTimeTable[i + 1];
                if (sp1C <= field_0x170d && sp18 > field_0x170d) {
                    cXyz spC8(l_WarpInScaleTable[i]);
                    cXyz spBC(l_WarpInScaleTable[i + 1]);

                    field_0x16f4 = spC8 + (((spBC - spC8) * (field_0x170d - sp1C)) / (sp18 - sp1C));
                }
            }
        } else {
            field_0x16f4 = l_WarpInScaleTable[7];
            field_0x170c = 2;
            field_0x170d = 0;
        }
        break;
    case 2:
        sp28 = 1;
        field_0x170d++;
        if (field_0x170d >= 5) {
            field_0x170d = 0;
            field_0x170c = 3;
            setPos(field_0x1700);
            setAngle(fopAcM_searchPlayerAngleY(this));
        }
        break;
    case 3:
        sp28 = 1;
        field_0x170d++;

        static Vec l_WarpOutScaleTable[] = {
            {0.0f, 1.0f, 0.0f},
            {0.0f, 1.32f, 0.0f},
            {0.594f, 1.5f, 1.0f},
            {1.0f, 1.0f, 1.0f},
        };

        if (field_0x170d < 6) {
            for (int i = 0; i < 3; i++) {
                static u8 l_warpOutTimeTable[] = {1, 3, 5, 6};

                int sp10 = l_warpOutTimeTable[i];
                int spC = l_warpOutTimeTable[i + 1];
                if (sp10 <= field_0x170d && spC > field_0x170d) {
                    cXyz spB0(l_WarpOutScaleTable[i]);
                    cXyz spA4(l_WarpOutScaleTable[i + 1]);

                    field_0x16f4 = spB0 + (((spA4 - spB0) * (field_0x170d - sp10)) / (spC - sp10));
                }
            }
        } else {
            field_0x16f4 = l_WarpOutScaleTable[3];
            field_0x170c = 0;
            field_0x170d = 0;
        }
        break;
    }

    return sp28;
}

int daNpc_Kn_c::ECut_firstEncount(int i_idx) {
    dEvent_manager_c* event_manager = &dComIfGp_getEventManager();

    int reg_r24 = 0;
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
            mJntAnm.lookPlayer(0);
            mActionMode = 1;
            break;
        case 10:
            daPy_getPlayerActorClass()->changeDemoMoveAngle(fopAcM_searchPlayerAngleY(this) + 0x8000);
#if VERSION == VERSION_SHIELD_DEBUG
            field_0xdec = 50;
#else
            field_0xdec = 52;
#endif
            break;
        case 20:
            break;
        case 30:
            mMotionSeqMngr.setNo(9, -1.0f, 1, 0);
            mFaceMotionSeqMngr.setNo(1, -1.0f, 0, 0);
            break;
        }
    }

    switch (prm) {
    case 0:
        rt = 1;
        break;
    case 10:
        if (cLib_calcTimer<int>(&field_0xdec) == 0) {
            fopAc_ac_c* parent_p = (fopAc_ac_c*)fpcM_SearchByID(parentActorID);
            if (parent_p != NULL) {
                fopAcM_delete(parent_p);
            }

            field_0xe2c = 0;
            rt = 1;
        }
        break;
    default:
        rt = 1;
        break;
    }

    return rt;
}

int daNpc_Kn_c::ECut_attackFailed(int i_idx) {
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
            daNpcT_offTmpBit(0xD);
            
            mMotionSeqMngr.setNo(0x1A, -1.0f, 1, 0);
            mFaceMotionSeqMngr.setNo(1, -1.0f, 0, 0);
            
            mJntAnm.lookPlayer(0);
            field_0xdec = 12;
            break;
        case 10:
            break;
        case 20:
            initTalk(0x2D6, NULL);
            break;
        case 25:
            initTalk(0x2D6, NULL);
            break;
        case 30:
            initTalk(0x2D6, NULL);
            break;
        case 40:
            initTalk(0x2D6, NULL);
            break;
        case 50: {
            Z2GetAudioMgr()->bgmStart(Z2BGM_OUGI_TRAINING, 0, 0);
            current.pos = home.pos;
            old.pos = home.pos;
            setAngle(home.angle.y);

            cXyz sp18(0.0f, 0.0f, 375.0f);
            mDoMtx_stack_c::transS(current.pos);
            mDoMtx_stack_c::YrotM(home.angle.y);
            mDoMtx_stack_c::multVec(&sp18, &sp18);

            cXyz spC(sp18);
            spC.y += 500.0f;
            if (fopAcM_gc_c::gndCheck(&spC)) {
                sp18.y = fopAcM_gc_c::getGroundY();
            }

            daPy_getPlayerActorClass()->setPlayerPosAndAngle(&sp18, fopAcM_searchPlayerAngleY(this) + 0x8000, 0);
            daPy_getPlayerActorClass()->onTraningCutDown();
            break;
        }
        case 60:
            initTalk(0x2D8, NULL);
            break;
        case 70:
            initTalk(0x2D9, NULL);
            break;
        case 80:
            mFaceMotionSeqMngr.setNo(1, -1.0f, 0, 0);
            mMotionSeqMngr.setNo(24, -1.0f, 1, 0);
            mJntAnm.lookNone(0);
            break;
        }
    }

    switch (prm) {
    case 0: 
        if (cLib_calcTimer<int>(&field_0xdec) == 0 && daPy_getPlayerActorClass()->checkPlayerFly()) {
            rt = 1;
            daPy_getPlayerActorClass()->onLargeDamageUpStop();
        } else if (field_0xdec == 1) {
            daPy_getPlayerActorClass()->changeDemoMoveAngle(fopAcM_searchPlayerAngleY(this) + 0x8000);
            daPy_getPlayerActorClass()->setThrowDamage(fopAcM_searchPlayerAngleY(this), 20.0f, 30.0f, 0, 1, 0);
        }

        cLib_addCalcAngleS2(&mCurAngle.y, fopAcM_searchPlayerAngleY(this), 2, 0x800);
        setAngle(mCurAngle.y);
        break;
    case 10: 
        if (!daPy_getPlayerActorClass()->checkPlayerFly()) {
            rt = 1;
        }

        daPy_getPlayerActorClass()->onLargeDamageUpStop();
        cLib_addCalcAngleS2(&mCurAngle.y, fopAcM_searchPlayerAngleY(this), 2, 0x800);
        setAngle(mCurAngle.y);
        break;
    case 20:
        if (talkProc(NULL, 0, NULL, 0)) {
            rt = 1;
        } else {
            daPy_getPlayerActorClass()->onLargeDamageUpStop();
        }
        break;
    case 40:
    case 30:
    case 25:
        if (talkProc(NULL, 0, NULL, 0)) {
            rt = 1;
        }
        break;
    case 50:
        setAngle(fopAcM_searchPlayerAngleY(this));
        rt = 1;
        break;
    case 70:
    case 60:
        if (talkProc(NULL, 0, NULL, 0)) {
            rt = 1;
        }
        break;
    case 80:
        if (mMotionSeqMngr.getNo() == 0x18 && mMotionSeqMngr.getStepNo() > 0) {
            rt = 1;
            mActionMode = 2;
        }
        break;
    default:  
        rt = 1;
        break;
    }

    return rt;
}

int daNpc_Kn_c::ECut_firstSkillGet(int i_idx) {
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
            daNpcT_offTmpBit(0xD);
            daNpcT_offTmpBit(0xE);
            daNpcT_offTmpBit(0xF);
            field_0xdec = 30;
            break;
        case 2: 
            mFaceMotionSeqMngr.setNo(1, -1.0f, 0, 0);
            mMotionSeqMngr.setNo(0x15, -1.0f, 0, 0);
            break;
        case 4:
            mFaceMotionSeqMngr.setNo(1, -1.0f, 0, 0);
            mMotionSeqMngr.setNo(0x16, -1.0f, 0, 0);
            break;
        case 6: 
            mJntAnm.lookNone(0);
            break;
        case 10: {
            mStepMode = 2;

            cXyz sp1C(0.0f, 0.0f, 500.0f);
            mDoMtx_stack_c::transS(current.pos);
            mDoMtx_stack_c::YrotM(cLib_targetAngleY(&current.pos, &home.pos));
            mDoMtx_stack_c::multVec(&sp1C, &sp1C);

            cXyz sp10(sp1C);
            sp10.y += 500.0f;
            if (fopAcM_gc_c::gndCheck(&sp10)) {
                sp1C.y = fopAcM_gc_c::getGroundY();
            }

            daPy_getPlayerActorClass()->setPlayerPosAndAngle(&sp1C, (s16)cLib_targetAngleY(&home.pos, &current.pos), 0);
            initTalk(0x2DD, NULL);
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
            initTalk(0x2DD, NULL);
            break;
        case 30:
            initTalk(0x2DD, NULL);
            break;
        case 40:
            initTalk(0x2DD, NULL);
            break;
        case 50:
            initTalk(0x2DD, NULL);
            break;
        case 60:
            initTalk(0x2DD, NULL);
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
            rt = 1;
        }
        cLib_addCalcAngleS2(&mCurAngle.y, fopAcM_searchPlayerAngleY(this), 2, 0x800);
        setAngle(mCurAngle.y);
        break;
    case 40:
        if (talkProc(NULL, 0, NULL, 0)) {
            rt = 1;
        }
        break;
    case 50:
        if (talkProc(NULL, 0, NULL, 0)) {
            rt = 1;
        }
        break;
    case 60:
        if (talkProc(NULL, 0, NULL, 0)) {
            rt = 1;
        }
        break;
    default:
        rt = 1;
        break;
    }

    return rt;
}
