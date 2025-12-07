/**
 * @file d_a_npc_seid.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_npc_seid.h"
#include "SSystem/SComponent/c_counter.h"

static int l_bmdData[1][2] = {
    {5, 1}
};

static daNpcT_evtData_c l_evtList[3] = {
    {"", 0},
    {"DEFAULT_GETITEM", 0},
    {"NO_RESPONSE",0 }
};

static char* l_resNameList[2] = {
    "",
    "seiD",
};

static s8 l_loadResPtrn0[2] = {
    1, -1
};

static s8* l_loadResPtrnList[2] = {
    l_loadResPtrn0,
    l_loadResPtrn0,
};

static daNpcT_faceMotionAnmData_c l_faceMotionAnmData[1] = {
    {-1, 0, 0, -1, 0, 0, 0},
};

static daNpcT_motionAnmData_c l_motionAnmData[1] = {
    {-1, 2, 0, -1, 0, 0, 0, 0},
};

static daNpcT_MotionSeqMngr_c::sequenceStepData_c l_faceMotionSequenceData[4] = {
    {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
};

static daNpcT_MotionSeqMngr_c::sequenceStepData_c l_motionSequenceData[4] = {
    {0, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
};

char* daNpc_seiD_c::mCutNameList = "";

daNpc_seiD_c::cutFunc daNpc_seiD_c::mCutList[1] = {
    0
};

daNpc_seiD_c::~daNpc_seiD_c() {
    OS_REPORT("|%06d:%x|daNpc_seiD_c -> デストラクト\n", g_Counter.mCounter0, this);

    if (heap != NULL) {
        mpMorf[0]->stopZelAnime();
    }

    #if DEBUG
    if (field_0xe40 != NULL) {
        field_0xe40->removeHIO();
    }
    #endif

    deleteRes(l_loadResPtrnList[mType], (const char**)l_resNameList);
}

daNpc_seiD_HIOParam const daNpc_seiD_Param_c::m = {
    0.0f,
    0.0f,
    1.0f,
    4000.0f,
    255.0f,
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

static daNpc_seiD_Param_c l_HIO;

int daNpc_seiD_c::create() {
    daNpcT_ct(this, daNpc_seiD_c, l_faceMotionAnmData, l_motionAnmData, l_faceMotionSequenceData, 
        4, l_motionSequenceData, 4, l_evtList, l_resNameList);

    mType = getType();
    mFlowNodeNo = getFlowNodeNo();
    mTwilight = false;

    int phase = loadRes(l_loadResPtrnList[mType], (const char**)l_resNameList);
    if (phase == cPhs_COMPLEATE_e) {
        if (!fopAcM_entrySolidHeap(this, createHeapCallBack, 0)) {
            return cPhs_ERROR_e;
        }

        OS_REPORT("\t(%s:%d) flowNo:%d<%08x> ", fopAcM_getProcNameString(this), mType, mFlowNodeNo, fopAcM_GetParam(this));

        if (isDelete()) {
            OS_REPORT("===>isDelete:TRUE\n");
            return cPhs_ERROR_e;
        }

        OS_REPORT("\n");
        fopAcM_SetMtx(this, mpMorf[0]->getModel()->getBaseTRMtx());
        mSound.init(&current.pos, &eyePos, 3, 1);

#if DEBUG
        //field_0xe40->field_0x8 = &l_HIO;
        field_0xe40->entryHIO("光の精霊ｄ"); // Spirit of Light d
#endif

        mAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1, &mAcchCir, 
        fopAcM_GetSpeed_p(this), fopAcM_GetAngle_p(this), fopAcM_GetShapeAngle_p(this));
        mAcch.CrrPos(dComIfG_Bgsp());
        mGndChk = mAcch.m_gnd;
        mGroundH = mAcch.GetGroundH();
        setEnvTevColor();
        setRoomNo();

        mCcStts.Init(daNpc_seiD_Param_c::m.mSttsWeight, 0, this);

        reset();
        mCreating = true;
        Execute();
        mCreating = false;
    }

    return phase;
}

int daNpc_seiD_c::CreateHeap() {
    J3DModelData* mdlData_p = (J3DModelData*)dComIfG_getObjectRes(l_resNameList[l_bmdData[0][1]], l_bmdData[0][0]);
    JUT_ASSERT(424, NULL != mdlData_p);

    mpMorf[0] = new mDoExt_McaMorfSO(mdlData_p, NULL, NULL, NULL, -1, 1.0f, 0, -1, &mSound, 0, 0x11020284);
    if (mpMorf[0] != NULL && mpMorf[0]->getModel() == NULL) {
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

int daNpc_seiD_c::Delete() {
    OS_REPORT("|%06d:%x|daNpc_seiD_c -> Delete\n", g_Counter.mCounter0, this);
    fopAcM_GetID(this);
    this->~daNpc_seiD_c();
    return 1;
}

int daNpc_seiD_c::Execute() {
    return execute();
}

int daNpc_seiD_c::Draw() {
    daNpcT_MatAnm_c* matAnm = mpMatAnm[0];
    if (matAnm != NULL) {
        J3DModelData* modelData = mpMorf[0]->getModel()->getModelData();
        J3DMaterial* material = modelData->getMaterialNodePointer(getEyeballMaterialNo());
        material->setMaterialAnm(matAnm);
    }
    return draw(FALSE, TRUE, mRealShadowSize, NULL, 100.0f, FALSE, FALSE, FALSE);
}

int daNpc_seiD_c::createHeapCallBack(fopAc_ac_c* actor) {
    daNpc_seiD_c* i_this = (daNpc_seiD_c*)actor;
    return i_this->CreateHeap();
}

u8 daNpc_seiD_c::getType() {
    switch ((u8)fopAcM_GetParam(this)) {
        case 0:
            return TYPE_0;
    }

    return TYPE_1;
}

u32 daNpc_seiD_c::getFlowNodeNo() {
    u16 rv = home.angle.x;
    if (rv == 0xffff) {
        return -1;
    }
    return rv;
}

int daNpc_seiD_c::isDelete() {
    return 0;
}

void daNpc_seiD_c::reset() {
    initialize();
    int size = (u8*)&field_0xe60 - (u8*)&mAction;
    memset(&mAction, 0, size);

    if (mpMatAnm[0] != NULL) {
        mpMatAnm[0]->initialize();
    }

    setAngle(home.angle.y);
    mMotionSeqMngr.setNo(0, -1.0f, 0, 0);
}

void daNpc_seiD_c::setParam() {
    selectAction();
    srchActors();
    
    dComIfGp_getAttention()->getDistTable(0x28).mDistMax = daNpc_seiD_Param_c::m.field_0x90;
    dComIfGp_getAttention()->getDistTable(0x28).mDistMaxRelease = daNpc_seiD_Param_c::m.field_0x90;
    dComIfGp_getAttention()->getDistTable(0x27).mDistMax = daNpc_seiD_Param_c::m.field_0x90;
    dComIfGp_getAttention()->getDistTable(0x27).mDistMaxRelease = daNpc_seiD_Param_c::m.field_0x90;

    attention_info.distances[0] = 39;
    attention_info.distances[1] = 39;
    attention_info.distances[3] = 39;
    attention_info.flags = 0;

    scale.set(daNpc_seiD_Param_c::m.mScale, daNpc_seiD_Param_c::m.mScale, daNpc_seiD_Param_c::m.mScale);
    mCcStts.SetWeight(daNpc_seiD_Param_c::m.mSttsWeight);
    mCylH = daNpc_seiD_Param_c::m.mCylH;
    mWallR = daNpc_seiD_Param_c::m.mWallR;
    mAcchCir.SetWallR(mWallR);
    mAcchCir.SetWallH(daNpc_seiD_Param_c::m.mWallH);
    mRealShadowSize = daNpc_seiD_Param_c::m.field_0x0c;
    gravity = daNpc_seiD_Param_c::m.mGravity;
    mExpressionMorfFrame = daNpc_seiD_Param_c::m.field_0x6c;
    mMorfFrames = daNpc_seiD_Param_c::m.mMorfFrames;
}

void daNpc_seiD_c::srchActors() {
    /* empty function */
}

