/**
 * @file d_a_npc_doorboy.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_npc_doorboy.h"
#include "d/actor/d_a_npc.h"

enum DoorBoy_RES_File_ID {
    /* BMDR */
    /* 0x4 */ BMDR_DOORBOY = 0x4,

    /* BTP */
    /* 0x7 */ BTP_DOORBOY = 0x7,
};

enum Mgeneral_RES_File_ID {
    /* BCK */
    /* 0x03 */ BCK_M_BROWSE_A = 0x3,
    /* 0x04 */ BCK_M_BROWSE_B,
    /* 0x05 */ BCK_M_LOOK_A,
    /* 0x06 */ BCK_M_LOOK_B,
    /* 0x07 */ BCK_M_MARO_DANCE,
    /* 0x08 */ BCK_M_RUN_A,
    /* 0x09 */ BCK_M_RUN_B,
    /* 0x0A */ BCK_M_SING,
    /* 0x0B */ BCK_M_SIT_TO_WOLF_A,
    /* 0x0C */ BCK_M_SIT_TO_WOLF_B,
    /* 0x0D */ BCK_M_SITTALK_A,
    /* 0x0E */ BCK_M_SITTALK_A_B,
    /* 0x0F */ BCK_M_SITTALK_B,
    /* 0x10 */ BCK_M_SITTALK_B_B,
    /* 0x11 */ BCK_M_SITWAIT_A,
    /* 0x12 */ BCK_M_SITWAIT_B,
    /* 0x13 */ BCK_M_SURPRISE,
    /* 0x14 */ BCK_M_TALK_A,
    /* 0x15 */ BCK_M_TALK_B,
    /* 0x16 */ BCK_M_TALK_B_WALL,
    /* 0x17 */ BCK_M_TALK_C,
    /* 0x18 */ BCK_M_TALK_WALL,
    /* 0x19 */ BCK_M_TO_WOLF,
    /* 0x1A */ BCK_M_WAIT_A,
    /* 0x1B */ BCK_M_WAIT_B,
    /* 0x1C */ BCK_M_WAIT_WALL,
    /* 0x1D */ BCK_M_WALK_A,
    /* 0x1E */ BCK_M_WALK_B,
};

enum RES_Name {
    /* 0x0 */ DOORBOY,
    /* 0x1 */ MGENERAL,
};

enum Joint {
    /* 0x00 */ JNT_CENTER,
    /* 0x01 */ JNT_BACKBONE,
    /* 0x02 */ JNT_NECK,
    /* 0x03 */ JNT_HEAD,
    /* 0x04 */ JNT_SHOULDERL,
    /* 0x05 */ JNT_ARML1,
    /* 0x06 */ JNT_ARML2,
    /* 0x07 */ JNT_HANDL,
    /* 0x08 */ JNT_SHOULDERR,
    /* 0x09 */ JNT_ARMR1,
    /* 0x0A */ JNT_ARMR2,
    /* 0x0B */ JNT_HANDR,
    /* 0x0C */ JNT_WAIST,
    /* 0x0D */ JNT_LEGL1,
    /* 0x0E */ JNT_LEGL2,
    /* 0x0F */ JNT_FOOTL,
    /* 0x10 */ JNT_LEGR1,
    /* 0x11 */ JNT_LEGR2,
    /* 0x12 */ JNT_FOOTR,
};

enum Animation {
    /* 0x0 */ ANM_WAIT_A,
    /* 0x1 */ ANM_TALK_A,
    /* 0x2 */ ANM_TALK_B,
    /* 0x3 */ ANM_TALK_C,
    /* 0x4 */ ANM_SING,
    /* 0x5 */ ANM_SIT_TO_WOLF_A,
};

enum Motion {
    /* 0x0 */ MOT_WAIT_A,
    /* 0x1 */ MOT_SING,
    /* 0x2 */ MOT_TALK_B,
    /* 0x3 */ MOT_TALK_A,
    /* 0xA */ MOT_TALK_C = 0xA,
    /* 0xB */ MOT_SIT_TO_WOLF_A,
};

