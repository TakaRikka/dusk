/**
 * d_a_npc_the.cpp
 * NPC - Telma
 */

#include "d/dolzel_rel.h"  // IWYU pragma: keep

#include "d/actor/d_a_npc_the.h"
#include "d/actor/d_a_npc.h"
#include "d/actor/d_a_tag_evtarea.h"
#include "JSystem/J3DGraphBase/J3DMaterial.h"
#include "SSystem/SComponent/c_math.h"

const daNpcThe_HIOParam daNpcThe_Param_c::m = {
    220.0f,   // attention_offset
    -3.0f,    // gravity
    1.0f,     // scale
    400.0f,   // real_shadow_size
    255.0f,   // weight
    200.0f,   // height
    35.0f,    // knee_length
    40.0f,    // width
    0.0f,     // body_angleX_max
    -20.0f,   // body_angleX_min
    10.0f,    // body_angleY_max
    -10.0f,   // body_angleY_min
    30.0f,    // head_angleX_max
    -20.0f,   // head_angleX_min
    45.0f,    // head_angleY_max
    -45.0f,   // head_angleY_min
    0.6f,     // neck_rotation_ratio
    12.0f,    // morf_frame
    3,        // talk_distance
    6,        // talk_angle
    5,        // attention_distance
    6,        // attention_angle
    80.0f,    // fov
    500.0f,   // search_distance
    300.0f,   // search_height
    -300.0f,  // search_depth
    60,       // attention_time
    8,        // damage_time
    0,        // face_expression
    0,        // motion
    0,        // look_mode
    0,        // debug_mode_ON
    0,        // debug_info_ON
};

#if DEBUG
daNpcThe_HIO_c::daNpcThe_HIO_c() {
    m = daNpcThe_Param_c::m;
}

void daNpcThe_HIO_c::listenPropertyEvent(const JORPropertyEvent* event) {
    // TODO
}

void daNpcThe_HIO_c::genMessage(JORMContext* ctext) {
    // TODO
}
#endif

NPC_THE_HIO_CLASS l_HIO;

static daNpc_GetParam1 l_bmdGetParamList[2] = {
    {23, 0},  // the
    {24, 0},  // the_tw
};

static daNpc_GetParam1 l_bckGetParamList[27] = {
    {-1, 0},  // <none>
    {10, 0},  // the_f_talk_a
    {9, 0},   // the_f_smile
    {7, 0},   // the_f_chuckle
    {11, 0},  // the_f_talk_b
    {12, 0},  // the_f_talk_c
    {8, 0},   // the_f_laugh
    {13, 0},  // the_f_talk_r
    {16, 0},  // the_fh_smile
    {14, 0},  // the_fh_chuckle
    {17, 0},  // the_fh_talk_b
    {18, 0},  // the_fh_talk_c
    {15, 0},  // the_fh_laugh
    {19, 0},  // the_fh_talk_r
    {20, 0},  // the_wait_a
    {9, 1},   // the_wait_looking
    {7, 1},   // the_to_wait_b
    {8, 1},   // the_wait_b
    {10, 1},  // the_wait_lookup
    {5, 1},   // the_talk_r
    {6, 1},   // the_talking_r
    {4, 1},   // the_look_a
    {3, 1},   // the_look
    {5, 2},   // the_kune_wait_a
    {4, 2},   // the_kune_talk
    {6, 2},   // the_kune_wait_b
    {3, 2},   // the_kune_shishi
};

static daNpc_GetParam1 l_btpGetParamList[14] = {
    {30, 0},  // the
    {34, 0},  // the_f_talk_a
    {33, 0},  // the_f_smile
    {31, 0},  // the_f_chuckle
    {35, 0},  // the_f_talk_b
    {36, 0},  // the_f_talk_c
    {32, 0},  // the_f_laugh
    {37, 0},  // the_f_talk_r
    {40, 0},  // the_fh_smile
    {38, 0},  // the_fh_chuckle
    {41, 0},  // the_fh_talk_b
    {42, 0},  // the_fh_talk_c
    {39, 0},  // the_fh_laugh
    {43, 0},  // the_fh_talk_r
};

static daNpc_GetParam1 l_btkGetParamList[1] = {
    {27, 0},  // the
};

static int l_loadRes_THE0[3] = {0, 1, -1};

static int l_loadRes_THE1[3] = {0, 1, -1};

static int l_loadRes_THE2[3] = {0, 2, -1};

static int* l_loadRes_list[3] = {
    l_loadRes_THE0,
    l_loadRes_THE1,
    l_loadRes_THE2,
};

static char* l_resNames[4] = {
    "The",
    "The1",
    "The2",
    "yelB_TW",
};

static int l_evtGetParamList[4] = {
    0,
    3,
    3,
    0,
};

static char* l_evtNames[4] = {
    NULL,
    "TW_RESISTANCE1",
    "TW_RESISTANCE2",
    "THE_INTRODUCTION",
};

static char* l_myName = "The";

char* daNpcThe_c::mEvtCutNameList[4] = {
    "",
    "TW_RESISTANCE",
    "TW_RESISTANCE",
    "THE_INTRODUCTION",
};

daNpcThe_c::EventFn daNpcThe_c::mEvtCutList[4] = {
    NULL,
    &daNpcThe_c::EvCut_TwResistance,
    &daNpcThe_c::EvCut_TwResistance,
    &daNpcThe_c::EvCut_Introduction,
};

int daNpcThe_c::Draw() {
    mAnm_p->getModel()->getModelData()->getMaterialNodePointer(2)->setMaterialAnm(mpMatAnm);
    ActionFn action_test = &daNpcThe_c::test;
    int is_test = mpActionFn == action_test;
    return draw(is_test, false, mpHIO->m.common.real_shadow_size, NULL, false);
}

daNpcThe_c::daNpcThe_c() {}

daNpcThe_c::~daNpcThe_c() {
    for (int i = 0; l_loadRes_list[mType][i] >= 0; i++) {
        dComIfG_resDelete(&mPhase[i], l_resNames[l_loadRes_list[mType][i]]);
    }
    if (heap != NULL) {
        mAnm_p->stopZelAnime();
    }
}

