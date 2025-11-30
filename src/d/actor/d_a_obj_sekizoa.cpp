#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_sekizoa.h"
#include "Z2AudioLib/Z2Instances.h"
#include "d/actor/d_a_obj_sekidoor.h"
#include "d/actor/d_a_obj_smtile.h"
#include "d/actor/d_a_tag_evtarea.h"
#include "d/actor/d_a_tag_kmsg.h"
#include "dolphin/types.h"
#include "f_op/f_op_actor_mng.h"
#include "f_op/f_op_msg.h"


static daNpc_GetParam1 l_bmdData[9] = {
    {36, 1}, {36, 1}, {35, 1}, {37, 1}, {38, 1}, {3, 2}, {3, 2}, {4, 2}, {5, 2},
};

static daNpcT_evtData_c l_evtList[10] = {
    {"", 1},     {"NO_RESPONSE", 1}, {"START", 1}, {"RESTART", 1}, {"TURN", 1},
    {"JUMP", 1}, {"GOAL", 1},        {"EXIT", 1},  {"FREE", 1},    {"EXTINCTION", 1},
};

static char* l_resNameList[3] = {
    "",
    "sekizoA",
    "seki_1k",
};

static s8 l_loadResPtrn0[2] = {
    0x01,
    0xFF,
};

static s8 l_loadResPtrn1[3] = {
    0x01,
    0x02,
    0xFF,
};

static s8* l_loadResPtrnList[8] = {
    l_loadResPtrn0, l_loadResPtrn0, l_loadResPtrn0, l_loadResPtrn0,
    l_loadResPtrn1, l_loadResPtrn1, l_loadResPtrn1, l_loadResPtrn1,
};

static daNpcT_faceMotionAnmData_c l_faceMotionAnmData = {
    -1, 0, 0, -1, 0, 0, 0,
};

static daNpcT_motionAnmData_c l_motionAnmData[24] = {
    {daObj_Sekizoa_c::ANM_SEKI_WAIT_A, 2, 1, -1, 0, 0, 0},
    {daObj_Sekizoa_c::ANM_SEKI_WAIT_R, 2, 1, -1, 0, 0, 0},
    {daObj_Sekizoa_c::ANM_SEKI_STILL_R, 2, 1, -1, 0, 0, 0},
    {daObj_Sekizoa_c::ANM_SEKI_R_DEMO, 0, 1, -1, 0, 0, 0},
    {daObj_Sekizoa_c::ANM_SEKI_R_LASTDEMO, 0, 1, -1, 0, 0, 0},
    {daObj_Sekizoa_c::ANM_SEKI_WAIT_GAME_R, 2, 1, -1, 0, 0, 0},
    {daObj_Sekizoa_c::ANM_SEKI_HIT_R, 0, 1, -1, 0, 0, 0},
    {daObj_Sekizoa_c::ANM_SEKIR_JUMP, 0, 1, -1, 0, 0, 0},
    {daObj_Sekizoa_c::ANM_SEKIR_JUMP, 0, 1, -1, 0, 0, 0},
    {daObj_Sekizoa_c::ANM_SEKIR_JUMP, 0, 1, -1, 0, 0, 0},
    {daObj_Sekizoa_c::ANM_SEKI_NO_JUMP_R, 0, 1, -1, 0, 0, 0},
    {daObj_Sekizoa_c::ANM_SEKI_STEP_R, 0, 1, -1, 0, 0, 0},
    {daObj_Sekizoa_c::ANM_SEKI_WAIT_L, 2, 1, -1, 0, 0, 0},
    {daObj_Sekizoa_c::ANM_SEKI_STILL_L, 2, 1, -1, 0, 0, 0},
    {daObj_Sekizoa_c::ANM_SEKI_L_DEMO, 0, 1, -1, 0, 0, 0},
    {daObj_Sekizoa_c::ANM_SEKI_L_LASTDEMO, 0, 1, -1, 0, 0, 0},
    {daObj_Sekizoa_c::ANM_SEKI_WAIT_GAME_L, 2, 1, -1, 0, 0, 0},
    {daObj_Sekizoa_c::ANM_SEKI_HIT_L, 0, 1, -1, 0, 0, 0},
    {daObj_Sekizoa_c::ANM_SEKIL_JUMP, 0, 1, -1, 0, 0, 0},
    {daObj_Sekizoa_c::ANM_SEKIL_JUMP, 0, 1, -1, 0, 0, 0},
    {daObj_Sekizoa_c::ANM_SEKIL_JUMP, 0, 1, -1, 0, 0, 0},
    {daObj_Sekizoa_c::ANM_SEKI_NO_JUMP_L, 0, 1, -1, 0, 0, 0},
    {daObj_Sekizoa_c::ANM_SEKI_STEP_L, 0, 1, -1, 0, 0, 0},
    {daObj_Sekizoa_c::ANM_SEKI_WAIT_A, 2, 1, -1, 0, 0, 0}};

static daNpcT_MotionSeqMngr_c::sequenceStepData_c l_faceMotionSequenceData[4] = {
    {0, -1, 0},
    {-1, 0, 0},
    {-1, 0, 0},
    {-1, 0, 0},
};

static daNpcT_MotionSeqMngr_c::sequenceStepData_c l_motionSequenceData[96] = {
    {0, -1, 0},  {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},  {1, -1, 0},  {-1, 0, 0},  {-1, 0, 0},
    {-1, 0, 0},  {2, -1, 0},  {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},  {3, -1, 1},  {-1, 0, 0},
    {-1, 0, 0},  {-1, 0, 0},  {4, -1, 1},  {2, 0, 0},   {-1, 0, 0},  {-1, 0, 0},  {5, -1, 0},
    {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},  {6, 0, 1},   {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},
    {7, -1, 1},  {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},  {8, 0, 1},   {-1, 0, 0},  {-1, 0, 0},
    {-1, 0, 0},  {9, 4, 1},   {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},  {10, -1, 1}, {-1, 0, 0},
    {-1, 0, 0},  {-1, 0, 0},  {11, 4, 1},  {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},  {12, -1, 0},
    {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},  {13, -1, 0}, {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},
    {14, -1, 1}, {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},  {15, -1, 1}, {13, 0, 0},  {-1, 0, 0},
    {-1, 0, 0},  {16, -1, 0}, {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},  {17, 0, 1},  {-1, 0, 0},
    {-1, 0, 0},  {-1, 0, 0},  {18, -1, 1}, {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},  {19, 0, 1},
    {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},  {20, 4, 1},  {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},
    {21, -1, 1}, {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},  {22, 4, 1},  {-1, 0, 0},  {-1, 0, 0},
    {-1, 0, 0},  {23, 0, 0},  {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0}};

char* daObj_Sekizoa_c::mCutNameList[9] = {
    "", "START", "START", "TURN", "JUMP", "GOAL", "EXIT", "FREE", "EXTINCTION",
};

daObj_Sekizoa_c::cutFunc daObj_Sekizoa_c::mCutList[9] = {
    NULL,
    &daObj_Sekizoa_c::cutStart,
    &daObj_Sekizoa_c::cutStart,
    &daObj_Sekizoa_c::cutTurn,
    &daObj_Sekizoa_c::cutJump,
    &daObj_Sekizoa_c::cutGoal,
    &daObj_Sekizoa_c::cutExit,
    &daObj_Sekizoa_c::cutFree,
    &daObj_Sekizoa_c::cutExtinction,
};

static cXyz l_srcPosR(-600.0f, 1000.0f, 1800.0f);

static cXyz l_srcPosL(600.0f, 1000.0f, 1800.0f);

daObj_Sekizoa_Param_c::Data const daObj_Sekizoa_Param_c::m = {
    600.0, -10.0, 1.0, 1100.0, 255.0, 550.0, 100.0, 70.0, 0.0, 0.0,   30.0, 0.0,   30.0,     -30.0,
    45.0,  -45.0, 0.6, 8.0,    0x3,   0x6,   0x5,   0x6,  0.0, 0.0,   0.0,  0.0,   0x3C0008, 0.0,
    0.0,   4.0,   0.0, 0.0,    0.0,   0.0,   0.0,   0.0,  0.0, 600.0, 30.0, 0.004, 0,
};

daObj_Sekizoa_c::~daObj_Sekizoa_c() {
    if (mpMorf[0] != NULL) {
        mpMorf[0]->stopZelAnime();
    }
    if (mpMcaMorf != NULL) {
        mpMcaMorf->stopZelAnime();
    }
    if (mpMorf[1] != NULL) {
        mpMorf[1]->stopZelAnime();
    }
    if (mType == TYPE_0) {
        if (daNpcT_chkTmpBit(0x31)) {
            mDoAud_subBgmStop();
        }
    }
    deleteRes(l_loadResPtrnList[mType], (const char**)l_resNameList);
}


int daObj_Sekizoa_c::create() {
    static u32 const heapSize[8] = {
        0x2390, 0x2380, 0x2390, 0x2380, 0x2350, 0x2340, 0x33E0, 0,
    };

    daNpcT_ct(this, daObj_Sekizoa_c, &l_faceMotionAnmData, l_motionAnmData,
                       l_faceMotionSequenceData, 4, l_motionSequenceData, 4, l_evtList,
                       l_resNameList);

    mType = getType();
    mTwilight = 0;

    int ret = loadRes(l_loadResPtrnList[mType], (const char**)l_resNameList);
    if (ret == cPhs_COMPLEATE_e) {
        if (isDelete()) {
            return cPhs_ERROR_e;
        }
        if (fopAcM_entrySolidHeap(this, createHeapCallBack, heapSize[mType]) == 0) {
            return cPhs_ERROR_e;
        }
        J3DModelData* mpModelData = mpMorf[0]->getModel()->getModelData();
        fopAcM_SetMtx(this, mpMorf[0]->getModel()->getBaseTRMtx());
        fopAcM_setCullSizeBox2(this, mpModelData);
        mSound.init(&current.pos, &eyePos, 3, 1);
        reset();
        mAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1,
                    &mAcchCir, fopAcM_GetSpeed_p(this), fopAcM_GetAngle_p(this),
                    fopAcM_GetShapeAngle_p(this));
        mCcStts.Init(daObj_Sekizoa_Param_c::m.field_0x10, 0,
                        this);

        mCyl.Set(mCcDCyl);
        mCyl.SetStts(&mCcStts);
        mCyl.SetTgHitCallback(tgHitCallBack);

        mCyl2.Set(mCcDCyl);
        mCyl2.SetStts(&mCcStts);
        mCyl2.SetTgHitCallback(tgHitCallBack);

        mAcch.CrrPos(dComIfG_Bgsp());
        mGndChk = mAcch.m_gnd;
        mGroundH = mAcch.GetGroundH();
        if (mGroundH != -1.0e9f) {
            setEnvTevColor();
            setRoomNo();
        }
        mCreating = true;
        Execute();
        mCreating = false;
    }
    return ret;
}


