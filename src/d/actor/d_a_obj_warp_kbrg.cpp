/**
 * @file d_a_obj_warp_kbrg.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_warp_kbrg.h"
#include "d/d_com_inf_game.h"
#include "d/actor/d_a_player.h"
#include "d/actor/d_a_midna.h"
#include "d/d_s_play.h"
#include "d/d_bg_w.h"
#include "d/d_meter2_info.h"

static const int l_bmd_idx[] = {9, 11};

static const int l_dzb_idx[] = {15, 22};

static const int l_btk_idx[] = {12, 19};

static const int l_bck_idx[] = {6, 7};

static const u16 l_disapp_eff[] = {0x8532, 0x8533, 0x8534, 0x8535, 0x8536};

static const u16 l_start_eff[] = {0x8B3F, 0x8B40, 0x8B41};

static const u16 l_app_eff[] = {0x852D, 0x852E, 0x852F, 0x8530, 0x8531, 0x8B3C};

static const u16 l_end_eff[] = {0x8B3D, 0x8B3E};

static const u16 l_bind_eff[] = {0x8B4F, 0x8B50, 0x8B51, 0x8B52};

static const u32 l_heap_size[] = {0x6400, 0x7E60};

static const Vec l_midna_wait_offset = {0.0f, -200.0f, 1200.0f};

static int eventCallBack(void* i_proc, int param_1) {
    daPy_py_c* player_p = daPy_getPlayerActorClass();

    if (i_proc != NULL) {
        player_p->skipPortalObjWarp();
        if (dComIfGp_TransportWarp_check()) {
            /* dSv_event_flag_c::M_018 - Main Event - Brought Kakariko bridge back to original location */
            dComIfGs_onEventBit(dSv_event_flag_c::saveBitLabels[51]);
            // "Kakariko Bridge: Warp destination is correct"
            OS_REPORT("\x1B[43;30mカカリコ橋：ワープ先正解です\n\x1B[m");
        }
    }

    return 1;
}

void daObjWarpKBrg_c::initBaseMtx() {
    mpBridgeModel->setBaseScale(scale);

    if (mpPortalModel != NULL) {
        mpPortalModel->setBaseScale(scale);
    }

    setBaseMtx();
}

void daObjWarpKBrg_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos);
    mpBridgeModel->setBaseTRMtx(mDoMtx_stack_c::get());

    mDoMtx_stack_c::ZXYrotM(shape_angle);
    MTXCopy(mDoMtx_stack_c::get(), mBgMtx);

    if (mpPortalModel != NULL) {
        cXyz portal_pos(-7643.0f, -2770.0f + KREG_F(0), 56750.0f);

        mDoMtx_stack_c::transS(portal_pos);
        mDoMtx_stack_c::scaleM(5.0f, 5.0f, 5.0f);
        mpPortalModel->setBaseTRMtx(mDoMtx_stack_c::get());
    }
}

static char* l_arcName[2] = {
    "Obj_kbrgS",
    "Obj_kbrgD",
};

static char* l_evArcName = "Obj_kbrg";

static char* l_portal_warp_name[2] = {
    "PORTAL_WARP_KBRIDGE",
    "PORTAL_WARP_KBRIDGE_OUT",
};

static char* l_staff_name = "WarpBrg";

