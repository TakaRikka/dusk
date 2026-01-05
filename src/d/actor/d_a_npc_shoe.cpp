/**
 * @file d_a_npc_shoe.cpp
 *
 */

#include "d/dolzel_rel.h"  // IWYU pragma: keep

#include "d/actor/d_a_npc_shoe.h"
#include "d/actor/d_a_npc.h"

const daNpcShoe_HIOParam daNpcShoe_Param_c::m = {
    55.0f,    // attention_offset
    -3.0f,    // gravity
    1.0f,     // scale
    300.0f,   // real_shadow_size
    255.0f,   // weight
    140.0f,   // height
    35.0f,    // knee_length
    30.0f,    // width
    0.0f,     // body_angleX_max
    0.0f,     // body_angleX_min
    10.0f,    // body_angleY_max
    -10.0f,   // body_angleY_min
    30.0f,    // head_angleX_max
    -10.0f,   // head_angleX_min
    45.0f,    // head_angleY_max
    -45.0f,   // head_angleY_min
    0.6f,     // neck_rotation_ratio
    12.0f,    // morf_frame
    2,        // talk_distance
    2,        // talk_angle
    3,        // attention_distance
    2,        // attention_angle
    60.0f,    // fov
    500.0f,   // search_distance
    300.0f,   // search_height
    -300.0f,  // search_depth
    60,       // attention_time
    0,        // damage_time
    0,        // face_expression
    0,        // motion
    0,        // look_mode
    0,        // debug_mode_ON
    0,        // debug_info_ON
};

#if DEBUG
daNpcShoe_HIO_c::daNpcShoe_HIO_c() {
    m = daNpcShoe_Param_c::m;
}

void daNpcShoe_HIO_c::genMessage(JORMContext* ctext) {
    // TODO
}
#endif

NPC_SHOE_HIO_CLASS l_HIO;

void daNpcShoe_c::setLookMode(int i_lookMode) {
    if (mLookMode != i_lookMode) {
        mLookMode = i_lookMode;
    }
}

bool daNpcShoe_c::chkFindPlayer() {
    bool ret;
    if (!chkActorInSight(daPy_getPlayerActorClass(), mpHIO->m.common.fov)) {
        mActorMngr[0].remove();
        ret = false;
    } else {
        if (mActorMngr[0].getActorP() == NULL) {
            ret = chkPlayerInSpeakArea(this);
        } else {
            ret = chkPlayerInTalkArea(this);
        }
        if (ret) {
            mActorMngr[0].entry(daPy_getPlayerActorClass());
        } else {
            mActorMngr[0].remove();
        }
    }
    return ret;
}

static daNpc_GetParam2 l_bckGetParamList[5] = {
    {17, 2, 1},  // m_sitwait_a
    {13, 0, 1},  // m_sittalk_a
    {14, 0, 1},  // m_sittalk_a_b
    {5, 0, 0},   // shoe_talk_a
    {11, 2, 1}   // m_sit_to_wolf_a
};

static daNpc_GetParam2 l_btpGetParamList[1] = {
    {12, 2, 0},  // shoe
};

static char* l_arcNames[3] = {
    "shoe",
    "Mgeneral",
    "object",
};

static char* l_evtNames[1] = {NULL};

static char* l_myName = "Shoe";

daNpcShoe_c::EventFn daNpcShoe_c::mEvtSeqList[1] = {NULL};

daNpcShoe_c::daNpcShoe_c() {}

daNpcShoe_c::~daNpcShoe_c() {
    for (int i = 0; i < 3; i++) {
        dComIfG_resDelete(&mPhases[i], l_arcNames[i]);
    }
    if (heap != NULL) {
        mAnm_p->stopZelAnime();
    }
}

