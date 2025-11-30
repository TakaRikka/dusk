/**
 * @file d_a_npc_drainSol.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_npc_drainSol.h"
#include "d/actor/d_a_npc.h"

static int l_bmdGetParamList[] = {
    9, 7
};

static daNpc_GetParam3 l_bckGetParamList[] = {
    {5, 2},
    {4, 2},
    {4, 0},
};

static char* l_arcNames[2] = {
    "DrainSol1",
    "DrainSol2",
};

daNpcDrSol_c::daNpcDrSol_c() {}

daNpcDrSol_c::~daNpcDrSol_c() {
    dComIfG_resDelete(&mPhase, l_arcNames[mType]);

    if (heap != NULL) {
        mAnm_p->stopZelAnime();
    }
}

const daNpcDrSol_HIOParam daNpcDrSol_Param_c::m = {
    40.0f,
    0.0f,
    1.0f,
    300.0f,
    255.0f,
    160.0f,
    35.0f,
    40.0f,
    0.0f,
    0.0f,
    10.0f,
    -10.0f,
    30.0f,
    -10.0f,
    45.0f,
    -45.0f,
    0.6f,
    12.0f,
    3,
    6,
    4,
    6,
    60.0f,
    500.0f,
    300.0f,
    -300.0f,
    60,
    0,
    0,
    0,
    0,
    0,
    0,
};

int daNpcDrSol_c::Create() {
    fopAcM_ct(this, daNpcDrSol_c);

    if (!dKy_darkworld_check()) {
        return cPhs_ERROR_e;
    }

    mType = getType();
    mMessageNo = getMessageNo();

    int phase_state = cPhs_ERROR_e;
    phase_state = dComIfG_resLoad(&mPhase, l_arcNames[mType]);
    if (phase_state == cPhs_COMPLEATE_e) {
        if (!fopAcM_entrySolidHeap(this, createHeapCallBack, 0x29B0)) {
            return cPhs_ERROR_e;
        }

        J3DModelData* model_data = mAnm_p->getModel()->getModelData();
        fopAcM_SetMtx(this, mAnm_p->getModel()->getBaseTRMtx());
        fopAcM_setCullSizeBox(this, -60.0f, -10.0f, -60.0f, 60.0f, 200.0f, 60.0f);
        mSound.init(&current.pos, &eyePos, 3, 1);

        mAcchCir.SetWall(daNpcDrSol_Param_c::m.common.width, daNpcDrSol_Param_c::m.common.knee_length);
        mAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1,
                  &mAcchCir, fopAcM_GetSpeed_p(this), fopAcM_GetAngle_p(this), fopAcM_GetShapeAngle_p(this));
        mAcch.CrrPos(dComIfG_Bgsp());

        mCcStts.Init(0xFF, 0, this);
        mCyl.Set(mCcDCyl);
        mCyl.SetStts(&mCcStts);
        mCyl.SetTgType(0);
        mCyl.SetTgSPrm(0);

        mGndChk = mAcch.m_gnd;
        mGroundH = mAcch.GetGroundH();

        setEnvTevColor();
        setRoomNo();

        reset();
        Execute();
    }

    return phase_state;
}

int daNpcDrSol_c::CreateHeap() {
    J3DModelData* mdlData_p = (J3DModelData*)dComIfG_getObjectRes(l_arcNames[mType], l_bmdGetParamList[mType]);
    JUT_ASSERT(268, NULL != mdlData_p);

    mAnm_p = new mDoExt_McaMorfSO(mdlData_p, NULL, NULL, NULL, -1, 1.0f, 0, -1, &mSound, 0x80000, 0x11020084);
    if (mAnm_p != NULL && mAnm_p->getModel() == NULL) {
        mAnm_p->stopZelAnime();
        mAnm_p = NULL;
    }

    if (mAnm_p == NULL) {
        return 0;
    }

    Vec sp28[] = {
        {1.0f, 1.0f, 1.0f},
        {1.0f, 1.1983438f, 2.173357f},
    };
    cXyz sp18(sp28[mType]);

    mAnm_p->offTranslate();
    mAnm_p->setTranslateScale(sp18);

    if (mType == 0) {
        mdlData_p = (J3DModelData*)dComIfG_getObjectRes(l_arcNames[0], 8);
        JUT_ASSERT(305, NULL != mdlData_p);
        field_0xbd8 = mDoExt_J3DModel__create(mdlData_p, 0x80000, 0x11000084);
        if (field_0xbd8 == NULL) {
            return 0;
        }
    }

    return 1;
}

int daNpcDrSol_c::Delete() {
    fpc_ProcID id = fopAcM_GetID(this);
    this->~daNpcDrSol_c();
    return 1;
}

int daNpcDrSol_c::Execute() {
    execute();

    if (daPy_py_c::checkNowWolfEyeUp() && !fopAcM_CheckCondition(this, 4) && chkFindPlayer() && getSwitchNo() != 0xFF) {
        fopAcM_onSwitch(this, getSwitchNo());
    }

    return 1;
}

int daNpcDrSol_c::Draw() {
    draw(FALSE, FALSE, daNpcDrSol_Param_c::m.common.real_shadow_size, NULL, 0);
    return 1;
}

void daNpcDrSol_c::drawOtherMdls() {
    if (daPy_py_c::checkNowWolfEyeUp() && field_0xbd8 != NULL) {
        g_env_light.setLightTevColorType_MAJI(field_0xbd8, &tevStr);
        mDoMtx_stack_c::copy(mAnm_p->getModel()->getAnmMtx(14));
        field_0xbd8->setBaseTRMtx(mDoMtx_stack_c::get());
        mDoExt_modelUpdateDL(field_0xbd8);
    }
}

int daNpcDrSol_c::createHeapCallBack(fopAc_ac_c* actor) {
    return ((daNpcDrSol_c*)actor)->CreateHeap();
}

void daNpcDrSol_c::playMotion() {
    daNpcF_anmPlayData dat0 = {0, daNpcDrSol_Param_c::m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat0[] = {&dat0};

    daNpcF_anmPlayData dat1 = {1, daNpcDrSol_Param_c::m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat1[] = {&dat1};

    daNpcF_anmPlayData dat2a = {2, daNpcDrSol_Param_c::m.common.morf_frame, 1};
    daNpcF_anmPlayData dat2b = {1, 0.0f, 0};
    daNpcF_anmPlayData* pDat2[] = {&dat2a, &dat2b};

    daNpcF_anmPlayData dat3a = {0, daNpcDrSol_Param_c::m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat3[] = {&dat3a};

    daNpcF_anmPlayData** ppDat[5] = {
        pDat0,
        pDat1,
        pDat3,
        pDat3,
        pDat2,
    };

    if (mMotion >= 0 && mMotion < 5) {
        field_0xd34 = mMotionPhase;
        playMotionAnm(ppDat);
    }
}

void daNpcDrSol_c::reset() {
    initialize();

    mAction = NULL;

    mMode = 0;
    gravity = 0.0f;

    current.pos = home.pos;
    old.pos = current.pos;

    current.angle.set(0, home.angle.y, 0);
    old.angle = current.angle;
    shape_angle = current.angle;
    mCurAngle = current.angle;
    mOldAngle = mCurAngle;

    speedF = 0.0f;
    speed.setall(0.0f);

    mMotionMorfOverride = 0.0f;

    mTwilight = dKy_darkworld_check();

    attention_info.distances[fopAc_attn_LOCK_e] = getDistTableIdx(daNpcDrSol_Param_c::m.common.attention_distance, daNpcDrSol_Param_c::m.common.attention_angle);
    attention_info.distances[fopAc_attn_TALK_e] = attention_info.distances[fopAc_attn_LOCK_e];
    attention_info.distances[fopAc_attn_SPEAK_e] = getDistTableIdx(daNpcDrSol_Param_c::m.common.talk_distance, daNpcDrSol_Param_c::m.common.talk_angle);

    setAction(&daNpcDrSol_c::wait);
}

int daNpcDrSol_c::setAction(bool (daNpcDrSol_c::*i_action)(void*)) {
    mMode = 3;

    if (mAction) {
        (this->*mAction)(NULL);
    }

    mMode = 0;
    mAction = i_action;

    if (mAction) {
        (this->*mAction)(NULL);
    }

    return TRUE;
}

bool daNpcDrSol_c::chkFindPlayer() {
    dBgS_LinChk linechk;
    cXyz end(fopAcM_GetPosition_p(daPy_getPlayerActorClass())->x, eyePos.y, fopAcM_GetPosition_p(daPy_getPlayerActorClass())->z);
    linechk.Set(&eyePos, &end, this);
    
    if (dComIfG_Bgsp().LineCross(&linechk)) {
        return false;
    }

    return true;
}

bool daNpcDrSol_c::wait(void* param_0) {
    switch (mMode) {
    case 0:
        setMotion(mType, -1.0f, 0);
        mMode = 2;
        break;
    case 2:
        if (dComIfGp_event_runCheck() != 0 && eventInfo.checkCommandTalk()) {
            if (!dComIfGp_event_chkTalkXY() || dComIfGp_evmng_ChkPresentEnd()) {
                setAction(&daNpcDrSol_c::talk);
            }
        } else {
            orderEvent(field_0xd38, NULL, 0xFFFF, 40, 0xFF, 1);
        }
        break;
    case 3:
        JUT_ASSERT(760, 0);
        break;
    }

    return 1;
}

void daNpcDrSol_c::setMotion(int i_motion, f32 i_morf, int param_2) {
    s16 motion = i_motion;

    if ((param_2 != 0 || mMotion != motion) && motion >= 0 && motion < 5) {
        if (motion == 0 && mType == 1) {
            motion = 1;
        } else if (motion == 1 && mType == 0) {
            motion = 0;
        }

        mMotion = motion;
        mMotionMorfOverride = i_morf;
        mMotionPrevPhase = -1;
        mMotionPhase = 0;
        field_0xd34 = 0;
    }
}

bool daNpcDrSol_c::talk(void* param_0) {
    bool rt = false;
    
    switch (mMode) {
    case 0:
        initTalk(mMessageNo, NULL);
        mMsgTimer = 0;
        field_0xd38 = 0;
        mMode = 2;
        break;
    case 2:
        if (talkProc(NULL, 1, NULL)) {
            setAction(&daNpcDrSol_c::wait);
            rt = true;
        } else {
            int spC, sp8;
            if (ctrlMsgAnm(spC, sp8, this, TRUE)) {
                setMotion(sp8, -1.0f, 0);
            }
        }
        break;
    case 3:
        dComIfGp_event_reset();
        break;
    default:
        JUT_ASSERT(760, 0);
        break;
    }

    return rt;
}

static int daNpcDrSol_Create(void* i_this) {
    return ((daNpcDrSol_c*)i_this)->Create();
}

static int daNpcDrSol_Delete(void* i_this) {
    return ((daNpcDrSol_c*)i_this)->Delete();
}

static int daNpcDrSol_Execute(void* i_this) {
    return ((daNpcDrSol_c*)i_this)->Execute();
}

static int daNpcDrSol_Draw(void* i_this) {
    return ((daNpcDrSol_c*)i_this)->Draw();
}

static int daNpcDrSol_IsDelete(void* i_this) {
    return 1;
}

void daNpcDrSol_c::setParam() {
    if (checkHide()) {
        attention_info.flags = 0;
    } else {
        attention_info.flags = fopAc_AttnFlag_UNK_0x400000 | fopAc_AttnFlag_SPEAK_e | fopAc_AttnFlag_TALK_e;
    }

    #if PLATFORM_SHIELD
    attention_info.distances[fopAc_attn_LOCK_e] = getDistTableIdx(daNpcDrSol_Param_c::m.common.attention_distance, daNpcDrSol_Param_c::m.common.attention_angle);
    attention_info.distances[fopAc_attn_TALK_e] = attention_info.distances[fopAc_attn_LOCK_e];
    attention_info.distances[fopAc_attn_SPEAK_e] = getDistTableIdx(daNpcDrSol_Param_c::m.common.talk_distance, daNpcDrSol_Param_c::m.common.talk_angle);

    mAcchCir.SetWallR(daNpcDrSol_Param_c::m.common.width);
    mAcchCir.SetWallH(daNpcDrSol_Param_c::m.common.knee_length);
    #endif
}

BOOL daNpcDrSol_c::main() {
    if (mAction != NULL) {
        (this->*mAction)(NULL);
    }

    playMotion();
    return 0;
}

void daNpcDrSol_c::setAttnPos() {
    setMtx();

    cXyz sp14(10.0f, 15.0f, 0.0f);

    mDoMtx_stack_c::copy(mAnm_p->getModel()->getAnmMtx(4));
    mDoMtx_stack_c::multVecZero(&mHeadPos);
    mDoMtx_stack_c::multVec(&sp14, &eyePos);

    attention_info.position.set(mHeadPos.x, mHeadPos.y + daNpcDrSol_Param_c::m.common.attention_offset, mHeadPos.z);

    cXyz sp8;
    mDoMtx_stack_c::copy(mAnm_p->getModel()->getAnmMtx(2));
    mDoMtx_stack_c::multVecZero(&sp8);
    sp8.y = current.pos.y;

    mCyl.SetC(sp8);
    mCyl.SetH(daNpcDrSol_Param_c::m.common.height);
    mCyl.SetR(daNpcDrSol_Param_c::m.common.width);
    dComIfG_Ccsp()->Set(&mCyl);
}

void daNpcDrSol_c::setMotionAnm(int i_index, f32 i_morf) {
    J3DAnmTransformKey* bck_p = NULL;

    bck_p = getTrnsfrmKeyAnmP(l_arcNames[mType], l_bckGetParamList[i_index].fileIdx);
    int attr = l_bckGetParamList[i_index].attr;
    mAnmFlags &= ~(ANM_PLAY_MORF | ANM_PAUSE_MORF);

    if (bck_p != NULL && setMcaMorfAnm(bck_p, 1.0f, i_morf, attr, 0, -1)) {
        mAnmFlags |= ANM_PLAY_MORF | ANM_PAUSE_MORF;
        mMotionLoops = 0;
    }
}

BOOL daNpcDrSol_c::drawDbgInfo() {
    return false;
}

static daNpcDrSol_Param_c l_HIO;

static char* dummyString() {
    return "Shoe";
};

static actor_method_class daNpcDrSol_MethodTable = {
    (process_method_func)daNpcDrSol_Create,
    (process_method_func)daNpcDrSol_Delete,
    (process_method_func)daNpcDrSol_Execute,
    (process_method_func)daNpcDrSol_IsDelete,
    (process_method_func)daNpcDrSol_Draw,
};

extern actor_process_profile_definition g_profile_NPC_DRSOL = {
  fpcLy_CURRENT_e,         // mLayerID
  7,                       // mListID
  fpcPi_CURRENT_e,         // mListPrio
  PROC_NPC_DRSOL,          // mProcName
  &g_fpcLf_Method.base,   // sub_method
  sizeof(daNpcDrSol_c),    // mSize
  0,                       // mSizeOther
  0,                       // mParameters
  &g_fopAc_Method.base,    // sub_method
  423,                     // mPriority
  &daNpcDrSol_MethodTable, // sub_method
  0x00044187,              // mStatus
  fopAc_NPC_e,             // mActorType
  fopAc_CULLBOX_CUSTOM_e,  // cullType
};
