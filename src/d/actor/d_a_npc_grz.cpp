/**
 * @file d_a_npc_grz.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_npc_grz.h"
#include "d/actor/d_a_npc.h"
#include "d/actor/d_a_obj_grz_rock.h"
#include "Z2AudioLib/Z2Instances.h"

enum grZ_RES_File_ID {
    /* BCK */
    /* 0x06 */ BCK_GRZ_F_MUKIDASHI = 0x6,
    /* 0x07 */ BCK_GRZ_F_TALK_A,
    /* 0x08 */ BCK_GRZ_FH_TALK_A,
    /* 0x09 */ BCK_GRZ_STEP,
    /* 0x0A */ BCK_GRZ_TALK_A,
    /* 0x0B */ BCK_GRZ_WAIT_A,

    /* BMDR */
    /* 0x0E */ BMDR_GRZ = 0xE,

    /* BTK */
    /* 0x11 */ BTK_GRZ = 0x11,

    /* BTP */
    /* 0x14 */ BTP_GRZ = 0x14,
};

enum grZ1_RES_File_ID {
    /* BCK */
    /* 0x05 */ BCK_GRZ_F_GETUP = 0x5,
    /* 0x06 */ BCK_GRZ_F_LIEDOWN,
    /* 0x07 */ BCK_GRZ_F_WEAK_TALK,
    /* 0x08 */ BCK_GRZ_F_WEAK_TURN,
    /* 0x09 */ BCK_GRZ_F_WEAK_WAIT,
    /* 0x0A */ BCK_GRZ_GETUP,
    /* 0x0B */ BCK_GRZ_LIEDOWN,
    /* 0x0C */ BCK_GRZ_WEAK_STEP,
    /* 0x0D */ BCK_GRZ_WEAK_TURN,
    /* 0x0E */ BCK_GRZ_WEAK_WAIT,

    /* BTK */
    /* 0x11 */ BTK_GRZ_GETUP = 0x11,

    /* BTP */
    /* 0x14 */ BTP_GRZ_F_GETUP = 0x14,
    /* 0x15 */ BTP_GRZ_F_LIEDOWN,
    /* 0x16 */ BTP_GRZ_F_WEAK_TALK,
    /* 0x17 */ BTP_GRZ_F_WEAK_TURN,
    /* 0x18 */ BTP_GRZ_F_WEAK_WAIT,
};

enum grZ2_RES_File_ID {
    /* BCK */
    /* 0x05 */ BCK_GRZ_F_LAUGH = 0x5,
    /* 0x06 */ BCK_GRZ_F_SNIFF,
    /* 0x07 */ BCK_GRZ_FH_LAUGH,
    /* 0x08 */ BCK_GRZ_LAUGH,
    /* 0x09 */ BCK_GRZ_SNIFF,
    /* 0x0A */ BCK_GRZ_UNAZUKI,
    /* 0x0B */ BCK_GRZ_WAIT_B,
    /* 0x0C */ BCK_GRZ_WALK_A,

    /* BTK */
    /* 0x0F */ BTK_GRZ_SNIFF = 0xF,

    /* BTP */
    /* 0x12 */ BTP_GRZ_F_LAUGH = 0x12,
    /* 0x13 */ BTP_GRZ_F_SNIFF,
    /* 0x14 */ BTP_GRZ_FH_LAUGH,
};

enum grZ3_RES_File_ID {
    /* BCK */
    /* 0x4 */ BCK_GRZ_F_PUNCH = 0x4,
    /* 0x5 */ BCK_GRZ_F_PUNCHING,
    /* 0x6 */ BCK_GRZ_F_RECOVER,
    /* 0x7 */ BCK_GRZ_PUNCH,
    /* 0x8 */ BCK_GRZ_PUNCHING,
    /* 0x9 */ BCK_GRZ_RECOVER,
    /* 0xA */ BCK_GRZ_ROTATE,
    /* 0xB */ BCK_GRZ_TO_STONE_NORMAL,

    /* BTP */
    /* 0xE */ BTP_GRZ_F_RECOVER = 0xE,
};

enum grZD1_RES_File_ID {
    /* EVT */
    /* 0x3 */ EVT_GRZD1_EVENT_LIST = 0x3,
};

enum grZD3_RES_File_ID {
    /* EVT */
    /* 0x3 */ EVT_GRZD3_EVENT_LIST = 0x3,
};

enum RES_Names {
    /* 0x0 */ GRZ,
    /* 0x1 */ GRZ1,
    /* 0x2 */ GRZ2,
    /* 0x3 */ GRZ3,
    /* 0x4 */ GRZD1,
    /* 0x5 */ GRZD3,
};

enum Joint {
    /* 0x00 */ JNT_CENTER,
    /* 0x01 */ JNT_BACKBONE1,
    /* 0x02 */ JNT_BACKBONE2,
    /* 0x03 */ JNT_NECK,
    /* 0x04 */ JNT_HEAD,
    /* 0x05 */ JNT_CHIN,
    /* 0x06 */ JNT_LIP2,
    /* 0x07 */ JNT_LIP1,
    /* 0x08 */ JNT_MAYUL,
    /* 0x09 */ JNT_MAYUR,
    /* 0x0A */ JNT_MOUTH,
    /* 0x0B */ JNT_SHOULDERL,
    /* 0x0C */ JNT_ARML1,
    /* 0x0D */ JNT_ARML2,
    /* 0x0E */ JNT_HANDL,
    /* 0x0F */ JNT_FINGERL1,
    /* 0x10 */ JNT_THAMBL,
    /* 0x11 */ JNT_SHOULDERR,
    /* 0x12 */ JNT_ARMR1,
    /* 0x13 */ JNT_ARMR2,
    /* 0x14 */ JNT_HANDR,
    /* 0x15 */ JNT_FINGERR1,
    /* 0x16 */ JNT_THAMBR,
    /* 0x17 */ JNT_WAIST,
    /* 0x18 */ JNT_LEGL1,
    /* 0x19 */ JNT_LEGL2,
    /* 0x1A */ JNT_FOOTL,
    /* 0x1B */ JNT_LEGR1,
    /* 0x1C */ JNT_LEGR2,
    /* 0x1D */ JNT_FOOTR,
    /* 0x1E */ JNT_SKIRT,
};

enum Event {
    /* 0x0 */ EVT_NONE,
    /* 0x1 */ EVT_TALK_STAND,
    /* 0x2 */ EVT_STONE_SMASH,
    /* 0x3 */ EVT_STONE_SMASH_SKIP,
};

enum Animation {
    /* 0x00 */ ANM_NONE,
    /* 0x01 */ ANM_F_MUKIDASHI,
    /* 0x02 */ ANM_F_RECOVER,
    /* 0x03 */ ANM_FH_TALK_A,
    /* 0x04 */ ANM_F_TALK_A,
    /* 0x05 */ ANM_LIEDOWN,
    /* 0x06 */ ANM_GETUP,
    /* 0x07 */ ANM_F_WEAK_WAIT,
    /* 0x08 */ ANM_F_WEAK_TURN,
    /* 0x09 */ ANM_F_WEAK_TALK,
    /* 0x0A */ ANM_F_SNIFF,
    /* 0x0B */ ANM_F_LAUGH,
    /* 0x0C */ ANM_FH_LAUGH,
    /* 0x0D */ ANM_F_PUNCHING,
    /* 0x0E */ ANM_F_PUNCH,
    /* 0x0F */ ANM_WAIT_A,
    /* 0x10 */ ANM_TALK_A,
    /* 0x11 */ ANM_TO_STONE_NORMAL,
    /* 0x12 */ ANM_UNK_18,
    /* 0x13 */ ANM_RECOVER,
    /* 0x14 */ ANM_ROTATE,
    /* 0x15 */ ANM_STEP,
    /* 0x16 */ ANM_LIEDOWN_2,
    /* 0x17 */ ANM_GETUP_2,
    /* 0x18 */ ANM_WEAK_WAIT,
    /* 0x19 */ ANM_WEAK_TURN,
    /* 0x1A */ ANM_WEAK_STEP,
    /* 0x1B */ ANM_WAIT_B,
    /* 0x1C */ ANM_SNIFF,
    /* 0x1D */ ANM_LAUGH,
    /* 0x1E */ ANM_UNAZUKI,
    /* 0x1F */ ANM_WALK_A,
    /* 0x20 */ ANM_PUNCHING,
    /* 0x21 */ ANM_PUNCH,
};

enum Expression_BTP {
    /* 0x0 */ EXPR_BTP_GRZ,
    /* 0x1 */ EXPR_BTP_F_LIEDOWN,
    /* 0x2 */ EXPR_BTP_F_GETUP,
    /* 0x3 */ EXPR_BTP_F_WEAK_WAIT,
    /* 0x4 */ EXPR_BTP_F_WEAK_TURN,
    /* 0x5 */ EXPR_BTP_F_WEAK_TALK,
    /* 0x6 */ EXPR_BTP_F_SNIFF,
    /* 0x7 */ EXPR_BTP_F_LAUGH,
    /* 0x8 */ EXPR_BTP_FH_LAUGH,
    /* 0x9 */ EXPR_BTP_F_RECOVER,
};

enum Expression {
    /* 0x0 */ EXPR_TALK_A,
    /* 0x1 */ EXPR_WEAK_TALK,
    /* 0x2 */ EXPR_SNIFF,
    /* 0x3 */ EXPR_LAUGH,
    /* 0x4 */ EXPR_H_TALK_A,
    /* 0x5 */ EXPR_PUNCHING,
    /* 0x6 */ EXPR_PUNCH,
    /* 0x7 */ EXPR_LIEDOWN,
    /* 0x8 */ EXPR_GETUP,
    /* 0x9 */ EXPR_WEAK_WAIT,
    /* 0xA */ EXPR_NONE,
};

enum Motion {
    /* 0x0 */ MOT_WAIT_A,
    /* 0x1 */ MOT_WEAK_WAIT,
    /* 0x2 */ MOT_WEAK_TURN,
    /* 0x3 */ MOT_WAIT_B,
    /* 0x4 */ MOT_SNIFF,
    /* 0x5 */ MOT_LAUGH,
    /* 0x6 */ MOT_PUNCHING,
    /* 0x7 */ MOT_PUNCH,
    /* 0x8 */ MOT_TO_STONE_NORMAL,
    /* 0x9 */ MOT_UNAZUKI,
    /* 0xA */ MOT_WALK_A,
    /* 0xB */ MOT_LIEDOWN,
    /* 0xC */ MOT_GETUP,
    /* 0xD */ MOT_STEP,
    /* 0xE */ MOT_WEAK_STEP,
};

enum Type {
    /* 0x0 */ TYPE_YELIA,
    /* 0x1 */ TYPE_LIEDOWN,
    /* 0x2 */ TYPE_SMASH,
    /* 0x3 */ TYPE_3,
};

enum Event_Cut_Nums {
    /* 0x7 */ NUM_EVT_CUTS_e = 0x7,
};

static daNpc_Grz_Param_c l_HIO;

static int l_bmdGetParamList[1][2] = {
    {BMDR_GRZ, GRZ},
};

