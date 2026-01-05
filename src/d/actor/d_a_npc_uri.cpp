/**
 * @file d_a_npc_uri.cpp
 *
 */

#include "d/dolzel_rel.h"  // IWYU pragma: keep

#include "Z2AudioLib/Z2Instances.h"
#include "d/actor/d_a_npc_uri.h"
#include "m_Do/m_Do_graphic.h"

const daNpc_Uri_HIOParam daNpc_Uri_Param_c::m = {
    200.0f,   // attention_offset
    -3.0f,    // gravity
    1.0f,     // scale
    500.0f,   // real_shadow_size
    255.0f,   // weight
    180.0f,   // height
    35.0f,    // knee_length
    30.0f,    // width
    0.0f,     // body_angleX_max
    0.0f,     // body_angleX_min
    10.0f,    // body_angleY_max
    -10.0f,   // body_angleY_min
    30.0f,    // head_angleX_max
    -10.0f,   // head_angleX_min
    45.0f,    // head_angleY_max
    -45.0f,   // head_angleY_min
    0.6f,     // neck_rotation_ratio
    20.0f,    // morf_frame
    3,        // talk_distance
    6,        // talk_angle
    5,        // attention_distance
    6,        // attention_angle
    110.0f,   // fov
    400.0f,   // search_distance
    300.0f,   // search_height
    -300.0f,  // search_depth
    60,       // attention_time
    8,        // damage_time
    0,        // face_expression
    0,        // motion
    0,        // look_mode
    0,        // debug_mode_ON
    0,        // debug_info_ON
    4.0f,     // expression_morf_frame
    0.0f,     // box_min_x
    0.0f,     // box_min_y
    0.0f,     // box_min_z
    0.0f,     // box_max_x
    0.0f,     // box_max_y
    0.0f,     // box_max_z
    0.0f,     // box_offset
    120,      // field_0x8c
    120,      // field_0x8e
    60,       // field_0x90
    180,      // field_0x92
    10,       // field_0x94
    0,        // field_0x96
    350.0f,   // field_0x98
    400.0f,   // field_0x9c
    3000.0f,  // field_0xa0
    200.0f,   // field_0xa4
    600.0f,   // field_0xa8
};

#if DEBUG
daNpc_Uri_HIO_c::daNpc_Uri_HIO_c() {
    m = daNpc_Uri_Param_c::m;
}

void daNpc_Uri_HIO_c::listenPropertyEvent(const JORPropertyEvent* event) {
    // TODO
}

void daNpc_Uri_HIO_c::genMessage(JORMContext* ctext) {
    // TODO
}
#endif

static int l_bmdData[2][2] = {{12, 1}, {33, 2}};

static daNpcT_evtData_c l_evtList[11] = {
    {"", 0},
    {"DEFAULT_GETITEM", 2},
    {"NO_RESPONSE", 0},
    {"FIRST_CONVERSATION", 2},
    {"CONVERSATION", 2},
    {"START_CARRY_TUTORIAL", 2},
    {"END_CARRY_TUTORIAL", 2},
    {"FAILURE_CARRY_TUTORIAL", 2},
    {"CONVERSATION_WITH_MOI", 3},
    {"FIND_WOLF", 3},
    {"MEETING_AGAIN", 4},
};

static char* l_resNameList[6] = {
    "", "Uri", "Uri1", "Uri2", "Uri3", "Uri_P1",
};

static s8 l_loadResPtrn0[2] = {1, -1};

static s8 l_loadResPtrn1[3] = {
    1,
    2,
    -1,
};

static s8 l_loadResPtrn2[3] = {
    1,
    3,
    -1,
};

static s8 l_loadResPtrn3[5] = {
    1, 2, 3, 4, -1,
};

static s8 l_loadResPtrn4[3] = {
    1,
    5,
    -1,
};

static s8 l_loadResPtrn9[5] = {
    1, 2, 3, 4, -1,
};

static s8* l_loadResPtrnList[7] = {l_loadResPtrn0, l_loadResPtrn1, l_loadResPtrn0, l_loadResPtrn2,
                                   l_loadResPtrn3, l_loadResPtrn4, l_loadResPtrn9};

static daNpcT_faceMotionAnmData_c l_faceMotionAnmData[19] = {
    {-1, 0, 0, 18, 2, 1, TRUE},  {-1, 0, 0, -1, 0, 0, FALSE}, {6, 0, 1, 18, 2, 1, TRUE},
    {10, 0, 2, 43, 0, 2, FALSE}, {11, 0, 2, 44, 0, 2, FALSE}, {9, 0, 2, 42, 0, 2, FALSE},
    {12, 0, 2, 45, 0, 2, FALSE}, {6, 0, 3, 21, 0, 3, FALSE},  {8, 0, 3, 23, 0, 3, FALSE},
    {15, 2, 2, 18, 2, 1, TRUE},  {7, 2, 2, 18, 2, 1, TRUE},   {14, 2, 2, 47, 2, 2, FALSE},
    {13, 2, 2, 46, 2, 2, FALSE}, {8, 2, 2, 41, 2, 2, FALSE},  {10, 2, 3, 18, 2, 1, TRUE},
    {7, 0, 3, 22, 0, 3, FALSE},  {11, 2, 3, 18, 2, 1, TRUE},  {7, 0, 1, 20, 0, 1, FALSE},
    {8, 2, 1, 21, 2, 1, FALSE},
};

static daNpcT_motionAnmData_c l_motionAnmData[34] = {
    {9, 2, 1, 15, 0, 1, 1, 0},  {-1, 0, 0, -1, 0, 0, 0, 0}, {9, 2, 3, 18, 2, 3, 0, 0},
    {-1, 0, 0, -1, 0, 0, 0, 0}, {-1, 0, 0, -1, 0, 0, 0, 0}, {-1, 0, 0, -1, 0, 0, 0, 0},
    {-1, 0, 0, -1, 0, 0, 0, 0}, {-1, 0, 0, -1, 0, 0, 0, 0}, {30, 0, 2, 15, 0, 1, 1, 0},
    {27, 2, 2, 15, 0, 1, 1, 0}, {28, 0, 2, 15, 0, 1, 1, 0}, {29, 2, 2, 38, 2, 2, 0, 0},
    {18, 2, 2, 36, 2, 2, 0, 0}, {26, 2, 2, 37, 2, 2, 0, 0}, {22, 2, 2, 15, 0, 1, 1, 0},
    {23, 2, 2, 15, 0, 1, 1, 0}, {24, 2, 2, 15, 0, 1, 1, 0}, {25, 2, 2, 15, 0, 1, 1, 0},
    {20, 0, 2, 15, 0, 1, 1, 0}, {21, 0, 2, 15, 0, 1, 1, 0}, {16, 0, 2, 15, 0, 1, 1, 0},
    {17, 2, 2, 15, 0, 1, 1, 0}, {12, 0, 3, 15, 0, 1, 1, 0}, {13, 2, 3, 15, 0, 1, 1, 0},
    {14, 0, 3, 15, 0, 1, 1, 0}, {15, 0, 3, 15, 0, 1, 1, 0}, {6, 2, 4, 15, 0, 1, 1, 0},
    {7, 0, 4, 15, 0, 1, 1, 0},  {8, 0, 4, 15, 0, 1, 1, 0},  {10, 2, 4, 15, 0, 1, 1, 0},
    {9, 0, 4, 15, 0, 1, 1, 0},  {4, 2, 4, 15, 0, 1, 1, 0},  {5, 0, 4, 15, 0, 1, 1, 0},
    {19, 0, 2, 15, 0, 1, 1, 0},
};

static daNpcT_MotionSeqMngr_c::sequenceStepData_c l_faceMotionSequenceData[72] = {
    {2, -1, 1},  {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},  {7, -1, 1}, {14, 0, 0},  {-1, 0, 0},
    {-1, 0, 0},  {3, -1, 1},  {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0}, {4, -1, 1},  {-1, 0, 0},
    {-1, 0, 0},  {-1, 0, 0},  {5, -1, 1},  {11, 0, 0},  {-1, 0, 0}, {-1, 0, 0},  {12, -1, 0},
    {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},  {13, -1, 0}, {-1, 0, 0}, {-1, 0, 0},  {-1, 0, 0},
    {6, -1, 1},  {9, 0, 0},   {-1, 0, 0},  {-1, 0, 0},  {8, -1, 1}, {14, 0, 0},  {-1, 0, 0},
    {-1, 0, 0},  {17, -1, 0}, {18, 0, 0},  {-1, 0, 0},  {-1, 0, 0}, {14, -1, 0}, {-1, 0, 0},
    {-1, 0, 0},  {-1, 0, 0},  {15, -1, 1}, {16, 0, 0},  {-1, 0, 0}, {-1, 0, 0},  {16, -1, 0},
    {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},  {10, -1, 0}, {-1, 0, 0}, {-1, 0, 0},  {-1, 0, 0},
    {11, -1, 0}, {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},  {9, -1, 0}, {-1, 0, 0},  {-1, 0, 0},
    {-1, 0, 0},  {18, -1, 0}, {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0}, {0, -1, 0},  {-1, 0, 0},
    {-1, 0, 0},  {-1, 0, 0},
};

