/**
 * @file d_a_npc_kkri.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_npc_kkri.h"
#include "d/actor/d_a_e_ym.h"

static int l_bmdData[2][2] = {
    {35, 1},
    {18, 2},
};

static daNpcT_evtData_c l_evtList[5] = {
    {"", 0},
    {"DEFAULT_GETITEM", 0},
    {"NO_RESPONSE", 0},
    {"CONVERSATION_ABOUT_SOUP", 1},
    {"YM_LOOK", 2},
};

static char* l_resNameList[3] = {
    "",
    "Kkri",
    "Kkri_TW",
};

static s8 l_loadResPtrn0[] = {1, -1};

static s8 l_loadResPtrn1[] = {1, 2, -1};

static s8* l_loadResPtrnList[3] = {
    l_loadResPtrn0,
    l_loadResPtrn1,
    l_loadResPtrn1,
};

static daNpcT_faceMotionAnmData_c l_faceMotionAnmData[15] = {
    {-1, 0, 0, 44, 2, 1, 1},
    {13, 0, 1, 44, 2, 1, 1},
    {11, 0, 1, 50, 0, 1, 0},
    {8, 0, 1, 46, 0, 1, 0},
    {10, 2, 1, 49, 2, 1, 0},
    {7, 0, 1, 45, 0, 1, 0},
    {9, 2, 1, 47, 2, 1, 0},
    {12, 0, 1, 51, 0, 1, 1},
    {16, 0, 1, 52, 0, 1, 1},
    {17, 2, 1, 53, 2, 1, 1},
    {7, 2, 2, 44, 2, 1, 1},
    {9, 2, 2, 27, 2, 2, 0},
    {15, 2, 1, 44, 2, 1, 1},
    {14, 0, 1, 44, 2, 1, 1},
    {8, 0, 2, 26, 0, 2, 0},
};

static daNpcT_motionAnmData_c l_motionAnmData[21] = {
    {30, 2, 1, 38, 0, 1, 1, 0},
    {31, 2, 1, 38, 0, 1, 1, 0},
    {28, 2, 1, 38, 0, 1, 1, 0},
    {27, 0, 1, 38, 0, 1, 1, 0},
    {29, 0, 1, 38, 0, 1, 1, 0},
    {23, 0, 1, 41, 0, 1, 0, 0},
    {32, 0, 1, 38, 0, 1, 1, 0},
    {18, 0, 1, 38, 0, 1, 1, 0},
    {19, 2, 1, 38, 0, 1, 1, 0},
    {21, 0, 1, 38, 0, 1, 1, 0},
    {24, 0, 1, 38, 0, 1, 1, 0},
    {20, 0, 1, 39, 0, 1, 0, 0},
    {22, 2, 1, 40, 2, 1, 0, 0},
    {25, 0, 1, 38, 0, 1, 1, 0},
    {26, 2, 1, 38, 0, 1, 1, 0},
    {10, 2, 2, 38, 0, 1, 1, 0},
    {12, 2, 2, 22, 2, 2, 0, 0},
    {15, 2, 2, 38, 0, 1, 1, 0},
    {14, 0, 2, 23, 0, 2, 0, 0},
    {11, 2, 2, 21, 2, 2, 0, 0},
    {13, 0, 2, 38, 0, 1, 1, 0},
};

static daNpcT_MotionSeqMngr_c::sequenceStepData_c l_faceMotionSequenceData[60] = {
    {1, -1, 1},
    {-1, 0, 0},
    {-1, 0, 0},
    {-1, 0, 0},
    {4, -1, 0},
    {-1, 0, 0},
    {-1, 0, 0},
    {-1, 0, 0},
    {10, -1, 0},
    {-1, 0, 0},
    {-1, 0, 0},
    {-1, 0, 0},
    {7, -1, 1},
    {-1, 0, 0},
    {-1, 0, 0},
    {-1, 0, 0},
    {8, -1, 1},
    {-1, 0, 0},
    {-1, 0, 0},
    {-1, 0, 0},
    {3, -1, 1},
    {9, -1, 0},
    {-1, 0, 0},
    {-1, 0, 0},
    {5, -1, 1},
    {6, -1, 0},
    {-1, 0, 0},
    {-1, 0, 0},
    {11, -1, 0},
    {-1, 0, 0},
    {-1, 0, 0},
    {-1, 0, 0},
    {2, -1, 1},
    {4, -1, 0},
    {-1, 0, 0},
    {-1, 0, 0},
    {13, -1, 1},
    {12, -1, 0},
    {-1, 0, 0},
    {-1, 0, 0},
    {14, -1, 1},
    {12, -1, 0},
    {-1, 0, 0},
    {-1, 0, 0},
    {12, -1, 0},
    {-1, 0, 0},
    {-1, 0, 0},
    {-1, 0, 0},
    {6, -1, 0},
    {-1, 0, 0},
    {-1, 0, 0},
    {-1, 0, 0},
    {9, -1, 0},
    {-1, 0, 0},
    {-1, 0, 0},
    {-1, 0, 0},
    {0, -1, 0},
    {-1, 0, 0},
    {-1, 0, 0},
    {-1, 0, 0},
};

static daNpcT_MotionSeqMngr_c::sequenceStepData_c l_motionSequenceData[76] = {
    {1, -1, 0},
    {-1, 0, 0},
    {-1, 0, 0},
    {-1, 0, 0},
    {0, -1, 0},
    {-1, 0, 0},
    {-1, 0, 0},
    {-1, 0, 0},
    {2, -1, 0},
    {-1, 0, 0},
    {-1, 0, 0},
    {-1, 0, 0},
    {15, -1, 0},
    {-1, 0, 0},
    {-1, 0, 0},
    {-1, 0, 0},
    {4, -1, 1},
    {1, -1, 0},
    {-1, 0, 0},
    {-1, 0, 0},
    {5, -1, 1},
    {1, -1, 0},
    {-1, 0, 0},
    {-1, 0, 0},
    {6, -1, 1},
    {1, -1, 0},
    {-1, 0, 0},
    {-1, 0, 0},
    {7, -1, 1},
    {8, -1, 0},
    {-1, 0, 0},
    {-1, 0, 0},
    {9, -1, 1},
    {1, -1, 0},
    {-1, 0, 0},
    {-1, 0, 0},
    {10, -1, 1},
    {1, -1, 0},
    {-1, 0, 0},
    {-1, 0, 0},
    {11, -1, 1},
    {12, -1, 0},
    {-1, 0, 0},
    {-1, 0, 0},
    {16, -1, 0},
    {-1, 0, 0},
    {-1, 0, 0},
    {-1, 0, 0},
    {17, -1, 0},
    {-1, 0, 0},
    {-1, 0, 0},
    {-1, 0, 0},
    {18, -1, 1},
    {17, -1, 0},
    {-1, 0, 0},
    {-1, 0, 0},
    {20, 0, 1},
    {19, 0, 0},
    {-1, 0, 0},
    {-1, 0, 0},
    {19, -1, 0},
    {-1, 0, 0},
    {-1, 0, 0},
    {-1, 0, 0},
    {8, -1, 0},
    {-1, 0, 0},
    {-1, 0, 0},
    {-1, 0, 0},
    {13, -1, 1},
    {14, 0, 0},
    {-1, 0, 0},
    {-1, 0, 0},
    {3, -1, 1},
    {-1, 0, 0},
    {-1, 0, 0},
    {-1, 0, 0},
};

char* daNpc_Kkri_c::mCutNameList[3] = {
    "",
    "CONVERSATION_ABOUT_SOUP",
    "YM_LOOK",
};

int (daNpc_Kkri_c::*daNpc_Kkri_c::mCutList[])(int) = {
    NULL,
    &daNpc_Kkri_c::cutConversationAboutSoup,
    &daNpc_Kkri_c::cutYmLook,
};

daNpc_Kkri_c::~daNpc_Kkri_c() {
    OS_REPORT("|%06d:%x|daNpc_Kkri_c -> デストラクト\n", g_Counter.mCounter0, this);

    if (mpMorf[0] != NULL) {
        mpMorf[0]->stopZelAnime();
    }

    deleteRes(l_loadResPtrnList[mType], (const char**)l_resNameList);
}

const daNpc_Kkri_HIOParam daNpc_Kkri_Param_c::m = {
    180.0f,
    -3.0f,
    1.0f,
    400.0f,
    255.0f,
    170.0f,
    35.0f,
    30.0f,
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
    0,
    0,
    4.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
};

int daNpc_Kkri_c::create() {
    daNpcT_ct(this, daNpc_Kkri_c, l_faceMotionAnmData, l_motionAnmData,
                       l_faceMotionSequenceData, 4, l_motionSequenceData, 4,
                       l_evtList, l_resNameList);

    mType = getType();
    mFlowNodeNo = getFlowNodeNo();
    mTwilight = dKy_darkworld_check();

    int phase_state = loadRes(l_loadResPtrnList[mType], (const char**)l_resNameList);
    if (phase_state == cPhs_COMPLEATE_e) {
        OS_REPORT("\t(%s:%d) flowNo:%d, PathID:%02x, BitSW:%02x, BitSW2:%02x<%08x> ",
                  fopAcM_getProcNameString(this), mType, mFlowNodeNo, getPathID(), getBitSW(), getBitSW2(), fopAcM_GetParam(this), fopAcM_GetRoomNo(this));

        if (isDelete()) {
            OS_REPORT("===>isDelete:TRUE\n");
            return cPhs_ERROR_e;
        }

        OS_REPORT("\n");

        static const u32 heapSize[3] = {
            0x42D0,
            0x4210,
            0x0,
        };

        if (!fopAcM_entrySolidHeap(this, createHeapCallBack, heapSize[mType])) {
            return cPhs_ERROR_e;
        }

        J3DModelData* modelData = mpMorf[0]->getModel()->getModelData();
        fopAcM_SetMtx(this, mpMorf[0]->getModel()->getBaseTRMtx());
        fopAcM_setCullSizeBox(this, -200.0f, -100.0f, -200.0f, 200.0f, 300.0f, 200.0f);
        
        mSound.init(&current.pos, &eyePos, 3, 1);

        reset();

        mAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1, &mAcchCir, fopAcM_GetSpeed_p(this), fopAcM_GetAngle_p(this), fopAcM_GetShapeAngle_p(this));
    
        mCcStts.Init(daNpc_Kkri_Param_c::m.common.weight, 0, this);

        mCcCyl.Set(mCcDCyl);
        mCcCyl.SetStts(&mCcStts);
        mCcCyl.SetTgHitCallback(tgHitCallBack);

        if (mType == 0) {
            mAcch.SetGrndNone();
            mAcch.SetWallNone();
        }

        mAcch.CrrPos(dComIfG_Bgsp());
        mGndChk = mAcch.m_gnd;
        mGroundH = mAcch.GetGroundH();

        if (mGroundH != -G_CM3D_F_INF) {
            setEnvTevColor();
            setRoomNo();
        }

        mCreating = TRUE;
        Execute();
        mCreating = FALSE;
    }

    return phase_state;
}

static f32 dummyFloat(int _) {
    switch (_) {
    case 0: return 0.0f;
    case 1: return 65536.0f;
    default: return 1.0f / 5.0f;
    }
}

int daNpc_Kkri_c::CreateHeap() {
    void* mdlData_p = NULL;
    J3DModel* mdl_p = NULL;
    int var_r25 = mTwilight == TRUE ? 1 : 0;
    const char* arcname = l_resNameList[l_bmdData[var_r25][1]];
    int bmd_index = l_bmdData[var_r25][0];

    mdlData_p = dComIfG_getObjectRes(arcname, bmd_index);
    if (mdlData_p == NULL) {
        return 1;
    }

    u32 diff_flags = J3D_DIFF_FLAG(FALSE, FALSE, TRUE, 8, 2, FALSE, 2, 0, TRUE, FALSE, FALSE, FALSE, TRUE, FALSE);
    mpMorf[0] = new mDoExt_McaMorfSO((J3DModelData*)mdlData_p, NULL, NULL, NULL, -1, 1.0f, 0, -1, &mSound, J3DMdlFlag_DifferedDLBuffer, diff_flags);
    if (mpMorf[0] == NULL || mpMorf[0]->getModel() == NULL) {
        return 0;
    }

    mdl_p = mpMorf[0]->getModel();
    for (u16 i = 0; i < ((J3DModelData*)mdlData_p)->getJointNum(); i++) {
        ((J3DModelData*)mdlData_p)->getJointNodePointer(i)->setCallBack(ctrlJointCallBack);
    }

    mdl_p->setUserArea((uintptr_t)this);

    mpMatAnm[0] = new daNpcT_MatAnm_c();
    if (mpMatAnm[0] == NULL) {
        return 0;
    }

    if (setFaceMotionAnm(1, false) && setMotionAnm(0, 0.0f, false)) {
        return 1;
    }

    return 0;
}

int daNpc_Kkri_c::Delete() {
    OS_REPORT("|%06d:%x|daNpc_Kkri_c -> Delete\n", g_Counter.mCounter0, this);
    fpc_ProcID id = fopAcM_GetID(this);
    this->~daNpc_Kkri_c();
    return 1;
}

int daNpc_Kkri_c::Execute() {
    return execute();
}

int daNpc_Kkri_c::Draw() {
    if (mpMatAnm[0] != NULL) {
        J3DModelData* mdlData_p = mpMorf[0]->getModel()->getModelData();
        mdlData_p->getMaterialNodePointer(getEyeballMaterialNo())->setMaterialAnm(mpMatAnm[0]);
    }

    return draw(FALSE, FALSE, daNpc_Kkri_Param_c::m.common.real_shadow_size, NULL, 100.0f, FALSE, FALSE, FALSE);
}

int daNpc_Kkri_c::createHeapCallBack(fopAc_ac_c* i_this) {
    return ((daNpc_Kkri_c*)i_this)->CreateHeap();
}

int daNpc_Kkri_c::ctrlJointCallBack(J3DJoint* i_joint, int param_1) {
    if (param_1 == 0) {
        J3DModel* mdl_p = j3dSys.getModel();
        daNpc_Kkri_c* i_this = (daNpc_Kkri_c*)mdl_p->getUserArea();
        if (i_this != NULL) {
            i_this->ctrlJoint(i_joint, mdl_p);
        }
    }

    return 1;
}

void* daNpc_Kkri_c::srchYm(void* i_actor, void* i_data) {
    if (mFindCount < 50) {
        daE_YM_c* ym_p = (daE_YM_c*)i_actor;
        if (ym_p != NULL && ym_p != i_data && fopAcM_IsExecuting(fopAcM_GetID(ym_p)) && fopAcM_GetName(ym_p) == PROC_E_YM && !ym_p->isHide()) {
            mFindActorPtrs[mFindCount] = ym_p;
            mFindCount++;
        }
    }

    return NULL;
}

int daNpc_Kkri_c::getType() {
    switch (fopAcM_GetParam(this) & 0xFF) {
    case 0:
        return 0;
    case 1:
        return 1;
    default:
        return 2;
    }
}

int daNpc_Kkri_c::isDelete() {
    switch (mType) {
    case 0:
        return FALSE;
    case 1:
        return FALSE;
    default:
        return FALSE;
    }
}

void daNpc_Kkri_c::reset() {
    csXyz angle;
    u32 var_r30 = (uintptr_t)&field_0xfd8 - (uintptr_t)&mSelectAction;

    if (mpMatAnm[0] != NULL) {
        mpMatAnm[0]->initialize();
    }

    initialize();

    for (int i = 0; i < ARRAY_SIZE(mActorMng); i++) {
        mActorMng[i].initialize();
    }

    memset(&mSelectAction, 0, var_r30);

    angle.setall(0);
    angle.y = home.angle.y;

    setAngle(angle);
}

BOOL daNpc_Kkri_c::checkChangeEvt() {
    if (!chkAction(&daNpc_Kkri_c::talk)) {
        mPreItemNo = 0;
        if (dComIfGp_event_chkTalkXY()) {
            if (dComIfGp_evmng_ChkPresentEnd()) {
                mEvtNo = 2;
                evtChange();
            }
            return TRUE;
        }
    }

    return FALSE;
}

void daNpc_Kkri_c::setParam() {
    selectAction();
    srchActors();

    u32 attn_flags = fopAc_AttnFlag_SPEAK_e | fopAc_AttnFlag_TALK_e;
    s16 talk_dist = daNpc_Kkri_Param_c::m.common.talk_distance;
    s16 talk_angle = daNpc_Kkri_Param_c::m.common.talk_angle;
    s16 attn_dist = daNpc_Kkri_Param_c::m.common.attention_distance;
    s16 attn_angle = daNpc_Kkri_Param_c::m.common.attention_angle;

    if (mType == 0) {
        talk_dist = 3;
        talk_angle = 2;
        attn_dist = 5;
        attn_angle = 2;
    }

    if (daPy_getPlayerActorClass()->checkHorseRide()) {
        if (talk_dist < 7) {
            talk_dist = 7;
        }

        if (attn_dist < 9) {
            attn_dist = 9;
        }
    }

    if (mType == 1) {
        attention_info.distances[fopAc_attn_LOCK_e] = 57;
        attention_info.distances[fopAc_attn_TALK_e] = 57;
        attention_info.distances[fopAc_attn_SPEAK_e] = 57;
    } else {
        attention_info.distances[fopAc_attn_LOCK_e] = daNpcT_getDistTableIdx(attn_dist, attn_angle);
        attention_info.distances[fopAc_attn_TALK_e] = attention_info.distances[0];
        attention_info.distances[fopAc_attn_SPEAK_e] = daNpcT_getDistTableIdx(talk_dist, talk_angle);
    }

    if (daPy_py_c::checkNowWolf()) {
        attn_flags |= fopAc_AttnFlag_UNK_0x800000;
    }

    attention_info.flags = attn_flags;

    scale.set(daNpc_Kkri_Param_c::m.common.scale, daNpc_Kkri_Param_c::m.common.scale, daNpc_Kkri_Param_c::m.common.scale);

    mCcStts.SetWeight(daNpc_Kkri_Param_c::m.common.weight);
    mCylH = daNpc_Kkri_Param_c::m.common.height;
    mWallR = daNpc_Kkri_Param_c::m.common.width;
    mAttnFovY = daNpc_Kkri_Param_c::m.common.fov;
    mAcchCir.SetWallR(mWallR);
    mAcchCir.SetWallH(daNpc_Kkri_Param_c::m.common.knee_length);
    mRealShadowSize = daNpc_Kkri_Param_c::m.common.real_shadow_size;
    mExpressionMorfFrame = daNpc_Kkri_Param_c::m.common.expression_morf_frame;
    mMorfFrames = daNpc_Kkri_Param_c::m.common.morf_frame;
    gravity = daNpc_Kkri_Param_c::m.common.gravity;

    if (mType == 0) {
        mAcch.SetGrndNone();
        mAcch.SetWallNone();
        gravity = 0.0f;
    }
}

void daNpc_Kkri_c::setAfterTalkMotion() {
    int var_r31 = 14;

    switch (mFaceMotionSeqMngr.getNo()) {
    case 5:
        var_r31 = 13;
        break;
    case 6:
        return;
    case 8:
        var_r31 = 1;
        break;
    case 9:
        var_r31 = 11;
        break;
    case 10:
        var_r31 = 11;
        break;
    }

    mFaceMotionSeqMngr.setNo(var_r31, -1.0f, 0, 0);
}

void daNpc_Kkri_c::srchActors() {
    switch (mType) {
    case 0:
        if (mActorMng[0].getActorP() == NULL) {
            mActorMng[0].entry(getNearestActorP(PROC_Tag_KtOnFire));
        }
        /* fallthrough */
    case 1:
        return;
    }
}

