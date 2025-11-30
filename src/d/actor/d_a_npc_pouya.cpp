/**
 * @file d_a_npc_pouya.cpp
 *
 */

#include "d/dolzel_rel.h"  // IWYU pragma: keep

#include "d/actor/d_a_npc_pouya.h"

const daNpc_Pouya_HIOParam daNpc_Pouya_Param_c::m = {
    120.0f,  // attention_offset
    -3.0f,   // gravity
    1.0f,    // scale
    400.0f,  // real_shadow_size
    255.0f,  // weight
    100.0f,  // height
    35.0f,   // knee_length
    40.0f,   // width
    0.0f,    // body_angleX_max
    0.0f,    // body_angleX_min
    10.0f,   // body_angleY_max
    -10.0f,  // body_angleY_min
    30.0f,   // head_angleX_max
    -10.0f,  // head_angleX_min
    45.0f,   // head_angleY_max
    -45.0f,  // head_angleY_min
    0.6f,    // neck_rotation_ratio
    12.0f,   // morf_frame
    3,       // talk_distance
    6,       // talk_angle
    5,       // attention_distance
    6,       // attention_angle
    110.0f,  // fov
    0.0f,    // search_distance
    0.0f,    // search_height
    0.0f,    // search_depth
    60,      // attention_time
    8,       // damage_time
    0,       // face_expression
    0,       // motion
    0,       // look_mode
    0,       // debug_mode_ON
    0,       // debug_info_ON
    4.0f,    // expression_morf_frame
    0.0f,    // box_min_x
    0.0f,    // box_min_y
    0.0f,    // box_min_z
    0.0f,    // box_max_x
    0.0f,    // box_max_y
    0.0f,    // box_max_z
    0.0f,    // box_offset
};

#if DEBUG
daNpc_Pouya_HIO_c::daNpc_Pouya_HIO_c() {
    m = daNpc_Pouya_Param_c::m;
}

void daNpc_Pouya_HIO_c::listenPropertyEvent(const JORPropertyEvent* event) {
    // TODO
}

void daNpc_Pouya_HIO_c::genMessage(JORMContext* ctext) {
    // TODO
}
#endif

static int l_bmdData[3][2] = {
    {26, 1},
    {23, 1},
    {34, 2},
};

static daNpcT_evtData_c l_evtList[9] = {
    {"", 0},
    {"DEFAULT_GETITEM", 0},
    {"NO_RESPONSE", 0},
    {"HAVE_FAVORTO_ASK1", 1},
    {"HAVE_FAVORTO_ASK2", 1},
    {"RETURN_FAVOR_1_01", 1},
    {"RETURN_FAVOR_1_02", 1},
    {"RETURN_FAVOR_2_01", 2},
    {"RETURN_FAVOR_2_02", 2},
};

static char* l_resNameList[3] = {
    "",
    "pouyaA",
    "pouyaB",
};

static s8 l_loadResPtrn0[2] = {1, -1};

static s8 l_loadResPtrn1[2] = {2, -1};

static s8* l_loadResPtrnList[6] = {
    l_loadResPtrn0, l_loadResPtrn0, l_loadResPtrn1, l_loadResPtrn1, l_loadResPtrn1, l_loadResPtrn0,
};

static daNpcT_faceMotionAnmData_c l_faceMotionAnmData[16] = {
    {-1, 0, 0, -1, 0, 0, 0}, {11, 0, 1, -1, 0, 0, 0}, {8, 2, 1, -1, 0, 0, 0},
    {9, 0, 1, -1, 0, 0, 0},  {10, 0, 1, -1, 0, 0, 0}, {12, 2, 1, -1, 0, 0, 0},
    {14, 2, 2, 47, 2, 2, 0}, {10, 0, 2, 43, 0, 2, 0}, {16, 2, 2, 49, 2, 2, 0},
    {11, 0, 2, 44, 0, 2, 0}, {17, 2, 2, 50, 2, 2, 0}, {9, 0, 2, 42, 0, 2, 0},
    {15, 2, 2, 48, 2, 2, 0}, {12, 2, 2, 45, 2, 2, 0}, {13, 0, 2, 46, 0, 2, 0},
    {18, 2, 2, 51, 2, 2, 0},
};

static daNpcT_motionAnmData_c l_motionAnmData[26] = {
    {18, 2, 1, 36, 2, 1, 0, 0}, {19, 2, 1, 36, 2, 1, 0, 0}, {20, 2, 1, 36, 2, 1, 0, 0},
    {13, 0, 1, 36, 2, 1, 0, 0}, {16, 0, 1, 36, 2, 1, 0, 0}, {17, 2, 1, 36, 2, 1, 0, 0},
    {14, 0, 1, 36, 2, 1, 0, 0}, {15, 2, 1, 36, 2, 1, 0, 0}, {16, 0, 1, 36, 2, 1, 0, 0},
    {14, 0, 1, 36, 2, 1, 0, 0}, {30, 2, 2, 37, 0, 2, 1, 0}, {25, 0, 2, 37, 0, 2, 1, 0},
    {31, 2, 2, 38, 2, 2, 0, 0}, {21, 0, 2, 37, 0, 2, 1, 0}, {22, 2, 2, 37, 0, 2, 1, 0},
    {7, 0, 2, 37, 0, 2, 1, 0},  {8, 2, 2, 37, 0, 2, 1, 0},  {23, 0, 2, 37, 0, 2, 1, 0},
    {24, 2, 2, 37, 0, 2, 1, 0}, {19, 0, 2, 37, 0, 2, 1, 0}, {20, 2, 2, 37, 0, 2, 1, 0},
    {28, 2, 2, 37, 0, 2, 1, 0}, {27, 0, 2, 37, 0, 2, 1, 0}, {29, 2, 2, 37, 0, 2, 1, 0},
    {26, 0, 2, 37, 0, 2, 1, 0}, {23, 0, 2, 37, 0, 2, 1, 0},
};

