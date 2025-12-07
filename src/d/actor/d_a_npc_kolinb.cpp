/**
 * @file d_a_npc_kolinb.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_npc_kolinb.h"

enum Kolinb_RES_File_ID {
    /* BCK */
    /* 0x07 */ BCK_KOLINB_DISLIKE = 0x7,
    /* 0x08 */ BCK_KOLINB_F_DISLIKE,
    /* 0x09 */ BCK_KOLINB_F_NOD_A,
    /* 0x0A */ BCK_KOLINB_F_NOD_B,
    /* 0x0B */ BCK_KOLINB_F_SUFFER_A,
    /* 0x0C */ BCK_KOLINB_F_SUFFER_B,
    /* 0x0D */ BCK_KOLINB_F_WAIT_A,
    /* 0x0E */ BCK_KOLINB_NOD_A,
    /* 0x0F */ BCK_KOLINB_NOD_B,
    /* 0x10 */ BCK_KOLINB_SUFFER_A,
    /* 0x11 */ BCK_KOLINB_SUFFER_B,
    /* 0x12 */ BCK_KOLINB_WAIT_A,

    /* BMDR */
    /* 0x15 */ BMDR_KOLINB = 0x15,

    /* BTK */
    /* 0x18 */ BTK_KOLINB = 0x18,
    /* 0x19 */ BTK_KOLINB_DISLIKE,
    /* 0x1A */ BTK_KOLINB_NOD_A,
    /* 0x1B */ BTK_KOLINB_NOD_B,

    /* BTP */
    /* 0x1E */ BTP_KOLINB = 0x1E,
    /* 0x1F */ BTP_KOLINB_F_DISLIKE,
    /* 0x20 */ BTP_KOLINB_F_NOD_A,
    /* 0x21 */ BTP_KOLINB_F_NOD_B,
    /* 0x22 */ BTP_KOLINB_F_SUFFER_A,
    /* 0x23 */ BTP_KOLINB_F_SUFFER_B,
    /* 0x24 */ BTP_KOLINB_F_WAIT_A,

    /* DZB */
    /* 0x27 */ DZB_KOLINB = 0x27,
};

enum zrCb_RES_File_ID {
    /* BCK */
    /* 0x05 */ BCK_ZRCB_F_SUFFER_A = 0x5,
    /* 0x06 */ BCK_ZRCB_F_SUFFER_B,
    /* 0x07 */ BCK_ZRCB_F_WAIT_A,
    /* 0x08 */ BCK_ZRCB_SUFFER_A,
    /* 0x09 */ BCK_ZRCB_SUFFER_B,
    /* 0x0A */ BCK_ZRCB_WAIT_A,

    /* BMDR */
    /* 0x0D */ BMDR_SRCB = 0xD,

    /* BTP */
    /* 0x10 */ BTP_ZRCB_F_SUFFER_A = 0x10,
    /* 0x11 */ BTP_ZRCB_F_SUFFER_B,
    /* 0x12 */ BTP_ZRCB_F_WAIT_A,
};

enum RES_Name {
    /* 0x1 */ KOLINB = 0x1,
    /* 0x2 */ ZRCB,
};

enum FaceMotion {
    /* 0x0 */ FACE_KOLINB_WAIT_A,
    /* 0x1 */ FACE_KOLINB_DISLIKE,
    /* 0x2 */ FACE_KOLINB_NOD_A,
    /* 0x3 */ FACE_KOLINB_SUFFER_A,
    /* 0x4 */ FACE_KOLINB_SUFFER_B,
    /* 0x5 */ FACE_ZRCB_WAIT_A,
    /* 0x6 */ FACE_ZRCB_SUFFER_A,
    /* 0x7 */ FACE_ZRCB_SUFFER_B,
    /* 0x8 */ FACE_NONE,
};

enum Motion {
    /* 0x0 */ MOT_KOLINB_WAIT_A,
    /* 0x1 */ MOT_KOLINB_DISLIKE,
    /* 0x2 */ MOT_KOLINB_NOD,
    /* 0x3 */ MOT_KOLINB_SUFFER_A,
    /* 0x4 */ MOT_KOLINB_SUFFER_B,
    /* 0x5 */ MOT_ZRCB_WAIT_A,
    /* 0x6 */ MOT_ZRCB_SUFFER_A,
    /* 0x7 */ MOT_ZRCB_SUFFER_B,
};

