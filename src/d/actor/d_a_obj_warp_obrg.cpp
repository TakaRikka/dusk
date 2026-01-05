/**
 * @file d_a_obj_warp_obrg.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_warp_obrg.h"
#include "d/d_com_inf_game.h"
#include "d/d_meter2_info.h"
#include "d/actor/d_a_player.h"
#include "d/actor/d_a_midna.h"
#include "d/d_bg_w.h"

static char* l_arcName = "Obj_obrg";

static char* l_portal_warp_name[2] = {
    "PORTAL_WARP_OBRIDGE",
    "PORTAL_WARP_OUT_OBRIDGE",
};

static const int l_bmd_idx[] = {12, 11};

static const int l_dzb_idx[] = {20, 19};

static const int l_bck_idx[] = {8, 7};

static const int l_btk_idx[] = {16, 15};

static const u16 l_beam_eff[] = {0x8B4B, 0x8B4C, 0x8B4D, 0x8B4E, 0x8B53};

static const u16 l_disapp_eff[] = {0x8B38, 0x8B39, 0x8B3A, 0x8B3B, 0x8B35};

static const u16 l_app_eff[] = {0x8B34, 0x8B36, 0x8B37};

static const Vec l_midna_wait_offset = {100.0f, -1265.0f, -760.0f};

static char* l_staff_name[2] = {
    "WarpOB1",
    "WarpOB2",
};

void daObjWarpOBrg_c::initBaseMtx() {
    fopAcM_SetMtx(this, mpModel->getBaseTRMtx());
    mpModel->setBaseScale(scale);
    setBaseMtx();
}

void daObjWarpOBrg_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::ZXYrotM(shape_angle);
    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());

    if (getNameArg() == 0) {
        J3DAnmTransform* pbck = (J3DAnmTransform*)dComIfG_getObjectRes(l_arcName, l_bck_idx[getNameArg()]);
        JUT_ASSERT(189, pbck != NULL);

        J3DTransformInfo transinfo;
        pbck->getTransform(0, &transinfo);
        mDoMtx_stack_c::ZXYrotM(transinfo.mRotation.x, transinfo.mRotation.y, transinfo.mRotation.z);
    }

    MTXCopy(mDoMtx_stack_c::get(), mBgMtx);
}

int daObjWarpOBrg_c::Create() {
    initBaseMtx();
    fopAcM_setCullSizeBox2(this, mpModel->getModelData());

    attention_info.distances[fopAc_attn_SPEAK_e] = 54;
    attention_info.distances[fopAc_attn_TALK_e] = 54;

    eventInfo.setArchiveName(l_arcName);

    mEventId = dComIfGp_getEventManager().getEventIdx(this, l_portal_warp_name[getNameArg()], 0xFF);

    calcMidnaWaitPos();

    if (getNameArg() == 0) {
        setAction(ACTION_WAIT_e);
    } else if (fopAcM_isSwitch(this, getSwNo())) {
        setAction(ACTION_DEAD_e);
    } else {
        setAction(ACTION_ORDER_EVENT_DST_e);
    }

    MoveBGExecute();
    return 1;
}

int daObjWarpOBrg_c::CreateHeap() {
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes(l_arcName, l_bmd_idx[getNameArg()]);
    JUT_ASSERT(254, modelData != NULL);

    mpModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000284);
    if (mpModel == NULL) {
        return 0;
    }

    J3DAnmTransform* pbck = (J3DAnmTransform*)dComIfG_getObjectRes(l_arcName, l_bck_idx[getNameArg()]);
    JUT_ASSERT(263, pbck != NULL);
    mpBck = new mDoExt_bckAnm();
    if (mpBck == NULL || !mpBck->init(pbck, TRUE, 0, 0.0f, 0, -1, false)) {
        return 0;
    }
    mpBck->setPlaySpeed(0.0f);

    if (fopAcM_isSwitch(this, getSwNo())) {
        mpBck->setFrame(mpBck->getEndFrame());
    }

    J3DAnmTextureSRTKey* pbtk = (J3DAnmTextureSRTKey*)dComIfG_getObjectRes(l_arcName, l_btk_idx[getNameArg()]);
    JUT_ASSERT(271, pbtk != NULL);
    mpBtk = new mDoExt_btkAnm();
    if (mpBtk == NULL || !mpBtk->init(modelData, pbtk, TRUE, 0, 0.0f, 0, -1)) {
        return 0;
    }
    mpBtk->setPlaySpeed(0.0f);

    if (fopAcM_isSwitch(this, getSwNo())) {
        mpBtk->setFrame(mpBtk->getEndFrame());
    }

    return 1;
}

int daObjWarpOBrg_c::create1st() {
    if (!mPrmInit) {
        mMsgID = home.angle.z;
        mPrmInit = TRUE;
        home.angle.z = 0;
        current.angle.z = 0;
        shape_angle.z = 0;
    }

    switch (getNameArg()) {
    case 0:
            /* dSv_event_flag_c::M_092 - Hyrule Field - Warped Eldin Bridge */
        if (dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[125])) {
            // "Eldin Bridge: already warped so creation cancelled!"
            OS_REPORT("\x1B[43;30mオルディン大橋：ワープさせたので発生取り消し！\n\x1B[m");
            return cPhs_ERROR_e;
        }
        break;
    case 1:
             /* dSv_event_flag_c::M_092 - Hyrule Field - Warped Eldin Bridge */
        if (!dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[125])) {
            // "Eldin Bridge: not warped so creation cancelled!"
            OS_REPORT("\x1B[43;30mオルディン大橋：ワープさせてないので発生取り消し！\n\x1B[m");
            return cPhs_ERROR_e;
        }
        break;
    }

    int phase_state = dComIfG_resLoad(&mPhase, l_arcName);
    if (phase_state == cPhs_COMPLEATE_e) {
        u32 heap_size;
        if (getNameArg() != 0) {
            heap_size = 0x3B40;
        } else {
            heap_size = 0x21A0;
        }

        phase_state = MoveBGCreate(l_arcName, l_dzb_idx[getNameArg()], dBgS_MoveBGProc_TypicalRotY, heap_size, NULL);
        if (phase_state == cPhs_ERROR_e) {
            return phase_state;
        }

        if (getArg0() != 0xFF) {
            mTalkRange = getArg0() * 10.0f;
        } else {
            mTalkRange = 550.0f;
        }
    }

    return phase_state;
}