static daNpcT_MotionSeqMngr_c::sequenceStepData_c l_motionSequenceData[120] = {
    {0, -1, 0},  {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},  {28, -1, 1}, {29, 0, 0},  {-1, 0, 0},
    {-1, 0, 0},  {30, -1, 1}, {26, 0, 0},  {-1, 0, 0},  {-1, 0, 0},  {31, -1, 0}, {-1, 0, 0},
    {-1, 0, 0},  {-1, 0, 0},  {32, -1, 1}, {31, 0, 0},  {-1, 0, 0},  {-1, 0, 0},  {9, -1, 0},
    {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},  {29, -1, 0}, {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},
    {10, -1, 1}, {9, 0, 0},   {-1, 0, 0},  {-1, 0, 0},  {14, -1, 0}, {-1, 0, 0},  {-1, 0, 0},
    {-1, 0, 0},  {18, -1, 1}, {14, 0, 0},  {-1, 0, 0},  {-1, 0, 0},  {19, -1, 1}, {14, 0, 0},
    {-1, 0, 0},  {-1, 0, 0},  {15, -1, 0}, {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},  {11, -1, 0},
    {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},  {13, -1, 0}, {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},
    {25, -1, 1}, {24, 0, 0},  {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},
    {-1, 0, 0},  {2, -1, 0},  {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},  {24, -1, 1}, {2, 0, 0},
    {-1, 0, 0},  {-1, 0, 0},  {26, -1, 0}, {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},  {27, -1, 1},
    {26, 0, 0},  {-1, 0, 0},  {-1, 0, 0},  {20, -1, 1}, {21, 0, 0},  {-1, 0, 0},  {-1, 0, 0},
    {21, -1, 0}, {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},  {23, -1, 0}, {-1, 0, 0},  {-1, 0, 0},
    {-1, 0, 0},  {22, -1, 1}, {23, 0, 0},  {-1, 0, 0},  {-1, 0, 0},  {33, 4, 1},  {-1, 0, 0},
    {-1, 0, 0},  {-1, 0, 0},  {12, -1, 0}, {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},  {8, -1, 1},
    {9, 0, 0},   {-1, 0, 0},  {-1, 0, 0},  {3, -1, 0},  {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},
    {16, -1, 0}, {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},  {17, -1, 0}, {-1, 0, 0},  {-1, 0, 0},
    {-1, 0, 0},
};

const char* daNpc_Uri_c::mCutNameList[7] = {
    "",
    "CONVERSATION",
    "START_CARRY_TUTORIAL",
    "END_CARRY_TUTORIAL",
    "CONVERSATION_WITH_MOI",
    "FIND_WOLF",
    "MEETING_AGAIN",
};

daNpc_Uri_c::cutFunc daNpc_Uri_c::mCutList[7] = {
    NULL,
    &daNpc_Uri_c::cutConversation,
    &daNpc_Uri_c::cutStartCarryTutorial,
    &daNpc_Uri_c::cutEndCarryTutorial,
    &daNpc_Uri_c::cutConversationWithMoi,
    &daNpc_Uri_c::cutFindWolf,
    &daNpc_Uri_c::cutMeetingAgain,
};

daNpc_Uri_c::~daNpc_Uri_c() {
    if (mpMorf[0] != 0) {
        mpMorf[0]->stopZelAnime();
    }
    deleteRes(l_loadResPtrnList[mType], (char const**)l_resNameList);
}