enum TYPE {
    /* 0x0 */ TYPE_0,
    /* 0x1 */ TYPE_1,
    /* 0x2 */ TYPE_ZRCB,
    /* 0x3 */ TYPE_3,
};

static int l_bmdData[2][2] = {
    {BMDR_KOLINB, KOLINB},
    {BMDR_SRCB, ZRCB},
};

static daNpcT_evtData_c l_evtList[2] = {
    {"", 0},
    {"", 0},
};

static char* l_resNameList[3] = {
    "",
    "Kolinb",
    "zrCb",
};

static s8 l_loadResPtrn0[2] = {
    KOLINB, -1
};

static s8 l_loadResPtrn1[3] = {
    KOLINB, ZRCB, -1,
};

static s8* l_loadResPtrnList[4] = {
    l_loadResPtrn0,
    l_loadResPtrn0,
    l_loadResPtrn1,
    NULL,
};

static daNpcT_faceMotionAnmData_c l_faceMotionAnmData[10] = {
    {-1, J3DFrameCtrl::EMode_NONE, 0, BTP_KOLINB, J3DFrameCtrl::EMode_LOOP, KOLINB, 1},
    {BCK_KOLINB_F_WAIT_A, J3DFrameCtrl::EMode_LOOP, KOLINB, BTP_KOLINB_F_WAIT_A, J3DFrameCtrl::EMode_LOOP, KOLINB, 0},
    {BCK_KOLINB_F_SUFFER_A, J3DFrameCtrl::EMode_LOOP, KOLINB, BTP_KOLINB_F_SUFFER_A, J3DFrameCtrl::EMode_LOOP, KOLINB, 0},
    {BCK_KOLINB_F_SUFFER_B, J3DFrameCtrl::EMode_LOOP, KOLINB, BTP_KOLINB_F_SUFFER_B, J3DFrameCtrl::EMode_LOOP, KOLINB, 0},
    {BCK_KOLINB_F_DISLIKE, J3DFrameCtrl::EMode_LOOP, KOLINB, BTP_KOLINB_F_DISLIKE, J3DFrameCtrl::EMode_LOOP, KOLINB, 0},
    {BCK_KOLINB_F_NOD_A, J3DFrameCtrl::EMode_NONE, KOLINB, BTP_KOLINB_F_NOD_A, J3DFrameCtrl::EMode_NONE, KOLINB, 0},
    {BCK_KOLINB_F_NOD_B, J3DFrameCtrl::EMode_LOOP, KOLINB, BTP_KOLINB_F_NOD_B, J3DFrameCtrl::EMode_LOOP, KOLINB, 0},
    {BCK_ZRCB_F_WAIT_A, J3DFrameCtrl::EMode_LOOP, ZRCB, BTP_ZRCB_F_WAIT_A, J3DFrameCtrl::EMode_LOOP, ZRCB, 0},
    {BCK_ZRCB_F_SUFFER_A, J3DFrameCtrl::EMode_LOOP, ZRCB, BTP_ZRCB_F_SUFFER_A, J3DFrameCtrl::EMode_LOOP, ZRCB, 0},
    {BCK_ZRCB_F_SUFFER_B, J3DFrameCtrl::EMode_LOOP, ZRCB, BTP_ZRCB_F_SUFFER_B, J3DFrameCtrl::EMode_LOOP, ZRCB, 0},
};

static daNpcT_motionAnmData_c l_motionAnmData[9] = {
    {BCK_KOLINB_WAIT_A, J3DFrameCtrl::EMode_LOOP, KOLINB, BTK_KOLINB, J3DFrameCtrl::EMode_NONE, KOLINB, 0, 0},
    {BCK_KOLINB_SUFFER_A, J3DFrameCtrl::EMode_LOOP, KOLINB, BTK_KOLINB, J3DFrameCtrl::EMode_NONE, KOLINB, 0, 0},
    {BCK_KOLINB_SUFFER_B, J3DFrameCtrl::EMode_LOOP, KOLINB, BTK_KOLINB, J3DFrameCtrl::EMode_NONE, KOLINB, 0, 0},
    {BCK_KOLINB_DISLIKE, J3DFrameCtrl::EMode_LOOP, KOLINB, BTK_KOLINB_DISLIKE, J3DFrameCtrl::EMode_LOOP, KOLINB, 0, 0},
    {BCK_KOLINB_NOD_A, J3DFrameCtrl::EMode_NONE, KOLINB, BTK_KOLINB_NOD_A, J3DFrameCtrl::EMode_NONE, KOLINB, 0, 0},
    {BCK_KOLINB_NOD_B, J3DFrameCtrl::EMode_LOOP, KOLINB, BTK_KOLINB_NOD_B, J3DFrameCtrl::EMode_LOOP, KOLINB, 0, 0},
    {BCK_ZRCB_WAIT_A, J3DFrameCtrl::EMode_LOOP, ZRCB, -1, J3DFrameCtrl::EMode_NONE, 0, 0, 0},
    {BCK_ZRCB_SUFFER_A, J3DFrameCtrl::EMode_LOOP, ZRCB, -1, J3DFrameCtrl::EMode_NONE, 0, 0, 0},
    {BCK_ZRCB_SUFFER_B, J3DFrameCtrl::EMode_LOOP, ZRCB, -1, J3DFrameCtrl::EMode_NONE, 0, 0, 0},
};