BOOL daNpc_Kkri_c::evtTalk() {
    if (chkAction(&daNpc_Kkri_c::talk)) {
        (this->*mAction)(NULL);
    } else {
        setAction(&daNpc_Kkri_c::talk);
    }

    return TRUE;
}

BOOL daNpc_Kkri_c::evtCutProc() {
    int staffId = dComIfGp_getEventManager().getMyStaffId("Kkri", this, -1);
    if (staffId != -1) {
        mStaffId = staffId;
        int actIdx = dComIfGp_getEventManager().getMyActIdx(mStaffId, mCutNameList, ARRAY_SIZE(mCutNameList), 0, 0);

        if ((this->*mCutList[actIdx])(mStaffId)) {
            dComIfGp_getEventManager().cutEnd(mStaffId);
        }

        return TRUE;
    }

    return FALSE;
}

void daNpc_Kkri_c::action() {
    fopAc_ac_c* hit_actor = hitChk(&mCcCyl, -1);
    if (mStagger.checkRebirth()) {
        mStagger.initialize();
        mMode = 1;
    }

    if (mSelectAction != NULL) {
        if (mAction == mSelectAction) {
            (this->*mAction)(NULL);
        } else {
            setAction(mSelectAction);
        }
    }
}

void daNpc_Kkri_c::beforeMove() {
    fopAcM_OffStatus(this, fopAcM_STATUS_UNK_0x8000000);

    if (checkHide()) {
        fopAcM_OnStatus(this, fopAcM_STATUS_UNK_0x8000000);
    }

    if (checkHide() || mNoDraw) {
        attention_info.flags = 0;
    }
}