int daObjWarpOBrg_c::Execute(Mtx** param_0) {
    #if DEBUG
    calcMidnaWaitPos();
    #endif

    event_proc_call();

    *param_0 = &mBgMtx;
    return 1;
}

void daObjWarpOBrg_c::event_proc_call() {
    static void (daObjWarpOBrg_c::*l_func[])() = {
        &daObjWarpOBrg_c::actionWait,
        &daObjWarpOBrg_c::actionOrderEvent,
        &daObjWarpOBrg_c::actionTalkEvent,
        &daObjWarpOBrg_c::actionWarpEvent,
        &daObjWarpOBrg_c::actionDead,
        &daObjWarpOBrg_c::actionOrderEventDst,
        &daObjWarpOBrg_c::actionWarpEventDst,
    };

    attention_info.flags = 0;

    if (field_0x63a != 0) {
        field_0x63a--;
    }

    (this->*l_func[mAction])();

    if (!eventInfo.chkCondition(dEvtCnd_CANTALK_e)) {
        field_0x639 = 0;
    }
}

void daObjWarpOBrg_c::actionWait() {
    daMidna_c* midna_p = daPy_py_c::getMidnaActor();

    if (midna_p != NULL) {
        if (dComIfGp_event_runCheck() && eventInfo.checkCommandTalk()) {
            midna_p->onTagWaitPosPortalObj(&mMidnaWaitPos);
            if (midna_p->current.pos.abs(mMidnaWaitPos) < 5.0f) {
                dComIfGp_TargetWarpPt_set(getMyPortalID());
                mMsgFlow.init(this, getMsgID(), 0, NULL);
                setAction(ACTION_TALK_EVENT_e);
            }
        } else if (fopAcM_isSwitch(this, getSwNo2()) && checkTalkDistance()) {
            eventInfo.onCondition(dEvtCnd_CANTALK_e);
            if (!daPy_getPlayerActorClass()->checkPlayerFly()) {
                dComIfGp_att_ZHintRequest(this, 0x1FF);

                if (field_0x639 == 0) {
                    field_0x639 = 1;
                    if (field_0x63a == 0) {
                        mDoAud_seStart(Z2SE_NAVI_CALLVOICE, NULL, 0, 0);
                    }
                }

                field_0x63a = 60;
            }
        }

        if (dComIfGp_evmng_startCheck(mEventId)) {
            if (strcmp(dComIfGp_getEventManager().getRunEventName(), "PORTAL_WARP_OBRIDGE") == 0) {
                mStaffId = dComIfGp_evmng_getMyStaffId(l_staff_name[getNameArg()], NULL, 0);
                setAction(ACTION_WARP_EVENT_e);
                dComIfGp_getEvent()->startCheckSkipEdge(this);
                midna_p->offTagWaitPos();
                dComIfGp_getEvent()->setPt2(this);
            }
        }
    }
}

