/**
 * @file d_a_npc_kyury.cpp
 *
 */

#include "d/dolzel_rel.h"  // IWYU pragma: keep

#include "d/actor/d_a_npc_kyury.h"

const daNpc_Kyury_HIOParam daNpc_Kyury_Param_c::m = {
    200.0f,  // attention_offset
    -3.0f,   // gravity
    1.0f,    // scale
    400.0f,  // real_shadow_size
    255.0f,  // weight
    190.0f,  // height
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
daNpc_Kyury_HIO_c::daNpc_Kyury_HIO_c() {
    m = daNpc_Kyury_Param_c::m;
}

void daNpc_Kyury_HIO_c::listenPropertyEvent(const JORPropertyEvent* event) {
    // TODO
}

void daNpc_Kyury_HIO_c::genMessage(JORMContext* ctext) {
    // TODO
}
#endif

static int l_bmdData[3][2] = {
    {41, 1},
    {42, 1},
    {43, 1},
};

static daNpcT_evtData_c l_evtList[3] = {
    {"", 0},
    {"NO_RESPONSE", 0},
    {"FIRST_CONVERSATION", 2},
};

static char* l_resNameList[3] = {
    "",
    "Kyury",
    "Kyury1",
};

static s8 l_loadResPtrn0[2] = {1, -1};

static s8 l_loadResPtrn9[3] = {
    1,
    2,
    -1,
};

static s8* l_loadResPtrnList[4] = {
    l_loadResPtrn9,
    l_loadResPtrn9,
    l_loadResPtrn9,
    l_loadResPtrn9,
};

static daNpcT_faceMotionAnmData_c l_faceMotionAnmData[17] = {
    {-1, 0, 0, 50, 2, 1, 1}, {22, 0, 1, 50, 2, 1, 1}, {23, 0, 1, 56, 0, 1, 0},
    {24, 0, 1, 57, 0, 1, 0}, {19, 0, 1, 53, 0, 1, 0}, {21, 0, 1, 55, 0, 1, 0},
    {25, 2, 1, 50, 2, 1, 1}, {27, 2, 1, 59, 2, 1, 0}, {11, 2, 1, 50, 2, 1, 1},
    {10, 2, 1, 50, 2, 1, 1}, {6, 2, 1, 50, 2, 1, 1},  {7, 2, 1, 50, 2, 1, 1},
    {8, 0, 1, 51, 0, 1, 0},  {9, 2, 1, 52, 2, 1, 0},  {12, 2, 1, 50, 2, 1, 1},
    {20, 0, 1, 54, 0, 1, 0}, {26, 2, 1, 58, 2, 1, 0},
};

static daNpcT_motionAnmData_c l_motionAnmData[16] = {
    {36, 2, 1, 46, 0, 1, 1, 0}, {37, 2, 1, 46, 0, 1, 1, 0}, {33, 0, 1, 46, 0, 1, 1, 0},
    {34, 0, 1, 46, 0, 1, 1, 0}, {35, 0, 1, 46, 0, 1, 1, 0}, {38, 2, 1, 46, 0, 1, 1, 0},
    {28, 0, 1, 46, 0, 1, 1, 0}, {29, 0, 1, 46, 0, 1, 1, 0}, {18, 2, 1, 46, 0, 1, 1, 0},
    {17, 2, 1, 46, 0, 1, 1, 0}, {13, 0, 1, 46, 0, 1, 1, 0}, {14, 0, 1, 46, 0, 1, 1, 0},
    {15, 0, 1, 46, 0, 1, 1, 0}, {16, 2, 1, 46, 0, 1, 1, 0}, {30, 2, 1, 46, 0, 1, 1, 0},
    {31, 0, 1, 46, 0, 1, 1, 0},
};

static daNpcT_MotionSeqMngr_c::sequenceStepData_c l_faceMotionSequenceData[64] = {
    {1, -1, 1},  {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},  {2, -1, 1},  {7, 0, 0},  {-1, 0, 0},
    {-1, 0, 0},  {4, -1, 1}, {-1, 0, 0}, {-1, 0, 0},  {-1, 0, 0},  {5, -1, 1}, {-1, 0, 0},
    {-1, 0, 0},  {-1, 0, 0}, {3, -1, 1}, {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0}, {15, -1, 1},
    {16, 0, 0},  {-1, 0, 0}, {-1, 0, 0}, {11, -1, 1}, {9, 0, 0},   {-1, 0, 0}, {-1, 0, 0},
    {12, -1, 1}, {14, 0, 0}, {-1, 0, 0}, {-1, 0, 0},  {13, -1, 1}, {14, 0, 0}, {-1, 0, 0},
    {-1, 0, 0},  {7, -1, 0}, {-1, 0, 0}, {-1, 0, 0},  {-1, 0, 0},  {8, -1, 0}, {-1, 0, 0},
    {-1, 0, 0},  {-1, 0, 0}, {10, 4, 1}, {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0}, {9, -1, 0},
    {-1, 0, 0},  {-1, 0, 0}, {-1, 0, 0}, {16, -1, 0}, {-1, 0, 0},  {-1, 0, 0}, {-1, 0, 0},
    {14, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},  {0, -1, 0},  {-1, 0, 0}, {-1, 0, 0},
    {-1, 0, 0},
};

static daNpcT_MotionSeqMngr_c::sequenceStepData_c l_motionSequenceData[52] = {
    {0, -1, 0},  {-1, 0, 0},  {-1, 0, 0}, {-1, 0, 0},  {2, -1, 1},  {0, 0, 0},   {-1, 0, 0},
    {-1, 0, 0},  {1, -1, 0},  {-1, 0, 0}, {-1, 0, 0},  {-1, 0, 0},  {3, -1, 1},  {0, 0, 0},
    {-1, 0, 0},  {-1, 0, 0},  {4, -1, 1}, {0, 0, 0},   {-1, 0, 0},  {-1, 0, 0},  {9, -1, 0},
    {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0}, {11, -1, 1}, {9, 0, 0},   {-1, 0, 0},  {-1, 0, 0},
    {15, -1, 1}, {14, 0, 0},  {-1, 0, 0}, {-1, 0, 0},  {14, -1, 0}, {-1, 0, 0},  {-1, 0, 0},
    {-1, 0, 0},  {12, -1, 1}, {13, 0, 0}, {-1, 0, 0},  {-1, 0, 0},  {13, -1, 0}, {-1, 0, 0},
    {-1, 0, 0},  {-1, 0, 0},  {10, 4, 1}, {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},  {8, -1, 0},
    {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},
};

char* daNpc_Kyury_c::mCutNameList[2] = {
    "",
    "CONVERSATION",
};

daNpc_Kyury_c::cutFunc daNpc_Kyury_c::mCutList[2] = {
    NULL,
    &daNpc_Kyury_c::cutConversation,
};

daNpc_Kyury_c::~daNpc_Kyury_c() {
    if (mpMorf[0] != 0) {
        mpMorf[0]->stopZelAnime();
    }
    deleteRes(l_loadResPtrnList[mType], (char const**)l_resNameList);
}

int daNpc_Kyury_c::create() {
    daNpcT_ct(this, daNpc_Kyury_c, l_faceMotionAnmData, l_motionAnmData, l_faceMotionSequenceData,
              4, l_motionSequenceData, 4, l_evtList, l_resNameList);

    mType = getType();
    mFlowNodeNo = getFlowNodeNo();
    mTwilight = 0;

    int phase = loadRes(l_loadResPtrnList[mType], (const char**)l_resNameList);
    if (phase == cPhs_COMPLEATE_e) {
        if (isDelete()) {
            return cPhs_ERROR_e;
        }
        if (!fopAcM_entrySolidHeap(this, createHeapCallBack, 0x4890)) {
            return cPhs_ERROR_e;
        }
        J3DModelData* modelData = mpMorf[0]->getModel()->getModelData();
        fopAcM_SetMtx(this, mpMorf[0]->getModel()->getBaseTRMtx());
        fopAcM_setCullSizeBox(this, -200.0f, -100.0f, -200.0f, 200.0f, 300.0f, 200.0f);
        mSound.init(&current.pos, &eyePos, 3, 1);
        reset();
        mAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1, &mAcchCir,
                  fopAcM_GetSpeed_p(this), fopAcM_GetAngle_p(this), fopAcM_GetShapeAngle_p(this));
        mCcStts.Init(daNpc_Kyury_Param_c::m.common.weight, 0, this);
        mCyl.Set(mCcDCyl);
        mCyl.SetStts(&mCcStts);
        mCyl.SetTgHitCallback(tgHitCallBack);
        if (mType == TYPE_1) {
            mAcch.SetGrndNone();
            mAcch.SetWallNone();
        }
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

    return phase;
}

int daNpc_Kyury_c::CreateHeap() {
    static int const bmdTypeList[2] = {1, 2};

    J3DModelData* modelData = static_cast<J3DModelData*>(
        dComIfG_getObjectRes(l_resNameList[l_bmdData[0][1]], l_bmdData[0][0]));
    if (modelData == NULL) {
        return 0;
    }

    mpMorf[0] = new mDoExt_McaMorfSO(modelData, NULL, NULL, NULL, -1, 1.0f, 0, -1, &mSound, 0x80000,
                                     0x11020284);
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

    for (int i = 0; i < 2; i++) {
        if (l_bmdData[bmdTypeList[i]][0] >= 0) {
            modelData = (J3DModelData*)dComIfG_getObjectRes(
                l_resNameList[l_bmdData[bmdTypeList[i]][1]], l_bmdData[bmdTypeList[i]][0]);
        } else {
            modelData = NULL;
        }
        if (modelData != NULL) {
            mpModel[i] = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
        } else {
            mpModel[i] = NULL;
        }
    }

    if (setFaceMotionAnm(FACE_MOT_UNK_1, false) && setMotionAnm(MOT_UNK_0, 0.0f, 0)) {
        return 1;
    }

    return 0;
}

int daNpc_Kyury_c::Delete() {
    fopAcM_GetID(this);
    this->~daNpc_Kyury_c();
    return 1;
}

int daNpc_Kyury_c::Execute() {
    return daNpcT_c::execute();
}

int daNpc_Kyury_c::Draw() {
    if (mpMatAnm[0] != NULL) {
        J3DModelData* modelData = mpMorf[0]->getModel()->getModelData();
        modelData->getMaterialNodePointer(getEyeballMaterialNo())->setMaterialAnm(mpMatAnm[0]);
    }

    return daNpcT_c::draw(0, 0, mRealShadowSize, NULL, 100.0f, 0, 0, 0);
}

int daNpc_Kyury_c::createHeapCallBack(fopAc_ac_c* i_this) {
    return static_cast<daNpc_Kyury_c*>(i_this)->CreateHeap();
}

int daNpc_Kyury_c::ctrlJointCallBack(J3DJoint* param_0, int param_1) {
    if (param_1 == 0) {
        J3DModel* model = j3dSys.getModel();

        daNpc_Kyury_c* i_this = reinterpret_cast<daNpc_Kyury_c*>(model->getUserArea());
        if (i_this != 0) {
            i_this->ctrlJoint(param_0, model);
        }
    }

    return 1;
}

u8 daNpc_Kyury_c::getType() {
    switch (fopAcM_GetParam(this) & 0xFF) {
    case 0:
        return TYPE_0;
    case 1:
        return TYPE_1;
    case 2:
        return TYPE_2;
    }
    return TYPE_3;
}

int daNpc_Kyury_c::isDelete() {
    switch (mType) {
    case TYPE_0:
        return FALSE;
    case TYPE_1:
        return FALSE;
    case TYPE_2:
        return FALSE;
    default:
        return FALSE;
    }
}

void daNpc_Kyury_c::reset() {
    int iVar1 = (u8*)&field_0xfd8 - (u8*)&field_0xfbc[0];

    if (mpMatAnm[0] != NULL) {
        mpMatAnm[0]->initialize();
    }
    initialize();
    for (int i = 0; i < 1; i++) {
        mActorMngr[i].initialize();
    }

    memset(&field_0xfbc[0], 0, iVar1);

    switch (mType) {
    case TYPE_0:
        field_0xfd4 = 1;
        field_0xfd5 = 1;
        break;
    case TYPE_1:
    case TYPE_2:
    case TYPE_3:
        break;
    }

    setAngle(home.angle.y);
}

void daNpc_Kyury_c::afterJntAnm(int param_1) {
    if (param_1 == 1) {
        mDoMtx_stack_c::YrotM(-mStagger.getAngleZ(1));
        mDoMtx_stack_c::ZrotM(mStagger.getAngleX(1));
    } else if (param_1 == 4) {
        mDoMtx_stack_c::YrotM(-mStagger.getAngleZ(0));
        mDoMtx_stack_c::ZrotM(mStagger.getAngleX(0));
    }
}

void daNpc_Kyury_c::setParam() {
    selectAction();
    srchActors();

    s16 talk_distance = mpHIO->m.common.talk_distance;
    s16 talk_angle = mpHIO->m.common.talk_angle;
    s16 attention_distance = mpHIO->m.common.attention_distance;
    s16 attention_angle = mpHIO->m.common.attention_angle;

    attention_info.distances[fopAc_attn_LOCK_e] =
        daNpcT_getDistTableIdx(attention_distance, attention_angle);
    attention_info.distances[fopAc_attn_TALK_e] = attention_info.distances[fopAc_attn_LOCK_e];
    attention_info.distances[fopAc_attn_SPEAK_e] =
        daNpcT_getDistTableIdx(talk_distance, talk_angle);
    attention_info.flags = (fopAc_AttnFlag_SPEAK_e | fopAc_AttnFlag_TALK_e);
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
    if (mType == TYPE_1) {
        mAcch.SetGrndNone();
        mAcch.SetWallNone();
        gravity = 0.0f;
    }
}

BOOL daNpc_Kyury_c::checkChangeEvt() {
    if (!chkAction(&daNpc_Kyury_c::talk)) {
        mPreItemNo = 0;

        if (dComIfGp_event_chkTalkXY()) {
            if (dComIfGp_evmng_ChkPresentEnd()) {
                mEvtNo = 1;
                evtChange();
            }
            return TRUE;
        }

        switch (mType) {
        case TYPE_0:
            if (!daNpcT_chkEvtBit(0x2D) && !daNpcT_chkEvtBit(0x1A)) {
                mEvtNo = 2;
                evtChange();
                return TRUE;
            }
            break;
        case TYPE_1:
        case TYPE_2:
        case TYPE_3:
            break;
        }
    }
    return FALSE;
}

void daNpc_Kyury_c::setAfterTalkMotion() {
    int iVar2 = FACE_MOT_UNK_12;
    switch (mFaceMotionSeqMngr.getNo()) {
    case FACE_MOT_UNK_1:
        iVar2 = FACE_MOT_UNK_9;
        break;
    case FACE_MOT_UNK_5:
        iVar2 = FACE_MOT_UNK_13;
        break;
    case FACE_MOT_UNK_6:
        iVar2 = FACE_MOT_UNK_12;
        break;
    case FACE_MOT_UNK_7:
    case FACE_MOT_UNK_8:
        iVar2 = FACE_MOT_UNK_14;
        break;
    }
    mFaceMotionSeqMngr.setNo(iVar2, -1.0f, 0, 0);
}

void daNpc_Kyury_c::srchActors() {
    switch (mType) {
    case TYPE_0:
        if (!mActorMngr[0].getActorP()) {
            mActorMngr[0].entry(getNearestActorP(0x21C));
        }
        break;
    case TYPE_1:
    case TYPE_2:
    case TYPE_3:
        break;
    }
}

BOOL daNpc_Kyury_c::evtTalk() {
    if (chkAction(&daNpc_Kyury_c::talk)) {
        (this->*field_0xfbc[1])(NULL);
    } else {
        setAction(&daNpc_Kyury_c::talk);
    }
    return 1;
}

BOOL daNpc_Kyury_c::evtCutProc() {
    int staffId = dComIfGp_getEventManager().getMyStaffId("Kyury", this, -1);
    if (staffId != -1) {
        mStaffId = staffId;
        int actIdx =
            dComIfGp_getEventManager().getMyActIdx(mStaffId, (char**)mCutNameList, 2, 0, 0);
        if ((this->*(mCutList[actIdx]))(mStaffId) != 0) {
            dComIfGp_getEventManager().cutEnd(mStaffId);
        }
        return true;
    }
    return false;
}

void daNpc_Kyury_c::action() {
    fopAc_ac_c* hit_actor = hitChk(&mCyl, 0xFFFFFFFF);

    if (mStagger.checkRebirth()) {
        mStagger.initialize();
        mMode = MODE_INIT;
    }

    if (field_0xfbc[0] != NULL) {
        if (field_0xfbc[1] == field_0xfbc[0]) {
            (this->*field_0xfbc[1])(NULL);
        } else {
            setAction(field_0xfbc[0]);
        }
    }
}

void daNpc_Kyury_c::beforeMove() {
    fopAcM_OffStatus(this, 0x8000000);
    if (checkHide()) {
        fopAcM_OnStatus(this, 0x8000000);
    }

    if (checkHide() || mNoDraw != 0) {
        attention_info.flags = 0;
    }
}

void daNpc_Kyury_c::setAttnPos() {
    cXyz acStack_40(10.0f, 30.0f, 0.0f);
    cXyz cStack_4c(10.0f, 0.0f, 0.0f);

    mStagger.calc(0);

    f32 dVar6 = cM_s2rad(mCurAngle.y - field_0xd7e.y);

    if (mType == TYPE_1) {
        mJntAnm.setParam(this, mpMorf[0]->getModel(), &acStack_40, getBackboneJointNo(),
                         getNeckJointNo(), getHeadJointNo(), 0.0f, 0.0f, 0.0f, 0.0f,
                         mpHIO->m.common.head_angleX_min, mpHIO->m.common.head_angleX_max,
                         mpHIO->m.common.head_angleY_min, mpHIO->m.common.head_angleY_max,
                         mpHIO->m.common.neck_rotation_ratio, dVar6, &cStack_4c);
    } else {
        mJntAnm.setParam(this, mpMorf[0]->getModel(), &acStack_40, getBackboneJointNo(),
                         getNeckJointNo(), getHeadJointNo(), mpHIO->m.common.body_angleX_min,
                         mpHIO->m.common.body_angleX_max, mpHIO->m.common.body_angleY_min,
                         mpHIO->m.common.body_angleY_max, mpHIO->m.common.head_angleX_min,
                         mpHIO->m.common.head_angleX_max, mpHIO->m.common.head_angleY_min,
                         mpHIO->m.common.head_angleY_max, mpHIO->m.common.neck_rotation_ratio,
                         dVar6, &cStack_4c);
    }
    mJntAnm.calcJntRad(0.2f, 1.0f, dVar6);

    setMtx();
    mDoMtx_stack_c::copy(mpMorf[0]->getModel()->getAnmMtx(getHeadJointNo()));
    mDoMtx_stack_c::multVec(&acStack_40, &eyePos);

    mJntAnm.setEyeAngleX(eyePos, 1.0f, 0);
    mJntAnm.setEyeAngleY(eyePos, mCurAngle.y, 1, 1.0f, 0);

    acStack_40.set(0.0f, 0.0f, 14.0f);
    acStack_40.y = mpHIO->m.common.attention_offset;

    if (mType == TYPE_1) {
        acStack_40.set(0.0f, 180.0f, 0.0f);
    }

    mDoMtx_stack_c::YrotS(mCurAngle.y);
    mDoMtx_stack_c::multVec(&acStack_40, &acStack_40);
    attention_info.position = current.pos + acStack_40;
}

void daNpc_Kyury_c::setCollision() {
    cXyz cStack_48;

    if (mHide == 0) {
        u32 tgType = 0xd8fbfdff;
        u32 tgSPrm = 0x1f;

        if (mTwilight != 0) {
            tgType = 0;
            tgSPrm = 0;
        } else {
            if (mStagger.checkStagger()) {
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
        if (mType == TYPE_1) {
            cStack_48.set(0.0f, 0.0f, -10.0f);
        }
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

int daNpc_Kyury_c::drawDbgInfo() {
    return 0;
}

void daNpc_Kyury_c::drawOtherMdl() {
    static int const jointNo[2] = {19, 14};

    Mtx mtx;
    J3DModel* model = mpMorf[0]->getModel();

    for (int i = 0; i < 2; i++) {
        if (mpModel[i] != NULL && ((i == 0 && field_0xfd4 != 0) || (i == 1 && field_0xfd5 != 0))) {
            g_env_light.setLightTevColorType_MAJI(mpModel[i], &tevStr);
            mDoMtx_stack_c::copy(model->getAnmMtx(jointNo[i]));
            cMtx_copy(mDoMtx_stack_c::get(), mtx);
            mpModel[i]->setBaseTRMtx(mtx);
            mDoExt_modelUpdateDL(mpModel[i]);
            dComIfGd_addRealShadow(mShadowKey, mpModel[i]);
        }
    }
}

int daNpc_Kyury_c::selectAction() {
    field_0xfbc[0] = NULL;

    switch (mType) {
    default:
        field_0xfbc[0] = &daNpc_Kyury_c::wait;
    }

    return 1;
}

int daNpc_Kyury_c::chkAction(int (daNpc_Kyury_c::*action)(void*)) {
    return field_0xfbc[1] == action;
}

int daNpc_Kyury_c::setAction(int (daNpc_Kyury_c::*action)(void*)) {
    mMode = MODE_EXIT;
    if (field_0xfbc[1] != NULL) {
        (this->*field_0xfbc[1])(NULL);
    }
    mMode = MODE_ENTER;
    field_0xfbc[1] = action;
    if (field_0xfbc[1] != NULL) {
        (this->*field_0xfbc[1])(NULL);
    }

    return 1;
}

int daNpc_Kyury_c::cutConversation(int param_0) {
    fopAc_ac_c* actor_p;
    cXyz acStack_48;
    csXyz cStack_50;

    int rv = 0;
    int local_34 = -1;
    int iVar9 = 0;
    int iVar8 = 0;
    int local_38 = 0;

    int* piVar1 = dComIfGp_evmng_getMyIntegerP(param_0, "prm");
    if (piVar1) {
        local_34 = *piVar1;
    }

    piVar1 = dComIfGp_evmng_getMyIntegerP(param_0, "msgNo");
    if (piVar1) {
        iVar9 = *piVar1;
    }

    piVar1 = dComIfGp_evmng_getMyIntegerP(param_0, "msgNo2");
    if (piVar1) {
        iVar8 = *piVar1;
    }

    piVar1 = dComIfGp_evmng_getMyIntegerP(param_0, "send");
    if (piVar1) {
        local_38 = *piVar1;
    }

    if (dComIfGp_getEventManager().getIsAddvance(param_0)) {
        switch (local_34) {
        case 0:
            actor_p = mActorMngr[0].getActorP();
            dComIfGp_getEvent().setPt2(actor_p);
            mPlayerAngle = fopAcM_searchPlayerAngleY(this);
            if (checkStep()) {
                mStepMode = 0;
            }
            break;
        case 1:
            mFaceMotionSeqMngr.setNo(FACE_MOT_UNK_12, -1.0f, 0, 0);
            mMotionSeqMngr.setNo(MOT_UNK_5, -1.0f, 0, 0);
            initTalk(mFlowNodeNo, NULL);
            break;
        case 2:
        case 4:
            break;
        }
    }

    int local_28[3] = {-1, -1, -1};

    switch (local_34) {
    case 0:
        mJntAnm.lookPlayer(0);

        if (mPlayerAngle == mCurAngle.y) {
            rv = 1;
        } else {
            step(mPlayerAngle, 11, 11, 15, 0);
        }
        break;
    case 1:
        local_28[0] = iVar9;
        local_28[1] = iVar8;
        if (talkProc(local_28, 0, NULL, 0)) {
            if (iVar9 == 0 && iVar8 == 0) {
                if (mFlow.checkEndFlow()) {
                    rv = 1;
                }
            } else {
                rv = 1;
            }
        }
        break;
    case 2:
        actor_p = mActorMngr[0].getActorP();
        mJntAnm.lookPos(&actor_p->attention_info.position, 0);
    case 4:
        mJntAnm.lookPlayer(0);
        local_28[0] = iVar9;
        local_28[1] = iVar8;
        if (talkProc(local_28, local_38, NULL, 0)) {
            if (iVar9 == 0 && iVar8 == 0) {
                if (mFlow.checkEndFlow()) {
                    mPlayerActorMngr.entry(daPy_getPlayerActorClass());
                    rv = 1;
                }
            } else {
                rv = 1;
            }
        }
        break;
    }

    return rv;
}

int daNpc_Kyury_c::wait(void* param_0) {
    switch (mMode) {
    case MODE_ENTER:
    case MODE_INIT:
        if (!mStagger.checkStagger()) {
            switch (mType) {
            case TYPE_0:
                if (mPlayerActorMngr.getActorP()) {
                    mFaceMotionSeqMngr.setNo(FACE_MOT_UNK_12, -1.0f, 0, 0);
                    mMotionSeqMngr.setNo(MOT_UNK_5, -1.0f, 0, 0);
                } else {
                    mFaceMotionSeqMngr.setNo(FACE_MOT_UNK_10, -1.0f, 0, 0);
                    mMotionSeqMngr.setNo(MOT_UNK_12, -1.0f, 0, 0);
                }
                break;
            case TYPE_1:
                mFaceMotionSeqMngr.setNo(FACE_MOT_UNK_13, -1.0f, 0, 0);
                mMotionSeqMngr.setNo(MOT_UNK_8, -1.0f, 0, 0);
                break;
            default:
                mFaceMotionSeqMngr.setNo(FACE_MOT_UNK_15, -1.0f, 0, 0);
                mMotionSeqMngr.setNo(MOT_UNK_0, -1.0f, 0, 0);
                break;
            }
            mMode = MODE_RUN;
        }
    case MODE_RUN:
        if (!mStagger.checkStagger()) {
            if (mType == TYPE_1) {
                mPlayerActorMngr.remove();
            }
            if (mPlayerActorMngr.getActorP()) {
                mJntAnm.lookPlayer(0);
                if (!chkActorInSight(mPlayerActorMngr.getActorP(), mAttnFovY, mCurAngle.y)) {
                    mJntAnm.lookNone(0);
                }
                if (!srchPlayerActor() && home.angle.y == mCurAngle.y) {
                    mMode = MODE_INIT;
                }
            } else {
                mJntAnm.lookNone(0);
                if (mType == TYPE_0) {
                    field_0xd6c.set(0.0f, 120.0f, 50.0f);
                    mDoMtx_stack_c::YrotS(mCurAngle.y);
                    mDoMtx_stack_c::multVec(&field_0xd6c, &field_0xd6c);
                    field_0xd6c += current.pos;
                    mJntAnm.lookPos(&field_0xd6c, 0);
                }
                if (home.angle.y != mCurAngle.y) {
                    if (field_0xe34 != 0) {
                        if (mType == TYPE_0) {
                            if (step(home.angle.y, 11, 11, 15, 0)) {
                                mMode = MODE_INIT;
                            }
                        } else {
                            if (step(home.angle.y, -1, -1, 15, 0)) {
                                mMode = MODE_INIT;
                            }
                        }
                    } else {
                        setAngle(home.angle.y);
                        mMode = MODE_INIT;
                    }
                    attention_info.flags = 0;
                } else if (mType != TYPE_0) {
                    srchPlayerActor();
                }
            }
            mJntAnm.getMode();
        }
        break;
    case MODE_EXIT:
    case MODE_4:
        break;
    }
    return 1;
}

int daNpc_Kyury_c::talk(void* param_0) {
    switch (mMode) {
    case MODE_ENTER:
    case MODE_INIT:
        if (!mStagger.checkStagger()) {
            initTalk(mFlowNodeNo, NULL);
            mMode = MODE_RUN;
        }
    case MODE_RUN:
        if (!mStagger.checkStagger()) {
            if (mTwilight || mPlayerAngle == mCurAngle.y || mType == TYPE_1) {
                if (talkProc(NULL, 0, NULL, 0) && mFlow.checkEndFlow() != 0) {
                    mPlayerActorMngr.entry(daPy_getPlayerActorClass());
                    dComIfGp_event_reset();
                    mMode = MODE_EXIT;
                }
                mJntAnm.lookPlayer(0);
                if (mTwilight != 0) {
                    mJntAnm.lookNone(0);
                }
            } else {
                mJntAnm.lookPlayer(0);
                if (mType == TYPE_0) {
                    step(mPlayerAngle, 11, 11, 15, 0);
                } else {
                    step(mPlayerAngle, -1, -1, 15, 0);
                }
            }
        }
        break;
    case MODE_EXIT:
    case MODE_4:
        break;
    }

    return 0;
}

static int daNpc_Kyury_Create(void* i_this) {
    return static_cast<daNpc_Kyury_c*>(i_this)->create();
}

static int daNpc_Kyury_Delete(void* i_this) {
    return static_cast<daNpc_Kyury_c*>(i_this)->Delete();
}

static int daNpc_Kyury_Execute(void* i_this) {
    return static_cast<daNpc_Kyury_c*>(i_this)->Execute();
}

static int daNpc_Kyury_Draw(void* i_this) {
    return static_cast<daNpc_Kyury_c*>(i_this)->Draw();
}

static int daNpc_Kyury_IsDelete(void* i_this) {
    return 1;
}

NPC_KYURY_HIO_CLASS l_HIO;

static actor_method_class daNpc_Kyury_MethodTable = {
    (process_method_func)daNpc_Kyury_Create,  (process_method_func)daNpc_Kyury_Delete,
    (process_method_func)daNpc_Kyury_Execute, (process_method_func)daNpc_Kyury_IsDelete,
    (process_method_func)daNpc_Kyury_Draw,
};

extern actor_process_profile_definition g_profile_NPC_KYURY = {
    fpcLy_CURRENT_e,           // mLayerID
    7,                         // mListID
    fpcPi_CURRENT_e,           // mListPrio
    PROC_NPC_KYURY,            // mProcName
    &g_fpcLf_Method.base,      // sub_method
    sizeof(daNpc_Kyury_c),     // mSize
    0,                         // mSizeOther
    0,                         // mParameters
    &g_fopAc_Method.base,      // sub_method
    352,                       // mPriority
    &daNpc_Kyury_MethodTable,  // sub_method
    0x00040108,                // mStatus
    fopAc_NPC_e,               // mActorType
    fopAc_CULLBOX_CUSTOM_e,    // cullType
};