void daNpc_Kkri_c::setAttnPos() {
    cXyz sp34(-30.0f, 0.0f, 0.0f);

    if (field_0xfd4 != 0) {
        daPy_getPlayerActorClass()->onWolfEyeKeep();
    }

    f32 var_f31 = cM_s2rad(mCurAngle.y - field_0xd7e.y);

    if (mType == 0) {
        mJntAnm.setParam(this, mpMorf[0]->getModel(), &sp34, getBackboneJointNo(), getNeckJointNo(), getHeadJointNo(),
            0.0f, 0.0f,
            0.0f, 0.0f,
            daNpc_Kkri_Param_c::m.common.head_angleX_min, daNpc_Kkri_Param_c::m.common.head_angleX_max,
            -30.0f, 30.0f,
            daNpc_Kkri_Param_c::m.common.neck_rotation_ratio, 0.0f, NULL);
    } else if (mType == 1) {
        mJntAnm.setParam(this, mpMorf[0]->getModel(), &sp34, getBackboneJointNo(), getNeckJointNo(), getHeadJointNo(),
            0.0f, 0.0f,
            0.0f, 0.0f,
            daNpc_Kkri_Param_c::m.common.head_angleX_min, daNpc_Kkri_Param_c::m.common.head_angleX_max,
            daNpc_Kkri_Param_c::m.common.head_angleY_min, daNpc_Kkri_Param_c::m.common.head_angleY_max,
            daNpc_Kkri_Param_c::m.common.neck_rotation_ratio, 0.0f, NULL);
    } else {
        mJntAnm.setParam(this, mpMorf[0]->getModel(), &sp34, getBackboneJointNo(), getNeckJointNo(), getHeadJointNo(),
            daNpc_Kkri_Param_c::m.common.body_angleX_min, daNpc_Kkri_Param_c::m.common.body_angleX_max,
            daNpc_Kkri_Param_c::m.common.body_angleY_min, daNpc_Kkri_Param_c::m.common.body_angleY_max,
            daNpc_Kkri_Param_c::m.common.head_angleX_min, daNpc_Kkri_Param_c::m.common.head_angleX_max,
            daNpc_Kkri_Param_c::m.common.head_angleY_min, daNpc_Kkri_Param_c::m.common.head_angleY_max,
            daNpc_Kkri_Param_c::m.common.neck_rotation_ratio, 0.0f, NULL);
    }

    mJntAnm.calcJntRad(0.2f, 1.0f, var_f31);

    setMtx();

    mDoMtx_stack_c::copy(mpMorf[0]->getModel()->getAnmMtx(getHeadJointNo()));
    mDoMtx_stack_c::multVec(&sp34, &eyePos);

    mJntAnm.setEyeAngleX(eyePos, 1.0f, 0);
    mJntAnm.setEyeAngleY(eyePos, mCurAngle.y, 0, 1.0f, 0);

    sp34.set(0.0f, 0.0f, 0.0f);
    sp34.y = daNpc_Kkri_Param_c::m.common.attention_offset;

    if (mType == 0) {
        sp34.set(0.0f, 160.0f, -20.0f);
    } else if (mType == 1) {
        sp34.set(0.0f, 160.0f, 60.0f);
    }

    mDoMtx_stack_c::YrotS(mCurAngle.y);
    mDoMtx_stack_c::multVec(&sp34, &sp34);
    attention_info.position = sp34 + current.pos;
}