cPhs__Step daNpcShoe_c::Create() {
    cPhs__Step step;

    fopAcM_ct(this, daNpcShoe_c);

    field_0xe0c = getMessageNo();

    step = cPhs_ERROR_e;
    for (int i = 0; i < 3; i++) {
        step = (cPhs__Step)dComIfG_resLoad(&mPhases[i], l_arcNames[i]);
        if (step != cPhs_COMPLEATE_e) {
            return step;
        }
    }

    if (step == cPhs_COMPLEATE_e) {
        if (!fopAcM_entrySolidHeap(this, createHeapCallBack, 0x2920)) {
            return cPhs_ERROR_e;
        }

        fopAcM_SetMtx(this, mAnm_p->mpModel->getBaseTRMtx());

        fopAcM_setCullSizeBox(this, -50.0f, -10.0f, -50.0f, 50.0f, 160.0f, 50.0f);

        mCreature.init(&current.pos, &eyePos, 3, 1);
        mCreature.setMdlType(7, false, (BOOL)(dKy_darkworld_check() != 0));

        mAcchCir.SetWall(mpHIO->m.common.width, mpHIO->m.common.knee_length);

        mAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1, &mAcchCir,
                  fopAcM_GetSpeed_p(this), fopAcM_GetAngle_p(this), fopAcM_GetShapeAngle_p(this));

        mAcch.CrrPos(dComIfG_Bgsp());
        mCcStts.Init((int)mpHIO->m.common.weight, 0, this);
        mCcCyl.Set(mCcDCyl);

        mCcCyl.SetStts(&mCcStts);
        mCcCyl.SetTgType(0);
        mCcCyl.SetTgSPrm(0);

        mGndChk = mAcch.m_gnd;
        mGroundH = mAcch.GetGroundH();
        gravity = mpHIO->m.common.gravity;

        setEnvTevColor();
        setRoomNo();
        reset();
        execute();
    }

    return step;
}

BOOL daNpcShoe_c::CreateHeap() {
    J3DModelData* mdlData_p = static_cast<J3DModelData*>(dComIfG_getObjectRes(l_arcNames[0], 8));

    JUT_ASSERT(292, NULL != mdlData_p)

    mAnm_p = new mDoExt_McaMorfSO(mdlData_p, NULL, NULL, NULL, -1, 1.0f, 0, -1, &mCreature, 0x80000,
                                  0x11020084);

    if (mAnm_p != NULL && mAnm_p->mpModel == NULL) {
        mAnm_p->stopZelAnime();
        mAnm_p = NULL;
    }

    if (mAnm_p == NULL) {
        return false;
    }

    cXyz cstack_30(1.0f, 0.5581154f, 1.0f);
    mAnm_p->offTranslate();
    mAnm_p->setTranslateScale(cstack_30);

    J3DModel* model = mAnm_p->getModel();
    for (u16 jointNo = 0; jointNo < mdlData_p->getJointNum(); jointNo++) {
        mdlData_p->getJointNodePointer(jointNo)->setCallBack(ctrlJointCallBack);
    }
    model->setUserArea((uintptr_t)this);

    mdlData_p = static_cast<J3DModelData*>(dComIfG_getObjectRes(l_arcNames[0], 9));

    JUT_ASSERT(331, NULL != mdlData_p)

    mpModel1 = mDoExt_J3DModel__create(mdlData_p, 0x80000, 0x11000084);
    if (mpModel1 == NULL) {
        return false;
    }

    mdlData_p = static_cast<J3DModelData*>(dComIfG_getObjectRes(l_arcNames[2], 4));

    JUT_ASSERT(338, NULL != mdlData_p)

    mpModel2 = mDoExt_J3DModel__create(mdlData_p, 0x80000, 0x11000084);
    if (mpModel2 == NULL) {
        return false;
    }

    setMotion(MOT_SITWAIT_A, -1.0f, 0);

    return true;
}

int daNpcShoe_c::Delete() {
    this->~daNpcShoe_c();
    return 1;
}

