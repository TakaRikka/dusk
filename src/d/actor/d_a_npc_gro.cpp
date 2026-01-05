/**
 * @file d_a_npc_gro.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_npc_gro.h"
#include "d/actor/d_a_npc_maro.h"
#include "JSystem/JHostIO/JORFile.h"
#include "d/d_debug_viewer.h"
#include "d/actor/d_a_tag_push.h"
#include "Z2AudioLib/Z2Instances.h"

enum grO_RES_File_ID {
    /* BCK */
    /* 0x07 */ BCK_GRO_F_TALK_A = 0x7,
    /* 0x08 */ BCK_GRO_F_TALK_B,
    /* 0x09 */ BCK_GRO_F_WAIT_A,
    /* 0x0A */ BCK_GRO_F_WALK_A,
    /* 0x0B */ BCK_GRO_STEP,
    /* 0x0C */ BCK_GRO_TALK_A,
    /* 0x0D */ BCK_GRO_TALK_B,
    /* 0x0E */ BCK_GRO_WAIT_A,
    /* 0x0F */ BCK_GRO_WALK_A,

    /* BMDR */
    /* 0x12 */ BMDR_GRO_A = 0x12,
    /* 0x13 */ BMDR_GRO_PIPE,

    /* BTK */
    /* 0x16 */ BTK_GRO_A = 0x16,

    /* BTP */
    /* 0x19 */ BTP_GRO_A = 0x19,
    /* 0x1A */ BTP_GRO_F_TALK_B,

    /* EVT */
    /* 0x1D */ EVT_GRO_EVENT_LIST = 0x1D,
};

enum grO1_RES_File_ID {
    /* BCK */
    /* 0x05 */ BCK_GRO_BOKIN_WAIT = 0x5,
    /* 0x06 */ BCK_GRO_BOW,
    /* 0x07 */ BCK_GRO_CALL,
    /* 0x08 */ BCK_GRO_CALL_WAIT,
    /* 0x09 */ BCK_GRO_F_BOKIN,
    /* 0x0A */ BCK_GRO_F_BOW,
    /* 0x0B */ BCK_GRO_F_CALL,
    /* 0x0C */ BCK_GRO_F_CALL_B,
    /* 0x0D */ BCK_GRO_F_SWING,
    /* 0x0E */ BCK_GRO_FH_BOKIN,
    /* 0x0F */ BCK_GRO_FH_BOW,
    /* 0x10 */ BCK_GRO_FH_CALL_WAIT,
    /* 0x11 */ BCK_GRO_SWING,

    /* BMDR */
    /* 0x14 */ BMDR_MARO_BOKIN = 0x14,

    /* BTP */
    /* 0x17 */ BTP_GRO_F_BOW = 0x17,
    /* 0x18 */ BTP_GRO_F_CALL,
    /* 0x19 */ BTP_GRO_F_CALL_B,
    /* 0x1A */ BTP_GRO_FH_CALL_WAIT,
};

enum grO1D_RES_File_ID {
    /* EVT */
    /* 0x3 */ EVT_GRO1D_EVENT_LIST = 0x3,
};

enum RES_Name {
    /* 0x0 */ GRO,
    /* 0x1 */ GRO1,
    /* 0x2 */ GRO1D,
};

enum Event {
    /* 0x0 */ EVT_NONE,
    /* 0x1 */ EVT_BOKIN_FINISH,
    /* 0x2 */ EVT_PUSHOUT,
};

enum Event_Cut_Nums {
    /* 0x3 */ NUM_EVT_CUTS_e = 0x3,
};

enum Joint {
    /* 0x00 */ JNT_CENTER,
    /* 0x01 */ JNT_BACKBONE1,
    /* 0x02 */ JNT_BACKBONE2,
    /* 0x03 */ JNT_NECK,
    /* 0x04 */ JNT_HEAD,
    /* 0x05 */ JNT_CHIN,
    /* 0x06 */ JNT_BEARD,
    /* 0x07 */ JNT_HAIRL,
    /* 0x08 */ JNT_HAIRR,
    /* 0x09 */ JNT_MAYU,
    /* 0x0A */ JNT_MOUTH,
    /* 0x0B */ JNT_SHOULDERL,
    /* 0x0C */ JNT_ARML1,
    /* 0x0D */ JNT_ARML2,
    /* 0x0E */ JNT_HANDL,
    /* 0x0F */ JNT_SHOULDERR,
    /* 0x10 */ JNT_ARMR1,
    /* 0x11 */ JNT_ARMR2,
    /* 0x12 */ JNT_HANDR,
    /* 0x13 */ JNT_WAIST,
    /* 0x14 */ JNT_LEGL1,
    /* 0x15 */ JNT_LEGL2,
    /* 0x16 */ JNT_FOOTL,
    /* 0x17 */ JNT_LEGR1,
    /* 0x18 */ JNT_LEGR2,
    /* 0x19 */ JNT_FOOTR,
};

enum Animation {
    /* 0x00 */ ANM_NONE,
    /* 0x01 */ ANM_F_WAIT_A,
    /* 0x02 */ ANM_F_WALK_A,
    /* 0x03 */ ANM_F_TALK_A,
    /* 0x04 */ ANM_F_TALK_B,
    /* 0x05 */ ANM_F_CALL,
    /* 0x06 */ ANM_FH_CALL_WAIT,
    /* 0x07 */ ANM_F_CALL_B,
    /* 0x08 */ ANM_F_BOW,
    /* 0x09 */ ANM_FH_BOW,
    /* 0x0A */ ANM_F_BOKIN,
    /* 0x0B */ ANM_FH_BOKIN,
    /* 0x0C */ ANM_F_SWING,
    /* 0x0D */ ANM_WAIT_A,
    /* 0x0E */ ANM_WALK_A,
    /* 0x0F */ ANM_TALK_A,
    /* 0x10 */ ANM_TALK_B,
    /* 0x11 */ ANM_STEP,
    /* 0x12 */ ANM_BOKIN_WAIT,
    /* 0x13 */ ANM_CALL,
    /* 0x14 */ ANM_CALL_WAIT,
    /* 0x15 */ ANM_BOW,
    /* 0x16 */ ANM_SWING,
};
enum Expression_BTP {
    /* 0x0 */ EXPR_BTP_GRO_A,
    /* 0x1 */ EXPR_BTP_TALK_B,
    /* 0x2 */ EXPR_BTP_CALL,
    /* 0x3 */ EXPR_BTP_H_CALL_WAIT,
    /* 0x4 */ EXPR_BTP_CALL_B,
    /* 0x5 */ EXPR_BTP_BOW,
};

enum Expression {
    /* 0x0 */ EXPR_TALK_A,
    /* 0x1 */ EXPR_TALK_B,
    /* 0x2 */ EXPR_CALL,
    /* 0x3 */ EXPR_CALL_B,
    /* 0x4 */ EXPR_BOW,
    /* 0x5 */ EXPR_BOKIN,
    /* 0x6 */ EXPR_UNK,
    /* 0x7 */ EXPR_SWING,
    /* 0x8 */ EXPR_H_CALL_WAIT,
    /* 0x9 */ EXPR_H_BOW,
    /* 0xA */ EXPR_H_BOKIN,
    /* 0xB */ EXPR_WAIT_A,
    /* 0xC */ EXPR_NONE,
};

enum Motion {
    /* 0x0 */ MOT_WAIT_A,
    /* 0x1 */ MOT_WALK_A,
    /* 0x2 */ MOT_TALK_A,
    /* 0x3 */ MOT_TALK_B,
    /* 0x4 */ MOT_BOKIN_WAIT,
    /* 0x5 */ MOT_CALL,
    /* 0x6 */ MOT_BOW,
    /* 0x7 */ MOT_SWING,
    /* 0x8 */ MOT_STEP,
};

enum Type {
    /* 0x0 */ TYPE_MINES,
    /* 0x1 */ TYPE_BOKIN,
    /* 0x2 */ TYPE_MARO,
    /* 0x3 */ TYPE_DEFAULT,
};

enum Mode {
    /* 0x0 */ MODE_INIT,
    /* 0x2 */ MODE_RUN = 0x2,
    /* 0x3 */ MODE_EXIT,
};

static NPC_GRO_HIO_CLASS l_HIO;

daNpc_Maro_c::actionFunc dummy_lit_3931() {
    return &daNpc_Maro_c::choccai;
}

static daNpc_GetParam1 l_bmdGetParamList[3] = {
    {BMDR_GRO_A, GRO},
    {BMDR_GRO_PIPE, GRO},
    {BMDR_MARO_BOKIN, GRO1},
};

