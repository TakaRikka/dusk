#include "d/actor/d_a_npc_kn.h"
#include "d/d_msg_object.h"
#include "d/d_k_wmark.h"

void daNpc_Kn_c::tgHitCallBack(fopAc_ac_c* i_tgActor, dCcD_GObjInf* i_tgObjInf,
                               fopAc_ac_c* i_atActor, dCcD_GObjInf* i_atObjInf) {
    daNpc_Kn_c* kn_actor = (daNpc_Kn_c*)i_tgActor;
    u8 cut_type = 0;
    if (i_atActor != NULL) {
        if (fopAcM_GetProfName(i_atActor) == PROC_ALINK) {
            cut_type = ((daPy_py_c*)i_atActor)->getCutType();
        } else {
            cut_type = 1;
        }
    }

    kn_actor->setCutType(cut_type);
}

J3DAnmTransform* daNpc_Kn_c::getTrnsfrmAnmP(char const* i_arcName, int i_resIdx) {
    return (J3DAnmTransform*)dComIfG_getObjectRes(i_arcName, i_resIdx);
}

J3DAnmTransformKey* daNpc_Kn_c::getTrnsfrmKeyAnmP(char const* i_arcName, int i_resIdx) {
    return (J3DAnmTransformKey*)dComIfG_getObjectRes(i_arcName, i_resIdx);
}

J3DAnmTexPattern* daNpc_Kn_c::getTexPtrnAnmP(char const* i_arcName, int i_resIdx) {
    return (J3DAnmTexPattern*)dComIfG_getObjectRes(i_arcName, i_resIdx);
}

J3DAnmTextureSRTKey* daNpc_Kn_c::getTexSRTKeyAnmP(char const* i_arcName, int i_resIdx) {
    return (J3DAnmTextureSRTKey*)dComIfG_getObjectRes(i_arcName, i_resIdx);
}

BOOL daNpc_Kn_c::setBckAnm(J3DAnmTransform* i_bck, f32 i_speed, int i_mode, int i_startF,
                           int i_endF, bool i_modify) {
    s16 start_f = i_startF;
    s16 end_f = i_endF;
    return mBckAnm.init(i_bck, TRUE, i_mode, i_speed, start_f, end_f, i_modify);
}

int daNpc_Kn_c::setMcaMorfAnm(J3DAnmTransformKey* i_bck, f32 i_speed, f32 i_morf, int i_mode,
                              int i_start, int i_end) {
    f32 f_start = i_start;
    f32 f_end = i_end;

    mpModelMorf[0]->setAnm(i_bck, i_mode, i_morf, i_speed, f_start, f_end);

    if (mpModelMorf[1] != NULL) {
        mpModelMorf[1]->setAnm(i_bck, i_mode, i_morf, i_speed, f_start, f_end);
    }

    return 1;
}

BOOL daNpc_Kn_c::setBtpAnm(J3DAnmTexPattern* i_btp, J3DModelData* i_modelData, f32 i_speed,
                           int i_mode) {
    return mBtpAnm.init(i_modelData, i_btp, TRUE, i_mode, i_speed, 0, -1);
}

BOOL daNpc_Kn_c::setBtkAnm(J3DAnmTextureSRTKey* i_btk, J3DModelData* i_modelData, f32 i_speed,
                           int i_mode) {
    return mBtkAnm.init(i_modelData, i_btk, TRUE, i_mode, i_speed, 0, -1);
}

int daNpc_Kn_c::loadRes(const s8* i_ptrn_list, const char** i_arcname_list) {
    int phase;
    int resLoad_cnt = 0;

    for (int i = 0; i < 8; ++i) {
        if (i_ptrn_list[i] == -1) {
            if (i != resLoad_cnt) {
                break;
            }

            return cPhs_COMPLEATE_e;
        }

        phase = dComIfG_resLoad(&mPhase[i], i_arcname_list[i_ptrn_list[i]]);
        if (phase == cPhs_COMPLEATE_e) {
            ++resLoad_cnt;
        }

        if (phase == cPhs_ERROR_e || phase == cPhs_UNK3_e) {
            return cPhs_ERROR_e;
        }
    }

    return cPhs_INIT_e;
}

void daNpc_Kn_c::deleteRes(const s8* i_ptrn_list, const char** i_arcname_list) {
    for (int i = 0; i < 8 && i_ptrn_list[i] != -1; i++) {
        dComIfG_resDelete(&mPhase[i], i_arcname_list[i_ptrn_list[i]]);
    }
}

int daNpc_Kn_c::execute() {
    BOOL evt_proc = FALSE;
    setParam();

    evt_proc = evtProc();
    if (evt_proc == FALSE) {
        action();
    }

    beforeMove();

    if (speedF != 0.0f) {
        fopAcM_posMoveF(this, mCcStts.GetCCMoveP());
    } else {
        fopAcM_posMove(this, mCcStts.GetCCMoveP());
    }

    mAcch.CrrPos(dComIfG_Bgsp());
    mGndChk = mAcch.m_gnd;
    mGroundAngle = fopAcM_getPolygonAngle(mGndChk, current.angle.y);
    mGroundHeight = mAcch.GetGroundH();
    afterMoved();

    if (mGroundHeight != -G_CM3D_F_INF) {
        setEnvTevColor();
        setRoomNo();
    }

    if (mFaceMotionSeqMngr.checkEntryNewMotion()) {
        ctrlFaceMotion();
    }

    if (mMotionSeqMngr.checkEntryNewMotion()) {
        ctrlMotion();
    }

    playAllAnm();
    ctrlFaceMotion();
    ctrlMotion();
    setAttnPos();
    twilight();
    setCollision();

    if (field_0xe33 == 0) {
        if ((field_0xe39 != 0 && dComIfGp_getEvent()->isOrderOK()) || dComIfGp_event_runCheck() == FALSE)
        {
            evtOrder();
        }
    }

    clrParam();
    decTmr();
    return 1;
}