int daObj_Sekizoa_c::CreateHeap() {
    J3DModelData* modelData;
    int arcIdx;
    int fileIdx;

    int bmd_idx = 0;
    switch (mType) {
    case TYPE_4:
    case TYPE_5:
        bmd_idx = 5;
        break;
    case TYPE_6:
        bmd_idx = 2;
        break;
    }

    arcIdx = l_bmdData[bmd_idx].arcIdx;
    fileIdx = l_bmdData[bmd_idx].fileIdx;
    modelData = static_cast<J3DModelData*>(dComIfG_getObjectRes(l_resNameList[arcIdx], fileIdx));
    if (modelData == NULL) {
        return 0;
    }
    mpMorf[0] = new mDoExt_McaMorfSO(modelData, NULL, NULL, NULL, -1, 1.0f, 0, -1, &mSound,
                                        0x80000, 0x11000284);

    if (mpMorf[0] == NULL || mpMorf[0]->getModel() == NULL) {
        return 0;
    }
    if (mType == TYPE_6) {
        int success_create = mInvModel.create(mpMorf[0]->getModel(), 1);
        if (success_create == NULL) {
            return 0;
        }

        mpMorf[1] = new mDoExt_McaMorfSO(modelData, NULL, NULL, NULL, -1, 1.0f, 0, -1,
                                            &mSound, 0x80000, 0x11000284);
        if (mpMorf[1] == NULL || mpMorf[1]->getModel() == NULL) {
            return 0;
        }
    } else {
        switch (mType) {
        case TYPE_0:
            bmd_idx = 3;
            break;
        case TYPE_1:
            bmd_idx = 4;
            break;
        case TYPE_2:
            bmd_idx = 3;
            break;
        case TYPE_3:
            bmd_idx = 4;
            break;
        case TYPE_4:
            bmd_idx = 7;
            break;
        case TYPE_5:
            bmd_idx = 8;
            break;
        default:
            bmd_idx = 3;
            break;
        }
        arcIdx = l_bmdData[bmd_idx].arcIdx;
        fileIdx = l_bmdData[bmd_idx].fileIdx;
        modelData = static_cast<J3DModelData*>(dComIfG_getObjectRes(l_resNameList[arcIdx], fileIdx));
        if (modelData == NULL) {
            return 0;
        }
        mpMcaMorf = new mDoExt_McaMorfSO(modelData, NULL, NULL, NULL, -1, 1.0f, 0, -1, NULL,
                                            0x80000, 0x11000284);
        if (mpMcaMorf == NULL || mpMcaMorf->getModel() == NULL) {
            return 0;
        }
    }

    if (mType == TYPE_6) {
        if (setFaceMotionAnm(0, false) && setMotionAnm(0x17, 0.0f, 0)) {
            return 1;
        }
        return 0;
    } else {
        if (setFaceMotionAnm(0, false) && setMotionAnm(0, 0.0f, 0)) {
            return 1;
        }
    }
    return 0;
}

int daObj_Sekizoa_c::Delete() {
    this->~daObj_Sekizoa_c();
    return 1;
}

int daObj_Sekizoa_c::Execute() {
    return execute();
}

int daObj_Sekizoa_c::Draw() {
    daNpcT_MatAnm_c* mat_anm = mpMatAnm[0];
    if (mat_anm != NULL) {
        J3DModelData* this_01 = mpMorf[0]->getModel()->getModelData();
        J3DMaterial* mpNodePtr = this_01->getMaterialNodePointer(getEyeballMaterialNo());
        mpNodePtr->setMaterialAnm(mat_anm);
    }

    int temp_int = 0;
    if (mType == TYPE_2 || mType == TYPE_3 || mType == TYPE_4 || mType == TYPE_5 || mType == TYPE_6)
    {
        temp_int = 1;
    }
    return daNpcT_c::draw(0, 0, mRealShadowSize, NULL, 0.0f, temp_int, (mType == TYPE_6), 0);
}

int daObj_Sekizoa_c::createHeapCallBack(fopAc_ac_c* i_this) {
    static_cast<daObj_Sekizoa_c*>(i_this)->CreateHeap();
}

void* daObj_Sekizoa_c::srchSekizoa(void* i_actor, void* i_this) {
    if (mFindCount < 50 && i_actor != NULL && i_actor != i_this) {
        if (fopAcM_IsExecuting(fopAcM_GetID(i_actor)) && fopAcM_GetName(i_actor) == PROC_OBJ_SEKIZOA) {
            mFindActorPtrs[mFindCount] = (fopAc_ac_c*)i_actor;
            mFindCount++;
        }
    }
    return NULL;
}


fopAc_ac_c* daObj_Sekizoa_c::getSekizoaP(int param_0) {
    fopAc_ac_c* actor = NULL;
    f32 minDistance = 1.0e9f;
    mFindCount = 0;
    daObj_Sekizoa_c* pfVar4 = (daObj_Sekizoa_c*)fopAcM_Search(srchSekizoa, this);
    for (int i = 0; i < mFindCount; i++) {
        pfVar4 = (daObj_Sekizoa_c*)mFindActorPtrs[i];
        if (param_0 == pfVar4->getType() &&
            fopAcM_searchActorDistance(this, pfVar4) < minDistance)
        {
            minDistance = fopAcM_searchActorDistance(this, mFindActorPtrs[i]);
            actor = mFindActorPtrs[i];
        }
    }
    return actor;
}

fopAc_ac_c* daObj_Sekizoa_c::getKMsgTagP() {
    mFindCount = 0;
    mSrchName = 0x2eb;
    fopAcM_Search((fopAcIt_JudgeFunc)srchActor, this);
    for (int i = 0; i < mFindCount; i++) {
        if (((daTag_KMsg_c*)mFindActorPtrs[i])->getType() == daTag_KMsg_c::KMSG_TYPE_3) {
            return mFindActorPtrs[i];
        }
    }
    return NULL;
}

int daObj_Sekizoa_c::isDelete() {
    bool ret;
    switch (mType) {
    case TYPE_0:
    case TYPE_1:
        ret = false;
        if (getBitSW2() != 0xFF) {
            if (dComIfGs_isSwitch(getBitSW2(), fopAcM_GetRoomNo(this))) {
                ret = true;
            }
        }
        return ret;
    case TYPE_2:
        return false;
    case TYPE_3:
        return false;
    case TYPE_4:
        return false;
    case TYPE_5:
        return false;
    case TYPE_6:
        ret = false;
        if (getBitSW() != 0xFF) {
            if (dComIfGs_isSwitch(getBitSW(), fopAcM_GetRoomNo(this))) {
                ret = true;
            }
        }
        return ret;
    default:
        return false;
    }
}

void daObj_Sekizoa_c::reset() {
    csXyz temp_angle;
    int iVar1 = (u8*)&field_0x117C - (u8*)&mInitFunc;

    if (mpMatAnm[0] != NULL) {
        mpMatAnm[0]->initialize();
    }
    initialize();
    for (int i = 0; i < 8; i++) {
        mActorMngrs[i].initialize();
    }
    memset(&mInitFunc, 0, iVar1);
    temp_angle.setall(0.0f);
    temp_angle.y = home.angle.y;
    switch (mType) {
    case TYPE_0:
        if (daNpcT_chkTmpBit(0x31)) {
            daNpcT_offTmpBit(0x31);
            mSetWolfHowling = true;
        }
        eventInfo.setIdx(0);
        mHide = true;
        break;

    case TYPE_1:
        eventInfo.setIdx(0);
        mHide = true;
        break;

    case TYPE_2:
        eventInfo.setIdx(1);
        break;

    case TYPE_3:
        eventInfo.setIdx(1);
        break;

    case TYPE_4:
        eventInfo.setIdx(1);
        break;

    case TYPE_5:
        eventInfo.setIdx(1);
        break;

    case TYPE_6:
        eventInfo.setIdx(2);
    }
    mReset = true;
    setAngle(temp_angle);
}

void daObj_Sekizoa_c::afterJntAnm(int param_0) {
    if (param_0 == 1) {
        mDoMtx_stack_c::YrotM(mStagger.getAngleZ(1));
        mDoMtx_stack_c::ZrotM(-mStagger.getAngleX(1));
    } else if (param_0 == 3) {
        mDoMtx_stack_c::YrotM(-mStagger.getAngleZ(0));
        mDoMtx_stack_c::ZrotM(mStagger.getAngleX(0));
    }
}


void daObj_Sekizoa_c::setParam() {
    selectAction();
    srchActors();
    attention_info.distances[fopAc_attn_LOCK_e] = 0x12;
    attention_info.distances[fopAc_attn_TALK_e] = attention_info.distances[fopAc_attn_LOCK_e];
    attention_info.distances[fopAc_attn_SPEAK_e] = 0x13;
    attention_info.flags = 0;
    scale.set(daObj_Sekizoa_Param_c::m.field_0x08, daObj_Sekizoa_Param_c::m.field_0x08,
              daObj_Sekizoa_Param_c::m.field_0x08);
    mCcStts.SetWeight(daObj_Sekizoa_Param_c::m.field_0x10);
    mCylH = daObj_Sekizoa_Param_c::m.field_0x14;
    mWallR = daObj_Sekizoa_Param_c::m.field_0x1C;
    mAttnFovY = daObj_Sekizoa_Param_c::m.field_0x50;

    mAcchCir.SetWallR(mWallR);
    mAcchCir.SetWallH(daObj_Sekizoa_Param_c::m.field_0x18);
    mRealShadowSize = daObj_Sekizoa_Param_c::m.field_0x0C;
    mExpressionMorfFrame = daObj_Sekizoa_Param_c::m.field_0x6C;
    mMorfFrames = daObj_Sekizoa_Param_c::m.field_0x44;
    gravity = daObj_Sekizoa_Param_c::m.field_0x04;

    if (mType == TYPE_2 || mType == TYPE_3) {
        gravity = 0.0f;
    }
    if (mReset != 0) {
        cXyz cStack_24;
        csXyz cStack_38;
        cStack_38.setall(0);
        switch (mType) {
        case TYPE_0:
            if (daNpcT_chkEvtBit(0x25E)) {
                cStack_24 = l_srcPosL;
            } else {
                cStack_24 = l_srcPosR;
            }
            break;

        case TYPE_1:
            if (daNpcT_chkEvtBit(0x25E)) {
                cStack_24 = l_srcPosR;
            } else {
                cStack_24 = l_srcPosL;
            }
            break;

        case TYPE_2:
            if (daNpcT_chkEvtBit(0x25E)) {
                cStack_24 = l_srcPosL;
            } else {
                cStack_24 = l_srcPosR;
            }
            break;

        case TYPE_3:
            if (daNpcT_chkEvtBit(0x25E)) {
                cStack_24 = l_srcPosR;
            } else {
                cStack_24 = l_srcPosL;
            }
            break;

        case TYPE_4:
        case TYPE_5:
            mReset = 0;
            break;

        default:
            mReset = 0;
        }
        if (mReset != 0) {
            setPos(cStack_24);
            setAngle(cStack_38.y);
            mReset = 0;
        }
    }
}

