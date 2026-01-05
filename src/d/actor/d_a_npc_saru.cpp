/**
 * @file d_a_npc_saru.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_npc_saru.h"
#include "d/actor/d_a_npc_ks.h"
#include "d/actor/d_a_obj_so.h"
#include "d/actor/d_a_e_ym.h"

enum saru_TW_RES_File_ID {
    /* BMDR */
    /* 0x4 */ BMDR_SARU_BARA_TW = 0x4,
    /* 0x5 */ BMDR_SARU_TW,

    /* EVT */
    /* 0x8 */ EVT_SARU_TW_EVENT_LIST = 0x8,
};

enum saru_RES_File_ID {
    /* BCK */
    /* 0x3 */ BCK_SARU_HELP_E = 0x3,
};

enum RES_Name {
    /* 0x0 */ NONE,
    /* 0x1 */ NPC_KS,
    /* 0x2 */ SARU_TW,
    /* 0x3 */ SARU,
};

enum FaceMotion {
    /* 0x0 */ FACE_NONE,
};

enum Motion {
    /* 0x0 */ MOT_WAIT_A,
    /* 0x1 */ MOT_WAIT_B,
    /* 0x2 */ MOT_WAIT_C,
    /* 0x3 */ MOT_WAITSTAND,
    /* 0x4 */ MOT_SCARED,
    /* 0x5 */ MOT_SCARED_LOOK,
    /* 0x6 */ MOT_RELIEF,
    /* 0x7 */ MOT_JUMPRUN,
    /* 0x8 */ MOT_HELP_E,
    /* 0x9 */ MOT_HELP_C,
    /* 0xA */ MOT_CLAP_WAIT,
};

enum Event {
    /* 0x0 */ EVENT_NONE,
    /* 0x1 */ EVENT_NO_RESPONSE,
    /* 0x2 */ EVENT_YM_LOOK,
};

enum Material {
    /* 0x1 */ MAT_SARU_BODY_M = 0x1,
    /* 0x2 */ MAT_SARU_FACE_M,
};

static int l_bmdData[4][2] = {
    { npc_ks_class::BMDR_SARU, 1 }, { BMDR_SARU_TW, 2 }, { npc_ks_class::BMDR_SARU_BARA, 1 }, { BMDR_SARU_BARA_TW, 2 },
};

static daNpcT_evtData_c l_evtList[3] = {
    {"", 0},
    {"NO_RESPONSE", 0},
    {"YM_LOOK", 2},
};

static char* l_resNameList[4] = {
    "", "Npc_ks", "saru_TW", "saru"
};

static s8 l_loadResPtrn0[4] = {
    NPC_KS, SARU_TW, SARU, -1,
};

static s8* l_loadResPtrnList[4] = {
    l_loadResPtrn0,
    l_loadResPtrn0,
    l_loadResPtrn0,
    l_loadResPtrn0
};

static daNpcT_faceMotionAnmData_c l_faceMotionAnmData[1] = {
    {-1, J3DFrameCtrl::EMode_NONE, 0,
        npc_ks_class::BTP_SARU, J3DFrameCtrl::EMode_LOOP, NPC_KS, 1},
};