static daNpc_GetParam1 l_bckGetParamList[34] = {
    {-1, GRZ},
    {BCK_GRZ_F_MUKIDASHI, GRZ},
    {BCK_GRZ_F_RECOVER, GRZ3},
    {BCK_GRZ_FH_TALK_A, GRZ},
    {BCK_GRZ_F_TALK_A, GRZ},
    {BCK_GRZ_LIEDOWN, GRZ1},
    {BCK_GRZ_GETUP, GRZ1},
    {BCK_GRZ_F_WEAK_WAIT, GRZ1},
    {BCK_GRZ_F_WEAK_TURN, GRZ1},
    {BCK_GRZ_F_WEAK_TALK, GRZ1},
    {BCK_GRZ_F_SNIFF, GRZ2},
    {BCK_GRZ_F_LAUGH, GRZ2},
    {BCK_GRZ_FH_LAUGH, GRZ2},
    {BCK_GRZ_F_PUNCHING, GRZ3},
    {BCK_GRZ_F_PUNCH, GRZ3},
    {BCK_GRZ_WAIT_A, GRZ},
    {BCK_GRZ_TALK_A, GRZ},
    {BCK_GRZ_TO_STONE_NORMAL, GRZ3},
    {-1, GRZ},
    {BCK_GRZ_RECOVER, GRZ3},
    {BCK_GRZ_ROTATE, GRZ3},
    {BCK_GRZ_STEP, GRZ},
    {BCK_GRZ_LIEDOWN, GRZ1},
    {BCK_GRZ_GETUP, GRZ1},
    {BCK_GRZ_WEAK_WAIT, GRZ1},
    {BCK_GRZ_WEAK_TURN, GRZ1},
    {BCK_GRZ_WEAK_STEP, GRZ1},
    {BCK_GRZ_WAIT_B, GRZ2},
    {BCK_GRZ_SNIFF, GRZ2},
    {BCK_GRZ_LAUGH, GRZ2},
    {BCK_GRZ_UNAZUKI, GRZ2},
    {BCK_GRZ_WALK_A, GRZ2},
    {BCK_GRZ_PUNCHING, GRZ3},
    {BCK_GRZ_PUNCH, GRZ3},
};

static daNpc_GetParam1 l_btpGetParamList[10] = {
    {BTP_GRZ, GRZ},
    {BTP_GRZ_F_LIEDOWN, GRZ1},
    {BTP_GRZ_F_GETUP, GRZ1},
    {BTP_GRZ_F_WEAK_WAIT, GRZ1},
    {BTP_GRZ_F_WEAK_TURN, GRZ1},
    {BTP_GRZ_F_WEAK_TALK, GRZ1},
    {BTP_GRZ_F_SNIFF, GRZ2},
    {BTP_GRZ_F_LAUGH, GRZ2},
    {BTP_GRZ_FH_LAUGH, GRZ2},
    {BTP_GRZ_F_RECOVER, GRZ3}, 
};

static daNpc_GetParam1 l_btkGetParamList[3] = {
    {BTK_GRZ, GRZ},
    {BTK_GRZ_GETUP, GRZ1},
    {BTK_GRZ_SNIFF, GRZ2},
};

static daNpc_GetParam1 l_evtGetParamList[4] = {
    {0, GRZ},
    {1, GRZD1},
    {2, GRZD3},
    {3, GRZD3},
};

static char* l_evtNames[4] = {
    NULL,
    "TALK_STAND",
    "STONE_SMASH",
    "STONE_SMASH_SKIP",
};

static int l_loadRes_YELIA[5] = {
    GRZ, GRZ2, -1, -1, -1,
};

static int l_loadRes_LieDown[5] = {
    GRZ, GRZ1, GRZD1, -1, -1,
};

static int l_loadRes_Smash[5] = {
    GRZ, GRZ2, GRZ3, GRZD3, -1,
};

static int l_loadRes_GRZa[5] = {
    GRZ, GRZ2, -1, -1, -1,
};

static int l_loadRes_GRZ0[5] = {
    GRZ, -1, -1, -1, -1,
};

static int* l_loadRes_list[5] = {
    l_loadRes_YELIA, l_loadRes_LieDown, l_loadRes_Smash,
    l_loadRes_GRZa,  l_loadRes_GRZ0,
};

static char* l_resNames[6] = {
    "grZ",
    "grZ1",
    "grZ2",
    "grZ3",
    "grZD1",
    "grZD3",
};

static char* l_myName = "grZ";

char* daNpc_Grz_c::mEvtCutNameList[7] = {
    "",
    "WAIT",
    "REBIRTH",
    "TALK_STAND",
    "GO_OUT",
    "STONE_SMASH",
    "STONE_SMASH_SKIP",
};

daNpc_Grz_c::cutFunc daNpc_Grz_c::mEvtCutList[7] = {
    NULL,
    &daNpc_Grz_c::doWaitCut,
    &daNpc_Grz_c::doRebirthCut,
    &daNpc_Grz_c::doTalkStandCut,
    &daNpc_Grz_c::doGoOutCut,
    &daNpc_Grz_c::doStoneSmashCut,
    &daNpc_Grz_c::doStoneSmashSkipCut,
};

daNpc_Grz_c::daNpc_Grz_c() {}

daNpc_Grz_c::~daNpc_Grz_c() {
    for (int i = 0; l_loadRes_list[mType][i] >= 0; i++) {
        dComIfG_resDelete(&mPhases[i], l_resNames[l_loadRes_list[mType][i]]);
    }

    if (heap != NULL) {
        mAnm_p->stopZelAnime();
    }
}

daNpc_Grz_HIOParam const daNpc_Grz_Param_c::m = {
    130.0f,
    -3.0f,
    1.0f,
    1200.0f,
    255.0f,
    300.0f,
    35.0f,
    110.0f,
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
    7,
    6,
    9,
    6,
    80.0f,
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
    60,
    -40,
    -40,
    -40,
    11.0f,
    35.0f,
    500.0f,
};

