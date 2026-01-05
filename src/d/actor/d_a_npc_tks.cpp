/**
 * @file d_a_npc_tks.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_npc_tks.h"
#include "d/actor/d_a_npc.h"
#include "d/actor/d_a_npc_tkc.h"
#include "d/d_a_obj.h"
#include "d/d_meter2_info.h"
#include "f_op/f_op_kankyo_mng.h"

enum TKS_RES_File_ID {
    /* BCK */
    /* 0x07 */ BCK_TKS_F_TALK_A = 0x7,
    /* 0x08 */ BCK_TKS_FALL,
    /* 0x09 */ BCK_TKS_FLY,
    /* 0x0A */ BCK_TKS_HIDE,
    /* 0x0B */ BCK_TKS_IN,
    /* 0x0C */ BCK_TKS_JUMP_E,
    /* 0x0D */ BCK_TKS_JUMP_S,
    /* 0x0E */ BCK_TKS_LAND,
    /* 0x0F */ BCK_TKS_LOOKAROUND_A,
    /* 0x10 */ BCK_TKS_LOOKAROUND_B,
    /* 0x11 */ BCK_TKS_RUN,
    /* 0x12 */ BCK_TKS_SHAKE,
    /* 0x13 */ BCK_TKS_SHOW_UP,
    /* 0x14 */ BCK_TKS_SHOW_UP_WAIT,
    /* 0x15 */ BCK_TKS_SWIM,
    /* 0x16 */ BCK_TKS_TUBO_HIDE,
    /* 0x17 */ BCK_TKS_TUBO_IN,
    /* 0x18 */ BCK_TKS_TUBO_SHAKE,
    /* 0x19 */ BCK_TKS_TUBO_SHOW_UP,
    /* 0x1A */ BCK_TKS_WAIT_A,
    /* 0x1B */ BCK_TKS_WALK_A,

    /* BMDR */
    /* 0x1E */ BMDR_TKS = 0x1E,

    /* BTK */
    /* 0x21 */ BTK_TKS = 0x21,

    /* BTP */
    /* 0x24 */ BTP_TKS = 0x24,

    /* EVT */
    /* 0x27 */ EVT_TKS_EVENT_LIST = 0x27,
};

enum J_tubo_00_RES_File_ID {
    /* BMDR */
    /* 0x3 */ BMDR_J_TUBO_00 = 0x3,
};

enum K_tubo02_RES_File_ID {
    /* BMDR */
    /* 0x3 */ BMDR_K_TUBO02 = 0x3,
};

enum Animation {
    /* 0x00 */ ANM_NONE,
    /* 0x01 */ ANM_F_TALK_A,
    /* 0x02 */ ANM_WAIT_A,
    /* 0x03 */ ANM_WALK_A,
    /* 0x04 */ ANM_HIDE,
    /* 0x05 */ ANM_SHOW_UP,
    /* 0x06 */ ANM_SHOW_UP_WAIT,
    /* 0x07 */ ANM_LOOKAROUND_A,
    /* 0x08 */ ANM_LOOKAROUND_B,
    /* 0x09 */ ANM_IN,
    /* 0x0A */ ANM_SHAKE,
    /* 0x0B */ ANM_FALL,
    /* 0x0C */ ANM_LAND,
    /* 0x0D */ ANM_TUBO_HIDE,
    /* 0x0E */ ANM_TUBO_SHOW_UP,
    /* 0x0F */ ANM_TUBO_IN,
    /* 0x10 */ ANM_TUBO_SHAKE,
    /* 0x11 */ ANM_JUMP_S,
    /* 0x12 */ ANM_FLY,
    /* 0x13 */ ANM_JUMP_E,
    /* 0x14 */ ANM_RUN,
    /* 0x15 */ ANM_SWIM,
};

enum Motion {
    /* 0x0 */ MOT_WAIT_A,
    /* 0x1 */ MOT_SWIM,
    /* 0x2 */ MOT_RUN,
    /* 0x3 */ MOT_WALK_A,
    /* 0x4 */ MOT_JUMP_S,
    /* 0x5 */ MOT_JUMP_E,
    /* 0x6 */ MOT_LAND,
    /* 0x7 */ MOT_WALK_A_LOOP,
};

enum Expression {
    /* 0x0 */ EXPR_TALK_A,
    /* 0x1 */ EXPR_NONE,
};

enum Expression_BTP {
    /* 0x0 */ EXPR_BTP_TKS,
};

enum Joint {
    /* 0x00 */ JNT_CENTER,
    /* 0x01 */ JNT_ARML1,
    /* 0x02 */ JNT_ARML2,
    /* 0x03 */ JNT_HANDL,
    /* 0x04 */ JNT_TUBASAL,
    /* 0x05 */ JNT_ARMR1,
    /* 0x06 */ JNT_ARMR2,
    /* 0x07 */ JNT_HANDR,
    /* 0x08 */ JNT_TUBASAR,
    /* 0x09 */ JNT_LEGL,
    /* 0x0A */ JNT_FOOTL1,
    /* 0x0B */ JNT_FOOTL2,
    /* 0x0C */ JNT_LEGR,
    /* 0x0D */ JNT_FOOTR1,
    /* 0x0E */ JNT_FOOTR2,
    /* 0x0F */ JNT_NECK1,
    /* 0x10 */ JNT_NECK2,
    /* 0x11 */ JNT_HEAD,
    /* 0x12 */ JNT_CHIN,
    /* 0x13 */ JNT_MOUTH,
};

enum RES_Name {
    /* 0x0 */ TKS,
    /* 0x1 */ J_TUBO_00,
    /* 0x2 */ K_TUBO02,
};

static daNpcTks_Param_c l_HIO;

static daNpc_GetParam3 l_bckGetParamList[22] = {
    {-1, J3DFrameCtrl::EMode_LOOP},
    {BCK_TKS_F_TALK_A, J3DFrameCtrl::EMode_NONE},
    {BCK_TKS_WAIT_A, J3DFrameCtrl::EMode_LOOP},
    {BCK_TKS_WALK_A, J3DFrameCtrl::EMode_LOOP},
    {BCK_TKS_HIDE, J3DFrameCtrl::EMode_LOOP},
    {BCK_TKS_SHOW_UP, J3DFrameCtrl::EMode_NONE},
    {BCK_TKS_SHOW_UP_WAIT, J3DFrameCtrl::EMode_LOOP},
    {BCK_TKS_LOOKAROUND_A, J3DFrameCtrl::EMode_NONE},
    {BCK_TKS_LOOKAROUND_B, J3DFrameCtrl::EMode_NONE},
    {BCK_TKS_IN, J3DFrameCtrl::EMode_NONE},
    {BCK_TKS_SHAKE, J3DFrameCtrl::EMode_NONE},
    {BCK_TKS_FALL, J3DFrameCtrl::EMode_LOOP},
    {BCK_TKS_LAND, J3DFrameCtrl::EMode_NONE},
    {BCK_TKS_TUBO_HIDE, J3DFrameCtrl::EMode_LOOP},
    {BCK_TKS_TUBO_SHOW_UP, J3DFrameCtrl::EMode_NONE},
    {BCK_TKS_TUBO_IN, J3DFrameCtrl::EMode_NONE},
    {BCK_TKS_TUBO_SHAKE, J3DFrameCtrl::EMode_NONE},
    {BCK_TKS_JUMP_S, J3DFrameCtrl::EMode_NONE},
    {BCK_TKS_FLY, J3DFrameCtrl::EMode_LOOP},
    {BCK_TKS_JUMP_E, J3DFrameCtrl::EMode_NONE},
    {BCK_TKS_RUN, J3DFrameCtrl::EMode_LOOP},
    {BCK_TKS_SWIM, J3DFrameCtrl::EMode_LOOP},
};

static daNpc_GetParam3 l_btpGetParamList[1] = {
    {BTP_TKS, J3DFrameCtrl::EMode_LOOP},
};

static daNpc_GetParam3 l_btkGetParamList[1] = {
    {BTK_TKS, J3DFrameCtrl::EMode_LOOP},
};

static int l_loadRes_TKS0[3] = {
    TKS, J_TUBO_00, -1,
};

static int l_loadRes_TKS1[3] = {
    TKS, K_TUBO02, -1,
};

static int l_loadRes_TKS2[3] = {
    TKS, -1, -1,
};

static int* l_loadRes_list[3] = {
    l_loadRes_TKS0,
    l_loadRes_TKS1,
    l_loadRes_TKS2,
};

static char* l_arcNames[3] = {
    "TKS",
    "J_tubo_00",
    "K_tubo02",
};

static char* l_evtNames[2] = {
    NULL,
    "TKS_ENCOUNTER",
};

static char* l_arcName = "TKS";

static char* l_myName = "Tks";

daNpcTks_c::daNpcTks_c() {}

daNpcTks_c::~daNpcTks_c() {
    for (int i = 0; l_loadRes_list[mTksTsubo.mTsuboType][i] >= 0; i++) {
        dComIfG_resDelete(&mPhases[i], l_arcNames[l_loadRes_list[mTksTsubo.mTsuboType][i]]);
    }

    if (heap != NULL) {
        mAnm_p->stopZelAnime();
    }

    if (parentActorID != fpcM_ERROR_PROCESS_ID_e) {
        fopAcM_delete(parentActorID);
    }
}

daNpcTks_HIOParam const daNpcTks_Param_c::m = {
    90.0f,
    -3.0f,
    1.0f,
    150.0f,
    255.0f,
    80.0f,
    33.0f,
    25.0f,
    0.0f,
    0.0f,
    45.0f,
    -45.0f,
    30.0f,
    -10.0f,
    45.0f,
    -45.0f,
    0.8f,
    12.0f,
    3,
    6,
    19,
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
    0x2D,
    0x96,
    30,
    0x5A,
    1000.0f,
    500.0f,
    35.0f,
    25.0f,
    7.0f,
    2.0f,
    5.0f,
};

cPhs__Step daNpcTks_c::Create() {
    fopAcM_ct(this, daNpcTks_c);

    if (dComIfGs_isStageBossEnemy()) {
        return cPhs_ERROR_e;
    }

    if (getType() == 0 || getType() == 0xFF || getType() == 6) {
        if (dComIfGs_isDungeonItemWarp()) {
            return cPhs_ERROR_e;
        }
    } else if (getType() == 3 &&
        /* dSv_event_flag_c::F_0267 - Temple of Time - Temple of Time clear */
        (!daNpcF_chkEvtBit(267) ||
        /* dSv_event_flag_c::F_0545 - Sacred Grove - Watched cutscene of Ooccoo Sr. parting (after Temple of Time) */
        daNpcF_chkEvtBit(545))) {
        return cPhs_ERROR_e;
    }

    if ((getType() == 0 || getType() == 0xFF) && !fopAcM_isSwitch(this, getSwitchNo())) {
        mTksTsubo.mTsuboType = getTsuboType();
    } else {
        mTksTsubo.mTsuboType = 2;
    }

    mMessageNo = getMessageNo();

    cPhs__Step phase = cPhs_ERROR_e;
    for (int i = 0; l_loadRes_list[mTksTsubo.mTsuboType][i] >= 0; i++) {
        phase = (cPhs__Step)dComIfG_resLoad(&mPhases[i], l_arcNames[l_loadRes_list[mTksTsubo.mTsuboType][i]]);
        if (phase != cPhs_COMPLEATE_e) {
            return phase;
        }
    }

    if (phase == cPhs_COMPLEATE_e) {
        if (!fopAcM_entrySolidHeap(this, createHeapCallBack, 0x3870)) {
            return cPhs_ERROR_e;
        }

        fopAcM_SetMtx(this, mAnm_p->getModel()->getBaseTRMtx());
        fopAcM_setCullSizeBox(this, -80.0f, -30.0f, -80.0f, 80.0f, 10.0f, 80.0f);
        mSound.init(&current.pos, &eyePos, 3, 1);
        mTksTsubo.mSound.init(&mTksTsubo.mPos, &mTksTsubo.mPos, 3, 1);

        mAcchCir.SetWall(daNpcTks_Param_c::m.common.width, daNpcTks_Param_c::m.common.knee_length);
        mAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1, &mAcchCir,
                  fopAcM_GetSpeed_p(this), fopAcM_GetAngle_p(this), fopAcM_GetShapeAngle_p(this));
        mAcch.SetRoofNone();
        mAcch.CrrPos(dComIfG_Bgsp());
        mGndChk = mAcch.m_gnd;
        mGroundH = mAcch.GetGroundH();

        mCcStts.Init(daNpcTks_Param_c::m.common.weight, 0, this);
        mCyl.Set(mCcDCyl);
        mCyl.SetStts(&mCcStts);
        mCyl.SetTgType(0);
        mCyl.SetTgSPrm(0);
        mCyl.SetH(daNpcTks_Param_c::m.common.height);
        mCyl.SetR(daNpcTks_Param_c::m.common.width);
        mTksTsubo.mCyl.Set(mCcDCyl);
        mTksTsubo.mCyl.SetStts(&mCcStts);
        mTksTsubo.mCyl.SetTgType(0xD8FBFDFF);
        mTksTsubo.mCyl.SetTgSPrm(17);
        mTksTsubo.mCyl.SetH(100.0f);
        mTksTsubo.mCyl.SetR(30.0f);

        setEnvTevColor();
        setRoomNo();
        reset();
        Execute();
    }

    return phase;
}

