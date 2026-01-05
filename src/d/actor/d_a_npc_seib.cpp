/**
* @file d_a_npc_seib.cpp
 *
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_npc_seib.h"

static int l_bmdData[2][1] = {12, 1};

static daNpcT_evtData_c l_evtList[3] = {
    {"", 0},
    {"DEFAULT_GETITEM", 0},
    {"NO_RESPONSE", 0},
};

static char* l_resNameList[2] = {"", "seiB"};

static s8 l_loadResPtrn0[2] = {1, -1};

static s8* l_loadResPtrnList[2] = {
    l_loadResPtrn0,
    l_loadResPtrn0,
};

static daNpcT_faceMotionAnmData_c l_faceMotionAnmData = {-1, 0, 0, -1, 0, 0, 0};

static daNpcT_motionAnmData_c l_motionAnmData[4] = {
    {9, 2, 1, -1, 0, 0, 0, 0},
    {7, 0, 1, -1, 0, 0, 0, 0},
    {6, 2, 1, -1, 0, 0, 0, 0},
    {8, 0, 1, -1, 0, 0, 0, 0},
};

static daNpcT_MotionSeqMngr_c::sequenceStepData_c l_faceMotionSequenceData[4] = {
    {-1, 0, 0},
    {-1, 0, 0},
    {-1, 0, 0},
    {-1, 0, 0},
};

static daNpcT_MotionSeqMngr_c::sequenceStepData_c l_motionSequenceData[16] = {
    {0, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {1, 0, 1}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {2, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {3, 0, 1}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
};

char* daNpc_seiB_c::mCutNameList = "";

daNpc_seiB_c::cutFunc daNpc_seiB_c::mCutList[1] = {
    0,
};

daNpc_seiB_c::~daNpc_seiB_c() {
    if (heap != NULL) {
        mpMorf[0]->stopZelAnime();
    }

    deleteRes(l_loadResPtrnList[mType], (const char**)l_resNameList);
}

daNpc_seiB_Param_c::Data const daNpc_seiB_Param_c::m = {
    0.0f,
    0.0f,
    1.0f,
    4000.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    1200.0f,
};

int daNpc_seiB_c::create() {
    daNpcT_ct(this, daNpc_seiB_c, &l_faceMotionAnmData, l_motionAnmData,
                       l_faceMotionSequenceData, 4, l_motionSequenceData, 4, l_evtList,
                       l_resNameList);

    mType = getType();
    mFlowNodeNo = getFlowNodeNo();
    mTwilight = false;

    int rv = loadRes(l_loadResPtrnList[mType], (const char**)l_resNameList);
    if (rv == cPhs_COMPLEATE_e) {
        if (!fopAcM_entrySolidHeap(this, createHeapCallBack, 0)) {
            return cPhs_ERROR_e;
        }

        OS_REPORT("\t(%s:%d) flowNo:%d<%08x> ", fopAcM_getProcNameString(this), mType, mFlowNodeNo,
                  fopAcM_GetParam(this));

        if (isDelete()) {
            OS_REPORT("===>isDelete:TRUE\n");
            return cPhs_ERROR_e;
        }

        fopAcM_SetMtx(this, mpMorf[0]->getModel()->getBaseTRMtx());
        mSound.init(&current.pos, &eyePos, 3, 1);
        mAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1, &mAcchCir,
                  fopAcM_GetSpeed_p(this), fopAcM_GetAngle_p(this), fopAcM_GetShapeAngle_p(this));
        mAcch.CrrPos(dComIfG_Bgsp());
        mGndChk = mAcch.m_gnd;
        mGroundH = mAcch.GetGroundH();
        setEnvTevColor();
        setRoomNo();

        mCcStts.Init(mpParam->m.mWeight, 0, this);
        reset();
        mCreating = true;
        Execute();
        mCreating = false;
    }

    return rv;
}

int daNpc_seiB_c::CreateHeap() {
    J3DModelData* mdlData_p =
        (J3DModelData*)dComIfG_getObjectRes(l_resNameList[l_bmdData[0][1]], l_bmdData[0][0]);
    JUT_ASSERT(466, NULL != mdlData_p);

    mpMorf[0] =
        new mDoExt_McaMorfSO(mdlData_p, NULL, NULL, NULL, -1, 1.0f, 0, -1, &mSound, 0, 0x11020284);
    if (mpMorf[0] && mpMorf[0]->getModel() == NULL) {
        mpMorf[0]->stopZelAnime();
        mpMorf[0] = NULL;
    }

    if (mpMorf[0] == NULL) {
        return 0;
    }

    mpMorf[0]->getModel()->setUserArea((uintptr_t)this);
    if (setFaceMotionAnm(0, false) && setMotionAnm(0, 0.0f, FALSE)) {
        return 1;
    }

    return 0;
}

int daNpc_seiB_c::Delete() {
    // OS_REPORT("|%06d:%x|daNpc_seiB_c -> Delete\n", g_Counter.mCounter0, this);
    fopAcM_GetID(this);
    this->~daNpc_seiB_c();
    return 1;
}

int daNpc_seiB_c::Execute() {
    return execute();
}

int daNpc_seiB_c::Draw() {
    if (mpMatAnm[0] != NULL) {
        J3DModelData* mdlData_p = mpMorf[0]->getModel()->getModelData();
        mdlData_p->getMaterialNodePointer(getEyeballMaterialNo())->setMaterialAnm(mpMatAnm[0]);
    }

    return draw(FALSE, TRUE, mRealShadowSize, NULL, 100.0f, FALSE, FALSE, FALSE);
}

int daNpc_seiB_c::createHeapCallBack(fopAc_ac_c* a_this) {
    daNpc_seiB_c* i_this = (daNpc_seiB_c*)a_this;
    return i_this->CreateHeap();
}

u8 daNpc_seiB_c::getType() {
    switch (fopAcM_GetParam(this) & 0xFF) {
    case 0:
        return TYPE_0;
    }

    return TYPE_1;
}

int daNpc_seiB_c::getFlowNodeNo() {
    u16 nodeNo = home.angle.x;
    if (nodeNo == 0xffff) {
        return -1;
    }
    return nodeNo;
}

int daNpc_seiB_c::isDelete() {
    return 0;
}

void daNpc_seiB_c::reset() {
    initialize();
    memset(&mActionFunc1, 0, (u8*)&field_0xe64 - (u8*)&mActionFunc1);
    if (mpMatAnm[0] != NULL) {
        mpMatAnm[0]->initialize();
    }
    setAngle(home.angle.y);
    mMotionSeqMngr.setNo(0, -1.0f, 0, 0);
    mAnim = 0;
}

void daNpc_seiB_c::setParam() {
    selectAction();
    srchActors();

    dComIfGp_getAttention()->getDistTable(0x28).mDistMax = mpParam->m.mDist;
    dComIfGp_getAttention()->getDistTable(0x28).mDistMaxRelease = mpParam->m.mDist;
    dComIfGp_getAttention()->getDistTable(0x27).mDistMax = mpParam->m.mDist;
    dComIfGp_getAttention()->getDistTable(0x27).mDistMaxRelease = mpParam->m.mDist;

    attention_info.distances[0] = 39;
    attention_info.distances[1] = 39;
    attention_info.distances[3] = 39;
    attention_info.flags = 0;

    scale.set(mpParam->m.mScale, mpParam->m.mScale, mpParam->m.mScale);
    mCcStts.SetWeight(mpParam->m.mWeight);
    mCylH = mpParam->m.mCylH;
    mWallR = mpParam->m.mWallR;
    mAcchCir.SetWallR(mWallR);
    mAcchCir.SetWallH(mpParam->m.mWallH);

    mRealShadowSize = mpParam->m.field_0xc;
    gravity = mpParam->m.mGravity;
    mExpressionMorfFrame = mpParam->m.field_0x6c;
    mMorfFrames = mpParam->m.mMorfFrames;
}

void daNpc_seiB_c::srchActors() {
    /* empty function */
}

