/**
 * @file d_a_npc_bouS.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_npc_bouS.h"
#include "JSystem/JHostIO/JORFile.h"
#include "d/actor/d_a_tag_instruction.h"
#include "d/d_item.h"
#include "d/actor/d_a_npc_wrestler.h"
#include "Z2AudioLib/Z2Instances.h"
#include "d/d_camera.h"

enum Bou_RES_File_ID {
    /* BCK */
    /* 0x06 */ BCK_BOU_F_TALK_A = 0x6,
    /* 0x07 */ BCK_BOU_STEP,
    /* 0x08 */ BCK_BOU_WAIT_A,

    /* BMDR */
    /* 0x0B */ BMDR_BOU = 0xB,

    /* BTK */
    /* 0x0E */ BTK_BOU = 0xE,

    /* BTP */
    /* 0x11 */ BTP_BOU = 0x11,
};

enum Bou4_RES_File_ID {
    /* BCK */
    /* 0x05 */ BCK_BOU4_CONSIDER = 0x5,
    /* 0x06 */ BCK_BOU4_CONSIDERING,
    /* 0x07 */ BCK_BOU4_F_CONSIDER,
    /* 0x08 */ BCK_BOU4_F_CONSIDERING,
    /* 0x09 */ BCK_BOU4_F_TALK_A,
    /* 0x0A */ BCK_BOU4_F_TALK_B,
    /* 0x0B */ BCK_BOU4_FH_TALK_B,
    /* 0x0C */ BCK_BOU4_LOOKBACK,
    /* 0x0D */ BCK_BOU4_ORDER,
    /* 0x0E */ BCK_BOU4_STEP,
    /* 0x0F */ BCK_BOU4_TALK_B,
    /* 0x10 */ BCK_BOU4_TALK_C,
    /* 0x11 */ BCK_BOU4_THINKING_A,
    /* 0x12 */ BCK_BOU4_THINKING_B,
    /* 0x13 */ BCK_BOU4_UNADUKI,
    /* 0x14 */ BCK_BOU4_WAIT_A,
    /* 0x15 */ BCK_BOU4_WALK,
    /* 0x16 */ BCK_BOU4_WORRY_WAIT,

    /* BTP */
    /* 0x19 */ BTP_BOU4_F_CONSIDER = 0x19,
    /* 0x1A */ BTP_BOU4_F_CONSIDERING,
    /* 0x1B */ BTP_BOU4_F_TALK_B,
    /* 0x1C */ BTP_BOU4_FH_TALK_B,

    /* EVT */
    /* 0x1F */ EVT_BOU4_EVENT_LIST = 0x1F,
};

enum RES_Name {
    /* 0x0 */ BOU,
    /* 0x1 */ BOU4,
};

enum Animation {
    /* 0x00 */ ANM_NONE,
    /* 0x01 */ ANM_F_TALK_A,
    /* 0x02 */ ANM_F_TALK_A_2,
    /* 0x03 */ ANM_F_CONSIDER,
    /* 0x04 */ ANM_F_CONSIDERING,
    /* 0x05 */ ANM_F_TALK_B,
    /* 0x06 */ ANM_FH_TALK_B,
    /* 0x07 */ ANM_WAIT_A,
    /* 0x0A */ ANM_LOOKBACK = 0xA,
    /* 0x0B */ ANM_ORDER,
    /* 0x0C */ ANM_STEP,
    /* 0x0D */ ANM_TALK_B,
    /* 0x0E */ ANM_TALK_C,
    /* 0x0F */ ANM_UNADUKI,
    /* 0x10 */ ANM_WAIT_A_2,
    /* 0x11 */ ANM_WORRY_WAIT,
    /* 0x12 */ ANM_WALK,
    /* 0x13 */ ANM_CONSIDER,
    /* 0x14 */ ANM_CONSIDERING,
    /* 0x15 */ ANM_THINKING_A,
    /* 0x16 */ ANM_THINKING_B,
};

enum Expression_BTP {
    /* 0x0 */ EXPR_BTP_BOU,
    /* 0x1 */ EXPR_BTP_F_CONSIDER,
    /* 0x2 */ EXPR_BTP_F_CONSIDERING,
    /* 0x3 */ EXPR_BTP_F_TALK_B,
    /* 0x4 */ EXPR_BTP_FH_TALK_B,
};

enum Expression {
    /* 0x2 */ EXPR_TALK_A = 0x2,
    /* 0x3 */ EXPR_CONSIDER,
    /* 0x4 */ EXPR_TALK_B,
    /* 0x5 */ EXPR_NONE,
};

enum Motion {
    /* 0x06 */ MOT_WAIT_A = 0x6,
    /* 0x07 */ MOT_WORRY_WAIT,
    /* 0x09 */ MOT_TALK_C = 0x9,
    /* 0x0A */ MOT_UNADUKI,
    /* 0x0B */ MOT_LOOKBACK,
    /* 0x0C */ MOT_ORDER,
    /* 0x0D */ MOT_TALK_B,
    /* 0x0E */ MOT_CONSIDER,
    /* 0x0F */ MOT_CONSIDERING,
    /* 0x10 */ MOT_THINKING_A,
    /* 0x11 */ MOT_THINKING_B,
    /* 0x12 */ MOT_WALK,
    /* 0x13 */ MOT_STEP,
};

enum Joint {
    /* 0x00 */ JNT_CENTER,
    /* 0x01 */ JNT_BACKBONE1,
    /* 0x02 */ JNT_BACKBONE2,
    /* 0x03 */ JNT_NECK,
    /* 0x04 */ JNT_HEAD,
    /* 0x05 */ JNT_CHIN,
    /* 0x06 */ JNT_MAYU_L,
    /* 0x07 */ JNT_MAYU_R,
    /* 0x08 */ JNT_MOUTH,
    /* 0x09 */ JNT_SHOULDERL,
    /* 0x0A */ JNT_ARML1,
    /* 0x0B */ JNT_ARML2,
    /* 0x0C */ JNT_HANDL,
    /* 0x0D */ JNT_FINGERL1,
    /* 0x0E */ JNT_THAMBL,
    /* 0x0F */ JNT_SHOULDERR,
    /* 0x10 */ JNT_ARMR1,
    /* 0x11 */ JNT_ARMR2,
    /* 0x12 */ JNT_HANDR,
    /* 0x13 */ JNT_FINGERR1,
    /* 0x14 */ JNT_THAMBR,
    /* 0x15 */ JNT_SKIRT1,
    /* 0x16 */ JNT_SKIRT2,
    /* 0x17 */ JNT_SKIRTL1,
    /* 0x18 */ JNT_SKIRTL2,
    /* 0x19 */ JNT_SKIRTR1,
    /* 0x1A */ JNT_SKIRTR2,
    /* 0x1B */ JNT_WAIST,
    /* 0x1C */ JNT_LEGL1,
    /* 0x1D */ JNT_LEGL2,
    /* 0x1E */ JNT_FOOTL,
    /* 0x1F */ JNT_LEGR1,
    /* 0x20 */ JNT_LEGR2,
    /* 0x21 */ JNT_FOOTR,
};

enum Event {
    /* 0x0 */ EVENT_NONE,
    /* 0x1 */ EVENT_INTRO_SUMO1,
    /* 0x2 */ EVENT_INTRO_SUMO2,
    /* 0x3 */ EVENT_INTRO_SUMO3,
};