static daNpc_GetParam2 l_bckGetParamList[6] = {
    {BCK_M_WAIT_A, J3DFrameCtrl::EMode_LOOP, MGENERAL},
    {BCK_M_TALK_A, J3DFrameCtrl::EMode_LOOP, MGENERAL},
    {BCK_M_TALK_B, J3DFrameCtrl::EMode_LOOP, MGENERAL},
    {BCK_M_TALK_C, J3DFrameCtrl::EMode_LOOP, MGENERAL},
    {BCK_M_SING, J3DFrameCtrl::EMode_LOOP, MGENERAL},
    {BCK_M_SIT_TO_WOLF_A, J3DFrameCtrl::EMode_LOOP, MGENERAL},
};

static daNpc_GetParam2 l_btpGetParamList[1] = {
    {BTP_DOORBOY, J3DFrameCtrl::EMode_LOOP, DOORBOY},
};

static char* l_arcNames[2] = {
    "DoorBoy",
    "Mgeneral",
};

static char* l_evtNames[1] = {NULL};

static char* l_myName = "DoorBoy";

static daNpcDoorBoy_Param_c l_HIO;

daNpcDoorBoy_c::EventFn daNpcDoorBoy_c::mEvtSeqList[1] = {
    NULL
};

daNpcDoorBoy_c::daNpcDoorBoy_c() {}

daNpcDoorBoy_c::~daNpcDoorBoy_c() {
    for (int i = 0; i < 2; i++) {
        dComIfG_resDelete(&mPhases[i], l_arcNames[i]);
    }

    if (heap != NULL) {
        mAnm_p->stopZelAnime();
    }
}

daNpcDoorBoy_HIOParam const daNpcDoorBoy_Param_c::m = {
    55.0f,
    0.0f,
    1.0f,
    300.0f,
    255.0f,
    200.0f,
    35.0f,
    30.0f,
    0.0f,
    0.0f,
    20.0f,
    -20.0f,
    30.0f,
    -10.0f,
    45.0f,
    -45.0f,
    0.6f,
    12.0f,
    3,
    4,
    4,
    2,
    60.0f,
    500.0f,
    300.0f,
    -300.0f,
    60,
    0,
    0,
    0,
    0,
    false,
    false,
};

cPhs__Step daNpcDoorBoy_c::Create() {
    fopAcM_ct(this, daNpcDoorBoy_c);

    mMessageNo = (s16)getMessageNo();

    cPhs__Step phase = cPhs_ERROR_e;
    for (int i = 0; i < 2; i++) {
        phase = (cPhs__Step)dComIfG_resLoad(&mPhases[i], l_arcNames[i]);

        if (phase != cPhs_COMPLEATE_e) {
            return phase;
        }
    }

    if (phase == cPhs_COMPLEATE_e) {
        if (!fopAcM_entrySolidHeap(this, createHeapCallBack, 0x1860)) {
            return cPhs_ERROR_e;
        }

        fopAcM_SetMtx(this, mAnm_p->getModel()->getBaseTRMtx());
        fopAcM_setCullSizeBox(this, -50.0f, -10.0f, -50.0f, 50.0f, 210.0f, 50.0f);
        mSound.init(&current.pos, &eyePos, 3, 1);
        mSound.setMdlType(5, false, dKy_darkworld_check());

        mAcchCir.SetWall(daNpcDoorBoy_Param_c::m.common.width, daNpcDoorBoy_Param_c::m.common.knee_length);
        mAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1, &mAcchCir, fopAcM_GetSpeed_p(this),
                fopAcM_GetAngle_p(this), fopAcM_GetShapeAngle_p(this));
        mAcch.CrrPos(dComIfG_Bgsp());
        mCcStts.Init(daNpcDoorBoy_Param_c::m.common.weight, 0, this);

        field_0xc98.Set(mCcDCyl);
        field_0xc98.SetStts(&mCcStts);
        field_0xc98.SetTgType(0);
        field_0xc98.SetTgSPrm(0);
        field_0xc98.SetH(daNpcDoorBoy_Param_c::m.common.height);
        field_0xc98.SetR(daNpcDoorBoy_Param_c::m.common.width);

        mGndChk = mAcch.m_gnd;
        mGroundH = mAcch.GetGroundH();
        setEnvTevColor();
        setRoomNo();
        reset();
        Execute();
    }

    return phase;
}

