/**
 * @file d_a_npc_ins.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_npc_ins.h"
#include "d/actor/d_a_npc.h"
#include "d/d_meter2_info.h"
#include "d/d_msg_object.h"

enum Ins_RES_File_ID {
    /* BCK */
    /* 0x06 */ BCK_INS_F_HAPPY = 0x6,
    /* 0x07 */ BCK_INS_F_MAD,
    /* 0x08 */ BCK_INS_F_SNIFF,
    /* 0x09 */ BCK_INS_F_SURPRISED,
    /* 0x0A */ BCK_INS_F_TALK_A,
    /* 0x0B */ BCK_INS_FH_HAPPY,
    /* 0x0C */ BCK_INS_FH_MAD,
    /* 0x0D */ BCK_INS_FH_SNIFF,
    /* 0x0E */ BCK_INS_FH_SURPRISED,

    /* BMDR */
    /* 0x11 */ BMDR_INS = 0x11,

    /* BTK */
    /* 0x14 */ BTK_INS = 0x14,

    /* BTP */
    /* 0x17 */ BTP_INS = 0x17,
    /* 0x18 */ BTP_INS_F_HAPPY,
    /* 0x19 */ BTP_INS_F_MAD,
    /* 0x1A */ BTP_INS_F_SNIFF,
    /* 0x1B */ BTP_INS_F_SURPRISED,
    /* 0x1C */ BTP_INS_FH_HAPPY,
    /* 0x1D */ BTP_INS_FH_MAD,
    /* 0x1E */ BTP_INS_FH_SURPRISED,
};

enum Ins1_RES_File_ID {
    /* BCK */
    /* 0x4 */ BCK_INS_HI = 0x4,
    /* 0x5 */ BCK_INS_SNIFF,
    /* 0x6 */ BCK_INS_STEP,
    /* 0x7 */ BCK_INS_SURPRISED,
    /* 0x8 */ BCK_INS_SURPRISED_WAIT,
    /* 0x9 */ BCK_INS_TALK_A,
    /* 0xA */ BCK_INS_TALK_A_WAIT,
    /* 0xB */ BCK_INS_WAIT_A,

    /* BTK */
    /* 0xE */ BTK_INS_SNIFF = 0xE,
};

enum Ins2_RES_File_ID {
    /* BCK */
    /* 0x05 */ BCK_INS_K_LOOKING = 0x5,
    /* 0x06 */ BCK_INS_K_SEARCHING_A,
    /* 0x07 */ BCK_INS_K_SEARCHING_B,
    /* 0x08 */ BCK_INS_K_STEP,
    /* 0x09 */ BCK_INS_K_WAIT,
    /* 0x0A */ BCK_INS_K_WALK,

    /* BMDE */
    /* 0x0D */ BMDE_INS_KAGO = 0xD,

    /* BMDR */
    /* 0x10 */ BMDR_INS_UMBRELLA = 0x10,
};

enum RES_Names {
    /* 0x0 */ INS,
    /* 0x1 */ INS1,
    /* 0x2 */ INS2,
};

enum Joint {
    /* 0x00 */ JNT_CENTER,
    /* 0x01 */ JNT_BACKBONE1,
    /* 0x02 */ JNT_BACKBONE2,
    /* 0x03 */ JNT_NECK,
    /* 0x04 */ JNT_HEAD,
    /* 0x05 */ JNT_CHIN,
    /* 0x06 */ JNT_MAYUL,
    /* 0x07 */ JNT_MAYUR,
    /* 0x08 */ JNT_MOUTH,
    /* 0x09 */ JNT_SHOULDERL,
    /* 0x0A */ JNT_ARML1,
    /* 0x0B */ JNT_ARML2,
    /* 0x0C */ JNT_HANDL,
    /* 0x0D */ JNT_FINGERL,
    /* 0x0E */ JNT_THUMBL,
    /* 0x0F */ JNT_SHOULDERR,
    /* 0x10 */ JNT_ARMR1,
    /* 0x11 */ JNT_ARMR2,
    /* 0x12 */ JNT_HANDR,
    /* 0x13 */ JNT_FINGERR,
    /* 0x14 */ JNT_THUMBR,
    /* 0x15 */ JNT_WAIST,
    /* 0x16 */ JNT_LEGL1,
    /* 0x17 */ JNT_LEGL2,
    /* 0x18 */ JNT_FOOTL,
    /* 0x19 */ JNT_LEGR1,
    /* 0x1A */ JNT_LEGR2,
    /* 0x1B */ JNT_FOOTR,
};

enum Animation {
    /* 0x00 */ ANM_NONE,
    /* 0x01 */ ANM_F_TALK_A,
    /* 0x02 */ ANM_F_SNIFF,
    /* 0x03 */ ANM_F_SURPRISED,
    /* 0x04 */ ANM_F_HAPPY,
    /* 0x05 */ ANM_F_MAD,
    /* 0x06 */ ANM_FH_SNIFF,
    /* 0x07 */ ANM_FH_SURPRISED,
    /* 0x08 */ ANM_FH_HAPPY,
    /* 0x09 */ ANM_FH_MAD,
    /* 0x0A */ ANM_WAIT_A,
    /* 0x0B */ ANM_TALK_A_WAIT,
    /* 0x0C */ ANM_TALK_A,
    /* 0x0D */ ANM_HI,
    /* 0x0E */ ANM_SURPRISED,
    /* 0x0F */ ANM_SURPRISED_WAIT,
    /* 0x10 */ ANM_SNIFF,
    /* 0x11 */ ANM_STEP,
    /* 0x12 */ ANM_K_WAIT,
    /* 0x13 */ ANM_K_WALK,
    /* 0x14 */ ANM_K_LOOKING,
    /* 0x15 */ ANM_K_SEARCHING_A,
    /* 0x16 */ ANM_K_SEARCHING_B,
    /* 0x17 */ ANM_K_STEP,
};

enum Expression_BTP {
    /* 0x0 */ EXPR_BTP_INS,
    /* 0x1 */ EXPR_BTP_SNIFF,
    /* 0x2 */ EXPR_BTP_SURPRISED,
    /* 0x3 */ EXPR_BTP_HAPPY,
    /* 0x4 */ EXPR_BTP_MAD,
    /* 0x5 */ EXPR_BTP_H_SURPRISED,
    /* 0x6 */ EXPR_BTP_H_HAPPY,
    /* 0x7 */ EXPR_BTP_H_MAD,
};

enum Expression {
    /* 0x0 */ EXPR_TALK_A,
    /* 0x1 */ EXPR_SNIFF,
    /* 0x2 */ EXPR_SURPRISED,
    /* 0x3 */ EXPR_HAPPY,
    /* 0x4 */ EXPR_MAD,
    /* 0x5 */ EXPR_H_SNIFF,
    /* 0x6 */ EXPR_H_SURPRISED,
    /* 0x7 */ EXPR_H_HAPPY,
    /* 0x8 */ EXPR_H_MAD,
    /* 0x9 */ EXPR_NONE,
};

enum Motion {
    /* 0x0 */ MOT_WAIT_A,
    /* 0x1 */ MOT_TALK_A,
    /* 0x2 */ MOT_TALK_A_WAIT,
    /* 0x3 */ MOT_HI,
    /* 0x4 */ MOT_K_WAIT,
    /* 0x5 */ MOT_SURPRISED,
    /* 0x6 */ MOT_SNIFF,
    /* 0x7 */ MOT_K_WALK,
    /* 0x8 */ MOT_K_LOOKING,
    /* 0x9 */ MOT_K_SEARCHING_A,
    /* 0xA */ MOT_K_SEARCHING_B,
    /* 0xB */ MOT_STEP,
    /* 0xC */ MOT_K_STEP,
};

