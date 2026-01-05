/**
 * @file d_a_npc_bou.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_npc_bou.h"
#include "d/actor/d_a_cow.h"
#include "d/actor/d_a_horse.h"
#include "d/actor/d_a_npc_jagar.h"
#include "d/actor/d_a_tag_push.h"
#include "d/d_meter2_info.h"

static int l_bmdData[1][2] = {
    {11, 1},
};

daNpcT_evtData_c l_evtList[11] = {
    {"", 0},
    {"NO_RESPONSE", 0},
    {"WILDGOAT", 2},
    {"WILDGOAT_SUCCESS", 2},
    {"WILDGOAT_FAILURE", 2},
    {"SPEAKTO", 2},
    {"CONFIDENTIAL_CONVERSATION", 6},
    {"FIND_WOLF", 6},
    {"MEETING_AGAIN", 4},
    {"MEETING_AGAIN_PLAYER_ON_HORSE", 4},
    {"MEETING_AGAIN_NEAR", 4},
};

static char* l_resNameList[7] = {
    "",
    "Bou",
    "Bou1",
    "Bou2",
    "Bou4",
    "Bou_p1",
    "Jagar2"
};

static s8 l_loadResPtrn0[4] = {1, 2, 4, -1};

static s8 l_loadResPtrn1[3] = {1, 3, -1};

static s8 l_loadResPtrn2[3] = {1, 4, -1};

static s8 l_loadResPtrn3[3] = {1, 5, -1};

static s8 l_loadResPtrn9[5] = {1, 2, 3, 4, -1};

static s8* l_loadResPtrnList[6] = {
    l_loadResPtrn0, l_loadResPtrn1, l_loadResPtrn2,
    l_loadResPtrn3, l_loadResPtrn0, l_loadResPtrn9,
};

static daNpcT_faceMotionAnmData_c l_faceMotionAnmData[11] = {
    {-1, 0, 0, 17, 2, 1, 1},
    {6, 0, 1, 17, 2, 1, 1},
    {4, 0, 2, 17, 2, 1, 1},
    {4, 2, 3, 16, 2, 3, 0},
    {5, 0, 3, 17, 0, 3, 0},
    {6, 2, 3, 18, 2, 3, 0},
    {7, 0, 4, 25, 0, 4, 1},
    {8, 2, 4, 26, 2, 4, 1},
    {11, 2, 4, 28, 2, 4, 1},
    {9, 0, 4, 17, 2, 1, 1},
    {10, 0, 4, 27, 0, 4, 1},
};

static daNpcT_motionAnmData_c l_motionAnmData[23] = {
    {8, 2, 1, 14, 0, 1, 1, 0},
    {5, 0, 2, 14, 0, 1, 1, 0},
    {7, 2, 3, 14, 0, 1, 1, 0},
    {7, 0, 1, 14, 0, 1, 1, 0},
    {21, 2, 4, 14, 0, 1, 1, 0},
    {12, 2, 3, 14, 0, 1, 1, 0},
    {11, 0, 3, 14, 0, 1, 1, 0},
    {13, 2, 3, 14, 0, 1, 1, 0},
    {8, 0, 3, 14, 0, 1, 1, 0},
    {10, 0, 3, 14, 0, 1, 1, 0},
    {9, 2, 3, 14, 0, 1, 1, 0},
    {20, 2, 4, 14, 0, 1, 1, 0},
    {22, 2, 4, 14, 0, 1, 1, 0},
    {14, 0, 4, 14, 0, 1, 1, 0},
    {16, 0, 4, 14, 0, 1, 1, 0},
    {19, 0, 4, 14, 0, 1, 1, 0},
    {12, 0, 4, 14, 0, 1, 1, 0},
    {13, 0, 4, 14, 0, 1, 1, 0},
    {15, 0, 4, 14, 0, 1, 1, 0},
    {5, 0, 4, 14, 0, 1, 1, 0},
    {6, 2, 4, 14, 0, 1, 1, 0},
    {17, 0, 4, 14, 0, 1, 1, 0},
    {18, 2, 4, 14, 0, 1, 1, 0}
};

static daNpcT_MotionSeqMngr_c::sequenceStepData_c l_faceMotionSequenceData[44] = {
    {1, -1, 1}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {4, 0, 1}, {5, -1, 0}, {-1, 0, 0}, {-1, 0, 0},
    {9, -1, 1}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {6, -1, 1}, {7, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {10, -1, 1}, {8, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {2, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {5, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {3, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {7, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {8, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {0, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}
};

static daNpcT_MotionSeqMngr_c::sequenceStepData_c l_motionSequenceData[88] = {
    {0, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {5, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {6, -1, 1}, {5, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {8, 0, 1}, {10, -1, 0}, {-1, 0, 0}, {-1, 0, 0},
    {9, -1, 1}, {5, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {10, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {11, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {12, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {1, -1, 1}, {0, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {14, -1, 1}, {11, -1, 0}, {-1, 0, 0}, {-1, 0, 0},
    {15, -1, 1}, {12, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {16, -1, 1}, {11, -1, 0}, {-1, 0, 0}, {-1, 0, 0},
    {17, -1, 1}, {11, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {18, -1, 1}, {11, -1, 0}, {-1, 0, 0}, {-1, 0, 0},
    {19, -1, 1}, {20, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {20, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {21, -1, 1}, {22, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {22, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {3, -1, 1}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {13, -1, 1}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {2, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {6, -1, 1}, {5, -1, 0}, {-1, 0, 0}, {-1, 0, 0},
};

char* daNpc_Bou_c::mCutNameList[9] = {
    "",
    "WILDGOAT",
    "WILDGOAT_SUCCESS",
    "WILDGOAT_FAILURE",
    "SPEAKTO",
    "CONVERSATION_WITH_BOU",
    "CONFIDENTIAL_CONVERSATION",
    "FIND_WOLF",
    "MEETING_AGAIN"
};

daNpc_Bou_c::cutFunc daNpc_Bou_c::mCutList[9] = {
    NULL,
    &daNpc_Bou_c::cutWildGoat,
    &daNpc_Bou_c::cutWildGoatSuccess,
    &daNpc_Bou_c::cutWildGoatFailure,
    &daNpc_Bou_c::cutSpeakTo,
    &daNpc_Bou_c::cutConversationWithBou,
    &daNpc_Bou_c::cutConfidentialConversation,
    &daNpc_Bou_c::cutFindWolf,
    &daNpc_Bou_c::cutMeetingAgain
};

daNpc_Bou_c::~daNpc_Bou_c() {
    if (mpMorf[0] != 0) {
        mpMorf[0]->stopZelAnime();
    }
    deleteRes(l_loadResPtrnList[mType], (char const**)l_resNameList);
}

daNpc_Bou_Param_c::Data const daNpc_Bou_Param_c::m= {
    210.0f, -3.0f, 1.0f, 600.0f, 255.0f, 200.0f, 35.0f, 40.0f,
    0.0f, 0.0f, 10.0f, -10.0f, 30.0f, -10.0f, 45.0f, -45.0f,
    0.6f, 12.0f, 3, 6, 5, 6, 110.0f, 500.0f, 300.0f, -300.0f,
    60, 8, 0.0f, 0.0f, 4.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 16.0f, 1000.0f, 500.0f, -500.0f,
};

int daNpc_Bou_c::create() {
    static int const heapSize[6] = {15696, 15696, 15696, 15696, 15696, 0};
    daNpcT_ct(this, daNpc_Bou_c, l_faceMotionAnmData, l_motionAnmData,
                       l_faceMotionSequenceData, 4, l_motionSequenceData, 4,
                       l_evtList, l_resNameList
    );
    mType = getType();
    mFlowNodeNo = getFlowNodeNo();
    mTwilight = 0;
    int rv = loadRes(l_loadResPtrnList[mType], (const char**)l_resNameList);
    if (rv == cPhs_COMPLEATE_e) {
        if (isDelete()) {
            return cPhs_ERROR_e;
        }

        if (!fopAcM_entrySolidHeap(this, createHeapCallBack, heapSize[mType])) {
            return cPhs_ERROR_e;
        }

        J3DModelData* modelData = mpMorf[0]->getModel()->getModelData();
        fopAcM_SetMtx(this, mpMorf[0]->getModel()->getBaseTRMtx());
        fopAcM_setCullSizeBox(this, -200.0f, -100.0f, -200.0f, 200.0f, 300.0f, 200.0f);
        mSound.init(&current.pos, &eyePos, 3, 1);
        reset();
        mAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1,
                        &mAcchCir, fopAcM_GetSpeed_p(this), fopAcM_GetAngle_p(this),
                        fopAcM_GetShapeAngle_p(this));
        mCcStts.Init(daNpc_Bou_Param_c::m.field_0x10, 0, this);
        mCyl1.Set(mCcDCyl);
        mCyl1.SetStts(&mCcStts);
        mCyl1.SetTgHitCallback(tgHitCallBack);
        mAcch.CrrPos(dComIfG_Bgsp());
        mGndChk = mAcch.m_gnd;
        mGroundH = mAcch.GetGroundH();
        if (mGroundH != -G_CM3D_F_INF) {
            setEnvTevColor();
            setRoomNo();
        }

        mCreating = 1;
        Execute();
        mCreating = 0;
    }

    return rv;
}

static f32 dummy(u8 i_dummy) {
    if (i_dummy == 0) {
        return 0.0f;
    } else if (i_dummy == 1) {
        return 65536.0f;
    } else {
        return 0.2f;
    }
}

int daNpc_Bou_c::CreateHeap() {
    J3DModelData* modelData = static_cast<J3DModelData*>(dComIfG_getObjectRes(
        l_resNameList[l_bmdData[0][1]], l_bmdData[0][0]));
    if (modelData == NULL) {
        return 0;
    }

    mpMorf[0] = new mDoExt_McaMorfSO(modelData, NULL, NULL, NULL, -1, 1.0f, 0, -1, &mSound,
        0x80000, 0x11020284);
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

    if (setFaceMotionAnm(1, false) && setMotionAnm(0, 0.0f, 0)) {
        return 1;
    }

    return 0;
}

int daNpc_Bou_c::Delete() {
    fopAcM_GetID(this);
    this->~daNpc_Bou_c();
    return 1;
}

int daNpc_Bou_c::Execute() {
    return daNpcT_c::execute();
}

int daNpc_Bou_c::Draw() {
    if (mpMatAnm[0] != NULL) {
        J3DModelData* modelData = mpMorf[0]->getModel()->getModelData();
        modelData->getMaterialNodePointer(getEyeballMaterialNo())->setMaterialAnm(mpMatAnm[0]);
    }
    return daNpcT_c::draw(0, 0, mRealShadowSize, NULL, 100.0f, 0, 0, 0);
}

int daNpc_Bou_c::createHeapCallBack(fopAc_ac_c* i_this) {
    return static_cast<daNpc_Bou_c*>(i_this)->CreateHeap();
}

int daNpc_Bou_c::ctrlJointCallBack(J3DJoint* param_0, int param_1) {
    if (param_1 == 0) {
        J3DModel* model = j3dSys.getModel();
        daNpc_Bou_c* i_this = reinterpret_cast<daNpc_Bou_c*>(model->getUserArea());
        if (i_this != 0) {
            i_this->ctrlJoint(param_0, model);
        }
    }

    return 1;
}

void* daNpc_Bou_c::srchCow(void* arg0, void* arg1) {
    if (mFindCount < 50 && arg0 != NULL && arg0 != arg1) {
        if (fopAcM_IsExecuting(fopAcM_GetID(arg0)) && fopAcM_GetName(arg0) == PROC_COW) {
            mFindActorPtrs[mFindCount] = (fopAc_ac_c*)arg0;
            mFindCount++;
        }
    }
    return NULL;
}

fopAc_ac_c* daNpc_Bou_c::getCowP() {
    fopAc_ac_c* pActor = NULL;
    f32 minDist = G_CM3D_F_INF;
    mFindCount = 0;
    fopAcM_Search(srchCow, this);
    for (int i = 0; i < mFindCount; i++) {
        if (fopAcM_searchActorDistance(this, mFindActorPtrs[i]) < minDist)
        {
            minDist = fopAcM_searchActorDistance(this, mFindActorPtrs[i]);
            pActor = mFindActorPtrs[i];
        }
    }

    return pActor;
}

u8 daNpc_Bou_c::getType() {
    switch ((u8)fopAcM_GetParam(this)) {
        case 0:
            return TYPE_0;
        case 1:
            return TYPE_1;
        case 2:
            return TYPE_2;
        case 3:
            return TYPE_3;
        case 4:
            return TYPE_4;
        default:
            return TYPE_5;
    }
}

int daNpc_Bou_c::isDelete() {
    switch (mType) {
        case TYPE_0: {
            return FALSE;
        }

        case TYPE_1: {
            bool rv = true;
            if (!daNpcT_chkEvtBit(0xd3) && !dComIfGs_isCollectShield(0)) {
                rv = false;
            }

            return rv;
        }

        case TYPE_2: {
            bool rv = false;
            if (!daNpcT_chkEvtBit(0xa4) || daNpcT_chkEvtBit(0x3b)) {
                rv = true;
            }

            return rv;
        }

        case TYPE_3: {
            return FALSE;
        }

        case TYPE_4: {
            return FALSE;
        }

        default: {
            return FALSE;
        }
    }
}

void daNpc_Bou_c::reset() {
    csXyz acStack_20;
    int iVar1 = (u8*)&field_0xff8 - (u8*)&field_0xfc4;
    if (mpMatAnm[0] != NULL) {
        mpMatAnm[0]->initialize();
    }

    initialize();
    for (int i = 0; i < 3; i++) {
        mActorMngr[i].initialize();
    }

    memset(&field_0xfc4, 0, iVar1);
    acStack_20.setall(0);
    acStack_20.y = home.angle.y;
    daNpcT_offTmpBit(0x1C);
    daNpcT_offTmpBit(0x59);
    setAngle(acStack_20);
}

void daNpc_Bou_c::afterJntAnm(int param_1) {
    if (param_1 == 1) {
        mDoMtx_stack_c::YrotM(mStagger.getAngleZ(1));
        mDoMtx_stack_c::ZrotM(-mStagger.getAngleX(1));
    } else if (param_1 == 4) {
        mDoMtx_stack_c::YrotM(mStagger.getAngleZ(0));
        mDoMtx_stack_c::ZrotM(mStagger.getAngleX(0));
    }
}

void daNpc_Bou_c::setParam() {
    selectAction();
    srchActors();
    u32 uVar7 = (fopAc_AttnFlag_SPEAK_e | fopAc_AttnFlag_TALK_e);
    s16 sVar10 = daNpc_Bou_Param_c::m.field_0x48;
    s16 sVar9 = daNpc_Bou_Param_c::m.field_0x4a;
    s16 sVar8 = daNpc_Bou_Param_c::m.field_0x4c;
    s16 sVar7 = daNpc_Bou_Param_c::m.field_0x4e;
    if (mType == TYPE_1) {
        field_0xfe0 = 3;
        field_0xfe4 = 6;
        sVar10 = 19;
        sVar9 = 6;
        sVar8 = 19;
        sVar7 = 6;
    }

    if (daPy_getPlayerActorClass()->checkHorseRide()) {
        if (sVar10 < 7) {
            sVar10 = 7;
        }

        if (sVar8 < 9) {
            sVar8 = 9;
        }
    }

    attention_info.distances[0] = daNpcT_getDistTableIdx(sVar8, sVar7);
    attention_info.distances[1] = attention_info.distances[0];
    attention_info.distances[3] = daNpcT_getDistTableIdx(sVar10, sVar9);
    if (mType == TYPE_1) {
        uVar7 |= fopAc_AttnFlag_UNK_0x800000;
        field_0xfdc = getActorDistance(daPy_getPlayerActorClass(), 
                    daNpcT_getDistTableIdx(field_0xfe0, field_0xfe4), 
                    attention_info.distances[1]);
        if (field_0xfdc < 4) {
            g_meter2_info.mBlinkButton |= 1;
        }

        field_0xe2d = 1;
    }

    attention_info.flags = uVar7;
    scale.set(daNpc_Bou_Param_c::m.field_0x08, daNpc_Bou_Param_c::m.field_0x08,
            daNpc_Bou_Param_c::m.field_0x08);
    mCcStts.SetWeight(daNpc_Bou_Param_c::m.field_0x10);
    mCylH = daNpc_Bou_Param_c::m.field_0x14;
    mWallR = daNpc_Bou_Param_c::m.field_0x1c;
    mAttnFovY = daNpc_Bou_Param_c::m.field_0x50;
    mAcchCir.SetWallR(mWallR);
    mAcchCir.SetWallH(daNpc_Bou_Param_c::m.field_0x18);
    mRealShadowSize = daNpc_Bou_Param_c::m.field_0x0c;
    mExpressionMorfFrame = daNpc_Bou_Param_c::m.field_0x6c;
    mMorfFrames = daNpc_Bou_Param_c::m.field_0x44;
    gravity = daNpc_Bou_Param_c::m.field_0x04;
}

BOOL daNpc_Bou_c::checkChangeEvt() {
    if (!chkAction(&daNpc_Bou_c::talk)) {
        mPreItemNo = 0;
        if (dComIfGp_event_chkTalkXY()) {
            if (dComIfGp_evmng_ChkPresentEnd()) {
                mEvtNo = 1;
                evtChange();
            }

            return true;
        }

        switch (mType) {
            case TYPE_0: {
                if (!daNpcT_chkEvtBit(0x24A)) {
                    mEvtNo = 2;
                    evtChange();
                    return true;
                }

                break;
            }

            case TYPE_1: {
                if (field_0xfdc < 4) {
                    mEvtNo = 6;
                    evtChange();
                    return true;
                }

                break;
            }

            case TYPE_4: {
                if (!daNpcT_chkEvtBit(0xB3)) {
                    mEvtNo = 2;
                    evtChange();
                    return true;
                }

                break;
            }
        }
    }

    return false;
}

void daNpc_Bou_c::setAfterTalkMotion() {
    int iVar2 = 10;
    switch(mFaceMotionSeqMngr.getNo()) {
        case 1:
            iVar2 = 6;
            break;
        case 3:
            iVar2 = 8;
            break;
        case 4:
            iVar2 = 9;
            break;
    }
    mFaceMotionSeqMngr.setNo(iVar2, -1.0f, 0, 0);
}

void daNpc_Bou_c::srchActors() {
    switch (mType) {
        case TYPE_0: {
            if (!mActorMngr[0].getActorP()) {
                mActorMngr[0].entry(getCowP());
            }

            break;
        }

        case TYPE_1: {
            if (!mActorMngr[2].getActorP()) {
                mActorMngr[2].entry(getNearestActorP(0x15A));
            }

            if (!mActorMngr[1].getActorP()) {
                mActorMngr[1].entry(getNearestActorP(0x258));
            }
            break;
        }

        case TYPE_4: {
            if (!mActorMngr[0].getActorP()) {
                mActorMngr[0].entry(getCowP());
            }

            break;
        }
    }
}

BOOL daNpc_Bou_c::evtTalk() {
    if (chkAction(&daNpc_Bou_c::talk)) {
        (this->*field_0xfd0)(NULL);
    } else {
        setAction(&daNpc_Bou_c::talk);
    }

    return 1;
}

BOOL daNpc_Bou_c::evtEndProc() {
    return true;
}

BOOL daNpc_Bou_c::evtCutProc() {
    int staffId = dComIfGp_getEventManager().getMyStaffId("Bou", this, -1);
    if (staffId != -1) {
        mStaffId = staffId;
        int actIdx = dComIfGp_getEventManager().getMyActIdx(mStaffId, (char**)mCutNameList, 9, 0, 0);
        if ((this->*(mCutList[actIdx]))(mStaffId) != 0) {
            dComIfGp_getEventManager().cutEnd(mStaffId);
        }

        return true;
    } 

    return false;
}

void daNpc_Bou_c::action() {
    fopAc_ac_c* hit_actor = hitChk(&mCyl1, 0xffffffff);
    if (hit_actor != NULL && mCyl1.GetTgHitObj()->ChkAtType(AT_TYPE_THROW_OBJ)) {
        if (mType == TYPE_1) {
            daNpc_Jagar_c* jagar = (daNpc_Jagar_c*) mActorMngr[1].getActorP();
            if (jagar && jagar->getType() == TYPE_1 && jagar->chkCondition(1)) {
                jagar->setSurpriseMotion();
            }
        }

        mStagger.setParam(this, hit_actor, mCurAngle.y);
        setDamage(0, 10, 0);
        mDamageTimerStart = 0;
        mJntAnm.lookNone(1);
    }

    if (mStagger.checkRebirth()) {
        mStagger.initialize();
        mMode = 1;
    }

    if (field_0xfc4) {
        if (field_0xfd0 == field_0xfc4) {
            (this->*field_0xfd0)(NULL);
        } else {
            setAction(field_0xfc4);
        }
    }

    hit_actor = field_0xba0.getActorP();
    if (hit_actor != NULL) {
        abs( (s16)(fopAcM_searchPlayerAngleY(this) - mCurAngle.y) );
        switch (((daTag_Push_c*) hit_actor)->getId()) {
            case 7: {
                mEvtNo = 8;
                if (daNpc_Bou_Param_c::m.field_0x54 < current.pos.absXZ(daPy_getPlayerActorClass()->current.pos)) {
                    if (daPy_getPlayerActorClass()->checkHorseRide()) {
                        mEvtNo = 9;
                    }
                } else {
                    mEvtNo = 10;
                }
            }
        }
    }
}

void daNpc_Bou_c::beforeMove() {
    fopAcM_OffStatus(this, 0x8000000);
    if (checkHide()) {
        fopAcM_OnStatus(this, 0x8000000);
    }

    if (checkHide() || mNoDraw != 0) {
        attention_info.flags = 0;
    }
}

void daNpc_Bou_c::setAttnPos() {
    cXyz cStack_3c(-30.0f, 10.0f, 0.0f);
    mStagger.calc(0);
    f32 dVar8 = cM_s2rad(mCurAngle.y - field_0xd7e.y);
    J3DModel* model = mpMorf[0]->getModel();
    mJntAnm.setParam(this, model, &cStack_3c, getBackboneJointNo(), getNeckJointNo(),
        getHeadJointNo(), daNpc_Bou_Param_c::m.field_0x24, daNpc_Bou_Param_c::m.field_0x20,
        daNpc_Bou_Param_c::m.field_0x2c, daNpc_Bou_Param_c::m.field_0x28,
        daNpc_Bou_Param_c::m.field_0x34, daNpc_Bou_Param_c::m.field_0x30,
        daNpc_Bou_Param_c::m.field_0x3c, daNpc_Bou_Param_c::m.field_0x38,
        daNpc_Bou_Param_c::m.field_0x40, dVar8, NULL);
    mJntAnm.calcJntRad(0.2f, 1.0f, dVar8);
    setMtx();
    mDoMtx_stack_c::copy(mpMorf[0]->getModel()->getAnmMtx(getHeadJointNo()));
    mDoMtx_stack_c::multVec(&cStack_3c, &eyePos);
    mJntAnm.setEyeAngleX(eyePos, 1.0f, 0);
    mJntAnm.setEyeAngleY(eyePos, mCurAngle.y, 1, 1.0f, 0);
    cStack_3c.set(0.0f, 0.0f, 10.0f);
    cStack_3c.y = daNpc_Bou_Param_c::m.field_0x00;
    mDoMtx_stack_c::YrotS(mCurAngle.y);
    mDoMtx_stack_c::multVec(&cStack_3c, &cStack_3c);
    attention_info.position = current.pos + cStack_3c;
}

void daNpc_Bou_c::setCollision() {
    cXyz cStack_48;
    if (mHide == 0) {
        u32 tgType = -0x27040201;
        u32 tgSPrm = 0x1f;
        if (mTwilight) {
            tgType = 0;
            tgSPrm = 0;
        } else {
            if (mStagger.checkStagger()) {
                tgType = 0;
                tgSPrm = 0;
            }
        }

        mCyl1.SetCoSPrm(0x79);
        mCyl1.SetTgType(tgType);
        mCyl1.SetTgSPrm(tgSPrm);
        mCyl1.OnTgNoHitMark();
        cStack_48.set(0.0f, 0.0f, 0.0f);
        f32 cylHeight = mCylH;
        f32 cylRadius = mWallR;
        mDoMtx_stack_c::YrotS(mCurAngle.y);
        mDoMtx_stack_c::multVec(&cStack_48, &cStack_48);
        cStack_48 += current.pos;
        mCyl1.SetH(cylHeight);
        mCyl1.SetR(cylRadius);
        mCyl1.SetC(cStack_48);
        dComIfG_Ccsp()->Set(&mCyl1);
    }

    mCyl1.ClrCoHit();
    mCyl1.ClrTgHit();
}

int daNpc_Bou_c::drawDbgInfo() {
    return false;
}

void daNpc_Bou_c::changeAnm(int* param_0, int* param_1) {
    if (mType == 3) {
        switch (*param_0) {
            case 13: {
                *param_0 = 3;
                *param_1 = 5;
                break;
            }

            case 20: {
                *param_0 = 4;
                *param_1 = 5;
                break;
            }
        }
    }
}

int daNpc_Bou_c::selectAction() {
    field_0xfc4 = NULL;
    switch (mType) {
        case 1: {
            field_0xfc4 = &daNpc_Bou_c::talkwithJagar;
            break;
        }

        default: {
            field_0xfc4 = &daNpc_Bou_c::wait;
            break;
        }
    }

    return 1;
}

int daNpc_Bou_c::chkAction(actionFunc param_0) {
    return field_0xfd0 == param_0;
}

int daNpc_Bou_c::setAction(actionFunc param_0) {
    mMode = 3;
    if (field_0xfd0 != NULL) {
        (this->*field_0xfd0)(NULL);
    }

    mMode = 0;
    field_0xfd0 = param_0;
    if (field_0xfd0 != NULL) {
        (this->*field_0xfd0)(NULL);
    }

    return 1;
}

int daNpc_Bou_c::cutWildGoat(int arg) {
    fopAc_ac_c* actor_p = NULL;
    cXyz my_vec_0;
    csXyz my_svec;
    int ret_val = 0;
    int* pVal = NULL;
    int prm_val = -1;
    int msgNo_val = 0;
    int msgNo2_val = 0;
    pVal = dComIfGp_evmng_getMyIntegerP(arg, "prm");
    if (pVal) {
        prm_val = *pVal;
    }

    pVal = dComIfGp_evmng_getMyIntegerP(arg, "msgNo");
    if (pVal) {
        msgNo_val = *pVal;
    }

    pVal = dComIfGp_evmng_getMyIntegerP(arg, "msgNo2");
    if (pVal) {
        msgNo2_val = *pVal;
    }

    if (dComIfGp_getEventManager().getIsAddvance(arg)) {
        switch (prm_val) {
            case 0: {
                actor_p = mActorMngr[0].getActorP();
                JUT_ASSERT(0x803, NULL != actor_p);
                ((daCow_c *)actor_p)->setCrazyReady();
                daNpcT_offTmpBit(0x1C);
                initTalk(mFlowNodeNo, NULL);
                break;
            }

            case 2: {
                actor_p = mActorMngr[0].getActorP();
                JUT_ASSERT(0x811, NULL != actor_p);
                dComIfGp_getEvent()->setPt2(actor_p);
                Z2GetAudioMgr()->subBgmStart(0x1000011);
                break;
            }

            case 5: {
                actor_p = mActorMngr[0].getActorP();
                JUT_ASSERT(0x820, NULL != actor_p);
                ((daCow_c *)actor_p)->setCrazyGo();
                break;
            }

            case 6: {
                if (daNpcT_getPlayerInfoFromPlayerList(0xD, fopAcM_GetRoomNo(this), &my_vec_0, &my_svec)) {
                    daPy_getPlayerActorClass()->setPlayerPosAndAngle(&my_vec_0, my_svec.y, 0);
                }

                setPos(home.pos);
                setAngle(home.angle.y);
                break;
            }

            case 7: {
                daPy_getPlayerActorClass()->onGoatStopGame();
                break;
            }

            case 3:
            case 4:
            case 8:
            case 1: {
                break;
            }
        }
    }

    int int_vec[3] = {-1, -1, -1};
    switch (prm_val) {
        case 0: {
            mJntAnm.lookPlayer(0);
            if (mPlayerAngle == mCurAngle.y) {
                ret_val = 1;
            } else {
                step(mPlayerAngle, 10, 18, 15, 0);
            }

            break;
        }

        case 1:
        case 2:
        case 3: {
            mJntAnm.lookPlayer(0);
            if (prm_val == 2 || prm_val == 3) {
                if (prm_val == 3) {
                    actor_p = mActorMngr[0].getActorP();
                    JUT_ASSERT(0x853, NULL != actor_p);
                    mJntAnm.lookActor(actor_p, 0.0f, 0);
                }

                field_0xe26 = 0;
            }

            int_vec[0] = msgNo_val;
            int_vec[1] = msgNo2_val;
            if (talkProc(int_vec, 0, NULL, 0)) {
                if (msgNo_val == 0 && msgNo2_val == 0) {
                    if (mFlow.checkEndFlow()) {
                        ret_val = 1;
                    }
                } else {
                    ret_val = 1;
                }
            }

            if (prm_val == 3 && ret_val != 0) {
                actor_p = mActorMngr[0].getActorP();
                JUT_ASSERT(0x86A, NULL != actor_p);
                ((daCow_c *)actor_p)->setCrazyReadyDrawOn();
            }

            break;
        }

        case 4:
        case 5:
        case 6:
        case 7:
        case 8: {
            actor_p = mActorMngr[0].getActorP();
            JUT_ASSERT(0x875, NULL != actor_p);
            mJntAnm.lookActor(actor_p, 0.0f, 0);
            ret_val = 1;
        }
    }

    return ret_val;
}

int daNpc_Bou_c::cutWildGoatSuccess(int arg) {
    int ret_val = 0;
    int prm_val = -1;
    int* pVal = dComIfGp_evmng_getMyIntegerP(arg, "prm");
    if (pVal) {
        prm_val = *pVal;
    }

    if (dComIfGp_getEventManager().getIsAddvance(arg)) {
        switch (prm_val) {
            case 0: {
                mFaceMotionSeqMngr.setNo(10, -1.0f, 0, 0);
                mMotionSeqMngr.setNo(0, -1.0f, 0, 0);
                initTalk(mFlowNodeNo, NULL);
                field_0xff4 = 0;
                Z2GetAudioMgr()->subBgmStop();
                daNpcT_onTmpBit(0x1C);
                break;
            }

            case 1: {
                break;
            }
        }
    }

    switch (prm_val) {
        case 0: {
            mJntAnm.lookPlayer(0);
            if (mPlayerAngle == mCurAngle.y) {
                ret_val = 1;
            } else {
                step(mPlayerAngle, 10, 18, 15, 0);
            }

            break;
        }

        case 1: {
            mJntAnm.lookPlayer(0);
            if (talkProc(NULL, 0, NULL, 0) && mFlow.checkEndFlow()) {
                daNpcT_offTmpBit(0x1C);
                ret_val = 1;
            }
        }
    }

    return ret_val;
}

int daNpc_Bou_c::cutWildGoatFailure(int arg) {
    int ret_val = 0;
    int prm_val = -1;
    int* pVal = dComIfGp_evmng_getMyIntegerP(arg, "prm");
    if (pVal) {
        prm_val = *pVal;
    }

    if (dComIfGp_getEventManager().getIsAddvance(arg)) {
        switch (prm_val) {
            case 0: {
                Z2GetAudioMgr()->subBgmStop();
                daNpcT_onTmpBit(0x1C);
                break;
            }

            case 1: {
                mFaceMotionSeqMngr.setNo(10, -1.0f, 0, 0);
                mMotionSeqMngr.setNo(0, -1.0f, 0, 0);
                initTalk(mFlowNodeNo, NULL);
                field_0xff4 = 0;
                break;
            }

            case 2: {
                break;
            }
        }
    }

    switch (prm_val) {
        case 0: {
            ret_val = 1;
            break;
        }

        case 1: {
            mJntAnm.lookPlayer(0);
            if (mPlayerAngle == mCurAngle.y) {
                ret_val = 1;
            } else {
                step(mPlayerAngle, 10, 18, 15, 0);
            }

            break;
        }

        case 2: {
            mJntAnm.lookPlayer(0);
            if (talkProc(NULL, 0, NULL, 0) && mFlow.checkEndFlow()) {
                daNpcT_offTmpBit(0x1C);
                ret_val = 1;
            }
        }
    }

    return ret_val;
}

int daNpc_Bou_c::cutSpeakTo(int arg) {
    int ret_val = 0;
    int* pVal = NULL;
    int prm_val = -1;
    pVal = dComIfGp_evmng_getMyIntegerP(arg, "prm");
    if (pVal) {
        prm_val = *pVal;
    }

    if (dComIfGp_getEventManager().getIsAddvance(arg)) {
        switch (prm_val) {
            case 0: {
                mFaceMotionSeqMngr.setNo(10, -1.0f, 0, 0);
                mMotionSeqMngr.setNo(0, -1.0f, 0, 0);
                initTalk(0x190, NULL);
                break;
            }

            case 1:
            case 2: {
                break;
            }
        }
    }

    switch (prm_val) {
        case 0: {
            mJntAnm.lookPlayer(0);
            if (mPlayerAngle == mCurAngle.y) {
                ret_val = 1;
            } else {
                shape_angle.y = mPlayerAngle;
                step(mPlayerAngle, 10, 18, 15, 0);
            }

            break;
        }

        case 1: {
            mJntAnm.lookPlayer(0);
            if (talkProc(NULL, 0, NULL, 0) && mFlow.checkEndFlow()) {
                ret_val = 1;
            }

            break;
        }

        case 2: {
            mJntAnm.lookNone(0);
            ret_val = 1;
        }
    }

    return ret_val;
}

int daNpc_Bou_c::cutConversationWithBou(int arg) {
    fopAc_ac_c* actor = NULL;
    int ret_val = 0;
    int* pVal = NULL;
    int prm_val = -1;
    pVal = dComIfGp_evmng_getMyIntegerP(arg, "prm");
    if (pVal) {
        prm_val = *pVal;
    }

    if (dComIfGp_getEventManager().getIsAddvance(arg)) {
        switch (prm_val) {
            case 0: {
                mFaceMotionSeqMngr.setNo(10, -1.0f, 0, 0);
                mMotionSeqMngr.setNo(1, -1.0f, 0, 0);
                break;
            }
        }
    }

    switch (prm_val) {
        case 0: {
            actor = mActorMngr[1].getActorP();
            if (actor != NULL) {
                mJntAnm.lookActor(actor, -40.0f, 0);
            } else {
                mJntAnm.lookNone(0);
            }

            ret_val = 1;
            break;
        }
    }

    return ret_val;
}

int daNpc_Bou_c::cutConfidentialConversation(int arg) {
    fopAc_ac_c* actor = NULL;
    int ret_val = 0;
    int* pVal = NULL;
    int prm_val = -1;
    pVal = dComIfGp_evmng_getMyIntegerP(arg, "prm");
    if (pVal) {
        prm_val = *pVal;
    }

    if (dComIfGp_getEventManager().getIsAddvance(arg)) {
        switch (prm_val) {
            case 0: {
                mFaceMotionSeqMngr.setNo(10, -1.0f, 0, 0);
                mMotionSeqMngr.setNo(1, -1.0f, 0, 0);
                break;
            }

            case 1:
            case 2: {
                break;
            }
        }
    }

    switch (prm_val) {
        case 0:
        case 2: {
            actor = mActorMngr[1].getActorP();
            if (actor != NULL) {
                mJntAnm.lookActor(actor, -40.0f, 0);
            } else {
                mJntAnm.lookNone(0);
            }

            ret_val = 1;
            break;
        }

        case 1: {
            actor = mActorMngr[2].getActorP();
            if (actor != NULL) {
                mJntAnm.lookActor(actor, 0.0f, 0);
            } else {
                mJntAnm.lookNone(0);
            }

            ret_val = 1;
            break;
        }
    }

    return ret_val;
}

int daNpc_Bou_c::cutFindWolf(int arg) {
    int ret_val = 0;
    int* pVal = NULL;
    int prm_val = -1;
    int timer_val = 0;
    pVal = dComIfGp_evmng_getMyIntegerP(arg, "prm");
    if (pVal) {
        prm_val = *pVal;
    }

    pVal = dComIfGp_evmng_getMyIntegerP(arg, "timer");
    if (pVal) {
        timer_val = *pVal;
    }

    if (dComIfGp_getEventManager().getIsAddvance(arg)) {
        switch (prm_val) {
            case 0: {
                mFaceMotionSeqMngr.setNo(1, -1.0f, 0, 0);
                mMotionSeqMngr.setNo(3, -1.0f, 0, 0);
                dComIfGp_getEvent()->setPtT(this);
                initTalk(0xD4, NULL);
                break;
            }

            case 2: {
                mFaceMotionSeqMngr.setNo(10, -1.0f, 0, 0);
                mMotionSeqMngr.setNo(20, -1.0f, 0, 0);
                current.angle.y = mPlayerAngle + -0x8000;
                mEventTimer = timer_val;
                break;
            }

            case 3: {
                mFaceMotionSeqMngr.setNo(10, 0.0f, 0, 0);
                mMotionSeqMngr.setNo(1, 0.0f, 0, 0);
                setPos(home.pos);
                setAngle(home.angle.y);
                speedF = 0.0f;
                speed.setall(0.0f);
                mAcch.ClrWallNone();
                mHide = true;
                break;
            }
        }
    }

    switch (prm_val) {
        case 0: {
            mJntAnm.lookPlayer(0);
            if (mPlayerAngle == mCurAngle.y) {
                ret_val = 1;
            } else {
                step(mPlayerAngle, -1, -1, 15, 0);
            }

            break;
        }

        case 1: {
            mJntAnm.lookPlayer(0);
            if (talkProc(NULL, 0, NULL, 0) && mFlow.checkEndFlow()) {
                ret_val = 1;
            }

            break;
        }

        case 2: {
            mJntAnm.lookPlayer(0);
            cLib_chaseS(&shape_angle.y, current.angle.y, 0x800);
            mCurAngle.y = shape_angle.y;
            field_0xd7e.y = mCurAngle.y;
            cLib_chaseF(&speedF, daNpc_Bou_Param_c::m.field_0x8c, 0.5f);
            mAcch.SetWallNone();
            if (!cLib_calcTimer(&mEventTimer)) {
                ret_val = 1;
            }

            break;
        }

        case 3: {
            mJntAnm.lookNone(0);
            ret_val = 1;
            break;
        }
    }

    return ret_val;
}

int daNpc_Bou_c::cutMeetingAgain(int arg) {
    fopAc_ac_c* actor_p = field_0xba0.getActorP();
    cXyz work;
    cXyz temp;
    csXyz my_svec;
    int ret_val = 0;
    int* pVal = NULL;
    int prm_val = -1;
    int timer_val = 0;
    pVal = dComIfGp_evmng_getMyIntegerP(arg, "prm");
    if (pVal) {
        prm_val = *pVal;
    }

    pVal = dComIfGp_evmng_getMyIntegerP(arg, "timer");
    if (pVal) {
        timer_val = *pVal;
    }

    if (dComIfGp_getEventManager().getIsAddvance(arg)) {
        switch (prm_val) {
            case 0: {
                mFaceMotionSeqMngr.setNo(10, -1.0f, 0, 0);
                mMotionSeqMngr.setNo(0, -1.0f, 0, 0);
                mPlayerAngle = fopAcM_searchPlayerAngleY(this);
                break;
            }

            case 1: {
                daNpcT_offTmpBit(0xB);
                daNpcT_offTmpBit(0xC);
                initTalk(((daTag_Push_c*)actor_p)->getFlowNodeNo(), NULL);
                break;
            }

            case 2: {
                mFaceMotionSeqMngr.setNo(10, -1.0f, 0, 0);
                mMotionSeqMngr.setNo(0, -1.0f, 0, 0);
                mEventTimer = timer_val;
                break;
            }

            case 3: {
                if (daPy_getPlayerActorClass()->checkHorseRide()) {
                    setAngle(home.angle.y);
                    work.set(0.0f, 300.0f, 250.0f);
                    mDoMtx_stack_c::YrotS(mCurAngle.y);
                    mDoMtx_stack_c::multVec(&work, &work);
                    work += current.pos;
                    mGndChk.SetPos(&work);
                    work.y = dComIfG_Bgsp().GroundCross(&mGndChk);
                    JUT_ASSERT(0xab4, -G_CM3D_F_INF != work.y);
                    dComIfGp_evmng_setGoal(&work);

                    my_svec.y = mCurAngle.y + 0x5800;
                    temp.set(0.0f, 300.0f, -250.0f);
                    mDoMtx_stack_c::YrotS(my_svec.y);
                    mDoMtx_stack_c::multVec(&temp, &temp);
                    temp += work;
                    mGndChk.SetPos(&temp);
                    work.y = dComIfG_Bgsp().GroundCross(&mGndChk);
                    JUT_ASSERT(0xabf, -G_CM3D_F_INF != work.y);
                    daPy_getPlayerActorClass()->setPlayerPosAndAngle(&temp, my_svec.y, 0);
                } else {
                    my_svec.y = mPlayerAngle - home.angle.y;
                    if (my_svec.y < 0) {
                        if (abs(my_svec.y) < 0x4000) {
                            my_svec.y = 0;
                        } else {
                            my_svec.y = -0x8000;
                        }
                    }

                    my_svec.y += home.angle.y;
                    setAngle(my_svec.y);
                    work.set(0.0f, 300.0f, 300.0f);
                    mDoMtx_stack_c::YrotS(mCurAngle.y);
                    mDoMtx_stack_c::multVec(&work, &work);
                    work += current.pos;
                    mGndChk.SetPos(&work);
                    work.y = dComIfG_Bgsp().GroundCross(&mGndChk);
                    JUT_ASSERT(0xad8, -G_CM3D_F_INF != work.y);
                    daPy_getPlayerActorClass()->setPlayerPosAndAngle(&work, mCurAngle.y + 0x8000, 0);

                    work.set(0.0f, 300.0f, 150.0f);
                    mDoMtx_stack_c::YrotS(mCurAngle.y);
                    mDoMtx_stack_c::multVec(&work, &work);
                    work += current.pos;
                    mGndChk.SetPos(&work);
                    work.y = dComIfG_Bgsp().GroundCross(&mGndChk);
                    JUT_ASSERT(0xae2, -G_CM3D_F_INF != work.y);
                    dComIfGp_evmng_setGoal(&work);
                }

                break;
            }

            case 4: {
                initTalk(((daTag_Push_c*)actor_p)->getFlowNodeNo(), NULL);
                break;
            }

            case 5: {
                mJntAnm.lookNone(1);
                initTalk(((daTag_Push_c*)actor_p)->getFlowNodeNo(), NULL);
                break;
            }
        }
    }

    switch (prm_val) {
        case 0: {
            mJntAnm.lookPlayer(0);
            if (mPlayerAngle == mCurAngle.y) {
                ret_val = 1;
            } else {
                shape_angle.y = mPlayerAngle;
                step(mPlayerAngle, 10, 19, 15, 0);
            }

            break;
        }

        case 1: {
            mJntAnm.lookPlayer(0);
            if (talkProc(NULL, 0, NULL, 0) && mFlow.checkEndFlow()) {
                ret_val = 1;
            }

            break;
        }

        case 2: {
            mJntAnm.lookPlayer(0);
            if (!cLib_calcTimer(&mEventTimer)) {
                ret_val = 1;
            }

            break;
        }

        case 3: {
            ret_val = 1;
            break;
        }

        case 4:
        case 5: {
            mJntAnm.lookNone(0);
            if (prm_val == 4) {
                mJntAnm.lookPlayer(0);
            }

            if (talkProc(NULL, 0, NULL, 0) && mFlow.checkEndFlow()) {
                if (prm_val == 4) {
                    dStage_changeScene(24, 0.0f, 0, fopAcM_GetRoomNo(this), 0, dComIfG_play_c::getLayerNo(0));
                } else {
                    ret_val = 1;
                }
            }
        }
    }

    return ret_val;
}

int daNpc_Bou_c::wait(void* param_0) {
    fopAc_ac_c* actor_p = NULL;
    switch (mMode) {
        case 0:
        case 1: {
            if (!mStagger.checkStagger()) {
                mFaceMotionSeqMngr.setNo(10, -1.0f, 0, 0);
                mMotionSeqMngr.setNo(0, -1.0f, 0, 0);
                mMode = 2;
            }
        }
        // fallthrough intentional
        case 2: {
            if (daPy_getPlayerActorClass()->checkGoatStopGame()) {
                field_0xff4 = 1;
            } else if (field_0xff4) {
                mEvtNo = 4;
                if (daNpcT_chkEvtBit(0x235)) {
                    if (daNpcT_chkEvtBit(0x24b)) {
                        mEvtNo = 3;
                    }
                } else if (daNpcT_chkEvtBit(0xB2)) {
                    mEvtNo = 3;
                }

                field_0xe33 = 1;
            }

            if (!mStagger.checkStagger()) {
                if (field_0xff4) {
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
                        if (field_0xe34) {
                            if (mType == 2) {
                                if (step(home.angle.y, 10, 19, 15, 0)) {
                                    mMode = 1;
                                }
                            } else {
                                if (step(home.angle.y, 10, 18, 15, 0)) {
                                    mMode = 1;
                                }
                            }
                        } else {
                            setAngle(home.angle.y);
                            mMode = 1;
                        }

                        attention_info.flags = 0;
                    } else if (field_0xff4) {
                        attention_info.flags = 0;
                    } else if (!mTwilight) {
                        srchPlayerActor();
                    }
                }

                switch (mJntAnm.getMode()) {
                    case 0: {
                        if (field_0xff4) {
                            actor_p = mActorMngr[0].getActorP();
                            if (actor_p) {
                                mJntAnm.lookActor(actor_p, 0.0f, 0);
                            }
                        }
                    }
                }

                if (!field_0xe34) {
                    daNpcT_offTmpBit(0x1D);
                }
            }

            break;
        }

        case 3: {
            break;
        }
    }

    return 1;
}

int daNpc_Bou_c::talkwithJagar(void* param_0) {
    daNpc_Jagar_c* jagar = (daNpc_Jagar_c*) mActorMngr[1].getActorP();
    switch (mMode) {
        case 0:
        case 1: {
            if (!mStagger.checkStagger()) {
                mFaceMotionSeqMngr.setNo(10, -1.0f, 0, 0);
                mMotionSeqMngr.setNo(1, -1.0f, 0, 0);
                mMode = 2;
            }
        }
        // fallthrough intentional
        case 2: {
            if (jagar) {
                mJntAnm.lookActor(jagar, -40.0f, 0);
            } else {
                mJntAnm.lookNone(0);
            }
        }

        case 3: {
            break;
        }
    }

    return 1;
}

int daNpc_Bou_c::talk(void* param_0) {
    daNpc_Jagar_c* jagar_p = NULL;
    switch (mMode) {
        case 0:
        case 1: {
            if (!mStagger.checkStagger()) {
                if (mType == 1) {
                    jagar_p = (daNpc_Jagar_c*) mActorMngr[1].getActorP();
                    if (jagar_p != NULL && jagar_p->getType() == 1 && jagar_p->chkCondition(1)) {
                        jagar_p->setListenMotion(-10.0f);
                        jagar_p->onListen();
                    }

                    switch (field_0xfdc) {
                        case 4: {
                            initTalk(0xD6, NULL);
                            break;
                        }

                        default: {
                            initTalk(0xD5, NULL);
                        }
                    }
                } else {
                    daNpcT_offTmpBit(0x59);
                    daHorse_c* horse_p = dComIfGp_getHorseActor();
                    if (horse_p && !horse_p->checkHorseCallWait()) {
                        if (chkPointInArea(horse_p->current.pos, current.pos, daNpc_Bou_Param_c::m.field_0x90,
                                           daNpc_Bou_Param_c::m.field_0x94, daNpc_Bou_Param_c::m.field_0x98, 0)) {
                            daNpcT_onTmpBit(0x59);
                        }
                    }

                    initTalk(mFlowNodeNo, NULL);
                }

                mMode = 2;
            }
        }
        // fallthrough intentional
        case 2: {
            if (!mStagger.checkStagger()) {
                if (mTwilight || mPlayerAngle == mCurAngle.y || mType == 1) {
                    if (talkProc(NULL, 0, NULL, 0) && mFlow.checkEndFlow()) {
                        if (mType == 1) {
                            jagar_p = (daNpc_Jagar_c*) mActorMngr[1].getActorP();
                            if (jagar_p != NULL && jagar_p->getType() == 1) {
                                jagar_p->offListen();
                            }
                        }

                        mPlayerActorMngr.entry(daPy_getPlayerActorClass());
                        dComIfGp_event_reset();
                        mMode = 3;
                    }

                    if (mType == 1) {
                        fopAc_ac_c* actor_p = mActorMngr[1].getActorP();
                        if (actor_p != NULL) {
                            mJntAnm.lookActor(actor_p, -40.0f, 0);
                        } else {
                            mJntAnm.lookNone(0);
                        }
                    } else {
                        mJntAnm.lookPlayer(0);
                        if (mTwilight) {
                            mJntAnm.lookNone(0);
                        }
                    }
                } else {
                    mJntAnm.lookPlayer(0);
                    if (mType == 2) {
                        step(mPlayerAngle, 10, 19, 15, 0);
                    } else {
                        step(mPlayerAngle, 10, 18, 15, 0);
                    }
                }
            }

            break;
        }

        case 3: {
            break;
        }
    }

    return 0;
}

static int daNpc_Bou_Create(void* i_this) {
    return static_cast<daNpc_Bou_c*>(i_this)->create();
}

static int daNpc_Bou_Delete(void* i_this) {
    return static_cast<daNpc_Bou_c*>(i_this)->Delete();
}

static int daNpc_Bou_Execute(void* i_this) {
    return static_cast<daNpc_Bou_c*>(i_this)->Execute();
}

static int daNpc_Bou_Draw(void* i_this) {
    return static_cast<daNpc_Bou_c*>(i_this)->Draw();
}

static int daNpc_Bou_IsDelete(void* i_this) {
    return 1;
}

static actor_method_class daNpc_Bou_MethodTable = {
    (process_method_func)daNpc_Bou_Create,
    (process_method_func)daNpc_Bou_Delete,
    (process_method_func)daNpc_Bou_Execute,
    (process_method_func)daNpc_Bou_IsDelete,
    (process_method_func)daNpc_Bou_Draw,
};

actor_process_profile_definition g_profile_NPC_BOU = {
  fpcLy_CURRENT_e,        // mLayerID
  7,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_NPC_BOU,           // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(daNpc_Bou_c),    // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  324,                    // mPriority
  &daNpc_Bou_MethodTable, // sub_method
  0x00040107,             // mStatus
  fopAc_NPC_e,            // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};

static daNpc_Bou_Param_c l_HIO;