void daNpc_Kkri_c::setCollision() {
    cXyz pos;

    if (!mHide) {
        u32 tg_type = 0xD8FBFDFF;
        u32 tg_sprm = 0x1F;
        if (dComIfGp_event_runCheck()) {
            tg_type = 0;
            tg_sprm = 0;
        } else if (mTwilight) {
            tg_type = 0;
            tg_sprm = 0;
        } else if (mStagger.checkStagger()) {
            tg_type = 0;
            tg_sprm = 0;
        }

        mCcCyl.SetCoSPrm(0x79);
        mCcCyl.SetTgType(tg_type);
        mCcCyl.SetTgSPrm(tg_sprm);
        mCcCyl.OnTgNoHitMark();

        pos.set(0.0f, 0.0f, 0.0f);
        f32 cyl_h = mCylH;
        f32 cyl_r = mWallR;

        if (mType == 0) {
            pos.set(0.0f, 0.0f, -20.0f);
            cyl_h = 140.0f;
            cyl_r = 40.0f;
        } else if (mType == 1) {
            pos.set(0.0f, 0.0f, 20.0f);
            cyl_h = 150.0f;
            cyl_r = 40.0f;
        }

        mDoMtx_stack_c::YrotS(mCurAngle.y);
        mDoMtx_stack_c::multVec(&pos, &pos);
        pos += current.pos;

        mCcCyl.SetH(cyl_h);
        mCcCyl.SetR(cyl_r);
        mCcCyl.SetC(pos);
        dComIfG_Ccsp()->Set(&mCcCyl);
    }

    mCcCyl.ClrCoHit();
    mCcCyl.ClrTgHit();
}

