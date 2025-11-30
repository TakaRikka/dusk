/**
 * @file d_a_npc_post.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_npc_post.h"
#include "JSystem/JHostIO/JORFile.h"
#include "d/d_meter2_info.h"
#include "d/actor/d_a_horse.h"
#include "d/actor/d_a_tag_evtarea.h"
#include "d/d_debug_viewer.h"
#include "Z2AudioLib/Z2Instances.h"

enum post_RES_File_ID {
    /* BCK */
    /* 0x06 */ BCK_POST_F_HAPPY = 0x6,
    /* 0x07 */ BCK_POST_F_TALK_A,
    /* 0x08 */ BCK_POST_FH_HAPPY,
    /* 0x09 */ BCK_POST_FLAG_STEP,
    /* 0x0A */ BCK_POST_FLAG_WAIT_A,
    /* 0x0B */ BCK_POST_STEP,
    /* 0x0C */ BCK_POST_WAIT_A,

    /* BMDR */
    /* 0x0F */ BMDR_POST = 0xF,
    /* 0x10 */ BMDR_POST_FLAG,
    /* 0x11 */ BMDR_POST_LETTER,

    /* BTK */
    /* 0x14 */ BTK_POST = 0x14,

    /* BTP */
    /* 0x17 */ BTP_POST = 0x17,
    /* 0x18 */ BTP_POST_F_HAPPY,
    /* 0x19 */ BTP_POST_F_TALK_A,
    /* 0x1A */ BTP_POST_FH_HAPPY,
};

enum post1_RES_File_ID {
    /* BCK */
    /* 0x06 */ BCK_POST_BYE = 0x6,
    /* 0x07 */ BCK_POST_BYE_WAIT,
    /* 0x08 */ BCK_POST_EXPLAIN,
    /* 0x09 */ BCK_POST_F_BYE,
    /* 0x0A */ BCK_POST_F_EXPLAIN,
    /* 0x0B */ BCK_POST_F_HAND,
    /* 0x0C */ BCK_POST_F_HELLO,
    /* 0x0D */ BCK_POST_F_REGRET,
    /* 0x0E */ BCK_POST_FH_REGRET,
    /* 0x0F */ BCK_POST_FLAG_EXPLAIN,
    /* 0x10 */ BCK_POST_FLAG_HAND,
    /* 0x11 */ BCK_POST_FLAG_HAND_WAIT,
    /* 0x12 */ BCK_POST_FLAG_HELLO,
    /* 0x13 */ BCK_POST_FLAG_REGRET,
    /* 0x14 */ BCK_POST_FLAG_RUN,
    /* 0x15 */ BCK_POST_HAND,
    /* 0x16 */ BCK_POST_HAND_WAIT,
    /* 0x17 */ BCK_POST_HELLO,
    /* 0x18 */ BCK_POST_REGRET,
    /* 0x19 */ BCK_POST_RUN,
    
    /* BTK */
    /* 0x1C */ BTK_POST_HAND = 0x1C,
    /* 0x1D */ BTK_POST_RUN,

    /* BTP */
    /* 0x20 */ BTP_POST_F_BYE = 0x20,
    /* 0x21 */ BTP_POST_F_EXPLAIN,
    /* 0x22 */ BTP_POST_F_HAND,
    /* 0x23 */ BTP_POST_F_HELLO,
    /* 0x24 */ BTP_POST_F_REGRET,
    /* 0x25 */ BTP_POST_FH_REGRET,
    /* 0x26 */ BTP_POST_RUN,

    /* EVT */
    /* 0x29 */ EVT_POST1_EVENT_LIST = 0x29,
};

enum post2_RES_File_ID {
    /* BCK */
    /* 0x06 */ BCK_POST_F_ANGRY = 0x6,
    /* 0x07 */ BCK_POST_F_SAD,
    /* 0x08 */ BCK_POST_FH_ANGRY,
    /* 0x09 */ BCK_POST_FH_SAD,
    /* 0x0A */ BCK_POST_SIT_A,
    /* 0x0B */ BCK_POST_SIT_NOD,
    /* 0x0C */ BCK_POST_SIT_TALK_A,

    /* BMDR */
    /* 0x0F */ BMDR_POST_LETTER_B = 0xF,

    /* BTK */
    /* 0x12 */ BTK_POST_SIT_A = 0x12,
    /* 0x13 */ BTK_POST_SIT_NOD,
    /* 0x14 */ BTK_POST_SIT_TALK_A,

    /* BTP */
    /* 0x17 */ BTP_POST_F_ANGRY = 0x17,
    /* 0x18 */ BTP_POST_F_SAD,
    /* 0x19 */ BTP_POST_FH_ANGRY,
    /* 0x1A */ BTP_POST_FH_SAD,
    /* 0x1B */ BTP_POST_SIT_NOD,
};

enum RES_Name {
    /* 0x0 */ NONE,
    /* 0x1 */ POST,
    /* 0x2 */ POST1,
    /* 0x3 */ POST2,
};

enum Face_Motion {
    /* 0x0 */ FACE_MOT_TALK_A,
    /* 0x1 */ FACE_MOT_HAND,
    /* 0x2 */ FACE_MOT_EXPLAIN,
    /* 0x3 */ FACE_MOT_HELLO,
    /* 0x4 */ FACE_MOT_REGRET,
    /* 0x5 */ FACE_MOT_BYE,
    /* 0x6 */ FACE_MOT_HAPPY,
    /* 0x7 */ FACE_MOT_SAD,
    /* 0x8 */ FACE_MOT_ANGRY,
    /* 0x9 */ FACE_MOT_H_REGRET,
    /* 0xA */ FACE_MOT_NONE,
    /* 0xB */ FACE_MOT_H_HAPPY,
    /* 0xC */ FACE_MOT_H_SAD,
    /* 0xD */ FACE_MOT_H_ANGRY,
    /* 0xE */ FACE_MOT_NONE_2,
};

enum Motion {
    /* 0x0 */ MOT_WAIT_A,
    /* 0x1 */ MOT_HAND,
    /* 0x2 */ MOT_EXPLAIN,
    /* 0x3 */ MOT_HELLO,
    /* 0x4 */ MOT_REGRET,
    /* 0x5 */ MOT_BYE,
    /* 0x6 */ MOT_SIT_A,
    /* 0x7 */ MOT_SIT_TALK_A,
    /* 0x8 */ MOT_RUN,
    /* 0x9 */ MOT_SIT_NOD,
    /* 0xA */ MOT_STEP,
};

enum Event {
    /* 0x0 */ EVT_NONE,
    /* 0x1 */ EVT_NO_RESPONSE,
    /* 0x2 */ EVT_DELIVER,
    /* 0x3 */ EVT_DELIVERTO_PLAYER_ON_HORSE,
    /* 0x4 */ EVT_DELIVERTO_WOLF,
};

enum Type {
    /* 0x0 */ TYPE_0,
    /* 0x1 */ TYPE_DELIVER,
    /* 0x2 */ TYPE_BAR,
    /* 0x3 */ TYPE_DEFAULT,
};