BOOL daObj_Sekizoa_c::checkChangeEvt() {
    switch (mType) {
    case TYPE_0:
        mEvtNo = 7;
        evtChange();
        return 1;
    case TYPE_1:
    case TYPE_2:
    case TYPE_3:
    case TYPE_4:
    case TYPE_5:
    case TYPE_6:
    default:
        return 0;
    }
}

void daObj_Sekizoa_c::setAfterTalkMotion() {
    /* empty function */
}

void daObj_Sekizoa_c::srchActors() {
    switch (mType) {
    case TYPE_0:
        if (mActorMngrs[1].getActorP() == NULL) {
            mActorMngrs[1].entry(getSekizoaP(2));
        }
        if (mActorMngrs[2].getActorP() == NULL) {
            mActorMngrs[2].entry(getSekizoaP(3));
        }
        if (mActorMngrs[3].getActorP() == NULL) {
            mActorMngrs[3].entry(getSekizoaP(1));
        }
        if (mActorMngrs[4].getActorP() == NULL) {
            mActorMngrs[4].entry(getNearestActorP(0x134));
        }
        if (mActorMngrs[5].getActorP() == NULL) {
            mActorMngrs[5].entry(getEvtAreaTagP(1, 0));
        }
        if (mActorMngrs[6].getActorP() == NULL) {
            mActorMngrs[6].entry(getEvtAreaTagP(1, 1));
        }
        if (mActorMngrs[7].getActorP() == NULL) {
            mActorMngrs[7].entry(getNearestActorP(0x132));
        }
        return;

    case TYPE_1:
        if (mActorMngrs[3].getActorP() == NULL) {
            mActorMngrs[3].entry(getSekizoaP(0));
        }
        if (mActorMngrs[4].getActorP() == NULL) {
            mActorMngrs[4].entry(getNearestActorP(0x134));
        }
        if (mActorMngrs[5].getActorP() == NULL) {
            mActorMngrs[5].entry(getEvtAreaTagP(1, 0));
        }
        if (mActorMngrs[6].getActorP() == NULL) {
            mActorMngrs[6].entry(getEvtAreaTagP(1, 1));
        }
        if (mActorMngrs[7].getActorP() == NULL) {
            mActorMngrs[7].entry(getNearestActorP(0x132));
            return;
        }
        break;

    case TYPE_2:
    case TYPE_3:
        if (mActorMngrs[3].getActorP() == NULL) {
            mActorMngrs[3].entry(getSekizoaP(0));
            return;
        }
        break;

    case TYPE_4:
    case TYPE_5:
        break;

    case TYPE_6:
        if (mActorMngrs[0].getActorP() == NULL) {
            mActorMngrs[0].entry(getKMsgTagP());
        }
        break;

    default:
        break;
    }
}

BOOL daObj_Sekizoa_c::evtTalk() {
    if (chkAction(&daObj_Sekizoa_c::talk)) {
        (this->*(mExecuteFunc))(0);
    } else {
        mPreItemNo = 0;
        if (dComIfGp_event_chkTalkXY()) {
            if (!dComIfGp_evmng_ChkPresentEnd()) {
                return 1;
            }
            mEvtNo = 1;
            evtChange();
            return 1;
        }
        setAction(&daObj_Sekizoa_c::talk);
    }
    return 1;
}


BOOL daObj_Sekizoa_c::evtCutProc() {
    int staff_id = dComIfGp_getEventManager().getMyStaffId("Sekizoa", this, -1);
    if (mType == TYPE_1 || mType == TYPE_3) {
        staff_id = dComIfGp_getEventManager().getMyStaffId("Sekizob", this, -1);
    }
    if (staff_id != -1) {
        mStaffId = staff_id;
        int actIdx = dComIfGp_getEventManager().getMyActIdx(mStaffId, mCutNameList, 9, 0, 0);
        if ((this->*(mCutList[actIdx]))(mStaffId) != 0) {
            dComIfGp_getEventManager().cutEnd(mStaffId);
        }
        return true;
    } else {
        return false;
    }
}

void daObj_Sekizoa_c::action() {
    if (mCyl.ChkTgHit()) {
        mSound.startCollisionSE(mCyl.GetTgHitObjHitSeID(1), 2);
    } else {
        if (mCyl2.ChkTgHit()) {
            mSound.startCollisionSE(mCyl2.GetTgHitObjHitSeID(1), 2);
        }
    }
    if (mStagger.checkRebirth()) {
        mStagger.initialize();
        mMode = 1;
    }
    if (mInitFunc != NULL) {
        if (mExecuteFunc == mInitFunc) {
            (this->*(mExecuteFunc))(0);
        } else {
            setAction(mInitFunc);
        }
    }
}

void daObj_Sekizoa_c::beforeMove() {
    if (checkHide() || mNoDraw != 0) {
        attention_info.flags = 0;
    }
}


void daObj_Sekizoa_c::setAttnPos() {
    cXyz vec_pos(30.0f, 30.0f, 0.0f);
    Mtx M_stack;

    mStagger.calc(0);
    f32 rad_angle_y = cM_s2rad(mCurAngle.y - field_0xd7e.y);
    mJntAnm.setParam(this, mpMorf[0]->getModel(), &vec_pos, getBackboneJointNo(), getNeckJointNo(),
                     getHeadJointNo(), daObj_Sekizoa_Param_c::m.field_0x24,
                     daObj_Sekizoa_Param_c::m.field_0x20, daObj_Sekizoa_Param_c::m.field_0x2C,
                     daObj_Sekizoa_Param_c::m.field_0x28, daObj_Sekizoa_Param_c::m.field_0x34,
                     daObj_Sekizoa_Param_c::m.field_0x30, daObj_Sekizoa_Param_c::m.field_0x3C,
                     daObj_Sekizoa_Param_c::m.field_0x38, daObj_Sekizoa_Param_c::m.field_0x40, 0.0f,
                     NULL);
    mJntAnm.calcJntRad(0.2f, 1.0f, rad_angle_y);
    setMtx();

    if (mType == TYPE_6) {
        J3DModel* mp_model = mpMorf[1]->getModel();
        mDoMtx_stack_c::transS(current.pos);
        mDoMtx_stack_c::ZXYrotM(mCurAngle);
        mDoMtx_stack_c::scaleM(scale);

        mp_model->setBaseTRMtx(mDoMtx_stack_c::get());
        mp_model->setUserArea((uintptr_t)this);
        mpMorf[1]->modelCalc();
    }
    if (mpMcaMorf != NULL) {
        ((mDoExt_McaMorfSO*)mpMcaMorf)->play(NULL, 0);
        if (mType == TYPE_1 || mType == TYPE_3 || mType == TYPE_5) {
            mDoMtx_stack_c::copy(mpMorf[0]->getModel()->getAnmMtx(7));
        } else {
            mDoMtx_stack_c::copy(mpMorf[0]->getModel()->getAnmMtx(13));
        }
        cMtx_copy(mDoMtx_stack_c::get(), M_stack);
        mpMcaMorf->getModel()->setBaseTRMtx(M_stack);
        ((mDoExt_McaMorfSO*)mpMcaMorf)->modelCalc();
    }
    mDoMtx_stack_c::copy(mpMorf[0]->getModel()->getAnmMtx(getHeadJointNo()));
    mDoMtx_stack_c::multVec(&vec_pos, &eyePos);
    mJntAnm.setEyeAngleX(eyePos, 1.0f, 0);
    mJntAnm.setEyeAngleY(eyePos, mCurAngle.y, 1, 1.0f, 0);
    attention_info.position = current.pos;
    attention_info.position.y += daObj_Sekizoa_Param_c::m.field_0x00;
}


void daObj_Sekizoa_c::setCollision() {
    u32 temp_1;
    u32 temp_2;
    u32 temp_3;
    cXyz temp_vec2;

    if (mHide == 0) {
        temp_1 = 0x79;
        temp_2 = 0xd8fafdbf;
        temp_3 = 0x1f;
        if (daNpcT_chkTmpBit(0x31)) {
            temp_1 = 0x69;
        } else if (dComIfGp_event_runCheck()) {
            temp_2 = 0;
            temp_3 = 0;
        } else if (mTwilight != 0) {
            temp_2 = 0;
            temp_3 = 0;
        } else if (mStagger.checkStagger() != 0) {
            temp_2 = 0;
            temp_3 = 0;
        }
        if (mType == TYPE_2 || mType == TYPE_3 || mType == TYPE_4 || mType == TYPE_5) {
            cXyz temp_vec;
            if (mType == TYPE_2 || mType == TYPE_4) {
                temp_vec.set(-160.0f, 0.0f, 120.0f);
            } else {
                temp_vec.set(160.0f, 0.0f, 120.0f);
            }
            mDoMtx_stack_c::YrotS(mCurAngle.y);
            mDoMtx_stack_c::multVec(&temp_vec, &temp_vec);
            f32 height = mCylH;
            temp_vec2 = current.pos + temp_vec;
            mCyl2.SetCoSPrm(temp_1);
            mCyl2.SetTgType(temp_2);
            mCyl2.SetTgSe(9);
            mCyl2.SetTgHitMark(CcG_Tg_UNK_MARK_2);
            mCyl2.SetTgSPrm(temp_3);
            mCyl2.OnTgShield();
            mCyl2.OnTgNoConHit();
            mCyl2.SetH(height);
            mCyl2.SetR(10.0f);
            mCyl2.SetC(temp_vec2);
            dComIfG_Ccsp()->Set(&mCyl2);
        }
        mCyl.SetCoSPrm(temp_1);
        mCyl.SetTgType(temp_2);
        mCyl.SetTgSe(9);
        mCyl.SetTgHitMark(CcG_Tg_UNK_MARK_2);
        mCyl.SetTgSPrm(temp_3);
        mCyl.OnTgShield();
        mCyl.OnTgNoConHit();
        temp_vec2.set(0.0f, 0.0f, 0.0f);
        f32 height = mCylH;
        f32 radius = mWallR;
        if (mType == TYPE_6) {
            temp_vec2.set(0.0f, 0.0f, -40.0f);
            radius = 140.0f;
        }
        mDoMtx_stack_c::YrotS(mCurAngle.y);
        mDoMtx_stack_c::multVec(&temp_vec2, &temp_vec2);
        temp_vec2 += current.pos;
        mCyl.SetH(height);
        mCyl.SetR(radius);
        mCyl.SetC(temp_vec2);
        dComIfG_Ccsp()->Set(&mCyl);
    }
    mCyl2.ClrCoHit();
    mCyl.ClrCoHit();
    mCyl2.ClrTgHit();
    mCyl.ClrTgHit();
}

int daObj_Sekizoa_c::drawDbgInfo() {
    return 0;
}

