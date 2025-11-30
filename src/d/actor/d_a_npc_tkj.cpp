/**
 * @file d_a_npc_tkj.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_npc_tkj.h"
#include "d/actor/d_a_npc4.h"

static int l_bmdData[1][2] = {
    {12, 1},
};

static daNpcT_evtData_c l_evtList[2] = {
    {"", 0},
    {"DEFAULT_GETITEM", 0},
};

static char* l_resNameList[2] = {
    "",
    "Tkj",
};

static s8 l_loadResPtrn0[] = {
    1, -1,
};

static s8* l_loadResPtrnList[2] = {
    l_loadResPtrn0,
    l_loadResPtrn0,
};

static daNpcT_faceMotionAnmData_c l_faceMotionAnmData[2] = {
    {-1, 0, 0, 18, 2, 1, 1},
    {6, 0, 1, 18, 2, 1, 1},
};

static daNpcT_motionAnmData_c l_motionAnmData[2] = {
    {9, 2, 1, 15, 0, 1, 0, 0},
    {8, 2, 1, 15, 0, 1, 0, 0},
};

static daNpcT_MotionSeqMngr_c::sequenceStepData_c l_faceMotionSequenceData[8] = {
    {1, -1, 0},
    {-1, 0, 0},
    {-1, 0, 0},
    {-1, 0, 0},
    {0, -1, 0},
    {-1, 0, 0},
    {-1, 0, 0},
    {-1, 0, 0},
};

static daNpcT_MotionSeqMngr_c::sequenceStepData_c l_motionSequenceData[8] = {
    {0, -1, 0},
    {-1, 0, 0},
    {-1, 0, 0},
    {-1, 0, 0},
    {1, -1, 0},
    {-1, 0, 0},
    {-1, 0, 0},
    {-1, 0, 0},
};

char* daNpcTkj_c::mCutNameList[2] = {
    "",
    "LV7_FIRST_START",
};

int (daNpcTkj_c::*daNpcTkj_c::mCutList[])(int) = {
    NULL,
    &daNpcTkj_c::cutLv7Start,
};

daNpcTkj_c::~daNpcTkj_c() {
    OS_REPORT("|%06d:%x|daNpcTkj_c -> デストラクト\n", g_Counter.mCounter0, this);

    if (heap != NULL) {
        mpMorf[0]->stopZelAnime();
    }

    deleteRes(l_loadResPtrnList[mType], (const char**)l_resNameList);
}

const daNpc_Tkj_HIOParam daNpc_Tkj_Param_c::m = {
    120.0f,
    -3.0f,
    1.0f,
    400.0f,
    255.0f,
    100.0f,
    35.0f,
    20.0f,
    0.0f,
    0.0f,
    30.0f,
    -30.0f,
    30.0f,
    -10.0f,
    45.0f,
    -45.0f,
    0.6f,
    12.0f,
    5,
    6,
    7,
    6,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    60,
    8,
    0,
    0,
    0,
    0,
    0,
    4.0f,
    -20.0f,
    0.0f,
    -20.0f,
    20.0f,
    40.0f,
    20.0f,
    110.0f,
};

int daNpcTkj_c::create() {
    daNpcT_ct(this, daNpcTkj_c, l_faceMotionAnmData, l_motionAnmData,
                       l_faceMotionSequenceData, 4, l_motionSequenceData, 4,
                       l_evtList, l_resNameList);

    mType = getType();
    mFlowNodeNo = getFlowNodeNo();
    mTwilight = FALSE;

    if (mType == 1 && !daNpcF_chkEvtBit(0x10C) && dComIfGs_isDungeonItemWarp()) {
        return 5;
    }

    int phase_state = loadRes(l_loadResPtrnList[mType], (const char**)l_resNameList);
    if (phase_state == 4) {
        if (!fopAcM_entrySolidHeap(this, createHeapCallBack, 0x2FE0)) {
            return 5;
        }

        OS_REPORT("\t(%s:%d) flowNo:%d, PathNo:%02x<%08x> ", fopAcM_getProcNameString(this), mType, mFlowNodeNo, getPath(), fopAcM_GetParam(this));

        if (isDelete()) {
            OS_REPORT("===>isDelete:TRUE\n");
            return 5;
        }

        OS_REPORT("\n");

        J3DModelData* modelData = mpMorf[0]->getModel()->getModelData();
        fopAcM_SetMtx(this, mpMorf[0]->getModel()->getBaseTRMtx());
        fopAcM_setCullSizeBox(this, -300.0f, -50.0f, -300.0f, 300.0f, 450.0f, 300.0f);
        
        mSound.init(&current.pos, &eyePos, 3, 1);

        mAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1, &mAcchCir, fopAcM_GetSpeed_p(this), fopAcM_GetAngle_p(this), fopAcM_GetShapeAngle_p(this));
        mAcch.CrrPos(dComIfG_Bgsp());
        mGndChk = mAcch.m_gnd;
        mGroundH = mAcch.GetGroundH();

        setEnvTevColor();
        setRoomNo();

        mCcStts.Init(daNpc_Tkj_Param_c::m.common.weight, 0, this);
        mCcCyl.Set(mCcDCyl);
        mCcCyl.SetStts(&mCcStts);
        mCcCyl.SetTgHitCallback(tgHitCallBack);

        reset();
        mCreating = TRUE;
        Execute();
        mCreating = FALSE;
    }

    return phase_state;
}

int daNpcTkj_c::CreateHeap() {
    enum { IDX0 };
    int temp_r23 = mTwilight == TRUE ? IDX0 : IDX0;  // needs enum to match
    const char* arcname = l_resNameList[l_bmdData[temp_r23][1]];
    int bmd_index = l_bmdData[temp_r23][0];

    J3DModelData* mdlData_p = (J3DModelData*)dComIfG_getObjectRes(arcname, bmd_index);
    JUT_ASSERT(446, NULL != mdlData_p);

    u32 diff_flags = J3D_DIFF_FLAG(FALSE, FALSE, TRUE, 8, 2, FALSE, 2, 0, TRUE, FALSE, FALSE, FALSE, TRUE, FALSE);
    mpMorf[0] = new mDoExt_McaMorfSO(mdlData_p, NULL, NULL, NULL, -1, 1.0f, 0, -1, &mSound, J3DMdlFlag_DifferedDLBuffer, diff_flags);
    if (mpMorf[0] != NULL && mpMorf[0]->getModel() == NULL) {
        mpMorf[0]->stopZelAnime();
        mpMorf[0] = NULL;
    }

    if (mpMorf[0] == NULL) {
        return 0;
    }

    J3DModel* mdl_p = mpMorf[0]->getModel();
    for (u16 i = 0; i < mdlData_p->getJointNum(); i++) {
        mdlData_p->getJointNodePointer(i)->setCallBack(ctrlJointCallBack);
    }

    mdl_p->setUserArea((uintptr_t)this);

    mpMatAnm[0] = new daNpcT_MatAnm_c();
    if (mpMatAnm[0] == NULL) {
        return 0;
    }

    if (setFaceMotionAnm(1, 0) && setMotionAnm(0, 0.0f, 0)) {
        return 1;
    }

    return 0;
}

int daNpcTkj_c::Delete() {
    OS_REPORT("|%06d:%x|daNpcTkj_c -> Delete\n", g_Counter.mCounter0, this);
    fpc_ProcID id = fopAcM_GetID(this);
    this->~daNpcTkj_c();
    return 1;
}

int daNpcTkj_c::Execute() {
    return execute();
}

int daNpcTkj_c::Draw() {
    if (mpMatAnm[0] != NULL) {
        J3DModelData* mdlData_p = mpMorf[0]->getModel()->getModelData();
        mdlData_p->getMaterialNodePointer(getEyeballMaterialNo())->setMaterialAnm(mpMatAnm[0]);
    }

    draw(FALSE, 0, daNpc_Tkj_Param_c::m.common.real_shadow_size, NULL, 100.0f, FALSE, FALSE, FALSE);
}

int daNpcTkj_c::createHeapCallBack(fopAc_ac_c* i_this) {
    return ((daNpcTkj_c*)i_this)->CreateHeap();
}

int daNpcTkj_c::ctrlJointCallBack(J3DJoint* i_joint, int param_1) {
    if (param_1 == 0) {
        J3DModel* mdl_p = j3dSys.getModel();
        daNpcTkj_c* i_this = (daNpcTkj_c*)mdl_p->getUserArea();
        if (i_this != NULL) {
            i_this->ctrlJoint(i_joint, mdl_p);
        }
    }

    return 1;
}

int daNpcTkj_c::getType() {
    switch (fopAcM_GetParam(this) & 0xFF) {
    case 0:
        return 0;
    case 1:
        return 1;
    default:
        return 0;
    }
}

int daNpcTkj_c::getFlowNodeNo() {
    u16 prm = home.angle.x;
    return prm == 0xFFFF ? -1 : prm;
}

int daNpcTkj_c::isDelete() {
    if (mType < 2) {
        return 0;
    }

    return 1;
}

void daNpcTkj_c::reset() {
    initialize();
    memset(&field_0xf84, 0, (uintptr_t)&field_0xf9c - (uintptr_t)&field_0xf84);

    if (mpMatAnm[0] != NULL) {
        mpMatAnm[0]->initialize();
    }

    setAngle(home.angle.y);
    mItemNo = -1;
}

void daNpcTkj_c::setAfterTalkMotion() {
    mFaceMotionSeqMngr.getNo();
    mFaceMotionSeqMngr.setNo(1, -1.0f, 0, 0);
}

void daNpcTkj_c::srchActors() {
    switch (mType) {
    case 0:
    case 1:
        break;
    }
}

BOOL daNpcTkj_c::evtTalk() {
    if (chkAction(&daNpcTkj_c::talk)) {
        (this->*mAction)(NULL);
    } else if (dComIfGp_event_chkTalkXY() == 0 || dComIfGp_evmng_ChkPresentEnd()) {
        setAction(&daNpcTkj_c::talk);
    }

    return 1;
}

BOOL daNpcTkj_c::evtCutProc() {
    BOOL rt = FALSE;
    int staffId = dComIfGp_getEventManager().getMyStaffId("Tkj", this, -1);
    if (staffId != -1) {
        mStaffId = staffId;

        int i;
        const char* evt_name = dComIfGp_getEventManager().getRunEventName();
        for (i = 0; i < 2; i++) {
            if (strcmp(evt_name, mCutNameList[i]) == 0) {
                break;
            }
        }

        if ((this->*mCutList[i])(mStaffId)) {
            dComIfGp_getEventManager().cutEnd(mStaffId);
        }

        rt = TRUE;
    }

    return rt;
}

void daNpcTkj_c::action() {
    if (field_0xf84 != NULL) {
        if (mAction == field_0xf84) {
            (this->*mAction)(NULL);
        } else {
            setAction(field_0xf84);
        }
    }
}

void daNpcTkj_c::setAttnPos() {
    cXyz sp28(0.0f, -30.0f, 0.0f);

    mJntAnm.setParam(this, mpMorf[0]->getModel(), &sp28, getBackboneJointNo(), getNeckJointNo(), getHeadJointNo(),
        daNpc_Tkj_Param_c::m.common.body_angleX_min, daNpc_Tkj_Param_c::m.common.body_angleX_max,
        daNpc_Tkj_Param_c::m.common.body_angleY_min, daNpc_Tkj_Param_c::m.common.body_angleY_max,
        daNpc_Tkj_Param_c::m.common.head_angleX_min, daNpc_Tkj_Param_c::m.common.head_angleX_max,
        daNpc_Tkj_Param_c::m.common.head_angleY_min, daNpc_Tkj_Param_c::m.common.head_angleY_max,
        daNpc_Tkj_Param_c::m.common.neck_rotation_ratio, 0.0f, NULL);
    mJntAnm.calcJntRad(0.2f, 1.0f, cM_s2rad(mCurAngle.y - field_0xd7e.y));

    J3DModelData* mdlData_p = mpMorf[0]->getModel()->getModelData();
    for (u16 i = 0; i < mdlData_p->getJointNum(); i++) {
        mdlData_p->getJointNodePointer(i)->setCallBack(ctrlJointCallBack);
    }

    setMtx();

    mDoMtx_stack_c::copy(mpMorf[0]->getModel()->getAnmMtx(getHeadJointNo()));
    mDoMtx_stack_c::multVec(&sp28, &eyePos);

    mJntAnm.setEyeAngleX(eyePos, 1.0f, 0);
    mJntAnm.setEyeAngleY(eyePos, mCurAngle.y, 0, 1.0f, 0);


    attention_info.position = current.pos;
    attention_info.position.y += daNpc_Tkj_Param_c::m.common.attention_offset;
}

void daNpcTkj_c::setCollision() {
    cXyz pos;
    if (!mHide) {
        mCcCyl.SetCoSPrm(mTwilight == TRUE ? 0x69 : 0x79);
        
        pos = current.pos;
        f32 height = daNpc_Tkj_Param_c::m.common.height;
        f32 radius = daNpc_Tkj_Param_c::m.common.width;

        mCcCyl.SetH(height);
        mCcCyl.SetR(radius);
        mCcCyl.SetC(pos);
        dComIfG_Ccsp()->Set(&mCcCyl);
    }

    mCcCyl.ClrCoHit();
    mCcCyl.ClrTgHit();
}

int daNpcTkj_c::drawDbgInfo() {
    return 0;
}

int daNpcTkj_c::selectAction() {
    field_0xf84 = NULL;

    switch (mType) {
    case 0:
    case 1:
        break;
    }

    field_0xf84 = &daNpcTkj_c::wait;
    return 1;
}

BOOL daNpcTkj_c::chkAction(int (daNpcTkj_c::*i_action)(void*)) {
    return mAction == i_action;
}

int daNpcTkj_c::setAction(int (daNpcTkj_c::*i_action)(void*)) {
    mMode = 3;
    if (mAction != NULL) {
        (this->*mAction)(NULL);
    }

    mMode = 0;
    mAction = i_action;
    if (mAction != NULL) {
        (this->*mAction)(NULL);
    }

    return 1;
}

int daNpcTkj_c::wait(void*) {
    switch (mMode) {
    case 0:
    case 1:
        mFaceMotionSeqMngr.setNo(1, -1.0f, 0, 0);
        mMotionSeqMngr.setNo(0, -1.0f, 0, 0);
        mMode = 2;
    case 2:
        if (srchPlayerActor()) {
            mJntAnm.lookPlayer(0);
        } else {
            if (home.angle.y != mCurAngle.y && step(home.angle.y, -1, -1, 15, 0)) {
                mMode = 1;
            }
            mJntAnm.lookNone(0);
        }

        if (mItemNo == fpcNm_ITEM_DUNGEON_EXIT_2 && mItemPartnerId == fpcM_ERROR_PROCESS_ID_e) {
            dComIfGs_onDungeonItemWarp();
            fopAcM_delete(this);
        }
    case 3:
        break;
    }

    return 1;
}

int daNpcTkj_c::talk(void*) {
    BOOL var_r29 = FALSE;

    switch (mMode) {
    case 0:
    case 1:
        initTalk(mFlowNodeNo, NULL);
        mMode = 2;
    case 2:
        if (!mTwilight) {
            mJntAnm.lookPlayer(0);
            if (mCurAngle.y == fopAcM_searchPlayerAngleY(this)) {
                var_r29 = TRUE;
            } else if (step(fopAcM_searchPlayerAngleY(this), -1, -1, 15, 0)) {
                var_r29 = TRUE;
            }
        } else {
            var_r29 = TRUE;
        }

        if (var_r29 && talkProc(NULL, 0, NULL, 0)) {
            int itemNo;
            int eventId = mFlow.getEventId(&itemNo);
            mItemNo = -1;

            if (eventId == 1) {
                if (mItemPartnerId == fpcM_ERROR_PROCESS_ID_e) {
                    mItemPartnerId = fopAcM_createItemForPresentDemo(&current.pos, itemNo, 0, -1, -1, NULL, NULL);
                }

                if (fopAcM_IsExecuting(mItemPartnerId)) {
                    mEvtNo = 1;
                    evtChange();
                    mItemNo = itemNo;
                    mMode = 3;
                }
            } else {
                mPlayerActorMngr.entry(daPy_getPlayerActorClass());
                dComIfGp_event_reset();
                mMode = 3;
            }
        }
    case 3:
        break;
    }

    return 0;
}

int daNpcTkj_c::cutLv7Start(int i_staffId) {
    dEvent_manager_c* evtmng_p = &dComIfGp_getEventManager();

    int rt = 0;
    if (i_staffId == -1) {
        return 0;
    }

    char* cut_name = evtmng_p->getMyNowCutName(i_staffId);
    if (evtmng_p->getIsAddvance(i_staffId)) {
        switch (*(u32*)cut_name) {
        case '0001':
            mMotionSeqMngr.setNo(0, -1.0f, 0, 0);
            break;
        case '0002':
            mMotionSeqMngr.setNo(1, -1.0f, 0, 0);
            speedF = 13.5f;
            break;
        case '0003':
            mFaceMotionSeqMngr.setNo(0, -1.0f, 0, 0);
            break;
        case '0004':
            break;
        case '0005':
            mMotionSeqMngr.setNo(1, -1.0f, 0, 0);
            break;
        case '0006':
            fopAcM_delete(this);
            break;
        default:
            JUT_ASSERT(1188, FALSE);
            break;
        }
    }

    switch (*(u32*)cut_name) {
    case '0001':
        rt = 1;
        break;
    case '0002': {
        cXyz pos(1753.437f,-200.0f, 5485.9224f);
        if (current.pos.abs(pos) <= speedF) {
            mMotionSeqMngr.setNo(0, -1.0f, 0, 0);
            current.pos = pos;
            speedF = 0.0f;
            rt = 1;
        }

        mSound.startCreatureVoiceLevel(Z2SE_TKJ_V_ODOROKI_LOOP, -1);
        break;
    }
    case '0003':
        if (step(-9090, -1, -1, 15, 0)) {
            rt = 1;
        }

        mSound.startCreatureVoiceLevel(Z2SE_TKJ_V_ODOROKI_LOOP, -1);
        break;
    case '0004':
    case '0006':
        rt = 1;
        break;
    case '0005':
        if (0.0f == speedF) {
            if (step(cLib_targetAngleY(&current.pos, &home.pos), -1, -1, 15, 0)) {
                speedF = 13.5f;
            }
        } else {
            mSound.startCreatureVoiceLevel(Z2SE_TKJ_V_RUNNING_LOOP, -1);
        }

        rt = 1;
        break;
    default:
        JUT_ASSERT(1225, FALSE);
        break; 
    }

    return rt;
}

static int daNpc_Tkj_Create(void* i_this) {
    return ((daNpcTkj_c*)i_this)->create();
}

static int daNpc_Tkj_Delete(void* i_this) {
    return ((daNpcTkj_c*)i_this)->Delete();
}

static int daNpc_Tkj_Execute(void* i_this) {
    return ((daNpcTkj_c*)i_this)->Execute();
}

static int daNpc_Tkj_Draw(void* i_this) {
    return ((daNpcTkj_c*)i_this)->Draw();
}

static int daNpc_Tkj_IsDelete(void* i_this) {
    return 1;
}

void daNpcTkj_c::setParam() {
    u32 attn_flags = fopAc_AttnFlag_SPEAK_e | fopAc_AttnFlag_TALK_e;
    selectAction();
    srchActors();

    s16 talk_dist = daNpc_Tkj_Param_c::m.common.talk_distance;
    s16 talk_angle = daNpc_Tkj_Param_c::m.common.talk_angle;
    s16 attn_dist = daNpc_Tkj_Param_c::m.common.attention_distance;
    s16 attn_angle = daNpc_Tkj_Param_c::m.common.attention_angle;

    attention_info.distances[fopAc_attn_LOCK_e] = daNpcT_getDistTableIdx(attn_dist, attn_angle);
    attention_info.distances[fopAc_attn_TALK_e] = attention_info.distances[0];
    attention_info.distances[fopAc_attn_SPEAK_e] = daNpcT_getDistTableIdx(talk_dist, talk_angle);
    attention_info.flags = attn_flags;

    f32 size;
    if (mType == 1) {
        size = 0.7f;
    } else {
        size = daNpc_Tkj_Param_c::m.common.scale;
    }

    scale.setall(size);

    mAcchCir.SetWallR(daNpc_Tkj_Param_c::m.common.width);
    mAcchCir.SetWallH(daNpc_Tkj_Param_c::m.common.knee_length);
    mCcStts.SetWeight(daNpc_Tkj_Param_c::m.common.weight);
    mCylH = daNpc_Tkj_Param_c::m.common.height;
    mWallR = daNpc_Tkj_Param_c::m.common.width;
    gravity = daNpc_Tkj_Param_c::m.common.gravity;

    mExpressionMorfFrame = daNpc_Tkj_Param_c::m.common.expression_morf_frame;
    mMorfFrames = daNpc_Tkj_Param_c::m.common.morf_frame;
}

void daNpcTkj_c::beforeMove() {
    if (checkHide() || mNoDraw) {
        attention_info.flags = 0;
    }
}

static daNpc_Tkj_Param_c l_HIO;

static actor_method_class daNpc_Tkj_MethodTable = {
    (process_method_func)daNpc_Tkj_Create,
    (process_method_func)daNpc_Tkj_Delete,
    (process_method_func)daNpc_Tkj_Execute,
    (process_method_func)daNpc_Tkj_IsDelete,
    (process_method_func)daNpc_Tkj_Draw,
};

extern actor_process_profile_definition g_profile_NPC_TKJ = {
  fpcLy_CURRENT_e,        // mLayerID
  7,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_NPC_TKJ,           // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(daNpcTkj_c),     // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  373,                    // mPriority
  &daNpc_Tkj_MethodTable, // sub_method
  0x08044107,             // mStatus
  fopAc_NPC_e,            // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};