int daNpcThe_c::CreateHeap() {
    J3DModelData* model_data = NULL;
    mTwilight = dKy_darkworld_check();
    int bmd_get_idx = mTwilight ? 1 : 0;
    if (l_bmdGetParamList[bmd_get_idx].fileIdx >= 0) {
        model_data = static_cast<J3DModelData*>(
            dComIfG_getObjectRes(l_resNames[l_bmdGetParamList[bmd_get_idx].arcIdx],
                                 l_bmdGetParamList[bmd_get_idx].fileIdx));
    }
    mAnm_p = new mDoExt_McaMorfSO(model_data, NULL, NULL, NULL, -1, 1.0f, 0, -1, &mSound, 0x80000,
                                  0x11020284);

    if (mAnm_p != NULL && mAnm_p->getModel() == NULL) {
        mAnm_p->stopZelAnime();
        mAnm_p = NULL;
    }
    if (mAnm_p == NULL) {
        return 0;
    }

    J3DModel* model = mAnm_p->getModel();
    for (u16 i = 0; i < model_data->getJointTree().getJointNum(); i++) {
        model_data->getJointNodePointer(i)->setCallBack(ctrlJointCallBack);
    }
    model->setUserArea((uintptr_t)this);

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

int daNpcThe_c::ctrlJoint(J3DJoint* i_joint, J3DModel* i_model) {
    int joint_no = i_joint->getJntNo();
    int lookat_joints[3] = {1, 3, 4};

    if (joint_no == 0) {
        mDoMtx_stack_c::copy(mAnm_p->getModel()->getAnmMtx(1));
        mDoMtx_stack_c::multVecZero(&mLookatPos[0]);
        mDoMtx_stack_c::copy(mAnm_p->getModel()->getAnmMtx(3));
        mDoMtx_stack_c::multVecZero(&mLookatPos[1]);
        mDoMtx_stack_c::copy(mAnm_p->getModel()->getAnmMtx(4));
        mDoMtx_stack_c::multVecZero(&mLookatPos[2]);
    }

    mDoMtx_stack_c::copy(i_model->getAnmMtx(joint_no));

    switch (joint_no) {
    case 1:
    case 3:
    case 4:
        setLookatMtx(joint_no, lookat_joints, mpHIO->m.common.neck_rotation_ratio);
        break;
    }

    i_model->setAnmMtx(joint_no, mDoMtx_stack_c::get());
    MTXCopy(mDoMtx_stack_c::get(), J3DSys::mCurrentMtx);

    if ((joint_no == 4 || joint_no == 11) && (mAnmFlags & 0x100)) {
        J3DAnmTransform* anm = mBckAnm.getBckAnm();
        mBckAnm.changeBckOnly(mAnm_p->getAnm());
        mAnm_p->changeAnm(anm);
    }

    return 1;
}

int daNpcThe_c::createHeapCallBack(fopAc_ac_c* i_this) {
    return static_cast<daNpcThe_c*>(i_this)->CreateHeap();
}

int daNpcThe_c::ctrlJointCallBack(J3DJoint* i_joint, int param_1) {
    if (param_1 == 0) {
        daNpcThe_c* _this = (daNpcThe_c*)j3dSys.getModel()->getUserArea();
        if (_this != NULL) {
            _this->ctrlJoint(i_joint, j3dSys.getModel());
        }
    }
    return true;
}

bool daNpcThe_c::setExpressionAnm(int i_idx, bool i_modify) {
    J3DAnmTransform* anm = NULL;
    int attr = J3DFrameCtrl::EMode_NONE;
    mAnmFlags &= ~ANM_EXPRESSION_FLAGS;

    if (l_bckGetParamList[i_idx].fileIdx >= 0) {
        anm = getTrnsfrmKeyAnmP(l_resNames[l_bckGetParamList[i_idx].arcIdx],
                                l_bckGetParamList[i_idx].fileIdx);
    }

    bool res = false;
    switch (i_idx) {
    case ANM_NONE:
        res = setExpressionBtp(0);
        break;
    case ANM_F_TALK_A:
        res = setExpressionBtp(1);
        break;
    case ANM_F_SMILE:
        res = setExpressionBtp(2);
        break;
    case ANM_F_CHUCKLE:
        res = setExpressionBtp(3);
        break;
    case ANM_F_TALK_B:
        res = setExpressionBtp(4);
        attr = J3DFrameCtrl::EMode_LOOP;
        break;
    case ANM_F_TALK_C:
        res = setExpressionBtp(5);
        break;
    case ANM_F_LAUGH:
        res = setExpressionBtp(6);
        break;
    case ANM_F_TALK_R:
        res = setExpressionBtp(7);
        break;
    case ANM_FH_SMILE:
        res = setExpressionBtp(8);
        attr = J3DFrameCtrl::EMode_LOOP;
        break;
    case ANM_FH_CHUCKLE:
        res = setExpressionBtp(9);
        attr = J3DFrameCtrl::EMode_LOOP;
        break;
    case ANM_FH_TALK_B:
        res = setExpressionBtp(10);
        attr = J3DFrameCtrl::EMode_LOOP;
        break;
    case ANM_FH_TALK_C:
        res = setExpressionBtp(11);
        attr = J3DFrameCtrl::EMode_LOOP;
        break;
    case ANM_FH_LAUGH:
        res = setExpressionBtp(12);
        attr = J3DFrameCtrl::EMode_LOOP;
        break;
    case ANM_FH_TALK_R:
        res = setExpressionBtp(13);
        attr = J3DFrameCtrl::EMode_LOOP;
        break;
    default:
        anm = NULL;
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

    return false;
}

bool daNpcThe_c::setExpressionBtp(int i_idx) {
    J3DAnmTexPattern* anm = NULL;
    mAnmFlags &= ~(ANM_PAUSE_BTP | ANM_PLAY_BTP | ANM_FLAG_800);

    if (l_btpGetParamList[i_idx].fileIdx >= 0) {
        anm = getTexPtrnAnmP(l_resNames[l_btpGetParamList[i_idx].arcIdx],
                             l_btpGetParamList[i_idx].fileIdx);
    }

    int attr = J3DFrameCtrl::EMode_NONE;
    switch (i_idx) {
    case 0:
        attr = J3DFrameCtrl::EMode_LOOP;
        break;
    case 1:
        break;
    case 2:
        break;
    case 3:
        break;
    case 4:
        attr = J3DFrameCtrl::EMode_LOOP;
        break;
    case 5:
        break;
    case 6:
        break;
    case 7:
        break;
    case 8:
        attr = J3DFrameCtrl::EMode_LOOP;
        break;
    case 9:
        attr = J3DFrameCtrl::EMode_LOOP;
        break;
    case 10:
        attr = J3DFrameCtrl::EMode_LOOP;
        break;
    case 11:
        attr = J3DFrameCtrl::EMode_LOOP;
        break;
    case 12:
        attr = J3DFrameCtrl::EMode_LOOP;
        break;
    case 13:
        attr = J3DFrameCtrl::EMode_LOOP;
        break;
    default:
        anm = NULL;
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

    return false;
}

void daNpcThe_c::setMotionAnm(int i_idx, f32 i_morf) {
    J3DAnmTransformKey* anm_trans = NULL;
    J3DAnmTextureSRTKey* anm_tex = NULL;
    int attr = J3DFrameCtrl::EMode_LOOP;
    mAnmFlags &= ~ANM_MOTION_FLAGS;

    if (l_bckGetParamList[i_idx].fileIdx >= 0) {
        anm_trans = getTrnsfrmKeyAnmP(l_resNames[l_bckGetParamList[i_idx].arcIdx],
                                      l_bckGetParamList[i_idx].fileIdx);
    }

    switch (i_idx) {
    case ANM_WAIT_A:
        attr = J3DFrameCtrl::EMode_LOOP;
        break;
    case ANM_WAIT_LOOKING:
        attr = J3DFrameCtrl::EMode_LOOP;
        break;
    case ANM_TO_WAIT_B:
        attr = J3DFrameCtrl::EMode_NONE;
        break;
    case ANM_WAIT_B:
        attr = J3DFrameCtrl::EMode_LOOP;
        break;
    case ANM_WAIT_LOOKUP:
        attr = J3DFrameCtrl::EMode_LOOP;
        break;
    case ANM_TALK_R:
        attr = J3DFrameCtrl::EMode_NONE;
        break;
    case ANM_TALKING_R:
        attr = J3DFrameCtrl::EMode_LOOP;
        break;
    case ANM_LOOK_A:
        attr = J3DFrameCtrl::EMode_NONE;
        break;
    case ANM_LOOK:
        attr = J3DFrameCtrl::EMode_NONE;
        break;
    case ANM_KUNE_WAIT_A:
        attr = J3DFrameCtrl::EMode_LOOP;
        break;
    case ANM_KUNE_TALK:
        attr = J3DFrameCtrl::EMode_NONE;
        break;
    case ANM_KUNE_WAIT_B:
        attr = J3DFrameCtrl::EMode_LOOP;
        break;
    case ANM_KUNE_SHISHI:
        attr = J3DFrameCtrl::EMode_NONE;
        break;
    default:
        anm_trans = NULL;
        anm_tex = NULL;
    }

    if (l_btkGetParamList[0].fileIdx >= 0) {
        anm_tex =
            getTexSRTKeyAnmP(l_resNames[l_btkGetParamList[0].arcIdx], l_btkGetParamList[0].fileIdx);
    }

    if (anm_tex != NULL) {
        if (setBtkAnm(anm_tex, mAnm_p->getModel()->getModelData(), 1.0f, 2)) {
            mAnmFlags |= ANM_PAUSE_BTK | ANM_PLAY_BTK;
        }
    }

    if (anm_trans != NULL) {
        if (setMcaMorfAnm(anm_trans, 1.0f, i_morf, attr, 0, -1)) {
            mAnmFlags |= ANM_PAUSE_MORF | ANM_PLAY_MORF;
            mMotionLoops = 0;
        }
    }
}

void daNpcThe_c::playExpression() {
    daNpcF_anmPlayData dat0 = {ANM_F_TALK_A, mpHIO->m.common.morf_frame, 1};
    daNpcF_anmPlayData* pDat0[2] = {&dat0, NULL};
    daNpcF_anmPlayData dat1 = {ANM_F_SMILE, mpHIO->m.common.morf_frame, 1};
    daNpcF_anmPlayData* pDat1[2] = {&dat1, NULL};
    daNpcF_anmPlayData dat2 = {ANM_F_CHUCKLE, mpHIO->m.common.morf_frame, 1};
    daNpcF_anmPlayData* pDat2[2] = {&dat2, NULL};
    daNpcF_anmPlayData dat3 = {ANM_F_TALK_B, mpHIO->m.common.morf_frame, 1};
    daNpcF_anmPlayData* pDat3[2] = {&dat3, NULL};
    daNpcF_anmPlayData dat4 = {ANM_F_TALK_C, mpHIO->m.common.morf_frame, 1};
    daNpcF_anmPlayData* pDat4[2] = {&dat4, NULL};
    daNpcF_anmPlayData dat5 = {ANM_F_LAUGH, mpHIO->m.common.morf_frame, 1};
    daNpcF_anmPlayData* pDat5[2] = {&dat5, NULL};
    daNpcF_anmPlayData dat6 = {ANM_F_TALK_R, mpHIO->m.common.morf_frame, 1};
    daNpcF_anmPlayData* pDat6[2] = {&dat6, NULL};
    daNpcF_anmPlayData dat7 = {ANM_FH_TALK_R, 0.0f, 0};
    daNpcF_anmPlayData* pDat7[2] = {&dat7, NULL};
    daNpcF_anmPlayData dat8 = {ANM_FH_SMILE, 0.0f, 0};
    daNpcF_anmPlayData* pDat8[2] = {&dat8, NULL};
    daNpcF_anmPlayData dat9 = {ANM_FH_CHUCKLE, 0.0f, 0};
    daNpcF_anmPlayData* pDat9[2] = {&dat9, NULL};
    daNpcF_anmPlayData dat10 = {ANM_FH_TALK_B, 0.0f, 0};
    daNpcF_anmPlayData* pDat10[2] = {&dat10, NULL};
    daNpcF_anmPlayData dat11 = {ANM_FH_TALK_C, 0.0f, 0};
    daNpcF_anmPlayData* pDat11[2] = {&dat11, NULL};
    daNpcF_anmPlayData dat12 = {ANM_FH_LAUGH, 0.0f, 0};
    daNpcF_anmPlayData* pDat12[2] = {&dat12, NULL};
    daNpcF_anmPlayData dat13 = {ANM_NONE, mpHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat13[1] = {&dat13};
    daNpcF_anmPlayData** ppDat[14] = {
        pDat0, pDat1, pDat2, pDat3,  pDat4,  pDat5,  pDat6,
        pDat7, pDat8, pDat9, pDat10, pDat11, pDat12, pDat13,
    };
    if (mExpression >= 0 && mExpression < 14) {
        playExpressionAnm(ppDat);
    }
}

void daNpcThe_c::playMotion() {
    daNpcF_anmPlayData dat0 = {ANM_WAIT_A, mpHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat0[1] = {&dat0};
    daNpcF_anmPlayData dat1a = {ANM_LOOK_A, mpHIO->m.common.morf_frame, 1};
    daNpcF_anmPlayData dat1b = {ANM_WAIT_LOOKING, 0.0f, 0};
    daNpcF_anmPlayData* pDat1[2] = {&dat1a, &dat1b};
    daNpcF_anmPlayData dat2a = {ANM_TO_WAIT_B, mpHIO->m.common.morf_frame, 1};
    daNpcF_anmPlayData dat2b = {ANM_WAIT_B, 0.0f, 0};
    daNpcF_anmPlayData* pDat2[2] = {&dat2a, &dat2b};
    daNpcF_anmPlayData dat3 = {ANM_WAIT_LOOKUP, mpHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat3[1] = {&dat3};
    daNpcF_anmPlayData dat4a = {ANM_KUNE_TALK, mpHIO->m.common.morf_frame, 1};
    daNpcF_anmPlayData dat4b = {ANM_KUNE_WAIT_B, 0.0f, 0};
    daNpcF_anmPlayData* pDat4[2] = {&dat4a, &dat4b};
    daNpcF_anmPlayData dat5 = {ANM_KUNE_WAIT_A, mpHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat5[1] = {&dat5};
    daNpcF_anmPlayData dat6a = {ANM_KUNE_SHISHI, mpHIO->m.common.morf_frame, 1};
    daNpcF_anmPlayData dat6b = {ANM_KUNE_WAIT_A, mpHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat6[2] = {&dat6a, &dat6b};
    daNpcF_anmPlayData dat7 = {ANM_WAIT_B, mpHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat7[1] = {&dat7};
    daNpcF_anmPlayData dat8 = {ANM_KUNE_WAIT_B, mpHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat8[1] = {&dat8};
    daNpcF_anmPlayData dat9a = {ANM_TALK_R, mpHIO->m.common.morf_frame, 1};
    daNpcF_anmPlayData dat9b = {ANM_TALKING_R, 0.0f, 0};
    daNpcF_anmPlayData* pDat9[2] = {&dat9a, &dat9b};
    daNpcF_anmPlayData dat10a = {ANM_LOOK, mpHIO->m.common.morf_frame, 1};
    daNpcF_anmPlayData dat10b = {ANM_WAIT_LOOKING, 0.0f, 0};
    daNpcF_anmPlayData* pDat10[2] = {&dat10a, &dat10b};
    daNpcF_anmPlayData** ppDat[11] = {
        pDat0, pDat1, pDat2, pDat3, pDat4, pDat5, pDat6, pDat7, pDat8, pDat9, pDat10,
    };
    if (mMotion >= 0 && mMotion < 11) {
        playMotionAnm(ppDat);
    }
}

void daNpcThe_c::lookat() {
    fopAc_ac_c* actor = NULL;
    J3DModel* model = mAnm_p->getModel();

    BOOL snap = false;
    f32 body_angleX_min = mpHIO->m.common.body_angleX_min;
    f32 body_angleX_max = mpHIO->m.common.body_angleX_max;
    f32 body_angleY_min = mpHIO->m.common.body_angleY_min;
    f32 body_angleY_max = mpHIO->m.common.body_angleY_max;
    f32 head_angleX_min = mpHIO->m.common.head_angleX_min;
    f32 head_angleX_max = mpHIO->m.common.head_angleX_max;
    f32 head_angleY_min = mpHIO->m.common.head_angleY_min;
    f32 head_angleY_max = mpHIO->m.common.head_angleY_max;

    s16 angle_delta = mCurAngle.y - mOldAngle.y;
    cXyz lookat_pos[3] = {mLookatPos[0], mLookatPos[1], mLookatPos[2]};
    csXyz* lookat_angle[3] = {&mLookatAngle[0], &mLookatAngle[1], &mLookatAngle[2]};

    switch (mLookMode) {
    case LOOK_RESET:
        snap = true;
        break;
    case LOOK_PLAYER_TALK:
        head_angleY_min = -80.0f;
        head_angleY_max = 80.0f;
        // fallthrough
    case LOOK_PLAYER:
        actor = daPy_getPlayerActorClass();
        break;
    case LOOK_ACTOR:
        actor = fopAcM_SearchByName(PROC_NPC_ZRC);
        break;
    case LOOK_ATTN:
        break;
    }

    if (actor != NULL) {
        if (mLookMode == LOOK_ACTOR) {
            mLookPos = actor->eyePos;
        } else {
            mLookPos = actor->attention_info.position;
            if (mLookMode != LOOK_PLAYER && mLookMode != LOOK_PLAYER_TALK && mLookMode != LOOK_ATTN)
            {
                mLookPos.y -= 40.0f;
            }
        }
        mLookat.setAttnPos(&mLookPos);
    } else {
        mLookat.setAttnPos(NULL);
    }

    mLookat.setParam(body_angleX_min, body_angleX_max, body_angleY_min, body_angleY_max, 0.0f, 0.0f,
                     0.0f, 0.0f, head_angleX_min, head_angleX_max, head_angleY_min, head_angleY_max,
                     mCurAngle.y, lookat_pos);
    mLookat.calc(this, model->getBaseTRMtx(), lookat_angle, snap, angle_delta, 0);
}

BOOL daNpcThe_c::wait(void* param_0) {
    switch (mMode) {
    case 0:
        mAttnChangeTimer = 0;
        // fallthrough

    case 1:
        if (mType == TYPE_KAKARIKO) {
            setExpression(EXPR_H_SMILE, -1.0f);
            setMotion(MOT_KUNE_WAIT_B, -1.0f, false);
        } else {
            setExpression(EXPR_NONE, -1.0f);
            setMotion(MOT_WAIT_A, -1.0f, false);
        }
        setLookMode(LOOK_NONE);
        mTurnMode = 0;
        mMode = 2;
        break;

    case 2:
        if (mType == TYPE_KAKARIKO) {
            if (mCurAngle.y != home.angle.y && step(home.angle.y, -1, -1, 0xf)) {
                mMode = 1;
            }
            setLookMode(LOOK_NONE);
        } else {
            if (!isSneaking()) {
                bool uvar3 = mActorMngr[0].getActorP() != NULL;
                if (chkFindPlayer2(uvar3, mCurAngle.y)) {
                    if (!uvar3) {
                        mActorMngr[0].entry(daPy_getPlayerActorClass());
                        mTurnMode = 0;
                    }
                } else if (uvar3) {
                    mActorMngr[0].remove();
                    mTurnMode = 0;
                }

                if (mActorMngr[0].getActorP() != NULL) {
                    setLookMode(LOOK_PLAYER);
                } else {
                    if (mCurAngle.y != home.angle.y && step(home.angle.y, -1, -1, 0xf)) {
                        mMode = 1;
                    }
                    setLookMode(LOOK_NONE);
                }

                if (mType == TYPE_BAR) {
                    daTag_EvtArea_c* evt_area =
                        static_cast<daTag_EvtArea_c*>(mActorMngr[2].getActorP());
                    if (evt_area != NULL) {
                        if (evt_area->chkPointInArea(daPy_getPlayerActorClass()->current.pos)) {
                            daNpcF_offTmpBit(0xb);
                            daNpcF_offTmpBit(0xc);
                            daNpcF_offTmpBit(0xd);
                            daNpcF_offTmpBit(0xe);
                            mOrderEvtNo = 3;
                            fopAcM_delete(evt_area);
                        }
                    }
                }
            } else {
                setLookMode(LOOK_NONE);
            }

            if (mCurAngle.y == home.angle.y) {
                fopAc_ac_c* actor = getAttnActorP(
                    mActorMngr[0].getActorP() != NULL, srchAttnActor1,
                    mpHIO->m.common.search_distance, mpHIO->m.common.search_height,
                    mpHIO->m.common.search_depth, mpHIO->m.common.fov, shape_angle.y, 120, 1);
                if (actor != NULL) {
                    mActorMngr[1].entry(actor);
                    setLookMode(LOOK_ATTN);
                }
            } else {
                mAttnChangeTimer = 0;
            }
        }

    case 3:
        break;
    }

    return true;
}

void daNpcThe_c::setMotion(int i_motion, f32 i_morf, BOOL i_restart) {
    s16 motion = i_motion;
    if (i_restart || mMotion != motion) {
        if (i_motion >= 0 && i_motion < 11) {
            mMotion = motion;
            mMotionMorfOverride = i_morf;
            mMotionPrevPhase = -1;
            mMotionPhase = 0;
        }
    }
}

void daNpcThe_c::setExpression(int i_expression, f32 i_morf) {
    if (i_expression >= 0 && i_expression < 14) {
        mExpression = i_expression;
        mExpressionMorfOverride = i_morf;
        mExpressionPrevPhase = -1;
        mExpressionPhase = 0;
    }
}

BOOL daNpcThe_c::waitTW(void* param_0) {
    switch (mMode) {
    case 0:
        mAttnChangeTimer = 0;
        // fallthrough

    case 1:
        if (mType == TYPE_KAKARIKO) {
            setExpression(EXPR_H_SMILE, -1.0f);
            setMotion(MOT_KUNE_WAIT_B, -1.0f, false);
        } else {
            setExpression(EXPR_H_TALK_C, -1.0f);
            setMotion(MOT_WAIT_A, -1.0f, false);
        }
        mTurnMode = 0;
        mMode = 2;
        break;

    case 2:
        if (mCurAngle.y != home.angle.y && step(home.angle.y, -1, -1, 0xf)) {
            mMode = 1;
        }
        setLookMode(LOOK_ACTOR);
        break;

    case 3:
        break;
    }

    return true;
}

BOOL daNpcThe_c::talk(void* param_0) {
    BOOL ret = false;
    BOOL bvar1 = false;
    switch (mMode) {
    case 0:
    case 1:
        initTalk(mFlowID, NULL);
        mAttnChangeTimer = 0;
        mTurnMode = 0;
        field_0xe1c = false;
        mMode = 2;
        break;

    case 2:
        if (mTwilight || isSneaking()) {
            bvar1 = true;
        } else {
            if (mType == TYPE_KAKARIKO &&
                (mAnm_p->getAnm() == getTrnsfrmKeyAnmP(l_resNames[l_bckGetParamList[24].arcIdx],
                                                       l_bckGetParamList[24].fileIdx) ||
                 mAnm_p->getAnm() == getTrnsfrmKeyAnmP(l_resNames[l_bckGetParamList[25].arcIdx],
                                                       l_bckGetParamList[25].fileIdx) ||
                 mAnm_p->getAnm() == getTrnsfrmKeyAnmP(l_resNames[l_bckGetParamList[26].arcIdx],
                                                       l_bckGetParamList[26].fileIdx)))
            {
                setLookMode(LOOK_NONE);
            } else {
                setLookMode(LOOK_PLAYER_TALK);
                mActorMngr[0].entry(daPy_getPlayerActorClass());
            }

            if (mType == TYPE_KAKARIKO || mCurAngle.y == fopAcM_searchPlayerAngleY(this)) {
                bvar1 = true;
            } else if (step(fopAcM_searchPlayerAngleY(this), -1, -1, 0xf)) {
                setExpression(EXPR_NONE, -1.0f);
                setMotion(MOT_WAIT_A, -1.0f, false);
                mTurnMode = 0;
            }
        }

        if (bvar1) {
            int local_24[2] = {-1, -1};
            if (mFlowID == 0x430) {
                local_24[0] = 0x13ef;
            }
            if (talkProc(local_24, 1, NULL)) {
                if (!mFlow.checkEndFlow()) {
                    daPy_getPlayerActorClass()->changeOriginalDemo();
                    daPy_getPlayerActorClass()->changeDemoMode(6, 0, 1, 0);
                } else {
                    if (mFlowID == 0x430) {
                        daPy_getPlayerActorClass()->cancelOriginalDemo();
                    }
                    int item_no = 0;
                    if (mFlow.getEventId(&item_no) == 1) {
                        mItemID = fopAcM_createItemForPresentDemo(&current.pos, item_no, 0, -1, -1,
                                                                  NULL, NULL);
                        if (mItemID != -1) {
                            s16 event_id = dComIfGp_getEventManager().getEventIdx(
                                this, "DEFAULT_GETITEM", 0xff);
                            dComIfGp_getEvent()->reset(this);
                            fopAcM_orderChangeEventId(this, event_id, 1, 0xffff);
                            field_0x9ec = true;
                        }
                    }
                    ret = true;
                }
            }
        }

        if (ret) {
            mMode = 3;
            if (mFlowID == 0x430) {
                if (!field_0xe1d) {
                    field_0xe1d = true;
                    field_0xe1c = true;
                } else {
                    mFlowID = home.angle.x;
                }
            }
            if (!field_0x9ec) {
                dComIfGp_event_reset();
            }
            field_0x9ec = false;
        }
        // fallthrough

    case 3:
    default:
        break;
    }
    return ret;
}

BOOL daNpcThe_c::test(void* param_0) {
    switch (mMode) {
    case 0:
        mAttnChangeTimer = 0;
        break;

    case 1:
        speedF = 0.0f;
        speed.setall(0.0f);
        mTurnMode = 0;
        mMode = 2;
        // fallthrough

    case 2:
        attention_info.flags = 0;
        if (mExpression != mpHIO->m.common.face_expression) {
            setExpression(mpHIO->m.common.face_expression, mpHIO->m.common.morf_frame);
        }
        setMotion(mpHIO->m.common.motion, mpHIO->m.common.morf_frame, false);
        setLookMode(mpHIO->m.common.look_mode);
        break;

    case 3:
    default:
        break;
    }

    return true;
}

BOOL daNpcThe_c::EvCut_TwResistance(int i_staffID) {
    return true;
}

BOOL daNpcThe_c::EvCut_Introduction(int i_staffID) {
    dEvent_manager_c& event_manager = dComIfGp_getEventManager();
    int* cut_name = (int*)event_manager.getMyNowCutName(i_staffID);
    if (event_manager.getIsAddvance(i_staffID)) {
        switch (*cut_name) {
        case '0001':
        case '0002':
        case '0003':
        case '0004':
        case '0005':
            initTalk(mFlowID, NULL);
            setLookMode(LOOK_PLAYER);
        }
    }

    switch (*cut_name) {
    case '0001':
    case '0002':
    case '0003':
    case '0004':
    case '0005':
        if (talkProc(NULL, 1, NULL)) {
            return true;
        }
    }

    return false;
}

static cPhs__Step daNpcThe_Create(void* i_this) {
    return static_cast<daNpcThe_c*>(i_this)->create();
}

cPhs__Step daNpcThe_c::create() {
    fopAcM_ct(this, daNpcThe_c);

    mType = getTypeFromParam();

    int flow_id = home.angle.x;
    if (flow_id != 0xffff) {
        mFlowID = flow_id;
    } else {
        mFlowID = -1;
    }

    if (mType == TYPE_KAKARIKO
        /* dSv_event_flag_c::M_035 - Cutscene - [cutscene: 35] after carriage guarding event */
        && (!dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[68])
            /* dSv_event_flag_c::M_045 - Lakebed Temple - Lakebed Temple clear */
            || dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[78])))
    {
        return cPhs_ERROR_e;
    }

    cPhs__Step step;
    int resources_loaded = 0;
    int i = 0;
    for (; l_loadRes_list[mType][i] >= 0; i++) {
        step = (cPhs__Step)dComIfG_resLoad(&mPhase[i], l_resNames[l_loadRes_list[mType][i]]);
        if (step == cPhs_ERROR_e || step == cPhs_UNK3_e) {
            return cPhs_ERROR_e;
        }
        if (step == cPhs_COMPLEATE_e) {
            resources_loaded++;
        }
    }

    if (resources_loaded == i) {
        u32 heap_size = 0;
        switch (mType) {
        case TYPE_BAR:
            heap_size = 0x4d70;
            break;
        case TYPE_1:
            heap_size = 0x4cc0;
            break;
        case TYPE_KAKARIKO:
            heap_size = 0x4d80;
            break;
        }
        if (!fopAcM_entrySolidHeap(this, createHeapCallBack, heap_size)) {
            return cPhs_ERROR_e;
        }

        if (isDelete()) {
            return cPhs_ERROR_e;
        }

        fopAcM_SetMtx(this, mAnm_p->getModel()->getBaseTRMtx());
        fopAcM_setCullSizeBox2(this, mAnm_p->getModel()->getModelData());
        mSound.init(&current.pos, &eyePos, 3, 1);
        mAcchCir.SetWall(mpHIO->m.common.width, mpHIO->m.common.knee_length);
        mAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1, &mAcchCir,
                  fopAcM_GetSpeed_p(this), fopAcM_GetAngle_p(this), fopAcM_GetShapeAngle_p(this));
        mAcch.SetRoofNone();
        mAcch.SetWaterNone();
        mAcch.CrrPos(dComIfG_Bgsp());
        mCcStts.Init(mpHIO->m.common.weight, 0, this);
        mColCyl.Set(mCcDCyl);
        mColCyl.SetStts(&mCcStts);
        mColCyl.SetTgType(0);
        mColCyl.SetTgSPrm(0);
        mColCyl.SetH(mpHIO->m.common.height);
        mColCyl.SetR(mpHIO->m.common.width);
        mGndChk = mAcch.m_gnd;
        mGroundH = mAcch.GetGroundH();

        setEnvTevColor();
        setRoomNo();
        reset();
        execute();

        return cPhs_COMPLEATE_e;
    }

    return cPhs_INIT_e;
}

void daNpcThe_c::reset() {
    initialize();
    mpMatAnm->initialize();
    mLookat.initialize();
    for (int i = 0; i < 3; i++) {
        mActorMngr[i].initialize();
    }
    mpNextActionFn = NULL;
    mpActionFn = NULL;
    field_0xe04 = 0;
    field_0xe08 = 0;
    field_0xe0c = 0;
    field_0xe10 = 0;
    mLookMode = -1;
    mMode = 0;
    field_0xe1c = false;
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
    field_0x9ee = 1;
}

static int daNpcThe_Delete(void* i_this) {
    static_cast<daNpcThe_c*>(i_this)->~daNpcThe_c();
    return 1;
}

static int daNpcThe_Execute(void* i_this) {
    return static_cast<daNpcThe_c*>(i_this)->execute();
}

static int daNpcThe_Draw(void* i_this) {
    return static_cast<daNpcThe_c*>(i_this)->Draw();
}

static int daNpcThe_IsDelete(void* i_this) {
    return 1;
}

void daNpcThe_c::setParam() {
    if (mActorMngr[2].getActorP() == NULL) {
        mActorMngr[2].entry(getEvtAreaTagP(19, 0));
    }

    ActionFn next_action = mpNextActionFn;
    mpNextActionFn = NULL;
    if (mpHIO->m.common.debug_mode_ON) {
        mpNextActionFn = &daNpcThe_c::test;
    } else if (mTwilight) {
        mpNextActionFn = &daNpcThe_c::waitTW;
    } else {
        mpNextActionFn = &daNpcThe_c::wait;
    }
    if (mpNextActionFn != next_action) {
        for (int i = 3; i < 3; i++) {
            mActorMngr[i].initialize();
        }
    }

    int speak_angle_idx = mType == TYPE_KAKARIKO ? (s16)1 : mpHIO->m.common.talk_angle;
    int talk_angle_idx = mType == TYPE_KAKARIKO ? (s16)1 : mpHIO->m.common.attention_angle;
    attention_info.flags = fopAc_AttnFlag_SPEAK_e | fopAc_AttnFlag_TALK_e;
    if (isSneaking()) {
        attention_info.distances[fopAc_attn_LOCK_e] = 0x4e;
        attention_info.distances[fopAc_attn_TALK_e] = attention_info.distances[fopAc_attn_LOCK_e];
        attention_info.distances[fopAc_attn_SPEAK_e] = 0x4d;
        attention_info.flags |= fopAc_AttnFlag_UNK_0x800000;
    } else {
        if (!strcmp(dComIfGp_getStartStageName(), "R_SP116")) {
            attention_info.distances[fopAc_attn_LOCK_e] =
                getDistTableIdx(mpHIO->m.common.attention_distance, talk_angle_idx);
            attention_info.distances[fopAc_attn_TALK_e] =
                attention_info.distances[fopAc_attn_LOCK_e];
            attention_info.distances[fopAc_attn_SPEAK_e] =
                getDistTableIdx(mpHIO->m.common.talk_distance + 1, speak_angle_idx);
        } else {
            attention_info.distances[fopAc_attn_LOCK_e] =
                getDistTableIdx(mpHIO->m.common.attention_distance, talk_angle_idx);
            attention_info.distances[fopAc_attn_TALK_e] =
                attention_info.distances[fopAc_attn_LOCK_e];
            attention_info.distances[fopAc_attn_SPEAK_e] =
                getDistTableIdx(mpHIO->m.common.talk_distance, speak_angle_idx);
        }
    }

    scale.setall(mpHIO->m.common.scale);
    mAcchCir.SetWallR(mpHIO->m.common.width);
    mAcchCir.SetWallH(mpHIO->m.common.knee_length);
    gravity = mpHIO->m.common.gravity;
}

BOOL daNpcThe_c::main() {
    if (!doEvent()) {
        doNormalAction(1);
    }

    if (field_0x9ee) {
        mExpressionMorfOverride = 0.0f;
        mMotionMorfOverride = 0.0f;
        field_0x9ee = false;
    }

    if (checkHide()) {
        attention_info.flags = 0;
    }

    if (dComIfGp_event_runCheck() && !eventInfo.checkCommandTalk() && mItemID != -1) {
        dComIfGp_event_setItemPartnerId(mItemID);
        mItemID = -1;
    }

    if (!mpHIO->m.common.debug_mode_ON &&
        (dComIfGp_event_runCheck() == FALSE || (mOrderNewEvt && dComIfGp_getEvent()->isOrderOK())))
    {
        if (mOrderEvtNo != 0 && l_resNames[l_evtGetParamList[mOrderEvtNo]] != NULL) {
            eventInfo.setArchiveName(l_resNames[l_evtGetParamList[mOrderEvtNo]]);
        }
        if (!strcmp(dComIfGp_getStartStageName(), "R_SP116")) {
            eventInfo.onCondition(dEvtCnd_CANTALKITEM_e);
        }
        orderEvent(field_0xe1c, l_evtNames[mOrderEvtNo], 0xffff, 0x28, 0xff, 1);
    }

    playExpression();
    playMotion();
    return true;
}

void daNpcThe_c::setExpressionTalkAfter() {
    switch (mExpression) {
    case EXPR_SMILE:
        setExpression(EXPR_H_SMILE, -1.0f);
        break;
    case EXPR_CHUCKLE:
        setExpression(EXPR_H_CHUCKLE, -1.0f);
        break;
    case EXPR_TALK_B:
        setExpression(EXPR_H_TALK_B, -1.0f);
        break;
    case EXPR_TALK_C:
        setExpression(EXPR_H_TALK_C, -1.0f);
        break;
    case EXPR_LAUGH:
        setExpression(EXPR_H_LAUGH, -1.0f);
        break;
    case EXPR_TALK_R:
        setExpression(EXPR_H_TALK_R, -1.0f);
        break;
    default:
        setExpression(EXPR_NONE, -1.0f);
        break;
    }
}

BOOL daNpcThe_c::doEvent() {
    BOOL ret = false;
    if (dComIfGp_event_runCheck()) {
        dEvent_manager_c& event_manager = dComIfGp_getEventManager();
        if (eventInfo.checkCommandTalk()) {
            if (mTwilight) {
                /* dSv_event_flag_c::F_0277 - Castle Town - Hear conversation between Telma and Ilia
                 * in Telma's shop (Twilight) */
                if (daNpcF_chkEvtBit(0x115)) {
                    mOrderEvtNo = 2;
                } else {
                    mOrderEvtNo = 1;
                }
                eventInfo.setArchiveName(l_resNames[l_evtGetParamList[mOrderEvtNo]]);
                event_manager.setObjectArchive(eventInfo.getArchiveName());
                mEventIdx = event_manager.getEventIdx(this, l_evtNames[mOrderEvtNo], 0xff);
                if (mEventIdx != -1) {
                    dComIfGp_getEvent()->reset(this);
                    fopAcM_orderChangeEventId(this, mEventIdx, 1, 0xffff);
                }
            } else if (dComIfGp_event_chkTalkXY()) {
                if (!dComIfGp_evmng_ChkPresentEnd()) {
                    return true;
                }
                u8 item_no = dComIfGp_event_getPreItemNo();
                if (item_no == 0x80) {
                    mFlowID = 0x430;
                    if (chkAction(&daNpcThe_c::talk)) {
                        (this->*mpActionFn)(NULL);
                    } else {
                        setAction(&daNpcThe_c::talk);
                    }
                } else if (item_no == 0x81) {
                    mFlowID = 0x431;
                    if (chkAction(&daNpcThe_c::talk)) {
                        (this->*mpActionFn)(NULL);
                    } else {
                        setAction(&daNpcThe_c::talk);
                    }
                } else {
                    s16 event_idx =
                        dComIfGp_getEventManager().getEventIdx(this, "NO_RESPONSE", 0xff);
                    dComIfGp_getEvent()->reset(this);
                    fopAcM_orderChangeEventId(this, event_idx, 1, 0xffff);
                }
            } else {
                if (chkAction(&daNpcThe_c::talk)) {
                    (this->*mpActionFn)(NULL);
                } else {
                    setAction(&daNpcThe_c::talk);
                }
            }
            ret = true;
        } else {
            s32 staff_id = event_manager.getMyStaffId(l_myName, this, -1);
            if (staff_id != -1) {
                mStaffID = staff_id;
                int act_idx = event_manager.getMyActIdx(staff_id, mEvtCutNameList,
                                                        ARRAY_SIZEU(mEvtCutNameList), 0, 0);
                if (act_idx > 0 && act_idx < 4) {
                    if ((this->*mEvtCutList[act_idx])(staff_id)) {
                        event_manager.cutEnd(staff_id);
                    }
                } else {
                    if ((this->*mEvtCutList[mOrderEvtNo])(staff_id)) {
                        event_manager.cutEnd(staff_id);
                    }
                }
                ret = true;
            }
            if (eventInfo.checkCommandDemoAccrpt() && mEventIdx != -1 &&
                event_manager.endCheck(mEventIdx))
            {
                dComIfGp_event_reset();
                mOrderEvtNo = 0;
                mEventIdx = -1;
            }
        }
        int expression, motion;
        int prev_msg_timer = mMsgTimer;
        if (ctrlMsgAnm(expression, motion, this, 0)) {
            if (!field_0x9eb) {
                setExpression(expression, -1.0f);
                setMotion(motion, -1.0f, false);
            }
        } else {
            if (prev_msg_timer != 0 && !field_0x9eb) {
                setExpressionTalkAfter();
            }
        }
    } else {
        mMsgTimer = 0;
        if (mStaffID != -1) {
            mMode = 1;
            mStaffID = -1;
        }
    }
    return ret;
}

BOOL daNpcThe_c::ctrlBtk() {
    if (mpMatAnm != NULL) {
        J3DAnmTextureSRTKey* anm = NULL;
        if (l_btkGetParamList[0].fileIdx >= 0) {
            anm = getTexSRTKeyAnmP(l_resNames[l_btkGetParamList[0].arcIdx],
                                   l_btkGetParamList[0].fileIdx);
        }
        if (anm == mBtkAnm.getBtkAnm()) {
            mpMatAnm->setNowOffsetX(cM_ssin(mEyeAngle.y) * 0.2f);
            mpMatAnm->setNowOffsetY(cM_ssin(mEyeAngle.x) * 0.2f);
            mpMatAnm->onEyeMoveFlag();
            return true;
        }
        mpMatAnm->offEyeMoveFlag();
    }
    return false;
}

void daNpcThe_c::setAttnPos() {
    if (mLookMode == LOOK_RESET) {
        for (int i = 0; i < 3; i++) {
            mLookatAngle[i].setall(0);
        }
    }

    setMtx();
    lookat();

    cXyz vec1(-10.0f, 0.0f, 0.0f);
    mDoMtx_stack_c::copy(mAnm_p->getModel()->getAnmMtx(4));
    mDoMtx_stack_c::multVecZero(&mHeadPos);
    mDoMtx_stack_c::multVec(&vec1, &eyePos);
    vec1.y = 0.0f;
    mDoMtx_stack_c::multVec(&vec1, &vec1);
    mHeadAngle.x = cLib_targetAngleX(&mHeadPos, &vec1);
    mHeadAngle.y = cLib_targetAngleY(&mHeadPos, &vec1);

    cXyz* attn_pos = mLookat.getAttnPos();
    if (attn_pos != NULL) {
        cXyz vec2 = *attn_pos - eyePos;
        mEyeAngle.y = -(mLookatAngle[2].y + mCurAngle.y);
        mEyeAngle.y += cM_atan2s(vec2.x, vec2.z);
        mEyeAngle.x = mHeadAngle.x - cM_atan2s(vec2.y, vec2.absXZ());
    } else {
        mEyeAngle.y = 0;
        mEyeAngle.x = 0;
    }

    attention_info.position.set(current.pos.x, current.pos.y + mpHIO->m.common.attention_offset,
                                current.pos.z);

    if (!mHide) {
        mColCyl.SetC(current.pos);
        dComIfG_Ccsp()->Set(&mColCyl);
    }
}

BOOL daNpcThe_c::drawDbgInfo() {
    return false;
}

static actor_method_class daNpcThe_MethodTable = {
    (process_method_func)daNpcThe_Create,  (process_method_func)daNpcThe_Delete,
    (process_method_func)daNpcThe_Execute, (process_method_func)daNpcThe_IsDelete,
    (process_method_func)daNpcThe_Draw,
};

actor_process_profile_definition g_profile_NPC_THE = {
    fpcLy_CURRENT_e,
    7,
    fpcPi_CURRENT_e,
    PROC_NPC_THE,
    &g_fpcLf_Method.base,
    sizeof(daNpcThe_c),
    0,
    0,
    &g_fopAc_Method.base,
    0x174,
    &daNpcThe_MethodTable,
    0x44108,
    fopAc_NPC_e,
    fopAc_CULLBOX_CUSTOM_e,
};