enum Insect_Type {
    /* 0x00 */ INSECT_M_BEETLE,
    /* 0x01 */ INSECT_F_BEETLE,
    /* 0x02 */ INSECT_M_BUTTERFLY,
    /* 0x03 */ INSECT_F_BUTTERFLY,
    /* 0x04 */ INSECT_M_STAG_BEETLE,
    /* 0x05 */ INSECT_F_STAG_BEETLE,
    /* 0x06 */ INSECT_M_GRASSHOPPER,
    /* 0x07 */ INSECT_F_GRASSHOPPER,
    /* 0x08 */ INSECT_M_NANAFUSHI,
    /* 0x09 */ INSECT_F_NANAFUSHI,
    /* 0x0A */ INSECT_M_DANGOMUSHI,
    /* 0x0B */ INSECT_F_DANGOMUSHI,
    /* 0x0C */ INSECT_M_MANTIS,
    /* 0x0D */ INSECT_F_MANTIS,
    /* 0x0E */ INSECT_M_LADYBUG,
    /* 0x0F */ INSECT_F_LADYBUG,
    /* 0x10 */ INSECT_M_SNAIL,
    /* 0x11 */ INSECT_F_SNAIL,
    /* 0x12 */ INSECT_M_DRAGONFLY,
    /* 0x13 */ INSECT_F_DRAGONFLY,
    /* 0x14 */ INSECT_M_ANT,
    /* 0x15 */ INSECT_F_ANT,
    /* 0x16 */ INSECT_M_MAYFLY,
    /* 0x17 */ INSECT_F_MAYFLY,
};

/*
    TYPE_0 is for when Agitha is inside her home in Castle Town.
    TYPE_1 and TYPE_2 are for when Agitha is outside Castle Town.
    Her outside type is selected by random.
*/
enum Type {
    /* 0x0 */ TYPE_0,
    /* 0x1 */ TYPE_1,
    /* 0x2 */ TYPE_2,
};

static daNpcIns_Param_c l_HIO;

static daNpc_GetParam2 l_bckGetParamList[24] = {
    {-1, J3DFrameCtrl::EMode_LOOP, INS},
    {BCK_INS_F_TALK_A, J3DFrameCtrl::EMode_NONE, INS},
    {BCK_INS_F_SNIFF, J3DFrameCtrl::EMode_NONE, INS},
    {BCK_INS_F_SURPRISED, J3DFrameCtrl::EMode_NONE, INS},
    {BCK_INS_F_HAPPY, J3DFrameCtrl::EMode_NONE, INS},
    {BCK_INS_F_MAD, J3DFrameCtrl::EMode_NONE, INS},
    {BCK_INS_FH_SNIFF, J3DFrameCtrl::EMode_LOOP, INS},
    {BCK_INS_FH_SURPRISED, J3DFrameCtrl::EMode_LOOP, INS},
    {BCK_INS_FH_HAPPY, J3DFrameCtrl::EMode_LOOP, INS},
    {BCK_INS_FH_MAD, J3DFrameCtrl::EMode_LOOP, INS},
    {BCK_INS_WAIT_A, J3DFrameCtrl::EMode_LOOP, INS1},
    {BCK_INS_TALK_A_WAIT, J3DFrameCtrl::EMode_LOOP, INS1},
    {BCK_INS_TALK_A, J3DFrameCtrl::EMode_NONE, INS1},
    {BCK_INS_HI, J3DFrameCtrl::EMode_NONE, INS1},
    {BCK_INS_SURPRISED, J3DFrameCtrl::EMode_NONE, INS1},
    {BCK_INS_SURPRISED_WAIT, J3DFrameCtrl::EMode_LOOP, INS1},
    {BCK_INS_SNIFF, J3DFrameCtrl::EMode_NONE, INS1},
    {BCK_INS_STEP, J3DFrameCtrl::EMode_NONE, INS1},
    {BCK_INS_K_WAIT, J3DFrameCtrl::EMode_LOOP, INS2},
    {BCK_INS_K_WALK, J3DFrameCtrl::EMode_LOOP, INS2},
    {BCK_INS_K_LOOKING, J3DFrameCtrl::EMode_LOOP, INS2},
    {BCK_INS_K_SEARCHING_A, J3DFrameCtrl::EMode_LOOP, INS2},
    {BCK_INS_K_SEARCHING_B, J3DFrameCtrl::EMode_LOOP, INS2},
    {BCK_INS_K_STEP, J3DFrameCtrl::EMode_NONE, INS2},
};

static daNpc_GetParam2 l_btpGetParamList[8] = {
    {BTP_INS, J3DFrameCtrl::EMode_LOOP, INS},
    {BTP_INS_F_SNIFF, J3DFrameCtrl::EMode_NONE, INS},
    {BTP_INS_F_SURPRISED, J3DFrameCtrl::EMode_NONE, INS},
    {BTP_INS_F_HAPPY, J3DFrameCtrl::EMode_NONE, INS},
    {BTP_INS_F_MAD, J3DFrameCtrl::EMode_NONE, INS},
    {BTP_INS_FH_SURPRISED, J3DFrameCtrl::EMode_LOOP, INS},
    {BTP_INS_FH_HAPPY, J3DFrameCtrl::EMode_LOOP, INS},
    {BTP_INS_FH_MAD, J3DFrameCtrl::EMode_LOOP, INS},
};

static daNpc_GetParam2 l_btkGetParamList[2] = {
    {BTK_INS, J3DFrameCtrl::EMode_LOOP, INS},
    {BTK_INS_SNIFF, J3DFrameCtrl::EMode_NONE, INS1},
};

static int l_loadRes_INS0[3] = {
    INS, INS1, -1,
};

static int l_loadRes_INS1[3] = {
    INS, INS2, -1,
};

static int l_loadRes_INS2[3] = {
    INS, INS2, -1,
};

static int* l_loadRes_list[3] = {
    l_loadRes_INS0,
    l_loadRes_INS1,
    l_loadRes_INS2,
};

static char* l_arcNames[3] = {
    "Ins",
    "Ins1",
    "Ins2",
};

static char* l_evtNames[1] = {
    NULL,
};

static char* l_myName = "ins";

daNpcIns_c::eventFunc daNpcIns_c::mEvtSeqList[1] = {
    NULL,
};

daNpcIns_c::daNpcIns_c() {}

daNpcIns_c::~daNpcIns_c() {
    for (int i = 0; l_loadRes_list[mType][i] >= 0; i++) {
        dComIfG_resDelete(&mPhases[i], l_arcNames[l_loadRes_list[mType][i]]);
    }

    if (heap != 0) {
        mAnm_p->stopZelAnime();
    }
}

static insect_param_data const l_insectParams[24] = {
    {0x0191, 0x709, 0, 0},
    {0x0192, 0x709, 0, 0},
    {0x0193, 0x70A, 0, 0},
    {0x0194, 0x70A, 0, 0},
    {0x0195, 0x70B, 0, 0},
    {0x0196, 0x70B, 0, 0},
    {0x0197, 0x70C, 0, 0},
    {0x0198, 0x70C, 0, 0},
    {0x0199, 0x70D, 0, 0},
    {0x019A, 0x70D, 0, 0},
    {0x019B, 0x70E, 0, 0},
    {0x019C, 0x70E, 0, 0},
    {0x019D, 0x70F, 0, 0},
    {0x019E, 0x70F, 0, 0},
    {0x019F, 0x710, 0, 0},
    {0x01A0, 0x710, 0, 0},
    {0x01A1, 0x711, 0, 0},
    {0x01A2, 0x711, 0, 0},
    {0x01A3, 0x712, 0, 0},
    {0x01A4, 0x712, 0, 0},
    {0x01A5, 0x713, 0, 0},
    {0x01A6, 0x713, 0, 0},
    {0x01A7, 0x714, 0, 0},
    {0x01A8, 0x714, 0, 0},
};

