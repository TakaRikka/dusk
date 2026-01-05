/**
 * @file d_a_obj_scannon_crs.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_scannon_crs.h"
#include "d/actor/d_a_player.h"
#include "d/d_meter2_info.h"

static int daSCannonCrs_c_createHeap(fopAc_ac_c* i_this) {
    return ((daSCannonCrs_c*)i_this)->createHeap();
}

static int eventCallBack(void* i_proc, int param_1) {
    if (i_proc == NULL) {
        return 1;
    }

    daPy_py_c* player_p = daPy_getPlayerActorClass();
    if (player_p != NULL) {
        player_p->skipPortalObjWarp();
        dComIfGp_TargetWarpPt_set(((daSCannonCrs_c*)i_proc)->getWarpId());
        
        if (dComIfGp_TransportWarp_check()) {
            dComIfGs_onEventBit(dSv_event_flag_c::saveBitLabels[400]);
            dComIfGs_onSaveSwitch(6, 0x35);
        }
    }

    return 1;
}

static char* l_arcName = "SCanCrs";

static char* l_eventName = "SKY_CANNON_WARP_START";

static char* l_staffName = "SCanCrs";

daSCannonCrs_c::daSCannonCrs_c() {
    mpBgW = NULL;
    memset(mpDisapEmt, 0, sizeof(mpDisapEmt));
    memset(mpBeamEmt, 0, sizeof(mpBeamEmt));
}

daSCannonCrs_c::~daSCannonCrs_c() {
    dComIfG_resDelete(this, l_arcName);
}

static const u16 DISAP_PARTICLE_NAME[] = {
    dPa_RM(ID_ZI_S_SKYCANNON_CRASH_DISAPP_A),
    dPa_RM(ID_ZI_S_SKYCANNON_CRASH_DISAPP_B),
    dPa_RM(ID_ZI_S_SKYCANNON_CRASH_DISAPP_C),
    dPa_RM(ID_ZI_S_SKYCANNON_CRASH_DISAPP_D),
};

static const u16 BEAM_PARTICLE_NAME[] = {
    dPa_RM(ID_ZI_S_SKYCANNON_CRASH_BIND_A),
    dPa_RM(ID_ZI_S_SKYCANNON_CRASH_BIND_B),
    dPa_RM(ID_ZI_S_SKYCANNON_CRASH_BIND_C),
    dPa_RM(ID_ZI_S_SKYCANNON_CRASH_BIND_D),
};

int daSCannonCrs_c::create() {
    fopAcM_ct(this, daSCannonCrs_c);

    if (dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[400])) {
        return cPhs_ERROR_e;
    }

    int phase_state = dComIfG_resLoad(this, l_arcName);
    if (phase_state == cPhs_COMPLEATE_e) {
        if (!fopAcM_entrySolidHeap(this, daSCannonCrs_c_createHeap, 0x9000)) {
            return cPhs_ERROR_e;
        }

        if (dComIfG_Bgsp().Regist(mpBgW, this)) {
            return cPhs_ERROR_e;
        }

        init();

        setModelMtx();
        fopAcM_SetMtx(this, mpModel->getBaseTRMtx());
        fopAcM_setCullSizeBox(this,
                              -500.0f, -100.0f, -500.0f,
                              500.0f, 2500.0f, 500.0f);

        attention_info.distances[1] = 54;
        attention_info.distances[3] = 54;

        eventInfo.setArchiveName(l_arcName);
        mEvtIdx = dComIfGp_getEventManager().getEventIdx(this, l_eventName, 0xFF);

        JUTNameTab* nametab_p = mpModel->getModelData()->getJointTree().getJointName();
        for (int i = 0; i < mpModel->getModelData()->getJointNum(); i++) {
            if (strcmp(nametab_p->getName(i), "effect_p") == 0) {
                mEffectJointNo = i;
            } else if (strcmp(nametab_p->getName(i), "center") == 0) {
                mCenterJointNo = i;
            }
        }
    }

    return phase_state;
}

int daSCannonCrs_c::Delete() {
    if (mpBgW != NULL && mpBgW->ChkUsed()) {
        dComIfG_Bgsp().Release(mpBgW);
    }

    this->~daSCannonCrs_c();
    return 1;
}

int daSCannonCrs_c::draw() {
    g_env_light.settingTevStruct(16, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpModel, &tevStr);

    dComIfGd_setListBG();
    mBck.entry(mpModel->getModelData());
    mBtk.entry(mpModel->getModelData());
    mDoExt_modelUpdateDL(mpModel);

    mBtk.remove(mpModel->getModelData());
    dComIfGd_setList();
    return 1;
}

int daSCannonCrs_c::execute() {
    daMidna_c* midna_p = daPy_py_c::getMidnaActor();
    if (midna_p == NULL) {
        return 1;
    }

    middleExe(midna_p);
    return 1;
}

void daSCannonCrs_c::middleExe(daMidna_c* i_midna_p) {
    if (mVoiceTimer != 0) {
        mVoiceTimer--;
    }

    if (s_exeProc[mMode] != NULL) {
        (this->*s_exeProc[mMode])(i_midna_p);
    }

    if (!eventInfo.chkCondition(dEvtCnd_CANTALK_e)) {
        field_0x651 = 0;
    }

    setModelMtx();
    mBck.play();
    mBtk.play();
}

void daSCannonCrs_c::exeModeWait(daMidna_c* i_midna_p) {
    if (dComIfGp_evmng_startCheck(mEvtIdx) != 0) {
        if (strcmp(dComIfGp_getEventManager().getRunEventName(), "SKY_CANNON_WARP_START") == 0) {
            dComIfGp_getEvent()->setPt2(this);
            
            daPy_py_c* player_p = (daPy_py_c*)dComIfGp_getPlayer(0);
            if (player_p != NULL) {
                cXyz pos(4750.0f, -1520.0f, -3900.0f);
                player_p->setPlayerPosAndAngle(&pos, 0x4000, 0);
            }

            mStaffId = dComIfGp_evmng_getMyStaffId(l_staffName, NULL, 0);
            mMode = MODE_WARP_EVT;
            return;
        }
    }

    if (chkInTalkRange()) {
        daPy_py_c* player_p = (daPy_py_c*)daPy_getLinkPlayerActorClass();
        if (player_p != NULL && !player_p->checkPlayerFly() && field_0x651 == 0) {
            field_0x651 = 1;
            if (mVoiceTimer == 0) {
                mDoAud_seStart(Z2SE_NAVI_CALLVOICE, NULL, 0, 0);
            }
            mVoiceTimer = 60;
        }

        dComIfGp_TargetWarpPt_set(getWarpId());
        mMode = MODE_ORDER_EVT;
    } else {
        attention_info.flags = 0;
        field_0x652 = 0;
        i_midna_p->offTagWaitPos();
    }
}

void daSCannonCrs_c::exeModePreWait(daMidna_c* i_midna_p) {
    if (eventInfo.checkCommandTalk()) {
        dComIfGp_event_reset();
    }

    mMode = MODE_WAIT;
}

void daSCannonCrs_c::exeModeOrderEvt(daMidna_c* i_midna_p) {
    if (eventInfo.checkCommandTalk()) {
        i_midna_p->onTagWaitPosPortalObj(&mPortalWaitPos);
        if (i_midna_p->current.pos.abs(mPortalWaitPos) < 5.0f) {
            mMode = MODE_TALK_EVT;
            mMsgFlow.init(this, getMsgId(), 0, NULL);
            dComIfGp_TargetWarpPt_set(getWarpId());
        }
    } else {
        if (dComIfGp_evmng_startCheck(mEvtIdx) != 0) {
            dComIfGp_getEvent()->setPt2(this);

            daPy_py_c* player_p = (daPy_py_c*)dComIfGp_getPlayer(0);
            if (player_p != NULL) {
                cXyz pos(4750.0f, -1520.0f, -3900.0f);
                player_p->setPlayerPosAndAngle(&pos, 0x4000, 0);
            }

            mStaffId = dComIfGp_evmng_getMyStaffId(l_staffName, NULL, 0);
            mMode = MODE_WARP_EVT;
        } else {
            eventInfo.onCondition(dEvtCnd_CANTALK_e);
            
            daPy_py_c* player_p = daPy_getLinkPlayerActorClass();
            if (player_p != NULL && !player_p->checkPlayerFly()) {
                if (field_0x652 != 0) {
                    attention_info.flags = (fopAc_AttnFlag_SPEAK_e | fopAc_AttnFlag_TALK_e);
                } else {
                    dComIfGp_att_ZHintRequest(this, 0x1FF);
                }
            }

            if (!chkInTalkRange()) {
                attention_info.flags = 0;
                field_0x652 = 0;
                mMode = MODE_PRE_WAIT;
            }
        }
    }
}

void daSCannonCrs_c::exeModeTalkEvt(daMidna_c* i_midna_p) {
    if (mMsgFlow.doFlow(this, NULL, 0)) {
        field_0x652 = 1;
        mMode = MODE_ORDER_EVT;
        dComIfGp_event_reset();

        int spC;
        if (mMsgFlow.getEventId(&spC) == 11) {
            dMeter2Info_setPauseStatus(6);
        }
    }
}

void daSCannonCrs_c::exeModeWarpEvt(daMidna_c* i_midna_p) {
    if (dComIfGp_evmng_endCheck(mEvtIdx) != 0) {
        mMode = MODE_END;
    } else {
        dComIfGp_getEvent()->setSkipProc(this, eventCallBack, 0);
        demoExe();
    }
}

void daSCannonCrs_c::exeModeEnd(daMidna_c* i_midna_p) {}

void (daSCannonCrs_c::*daSCannonCrs_c::s_exeProc[])(daMidna_c*) = {
    &daSCannonCrs_c::exeModeWait,
    &daSCannonCrs_c::exeModePreWait,
    &daSCannonCrs_c::exeModeOrderEvt,
    &daSCannonCrs_c::exeModeTalkEvt,
    &daSCannonCrs_c::exeModeWarpEvt,
    &daSCannonCrs_c::exeModeEnd,
};

void daSCannonCrs_c::demoExe() {
    static char* CUT_TYPE_TABLE[4] = {
        "WAIT",
        "BEAM_HIT",
        "READY",
        "UP",
    };

    int act_idx = dComIfGp_evmng_getMyActIdx(mStaffId, CUT_TYPE_TABLE, ARRAY_SIZE(CUT_TYPE_TABLE), 0, 0);
    if (act_idx != -1) {
        if (dComIfGp_evmng_getIsAddvance(mStaffId)) {
            switch (act_idx) {
            case 0:
                mBck.setPlaySpeed(0.0f);
                mBtk.setPlaySpeed(0.0f);
                break;
            case 1:
                mBck.setPlaySpeed(1.0f);
                mBtk.setPlaySpeed(1.0f);
                fopAcM_seStartCurrent(this, Z2SE_OBJ_TCANNON_LIFT_UP, 0);
                fopAcM_seStartCurrent(this, Z2SE_MDN_WARP_MAGIC, 0);
                callMidnaBeamEmt();
                initEmtRt(mCenterJointNo, 4, BEAM_PARTICLE_NAME, mpBeamEmt);
                break;
            case 2:
                mBck.setPlaySpeed(1.0f);
                mBtk.setPlaySpeed(1.0f);
                dComIfGp_getVibration().StartQuake(2, 0x1F, cXyz(0.0f, 1.0f, 0.0f));
                break;
            case 3:
                mBck.setPlaySpeed(1.0f);
                mBtk.setPlaySpeed(1.0f);
                mWarpQuakeTimer = 0;
                dComIfGp_getVibration().StartShock(VIBMODE_S_POWER3, 0x1F, cXyz(0.0f, 1.0f, 0.0f));
                dComIfGp_getVibration().StartQuake(2, 0x1F, cXyz(0.0f, 1.0f, 0.0f));

                dComIfGp_TargetWarpPt_set(getWarpId());
                if (dComIfGp_TransportWarp_check()) {
                    dComIfGs_onEventBit(dSv_event_flag_c::saveBitLabels[400]);
                    dComIfGs_onSaveSwitch(6, 0x35);
                }
                break;
            }
        }

        switch (act_idx) {
        case 0:
            dComIfGp_evmng_cutEnd(mStaffId);
            return;
        case 1:
            if (mBck.checkFrame(35.0f)) {
                dComIfGp_evmng_cutEnd(mStaffId);
            }
            exeEmtRt(mCenterJointNo, 4, mpBeamEmt);
            return;
        case 2:
            if (mBck.checkFrame(88.0f)) {
                dComIfGp_getVibration().StopQuake(0x1F);
                dComIfGp_evmng_cutEnd(mStaffId);
                return;
            }
            break;
        case 3:
            mWarpQuakeTimer++;
            if (mWarpQuakeTimer == 50) {
                dComIfGp_getVibration().StopQuake(0x1F);
            }

            if (mBck.checkFrame(240.0f)) {
                initEmtRt(mEffectJointNo, 4, DISAP_PARTICLE_NAME, mpDisapEmt);
                fopAcM_seStartCurrent(this, Z2SE_OBJ_TCANNON_WARP_IN, 0);
            } else if (mBck.getFrame() > 240.0f) {
                exeEmtRt(mEffectJointNo, 4, mpDisapEmt);
            }

            if (mBck.isStop()) {
                dComIfGp_evmng_cutEnd(mStaffId);
            }
            break;
        }
    }
}

void daSCannonCrs_c::init() {
    static const Vec MIDNA_WAIT_POS_OFFSET = {100.0f, 900.0f, 90.0f};
    cXyz wait_offset(MIDNA_WAIT_POS_OFFSET);

    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::ZXYrotM(shape_angle);
    mDoMtx_stack_c::multVec(&wait_offset, &mPortalWaitPos);
    attention_info.position = mPortalWaitPos;
    attention_info.position.y += 200.0f;

    f32 talk_range = 10.0f * getTalkRange();
    mTalkRangeSqDist = SQUARE(talk_range);
    mMode = MODE_WAIT;
    mWarpQuakeTimer = 0;
    mEffectJointNo = 0xFFFF;
    mCenterJointNo = 0xFFFF;
    mVoiceTimer = 0;
    field_0x651 = 0;
    field_0x652 = 0;
}

void daSCannonCrs_c::setModelMtx() {
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::YrotM(shape_angle.y);
    mDoMtx_stack_c::XrotM(shape_angle.x);
    mDoMtx_stack_c::ZrotM(shape_angle.z);
    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
}

int daSCannonCrs_c::createHeap() {
    J3DModelData* a_model_data_p = (J3DModelData*)dComIfG_getObjectRes(l_arcName, 10);
    JUT_ASSERT(967, a_model_data_p != NULL);

    mpModel = mDoExt_J3DModel__create(a_model_data_p, J3DMdlFlag_DifferedDLBuffer, J3D_DIFF_FLAG(FALSE, FALSE, TRUE, 8, 2, FALSE, 0, 0, TRUE, FALSE, FALSE, FALSE, TRUE, FALSE));
    if (mpModel == NULL) {
        return 0;
    }

    J3DAnmTransform* a_bck_p = (J3DAnmTransform*)dComIfG_getObjectRes(l_arcName, 7);
    JUT_ASSERT(981, a_bck_p != NULL);
    int rt = mBck.init(a_bck_p, 1, 0, 0.0f, 0, -1, 0);
    if (!rt) {
        return 0;
    }

    J3DAnmTextureSRTKey* a_btk_p = (J3DAnmTextureSRTKey*)dComIfG_getObjectRes(l_arcName, 13);
    JUT_ASSERT(1000, a_btk_p != NULL);
    rt = mBtk.init(a_model_data_p, a_btk_p, 1, 0, 0.0f, 0, -1);
    if (!rt) {
        return 0;
    }

    mpBgW = new dBgW();
    if (mpBgW == NULL) {
        return 0;
    }

    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::YrotM(shape_angle.y);
    mDoMtx_stack_c::XrotM(shape_angle.x);
    mDoMtx_stack_c::ZrotM(shape_angle.z);
    MTXCopy(mDoMtx_stack_c::get(), mBgMtx);

    if (mpBgW->Set((cBgD_t*)dComIfG_getObjectRes(l_arcName, 16), 1, &mBgMtx) == 1) {
        return 0;
    }

    return 1;
}

bool daSCannonCrs_c::chkInTalkRange() {
    fopAc_ac_c* player_p = dComIfGp_getPlayer(0);
    if (player_p == NULL) {
        return false;
    }

    if (player_p->current.pos.abs2(current.pos) < mTalkRangeSqDist) {
        return true;
    }

    return false;
}

void daSCannonCrs_c::initEmtRt(u16 i_jointNo, int i_emtNum, const u16* i_name_list, JPABaseEmitter** i_emt_list) {
    MtxP mtx = mpModel->getAnmMtx(i_jointNo);
    if (mtx != NULL) {
        cXyz pos(mtx[0][3], mtx[1][3], mtx[2][3]);
        csXyz rot(shape_angle);
        cXyz size(1.0f, 1.0f, 1.0f);

        for (int i = 0; i < i_emtNum; i++) {
            i_emt_list[i] = dComIfGp_particle_set(i_name_list[i], &pos, &rot, &size);
            if (i_emt_list[i] != NULL) {
                i_emt_list[i]->setGlobalRTMatrix(mtx);
            }
        }
    }
}

void daSCannonCrs_c::exeEmtRt(u16 i_jointNo, int i_emtNum, JPABaseEmitter** i_emt_list) {
    MtxP mtx = mpModel->getAnmMtx(i_jointNo);
    if (mtx != NULL) {
        for (int i = 0; i < i_emtNum; i++) {
            if (i_emt_list[i] != NULL) {
                i_emt_list[i]->setGlobalRTMatrix(mtx);
            }
        }
    }
}

void daSCannonCrs_c::callMidnaBeamEmt() {
    static const Vec MIDNA_BEAM_OFFSET = {0.0f, 50.0f, 0.0f};

    daMidna_c* midna_p = daPy_py_c::getMidnaActor();
    if (midna_p != NULL) {
        cXyz beam_pos;
        cMtx_multVec(midna_p->getMtxHairTop(), &MIDNA_BEAM_OFFSET, &beam_pos);
        cXyz eff_pos(current.pos.x, 400.0f + current.pos.y, current.pos.z);
        cXyz beam_len = beam_pos - eff_pos;

        csXyz eff_rot;
        eff_rot.y = cM_atan2s(beam_len.x, beam_len.z);
        eff_rot.x = -cM_atan2s(beam_len.y, JMAFastSqrt(SQUARE(beam_len.x) + SQUARE(beam_len.z)));
        eff_rot.z = 0;

        cXyz eff_size(1.0f, 1.0f, 1.0f);
        JPABaseEmitter* emt_p = dComIfGp_particle_set(dPa_RM(ID_ZI_S_OBJWARP_BINDBEAM_A), &eff_pos, &eff_rot, &eff_size);
        if (emt_p != NULL) {
            emt_p->setGlobalParticleHeightScale(0.01f * beam_len.abs());
        }
    }
}

static int daSCannonCrs_create(daSCannonCrs_c* i_this) {
    fopAcM_ct(i_this, daSCannonCrs_c);
    return i_this->create();
}

static int daSCannonCrs_Delete(daSCannonCrs_c* i_this) {
    return i_this->Delete();
}

static int daSCannonCrs_execute(daSCannonCrs_c* i_this) {
    return i_this->execute();
}

static int daSCannonCrs_draw(daSCannonCrs_c* i_this) {
    return i_this->draw();
}

static actor_method_class daSCannonCrs_METHODS = {
    (process_method_func)daSCannonCrs_create,
    (process_method_func)daSCannonCrs_Delete,
    (process_method_func)daSCannonCrs_execute,
    0,
    (process_method_func)daSCannonCrs_draw,
};

actor_process_profile_definition g_profile_Obj_SCannonCrs = {
  fpcLy_CURRENT_e,        // mLayerID
  7,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_Obj_SCannonCrs,    // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(daSCannonCrs_c), // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  736,                    // mPriority
  &daSCannonCrs_METHODS,  // sub_method
  0x00044180,             // mStatus
  fopAc_ENV_e,            // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};