void daObj_Sekizoa_c::drawOtherMdl() {
    if (mType == TYPE_6) {
        J3DModel* mp_model = mpMorf[1]->getModel();
        mp_model->getModelData();
        g_env_light.settingTevStruct(0, &current.pos, &tevStr);
        g_env_light.setLightTevColorType_MAJI(mp_model, &tevStr);
        mpMorf[1]->entryDL();
    }
    if (mpMcaMorf != NULL && field_0x1179 != 0) {
        J3DModel* mp_model = mpMcaMorf->getModel();
        J3DModelData* mp_model_data = mp_model->getModelData();
        g_env_light.setLightTevColorType_MAJI(mp_model_data, &tevStr);
        if (mAnmFlags & ANM_PLAY_BTK) {
            mBtkAnm.entry(mp_model_data);
        }
        if (mAnmFlags & ANM_PLAY_BRK) {
            mBrkAnm.entry(mp_model_data);
        }
        mpMcaMorf->entryDL();
        if (mAnmFlags & ANM_PLAY_BRK) {
            mBrkAnm.remove(mp_model_data);
        }
        if (mAnmFlags & ANM_PLAY_BTK) {
            mBtkAnm.remove(mp_model_data);
        }
        dComIfGd_addRealShadow(mShadowKey, mp_model);
    }
}


int daObj_Sekizoa_c::setYariAnm(int i_frame, int i_mode, f32 i_morf) {
    static struct {
        u32 param_0;
        u32 param_1;
    } yariAnmData[24] = {
        {-1, 0},
        {ANM_YARIA_STILL_L, 1},
        {ANM_YARIA_STILL_L, 1},
        {ANM_YARIA_STILL_L, 1},
        {ANM_YARIA_STILL_L, 1},
        {ANM_YARIA_GAME, 1},
        {ANM_YARIA_GAME, 1},
        {ANM_YARIA_GAME, 1},
        {ANM_YARIA_GAME, 1},
        {ANM_YARIA_GAME, 1},
        {ANM_YARIA_GAME, 1},
        {ANM_YARIA_GAME, 1},
        {ANM_YARIB_STILL_R, 1},
        {ANM_YARIB_STILL_R, 1},
        {ANM_YARIB_STILL_R, 1},
        {ANM_YARIB_STILL_R, 1},
        {ANM_YARIB_GAME, 1},
        {ANM_YARIB_GAME, 1},
        {ANM_YARIB_GAME, 1},
        {ANM_YARIB_GAME, 1},
        {ANM_YARIB_GAME, 1},
        {ANM_YARIB_GAME, 1},
        {ANM_YARIB_GAME, 1},
        {0, 0},

    };

    s32 anm_data;
    J3DAnmTransform* anim = NULL;
    field_0x1179 = 0;
    if (mpMcaMorf != NULL) {
        anm_data = yariAnmData[i_frame].param_0;
        if (anm_data > 0) {
            anim = getTrnsfrmKeyAnmP(l_resNameList[yariAnmData[i_frame].param_1], anm_data);
        }
        if (anim != NULL) {
            ((mDoExt_McaMorfSO*)mpMcaMorf)->setAnm(anim, i_mode, i_morf, 1.0f, 0.0f, -1.0f);
            field_0x1179 = 1;
        }
    }
    return 1;
}

void daObj_Sekizoa_c::drawGhost() {
    J3DModel* ghost_model = mpMorf[0]->getModel();
    if (mType == TYPE_6) {
        g_env_light.settingTevStruct(7, &current.pos, &tevStr);
        g_env_light.setLightTevColorType_MAJI(ghost_model->getModelData(), &tevStr);
        mInvModel.entryDL(NULL);
    } else {
        g_env_light.settingTevStruct(16, &current.pos, &tevStr);
        g_env_light.setLightTevColorType_MAJI(ghost_model->getModelData(), &tevStr);
        dComIfGd_setListBG();
        mpMorf[0]->entryDL();
        dComIfGd_setList();
    }
}

bool daObj_Sekizoa_c::afterSetMotionAnm(int i_frame, int i_mode, f32 i_morf, int param_3) {
    f32 var1;
    static struct {
        int field_0x0;
        u32 field_0x4;
        u32 field_0x8;
    } brkAnmData[6] = {
        {0x2A, 0, 1}, {0x2B, 0, 1}, {0x2C, 0, 1}, {0x2D, 0, 1}, {0x2E, 0, 1}, {0x29, 0, 1},
    };

    static struct {
        int field_0x0;
        u32 field_0x4;
        u32 field_0x8;
    } btkAnmData[4] = {
        {0x32, 0, 1},
        {-1, 0, 0},
        {-1, 0, 0},
        {0x31, 0, 1},
    };

    J3DAnmTextureSRTKey* anm_text = NULL;
    J3DAnmTevRegKey* anm_tev = NULL;
    if (mCreating == 1) {
        var1 = 0.0f;
    } else {
        var1 = i_morf;
    }
    int frame_1 = 1;
    int frame_2 = 3;
    switch (i_frame) {
    case 7:
    case 18:
        mpMorf[0]->setStartFrame(0.0f);
        mpMorf[0]->setEndFrame(17.0f);
        break;
    case 8:
    case 19:
        mpMorf[0]->setStartFrame(18.0f);
        mpMorf[0]->setEndFrame(35.0f);
        break;
    case 9:
    case 20:
        mpMorf[0]->setStartFrame(36.0f);
        break;
    case 6:
    case 17:
        mpMorf[0]->setStartFrame(29.0f);
        mpMorf[0]->setEndFrame(41.0f);
        break;
    case 23:
        mpMorf[0]->setStartFrame(0.0f);
        mpMorf[0]->setEndFrame(0.0f);
        mpMorf[0]->setPlaySpeed(0.0f);
        break;
    }
    if (i_frame == 23) {
        frame_1 = 3;
        frame_2 = 5;
    } else if (mFadeLightOn) {
        frame_1 = 2;
        frame_2 = 4;
        if (field_0x1178 != false) {
            frame_2 = 1;
        } else {
            if (field_0x1177 != false) {
                frame_2 = 2;
            } else {
                if (i_frame == 3 || i_frame == 14) {
                    frame_1 = 0;
                    frame_2 = 0;
                }
            }
        }
    }
    if (btkAnmData[frame_1].field_0x0 != -1) {
        anm_text = getTexSRTKeyAnmP(l_resNameList[btkAnmData[frame_1].field_0x8],
                                    btkAnmData[frame_1].field_0x0);
    }
    if (anm_text != NULL) {
        if (mBtkAnm.getBtkAnm() == anm_text) {
            mAnmFlags |= ANM_PLAY_BTK;
        } else {
            if (setBtkAnm(anm_text, mpMorf[0]->getModel()->getModelData(), 1.0f,
                          btkAnmData[4].field_0x4))
            {
                if (frame_1 == 3) {
                    mBtkAnm.setPlaySpeed(0.0f);
                }
                mAnmFlags |= 0x82;
            }
            if ((mAnmFlags & 2) == 0) {
                anm_text = NULL;
            }
        }
    }
    if (anm_text == NULL && btkAnmData[frame_1].field_0x0 != -1) {
        return 0;
    }
    if (brkAnmData[frame_2].field_0x0 != -1) {
        anm_tev = getTevRegKeyAnmP(l_resNameList[brkAnmData[frame_2].field_0x8],
                                    brkAnmData[frame_2].field_0x0);
    }
    if (anm_tev != NULL) {
        if (mBrkAnm.getBrkAnm() == anm_tev) {
            mAnmFlags |= ANM_PLAY_BRK;
        } else {
            frame_1 = setBrkAnm(anm_tev, mpMorf[0]->getModel()->getModelData(), 1.0,
                                brkAnmData[6].field_0x4);
            if (frame_1 != 0) {
                if (frame_2 == 5) {
                    mBrkAnm.setPlaySpeed(0.0f);
                }
                mAnmFlags |= 0x104;
            }
            if ((mAnmFlags & 4) == 0) {
                anm_tev = NULL;
            }
        }
    }
    if (anm_tev == NULL && brkAnmData[frame_2].field_0x0 != -1) {
        return 0;
    }
    if (mType == TYPE_6) {
        return 1;
    }
    frame_1 = setYariAnm(i_frame, i_mode, var1);
    if (frame_1 & 0xFF) {
        if (mType == TYPE_2 || mType == TYPE_3 || mType == TYPE_4 || mType == TYPE_5) {
            mpMorf[0]->setPlaySpeed(0.0f);
            if (mpMcaMorf != NULL) {
                mpMcaMorf->setPlaySpeed(0.0f);
            }
        }
        return 1;
    }
    return 0;
}

int daObj_Sekizoa_c::selectAction() {
    mInitFunc = NULL;
    switch (mType) {
    case TYPE_0:
    case TYPE_1:
        mInitFunc = &daObj_Sekizoa_c::puzzle;
        break;
    default:
        mInitFunc = &daObj_Sekizoa_c::wait;
        break;
    }
    return 1;
}

int daObj_Sekizoa_c::chkAction(actionFunc i_action) {
    return mExecuteFunc == i_action;
}

int daObj_Sekizoa_c::setAction(actionFunc i_action) {
    mMode = 3;
    if (mExecuteFunc != NULL) {
        (this->*(mExecuteFunc))(0);
    }

    mMode = 0;
    mExecuteFunc = i_action;
    if (mExecuteFunc != NULL) {
        (this->*(mExecuteFunc))(0);
    }
    return 1;
}


int daObj_Sekizoa_c::checkMoveDirection() {
    cXyz temp_vec;
    cXyz temp_vec2;

    f32 temp_float_y = daObj_Sekizoa_Param_c::m.field_0x00 * 0.33f;
    fopAc_ac_c* actor_3 = mActorMngrs[3].getActorP();
    temp_vec.set(0.0f, 0.0f, daObj_Sekizoa_Param_c::m.field_0x8C);
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::YrotM(current.angle.y);
    mDoMtx_stack_c::multVec(&temp_vec, &temp_vec2);
    if (chkPointInArea(actor_3->current.pos, temp_vec2, daObj_Sekizoa_Param_c::m.field_0x8C / 2,
                       300.0f, -300.0f, 0) == false)
    {
        temp_vec2.y += 10.0f;
        mGndChk.SetPos(&temp_vec2);
        if (fabsf(dComIfG_Bgsp().GroundCross(&mGndChk) - current.pos.y) < 0.1f) {
            temp_vec.set(0.0f, temp_float_y, daObj_Sekizoa_Param_c::m.field_0x8C);
            mDoMtx_stack_c::YrotS(current.angle.y);
            mDoMtx_stack_c::multVec(&temp_vec, &temp_vec2);
            temp_vec2 = current.pos + temp_vec2;
            temp_vec = current.pos;
            temp_vec.y += temp_float_y;
            mLinChk.Set(&temp_vec, &temp_vec2, this);
            if (!dComIfG_Bgsp().LineCross(&mLinChk)) {
                return 1;
            }
        }
    }
    return 0;
}

int daObj_Sekizoa_c::getWaitMotionNo() {
    switch (mType) {
    case TYPE_0:
        return 1;
    case TYPE_1:
        return 12;
    case TYPE_6:
        return 23;
    }
    return 0;
}