int daNpc_Kn_c::draw(BOOL param_0, BOOL i_setEffMtx, f32 param_2, GXColorS10* i_tevcolor, f32 param_4,
                     BOOL i_isDrawGhost, BOOL i_noShadow, BOOL i_simpleShadow) {
    J3DModel* model_p = mpModelMorf[0]->getModel();
    J3DModelData* mdlData_p = model_p->getModelData();
    field_0xe3a = 1;

    if (drawDbgInfo() || checkHide() || mNoDraw) {
        return 1;
    }

    tevStr.TevColor.r = 0;
    tevStr.TevColor.g = 0;
    tevStr.TevColor.b = 0;
    tevStr.TevColor.a = 0;
    if (i_tevcolor != NULL) {
        tevStr.TevColor.r = i_tevcolor->r;
        tevStr.TevColor.g = i_tevcolor->g;
        tevStr.TevColor.b = i_tevcolor->b;
        tevStr.TevColor.a = i_tevcolor->a;
    }

    if (mTimerStart != 0 && mTimer != 0) {
        f32 color_strength = f32(mTimer) / f32(mTimerStart);
        if (cM3d_IsZero(color_strength) == false) {
            tevStr.TevColor.r = 20.0f * color_strength;
        }
    } else if (param_0) {
        tevStr.TevColor.g = 20;
    }

    if (mAnmFlags & ANM_PLAY_BTP) {
        mBtpAnm.entry(mdlData_p);
    }

    if (mAnmFlags & ANM_PLAY_BTK) {
        mBtkAnm.entry(mdlData_p);
    }

    if (mAnmFlags & ANM_PLAY_BRK) {
        mBrkAnm.entry(mdlData_p);
    }

    if (mAnmFlags & ANM_PLAY_BPK) {
        mBpkAnm.entry(mdlData_p);
    }

    if (i_setEffMtx) {
        fopAcM_setEffectMtx(this, mdlData_p);
    }

    if (i_isDrawGhost) {
        drawGhost();
    } else if (mTwilight) {
        g_env_light.settingTevStruct(4, &current.pos, &tevStr);
        g_env_light.setLightTevColorType_MAJI(model_p, &tevStr);

        dComIfGd_setListDark();
        mpModelMorf[0]->entryDL();
        dComIfGd_setList();
    } else {
        g_env_light.settingTevStruct(0, &current.pos, &tevStr);
        g_env_light.setLightTevColorType_MAJI(model_p, &tevStr);
        mpModelMorf[0]->entryDL();
    }

    if (mAnmFlags & ANM_PLAY_BTP) {
        mBtpAnm.remove(mdlData_p);
    }

    if (mAnmFlags & ANM_PLAY_BTK) {
        mBtkAnm.remove(mdlData_p);
    }

    if (mAnmFlags & ANM_PLAY_BRK) {
        mBrkAnm.remove(mdlData_p);
    }

    if (mAnmFlags & ANM_PLAY_BPK) {
        mBpkAnm.remove(mdlData_p);
    }

    if (!i_noShadow) {
        if (!i_simpleShadow) {
            mShadowKey = dComIfGd_setShadow(mShadowKey, 1, model_p, &current.pos, param_2, param_4,
                                            current.pos.y, mGroundHeight, mGndChk, &tevStr, 0, 1.0f,
                                            dDlst_shadowControl_c::getSimpleTex());
        } else {
            dComIfGd_setSimpleShadow(&current.pos, mGroundHeight, param_4, mGndChk, 0, 1.0f,
                                     dDlst_shadowControl_c::getSimpleTex());
        }
    }

    drawOtherMdl();
    return 1;
}

void daNpc_Kn_c::setEnvTevColor() {
    tevStr.YukaCol = dComIfG_Bgsp().GetPolyColor(mGndChk);
    tevStr.room_no = dComIfG_Bgsp().GetRoomId(mGndChk);
}

void daNpc_Kn_c::setRoomNo() {
    s8 room_id = dComIfG_Bgsp().GetRoomId(mGndChk);
    fopAcM_SetRoomNo(this, room_id);
    mCcStts.SetRoomId(room_id);
}

BOOL daNpc_Kn_c::checkEndAnm(f32 i_speed) {
    switch ((u8)mpModelMorf[0]->getPlayMode()) {
    case J3DFrameCtrl::EMode_LOOP:
        return mpModelMorf[0]->isLoop();
    case J3DFrameCtrl::EMode_RESET:
    case J3DFrameCtrl::EMode_NONE:
        return mpModelMorf[0]->isStop() && mpModelMorf[0]->getPlaySpeed() == 0.0f && i_speed != 0.0f;
    case J3DFrameCtrl::EMode_REVERSE:
    case J3DFrameCtrl::EMode_LOOP_REVERSE:
    default:
        return FALSE;
    }
}

BOOL daNpc_Kn_c::checkEndAnm(J3DFrameCtrl* i_frameCtrl, f32 i_speed) {
    switch (i_frameCtrl->getAttribute()) {
    case J3DFrameCtrl::EMode_LOOP:
        return i_frameCtrl->checkState(2);
    case J3DFrameCtrl::EMode_RESET:
    case J3DFrameCtrl::EMode_NONE:
        return i_frameCtrl->checkState(1) && i_frameCtrl->getRate() == 0.0f && i_speed != 0.0f;
    case J3DFrameCtrl::EMode_REVERSE:
    case J3DFrameCtrl::EMode_LOOP_REVERSE:
    default:
        return FALSE;
    }
}