#if DEBUG
daNpc_Post_HIO_c::daNpc_Post_HIO_c() {
    m = daNpc_Post_Param_c::m;
}

void daNpc_Post_HIO_c::listenPropertyEvent(const JORPropertyEvent* evt) {
    JORReflexible::listenPropertyEvent(evt);

    char buffer[2000];
    JORFile jorFile;
    int len;

    switch ((u32)evt->id) {
        case 0x40000002:
            if (jorFile.open(6, "", NULL, NULL, NULL)) {
                memset(&buffer, 0, sizeof(buffer));
                len = 0;
                daNpcT_cmnListenPropertyEvent(buffer, &len, &m.common);
                
                sprintf(buffer + len, "%.3ff,\t//  走り速度\n", m.run_spd);
                len = strlen(buffer);
                sprintf(buffer + len, "%d,   \t//  頷き間隔\n", m.nod_interval);
                len = strlen(buffer);

                jorFile.writeData(buffer, len);
                jorFile.close();
                OS_REPORT("write append success!::%6d\n", len);
            } else {
                OS_REPORT("write append failure!\n");
            }
            break;
    }
}

void daNpc_Post_HIO_c::genMessage(JORMContext* ctx) {
    daNpcT_cmnGenMessage(ctx, &m.common);
    ctx->genSlider("走り速度        ", &m.run_spd, 0.0f, 100.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    ctx->genSlider("頷き間隔        ", &m.nod_interval, 0, 1000, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    ctx->genButton("ファイル書き出し", 0x40000002, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
}
#endif

static int l_bmdData[4][2] = {
    {BMDR_POST, POST},
    {BMDR_POST_FLAG, POST},
    {BMDR_POST_LETTER, POST},
    {BMDR_POST_LETTER_B, POST2},
};

static daNpcT_evtData_c l_evtList[5] = {
    {"", 0},
    {"NO_RESPONSE", 0},
    {"DELIVER", 2},
    {"DELIVERTO_PLAYER_ON_HORSE", 2},
    {"DELIVERTO_WOLF", 2},
};

static char* l_resNameList[4] = {
    "",
    "post",
    "post1",
    "post2",
};

static s8 l_loadResPtrn0[3] = {
    POST, POST1, -1
};

static s8 l_loadResPtrn1[3] = {
    POST, POST2, -1,
};

static s8 l_loadResPtrn9[4] = {
    POST, POST1, POST2, -1,
};

static s8* l_loadResPtrnList[4] = {
    l_loadResPtrn0,
    l_loadResPtrn0,
    l_loadResPtrn1,
    l_loadResPtrn9,
};

static daNpcT_faceMotionAnmData_c l_faceMotionAnmData[15] = {
    {-1, J3DFrameCtrl::EMode_NONE, NONE, BTP_POST, J3DFrameCtrl::EMode_LOOP, POST, 1},
    {-1, J3DFrameCtrl::EMode_NONE, NONE, BTP_POST_RUN, J3DFrameCtrl::EMode_LOOP, POST1, 0},
    {BCK_POST_F_TALK_A, J3DFrameCtrl::EMode_NONE, POST, BTP_POST_F_TALK_A, J3DFrameCtrl::EMode_NONE, POST, 0},
    {BCK_POST_F_HAND, J3DFrameCtrl::EMode_NONE, POST1, BTP_POST_F_HAND, J3DFrameCtrl::EMode_NONE, POST1, 0},
    {BCK_POST_F_EXPLAIN, J3DFrameCtrl::EMode_NONE, POST1, BTP_POST_F_EXPLAIN, J3DFrameCtrl::EMode_NONE, POST1, 0},
    {BCK_POST_F_HELLO, J3DFrameCtrl::EMode_NONE, POST1, BTP_POST_F_HELLO, J3DFrameCtrl::EMode_NONE, POST1, 0},
    {BCK_POST_F_REGRET, J3DFrameCtrl::EMode_NONE, POST1, BTP_POST_F_REGRET, J3DFrameCtrl::EMode_NONE, POST1, 0},
    {BCK_POST_F_BYE, J3DFrameCtrl::EMode_NONE, POST1, BTP_POST_F_BYE, J3DFrameCtrl::EMode_NONE, POST1, 0},
    {BCK_POST_F_HAPPY, J3DFrameCtrl::EMode_NONE, POST, BTP_POST_F_HAPPY, J3DFrameCtrl::EMode_NONE, POST, 0},
    {BCK_POST_F_SAD, J3DFrameCtrl::EMode_NONE, POST2, BTP_POST_F_SAD, J3DFrameCtrl::EMode_NONE, POST2, 0},
    {BCK_POST_F_ANGRY, J3DFrameCtrl::EMode_NONE, POST2, BTP_POST_F_ANGRY, J3DFrameCtrl::EMode_NONE, POST2, 0},
    {BCK_POST_FH_REGRET, J3DFrameCtrl::EMode_LOOP, POST1, BTP_POST_FH_REGRET, J3DFrameCtrl::EMode_LOOP, POST1, 0},
    {BCK_POST_FH_HAPPY, J3DFrameCtrl::EMode_LOOP, POST, BTP_POST_FH_HAPPY, J3DFrameCtrl::EMode_LOOP, POST, 0},
    {BCK_POST_FH_SAD, J3DFrameCtrl::EMode_LOOP, POST2, BTP_POST_FH_SAD, J3DFrameCtrl::EMode_LOOP, POST2, 0},
    {BCK_POST_FH_ANGRY, J3DFrameCtrl::EMode_LOOP, POST2, BTP_POST_FH_ANGRY, J3DFrameCtrl::EMode_LOOP, POST2, 0},
};

static daNpcT_motionAnmData_c l_motionAnmData[13] = {
    {BCK_POST_WAIT_A, J3DFrameCtrl::EMode_LOOP, POST, BTK_POST, J3DFrameCtrl::EMode_NONE, POST, 1, 0},
    {BCK_POST_RUN, J3DFrameCtrl::EMode_LOOP, POST1, BTK_POST_RUN, J3DFrameCtrl::EMode_LOOP, POST1, 0, 0},
    {BCK_POST_STEP, J3DFrameCtrl::EMode_NONE, POST, BTK_POST, J3DFrameCtrl::EMode_NONE, POST, 1, 0},
    {BCK_POST_HAND, J3DFrameCtrl::EMode_NONE, POST1, BTK_POST_HAND, J3DFrameCtrl::EMode_NONE, POST1, 0, 0},
    {BCK_POST_HAND_WAIT, J3DFrameCtrl::EMode_LOOP, POST1, BTK_POST, J3DFrameCtrl::EMode_NONE, POST, 1, 0},
    {BCK_POST_EXPLAIN, J3DFrameCtrl::EMode_NONE, POST1, BTK_POST, J3DFrameCtrl::EMode_NONE, POST, 1, 0},
    {BCK_POST_HELLO, J3DFrameCtrl::EMode_NONE, POST1, BTK_POST, J3DFrameCtrl::EMode_NONE, POST, 1, 0},
    {BCK_POST_REGRET, J3DFrameCtrl::EMode_NONE, POST1, BTK_POST, J3DFrameCtrl::EMode_NONE, POST, 1, 0},
    {BCK_POST_BYE, J3DFrameCtrl::EMode_NONE, POST1, BTK_POST, J3DFrameCtrl::EMode_NONE, POST, 1, 0},
    {BCK_POST_BYE_WAIT, J3DFrameCtrl::EMode_LOOP, POST1, BTK_POST, J3DFrameCtrl::EMode_NONE, POST, 1, 0},
    {BCK_POST_SIT_A, J3DFrameCtrl::EMode_LOOP, POST2, BTK_POST_SIT_A, J3DFrameCtrl::EMode_LOOP, POST2, 0, 0},
    {BCK_POST_SIT_TALK_A, J3DFrameCtrl::EMode_NONE, POST2, BTK_POST_SIT_TALK_A, J3DFrameCtrl::EMode_NONE, POST2, 0, 0},
    {BCK_POST_SIT_NOD, J3DFrameCtrl::EMode_NONE, POST2, BTK_POST_SIT_NOD, J3DFrameCtrl::EMode_NONE, POST2, 0, 0},
};

static daNpcT_MotionSeqMngr_c::sequenceStepData_c l_faceMotionSequenceData[60] = {
    {2, -1, 1}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {3, -1, 1}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {4, -1, 1}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {5, -1, 1}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {6, -1, 1}, {11, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {7, -1, 1}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {8, -1, 1}, {12, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {9, -1, 1}, {13, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {10, -1, 1}, {14, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {11, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {1, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {12, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {13, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {14, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {0, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
};

static daNpcT_MotionSeqMngr_c::sequenceStepData_c l_motionSequenceData[44] = {
    {0, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {3, -1, 1}, {4, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {5, -1, 1}, {0, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {6, -1, 1}, {0, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {7, -1, 1}, {0, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {8, -1, 1}, {9, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {10, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {11, -1, 1}, {10, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {1, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {12, -1, 1}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {2, 4, 1}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
};

char* daNpc_Post_c::mCutNameList[2] = {
    "",
    "DELIVER",
};

daNpc_Post_c::cutFunc daNpc_Post_c::mCutList[2] = {
    NULL,
    &daNpc_Post_c::cutDeliver,
};

static NPC_POST_HIO_CLASS l_HIO;

daNpc_Post_c::~daNpc_Post_c() {
    OS_REPORT("|%06d:%x|daNpc_Post_c -> コンストラクト\n", g_Counter.mCounter0, this);

    if (mpMorf[0] != NULL) {
        mpMorf[0]->stopZelAnime();
    }

    if (mpFlagModelMorf != NULL) {
        mpFlagModelMorf->stopZelAnime();
    }

    #if DEBUG
    if (mHIO != NULL) {
        mHIO->removeHIO();
    }
    #endif

    deleteRes(l_loadResPtrnList[mType], (const char**)l_resNameList);
}

daNpc_Post_HIOParam const daNpc_Post_Param_c::m = {
    190.0f,
    -3.0f,
    1.0f,
    700.0f,
    255.0f,
    180.0f,
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
    16.0f,
    60,
};

cPhs__Step daNpc_Post_c::create() {
    daNpcT_ct(this, daNpc_Post_c, l_faceMotionAnmData, l_motionAnmData, l_faceMotionSequenceData,
              4, l_motionSequenceData, 4, l_evtList, l_resNameList);

    mType = getType();
    mFlowNodeNo = getFlowNodeNo();
    mTwilight = dKy_darkworld_check();

    cPhs__Step phase = (cPhs__Step)loadRes(l_loadResPtrnList[mType], (const char**)l_resNameList);
    if (phase == cPhs_COMPLEATE_e) {
        OS_REPORT("\t(%s:%d) flowNo:%d, BitSW:%02x<%08x> ", fopAcM_getProcNameString(this), mType, mFlowNodeNo,
                  getBitSW(), fopAcM_GetParam(this));

        if (isDelete()) {
            OS_REPORT("===>isDelete:TRUE\n");
            return cPhs_ERROR_e;
        }

        OS_REPORT("\n");
        
        static int const heapSize[4] = {
            0x0, 0x5060, 0x4EC0, 0x0,
        };

        if (!fopAcM_entrySolidHeap(this, createHeapCallBack, heapSize[mType])) {
            return cPhs_ERROR_e;
        }

        J3DModel* model = mpMorf[0]->getModel();
        fopAcM_SetMtx(this, mpMorf[0]->getModel()->getBaseTRMtx());
        fopAcM_setCullSizeBox(this, -200.0f, -100.0f, -200.0f, 200.0f, 300.0f, 200.0f);

        mSound.init(&current.pos, &eyePos, 3, 1);
        field_0x9c0.init(&mAcch, 0.0f, 0.0f);

        #if DEBUG
        mHIO = &l_HIO;
        mHIO->entryHIO("ポストマン");
        #endif

        reset();

        mAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1, &mAcchCir, fopAcM_GetSpeed_p(this),
                  fopAcM_GetAngle_p(this), fopAcM_GetShapeAngle_p(this));
        mCcStts.Init(mHIO->m.common.weight, 0, this);

        mCyl.Set(mCcDCyl);
        mCyl.SetStts(&mCcStts);
        mCyl.SetTgHitCallback(tgHitCallBack);

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

int daNpc_Post_c::CreateHeap() {
    int bmdIdx = 0;
    int resIdx = l_bmdData[bmdIdx][1];
    int idx = l_bmdData[bmdIdx][0];
    J3DModelData* modelData = static_cast<J3DModelData*>(dComIfG_getObjectRes(l_resNameList[resIdx], idx));
    if (modelData == NULL) {
        return 0;
    }

    u32 uVar1 = 0x11020284;
    mpMorf[0] = new mDoExt_McaMorfSO(modelData, NULL, NULL, NULL, -1, 1.0f, 0, -1, &mSound, 0x80000, uVar1);
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

    bmdIdx = 1;
    resIdx = l_bmdData[bmdIdx][1];
    idx = l_bmdData[bmdIdx][0];
    modelData = (J3DModelData*)dComIfG_getObjectRes(l_resNameList[resIdx], idx);
    if (modelData == NULL) {
        return 0;
    }

    if (mType != TYPE_BAR) {
        u32 uVar2 = 0x11000084;
        mpFlagModelMorf = new mDoExt_McaMorfSO(modelData, NULL, NULL, NULL, -1, 1.0f, 0, -1, NULL, J3DMdlFlag_DifferedDLBuffer, uVar2);
        if (mpFlagModelMorf == NULL || mpFlagModelMorf->getModel() == NULL) {
            return 0;
        }
    }

    static int const bmdTypeList[2] = {
        2, 3,
    };

    for (int i = 0; i < 2; i++) {
        if (l_bmdData[bmdTypeList[i]][0] >= 0) {
            modelData = (J3DModelData*)dComIfG_getObjectRes(l_resNameList[l_bmdData[bmdTypeList[i]][1]], l_bmdData[bmdTypeList[i]][0]);
        } else {
            modelData = NULL;
        }

        if (modelData != NULL) {
            mpLetterModels[i] = mDoExt_J3DModel__create(modelData, J3DMdlFlag_DifferedDLBuffer, 0x11000084);
        } else {
            mpLetterModels[i] = NULL;
        }
    }

    if (setFaceMotionAnm(2, false) && setMotionAnm(0, 0.0f, FALSE)) {
        return 1;
    }

    return 0;
}

int daNpc_Post_c::Delete() {
    OS_REPORT("|%06d:%x|daNpc_Post_c -> コンストラクト\n", g_Counter.mCounter0, this);
    fpc_ProcID id = fopAcM_GetID(this);
    this->~daNpc_Post_c();
    return 1;
}

int daNpc_Post_c::Execute() {
    return execute();
}

int daNpc_Post_c::Draw() {
    if (mpMatAnm[0] != NULL) {
        J3DModelData* modelData = mpMorf[0]->getModel()->getModelData();
        modelData->getMaterialNodePointer(getEyeballMaterialNo())->setMaterialAnm(mpMatAnm[0]);
    }

    return draw(
#if DEBUG
        chkAction(&daNpc_Post_c::test),
#else
        0,
#endif
        0, mRealShadowSize, NULL, 100.0f, 0, 0, 0);
}

int daNpc_Post_c::createHeapCallBack(fopAc_ac_c* i_this) {
    daNpc_Post_c* actor = (daNpc_Post_c*)i_this;
    return actor->CreateHeap();
}

int daNpc_Post_c::ctrlJointCallBack(J3DJoint* i_joint, int param_2) {
    if (param_2 == 0) {
        J3DModel* model = j3dSys.getModel();
        daNpc_Post_c* actor = (daNpc_Post_c*)model->getUserArea();
        if (actor != NULL) {
            actor->ctrlJoint(i_joint, model);
        }
    }

    return 1;
}

u8 daNpc_Post_c::getType() {
    u8 param = fopAcM_GetParam(this) & 0xFF;
    switch (param) {
        case 0:
            return TYPE_0;
        
        case 1:
            return TYPE_DELIVER;

        case 2:
            return TYPE_BAR;

        default:
            return TYPE_DEFAULT;
    }
}

BOOL daNpc_Post_c::isDelete() {
    switch (mType) {
        case TYPE_0:
            return FALSE;

        case TYPE_DELIVER:
            return FALSE;

        case TYPE_BAR:
            return FALSE;

        default:
            return FALSE;
    }
}

void daNpc_Post_c::reset() {
    csXyz angle;
    int size = (u8*)&field_0x1014 - (u8*)&mNextAction;

    if (mpMatAnm[0] != NULL) {
        mpMatAnm[0]->initialize();
    }

    initialize();

    for (int i = 0; i < 4; i++) {
        mActorMngrs[i].initialize();
    }

    memset(&mNextAction, 0, size);

    if (dMeter2Info_getNewLetterNum()) {
        mFlagModelDispFlag = 1;
    }

    angle.setall(0);
    angle.y = home.angle.y;

    switch (mType) {
        case TYPE_0:
            break;
        
        case TYPE_DELIVER:
            mHide = true;
            break;

        case TYPE_BAR:
            mLetterBDispFlag = 1;
            mFlagModelDispFlag = 0;
            mSitFlag = 1;
            break;
    }

    setAngle(angle);
}

void daNpc_Post_c::afterJntAnm(int i_joint) {
    if (i_joint == JNT_BACKBONE1) {
        mDoMtx_stack_c::YrotM(mStagger.getAngleZ(1));
        mDoMtx_stack_c::ZrotM(-mStagger.getAngleX(1));
    } else if (i_joint == JNT_HEAD) {
        mDoMtx_stack_c::YrotM(-mStagger.getAngleZ(0));
        mDoMtx_stack_c::ZrotM(mStagger.getAngleX(0));
    }
}

void daNpc_Post_c::setParam() {
    selectAction();
    srchActors();

    u32 flags = fopAc_AttnFlag_SPEAK_e | fopAc_AttnFlag_TALK_e;
    s16 talk_distance = mHIO->m.common.talk_distance;
    s16 talk_angle = mHIO->m.common.talk_angle;
    s16 attention_distance = mHIO->m.common.attention_distance;
    s16 attention_angle = mHIO->m.common.attention_angle;

    attention_info.distances[fopAc_attn_LOCK_e] = daNpcT_getDistTableIdx(attention_distance, attention_angle);
    attention_info.distances[fopAc_attn_TALK_e] = attention_info.distances[fopAc_attn_LOCK_e];
    attention_info.distances[fopAc_attn_SPEAK_e] = daNpcT_getDistTableIdx(talk_distance, talk_angle);
    attention_info.flags = flags;

    if (daPy_py_c::checkNowWolf()) {
        attention_info.flags |= fopAc_AttnFlag_UNK_0x800000;
    }

    scale.set(mHIO->m.common.scale, mHIO->m.common.scale, mHIO->m.common.scale);
    mCcStts.SetWeight(mHIO->m.common.weight);

    mCylH = mHIO->m.common.height;
    mWallR = mHIO->m.common.width;
    mAttnFovY = mHIO->m.common.fov;
    mAcchCir.SetWallR(mWallR);
    mAcchCir.SetWallH(mHIO->m.common.knee_length);
    mRealShadowSize = mHIO->m.common.real_shadow_size;
    mExpressionMorfFrame = mHIO->m.common.expression_morf_frame;
    mMorfFrames = mHIO->m.common.morf_frame;
    gravity = mHIO->m.common.gravity;

    pullOutLetter();
}

BOOL daNpc_Post_c::checkChangeEvt() {
    if (!chkAction(&daNpc_Post_c::talk)) {
        mPreItemNo = 0;

        if (dComIfGp_event_chkTalkXY()) {
            if (dComIfGp_evmng_ChkPresentEnd()) {
                mEvtNo = EVT_NO_RESPONSE;
                evtChange();
            }

            return TRUE;
        }
    }

    return FALSE;
}

void daNpc_Post_c::setAfterTalkMotion() {
    int idx = FACE_MOT_NONE_2;

    switch (mFaceMotionSeqMngr.getNo()) {
        case FACE_MOT_REGRET:
            idx = FACE_MOT_H_REGRET;
            break;
        
        case FACE_MOT_HAPPY:
            idx = FACE_MOT_H_HAPPY;
            break;

        case FACE_MOT_SAD:
            idx = FACE_MOT_H_SAD;
            break;
            
        case FACE_MOT_ANGRY:
            idx = FACE_MOT_H_ANGRY;
            break;
    }

    mFaceMotionSeqMngr.setNo(idx, -1.0f, FALSE, 0);
}

void daNpc_Post_c::srchActors() {
    switch (mType) {
        case TYPE_0:
            break;
        
        case TYPE_DELIVER:
            for (int i = 0; i < 4; i++) {
                if (mActorMngrs[i].getActorP() == NULL) {
                    mActorMngrs[i].entry(getEvtAreaTagP(21, i));
                }
            }
            break;

        case TYPE_BAR:
            break;
    }
}

BOOL daNpc_Post_c::evtTalk() {
    if (chkAction(&daNpc_Post_c::talk)) {
        (this->*mAction)(NULL);
    } else {
        mPreItemNo = 0;

        if (dComIfGp_event_chkTalkXY()) {
            if (!dComIfGp_evmng_ChkPresentEnd()) {
                return TRUE;
            }

            mEvtNo = EVT_NO_RESPONSE;
            evtChange();

            return TRUE;
        } else {
            setAction(&daNpc_Post_c::talk);
        }
    }

    return TRUE;
}

BOOL daNpc_Post_c::evtCutProc() {
    int staffId = dComIfGp_getEventManager().getMyStaffId("Post", this, -1);
    if (staffId != -1) {
        mStaffId = staffId;
        int actIdx = dComIfGp_getEventManager().getMyActIdx(mStaffId, mCutNameList, 2, FALSE, FALSE);

        if ((this->*mCutList[actIdx])(mStaffId)) {
            dComIfGp_getEventManager().cutEnd(mStaffId);
        }

        return TRUE;
    }

    return FALSE;
}

void daNpc_Post_c::action() {
    fopAc_ac_c* actor = NULL;
    actor = hitChk(&mCyl, 0xFFFFFFFF);
    #if DEBUG
    if (actor != NULL) {
        if (mCyl.GetTgHitObj()->ChkAtType(AT_TYPE_THROW_OBJ)) {
            mStagger.setParam(this, actor, mCurAngle.y);
            setDamage(0, 14, 0);
            mDamageTimerStart = 0;
            mJntAnm.lookNone(1);
        }
    }
    #endif

    if (mStagger.checkRebirth()) {
        mStagger.initialize();
        mMode = MODE_INIT;
    }

    if (mNextAction) {
        if (mAction == mNextAction) {
            (this->*mAction)(NULL);
        } else {
            setAction(mNextAction);
        }
    }
}

void daNpc_Post_c::beforeMove() {
    fopAcM_OffStatus(this, fopAcM_STATUS_UNK_0x8000000);

    if (checkHide()) {
        fopAcM_OnStatus(this, fopAcM_STATUS_UNK_0x8000000);
    }

    if (checkHide() || mNoDraw) {
        attention_info.flags = 0;
    }

    mPrevPosY = current.pos.y;
}

void daNpc_Post_c::afterMoved() {
    if (mGroundH == -G_CM3D_F_INF) {
        cXyz pos(current.pos);
        pos.y += 300.0f;
        mGndChk.SetPos(&pos);

        pos.y = dComIfG_Bgsp().GroundCross(&mGndChk);
        if (pos.y != -G_CM3D_F_INF) {
            current.pos.y = pos.y;
        } else {
            current.pos.y = mPrevPosY;
        }

        speed.y = 0.0f;
        old.pos.y = current.pos.y;
    }
}

void daNpc_Post_c::setAttnPos() {
    cXyz work(-30.0f, 10.0f, 0.0f);
    cXyz sp7c(0.0f, 10.0f, 0.0f);

    mStagger.calc(FALSE);
    f32 rad_val = cM_s2rad(mCurAngle.y - field_0xd7e.y);
    mJntAnm.setParam(this, mpMorf[0]->getModel(), &work, getBackboneJointNo(), getNeckJointNo(), getHeadJointNo(),
                     mHIO->m.common.body_angleX_min, mHIO->m.common.body_angleX_max,
                     mHIO->m.common.body_angleY_min, mHIO->m.common.body_angleY_max,
                     mHIO->m.common.head_angleX_min, mHIO->m.common.head_angleX_max,
                     mHIO->m.common.head_angleY_min, mHIO->m.common.head_angleY_max,
                     mHIO->m.common.neck_rotation_ratio, rad_val, &sp7c);
    mJntAnm.calcJntRad(0.2f, 1.0f, rad_val);
    
    setMtx();

    if (mpFlagModelMorf != NULL) {
        mpFlagModelMorf->play(0, 0);
        mDoMtx_stack_c::copy(mpMorf[0]->getModel()->getAnmMtx(JNT_BACKBONE2));
        Mtx mtx;
        MTXCopy(mDoMtx_stack_c::get(), mtx);
        mpFlagModelMorf->getModel()->setBaseTRMtx(mtx);
        mpFlagModelMorf->modelCalc();
    }

    mDoMtx_stack_c::copy(mpMorf[0]->getModel()->getAnmMtx(getHeadJointNo()));
    mDoMtx_stack_c::multVec(&work, &eyePos);
    mJntAnm.setEyeAngleX(eyePos, 1.0f, 0);
    mJntAnm.setEyeAngleY(eyePos, mCurAngle.y, TRUE, 1.0f, 0);
    field_0xde4 = 0.3f;

    work.set(0.0f, 0.0f, 16.0f);
    work.y = mHIO->m.common.attention_offset;
    if (mSitFlag != 0) {
        work.set(0.0f, 110.0f, 40.0f);
    }

    mDoMtx_stack_c::YrotS(mCurAngle.y);
    mDoMtx_stack_c::multVec(&work, &work);
    attention_info.position = work + current.pos;

    static cXyz prtclScl(1.0f, 1.0f, 1.0f);
    setFootPos();
    if (3.0f < speedF) {
        setFootPrtcl(&prtclScl, 11.0f, 0.0f);
    }
}

void daNpc_Post_c::setCollision() {
    cXyz work;

    if (!mHide) {
        u32 coSPrm = 0x79;
        u32 tgType = 0xD8FBFDFF;
        u32 tgSPrm = 0x1F;

        if (dComIfGp_event_runCheck()) {
            tgType = 0;
            tgSPrm = 0;
        } else if (mTwilight) {
            tgType = 0;
            tgSPrm = 0;
        } else if (mStagger.checkStagger()) {
            tgType = 0;
            tgSPrm = 0;
        }

        mCyl.SetCoSPrm(coSPrm);
        mCyl.SetTgType(tgType);
        mCyl.SetTgSPrm(tgSPrm);
        mCyl.OnTgNoHitMark();

        work.set(0.0f, 0.0f, 10.0f);
        f32 height = mCylH;
        f32 width = mWallR;

        if (mType == TYPE_BAR) {
            work.set(0.0f, 0.0f, 20.0f);
            height = 80.0f;
            width = 50.0f;
        }

        mDoMtx_stack_c::YrotS(mCurAngle.y);
        mDoMtx_stack_c::multVec(&work, &work);
        work += current.pos;
        mCyl.SetH(height);
        mCyl.SetR(width);
        mCyl.SetC(work);
        dComIfG_Ccsp()->Set(&mCyl);
    }

    mCyl.ClrCoHit();
    mCyl.ClrTgHit();
}

int daNpc_Post_c::drawDbgInfo() {
    #if DEBUG
    if (mHIO->m.common.debug_info_ON) {
        f32 radius = dComIfGp_getAttention()->getDistTable(attention_info.distances[fopAc_attn_SPEAK_e]).mDistMax;
        f32 radius2 = dComIfGp_getAttention()->getDistTable(attention_info.distances[fopAc_attn_TALK_e]).mDistMax;
        dDbVw_drawCircleOpa(attention_info.position, radius, (GXColor){0x00, 0xC8, 0x00, 0xFF}, 1, 12);
        dDbVw_drawCircleOpa(attention_info.position, radius2, (GXColor){0xC8, 0x00, 0x00, 0xFF}, 1, 12);
        dDbVw_drawSphereXlu(eyePos, 18.0f, (GXColor){0x80, 0x80, 0x80, 0xA0}, 1);
        dDbVw_drawSphereXlu(attention_info.position, 9.0f, (GXColor){0x80, 0x80, 0x80, 0xA0}, 1);
    }
    #endif
    
    return 0;
}

void daNpc_Post_c::drawOtherMdl() {
    static int const jointNo[2] = {
        JNT_FINGERR, JNT_HANDR,
    };

    J3DModel* model = mpMorf[0]->getModel();

    if (mpFlagModelMorf != NULL) {
        g_env_light.setLightTevColorType_MAJI(mpFlagModelMorf->getModel(), &tevStr);

        if (!mFlagModelDispFlag) {
            mpFlagModelMorf->getModel()->getModelData()->hide();
        } else {
            mpFlagModelMorf->getModel()->getModelData()->show();
        }

        mpFlagModelMorf->entryDL();
        dComIfGd_addRealShadow(mShadowKey, mpFlagModelMorf->getModel());
    }

    for (int i = 0; i < 2; i++) {
        if (mpLetterModels[i] != NULL && ((i == 0 && mLetterADispFlag) || (i == 1 && mLetterBDispFlag))) {
            g_env_light.setLightTevColorType_MAJI(mpLetterModels[i], &tevStr);
            mDoMtx_stack_c::copy(model->getAnmMtx(jointNo[i]));

            Mtx mtx;
            MTXCopy(mDoMtx_stack_c::get(), mtx);
            mpLetterModels[i]->setBaseTRMtx(mtx);
            mDoExt_modelUpdateDL(mpLetterModels[i]);
            dComIfGd_addRealShadow(mShadowKey, mpLetterModels[i]);
        }
    }
}

BOOL daNpc_Post_c::setFlagAnm(int i_idx, int i_attr, f32 i_morf) {
    static daNpc_GetParam1 flagAnmData[13] = {
        {BCK_POST_FLAG_WAIT_A, POST},
        {BCK_POST_FLAG_RUN, POST1},
        {BCK_POST_FLAG_STEP, POST},
        {BCK_POST_FLAG_HAND, POST1},
        {BCK_POST_FLAG_HAND_WAIT, POST1},
        {BCK_POST_FLAG_EXPLAIN, POST1},
        {BCK_POST_FLAG_HELLO, POST1},
        {BCK_POST_FLAG_REGRET, POST1},
        {NONE, NONE},
        {NONE, NONE},
        {NONE, NONE},
        {NONE, NONE},
        {NONE, NONE},
    };

    J3DAnmTransform* anm = NULL;
    if (mpFlagModelMorf != NULL) {
        if (flagAnmData[i_idx].fileIdx > 0) {
            anm = getTrnsfrmKeyAnmP(l_resNameList[flagAnmData[i_idx].arcIdx], flagAnmData[i_idx].fileIdx);
        }

        if (anm != NULL) {
            mpFlagModelMorf->setAnm(anm, i_attr, i_morf, 1.0f, 0.0f, -1.0f);
        }
    }

    return TRUE;
}

bool daNpc_Post_c::afterSetMotionAnm(int i_idx, int i_attr, f32 i_morf, int param_4) {
    f32 morf = mCreating == true ? 0.0f : i_morf;
    setFlagAnm(i_idx, i_attr, morf);
}

BOOL daNpc_Post_c::selectAction() {
    mNextAction = NULL;

    #if DEBUG
    if (mHIO->m.common.debug_mode_ON) {
        mNextAction = &daNpc_Post_c::test;
        return TRUE;
    }
    #endif

    switch (mType) {
        default:
            break;
    }

    mNextAction = &daNpc_Post_c::wait;
    return TRUE;
}

BOOL daNpc_Post_c::chkAction(actionFunc action) {
    return mAction == action;
}

BOOL daNpc_Post_c::setAction(actionFunc action) {
    mMode = MODE_EXIT;

    if (mAction) {
        (this->*mAction)(NULL);
    }

    mMode = MODE_ENTER;
    mAction = action;

    if (mAction) {
        (this->*mAction)(NULL);
    }

    return TRUE;
}

BOOL daNpc_Post_c::chkPullOutLetter1() {
    if (mType == TYPE_DELIVER) {
        J3DAnmTransform* anm = NULL;
        anm = getTrnsfrmAnmP(l_resNameList[l_motionAnmData[3].mBckArcIdx], l_motionAnmData[3].mBckFileIdx);
        return anm == mpMorf[0]->getAnm();
    }

    return false;
}

BOOL daNpc_Post_c::chkPullOutLetter2() {
    if (mType == TYPE_DELIVER) {
        J3DAnmTransform* anm = NULL;
        anm = getTrnsfrmAnmP(l_resNameList[l_motionAnmData[4].mBckArcIdx], l_motionAnmData[4].mBckFileIdx);
        return anm == mpMorf[0]->getAnm();
    }

    return false;
}

void daNpc_Post_c::pullOutLetter() {
    u8 uVar1 = mLetterADispFlag;

    if (!mStagger.checkStagger()) {
        mLetterADispFlag = 0;

        if (chkPullOutLetter1()) {
            if (37.0f < mpMorf[0]->getFrame()) {
                mLetterADispFlag = 1;
                if (uVar1 != mLetterADispFlag) {
                    mSound.startCreatureVoice(Z2SE_POST_V_FANFARE, -1);
                }
            }
        } else if (chkPullOutLetter2()) {
            mLetterADispFlag = 1;
        }
    }
}

static int const dummy[2] = {
    -1, -1,
};

int daNpc_Post_c::cutDeliver(int i_staffId) {
    cXyz work;
    csXyz angle;
    int rv = 0;
    int* piVar1 = NULL;
    int prm = -1;
    int timer = 0;
    
    piVar1 = dComIfGp_evmng_getMyIntegerP(i_staffId, "prm");
    if (piVar1 != NULL) {
        prm = *piVar1;
    }

    piVar1 = dComIfGp_evmng_getMyIntegerP(i_staffId, "timer");
    if (piVar1 != NULL) {
        timer = *piVar1;
    }

    if (dComIfGp_getEventManager().getIsAddvance(i_staffId)) {
        switch (prm) {
            case 0:
                work = mActorPos;
                work.y += 300.0f;
                mGndChk.SetPos(&work);
                current.pos.x = work.x;
                current.pos.z = work.z;
                work.y = dComIfG_Bgsp().GroundCross(&mGndChk);
                
                if (work.y != -1e9f) {
                    current.pos.y = work.y;
                }

                old.pos = current.pos;
                setAngle(fopAcM_searchPlayerAngleY(this));
                mAcch.SetWallNone();
                mEventTimer = timer;
                break;

            case 1:
            case 2:
                if (prm == 2) {
                    mHide = false;
                }

                mEventTimer = timer;
                break;

            case 3:
                mFaceMotionSeqMngr.setNo(FACE_MOT_NONE, 0.0f, TRUE, 0);
                mMotionSeqMngr.setNo(MOT_RUN, 0.0f, TRUE, 0);
                mHide = false;
                mEventTimer = timer;
                mAcch.ClrGrndNone();
                mDoAud_subBgmStart(Z2BGM_POSTMAN);
                Z2GetAudioMgr()->bgmAllUnMute(0);
                break;

            case 4:
                mFaceMotionSeqMngr.setNo(FACE_MOT_NONE, 0.0f, TRUE, 0);
                mMotionSeqMngr.setNo(MOT_RUN, 0.0f, TRUE, 0);
                work.set(0.0f, 0.0f, 1000.0f);
                mDoMtx_stack_c::YrotS(fopAcM_searchActorAngleY(daPy_getPlayerActorClass(), this));
                mDoMtx_stack_c::multVec(&work, &work);
                work += daPy_getPlayerActorClass()->current.pos;
                work.y += 300.0f;
                mGndChk.SetPos(&work);
                current.pos.x = work.x;
                current.pos.z = work.z;
                work.y = dComIfG_Bgsp().GroundCross(&mGndChk);

                if (work.y != -1e9f) {
                    current.pos.y = work.y;
                } else {
                    current.pos.y = daPy_getPlayerActorClass()->current.pos.y;
                    if (daPy_getPlayerActorClass()->checkHorseRide()) {
                        current.pos.y = dComIfGp_getHorseActor()->current.pos.y;
                    }
                }

                old.pos = current.pos;
                speedF = 0.0f;
                speed.setall(0.0f);
                break;

            case 5:
                mFaceMotionSeqMngr.setNo(FACE_MOT_NONE_2, -1.0f, FALSE, 0);
                mMotionSeqMngr.setNo(MOT_WAIT_A, 5.0f, FALSE, 0);
                speedF = 0.0f;
                speed.setall(0.0f);
                initTalk(0x14, NULL);
                break;

            case 6:
                if (getBitSW() != 0xFF) {
                    dComIfGs_onSwitch(getBitSW(), fopAcM_GetRoomNo(this));
                }

                home.pos = current.pos;
                home.angle.y = current.angle.y;
                break;

            case 7:
                angle.y = fopAcM_searchActorAngleY(daPy_getPlayerActorClass(), this);
                work = daPy_getPlayerActorClass()->current.pos;
                work.y += 100.0f;
                mGndChk.SetPos(&work);
                work.y = dComIfG_Bgsp().GroundCross(&mGndChk);

                JUT_ASSERT(2046, -(1000000000.0f) != work.y);

                daPy_getPlayerActorClass()->setPlayerPosAndAngle(&work, angle.y, 0);
                break;

            case 8:
                mFlow.init(NULL, 0x13, 0, NULL);
                mDoAud_bgmAllMute(0x5A);
                break;

            case 9:
                mFaceMotionSeqMngr.setNo(FACE_MOT_NONE, -1.0f, FALSE, 0);
                mMotionSeqMngr.setNo(MOT_RUN, -1.0f, FALSE, 0);
                mEventTimer = timer;
                break;

            case 10:
                mFaceMotionSeqMngr.setNo(FACE_MOT_NONE_2, 0.0f, TRUE, 0);
                mMotionSeqMngr.setNo(MOT_WAIT_A, 0.0f, TRUE, 0);

                if (getBitSW() != 0xFF) {
                    dComIfGs_onSwitch(getBitSW(), fopAcM_GetRoomNo(this));
                }

                speedF = 0.0f;
                speed.setall(0.0f);
                work = home.pos;
                work.y += 300.0f;
                mGndChk.SetPos(&work);
                current.pos.x = work.x;
                current.pos.z = work.z;
                work.y = dComIfG_Bgsp().GroundCross(&mGndChk);

                if (work.y != -1e9f) {
                    current.pos.y = work.y;
                }

                old.pos = current.pos;
                setAngle(home.angle.y);
                mHide = true;
                mEventTimer = timer;
                mDoAud_subBgmStop();
                break;

            case 11:
            case 12:
                break;
        }
    }

    switch (prm) {
        case 0:
            mJntAnm.lookPlayer(0);
            if (cLib_calcTimer(&mEventTimer) == 0) {
                rv = 1;
            }
            break;

        case 1:
        case 2:
            if (cLib_calcTimer(&mEventTimer) == 0) {
                rv = 1;
            }

            mAcch.SetGrndNone();
            mAcch.SetWallNone();
            gravity = 0.0f;
            break;

        case 3:
            if (cLib_calcTimer(&mEventTimer) != 0) {
                speedF = mHIO->m.run_spd;

                if (fopAcM_searchPlayerDistanceXZ(this) < 200.0f) {
                    rv = 1;
                }
            } else {
                rv = 1;
            }
            break;

        case 4:
            if (!dComIfGp_getEventManager().getIsAddvance(i_staffId)) {
                speedF = mHIO->m.run_spd;

                if (daPy_getPlayerActorClass()->checkHorseRide()) {
                    if (fopAcM_searchPlayerDistanceXZ(this) < 400.0f) {
                        rv = 1;
                    }
                } else if (fopAcM_searchPlayerDistanceXZ(this) < 200.0f) {
                    rv = 1;
                }
            }
            break;

        case 5:
            if (talkProc(NULL, FALSE, NULL, FALSE) && mFlow.checkEndFlow()) {
                rv = 1;
            }
            break;

        case 6:
            rv = 1;
            break;

        case 7:
            speedF = mHIO->m.run_spd;
            rv = 1;
            break;

        case 8:
            field_0xe26 = 0;
            if (mFlow.doFlow(NULL, NULL, 0)) {
                rv = 1;
            }
            break;

        case 9:
            mJntAnm.lookNone(0);
            current.angle.y = cLib_targetAngleY(&current.pos, &mActorPos);
            cLib_addCalcAngleS2(&shape_angle.y, current.angle.y, 4, 0x800);
            mCurAngle.y = shape_angle.y;
            cLib_chaseF(&speedF, mHIO->m.run_spd, 1.5f);

            if (cLib_calcTimer(&mEventTimer) == 0) {
                rv = 1;
            }
            break;

        case 10:
            if (cLib_calcTimer(&mEventTimer) == 0) {
                rv = 1;
            }
            break;

        case 11:
            angle.y = cLib_targetAngleY(&current.pos, &mActorPos);
            if (angle.y != mCurAngle.y) {
                if (step(angle.y, 14, 10, 15, 0)) {
                    rv = 1;
                }
            } else {
                rv = 1;
            }
            break;

        case 12:
            rv = 1;
            break;
    }

    return rv;
}

int daNpc_Post_c::wait(void* param_1) {
    daPy_py_c* player = daPy_getPlayerActorClass();
    f32 fVar1 = mHIO->m.nod_interval;

    switch (mMode) {
        case MODE_ENTER:
        case MODE_INIT:
            if (!mStagger.checkStagger()) {
                if (mSitFlag) {
                    mFaceMotionSeqMngr.setNo(FACE_MOT_NONE_2, -1.0f, FALSE, 0);
                    mMotionSeqMngr.setNo(MOT_SIT_A, -1.0f, FALSE, 0);
                    mNodTimer = cLib_getRndValue(fVar1 * 0.5f, fVar1 * 1.5f);
                } else {
                    mFaceMotionSeqMngr.setNo(FACE_MOT_NONE_2, -1.0f, FALSE, 0);
                    mMotionSeqMngr.setNo(MOT_WAIT_A, -1.0f, FALSE, 0);
                }

                mMode = MODE_RUN;
            }
            // fallthrough
        case MODE_RUN:
            if (
                mType == TYPE_DELIVER &&
                (
                    !daPy_py_c::checkNowWolf() ||
                    daNpcT_chkEvtBit(110) // dSv_event_flag_c::M_077 - Main Event - Get shadow crystal (can now transform)
                )
            ) {
                for (int i = 0; i < 4; i++) {
                    daTag_EvtArea_c* actor_p = (daTag_EvtArea_c*)mActorMngrs[i].getActorP();
                    if (actor_p != NULL) {
                        if (actor_p->chkPointInArea(player->current.pos)) {
                            if (daPy_getPlayerActorClass()->checkBoarRide()) {
                                actor_p->noEffect();
                            } else if (daPy_getPlayerActorClass()->eventInfo.chkCondition(1) != FALSE && dMeter2Info_getNewLetterNum()) {
                                mActorPos = actor_p->current.pos;
                                
                                f32 fVar2 = player->current.pos.absXZ(actor_p->current.pos);
                                if (actor_p->scale.x - 700.0f <= fVar2) {
                                    if (daPy_getPlayerActorClass()->checkHorseRide()) {
                                        mEvtNo = EVT_DELIVERTO_PLAYER_ON_HORSE;
                                    } else if (daPy_py_c::checkNowWolf()) {
                                        mEvtNo = EVT_DELIVERTO_WOLF;
                                    } else {
                                        mEvtNo = EVT_DELIVER;
                                    }
                                    break;
                                }
                            }
                        }
                    }
                }
            }

            if (!mStagger.checkStagger()) {
                if (mSitFlag != 0) {
                    mPlayerActorMngr.remove();
                }

                if (mPlayerActorMngr.getActorP() != NULL && !mTwilight) {
                    mJntAnm.lookPlayer(0);
                    
                    if (!chkActorInSight(mPlayerActorMngr.getActorP(), mAttnFovY, mCurAngle.y)) {
                        mJntAnm.lookNone(0);
                    }

                    if (!srchPlayerActor() && home.angle.y == mCurAngle.y) {
                        mMode = MODE_INIT;
                    }
                } else {
                    mJntAnm.lookNone(0);

                    if (home.angle.y != mCurAngle.y) {
                        if (field_0xe34 == 0) {
                            setAngle(home.angle.y);
                            mMode = MODE_INIT;
                        } else if (step(home.angle.y, 14, 10, 15, 0)) {
                            mMode = MODE_INIT;
                        }

                        attention_info.flags = 0;
                    } else if (mSitFlag) {
                        if (mNodTimer != 0) {
                            if (cLib_calcTimer(&mNodTimer) == 0) {
                                mMotionSeqMngr.setNo(MOT_SIT_NOD, -1.0f, FALSE, 0);
                            }
                        } else if (mMotionSeqMngr.checkEndSequence()) {
                            mMode = MODE_INIT;
                        }
                    } else {
                        srchPlayerActor();
                    }
                }

                switch (mJntAnm.getMode()) {
                    case 0:
                        break;
                }
            }
            break;

        case MODE_EXIT:
            break;
    }

    return 1;
}

int daNpc_Post_c::talk(void* param_1) {
    switch (mMode) {
        case MODE_ENTER:
        case MODE_INIT:
            if (!mStagger.checkStagger()) {
                initTalk(mFlowNodeNo, NULL);
                mMode = MODE_RUN;
            }
            // fallthrough
        case MODE_RUN:
            if (!mStagger.checkStagger()) {
                if (mTwilight || mPlayerAngle == mCurAngle.y || mSitFlag) {
                    if (talkProc(NULL, FALSE, NULL, FALSE) && mFlow.checkEndFlow()) {
                        mPlayerActorMngr.entry(daPy_getPlayerActorClass());
                        dComIfGp_event_reset();
                        mMode = MODE_EXIT;
                    }

                    mJntAnm.lookPlayer(0);

                    if (mTwilight || mSitFlag) {
                        mJntAnm.lookNone(0);
                    }
                } else {
                    mJntAnm.lookPlayer(0);
                    step(mPlayerAngle, 14, 10, 15, 0);
                }
            }
            break;

        case MODE_EXIT: 
            break;
    }

    return 0;
}

int daNpc_Post_c::test(void* param_1) {
    switch (mMode) {
        case MODE_ENTER:
        case MODE_INIT:
            speedF = 0.0f;
            speed.setall(0.0f);
            mMode = MODE_RUN;
            // fallthrough
        case MODE_RUN:
            mFaceMotionSeqMngr.setNo(mHIO->m.common.face_expression, -1.0f, FALSE, 0);
            mMotionSeqMngr.setNo(mHIO->m.common.motion, -1.0f, FALSE, 0);
            mJntAnm.lookNone(0);
            attention_info.flags = 0;
            break;

        case MODE_EXIT:
            break;
    }

    return 1;
}

static int daNpc_Post_Create(void* i_this) {
    return static_cast<daNpc_Post_c*>(i_this)->create();
}

static int daNpc_Post_Delete(void* i_this) {
    return static_cast<daNpc_Post_c*>(i_this)->Delete();
}

static int daNpc_Post_Execute(void* i_this) {
    return static_cast<daNpc_Post_c*>(i_this)->Execute();
}

static int daNpc_Post_Draw(void* i_this) {
    return static_cast<daNpc_Post_c*>(i_this)->Draw();
}

static int daNpc_Post_IsDelete(void* i_this) {
    return 1;
}

AUDIO_INSTANCES;

static actor_method_class daNpc_Post_MethodTable = {
    (process_method_func)daNpc_Post_Create,
    (process_method_func)daNpc_Post_Delete,
    (process_method_func)daNpc_Post_Execute,
    (process_method_func)daNpc_Post_IsDelete,
    (process_method_func)daNpc_Post_Draw,
};

extern actor_process_profile_definition g_profile_NPC_POST = {
  fpcLy_CURRENT_e,         // mLayerID
  7,                       // mListID
  fpcPi_CURRENT_e,         // mListPrio
  PROC_NPC_POST,           // mProcName
  &g_fpcLf_Method.base,   // sub_method
  sizeof(daNpc_Post_c),    // mSize
  0,                       // mSizeOther
  0,                       // mParameters
  &g_fopAc_Method.base,    // sub_method
  403,                     // mPriority
  &daNpc_Post_MethodTable, // sub_method
  0x00040107,              // mStatus
  fopAc_NPC_e,             // mActorType
  fopAc_CULLBOX_CUSTOM_e,  // cullType
};