int daNpcShoe_c::Execute() {
    if (field_0xe1b != 0) {
        fopAcM_OnStatus(this, 0x8000000);
    } else {
        fopAcM_OffStatus(this, 0x8000000);
    }

    if (fopAcM_CheckCondition(this, fopAcCnd_NODRAW_e)) {
        int time = getTime();
        if (time >= mStartTime && time < mEndTime) {
            field_0xe1b = 1;
        } else {
            field_0xe1b = 0;
        }
    } else {
        if (field_0xe1b != 0) {
            execute();
        }
    }

    return 1;
}

int daNpcShoe_c::Draw() {
    if (!field_0xe1b) {
        return 1;
    }

    draw(false, false, mpHIO->m.common.real_shadow_size, NULL, false);
    g_env_light.setLightTevColorType_MAJI(mpModel1->mModelData, &tevStr);
    mDoMtx_stack_c::copy(mAnm_p->getModel()->getAnmMtx(11));
    mpModel1->setBaseTRMtx(mDoMtx_stack_c::get());
    mDoExt_modelUpdateDL(mpModel1);
    g_env_light.setLightTevColorType_MAJI(mpModel2->mModelData, &tevStr);
    mDoExt_modelUpdateDL(mpModel2);
    dComIfGd_setSimpleShadow(&current.pos, mAcch.GetGroundH(), 50.0f, mAcch.m_gnd, 0, 1.0f,
                             dDlst_shadowControl_c::getSimpleTex());

    return 1;
}

bool daNpcShoe_c::ctrlJoint(J3DJoint* i_joint, J3DModel* i_model) {
    int joint_no = i_joint->getJntNo();
    int lookat_joints[3] = {1, 2, 3};

    if (joint_no == 0) {
        mDoMtx_stack_c::copy(mAnm_p->getModel()->getAnmMtx(1));
        mDoMtx_stack_c::multVecZero(&mLookatPos[0]);
        mDoMtx_stack_c::copy(mAnm_p->getModel()->getAnmMtx(2));
        mDoMtx_stack_c::multVecZero(&mLookatPos[1]);
        mDoMtx_stack_c::copy(mAnm_p->getModel()->getAnmMtx(3));
        mDoMtx_stack_c::multVecZero(&mLookatPos[2]);
    }

    mDoMtx_stack_c::copy(i_model->getAnmMtx(joint_no));
    switch (joint_no) {
    case 1:
    case 2:
    case 3:
        setLookatMtx(joint_no, lookat_joints, mpHIO->m.common.neck_rotation_ratio);
        break;
    }

    i_model->setAnmMtx(joint_no, mDoMtx_stack_c::get());
    cMtx_copy(mDoMtx_stack_c::get(), J3DSys::mCurrentMtx);

    return true;
}

BOOL daNpcShoe_c::createHeapCallBack(fopAc_ac_c* i_this) {
    return static_cast<daNpcShoe_c*>(i_this)->CreateHeap();
}

BOOL daNpcShoe_c::ctrlJointCallBack(J3DJoint* i_joint, int param_1) {
    if (param_1 == 0) {
        J3DModel* model = j3dSys.getModel();
        daNpcShoe_c* _this = (daNpcShoe_c*)model->getUserArea();
        if (_this != NULL) {
            _this->ctrlJoint(i_joint, model);
        }
    }
    return true;
}

void daNpcShoe_c::setMotion(int i_motion, f32 i_morf, BOOL i_restart) {
    s16 motion = (s16)i_motion;
    if (i_restart || mMotion != motion) {
        if (i_motion >= 0 && i_motion < 5) {
            mMotion = motion;
            mMotionMorfOverride = i_morf;
            mMotionPrevPhase = -1;
            mMotionPhase = 0;
            field_0xe16 = 0;
        }
    }
}