cPhs__Step daNpc_Grz_c::create() {
    fopAcM_ct(this, daNpc_Grz_c);

    mType = getTypeFromParam();
    mSwNo = fopAcM_GetParam(this) >> 16;

    if (home.angle.x != 0xFFFF) {
        mMsgNo = home.angle.x;
    } else {
        mMsgNo = -1;
    }

    if (isDelete()) {
        return cPhs_ERROR_e;
    }

    int iVar1 = 0;
    int i = 0;
    for (; l_loadRes_list[mType][i] >= 0; i++) {
        cPhs__Step phase = (cPhs__Step)dComIfG_resLoad(&mPhases[i], l_resNames[l_loadRes_list[mType][i]]);
        if (phase == cPhs_ERROR_e || phase == cPhs_UNK3_e) {
            return cPhs_ERROR_e;
        }

        if (phase == cPhs_COMPLEATE_e) {
            iVar1++;
        }
    }

    if (iVar1 == i) {
        static int const heapSize[5] = {
            0x3AE0, 0x3AF0, 0x3AE0, 0x3AF0, 0x0,
        };

        if (!fopAcM_entrySolidHeap(this, createHeapCallBack, heapSize[mType])) {
            return cPhs_ERROR_e;
        }

        fopAcM_SetMtx(this, mAnm_p->getModel()->getBaseTRMtx());
        fopAcM_setCullSizeBox(this, -300.0f, -50.0f, -300.0f, 300.0f, 450.0f, 300.0f);
        
        mSound.init(&current.pos, &eyePos, 3, 1);
        
        mAcchCir.SetWall(daNpc_Grz_Param_c::m.common.width, daNpc_Grz_Param_c::m.common.knee_length);
        mAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1, &mAcchCir, fopAcM_GetSpeed_p(this),
                    fopAcM_GetAngle_p(this), fopAcM_GetShapeAngle_p(this));
        mCcStts.Init(daNpc_Grz_Param_c::m.common.weight, 0, this);
        resetCol();
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

int daNpc_Grz_c::CreateHeap() {
    J3DModelData* mdlData_p = NULL;

    if (l_bmdGetParamList[0][0] >= 0) {
        mdlData_p = (J3DModelData*)dComIfG_getObjectRes(l_resNames[l_bmdGetParamList[0][1]], l_bmdGetParamList[0][0]);
    }

    JUT_ASSERT(556, NULL != mdlData_p);

    mAnm_p = new mDoExt_McaMorfSO(mdlData_p, NULL, NULL, NULL, -1, 1.0f, 0, -1, &mSound, 0x80000, 0x11020284);
    if (mAnm_p != NULL && mAnm_p->getModel() == NULL) {
        mAnm_p->stopZelAnime();
        mAnm_p = NULL;
    }

    if (mAnm_p == NULL) {
        return 0;
    }

    J3DModel* model = mAnm_p->getModel();
    for (u16 i = 0; i < mdlData_p->getJointNum(); i++) {
        mdlData_p->getJointNodePointer(i)->setCallBack(ctrlJointCallBack);
    }
    model->setUserArea((uintptr_t)this);

    mpMatAnm = new daNpcF_MatAnm_c();
    if (mpMatAnm == NULL) {
        return 0;
    }

    if (!setExpressionAnm(ANM_F_MUKIDASHI, FALSE)) {
        return 0;
    }

    setMotionAnm(ANM_WAIT_A, 0.0f);
    return 1;
}

int daNpc_Grz_c::Delete() {
    this->~daNpc_Grz_c();
    return 1;
}

int daNpc_Grz_c::Execute() {
    int rv = execute();
    setPrtcl();
    return rv;
}

int daNpc_Grz_c::Draw() {
    J3DModel* model = mAnm_p->getModel();
    J3DModelData* mdlData_p = mAnm_p->getModel()->getModelData();

    if (checkHide()) {
        return 1;
    }

    g_env_light.settingTevStruct(0, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(model, &tevStr);

    for (u16 i = 0; i < mdlData_p->getMaterialNum(); i++) {
        J3DMaterial* matNode_p = mdlData_p->getMaterialNodePointer(i);
        J3DGXColorS10* color = matNode_p->getTevColor(0);

        if (i != 1) {
            color->r = mColor.r;
            color->g = mColor.g;
            color->b = mColor.b;
        } else {
            matNode_p->setMaterialAnm(mpMatAnm);
        }
    }

    if (drawDbgInfo() == 0) {
        if ((mAnmFlags & ANM_PLAY_BTP) != 0) {
            mBtpAnm.entry(mdlData_p);
        }

        if ((mAnmFlags & ANM_PLAY_BTK) != 0) {
            mBtkAnm.entry(mdlData_p);
        }

        if ((mAnmFlags & ANM_PLAY_BRK) != 0) {
            mBrkAnm.entry(mdlData_p);
        }

        mAnm_p->entryDL();

        if ((mAnmFlags & ANM_PLAY_BTP) != 0) {
            mBtpAnm.remove(mdlData_p);
        }

        if ((mAnmFlags & ANM_PLAY_BTK) != 0) {
            mBtkAnm.remove(mdlData_p);
        }

        if ((mAnmFlags & ANM_PLAY_BRK) != 0) {
            mBrkAnm.remove(mdlData_p);
        }

        mShadowKey = dComIfGd_setShadow(mShadowKey, 1, model, &current.pos, daNpc_Grz_Param_c::m.common.real_shadow_size,
                                        20.0f, current.pos.y, mGroundH, mGndChk, &tevStr,
                                        0, 1.0f, dDlst_shadowControl_c::getSimpleTex());
        drawOtherMdls();
    }

    return 1;
}

int daNpc_Grz_c::ctrlJoint(J3DJoint* i_joint, J3DModel* i_model) {
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
            setLookatMtx(jntNo, i_jointList, daNpc_Grz_Param_c::m.common.neck_rotation_ratio);
            break;
    }

    if (jntNo == JNT_BACKBONE1) {
        mDoMtx_stack_c::YrotM(-field_0x908[0].z);
        mDoMtx_stack_c::ZrotM(field_0x908[0].x);
    } else if (jntNo == JNT_HEAD) {
        mDoMtx_stack_c::YrotM(-field_0x908[2].z);
        mDoMtx_stack_c::ZrotM(field_0x908[2].x);
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

int daNpc_Grz_c::createHeapCallBack(fopAc_ac_c* a_this) {
    return static_cast<daNpc_Grz_c*>(a_this)->CreateHeap();
}

int daNpc_Grz_c::ctrlJointCallBack(J3DJoint* i_joint, int param_2) {
    if (param_2 == 0) {
        J3DModel* model = j3dSys.getModel();
        daNpc_Grz_c* i_this = (daNpc_Grz_c*)model->getUserArea();

        if (i_this != NULL) {
            i_this->ctrlJoint(i_joint, model);
        }
    }

    return 1;
}

void daNpc_Grz_c::setParam() {
    actionFunc action = mNextAction;
    u32 flags = fopAc_AttnFlag_SPEAK_e | fopAc_AttnFlag_TALK_e;
    selectAction();

    if (!mTwilight && daPy_py_c::checkNowWolf()) {
        flags = 0;
    }

    if (mNextAction != action) {
        for (int i = 4; i < 4; i++) {
            mActorMngrs[i].initialize();
        }
    }

    field_0x1a64 = 0;
    field_0x1a68 = 0;
    attention_info.distances[fopAc_attn_LOCK_e] = 67;
    attention_info.distances[fopAc_attn_TALK_e] = 67;
    attention_info.distances[fopAc_attn_SPEAK_e] = 66;

    switch (mType & 0xFF) {
        case TYPE_SMASH:
            if (daPy_py_c::checkNowWolf()) {
                attention_info.flags = 0;
            } else {
                attention_info.flags = flags;
            }
            break;

        default:
            attention_info.flags = flags;
            break;
    }

    scale.set(daNpc_Grz_Param_c::m.common.scale, daNpc_Grz_Param_c::m.common.scale, daNpc_Grz_Param_c::m.common.scale);
    mAcchCir.SetWallR(daNpc_Grz_Param_c::m.common.width);
    mAcchCir.SetWallH(daNpc_Grz_Param_c::m.common.knee_length);
    gravity = daNpc_Grz_Param_c::m.common.gravity;
}

BOOL daNpc_Grz_c::main() {
    if (!doEvent()) {
        doNormalAction(1);
    }

    if (checkHide()) {
        attention_info.flags = 0;
    }

    if (!daNpc_Grz_Param_c::m.common.debug_mode_ON) {
        if (!dComIfGp_event_runCheck() || mOrderNewEvt && dComIfGp_getEvent()->isOrderOK()) {
            if (mOrderEvtNo != EVT_NONE) {
                eventInfo.setArchiveName(l_resNames[l_evtGetParamList[mOrderEvtNo].arcIdx]);
            }

            orderEvent(mOrderSpeakEvt, l_evtNames[l_evtGetParamList[mOrderEvtNo].fileIdx], 0xFFFF, 40, 0xFF, 1);
        }
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

BOOL daNpc_Grz_c::ctrlBtk() {
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

void daNpc_Grz_c::setAttnPos() {
    static cXyz eyeOffset(20.0f, 40.0f, 0.0f);

    cXyz sp80, sp8c, sp98, spa4;
    f32 attention_offset = daNpc_Grz_Param_c::m.common.attention_offset;
    f32 fVar2 = 0.0f;
    f32 fVar3 = 0.0f;

    mDoMtx_stack_c::YrotS(field_0x990);
    cLib_addCalc2(&field_0x984[0], 0.0f, 0.1, 125.0f);
    cLib_addCalc2(&field_0x984[2], 0.0f, 0.1f, 125.0f);

    for (int i = 0; i < 3; i++) {
        sp98.set(0.0f, 0.0f, field_0x984[i] * cM_ssin(field_0x992));
        mDoMtx_stack_c::multVec(&sp98, &spa4);
        field_0x908[i].x = -spa4.z;
        field_0x908[i].z = -spa4.x;
    }

    cLib_chaseS(&field_0x992, 0, 0x555);

    if (mLookMode == 1) {
        for (int i = 0; i < 3; i++) {
            mLookatAngle[i].setall(0);
        }
    }

    setMtx();
    lookat();
    mDoMtx_stack_c::copy(mAnm_p->getModel()->getAnmMtx(JNT_HEAD));
    mDoMtx_stack_c::multVecZero(&mHeadPos);
    mDoMtx_stack_c::multVec(&eyeOffset, &eyePos);
    eyeOffset.x = 0.0f;
    mDoMtx_stack_c::multVec(&eyeOffset, &sp98);
    mHeadAngle.x = cLib_targetAngleX(&mHeadPos, &sp98);
    mHeadAngle.y = cLib_targetAngleY(&mHeadPos, &sp98);

    cXyz* attnPos = mLookat.getAttnPos();
    if (attnPos != NULL) {
        sp8c = *attnPos - eyePos;
        mEyeAngle.y = -(mLookatAngle[2].y + mCurAngle.y);
        mEyeAngle.y += cM_atan2s(sp8c.x, sp8c.z);
        mEyeAngle.x = -cM_atan2s(sp8c.y, sp8c.absXZ());
    } else {
        mEyeAngle.y = 0;
        mEyeAngle.x = 0;
    }

    if (field_0x1a84 == 2) {
        fVar3 = 100.0f;
        fVar2 = 120.0f;
        sp8c.set(0.0f, 0.0f, 100.0f);
        mDoMtx_stack_c::transS(current.pos);
        mDoMtx_stack_c::YrotM(mCurAngle.y);
        mDoMtx_stack_c::multVec(&sp8c, &sp80);
        attention_info.position.set(sp80.x, sp80.y + attention_offset + 120.0f, sp80.z);
    } else if (mMotion == MOT_TO_STONE_NORMAL) {
        attention_info.position = current.pos;
        attention_info.position.y += 200.0f;
    } else {
        fVar3 = 110.0f;
        fVar2 = 50.0f;
        sp8c.set(0.0f, 0.0f, 0.0f);
        mDoMtx_stack_c::transS(current.pos);
        mDoMtx_stack_c::YrotM(mCurAngle.y);
        mDoMtx_stack_c::multVec(&sp8c, &sp80);
        attention_info.position.set(mHeadPos.x, mHeadPos.y + attention_offset, mHeadPos.z);

        if (mType == TYPE_SMASH && !dComIfGp_event_runCheck()) {
            fVar3 = 35.0f;
            sp8c.set(0.0f, 0.0f, 35.0f);
            mDoMtx_stack_c::multVec(&sp8c, &sp80);
        }
    }

    mCcStts.Move();

    if (!mHide && !mTwilight) {
        if (!mIsDamaged) {
            mCyl1.SetTgType(0xD8FBFDFF);
            mCyl1.SetTgSPrm(0x1F);
            mCyl1.OnTgNoHitMark();
        } else {
            mCyl1.SetTgType(0);
            mCyl1.SetTgSPrm(0);
        }

        mCyl1.SetC(sp80);
        mCyl1.SetH(daNpc_Grz_Param_c::m.common.height + fVar2);
        mCyl1.SetR(daNpc_Grz_Param_c::m.common.width + fVar3);
        dComIfG_Ccsp()->Set(&mCyl1);
    }

    mCyl1.ClrTgHit();
    setCollisionPunch();
}

void daNpc_Grz_c::setCollisionPunch() {
    if (mType == TYPE_SMASH) {
        f32 frame = mAnm_p->getFrame();

        if (mAnm == ANM_PUNCHING && ((frame >= 15.0f && frame < 16.0f) || (frame >= 35.0f && frame < 36.0f))) {
            daPy_py_c* player = daPy_getPlayerActorClass();
            f32 fVar2 = (player->current.pos - current.pos).absXZ();

            if (fVar2 < 500.0f) {
                dComIfGp_getVibration().StartShock(6, 15, cXyz(0.0f, 1.0f, 0.0f));
            } else if (fVar2 < 700.0f) {
                dComIfGp_getVibration().StartShock(5, 15, cXyz(0.0f, 1.0f, 0.0f));
            } else if (fVar2 < 900.0f) {
                dComIfGp_getVibration().StartShock(4, 15, cXyz(0.0f, 1.0f, 0.0f));
            } else if (fVar2 < 1100.0f) {
                dComIfGp_getVibration().StartShock(3, 15, cXyz(0.0f, 1.0f, 0.0f));
            } else if (fVar2 < 1300.0f) {
                dComIfGp_getVibration().StartShock(2, 15, cXyz(0.0f, 1.0f, 0.0f));
            } else if (fVar2 < 1500.0f) {
                dComIfGp_getVibration().StartShock(1, 15, cXyz(0.0f, 1.0f, 0.0f));
            }
        }

        for (int i = 0; i < 4; i++) {
            if (!mHide) {
                if (mAnm == ANM_PUNCHING && ((frame >= 10.0f && frame < 18.0f && i >= 2) || ((frame < 2.0f || frame >= 29.0f) && i < 2))) {
                    cXyz sp60;
                    mSphs[i].OnAtSetBit();
                    J3DModel* model = mAnm_p->getModel();

                    static int l_attack_jnt[4] = {
                        JNT_ARML2, JNT_HANDL, JNT_ARMR2, JNT_HANDR,
                    };

                    mDoMtx_stack_c::copy(model->getAnmMtx(l_attack_jnt[i]));
                    mDoMtx_stack_c::scaleM(scale);
                    mDoMtx_stack_c::multVecZero(&sp60);
                    sp60.y += 10.0f;
                    mSphs[i].SetC(sp60);
                    mSphs[i].SetR(85.0f);
                    dComIfG_Ccsp()->Set(&mSphs[i]);
                } else {
                    mSphs[i].OffAtSetBit();
                }
            }

            mSphs[i].ClrAtHit();
        }

        if (mAnm == ANM_PUNCHING && !dComIfGp_event_runCheck()) {
            cXyz sp6c(0.0f, 0.0f, 245.0f);
            mDoMtx_stack_c::transS(current.pos);
            mDoMtx_stack_c::YrotM(mCurAngle.y);
            mDoMtx_stack_c::multVec(&sp6c, &sp6c);
            mCyl2.SetTgType(0xD8FBFDFF);
            mCyl2.SetTgSPrm(0x1F);
            mCyl2.OnTgNoHitMark();
            mCyl2.SetC(sp6c);
            f32 heightOffset = 50.0f;
            mCyl2.SetH(daNpc_Grz_Param_c::m.common.height + heightOffset);
            f32 widthOffset = 15.0f;
            mCyl2.SetR(daNpc_Grz_Param_c::m.common.width + widthOffset);
            dComIfG_Ccsp()->Set(&mCyl2);
        }
    }
}

bool daNpc_Grz_c::setExpressionAnm(int i_index, bool i_modify) {
    J3DAnmTransform* anm = NULL;
    int i_attr = J3DFrameCtrl::EMode_NONE;

    mAnmFlags &= ~ANM_EXPRESSION_FLAGS;
    
    if (l_bckGetParamList[i_index].fileIdx >= 0) {
        anm = getTrnsfrmKeyAnmP(l_resNames[l_bckGetParamList[i_index].arcIdx], l_bckGetParamList[i_index].fileIdx);
    }
    bool bVar1 = false;

    switch (i_index) {
        case ANM_NONE:
            bVar1 = setExpressionBtp(EXPR_BTP_GRZ);
            break;

        case ANM_F_MUKIDASHI:
            bVar1 = setExpressionBtp(EXPR_BTP_GRZ);
            i_attr = J3DFrameCtrl::EMode_LOOP;
            break;

        case ANM_F_RECOVER:
            bVar1 = setExpressionBtp(EXPR_BTP_F_RECOVER);
            break;

        case ANM_FH_TALK_A:
            bVar1 = setExpressionBtp(EXPR_BTP_GRZ);
            i_attr = J3DFrameCtrl::EMode_LOOP;
            break;

        case ANM_F_TALK_A:
            bVar1 = setExpressionBtp(EXPR_BTP_GRZ);
            break;

        case ANM_LIEDOWN:
            bVar1 = setExpressionBtp(EXPR_BTP_F_LIEDOWN);
            i_attr = J3DFrameCtrl::EMode_LOOP;
            break;

        case ANM_GETUP:
            bVar1 = setExpressionBtp(EXPR_BTP_F_GETUP);
            break;

        case ANM_F_WEAK_WAIT:
            bVar1 = setExpressionBtp(EXPR_BTP_F_WEAK_WAIT);
            i_attr = J3DFrameCtrl::EMode_LOOP;
            break;

        case ANM_F_WEAK_TURN:
            bVar1 = setExpressionBtp(EXPR_BTP_F_WEAK_TURN);
            break;

        case ANM_F_WEAK_TALK:
            bVar1 = setExpressionBtp(EXPR_BTP_F_WEAK_TALK);
            break;

        case ANM_F_SNIFF:
            bVar1 = setExpressionBtp(EXPR_BTP_F_SNIFF);
            break;

        case ANM_F_LAUGH:
            bVar1 = setExpressionBtp(EXPR_BTP_F_LAUGH);
            break;

        case ANM_FH_LAUGH:
            bVar1 = setExpressionBtp(EXPR_BTP_FH_LAUGH);
            i_attr = J3DFrameCtrl::EMode_LOOP;
            break;

        case ANM_F_PUNCHING:
            bVar1 = setExpressionBtp(EXPR_BTP_GRZ);
            i_attr = J3DFrameCtrl::EMode_LOOP;
            break;

        case ANM_F_PUNCH:
            bVar1 = setExpressionBtp(EXPR_BTP_GRZ);
            break;

        default:
            anm = NULL;
            break;
    }

    if (!bVar1) {
        return false;
    }

    if (anm == NULL) {
        return true;
    }

    if (setBckAnm(anm, 1.0f, i_attr, 0, -1, i_modify)) {
        mAnmFlags |= ANM_PLAY_BCK | ANM_PAUSE_BCK;
        mExpressionLoops = 0;
        return true;
    }

    OS_REPORT("%s: 表情Bckアニメーションの登録に失敗しました！\n", __FILE__);
    return false;
}

bool daNpc_Grz_c::setExpressionBtp(int i_index) {
    J3DAnmTexPattern* btp = NULL;
    int i_attr = J3DFrameCtrl::EMode_NONE;

    mAnmFlags &= 0xFFFFF57F;

    if (l_btpGetParamList[i_index].fileIdx >= 0) {
        btp = getTexPtrnAnmP(l_resNames[l_btpGetParamList[i_index].arcIdx], l_btpGetParamList[i_index].fileIdx);
    }

    switch (i_index) {
        case EXPR_BTP_GRZ:
        case EXPR_BTP_F_LIEDOWN:
        case EXPR_BTP_F_WEAK_WAIT:
        case EXPR_BTP_FH_LAUGH:
            i_attr = J3DFrameCtrl::EMode_LOOP;
            break;

        case EXPR_BTP_F_GETUP:
        case EXPR_BTP_F_RECOVER:
            break;

        case EXPR_BTP_F_WEAK_TURN:
        case EXPR_BTP_F_WEAK_TALK:
        case EXPR_BTP_F_SNIFF:
        case EXPR_BTP_F_LAUGH:
            break;
            
        default:
            btp = NULL;
            break;
    }

    if (btp == NULL) {
        return true;
    }

    if (setBtpAnm(btp, mAnm_p->getModel()->getModelData(), 1.0f, i_attr)) {
        mAnmFlags |= ANM_PLAY_BTP | ANM_PAUSE_BTP;

        if (i_index == 0) {
            mAnmFlags |= ANM_FLAG_800;
        }

        return true;
    }

    OS_REPORT("%s: 表情Btpアニメーションの登録に失敗しました！\n", "d_a_npc_grz.cpp");
    return false;
}

void daNpc_Grz_c::setExpression(int i_expression, f32 i_morf) {
    if (i_expression >= 0 && i_expression < 0xB) {
        mExpression = i_expression;
        mExpressionMorfOverride = i_morf;
        mExpressionPrevPhase = -1;
        mExpressionPhase = 0;
    }
}

void daNpc_Grz_c::setMotionAnm(int i_index, f32 i_morf) {
    J3DAnmTransformKey* anm = NULL;
    J3DAnmTextureSRTKey* btk = NULL;
    int index = 0;
    int i_attr = J3DFrameCtrl::EMode_LOOP;
    int i_attr2 = J3DFrameCtrl::EMode_LOOP;

    mAnmFlags &= ~ANM_MOTION_FLAGS;

    if (l_bckGetParamList[i_index].fileIdx >= 0) {
        anm = getTrnsfrmKeyAnmP(l_resNames[l_bckGetParamList[i_index].arcIdx], l_bckGetParamList[i_index].fileIdx);
    }

    mAnm = i_index;
    
    switch (i_index) {
        case ANM_WAIT_A:
        case ANM_UNK_18:
        case ANM_ROTATE:
        case ANM_STEP:
        case ANM_LIEDOWN_2:
        case ANM_WEAK_WAIT:
        case ANM_WAIT_B:
        case ANM_WALK_A:
        case ANM_PUNCHING:
            break;

        case ANM_TALK_A:
            i_attr = J3DFrameCtrl::EMode_NONE;
            break;

        case ANM_TO_STONE_NORMAL:
            i_attr = J3DFrameCtrl::EMode_NONE;
            break;

        case ANM_RECOVER:
            i_attr = J3DFrameCtrl::EMode_NONE;
            break;

        case ANM_GETUP_2:
            index = 1;
            i_attr2 = J3DFrameCtrl::EMode_NONE;
            i_attr = J3DFrameCtrl::EMode_NONE;
            break;

        case ANM_WEAK_TURN:
            i_attr = J3DFrameCtrl::EMode_NONE;
            break;

        case ANM_WEAK_STEP:
            i_attr = J3DFrameCtrl::EMode_NONE;
            break;

        case ANM_SNIFF:
            index = 2;
            i_attr2 = J3DFrameCtrl::EMode_NONE;
            i_attr = J3DFrameCtrl::EMode_NONE;
            break;

        case ANM_LAUGH:
            i_attr = J3DFrameCtrl::EMode_NONE;
            break;

        case ANM_UNAZUKI:
            i_attr = J3DFrameCtrl::EMode_NONE;
            break;

        case ANM_PUNCH:
            i_attr = J3DFrameCtrl::EMode_NONE;
            break;

        default:
            anm = NULL;
            btk = NULL;
            break;
    }

    if (l_btkGetParamList[index].fileIdx >= 0) {
        btk = getTexSRTKeyAnmP(l_resNames[l_btkGetParamList[index].arcIdx], l_btkGetParamList[index].fileIdx);
    }

    if (btk != NULL) {
        if (setBtkAnm(btk, mAnm_p->getModel()->getModelData(), 1.0f, i_attr2)) {
            mAnmFlags |= ANM_PLAY_BTK | ANM_PAUSE_BTK;
        }
    }

    if (anm != NULL && setMcaMorfAnm(anm, 1.0f, i_morf, i_attr, 0, -1)) {
        mAnmFlags |= ANM_PLAY_MORF | ANM_PAUSE_MORF;
        mMotionLoops = 0;
    }
}

void daNpc_Grz_c::setMotion(int i_motion, f32 i_morf, int param_3) {
    s16 motion = i_motion;
    if ((param_3 != 0 || mMotion != motion) && i_motion >= 0 && i_motion < 0xF) {
        mMotion = motion;
        mMotionMorfOverride = i_morf;
        mMotionPrevPhase = -1;
        mMotionPhase = 0;
    }
}

int daNpc_Grz_c::drawDbgInfo() {
    return 0;
}

void daNpc_Grz_c::drawOtherMdls() {
    /* empty function */
}

int daNpc_Grz_c::getTypeFromParam() {
    switch (fopAcM_GetParam(this) & 0xFF) {
        case 0:
            return TYPE_YELIA;
        
        case 1:
            return TYPE_LIEDOWN;

        case 2:
            return TYPE_SMASH;

        default:
            return TYPE_3;
    }
}

BOOL daNpc_Grz_c::isDelete() {
    if (mType == TYPE_3 || mType == TYPE_LIEDOWN) {
        return FALSE;
    }

    if (mType == TYPE_YELIA) {
        if (daNpcF_chkEvtBit(267) &&    // dSv_event_flag_c::F_0267 - Temple of Time - Temple of Time clear
            !daNpcF_chkEvtBit(287)) {   // dSv_event_flag_c::F_287 - Kakariko Village - Handed wood carving to Ilia
            return FALSE;
        }
    } else if (mType == TYPE_SMASH) {
        if (!dComIfGs_isSwitch(mSwNo, fopAcM_GetRoomNo(this))) {
            return FALSE;
        }
    }

    return TRUE;
}

void daNpc_Grz_c::reset() {
    initialize();
    mpMatAnm->initialize();
    mPath.initialize();

    if (mType == TYPE_SMASH) {
        if (mPath.setPathInfo(getPathNoFromParam(), fopAcM_GetRoomNo(this), 0) != 0) {
            mPath.setRange(100.0f);
        }
    }

    mLookat.initialize();

    for (int i = 0; i < 4; i++) {
        mActorMngrs[i].initialize();
    }

    mNextAction = NULL;
    mAction = NULL;
    field_0x1a64 = 0;
    field_0x1a68 = 0;
    field_0x1a6c = 0;
    field_0x1a70 = 0;
    mLookMode = -1;
    mMode = 0;
    current.pos = home.pos;
    old.pos = current.pos;
    current.angle.set(0, home.angle.y, 0);
    old.angle = current.angle;
    shape_angle = current.angle;
    mCurAngle = current.angle;
    mOldAngle = mCurAngle;
    speedF = 0.0f;
    speed.setall(0.0f);

    for (int i = 0; i < 1; i++) {
        mPrtclMngr[i].mPaPo.init(&mAcch, 0.0f, 0.0f);
    }

    if (mType == TYPE_LIEDOWN) {
        field_0x1a84 = 2;
        mMsgNo = 208;
        mColor.r = daNpc_Grz_Param_c::m.color_r;
        mColor.g = daNpc_Grz_Param_c::m.color_g;
        mColor.b = daNpc_Grz_Param_c::m.color_b;
        setExpression(EXPR_LIEDOWN, -1.0f);
        setMotion(MOT_LIEDOWN, 0.0f, 0);
    } else {
        field_0x1a84 = 0;
        mColor.r = 0;
        mColor.g = 0;
        mColor.b = 0;
        setExpression(EXPR_NONE, -1.0f);
        setMotion(MOT_WAIT_A, -1.0f, 0);
    }

    mTimer = 0;
    field_0x1a88 = 0;
    field_0x1a9c = 0;

    for (int i = 0; i < 3; i++) {
        field_0x1aa0[i] = -1;
    }

    field_0x9ee = true;
}

void daNpc_Grz_c::resetCol() {
    mCyl1.Set(mCcDCyl);
    mCyl1.SetStts(&mCcStts);
    mCyl1.SetTgHitCallback(tgHitCallBack);
    mCyl2.Set(mCcDCyl);
    mCyl2.SetStts(&mCcStts);

    for (int i = 0; i < 4; i++) {
        mSphs[i].Set(mCcDSph);
        mSphs[i].SetStts(&mCcStts);
        mSphs[i].SetAtSe(dCcD_SE_HARD_BODY);
        mSphs[i].SetAtSpl((dCcG_At_Spl)1);
        mSphs[i].SetAtType(AT_TYPE_800);
        mSphs[i].OnAtSPrmBit(12);
        mSphs[i].SetAtAtp(1);
        mSphs[i].OnTgNoConHit();
        mSphs[i].OffCoSetBit();
    }
}

void daNpc_Grz_c::playExpression() {
    daNpcF_anmPlayData dat0a = {ANM_F_TALK_A, daNpc_Grz_Param_c::m.common.morf_frame, 1};
    daNpcF_anmPlayData dat0b= {ANM_FH_TALK_A, daNpc_Grz_Param_c::m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat0[2] = {&dat0a, &dat0b};
    daNpcF_anmPlayData dat1a = {ANM_F_WEAK_TALK, daNpc_Grz_Param_c::m.common.morf_frame, 1};
    daNpcF_anmPlayData dat1b = {ANM_F_WEAK_WAIT, daNpc_Grz_Param_c::m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat1[2] = {&dat1a, &dat1b};
    daNpcF_anmPlayData dat2a = {ANM_F_SNIFF, daNpc_Grz_Param_c::m.common.morf_frame, 1};
    daNpcF_anmPlayData dat2b = {ANM_FH_TALK_A, daNpc_Grz_Param_c::m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat2[2] = {&dat2a, &dat2b};
    daNpcF_anmPlayData dat3a = {ANM_F_LAUGH, daNpc_Grz_Param_c::m.common.morf_frame, 1};
    daNpcF_anmPlayData dat3b = {ANM_NONE, daNpc_Grz_Param_c::m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat3[2] = {&dat3a, &dat3b};
    daNpcF_anmPlayData dat4 = {ANM_FH_TALK_A, daNpc_Grz_Param_c::m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat4[1] = {&dat4};
    daNpcF_anmPlayData dat5 = {ANM_F_PUNCHING, daNpc_Grz_Param_c::m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat5[1] = {&dat5};
    daNpcF_anmPlayData dat6a = {ANM_F_PUNCH, daNpc_Grz_Param_c::m.common.morf_frame, 1};
    daNpcF_anmPlayData dat6b = {ANM_NONE, daNpc_Grz_Param_c::m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat6[2] = {&dat6a, &dat6b};
    daNpcF_anmPlayData dat7 = {ANM_LIEDOWN, daNpc_Grz_Param_c::m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat7[1] = {&dat7};
    daNpcF_anmPlayData dat8 = {ANM_GETUP, daNpc_Grz_Param_c::m.common.morf_frame, 1};
    daNpcF_anmPlayData* pDat8[1] = {&dat8};
    daNpcF_anmPlayData dat9 = {ANM_F_WEAK_WAIT, daNpc_Grz_Param_c::m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat9[1] = {&dat9};
    daNpcF_anmPlayData dat10 = {ANM_NONE, daNpc_Grz_Param_c::m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat10[1] = {&dat10};

    daNpcF_anmPlayData** ppDat[11] = {
        pDat0,
        pDat1,
        pDat2,
        pDat3,
        pDat4,
        pDat5,
        pDat6,
        pDat7,
        pDat8,
        pDat9,
        pDat10,
    };

    if (mExpression >= 0 && mExpression < 0xB) {
        playExpressionAnm(ppDat);
    }
}

void daNpc_Grz_c::playMotion() {
    daNpcF_anmPlayData dat0 = {ANM_WAIT_A, daNpc_Grz_Param_c::m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat0[1] = {&dat0};
    daNpcF_anmPlayData dat1 = {ANM_WEAK_WAIT, daNpc_Grz_Param_c::m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat1[1] = {&dat1};
    daNpcF_anmPlayData dat2a = {ANM_WEAK_TURN, daNpc_Grz_Param_c::m.common.morf_frame, 1};
    daNpcF_anmPlayData dat2b = {ANM_WEAK_WAIT, daNpc_Grz_Param_c::m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat2[2] = {&dat2a, &dat2b};
    daNpcF_anmPlayData dat3 = {ANM_WAIT_B, daNpc_Grz_Param_c::m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat3[1] = {&dat3};
    daNpcF_anmPlayData dat4a = {ANM_SNIFF, daNpc_Grz_Param_c::m.common.morf_frame, 1};
    daNpcF_anmPlayData dat4b = {ANM_WAIT_B, daNpc_Grz_Param_c::m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat4[2] = {&dat4a, &dat4b};
    daNpcF_anmPlayData dat5a = {ANM_LAUGH, daNpc_Grz_Param_c::m.common.morf_frame, 1};
    daNpcF_anmPlayData dat5b = {ANM_WAIT_B, daNpc_Grz_Param_c::m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat5[2] = {&dat5a, &dat5b};
    daNpcF_anmPlayData dat6 = {ANM_PUNCHING, daNpc_Grz_Param_c::m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat6[1] = {&dat6};
    daNpcF_anmPlayData dat7a = {ANM_PUNCH, daNpc_Grz_Param_c::m.common.morf_frame, 1};
    daNpcF_anmPlayData dat7b = {ANM_WAIT_A, daNpc_Grz_Param_c::m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat7[2] = {&dat7a, &dat7b};
    daNpcF_anmPlayData dat8a = {ANM_TO_STONE_NORMAL, daNpc_Grz_Param_c::m.common.morf_frame, 1};
    daNpcF_anmPlayData dat8b = {ANM_ROTATE, 6.0f, 0};
    daNpcF_anmPlayData* pDat8[2] = {&dat8a, &dat8b};
    daNpcF_anmPlayData dat9a = {ANM_UNAZUKI, daNpc_Grz_Param_c::m.common.morf_frame, 1};
    daNpcF_anmPlayData dat9b = {ANM_WAIT_A, daNpc_Grz_Param_c::m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat9[2] = {&dat9a, &dat9b};
    daNpcF_anmPlayData dat10 = {ANM_WALK_A, daNpc_Grz_Param_c::m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat10[1] = {&dat10};
    daNpcF_anmPlayData dat11 = {ANM_LIEDOWN_2, daNpc_Grz_Param_c::m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat11[1] = {&dat11};
    daNpcF_anmPlayData dat12 = {ANM_GETUP_2, daNpc_Grz_Param_c::m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat12[1] = {&dat12};
    daNpcF_anmPlayData dat13 = {ANM_STEP, 6.0f, 0};
    daNpcF_anmPlayData* pDat13[1] = {&dat13};
    daNpcF_anmPlayData dat14 = {ANM_WEAK_STEP, 6.0f, 0};
    daNpcF_anmPlayData* pDat14[1] = {&dat14};

    daNpcF_anmPlayData** ppDat[15] = {
        pDat0,
        pDat1,
        pDat2,
        pDat3,
        pDat4,
        pDat5,
        pDat6,
        pDat7,
        pDat8,
        pDat9,
        pDat10,
        pDat11,
        pDat12,
        pDat13,
        pDat14,
    };

    if (mMotion >= 0 && mMotion < 0xF) {
        playMotionAnm(ppDat);
    }
}

BOOL daNpc_Grz_c::chkAction(actionFunc action) {
    return mAction == action;
}

BOOL daNpc_Grz_c::setAction(actionFunc action) {
    mMode = 3;

    if (mAction) {
        (this->*mAction)(NULL);
    }

    mMode = 0;
    mAction = action;

    if (mAction) {
        (this->*mAction)(NULL);
    }

    return TRUE;
}

BOOL daNpc_Grz_c::selectAction() {
    mNextAction = NULL;

    if (daNpc_Grz_Param_c::m.common.debug_mode_ON) {
        mNextAction = &daNpc_Grz_c::test;
    } else {
        switch (mType) {
            case TYPE_LIEDOWN:
                if (field_0x1a84 > 0) {
                    mNextAction = &daNpc_Grz_c::lieDown;
                } else {
                    mNextAction = &daNpc_Grz_c::waitWeak;
                }
                break;
            
            case TYPE_SMASH:
                mNextAction = &daNpc_Grz_c::waitSmash;
                break;

            default:
                mNextAction = &daNpc_Grz_c::wait;
                break;
        }
    }

    return TRUE;
}

void daNpc_Grz_c::doNormalAction(int param_1) {
    if (param_1 != 0 && hitChk2(&mCyl1, TRUE, FALSE)) {
        int damage_time;

        if (mCutType == 8) {
            damage_time = 20;
        } else {
            damage_time = daNpc_Grz_Param_c::m.common.damage_time;
        }

        setDamage(damage_time, 10, 0);
        setLookMode(LOOK_RESET);
    } else if (mIsDamaged && mDamageTimer == 0) {
        mMode = 0;
        mIsDamaged = false;
    }

    mOrderEvtNo = EVT_NONE;

    if (mNextAction) {
        if (mAction == mNextAction) {
            (this->*mAction)(NULL);
        } else {
            setAction(mNextAction);
        }
    }
}

BOOL daNpc_Grz_c::doEvent() {
    BOOL rv = FALSE;

    if (dComIfGp_event_runCheck()) {
        dEvent_manager_c& eventManager = dComIfGp_getEventManager();

        if (eventInfo.checkCommandTalk() || eventInfo.checkCommandDemoAccrpt()) {
            mOrderNewEvt = false;
        }

        if (eventInfo.checkCommandTalk()) {
            if (field_0x1a84 > 0) {
                mOrderEvtNo = EVT_TALK_STAND;
                changeEvent(l_resNames[l_evtGetParamList[mOrderEvtNo].arcIdx], l_evtNames[l_evtGetParamList[mOrderEvtNo].fileIdx], 1, 0xFFFF);
            } else if (chkAction(&daNpc_Grz_c::talk)) {
                (this->*mAction)(NULL);
            } else if (dComIfGp_event_chkTalkXY() == FALSE || dComIfGp_evmng_ChkPresentEnd()) {
                setAction(&daNpc_Grz_c::talk);
            }

            rv = TRUE;
        } else {
            int staffId = eventManager.getMyStaffId(l_myName, NULL, 0);

            if (staffId != -1) {
                mStaffID = staffId;
                int evtCutNo = eventManager.getMyActIdx(staffId, mEvtCutNameList, 7, 0, 0);

                JUT_ASSERT(2262, (0 <= evtCutNo) && (evtCutNo < NUM_EVT_CUTS_e));
                JUT_ASSERT(2263, NULL != mEvtCutList[evtCutNo]);

                if ((this->*mEvtCutList[evtCutNo])(staffId)) {
                    eventManager.cutEnd(staffId);
                }

                rv = TRUE;
            }

            if (eventInfo.checkCommandDemoAccrpt()) {
                if (mEventIdx != -1 && eventManager.endCheck(mEventIdx)) {
                    switch (mOrderEvtNo) {
                        case EVT_STONE_SMASH:
                            dComIfGp_event_reset();
                            mOrderEvtNo = EVT_NONE;
                            mEventIdx = -1;
                            dComIfGs_onSwitch(mSwNo, fopAcM_GetRoomNo(this));
                            fopAcM_delete(this);
                            break;
                        
                        default:
                            dComIfGp_event_reset();
                            mOrderEvtNo = EVT_NONE;
                            mEventIdx = -1;
                            break;
                    }
                } else {
                    switch (mOrderEvtNo) {
                        case EVT_STONE_SMASH:
                            setSkipZev(2, 3);
                            break;

                        default:
                            break;
                    }
                }
            }
        }

        int msgTimer = mMsgTimer;
        int i_expression, i_motion;

        if (ctrlMsgAnm(i_expression, i_motion, this, FALSE)) {
            if (!field_0x9eb) {
                setExpression(i_expression, -1.0f);
                setMotion(i_motion, -1.0f, 0);
            }
        } else if (msgTimer != 0 && !field_0x9eb) {
            setExpressionTalkAfter();
        }

        switch (mExpression) {
            case EXPR_LAUGH:
            case EXPR_SNIFF:
                if (mExpressionPhase != 0) {
                    mAnmFlags &= ~ANM_PAUSE_EXPRESSION;
                }
                break;
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

bool daNpc_Grz_c::setSkipZev(int param_1, int param_2) {
    dEvent_manager_c& eventManager = dComIfGp_getEventManager();

    if (strcmp(eventManager.getRunEventName(), l_evtNames[param_1]) == 0) {
        dComIfGp_getEvent()->setSkipZev(this, l_evtNames[param_2]);
        dComIfGp_getEvent()->onSkipFade();
        return true;
    }

    return false;
}

void daNpc_Grz_c::setLookMode(int i_lookMode) {
    if (i_lookMode >= 0 && i_lookMode < 6 && i_lookMode != mLookMode) {
        mLookMode = i_lookMode;
    }
}

void daNpc_Grz_c::lookat() {
    daPy_py_c* player = NULL;
    J3DModel* model = mAnm_p->getModel();
    BOOL bVar1 = FALSE;
    f32 body_angleX_min = daNpc_Grz_Param_c::m.common.body_angleX_min;
    f32 body_angleX_max = daNpc_Grz_Param_c::m.common.body_angleX_max;
    f32 body_angleY_min = daNpc_Grz_Param_c::m.common.body_angleY_min;
    f32 body_angleY_max = daNpc_Grz_Param_c::m.common.body_angleY_max;
    f32 head_angleX_min = daNpc_Grz_Param_c::m.common.head_angleX_min;
    f32 head_angleX_max = daNpc_Grz_Param_c::m.common.head_angleX_max;
    f32 head_angleY_min = daNpc_Grz_Param_c::m.common.head_angleY_min;
    f32 head_angleY_max = daNpc_Grz_Param_c::m.common.head_angleY_max;
    s16 angle_delta = mCurAngle.y - mOldAngle.y;
    cXyz lookat_pos[3] = {mLookatPos[0], mLookatPos[1], mLookatPos[2]};
    csXyz* lookat_angle[3] = {&mLookatAngle[0], &mLookatAngle[1], &mLookatAngle[2]};
    cXyz spe8;

    switch (mLookMode) {
        case LOOK_RESET:
            bVar1 = TRUE;
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

        case LOOK_ATTN:
            player = (daPy_py_c*)mActorMngrs[2].getActorP();
            break;
    }

    if (player != NULL) {
        mLookPos = player->attention_info.position;

        if (mLookMode != 2 && mLookMode != 3 && mLookMode != 4) {
            mLookPos.y -= 40.0f;
        }

        mLookat.setAttnPos(&mLookPos);
    } else {
        mLookat.setAttnPos(NULL);
    }

    mLookat.setParam(body_angleX_min, body_angleX_max, body_angleY_min, body_angleY_max, 0.0f, 0.0f, 0.0f, 0.0f,
                     head_angleX_min, head_angleX_max, head_angleY_min, head_angleY_max, mCurAngle.y, lookat_pos);
    mLookat.calc(this, model->getBaseTRMtx(), lookat_angle, bVar1, angle_delta, FALSE);
}

void daNpc_Grz_c::setExpressionTalkAfter() {
    switch (mExpression) {
        case EXPR_TALK_A:
            setExpression(EXPR_H_TALK_A, -1.0f);
            break;
        
        case EXPR_WEAK_TALK:
            setExpression(EXPR_WEAK_WAIT, -1.0f);
            break;

        case EXPR_SNIFF:
            break;

        default:
            setExpression(EXPR_NONE, -1.0f);
            break;
    }
}

int daNpc_Grz_c::wait(void* param_1) {
    switch (mMode) {
        case 0:
            if (mType == TYPE_LIEDOWN) {
                setExpression(EXPR_WEAK_WAIT, -1.0f);
                setMotion(MOT_WEAK_WAIT, -1.0f, 0);
            } else {
                setExpression(EXPR_NONE, -1.0f);
                setMotion(MOT_WAIT_A, -1.0f, 0);
            }

            setLookMode(LOOK_NONE);
            mTurnMode = 0;
            mMode = 2;
            // fallthrough
        case 2:
            if (mType == TYPE_YELIA && mHide == true) {
                fopAcM_delete(this);
            }

            if (!mIsDamaged) {
                BOOL bVar1 = mActorMngrs[0].getActorP() != NULL;
                if (chkFindPlayer2(bVar1, shape_angle.y)) {
                    if (!bVar1) {
                        mActorMngrs[0].entry(daPy_getPlayerActorClass());
                        mTurnMode = 0;
                    }
                } else if (bVar1) {
                    mActorMngrs[0].remove();
                    mTurnMode = 0;
                }

                if (mActorMngrs[0].getActorP() != NULL) {
                    setLookMode(LOOK_PLAYER);
                } else {
                    setLookMode(LOOK_NONE);

                    if (home.angle.y != mCurAngle.y) {
                        if (mType == TYPE_LIEDOWN) {
                            if (step(home.angle.y, 9, 14, 45)) {
                                mMode = 0;
                            }
                        } else if (step(home.angle.y, 10, 13, 15)) {
                            mMode = 0;
                        }
                    }
                }

                if (home.angle.y == mCurAngle.y) {
                    fopAc_ac_c* actor_p = getAttnActorP(mActorMngrs[0].getActorP() != NULL, srchAttnActor1,
                                                        daNpc_Grz_Param_c::m.common.search_distance, daNpc_Grz_Param_c::m.common.search_height,
                                                        daNpc_Grz_Param_c::m.common.search_depth, daNpc_Grz_Param_c::m.common.fov,
                                                        shape_angle.y, 120, TRUE);
                    if (actor_p != NULL) {
                        mActorMngrs[1].entry(actor_p);
                        setLookMode(LOOK_ACTOR);
                    }
                } else {
                    mAttnChangeTimer = 0;
                }
            }
            break;

        case 3:
            break;
    }

    return 1;
}

static void* s_sub(void* i_actor, void* i_data) {
    if (fopAc_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_Obj_GrzRock) {
        return i_actor;
    }

    return NULL;
}

static void* s_sub2(void* i_actor, void* i_data) {
    if (fopAc_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_NPC_YELIA) {
        return i_actor;
    }

    return NULL;
}

static void* s_sub3(void* i_actor, void* i_data) {
    if (fopAc_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_NPC_GRD) {
        return i_actor;
    }

    return NULL;
}

int daNpc_Grz_c::waitSmash(void* param_1) {
    switch (mMode) {
        case 0:
            setExpression(EXPR_PUNCHING, -1.0f);
            setMotion(MOT_PUNCHING, -1.0f, 0);
            setLookMode(LOOK_NONE);
            mTurnMode = 0;
            speedF = 0.0f;
            mMode = 2;
            // fallthrough
        case 2:
            if (mHide == true) {
                mOrderEvtNo = EVT_NONE;
                mEventIdx = -1;
                dComIfGs_onSwitch(mSwNo, fopAcM_GetRoomNo(this));
                fopAcM_delete(this);
            } else {
                if (mAnm == ANM_PUNCHING) {
                    f32 frame = mAnm_p->getFrame();
                    if ((frame >= 10.0f && frame < 11.0f) || (frame >= 28.0f && frame < 29.0f)) {
                        mSound.startCreatureVoice(Z2SE_GRZ_V_BREAK, -1);
                    }
                }

                if (mActorMngrs[3].getActorP() == NULL) {
                    fopAc_ac_c* actor_p = (fopAc_ac_c*)fpcM_Search(s_sub, this);
                    if (actor_p != NULL) {
                        mActorMngrs[3].entry(actor_p);
                        cXyz sp3c(0.0f, 100.0f, 400.0f);
                        mDoMtx_stack_c::YrotS(actor_p->current.angle.y);
                        mDoMtx_stack_c::multVec(&sp3c, &sp3c);
                        current.pos = sp3c + actor_p->current.pos;

                        if (fopAcM_gc_c::gndCheck(&current.pos)) {
                            current.pos.y = fopAcM_gc_c::getGroundY();
                        }

                        old.pos = current.pos;
                        home.pos = old.pos;
                    }
                } else if (!daPy_py_c::checkNowWolf()) {
                    if ((daPy_getPlayerActorClass()->current.pos - current.pos).absXZ() < daNpc_Grz_Param_c::m.demo_start_distance) {
                        mOrderEvtNo = EVT_STONE_SMASH;
                    }
                }
            }
            break;

        case 3:
            break;
    }

    return 1;
}

int daNpc_Grz_c::lieDown(void* param_1) {
    switch (mMode) {
        case 0:
            setExpression(EXPR_LIEDOWN, -1.0f);
            setMotion(MOT_LIEDOWN, -1.0f, 0);
            setLookMode(LOOK_RESET);
            mTurnMode = 0;
            field_0x9ea = true;
            mMode = 2;
            break;

        case 2:
            break;
    }

    return 1;
}

int daNpc_Grz_c::waitWeak(void* param_1) {
    switch (mMode) {
        case 0:
            setExpression(EXPR_WEAK_WAIT, -1.0f);
            setMotion(MOT_WEAK_WAIT, -1.0f, 0);
            setLookMode(LOOK_NONE);
            mTurnMode = 0;
            field_0x9ea = true;
            mMode = 2;
            break;

        case 2:
            break;
    }

    return 1;
}

int daNpc_Grz_c::talk(void* param_1) {
    int rv = 0;
    BOOL bVar1 = FALSE;

    switch (mMode) {
        case 0:
            if (mIsDamaged) {
                break;
            }

            initTalk(mMsgNo, NULL);
            mTurnMode = 0;
            mMode = 2;
            // fallthrough
        case 2:
            if (field_0x9ea) {
                bVar1 = TRUE;
            } else {
                setLookMode(LOOK_PLAYER_TALK);
                mActorMngrs[0].entry(daPy_getPlayerActorClass());

                if (mCurAngle.y == fopAcM_searchPlayerAngleY(this)) {
                    bVar1 = TRUE;
                } else if (mType == TYPE_LIEDOWN) {
                    if (step(fopAcM_searchPlayerAngleY(this), 9, 14, 30)) {
                        setExpression(EXPR_WEAK_WAIT, -1.0f);
                        setMotion(MOT_WEAK_WAIT, -1.0f, 0);
                        mTurnMode = 0;
                    }
                } else if (step(fopAcM_searchPlayerAngleY(this), 10, 13, 15)) {
                    setExpression(EXPR_NONE, -1.0f);
                    setMotion(MOT_WAIT_A, -1.0f, 0);
                    mTurnMode = 0;
                }
            }

            if (bVar1 && talkProc(NULL, TRUE, NULL)) {
                rv = 1;
            }

            if (rv != 0) {
                mMode = 3;

                if (!field_0x9ec) {
                    dComIfGp_event_reset();
                }

                field_0x9ec = false;
            }
            break;

        case 3:
            break;
    }

    return rv;
}

int daNpc_Grz_c::test(void* param_1) {
    switch (mMode) {
        case 0:
            speedF = 0.0f;
            speed.setall(0.0f);
            mMode = 2;
            // fallthrough
        case 2:
            if (daNpc_Grz_Param_c::m.common.face_expression != mExpression) {
                setExpression(daNpc_Grz_Param_c::m.common.face_expression, daNpc_Grz_Param_c::m.common.morf_frame);
            }

            setMotion(daNpc_Grz_Param_c::m.common.motion, daNpc_Grz_Param_c::m.common.morf_frame, 0);
            setLookMode(daNpc_Grz_Param_c::m.common.look_mode);
            mOrderEvtNo = EVT_NONE;
            attention_info.flags = 0;
            break;

        case 3:
            break;
    }

    return 1;
}

int daNpc_Grz_c::setPrtcl() {
    for (int i = 0; i < 1; i++) {
        BOOL bVar1 = i == 0 ? TRUE : FALSE;
        mPrtclMngr[i].mPaPo.setEffectCenter(&tevStr, &mPrtclMngr[i].mPos, bVar1, 0,
                                            mPrtclMngr[i].field_0x0 == 1 ? &mPrtclMngr[i].mPos : NULL,
                                            mPrtclMngr[i].field_0x0 == 1 ? &mPrtclMngr[i].mAngle : NULL, NULL,
                                            fopAcM_GetRoomNo(this), 0.0f, speedF);
        
        if (mPrtclMngr[i].field_0x0 != 0) {
            JPABaseEmitter* emitter = mPrtclMngr[i].mPaPo.getCenterEmitter(0, 0);
            if (emitter != NULL) {
                switch (i) {
                    case 0:
                        mPrtclMngr[0].mScale.set(4.0f, 4.0f, 4.0f);
                        emitter->setGlobalParticleScale(mPrtclMngr[0].mScale);
                        emitter->setGlobalAlpha(64);
                        break;
                }
            }
        }

        mPrtclMngr[i].field_0x0 = 0;
    }

    return 1;
}

int daNpc_Grz_c::doWaitCut(int i_staffId) {
    int rv = 0;
    int timer = 0;
    int* piVar1 = dComIfGp_evmng_getMyIntegerP(i_staffId, "timer");
    if (piVar1 != NULL) {
        timer = *piVar1;
    }

    if (dComIfGp_getEventManager().getIsAddvance(i_staffId)) {
        mEventTimer = timer;
    }

    if (cLib_calcTimer(&mEventTimer) == 0) {
        rv = 1;
    }

    return rv;
}

int daNpc_Grz_c::doRebirthCut(int i_staffId) {
    int rv = 0;

    if (dComIfGp_getEventManager().getIsAddvance(i_staffId)) {
        mTimer = daNpc_Grz_Param_c::m.color_time;
    }

    if (cLib_calcTimer(&mTimer) == 0) {
        mColor.r = 0;
        mColor.g = 0;
        mColor.b = 0;
        rv = 1;
    } else {
        mColor.r = daNpc_Grz_Param_c::m.color_r * ((f32)mTimer / daNpc_Grz_Param_c::m.color_time);
        mColor.g = daNpc_Grz_Param_c::m.color_g * ((f32)mTimer / daNpc_Grz_Param_c::m.color_time);
        mColor.b = daNpc_Grz_Param_c::m.color_b * ((f32)mTimer / daNpc_Grz_Param_c::m.color_time);
    }

    return rv;
}

int daNpc_Grz_c::doTalkStandCut(int i_staffId) {
    dEvent_manager_c& eventManager = dComIfGp_getEventManager();
    int rv = 0;
    int prm = -1;
    int* piVar1 = dComIfGp_evmng_getMyIntegerP(i_staffId, "prm");
    if (piVar1 != NULL) {
        prm = *piVar1;
    }

    if (eventManager.getIsAddvance(i_staffId)) {
        switch (prm) {
            case 0:
                speedF = 0.0f;
                speed.set(0.0f, 0.0f, 0.0f);
                mOrderSpeakEvt = false;
                mTurnMode = 0;
                field_0x1a84 = 0;
                setLookMode(LOOK_NONE);
                setExpression(EXPR_GETUP, -1.0f);
                setMotion(MOT_GETUP, -1.0f, 0);
                break;
            
            case 10:
                setExpression(EXPR_WEAK_WAIT, -1.0f);
                setMotion(MOT_WEAK_WAIT, -1.0f, 0);
                break;

            case 20:
                initTalk(mMsgNo, NULL);
                break;
        }
    }

    switch (prm) {
        case 0:
            if (mAnm == ANM_GETUP_2) {
                if (mAnm_p->isStop()) {
                    rv = 1;
                }
            }
            break;

        case 10:
            rv = 1;
            break;
            
        case 20:
            if (talkProc(NULL, TRUE, NULL)) {
                rv = 1;
            }
            break;

        default:
            rv = 1;
            break;
    }

    return rv;
}

int daNpc_Grz_c::doGoOutCut(int i_staffId) {
    cXyz sp30(150.0f, 0.0f, 1020.0f);
    dEvent_manager_c* eventManager = (dEvent_manager_c*)&dComIfGp_getEventManager();
    int rv = 0;
    int prm = -1;
    int timer = 0;
    int* piVar1 = dComIfGp_evmng_getMyIntegerP(i_staffId, "prm");
    if (piVar1 != NULL) {
        prm = *piVar1;
    }

    piVar1 = dComIfGp_evmng_getMyIntegerP(i_staffId, "timer");
    if (piVar1 != NULL) {
        timer = *piVar1;
    }

    if (eventManager->getIsAddvance(i_staffId)) {
        fopAc_ac_c* actor;

        switch (prm) {
            case 0:
                actor = (fopAc_ac_c*)fpcM_Search(s_sub2, this);

                JUT_ASSERT(3349, NULL != actor);

                mActorMngrs[2].entry(actor);
                setLookMode(LOOK_ATTN);
                break;
            
            case 1:
                actor = (fopAc_ac_c*)fpcM_Search(s_sub3, this);

                JUT_ASSERT(3359, NULL != actor);

                mActorMngrs[2].entry(actor);
                setLookMode(LOOK_ATTN);
                break;

            case 10:
                setMotion(MOT_UNAZUKI, -1.0f, 0);
                setExpression(EXPR_NONE, -1.0f);
                mSound.startCreatureVoice(Z2SE_GRZ_V_NOD, -1);
                break;
            
            case 20:
                setLookMode(LOOK_NONE);
                setMotion(MOT_WALK_A, -1.0f, 0);
                setExpression(EXPR_NONE, -1.0f);
                speedF = daNpc_Grz_Param_c::m.walk_speed;
                mEventTimer = timer;
                break;

            case 21:
                mAnm_p->setPlaySpeed(0.0f);
                break;

            case 99:
                current.pos = sp30;
                old.pos = current.pos;
                mHide = true;
                mEventTimer = timer;
                break;
        }
    }

    switch (prm) {
        case 0: {
            fopAc_ac_c* actor_p = mActorMngrs[2].getActorP();

            JUT_ASSERT(3415, NULL != actor_p);

            s16 angleY = fopAcM_searchActorAngleY(this, actor_p);
            if (angleY == mCurAngle.y) {
                mTurnMode = 0;
                rv = 1;
            } else if (step(angleY, 10, 13, 15)) {
                mTurnMode = 0;
                setMotion(MOT_WAIT_A, -1.0f, 0);
            }
            break;
        }
        
        case 1:
        case 99:
            rv = 1;
            break;

        case 10:
            if (mMotion == MOT_UNAZUKI) {
                if (mMotionPhase > 0) {
                    rv = 1;
                }
            }
            break;

        case 20:
            if (mEventTimer != 0 || eventManager->getIsAddvance(i_staffId)) {
                if ((current.pos - sp30).abs() > 50.0f) {
                    speedF = daNpc_Grz_Param_c::m.walk_speed;
                    cLib_addCalcAngleS2(&mCurAngle.y, cLib_targetAngleY(&current.pos, &sp30), 4, 0x400);
                    setAngle(mCurAngle.y);
                } else {
                    mEventTimer = 0;
                }

                if (cLib_calcTimer(&mEventTimer) == 0) {
                    speedF = 0.0f;
                }
            } else {
                mHide = true;
                rv = 1;
            }
            break;

        case 21:
            if ((mEventTimer != 0 || eventManager->getIsAddvance(i_staffId)) && cLib_calcTimer(&mEventTimer) == 0) {
                mDoAud_seStart(Z2SE_GRZ_DOOR_CL, NULL, 0, 0);
                dComIfGp_getVibration().StartShock(7, 15, cXyz(0.0f, 1.0f, 0.0f));
            }

            if (mEventTimer == 0) {
                rv = 1;
            }
            break;

        default:
            rv = 1;
            break;
    }

    return rv;
}

int daNpc_Grz_c::doStoneSmashCut(int i_staffId) {
    dEvent_manager_c& eventManager = dComIfGp_getEventManager();
    int iVar1 = 0;
    int rv = 0;
    int* piVar1 = NULL;
    int prm = -1;
    piVar1 = dComIfGp_evmng_getMyIntegerP(i_staffId, "prm");
    if (piVar1 != NULL) {
        prm = *piVar1;
    }

    if (eventManager.getIsAddvance(i_staffId)) {
        switch (prm) {
            case 0: {
                dComIfGs_offStageSwitch(6, 64);
                cXyz sp58(-250.0f, 0.0f, -250.0f);
                mDoMtx_stack_c::YrotS(shape_angle.y);
                mDoMtx_stack_c::multVec(&sp58, &sp58);
                sp58 += current.pos;
                daPy_getPlayerActorClass()->setPlayerPosAndAngle(&sp58, shape_angle.y + 0x2000, 0);
                break;
            }

            case 10:
                break;
            
            case 20:
                dComIfGp_getVibration().StartQuake(5, 15, cXyz(0.0f, 1.0f, 0.0f));
                setMotion(MOT_PUNCH, -1.0f, 0);
                setExpression(EXPR_PUNCH, -1.0f);
                mSound.startCreatureVoice(Z2SE_GRZ_V_BREAK_FINISH_M, -1);
                break;

            case 30:
                break;

            case 40:
                setLookMode(LOOK_PLAYER_TALK);
                mActorMngrs[0].entry(daPy_getPlayerActorClass());
                break;
            
            case 50:
                initTalk(mMsgNo, NULL);
                break;

            case 60:
                setLookMode(LOOK_NONE);
                break;

            case 70:
                setMotion(MOT_TO_STONE_NORMAL, -1.0f, 0);
                setExpression(EXPR_NONE, -1.0f);
                break;

            case 75:
                mAnm_p->setPlaySpeed(0.0f);
                mAnm_p->setEndFrame(10.0f);
                break;

            case 80:
                daPy_getPlayerActorClass()->setPlayerPosAndAngle(&home.pos, (s16)home.angle.y, 0);
                break;
        }
    }

    switch (prm) {
        case 0:
            if (mAnm_p->getFrame() == mAnm_p->getStartFrame()) {
                field_0x1a9c++;
            }

            if (field_0x1a9c >= 2) {
                if (mAnm_p->getFrame() >= 20.0f) {
                    rv = 1;
                }
            } else {
                cXyz sp64(-250.0f, 0.0f, -250.0f);
                mDoMtx_stack_c::YrotS(shape_angle.y);
                mDoMtx_stack_c::multVec(&sp64, &sp64);
                sp64 += current.pos;
                daPy_getPlayerActorClass()->setPlayerPosAndAngle(&sp64, shape_angle.y + 0x2000, 0);
            }

            if (mAnm == ANM_PUNCHING) {
                f32 frame = mAnm_p->getFrame();
                if ((frame >= 10.0f && frame < 11.0f) || (frame >= 28.0f && frame < 29.0f)) {
                    mSound.startCreatureVoice(Z2SE_GRZ_V_BREAK, -1);
                }
            }
            break;
        
        case 10:
            if (mAnm_p->getFrame() == mAnm_p->getStartFrame()) {
                rv = 1;
            }

            if (mAnm == ANM_PUNCHING) {
                f32 frame = mAnm_p->getFrame();
                if ((frame >= 10.0f && frame < 11.0f) || (frame >= 28.0f && frame < 29.0f)) {
                    mSound.startCreatureVoice(Z2SE_GRZ_V_BREAK, -1);
                }
            }
            break;
        
        case 20:
            if (mAnm_p->getFrame() >= 17.0f) {
                dComIfGp_getVibration().StopQuake(31);
                rv = 1;
            }
            break;

        case 30:
            if (mMotionPhase > 0) {
                rv = 1;
            } else if (mAnm_p->getFrame() == 35.0f) {
                dComIfGp_getVibration().StartShock(8, 15, cXyz(0.0f, 1.0f, 0.0f));
                daObjGrzRock_c* rock_p = (daObjGrzRock_c*)mActorMngrs[3].getActorP();
                if (rock_p != NULL) {
                    rock_p->setSmashFlag(1);
                }
            }
            break;

        case 40:
            if (mCurAngle.y == fopAcM_searchPlayerAngleY(this)) {
                rv = 1;
            } else if (step(fopAcM_searchPlayerAngleY(this), 10, 13, 15)) {
                setExpression(EXPR_NONE, -1.0f);
                setMotion(MOT_WAIT_A, -1.0f, 0);
                mTurnMode = 0;
            }
            break;
        
        case 50:
            if (talkProc(NULL, TRUE, NULL)) {
                rv = 1;
            } else if (mAnm == ANM_SNIFF) {
                mSound.startCreatureVoiceLevel(Z2SE_GRZ_V_SNIFF, -1);
            } else {
                mSound.startCreatureVoiceLevel(Z2SE_GRZ_V_BREATH, -1);
            }
            break;

        case 60:
            if ((s16)(home.angle.y + 0x8000) == mCurAngle.y) {
                rv = 1;
            } else if (step(home.angle.y + 0x8000, 10, 13, 15)) {
                setExpression(EXPR_NONE, -1.0f);
                setMotion(MOT_WAIT_A, -1.0f, 0);
                mTurnMode = 0;
            }
            break;

        case 70:
            if (mMotion == MOT_TO_STONE_NORMAL) {
                if (mMotionPhase > 0) {
                    rv = 1;
                } else if (mAnm_p->checkFrame(14.0f)) {
                    cXyz sp70(0.0f, 0.0f, 20.0f);
                    mDoMtx_stack_c::transS(current.pos);
                    mDoMtx_stack_c::YrotM(mCurAngle.y);
                    mDoMtx_stack_c::multVec(&sp70, &mPrtclMngr[0].mPos);
                    mPrtclMngr[0].mAngle = mCurAngle;
                    mPrtclMngr[0].field_0x0 = 1;
                }
            }
            break;

        case 80:
        case 75:
            if (mPath.getPathInfo() != NULL) {
                cXyz sp7c;
                mPath.getDstPos(current.pos, sp7c);
                cLib_addCalcAngleS2(&mCurAngle.y, cLib_targetAngleY(&current.pos, &sp7c), 6, 0x400);
                setAngle(mCurAngle.y);
                f32 playSpeed = mAnm_p->getPlaySpeed();
                cLib_addCalc2(&playSpeed, 1.0f, 0.1f, 0.1f);
                mAnm_p->setPlaySpeed(playSpeed);
                speedF = daNpc_Grz_Param_c::m.rotation_movement_speed * playSpeed;
            }

            rv = 1;
            break;

        default:
            rv = 1;
            break;
    }

    if (mMotion == MOT_TO_STONE_NORMAL && mMotionPhase > 0 && mAnm_p->getPlaySpeed() >= 0.5f) {
        JPABaseEmitter* emitter = NULL;
        cXyz pos(current.pos);
        cXyz scale(1.2f, 1.2f, 1.2f);

        static u16 const l_prticles_id[3] = {
            dPa_RM(ID_ZF_S_ROLLINGG00_ROCK),
            dPa_RM(ID_ZF_S_ROLLINGG01_SMKTAIL),
            dPa_RM(ID_ZF_S_ROLLINGG02_SMKCRCL),
        };

        for (int i = 0; i < 3; i++) {
            if (i == 0) {
                field_0x1aa0[i] = dComIfGp_particle_set(field_0x1aa0[i], l_prticles_id[i], &pos, &tevStr, &current.angle, &scale, 0xFF, NULL, -1, NULL, NULL, NULL);
            } else {
                field_0x1aa0[i] = dComIfGp_particle_setPolyColor(field_0x1aa0[i], l_prticles_id[i], mAcch.m_gnd, &pos, &tevStr, &current.angle, &scale, 0, NULL, -1, NULL);
            }

            emitter = dComIfGp_particle_getEmitter(field_0x1aa0[i]);
            if (emitter != NULL) {
                emitter->setGlobalTranslation(pos.x, pos.y, pos.z);
            }
        }
    }

    return rv;
}

int daNpc_Grz_c::doStoneSmashSkipCut(int i_staffId) {
    dEvent_manager_c& eventManager = dComIfGp_getEventManager();
    int prm = -1;
    int* piVar1 = dComIfGp_evmng_getMyIntegerP(i_staffId, "prm");
    if (piVar1 != NULL) {
        prm = *piVar1;
    }

    if (eventManager.getIsAddvance(i_staffId)) {
        switch (prm) {
            case 10:
                dComIfGs_offStageSwitch(6, 64);
                setMotion(MOT_WAIT_A, -1.0f, 0);
                setExpression(EXPR_NONE, -1.0f);
                daPy_getPlayerActorClass()->setPlayerPosAndAngle(&home.pos, home.angle.y, 0);
                mHide = true;

                fopAc_ac_c* actor_p = mActorMngrs[3].getActorP();
                if (actor_p != NULL) {
                    fopAcM_delete(actor_p);
                }
                break;
        }
    }

    int rv;
    switch (prm) {
        case 10:
            rv = 1;
            break;

        default:
            rv = 1;
            break;
    }

    return rv;
}

static int daNpc_Grz_Create(void* a_this) {
    return static_cast<daNpc_Grz_c*>(a_this)->create();
}

static int daNpc_Grz_Delete(void* a_this) {
    return static_cast<daNpc_Grz_c*>(a_this)->Delete();
}

static int daNpc_Grz_Execute(void* a_this) {
    return static_cast<daNpc_Grz_c*>(a_this)->Execute();
}

static int daNpc_Grz_Draw(void* a_this) {
    return static_cast<daNpc_Grz_c*>(a_this)->Draw();
}

static int daNpc_Grz_IsDelete(void* a_this) {
    return 1;
}

AUDIO_INSTANCES;

static actor_method_class daNpc_Grz_MethodTable = {
    (process_method_func)daNpc_Grz_Create,
    (process_method_func)daNpc_Grz_Delete,
    (process_method_func)daNpc_Grz_Execute,
    (process_method_func)daNpc_Grz_IsDelete,
    (process_method_func)daNpc_Grz_Draw,
};

actor_process_profile_definition g_profile_NPC_GRZ = {
  fpcLy_CURRENT_e,               // mLayerID
  7,                             // mListID
  fpcPi_CURRENT_e,               // mListPrio
  PROC_NPC_GRZ,                  // mProcName
  &g_fpcLf_Method.base,         // sub_method
  sizeof(daNpc_Grz_c),           // mSize
  0,                             // mSizeOther
  0,                             // mParameters
  &g_fopAc_Method.base,          // sub_method
  314,                           // mPriority
  &daNpc_Grz_MethodTable,        // sub_method
  0x00044100,                    // mStatus
  fopAc_NPC_e,                   // mActorType
  fopAc_CULLBOX_CUSTOM_e,        // cullType
};