static daNpc_GetParam1 l_bckGetParamList[23] = {
    {-1, GRO},
    {BCK_GRO_F_WAIT_A, GRO},
    {BCK_GRO_F_WALK_A, GRO},
    {BCK_GRO_F_TALK_A, GRO},
    {BCK_GRO_F_TALK_B, GRO},
    {BCK_GRO_F_CALL, GRO1},
    {BCK_GRO_FH_CALL_WAIT, GRO1},
    {BCK_GRO_F_CALL_B, GRO1},
    {BCK_GRO_F_BOW, GRO1},
    {BCK_GRO_FH_BOW, GRO1},
    {BCK_GRO_F_BOKIN, GRO1},
    {BCK_GRO_FH_BOKIN, GRO1},
    {BCK_GRO_F_SWING, GRO1},
    {BCK_GRO_WAIT_A, GRO},
    {BCK_GRO_WALK_A, GRO},
    {BCK_GRO_TALK_A, GRO},
    {BCK_GRO_TALK_B, GRO},
    {BCK_GRO_STEP, GRO},
    {BCK_GRO_BOKIN_WAIT, GRO1},
    {BCK_GRO_CALL, GRO1},
    {BCK_GRO_CALL_WAIT, GRO1},
    {BCK_GRO_BOW, GRO1},
    {BCK_GRO_SWING, GRO1},
};

static daNpc_GetParam1 l_btpGetParamList[6] = {
    {BTP_GRO_A, GRO},
    {BTP_GRO_F_TALK_B, GRO},
    {BTP_GRO_F_CALL, GRO1},
    {BTP_GRO_FH_CALL_WAIT, GRO1},
    {BTP_GRO_F_CALL_B, GRO1},
    {BTP_GRO_F_BOW, GRO1},
};

static daNpc_GetParam1 l_btkGetParamList[1] = {
    {BTK_GRO_A, GRO},
};

static daNpc_GetParam1 l_evtGetParamList[3] = {
    {0, GRO},
    {1, GRO1D},
    {2, GRO},
};

static int l_loadRes_GROa[4] = {
    GRO, -1, -1, -1,
};

static int l_loadRes_GRO_BOKIN[4] = {
    GRO, GRO1, GRO1D, -1,
};

static int l_loadRes_GRO_MARO[4] = {
    GRO, GRO1, -1, -1,
};

static int l_loadRes_GRO0[4] = {
    GRO, -1, -1, -1,
};

static int* l_loadRes_list[4] = {
    l_loadRes_GROa,
    l_loadRes_GRO_BOKIN,
    l_loadRes_GRO_MARO,
    l_loadRes_GRO0,
};

static int l_loadObj_list[2][2] = {
    {1, 2},
    {-1, 1},
};

static char* l_resNames[3] = {
    "grO",
    "grO1",
    "grO1D",
};

static char* l_evtNames[3] = {
    NULL,
    "BOKIN_FINISH",
    "PUSHOUT",
};

static char* l_myName = "grO";

#if DEBUG
daNpc_grO_HIO_c::daNpc_grO_HIO_c() {
    m = daNpc_grO_Param_c::m;
}

void daNpc_grO_HIO_c::listenPropertyEvent(const JORPropertyEvent* event) {
    char buffer[2000];

    JORReflexible::listenPropertyEvent(event);

    JORFile jorFile;
    int len;

    switch ((u32)event->id) {
        case 0x40000002:
            if (jorFile.open(6, "すべてのファイル(*.*)\0*.*\0", NULL, NULL, NULL)) {
                memset(buffer, 0, sizeof(buffer));
                len = 0;
                daNpcF_commonListenPropertyEvent(buffer, &len, &m.common);
                jorFile.writeData(buffer, len);
                jorFile.close();
                OS_REPORT("write append success!::%6d\n", len);
            } else {
                OS_REPORT("write append failure!\n");
            }
            break;
    }
}

void daNpc_grO_HIO_c::genMessage(JORMContext* ctx) {
    daNpcF_commonGenMessage(ctx, &m.common);
    ctx->genButton("ファイル書き出し", 0x40000002, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
}
#endif

char* daNpc_grO_c::mEvtCutNameList[3] = {
    "",
    "BOKIN_FINISH",
    "PUSHOUT",
};

daNpc_grO_c::cutFunc daNpc_grO_c::mEvtCutList[3] = {
    NULL,
    &daNpc_grO_c::ECut_bokinFinish,
    &daNpc_grO_c::cutPushOut,
};

daNpc_grO_c::daNpc_grO_c() {}

daNpc_grO_c::~daNpc_grO_c() {
    for (int i = 0; l_loadRes_list[mType][i] >= 0; i++) {
        dComIfG_resDelete(&mPhases[i], l_resNames[l_loadRes_list[mType][i]]);
    }

    if (heap != NULL) {
        mAnm_p->stopZelAnime();
    }

    #if DEBUG
    if (mpHIO != NULL) {
        mpHIO->removeHIO();
    }
    #endif
}

daNpc_grO_HIOParam const daNpc_grO_Param_c::m = {
    160.0f,
    -3.0f,
    1.0f,
    600.0f,
    255.0f,
    140.0f,
    35.0f,
    90.0f,
    0.0f,
    0.0f,
    10.0f,
    -10.0f,
    30.0f,
    -30.0f,
    45.0f,
    -45.0f,
    0.6f,
    12.0f,
    3,
    3,
    5,
    3,
    180.0f,
    500.0f,
    300.0f,
    -300.0f,
    60,
    8,
    0,
    0,
    0,
    false,
    false,
};

cPhs__Step daNpc_grO_c::create() {
    fopAcM_ct(this, daNpc_grO_c);

    mType = getTypeFromParam();

    if (home.angle.x != 0xFFFF) {
        mMsgNo = home.angle.x;
    } else {
        mMsgNo = -1;
    }

    if (isDelete()) {
        return cPhs_ERROR_e;
    }

    cPhs__Step phase;
    int res = 0;
    int i = 0;
    for (; l_loadRes_list[mType][i] >= 0; i++) {
        phase = (cPhs__Step)dComIfG_resLoad(&mPhases[i], l_resNames[l_loadRes_list[mType][i]]);
        if (phase == cPhs_ERROR_e || phase == cPhs_UNK3_e) {
            return cPhs_ERROR_e;
        }

        if (phase == cPhs_COMPLEATE_e) {
            res++;
        }
    }

    if (res == i) {
        static u32 const heapSize[4] = {
            0x3D50, 0x3D60, 0x3550, 0x0000,
        };

        if (!fopAcM_entrySolidHeap(this, createHeapCallBack, heapSize[mType])) {
            return cPhs_ERROR_e;
        }

        J3DModelData* mdlData_p = mAnm_p->getModel()->getModelData();
        fopAcM_SetMtx(this, mAnm_p->getModel()->getBaseTRMtx());
        fopAcM_setCullSizeBox(this, -300.0f, -50.0f, -300.0f, 300.0f, 450.0f, 300.0f);
        mSound.init(&current.pos, &eyePos, 3, 1);

        #if DEBUG
        mpHIO = &l_HIO;
        mpHIO->entryHIO("老いゴロン");
        #endif

        mAcchCir.SetWall(mpHIO->m.common.width, mpHIO->m.common.knee_length);
        mAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1, &mAcchCir,
                fopAcM_GetSpeed_p(this), fopAcM_GetAngle_p(this), fopAcM_GetShapeAngle_p(this));
        mCcStts.Init(mpHIO->m.common.weight, 0, this);
        mCyl.Set(mCcDCyl);
        mCyl.SetStts(&mCcStts);
        mCyl.SetTgHitCallback(tgHitCallBack);
        mAcch.CrrPos(dComIfG_Bgsp());
        mGndChk = mAcch.m_gnd;
        mGroundH = mAcch.GetGroundH();

        setEnvTevColor();
        setRoomNo();
        reset();
        Execute();

        return cPhs_COMPLEATE_e;
    }

    return cPhs_INIT_e;
}

