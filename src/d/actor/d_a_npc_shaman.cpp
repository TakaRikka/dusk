/**
 * @file d_a_npc_shaman.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_npc_shaman.h"
#include "JSystem/JHostIO/JORFile.h"

#if DEBUG
daNpc_Sha_HIO_c::daNpc_Sha_HIO_c() {
    m = daNpc_Sha_Param_c::m;
}

void daNpc_Sha_HIO_c::listenPropertyEvent(const JORPropertyEvent* event) {
    char buffer[2000];

    JORReflexible::listenPropertyEvent(event);
    
    JORFile jorFile;
    int len;

    switch ((u32)event->id) {
        case 0x40000002:
            if (jorFile.open(6, "", NULL, NULL, NULL)) {
                memset(buffer, 0, sizeof(buffer));
                len = 0;
                daNpcT_cmnListenPropertyEvent(buffer, &len, &m.common);
                jorFile.writeData(buffer, len);
                jorFile.close();
                OS_REPORT("write append success!::%6d\n", len);
            } else {
                OS_REPORT("write append failure!\n");
            }
            break;
    }
}

void daNpc_Sha_HIO_c::genMessage(JORMContext* ctx) {
    daNpcT_cmnGenMessage(ctx, &m.common);
    ctx->genButton("ファイル書き出し", 0x40000002, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
}
#endif

enum Sha_RES_File_ID {
    /* BCK */
    /* 0x07 */ BCK_SHA_F_CLOSEEYES = 0x7,
    /* 0x08 */ BCK_SHA_F_HIRAMEKU,
    /* 0x09 */ BCK_SHA_F_LAUGH,
    /* 0x0A */ BCK_SHA_F_TALK_A,
    /* 0x0B */ BCK_SHA_F_TALK_B,
    /* 0x0C */ BCK_SHA_F_TUNAGARI,
    /* 0x0D */ BCK_SHA_FH_CLOSEEYES,
    /* 0x0E */ BCK_SHA_FH_HIRAMEKU,
    /* 0x0F */ BCK_SHA_FH_LAUGH,
    /* 0x10 */ BCK_SHA_HIRAMEKU,
    /* 0x11 */ BCK_SHA_HIRAMEKU_WAIT,
    /* 0x12 */ BCK_SHA_LAUGH,
    /* 0x13 */ BCK_SHA_TALK_A,
    /* 0x14 */ BCK_SHA_TALK_B,
    /* 0x15 */ BCK_SHA_TUNAGARI,
    /* 0x16 */ BCK_SHA_URANAU,
    /* 0x17 */ BCK_SHA_WAIT_A,

    /* BMDR */
    /* 0x1A */ BMDR_SHA = 0x1A,

    /* BTK */
    /* 0x1D */ BTK_SHA = 0x1D,

    /* BTP */
    /* 0x20 */ BTP_SHA = 0x20,
    /* 0x21 */ BTP_SHA_F_CLOSEEYES,
    /* 0x22 */ BTP_SHA_F_LAUGH,
    /* 0x23 */ BTP_SHA_F_TALK,
    /* 0x24 */ BTP_SHA_F_TUNAGARI,
    /* 0x25 */ BTP_SHA_FH_CLOSEEYES,
    /* 0x26 */ BTP_SHA_FH_HIRAMEKU,
    /* 0x27 */ BTP_SHA_FH_LAUGH,

    /* EVT */
    /* 0x2B */ EVT_SHA_EVENT_LIST = 0x2B,
};

enum RES_Name {
    /* 0x0 */ NONE,
    /* 0x1 */ SHA,
};

static int l_bmdData[1][2] = {
    {BMDR_SHA, SHA},
};

static daNpcT_evtData_c l_evtList[6] = {
    {"", 0},
    {"NO_RESPONSE", 0},
    {"PERFORM_AUGURY", 1},
    {"ALLGET", 1},
    {"NOLOOK", 1},
    {"RETURN", 1},
};

static char* l_resNameList[2] = {
    "",
    "Sha",
};

static s8 l_loadResPtrn0[2] = {
    SHA, -1,
};

static s8* l_loadResPtrnList[3] = {
    l_loadResPtrn0,
    l_loadResPtrn0,
    l_loadResPtrn0,
};

static daNpcT_faceMotionAnmData_c l_faceMotionAnmData[10] = {
    {-1, J3DFrameCtrl::EMode_NONE, NONE, 32, J3DFrameCtrl::EMode_LOOP, SHA, 1},
    {10, J3DFrameCtrl::EMode_NONE, SHA, 32, J3DFrameCtrl::EMode_LOOP, SHA, 1},
    {9, J3DFrameCtrl::EMode_NONE, SHA, 35, J3DFrameCtrl::EMode_NONE, SHA, 0},
    {8, J3DFrameCtrl::EMode_NONE, SHA, 34, J3DFrameCtrl::EMode_NONE, SHA, 0},
    {7, J3DFrameCtrl::EMode_NONE, SHA, 33, J3DFrameCtrl::EMode_NONE, SHA, 0},
    {12, J3DFrameCtrl::EMode_NONE, SHA, 37, J3DFrameCtrl::EMode_NONE, SHA, 0},
    {11, J3DFrameCtrl::EMode_NONE, SHA, 36, J3DFrameCtrl::EMode_NONE, SHA, 0},
    {15, J3DFrameCtrl::EMode_LOOP, SHA, 40, J3DFrameCtrl::EMode_LOOP, SHA, 0},
    {14, J3DFrameCtrl::EMode_LOOP, SHA, 39, J3DFrameCtrl::EMode_LOOP, SHA, 0},
    {13, J3DFrameCtrl::EMode_LOOP, SHA, 38, J3DFrameCtrl::EMode_LOOP, SHA, 0},
};