void daObjWarpOBrg_c::actionOrderEvent() {
    daMidna_c* midna_p = (daMidna_c*)daPy_py_c::getMidnaActor();

    if (eventInfo.checkCommandTalk()) {
        midna_p->onTagWaitPosPortalObj(&mMidnaWaitPos);
        if (midna_p->current.pos.abs(mMidnaWaitPos) < 5.0f) {
            dComIfGp_TargetWarpPt_set(getMyPortalID());
            mMsgFlow.init(this, getMsgID(), 0, NULL);
            setAction(ACTION_TALK_EVENT_e);
        }
    } else if (dComIfGp_evmng_startCheck(mEventId)) {
        mStaffId = dComIfGp_evmng_getMyStaffId(l_staff_name[getNameArg()], NULL, 0);
        setAction(ACTION_WARP_EVENT_e);
        dComIfGp_getEvent()->startCheckSkipEdge(this);
        midna_p->offTagWaitPos();
        dComIfGp_getEvent()->setPt2(this);
    } else {
        eventInfo.onCondition(1);
        if (checkTalkDistance()) {
            attention_info.flags = fopAc_AttnFlag_SPEAK_e | fopAc_AttnFlag_TALK_e;
        } else {
            midna_p->offTagWaitPos();
            setAction(ACTION_WAIT_e);
        }
    }
}

void daObjWarpOBrg_c::actionTalkEvent() {
    if (mMsgFlow.doFlow(this, NULL, 0)) {
        setAction(ACTION_ORDER_EVENT_e);
        dComIfGp_event_reset();

        int sp8;
        if (mMsgFlow.getEventId(&sp8) == 11) {
            dMeter2Info_setPauseStatus(6);
        }
    }
}