int daNpcDoorBoy_c::CreateHeap() {
    J3DModelData* mdlData_p = (J3DModelData*)dComIfG_getObjectRes(l_arcNames[0], BMDR_DOORBOY);

    JUT_ASSERT(292, NULL != mdlData_p);

    mAnm_p = new mDoExt_McaMorfSO(mdlData_p, NULL, NULL, NULL, -1, 1.0f, 0, -1, &mSound, 0x80000, 0x11020084);
    if (mAnm_p != NULL && mAnm_p->getModel() == NULL) {
        mAnm_p->stopZelAnime();
        mAnm_p = NULL;
    }

    if (mAnm_p == NULL) {
        return 0;
    }

    cXyz translateScale(1.0f, 1.0820773f, 1.0f);
    mAnm_p->offTranslate();
    mAnm_p->setTranslateScale(translateScale);

    for (u16 i = 0; i < mdlData_p->getJointNum(); i++) {
        mdlData_p->getJointNodePointer(i)->setCallBack(ctrlJointCallBack);
    }
    mAnm_p->getModel()->setUserArea((uintptr_t)this);
    
    setMotion(MOT_WAIT_A, -1.0f, 0);

    return 1;
}

int daNpcDoorBoy_c::Delete() {
    this->~daNpcDoorBoy_c();
    return 1;
}

int daNpcDoorBoy_c::Execute() {
    execute();
    return 1;
}

int daNpcDoorBoy_c::Draw() {
    draw(FALSE, FALSE, daNpcDoorBoy_Param_c::m.common.real_shadow_size, NULL, FALSE);
    dComIfGd_setSimpleShadow(&current.pos, mAcch.GetGroundH(), 50.0f, mAcch.m_gnd, 0, 1.0f, dDlst_shadowControl_c::getSimpleTex());
    return 1;
}

int daNpcDoorBoy_c::ctrlJoint(J3DJoint* i_joint, J3DModel* i_model) {
    int jointNo = i_joint->getJntNo();
    int i_jointList[3] = {JNT_BACKBONE, JNT_NECK, JNT_HEAD};

    if (jointNo == JNT_CENTER) {
        mDoMtx_stack_c::copy(mAnm_p->getModel()->getAnmMtx(JNT_BACKBONE));
        mDoMtx_stack_c::multVecZero(&mLookatPos[0]);
        mDoMtx_stack_c::copy(mAnm_p->getModel()->getAnmMtx(JNT_NECK));
        mDoMtx_stack_c::multVecZero(&mLookatPos[1]);
        mDoMtx_stack_c::copy(mAnm_p->getModel()->getAnmMtx(JNT_HEAD));
        mDoMtx_stack_c::multVecZero(&mLookatPos[2]);
    }

    mDoMtx_stack_c::copy(i_model->getAnmMtx(jointNo));

    switch (jointNo) {
        case JNT_BACKBONE:
        case JNT_NECK:
        case JNT_HEAD:
            setLookatMtx(jointNo, i_jointList, daNpcDoorBoy_Param_c::m.common.neck_rotation_ratio);
    }

    i_model->setAnmMtx(jointNo, mDoMtx_stack_c::get());
    MTXCopy(mDoMtx_stack_c::get(), J3DSys::mCurrentMtx);

    return 1;
}

int daNpcDoorBoy_c::createHeapCallBack(fopAc_ac_c* a_this) {
    daNpcDoorBoy_c* i_this = (daNpcDoorBoy_c*)a_this;
    return i_this->CreateHeap();
}