static daNpcT_MotionSeqMngr_c::sequenceStepData_c l_faceMotionSequenceData[72] = {
    {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0}, {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},
    {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0}, {-1, 0, 0},  {1, -1, 1},  {-1, 0, 0},
    {-1, 0, 0},  {-1, 0, 0},  {3, -1, 1},  {2, 0, 0},  {-1, 0, 0},  {-1, 0, 0},  {4, -1, 1},
    {5, 0, 0},   {-1, 0, 0},  {-1, 0, 0},  {2, -1, 0}, {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},
    {6, -1, 0},  {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0}, {7, -1, 1},  {8, 0, 0},   {-1, 0, 0},
    {-1, 0, 0},  {9, -1, 1},  {10, 0, 0},  {-1, 0, 0}, {-1, 0, 0},  {11, -1, 1}, {12, 0, 0},
    {-1, 0, 0},  {-1, 0, 0},  {13, -1, 0}, {-1, 0, 0}, {-1, 0, 0},  {-1, 0, 0},  {14, -1, 1},
    {15, 0, 0},  {-1, 0, 0},  {-1, 0, 0},  {8, -1, 0}, {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},
    {10, -1, 0}, {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0}, {12, -1, 0}, {-1, 0, 0},  {-1, 0, 0},
    {-1, 0, 0},  {15, -1, 0}, {-1, 0, 0},  {-1, 0, 0}, {-1, 0, 0},  {0, -1, 0},  {-1, 0, 0},
    {-1, 0, 0},  {-1, 0, 0},
};

static daNpcT_MotionSeqMngr_c::sequenceStepData_c l_motionSequenceData[76] = {
    {0, -1, 0},  {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},  {1, -1, 0},  {-1, 0, 0},  {-1, 0, 0},
    {-1, 0, 0},  {3, -1, 1},  {1, 0, 0},   {-1, 0, 0},  {-1, 0, 0},  {4, -1, 1},  {5, 0, 0},
    {-1, 0, 0},  {-1, 0, 0},  {2, -1, 0},  {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},  {6, -1, 1},
    {7, 0, 0},   {-1, 0, 0},  {-1, 0, 0},  {8, -1, 1},  {1, 0, 0},   {-1, 0, 0},  {-1, 0, 0},
    {9, -1, 1},  {1, 0, 0},   {-1, 0, 0},  {-1, 0, 0},  {10, -1, 0}, {-1, 0, 0},  {-1, 0, 0},
    {-1, 0, 0},  {11, -1, 1}, {10, 0, 0},  {-1, 0, 0},  {-1, 0, 0},  {12, -1, 0}, {-1, 0, 0},
    {-1, 0, 0},  {-1, 0, 0},  {13, -1, 1}, {14, 0, 0},  {-1, 0, 0},  {-1, 0, 0},  {15, -1, 1},
    {16, 0, 0},  {-1, 0, 0},  {-1, 0, 0},  {17, -1, 1}, {18, 0, 0},  {-1, 0, 0},  {-1, 0, 0},
    {19, -1, 1}, {20, 0, 0},  {-1, 0, 0},  {-1, 0, 0},  {21, -1, 0}, {-1, 0, 0},  {-1, 0, 0},
    {-1, 0, 0},  {22, -1, 1}, {23, 0, 0},  {-1, 0, 0},  {-1, 0, 0},  {24, -1, 1}, {21, 0, 0},
    {-1, 0, 0},  {-1, 0, 0},  {25, -1, 1}, {10, 0, 0},  {-1, 0, 0},  {-1, 0, 0},
};

char* daNpc_Pouya_c::mCutNameList[3] = {
    "",
    "HAVE_FAVORTO_ASK",
    "RETURN_FAVOR",
};

daNpc_Pouya_c::cutFunc daNpc_Pouya_c::mCutList[3] = {
    NULL,
    &daNpc_Pouya_c::cutHaveFavorToAsk,
    &daNpc_Pouya_c::cutHaveFavorToAsk,
};

daNpc_Pouya_c::~daNpc_Pouya_c() {
    if (mpMorf[0] != NULL) {
        mpMorf[0]->stopZelAnime();
    }

    deleteRes(l_loadResPtrnList[mType], (const char**)l_resNameList);
}

