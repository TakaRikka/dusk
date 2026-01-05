/**
* @file d_a_npc_fairy_seirei.cpp
 *
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_npc_fairy_seirei.h"
#include "d/actor/d_a_npc4.h"


static daNpcT_evtData_c l_evtList[1] = {
    {"", 0},
};

static char* l_resNameList[2] = {
    "",
    NULL,
};

static s8 l_loadResPtrn0[2] = {
    1,
    -1,
};

static s8* l_loadResPtrnList[4] = {
    l_loadResPtrn0,
    l_loadResPtrn0,
    l_loadResPtrn0,
    l_loadResPtrn0,
};

static daNpcT_faceMotionAnmData_c l_faceMotionAnmData = {
    -1, 0, 0, -1, 0, 0, 0,
};

static daNpcT_motionAnmData_c l_motionAnmData[1] = {
    {-1, 0, 0, -1, 0, 0, 0},
};

static daNpcT_MotionSeqMngr_c::sequenceStepData_c l_faceMotionSequenceData[4] = {
    {-1, 0, 0},
    {-1, 0, 0},
    {-1, 0, 0},
    {-1, 0, 0},
};

static daNpcT_MotionSeqMngr_c::sequenceStepData_c l_motionSequenceData[4] = {
    {-1, 0, 0},
    {-1, 0, 0},
    {-1, 0, 0},
    {-1, 0, 0},
};

char* daNpc_FairySeirei_c::mCutNameList[1] = {
    "",
};

daNpc_FairySeirei_c::cutFunc daNpc_FairySeirei_c::mCutList[1] = {
    NULL,
};

daNpc_FairySeirei_c::~daNpc_FairySeirei_c() {
    if (heap != NULL) {
        mpMorf[0]->stopZelAnime();
    }
}


const f32 daNpc_FairySeirei_Param_c::m[37] = {
    600.0f, 0.0f, 1.0f, 4000.0f, 255.0f, 200.0f, 0.0f, 60.0f, 0.0f, 0.0f, 0.0f,   0.0f, 0.0f,
    0.0f,   0.0f, 0.0f, 0.0f,    0.0f,   0.0f,   0.0f, 0.0f,  0.0f, 0.0f, 0.0f,   0.0f, 0.0f,
    0.0f,   0.0f, 0.0f, 0.0f,    0.0f,   0.0f,   0.0f, 0.0f,  0.0f, 0.0f, 600.0f,
};

int daNpc_FairySeirei_c::create() {
    daNpcT_ct(this, daNpc_FairySeirei_c, &l_faceMotionAnmData, l_motionAnmData,
                       l_faceMotionSequenceData, 4, l_motionSequenceData, 4, l_evtList,
                       l_resNameList);
    mType = getType();
    mFlowNodeNo = getFlowNodeNo();
    mTwilight = false;
    if (isDelete()) {
        return cPhs_ERROR_e;
    }
    mAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1, &mAcchCir,
              fopAcM_GetSpeed_p(this), fopAcM_GetAngle_p(this), fopAcM_GetShapeAngle_p(this));
    mCcStts.Init(daNpc_FairySeirei_Param_c::m[4], 0, this);
    mCyl.Set(mCcDCyl);
    mCyl.SetStts(&mCcStts);
    mAcch.CrrPos(dComIfG_Bgsp());
    mGndChk = mAcch.m_gnd;
    mGroundH = mAcch.GetGroundH();
    if (mGroundH != -G_CM3D_F_INF) {
        setEnvTevColor();
        setRoomNo();
    }
    current.pos = home.pos;
    old.pos = home.pos;
    reset();
    mCreating = true;
    Execute();
    mCreating = false;
    return cPhs_COMPLEATE_e;
}

int daNpc_FairySeirei_c::Delete() {
    this->~daNpc_FairySeirei_c();
    return 1;
}

int daNpc_FairySeirei_c::Execute() {
    return execute();
}

int daNpc_FairySeirei_c::Draw() {
    return 1;
}

int daNpc_FairySeirei_c::isDelete() {
    int ret = 0;
    if (daNpcT_chkEvtBit(0x1F9) == false) {
        ret = 1;
    }
    return ret;
}


void daNpc_FairySeirei_c::reset() {
    int iVar1 = (u8*)&field_0xFAC - (u8*)&mInitFunc;
    if (mpMatAnm[0] != NULL) {
        mpMatAnm[0]->initialize();
    }
    initialize();
    memset(&mInitFunc, 0, iVar1);
    mMotionSeqMngr.setNo(0, -1.0f, 0, 0);
    setAngle(home.angle.y);
}

void daNpc_FairySeirei_c::setParam() {
    selectAction();
    srchActors();
    dComIfGp_getAttention()->getDistTable(0x28).mDistMax = daNpc_FairySeirei_Param_c::m[36];
    dComIfGp_getAttention()->getDistTable(0x28).mDistMaxRelease = daNpc_FairySeirei_Param_c::m[36];
    dComIfGp_getAttention()->getDistTable(0x27).mDistMax = daNpc_FairySeirei_Param_c::m[36];
    dComIfGp_getAttention()->getDistTable(0x27).mDistMaxRelease = daNpc_FairySeirei_Param_c::m[36];
    attention_info.distances[fopAc_attn_LOCK_e] = 0x27;
    attention_info.distances[fopAc_attn_TALK_e] = 0x27;
    attention_info.distances[fopAc_attn_SPEAK_e] = 0x27;
    attention_info.flags = fopAc_AttnFlag_SPEAK_e;
    mCcStts.SetWeight(daNpc_FairySeirei_Param_c::m[4]);
    mCylH = daNpc_FairySeirei_Param_c::m[5];
    mWallR = daNpc_FairySeirei_Param_c::m[7];
    mAttnFovY = daNpc_FairySeirei_Param_c::m[20];
    mAcchCir.SetWallR(mWallR);
    mAcchCir.SetWallH(daNpc_FairySeirei_Param_c::m[6]);
    mRealShadowSize = daNpc_FairySeirei_Param_c::m[3];
    mExpressionMorfFrame = daNpc_FairySeirei_Param_c::m[27];
    mMorfFrames = daNpc_FairySeirei_Param_c::m[17];
    gravity = daNpc_FairySeirei_Param_c::m[1];
}

void daNpc_FairySeirei_c::srchActors() {
    /* empty function */
}