BOOL daNpc_seiD_c::evtTalk() {
    if (chkAction(&daNpc_seiD_c::talk) != 0) {
        (this->*mAction2)(NULL);
    } else {
        mPreItemNo = 0;
        if (dComIfGp_event_chkTalkXY()) {
            if (!dComIfGp_evmng_ChkPresentEnd()) {
                return TRUE;
            }
            mEvtNo = 2;
            evtChange();
            return TRUE;
        }

        setAction(&daNpc_seiD_c::talk);
    }

    return TRUE;
}

BOOL daNpc_seiD_c::evtCutProc() {
    BOOL rv = 0;
    int staffID = dComIfGp_getEventManager().getMyStaffId("Seid", this, -1);
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

void daNpc_seiD_c::action() {
    if (mAction) {
        if (mAction2 == mAction) {
            (this->*mAction2)(NULL);
        } else {
            setAction(mAction);
        }
    }
}

void daNpc_seiD_c::beforeMove() {
    if (checkHide() || mNoDraw) {
        attention_info.flags = 0;
    }
    ctrlWaitAnm();
}

void daNpc_seiD_c::setAttnPos() {
    setMtx();
    eyePos = current.pos;
    attention_info.position = current.pos;
}

int daNpc_seiD_c::drawDbgInfo() {
    return 0;
}

bool daNpc_seiD_c::afterSetMotionAnm(int param_1, int param_2, f32 param_3, int param_4) {
    static struct {
        int field_0x0;
        u32 field_0x4;
    } btkAnmData[1] = {
        {11, 1},
    };

    static struct {
        int field_0x0;
        u32 field_0x4;
    } brkAnmData[1] = {
        {8, 1},
    };

    J3DAnmTextureSRTKey* anm_text = NULL;
    J3DAnmTevRegKey* anm_tev = NULL;
    if (btkAnmData[param_1].field_0x0 != -1) {
        anm_text = getTexSRTKeyAnmP(l_resNameList[btkAnmData[param_1].field_0x4], btkAnmData[param_1].field_0x0);
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
        anm_tev = getTevRegKeyAnmP(l_resNameList[brkAnmData[param_1].field_0x4], brkAnmData[param_1].field_0x0);
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

int daNpc_seiD_c::selectAction() {
    mAction = NULL;
    mAction = &daNpc_seiD_c::wait;
    return 1;
}

int daNpc_seiD_c::chkAction(int (daNpc_seiD_c::*action)(void*)) {
    return mAction2 == action;
}

int daNpc_seiD_c::setAction(int (daNpc_seiD_c::*action)(void*)) {
    mMode = 3;
    if (mAction2) {
        (this->*mAction2)(NULL);
    }

    mMode = 0;
    mAction2 = action;
    if (mAction2) {
        (this->*mAction2)(NULL); 
    }

    return 1;
}

void daNpc_seiD_c::ctrlWaitAnm() {
    /* empty function */
}

int daNpc_seiD_c::wait(void* param_1) {
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

int daNpc_seiD_c::talk(void* param_1) {
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

static int daNpc_seiD_Create(void* param_1) {
    daNpc_seiD_c* i_this = (daNpc_seiD_c*)param_1;
    return i_this->create();
}

static int daNpc_seiD_Delete(void* param_1) {
    daNpc_seiD_c* i_this = (daNpc_seiD_c*)param_1;
    return i_this->Delete();
}

static int daNpc_seiD_Execute(void* param_1) {
    daNpc_seiD_c* i_this = (daNpc_seiD_c*)param_1;
    return i_this->Execute();
}

static int daNpc_seiD_Draw(void* param_1) {
    daNpc_seiD_c* i_this = (daNpc_seiD_c*)param_1;
    return i_this->Draw();
}

static int daNpc_seiD_IsDelete(void* param_1) {
    return 1;
}

static actor_method_class daNpc_seiD_MethodTable = {
    (process_method_func)daNpc_seiD_Create,
    (process_method_func)daNpc_seiD_Delete,
    (process_method_func)daNpc_seiD_Execute,
    (process_method_func)daNpc_seiD_IsDelete,
    (process_method_func)daNpc_seiD_Draw,
};

extern actor_process_profile_definition g_profile_NPC_SEID = {
  fpcLy_CURRENT_e,         // mLayerID
  7,                       // mListID
  fpcPi_CURRENT_e,         // mListPrio
  PROC_NPC_SEID,           // mProcName
  &g_fpcLf_Method.base,   // sub_method
  sizeof(daNpc_seiD_c),    // mSize
  0,                       // mSizeOther
  0,                       // mParameters
  &g_fopAc_Method.base,    // sub_method
  360,                     // mPriority
  &daNpc_seiD_MethodTable, // sub_method
  0x00044000,              // mStatus
  fopAc_ACTOR_e,           // mActorType
  fopAc_CULLBOX_CUSTOM_e,  // cullType
};