int daNpcDoorBoy_c::ctrlJointCallBack(J3DJoint* i_joint, int param_2) {
    if (param_2 == 0) {
        J3DModel* model = j3dSys.getModel();
        daNpcDoorBoy_c* i_this = (daNpcDoorBoy_c*)model->getUserArea();

        if (i_this != NULL) {
            i_this->ctrlJoint(i_joint, model);
        }
    }

    return 1;
}

void daNpcDoorBoy_c::setMotion(int i_motion, f32 i_morf, int param_3) {
    s16 motion = i_motion;

    if ((param_3 != 0 || mMotion != motion) && i_motion >= 0 && i_motion < 0xC) {
        mMotion = motion;
        mMotionMorfOverride = i_morf;
        mMotionPrevPhase = -1;
        mMotionPhase = 0;
        field_0xe02 = 0;
    }
}

void daNpcDoorBoy_c::reset() {
    initialize();
    mLookat.initialize();

    for (int i = 0; i < 1; i++) {
        mActorMngr[i].initialize();
    }

    field_0xdf4 = 0;
    field_0xdf8 = 0;
    mAction = NULL;
    mLookMode = -1;
    mMode = 0;
    current.pos.set(home.pos);
    old.pos.set(current.pos);
    current.angle.set(0, home.angle.y, 0);
    old.angle = current.angle;
    shape_angle = current.angle;
    mCurAngle = current.angle;
    mOldAngle = mCurAngle;
    speedF = 0.0f;
    speed.setall(0.0f);
    field_0xdf0 = -1;
    mOrderEvtNo = 0;

    J3DAnmTexPattern* i_btp = getTexPtrnAnmP(l_arcNames[0], 7);
    mAnmFlags &= 0xFFFFF57F;

    if (setBtpAnm(i_btp, mAnm_p->getModel()->getModelData(), 1.0f, 2)) {
        mAnmFlags |= 0xA80;
    }

    setAction(&daNpcDoorBoy_c::wait);
    mMotionMorfOverride = 0.0f;
}

BOOL daNpcDoorBoy_c::setAction(actionFunc action) {
    mMode = 3;

    if (mAction) {
        (this->*mAction)(NULL);
    }

    mMode = 0;
    mAction = action;

    if (mAction) {
        (this->*mAction)(NULL);
    }

    return TRUE;
}