void daNpcShoe_c::reset() {
    initialize();
    mLookat.initialize();
    for (int i = 0; i < 1; i++) {
        mActorMngr[i].initialize();
    }

    field_0xe04 = 0;
    field_0xe08 = 0;
    mpActionFn = NULL;
    mLookMode = -1;
    mMode = 0;

    u16 temp1;
    u16 temp2;
    u8 start_time = getStartTime();
    u8 end_time = getEndTime();
    if (start_time != 0xff) {
        temp1 = (start_time / 10);
        temp2 = (start_time % 10) * 10;
        mStartTime = temp2 + temp1 * 60;
    }
    if (end_time != 0xff) {
        temp1 = (end_time / 10);
        temp2 = (end_time % 10) * 10;
        mEndTime = temp2 + temp1 * 60;
    }

    field_0xe1b = 0;
    int time = getTime();
    if (time >= mStartTime && time < mEndTime) {
        field_0xe1b = 1;
    } else {
        field_0xe1b = 0;
    }

    current.pos.set(home.pos);
    old.pos.set(current.pos);
    current.angle.set(0, home.angle.y, 0);
    old.angle = current.angle;
    shape_angle = current.angle;
    mCurAngle = current.angle;
    mOldAngle = mCurAngle;

    speedF = 0.0f;
    speed.setall(0.0f);
    field_0xe00 = -1;
    mOrderEvtNo = 0;

    J3DAnmTexPattern* btpAnm = getTexPtrnAnmP(l_arcNames[0], 12);
    mAnmFlags &= ~(ANM_PAUSE_BTP | ANM_PLAY_BTP | ANM_FLAG_800);
    if (setBtpAnm(btpAnm, mAnm_p->getModel()->getModelData(), 1.0f, 2)) {
        mAnmFlags |= ANM_PAUSE_BTP | ANM_PLAY_BTP | ANM_FLAG_800;
    }

    Vec base_scale = {0.6, 0.6, 0.6};
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::ZXYrotM(shape_angle);
    mpModel2->setBaseScale(base_scale);
    mpModel2->setBaseTRMtx(mDoMtx_stack_c::get());

    setAction(&daNpcShoe_c::wait);

    mMotionMorfOverride = 0.0f;
}