daNpcIns_HIOParam const daNpcIns_Param_c::m = {
    35.0f,
    -3.0f,
    1.0f,
    400.0f,
    255.0f,
    130.0f,
    45.0f,
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
    1.5f,
};

cPhs__Step daNpcIns_c::Create() {
    fopAcM_ct(this, daNpcIns_c);

    mType = TYPE_0;

    if (strcmp(dComIfGp_getStartStageName(), "R_SP160") == 0) {
        mType = TYPE_0;
    } else if (strcmp(dComIfGp_getStartStageName(), "F_SP122") == 0) {
        if (isInsectComplete()) {
            return cPhs_ERROR_e;
        }

        if (cLib_getRndValue(0, 2) != 0) {
            mType = TYPE_1;
        } else {
            mType = TYPE_2;
        }

        u16 StartTime = ((getStartTime() & 0xFF) / 10) * 60 + (((getStartTime() & 0xFF) % 10) * 10);
        mGoHomeTime = ((getEndTime() & 0xFF) / 10) * 60 + (((getEndTime() & 0xFF) % 10) * 10);

        JUT_ASSERT(331, mGoHomeTime < 1440);
        JUT_ASSERT(332, StartTime < mGoHomeTime);

        if (getTime() < StartTime || getTime() > mGoHomeTime) {
            return cPhs_ERROR_e;
        }
    }

    cPhs__Step phase = cPhs_ERROR_e;
    for (int i = 0; l_loadRes_list[mType][i] >= 0; i++) {
        phase = (cPhs__Step)dComIfG_resLoad(&mPhases[i], l_arcNames[l_loadRes_list[mType][i]]);

        if (phase != cPhs_COMPLEATE_e) {
            return phase;
        }
    }

    if (phase == cPhs_COMPLEATE_e) {
        if (!fopAcM_entrySolidHeap(this, createHeapCallBack, 0x6200)) {
            return cPhs_ERROR_e;
        }

        mInsectMsgNo = getMessageNo();
        field_0xe16 = mInsectMsgNo;
        mAnm_p->getModel()->getModelData();
        fopAcM_SetMtx(this, mAnm_p->getModel()->getBaseTRMtx());
        fopAcM_setCullSizeBox(this, -160.0f, -50.0f, -160.0f, 160.0f, 220.0f, 160.0f);
        mSound.init(&current.pos, &eyePos, 3, 1);
        mAcchCir.SetWall(daNpcIns_Param_c::m.common.width, daNpcIns_Param_c::m.common.knee_length);
        mAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1, &mAcchCir, fopAcM_GetSpeed_p(this),
                  fopAcM_GetAngle_p(this), fopAcM_GetShapeAngle_p(this));
        mAcch.SetRoofNone();
        mAcch.SetWaterNone();
        mAcch.CrrPos(dComIfG_Bgsp());
        mCcStts.Init(daNpcIns_Param_c::m.common.weight, 0, this);
        mCyl.Set(mCcDCyl);
        mCyl.SetStts(&mCcStts);
        mCyl.SetTgType(0);
        mCyl.SetTgSPrm(0);
        mCyl.SetH(daNpcIns_Param_c::m.common.height);
        mCyl.SetR(daNpcIns_Param_c::m.common.width);
        mGndChk = mAcch.m_gnd;
        mGroundH = mAcch.GetGroundH();
        setEnvTevColor();
        setRoomNo();
        mAnm_p->modelCalc();
        reset();
        Execute();
    }

    return phase;
}

int daNpcIns_c::CreateHeap() {
    J3DModelData* mdlData_p = static_cast<J3DModelData*>(dComIfG_getObjectRes(l_arcNames[0], BMDR_INS));

    JUT_ASSERT(424, NULL != mdlData_p);

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

    mpUmbrellaModel = NULL;
    mpKagoModel = NULL;

    if (mType == TYPE_1 || mType == TYPE_2) {
        mdlData_p = (J3DModelData*)dComIfG_getObjectRes(l_arcNames[2], BMDR_INS_UMBRELLA);
        JUT_ASSERT(456, mdlData_p != NULL);
        mpUmbrellaModel = mDoExt_J3DModel__create(mdlData_p, 0x80000, 0x11000084);
        if (mpUmbrellaModel == NULL) {
            return 0;
        }

        mdlData_p = (J3DModelData*)dComIfG_getObjectRes(l_arcNames[2], BMDE_INS_KAGO);
        JUT_ASSERT(463, mdlData_p != NULL);
        mpKagoModel = mDoExt_J3DModel__create(mdlData_p, 0, 0x11000084);
        if (mpKagoModel == NULL) {
            return 0;
        }
    }

    mpMatAnm = new daNpcF_MatAnm_c();
    if (mpMatAnm == NULL) {
        return 0;
    }

    if (!setExpressionAnm(ANM_F_TALK_A, false)) {
        return 0;
    }

    int i_motion = ANM_WAIT_A;
    switch (mType) {
        case TYPE_0:
            i_motion = ANM_WAIT_A;
            break;
        
        case TYPE_1:
            i_motion = ANM_K_LOOKING;
            break;

        case TYPE_2:
            i_motion = ANM_K_SEARCHING_B;
            break;
    }

    setMotionAnm(i_motion, 0.0f);

    return 1;
}

int daNpcIns_c::Delete() {
    this->~daNpcIns_c();
    return 1;
}

int daNpcIns_c::Execute() {
    execute();
    return 1;
}

int daNpcIns_c::Draw() {
    mAnm_p->getModel()->getModelData()->getMaterialNodePointer(1)->setMaterialAnm(mpMatAnm);
    draw(FALSE, FALSE, daNpcIns_Param_c::m.common.real_shadow_size, NULL, FALSE);

    if (mpUmbrellaModel != NULL) {
        g_env_light.setLightTevColorType_MAJI(mpUmbrellaModel, &tevStr);
        mpUmbrellaModel->setBaseTRMtx(mAnm_p->getModel()->getAnmMtx(JNT_FINGERL));
        mDoExt_modelUpdateDL(mpUmbrellaModel);
        dComIfGd_addRealShadow(mShadowKey, mpUmbrellaModel);
    }

    if (mpKagoModel != NULL) {
        g_env_light.setLightTevColorType_MAJI(mpKagoModel, &tevStr);
        mpKagoModel->setBaseTRMtx(mAnm_p->getModel()->getAnmMtx(JNT_ARML2));
        mDoExt_modelUpdateDL(mpKagoModel);
        dComIfGd_addRealShadow(mShadowKey, mpKagoModel);
    }

    return 1;
}

int daNpcIns_c::ctrlJoint(J3DJoint* i_joint, J3DModel* i_model) {
    int jntNo = i_joint->getJntNo();
    int lookatJoints[3] = {JNT_BACKBONE1, JNT_NECK, JNT_HEAD};

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
            setLookatMtx(jntNo, lookatJoints, daNpcIns_Param_c::m.common.neck_rotation_ratio);
            break;
    }

    i_model->setAnmMtx(jntNo, mDoMtx_stack_c::get());
    MTXCopy(mDoMtx_stack_c::get(), J3DSys::mCurrentMtx);

    if ((jntNo == JNT_HEAD || jntNo == JNT_MOUTH) && (mAnmFlags & ANM_PLAY_BCK) != 0) {
        J3DAnmTransform* anmTransform = mBckAnm.getBckAnm();
        mBckAnm.changeBckOnly(mAnm_p->getAnm());
        mAnm_p->changeAnm(anmTransform);
    }

    return 1;
}