int daObj_Sekizoa_c::getGameMotionNo() {
    switch (mType) {
    case TYPE_0:
        return 5;
    case TYPE_1:
        return 16;
    case TYPE_6:
        return 23;
    }
    return 0;
}

int daObj_Sekizoa_c::getNoJumpMotionNo() {
    switch (mType) {
    case TYPE_0:
        return 10;
    case TYPE_1:
        return 21;
    }
    return -1;
}

int daObj_Sekizoa_c::getHitMotionNo() {
    switch (mType) {
    case TYPE_0:
        return 6;
    case TYPE_1:
        return 17;
    }
    return -1;
}

int daObj_Sekizoa_c::getStepMotionNo() {
    switch (mType) {
    case TYPE_0:
        return 11;
    case TYPE_1:
        return 22;
    }
    return -1;
}


void daObj_Sekizoa_c::jump() {
    cXyz temp_vec;

    switch (mJump) {
    case 0:
        if (mMotionSeqMngr.checkEndSequence() != 0) {
            mMotionSeqMngr.setNo(getGameMotionNo(), -1.0f, 0, 0);
            mJump = 5;
        }
        break;

    case 1:
        if (checkMoveDirection() != 0) {
            switch (mType) {
            case TYPE_0:
                mMotionSeqMngr.setNo(7, -1.0f, 0, 0);
                mSound.startCreatureVoice(Z2SE_SEKI_V_JUMP_1, -1);
                break;
            case TYPE_1:
                mMotionSeqMngr.setNo(18, -1.0f, 0, 0);
                mSound.startCreatureVoice(Z2SE_SEKI_V_JUMP_2, -1);
                break;
            case TYPE_2:;
            }
            mJump = 2;
        } else {
            mMotionSeqMngr.setNo(getNoJumpMotionNo(), -1.0f, 0, 0);
            if (mType == TYPE_0) {
                mSound.startCreatureVoice(Z2SE_SEKI_V_NO_JUMP_1, -1);
            } else if (mType == TYPE_1) {
                mSound.startCreatureVoice(Z2SE_SEKI_V_NO_JUMP_2, -1);
            }
            mJump = 0;
        }
        speedF = 0.0f;
        speed.setall(0.0f);
        mJumpHeight = 0.0f;
        mJumpSpeed = 0.0f;
        break;

    case 2:
    case 3: {
        cXyz temp_vec2;

        if (mMotionSeqMngr.getNo() == 7 || mMotionSeqMngr.getNo() == 18) {
            if (mMotionSeqMngr.checkEndSequence() != 0) {
                switch (mMotionSeqMngr.getNo()) {
                case 7:
                    mMotionSeqMngr.setNo(8, -1.0f, 0, 0);
                    break;
                case 18:
                    mMotionSeqMngr.setNo(19, -1.0f, 0, 0);
                    break;
                }
                mCXyzJump = current.pos;
                mJumpSpeed = daObj_Sekizoa_Param_c::m.field_0x90;
                return;
            }
        } else {
            if (mJump == 2 && mCyl.ChkCoHit() != 0 &&
                fopAcM_GetName(mCyl.GetCoHitObj()->GetAc()) == 0x20)
            {
                dComIfGp_getVibration().StartShock(VIBMODE_S_POWER5, 15, cXyz(0.0f, 1.0f, 0.0f));
                mMotionSeqMngr.setNo(getHitMotionNo(), -1.0f, 0, 0);
                if (mType == TYPE_0) {
                    mSound.startCreatureVoice(Z2SE_SEKI_V_COL1, -1);
                } else if (mType == TYPE_1) {
                    mSound.startCreatureVoice(Z2SE_SEKI_V_COL2, -1);
                    mSound.startCreatureSound(Z2SE_SEKI_JUMP_COL, 0, -1);
                }
                mJumpSpeed = daObj_Sekizoa_Param_c::m.field_0x90;
                mJump = 3;
            }
            if (mJump == 2) {
                cLib_chaseF(&mJumpHeight, daObj_Sekizoa_Param_c::m.field_0x8C, mJumpSpeed);
            } else {
                cLib_chaseF(&mJumpHeight, 0.0f, mJumpSpeed);
            }

            f32 var_f31 = daObj_Sekizoa_Param_c::m.field_0x94;
            f32 var_f29 = var_f31 * ((daObj_Sekizoa_Param_c::m.field_0x8C * 0.5f) *
                                     (daObj_Sekizoa_Param_c::m.field_0x8C * 0.5f));
            f32 var_f30 = mJumpHeight - (daObj_Sekizoa_Param_c::m.field_0x8C * 0.5f);

            temp_vec.set(0.0f, var_f29 + (-var_f31 * (var_f30 * var_f30)), mJumpHeight);

            mDoMtx_stack_c::YrotS(current.angle.y);
            mDoMtx_stack_c::multVec(&temp_vec, &temp_vec2);
            current.pos = mCXyzJump + temp_vec2;
            cLib_chaseF(&mJumpSpeed, daObj_Sekizoa_Param_c::m.field_0x90 * 0.5f, 0.25f);
            if (mJumpHeight <= 0 || daObj_Sekizoa_Param_c::m.field_0x8C <= mJumpHeight) {
                landing();
                mJump = 4;
                return;
            }
        }
        break;
    }
    case 4:
        if (mMotionSeqMngr.checkEndSequence() != 0) {
            mMotionSeqMngr.setNo(getGameMotionNo(), -1.0f, 0, 0);
            mJump = 5;
        }
        break;

    case 5:;
    }
    return;
}


void daObj_Sekizoa_c::landing() {
    mDoAud_seStart(Z2SE_SEKI_JUMP_LAND, 0, 0, 0);
    if (chkPressPlayer()) {
        mSound.startCreatureSound(Z2SE_SY_MG_SEKI_FAILURE, 0, -1);
    }
    dComIfGp_getVibration().StartShock(VIBMODE_S_POWER7, 15, cXyz(0.0f, 1.0f, 0.0f));
    switch (mType) {
    case TYPE_0:
        mMotionSeqMngr.setNo(9, -1.0f, 0, 0);
        return;
    case TYPE_1:
        mMotionSeqMngr.setNo(20, -1.0f, 0, 0);
        break;
    case TYPE_2:;
    }
}