static daNpcT_motionAnmData_c l_motionAnmData[51] = {
    {npc_ks_class::BCK_SARU_WAIT_A, J3DFrameCtrl::EMode_LOOP, NPC_KS, 
        -1, J3DFrameCtrl::EMode_NONE, 0, 0, 0},
    {npc_ks_class::BCK_SARU_WAIT_B, J3DFrameCtrl::EMode_LOOP, NPC_KS, 
        -1, J3DFrameCtrl::EMode_NONE, 0, 0, 0},
    {npc_ks_class::BCK_SARU_WAIT_C, J3DFrameCtrl::EMode_LOOP, NPC_KS, 
        -1, J3DFrameCtrl::EMode_NONE, 0, 0, 0},
    {npc_ks_class::BCK_SARU_CLAP_WAIT, J3DFrameCtrl::EMode_LOOP, NPC_KS, 
        -1, J3DFrameCtrl::EMode_NONE, 0, 0, 0},
    {npc_ks_class::BCK_SARU_YOROKOBI, J3DFrameCtrl::EMode_LOOP, NPC_KS, 
        -1, J3DFrameCtrl::EMode_NONE, 0, 0, 0},
    {npc_ks_class::BCK_SARU_WAITSTAND, J3DFrameCtrl::EMode_LOOP, NPC_KS, 
        -1, J3DFrameCtrl::EMode_NONE, 0, 0, 0},
    {npc_ks_class::BCK_SARU_CALL, J3DFrameCtrl::EMode_LOOP, NPC_KS, 
        -1, J3DFrameCtrl::EMode_NONE, 0, 0, 0},
    {npc_ks_class::BCK_SARU_CALLBACK, J3DFrameCtrl::EMode_LOOP, NPC_KS, 
        -1, J3DFrameCtrl::EMode_NONE, 0, 0, 0},
    {npc_ks_class::BCK_SARU_STOPCALL, J3DFrameCtrl::EMode_NONE, NPC_KS, 
        -1, J3DFrameCtrl::EMode_NONE, 0, 0, 0},
    {npc_ks_class::BCK_SARU_SCARED, J3DFrameCtrl::EMode_LOOP, NPC_KS, 
        -1, J3DFrameCtrl::EMode_NONE, 0, 0, 0},
    {npc_ks_class::BCK_SARU_SCARED_LOOK, J3DFrameCtrl::EMode_NONE, NPC_KS, 
        -1, J3DFrameCtrl::EMode_NONE, 0, 0, 0},
    {npc_ks_class::BCK_SARU_SCARED_B, J3DFrameCtrl::EMode_LOOP, NPC_KS, 
        -1, J3DFrameCtrl::EMode_NONE, 0, 0, 0},
    {npc_ks_class::BCK_SARU_HUNGING, J3DFrameCtrl::EMode_LOOP, NPC_KS, 
        -1, J3DFrameCtrl::EMode_NONE, 0, 0, 0},
    {npc_ks_class::BCK_SARU_HANGWAIT_A, J3DFrameCtrl::EMode_NONE, NPC_KS, 
        -1, J3DFrameCtrl::EMode_NONE, 0, 0, 0},
    {npc_ks_class::BCK_SARU_HANGWAIT_B, J3DFrameCtrl::EMode_NONE, NPC_KS, 
        -1, J3DFrameCtrl::EMode_NONE, 0, 0, 0},
    {npc_ks_class::BCK_SARU_HANGWAIT_C, J3DFrameCtrl::EMode_NONE, NPC_KS, 
        -1, J3DFrameCtrl::EMode_NONE, 0, 0, 0},
    {npc_ks_class::BCK_SARU_HANGCALL, J3DFrameCtrl::EMode_LOOP, NPC_KS, 
        -1, J3DFrameCtrl::EMode_NONE, 0, 0, 0},
    {npc_ks_class::BCK_SARU_SWINGPOSE, J3DFrameCtrl::EMode_LOOP, NPC_KS, 
        -1, J3DFrameCtrl::EMode_NONE, 0, 0, 0},
    {npc_ks_class::BCK_SARU_JUMP_ATTN, J3DFrameCtrl::EMode_LOOP, NPC_KS, 
        -1, J3DFrameCtrl::EMode_NONE, 0, 0, 0},
    {npc_ks_class::BCK_SARU_JUMP_S, J3DFrameCtrl::EMode_NONE, NPC_KS, 
        -1, J3DFrameCtrl::EMode_NONE, 0, 0, 0},
    {npc_ks_class::BCK_SARU_JUMP_T, J3DFrameCtrl::EMode_NONE, NPC_KS, 
        -1, J3DFrameCtrl::EMode_NONE, 0, 0, 0},
    {npc_ks_class::BCK_SARU_JUMP_L, J3DFrameCtrl::EMode_NONE, NPC_KS, 
        -1, J3DFrameCtrl::EMode_NONE, 0, 0, 0},
    {npc_ks_class::BCK_SARU_JUMP_L, J3DFrameCtrl::EMode_LOOP, NPC_KS, 
        -1, J3DFrameCtrl::EMode_NONE, 0, 0, 0},
    {npc_ks_class::BCK_SARU_BAKUCHU, J3DFrameCtrl::EMode_LOOP, NPC_KS, 
        -1, J3DFrameCtrl::EMode_NONE, 0, 0, 0},
    {npc_ks_class::BCK_SARU_JUMPWALK, J3DFrameCtrl::EMode_LOOP, NPC_KS, 
        -1, J3DFrameCtrl::EMode_NONE, 0, 0, 0},
    {npc_ks_class::BCK_SARU_JUMPRUN, J3DFrameCtrl::EMode_LOOP, NPC_KS, 
        -1, J3DFrameCtrl::EMode_NONE, 0, 0, 0},
    {npc_ks_class::BCK_SARU_WALKCLAP, J3DFrameCtrl::EMode_LOOP, NPC_KS, 
        -1, J3DFrameCtrl::EMode_NONE, 0, 0, 0},
    {npc_ks_class::BCK_SARU_CLIMB, J3DFrameCtrl::EMode_LOOP, NPC_KS, 
        -1, J3DFrameCtrl::EMode_NONE, 0, 0, 0},
    {npc_ks_class::BCK_SARU_GODOWN, J3DFrameCtrl::EMode_LOOP, NPC_KS, 
        -1, J3DFrameCtrl::EMode_NONE, 0, 0, 0},
    {npc_ks_class::BCK_SARU_SWIM, J3DFrameCtrl::EMode_LOOP, NPC_KS, 
        -1, J3DFrameCtrl::EMode_NONE, 0, 0, 0},
    {npc_ks_class::BCK_SARU_TO_ROTATE, J3DFrameCtrl::EMode_NONE, NPC_KS, 
        -1, J3DFrameCtrl::EMode_NONE, 0, 0, 0},
    {npc_ks_class::BCK_SARU_ROTATE, J3DFrameCtrl::EMode_LOOP, NPC_KS, 
        -1, J3DFrameCtrl::EMode_NONE, 0, 0, 0},
    {npc_ks_class::BCK_SARU_BIKKURI, J3DFrameCtrl::EMode_NONE, NPC_KS, 
        -1, J3DFrameCtrl::EMode_NONE, 0, 0, 0},
    {npc_ks_class::BCK_SARU_FALL, J3DFrameCtrl::EMode_LOOP, NPC_KS, 
        -1, J3DFrameCtrl::EMode_NONE, 0, 0, 0},
    {npc_ks_class::BCK_SARU_LAND, J3DFrameCtrl::EMode_NONE, NPC_KS, 
        -1, J3DFrameCtrl::EMode_NONE, 0, 0, 0},
    {npc_ks_class::BCK_SARU_RECOVER, J3DFrameCtrl::EMode_NONE, NPC_KS, 
        -1, J3DFrameCtrl::EMode_NONE, 0, 0, 0},
    {BCK_SARU_HELP_E, J3DFrameCtrl::EMode_LOOP, SARU, 
        -1, J3DFrameCtrl::EMode_NONE, 0, 0, 0},
    {npc_ks_class::BCK_SARU_HOLDING, J3DFrameCtrl::EMode_LOOP, NPC_KS, 
        -1, J3DFrameCtrl::EMode_NONE, 0, 0, 0},
    {npc_ks_class::BCK_SARU_HELP_C, J3DFrameCtrl::EMode_LOOP, NPC_KS, 
        -1, J3DFrameCtrl::EMode_NONE, 0, 0, 0},
    {npc_ks_class::BCK_SARU_HELP_B, J3DFrameCtrl::EMode_LOOP, NPC_KS, 
        -1, J3DFrameCtrl::EMode_NONE, 0, 0, 0},
    {npc_ks_class::BCK_SARU_RELIEF, J3DFrameCtrl::EMode_NONE, NPC_KS, 
        -1, J3DFrameCtrl::EMode_NONE, 0, 0, 0},
    {npc_ks_class::BCK_SARU_JUMPWALK_K, J3DFrameCtrl::EMode_LOOP, NPC_KS, 
        -1, J3DFrameCtrl::EMode_NONE, 0, 0, 0},
    {npc_ks_class::BCK_SARU_JUMPRUN_K, J3DFrameCtrl::EMode_LOOP, NPC_KS, 
        -1, J3DFrameCtrl::EMode_NONE, 0, 0, 0},
    {npc_ks_class::BCK_SARU_WAIT_A_K, J3DFrameCtrl::EMode_LOOP, NPC_KS, 
        -1, J3DFrameCtrl::EMode_NONE, 0, 0, 0},
    {npc_ks_class::BCK_SARU_CALL_K, J3DFrameCtrl::EMode_LOOP, NPC_KS, 
        -1, J3DFrameCtrl::EMode_NONE, 0, 0, 0},
    {npc_ks_class::BCK_SARU_CALLBACK_K, J3DFrameCtrl::EMode_LOOP, NPC_KS, 
        -1, J3DFrameCtrl::EMode_NONE, 0, 0, 0},
    {npc_ks_class::BCK_SARU_FIRE_K, J3DFrameCtrl::EMode_NONE, NPC_KS, 
        -1, J3DFrameCtrl::EMode_NONE, 0, 0, 0},
    {npc_ks_class::BCK_SARU_SHAKE_K, J3DFrameCtrl::EMode_LOOP, NPC_KS, 
        -1, J3DFrameCtrl::EMode_NONE, 0, 0, 0},
    {npc_ks_class::BCK_SARU_SCARED_K, J3DFrameCtrl::EMode_LOOP, NPC_KS, 
        -1, J3DFrameCtrl::EMode_NONE, 0, 0, 0},
    {npc_ks_class::BCK_SARU_SCAREDLOOK_K, J3DFrameCtrl::EMode_NONE, NPC_KS, 
        -1, J3DFrameCtrl::EMode_NONE, 0, 0, 0},
    {npc_ks_class::BCK_SARU_SURPRISE_K, J3DFrameCtrl::EMode_NONE, NPC_KS, 
        -1, J3DFrameCtrl::EMode_NONE, 0, 0, 0},
};