int daNpc_grO_c::CreateHeap() {
    J3DModelData* mdlData_p = NULL;
    J3DModel* mdl_p = NULL;
    if (l_bmdGetParamList[0].fileIdx >= 0) {
        mdlData_p = (J3DModelData*)dComIfG_getObjectRes(l_resNames[l_bmdGetParamList[0].arcIdx], l_bmdGetParamList[0].fileIdx);
    }
    JUT_ASSERT(480, NULL != mdlData_p);

    u32 uVar1 = 0x11020284;
    mAnm_p = new mDoExt_McaMorfSO(mdlData_p, NULL, NULL, NULL, -1, 1.0f, 0, -1, &mSound, J3DMdlFlag_DifferedDLBuffer, uVar1);
    if (mAnm_p != NULL && mAnm_p->getModel() == NULL) {
        mAnm_p->stopZelAnime();
        mAnm_p = NULL;
    }

    if (mAnm_p == NULL) {
        return 0;
    }

    mdl_p = mAnm_p->getModel();
    for (u16 i = 0; i < mdlData_p->getJointNum(); i++) {
        mdlData_p->getJointNodePointer(i)->setCallBack(ctrlJointCallBack);
    }
    mdl_p->setUserArea((uintptr_t)this);

    mpMatAnm = new daNpcF_MatAnm_c();
    if (mpMatAnm == NULL) {
        return 0;
    }

    for (int i = 0; i < 1; i++) {
        int mdlIdx = l_loadObj_list[i][mType];
        if (mdlIdx > 0) {
            J3DModelData* mdlData_p = NULL;
            mdlData_p = (J3DModelData*)dComIfG_getObjectRes(l_resNames[l_bmdGetParamList[mdlIdx].arcIdx], l_bmdGetParamList[mdlIdx].fileIdx);
            if (mdlData_p != NULL) {
                mpOtherModel[i] = mDoExt_J3DModel__create(mdlData_p, J3DMdlFlag_DifferedDLBuffer, 0x11000084);
                if (mpOtherModel[i] == NULL) {
                    return 0;
                }
            } else {
                return 0;
            }
        } else {
            mpOtherModel[i] = NULL;
        }
    }

    if (!setExpressionAnm(ANM_F_TALK_A, false)) {
        return 0;
    }

    setMotionAnm(ANM_WAIT_A, 0.0f);

    return 1;
}

int daNpc_grO_c::Delete() {
    fpc_ProcID id = fopAcM_GetID(this);
    this->~daNpc_grO_c();
    return 1;
}

int daNpc_grO_c::Execute() {
    return execute();
}

int daNpc_grO_c::Draw() {
    J3DModelData* mdlData_p = mAnm_p->getModel()->getModelData();
    mdlData_p->getMaterialNodePointer(1)->setMaterialAnm(mpMatAnm);
    return draw(chkAction(&daNpc_grO_c::test), FALSE, mpHIO->m.common.real_shadow_size, NULL, FALSE);
}

int daNpc_grO_c::ctrlJoint(J3DJoint* i_joint, J3DModel* i_model) {
    int jntNo = i_joint->getJntNo();
    int i_jointList[3] = {JNT_BACKBONE1, JNT_NECK, JNT_HEAD};

    if (jntNo == JNT_CENTER) {
        mDoMtx_stack_c::copy(mAnm_p->getModel()->getAnmMtx(JNT_BACKBONE1));
        mDoMtx_stack_c::multVecZero(&mLookatPos[0]);
        mDoMtx_stack_c::copy(mAnm_p->getModel()->getAnmMtx(JNT_NECK));
        mDoMtx_stack_c::multVecZero(&mLookatPos[1]);
        mDoMtx_stack_c::copy(mAnm_p->getModel()->getAnmMtx(JNT_HEAD));
        mDoMtx_stack_c::multVecZero(&mLookatPos[2]);
    }

    mDoMtx_stack_c::copy(i_model->getAnmMtx(jntNo));

    switch (jntNo) {
        case JNT_BACKBONE1:
        case JNT_NECK:
        case JNT_HEAD:
            setLookatMtx(jntNo, i_jointList, mpHIO->m.common.neck_rotation_ratio);
            break;
    }

    if (jntNo == JNT_BACKBONE1) {
        mDoMtx_stack_c::ZrotM(field_0x908[0].x);
        mDoMtx_stack_c::YrotM(-field_0x908[0].z);
    } else if (jntNo == JNT_HEAD) {
        mDoMtx_stack_c::ZrotM(field_0x908[2].x);
        mDoMtx_stack_c::YrotM(-field_0x908[2].z);
    }

    i_model->setAnmMtx(jntNo, mDoMtx_stack_c::get());
    MTXCopy(mDoMtx_stack_c::get(), J3DSys::mCurrentMtx);

    if ((jntNo == JNT_HEAD || jntNo == JNT_MOUTH) && (mAnmFlags & ANM_PLAY_BCK) != 0) {
        J3DAnmTransform* anm = mBckAnm.getBckAnm();
        mBckAnm.changeBckOnly(mAnm_p->getAnm());
        mAnm_p->changeAnm(anm);
    }

    return 1;
}

int daNpc_grO_c::createHeapCallBack(fopAc_ac_c* i_this) {
    daNpc_grO_c* actor = (daNpc_grO_c*)i_this;
    return actor->CreateHeap();
}

int daNpc_grO_c::ctrlJointCallBack(J3DJoint* i_joint, int param_2) {
    if (param_2 == 0) {
        J3DModel* model = j3dSys.getModel();
        daNpc_grO_c* i_this = (daNpc_grO_c*)model->getUserArea();

        if (i_this != NULL) {
            i_this->ctrlJoint(i_joint, model);
        }
    }

    return 1;
}

void daNpc_grO_c::setParam() {
    actionFunc action = mNextAction;
    u32 flags = (fopAc_AttnFlag_SPEAK_e | fopAc_AttnFlag_TALK_e);

    selectAction();

    if (!mTwilight && daPy_py_c::checkNowWolf()) {
        flags = 0;
    }

    if (mNextAction != action) {
        for (int i = 3; i < 3; i++) {
            mActorMngrs[i].initialize();
        }
    }

    field_0xe0c = 0;
    field_0xe10 = 0;

    s16 attention_distance, attention_angle, talk_distance, talk_angle;
    talk_distance = mpHIO->m.common.talk_distance;
    talk_angle = mpHIO->m.common.talk_angle;
    attention_distance = mpHIO->m.common.attention_distance;
    attention_angle = mpHIO->m.common.attention_angle;

    attention_info.distances[fopAc_attn_LOCK_e] = getDistTableIdx(attention_distance, attention_angle);
    attention_info.distances[fopAc_attn_TALK_e] = attention_info.distances[fopAc_attn_LOCK_e];
    attention_info.distances[fopAc_attn_SPEAK_e] = getDistTableIdx(talk_distance, talk_angle);
    attention_info.flags = flags;

    scale.set(mpHIO->m.common.scale, mpHIO->m.common.scale, mpHIO->m.common.scale);
    mAcchCir.SetWallR(mpHIO->m.common.width);
    mAcchCir.SetWallH(mpHIO->m.common.knee_length);
    gravity = mpHIO->m.common.gravity;
}

BOOL daNpc_grO_c::main() {
    if (!doEvent()) {
        doNormalAction(1);
    }

    if (checkHide()) {
        attention_info.flags = 0;
    }

    if (!mpHIO->m.common.debug_mode_ON && (!dComIfGp_event_runCheck() || (mOrderNewEvt && dComIfGp_getEvent()->isOrderOK()))) {
        if (mOrderEvtNo != EVT_NONE) {
            eventInfo.setArchiveName(l_resNames[l_evtGetParamList[mOrderEvtNo].arcIdx]);
        }

        orderEvent(mUnkFlag, l_evtNames[l_evtGetParamList[mOrderEvtNo].fileIdx], 0xFFFF, 40, 0xFF, 1);
    }

    if (field_0x9ee) {
        mExpressionMorfOverride = 0.0f;
        mMotionMorfOverride = 0.0f;
        field_0x9ee = false;
    }

    playExpression();
    playMotion();

    return TRUE;
}

BOOL daNpc_grO_c::ctrlBtk() {
    if (mpMatAnm != NULL) {
        J3DAnmTextureSRTKey* btk = NULL;

        if (l_btkGetParamList[0].fileIdx >= 0) {
            btk = getTexSRTKeyAnmP(l_resNames[l_btkGetParamList[0].arcIdx], l_btkGetParamList[0].fileIdx);
        }

        if (btk == mBtkAnm.getBtkAnm()) {
            mpMatAnm->setNowOffsetX(cM_ssin(mEyeAngle.y) * 0.02f * -1.0f);
            mpMatAnm->setNowOffsetY(cM_ssin(mEyeAngle.x) * 0.02f);
            mpMatAnm->onEyeMoveFlag();
            
            return TRUE;
        }

        mpMatAnm->offEyeMoveFlag();
    }

    return FALSE;
}