int daObj_Sekizoa_c::cutStart(int i_staffIdx) {
    cXyz c_stack_9c;
    csXyz c_stack_c8;

    int ret = 0;
    int prm = -1;
    int timer = 0;
    int msgNo = 0;
    char acStack_90[100];

    int* prm_p = dComIfGp_evmng_getMyIntegerP(i_staffIdx, "prm");
    if (prm_p != NULL) {
        prm = *prm_p;
    }
    int* timer_p = dComIfGp_evmng_getMyIntegerP(i_staffIdx, "timer");
    if (timer_p != NULL) {
        timer = *timer_p;
    }
    int* msgNo_p = dComIfGp_evmng_getMyIntegerP(i_staffIdx, "msgNo");
    if (msgNo_p != NULL) {
        msgNo = *msgNo_p;
    }

    if (dComIfGp_getEventManager().getIsAddvance(i_staffIdx) != 0) {
        switch (prm) {
        case 0:
            mFaceMotionSeqMngr.setNo(0, 0.0f, 0, 0);
            switch (mType) {
            case TYPE_0:
                mMotionSeqMngr.setNo(2, 0.0f, 0, 0);
                break;
            case TYPE_1:
                mMotionSeqMngr.setNo(13, 0.0f, 0, 0);
                break;
            case TYPE_2:;
            }
            if (mType == TYPE_0) {
                daObj_SMTile_c* actor_4 = (daObj_SMTile_c*)mActorMngrs[4].getActorP();
                actor_4->reset();
                dComIfGp_getEvent().setPt2(actor_4);
                strcpy(acStack_90, l_evtList[2].eventName);
                strcat(acStack_90, "@");
                dComIfGp_getEvent().setSkipZev(this, acStack_90);
                dComIfGp_getEvent().onSkipFade();
                if (daNpcT_getPlayerInfoFromPlayerList(1, fopAcM_GetRoomNo(this), &c_stack_9c,
                                                       &c_stack_c8))
                {
                    daPy_getPlayerActorClass()->setPlayerPosAndAngle(&c_stack_9c, c_stack_c8.y, 0);
                    dComIfGp_evmng_setGoal(&c_stack_9c);
                }
                daPy_getPlayerActorClass()->setGiantPuzzle();
                daNpcT_onTmpBit(0x31);
                dComIfGp_2dShowOff();
                mDoAud_seStart(Z2SE_SY_MG_SEKI_BLACK_OUT, 0, 0, 0);
            }
            c_stack_c8.setall(0);
            switch (mType) {
            case TYPE_0:
                if (daNpcT_chkEvtBit(0x25E)) {
                    c_stack_9c = l_srcPosL;
                } else {
                    c_stack_9c = l_srcPosR;
                }
                setPos(c_stack_9c);
                setAngle(c_stack_c8.y);
                break;
            case TYPE_1:
                if (daNpcT_chkEvtBit(0x25E)) {
                    c_stack_9c = l_srcPosR;
                } else {
                    c_stack_9c = l_srcPosL;
                }
                setPos(c_stack_9c);
                setAngle(c_stack_c8.y);
                break;
            case TYPE_2:;
            }
            break;

        case 1:
            if (mType == TYPE_0) {
                daNpcT_c* actor_1 = (daNpcT_c*)mActorMngrs[1].getActorP();
                actor_1->current.pos.y += 1000.0f;
                actor_1->old.pos = actor_1->current.pos;
                actor_1->hide();

                daNpcT_c* actor_2 = (daNpcT_c*)mActorMngrs[2].getActorP();
                actor_2->current.pos.y += 1000.0f;
                actor_2->old.pos = actor_2->current.pos;
                actor_2->hide();
                mColsetBlend = 0.0f;
            }
            mHide = 0;
            break;

        case 2:
            mEventTimer = timer;
            break;

        case 3:
            if (mType == TYPE_0) {
                mColsetBlend = 0.0f;
            }
            break;

        case 4:
            switch (mType) {
            case TYPE_0:
                mMotionSeqMngr.setNo(1, -1.0f, 0, 0);
                break;
            case TYPE_1:
                mMotionSeqMngr.setNo(12, -1.0f, 0, 0);
                break;
            case TYPE_2:;
            }
            mEventTimer = timer;
            break;

        case 5:
            mJump = 1;
            mEventTimer = timer;
            break;

        case 6:
            switch (mType) {
            case TYPE_0:
                mMotionSeqMngr.setNo(8, 0.0f, 0, 0);
                break;
            case TYPE_1:
                mMotionSeqMngr.setNo(19, 0.0f, 0, 0);
                break;
            case TYPE_2:;
            }
            mAcch.ClrGroundHit();
            mAcch.ClrGroundLanding();
            c_stack_9c = home.pos;
            c_stack_9c.y += 500.0f;
            current.pos = c_stack_9c;
            old.pos = current.pos;
            setAngle(home.angle.y);
            speedF = 0.0f;
            speed.setall(0.0f);
            mEventTimer = timer;
            mHide = 0;
            break;

        case 13:
        case 7:
            if (mType == TYPE_0) {
                daPy_getPlayerActorClass()->setGiantPuzzleEnd();
                if (prm == 13) {
                    daObj_SMTile_c* actor_4 = (daObj_SMTile_c*)mActorMngrs[4].getActorP();
                    actor_4->reset();
                    dComIfGp_getEvent().setPt2(actor_4);
                    mDoAud_seStart(Z2SE_SY_MG_SEKI_WHITE_OUT, 0, 0, 0);
                }
            }
            break;

        case 8:
            mFaceMotionSeqMngr.setNo(0, 0.0f, 0, 0);
            mMotionSeqMngr.setNo(getWaitMotionNo(), 0.0f, 0, 0);
            setPos(home.pos);
            setAngle(home.angle.y);
            if (mType == TYPE_0) {
                if (daNpcT_getPlayerInfoFromPlayerList(1, fopAcM_GetRoomNo(this), &c_stack_9c,
                                                       &c_stack_c8))
                {
                    daPy_getPlayerActorClass()->setPlayerPosAndAngle(&c_stack_9c, c_stack_c8.y, 0);
                    dComIfGp_evmng_setGoal(&c_stack_9c);
                }
                dComIfGs_onSwitch(getBitSW(), fopAcM_GetRoomNo(this));
                daObj_SMTile_c* actor_4 = (daObj_SMTile_c*)mActorMngrs[4].getActorP();
                actor_4->reset();
                dKy_custom_colset(10, 10, 0.0f);
                daPy_getPlayerActorClass()->setGiantPuzzle();
                mDoAud_subBgmStart(Z2BGM_STATUE_GAME);
                daNpcT_onTmpBit(0x31);
                dComIfGp_2dShowOff();

                daNpcT_c* actor_1 = (daNpcT_c*)mActorMngrs[1].getActorP();
                actor_1->current.pos.y += 1000.0f;
                actor_1->old.pos = actor_1->current.pos;
                actor_1->hide();

                daNpcT_c* actor_2 = (daNpcT_c*)mActorMngrs[2].getActorP();
                actor_2->current.pos.y += 1000.0f;
                actor_2->old.pos = actor_2->current.pos;
                actor_2->hide();
            }
            mHide = 0;
            mFadeLightOn = true;
            break;

        case 9:
            mMotionSeqMngr.setNo(getGameMotionNo(), -1.0f, 0, 0);
            mEventTimer = timer;
            break;

        case 10:
            mEventTimer = timer;
            break;

        case 11:
            mFlow.init(NULL, 0x71, 0, NULL);
            break;

        case 12:
            if (mType == TYPE_0) {
                if (daNpcT_getPlayerInfoFromPlayerList(1, fopAcM_GetRoomNo(this), &c_stack_9c,
                                                       &c_stack_c8))
                {
                    daPy_getPlayerActorClass()->setPlayerPosAndAngle(&c_stack_9c, c_stack_c8.y, 0);
                    dComIfGp_evmng_setGoal(&c_stack_9c);
                }
                daPy_getPlayerActorClass()->setGiantPuzzle();
            }
            mHide = 1;
            break;

        case 20:
            break;

        case 99:
            break;
        }
    }

    int local_d0[2] = {-1, -1};

    switch (prm) {
    case 0:
        ret = 1;
        break;

    case 1:
        if (mType == TYPE_0) {
            dKy_custom_colset(0, 9, mColsetBlend);
            if (mColsetBlend < 1.0f) {
                cLib_chaseF(&mColsetBlend, 1.0f, 0.05f);
                if (mColsetBlend == 1.0f) {
                    mDoAud_bgmStop(0x1e);
                }
            } else {
                ret = 1;
            }
        } else {
            ret = 1;
        }
        break;

    case 2:
        if (mType == TYPE_0) {
            daObj_SMTile_c* actor_4 = (daObj_SMTile_c*)mActorMngrs[4].getActorP();
            if (mEventTimer != 0 || dComIfGp_getEventManager().getIsAddvance(i_staffIdx) != 0) {
                if (cLib_calcTimer(&mEventTimer) == 0) {
                    dComIfGs_onSwitch(getBitSW(), fopAcM_GetRoomNo(this));
                    actor_4->slide();
                }
            } else if (actor_4->checkEndMove()) {
                ret = 1;
            }
        }
        break;

    case 3:
        if (mType == TYPE_0) {
            dKy_custom_colset(9, 10, mColsetBlend);
            if (mColsetBlend < 1.0f) {
                cLib_chaseF(&mColsetBlend, 1.0f, 0.05f);
            } else {
                ret = 1;
            }
        }
        break;

    case 4:
        if (mEventTimer != 0 || dComIfGp_getEventManager().getIsAddvance(i_staffIdx) != 0) {
            if (cLib_calcTimer(&mEventTimer) == 0) {
                switch (mType) {
                case TYPE_0:
                    mMotionSeqMngr.setNo(3, -1.0f, 0, 0);
                    mSound.startCreatureSound(Z2SE_SEKI_ST_DEMO, 0, -1);
                    mSound.startCreatureVoice(Z2SE_SEKI_V_KAKUSEI_1, -1);
                    mFadeLightOn = true;
                    break;
                case TYPE_1:
                    mMotionSeqMngr.setNo(14, -1.0f, 0, 0);
                    mSound.startCreatureSound(Z2SE_SEKI_ST_DEMO, 0, -1);
                    mSound.startCreatureVoice(Z2SE_SEKI_V_KAKUSEI_2, -1);
                    mFadeLightOn = true;
                    break;
                case TYPE_2:;
                }
            }
        } else {
            if (((mType == TYPE_0 && mMotionSeqMngr.getNo() == 3) ||
                 (mType == TYPE_1 && mMotionSeqMngr.getNo() == 0xE)) &&
                mMotionSeqMngr.checkEndSequence() != 0)
            {
                ret = 1;
            }
        }
        break;

    case 5:
        if (mEventTimer != 0) {
            if (cLib_calcTimer(&mEventTimer) != 0) {
                jump();
            } else {
                mAcch.ClrGroundHit();
                mAcch.ClrGroundLanding();
                c_stack_9c = home.pos;
                c_stack_9c.y += 500.0f;
                current.pos = c_stack_9c;
                old.pos = current.pos;
                setAngle(home.angle.y);
                speedF = 0.0f;
                speed.setall(0.0f);
            }
        }
        if (mEventTimer == 0) {
            gravity = 0.0f;
            ret = 1;
        }
        break;

    case 6:
        if (mEventTimer != 0) {
            cLib_calcTimer(&mEventTimer);
            gravity = 0.0f;
        } else {
            if (mAcch.ChkGroundHit()) {
                if (mAcch.ChkGroundLanding()) {
                    landing();
                } else {
                    if (mMotionSeqMngr.getNo() == 9 || mMotionSeqMngr.getNo() == 20) {
                        if (mMotionSeqMngr.checkEndSequence() != 0) {
                            mMotionSeqMngr.setNo(getWaitMotionNo(), -1.0, 0, 0);
                            break;
                        } else {
                            break;
                        }
                    } else {
                        ret = 1;
                    }
                }
            }
        }
        break;

    case 7:
    case 8:
    case 13:
        ret = 1;
        break;

    case 9:
        if ((mEventTimer != 0 || dComIfGp_getEventManager().getIsAddvance(i_staffIdx) != 0) &&
            cLib_calcTimer(&mEventTimer) == 0)
        {
            mDoAud_subBgmStart(Z2BGM_STATUE_GAME);
        }
        ret = (mEventTimer == 0);
        break;

    case 10:
        if (cLib_calcTimer(&mEventTimer) == 0) {
            ret = 1;
        }
        break;

    case 20:
    case 11:
        local_d0[0] = msgNo;
        field_0xe26 = 0;
        if (mFlow.doFlow(NULL, NULL, 0) != 0) {
            if (mFlow.checkEndFlow()) {
                ret = 1;
            }
        } else if (mFlow.getMsg() != NULL) {
            switch (mFlow.getMsg()->mode) {
            case 2:
            case 6:
                for (int i = 0; i < local_d0[i]; i++) {
                    if (local_d0[i] == mFlow.getMsgNo()) {
                        ret = 1;
                    }
                }
                break;
            default:
                break;
            }
        }
        break;

    case 12:
        gravity = 0.0f;
        ret = 1;
        break;

    case 99:
        break;

    default:
        break;
    }
    return ret;
}

int daObj_Sekizoa_c::cutTurn(int i_staffIdx) {
    int ret = 0;
    int prm = -1;
    int* prm_p = dComIfGp_evmng_getMyIntegerP(i_staffIdx, "prm");
    if (prm_p != NULL) {
        prm = *prm_p;
    }
    if (dComIfGp_getEventManager().getIsAddvance(i_staffIdx) != 0) {
        switch (prm) {
        case 0:
            mPlayerDirection =
                daPy_getPlayerActorClass()->getGiantPuzzleAimAngle() - mPlayerDirection;
            mPlayerDirection += mCurAngle.y;
            break;
        case 1:
            mPlayerDirection = daPy_getPlayerActorClass()->getGiantPuzzleAimAngle();
            break;
        }
    }
    switch (prm) {
    case 0:
        if (mPlayerDirection == mCurAngle.y) {
            ret = 1;
        } else if (step(mPlayerDirection, -1, getStepMotionNo(), 0x1e, 0)) {
            mFaceMotionSeqMngr.setNo(0, -1.0f, 0, 0);
            mMotionSeqMngr.setNo(getGameMotionNo(), -1.0f, 0, 0);
            mTurnPlayer = 0;
        }
        break;
    case 1:
        if (daPy_getPlayerActorClass()->shape_angle.y ==
            daPy_getPlayerActorClass()->getGiantPuzzleAimAngle())
        {
            ret = 1;
        }
        break;
    }
    return ret;
}

int daObj_Sekizoa_c::cutJump(int i_staffIdx) {
    daObj_Sekizoa_c* actor_3;

    int ret = 0;
    int prm = -1;
    int* prm_p = dComIfGp_evmng_getMyIntegerP(i_staffIdx, "prm");
    if (prm_p != NULL) {
        prm = *prm_p;
    }
    if (dComIfGp_getEventManager().getIsAddvance(i_staffIdx) != 0) {
        switch (prm) {
        case 0:
            break;
        case 1:;
            mJump = 1;
            break;
        case 2:;
        }
    }
    switch (prm) {
    case 0:
        if (mType == TYPE_0) {
            if (mLatencyTime != 0) {
                cLib_calcTimer(&mLatencyTime);
            }
            ret = (mLatencyTime == 0);
        } else {
            ret = 1;
        }

    default:
        break;

    case 1:
        jump();
        gravity = 0.0f;
        speedF = 0.0f;
        speed.setall(0.0f);
        if (mJump == 5) {
            ret = 1;
        }
        break;

    case 2:
        if (mType == TYPE_0) {
            actor_3 = (daObj_Sekizoa_c*)mActorMngrs[3].getActorP();
            if (chkPressPlayer() || actor_3->chkPressPlayer()) {
                mEvtNo = 3;
                evtChange();
                return 0;
            }
            if (chkGoal() && actor_3->chkGoal()) {
                mEvtNo = 6;
                evtChange();
            }
        }
        ret = 1;
        break;
    }
    return ret;
}