static daNpcT_MotionSeqMngr_c::sequenceStepData_c l_faceMotionSequenceData[36] = {
    {1, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {4, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {5, -1, 1}, {6, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {2, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {3, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {7, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {8, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {9, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {0, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
};

static daNpcT_MotionSeqMngr_c::sequenceStepData_c l_motionSequenceData[32] = {
    {0, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {3, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {4, -1, 1}, {5, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {1, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {2, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {6, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {7, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {8, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
};

char* daNpc_Kolinb_c::mCutNameList[7] = {
    "",
    "CONVERSATION_IN_HOTEL",
    "CONVERSATION_ABOUT_DEATHMT",
    "CONVERSATION_ABOUT_GORON",
    "NURSE",
    "CLOTH_TRY",
    "THANK_YOU",
};

daNpc_Kolinb_c::cutFunc daNpc_Kolinb_c::mCutList[7] = {
    NULL,
    &daNpc_Kolinb_c::cutConversationInHotel,
    &daNpc_Kolinb_c::cutConversationAboutDeathMt,
    &daNpc_Kolinb_c::cutConversationAboutGoron,
    &daNpc_Kolinb_c::cutNurse,
    &daNpc_Kolinb_c::cutClothTry,
    &daNpc_Kolinb_c::cutThankYou
};

daNpc_Kolinb_c::~daNpc_Kolinb_c() {
    if (mpMorf[0] != NULL) {
        mpMorf[0]->stopZelAnime();
    }

    if (mpBgW != NULL) {
        dComIfG_Bgsp().Release(mpBgW);
    }

    deleteRes(l_loadResPtrnList[mType], (const char**)l_resNameList);
}

daNpc_Kolinb_HIOParam const daNpc_Kolinb_Param_c::m = {
    100.0f,
    -3.0f,
    1.0f,
    500.0f,
    255.0f,
    160.0f,
    35.0f,
    50.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
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
    0.0f,
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
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
};

cPhs__Step daNpc_Kolinb_c::create() {
    daNpcT_ct(this, daNpc_Kolinb_c, l_faceMotionAnmData, l_motionAnmData, l_faceMotionSequenceData, 4, l_motionSequenceData, 4, l_evtList, l_resNameList);

    mType = getType();
    mFlowNodeNo = getFlowNodeNo();
    mTwilight = false;

    cPhs__Step phase = (cPhs__Step)loadRes(l_loadResPtrnList[mType], (const char**)l_resNameList);
    if (phase == cPhs_COMPLEATE_e) {
        OS_REPORT("\t(%s:%d) flowNo:%d, modelType:%02x<%08x> ", fopAcM_getProcNameString(this), mType, mFlowNodeNo, (getModelType() >> 32) & 0xFF, fopAcM_GetParam(this));

        if (isDelete()) {
            OS_REPORT("===>isDelete:TRUE\n");
            return cPhs_ERROR_e;
        }

        OS_REPORT("\n");

        static int const heapSize[4] = {
            0x47A0, 0x4790, 0x4710, 0x0,
        };
        if (!fopAcM_entrySolidHeap(this, createHeapCallBack, heapSize[mType])) {
            return cPhs_ERROR_e;
        }

        if (mpBgW != NULL) {
            if (dComIfG_Bgsp().Regist(mpBgW, this) == true) {
                return cPhs_ERROR_e;
            }
        }

        J3DModel* model = mpMorf[0]->getModel();
        fopAcM_SetMtx(this, mpMorf[0]->getModel()->getBaseTRMtx());
        fopAcM_setCullSizeBox(this, -200.0f, -100.0f, -200.0f, 200.0f, 200.0f, 200.0f);

        mSound.init(&current.pos, &eyePos, 3, 1);

        reset();

        mAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1, &mAcchCir, fopAcM_GetSpeed_p(this),
                  fopAcM_GetAngle_p(this), fopAcM_GetShapeAngle_p(this));
        mCcStts.Init(daNpc_Kolinb_Param_c::m.common.weight, 0, this);
        
        field_0xe44.Set(mCcDCyl);
        field_0xe44.SetStts(&mCcStts);
        field_0xe44.SetTgHitCallback(tgHitCallBack);
        
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

int daNpc_Kolinb_c::CreateHeap() {
    int bmdIdx;
    if (mType == TYPE_ZRCB) {
        bmdIdx = 1;
    } else {
        bmdIdx = 0;
    }

    J3DModelData* modelData = static_cast<J3DModelData*>(dComIfG_getObjectRes(l_resNameList[l_bmdData[bmdIdx][1]], l_bmdData[bmdIdx][0]));
    if (modelData == NULL) {
        return 0;
    }

    mpMorf[0] = new mDoExt_McaMorfSO(modelData, NULL, NULL, NULL, -1, 1.0f, 0, -1, &mSound, 0x80000, 0x11020284);
    if (mpMorf[0] == NULL || mpMorf[0]->getModel() == NULL) {
        return 0;
    }

    J3DModel* model = mpMorf[0]->getModel();
    for (u16 i = 0; i < modelData->getJointNum(); i++) {
        modelData->getJointNodePointer(i)->setCallBack(ctrlJointCallBack);
    }
    model->setUserArea((uintptr_t)this);

    mpBgW = new dBgW();
    if (mpBgW == NULL) {
        return 0;
    }

    if (mpBgW->Set((cBgD_t*)dComIfG_getObjectRes(l_resNameList[1], 0x27), 1, &mMtx) == true) {
        return 0;
    }

    mpBgW->SetCrrFunc(dBgS_MoveBGProc_Typical);

    if (mType != TYPE_ZRCB) {
        if (setFaceMotionAnm(1, false) && setMotionAnm(0, 0.0f, FALSE)) {
            return 1;
        } else {
            return 0;
        }
    }

    if (setFaceMotionAnm(7, false) && setMotionAnm(6, 0.0f, FALSE)) {
        return 1;
    } else {
        return 0;
    }
}

int daNpc_Kolinb_c::Delete() {
    this->~daNpc_Kolinb_c();
    return 1;
}

int daNpc_Kolinb_c::Execute() {
    return execute();
}

int daNpc_Kolinb_c::Draw() {
    if (mpMatAnm[0] != NULL) {
        J3DModelData* modelData = mpMorf[0]->getModel()->getModelData();
        modelData->getMaterialNodePointer(getEyeballMaterialNo())->setMaterialAnm(mpMatAnm[0]);
    }

    draw(FALSE, FALSE, mRealShadowSize, NULL, 100.0f, FALSE, FALSE, FALSE);
}

int daNpc_Kolinb_c::createHeapCallBack(fopAc_ac_c* a_this) {
    daNpc_Kolinb_c* i_this = (daNpc_Kolinb_c*)a_this;
    return i_this->CreateHeap();
}

int daNpc_Kolinb_c::ctrlJointCallBack(J3DJoint* i_joint, int param_2) {
    if (param_2 == 0) {
        daNpc_Kolinb_c* i_this = (daNpc_Kolinb_c*)j3dSys.getModel()->getUserArea();
        if (i_this != NULL) {
            i_this->ctrlJoint(i_joint, j3dSys.getModel());
        }
    }

    return 1;
}

u8 daNpc_Kolinb_c::getType() {
    switch (fopAcM_GetParam(this) & 0xFF) {
        case 0:
            return 0;
        
        case 1:
            return 1;

        case 2:
            return 2;

        default:
            return 3;
    }
}

BOOL daNpc_Kolinb_c::isDelete() {
    switch (mType) {
        case TYPE_0:
                    /* dSv_event_flag_c::M_052 - Main Event - Horseback battle clear */
            return !daNpcT_chkEvtBit(85) ||
                    /* dSv_event_flag_c::M_031 - Goron Mines - Goron Mines clear */
                    daNpcT_chkEvtBit(64);
        case TYPE_1:
                    /* dSv_event_flag_c::M_031 - Goron Mines - Goron Mines clear */
            return !daNpcT_chkEvtBit(64) ||
                    /* dSv_event_flag_c::M_035 - Cutscene - [cutscene: 35] after carriage guarding event */
                    daNpcT_chkEvtBit(68);

        case TYPE_ZRCB:
                    /* dSv_event_flag_c::M_035 - Cutscene - [cutscene: 35] after carriage guarding event */
            return !daNpcT_chkEvtBit(68) ||
                    /* dSv_event_flag_c::F_0264 - Cutscene - Get master sword */
                    daNpcT_chkEvtBit(264);

        default:
            return FALSE;
    }
}

void daNpc_Kolinb_c::reset() {
    csXyz angle;
    int iVar1 = (u8*)&field_0xff8 - (u8*)&mNextAction;

    if (mpMatAnm[0] != NULL) {
        mpMatAnm[0]->initialize();
    }

    initialize();

    memset(&mNextAction, 0, iVar1);

    angle.setall(0);
    angle.y = home.angle.y;
    setAngle(angle);
}

void daNpc_Kolinb_c::setParam() {
    selectAction();
    srchActors();

    s16 talk_distance = daNpc_Kolinb_Param_c::m.common.talk_distance;
    s16 talk_angle = daNpc_Kolinb_Param_c::m.common.talk_angle;
    s16 attention_distance = daNpc_Kolinb_Param_c::m.common.attention_distance;
    s16 attention_angle = daNpc_Kolinb_Param_c::m.common.attention_angle;
    attention_info.distances[fopAc_attn_LOCK_e] = daNpcT_getDistTableIdx(attention_distance, attention_angle);
    attention_info.distances[fopAc_attn_TALK_e] = attention_info.distances[fopAc_attn_LOCK_e];
    attention_info.distances[fopAc_attn_SPEAK_e] = daNpcT_getDistTableIdx(talk_distance, talk_angle);
    attention_info.flags = fopAc_AttnFlag_SPEAK_e | fopAc_AttnFlag_TALK_e;

    scale.set(daNpc_Kolinb_Param_c::m.common.scale, daNpc_Kolinb_Param_c::m.common.scale, daNpc_Kolinb_Param_c::m.common.scale);
    mCcStts.SetWeight(daNpc_Kolinb_Param_c::m.common.weight);

    mCylH = daNpc_Kolinb_Param_c::m.common.height;
    mWallR = daNpc_Kolinb_Param_c::m.common.width;
    mAttnFovY = daNpc_Kolinb_Param_c::m.common.fov;
    mAcchCir.SetWallR(mWallR);
    mAcchCir.SetWallH(daNpc_Kolinb_Param_c::m.common.knee_length);
    mRealShadowSize = daNpc_Kolinb_Param_c::m.common.real_shadow_size;
    mExpressionMorfFrame = daNpc_Kolinb_Param_c::m.common.expression_morf_frame;
    mMorfFrames = daNpc_Kolinb_Param_c::m.common.morf_frame;
    gravity = daNpc_Kolinb_Param_c::m.common.gravity;
}

BOOL daNpc_Kolinb_c::checkChangeEvt() {
    if (!chkAction(&daNpc_Kolinb_c::talk)) {
        mPreItemNo = 0;
        if (dComIfGp_event_chkTalkXY()) {
            if (dComIfGp_evmng_ChkPresentEnd()) {
                mEvtNo = 1;
                evtChange();
            }

            return TRUE;
        }
    }

    return FALSE;
}

void daNpc_Kolinb_c::setAfterTalkMotion() {
    mFaceMotionSeqMngr.getNo();
    mFaceMotionSeqMngr.setNo(FACE_NONE, -1.0f, FALSE, 0);
}

void daNpc_Kolinb_c::srchActors() {
    /* empty function */
}

BOOL daNpc_Kolinb_c::evtTalk() {
    if (chkAction(&daNpc_Kolinb_c::talk)) {
        (this->*mAction)(NULL);
    } else {
        setAction(&daNpc_Kolinb_c::talk);
    }

    return TRUE;
}

BOOL daNpc_Kolinb_c::evtCutProc() {
    int staffId = dComIfGp_getEventManager().getMyStaffId("Kolinb", this, -1);
    if (staffId != -1) {
        mStaffId = staffId;
        int actIdx = dComIfGp_getEventManager().getMyActIdx(mStaffId, mCutNameList, 7, 0, 0);
        if ((this->*mCutList[actIdx])(mStaffId)) {
            dComIfGp_getEventManager().cutEnd(mStaffId);
        }

        return TRUE;
    }

    return FALSE;
}

void daNpc_Kolinb_c::action() {
    if (mNextAction) {
        if (mAction == mNextAction) {
            (this->*mAction)(NULL);
        } else {
            setAction(mNextAction);
        }
    }
}

void daNpc_Kolinb_c::beforeMove() {
    fopAcM_OffStatus(this, fopAcM_STATUS_UNK_0x8000000);

    if (checkHide()) {
        fopAcM_OnStatus(this, fopAcM_STATUS_UNK_0x8000000);
    }

    if (checkHide() || mNoDraw) {
        attention_info.flags = 0;
    }
}

void daNpc_Kolinb_c::setAttnPos() {
    cXyz work(10.0f, 30.0f, 0.0f);
    setMtx();

    if (mpBgW != NULL) {
        MTXCopy(mpMorf[0]->getModel()->getBaseTRMtx(), mMtx);
        mpBgW->Move();
    }

    mDoMtx_stack_c::copy(mpMorf[0]->getModel()->getAnmMtx(getHeadJointNo()));
    mDoMtx_stack_c::multVec(&work, &eyePos);
    work.set(100.0f, 0.0f, 0.0f);
    work.y = daNpc_Kolinb_Param_c::m.common.attention_offset;
    mDoMtx_stack_c::YrotS(mCurAngle.y);
    mDoMtx_stack_c::multVec(&work, &work);
    attention_info.position = current.pos + work;

    if (mType == TYPE_ZRCB) {
        mSound.startCreatureVoiceLevel(Z2SE_ZRC_V_UNASARE, -1);
    }
}

void daNpc_Kolinb_c::setCollision() {
    field_0xe44.ClrCoHit();
    field_0xe44.ClrTgHit();
}

int daNpc_Kolinb_c::drawDbgInfo() {
    return 0;
}

int daNpc_Kolinb_c::selectAction() {
    mNextAction = NULL;

#if DEBUG
    if (daNpc_Kolinb_Param_c::m.common.debug_mode_ON) {
        mNextAction = &daNpc_Kolinb_c::test;
        return 1;
    }
#endif

    mNextAction = &daNpc_Kolinb_c::wait;

    return 1;
}

BOOL daNpc_Kolinb_c::chkAction(actionFunc action) {
    return mAction == action;
}

int daNpc_Kolinb_c::setAction(actionFunc action) {
    mMode = 3;
    
    if (mAction) {
        (this->*mAction)(NULL);
    }

    mMode = 0;

    mAction = action;
    if (mAction) {
        (this->*mAction)(NULL);
    }

    return 1;
}

int daNpc_Kolinb_c::cutConversationInHotel(int i_cutIndex) {
    int rv = 0;
    int prm = -1;

    int* piVar1 = dComIfGp_evmng_getMyIntegerP(i_cutIndex, "prm");
    if (piVar1 != NULL) {
        prm = *piVar1;
    }

    dComIfGp_getEventManager().getIsAddvance(i_cutIndex);

    switch (prm) {
        case 0:
            rv = 1;
            break;
    }

    return rv;
}

int daNpc_Kolinb_c::cutConversationAboutDeathMt(int i_cutIndex) {
    int rv = 0;
    int prm = -1;

    int* piVar1 = dComIfGp_evmng_getMyIntegerP(i_cutIndex, "prm");
    if (piVar1 != NULL) {
        prm = *piVar1;
    }

    if (dComIfGp_getEventManager().getIsAddvance(i_cutIndex)) {
        switch (prm) {
            case 0:
                mEventTimer = 61;
                break;

            case 1:
                if (mType == TYPE_ZRCB) {
                    mFaceMotionSeqMngr.setNo(FACE_ZRCB_WAIT_A, -1.0f, FALSE, 0);
                    mMotionSeqMngr.setNo(MOT_ZRCB_WAIT_A, -1.0f, FALSE, 0);
                } else {
                    mFaceMotionSeqMngr.setNo(FACE_KOLINB_WAIT_A, -1.0f, FALSE, 0);
                    mMotionSeqMngr.setNo(MOT_KOLINB_WAIT_A, -1.0f, FALSE, 0);
                }
                break;
        }
    }

    switch (prm) {
        case 0:
            action();

            if (mEventTimer != 0 && cLib_calcTimer<int>(&mEventTimer) == 0) {
                if (mType == TYPE_ZRCB) {
                    mFaceMotionSeqMngr.setNo(FACE_ZRCB_SUFFER_A, -1.0f, FALSE, 0);
                    mMotionSeqMngr.setNo(MOT_ZRCB_SUFFER_A, -1.0f, FALSE, 0);
                } else {
                    mFaceMotionSeqMngr.setNo(FACE_KOLINB_SUFFER_A, -1.0f, FALSE, 0);
                    mMotionSeqMngr.setNo(MOT_KOLINB_SUFFER_A, -1.0f, FALSE, 0);
                }
            }

            rv = 1;
            break;

        case 1:
            rv = 1;
            break;
    }

    return rv;
}

int daNpc_Kolinb_c::cutConversationAboutGoron(int i_cutIndex) {
    int rv = 0;
    int prm = -1;

    int* piVar1 = dComIfGp_evmng_getMyIntegerP(i_cutIndex, "prm");
    if (piVar1 != NULL) {
        prm = *piVar1;
    }

    if (dComIfGp_getEventManager().getIsAddvance(i_cutIndex)) {
        switch (prm) {
            case 0:
                mEventTimer = 61;
                break;

            case 1:
                if (mType == TYPE_ZRCB) {
                    mFaceMotionSeqMngr.setNo(FACE_ZRCB_WAIT_A, -1.0f, FALSE, 0);
                    mMotionSeqMngr.setNo(MOT_ZRCB_WAIT_A, -1.0f, FALSE, 0);
                } else {
                    mFaceMotionSeqMngr.setNo(MOT_KOLINB_WAIT_A, -1.0f, FALSE, 0);
                    mMotionSeqMngr.setNo(MOT_KOLINB_WAIT_A, -1.0f, FALSE, 0);
                }
                break;
        }
    }

    switch (prm) {
        case 0:
            action();

            if (mEventTimer != 0 && cLib_calcTimer<int>(&mEventTimer) == 0) {
                if (mType == TYPE_ZRCB) {
                    mFaceMotionSeqMngr.setNo(FACE_ZRCB_SUFFER_B, -1.0f, FALSE, 0);
                    mMotionSeqMngr.setNo(MOT_ZRCB_SUFFER_B, -1.0f, FALSE, 0);
                } else {
                    mFaceMotionSeqMngr.setNo(FACE_KOLINB_SUFFER_B, -1.0f, FALSE, 0);
                    mMotionSeqMngr.setNo(MOT_KOLINB_SUFFER_B, -1.0f, FALSE, 0);
                }
            }

            rv = 1;
            break;

        case 1:
            rv = 1;
            break;
    }

    return rv;
}

int daNpc_Kolinb_c::cutNurse(int i_cutIndex) {
    int rv = 0;
    int prm = -1;

    int* piVar1 = dComIfGp_evmng_getMyIntegerP(i_cutIndex, "prm");
    if (piVar1 != NULL) {
        prm = *piVar1;
    }

    if (dComIfGp_getEventManager().getIsAddvance(i_cutIndex)) {
        switch (prm) {
            case 0:
                break;

            case 1:
                mFaceMotionSeqMngr.setNo(FACE_KOLINB_NOD_A, -1.0f, FALSE, 0);
                mMotionSeqMngr.setNo(MOT_KOLINB_NOD, -1.0f, FALSE, 0);
                break;

            case 2:
                mFaceMotionSeqMngr.setNo(FACE_KOLINB_DISLIKE, 0.0f, TRUE, 0);
                mMotionSeqMngr.setNo(MOT_KOLINB_DISLIKE, 0.0f, TRUE, 0);
                break;
        }
    }

    switch (prm) {
        case 0:
        case 1:
        case 2:
            field_0xe26 = false;
            rv = 1;
            break;
    }

    return rv;
}

int daNpc_Kolinb_c::cutClothTry(int i_cutIndex) {
    int rv = 0;
    int prm = -1;

    int* piVar1 = dComIfGp_evmng_getMyIntegerP(i_cutIndex, "prm");
    if (piVar1 != NULL) {
        prm = *piVar1;
    }

    dComIfGp_getEventManager().getIsAddvance(i_cutIndex);

    switch (prm) {
        case 0:
            action();
            rv = 1;
            break;
    }

    return rv;
}

int daNpc_Kolinb_c::cutThankYou(int i_cutIndex) {
    int rv = 0;
    int prm = -1;

    int* piVar1 = dComIfGp_evmng_getMyIntegerP(i_cutIndex, "prm");
    if (piVar1 != NULL) {
        prm = *piVar1;
    }

    dComIfGp_getEventManager().getIsAddvance(i_cutIndex);

    switch (prm) {
        case 0:
            action();
            rv = 1;
            break;
    }

    return rv;
}

int daNpc_Kolinb_c::wait(void* param_1) {
    switch (mMode) {
        case 0:
        case 1:
            switch (mType) {
                case TYPE_1:
                    mFaceMotionSeqMngr.setNo(FACE_KOLINB_DISLIKE, -1.0f, FALSE, 0);
                    mMotionSeqMngr.setNo(MOT_KOLINB_DISLIKE, -1.0f, FALSE, 0);
                    break;

                case TYPE_ZRCB:
                    mFaceMotionSeqMngr.setNo(FACE_ZRCB_WAIT_A, -1.0f, FALSE, 0);
                    mMotionSeqMngr.setNo(MOT_ZRCB_WAIT_A, -1.0f, FALSE, 0);
                    break;

                default:
                    mFaceMotionSeqMngr.setNo(FACE_KOLINB_WAIT_A, -1.0f, FALSE, 0);
                    mMotionSeqMngr.setNo(MOT_KOLINB_WAIT_A, -1.0f, FALSE, 0);
                    break;
            }

            mMode = 2;
            // fallthrough
        case 2:
            attention_info.flags = 0;
            break;

        case 3:
            break;
    }

    return 1;
}

int daNpc_Kolinb_c::talk(void* param_1) {
    switch (mMode) {
        case 0:
        case 1:
            initTalk(mFlowNodeNo, NULL);
            mMode = 2;
            // fallthrough
        case 2:
            if (talkProc(NULL, FALSE, NULL, FALSE) && mFlow.checkEndFlow()) {
                mPlayerActorMngr.entry(daPy_getPlayerActorClass());
                dComIfGp_event_reset();
                mMode = 3;
            }
            break;

        case 3:
            break;
    }

    return 0;
}

static int daNpc_Kolinb_Create(void* a_this) {
    daNpc_Kolinb_c* i_this = (daNpc_Kolinb_c*)a_this;
    return i_this->create();
}

static int daNpc_Kolinb_Delete(void* a_this) {
    daNpc_Kolinb_c* i_this = (daNpc_Kolinb_c*)a_this;
    return i_this->Delete();
}

static int daNpc_Kolinb_Execute(void* a_this) {
    daNpc_Kolinb_c* i_this = (daNpc_Kolinb_c*)a_this;
    return i_this->Execute();
}

static int daNpc_Kolinb_Draw(void* a_this) {
    daNpc_Kolinb_c* i_this = (daNpc_Kolinb_c*)a_this;
    return i_this->Draw();
}

static int daNpc_Kolinb_IsDelete(void* a_this) {
    return 1;
}

static daNpc_Kolinb_Param_c l_HIO;

static actor_method_class daNpc_Kolinb_MethodTable = {
    (process_method_func)daNpc_Kolinb_Create,
    (process_method_func)daNpc_Kolinb_Delete,
    (process_method_func)daNpc_Kolinb_Execute,
    (process_method_func)daNpc_Kolinb_IsDelete,
    (process_method_func)daNpc_Kolinb_Draw,
};

extern actor_process_profile_definition g_profile_NPC_KOLINB = {
  fpcLy_CURRENT_e,           // mLayerID
  7,                         // mListID
  fpcPi_CURRENT_e,           // mListPrio
  PROC_NPC_KOLINB,           // mProcName
  &g_fpcLf_Method.base,     // sub_method
  sizeof(daNpc_Kolinb_c),    // mSize
  0,                         // mSizeOther
  0,                         // mParameters
  &g_fopAc_Method.base,      // sub_method
  351,                       // mPriority
  &daNpc_Kolinb_MethodTable, // sub_method
  0x00040107,                // mStatus
  fopAc_NPC_e,               // mActorType
  fopAc_CULLBOX_CUSTOM_e,    // cullType
};