BOOL daNpc_seiB_c::evtTalk() {
    if (chkAction(&daNpc_seiB_c::talk)) {
        (this->*mActionFunc2)(NULL);
    } else {
        mPreItemNo = 0;
        if (dComIfGp_event_chkTalkXY()) {
            if (!dComIfGp_evmng_ChkPresentEnd()) {
                return 1;
            }
            mEvtNo = 2;
            evtChange();
            return 1;
        }

        setAction(&daNpc_seiB_c::talk);
    }

    return 1;
}

BOOL daNpc_seiB_c::evtCutProc() {
    BOOL rv = 0;
    int staffID = dComIfGp_getEventManager().getMyStaffId("Seib", this, -1);
    if (staffID != -1) {
        mStaffId = staffID;
        staffID = dComIfGp_getEventManager().getMyActIdx(mStaffId, &mCutNameList, 1, 0, 0);
        if ((this->*(mCutList[staffID]))(mStaffId)) {
            dComIfGp_getEventManager().cutEnd(mStaffId);
        }
        rv = 1;
    }

    return rv;
}

void daNpc_seiB_c::action() {
    if (mActionFunc1) {
        if (mActionFunc2 == mActionFunc1) {
            (this->*mActionFunc2)(NULL);
        } else {
            setAction(mActionFunc1);
        }
    }
}