void daNpcShoe_c::playMotion() {
    daNpcF_anmPlayData dat0 = {ANM_M_SITWAIT_A, mpHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat0[1] = {&dat0};
    daNpcF_anmPlayData dat1a = {ANM_M_SITTALK_A, mpHIO->m.common.morf_frame, 1};
    daNpcF_anmPlayData dat1b = {ANM_M_SITWAIT_A, mpHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat1[2] = {&dat1a, &dat1b};
    daNpcF_anmPlayData dat2a = {ANM_M_SITTALK_A_B, mpHIO->m.common.morf_frame, 1};
    daNpcF_anmPlayData dat2b = {ANM_M_SITWAIT_A, mpHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat2[2] = {&dat2a, &dat2b};
    daNpcF_anmPlayData dat3a = {ANM_SHOE_TALK_A, mpHIO->m.common.morf_frame, 1};
    daNpcF_anmPlayData dat3b = {ANM_M_SITWAIT_A, mpHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat3[2] = {&dat3a, &dat3b};
    daNpcF_anmPlayData dat4 = {ANM_M_SIT_TO_WOLF_A, mpHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat4[1] = {&dat4};

    daNpcF_anmPlayData** ppDat[5] = {
        pDat0, pDat1, pDat2, pDat3, pDat4,
    };
    if (mMotion >= 0 && mMotion < 5) {
        field_0xe16 = mMotionPhase;
        playMotionAnm(ppDat);
    }
}

void daNpcShoe_c::lookat() {
    fopAc_ac_c* actor = NULL;
    J3DModel* model = mAnm_p->getModel();

    BOOL snap = false;
    f32 body_angleX_min = mpHIO->m.common.body_angleX_min;
    f32 body_angleX_max = mpHIO->m.common.body_angleX_max;
    f32 body_angleY_min = mpHIO->m.common.body_angleY_min;
    f32 body_angleY_max = mpHIO->m.common.body_angleY_max;
    f32 head_angleX_min = mpHIO->m.common.head_angleX_min;
    f32 head_angleX_max = mpHIO->m.common.head_angleX_max;
    f32 head_angleY_min = mpHIO->m.common.head_angleY_min;
    f32 head_angleY_max = mpHIO->m.common.head_angleY_max;

    s16 angle_delta = mCurAngle.y - mOldAngle.y;
    cXyz lookat_pos[3] = {mLookatPos[0], mLookatPos[1], mLookatPos[2]};
    csXyz* lookat_angle[3] = {&mLookatAngle[0], &mLookatAngle[1], &mLookatAngle[2]};

    switch (mLookMode) {
    case LOOK_NONE:
        break;
    case LOOK_RESET:
        snap = true;
        break;
    case LOOK_PLAYER:
    case LOOK_PLAYER_TALK:
        actor = daPy_getPlayerActorClass();
        if (mLookMode != LOOK_PLAYER_TALK) {
            break;
        }
        head_angleY_min = -80.0f;
        head_angleY_max = 80.0f;
        break;
    }

    if (actor != NULL) {
        mLookPos = actor->attention_info.position;
        if (mLookMode != LOOK_PLAYER && mLookMode != LOOK_PLAYER_TALK) {
            mLookPos.y -= 40.0f;
        }
        mLookat.setAttnPos(&mLookPos);
    } else {
        mLookat.setAttnPos(NULL);
    }

    mLookat.setParam(body_angleX_min, body_angleX_max, body_angleY_min, body_angleY_max, 0.0f, 0.0f,
                     0.0f, 0.0f, head_angleX_min, head_angleX_max, head_angleY_min, head_angleY_max,
                     mCurAngle.y, lookat_pos);
    mLookat.calc(this, model->getBaseTRMtx(), lookat_angle, snap, angle_delta, false);
}

int daNpcShoe_c::setAction(bool (daNpcShoe_c::*i_actionFunc)(void*)) {
    mMode = 3;
    if (mpActionFn != NULL) {
        (this->*mpActionFn)(NULL);
    }

    mMode = 0;
    mpActionFn = i_actionFunc;
    if (mpActionFn != NULL) {
        (this->*mpActionFn)(NULL);
    }
    return 1;
}

bool daNpcShoe_c::wait(void* param_0) {
    switch (mMode) {
    case 0:
        setMotion(MOT_SITWAIT_A, -1.0f, false);
        speedF = 0.0f;
        mMode = 2;
        break;
    case 1:
        break;
    case 2: {
        chkFindPlayer();

        fopAc_ac_c* pactor = mActorMngr->getActorP();
        if (pactor != NULL) {
            setLookMode(LOOK_PLAYER);
        } else {
            setLookMode(LOOK_NONE);
        }

        if (dComIfGp_event_runCheck()) {
            if (eventInfo.checkCommandTalk()) {
                if (!dComIfGp_event_chkTalkXY() || dComIfGp_evmng_ChkPresentEnd()) {
                    setAction(&daNpcShoe_c::talk);
                }
            } else {
                if (dComIfGp_getEventManager().getMyStaffId(l_myName, NULL, -0) != -1) {
                    setAction(&daNpcShoe_c::demo);
                }
            }
        } else {
            if (!dKy_darkworld_check() && daPy_py_c::checkNowWolf()) {
                f32 max_distance = pow(500.0f, 2.0f);
                if (fopAcM_searchPlayerDistanceXZ2(this) < max_distance) {
                    setAction(&daNpcShoe_c::fear);
                    break;
                }
            }

            if (mOrderEvtNo != 0) {
                eventInfo.setArchiveName(l_arcNames[0]);
            }
            orderEvent(field_0xe1a, l_evtNames[mOrderEvtNo], 0xffff, 0x28, 0xff, 1);
        }
        break;
    }
    case 3:
        break;
    default:
        JUT_ASSERT(1063, FALSE);
        break;
    }

    return 1;
}

bool daNpcShoe_c::fear(void* param_0) {
    switch (mMode) {
    case 0:
        setMotion(MOT_SIT_TO_WOLF_A, -1.0f, false);
        speedF = 0.0f;
        mMode = 2;
        break;
    case 1:
        break;
    case 2:
        if (mAnm_p->checkFrame(1.0f)) {
            mCreature.playVoice(2);
        }
        break;
    case 3:
        break;
    default:
        JUT_ASSERT(1100, FALSE);
        break;
    }
    return 1;
}

bool daNpcShoe_c::talk(void* param_0) {
    bool ret = 0;
    switch (mMode) {
    case 0:
        initTalk(field_0xe0c, NULL);
        mMsgTimer = 0;
        field_0xe1a = 0;
        setLookMode(LOOK_PLAYER_TALK);
        mMode = 2;
        break;
    case 1:
        break;
    case 2:
        if (talkProc(NULL, 1, NULL)) {
            mActorMngr->entry(daPy_getPlayerActorClass());
            setAction(&daNpcShoe_c::wait);
            ret = 1;
        } else {
            int expression, motion;
            if (ctrlMsgAnm(expression, motion, this, false)) {
                setMotion(motion, -1.0f, false);
            }
        }
        break;
    case 3:
        if (field_0x9ec != 0) {
            break;
        }

        dComIfGp_event_reset();
        break;
    default:
        JUT_ASSERT(1169, FALSE);
        break;
    }
    return ret;
}

bool daNpcShoe_c::demo(void* param_0) {
    switch (mMode) {
    case 0:
        setMotion(MOT_SITWAIT_A, -1.0f, false);
        mMode = 2;
        break;
    case 1:
        break;
    case 2:
        if (dComIfGp_event_runCheck() && !eventInfo.checkCommandTalk()) {
            dEvent_manager_c& eventManager = dComIfGp_getEventManager();
            s32 staff_id = eventManager.getMyStaffId(l_myName, NULL, 0);
            if (staff_id != -1) {
                mStaffID = staff_id;

                JUT_ASSERT(1203, NULL != mEvtSeqList[mOrderEvtNo])

                if ((this->*(mEvtSeqList[mOrderEvtNo]))(staff_id)) {
                    eventManager.cutEnd(staff_id);
                }
            }

            if (eventInfo.checkCommandDemoAccrpt() && mEventIdx != -1 &&
                eventManager.endCheck(mEventIdx))
            {
                dComIfGp_event_reset();
                mOrderEvtNo = 0;
                mEventIdx = -1;
                setAction(&daNpcShoe_c::wait);
            }
        }
        break;
    case 3:
        break;
    default:
        JUT_ASSERT(1169, FALSE);
        break;
    }
    return 1;
}

static cPhs__Step daNpcShoe_Create(void* i_this) {
    return static_cast<daNpcShoe_c*>(i_this)->Create();
}

static int daNpcShoe_Delete(void* i_this) {
    return static_cast<daNpcShoe_c*>(i_this)->Delete();
}

static int daNpcShoe_Execute(void* i_this) {
    return static_cast<daNpcShoe_c*>(i_this)->Execute();
}

static int daNpcShoe_Draw(void* i_this) {
    return static_cast<daNpcShoe_c*>(i_this)->Draw();
}

static bool daNpcShoe_IsDelete(void* param_0) {
    return true;
}

void daNpcShoe_c::setParam() {
    attention_info.distances[0] =
        getDistTableIdx(mpHIO->m.common.attention_distance, mpHIO->m.common.attention_angle);
    attention_info.distances[1] = attention_info.distances[0];
    attention_info.distances[3] =
        getDistTableIdx(mpHIO->m.common.talk_distance, mpHIO->m.common.talk_angle);

    attention_info.flags =
        (field_0xe1b != 0) ? (fopAc_AttnFlag_SPEAK_e | fopAc_AttnFlag_TALK_e) : 0;

    scale.setall(mpHIO->m.common.scale);
    mAcchCir.SetWallR(mpHIO->m.common.width);
    mAcchCir.SetWallH(mpHIO->m.common.knee_length);
    gravity = mpHIO->m.common.gravity;
}

BOOL daNpcShoe_c::main() {
    if (mpActionFn != NULL) {
        (this->*mpActionFn)(0);
    }
    if (dComIfGp_event_runCheck() && !eventInfo.checkCommandTalk() && field_0xe00 != -1) {
        dComIfGp_event_setItemPartnerId(field_0xe00);
        field_0xe00 = -1;
    }
    playMotion();
    return 0;
}

void daNpcShoe_c::setAttnPos() {
    if (mLookMode == LOOK_RESET) {
        for (int i = 0; i < 3; i++) {
            mLookatAngle[i].setall(0);
        }
    }

    setMtx();
    lookat();

    cXyz vec(10.0f, 15.0f, 0.0f);
    mDoMtx_stack_c::copy(mAnm_p->getModel()->getAnmMtx(3));
    mDoMtx_stack_c::multVecZero(&mHeadPos);
    mDoMtx_stack_c::multVec(&vec, &eyePos);
    vec.x = 0.0f;
    mDoMtx_stack_c::multVec(&vec, &vec);

    mHeadAngle.x = cLib_targetAngleX(&mHeadPos, &vec);
    mHeadAngle.y = cLib_targetAngleY(&mHeadPos, &vec);

    mDoMtx_stack_c::copy(mAnm_p->getModel()->getAnmMtx(3));
    mDoMtx_stack_c::multVecZero(&attention_info.position);
    attention_info.position.y += mpHIO->m.common.attention_offset;

    cXyz cStack28;
    mDoMtx_stack_c::copy(mAnm_p->getModel()->getAnmMtx(1));
    mDoMtx_stack_c::multVecZero(&cStack28);
    cStack28.y = current.pos.y;
    mCcCyl.SetC(cStack28);
    mCcCyl.SetH(mpHIO->m.common.height);
    mCcCyl.SetR(mpHIO->m.common.width);
    dComIfG_Ccsp()->Set(&mCcCyl);
}

void daNpcShoe_c::setMotionAnm(int i_idx, f32 i_morf) {
    J3DAnmTransformKey* morfAnm = getTrnsfrmKeyAnmP(l_arcNames[l_bckGetParamList[i_idx].arcIdx],
                                                    l_bckGetParamList[i_idx].fileIdx);

    int morfAttr = l_bckGetParamList[i_idx].attr;

    mAnmFlags &= ~(ANM_PAUSE_MORF | ANM_PLAY_MORF);

    if (morfAnm) {
        if (setMcaMorfAnm(morfAnm, 1.0f, i_morf, morfAttr, 0, -1)) {
            mAnmFlags |= ANM_PAUSE_MORF | ANM_PLAY_MORF;
            mMotionLoops = 0;
        }
    }
}

BOOL daNpcShoe_c::drawDbgInfo() {
    return FALSE;
}

static actor_method_class daNpcShoe_MethodTable = {
    (process_method_func)daNpcShoe_Create,  (process_method_func)daNpcShoe_Delete,
    (process_method_func)daNpcShoe_Execute, (process_method_func)daNpcShoe_IsDelete,
    (process_method_func)daNpcShoe_Draw,
};

actor_process_profile_definition g_profile_NPC_SHOE = {
    fpcLy_CURRENT_e,         // mLayerID
    7,                       // mListID
    fpcPi_CURRENT_e,         // mListPrio
    PROC_NPC_SHOE,           // mProcName
    &g_fpcLf_Method.base,    // sub_method
    sizeof(daNpcShoe_c),     // mSize
    0,                       // mSizeOther
    0,                       // mParameters
    &g_fopAc_Method.base,    // sub_method
    417,                     // mPriority
    &daNpcShoe_MethodTable,  // sub_method
    0x00044107,              // mStatus
    fopAc_NPC_e,             // mActorType
    fopAc_CULLBOX_CUSTOM_e,  // cullType
};