void daObjWarpOBrg_c::actionWarpEvent() {
    if (!dComIfGp_evmng_endCheck(mEventId)) {
        demoProc();

        if (dComIfGp_getEvent()->checkSkipEdge()) {
            /* dSv_event_tmp_flag_c::T_0010 - General use - General use temporary flag (flow control) A */
            dComIfGs_offTmpBit(dSv_event_tmp_flag_c::tempBitLabels[11]);
            /* dSv_event_tmp_flag_c::T_0011 - General use - General use temporary flag (flow control) B */
            dComIfGs_offTmpBit(dSv_event_tmp_flag_c::tempBitLabels[12]);
            /* dSv_event_tmp_flag_c::T_0012 - General use - General use temporary flag (flow control) C */
            dComIfGs_offTmpBit(dSv_event_tmp_flag_c::tempBitLabels[13]);
            /* dSv_event_tmp_flag_c::T_0013 - General use - General use temporary flag (flow control) D */
            dComIfGs_offTmpBit(dSv_event_tmp_flag_c::tempBitLabels[14]);
            /* dSv_event_tmp_flag_c::T_0014 - General use - General use temporary flag (flow control) E */
            dComIfGs_offTmpBit(dSv_event_tmp_flag_c::tempBitLabels[15]);
            /* dSv_event_tmp_flag_c::T_0051 - General use - General use temporary flag (flow control) F */
            dComIfGs_offTmpBit(dSv_event_tmp_flag_c::tempBitLabels[51]);
            /* dSv_event_tmp_flag_c::T_0052 - General use - General use temporary flag (flow control) G */
            dComIfGs_offTmpBit(dSv_event_tmp_flag_c::tempBitLabels[52]);
            /* dSv_event_tmp_flag_c::T_0053 - General use - General use temporary flag (flow control) H */
            dComIfGs_offTmpBit(dSv_event_tmp_flag_c::tempBitLabels[53]);
            /* dSv_event_tmp_flag_c::T_0054 - General use - General use temporary flag (flow control) I */
            dComIfGs_offTmpBit(dSv_event_tmp_flag_c::tempBitLabels[54]);
            /* dSv_event_tmp_flag_c::T_0055 - General use - General use temporary flag (flow control) J */
            dComIfGs_offTmpBit(dSv_event_tmp_flag_c::tempBitLabels[55]);

            daPy_getPlayerActorClass()->skipPortalObjWarp();
            if (dComIfGp_TransportWarp_check()) {
                /* dSv_event_flag_c::M_092 - Hyrule Field - Warped Eldin Bridge */
                dComIfGs_onEventBit(dSv_event_flag_c::saveBitLabels[125]);
                // "Eldin Bridge: Warp destination is correct"
                OS_REPORT("\x1B[43;30mオルディン大橋：ワープ先正解です\n\x1B[m");
            }
        }
    }
}

void daObjWarpOBrg_c::actionOrderEventDst() {
    if (eventInfo.checkCommandDemoAccrpt()) {
        mStaffId = dComIfGp_evmng_getMyStaffId(l_staff_name[getNameArg()], NULL, 0);
        fopAcM_onSwitch(this, getSwNo());
        setAction(ACTION_WARP_EVENT_DST_e);
    } else {
        fopAcM_orderOtherEventId(this, mEventId, 0xFF, 0xFFFF, 2, 1);
    }
}

void daObjWarpOBrg_c::actionWarpEventDst() {
    if (!dComIfGp_evmng_endCheck(mEventId)) {
        demoProc();
    }
}

void daObjWarpOBrg_c::actionDead() {}