void daNpc_seiB_c::beforeMove() {
    if (checkHide() || mNoDraw) {
        attention_info.flags = 0;
    }
    ctrlWaitAnm();
}

void daNpc_seiB_c::setAttnPos() {
    setMtx();
    eyePos = current.pos;
    attention_info.position = current.pos;
}

int daNpc_seiB_c::drawDbgInfo() {
    return false;
}

bool daNpc_seiB_c::afterSetMotionAnm(int param_1, int param_2, f32 param_3, int param_4) {
    static struct {
        int field_0x0;
        u32 field_0x4;
    } btkAnmData[4] = {
        {18, 1},
        {18, 1},
        {18, 1},
        {18, 1},
    };

    static struct {
        int field_0x0;
        u32 field_0x4;
    } brkAnmData[4] = {
        {15, 1},
        {15, 1},
        {15, 1},
        {15, 1},
    };

    J3DAnmTextureSRTKey* anm_text = NULL;
    J3DAnmTevRegKey* anm_tev = NULL;
    if (btkAnmData[param_1].field_0x0 != -1) {
        anm_text = getTexSRTKeyAnmP(l_resNameList[btkAnmData[param_1].field_0x4],
                                    btkAnmData[param_1].field_0x0);
    }

    if (anm_text) {
        if (mBtkAnm.getBtkAnm() == anm_text) {
            mAnmFlags |= 0x80;
        } else if (setBtkAnm(anm_text, mpMorf[0]->getModel()->getModelData(), 1.0f, param_2)) {
            mAnmFlags |= 0x82;
        }
    }

    if (anm_text == NULL && btkAnmData[param_1].field_0x0 != -1) {
        return 0;
    }

    if (brkAnmData[param_1].field_0x0 != -1) {
        anm_tev = getTevRegKeyAnmP(l_resNameList[brkAnmData[param_1].field_0x4],
                                   brkAnmData[param_1].field_0x0);
    }

    if (anm_tev) {
        if (mBrkAnm.getBrkAnm() == anm_tev) {
            mAnmFlags |= 0x100;
        } else if (setBrkAnm(anm_tev, mpMorf[0]->getModel()->getModelData(), 1.0f, param_2)) {
            mAnmFlags |= 0x104;
        }
    }

    if (anm_tev == NULL && brkAnmData[param_1].field_0x0 != -1) {
        return 0;
    }

    return 1;
}

int daNpc_seiB_c::selectAction() {
    mActionFunc1 = NULL;
    mActionFunc1 = &daNpc_seiB_c::wait;
    return 1;
}

int daNpc_seiB_c::chkAction(int (daNpc_seiB_c::*action)(void*)) {
    return mActionFunc2 == action;
}

int daNpc_seiB_c::setAction(int (daNpc_seiB_c::*action)(void*)) {
    mMode = 3;
    if (mActionFunc2) {
        (this->*mActionFunc2)(NULL);
    }

    mMode = 0;
    mActionFunc2 = action;
    if (mActionFunc2) {
        (this->*mActionFunc2)(NULL);
    }

    return 1;
}