int daNpcIns_c::createHeapCallBack(fopAc_ac_c* a_this) {
    return static_cast<daNpcIns_c*>(a_this)->CreateHeap();
}

int daNpcIns_c::ctrlJointCallBack(J3DJoint* i_joint, int param_2) {
    if (param_2 == 0) {
        J3DModel* model = j3dSys.getModel();
        daNpcIns_c* i_this = (daNpcIns_c*)model->getUserArea();
        
        if (i_this != NULL) {
            i_this->ctrlJoint(i_joint, model);
        }
    }

    return 1;
}

void daNpcIns_c::playExpression() {
    daNpcF_anmPlayData dat0 = {ANM_F_TALK_A, daNpcIns_Param_c::m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat0[1] = {&dat0};
    daNpcF_anmPlayData dat1 = {ANM_F_SNIFF, daNpcIns_Param_c::m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat1[1] = {&dat1};
    daNpcF_anmPlayData dat2 = {ANM_F_SURPRISED, 0.0f, 0};
    daNpcF_anmPlayData* pDat2[1] = {&dat2};
    daNpcF_anmPlayData dat3 = {ANM_F_HAPPY, daNpcIns_Param_c::m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat3[1] = {&dat3};
    daNpcF_anmPlayData dat4 = {ANM_F_MAD, daNpcIns_Param_c::m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat4[1] = {&dat4};
    daNpcF_anmPlayData dat5 = {ANM_FH_SNIFF, daNpcIns_Param_c::m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat5[1] = {&dat5};
    daNpcF_anmPlayData dat6 = {ANM_FH_SURPRISED, 0.0f, 0};
    daNpcF_anmPlayData* pDat6[1] = {&dat6};
    daNpcF_anmPlayData dat7 = {ANM_FH_HAPPY, daNpcIns_Param_c::m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat7[1] = {&dat7};
    daNpcF_anmPlayData dat8 = {ANM_FH_MAD, daNpcIns_Param_c::m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat8[1] = {&dat8};
    daNpcF_anmPlayData dat9 = {ANM_NONE, daNpcIns_Param_c::m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat9[1] = {&dat9};

    daNpcF_anmPlayData** ppDat[10] = {
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
    };

    if (mExpression >= 0 && mExpression < 10) {
        playExpressionAnm(ppDat);
    }
}

void daNpcIns_c::playMotion() {
    daNpcF_anmPlayData dat0 = {ANM_WAIT_A, daNpcIns_Param_c::m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat0[1] = {&dat0};
    daNpcF_anmPlayData dat1a = {ANM_TALK_A, daNpcIns_Param_c::m.common.morf_frame, 1};
    daNpcF_anmPlayData dat1b = {ANM_TALK_A_WAIT, 0.0f, 0};
    daNpcF_anmPlayData* pDat1[2] = {&dat1a, &dat1b};
    daNpcF_anmPlayData dat2 = {ANM_TALK_A_WAIT, daNpcIns_Param_c::m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat2[1] = {&dat2};
    daNpcF_anmPlayData dat3a = {ANM_HI, daNpcIns_Param_c::m.common.morf_frame, 1};
    daNpcF_anmPlayData dat3b = {ANM_WAIT_A, daNpcIns_Param_c::m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat3[2] = {&dat3a, &dat3b};
    daNpcF_anmPlayData dat4 = {ANM_K_WAIT, daNpcIns_Param_c::m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat4[1] = {&dat4};
    daNpcF_anmPlayData dat5a = {ANM_SURPRISED, 0.0f, 1};
    daNpcF_anmPlayData dat5b = {ANM_SURPRISED_WAIT, 0.0f, 0};
    daNpcF_anmPlayData* pDat5[2] = {&dat5a, &dat5b};
    daNpcF_anmPlayData dat6a = {ANM_SNIFF, daNpcIns_Param_c::m.common.morf_frame, 1};
    daNpcF_anmPlayData dat6b = {ANM_TALK_A_WAIT, 0.0f, 0};
    daNpcF_anmPlayData* pDat6[2] = {&dat6a, &dat6b};
    daNpcF_anmPlayData dat7 = {ANM_K_WALK, daNpcIns_Param_c::m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat7[1] = {&dat7};
    daNpcF_anmPlayData dat8 = {ANM_K_LOOKING, daNpcIns_Param_c::m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat8[1] = {&dat8};
    daNpcF_anmPlayData dat9 = {ANM_K_SEARCHING_A, daNpcIns_Param_c::m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat9[1] = {&dat9};
    daNpcF_anmPlayData dat10 = {ANM_K_SEARCHING_B, daNpcIns_Param_c::m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat10[1] = {&dat10};
    daNpcF_anmPlayData dat11 = {ANM_STEP, daNpcIns_Param_c::m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat11[1] = {&dat11};
    daNpcF_anmPlayData dat12 = {ANM_K_STEP, daNpcIns_Param_c::m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat12[1] = {&dat12};

    daNpcF_anmPlayData** ppDat[13] = {
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
    };

    if (mMotion >= 0 && mMotion < 0xD) {
        playMotionAnm(ppDat);
    }
}

void daNpcIns_c::setParam() {
    attention_info.distances[fopAc_attn_LOCK_e] = getDistTableIdx(daNpcIns_Param_c::m.common.attention_distance, daNpcIns_Param_c::m.common.attention_angle);
    attention_info.distances[fopAc_attn_TALK_e] = attention_info.distances[fopAc_attn_LOCK_e];
    attention_info.distances[fopAc_attn_SPEAK_e] = getDistTableIdx(daNpcIns_Param_c::m.common.talk_distance, daNpcIns_Param_c::m.common.talk_angle);
    attention_info.flags = fopAc_AttnFlag_SPEAK_e | fopAc_AttnFlag_TALK_e;

    if (daPy_py_c::checkNowWolf()) {
        attention_info.flags |= fopAc_AttnFlag_UNK_0x800000;
    }

    mAcchCir.SetWallR(daNpcIns_Param_c::m.common.width);
    mAcchCir.SetWallH(daNpcIns_Param_c::m.common.knee_length);
    gravity = daNpcIns_Param_c::m.common.gravity;
}

int daNpcIns_c::drawDbgInfo() {
    return 0;
}

BOOL daNpcIns_c::main() {
    if (mAction) {
        (this->*mAction)(NULL);
    }

    if (mItemID != fpcM_ERROR_PROCESS_ID_e && dComIfGp_event_runCheck()) {
        if (strcmp(dComIfGp_getEventManager().getRunEventName(), "DEFAULT_GETITEM") == 0) {
            dComIfGp_event_setItemPartnerId(mItemID);
            mItemID = fpcM_ERROR_PROCESS_ID_e;
        }
    }

    playExpression();
    playMotion();
    return chkAction(&daNpcIns_c::goHome);
}

bool daNpcIns_c::setExpressionAnm(int i_index, bool i_modify) {
    mAnmFlags &= ~ANM_EXPRESSION_FLAGS;
    J3DAnmTransform* i_bck;

    if (l_bckGetParamList[i_index].fileIdx >= 0) {
        i_bck = getTrnsfrmKeyAnmP(l_arcNames[l_bckGetParamList[i_index].arcIdx], l_bckGetParamList[i_index].fileIdx);    
    } else {
        i_bck = NULL;
    }

    int i_attr = l_bckGetParamList[i_index].attr;
    bool bVar1 = 0;

    switch (i_index) {
        case ANM_NONE:
            bVar1 = setExpressionBtp(EXPR_BTP_INS);
            break;

        case ANM_F_SNIFF:
            bVar1 = setExpressionBtp(EXPR_BTP_SNIFF);
            break;

        case ANM_F_SURPRISED:
            bVar1 = setExpressionBtp(EXPR_BTP_SURPRISED);
            break;

        case ANM_F_HAPPY:
            bVar1 = setExpressionBtp(EXPR_BTP_HAPPY);
            break;

        case ANM_F_MAD:
            bVar1 = setExpressionBtp(EXPR_BTP_MAD);
            break;

        case ANM_FH_SNIFF:
            bVar1 = setExpressionBtp(EXPR_BTP_INS);
            break;

        case ANM_FH_SURPRISED:
            bVar1 = setExpressionBtp(EXPR_BTP_H_SURPRISED);
            break;

        case ANM_FH_HAPPY:
            bVar1 = setExpressionBtp(EXPR_BTP_H_HAPPY);
            break;

        case ANM_FH_MAD:
            bVar1 = setExpressionBtp(EXPR_BTP_H_MAD);
            break;

        case ANM_F_TALK_A:
            bVar1 = setExpressionBtp(EXPR_BTP_INS);
            break;

        default:
            i_bck = NULL;
    }

    if (!bVar1) {
        return false;
    }

    if (i_bck == NULL) {
        return true;
    }

    if (setBckAnm(i_bck, 1.0f, i_attr, 0, -1, i_modify)) {
        mAnmFlags |= ANM_PLAY_BCK | ANM_PAUSE_BCK;
        mExpressionLoops = 0;

        return true;
    }

    OS_REPORT("%s: 表情Bckアニメーションの登録に失敗しました！\n", __FILE__);
    return false;
}

bool daNpcIns_c::setExpressionBtp(int i_index) {
    J3DAnmTexPattern* i_btp = getTexPtrnAnmP(l_arcNames[l_btpGetParamList[i_index].arcIdx], l_btpGetParamList[i_index].fileIdx);
    int i_attr = l_btpGetParamList[i_index].attr;
    mAnmFlags &= ~(ANM_FLAG_800 | ANM_PLAY_BTP | ANM_PAUSE_BTP);

    if (i_btp == NULL) {
        return true;
    }

    if (setBtpAnm(i_btp, mAnm_p->getModel()->getModelData(), 1.0f, i_attr)) {
        mAnmFlags |= ANM_PLAY_BTP | ANM_PAUSE_BTP;

        if (i_index == EXPR_BTP_INS) {
            mAnmFlags |= ANM_FLAG_800;
        }

        return true;
    }

    OS_REPORT("%s: 表情Btpアニメーションの登録に失敗しました！\n", __FILE__);

    return false;
}

void daNpcIns_c::setMotionAnm(int i_index, f32 i_morf) {
    if (i_index < 0xA || i_index >= 0x18) {
        return;
    }

    int btkIdx = 0;
    switch (i_index) {
        case ANM_SNIFF:
            btkIdx = 1;
            break;
    }

    J3DAnmTransformKey* i_bck = getTrnsfrmKeyAnmP(l_arcNames[l_bckGetParamList[i_index].arcIdx], l_bckGetParamList[i_index].fileIdx);
    J3DAnmTextureSRTKey* i_btk = getTexSRTKeyAnmP(l_arcNames[l_btkGetParamList[btkIdx].arcIdx], l_btkGetParamList[btkIdx].fileIdx);
    int bck_attr = l_bckGetParamList[i_index].attr;
    int btk_attr = l_btkGetParamList[btkIdx].attr;
    mAnmFlags &= ~ANM_MOTION_FLAGS;

    if (i_bck != NULL && setMcaMorfAnm(i_bck, 1.0f, i_morf, bck_attr, 0, -1)) {
        mAnmFlags |= ANM_PLAY_MORF | ANM_PAUSE_MORF;
        mMotionLoops = 0;
    }

    if (i_btk != NULL && setBtkAnm(i_btk, mAnm_p->getModel()->getModelData(), 1.0f, btk_attr)) {
        mAnmFlags |= ANM_PLAY_BTK | ANM_PAUSE_BTK;
    }   
}

void daNpcIns_c::reset() {
    initialize();
    mpMatAnm->initialize();
    mLookat.initialize();

    for (int i = 0; i < 1; i++) {
        mActorMngr[i].initialize();
    }
    

    field_0xdfc = 0;
    mRndValue = 0;
    mAction = NULL;
    mLookMode = -1;
    mMode = 0;
    mItemID = fpcM_ERROR_PROCESS_ID_e;
    mOrderEvtNo = 0;
    mExpressionMorfOverride = 0.0f;
    mMotionMorfOverride = 0.0f;
    speedF = 0.0f;
    speed.setall(0.0f);

    switch (mType) {
        case TYPE_0:
            current.pos.set(home.pos);
            break;
        
        case TYPE_1:
            setPath(getPathID1());
            getTargetPoint(0, &home.pos);
            current.pos = home.pos;
            break;

        case TYPE_2:
            setPath(getPathID2());
            getTargetPoint(0, &home.pos);
            current.pos = home.pos;
            break;
    }

    old.pos.set(current.pos);
    current.angle.set(0, home.angle.y, 0);
    old.angle = current.angle;
    shape_angle = current.angle;
    mCurAngle = current.angle;
    mOldAngle = mCurAngle;
    mCurrentPosY = current.pos.y;
    setWaitAction();
}

BOOL daNpcIns_c::ctrlBtk() {
    if (mpMatAnm != NULL) {
        J3DAnmTextureSRTKey* i_btk = getTexSRTKeyAnmP(l_arcNames[l_btkGetParamList[0].arcIdx], l_btkGetParamList[0].fileIdx);

        if (i_btk == mBtkAnm.getBtkAnm()) {
            mpMatAnm->setNowOffsetX(cM_ssin(mEyeAngle.y) * 0.2f * -1.0f);
            mpMatAnm->setNowOffsetY(cM_ssin(mEyeAngle.x) * 0.2f);
            mpMatAnm->onEyeMoveFlag();
            return TRUE;
        }

        mpMatAnm->offEyeMoveFlag();
    }

    return FALSE;
}

void daNpcIns_c::setAttnPos() {
    if (mLookMode == LOOK_RESET) {
        for (int i = 0; i < 3; i++) {
            mLookatAngle[i].setall(0);
        }
    }

    setMtx();
    lookat();
    cXyz sp2c(10.0f, 10.0f, 0.0f);
    mDoMtx_stack_c::copy(mAnm_p->getModel()->getAnmMtx(JNT_HEAD));
    mDoMtx_stack_c::multVecZero(&mHeadPos);
    mDoMtx_stack_c::multVec(&sp2c, &eyePos);
    sp2c.x = 0.0f;
    mDoMtx_stack_c::multVec(&sp2c, &sp2c);
    mHeadAngle.x = cLib_targetAngleX(&mHeadPos, &sp2c);
    mHeadAngle.y = cLib_targetAngleY(&mHeadPos, &sp2c);
    
    cXyz* attnPos = mLookat.getAttnPos();
    if (attnPos != NULL) {
        cXyz sp38 = *attnPos;
        sp38.y += 60.0f;
        sp38 -= eyePos;
        mEyeAngle.y = -(mLookatAngle[2].y + mCurAngle.y);
        mEyeAngle.y += cM_atan2s(sp38.x, sp38.z);
        mEyeAngle.x = mHeadAngle.x - cM_atan2s(sp38.y, sp38.absXZ());
    } else {
        mEyeAngle.y = 0;
        mEyeAngle.x = 0;
    }

    attention_info.position.set(mHeadPos.x, mHeadPos.y + daNpcIns_Param_c::m.common.attention_offset, mHeadPos.z);
    mCyl.SetC(current.pos);
    dComIfG_Ccsp()->Set(&mCyl);
}

void daNpcIns_c::lookat() {
    daPy_py_c* player = NULL;
    J3DModel* model = mAnm_p->getModel();
    BOOL i_snap = FALSE;
    f32 body_angleX_min = daNpcIns_Param_c::m.common.body_angleX_min;
    f32 body_angleX_max = daNpcIns_Param_c::m.common.body_angleX_max;
    f32 body_angleY_min = daNpcIns_Param_c::m.common.body_angleY_min;
    f32 body_angleY_max = daNpcIns_Param_c::m.common.body_angleY_max;
    f32 head_angleX_min = daNpcIns_Param_c::m.common.head_angleX_min;
    f32 head_angleX_max = daNpcIns_Param_c::m.common.head_angleX_max;
    f32 head_angleY_min = daNpcIns_Param_c::m.common.head_angleY_min;
    f32 head_angleY_max = daNpcIns_Param_c::m.common.head_angleY_max;
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

            if (mLookMode == LOOK_PLAYER_TALK) {
                head_angleY_min = -80.0f;
                head_angleY_max = 80.0f;
            }
            break;
    }

    if (mType == TYPE_0) {
        head_angleX_max = 5.0f;
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

    mLookat.setParam(body_angleX_min, body_angleX_max, body_angleY_min, body_angleY_max,
                     0.0f, 0.0f, 0.0f, 0.0f,
                     head_angleX_min, head_angleX_max, head_angleY_min, head_angleY_max,
                     mCurAngle.y, lookatPos);
    mLookat.calc(this, model->getBaseTRMtx(), lookatAngle, i_snap, angle_delta, FALSE);
}

void daNpcIns_c::setMtx() {
    J3DModel* model = mAnm_p->getModel();

    mDoMtx_stack_c::transS(current.pos.x, mCurrentPosY, current.pos.z);
    mDoMtx_stack_c::ZXYrotM(mCurAngle);
    model->setBaseTRMtx(mDoMtx_stack_c::get());
    model->setUserArea((uintptr_t)this);

    if (!cM3d_IsZero(mExpressionMorf)) {
        mAnm_p->onMorfNone();
    } else {
        mAnm_p->offMorfNone();
    }

    if ((mAnmFlags & ANM_PLAY_BCK) != 0) {
        mBckAnm.getBckAnm()->setFrame(mBckAnm.getFrame());
    }

    mAnm_p->modelCalc();
}

BOOL daNpcIns_c::setAction(actionFunc action) {
    mMode = 3;

    if (mAction) {
        (this->*mAction)(NULL);
    }

    mPrevAction = mAction;
    mMode = 0;
    mAction = action;

    if (mAction) {
        (this->*mAction)(NULL);
    }

    return TRUE;
}

void daNpcIns_c::waitEventMng() {
    if (dComIfGp_event_runCheck()) {
        if (eventInfo.checkCommandTalk()) {
            setAction(&daNpcIns_c::talk);
        } else if (dComIfGp_getEventManager().getMyStaffId(l_myName, NULL, 0) != -1) {
            setAction(&daNpcIns_c::demo);
        }
    } else {
        if (mOrderEvtNo != 0) {
            eventInfo.setArchiveName("ins");
        }

        orderEvent(mOrderSpeakEvent, l_evtNames[mOrderEvtNo], 0xFFFF, 0x28, 0xFF, 1);
    }
}

BOOL daNpcIns_c::step(s16 i_turnTargetAngle, int param_2) {
    if (mTurnMode == 0) {
        if (param_2 != 0) {
            if ((int)fabsf(cM_sht2d((s16)(i_turnTargetAngle - mCurAngle.y))) > 0x28) {
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

BOOL daNpcIns_c::chkFindPlayer() {
    BOOL rv;

    if (!chkActorInSight(daPy_getPlayerActorClass(), daNpcIns_Param_c::m.common.fov)) {
        mActorMngr[0].remove();
        return FALSE;
    }

    if (mActorMngr[0].getActorP() == NULL) {
        rv = chkPlayerInSpeakArea(this);
    } else {
        rv = chkPlayerInTalkArea(this);
    }

    if (rv) {
        mActorMngr[0].entry(daPy_getPlayerActorClass());
    } else {
        mActorMngr[0].remove();
    }

    return rv;
}

void daNpcIns_c::checkPlayerSearch() {
    if (mActorMngr[0].getActorP() != NULL) {
        if (!chkFindPlayer()) {
            mTurnMode = 0;
        }
    } else if (chkFindPlayer()) {
        mTurnMode = 0;
    }
}

int daNpcIns_c::waitShop(void* param_1) {
    switch (mMode) {
        case 0:
            setExpression(EXPR_NONE, -1.0f);
            setMotion(MOT_WAIT_A, -1.0f, 0);
            mTurnMode = 0;
            speedF = 0.0f;
            mMode = 2;
            break;
        
        case 2:
            checkPlayerSearch();

            if (mActorMngr[0].getActorP() != NULL) {
                setLookMode(LOOK_PLAYER);
            } else {
                setLookMode(LOOK_NONE);

                if (home.angle.y != mCurAngle.y && step(home.angle.y, 1)) {
                    setExpression(EXPR_NONE, -1.0f);
                    setMotion(MOT_WAIT_A, -1.0f, 0);
                    mTurnMode = 0;
                }
            }

            waitEventMng();
            break;

        case 3:
            break;

        default:
            JUT_ASSERT(1530, FALSE);
            break;
    }

    return 1;
}

void daNpcIns_c::setMotion(int i_motion, f32 i_morf, int param_3) {
    s16 motion = i_motion;

    switch (motion) {
        case MOT_STEP:
            if (mType != TYPE_0) {
                motion = MOT_K_STEP;
            }
            break;
        
        case MOT_K_STEP:
            if (mType == TYPE_0) {
                motion = MOT_STEP;
            }
            break;
    }

    if ((param_3 != 0 || mMotion != motion) && i_motion >= 0 && i_motion < 0xD) {
        mMotion = motion;
        mMotionMorfOverride = i_morf;
        mMotionPrevPhase = -1;
        mMotionPhase = 0;
    }
}

void daNpcIns_c::setExpression(int i_expression, f32 i_morf) {
    if (i_expression >= 0 && i_expression < 10) {
        mExpression = i_expression;
        mExpressionMorfOverride = i_morf;
        mExpressionPrevPhase = -1;
        mExpressionPhase = 0;
    }
}

int daNpcIns_c::waitPresent(void* param_1) {
    switch (mMode) {
        case 0:
            setExpression(EXPR_NONE, -1.0f);
            setMotion(MOT_WAIT_A, -1.0f, 0);
            setLookMode(LOOK_PLAYER);
            mTurnMode = 0;
            speedF = 0.0f;
            mMode = 2;
            break;
        
        case 2: {
            checkPlayerSearch();

            u8 type = dMeter2Info_getInsectSelectType();
            if (type != fpcNm_ITEM_NONE) {
                if (type != 0 && !isInsect(type)) {
                    break;
                }

                if (type != 0) {
                    OS_REPORT("Insects released. Type=%d\n", type);
                    mInsectMsgNo = getInsectMessageNo(type);
                    daNpcF_onEvtBit(getInsectEvtBitNo(type));
                    dMsgObject_setInsectItemNo(type);
                    daPy_py_c* player = daPy_getPlayerActorClass();
                    player->changeOriginalDemo();
                    player->changeDemoMode(0x25, 2, type, 0);
                } else {
                    mInsectMsgNo = 0x719;
                }

                dMeter2Info_setInsectSelectType(0xFF);
            }

            if (mInsectMsgNo != 0x719) {
                if (!daPy_getPlayerActorClass()->checkInsectRelease()) {
                    break;
                }
            }

            setAction(&daNpcIns_c::talk);
            break;
        }

        case 3:
            break;

        default:
            JUT_ASSERT(1585, FALSE);
            break;
    }

    return 1;
}

int daNpcIns_c::waitOutSide1(void* param_1) {
    switch (mMode) {
        case 0:
            setExpression(EXPR_NONE, -1.0f);
            setMotion(MOT_K_LOOKING, -1.0f, 0);
            setLookMode(LOOK_NONE);
            mRndValue = cLib_getRndValue(3, 5);
            mTurnMode = 0;
            speedF = 0.0f;
            mMode = 2;
            break;
        
        case 2:
            if (home.angle.y != mCurAngle.y && step(home.angle.y, 1)) {
                setExpression(EXPR_NONE, -1.0f);
                setMotion(MOT_K_LOOKING, -1.0f, 0);
                mRndValue = cLib_getRndValue(3, 5);
                mTurnMode = 0;
            }

            if (mMotionLoops >= mRndValue) {
                if (mMotion == 8) {
                    setMotion(MOT_K_SEARCHING_A, -1.0f, 0);
                    mRndValue = cLib_getRndValue(1, 3);
                } else {
                    setMotion(MOT_K_LOOKING, -1.0f, 0);
                    mRndValue = cLib_getRndValue(3, 5);
                }
            }

            if (getTime() >= mGoHomeTime) {
                setAction(&daNpcIns_c::goHome);
            }

            mSound.startCreatureVoiceLevel(Z2SE_INS_V_UFUFU, -1);
            waitEventMng();
            break;

        case 3:
            break;

        default:
            JUT_ASSERT(1642, FALSE);
            break;
    }

    return 1;
}

int daNpcIns_c::waitOutSide2(void* param_1) {
    switch (mMode) {
        case 0:
            setExpression(EXPR_NONE, -1.0f);
            setMotion(MOT_K_SEARCHING_B, -1.0f, 0);
            setLookMode(LOOK_NONE);
            mTurnMode = 0;
            speedF = 0.0f;
            mMode = 2;
            break;
        
        case 2:
            if (getTime() >= mGoHomeTime) {
                setAction(&daNpcIns_c::goHome);
            }

            mSound.startCreatureVoiceLevel(Z2SE_INS_V_UFUFU, -1);
            waitEventMng();
            break;

        case 3:
            break;

        default:
            JUT_ASSERT(1679, FALSE);
            break;
    }

    return 1;
}

int daNpcIns_c::goHome(void* param_1) {
    switch (mMode) {
        case 0: {
            cXyz targetPoint;

            JUT_ASSERT(1702, mPath != NULL);

            getTargetPoint(mPntIndex, &targetPoint);

            if (step(cLib_targetAngleY(&current.pos, &targetPoint), 1)) {
                setExpression(EXPR_NONE, -1.0f);
                setMotion(MOT_K_WALK, -1.0f, 0);
                speedF = daNpcIns_Param_c::m.walk_speed;
                mInsectMsgNo = 0x27;
                mMode = 2;
            }
            break;
        }
        
        case 2:
            checkPlayerSearch();

            if (mActorMngr[0].getActorP() != NULL) {
                setLookMode(LOOK_PLAYER);
            } else {
                setLookMode(LOOK_NONE);
            }

            speedF = daNpcIns_Param_c::m.walk_speed;

            if (checkPoint(current.pos, speedF) && setNextPoint()) {
                fopAcM_delete(this);
            }

            pathMoveF();

            if (current.pos.y != old.pos.y) {
                cM3dGPla plane;
                s16 sVar1;
                if (dComIfG_Bgsp().GetTriPla(mAcch.m_gnd, &plane)) {
                    sVar1 = (s16)fopAcM_getPolygonAngle(&plane, shape_angle.y);
                } else {
                    sVar1 = 0;
                }

                if (sVar1 != 0) {
                    mCurrentPosY = current.pos.y;
                }
            }

            cLib_addCalc2(&mCurrentPosY, current.pos.y, 0.4f, 25.0f);
            waitEventMng();
            break;

        case 3:
            break;

        default:
            JUT_ASSERT(1750, FALSE);
            break;
    }

    return 0;
}

void daNpcIns_c::setExpressionTalkAfter() {
    switch (mExpression) {
        case EXPR_SNIFF:
            setExpression(EXPR_H_SNIFF, -1.0f);
            break;

        case EXPR_SURPRISED:
            setExpression(EXPR_H_SURPRISED, -1.0f);
            break;
        
        case EXPR_HAPPY:
            setExpression(EXPR_H_HAPPY, -1.0f);
            break;

        case EXPR_MAD:
            setExpression(EXPR_H_MAD, -1.0f);
            break;

        default:
            setExpression(EXPR_NONE, -1.0f);
            break;
    }
}

int daNpcIns_c::talk(void* param_1) {
    int rv = 0;
    int itemNo;

    switch (mMode) {
        case 0:
            if (daPy_py_c::checkNowWolf()) {
                mInsectMsgNo = 0x25;
            }

            initTalk(mInsectMsgNo, NULL);
            mTurnMode = 0;
            mMsgTimer = 0;
            mOrderSpeakEvent = 0;
            field_0xe14 = fopAcM_searchPlayerAngleY(this);
            setLookMode(LOOK_PLAYER_TALK);
            mMode = 2;
            break;
        
        case 2:
            if (field_0xe14 == mCurAngle.y) {
                if (talkProc(NULL, TRUE, NULL)) {
                    mActorMngr[0].entry(daPy_getPlayerActorClass());

                    if (mType == TYPE_0) {
                        itemNo = 0;
                        u32 eventID = mFlow.getEventId(&itemNo);

                        OS_REPORT("会話終了時 イベントID=%d アイテムNo=%d\n", eventID, itemNo);

                        if (eventID == 1) {
                            mItemID = fopAcM_createItemForPresentDemo(&current.pos, itemNo, 0, -1, -1, NULL, NULL);

                            if (mItemID != fpcM_ERROR_PROCESS_ID_e) {
                                daPy_getPlayerActorClass()->cancelOriginalDemo();
                                s16 eventIdx = dComIfGp_getEventManager().getEventIdx(this, "DEFAULT_GETITEM", 0xFF);
                                dComIfGp_getEvent()->reset(this);
                                fopAcM_orderChangeEventId(this, eventIdx, 1, 0xFFFF);
                                field_0x9ec = true;
                                mOrderSpeakEvent = 1;
                            }
                        } else if (eventID == 0x18 && mInsectMsgNo == field_0xe16) {
                            dMeter2Info_setPauseStatus(8);
                            field_0x9ec = true;
                            setAction(&daNpcIns_c::waitPresent);
                            rv = 1;
                            break;
                        }

                        setAction(&daNpcIns_c::waitShop);
                    } else {
                        setAction(mPrevAction);
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
            } else if (step(field_0xe14, 1)) {
                setMotion(MOT_WAIT_A, -1.0f, 0);
                mTurnMode = 0;
            }
            break;

        case 3:
            mInsectMsgNo = field_0xe16;
            setExpression(EXPR_NONE, -1.0f);

            if (!field_0x9ec) {
                dComIfGp_event_reset();
            }
            break;

        default:
            JUT_ASSERT(1848, FALSE);
            break;
    }

    return rv;
}

int daNpcIns_c::demo(void* param_1) {
    dEvent_manager_c* eventManager = &dComIfGp_getEventManager();
    int iVar1 = 0;

    switch (mMode) {
        case 0:
            setExpression(EXPR_NONE, -1.0f);
            setMotion(MOT_WAIT_A, -1.0f, 0);
            mMode = 2;
            // fallthrough        
        case 2:
            if (dComIfGp_event_runCheck() && !eventInfo.checkCommandTalk()) {
                eventManager = &dComIfGp_getEventManager();
                s32 staffId = eventManager->getMyStaffId(l_myName, NULL, 0);
                if (staffId != -1) {
                    mStaffID = staffId;

                    JUT_ASSERT(1882, NULL != mEvtSeqList[mOrderEvtNo]);

                    if ((this->*mEvtSeqList[mOrderEvtNo])(staffId)) {
                        eventManager->cutEnd(staffId);
                    }

                    iVar1 = 1;
                }

                if (eventInfo.checkCommandDemoAccrpt() && mEventIdx != -1 && eventManager->endCheck(mEventIdx)) {
                    dComIfGp_event_reset();
                    mOrderEvtNo = 0;
                    mEventIdx = -1;
                    setWaitAction();
                }
            }
            break;

        case 3:
            break;

        default:
            JUT_ASSERT(1910, FALSE);
            break;
    }

    return 1;
}

bool daNpcIns_c::isInsectComplete() {
    const insect_param_data* data_p = l_insectParams;

    for (int i = 0; i < 0x18; i++, data_p++) {
        if (!daNpcF_chkEvtBit(data_p->evt_bit_no)) {
            return false;
        }
    }

    return true;
}

const insect_param_data& daNpcIns_c::getInsectParamData(int i_index) {
    switch (i_index) {
        case fpcNm_ITEM_M_BEETLE:
            i_index = INSECT_M_BEETLE;
            break;

        case fpcNm_ITEM_F_BEETLE:
            i_index = INSECT_F_BEETLE;
            break;

        case fpcNm_ITEM_M_BUTTERFLY:
            i_index = INSECT_M_BUTTERFLY;
            break;

        case fpcNm_ITEM_F_BUTTERFLY:
            i_index = INSECT_F_BUTTERFLY;
            break;

        case fpcNm_ITEM_M_STAG_BEETLE:
            i_index = INSECT_M_STAG_BEETLE;
            break;

        case fpcNm_ITEM_F_STAG_BEETLE:
            i_index = INSECT_F_STAG_BEETLE;
            break;

        case fpcNm_ITEM_M_GRASSHOPPER:
            i_index = INSECT_M_GRASSHOPPER;
            break;

        case fpcNm_ITEM_F_GRASSHOPPER:
            i_index = INSECT_F_GRASSHOPPER;
            break;

        case fpcNm_ITEM_M_NANAFUSHI:
            i_index = INSECT_M_NANAFUSHI;
            break;

        case fpcNm_ITEM_F_NANAFUSHI:
            i_index = INSECT_F_NANAFUSHI;
            break;

        case fpcNm_ITEM_M_DANGOMUSHI:
            i_index = INSECT_M_DANGOMUSHI;
            break;

        case fpcNm_ITEM_F_DANGOMUSHI:
            i_index = INSECT_F_DANGOMUSHI;
            break;

        case fpcNm_ITEM_M_MANTIS:
            i_index = INSECT_M_MANTIS;
            break;

        case fpcNm_ITEM_F_MANTIS:
            i_index = INSECT_F_MANTIS;
            break;

        case fpcNm_ITEM_M_LADYBUG:
            i_index = INSECT_M_LADYBUG;
            break;

        case fpcNm_ITEM_F_LADYBUG:
            i_index = INSECT_F_LADYBUG;
            break;

        case fpcNm_ITEM_M_SNAIL:
            i_index = INSECT_M_SNAIL;
            break;

        case fpcNm_ITEM_F_SNAIL:
            i_index = INSECT_F_SNAIL;
            break;

        case fpcNm_ITEM_M_DRAGONFLY:
            i_index = INSECT_M_DRAGONFLY;
            break;

        case fpcNm_ITEM_F_DRAGONFLY:
            i_index = INSECT_F_DRAGONFLY;
            break;

        case fpcNm_ITEM_M_ANT:
            i_index = INSECT_M_ANT;
            break;

        case fpcNm_ITEM_F_ANT:
            i_index = INSECT_F_ANT;
            break;

        case fpcNm_ITEM_M_MAYFLY:
            i_index = INSECT_M_MAYFLY;
            break;

        case fpcNm_ITEM_F_MAYFLY:
            i_index = INSECT_F_MAYFLY;
            break;

        default:
            JUT_ASSERT(1965, FALSE);
            break;
    }

    return l_insectParams[i_index];
}

void daNpcIns_c::setWaitAction() {
    switch (mType) {
        case TYPE_0:
            setAction(&daNpcIns_c::waitShop);
            break;
        
        case TYPE_1:
            setAction(&daNpcIns_c::waitOutSide1);
            break;

        case TYPE_2:
            setAction(&daNpcIns_c::waitOutSide2);
            break;
    }
}

bool daNpcIns_c::setPath(int path_index) {
    mPath = dPath_GetRoomPath(path_index, fopAcM_GetRoomNo(this));

    if (mPath == NULL) {
        return false;
    }

    mPntIndex = 1;
    field_0xe0c = current.pos.absXZ(dPath_GetPnt(mPath, mPntIndex)->m_position);
    return true;
}

bool daNpcIns_c::checkPoint(cXyz& param_1, f32 param_2) {
    f32 fVar1 = param_1.absXZ(dPath_GetPnt(mPath, mPntIndex)->m_position);
    if (fVar1 < param_2 || fVar1 < 10.0f) {
        return true;
    }

    field_0xe0c -= param_2;

    if (field_0xe0c < 0.0f) {
        return true;
    }

    return false;
}

bool daNpcIns_c::setNextPoint() {
    dPath_GetPnt(mPath, mPntIndex);
    mPntIndex++;

    if (mPntIndex >= mPath->m_num) {
        mPntIndex = mPath->m_num - 1;
        field_0xe0c = 0.0f;
        return true;
    }

    dPath_GetPnt(mPath, mPntIndex);
    field_0xe0c = current.pos.absXZ(dPath_GetPnt(mPath, mPntIndex)->m_position);
    return false;
}

void daNpcIns_c::getTargetPoint(int pnt_index, Vec* param_2) {
    if (mPath != NULL) {
        *param_2 = dPath_GetPnt(mPath, pnt_index)->m_position;
    }
}

void daNpcIns_c::pathMoveF() {
    if (mPath != NULL) {
        cXyz sp18;
        getTargetPoint(mPntIndex, &sp18);
        cLib_addCalcAngleS2(&mCurAngle.y, cLib_targetAngleY(&current.pos, &sp18), 13, 0x600);
        setAngle(mCurAngle.y);
        fopAcM_posMoveF(this, NULL);
    }
}

static int daNpcIns_Create(void* a_this) {
    return static_cast<daNpcIns_c*>(a_this)->Create();
}

static int daNpcIns_Delete(void* a_this) {
    return static_cast<daNpcIns_c*>(a_this)->Delete();
}

static int daNpcIns_Execute(void* a_this) {
    return static_cast<daNpcIns_c*>(a_this)->Execute();
}

static int daNpcIns_Draw(void* a_this) {
    return static_cast<daNpcIns_c*>(a_this)->Draw();
}

static int daNpcIns_IsDelete(void* a_this) {
    return 1;
}

static actor_method_class daNpcIns_MethodTable = {
    (process_method_func)daNpcIns_Create,
    (process_method_func)daNpcIns_Delete,
    (process_method_func)daNpcIns_Execute,
    (process_method_func)daNpcIns_IsDelete,
    (process_method_func)daNpcIns_Draw,
};

actor_process_profile_definition g_profile_NPC_INS = {
  fpcLy_CURRENT_e,        // mLayerID
  7,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_NPC_INS,           // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(daNpcIns_c),     // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  425,                    // mPriority
  &daNpcIns_MethodTable,  // sub_method
  0x00040100,             // mStatus
  fopAc_NPC_e,            // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};
