/**
 * @file d_a_obj_tks.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_tks.h"
#include "d/actor/d_a_npc.h"
#include "d/d_camera.h"
#include "d/d_com_inf_game.h"

static daNpc_GetParam1 l_bckGetParamList[] = {
    {-1, 2},
    {7, 0},
    {11, 2},
    {10, 0},
    {8, 2},
    {9, 0},
};

static daNpc_GetParam1 l_btpGetParamList[] = {
    {20, 2},
};

static daNpc_GetParam1 l_btkGetParamList[] = {
    {17, 2},
};

// unused
static u8 l_evtNames[4] = {
    0x00,
    0x00,
    0x00,
    0x00,
};

static char* l_arcName = "TKS2";

static char* l_myName = "ObjTks";

daObjTks_c::daObjTks_c() {}

daObjTks_c::~daObjTks_c() {
    dComIfG_resDelete(&mPhase, l_arcName);

    if (heap != NULL) {
        mAnm_p->stopZelAnime();
    }

    if (parentActorID != fpcM_ERROR_PROCESS_ID_e) {
        fopAcM_delete(parentActorID);
    }
}

const daObjTks_HIOParam daObjTks_Param_c::m = {
    90.0f,
    -3.0f,
    1.0f,
    150.0f,
    255.0f,
    55.0f,
    20.0f,
    18.0f,
    0.0f,
    0.0f,
    45.0f,
    -45.0f,
    30.0f,
    -10.0f,
    45.0f,
    -45.0f,
    0.8f,
    12.0f,
    3,
    6,
    19,
    6,
    80.0f,
    500.0f,
    300.0f,
    -300.0f,
    60,
    8,
    0,
    0,
    0,
    false,
    false,
    0.0f,
    111.0f,
    50.0f,
    -0.2f,
    0.8f,
};

int daObjTks_c::Create() {
    fopAcM_ct(this, daObjTks_c);

    int phase_state = dComIfG_resLoad(&mPhase, l_arcName);
    if (phase_state == cPhs_COMPLEATE_e) {
        if (!fopAcM_entrySolidHeap(this, createHeapCallBack, 0x2FF0)) {
            return cPhs_ERROR_e;
        }

        J3DModelData* mdlData_p = mAnm_p->getModel()->getModelData();
        fopAcM_SetMtx(this, mAnm_p->getModel()->getBaseTRMtx());
        fopAcM_setCullSizeBox(this, -80.0f, -30.0f, -80.0f, 80.0f, 10.0f, 80.0f);

        mSound.init(&current.pos, &eyePos, 3, 1);

        mAcchCir.SetWall(daObjTks_Param_c::m.common.width, daObjTks_Param_c::m.common.knee_length);
        mAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1, &mAcchCir, fopAcM_GetSpeed_p(this), fopAcM_GetAngle_p(this), fopAcM_GetShapeAngle_p(this));
        mAcch.OffClrSpeedY();
        mAcch.SetWallNone();
    
        mCcStts.Init(daObjTks_Param_c::m.common.weight, 0, this);
        mCcCyl.Set(mCcDCyl);
        mCcCyl.SetStts(&mCcStts);
        mCcCyl.SetTgType(0);
        mCcCyl.SetTgSPrm(0);
        mCcCyl.SetCoIGrp(8);
        mCcCyl.SetCoVsGrp(0x40);

        mCcCyl.SetH(daObjTks_Param_c::m.common.height);
        mCcCyl.SetR(daObjTks_Param_c::m.common.width);

        mAcch.CrrPos(dComIfG_Bgsp());

        mGndChk = mAcch.m_gnd;
        mGroundH = mAcch.GetGroundH();
        setEnvTevColor();
        setRoomNo();
        reset();

        field_0xde0 = 1;
        field_0xde1 = 0;
        Execute();
        field_0xde0 = 0;
        field_0xde1 = 1;

        static const int m_cameraItemNum = 1;
        dCam_getBody()->StartEventCamera(18, fopAcM_GetID(this), "Type", 1, &m_cameraItemNum, 0);
    }

    return phase_state;
}

int daObjTks_c::CreateHeap() {
    J3DModelData* mdlData_p = (J3DModelData*)dComIfG_getObjectRes(l_arcName, 0xE);
    JUT_ASSERT(314, NULL != mdlData_p);

    mAnm_p = new mDoExt_McaMorfSO(mdlData_p, NULL, NULL, NULL, -1, 1.0f, 0, -1, &mSound, 0x80000, 0x11020284);
    if (mAnm_p != NULL && mAnm_p->getModel() == NULL) {
        mAnm_p->stopZelAnime();
        mAnm_p = NULL;
    }

    if (mAnm_p == NULL) {
        return 0;
    }

    J3DModel* mdl_p = mAnm_p->getModel();
    for (u16 i = 0; i < mdlData_p->getJointNum(); i++) {
        mdlData_p->getJointNodePointer(i)->setCallBack(ctrlJointCallBack);
    }

    mdl_p->setUserArea((uintptr_t)this);


    mpMatAnm = new daNpcF_MatAnm_c();
    if (mpMatAnm == NULL) {
        return 0;
    }

    if (!setExpressionAnm(1, false)) {
        return 0;
    }

    setMotion(0, -1.0f, 0);
    return 1;
}

int daObjTks_c::Delete() {
    fpc_ProcID id = fopAcM_GetID(this);
    this->~daObjTks_c();
    return 1;
}

int daObjTks_c::Execute() {
    if (field_0xde0 == 0) {
        return 1;
    }

    if (!chkAction(&daObjTks_c::warp)) {
        current.pos.set(6.0f, -5.0f, 4.0f);
        mDoMtx_stack_c::copy(daPy_getPlayerActorClass()->getLeftHandMatrix());
        mDoMtx_stack_c::multVec(&current.pos, &current.pos);
        old.pos = current.pos;
    }

    execute();
    return 1;
}

int daObjTks_c::Draw() {
    if (field_0xde0 == 0) {
        return 1;
    }

    mAnm_p->getModel()->getModelData()->getMaterialNodePointer(2)->setMaterialAnm(mpMatAnm);
    draw(FALSE, FALSE, daObjTks_Param_c::m.common.real_shadow_size, NULL, FALSE);
    return 1;
}

int daObjTks_c::ctrlJoint(J3DJoint* i_joint, J3DModel* i_model) {
    int jnt_no = i_joint->getJntNo();

    int spC[] = {15, 16, 17};
    if (jnt_no == 0) {
        mDoMtx_stack_c::copy(mAnm_p->getModel()->getAnmMtx(15));
        mDoMtx_stack_c::multVecZero(&mLookatPos[0]);

        mDoMtx_stack_c::copy(mAnm_p->getModel()->getAnmMtx(16));
        mDoMtx_stack_c::multVecZero(&mLookatPos[1]);

        mDoMtx_stack_c::copy(mAnm_p->getModel()->getAnmMtx(17));
        mDoMtx_stack_c::multVecZero(&mLookatPos[2]);
    }

    mDoMtx_stack_c::copy(i_model->getAnmMtx(jnt_no));

    switch (jnt_no) {
    case 15:
    case 16:
    case 17:
        setLookatMtx(jnt_no, spC, daObjTks_Param_c::m.common.neck_rotation_ratio);
        break;
    }

    i_model->setAnmMtx(jnt_no, mDoMtx_stack_c::get());
    cMtx_copy(mDoMtx_stack_c::get(), J3DSys::mCurrentMtx);

    if ((jnt_no == 0x11 || jnt_no == 0x13) && (mAnmFlags & 0x100)) {
        J3DAnmTransform* bck_p = mBckAnm.getBckAnm();
        mBckAnm.changeBckOnly(mAnm_p->getAnm());
        mAnm_p->changeAnm(bck_p);
    }

    return 1;
}

int daObjTks_c::createHeapCallBack(fopAc_ac_c* i_this) {
    return ((daObjTks_c*)i_this)->CreateHeap();
}

int daObjTks_c::ctrlJointCallBack(J3DJoint* i_joint, int param_1) {
    if (param_1 == NULL) {
        J3DModel* mdl_p = j3dSys.getModel();
        daObjTks_c* a_this = (daObjTks_c*)mdl_p->getUserArea();

        if (a_this != NULL) {
            a_this->ctrlJoint(i_joint, mdl_p);
        }
    }

    return 1;
}

void daObjTks_c::setMtx() {
    J3DModel* mdl_p = mAnm_p->getModel();

    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::ZXYrotM(shape_angle);
    mDoMtx_stack_c::scaleM(scale);

    mdl_p->setBaseTRMtx(mDoMtx_stack_c::get());
    mdl_p->setUserArea((uintptr_t)this);

    if (mAnmFlags & ANM_PLAY_BCK) {
        mBckAnm.getBckAnm()->setFrame(mBckAnm.getFrame());
    }

    mAnm_p->modelCalc();
}

bool daObjTks_c::setExpressionAnm(int i_anm, bool i_modify) {
    mAnmFlags &= ~ANM_EXPRESSION_FLAGS;

    J3DAnmTransform* bck_p = l_bckGetParamList[i_anm].fileIdx >= 0 ? getTrnsfrmKeyAnmP(l_arcName, l_bckGetParamList[i_anm].fileIdx) : NULL;
    int mode = l_bckGetParamList[i_anm].arcIdx;
    bool var_r29 = false;

    switch (i_anm) {
    case 0:
        var_r29 = setExpressionBtp(0);
        break;
    case 1:
        var_r29 = setExpressionBtp(0);
        break;
    default:
        bck_p = NULL;
        break;
    }

    if (!var_r29) {
        return false;
    }

    if (bck_p == NULL) {
        return true;
    }

    if (setBckAnm(bck_p, 1.0f, mode, 0, -1, i_modify)) {
        mAnmFlags |= (ANM_PLAY_BCK | ANM_PAUSE_BCK);
        mExpressionLoops = 0;
        return true;
    } else {
        // "%s: Failed to register expression Bck animation! \n"
        OS_REPORT("%s: 表情Bckアニメーションの登録に失敗しました！\n", __FILE__);
        return false;
    }    
}

bool daObjTks_c::setExpressionBtp(int i_anm) {
    J3DAnmTexPattern* btp_p = getTexPtrnAnmP(l_arcName, l_btpGetParamList[i_anm].fileIdx);
    int mode = l_btpGetParamList[i_anm].arcIdx;
    mAnmFlags &= ~(ANM_FLAG_800 | ANM_PLAY_BTP | ANM_PAUSE_BTP);

    if (btp_p == NULL) {
        return 1;
    }

    if (setBtpAnm(btp_p, mAnm_p->getModel()->getModelData(), 1.0f, mode)) {
        mAnmFlags |= (ANM_PLAY_BTP | ANM_PAUSE_BTP);
        if (i_anm == 0) {
            mAnmFlags |= ANM_FLAG_800;
        }
        return true;
    } else {
        // "%s: Failed to register expression Btp animation! \n"
        OS_REPORT("%s: 表情Btpアニメーションの登録に失敗しました！\n", __FILE__);
        return false;
    }
}

void daObjTks_c::setMotion(int i_motion, f32 i_morf, BOOL i_restart) {
    s16 motion = i_motion;
    if (i_restart || mMotion != motion) {
        if (i_motion >= 0 && i_motion < 4) {
            mMotion = motion;
            mMotionMorfOverride = i_morf;
            mMotionPrevPhase = -1;
            mMotionPhase = 0;
        }
    }
}

void daObjTks_c::reset() {
    initialize();
    mpMatAnm->initialize();
    mLookat.initialize();

    gravity = 0.0f;
    mAction = NULL;
    mLookMode = -1;
    field_0xdde = 0;

    daPy_py_c* player = daPy_getPlayerActorClass();
    mDoMtx_stack_c::transS(*fopAcM_GetPosition_p(player));
    mDoMtx_stack_c::YrotM(fopAcM_GetAngle_p(player)->y);
    mDoMtx_stack_c::transM(daObjTks_Param_c::m.offset_x, daObjTks_Param_c::m.offset_y, daObjTks_Param_c::m.offset_z);
    mDoMtx_stack_c::multVecZero(&home.pos);
    old.pos = home.pos;
    current.pos = home.pos;

    setAngle(fopAcM_searchPlayerAngleY(this));
    speedF = 0.0f;
    speed.setall(0.0f);
    scale.setall(0.0f);
    field_0xdcc = 0.15f;
    field_0xdd0 = 0.0f;
    parentActorID = fpcM_ERROR_PROCESS_ID_e;
    setAction(&daObjTks_c::talk);

    mExpressionMorfOverride = 0.0f;
    mMotionMorfOverride = 0.0f;
}

int daObjTks_c::setAction(void (daObjTks_c::*i_action)()) {
    field_0xdde = 3;

    if (mAction) {
        (this->*mAction)();
    }

    field_0xdde = 0;
    mAction = i_action;

    if (mAction) {
        (this->*mAction)();
    }

    return 1;
}

void daObjTks_c::wait() {
    switch (field_0xdde) {
    case 0:
        setExpression(1, -1.0f);
        setMotion(0, -1.0f, 0);
        setLookMode(2);
        speedF = 0.0f;
        field_0xdde = 2;
        break;
    case 2:
    case 3:
        break;
    default:
        JUT_ASSERT(1183, FALSE);
        break;
    }
}

void daObjTks_c::setExpression(int i_expression, f32 i_morf) {
    if (i_expression >= 0 && i_expression < 2) {
        mExpression = i_expression;
        mExpressionMorfOverride = i_morf;
        mExpressionPrevPhase = -1;
        mExpressionPhase = 0;
    }
}

void daObjTks_c::playExpression() {
    daNpcF_anmPlayData anm0_phase1 = {1, daObjTks_Param_c::m.common.morf_frame, 0};
    daNpcF_anmPlayData* anm0_m[] = {&anm0_phase1};

    daNpcF_anmPlayData anm1_phase1 = {0, daObjTks_Param_c::m.common.morf_frame, 0};
    daNpcF_anmPlayData* anm1_m[] = {&anm1_phase1};

    daNpcF_anmPlayData** anmData_p[] = {anm0_m, anm1_m};

    if (mExpression >= 0 && mExpression < 2) {
        playExpressionAnm(anmData_p);
    }
}

void daObjTks_c::playMotion() {
    daNpcF_anmPlayData anm0_phase1 = {2, daObjTks_Param_c::m.common.morf_frame, 0};
    daNpcF_anmPlayData* anm0_m[] = {&anm0_phase1};

    daNpcF_anmPlayData anm1_phase1 = {3, daObjTks_Param_c::m.common.morf_frame, 1};
    daNpcF_anmPlayData* anm1_m[] = {&anm1_phase1, NULL};

    daNpcF_anmPlayData anm2_phase1 = {4, daObjTks_Param_c::m.common.morf_frame, 0};
    daNpcF_anmPlayData* anm2_m[] = {&anm2_phase1};

    daNpcF_anmPlayData anm3_phase1 = {5, daObjTks_Param_c::m.common.morf_frame, 1};
    daNpcF_anmPlayData* anm3_m[] = {&anm3_phase1, NULL};

    daNpcF_anmPlayData** anmData_p[] = {anm0_m, anm1_m, anm2_m, anm3_m};

    if (mMotion >= 0 && mMotion < 4) {
        playMotionAnm(anmData_p);
    }
}

void daObjTks_c::lookat() {
    daPy_py_c* player = NULL;
    J3DModel* model_p = mAnm_p->getModel();

    int var_r28 = 0;
    f32 body_angleX_min = daObjTks_Param_c::m.common.body_angleX_min;
    f32 body_angleX_max = daObjTks_Param_c::m.common.body_angleX_max;
    f32 body_angleY_min = daObjTks_Param_c::m.common.body_angleY_min;
    f32 body_angleY_max = daObjTks_Param_c::m.common.body_angleY_max;
    f32 head_angleX_min = daObjTks_Param_c::m.common.head_angleX_min;
    f32 head_angleX_max = daObjTks_Param_c::m.common.head_angleX_max;
    f32 head_angleY_min = daObjTks_Param_c::m.common.head_angleY_min;
    f32 head_angleY_max = daObjTks_Param_c::m.common.head_angleY_max;

    s16 temp_r26 = mCurAngle.y - mOldAngle.y;
    cXyz sp30[] = {mLookatPos[0], mLookatPos[1], mLookatPos[2]};
    csXyz* sp24[] = {&mLookatAngle[0], &mLookatAngle[1], &mLookatAngle[2]};

    cXyz sp18;
    switch (mLookMode) {
    case LOOK_NONE:
        break;
    case LOOK_RESET:
        var_r28 = 1;
        break;
    case LOOK_PLAYER:
    case LOOK_PLAYER_TALK:
        player = daPy_getPlayerActorClass();
        if (mLookMode == LOOK_PLAYER_TALK) {
            head_angleY_min = -80.0f;
            head_angleY_max = 80.0f;
        }
        break;
    }

    if (player != NULL) {
        mLookPos = player->attention_info.position;
        if (mLookMode != LOOK_PLAYER && mLookMode != LOOK_PLAYER_TALK) {
            mLookPos.y -= 40.0f;
        }
        mLookat.setAttnPos(&mLookPos);
    } else {
        mLookat.setAttnPos(NULL);
    }

    mLookat.setParam(body_angleX_min, body_angleX_max, body_angleY_min, body_angleY_max,
                     0.0f, 0.0f, 0.0f, 0.0f,
                     head_angleX_min, head_angleX_max, head_angleY_min, head_angleY_max,
                     mCurAngle.y, sp30);
    mLookat.calc(this, model_p->getBaseTRMtx(), sp24, var_r28, temp_r26, 0);
}

void daObjTks_c::talk() {
    switch (field_0xdde) {
    case 0:
        setExpression(1, -1.0f);
        setMotion(0, -1.0f, 0);
        setLookMode(3);
        initTalk(2, NULL);
        mMsgTimer = 0;
        field_0xdde = 2;
        return;
    case 2: {
        if (field_0xde1 != 0 && parentActorID == fpcM_ERROR_PROCESS_ID_e && daNpcF_chkEvtBit(0x147)) {
            cXyz pos(current.pos);
            pos.y += 30.0f;
            parentActorID = fopAcM_createChild(PROC_NPC_TKC, fopAcM_GetID(this), 1, &pos, fopAcM_GetRoomNo(this), &mCurAngle, NULL, -1, NULL);
        }

        if (talkProc(NULL, TRUE, NULL)) {
            int choice_no = mFlow.getChoiceNo();
            OS_REPORT("おばちゃん二択分岐 %s\n", choice_no == 0 ? "はい" : "いいえ");

            if (choice_no == 0) {
                if (daNpcF_chkEvtBit(0x147)) {
                    setAction(&daObjTks_c::warp);
                } else {
                    setAction(&daObjTks_c::demo);
                }
            } else {
                dCam_getBody()->EndEventCamera(fopAcM_GetID(this));
                dComIfGp_event_reset();
                fopAcM_delete(this);
            }
            return;
        }

        int temp_r27 = mMsgTimer;
        int sp14, sp10;
        if (ctrlMsgAnm(sp14, sp10, this, FALSE)) {
            setExpression(sp14, -1.0f);
            setMotion(sp10, -1.0f, 0);
        } else if (temp_r27 != 0 && mMsgTimer == 0) {
            setExpressionTalkAfter();
        }
        return;
    }
    case 3:
    default:
        JUT_ASSERT(1243, FALSE);
        return;
    }
}

void daObjTks_c::demo() {
    switch (field_0xdde) {
    case 0:
        dCam_getBody()->EndEventCamera(fopAcM_GetID(this));
        dComIfGp_getEvent().reset(this);
        eventInfo.setArchiveName(l_arcName);
        dComIfGp_getEventManager().setObjectArchive(eventInfo.getArchiveName());
        mEventIdx = dComIfGp_getEventManager().getEventIdx(this, "TKS_SECRET", 0xFF);
        fopAcM_orderChangeEventId(this, mEventIdx, 1, 0xFFFF);
        setExpression(1, -1.0f);
        setMotionAnm(2, 0.0f);
        setLookMode(2);
        field_0xdde = 2;
        return;
    case 2:
        if (dComIfGp_event_runCheck() && !eventInfo.checkCommandTalk()) {
            dEvent_manager_c& eventMgr = dComIfGp_getEventManager();
            int staff_id = eventMgr.getMyStaffId(l_myName, NULL, 0);
            if (staff_id == -1) {
                return;
            }

            char* cut_name = eventMgr.getMyNowCutName(staff_id);

            if (eventMgr.getIsAddvance(staff_id)) {
                switch (*(u32*)cut_name) {
                case '0001':
                case '0003':
                case '0004':
                case '0005':
                case '0006':
                    initTalk(2, NULL);
                    break;
                case '0002':
                    cXyz pos(current.pos);
                    pos.y += 30.0f;
                    parentActorID = fopAcM_createChild(PROC_NPC_TKC, fopAcM_GetID(this), 0, &pos, fopAcM_GetRoomNo(this), &mCurAngle, NULL, -1, NULL);

                    Z2GetAudioMgr()->subBgmStart(Z2BGM_OBACHAN);
                    Z2GetAudioMgr()->bgmAllUnMute(0);
                    break;
                }
            }

            switch (*(u32*)cut_name) {
            case '0001':
            case '0003':
            case '0004':
            case '0005': {
                fopAc_ac_c* mesg_actor_list[2] = {NULL, NULL};
                mesg_actor_list[0] = this;
                mesg_actor_list[1] = fopAcM_SearchByID(parentActorID);
                dComIfGp_setMesgCameraInfoActor(mesg_actor_list[0], mesg_actor_list[1], NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

                if (talkProc(NULL, 1, mesg_actor_list)) {
                    if (*(u32*)cut_name == '0001') {
                        Z2GetAudioMgr()->bgmAllMute(140, 0.0f);
                    }
                    eventMgr.cutEnd(staff_id);
                } else {
                    int sp30, sp2C;
                    int sp28 = mMsgTimer;
                    if (ctrlMsgAnm(sp30, sp2C, this, FALSE)) {
                        setExpression(sp30, -1.0f);
                        setMotion(sp2C, -1.0f, 0);
                    } else if (sp28 != 0 && mMsgTimer == 0) {
                        setExpressionTalkAfter();
                    }
                }
                break;
            }
            case '0006':
                if (talkProc(NULL, TRUE, NULL)) {
                    setAction(&daObjTks_c::warp);
                    eventMgr.cutEnd(staff_id);
                } else {
                    int sp24, sp20;
                    int sp1C = mMsgTimer;
                    if (ctrlMsgAnm(sp24, sp20, this, 0)) {
                        setExpression(sp24, -1.0f);
                        setMotion(sp20, -1.0f, 0);
                    } else if (sp1C != 0 && mMsgTimer == 0) {
                        setExpressionTalkAfter();
                    }
                }
                break;
            case '0002':
                eventMgr.cutEnd(staff_id);
                break;
            }

            if (eventInfo.checkCommandDemoAccrpt() && mEventIdx != -1 && eventMgr.endCheck(mEventIdx)) {
                setAction(&daObjTks_c::wait);
            }
        }
        break;
    case 3:
        if (field_0x9ec == 0) {
            dComIfGp_event_reset();
        }
        field_0x9ec = 0;
        mEventIdx = -1;
        return;
    default:
        JUT_ASSERT(1374, FALSE);
        break;
    }
}

void daObjTks_c::setMotionAnm(int i_anm, f32 i_morf) {
    if (i_anm < 2 || i_anm >= 6) {
        return;
    }

    J3DAnmTransformKey* bck_p = getTrnsfrmKeyAnmP(l_arcName, l_bckGetParamList[i_anm].fileIdx);
    int temp_r25 = l_bckGetParamList[i_anm].arcIdx;
    mAnmFlags &= ~(ANM_PLAY_BTK | ANM_PLAY_MORF | ANM_PAUSE_BTK | ANM_PAUSE_MORF);
    if (bck_p != NULL && setMcaMorfAnm(bck_p, 1.0f, i_morf, temp_r25, 0, -1)) {
        mAnmFlags |= (ANM_PLAY_MORF | ANM_PAUSE_MORF);
        mMotionLoops = 0;
    }

    J3DAnmTextureSRTKey* btk_p = getTexSRTKeyAnmP(l_arcName, l_btkGetParamList[0].fileIdx);
    int temp_r24 = l_btkGetParamList[0].arcIdx;
    if (btk_p != NULL && setBtkAnm(btk_p, mAnm_p->getModel()->getModelData(), 1.0f, temp_r24)) {
        mAnmFlags |= (ANM_PLAY_BTK | ANM_PAUSE_BTK);
    }
}

void daObjTks_c::warp() {
    switch (field_0xdde) {
    case 0:
        dCam_getBody()->EndEventCamera(fopAcM_GetID(this));
        dComIfGp_getEvent().reset(this);
        eventInfo.setArchiveName(l_arcName);
        dComIfGp_getEventManager().setObjectArchive(eventInfo.getArchiveName());
        mEventIdx = dComIfGp_getEventManager().getEventIdx(this, "TKS_WARP", 0xFF);
        fopAcM_orderChangeEventId(this, mEventIdx, 1, 0xFFFF);

        setExpression(1, -1.0f);
        setMotionAnm(2, 0.0f);
        setLookMode(2);
        field_0xdde = 2;
        return;
    case 2:
        if (dComIfGp_event_runCheck() != 0 && !eventInfo.checkCommandTalk()) {
            dEvent_manager_c& eventMgr = dComIfGp_getEventManager();
            int staff_id = eventMgr.getMyStaffId(l_myName, NULL, 0);

            if (staff_id != -1) {
                char* cut_name = eventMgr.getMyNowCutName(staff_id);
                if (eventMgr.getIsAddvance(staff_id)) {
                    switch (*(u32*)cut_name) {
                    case '0002':
                        gravity = daObjTks_Param_c::m.common.gravity;
                        break;
                    case '0001':
                        JUT_ASSERT(1419, FALSE);
                    }
                }

                switch (*(u32*)cut_name) {
                case '0001':
                    eventMgr.cutEnd(staff_id);
                    break;
                case '0002':
                    if (mAcch.ChkGroundLanding()) {
                        speed.y *= -0.5f;
                    }
                    eventMgr.cutEnd(staff_id);
                    break;
                default:
                    JUT_ASSERT(1431, FALSE);
                }

                if (eventInfo.checkCommandDemoAccrpt() && mEventIdx != -1 && eventMgr.endCheck(mEventIdx)) {
                    setAction(&daObjTks_c::wait);
                }
            }
        } else {
            setAction(&daObjTks_c::wait);
        }
        return;
    case 3:
        if (field_0x9ec == 0) {
            dComIfGp_event_reset();
        }
        field_0x9ec = 0;
        mEventIdx = -1;
        return;
    default:
        JUT_ASSERT(1452, FALSE);
        break;
    }
}

static int daObjTks_Create(void* i_this) {
    return ((daObjTks_c*)i_this)->Create();
}

static int daObjTks_Delete(void* i_this) {
    return ((daObjTks_c*)i_this)->Delete();
}

static int daObjTks_Execute(void* i_this) {
    return ((daObjTks_c*)i_this)->Execute();
}

static int daObjTks_Draw(void* i_this) {
    return ((daObjTks_c*)i_this)->Draw();
}

static int daObjTks_IsDelete(void* i_this) {
    return 1;
}

void daObjTks_c::setParam() {
    calcSpringF(&field_0xdcc, 0.7f, &field_0xdd0);
    scale.setall(field_0xdcc);

    #if VERSION == VERSION_SHIELD_DEBUG
    mAcchCir.SetWallR(daObjTks_Param_c::m.common.width);
    mAcchCir.SetWallH(daObjTks_Param_c::m.common.height);
    #endif
}

BOOL daObjTks_c::main() {
    if (mAction != NULL) {
        (this->*mAction)();
    }

    playExpression();
    playMotion();
    return 1;
}

BOOL daObjTks_c::ctrlBtk() {
    if (mpMatAnm != NULL) {
        J3DAnmTextureSRTKey* btk_p = getTexSRTKeyAnmP(l_arcName, l_btkGetParamList[0].fileIdx);
        if (btk_p == mBtkAnm.getBtkAnm()) {
            mpMatAnm->setNowOffsetX(-1.0f * (0.2f * cM_ssin(mEyeAngle.y)));
            mpMatAnm->setNowOffsetY(0.2f * cM_ssin(mEyeAngle.x));
            mpMatAnm->onEyeMoveFlag();
            return TRUE;
        }

        mpMatAnm->offEyeMoveFlag();
    }

    return FALSE;
}

void daObjTks_c::setAttnPos() {
    if (mLookMode == LOOK_RESET) {
        for (int i = 0; i < 3; i++) {
            mLookatAngle[i].setall(0);
        }
    }

    setMtx();
    lookat();

    cXyz sp14(10.0f, 10.0f, 0.0f);

    mDoMtx_stack_c::copy(mAnm_p->getModel()->getAnmMtx(17));
    mDoMtx_stack_c::multVecZero(&mHeadPos);
    mDoMtx_stack_c::multVec(&sp14, &eyePos);
    sp14.x = 0.0f;
    mDoMtx_stack_c::multVec(&sp14, &sp14);

    mHeadAngle.x = cLib_targetAngleX(&mHeadPos, &sp14);
    mHeadAngle.y = cLib_targetAngleY(&mHeadPos, &sp14);
    
    cXyz* attn_pos_p = mLookat.getAttnPos();
    if (attn_pos_p != NULL) {
        cXyz sp8 = *attn_pos_p - eyePos;
        
        mEyeAngle.y = -mLookatAngle[2].y - mCurAngle.y;
        mEyeAngle.y += cM_atan2s(sp8.x, sp8.z);
        mEyeAngle.x = -cM_atan2s(sp8.y, sp8.absXZ());
    } else {
        mEyeAngle.y = 0;
        mEyeAngle.x = 0;
    }

    attention_info.position.set(current.pos.x, current.pos.y + daObjTks_Param_c::m.common.attention_offset, current.pos.z);

    if (!fopAcM_checkCarryNow(this)) {
        mCcCyl.SetC(current.pos);
        #if VERSION == VERSION_SHIELD_DEBUG
        mCcCyl.SetH(daObjTks_Param_c::m.common.height);
        mCcCyl.SetR(daObjTks_Param_c::m.common.width);
        #endif
        dComIfG_Ccsp()->Set(&mCcCyl);
    }
}

BOOL daObjTks_c::drawDbgInfo() {
    return false;
}

void daObjTks_c::drawOtherMdls() {}

static daObjTks_Param_c l_HIO;

static actor_method_class daObjTks_MethodTable = {
    (process_method_func)daObjTks_Create,
    (process_method_func)daObjTks_Delete,
    (process_method_func)daObjTks_Execute,
    (process_method_func)daObjTks_IsDelete,
    (process_method_func)daObjTks_Draw,
};

extern actor_process_profile_definition g_profile_OBJ_TKS = {
    fpcLy_CURRENT_e,            // mLayerID
    7,                          // mListID
    fpcPi_CURRENT_e,            // mListPrio
    PROC_OBJ_TKS,               // mProcName
    &g_fpcLf_Method.base,      // sub_method
    sizeof(daObjTks_c),         // mSize
    0,                          // mSizeOther
    0,                          // mParameters
    &g_fopAc_Method.base,       // sub_method
    376,                        // mPriority
    &daObjTks_MethodTable,      // sub_method
    0x08044008,                 // mStatus
    fopAc_NPC_e,                // mActorType
    fopAc_CULLBOX_CUSTOM_e,     // cullType
};