int daNpc_Pouya_c::create() {
    static int const heapSize[6] = {
        0x8000, 0x5440, 0x3680, 0x3680, 0x8000, 0x8000,
    };

    daNpcT_ct(this, daNpc_Pouya_c, l_faceMotionAnmData, l_motionAnmData, l_faceMotionSequenceData,
              4, l_motionSequenceData, 4, l_evtList, l_resNameList);

    mType = getType();
    mFlowNodeNo = getFlowNodeNo();
    mTwilight = 0;

    int phase = loadRes(l_loadResPtrnList[mType], (const char**)l_resNameList);
    if (phase == cPhs_COMPLEATE_e) {
        if (isDelete()) {
            return cPhs_ERROR_e;
        }
        if (!fopAcM_entrySolidHeap(this, createHeapCallBack, heapSize[mType])) {
            return cPhs_ERROR_e;
        }
        J3DModelData* modelData = mpMorf[0]->getModel()->getModelData();
        fopAcM_SetMtx(this, mpMorf[0]->getModel()->getBaseTRMtx());
        fopAcM_setCullSizeBox(this, -150.0f, -100.0f, -150.0f, 150.0f, 300.0f, 150.0f);
        mSound.init(&current.pos, &eyePos, 3, 1);
        reset();
        mAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1, &mAcchCir,
                  fopAcM_GetSpeed_p(this), fopAcM_GetAngle_p(this), fopAcM_GetShapeAngle_p(this));
        mCcStts.Init(mpHIO->m.common.weight, 0, this);
        mCyl.Set(mCcDCyl);
        mCyl.SetStts(&mCcStts);
        mCyl.SetTgHitCallback(tgHitCallBack);
        mAcch.SetGrndNone();
        mAcch.SetWallNone();
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

int daNpc_Pouya_c::CreateHeap() {
    static int const bmdTypeList[1] = {1};

    u32 bmd_idx = 0;
    if (chkPouyaB()) {
        bmd_idx = 2;
    }
    int arc_idx = l_bmdData[bmd_idx][1];
    int res_idx = l_bmdData[bmd_idx][0];

    J3DModelData* modelData =
        static_cast<J3DModelData*>(dComIfG_getObjectRes(l_resNameList[arc_idx], res_idx));
    if (modelData == NULL) {
        return 0;
    }

    u32 local_30 = chkPouyaB() ? 0x11020284 : 0x11000284;

    mpMorf[0] =
        new mDoExt_McaMorfSO(modelData, NULL, NULL, NULL, -1, 1.0f, 0, -1, &mSound, 0, local_30);
    if (mpMorf[0] == NULL || mpMorf[0]->getModel() == NULL) {
        return 0;
    }

    J3DModel* model = mpMorf[0]->getModel();

    for (u16 i = 0; i < modelData->getJointNum(); i++) {
        modelData->getJointNodePointer(i)->setCallBack(ctrlJointCallBack);
    }

    model->setUserArea((uintptr_t)this);

    if (chkPouyaB()) {
        mpMatAnm[0] = new daNpcT_MatAnm_c();
        if (mpMatAnm[0] == NULL) {
            return 0;
        }
    }

    if (!chkPouyaB()) {
        for (int i = 0; i < 1; i++) {
            if (l_bmdData[bmdTypeList[i]][0] >= 0) {
                modelData = (J3DModelData*)dComIfG_getObjectRes(
                    l_resNameList[l_bmdData[bmdTypeList[i]][1]], l_bmdData[bmdTypeList[i]][0]);
            } else {
                modelData = NULL;
            }
            if (modelData != NULL) {
                mpModel[i] = mDoExt_J3DModel__create(modelData, 0, 0x11000084);
            } else {
                mpModel[i] = NULL;
            }
        }
    }

    if (chkPouyaB()) {
        if (setFaceMotionAnm(6, false) && setMotionAnm(10, 0.0f, 0)) {
            return 1;
        }
    } else if (setFaceMotionAnm(1, false) && setMotionAnm(0, 0.0f, 0)) {
        return 1;
    }

    return 0;
}

int daNpc_Pouya_c::Delete() {
    fopAcM_GetID(this);
    this->~daNpc_Pouya_c();
    return 1;
}

int daNpc_Pouya_c::Execute() {
    return daNpcT_c::execute();
}

int daNpc_Pouya_c::Draw() {
    if (mpMatAnm[0] != NULL) {
        J3DModelData* modelData = mpMorf[0]->getModel()->getModelData();
        modelData->getMaterialNodePointer(getEyeballMaterialNo())->setMaterialAnm(mpMatAnm[0]);
    }
    return daNpcT_c::draw(0, 1, mRealShadowSize, NULL, 100.0f, 0, 0, 0);
}

int daNpc_Pouya_c::createHeapCallBack(fopAc_ac_c* i_this) {
    return static_cast<daNpc_Pouya_c*>(i_this)->CreateHeap();
}

int daNpc_Pouya_c::ctrlJointCallBack(J3DJoint* param_0, int param_1) {
    if (param_1 == 0) {
        J3DModel* model = j3dSys.getModel();

        daNpc_Pouya_c* i_this = reinterpret_cast<daNpc_Pouya_c*>(model->getUserArea());
        if (i_this != 0) {
            i_this->ctrlJoint(param_0, model);
        }
    }

    return 1;
}

u8 daNpc_Pouya_c::getType() {
    switch (fopAcM_GetParam(this) & 0xFF) {
    case 0:
        return TYPE_0;
    case 1:
        return TYPE_1;
    case 2:
        return TYPE_2;
    case 3:
        return TYPE_3;
    case 4:
        return TYPE_4;
    }
    return TYPE_5;
}

BOOL daNpc_Pouya_c::isDelete() {
    switch (mType) {
    case TYPE_0:
        return FALSE;
    case TYPE_1:
        /* dSv_event_flag_c::F_0458 - Coversation with Jovani after collecting 60 ghosts */
        return daNpcT_chkEvtBit(0x1CA);
    case TYPE_2:
        /* dSv_event_flag_c::F_0458 - Coversation with Jovani after collecting 60 ghosts */
        return !daNpcT_chkEvtBit(0x1CA);
    case TYPE_3:
        /* dSv_event_flag_c::F_0458 - Coversation with Jovani after collecting 60 ghosts */
        return daNpcT_chkEvtBit(0x1CA);
    case TYPE_4:
        return FALSE;
    }
    return FALSE;
}

void daNpc_Pouya_c::reset() {
    csXyz cStack_10;
    int size = (u8*)&field_0xfd0 - (u8*)&field_0xfb0;

    if (mpMatAnm[0] != NULL) {
        mpMatAnm[0]->initialize();
    }
    initialize();
    memset(&field_0xfb0, 0, size);

    cStack_10.setall(0);
    cStack_10.y = home.angle.y;

    switch (mType) {
    case TYPE_0:
        break;
    case TYPE_1:
        /* dSv_event_flag_c::F_0694 - Conversation with Jovani after collecting 20 ghosts */
        field_0xfcf = !daNpcT_chkEvtBit(0x2B6);
        eventInfo.setIdx(0);
        break;
    case TYPE_2:
        break;
    case TYPE_3:
        eventInfo.setIdx(1);
        mHide = 1;
        break;
    case TYPE_4:
        break;
    }

    if (chkPouyaB()) {
        mFaceMotionSeqMngr.setOffset(7);
        mMotionSeqMngr.setOffset(8);
    }

    setAngle(cStack_10);
}

void daNpc_Pouya_c::ctrlSubFaceMotion(int param_1) {
    static int normal[3] = {31, 2, 1};

    static int uttae01[3] = {32, 0, 1};
    static int uttae02[3] = {33, 2, 1};

    static int koufun01[3] = {29, 0, 1};
    static int koufun02[3] = {30, 2, 1};

    static int* brkSeq_normal[2] = {
        normal,
        NULL,
    };

    static int* brkSeq_uttae01_uttae02[3] = {
        uttae01,
        uttae02,
        NULL,
    };

    static int* brkSeq_koufun01_koufun02[3] = {
        koufun01,
        koufun02,
        NULL,
    };

    static int** brkSeqList[3] = {
        brkSeq_normal,
        brkSeq_uttae01_uttae02,
        brkSeq_koufun01_koufun02,
    };

    J3DAnmTevRegKey* anm_tev = NULL;

    if (param_1 != 0) {
        switch (mFaceMotionSeqMngr.getNo()) {
        case FACE_MOT_UNK_0:
        case FACE_MOT_UNK_3:
            mAnmFlags &= ~(ANM_PAUSE_BRK | ANM_PLAY_BRK);
            field_0xfcc = 0;
            field_0xfcd = 0;
            break;
        case FACE_MOT_UNK_1:
        case FACE_MOT_UNK_4:
        case FACE_MOT_UNK_6:
            mAnmFlags &= ~(ANM_PAUSE_BRK | ANM_PLAY_BRK);
            field_0xfcc = 1;
            field_0xfcd = 0;
            break;
        case FACE_MOT_UNK_2:
        case FACE_MOT_UNK_5:
            mAnmFlags &= ~(ANM_PAUSE_BRK | ANM_PLAY_BRK);
            field_0xfcc = 2;
            field_0xfcd = 0;
            break;
        default:
            field_0xfcc = -1;
        }
    }

    if (field_0xfcc >= 0) {
        if ((mAnmFlags & ANM_PLAY_BRK) != 0 && mBrkAnm.isStop()) {
            mAnmFlags &= ~(ANM_PAUSE_BRK | ANM_PLAY_BRK);
            field_0xfcd++;
        }
        if ((mAnmFlags & ANM_PLAY_BRK) == 0) {
            int* piVar4 = brkSeqList[field_0xfcc][field_0xfcd];
            if (piVar4) {
                anm_tev = getTevRegKeyAnmP(l_resNameList[piVar4[2]], *piVar4);
            }
            if (anm_tev != NULL &&
                setBrkAnm(anm_tev, mpMorf[0]->getModel()->getModelData(), 1.0f, piVar4[1]))
            {
                mAnmFlags |= ANM_PAUSE_BRK;
            }
            if (mBrkAnm.getBrkAnm()) {
                mAnmFlags |= ANM_PLAY_BRK;
            }
        }
    }
}

void daNpc_Pouya_c::afterJntAnm(int param_1) {
    if (param_1 == 1) {
        mDoMtx_stack_c::YrotM(-mStagger.getAngleZ(1));
        mDoMtx_stack_c::ZrotM(mStagger.getAngleX(1));
    } else if (param_1 == 4) {
        mDoMtx_stack_c::YrotM(-mStagger.getAngleZ(0));
        mDoMtx_stack_c::ZrotM(mStagger.getAngleX(0));
    }
}

void daNpc_Pouya_c::setParam() {
    selectAction();
    srchActors();

    s16 talk_distance = mpHIO->m.common.talk_distance;
    s16 talk_angle = mpHIO->m.common.talk_angle;
    s16 attention_distance = mpHIO->m.common.attention_distance;
    s16 attention_angle = mpHIO->m.common.attention_angle;

    if (mType != TYPE_0 && mType != TYPE_4 && mType != TYPE_5) {
        if (mType == TYPE_2) {
            talk_distance = 3;
            talk_angle = 4;
            attention_distance = 3;
            attention_angle = 4;
        } else {
            talk_distance = 3;
            talk_angle = 2;
            attention_distance = 3;
            attention_angle = 2;
        }
    }

    attention_info.distances[fopAc_attn_LOCK_e] =
        daNpcT_getDistTableIdx(attention_distance, attention_angle);
    attention_info.distances[fopAc_attn_TALK_e] = attention_info.distances[fopAc_attn_LOCK_e];
    attention_info.distances[fopAc_attn_SPEAK_e] =
        daNpcT_getDistTableIdx(talk_distance, talk_angle);
    attention_info.flags = fopAc_AttnFlag_SPEAK_e | fopAc_AttnFlag_TALK_e;

    scale.set(mpHIO->m.common.scale, mpHIO->m.common.scale, mpHIO->m.common.scale);

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
    mAcch.SetGrndNone();
    mAcch.SetWallNone();
    gravity = 0.0f;
}

BOOL daNpc_Pouya_c::checkChangeEvt() {
    if (!chkAction(&daNpc_Pouya_c::talk)) {
        mPreItemNo = 0;
        if (dComIfGp_event_chkTalkXY()) {
            if (dComIfGp_evmng_ChkPresentEnd()) {
                mEvtNo = 2;
                evtChange();
            }
            return TRUE;
        }

        switch (mType) {
        case TYPE_0:
            break;
        case TYPE_1:
            if (dComIfGs_isSaveSwitch(0x1F)) {
                /* dSv_event_flag_c::F_0456 - First time meeting with Jovani */
                if (!daNpcT_chkEvtBit(0x1C8)) {
                    daNpcT_offTmpBit(0xB);
                    mEvtNo = 3;
                    evtChange();
                    return TRUE;
                }
                if (dComIfGs_getPohSpiritNum() >= 60) {
                    /* dSv_event_flag_c::F_0458 - Coversation with Jovani after collecting 60 ghosts
                     */
                    if (!daNpcT_chkEvtBit(0x1CA)) {
                        daNpcT_offTmpBit(0xB);
                        mEvtNo = 7;
                        evtChange();
                        return TRUE;
                    }
                } else if (dComIfGs_getPohSpiritNum() >= 20 &&
                           /* dSv_event_flag_c::F_0694 - Conversation with Jovani after collecting
                            * 20 ghosts */
                           !daNpcT_chkEvtBit(0x2B6))
                {
                    daNpcT_offTmpBit(0xB);
                    mEvtNo = 5;
                    evtChange();
                    return TRUE;
                }
            }
            break;
        case TYPE_2:
        case TYPE_3:
        case TYPE_4:
        case TYPE_5:
            break;
        }
    }

    return FALSE;
}

void daNpc_Pouya_c::setAfterTalkMotion() {
    int iVar2 = FACE_MOT_UNK_17;

    switch (mFaceMotionSeqMngr.getNo()) {
    case FACE_MOT_UNK_0:
    case FACE_MOT_UNK_1:
    case FACE_MOT_UNK_2:
    case FACE_MOT_UNK_3:
    case FACE_MOT_UNK_4:
    case FACE_MOT_UNK_5:
    case FACE_MOT_UNK_6:
        return;
    case FACE_MOT_UNK_8:
        iVar2 = FACE_MOT_UNK_13;
        break;
    case FACE_MOT_UNK_9:
        iVar2 = FACE_MOT_UNK_14;
        break;
    case FACE_MOT_UNK_10:
        iVar2 = FACE_MOT_UNK_15;
        break;
    case FACE_MOT_UNK_12:
        iVar2 = FACE_MOT_UNK_16;
        break;
    }

    mFaceMotionSeqMngr.setNo(iVar2, -1.0f, 0, 0);
}

void daNpc_Pouya_c::srchActors() {}

BOOL daNpc_Pouya_c::evtTalk() {
    if (chkAction(&daNpc_Pouya_c::talk)) {
        (this->*mAction)(NULL);
    } else {
        setAction(&daNpc_Pouya_c::talk);
    }
    return TRUE;
}

BOOL daNpc_Pouya_c::evtCutProc() {
    int staffId = dComIfGp_getEventManager().getMyStaffId("Pouya", this, -1);
    if (staffId != -1) {
        mStaffId = staffId;
        int actIdx =
            dComIfGp_getEventManager().getMyActIdx(mStaffId, (char**)mCutNameList, 3, 0, 0);
        if ((this->*(mCutList[actIdx]))(mStaffId) != 0) {
            dComIfGp_getEventManager().cutEnd(mStaffId);
        }
        return true;
    }
    return false;
}

void daNpc_Pouya_c::action() {
    fopAc_ac_c* hit_actor = hitChk(&mCyl, 0xFFFFFFFF);

    if (hit_actor != NULL && mType != TYPE_1) {
        cCcD_ObjHitInf* hit_obj = mCyl.GetTgHitObj();
        if (hit_obj->ChkAtType(AT_TYPE_THROW_OBJ)) {
            mStagger.setParam(this, hit_actor, mCurAngle.y);
            setDamage(0, 17, 0);
            mDamageTimerStart = 0;
            mJntAnm.lookNone(1);
        }
    }

    if (mStagger.checkRebirth()) {
        mStagger.initialize();
        mMode = MODE_INIT;
    }

    if (field_0xfb0 != NULL) {
        if (mAction == field_0xfb0) {
            (this->*mAction)(NULL);
        } else {
            setAction(field_0xfb0);
        }
    }
}

void daNpc_Pouya_c::beforeMove() {
    fopAcM_OffStatus(this, 0x8000000);
    if (checkHide()) {
        fopAcM_OnStatus(this, 0x8000000);
    }
    if (checkHide() || mNoDraw) {
        attention_info.flags = 0;
    }
}

void daNpc_Pouya_c::setAttnPos() {
    cXyz acStack_40(20.0f, 30.0f, 0.0f);
    cXyz cStack_4c(20.0f, 0.0f, 0.0f);

    mStagger.calc(0);

    f32 dVar8 = cM_s2rad(mCurAngle.y - field_0xd7e.y);

    mJntAnm.setParam(this, mpMorf[0]->getModel(), &acStack_40, getBackboneJointNo(),
                     getNeckJointNo(), getHeadJointNo(), 0.0f, 0.0f, 0.0f, 0.0f,
                     mpHIO->m.common.head_angleX_min, mpHIO->m.common.head_angleX_max,
                     mpHIO->m.common.head_angleY_min, mpHIO->m.common.head_angleY_max,
                     mpHIO->m.common.neck_rotation_ratio, dVar8, &cStack_4c);

    mJntAnm.calcJntRad(0.2f, 1.0f, dVar8);
    setMtx();

    mDoMtx_stack_c::copy(mpMorf[0]->getModel()->getAnmMtx(getHeadJointNo()));
    mDoMtx_stack_c::multVec(&acStack_40, &eyePos);

    mJntAnm.setEyeAngleX(eyePos, 1.0f, 0);
    mJntAnm.setEyeAngleY(eyePos, mCurAngle.y, 1, 1.0f, 0);

    acStack_40.set(0.0f, 0.0f, -10.0f);
    acStack_40.y = mpHIO->m.common.attention_offset;

    if (chkPouyaB()) {
        if (mType != TYPE_3) {
            acStack_40.set(0.0f, 120.0f, 60.0f);
        }
    } else if (field_0xfcf != 0) {
        acStack_40.y += 20.0f;
    }

    mDoMtx_stack_c::YrotS(mCurAngle.y);
    mDoMtx_stack_c::multVec(&acStack_40, &acStack_40);

    attention_info.position = current.pos + acStack_40;

    if (!chkPouyaB() && mHide == 0) {
        setPrtcls();
    }
}

void daNpc_Pouya_c::setCollision() {
    cXyz cStack_48;

    if (mHide == 0) {
        u32 tgType = 0xd8fbfdff;
        u32 tgSPrm = 0x1f;

        if (dComIfGp_event_runCheck()) {
            tgType = 0;
            tgSPrm = 0;
        } else {
            if (mTwilight != 0) {
                tgType = 0;
                tgSPrm = 0;
            } else if (mStagger.checkStagger()) {
                tgType = 0;
                tgSPrm = 0;
            }
        }

        mCyl.SetCoSPrm(0x79);
        mCyl.SetTgType(tgType);
        mCyl.SetTgSPrm(tgSPrm);
        mCyl.OnTgNoHitMark();

        cStack_48.set(0.0f, 0.0f, 0.0f);

        f32 cylHeight = mCylH;
        f32 cylRadius = mWallR;

        mDoMtx_stack_c::YrotS(mCurAngle.y);
        mDoMtx_stack_c::multVec(&cStack_48, &cStack_48);
        cStack_48 += current.pos;
        mCyl.SetH(cylHeight);
        mCyl.SetR(cylRadius);
        mCyl.SetC(cStack_48);
        dComIfG_Ccsp()->Set(&mCyl);
    }

    mCyl.ClrCoHit();
    mCyl.ClrTgHit();
}

int daNpc_Pouya_c::drawDbgInfo() {
    return 0;
}

void daNpc_Pouya_c::drawOtherMdl() {
    static int const jointNo[1] = {4};

    J3DModel* model_p = mpMorf[0]->getModel();

    for (int i = 0; i < 1; i++) {
        if (mpModel[i] != NULL && (i == 0 && field_0xfcf != 0)) {
            J3DModelData* model_data = mpModel[i]->getModelData();
            g_env_light.setLightTevColorType_MAJI(mpModel[i], &tevStr);
            mDoMtx_stack_c::copy(model_p->getAnmMtx(jointNo[i]));
            Mtx jointAnmMtx;
            cMtx_copy(mDoMtx_stack_c::get(), jointAnmMtx);
            mpModel[i]->setBaseTRMtx(jointAnmMtx);
            fopAcM_setEffectMtx(this, model_data);
            mDoExt_modelUpdateDL(mpModel[i]);
            dComIfGd_addRealShadow(mShadowKey, mpModel[i]);
        }
    }
}

bool daNpc_Pouya_c::afterSetMotionAnm(int param_0, int param_1, f32 param_2, int param_3) {
    switch (param_0) {
    case 8:
    case 9:
    case 25:
        mpMorf[0]->setFrame(mpMorf[0]->getEndFrame());
        mpMorf[0]->setPlaySpeed(-1.0f);
        break;
    }
    return true;
}

int daNpc_Pouya_c::selectAction() {
    field_0xfb0 = NULL;

    switch (mType) {
    default:
        field_0xfb0 = &daNpc_Pouya_c::wait;
    }

    return 1;
}

BOOL daNpc_Pouya_c::chkAction(actionFunc param_0) {
    return mAction == param_0;
}

int daNpc_Pouya_c::setAction(actionFunc param_0) {
    mMode = MODE_EXIT;
    if (mAction != NULL) {
        (this->*(mAction))(NULL);
    }

    mMode = MODE_ENTER;
    mAction = param_0;
    if (mAction != NULL) {
        (this->*(mAction))(NULL);
    }

    return 1;
}

void daNpc_Pouya_c::setPrtcls() {
    JGeometry::TVec3<s16> emitter_rot;
    cXyz cStack_14;
    cXyz cStack_20(1.0f, 1.0f, 1.0f);

    mDoMtx_stack_c::copy(mpMorf[0]->getModel()->getAnmMtx(2));
    mDoMtx_stack_c::multVecZero(&cStack_14);

    field_0xfc8 = dComIfGp_particle_set(field_0xfc8, 0X8BFC, &cStack_14, &mCurAngle, &cStack_20);
    JPABaseEmitter* emitter = dComIfGp_particle_getEmitter(field_0xfc8);

    if (emitter != NULL) {
        emitter_rot.x = mCurAngle.x;
        emitter_rot.y = mCurAngle.y;
        emitter_rot.z = mCurAngle.z;
        emitter->setGlobalRotation(emitter_rot);
    }
}

int daNpc_Pouya_c::cutHaveFavorToAsk(int param_0) {
    cXyz acStack_48;
    csXyz cStack_50;

    int rv = 0;
    int uVar11 = -1;
    int uVar12 = 0;
    int local_60 = 0;
    f32 fVar1 = 0.0f;

    int* piVar1 = dComIfGp_evmng_getMyIntegerP(param_0, "prm");
    if (piVar1) {
        uVar11 = *piVar1;
    }

    piVar1 = dComIfGp_evmng_getMyIntegerP(param_0, "timer");
    if (piVar1) {
        uVar12 = *piVar1;
    }

    piVar1 = dComIfGp_evmng_getMyIntegerP(param_0, "msgNo");
    if (piVar1) {
        local_60 = *piVar1;
    }

    float* pfVar5 = dComIfGp_evmng_getMyFloatP(param_0, "rate");
    if (pfVar5) {
        fVar1 = *pfVar5;
    }

    if (dComIfGp_getEventManager().getIsAddvance(param_0)) {
        switch (uVar11) {
        case 0:
            initTalk(mFlowNodeNo, NULL);
            if (daNpcT_getPlayerInfoFromPlayerList(4, fopAcM_GetRoomNo(this), &acStack_48,
                                                   &cStack_50))
            {
                daPy_getPlayerActorClass()->setPlayerPosAndAngle(&acStack_48, cStack_50.y, 0);
            }
            break;
        case 2:
            mEventTimer = uVar12;
            break;
        case 3:
            mFaceMotionSeqMngr.setNo(FACE_MOT_UNK_6, -1.0f, 0, 0);
            mMotionSeqMngr.setNo(MOT_UNK_4, -1.0f, 0, 0);
            field_0xfcf = 0;
            if (daNpcT_getPlayerInfoFromPlayerList(4, fopAcM_GetRoomNo(this), &acStack_48,
                                                   &cStack_50))
            {
                daPy_getPlayerActorClass()->setPlayerPosAndAngle(&acStack_48, cStack_50.y, 0);
            }
            mEventTimer = uVar12;
            break;
        case 4:
            initTalk(mFlowNodeNo, NULL);
            break;
        case 5:
            mHide = 1;
            break;
        case 6:
            mFaceMotionSeqMngr.setNo(FACE_MOT_UNK_7, -1.0f, 0, 0);
            mMotionSeqMngr.setNo(MOT_UNK_10, -1.0f, 0, 0);
            mHide = 0;
            if (daNpcT_getPlayerInfoFromPlayerList(4, fopAcM_GetRoomNo(this), &acStack_48,
                                                   &cStack_50))
            {
                daPy_getPlayerActorClass()->setPlayerPosAndAngle(&acStack_48, cStack_50.y, 0);
            }
            mEventTimer = uVar12;
            break;
        case 10:
            mEventTimer = uVar12;
            if (mEventTimer == 0) {
                mDoAud_seStart(Z2SE_POUYA_RECOVER_FADE, NULL, 0, 0);
                mDoGph_gInf_c::fadeOut_f(fVar1, g_saftyWhiteColor);
            }
            break;
        case 11:
            mDoGph_gInf_c::fadeIn_f(fVar1, g_saftyWhiteColor);
            break;
        case 12:
            mFaceMotionSeqMngr.setNo(FACE_MOT_UNK_6, 0.0f, 0, 0);
            mMotionSeqMngr.setNo(MOT_UNK_4, 0.0f, 1, 0);
            mEventTimer = uVar12;
            if (mEventTimer == 0) {
                mDoAud_seStart(Z2SE_POUYA_RECOVER_FADE, NULL, 0, 0);
                mDoGph_gInf_c::fadeOut_f(fVar1, g_saftyWhiteColor);
            }
            break;
        }
    }

    int local_58[2] = {-1, -1};

    switch (uVar11) {
    case 0:
    case 1:
    case 4:
        mJntAnm.lookNone(0);
        local_58[0] = local_60;
        if (talkProc(local_58, 0, NULL, 0)) {
            if (local_60 == 0) {
                int local_64;
                int evt_id = mFlow.getEventId(&local_64);
                field_0xfce = 0;
                switch (evt_id) {
                case 1:
                    if (mItemPartnerId == fpcM_ERROR_PROCESS_ID_e) {
                        mItemPartnerId = fopAcM_createItemForPresentDemo(&current.pos, local_64, 0,
                                                                         -1, -1, 0, 0);
                    }
                    if (fopAcM_IsExecuting(mItemPartnerId)) {
                        field_0xfce = 1;
                        mMode = 1;
                        mEvtNo = 1;
                        evtChange();
                    }
                    break;
                default:
                    if (mFlow.checkEndFlow()) {
                        rv = 1;
                    }
                }
            } else {
                rv = 1;
            }
        }
        break;
    case 2:
    case 3:
    case 6:
        mJntAnm.lookNone(0);
        if (cLib_calcTimer(&mEventTimer) == 0) {
            rv = 1;
        }
        break;
    case 5:
        rv = 1;
    case 7:
    case 8:
    case 9:
        break;
    case 10:
    case 12:
        if (mEventTimer != 0) {
            if (cLib_calcTimer(&mEventTimer) == 0) {
                mDoAud_seStart(Z2SE_POUYA_RECOVER_FADE, NULL, 0, 0);
                mDoGph_gInf_c::fadeOut_f(fVar1, g_saftyWhiteColor);
            }
        } else if (1.0f <= mDoGph_gInf_c::getFadeRate()) {
            rv = 1;
        }
        if (uVar11 == 12 && mpMorf[0]->checkFrame(22.0f)) {
            mpMorf[0]->setPlaySpeed(0.0f);
            mBckAnm.setPlaySpeed(0.0f);
        }
        break;
    case 11:
        if (!mDoGph_gInf_c::isFade()) {
            rv = 1;
        }
        break;
    }

    return rv;
}

int daNpc_Pouya_c::wait(void* param_0) {
    switch (mMode) {
    case 0:
    case 1:
        if (!mStagger.checkStagger()) {
            if (chkPouyaB()) {
                if (mType == TYPE_2) {
                    mFaceMotionSeqMngr.setNo(FACE_MOT_UNK_11, -1.0f, 0, 0);
                    mMotionSeqMngr.setNo(MOT_UNK_15, -1.0f, 0, 0);
                } else if (field_0xfce != 0) {
                    mFaceMotionSeqMngr.setNo(FACE_MOT_UNK_14, -1.0f, 0, 0);
                    mMotionSeqMngr.setNo(MOT_UNK_8, -1.0f, 0, 0);
                } else {
                    mFaceMotionSeqMngr.setNo(FACE_MOT_UNK_7, -1.0f, 0, 0);
                    mMotionSeqMngr.setNo(MOT_UNK_10, -1.0f, 0, 0);
                }
            } else {
                if (mType == TYPE_1) {
                    if (field_0xfce != 0) {
                        mFaceMotionSeqMngr.setNo(FACE_MOT_UNK_6, -1.0f, 0, 0);
                        mMotionSeqMngr.setNo(MOT_UNK_1, -1.0f, 0, 0);
                    } else {
                        /* dSv_event_flag_c::F_0694 - Conversation with Jovani after collecting 20
                         * ghosts */
                        if (daNpcT_chkEvtBit(0x2B6)) {
                            mFaceMotionSeqMngr.setNo(FACE_MOT_UNK_6, -1.0f, 0, 0);
                            mMotionSeqMngr.setNo(MOT_UNK_4, -1.0f, 0, 0);
                        } else {
                            mFaceMotionSeqMngr.setNo(FACE_MOT_UNK_17, -1.0f, 0, 0);
                            mMotionSeqMngr.setNo(MOT_UNK_0, -1.0f, 0, 0);
                        }
                    }
                } else {
                    mFaceMotionSeqMngr.setNo(FACE_MOT_UNK_17, -1.0f, 0, 0);
                    mMotionSeqMngr.setNo(MOT_UNK_0, -1.0f, 0, 0);
                }
            }

            mMode = 2;
        }
    case 2:
        if (field_0xfce != 0) {
            if (mType == TYPE_3) {
                mEvtNo = 8;
                field_0xe33 = true;
            } else if (mType == TYPE_1) {
                mEvtNo = 6;
                field_0xe33 = true;
            }
        }

        /* dSv_event_flag_c::F_0456 - First time meeting with Jovani */
        if (mType == TYPE_1 && !daNpcT_chkEvtBit(0x1C8) && getBitSW() != 0xFF &&
            dComIfGs_isSwitch(getBitSW(), fopAcM_GetRoomNo(this)))
        {
            mEvtNo = 4;
            field_0xe33 = true;
        }

        if (!mStagger.checkStagger()) {
            if (mType == TYPE_1 || mType == TYPE_2 || mType == TYPE_3) {
                mPlayerActorMngr.remove();
            }
            if (mPlayerActorMngr.getActorP() != NULL && !mTwilight) {
                mJntAnm.lookPlayer(0);
                if (!chkActorInSight(mPlayerActorMngr.getActorP(), mAttnFovY, mCurAngle.y)) {
                    mJntAnm.lookNone(0);
                }
                if (!srchPlayerActor() && home.angle.y == mCurAngle.y) {
                    mMode = 1;
                }
            } else {
                mJntAnm.lookNone(0);
                if (home.angle.y != mCurAngle.y) {
                    if (field_0xe34 != 0) {
                        if (step(home.angle.y, -1, -1, 15, 0)) {
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
            mJntAnm.getMode();
        }
        break;
    case 3:
        break;
    }

    return 1;
}

int daNpc_Pouya_c::talk(void* param_0) {
    switch (mMode) {
    case 0:
    case 1:
        if (!mStagger.checkStagger()) {
            initTalk(mFlowNodeNo, NULL);
            mMode = 2;
        }
    case 2:
        if (!mStagger.checkStagger()) {
            if (mTwilight || mPlayerAngle == mCurAngle.y ||
                !(mType == 0 || mType == 4 || mType == 5))
            {
                if (talkProc(NULL, 0, NULL, 0)) {
                    int local_18;
                    int evt_id = mFlow.getEventId(&local_18);
                    field_0xfce = 0;
                    switch (evt_id) {
                    case 1:
                        if (mItemPartnerId == fpcM_ERROR_PROCESS_ID_e) {
                            mItemPartnerId = fopAcM_createItemForPresentDemo(&current.pos, local_18,
                                                                             0, -1, -1, 0, 0);
                        }
                        if (fopAcM_IsExecuting(mItemPartnerId)) {
                            field_0xfce = 1;
                            mEvtNo = 1;
                            evtChange();
                        }
                        break;
                    default:
                        if (mFlow.checkEndFlow()) {
                            mPlayerActorMngr.entry(daPy_getPlayerActorClass());
                            dComIfGp_event_reset();
                            mMode = 3;
                        }
                    }
                }
                mJntAnm.lookPlayer(0);
                if (!mTwilight && (mType == 0 || mType == 4 || mType == 5)) {
                    break;
                }
                mJntAnm.lookNone(0);

            } else {
                mJntAnm.lookPlayer(0);
                step(mPlayerAngle, -1, -1, 15, 0);
            }
        }
        break;
    case 3:
        break;
    }

    return 0;
}

static int daNpc_Pouya_Create(void* i_this) {
    return static_cast<daNpc_Pouya_c*>(i_this)->create();
}

static int daNpc_Pouya_Delete(void* i_this) {
    return static_cast<daNpc_Pouya_c*>(i_this)->Delete();
}

static int daNpc_Pouya_Execute(void* i_this) {
    return static_cast<daNpc_Pouya_c*>(i_this)->Execute();
}

static int daNpc_Pouya_Draw(void* i_this) {
    return static_cast<daNpc_Pouya_c*>(i_this)->Draw();
}

static bool daNpc_Pouya_IsDelete(void* i_this) {
    return 1;
}

NPC_POUYA_HIO_CLASS l_HIO;

static actor_method_class daNpc_Pouya_MethodTable = {
    (process_method_func)daNpc_Pouya_Create,  (process_method_func)daNpc_Pouya_Delete,
    (process_method_func)daNpc_Pouya_Execute, (process_method_func)daNpc_Pouya_IsDelete,
    (process_method_func)daNpc_Pouya_Draw,
};

extern actor_process_profile_definition g_profile_NPC_POUYA = {
    fpcLy_CURRENT_e,           // mLayerID
    7,                         // mListID
    fpcPi_CURRENT_e,           // mListPrio
    PROC_NPC_POUYA,            // mProcName
    &g_fpcLf_Method.base,      // sub_method
    sizeof(daNpc_Pouya_c),     // mSize
    0,                         // mSizeOther
    0,                         // mParameters
    &g_fopAc_Method.base,      // sub_method
    404,                       // mPriority
    &daNpc_Pouya_MethodTable,  // sub_method
    0x00040107,                // mStatus
    fopAc_NPC_e,               // mActorType
    fopAc_CULLBOX_CUSTOM_e,    // cullType
};
