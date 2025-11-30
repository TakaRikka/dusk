/**
 * d_a_npc_jagar.cpp
 * NPC - Jaggle
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_npc_jagar.h"
#include "Z2AudioLib/Z2Instances.h"
#include "d/actor/d_a_npc_bou.h"
#include "d/d_meter2_info.h"
#include "d/actor/d_a_tag_push.h"

enum Jagar_RES_File_ID {
    /* BCK */
    /* 0x06 */ BCK_JAGA_F_TALK_A = 6,
    /* 0x07 */ BCK_JAGA_FH_TALK_A,
    /* 0x08 */ BCK_JAGA_STEP,
    /* 0x09 */ BCK_JAGA_TALK_B,
    /* 0x0A */ BCK_JAGAR_WAIT_A,

    /* BMDR */
    /* 0x0D */ BMDR_JAGAR = 0xD,

    /* BTK */
    /* 0x10 */ BTK_JAGA = 0x10,

    /* BTP */
    /* 0x13 */ BTP_JAGA = 0x13,
    /* 0x14 */ BTP_JAGA_F_TALK_A,
    /* 0x15 */ BTP_JAGA_FH_TALK_A,
};

enum Jagar1_RES_File_ID {
    /* BCK */
    /* 0x04 */ BCK_JAGA_CHU = 4,
    /* 0x05 */ BCK_JAGA_CHU_TO_SIT,
    /* 0x06 */ BCK_JAGA_F_SMILETALK,
    /* 0x07 */ BCK_JAGA_SIT,
    /* 0x08 */ BCK_JAGA_SIT_TO_WAIT,
    /* 0x09 */ BCK_JAGA_TO_CHU,
    /* 0x0A */ BCK_JAGA_WAIT_B,
    /* 0x0B */ BCK_JAGA_WAIT_TO_SIT,

    /* EVT */
    /* 0x0E */ EVT_JAGAR1_EVENT_LIST = 0xE,
};

enum Jagar2_RES_File_ID {
    /* BCK */
    /* 0x05 */ BCK_JAGA_F_SURPRISE = 0x5,
    /* 0x06 */ BCK_JAGA_F_SURPRISE_WAIT,
    /* 0x07 */ BCK_JAGA_F_WORRY_TALK,
    /* 0x08 */ BCK_JAGA_RUN_A,
    /* 0x09 */ BCK_JAGA_SURPRISE,
    /* 0x0A */ BCK_JAGA_SURPRISE_WAIT,
    /* 0x0B */ BCK_JAGA_UNADUKI,
    /* 0x0C */ BCK_JAGA_WORRY_TALK,
    /* 0x0D */ BCK_JAGA_WORRY_WAIT,
    /* 0x0E */ BCK_JAGA_WORRY_WALK,

    /* BTP */
    /* 0x11 */ BTP_JAGA_BLINK = 0x11,
    /* 0x12 */ BTP_JAGA_F_SURPRISE,
    /* 0x13 */ BTP_JAGA_F_SURPRISE_WAIT,
    /* 0x14 */ BTP_JAGA_F_WORRY_TALK,

    /* EVT */
    /* 0x17 */ EVT_JAGAR2_EVENT_LIST = 0x17,
};

enum Jagar3_RES_File_ID {
    /* BCK */
    /* 0x04 */ BCK_JAGA_F_TALK_B = 4,
    /* 0x05 */ BCK_JAGA_FH_TALK_B,
    /* 0x06 */ BCK_JAGA_TALK_A,

    /* BTP */
    /* 0x09 */ BTP_JAGA_F_TALK_B = 9,
    /* 0x0A */ BTP_JAGA_FH_TALK_B,
};

enum Jagar4_RES_File_ID {
    /* BMDR */
    /* 0x03 */ BMDR_JAGA_NOUGU = 3,
};

enum RES_Name {
    /* 0x1 */ Jagar = 0x1,
    /* 0x2 */ Jagar1,
    /* 0x3 */ Jagar2,
    /* 0x4 */ Jagar3,
};

enum FaceMotion {
    /* 0x00 */ FACE_TALK_A,
    /* 0x01 */ FACE_SURPRISE,
    /* 0x02 */ FACE_SMILETALK,
    /* 0x03 */ FACE_TALK_B,
    /* 0x04 */ FACE_WORRY_TALK,
    /* 0x05 */ FACE_H_TALK_B,
    /* 0x06 */ FACE_H_TALK_A,
    /* 0x07 */ FACE_7,
    /* 0x08 */ FACE_NONE,
};

enum Motion {
    /* 0x00 */ MOT_WAIT_A,
    /* 0x01 */ MOT_TALK_A,
    /* 0x02 */ MOT_CHU,
    /* 0x03 */ MOT_SIT,
    /* 0x04 */ MOT_TALK_B,
    /* 0x05 */ MOT_WORRY_WAIT,
    /* 0x06 */ MOT_WORRY_TALK,
    /* 0x07 */ MOT_SURPRISE,
    /* 0x08 */ MOT_UNADUKI,
    /* 0x09 */ MOT_SURPRISE_WAIT,
    /* 0x0A */ MOT_SIT_TO_WAIT,
    /* 0x0B */ MOT_TO_CHU,
    /* 0x0C */ MOT_CHU_TO_SIT,
    /* 0x0D */ MOT_WAIT_TO_SIT,
    /* 0x0E */ MOT_STEP,
    /* 0x0F */ MOT_RUN_A,
    /* 0x10 */ MOT_WORRY_WALK,
    /* 0x11 */ MOT_WAIT_B,
};

enum Event {
    /* 0x0 */ EVENT_NONE,
    /* 0x1 */ EVENT_NO_RESPONSE,
    /* 0x2 */ EVENT_CLIMBUP,
    /* 0x3 */ EVENT_NEED_YOUR_HELP,
    /* 0x4 */ EVENT_ANGER,
    /* 0x5 */ EVENT_CONVERSATION_WITH_BOU,
    /* 0x6 */ EVENT_CONFIDENTIAL_CONVERSATION,
    /* 0x7 */ EVENT_FIND_WOLF,
    /* 0x8 */ EVENT_FIND_WOLF_VER2,
};

daNpc_Jagar_HIOParam const daNpc_Jagar_Param_c::m = {
    170.0f, -3.0f, 1.0f, 400.0f, 255.0f, 160.0f,
    35.0f, 30.0f, 0.0f, 0.0f, 10.0f, -10.0f,
    30.0f, -10.0f, 45.0f, -45.0f, 0.6f, 12.0f,
    3, 6, 5, 6, 110.0f, 500.0f, 300.0f, -300.0f,
    60, 8, 0, 0, 0, 0.0f, 0.0f, 4.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    // Jagar-specific:
    1400.0f, 200.0f, -800.0f, 16.0f, 1800.0f,
};

#if DEBUG
daNpc_Jagar_HIO_c::daNpc_Jagar_HIO_c() {
    m = daNpc_Jagar_Param_c::m;
}

void daNpc_Jagar_HIO_c::listenPropertyEvent(const JORPropertyEvent* event) {
    // TODO.
}