int daObjWarpKBrg_c::Create() {
    switch (getNameArg()) {
    case 0:
        mEventType = 0;
        break;
    case 1:
        if (!fopAcM_isSwitch(this, getSwbit())) {
            setAction(ACTION_ORDER_EVENT2_e);

            if (mpBgW != NULL && mpBgW->ChkUsed()) {
                dComIfG_Bgsp().Release(mpBgW);
            }
        } else {
            mpBridgeBck->setFrame(mpBridgeBck->getEndFrame());
            mpBridgeBtk->setFrame(mpBridgeBtk->getEndFrame());
            field_0x5c8 = 1;
            mpPortalBrk->setFrame(0.0f);

            setAction(ACTION_DEAD2_e);
        }

        mEventType = 1;
        break;
    }

    if (getNameArg() == 0) {
        JUTNameTab* jnt_nametab = mpBridgeModel->getModelData()->getJointTree().getJointName();
        mEffJointNo = 0xFFFF;

        for (u16 i = 0; i < mpBridgeModel->getModelData()->getJointNum(); i++) {
            if (strcmp(jnt_nametab->getName(i), "effect_p") == 0) {
                mEffJointNo = i;
                break;
            }
        }

        #if DEBUG
        if (mEffJointNo == 0xFFFF) {
            // "Kakariko Bridge: joint effect_p not found"
            OSReport_Error("カカリコ橋；ジョイント effect_p が見つかりませんでした\n");
            return 0;
        }
        #endif
    }

    initBaseMtx();
    fopAcM_SetMtx(this, mpBridgeModel->getBaseTRMtx());
    fopAcM_setCullSizeBox2(this, mpBridgeModel->getModelData());

    attention_info.distances[fopAc_attn_TALK_e] = 54;
    attention_info.distances[fopAc_attn_SPEAK_e] = 54;

    eventInfo.setArchiveName(l_evArcName);

    for (int i = 0; i < 2; i++) {
        mEventId[i] = dComIfGp_getEventManager().getEventIdx(this, l_portal_warp_name[i], 0xFF);
    }

    calcMidnaWaitPos();
    MoveBGExecute();
    return 1;
}

int daObjWarpKBrg_c::CreateHeap() {
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes(l_arcName[getNameArg()], l_bmd_idx[getNameArg()]);
    JUT_ASSERT(380, modelData != NULL);

    u32 sp2C = 0x11000284;
    mpBridgeModel = mDoExt_J3DModel__create(modelData, 0x80000, sp2C);
    if (mpBridgeModel == NULL) {
        return 0;
    }

    J3DAnmTextureSRTKey* pbtk = (J3DAnmTextureSRTKey*)dComIfG_getObjectRes(l_arcName[getNameArg()], l_btk_idx[getNameArg()]);
    JUT_ASSERT(394, pbtk != NULL);
    mpBridgeBtk = new mDoExt_btkAnm();
    if (mpBridgeBtk == NULL || !mpBridgeBtk->init(modelData, pbtk, TRUE, 0, 1.0f, 0, -1)) {
        return 0;
    }
    mpBridgeBtk->setFrame(0.0f);

    J3DAnmTransform* pbck = (J3DAnmTransform*)dComIfG_getObjectRes(l_arcName[getNameArg()], l_bck_idx[getNameArg()]);
    JUT_ASSERT(407, pbck != NULL);
    mpBridgeBck = new mDoExt_bckAnm();
    if (mpBridgeBck == NULL || !mpBridgeBck->init(pbck, TRUE, 0, 1.0f, 0, -1, false)) {
        return 0;
    }

    if (getNameArg() == 1) {
        modelData = (J3DModelData*)dComIfG_getObjectRes(l_arcName[getNameArg()], 10);
        JUT_ASSERT(424, modelData != NULL);
        mpPortalModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000284);
        if (mpPortalModel == NULL) {
            return 0;
        }

        pbtk = (J3DAnmTextureSRTKey*)dComIfG_getObjectRes(l_arcName[getNameArg()], 18);
        JUT_ASSERT(438, pbtk != NULL);
        mpPortalBtk = new mDoExt_btkAnm();
        if (mpPortalBtk == NULL || !mpPortalBtk->init(modelData, pbtk, TRUE, 2, 1.0f, 0, -1)) {
            return 0;
        }
        mpPortalBtk->setFrame(0.0f);

        J3DAnmTevRegKey* pbrk = (J3DAnmTevRegKey*)dComIfG_getObjectRes(l_arcName[getNameArg()], 14);
        JUT_ASSERT(451, pbrk != NULL);
        mpPortalBrk = new mDoExt_brkAnm();
        if (mpPortalBrk == NULL || !mpPortalBrk->init(modelData, pbrk, TRUE, 0, 1.0f, 0, -1)) {
            return 0;
        }

        #if VERSION == VERSION_SHIELD_DEBUG
        pbrk = (J3DAnmTevRegKey*)dComIfG_getObjectRes(l_arcName[getNameArg()], 15);
        JUT_ASSERT(463, pbrk != NULL);
        #endif
    }

    return 1;
}