void daNpc_grO_c::setAttnPos() {
    static cXyz eyeOffset(30.0f, 10.0f, 0.0f);

    cXyz* attnPos = NULL;
    int unused = 0;
    cXyz sp7c, sp88, sp94, spa0;
    f32 attention_offset = mpHIO->m.common.attention_offset;
    f32 pZ = 0.0f;
    f32 fVar1 = 0.0f;
    f32 fVar2 = 0.0f;

    mDoMtx_stack_c::YrotS(field_0x990);
    cLib_addCalc2(&field_0x984[0], 0.0f, 0.1f, 125.0f);
    cLib_addCalc2(&field_0x984[2], 0.0f, 0.1f, 125.0f);

    for (int i = 0; i < 3; i++) {
        sp94.set(0.0f, 0.0f, field_0x984[i] * cM_ssin(field_0x992));
        mDoMtx_stack_c::multVec(&sp94, &spa0);
        field_0x908[i].x = -spa0.z;
        field_0x908[i].z = -spa0.x;
    }

    cLib_chaseS(&field_0x992, 0, 0x555);

    if (mLookMode == LOOK_RESET) {
        for (int i = 0; i < 3; i++) {
            mLookatAngle[i].setall(0);
        }
    }

    setMtx();
    lookat();

    mDoMtx_stack_c::copy(mAnm_p->getModel()->getAnmMtx(4));
    mDoMtx_stack_c::multVecZero(&mHeadPos);
    mDoMtx_stack_c::multVec(&eyeOffset, &eyePos);
    eyeOffset.y = 0.0f;
    mDoMtx_stack_c::multVec(&eyeOffset, &sp94);

    mHeadAngle.x = cLib_targetAngleX(&mHeadPos, &sp94);
    mHeadAngle.y = cLib_targetAngleY(&mHeadPos, &sp94);

    attnPos = mLookat.getAttnPos();
    if (attnPos != NULL) {
        sp88 = *attnPos - eyePos;
        mEyeAngle.y = -mLookatAngle[2].y - mCurAngle.y;
        mEyeAngle.y += cM_atan2s(sp88.x, sp88.z);
        mEyeAngle.x = -cM_atan2s(sp88.y, sp88.absXZ());
    } else {
        mEyeAngle.y = 0;
        mEyeAngle.x = 0;
    }

    pZ = 60.0f;
    fVar1 = 0.0f;
    fVar2 = 0.0f;

    sp88.set(0.0f, 0.0f, pZ);
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::ZXYrotM(mCurAngle.x, mCurAngle.y, mCurAngle.z);
    mDoMtx_stack_c::multVec(&sp88, &sp7c);

    attention_info.position.set(sp7c.x, sp7c.y + attention_offset, sp7c.z);

    if (!mHide && !mTwilight) {
        if (!mIsDamaged) {
            mCyl.SetTgType(0xD8FBFDFF);
            mCyl.SetTgSPrm(0x1F);
            mCyl.OnTgNoHitMark();
        } else {
            mCyl.SetTgType(0);
            mCyl.SetTgSPrm(0);
        }

        mCyl.SetC(sp7c);
        mCyl.SetH(mpHIO->m.common.height + fVar1);
        mCyl.SetR(mpHIO->m.common.width + fVar2);
        dComIfG_Ccsp()->Set(&mCyl);
    }

    mCyl.ClrTgHit();
}

bool daNpc_grO_c::setExpressionAnm(int i_idx, bool i_modify) {
    J3DAnmTransform* anm = NULL;
    int attr = J3DFrameCtrl::EMode_NONE;

    mAnmFlags &= ~ANM_EXPRESSION_FLAGS;

    if (l_bckGetParamList[i_idx].fileIdx >= 0) {
        anm = getTrnsfrmKeyAnmP(l_resNames[l_bckGetParamList[i_idx].arcIdx], l_bckGetParamList[i_idx].fileIdx);
    }

    bool res = false;
    switch (i_idx) {
        case ANM_NONE:
            res = setExpressionBtp(EXPR_BTP_GRO_A);
            break;

        case ANM_F_WAIT_A:
            res = setExpressionBtp(EXPR_BTP_GRO_A);
            attr = J3DFrameCtrl::EMode_LOOP;
            break;

        case ANM_F_WALK_A:
            res = setExpressionBtp(EXPR_BTP_GRO_A);
            attr = J3DFrameCtrl::EMode_LOOP;
            break;

        case ANM_F_TALK_A:
            res = setExpressionBtp(EXPR_BTP_GRO_A);
            break;

        case ANM_F_TALK_B:
            res = setExpressionBtp(EXPR_BTP_TALK_B);
            break;

        case ANM_F_CALL:
            res = setExpressionBtp(EXPR_BTP_CALL);
            break;

        case ANM_FH_CALL_WAIT:
            res = setExpressionBtp(EXPR_BTP_H_CALL_WAIT);
            attr = J3DFrameCtrl::EMode_LOOP;
            break;

        case ANM_F_CALL_B:
            res = setExpressionBtp(EXPR_BTP_CALL_B);
            break;

        case ANM_F_BOW:
            res = setExpressionBtp(EXPR_BTP_BOW);
            break;

        case ANM_FH_BOW:
            res = setExpressionBtp(EXPR_BTP_GRO_A);
            attr = J3DFrameCtrl::EMode_LOOP;
            break;

        case ANM_F_BOKIN:
            res = setExpressionBtp(EXPR_BTP_GRO_A);
            break;

        case ANM_FH_BOKIN:
            res = setExpressionBtp(EXPR_BTP_GRO_A);
            attr = J3DFrameCtrl::EMode_LOOP;
            break;

        case ANM_F_SWING:
            res = setExpressionBtp(EXPR_BTP_GRO_A);
            attr = J3DFrameCtrl::EMode_LOOP;
            break;
        
        default:
            anm = NULL;
            break;
    }

    if (!res) {
        return false;
    }

    if (anm == NULL) {
        return true;
    }

    if (setBckAnm(anm, 1.0f, attr, 0, -1, i_modify)) {
        mAnmFlags |= ANM_PAUSE_BCK | ANM_PLAY_BCK;
        mExpressionLoops = 0;
        return true;
    }

    OS_REPORT("%s: 表情Bckアニメーションの登録に失敗しました！\n", __FILE__);

    return false;
}

bool daNpc_grO_c::setExpressionBtp(int i_idx) {
    J3DAnmTexPattern* anm = NULL;
    int attr = J3DFrameCtrl::EMode_NONE;

    mAnmFlags &= ~(ANM_PLAY_BTP | ANM_PAUSE_BTP | ANM_FLAG_800);

    if (l_btpGetParamList[i_idx].fileIdx >= 0) {
        anm = getTexPtrnAnmP(l_resNames[l_btpGetParamList[i_idx].arcIdx], l_btpGetParamList[i_idx].fileIdx);
    }

    switch (i_idx) {
        case EXPR_BTP_GRO_A:
            attr = J3DFrameCtrl::EMode_LOOP;
            break;

        case EXPR_BTP_TALK_B:
        case EXPR_BTP_CALL:
            break;
        
        case EXPR_BTP_H_CALL_WAIT:
            attr = J3DFrameCtrl::EMode_LOOP;
            break;

        case EXPR_BTP_CALL_B:
        case EXPR_BTP_BOW:
            break;

        default:
            anm = NULL;
            break;
    }

    if (anm == NULL) {
        return true;
    }

    if (setBtpAnm(anm, mAnm_p->getModel()->getModelData(), 1.0f, attr)) {
        mAnmFlags |= ANM_PAUSE_BTP | ANM_PLAY_BTP;

        if (i_idx == 0) {
            mAnmFlags |= ANM_FLAG_800;
        }

        return true;
    }

    OS_REPORT("%s: 表情Btpアニメーションの登録に失敗しました！\n", __FILE__);

    return false;
}

void daNpc_grO_c::setExpression(int i_expression, f32 i_morf) {
    if (i_expression >= 0 && i_expression < 13) {
        mExpression = i_expression;
        mExpressionMorfOverride = i_morf;
        mExpressionPrevPhase = -1;
        mExpressionPhase = 0;
    }
}

