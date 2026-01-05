/**
 * @file d_a_npc_zrc.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_npc_zrc.h"
#include "d/actor/d_a_npc.h"
#include "SSystem/SComponent/c_math.h"
#include "JSystem/J3DGraphBase/J3DMaterial.h"
#include "Z2AudioLib/Z2Instances.h"

enum Event_Cut_Nums {
    /* 0x2 */ NUM_EVT_CUTS_e = 0x2,
};

static daNpc_zrC_Param_c l_HIO;

daNpc_zrC_Param_c::param const daNpc_zrC_Param_c::m = {
    50.0f,    // mAttnOffsetY
    -3.0f,    // mGravity
    1.0f,     // mScale
    600.0f,   // mShadowDepth
    255.0f,   // mCcWeight
    140.0f,   // mCylH
    0.0f,     // mWallH
    30.0f,    // mWallR
    0.0f,     // mBodyUpAngle
    0.0f,     // mBodyDownAngle
    10.0f,    // mBodyLeftAngle
    -10.0f,   // mBodyRightAngle
    30.0f,    // mHeadUpAngle
    -10.0f,   // mHeadDownAngle
    45.0f,    // mHeadLeftAngle
    -45.0f,   // mHeadRightAngle
    0.6f,     // mNeckAngleScl
    12.0f,    // mMorfFrames
    3,        // mSpeakDistIdx
    6,        // mSpeakAngleIdx
    5,        // mTalkDistIdx
    6,        // mTalkAngleIdx
    80.0f,    // mAttnFovY
    500.0f,   // mAttnRadius
    300.0f,   // mAttnUpperY
    -300.0f,  // mAttnLowerY
    60,
    8,        // mDamageTimer
    0,        // mTestExpression
    0,        // mTestMotion
    0,        // mTestLookMode
    false,    // mTest
    350.0f,
    2300.0f,
};

static daNpc_GetParam1 l_bmdGetParamList[2] = {
    {3, 3},   // zrc
    {3, 4},   // zrc_sick_tw
};

static daNpc_GetParam1 l_bckGetParamList[32] = {
    {9, 0},   // zrc_f_wait_a
    {5, 2},   // zrc_f_sit_talk
    {8, 0},   // zrc_f_talk_a
    {11, 1},  // zrc_f_topray
    {5, 0},   // zrc_f_sad
    {6, 0},   // zrc_f_smile
    {6, 1},   // zrc_f_kizuku
    {9, 1},   // zrc_f_sad_wait
    {7, 1},   // zrc_f_kizuku_wait
    {10, 1},  // zrc_f_talk_normal
    {8, 1},   // zrc_f_pray
    {5, 1},   // zrc_f_getup
    {4, 2},   // zrc_f_sit
    {7, 0},   // zrc_f_step
    {10, 0},  // zrc_fh_sad
    {11, 0},  // zrc_fh_smile
    {12, 1},  // zrc_fh_kizuku
    {4, 5},   // zrc_f_sick_demo_wait
    {13, 0},  // zrc_wait_a
    {21, 1},  // zrc_topray
    {16, 1},  // zrc_pray
    {13, 1},  // zrc_getup
    {12, 0},  // zrc_step
    {6, 2},   // zrc_sit
    {7, 2},   // zrc_sit_talk
    {17, 1},  // zrc_sad_talk
    {18, 1},  // zrc_sad_wait
    {20, 1},  // zrc_talk_a
    {14, 1},  // zrc_kizuku
    {15, 1},  // zrc_kizuku_wait
    {19, 1},  // zrc_smile_talk
    {5, 5},   // zrc_sick_demo_wait
};

static daNpc_GetParam1 l_btpGetParamList[15] = {
    {19, 0},  // zrc
    {10, 2},  // zrc_f_sit_talk
    {23, 0},  // zrc_f_talk_a
    {20, 0},  // zrc_f_sad
    {21, 0},  // zrc_f_smile
    {31, 1},  // zrc_f_kizuku
    {34, 1},  // zrc_f_sad_wait
    {32, 1},  // zrc_f_kizuku_wait
    {35, 1},  // zrc_f_topray
    {33, 1},  // zrc_f_pray
    {30, 1},  // zrc_f_getup
    {22, 0},  // zrc_f_step
    {24, 0},  // zrc_fh_sad
    {25, 0},  // zrc_fh_smile
    {8, 5},   // zrc_f_sick_demo_wait
};

static daNpc_GetParam1 l_btkGetParamList[5] = {
    {16, 0},  // zrc
    {26, 1},  // zrc_sad_talk
    {27, 1},  // zrc_sad_wait
    {24, 1},  // zrc_kizuku
    {25, 1},  // zrc_kizuku_wait
};

static daNpc_GetParam1 l_evtGetParamList[2] = {
    {0, 0},
    {1, 6},
};

static char* l_evtNames[2] = {
    NULL,
    "EARRING_GET",
};

static int l_loadRes_ZRCa[5] = {
    0, 3, -1, -1, 0,
};

static int l_loadRes_ZRC_SICK[5] = {
    3, 5, 0, -1, 0,
};

static int l_loadRes_ZRC0[5] = {
    0, 3, -1, -1, 0,
};

static int l_loadRes_ZRC_PRAY[5] = {
    0, 3, 1, 6, -1,
};

static int l_loadRes_ZRC_SIT[5] = {
    0, 2, 3, -1, 0,
};

static int* l_loadRes_list[5] = {
    l_loadRes_ZRCa,
    l_loadRes_ZRC_SICK,
    l_loadRes_ZRC_PRAY,
    l_loadRes_ZRC_SIT,
    l_loadRes_ZRC0,
};

static char* l_resNames[7] = {
    "zrC",
    "zrC1",
    "zrC2",
    "zrC_MDL",
    "zrC_TW",
    "zrC_Sick",
    "zrC_EGD",
};

static char* l_myName = "zrC";

char* daNpc_zrC_c::mEvtCutNameList[2] = {
    "",
    "EARRING_GET",
};

daNpc_zrC_c::EventFn daNpc_zrC_c::mEvtCutList[2] = {
    NULL,
    &daNpc_zrC_c::ECut_earringGet,
};

daNpc_zrC_c::daNpc_zrC_c() {
}

daNpc_zrC_c::~daNpc_zrC_c() {
    for (int i = 0; l_loadRes_list[mType][i] >= 0; i++) {
        int res = l_loadRes_list[mType][i];
        if (res == 3 && mTwilight) {
            res = 4;
        }
        dComIfG_resDelete(&mPhase[i], l_resNames[res]);
    }

    if (heap != NULL) {
        mAnm_p->stopZelAnime();
    }
}

cPhs__Step daNpc_zrC_c::create() {
    fopAcM_ct(this, daNpc_zrC_c);

    mType = getTypeFromParam();
    if (home.angle.x != 0xffff) {
        mFlowID = home.angle.x;
    } else {
        mFlowID = -1;
    }
    mTwilight = dKy_darkworld_check();

    if (isDelete()) {
        return cPhs_ERROR_e;
    }

    int res_count = 0;
    int i;
    for (i = 0; l_loadRes_list[mType][i] >= 0; i++) {
        int res_idx = l_loadRes_list[mType][i];
        if (res_idx == 3 && mTwilight) {
            res_idx = 4;
        }
        cPhs__Step step = (cPhs__Step)dComIfG_resLoad(&mPhase[i], l_resNames[res_idx]);
        if (step == cPhs_ERROR_e || step == cPhs_UNK3_e) {
            return cPhs_ERROR_e;
        }
        if (step == cPhs_COMPLEATE_e) {
            res_count++;
        }
    }

    if (res_count == i) {
        static u32 const heapSize[5] = {0x6010, 0x4690, 0x6010, 0x6010, 0x0000};
        if (!fopAcM_entrySolidHeap(this, createHeapCallBack, heapSize[mType])) {
            return cPhs_ERROR_e;
        }

        fopAcM_SetMtx(this, mAnm_p->getModel()->getBaseTRMtx());
        fopAcM_setCullSizeBox(this, -300.0f, -50.0f, -300.0f, 300.0f, 450.0f, 300.0f);
        mCreatureSound.init(&current.pos, &eyePos, 3, 1);
        mAcchCir.SetWall(daNpc_zrC_Param_c::m.mWallR, daNpc_zrC_Param_c::m.mWallH);
        mAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1, &mAcchCir,
                  fopAcM_GetSpeed_p(this), fopAcM_GetAngle_p(this), fopAcM_GetShapeAngle_p(this));
        mCcStts.Init(daNpc_zrC_Param_c::m.mCcWeight, 0, this);
        mCcCyl.Set(mCcDCyl);
        mCcCyl.SetStts(&mCcStts);
        mCcCyl.SetTgHitCallback(tgHitCallBack);
        mAcch.CrrPos(dComIfG_Bgsp());
        mGndChk = mAcch.m_gnd;
        mGroundH = mAcch.GetGroundH();

        setEnvTevColor();
        setRoomNo();
        reset();
        Execute();

        return cPhs_COMPLEATE_e;
    } else {
        return cPhs_INIT_e;
    }
}