inline void daNpcDoorBoy_c::playMotion() {
    daNpcF_anmPlayData dat0 = {ANM_WAIT_A, daNpcDoorBoy_Param_c::m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat0[1] = {&dat0};
    daNpcF_anmPlayData dat1 = {ANM_SING, daNpcDoorBoy_Param_c::m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat1[1] = {&dat1};
    daNpcF_anmPlayData dat2 = {ANM_TALK_B, daNpcDoorBoy_Param_c::m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat2[1] = {&dat2};
    daNpcF_anmPlayData dat3 = {ANM_TALK_A, daNpcDoorBoy_Param_c::m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat3[1] = {&dat3};
    daNpcF_anmPlayData dat4 = {ANM_TALK_C, daNpcDoorBoy_Param_c::m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat4[1] = {&dat4};
    daNpcF_anmPlayData dat5 = {ANM_SIT_TO_WOLF_A, daNpcDoorBoy_Param_c::m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat5[1] = {&dat5};
    daNpcF_anmPlayData** ppDat[12] = {
        pDat0,
        pDat1,
        pDat2,
        pDat3,
        pDat0,
        pDat0,
        pDat0,
        pDat0,
        pDat0,
        pDat0,
        pDat4,
        pDat5,
    };
    
    if (mMotion >= 0 && mMotion < 0xC) {
        field_0xe02 = mMotionPhase;
        playMotionAnm(ppDat);
    }
}

inline void daNpcDoorBoy_c::lookat() {
    daPy_py_c* player = NULL;
    J3DModel* model_p = mAnm_p->getModel();

    int iVar1 = 0;
    f32 body_angleX_min = daNpcDoorBoy_Param_c::m.common.body_angleX_min;
    f32 body_angleX_max = daNpcDoorBoy_Param_c::m.common.body_angleX_max;
    f32 body_angleY_min = daNpcDoorBoy_Param_c::m.common.body_angleY_min;
    f32 body_angleY_max = daNpcDoorBoy_Param_c::m.common.body_angleY_max;
    f32 head_angleX_min = daNpcDoorBoy_Param_c::m.common.head_angleX_min;
    f32 head_angleX_max = daNpcDoorBoy_Param_c::m.common.head_angleX_max;
    f32 head_angleY_min = daNpcDoorBoy_Param_c::m.common.head_angleY_min;
    f32 head_angleY_max = daNpcDoorBoy_Param_c::m.common.head_angleY_max;

    s16 temp_r26 = mCurAngle.y - mOldAngle.y;
    cXyz sp30[] = {mLookatPos[0], mLookatPos[1], mLookatPos[2]};
    csXyz* sp24[] = {&mLookatAngle[0], &mLookatAngle[1], &mLookatAngle[2]};

    cXyz sp18;
    switch (mLookMode) {
    case LOOK_NONE:
        break;
    case LOOK_RESET:
        iVar1 = 1;
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
    mLookat.calc(this, model_p->getBaseTRMtx(), sp24, iVar1, temp_r26, 0);
}

inline bool daNpcDoorBoy_c::chkFindPlayer() {
    bool rv;
    if (!chkActorInSight(daPy_getPlayerActorClass(), daNpcDoorBoy_Param_c::m.common.fov)) {
        mActorMngr[0].remove();
        return false;
    }

    if (mActorMngr[0].getActorP() == NULL) {
        rv = chkPlayerInSpeakArea(this);
    } else {
        rv = chkPlayerInTalkArea(this);
    }

    if (rv) {
        mActorMngr[0].entry(daPy_getPlayerActorClass());
    } else {
        mActorMngr[0].remove();
    }

    return rv;
}

inline void daNpcDoorBoy_c::setLookMode(int i_lookMode) {
    s16 lookMode = i_lookMode;

    if (i_lookMode >= 0 && i_lookMode < 4 && lookMode != mLookMode) {
        mLookMode = lookMode;
    }
}

inline BOOL daNpcDoorBoy_c::step(s16 i_turnTargetAngle, int param_2) {
    if (mTurnMode == 0) {
        mTurnTargetAngle = i_turnTargetAngle;
        mTurnAmount = 0;

        if (mCurAngle.y == mTurnTargetAngle) {
            mTurnMode++;
        }

        current.angle.y = mCurAngle.y;
        shape_angle.y = current.angle.y;
        mTurnMode++;
    } else if (mTurnMode == 1) {
        if (turn(mTurnTargetAngle, 15.0f, 0)) {
            shape_angle.y = current.angle.y;
            mCurAngle.y = current.angle.y;
            mOldAngle.y = current.angle.y;
            mTurnMode++;
        } else {
            shape_angle.y = current.angle.y;
            mCurAngle.y = current.angle.y;
        }
    }

    return mTurnMode > 1;
}

inline int daNpcDoorBoy_c::getTimeHour() {
    if (dKy_darkworld_check()) {
        return dKy_getDarktime_hour();
    }

    return dKy_getdaytime_hour();
}

inline bool daNpcDoorBoy_c::isDummyTalk() {
    /* dSv_event_flag_c::M_091 - Kakariko Village - Buy out fundraiser amount (Malo becomes nice) */
    if (dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[124])) {
        return false;
    }

    int timeHour = getTimeHour();
    if (timeHour > 10 && timeHour < 20) {
        if (dComIfGs_isOneZoneSwitch(0, fopAcM_GetRoomNo(this))) {
            return false;
        }
    }

    return true;
}

bool daNpcDoorBoy_c::wait(void* param_1) {
    switch (mMode) {
        case 0:
            setMotion(MOT_WAIT_A, -1.0f, 0);
            mTurnMode = 0;
            speedF = 0.0f;
            mMode = 2;
            break;

        case 2:
            chkFindPlayer();
            if (mActorMngr[0].getActorP() != NULL) {
                setLookMode(LOOK_PLAYER);
            } else {
                setLookMode(LOOK_NONE);

                if (mCurAngle.y != home.angle.y && step(home.angle.y, 0)) {
                    setMotion(MOT_WAIT_A, -1.0f, 0);
                    mTurnMode = 0;
                }
            }

            if (dComIfGp_event_runCheck()) {
                if (eventInfo.checkCommandTalk()) {
                    if (!dComIfGp_event_chkTalkXY() || dComIfGp_evmng_ChkPresentEnd()) {
                        setAction(&daNpcDoorBoy_c::talk);
                    }
                } else {
                    if (dMsgObject_getNowTalkFlowNo() == 0x435) {
                        if (isDummyTalk()) {
                            setAction(&daNpcDoorBoy_c::dummyTalk);
                        }
                    } else {
                        if (dComIfGp_getEventManager().getMyStaffId(l_myName, NULL, 0) != -1) {
                            setAction(&daNpcDoorBoy_c::demo);
                        }
                    }
                }
            } else {
                if (!dKy_darkworld_check()) {
                    if (daPy_py_c::checkNowWolf()) {
                        f32 fVar1 = pow(500.0, 2.0);
                        if (fopAcM_searchPlayerDistanceXZ2(this) < fVar1) {
                            setAction(&daNpcDoorBoy_c::fear);
                            break;
                        }
                    }
                }

                if (mOrderEvtNo != 0) {
                    eventInfo.setArchiveName(l_arcNames[0]);
                }

                orderEvent(field_0xe06, l_evtNames[mOrderEvtNo], 0xFFFF, 40, 0xFF, 1);
            }
            break;

        case 3:
            break;

        default:
            JUT_ASSERT(1064, FALSE);
            break;
    }

    return true;
}

bool daNpcDoorBoy_c::fear(void* param_1) {
    switch (mMode) {
        case 0:
            setMotion(MOT_SIT_TO_WOLF_A, -1.0f, 0);
            speedF = 0.0f;
            mMode = 2;
            break;

        case 2:
            if (mAnm_p->checkFrame(1.0f)) {
                mSound.playVoice(2);
            }
            break;

        case 3:
            break;

        default:
            JUT_ASSERT(1094, FALSE);
            break;
    }

    return true;
}

bool daNpcDoorBoy_c::talk(void* param_1) {
    int itemNo;
    int iVar1, i_motion;
    bool rv = false;

    switch (mMode) {
        case 0:
            initTalk(mMessageNo, NULL);
            mMsgTimer = 0;
            field_0xe06 = 0;
            setLookMode(LOOK_PLAYER_TALK);
            mMode = 2;
            break;

        case 2:
            if (mCurAngle.y == fopAcM_searchPlayerAngleY(this)) {
                if (talkProc(NULL, TRUE, NULL)) {
                    mActorMngr[0].entry(daPy_getPlayerActorClass());
                    itemNo = 0;

                    OS_REPORT("会話終了時 イベントID=%d アイテムNo=%d\n", mFlow.getEventId(&itemNo), itemNo);

                    if (mFlow.getEventId(&itemNo) == 1) {
                        field_0xdf0 = fopAcM_createItemForPresentDemo(&current.pos, itemNo, 0, -1, -1, NULL, NULL);
                        if (field_0xdf0 != fpcM_ERROR_PROCESS_ID_e) {
                            s16 eventIdx = dComIfGp_getEventManager().getEventIdx(this, "DEFAULT_GETITEM", 0xFF);
                            dComIfGp_getEvent().reset(this);
                            fopAcM_orderChangeEventId(this, eventIdx, 1, 0xFFFF);
                            field_0x9ec = true;
                            setAction(&daNpcDoorBoy_c::wait);
                        }
                    } else {
                        setAction(&daNpcDoorBoy_c::wait);
                    }

                    rv = true;
                } else if (ctrlMsgAnm(iVar1, i_motion, this, FALSE) != 0) {
                    setMotion(i_motion, -1.0f, 0);
                }
            } else if (step(fopAcM_searchPlayerAngleY(this), 0)) {
                setMotion(MOT_WAIT_A, -1.0f, 0);
                mTurnMode = 0;
            }
            break;

        case 3:
            if (!field_0x9ec) {
                dComIfGp_event_reset();
            }
            break;

        default:
            JUT_ASSERT(1167, FALSE);
            break;
    }

    return rv;
}

bool daNpcDoorBoy_c::demo(void* param_1) {
    switch (mMode) {
        case 0:
            setMotion(MOT_WAIT_A, -1.0f, 0);
            mMode = 2;
            break;

        case 2:
            if (dComIfGp_event_runCheck() != 0 && !eventInfo.checkCommandTalk()) {
                dEvent_manager_c& eventManager = dComIfGp_getEventManager();
                s32 staffId = eventManager.getMyStaffId(l_myName, NULL, 0);
                if (staffId != -1) {
                    mStaffID = staffId;

                    JUT_ASSERT(1201, NULL != mEvtSeqList[mOrderEvtNo]);

                    if ((this->*mEvtSeqList[mOrderEvtNo])(staffId)) {
                        eventManager.cutEnd(staffId);
                    }
                }

                if (eventInfo.checkCommandDemoAccrpt() && mEventIdx != -1 && eventManager.endCheck(mEventIdx)) {
                    dComIfGp_event_reset();
                    mOrderEvtNo = 0;
                    mEventIdx = -1;
                    setAction(&daNpcDoorBoy_c::wait);
                }
            }
            break;

        case 3:
            break;

        default:
            JUT_ASSERT(1229, FALSE);
            break;
    }

    return true;
}

bool daNpcDoorBoy_c::dummyTalk(void* param_1) {
    switch (mMode) {
        case 0:
            setMotion(MOT_WAIT_A, -1.0f, 0);
            mTurnMode = 0;
            mMode = 2;
            break;

        case 2:
            if (step(fopAcM_searchPlayerAngleY(this), 0)) {
                setMotion(MOT_TALK_A, -1.0f, 0);
            }

            if (!dComIfGp_event_runCheck()) {
                setAction(&daNpcDoorBoy_c::wait);
            }
            break;

        case 3:
            break;

        default:
            JUT_ASSERT(1263, FALSE);
            break;
    }

    return true;
}

static int daNpcDoorBoy_Create(void* a_this) {
    daNpcDoorBoy_c* i_this = (daNpcDoorBoy_c*)a_this;
    return i_this->Create();
}

static int daNpcDoorBoy_Delete(void* a_this) {
    daNpcDoorBoy_c* i_this = (daNpcDoorBoy_c*)a_this;
    return i_this->Delete();
}

static int daNpcDoorBoy_Execute(void* a_this) {
    daNpcDoorBoy_c* i_this = (daNpcDoorBoy_c*)a_this;
    return i_this->Execute();
}

static int daNpcDoorBoy_Draw(void* a_this) {
    daNpcDoorBoy_c* i_this = (daNpcDoorBoy_c*)a_this;
    return i_this->Draw();
}

static int daNpcDoorBoy_IsDelete(void* a_this) {
    return 1;
}

void daNpcDoorBoy_c::setParam() {
    attention_info.distances[fopAc_attn_LOCK_e] = getDistTableIdx(daNpcDoorBoy_Param_c::m.common.attention_distance,
                                                                  daNpcDoorBoy_Param_c::m.common.attention_angle);
    attention_info.distances[fopAc_attn_TALK_e] = attention_info.distances[fopAc_attn_LOCK_e];
    attention_info.distances[fopAc_attn_SPEAK_e] = getDistTableIdx(daNpcDoorBoy_Param_c::m.common.talk_distance,
                                                                   daNpcDoorBoy_Param_c::m.common.talk_angle);
    attention_info.flags = daPy_py_c::checkNowWolf() ? 0 : (fopAc_AttnFlag_SPEAK_e | fopAc_AttnFlag_TALK_e);
    scale.setall(daNpcDoorBoy_Param_c::m.common.scale);
    mAcchCir.SetWallR(daNpcDoorBoy_Param_c::m.common.width);
    mAcchCir.SetWallH(daNpcDoorBoy_Param_c::m.common.knee_length);
    gravity = daNpcDoorBoy_Param_c::m.common.gravity;
}

BOOL daNpcDoorBoy_c::main() {
    if (mAction) {
        (this->*mAction)(NULL);
    }

    playMotion();
    return FALSE;
}

void daNpcDoorBoy_c::setAttnPos() {
    if (mLookMode == 1) {
        for (int i = 0; i < 3; i++) {
            mLookatAngle[i].setall(0);
        }
    }

    setMtx();
    lookat();

    mDoMtx_stack_c::copy(mAnm_p->getModel()->getAnmMtx(JNT_HEAD));
    mDoMtx_stack_c::multVecZero(&mHeadPos);
    cXyz sp1c(23.0f, 15.0f, 0.0f);
    mDoMtx_stack_c::multVec(&sp1c, &eyePos);
    sp1c.x = 0.0f;
    mDoMtx_stack_c::multVec(&sp1c, &sp1c);
    mHeadAngle.x = cLib_targetAngleX(&mHeadPos, &sp1c);
    mHeadAngle.y = cLib_targetAngleY(&mHeadPos, &sp1c);
    attention_info.position.set(55.0f, 0.0f, 0.0f);
    mDoMtx_stack_c::multVec(&attention_info.position, &attention_info.position);

    cXyz sp28;
    mDoMtx_stack_c::copy(mAnm_p->getModel()->getAnmMtx(JNT_BACKBONE));
    mDoMtx_stack_c::multVecZero(&sp28);
    sp28.y = current.pos.y;
    
    field_0xc98.SetC(sp28);
    #if DEBUG
    field_0xc98.SetH(daNpcDoorBoy_Param_c::m.common.height);
    field_0xc98.SetR(daNpcDoorBoy_Param_c::m.common.width);
    #endif
    dComIfG_Ccsp()->Set(&field_0xc98);
}

void daNpcDoorBoy_c::setMotionAnm(int i_index, f32 i_morf) {
    J3DAnmTransformKey* anm = getTrnsfrmKeyAnmP(l_arcNames[l_bckGetParamList[i_index].arcIdx], l_bckGetParamList[i_index].fileIdx);
    int i_attr = l_bckGetParamList[i_index].attr;
    mAnmFlags &= 0xFFFFFFF6;

    if (anm != NULL && setMcaMorfAnm(anm, 1.0f, i_morf, i_attr, 0, -1)) {
        mAnmFlags |= 9;
        mMotionLoops = 0;
    }
}

BOOL daNpcDoorBoy_c::drawDbgInfo() {
    return FALSE;
}

static actor_method_class daNpcDoorBoy_MethodTable = {
    (process_method_func)daNpcDoorBoy_Create,
    (process_method_func)daNpcDoorBoy_Delete,
    (process_method_func)daNpcDoorBoy_Execute,
    (process_method_func)daNpcDoorBoy_IsDelete,
    (process_method_func)daNpcDoorBoy_Draw,
};

extern actor_process_profile_definition g_profile_NPC_DOORBOY = {
  fpcLy_CURRENT_e,           // mLayerID
  7,                         // mListID
  fpcPi_CURRENT_e,           // mListPrio
  PROC_NPC_DOORBOY,          // mProcName
  &g_fpcLf_Method.base,     // sub_method
  sizeof(daNpcDoorBoy_c),    // mSize
  0,                         // mSizeOther
  0,                         // mParameters
  &g_fopAc_Method.base,      // sub_method
  418,                       // mPriority
  &daNpcDoorBoy_MethodTable, // sub_method
  0x00044187,                // mStatus
  fopAc_NPC_e,               // mActorType
  fopAc_CULLBOX_CUSTOM_e,    // cullType
};