int daObj_Sekizoa_c::cutGoal(int i_staffIdx) {
    cXyz cStack_a8;
    csXyz cStack_b0;
    char acStack_9c[100];

    int ret = 0;
    int prm = -1;
    int timer = 0;
    int* prm_p = dComIfGp_evmng_getMyIntegerP(i_staffIdx, "prm");
    if (prm_p != NULL) {
        prm = *prm_p;
    }
    int* timer_p = dComIfGp_evmng_getMyIntegerP(i_staffIdx, "timer");
    if (timer_p != NULL) {
        timer = *timer_p;
    }
    if (dComIfGp_getEventManager().getIsAddvance(i_staffIdx) != 0) {
        switch (prm) {
        case 0:
            if (mType == TYPE_0) {
                strcpy(acStack_9c, l_evtList[6].eventName);
                strcat(acStack_9c, "@");
                dComIfGp_getEvent().setSkipZev(this, acStack_9c);
                dComIfGp_getEvent().onSkipFade();
            }
            mEventTimer = timer;
            break;

        case 1:
            if (mCurAngle.y == 0) {
                mGoalStatueAngle = 65536.0f;
            } else {
                mGoalStatueAngle = (float)-mCurAngle.y;
            }
            mGoalStatueTurnSpeed = fabsf(mGoalStatueAngle / 30.0f);
            mMotionSeqMngr.setNo(getStepMotionNo(), -1.0f, 0, 0);
            mEventTimer = timer;
            break;

        case 2:
            if (mType == TYPE_0) {
                mMotionSeqMngr.setNo(4, -1.0f, 0, 0);
                mSound.startCreatureSound(Z2SE_SEKI_END_DEMO_1, 0, -1);
                mSound.startCreatureVoice(Z2SE_SEKI_V_CLEAR_1, -1);
                dKy_custom_colset(10, 0, 0.0f);
                daPy_getPlayerActorClass()->setGiantPuzzleEnd();
                mDoAud_subBgmStop();
                daNpcT_offTmpBit(0x31);
                mSetWolfHowling = false;
                dComIfGp_2dShowOn();
                daNpcT_onEvtBit(0xEB);
            } else if (mType == TYPE_1) {
                mMotionSeqMngr.setNo(15, -1.0f, 0, 0);
                mSound.startCreatureSound(Z2SE_SEKI_END_DEMO_1, 0, -1);
                mSound.startCreatureVoice(Z2SE_SEKI_V_CLEAR_2, -1);
            }
            field_0x1177 = true;
            break;

        case 3:
            mEventTimer = timer;
            break;

        case 4:
            if (mType == TYPE_0) {
                if (daNpcT_getPlayerInfoFromPlayerList(1, fopAcM_GetRoomNo(this), &cStack_a8,
                                                       &cStack_b0))
                {
                    daPy_getPlayerActorClass()->setPlayerPosAndAngle(&cStack_a8, cStack_b0.y, 0);
                    dComIfGp_evmng_setGoal(&cStack_a8);
                }
                if (getBitSW() != 0xFF) {
                    dComIfGs_offSwitch(getBitSW(), fopAcM_GetRoomNo(this));
                }
                mFlow.init(NULL, 0x73, 0, NULL);
            }
            break;

        case 5:
            mEventTimer = timer;
            break;

        case 6:
            if (mType == TYPE_0) {
                if (mReverseStatues != 0) {
                    daNpcT_onEvtBit(0x25E);
                }
                daObj_Sekizoa_c* actor_1 = (daObj_Sekizoa_c*)mActorMngrs[1].getActorP();
                if (daNpcT_chkEvtBit(0x25E)) {
                    actor_1->current.pos = l_srcPosL;
                } else {
                    actor_1->current.pos = l_srcPosR;
                }
                actor_1->old.pos = actor_1->current.pos;
                actor_1->show();
                actor_1->setFadeLightOff();

                daObj_Sekizoa_c* actor_2 = (daObj_Sekizoa_c*)mActorMngrs[2].getActorP();
                if (daNpcT_chkEvtBit(0x25E)) {
                    actor_2->current.pos = l_srcPosR;
                } else {
                    actor_2->current.pos = l_srcPosL;
                }
                actor_2->old.pos = actor_2->current.pos;
                actor_2->show();
                actor_2->setFadeLightOff();
            }
            mHide = 1;
            break;

        case 9:
            if (mType == TYPE_0) {
                daObj_SekiDoor_c* actor_7 = (daObj_SekiDoor_c*)mActorMngrs[7].getActorP();
                if (actor_7 != NULL) {
                    actor_7->evtSkip();
                }
                if (mReverseStatues != 0) {
                    daNpcT_onEvtBit(0x25E);
                }

                daObj_Sekizoa_c* actor_1 = (daObj_Sekizoa_c*)mActorMngrs[1].getActorP();
                if (daNpcT_chkEvtBit(0x25E)) {
                    actor_1->current.pos = l_srcPosL;
                } else {
                    actor_1->current.pos = l_srcPosR;
                }
                actor_1->old.pos = actor_1->current.pos;
                actor_1->show();
                actor_1->setFadeLightOff();

                daObj_Sekizoa_c* actor_2 = (daObj_Sekizoa_c*)mActorMngrs[2].getActorP();
                if (daNpcT_chkEvtBit(0x25E)) {
                    actor_2->current.pos = l_srcPosR;
                } else {
                    actor_2->current.pos = l_srcPosL;
                }
                actor_2->old.pos = actor_2->current.pos;
                actor_2->show();
                actor_2->setFadeLightOff();

                if (daNpcT_getPlayerInfoFromPlayerList(1, fopAcM_GetRoomNo(this), &cStack_a8,
                                                       &cStack_b0))
                {
                    daPy_getPlayerActorClass()->setPlayerPosAndAngle(&cStack_a8, cStack_b0.y, 0);
                    dComIfGp_evmng_setGoal(&cStack_a8);
                }
                if (getBitSW() != 0xFF) {
                    dComIfGs_offSwitch(getBitSW(), fopAcM_GetRoomNo(this));
                }
                if (getBitSW2() != 0xFF) {
                    dComIfGs_onSwitch(getBitSW2(), fopAcM_GetRoomNo(this));
                }
                dKy_custom_colset(0, 0, 0.0f);
                daPy_getPlayerActorClass()->setGiantPuzzleEnd();
                mDoAud_subBgmStop();
                daNpcT_offTmpBit(0x31);
                mSetWolfHowling = false;
                dComIfGp_2dShowOn();
                daNpcT_onEvtBit(0xEB);
            }
            mHide = 1;
            mFadeLightOn = false;
        }
    }
    switch (prm) {
    case 0:
        if (cLib_calcTimer(&mEventTimer) == 0) {
            ret = 1;
        }
        break;

    case 1: {
        f32 temp_float = mGoalStatueAngle;
        cLib_chaseF(&mGoalStatueAngle, 0.0f, mGoalStatueTurnSpeed);
        current.angle.y -= (int)(mGoalStatueAngle - temp_float);
        shape_angle.y = current.angle.y;
        mCurAngle.y = current.angle.y;
        if (mMotionSeqMngr.checkEndSequence() != 0) {
            ret = 1;
        }
        break;
    }
    case 2:
        if (mMotionSeqMngr.checkEndSequence() != 0) {
            ret = 1;
        }
        break;

    case 3:
        if (cLib_calcTimer(&mEventTimer) == 0 && mType == TYPE_0) {
            daObj_SekiDoor_c* actor_7 = (daObj_SekiDoor_c*)mActorMngrs[7].getActorP();
            if (actor_7->chkDestroy()) {
                if (getBitSW2() != 0xFF) {
                    dComIfGs_onSwitch(getBitSW2(), fopAcM_GetRoomNo(this));
                }
                ret = 1;
            } else {
                actor_7->open();
            }
        }
        break;

    case 4:
        if (mType == TYPE_0) {
            field_0xe26 = 0;
            if (mFlow.doFlow(NULL, NULL, 0) != 0 && mFlow.checkEndFlow()) {
                ret = 1;
            }
        } else {
            ret = 1;
        }
        break;

    case 5:
        if (cLib_calcTimer(&mEventTimer) == 0) {
            ret = 1;
        }
        break;

    case 6:
        ret = 1;
        break;

    case 9:
        ret = 1;
        break;
    }
    return ret;
}

int daObj_Sekizoa_c::cutExit(int i_staffIdx) {
    int var_r30 = 0;
    int prm = -1;
    int* prm_p = dComIfGp_evmng_getMyIntegerP(i_staffIdx, "prm");
    if (prm_p != NULL) {
        prm = *prm_p;
    }
    if (dComIfGp_getEventManager().getIsAddvance(i_staffIdx) != 0) {
        switch (prm) {
        case 0:
            if (mType == TYPE_0) {
                mFlow.init(NULL, 0x72, 0, NULL);
            }
        case 1:
            break;
        }
    }
    switch (prm) {
    case 0:
        if (mType == TYPE_0) {
            field_0xe26 = 0;
            if (mFlow.doFlow(NULL, NULL, 0) != 0 && mFlow.checkEndFlow()) {
                switch (mFlow.getChoiceNo()) {
                case 0:
                    mEvtNo = 3;
                    evtChange();
                    break;
                case 1:
                    mEvtNo = 8;
                    evtChange();
                    break;
                case 2:
                    var_r30 = 1;
                    break;
                default:
                    var_r30 = 1;
                    break;
                }
            }
        } else {
            var_r30 = 1;
        }
        break;
    case 1:
        var_r30 = 1;
        break;
    }
    return var_r30;
}