int daNpcTks_c::CreateHeap() {
    J3DModelData* mdlData_p = (J3DModelData*)dComIfG_getObjectRes(l_arcName, BMDR_TKS);

    JUT_ASSERT(410, NULL != mdlData_p);

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
    model->setUserArea((u32)this);

    mpMatAnm = new daNpcF_MatAnm_c();
    if (mpMatAnm == NULL) {
        return 0;
    }

    if (!setExpressionAnm(ANM_F_TALK_A, false)) {
        return 0;
    }

    setMotion(MOT_WAIT_A, -1.0f, 0);

    if (mTksTsubo.mTsuboType < 2) {
        const char* sp30 = mTksTsubo.mTsuboType != 0 ? l_arcNames[2] : l_arcNames[1];
        int sp2C = mTksTsubo.mTsuboType != 0 ? 3 : 3;
        mdlData_p = static_cast<J3DModelData*>(dComIfG_getObjectRes(sp30, sp2C));

        JUT_ASSERT(453, NULL != mdlData_p);
        
        mTksTsubo.mpModelMorf = new mDoExt_McaMorfSO(mdlData_p, NULL, NULL, getTrnsfrmKeyAnmP(l_arcName, BCK_TKS_TUBO_HIDE), -1, 1.0f, 0, -1, &mTksTsubo.mSound, 0x80000, 0x11000084);
        if (mTksTsubo.mpModelMorf == NULL) {
            return 0;
        }
    }

    return 1;
}

int daNpcTks_c::Delete() {
    this->~daNpcTks_c();
    return 1;
}

int daNpcTks_c::Execute() {
    execute();
    return 1;
}

int daNpcTks_c::Draw() {
    mAnm_p->getModel()->getModelData()->getMaterialNodePointer(2)->setMaterialAnm(mpMatAnm);
    draw(FALSE, FALSE, daNpcTks_Param_c::m.common.real_shadow_size, NULL, FALSE);
    return 1;
}