void daObjWarpOBrg_c::demoProc() {
    static char* action_table[4] = {
        "WAIT",
        "OBJ_DISAPP",
        "OBJ_APP",
        "BEAM",
    };

    daPy_py_c* player_p = daPy_getPlayerActorClass();
    daMidna_c* midna_p = (daMidna_c*)daPy_py_c::getMidnaActor();
    int act_idx = dComIfGp_evmng_getMyActIdx(mStaffId, action_table, 4, 0, 0);

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
            if (mpBgW != NULL && mpBgW->ChkUsed()) {
                dComIfG_Bgsp().Release(mpBgW);
            }

            if (dComIfGp_TransportWarp_check()) {
                /* dSv_event_flag_c::M_092 - Hyrule Field - Warped Eldin Bridge */
                dComIfGs_onEventBit(dSv_event_flag_c::saveBitLabels[125]);
                // "Eldin Bridge: Warp destination is correct"
                OS_REPORT("\x1B[43;30mオルディン大橋：ワープ先正解です\n\x1B[m");
            }

            mpBck->setFrame(0.0f);
            mpBtk->setFrame(0.0f);
            mpBck->setPlaySpeed(1.0f);
            mpBtk->setPlaySpeed(1.0f);

            for (int i = 0; i < 5; i++) {
                mEfEmitter[i] = dComIfGp_particle_set(l_disapp_eff[i], &current.pos, &shape_angle, NULL, 0xFF, NULL, -1, NULL, NULL, NULL);
                if (mEfEmitter[i] != NULL) {
                    mEfEmitter[i]->becomeImmortalEmitter();
                }
            }
            break;
        case 2:
            mTimer = 35;

            mpBck->setFrame(0.0f);
            mpBtk->setFrame(0.0f);
            mpBck->setPlaySpeed(1.0f);
            mpBtk->setPlaySpeed(1.0f);

            for (int i = 0; i < 3; i++) {
                mEfEmitter[i] = dComIfGp_particle_set(l_app_eff[i], &current.pos, &shape_angle, NULL, 0xFF, NULL, -1, NULL, NULL, NULL);
                if (mEfEmitter[i] != NULL) {
                    mEfEmitter[i]->becomeImmortalEmitter();
                }
            }

            Z2GetAudioMgr()->seStart(Z2SE_OBJ_ORDN_BRG_WARP_OUT, &attention_info.position, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
            break;
        case 3:
            mTimer = 60;

            for (int i = 0; i < 5; i++) {
                cXyz effpos(current.pos);
                cXyz sp74;
                csXyz effrot(shape_angle);

                if (i == 4) {
                    sp74.set(-10.0f, -65.0f, 55.0f);
                    mDoMtx_stack_c::copy(midna_p->getMtxHairTop());
                    mDoMtx_stack_c::multVec(&sp74, &sp74);

                    mDoMtx_stack_c::copy(mpModel->getAnmMtx(0));
                    mDoMtx_stack_c::transM(0.0f, -500.0f, 0.0f);
                    mDoMtx_stack_c::multVecZero(&effpos);

                    effrot.set(cLib_targetAngleX(&sp74, &effpos), (s16)cLib_targetAngleY(&effpos, &sp74), 0);
                }

                mEfEmitter[i] = dComIfGp_particle_set(l_beam_eff[i], &effpos, &effrot, NULL, 0xFF, NULL, -1, NULL, NULL, NULL);
                if (mEfEmitter[i] != NULL) {
                    if (i == 4) {
                        mEfEmitter[i]->setGlobalParticleHeightScale(effpos.abs(sp74) * 0.01f);
                    } else {
                        mEfEmitter[i]->setGlobalRTMatrix(mpModel->getAnmMtx(0));
                    }
                }
            }

            Z2GetAudioMgr()->seStart(Z2SE_MDN_WARP_MAGIC, &attention_info.position, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
            break;
        }
    }

    int sp40 = 0;
    switch (act_idx) {
    case 0:
    case 3:
        if (cLib_calcTimer<u8>(&mTimer) == 0) {
            dComIfGp_evmng_cutEnd(mStaffId);
        }
        break;
    case 1:
        sp40 = mpBck->play();
        sp40 &= mpBtk->play();

        for (int i = 0; i < 5; i++) {
            int jnt_no = i == 4 ? 1 : 0;
            if (mEfEmitter[i] != NULL) {
                mEfEmitter[i]->setGlobalRTMatrix(mpModel->getAnmMtx(jnt_no));
            }
        }

        if (mpBck->checkFrame(40.0f)) {
            Z2GetAudioMgr()->seStart(0x80244, &attention_info.position, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
        } else if (mpBck->checkFrame(367.0f)) {
            Z2GetAudioMgr()->seStart(0x80245, &attention_info.position, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
        }

        if (sp40 != 0) {
            for (int i = 0; i < 5; i++) {
                if (mEfEmitter[i] != NULL) {
                    mEfEmitter[i]->becomeInvalidEmitter();
                    mEfEmitter[i]->quitImmortalEmitter();
                    mEfEmitter[i] = NULL;
                }
            }

            dComIfGp_evmng_cutEnd(mStaffId);
        }
        break;
    case 2: {
        if (cLib_calcTimer<u8>(&mTimer) == 0) {
            sp40 = mpBck->play();
            sp40 &= mpBtk->play();
        }

        J3DAnmTransform* pbck = (J3DAnmTransform*)dComIfG_getObjectRes(l_arcName, l_bck_idx[getNameArg()]);
        JUT_ASSERT(740, pbck != NULL);

        J3DTransformInfo transinfo;
        pbck->getTransform(0, &transinfo);

        attention_info.position.set(current.pos.x + transinfo.mTranslate.x,
                                    current.pos.y + transinfo.mTranslate.y,
                                    current.pos.z + transinfo.mTranslate.z);

        if (mpBck->checkFrame(120.0f)) {
            Z2GetAudioMgr()->seStart(0x80247, &attention_info.position, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
        } else if (mpBck->checkFrame(247.0f)) {
            dComIfGp_getVibration().StartShock(8, 0xF, cXyz(0.0f, 1.0f, 0.0f));
        }

        for (int i = 0; i < 3; i++) {
            if (mEfEmitter[i] != NULL) {
                mEfEmitter[i]->setGlobalRTMatrix(mpModel->getAnmMtx(0));
            }
        }

        if (sp40 != 0) {
            for (int i = 0; i < 3; i++) {
                if (mEfEmitter[i] != NULL) {
                    mEfEmitter[i]->becomeInvalidEmitter();
                    mEfEmitter[i]->quitImmortalEmitter();
                    mEfEmitter[i] = NULL;
                }
            }

            dComIfGp_evmng_cutEnd(mStaffId);
        }
        break;
    }
    default:
        dComIfGp_evmng_cutEnd(mStaffId);
        break;
    }
}

bool daObjWarpOBrg_c::checkTalkDistance() {
    daPy_py_c* player = daPy_getPlayerActorClass();
    f32 player_dist = mMidnaWaitPos.absXZ(*fopAcM_GetPosition_p(daPy_getPlayerActorClass()));

    mTalkRange = 550.0f;

    if (player_dist < mTalkRange) {
        return true;
    } else {
        return false;
    }
}

void daObjWarpOBrg_c::calcMidnaWaitPos() {
    cXyz offset(l_midna_wait_offset);
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::ZXYrotM(shape_angle);
    mDoMtx_stack_c::multVec(&offset, &mMidnaWaitPos);

    attention_info.position = mMidnaWaitPos;
    attention_info.position.y += 250.0f;
}

int daObjWarpOBrg_c::Draw() {
    if (getNameArg() == 1) {
        g_env_light.settingTevStruct(32, &current.pos, &tevStr);
    } else {
        g_env_light.settingTevStruct(16, &current.pos, &tevStr);
    }

    g_env_light.setLightTevColorType_MAJI(mpModel, &tevStr);
    dComIfGd_setListBG();

    mpBck->entry(mpModel->getModelData());
    mpBtk->entry(mpModel->getModelData());

    mDoExt_modelUpdateDL(mpModel);
    dComIfGd_setList();
    return 1;
}

int daObjWarpOBrg_c::Delete() {
    dComIfG_resDelete(&mPhase, l_arcName);
    return 1;
}

static int daObjWarpOBrg_create1st(daObjWarpOBrg_c* i_this) {
    fopAcM_ct(i_this, daObjWarpOBrg_c);
    return i_this->create1st();
}

static int daObjWarpOBrg_MoveBGDelete(daObjWarpOBrg_c* i_this) {
    return i_this->MoveBGDelete();
}

static int daObjWarpOBrg_MoveBGExecute(daObjWarpOBrg_c* i_this) {
    return i_this->MoveBGExecute();
}

static int daObjWarpOBrg_MoveBGDraw(daObjWarpOBrg_c* i_this) {
    return i_this->MoveBGDraw();
}

static actor_method_class daObjWarpOBrg_METHODS = {
    (process_method_func)daObjWarpOBrg_create1st,
    (process_method_func)daObjWarpOBrg_MoveBGDelete,
    (process_method_func)daObjWarpOBrg_MoveBGExecute,
    (process_method_func)NULL,
    (process_method_func)daObjWarpOBrg_MoveBGDraw,
};

actor_process_profile_definition g_profile_Obj_OrdinBrg = {
    fpcLy_CURRENT_e,         // mLayerID
    3,                       // mListID
    fpcPi_CURRENT_e,         // mListPrio
    PROC_Obj_OrdinBrg,       // mProcName
    &g_fpcLf_Method.base,   // sub_method
    sizeof(daObjWarpOBrg_c), // mSize
    0,                       // mSizeOther
    0,                       // mParameters
    &g_fopAc_Method.base,    // sub_method
    531,                     // mPriority
    &daObjWarpOBrg_METHODS,  // sub_method
    0x00040000,              // mStatus
    fopAc_ENV_e,             // mActorType
    fopAc_CULLBOX_CUSTOM_e,  // cullType
};