int daObjWarpKBrg_c::create1st() {
    if (!mPrmInit) {
        mMsgID = home.angle.z;
        mPrmInit = TRUE;
        home.angle.z = 0;
        current.angle.z = 0;
        shape_angle.z = 0;
    }

    switch (getNameArg()) {
    case 0:
            /* dSv_event_flag_c::M_018 - Main Event - Brought Kakariko bridge back to original location */
        if (dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[51])) {
            // "Kakariko Bridge: already warped so creation cancelled!"
            OS_REPORT("\x1B[43;30mカカリコ橋：ワープさせたので発生取り消し！\n\x1B[m");
            return cPhs_ERROR_e;
        }
        break;
    case 1:
             /* dSv_event_flag_c::M_018 - Main Event - Brought Kakariko bridge back to original location */
        if (!dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[51])) {
            // "Kakariko Bridge: not warped so creation cancelled!"
            OS_REPORT("\x1B[43;30mカカリコ橋：ワープさせてないので発生取り消し！\n\x1B[m");
            return cPhs_ERROR_e;
        }
        break;
    }

    int phase_state = dComIfG_resLoad(&mEvPhase, l_evArcName);
    if (phase_state != cPhs_COMPLEATE_e) {
        return phase_state;
    }

    phase_state = dComIfG_resLoad(&mPhase, l_arcName[getNameArg()]);
    if (phase_state == cPhs_COMPLEATE_e) {
        phase_state = MoveBGCreate(l_arcName[getNameArg()], l_dzb_idx[getNameArg()], dBgS_MoveBGProc_TypicalRotY, l_heap_size[getNameArg()], NULL);
        if (phase_state == cPhs_ERROR_e) {
            return phase_state;
        }
    }

    return phase_state;
}

int daObjWarpKBrg_c::Execute(Mtx** param_0) {
    #if VERSION == VERSION_SHIELD_DEBUG
    calcMidnaWaitPos();
    #endif

    if (getNameArg() == 0 && daPy_py_c::getMidnaActor() == NULL) {
        return 1;
    }

    event_proc_call();

    *param_0 = &mBgMtx;
    setBaseMtx();
    return 1;
}

void daObjWarpKBrg_c::event_proc_call() {
    static void (daObjWarpKBrg_c::*l_func[])() = {
        &daObjWarpKBrg_c::actionWait,
        &daObjWarpKBrg_c::actionOrderEvent,
        &daObjWarpKBrg_c::actionTalkEvent,
        &daObjWarpKBrg_c::actionWaitWarpEvent,
        &daObjWarpKBrg_c::actionWarpEvent,
        &daObjWarpKBrg_c::actionOrderATalkEvent,
        &daObjWarpKBrg_c::actionDead,
        &daObjWarpKBrg_c::actionWait2,
        &daObjWarpKBrg_c::actionOrderEvent2,
        &daObjWarpKBrg_c::actionWarpEvent2,
        &daObjWarpKBrg_c::actionDead2,
    };

    attention_info.flags = 0;

    if (field_0x69b != 0) {
        field_0x69b--;
    }

    (this->*l_func[mAction])();

    if (!eventInfo.chkCondition(dEvtCnd_CANTALK_e)) {
        field_0x69a = 0;
    }
}

void daObjWarpKBrg_c::orderZHintEvent() {
    daMidna_c* midna_p = (daMidna_c*)daPy_py_c::getMidnaActor();
    daPy_py_c* player_p = daPy_getPlayerActorClass();

    if (checkTalkDistance()) {
        eventInfo.onCondition(dEvtCnd_CANTALK_e);
        if (!player_p->checkPlayerFly()) {
            dComIfGp_att_ZHintRequest(this,  0x1FF);

            if (field_0x69a == 0) {
                field_0x69a = 1;
                if (field_0x69b == 0) {
                    mDoAud_seStart(Z2SE_NAVI_CALLVOICE, NULL, 0, 0);
                }
                field_0x69b = 60;
            }
        }
    }
}