static daNpcT_MotionSeqMngr_c::sequenceStepData_c l_faceMotionSequenceData[4] = {
    {0, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
};

static daNpcT_MotionSeqMngr_c::sequenceStepData_c l_motionSequenceData[44] = {
    {0, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {1, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {2, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {5, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {9, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {10, -1, 1}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {40, -1, 1}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {25, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {36, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {38, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {3, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
};

char* daNpc_Saru_c::mCutNameList[4] = {
    "",
    "FIND_MONKEY",
    "HELP_ME",
    "YM_LOOK",
};

daNpc_Saru_c::cutFunc daNpc_Saru_c::mCutList[4] = {
    NULL,
    &daNpc_Saru_c::cutFindMonkey,
    &daNpc_Saru_c::cutHelpMe,
    &daNpc_Saru_c::cutYmLook,
};

daNpc_Saru_c::~daNpc_Saru_c() {
    if (mpMorf[0] != NULL) {
        mpMorf[0]->stopZelAnime();
    }

    deleteRes(l_loadResPtrnList[mType], (const char**)l_resNameList);
}

daNpc_Saru_HIOParam const daNpc_Saru_Param_c::m = {
    140.0f,
    -3.0f,
    1.0f,
    550.0f,
    255.0f,
    140.0f,
    35.0f,
    60.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    30.0f,
    -10.0f,
    30.0f,
    -30.0f,
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
    -20.0f,
    0.0f,
    -20.0f,
    20.0f,
    50.0f,
    20.0f,
    100.0f,
    180,
    0,
};

int daNpc_Saru_c::create() {
    static int const heapSize[4] = {
        14480, 14480, 14464, 0,
    };

    daNpcT_ct(this, daNpc_Saru_c, l_faceMotionAnmData, l_motionAnmData, 
                       l_faceMotionSequenceData, 4, l_motionSequenceData, 4, l_evtList, (char**)l_resNameList);
    
    mType = getType();
    mFlowNodeNo = getFlowNodeNo();
    mTwilight = dKy_darkworld_check();

    int phase = loadRes(l_loadResPtrnList[mType], (const char**)l_resNameList);
    if (phase == cPhs_COMPLEATE_e) {
        OS_REPORT("\t(%s:%d) flowNo:%d, BitSW:%02x, PathID:%02x<%08x> ", fopAcM_getProcNameString(this), mType, mFlowNodeNo, 
                 getBitSW(), getPathID(), fopAcM_GetParam(this));

        if (isDelete()) {
            OS_REPORT("===>isDelete:TRUE\n");
            return cPhs_ERROR_e;
        }

        OS_REPORT("\n");

        if (!fopAcM_entrySolidHeap(this, createHeapCallBack, heapSize[mType])) {
            return cPhs_ERROR_e;
        }

        fopAcM_SetMtx(this, mpMorf[0]->getModel()->getBaseTRMtx());
        fopAcM_setCullSizeBox(this, -200.0f, -100.0f, -200.0f, 200.0f, 300.0f, 200.0f);
        mSound.init(&current.pos, &eyePos, 3, 1);
        #if DEBUG
        field_0xe90->entryHIO("サル"); // Monkey
        #endif
        reset();

        mAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1, &mAcchCir, 
                  fopAcM_GetSpeed_p(this), fopAcM_GetAngle_p(this), fopAcM_GetShapeAngle_p(this));
        mCcStts.Init(daNpc_Saru_Param_c::m.common.weight, 0, this);
        field_0xe4c.Set(mCcDCyl);
        field_0xe4c.SetStts(&mCcStts);
        field_0xe4c.SetTgHitCallback(tgHitCallBack);
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

int daNpc_Saru_c::CreateHeap() {
    static int const bmdTypeList[2] = {
        2, 3,
    };

    int bmdIdx = mTwilight == true ? TRUE : FALSE;
    J3DModelData* modelData = static_cast<J3DModelData*>(dComIfG_getObjectRes(l_resNameList[l_bmdData[bmdIdx][1]], l_bmdData[bmdIdx][0]));
    if (modelData == NULL) {
        return 0;
    }
    mpMorf[0] = new mDoExt_McaMorfSO(modelData, NULL,
                                  NULL, NULL, -1, 1.0f, 0, -1, &mSound, 0x80000, 0x11020084);
    if (mpMorf[0] == NULL || mpMorf[0]->getModel() == NULL) {
        return 0;
    }

    J3DModel* model = mpMorf[0]->getModel();
    for (u16 i = 0; i < modelData->getJointNum(); i++) {
        modelData->getJointNodePointer(i)->setCallBack(ctrlJointCallBack);
    }

    model->setUserArea((uintptr_t)this);

    for (int i = 0; i < 2; i++) {
        if (l_bmdData[bmdTypeList[i]][0] >= 0) {
            modelData = (J3DModelData*)dComIfG_getObjectRes(l_resNameList[l_bmdData[bmdTypeList[i]][1]], l_bmdData[bmdTypeList[i]][0]);
        } else {
            modelData = NULL;
        }

        if (modelData != NULL) {
            mpRoseModels[i] = mDoExt_J3DModel__create(modelData,  0x80000, 0x11000084);
        } else {
            mpRoseModels[i] = NULL;
        }
    }

    if (setFaceMotionAnm(0, false) && setMotionAnm(0, 0.0f, FALSE)) {
        return 1;
    }
        
    return 0;
}

int daNpc_Saru_c::Delete() {
    this->~daNpc_Saru_c();
    return 1;
}

int daNpc_Saru_c::Execute() {
    return execute();
}

int daNpc_Saru_c::Draw() {
    return draw(FALSE, FALSE, mRealShadowSize, NULL, 100.0f, FALSE, FALSE, FALSE);
}

int daNpc_Saru_c::createHeapCallBack(fopAc_ac_c* a_this) {
    daNpc_Saru_c* i_this = (daNpc_Saru_c*)a_this;
    return i_this->CreateHeap();
}

int daNpc_Saru_c::ctrlJointCallBack(J3DJoint* i_joint, int param_2) {
    if (param_2 == 0) {
        daNpc_Saru_c* _this = (daNpc_Saru_c*)j3dSys.getModel()->getUserArea();
        if (_this != NULL) {
            _this->ctrlJoint(i_joint, j3dSys.getModel());
        }
    }
    return 1;
}

void* daNpc_Saru_c::srchYm(void* i_actor, void* i_data) {
    if (mFindCount < 50 && i_actor != NULL && i_actor != i_data) {
        if (fopAcM_IsExecuting(fopAcM_GetID(i_actor)) && fopAcM_GetName(i_actor) == PROC_E_YM && ((daE_YM_c*)i_actor)->isHide() == 0) {
            mFindActorPtrs[mFindCount] = (fopAc_ac_c*)i_actor;
            mFindCount++;
        }
    }
    return NULL;
}

u8 daNpc_Saru_c::getType() {
    switch (fopAcM_GetParam(this) & 0xFF) {
        case 0:
            return TYPE_0;

        case 1:
            return TYPE_1;

        case 2:
            return TYPE_2;

        default:
            return TYPE_3;
    }
}

int daNpc_Saru_c::isDelete() {
    u8 rv;
    switch (mType) {
        case 0:
            return 0;

        case 1:
            rv = 0;
            if (!daNpcT_chkEvtBit(0x12) /* dSv_event_flag_c::F_0014 - Ordon Village - sword tutorial ends */
                || daNpcT_chkEvtBit(0x25C) /* dSv_event_flag_c::F_0701 - N/A - Talo discovers monkey in Link's house garden */) {
                rv = 1;
            }

            return rv;

        case 2:
            return 0;

        default:
            return 0;
    }
}

void daNpc_Saru_c::reset() {
    csXyz acStack_20;
    int iVar1 = (u8*)&field_0xfdc - (u8*)&field_0xfbc;
    initialize();
    for (int i = 0; i <1; i++) {
        mActrMngr[0].initialize();
    }

    if (getPathID() != 0xFF) {
        mPath.initialize();
        mPath.setPathInfo(getPathID(), fopAcM_GetRoomNo(this), 0);
    }

    memset(&field_0xfbc, 0, iVar1);

    acStack_20.setall(0);
    acStack_20.y = home.angle.y;

    if (getBitSW() != 0xFF) {
        if (dComIfGs_isSwitch(getBitSW(), fopAcM_GetRoomNo(this))) {
            field_0xfd9 = 1;
        }
    }

    setAngle(acStack_20);
}

void daNpc_Saru_c::afterJntAnm(int param_1) {
    if (param_1 == 1) {
        mDoMtx_stack_c::YrotM(mStagger.getAngleZ(1));
        mDoMtx_stack_c::ZrotM(-mStagger.getAngleX(1));
    } else if (param_1 == 4) {
        mDoMtx_stack_c::YrotM(mStagger.getAngleZ(0));
        mDoMtx_stack_c::ZrotM(mStagger.getAngleX(0));
    }
}

BOOL daNpc_Saru_c::checkChangeEvt() {
    if (!chkAction(&daNpc_Saru_c::talk)) {
        mPreItemNo = 0;
        if (dComIfGp_event_chkTalkXY()) {
            if (dComIfGp_evmng_ChkPresentEnd()) {
                mEvtNo = EVENT_NO_RESPONSE;
                evtChange();
            }

            return TRUE;
        }
    }

    return FALSE;
}

void daNpc_Saru_c::setParam() {
    selectAction();
    srchActors();
    u32 uVar1 = (fopAc_AttnFlag_SPEAK_e | fopAc_AttnFlag_TALK_e);

    s16 sVar1 = daNpc_Saru_Param_c::m.common.talk_distance;
    s16 sVar2 = daNpc_Saru_Param_c::m.common.talk_angle;
    s16 sVar3 = daNpc_Saru_Param_c::m.common.attention_distance;
    s16 sVar4 = daNpc_Saru_Param_c::m.common.attention_angle;

    attention_info.distances[0] = daNpcT_getDistTableIdx(sVar3, sVar4);
    attention_info.distances[1] = attention_info.distances[0];
    attention_info.distances[3] = daNpcT_getDistTableIdx(sVar1, sVar2);

    if (mType == 2) {
        uVar1 = 0;
    }

    attention_info.flags = uVar1;

    scale.set(daNpc_Saru_Param_c::m.common.scale, daNpc_Saru_Param_c::m.common.scale, daNpc_Saru_Param_c::m.common.scale);

    if (mType != 0) {
        scale.setall(0.8f);
    }

    mCcStts.SetWeight(daNpc_Saru_Param_c::m.common.weight);
    mCylH = daNpc_Saru_Param_c::m.common.height;
    mWallR = daNpc_Saru_Param_c::m.common.width;
    mAttnFovY = daNpc_Saru_Param_c::m.common.fov;
    mAcchCir.SetWallR(mWallR);
    mAcchCir.SetWallH(daNpc_Saru_Param_c::m.common.knee_length);
    mRealShadowSize = daNpc_Saru_Param_c::m.common.real_shadow_size;
    mExpressionMorfFrame = daNpc_Saru_Param_c::m.common.expression_morf_frame;
    mMorfFrames = daNpc_Saru_Param_c::m.common.morf_frame;
    gravity = daNpc_Saru_Param_c::m.common.gravity;
}

void daNpc_Saru_c::setAfterTalkMotion() {
    #if DEBUG
    mFaceMotionSeqMngr.getNo();
    #endif
    mFaceMotionSeqMngr.setNo(FACE_NONE, -1.0f, 0, 0);
}

void daNpc_Saru_c::srchActors() {
    switch (mType) {
        case 2:
            if (mActrMngr[0].getActorP() == NULL) {
                mActrMngr[0].entry(getNearestActorP(28));
            }

        case 1:
        default:
            return;
    }
}

BOOL daNpc_Saru_c::evtTalk() {
    if (chkAction(&daNpc_Saru_c::talk)) {
        (this->*field_0xfc8)(NULL);
    } else {
        setAction(&daNpc_Saru_c::talk);
    }

    return TRUE;
}

BOOL daNpc_Saru_c::evtCutProc() {
    int staffId = dComIfGp_getEventManager().getMyStaffId("Saru", this, -1);
    if (staffId != -1) {
        mStaffId = staffId;
        int actIdx =
            dComIfGp_getEventManager().getMyActIdx(mStaffId, (char**)mCutNameList, 4, 0, 0);
        if ((this->*(mCutList[actIdx]))(mStaffId) != 0) {
            dComIfGp_getEventManager().cutEnd(mStaffId);
        }
        return true;
    } 
    return false;
}

void daNpc_Saru_c::action() {
    fopAc_ac_c* actor_p = NULL;
    if (mTwilight == false) {
        actor_p = hitChk(&field_0xe4c, -1);
    }

    if (actor_p != NULL) {
        mStagger.setParam(this, actor_p, mCurAngle.y);
        setDamage(0, 0, 4);
        mStagger.setPower(0.0f);
        mDamageTimerStart = 0;
        mJntAnm.lookNone(1);
    }

    if (mStagger.checkRebirth() != 0) {
        mStagger.initialize();
        mMode = 1;
    }

    if (field_0xfbc) {
        if (field_0xfc8 == field_0xfbc) {
            (this->*field_0xfc8)(NULL);
        } else {
            setAction(field_0xfbc);
        }
    }
}

void daNpc_Saru_c::beforeMove() {
    fopAcM_OffStatus(this, 0x8000000);

    if (checkHide()) {
        fopAcM_OnStatus(this, 0x8000000);
    }

    if ((checkHide() || mNoDraw != 0) && mSpeakEvent == false) {
        attention_info.flags = 0;
    }
}

void daNpc_Saru_c::setAttnPos() {
    cXyz sp3c(30.0f, 0.0f, 0.0f);

    if (field_0xfd8 != 0) {
        daPy_getPlayerActorClass()->onWolfEyeKeep();
    }

    mStagger.calc(FALSE);
    f32 fVar1 = cM_s2rad(mCurAngle.y - field_0xd7e.y);
    mJntAnm.setParam(this, mpMorf[0]->getModel(), &sp3c, getBackboneJointNo(), getNeckJointNo(),
        getHeadJointNo(), daNpc_Saru_Param_c::m.common.body_angleX_min, daNpc_Saru_Param_c::m.common.body_angleX_max,
        daNpc_Saru_Param_c::m.common.body_angleY_min, daNpc_Saru_Param_c::m.common.body_angleY_max,
        daNpc_Saru_Param_c::m.common.head_angleX_min, daNpc_Saru_Param_c::m.common.head_angleX_max,
        daNpc_Saru_Param_c::m.common.head_angleY_min, daNpc_Saru_Param_c::m.common.head_angleY_max,
        daNpc_Saru_Param_c::m.common.neck_rotation_ratio, fVar1, NULL);
    mJntAnm.calcJntRad(0.2f, 1.0f, fVar1);

    setMtx();
    mDoMtx_stack_c::copy(mpMorf[0]->getModel()->getAnmMtx(getHeadJointNo()));
    mDoMtx_stack_c::multVec(&sp3c, &eyePos);
    mDoMtx_stack_c::multVecZero(&sp3c);
    field_0xe0c.set(-30.0f, 0.0f, 10.0f);
    mDoMtx_stack_c::multVec(&field_0xe0c, &field_0xe0c);
    field_0xe0c -= sp3c;
    mJntAnm.setEyeAngleX(eyePos, 1.0f, 0);
    mJntAnm.setEyeAngleY(eyePos, mCurAngle.y, 0, 1.0f, 0);

    sp3c.set(0.0f, 0.0f, 60.0f);
    sp3c.y = daNpc_Saru_Param_c::m.common.attention_offset;
    mDoMtx_stack_c::YrotS(mCurAngle.y);
    mDoMtx_stack_c::multVec(&sp3c, &sp3c);
    attention_info.position = sp3c + current.pos;
    setSe();
}

void daNpc_Saru_c::setCollision() {
    cXyz sp48;
    if (mHide == false) {
        int i_type = -0x27040201;
        int i_prm = 31;

        if (dComIfGp_event_runCheck()) {
            i_type = 0;
            i_prm = 0;
        } else if (mTwilight != false) {
            i_type = 0;
            i_prm = 0;
        } else if (mStagger.checkStagger()) {
            i_type = 0;
            i_prm = 0;
        }

        field_0xe4c.SetCoSPrm(121);
        field_0xe4c.SetTgType(i_type);
        field_0xe4c.SetTgSPrm(i_prm);
        field_0xe4c.OnTgNoHitMark();

        sp48.set(0.0f, 0.0f, 20.0f);
        f32 i_cylH = mCylH;
        f32 i_wallR = mWallR;
        mDoMtx_stack_c::YrotS(mCurAngle.y);
        mDoMtx_stack_c::multVec(&sp48, &sp48);
        sp48 += current.pos;
        field_0xe4c.SetH(i_cylH);
        field_0xe4c.SetR(i_wallR);
        field_0xe4c.SetC(sp48);
        dComIfG_Ccsp()->Set(&field_0xe4c);
    }

    field_0xe4c.ClrCoHit();
    field_0xe4c.ClrTgHit();
}

int daNpc_Saru_c::drawDbgInfo() {
    return 0;
}

void daNpc_Saru_c::drawOtherMdl() {
    static int const jointNo[2] = {
        4, 4,
    };

    Mtx mtx;
    J3DModel* model = mpMorf[0]->getModel();
    for (int i = 0; i < 2; i++) {
        if (mpRoseModels[i] != NULL && ((i == 0 && mTwilight == false) || (i == 1 && mTwilight != false))) {
            g_env_light.setLightTevColorType_MAJI(mpRoseModels[i], &tevStr);
            mDoMtx_stack_c::copy(model->getAnmMtx(jointNo[i]));
            cMtx_copy(mDoMtx_stack_c::get(), mtx);
            mpRoseModels[i]->setBaseTRMtx(mtx);
            mDoExt_modelUpdateDL(mpRoseModels[i]);
            dComIfGd_addRealShadow(mShadowKey, mpRoseModels[i]);
        }
    }
}

int daNpc_Saru_c::selectAction() {
    field_0xfbc = NULL;
    field_0xfbc = &daNpc_Saru_c::wait;
    return 1;
}

BOOL daNpc_Saru_c::chkAction(int (daNpc_Saru_c::*action)(void*)) {
    return field_0xfc8 == action;
}

int daNpc_Saru_c::setAction(int (daNpc_Saru_c::*action)(void*)) {
    mMode = 3;
    if (field_0xfc8) {
        (this->*field_0xfc8)(NULL);
    }

    mMode = 0;
    field_0xfc8 = action;
    if (field_0xfc8) {
        (this->*field_0xfc8)(NULL);
    }

    return 1;
}

void daNpc_Saru_c::setSe() {
    if (cM3d_IsZero(mpMorf[0]->getPlaySpeed()) <= 0) {
        if ((J3DAnmTransform*)dComIfG_getObjectRes(l_resNameList[l_motionAnmData[25].mBckArcIdx], l_motionAnmData[25].mBckFileIdx) == mpMorf[0]->getAnm()) {
            if (mpMorf[0]->checkFrame(0.0f)) {
                mSound.startCreatureVoice(Z2SE_KOSARU_V_WALK, -1);
            }

            if (mpMorf[0]->checkFrame(12.0f)) {
                mSound.startCreatureSound(Z2SE_KOSARU_FOOTNOTE, 0, -1);
            }
        }

        if ((J3DAnmTransform*)dComIfG_getObjectRes(l_resNameList[l_motionAnmData[36].mBckArcIdx], l_motionAnmData[36].mBckFileIdx) == mpMorf[0]->getAnm()) {
            if (((mpMorf[0]->checkFrame(0.0f) || mpMorf[0]->checkFrame(19.0f)) || mpMorf[0]->checkFrame(42.0f)) || mpMorf[0]->checkFrame(75.0f)) {
                mSound.startCreatureVoice(Z2SE_KOSARU_V_PRISONED, -1);
            }

            if (mpMorf[0]->checkFrame(5.0f) || mpMorf[0]->checkFrame(21.0f)) {
                mSound.startCreatureSound(Z2SE_OBJ_MONKEYJAIL_CREAK, 0, -1);
            }

            if (mpMorf[0]->checkFrame(53.0f) || mpMorf[0]->checkFrame(64.0f)) {
                mSound.startCreatureSound(Z2SE_KOSARU_JUMP_END, 0, -1);
            }
        }

        if ((J3DAnmTransform*)dComIfG_getObjectRes(l_resNameList[l_motionAnmData[11].mBckArcIdx], l_motionAnmData[11].mBckFileIdx) == mpMorf[0]->getAnm() && mpMorf[0]->checkFrame(0.0f)) {
            mSound.startCreatureVoice(Z2SE_KOSARU_V_SCARED, -1);
        }

        if ((J3DAnmTransform*)dComIfG_getObjectRes(l_resNameList[l_motionAnmData[38].mBckArcIdx], l_motionAnmData[38].mBckFileIdx) == mpMorf[0]->getAnm()) {
            if (mpMorf[0]->checkFrame(4.0f) || mpMorf[0]->checkFrame(21.0f)) {
                mSound.startCreatureVoice(Z2SE_KOSARU_V_POINT, -1);
            }

            if ((mpMorf[0]->checkFrame(0.0f) || mpMorf[0]->checkFrame(11.0f)) || mpMorf[0]->checkFrame(23.0f)) {
                mSound.startCreatureSound(Z2SE_KOSARU_JUMP_END, 0, -1);
            }
        }

        if ((J3DAnmTransform*)dComIfG_getObjectRes(l_resNameList[l_motionAnmData[3].mBckArcIdx], l_motionAnmData[3].mBckFileIdx) == mpMorf[0]->getAnm()) {
            if (mpMorf[0]->checkFrame(0.0f)) {
                mSound.startCreatureVoice(Z2SE_KOSARU_V_DELIGHT, -1);
            }

            if (mpMorf[0]->checkFrame(9.0f)) {
                mSound.startCreatureSound(Z2SE_KOSARU_HAND_CLAP, 0, -1);
            }

            if (mpMorf[0]->checkFrame(15.0f)) {
                mSound.startCreatureSound(Z2SE_KOSARU_JUMP_END, 0, -1);
            }
        }
    }
}

int daNpc_Saru_c::cutFindMonkey(int param_1) {
    cXyz sp2c;
    int rv = 0;
    int iVar1 = -1;
    int iVar2 = 0;

    int* piVar1 = dComIfGp_evmng_getMyIntegerP(param_1, "prm");
    if (piVar1 != NULL){
        iVar1 = *piVar1;
    }

    piVar1 = dComIfGp_evmng_getMyIntegerP(param_1, "timer");
    if (piVar1 != NULL) {
        iVar2 = *piVar1;
    }

    if (dComIfGp_getEventManager().getIsAddvance(param_1) != 0) {
        switch (iVar1) {
            case 0:
                break;

            case 1:
                mMotionSeqMngr.setNo(MOT_JUMPRUN, -1.0f, 0, 0);
                mEventTimer = iVar2;
                break;

            case 2:
                speedF = 0.0f;
                speed.setall(0.0f);
        }
    }

    switch (iVar1) {
        case 0:
            rv = 1;
            break;

        case 1:
            JUT_ASSERT(1859, NULL != mPath.getPathInfo());
            if (cLib_calcTimer(&mEventTimer) == 0) {
                rv = 1;
            }
            sp2c = mPath.getPntPos(1);
            if (2500.0f < current.pos.abs2(sp2c)) {
                cLib_addCalcAngleS2(&current.angle.y, cLib_targetAngleY(&current.pos, &sp2c), 4, 0x800);
                shape_angle.y = current.angle.y;
                mCurAngle.y = shape_angle.y;
                cLib_chaseF(&speedF, 18.0f, 0.5f);
            } else {
                rv = 1;
            }
            break;

        case 2:
            rv = 1;
    }

    return rv;
}

int daNpc_Saru_c::cutHelpMe(int param_1) {
    int rv = 0;
    int iVar1 = -1;

    int* piVar1 = dComIfGp_evmng_getMyIntegerP(param_1, "prm");
    if (piVar1 != NULL) {
        iVar1 = *piVar1;
    }

    if (dComIfGp_getEventManager().getIsAddvance(param_1) != 0) {
        switch (iVar1) {
            case 0:
                mFaceMotionSeqMngr.setNo(FACE_NONE, 0.0f, 0, 0);
                mMotionSeqMngr.setNo(MOT_HELP_C, 0.0f, 0, 0);
                break;

            case 1:
                mFaceMotionSeqMngr.setNo(FACE_NONE, -1.0f, 0, 0);
                mMotionSeqMngr.setNo(MOT_CLAP_WAIT, -1.0f, 0, 0);
                mPlayerAngle = home.angle.y + 0x8000;
                break;
        }
    }

    switch (iVar1) {
        case 0:
            mJntAnm.lookNone(0);
            rv = 1;
            break;

        case 1:
            mJntAnm.lookNone(0);
            if (mPlayerAngle == mCurAngle.y) {
                rv = 1;
            } else {
                step(mPlayerAngle, -1, -1, 20, 0);
            }
    }

    return rv;
}

int daNpc_Saru_c::cutYmLook(int param_1) {
    char acStack_88[104];
    int rv = 0;
    int iVar1 = -1;
    int i_eventTimer = 0;

    int* piVar1 = dComIfGp_evmng_getMyIntegerP(param_1, "prm");
    if (piVar1 != NULL) {
        iVar1 = *piVar1;
    }

    int* piVar2 = dComIfGp_evmng_getMyIntegerP(param_1, "timer");
    if (piVar2 != NULL) {
        i_eventTimer = *piVar2;
    }

    if (dComIfGp_getEventManager().getIsAddvance(param_1) != 0) {
        switch (iVar1) {
            case 0:
                strcpy(acStack_88, l_evtList[2].eventName);
                strcat(acStack_88, "@");
                dComIfGp_getEvent()->setSkipZev(this, acStack_88);
                dComIfGp_getEvent()->onSkipFade();
                field_0xfd9 = 1;

            case 1:
                mEventTimer = i_eventTimer;
                break;

            case 2:
                mMotionSeqMngr.setNo(MOT_WAIT_C, -1.0f, 0, 0);
                break;

            case 3:
                initTalk(mFlowNodeNo, NULL);
                break;

            case 4:
                mMotionSeqMngr.setNo(MOT_RELIEF, -1.0f, 0, 0);
                break;

            case 5:
                break;

            case 99:
                daNpcT_onEvtBit(0x16C); // dSv_event_flag_c::F_0364 - Faron Woods - Listened to monkey girl's laments (Twilight)
                mFlow.remove();
        }
    }

    switch (iVar1) {
        case 0:
            mFindCount = 0;
            fopAcM_Search(srchYm, this);
            if (mFindCount == 0 && cLib_calcTimer(&mEventTimer) == 0) {
                rv = 1;
            }
            action();
            break;

        case 1:
            action();
            if (cLib_calcTimer(&mEventTimer) == 0) {
                rv = 1;
            }
            break;

        case 2:
            if (mMotionSeqMngr.checkEndSequence() != 0) {
                rv = 1;
            }
            break;

        case 3:
            if (talkProc(NULL, FALSE, NULL, FALSE) && mFlow.checkEndFlow()) {
                rv = 1;
            }
            break;

        case 4:
            if (mMotionSeqMngr.checkEndSequence() != 0) {
                rv = 1;
            }
            break;

        case 5:
            rv = 1;
            break;

        case 99:
            rv = 1;
            break;
    }

    return rv;
}

int daNpc_Saru_c::wait(void* param_1) {
    obj_so_class* cage_p;
    switch (mMode) {
    case 0:
    case 1:
        if (mStagger.checkStagger() == 0) {
            switch (mType) {
                case 0:
                    if (field_0xfd9 != 0) {
                        mFaceMotionSeqMngr.setNo(FACE_NONE, -1.0f, 0, 0);
                        mMotionSeqMngr.setNo(MOT_WAIT_A, -1.0f, 0, 0);
                    } else {
                        mFaceMotionSeqMngr.setNo(FACE_NONE, -1.0f, 0, 0);
                        mMotionSeqMngr.setNo(MOT_SCARED, -1.0f, 0, 0);
                    }
                    break;

                case 2:
                    cage_p = (obj_so_class*)mActrMngr[0].getActorP();
                    if (cage_p != NULL) {
                        mFaceMotionSeqMngr.setNo(FACE_NONE, -1.0f, 0, 0);
                        mMotionSeqMngr.setNo(MOT_HELP_E, -1.0f, 0, 0);
                        if (cage_p->partBreak() == 1) {
                            mFaceMotionSeqMngr.setNo(FACE_NONE, -1.0f, 0 ,0);
                            mMotionSeqMngr.setNo(MOT_HELP_C, -1.0f, 0, 0);
                        }
                    } else {
                        mFaceMotionSeqMngr.setNo(FACE_NONE, -1.0f, 0, 0);
                        mMotionSeqMngr.setNo(MOT_WAIT_A, -1.0f, 0, 0);
                    }
                    break;

                default:
                    mFaceMotionSeqMngr.setNo(FACE_NONE, -1.0f, 0, 0);
                    mMotionSeqMngr.setNo(MOT_WAIT_A, -1.0f, 0, 0);
            }

            mMode = 2;
        }
        // fallthrough
    case 2:
        if (mType == 1 && daNpcT_chkEvtBit(0x25C) /* dSv_event_flag_c::F_0701 - N/A - Talo discovers monkey in Link's house garden */) {
            fopAcM_delete(this);
            return 1;
        } else {
            if (mStagger.checkStagger() == 0) {
                if (mType == 2 && mMotionSeqMngr.getNo() == MOT_HELP_E) {
                    cage_p = (obj_so_class*)mActrMngr[0].getActorP();
                    if (cage_p != NULL && cage_p->partBreak() == 1) {
                        mFaceMotionSeqMngr.setNo(FACE_NONE, -1.0f, 0, 0);
                        mMotionSeqMngr.setNo(MOT_HELP_C, -1.0f, 0, 0);
                    }
                }

                if (mPlayerActorMngr.getActorP() != NULL && mTwilight == false) {
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
                    } else {
                        if (mType == 0) {
                            if (mMotionSeqMngr.getNo() == MOT_SCARED_LOOK) {
                                if (mMotionSeqMngr.checkEndSequence() != 0) {
                                    mMode = 1;
                                }
                            } else if (field_0xfd4 != 0 && cLib_calcTimer(&field_0xfd4) == 0 && mType == 0 && field_0xfd9 == 0) {
                                mFaceMotionSeqMngr.setNo(FACE_NONE, -1.0f, 0, 0);
                                mMotionSeqMngr.setNo(MOT_SCARED_LOOK, -1.0f, 0, 0);
                            }
                        }

                        if (!mTwilight) {
                            srchPlayerActor();
                        }
                    }
                }

                switch (mJntAnm.getMode()) {
                case 0:
                    break;
                }
            }

            if (mType == 0) {
                if (field_0xfd9 == 0) {
                    mFindCount = 0;
                    fopAcM_Search(srchYm, this);
                    int iVar1 = 0;
                    for (int i = 0; i < mFindCount; i++) {
                        if (mFindActorPtrs[i]->health > 0) {
                            iVar1++;
                        }
                    }

                    if (mFindCount && iVar1 == 0) {
                        mEvtNo = EVENT_YM_LOOK;
                        field_0xfd8 = 1;
                    }
                } else {
                    field_0xfd8 = 0;
                }
            }
        }
        break;

    case 3:
        break;
    }

    return 1;
}

int daNpc_Saru_c::talk(void* param_1) {
    switch (mMode) {
        case 0:
        case 1:
            if (mStagger.checkStagger() == 0) {
                initTalk(mFlowNodeNo, NULL);
                mMode = 2;
            }

        case 2:
            if (mStagger.checkStagger() == 0) {
                if (mTwilight != false || mPlayerAngle == mCurAngle.y) {
                    if (talkProc(NULL, FALSE, NULL, FALSE) && mFlow.checkEndFlow()) {
                        mPlayerActorMngr.entry(daPy_getPlayerActorClass());
                        dComIfGp_event_reset();
                        mMode = 3;
                    }

                    mJntAnm.lookPlayer(0);
                    if (mTwilight != false) {
                        mJntAnm.lookNone(0);
                    }
                } else {
                    mJntAnm.lookPlayer(0);
                    step(fopAcM_searchPlayerAngleY(this), -1, -1, 15, 0);
                }
                break;

            case 3:
                break;
        }
    }

    return 0;
}

static int daNpc_Saru_Create(void* param_1) {
    daNpc_Saru_c* i_this = (daNpc_Saru_c*)param_1;
    return i_this->create();
}

static int daNpc_Saru_Delete(void* param_1) {
    daNpc_Saru_c* i_this = (daNpc_Saru_c*)param_1;
    return i_this->Delete();
}

static int daNpc_Saru_Execute(void* param_1) {
    daNpc_Saru_c* i_this = (daNpc_Saru_c*)param_1;
    return i_this->Execute();
}

static int daNpc_Saru_Draw(void* param_1) {
    daNpc_Saru_c* i_this = (daNpc_Saru_c*)param_1;
    return i_this->Draw();
}

static int daNpc_Saru_IsDelete(void* param_1) {
    return 1;
}

static daNpc_Saru_Param_c l_HIO;

static actor_method_class daNpc_Saru_MethodTable = {
    (process_method_func)daNpc_Saru_Create,
    (process_method_func)daNpc_Saru_Delete,
    (process_method_func)daNpc_Saru_Execute,
    (process_method_func)daNpc_Saru_IsDelete,
    (process_method_func)daNpc_Saru_Draw,
};

actor_process_profile_definition g_profile_NPC_SARU = {
  fpcLy_CURRENT_e,         // mLayerID
  7,                       // mListID
  fpcPi_CURRENT_e,         // mListPrio
  PROC_NPC_SARU,           // mProcName
  &g_fpcLf_Method.base,   // sub_method
  sizeof(daNpc_Saru_c),    // mSize
  0,                       // mSizeOther
  0,                       // mParameters
  &g_fopAc_Method.base,    // sub_method
  357,                     // mPriority
  &daNpc_Saru_MethodTable, // sub_method
  0x00040107,              // mStatus
  fopAc_NPC_e,             // mActorType
  fopAc_CULLBOX_CUSTOM_e,  // cullType
};