int daNpc_zrC_c::CreateHeap() {
    J3DModelData* model_data = NULL;
    u32 model_flag;
    if (mTwilight) {
        if (l_bmdGetParamList[1].fileIdx >= 0) {
            model_data = static_cast<J3DModelData*>(
                dComIfG_getObjectRes(l_resNames[l_bmdGetParamList[1].arcIdx],
                                     l_bmdGetParamList[1].fileIdx));
        }
        model_flag = 0x80000;
    } else {
        if (l_bmdGetParamList[0].fileIdx >= 0) {
            model_data = static_cast<J3DModelData*>(
                dComIfG_getObjectRes(l_resNames[l_bmdGetParamList[0].arcIdx],
                                     l_bmdGetParamList[0].fileIdx));
        }
        model_flag = 0;
    }

    mAnm_p = new mDoExt_McaMorfSO(model_data, NULL, NULL, NULL, -1, 1.0f, 0, -1,
                                  &mCreatureSound, model_flag, 0x11020284);
    if (mAnm_p != NULL && mAnm_p->getModel() == NULL) {
        mAnm_p->stopZelAnime();
        mAnm_p = NULL;
    }
    if (mAnm_p == NULL) {
        return 0;
    }
    
    J3DModel* model = mAnm_p->getModel();
    for (u16 i = 0; i < model_data->getJointNum(); i++) {
        model_data->getJointNodePointer(i)->setCallBack(ctrlJointCallBack);
    }
    model->setUserArea((uintptr_t)this);

    mpMatAnm = new daNpcF_MatAnm_c();
    if (mpMatAnm == NULL) {
        return 0;
    }

    if (mType == 1) {
        if (!setExpressionAnm(ANM_F_SICK_DEMO_WAIT, false)) {
            return 0;
        }
        setMotionAnm(ANM_SICK_DEMO_WAIT, 0.0f);
    } else {
        if (!setExpressionAnm(ANM_F_TALK_A, false)) {
            return 0;
        }
        setMotionAnm(ANM_WAIT_A, 0.0f);
    }

    return 1;
}

int daNpc_zrC_c::Delete() {
    this->~daNpc_zrC_c();
    return 1;
}

int daNpc_zrC_c::Execute() {
    return execute();
}

int daNpc_zrC_c::Draw() {
    mAnm_p->getModel()->getModelData()->getMaterialNodePointer(1)->setMaterialAnm(mpMatAnm);
    BOOL is_test = chkAction(&daNpc_zrC_c::test);
    return draw(is_test, true, daNpc_zrC_Param_c::m.mShadowDepth, NULL, false);
}

int daNpc_zrC_c::ctrlJoint(J3DJoint* i_joint, J3DModel* i_model) {
    int jnt_no = i_joint->getJntNo();
    int lookat_joints[3] = {1, 3, 4};

    if (jnt_no == 0) {
        mDoMtx_stack_c::copy(mAnm_p->getModel()->getAnmMtx(1));
        mDoMtx_stack_c::multVecZero(&mLookatPos[0]);
        mDoMtx_stack_c::copy(mAnm_p->getModel()->getAnmMtx(3));
        mDoMtx_stack_c::multVecZero(&mLookatPos[1]);
        mDoMtx_stack_c::copy(mAnm_p->getModel()->getAnmMtx(4));
        mDoMtx_stack_c::multVecZero(&mLookatPos[2]);
    }

    mDoMtx_stack_c::copy(i_model->getAnmMtx(jnt_no));

    switch (jnt_no) {
    case 1:   // backbone1
    case 3:   // neck
    case 4:   // head
        setLookatMtx(jnt_no, lookat_joints, daNpc_zrC_Param_c::m.mNeckAngleScl);
        break;
    }

    if (jnt_no == 1) {
        mDoMtx_stack_c::YrotM(-field_0x908[0].z);
        mDoMtx_stack_c::ZrotM(field_0x908[0].x);
    } else if (jnt_no == 4) {
        mDoMtx_stack_c::YrotM(-field_0x908[2].z);
        mDoMtx_stack_c::ZrotM(field_0x908[2].x);
    }

    i_model->setAnmMtx(jnt_no, mDoMtx_stack_c::get());
    MTXCopy(mDoMtx_stack_c::get(), J3DSys::mCurrentMtx);

    if ((jnt_no == 4 || jnt_no == 13) && (mAnmFlags & ANM_PLAY_BCK)) {
        J3DAnmTransform* bck_anm = mBckAnm.getBckAnm();
        mBckAnm.changeBckOnly(mAnm_p->getAnm());
        mAnm_p->changeAnm(bck_anm);
    }

    return 1;
}

int daNpc_zrC_c::createHeapCallBack(fopAc_ac_c* i_this) {
    return static_cast<daNpc_zrC_c*>(i_this)->CreateHeap();
}

int daNpc_zrC_c::ctrlJointCallBack(J3DJoint* i_joint, int param_1) {
    if (param_1 == 0) {
        daNpc_zrC_c* _this = (daNpc_zrC_c*)j3dSys.getModel()->getUserArea();
        if (_this != NULL) {
            _this->ctrlJoint(i_joint, j3dSys.getModel());
        }
    }
    return 1;
}

void daNpc_zrC_c::setParam() {
    ActionFn action = mpNextActionFn;
    u32 attn_flags = 10;
    selectAction();

    if (!mTwilight && daPy_py_c::checkNowWolf()) {
        attn_flags = 0;
    }

    if (mpNextActionFn != action) {
        for (int i = 3; i < 3; i++) {
            mActorMngr[i].initialize();
        }
    }

    field_0xe10 = 0;
    field_0xe14 = 0;

    if (mType != 1) {
        if (mType == 2) {
            if (!field_0xe30 || daPy_getPlayerActorClass()->checkPlayerFly()
                || !daPy_getPlayerActorClass()->checkSwimUp()
                || dComIfGp_checkPlayerStatus0(0, 0x100))
            {
                attn_flags = 0;
            }
        }

        attention_info.distances[fopAc_attn_LOCK_e] = getDistTableIdx(5, 6);
        attention_info.distances[fopAc_attn_TALK_e] = attention_info.distances[fopAc_attn_LOCK_e];
        attention_info.distances[fopAc_attn_SPEAK_e] = getDistTableIdx(3, 6);
        attention_info.flags = attn_flags;

        if (mType == 3) {
            gravity = 0.0f;
        } else {
            gravity = daNpc_zrC_Param_c::m.mGravity;
        }

    } else {
        attention_info.distances[fopAc_attn_LOCK_e] = 0;
        attention_info.distances[fopAc_attn_TALK_e] = 0;
        attention_info.distances[fopAc_attn_SPEAK_e] = 0;
        attention_info.flags = 0;
        gravity = 0.0f;
    }

    scale.set(daNpc_zrC_Param_c::m.mScale,
              daNpc_zrC_Param_c::m.mScale,
              daNpc_zrC_Param_c::m.mScale);
    
    mAcchCir.SetWallR(daNpc_zrC_Param_c::m.mWallR);
    mAcchCir.SetWallH(daNpc_zrC_Param_c::m.mWallH);
}