void daNpc_seiB_c::ctrlWaitAnm() {
    switch (mAnim) {
    case 0:
        if (mMotionSeqMngr.getNo() == 0) {
            if (mMorfLoops < 3) {
                return;
            }

            mMotionSeqMngr.setNo(1, -1.0f, 0, 0);
            mAnim++;
            return;
        }

        mMotionSeqMngr.setNo(0, -1.0f, 0, 0);
        return;

    case 1:
        if (mMotionSeqMngr.getNo() == 1) {
            if (mMotionSeqMngr.checkEndSequence() == 0) {
                return;
            }

            mMotionSeqMngr.setNo(2, -1.0f, 0, 0);
            mAnim++;
            return;
        }

        mMotionSeqMngr.setNo(1, -1.0f, 0, 0);
        return;

    case 2:
        if (mMotionSeqMngr.getNo() == 2) {
            if (mMorfLoops < 2) {
                return;
            }

            mMotionSeqMngr.setNo(3, -1.0f, 0, 0);
            mAnim++;
            return;
        }

        mMotionSeqMngr.setNo(2, -1.0f, 0, 0);
        return;

    case 3:
        if (mMotionSeqMngr.getNo() == 3) {
            if (mMotionSeqMngr.checkEndSequence() == 0) {
                return;
            }

            mMotionSeqMngr.setNo(0, -1.0f, 0, 0);
            mAnim = 0;
            return;
        }

        mMotionSeqMngr.setNo(3, -1.0f, 0, 0);
        return;
    }

    JUT_ASSERT(1068, FALSE);
}

int daNpc_seiB_c::wait(void* param_0) {
    switch (mMode) {
    case 0:
    case 1:
        mMode = 2;
        break;

    case 2:
    case 3:
        break;
    }

    return 1;
}

int daNpc_seiB_c::talk(void* param_0) {
    switch (mMode) {
    case 0:
    case 1:
        initTalk(mFlowNodeNo, NULL);
        field_0xe26 = 0;
        mMode = 2;
        // fallthrough
    case 2:
        if (talkProc(NULL, 0, NULL, 0)) {
            dComIfGp_event_reset();
            mMode = 3;
        }
        // fallthrough
    case 3:
        break;
    }

    return 0;
}

static int daNpc_seiB_Create(void* param_1) {
    daNpc_seiB_c* i_this = (daNpc_seiB_c*)param_1;
    return i_this->create();
}

static int daNpc_seiB_Delete(void* param_1) {
    daNpc_seiB_c* i_this = (daNpc_seiB_c*)param_1;
    return i_this->Delete();
}

static int daNpc_seiB_Execute(void* param_1) {
    daNpc_seiB_c* i_this = (daNpc_seiB_c*)param_1;
    return i_this->Execute();
}

static int daNpc_seiB_Draw(void* param_1) {
    daNpc_seiB_c* i_this = (daNpc_seiB_c*)param_1;
    return i_this->Draw();
}

static int daNpc_seiB_IsDelete(void* param_1) {
    return 1;
}

static daNpc_seiB_Param_c l_HIO;

static actor_method_class daNpc_seiB_MethodTable = {
    (process_method_func)daNpc_seiB_Create,  (process_method_func)daNpc_seiB_Delete,
    (process_method_func)daNpc_seiB_Execute, (process_method_func)daNpc_seiB_IsDelete,
    (process_method_func)daNpc_seiB_Draw,
};

actor_process_profile_definition g_profile_NPC_SEIB = {
    fpcLy_CURRENT_e,          // mLayerID
    7,                        // mListID
    fpcPi_CURRENT_e,          // mListPrio
    PROC_NPC_SEIB,            // mProcName
    &g_fpcLf_Method.base,     // sub_method
    sizeof(daNpc_seiB_c),     // mSize
    0,                        // mSizeOther
    0,                        // mParameters
    &g_fopAc_Method.base,     // sub_method
    358,                      // mPriority
    &daNpc_seiB_MethodTable,  // sub_method
    0x00044000,               // mStatus
    fopAc_ACTOR_e,            // mActorType
    fopAc_CULLBOX_CUSTOM_e,   // cullType
};