int daObj_Sekizoa_c::cutFree(int i_staffIdx) {
    cXyz cStack_2c;
    csXyz cStack_34;
    int var_r30 = 0;
    int prm = -1;
    int timer = 0;

    int* prm_p = dComIfGp_evmng_getMyIntegerP(i_staffIdx, "prm");
    if (prm_p != NULL) {
        prm = *prm_p;
    }
    int* timer_p = dComIfGp_evmng_getMyIntegerP(i_staffIdx, "timer");
    if (timer_p != NULL) {
        timer = *timer_p;
    }
    if (dComIfGp_getEventManager().getIsAddvance(i_staffIdx) != 0) {
        switch (prm) {
        case 0:
            break;
        case 1:
            if (mType == TYPE_0) {
                daNpcT_c* actor_1 = (daNpcT_c*)mActorMngrs[1].getActorP();
                actor_1->current.pos = actor_1->home.pos;
                actor_1->old.pos = actor_1->current.pos;
                actor_1->show();

                daNpcT_c* actor_2 = (daNpcT_c*)mActorMngrs[2].getActorP();
                actor_2->current.pos = actor_2->home.pos;
                actor_2->old.pos = actor_2->current.pos;
                actor_2->show();
                if (daNpcT_getPlayerInfoFromPlayerList(1, fopAcM_GetRoomNo(this), &cStack_2c,
                                                       &cStack_34))
                {
                    daPy_getPlayerActorClass()->setPlayerPosAndAngle(&cStack_2c, cStack_34.y, 0);
                    dComIfGp_evmng_setGoal(&cStack_2c);
                }
                if (getBitSW() != 0xFF) {
                    dComIfGs_offSwitch(getBitSW(), fopAcM_GetRoomNo(this));
                }
                dKy_custom_colset(0, 0, 0.0f);
                daPy_getPlayerActorClass()->setGiantPuzzleEnd();
                mDoAud_subBgmStop();
                daNpcT_offTmpBit(0x31);
                mSetWolfHowling = 0;
                dComIfGp_2dShowOn();
            }
            mHide = 1;
            mFadeLightOn = false;
            break;
        case 2:
            mEventTimer = timer;
            break;
        case 3:
            mDoAud_seStart(Z2SE_SY_MG_SEKI_WHITE_OUT, 0, 0, 0);
            break;
        }
    }
    switch (prm) {
    case 0:
        var_r30 = 1;
        break;
    case 1:
        var_r30 = 1;
        break;
    case 2:
        if (cLib_calcTimer(&mEventTimer) == 0) {
            var_r30 = 1;
        }
        break;
    case 3:
        var_r30 = 1;
        break;
    }
    return var_r30;
}


int daObj_Sekizoa_c::cutExtinction(int i_staffIdx) {
    cXyz cStack_b0;
    char acStack_a4[100];
    daTag_KMsg_c* actor_0;

    int ret = 0;
    int prm = -1;
    int timer = 0;
    int* prm_p = dComIfGp_evmng_getMyIntegerP(i_staffIdx, "prm");
    if (prm_p != NULL) {
        prm = *prm_p;
    }
    int* timer_p = dComIfGp_evmng_getMyIntegerP(i_staffIdx, "timer");
    if (timer_p != NULL) {
        timer = *timer_p;
    }
    if (dComIfGp_getEventManager().getIsAddvance(i_staffIdx) != 0) {
        switch (prm) {
        case 0:
            if (getBitSW2() != 0xff) {
                dComIfGs_onSwitch(getBitSW2(), fopAcM_GetRoomNo(this));
            }
            actor_0 = (daTag_KMsg_c*)mActorMngrs[0].getActorP();
            cStack_b0.set(0.0f, 0.0f, 78.0f);
            mDoMtx_stack_c::YrotS(actor_0->shape_angle.y);
            mDoMtx_stack_c::multVec(&cStack_b0, &cStack_b0);
            cStack_b0 += actor_0->current.pos;
            daPy_getPlayerActorClass()->setPlayerPosAndAngle(&cStack_b0,
                                                             actor_0->shape_angle.y - -0x8000, 0);
            strcpy(acStack_a4, l_evtList[9].eventName);
            strcat(acStack_a4, "@");
            dComIfGp_getEvent().setSkipZev(actor_0, acStack_a4);
            dComIfGp_getEvent().onSkipFade();
            actor_0->pullMasterSword();
            dComIfGp_getEvent().setPt2(actor_0);
            break;

        case 1:
            mBrkAnm.setPlaySpeed(1.0f);
            mBtkAnm.setPlaySpeed(1.0f);
            mSound.startCreatureSound(Z2SE_SEKI_OFF, 0, -1);
            break;

        case 2:
            mEventTimer = timer;
            break;

        case 99:
            mBrkAnm.setFrame(mBrkAnm.getEndFrame() - 1.0f);
            mBtkAnm.setFrame(mBtkAnm.getEndFrame() - 1.0f);
            mBrkAnm.setPlaySpeed(1.0f);
            mBtkAnm.setPlaySpeed(1.0f);
            mHide = 1;
            break;
        }
    }
    switch (prm) {
    case 0:
        ret = 1;
        break;

    case 1:
        mHide = (mBrkAnm.getEndFrame() <= (mBrkAnm.getFrame() + mBrkAnm.getPlaySpeed()));
        if (mHide != 0) {
            ret = 1;
        }
        break;

    case 2:
        if (cLib_calcTimer(&mEventTimer) == 0) {
            ret = 1;
        }
        break;

    case 99:
        ret = 1;
        break;
    }
    return ret;
}

int daObj_Sekizoa_c::wait(void* param_0) {
    switch (mMode) {
    case 0:
    case 1:
        if (mStagger.checkStagger() == 0) {
            switch (mType) {
            case TYPE_0:
            case TYPE_1:
                mFaceMotionSeqMngr.setNo(0, -1.0f, 0, 0);
                mMotionSeqMngr.setNo(getWaitMotionNo(), -1.0f, 0, 0);
                break;

            case TYPE_2:
                mFaceMotionSeqMngr.setNo(0, -1.0f, 0, 0);
                mMotionSeqMngr.setNo(2, -1.0f, 0, 0);
                break;

            case TYPE_3:
                mFaceMotionSeqMngr.setNo(0, -1.0f, 0, 0);
                mMotionSeqMngr.setNo(13, -1.0f, 0, 0);
                break;

            case TYPE_4:
                mFaceMotionSeqMngr.setNo(0, -1.0f, 0, 0);
                mMotionSeqMngr.setNo(2, -1.0f, 0, 0);
                break;

            case TYPE_5:
                mFaceMotionSeqMngr.setNo(0, -1.0f, 0, 0);
                mMotionSeqMngr.setNo(13, -1.0f, 0, 0);
                break;

            default:
                mFaceMotionSeqMngr.setNo(0, -1.0f, 0, 0);
                mMotionSeqMngr.setNo(getWaitMotionNo(), -1.0f, 0, 0);
                break;
            }
            mMode = 2;
        }
    case 2:
        if (mStagger.checkStagger() == 0) {
            mJntAnm.lookNone(0);
        }
        attention_info.flags = 0;
    case 3:
    default:
        break;
    }
    return 1;
}

int daObj_Sekizoa_c::puzzle(void* param_0) {
    switch (mMode) {
    case 0:
    case 1:
        if (mStagger.checkStagger() == 0) {
            mFaceMotionSeqMngr.setNo(0, -1.0f, 0, 0);
            if (daNpcT_chkTmpBit(0x31)) {
                mMotionSeqMngr.setNo(getGameMotionNo(), -1.0f, 0, 0);
            } else {
                switch (mType) {
                case TYPE_0:
                    mMotionSeqMngr.setNo(2, -1.0f, 0, 0);
                    break;

                case TYPE_1:
                    mMotionSeqMngr.setNo(13, -1.0f, 0, 0);
                    break;

                case TYPE_2:
                case TYPE_3:
                case TYPE_4:
                case TYPE_5:
                    mMotionSeqMngr.setNo(getWaitMotionNo(), -1.0f, 0, 0);
                    break;

                default:
                    mMotionSeqMngr.setNo(getWaitMotionNo(), -1.0f, 0, 0);
                    break;
                }
            }
            mMode = 2;
        }
    case 2:
        if (mStagger.checkStagger() == 0) {
            if (daNpcT_chkTmpBit(0x31)) {
                if (mType == TYPE_0) {
                    attention_info.flags = fopAc_AttnFlag_SPEAK_e | fopAc_AttnFlag_TALK_e;
                }
                if (daPy_getPlayerActorClass()->checkPlayerFly() != 0) {
                    if (mType == TYPE_0) {
                        mLatencyTime = daObj_Sekizoa_Param_c::m.field_0x98;
                        mEvtNo = 5;
                    }
                } else {
                    if (mTurnPlayer == 0) {
                        if (daPy_getPlayerActorClass()->shape_angle.y !=
                            daPy_getPlayerActorClass()->getGiantPuzzleAimAngle())
                        {
                            mTurnPlayer = 1;
                        } else {
                            mPlayerDirection = daPy_getPlayerActorClass()->shape_angle.y;
                        }
                    }
                    if (mTurnPlayer != 0 && mType == TYPE_0) {
                        mEvtNo = 4;
                    }
                }
            } else {
                if (mType == TYPE_0 && mSetWolfHowling != 0 && getBitSW2() != 0xFF) {
                    if (!dComIfGs_isSwitch(getBitSW2(), fopAcM_GetRoomNo(this))) {
                        field_0xe33 = 1;
                        mEvtNo = 2;
                    }
                }
            }
            mJntAnm.lookNone(0);
        }

    case 3:
    default:
        break;
    }
    return 1;
}

int daObj_Sekizoa_c::talk(void* param_0) {
    switch (mMode) {
    case 0:
    case 1:
        if (mStagger.checkStagger() == 0) {
            initTalk(mFlowNodeNo, NULL);
            mMode = 2;
        }
    case 2:
        if (mStagger.checkStagger() == 0) {
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
                step(mPlayerAngle, -1, -1, 0xF, 0);
            }
        }

    case 3:
    default:
        return 0;
    }
    return 0;
}

static int daObj_Sekizoa_Create(void* i_this) {
    return static_cast<daObj_Sekizoa_c*>(i_this)->create();
}

static int daObj_Sekizoa_Delete(void* i_this) {
    return static_cast<daObj_Sekizoa_c*>(i_this)->Delete();
}

static int daObj_Sekizoa_Execute(void* i_this) {
    return static_cast<daObj_Sekizoa_c*>(i_this)->Execute();
}

static int daObj_Sekizoa_Draw(void* i_this) {
    return static_cast<daObj_Sekizoa_c*>(i_this)->Draw();
}

static int daObj_Sekizoa_IsDelete(void* i_this) {
    return 1;
}


static daObj_Sekizoa_Param_c l_HIO;

static actor_method_class daObj_Sekizoa_MethodTable = {
    daObj_Sekizoa_Create,   daObj_Sekizoa_Delete, daObj_Sekizoa_Execute,
    daObj_Sekizoa_IsDelete, daObj_Sekizoa_Draw,
};

extern actor_process_profile_definition g_profile_OBJ_SEKIZOA = {
    fpcLy_CURRENT_e,             // mLayerID
    3,                           // mListID
    fpcPi_CURRENT_e,             // mListPrio
    PROC_OBJ_SEKIZOA,            // mProcName
    &g_fpcLf_Method.base,        // sub_method
    sizeof(daObj_Sekizoa_c),     // mSize
    0,                           // mSizeOther
    0,                           // mParameters
    &g_fopAc_Method.base,        // sub_method
    0x12,                        // mPriority
    &daObj_Sekizoa_MethodTable,  // sub_method
    0x00040000,                  // mStatus
    fopAc_ACTOR_e,               // mActorType
    fopAc_CULLBOX_CUSTOM_e,      // cullType
};

AUDIO_INSTANCES