BOOL daNpc_FairySeirei_c::evtTalk() {
    if (chkAction(&daNpc_FairySeirei_c::talk)) {
        (this->*(mExecuteFunc))(0);
    } else {
        mPreItemNo = 0;
        if (dComIfGp_event_chkTalkXY()) {
            if (!dComIfGp_evmng_ChkPresentEnd()) {
                return true;
            }
            dComIfGp_event_reset();
            return true;
        }
        setAction(&daNpc_FairySeirei_c::talk);
    }
    return true;
}


BOOL daNpc_FairySeirei_c::evtCutProc() {
    dEvent_manager_c* temp_r31;
    s32 temp_r3;

    int staff_id = dComIfGp_getEventManager().getMyStaffId("Seirei", this, -1);
    if (staff_id != -1) {
        mStaffId = staff_id;
        int actIdx = dComIfGp_getEventManager().getMyActIdx(mStaffId, &mCutNameList[0], 1, 0, 0);
        if ((this->*(mCutList[actIdx]))(mStaffId) != 0) {
            dComIfGp_getEventManager().cutEnd(mStaffId);
        }
        return true;
    }
    return false;
}

void daNpc_FairySeirei_c::action() {
    if (mInitFunc != NULL) {
        if (mExecuteFunc == mInitFunc) {
            (this->*(mExecuteFunc))(0);
        } else {
            setAction(mInitFunc);
        }
    }
}

void daNpc_FairySeirei_c::beforeMove() {
    if (checkHide() || mNoDraw) {
        attention_info.flags = 0;
    }
}

void daNpc_FairySeirei_c::setCollision() {
    u32 var_r5;
    u32 var_r4;

    cXyz center;
    var_r5 = 0xd8fbfdff;
    var_r4 = 0x1f;
    if (mHide == 0) {
        if (dComIfGp_event_runCheck()) {
            var_r5 = 0;
            var_r4 = 0;
        }
        f32 height = mCylH;
        f32 radius = mWallR;
        center = current.pos;
        mCyl.SetCoSPrm(0x79);
        mCyl.SetTgType(var_r5);
        mCyl.SetTgSPrm(var_r4);
        mCyl.OnTgNoHitMark();
        mCyl.SetH(height);
        mCyl.SetR(radius);
        mCyl.SetC(center);
        dComIfG_Ccsp()->Set(&mCyl);
    }
    mCyl.ClrCoHit();
    mCyl.ClrTgHit();
}


void daNpc_FairySeirei_c::setAttnPos() {
    attention_info.position = current.pos;
    attention_info.position.y += daNpc_FairySeirei_Param_c::m[0] - 350.0f;
    eyePos = attention_info.position;
    setPrtcls();
    ptcl_se();
}

BOOL daNpc_FairySeirei_c::drawDbgInfo() {
    return false;
}

bool daNpc_FairySeirei_c::selectAction() {
    mInitFunc = NULL;
    mInitFunc = &daNpc_FairySeirei_c::wait;
    return true;
}