void daObjWarpKBrg_c::actionWait() {
    daMidna_c* midna_p = (daMidna_c*)daPy_py_c::getMidnaActor();
    daPy_py_c* player_p = daPy_getPlayerActorClass();

    if (fopAcM_isSwitch(this, getSwbit2())
           /* dSv_event_flag_c::M_021 - Main Event - First portal warp */
        && dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[54])) {
        setAction(ACTION_ORDER_EVENT_e);
        actionOrderEvent();
    }

    if (dComIfGp_evmng_startCheck(mEventId[mEventType])) {
        if (strcmp(dComIfGp_getEventManager().getRunEventName(), "PORTAL_WARP_KBRIDGE") == 0) {
            midna_p->onTagWaitPosPortalObj(&mMidnaWaitPos);
            dComIfGp_getEvent().setPt2(this);
            mStaffId = dComIfGp_evmng_getMyStaffId(l_staff_name, NULL, 0);
            setAction(ACTION_WARP_EVENT_e);
            demoProc();
        }
    }
}

void daObjWarpKBrg_c::actionOrderEvent() {
    daMidna_c* midna_p = (daMidna_c*)daPy_py_c::getMidnaActor();

    if (eventInfo.checkCommandTalk()) {
        midna_p->onTagWaitPosPortalObj(&mMidnaWaitPos);
        if (midna_p->current.pos.abs(mMidnaWaitPos) < 5.0f) {
            setAction(ACTION_TALK_EVENT_e);
            mMsgFlow.init(this, getMsgID(), 0, NULL);
        }
    } else {
        orderZHintEvent();
    }
}

void daObjWarpKBrg_c::actionWaitWarpEvent() {
    daMidna_c* midna_p = (daMidna_c*)daPy_py_c::getMidnaActor();

    if (dComIfGp_evmng_startCheck(mEventId[mEventType])) {
        midna_p->onTagWaitPosPortalObj(&mMidnaWaitPos);
        dComIfGp_getEvent().setPt2(this);
        mStaffId = dComIfGp_evmng_getMyStaffId(l_staff_name, NULL, 0);
        setAction(ACTION_WARP_EVENT_e);
        demoProc();
    }
}

void daObjWarpKBrg_c::actionOrderATalkEvent() {
    daMidna_c* midna_p = (daMidna_c*)daPy_py_c::getMidnaActor();

    if (eventInfo.checkCommandTalk()) {
        setAction(ACTION_TALK_EVENT_e);
        mMsgFlow.init(this, getMsgID(), 0, NULL);
    } else if (dComIfGp_evmng_startCheck(mEventId[mEventType])) {
        midna_p->onTagWaitPosPortalObj(&mMidnaWaitPos);
        dComIfGp_getEvent().setPt2(this);
        mStaffId = dComIfGp_evmng_getMyStaffId(l_staff_name, NULL, 0);
        setAction(ACTION_WARP_EVENT_e);
        demoProc();
    } else if (checkTalkDistance()) {
        midna_p->onTagWaitPosPortalObj(&mMidnaWaitPos);
        dComIfGp_TargetWarpPt_set(3);
        eventInfo.onCondition(dEvtCnd_CANTALK_e);
        attention_info.flags = (fopAc_AttnFlag_SPEAK_e | fopAc_AttnFlag_TALK_e);
    } else {
        midna_p->offTagWaitPos();
        setAction(ACTION_WAIT_e);
    }
}

void daObjWarpKBrg_c::actionTalkEvent() {
    if (mMsgFlow.doFlow(this, NULL, 0)) {
        dComIfGp_event_reset();
        field_0x5d9 = 1;

        int sp8;
        if (mMsgFlow.getEventId(&sp8) == 11) {
            dMeter2Info_setPauseStatus(6);
        }

        setAction(ACTION_ORDER_A_TALK_EVENT_e);
        // "Kakariko Bridge: talk end!"
        OS_REPORT_ERROR("カカリコ橋：会話終了！\n");
    }
}

void daObjWarpKBrg_c::actionWarpEvent() {
    if (!dComIfGp_evmng_endCheck(mEventId[mEventType])) {
        dComIfGp_getEvent().setSkipProc(this, eventCallBack, 0);
        demoProc();
    }
}

void daObjWarpKBrg_c::actionDead() {}

void daObjWarpKBrg_c::actionWait2() {}

