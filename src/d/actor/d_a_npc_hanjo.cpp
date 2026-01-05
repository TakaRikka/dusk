/**
 * @file d_a_npc_hanjo.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_npc_hanjo.h"
#include "d/actor/d_a_npc_tk.h"
#include "d/actor/d_a_player.h"
#include "JSystem/J3DGraphBase/J3DMaterial.h"
#include "SSystem/SComponent/c_counter.h"
#include "d/actor/d_a_tag_evtarea.h"
#include "d/d_cc_uty.h"
#include "Z2AudioLib/Z2Instances.h"

static int l_bmdData[4][2] = {
    {14, 1}, {26, 2},
    {25, 2}, {3, 4},
};

static daNpcT_evtData_c l_evtList[11] = {
    {"", 0},
    {"NO_RESPONSE", 0},
    {"CONVERSATION_ABOUT_SARU01", 5},
    {"CONVERSATION_ABOUT_SARU02", 5},
    {"CONVERSATION_ABOUT_HONEYCOMB1", 2},
    {"CONVERSATION_ABOUT_HONEYCOMB2", 2},
    {"CONVERSATION_ABOUT_HONEYCOMB3", 2},
    {"CONVERSATION_ABOUT_HONEYCOMB4", 2},
    {"PURSUIT_BEE", 2},
    {"APPEAR_HAWKER", 3},
    {"DIVE", 3},
};



static char* l_resNameList[6] = {
    "",
    "Hanjo",
    "Hanjo1",
    "Hanjo2",
    "J_Tobi",
    "Sera",
};

static s8 l_loadResPtrn0[3] = {
    1,
    2,
    -1,
};

static s8 l_loadResPtrn1[5] = {
    1,
    2,
    3,
    4,
    -1,
};

static s8 l_loadResPtrn2[2] = {
    1, -1
};

static s8 l_loadResPtrn9[5] = {
    1,
    2,
    3,
    4,
    -1,
};

static s8* l_loadResPtrnList[5] = {
    l_loadResPtrn0, l_loadResPtrn1, l_loadResPtrn1,
    l_loadResPtrn2, l_loadResPtrn9,
};

static daNpcT_faceMotionAnmData_c l_faceMotionAnmData[10] = {
    {-1, 0, 0, 20, 2, 1, 1}, 
    {7, 0, 1, 20, 2, 1, 1}, 
    {7, 0, 2, 34, 0, 2, 0},
     {8, 2, 2, 35, 2, 2, 0},
     {10, 2, 2, 37, 2, 2, 0},
     {12, 2, 2, 39, 2, 2, 0}, 
     {5, 0, 3, 16, 0, 3, 0},
     {6, 2, 3, 17, 2, 3, 0}, 
     {9, 0, 2, 36, 0, 2, 0},
     { 11, 2, 2, 38, 2, 2, 0},
};

static int l_motionAnmData[161] = {
    11, 2, 1, 17, 0, 1, 65536, 22, 2, 2, 17, 0, 1, 65536, 16, 2, 2, 17, 0, 1, 65536,
    13, 2, 2, 17, 0, 1, 65536, 21, 2, 2, 31, 2, 2, 0,     20, 0, 2, 30, 0, 2, 0,
    14, 0, 2, 29, 0, 2, 0,     8,  2, 1, 17, 0, 1, 65536, 6,  0, 1, 17, 0, 1, 65536,
    17, 2, 2, 17, 0, 1, 65536, 18, 2, 2, 17, 0, 1, 65536, 19, 0, 2, 17, 0, 1, 65536,
    15, 0, 2, 17, 0, 1, 65536, 13, 2, 3, 17, 0, 1, 65536, 9,  2, 3, 17, 0, 1, 65536,
    12, 2, 3, 17, 0, 1, 65536, 7,  0, 3, 17, 0, 1, 65536, 8,  2, 3, 17, 0, 1, 65536,
    10, 0, 3, 17, 0, 1, 65536, 9,  0, 1, 17, 0, 1, 65536, 11, 0, 3, 17, 0, 1, 65536,
    10, 0, 1, 17, 0, 1, 65536, -1, 0, 0, 17, 0, 1, 65536};

static daNpcT_MotionSeqMngr_c::sequenceStepData_c l_faceMotionSequenceData[32] = {
    {1, -1, 1}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {2, -1, 1}, {4, 0, 0},  {-1, 0, 0}, {-1, 0, 0},
    {8, -1, 1}, {9, 0, 0},  {-1, 0, 0}, {-1, 0, 0}, {9, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {6, -1, 1}, {7, 0, 0},  {-1, 0, 0}, {-1, 0, 0}, {4, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {5, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {0, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}};

static daNpcT_MotionSeqMngr_c::sequenceStepData_c l_motionSequenceData[80] = {
    {0, -1, 0},  {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},  {21, -1, 1}, {0, 0, 0},   {-1, 0, 0},
    {-1, 0, 0},  {4, -1, 0},  {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},  {11, -1, 1}, {9, 0, 0},
    {-1, 0, 0},  {-1, 0, 0},  {12, -1, 1}, {2, 0, 0},   {-1, 0, 0},  {-1, 0, 0},  {1, -1, 0},
    {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},  {3, -1, 0},  {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},
    {10, -1, 0}, {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},  {16, -1, 1}, {17, 0, 0},  {-1, 0, 0},
    {-1, 0, 0},  {18, -1, 1}, {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},  {20, 4, 1},  {-1, 0, 0},
    {-1, 0, 0},  {-1, 0, 0},  {13, -1, 0}, {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},  {14, -1, 0},
    {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},  {15, -1, 0}, {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},
    {19, 4, 1},  {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},  {6, -1, 1},  {5, 0, 1},   {-1, 0, 0},
    {-1, 0, 0},  {6, -1, 1},  {5, 0, 1},   {4, 0, 0},   {-1, 0, 0},  {7, -1, 0},  {-1, 0, 0},
    {-1, 0, 0},  {-1, 0, 0},  {8, -1, 0},  {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},  {9, -1, 0},
    {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0}};

char* daNpc_Hanjo_c::mCutNameList[6] = {
    "",
    "CONVERSATION_ABOUT_SARU",
    "CONVERSATION",
    "PURSUIT_BEE",
    "APPEAR_HAWKER",
    "DIVE",
};

daNpc_Hanjo_c::cutFunc daNpc_Hanjo_c::mCutList[6] = {
    NULL,
    &daNpc_Hanjo_c::cutConversationAboutSaru,
    &daNpc_Hanjo_c::cutConversation,
    &daNpc_Hanjo_c::cutPursuitBee,
    &daNpc_Hanjo_c::cutAppearHawker,
    &daNpc_Hanjo_c::cutDive,
};

dCcD_SrcSph daNpc_Hanjo_c::mStoneCcDSph = {
    daNpc_Hanjo_c::mStoneCcDObjInfo, {}
};


daNpc_Hanjo_c::~daNpc_Hanjo_c() {
    OS_REPORT("|%06d:%x|daNpc_Hanjo_c -> デストラクト\n", g_Counter.mCounter0, this);
    if (mpMorf[0] != 0) {
        mpMorf[0]->stopZelAnime();
    }
    deleteRes(l_loadResPtrnList[mType], (char const**)l_resNameList);
}

daNpc_Hanjo_Param_c::Data const daNpc_Hanjo_Param_c::m = {
    160.0f, -3.0f, 1.0f, 400.0f, 
    255.0f, 160.0f, 35.0f, 30.0f, 0.0f, 0.0f, 10.0f, -10.0f, 30.0f, -10.0f, 45.0f,
    -45.0f, 0.6f, 12.0f, 3, 6, 5, 6, 110.0f, 500.0f, 300.0f, -300.0f, 60, 8, 0, 0, 
    4.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 24.0f, 16.0f, 4.0f,
     80.0f, 65.0f, 4.0f, 600.0f, 136.0f, 0x500, 90, 90, 2,
};

dCcD_SrcGObjInf const daNpc_Hanjo_c::mStoneCcDObjInfo = {
    {0, {{0, 0, 0}, {0, 0}, {0x79}}},
    {0, 0, 0, 0, {0}},
    {0, 0, 0, 0, {0}},
    {{0}},
};

int daNpc_Hanjo_c::create() {
    static int const heapSize[5] = {0x61B0, 0x41D0, 0x39D0, 0x39C0, 0};
    daNpcT_ct(this, daNpc_Hanjo_c, l_faceMotionAnmData,
                       (daNpcT_motionAnmData_c*)l_motionAnmData, l_faceMotionSequenceData, 4,
                       l_motionSequenceData, 4, l_evtList, l_resNameList);
    mType = getType();
    mFlowNodeNo = getFlowNodeNo();
    mTwilight = 0;
    int rv = loadRes(l_loadResPtrnList[mType], (const char**)l_resNameList);
    if (rv == cPhs_COMPLEATE_e) {
        OS_REPORT("\t(%s:%d) flowNo:%d, PathID:%02x<%08x> ", fopAcM_getProcNameString(this), (uint)mType,
                              mFlowNodeNo, getPathID(), fopAcM_GetParam(this));
        if (isDelete()) {
            OS_REPORT("===>isDelete:TRUE\n");
            return cPhs_ERROR_e;
        }
        OS_REPORT("\n");
        if (!fopAcM_entrySolidHeap(this, createHeapCallBack, heapSize[mType])) {
            OS_REPORT("===>isDelete:TRUE\n");
            return cPhs_ERROR_e;
        } 
        J3DModelData* modelData = mpMorf[0]->getModel()->getModelData();
        fopAcM_SetMtx(this, mpMorf[0]->getModel()->getBaseTRMtx());
        fopAcM_setCullSizeBox2(this, modelData);
        mSound.init(&current.pos, &eyePos, 3, 1);
        field_0x9c0.init(&mAcch, 60.0f, 0.0f);
        reset();
        mAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1,
                        &mAcchCir, fopAcM_GetSpeed_p(this), fopAcM_GetAngle_p(this),
                        fopAcM_GetShapeAngle_p(this));
        mCcStts.Init(daNpc_Hanjo_Param_c::m.field_0x10, 0, this);
        mCyl1.Set(mCcDCyl);
        mCyl1.SetStts(&mCcStts);
        mCyl1.SetTgHitCallback(tgHitCallBack);
        mCyl2.Set(mCcDCyl);
        mCyl2.SetStts(&mCcStts);
        mAcch.CrrPos(dComIfG_Bgsp());
        mGndChk = mAcch.m_gnd;
        mGroundH = mAcch.GetGroundH();
        if (mGroundH != -G_CM3D_F_INF) {
            setEnvTevColor();
            setRoomNo();
        }
        for (int i = 0; i < 4; i ++) {
            mStones[i].getSphP()->Set(mStoneCcDSph);
            mStones[i].getSphP()->SetStts(&mCcStts);
        }
        mCreating = 1;
        Execute();
        mCreating = 0;
    }
    return rv;
}

int daNpc_Hanjo_c::CreateHeap() {
    static int const bmdTypeList[2] = {2, 3};
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
    switch (mType) {
    case TYPE_0:
        for (int i = 0; i < 1; i++) {
            if (l_bmdData[bmdTypeList[i]][0] >= 0) {
                modelData = (J3DModelData*)dComIfG_getObjectRes(l_resNameList[l_bmdData[bmdTypeList[i]][1]],
                                                                l_bmdData[bmdTypeList[i]][0]);
            } else {
                modelData = 0;
            }
            if (modelData != NULL) {
                mModel1 = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
            } else {
                mModel1 = NULL;
            }
        }
        for (int i = 0; i < 4; i++) {
            if (l_bmdData[bmdTypeList[1]][0] >= 0) {
                modelData = (J3DModelData*)dComIfG_getObjectRes(l_resNameList[l_bmdData[1][1]], l_bmdData[1][0]);
            } else {
                modelData = NULL;
            }
            if (modelData != NULL) {
                model = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
            } else {
                model = NULL;
            }
            mStones[i].setModel(model);
        }
        break;
    case TYPE_1:
        for (int i = 1; i < 2; i++) {
            if (l_bmdData[bmdTypeList[i]][0] >= 0) {
                modelData = (J3DModelData*)dComIfG_getObjectRes(l_resNameList[l_bmdData[bmdTypeList[i]][1]],
                                                                l_bmdData[bmdTypeList[i]][0]);
            } else {
                modelData = NULL;
            }
            if (modelData != NULL) {
                mModel2 = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
            } else {
                mModel2 = NULL;
            }
        }
        break;
    }
    
    if (setFaceMotionAnm(1, false) && setMotionAnm(0, 0.0f, 0))
    {
        return 1;
    }
    return 0;
}

int daNpc_Hanjo_c::Delete() {
    OS_REPORT("|%06d:%x|daNpc_Hanjo_c -> Delete\n", g_Counter.mCounter0, this);
    fopAcM_GetID(this);
    this->~daNpc_Hanjo_c();
    return 1;
}

int daNpc_Hanjo_c::Execute() {
    return daNpcT_c::execute();
}

int daNpc_Hanjo_c::Draw() {
    if (mpMatAnm[0] != NULL) {
        J3DModelData* modelData = mpMorf[0]->getModel()->getModelData();
        modelData->getMaterialNodePointer(getEyeballMaterialNo())->setMaterialAnm(mpMatAnm[0]);
    }
    return daNpcT_c::draw(0, 0, mRealShadowSize, NULL, 100.0f, 0, 0, 0);
}

int daNpc_Hanjo_c::createHeapCallBack(fopAc_ac_c* i_this) {
    return static_cast<daNpc_Hanjo_c*>(i_this)->CreateHeap();
}

int daNpc_Hanjo_c::ctrlJointCallBack(J3DJoint* param_1, int param_2) {
    if (param_2 == 0) {
        J3DModel* model = j3dSys.getModel();
        daNpc_Hanjo_c* i_this = reinterpret_cast<daNpc_Hanjo_c*>(model->getUserArea());
        if (i_this != 0) {
            i_this->ctrlJoint(param_1, model);
        }
    }
    return 1;
}

u8 daNpc_Hanjo_c::getType() {
    switch ((u8)fopAcM_GetParam(this)) {
    case 0:
        return TYPE_0;
    case 1:
        return TYPE_1;
    case 2:
        return TYPE_2;
    case 3:
        return TYPE_3;
    default:
        return TYPE_4;
    }
}

int daNpc_Hanjo_c::isDelete() {
    switch (mType) {
    case TYPE_0:
        return FALSE;
    case TYPE_1: {
        bool rv = true;
        if (!daNpcT_chkEvtBit(0xd0) && !dComIfGs_isCollectShield(0)) {
            rv = false;
        }
        return rv;
    }
    case TYPE_2:
        return FALSE;
    case TYPE_3:
        return FALSE;
    default:
        return FALSE;
    }
}

void daNpc_Hanjo_c::reset() {
    csXyz acStack_20;
    int iVar1 = (u8*)&field_0x1728 - (u8*)&field_0x16e0;
    if (mpMatAnm[0] != NULL) {
        mpMatAnm[0]->initialize();
    }
    initialize();
    for (int i = 0; i < 4; i++) {
        field_0x10c8[i].initialize();
    }
    memset(&field_0x16e0, 0, iVar1);
    if (getPathID() != 0xff) {
        field_0x10e8.initialize();
        field_0x10e8.setPathInfo(getPathID(), fopAcM_GetRoomNo(this), 0);
    }
    acStack_20.setall(0);
    acStack_20.y = home.angle.y;
    switch(mType) {
    case TYPE_0:
        if (daNpcT_chkEvtBit(0xad)) {
            field_0x1718 = 1.0f;
        }
        break;
    case TYPE_1:
        if (daNpcT_chkEvtBit(0xcf)) {
            home.angle.y += 0x8000;
            acStack_20.y = home.angle.y;
        }
        break;
    case TYPE_2:
        break;
    }
    daNpcT_offTmpBit(0xac);
    if (daNpcT_chkEvtBit(0xb1)) {
        daNpcT_onTmpBit(0xac);
    }
    setAngle(acStack_20);
}

void daNpc_Hanjo_c::afterJntAnm(int param_1) {
    if (param_1 == 1) {
        mDoMtx_stack_c::YrotM(mStagger.getAngleZ(1));
        mDoMtx_stack_c::ZrotM(-mStagger.getAngleX(1));
    } else if (param_1 == 4) {
        mDoMtx_stack_c::YrotM(mStagger.getAngleZ(0));
        mDoMtx_stack_c::ZrotM(mStagger.getAngleX(0));
    }
}

void daNpc_Hanjo_c::setParam() {
    selectAction();
    srchActors();
    s16 sVar6 = daNpc_Hanjo_Param_c::m.field_0x48;
    s16 sVar5 = daNpc_Hanjo_Param_c::m.field_0x4a;
    s16 sVar4 = daNpc_Hanjo_Param_c::m.field_0x4c;
    s16 sVar1 = daNpc_Hanjo_Param_c::m.field_0x4e;
    if (field_0x1721 != 0) {
        sVar6 = 6;
        sVar5 = 6;
    } else if (mType == TYPE_2) {
        sVar6 = 4;
        sVar5 = 6;
    }
    daPy_py_c* player = daPy_getPlayerActorClass();
    if (player->checkHorseRide() != 0) {
        if (sVar6 < 7) {
            sVar6 = 7;
        }
        if (sVar4 < 9) {
            sVar4 = 9;
        }
    }
    attention_info.distances[0] = daNpcT_getDistTableIdx(sVar4, sVar1);
    attention_info.distances[1] = attention_info.distances[0];
    attention_info.distances[3] = daNpcT_getDistTableIdx(sVar6, sVar5);
    attention_info.flags = fopAc_AttnFlag_SPEAK_e | fopAc_AttnFlag_TALK_e;
    scale.set(daNpc_Hanjo_Param_c::m.field_0x08, daNpc_Hanjo_Param_c::m.field_0x08,
              daNpc_Hanjo_Param_c::m.field_0x08);
    mCcStts.SetWeight(daNpc_Hanjo_Param_c::m.field_0x10);
    mCylH = daNpc_Hanjo_Param_c::m.field_0x14;
    mWallR = daNpc_Hanjo_Param_c::m.field_0x1c;
    mAttnFovY = daNpc_Hanjo_Param_c::m.field_0x50;
    mAcchCir.SetWallR(mWallR);
    mAcchCir.SetWallH(daNpc_Hanjo_Param_c::m.field_0x18);
    mRealShadowSize = daNpc_Hanjo_Param_c::m.field_0x0c;
    mExpressionMorfFrame = daNpc_Hanjo_Param_c::m.field_0x6c;
    mMorfFrames = daNpc_Hanjo_Param_c::m.field_0x44;
    gravity = daNpc_Hanjo_Param_c::m.field_0x04;
}

BOOL daNpc_Hanjo_c::checkChangeEvt() {
    if (!chkAction(&daNpc_Hanjo_c::talk)) {
        mPreItemNo = 0;
        if (dComIfGp_event_chkTalkXY()) {
            if (dComIfGp_evmng_ChkPresentEnd()) {
                mEvtNo = 1;
                evtChange();
            }
            return true;
        }
        u16 sVar3 = 0;
        switch (mType) {
        case TYPE_0:
            if (field_0x1723 == 0 &&
                !daNpcT_chkEvtBit(0xac) && !daNpcT_chkEvtBit(0xab) &&
                !daNpcT_chkEvtBit(0xb7))
            {
                if (daNpcT_chkEvtBit(0x235)) {
                    if (!daNpcT_chkEvtBit(0x243) && !daNpcT_chkEvtBit(0x26f)) {
                        sVar3 = 7;
                    }
                } else {
                    if (daNpcT_chkEvtBit(0x18)) {
                        if (!daNpcT_chkEvtBit(0x243)) {
                            sVar3 = 5;
                        } else {
                            if (!daNpcT_chkEvtBit(0x8b)) {
                                sVar3 = 6;
                            }
                        }
                    } else {
                        if (!daNpcT_chkEvtBit(0x243)) {
                            sVar3 = 4;
                        }
                    }
                    
                }
                if (sVar3 != 0) {
                    mEvtNo = sVar3;
                    evtChange();
                    return true;
                }
            }
            break;
        case TYPE_3:
            if (daNpcT_chkEvtBit(0x24d)) {
                mEvtNo = 3;
            } else {
                mEvtNo = 2;
            }
            evtChange();
            return 1;
        }
    }
    return false;
}


void daNpc_Hanjo_c::setAfterTalkMotion() {
    int iVar2 = 7;
    switch(mFaceMotionSeqMngr.getNo()) {
    case 1:
        iVar2 = 5;
        break;
    case 2:
        iVar2 = 3;
        break;
    case 3:
        break;
    case 4:
        iVar2 = 6;
        break;
    }
    mFaceMotionSeqMngr.setNo(iVar2, -1.0f, 0, 0);
}

void daNpc_Hanjo_c::srchActors() {
    switch(mType) {
    case TYPE_0:
        if (field_0x10c8[0].getActorP() == NULL) {
            field_0x10c8[0].entry(getNearestActorP(0x1e8));
        }
        break;
    case TYPE_1:
        if (field_0x10c8[2].getActorP() == NULL) {
            field_0x10c8[2].entry(getNearestActorP(0x100));
        }
        if (field_0x10c8[3].getActorP() == NULL) {
            field_0x10c8[3].entry(getEvtAreaTagP(0x16, 0));
        }
        break;
    case TYPE_2:
        break;
    case TYPE_3:
        if (field_0x10c8[1].getActorP() == NULL) {
            field_0x10c8[1].entry(getNearestActorP(0x245));
        }
        break;
    }
}

BOOL daNpc_Hanjo_c::evtTalk() {
    if (chkAction(&daNpc_Hanjo_c::talk)) {
        (this->*field_0x16ec)(NULL);
    } else {
        setAction(&daNpc_Hanjo_c::talk);
    }
    return 1;
}

BOOL daNpc_Hanjo_c::evtCutProc() {
    int staffId = dComIfGp_getEventManager().getMyStaffId("Hanjo", this, -1);
    if (staffId != -1) {
        mStaffId = staffId;
        int actIdx =
            dComIfGp_getEventManager().getMyActIdx(mStaffId, (char**)mCutNameList, 6, 0, 0);
        if ((this->*(mCutList[actIdx]))(mStaffId) != 0) {
            dComIfGp_getEventManager().cutEnd(mStaffId);
        }
        return true;
    } 
    return false;
}

void daNpc_Hanjo_c::action() {
    fopAc_ac_c* hitActor = hitChk(&mCyl1, 0xffffffff);
    if (hitActor != NULL) {
        cCcD_Obj* hitObj = mCyl1.GetTgHitObj();
        if (hitObj->ChkAtType(AT_TYPE_THROW_OBJ) && (fopAcM_GetName(hitActor) != PROC_NPC_TK || mType != TYPE_1))
        {
            mStagger.setParam(this,
                                            hitActor, mCurAngle.y);
            setDamage(0, 7, 0);
            mDamageTimerStart = 0;
            mJntAnm.lookNone(1);
        }
    }
    if (mStagger.checkRebirth()) {
        mStagger.initialize();
        mMode = 1;
    }
    if (field_0x16e0) {
        if (field_0x16ec == field_0x16e0) {
            (this->*field_0x16ec)(NULL);
        } else {
            setAction(field_0x16e0);
        }
    }
    if ((mType != TYPE_0) && field_0x1721 != 0) {
        fopAcM_delete(this);
    }
}

void daNpc_Hanjo_c::beforeMove() {
    cXyz oldPos;
    cXyz pos;
    fopAcM_OffStatus(this, 0x8000000);
    if (checkHide()) {
        fopAcM_OnStatus(this, 0x8000000);
    }
    if (checkHide() || mNoDraw != 0) {
        attention_info.flags = 0;
    }
    for (int i = 0; i < 4; i++) {
        if (mStones[i].chkActive()) {
            if (cM3d_IsZero(mStones[i].getSpeedF()) == false) {
                oldPos = mStones[i].getOld();
                pos = mStones[i].getPos();
                mLinChk.Set(&oldPos, &pos, this);
                if (!dComIfG_Bgsp().LineCross(&mLinChk)) {
                    if (!mStones[i].getSphP()->ChkCoHit()) {
                        continue;
                    }
                }
                mStones[i].initialize();
            }
        }
    }

    if (field_0x1721 != 0) {
        dive();
    }
}

void daNpc_Hanjo_c::afterMoved() {
    cXyz cStack_1c;
    for (int i = 0; i < 4; i++) {
        if (mStones[i].chkActive()) {
            if (cM3d_IsZero(mStones[i].getSpeedF())) {
                mDoMtx_stack_c::copy(mpMorf[0]->getModel()->getAnmMtx(18));
                mDoMtx_stack_c::multVecZero((Vec*)&cStack_1c);
                mStones[i].setPos(cStack_1c);
            } else {
                mStones[i].posMoveF(this);
            }
        }
    }
}

static daNpc_Hanjo_Param_c l_HIO;

void daNpc_Hanjo_c::setAttnPos() {
    cXyz cStack_38(-10.0f, 10.0f, 0.0f);
    cXyz cStack_44;
    mStagger.calc(0);
    f32 dVar8 = cM_s2rad(mCurAngle.y - field_0xd7e.y);
    J3DModel* model = mpMorf[0]->getModel();
    mJntAnm.setParam(this, model, &cStack_38, getBackboneJointNo(), getNeckJointNo(),
        getHeadJointNo(), daNpc_Hanjo_Param_c::m.field_0x24, daNpc_Hanjo_Param_c::m.field_0x20,
        daNpc_Hanjo_Param_c::m.field_0x2c, daNpc_Hanjo_Param_c::m.field_0x28,
        daNpc_Hanjo_Param_c::m.field_0x34, daNpc_Hanjo_Param_c::m.field_0x30,
        daNpc_Hanjo_Param_c::m.field_0x3c, daNpc_Hanjo_Param_c::m.field_0x38,
        daNpc_Hanjo_Param_c::m.field_0x40, dVar8, NULL);
    mJntAnm.calcJntRad(0.2f, 1.0f, dVar8);
    setMtx();
    mDoMtx_stack_c::copy(mpMorf[0]->getModel()->getAnmMtx(getHeadJointNo()));
    mDoMtx_stack_c::multVec(&cStack_38, &eyePos);
    mJntAnm.setEyeAngleX(eyePos, 1.0f, 0);
    mJntAnm.setEyeAngleY(eyePos, mCurAngle.y, 1, 1.0f, 0);
    cStack_38.set(0.0f, 0.0f, 20.0f);
    cStack_38.y = daNpc_Hanjo_Param_c::m.field_0x00;
    if (field_0x1721 != 0) {
        cStack_38.set(0.0f, 170.0f, -10.0f);
    } else if (mMotionSeqMngr.getNo() == 2) {
        cStack_38.set(0.0f, 170.0f, -20.0f);
    }
    if (1.0f <= mpMorf[0]->getMorfRatio()) {
        field_0x16f8 = cStack_38;
    } else {
        cStack_44 = field_0x16f8 - cStack_38;
        cStack_44 *= 1.0f - mpMorf[0]->getMorfRatio();
        cStack_38 = cStack_38 + cStack_44;
    }
    mDoMtx_stack_c::YrotS(mCurAngle.y);
    mDoMtx_stack_c::multVec(&cStack_38, &cStack_38);
    attention_info.position = current.pos + cStack_38;
    static cXyz prtclScl(1.0f, 1.0f, 1.0f);
    setFootPos();
    if (3.0f < speedF) {
        setFootPrtcl(&prtclScl, 17.0f, 2.0f);
    }
}

void daNpc_Hanjo_c::setCollision() {
    cXyz cStack_4c;
    if (mHide == 0) {
        u32 tgType = 0xd8fbfdff;
        u32 tgSPrm = 0x1f;
        if (mTwilight != 0) {
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
        cStack_4c.set(0.0f, 0.0f, 0.0f);
        f32 cylHeight = mCylH;
        f32 cylRadius = mWallR;
        mDoMtx_stack_c::YrotS(mCurAngle.y);
        mDoMtx_stack_c::multVec(&cStack_4c, &cStack_4c);
        cStack_4c += current.pos;
        mCyl1.SetH(cylHeight);
        mCyl1.SetR(cylRadius);
        mCyl1.SetC(cStack_4c);
        dComIfG_Ccsp()->Set(&mCyl1);
        if (mType == 1 || mType == 2) {
            switch(mType) {
            case TYPE_1:
                cylRadius = 320.0f;
                cStack_4c.set(-140.0f, 40.0f, -240.0f);
                break;
            case TYPE_2:
                cylRadius = 320.0f;
                cStack_4c.set(0.0f, 40.0f, -200.0f);
                break;
            default:
                cylRadius = 0.0f;
                cStack_4c.set(0.0f, 0.0f, 0.0f);
                break;
            }
            if (cM3d_IsZero(cylRadius) == false) {
                mDoMtx_stack_c::YrotS(0);
                mDoMtx_stack_c::multVec(&cStack_4c, &cStack_4c);
                cStack_4c += current.pos;
                mCyl2.SetCoSPrm(0x19);
                mCyl2.OnCoNoCrrBit();
                mCyl2.SetH(cylHeight);
                mCyl2.SetR(cylRadius);
                mCyl2.SetC(cStack_4c);
                dComIfG_Ccsp()->Set(&mCyl2);
            }
        }
    }
    for (int i = 0; i < 4; i++) {
        if (mStones[i].chkActive()) {
            mStones[i].getSphP()->SetC(mStones[i].getPos());
            mStones[i].getSphP()->SetR(3.0f);
            dComIfG_Ccsp()->Set(mStones[i].getSphP());
            mStones[i].getSphP()->ClrCoHit();
        }
    }
    mCyl1.ClrCoHit();
    mCyl2.ClrCoHit();
    mCyl1.ClrTgHit();
}

int daNpc_Hanjo_c::drawDbgInfo() {
    return 0;
}

void daNpc_Hanjo_c::drawOtherMdl() {
    Mtx mtx;
    J3DModel* model = mpMorf[0]->getModel();
    for (int i = 0; i < 4; i++) {
        if (mStones[i].chkActive()) {
            g_env_light.setLightTevColorType_MAJI(mStones[i].getModelP(), &tevStr);
            mDoMtx_stack_c::copy(mpMorf[0]->getModel()->getAnmMtx(18));
            cMtx_copy(mDoMtx_stack_c::get(), mtx);
            mStones[i].setMtx(mtx);
            mDoExt_modelUpdateDL(mStones[i].getModelP());
        }
    }
    if (mModel2 != NULL && (mType == TYPE_1 || mType == TYPE_2)) {
        J3DAnmTransformKey* transformKey = getTrnsfrmKeyAnmP(
            l_resNameList[l_motionAnmData[100]], l_motionAnmData[98]);
        if (transformKey == mpMorf[0]->getAnm()) {
            g_env_light.setLightTevColorType_MAJI(mModel2,
                &tevStr);
            mDoMtx_stack_c::copy(model->getAnmMtx(18));
            mDoMtx_stack_c::XYZrotM(cM_deg2s(32.602f), cM_deg2s(23.207f),
                                    cM_deg2s(49.943f));
            mDoMtx_stack_c::transM(0.009f, -0.18f, -0.145f);
            cMtx_copy(mDoMtx_stack_c::get(), mtx);
            mModel2->setBaseTRMtx(mtx);
            mDoExt_modelUpdateDL(mModel2);
        }
    }
    if (mModel1 != NULL && cM3d_IsZero(field_0x1718) == false) {
        g_env_light.setLightTevColorType_MAJI(mModel1, &tevStr);
        mDoMtx_stack_c::copy(model->getAnmMtx(4));
        mDoMtx_stack_c::scaleM(field_0x1718, field_0x1718, field_0x1718);
        cMtx_copy(mDoMtx_stack_c::get(), mtx);
        mModel1->setBaseTRMtx(mtx);
        mDoExt_modelUpdateDL(mModel1);
        cLib_chaseF(&field_0x1718, 1.0f, 0.15f);
    }
}

int daNpc_Hanjo_c::selectAction() {
    field_0x16e0 = NULL;
    switch(mType) {
    case TYPE_0:
        field_0x16e0 = &daNpc_Hanjo_c::wait;
        if (!daNpcT_chkEvtBit(0xad) && !daNpcT_chkEvtBit(0xab) && !daNpcT_chkEvtBit(0xb7) &&
            (daNpcT_chkEvtBit(0x235) || daNpcT_chkEvtBit(0x18)))
        {
            field_0x16e0 = &daNpc_Hanjo_c::throwStone;
        }
        break;
    case TYPE_1:
        field_0x16e0 = &daNpc_Hanjo_c::takayose;
        break;
    default:
        field_0x16e0 = &daNpc_Hanjo_c::wait;
        break;
    }
   
    return 1;
}

int daNpc_Hanjo_c::chkAction(int (daNpc_Hanjo_c::*action)(void*)) {
    return field_0x16ec == action;
}

int daNpc_Hanjo_c::setAction(int (daNpc_Hanjo_c::*action)(void*)) {
    mMode = 3;
    if (field_0x16ec != NULL) {
        (this->*field_0x16ec)(NULL);
    }
    mMode = 0;
    field_0x16ec = action;
    if (field_0x16ec != NULL) {
        (this->*field_0x16ec)(NULL);
    }
    return 1;
}

void daNpc_Hanjo_c::initShoot(int param_1) {
    if (param_1 != 0) {
        mFaceMotionSeqMngr.setNo( 7, -1.0f, 0, 0);
        mMotionSeqMngr.setNo( 16, 0.0f, 1, 0);
        mJntAnm.lookNone(1);
    } else {
        mFaceMotionSeqMngr.setNo(7, -1.0f, 0, 0);
        mMotionSeqMngr.setNo(15, -1.0f, 0, 0);
        mJntAnm.lookNone(0);
    }
}

int daNpc_Hanjo_c::shoot(int param_1) {
    csXyz cStack_60;
    f32 dVar13 = daNpc_Hanjo_Param_c::m.field_0x98;
    s16 sVar9 = cM_deg2s(daNpc_Hanjo_Param_c::m.field_0x9c);
    if (mMotionSeqMngr.getNo() == 16 || mMotionSeqMngr.getNo() == 15) {
        switch(mMotionSeqMngr.getStepNo()) {
        case 0:
            dVar13 = mpMorf[0]->getFrame();
            if (36.0f < dVar13 && field_0x170c == -1) {
                int i = 0;
                for (; i < 4; i = i + 1) {
                    if (!mStones[i].chkActive())
                        break;
                }
                if (i < 4) {
                    mStones[i].initialize();
                    mStones[i].setTmr(0x708);
                    cStack_60 = current.angle;
                    cStack_60.y += daNpc_Hanjo_Param_c::m.field_0xac;
                    mStones[i].setAngle(cStack_60);
                    field_0x170c = i;
                }
            }
            break;
        case 1:
            if ((mpMorf[0]->checkFrame(21.0f)) && field_0x170c >= 0) {
                if (param_1 != 0) {
                    dVar13 = 110.0f;
                    sVar9 = cM_deg2s(78.0f);
                }
                f32 dVar12 = dVar13 * cM_ssin(sVar9);
                f32 dVar11 = dVar13 * cM_scos(sVar9);
                f32 fVar1 = ((cM_rnd() - 0.5f) / 0.5f) * 0.1f;
                if (param_1 != 0) {
                    fVar1 = 0.0;
                }
                mStones[field_0x170c].setVSpeed(dVar12 - (dVar12 * fVar1));
                mStones[field_0x170c].setSpeedF(dVar11 + (dVar11 * fVar1));
            }
            break;
        default:
            return 1;
        }
    }
    return 0;
}

void daNpc_Hanjo_c::initDive() {
    mFaceMotionSeqMngr.setNo(6, -1.0f, 0, 0);
    mMotionSeqMngr.setNo(18, -1.0f, 0, 0);
    speed.y = daNpc_Hanjo_Param_c::m.field_0x8c;
    field_0x1721 = 1;
}

void daNpc_Hanjo_c::dive() {
    f32 local_34;
    if (fopAcM_getWaterY(&current.pos, &local_34) != 0) {
        if (local_34 <= current.pos.y) {
            cLib_chaseF(&speedF, 0.0f, 0.1f);
        } else {
            if (field_0x1720 == 0) {
                mSound.startCreatureSound(Z2SE_CM_BODYFALL_WATER_M, 0, -1);
                speedF *= 0.5f;
                field_0x1720 = 1;
            } else if (field_0x171f == 0) {
                if (current.pos.y + mCylH < local_34) {
                    if (0.0f < speed.y) {
                        cLib_addCalc(&speed.y, 2.0f, 0.5f, 0.5f, 0.5f);
                    } else {
                        cLib_addCalc(&speed.y, 2.0f, 0.5f, 5.5f, 0.5f);
                    }
                }
                if (0.0f < speed.y && local_34 < current.pos.y + daNpc_Hanjo_Param_c::m.field_0xa8)
                {
                    field_0x171f = 1;
                    if (mMotionSeqMngr.getNo() == 18) {
                        mFaceMotionSeqMngr.setNo(7, -1.0f, 0, 0);
                        mMotionSeqMngr.setNo(19, -1.0f, 0, 0);
                    }
                    field_0x171c = 0;
                    speed.y = 0;
                }
            }
            if (field_0x171f != 0) {
                s16 sVar1 = field_0x171c;
                field_0x171c += 2100;
                f32 dVar7 = cM_ssin(field_0x171c & 0x7fff);
                if ((sVar1 < 0 && 0 <= field_0x171c) ||
                    (sVar1 >= 0 && field_0x171c < 0))
                {
                    cXyz cStack_30(current.pos);
                    cStack_30.y = local_34;
                    dComIfGp_particle_set(0xffffffff, 0x8363, &cStack_30, 0, 0);
                    dComIfGp_particle_set(0xffffffff, 0x8364, &cStack_30, 0, 0);
                    dComIfGp_particle_set(0xffffffff, 0x8365, &cStack_30, 0, 0);
                }
                current.pos.y = local_34 - daNpc_Hanjo_Param_c::m.field_0xa8;
                current.pos.y += dVar7 * -20.0f;
            }
            cLib_chaseF(&speedF, 0.0f, 0.3f);
            gravity = 0;
        }
    }
    if (mMotionSeqMngr.getNo() == 18) {
        if (field_0x1720 != 0 && field_0x171f == 0 && 0.0f < speed.y) {
            if (local_34 - current.pos.y < 260.0f && mMotionSeqMngr.getNo() == 18) {
                if (cM3d_IsZero(mpMorf[0]->getPlaySpeed())) {
                    mpMorf[0]->setPlaySpeed(1.0f);
                }
            }
        } else if ((mMotionSeqMngr.getNo() == 18) && mpMorf[0]->checkFrame(18.0f)) {
            mpMorf[0]->setPlaySpeed(0.0f);
        }
    }
}

int daNpc_Hanjo_c::lookround(s16 param_1) {
    if (field_0x171e != 0) {
        cLib_addCalcAngleS2(&current.angle.y, param_1, 4, 0x800);
        shape_angle.y = current.angle.y;
        mCurAngle.y = shape_angle.y;
        if (mMotionSeqMngr.getNo() != 14) {
            mMotionSeqMngr.setNo(14, -1.0f, 0, 0);
        } else {
            if (mMotionSeqMngr.checkEndSequence()) {
                s16 angleDiff = param_1 - mCurAngle.y;
                s16 absDiff = abs(angleDiff);
                if (absDiff <= cM_deg2s(4.0f)) {
                    mMotionSeqMngr.setNo(0, -1.0f, 0, 0);
                    field_0x171e = 0;
                } else {
                    mMotionSeqMngr.setNo(14, 0.0f, 1, 0);
                }
            }
        }
    } else {
        s16 angleDiff = param_1 - mCurAngle.y;
        s16 absDiff = abs(angleDiff);
        if (absDiff > cM_deg2s(90.0f)) {
            field_0x171e = 1;
        }
    }
    return 1;
}

int daNpc_Hanjo_c::commandToHawk() {
    daNPC_TK_c* pTkNpc = (daNPC_TK_c*)field_0x10c8[2].getActorP();
    int rv = 0;
    if (pTkNpc != NULL) {
        cXyz acStack_30(0.0f, 0.0f, 0.0f);
        int iVar1 = field_0x1722;
        if (iVar1) {
            acStack_30.setall(50.0f);
        }
        rv = 0;
        daTag_EvtArea_c* pEvtArea =
            (daTag_EvtArea_c*)field_0x10c8[3].getActorP();
        if (pEvtArea != NULL) {
            daPy_py_c* player = daPy_getPlayerActorClass();
            if (pEvtArea->chkPointInArea(player->current.pos, acStack_30)) {
                rv = 1;
            }
            if (rv != 0) {
                if (!pTkNpc->isAttackLink()) {
                    pTkNpc->setAttackLink();
                }
            } else {
                if (pTkNpc->isAttackLink()) {
                    pTkNpc->setBackHanjo();
                }
            }
        }
    }
    return rv == 1;
}

int daNpc_Hanjo_c::cutConversationAboutSaru(int staffIdx) {
    int rv = 0;
    int iVar4 = -1;
    int* piVar1 = (int*)dComIfGp_evmng_getMyIntegerP(staffIdx, "prm");
    if (piVar1 != NULL) {
        iVar4 = *piVar1;
    }
    if (dComIfGp_getEventManager().getIsAddvance(staffIdx)) { 
        switch(iVar4) {
        case 0:
            mPlayerAngle = fopAcM_searchPlayerAngleY(this);
            if (checkStep()) {
                mStepMode = 0;
            }
            break;
        case 1:
            break;
        }
    }
    switch (iVar4) {
    case 0:
        rv = 1;
        break;
    case 1:
        mJntAnm.lookPlayer(0);
        if (mPlayerAngle != mCurAngle.y) {
            step(mPlayerAngle, 7, 14, 15, 0);
        } else {
            rv = 1;
        }

    }
    return rv;
}

int daNpc_Hanjo_c::cutConversation(int param_1) {
    int rv = 0;
    int iVar9 = -1;
    int iVar6 = -1;
    int iVar7 = 0;
    int* piVar1 = dComIfGp_evmng_getMyIntegerP(param_1, "prm0");
    if (piVar1 != NULL) {
        iVar9 = *piVar1;
    }
    piVar1 = dComIfGp_evmng_getMyIntegerP(param_1, "prm1");
    if (piVar1 != NULL) {
        iVar6 = *piVar1;
    }
    piVar1 = dComIfGp_evmng_getMyIntegerP(param_1, "msgNo");
    if (piVar1 != NULL) {
        iVar7 = *piVar1;
    }
    if (dComIfGp_getEventManager().getIsAddvance(param_1)) {
        switch(iVar9) {
        case 0:
            mMotionSeqMngr.setNo(0, -1.0f, 0, 0);
            initTalk(mFlowNodeNo, NULL);
            mPlayerActorMngr.entry(daPy_getPlayerActorClass());
            break;
        case 1:
        case 2:
        case 3:
            if (iVar9 == 2) {
                initTalk(mFlowNodeNo, NULL);
                mPlayerActorMngr.entry(daPy_getPlayerActorClass());
            }
            if (iVar6 == 1) {
                fopAc_ac_c* actor_p =field_0x10c8[0].getActorP();
                JUT_ASSERT(2650, NULL != actor_p);
                dComIfGp_getEvent()->setPt2(actor_p);
            }
            break;
        }
    }
    int local_38[2] = {-1, -1};
    switch(iVar9) {
    case 0:
        mJntAnm.lookPlayer(0);
        if (mPlayerAngle == mCurAngle.y) {
            rv = 1;
        } else if (step(mPlayerAngle, 7, 0xe, 0xf, 0)) {
            mFaceMotionSeqMngr.setNo(7, -1.0f, 0, 0);
            mMotionSeqMngr.setNo(0, -1.0f, 0, 0);
        }
        break;
    case 1:
    case 2:
    case 3:
        mJntAnm.lookPlayer(0);
        if (iVar9 == 2) {
            mJntAnm.lookNone(0);
        }
        if ((iVar9 == 3 && mPlayerAngle != mCurAngle.y) &&
            step(mPlayerAngle, -1, -1, 15, 0))
        {
            mFaceMotionSeqMngr.setNo(7, -1.0f, 0, 0);
            mMotionSeqMngr.setNo(0, -1.0f, 0, 0);
        }
        local_38[0] = iVar7;
        if (talkProc(local_38, 0, NULL, 0)) {
            if (iVar7 == 0) {
                if (mFlow.checkEndFlow()) {
                    rv = 1;
                }
            } else {
                rv = 1;
            }
        }
        break;
    }

    return rv;
}

int daNpc_Hanjo_c::cutPursuitBee(int param_1) {
    cXyz cStack_6c;
    int rv = 0;
    int* local_88 = NULL;
    int local_8c = -1;
    local_88 = dComIfGp_evmng_getMyIntegerP(param_1, "prm");
    if (local_88 != NULL) {
        local_8c = *local_88;
    }
    if (dComIfGp_getEventManager().getIsAddvance(param_1)) {
        fopAc_ac_c* actor_p;
        switch(local_8c) {
        case 0:
            setAngle(home.angle.y);
            actor_p = field_0x10c8[0].getActorP();
            JUT_ASSERT(2748, NULL != actor_p);
            dComIfGp_getEvent()->setPt2(actor_p);
            for (int i = 0; i < 4; i++) {
                if (mStones[i].chkActive()) {
                    mStones[i].initialize();
                }
            }
            initShoot(1);
            field_0x170c = 0xffffffff;
            break;
        case 1:
            mEventTimer = 0;
            break;
        case 3:
            mFaceMotionSeqMngr.setNo(6, -1.0f, 0,
                                            0);
            mMotionSeqMngr.setNo(17, -1.0f,
                                            0, 0);
            mEventTimer = 1;
            mSound.startCreatureVoice(Z2SE_M077_HANJO_05, -1);
            break;
        case 4:
            field_0x1724 = 0;
            initDive();
            break;
        case 5:
            break;
        case 99:
            cStack_6c.set(1074.0f, -100.0f, -443.0f);
            setPos(cStack_6c);
            setAngle(home.angle.y);
            speedF = 0;
            speed.setall(0.0f);
            field_0x171f = 1;
            field_0x1720 = 1;
            field_0x1721 = 1;
            daNpcT_onEvtBit(0xad);
            daNpcT_onEvtBit(0xb1);
            field_0x1718 = 0.15f;
            mFlow.remove();
            break;
        }
    }
    switch(local_8c) {
    case 0:
        if (shoot(1)) {
            rv = 1;
        }
        break;
    case 1:
        if (mEventTimer != 0) {
            rv = 1;
        } else {
            for (int i = 0; i < 4; i++) {
                if (mStones[i].getSphP()->ChkCoHit()) {
                    fopAc_ac_c* hitActor = mStones[i].getSphP()->GetCoHitObj()->GetAc();
                    fopAc_ac_c* hitActor2 = dCc_GetAc(hitActor);
                    if (hitActor2 != NULL && fopAcM_GetName(hitActor2) == PROC_E_NEST) {
                        mEventTimer = daNpc_Hanjo_Param_c::m.field_0xb0;
                    }
                    mStones[i].initialize();
                }
            }
        }
        break;
    case 2:
        if (cLib_calcTimer(&mEventTimer) == 0) {
            rv = 1;
        }
        break;
    case 3:
        mJntAnm.lookNone(0);
        if (field_0x10e8.getDstPosH(current.pos, &cStack_6c, field_0x10e8.getNumPnts(), 4)) {
            rv = 1;
            break;
        }

        cLib_addCalcAngleS2(&current.angle.y, cLib_targetAngleY(&current.pos, &cStack_6c), 4, 0x800);
        shape_angle.y = current.angle.y;
        mCurAngle.y = current.angle.y;
        field_0xd7e.y = mCurAngle.y;
        cLib_chaseF(&speedF, daNpc_Hanjo_Param_c::m.field_0x90, 0.5f);
        if (mEventTimer != 0 &&
            mCyl1.ChkCoHit())
        {
            fopAc_ac_c* hitActor = mCyl1.GetCoHitAc();
            if (daPy_getPlayerActorClass() == hitActor) {
                s16 sVar11 = cM_deg2s(50.0f);
                s16 actorAngle = fopAcM_searchActorAngleY(this, daPy_getPlayerActorClass());
                actorAngle -= current.angle.y;
                if (actorAngle > 0) {
                    actorAngle = current.angle.y + 0x2000;
                } else {
                    actorAngle = current.angle.y - 0x2000;
                }
                daPy_getPlayerActorClass()->setThrowDamage(actorAngle, 40.0f * cM_scos(sVar11),
                                                           40.0f * cM_ssin(sVar11), 0, 1, 0);
                field_0x1724 = 1;
                mEventTimer = 0;
            }
        }
        break;
    case 4:
        if (field_0x171f != 0) {
            field_0x1718 = 0.15f;
            daNpcT_onEvtBit(0xad);
            daNpcT_onEvtBit(0xb1);
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

int daNpc_Hanjo_c::cutAppearHawker(int param_1) {
    cXyz cStack_a4;
    int rv = 0;
    int local_b4 = -1;
    int iVar7 = 0;
    int* piVar1 = dComIfGp_evmng_getMyIntegerP(param_1, "prm");
    fopAc_ac_c* actor_p;
    if (piVar1 != NULL) {
        local_b4 = *piVar1;
    }
    piVar1 = dComIfGp_evmng_getMyIntegerP(param_1, "timer");
    if (piVar1 != NULL) {
        iVar7 = *piVar1;
    }
    if (dComIfGp_getEventManager().getIsAddvance(param_1)) {
        char acStack_98 [100];
        switch (local_b4) {
        case 0:
            daNpcT_offTmpBit(0x27);
            mFaceMotionSeqMngr.setNo(7, 0.0f, 0, 0);
            mMotionSeqMngr.setNo(11, 0.0f, 1, 0);
            mJntAnm.lookNone(1);
            home.angle.y += 0x8000;
            setAngle(home.angle.y);
            initTalk(0xcf, NULL);
            strcpy(acStack_98, l_evtList[9].eventName);
            strcat(acStack_98, "@");
            dComIfGp_getEvent()->setSkipZev(this, acStack_98);
            dComIfGp_getEvent()->onSkipFade();
            dComIfGp_getVibration().StartShock( 9, 15, cXyz(0.0f, 1.0f, 0.0f));
            break;
        case 1:
            mEventTimer = iVar7;
            break;
        case 2:
            initTalk(0xcf, NULL);
            break;
        case 3:
            if (iVar7 < 1) {
                iVar7 = 1;
            }
            mEventTimer = iVar7;
            break;
        case 4:
            mMotionSeqMngr.setNo(13, -1.0f, 0, 0);
            break;
        case 5:
            mEventTimer = iVar7;
            break;
        case 6: {
            mEventTimer = iVar7;
            fopAc_ac_c* actor_p = field_0x10c8[2].getActorP();
            JUT_ASSERT(3005, NULL != actor_p);
            ((daNPC_TK_c*)actor_p)->setAttackLink();
            field_0x1722 = 1;
            break;
        }
        case 99:
            mMotionSeqMngr.setNo(13, 0.0, 1,
                                            0);
            dComIfGs_onSaveSwitch(0x2b);
            actor_p = field_0x10c8[2].getActorP();
            JUT_ASSERT(3014, NULL != actor_p);
            ((daNPC_TK_c*)actor_p)->setQuickHanjoHand();
            field_0x1722 = 1;
            daNpcT_onTmpBit(0x27);
            daNpcT_onEvtBit(0xcf);
            mFlow.remove();
            break;
        }
    }
    switch (local_b4) {
    case 0:
        if (talkProc(NULL, 0, NULL, 0) && mFlow.checkEndFlow()) {
            daNpcT_onTmpBit(0x27);
            rv = 1;
        }
        break;
    case 1:
        if (cLib_calcTimer(&mEventTimer) == 0) {
            rv = 1;
        }
        break;
    case 2:
        mJntAnm.lookCamera(0);
        if (talkProc(NULL, 0, NULL, 0) && mFlow.checkEndFlow()) {
            rv = 1;
        }
        break;
    case 3:
        if (mEventTimer != 0) {
            if (cLib_calcTimer(&mEventTimer) == 0) {
                mMotionSeqMngr.setNo(12, 8.0f, 0, 0);
                actor_p = field_0x10c8[2].getActorP();
                JUT_ASSERT(3068, NULL != actor_p);
                ((daNPC_TK_c*)actor_p)->setHanjoHand();
            }
        } else if (mMotionSeqMngr.checkEndSequence()) {
            rv = 1;
        } else if (mpMorf[0]->checkFrame(8.0f)) {
            Z2GetAudioMgr()->seStart(Z2SE_PLAY_REED_TOBIKUSA, &current.pos, 0, 0, 1.0f, 1.0f,
                                        -1.0f, -1.0f, 0);
        }
        break;
    case 4:
        mJntAnm.lookNone(0);
        actor_p = field_0x10c8[2].getActorP();
        JUT_ASSERT(3090, NULL != actor_p);
        if (((daNPC_TK_c*)actor_p)->isHanjoHand()) {
            rv = 1;
        }
        break;
    case 5:
        mJntAnm.lookCamera(0);
        if (cLib_calcTimer(&mEventTimer) == 0) {
            rv = 1;
        }
        break;
    case 6:
        if (cLib_calcTimer(&mEventTimer) == 0) {
            daNpcT_onEvtBit(0xcf);
            rv = 1;
        }
        break;
    case 99:
        rv = 1;
        break;
    }
    
    if ((local_b4 == 1 && rv == 1) || 1 < local_b4) {
        dKy_set_actcol_ratio(1.0f);
        dKy_set_bgcol_ratio(1.0f);
        dKy_set_fogcol_ratio(1.0f);
    }
    return rv;
}

int daNpc_Hanjo_c::cutDive(int param_1) {
    cXyz cStack_2c;
    csXyz cStack_34;
    int rv = 0;
    int iVar5 = -1;
    int uVar4 = 0;
    int* piVar1 = dComIfGp_evmng_getMyIntegerP(param_1, "prm");
    if (piVar1 != NULL) {
        iVar5 = *piVar1;
    }
    int* puVar2 = dComIfGp_evmng_getMyIntegerP(param_1, "timer");
    if (puVar2 != NULL) {
        uVar4 = *puVar2;
    }
    if (dComIfGp_getEventManager().getIsAddvance(param_1)) {
        switch(iVar5) {
        case 0:
            cStack_2c.set(1400.0f, 600.0f, 180.0f);
            if (daPy_getPlayerActorClass()->checkPlayerFly()) {
                cStack_2c.y += 200.0f;
            }
            cStack_34.y = cLib_targetAngleY(&cStack_2c, &current.pos);
            daPy_getPlayerActorClass()->setPlayerPosAndAngle(&cStack_2c, cStack_34.y, 0);
            dComIfGp_evmng_setGoal(&cStack_2c);
            initTalk(0xd0, NULL);
            mEventTimer = uVar4;
            break;
        case 1: {
            fopAc_ac_c* actor_p = field_0x10c8[2].getActorP();
            if (actor_p != NULL){
                ((daNPC_TK_c*)actor_p)->setAway();
            }
            break;
        }
        case 2:
            mFaceMotionSeqMngr.setNo(6, -1.0f, 0, 0);
            mMotionSeqMngr.setNo(17, -1.0f, 0,
                                            0);
            mJntAnm.lookNone(1);
            mSound.startCreatureVoice(Z2SE_M077_HANJO_05, -1);
            break;
        case 3:
            initDive();
            break;
        }
    }
    switch(iVar5) {
    case 0:
        if (cLib_calcTimer(&mEventTimer) == 0) {
            mJntAnm.lookPlayer(0);
            if (mPlayerAngle == mCurAngle.y) {
                rv = 1;
            } else if (mType == TYPE_1) {
                step(mPlayerAngle, 7, 10, 0xf, 0);
            } else {
                step(mPlayerAngle, 7, 0xe, 0xf, 0);
            }
        }
        break;
    case 1:
        if (talkProc(NULL, 0, NULL, 0) && mFlow.checkEndFlow()) {
            rv = 1;
        }
        break;
    case 2: {
        cStack_2c.set(0.0f, 0.0f, 50.0f);
        mDoMtx_stack_c::YrotS(mCurAngle.y);
        mDoMtx_stack_c::multVec(&cStack_2c, &cStack_2c);
        cStack_2c += current.pos;
        cStack_2c.y = attention_info.position.y;
        mGndChk.SetPos(&cStack_2c);
        f32 dVar7 = dComIfG_Bgsp().GroundCross(&mGndChk);
        if ((dVar7 - mGroundH) < -daNpc_Hanjo_Param_c::m.field_0x18) {
            rv = 1;
        }
        if (mType == TYPE_1) {
            cStack_34.y = home.angle.y - 0x8000;
            if (daNpcT_chkEvtBit(0xcf)) {
                cStack_34.y = home.angle.y + 0x4000;
            } else {
                cStack_34.y = home.angle.y - 0x4000;
            }
        } else {
            cStack_34.y = home.angle.y;
        }
        cLib_addCalcAngleS2(&current.angle.y, cStack_34.y, 4, 0x800);
        shape_angle.y = current.angle.y;
        mCurAngle.y = current.angle.y;
        cLib_chaseF(&speedF, daNpc_Hanjo_Param_c::m.field_0x90, 1.0f);
        break;
    }
    case 3:
        if (field_0x1720) {
            if (mType == TYPE_1) {
                daNpcT_onEvtBit(0xd0);
            }
            rv = 1;
        }
        break;
    }
    return rv;
}

int daNpc_Hanjo_c::wait(void* param_0) {
    switch(mMode) {
    case 0:
    case 1:
        if (!mStagger.checkStagger()) {
            switch (mType) {
            case TYPE_0: 
                if (field_0x1721 != 0) {
                    mFaceMotionSeqMngr.setNo(7, -1.0f, 0, 0);
                    mMotionSeqMngr.setNo(19, -1.0f, 0, 0);
                } else {
                    mFaceMotionSeqMngr.setNo(7, -1.0f, 0, 0);
                    if (mPlayerActorMngr.getActorP() != NULL || daNpcT_chkEvtBit(0xab) ||
                        daNpcT_chkEvtBit(0xb7))
                    {
                        mMotionSeqMngr.setNo(0, -1.0f, 0, 0);
                    } else {
                        mMotionSeqMngr.setNo(2, -1.0f, 0, 0);
                    }
                }
                break;
            default:
                mFaceMotionSeqMngr.setNo(7, -1.0f, 0, 0);
                mMotionSeqMngr.setNo(0, -1.0f, 0, 0);
                break;
            }
            mMode = 2;
        }
    case 2:
        switch(mType) {
        case TYPE_0:
            if (chkPointInArea(daPy_getPlayerActorClass()->current.pos, current.pos,
                                                daNpc_Hanjo_Param_c::m.field_0x54, 150.0f,
                                                -150.0f, 0)) {
                if (daPy_getPlayerActorClass()->checkBeeChildDrink()) {
                    mSpeakEvent = 1;
                    field_0x1723 = 1;
                }
            }
            break;
        case TYPE_1:
            break;
        case TYPE_2:
            if (daPy_py_c::checkNowWolf() && mCyl2.ChkCoHit()) {
                if (!daPy_getPlayerActorClass()->checkPlayerFly() ||
                    daPy_getPlayerActorClass()->checkAutoJump() ||
                    daPy_getPlayerActorClass()->checkWolfTriggerJump())
                {
                    mEvtNo = 10;
                }
            }
        }

        if (!mStagger.checkStagger()) {
            if (mType == TYPE_0) {
                if ((daNpcT_chkEvtBit(0xab) ||
                    daNpcT_chkEvtBit(0xb7)) && field_0x1721 == 0 && mMotionSeqMngr.getNo() == 2)
                {
                    mMotionSeqMngr.setNo(0, -1.0, 0, 0);
                }
            } else if (mType == TYPE_3) {
                mPlayerActorMngr.remove();
            }
            if (mPlayerActorMngr.getActorP() != NULL) {
                mJntAnm.lookPlayer(0);
                if (!chkActorInSight(mPlayerActorMngr.getActorP(), mAttnFovY,
                                                    mCurAngle.y)) {
                    mJntAnm.lookNone(0);
                }
                if (!srchPlayerActor() && home.angle.y == mCurAngle.y) {
                    mMode = 1;
                }
            } else {
                mJntAnm.lookNone(0);
                if (home.angle.y != mCurAngle.y) {
                    if (field_0xe34 != 0) {
                        if (field_0x1721 != 0) {
                            if (step(home.angle.y, -1, -1, 15, 0)) {
                                mMode = 1;
                            }
                        } else {
                            if (step(home.angle.y, 7, 14, 15, 0)) {
                                mMode = 1;
                            }
                        }
                    } else {
                        setAngle(home.angle.y);
                        mMode = 1;
                    }
                    attention_info.flags = 0;
                } else {
                    srchPlayerActor();
                }
            }
            switch(mJntAnm.getMode()) {
            case 1:
                switch (mType) {
                case TYPE_0:
                    if (field_0x1721 == 0 && mMotionSeqMngr.getNo() == 2) {
                        mMotionSeqMngr.setNo(0, -1.0f, 0, 0);
                    }
                    break;
                }
                break;
            case 0:
                switch (mType) {
                case TYPE_3:
                    fopAc_ac_c* actor_p = field_0x10c8[1].getActorP();
                    if (actor_p != NULL) {
                        mJntAnm.lookActor(actor_p, -40.0f, 0);
                    }
                    break;
                }
                break;
            }
        }
    case 3:
    default:
        break;
    }
    return 1;
}

int daNpc_Hanjo_c::throwStone(void* param_0) {
    int sVar4 = daNpc_Hanjo_Param_c::m.field_0xae;
    switch(mMode) {
    case 0:
    case 1:
        if (!mStagger.checkStagger()) {
            if (daNpcT_chkEvtBit(0x8b)) {
                mFaceMotionSeqMngr.setNo(7, -1.0f, 0, 0);
                mMotionSeqMngr.setNo(0, -1.0f, 0, 0);
            } else {
                mFaceMotionSeqMngr.setNo(7, -1.0f, 0, 0);
                mMotionSeqMngr.setNo(2, -1.0f, 0, 0);
                field_0x1704 = cLib_getRndValue(sVar4 * 0.5f, sVar4 * 1.5f);
            }
            mMode = 2;
        }
    case 2:
        if (daNpcT_chkEvtBit(0x8b) && !daNpcT_chkEvtBit(0xad) && !daNpcT_chkEvtBit(0xb7)) {
            if (chkPointInArea(daPy_getPlayerActorClass()->current.pos, current.pos,
                               daNpc_Hanjo_Param_c::m.field_0xa4, 1000.0f, -1000.0f, 0) == false)
            {
                mEvtNo = 8;
            }
        }
        if (!mStagger.checkStagger()) {
            if (daNpcT_chkEvtBit(0x8b)) {
                mJntAnm.lookPlayer(0);
                lookround(fopAcM_searchPlayerAngleY(this));
            } else {
                mJntAnm.lookNone(0);
                if (home.angle.y != mCurAngle.y) {
                    if (field_0xe34 != 0) {
                        if (step(home.angle.y, 7, 14, 15, 0)) {
                            mMode = 1;
                        }
                    } else {
                        setAngle(home.angle.y);
                        mMode = 1;
                    }
                    attention_info.flags = 0;
                } else {
                    if (field_0x1704 != 0) {
                        if (cLib_calcTimer(&field_0x1704) == 0) {
                            field_0x1708 = (g_Counter.mCounter0 & 1) + 1;
                            initShoot(0);
                            field_0x170c = 0xffffffff;
                        }
                    } else {
                        if (shoot(0)) {
                            if (field_0x1708 != 0) {
                                mMotionSeqMngr.setNo(15, -1.0f, 1, 0);
                                cLib_calcTimer(&field_0x1708);
                                field_0x170c = 0xffffffff;
                            } else {
                                mMode = 1;
                            }
                        }
                    }
                }
                
            }
        }
        break;
    case 3:
    default:
        break;
    }
    
    return 1;
}

int daNpc_Hanjo_c::takayose(void* param_0) {
    f32 dVar8 = daNpc_Hanjo_Param_c::m.field_0xae;
    cXyz cStack_50;
    cXyz cStack_5c;
    switch (mMode) {
    case 0:
    case 1:
        if (!mStagger.checkStagger()) {
            if (mType == 1) {
                if (daNpcT_chkEvtBit(0xcf)) {
                    mFaceMotionSeqMngr.setNo(0, -1.0f, 0, 0);
                    mMotionSeqMngr.setNo(13, -1.0f, 0, 0);
                    field_0x1714 = 0;
                } else {
                    if (field_0x1714 != 0) {
                        mFaceMotionSeqMngr.setNo(0, -1.0f, 0, 0);
                        mMotionSeqMngr.setNo(0, -1.0f, 0, 0);
                    } else {
                        mFaceMotionSeqMngr.setNo(7, -1.0f, 0, 0);
                        mMotionSeqMngr.setNo(2, -1.0f, 0, 0);
                    }
                }
            } else {
                mFaceMotionSeqMngr.setNo(0, -1.0f, 0, 0);
                mMotionSeqMngr.setNo(0, -1.0f, 0, 0);
            }
            field_0x1710 = cLib_getRndValue(dVar8 * 0.5f, dVar8 * 1.5f);
            mMode = 2;
        }
    case 2: {
        field_0xe2d = 0;
        if (dComIfGp_event_runCheck() != FALSE) {
            if (strcmp("toal_enter01", dComIfGp_getEventManager().getRunEventName()) == 0) {
                dComIfGp_event_setTalkPartner(this);
                field_0xe2d = 1;
            }
        }
        if (mType == TYPE_1 && !daNpcT_chkEvtBit(0xcf) && field_0x10c8[2].getActorP() != NULL) {
            if (!daPy_getPlayerActorClass()->checkPlayerFly()) {
                daTag_EvtArea_c* evtArea = (daTag_EvtArea_c*)field_0x10c8[3].getActorP();
                if (evtArea != NULL) {
                    if (evtArea->chkPointInArea(daPy_getPlayerActorClass()->current.pos)) {
                        mEvtNo = 9;
                    }
                }
            }
        }
        if (daPy_py_c::checkNowWolf() && mCyl2.ChkCoHit() != NULL) {
            if (!daPy_getPlayerActorClass()->checkPlayerFly() ||
                daPy_getPlayerActorClass()->checkAutoJump() ||
                daPy_getPlayerActorClass()->checkWolfTriggerJump())
            {
                mEvtNo = 10;
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
                        if (step(home.angle.y, 7, 14, 15, 0)) {
                            mMode = 1;
                        }
                    } else {
                        setAngle(home.angle.y);
                        mMode = 1;
                    }
                    attention_info.flags = 0;
                } else {
                    if (mType == 1) {
                        if (daNpcT_chkEvtBit(0xcf)) {
                            u8 dVar1 = field_0x1722;
                            fopAc_ac_c* actor_p = field_0x10c8[2].getActorP();
                            if (actor_p != NULL) {
                                field_0x1722 = commandToHawk();
                                if (field_0x1722 != 0) {
                                    mJntAnm.lookPlayer(0);
                                    if (mMotionSeqMngr.getNo() != 11) {
                                        mFaceMotionSeqMngr.setNo(7, -1.0f, 0, 0);
                                        mMotionSeqMngr.setNo(11, -1.0f, 0, 0);
                                    }
                                } else {
                                    if (dVar1 != 0) {
                                        field_0x1714 = 0;
                                        mMode = 1;
                                    } else {
                                        if (field_0x1714 != 0) {
                                            mJntAnm.lookNone(0);
                                            if (mMotionSeqMngr.getStepNo() > 0) {
                                                field_0x1714++;
                                                field_0x1714 &= 1;
                                                mMode = 1;
                                            }
                                        } else {
                                            mJntAnm.lookActor(actor_p, 0.0, 0);
                                            if (((daNPC_TK_c*)actor_p)->isHanjoHand() &&
                                                cLib_calcTimer(&field_0x1710) == 0)
                                            {
                                                mMotionSeqMngr.setNo(9, -1.0, 0, 0);
                                                field_0x1714 = field_0x1714 + 1;
                                                field_0x1714 &= 1;
                                            }
                                        }
                                    }
                                }
                            }
                        } else {
                            if (!dComIfGp_event_runCheck() &&
                                cLib_calcTimer(&field_0x1710) == 0)
                            {
                                field_0x1714++;
                                field_0x1714 &= 1;
                                mMode = 1;
                            }
                        }
                    }
                    srchPlayerActor();
                }
            }
        }
        break;
    }
    case 3:
    default:
        break;
    }
    return 1;
}

int daNpc_Hanjo_c::talk(void* param_0) {
    switch(mMode) {
    case 0:
    case 1:
        if (!mStagger.checkStagger()) {
            if (mType == TYPE_0 && mMotionSeqMngr.getNo() == 2) {
                mMotionSeqMngr.setNo(0, -1.0f, 0, 0);
            }
            if (field_0x1723 != 0) {
                initTalk(44, NULL);
                field_0x1723 = 0;
            } else {
                initTalk(mFlowNodeNo, NULL);
            }
            mMode = 2;
        }
    case 2:
        if (!mStagger.checkStagger()) {
            if (mTwilight != 0 || mPlayerAngle == mCurAngle.y) {
                if (talkProc(NULL, 0, NULL, 0) && mFlow.checkEndFlow()) {
                    mPlayerActorMngr.entry(daPy_getPlayerActorClass());
                    dComIfGp_event_reset();
                    mMode = 3;
                }
                mJntAnm.lookPlayer(0);
                if (mTwilight != 0) {
                    mJntAnm.lookNone(0);
                }
            } else {
                mJntAnm.lookPlayer(0);
                if (field_0x1721 != 0) {
                    step(mPlayerAngle, -1, -1, 15, 0);
                } else {
                    step(mPlayerAngle, 7, 14, 15, 0);
                }
            }
        }
        break;
    case 3:
    default:
        break;
    }
    return 0;
}

static int daNpc_Hanjo_Create(void* i_this) {
    return static_cast<daNpc_Hanjo_c*>(i_this)->create();
}

static int daNpc_Hanjo_Delete(void* i_this) {
    return static_cast<daNpc_Hanjo_c*>(i_this)->Delete();
}

static int daNpc_Hanjo_Execute(void* i_this) {
    return static_cast<daNpc_Hanjo_c*>(i_this)->Execute();
}

static int daNpc_Hanjo_Draw(void* i_this) {
    return static_cast<daNpc_Hanjo_c*>(i_this)->Draw();
}

static int daNpc_Hanjo_IsDelete(void* i_this) {
    return 1;
}

AUDIO_INSTANCES;

static actor_method_class daNpc_Hanjo_MethodTable = {
    (process_method_func)daNpc_Hanjo_Create,
    (process_method_func)daNpc_Hanjo_Delete,
    (process_method_func)daNpc_Hanjo_Execute,
    (process_method_func)daNpc_Hanjo_IsDelete,
    (process_method_func)daNpc_Hanjo_Draw,
};

actor_process_profile_definition g_profile_NPC_HANJO = {
  fpcLy_CURRENT_e,            // mLayerID
  7,                          // mListID
  fpcPi_CURRENT_e,            // mListPrio
  PROC_NPC_HANJO,             // mProcName
  &g_fpcLf_Method.base,      // sub_method
  sizeof(daNpc_Hanjo_c),      // mSize
  0,                          // mSizeOther
  0,                          // mParameters
  &g_fopAc_Method.base,       // sub_method
  341,                        // mPriority
  &daNpc_Hanjo_MethodTable,   // sub_method
  0x00040107,                 // mStatus
  fopAc_NPC_e,                // mActorType
  fopAc_CULLBOX_CUSTOM_e,     // cullType
};