int daNpc_Kkri_c::drawDbgInfo() {
    return 0;
}

int daNpc_Kkri_c::selectAction() {
    mSelectAction = NULL;

    switch (mType) {
    case 0:
        mSelectAction = &daNpc_Kkri_c::sitWait1;
        break;
    case 1:
        mSelectAction = &daNpc_Kkri_c::fearWait;
        break;
    default:
        mSelectAction = &daNpc_Kkri_c::wait;
        break;
    }

    return 1;
}

BOOL daNpc_Kkri_c::chkAction(int (daNpc_Kkri_c::*i_action)(void*)) {
    return mAction == i_action;
}

BOOL daNpc_Kkri_c::setAction(int (daNpc_Kkri_c::*i_action)(void*)) {
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

int daNpc_Kkri_c::cutConversationAboutSoup(int i_staffId) {
    int rt = 0;
    int prm = -1;
    int timer = 0;

    int* data_p = dComIfGp_evmng_getMyIntegerP(i_staffId, "prm");
    if (data_p != NULL) {
        prm = *data_p;
    }

    data_p = dComIfGp_evmng_getMyIntegerP(i_staffId, "timer");
    if (data_p != NULL) {
        timer = *data_p;
    }

    if (dComIfGp_getEventManager().getIsAddvance(i_staffId)) {
        switch (prm) {
        case 0:
            initTalk(103, NULL);
            mEventTimer = timer;
        }
    }

    switch (prm) {
    case 0:
        mJntAnm.lookPlayer(0);
        if (cLib_calcTimer<int>(&mEventTimer) == 0 && talkProc(NULL, 0, NULL, 0) && mFlow.checkEndFlow()) {
            rt = 1;
        }
    }

    return rt;
}

int daNpc_Kkri_c::cutYmLook(int i_staffId) {
    int rt = 0;
    int prm = -1;
    int timer = 0;

    int* data_p = dComIfGp_evmng_getMyIntegerP(i_staffId, "prm");
    if (data_p != NULL) {
        prm = *data_p;
    }

    data_p = dComIfGp_evmng_getMyIntegerP(i_staffId, "timer");
    if (data_p != NULL) {
        timer = *data_p;
    }

    if (dComIfGp_getEventManager().getIsAddvance(i_staffId)) {
        switch (prm) {
        case 0:
            mEventTimer = timer;
        }
    }

    switch (prm) {
    case 0:
        mFindCount = 0;
        fopAcM_Search(srchYm, this);
        if (mFindCount == 0 && cLib_calcTimer<int>(&mEventTimer) == 0) {
            field_0xfd7 = 1;
            rt = 1;
        }
    }

    return rt;
}

int daNpc_Kkri_c::sleep() {
    int time_hour = dKy_getdaytime_hour();
    int rnd_value = cLib_getRndValue<int>(150, 100);

    if (field_0xfd6 != 0) {
        if (field_0xfcc != 0) {
            if (time_hour >= 6 && time_hour < 21) {
                mFaceMotionSeqMngr.setNo(0xE, -1.0f, 0, 0);
                mMotionSeqMngr.setNo(0, -1.0f, 0, 0);
                field_0xfcc = 0;
                field_0xfd6 = 0;
            } else if (cLib_calcTimer<int>(&field_0xfcc) == 0) {
                mFaceMotionSeqMngr.setNo(1, -1.0f, 0, 0);
                mMotionSeqMngr.setNo(0x12, -1.0f, 0, 0);
            }
        } else if (mMotionSeqMngr.checkEndSequence()) {
            field_0xfcc = rnd_value;
            mFaceMotionSeqMngr.setNo(1, -1.0f, 0, 0);
            mMotionSeqMngr.setNo(2, -1.0f, 0, 0);
        }
    } else if (cLib_calcTimer<int>(&field_0xfcc) == 0 && (time_hour >= 21 || time_hour < 6)) {
        field_0xfcc = rnd_value;
        mFaceMotionSeqMngr.setNo(1, -1.0f, 0, 0);
        mMotionSeqMngr.setNo(2, -1.0f, 0, 0);
        field_0xfd6 = 1;
    }

    return field_0xfd6;
}

int daNpc_Kkri_c::wait(void*) {
    switch (mMode) {
    case 0:
    case 1:
        if (!mStagger.checkStagger()) {
            mFaceMotionSeqMngr.setNo(0xE, -1.0f, 0, 0);
            mMotionSeqMngr.setNo(1, -1.0f, 0, 0);
            mMode = 2;
        }
    case 2:
        if (!mStagger.checkStagger()) {
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
                break;
            }
        }
        break;
    case 3:
        break;
    }

    return 1;
}