void daNpc_Kn_c::playAllAnm() {
    f32 play_speed;
    if (!(mAnmFlags & ANM_FLAG_4000)) {
        if (mAnmFlags & ANM_PLAY_BCK) {
            play_speed = mBckAnm.getPlaySpeed();
            if (mAnmFlags & ANM_PAUSE_BCK) {
                mBckAnm.setPlaySpeed(0.0f);
            }

            mBckAnm.play();
            if (checkEndAnm(mBckAnm.getFrameCtrl(), play_speed)) {
                ++mBckLoops;
            }

            mBckAnm.setPlaySpeed(play_speed);
        }

        if (mAnmFlags & ANM_PLAY_BTP) {
            play_speed = mBtpAnm.getPlaySpeed();
            if (mAnmFlags & ANM_PAUSE_BTP) {
                mBtpAnm.setPlaySpeed(0.0f);
                if (mAnmFlags & ANM_PAUSE_TIMER_BTP) {
                    mBtpPauseTimer = cLib_getRndValue(90, 90);
                }
            }

            if (cLib_calcTimer(&mBtpPauseTimer)) {
                mBtpAnm.setPlaySpeed(0.0f);
            }

            mBtpAnm.play();
            if (mAnmFlags & ANM_PAUSE_TIMER_BTP) {
                if (checkEndAnm(mBtpAnm.getFrameCtrl(), mBtpAnm.getPlaySpeed())) {
                    mBtpPauseTimer = cLib_getRndValue(90, 90);
                }
            } else {
                mBtpPauseTimer = 0;
            }

            mBtpAnm.setPlaySpeed(play_speed);
        }
    }

    if (mAnmFlags & ANM_PLAY_MORF) {
        play_speed = mpModelMorf[0]->getPlaySpeed();
        if (mAnmFlags & ANM_PAUSE_MORF) {
            mpModelMorf[0]->setPlaySpeed(0.0f);
            mpModelMorf[0]->play(0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
            mpModelMorf[0]->setPlaySpeed(play_speed);

            if (mpModelMorf[1] != NULL) {
                mpModelMorf[1]->setPlaySpeed(0.0f);
                mpModelMorf[1]->play(0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
                mpModelMorf[1]->setPlaySpeed(play_speed);
            }
        } else {
            mpModelMorf[0]->play(0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
            if (mpModelMorf[1] != NULL) {
                mpModelMorf[1]->play(0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
            }

            if (checkEndAnm(play_speed)) {
                ++mMorfLoops;
            }
        }
    }

    if (mAnmFlags & ANM_PLAY_BTK) {
        play_speed = mBtkAnm.getPlaySpeed();
        if (mAnmFlags & ANM_PAUSE_BTK) {
            mBtkAnm.setPlaySpeed(0.0f);
        }

        ctrlBtk();
        mBtkAnm.play();
        mBtkAnm.setPlaySpeed(play_speed);
    }

    if (mAnmFlags & ANM_PLAY_BRK) {
        play_speed = mBrkAnm.getPlaySpeed();
        if (mAnmFlags & ANM_PAUSE_BRK) {
            mBrkAnm.setPlaySpeed(0.0f);
        }

        mBrkAnm.play();
        mBrkAnm.setPlaySpeed(play_speed);
    }

    if (mAnmFlags & ANM_PLAY_BPK) {
        play_speed = mBpkAnm.getPlaySpeed();
        if (mAnmFlags & ANM_PAUSE_BPK) {
            mBpkAnm.setPlaySpeed(0.0f);
        }

        mBpkAnm.play();
        mBpkAnm.setPlaySpeed(play_speed);
    }

    mAnmFlags &= ~(ANM_PAUSE_MORF | ANM_PAUSE_BTK | ANM_PAUSE_BRK | ANM_PAUSE_BPK | ANM_PAUSE_BCK
                    | ANM_PAUSE_BTP | ANM_FLAG_4000);

    if (mPodAnmFlags & ANM_PLAY_MORF) {
        play_speed = mPodBck.getPlaySpeed();
        if (mPodAnmFlags & ANM_PAUSE_MORF) {
            mPodBck.setPlaySpeed(0.0f);
        }

        mPodBck.play();
        mPodBck.setPlaySpeed(play_speed);
    }

    mPodAnmFlags &= ~(ANM_PAUSE_MORF | ANM_PAUSE_BTK | ANM_PAUSE_BRK | ANM_PAUSE_BPK | ANM_PAUSE_BCK
                        | ANM_PAUSE_BTP | ANM_FLAG_4000);
}

int daNpc_Kn_c::ctrlBtk() {
    if (mpMatAnm != NULL) {
        if (field_0xe30 != 0) {
            J3DAnmTextureSRTKey* btk_anm = mBtkAnm.getBtkAnm();
            if (btk_anm) {
                mpMatAnm->setNowOffsetX(cM_ssin(mJntAnm.getEyeAngleY()) * 0.2f);
                mpMatAnm->setNowOffsetY(cM_ssin(mJntAnm.getEyeAngleX()) * 0.2f);

                if (field_0xe31 != 0) {
                    mpMatAnm->setMorfFrm(field_0xe31);
                    field_0xe31 = 0;
                }

                mpMatAnm->onEyeMoveFlag();
                return 1;
            }
        }

        if (field_0xe31 != 0) {
            mpMatAnm->setMorfFrm(field_0xe31);
            field_0xe31 = 0;
        }

        mpMatAnm->offEyeMoveFlag();
    }

    return 0;
}

void daNpc_Kn_c::setMtx() {
    J3DModel* model_p = mpModelMorf[0]->getModel();
    J3DModelData* mdlData_p = model_p->getModelData();

    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::ZXYrotM(mCurAngle);
    mDoMtx_stack_c::scaleM(scale);
    model_p->setBaseTRMtx(mDoMtx_stack_c::get());
    model_p->setUserArea((uintptr_t)this);

    if (mAnmFlags & ANM_PLAY_BCK) {
        mBckAnm.getBckAnm()->setFrame(mBckAnm.getFrame());
    }

    mpModelMorf[0]->modelCalc();
}

void daNpc_Kn_c::ctrlFaceMotion() {
    int motionNo;
    f32 morfFrm;

    if (mFaceMotionSeqMngr.play(mBckLoops, &motionNo, &morfFrm)) {
        setFaceMotionAnm(motionNo, true);

        if (morfFrm < 0.0f) {
            mpModelMorf[0]->setMorf(mExpressionMorfFrame);
            if (mpModelMorf[1] != NULL) {
                mpModelMorf[1]->setMorf(mExpressionMorfFrame);
            }
        } else {
            mpModelMorf[0]->setMorf(morfFrm);
            if (mpModelMorf[1] != NULL) {
                mpModelMorf[1]->setMorf(morfFrm);
            }
        }
    }
}

void daNpc_Kn_c::ctrlMotion() {
    int motionNo;
    f32 morfFrm;

    if (mMotionSeqMngr.play(mMorfLoops, &motionNo, &morfFrm)) {
        BOOL restart = FALSE;
        if (morfFrm < -1.0f) {
            restart = TRUE;
        }

        setMotionAnm(motionNo, morfFrm < 0.0f ? mMorfFrames : morfFrm, restart);

        if (mCreating) {
            mpModelMorf[0]->setMorf(0.0f);

            if (mpModelMorf[1]) {
                mpModelMorf[1]->setMorf(0.0f);
            }
        }
    }
}

int daNpc_Kn_c::ctrlMsgAnm(int* o_faceAnmAttr_p, int* o_anmAttr_p, fopAc_ac_c* i_talkPartner_p, int param_3) {
    *o_faceAnmAttr_p = -1;
    *o_anmAttr_p = -1;

    if (param_3 || eventInfo.checkCommandTalk() || mStaffId != -1) {
        if (dComIfGp_event_getTalkPartner() == i_talkPartner_p) {
            msg_class* msg_p = dMsgObject_c::getActor();
            JUT_ASSERT(1088, NULL != msg_p);

            if (msg_p->mode == 2 || msg_p->mode == 3) {
                mMsgId = fpcM_ERROR_PROCESS_ID_e;
            } else if (msg_p->mode == 6) {
                if (msg_p->msg_idx != mMsgId) {
                    *o_faceAnmAttr_p = dComIfGp_getMesgFaceAnimeAttrInfo();
                    *o_anmAttr_p = dComIfGp_getMesgAnimeAttrInfo();
                    mMsgId = msg_p->msg_idx;
                }

                if (dMsgObject_isMouthCheck()) {
                    mAnmFlags &= ~ANM_FLAG_4000;
                } else {
                    mAnmFlags |= ANM_FLAG_4000;
                }

                field_0xddc = 20;
            } else {
                if (mAnmFlags & ANM_FLAG_4000) {
                    mAnmFlags &= ~ANM_FLAG_4000;
                }

                mMsgId = fpcM_ERROR_PROCESS_ID_e;
            }
        } else {
            mMsgId = fpcM_ERROR_PROCESS_ID_e;
            if (field_0xddc) {
                field_0xddc = 1;
            }
        }
    }

    if (param_3 == 0) {
        cLib_calcTimer(&field_0xddc);
    }

    return field_0xddc;
}

int daNpc_Kn_c::ctrlJoint(J3DJoint* i_joint, J3DModel* i_model) {
    J3DJoint* my_joint = i_joint;
    int joint_no = my_joint->getJntNo();
    cXyz pos;
    csXyz angle;
    mDoMtx_stack_c::copy(i_model->getAnmMtx(joint_no));

    if (joint_no == getBackboneJointNo() || joint_no == getNeckJointNo() || joint_no == getHeadJointNo()) {
        Mtx joint_mtx;
        cMtx_copy(mDoMtx_stack_c::get(), joint_mtx);
        pos.set(joint_mtx[0][3], joint_mtx[1][3], joint_mtx[2][3]);
        joint_mtx[0][3] = joint_mtx[1][3] = joint_mtx[2][3] = 0.0f;

        mDoMtx_stack_c::ZXYrotS(mCurAngle.x, (mCurAngle.y + field_0xDBE.y), mCurAngle.z);

        if (joint_no == getNeckJointNo()) {
            angle.set(mJntAnm.getJntRadX2S(2), mJntAnm.getJntRadY2S(2), 0);
            mDoMtx_stack_c::ZXYrotM(angle);
        }

        if (joint_no == getHeadJointNo()) {
            angle.set(mJntAnm.getJntRadX2S(2), mJntAnm.getJntRadY2S(2), 0);
            mDoMtx_stack_c::ZXYrotM(angle);
        }

        mDoMtx_stack_c::inverse();
        Mtx inverse_mtx;
        cMtx_copy(mDoMtx_stack_c::get(), inverse_mtx);
        mDoMtx_stack_c::transS(pos);
        mDoMtx_stack_c::ZXYrotM(mCurAngle.x, (mCurAngle.y + field_0xDBE.y), mCurAngle.z);

        if (joint_no == getBackboneJointNo()) {
            angle.set(mJntAnm.getJntRadX2S(2), mJntAnm.getJntRadY2S(2), 0);
            mDoMtx_stack_c::ZXYrotM(angle);
        }

        if (joint_no == getNeckJointNo()) {
            angle.set(mJntAnm.getJntRadX2S(1), mJntAnm.getJntRadY2S(1), 0);
            mDoMtx_stack_c::ZXYrotM(angle);
        }

        if (joint_no == getHeadJointNo()) {
            angle.set(mJntAnm.getJntRadX2S(0), mJntAnm.getJntRadY2S(0), 0);
            mDoMtx_stack_c::ZXYrotM(angle);
        }

        mDoMtx_stack_c::concat(inverse_mtx);
        mDoMtx_stack_c::concat(joint_mtx);
    }

    afterJntAnm(joint_no);
    i_model->setAnmMtx(joint_no, mDoMtx_stack_c::get());
    cMtx_copy(mDoMtx_stack_c::get(), J3DSys::mCurrentMtx);

    if ((mAnmFlags & ANM_PLAY_BCK) && (checkChangeJoint(joint_no) || checkRemoveJoint(joint_no))) {
        J3DAnmTransform* bck = mBckAnm.getBckAnm();
        mBckAnm.changeBckOnly(mpModelMorf[0]->getAnm());
        mpModelMorf[0]->changeAnm(bck);
    }

    return 1;
}

int daNpc_Kn_c::evtProc() {
    BOOL ret = FALSE;

    if (dComIfGp_event_runCheck()) {
        if (eventInfo.checkCommandTalk()) {
            if (!checkChangeEvt()) {
                evtTalk();
            }
            ret = TRUE;
        } else if (eventInfo.checkCommandDemoAccrpt() && dComIfGp_getEventManager().endCheck(mEvtId)) {
            if (evtEndProc()) {
                dComIfGp_event_reset();
                mEvtId = -1;
            }
        } else {
            if (strcmp(dComIfGp_getEventManager().getRunEventName(), "DEFAULT_GETITEM") == 0 && mItemPartnerId != fpcM_ERROR_PROCESS_ID_e) {
                dComIfGp_event_setTalkPartner(NULL);
                dComIfGp_event_setItemPartnerId(mItemPartnerId);
                mItemPartnerId = fpcM_ERROR_PROCESS_ID_e;
            }
            ret = evtCutProc();
        }

        int temp_r28 = field_0xddc;
        int face_motion, motion;
        if (ctrlMsgAnm(&face_motion, &motion, this, field_0xe34)) {
            if (field_0xe2d != 0) {
                mFaceMotionSeqMngr.setNo(face_motion, -1.0f, 1, 0);
                mMotionSeqMngr.setNo(motion, -1.0f, 1, 0);
            }
        } else if (temp_r28 != 0 && field_0xe2d != 0) {
            setAfterTalkMotion();
        }
    } else {
        if (mStaffId != -1) {
            mMode = 1;
            mStaffId = -1;
        }
        field_0xddc = 0;
        field_0xe2d = 1;
    }

    return ret;
}

void daNpc_Kn_c::setFootPos() {
    if (getFootLJointNo() >= 0) {
        mOldFootLOffset = mFootLOffset;
        mDoMtx_stack_c::copy(mpModelMorf[0]->getModel()->getAnmMtx(getFootLJointNo()));
        mDoMtx_stack_c::multVecZero(&mFootLPos);
        mFootLOffset = mFootLPos - current.pos;
    }

    if (getFootRJointNo() >= 0) {
        mOldFootROffset = mFootROffset;
        mDoMtx_stack_c::copy(mpModelMorf[0]->getModel()->getAnmMtx(getFootRJointNo()));
        mDoMtx_stack_c::multVecZero(&mFootRPos);
        mFootROffset = mFootRPos - current.pos;
    }

    if (mCreating) {
        mOldFootLOffset = mFootLOffset;
        mOldFootROffset = mFootROffset;
    }
}

void daNpc_Kn_c::setFootPrtcl(cXyz* param_0, f32 param_1) {
    cXyz foot_l_pos(mFootLPos);
    cXyz foot_r_pos(mFootRPos);
    cXyz start;
    cXyz end;

    start = mFootLPos;
    end = start;
    end.y -= param_1;
    mLinChk.Set(&start, &end, this);

    u8 cross_l, foot_l_att;
    u8 cross_r, foot_r_att;
    cross_l = dComIfG_Bgsp().LineCross(&mLinChk);
    if (cross_l) {
        foot_l_pos.y = mLinChk.GetCross().y;
        mFootLPolyAngle = fopAcM_getPolygonAngle(mLinChk, mCurAngle.y);
        foot_l_att = dComIfG_Bgsp().GetPolyAtt0(mLinChk);
    } else {
        mFootLPolyAngle = 0;
    }

    start = mFootRPos;
    end = start;
    end.y -= param_1;
    mLinChk.Set(&start, &end, this);

    cross_r = dComIfG_Bgsp().LineCross(&mLinChk);
    if (cross_r) {
        foot_r_pos.y = mLinChk.GetCross().y;
        mFootRPolyAngle = fopAcM_getPolygonAngle(mLinChk, mCurAngle.y);
        foot_r_att = dComIfG_Bgsp().GetPolyAtt0(mLinChk);
    } else {
        mFootRPolyAngle = 0;
    }

    field_0x9F4.setEffectTwo(&tevStr, &current.pos, 0, 0, NULL,
                             !mFootLOnGround && cross_l == true ? &foot_l_pos : NULL,
                             !mFootROnGround && cross_r == true ? &foot_r_pos : NULL,
                             &mCurAngle, param_0, fopAcM_GetRoomNo(this), 0.0f, speedF);

    if (!mFootLOnGround && cross_l == true) {
        switch (foot_l_att) {
        case 3:
            break;
        case 13:
            dkWmark_c::setFootMark(&foot_l_pos, mCurAngle.y, 3);
            break;
        }
    }

    if (!mFootROnGround && cross_r == true) {
        switch (foot_r_att) {
        case 3:
            break;
        case 13:
            dkWmark_c::setFootMark(&foot_r_pos, mCurAngle.y, 3);
            break;
        }
    }

    mFootLOnGround = cross_l;
    mFootROnGround = cross_r;
}

bool daNpc_Kn_c::checkCullDraw() {
    bool ret = false;
    return ret;
}

void daNpc_Kn_c::twilight() {
    if (mTwilight) {
        attention_info.flags |= fopAc_AttnFlag_UNK_0x400000;
        mNoDraw = FALSE;
        attention_info.flags |= fopAc_AttnFlag_UNK_0x800000;
        setHitodamaPrtcl();
    }
}

void daNpc_Kn_c::evtOrder() {
    if (strlen(mpEventData[mEvtNo].eventName) != 0) {
        if (strlen(mpArcNames[mpEventData[mEvtNo].num])) {
            eventInfo.setArchiveName(mpArcNames[mpEventData[mEvtNo].num]);
            dComIfGp_getEventManager().setObjectArchive(eventInfo.getArchiveName());
        }

        mEvtId = dComIfGp_getEventManager().getEventIdx(this, mpEventData[mEvtNo].eventName, 0xff);
        fopAcM_orderOtherEventId(this, mEvtId, 0xff, 0xffff, 6, 1);

    } else {
        if (attention_info.flags & fopAc_AttnFlag_SPEAK_e || attention_info.flags & fopAc_AttnFlag_TALK_e) {
            eventInfo.onCondition(dEvtCnd_CANTALK_e);

            if (chkXYItems()) {
                eventInfo.onCondition(dEvtCnd_CANTALKITEM_e);
            }

            if (mSpeakEvent) {
                fopAcM_orderSpeakEvent(this, 0, 0);
            }
        }
    }
}

void daNpc_Kn_c::evtChange() {
    if (strlen(mpEventData[mEvtNo].eventName) != 0) {
        if (strlen(mpArcNames[mpEventData[mEvtNo].num])) {
            eventInfo.setArchiveName(mpArcNames[mpEventData[mEvtNo].num]);
            dComIfGp_getEventManager().setObjectArchive(eventInfo.getArchiveName());
        }

        mEvtId = dComIfGp_getEventManager().getEventIdx(this, mpEventData[mEvtNo].eventName, 0xff);
        dComIfGp_getEvent()->reset(this);
        fopAcM_orderChangeEventId(this, mEvtId, 1, 0xffff);
    }
}

void daNpc_Kn_c::clrParam() {
    field_0xDB2 = mCurAngle;
    mCutType = 0;
    memset((void*)&mEvtNo, 0, (intptr_t)(void*)&field_0xe3c - (intptr_t)(void*)&mEvtNo);  // fake match?
    mJntAnm.clrDirectFlag();
}

bool daNpc_Kn_c::setFaceMotionAnm(int i_idx, bool i_modify) {
    J3DAnmTransform* bck = NULL;
    J3DAnmTexPattern* btp = NULL;
    mAnmFlags &= ~0x7C30;
    
    daNpcT_faceMotionAnmData_c anm_data = getFaceMotionAnm(mpFaceMotionAnmData[i_idx]);

    if (anm_data.mBckFileIdx != -1) {
        bck = getTrnsfrmAnmP(mpArcNames[anm_data.mBckArcIdx], anm_data.mBckFileIdx);
    }

    if (bck != NULL) {
        if (setBckAnm(bck, 1.0f, anm_data.mBckAttr, 0, -1, i_modify)) {
            mAnmFlags |= ANM_PAUSE_BCK | ANM_PLAY_BCK;
            mBckLoops = 0;
        } else {
            bck = NULL;
        }
    }

    if (bck == NULL && anm_data.mBckFileIdx != -1) {
        return false;
    }

    if (anm_data.mBtpFileIdx != -1) {
        btp = getTexPtrnAnmP(mpArcNames[anm_data.mBtpArcIdx], anm_data.mBtpFileIdx);
    }

    if (btp != NULL) {
        if (btp == mBtpAnm.getBtpAnm() && anm_data.field_0x18) {
            mAnmFlags |= ANM_PLAY_BTP | ANM_PAUSE_TIMER_BTP;
        } else if (setBtpAnm(btp, mpModelMorf[0]->getModel()->getModelData(), 1.0f, anm_data.mBtpAttr)) {
            mAnmFlags |= ANM_PAUSE_BTP | ANM_PLAY_BTP;
            if (anm_data.field_0x18) {
                mAnmFlags |= ANM_PAUSE_TIMER_BTP;
            }
        } else {
            btp = NULL;
        }
    }

    if (btp == NULL && anm_data.mBtpFileIdx != -1) {
        return false;
    }

    return afterSetFaceMotionAnm(i_idx, anm_data.mBckAttr, 0.0f, anm_data.mBtpAttr);
}

bool daNpc_Kn_c::setMotionAnm(int i_idx, f32 i_morf, BOOL i_restart) {
    J3DAnmTransformKey* bck = NULL;
    J3DAnmTextureSRTKey* btk = NULL;
    BOOL restarted = FALSE;

    mAnmFlags &= ~(ANM_PAUSE_MORF | ANM_PAUSE_BTK | ANM_PAUSE_BRK | ANM_PAUSE_BPK
                    | ANM_PLAY_MORF | ANM_PLAY_BTK | ANM_PLAY_BRK | ANM_PLAY_BPK);

    daNpcT_motionAnmData_c anm_data = getMotionAnm(mpMotionAnmData[i_idx]);

    if (anm_data.mBckFileIdx != -1) {
        bck = getTrnsfrmKeyAnmP(mpArcNames[anm_data.mBckArcIdx], anm_data.mBckFileIdx);
    }

    if (bck != NULL) {
        if (i_restart && bck == mpModelMorf[0]->getAnm()) {
            mAnmFlags |= ANM_PLAY_MORF;
            mMorfLoops = 0;
            restarted = TRUE;
        } else if (setMcaMorfAnm(bck, 1.0f, i_morf, anm_data.mBckAttr, 0, -1)) {
            field_0x15A8 = i_idx;
            mAnmFlags |= ANM_PAUSE_MORF | ANM_PLAY_MORF;
            mMorfLoops = 0;
        } else {
            bck = NULL;
        }
    }

    if (bck == NULL && anm_data.mBckFileIdx != -1) {
        return false;
    }

    field_0xe30 = 0;
    field_0xe31 = 0;

    if (anm_data.mBtkFileIdx != -1) {
        field_0xe30 = anm_data.field_0x18;
        field_0xe31 = anm_data.field_0x1a;
        btk = getTexSRTKeyAnmP(mpArcNames[anm_data.mBtkArcIdx], anm_data.mBtkFileIdx);
    }

    if (btk != NULL) {
        if (restarted) {
            mAnmFlags |= ANM_PLAY_BTK;
        } else if (setBtkAnm(btk, mpModelMorf[0]->getModel()->getModelData(), 1.0f, anm_data.mBtkAttr)) {
            mAnmFlags |= ANM_PAUSE_BTK | ANM_PLAY_BTK;
        } else {
            btk = NULL;
        }
    }

    if (btk == NULL && anm_data.mBtkFileIdx != -1) {
        return false;
    }

    return afterSetMotionAnm(i_idx, anm_data.mBckAttr, i_morf, anm_data.mBtkAttr);
}

void daNpc_Kn_c::setPos(cXyz i_pos) {
    i_pos.y += 500.0f;
    mGndChk.SetPos(&i_pos);
    i_pos.y = dComIfG_Bgsp().GroundCross(&mGndChk);
    JUT_ASSERT(1841, -(1000000000.0f) != i_pos.y);

    current.pos = i_pos;
    old.pos = current.pos;
}

void daNpc_Kn_c::setAngle(s16 i_angle) {
    current.angle.y = i_angle;
    shape_angle.y = current.angle.y;
    mCurAngle.y = current.angle.y;
    field_0xDB2.y = mCurAngle.y;
    old.angle.y = current.angle.y;
}

BOOL daNpc_Kn_c::chkActorInSight(fopAc_ac_c* actor, f32 i_fovY, s16 i_angleY) {
    if (i_fovY < 180.0f) {
        s16 fov_s = cM_deg2s(i_fovY);
        cXyz attn_pos = getAttnPos(actor);
        cSGlobe globe(attention_info.position - attn_pos);
        cSAngle angle_diff(cSAngle(globe.U().Inv()) - i_angleY);
        s16 angle_diff_s = angle_diff.Val();
        if (angle_diff_s < 0) {
            angle_diff_s = -angle_diff_s;
        }

        return (angle_diff_s > fov_s) == FALSE;
    }

    return TRUE;
}

BOOL daNpc_Kn_c::chkPointInArea(cXyz i_pos, cXyz i_center, f32 i_boundXZ, f32 i_boundY1, f32 i_boundY2,
                                s16 i_angle) {
    cXyz bounds, center;
    f32 max_y = i_boundY1;
    f32 min_y = i_boundY2;
    if (max_y < min_y) {
        min_y = i_boundY1;
        max_y = i_boundY2;
    }

    bounds.set(i_boundXZ, fabsf(max_y - min_y), i_boundXZ);
    center = i_center;
    center.y += min_y;
    return chkPointInArea(i_pos, center, bounds, i_angle);
}

BOOL daNpc_Kn_c::chkPointInArea(cXyz i_pos, cXyz i_center, cXyz i_bounds, s16 i_angle) {
    return daNpcT_chkPointInArea(i_pos, i_center, i_bounds, i_angle, TRUE);
}

int daNpc_Kn_c::chkFindPlayer(BOOL i_release, s16 i_angleY) {
    cXyz attn_pos = getAttnPos(daPy_getPlayerActorClass());

    int dist_index = i_release == FALSE ? attention_info.distances[fopAc_attn_SPEAK_e]
                                        : attention_info.distances[fopAc_attn_TALK_e];
    f32 dist_max = i_release == FALSE ? dComIfGp_getAttention()->getDistTable(dist_index).mDistMax :
                                        dComIfGp_getAttention()->getDistTable(dist_index).mDistMaxRelease;
    f32 lower_y = -1.0f * dComIfGp_getAttention()->getDistTable(dist_index).mLowerY;
    f32 upper_y = -1.0f * dComIfGp_getAttention()->getDistTable(dist_index).mUpperY;
    int mAngleSelect = dComIfGp_getAttention()->getDistTable(dist_index).mAngleSelect;

    f32 fov = 180.0f;
    if (mAngleSelect & 8) {
        fov = 30.0f;
    } else if (mAngleSelect & 0x10) {
        fov = 45.0f;
    } else if (mAngleSelect & 0x20) {
        fov = 60.0f;
    } else if (mAngleSelect & 0x40) {
        fov = 90.0f;
    } else if (mAngleSelect & 0x80) {
        fov = 110.0f;
    } else if (mAngleSelect & 0x100) {
        fov = 135.0f;
    }

    // writing like the following may be a fakematch, but it fixes dbg asm:
    int pnt_chk[1];
    pnt_chk[0] = chkPointInArea(attn_pos, attention_info.position, dist_max, lower_y, upper_y, 0);
    if (pnt_chk[0] && chkActorInSight(daPy_getPlayerActorClass(), fov, i_angleY)) {
        return TRUE;
    }

    return FALSE;
}

BOOL daNpc_Kn_c::srchPlayerActor() {
    if (mActorMngr1.getActorP() != NULL) {
        if (!chkFindPlayer(TRUE, mCurAngle.y)) {
            mActorMngr1.remove();
        }
    } else {
        if (chkFindPlayer(FALSE, mCurAngle.y)) {
            mActorMngr1.entry(daPy_getPlayerActorClass());
        }
    }

    return NULL != mActorMngr1.getActorP();
}

cXyz daNpc_Kn_c::getAttnPos(fopAc_ac_c* i_actor) {
    cXyz attn_pos(i_actor->attention_info.position);

    if (fopAcM_GetName(i_actor) == PROC_ALINK) {
        attn_pos.y -= static_cast<daPy_py_c*>(i_actor)->getAttentionOffsetY();
    }

    return attn_pos;
}

BOOL daNpc_Kn_c::turn(s16 i_angle, int i_count, int i_direction) {
    if (mTurnAmount == 0) {
        mTurnCount = fabsf(cM_sht2d((s16)(i_angle - mStartAngle)));
        mTurnCount = (mTurnCount / 180.0f) * i_count;
        mTurnCount = (int)mTurnCount + 1;
        if (mTurnCount < 8.0f) {
            mTurnCount = 8.0f;
        }
    }

    s16 sVar1 = mStartAngle;
    int o_arg = i_angle;
    if (i_direction == 0) {
        i_direction = ((s16)(i_angle - mStartAngle) >= 0) ? 1 : -1;
    }

    int angle_diff = (u16)(o_arg - sVar1);
    if (i_direction == -1) {
        angle_diff = -(u16)(0xFFFF - angle_diff);
    }

    int offset = angle_diff * cM_ssin((s16)mTurnAmount);
    mTurnAmount += (s16)(16384.0f / mTurnCount);

    if ((u16)mTurnAmount < 0x4000) {
        mCurAngle.y = mStartAngle + offset;
    } else {
        mCurAngle.y = i_angle;
        mTurnAmount = 0x4000;
    }

    return i_angle == mCurAngle.y;
}

int daNpc_Kn_c::step(s16 i_angle, int i_faceMotion, int i_motion, int i_turnCount,
                     int i_turnDirection) {
    if (mStepMode >= 2 && i_angle != mCurAngle.y) {
        mStepMode = 0;
    }

    if (mStepMode == 0) {
        s16 angle_diff = i_angle - mCurAngle.y;
        if (angle_diff == 0) {
            mStepMode = 2;
        } else {
            mTargetAngle = i_angle;
            mStartAngle = mCurAngle.y;
            mTurnAmount = 0;
            if ((int)fabsf(cM_sht2d(angle_diff)) >= 20) {
                mFaceMotionSeqMngr.setNo(i_faceMotion, -1.0f, TRUE, 0);
                mMotionSeqMngr.setNo(i_motion, -1.0f, TRUE, 0);
            }
            current.angle.y = mCurAngle.y;
            shape_angle.y = current.angle.y;
            mStepMode++;
        }
    } else if (mStepMode == 1) {
        if (turn(mTargetAngle, i_turnCount, i_turnDirection)) {
            current.angle.y = mCurAngle.y;
            shape_angle.y = current.angle.y;
            field_0xDB2.y = current.angle.y;
            mStepMode++;
        } else {
            current.angle.y = mCurAngle.y;
            shape_angle.y = current.angle.y;
        }
    }

    return mStepMode > 1;
}

int daNpc_Kn_c::initTalk(int i_flowID, fopAc_ac_c** i_partnerList_p) {
    mFlow.init(this, i_flowID, 0, i_partnerList_p);
    speedF = 0.0f;
    speed.setall(0.0f);
    return 1;
}

BOOL daNpc_Kn_c::talkProc(int* param_0, int param_1, fopAc_ac_c** i_partnerList_p, int param_3) {
    BOOL ret = FALSE;

    if (dMsgObject_isMsgSendControl()) {
        if (param_1) {
            dMsgObject_offMsgSendControl();
        }
    } else if (mFlow.doFlow(this, i_partnerList_p, 0)) {
        ret = TRUE;
    } else {
        if (mFlow.getMsg() != NULL) {
            switch (mFlow.getMsg()->mode) {
            case 2:
            case 6:
                if (param_0 != NULL) {
                    for (int i = 0; param_0[i] > 0; i++) {
                        if (param_0[i] == mFlow.getMsgNo()) {
                            ret = TRUE;
                            break;
                        }
                    }
                }
                break;
            case 17:
                if (param_3) {
                    ret = TRUE;
                }
                break;
            }
        }

        int tmp1, face_motion, motion, tmp2;
        if (mFlow.checkEventRender(&tmp1, &motion, &face_motion, &tmp2)) {
            mFaceMotionSeqMngr.setNo(face_motion, -1.0f, FALSE, 0);
            mMotionSeqMngr.setNo(motion, -1.0f, FALSE, 0);
        }
    }

    return ret;
}

// FIXME: dummy functions to get rodata to align.
#if !DEBUG
static f64 dummy_lits_00(int sel) {
    switch (sel) {
    case 0:
        return 0.5;
    case 1:
        return 3.0;
    default:
        return 0.0;
    }
}
#endif

static f32 dummy_lits_01(int sel) {
    switch (sel) {
    case 0:
        return 0.85f;
    case 1:
        return 6.0f;
    default:
        return G_CM3D_F_INF;
    }
}

void daNpc_Kn_c::setHitodamaPrtcl() {
    JPABaseEmitter* emitter = NULL;
    cXyz pos(eyePos);

    for (int i = 0; i < 2; i++) {
        static const u16 id[2] = {0x8497,0x8498};
        mHitodamaPrtclKey[i] = dComIfGp_particle_set(mHitodamaPrtclKey[i], id[i], &pos, &mCurAngle, 0);

        emitter = dComIfGp_particle_getEmitter(mHitodamaPrtclKey[i]);
        if (emitter != NULL) {
            u8 alpha = (dComIfGs_wolfeye_effect_check() == FALSE) ? 0xFF : 0;
            emitter->setGlobalTranslation(pos.x, pos.y, pos.z);
            emitter->setGlobalAlpha(alpha);
        }
    }
}

void daNpc_Kn_c::decTmr() {
    if (mTimer != 0) {
        mTimer--;
    }
}

#if DEBUG
void dummy_lit_125958() {
    OS_REPORT("name:%s\n");
    OS_REPORT("JNT_%s_e,\t//  (%2d)%s\n");
}
#endif