void daNpc_grO_c::setMotionAnm(int i_idx, f32 i_morf) {
    J3DAnmTransformKey* anm = NULL;
    J3DAnmTextureSRTKey* i_btk = NULL;
    int btkIdx = 0;
    int attr = J3DFrameCtrl::EMode_LOOP;
    int btkAttr = J3DFrameCtrl::EMode_LOOP;

    mAnmFlags &= ~ANM_MOTION_FLAGS;

    if (l_bckGetParamList[i_idx].fileIdx >= 0) {
        anm = getTrnsfrmKeyAnmP(l_resNames[l_bckGetParamList[i_idx].arcIdx], l_bckGetParamList[i_idx].fileIdx);
    }

    switch (i_idx) {
        case ANM_WAIT_A:
        case ANM_WALK_A:
        case ANM_BOKIN_WAIT:
        case ANM_CALL_WAIT:
        case ANM_SWING:
            break;

        case ANM_TALK_A:
            attr = J3DFrameCtrl::EMode_NONE;
            break;

        case ANM_TALK_B:
            attr = J3DFrameCtrl::EMode_NONE;
            break;

        case ANM_STEP:
            attr = J3DFrameCtrl::EMode_NONE;
            break;

        case ANM_CALL:
            attr = J3DFrameCtrl::EMode_NONE;
            break;

        case ANM_BOW:
            attr = J3DFrameCtrl::EMode_NONE;
            break;

        default:
            anm = NULL;
            i_btk = NULL;
            break;
    }

    if (l_btkGetParamList[btkIdx].fileIdx >= 0) {
        i_btk = getTexSRTKeyAnmP(l_resNames[l_btkGetParamList[btkIdx].arcIdx], l_btkGetParamList[btkIdx].fileIdx);
    }

    if (i_btk != NULL && setBtkAnm(i_btk, mAnm_p->getModel()->getModelData(), 1.0f, btkAttr)) {
        mAnmFlags |= ANM_PAUSE_BTK | ANM_PLAY_BTK;
    }

    if (anm != NULL && setMcaMorfAnm(anm, 1.0f, i_morf, attr, 0, -1)) {
        mAnmFlags |= ANM_PLAY_MORF | ANM_PAUSE_MORF;
        mMotionLoops = 0;
    }
}

void daNpc_grO_c::setMotion(int i_motion, f32 i_morf, int param_3) {
    s16 motion = i_motion;
    if ((param_3 != 0 || mMotion != motion) && i_motion >= 0 && i_motion < 9) {
        mMotion = motion;
        mMotionMorfOverride = i_morf;
        mMotionPrevPhase = -1;
        mMotionPhase = 0;
    }
}

int daNpc_grO_c::drawDbgInfo() {
    #if DEBUG
    if (mpHIO->m.common.debug_info_ON) {
        cXyz work;
        f32 distMax = dComIfGp_getAttention()->getDistTable(attention_info.distances[fopAc_attn_SPEAK_e]).mDistMax;
        f32 distMax2 = dComIfGp_getAttention()->getDistTable(attention_info.distances[fopAc_attn_TALK_e]).mDistMax;
        s16 angleX = cLib_targetAngleX(&current.pos, &eyePos);
        s16 angleY = cLib_targetAngleY(&current.pos, &eyePos);
        dDbVw_drawCircleOpa(attention_info.position, distMax, (GXColor){0x00, 0xC8, 0x00, 0xFF}, 1, 12);
        dDbVw_drawCircleOpa(attention_info.position, distMax2, (GXColor){0xC8, 0x00, 0x00, 0xFF}, 1, 12);

        if (mpHIO->m.common.fov != 180.0f) {
            cXyz sp54(0.0f, 0.0f, mpHIO->m.common.search_distance);
            mDoMtx_stack_c::transS(attention_info.position);
            mDoMtx_stack_c::YrotM(mHeadAngle.y);
            mDoMtx_stack_c::YrotM(cM_deg2s(-mpHIO->m.common.fov));
            mDoMtx_stack_c::multVec(&sp54, &work);
            dDbVw_drawLineOpa(attention_info.position, work, (GXColor){0x00, 0x00, 0xC8, 0xFF}, 1, 12);

            mDoMtx_stack_c::transS(attention_info.position);
            mDoMtx_stack_c::YrotM(mHeadAngle.y);
            mDoMtx_stack_c::YrotM(cM_deg2s(mpHIO->m.common.fov));
            mDoMtx_stack_c::multVec(&sp54, &work);
            dDbVw_drawLineOpa(attention_info.position, work, (GXColor){0x00, 0x00, 0xC8, 0xFF}, 1, 12);
        }

        dDbVw_drawCircleOpa(attention_info.position, mpHIO->m.common.search_distance, (GXColor){0x00, 0x00, 0xC8, 0xFF}, 1, 12);
        dDbVw_drawSphereXlu(eyePos, 18.0f, (GXColor){0x80, 0x80, 0x80, 0xA0}, 1);
        dDbVw_drawSphereXlu(mLookPos, 18.0f, (GXColor){0x80, 0x80, 0x80, 0xA0}, 1);
        int i_no = getDistTableIdx(field_0xe0c, field_0xe10);
        distMax = dComIfGp_getAttention()->getDistTable(i_no).mDistMax;
        dDbVw_drawCircleOpa(attention_info.position, distMax, (GXColor){0xC8, 0xC8, 0xC8, 0xFF}, 1, 12);
    }
    #endif
    
    return 0;
}

void daNpc_grO_c::drawOtherMdls() {
    for (int i = 0; i < 1; i++) {
        J3DModel* mdl_p = mpOtherModel[i];
        if (mdl_p != NULL) {
            g_env_light.setLightTevColorType_MAJI(mdl_p, &tevStr);
            mDoMtx_stack_c::copy(mAnm_p->getModel()->getAnmMtx(JNT_HANDR));
            mDoMtx_stack_c::scaleM(scale);
            mdl_p->setBaseTRMtx(mDoMtx_stack_c::get());
            mDoExt_modelUpdateDL(mdl_p);
            dComIfGd_addRealShadow(mShadowKey, mdl_p);
        }
    }
}

u8 daNpc_grO_c::getTypeFromParam() {
    u8 param = fopAcM_GetParam(this) & 0xFF;
    switch (param) {
        case 0:
            return TYPE_MINES;
        
        case 1:
            // dSv_event_flag_c::M_076 - Misc. - First conversation with Castle Town Malo Mart shop clerk
            if (daNpcF_chkEvtBit(109)) {
                return TYPE_MARO;
            }

            return TYPE_BOKIN;

        default:
            return TYPE_MINES;
    }
}

BOOL daNpc_grO_c::isDelete() {
    /* 
        Delete actor if:
        Stage is Goron Mines and the dungeon has been completed
    */
    if (
        // Goron Mines
        strcmp(dComIfGp_getStartStageName(), "D_MN04") == 0 &&
        // dSv_event_flag_c::M_031 - Goron Mines - Goron Mines clear
        daNpcF_chkEvtBit(64)
    ) {
        return TRUE;
    }

    if (
        mType == TYPE_DEFAULT || mType == TYPE_MINES ||
        // dSv_event_flag_c::F_0246 - Kakariko Village - Malo Mart fundraiser (and carying spring water) start
        (mType == TYPE_BOKIN && daNpcF_chkEvtBit(246)) ||
        (mType == TYPE_MARO && daNpcF_chkEvtBit(246))
    ) {
        return FALSE;
    }

    return TRUE;
}

void daNpc_grO_c::reset() {
    initialize();
    mpMatAnm->initialize();
    mLookat.initialize();
    for (int i = 0; i < 3; i++) {
        mActorMngrs[i].initialize();
    }

    mNextAction = NULL;
    mAction = NULL;
    field_0xe0c = 0;
    field_0xe10 = 0;
    field_0xe14 = 0;
    field_0xe18 = 0;
    mLookMode = -1;
    mMode = MODE_INIT;
    mItemID = fpcM_ERROR_PROCESS_ID_e;
    current.pos = home.pos;
    old.pos = current.pos;
    current.angle.set(0, home.angle.y, 0);
    old.angle = current.angle;
    shape_angle = current.angle;
    mCurAngle = current.angle;
    mOldAngle = mCurAngle;
    speedF = 0.0f;
    speed.setall(0.0f);
    mUnkFlag = 0;
    field_0x9ee = true;
}