int daNpc_Kkri_c::sitWait1(void*) {
    BOOL var_r29 = FALSE;

    switch (mMode) {
    case 0:
    case 1:
        if (!mStagger.checkStagger()) {
            mFaceMotionSeqMngr.setNo(14, -1.0f, 0, 0);
            mMotionSeqMngr.setNo(0, -1.0f, 0, 0);
            field_0xfd6 = 0;
            field_0xfd0 = 0;
            mMode = 2;
        }
    case 2:
        if (mType == 0) {
            if (field_0xfd5 != 0) {
                mPlayerActorMngr.entry(daPy_getPlayerActorClass());
                mSpeakEvent = 1;
                field_0xe33 = 1;
            } else if (dComIfGs_isSaveDunSwitch(1) != 0) {
                if (!daNpcT_chkEvtBit(0x159) && getBitSW() != 0xFF && dComIfGs_isSwitch(getBitSW(), fopAcM_GetRoomNo(this))) {
                    mPlayerActorMngr.remove();
                    var_r29 = TRUE;
                    mSpeakEvent = 1;
                    field_0xe33 = 1;
                } else if (!daNpcT_chkEvtBit(0x2F1)) {
                    if (dComIfGp_getBottleStatus() == 0x6C) {
                        if (field_0xfd0 != 0 && !daPy_getPlayerActorClass()->checkEmptyBottleSwing()) {
                            mEvtNo = 3;
                            field_0xe33 = 1;
                        }
                        field_0xfd0 = daPy_getPlayerActorClass()->checkEmptyBottleSwing();
                    } else {
                        field_0xfd0 = 0;
                    }
                }
            } else if (dComIfGp_getBottleStatus() == 0x6C && daPy_getPlayerActorClass()->checkEmptyBottleSwing()) {
                mEvtNo = 3;
                field_0xe33 = 1;
            }
        }

        if (!mStagger.checkStagger()) {
            if (mPlayerActorMngr.getActorP() != NULL) {
                mJntAnm.lookPlayer(0);

                if (!chkActorInSight(mPlayerActorMngr.getActorP(), mAttnFovY, mCurAngle.y)) {
                    mJntAnm.lookNone(0);
                }

                if (field_0xfd6 != 0) {
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

            daNpcT_offTmpBit(0x29);

            switch (mJntAnm.getMode()) {
            case 0:
                if (mType == 0 && field_0xfd6 == 0 && var_r29) {
                    fopAc_ac_c* actor_p = mActorMng[0].getActorP();
                    if (actor_p != NULL && field_0xfd6 == 0) {
                        mJntAnm.lookActor(actor_p, 30.0f, 0);
                    }
                }
                sleep();
            }

            if (field_0xfd6 != 0) {
                daNpcT_onTmpBit(0x29);
            }
        }
        break;
    case 3:
        break;
    }

    return 1;
}

int daNpc_Kkri_c::fearWait(void*) {
    fopAc_ac_c* nearest_actor = NULL;
    f32 nearest_dist = G_CM3D_F_INF;

    switch (mMode) {
    case 0:
    case 1:
        mFaceMotionSeqMngr.setNo(2, -1.0f, 0, 0);
        mMotionSeqMngr.setNo(3, -1.0f, 0, 0);
        mMode = 2;
    case 2:
        mJntAnm.lookNone(0);
        if (getBitSW2() != 0xFF) {
            if (dComIfGs_isSwitch(getBitSW2(), fopAcM_GetRoomNo(this))) {
                mMotionSeqMngr.setNo(0xC, -1.0f, 0, 0);
                field_0xfd4 = 0;
            } else if (field_0xfd7 != 0) {
                mSpeakEvent = 1;
                field_0xe33 = 1;
            } else {
                mFindCount = 0;
                fopAcM_Search(srchYm, this);
                
                int alive_ym_cnt = 0;
                for (int i = 0; i < mFindCount; i++) {
                    if (mFindActorPtrs[i]->health > 0) {
                        alive_ym_cnt++;
                    }
                }

                if (mFindCount != 0 && alive_ym_cnt == 0) {
                    mMotionSeqMngr.setNo(0xB, -1.0f, 0, 0);
                    mJntAnm.lookNone(0);
                    field_0xfd4 = 1;
                    mEvtNo = 4;
                }

                if (alive_ym_cnt != 0) {
                    for (int i = 0; i < mFindCount; i++) {
                        if (fopAcM_searchActorDistance(this, mFindActorPtrs[i]) < nearest_dist) {
                            nearest_actor = mFindActorPtrs[i];
                            nearest_dist = fopAcM_searchActorDistance(this, nearest_actor);
                        }
                    }

                    if (nearest_actor != NULL) {
                        mJntAnm.lookActor(nearest_actor, 0.0f, 0);
                    }
                }
            }
        }
        break;
    case 3:
        break;
    }

    return 1;
}

int daNpc_Kkri_c::talk(void*) {
    switch (mMode) {
    case 0:
    case 1:
        if (!mStagger.checkStagger()) {
            if ((mFlowNodeNo == 0x65 || mFlowNodeNo == 0x6D) && field_0xfd5 == 0) {
                daNpcT_offTmpBit(0xB);
                daNpcT_offTmpBit(0xC);
                daNpcT_offTmpBit(0x2A);
            }

            if (!daNpcT_chkEvtBit(0x159)) {
                if (getBitSW() != 0xFF && dComIfGs_isSwitch(getBitSW(), fopAcM_GetRoomNo(this))) {
                    initTalk(122, NULL);
                } else {
                    if (!daNpcT_chkEvtBit(0x80) && field_0xfd5 == 0) {
                        daNpcT_offTmpBit(0xB);
                    }
                    initTalk(mFlowNodeNo, NULL);
                }
            } else {
                initTalk(mFlowNodeNo, NULL);
            }

            mMode = 2;
        }
    case 2:
        if (!mStagger.checkStagger()) {
            if (mTwilight || mType == 0) {
                if (talkProc(NULL, 0, NULL, 0)) {
                    int item_no;
                    int eventId = mFlow.getEventId(&item_no);
                    field_0xfd5 = 0;

                    switch (eventId) {
                    case 1:
                        if (mItemPartnerId == fpcM_ERROR_PROCESS_ID_e) {
                            mItemPartnerId = fopAcM_createItemForPresentDemo(&current.pos, item_no, 0, -1, -1, NULL, NULL);
                        }

                        if (fopAcM_IsExecuting(mItemPartnerId)) {
                            field_0xfd5 = 1;
                            mEvtNo = 1;
                            evtChange();
                        }
                        break;
                    default:
                        if (mFlow.checkEndFlow()) {
                            dComIfGp_event_reset();
                            field_0xfd5 = 0;
                            mMode = 3;
                        }
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

static int daNpc_Kkri_Create(void* i_this) {
    return ((daNpc_Kkri_c*)i_this)->create();
}

static int daNpc_Kkri_Delete(void* i_this) {
    return ((daNpc_Kkri_c*)i_this)->Delete();
}

static int daNpc_Kkri_Execute(void* i_this) {
    return ((daNpc_Kkri_c*)i_this)->Execute();
}

static int daNpc_Kkri_Draw(void* i_this) {
    return ((daNpc_Kkri_c*)i_this)->Draw();
}

static int daNpc_Kkri_IsDelete(void* i_this) {
    return 1;
}

static daNpc_Kkri_Param_c l_HIO;

static actor_method_class daNpc_Kkri_MethodTable = {
    (process_method_func)daNpc_Kkri_Create,
    (process_method_func)daNpc_Kkri_Delete,
    (process_method_func)daNpc_Kkri_Execute,
    (process_method_func)daNpc_Kkri_IsDelete,
    (process_method_func)daNpc_Kkri_Draw,
};

actor_process_profile_definition g_profile_NPC_KKRI = {
  fpcLy_CURRENT_e,         // mLayerID
  7,                       // mListID
  fpcPi_CURRENT_e,         // mListPrio
  PROC_NPC_KKRI,           // mProcName
  &g_fpcLf_Method.base,   // sub_method
  sizeof(daNpc_Kkri_c),    // mSize
  0,                       // mSizeOther
  0,                       // mParameters
  &g_fopAc_Method.base,    // sub_method
  346,                     // mPriority
  &daNpc_Kkri_MethodTable, // sub_method
  0x00040107,              // mStatus
  fopAc_NPC_e,             // mActorType
  fopAc_CULLBOX_CUSTOM_e,  // cullType
};