BOOL daNpc_zrC_c::main() {
    if (!doEvent()) {
        doNormalAction(TRUE);
    }

    if (checkHide()) {
        attention_info.flags = 0;
    }

    if (!daNpc_zrC_Param_c::m.mTest
        && (!dComIfGp_event_runCheck() || (mOrderNewEvt && dComIfGp_getEvent()->isOrderOK())))
    {
        if (mOrderEvtNo != EVT_NONE) {
            eventInfo.setArchiveName(l_resNames[l_evtGetParamList[mOrderEvtNo].arcIdx]);
        }
        orderEvent(mSpeakEvent, l_evtNames[l_evtGetParamList[mOrderEvtNo].fileIdx],
                   0xffff, 0x28, 0xff, 1);

        if (mType == 2) {
            eventInfo.onCondition(dEvtCnd_CANTALKITEM_e);
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

BOOL daNpc_zrC_c::ctrlBtk() {
    if (mpMatAnm != NULL) {
        J3DAnmTextureSRTKey* btk_anm = NULL;
        if (l_btkGetParamList[0].fileIdx >= 0) {
            btk_anm = getTexSRTKeyAnmP(l_resNames[l_btkGetParamList[0].arcIdx],
                                        l_btkGetParamList[0].fileIdx);
        }

        if (btk_anm == mBtkAnm.getBtkAnm()) {
            mpMatAnm->setNowOffsetX(cM_ssin(mEyeAngle.y) * 0.2f * -1.0f);
            mpMatAnm->setNowOffsetY(cM_ssin(mEyeAngle.x) * 0.2f);
            mpMatAnm->onEyeMoveFlag();
            return TRUE;
        }
        mpMatAnm->offEyeMoveFlag();
    }

    return FALSE;
}

void daNpc_zrC_c::setAttnPos() {
    static cXyz eyeOffset(10.0f, 10.0f, 0.0f);
    f32 offset = daNpc_zrC_Param_c::m.mAttnOffsetY;
    cXyz center, vec2, vec3, vec4;

    mDoMtx_stack_c::YrotS(field_0x990);

    cLib_addCalc2(&field_0x984[0], 0.0f, 0.1f, 125.0f);
    cLib_addCalc2(&field_0x984[2], 0.0f, 0.1f, 125.0f);

    for (int i = 0; i < 3; i++) {
        vec3.set(0.0f, 0.0f, field_0x984[i] * cM_ssin(field_0x992));
        mDoMtx_stack_c::multVec(&vec3, &vec4);
        field_0x908[i].x = -vec4.z;
        field_0x908[i].z = -vec4.x;
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
    eyeOffset.x = 0.0f;
    mDoMtx_stack_c::multVec(&eyeOffset, &vec3);
    mHeadAngle.x = cLib_targetAngleX(&mHeadPos, &vec3);
    mHeadAngle.y = cLib_targetAngleY(&mHeadPos, &vec3);

    cXyz* attn_pos = mLookat.getAttnPos();
    if (attn_pos != NULL) {
        vec2 = *attn_pos - eyePos;
        mEyeAngle.y = -(mLookatAngle[2].y + mCurAngle.y);
        mEyeAngle.y += cM_atan2s(vec2.x, vec2.z);
        mEyeAngle.x = -cM_atan2s(vec2.y, vec2.absXZ());
        mEyeAngle.x += mHeadAngle.x;
    } else {
        mEyeAngle.x = mEyeAngle.y = 0;
    }

    f32 extra_height, extra_radius;
    if (mType == 1) {
        extra_height = -20.0f;
        extra_radius = 15.0f;
        vec2.set(0.0f, 0.0f, 0.0f);
        mDoMtx_stack_c::copy(mAnm_p->getModel()->getAnmMtx(0));
        mDoMtx_stack_c::multVec(&vec2, &center);
        attention_info.position.set(center.x, center.y + offset, center.z);
    } else {
        extra_height = 0.0f;
        extra_radius = 0.0f;
        vec2.set(0.0f, 0.0f, 0.0f);
        center = mHeadPos;
        attention_info.position.set(center.x, center.y + offset, center.z);
        center.y = current.pos.y;
    }

    if (!mHide) {
        if (!mIsDamaged) {
            mCcCyl.SetTgType(0xD8FBFDFF);
            mCcCyl.SetTgSPrm(0x1F);
            mCcCyl.OnTgNoHitMark();
        } else {
            mCcCyl.SetTgType(0);
            mCcCyl.SetTgSPrm(0);
        }
        mCcCyl.SetC(center);
        mCcCyl.SetH(daNpc_zrC_Param_c::m.mCylH + extra_height);
        mCcCyl.SetR(daNpc_zrC_Param_c::m.mWallR + extra_radius);
        dComIfG_Ccsp()->Set(&mCcCyl);
    }
    mCcCyl.ClrTgHit();
}

bool daNpc_zrC_c::setExpressionAnm(int i_idx, bool i_modify) {
    J3DAnmTransform* bck_anm = NULL;
    int attr = J3DFrameCtrl::EMode_NONE;
    mAnmFlags &= ~ANM_EXPRESSION_FLAGS;

    if (l_bckGetParamList[i_idx].fileIdx >= 0) {
        bck_anm = getTrnsfrmKeyAnmP(l_resNames[l_bckGetParamList[i_idx].arcIdx],
                                    l_bckGetParamList[i_idx].fileIdx);
    }

    bool res = false;
    switch (i_idx) {
    case ANM_F_WAIT_A:
        res = setExpressionBtp(0);
        attr = J3DFrameCtrl::EMode_LOOP;
        break;
    case ANM_F_SIT_TALK:
        res = setExpressionBtp(1);
        break;
    case ANM_F_TALK_A:
        res = setExpressionBtp(2);
        break;
    case ANM_F_TOPRAY:
        res = setExpressionBtp(8);
        break;
    case ANM_F_SAD:
        res = setExpressionBtp(3);
        break;
    case ANM_F_SMILE:
        res = setExpressionBtp(4);
        break;
    case ANM_F_KIZUKU:
        res = setExpressionBtp(5);
        break;
    case ANM_F_SAD_WAIT:
        res = setExpressionBtp(6);
        break;
    case ANM_F_KIZUKU_WAIT:
        res = setExpressionBtp(7);
        break;
    case ANM_F_TALK_NORMAL:
        res = setExpressionBtp(0);
        break;
    case ANM_F_PRAY:
        res = setExpressionBtp(9);
        attr = J3DFrameCtrl::EMode_LOOP;
        break;
    case ANM_F_GETUP:
        res = setExpressionBtp(10);
        break;
    case ANM_F_SIT:
        res = setExpressionBtp(0);
        attr = J3DFrameCtrl::EMode_LOOP;
        break;
    case ANM_F_STEP:
        res = setExpressionBtp(11);
        break;
    case ANM_FH_SAD:
        res = setExpressionBtp(12);
        attr = J3DFrameCtrl::EMode_LOOP;
        break;
    case ANM_FH_SMILE:
        res = setExpressionBtp(13);
        attr = J3DFrameCtrl::EMode_LOOP;
        break;
    case ANM_FH_KIZUKU:
        res = setExpressionBtp(0);
        attr = J3DFrameCtrl::EMode_LOOP;
        break;
    case ANM_F_SICK_DEMO_WAIT:
        res = setExpressionBtp(14);
        attr = J3DFrameCtrl::EMode_LOOP;
        break;
    default:
        bck_anm = NULL;
        break;
    }

    if (!res) {
        return false;
    }

    if (bck_anm == NULL) {
        return true;
    }

    if (setBckAnm(bck_anm, 1.0f, attr, 0, -1, i_modify)) {
        mAnmFlags |= ANM_PLAY_BCK | ANM_PAUSE_BCK;
        mExpressionLoops = 0;
        return true;
    }

    return false;
}

bool daNpc_zrC_c::setExpressionBtp(int i_idx) {
    J3DAnmTexPattern* btp_anm = NULL;
    int attr = J3DFrameCtrl::EMode_NONE;
    mAnmFlags &= ~(ANM_PLAY_BTP | ANM_PAUSE_BTP | ANM_FLAG_800);

    if (l_btpGetParamList[i_idx].fileIdx >= 0) {
        btp_anm = getTexPtrnAnmP(l_resNames[l_btpGetParamList[i_idx].arcIdx],
                                    l_btpGetParamList[i_idx].fileIdx);
    }

    switch (i_idx) {
    case 0:
        attr = J3DFrameCtrl::EMode_LOOP;
        break;
    case 9:
        attr = J3DFrameCtrl::EMode_LOOP;
        break;
    case 12:
        attr = J3DFrameCtrl::EMode_LOOP;
        break;
    case 13:
        attr = J3DFrameCtrl::EMode_LOOP;
        break;
    case 14:
        attr = J3DFrameCtrl::EMode_LOOP;
        break;
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 10:
    case 11:
        break;
    default:
        btp_anm = NULL;
        break;
    }

    if (btp_anm == NULL) {
        return true;
    }

    if (setBtpAnm(btp_anm, mAnm_p->getModel()->getModelData(), 1.0f, attr)) {
        mAnmFlags |= ANM_PLAY_BTP | ANM_PAUSE_BTP;
        if (i_idx == 0) {
            mAnmFlags |= ANM_FLAG_800;
        }
        return true;
    }

    return false;
}

void daNpc_zrC_c::setExpression(int i_expression, f32 i_morf) {
    if (i_expression >= 0 && i_expression < 18) {
        mExpression = i_expression;
        mExpressionMorfOverride = i_morf;
        mExpressionPrevPhase = -1;
        mExpressionPhase = 0;
    }
}

void daNpc_zrC_c::setMotionAnm(int i_idx, f32 i_morf) {
    J3DAnmTransformKey* bck_anm = NULL;
    J3DAnmTextureSRTKey* btk_anm = NULL;
    int btk_idx = 0;
    int attr = J3DFrameCtrl::EMode_LOOP;
    int btk_attr = J3DFrameCtrl::EMode_LOOP;
    mAnmFlags &= ~ANM_MOTION_FLAGS;

    if (l_bckGetParamList[i_idx].fileIdx >= 0) {
        bck_anm = getTrnsfrmKeyAnmP(l_resNames[l_bckGetParamList[i_idx].arcIdx],
                                    l_bckGetParamList[i_idx].fileIdx);
    }

    switch (i_idx) {
    case ANM_WAIT_A:
        break;
    case ANM_TOPRAY:
        attr = J3DFrameCtrl::EMode_NONE;
        break;
    case ANM_PRAY:
        break;
    case ANM_GETUP:
        attr = J3DFrameCtrl::EMode_NONE;
        break;
    case ANM_STEP:
        attr = J3DFrameCtrl::EMode_NONE;
        break;
    case ANM_SIT:
        break;
    case ANM_SIT_TALK:
        attr = J3DFrameCtrl::EMode_NONE;
        break;
    case ANM_SAD_TALK:
        attr = J3DFrameCtrl::EMode_NONE;
        btk_idx = 1;
        btk_attr = J3DFrameCtrl::EMode_NONE;
        break;
    case ANM_SAD_WAIT:
        btk_idx = 2;
        break;
    case ANM_TALK_A:
        attr = J3DFrameCtrl::EMode_NONE;
        break;
    case ANM_KIZUKU:
        attr = J3DFrameCtrl::EMode_NONE;
        btk_idx = 3;
        btk_attr = J3DFrameCtrl::EMode_NONE;
        break;
    case ANM_KIZUKU_WAIT:
        btk_idx = 4;
        break;
    case ANM_SMILE_TALK:
        attr = J3DFrameCtrl::EMode_NONE;
        break;
    case ANM_SICK_DEMO_WAIT:
        break;
    default:
        bck_anm = NULL;
        btk_anm = NULL;
        break;
    }

    if (l_btkGetParamList[btk_idx].fileIdx >= 0) {
        btk_anm = getTexSRTKeyAnmP(l_resNames[l_btkGetParamList[btk_idx].arcIdx],
                                    l_btkGetParamList[btk_idx].fileIdx);
    }

    if (btk_anm != NULL && setBtkAnm(btk_anm, mAnm_p->getModel()->getModelData(), 1.0f, btk_attr)) {
        mAnmFlags |= ANM_PLAY_BTK | ANM_PAUSE_BTK;
    }

    if (bck_anm != NULL && setMcaMorfAnm(bck_anm, 1.0f, i_morf, attr, 0, -1)) {
        mAnmFlags |= ANM_PLAY_MORF | ANM_PAUSE_MORF;
        mMotionLoops = 0;
        mMotionAnm = i_idx;
    }
}

void daNpc_zrC_c::setMotion(int i_motion, f32 i_morf, BOOL i_restart) {
    s16 motion = i_motion;
    if (i_restart || mMotion != motion) {
        if (i_motion >= 0 && i_motion < 14) {
            mMotion = motion;
            mMotionMorfOverride = i_morf;
            mMotionPrevPhase = -1;
            mMotionPhase = 0;
        }
    }
}

BOOL daNpc_zrC_c::drawDbgInfo() {
    return false;
}

void daNpc_zrC_c::drawOtherMdls() {
    /* empty function */
}

u8 daNpc_zrC_c::getTypeFromParam() {
    switch (fopAcM_GetParam(this) & 0xff) {
    case 0:
        return 0;
    case 1:
        return 1;
    case 2:
        return 2;
    case 3:
        return 3;
    default:
        return 0;
    }
}

int daNpc_zrC_c::isDelete() {
    if (mType == 4 || mType == 0 || mType == 1 || (mType == 2 && daNpcF_chkEvtBit(0x108)
        && !daNpcF_chkEvtBit(0x10A)) || mType == 3)
    {
        return false;
    }
    return true;
}

void daNpc_zrC_c::reset() {
    initialize();
    mpMatAnm->initialize();
    mLookat.initialize();
    for (int i = 0; i < 3; i++) {
        mActorMngr[i].initialize();
    }
    mpNextActionFn = NULL;
    mpActionFn = NULL;
    field_0xe10 = 0;
    field_0xe14 = 0;
    field_0xe18 = 0;
    field_0xe1c = 0;
    mLookMode = -1;
    mMode = 0;
    mItemID = -1;
    current.pos = home.pos;
    old.pos = current.pos;
    current.angle.set(0, home.angle.y, 0);
    old.angle = current.angle;
    shape_angle = current.angle;
    mCurAngle = current.angle;
    mOldAngle = mCurAngle;
    speedF = 0.0f;
    speed.setall(0.0f);
    setLookMode(LOOK_NONE);
    if (mType == 1) {
        fopAcM_OffStatus(this, 0x100);
    } else if (mType == 2) {
        setExpression(EXPR_PRAY, -1.0f);
        setMotion(2, -1.0f, FALSE);
    }
    field_0xe30 = false;
    mSpeakEvent = false;
    field_0x9ee = true;
}

void daNpc_zrC_c::playExpression() {
    daNpcF_anmPlayData dat0a = {ANM_F_TALK_A, daNpc_zrC_Param_c::m.mMorfFrames, 1};
    daNpcF_anmPlayData dat0b = {ANM_F_WAIT_A, daNpc_zrC_Param_c::m.mMorfFrames, 0};
    daNpcF_anmPlayData* pDat0[2] = {&dat0a, &dat0b};
    daNpcF_anmPlayData dat1a = {ANM_F_SIT_TALK, daNpc_zrC_Param_c::m.mMorfFrames, 1};
    daNpcF_anmPlayData dat1b = {ANM_F_SIT, daNpc_zrC_Param_c::m.mMorfFrames, 0};
    daNpcF_anmPlayData* pDat1[2] = {&dat1a, &dat1b};
    daNpcF_anmPlayData dat2 = {ANM_F_PRAY, daNpc_zrC_Param_c::m.mMorfFrames, 0};
    daNpcF_anmPlayData* pDat2[1] = {&dat2};
    daNpcF_anmPlayData dat3a = {ANM_F_SAD, daNpc_zrC_Param_c::m.mMorfFrames, 1};
    daNpcF_anmPlayData dat3b = {ANM_FH_SAD, daNpc_zrC_Param_c::m.mMorfFrames, 0};
    daNpcF_anmPlayData* pDat3[2] = {&dat3a, &dat3b};
    daNpcF_anmPlayData dat4a = {ANM_F_SMILE, daNpc_zrC_Param_c::m.mMorfFrames, 1};
    daNpcF_anmPlayData dat4b = {ANM_FH_SMILE, daNpc_zrC_Param_c::m.mMorfFrames, 0};
    daNpcF_anmPlayData* pDat4[2] = {&dat4a, &dat4b};
    daNpcF_anmPlayData dat5a = {ANM_F_KIZUKU, daNpc_zrC_Param_c::m.mMorfFrames, 1};
    daNpcF_anmPlayData dat5b = {ANM_FH_KIZUKU, daNpc_zrC_Param_c::m.mMorfFrames, 0};
    daNpcF_anmPlayData* pDat5[2] = {&dat5a, &dat5b};
    daNpcF_anmPlayData dat6a = {ANM_F_SAD_WAIT, daNpc_zrC_Param_c::m.mMorfFrames, 1};
    daNpcF_anmPlayData dat6b = {ANM_FH_SAD, daNpc_zrC_Param_c::m.mMorfFrames, 0};
    daNpcF_anmPlayData* pDat6[2] = {&dat6a, &dat6b};
    daNpcF_anmPlayData dat7a = {ANM_F_TALK_NORMAL, daNpc_zrC_Param_c::m.mMorfFrames, 1};
    daNpcF_anmPlayData dat7b = {ANM_F_WAIT_A, daNpc_zrC_Param_c::m.mMorfFrames, 0};
    daNpcF_anmPlayData* pDat7[2] = {&dat7a, &dat7b};
    daNpcF_anmPlayData dat8a = {ANM_F_KIZUKU_WAIT, daNpc_zrC_Param_c::m.mMorfFrames, 1};
    daNpcF_anmPlayData dat8b = {ANM_FH_KIZUKU, 0.0f, 0};
    daNpcF_anmPlayData* pDat8[2] = {&dat8a, &dat8b};
    daNpcF_anmPlayData dat9 = {ANM_FH_KIZUKU, daNpc_zrC_Param_c::m.mMorfFrames, 0};
    daNpcF_anmPlayData* pDat9[1] = {&dat9};
    daNpcF_anmPlayData dat10 = {ANM_FH_SAD, daNpc_zrC_Param_c::m.mMorfFrames, 0};
    daNpcF_anmPlayData* pDat10[1] = {&dat10};
    daNpcF_anmPlayData dat11 = {ANM_FH_SMILE, daNpc_zrC_Param_c::m.mMorfFrames, 0};
    daNpcF_anmPlayData* pDat11[1] = {&dat11};
    daNpcF_anmPlayData dat12 = {ANM_F_STEP, daNpc_zrC_Param_c::m.mMorfFrames, 0};
    daNpcF_anmPlayData* pDat12[1] = {&dat12};
    daNpcF_anmPlayData dat13a = {ANM_F_TOPRAY, daNpc_zrC_Param_c::m.mMorfFrames, 1};
    daNpcF_anmPlayData dat13b = {ANM_F_PRAY, daNpc_zrC_Param_c::m.mMorfFrames, 0};
    daNpcF_anmPlayData* pDat13[2] = {&dat13a, &dat13b};
    daNpcF_anmPlayData dat14a = {ANM_F_GETUP, daNpc_zrC_Param_c::m.mMorfFrames, 1};
    daNpcF_anmPlayData dat14b = {ANM_F_WAIT_A, daNpc_zrC_Param_c::m.mMorfFrames, 0};
    daNpcF_anmPlayData* pDat14[2] = {&dat14a, &dat14b};
    daNpcF_anmPlayData dat15 = {ANM_F_SIT, daNpc_zrC_Param_c::m.mMorfFrames, 0};
    daNpcF_anmPlayData* pDat15[1] = {&dat15};
    daNpcF_anmPlayData dat16 = {ANM_F_SICK_DEMO_WAIT, daNpc_zrC_Param_c::m.mMorfFrames, 0};
    daNpcF_anmPlayData* pDat16[1] = {&dat16};
    daNpcF_anmPlayData dat17 = {ANM_F_WAIT_A, daNpc_zrC_Param_c::m.mMorfFrames, 0};
    daNpcF_anmPlayData* pDat17[1] = {&dat17};
    daNpcF_anmPlayData** ppDat[18] = {
        pDat0, pDat1, pDat2, pDat3, pDat4, pDat5, pDat6, pDat7, pDat8, pDat9, pDat10, pDat11,
        pDat12, pDat13, pDat14, pDat15, pDat16, pDat17,
    };
    if (mExpression >= 0 && mExpression < 18) {
        playExpressionAnm(ppDat);
    }
}

void daNpc_zrC_c::playMotion() {
    daNpcF_anmPlayData dat0 = {ANM_WAIT_A, daNpc_zrC_Param_c::m.mMorfFrames, 0};
    daNpcF_anmPlayData* pDat0[1] = {&dat0};
    daNpcF_anmPlayData dat1a = {ANM_SIT_TALK, daNpc_zrC_Param_c::m.mMorfFrames, 1};
    daNpcF_anmPlayData dat1b = {ANM_SIT, daNpc_zrC_Param_c::m.mMorfFrames, 0};
    daNpcF_anmPlayData* pDat1[2] = {&dat1a, &dat1b};
    daNpcF_anmPlayData dat2 = {ANM_PRAY, daNpc_zrC_Param_c::m.mMorfFrames, 0};
    daNpcF_anmPlayData* pDat2[1] = {&dat2};
    daNpcF_anmPlayData dat3 = {ANM_SIT, daNpc_zrC_Param_c::m.mMorfFrames, 0};
    daNpcF_anmPlayData* pDat3[1] = {&dat3};
    daNpcF_anmPlayData dat4a = {ANM_SAD_TALK, daNpc_zrC_Param_c::m.mMorfFrames, 1};
    daNpcF_anmPlayData dat4b = {ANM_SAD_WAIT, 0.0f, 0};
    daNpcF_anmPlayData* pDat4[2] = {&dat4a, &dat4b};
    daNpcF_anmPlayData dat5 = {ANM_SAD_WAIT, daNpc_zrC_Param_c::m.mMorfFrames, 0};
    daNpcF_anmPlayData* pDat5[1] = {&dat5};
    daNpcF_anmPlayData dat6a = {ANM_TALK_A, daNpc_zrC_Param_c::m.mMorfFrames, 1};
    daNpcF_anmPlayData dat6b = {ANM_WAIT_A, 0.0f, 0};
    daNpcF_anmPlayData* pDat6[2] = {&dat6a, &dat6b};
    daNpcF_anmPlayData dat7a = {ANM_KIZUKU, daNpc_zrC_Param_c::m.mMorfFrames, 1};
    daNpcF_anmPlayData dat7b = {ANM_KIZUKU_WAIT, 0.0f, 0};
    daNpcF_anmPlayData* pDat7[2] = {&dat7a, &dat7b};
    daNpcF_anmPlayData dat8 = {ANM_KIZUKU_WAIT, daNpc_zrC_Param_c::m.mMorfFrames, 0};
    daNpcF_anmPlayData* pDat8[1] = {&dat8};
    daNpcF_anmPlayData dat9a = {ANM_SMILE_TALK, daNpc_zrC_Param_c::m.mMorfFrames, 1};
    daNpcF_anmPlayData dat9b = {ANM_WAIT_A, 0.0f, 0};
    daNpcF_anmPlayData* pDat9[2] = {&dat9a, &dat9b};
    daNpcF_anmPlayData dat10a = {ANM_TOPRAY, daNpc_zrC_Param_c::m.mMorfFrames, 1};
    daNpcF_anmPlayData dat10b = {ANM_PRAY, daNpc_zrC_Param_c::m.mMorfFrames, 0};
    daNpcF_anmPlayData* pDat10[2] = {&dat10a, &dat10b};
    daNpcF_anmPlayData dat11a = {ANM_GETUP, daNpc_zrC_Param_c::m.mMorfFrames, 1};
    daNpcF_anmPlayData dat11b = {ANM_WAIT_A, daNpc_zrC_Param_c::m.mMorfFrames, 0};
    daNpcF_anmPlayData* pDat11[2] = {&dat11a, &dat11b};
    daNpcF_anmPlayData dat12 = {ANM_STEP, 3.0f, 0};
    daNpcF_anmPlayData* pDat12[1] = {&dat12};
    daNpcF_anmPlayData dat13 = {ANM_SICK_DEMO_WAIT, daNpc_zrC_Param_c::m.mMorfFrames, 0};
    daNpcF_anmPlayData* pDat13[1] = {&dat13};
    daNpcF_anmPlayData** ppDat[14] = {
        pDat0, pDat1, pDat2, pDat3, pDat4, pDat5, pDat6, pDat7, pDat8, pDat9, pDat10, pDat11,
        pDat12, pDat13,
    };
    if (mMotion >= 0 && mMotion < 14) {
        playMotionAnm2(ppDat);
    }
}

void daNpc_zrC_c::playMotionAnm2(daNpcF_c::daNpcF_anmPlayData*** i_data) {
    f32 morf;
    daNpcF_anmPlayData* playData = NULL;

    if (i_data[mMotion] != NULL) {
        playData = i_data[mMotion][mMotionPhase];
    }

    if (playData != NULL) {
        if (mMotionPrevPhase == mMotionPhase && (int)playData->numLoops > 0 && playData->numLoops <= mMotionLoops) {
            mMotionPhase++;
            playData = i_data[mMotion][mMotionPhase];
        }

        if (playData != NULL && mMotionPrevPhase != mMotionPhase) {
            setMotionAnm(playData->idx, 0.0f);
            morf = playData->morf;

            if (mMotionPhase == 0 && 0.0f <= mMotionMorfOverride) {
                morf = mMotionMorfOverride;
            }

            if (morf > 0.0f) {
                mExpressionMorf = 0.0f;
                mAnm_p->setMorf(morf);
            }
        }
    }

    mMotionPrevPhase = mMotionPhase;
}

BOOL daNpc_zrC_c::chkAction(ActionFn i_action) {
    return mpActionFn == i_action;
}

BOOL daNpc_zrC_c::setAction(ActionFn i_action) {
    mMode = 3;
    if (mpActionFn != NULL) {
        (this->*mpActionFn)(NULL);
    }
    mMode = 0;
    mpActionFn = i_action;
    if (mpActionFn != NULL) {
        (this->*mpActionFn)(NULL);
    }
    return TRUE;
}

BOOL daNpc_zrC_c::selectAction() {
    mpNextActionFn = NULL;
    if (daNpc_zrC_Param_c::m.mTest) {
        mpNextActionFn = &daNpc_zrC_c::test;
    } else {
        switch (mType) {
        case 1:
            mpNextActionFn = &daNpc_zrC_c::waitSick;
            break;
        case 2:
            mpNextActionFn = &daNpc_zrC_c::waitPray;
            break;
        case 3:
            mpNextActionFn = &daNpc_zrC_c::waitThrone;
            break;
        default:
            mpNextActionFn = &daNpc_zrC_c::wait;
            break;
        }
    }
    return TRUE;
}

void daNpc_zrC_c::doNormalAction(BOOL param_0) {
    if (param_0 && hitChk2(&mCcCyl, TRUE, FALSE)) {
        int damage_timer;
        if (mCutType == daPy_py_c::CUT_TYPE_TURN_RIGHT) {
            damage_timer = 20;
        } else {
            damage_timer = daNpc_zrC_Param_c::m.mDamageTimer;
        }
        setDamage(damage_timer, EXPR_WAIT_A, MOT_WAIT_A);
        setLookMode(LOOK_RESET);
    } else if (mIsDamaged && mDamageTimer == 0) {
        mMode = 0;
        mIsDamaged = false;
    }

    mOrderEvtNo = EVT_NONE;
    if (mpNextActionFn != NULL) {
        if (mpActionFn == mpNextActionFn) {
            (this->*mpActionFn)(NULL);
        } else {
            setAction(mpNextActionFn);
        }
    }
}

BOOL daNpc_zrC_c::doEvent() {
    dEvent_manager_c* event_mgr = NULL;
    BOOL ret = 0;
    int staff_id;

    if (dComIfGp_event_runCheck()) {
        event_mgr = &dComIfGp_getEventManager();
        if ((eventInfo.checkCommandTalk() || eventInfo.checkCommandDemoAccrpt()) && !mSpeakEvent)
        {
            mOrderNewEvt = false;
        }

        if (eventInfo.checkCommandTalk()) {
            if (chkAction(&daNpc_zrC_c::talk)) {
                (this->*mpActionFn)(NULL);
            } else if (dComIfGp_event_chkTalkXY()) {
                if (dComIfGp_evmng_ChkPresentEnd()) {
                    if (mType == 2 && dComIfGp_event_getPreItemNo() == fpcNm_ITEM_ASHS_SCRIBBLING) {
                        mFlowID = 0x35;
                        mOrderEvtNo = EVT_EARRING_GET;
                        changeEvent(l_resNames[l_evtGetParamList[mOrderEvtNo].arcIdx],
                                    l_evtNames[l_evtGetParamList[mOrderEvtNo].fileIdx], 2, 0xffff);
                    } else {
                        s16 idx = dComIfGp_getEventManager().getEventIdx(this, "NO_RESPONSE", 0xff);
                        dComIfGp_getEvent()->reset(this);
                        fopAcM_orderChangeEventId(this, idx, 1, 0xffff);
                        field_0x9ec = true;
                    }
                }
            } else {
                setAction(&daNpc_zrC_c::talk);
            }
            ret = TRUE;

        } else {
            if (mItemID != -1) {
                dComIfGp_event_setItemPartnerId(mItemID);
                mItemID = -1;
            }

            staff_id = event_mgr->getMyStaffId(l_myName, NULL, 0);
            if (staff_id != -1) {
                mStaffID = staff_id;
                int evtCutNo = event_mgr->getMyActIdx(staff_id, mEvtCutNameList, 2, 0, 0);
                JUT_ASSERT(0x8ca, (0 <= evtCutNo) && (evtCutNo < NUM_EVT_CUTS_e));
                JUT_ASSERT(0x8cb, NULL != mEvtCutList[evtCutNo]);
                if ((this->*mEvtCutList[evtCutNo])(staff_id)) {
                    event_mgr->cutEnd(staff_id);
                }
                ret = TRUE;
            }

            if (eventInfo.checkCommandDemoAccrpt() && mEventIdx != -1
                                                     && event_mgr->endCheck(mEventIdx)) {
                dComIfGp_event_reset();
                mOrderEvtNo = EVT_NONE;
                mEventIdx = -1;
                mFlowID = home.angle.x;
            }
        }

        int expression, motion;
        int msg_timer = mMsgTimer;
        if (ctrlMsgAnm(expression, motion, this, FALSE)) {
            if (!field_0x9eb) {
                setExpression(expression, -1.0f);
                setMotion(motion, -1.0f, FALSE);
            }
        } else if (msg_timer != 0 && !field_0x9eb) {
            setExpressionTalkAfter();
        }

        switch (mExpression) {
        case EXPR_KIZUKU:
            mAnmFlags &= ~ANM_PAUSE_EXPRESSION;
            break;
        case EXPR_WAIT_A:
            break;
        default:
            if (mExpressionPhase != 0) {
                mAnmFlags &= ~ANM_PAUSE_EXPRESSION;
            }
            break;
        }

    } else {
        mMsgTimer = 0;
        if (mStaffID != -1) {
            mpActionFn = NULL;
            mStaffID = -1;
        }
    }

    return ret;
}

void daNpc_zrC_c::setLookMode(int i_lookMode) {
    if (i_lookMode >= 0 && i_lookMode < 6 && i_lookMode != mLookMode) {
        mLookMode = i_lookMode;
    }
}

void daNpc_zrC_c::lookat() {
    fopAc_ac_c* attn_actor = NULL;
    J3DModel* model = mAnm_p->getModel();
    BOOL snap = false;
    f32 body_down_angle = daNpc_zrC_Param_c::m.mBodyDownAngle;
    f32 body_up_angle = daNpc_zrC_Param_c::m.mBodyUpAngle;
    f32 body_right_angle = daNpc_zrC_Param_c::m.mBodyRightAngle;
    f32 body_left_angle = daNpc_zrC_Param_c::m.mBodyLeftAngle;
    f32 head_down_angle = daNpc_zrC_Param_c::m.mHeadDownAngle;
    f32 head_up_angle = daNpc_zrC_Param_c::m.mHeadUpAngle;
    f32 head_right_angle = daNpc_zrC_Param_c::m.mHeadRightAngle;
    f32 head_left_angle = daNpc_zrC_Param_c::m.mHeadLeftAngle;
    s16 angle_delta = mCurAngle.y - mOldAngle.y;
    cXyz lookat_pos[3] = {mLookatPos[0], mLookatPos[1], mLookatPos[2]};
    csXyz* lookat_angle[3] = {&mLookatAngle[0], &mLookatAngle[1], &mLookatAngle[2]};

    switch (mLookMode) {
    case LOOK_NONE:
        break;
    case LOOK_RESET:
        snap = true;
        break;
    case LOOK_PLAYER:
    case LOOK_PLAYER_TALK:
        attn_actor = daPy_getPlayerActorClass();
        if (mLookMode == LOOK_PLAYER_TALK) {
            head_right_angle = -80.0f;
            head_left_angle = 80.0f;
        }
        break;
    case LOOK_ACTOR:
        attn_actor = mActorMngr[1].getActorP();
        break;
    case LOOK_ATTN:
        attn_actor = mActorMngr[2].getActorP();
        break;
    }

    if (attn_actor != NULL) {
        mLookPos = attn_actor->attention_info.position;
        if (mLookMode == LOOK_ATTN) {
            mLookPos.y += 400.0f;
        } else if (mLookMode != LOOK_PLAYER && mLookMode != LOOK_PLAYER_TALK
                                            && mLookMode != LOOK_ACTOR) {
            mLookPos.y -= 40.0f;
        }
        mLookat.setAttnPos(&mLookPos);
    } else {
        mLookat.setAttnPos(NULL);
    }

    mLookat.setParam(body_down_angle, body_up_angle, body_right_angle, body_left_angle,
                     0.0f, 0.0f, 0.0f, 0.0f,
                     head_down_angle, head_up_angle, head_right_angle, head_left_angle,
                     mCurAngle.y, lookat_pos);
    mLookat.calc(this, model->getBaseTRMtx(), lookat_angle, snap, angle_delta, false);
}

void daNpc_zrC_c::setExpressionTalkAfter() {
    switch (mExpression) {
    case EXPR_SIT_TALK:
        setExpression(EXPR_SIT, -1.0f);
        break;
    case EXPR_PRAY:
        break;
    case EXPR_SAD:
    case EXPR_SAD_WAIT:
        setExpression(EXPR_H_SAD, -1.0f);
        break;
    case EXPR_SMILE:
        setExpression(EXPR_H_SMILE, -1.0f);
        break;
    case EXPR_KIZUKU:
    case EXPR_KIZUKU_WAIT:
        setExpression(EXPR_H_KIZUKU, -1.0f);
        break;
    default:
        setExpression(EXPR_WAIT_A, -1.0f);
        break;
    }
}

BOOL daNpc_zrC_c::wait(void* param_0) {
    switch (mMode) {
    case 0:
        setExpression(EXPR_WAIT_A, -1.0f);
        setMotion(MOT_WAIT_A, -1.0f, FALSE);
        setLookMode(LOOK_NONE);
        mTurnMode = 0;
        mMode = 2;
        // fallthrough

    case 2: {
        if (mIsDamaged) {
            break;
        }

        BOOL player_attn = mActorMngr[0].getActorP() != NULL;
        if (chkFindPlayer2(player_attn, shape_angle.y)) {
            if (!player_attn) {
                mActorMngr[0].entry(daPy_getPlayerActorClass());
                mTurnMode = 0;
            }
        } else {
            if (player_attn) {
                mActorMngr[0].remove();
                mTurnMode = 0;
            }
        }

        if (mActorMngr[0].getActorP() != NULL) {
            setLookMode(LOOK_PLAYER);
        } else {
            setLookMode(LOOK_NONE);
            if (home.angle.y != mCurAngle.y && step(home.angle.y, EXPR_STEP, MOT_STEP, 15)) {
                mMode = 0;
            }
        }

        if (home.angle.y == mCurAngle.y) {
            BOOL player_attn = mActorMngr[0].getActorP() != NULL;
            fopAc_ac_c* attn_actor = getAttnActorP(
                player_attn, srchAttnActor1, daNpc_zrC_Param_c::m.mAttnRadius,
                daNpc_zrC_Param_c::m.mAttnUpperY, daNpc_zrC_Param_c::m.mAttnLowerY,
                daNpc_zrC_Param_c::m.mAttnFovY, shape_angle.y, 120, TRUE);
            if (attn_actor != NULL) {
                mActorMngr[1].entry(attn_actor);
                setLookMode(LOOK_ACTOR);
            }
        } else {
            mAttnChangeTimer = 0;
        }
        break;
    }

    case 3:
        break;
    }

    return TRUE;
}

BOOL daNpc_zrC_c::waitSick(void* param_0) {
    switch (mMode) {
    case 0:
        setExpression(EXPR_SICK_DEMO_WAIT, -1.0f);
        setMotion(MOT_SICK_DEMO_WAIT, -1.0f, FALSE);
        setLookMode(LOOK_NONE);
        mTurnMode = 0;
        mAcch.SetGrndNone();
        mAcch.SetWallNone();
        mMode = 2;
        // fallthrough

    case 2:
        if (daPy_py_c::checkNowWolf() && daPy_py_c::checkNowWolfEyeUp()) {
            mCreatureSound.startCreatureVoiceLevel(Z2SE_ZRC_V_SLEEPING, -1);
        }
        break;

    case 3:
        break;
    }

    return TRUE;
}

BOOL daNpc_zrC_c::waitPray(void* param_0) {
    f32 player_dist = (daPy_getPlayerActorClass()->current.pos - current.pos).absXZ();

    switch (mMode) {
    case 0:
        if (!field_0xe30) {
            if (mMotionAnm != ANM_TOPRAY && mMotionAnm != ANM_PRAY) {
                setExpression(EXPR_TOPRAY, -1.0f);
                setMotion(MOT_TOPRAY, -1.0f, FALSE);
            }
        } else {
            setExpression(EXPR_WAIT_A, -1.0f);
            setMotion(MOT_WAIT_A, -1.0f, FALSE);
        }
        mTurnMode = 0;
        mMode = 2;
        // fallthrough

    case 2:
        if (!daNpcF_chkEvtBit(0x1df) && field_0xe30 && !daPy_py_c::checkNowWolf()
            && player_dist <= daNpc_zrC_Param_c::m.field_0x6c
            && !daPy_getPlayerActorClass()->checkPlayerFly()
            && daPy_getPlayerActorClass()->checkSwimUp() && !dComIfGp_checkPlayerStatus0(0, 0x100))
        {
            cXyz pos = daPy_getPlayerActorClass()->current.pos;
            pos.y += 100.0f;
            if (!fopAcM_lc_c::lineCheck(&mHeadPos, &pos, this)) {
                mSpeakEvent = true;
                mFlowID = 0x34;
                break;
            }
        }

        if (mIsDamaged) {
            break;
        }

        if (field_0xe30 == true) {
            if (player_dist > daNpc_zrC_Param_c::m.field_0x70) {
                setLookMode(LOOK_NONE);
                if (home.angle.y != mCurAngle.y) {
                    if (step(home.angle.y, EXPR_TOPRAY, MOT_TOPRAY, 30)) {
                        mMode = 0;
                    }
                } else if (mMotionAnm != ANM_TOPRAY && mMotionAnm != ANM_PRAY) {
                    setExpression(EXPR_TOPRAY, -1.0f);
                    setMotion(MOT_TOPRAY, -1.0f, FALSE);
                }
                mTurnMode = 0;
                field_0xe30 = false;
            }
        } else if (mLookMode == LOOK_PLAYER || mLookMode == LOOK_PLAYER_TALK) {
            if (player_dist > daNpc_zrC_Param_c::m.field_0x70) {
                setLookMode(LOOK_NONE);
                setMotion(MOT_PRAY, -1.0f, FALSE);
                setExpression(EXPR_PRAY, -1.0f);
            }
        } else if (player_dist > daNpc_zrC_Param_c::m.field_0x6c) {
            setLookMode(LOOK_NONE);
            if (home.angle.y != mCurAngle.y) {
                if (step(home.angle.y, -1, -1, 30)) {
                    mMode = 0;
                }
            } else if (mMotionAnm != ANM_TOPRAY && mMotionAnm != ANM_PRAY) {
                setExpression(EXPR_TOPRAY, -1.0f);
                setMotion(MOT_TOPRAY, -1.0f, FALSE);
            }
        } else {
            mTurnMode = 0;
            setLookMode(LOOK_PLAYER);
            setExpression(EXPR_WAIT_A, -1.0f);
        }

        if ((mLookMode == LOOK_PLAYER || mLookMode == LOOK_PLAYER_TALK)) {
            BOOL find_player = chkFindPlayer2(FALSE, shape_angle.y);
            if (find_player || field_0xe30) {
                s16 player_angle = fopAcM_searchPlayerAngleY(this);
                if (find_player && !field_0xe30) {
                    setExpression(EXPR_GETUP, -1.0f);
                    setMotion(MOT_GETUP, -1.0f, FALSE);
                    field_0xe30 = true;
                    if (player_angle != mCurAngle.y && step(player_angle, -1, -1, 30)) {
                        setExpression(EXPR_WAIT_A, -1.0f);
                        setMotion(MOT_WAIT_A, -1.0f, FALSE);
                        mMode = 0;
                    }
                } else if (mTurnMode != 0) {
                    int step_rate = 15;
                    if (mMotionAnm == ANM_GETUP) {
                        step_rate = 30;
                    }
                    if (step(player_angle, -1, -1, step_rate)) {
                        setExpression(EXPR_WAIT_A, -1.0f);
                        setMotion(MOT_WAIT_A, -1.0f, FALSE);
                        mMode = 0;
                    }
                } else if (field_0xe30) {
                    s16 angle = cM_deg2s(daNpc_zrC_Param_c::m.mHeadLeftAngle);
                    if (abs((s16)(player_angle - current.angle.y)) > angle
                        && step(player_angle, EXPR_WAIT_A, MOT_STEP, 15))
                    {
                        setExpression(EXPR_WAIT_A, -1.0f);
                        setMotion(MOT_WAIT_A, -1.0f, FALSE);
                        mMode = 0;
                    }
                }
            }
        }
        break;

    case 3:
        break;
    }

    return TRUE;
}

BOOL daNpc_zrC_c::waitThrone(void* param_0) {
    switch (mMode) {
    case 0:
        setExpression(EXPR_SIT, -1.0f);
        setMotion(MOT_SIT, -1.0f, FALSE);
        setLookMode(LOOK_NONE);
        mTurnMode = 0;
        mAcch.SetGrndNone();
        mAcch.SetWallNone();
        field_0x9ea = true;
        mMode = 2;
        // fallthrough

    case 2: {
        if (mIsDamaged) {
            break;
        }

        BOOL player_attn = mActorMngr[0].getActorP() != NULL;
        if (chkFindPlayer2(player_attn, shape_angle.y)) {
            if (!player_attn) {
                mActorMngr[0].entry(daPy_getPlayerActorClass());
                mTurnMode = 0;
            }
        } else {
            if (player_attn) {
                mActorMngr[0].remove();
                mTurnMode = 0;
            }
        }

        if (mActorMngr[0].getActorP() != NULL) {
            setLookMode(LOOK_PLAYER);
        } else {
            setLookMode(LOOK_NONE);
        }
        break;
    }

    case 3:
        break;
    }

    return TRUE;
}

BOOL daNpc_zrC_c::talk(void* param_0) {
    BOOL ret = FALSE;
    BOOL talk = FALSE;

    switch (mMode) {
    case 0:
        if (mIsDamaged) {
            break;
        }

        mSpeakEvent = false;
        initTalk(mFlowID, NULL);
        mTurnMode = 0;
        mMode = 2;
        // fallthrough

    case 2:
        if (field_0x9ea) {
            talk = true;
            if (mType == 3) {
                setLookMode(LOOK_PLAYER_TALK);
                mActorMngr[0].entry(daPy_getPlayerActorClass());
            }
        } else {
            setLookMode(LOOK_PLAYER_TALK);
            mActorMngr[0].entry(daPy_getPlayerActorClass());
            if (mCurAngle.y == fopAcM_searchPlayerAngleY(this)) {
                talk = TRUE;
            } else if (step(fopAcM_searchPlayerAngleY(this), EXPR_STEP, MOT_STEP, 15)) {
                setExpression(EXPR_WAIT_A, -1.0f);
                setMotion(MOT_WAIT_A, -1.0f, FALSE);
                mTurnMode = 0;
            }
        }
        
        if (talk && talkProc(NULL, TRUE, NULL)) {
            ret = TRUE;
            mFlowID = home.angle.x;
        }

        if (ret) {
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

    return ret;
}

BOOL daNpc_zrC_c::test(void* param_0) {
    switch (mMode) {
    case 0:
        speedF = 0.0f;
        speed.setall(0.0f);
        mMode = 2;
        // fallthrough

    case 2:
        if (daNpc_zrC_Param_c::m.mTestExpression != mExpression) {
            setExpression(daNpc_zrC_Param_c::m.mTestExpression, daNpc_zrC_Param_c::m.mMorfFrames);
        }
        setMotion(daNpc_zrC_Param_c::m.mTestMotion, daNpc_zrC_Param_c::m.mMorfFrames, FALSE);
        setLookMode(daNpc_zrC_Param_c::m.mTestLookMode);
        mOrderEvtNo = EVT_NONE;
        attention_info.flags = 0;
        break;

    case 3:
        break;
    }

    return TRUE;
}

BOOL daNpc_zrC_c::ECut_earringGet(int i_staffID) {
    dEvent_manager_c& event_mgr = dComIfGp_getEventManager();
    BOOL ret = FALSE;

    int prm = -1;
    int* prm_p = dComIfGp_evmng_getMyIntegerP(i_staffID, "prm");
    if (prm_p != NULL) {
        prm = *prm_p;
    }

    if (event_mgr.getIsAddvance(i_staffID)) {
        switch (prm) {
        case 0:
            setLookMode(LOOK_PLAYER);
            daNpcF_offTmpBit(0xb);
            daNpcF_offTmpBit(0xc);
            daNpcF_offTmpBit(0xd);
            daNpcF_offTmpBit(0xe);
            daNpcF_offTmpBit(0xf);
            break;

        case 10:
            initTalk(mFlowID, NULL);
            break;

        case 20: {
            Z2GetAudioMgr()->subBgmStart(Z2BGM_LUTERA1);
            cXyz pos(21070.344f, 700.0f, 180.55788f);
            if (fopAcM_gc_c::gndCheck(&pos)) {
                pos.y = fopAcM_gc_c::getGroundY();
            }
            s16 angle = cLib_targetAngleY(&pos, &current.pos);
            daPy_getPlayerActorClass()->setPlayerPosAndAngle(&pos, angle, 0);
            setAngle(angle + 0x8000);
            break;
        }

        case 30:
            initTalk(mFlowID, NULL);
            break;

        case 40: {
            int item_no = 0;
            if (mFlow.getEventId(&item_no) == 1) {
                mItemID = fopAcM_createItemForPresentDemo(&current.pos, item_no,
                                                          0, -1, -1, NULL, NULL);
            }
            break;
        }

        case 50:
            initTalk(mFlowID, NULL);
            break;

        case 60:
            initTalk(mFlowID, NULL);
            break;

        case 70:
            initTalk(mFlowID, NULL);
            break;

        case 80:
            setLookMode(LOOK_PLAYER);
            initTalk(mFlowID, NULL);
            break;

        case 90:
            setLookMode(LOOK_PLAYER);
            setExpression(EXPR_WAIT_A, -1.0f);
            setMotion(MOT_WAIT_A, -1.0f, FALSE);
            Z2GetAudioMgr()->subBgmStop();
            break;
        }
    }

    switch (prm) {
    case 0:
        if (mCurAngle.y != fopAcM_searchPlayerAngleY(this)) {
            if (step(fopAcM_searchPlayerAngleY(this), -1, -1, 30)) {
                setExpression(EXPR_WAIT_A, -1.0f);
                setMotion(MOT_WAIT_A, -1.0f, FALSE);
                mTurnMode = 0;
                ret = TRUE;
            }
        } else {
            ret = TRUE;
        }
        break;

    case 10:
        if (talkProc(NULL, TRUE, NULL)) {
            ret = TRUE;
        }
        break;

    case 20:
        ret = TRUE;
        break;

    case 30:
        if (talkProc(NULL, TRUE, NULL)) {
            ret = TRUE;
        }
        break;

    case 40:
        field_0x9eb = true;
        ret = TRUE;
        break;

    case 50:
    case 60:
    case 70:
    case 80:
        if (talkProc(NULL, TRUE, NULL)) {
            ret = TRUE;
        }
        break;

    case 90:
        ret = TRUE;
        break;

    default:
        ret = TRUE;
        break;
    }

    return ret;
}

static cPhs__Step daNpc_zrC_Create(void* i_this) {
    return static_cast<daNpc_zrC_c*>(i_this)->create();
}

static int daNpc_zrC_Delete(void* i_this) {
    return static_cast<daNpc_zrC_c*>(i_this)->Delete();
}

static int daNpc_zrC_Execute(void* i_this) {
    return static_cast<daNpc_zrC_c*>(i_this)->Execute();
}

static int daNpc_zrC_Draw(void* i_this) {
    return static_cast<daNpc_zrC_c*>(i_this)->Draw();
}

static int daNpc_zrC_IsDelete(void* i_this) {
    return 1;
}

AUDIO_INSTANCES

static actor_method_class daNpc_zrC_MethodTable = {
    (process_method_func)daNpc_zrC_Create,
    (process_method_func)daNpc_zrC_Delete,
    (process_method_func)daNpc_zrC_Execute,
    (process_method_func)daNpc_zrC_IsDelete,
    (process_method_func)daNpc_zrC_Draw,
};

actor_process_profile_definition g_profile_NPC_ZRC = {
  fpcLy_CURRENT_e,        // mLayerID
  7,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_NPC_ZRC,           // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(daNpc_zrC_c),    // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  389,                    // mPriority
  &daNpc_zrC_MethodTable, // sub_method
  0x00044100,             // mStatus
  fopAc_NPC_e,            // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};