static daNpc_GetParam2 l_bckGetParamList[23] = {
    {-1, J3DFrameCtrl::EMode_LOOP, BOU},
    {BCK_BOU_F_TALK_A, J3DFrameCtrl::EMode_NONE, BOU},
    {BCK_BOU4_F_TALK_A, J3DFrameCtrl::EMode_NONE, BOU4},
    {BCK_BOU4_F_CONSIDER, J3DFrameCtrl::EMode_NONE, BOU4},
    {BCK_BOU4_F_CONSIDERING, J3DFrameCtrl::EMode_LOOP, BOU4},
    {BCK_BOU4_F_TALK_B, J3DFrameCtrl::EMode_NONE, BOU4},
    {BCK_BOU4_FH_TALK_B, J3DFrameCtrl::EMode_LOOP, BOU4},
    {BCK_BOU_WAIT_A, J3DFrameCtrl::EMode_LOOP, BOU},
    {-1, J3DFrameCtrl::EMode_LOOP, BOU},
    {BCK_BOU_STEP, J3DFrameCtrl::EMode_NONE, BOU},
    {BCK_BOU4_LOOKBACK, J3DFrameCtrl::EMode_NONE, BOU4},
    {BCK_BOU4_ORDER, J3DFrameCtrl::EMode_NONE, BOU4},
    {BCK_BOU4_STEP, J3DFrameCtrl::EMode_NONE, BOU4},
    {BCK_BOU4_TALK_B, J3DFrameCtrl::EMode_NONE, BOU4},
    {BCK_BOU4_TALK_C, J3DFrameCtrl::EMode_NONE, BOU4},
    {BCK_BOU4_UNADUKI, J3DFrameCtrl::EMode_NONE, BOU4},
    {BCK_BOU4_WAIT_A, J3DFrameCtrl::EMode_LOOP, BOU4},
    {BCK_BOU4_WORRY_WAIT, J3DFrameCtrl::EMode_LOOP, BOU4},
    {BCK_BOU4_WALK, J3DFrameCtrl::EMode_LOOP, BOU4},
    {BCK_BOU4_CONSIDER, J3DFrameCtrl::EMode_NONE, BOU4},
    {BCK_BOU4_CONSIDERING, J3DFrameCtrl::EMode_LOOP, BOU4},
    {BCK_BOU4_THINKING_A, J3DFrameCtrl::EMode_NONE, BOU4},
    {BCK_BOU4_THINKING_B, J3DFrameCtrl::EMode_LOOP, BOU4},
};

static daNpc_GetParam2 l_btpGetParamList[5] = {
    {BTP_BOU, J3DFrameCtrl::EMode_LOOP, BOU},
    {BTP_BOU4_F_CONSIDER, J3DFrameCtrl::EMode_NONE, BOU4},
    {BTP_BOU4_F_CONSIDERING, J3DFrameCtrl::EMode_LOOP, BOU4},
    {BTP_BOU4_F_TALK_B, J3DFrameCtrl::EMode_NONE, BOU4},
    {BTP_BOU4_FH_TALK_B, J3DFrameCtrl::EMode_LOOP, BOU4},
};

static daNpc_GetParam2 l_btkGetParamList[1] = {
    {BTK_BOU, J3DFrameCtrl::EMode_LOOP, BOU},
};

static char* l_arcNames[2] = {
    "Bou",
    "Bou4",
};

static char* l_evtNames[4] = {
    NULL,
    "BOUS_INTRO_SUMO1",
    "BOUS_INTRO_SUMO2",
    "BOUS_INTRO_SUMO3",
};

static char* l_myName = "BouS";

static cXyz l_createPos(1000000000.0f, 1000000000.0f, 1000000000.0f);

static NPC_BOUS_HIO_CLASS l_HIO;

#if DEBUG
daNpcBouS_HIO_c::daNpcBouS_HIO_c() {
    m = daNpcBouS_Param_c::m;
}

void daNpcBouS_HIO_c::listenPropertyEvent(const JORPropertyEvent* event) {
    char buffer[2000];
    
    JORReflexible::listenPropertyEvent(event);
    JORFile jorFile;
    int length;

    switch ((int)event->id) {
        case 0x40000002:
            if (jorFile.open(6, "", NULL, NULL, NULL)) {
                memset(buffer, 0, sizeof(buffer));
                length = 0;
                daNpcF_commonListenPropertyEvent(buffer, &length, &m.common);
                length = strlen(buffer);
                jorFile.writeData(buffer, length);
                jorFile.close();
                OS_REPORT("write append success!::%6d\n", length);
            } else {
                OS_REPORT("write append failure!\n");
            }
    }
}