static daNpcT_motionAnmData_c l_motionAnmData[8] = {
    {23, J3DFrameCtrl::EMode_LOOP, SHA, 29, J3DFrameCtrl::EMode_NONE, SHA, 1, 0},
    {19, J3DFrameCtrl::EMode_NONE, SHA, 29, J3DFrameCtrl::EMode_NONE, SHA, 1, 0},
    {21, J3DFrameCtrl::EMode_NONE, SHA, 29, J3DFrameCtrl::EMode_NONE, SHA, 1, 0},
    {22, J3DFrameCtrl::EMode_LOOP, SHA, 29, J3DFrameCtrl::EMode_NONE, SHA, 1, 0},
    {18, J3DFrameCtrl::EMode_NONE, SHA, 29, J3DFrameCtrl::EMode_NONE, SHA, 1, 0},
    {16, J3DFrameCtrl::EMode_NONE, SHA, 29, J3DFrameCtrl::EMode_NONE, SHA, 1, 0},
    {17, J3DFrameCtrl::EMode_LOOP, SHA, 29, J3DFrameCtrl::EMode_NONE, SHA, 1, 0},
    {20, J3DFrameCtrl::EMode_NONE, SHA, 29, J3DFrameCtrl::EMode_NONE, SHA, 1, 0},
};