void daObjWarpKBrg_c::actionOrderEvent2() {
    if (eventInfo.checkCommandDemoAccrpt()) {
        setAction(ACTION_WARP_EVENT2_e);
        mStaffId = dComIfGp_evmng_getMyStaffId(l_staff_name, NULL, 0);
        demoProc();
    } else {
        fopAcM_orderOtherEventId(this, mEventId[mEventType], 0xFF, 0xFFFF, 2, 1);
    }
}

void daObjWarpKBrg_c::actionWarpEvent2() {
    if (dComIfGp_evmng_endCheck(mEventId[mEventType])) {
        setAction(ACTION_DEAD2_e);
    } else {
        demoProc();
    }
}

void daObjWarpKBrg_c::actionDead2() {}

void daObjWarpKBrg_c::demoProc() {
    static char* action_table[7] = {
        "WAIT",
        "FLY",
        "DOWN",
        "P_APP",
        "P_DIS",
        "B_APP",
        "BEAM",
    };

    daPy_py_c* player_p = daPy_getPlayerActorClass();
    daMidna_c* midna_p = (daMidna_c*)daPy_py_c::getMidnaActor();
    int act_idx = dComIfGp_evmng_getMyActIdx(mStaffId, action_table, 7, 0, 0);

    if (dComIfGp_evmng_getIsAddvance(mStaffId)) {
        switch (act_idx) {
        case 0: {
            int* timer_p = dComIfGp_evmng_getMyIntegerP(mStaffId, "Timer");
            if (timer_p == NULL) {
                mTimer = 1;
            } else {
                mTimer = *timer_p;
            }
            break;
        }
        case 1:
            mpBridgeBck->setPlaySpeed(1.0f);
            mpBridgeBtk->setPlaySpeed(1.0f);

            shape_angle.setall(0);
            field_0x5c8 = 1;
            setSrcEffect();
            fopAcM_seStart(this, Z2SE_OBJ_KKRK_BRG_LIFT_UP, 0);

            if (mpBgW != NULL && mpBgW->ChkUsed()) {
                dComIfG_Bgsp().Release(mpBgW);
            }

            if (dComIfGp_TransportWarp_check()) {
                /* dSv_event_flag_c::M_018 - Main Event - Brought Kakariko bridge back to original location */
                dComIfGs_onEventBit(dSv_event_flag_c::saveBitLabels[51]);
                // "Kakariko Bridge: Warp destination is correct"
                OS_REPORT("\x1B[43;30mカカリコ橋：ワープ先正解です\n\x1B[m");
            }
            break;
        case 2:
            mpBridgeBtk->setPlaySpeed(1.0f);
            mpBridgeBck->setPlaySpeed(1.0f);

            if (mpBgW != NULL) {
                dComIfG_Bgsp().Regist(mpBgW, this);
            }

            mDoAud_seStart(Z2SE_OBJ_KKRK_BRG_LIFT_DW, &mObjPos, 0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
            player_p->onPlayerNoDraw();
            player_p->onPlayerShadowNoDraw();
            break;
        case 3:
            mpPortalBrk->setFrame(mpPortalBrk->getEndFrame());
            break;
        case 4:
            mpPortalBrk->init((J3DModelData*)dComIfG_getObjectRes(l_arcName[getNameArg()], 10), (J3DAnmTevRegKey*)dComIfG_getObjectRes(l_arcName[getNameArg()], 15), 0, 0, 1.0f, 0, -1);
            mpPortalBrk->setFrame(0.0f);
            mpPortalBrk->setPlaySpeed(1.0f);
            mpPortalBtk->setPlaySpeed(1.0f);
            break;
        case 5:
            field_0x5c8 = 1;
            mTimer = 35;
            setDstEffect();
            calcObjPos();
            mDoAud_seStart(Z2SE_OBJ_KKRK_BRG_WARP_OUT, &mObjPos, 0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
            break;
        case 6:
            setBindEffect();
            fopAcM_seStart(this, Z2SE_MDN_WARP_MAGIC, 0);
            break;
        }
    }

    cXyz effpos;
    cXyz sp28;
    csXyz effrot;
    switch (act_idx) {
    case 0:
        if (cLib_calcTimer<u8>(&mTimer) == 0) {
            dComIfGp_evmng_cutEnd(mStaffId);
        }
        break;
    case 1: {
        int temp_r25 = mpBridgeBtk->play();
        temp_r25 &= mpBridgeBck->play();

        if (240.0f == mpBridgeBck->getFrame()) {
            calcObjPos();
            mDoAud_seStart(Z2SE_OBJ_KKRK_BRG_WARP_IN, &mObjPos, 0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
            // "Kakariko Bridge: Warp SE"
            OS_REPORT("カカリコ橋：ワープＳＥ\n");
        }

        if (temp_r25 != 0) {
            dComIfGp_evmng_cutEnd(mStaffId);
        }

        followSrcEffect();
        break;
    }
    case 2:
        mpBridgeBtk->play();

        if (180.0f == mpBridgeBck->getFrame()) {
            dComIfGp_getVibration().StartShock(8, 0x1F, cXyz(0.0f, 1.0f, 0.0f));
        }

        if (mpBridgeBck->play()) {
            player_p->offPlayerNoDraw();
            player_p->offPlayerShadowNoDraw();
            fopAcM_onSwitch(this, getSwbit());
            dComIfGp_evmng_cutEnd(mStaffId);
        }

        followDstEffect();
        calcObjPos();
        break;
    case 4:
        if (mpPortalBrk->play()) {
            dComIfGp_evmng_cutEnd(mStaffId);
        }
        endDstEffect();
        break;
    case 5:
        if (cLib_calcTimer<u8>(&mTimer) == 0) {
            dComIfGp_evmng_cutEnd(mStaffId);
        }
        followDstEffect();
        break;
    case 6: {
        mDoMtx_stack_c::copy(midna_p->getMtxHairTop());
        mDoMtx_stack_c::transM(-10.0f, -65.0f, 55.0f);
        mDoMtx_stack_c::multVecZero(&sp28);

        mDoMtx_stack_c::copy(mpBridgeModel->getAnmMtx(0));
        mDoMtx_stack_c::transM(0.0f, -180.0f, 0.0f);
        mDoMtx_stack_c::multVecZero(&effpos);

        effrot.y = cLib_targetAngleY(&effpos, &sp28);
        effrot.x = cLib_targetAngleX(&sp28, &effpos);
        effrot.z = 0;

        f32 h_scale = 0.01f * effpos.abs(sp28);

        JPABaseEmitter* emitter_p = dComIfGp_particle_set(0x8B53, &effpos, &effrot, NULL, 0xFF, NULL, -1, NULL, NULL, NULL);
        if (emitter_p != NULL) {
            emitter_p->setGlobalParticleHeightScale(h_scale);
        }

        followBindEffect();
        dComIfGp_evmng_cutEnd(mStaffId);
        break;
    }
    default:
        dComIfGp_evmng_cutEnd(mStaffId);
        break;
    }

    if (mpPortalBtk != NULL) {
        mpPortalBtk->play();
    }
}

void daObjWarpKBrg_c::calcObjPos() {
    J3DAnmTransform* pbck = (J3DAnmTransform*)dComIfG_getObjectRes(l_arcName[getNameArg()], l_bck_idx[getNameArg()]);
    JUT_ASSERT(1191, pbck != NULL);

    J3DTransformInfo transinfo;
    pbck->getTransform(0, &transinfo);
    mObjPos.x = transinfo.mTranslate.x;
    mObjPos.y = transinfo.mTranslate.y;
    mObjPos.z = transinfo.mTranslate.z;
}

bool daObjWarpKBrg_c::checkTalkDistance() {
    daPy_py_c* player = daPy_getPlayerActorClass();
    f32 player_dist = fopAcM_searchPlayerDistanceXZ(this);

    if ((getArg0() != 0xFF && player_dist < (10.0f * getArg0())) || (getArg0() == 0xFF && player_dist < 500.0f)) {
        return true;
    } else {
        return false;
    }
}

void daObjWarpKBrg_c::calcMidnaWaitPos() {
    cXyz offset(l_midna_wait_offset);
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::ZXYrotM(shape_angle);
    mDoMtx_stack_c::multVec(&offset, &mMidnaWaitPos);

    attention_info.position = mMidnaWaitPos;
    attention_info.position.y += 250.0f;
}

void daObjWarpKBrg_c::setBindEffect() {
    for (int i = 0; i < 4; i++) {
        mBindEfEmitterID[i] = dComIfGp_particle_set(l_bind_eff[i], &current.pos, &home.angle, &scale, 0xFF, NULL, -1, NULL, NULL, NULL);
        if (mBindEfEmitterID[i] != NULL) {
            mBindEfEmitterID[i]->becomeImmortalEmitter();
        }
    }
}

void daObjWarpKBrg_c::followBindEffect() {
    for (int i = 0; i < 4; i++) {
        if (mBindEfEmitterID[i] != NULL) {
            mBindEfEmitterID[i]->setGlobalRTMatrix(mpBridgeModel->getAnmMtx(0));
        }
    }
}

void daObjWarpKBrg_c::setSrcEffect() {
    for (int i = 0; i < 5; i++) {
        mDisappEfEmitterID[i] = dComIfGp_particle_set(l_disapp_eff[i], &current.pos, &home.angle, &scale, 0xFF, NULL, -1, NULL, NULL, NULL);
        if (mDisappEfEmitterID[i] != NULL) {
            mDisappEfEmitterID[i]->becomeImmortalEmitter();
        }
    }

    for (int i = 0; i < 3; i++) {
        mStartEfEmitterID[i] = dComIfGp_particle_set(l_start_eff[i], &current.pos, &home.angle, &scale, 0xFF, NULL, -1, NULL, NULL, NULL);
        if (mStartEfEmitterID[i] != NULL) {
            mStartEfEmitterID[i]->becomeImmortalEmitter();
        }
    }
}

void daObjWarpKBrg_c::setDstEffect() {
    for (int i = 0; i < 6; i++) {
        mAppEfEmitterID[i] = dComIfGp_particle_set(l_app_eff[i], &current.pos, &home.angle, &scale, 0xFF, NULL, -1, NULL, NULL, NULL);
        if (mAppEfEmitterID[i] != NULL) {
            mAppEfEmitterID[i]->becomeImmortalEmitter();
        }
    }

    for (int i = 0; i < 2; i++) {
        mEndEfEmitterID[i] = dComIfGp_particle_set(l_end_eff[i], &current.pos, &home.angle, &scale, 0xFF, NULL, -1, NULL, NULL, NULL);
        if (mEndEfEmitterID[i] != NULL) {
            mEndEfEmitterID[i]->becomeImmortalEmitter();
        }
    }
}

void daObjWarpKBrg_c::followSrcEffect() {
    for (int i = 0; i < 5; i++) {
        if (mDisappEfEmitterID[i] != NULL) {
            mDisappEfEmitterID[i]->setGlobalRTMatrix(mpBridgeModel->getAnmMtx(mEffJointNo));
        }
    }

    for (int i = 0; i < 3; i++) {
        if (mStartEfEmitterID[i] != NULL) {
            mStartEfEmitterID[i]->setGlobalRTMatrix(mpBridgeModel->getAnmMtx(0));
        }
    }
}

void daObjWarpKBrg_c::followDstEffect() {
    for (int i = 0; i < 6; i++) {
        if (mAppEfEmitterID[i] != NULL) {
            mAppEfEmitterID[i]->setGlobalRTMatrix(mpBridgeModel->getAnmMtx(0));
        }
    }

    for (int i = 0; i < 2; i++) {
        if (mEndEfEmitterID[i] != NULL) {
            mEndEfEmitterID[i]->setGlobalRTMatrix(mpBridgeModel->getAnmMtx(0));
        }
    }
}

void daObjWarpKBrg_c::endSrcEffect() {
    for (int i = 0; i < 5; i++) {
        if (mDisappEfEmitterID[i] != NULL) {
            mDisappEfEmitterID[i]->becomeInvalidEmitter();
            mDisappEfEmitterID[i]->quitImmortalEmitter();
            mDisappEfEmitterID[i] = NULL;
        }
    }

    for (int i = 0; i < 3; i++) {
        if (mStartEfEmitterID[i] != NULL) {
            mStartEfEmitterID[i]->becomeInvalidEmitter();
            mStartEfEmitterID[i]->quitImmortalEmitter();
            mStartEfEmitterID[i] = NULL;
        }
    }
}

void daObjWarpKBrg_c::endDstEffect() {
    for (int i = 0; i < 6; i++) {
        if (mAppEfEmitterID[i] != NULL) {
            mAppEfEmitterID[i]->becomeInvalidEmitter();
            mAppEfEmitterID[i]->quitImmortalEmitter();
            mAppEfEmitterID[i] = NULL;
        }
    }

    for (int i = 0; i < 2; i++) {
        if (mEndEfEmitterID[i] != NULL) {
            mEndEfEmitterID[i]->becomeInvalidEmitter();
            mEndEfEmitterID[i]->quitImmortalEmitter();
            mEndEfEmitterID[i] = NULL;
        }
    }
}

int daObjWarpKBrg_c::Draw() {
    if (getNameArg() == 1) {
        g_env_light.settingTevStruct(32, &current.pos, &tevStr);
    } else {
        g_env_light.settingTevStruct(16, &current.pos, &tevStr);
    }

    g_env_light.setLightTevColorType_MAJI(mpBridgeModel, &tevStr);
    dComIfGd_setListBG();

    if (field_0x5c8 && mpBridgeBck != NULL) {
        mpBridgeBck->entry(mpBridgeModel->getModelData());
    }

    if (mpBridgeBtk != NULL) {
        mpBridgeBtk->entry(mpBridgeModel->getModelData());
    }

    mDoExt_modelUpdateDL(mpBridgeModel);
    mDoExt_bckAnmRemove(mpBridgeModel->getModelData());
    dComIfGd_setList();

    if (mpPortalModel != NULL) {
        dComIfGd_setListDark();
        mpPortalBtk->entry(mpPortalModel->getModelData());
        mpPortalBrk->entry(mpPortalModel->getModelData());
        mDoExt_modelUpdateDL(mpPortalModel);
        mDoExt_brkAnmRemove(mpBridgeModel->getModelData());
        mDoExt_btkAnmRemove(mpBridgeModel->getModelData());
        dComIfGd_setList();
    }

    return 1;
}

int daObjWarpKBrg_c::Delete() {
    endSrcEffect();
    endDstEffect();

    dComIfG_resDelete(&mPhase, l_arcName[getNameArg()]);
    dComIfG_resDelete(&mEvPhase, l_evArcName);
    return 1;
}

static int daObjWarpKBrg_create1st(daObjWarpKBrg_c* i_this) {
    fopAcM_ct(i_this, daObjWarpKBrg_c);
    return i_this->create1st();
}

static int daObjWarpKBrg_MoveBGDelete(daObjWarpKBrg_c* i_this) {
    return i_this->MoveBGDelete();
}

static int daObjWarpKBrg_MoveBGExecute(daObjWarpKBrg_c* i_this) {
    return i_this->MoveBGExecute();
}

static int daObjWarpKBrg_MoveBGDraw(daObjWarpKBrg_c* i_this) {
    return i_this->MoveBGDraw();
}

static actor_method_class daObjWarpKBrg_METHODS = {
    (process_method_func)daObjWarpKBrg_create1st,
    (process_method_func)daObjWarpKBrg_MoveBGDelete,
    (process_method_func)daObjWarpKBrg_MoveBGExecute,
    (process_method_func)NULL,
    (process_method_func)daObjWarpKBrg_MoveBGDraw,
};

extern actor_process_profile_definition g_profile_Obj_KakarikoBrg = {
    fpcLy_CURRENT_e,         // mLayerID
    3,                       // mListID
    fpcPi_CURRENT_e,         // mListPrio
    PROC_Obj_KakarikoBrg,    // mProcName
    &g_fpcLf_Method.base,   // sub_method
    sizeof(daObjWarpKBrg_c), // mSize
    0,                       // mSizeOther
    0,                       // mParameters
    &g_fopAc_Method.base,    // sub_method
    530,                     // mPriority
    &daObjWarpKBrg_METHODS,  // sub_method
    0x00040000,              // mStatus
    fopAc_ACTOR_e,           // mActorType
    fopAc_CULLBOX_CUSTOM_e,  // cullType
};