int daNpc_Uri_c::create() {
    static const int heapSize[] = {0, 0x3F20, 0, 0, 0x3740, 0x3740, 0};

    daNpcT_ct(this, daNpc_Uri_c, l_faceMotionAnmData,
                       (const daNpcT_motionAnmData_c*)l_motionAnmData,
                       (const daNpcT_MotionSeqMngr_c::sequenceStepData_c*)l_faceMotionSequenceData,
                       4, (const daNpcT_MotionSeqMngr_c::sequenceStepData_c*)l_motionSequenceData,
                       4, (const daNpcT_evtData_c*)l_evtList, (char**)l_resNameList);

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
        fopAcM_setCullSizeBox(this, -200.0f, -100.0f, -200.0f, 200.0f, 300.0f, 200.0f);
        mSound.init(&current.pos, &eyePos, 3, 1);
        field_0x9c0.init(&mAcch, 0.0f, 0.0f);
        reset();
        mAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1, &mAcchCir,
                  fopAcM_GetSpeed_p(this), fopAcM_GetAngle_p(this), fopAcM_GetShapeAngle_p(this));
        mCcStts.Init(daNpc_Uri_Param_c::m.common.weight, 0, this);
        mCyl.Set(mCcDCyl);
        mCyl.SetStts(&mCcStts);
        mCyl.SetTgHitCallback(tgHitCallBack);
        if (mType == TYPE_4) {
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

int daNpc_Uri_c::CreateHeap() {
    J3DModelData* modelData = static_cast<J3DModelData*>(
        dComIfG_getObjectRes(l_resNameList[l_bmdData[0][1]], l_bmdData[0][0]));
    if (modelData == NULL) {
        return 1;
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

    if (mType == TYPE_1) {
        J3DModelData* modelData2;
        if (l_bmdData[1][0] >= 0) {
            modelData2 = static_cast<J3DModelData*>(
                dComIfG_getObjectRes(l_resNameList[l_bmdData[1][1]], l_bmdData[1][0]));
        } else {
            modelData2 = NULL;
        }
        if (modelData2 != NULL) {
            mpModel[0] = mDoExt_J3DModel__create(modelData2, 0x80000, 0x11000084);
        }
        if (mpModel[0] == NULL) {
            return 0;
        }
    }

    if (setFaceMotionAnm(2, false) && setMotionAnm(0, 0.0f, 0)) {
        return 1;
    }

    return 0;
}

int daNpc_Uri_c::Delete() {
    fopAcM_GetID(this);
    this->~daNpc_Uri_c();
    return 1;
}

int daNpc_Uri_c::Execute() {
    return daNpcT_c::execute();
}

int daNpc_Uri_c::Draw() {
    if (mpMatAnm[0] != NULL) {
        J3DModelData* modelData = mpMorf[0]->getModel()->getModelData();
        modelData->getMaterialNodePointer(getEyeballMaterialNo())->setMaterialAnm(mpMatAnm[0]);
    }

    return daNpcT_c::draw(0, 0, mRealShadowSize, NULL, 100.0f, 0, 0, 0);
}

int daNpc_Uri_c::createHeapCallBack(fopAc_ac_c* i_this) {
    return static_cast<daNpc_Uri_c*>(i_this)->CreateHeap();
}

int daNpc_Uri_c::ctrlJointCallBack(J3DJoint* param_0, int param_1) {
    if (param_1 == 0) {
        J3DModel* model = j3dSys.getModel();

        daNpc_Uri_c* i_this = reinterpret_cast<daNpc_Uri_c*>(model->getUserArea());
        if (i_this != 0) {
            i_this->ctrlJoint(param_0, model);
        }
    }

    return 1;
}

u8 daNpc_Uri_c::getType() {
    switch (fopAcM_GetParam(this) & 0xFF) {
    case 0:
        return TYPE_0;
        break;
    case 1:
        return TYPE_1;
        break;
    case 2:
        return TYPE_2;
        break;
    case 3:
        return TYPE_3;
        break;
    case 4:
        return TYPE_4;
        break;
    case 5:
        return TYPE_5;
    }
    return TYPE_6;
}

int daNpc_Uri_c::isDelete() {
    switch (mType) {
    case TYPE_0:
        return FALSE;
    case TYPE_1:
        return FALSE;
    case TYPE_2:
        return FALSE;
    case TYPE_3:
        return (chkPlayerGetWoodShield() == 0);
    case TYPE_4:
        return FALSE;
    case TYPE_5:
        return FALSE;
    default:
        return FALSE;
    }
}

void daNpc_Uri_c::reset() {
    csXyz cStack_a0;
    cXyz acStack_38;
    cXyz cStack_44;

    int iVar1 = (u8*)&field_0x1014 - (u8*)&field_0xfc0[0];
    if (mpMatAnm[0] != NULL) {
        mpMatAnm[0]->initialize();
    }
    initialize();
    for (int i = 0; i < 2; i++) {
        mActorMngr[i].initialize();
    }
    if (getPathID() != 0xFF) {
        mPath.initialize();
        mPath.setPathInfo(getPathID(), fopAcM_GetRoomNo(this), 0);
    }
    memset(&field_0xfc0[0], 0, iVar1);
    cStack_a0.setall(0);
    cStack_a0.y = home.angle.y;

    switch (mType) {
    case TYPE_1: {
        mPath.setNextPathInfo(fopAcM_GetRoomNo(this), 0);
        int num = mPath.getNumPnts();
        acStack_38 = mPath.getPntPos(num - 2);
        cStack_44 = mPath.getPntPos(num - 1);
        field_0xfd8 = cStack_44;
        field_0xfe4.setall(0);
        field_0xfe4.y = cLib_targetAngleY(&acStack_38, &field_0xfd8);
        /* dSv_event_flag_c::F_0032 - Ordon Ranch - 3rd day - First convo with fado (before forced
         * goat chase) */
        if (daNpcT_chkEvtBit(0x25)) {
            setPos(cStack_44);
            cStack_a0.y = cLib_targetAngleY(&acStack_38, &cStack_44);
            mPath.setPathInfo(getPathID(), fopAcM_GetRoomNo(this), 0);
            field_0x100e = 1;
        } else {
            acStack_38 = mPath.getPntPos(0);
            cStack_44 = mPath.getPntPos(1);
            setPos(acStack_38);
            cStack_a0.y = cLib_targetAngleY(&acStack_38, &cStack_44);
            mPath.setNextIdx(mPath.getNumPnts());
        }
        /* dSv_event_flag_c::F_0027 - Ordon Village - Uli tutorial ends (same whether pass or fail)
         */
        if (daNpcT_chkEvtBit(0x20)
            /* dSv_event_flag_c::F_0025 - Ordon Village - Pass Uli's pick-up tutorial */
            || daNpcT_chkEvtBit(0x1E)
            /* dSv_event_flag_c::F_0048 - Ordon Village - Uli's pick-up tutorial <fail> */
            || daNpcT_chkEvtBit(0x92))
        {
            setPos(home.pos);
            /* dSv_event_flag_c::F_0027 - Ordon Village - Uli tutorial ends (same whether pass or
             * fail) */
            if (daNpcT_chkEvtBit(0x20)) {
                field_0x100d = 1;

            } else {
                /* dSv_event_flag_c::F_0025 - Ordon Village - Pass Uli's pick-up tutorial */
                if (daNpcT_chkEvtBit(0x1E)
                    /* dSv_event_flag_c::F_0048 - Ordon Village - Uli's pick-up tutorial <fail> */
                    || daNpcT_chkEvtBit(0x92))
                {
                    field_0x100f = 1;
                }
            }
        }
        break;
    }
    case TYPE_4:
        field_0x100d = 1;
        break;
    case TYPE_0:
    case TYPE_2:
    case TYPE_3:
    case TYPE_5:
        break;
    }

    /* T_0007 - Ordon Village - During Uli's pick-up tutorial */
    daNpcT_offTmpBit(7);
    /* dSv_event_tmp_flag_c::GUARD_URI - Ordon Village - Rusl is guarding Uli, Ordon village night
     */
    dComIfGs_offTmpBit(0x1308);
    setAngle(cStack_a0);
}

void daNpc_Uri_c::afterJntAnm(int param_1) {
    if (param_1 == 1) {
        mDoMtx_stack_c::YrotM(mStagger.getAngleZ(1));
        mDoMtx_stack_c::ZrotM(-mStagger.getAngleX(1));
    } else if (param_1 == 4) {
        mDoMtx_stack_c::YrotM(mStagger.getAngleZ(0));
        mDoMtx_stack_c::ZrotM(mStagger.getAngleX(0));
    }

    if (0.0f < field_0x1004) {
        if (param_1 == 18) {
            mpMorf[0]->onMorfNone();
        } else if (param_1 == 28) {
            mpMorf[0]->offMorfNone();
        }
    }
}

void daNpc_Uri_c::setParam() {
    selectAction();
    srchActors();

    s16 sVar7 = mpHIO->m.common.talk_distance;
    s16 sVar6 = mpHIO->m.common.talk_angle;
    s16 sVar5 = mpHIO->m.common.attention_distance;
    s16 sVar4 = mpHIO->m.common.attention_angle;
    if (mType == TYPE_1) {
        sVar6 = 4;
        sVar4 = 4;
    }

    daPy_py_c* player = daPy_getPlayerActorClass();
    if (player->checkHorseRide()) {
        sVar7 = 7;
        sVar5 = 9;
    }

    attention_info.distances[fopAc_attn_LOCK_e] = daNpcT_getDistTableIdx(sVar5, sVar4);
    attention_info.distances[fopAc_attn_TALK_e] = attention_info.distances[fopAc_attn_LOCK_e];
    attention_info.distances[fopAc_attn_SPEAK_e] = daNpcT_getDistTableIdx(sVar7, sVar6);
    attention_info.flags = (fopAc_AttnFlag_SPEAK_e | fopAc_AttnFlag_TALK_e);
    scale.set(mpHIO->m.common.scale, mpHIO->m.common.scale, mpHIO->m.common.scale);
    mCcStts.SetWeight(mpHIO->m.common.weight);
    mCylH = mpHIO->m.common.height;
    mWallR = mpHIO->m.common.width;
    mAttnFovY = mpHIO->m.common.fov;
    mAcchCir.SetWallR(mWallR);
    mAcchCir.SetWallH(mpHIO->m.common.knee_length);
    mRealShadowSize = mpHIO->m.common.real_shadow_size;
    gravity = mpHIO->m.common.gravity;
    mExpressionMorfFrame = mpHIO->m.common.expression_morf_frame;
    mMorfFrames = mpHIO->m.common.morf_frame;
    if (mType == TYPE_4) {
        mAcch.SetGrndNone();
        mAcch.SetWallNone();
        gravity = 0.0f;
    }
}

BOOL daNpc_Uri_c::checkChangeEvt() {
    if (!chkAction(&daNpc_Uri_c::talk)) {
        mPreItemNo = 0;

        if (dComIfGp_event_chkTalkXY()) {
            if (dComIfGp_evmng_ChkPresentEnd()) {
                mEvtNo = 2;
                evtChange();
            }
            return TRUE;
        }

        switch (mType) {
        case TYPE_1:
            /* dSv_event_flag_c::F_0025 - Ordon Village - Pass Uli's pick-up tutorial */
            if (!daNpcT_chkEvtBit(0x1E)
                /* dSv_event_flag_c::F_0048 - Ordon Village - Uli's pick-up tutorial <fail> */
                && !daNpcT_chkEvtBit(0x92))
            {
                /* T_0007 - Ordon Village - During Uli's pick-up tutorial */
                if (!daNpcT_chkTmpBit(0x7)) {
                    /* dSv_event_flag_c::F_0031 - Ordon Village - 2nd day - Spoke to Uli bfore
                     * finding basket */
                    if (daNpcT_chkEvtBit(0x24)) {
                        mEvtNo = 4;
                    } else {
                        mEvtNo = 3;
                    }
                    evtChange();
                    return TRUE;
                }
                /* dSv_event_flag_c::F_0027 - Ordon Village - Uli tutorial ends (same whether pass
                 * or fail) */
            } else if (!daNpcT_chkEvtBit(0x20) && field_0x100b == 0) {
                mEvtNo = 7;
                evtChange();
                return TRUE;
            }
            break;
        case TYPE_4:
            /* dSv_event_flag_c::M_028 - Cutscene - [cutscene: 14] restore mountain spirit -
             * Reuinion with Colin et al. */
            if (daNpcT_chkEvtBit(0x3D)
                /* dSv_event_flag_c::F_0330 - Ordon Village - Meet again with Uli for the first time
                   (first forced conversation) */
                && !daNpcT_chkEvtBit(0x14A))
            {
                mEvtNo = 10;
                evtChange();
                return TRUE;
            }
            break;
        case TYPE_0:
        case TYPE_2:
        case TYPE_3:
        case TYPE_5:
            break;
        }
    }

    return FALSE;
}

void daNpc_Uri_c::setAfterTalkMotion() {
    int iVar2 = 17;
    switch (mFaceMotionSeqMngr.getNo()) {
    case 1:
        iVar2 = 10;
        break;
    case 4:
        return;
    case 5:
        return;
    case 7:
        iVar2 = 15;
        break;
    case 8:
        iVar2 = 10;
        break;
    case 11:
        iVar2 = 12;
        break;
    case 9:
        iVar2 = 16;
    }
    mFaceMotionSeqMngr.setNo(iVar2, -1.0f, 0, 0);
}

void daNpc_Uri_c::srchActors() {
    switch (mType) {
    case TYPE_0:
    case TYPE_1:
    case TYPE_2:
        break;
    case TYPE_3:
        if (!mActorMngr[0].getActorP()) {
            mActorMngr[0].entry(getNearestActorP(0x262));
        }
        break;
    case TYPE_4:
        if (!mActorMngr[0].getActorP()) {
            mActorMngr[0].entry(getNearestActorP(0x262));
        }
        break;
    case TYPE_5:
        break;
    }
}

BOOL daNpc_Uri_c::evtTalk() {
    if (chkAction(&daNpc_Uri_c::talk)) {
        (this->*field_0xfc0[1])(NULL);
    } else {
        setAction(&daNpc_Uri_c::talk);
    }
    return 1;
}

BOOL daNpc_Uri_c::evtCutProc() {
    int staffId = dComIfGp_getEventManager().getMyStaffId("Uri", this, -1);
    if (staffId != -1) {
        mStaffId = staffId;
        int actIdx =
            dComIfGp_getEventManager().getMyActIdx(mStaffId, (char**)mCutNameList, 7, 0, 0);
        if ((this->*(mCutList[actIdx]))(mStaffId) != 0) {
            dComIfGp_getEventManager().cutEnd(mStaffId);
        }
        return true;
    }
    return false;
}

void daNpc_Uri_c::action() {
    fopAc_ac_c* hitActor = hitChk(&mCyl, 0xffffffff);

    if (mStagger.checkRebirth()) {
        mStagger.initialize();
        mMode = 1;
    }

    if (field_0xfc0[0] != NULL) {
        if (field_0xfc0[1] == field_0xfc0[0]) {
            (this->*field_0xfc0[1])(NULL);
        } else {
            setAction(field_0xfc0[0]);
        }
    }

    if (!dComIfGp_event_runCheck() && field_0x1012 != 0) {
        fopAcM_delete(this);
    }
}

void daNpc_Uri_c::beforeMove() {
    fopAcM_OffStatus(this, 0x8000000);
    if (checkHide()) {
        fopAcM_OnStatus(this, 0x8000000);
    }

    if (checkHide() || mNoDraw != 0) {
        attention_info.flags = 0;
    }
}

NPC_URI_HIO_CLASS l_HIO;

void daNpc_Uri_c::setAttnPos() {
    cXyz acStack_3c(-30.0f, 10.0f, 0.0f);

    mStagger.calc(0);

    f32 dVar6 = cM_s2rad(mCurAngle.y - field_0xd7e.y);
    mJntAnm.setParam(this, mpMorf[0]->getModel(), &acStack_3c, getBackboneJointNo(),
                     getNeckJointNo(), getHeadJointNo(), mpHIO->m.common.body_angleX_min,
                     mpHIO->m.common.body_angleX_max, mpHIO->m.common.body_angleY_min,
                     mpHIO->m.common.body_angleY_max, mpHIO->m.common.head_angleX_min,
                     mpHIO->m.common.head_angleX_max, mpHIO->m.common.head_angleY_min,
                     mpHIO->m.common.head_angleY_max, mpHIO->m.common.neck_rotation_ratio, dVar6,
                     NULL);
    mJntAnm.calcJntRad(0.2f, 1.0f, dVar6);

    setMtx();
    mDoMtx_stack_c::copy(mpMorf[0]->getModel()->getAnmMtx(getHeadJointNo()));
    mDoMtx_stack_c::multVec(&acStack_3c, &eyePos);

    mJntAnm.setEyeAngleX(eyePos, 1.0f, 0);
    mJntAnm.setEyeAngleY(eyePos, mCurAngle.y + field_0xd8a.y, 1, 1.0f, 0);

    acStack_3c.set(0.0f, 0.0f, 0.0f);
    acStack_3c.y = mpHIO->m.common.attention_offset;

    if (field_0x100d != 0) {
        if (mType == TYPE_1) {
            acStack_3c.y -= 10.0f;
            acStack_3c.z = -60.0f;
        } else if (mType == TYPE_4) {
            acStack_3c.y -= 20.0f;
        }
    }

    mDoMtx_stack_c::YrotS(mCurAngle.y);
    mDoMtx_stack_c::multVec(&acStack_3c, &acStack_3c);
    attention_info.position = acStack_3c + current.pos;
    static cXyz prtclScl(1.0f, 1.0f, 1.0f);
    setFootPos();
    if (3.0f < speedF) {
        setFootPrtcl(&prtclScl, 11.0f, 0);
    }
}

void daNpc_Uri_c::setCollision() {
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
        cStack_48.set(0.0f, 0.0f, 10.0f);
        f32 cylHeight = mCylH;
        f32 cylRadius = mWallR;
        if (field_0x100d != 0) {
            switch (mType) {
            case TYPE_1:
                cylHeight = mCylH - 10.0f;
                cylRadius = mWallR;
                cStack_48.set(0.0f, 0.0f, -60.0f);
                break;
            case TYPE_4:
                cylHeight = mCylH - 20.0f;
                cylRadius = mWallR;
                cStack_48.set(0.0f, 0.0f, 0.0f);
                break;
            case TYPE_2:
            case TYPE_3:
            case TYPE_5:
                break;
            }
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

int daNpc_Uri_c::drawDbgInfo() {
    return 0;
}

void daNpc_Uri_c::drawOtherMdl() {
    static int const jointNo[1] = {12};

    Mtx mtx;
    J3DModel* model = mpMorf[0]->getModel();

    for (int i = 0; i < 1; i++) {
        if (mpModel[i] != NULL && i == 0 && field_0x1008 != 0) {
            g_env_light.setLightTevColorType_MAJI(mpModel[i], &tevStr);
            mDoMtx_stack_c::copy(model->getAnmMtx(jointNo[0]));
            cMtx_copy(mDoMtx_stack_c::get(), mtx);
            mpModel[i]->setBaseTRMtx(mtx);
            mDoExt_modelUpdateDL(mpModel[i]);
            dComIfGd_addRealShadow(mShadowKey, mpModel[i]);
        }
    }
}

bool daNpc_Uri_c::afterSetMotionAnm(int param_1, int param_2, f32 param_3, int param_4) {
    field_0x1008 = 0;

    switch (param_1) {
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
    case 16:
    case 17:
    case 18:
    case 19:
    case 20:
    case 21:
    case 33:
        field_0x1008 = 1;
        break;
    case 22:
    case 23:
    case 24:
    case 25:
    case 26:
    case 27:
    case 28:
    case 29:
    case 30:
    case 31:
    case 32:
        break;
    }

    return true;
}

void daNpc_Uri_c::changeBck(int* param_0, int* param_1) {
    if (mType != TYPE_5) {
        return;
    }
    if (*param_0 != 10) {
        return;
    }
    *param_0 = 4;
    *param_1 = 5;
}

void daNpc_Uri_c::changeBtp(int* param_0, int* param_1) {
    if (mType != TYPE_5) {
        return;
    }
    if (*param_0 != 43) {
        return;
    }
    *param_0 = 7;
    *param_1 = 5;
}

int daNpc_Uri_c::selectAction() {
    field_0xfc0[0] = NULL;

    switch (mType) {
    case TYPE_1:
        if (field_0x100d != NULL) {
            field_0xfc0[0] = &daNpc_Uri_c::sitWait;
        } else {
            if (field_0x100e != NULL) {
                /* T_0007 - Ordon Village - During Uli's pick-up tutorial */
                if (daNpcT_chkTmpBit(7) && field_0x100f == NULL) {
                    field_0xfc0[0] = &daNpc_Uri_c::walk;
                } else {
                    field_0xfc0[0] = &daNpc_Uri_c::wait;
                }
            } else {
                field_0xfc0[0] = &daNpc_Uri_c::krun;
            }
        }
        break;
    case TYPE_4:
        field_0xfc0[0] = &daNpc_Uri_c::sitWait;
        break;
    default:
        field_0xfc0[0] = &daNpc_Uri_c::wait;
        break;
    }

    return 1;
}

int daNpc_Uri_c::chkAction(int (daNpc_Uri_c::*action)(void*)) {
    return field_0xfc0[1] == action;
}

int daNpc_Uri_c::setAction(int (daNpc_Uri_c::*action)(void*)) {
    mMode = 3;
    if (field_0xfc0[1] != NULL) {
        (this->*field_0xfc0[1])(NULL);
    }
    mMode = 0;
    field_0xfc0[1] = action;
    if (field_0xfc0[1] != NULL) {
        (this->*field_0xfc0[1])(NULL);
    }

    return 1;
}

BOOL daNpc_Uri_c::chkPlayerCarryBasket() {
    fopAc_ac_c* actor = NULL;
    daPy_py_c* player = daPy_getPlayerActorClass();
    if (fopAcM_SearchByID(player->getGrabActorID(), &actor) && actor != NULL &&
        fopAcM_GetName(actor) == PROC_OBJ_KAGO)
    {
        return TRUE;
    }
    return FALSE;
}

BOOL daNpc_Uri_c::chkPlayerGetWoodShield() {
    return dComIfGs_isCollectShield(0);
}

int daNpc_Uri_c::getTutorialCond(cXyz param_1) {
    cXyz local_44;
    cXyz local_50;
    cXyz cStack_5c;

    local_44 = mPath.getPntPos(mPath.getNumPnts() - 1);
    if (local_44.absXZ(param_1) < mpHIO->m.field_0x98) {
        return 9;
    }
    f32 fVar11 = (param_1 - current.pos).absXZ();
    s16 sVar8 = cLib_targetAngleY(&current.pos, &param_1) - mCurAngle.y;
    s16 uVar5 = abs(sVar8);

    if (mpHIO->m.field_0xa4 < fVar11 && ((u32)uVar5 & 0xFFFF) > 0x4000) {
        if (mpHIO->m.field_0xa8 < fVar11) {
            field_0x1000 = mpHIO->m.field_0xa8 - 100.0f;
            return 5;
        }
        if (field_0x1009 != NULL) {
            return 8;
        }
    }

    int i;
    for (i = 0; i < mPath.getNumPnts() - 1; i++) {
        local_44 = mPath.getPntPos(i);
        local_50 = mPath.getPntPos(i + 1);
        if (!((param_1 - local_44).absXZ() < mpHIO->m.field_0xa8)) {
            if (!((param_1 - local_50).absXZ() < mpHIO->m.field_0xa8)) {
                f32 local_a8;
                if (cM3d_Len2dSqPntAndSegLine(param_1.x, param_1.z, local_44.x, local_44.z,
                                              local_50.x, local_50.z, &cStack_5c.x, &cStack_5c.z,
                                              &local_a8) &&
                    local_a8 < mpHIO->m.field_0xa8)
                {
                    break;
                }
            } else {
                break;
            }
        } else {
            break;
        }
    }

    if (mPath.getNumPnts() - 1 <= i) {
        field_0x1000 = mpHIO->m.field_0xa8 - 100.0f;
        return 5;
    }

    return 2;
}

int daNpc_Uri_c::cutConversation(int param_1) {
    int rv = 0;
    int iVar4 = -1;
    int iVar3 = 0;

    int* piVar1 = dComIfGp_evmng_getMyIntegerP(param_1, "prm");
    if (piVar1) {
        iVar4 = *piVar1;
    }
    piVar1 = dComIfGp_evmng_getMyIntegerP(param_1, "msgNo");
    if (piVar1) {
        iVar3 = *piVar1;
    }

    if (dComIfGp_getEventManager().getIsAddvance(param_1)) {
        switch (iVar4) {
        case 0:
            initTalk(mFlowNodeNo, NULL);
            break;
        case 3:
            initTalk(mFlowNodeNo, NULL);
            break;
        }
    }

    int local_28[2] = {-1, -1};

    switch (iVar4) {
    case 0:
    case 1:
    case 2:
    case 3:
        mJntAnm.lookPlayer(0);
        if (iVar4 == 0 || iVar4 == 2) {
            mJntAnm.lookNone(0);
        }
        local_28[0] = iVar3;
        if (talkProc(local_28, 0, NULL, 0)) {
            if (iVar3 == 0) {
                if (mFlow.checkEndFlow() != 0) {
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

int daNpc_Uri_c::cutStartCarryTutorial(int param_1) {
    cXyz acStack_30;
    csXyz cStack_38;
    int uVar7 = 0;
    int iVar6 = -1;
    int* piVar1 = dComIfGp_evmng_getMyIntegerP(param_1, "prm");
    if (piVar1) {
        iVar6 = *piVar1;
    }

    if (dComIfGp_getEventManager().getIsAddvance(param_1)) {
        switch (iVar6) {
        case 0:
            break;
        case 1:
            if (daNpcT_getPlayerInfoFromPlayerList(23, fopAcM_GetRoomNo(this), &acStack_30,
                                                   &cStack_38))
            {
                daPy_getPlayerActorClass()->setPlayerPosAndAngle(&acStack_30, cStack_38.y, 0);
                dComIfGp_evmng_setGoal(&acStack_30);
            }
            mFaceMotionSeqMngr.setNo(17, 0.0f, 0, 0);
            mMotionSeqMngr.setNo(8, 0.0f, 0, 0);
            mJntAnm.lookPlayer(0);
            setAngle(fopAcM_searchPlayerAngleY(this));
            break;
        case 2:
            initTalk(mFlowNodeNo, NULL);
        }
    }

    switch (iVar6) {
    case 0:
        uVar7 = 1;
        break;
    case 1:
        uVar7 = 1;
        break;
    case 2:
        if (talkProc(NULL, 0, NULL, 0) && mFlow.checkEndFlow()) {
            uVar7 = 1;
        }
    }

    return uVar7;
}

int daNpc_Uri_c::cutEndCarryTutorial(int param_1) {
    cXyz acStack_30;
    csXyz cStack_44;
    int uVar8 = 0;
    int iVar7 = -1;
    int* piVar1 = dComIfGp_evmng_getMyIntegerP(param_1, "prm");
    if (piVar1) {
        iVar7 = *piVar1;
    }

    if (dComIfGp_getEventManager().getIsAddvance(param_1)) {
        switch (iVar7) {
        case 0:
            break;
        case 1:
            if (daNpcT_getPlayerInfoFromPlayerList(21, fopAcM_GetRoomNo(this), &acStack_30,
                                                   &cStack_44))
            {
                daPy_getPlayerActorClass()->setPlayerPosAndAngle(&acStack_30, cStack_44.y, 0);
                dComIfGp_evmng_setGoal(&acStack_30);
            }
            mFaceMotionSeqMngr.setNo(17, -1.0f, 0, 0);
            mMotionSeqMngr.setNo(0, -1.0f, 0, 0);
            mJntAnm.lookPlayer(0);
            setPos(home.pos);
            setAngle(home.angle.y);
            speedF = 0;
            speed.setall(0.0f);
            daNpcT_onEvtBit(0x1e);
            break;
        case 2:
            initTalk(19, NULL);
            mItemPartnerId = fpcM_ERROR_PROCESS_ID_e;
        }
    }

    switch (iVar7) {
    case 0:
        action();
        uVar8 = 1;
        break;
    case 1:
        uVar8 = 1;
        break;
    case 2:
        int local_48;
        if (talkProc(NULL, 0, NULL, 0) && mFlow.checkEndFlow() &&
            (s32)mFlow.getEventId(&local_48) == 1)
        {
            if (mItemPartnerId == fpcM_ERROR_PROCESS_ID_e) {
                mItemPartnerId =
                    fopAcM_createItemForPresentDemo(&current.pos, local_48, 0, -1, -1, NULL, NULL);
            }
            if (fopAcM_IsExecuting(mItemPartnerId)) {
                /* T_0007 - Ordon Village - During Uli's pick-up tutorial */
                daNpcT_offTmpBit(7);
                /* dSv_event_tmp_flag_c::T_0009 - Ordon Village - Walked a bit during Uli's pick-up
                 * tutorial */
                daNpcT_offTmpBit(10);
                /* dSv_event_tmp_flag_c::T_0031 - Ordon Village - Borrow Rusl's sword */
                daNpcT_offTmpBit(0x20);
                field_0x100b = 1;
                mEvtNo = 1;
                evtChange();
            }
        }
    }

    return uVar8;
}

int daNpc_Uri_c::cutConversationWithMoi(int param_1) {
    int uVar5 = 0;
    int iVar4 = -1;
    int iVar3 = 0;
    int* piVar1 = dComIfGp_evmng_getMyIntegerP(param_1, "prm");
    if (piVar1) {
        iVar4 = *piVar1;
    }
    piVar1 = dComIfGp_evmng_getMyIntegerP(param_1, "msgNo");
    if (piVar1) {
        iVar3 = *piVar1;
    }

    fopAc_ac_c* actors[2] = {mActorMngr[0].getActorP(), (fopAc_ac_c*)this};
    dComIfGp_setMesgCameraInfoActor(actors[0], actors[1], 0, 0, 0, 0, 0, 0, 0, 0);
    if (dComIfGp_getEventManager().getIsAddvance(param_1)) {
        switch (iVar4) {
        case 0:
            initTalk(0xce, &actors[0]);
            break;
        case 1:
            break;
        }
    }

    int local_30[2] = {-1, -1};

    switch (iVar4) {
    case 0:
    case 1:
        local_30[0] = iVar3;
        if (talkProc(local_30, 1, &actors[0], 0)) {
            if (iVar3 == 0) {
                if (mFlow.checkEndFlow()) {
                    uVar5 = 1;
                }
            } else {
                uVar5 = 1;
            }
        }
    }

    return uVar5;
}

int daNpc_Uri_c::cutFindWolf(int param_1) {
    cXyz cStack_2c;
    int iVar12 = 0;
    int iVar11 = -1;
    int local_30 = 0;
    int* piVar2 = dComIfGp_evmng_getMyIntegerP(param_1, "prm");
    if (piVar2) {
        iVar11 = *piVar2;
    }
    int* puVar3 = dComIfGp_evmng_getMyIntegerP(param_1, "timer");
    if (puVar3) {
        local_30 = *puVar3;
    }

    if (dComIfGp_getEventManager().getIsAddvance(param_1)) {
        switch (iVar11) {
        case 0: {
            mFaceMotionSeqMngr.setNo(12, 0.0f, 0, 0);
            mMotionSeqMngr.setNo(23, 0.0f, 0, 0);
            mSound.startCreatureVoice(JAISoundID(Z2SE_URI_V_SCREAM_S), -1);
            mEventTimer = local_30;
            s16 sVar9 = fopAcM_searchPlayerAngleY(this);
            sVar9 -= home.angle.y;
            if (sVar9 > 0x4000) {
                sVar9 = 0x4000;
            } else if (sVar9 < -0x4000) {
                sVar9 = -0x4000;
            }
            sVar9 += home.angle.y;
            cStack_2c.set(0.0f, 300.0f, mpHIO->m.common.search_distance + 150.0f);
            mDoMtx_stack_c::YrotS(sVar9);
            mDoMtx_stack_c::multVec(&cStack_2c, &cStack_2c);
            cStack_2c += current.pos;
            mGndChk.SetPos(&cStack_2c);
            cStack_2c.y = dComIfG_Bgsp().GroundCross(&mGndChk);
            daPy_getPlayerActorClass()->setPlayerPosAndAngle(&cStack_2c, sVar9 - (s16)0x8000, 0);
            /* dSv_event_tmp_flag_c::GUARD_URI - Ordon Village - Rusl is guarding Uli, Ordon village
             * night */
            dComIfGs_onTmpBit(0x1308);
            break;
        }
        case 1:
            fopAc_ac_c* actor = mActorMngr[0].getActorP();
            if (actor) {
                dComIfGp_getEvent()->setPt2(actor);
            }
            break;
        }
    }

    switch (iVar11) {
    case 0:
        mJntAnm.lookPlayer(0);
        if (mEventTimer != 0 && cLib_calcTimer(&mEventTimer) == 0) {
            iVar12 = 1;
        }
        if (iVar12 != 0) {
            fopAc_ac_c* actor = mActorMngr[0].getActorP();
            if (actor) {
                s16 angle = cLib_targetAngleY(&daPy_getPlayerActorClass()->current.pos,
                                              &actor->current.pos);
                daPy_getPlayerActorClass()->changeDemoMoveAngle(angle);
            }
        }
        break;
    case 1:
        mJntAnm.lookPlayer(0);
        break;
    }

    return iVar12;
}

int daNpc_Uri_c::cutMeetingAgain(int param_1) {
    cXyz acStack_40;
    csXyz cStack_48;
    int uVar9 = 0;
    int iVar10 = -1;
    int uVar8 = 0;
    int* piVar1 = dComIfGp_evmng_getMyIntegerP(param_1, "prm");
    if (piVar1) {
        iVar10 = *piVar1;
    }
    int* puVar2 = dComIfGp_evmng_getMyIntegerP(param_1, "timer");
    if (puVar2) {
        uVar8 = *puVar2;
    }

    if (dComIfGp_getEventManager().getIsAddvance(param_1)) {
        switch (iVar10) {
        case 0:
            mJntAnm.lookPlayer(0);
            /* dSv_event_tmp_flag_c::T_0010 - General use - General use temporary flag (flow
             * control) A */
            daNpcT_offTmpBit(0xB);
            initTalk(mFlowNodeNo, NULL);
            break;
        case 1:
        case 3:
            acStack_40.set(0.0f, 100.0f, 150.0f);
            cStack_48.y = fopAcM_searchPlayerAngleY(this);
            mDoMtx_stack_c::YrotS(cStack_48.y);
            mDoMtx_stack_c::multVec(&acStack_40, &acStack_40);
            acStack_40 += current.pos;
            mGndChk.SetPos(&acStack_40);
            acStack_40.y = dComIfG_Bgsp().GroundCross(&mGndChk);
            cStack_48.y = cLib_targetAngleY(&acStack_40, &current.pos);
            if (iVar10 == 1) {
                daPy_getPlayerActorClass()->setPlayerPosAndAngle(
                    &daPy_getPlayerActorClass()->current.pos, cStack_48.y, 0);
                dComIfGp_evmng_setGoal(&acStack_40);
                mEventTimer = uVar8;
            }
            if (iVar10 == 3) {
                daPy_getPlayerActorClass()->setPlayerPosAndAngle(&acStack_40, cStack_48.y, 0);
                initTalk(mFlowNodeNo, NULL);
                mEventTimer = 0;
            }
        }
    }

    switch (iVar10) {
    case 0:
    case 1:
    case 2:
        if (talkProc(NULL, 0, NULL, 0)) {
            if (mFlow.checkEndFlow() != 0) {
                uVar9 = 1;
            }
        } else {
            switch (iVar10) {
            case 0:
                if (mDoGph_gInf_c::isFade()) {
                    uVar9 = 1;
                }
                break;
            case 1:
                uVar9 = 1;
            }
        }
        break;
    case 3:
        iVar10 = talkProc(NULL, 0, NULL, 0);
        if (iVar10 != 0 && mFlow.checkEndFlow() != 0) {
            uVar9 = 1;
        }
        break;
    }

    return uVar9;
}

int daNpc_Uri_c::krun(void* param_0) {
    int iVar7 = mpHIO->m.field_0x8c / 2;

    switch (mMode) {
    case MODE_ENTER:
    case MODE_INIT:
        if (!mStagger.checkStagger()) {
            mFaceMotionSeqMngr.setNo(13, -1.0f, 0, 0);
            mMotionSeqMngr.setNo(25, -1.0f, 0, 0);
            mMode = MODE_RUN;
        }
    case MODE_RUN:
        if (!mStagger.checkStagger()) {
            mJntAnm.lookNone(0);
            if (field_0xfec == 0) {
                cXyz cStack_58;
                int iVar2 = mPath.getDstPosH(current.pos, &cStack_58, mPath.getNumPnts(), 4);

                calcSpeedAndAngle(cStack_58, iVar2, 6, 0x800);
                if (iVar2 != 0 && cM3d_IsZero(speedF)) {
                    mFaceMotionSeqMngr.setNo(17, -1.0f, 0, 0);
                    mMotionSeqMngr.setNo(11, -1.0f, 0, 0);
                    field_0xfec = cLib_getRndValue(iVar7 * 0.5f, iVar7 * 1.5f);
                    mPath.setPathInfo(getPathID(), fopAcM_GetRoomNo(this), 0);
                    daNpcT_onEvtBit(0x25);
                }
            } else {
                J3DAnmTransform* anm_transform = getTrnsfrmAnmP(
                    l_resNameList[l_motionAnmData[15].mBckArcIdx], l_motionAnmData[15].mBckFileIdx);
                if (anm_transform == mpMorf[0]->getAnm() && mpMorf[0]->checkFrame(14.0f)) {
                    mSound.startCreatureVoice(Z2SE_M036_URI_01, -1);
                }
                iVar7 = cLib_calcTimer(&field_0xfec);
                if (iVar7 == 0) {
                    field_0x100e = 1;
                }
            }
        }
        attention_info.flags = 0;
        break;
    case MODE_EXIT:
    case MODE_4:
        break;
    }

    return 1;
}

int daNpc_Uri_c::wait(void* param_0) {
    s16 sVar10 = home.angle.y;

    switch (mMode) {
    case MODE_ENTER:
    case MODE_INIT:
        if (!mStagger.checkStagger()) {
            switch (mType) {
            case TYPE_1:
                if (field_0x100b) {
                    mFaceMotionSeqMngr.setNo(17, -1.0f, 0, 0);
                    mMotionSeqMngr.setNo(0, -1.0f, 0, 0);
                } else {
                    if (field_0x100f) {
                        /* dSv_event_flag_c::F_0025 - Ordon Village - Pass Uli's pick-up tutorial */
                        if (!daNpcT_chkEvtBit(0x1E)
                            /* dSv_event_flag_c::F_0048 - Ordon Village - Uli's pick-up tutorial
                               <fail> */
                            && !daNpcT_chkEvtBit(0x92))
                        {
                            mFaceMotionSeqMngr.setNo(17, -1.0f, 0, 0);
                            mMotionSeqMngr.setNo(8, -1.0f, 0, 0);
                        } else {
                            mFaceMotionSeqMngr.setNo(17, -1.0f, 0, 0);
                            mMotionSeqMngr.setNo(0, -1.0f, 0, 0);
                        }
                    } else {
                        /* dSv_event_flag_c::F_0031 - Ordon Village - 2nd day - Spoke to Uli bfore
                         * finding basket */
                        if (daNpcT_chkEvtBit(0x24)) {
                            mFaceMotionSeqMngr.setNo(5, -1.0f, 0, 0);
                            mMotionSeqMngr.setNo(12, -1.0f, 0, 0);
                        } else {
                            mFaceMotionSeqMngr.setNo(6, -1.0f, 0, 0);
                            mMotionSeqMngr.setNo(13, -1.0f, 0, 0);
                        }
                    }
                }
                break;
            case TYPE_3:
                /* dSv_event_tmp_flag_c::GUARD_URI - Ordon Village - Rusl is guarding Uli, Ordon
                 * village night */
                if (dComIfGs_isTmpBit(0x1308)) {
                    mFaceMotionSeqMngr.setNo(12, -1.0f, 0, 0);
                    mMotionSeqMngr.setNo(22, -1.0f, 0, 0);
                } else {
                    mFaceMotionSeqMngr.setNo(10, -1.0f, 0, 0);
                    mMotionSeqMngr.setNo(16, -1.0f, 0, 0);
                }
                break;
            default:
                mFaceMotionSeqMngr.setNo(17, -1.0f, 0, 0);
                mMotionSeqMngr.setNo(0, -1.0f, 0, 0);
            }
            mMode = 2;
        }
    case MODE_RUN:
        switch (mType) {
        case TYPE_1:
            /* T_0007 - Ordon Village - During Uli's pick-up tutorial */
            if (daNpcT_chkTmpBit(0x7)) {
                if (chkPlayerCarryBasket()) {
                    switch (getTutorialCond(daPy_getPlayerActorClass()->current.pos)) {
                    case 9:
                        field_0x1009 = 0;
                        field_0x100a = 0;
                        mEvtNo = 6;
                        break;
                    default:
                        field_0x1009 = 0;
                        field_0x100a = 0;
                        break;
                    }
                }
            } else {
                /* dSv_event_flag_c::F_0025 - Ordon Village - Pass Uli's pick-up tutorial */
                if (!daNpcT_chkEvtBit(0x1E)
                    /* dSv_event_flag_c::F_0048 - Ordon Village - Uli's pick-up tutorial <fail> */
                    && !daNpcT_chkEvtBit(0x92))
                {
                    cXyz acStack_70 = getAttnPos(daPy_getPlayerActorClass());
                    int dist_index = attention_info.distances[1];
                    f32 fVar2 = dComIfGp_getAttention()->getDistTable(dist_index).mLowerY * -1.0f;
                    f32 fVar3 = dComIfGp_getAttention()->getDistTable(dist_index).mUpperY * -1.0f;
                    if (chkPointInArea(acStack_70, attention_info.position, 200.0f, fVar2, fVar3,
                                       0))
                    {
                        if (daPy_getPlayerActorClass()->eventInfo.chkCondition(dEvtCnd_CANTALK_e) !=
                                0 &&
                            chkPlayerCarryBasket())
                        {
                            mEvtNo = 5;
                        }
                    }
                    sVar10 = field_0xfe4.y;
                }
            }
            break;
        case TYPE_2:
            break;
        case TYPE_3:
            /* dSv_event_flag_c::F_0205 - Ordon Village - Heard Rusl and Uli talking in Ordon
             * village at night */
            if (daNpcT_chkEvtBit(0xCD)) {
                cXyz cStack_7c = getAttnPos(daPy_getPlayerActorClass());
                if (chkPointInArea(cStack_7c, attention_info.position,
                                   mpHIO->m.common.search_distance, mpHIO->m.common.search_height,
                                   mpHIO->m.common.search_depth, mCurAngle.y))
                {
                    if (!daPy_getPlayerActorClass()->checkPlayerFly()) {
                        if (daPy_getPlayerActorClass()->eventInfo.chkCondition(dEvtCnd_CANTALK_e) !=
                            0)
                        {
                            if (chkActorInSight(daPy_getPlayerActorClass(), mAttnFovY,
                                                home.angle.y))
                            {
                                mEvtNo = 9;
                            }
                        }
                    }
                }
            } else {
                mEvtNo = 8;
                field_0xe33 = true;
            }
        }

        if (!mStagger.checkStagger()) {
            if (mType == TYPE_1 && field_0x100b != 0) {
                mSpeakEvent = true;
                field_0xe33 = true;
            }
            /* dSv_event_tmp_flag_c::GUARD_URI - Ordon Village - Rusl is guarding Uli, Ordon village
             * night */
            if (dComIfGs_isTmpBit(0x1308)) {
                mPlayerActorMngr.entry(daPy_getPlayerActorClass());
                mJntAnm.lookPlayer(0);
            } else {
                if (mMotionSeqMngr.getNo() == 22) {
                    mFaceMotionSeqMngr.setNo(10, -1.0f, 0, 0);
                    mMotionSeqMngr.setNo(16, -1.0f, 0, 0);
                }
                if (mPlayerActorMngr.getActorP() && !mTwilight) {
                    mJntAnm.lookPlayer(0);
                    if (mType == TYPE_1 && !field_0x100b && !field_0x100f) {
                        mJntAnm.lookNone(0);
                    } else if (!chkActorInSight(mPlayerActorMngr.getActorP(), mAttnFovY,
                                                mCurAngle.y))
                    {
                        mJntAnm.lookNone(0);
                    }
                    if (!srchPlayerActor() && sVar10 == mCurAngle.y) {
                        mMode = 1;
                    }
                } else {
                    mJntAnm.lookNone(0);
                    if (sVar10 != mCurAngle.y) {
                        if (field_0xe34) {
                            if (field_0x1008) {
                                if (step(sVar10, 17, 24, 15, 0)) {
                                    mMode = 1;
                                }
                            } else if (step(sVar10, -1, -1, 15, 0)) {
                                mMode = 1;
                            }
                        } else {
                            setAngle(sVar10);
                            mMode = 1;
                        }
                        attention_info.flags = 0;
                    } else {
                        if (!mTwilight) {
                            srchPlayerActor();
                        }
                    }
                }
            }
        }
        break;
    case MODE_EXIT:
    case MODE_4:
        break;
    }

    return 1;
}

int daNpc_Uri_c::walk(void* param_0) {
    BOOL bVar = FALSE;
    int uVar2 = mpHIO->m.field_0x8c;
    int uVar3 = mpHIO->m.field_0x8e;
    int uVar4 = mpHIO->m.field_0x90;

    switch (mMode) {
    case MODE_ENTER:
    case MODE_INIT:
        if (!mStagger.checkStagger()) {
            field_0xff4 = 0;
            field_0xff8 = 0;
            mMode = MODE_RUN;
        }
    case MODE_RUN:
        if (chkPlayerCarryBasket()) {
            fopAcM_searchPlayerAngleY(this);
            switch (getTutorialCond(daPy_getPlayerActorClass()->current.pos)) {
            case 9:
                field_0x1009 = 0;
                field_0x100a = 0;
                mEvtNo = 6;
                break;
            default:
                field_0x1009 = 0;
                field_0x100a = 0;
                break;
            }
        }
        if (!mStagger.checkStagger()) {
            cXyz cStack_84;
            int iVar2 = mPath.getDstPosH(current.pos, &cStack_84, mPath.getNumPnts(), 4);
            if (iVar2) {
                bVar = TRUE;
            }
            calcSpeedAndAngle(cStack_84, bVar, 6, 0x800);

            if (iVar2 && cM3d_IsZero(speedF) && mMotionSeqMngr.getNo() != 8) {
                mFaceMotionSeqMngr.setNo(17, -1.0f, 0, 0);
                mMotionSeqMngr.setNo(8, -1.0f, 0, 0);

                int num = mPath.getNumPnts();
                cXyz cStack_54 = mPath.getPntPos((int)num - 1);
                cXyz cStack_60 = mPath.getPntPos((int)num - 2);
                home.angle.y = cLib_targetAngleY(&cStack_54, &cStack_60);
            }
            if (!bVar && field_0x1011 == 0) {
                if (field_0xffc == 0) {
                    bool bVar11 = false;
                    if (mMotionSeqMngr.getNo() != 28 && mMotionSeqMngr.getNo() != 29) {
                        bVar11 = true;
                        field_0xfec = 0;
                    } else {
                        cLib_chaseF(&field_0x1004, 0.0f, 1.0f);
                    }
                    if (field_0xfec == 0) {
                        if (bVar11 || mpMorf[0]->isLoop()) {
                            field_0xfec = cLib_getRndValue((f32)uVar2 * 0.5f, (f32)uVar2 * 1.5f);
                            field_0x1004 = mMorfFrames;
                            if (mMotionSeqMngr.getNo() == 28) {
                                mMotionSeqMngr.setNo(29, -1.0f, 0, 0);
                            } else {
                                mMotionSeqMngr.setNo(28, -1.0f, 0, 0);
                            }
                        }
                    } else {
                        field_0xfec -= 1;
                    }
                } else {
                    field_0xffc -= 1;
                }
            }
            if (field_0xff4 == 0) {
                if (field_0xff8 != 0) {
                    mPlayerActorMngr.entry(daPy_getPlayerActorClass());
                    field_0xff8 -= 1;
                } else {
                    field_0xff4 = cLib_getRndValue((f32)uVar3 * 0.5f, (f32)uVar3 * 1.5f);
                }
            } else {
                field_0xff4 -= 1;
                if (field_0xff4 == 0) {
                    field_0xff8 = cLib_getRndValue((f32)uVar4 * 0.5f, (f32)uVar4 * 1.5f);
                }
            }
            if (iVar2 && home.angle.y != mCurAngle.y) {
                mJntAnm.lookNone(0);
                if (step(home.angle.y, 17, 24, 15, 0)) {
                    mFaceMotionSeqMngr.setNo(17, -1.0f, 0, 0);
                    mMotionSeqMngr.setNo(8, -1.0f, 0, 0);
                    field_0x100f = 1;
                }
                attention_info.flags = 0;
            } else {
                if (mPlayerActorMngr.getActorP() && !mTwilight) {
                    if (field_0xff8 == 0) {
                        field_0x1011 = 0;
                    }
                    mJntAnm.lookPlayer(0);
                    if (!chkActorInSight(mPlayerActorMngr.getActorP(), mAttnFovY, mCurAngle.y) &&
                        field_0xff8 == 0)
                    {
                        mJntAnm.lookNone(0);
                    }
                    if (!srchPlayerActor() && home.angle.y == mCurAngle.y) {
                        mMode = MODE_INIT;
                    }
                } else {
                    mJntAnm.lookNone(0);
                    if (!mTwilight) {
                        srchPlayerActor();
                    }
                }
            }
        }
        break;
    case MODE_EXIT:
    case MODE_4:
        break;
    }

    return 1;
}

int daNpc_Uri_c::sitWait(void* param_0) {
    int sVar2 = mpHIO->m.field_0x92;

    switch (mMode) {
    case MODE_ENTER:
    case MODE_INIT:
        if (!mStagger.checkStagger()) {
            switch (mType) {
            case TYPE_1:
                if (field_0x100c != 0) {
                    mFaceMotionSeqMngr.setNo(17, -1.0f, 0, 0);
                    mMotionSeqMngr.setNo(26, 4.0f, 0, 0);
                } else {
                    mFaceMotionSeqMngr.setNo(17, -1.0f, 0, 0);
                    mMotionSeqMngr.setNo(5, -1.0f, 0, 0);
                    field_0xff0 = cLib_getRndValue(sVar2 * 0.5f, sVar2 * 1.5f);
                }
                break;
            case TYPE_4:
                /* dSv_event_flag_c::M_028 - Cutscene - [cutscene: 14] restore mountain spirit -
                 * Reuinion with Colin et al. */
                if (daNpcT_chkEvtBit(0x3D)
                    /* dSv_event_flag_c::F_0330 - Ordon Village - Meet again with Uli for the first
                       time (first forced conversation) */
                    && (daNpcT_chkEvtBit(0x14A)
                        /* dSv_event_flag_c::F_0516 - Ordon Village - Told Uli directly about having
                           found kids */
                        || daNpcT_chkEvtBit(0x204)))
                {
                    mFaceMotionSeqMngr.setNo(17, -1.0f, 0, 0);
                    mMotionSeqMngr.setNo(3, -1.0f, 0, 0);
                } else {
                    mFaceMotionSeqMngr.setNo(10, -1.0f, 0, 0);
                    mMotionSeqMngr.setNo(18, -1.0f, 0, 0);
                }
                break;
            }
            mMode = MODE_RUN;
        }
    case MODE_RUN:
        /* dSv_event_flag_c::F_0330 - Ordon Village - Meet again with Uli for the first time
                                                      (first forced conversation) */
        if (mType == TYPE_4 && !daNpcT_chkEvtBit(0x14A)) {
            mSpeakEvent = true;
            field_0xe33 = true;
        }
        if (!mStagger.checkStagger()) {
            if (field_0x100c != 0) {
                mJntAnm.lookNone(0);
                if (mMotionSeqMngr.getNo() == 26 && mMotionSeqMngr.checkEndSequence()) {
                    field_0x100c = 0;
                    mMode = MODE_INIT;
                }
                attention_info.flags = 0;
            } else {
                if (mPlayerActorMngr.getActorP() && !mTwilight) {
                    mJntAnm.lookPlayer(0);
                    if (!chkActorInSight(mPlayerActorMngr.getActorP(), mAttnFovY, mCurAngle.y)) {
                        mJntAnm.lookNone(0);
                    }
                    if (!srchPlayerActor() && home.angle.y == mCurAngle.y) {
                        mMode = MODE_INIT;
                    }
                } else {
                    mJntAnm.lookNone(0);
                    if (mType == TYPE_1) {
                        if (field_0xff0 == 0) {
                            if (mMotionSeqMngr.getNo() == 7) {
                                if (mMotionSeqMngr.checkEndSequence()) {
                                    mMode = MODE_INIT;
                                }
                            } else {
                                mMotionSeqMngr.setNo(7, -1.0f, 0, 0);
                            }
                        } else {
                            field_0xff0 -= 1;
                        }
                    }
                    if (!mTwilight && srchPlayerActor()) {
                        mMode = MODE_INIT;
                    }
                }
            }
            switch (mJntAnm.getMode()) {
            case 0:
                if (mType == TYPE_4) {
                    fopAc_ac_c* actor = mActorMngr[0].getActorP();
                    if (actor) {
                        mJntAnm.lookPos(&actor->eyePos, 0);
                    }
                }
                break;
            }
        }
        break;
    case MODE_EXIT:
    case MODE_4:
        break;
    }

    return 1;
}

int daNpc_Uri_c::talk(void* param_0) {
    switch (mMode) {
    case MODE_ENTER:
    case MODE_INIT:
        if (!mStagger.checkStagger()) {
            if (field_0x100b != 0) {
                initTalk(0x13, NULL);
            } else {
                initTalk(mFlowNodeNo, NULL);
            }
            mMode = MODE_RUN;
        }
    case MODE_RUN:
        if (!mStagger.checkStagger()) {
            if (mTwilight || mPlayerAngle == mCurAngle.y || field_0x100d != 0 || field_0x100f != 0)
            {
                if (talkProc(NULL, 0, NULL, 0) && mFlow.checkEndFlow() != 0) {
                    mPlayerActorMngr.entry(daPy_getPlayerActorClass());
                    dComIfGp_event_reset();
                    if (field_0x100b != 0) {
                        field_0x100b = 0;
                        field_0x100c = 1;
                        field_0x100d = 1;
                    }
                    mMode = MODE_EXIT;
                }
                mJntAnm.lookPlayer(0);
                if (mTwilight != 0) {
                    mJntAnm.lookNone(0);
                }
            } else {
                mJntAnm.lookPlayer(0);
                if (field_0x1008 != 0) {
                    step(mPlayerAngle, 17, 24, 15, 0);
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

static int daNpc_Uri_Create(void* i_this) {
    return static_cast<daNpc_Uri_c*>(i_this)->create();
}

static int daNpc_Uri_Delete(void* i_this) {
    return static_cast<daNpc_Uri_c*>(i_this)->Delete();
}

static int daNpc_Uri_Execute(void* i_this) {
    return static_cast<daNpc_Uri_c*>(i_this)->Execute();
}

static int daNpc_Uri_Draw(void* i_this) {
    return static_cast<daNpc_Uri_c*>(i_this)->Draw();
}

static int daNpc_Uri_IsDelete(void* i_this) {
    return 1;
}

static actor_method_class daNpc_Uri_MethodTable = {
    (process_method_func)daNpc_Uri_Create,  (process_method_func)daNpc_Uri_Delete,
    (process_method_func)daNpc_Uri_Execute, (process_method_func)daNpc_Uri_IsDelete,
    (process_method_func)daNpc_Uri_Draw,
};

actor_process_profile_definition g_profile_NPC_URI = {
    fpcLy_CURRENT_e,         // mLayerID
    7,                       // mListID
    fpcPi_CURRENT_e,         // mListPrio
    PROC_NPC_URI,            // mProcName
    &g_fpcLf_Method.base,    // sub_method
    sizeof(daNpc_Uri_c),     // mSize
    0,                       // mSizeOther
    0,                       // mParameters
    &g_fopAc_Method.base,    // sub_method
    378,                     // mPriority
    &daNpc_Uri_MethodTable,  // sub_method
    0x00040107,              // mStatus
    fopAc_NPC_e,             // mActorType
    fopAc_CULLBOX_CUSTOM_e,  // cullType
};

AUDIO_INSTANCES;