void daNpc_grO_c::playExpression() {
    daNpcF_anmPlayData dat0a = {ANM_F_TALK_A, mpHIO->m.common.morf_frame, 1};
    daNpcF_anmPlayData dat0b = {ANM_F_WAIT_A, mpHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat0[2] = {&dat0a, &dat0b};
    daNpcF_anmPlayData dat1a = {ANM_F_TALK_B, mpHIO->m.common.morf_frame, 1};
    daNpcF_anmPlayData dat1b = {ANM_F_WAIT_A, mpHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat1[2] = {&dat1a, &dat1b};
    daNpcF_anmPlayData dat2a = {ANM_F_CALL, mpHIO->m.common.morf_frame, 1};
    daNpcF_anmPlayData dat2b = {ANM_FH_CALL_WAIT, mpHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat2[2] = {&dat2a, &dat2b};
    daNpcF_anmPlayData dat3a = {ANM_F_CALL_B, mpHIO->m.common.morf_frame, 1};
    daNpcF_anmPlayData dat3b = {ANM_FH_CALL_WAIT, mpHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat3[2] = {&dat3a, &dat3b};
    daNpcF_anmPlayData dat4a = {ANM_F_BOW, mpHIO->m.common.morf_frame, 1};
    daNpcF_anmPlayData dat4b = {ANM_FH_BOW, mpHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat4[2] = {&dat4a, &dat4b};
    daNpcF_anmPlayData dat5a = {ANM_F_BOKIN, mpHIO->m.common.morf_frame, 1};
    daNpcF_anmPlayData dat5b = {ANM_FH_BOKIN, mpHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat5[2] = {&dat5a, &dat5b};
    daNpcF_anmPlayData dat6 = {ANM_F_SWING, mpHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat6[1] = {&dat6};
    daNpcF_anmPlayData dat7 = {ANM_FH_CALL_WAIT, mpHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat7[1] = {&dat7};
    daNpcF_anmPlayData dat8 = {ANM_FH_BOW, mpHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat8[1] = {&dat8};
    daNpcF_anmPlayData dat9 = {ANM_FH_BOKIN, mpHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat9[1] = {&dat9};
    daNpcF_anmPlayData dat10 = {ANM_F_WAIT_A, mpHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat10[1] = {&dat10};
    daNpcF_anmPlayData dat11 = {ANM_NONE, mpHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat11[1] = {&dat11};

    daNpcF_anmPlayData** ppDat[13] = {
        pDat0,
        pDat1,
        pDat2,
        pDat3,
        pDat4,
        pDat5,
        NULL,
        pDat6,
        pDat7,
        pDat8,
        pDat9,
        pDat10,
        pDat11
    };

    if (mExpression >= 0 && mExpression < 13) {
        playExpressionAnm(ppDat);
    }
}

void daNpc_grO_c::playMotion() {
    daNpcF_anmPlayData dat0 = {ANM_WAIT_A, mpHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat0[1] = {&dat0};
    daNpcF_anmPlayData dat1 = {ANM_WALK_A, mpHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat1[1] = {&dat1};
    daNpcF_anmPlayData dat2a = {ANM_TALK_A, mpHIO->m.common.morf_frame, 1};
    daNpcF_anmPlayData dat2b = {ANM_WAIT_A, mpHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat2[2] = {&dat2a, &dat2b};
    daNpcF_anmPlayData dat3a = {ANM_TALK_B, mpHIO->m.common.morf_frame, 1};
    daNpcF_anmPlayData dat3b = {ANM_WAIT_A, mpHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat3[2] = {&dat3a, &dat3b};
    daNpcF_anmPlayData dat4 = {ANM_BOKIN_WAIT, mpHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat4[1] = {&dat4};
    daNpcF_anmPlayData dat5a = {ANM_CALL, mpHIO->m.common.morf_frame, 1};
    daNpcF_anmPlayData dat5b = {ANM_CALL_WAIT, mpHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat5[2] = {&dat5a, &dat5b};
    daNpcF_anmPlayData dat6a = {ANM_BOW, mpHIO->m.common.morf_frame, 1};
    daNpcF_anmPlayData dat6b = {ANM_BOKIN_WAIT, mpHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat6[2] = {&dat6a, &dat6b};
    daNpcF_anmPlayData dat7 = {ANM_SWING, mpHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat7[1] = {&dat7};
    daNpcF_anmPlayData dat8 = {ANM_STEP, mpHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat8[1] = {&dat8};

    daNpcF_anmPlayData** ppDat[9] = {
        pDat0,
        pDat1,
        pDat2,
        pDat3,
        pDat4,
        pDat5,
        pDat6,
        pDat7,
        pDat8,
    };

    if (mMotion >= 0 && mMotion < 9) {
        playMotionAnm(ppDat);

        if (mMotion == MOT_SWING) {
            mSound.startCreatureVoiceLevel(Z2SE_GRO_MV_SINGING, -1);
        }
    }
}

BOOL daNpc_grO_c::chkAction(actionFunc action) {
    return mAction == action;
}

BOOL daNpc_grO_c::setAction(actionFunc action) {
    mMode = MODE_EXIT;

    if (mAction) {
        (this->*mAction)(NULL);
    }

    mMode = MODE_INIT;
    mAction = action;

    if (mAction) {
        (this->*mAction)(NULL);
    }

    return TRUE;
}

BOOL daNpc_grO_c::selectAction() {
    mNextAction = NULL;

    if (mpHIO->m.common.debug_mode_ON) {
        mNextAction = &daNpc_grO_c::test;
    } else {
        switch (mType) {
            case TYPE_BOKIN:
                mNextAction = &daNpc_grO_c::bokinWait;
                break;
            
            case TYPE_MARO:
                mNextAction = &daNpc_grO_c::waitMaro;
                break;

            default:
                mNextAction = &daNpc_grO_c::wait;
                break;
        }
        
    }

    return TRUE;
}

void daNpc_grO_c::doNormalAction(int param_1) {
    if (param_1 != 0 && hitChk2(&mCyl, TRUE, FALSE)) {
        int timer = mCutType == daPy_py_c::CUT_TYPE_TURN_RIGHT ? 20 : mpHIO->m.common.damage_time;
        setDamage(timer, 12, 0);
        setLookMode(LOOK_RESET);
    } else if (mIsDamaged && mDamageTimer == 0) {
        mMode = MODE_INIT;
        mIsDamaged = false;
    }

    if (field_0x9ef == 9) {
        mOrderEvtNo = EVT_PUSHOUT;
    } else {
        mOrderEvtNo = EVT_NONE;
    }

    if (mNextAction) {
        if (mAction == mNextAction) {
            (this->*mAction)(NULL);
        } else {
            setAction(mNextAction);
        }
    }
}

BOOL daNpc_grO_c::doEvent() {
    int evtCutNo = 0;
    BOOL rv = FALSE;

    if (dComIfGp_event_runCheck()) {
        dEvent_manager_c& eventManager = dComIfGp_getEventManager();

        if ((eventInfo.checkCommandTalk() || eventInfo.checkCommandDemoAccrpt()) && !mUnkFlag) {
            mOrderNewEvt = false;
        }

        if (eventInfo.checkCommandTalk()) {
            if (chkAction(&daNpc_grO_c::talk)) {
                (this->*mAction)(NULL);
            } else if (dComIfGp_event_chkTalkXY() == 0 || dComIfGp_evmng_ChkPresentEnd()) {
                setAction(&daNpc_grO_c::talk);
            }

            rv = TRUE;
        } else {
            if (mItemID != fpcM_ERROR_PROCESS_ID_e) {
                dComIfGp_event_setItemPartnerId(mItemID);
                mItemID = fpcM_ERROR_PROCESS_ID_e;
            }

            int staffId = eventManager.getMyStaffId(l_myName, NULL, 0);
            if (staffId != -1) {
                mStaffID = staffId;
                evtCutNo = eventManager.getMyActIdx(staffId, mEvtCutNameList, 3, 0, 0);

                JUT_ASSERT(1844, (0 <= evtCutNo) && (evtCutNo < NUM_EVT_CUTS_e));
                JUT_ASSERT(1845, NULL != mEvtCutList[evtCutNo]);

                if ((this->*mEvtCutList[evtCutNo])(staffId)) {
                    eventManager.cutEnd(staffId);
                }

                rv = TRUE;
            }

            if (eventInfo.checkCommandDemoAccrpt() && mEventIdx != -1 && eventManager.endCheck(mEventIdx) != 0) {
                switch (mOrderEvtNo) {
                    default:
                        break;
                }

                dComIfGp_event_reset();
                mOrderEvtNo = EVT_NONE;
                mEventIdx = -1;
            }
        }

        int i_expression, i_motion, i_msgTimer;
        i_msgTimer = mMsgTimer;

        if (ctrlMsgAnm(i_expression, i_motion, this, FALSE) != 0) {
            if (!field_0x9eb) {
                setExpression(i_expression, -1.0f);
                setMotion(i_motion, -1.0f, 1);

                if (chkAction(&daNpc_grO_c::talk)) {
                    if (mType == TYPE_BOKIN && mMotion == MOT_CALL) {
                        setLookMode(LOOK_NONE);
                    }
                }
            }
        } else if (i_msgTimer != 0 && !field_0x9eb) {
            setExpressionTalkAfter();
        }

        if (
            (mExpression == EXPR_CALL || mExpression == EXPR_BOW || mExpression == EXPR_BOKIN) &&
            mExpressionPhase == 0
        ) {
            mAnmFlags &= ~ANM_PAUSE_EXPRESSION;
        }
    } else {
        mMsgTimer = 0;

        if (mStaffID != -1) {
            mAction = NULL;
            mStaffID = -1;
        }
    }

    return rv;
}

void daNpc_grO_c::setLookMode(int i_lookMode) {
    if (i_lookMode >= 0 && i_lookMode < 5 && i_lookMode != mLookMode) {
        mLookMode = i_lookMode;
    }
}

void daNpc_grO_c::lookat() {
    daPy_py_c* player = NULL;
    J3DModel* model = mAnm_p->getModel();
    BOOL i_snap = FALSE;
    f32 body_angleX_min = mpHIO->m.common.body_angleX_min;
    f32 body_angleX_max = mpHIO->m.common.body_angleX_max;
    f32 body_angleY_min = mpHIO->m.common.body_angleY_min;
    f32 body_angleY_max = mpHIO->m.common.body_angleY_max;
    f32 head_angleX_min = mpHIO->m.common.head_angleX_min;
    f32 head_angleX_max = mpHIO->m.common.head_angleX_max;
    f32 head_angleY_min = mpHIO->m.common.head_angleY_min;
    f32 head_angleY_max = mpHIO->m.common.head_angleY_max;
    s16 angle_delta = mCurAngle.y - mOldAngle.y;
    cXyz lookatPos[3] = {mLookatPos[0], mLookatPos[1], mLookatPos[2]};
    csXyz* lookatAngle[3] = {&mLookatAngle[0], &mLookatAngle[1], &mLookatAngle[2]};
    cXyz spe8;

    switch (mLookMode) {
        case LOOK_NONE:
            break;

        case LOOK_RESET:
            i_snap = TRUE;
            break;

        case LOOK_PLAYER:
        case LOOK_PLAYER_TALK:
            player = daPy_getPlayerActorClass();

            if (mLookMode != LOOK_PLAYER_TALK) {
                break;
            }

            head_angleY_min = -80.0f;
            head_angleY_max = 80.0f;
            break;

        case LOOK_ACTOR:
            player = (daPy_py_c*)mActorMngrs[1].getActorP();
            break;
    }

    if (player != NULL) {
        mLookPos = player->attention_info.position;

        if (mLookMode != LOOK_PLAYER && mLookMode != LOOK_PLAYER_TALK && mLookMode != LOOK_ACTOR) {
            mLookPos.y -= 40.0f;
        }

        mLookat.setAttnPos(&mLookPos);
    } else {
        mLookat.setAttnPos(NULL);
    }
    
    mLookat.setParam(body_angleX_min, body_angleX_max, body_angleY_min, body_angleY_max, 0.0f, 0.0f, 0.0f, 0.0f, 
                     head_angleX_min, head_angleX_max, head_angleY_min, head_angleY_max, mCurAngle.y, lookatPos);
    mLookat.calc(this, model->getBaseTRMtx(), lookatAngle, i_snap, angle_delta, FALSE);
}

void daNpc_grO_c::setExpressionTalkAfter() {
    switch (mExpression) {
        case EXPR_TALK_A:
        case EXPR_TALK_B:
            setExpression(EXPR_WAIT_A, -1.0f);
            break;
        
        case EXPR_CALL:
        case EXPR_CALL_B:
            setExpression(EXPR_H_CALL_WAIT, -1.0f);
            break;

        case EXPR_BOW:
            break;
        
        case EXPR_BOKIN:
            setExpression(EXPR_H_BOKIN, -1.0f);
            break;

        default:
            setExpression(EXPR_NONE, -1.0f);
            
        case EXPR_SWING:
            break;
    }
}

int daNpc_grO_c::wait(void* param_1) {
    switch (mMode) {
        case MODE_INIT:
            setExpression(EXPR_NONE, -1.0f);
            setMotion(MOT_WAIT_A, -1.0f, 0);
            setLookMode(LOOK_NONE);
            mTurnMode = 0;
            mMode = MODE_RUN;
            // fallthrough
        case MODE_RUN:
            if (!mIsDamaged) {
                BOOL isActor = NULL != mActorMngrs[0].getActorP();
                BOOL findPlayer = chkFindPlayer2(isActor, shape_angle.y);
                if (findPlayer) {
                    if (!isActor) {
                        mActorMngrs[0].entry(daPy_getPlayerActorClass());
                        mTurnMode = 0;
                    }
                } else if (isActor) {
                    mActorMngrs[0].remove();
                    mTurnMode = 0;
                }

                if (mActorMngrs[0].getActorP() != NULL) {
                    setLookMode(LOOK_PLAYER);
                } else {
                    setLookMode(LOOK_NONE);

                    if (home.angle.y != mCurAngle.y && step(home.angle.y, 12, 8, 15)) {
                        mMode = MODE_INIT;
                    }
                }

                if (home.angle.y == mCurAngle.y) {
                    fopAc_ac_c* actor_p = getAttnActorP(NULL != mActorMngrs[0].getActorP(), srchAttnActor1, mpHIO->m.common.search_distance,
                                                        mpHIO->m.common.search_height, mpHIO->m.common.search_depth, mpHIO->m.common.fov, shape_angle.y, 120, 1);
                    if (actor_p != NULL) {
                        mActorMngrs[1].entry(actor_p);
                        setLookMode(LOOK_ACTOR);
                    }
                } else {
                    mAttnChangeTimer = 0;
                }
            }
            break;

        case MODE_EXIT:
            break;
    }

    return 1;
}

int daNpc_grO_c::bokinWait(void* param_1) {
    switch (mMode) {
        case MODE_INIT:
            setExpression(EXPR_WAIT_A, -1.0f);
            setMotion(MOT_BOKIN_WAIT, -1.0f, 0);
            setLookMode(LOOK_NONE);
            mTurnMode = 0;
            mMode = MODE_RUN;
            // fallthrough
        case MODE_RUN:
            if (!mIsDamaged) {
                BOOL isActor = NULL != mActorMngrs[0].getActorP();
                BOOL findPlayer = chkFindPlayer2(isActor, shape_angle.y);
                if (findPlayer) {
                    if (!isActor) {
                        mActorMngrs[0].entry(daPy_getPlayerActorClass());
                        mTurnMode = 0;
                    }
                } else if (isActor) {
                    mActorMngrs[0].remove();
                    mTurnMode = 0;
                }

                if (mActorMngrs[0].getActorP() != NULL) {
                    setLookMode(LOOK_PLAYER);
                } else {
                    setLookMode(LOOK_NONE);

                    if (home.angle.y != mCurAngle.y && step(home.angle.y, 12, 8, 15)) {
                        mMode = MODE_INIT;
                    }
                }

                if (home.angle.y == mCurAngle.y) {
                    fopAc_ac_c* actor_p = getAttnActorP(NULL != mActorMngrs[0].getActorP(), srchAttnActor1, mpHIO->m.common.search_distance,
                                                        mpHIO->m.common.search_height, mpHIO->m.common.search_depth, mpHIO->m.common.fov, shape_angle.y, 120, 1);
                    if (actor_p != NULL) {
                        mActorMngrs[1].entry(actor_p);
                        setLookMode(LOOK_ACTOR);
                    }
                } else {
                    mAttnChangeTimer = 0;
                }
            }
            break;

        case MODE_EXIT:
            break;
    }

    return 1;
}

int daNpc_grO_c::waitMaro(void* param_1) {
    switch (mMode) {
        case MODE_INIT:
            setExpression(EXPR_SWING, -1.0f);
            setMotion(MOT_SWING, -1.0f, 0);
            setLookMode(LOOK_NONE);
            mTurnMode = 0;
            field_0x9ea = true;
            mMode = MODE_RUN;
            // fallthrough
        case MODE_RUN:
            if (!mIsDamaged) {
                BOOL isActor = NULL != mActorMngrs[0].getActorP();
                BOOL findPlayer = chkFindPlayer2(isActor, shape_angle.y);
                if (findPlayer) {
                    if (!isActor) {
                        mActorMngrs[0].entry(daPy_getPlayerActorClass());
                        mTurnMode = 0;
                    }
                } else if (isActor) {
                    mActorMngrs[0].remove();
                    mTurnMode = 0;
                }

                if (mActorMngrs[0].getActorP() != NULL) {
                    setLookMode(LOOK_PLAYER);
                } else {
                    setLookMode(LOOK_NONE);

                    if (home.angle.y != mCurAngle.y && step(home.angle.y, 12, 8, 15)) {
                        mMode = MODE_INIT;
                    }
                }
            }
            break;

        case MODE_EXIT:
            break;
    }

    return 1;
}

int daNpc_grO_c::talk(void* param_1) {
    int unused = 0;
    int rv = 0;
    int msgNo = 0;
    BOOL facePlayerFlag = FALSE;
    int itemId = 0;

    switch (mMode) {
        case MODE_INIT:
            if (mIsDamaged) {
                break;
            }

            if (
                // Goron Mines
                strcmp(dComIfGp_getStartStageName(), "D_MN04") == 0 &&
                mUnkFlag == 0
            ) {
                daNpcF_offTmpBit(11);
            }

            msgNo = mMsgNo;
            mUnkFlag = 0;
            initTalk(msgNo, NULL);
            mTurnMode = 0;
            mMode = MODE_RUN;
            // fallthrough
        case MODE_RUN:
            if (field_0x9ea) {
                facePlayerFlag = TRUE;
            } else if (mType == TYPE_BOKIN) {
                facePlayerFlag = TRUE;

                if (mMotion == MOT_CALL) {
                    setLookMode(LOOK_NONE);
                } else {
                    setLookMode(LOOK_PLAYER);
                    mActorMngrs[0].entry(daPy_getPlayerActorClass());
                }
            } else {
                setLookMode(LOOK_PLAYER_TALK);
                mActorMngrs[0].entry(daPy_getPlayerActorClass());

                if (mCurAngle.y == fopAcM_searchPlayerAngleY(this)) {
                    facePlayerFlag = TRUE;
                } else if (step(fopAcM_searchPlayerAngleY(this), 12, 8, 15)) {
                    setExpression(EXPR_NONE, -1.0f);
                    setMotion(MOT_WAIT_A, -1.0f, 0);
                    mTurnMode = 0;
                }
            }

            if (facePlayerFlag && talkProc(NULL, TRUE, NULL)) {
                if (mType == TYPE_MINES) {
                    if (mFlow.getEventId(&itemId) == 1) {
                        mItemID = fopAcM_createItemForPresentDemo(&current.pos, itemId, 0, -1, -1, NULL, NULL);
                        if (mItemID != fpcM_ERROR_PROCESS_ID_e) {
                            s16 eventIdx = dComIfGp_getEventManager().getEventIdx(this, "DEFAULT_GETITEM", 0xFF);
                            dComIfGp_getEvent()->reset(this);
                            fopAcM_orderChangeEventId(this, eventIdx, 1, 0xFFFF);
                            mUnkFlag = 1;
                            field_0x9ec = true;
                            mOrderNewEvt = true;
                        }
                    }
                } else if (
                    mType == TYPE_BOKIN &&
                    // dSv_event_tmp_flag_c::T_0081 - Kakariko Village - For use in fundraiser event completion cutscene
                    daNpcF_chkTmpBit(81)
                ) {
                    dComIfGp_getEvent()->reset(this);
                    field_0x9ec = true;
                    mOrderNewEvt = true;
                    mOrderEvtNo = EVT_BOKIN_FINISH;
                    changeEvent(l_resNames[l_evtGetParamList[mOrderEvtNo].arcIdx], l_evtNames[l_evtGetParamList[mOrderEvtNo].fileIdx], 2, 0xFFFF);
                }

                rv = 1;
            }

            if (rv) {
                mMode = MODE_EXIT;

                if (!field_0x9ec) {
                    dComIfGp_event_reset();
                }

                field_0x9ec = false;
            }
            break;

        case MODE_EXIT:
            break;
    }

    return rv;
}

static void* s_sub(void* i_actor, void* i_data) {
    if (fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_NPC_MARO) {
        return i_actor;
    }

    return NULL;
}

int daNpc_grO_c::ECut_bokinFinish(int i_staffId) {
    dEvent_manager_c& eventManager = dComIfGp_getEventManager();
    int unused = 0;
    daPy_py_c* player = daPy_getPlayerActorClass();
    int rv = 0;
    int* piVar1 = NULL;
    int prm = -1;
    int unused2 = 0;
    fopAc_ac_c* actors[2] = {this, mActorMngrs[2].getActorP()};

    piVar1 = dComIfGp_evmng_getMyIntegerP(i_staffId, "prm");
    if (piVar1 != NULL) {
        prm = *piVar1;
    }

    if (eventManager.getIsAddvance(i_staffId)) {
        switch (prm) {
            case 0:
                daNpcF_offTmpBit(11);
                daNpcF_offTmpBit(12);
                setLookMode(LOOK_PLAYER);
                break;
            
            case 0xA:
                initTalk(mMsgNo, actors);
                break;
            
            case 0x14:
                initTalk(mMsgNo, actors);
                break;

            case 0x1E:
                initTalk(mMsgNo, actors);
                setLookMode(LOOK_PLAYER);
                break;
        }
    }

    switch (prm) {
        case 0: {
            if (mActorMngrs[2].getActorP() == NULL) {
                fopAc_ac_c* actor_p = (fopAc_ac_c*)fpcM_Search(s_sub, this);
                if (actor_p != NULL) {
                    mActorMngrs[2].entry(actor_p);
                    rv = 1;
                }
            } else {
                rv = 1;
            }
            break;
        }
        
        case 0xA:
            if (talkProc(NULL, FALSE, actors)) {
                rv = 1;
            }
            break;
        
        case 0x14: {
            if (talkProc(NULL, FALSE, actors)) {
                rv = 1;
            }

            setLookMode(LOOK_PLAYER);
            break;
        }

        case 0x1E:
            if (talkProc(NULL, FALSE, actors)) {
                rv = 1;
            }

            setLookMode(LOOK_PLAYER);
            break;

        default:
            rv = 1;
            break;
    }

    return rv;
}

int daNpc_grO_c::cutPushOut(int i_staffId) {
    daTag_Push_c* actor_p = NULL;
    int rv = 0;
    int* piVar1 = NULL;
    int prm = -1;

    piVar1 = dComIfGp_evmng_getMyIntegerP(i_staffId, "prm");
    if (piVar1 != NULL) {
        prm = *piVar1;
    }

    if (dComIfGp_getEventManager().getIsAddvance(i_staffId)) {
        switch (prm) {
            case 0: {
                actor_p = (daTag_Push_c*)field_0x824.getActorP();
                JUT_ASSERT(2669, NULL != actor_p);
                actor_p->pushBackPlayer(0);
                break;
            }
            
            case 1:
                setExpression(EXPR_NONE, 0.0f);
                setMotion(MOT_WAIT_A, 0.0f, 1);
                setLookMode(LOOK_RESET);
                initTalk(mFlowNodeNo, NULL);
                setAngle(fopAcM_searchPlayerAngleY(this));
                break;
        }
    }

    switch (prm) {
        case 0:
            rv = 1;
            break;
        
        case 1:
            if (talkProc(NULL, TRUE, NULL)) {
                rv = 1;
            }
            break;
    }

    return rv;
}

int daNpc_grO_c::test(void* param_1) {
    switch (mMode) {
        case MODE_INIT:
            speedF = 0.0f;
            speed.setall(0.0f);
            mMode = MODE_RUN;
            // fallthrough
        case MODE_RUN:
            if (mpHIO->m.common.face_expression != mExpression) {
                setExpression(mpHIO->m.common.face_expression, mpHIO->m.common.morf_frame);
            }

            setMotion(mpHIO->m.common.motion, mpHIO->m.common.morf_frame, 0);
            setLookMode(mpHIO->m.common.look_mode);
            mOrderEvtNo = EVT_NONE;
            attention_info.flags = 0;
            break;

        case MODE_EXIT:
            break;
    }

    return 1;
}

static int daNpc_grO_Create(void* i_this) {
    return static_cast<daNpc_grO_c*>(i_this)->create();
}

static int daNpc_grO_Delete(void* i_this) {
    return static_cast<daNpc_grO_c*>(i_this)->Delete();
}

static int daNpc_grO_Execute(void* i_this) {
    return static_cast<daNpc_grO_c*>(i_this)->Execute();
}

static int daNpc_grO_Draw(void* i_this) {
    return static_cast<daNpc_grO_c*>(i_this)->Draw();
}

static int daNpc_grO_IsDelete(void* i_this) {
    return 1;
}

AUDIO_INSTANCES;

static actor_method_class daNpc_grO_MethodTable = {
    (process_method_func)daNpc_grO_Create,
    (process_method_func)daNpc_grO_Delete,
    (process_method_func)daNpc_grO_Execute,
    (process_method_func)daNpc_grO_IsDelete,
    (process_method_func)daNpc_grO_Draw,
};

actor_process_profile_definition g_profile_NPC_GRO = {
  fpcLy_CURRENT_e,        // mLayerID
  7,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_NPC_GRO,           // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(daNpc_grO_c),    // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  311,                    // mPriority
  &daNpc_grO_MethodTable, // sub_method
  0x00044100,             // mStatus
  fopAc_NPC_e,            // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};