bool daNpc_FairySeirei_c::setAction(actionFunc i_action) {
    mMode = 3;
    if (mExecuteFunc != NULL) {
        (this->*(mExecuteFunc))(0);
    }
    mMode = 0;
    mExecuteFunc = i_action;
    if (mExecuteFunc != NULL) {
        (this->*(mExecuteFunc))(0);
    }
    return true;
}


void daNpc_FairySeirei_c::setPrtcls() {
    static u16 const id[3] = {
        0x8AA3,
        0x8AA4,
        0x8AA5,
    };

    cXyz loc;
    cXyz scale(1.0f, 1.0f, 1.0f);
    for (int i = 0; i < 3; i++) {
        loc = attention_info.position;
        loc.y -= 200.0f;
        mpEmitters[i] = dComIfGp_particle_set(mpEmitters[i], id[i], &loc, &mCurAngle, &scale);
        JPABaseEmitter* emitter_p = dComIfGp_particle_getEmitter(mpEmitters[i]);
        if (emitter_p != NULL) {
            emitter_p->setGlobalTranslation(loc.x, loc.y, loc.z);
        }
    }
}

int daNpc_FairySeirei_c::wait(int param_0) {
    switch (mMode) {
    case 0:
    case 1:
        mMode = 2;
    case 2:
        if (daNpcT_chkDoBtnIsSpeak(this)) {
            dComIfGp_setDoStatus(8, 0);
        }
    case 3:
    default:
        break;
    }
    return 1;
}

int daNpc_FairySeirei_c::talk(int param_0) {
    switch (mMode) {
    case 0:
    case 1:
        daNpcF_offTmpBit(0xB);
        initTalk(mFlowNodeNo, NULL);
        mMode = 2;

    case 2:
        if (talkProc(0, 0, NULL, 0)) {
            if (daNpcT_chkTmpBit(0xB)) {
                _to_FairyCave();
                return 0;
            }
            int itemNo;
            if ((int)mFlow.getEventId(&itemNo) == 1) {
                if (mItemPartnerId == fpcM_ERROR_PROCESS_ID_e) {
                    mItemPartnerId = fopAcM_createItemForPresentDemo(&current.pos, itemNo, 0, -1, -1, NULL,
                                                              NULL);
                }
                if (fopAcM_IsExecuting(mItemPartnerId)) {
                    mTalking = TRUE;
                    evtChange();
                }
            } else {
                if (mFlow.checkEndFlow()) {
                    dComIfGp_event_reset();
                    mTalking = FALSE;
                    mMode = 3;
                }
            }
        }
        field_0xe26 = FALSE;
        break;

    case 3:
    default:
        break;
    }
    return 0;
}


void daNpc_FairySeirei_c::_to_FairyCave() {
    static const int sTempBit[4] = {
        0x79,
        0x7A,
        0x7B,
        0x7C,
    };

    daNpcT_onTmpBit(sTempBit[mType]);
    dStage_changeScene(getSeneNo(), 0.0f, 0, fopAcM_GetRoomNo(this), 0, -1);
}

static int daNpc_FairySeirei_Create(void* i_this) {
    return static_cast<daNpc_FairySeirei_c*>(i_this)->create();
}

static int daNpc_FairySeirei_Delete(void* i_this) {
    return static_cast<daNpc_FairySeirei_c*>(i_this)->Delete();
}

static int daNpc_FairySeirei_Execute(void* i_this) {
    return static_cast<daNpc_FairySeirei_c*>(i_this)->Execute();
}

static int daNpc_FairySeirei_Draw(void* i_this) {
    return static_cast<daNpc_FairySeirei_c*>(i_this)->Draw();
}

static int daNpc_FairySeirei_IsDelete(void* i_this) {
    return 1;
}


static daNpc_FairySeirei_Param_c l_HIO;

static actor_method_class daNpc_FairySeirei_MethodTable = {
    daNpc_FairySeirei_Create,   daNpc_FairySeirei_Delete, daNpc_FairySeirei_Execute,
    daNpc_FairySeirei_IsDelete, daNpc_FairySeirei_Draw,
};

actor_process_profile_definition g_profile_NPC_FAIRY_SEIREI = {
    fpcLy_CURRENT_e,                 // mLayerID
    7,                               // mListID
    fpcPi_CURRENT_e,                 // mListPrio
    PROC_NPC_FAIRY_SEIREI,           // mProcName
    &g_fpcLf_Method.base,            // sub_method
    sizeof(daNpc_FairySeirei_c),     // mSize
    0,                               // mSizeOther
    0,                               // mParameters
    &g_fopAc_Method.base,            // sub_method
    339,                             // mPriority
    &daNpc_FairySeirei_MethodTable,  // sub_method
    0x00044000,                      // mStatus
    fopAc_ACTOR_e,                   // mActorType
    fopAc_CULLBOX_CUSTOM_e,          // cullType
};