void daNpcBouS_HIO_c::genMessage(JORMContext* ctx) {
    ctx->genSlider("興奮度", &m.excitement_level, 0.0f, 1.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    ctx->genSlider("会話距離", &m.talk_dist, 0.0f, 500.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    ctx->genSlider("目線 X", &m.gaze_x, -500.0f, 500.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    ctx->genSlider("目線 Y", &m.gaze_y, -500.0f, 500.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    ctx->genSlider("目線 Z", &m.gaze_z, -500.0f, 500.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    ctx->genSlider("Eye 距離", &m.eye_dist, 0.0f, 3000.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    ctx->genSlider("Eye 角度X", &m.eye_angle_x, -0x8000, 0x7FFF, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    ctx->genSlider("Eye 角度Y", &m.eye_angle_y, -0x8000, 0x7FFF, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
}
#endif

daNpcBouS_c::eventFunc daNpcBouS_c::mEvtSeqList[4] = {
    NULL,
    &daNpcBouS_c::EvCut_BousIntroSumo1,
    &daNpcBouS_c::EvCut_BousIntroSumo2,
    &daNpcBouS_c::EvCut_BousIntroSumo3,
};

daNpcBouS_c::daNpcBouS_c() {}

daNpcBouS_c::~daNpcBouS_c() {
    for (int i = 0; i < 2; i++) {
        dComIfG_resDelete(&mPhases[i], l_arcNames[i]);
    }

    if (heap != NULL) {
        mAnm_p->stopZelAnime();
    }

    #if DEBUG
    if (mHIO != NULL) {
        mHIO->removeHIO();
    }
    #endif
}

daNpcBouS_HIOParam const daNpcBouS_Param_c::m = {
    220.0f,
    -3.0f,
    1.0f,
    400.0f,
    255.0f,
    200.0f,
    35.0f,
    40.0f,
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
    0x13,
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
    0.0f,
    220.0f,
    0.0f,
    100.0f,
    80.0f,
    654.78497f,
    -0x14AF,
    -0x5C3A,
    0,
};

cPhs__Step daNpcBouS_c::Create() {
    fopAcM_ct(this, daNpcBouS_c);

    mMsgNo = getMessageNo();

    cPhs__Step phase = cPhs_ERROR_e;
    for (int i = 0; i < 2; i++) {
        phase = (cPhs__Step)dComIfG_resLoad(&mPhases[i], l_arcNames[i]);

        if (phase != cPhs_COMPLEATE_e) {
            return phase;
        }
    }

    if (phase == cPhs_COMPLEATE_e) {
        if (!fopAcM_entrySolidHeap(this, createHeapCallBack, 0x3D40)) {
            return cPhs_ERROR_e;
        }

        J3DModelData* mdlData_p = mAnm_p->getModel()->getModelData();
        fopAcM_SetMtx(this, mAnm_p->getModel()->getBaseTRMtx());
        fopAcM_setCullSizeBox(this, -160.0f, -50.0f, -160.0f, 160.0f, 220.0f, 160.0f);
        mSound.init(&current.pos, &eyePos, 3, 1);

        #if DEBUG
        mHIO = &l_HIO;
        mHIO->entryHIO("ボウ（着衣）");
        #endif

        mAcchCir.SetWall(mHIO->m.common.width, mHIO->m.common.knee_length);
        mAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1, &mAcchCir, fopAcM_GetSpeed_p(this),
                  fopAcM_GetAngle_p(this), fopAcM_GetShapeAngle_p(this));
        mCcStts.Init(mHIO->m.common.weight, 0, this);
        mCyl.Set(mCcDCyl);
        mCyl.SetStts(&mCcStts);
        mAcch.CrrPos(dComIfG_Bgsp());
        mGndChk = mAcch.m_gnd;
        mGroundH = mAcch.GetGroundH();
        
        setEnvTevColor();
        setRoomNo();
        reset();
        Execute();
    }

    return phase;
}

int daNpcBouS_c::CreateHeap() {
    J3DModelData* mdlData_p = NULL;
    J3DModel* mdl_p = NULL;
    
    mdlData_p = (J3DModelData*)dComIfG_getObjectRes(l_arcNames[0], BMDR_BOU);
    JUT_ASSERT(393, NULL != mdlData_p);

    u32 i_differedDlistFlag = 0x11020284;
    mAnm_p = new mDoExt_McaMorfSO(mdlData_p, NULL, NULL, NULL, -1, 1.0f, 0, -1, &mSound, J3DMdlFlag_DifferedDLBuffer, i_differedDlistFlag);
    if (mAnm_p != NULL && mAnm_p->getModel() == NULL) {
        mAnm_p->stopZelAnime();
        mAnm_p = NULL;
    }

    if (mAnm_p == NULL) {
        return 0;
    }

    mdl_p = mAnm_p->getModel();
    for (u16 i = 0; i <mdlData_p->getJointNum(); i++) {
        mdlData_p->getJointNodePointer(i)->setCallBack(ctrlJointCallBack);
    }
    mdl_p->setUserArea((uintptr_t)this);

    mpMatAnm = new daNpcF_MatAnm_c();
    if (mpMatAnm == NULL) {
        return 0;
    }

    if (!setExpressionAnm(ANM_F_TALK_A, false)) {
        return 0;
    }

    setMotionAnm(ANM_WAIT_A, 0.0f);
    return 1;
}

int daNpcBouS_c::Delete() {
    fpc_ProcID id = fopAcM_GetID(this);
    this->~daNpcBouS_c();
    return 1;
}

int daNpcBouS_c::Execute() {
    execute();
    return 1;
}

int daNpcBouS_c::Draw() {
    if (!mDispFlag) {
        return 1;
    }

    J3DModelData* mdlData_p = mAnm_p->getModel()->getModelData();
    mdlData_p->getMaterialNodePointer(1)->setMaterialAnm(mpMatAnm);
    draw(FALSE, FALSE, mHIO->m.common.real_shadow_size, NULL, FALSE);
    return 1;
}

int daNpcBouS_c::ctrlJoint(J3DJoint* i_joint, J3DModel* i_model) {
    int jntNo = i_joint->getJntNo();
    int lookatJoints[3] = {JNT_BACKBONE1, JNT_NECK, JNT_HEAD};

    if (jntNo == 0) {
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
            setLookatMtx(jntNo, lookatJoints, mHIO->m.common.neck_rotation_ratio);
            break;
    }

    if (jntNo == JNT_BACKBONE1) {
        mDoMtx_stack_c::YrotM(field_0x908[0].z);
        mDoMtx_stack_c::ZrotM(-field_0x908[0].x);
    } else if (jntNo == JNT_HEAD) {
        mDoMtx_stack_c::YrotM(field_0x908[2].z);
        mDoMtx_stack_c::ZrotM(field_0x908[2].x);
    }

    i_model->setAnmMtx(jntNo, mDoMtx_stack_c::get());

    #if DEBUG
    cMtx_copy(mDoMtx_stack_c::get(), J3DSys::mCurrentMtx);
    #else
    MTXCopy(mDoMtx_stack_c::get(), J3DSys::mCurrentMtx);
    #endif

    if ((jntNo == JNT_HEAD || jntNo == JNT_MOUTH) && (mAnmFlags & ANM_PLAY_BCK)) {
        J3DAnmTransform* anm = mBckAnm.getBckAnm();
        mBckAnm.changeBckOnly(mAnm_p->getAnm());
        mAnm_p->changeAnm(anm);
    }

    return 1;
}

int daNpcBouS_c::createHeapCallBack(fopAc_ac_c* i_this) {
    daNpcBouS_c* actor = (daNpcBouS_c*)i_this;
    return actor->CreateHeap();
}

int daNpcBouS_c::ctrlJointCallBack(J3DJoint* i_joint, int param_2) {
    if (param_2 == 0) {
        J3DModel* mdl_p = j3dSys.getModel();
        daNpcBouS_c* actor = (daNpcBouS_c*)mdl_p->getUserArea();
        if (actor != NULL) {
            actor->ctrlJoint(i_joint, mdl_p);
        }
    }

    return 1;
}

bool daNpcBouS_c::setExpressionAnm(int i_idx, bool i_modify) {
    mAnmFlags &= ~ANM_EXPRESSION_FLAGS;

    J3DAnmTransform* bckAnm;
    
    if (l_bckGetParamList[i_idx].fileIdx >= 0) {
        bckAnm = getTrnsfrmKeyAnmP(l_arcNames[l_bckGetParamList[i_idx].arcIdx], l_bckGetParamList[i_idx].fileIdx);
    } else {
        bckAnm = NULL;
    }

    s32 i_attr = l_bckGetParamList[i_idx].attr;
    bool res = false;

    switch (i_idx) {
        case ANM_NONE:
            res = setExpressionBtp(EXPR_BTP_BOU);
            break;

        case ANM_F_TALK_A:
            res = setExpressionBtp(EXPR_BTP_BOU);
            break;

        case ANM_F_TALK_A_2:
            res = setExpressionBtp(EXPR_BTP_BOU);
            break;

        case ANM_F_CONSIDER:
            res = setExpressionBtp(EXPR_BTP_F_CONSIDER);
            break;

        case ANM_F_CONSIDERING:
            res = setExpressionBtp(EXPR_BTP_F_CONSIDERING);
            break;

        case ANM_F_TALK_B:
            res = setExpressionBtp(EXPR_BTP_F_TALK_B);
            break;

        case ANM_FH_TALK_B:
            res = setExpressionBtp(EXPR_BTP_FH_TALK_B);
            break;

        default:
            bckAnm = NULL;
            break;
    }

    if (!res) {
        return false;
    }

    if (bckAnm == NULL) {
        return true;
    }

    if (setBckAnm(bckAnm, 1.0f, i_attr, 0, -1, i_modify)) {
        mAnmFlags |= (ANM_PAUSE_BCK | ANM_PLAY_BCK);
        mExpressionLoops = 0;
        return true;
    }

    OS_REPORT("%s: 表情Bckアニメーションの登録に失敗しました！\n", __FILE__);
    return false;
}

bool daNpcBouS_c::setExpressionBtp(int i_idx) {
    J3DAnmTexPattern* btp = getTexPtrnAnmP(l_arcNames[l_btpGetParamList[i_idx].arcIdx], l_btpGetParamList[i_idx].fileIdx);
    s32 i_attr = l_btpGetParamList[i_idx].attr;

    mAnmFlags &= ~(ANM_PAUSE_BTP | ANM_PLAY_BTP | ANM_FLAG_800);

    if (btp == NULL) {
        return true;
    }

    if (setBtpAnm(btp, mAnm_p->getModel()->getModelData(), 1.0f, i_attr)) {
        mAnmFlags |= ANM_PAUSE_BTP | ANM_PLAY_BTP;

        if (i_idx == EXPR_BTP_BOU) {
            mAnmFlags |= ANM_FLAG_800;
        }

        return true;
    }

    OS_REPORT("%s: 表情Btpアニメーションの登録に失敗しました！\n", __FILE__);
    return false;
}

void daNpcBouS_c::setMotionAnm(int i_idx, f32 i_morf) {
    int btk_idx;
    switch (i_idx) {
        default:
            btk_idx = 0;
            break;
    }
    
    J3DAnmTransformKey* morfAnm = getTrnsfrmKeyAnmP(l_arcNames[l_bckGetParamList[i_idx].arcIdx], l_bckGetParamList[i_idx].fileIdx);
    J3DAnmTextureSRTKey* btk = getTexSRTKeyAnmP(l_arcNames[l_btkGetParamList[btk_idx].arcIdx], l_btkGetParamList[btk_idx].fileIdx);
    int morfAttr = l_bckGetParamList[i_idx].attr;
    int btkAttr = l_btkGetParamList[btk_idx].attr;

    mAnmFlags &= ~ANM_MOTION_FLAGS;

    if (morfAnm != NULL && setMcaMorfAnm(morfAnm, 1.0f, i_morf, morfAttr, 0, -1)) {
        mAnmFlags |= ANM_PLAY_MORF | ANM_PAUSE_MORF;
        mMotionLoops = 0;
    }

    if (btk != NULL && setBtkAnm(btk, mAnm_p->getModel()->getModelData(), 1.0f, btkAttr)) {
        mAnmFlags |= ANM_PAUSE_BTK | ANM_PLAY_BTK;
    }
}

void daNpcBouS_c::reset() {
    initialize();
    mpMatAnm->initialize();
    mLookat.initialize();
    for (int i = 0; i < 1; i++) {
        mActorMngrs[i].initialize();
    }

    mTimer = 0;
    mInstructionMode = 0;
    mAction = NULL;
    mLookMode = -1;
    mMode = 0;
    mDispFlag = 1;
    current.pos.set(home.pos);
    old.pos.set(current.pos);
    current.angle.set(0, home.angle.y, 0);
    old.angle = current.angle;
    shape_angle = current.angle;
    mCurAngle = current.angle;
    mOldAngle = mCurAngle;
    speedF = 0.0f;
    speed.setall(0.0f);
    mItemPartnerId = fpcM_ERROR_PROCESS_ID_e;
    mOrderEvtNo = EVENT_NONE;

    if (checkIntroDemoStart()) {
        setAction(&daNpcBouS_c::demo);
    } else {
        setAction(&daNpcBouS_c::wait);
    }

    mExpressionMorfOverride = 0.0f;
    mMotionMorfOverride = 0.0f;
}

BOOL daNpcBouS_c::setAction(actionFunc action) {
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

static void* s_sub(void* i_actor, void* i_data) {
    if (
        fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_Tag_Instruction &&
        ((daNpcBouS_c*)i_data)->checkInstructionTag((fopAc_ac_c*)i_actor)
    ) {
        return i_actor;
    }

    return NULL;
}

bool daNpcBouS_c::checkInstructionTag(fopAc_ac_c* i_this) {
    daTagInst_c* actor = (daTagInst_c*)i_this;
    if (getArenaNo() == actor->getInstructionNo()) {
        return true;
    }

    return false;
}

void daNpcBouS_c::setParam() {
    s16 attention_distance = 5;
    s16 attention_angle = 6;
    s16 talk_distance = 3;
    s16 talk_angle = 6;

    attention_info.distances[fopAc_attn_LOCK_e] = getDistTableIdx(attention_distance, attention_angle);
    attention_info.distances[fopAc_attn_TALK_e] = attention_info.distances[fopAc_attn_LOCK_e];
    attention_info.distances[fopAc_attn_SPEAK_e] = getDistTableIdx(talk_distance, talk_angle);
    #if PLATFORM_SHIELD
    attention_info.flags = fopAc_AttnFlag_SPEAK_e | fopAc_AttnFlag_TALK_e;
    #else
    attention_info.flags = mMsgNo > 0 ? fopAc_AttnFlag_SPEAK_e | fopAc_AttnFlag_TALK_e : 0;
    #endif
    mAcchCir.SetWallR(mHIO->m.common.width);
    mAcchCir.SetWallH(mHIO->m.common.knee_length);
    gravity = mHIO->m.common.gravity;
}

BOOL daNpcBouS_c::main() {
    if (mAction) {
        (this->*mAction)(NULL);
    }

    if (dComIfGp_event_runCheck() && !eventInfo.checkCommandTalk() && mItemPartnerId != fpcM_ERROR_PROCESS_ID_e) {
        dComIfGp_event_setItemPartnerId(mItemPartnerId);
        mItemPartnerId = fpcM_ERROR_PROCESS_ID_e;
    }

    playExpression();
    playMotion();
    return TRUE;
}

void daNpcBouS_c::playExpression() {
    daNpcF_anmPlayData dat0 = {ANM_F_TALK_A, mHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat0[1] = {&dat0};
    daNpcF_anmPlayData dat1 = {ANM_F_TALK_A_2, mHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat1[1] = {&dat1};
    daNpcF_anmPlayData dat2a = {ANM_F_CONSIDER, mHIO->m.common.morf_frame, 1};
    daNpcF_anmPlayData dat2b = {ANM_F_CONSIDERING, mHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat2[2] = {&dat2a, &dat2b};
    daNpcF_anmPlayData dat3a = {ANM_F_TALK_B, mHIO->m.common.morf_frame, 1};
    daNpcF_anmPlayData dat3b = {ANM_FH_TALK_B, mHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat3[2] = {&dat3a, &dat3b};
    daNpcF_anmPlayData dat4 = {ANM_NONE, mHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat4[1] = {&dat4};

    daNpcF_anmPlayData** ppDat[6] = {
        pDat0,
        NULL,
        pDat1,
        pDat2,
        pDat3,
        pDat4,
    };

    if (mExpression >= 0 && mExpression < 6) {
        playExpressionAnm(ppDat);
    }
}

void daNpcBouS_c::playMotion() {
    daNpcF_anmPlayData dat0 = {ANM_WAIT_A, mHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat0[1] = {&dat0};
    daNpcF_anmPlayData dat1 = {ANM_WAIT_A_2, mHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat1[1] = {&dat1};
    daNpcF_anmPlayData dat2 = {ANM_WORRY_WAIT, mHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat2[1] = {&dat2};
    daNpcF_anmPlayData dat3a = {ANM_TALK_C, mHIO->m.common.morf_frame, 1};
    daNpcF_anmPlayData dat3b = {ANM_WAIT_A_2, mHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat3[2] = {&dat3a, &dat3b};
    daNpcF_anmPlayData dat4a = {ANM_UNADUKI, mHIO->m.common.morf_frame, 1};
    daNpcF_anmPlayData dat4b = {ANM_WORRY_WAIT, mHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat4[2] = {&dat4a, &dat4b};
    daNpcF_anmPlayData dat5a = {ANM_LOOKBACK, mHIO->m.common.morf_frame, 1};
    daNpcF_anmPlayData dat5b = {ANM_WAIT_A_2, mHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat5[2] = {&dat5a, &dat5b};
    daNpcF_anmPlayData dat6a = {ANM_ORDER, mHIO->m.common.morf_frame, 1};
    daNpcF_anmPlayData dat6b = {ANM_WAIT_A_2, mHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat6[2] = {&dat6a, &dat6b};
    daNpcF_anmPlayData dat7a = {ANM_TALK_B, mHIO->m.common.morf_frame, 1};
    daNpcF_anmPlayData dat7b = {ANM_WAIT_A_2, mHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat7[2] = {&dat7a, &dat7b};
    daNpcF_anmPlayData dat8a = {ANM_CONSIDER, mHIO->m.common.morf_frame, 1};
    daNpcF_anmPlayData dat8b = {ANM_CONSIDERING, mHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat8[2] = {&dat8a, &dat8b};
    daNpcF_anmPlayData dat9 = {ANM_CONSIDERING, mHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat9[1] = {&dat9};
    daNpcF_anmPlayData dat10a = {ANM_THINKING_A, mHIO->m.common.morf_frame, 1};
    daNpcF_anmPlayData dat10b = {ANM_THINKING_B, mHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat10[2] = {&dat10a, &dat10b};
    daNpcF_anmPlayData dat11 = {ANM_THINKING_B, mHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat11[1] = {&dat11};
    daNpcF_anmPlayData dat12 = {ANM_WALK, mHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat12[1] = {&dat12};
    daNpcF_anmPlayData dat13 = {ANM_STEP, 4.0f, 1};
    daNpcF_anmPlayData* pDat13[1] = {&dat13};

    daNpcF_anmPlayData** ppDat[20] = {
        pDat0,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        pDat1,
        pDat2,
        NULL,
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
    };

    if (mMotion >= 0 && mMotion < 0x14) {
        mMotionPhaseCopy = mMotionPhase;
        playMotionAnm(ppDat);
    }
}

BOOL daNpcBouS_c::ctrlBtk() {
    if (mpMatAnm != NULL) {
        J3DAnmTextureSRTKey* btk = NULL;
        btk = getTexSRTKeyAnmP(l_arcNames[l_btkGetParamList[0].arcIdx], l_btkGetParamList[0].fileIdx);
        if (btk == mBtkAnm.getBtkAnm()) {
            mpMatAnm->setNowOffsetX(cM_ssin(mEyeAngle.y) * 0.2f * -1.0f);
            mpMatAnm->setNowOffsetY(cM_ssin(mEyeAngle.x) * 0.2f);
            mpMatAnm->onEyeMoveFlag();
            return TRUE;
        }

        mpMatAnm->offEyeMoveFlag();
    }

    return FALSE;
}

void daNpcBouS_c::setAttnPos() {
    static cXyz eyeOffset(-10.0f, 10.0f, 0.0f);

    cXyz* attnPos = NULL;
    cXyz sp44, sp50, sp5c;

    mDoMtx_stack_c::YrotS(field_0x990);
    cLib_addCalc2(&field_0x984[0], 0.0f, 0.1f, 125.0f);
    cLib_addCalc2(&field_0x984[2], 0.0f, 0.1f, 125.0f);

    for (int i = 0; i < 3; i++) {
        sp50.set(0.0f, 0.0f, field_0x984[i] * cM_ssin(field_0x992));
        mDoMtx_stack_c::multVec(&sp50, &sp5c);
        field_0x908[i].x = -sp5c.z;
        field_0x908[i].z = -sp5c.x;
    }

    cLib_chaseS(&field_0x992, 0, 0x555);

    if (mLookMode == LOOK_RESET) {
        for (int i = 0; i < 3; i++) {
            mLookatAngle[i].setall(0);
        }
    }

    setMtx();
    lookat();

    mDoMtx_stack_c::copy(mAnm_p->getModel()->getAnmMtx(JNT_HEAD));
    mDoMtx_stack_c::multVecZero(&mHeadPos);
    mDoMtx_stack_c::multVec(&eyeOffset, &eyePos);
    eyeOffset.y = 0.0f;
    mDoMtx_stack_c::multVec(&eyeOffset, &sp50);

    mHeadAngle.x = cLib_targetAngleX(&mHeadPos, &sp50);
    mHeadAngle.y = cLib_targetAngleY(&mHeadPos, &sp50);

    attnPos = mLookat.getAttnPos();
    if (attnPos != NULL) {
        sp44 = *attnPos - eyePos;
        mEyeAngle.y = -(mLookatAngle[2].y + mCurAngle.y);
        mEyeAngle.y += cM_atan2s(sp44.x, sp44.z);
        mEyeAngle.x = mHeadAngle.x - cM_atan2s(sp44.y, sp44.absXZ());
    } else {
        mEyeAngle.y = 0;
        mEyeAngle.x = 0;
    }

    cXyz sp68;
    f32 attnOffset = mHIO->m.common.attention_offset;
    sp44.set(0.0f, 0.0f, 0.0f);
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::ZXYrotM(mCurAngle.x, mCurAngle.y, mCurAngle.z);
    mDoMtx_stack_c::multVec(&sp44, &sp68);
    attention_info.position.set(sp68.x, sp68.y + attnOffset, sp68.z);
    mCyl.SetTgType(0);
    mCyl.SetTgSPrm(0);
    mCyl.SetC(sp68);
    mCyl.SetH(mHIO->m.common.height);
    mCyl.SetR(mHIO->m.common.width);
    dComIfG_Ccsp()->Set(&mCyl);
}

void daNpcBouS_c::lookat() {
    daPy_py_c* player = NULL;
    J3DModel* mdl_p = mAnm_p->getModel();
    BOOL i_snap = FALSE;
    f32 body_angleX_min = mHIO->m.common.body_angleX_min;
    f32 body_angleX_max = mHIO->m.common.body_angleX_max;
    f32 body_angleY_min = mHIO->m.common.body_angleY_min;
    f32 body_angleY_max = mHIO->m.common.body_angleY_max;
    f32 head_angleX_min = mHIO->m.common.head_angleX_min;
    f32 head_angleX_max = mHIO->m.common.head_angleX_max;
    f32 head_angleY_min = mHIO->m.common.head_angleY_min;
    f32 head_angleY_max = mHIO->m.common.head_angleY_max;
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
    }

    if (player != NULL) {
        mLookPos = player->attention_info.position;

        if (mLookMode != LOOK_PLAYER && mLookMode != LOOK_PLAYER_TALK) {
            mLookPos.y -= 40.0f;
        }

        mLookat.setAttnPos(&mLookPos);
    } else {
        mLookat.setAttnPos(NULL);
    }
    
    mLookat.setParam(body_angleX_min, body_angleX_max, body_angleY_min, body_angleY_max, 0.0f, 0.0f, 0.0f, 0.0f, 
                     head_angleX_min, head_angleX_max, head_angleY_min, head_angleY_max, mCurAngle.y, lookatPos);
    mLookat.calc(this, mdl_p->getBaseTRMtx(), lookatAngle, i_snap, angle_delta, FALSE);
}

BOOL daNpcBouS_c::step(s16 i_turnTargetAngle, int param_2) {
    if (mTurnMode == 0) {
        if (param_2 != 0) {
            if ((int)fabsf(cM_sht2d((s16)(i_turnTargetAngle - mCurAngle.y))) > 40) {
                setExpression(EXPR_NONE, -1.0f);
                setMotion(MOT_STEP, -1.0f, 0);
            }
        }

        mTurnTargetAngle = i_turnTargetAngle;
        mTurnAmount = 0;

        if (mCurAngle.y == mTurnTargetAngle) {
            mTurnMode++;
        }

        current.angle.y = mCurAngle.y;
        shape_angle.y = current.angle.y;
        mTurnMode++;
    } else if (mTurnMode == 1) {
        if (turn(mTurnTargetAngle, 15.0f, 0)) {
            shape_angle.y = current.angle.y;
            mCurAngle.y = current.angle.y;
            mOldAngle.y = current.angle.y;
            mTurnMode++;
        } else {
            shape_angle.y = current.angle.y;
            mCurAngle.y = current.angle.y;
        }
    }

    return mTurnMode > 1;
}

bool daNpcBouS_c::wait(void* param_1) {
    switch (mMode) {
        case 0:
            setExpression(EXPR_NONE, -1.0f);
            setMotion(MOT_WAIT_A, -1.0f, 0);
            mTurnMode = 0;
            speedF = 0.0f;
            mMode = 2;
            break;
        
        case 2:
            if (mActorMngrs[0].getActorP() != NULL) {
                if (!chkFindPlayer()) {
                    mTurnMode = 0;
                }
            } else if (chkFindPlayer()) {
                mTurnMode = 0;
            }

            if (mActorMngrs[0].getActorP() != NULL) {
                setLookMode(LOOK_PLAYER);
            } else {
                setLookMode(LOOK_NONE);
                if (home.angle.y != mCurAngle.y && step(home.angle.y, 1)) {
                    setExpression(EXPR_NONE, -1.0f);
                    setMotion(MOT_WAIT_A, -1.0f, 0);
                    mTurnMode = 0;
                }
            }

            if (!checkItemGet(fpcNm_ITEM_HVY_BOOTS, 1) && dComIfGs_isTbox(2)) {
                mForcibleTalk = 1;
            }

            if (dComIfGp_event_runCheck()) {
                if (eventInfo.checkCommandTalk()) {
                    if (!dComIfGp_event_chkTalkXY() || dComIfGp_evmng_ChkPresentEnd()) {
                        if (daNpcF_chkEvtBit(231)) { // dSv_event_flag_c::F_0231 - Ordon Village - Did first wrestle match with Bo
                            setAction(&daNpcBouS_c::talk);
                        } else {
                            mOrderEvtNo = EVENT_INTRO_SUMO2;
                            changeEvent("Bou4", l_evtNames[mOrderEvtNo], 1, 0xFFFF);
                            setAction(&daNpcBouS_c::demo);
                        }
                    }
                } else {
                    s32 staffId = dComIfGp_getEventManager().getMyStaffId(l_myName, NULL, 0);
                    if (staffId != -1) {
                        setAction(&daNpcBouS_c::demo);
                    }
                }
            } else {
                if (mOrderEvtNo != EVENT_NONE) {
                    eventInfo.setArchiveName("Bou4");
                }

                orderEvent(mForcibleTalk, l_evtNames[mOrderEvtNo], 0xFFFF, 40, 0xFF, 1);
            }
            break;

        case 3:
            break;

        default:
            JUT_ASSERT(1527, FALSE);
            break;
    }

    return true;
}

void daNpcBouS_c::setMotion(int i_motion, f32 i_morf, int param_3) {
    s16 motion = i_motion;
    if ((param_3 != 0 || mMotion != motion) && i_motion >= 0 && i_motion < 20) {
        mMotion = motion;
        mMotionMorfOverride = i_morf;
        mMotionPrevPhase = -1;
        mMotionPhase = 0;
        mMotionPhaseCopy = 0;
    }
}

void daNpcBouS_c::setExpression(int i_expression, f32 i_morf) {
    if (i_expression >= 0 && i_expression < 6) {
        mExpression = i_expression;
        mExpressionMorfOverride = i_morf;
        mExpressionPrevPhase = -1;
        mExpressionPhase = 0;
    }
}

void daNpcBouS_c::setExpressionTalkAfter() {
    switch (mExpression) {
        case EXPR_CONSIDER:
            setExpressionAnm(ANM_F_CONSIDERING, true);
            break;
        
        case EXPR_TALK_B:
            setExpressionAnm(ANM_FH_TALK_B, true);
            break;

        default:
            setExpression(ANM_F_TALK_B, -1.0f);
            break;
    }
}

bool daNpcBouS_c::talk(void* param_1) {
    int itemNo;
    bool rv = false;

    switch (mMode) {
        case 0:
            initTalk(mMsgNo, NULL);
            mTurnMode = 0;
            mMsgTimer = 0;
            mForcibleTalk = 0;
            speedF = 0.0f;
            setLookMode(LOOK_PLAYER_TALK);
            mMode = 2;

            if (mMsgNo != getMessageNo()) {
                mMsgNo = getMessageNo();
            }
            break;
        
        case 2:
            if (mCurAngle.y == fopAcM_searchPlayerAngleY(this)) {
                if (talkProc(NULL, TRUE, NULL)) {
                    mActorMngrs[0].entry(daPy_getPlayerActorClass());
                    itemNo = 0;
                    u16 eventId = mFlow.getEventId(&itemNo);
                    OS_REPORT("会話終了時 イベントID=%d アイテムNo=%d\n", eventId, itemNo);

                    if (eventId == 6) {
                        if (itemNo == 1) {
                            setAction(&daNpcBouS_c::instruction);
                        } else {
                            parentActorID = fopAcM_createChild(PROC_NPC_WRESTLER, fopAcM_GetID(this),
                                               (itemNo << 24) | (getArenaNo() | 0x700), &l_createPos, fopAcM_GetRoomNo(this),
                                               NULL, NULL, getType(), NULL);
                            setAngle(home.angle.y);
                            setAction(&daNpcBouS_c::wait);
                            mMsgNo = 0;
                            attention_info.flags = 0;
                            eventInfo.offCondition(dEvtCnd_CANTALK_e);
                        }
                    } else {
                        setAction(&daNpcBouS_c::wait);
                    }

                    rv = 1;
                } else {
                    int i_expression, i_motion;
                    int msgTimer = mMsgTimer;

                    if (ctrlMsgAnm(i_expression, i_motion, this, FALSE)) {
                        setExpression(i_expression, -1.0f);
                        setMotion(i_motion, -1.0f, 0);
                    } else if (msgTimer != 0 && mMsgTimer == 0) {
                        setExpressionTalkAfter();
                    }
                }
            } else if (step(fopAcM_searchPlayerAngleY(this), 1)) {
                setMotion(MOT_WAIT_A, -1.0f, 0);
                mTurnMode = 0;
            }
            break;

        case 3:
            setExpression(EXPR_NONE, -1.0f);

            if (!field_0x9ec) {
                dComIfGp_event_reset();
            }
            break;

        default:
            JUT_ASSERT(1614, FALSE);
            break;
    }

    return rv;
}

bool daNpcBouS_c::demo(void* param_1) {
    dEvent_manager_c& eventManager = dComIfGp_getEventManager();
    s32 staffId = 0;

    switch (mMode) {
        case 0:
            setExpression(EXPR_NONE, -1.0f);
            setMotion(MOT_WAIT_A, -1.0f, 0);
            mMode = 2;
            // fallthrough
        case 2:
            if (dComIfGp_event_runCheck() && !eventInfo.checkCommandTalk()) {
                dEvent_manager_c& eventManager = dComIfGp_getEventManager();
                staffId = eventManager.getMyStaffId(l_myName, NULL, 0);
                if (staffId != -1) {
                    mStaffID = staffId;
                    JUT_ASSERT(1648, NULL != mEvtSeqList[mOrderEvtNo]);
                    if ((this->*mEvtSeqList[mOrderEvtNo])(staffId)) {
                        eventManager.cutEnd(staffId);
                        staffId = 1;
                    }
                }

                if (eventInfo.checkCommandDemoAccrpt() && mEventIdx != -1 && eventManager.endCheck(mEventIdx)) {
                    dComIfGp_event_reset();
                    mOrderEvtNo = EVENT_NONE;
                    mEventIdx = -1;
                    setAction(&daNpcBouS_c::wait);
                }
            }
            break;

        case 3:
            break;

        default:
            JUT_ASSERT(1676, FALSE);
            break;
    }

    return 1;
}

fopAc_ac_c* daNpcBouS_c::searchInstructionTag() {
    fopAc_ac_c* instructionTag = (fopAc_ac_c*)fpcM_Search(s_sub, this);
    return instructionTag;
}

bool daNpcBouS_c::instruction(void* param_1) {
    dCamera_c* camBody = dCam_getBody();

    switch (mMode) {
        case 0:
            setLookMode(LOOK_PLAYER);
            mInstructionMode = 0;
            mTimer = 20;
            mDoGph_gInf_c::fadeOut(0.05f);
            camBody->Stop();
            mMode = 2;
            break;
        
        case 2:
            switch (mInstructionMode) {
                case 0:
                    if (!eventInfo.checkCommandDemoAccrpt()) {
                        fopAcM_orderPotentialEvent(this, 1, 0xFFFF, 0);
                        eventInfo.onCondition(dEvtCnd_CANDEMO_e);
                        return false;
                    }

                    if (mTimer <= 0) {
                        setInstructionPos();
                        cXyz camCenter(mHIO->m.gaze_x, mHIO->m.gaze_y, mHIO->m.gaze_z);
                        cXyz camEye(0.0f, 0.0f, mHIO->m.eye_dist);
                        mDoMtx_stack_c::transS(fopAcM_GetPosition(daPy_getPlayerActorClass()));
                        mDoMtx_stack_c::YrotM(fopAcM_GetShapeAngle_p(daPy_getPlayerActorClass())->y);
                        mDoMtx_stack_c::multVec(&camCenter, &camCenter);
                        mDoMtx_stack_c::YrotM(mHIO->m.eye_angle_y);
                        mDoMtx_stack_c::XrotM(mHIO->m.eye_angle_x);
                        mDoMtx_stack_c::multVec(&camEye, &camEye);
                        camBody->Set(camCenter, camEye, camBody->Fovy(), 0);
                        mTimer = 16;
                        mInstructionMode++;
                    }

                    mTimer--;
                    break;

                case 1:
                    if (mTimer-- <= 0) {
                        mTimer = abs(20);
                        mDoGph_gInf_c::fadeOut(-0.05f);
                        initTalk(6, NULL);
                        setMotionAnm(ANM_WAIT_A, 0.0f);
                        mInstructionMode++;
                    }
                    break;
                
                case 2:
                    if (talkProc(NULL, TRUE, NULL)) {
                        mActorMngrs[0].entry(daPy_getPlayerActorClass());
                        parentActorID = fopAcM_createChild(PROC_NPC_WRESTLER, fopAcM_GetID(this),
                                            getArenaNo() | 0x700 | 0x2000000, &l_createPos, fopAcM_GetRoomNo(this),
                                            NULL, NULL, getType(), NULL);
                        mInstructionMode++;
                    } else {
                        int i_expression, i_motion;
                        int msgTimer = mMsgTimer;

                        if (ctrlMsgAnm(i_expression, i_motion, this, FALSE)) {
                            setExpression(i_expression, -1.0f);
                            setMotion(i_motion, -1.0f, 0);
                        } else if (msgTimer != 0 && mMsgTimer == 0) {
                            setExpressionTalkAfter();
                        }
                    }
                    break;

                case 3: {
                    daNpcWrestler_c* actor = (daNpcWrestler_c*)fpcM_SearchByID(parentActorID);
                    if (actor != NULL) {
                        if (actor->checkStartUp()) {
                            dComIfGp_event_reset();
                            setAction(&daNpcBouS_c::wait);
                        }
                    }
                    break;
                }

                default:
                    JUT_ASSERT(1798, FALSE);
                    break;
            }
            break;

        case 3:
            break;

        default:
            JUT_ASSERT(1803, FALSE);
            break;
    }

    return true;
}

bool daNpcBouS_c::checkIntroDemoStart() {
    if (strcmp(dComIfGp_getStartStageName(), "R_SP01") != 0 || dComIfGs_getStartPoint() != 2) {
        return FALSE;
    }

    OS_REPORT("イベント実行中%s 会話イベント%s\n", dComIfGp_event_runCheck() ? "です。" : "じゃないです。", eventInfo.checkCommandTalk() ? "です。" : "じゃないです。");

    mOrderEvtNo = EVENT_INTRO_SUMO1;

    if (dComIfGp_event_runCheck()) {
        changeEvent("Bou4", l_evtNames[mOrderEvtNo], 1, 0xFFFF);
    } else {
        eventInfo.setArchiveName("Bou4");
        orderEvent(0, l_evtNames[mOrderEvtNo], 0xFFFF, 4, 0xFF, 1);
    }

    return TRUE;
}

int daNpcBouS_c::EvCut_BousIntroSumo1(int i_staffId) {
    dEvent_manager_c& eventManager = dComIfGp_getEventManager();
    int* cutName = (int*)eventManager.getMyNowCutName(i_staffId);

    if (eventManager.getIsAddvance(i_staffId)) {
        switch (*cutName) {
            case '0x0001':
                setLookMode(LOOK_PLAYER_TALK);
                mActorMngrs[0].entry(daPy_getPlayerActorClass());
                break;

            case '0x0002':
            case '0x0003':
            case '0x0005':
            case '0x0006':
            case '0x0007':
            case '0x0008':
            case '0x0009':
                initTalk(9, NULL);
                break;
            
            case '0x0004':
                setExpressionAnm(ANM_FH_TALK_B, true);
                break;

            default:
                JUT_ASSERT(1870, FALSE);
                break;
        }
    }

    int i_expression, i_motion;
    int msgTimer = mMsgTimer;

    if (ctrlMsgAnm(i_expression, i_motion, this, FALSE)) {
        setExpression(i_expression, -1.0f);
        setMotion(i_motion, -1.0f, 0);
    } else if (msgTimer != 0 && mMsgTimer == 0) {
        setExpressionTalkAfter();
    }

    switch (*cutName) {
        case '0x0001':
        case '0x0004':
            return 1;

        case '0x0002':
        case '0x0003':
        case '0x0005':
        case '0x0006':
        case '0x0007':
        case '0x0008':
        case '0x0009':
            if (talkProc(NULL, TRUE, NULL)) {
                s32 choiceNo = mFlow.getChoiceNo();
                OS_REPORT("二択分岐 %s\n", choiceNo == 0 ? "はい" : "いいえ");
                
                if (choiceNo == 0) {
                    mOrderEvtNo = EVENT_INTRO_SUMO3;
                    changeEvent("Bou4", l_evtNames[mOrderEvtNo], 1, 0xFFFF);
                }
                
                return 1;
            }
            break;

        default:
            JUT_ASSERT(1911, FALSE);
            break;
    }

    return 0;
}

int daNpcBouS_c::EvCut_BousIntroSumo2(int i_staffId) {
    dEvent_manager_c& eventManager = dComIfGp_getEventManager();
    int* cutName = (int*)eventManager.getMyNowCutName(i_staffId);

    if (eventManager.getIsAddvance(i_staffId)) {
        switch (*cutName) {
            case '0x0001':
                initTalk(9, NULL);
                setLookMode(LOOK_PLAYER_TALK);
                mActorMngrs[0].entry(daPy_getPlayerActorClass());
                break;

            default:
                JUT_ASSERT(1939, FALSE);
        }
    }

    int i_expression, i_motion;
    int msgTimer = mMsgTimer;

    if (ctrlMsgAnm(i_expression, i_motion, this, FALSE)) {
        setExpression(i_expression, -1.0f);
        setMotion(i_motion, -1.0f, 0);
    } else if (msgTimer != 0 && mMsgTimer == 0) {
        setExpressionTalkAfter();
    }

    switch (*cutName) {
        case '0x0001':
            if (mCurAngle.y == fopAcM_searchPlayerAngleY(this)) {
                if (talkProc(NULL, TRUE, NULL)) {
                    int choiceNo = mFlow.getChoiceNo();
                    OS_REPORT("二択分岐 %s\n", choiceNo == 0 ? "はい" : "いいえ");

                    if (choiceNo == 0) {
                        mOrderEvtNo = EVENT_INTRO_SUMO3;
                        changeEvent("Bou4", l_evtNames[mOrderEvtNo], 1, 0xFFFF);
                    }

                    return 1;
                }
            } else if (step(fopAcM_searchPlayerAngleY(this), 1)) {
                setMotion(MOT_WAIT_A, -1.0f, 0);
                mTurnMode = 0;
            }
            break;

        default:
            JUT_ASSERT(1979, FALSE);
            break;
    }

    return 0;
}

int daNpcBouS_c::EvCut_BousIntroSumo3(int i_staffId) {
    dEvent_manager_c& eventManager = dComIfGp_getEventManager();
    int* cutName = (int*)eventManager.getMyNowCutName(i_staffId);

    if (eventManager.getIsAddvance(i_staffId)) {
        switch (*cutName) {
            case '0x0001':
                setLookMode(LOOK_PLAYER_TALK);
                mActorMngrs[0].entry(daPy_getPlayerActorClass());
                break;

            case '0x0003':
                setMotion(MOT_WALK, -1.0f, 0);
                // fallthrough
            case '0x0002':
                setAngle(-0x2AAA);
                initTalk(9, NULL);
                break;
            
            default:
                JUT_ASSERT(2012, FALSE);
                break;
        }
    }

    int i_expression, i_motion;
    int msgTimer = mMsgTimer;

    if (ctrlMsgAnm(i_expression, i_motion, this, FALSE)) {
        if (*cutName != 0x30303033) {
            setExpression(i_expression, -1.0f);
            setMotion(i_motion, -1.0f, 0);
        }
    } else if (msgTimer != 0 && mMsgTimer == 0) {
        setExpressionTalkAfter();
    }

    switch (*cutName) {
        case '0x0001':
            return 1;
        
        case '0x0002':
            if (talkProc(NULL, TRUE, NULL)) {
                return 1;
            }
            break;

        case '0x0003': {
            cXyz* pos = dComIfGp_evmng_getMyXyzP(i_staffId, "pos");
            if (pos != NULL) {
                if (cLib_chaseAngleS(&shape_angle.y, cLib_targetAngleY(&current.pos, pos), 0x100)) {
                    speedF = 1.2f;
                } else {
                    cLib_chasePosXZ(&current.pos, *pos, 2.5f);
                }

                setAngle(shape_angle.y);

                if (speedF > 0.0f) {
                    fopAcM_posMoveF(this, NULL);
                }
            }

            if (talkProc(NULL, TRUE, NULL)) {
                int itemNo = 0;
                u16 eventId = mFlow.getEventId(&itemNo);
                OS_REPORT("会話終了時 イベントID=%d アイテムNo=%d\n", eventId, itemNo);

                if (eventId == 6) {
                    if (itemNo == 1) {
                        setAction((&daNpcBouS_c::instruction));
                    } else {
                        parentActorID = fopAcM_createChild(PROC_NPC_WRESTLER, fopAcM_GetID(this),
                                            (itemNo << 24) | (getArenaNo() | 0x700), &l_createPos, fopAcM_GetRoomNo(this),
                                            NULL, NULL, getType(), NULL);
                        setAngle(home.angle.y);
                        setAction(&daNpcBouS_c::wait);
                    }

                    dComIfGp_event_reset();
                    mOrderEvtNo = EVENT_NONE;
                    mEventIdx = -1;
                }

                return 1;
            }
            break;
        }

        default:
            JUT_ASSERT(2084, FALSE);
            break;
    }

    return 0;
}

static int daNpcBouS_Create(void* i_this) {
    return static_cast<daNpcBouS_c*>(i_this)->Create();
}

static int daNpcBouS_Delete(void* i_this) {
    return static_cast<daNpcBouS_c*>(i_this)->Delete();
}

static int daNpcBouS_Execute(void* i_this) {
    return static_cast<daNpcBouS_c*>(i_this)->Execute();
}

static int daNpcBouS_Draw(void* i_this) {
    return static_cast<daNpcBouS_c*>(i_this)->Draw();
}

static int daNpcBouS_IsDelete(void* i_this) {
    return 1;
}

int daNpcBouS_c::drawDbgInfo() {
    #if DEBUG
    if (mHIO->m.common.debug_info_ON) {
        cXyz sp48;

    }
    #endif
    
    return 0;
}

void daNpcBouS_c::drawOtherMdls() {}

AUDIO_INSTANCES;

static actor_method_class daNpcBouS_MethodTable = {
    (process_method_func)daNpcBouS_Create,
    (process_method_func)daNpcBouS_Delete,
    (process_method_func)daNpcBouS_Execute,
    (process_method_func)daNpcBouS_IsDelete,
    (process_method_func)daNpcBouS_Draw,
};

actor_process_profile_definition g_profile_NPC_BOU_S = {
  fpcLy_CURRENT_e,        // mLayerID
  7,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_NPC_BOU_S,         // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(daNpcBouS_c),    // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  325,                    // mPriority
  &daNpcBouS_MethodTable, // sub_method
  0x00044100,             // mStatus
  fopAc_NPC_e,            // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};