int daNpcTks_c::ctrlJoint(J3DJoint* i_joint, J3DModel* i_model) {
    int jntNo = i_joint->getJntNo();
    int i_jointList[3] = {JNT_NECK1, JNT_NECK2, JNT_HEAD};

    if (jntNo == 0) {
        mDoMtx_stack_c::copy(mAnm_p->getModel()->getAnmMtx(JNT_NECK1));
        mDoMtx_stack_c::multVecZero(&mLookatPos[0]);
        mDoMtx_stack_c::copy(mAnm_p->getModel()->getAnmMtx(JNT_NECK2));
        mDoMtx_stack_c::multVecZero(&mLookatPos[1]);
        mDoMtx_stack_c::copy(mAnm_p->getModel()->getAnmMtx(JNT_HEAD));
        mDoMtx_stack_c::multVecZero(&mLookatPos[2]);
    }

    mDoMtx_stack_c::copy(i_model->getAnmMtx(jntNo));

    switch (jntNo) {
        case JNT_NECK1:
        case JNT_NECK2:
        case JNT_HEAD:
            setLookatMtx(jntNo, i_jointList, daNpcTks_Param_c::m.common.neck_rotation_ratio);
            break;
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

int daNpcTks_c::createHeapCallBack(fopAc_ac_c* a_this) {
    return static_cast<daNpcTks_c*>(a_this)->CreateHeap();
}

int daNpcTks_c::ctrlJointCallBack(J3DJoint* i_joint, int param_2) {
    if (param_2 == 0) {
        J3DModel* model = j3dSys.getModel();
        daNpcTks_c* i_this = (daNpcTks_c*)model->getUserArea();

        if (i_this != NULL) {
            i_this->ctrlJoint(i_joint, model);
        }
    }

    return 1;
}

void daNpcTks_c::setMtx() {
    J3DModel* model = mAnm_p->getModel();

    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::ZXYrotM(shape_angle);
    mDoMtx_stack_c::scaleM(scale);
    model->setBaseTRMtx(mDoMtx_stack_c::get());
    model->setUserArea((u32)this);

    if (mTksTsubo.field_0x586 != 0) {
        if (field_0x138a) {
            mTksTsubo.mPos = current.pos;
        } else {
            mDoMtx_stack_c::transS(mTksTsubo.mPos);
            mDoMtx_stack_c::ZXYrotM(mTksTsubo.mAngle);
        }

        model = mTksTsubo.mpModelMorf->getModel();
        model->setBaseTRMtx(mDoMtx_stack_c::get());
        mTksTsubo.mpModelMorf->modelCalc();
        mTksTsubo.mpModelMorf->getModel()->getModelData()->getJointNodePointer(0)->setMtxCalc(NULL);
    }

    if ((mAnmFlags & ANM_PLAY_BCK) != 0) {
        mBckAnm.getBckAnm()->setFrame(mBckAnm.getFrame());
    }

    mAnm_p->modelCalc();
}

bool daNpcTks_c::setExpressionAnm(int i_index, bool i_modify) {
    mAnmFlags &= 0xFFFFE03F;

    J3DAnmTransform* i_bck = l_bckGetParamList[i_index].fileIdx >= 0 ? getTrnsfrmKeyAnmP(l_arcName, l_bckGetParamList[i_index].fileIdx) : NULL;
    int i_attr = l_bckGetParamList[i_index].attr;
    bool bVar1 = FALSE;
    
    switch (i_index) {
        case ANM_NONE:
            bVar1 = setExpressionBtp(EXPR_BTP_TKS);
            break;
        
        case ANM_F_TALK_A:
            bVar1 = setExpressionBtp(EXPR_BTP_TKS);
            break;

        default:
            i_bck = NULL;
            break;
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

    OS_REPORT("%s: 表情Bckアニメーションの登録に失敗しました！\n", "d_a_npc_tks.cpp");

    return false;
}

bool daNpcTks_c::setExpressionBtp(int i_index) {
    J3DAnmTexPattern* i_btp = getTexPtrnAnmP(l_arcName, l_btpGetParamList[i_index].fileIdx);
    int i_attr = l_btpGetParamList[i_index].attr;

    mAnmFlags &= 0xFFFFF57F;

    if (i_btp == NULL) {
        return true;
    }

    if (setBtpAnm(i_btp, mAnm_p->getModel()->getModelData(), 1.0f, i_attr)) {
        mAnmFlags |= ANM_PLAY_BTP | ANM_PAUSE_BTP;

        if (i_index == 0) {
            mAnmFlags |= ANM_FLAG_800;
        }

        return true;
    }

    OS_REPORT("%s: 表情Btpアニメーションの登録に失敗しました！\n", "d_a_npc_tks.cpp");

    return false;
}

void daNpcTks_c::setMotion(int i_motion, f32 i_morf, int param_3) {
    s16 motion = i_motion;
    if ((param_3 != 0 || mMotion != motion) && i_motion >= 0 && i_motion < 8) {
        mMotion = motion;
        mMotionMorfOverride = i_morf;
        mMotionPrevPhase = -1;
        mMotionPhase = 0;
    }
}

BOOL daNpcTks_c::main() {
    if (field_0x138a && mTksTsubo.mCyl.ChkTgHit()) {
        if (mTksTsubo.mCyl.GetTgHitObj()->ChkAtType(AT_TYPE_BOOMERANG)) {
            setAction(&daNpcTks_c::damage);
        } else {
            setAction(&daNpcTks_c::broken);
        }
    }

    if (cLib_checkBit<u32>(attention_info.flags, fopAc_AttnFlag_CARRY_e) && fopAcM_checkCarryNow(this)) {
        setAction(&daNpcTks_c::demo_appear);
    }

    if (dComIfGp_event_runCheck() && !eventInfo.checkCommandTalk() && mItemID != fpcM_ERROR_PROCESS_ID_e) {
        dComIfGp_event_setItemPartnerId(mItemID);
        mItemID = fpcM_ERROR_PROCESS_ID_e;
        fopAcM_delete(this);
    }

    if (mAction) {
        (this->*mAction)();
    }

    playExpression();
    playMotion();
    playTsuboAnm();
    mPlayerArea = getPlayerArea();

    return TRUE;
}

void daNpcTks_c::reset() {
    initialize();
    mpMatAnm->initialize();
    mLookat.initialize();

    for (int i = 0; i < 1; i++) {
        mActorMngr[i].initialize();
    }

    attention_info.distances[fopAc_attn_CARRY_e] = 43;
    attention_info.flags = fopAc_AttnFlag_CARRY_e;
    field_0x1370 = 0;
    field_0x1374 = 0;
    mAction = NULL;
    mLookMode = -1;
    mMode = 0;
    field_0x138a = true;
    mPlayerArea = -1;
    scale.setall(daNpcTks_Param_c::m.common.scale);

    dKy_tevstr_init(&mTksTsubo.mTevStr, fopAcM_GetRoomNo(this), 0xFF);
    setAngle(shape_angle.y);
    mTksTsubo.mPos = current.pos;
    mTksTsubo.mAngle = shape_angle;
    speedF = 0.0f;
    speed.setall(0.0f);
    mItemID = fpcM_ERROR_PROCESS_ID_e;
    mExpressionMorfOverride = 0.0f;
    mMotionMorfOverride = 0.0f;

    switch (getType()) {
        case 0:
        case 255:
            if (fopAcM_isSwitch(this, getSwitchNo())) {
                cLib_offBit<u32>(attention_info.flags, fopAc_AttnFlag_CARRY_e);
                field_0x138a = false;
                mTksTsubo.field_0x586 = 0;
                setAction(&daNpcTks_c::waitNude);
            } else {
                mTksTsubo.field_0x586 = 1;
                setAction(&daNpcTks_c::hide);
            }
            break;

        case 1: {
            mTksTsubo.field_0x586 = 0;
            field_0x138a = false;
            setAction(&daNpcTks_c::demo_scannon);
            cXyz i_pos(current.pos);
            i_pos.y += 60.0f;
            parentActorID = fopAcM_createChild(PROC_NPC_TKC, fopAcM_GetID(this), 3, &i_pos, fopAcM_GetRoomNo(this),
                                            &mCurAngle, NULL, -1, NULL);
            break;
        }

        case 2: {
            mTksTsubo.field_0x586 = 0;
            field_0x138a = false;
            setAction(&daNpcTks_c::demo_Lv6Gate);
            cXyz i_pos(current.pos);
            i_pos.y += 60.0f;
            parentActorID = fopAcM_createChild(PROC_NPC_TKC, fopAcM_GetID(this), 3, &i_pos, fopAcM_GetRoomNo(this),
                                            &mCurAngle, NULL, -1, NULL);
            break;
        }

        case 3: {
            mTksTsubo.field_0x586 = 0;
            field_0x138a = false;
            field_0x1370 = 10;
            setAction(&daNpcTks_c::demo_farewell);
            cXyz i_pos(current.pos);
            csXyz i_angle(0, fopAcM_searchPlayerAngleY(this), 0);
            i_pos.y += 60.0f;
            parentActorID = fopAcM_createChild(PROC_NPC_TKC, fopAcM_GetID(this), 4, &i_pos, fopAcM_GetRoomNo(this),
                                            &i_angle, NULL, -1, NULL);
            break;
        }

        case 4:
            mTksTsubo.field_0x586 = 0;
            field_0x138a = false;
            setAction(&daNpcTks_c::demo_warpBack);
            break;

        case 5:
            mTksTsubo.field_0x586 = 0;
            field_0x138a = false;

            if (strcmp(dComIfGp_getStartStageName(), "D_MN11") == 0) {
                field_0x1370 = 5;
            } else if (strcmp(dComIfGp_getStartStageName(), "D_MN07") == 0) {
                if (fopAcM_GetPosition_p(daPy_getPlayerActorClass())->y < 7500.0f) {
                    field_0x1370 = 5;
                } else {
                    field_0x1370 = 5;
                    setAction(&daNpcTks_c::demo_Lv3PickUp);
                    break;
                }
            } else {
                field_0x1370 = 0;
            }

            setAction(&daNpcTks_c::demo_walkBack);
            break;

        case 6: {
            mTksTsubo.field_0x586 = 0;
            field_0x138a = false;
            setAction(&daNpcTks_c::waitLv6);
            cXyz i_pos(current.pos);
            i_pos.y += 60.0f;
            parentActorID = fopAcM_createChild(PROC_NPC_TKC, fopAcM_GetID(this), 4, &i_pos, fopAcM_GetRoomNo(this),
                                            &mCurAngle, NULL, -1, NULL);
            break;
        }

        case 7:
            mTksTsubo.field_0x586 = 0;
            field_0x138a = false;
            gravity = 0.0f;
            setAction(&daNpcTks_c::demo_Lv7Start);
            break;

        case 8:
            mTksTsubo.field_0x586 = 0;
            field_0x138a = false;
            setAction(&daNpcTks_c::demo_Lv3PickUp);
            break;

        case 9:
            mTksTsubo.field_0x586 = 0;
            field_0x138a = false;
            setAction(&daNpcTks_c::demo_Lv6PickUp);
            break;

        default:
            JUT_ASSERT(1298, FALSE);
            break;
    }
}

BOOL daNpcTks_c::setAction(actionFunc action) {
    mMode = 3;

    if (mAction) {
        (this->*mAction)();
    }

    mMode = 0;
    mAction = action;

    if (mAction) {
        (this->*mAction)();
    }

    return TRUE;
}

int daNpcTks_c::getPlayerArea() {
    f32 fVar3, fVar2, fVar1;
    fVar1 = std::pow(daNpcTks_Param_c::m.area_a_dist, 2.0f);
    fVar2 = std::pow(daNpcTks_Param_c::m.area_b_dist, 2.0f);
    fVar3 = fopAcM_searchPlayerDistanceXZ2(this);

    if (fVar3 > 0.0f && fVar3 <= fVar2) {
        return 1;
    }

    if (fVar3 > fVar2 && fVar3 <= fVar1) {
        return 0;
    }

    return -1;
};

void daNpcTks_c::playExpression() {
    daNpcF_anmPlayData dat0 = {ANM_F_TALK_A, daNpcTks_Param_c::m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat0[1] = {&dat0};
    daNpcF_anmPlayData dat1 = {ANM_NONE, daNpcTks_Param_c::m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat1[1] = {&dat1};

    daNpcF_anmPlayData** ppDat[2] = {
        pDat0,
        pDat1
    };

    if (mExpression >= 0 && mExpression < 2) {
        playExpressionAnm(ppDat);
    }
};

void daNpcTks_c::playMotion() {
    daNpcF_anmPlayData dat0 = {ANM_WAIT_A, daNpcTks_Param_c::m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat0[1] = {&dat0};
    daNpcF_anmPlayData dat1 = {ANM_SWIM, daNpcTks_Param_c::m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat1[1] = {&dat1};
    daNpcF_anmPlayData dat2 = {ANM_RUN, daNpcTks_Param_c::m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat2[1] = {&dat2};
    daNpcF_anmPlayData dat3 = {ANM_WALK_A, daNpcTks_Param_c::m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat3[1] = {&dat3};
    daNpcF_anmPlayData dat4a = {ANM_JUMP_S, 4.0f, 1};
    daNpcF_anmPlayData dat4b = {ANM_FLY, 0.0f, 0};
    daNpcF_anmPlayData* pDat4[2] = {&dat4a, &dat4b};
    daNpcF_anmPlayData dat5 = {ANM_JUMP_E, 0.0f, 1};
    daNpcF_anmPlayData* pDat5[1] = {&dat5};
    daNpcF_anmPlayData dat6a = {ANM_LAND, 0.0f, 1};
    daNpcF_anmPlayData dat6b = {ANM_WAIT_A, 4.0f, 0};
    daNpcF_anmPlayData* pDat6[2] = {&dat6a, &dat6b};
    daNpcF_anmPlayData dat7 = {ANM_WALK_A, 4.0f, 1};
    daNpcF_anmPlayData* pDat7[1] = {&dat7};

    daNpcF_anmPlayData** ppDat[8] = {
        pDat0,
        pDat1,
        pDat2,
        pDat3,
        pDat4,
        pDat5,
        pDat6,
        pDat7,
    };

    if (mMotion >= 0 && mMotion < 8) {
        playMotionAnm(ppDat);
    }
}

void daNpcTks_c::lookat() {
    daPy_py_c* player = NULL;
    J3DModel* model = mAnm_p->getModel();
    BOOL i_snap = FALSE;
    f32 body_angleX_min = daNpcTks_Param_c::m.common.body_angleX_min;
    f32 body_angleX_max = daNpcTks_Param_c::m.common.body_angleX_max;
    f32 body_angleY_min = daNpcTks_Param_c::m.common.body_angleY_min;
    f32 body_angleY_max = daNpcTks_Param_c::m.common.body_angleY_max;
    f32 head_angleX_min = field_0x138a ? 0.0f : daNpcTks_Param_c::m.common.head_angleX_min;
    f32 head_angleX_max = field_0x138a ? 0.0f : daNpcTks_Param_c::m.common.head_angleX_max;
    f32 head_angleY_min = daNpcTks_Param_c::m.common.head_angleY_min;
    f32 head_angleY_max = daNpcTks_Param_c::m.common.head_angleY_max;
    s16 angle_delta = mCurAngle.y - mOldAngle.y;
    cXyz lookatPos[3] = {mLookatPos[0], mLookatPos[1], mLookatPos[2]};
    csXyz* lookatAngle[3] = {&mLookatAngle[0], &mLookatAngle[1], &mLookatAngle[2]};
    cXyz spe8;

    switch (mLookMode) {
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
            player = (daPy_py_c*)field_0x1208;
            break;

        case LOOK_ATTN:
            break;
    }

    if (player != NULL) {
        mLookPos = player->attention_info.position;

        if (mLookMode != LOOK_PLAYER && mLookMode != LOOK_PLAYER_TALK) {
            mLookPos.y -= 40.0f;
        }

        mLookat.setAttnPos(&mLookPos);
    } else if (mLookMode == LOOK_ATTN) {
        mLookat.setAttnPos(&mLookPos);
    } else {
        mLookat.setAttnPos(NULL);
    }
    
    mLookat.setParam(body_angleX_min, body_angleX_max, body_angleY_min, body_angleY_max, 0.0f, 0.0f, 0.0f, 0.0f, 
                     head_angleX_min, head_angleX_max, head_angleY_min, head_angleY_max, mCurAngle.y, lookatPos);
    mLookat.calc(this, model->getBaseTRMtx(), lookatAngle, i_snap, angle_delta, FALSE);
}

void daNpcTks_c::playTsuboAnm() {
    if (mTksTsubo.field_0x586 == 0) {
        return;
    }

    if ((mAnmFlags & ANM_PLAY_MORF) != 0) {
        f32 playSpeed = mTksTsubo.mpModelMorf->getPlaySpeed();

        if ((mTksTsubo.mAnmFlags & ANM_PAUSE_MORF) != 0) {
            mTksTsubo.mpModelMorf->setPlaySpeed(0.0f);
            mTksTsubo.mpModelMorf->play(0, 0);
            mTksTsubo.mpModelMorf->setPlaySpeed(playSpeed);
        } else {
            mTksTsubo.mpModelMorf->play(0, 0);
        }
    }

    mTksTsubo.mAnmFlags &= 0xFFFFEF38;
};

BOOL daNpcTks_c::checkFindPlayer() {
    f32 fVar1 = pow(daNpcTks_Param_c::m.area_a_dist, 2.0);
    f32 fVar2 = fopAcM_searchPlayerDistanceXZ2(this);
    mActorMngr[0].remove();

    if (fVar2 <= fVar1) {
        mActorMngr[0].entry(daPy_getPlayerActorClass());
        return TRUE;
    }

    return FALSE;
}

void daNpcTks_c::setLookMode(int i_lookMode, fopAc_ac_c* actor_p) {
    if (i_lookMode >= 0 && i_lookMode < 6 && (i_lookMode != mLookMode || actor_p != field_0x1208)) {
        mLookMode = i_lookMode;
        field_0x1208 = actor_p;
    }
}

void daNpcTks_c::wait() {
    switch (mMode) {
        case 0:
            cLib_offBit<u32>(attention_info.flags, fopAc_AttnFlag_CARRY_e);
            setMotionAnm(ANM_WAIT_A, 0.0f);
            field_0x138a = false;
            speedF = 0.0f;
            mMode = 2;
            break;

        case 2:
            if (mActorMngr[0].getActorP() != NULL) {
                if (!checkFindPlayer()) {
                    mTurnMode = 0;
                }
            } else if (checkFindPlayer()) {
                mTurnMode = 0;
            }

            if (mActorMngr[0].getActorP() != NULL) {
                setLookMode(LOOK_PLAYER, NULL);
            } else {
                setLookMode(LOOK_NONE, NULL);
            }
            break;

        case 3:
            break;

        default:
            JUT_ASSERT(1766, FALSE);
            break;
    }
}

void daNpcTks_c::setTsuboMotionAnm(int i_index, f32 i_morf) {
    J3DAnmTransform* i_anm = getTrnsfrmKeyAnmP(l_arcName, l_bckGetParamList[i_index].fileIdx);
    int i_attr = l_bckGetParamList[i_index].attr;

    mTksTsubo.mAnmFlags &= 0xFFFFFFF6;

    if (i_anm != NULL) {
        mTksTsubo.mpModelMorf->setAnm(i_anm, i_attr, i_morf, 1.0f, 0.0f, -1.0f);
        mTksTsubo.mAnmFlags |= ANM_PLAY_MORF | ANM_PAUSE_MORF;
    }
}

void daNpcTks_c::setMotionAnm(int i_index, f32 i_morf) {
    if (i_index < 2 || i_index >= 0x16) {
        return;
    }

    J3DAnmTransformKey* i_anm = getTrnsfrmKeyAnmP(l_arcName, l_bckGetParamList[i_index].fileIdx);
    int i_attr = l_bckGetParamList[i_index].attr;

    mAnmFlags &= 0xFFFFFFE4;

    if (i_anm != NULL && setMcaMorfAnm(i_anm, 1.0f, i_morf, i_attr, 0, -1)) {
        mAnmFlags |= ANM_PLAY_MORF | ANM_PAUSE_MORF;
        mMotionLoops = 0;

        if (i_index == ANM_RUN) {
            mAnm_p->setPlaySpeed(speedF / daNpcTks_Param_c::m.walk_spd);
        } else {
            mAnm_p->setPlaySpeed(1.0f);
        }

        if (field_0x138a) {
            switch (i_index) {
                case ANM_SHOW_UP:
                    setTsuboMotionAnm(ANM_TUBO_SHOW_UP, i_morf);
                    break;
                
                case ANM_IN:
                    setTsuboMotionAnm(ANM_TUBO_IN, i_morf);
                    break;

                case ANM_SHAKE:
                    setTsuboMotionAnm(ANM_TUBO_SHAKE, i_morf);
                    break;

                default:
                    setTsuboMotionAnm(ANM_TUBO_HIDE, i_morf);
                    break;
            }
        }
    }

    J3DAnmTextureSRTKey* i_btk = getTexSRTKeyAnmP(l_arcName, l_btkGetParamList[0].fileIdx);
    i_attr = l_btkGetParamList[0].attr;

    if (i_btk != NULL) {
        if (setBtkAnm(i_btk, mAnm_p->getModel()->getModelData(), 1.0f, i_attr)) {
            mAnmFlags |= ANM_PLAY_BTK | ANM_PAUSE_BTK;
        }
    }
}

void daNpcTks_c::waitNude() {
    switch (mMode) {
        case 0:
            cLib_offBit<u32>(attention_info.flags, fopAc_AttnFlag_CARRY_e);
            setMotionAnm(ANM_WAIT_A, 0.0f);
            field_0x138a = false;
            speedF = 0.0f;
            mMode = 2;
            break;

        case 2: {
            f32 fVar1 = current.pos.y - fopAcM_GetPosition_p(daPy_getPlayerActorClass())->y;

            if (fVar1 > -200.0f && fVar1 < 150.0f && fopAcM_searchPlayerDistanceXZ(this) <= 300.0f) {
                if (!fopAcM_otherBgCheck(this, daPy_getPlayerActorClass())) {
                    eventInfo.onCondition(dEvtCnd_CANTALK_e);
                    fopAcM_orderSpeakEvent(this, 0, 0);
                }
            }

            if (dComIfGp_event_runCheck() && eventInfo.checkCommandTalk()) {
                dComIfGs_onDungeonItemWarp();

                if (getTBoxNo() != 0xFF) {
                    dComIfGs_onTbox(getTBoxNo());
                }

                Z2GetAudioMgr()->subBgmStart(Z2BGM_OBACHAN);
                setAction(&daNpcTks_c::talk);
            }
            break;
        }

        case 3:
            break;

        default:
            JUT_ASSERT(1805, FALSE);
            break;
    }
}

void daNpcTks_c::waitLv6() {
    switch (mMode) {
        case 0:
            cLib_offBit<u32>(attention_info.flags, fopAc_AttnFlag_CARRY_e);
            setMotionAnm(ANM_WAIT_A, 0.0f);
            field_0x138a = false;
            speedF = 0.0f;
            mMode = 2;
            break;

        case 2:
            if (mActorMngr[0].getActorP() != NULL) {
                if (!checkFindPlayer()) {
                    mTurnMode = 0;
                }
            } else if (checkFindPlayer()) {
                mTurnMode = 0;
            }

            if (mActorMngr[0].getActorP() != NULL) {
                setLookMode(LOOK_PLAYER, NULL);
            } else {
                setLookMode(LOOK_NONE, NULL);
            }

            if (current.pos.y - fopAcM_GetPosition_p(daPy_getPlayerActorClass())->y < 100.0f && fopAcM_searchPlayerDistanceXZ(this) <= 400.0f) {
                attention_info.flags = fopAc_AttnFlag_SPEAK_e | fopAc_AttnFlag_TALK_e;
                eventInfo.onCondition(dEvtCnd_CANTALK_e);
                fopAcM_orderSpeakEvent(this, 0, 0);
            }

            if (dComIfGp_event_runCheck() && eventInfo.checkCommandTalk()) {
                dComIfGs_onDungeonItemWarp();

                if (getTBoxNo() != 0xFF) {
                    dComIfGs_onTbox(getTBoxNo());
                }

                Z2GetAudioMgr()->subBgmStart(Z2BGM_OBACHAN);
                setAction(&daNpcTks_c::talk);
            }
            break;

        case 3:
            break;

        default:
            JUT_ASSERT(1858, FALSE);
            break;
    }
}

void daNpcTks_c::setExpressionTalkAfter() {
    setExpression(EXPR_NONE, -1.0f);
}

BOOL daNpcTks_c::step(s16 i_turnTargetAngle, int param_2) {
    if (mTurnMode == 0) {
        if (param_2 != 0) {
            if ((int)fabsf(cM_sht2d((s16)(i_turnTargetAngle - mCurAngle.y))) > 40) {
                setExpression(EXPR_NONE, -1.0f);
                setMotion(MOT_WALK_A_LOOP, -1.0f, 0);
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
};

void daNpcTks_c::talk() {
    switch (mMode) {
        case 0:
            setMotion(MOT_WAIT_A, -1.0f, 0);
            initTalk(mMessageNo, NULL);
            mMsgTimer = 0;
            mTurnMode = 0;
            setLookMode(LOOK_PLAYER_TALK, NULL);
            mMode = 2;
            break;

        case 2: {
            if (mCurAngle.y == fopAcM_searchPlayerAngleY(this)) {
                if (talkProc(NULL, TRUE, NULL)) {
                    mActorMngr[0].entry(daPy_getPlayerActorClass());
                    int itemNo = 0;

                    OS_REPORT("会話終了時 イベントID=%d アイテムNo=%d\n", mFlow.getEventId(&itemNo), itemNo);
                    
                    if (mFlow.getEventId(&itemNo) == 1) {
                        mItemID = fopAcM_createItemForPresentDemo(&current.pos, itemNo, 0, -1, -1, NULL, NULL);

                        if (mItemID != fpcM_ERROR_PROCESS_ID_e) {
                            const char* eventName = daPy_py_c::checkNowWolf() ? "WOLF_GET_TKS" : "DEFAULT_GETITEM";
                            s16 eventIdx = dComIfGp_getEventManager().getEventIdx(this, eventName, 0xFF);
                            dComIfGp_getEvent()->reset(this);
                            fopAcM_orderChangeEventId(this, eventIdx, 1, 0xFFFF);
                        }
                    }

                    setAction(&daNpcTks_c::wait);
                } else {
                    int msgTimer = mMsgTimer;
                    int i_expression, i_motion;

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
        }

        case 3:
            break;

        default:
            JUT_ASSERT(1920, FALSE);
            break;
    }
}

void daNpcTks_c::setExpression(int i_expression, f32 i_morf) {
    if (i_expression >= 0 && i_expression < 2) {
        mExpression = i_expression;
        mExpressionMorfOverride = i_morf;
        mExpressionPrevPhase = -1;
        mExpressionPhase = 0;
    }
}

void daNpcTks_c::hide() {
    switch (mMode) {
        case 0:
            setMotionAnm(ANM_HIDE, 0.0f);
            field_0x138a = true;
            field_0x1370 = cLib_getRndValue<int>(daNpcTks_Param_c::m.hide_time_1, daNpcTks_Param_c::m.hide_time_2 - daNpcTks_Param_c::m.hide_time_1);
            speedF = 0.0f;
            mMode = 2;
            break;

        case 2: {
            int iVar1 = --field_0x1370;
            if (iVar1 <= 0) {
                if (mPlayerArea == 1) {
                    setAction(&daNpcTks_c::shake);
                } else if (mPlayerArea == 0) {
                    if (cLib_getRndValue(0, 2) == 0) {
                        setAction(&daNpcTks_c::shake);
                    } else {
                        setAction(&daNpcTks_c::showUp);
                    }
                } else {
                    if (cLib_getRndValue(0, 2) == 0) {
                        setAction(&daNpcTks_c::shake);
                    } else {
                        setAction(&daNpcTks_c::showUp);
                    }
                }
            }

            if (mPlayerArea == 0) {
                setAction(&daNpcTks_c::showUp);
            }
            break;
        }

        case 3:
            break;

        default:
            JUT_ASSERT(1960, FALSE);
            break;
    }
}

void daNpcTks_c::shake() {
    switch (mMode) {
        case 0:
            setMotionAnm(ANM_SHAKE, 0.0f);
            field_0x138a = true;
            speedF = 0.0f;
            mMode = 2;
            break;
        
        case 2:
            if (mAnm_p->isStop()) {
                setAction(&daNpcTks_c::hide);
            }

            if (mPlayerArea == 0) {
                setAction(&daNpcTks_c::showUp);
            }
            break;

        case 3:
            break;

        default:
            JUT_ASSERT(1986, FALSE);
            break;
    }
}

void daNpcTks_c::showUp() {
    switch (mMode) {
        case 0:
            setMotionAnm(ANM_SHOW_UP, 0.0f);
            field_0x138a = true;
            speedF = 0.0f;
            mMode = 2;
            break;
        
        case 2:
            if (mAnm_p->isStop()) {
                setAction(&daNpcTks_c::showUpWait);
            }
            break;

        case 3:
            break;

        default:
            JUT_ASSERT(2011, FALSE);
            break;
    }
}

void daNpcTks_c::showUpWait() {
    switch (mMode) {
        case 0:
            setMotionAnm(ANM_SHOW_UP_WAIT, 0.0f);
            field_0x138a = true;
            field_0x1370 = cLib_getRndValue<int>(daNpcTks_Param_c::m.show_up_wait_time_1, daNpcTks_Param_c::m.show_up_wait_time_2 - daNpcTks_Param_c::m.show_up_wait_time_1);
            speedF = 0.0f;
            mMode = 2;
            break;

        case 2: {
            int iVar1 = --field_0x1370;

            if (iVar1 <= 0) {
                if (mPlayerArea == 0) {
                    switch (cLib_getRndValue(0, 2)) {
                        case 0:
                            setAction(&daNpcTks_c::lookAroundA);
                            break;
                        
                        case 1:
                            setAction(&daNpcTks_c::lookAroundB);
                            break;
                    }
                } else {
                    switch (cLib_getRndValue(0, 3)) {
                        case 0:
                            setAction(&daNpcTks_c::lookAroundA);
                            break;

                        case 1:
                            setAction(&daNpcTks_c::lookAroundB);
                            break;

                        case 2:
                            setAction(&daNpcTks_c::in);
                            break;
                    }
                }
            }

            if (mPlayerArea == 1) {
                setAction(&daNpcTks_c::in);
            }
            break;
        }

        case 3:
            break;

        default:
            JUT_ASSERT(2056, FALSE);
            break;
    }
}

void daNpcTks_c::lookAroundA() {
    switch (mMode) {
        case 0:
            setMotionAnm(ANM_LOOKAROUND_A, 0.0f);
            field_0x138a = true;
            speedF = 0.0f;
            mMode = 2;
            break;
        
        case 2:
            if (mAnm_p->isStop()) {
                setAction(&daNpcTks_c::showUpWait);
            }

            if (mPlayerArea == 1) {
                setAction(&daNpcTks_c::in);
            }
            break;

        case 3:
            break;

        default:
            JUT_ASSERT(2082, FALSE);
            break;
    }
}

void daNpcTks_c::lookAroundB() {
    switch (mMode) {
        case 0:
            setMotionAnm(ANM_LOOKAROUND_B, 0.0f);
            field_0x138a = true;
            speedF = 0.0f;
            mMode = 2;
            break;
        
        case 2:
            if (mAnm_p->isStop()) {
                setAction(&daNpcTks_c::showUpWait);
            }

            if (mPlayerArea == 1) {
                setAction(&daNpcTks_c::in);
            }
            break;

        case 3:
            break;

        default:
            JUT_ASSERT(2082, FALSE);
            break;
    }
}

void daNpcTks_c::in() {
    switch (mMode) {
        case 0:
            setMotionAnm(ANM_IN, 0.0f);
            setLookMode(LOOK_NONE, NULL);
            field_0x138a = true;
            mMode = 2;
            break;
        
        case 2:
            if (mAnm_p->isStop()) {
                setAction(&daNpcTks_c::hide);
            }
            break;

        case 3:
            break;

        default:
            JUT_ASSERT(2133, FALSE);
            break;
    }
}

void daNpcTks_c::damage() {
    switch (mMode) {
        case 0:
            setMotionAnm(ANM_SHAKE, 0.0f);
            cLib_offBit<u32>(attention_info.flags, fopAc_AttnFlag_CARRY_e);
            field_0x138a = true;
            speedF = 0.0f;
            mMode = 2;
            break;
        
        case 2:
            if (mAnm_p->isStop()) {
                setAction(&daNpcTks_c::hide);
            }

            if (mTksTsubo.mCyl.ChkTgHit()) {
                mMode = 0;
            }
            break;

        case 3:
            cLib_onBit<u32>(attention_info.flags, fopAc_AttnFlag_CARRY_e);
            break;

        default:
            JUT_ASSERT(2162, FALSE);
            break;
    }
}

void daNpcTks_c::broken() {
    switch (mMode) {
        case 0:
            setMotionAnm(ANM_FALL, 0);

            if (mTksTsubo.mTsuboType != 0) {
                daObj::make_eff_break_kotubo2(this);
            } else {
                daObj::make_eff_break_kotubo(this);
            }

            mTksTsubo.mSound.startCreatureSound(Z2SE_TKS_V_OUT_OF_POT, 0, -1);
            cLib_offBit<u32>(attention_info.flags, fopAc_AttnFlag_CARRY_e);
            field_0x138a = false;
            mTksTsubo.field_0x586 = 0;
            fopAcM_onSwitch(this, getSwitchNo());
            speedF = 0.0f;
            speed.y = daNpcTks_Param_c::m.launch_spd;
            mMode = 2;
            break;
        
        case 2:
            if (mAcch.ChkGroundLanding()) {
                setMotion(MOT_LAND, -1.0f, 0);
            } else if (mAcch.ChkGroundHit() && mAnm_p->isStop()) {
                setAction(&daNpcTks_c::waitNude);
            }
            break;

        case 3:
            break;

        default:
            JUT_ASSERT(2199, FALSE);
    }
}

void daNpcTks_c::demo_appear() {
    switch (mMode) {
        case 0:
            eventInfo.setArchiveName(l_arcName);
            mEventIdx = dComIfGp_getEventManager().getEventIdx(this, "TKS_ENCOUNTER", 0xFF);
            fopAcM_orderOtherEventId(this, mEventIdx, 0xFF, 0xFFFF, 4, 1);
            cLib_offBit<u32>(attention_info.flags, fopAc_AttnFlag_CARRY_e);
            setExpression(EXPR_NONE, -1.0f);
            setMotionAnm(ANM_HIDE, 0.0f);
            mMode = 2;
            // fallthrough
        case 2:
            if (dComIfGp_event_runCheck() && !eventInfo.checkCommandTalk()) {
                dEvent_manager_c& eventManager = dComIfGp_getEventManager();
                s32 staffId = eventManager.getMyStaffId(l_myName, NULL, 0);
                if (staffId != -1) {
                    if (!dComIfGs_isDungeonItemWarp()) {
                        dComIfGs_onDungeonItemWarp();
                        
                        if (getTBoxNo() != 0xFF) {
                            dComIfGs_onTbox(getTBoxNo());
                        }

                        Z2GetAudioMgr()->bgmAllMute(60, 0.0f);
                    }

                    int* cutName = (int*)eventManager.getMyNowCutName(staffId);

                    if (eventManager.getIsAddvance(staffId)) {
                        switch (*cutName) {
                            case '0001':
                                break;

                            case '0002':
                                setMotionAnm(ANM_SHAKE, 0.0f);
                                break;

                            case '0003':
                                setMotionAnm(ANM_FALL, 0.0f);
                                setLookMode(LOOK_NONE, NULL);

                                if (mTksTsubo.mTsuboType != 0) {
                                    daObj::make_eff_break_kotubo2(this);
                                } else {
                                    daObj::make_eff_break_kotubo(this);
                                }

                                dComIfGp_getVibration().StartShock(2, 15, cXyz(0.0f, 1.0f, 0.0f));
                                mTksTsubo.mSound.startCreatureSound(Z2SE_TKS_V_OUT_OF_POT, 0, -1);
                                fopAcM_cancelCarryNow(this);
                                cLib_offBit<u32>(attention_info.flags, fopAc_AttnFlag_CARRY_e);
                                field_0x138a = false;
                                mTksTsubo.field_0x586 = 0;
                                speedF = daNpcTks_Param_c::m.movement_spd;
                                speed.y = daNpcTks_Param_c::m.launch_spd;
                                break;

                            case '0004':
                                setLookMode(LOOK_PLAYER, NULL);
                                mTurnMode = 0;
                                Z2GetAudioMgr()->subBgmStart(Z2BGM_OBACHAN);
                                Z2GetAudioMgr()->bgmAllUnMute(0);
                                break;

                            case '0005':
                                initTalk(mMessageNo, NULL);
                                mMsgTimer = 0;
                                setLookMode(LOOK_PLAYER_TALK, NULL);
                                break;

                            default:
                                JUT_ASSERT(2277, FALSE);
                                break;
                        }
                    }

                    int msgTimer = mMsgTimer;
                    int i_expression, i_motion;

                    if (ctrlMsgAnm(i_expression, i_motion, this, TRUE)) {
                        setExpression(i_expression, -1.0f);
                        setMotion(i_motion, -1.0f, 0);
                    } else if (msgTimer != 0 && mMsgTimer == 0) {
                        setExpressionTalkAfter();
                    }

                    switch (*cutName) {
                        case '0001':
                            eventManager.cutEnd(staffId);
                            break;

                        case '0002':
                            if (mAnm_p->isStop()) {
                                eventManager.cutEnd(staffId);
                            }
                            break;

                        case '0003':
                            setAngle(shape_angle.y);
                            mTksTsubo.mAngle = shape_angle;

                            if (mAcch.ChkGroundLanding()) {
                                speedF = 0.0f;
                                setMotion(MOT_LAND, -1.0f, 0);
                            } else if (mAcch.ChkGroundHit() && mMotionPhase > 0) {
                                setMotionAnm(ANM_WAIT_A, 0.0f);
                                eventManager.cutEnd(staffId);
                            }
                            break;

                        case '0004':
                            if (step(fopAcM_searchPlayerAngleY(this), 1)) {
                                setMotionAnm(ANM_WAIT_A, 0.0f);
                                eventManager.cutEnd(staffId);
                            }
                            break;

                        case '0005':
                            if (talkProc(NULL, TRUE, NULL)) {
                                mActorMngr[0].entry(daPy_getPlayerActorClass());
                                int itemNo = 0;

                                OS_REPORT("会話終了時 イベントID=%d アイテムNo=%d\n", mFlow.getEventId(&itemNo), itemNo);

                                if (mFlow.getEventId(&itemNo) == 1) {
                                    mItemID = fopAcM_createItemForPresentDemo(&current.pos, itemNo, 0, -1, -1, NULL, NULL);

                                    if (mItemID != fpcM_ERROR_PROCESS_ID_e) {
                                        s16 eventIdx = dComIfGp_getEventManager().getEventIdx(this, "DEFAULT_GETITEM", 0xFF);
                                        dComIfGp_getEvent()->reset(this);
                                        fopAcM_orderChangeEventId(this, eventIdx, 1, 0xFFFF);
                                        field_0x9ec = true;
                                    }
                                }

                                setAction(&daNpcTks_c::wait);
                                eventManager.cutEnd(staffId);
                            }
                            break;

                        default:
                            JUT_ASSERT(2339, FALSE);
                            break;
                    }

                    if (eventInfo.checkCommandDemoAccrpt() && mEventIdx != -1 && eventManager.endCheck(mEventIdx) != 0) {
                        if (!field_0x9ec) {
                            dComIfGp_event_reset();
                        }

                        mEventIdx = -1;
                        setAction(&daNpcTks_c::wait);
                    }
                }
            }
            break;
        
        case 3:
            break;

        default:
            JUT_ASSERT(2356, FALSE);
    }
}

void daNpcTks_c::demo_scannon() {
    switch (mMode) {
        case 0:
            cLib_offBit<u32>(attention_info.flags, fopAc_AttnFlag_CARRY_e);
            setExpression(EXPR_NONE, -1.0f);
            setMotion(MOT_WAIT_A, -1.0f, 0);
            field_0x138a = false;
            mMode = 2;
            // fallthrough
        case 2:
            if (dComIfGp_event_runCheck() && !eventInfo.checkCommandTalk()) {
                dEvent_manager_c& eventManager = dComIfGp_getEventManager();
                s32 staffId = eventManager.getMyStaffId(l_myName, NULL, 0);

                if (staffId != -1) {
                    int* cutName = (int*)eventManager.getMyNowCutName(staffId);

                    if (eventManager.getIsAddvance(staffId)) {
                        switch (*cutName) {
                            case '0001':
                                break;
                            
                            case '0002':
                                old.pos.set(-98491.23f, -18346.793f, 52057.156f);
                                current.pos = old.pos;
                                setAngle(-0x2CA2);
                                speedF = 25.0f;
                                setMotion(MOT_RUN, -1.0f, 0);
                                break;

                            case '0003':
                                setMotion(MOT_JUMP_S, 0.0f, 0);
                                mAnm_p->setPlaySpeed(1.0f);
                                setAngle(0);
                                speedF = 0.0f;
                                speed.y = 0.0f;
                                break;

                            default:
                                JUT_ASSERT(2405, FALSE);
                                break;
                        }
                    }

                    switch (*cutName) {
                        case '0001':
                            eventManager.cutEnd(staffId);
                            break;

                        case '0002': {
                            cXyz sp1c(-102070.93f, -18470.0f, 53544.613f);
                            setAngle(cLib_targetAngleY(&current.pos, &sp1c));
                            fopAcM_effSmokeSet2(&field_0x137c[0], &field_0x137c[1], &current.pos, &mCurAngle, 1.0f, &tevStr);

                            for (int i = 0; i < 2; i++) {
                                JPABaseEmitter* emitter = dComIfGp_particle_getEmitter(field_0x137c[i]);
                                if (emitter != NULL) {
                                    emitter->setRate(1.0f);
                                    emitter->setLifeTime(30);
                                    emitter->setGlobalAlpha(0x80);
                                    emitter->setGlobalParticleScale(2.5f, 2.5f);
                                }
                            }

                            mSound.startCreatureVoiceLevel(Z2SE_TKS_V_RUNNING_LOOP, -1);

                            if (current.pos.absXZ(sp1c) < speedF) {
                                current.pos.set(sp1c);
                                speedF = 0.0f;
                                eventManager.cutEnd(staffId);
                            }
                            break;
                        }

                        case '0003':
                            if (mMotionPhase == 0 && mAnm_p->checkFrame(9.0f)) {
                                speedF = 25.0f;
                                speed.y = 60.0f;
                                mSound.startCreatureSound(Z2SE_TKS_V_JUMP_UP, 0, -1);
                            }

                            if (speedF > 0.0f && mAcch.ChkGroundLanding()) {
                                speedF = 0.0f;
                                speed.y = 0.0f;
                                eventManager.cutEnd(staffId);
                                fopAcM_delete(this);
                            }
                            break;

                        default:
                            JUT_ASSERT(2473, FALSE);
                            break;
                    }
                }
            }
            break;
        
        case 3:
            break;

        default:
            JUT_ASSERT(2479, FALSE);
            break;
    }
}

void daNpcTks_c::demo_Lv6Gate() {
    switch (mMode) {
        case 0:
            cLib_offBit<u32>(attention_info.flags, fopAc_AttnFlag_CARRY_e);
            setExpression(EXPR_NONE, -1.0f);
            setMotion(MOT_WAIT_A, -1.0f, 0);
            field_0x138a = false;
            mMode = 2;
            // fallthrough
        case 2:
            if (dComIfGp_event_runCheck() && !eventInfo.checkCommandTalk()) {
                dEvent_manager_c& eventManager = dComIfGp_getEventManager();
                s32 staffId = eventManager.getMyStaffId(l_myName, NULL, 0);

                if (staffId != -1) {
                    int* cutName = (int*)eventManager.getMyNowCutName(staffId);

                    if (eventManager.getIsAddvance(staffId)) {
                        switch (*cutName) {
                            case '0001':
                                break;

                            case '0002':
                                field_0x1370 = 320;
                                speedF = 13.0f;
                                setMotion(MOT_RUN, -1.0f, 0);
                                dComIfGp_event_setTalkPartner(this);
                                break;

                            default:
                                JUT_ASSERT(2520, FALSE);
                                break;
                        }
                    }

                    switch (*cutName) {
                        case '0001':
                            eventManager.cutEnd(staffId);
                            break;
                        
                        case '0002':
                            mSound.startCreatureVoiceLevel(Z2SE_TKS_V_RUNNING_LOOP, -1);

                            if (cLib_calcTimer<int>(&field_0x1370) == 0) {
                                eventManager.cutEnd(staffId);
                                fopAcM_delete(this);
                            }
                            break;

                        default:
                            JUT_ASSERT(2535, FALSE);
                    }
                }
            }
            break;

        case 3:
            break;

        default:
            JUT_ASSERT(2541, FALSE);
            break;
    }
}

void daNpcTks_c::demo_farewell() {
    switch (mMode) {
        case 0:
            if (cLib_calcTimer<int>(&field_0x1370) > 0 || dComIfGp_event_runCheck()) {
                return;
            }

            eventInfo.setArchiveName(l_arcName);
            mEventIdx = dComIfGp_getEventManager().getEventIdx(this, "TKS_FAREWELL", 0xFF);
            fopAcM_orderOtherEventId(this, mEventIdx, 0xFF, 0xFFFF, 4, 1);
            cLib_offBit<u32>(attention_info.flags, fopAc_AttnFlag_CARRY_e);
            setExpression(EXPR_NONE, -1.0f);
            setMotionAnm(ANM_WAIT_A, 0.0f);

            /* dSv_event_flag_c::F_0545 - Sacred Grove - Watched cutscene of Ooccoo Sr. parting (after Temple of Time) */
            daNpcF_onEvtBit(545);
            Z2GetAudioMgr()->subBgmStart(Z2BGM_OBACHAN);
            mMode = 2;
            // fallthrough
        case 2:
            if (dComIfGp_event_runCheck() && !eventInfo.checkCommandTalk()) {
                dEvent_manager_c& eventManager = dComIfGp_getEventManager();
                s32 staffId = eventManager.getMyStaffId(l_myName, NULL, 0);

                if (staffId != -1) {
                    int* cutName = (int*)eventManager.getMyNowCutName(staffId);

                    if (eventManager.getIsAddvance(staffId)) {
                        switch (*cutName) {
                            case '0001':
                                dComIfGp_getEvent()->setSkipZev(this, "TKS_FAREWELL_SKIP");
                                // fallthrough
                            case '0006':
                            case '0007':
                            case '0008':
                            case '0009':
                                initTalk(mMessageNo, NULL);
                                mMsgTimer = 0;
                                setLookMode(LOOK_PLAYER_TALK, NULL);
                                setAngle(fopAcM_searchPlayerAngleY(this));
                                break;
                            
                            case '0002':
                                setLookMode(LOOK_NONE, NULL);
                                break;

                            case '0003': {
                                daNpcTkc_c* actor_p = NULL;
                                fopAcM_SearchByID(parentActorID, (fopAc_ac_c**)&actor_p);
                                if (actor_p != NULL) {
                                    actor_p->setActionFollow();
                                }

                                setMotion(MOT_RUN, -1.0f, 0);
                                speedF = 10.0f;
                                break;
                            }

                            case '0004':
                                setMotion(MOT_JUMP_S, -1.0f, 0);
                                setAngle(0x16EA);
                                break;
                            
                            case '0005':
                                break;

                            case '0010':
                                break;

                            case '0011':
                                Z2GetAudioMgr()->subBgmStop();
                                fopAcM_delete(this);
                                break;

                            default:
                                JUT_ASSERT(2627, FALSE);
                                break;
                        }
                    }

                    int msgTimer = mMsgTimer;
                    int i_expression, i_motion;

                    if (ctrlMsgAnm(i_expression, i_motion, this, TRUE)) {
                        setExpression(i_expression, -1.0f);
                        setMotion(i_motion, -1.0f, 0);
                    } else if (msgTimer != 0 && mMsgTimer == 0) {
                        setExpressionTalkAfter();
                    }

                    switch (*cutName) {
                        case '0001':
                        case '0006':
                        case '0007':
                        case '0008':
                        case '0009':
                            if (talkProc(NULL, TRUE, NULL)) {
                                eventManager.cutEnd(staffId);
                            }
                            break;
                            
                        case '0002': {
                            cXyz* pos = dComIfGp_evmng_getMyXyzP(staffId, "pos");
                            if (pos != NULL) {
                                if (step(cLib_targetAngleY(&current.pos, pos), 1)) {
                                    mTurnMode = 0;
                                    eventManager.cutEnd(staffId);
                                }
                            }
                            break;
                        }

                        case '0003': {
                            cXyz* pos = dComIfGp_evmng_getMyXyzP(staffId, "pos");
                            if (pos != NULL && current.pos.abs(*pos) <= speedF) {
                                setMotion(MOT_WAIT_A, -1.0f, 0);
                                current.pos = *pos;
                                speedF = 0.0f;
                                eventManager.cutEnd(staffId);
                            }

                            mSound.startCreatureVoiceLevel(Z2SE_TKS_V_RUNNING_LOOP, -1);
                            break;
                        }

                        case '0004': {
                            cXyz* pos = dComIfGp_evmng_getMyXyzP(staffId, "pos");

                            if (mMotion == 4) {
                                if (mMotionPhase == 0 && mAnm_p->checkFrame(9.0f)) {
                                    speedF = 15.0f;
                                    speed.y = 60.0f;
                                    mSound.startCreatureSound(Z2SE_TKS_V_JUMP_UP, 0, -1);
                                } else if (speedF > 0.0f && mAcch.ChkGroundLanding()) {
                                    setMotion(MOT_JUMP_E, -1.0f, 0);
                                    speedF = 0.0f;
                                    speed.y = 0.0f;
                                }
                            } else if (mMotion == 5 && mAnm_p->isStop()) {
                                Z2GetAudioMgr()->subBgmStop();
                                eventManager.cutEnd(staffId);
                            }
                            break;
                        }

                        case '0005':
                            break;

                        case '0010':
                        case '0011':
                            eventManager.cutEnd(staffId);
                            break;

                        default:
                            JUT_ASSERT(2711, FALSE);
                            break;
                    }

                    if (eventInfo.checkCommandDemoAccrpt() && mEventIdx != -1 && eventManager.endCheck(mEventIdx)) {
                        dComIfGp_event_reset();
                        fopAcM_delete(this);
                        mEventIdx = -1;
                    }
                }
            }
            break;
        
        case 3:
            break;

        default:
            JUT_ASSERT(2737, FALSE);
    }
}

void daNpcTks_c::demo_warpBack() {
    switch (mMode){
        case 0:
            eventInfo.setArchiveName(l_arcName);
            mEventIdx = dComIfGp_getEventManager().getEventIdx(this, "TKS_WARPBACK", 0xFF);
            fopAcM_orderOtherEventId(this, mEventIdx, 0xFF, 0xFFFF, 4, 1);
            cLib_offBit<u32>(attention_info.flags, fopAc_AttnFlag_CARRY_e);
            setExpression(EXPR_NONE, -1.0f);
            setMotionAnm(ANM_WAIT_A, 0.0f);
            setLookMode(LOOK_PLAYER_TALK, NULL);

            if (strcmp(dComIfGp_getStartStageName(), "D_MN11") == 0 && dComIfGp_getStartStageRoomNo() == 3) {
                cXyz* pcVar1 = fopAcM_GetPosition_p(daPy_getPlayerActorClass());
                if (pcVar1->y >= 950.0f) {
                    if (pcVar1->x == 3700.0f && pcVar1->z == -4220.0f) {
                        current.pos.set(3500.0f, 950.0f, -4220.0f);
                        setAngle(0x4000);
                    } else if (pcVar1->x == 3360.0f && pcVar1->z == -2010.0f) {
                        current.pos.set(3360.0f, 950.0f, -1810.0f);
                        setAngle(0x7FFF);
                    }

                    old.pos = current.pos;
                }
            }

            mMode = 2;
            // fallthrough
        case 2:
            if (dComIfGp_event_runCheck() && !eventInfo.checkCommandTalk()) {
                dEvent_manager_c& eventManager = dComIfGp_getEventManager();
                s32 staffId = eventManager.getMyStaffId(l_myName, NULL, 0);

                if (staffId != -1) {
                    int* cutName = (int*)eventManager.getMyNowCutName(staffId);

                    if (eventManager.getIsAddvance(staffId)) {
                        switch (*cutName) {
                            case '0001':
                                dComIfGs_resetLastWarpAcceptStage();
                                break;

                            case '0002':
                                initTalk(3, NULL);
                                mMsgTimer = 0;
                                setAngle(fopAcM_searchPlayerAngleY(this));
                                break;

                            case '0003':
                                mCyl.OffCoSetBit();
                                setMotion(MOT_JUMP_S, -1.0f, 0);
                                speedF = 0.0f;
                                speed.y = 0.0f;
                                scale.setall(1.0f);
                                break;

                            default:
                                JUT_ASSERT(2809, FALSE);
                        }
                    }

                    int msgTimer = mMsgTimer;
                    int i_expression, i_motion;

                    if (ctrlMsgAnm(i_expression, i_motion, this, TRUE)) {
                        setExpression(i_expression, -1.0f);
                        setMotion(i_motion, -1.0f, 0);
                    } else if (msgTimer != 0 && mMsgTimer == 0) {
                        setExpressionTalkAfter();
                    }

                    switch (*cutName) {
                        case '0001':
                            eventManager.cutEnd(staffId);
                            break;
                        
                        case '0002':
                            if (talkProc(NULL, TRUE, NULL)) {
                                eventManager.cutEnd(staffId);
                            }
                            break;

                        case '0003':
                            if (mMotionPhase == 0 && mAnm_p->checkFrame(9.0f)) {
                                speedF = 12.0f;
                                speed.y = 33.0f;
                                field_0x1370 = 200.0f / speedF;
                                mSound.startCreatureSound(Z2SE_TKS_V_JUMP_UP, 0, -1);
                            }

                            if (speedF > 0.0f) {
                                cLib_chaseF(&scale.x, 0.15f, 0.85f / (200.0f / speedF));
                                scale.setall(scale.x);

                                if (cLib_calcTimer<int>(&field_0x1370) == 0) {
                                    speedF = 0.0f;
                                    speed.y = 0.0f;
                                    eventManager.cutEnd(staffId);
                                }
                            }
                            break;

                        default:
                            JUT_ASSERT(2853, FALSE);
                            break;
                    }

                    if (eventInfo.checkCommandDemoAccrpt() && mEventIdx != -1 && eventManager.endCheck(mEventIdx)) {
                        mEventIdx = -1;
                        dComIfGp_event_reset();
                        fopAcM_delete(this);
                    }
                }
            }
            break;
        
        case 3:
            break;

        default:
            JUT_ASSERT(2870, FALSE);
    }
}

void daNpcTks_c::demo_walkBack() {
    switch (mMode){
        case 0:
            cLib_offBit<u32>(attention_info.flags, fopAc_AttnFlag_CARRY_e);
            setExpression(EXPR_NONE, -1.0f);
            setMotionAnm(ANM_WAIT_A, 0.0f);
            setLookMode(LOOK_PLAYER_TALK, NULL);

            if (strcmp(dComIfGp_getStartStageName(), "D_MN11") == 0) {
                current.pos.set(0.0f, -150.0f, 6600.0f);
                old.pos = current.pos;
            } else if (strcmp(dComIfGp_getStartStageName(), "D_MN07") == 0) {
                current.pos.set(4955.0f, -200.0f, 5480.0f);
                old.pos = current.pos;
            }

            if (cLib_calcTimer<int>(&field_0x1370) > 0) {
                return;
            }

            if (dComIfGp_event_runCheck() && !eventInfo.checkCommandTalk()) {
                if (dComIfGp_getEventManager().getMyStaffId(l_myName, NULL, 0) != -1) {
                    mMode = 2;
                } else {
                    break;
                }
            } else {
                eventInfo.setArchiveName(l_arcName);
                mEventIdx = dComIfGp_getEventManager().getEventIdx(this, "TKS_WARPBACK", 0xFF);
                fopAcM_orderOtherEventId(this, mEventIdx, 0xFF, 0xFFFF, 4, 1);
            }
            // fallthrough
        case 2:
            if (dComIfGp_event_runCheck() && !eventInfo.checkCommandTalk()) {
                dEvent_manager_c& eventManager = dComIfGp_getEventManager();
                s32 staffId = eventManager.getMyStaffId(l_myName, NULL, 0);

                if (staffId != -1) {
                    int* cutName = (int*)eventManager.getMyNowCutName(staffId);

                    if (eventManager.getIsAddvance(staffId)) {
                        switch (*cutName) {
                            case '0001':
                                dComIfGs_resetLastWarpAcceptStage();
                                break;

                            case '0002':
                                initTalk(3, NULL);
                                mMsgTimer = 0;
                                setAngle(fopAcM_searchPlayerAngleY(this));
                                break;

                            case '0003':
                                mCyl.OffCoSetBit();
                                setMotion(MOT_JUMP_S, -1.0f, 0);
                                speedF = 0.0f;
                                speed.y = 0.0f;
                                scale.setall(1.0f);
                                break;

                            default:
                                JUT_ASSERT(2809, FALSE);
                        }
                    }

                    int msgTimer = mMsgTimer;
                    int i_expression, i_motion;

                    if (ctrlMsgAnm(i_expression, i_motion, this, TRUE)) {
                        setExpression(i_expression, -1.0f);
                        setMotion(i_motion, -1.0f, 0);
                    } else if (msgTimer != 0 && mMsgTimer == 0) {
                        setExpressionTalkAfter();
                    }

                    switch (*cutName) {
                        case '0001':
                            eventManager.cutEnd(staffId);
                            break;
                        
                        case '0002':
                            if (talkProc(NULL, TRUE, NULL)) {
                                eventManager.cutEnd(staffId);
                            }
                            break;

                        case '0003':
                            if (mMotionPhase == 0 && mAnm_p->checkFrame(9.0f)) {
                                speedF = 12.0f;
                                speed.y = 31.0f;
                                field_0x1370 = 200.0f / speedF;
                                mSound.startCreatureSound(Z2SE_TKS_V_JUMP_UP, 0, -1);
                            }

                            if (speedF > 0.0f) {
                                cLib_chaseF(&scale.x, 0.15f, 0.85f / (200.0f / speedF));
                                scale.setall(scale.x);

                                if (cLib_calcTimer<int>(&field_0x1370) == 0) {
                                    speedF = 0.0f;
                                    speed.y = 0.0f;
                                    eventManager.cutEnd(staffId);
                                }
                            }
                            break;

                        default:
                            JUT_ASSERT(2986, FALSE);
                            break;
                    }

                    if (eventInfo.checkCommandDemoAccrpt() && mEventIdx != -1 && eventManager.endCheck(mEventIdx)) {
                        dComIfGp_event_reset();
                        mEventIdx = -1;
                        dMeter2Info_warpInProc();
                        fopAcM_delete(this);
                    }
                }
            }
            break;
        
        case 3:
            break;

        default:
            JUT_ASSERT(3004, FALSE);
    }
}

void daNpcTks_c::demo_Lv7Start() {
    switch (mMode) {
        case 0:
            cLib_offBit<u32>(attention_info.flags, fopAc_AttnFlag_CARRY_e);
            setExpression(EXPR_NONE, -1.0f);
            setMotion(MOT_JUMP_S, -1.0f, 0);
            mMode = 2;
            // fallthrough
        case 2:
            if (dComIfGp_event_runCheck() && !eventInfo.checkCommandTalk()) {
                dEvent_manager_c& eventManager = dComIfGp_getEventManager();
                s32 staffId = eventManager.getMyStaffId(l_myName, NULL, 0);

                if (staffId != -1) {
                    int* cutName = (int*)eventManager.getMyNowCutName(staffId);

                    if (eventManager.getIsAddvance(staffId)) {
                        switch (*cutName) {
                            case '0000':
                                break;
                                
                            case '0002':
                                current.pos.set(120.0f, 3000.0f, 5500.0f);
                                old.pos = current.pos;
                                setAngle(fopAcM_searchPlayerAngleY(this));
                                parentActorID = fopAcM_createChild(PROC_NPC_TKC, fopAcM_GetID(this), 3, &current.pos, fopAcM_GetRoomNo(this),
                                                                   &mCurAngle, NULL, -1, NULL);
                                dComIfGp_event_setTalkPartner(this);
                                field_0x138b = false;
                                field_0x1370 = 143;
                                break;

                            case '0003': {
                                daNpcTkc_c* actor_p = NULL;
                                fopAcM_SearchByID(parentActorID, (fopAc_ac_c**)&actor_p);

                                if (actor_p != NULL) {
                                    actor_p->setActionWait();
                                }

                                setMotion(MOT_SWIM, -1.0f, 0);
                                field_0x1370 = 60;
                                break;
                            }

                            case '0004':
                            case '0008':
                                initTalk(mMessageNo, NULL);
                                mMsgTimer = 0;
                                setLookMode(LOOK_PLAYER_TALK, NULL);
                                break;

                            case '0005':
                                break;

                            case '0007': {
                                daNpcTkc_c* actor_p = NULL;
                                fopAcM_SearchByID(parentActorID, (fopAc_ac_c**)&actor_p);

                                if (actor_p != NULL) {
                                    actor_p->setActionWait();
                                }
                                break;
                            }

                            case '0009':
                                setLookMode(LOOK_NONE, NULL);
                                break;

                            case '0010': {                                
                                daNpcTkc_c* actor_p = NULL;
                                fopAcM_SearchByID(parentActorID, (fopAc_ac_c**)&actor_p);

                                if (actor_p != NULL) {
                                    actor_p->setActionFollow();
                                }

                                setMotion(MOT_JUMP_S, -1.0f, 0);
                                break;
                            }

                            case '0011':
                                setMotion(MOT_RUN, -1.0f, 0);
                                speedF = 10.0f;
                                break;

                            case '0012':
                                fopAcM_delete(this);
                                break;

                            default:
                                JUT_ASSERT(3091, FALSE);
                                break;
                        }
                    }

                    int msgTimer = mMsgTimer;
                    int i_expression, i_motion;

                    if (ctrlMsgAnm(i_expression, i_motion, this, TRUE)) {
                        setExpression(i_expression, -1.0f);
                    } else if (msgTimer != 0 && mMsgTimer == 0) {
                        setExpressionTalkAfter();
                    }

                    switch (*cutName) {
                        case '0001':
                            gravity = 0.0f;
                            if (!daPy_getPlayerActorClass()->checkSwimUp()) {
                                eventManager.cutEnd(staffId);
                            }
                            break;

                        case '0002':
                            if (cLib_calcTimer<int>(&field_0x1370) == 0) {
                                eventManager.cutEnd(staffId);
                            }

                            if (mAcch.ChkWaterIn()) {
                                f32 fVar3 = mAcch.m_wtr.GetHeight();
                                if (current.pos.y < fVar3) {
                                    cXyz i_pos(current.pos.x, fVar3, current.pos.z);

                                    if (!field_0x138b) {
                                        fopKyM_createWpillar(&i_pos, 1.0f, 0);
                                        Z2GetAudioMgr()->seStart(Z2SE_CM_BODYFALL_WATER_S, &i_pos, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
                                    }

                                    if (speed.y < 0.0f) {
                                        speed.y *= 0.7;
                                    }

                                    speed.y -= gravity - 1.0f;
                                    setMotion(MOT_SWIM, -1.0f, 0);
                                    fopAcM_effHamonSet(&field_0x137c[0], &i_pos, 0.7f, 0.08f);
                                    field_0x138b = true;
                                } else {
                                    field_0x138b = false;
                                }
                            }

                            if (!field_0x138b) {
                                mSound.startCreatureVoiceLevel(Z2SE_TKS_V_FALLING_LOOP, -1);
                            }
                            break;
                            
                        case '0003':
                            if (cLib_calcTimer<int>(&field_0x1370) == 0) {
                                eventManager.cutEnd(staffId);
                            }

                            if (mAcch.ChkWaterIn()) {
                                f32 fVar2 = mAcch.m_wtr.GetHeight();
                                if (current.pos.y < fVar2) {
                                    current.pos.y = fVar2;
                                    speed.y = 0.0f;
                                    fopAcM_effHamonSet(&field_0x137c[0], &current.pos, 0.7f, 0.08f);
                                    field_0x138b = true;
                                } else {
                                    field_0x138b = false;
                                }
                            }
                            break;

                        case '0008':
                            if (mCurAngle.y == fopAcM_searchPlayerAngleY(this)) {
                                if (talkProc(NULL, TRUE, NULL)) {
                                    eventManager.cutEnd(staffId);
                                }
                            } else {
                                if (step(fopAcM_searchPlayerAngleY(this), 0)) {
                                    mTurnMode = 0;
                                }
                            }

                            if (mAcch.ChkWaterIn()) {
                                f32 fVar1 = mAcch.m_wtr.GetHeight();
                                if (current.pos.y < fVar1) {
                                    current.pos.y = fVar1;
                                    speed.y = 0.0f;
                                    fopAcM_effHamonSet(&field_0x137c[0], &current.pos, 0.7f, 0.08f);
                                    field_0x138b = true;
                                } else {
                                    field_0x138b = false;
                                }
                            }
                            break;

                        case '0007': {
                        case '0005':
                            mSound.startCreatureVoiceLevel(Z2SE_TKJ_V_ODOROKI_LOOP, -1);
                            daNpcTkc_c* actor_p = NULL;
                            fopAcM_SearchByID(parentActorID, (fopAc_ac_c**)&actor_p);

                            if (actor_p != NULL) {
                                actor_p->callOdorokiSound();
                            }
                            // fallthrough
                        }
                        case '0012':
                            eventManager.cutEnd(staffId);
                        
                            if (mAcch.ChkWaterIn()) {
                                f32 fVar1 = mAcch.m_wtr.GetHeight();
                                if (current.pos.y < fVar1) {
                                    current.pos.y = fVar1;
                                    speed.y = 0.0f;
                                    fopAcM_effHamonSet(&field_0x137c[0], &current.pos, 0.7f, 0.08f);
                                    field_0x138b = true;
                                } else {
                                    field_0x138b = false;
                                }
                            }
                            break;

                        case '0004': {
                            int iVar1[2] = {0x2520, -1};
                            if (talkProc(iVar1, TRUE, NULL)) {
                                if (!mFlow.checkEndFlow()) {
                                    fopAc_ac_c* actor_p = fopAcM_SearchByName(PROC_Tag_Lv7Gate);
                                    if (actor_p != NULL) {
                                        setLookMode(LOOK_ACTOR, actor_p);
                                    }
                                } else {
                                    eventManager.cutEnd(staffId);
                                    current.pos.set(1627.059f, -250.0f, 5621.671f);
                                    old.pos = current.pos;
                                    setAngle(0x6024);
                                    setExpression(EXPR_NONE, -1.0f);
                                    setMotion(MOT_WAIT_A, -1.0f, 0);
                                    setLookMode(LOOK_NONE, NULL);

                                    daNpcTkc_c* actor_p = NULL;
                                    fopAcM_SearchByID(parentActorID, (fopAc_ac_c**)&actor_p);

                                    if (actor_p != NULL) {
                                        actor_p->setActionFollow();
                                    }
                                }
                            }

                            if (mAcch.ChkWaterIn()) {
                                f32 fVar2 = mAcch.m_wtr.GetHeight();
                                if (current.pos.y < fVar2) {
                                    current.pos.y = fVar2;
                                    speed.y = 0.0f;
                                    fopAcM_effHamonSet(&field_0x137c[0], &current.pos, 0.7f, 0.08f);
                                    field_0x138b = true;
                                } else {
                                    field_0x138b = false;
                                }
                            }
                            break;
                        }

                        case '0009': {
                            cXyz sp8c(3453.1926f, -200.0f, 5485.9224f);
                            if (step(cLib_targetAngleY(&current.pos, &sp8c), 1)) {
                                eventManager.cutEnd(staffId);
                            }
                            break;
                        }

                        case '0010':
                            if (mMotion == 4) {
                                if (mMotionPhase == 0 && mAnm_p->checkFrame(9.0f)) {
                                    speedF = 8.0f;
                                    speed.y = 30.0f;
                                    mSound.startCreatureSound(Z2SE_TKS_V_JUMP_UP, 0, -1);
                                    break;
                                }

                                if (speedF > 0.0f && mAcch.ChkGroundLanding()) {
                                    setMotion(MOT_JUMP_E, -1.0f, 0);
                                    speedF = 0.0f;
                                    speed.y = 0.0f;
                                }
                            } else if (mMotion == 5) {
                                if (mAnm_p->isStop()) {
                                    eventManager.cutEnd(staffId);
                                }
                            }
                            break;

                        case '0011': {
                            mSound.startCreatureVoiceLevel(Z2SE_TKS_V_RUNNING_LOOP, -1);
                            cXyz sp98(3453.1926f, -200.0f, 5485.9224f);
                            step(cLib_targetAngleY(&current.pos, &sp98), 0);
                            eventManager.cutEnd(staffId);
                            break;
                        }

                        default:
                            JUT_ASSERT(3263, FALSE);
                            break;
                    }
                }
            }
            break;
        
        case 3:
            break;

        default:
            JUT_ASSERT(3269, FALSE);
            break;
    }
}

void daNpcTks_c::demo_Lv3PickUp() {
    switch (mMode) {
        case 0:
            cLib_offBit<u32>(attention_info.flags, fopAc_AttnFlag_CARRY_e);
            setExpression(EXPR_NONE, -1.0f);
            setMotion(MOT_WAIT_A, -1.0f, 0);
            setLookMode(LOOK_PLAYER, NULL);

            if (strcmp(dComIfGp_getStartStageName(), "D_MN01") == 0) {
                current.pos.set(0.0f, 1225.0f, 14737.291f);
                setAngle(0);
            } else if (strcmp(dComIfGp_getStartStageName(), "D_MN07") == 0) {
                current.pos.set(0.0f, -250.0f, 3885.7244f);
                setAngle(0);
            }

            old.pos = current.pos;

            if (cLib_calcTimer<int>(&field_0x1370) <= 0) {
                if (dComIfGp_event_runCheck() && !eventInfo.checkCommandTalk()) {
                    if (dComIfGp_getEventManager().getMyStaffId(l_myName, NULL, 0) != -1) {
                        mMode = 2;
                    }
                    break;
                }

                if (daPy_getPlayerActorClass()->checkSwimUp()) {
                    eventInfo.setArchiveName(l_arcName);
                    mEventIdx = dComIfGp_getEventManager().getEventIdx(this, "TKS_WARPBACK", 0xFF);
                    fopAcM_orderOtherEventId(this, mEventIdx, 0xFF, 0xFFFF, 4, 1);
                }
            }
            break;

        case 2:
            if (dComIfGp_event_runCheck() && !eventInfo.checkCommandTalk()) {
                dEvent_manager_c& eventManager = dComIfGp_getEventManager();
                s32 staffId = eventManager.getMyStaffId(l_myName, NULL, 0);

                if (staffId != -1) {
                    int* cutName = (int*)eventManager.getMyNowCutName(staffId);

                    if (eventManager.getIsAddvance(staffId)) {
                        switch (*cutName) {
                            case '0001':
                                dComIfGs_resetLastWarpAcceptStage();
                                break;

                            case '0002':
                                initTalk(3, NULL);
                                mMsgTimer = 0;
                                mTurnMode = 0;
                                setLookMode(LOOK_PLAYER_TALK, NULL);
                                break;

                            case '0003':
                                mCyl.OffCoSetBit();
                                setMotion(MOT_JUMP_S, -1.0f, 0);
                                speedF = 0.0f;
                                speed.y = 0.0f;
                                scale.setall(1.0f);
                                break;
                                
                            default:
                                JUT_ASSERT(3348, FALSE);
                                break;
                        }
                    }
                    
                    int msgTimer = mMsgTimer;
                    int i_expression, i_motion;

                    if (ctrlMsgAnm(i_expression, i_motion, this, TRUE)) {
                        setExpression(i_expression, -1.0f);
                        setMotion(i_motion, -1.0f, 0);
                    } else if (msgTimer != 0 && mMsgTimer == 0) {
                        setExpressionTalkAfter();
                    }

                    switch (*cutName) {
                        case '0001':
                            eventManager.cutEnd(staffId);
                            break;

                        case '0002':
                            if (mCurAngle.y == fopAcM_searchPlayerAngleY(this)) {
                                if (talkProc(NULL, TRUE, NULL)) {
                                    eventManager.cutEnd(staffId);
                                }
                            } else if (step(fopAcM_searchPlayerAngleY(this), 0)) {
                                mTurnMode = 0;
                            }
                            break;

                        case '0003':
                            if (mMotionPhase == 0 && mAnm_p->checkFrame(9.0f)) {
                                speed.y = 33.0f;
                                field_0x136c = speed.y * -2.0f / gravity;
                                field_0x1370 = field_0x136c;
                                speedF = fopAcM_searchPlayerDistanceXZ(this) / field_0x136c;
                                mSound.startCreatureSound(Z2SE_TKS_V_JUMP_UP, 0, -1);
                            }

                            if (speedF > 0.0f) {
                                cLib_chaseF(&scale.x, 0.15f, 0.85f / field_0x136c);
                                scale.setall(scale.x);

                                if (cLib_calcTimer<int>(&field_0x1370) == 0) {
                                    speedF = 0.0f;
                                    speed.y = 0.0f;
                                    eventManager.cutEnd(staffId);
                                }
                            }
                            break;

                        default:
                            JUT_ASSERT(3406, FALSE);
                            break;
                    }

                    if (eventInfo.checkCommandDemoAccrpt() && mEventIdx != -1 && eventManager.endCheck(mEventIdx)) {
                        dComIfGp_event_reset();
                        mEventIdx = -1;
                        dMeter2Info_warpInProc();
                        fopAcM_delete(this);
                    }
                }
            }
            break;
            
        case 3:
            break;

        default:
            JUT_ASSERT(3424, FALSE);
            break;
    }
}

void daNpcTks_c::demo_Lv6PickUp() {
    switch (mMode) {
        case 0:
            if (strcmp(dComIfGp_getStartStageName(), "D_MN05") == 0) {
                current.pos.set(-11.226944f, 2204.15f, 17357.205f);
                setAngle(0);
            } else if (strcmp(dComIfGp_getStartStageName(), "D_MN06") == 0) {
                current.pos.set(0.0f, -275.0f, 1900.0f);
                setAngle(0);
            }

            old.pos = current.pos;

            if (fopAcM_searchPlayerDistance(this) <= 300.0f) {
                eventInfo.setArchiveName(l_arcName);
                mEventIdx = dComIfGp_getEventManager().getEventIdx(this, "TKS_WARPBACK", 0xFF);
                fopAcM_orderOtherEventId(this, mEventIdx, 0xFF, 0xFFFF, 4, 1);
                mMode = 2;
            } else {
                cLib_offBit<u32>(attention_info.flags, fopAc_AttnFlag_CARRY_e);
                setExpression(EXPR_NONE, -1.0f);
                setMotion(MOT_WAIT_A, -1.0f, 0);
                setLookMode(LOOK_PLAYER, NULL);
                break;
            }
            // fallthrough
        case 2:
            if (dComIfGp_event_runCheck() && !eventInfo.checkCommandTalk()) {
                dEvent_manager_c& eventManager = dComIfGp_getEventManager();
                s32 staffId = eventManager.getMyStaffId(l_myName, NULL, 0);

                if (staffId != -1) {
                    int* cutName = (int*)eventManager.getMyNowCutName(staffId);

                    if (eventManager.getIsAddvance(staffId)) {
                        switch (*cutName) {
                            case '0001':
                                dComIfGs_resetLastWarpAcceptStage();
                                break;

                            case '0002':
                                initTalk(3, NULL);
                                mMsgTimer = 0;
                                mTurnMode = 0;
                                setLookMode(LOOK_PLAYER_TALK, NULL);
                                break;

                            case '0003':
                                mCyl.OffCoSetBit();
                                setMotion(MOT_JUMP_S, -1.0f, 0);
                                speedF = 0.0f;
                                speed.y = 0.0f;
                                scale.setall(1.0f);
                                break;

                            default:
                                JUT_ASSERT(3499, FALSE);
                                break;
                        }
                    }

                    int msgTimer = mMsgTimer;
                    int i_expression, i_motion;

                    if (ctrlMsgAnm(i_expression, i_motion, this, TRUE)) {
                        setExpression(i_expression, -1.0f);
                        setMotion(i_motion, -1.0f, 0);
                    } else if (msgTimer != 0 && mMsgTimer == 0) {
                        setExpressionTalkAfter();
                    }

                    switch (*cutName) {
                        case '0001':
                            eventManager.cutEnd(staffId);
                            break;

                        case '0002':
                            if (mCurAngle.y == fopAcM_searchPlayerAngleY(this)) {
                                if (talkProc(NULL, TRUE, NULL)) {
                                    eventManager.cutEnd(staffId);
                                }
                            } else if (step(fopAcM_searchPlayerAngleY(this), 0)) {
                                mTurnMode = 0;
                            }
                            break;

                        case '0003':
                            if (mMotionPhase == 0 && mAnm_p->checkFrame(9.0f)) {
                                speedF = 16.0f;
                                speed.y = 36.0f;
                                field_0x1370 = 300.0f / speedF;
                                mSound.startCreatureSound(Z2SE_TKS_V_JUMP_UP, 0, -1);
                            }

                            if (speedF > 0.0f) {
                                cLib_chaseF(&scale.x, 0.15f, 0.85f / (300.0f / speedF));
                                scale.setall(scale.x);

                                if (cLib_calcTimer<int>(&field_0x1370) == 0) {
                                    speedF = 0.0f;
                                    speed.y = 0.0f;
                                    eventManager.cutEnd(staffId);
                                }
                            }
                            break;

                        default:
                            JUT_ASSERT(3650, FALSE);
                            break;
                    }

                    if (eventInfo.checkCommandDemoAccrpt() && mEventIdx != -1 && eventManager.endCheck(mEventIdx)) {
                        dComIfGp_event_reset();
                        mEventIdx = -1;
                        dMeter2Info_warpInProc();
                        fopAcM_delete(this);
                    }
                }
            }
            break;

        case 3:
            break;

        default:
            JUT_ASSERT(3568, FALSE);
            break;
    }
}

static int daNpcTks_Create(void* a_this) {
    return static_cast<daNpcTks_c*>(a_this)->Create();
}

static int daNpcTks_Delete(void* a_this) {
    return static_cast<daNpcTks_c*>(a_this)->Delete();
}

static int daNpcTks_Execute(void* a_this) {
    return static_cast<daNpcTks_c*>(a_this)->Execute();
}

static int daNpcTks_Draw(void* a_this) {
    return static_cast<daNpcTks_c*>(a_this)->Draw();
}

static int daNpcTks_IsDelete(void* a_this) {
    return 1;
}

void daNpcTks_c::setParam() {
    mAcchCir.SetWallR(daNpcTks_Param_c::m.common.width);
    mAcchCir.SetWallH(daNpcTks_Param_c::m.common.knee_length);
    gravity = fopAcM_checkCarryNow(this) != 0 ? 0.0f: daNpcTks_Param_c::m.common.gravity;
    maxFallSpeed = -50.0f;
}

BOOL daNpcTks_c::ctrlBtk() {
    if (mpMatAnm != NULL) {
        J3DAnmTextureSRTKey* i_anm = getTexSRTKeyAnmP(l_arcName, l_btkGetParamList[0].fileIdx);
        J3DAnmTextureSRTKey* i_btk = mBtkAnm.getBtkAnm();
        if (i_anm == i_btk) {
            mpMatAnm->setNowOffsetX((f32)cM_ssin(mEyeAngle.y) * 0.2f * -1.0f);
            mpMatAnm->setNowOffsetY((f32)cM_ssin(mEyeAngle.x) * 0.2f);
            mpMatAnm->onEyeMoveFlag();
            return TRUE;
        }

        mpMatAnm->offEyeMoveFlag();
    }

    return FALSE;
}

void daNpcTks_c::setAttnPos() {
    if(mLookMode == 1) {
        for (int i = 0; i < 3; i++) {
            mLookatAngle[i].setall(0);
        }
    }

    J3DModelData* mdlData_p = mAnm_p->getModel()->getModelData();
    for (u16 i = 0; i < mdlData_p->getJointNum(); i++) {
        mdlData_p->getJointNodePointer(i)->setCallBack(ctrlJointCallBack);
    }

    setMtx();
    lookat();

    if (field_0x138a) {
        eyePos = current.pos;
        eyePos.y += 80.0f;
    } else {
        cXyz sp2c(10.0f, 10.0f, 0.0f);
        mDoMtx_stack_c::copy(mAnm_p->getModel()->getAnmMtx(0x11));
        mDoMtx_stack_c::multVecZero(&mHeadPos);
        mDoMtx_stack_c::multVec(&sp2c, &eyePos);
        sp2c.x = 0.0f;
        mDoMtx_stack_c::multVec(&sp2c, &sp2c);
        mHeadAngle.x = cLib_targetAngleX(&mHeadPos, &sp2c);
        mHeadAngle.y = cLib_targetAngleY(&mHeadPos, &sp2c);
    }

    cXyz* attnPos = mLookat.getAttnPos();
    if (attnPos != NULL) {
        cXyz sp38(*attnPos - eyePos);
        mEyeAngle.y = -(mLookatAngle[2].y + mCurAngle.y);
        mEyeAngle.y += cM_atan2s(sp38.x, sp38.z);
        mEyeAngle.x = -cM_atan2s(sp38.y, sp38.absXZ());
    } else {
        mEyeAngle.y = 0;
        mEyeAngle.x = 0;
    }

    attention_info.position.set(current.pos.x, current.pos.y + daNpcTks_Param_c::m.common.attention_offset, current.pos.z);

    if (!fopAcM_checkCarryNow(this)) {
        if (!field_0x138a) {
            mCyl.SetC(current.pos);
            #if DEBUG
            mCyl.SetH(daNpcTks_Param_c::m.common.height);
            mCyl.SetR(daNpcTks_Param_c::m.common.width);
            #endif
            dComIfG_Ccsp()->Set(&mCyl);
        }

        if (mTksTsubo.field_0x586 != 0) {
            mTksTsubo.mCyl.SetC(mTksTsubo.mPos);
            dComIfG_Ccsp()->Set(&mTksTsubo.mCyl);
        }
    }
}

BOOL daNpcTks_c::drawDbgInfo() {
    return FALSE;
}

void daNpcTks_c::drawOtherMdls() {
    if (mTksTsubo.field_0x586 != 0) {
        J3DModel* model = mTksTsubo.mpModelMorf->getModel();

        if (mTwilight) {
            g_env_light.settingTevStruct(4, &mTksTsubo.mPos, &mTksTsubo.mTevStr);
        } else {
            g_env_light.settingTevStruct(8, &mTksTsubo.mPos, &mTksTsubo.mTevStr);
        }

        g_env_light.setLightTevColorType_MAJI(model, &mTksTsubo.mTevStr);
        dComIfGd_setListBG();
        mTksTsubo.mpModelMorf->entryDL();
        dComIfGd_setList();

        cXyz* pcVar1;
        if (field_0x138a) {
            pcVar1 = &current.pos;
        } else {
            pcVar1 = &mTksTsubo.mPos;
        }

        f32 fVar1;
        if (field_0x138a) {
            fVar1 = mGroundH;
        } else {
            fVar1 = mTksTsubo.mGroundH;
        }

        cBgS_PolyInfo* polyBase;
        if (field_0x138a) {
            polyBase = &mGndChk;
        } else {
            polyBase = &mTksTsubo.polyBase;
        }

        mTksTsubo.mShadowKey = dComIfGd_setShadow(mTksTsubo.mShadowKey, 1, model,
                                                  pcVar1, daNpcTks_Param_c::m.common.real_shadow_size,
                                                  30.0f, pcVar1->y, fVar1, *polyBase, &mTksTsubo.mTevStr, 0, 1.0f,
                                                  dDlst_shadowControl_c::getSimpleTex());
    }
}

static actor_method_class daNpcTks_MethodTable = {
    (process_method_func)daNpcTks_Create,
    (process_method_func)daNpcTks_Delete,
    (process_method_func)daNpcTks_Execute,
    (process_method_func)daNpcTks_IsDelete,
    (process_method_func)daNpcTks_Draw,
};

actor_process_profile_definition g_profile_NPC_TKS = {
  fpcLy_CURRENT_e,        // mLayerID
  7,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_NPC_TKS,           // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(daNpcTks_c),     // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  374,                    // mPriority
  &daNpcTks_MethodTable,  // sub_method
  0x08044108,             // mStatus
  fopAc_NPC_e,            // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};