void daNpc_Jagar_HIO_c::genMessage(JORMContext* ctext) {
    // TODO.
    daNpcT_cmnGenMessage(ctext, &m.common);
    // pumpkin monitoring distance
    ctext->genSlider("かぼちゃ監視距離", &m.pumpkin_watch_range, 0.0f, 1000.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
    // pumpkin watch height
    ctext->genSlider("かぼちゃ監視高さ", &m.pumpkin_watch_Ymax, -10000.0f, 10000.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
    // "Pumpkin monitoring is low"?
    ctext->genSlider("かぼちゃ監視低さ", &m.pumpkin_watch_Ymin, -10000.0f, 10000.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
    // running speed
    ctext->genSlider("走る速度        ", &m.running_speed, 0.0f, 10000.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
    // hidden state release dist
    ctext->genSlider("隠れ状態解除距離", &m.hidden_state_release_dist, 0.0f, 10000.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
    // export file:
    ctext->genButton("ファイル書き出し", 0x40000002, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
}
#endif

static int l_bmdData[1][2] = {
    {BMDR_JAGAR, 1},
};

static daNpcT_evtData_c l_evtList[9] = {
    {"", 0},
    {"NO_RESPONSE", 0},
    {"CLIMBUP", 2},
    {"NEED_YOUR_HELP", 2},
    {"ANGER", 2},
    {"CONVERSATION_WITH_BOU", 3},
    {"CONFIDENTIAL_CONVERSATION", 3},
    {"FIND_WOLF", 3},
    {"FIND_WOLF_VER2", 3},
};

static char* l_resNameList[5] = {
    "",
    "Jagar",
    "Jagar1",
    "Jagar2",
    "Jagar3",
};

static s8 l_loadResPtrn0[4] = {
    1, 2, 4, -1,
};

static s8 l_loadResPtrn1[3] = {
    1, 3, -1
};

static s8 l_loadResPtrn9[5] = {
    1, 2, 3, 4, -1
};

static s8* l_loadResPtrnList[4] = {
    l_loadResPtrn0,
    l_loadResPtrn1,
    l_loadResPtrn9,
    l_loadResPtrn9,
};

static daNpcT_faceMotionAnmData_c l_faceMotionAnmData[10] = {
    {-1, J3DFrameCtrl::EMode_NONE, 0,
        BTP_JAGA, J3DFrameCtrl::EMode_LOOP, Jagar, 1},
    {BCK_JAGA_F_TALK_A, J3DFrameCtrl::EMode_NONE, Jagar,
        BTP_JAGA_F_TALK_A, J3DFrameCtrl::EMode_NONE, Jagar, 0},
    {BCK_JAGA_F_SMILETALK, J3DFrameCtrl::EMode_NONE, Jagar1,
        BTP_JAGA, J3DFrameCtrl::EMode_LOOP, Jagar, 1},
    {BCK_JAGA_F_TALK_B, J3DFrameCtrl::EMode_NONE, Jagar3,
        BTP_JAGA_F_TALK_B, J3DFrameCtrl::EMode_NONE, Jagar3, 0},
    {BCK_JAGA_F_WORRY_TALK, J3DFrameCtrl::EMode_NONE, Jagar2,
        BTP_JAGA_F_WORRY_TALK, J3DFrameCtrl::EMode_NONE, Jagar2, 0},
    {BCK_JAGA_FH_TALK_B, J3DFrameCtrl::EMode_LOOP, Jagar3,
        BTP_JAGA_FH_TALK_B, J3DFrameCtrl::EMode_LOOP, Jagar3, 0},
    {-1, J3DFrameCtrl::EMode_NONE, 0,
        -1, J3DFrameCtrl::EMode_NONE, 0, 0},
    {BCK_JAGA_FH_TALK_A, J3DFrameCtrl::EMode_LOOP, Jagar,
        BTP_JAGA_FH_TALK_A, J3DFrameCtrl::EMode_LOOP, Jagar, 0},
    {BCK_JAGA_F_SURPRISE, J3DFrameCtrl::EMode_NONE, Jagar2,
        BTP_JAGA_F_SURPRISE, J3DFrameCtrl::EMode_NONE, Jagar2, 0},
    {BCK_JAGA_F_SURPRISE_WAIT, J3DFrameCtrl::EMode_LOOP, Jagar2,
        BTP_JAGA_F_SURPRISE_WAIT, J3DFrameCtrl::EMode_LOOP, Jagar2, 0},
};

daNpcT_motionAnmData_c l_motionAnmData[18] = {
    {BCK_JAGAR_WAIT_A, J3DFrameCtrl::EMode_LOOP, Jagar,
        BTK_JAGA, J3DFrameCtrl::EMode_NONE, Jagar, 1, 0},
    {BCK_JAGA_CHU, J3DFrameCtrl::EMode_LOOP, Jagar1,
        BTK_JAGA, J3DFrameCtrl::EMode_NONE, Jagar, 1, 0},
    {BCK_JAGA_SIT, J3DFrameCtrl::EMode_LOOP, Jagar1,
        BTK_JAGA, J3DFrameCtrl::EMode_NONE, Jagar, 1, 0},
    {BCK_JAGA_WAIT_B, J3DFrameCtrl::EMode_LOOP, Jagar1,
        BTK_JAGA, J3DFrameCtrl::EMode_NONE, Jagar, 1, 0},
    {BCK_JAGA_SIT_TO_WAIT, J3DFrameCtrl::EMode_NONE, Jagar1,
        BTK_JAGA, J3DFrameCtrl::EMode_NONE, Jagar, 1, 0},
    {BCK_JAGA_TO_CHU, J3DFrameCtrl::EMode_NONE, Jagar1,
        BTK_JAGA, J3DFrameCtrl::EMode_NONE, Jagar, 1, 0},
    {BCK_JAGA_CHU_TO_SIT, J3DFrameCtrl::EMode_NONE, Jagar1,
        BTK_JAGA, J3DFrameCtrl::EMode_NONE, Jagar, 1, 0},
    {BCK_JAGA_WAIT_TO_SIT, J3DFrameCtrl::EMode_NONE, Jagar1,
        BTK_JAGA, J3DFrameCtrl::EMode_NONE, Jagar, 1, 0},
    {BCK_JAGA_TALK_A, J3DFrameCtrl::EMode_NONE, Jagar3,
        BTK_JAGA, J3DFrameCtrl::EMode_NONE, Jagar, 1, 0},
    {BCK_JAGA_TALK_B, J3DFrameCtrl::EMode_NONE, Jagar,
        BTK_JAGA, J3DFrameCtrl::EMode_NONE, Jagar, 1, 0},
    {BCK_JAGA_STEP, J3DFrameCtrl::EMode_NONE, Jagar,
        BTK_JAGA, J3DFrameCtrl::EMode_NONE, Jagar, 1, 0},
    {BCK_JAGA_RUN_A, J3DFrameCtrl::EMode_LOOP, Jagar2,
        BTK_JAGA, J3DFrameCtrl::EMode_NONE, Jagar, 1, 0},
    {BCK_JAGA_WORRY_WAIT, J3DFrameCtrl::EMode_LOOP, Jagar2,
        BTK_JAGA, J3DFrameCtrl::EMode_NONE, Jagar, 1, 0},
    {BCK_JAGA_WORRY_TALK, J3DFrameCtrl::EMode_NONE, Jagar2,
        BTK_JAGA, J3DFrameCtrl::EMode_NONE, Jagar, 1, 0},
    {BCK_JAGA_WORRY_WALK, J3DFrameCtrl::EMode_LOOP, Jagar2,
        BTK_JAGA, J3DFrameCtrl::EMode_NONE, Jagar, 1, 0},
    {BCK_JAGA_SURPRISE, J3DFrameCtrl::EMode_NONE, Jagar2,
        BTK_JAGA, J3DFrameCtrl::EMode_NONE, Jagar, 1, 0},
    {BCK_JAGA_UNADUKI, J3DFrameCtrl::EMode_NONE, Jagar2,
        BTK_JAGA, J3DFrameCtrl::EMode_NONE, Jagar, 1, 0},
    {BCK_JAGA_SURPRISE_WAIT, J3DFrameCtrl::EMode_LOOP, Jagar2,
        BTK_JAGA, J3DFrameCtrl::EMode_NONE, Jagar, 1, 0},
};

static daNpcT_MotionSeqMngr_c::sequenceStepData_c l_faceMotionSequenceData[36] = {
    {1, -1, 1}, {7, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {8, -1, 1}, {9, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {2, -1, 1}, {6, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, 
    {3, -1, 1}, {5, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {4, -1, 1}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, 
    {5, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {7, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {6, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {0, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
};

static daNpcT_MotionSeqMngr_c::sequenceStepData_c l_motionSequenceData[76] = {
    {0, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, 
    {8, -1, 1}, {0, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {1, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, 
    {2, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {9, -1, 1}, {3, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, 
    {12, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {13, -1, 1}, {12, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, 
    {15, -1, 1}, {17, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {16, -1, 1}, {12, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, 
    {17, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {4, -1, 1}, {0, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, 
    {5, -1, 1}, {1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {6, -1, 1}, {2, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, 
    {7, -1, 1}, {2, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {10, 4, 1}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, 
    {11, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {14, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, 
    {3, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {13, -1, 1}, {12, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
};

char* daNpc_Jagar_c::mCutNameList[7] = {
    "",
    "CLIMBUP",
    "NEED_YOUR_HELP",
    "ANGER",
    "CONVERSATION_WITH_BOU",
    "CONFIDENTIAL_CONVERSATION",
    "FIND_WOLF",
};

daNpc_Jagar_c::cutFunc daNpc_Jagar_c::mCutList[7] = {
    NULL,
    &daNpc_Jagar_c::cutClimbUp,
    &daNpc_Jagar_c::cutNeedYourHelp,
    &daNpc_Jagar_c::cutAnger,
    &daNpc_Jagar_c::cutConversationWithBou,
    &daNpc_Jagar_c::cutConfidentialConversation,
    &daNpc_Jagar_c::cutFindWolf,
};

static NPC_JAGAR_HIO_CLASS l_HIO;

daNpc_Jagar_c::~daNpc_Jagar_c() {
    if (mpMorf[0] != 0) {
        mpMorf[0]->stopZelAnime();
    }
    deleteRes(l_loadResPtrnList[mType], (char const**)l_resNameList);
}

int daNpc_Jagar_c::create() {
    static int const heapSize[4] = {14416, 14448, 14448, 0};
    daNpcT_ct(this, daNpc_Jagar_c, l_faceMotionAnmData,
        (const daNpcT_motionAnmData_c *)l_motionAnmData, (const daNpcT_MotionSeqMngr_c::sequenceStepData_c *) l_faceMotionSequenceData, 4,
        (const daNpcT_MotionSeqMngr_c::sequenceStepData_c *)l_motionSequenceData, 4, (const daNpcT_evtData_c *)l_evtList, (char **)l_resNameList
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
        field_0x9c0.init(&mAcch, 0.0f, 0.0f);
        reset();
        mAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1,
                        &mAcchCir, fopAcM_GetSpeed_p(this), fopAcM_GetAngle_p(this),
                        fopAcM_GetShapeAngle_p(this));
        mCcStts.Init(mpHIO->m.common.weight, 0, this);
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

int daNpc_Jagar_c::CreateHeap() {
    J3DModelData* modelData = static_cast<J3DModelData*>(dComIfG_getObjectRes(
        l_resNameList[l_bmdData[0][1]], l_bmdData[0][0]));
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

    mpMatAnm[0] = new daNpcT_MatAnm_c();
    if (mpMatAnm[0] == NULL) {
        return 0;
    }

    if (setFaceMotionAnm(1, false) && setMotionAnm(0, 0.0f, 0)) {
        return 1;
    }

    return 0;
}

int daNpc_Jagar_c::Delete() {
    fopAcM_GetID(this);
    this->~daNpc_Jagar_c();
    return 1;
}

int daNpc_Jagar_c::Execute() {
    return daNpcT_c::execute();
}

int daNpc_Jagar_c::Draw() {
    if (mpMatAnm[0] != NULL) {
        J3DModelData* modelData = mpMorf[0]->getModel()->getModelData();
        modelData->getMaterialNodePointer(getEyeballMaterialNo())->setMaterialAnm(mpMatAnm[0]);
    }
    return daNpcT_c::draw(0, 0, mRealShadowSize, NULL, 100.0f, 0, 0, 0);
}

int daNpc_Jagar_c::createHeapCallBack(fopAc_ac_c* i_this) {
    return static_cast<daNpc_Jagar_c*>(i_this)->CreateHeap();
}

int daNpc_Jagar_c::ctrlJointCallBack(J3DJoint* i_joint, int param_2) {
    if (param_2 == 0) {
        J3DModel* model = j3dSys.getModel();
        daNpc_Jagar_c* i_this = reinterpret_cast<daNpc_Jagar_c*>(model->getUserArea());
        if (i_this != 0) {
            i_this->ctrlJoint(i_joint, model);
        }
    }
    return 1;
}

u8 daNpc_Jagar_c::getType() {
    switch ((u8)fopAcM_GetParam(this)) {
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

int daNpc_Jagar_c::isDelete() {
    switch (mType) {
        case TYPE_0:
            return FALSE;

        case TYPE_1: {
            bool rv = true;
            if (!daNpcT_chkEvtBit(0xD3) /* dSv_event_flag_c::F_0211 - Ordon Village - Successfully eavesdrop on Bo and Jaggle */
                && !dComIfGs_isCollectShield(0)) {
                rv = false;
            }
            return rv;
        }
        // fallthrough
        case TYPE_2:
            return FALSE;

        default:
            return FALSE;
    }
}

void daNpc_Jagar_c::reset() {
    csXyz acStack_20;
    int iVar1 = (u8*)&field_0x1008 - (u8*)&field_0xfd4;
    if (mpMatAnm[0] != NULL) {
        mpMatAnm[0]->initialize();
    }

    initialize();
    for (int i = 0; i < 5; i++) {
        mActorMngr[i].initialize();
    }
    
    memset(&field_0xfd4, 0, iVar1);
    acStack_20.setall(0);
    acStack_20.y = home.angle.y;
    switch (mType) {
        case TYPE_0:
            if (daNpcT_chkEvtBit(0x1C) /* dSv_event_flag_c::F_0023 - Ordon Village - Called by Jaggle from below hill */) {
                if (!daNpcT_chkEvtBit(0x86) /* dSv_event_flag_c::F_0036 - Ordon Village - Spoke to Jaggle using L-focus before climbing vines? */) {
                    daNpcT_onEvtBit(0x86);
                }
                field_0x1001 = 1;
            }
            // fallthrough
        case TYPE_1:
        case TYPE_2:
        default:
            daNpcT_offTmpBit(0x1B); // dSv_event_tmp_flag_c::T_0027 - Ordon Village - Rampaging goats complete
            daNpcT_offTmpBit(0x10); // dSv_event_tmp_flag_c::T_0015 - Ordon Village - Link came up the hill afte being called by Jaggle
            setAngle(acStack_20);
    }
}

void daNpc_Jagar_c::afterJntAnm(int param_1) {
    if (param_1 == 1) {
        mDoMtx_stack_c::YrotM(mStagger.getAngleZ(1));
        mDoMtx_stack_c::ZrotM(-mStagger.getAngleX(1));
    } else if (param_1 == 4) {
        mDoMtx_stack_c::YrotM(mStagger.getAngleZ(0));
        mDoMtx_stack_c::ZrotM(mStagger.getAngleX(0));
    }
}

void daNpc_Jagar_c::setParam() {
    selectAction();
    srchActors();
    u32 uVar7 = (fopAc_AttnFlag_SPEAK_e | fopAc_AttnFlag_TALK_e);
    s16 talk_dist = mpHIO->m.common.talk_distance;
    s16 talk_ang = mpHIO->m.common.talk_angle;
    s16 att_dist = mpHIO->m.common.attention_distance;
    s16 att_ang = mpHIO->m.common.attention_angle;

    switch (mType) {
        case TYPE_0:
            talk_dist = 4;
            talk_ang = 6;
            att_dist = 5;
            att_ang = 6;
            break;

        case TYPE_1:
            field_0xff0 = 3;
            field_0xff4 = 6;
            talk_dist = 19;
            talk_ang = 6;
            att_dist = 19;
            att_ang = 6;
            break;

        case TYPE_2:
            talk_dist = 3;
            talk_ang = 6;
            att_dist = 5;
            att_ang = 6;
            break;
    }
    
    attention_info.distances[fopAc_attn_LOCK_e] = daNpcT_getDistTableIdx(att_dist, att_ang);
    attention_info.distances[fopAc_attn_TALK_e] = attention_info.distances[fopAc_attn_LOCK_e];
    attention_info.distances[fopAc_attn_SPEAK_e] = daNpcT_getDistTableIdx(talk_dist, talk_ang);

    if (mType == TYPE_1) {
        uVar7 |= fopAc_AttnFlag_UNK_0x800000;
        field_0xfec = getActorDistance(daPy_getPlayerActorClass(), 
                    daNpcT_getDistTableIdx(field_0xff0, field_0xff4), 
                    attention_info.distances[1]);
        if (field_0xfec < 4) {
            g_meter2_info.mBlinkButton |= 1;
        }
    } else if (chkChuMotion() != 0) {
        attention_info.distances[fopAc_attn_LOCK_e] = 20;
        attention_info.distances[fopAc_attn_TALK_e] = 20;
        attention_info.distances[fopAc_attn_SPEAK_e] = 20;
        uVar7 = fopAc_AttnFlag_TALK_e;
    }

    attention_info.flags = uVar7;
    scale.set(mpHIO->m.common.scale, mpHIO->m.common.scale,
            mpHIO->m.common.scale);
    mCcStts.SetWeight(mpHIO->m.common.weight);
    mCylH = mpHIO->m.common.height;
    mWallR = mpHIO->m.common.width;
    mAttnFovY = mpHIO->m.common.fov;
    mAcchCir.SetWallR(mWallR);
    mAcchCir.SetWallH(mpHIO->m.common.knee_length);
    mRealShadowSize = mpHIO->m.common.real_shadow_size;
    mExpressionMorfFrame = mpHIO->m.common.expression_morf_frame;
    mMorfFrames = mpHIO->m.common.morf_frame;
    gravity = mpHIO->m.common.gravity;
}

int daNpc_Jagar_c::checkChangeEvt() {
    if (!chkAction(&daNpc_Jagar_c::talk)) {
        mPreItemNo = 0;
        if (dComIfGp_event_chkTalkXY()) {
            if (dComIfGp_evmng_ChkPresentEnd()) {
                mEvtNo = EVENT_NO_RESPONSE;
                evtChange();
            }
            return true;
        }

        switch (mType) {
            case TYPE_0:
                if (daNpcT_chkEvtBit(0x1C) /* dSv_event_flag_c::F_0023 - Ordon Village - Called by Jaggle from below hill */
                    && chkChuMotion()) {
                    mEvtNo = EVENT_CLIMBUP;
                    evtChange();
                    return true;
                }
                break;
                
            case TYPE_1:
                if (field_0xfec < 4) {
                    mEvtNo = EVENT_CONFIDENTIAL_CONVERSATION;
                    evtChange();
                    return true;
                }
                break;

            case TYPE_2:
                break;
        }
    }
    return false;
}

void daNpc_Jagar_c::setAfterTalkMotion() {
    int iVar2 = 8;
    switch(mFaceMotionSeqMngr.getNo()) {
        case FACE_TALK_A:
            iVar2 = 6;
            break;

        case FACE_SURPRISE:
            break;

        case FACE_SMILETALK:
            iVar2 = 7;
            break;

        case FACE_TALK_B:
            iVar2 = 5;
            break;
    }

    mFaceMotionSeqMngr.setNo(iVar2, -1.0f, 0, 0);
}

void daNpc_Jagar_c::srchActors() {
    switch(mType) {
        case TYPE_0:
            if (!mActorMngr[1].getActorP()) {
                mActorMngr[1].entry(getNearestActorP(0x15a));
            }

            if (!mActorMngr[0].getActorP()) {
                mActorMngr[0].entry(getNearestActorP(0x10d));
            }
            break;

        case TYPE_1:
            if (!mActorMngr[1].getActorP()) {
                mActorMngr[1].entry(getNearestActorP(0x15a));
            }

            if (!mActorMngr[2].getActorP()) {
                mActorMngr[2].entry(getNearestActorP(0x246));
            }
            break;

        case TYPE_2:
            break;
    }
}

BOOL daNpc_Jagar_c::evtTalk() {
    if (chkAction(&daNpc_Jagar_c::talk)) {
        (this->*field_0xfe0)(NULL);
    } else {
        setAction(&daNpc_Jagar_c::talk);
    }
    return 1;
}

BOOL daNpc_Jagar_c::evtCutProc() {
    int staffId = dComIfGp_getEventManager().getMyStaffId("Jagar", this, -1);
    if (staffId != -1) {
        mStaffId = staffId;
        int actIdx = dComIfGp_getEventManager().getMyActIdx(mStaffId, mCutNameList, 7, 0, 0);
        if ((this->*(mCutList[actIdx]))(mStaffId) != 0) {
            dComIfGp_getEventManager().cutEnd(mStaffId);
        }
        return true;
    } 
    return false;
}

void daNpc_Jagar_c::action() {
    fopAc_ac_c* hitActor = hitChk(&mCyl1, 0xffffffff);
    if (hitActor && mCyl1.GetTgHitObj()->ChkAtType(AT_TYPE_THROW_OBJ)) {
        if (mType == TYPE_1) {
            daNpc_Bou_c *bo = (daNpc_Bou_c *)mActorMngr[2].getActorP();
            if (bo && bo->getType() == TYPE_1) {
                if (bo->mStagger.checkStagger() ? 0 : 1) {
                    bo->mFaceMotionSeqMngr.setNo(1, -1, 0, 0);
                    bo->mMotionSeqMngr.setNo(MOT_SIT, -1, 0, 0);
                    field_0xff8 = 0;
                    field_0x1000 = 1;
                }
            }
        }

        if (field_0x1003 != 2 && field_0x1004 != 2) {
            mStagger.setParam(this, hitActor, mCurAngle.y);
            setDamage(0, 8, 0);
            mDamageTimerStart = 0;
            mJntAnm.setMode(0, 0);
            mJntAnm.setDirect(1);
        }
    }

    if (mStagger.checkRebirth()) {
        mStagger.initialize();
        field_0x1003 = 0;
        mMode = 1;
    }

    if (field_0xfd4) {
        if (field_0xfe0 == field_0xfd4) {
            (this->*field_0xfe0)(NULL);
        } else {
            setAction(field_0xfd4);
        }
    }
    daTag_Push_c *uVar4 = (daTag_Push_c *)field_0xba0.getActorP();
    if (uVar4) {
        switch ((int)uVar4->getId()) {
            case 2:
                mEvtNo = EVENT_CONVERSATION_WITH_BOU;
                break;

            default:
                break;
        }
    }
}

void daNpc_Jagar_c::beforeMove() {
    fopAcM_OffStatus(this, 0x8000000);
    if (checkHide()) {
        fopAcM_OnStatus(this, 0x8000000);
    }
    
    if (checkHide() || mNoDraw != 0) {
        attention_info.flags = 0;
    }
}

void daNpc_Jagar_c::setAttnPos() {
    cXyz cStack_3c(-10.0f, 10.0f, 0.0f);
    mStagger.calc(0);
    f32 dVar8 = cM_s2rad(mCurAngle.y - field_0xd7e.y);
    J3DModel* model = mpMorf[0]->getModel();
    mJntAnm.setParam(this, model, &cStack_3c, getBackboneJointNo(), getNeckJointNo(),
        getHeadJointNo(), mpHIO->m.common.body_angleX_min, mpHIO->m.common.body_angleX_max,
        mpHIO->m.common.body_angleY_min, mpHIO->m.common.body_angleY_max,
        mpHIO->m.common.head_angleX_min, mpHIO->m.common.head_angleX_max,
        mpHIO->m.common.head_angleY_min, mpHIO->m.common.head_angleY_max,
        mpHIO->m.common.neck_rotation_ratio, dVar8, NULL);
    mJntAnm.calcJntRad(0.2f, 1.0f, dVar8);
    setMtx();
    mDoMtx_stack_c::copy(mpMorf[0]->getModel()->getAnmMtx(getHeadJointNo()));
    mDoMtx_stack_c::multVec(&cStack_3c, &eyePos);
    mJntAnm.setEyeAngleX(eyePos, 1.0f, -0x2800);
    mJntAnm.setEyeAngleY(eyePos, mCurAngle.y, 1, 1.0f, 0);
    cStack_3c.set(0.0f, 0.0f, 10.0f);
    cStack_3c.y = mpHIO->m.common.attention_offset;
    if (field_0x1004 == 2) {
        cStack_3c.set(0.0f, 100.0f, -60.0f);
    }

    mDoMtx_stack_c::YrotS(mCurAngle.y);
    mDoMtx_stack_c::multVec(&cStack_3c, &cStack_3c);
    attention_info.position = current.pos + cStack_3c;
    static cXyz prtclScl(1.0f, 1.0f, 1.0f);
    setFootPos();
    if (3.0f < speedF) {
        setFootPrtcl(&prtclScl, 11.0f, 0);
    }
}

void daNpc_Jagar_c::setCollision() {
    cXyz cStack_48;
    if (mHide == 0) {
        u32 tgType = -0x27040201;
        u32 tgSPrm = 0x1f;
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
        f32 cylHeight = mCylH;
        f32 cylRadius = mWallR;
        if (field_0x1004 == 2) {
            cylHeight = 70.0f;
            cylRadius = 90.0f;
        }

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

int daNpc_Jagar_c::drawDbgInfo() {
    return 0;
}

void daNpc_Jagar_c::changeBtp(int* param_0, int* param_1) {
    if (((mType == TYPE_1 || mType == TYPE_2) && *param_0 == 19) && *param_1 == 1) {
        *param_0 = 17;
        *param_1 = 3;
    }
    return;
}

int daNpc_Jagar_c::selectAction() {
    field_0xfd4 = NULL;
    switch (mType) {
        case TYPE_1:
            field_0xfd4 = &daNpc_Jagar_c::talkwithBou;
            break;

        default:
            field_0xfd4 = &daNpc_Jagar_c::wait;
            break;
    }
    return 1;
}

int daNpc_Jagar_c::chkAction(int (daNpc_Jagar_c::*action)(void*)) {
    return field_0xfe0 == action;
}

int daNpc_Jagar_c::setAction(int (daNpc_Jagar_c::*action)(void*)) {
    mMode = 3;
    if (field_0xfe0 != NULL) {
        (this->*field_0xfe0)(NULL);
    }

    mMode = 0;
    field_0xfe0 = action;
    if (field_0xfe0 != NULL) {
        (this->*field_0xfe0)(NULL);
    }

    return 1;
}

int daNpc_Jagar_c::cutClimbUp(int i_cutIndex) {
    int rv = 0;
    int prm = -1;
    int* piVar1 = dComIfGp_evmng_getMyIntegerP(i_cutIndex, "prm");
    if (piVar1) {
        prm = *piVar1;
    }

    if (dComIfGp_getEventManager().getIsAddvance(i_cutIndex)) {
        switch (prm) {
            case 0:
                mFaceMotionSeqMngr.setNo(FACE_NONE, 0.0f, 0, 0);
                mMotionSeqMngr.setNo(MOT_CHU, 0.0f, 0, 0);
                mJntAnm.lookNone(1);
                field_0x1003 = 1;
                setAngle(home.angle.y);
                initTalk(mFlowNodeNo, NULL);
                break;

            case 2:
                break;
        }
    }

    switch (prm) {
        case 0:
            if (talkProc(NULL, 0, NULL, 0) && mFlow.checkEndFlow()) {
                rv = 1;
            }
            break;

        case 2:
            rv = 1;
            break;
    }
    return rv;
}

int daNpc_Jagar_c::cutNeedYourHelp(int i_cutIndex) {
    int rv = 0;
    int prm = -1;
    int msgNo = 0;
    int msgNo2 = 0;
    int send = 0;
    fopAc_ac_c* actor_p;
    int* piVar5 = dComIfGp_evmng_getMyIntegerP(i_cutIndex, "prm");
    if (piVar5) {
        prm = *piVar5;
    }

    piVar5 = dComIfGp_evmng_getMyIntegerP(i_cutIndex, "msgNo");
    if (piVar5) {
        msgNo = *piVar5;
    }

    piVar5 = dComIfGp_evmng_getMyIntegerP(i_cutIndex, "msgNo2");
    if (piVar5) {
        msgNo2 = *piVar5;
    }

    piVar5 = dComIfGp_evmng_getMyIntegerP(i_cutIndex, "send");
    if (piVar5) {
        send = *piVar5;
    }

    if (dComIfGp_getEventManager().getIsAddvance(i_cutIndex)) {
        switch(prm) {
            case 0:
                dComIfGp_getEvent().setPt2(mActorMngr[0].getActorP());
                break;

            case 1:
                initTalk(mFlowNodeNo, NULL);
                break;
                
            case 7: {
                fopAc_ac_c* iVar6 = mActorMngr[1].getActorP();
                if (iVar6) {
                    dComIfGp_getEvent().setPt2(iVar6);
                }
                break;
            }

            case 8:
                field_0x1002 = 0;
                field_0x1003 = 0;
        }
    }

    int local_2c[3] = {-1, -1, -1};
    switch (prm) {
        case 0:
            mJntAnm.lookNone(0);
            if (mMotionSeqMngr.getNo() == MOT_SIT_TO_WAIT) {
                if (mMotionSeqMngr.getStepNo() <= 0) {
                    break;
                }
                mFaceMotionSeqMngr.setNo(FACE_NONE, -1.0f, 0, 0);
                mMotionSeqMngr.setNo(MOT_WAIT_A, -1.0f, 0, 0);
                rv = 1;
                break;
            }
            if (mMotionSeqMngr.getNo() == MOT_WAIT_A) {
                rv = 1;
                break;
            }
            break;

        case 1:
            mJntAnm.lookPlayer(0);
            if (mPlayerAngle == mCurAngle.y) {
                rv = 1;
                break;
            }

            step(mPlayerAngle, 8, 0xe, 0xf, 0);
            break;

        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
            switch (prm) {
                case 3:
                case 4:
                case 5:
                    actor_p = mActorMngr[0].getActorP();
                    if (actor_p) {
                        field_0xd6c.setall(0.0f);
                        field_0xd6c.z = current.pos.absXZ(actor_p->current.pos);
                        mDoMtx_stack_c::YrotS(mCurAngle.y  + 0x4000);
                        mDoMtx_stack_c::multVec(&field_0xd6c, &field_0xd6c);
                        field_0xd6c += current.pos;
                        mJntAnm.lookPos(&field_0xd6c, 0);
                    }
                    break;
                    
                case 7: {
                    fopAc_ac_c* actor_p = mActorMngr[1].getActorP();
                    if (actor_p) {
                        field_0xd6c.setall(0.0f);
                        field_0xd6c.z = current.pos.absXZ(actor_p->current.pos);
                        mDoMtx_stack_c::YrotS(mCurAngle.y  + -0x4000);
                        mDoMtx_stack_c::multVec(&field_0xd6c, &field_0xd6c);
                        field_0xd6c += current.pos;
                        mJntAnm.lookPos(&field_0xd6c, 0);
                    }
                    break;
                }

                default:
                    mJntAnm.lookPlayer(0);
            }

            local_2c[0] = msgNo;
            local_2c[1] = msgNo2;
            if (talkProc(local_2c, send, NULL, 0)) {
                if (!msgNo && !msgNo2) {
                    if (mFlow.checkEndFlow() == 1) {
                        rv = 1;
                    }
                } else {
                    rv = 1;
                }
            }

            if (prm == 4) {
                rv = 1;
            }
            break;

        case 8:
            rv = 1;
    }

    return rv;
}

int daNpc_Jagar_c::cutAnger(int i_cutIndex) {
    int rv = 0;
    int prm = -1;
    s16 sVar3;
    int* piVar1 = dComIfGp_evmng_getMyIntegerP(i_cutIndex, "prm");
    if (piVar1 != NULL) {
        prm = *piVar1;
    }

    if (dComIfGp_getEventManager().getIsAddvance(i_cutIndex)) {
        switch (prm) {
            case 0:
                mFaceMotionSeqMngr.setNo(FACE_NONE, 0.0f, 0, 0);
                mMotionSeqMngr.setNo(MOT_WAIT_A, 0.0, 0, 0);
                field_0x1003 = 0;
                sVar3 = fopAcM_searchActorAngleY(this, dComIfGp_getPlayer(0));
                setAngle(sVar3);
                initTalk(mFlowNodeNo, NULL);
                break;

            case 1:
                break;
        }
    }

    switch (prm) {
        case 0:
            mJntAnm.lookPlayer(0);
            if (talkProc(NULL, 0, NULL, 0) && mFlow.checkEndFlow() == 1) {
                rv = 1;
            }
            break;

        case 1:
            mJntAnm.lookNone(0);
            if (home.angle.y == mCurAngle.y) {
                rv = 1;
            } else {
                step(home.angle.y, 8, 14, 15, 0);
            }
            break;

        default:
            break;
    }

    return rv;
}

int daNpc_Jagar_c::cutConversationWithBou(int i_cutIndex) {
    daTag_Push_c* this_00 = (daTag_Push_c*)field_0xba0.getActorP();
    int rv = 0;
    int prm = -1;
    int* piVar2 = (int*)dComIfGp_evmng_getMyIntegerP(i_cutIndex, "prm");
    if (piVar2) {
        prm = *piVar2;
    }

    fopAc_ac_c* speakers[2] = {(fopAc_ac_c *) this, mActorMngr[2].getActorP()};
    dComIfGp_setMesgCameraInfoActor(speakers[0], speakers[1], 0, 0, 0, 0, 0, 0, 0, 0);
    if (dComIfGp_getEventManager().getIsAddvance(i_cutIndex)) {
        switch (prm) {
            case 0:
                initTalk(this_00->getFlowNodeNo(), &speakers[0]);
                break;
        }
    }

    switch (prm) {
        case 0:
            if (talkProc(NULL, 0, &speakers[0], 0) && mFlow.checkEndFlow() == 1) {
                rv = 1;
            }
            break;
    }

    return rv;
}

int daNpc_Jagar_c::cutConfidentialConversation(int i_cutIndex) {
    int rv = 0;
    int prm = -1;
    int msgNo = 0;
    int* piVar2 = dComIfGp_evmng_getMyIntegerP(i_cutIndex, "prm");
    if(piVar2) {
        prm = *piVar2;
    }

    piVar2 = dComIfGp_evmng_getMyIntegerP(i_cutIndex, "msgNo");
    if(piVar2) {
        msgNo = *piVar2;
    }

    fopAc_ac_c* actors[2] = {(fopAc_ac_c *) this, mActorMngr[2].getActorP()};
    dComIfGp_setMesgCameraInfoActor(actors[0], actors[1], 0, 0, 0, 0, 0, 0, 0, 0);
    if (dComIfGp_getEventManager().getIsAddvance(i_cutIndex)) {
        switch (prm) {
            case 0:
                mFaceMotionSeqMngr.setNo(FACE_NONE, -1.0f, 0, 0);
                mMotionSeqMngr.setNo(MOT_WORRY_WAIT, -1.0f, 0, 0);
                initTalk(0xD7, &actors[0]);
                break;

            case 1:
                dComIfGp_getEvent().setPt2(mActorMngr[1].getActorP());
                break;

            case 2:
                break;
        }
    }

    int local_30[3] = {-1, -1, -1};
    switch (prm) {
        case 0:
        case 1:
        case 2:
        case 3: {
            if (prm == 0 || prm == 2) {
                mJntAnm.lookActor(mActorMngr[2].getActorP(), -40.0f, 0);
            } else if (prm == 1) {
                fopAc_ac_c* pfVar3 = mActorMngr[1].getActorP();
                mJntAnm.lookActor(pfVar3, 0.0f, 0);
            }

            local_30[0] = msgNo;
            if (talkProc(&local_30[0], 0, &actors[0], 0)) {
                if (msgNo == 0) {
                    if (mFlow.checkEndFlow()) {
                        rv = 1;
                    }
                } else {
                    rv = 1;
                }
            }
            break;
        }

        case 4:
            mEvtNo = EVENT_FIND_WOLF;
            evtChange();
            break;
    }

    return rv;
}

int daNpc_Jagar_c::cutFindWolf(int i_cutIndex) {
    int rv = 0;
    int prm = -1;
    int timer = 0;

    int* piVar1 = dComIfGp_evmng_getMyIntegerP(i_cutIndex, "prm");
    if (piVar1 != NULL) {
        prm = *piVar1;
    }

    int* piVar2 = dComIfGp_evmng_getMyIntegerP(i_cutIndex, "timer");
    if (piVar2 != NULL) {
        timer = *piVar2;
    }

    if (dComIfGp_getEventManager().getIsAddvance(i_cutIndex)) {
        switch (prm) {
            case 0:
                mFaceMotionSeqMngr.setNo(FACE_SURPRISE, -1.0f, 0, 0);
                mMotionSeqMngr.setNo(MOT_SURPRISE, -1.0f, 0, 0);
                mSound.startCreatureVoice(Z2SE_JAGA_V_SURPRISE, -1);
                mPlayerAngle = fopAcM_searchActorAngleY(this, daPy_getPlayerActorClass());
                if (checkStep()) {
                    mStepMode = 0;
                }

                dComIfGp_getVibration().StartShock(9, 15, cXyz(0.0f, 1.0f, 0.0f));
                break;

            case 2:
                mFaceMotionSeqMngr.setNo(FACE_NONE, -1.0f, 0, 0);
                mMotionSeqMngr.setNo(MOT_RUN_A, -1.0f, 0, 0);
                current.angle.y = home.angle.y;
                if ((s16)(mPlayerAngle - home.angle.y) > 0) {
                    current.angle.y = current.angle.y + -0x4000;
                } else {
                    current.angle.y = current.angle.y + 0x4000;
                    
                }

                mEventTimer = timer;
                break;

            case 3:
                mFaceMotionSeqMngr.setNo(FACE_NONE, 0.0f, 0, 0);
                mMotionSeqMngr.setNo(MOT_WORRY_WAIT, 0.0f, 0, 0);
                setPos(home.pos);
                setAngle(home.angle.y);
                speedF = 0;
                speed.setall(0.0f);
                mAcch.ClrWallNone();
                mHide = 1;
                break;

            case 11:
                mEventTimer = timer;
                break;

            case 4:
            case 5:
            case 6:
            case 7:
            case 8:
            case 9:
            case 1:
                break;
        }
    }

    switch (prm) {
        case 0:
            mJntAnm.lookPlayer(0);
            if (mPlayerAngle == mCurAngle.y) {
                rv = 1;
            } else {
                step(mPlayerAngle, -1, -1, 15, 0);
            }
            break;

        case 1:
            mJntAnm.lookPlayer(0);
            rv = 1;
            break;

        case 2:
            mJntAnm.lookPlayer(0);
            cLib_chaseS(&shape_angle.y, current.angle.y, 0x800);
            mCurAngle.y = shape_angle.y;
            field_0xd7e.y = mCurAngle.y;
            cLib_chaseF(&speedF, mpHIO->m.running_speed, 0.5f);
            mAcch.SetWallNone();
            if (cLib_calcTimer(&mEventTimer) == 0) {
                rv = 1;
            }
            break;

        case 3:
            mJntAnm.lookNone(0);
            rv = 1;
            break;

        case 10:
            if (mMotionSeqMngr.getStepNo() > 0) {
                rv = 1;
            }
            break;

        case 11:
            if (cLib_calcTimer(&mEventTimer) == 0) {
                rv = 1;
            }
            break;
    }

    return rv;
}

int daNpc_Jagar_c::chkSitMotion() {
    if (daNpcT_chkEvtBit(0x235) /* dSv_event_flag_c::F_0565 - N/A - 2nd Day Complete */) {
        if (field_0x1002) {
            return 0;
        }
    } else if (daNpcT_chkEvtBit(0x1C) /* dSv_event_flag_c::F_0023 - Ordon Village - Called by Jaggle from below hill */) {
        if (daNpcT_chkEvtBit(0x87) /* dSv_event_flag_c::F_0037 - Ordon Village - Jaggle - Spoke on the hill? */) {
            return 0;
        } else if (field_0x1002) {
            return 0;
        }
    }

    return 1;
}

int daNpc_Jagar_c::chkSitMotion2() {
    if (daNpcT_chkEvtBit(0x235) /* dSv_event_flag_c::F_0565 - N/A - 2nd Day Complete */) {
        if (daNpcT_chkEvtBit(0x224) /* dSv_event_flag_c::F_0548 - Ordon Village - Opening 3rd day - spoke with Jaggle */) {
            return 1;
        }
    } else if (daNpcT_chkEvtBit(0x8A) /* dSv_event_flag_c::F_0040 - Ordon Village - 2nd day: Spoke to Jaggle after blowing on whistle */) {
        return 1;
    }

    return 0;
}

int daNpc_Jagar_c::chkChuMotion() {
    if (!daNpcT_chkEvtBit(0x235) /* dSv_event_flag_c::F_0565 - N/A - 2nd Day Complete */
        && daNpcT_chkEvtBit(0x1C) /* dSv_event_flag_c::F_0023 - Ordon Village - Called by Jaggle from below hill */) {
        if (daNpcT_chkEvtBit(0x87) /* dSv_event_flag_c::F_0037 - Ordon Village - Jaggle - Spoke on the hill? */) {
            return 0;
        } else {
            return field_0x1001 == 0 ? 1 : 0;
        }
    }

    return 0;
}

int daNpc_Jagar_c::chkToMotion() {
    return mMotionSeqMngr.getNo() == MOT_SIT_TO_WAIT || mMotionSeqMngr.getNo() == MOT_TO_CHU ||
           mMotionSeqMngr.getNo() == MOT_CHU_TO_SIT || mMotionSeqMngr.getNo() == MOT_WAIT_TO_SIT;
}

int daNpc_Jagar_c::wait(void* param_1) {
    cXyz cStack_24;

    field_0x1004 = 0;
    if (mType == TYPE_0) {
        if(chkSitMotion2()) {
            field_0x1004 = 2;
        } else if (chkSitMotion()) {
            if (chkChuMotion()) {
                field_0x1004 = 1;
            } else {
                field_0x1004 = 2;
            }
        }
    }

    if (field_0x1004 != field_0x1003) {
        mMode = 1;
    }

    switch(mMode) {
        case 0:
        case 1:
            if (mCreating) {
                switch (field_0x1004) {
                    case 0:
                        if (mType == TYPE_2) {
                            mFaceMotionSeqMngr.setNo(FACE_NONE, -1.0f, 0, 0);
                            mMotionSeqMngr.setNo(MOT_WAIT_B, -1.0, 0, 0);
                        } else {
                            mFaceMotionSeqMngr.setNo(FACE_NONE, -1.0f, 0, 0);
                            mMotionSeqMngr.setNo(MOT_WAIT_A, -1.0, 0, 0);
                        }

                        field_0x1003 = 0;
                        break;

                    case 1:
                        mFaceMotionSeqMngr.setNo(FACE_NONE, -1.0f, 0, 0);
                        mMotionSeqMngr.setNo(MOT_CHU, -1.0, 0, 0);
                        field_0x1003 = 1;
                        break;

                    case 2:
                        mFaceMotionSeqMngr.setNo(FACE_NONE, -1.0f, 0, 0);
                        mMotionSeqMngr.setNo(MOT_SIT, -1.0, 0, 0);
                        field_0x1003 = 2;
                        break;
                }

                mMode = 2;
            } else if (!mStagger.checkStagger()) {
                switch (field_0x1004) {
                    case 0:
                        switch (field_0x1003) {
                            case 2:
                                mFaceMotionSeqMngr.setNo(FACE_NONE, -1.0f, 0, 0);
                                mMotionSeqMngr.setNo(MOT_SIT_TO_WAIT, -1.0, 0, 0);
                                break;

                            default:
                                if (mType == TYPE_2) {
                                    mFaceMotionSeqMngr.setNo(FACE_NONE, -1.0f, 0, 0);
                                    mMotionSeqMngr.setNo(MOT_WAIT_B, -1.0, 0, 0);
                                } else {
                                    mFaceMotionSeqMngr.setNo(FACE_NONE, -1.0f, 0, 0);
                                    mMotionSeqMngr.setNo(MOT_WAIT_A, -1.0, 0, 0);
                                }

                                field_0x1003 = 0;
                                break;
                        }
                        break;

                    case 1:
                        switch (field_0x1003) {
                            case 2:
                                mFaceMotionSeqMngr.setNo(FACE_NONE, -1.0f, 0, 0);
                                mMotionSeqMngr.setNo(MOT_TO_CHU, -1.0, 0, 0);
                                break;

                            default:
                                mFaceMotionSeqMngr.setNo(FACE_NONE, -1.0f, 0, 0);
                                mMotionSeqMngr.setNo(MOT_CHU, -1.0, 0, 0);
                                field_0x1003 = 1;
                                break;
                        }
                        break;

                    case 2:
                        if (home.angle.y == mCurAngle.y) {
                            switch (field_0x1003) {
                                case 0:
                                    mFaceMotionSeqMngr.setNo(FACE_NONE, -1.0f, 0, 0);
                                    mMotionSeqMngr.setNo(MOT_WAIT_TO_SIT, -1.0, 0, 0);
                                    break;

                                case 1:
                                    mFaceMotionSeqMngr.setNo(FACE_NONE, -1.0f, 0, 0);
                                    mMotionSeqMngr.setNo(MOT_CHU_TO_SIT, -1.0, 0, 0);
                                    break;
                            }
                        }
                        break;
                }
                mMode = 2;
            }
            // fallthrough
        case 2:
            switch (mType) {
                case TYPE_0:
                    daNpcT_offTmpBit(0x10); // dSv_event_tmp_flag_c::T_0015 - Ordon Village - Link came up the hill afte being called by Jaggle
                    cStack_24.set(mpHIO->m.common.search_distance, 10.0f, mpHIO->m.common.search_distance);
                    if (chkPointInArea(daPy_getPlayerActorClass()->current.pos, current.pos,
                        cStack_24, 0)) {
                        if (daPy_getPlayerActorClass()->checkPlayerFly()) {
                            if (daPy_getPlayerActorClass()->checkClimbEndHang()) {
                                field_0x1002 = 1;
                            }
                        }

                        daNpcT_onTmpBit(0x10); // dSv_event_tmp_flag_c::T_0015 - Ordon Village - Link came up the hill afte being called by Jaggle
                    }

                    if (!daNpcT_chkEvtBit(0x235) /* dSv_event_flag_c::F_0565 - N/A - 2nd Day Complete */) {
                        if (!daNpcT_chkEvtBit(0xAE) /* dSv_event_flag_c::F_0075 - Ordon Village - Angered Jaggle by destroying pumpkin */
                            && daNpcT_chkTmpBit(0x1B) /* dSv_event_tmp_flag_c::T_0026 - Ordon Village - Destroy pumpkin near Jaggle (Jaggle gets angry) */) {
                            mEvtNo = EVENT_ANGER;
                        } else if (chkSitMotion() || field_0x1002 != 0) {
                            if (!daNpcT_chkEvtBit(0x235) /* dSv_event_flag_c::F_0565 - N/A - 2nd Day Complete */
                                && !daNpcT_chkEvtBit(0x87) /* dSv_event_flag_c::F_0037 - Ordon Village - Jaggle - Spoke on the hill? */
                                && daNpcT_chkTmpBit(0x10) /* dSv_event_tmp_flag_c::T_0015 - Ordon Village - Link came up the hill afte being called by Jaggle */) {
                                if (daPy_getPlayerActorClass()->eventInfo.chkCondition(dEvtCnd_CANTALK_e) != 0 ? 1 : 0) {
                                    mEvtNo = EVENT_NEED_YOUR_HELP;
                                }
                            }
                        }
                    }
                    break;

                case TYPE_2:
                    if (!mHide) {
                        if (daNpcT_c::chkFindWolf(mCurAngle.y, daNpcT_getDistTableIdx(field_0xff0, field_0xff4),
                                                  field_0xfec, mpHIO->m.common.search_distance, 180.0f,
                                                  mpHIO->m.common.search_height,
                                                  mpHIO->m.common.search_depth, 1)) {
                            mEvtNo = EVENT_FIND_WOLF_VER2;
                        }
                    }
            }

            if (!mStagger.checkStagger()) {
                if (chkToMotion()) {
                    if (mMotionSeqMngr.getStepNo() > 0) {
                        switch (field_0x1004) {
                            case 0:
                                mFaceMotionSeqMngr.setNo(FACE_NONE, -1.0f, 0, 0);
                                mMotionSeqMngr.setNo(MOT_WAIT_A, -1.0f, 0, 0);
                                field_0x1003 = 0;
                                break;

                            case 1:
                                mFaceMotionSeqMngr.setNo(FACE_NONE, -1.0f, 0, 0);
                                mMotionSeqMngr.setNo(MOT_CHU, -1.0f, 0, 0);
                                field_0x1003 = 1;
                                break;

                            case 2:
                                mFaceMotionSeqMngr.setNo(FACE_NONE, -1.0f, 0, 0);
                                mMotionSeqMngr.setNo(MOT_SIT, -1.0f, 0, 0);
                                field_0x1003 = 2;
                                break;
                        }
                    } else if (mMotionSeqMngr.getNo() != MOT_TO_CHU) {
                        attention_info.flags = 0;
                    }

                    mJntAnm.lookNone(0);
                    break;
                }

                if (field_0x1004 == 2) {
                    mPlayerActorMngr.remove();
                } else if (field_0x1004 == 1) {
                    mPlayerActorMngr.entry((fopAc_ac_c*)daPy_getPlayerActorClass());
                }

                if (mPlayerActorMngr.getActorP()) {
                    mJntAnm.lookPlayer(0);

                    if (!chkActorInSight(mPlayerActorMngr.getActorP(), mAttnFovY, mCurAngle.y)) {
                        mJntAnm.lookNone(0);
                    }

                    if (!srchPlayerActor()) {
                        if (home.angle.y == mCurAngle.y) {
                            mMode = 1;
                        }

                        if (field_0x1003 == 1) {
                            if (!daNpcT_chkEvtBit(0x86) /* dSv_event_flag_c::F_0036 - Ordon Village - Spoke to Jaggle using L-focus before climbing vines? */) {
                                daNpcT_onEvtBit(0x86);
                            }
                            field_0x1001 = 1;
                        }
                    }
                } else {
                    mJntAnm.lookNone(0);

                    if (home.angle.y != mCurAngle.y) {
                        if (field_0xe34) {
                            if (step(home.angle.y, 8, 14, 15, 0)) {
                                mMode = 1;
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
            }
            break;
            
        case 3:
            break;
    }
    
    return 1;
}

int daNpc_Jagar_c::talkwithBou(void* param_0) {
    daNpc_Bou_c* bo = (daNpc_Bou_c *)mActorMngr[2].getActorP();

    switch (mMode) {
        case 0:
        case 1:
            if (!mStagger.checkStagger()) {
                mFaceMotionSeqMngr.setNo(FACE_NONE, -1.0f, 0, 0);
                mMotionSeqMngr.setNo(MOT_WORRY_WAIT, -1.0f, 0, 0);
                mMode = 2;
            }
            // fallthrough
        case 2:
            if (mHide != 0 && !daNpcT_chkEvtBit(0xD3)) {
                /* dSv_event_flag_c::F_0211 - Ordon Village - Successfully eavesdrop on Bo and Jaggle */
                f32 actor_dist = fopAcM_searchActorDistanceXZ(this, daPy_getPlayerActorClass());
                if (mpHIO->m.hidden_state_release_dist < actor_dist && field_0xe34 == 0) {
                    if (bo) {
                        bo->mHide = 0;
                    }

                    mHide = 0;
                }
            }

            if (mHide != 0) {
                fopAcM_setCullSizeBox((fopAc_ac_c *)this, -300.0f, -50.0f, -200.0f, 300.0f, 300.0f, 400.0f);
            } else {
                if (chkFindWolf() || (bo && bo->chkFindWolf())) {
                    mEvtNo = EVENT_FIND_WOLF;
                }

                if (bo) {
                    if (mListen == 0) {
                        if (cLib_calcTimer(&field_0xff8) == 0) {
                            if (chkCondition(0) && bo->chkCondition(0)) {
                                field_0xff8 = cLib_getRndValue(45.0f, 135.0f);
                                field_0x1000 ^= 1;
                                if ((field_0x1000 & 0x1) == 0) {
                                    bo->mFaceMotionSeqMngr.setNo(10, -1.0f, 0, 0);

                                    f32 zero_f_val = 0.0f;
                                    if (0.0f < zero_f_val + (cM_rnd() - 0.5f)) {
                                        bo->mMotionSeqMngr.setNo(4, -1.0f, 0, 0);
                                    } else {
                                        bo->mMotionSeqMngr.setNo(1, -1.0f, 0, 0);
                                    }

                                    mFaceMotionSeqMngr.setNo(FACE_NONE, -1.0f, 0, 0);
                                    mMotionSeqMngr.setNo(MOT_WORRY_TALK, -1.0f, 0, 0);
                                } else {
                                    bo->mFaceMotionSeqMngr.setNo(10, -1.0f, 0, 0);
                                    bo->mMotionSeqMngr.setNo(2, -1.0f, 0, 0);
                                    mFaceMotionSeqMngr.setNo(FACE_NONE, -1.0f, 0, 0);

                                    f32 val = 0.0f;
                                    if (0.0f < val + (cM_rnd() - 0.5f)) {
                                        mMotionSeqMngr.setNo(MOT_UNADUKI, -1.0f, 0, 0);
                                    } else {
                                        mMotionSeqMngr.setNo(MOT_WORRY_WAIT, -1.0f, 0, 0);
                                    }
                                }
                            }
                        }
                    } else {
                        field_0xff8 = 0;
                    }
                }

                fopAcM_setCullSizeBox((fopAc_ac_c *)this,-300.0f,-50.0f,-300.0f,300.0f,450.0f,300.0f);
            }

            if (bo) {
                mJntAnm.lookActor((fopAc_ac_c *)bo, -40.0f, 0);
            } else {
                mJntAnm.lookNone(0);
            }
            break;

        case 3:
            break;

        default:
            break;
    }

    return 1;
}

int daNpc_Jagar_c::talk(void* param_0) {
    switch(mMode) {
        case 0:
        case 1:
            if (!mStagger.checkStagger()) {
                if (mType == TYPE_1) {
                    daNpc_Bou_c *bo = (daNpc_Bou_c *)mActorMngr[2].getActorP();
                    if (bo && bo->getType() == TYPE_1 && ((bo->mStagger.checkStagger()) ? 0 : 1)) {
                        bo->mFaceMotionSeqMngr.setNo(10, -1.0f, 0, 0);
                        
                        if (0.0f < -10.0f + (cM_rnd() - 0.5f)) {
                            bo->mMotionSeqMngr.setNo(4, -1.0f, 0, 0);
                        } else {
                            bo->mMotionSeqMngr.setNo(1, -1.0f, 0, 0);
                        }
                    }

                    switch (field_0xfec) {
                        case 4:
                            initTalk(0xd6, NULL); 
                            break;

                        default:
                            initTalk(0xd5, NULL);
                            break;
                    }
                } else {
                    initTalk(mFlowNodeNo, NULL);
                }
                mMode = 2;
            }
            // fallthrough
        case 2:
            if (!mStagger.checkStagger()) {
                if (mTwilight != 0 || mPlayerAngle == mCurAngle.y || mType == TYPE_1 || field_0x1003 != 0) {
                    if (talkProc(NULL, 0, NULL, 0) && mFlow.checkEndFlow()) {
                        mPlayerActorMngr.entry(daPy_getPlayerActorClass());
                        dComIfGp_event_reset();
                        mMode = 3;
                    }

                    if (mType == TYPE_1) {
                        mJntAnm.lookActor(mActorMngr[2].getActorP(), -40.0f, 0);
                    } else {
                        mJntAnm.lookPlayer(0);

                        if (mTwilight || field_0x1003 == 2) {
                            mJntAnm.lookNone(0);
                        }
                    }
                } else {
                    mJntAnm.lookPlayer(0);
                    step(mPlayerAngle, 8, 14, 15, 0);
                }
            }
            break;

        case 3:
        default:
            break;
    }
    return 0;
}

static int daNpc_Jagar_Create(void* i_this) {
    return static_cast<daNpc_Jagar_c*>(i_this)->create();
}

static int daNpc_Jagar_Delete(void* i_this) {
    return static_cast<daNpc_Jagar_c*>(i_this)->Delete();
}

static int daNpc_Jagar_Execute(void* i_this) {
    return static_cast<daNpc_Jagar_c*>(i_this)->Execute();
}

static int daNpc_Jagar_Draw(void* i_this) {
    return static_cast<daNpc_Jagar_c*>(i_this)->Draw();
}

static int daNpc_Jagar_IsDelete(void* i_this) {
    return 1;
}

static actor_method_class daNpc_Jagar_MethodTable = {
    (process_method_func)daNpc_Jagar_Create,
    (process_method_func)daNpc_Jagar_Delete,
    (process_method_func)daNpc_Jagar_Execute,
    (process_method_func)daNpc_Jagar_IsDelete,
    (process_method_func)daNpc_Jagar_Draw,
};

extern actor_process_profile_definition g_profile_NPC_JAGAR = {
  fpcLy_CURRENT_e,          // mLayerID
  7,                        // mListID
  fpcPi_CURRENT_e,          // mListPrio
  PROC_NPC_JAGAR,           // mProcName
  &g_fpcLf_Method.base,    // sub_method
  sizeof(daNpc_Jagar_c),    // mSize
  0,                        // mSizeOther
  0,                        // mParameters
  &g_fopAc_Method.base,     // sub_method
  345,                      // mPriority
  &daNpc_Jagar_MethodTable, // sub_method
  0x00040107,               // mStatus
  fopAc_NPC_e,              // mActorType
  fopAc_CULLBOX_CUSTOM_e,   // cullType
};

AUDIO_INSTANCES;