static daNpcT_MotionSeqMngr_c::sequenceStepData_c l_faceMotionSequenceData[40] = {
    {1, -1, 1}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {2, -1, 1}, {7, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {3, 0, 1}, {8, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {5, -1, 1}, {4, 0, 1}, {-1, 0, 0}, {-1, 0, 0},
    {4, -1, 1}, {9, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {9, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {6, 0, 1}, {0, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {7, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {8, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {0, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
};

static daNpcT_MotionSeqMngr_c::sequenceStepData_c l_motionSequenceData[32] = {
    {0, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {1, -1, 1}, {0, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {2, -1, 1}, {3, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {5, 0, 1}, {6, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {4, 0, 1}, {0, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {3, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {7, 0, 1}, {0, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
};

char* daNpc_Sha_c::mCutNameList[2] = {
    "",
    "PERFORM_AUGURY",
};

daNpc_Sha_c::cutFunc daNpc_Sha_c::mCutList[2] = {
    NULL,
    &daNpc_Sha_c::cutPerformAugury,
};

static NPC_SHA_HIO_CLASS l_HIO;

daNpc_Sha_c::~daNpc_Sha_c() {
    OS_REPORT("|%06d:%x|daNpc_Sha_c -> デストラクト\n", g_Counter.mCounter0, this);

    if (mpMorf[0] != NULL) {
        mpMorf[0]->stopZelAnime();
    }

    #if DEBUG
    if (mpHIO != NULL) {
        mpHIO->removeHIO();
    }
    #endif

    deleteRes(l_loadResPtrnList[mType], (const char**)l_resNameList);
}

daNpc_Sha_HIOParam const daNpc_Sha_Param_c::m = {
    210.0f,
    -3.0f,
    1.0f,
    500.0f,
    255.0f,
    200.0f,
    0.0f,
    50.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    30.0f,
    -15.0f,
    20.0f,
    -20.0f,
    0.6f,
    12.0f,
    3,
    6,
    5,
    6,
    110.0f,
    0.0f,
    0.0f,
    0.0f,
    60,
    8,
    0,
    0,
    0,
    false,
    false,
    4.0f,
    0.0,
    0.0,
    0.0,
    0.0,
    0.0,
    0.0,
    0.0,
};

cPhs__Step daNpc_Sha_c::create() {
    daNpcT_ct(this, daNpc_Sha_c, l_faceMotionAnmData, l_motionAnmData,
                       l_faceMotionSequenceData, 4, l_motionSequenceData, 4, l_evtList, l_resNameList);

    mType = getType();
    mFlowNodeNo = getFlowNodeNo();
    mTwilight = false;

    cPhs__Step phase = (cPhs__Step)loadRes(l_loadResPtrnList[mType], (const char**)l_resNameList);
    if (phase == cPhs_COMPLEATE_e) {
        OS_REPORT("\t(%s:%d) flowNo:%d<%08x> ", fopAcM_getProcNameString(this), mType, mFlowNodeNo, fopAcM_GetParam(this));

        if (isDelete()) {
            OS_REPORT("===>isDelete:TRUE\n");
            return cPhs_ERROR_e;
        }

        OS_REPORT("\n");
        
        static int const heapSize[3] = {
            0x3AF0, 0x3AF0, 0x3AF0,
        };
        if (!fopAcM_entrySolidHeap(this, createHeapCallBack, heapSize[mType])) {
            return cPhs_ERROR_e;
        }

        J3DModelData* mdlData_p = mpMorf[0]->getModel()->getModelData();
        fopAcM_SetMtx(this, mpMorf[0]->getModel()->getBaseTRMtx());
        fopAcM_setCullSizeBox(this, -200.0f, -100.0f, -200.0f, 200.0f, 300.0f, 200.0f);
        mSound.init(&current.pos, &eyePos, 3, 1);

        #if DEBUG
        mpHIO = &l_HIO;
        mpHIO->entryHIO("占い師");
        #endif

        reset();
        
        mAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1, &mAcchCir, fopAcM_GetSpeed_p(this),
                  fopAcM_GetAngle_p(this), fopAcM_GetShapeAngle_p(this));
        mCcStts.Init(mpHIO->m.common.weight, 0, this);
        mCyl.Set(mCcDCyl);
        mCyl.SetStts(&mCcStts);
        mCyl.SetTgHitCallback(tgHitCallBack);
        mAcch.CrrPos(dComIfG_Bgsp());
        mGndChk = mAcch.m_gnd;
        mGroundH = mAcch.GetGroundH();

        if (mGroundH != -G_CM3D_F_INF) {
            setEnvTevColor();
            setRoomNo();
        }

        mCreating = true;
        Execute();
        mCreating = false;
    }

    return phase;
}

int daNpc_Sha_c::CreateHeap() {
    J3DModelData* modelData = static_cast<J3DModelData*>(dComIfG_getObjectRes(l_resNameList[l_bmdData[0][1]], l_bmdData[0][0]));
    if (modelData == NULL) {
        return 0;
    }

    mpMorf[0] = new mDoExt_McaMorfSO(modelData, NULL, NULL, NULL, -1, 1.0f, 0, -1, &mSound, J3DMdlFlag_DifferedDLBuffer, 0x11020284);
    if (mpMorf[0] == NULL || mpMorf[0]->getModel() == NULL) {
        return 0;
    }

    J3DModel* model = mpMorf[0]->getModel();
    for (u16 i = 0; i < modelData->getJointNum(); i++) {
        modelData->getJointNodePointer(i)->setCallBack(ctrlJointCallBack);
    }
    model->setUserArea((uintptr_t)this);

    mpMatAnm[0] = new daNpcT_MatAnm_c();
    if (mpMatAnm[0] == NULL) {
        return 0;
    }

    if (setFaceMotionAnm(1, false) && setMotionAnm(0, 0.0f, FALSE)) {
        return 1;
    }

    return 0;
}

int daNpc_Sha_c::Delete() {
    OS_REPORT("|%06d:%x|daNpc_Sha_c -> コンストラクト\n", g_Counter.mCounter0, this);
    fpc_ProcID id = fopAcM_GetID(this);
    this->~daNpc_Sha_c();
    return 1;
}

int daNpc_Sha_c::Execute() {
    return execute();
}

int daNpc_Sha_c::Draw() {
    if (mpMatAnm[0] != NULL) {
        J3DModelData* mdlData_p = mpMorf[0]->getModel()->getModelData();
        mdlData_p->getMaterialNodePointer(getEyeballMaterialNo())->setMaterialAnm(mpMatAnm[0]);
    }

    #if DEBUG
    return draw(chkAction(&daNpc_Sha_c::test), FALSE, mRealShadowSize, NULL, 100.0f, FALSE, FALSE, FALSE);
    #else
    return draw(FALSE, FALSE, mRealShadowSize, NULL, 100.0f, FALSE, FALSE, FALSE);
    #endif
}

int daNpc_Sha_c::createHeapCallBack(fopAc_ac_c* i_this) {
    daNpc_Sha_c* actor = (daNpc_Sha_c*)i_this;
    return actor->CreateHeap();
}

int daNpc_Sha_c::ctrlJointCallBack(J3DJoint* i_joint, int param_2) {
    if (param_2 == 0) {
        J3DModel* mdl_p = j3dSys.getModel();
        daNpc_Sha_c* actor = (daNpc_Sha_c*)mdl_p->getUserArea();
        if (actor != NULL) {
            actor->ctrlJoint(i_joint, mdl_p);
        }
    }

    return 1;
}

u8 daNpc_Sha_c::getType() {
    u8 param = fopAcM_GetParam(this) & 0xFF;
    switch (param) {
        case 0:
            return 0;
        
        case 1:
            return 1;

        default:
            return 2;
    }
}

BOOL daNpc_Sha_c::isDelete() {
    switch (mType) {
        case 0:
            return FALSE;

        case 1:
            return FALSE;

        default:
            return FALSE;
    }

    return FALSE;
}

void daNpc_Sha_c::reset() {
    csXyz angle;
    int size = (u8*)&field_0xfa4 - (u8*)&mNextAction;

    if (mpMatAnm[0] != NULL) {
        mpMatAnm[0]->initialize();
    }

    initialize();
    memset(&mNextAction, 0, size);
    angle.setall(0);
    angle.y = home.angle.y;

    switch (mType) {
        case 0:
            break;
        case 1:
            break;
        default:
            break;
    }

    setAngle(angle);
}

void daNpc_Sha_c::afterJntAnm(int i_joint) {
    if (i_joint == 1) {
        mDoMtx_stack_c::YrotM(mStagger.getAngleZ(1));
        mDoMtx_stack_c::ZrotM(-mStagger.getAngleX(1));
    } else if (i_joint == 4) {
        mDoMtx_stack_c::YrotM(mStagger.getAngleZ(0));
        mDoMtx_stack_c::ZrotM(mStagger.getAngleX(0));
    }
}

void daNpc_Sha_c::setParam() {
    selectAction();
    srchActors();
    u32 flags = fopAc_AttnFlag_SPEAK_e | fopAc_AttnFlag_TALK_e;
    s16 talk_distance = mpHIO->m.common.talk_distance;
    s16 talk_angle = mpHIO->m.common.talk_angle;
    s16 attention_distance = mpHIO->m.common.attention_distance;
    s16 attention_angle = mpHIO->m.common.attention_angle;

    if (mType == 1) {
        talk_angle = 4;
        attention_angle = 4;
    }

    attention_info.distances[fopAc_attn_LOCK_e] = daNpcT_getDistTableIdx(attention_distance, attention_angle);
    attention_info.distances[fopAc_attn_TALK_e] = attention_info.distances[fopAc_attn_LOCK_e];
    attention_info.distances[fopAc_attn_SPEAK_e] = daNpcT_getDistTableIdx(talk_distance, talk_angle);
    attention_info.flags = flags;

    scale.set(mpHIO->m.common.scale, mpHIO->m.common.scale, mpHIO->m.common.scale);
    mCcStts.SetWeight(mpHIO->m.common.weight);
    mCylH = mpHIO->m.common.height;
    mWallR = mpHIO->m.common.width;
    mAttnFovY = mpHIO->m.common.fov;
    mAcchCir.SetWallR(mWallR);
    mAcchCir.SetWallH(mpHIO->m.common.knee_length);
    mRealShadowSize = mpHIO->m.common.real_shadow_size;
    mExpressionMorfFrame = mpHIO->m.common.expression_morf_frame;
    mMorfFrames = mpHIO->m.common.morf_frame;
    gravity = mpHIO->m.common.gravity;
}

BOOL daNpc_Sha_c::checkChangeEvt() {
    if (!chkAction(&daNpc_Sha_c::talk)) {
        mPreItemNo = 0;

        if (dComIfGp_event_chkTalkXY()) {
            if (dComIfGp_evmng_ChkPresentEnd()) {
                mEvtNo = 1;
                evtChange();
            }

            return TRUE;
        }

        switch (mType) {
            case 0:
                break;
            
            case 1:
                if (field_0xfa0 == 0) {
                    mEvtNo = 2;
                    evtChange();
                    return TRUE;
                }
                break;

            default:
                break;
        }
    }

    return FALSE;
}

void daNpc_Sha_c::setAfterTalkMotion() {
    int idx = 9;

    switch (mFaceMotionSeqMngr.getNo()) {
        case 1:
            idx = 7;
            break;

        case 2:
            idx = 8;
            break;
        
        case 3:
            idx = 5;
            break;

        case 4:
            idx = 5;
            break;
    }

    mFaceMotionSeqMngr.setNo(idx, -1.0f, FALSE, 0);
}

void daNpc_Sha_c::srchActors() {
    #if DEBUG
    switch (mType) {
        case 0:
        case 1:
        default:
            break;
    }
    #endif
}

BOOL daNpc_Sha_c::evtTalk() {
    if (chkAction(&daNpc_Sha_c::talk)) {
        (this->*mAction)(NULL);
    } else {
        mPreItemNo = 0;

        if (dComIfGp_event_chkTalkXY()) {
            if (!dComIfGp_evmng_ChkPresentEnd()) {
                return TRUE;
            }

            mEvtNo = 1;
            evtChange();
            return TRUE;
        }

        setAction(&daNpc_Sha_c::talk);
    }

    return TRUE;
}

BOOL daNpc_Sha_c::evtCutProc() {
    s32 staffId = dComIfGp_getEventManager().getMyStaffId("Sha", this, -1);
    if (staffId != -1) {
        mStaffId = staffId;
        int act_idx = dComIfGp_getEventManager().getMyActIdx(mStaffId, mCutNameList, 2, FALSE, FALSE);

        if ((this->*mCutList[act_idx])(mStaffId)) {
            dComIfGp_getEventManager().cutEnd(mStaffId);
        }

        return TRUE;
    }

    return FALSE;
}

void daNpc_Sha_c::action() {
    fopAc_ac_c* actor_p = NULL;
    actor_p = hitChk(&mCyl, 0xFFFFFFFF);
    if (actor_p != NULL) {
        if (mCyl.GetTgHitObj()->ChkAtType(AT_TYPE_THROW_OBJ)) {
            mStagger.setParam(this, actor_p, mCurAngle.y);
            setDamage(0, 9, 0);
            mDamageTimerStart = 0;
            mJntAnm.lookNone(1);
        }
    }

    if (mStagger.checkRebirth()) {
        mStagger.initialize();
        mMode = 1;
    }

    if (mNextAction) {
        if (mAction == mNextAction) {
            (this->*mAction)(NULL);
        } else {
            setAction(mNextAction);
        }
    }
}

void daNpc_Sha_c::beforeMove() {
    fopAcM_OffStatus(this, fopAcM_STATUS_UNK_0x8000000);

    if (checkHide()) {
        fopAcM_OnStatus(this, fopAcM_STATUS_UNK_0x8000000);
    }

    if (checkHide() || mNoDraw) {
        attention_info.flags = 0;
    }
}

void daNpc_Sha_c::setAttnPos() {
    cXyz pos(-30.0f, 10.0f, 0.0f);
    cXyz sp40(0.0f, 10.0f, 0.0f);

    mStagger.calc(FALSE);
    f32 rad = cM_s2rad((s16)(mCurAngle.y - field_0xd7e.y));
    
    mJntAnm.setParam(this, mpMorf[0]->getModel(), &pos, getBackboneJointNo(), getNeckJointNo(), getHeadJointNo(),
                     mpHIO->m.common.body_angleX_min, mpHIO->m.common.body_angleX_max, mpHIO->m.common.body_angleY_min, mpHIO->m.common.body_angleY_max,
                     mpHIO->m.common.head_angleX_min, mpHIO->m.common.head_angleX_max, mpHIO->m.common.head_angleY_min, mpHIO->m.common.head_angleY_max,
                     mpHIO->m.common.neck_rotation_ratio, rad, &sp40);
    mJntAnm.calcJntRad(0.2f, 1.0f, rad);
    setMtx();

    mDoMtx_stack_c::copy(mpMorf[0]->getModel()->getAnmMtx(getHeadJointNo()));
    mDoMtx_stack_c::multVec(&pos, &eyePos);

    mJntAnm.setEyeAngleX(eyePos, 1.0f, 0);
    mJntAnm.setEyeAngleY(eyePos, mCurAngle.y, TRUE, 1.0f, 0);

    pos.set(0.0f, 0.0f, 0.0f);
    pos.y = mpHIO->m.common.attention_offset;
    mDoMtx_stack_c::YrotS(mCurAngle.y);
    mDoMtx_stack_c::multVec(&pos, &pos);
    attention_info.position = current.pos + pos;

    sp40.set(0.0f, 90.0f, 94.0f);
    mDoMtx_stack_c::YrotS(mCurAngle.y);
    mDoMtx_stack_c::multVec(&sp40, &sp40);
    field_0xd6c = current.pos + sp40;

    mDoAud_seStartLevel(Z2SE_OBJ_CRYSTAL_KRKR, &field_0xd6c, 0, 0);
}

void daNpc_Sha_c::setCollision() {
    cXyz pos;

    if (!mHide) {
        u32 co_s_prm = 0x79;
        u32 tg_type = 0xD8FBFDFF;
        u32 tg_s_prm = 0x1F;

        if (mTwilight) {
            tg_type = 0;
            tg_s_prm = 0;
        } else if (mStagger.checkStagger()) {
            tg_type = 0;
            tg_s_prm = 0;
        }

        mCyl.SetCoSPrm(co_s_prm);
        mCyl.SetTgType(tg_type);
        mCyl.SetTgSPrm(tg_s_prm);
        mCyl.OnTgNoHitMark();

        pos.set(0.0f, 0.0f, 0.0f);
        f32 cyl_h = mCylH;
        f32 wall_r = mWallR;

        mDoMtx_stack_c::YrotS(mCurAngle.y);
        mDoMtx_stack_c::multVec(&pos, &pos);
        pos += current.pos;

        mCyl.SetH(cyl_h);
        mCyl.SetR(wall_r);
        mCyl.SetC(pos);
        dComIfG_Ccsp()->Set(&mCyl);
    }

    mCyl.ClrCoHit();
    mCyl.ClrTgHit();
}

int daNpc_Sha_c::drawDbgInfo() {
    return 0;
}

BOOL daNpc_Sha_c::selectAction() {
    mNextAction = NULL;

    #if DEBUG
    if (mpHIO->m.common.debug_mode_ON) {
        mNextAction = &daNpc_Sha_c::test;
        return TRUE;
    }
    #endif

    switch (mType) {
        default:
            break;
    }

    mNextAction = &daNpc_Sha_c::wait;

    return TRUE;
}

BOOL daNpc_Sha_c::chkAction(actionFunc action) {
    return mAction == action;
}

BOOL daNpc_Sha_c::setAction(actionFunc action) {
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

int daNpc_Sha_c::getSceneChangeNoTableIx() {
    int iVar2[48];
    BOOL bool_array[48];
    int sp_0x40 = 0;
    int sp_0x3c = 0;
    int sp_0x38 = 0;
    int sp_0x34 = 0;
    int sp_0x30 = 0;
    int sp_0x2c = 0;
    int sp_0x28 = 0;
    sp_0x2c = 0;
    u8 eventReg;
    u8 tmp_reg;

    for (int i = 0; i < 6; i++) {
        eventReg = dComIfGs_getEventReg((u16)mEvtBitLabels[i]);
        tmp_reg = 1;

        for (int j = 0; j < 8; j++) {
            bool_array[i*8 + j] = FALSE;

            if ((tmp_reg & eventReg) != 0) {
                bool_array[i*8 + j] = TRUE;
                sp_0x2c++;
            }

            tmp_reg <<= 1;
        }
    }

    if (sp_0x2c >= 45) {
        return -2;
    }

    for (sp_0x40 = 2; sp_0x40 != 0; sp_0x40--) {
        for (int idx = 0; idx < 6; idx++) {
            tmp_reg = dComIfGs_getTmpReg((u16)mTmpBitLabels[idx]);
            eventReg = dComIfGs_getEventReg((u16)mEvtBitLabels[idx]);
            tmp_reg |= eventReg;
            dComIfGs_setTmpReg((u16)mTmpBitLabels[idx], tmp_reg);
        }

        for (int m = 0; m < 48; m++) {
            iVar2[m] = -1;
        }

        sp_0x34 = 0;
        sp_0x3c = 0;
        sp_0x38 = 0;
        sp_0x30 = 0;
        sp_0x28 = 0;
        for (int n = 0; n < 6; n++) {
            tmp_reg = 1;
            for (int o = 0; o < 8; o++) {
                if ((this->*mQueries[n*8 + o])()) {
                    sp_0x34++;
                    if (bool_array[n*8 + o]) {
                        sp_0x28++;
                    }

                    eventReg = dComIfGs_getTmpReg((u16)mTmpBitLabels[n]);
                    if ((tmp_reg & eventReg) == 0) {
                        iVar2[sp_0x3c] = sp_0x30;
                        sp_0x3c++;
                    } else {
                        sp_0x38++;
                    }
                }

                tmp_reg <<= 1;
                sp_0x30++;
            }
        }

        eventReg = dComIfGs_getTmpReg(0xF2FF);
        if (eventReg != 0) {
            if (eventReg > 45) {
                eventReg = 45;
            }

            --eventReg;
            return eventReg;
        }

        if (sp_0x34 == 0 || sp_0x34 == sp_0x28) {
            return -1;
        }

        if (sp_0x3c != 0) {
            sp_0x30 = cLib_getRndValue(0, sp_0x3c);
            sp_0x30 = (sp_0x30 + g_Counter.mCounter0) % sp_0x3c;
            return iVar2[sp_0x30];
        }

        if (sp_0x38 == 0) {
            break;
        }

        for (int p = 0; p < 6; p++) {
            dComIfGs_setTmpReg(mTmpBitLabels[p] & 0xFFFF, 0);
        }
    }

    return -1;
}

void daNpc_Sha_c::setTempBit(int i_idx) {
    u8 reg_r30 = 0;
    u8 reg_r29 = 0;
    if (i_idx < 48) {
        reg_r30 = dComIfGs_getTmpReg((u16)mTmpBitLabels[i_idx / 8]);
        reg_r29 = 1;

        if (i_idx % 8 != 0) {
            for (int i = 0; i < i_idx % 8; i++) {
                reg_r29 = reg_r29 << 1;
            }
        }

        reg_r30 |= reg_r29;
        dComIfGs_setTmpReg((u16)mTmpBitLabels[i_idx / 8], reg_r30);
    }
}

int daNpc_Sha_c::cutPerformAugury(int i_staffId) {
    cXyz pos;
    csXyz angle;
    int rv = 0;
    int* piVar1 = NULL;
    int prm = -1;
    int msgNo = 0;
    int msgNo2 = 0;
    int timer = 0;

    piVar1 = dComIfGp_evmng_getMyIntegerP(i_staffId, "prm");
    if (piVar1 != NULL) {
        prm = *piVar1;
    }

    piVar1 = dComIfGp_evmng_getMyIntegerP(i_staffId, "msgNo");
    if (piVar1 != NULL) {
        msgNo = *piVar1;
    }

    piVar1 = dComIfGp_evmng_getMyIntegerP(i_staffId, "msgNo2");
    if (piVar1 != NULL) {
        msgNo2 = *piVar1;
    }

    piVar1 = dComIfGp_evmng_getMyIntegerP(i_staffId, "timer");
    if (piVar1 != NULL) {
        timer = *piVar1;
    }

    if (dComIfGp_getEventManager().getIsAddvance(i_staffId)) {
        switch (prm) {
            case 0:
                mSceneChangeNoTableIx = -99999;
                dComIfGp_event_offHindFlag(128);
                initTalk(mFlowNodeNo, NULL);
                break;

            case 2:
                mEventTimer = timer;
                break;

            case 3:
                dComIfGp_event_offHindFlag(128);
                initTalk(mFlowNodeNo, NULL);
                break;

            case 4:
                dComIfGp_event_offHindFlag(128);
                initTalk(0x772, NULL);
                break;
            
            case 5:
                dComIfGp_event_offHindFlag(128);
                initTalk(0x773, NULL);
                break;

            case 6:
                if (daNpcT_getPlayerInfoFromPlayerList(2, fopAcM_GetRoomNo(this), &pos, &angle)) {
                    daPy_getPlayerActorClass()->setPlayerPosAndAngle(&pos, angle.y, 0);
                }

                daNpcT_offTmpBit(11);
                break;

            case 7:
                mEventTimer = timer;
                daNpcT_offTmpBit(11);
                break;
        }
    }

    int msgNos[3] = {-1, -1, -1};

    switch (prm) {
        case 0:
        case 3:
        case 4:
        case 5:
            mJntAnm.lookPlayer(0);
            msgNos[0] = msgNo;
            msgNos[1] = msgNo2;

            if (talkProc(msgNos, FALSE, NULL, FALSE)) {
                if (msgNo == 0 && msgNo2 == 0) {
                    if (mFlow.checkEndFlow()) {
                        rv = 1;
                    }
                } else {
                    rv = 1;
                }
            }
            break;

        case 1:
            mJntAnm.lookNone(0);

            if (talkProc(NULL, FALSE, NULL, FALSE)) {
                int id;
                u32 eventId = mFlow.getEventId(&id);
                switch (eventId) {
                    case 0x16:
                        if (mSceneChangeNoTableIx == -99999) {
                            mSceneChangeNoTableIx = getSceneChangeNoTableIx();

                            if (mSceneChangeNoTableIx >= 0) {
                                setTempBit(mSceneChangeNoTableIx);
                            }
                        }
                        
                        if (mSceneChangeNoTableIx == -2) {
                            mEvtNo = 3;
                            evtChange();
                        } else if (mSceneChangeNoTableIx == -1) {
                            mEvtNo = 4;
                            evtChange();
                        } else if (mSceneChangeNoTableIx >= 0) {
                            g_env_light.field_0x130a = 1;
                            dComIfGs_onTmpBit(0x1301);
                            dStage_changeScene(mSceneChangeNoTable[mSceneChangeNoTableIx], 0.0f, 0, fopAcM_GetRoomNo(this), 0, -1);
                        } else if (mFlow.checkEndFlow()) {
                            rv = 1;
                        }
                        break;

                    default:
                        if (mFlow.checkEndFlow()) {
                            rv = 1;
                        }
                        break;
                }
            }
            break;
        
        case 2:
        case 6:
        case 7:
            mJntAnm.lookNone(0);

            if (cLib_calcTimer(&mEventTimer) == 0) {
                rv = 1;
            }
            break;
    }

    return rv;
}

int daNpc_Sha_c::wait(void* param_1) {
    int unused = 0;
    daPy_py_c* player = daPy_getPlayerActorClass();

    switch (mMode) {
        case 0:
        case 1:
            if (!mStagger.checkStagger()) {
                mFaceMotionSeqMngr.setNo(9, -1.0f, FALSE, 0);
                mMotionSeqMngr.setNo(0, -1.0f, FALSE, 0);
                mMode = 2;
            }
            // fallthrough
        case 2:
            if (mType == 1) {
                field_0xfa0 = 0;

                if (daNpcT_chkTmpBit(11)) {
                    g_env_light.field_0x130a = 0;
                    dComIfGs_offTmpBit(0x1301);
                    field_0xfa0 = 1;
                    mEvtNo = 5;
                    field_0xe33 = true;
                }
            }

            if (!mStagger.checkStagger()) {
                if (mType == 1) {
                    mPlayerActorMngr.remove();
                }

                if (mPlayerActorMngr.getActorP() != NULL) {
                    mJntAnm.lookPlayer(0);

                    if (!chkActorInSight(mPlayerActorMngr.getActorP(), mAttnFovY, mCurAngle.y)) {
                        mJntAnm.lookNone(0);
                    }

                    if (!srchPlayerActor() && home.angle.y == mCurAngle.y) {
                        mMode = 1;
                    }
                } else {
                    mJntAnm.lookNone(0);

                    if (home.angle.y != mCurAngle.y) {
                        if (field_0xe34 != 0) {
                            if (step(home.angle.y, -1, -1, 15, 0)) {
                                mMode = 1;
                            }
                        } else {
                            setAngle(home.angle.y);
                            mMode = 1;
                        }

                        attention_info.flags = 0;
                    } else if (!mTwilight) {
                        srchPlayerActor();
                    }
                }

                switch (mJntAnm.getMode()) {
                    case 0:
                    default:
                        break;
                }
            }
            break;

        case 3:
            break;
    }

    return 1;
}

int daNpc_Sha_c::talk(void* param_1) {
    switch (mMode) {
        case 0:
        case 1:
            if (!mStagger.checkStagger()) {
                initTalk(mFlowNodeNo, NULL);
                mMode = 2;
            }
            // fallthrough
        case 2:
            if (!mStagger.checkStagger()) {
                if (mTwilight || mPlayerAngle == mCurAngle.y || mType == 1) {
                    if (talkProc(NULL, FALSE, NULL, FALSE)) {
                        int id;
                        u32 eventId = mFlow.getEventId(&id);

                        if (mFlow.checkEndFlow()) {
                            mPlayerActorMngr.entry(daPy_getPlayerActorClass());
                            dComIfGp_event_reset();
                            mMode = 3;
                        }
                    }

                    mJntAnm.lookPlayer(0);

                    if (mTwilight) {
                        mJntAnm.lookNone(0);
                    }
                } else {
                    mJntAnm.lookPlayer(0);
                    step(mPlayerAngle, -1, -1, 15, 0);
                }
            }
            break;

        case 3:
            break;
    }

    return 0;
}

int daNpc_Sha_c::test(void* param_1) {
    switch(mMode) {
    case 0:
    case 1:
        speedF = 0.0f;
        speed.setall(0.0f);
        mMode = 2;
        // fallthrough
    case 2:
        mFaceMotionSeqMngr.setNo(mpHIO->m.common.face_expression, -1.0f, 0, 0);
        mMotionSeqMngr.setNo(mpHIO->m.common.motion, -1.0f, 0, 0);
        mJntAnm.lookNone(0);
        attention_info.flags = 0;
        break;
    case 3:
        break;
    }
    return 1;
}

const u16 daNpc_Sha_c::mEvtBitLabels[6] = {
    0xF0FF, 0xEFFF, 0xEEFF, 0xEDFF, 0xECFF, 0xEBFF,
};

const u16 daNpc_Sha_c::mTmpBitLabels[6] = {
    0xFAFF, 0xF9FF, 0xF8FF, 0xF7FF, 0xF6FF, 0xF5FF,
};

const int daNpc_Sha_c::mSceneChangeNoTable[48] = {
    1, 2, 3, 4,
    5, 6, 7, 8,
    9, 0xA, 0xB, 0xC,
    0xD, 0xE, 0xF, 0x10,
    0x11, 0x12, 0x13, 0x14,
    0x15, 0x16, 0x17, 0x18,
    0x19, 0x1A, 0x1B, 0x1C,
    0x1D, 0x1E, 0x1F, 0x20,
    0x21, 0x22, 0x23, 0x24,
    0x25, 0x26, 0x27, 0x28,
    0x29, 0x2A, 0x2B, 0x2C,
    0x2D, -1, -1, -1,
};

daNpc_Sha_c::queryFunc daNpc_Sha_c::mQueries[48] = {
    &daNpc_Sha_c::query000, &daNpc_Sha_c::query265, &daNpc_Sha_c::query000, &daNpc_Sha_c::query267,
    &daNpc_Sha_c::query000, &daNpc_Sha_c::query000, &daNpc_Sha_c::query078, &daNpc_Sha_c::query268,
    &daNpc_Sha_c::query265, &daNpc_Sha_c::query265, &daNpc_Sha_c::query078, &daNpc_Sha_c::query000,
    &daNpc_Sha_c::query000, &daNpc_Sha_c::query264, &daNpc_Sha_c::query000, &daNpc_Sha_c::query000,
    &daNpc_Sha_c::query000, &daNpc_Sha_c::query000, &daNpc_Sha_c::query078, &daNpc_Sha_c::query078,
    &daNpc_Sha_c::query265, &daNpc_Sha_c::query265, &daNpc_Sha_c::query266, &daNpc_Sha_c::query266,
    &daNpc_Sha_c::query267, &daNpc_Sha_c::query267, &daNpc_Sha_c::query268, &daNpc_Sha_c::query268,
    &daNpc_Sha_c::query542, &daNpc_Sha_c::query542, &daNpc_Sha_c::query267, &daNpc_Sha_c::query288,
    &daNpc_Sha_c::query266, &daNpc_Sha_c::query000, &daNpc_Sha_c::query000, &daNpc_Sha_c::query000,
    &daNpc_Sha_c::query264, &daNpc_Sha_c::query267, &daNpc_Sha_c::query461, &daNpc_Sha_c::query264,
    &daNpc_Sha_c::query264, &daNpc_Sha_c::query264, &daNpc_Sha_c::query266, &daNpc_Sha_c::query267,
    &daNpc_Sha_c::query264, &daNpc_Sha_c::query999, &daNpc_Sha_c::query999, &daNpc_Sha_c::query999,
};

BOOL daNpc_Sha_c::query265() {
    // dSv_event_flag_c::F_0265 - Arbiter's Grounds - Arbiter's Grounds clear
    return daNpcT_chkEvtBit(265);
}

BOOL daNpc_Sha_c::query268() {
    // dSv_event_flag_c::F_0268 - City in the Sky - City in the Sky clear
    return daNpcT_chkEvtBit(268);
}

BOOL daNpc_Sha_c::query267() {
    // dSv_event_flag_c::F_0267 - Temple of Time - Temple of Time clear
    return daNpcT_chkEvtBit(267);
}

BOOL daNpc_Sha_c::query078() {
    // dSv_event_flag_c::M_045 - Lakebed Temple - Lakebed Temple clear
    return daNpcT_chkEvtBit(78);
}

BOOL daNpc_Sha_c::query264() {
    // dSv_event_flag_c::F_0264 - Cutscene - Get master sword
    return daNpcT_chkEvtBit(264);
}

BOOL daNpc_Sha_c::query266() {
    // dSv_event_flag_c::F_0266 - Snowpeak Ruins - Snowpeak Ruins clear
    return daNpcT_chkEvtBit(266);
}

BOOL daNpc_Sha_c::query542() {
    // dSv_event_flag_c::F_0542 - Cutscene - [Cutscene] Hyrule Castle barrier disappears (Midna goes crazy)
    return daNpcT_chkEvtBit(542);
}

BOOL daNpc_Sha_c::query461() {
    // dSv_event_flag_c::F_0461 - Fishing Pond - First time entered fishing house
    return daNpcT_chkEvtBit(461);
}

BOOL daNpc_Sha_c::query288() {
    // dSv_event_flag_c::F_288 - Cutscene - [cutscene: ] Ilia gets her memories back
    return daNpcT_chkEvtBit(288);
}

BOOL daNpc_Sha_c::query000() {
    return TRUE;
}

BOOL daNpc_Sha_c::query999() {
    return FALSE;
}

static int daNpc_Sha_Create(void* i_this) {
    return static_cast<daNpc_Sha_c*>(i_this)->create();
}

static int daNpc_Sha_Delete(void* i_this) {
    return static_cast<daNpc_Sha_c*>(i_this)->Delete();
}

static int daNpc_Sha_Execute(void* i_this) {
    return static_cast<daNpc_Sha_c*>(i_this)->Execute();
}

static int daNpc_Sha_Draw(void* i_this) {
    return static_cast<daNpc_Sha_c*>(i_this)->Draw();
}

static int daNpc_Sha_IsDelete(void* i_this) {
    return 1;
}

static actor_method_class daNpc_Sha_MethodTable = {
    (process_method_func)daNpc_Sha_Create,
    (process_method_func)daNpc_Sha_Delete,
    (process_method_func)daNpc_Sha_Execute,
    (process_method_func)daNpc_Sha_IsDelete,
    (process_method_func)daNpc_Sha_Draw,
};

actor_process_profile_definition g_profile_NPC_SHAMAN = {
  fpcLy_CURRENT_e,        // mLayerID
  7,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_NPC_SHAMAN,        // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(daNpc_Sha_c),    // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  364,                    // mPriority
  &daNpc_Sha_MethodTable, // sub_method
  0x00040108,             // mStatus
  fopAc_NPC_e,            // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};
