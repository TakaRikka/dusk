/**
 * @file d_a_npc_maro.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_npc_maro.h"

#include "d/actor/d_a_horse.h"
#include "d/actor/d_a_npc_len.h"
#include "d/actor/d_a_obj_itamato.h"
#include "d/actor/d_a_tag_evtarea.h"
#include "d/actor/d_a_tag_push.h"
#include "d/actor/d_a_tag_shop_item.h"
#include "d/d_camera.h"
#include "d/d_com_static.h"
#include "d/d_item.h"
#include "d/d_timer.h"

daNpc_Maro_c::actionFunc dummy_lit_4085() {
    return &daNpc_Maro_c::choccai;
}

static int l_bmdData[2][2] = {
    {11, 1}, {5, 2},
};

daNpcT_evtData_c l_evtList[15] = {
    {"", 0},
    {"DEFAULT_GETITEM", 0},
    {"NO_RESPONSE", 0},
    {"CONVERSATION_ABOUT_WOODSWD1", 9},
    {"CONVERSATION_ABOUT_WOODSWD2", 9},
    {"CONVERSATION_WITH_MARO1", 6},
    {"CONVERSATION_WITH_MARO2", 6},
    {"DEMO13_STB", 0},
    {"SURPRISE", 3},
    {"BREAK_ARROWTUTORIAL", 7},
    {"CONTINUE_ARROWTUTORIAL", 7},
    {"END_ARROWTUTORIAL", 7},
    {"CLEAR_ARROWTUTORIAL", 7},
    {"NOT_GONNA_LET_3", 6},
    {"TALK_TO_KAKASHI", 3},
};

static char* l_resNameList[10] = {
    "",
    "Maro",
    "Maro_TW",
    "Maro1",
    "Maro2",
    "Maro3",
    "Taro1",
    "Taro2",
    "Len1",
    "Besu1",
};

static s8 l_loadResPtrn0[3] = {1, 3, -1};

static s8 l_loadResPtrn1[3] = {1, 2, -1};

static s8 l_loadResPtrn2[4] = {1, 3, 4, -1};

static s8 l_loadResPtrn3[5] = {1, 2, 3, 4, -1};

static s8* l_loadResPtrnList[17] = {
    l_loadResPtrn2, l_loadResPtrn2, l_loadResPtrn0, l_loadResPtrn1,
    l_loadResPtrn0, l_loadResPtrn0, l_loadResPtrn0, l_loadResPtrn2,
    l_loadResPtrn2, l_loadResPtrn0, l_loadResPtrn2, l_loadResPtrn2,
    l_loadResPtrn2, l_loadResPtrn2, l_loadResPtrn2, l_loadResPtrn2,
    l_loadResPtrn3,
};

static daNpcT_faceMotionAnmData_c l_faceMotionAnmData[10] = {
    {-1, 0, 0, 17, 2, 1, 1},
    {6, 0, 1, 18, 0, 1, 0},
    {7, 0, 3, 22, 0, 3, 0},
    {8, 0, 3, 23, 0, 3, 0},
    {6, 0, 3, 21, 0, 3, 0},
    {6, 0, 4, 10, 0, 4, 0},
    {6, 0, 5, 10, 0, 5, 0},
    {9, 2, 3, 24, 2, 3, 0},
    {7, 2, 4, 11, 2, 4, 0},
    {7, 2, 5, 11, 2, 5, 0},
};

static daNpcT_motionAnmData_c l_motionAnmData[10] = {
    {8, 2, 1, 14, 0, 1, 1, 0},
    {5, 2, 4, 14, 0, 1, 1, 0},
    {5, 2, 5, 14, 0, 1, 1, 0},
    {12, 0, 3, 14, 0, 1, 1, 0},
    {13, 0, 3, 17, 0, 3, 0, 0},
    {14, 0, 3, 18, 0, 3, 0, 0},
    {4, 0, 5, 14, 0, 1, 1, 0},
    {11, 2, 3, 14, 0, 1, 1, 0},
    {7, 0, 1, 14, 0, 1, 1, 0},
    {10, 2, 3, 14, 0, 1, 1, 0},
};

static daNpcT_MotionSeqMngr_c::sequenceStepData_c l_faceMotionSequenceData[40] = {
    {1, -1, 1}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {4, -1, 1}, {7, -1, 0}, {-1, 0, 0}, {-1, 0, 0},
    {2, -1, 1}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {5, -1, 1}, {8, -1, 0}, {-1, 0, 0}, {-1, 0, 0},
    {6, -1, 1}, {9, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {3, -1, 1}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {7, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {8, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {9, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {0, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
};

static daNpcT_MotionSeqMngr_c::sequenceStepData_c l_motionSequenceData[36] = {
    {0, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {3, -1, 1}, {0, -1, 0}, {-1, 0, 0}, {-1, 0, 0},
    {1, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {6, -1, 1}, {2, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {7, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {4, -1, 1}, {0, -1, 0}, {-1, 0, 0}, {-1, 0, 0},
    {5, -1, 1}, {0, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {8, 4, 1}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {9, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
};

char* daNpc_Maro_c::mCutNameList[17] = {
    "",
    "CONVERSATION_ABOUT_PACHINKO",
    "CONVERSATION_ABOUT_WOODSWD",
    "SWDTUTORIAL",
    "HAIL",
    "FIND_MONKEY",
    "GIVEME_WOODSWD",
    "GET_WOODSWD",
    "CONVERSATION_WITH_MARO",
    "CACARICO_CONVERSATION",
    "SURPRISE",
    "ARROWTUTORIAL",
    "BOKIN_TALK",
    "MAROS_WHISPER",
    "TAG_PUSH1",
    "NOT_GONNA_LET",
    "TALK_TO_KAKASHI",
};

daNpc_Maro_c::cutFunc daNpc_Maro_c::mCutList[17] = {
    NULL,
    &daNpc_Maro_c::cutConversationAboutPachinko,
    &daNpc_Maro_c::cutConversationAboutWoodSwd,
    &daNpc_Maro_c::cutSwdTutorial,
    &daNpc_Maro_c::cutHail,
    &daNpc_Maro_c::cutFindMonkey,
    &daNpc_Maro_c::cutGiveMeWoodSwd,
    &daNpc_Maro_c::cutGetWoodSwd,
    &daNpc_Maro_c::cutConversationWithMaro,
    &daNpc_Maro_c::cutCacaricoConversation,
    &daNpc_Maro_c::cutSurprise,
    &daNpc_Maro_c::cutArrowTutorial,
    &daNpc_Maro_c::cutBokinTalk,
    &daNpc_Maro_c::cutMarosWhisper,
    &daNpc_Maro_c::cutTagPush1,
    &daNpc_Maro_c::cutNotGonnaLet,
    &daNpc_Maro_c::cutTalkToKakashi,
};

daNpc_Maro_c::~daNpc_Maro_c() {
    deleteObject();
    if (field_0x10bc != 0xFFFFFFFF) {
        dComIfG_TimerDeleteRequest(0);
    }

    if (mpMorf[0] != 0) {
        mpMorf[0]->stopZelAnime();
    }

    deleteRes(l_loadResPtrnList[mType], (char const**)l_resNameList);
}

const daNpc_Maro_HIOParam daNpc_Maro_Param_c::m = {
    100.0f,
    -3.0f,
    1.0f,
    400.0f,
    255.0f,
    80.0f,
    35.0f,
    30.0f,
    0.0f,
    0.0f,
    10.0f,
    -10.0f,
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
    0,
    0,
    4.0f,
    -15.0f,
    0.0f,
    -15.0f,
    15.0f,
    30.0f,
    15.0f,
    30.0f,
    0x00780000,
};

int daNpc_Maro_c::create() {
    static int const heapSize[17] = {
        0x3310, 0x3310,      0, 0x3260,
        0x3320, 0x3320, 0x3320, 0x3310,
        0x3310, 0x3320, 0x3310, 0x3310,
        0x3310, 0x3310, 0x3310, 0x3310,
        0,
    };
    daNpcT_ct(this, daNpc_Maro_c, l_faceMotionAnmData, l_motionAnmData,
                       l_faceMotionSequenceData, 4, l_motionSequenceData, 4,
                       l_evtList, l_resNameList
    );
    mType = getType();
    mFlowNodeNo = getFlowNodeNo();
    mTwilight = dKy_darkworld_check();
    if (mType == 9) {
        mTwilight = 0;
    }

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
        mCcStts.Init(daNpc_Maro_Param_c::m.common.weight, 0, this);
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

int daNpc_Maro_c::CreateHeap() {
    int bmd_get_idx;
    if (mTwilight == 1) {
        bmd_get_idx = 1;
    } else {
        bmd_get_idx = 0;
    }

    J3DModelData* modelData = static_cast<J3DModelData*>(dComIfG_getObjectRes(
        l_resNameList[l_bmdData[bmd_get_idx][1]], l_bmdData[bmd_get_idx][0]));
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

int daNpc_Maro_c::Delete() {
    fopAcM_GetID(this);
    this->~daNpc_Maro_c();
    return 1;
}

int daNpc_Maro_c::Execute() {
    if (!mCreating && !checkShopOpen() && mActorMngr[5].getActorP() != NULL && mType == 7 && !field_0x1131) {
        initShopSystem();
        setSellItemMax(getMaxNumItem());
        field_0xf60 = -1;
        setMasterType(2);
        mShopCamAction.setCamDataIdx(NULL);
        mShopCamAction.setCamAction(NULL);
        field_0x1131 = 1;
    }

    daNpcT_c::execute();
    if (field_0x1131 && searchItemActor()) {
        mShopCamAction.move();
    }

    return 1;
}

int daNpc_Maro_c::Draw() {
    if (mpMatAnm[0] != NULL) {
        J3DModelData* modelData = mpMorf[0]->getModel()->getModelData();
        modelData->getMaterialNodePointer(getEyeballMaterialNo())->setMaterialAnm(mpMatAnm[0]);
    }
    return daNpcT_c::draw(0, 0, mRealShadowSize, NULL, 100.0f, 0, 0, 0);
}

int daNpc_Maro_c::createHeapCallBack(fopAc_ac_c* i_this) {
    return static_cast<daNpc_Maro_c*>(i_this)->CreateHeap();
}

int daNpc_Maro_c::ctrlJointCallBack(J3DJoint* param_0, int param_1) {
    if (param_1 == 0) {
        J3DModel* model = j3dSys.getModel();
        daNpc_Maro_c* i_this = reinterpret_cast<daNpc_Maro_c*>(model->getUserArea());
        if (i_this != 0) {
            i_this->ctrlJoint(param_0, model);
        }
    }

    return 1;
}

void* daNpc_Maro_c::srchArrow(void* param_1, void* param_2) {
    if (mFindCount < 50 && param_1 != NULL && param_1 != param_2) {
        if (fopAcM_IsExecuting(fopAcM_GetID(param_1)) && fopAcM_GetName(param_1) == PROC_ARROW) {
            mFindActorPtrs[mFindCount] = (fopAc_ac_c*)param_1;
            mFindCount++;
        }
    }
    return NULL;
}

fopAc_ac_c* daNpc_Maro_c::getArrowP() {
    fopAc_ac_c* pActor = NULL;
    f32 minDist = G_CM3D_F_INF;
    mFindCount = 0;
    fopAcM_Search(srchArrow, this);
    for (int i = 0; i < mFindCount; i++) {
        if (cM3d_IsZero(mFindActorPtrs[i]->speedF) == false &&
            fopAcM_searchActorDistance(this, mFindActorPtrs[i]) < minDist)
        {
            minDist = fopAcM_searchActorDistance(this, mFindActorPtrs[i]);
            pActor = mFindActorPtrs[i];
        }
    }
    return pActor;
}

void* daNpc_Maro_c::srchItaMato(void* arg0, void* arg1) {
    fopAc_ac_c* fop1 = (fopAc_ac_c*) arg1;
    if (mFindCount < 50 && arg0 != NULL && arg0 != fop1) {
        if (fopAcM_IsExecuting(fopAcM_GetID(arg0)) && fopAcM_GetName(arg0) == PROC_OBJ_ITAMATO) {
            mFindActorPtrs[mFindCount] = (fopAc_ac_c*)arg0;
            mFindCount++;
        }
    }
    return NULL;
}

fopAc_ac_c* daNpc_Maro_c::getItaMatoP(int param_0) {
    fopAc_ac_c* pActor = NULL;
    f32 minDist = G_CM3D_F_INF;
    mFindCount = 0;
    fopAcM_Search(srchItaMato, this);
    for (int i = 0; i < mFindCount; i++) {
        if ( ((daObj_ItaMato_c*) mFindActorPtrs[i])->getNo() != param_0 ) {
            continue;
        }

        if (fopAcM_searchActorDistance(this, mFindActorPtrs[i]) < minDist)
        {
            minDist = fopAcM_searchActorDistance(this, mFindActorPtrs[i]);
            pActor = mFindActorPtrs[i];
        }
    }

    return pActor;
}

u8 daNpc_Maro_c::getType() {
    u8 param = fopAcM_GetParam(this) & 0xFF;
    switch (param) {
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
        case 5:
            return TYPE_5;
        case 6:
            return TYPE_6;
        case 7:
            return TYPE_7;
        case 8:
            return TYPE_8;
        case 9:
            return TYPE_9;
        case 0xA:
            return TYPE_10;
        case 0xB:
            return TYPE_11;
        case 0xC:
            return TYPE_12;
        case 0xD:
            return TYPE_13;
        case 0xE:
            return TYPE_14;
        case 0xF:
            return TYPE_15;
        default:
            return TYPE_16;
    }
}

int daNpc_Maro_c::isDelete() {
    switch (mType) {
        case 0: {
            return (!daNpcT_chkEvtBit(0x12) || daNpcT_chkEvtBit(0x1F));
        }

        case 1: {
            return !daNpcT_chkEvtBit(0x12);
        }

        case 2: {
            return true;
        }

        case 3: {
            return false;
        }

        case 4: {
            return (!daNpcT_chkEvtBit(0x3D) || daNpcT_chkEvtBit(0xA4));
        }

        case 5: {
            return (!daNpcT_chkEvtBit(0xA4) || daNpcT_chkEvtBit(0x35));
        }

        case 6: {
            return (!daNpcT_chkEvtBit(0x35) || daNpcT_chkEvtBit(0x55));
        }

        case 7: {
            return !daNpcT_chkEvtBit(0x55);
        }

        case 8: {
            return !daNpcT_chkEvtBit(0x40);
        }

        case 9: {
            return false;
        }

        case 0xA: {
            return daNpcT_chkEvtBit(0x13);
        }

        case 0xB: {
            return (!daNpcT_chkEvtBit(0x13) || daNpcT_chkEvtBit(0x12));
        }

        case 0xC: {
            return (!daNpcT_chkEvtBit(0x12) || daNpcT_chkEvtBit(0x25C));
        }

        case 0xD: {
            return false;
        }

        case 0xE: {
            return (!daNpcT_chkEvtBit(0x25C) || daNpcT_chkEvtBit(0x80));
        }

        case 0xF: {
            return (!daNpcT_chkEvtBit(0x80) || daNpcT_chkEvtBit(0x235));
        }

        default: {
            return false;
        }
    }
}

void daNpc_Maro_c::reset() {
    csXyz acStack_20;
    int iVar1 = (u8*)&field_0x113C - (u8*)&field_0x110c;
    if (mpMatAnm[0] != NULL) {
        mpMatAnm[0]->initialize();
    }

    initialize();
    for (int i = 0; i < 9; i++) {
        mActorMngr[i].initialize();
    }

    memset(&field_0x110c, 0, iVar1);
    acStack_20.setall(0);
    acStack_20.y = home.angle.y;
    switch (mType) {
        case 0:{
            eventInfo.setIdx(0);
            break;
        }

        case 1: {
            mHide = !daNpcT_chkEvtBit(0x1F);
            eventInfo.setIdx(1);
            break;
        }

        case 0xB: {
            dComIfGs_setTmpReg(0xfbff, 0);
        }

        case 0xC:
        case 0xD:
        case 0xE:
        case 0xF: {
            break;
        }
    }

    field_0x10bc = -1;
    daNpcT_offTmpBit(0x3C);
    daNpcT_offTmpBit(0x3F);
    daNpcT_offTmpBit(0x40);
    daNpcT_offTmpBit(0x41);
    daNpcT_offTmpBit(0x42);
    daNpcT_offTmpBit(0x64);
    setAngle(acStack_20);
}

void daNpc_Maro_c::afterJntAnm(int param_1) {
    if (param_1 == 1) {
        mDoMtx_stack_c::YrotM(mStagger.getAngleX(1));
        mDoMtx_stack_c::ZrotM(mStagger.getAngleZ(1));
    } else if (param_1 == 4) {
        mDoMtx_stack_c::YrotM(mStagger.getAngleX(0));
        mDoMtx_stack_c::ZrotM(mStagger.getAngleZ(0));
    }
}

void daNpc_Maro_c::setParam() {
    if (field_0x1130) {
        if (field_0x112c == 2) {
            mShopCamAction.Reset();
        } else {
            mShopCamAction.EventRecoverNotime();
        }

        field_0x1130 = 0;
    }

    selectAction();
    if (&daNpc_Maro_c::arrowTutorial == field_0x110c && field_0x10bc == 0xFFFFFFFF) {
        field_0x10bc = dTimer_createTimer(0, 60000, 0, 0, 210.0f, 410.0f, 32.0f, 419.0f);
    }

    srchActors();
    u32 uVar7 = (fopAc_AttnFlag_SPEAK_e | fopAc_AttnFlag_TALK_e);
    s16 sVar10 = daNpc_Maro_Param_c::m.common.talk_distance;
    s16 sVar9 = daNpc_Maro_Param_c::m.common.talk_angle;
    s16 sVar8 = daNpc_Maro_Param_c::m.common.attention_distance;
    s16 sVar7 = daNpc_Maro_Param_c::m.common.attention_angle;
    if (&daNpc_Maro_c::swdTutorial == field_0x110c) {
        sVar10 = 11;
        sVar9 = 6;
        sVar8 = 15;
        sVar7 = 6;
        uVar7 = 0;
    } else if (mType == 11) {
        sVar10 = 7;
        sVar8 = 9;
    } else if (mType == 6) {
        sVar10 = 4;
        sVar8 = 6;
    } else if (&daNpc_Maro_c::talk_withTaro == field_0x110c) {
        sVar10 = 5;
        sVar9 = 4;
        sVar8 = 7;
        sVar7 = 4;
    }

    if (field_0x1131) {
        sVar10 = 5;
        sVar8 = 5;
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

    attention_info.flags = uVar7;
    scale.set(daNpc_Maro_Param_c::m.common.scale, daNpc_Maro_Param_c::m.common.scale,
            daNpc_Maro_Param_c::m.common.scale);
    mCcStts.SetWeight(daNpc_Maro_Param_c::m.common.weight);
    mCylH = daNpc_Maro_Param_c::m.common.height;
    mWallR = daNpc_Maro_Param_c::m.common.width;
    mAttnFovY = daNpc_Maro_Param_c::m.common.fov;
    mAcchCir.SetWallR(mWallR);
    mAcchCir.SetWallH(daNpc_Maro_Param_c::m.common.knee_length);
    mRealShadowSize = daNpc_Maro_Param_c::m.common.real_shadow_size;
    mExpressionMorfFrame = daNpc_Maro_Param_c::m.common.expression_morf_frame;
    mMorfFrames = daNpc_Maro_Param_c::m.common.morf_frame;
    gravity = daNpc_Maro_Param_c::m.common.gravity;
}

BOOL daNpc_Maro_c::checkChangeEvt() {
    if (!chkAction(&daNpc_Maro_c::talk)) {
        mPreItemNo = 0;
        if (dComIfGp_event_chkTalkXY()) {
            if (dComIfGp_evmng_ChkPresentEnd()) {
                mEvtNo = 2;
                evtChange();
            }

            return true;
        }

        switch (mType) {
            case TYPE_1: {
                if (daNpcT_chkEvtBit(0x269)) {
                    mEvtNo = 6;
                } else {
                    mEvtNo = 5;
                }

                evtChange();
                return true;
            }

            case TYPE_2: {
                mEvtNo = 14;
                evtChange();
                return true;
            }

            case TYPE_11: {
                if (daNpcT_chkEvtBit(0x266) && checkItemGet(0x3F, 1) && !daNpcT_chkEvtBit(0x22)) {
                    mEvtNo = 4;
                } else {
                    mEvtNo = 3;
                }

                evtChange();
                return true;
            }

            case TYPE_0:
            case TYPE_3:
            case TYPE_4:
            case TYPE_5:
            case TYPE_6:
            case TYPE_7:
            case TYPE_8:
            case TYPE_9:
            case TYPE_10:
            case TYPE_12:
            case TYPE_13:
            case TYPE_14:
            case TYPE_15: {
                break;
            }
        }
    }

    return false;
}

BOOL daNpc_Maro_c::evtEndProc() {
    if (field_0x1138) {
        dCam_getBody()->CorrectCenter();
    }

    return 1;
}

void daNpc_Maro_c::setAfterTalkMotion() {
    int iVar2 = 9;
    switch(mFaceMotionSeqMngr.getNo()) {
        case 1:
            return;
        case 3:
            iVar2 = 7;
            break;
        case 4:
            iVar2 = 8;
            break;
    }

    mFaceMotionSeqMngr.setNo(iVar2, -1.0f, 0, 0);
}

void daNpc_Maro_c::srchActors() {
    switch (mType) {
        case TYPE_0: {
            if (mActorMngr[0].getActorP() == NULL) {
                mActorMngr[0].entry(getNearestActorP(0x241));
            }

            if (mActorMngr[1].getActorP() == NULL) {
                mActorMngr[1].entry(getNearestActorP(0x26E));
            }

            if (mActorMngr[2].getActorP() == NULL) {
                mActorMngr[2].entry(getNearestActorP(0x245));
            }

            if (mActorMngr[3].getActorP() == NULL) {
                mActorMngr[3].entry(getNearestActorP(0x25D));
            }
            break;
        }

        case TYPE_1: {
            if (mActorMngr[0].getActorP() == NULL) {
                mActorMngr[0].entry(getNearestActorP(0x241));
            }

            if (mActorMngr[1].getActorP() == NULL) {
                mActorMngr[1].entry(getNearestActorP(0x26E));
            }
            break;
        }

        case TYPE_3: {
            if (mActorMngr[8].getActorP() == NULL) {
                mActorMngr[8].entry(getNearestActorP(0x250));
            }

            break;
        }

        case TYPE_8:
        case TYPE_7: {
            if (&daNpc_Maro_c::arrowTutorial == field_0x110c) {
                if (mActorMngr[1].getActorP() == NULL) {
                    mActorMngr[1].entry(getNearestActorP(0x26E));
                }

                if (mActorMngr[6].getActorP() == NULL) {
                    mActorMngr[6].entry(getEvtAreaTagP(11, 0));
                }
            }

            if (&daNpc_Maro_c::tend == field_0x110c && mActorMngr[5].getActorP() == NULL) {
                mActorMngr[5].entry(getShopItemTagP());
            }

            break;
        }

        case TYPE_10: {
            if (mActorMngr[2].getActorP() == NULL) {
                mActorMngr[2].entry(getNearestActorP(0x245));
            }

            break;
        }

        case TYPE_12: {
            if (mActorMngr[1].getActorP() == NULL) {
                mActorMngr[1].entry(getNearestActorP(0x26E));
            }

            if (mActorMngr[3].getActorP() == NULL) {
                mActorMngr[3].entry(getNearestActorP(0x25d));
            }

            if (mActorMngr[7].getActorP() == NULL) {
                mActorMngr[7].entry(getNearestActorP(0x264));
            }
            break;
        }

        case TYPE_13: {
            if (mActorMngr[0].getActorP() == NULL) {
                mActorMngr[0].entry(getNearestActorP(0x241));
            }

            if (mActorMngr[1].getActorP() == NULL) {
                mActorMngr[1].entry(getNearestActorP(0x26E));
            }

            if (mActorMngr[2].getActorP() == NULL) {
                mActorMngr[2].entry(getNearestActorP(0x245));
            }

            if (mActorMngr[3].getActorP() == NULL) {
                mActorMngr[3].entry(getNearestActorP(0x25D));
            }
            break;
        }

        case TYPE_4:
        case TYPE_5:
        case TYPE_6:
        case TYPE_9:
        case TYPE_11:
        case TYPE_14:
        case TYPE_15:
        case TYPE_2: {
            break;
        }
    }
}

BOOL daNpc_Maro_c::evtTalk() {
    if (field_0x1131) {
        if (chkAction(&daNpc_Maro_c::shop)) {
            (this->*(field_0x1118))(NULL);
        } else {
            mShopCamAction.shop_cam_action_init();
            setAction(&daNpc_Maro_c::shop);
        }
    } else {
        if (chkAction(&daNpc_Maro_c::talk)) {
            (this->*(field_0x1118))(NULL);
        } else {
            setAction(&daNpc_Maro_c::talk);
        }
    }
    return 1;
}

BOOL daNpc_Maro_c::evtCutProc() {
    int staff_id = dComIfGp_getEventManager().getMyStaffId("Maro", this, -1);
    if (staff_id != -1) {
        if (mStaffId == -1) {
            if (0 == strcmp(dComIfGp_getEventManager().getRunEventName(), "BREAK_ARROWTUTORIAL")
             || 0 == strcmp(dComIfGp_getEventManager().getRunEventName(), "CONTINUE_ARROWTUTORIAL")
             || 0 == strcmp(dComIfGp_getEventManager().getRunEventName(), "END_ARROWTUTORIAL")
             || 0 == strcmp(dComIfGp_getEventManager().getRunEventName(), "CLEAR_ARROWTUTORIAL")) {
                field_0x1138 = 1;
            }
        }

        mStaffId = staff_id;
        int actIdx = dComIfGp_getEventManager().getMyActIdx(mStaffId, mCutNameList, 17, 0, 0);
        if ((this->*(mCutList[actIdx]))(mStaffId) != 0) {
            dComIfGp_getEventManager().cutEnd(mStaffId);
        }
        return 1;
    }

    if (field_0x1131) {
        mShopCamAction.Save();
    }

    return 0;
}

void daNpc_Maro_c::action() {
    fopAc_ac_c* actor_p = NULL;
    if (!mTwilight) {
        actor_p = hitChk(&mCyl1, -1);
    }

    if (actor_p != NULL && mType != 6) {
        mStagger.setParam(this, actor_p, mCurAngle.y);
        setDamage(0, 9, 0);
        mDamageTimerStart = 0;
        mJntAnm.lookNone(1);
    }

    if (mStagger.checkRebirth()) {
        mStagger.initialize();
        mMode = 1;
    }

    if (dComIfGp_event_runCheck() == 0) {
        field_0x1138 = 0;
        if (daNpcT_chkTmpBit(0x3C)) {
            if (dComIfGs_getArrowNum() == 0) {
                if (field_0x1128 && (cLib_calcTimer(&field_0x1128) == 0 || getArrowP() == NULL)) {
                    field_0x1128 = 0;
                    field_0x1137 = 1;
                }
            } else {
                field_0x1128 = 30;
            }
        }
    }

    if (field_0x110c) {
        if (field_0x1118 == field_0x110c) {
            (this->*(field_0x1118))(NULL);
        } else {
            setAction(field_0x110c);
        }
    }

    actor_p = field_0xba0.getActorP();
    if (actor_p != NULL) {
        switch (((daTag_Push_c*) actor_p)->getId()) {
            case 5: {
                mEvtNo = 8;
            }
        }
    }
}

void daNpc_Maro_c::beforeMove() {
    fopAcM_OffStatus(this, fopAcM_STATUS_UNK_0x8000000);
    if (checkHide()) {
        fopAcM_OnStatus(this, fopAcM_STATUS_UNK_0x8000000);
    }
    if (checkHide() || mNoDraw) {
        attention_info.flags = 0;
    }
}

void daNpc_Maro_c::setAttnPos() {
    cXyz eyeOffset(0.0f, 0.0f, -30.0f);
    if (field_0x1133) {
        daPy_getPlayerActorClass()->onWolfEyeKeep();
    }

    mStagger.calc(0);
    f32 dVar8 = cM_s2rad(mCurAngle.y - field_0xd7e.y);
    mJntAnm.setParam(
        this, mpMorf[0]->getModel(), &eyeOffset, getBackboneJointNo(), getNeckJointNo(),
        getHeadJointNo(), daNpc_Maro_Param_c::m.common.body_angleX_min, daNpc_Maro_Param_c::m.common.body_angleX_max,
        daNpc_Maro_Param_c::m.common.body_angleY_min, daNpc_Maro_Param_c::m.common.body_angleY_max,
        daNpc_Maro_Param_c::m.common.head_angleX_min, daNpc_Maro_Param_c::m.common.head_angleX_max,
        daNpc_Maro_Param_c::m.common.head_angleY_min, daNpc_Maro_Param_c::m.common.head_angleY_max,
        daNpc_Maro_Param_c::m.common.neck_rotation_ratio, 0.0f, NULL);
    mJntAnm.calcJntRad(0.2f, 1.0f, (float)dVar8);
    setMtx();
    mDoMtx_stack_c::copy(mpMorf[0]->getModel()->getAnmMtx(getHeadJointNo()));
    mDoMtx_stack_c::multVec(&eyeOffset, &eyePos);
    mJntAnm.setEyeAngleX(eyePos, 1.0f, 0);
    mJntAnm.setEyeAngleY(eyePos, mCurAngle.y, 0, 1.0f, 0);
    eyeOffset.set(0.0f, 0.0f, 0.0f);
    eyeOffset.y = daNpc_Maro_Param_c::m.common.attention_offset;
    mDoMtx_stack_c::YrotS(mCurAngle.y);
    mDoMtx_stack_c::multVec(&eyeOffset, &eyeOffset);
    attention_info.position = current.pos + eyeOffset;
}

void daNpc_Maro_c::setCollision() {
    cXyz cStack_48;
    if (!mHide) {
        int tgType = 0xd8fbfdff;
        int tgSPrm = 0x1f;
        if (mTwilight) {
            tgType = 0;
            tgSPrm = 0;
        } else if (mStagger.checkStagger()) {
            tgType = 0;
            tgSPrm = 0;
        }
        mCyl1.SetCoSPrm(0x79);
        mCyl1.SetTgType(tgType);
        mCyl1.SetTgSPrm(tgSPrm);
        mCyl1.OnTgNoHitMark();
        cStack_48.set(0.0f, 0.0f, 0.0f);
        f32 cylH = mCylH;
        f32 wallR = mWallR;
        mDoMtx_stack_c::YrotS(mCurAngle.y);
        mDoMtx_stack_c::multVec(&cStack_48, &cStack_48);
        cStack_48 += current.pos;
        mCyl1.SetH(cylH);
        mCyl1.SetR(wallR);
        mCyl1.SetC(cStack_48);
        dComIfG_Ccsp()->Set(&mCyl1);
    }

    mCyl1.ClrCoHit();
    mCyl1.ClrTgHit();
}

BOOL daNpc_Maro_c::drawDbgInfo() {
    return FALSE;
}

void daNpc_Maro_c::drawOtherMdl() {
    if (field_0x1131) {
        cXyz my_vec;
        itemRotate();
        my_vec.set(0.0f, 0.0f, 0.0f);
        itemZoom(&my_vec);
        drawCursor();
    }
}

 daNpcT_faceMotionAnmData_c daNpc_Maro_c::getFaceMotionAnm(daNpcT_faceMotionAnmData_c param_0) {
    daNpcT_faceMotionAnmData_c rv = param_0;
    if (mTwilight != TYPE_0 && rv.mBtpFileIdx == 17) {
        rv.mBtpFileIdx = 11;
        rv.mBtpArcIdx = 2;
    }
    return rv;
}

 daNpcT_motionAnmData_c daNpc_Maro_c::getMotionAnm(daNpcT_motionAnmData_c param_0) {
    daNpcT_motionAnmData_c rv = param_0;
    if (mTwilight && rv.mBtkFileIdx == 14) {
        rv.mBtkFileIdx = 8;
        rv.mBtkArcIdx = 2;
    }
    return rv;
}

int daNpc_Maro_c::selectAction() {
    field_0x110c = NULL;
    switch (mType) {
        case 7: {
            field_0x110c = &daNpc_Maro_c::tend;
            break;
        }

        case 8: {
            field_0x110c = &daNpc_Maro_c::arrowTutorial;
            break;
        }

        case 13: {
            if ((u8)daNpcKakashi_chkSwdTutorialStage()) {
                field_0x110c = &daNpc_Maro_c::swdTutorial;
            } else {
                field_0x110c = &daNpc_Maro_c::wait;
            }

            break;
        }

        default: {
            field_0x110c = &daNpc_Maro_c::wait;
            break;
        }
    }

    return 1;
}

int daNpc_Maro_c::chkAction(int (daNpc_Maro_c::*param_0)(void*)) {
    return field_0x1118 == param_0;
}

int daNpc_Maro_c::setAction(int (daNpc_Maro_c::*action)(void*)) {
    mMode = MODE_EXIT;
    if (field_0x1118 != NULL) {
        (this->*(field_0x1118))(NULL);
    }
    mMode = MODE_ENTER;
    field_0x1118 = action;
    if (field_0x1118 != NULL) {
        (this->*(field_0x1118))(NULL);
    }
    return 1;
}

int daNpc_Maro_c::cutConversationAboutPachinko(int arg0) {
    int retval = 0;
    int* param_p = NULL;
    int prm_val = -1;
    param_p = dComIfGp_evmng_getMyIntegerP(arg0, "prm");
    if (param_p != NULL) {
        prm_val = *param_p;
    }

    if (dComIfGp_getEventManager().getIsAddvance(arg0)) {
        switch (prm_val) {
            case 0: {
                break;
            }

            case 1: {
                mPlayerAngle = fopAcM_searchPlayerAngleY(this);
            }
        }
    }

    switch (prm_val) {
        case 0: {
            retval = 1;
            break;
        }

        case 1: {
            mJntAnm.lookPlayer(0);
            if (mPlayerAngle == mCurAngle.y) {
                retval = 1;
            } else if (step(mPlayerAngle, 9, 7, 0xF, 0)) {
                mFaceMotionSeqMngr.setNo(9, -1.0f, 0, 0);
                mMotionSeqMngr.setNo(0, -1.0f, 0, 0);
            }
        }
    }

    return retval;
}

int daNpc_Maro_c::cutConversationAboutWoodSwd(int arg0) {
    int retval = 0;
    int* param_p = NULL;
    int prm_val = -1;
    param_p = dComIfGp_evmng_getMyIntegerP(arg0, "prm");
    if (param_p != NULL) {
        prm_val = *param_p;
    }

    if (dComIfGp_getEventManager().getIsAddvance(arg0)) {
        switch (prm_val) {
            case 0: {
                mPlayerAngle = fopAcM_searchPlayerAngleY(this);
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
            if (mPlayerAngle != mCurAngle.y) {
                if (step(mPlayerAngle, 9, 7, 0xF, 0)) {
                    mFaceMotionSeqMngr.setNo(9, -1.0f, 0, 0);
                    mMotionSeqMngr.setNo(0, -1.0f, 0, 0);
                }
            } else {
                retval = 1;
            }

            break;
        }

        case 1: {
            retval = 1;
            break;
        }
    }

    return retval;
}

int daNpc_Maro_c::cutSwdTutorial(int arg0) {
    cXyz my_vec_0;
    int retval = 0;
    int* param_p = NULL;
    int prm_val = -1;
    param_p = dComIfGp_evmng_getMyIntegerP(arg0, "prm");
    if (param_p != NULL) {
        prm_val = *param_p;
    }

    if (dComIfGp_getEventManager().getIsAddvance(arg0)) {
        switch (prm_val) {
            case 0: {
                break;
            }

            case 1: {
                break;
            }

            case 71: {
                mFaceMotionSeqMngr.setNo(1, 0.0f, 0, 0);
                mMotionSeqMngr.setNo(0, 0.0f, 0, 0);
                setAngle(home.angle.y);
                setPos(current.pos);
            }
        }
    }

    switch (prm_val) {
        case 0: {
            mDamageTimer = 1;
            action();
            mJntAnm.lookNone(0);
            retval = 1;
            break;
        }

        case 1: {
            mDamageTimer = 1;
            action();
            my_vec_0.set(0.0f, 30.0f, 30.0f);
            mDoMtx_stack_c::YrotS(mCurAngle.y + 0x2000);
            mDoMtx_stack_c::multVec(&my_vec_0, &my_vec_0);
            field_0xd6c = current.pos + my_vec_0;
            mJntAnm.sorasu1(&field_0xd6c, 0);
            retval = 1;
            break;
        }

        case 71: {
            mJntAnm.lookCamera(0);
        }
    }

    return retval;
}

int daNpc_Maro_c::cutHail(int arg0) {
    int retval = 0;
    int* param_p = NULL;
    int prm_val = -1;
    param_p = dComIfGp_evmng_getMyIntegerP(arg0, "prm");
    if (param_p != NULL) {
        prm_val = *param_p;
    }

    if (dComIfGp_getEventManager().getIsAddvance(arg0)) {
        switch (prm_val) {
            case 0: {
                break;
            }

            case 1: {
                mFaceMotionSeqMngr.setNo(9, 0.0f, 0, 0);
                mMotionSeqMngr.setNo(0, 0.0f, 0, 0);
                break;
            }
        }
    }

    switch (prm_val) {
        case 0: {
            action();
            retval = 1;
            break;
        }

        case 1: {
            mJntAnm.lookCamera(0);
            break;
        }
    }

    return retval;
}

int daNpc_Maro_c::cutFindMonkey(int arg0) {
    daHorse_c* actor_p = NULL;
    cXyz my_vec_0;
    int retval = 0;
    int* param_p = NULL;
    int prm_val = -1;
    int timer_val = 0;
    param_p = dComIfGp_evmng_getMyIntegerP(arg0, "prm");
    if (param_p != NULL) {
        prm_val = *param_p;
    }

    param_p = dComIfGp_evmng_getMyIntegerP(arg0, "timer");
    if (param_p != NULL) {
        timer_val = *param_p;
    }

    if (dComIfGp_getEventManager().getIsAddvance(arg0)) {
        switch (prm_val) {
            case 0: {
                break;
            }

            case 1: {
                break;
            }

            case 3: {
                mEventTimer = timer_val;
                if (mEventTimer == 0) {
                    mMotionSeqMngr.setNo(8, -1.0f, 0, 0);
                }

                break;
            }

            case 4: {
                mHide = true;
                speedF = 0.0f;
                speed.setall(0.0f);
                break;
            }
        }
    }

    switch (prm_val) {
        case 2: {
            break;
        }

        case 0: {
            mJntAnm.lookNone(0);
            retval = 1;
            break;
        }

        case 1: {
            mJntAnm.lookNone(0);
            retval = 1;
            break;
        }

        case 3: {
            mJntAnm.lookNone(0);
            if (mEventTimer == 0) {
                actor_p = dComIfGp_getHorseActor();
                JUT_ASSERT(0xA99, NULL != actor_p);
                my_vec_0 = actor_p->current.pos;
                if (50.0f < current.pos.absXZ(my_vec_0)) {
                    cLib_addCalcAngleS2(&current.angle.y, cLib_targetAngleY(&current.pos, &my_vec_0), 4, 0x800);
                    shape_angle.y = current.angle.y;
                    mCurAngle.y = shape_angle.y;
                    cLib_chaseF(&speedF, 10.0f, 0.5f);
                } else {
                    retval = 1;
                }
            } else if (cLib_calcTimer(&mEventTimer) == 0) {
                mMotionSeqMngr.setNo(8, -1.0f, 0, 0);
            }

            break;
        }

        case 4: {
            retval = 1;
            break;
        }
    }

    return retval;
}

int daNpc_Maro_c::cutGiveMeWoodSwd(int arg0) {
    fopAc_ac_c* actor_p = NULL;
    cXyz my_vec_0;
    int retval = 0;
    int* param_p = NULL;
    int prm_val = -1;
    param_p = dComIfGp_evmng_getMyIntegerP(arg0, "prm");
    if (param_p != NULL) {
        prm_val = *param_p;
    }

    if (dComIfGp_getEventManager().getIsAddvance(arg0)) {
        switch (prm_val) {
            case 0: {
                mFaceMotionSeqMngr.setNo(9, -1.0f, 0, 0);
                mMotionSeqMngr.setNo(0, -1.0f, 0, 0);
                break;
            }

            case 1: {
                mFaceMotionSeqMngr.setNo(9, 0.0f, 0, 0);
                mMotionSeqMngr.setNo(4, 0.0f, 0, 0);
                my_vec_0.set(-718.0f, 1331.0f, -2139.0f);
                setPos(my_vec_0);
                setAngle(cM_deg2s(50.0f));
                mStepMode = 0;
                break;
            }

            case 2: {
                break;
            }

            case 3: {
                mFaceMotionSeqMngr.setNo(9, 0.0f, 0, 0);
                mMotionSeqMngr.setNo(0, 0.0f, 0, 0);
                my_vec_0.set(-511.0f, 1370.0f, -2153.0f);
                setPos(my_vec_0);
                setAngle(cM_deg2s(30.0f));
                mStepMode = 0;
                break;
            }
        }
    }

    switch (prm_val) {
        case 0: {
            mJntAnm.lookPlayer(0);
            retval = 1;
            break;
        }

        case 1: {
            actor_p = mActorMngr[3].getActorP();
            JUT_ASSERT(0xB00, NULL != actor_p);
            mJntAnm.lookActor(actor_p, -40.0f, 0);
            retval = 1;
            break;
        }

        case 2:
        case 3: {
            mJntAnm.lookPlayer(0);
            retval = 1;
            break;
        }
    }

    return retval;
}

int daNpc_Maro_c::cutGetWoodSwd(int arg0) {
    fopAc_ac_c* actor_p = NULL;
    cXyz my_vec_0;
    int retval = 0;
    int* param_p = NULL;
    int prm_val = -1;
    int timer_val;
    param_p = dComIfGp_evmng_getMyIntegerP(arg0, "prm");
    if (param_p != NULL) {
        prm_val = *param_p;
    }

    param_p = dComIfGp_evmng_getMyIntegerP(arg0, "timer");
    if (param_p != NULL) {
        timer_val = *param_p;
    }

    if (dComIfGp_getEventManager().getIsAddvance(arg0)) {
        switch (prm_val) {
            case 0: {
                mFaceMotionSeqMngr.setNo(9, -1.0f, 0, 0);
                mMotionSeqMngr.setNo(0, -1.0f, 0, 0);
                break;
            }

            case 1: {
                mFaceMotionSeqMngr.setNo(9, 0.0f, 0, 0);
                mMotionSeqMngr.setNo(0, 0.0f, 0, 0);
                my_vec_0.set(-718.0f, 1331.0f, -2139.0f);
                setPos(my_vec_0);
                setAngle(cM_deg2s(50.0f));
                mStepMode = 0;
                mHide = true;
                break;
            }

            case 2: {
                mHide = false;
                break;
            }

            case 3: {
                mFaceMotionSeqMngr.setNo(9, 0.0f, 0, 0);
                mMotionSeqMngr.setNo(0, 0.0f, 0, 0);
                my_vec_0.set(-511.0f, 1370.0f, -2153.0f);
                setPos(my_vec_0);
                setAngle(cM_deg2s(30.0f));
                mStepMode = 0;
                break;
            }

            case 4: {
                home.pos = current.pos;
                home.angle.y = mCurAngle.y;
                mEventTimer = timer_val;
                if (mEventTimer == 0) {
                    mMotionSeqMngr.setNo(8, -1.0f, 0, 0);
                }

                break;
            }

            case 5: {
                mFaceMotionSeqMngr.setNo(9, 0.0f, 0, 0);
                mMotionSeqMngr.setNo(0, 0.0f, 0, 0);
                speedF = 0.0f;
                speed.setall(0.0f);
                mHide = true;
                break;
            }

            case 11: {
                mHide = false;
                break;
            }
        }
    }

    switch (prm_val) {
        case 0: {
            mJntAnm.lookPlayer(0);
            retval = 1;
            break;
        }

        case 1: {
            actor_p = mActorMngr[3].getActorP();
            JUT_ASSERT(0xB79, NULL != actor_p);
            mJntAnm.lookActor(actor_p, -40.0f, 0);
            retval = 1;
            break;
        }

        case 2:
        case 3: {
            actor_p = mActorMngr[1].getActorP();
            JUT_ASSERT(0xB81, NULL != actor_p);
            mJntAnm.lookActor(actor_p, -40.0f, 0);
            retval = 1;
            break;
        }

        case 4: {
            actor_p = mActorMngr[1].getActorP();
            JUT_ASSERT(0xB81, NULL != actor_p);
            mJntAnm.lookActor(actor_p, -40.0f, 0);
            if (mEventTimer == 0) {
                my_vec_0.set(0.0f, 0.0f, 1600.0f);
                mDoMtx_stack_c::YrotS(home.angle.y + cM_deg2s(60.0f));
                mDoMtx_stack_c::multVec(&my_vec_0, &my_vec_0);
                my_vec_0 += home.pos;
                if (50.0f < current.pos.absXZ(my_vec_0)) {
                    cLib_addCalcAngleS2(&current.angle.y, cLib_targetAngleY(&current.pos, &my_vec_0), 4, 0x800);
                    shape_angle.y = current.angle.y;
                    mCurAngle.y = shape_angle.y;
                    cLib_chaseF(&speedF, 10.0f, 0.5f);
                } else {
                    speedF = 0.0f;
                    speed.setall(0.0f);
                }
            } else if (cLib_calcTimer(&mEventTimer) == 0) {
                mMotionSeqMngr.setNo(8, -1.0f, 0, 0);
            }

            retval = 1;
            break;
        }

        case 5: {
            mJntAnm.lookNone(0);
            retval = 1;
            break;
        }

        case 10:
        case 11: {
            action();
            retval = 1;
            break;
        }
    }

    return retval;
}

int daNpc_Maro_c::cutConversationWithMaro(int arg0) {
    fopAc_ac_c* actor_p = NULL;
    cXyz my_vec_0;
    int retval = 0;
    int* param_p = NULL;
    int prm_val = -1;
    param_p = dComIfGp_evmng_getMyIntegerP(arg0, "prm");
    if (param_p != NULL) {
        prm_val = *param_p;
    }

    dComIfGp_getEventManager().getIsAddvance(arg0);
    switch (prm_val) {
        case 0: {
            actor_p = mActorMngr[1].getActorP();
            JUT_ASSERT(0xB81, NULL != actor_p);
            mJntAnm.lookActor(actor_p, -40.0f, 0);
            retval = 1;
            break;
        }
    }

    return retval;
}

int daNpc_Maro_c::cutCacaricoConversation(int arg0) {
    int retval = 0;
    int* param_p = NULL;
    int prm_val = -1;
    param_p = dComIfGp_evmng_getMyIntegerP(arg0, "prm");
    if (param_p != NULL) {
        prm_val = *param_p;
    }

    if (dComIfGp_getEventManager().getIsAddvance(arg0)) {
        switch (prm_val) {
            case 0: {
                mPlayerAngle = fopAcM_searchPlayerAngleY(this);
                if (checkStep()) {
                    mStepMode = 0;
                }
            }

            case 1: {
                break;
            }
        }
    }

    switch (prm_val) {
        case 0: {
            mJntAnm.lookPlayer(0);
            if (mPlayerAngle != mCurAngle.y) {
                if (step(mPlayerAngle, 9, 7, 0xF, 0)) {
                    mFaceMotionSeqMngr.setNo(9, -1.0f, 0, 0);
                    mMotionSeqMngr.setNo(0, -1.0f, 0, 0);
                }
            } else {
                retval = 1;
            }

            break;
        }

        case 1: {
            mJntAnm.lookPlayer(0);
            retval = 1;
            break;
        }
    }

    return retval;
}

int daNpc_Maro_c::cutSurprise(int arg0) {
    daTag_Push_c* push_p = (daTag_Push_c*) field_0xba0.getActorP();
    int retval = 0;
    int* param_p = NULL;
    int prm_val = -1;
    param_p = dComIfGp_evmng_getMyIntegerP(arg0, "prm");
    if (param_p != NULL) {
        prm_val = *param_p;
    }

    if (dComIfGp_getEventManager().getIsAddvance(arg0)) {
        switch (prm_val) {
            case 0: {
                mFaceMotionSeqMngr.setNo(9, 0.0f, 0, 0);
                mMotionSeqMngr.setNo(0, 0.0f, 0, 0);
                mJntAnm.lookNone(1);
                push_p->pushBackPlayer(1);
                break;
            }

            case 1: {
                initTalk(push_p->getFlowNodeNo(), NULL);
                setAngle(mPlayerAngle);
                mJntAnm.lookPlayer(1);
                break;
            }
        }
    }

    switch (prm_val) {
        case 0: {
            retval = 1;
            break;
        }

        case 1: {
            if (talkProc(NULL, 0, NULL, 0) && mFlow.checkEndFlow()) {
                mPlayerActorMngr.entry(daPy_getPlayerActorClass());
                field_0xba0.remove();
                retval = 1;
            }
            break;
        }
    }

    return retval;
}

int dummy_lit_6272(int arg) {
    int dummy_vals[2] = {-1, -1};
    return dummy_vals[arg];
}

int daNpc_Maro_c::cutArrowTutorial(int arg0) {
    fopAc_ac_c* actor_p = NULL;
    cXyz my_vec_0, temp, my_vec_2;
    csXyz my_s_vec;
    int retval = 0;
    int* param_p = NULL;
    int prm_val = -1;
    int timer_val = 0;
    int msgNo_val = 0;
    int msgNo2_val = 0;
    int send_val = 0;
    param_p = dComIfGp_evmng_getMyIntegerP(arg0, "prm");
    if (param_p != NULL) {
        prm_val = *param_p;
    }

    param_p = dComIfGp_evmng_getMyIntegerP(arg0, "timer");
    if (param_p != NULL) {
        timer_val = *param_p;
    }

    param_p = dComIfGp_evmng_getMyIntegerP(arg0, "msgNo");
    if (param_p != NULL) {
        msgNo_val = *param_p;
    }

    param_p = dComIfGp_evmng_getMyIntegerP(arg0, "msgNo2");
    if (param_p != NULL) {
        msgNo2_val = *param_p;
    }

    param_p = dComIfGp_evmng_getMyIntegerP(arg0, "send");
    if (param_p != NULL) {
        send_val = *param_p;
    }

    fopAc_ac_c* local_4c[2] = {this, mActorMngr[1].getActorP()};
    dComIfGp_setMesgCameraInfoActor(local_4c[0], local_4c[1], NULL, 0, 0, 0, 0, 0, 0, 0);

    if (dComIfGp_getEventManager().getIsAddvance(arg0)) {
        switch (prm_val) {
            case 0: {
                mActorMngr[4].entry(getItaMatoP(1));
                actor_p = mActorMngr[4].getActorP();
                JUT_ASSERT(0xCC3, NULL != actor_p);
                dComIfGp_getEvent().setPtI(actor_p);
                break;
            }

            case 1: {
                mEventTimer = timer_val;
                break;
            }

            case 2:
            case 4: {
                actor_p = mActorMngr[4].getActorP();
                JUT_ASSERT(0xCCE, NULL != actor_p);
                dComIfGp_getEvent().setPt2(actor_p);
                break;
            }

            case 3: {
                mJntAnm.lookPlayer(1);
                break;
            }

            case 5: {
                mJntAnm.lookPlayer(1);
                break;
            }

            case 20: {
                mFaceMotionSeqMngr.setNo(9, -1.0f, 0, 0);
                mMotionSeqMngr.setNo(0, -1.0f, 0, 0);
                Z2GetAudioMgr()->subBgmStop();
                break;
            }

            case 21: {
                setAngle(home.angle.y);
                mHide = true;
                field_0x1135 = 0;
                if (daNpcT_getPlayerInfoFromPlayerList(0x42, current.roomNo, &my_vec_2, &my_s_vec)) {
                    daPy_getPlayerActorClass()->setPlayerPosAndAngle(&my_vec_2, my_s_vec.y, 0);
                }

                if (field_0x10bc != 0xFFFFFFFF) {
                    dComIfG_TimerDeleteRequest(0);
                    field_0x10bc = -1;
                }

                dComIfGp_getAttention()->offAttnDraw();
                daNpcT_offTmpBit(0x3C);
                daNpcT_offTmpBit(0x3F);
                daNpcT_offTmpBit(0x40);
                daNpcT_offTmpBit(0x41);
                daNpcT_offTmpBit(0x42);
                break;
            }

            case 22: {
                mEventTimer = timer_val;
                break;
            }

            case 30: {
                mFaceMotionSeqMngr.setNo(9, 0.0f, 0, 0);
                mMotionSeqMngr.setNo(0, 0.0f, 0, 0);
                mEventTimer = timer_val;
                dComIfGp_getEvent().setPtT(this);
                mActorMngr[4].entry(getItaMatoP(1));
                actor_p = mActorMngr[4].getActorP();
                JUT_ASSERT(0xD0B, NULL != actor_p);
                dComIfGp_getEvent().setPtI(actor_p);
                break;
            }

            case 31: {
                mFaceMotionSeqMngr.setNo(9, 0.0f, 0, 0);
                mMotionSeqMngr.setNo(0, 0.0f, 0, 0);
                initTalk(mFlowNodeNo, local_4c);
                setAngle(mPlayerAngle);
                mHide = false;
                mEventTimer = timer_val;
                dComIfGp_getEvent().setPt2(this);
                break;
            }

            case 32: {
                mActorMngr[4].entry(getItaMatoP(2));
                actor_p = mActorMngr[4].getActorP();
                JUT_ASSERT(0xD1E, NULL != actor_p);
                dComIfGp_getEvent().setPtI(actor_p);
                dComIfGp_getEvent().setPt2(actor_p);
            }
            // fallthrough intentional.
            case 33: {
                mJntAnm.lookPlayer(1);
                break;
            }

            case 40: {
                mFaceMotionSeqMngr.setNo(9, 0.0f, 0, 0);
                mMotionSeqMngr.setNo(0, 0.0f, 0, 0);
                mEventTimer = timer_val;
                mActorMngr[4].entry(getItaMatoP(2));
                actor_p = mActorMngr[4].getActorP();
                JUT_ASSERT(0xD2E, NULL != actor_p);
                dComIfGp_getEvent().setPtI(actor_p);
                dComIfGp_getEvent().setPt2(actor_p);
                break;
            }

            case 41: {
                initTalk(mFlowNodeNo, local_4c);
                break;
            }

            case 50: {
                if (daNpcT_chkEvtBit(0x4C) && daNpcT_chkEvtBit(0x4B)) {
                    initTalk(0x1D, local_4c);
                } else {
                    initTalk(mFlowNodeNo, local_4c);
                }

                mEventTimer = timer_val;
                break;
            }

            case 60: {
                mFaceMotionSeqMngr.setNo(9, 0.0f, 0, 0);
                mMotionSeqMngr.setNo(0, 0.0f, 0, 0);
                mJntAnm.lookNone(1);
                break;
            }

            case 61: {
                initTalk(mFlowNodeNo, local_4c);
                break;
            }

            case 65: {
                actor_p = mActorMngr[1].getActorP();
                JUT_ASSERT(0xD2E, NULL != actor_p);
                dComIfGp_getEvent().setPt2(actor_p);
                break;
            }

            case 66: {
                initTalk(29, local_4c);
                break;
            }

            case 70: {
                mFaceMotionSeqMngr.setNo(9, 0.0f, 0, 0);
                mMotionSeqMngr.setNo(0, 0.0f, 0, 0);
                mPlayerAngle = fopAcM_searchPlayerAngleY(this);
                if (checkStep()) {
                    mStepMode = 0;
                }

                break;
            }

            case 71: {
                initTalk(27, NULL);
                break;
            }

            case 80: {
                mFaceMotionSeqMngr.setNo(9, 0.0f, 0, 0);
                mMotionSeqMngr.setNo(0, 0.0f, 0, 0);
                if (daNpcT_getPlayerInfoFromPlayerList(0x42, current.roomNo, &my_vec_2, &my_s_vec)) {
                    my_vec_0.set(0.0f, 0.0f, 300.0f);
                    mDoMtx_stack_c::YrotS(cLib_targetAngleY(&my_vec_2, &daPy_getPlayerActorClass()->current.pos));
                    mDoMtx_stack_c::multVec(&my_vec_0, &my_vec_0);
                    my_vec_0 += my_vec_2;
                    if (daPy_getPlayerActorClass()->current.pos.absXZ(my_vec_0) < 100.0f) {
                        my_vec_0.set(0.0f, 0.0f, 200.0f);
                        mDoMtx_stack_c::YrotS(cLib_targetAngleY(&my_vec_2, &daPy_getPlayerActorClass()->current.pos));
                        mDoMtx_stack_c::multVec(&my_vec_0, &my_vec_0);
                        my_vec_0 += my_vec_2;
                    }

                    dComIfGp_evmng_setGoal(&my_vec_0);
                }

                break;
            }

            case 90: {
                mFaceMotionSeqMngr.setNo(9, 0.0f, 0, 0);
                mMotionSeqMngr.setNo(0, 0.0f, 0, 0);
                setAngle(fopAcM_searchPlayerAngleY(this));
                mHide = false;
                initTalk(mFlowNodeNo, local_4c);
                dComIfGp_getEvent().setPtT(this);
                break;
            }

            case 7: {
                dComIfGp_getEvent().setPt2(this);
                break;
            }

            case 10: {
                if (daNpcT_getPlayerInfoFromPlayerList(0x42, current.roomNo, &my_vec_2, &my_s_vec)) {
                    my_vec_0.set(0.0f, 300.0f, 200.0f);
                    mDoMtx_stack_c::transS(daPy_getPlayerActorClass()->current.pos);
                    mDoMtx_stack_c::YrotM(cLib_targetAngleY(&daPy_getPlayerActorClass()->current.pos, &my_vec_2));
                    mDoMtx_stack_c::multVec(&my_vec_0, &temp);
                    mGndChk.SetPos(&temp);
                    temp.y = dComIfG_Bgsp().GroundCross(&mGndChk);
                    JUT_ASSERT(0xDA9, -G_CM3D_F_INF != temp.y);
                    dComIfGp_evmng_setGoal(&temp);
                }

                break;
            }

            case 34: {
                mActorMngr[4].entry(getItaMatoP(2));
                actor_p = mActorMngr[4].getActorP();
                JUT_ASSERT(0xDB3, NULL != actor_p);
                dComIfGp_getEvent().setPtI(actor_p);
                dComIfGp_getEvent().setPt2(actor_p);
            }
            // fallthrough intentional.
            case 35: {
                mJntAnm.lookPlayer(1);
                break;
            }

            case 44: {
                actor_p = mActorMngr[1].getActorP();
                JUT_ASSERT(0xDBD, NULL != actor_p);
                dComIfGp_getEvent().setPt2(actor_p);
                break;
            }

            case 64: {
                initTalk(mFlowNodeNo, local_4c);
                break;
            }

            case 93: {
                break;
            }
        }
    }

    int evt_id;
    int my_vals[3] = {-1, -1, -1};
    switch (prm_val) {
        case 0:
        case 93: {
            action();
            retval = 1;
            break;
        }

        case 1:
        case 31: {
            mJntAnm.lookPlayer(0);
            my_vals[0] = msgNo_val;
            my_vals[1] = msgNo2_val;
            if (cLib_calcTimer(&mEventTimer) == 0 && talkProc(my_vals, false, local_4c, false)) {
                if (msgNo_val == 0 && msgNo2_val == 0) {
                    if (mFlow.checkEndFlow()) {
                        retval = 1;
                    }
                } else {
                    retval = 1;
                }
            }

            if (prm_val == 1 && retval != 0) {
                actor_p = mActorMngr[4].getActorP();
                JUT_ASSERT(0xDF0, NULL != actor_p);
                dComIfGp_getEvent().setPt2(actor_p);
            }

            break;
        }

        case 2:
        case 4:
        case 5: {
            if (prm_val != 5) {
                actor_p = mActorMngr[4].getActorP();
                JUT_ASSERT(0xDFA, NULL != actor_p);
                mJntAnm.lookActor(actor_p, 0.0f, 0);
            }

            my_vals[0] = msgNo_val;
            if (talkProc(my_vals, send_val, local_4c, false)) {
                if (msgNo_val == 0) {
                    if (mFlow.checkEndFlow()) {
                        switch (prm_val) {
                            case 2: {
                                mJntAnm.lookPlayer(1);
                                setAngle(home.angle.y);
                            }
                            // fallthrough intentional.
                            case 5: {
                                daNpcT_onEvtBit(0x4A);
                            }
                            // fallthrough intentional.
                            default: {
                                retval = 1;
                            }
                        }
                    }
                } else {
                    retval = 1;
                }
            }

            if (retval == 1) {
                switch (prm_val) {
                    case 2:
                    case 4: {
                        dComIfGp_getEvent().setPt2(this);
                        break;
                    }
                }
            }

            if (prm_val == 4 && send_val == 0) {
                retval = 1;
            }

            break;
        }

        case 3: {
            action();
            retval = 1;
            break;
        }

        case 6: {
            if (home.angle.y != mCurAngle.y) {
                step(home.angle.y, 9, 7, 15, 0);
            } else {
                retval = 1;
            }

            break;
        }

        case 20: {
            mJntAnm.lookPlayer(0);
            retval = 1;
            break;
        }

        case 21: {
            retval = 1;
            break;
        }

        case 22: {
            if (cLib_calcTimer(&mEventTimer) == 0) {
                retval = 1;
            }

            break;
        }

        case 30: {
            actor_p = mActorMngr[4].getActorP();
            JUT_ASSERT(0xE48, NULL != actor_p);
            mJntAnm.lookActor(actor_p, 0.0f, 0);
            if (cLib_calcTimer(&mEventTimer) == 0) {
                retval = 1;
            }

            break;
        }

        case 32:
        case 91: {
            actor_p = mActorMngr[4].getActorP();
            JUT_ASSERT(0xE48, NULL != actor_p);
            mJntAnm.lookActor(actor_p, 0.0f, 0);
            my_vals[0] = msgNo_val;
            if (talkProc(my_vals, send_val, local_4c, false)) {
                if (msgNo_val == 0 && msgNo2_val == 0) {
                    if (mFlow.checkEndFlow()) {
                        retval = 1;
                    }
                } else {
                    retval = 1;
                }
            }

            if (retval != 0 && prm_val == 91) {
                setAngle(home.angle.y);
            }

            if (send_val == 0) {
                retval = 1;
            }

            break;
        }

        case 33: {
            retval = 1;
            break;
        }

        case 40: {
            actor_p = mActorMngr[4].getActorP();
            JUT_ASSERT(0xE48, NULL != actor_p);
            mJntAnm.lookActor(actor_p, 0.0f, 0);
            if (cLib_calcTimer(&mEventTimer) == 0) {
                retval = 1;
            }

            break;
        }

        case 41: {
            actor_p = mActorMngr[4].getActorP();
            JUT_ASSERT(0xE48, NULL != actor_p);
            mJntAnm.lookActor(actor_p, 0.0f, 0);
            my_vals[0] = msgNo_val;
            if (talkProc(my_vals, 0, local_4c, false)) {
                if (msgNo_val == 0) {
                    if (mFlow.checkEndFlow()) {
                        retval = 1;
                    }
                } else {
                    retval = 1;
                }
            }

            if (retval != 0) {
                actor_p = mActorMngr[1].getActorP();
                JUT_ASSERT(0xE48, NULL != actor_p);
                dComIfGp_getEvent().setPt2(actor_p);
            }

            break;
        }

        case 42:
        case 43:
        case 92: {
            actor_p = mActorMngr[1].getActorP();
            JUT_ASSERT(0xE48, NULL != actor_p);
            mJntAnm.lookActor(actor_p, -40.0f, 0);
            my_vals[0] = msgNo_val;
            if (talkProc(my_vals, send_val, local_4c, false)) {
                if (msgNo_val == 0) {
                    if (mFlow.checkEndFlow()) {
                        retval = 1;
                    }
                } else {
                    retval = 1;
                }
            }

            if (retval != 0 && prm_val == 92) {
                if (dComIfG_getTimerPtr() != NULL && dComIfG_getTimerMode() == 0 && dTimer_isStart() == 0) {
                    dComIfG_TimerStart(0, 0);
                }

                setAngle(home.angle.y);
            }

            if (prm_val == 43 && send_val == 0) {
                retval = 1;
            }

            break;
        }

        case 50: {
            if (cLib_calcTimer(&mEventTimer) == 0 && talkProc(NULL, false, local_4c, false) && mFlow.checkEndFlow()) {
                retval = 1;
            }

            break;
        }

        case 60: {
            retval = 1;
            break;
        }

        case 61:
        case 62:
        case 63: {
            mJntAnm.lookNone(0);
            if (prm_val == 63) {
                mJntAnm.lookPlayer(dComIfGp_getEventManager().getIsAddvance(arg0));
            }

            my_vals[0] = msgNo_val;
            my_vals[1] = msgNo2_val;
            if (talkProc(my_vals, 0, local_4c, false)) {
                if (msgNo_val == 0 && msgNo2_val == 0) {
                    if (mFlow.checkEndFlow()) {
                        retval = 1;
                    }
                } else {
                    retval = 1;
                }
            }

            if (retval != 0) {
                if (prm_val == 61) {
                    my_s_vec.y = fopAcM_searchActorAngleY(daPy_getPlayerActorClass(), this);
                    daPy_getPlayerActorClass()->setPlayerPosAndAngle(&daPy_getPlayerActorClass()->current.pos,
                                        my_s_vec.y, 0);
                    dComIfGp_getEvent().setPt2(this);
                    setAngle(fopAcM_searchPlayerAngleY(this));
                } else if (prm_val == 63) {
                    if (!daNpcT_chkEvtBit(0x4B)) {
                        dComIfGs_offSaveSwitch(0x6D);
                    }

                    int evt_ret = mFlow.getEventId(&evt_id);
                    switch (evt_ret) {
                        case 1: {
                            if (mItemPartnerId == -1) {
                                mItemPartnerId = fopAcM_createItemForPresentDemo(&current.pos, evt_id, 0, -1, -1, NULL, NULL);
                            }

                            if (fopAcM_IsExecuting(mItemPartnerId)) {
                                mEvtNo = 1;
                                field_0x1135 = 1;
                                evtChange();
                            }

                            retval = 0;
                            break;
                        }

                        case 8: {
                            if (evt_id == 1) {
                                mEvtNo = 11;
                                evtChange();
                            }

                            retval = 0;
                            break;
                        }
                    }
                }
            }

            break;
        }

        case 65: {
            retval = 1;
            break;
        }

        case 66: {
            if (talkProc(my_vals, 0, local_4c, 0) && mFlow.checkEndFlow()) {
                retval = 1;
            }

            break;
        }

        case 70: {
            mJntAnm.lookPlayer(0);
            if (mPlayerAngle != mCurAngle.y) {
                step(mPlayerAngle, 9, 7, 0xF, 0);
            } else {
                retval = 1;
            }

            break;
        }

        case 71: {
            mJntAnm.lookPlayer(0);
            if (talkProc(NULL, 0, NULL, 0)) {
                int evt_ret = mFlow.getEventId(&evt_id);
                if (evt_ret == 8) {
                    switch (evt_id) {
                        case 1: {
                            mEvtNo = 11;
                            evtChange();
                            break;
                        }

                        case 2: {
                            mEvtNo = 10;
                            evtChange();
                            break;
                        }

                        default: {
                            if (mFlow.checkEndFlow()) {
                                retval = 1;
                            }

                            break;
                        }
                    }
                }
            }

            break;
        }

        case 80: {
            mJntAnm.lookPlayer(0);
            retval = 1;
            break;
        }

        case 90: {
            retval = 1;
            break;
        }

        case 7: {
            mJntAnm.lookPlayer(dComIfGp_getEventManager().getIsAddvance(arg0));
            if (talkProc(NULL, 0, NULL, 0)) {
                retval = 1;
            }

            break;
        }

        case 10:
        case 51: {
            action();
            retval = 1;
            break;
        }

        case 34: {
            actor_p = mActorMngr[4].getActorP();
            JUT_ASSERT(0xE48, NULL != actor_p);
            mJntAnm.lookActor(actor_p, 0.0f, 0);
            my_vals[0] = msgNo_val;
            if (talkProc(my_vals, send_val, local_4c, 0)) {
                if (msgNo_val == 0 && msgNo2_val == 0) {
                    if (mFlow.checkEndFlow()) {
                        retval = 1;
                    }
                } else {
                    retval = 1;
                }
            }

            if (retval != 0 && prm_val == 32) {
                setAngle(home.angle.y);
            }

            if (send_val == 0) {
                retval = 1;
            }

            break;
        }

        case 35: {
            retval = 1;
            break;
        }

        case 44: {
            actor_p = mActorMngr[1].getActorP();
            JUT_ASSERT(0xE48, NULL != actor_p);
            mJntAnm.lookActor(actor_p, -40.0f, 0);
            my_vals[0] = msgNo_val;
            if (talkProc(my_vals, send_val, local_4c, 0)) {
                if (prm_val == 43) {
                    setAngle(home.angle.y);
                    if (dComIfG_getTimerPtr() != NULL && dComIfG_getTimerMode() == 0 && dTimer_isStart() == 0) {
                        dComIfG_TimerStart(0, 0);
                    }
                }

                if (prm_val == 44) {
                    setAngle(fopAcM_searchPlayerAngleY(this));
                }

                retval = 1;
            }

            if (send_val == 0) {
                retval = 1;
            }

            break;
        }

        case 64: {
            if (talkProc(NULL, 0, local_4c, 0)) {
                retval = 1;
            }

            break;
        }
    }

    return retval;
}

int daNpc_Maro_c::cutBokinTalk(int arg0) {
    daTag_Push_c* push_p = (daTag_Push_c*) field_0xba0.getActorP();
    (void) push_p;
    int retval = 0;
    int* param_p = NULL;
    int prm_val = -1;
    param_p = dComIfGp_evmng_getMyIntegerP(arg0, "prm");
    if (param_p != NULL) {
        prm_val = *param_p;
    }

    if (dComIfGp_getEventManager().getIsAddvance(arg0)) {
        switch (prm_val) {
            case 0: {
                mFaceMotionSeqMngr.setNo(7, -1.0f, 0, 0);
                mMotionSeqMngr.setNo(0, -1.0f, 0, 0);
                mJntAnm.lookCamera(0);
                break;
            }
        }
    }

    switch (prm_val) {
        case 0: {
            retval = 1;
            break;
        }
    }

    return retval;
}

int daNpc_Maro_c::cutMarosWhisper(int arg0) {
    fopAc_ac_c* actor_p = NULL;
    cXyz my_vec_0;
    int retval = 0;
    int* param_p = NULL;
    int prm_val = -1;
    int timer_val = 0;
    s16 angle_y;
    param_p = dComIfGp_evmng_getMyIntegerP(arg0, "prm");
    if (param_p != NULL) {
        prm_val = *param_p;
    }

    param_p = dComIfGp_evmng_getMyIntegerP(arg0, "timer");
    if (param_p != NULL) {
        timer_val = *param_p;
    }

    if (dComIfGp_getEventManager().getIsAddvance(arg0)) {
        switch (prm_val) {
            case 0: {
                endChoccai();
                actor_p = mActorMngr[0].getActorP();
                JUT_ASSERT(0xFF8, NULL != actor_p);
                angle_y = fopAcM_searchActorAngleY(daPy_getPlayerActorClass(), actor_p);
                daPy_getPlayerActorClass()->setPlayerPosAndAngle(&daPy_getPlayerActorClass()->current.pos, 
                                                                angle_y, 0);
                break;
            }

            case 1:
            case 2: {
                mEventTimer = timer_val;
                break;
            }

            case 3: {
                mFaceMotionSeqMngr.setNo(9, 0.0f, 0, 0);
                mMotionSeqMngr.setNo(0, 0.0f, 0, 0);
                setAngle(home.angle.y);
                my_vec_0.set(0.0f, 40.0f, 40.0f);
                mDoMtx_stack_c::transS(current.pos);
                mDoMtx_stack_c::YrotM(mCurAngle.y - 0x2800);
                mDoMtx_stack_c::multVec(&my_vec_0, &field_0xd6c);
                mJntAnm.sorasu2(&field_0xd6c, 1);
                initTalk(22, NULL);
                dComIfGp_getEvent().setPtT(this);
                dComIfGp_getEvent().setPt2(this);
            }
        }
    }

    switch (prm_val) {
        case 0: {
            retval = 1;
            break;
        }

        case 1:
        case 2: {
            if (cLib_calcTimer(&mEventTimer) == 0) {
                retval = 1;
            }

            break;
        }

        case 3: {
            if (talkProc(NULL, 0, NULL, 0)) {
                retval = 1;
            }

            break;
        }

        default: {
            retval = 1;
            break;
        }
    }

    return retval;
}

int daNpc_Maro_c::cutTagPush1(int arg0) {
    int* param_p = NULL;
    int prm_val = -1;
    param_p = dComIfGp_evmng_getMyIntegerP(arg0, "prm");
    if (param_p != NULL) {
        prm_val = *param_p;
    }

    if (dComIfGp_getEventManager().getIsAddvance(arg0)) {
        switch (prm_val) {
            case 0: {
                break;
            }

            case 1: {
                mFaceMotionSeqMngr.setNo(9, 0.0f, 0, 0);
                mMotionSeqMngr.setNo(0, 0.0f, 0, 0);
                mJntAnm.lookCamera(0);
                field_0x1132 = 1;
            }
        }
    }

    switch (prm_val) {
        case 0: {
            action();
            mEvtNo = 0;
            return 1;
        }

        case 1: {
            return 1;
        }
    }

    return 1;
}

int daNpc_Maro_c::cutNotGonnaLet(int arg0) {
    fopAc_ac_c* actor_p = NULL;
    cXyz my_vec_0;
    int retval = 0;
    int* param_p = NULL;
    int prm_val = -1;
    param_p = dComIfGp_evmng_getMyIntegerP(arg0, "prm");
    if (param_p != NULL) {
        prm_val = *param_p;
    }

    if (dComIfGp_getEventManager().getIsAddvance(arg0)) {
        switch (prm_val) {
            case 0: {
                endChoccai();
                field_0x1132 = 1;
                break;
            }

            case 1: {
                mFaceMotionSeqMngr.setNo(9, 0.0f, 0, 0);
                mMotionSeqMngr.setNo(4, 0.0f, 0, 0);
                actor_p = mActorMngr[3].getActorP();
                JUT_ASSERT(0x108E, NULL != actor_p);
                mJntAnm.lookActor(actor_p, -40.0f, 0);
                my_vec_0.set(-718.0f, 1331.0f, -2139.0f);
                setPos(my_vec_0);
                setAngle(cM_deg2s(50.0f));
                mStepMode = 0;
                break;
            }

            case 2: {
                mJntAnm.lookPlayer(0);
                break;
            }

            case 4: {
                mFaceMotionSeqMngr.setNo(9, 0.0f, 0, 0);
                mMotionSeqMngr.setNo(0, 0.0f, 0, 0);
                mJntAnm.lookPlayer(0);
                field_0x1132 = 0;
                my_vec_0.set(-511.0f, 1370.0f, -2153.0f);
                setPos(my_vec_0);
                setAngle(cM_deg2s(30.0f));
                break;
            }

            case 20: {
                mFaceMotionSeqMngr.setNo(9, 0.0f, 0, 0);
                mMotionSeqMngr.setNo(0, 0.0f, 0, 0);
                actor_p = mActorMngr[3].getActorP();
                JUT_ASSERT(0x10B0, NULL != actor_p);
                mJntAnm.lookActor(actor_p, -40.0f, 0);
                my_vec_0.set(-718.0f, 1331.0f, -2139.0f);
                setPos(my_vec_0);
                setAngle(cM_deg2s(50.0f));
                mStepMode = 0;
                break;
            }

            case 21: {
                my_vec_0.set(0.0f, 30.0f, 30.0f);
                mDoMtx_stack_c::transS(current.pos);
                mDoMtx_stack_c::YrotM(mCurAngle.y - 0x2800);
                mDoMtx_stack_c::multVec(&my_vec_0, &field_0xd6c);
                mJntAnm.sorasu1(&field_0xd6c, 0);
                break;
            }

            case 31: {
                mFaceMotionSeqMngr.setNo(9, 0.0f, 0, 0);
                mMotionSeqMngr.setNo(0, 0.0f, 0, 0);
                mJntAnm.lookPlayer(1);
                findPlayer();
                setAngle(home.angle.y);
                break;
            }
        }
    }

    switch (prm_val) {
        case 0: {
            action();
            retval = 1;
            break;
        }

        case 1: {
            retval = 1;
            break;
        }

        case 2: {
            if (mCurAngle.y != fopAcM_searchPlayerAngleY(this)) {
                if (step(fopAcM_searchPlayerAngleY(this), 9, 7, 15, 0)) {
                    mFaceMotionSeqMngr.setNo(9, -1.0f, 0, 0);
                    mMotionSeqMngr.setNo(0, -1.0f, 0, 0);
                }
            } else {
                retval = 1;
            }

            break;
        }

        case 3:
        case 4:
        case 20:
        case 21: {
            retval = 1;
            break;
        }

        case 30: {
            action();
            retval = 1;
            break;
        }

        case 31: {
            retval = 1;
            break;
        }

        default: {
            retval = 1;
            break;
        }
    }

    return retval;
}

int daNpc_Maro_c::cutTalkToKakashi(int arg0) {
    fopAc_ac_c* actor_p = NULL;
    cXyz my_vec_0;
    cXyz temp;
    int retval = 0;
    int* param_p = NULL;
    int prm_val = -1;
    s16 angle_y;
    param_p = dComIfGp_evmng_getMyIntegerP(arg0, "prm");
    if (param_p != NULL) {
        prm_val = *param_p;
    }

    if (dComIfGp_getEventManager().getIsAddvance(arg0)) {
        switch (prm_val) {
            case 0: {
                my_vec_0.set(0.0f, 0.0f, 70.0f);
                mDoMtx_stack_c::transS(current.pos);
                mDoMtx_stack_c::YrotM(mCurAngle.y);
                mDoMtx_stack_c::multVec(&my_vec_0, &temp);
                mDoMtx_stack_c::transS(temp);
                mDoMtx_stack_c::YrotM(cLib_targetAngleY(&temp, &daPy_getPlayerActorClass()->current.pos));
                my_vec_0.set(0.0f, 500.0f, 150.0f);
                mDoMtx_stack_c::multVec(&my_vec_0, &temp);
                mGndChk.SetPos(&temp);
                temp.y = dComIfG_Bgsp().GroundCross(&mGndChk);
                JUT_ASSERT(0xDA9, -G_CM3D_F_INF != temp.y);
                angle_y = cLib_targetAngleY(&temp, &current.pos);
                daPy_getPlayerActorClass()->setPlayerPosAndAngle(&temp, angle_y, 0);
                break;
            }

            case 1: {
                mFaceMotionSeqMngr.setNo(9, 0.0f, 0, 0);
                mMotionSeqMngr.setNo(0, 0.0f, 0, 0);
                actor_p = mActorMngr[0].getActorP();
                JUT_ASSERT(0x1133, NULL != actor_p);
                mJntAnm.lookActor(actor_p, -40.0f, 1);
                initTalk(mFlowNodeNo, NULL);
                break;
            }
        }
    }

    switch (prm_val) {
        case 0: {
            mDamageTimer = 1;
            action();
            retval = 1;
            break;
        }

        case 1: {
            if (talkProc(NULL, 0, NULL, 0)) {
                retval = 1;
            }

            break;
        }

        default: {
            retval = 1;
            break;
        }
    }

    return retval;
}

int daNpc_Maro_c::wait(void* param_0) {
    fopAc_ac_c* actor_p = NULL;
    switch (mMode) {
        case 0:
        case 1: {
            if (mStagger.checkStagger() == 0) {
                mFaceMotionSeqMngr.setNo(9, -1.0f, 0, 0);
                mMotionSeqMngr.setNo(0, -1.0f, 0, 0);
                mMode = 2;
            }
        }

        case 2: {
            switch (mType) {
                case 0: {
                    if (daNpcT_chkEvtBit(0x1F) && mHide) {
                        fopAcM_delete(this);
                        return 1;
                    }

                    break;
                }

                case 3: {
                    actor_p = (daNpc_Len_c*)mActorMngr[8].getActorP();
                    if (actor_p != NULL &&
                        ((daNpc_Len_c*) actor_p)->checkStartDemo13StbEvt(
                            this, daNpc_Maro_Param_c::m.common.box_min_x, daNpc_Maro_Param_c::m.common.box_min_y,
                            daNpc_Maro_Param_c::m.common.box_min_z, daNpc_Maro_Param_c::m.common.box_max_x,
                            daNpc_Maro_Param_c::m.common.box_max_y, daNpc_Maro_Param_c::m.common.box_max_z,
                            daNpc_Maro_Param_c::m.common.box_offset))
                    {
                        mEvtNo = 7;
                        field_0x1133 = 1;
                    }

                    if (field_0x1133 && daNpcT_chkEvtBit(0x3C) && dComIfGp_event_runCheck() == 0) {
                        field_0x1133 = 0;
                    }

                    break;
                }

                case 11: {
                    if (!daNpcT_chkEvtBit(0x266)) {
                        mSpeakEvent = true;
                        field_0xe33 = 1;
                    }

                    break;
                }

                case 12: {
                    if (daNpcT_chkEvtBit(0x25C)) {
                        fopAcM_delete(this);
                        return 1;
                    }

                    break;
                }
            }

            if (mStagger.checkStagger() == 0) {
                if (mType == 3) {
                    mJntAnm.lookNone(0);
                    if (getBitSW() != 0xFF) {
                        if (dComIfGs_isSwitch(getBitSW(), fopAcM_GetRoomNo(this))) {
                            field_0xd6c.setall(0.0f);
                            mJntAnm.lookPos(&field_0xd6c, 0);
                        }
                    }

                    attention_info.flags = 0;
                } else {
                    switch (mType) {
                        case 1:
                        case 2: {
                            mPlayerActorMngr.remove();
                            break;
                        }

                        case 4:
                        case 5: {
                            mPlayerActorMngr.remove();
                            if (dComIfGs_isTmpBit(0xD02)) {
                                mPlayerActorMngr.entry(daPy_getPlayerActorClass());
                            }

                            mJntAnm.lookNone(0);
                            break;
                        }

                        case 10: {
                            mPlayerActorMngr.remove();
                            break;
                        }
                    }

                    if (mPlayerActorMngr.getActorP() != NULL) {
                        if (mType != 4 && mType != 5) {
                            mJntAnm.lookPlayer(0);
                            if (chkActorInSight(mPlayerActorMngr.getActorP(), mAttnFovY, mCurAngle.y) == 0) {
                                mJntAnm.lookNone(0);
                            }

                            if (srchPlayerActor() == 0 && home.angle.y == mCurAngle.y) {
                                mMode = 1;
                            }
                        }
                    } else {
                        mJntAnm.lookNone(0);
                        if (home.angle.y != mCurAngle.y) {
                            if (field_0xe34) {
                                if (step(home.angle.y, 9, 7, 15, 0)) {
                                    mMode = 1;
                                }
                            } else {
                                setAngle(home.angle.y);
                                mMode = 1;
                            }

                            attention_info.flags = 0;
                        } else if (mTwilight == 0 && mType != 4 && mType != 5) {
                            srchPlayerActor();
                        }
                    }

                    if (mType == 11) {
                        if (mJntAnm.getMode() == 1) {
                            dComIfGs_setTmpReg(0xFBFFU, 2);
                        } else {
                            dComIfGs_setTmpReg(0xFBFFU, 1);
                        }

                        if (mPlayerActorMngr.getActorP() == NULL) {
                            dComIfGs_setTmpReg(0xFBFFU, 0);
                        }
                    }
                }

                switch (mJntAnm.getMode()) {
                    case 0: {
                        switch (mType) {
                            case 1:
                            case 2: {
                                actor_p = mActorMngr[0].getActorP();
                                if (actor_p != NULL) {
                                    mJntAnm.lookActor(actor_p, -40.0f, 0);
                                }

                                break;
                            }

                            case 4:
                            case 5: {
                                if (dComIfGs_isTmpBit(0xE40)) {
                                    mJntAnm.lookPlayer(0);
                                }

                                break;
                            }

                            case 10: {
                                if (daNpcT_chkTmpBit(0x64)) {
                                    field_0x1139 = 1;
                                    home.angle.y = -0x8000;
                                }

                                if (field_0x1139) {
                                    mJntAnm.lookCamera(0);
                                } else {
                                    actor_p = mActorMngr[2].getActorP();
                                    if (actor_p != NULL) {
                                        mJntAnm.lookActor(actor_p, -40.0f, 0);
                                    }
                                }

                                break;
                            }
                        }

                        break;
                    }
                }

                switch (mType) {
                    case 4:
                    case 5:
                    case 10: {
                        attention_info.flags = 0;
                        break;
                    }
                }
            }
        }

        case 3: {
            break;
        }
    }

    return 1;
}

int daNpc_Maro_c::swdTutorial(void* ) {
    switch (mMode) {
        case 0:
        case 1: {
            if (mStagger.checkStagger() == 0) {
                mFaceMotionSeqMngr.setNo(9, -1.0f, 0, 0);
                mMotionSeqMngr.setNo(0, -1.0f, 0, 0);
                mMode = 2;
            }
        }
        // fallthrough intentional
        case 2: {
            if (mStagger.checkStagger() == 0) {
                mJntAnm.lookPlayer(0);
                if (home.angle.y != mCurAngle.y) {
                    if (field_0xe34) {
                        if (step(home.angle.y, 9, 7, 15, 0)) {
                            mMode = 1;
                        }
                    } else {
                        setAngle(home.angle.y);
                        mMode = 1;
                    }
                }
            }

            attention_info.flags = 0;
            break;
        }

        case 3: {
            break;
        }
    }

    return 1;
}

int daNpc_Maro_c::talk_withTaro(void* ) {
    switch (mMode) {
        case 0:
        case 1: {
            mFaceMotionSeqMngr.setNo(9, -1.0f, 0, 0);
            mMotionSeqMngr.setNo(0, -1.0f, 0, 0);
            mMode = 2;
        }
        // fallthrough intentional
        case 2: {
            if (mStagger.checkStagger() == 0) {
                mJntAnm.lookNone(0);
                if (field_0x1132 || srchPlayerActor()) {
                    mJntAnm.lookPlayer(0);
                }

                if (home.angle.y != mCurAngle.y) {
                    if (step(home.angle.y, 9, 7, 15, 0)) {
                        mMode = 1;
                    }
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

int daNpc_Maro_c::choccai(void* ) {
    fopAc_ac_c* actor_p = NULL;
    switch (mMode) {
        case 0:
        case 1: {
            mFaceMotionSeqMngr.setNo(9, -1.0f, 0, 0);
            mMotionSeqMngr.setNo(0, -1.0f, 0, 0);
            mMode = 2;
        }
        // fallthrough intentional
        case 2: {
            if (mStagger.checkStagger() == 0) {
                mJntAnm.lookNone(0);
                actor_p = mActorMngr[1].getActorP();
                if (actor_p != NULL) {
                    s16 angle_y = fopAcM_searchActorAngleY(this, actor_p) + 0xA9E;
                    if (angle_y != mCurAngle.y) {
                        step(angle_y, 9, 7, 15, 0);
                    }

                    if (checkNowMotionIsChoccai() == 0) {
                        mFaceMotionSeqMngr.setNo(2, -1.0f, 0, 0);
                        mMotionSeqMngr.setNo(5, -1.0f, 0, 0);
                    } else if (checkEndMotionIsChoccai()) {
                        endChoccai();
                    }
                }

                attention_info.flags = 0;
            }

            break;
        }

        case 3: {
            break;
        }
    }

    return 1;
}

int daNpc_Maro_c::tend(void* ) {
    daTag_ShopItem_c* actor_p = (daTag_ShopItem_c*) mActorMngr[5].getActorP();
    int do_step = 0;
    switch (mMode) {
        case 0:
        case 1: {
            if (actor_p == NULL) {
                break;
            }

            if (dComIfGs_isSaveSwitch(actor_p->getSwitchBit1()) == 0) {
                mFaceMotionSeqMngr.setNo(7, -1.0f, 0, 0);
                mMotionSeqMngr.setNo(2, -1.0f, 0, 0);
            } else {
                mFaceMotionSeqMngr.setNo(7, -1.0f, 0, 0);
                mMotionSeqMngr.setNo(0, -1.0f, 0, 0);
            }

            mMode = 2;
        }
        // fallthrough intentional
        case 2: {
            mJntAnm.lookNone(0);
            if (field_0x112c == 2) {
                mSpeakEvent = true;
                field_0xe33 = 1;
            }

            if (dComIfGs_isSaveSwitch(actor_p->getSwitchBit1()) == 0
                && daNpcT_chkDoBtnIsSpeak(this)) {
                dComIfGp_setDoStatusForce(0x39, 2);
            }

            if (srchPlayerActor()) {
                mJntAnm.lookPlayer(0);
                do_step = checkStep();
            } else {
                mJntAnm.lookNone(0);
                do_step = home.angle.y != mCurAngle.y;
            }

            if (do_step && step(home.angle.y, 9, 7, 15, 0)) {
                mMode = 1;
            }

            break;
        }

        case 3: {
            break;
        }
    }

    return 1;
}

int daNpc_Maro_c::arrowTutorial(void* ) {
    daTag_EvtArea_c* actor_p = NULL;
    int unused = 0;
    (void) unused;
    switch (mMode) {
        case 0:
        case 1: {
            if (mStagger.checkStagger() == 0) {
                mFaceMotionSeqMngr.setNo(9, -1.0f, 0, 0);
                mMotionSeqMngr.setNo(0, -1.0f, 0, 0);
                mMode = 2;
            }
        }
        // fallthrough intentional
        case 2: {
            mHide = daNpcT_chkTmpBit(0x3C) == 0;
            if (!mHide) {
                if (field_0x1135) {
                    mEvtNo = 12;
                    field_0xe33 = 1;
                } else {
                    actor_p = (daTag_EvtArea_c*) mActorMngr[6].getActorP();
                    if (actor_p != NULL && !(daPy_getPlayerActorClass()->eventInfo.chkCondition(dEvtCnd_CANTALK_e) == 0)) {
                        if (actor_p->chkPointInArea(daPy_getPlayerActorClass()->current.pos) == 0) {
                            mEvtNo = 9;
                        }
                    }

                    if (field_0x1137 || field_0x1136) {
                        mSpeakEvent = true;
                    }
                }

                if (daNpcT_chkEvtBit(0x47) == 0 && dComIfG_getTimerPtr() != NULL
                    && dComIfG_getTimerMode() == 0 && dComIfG_getTimerPtr()->getRestTimeMs() == 0) {
                    field_0x1136 = 1;
                }
            }

            if (mStagger.checkStagger() == 0) {
                mJntAnm.lookPlayer(0);
                if (home.angle.y != mCurAngle.y && field_0x1135 == 0) {
                    if (field_0xe34) {
                        if (step(home.angle.y, 9, 7, 15, 0)) {
                            mMode = 1;
                        }
                    } else {
                        setAngle(home.angle.y);
                        mMode = 1;
                    }

                    attention_info.flags = 0;
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

int daNpc_Maro_c::talk(void* param_0) {
    switch (mMode) {
        case 0:
        case 1: {
            if (mStagger.checkStagger() == 0) {
                initTalk((field_0x1137) ? 26 : (field_0x1136) ? 28 : mFlowNodeNo, NULL);
                mMode = 2;
            }
        }
        // fallthrough intentional
        case 2: {
            if (mStagger.checkStagger() == 0) {
                if (mTwilight != 0 || mPlayerAngle == mCurAngle.y) {
                    if (talkProc(NULL, 0, NULL, 0)) {
                        int evt_id;
                        int get_ret = mFlow.getEventId(&evt_id);
                        if (get_ret == 8 && evt_id == 1) {
                            mEvtNo = 11;
                            evtChange();
                        } else if (mFlow.checkEndFlow()) {
                            mPlayerActorMngr.entry(daPy_getPlayerActorClass());
                            dComIfGp_event_reset();
                            mMode = 3;
                        }

                        field_0x1136 = 0;
                        field_0x1137 = 0;
                    }

                    mJntAnm.lookPlayer(0);
                    if (mTwilight) {
                        mJntAnm.lookNone(0);
                    }
                } else {
                    mJntAnm.lookPlayer(0);
                    step(mPlayerAngle, 9, 7, 15, 0);
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

int daNpc_Maro_c::shop(void* param_0) {
    int retval = 0;
    switch (mMode) {
        case 0:
        case 1: {
            if (field_0x112c == 2) {
                shop_init(true);
            } else {
                mShopCamAction.Save();
                initTalk(mFlowNodeNo, NULL);
                shop_init(false);
            }

            mJntAnm.lookCamera(0);
            mMode = 2;
        }
        // fallthrough intentional
        case 2: {
            field_0x112c = shop_process(this, &mFlow);
            if (field_0x112c) {
                mPlayerActorMngr.entry(daPy_getPlayerActorClass());
                dComIfGp_event_reset();
                field_0x1130 = 1;
                mMode = 3;
            }

            break;
        }

        case 3: {
            break;
        }
    }

    return retval;
}

static int daNpc_Maro_Create(void* i_this) {
    return static_cast<daNpc_Maro_c*>(i_this)->create();
}

static int daNpc_Maro_Delete(void* i_this) {
    return static_cast<daNpc_Maro_c*>(i_this)->Delete();
}

static int daNpc_Maro_Execute(void* i_this) {
    return static_cast<daNpc_Maro_c*>(i_this)->Execute();
}

static int daNpc_Maro_Draw(void* i_this) {
    return static_cast<daNpc_Maro_c*>(i_this)->Draw();
}

static int daNpc_Maro_IsDelete(void* i_this) {
    return 1;
}

static daNpc_Maro_Param_c l_HIO;

static actor_method_class daNpc_Maro_MethodTable = {
    (process_method_func)daNpc_Maro_Create,
    (process_method_func)daNpc_Maro_Delete,
    (process_method_func)daNpc_Maro_Execute,
    (process_method_func)daNpc_Maro_IsDelete,
    (process_method_func)daNpc_Maro_Draw,
};

extern actor_process_profile_definition g_profile_NPC_MARO = {
  fpcLy_CURRENT_e,         // mLayerID
  7,                       // mListID
  fpcPi_CURRENT_e,         // mListPrio
  PROC_NPC_MARO,           // mProcName
  &g_fpcLf_Method.base,   // sub_method
  sizeof(daNpc_Maro_c),    // mSize
  0,                       // mSizeOther
  0,                       // mParameters
  &g_fopAc_Method.base,    // sub_method
  353,                     // mPriority
  &daNpc_Maro_MethodTable, // sub_method
  0x00040107,              // mStatus
  fopAc_NPC_e,             // mActorType
  fopAc_CULLBOX_CUSTOM_e,  // cullType
};
