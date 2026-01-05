/**
 * @file d_a_npc_moi.cpp
 *
 */

#include "d/dolzel_rel.h"  // IWYU pragma: keep

#include "SSystem/SComponent/c_counter.h"
#include "Z2AudioLib/Z2Instances.h"
#include "d/actor/d_a_ep.h"
#include "d/actor/d_a_npc4.h"
#include "d/actor/d_a_npc_moi.h"
#include "d/actor/d_a_npc_uri.h"

const daNpc_Moi_HIOParam daNpc_Moi_Param_c::m = {
    200.0f,   // attention_offset
    -3.0f,    // gravity
    1.0f,     // scale
    400.0f,   // real_shadow_size
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
    12.0f,    // morf_frame
    3,        // talk_distance
    6,        // talk_angle
    5,        // attention_distance
    6,        // attention_angle
    110.0f,   // fov
    0.0f,     // search_distance
    0.0f,     // search_height
    0.0f,     // search_depth
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
    350.0f,   // field_0x8c
    300.0f,   // field_0x90
    -100.0f,  // field_0x94
    90,       // field_0x98
    200,      // field_0x9a
    235,      // field_0x9c
    125,      // field_0x9e
    0,        // field_0xa0
    0,        // field_0xa2
    1.0f,     // field_0xa4
    1000.0f,  // field_0xa8
    300.0f,   // field_0xac
    -300.0f,  // field_0xb0
    30.0f,    // field_0xb4
    60.0f,    // field_0xb8
};

#if DEBUG
daNpc_Moi_HIO_c::daNpc_Moi_HIO_c() {
    m = daNpc_Moi_Param_c::m;
}

void daNpc_Moi_HIO_c::listenPropertyEvent(const JORPropertyEvent* event) {
    // TODO
}

void daNpc_Moi_HIO_c::genMessage(JORMContext* ctext) {
    // TODO
}
#endif

static int l_bmdData[8][2] = {
    {11, 1}, {29, 3}, {30, 3}, {31, 3}, {13, 1}, {33, 3}, {12, 1}, {32, 3},
};

static daNpcT_evtData_c l_evtList[4] = {
    {"", 0},
    {"DEFAULT_GETITEM", 0},
    {"NO_RESPONSE", 0},
    {"DELIVERED_SW", 2},
};

static char* l_resNameList[6] = {
    "", "Moi", "Moi1", "Moi2", "Moi3", "Moi_p1",
};

static s8 l_loadResPtrn0[3] = {
    1,
    2,
    -1,
};

static s8 l_loadResPtrn1[3] = {
    1,
    3,
    -1,
};

static s8 l_loadResPtrn3[4] = {
    1,
    3,
    4,
    -1,
};

static s8 l_loadResPtrn4[3] = {
    1,
    5,
    -1,
};

static s8 l_loadResPtrn9[5] = {
    1, 2, 3, 4, -1,
};

static s8* l_loadResPtrnList[7] = {
    l_loadResPtrn0, l_loadResPtrn4, l_loadResPtrn1, l_loadResPtrn3,
    l_loadResPtrn0, l_loadResPtrn0, l_loadResPtrn9,
};

static daNpcT_faceMotionAnmData_c l_faceMotionAnmData[20] = {
    {-1, 0, 0, 19, 2, 1, TRUE},  {6, 0, 1, 19, 2, 1, TRUE},   {9, 0, 2, 41, 0, 2, FALSE},
    {10, 0, 2, 42, 0, 2, FALSE}, {8, 0, 2, 40, 0, 2, FALSE},  {7, 0, 2, 39, 0, 2, FALSE},
    {11, 0, 2, 43, 0, 2, FALSE}, {7, 0, 3, 40, 0, 3, FALSE},  {8, 0, 3, 41, 0, 3, FALSE},
    {12, 2, 2, 44, 2, 2, FALSE}, {16, 2, 2, 48, 2, 2, FALSE}, {-1, 0, 0, -1, 0, 0, FALSE},
    {14, 2, 2, 46, 2, 2, FALSE}, {15, 2, 2, 47, 2, 2, FALSE}, {13, 2, 2, 45, 2, 2, FALSE},
    {10, 2, 3, 43, 2, 3, FALSE}, {11, 2, 3, 44, 2, 3, FALSE}, {9, 2, 3, 42, 2, 3, FALSE},
    {6, 2, 3, 39, 2, 3, FALSE},  {18, 0, 2, 49, 0, 2, FALSE},
};

static daNpcT_motionAnmData_c l_motionAnmData[35] = {
    {8, 2, 1, 16, 0, 1, 1, 0},  {29, 2, 2, 16, 0, 1, 1, 0}, {30, 2, 2, 16, 0, 1, 1, 0},
    {6, 0, 2, 16, 0, 1, 1, 0},  {17, 2, 2, 35, 2, 2, 0, 0}, {31, 2, 2, 16, 0, 1, 1, 0},
    {23, 0, 2, 16, 0, 1, 1, 0}, {28, 0, 2, 16, 0, 1, 1, 0}, {24, 0, 2, 16, 0, 1, 1, 0},
    {25, 0, 2, 16, 0, 1, 1, 0}, {26, 0, 2, 16, 0, 1, 1, 0}, {27, 0, 2, 16, 0, 1, 1, 0},
    {32, 2, 2, 16, 0, 1, 1, 0}, {7, 0, 1, 16, 0, 1, 1, 0},  {20, 2, 3, 16, 0, 1, 1, 0},
    {17, 0, 3, 16, 0, 1, 1, 0}, {18, 0, 3, 16, 0, 1, 1, 0}, {19, 0, 3, 16, 0, 1, 1, 0},
    {21, 2, 3, 16, 0, 1, 1, 0}, {-1, 0, 0, -1, 0, 0, 0, 0}, {12, 0, 3, 16, 0, 1, 1, 0},
    {13, 0, 3, 16, 0, 1, 1, 0}, {26, 2, 3, 16, 0, 1, 1, 0}, {25, 0, 3, 16, 0, 1, 1, 0},
    {22, 0, 3, 16, 0, 1, 1, 0}, {23, 0, 3, 16, 0, 1, 1, 0}, {24, 0, 3, 16, 0, 1, 1, 0},
    {14, 0, 3, 16, 0, 1, 1, 0}, {15, 2, 3, 16, 0, 1, 1, 0}, {16, 2, 3, 16, 0, 1, 1, 0},
    {19, 0, 2, 16, 0, 1, 1, 0}, {21, 0, 2, 16, 0, 1, 1, 0}, {20, 2, 2, 16, 0, 1, 1, 0},
    {3, 2, 4, 16, 0, 1, 1, 0},  {21, 2, 3, 16, 0, 1, 1, 0},
};

static daNpcT_MotionSeqMngr_c::sequenceStepData_c l_faceMotionSequenceData[88] = {
    {1, -1, 1},  {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},  {2, -1, 1},  {12, 0, 0},  {-1, 0, 0},
    {-1, 0, 0},  {3, -1, 1},  {13, 0, 0},  {-1, 0, 0},  {-1, 0, 0},  {4, -1, 1},  {14, 0, 0},
    {-1, 0, 0},  {-1, 0, 0},  {5, -1, 1},  {9, 0, 0},   {-1, 0, 0},  {-1, 0, 0},  {6, -1, 1},
    {10, 0, 0},  {-1, 0, 0},  {-1, 0, 0},  {7, -1, 1},  {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},
    {18, -1, 0}, {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},  {8, -1, 1},  {16, 0, 0},  {-1, 0, 0},
    {-1, 0, 0},  {17, -1, 0}, {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},
    {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},  {16, -1, 0},
    {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},  {15, -1, 0}, {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},
    {9, -1, 0},  {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},  {10, -1, 0}, {-1, 0, 0},  {-1, 0, 0},
    {-1, 0, 0},  {14, -1, 0}, {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},  {11, -1, 1}, {-1, 0, 0},
    {-1, 0, 0},  {-1, 0, 0},  {12, -1, 0}, {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},  {13, -1, 0},
    {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},  {19, -1, 1}, {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},
    {0, -1, 0},  {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},
};

static daNpcT_MotionSeqMngr_c::sequenceStepData_c l_motionSequenceData[196] = {
    {0, -1, 0},  {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},  {8, -1, 1},  {2, 0, 0},   {-1, 0, 0},
    {-1, 0, 0},  {1, -1, 0},  {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},  {9, -1, 1},  {1, 0, 0},
    {-1, 0, 0},  {-1, 0, 0},  {4, -1, 0},  {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},  {10, -1, 1},
    {1, 0, 0},   {-1, 0, 0},  {-1, 0, 0},  {11, -1, 1}, {5, 0, 0},   {-1, 0, 0},  {-1, 0, 0},
    {2, -1, 0},  {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},  {7, -1, 1},  {1, 0, 0},   {-1, 0, 0},
    {-1, 0, 0},  {6, -1, 1},  {0, 0, 0},   {-1, 0, 0},  {-1, 0, 0},  {22, -1, 0}, {-1, 0, 0},
    {-1, 0, 0},  {-1, 0, 0},  {25, -1, 1}, {22, 0, 0},  {-1, 0, 0},  {-1, 0, 0},  {28, -1, 0},
    {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},  {27, 0, 1},  {29, 0, 0},  {-1, 0, 0},  {-1, 0, 0},
    {14, -1, 0}, {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},  {15, -1, 1}, {14, 0, 0},  {-1, 0, 0},
    {-1, 0, 0},  {23, -1, 1}, {22, 0, 0},  {-1, 0, 0},  {-1, 0, 0},  {16, -1, 1}, {14, 0, 0},
    {-1, 0, 0},  {-1, 0, 0},  {17, -1, 1}, {14, 0, 0},  {-1, 0, 0},  {-1, 0, 0},  {33, -1, 0},
    {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},
    {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},
    {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},
    {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},
    {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},
    {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},
    {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},
    {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},
    {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},  {31, -1, 1}, {32, 0, 0},  {-1, 0, 0},  {-1, 0, 0},
    {32, -1, 0}, {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},  {29, -1, 0}, {-1, 0, 0},  {-1, 0, 0},
    {-1, 0, 0},  {20, -1, 1}, {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},  {21, -1, 1}, {-1, 0, 0},
    {-1, 0, 0},  {-1, 0, 0},  {26, -1, 1}, {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},  {18, -1, 0},
    {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},
    {25, -1, 1}, {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},  {24, -1, 1}, {-1, 0, 0},  {-1, 0, 0},
    {-1, 0, 0},  {13, -1, 1}, {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},  {12, -1, 0}, {-1, 0, 0},
    {-1, 0, 0},  {-1, 0, 0},  {3, -1, 0},  {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},  {30, -1, 1},
    {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},  {18, 4, 0},  {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},
};

char* daNpc_Moi_c::mCutNameList[5] = {
    "", "DELIVERED_SW", "APPEARANCE_MOI", "CONVERSATION_WITH_URI", "FIND_WOLF",
};

daNpc_Moi_c::cutFunc daNpc_Moi_c::mCutList[5] = {
    NULL,
    &daNpc_Moi_c::cutDeliveredSw,
    &daNpc_Moi_c::cutAppearanceMoi,
    &daNpc_Moi_c::cutConversationWithUri,
    &daNpc_Moi_c::cutFindWolf,
};

enum Motion {
    /* 0x00 */ MOT_UNK_0 = 0,
    /* 0x01 */ MOT_UNK_1 = 1,
    /* 0x02 */ MOT_UNK_2 = 2,
    /* 0x03 */ MOT_UNK_3 = 3,
    /* 0x04 */ MOT_UNK_4 = 4,
    /* 0x05 */ MOT_UNK_5 = 5,
    /* 0x08 */ MOT_UNK_8 = 8,
    /* 0x09 */ MOT_UNK_9 = 9,
    /* 0x0A */ MOT_UNK_10 = 10,
    /* 0x0B */ MOT_UNK_11 = 11,
    /* 0x0E */ MOT_UNK_14 = 14,
    /* 0x0F */ MOT_UNK_15 = 15,
    /* 0x10 */ MOT_UNK_16 = 16,
    /* 0x12 */ MOT_UNK_18 = 18,
    /* 0x13 */ MOT_UNK_19 = 19,
    /* 0x15 */ MOT_UNK_21 = 21,
    /* 0x25 */ MOT_UNK_37 = 37,
    /* 0x26 */ MOT_UNK_38 = 38,
    /* 0x27 */ MOT_UNK_39 = 39,
    /* 0x28 */ MOT_UNK_40 = 40,
    /* 0x2B */ MOT_UNK_43 = 43,
    /* 0x2D */ MOT_UNK_45 = 45,
    /* 0x2F */ MOT_UNK_47 = 47,
};

daNpc_Moi_c::~daNpc_Moi_c() {
    if (mpMorf[0] != 0) {
        mpMorf[0]->stopZelAnime();
    }
    deleteRes(l_loadResPtrnList[mType], (char const**)l_resNameList);
}

int daNpc_Moi_c::create() {
    static const int heapSize[] = {0, 0x5C70, 0x7CB0, 0x7CA0, 0x5C70, 0x5C80, 0};

    daNpcT_ct(this, daNpc_Moi_c, (daNpcT_faceMotionAnmData_c const*)l_faceMotionAnmData,
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
        mCcStts.Init(mpHIO->m.common.weight, 0, this);
        for (int i = 0; i < 4; i++) {
            mCyl[i].Set(mCcDCyl);
            mCyl[i].SetStts(&mCcStts);
            mCyl[i].SetTgHitCallback(tgHitCallBack);
        }
        mSph.Set(mCcDSph);
        mSph.SetStts(&mCcStts);
        if (mType == TYPE_3) {
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

int daNpc_Moi_c::CreateHeap() {
    static int const bmdTypeList[6] = {6, 4, 1, 2, 3, 5};

    int idx = 0;
    if (chkMoiN()) {
        idx = 7;
    }

    J3DModelData* modelData = static_cast<J3DModelData*>(
        dComIfG_getObjectRes(l_resNameList[l_bmdData[idx][1]], l_bmdData[idx][0]));
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

    if (chkMoiN()) {
        for (int i = 2; i < 6; i++) {
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
    }

    if (setFaceMotionAnm(1, false) && setMotionAnm(0, 0.0f, 0)) {
        return 1;
    }

    return 0;
}

int daNpc_Moi_c::Delete() {
    fopAcM_GetID(this);
    this->~daNpc_Moi_c();
    return 1;
}

int daNpc_Moi_c::Execute() {
    return daNpcT_c::execute();
}

int daNpc_Moi_c::Draw() {
    J3DModelData* modelData;
    if (mpMatAnm[0] != NULL) {
        modelData = mpMorf[0]->getModel()->getModelData();
        modelData->getMaterialNodePointer(getEyeballMaterialNo())->setMaterialAnm(mpMatAnm[0]);
    }

    if (mpModel[5] && field_0x1668) {
        ep_class* actor = (ep_class*)mActorMngr[0].getActorP();
        if (actor) {
            actor->field_0x634 = field_0x1628;
            actor->current.pos = field_0x1628;
            GXColor color;
            color.r = mpHIO->m.field_0x9c;
            color.g = mpHIO->m.field_0x9e;
            color.b = mpHIO->m.field_0xa0;
            color.a = (u8)actor->field_0x5c0;
            dKy_BossLight_set(&field_0x1628, &color, mpHIO->m.field_0xa4, 1);
        }
    }

    return daNpcT_c::draw(0, 0, mRealShadowSize, NULL, 100.0f, 0, 0, 0);
}

int daNpc_Moi_c::createHeapCallBack(fopAc_ac_c* i_this) {
    return static_cast<daNpc_Moi_c*>(i_this)->CreateHeap();
}

int daNpc_Moi_c::ctrlJointCallBack(J3DJoint* param_0, int param_1) {
    if (param_1 == 0) {
        J3DModel* model = j3dSys.getModel();

        daNpc_Moi_c* i_this = reinterpret_cast<daNpc_Moi_c*>(model->getUserArea());
        if (i_this != 0) {
            i_this->ctrlJoint(param_0, model);
        }
    }

    return 1;
}

u8 daNpc_Moi_c::getType() {
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

BOOL daNpc_Moi_c::isDelete() {
    bool ret;
    switch (mType) {
    case TYPE_0:
        return FALSE;
    case TYPE_1:
        return FALSE;
    case TYPE_2:
        return !chkPlayerGetWoodShield();
    case TYPE_3:
        return FALSE;
    case TYPE_4:
        ret = TRUE;
        if (daNpcT_chkEvtBit(0x258)) {
            bool bVar2 = false;
            if (getBitSW() != 0xFF) {
                if (dComIfGs_isSwitch(getBitSW(), fopAcM_GetRoomNo(this))) {
                    bVar2 = true;
                }
            }
            if (!bVar2) {
                ret = FALSE;
            }
        }
        return ret;
    case TYPE_5:
        ret = FALSE;
        if (!daNpcT_chkEvtBit(0x271) || daNpcT_chkEvtBit(0xb8)) {
            ret = TRUE;
        }
        return ret;
    default:
        return FALSE;
    }
}

void daNpc_Moi_c::reset() {
    int iVar8;
    cXyz cStack_30;
    csXyz cStack_80;

    int iVar1 = (u8*)&field_0x1670 - (u8*)&field_0x1610;
    if (mpMatAnm[0] != NULL) {
        mpMatAnm[0]->initialize();
    }
    initialize();
    for (int i = 0; i < 4; i++) {
        mActorMngr[i].initialize();
    }
    if (getPathID() != 0xFF) {
        mPath.initialize();
        mPath.setPathInfo(getPathID(), fopAcM_GetRoomNo(this), 0);
    }

    memset(&field_0x1610, 0, iVar1);
    cStack_80.setall(0.0f);
    cStack_80.y = home.angle.y;

    switch (mType) {
    case TYPE_0:
        break;
    case TYPE_1:
        if (mPath.getPathInfo()) {
            home.pos = mPath.getPntPos(0);
            cStack_30 = mPath.getPntPos(1);
            home.angle.y = cLib_targetAngleY(&home.pos, &cStack_30);
            cStack_80.y = home.angle.y;
        }
        setPos(home.pos);
        field_0x166b = 1;
        break;
    case TYPE_2:
        if (daNpcT_chkEvtBit(0xcd)) {
            JUT_ASSERT(1932, NULL != mPath.getPathInfo());
            iVar8 = g_Counter.mCounter0 % mPath.getNumPnts();
            cStack_30 = mPath.getPntPos(iVar8);
            setPos(cStack_30);
            mPath.setIdx(iVar8);
            mPath.setNextIdx();
            cStack_30 = mPath.getPntPos(mPath.getIdx());
            cStack_80.y = cLib_targetAngleY(&current.pos, &cStack_30);
        }
        field_0x1666 = 1;
        field_0x1668 = 1;
        break;
    case TYPE_3:
        field_0x166f = 1;
        break;
    case TYPE_4:
    case TYPE_5:
        break;
    }

    field_0x15c0 = -1;
    setAngle(cStack_80);
}

BOOL daNpc_Moi_c::chkMoiN() {
    return (mType == TYPE_2 || mType == TYPE_3);
}

void daNpc_Moi_c::afterJntAnm(int param_1) {
    if (param_1 == 1) {
        mDoMtx_stack_c::YrotM(mStagger.getAngleZ(1));
        mDoMtx_stack_c::ZrotM(-mStagger.getAngleX(1));
    } else if (param_1 == 4) {
        mDoMtx_stack_c::YrotM(mStagger.getAngleZ(0));
        mDoMtx_stack_c::ZrotM(mStagger.getAngleX(0));
    }
}

void daNpc_Moi_c::setParam() {
    selectAction();
    srchActors();

    int uVar5 = (fopAc_AttnFlag_SPEAK_e | fopAc_AttnFlag_TALK_e);
    s16 talk_distance = mpHIO->m.common.talk_distance;
    s16 talk_angle = mpHIO->m.common.talk_angle;
    s16 attention_distance = mpHIO->m.common.attention_distance;
    s16 attention_angle = mpHIO->m.common.attention_angle;

    if (mType == TYPE_2) {
        if (field_0x166d) {
            talk_distance = 1;
            talk_angle = 6;
            attention_distance = 1;
            attention_angle = 6;
            uVar5 = fopAc_AttnFlag_UNK_0x800000 | fopAc_AttnFlag_SPEAK_e;
        } else {
            talk_distance = 6;
            talk_angle = 6;
            attention_distance = 19;
            attention_angle = 6;
            uVar5 = fopAc_AttnFlag_LOCK_e;
        }
    } else if (mType == TYPE_1) {
        talk_distance = 5;
        attention_distance = 7;
    }

    if (daPy_getPlayerActorClass()->checkHorseRide()) {
        if (talk_distance < 7) {
            talk_distance = 7;
        }
        if (attention_distance < 9) {
            attention_distance = 9;
        }
    }

    if (mType == TYPE_2) {
        attention_info.distances[fopAc_attn_LOCK_e] = 0x35;
        attention_info.distances[fopAc_attn_TALK_e] = attention_info.distances[fopAc_attn_LOCK_e];
        attention_info.distances[fopAc_attn_SPEAK_e] =
            daNpcT_getDistTableIdx(talk_distance, talk_angle);
        attention_info.flags = 0;
        fopAcM_OffStatus(this, 0x100);
    } else {
        attention_info.distances[fopAc_attn_LOCK_e] =
            daNpcT_getDistTableIdx(attention_distance, attention_angle);
        attention_info.distances[fopAc_attn_TALK_e] = attention_info.distances[fopAc_attn_LOCK_e];
        attention_info.distances[fopAc_attn_SPEAK_e] =
            daNpcT_getDistTableIdx(talk_distance, talk_angle);
        attention_info.flags = uVar5;
    }

    scale.set(mpHIO->m.common.scale, mpHIO->m.common.scale, mpHIO->m.common.scale);
    mCcStts.SetWeight(mpHIO->m.common.weight);
    mCylH = mpHIO->m.common.height;
    mWallR = mpHIO->m.common.width;
    mAttnFovY = mpHIO->m.common.fov;
    if (mType == TYPE_2) {
        mAttnFovY = 135.0f;
    }
    mAcchCir.SetWallR(mWallR);
    mAcchCir.SetWallH(mpHIO->m.common.knee_length);
    mRealShadowSize = mpHIO->m.common.real_shadow_size;
    if (mType == TYPE_1) {
        mRealShadowSize = 600.0f;
    } else if (mType == TYPE_2) {
        mRealShadowSize = 600.0f;
        if (field_0x166d) {
            mRealShadowSize = 800.0f;
        }
    }
    gravity = mpHIO->m.common.gravity;
    mExpressionMorfFrame = mpHIO->m.common.expression_morf_frame;
    mMorfFrames = mpHIO->m.common.morf_frame;
    if (mType == TYPE_3) {
        mAcch.SetGrndNone();
        mAcch.SetWallNone();
        gravity = 0.0f;
    }

    setInSw();
    pullOutSw();
    setSwAT();
}

BOOL daNpc_Moi_c::checkChangeEvt() {
    if (!chkAction(&daNpc_Moi_c::talk)) {
        mPreItemNo = 0;
        if (dComIfGp_event_chkTalkXY()) {
            if (dComIfGp_evmng_ChkPresentEnd()) {
                mEvtNo = 2;
                evtChange();
            }
            return TRUE;
        }
    }
    return FALSE;
}

void daNpc_Moi_c::setAfterTalkMotion() {
    int iVar2 = MOT_UNK_21;
    switch (mFaceMotionSeqMngr.getNo()) {
    case MOT_UNK_1:
        iVar2 = MOT_UNK_18;
        break;
    case MOT_UNK_2:
        iVar2 = MOT_UNK_19;
        break;
    case MOT_UNK_3:
        iVar2 = MOT_UNK_16;
        break;
    case MOT_UNK_4:
        iVar2 = MOT_UNK_14;
        break;
    case MOT_UNK_5:
        iVar2 = MOT_UNK_15;
        break;
    case MOT_UNK_9:
        return;
    }
    mFaceMotionSeqMngr.setNo(iVar2, -1.0f, 0, 0);
}

void daNpc_Moi_c::srchActors() {
    switch (mType) {
    case TYPE_1:
        break;
    case TYPE_2:
        if (mActorMngr[2].getActorP() == NULL) {
            mActorMngr[2].entry(getNearestActorP(0x15a));
        }
        break;
    case TYPE_5:
        if (mActorMngr[3].getActorP() == NULL) {
            mActorMngr[3].entry(getNearestActorP(0x26e));
        }
    }
}

BOOL daNpc_Moi_c::evtTalk() {
    if (chkAction(&daNpc_Moi_c::talk)) {
        (this->*field_0x161c)(NULL);
    } else {
        setAction(&daNpc_Moi_c::talk);
    }
    return TRUE;
}

BOOL daNpc_Moi_c::evtCutProc() {
    int staffId = dComIfGp_getEventManager().getMyStaffId("Moi", this, -1);
    if (staffId != -1) {
        mStaffId = staffId;
        int actIdx =
            dComIfGp_getEventManager().getMyActIdx(mStaffId, (char**)mCutNameList, 5, 0, 0);
        if ((this->*(mCutList[actIdx]))(mStaffId) != 0) {
            dComIfGp_getEventManager().cutEnd(mStaffId);
        }
        return true;
    }
    return false;
}

void daNpc_Moi_c::action() {
    fopAc_ac_c* hitActor = NULL;
    int i;
    for (i = 0; i < 4; i++) {
        hitActor = hitChk(&mCyl[i], 0xffffffff);
        if (hitActor) {
            break;
        }
    }

    if (hitActor) {
        if (mType == TYPE_2) {
            if (field_0x166d == 0 && field_0x166c == 0) {
                setSSlash(field_0x1669 == 0);
            }
        } else {
            cCcD_ObjHitInf* hit_obj = mCyl[i].GetTgHitObj();
            if (hit_obj->ChkAtType(AT_TYPE_THROW_OBJ)) {
                mStagger.setParam(this, hitActor, mCurAngle.y);
                setDamage(0, 21, 0);
                mDamageTimerStart = 0;
                mJntAnm.lookNone(1);
            }
        }
    }

    if (mStagger.checkRebirth()) {
        mStagger.initialize();
        mMode = MODE_INIT;
    }

    if (field_0x1610 != NULL) {
        if (field_0x161c == field_0x1610) {
            (this->*field_0x161c)(NULL);
        } else {
            setAction(field_0x1610);
        }
    }

    if (mSph.ChkAtHit() && fopAcM_GetProfName(mSph.GetAtHitAc()) == PROC_ALINK) {
        /* dSv_event_tmp_flag_c::T_0047 - Ordon Village - Right after wrestling with Bo
                                          (Used to connect with conversation after wearing clothes)
         */
        daNpcF_onTmpBit(0x47);
    }
}

void daNpc_Moi_c::beforeMove() {
    fopAcM_OffStatus(this, 0x8000000);
    if (checkHide()) {
        fopAcM_OnStatus(this, 0x8000000);
    }

    if (checkHide() || mNoDraw != 0) {
        attention_info.flags = 0;
    }
}

NPC_MOI_HIO_CLASS l_HIO;

void daNpc_Moi_c::setAttnPos() {
    cXyz acStack_3c(-30.0f, 10.0f, 0.0f);

    mStagger.calc(0);

    f32 dVar6 = cM_s2rad(mCurAngle.y - field_0xd7e.y);

    if (mType == 3) {
        mJntAnm.setParam(this, mpMorf[0]->getModel(), &acStack_3c, getBackboneJointNo(),
                         getNeckJointNo(), getHeadJointNo(), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
                         0.0f, 0.0f, mpHIO->m.common.neck_rotation_ratio, dVar6, NULL);
    } else {
        mJntAnm.setParam(this, mpMorf[0]->getModel(), &acStack_3c, getBackboneJointNo(),
                         getNeckJointNo(), getHeadJointNo(), mpHIO->m.common.body_angleX_min,
                         mpHIO->m.common.body_angleX_max, mpHIO->m.common.body_angleY_min,
                         mpHIO->m.common.body_angleY_max, mpHIO->m.common.head_angleX_min,
                         mpHIO->m.common.head_angleX_max, mpHIO->m.common.head_angleY_min,
                         mpHIO->m.common.head_angleY_max, mpHIO->m.common.neck_rotation_ratio,
                         dVar6, NULL);
    }
    mJntAnm.calcJntRad(0.2f, 1.0f, dVar6);

    setMtx();
    mDoMtx_stack_c::copy(mpMorf[0]->getModel()->getAnmMtx(getHeadJointNo()));
    mDoMtx_stack_c::multVec(&acStack_3c, &eyePos);

    mJntAnm.setEyeAngleX(eyePos, 1.0f, 0);
    mJntAnm.setEyeAngleY(eyePos, mCurAngle.y + field_0xd8a.y, 1, 1.0f, 0);

    acStack_3c.set(0.0f, 0.0f, 0.0f);
    acStack_3c.y = mpHIO->m.common.attention_offset;

    if (mType == TYPE_3) {
        acStack_3c.set(0.0f, 140.0f, -60.0f);
    } else if (field_0x166d != 0) {
        acStack_3c.set(0.0f, 40.0f, 230.0f);
    }

    mDoMtx_stack_c::YrotS(mCurAngle.y);
    mDoMtx_stack_c::multVec(&acStack_3c, &acStack_3c);
    attention_info.position = current.pos + acStack_3c;

    if (field_0x166d != 0) {
        ep_class* actor = (ep_class*)mActorMngr[0].getActorP();
        if (actor) {
            actor->field_0x5a4 = 10;
        }
    } else {
        torch();
    }

    setFootPos();
}

void daNpc_Moi_c::setCollision() {
    cXyz cStack_50;
    cXyz cStack_5c;
    cXyz cStack_68;

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

        mCyl[0].SetCoSPrm(0x79);
        mCyl[0].SetTgType(tgType);
        mCyl[0].SetTgSPrm(tgSPrm);
        mCyl[0].OnTgNoHitMark();
        cStack_50.set(0.0f, 0.0f, 0.0f);
        f32 cylHeight = mCylH;
        f32 cylRadius = mWallR;

        if (&daNpc_Moi_c::rest == field_0x1610) {
            cStack_50.set(0.0f, 0.0f, 60.0f);
            cylHeight = 80.0f;
            cylRadius = 50.0f;
        }

        mDoMtx_stack_c::YrotS(mCurAngle.y);
        mDoMtx_stack_c::multVec(&cStack_50, &cStack_50);
        cStack_50 += current.pos;
        mCyl[0].SetH(cylHeight);
        mCyl[0].SetR(cylRadius);
        mCyl[0].SetC(cStack_50);
        dComIfG_Ccsp()->Set(&mCyl[0]);
    }

    if (field_0x166a) {
        cStack_5c.set(0.0f, 0.0f, -70.0f);
        mDoMtx_stack_c::copy(mpMorf[0]->getModel()->getAnmMtx(17));
        mDoMtx_stack_c::multVec(&cStack_5c, &cStack_68);
        mSph.SetAtType(0x400);
        mSph.SetAtSPrm(13);
        mSph.SetAtAtp(1);
        mSph.SetAtSe(dCcD_SE_METAL);
        mSph.SetTgSPrm(0);
        mSph.SetCoSPrm(0);
        mSph.SetC(cStack_68);
        mSph.SetR(60.0f);
        dComIfG_Ccsp()->Set(&mSph);
    }

    for (int i = 0; i < 4; i++) {
        mCyl[i].ClrCoHit();
        mCyl[i].ClrTgHit();
    }
    mCyl2.ClrCoHit();
}

int daNpc_Moi_c::drawDbgInfo() {
    return 0;
}

void daNpc_Moi_c::drawOtherMdl() {
    static int const jointNo[6] = {2, 17, 23, 24, 25, 12};

    Mtx mtx;
    J3DModel* model = mpMorf[0]->getModel();

    for (int i = 0; i < 6; i++) {
        if ((field_0x166f == 0 || (u32)i > 1) && mpModel[i] != NULL) {
            if ((i == 2 && field_0x1666 != 0) || (i == 3 && field_0x1666 != 0) ||
                (i == 4 && field_0x1666 != 0) || (i == 1 && field_0x166b != 0) ||
                (i == 5 && field_0x1668 != 0) || i == 0)
            {
                g_env_light.setLightTevColorType_MAJI(mpModel[i], &tevStr);
                mDoMtx_stack_c::copy(model->getAnmMtx(jointNo[i]));
                cMtx_copy(mDoMtx_stack_c::get(), mtx);
                mpModel[i]->setBaseTRMtx(mtx);
                if (i == 0 && field_0x166b != 0) {
                    mpModel[i]->getModelData()->getMaterialNodePointer(0)->getShape()->hide();
                    mDoExt_modelUpdateDL(mpModel[i]);
                    mpModel[i]->getModelData()->getMaterialNodePointer(0)->getShape()->show();
                } else {
                    mDoExt_modelUpdateDL(mpModel[i]);
                }
                dComIfGd_addRealShadow(mShadowKey, mpModel[i]);
            }
        }
    }
}

bool daNpc_Moi_c::afterSetMotionAnm(int param_1, int param_2, f32 param_3, int param_4) {
    switch (param_1) {
    case 0x22:
        mpMorf[0]->setPlaySpeed(2.0f);
    }
    return true;
}

void daNpc_Moi_c::changeAnm(int* param_1, int* param_2) {
    if (mType == TYPE_1) {
        switch (*param_1) {
        case 19:
            *param_1 = 6;
            *param_2 = 5;
            break;
        case 20:
            *param_1 = 7;
            *param_2 = 5;
            break;
        case 21:
            *param_1 = 8;
            *param_2 = 5;
        }
    }
}

void daNpc_Moi_c::changeBck(int* param_1, int* param_2) {
    if (mType == TYPE_1) {
        switch (*param_1) {
        case 8:
            *param_1 = 4;
            *param_2 = 5;
            break;
        case 13:
            *param_1 = 5;
            *param_2 = 5;
        }
    }
}

void daNpc_Moi_c::changeBtp(int* param_1, int* param_2) {
    if (mType == TYPE_1) {
        switch (*param_1) {
        case 40:
            *param_1 = 11;
            *param_2 = 5;
            break;
        case 45:
            *param_1 = 12;
            *param_2 = 5;
        }
    } else if (chkMoiN() && *param_1 == 19 && *param_2 == 1) {
        *param_1 = 45;
        *param_2 = 3;
    }
}

void daNpc_Moi_c::changeBtk(int* param_1, int* param_2) {
    if (mType != TYPE_1 && chkMoiN() && *param_1 == 16 && *param_2 == 3) {
        *param_1 = 36;
        *param_2 = 3;
    }
}

int daNpc_Moi_c::selectAction() {
    field_0x1610 = NULL;

    switch (mType) {
    case TYPE_2:
        if (daNpcT_chkEvtBit(0xcd)) {
            field_0x1610 = &daNpc_Moi_c::walkOnEggshell;
        } else {
            field_0x1610 = &daNpc_Moi_c::wait;
        }
        break;
    case TYPE_3:
        field_0x1610 = &daNpc_Moi_c::rest;
        break;
    case TYPE_4:
        field_0x1610 = &daNpc_Moi_c::walk;
        break;
    default:
        field_0x1610 = &daNpc_Moi_c::wait;
    }

    return 1;
}

BOOL daNpc_Moi_c::chkAction(actionFunc param_1) {
    return field_0x161c == param_1;
}

int daNpc_Moi_c::setAction(int (daNpc_Moi_c::*param_0)(void*)) {
    mMode = MODE_EXIT;
    if (field_0x161c != NULL) {
        (this->*field_0x161c)(NULL);
    }

    mMode = MODE_ENTER;
    field_0x161c = param_0;
    if (field_0x161c != NULL) {
        (this->*field_0x161c)(NULL);
    }

    return 1;
}

BOOL daNpc_Moi_c::chkPlayerGetWoodShield() {
    return (u8)(dComIfGs_isItemFirstBit(fpcNm_ITEM_WOOD_SHIELD) ? 1 : 0);
}

void daNpc_Moi_c::setSSlash(int param_1) {
    mMotionSeqMngr.setNo(MOT_UNK_43, 0.0f, 1, 0);
    /* dSv_event_tmp_flag_c::GUARD_URI - Ordon Village - Rusl is guarding Uli, Ordon village night
     */
    if (dComIfGs_isTmpBit(0x1308)) {
        mSound.startCreatureVoice(JAISoundID(Z2SE_MOI_V_GUARD_SWING), -1);
    } else {
        mSound.startCreatureVoice(JAISoundID(Z2SE_MOI_V_WOUND_SWING), -1);
    }
    field_0x1664 = fopAcM_searchPlayerAngleY(this);
    s16 iVar3 = cM_deg2s(mpHIO->m.field_0xb8);
    daPy_getPlayerActorClass()->setThrowDamage(fopAcM_searchPlayerAngleY(this),
                                               mpHIO->m.field_0xb4 * cM_scos(iVar3),
                                               mpHIO->m.field_0xb4 * cM_ssin(iVar3), 1, 0, 0);
    if (param_1 != 0) {
        field_0x1660 = 1;
        field_0x165c = 202;
    }
    field_0x1658 = 0;
    field_0x166b = 1;
    field_0x166c = 1;
    /* dSv_event_tmp_flag_c::T_0047 - Ordon Village - Right after wrestling with Bo
                                      (Used to connect with conversation after wearing clothes) */
    daNpcT_onTmpBit(0x47);
}

BOOL daNpc_Moi_c::chkPullOutSw() {
    if (mType == TYPE_2) {
        J3DAnmTransform* anmTransform = getTrnsfrmAnmP(
            l_resNameList[l_motionAnmData[25].mBckArcIdx], l_motionAnmData[25].mBckFileIdx);
        return anmTransform == mpMorf[0]->getAnm();
    }
    return FALSE;
}

void daNpc_Moi_c::pullOutSw() {
    if (chkPullOutSw() && mpMorf[0]->checkFrame(4.0f)) {
        field_0x166b = 1;
    }
}

BOOL daNpc_Moi_c::chkSetInSw() {
    if (mType == TYPE_2) {
        J3DAnmTransform* anmTransform = getTrnsfrmAnmP(
            l_resNameList[l_motionAnmData[26].mBckArcIdx], l_motionAnmData[26].mBckFileIdx);
        return anmTransform == mpMorf[0]->getAnm();
    }
    return FALSE;
}

void daNpc_Moi_c::setInSw() {
    if (chkSetInSw() && mpMorf[0]->checkFrame(6.0f)) {
        field_0x166b = 0;
    }
}

void daNpc_Moi_c::setSwAT() {
    field_0x166a = 0;

    if (mType == TYPE_2) {
        J3DAnmTransform* anmTransform = getTrnsfrmAnmP(
            l_resNameList[l_motionAnmData[24].mBckArcIdx], l_motionAnmData[24].mBckFileIdx);
        if (anmTransform == mpMorf[0]->getAnm()) {
            if (5.0f <= mpMorf[0]->getFrame() && mpMorf[0]->getFrame() <= 7.0f) {
                field_0x166a = 1;
            }
            if (20.0f <= mpMorf[0]->getFrame() && mpMorf[0]->getFrame() <= 22.0f) {
                field_0x166a = 1;
            }
        }
    }
}

void daNpc_Moi_c::torch() {
    fopAc_ac_c* actor = NULL;
    static cXyz lightOffset(10.0f, 0.0f, 40.0f);

    if (mpModel[5] && field_0x1668 != 0) {
        actor = mActorMngr[0].getActorP();
        mDoMtx_stack_c::copy(mpMorf[0]->getModel()->getAnmMtx(12));
        mDoMtx_stack_c::multVec(&lightOffset, &field_0x1628);
        if (actor == NULL && field_0x166f == 0) {
            if (field_0x15c0 == 0xFFFFFFFF) {
                field_0x15c0 =
                    fopAcM_createChild(PROC_EP, fopAcM_GetID(this), -247, &field_0x1628,
                                       fopAcM_GetRoomNo(this), &mCurAngle, NULL, -1, NULL);
            }
            if (fopAcM_SearchByID(field_0x15c0, &actor) && actor != NULL) {
                mActorMngr[0].entry(actor);
                dKy_plight_cut(&((ep_class*)actor)->mLightInf);
            }
        }
    }
}

int daNpc_Moi_c::injuryCheck() {
    BOOL bVar1 = TRUE;
    int timer = field_0x1658;

    if (mMotionSeqMngr.getNo() == MOT_UNK_37 || mMotionSeqMngr.getNo() == MOT_UNK_38) {
        if (mMotionSeqMngr.checkEndSequence()) {
            cLib_calcTimer(&timer);
        } else {
            bVar1 = false;
        }
    }

    if (bVar1 && timer != 0) {
        if (field_0x1667 != 0) {
            if ((timer & 1) != 0) {
                mMotionSeqMngr.setNo(MOT_UNK_38, -1.0f, 0, 0);
            } else {
                mMotionSeqMngr.setNo(MOT_UNK_37, -1.0f, 0, 0);
            }
        } else {
            f32 fVar3 = cM_rnd();
            fVar3 -= 0.5f;
            if (mMotionSeqMngr.getNo() == MOT_UNK_38) {
                fVar3 += 0.25f;
            }
            if (mMotionSeqMngr.getNo() == MOT_UNK_37) {
                fVar3 -= 0.25f;
            }
            if (0.0f < fVar3) {
                mMotionSeqMngr.setNo(MOT_UNK_38, -1.0f, 0, 0);
            } else {
                mMotionSeqMngr.setNo(MOT_UNK_37, -1.0f, 0, 0);
            }
        }
    }

    return timer;
}

int daNpc_Moi_c::injuryTurn(cXyz param_1) {
    BOOL bVar1 = FALSE;

    switch (field_0x1654) {
    case 0: {
        s16 iVar1 = cLib_targetAngleY(&current.pos, &param_1) - mCurAngle.y;
        if (abs(iVar1) < 0x2000) {
            bVar1 = TRUE;
        } else {
            mMotionSeqMngr.setNo(MOT_UNK_40, 4.0f, 1, 0);
            field_0x1664 = mCurAngle.y + 0x4000;
            mTurnAmount = 0;
            mStartAngle = current.angle.y;
            current.angle.y = mCurAngle.y;
            shape_angle.y = current.angle.y;
            field_0x1654 = 1;
        }
        break;
    }
    case 1:
        if (turn(field_0x1664, 40, 1)) {
            current.angle.y = mCurAngle.y;
            shape_angle.y = current.angle.y;
            field_0xd7e.y = mCurAngle.y;
            field_0x1654 = 0;
        } else {
            current.angle.y = mCurAngle.y;
            shape_angle.y = current.angle.y;
        }
        break;
    default:
        bVar1 = TRUE;
    }

    return bVar1;
}

int daNpc_Moi_c::injuryWalk() {
    /* dSv_event_tmp_flag_c::GUARD_URI - Ordon Village - Rusl is guarding Uli, Ordon village night
     */
    dComIfGs_offTmpBit(0x1308);

    cXyz cStack_40;

    int iVar2 = mPath.getDstPosH(current.pos, &cStack_40, mPath.getNumPnts(), 4);
    int iVar4 = iVar2 != 0 || field_0x164c == 0;

    if (field_0x1667) {
        if (injuryTurn(cStack_40)) {
            field_0x1667 = 0;
            mMode = MODE_INIT;
        }
    } else {
        calcSpeedAndAngle(cStack_40, iVar4, 6, 0x800);

        if (iVar4 != 0 && cM3d_IsZero(speedF) && (field_0x1658 = 2, iVar2 != 0)) {
            field_0x1667 = 1;
            field_0x1654 = 0;

            if (mPath.chkReverse()) {
                mPath.setIdx(0);
            } else {
                mPath.setIdx(mPath.getNumPnts() - 1);
            }
            mPath.reverse();
            mPath.setNextIdx(mPath.getNumPnts());
        }

        if ((int)mPath.getIdx() > 1 && (int)mPath.getIdx() < mPath.getNumPnts() - 2) {
            cLib_calcTimer(&field_0x164c);
        }
    }

    return 1;
}

int daNpc_Moi_c::poise() {
    int iVar13 = daNpc_Moi_Param_c::m.field_0x98;

    if (field_0x1669 != 0) {
        if (field_0x166c != 0) {
            if (field_0x1664 == mCurAngle.y) {
                if (mMotionSeqMngr.getNo() == MOT_UNK_43) {
                    if (mMotionSeqMngr.checkEndSequence()) {
                        mMotionSeqMngr.setNo(MOT_UNK_10, -1.0f, 0, 0);
                        field_0x166c = 0;
                    }
                } else if (mMotionSeqMngr.getNo() == MOT_UNK_10) {
                    mMotionSeqMngr.setNo(MOT_UNK_10, -1.0f, 0, 0);
                    field_0x166c = 0;
                }
            } else {
                step(field_0x1664, -1, -1, 10, 0);
            }
        } else {
            if (mMotionSeqMngr.getNo() == MOT_UNK_43) {
                if (mMotionSeqMngr.checkEndSequence()) {
                    mMotionSeqMngr.setNo(MOT_UNK_10, -1.0f, 0, 0);
                    field_0x166c = 0;
                }
            } else {
                if (daPy_getPlayerActorClass()->current.pos.absXZ(current.pos) < 150.0f &&
                    !daPy_getPlayerActorClass()->checkPlayerFly())
                {
                    setSSlash(0);
                } else if (field_0x166e) {
                    if (step(field_0x1664, -1, -1, 10, 0)) {
                        mMotionSeqMngr.setNo(MOT_UNK_10, -1.0f, 0, 0);
                        field_0x166e = 0;
                    }
                } else if ((s16)abs((s16)(fopAcM_searchPlayerAngleY(this) - mCurAngle.y)) <
                           cM_deg2s(60.0))
                {
                    cXyz cStack_74 = getAttnPos(daPy_getPlayerActorClass());
                    int speak_dist = attention_info.distances[fopAc_attn_SPEAK_e];
                    f32 fVar2 = dComIfGp_getAttention()->getDistTable(speak_dist).mDistMax;
                    f32 fVar3 = dComIfGp_getAttention()->getDistTable(speak_dist).mLowerY * -1.0f;
                    f32 fVar4 = dComIfGp_getAttention()->getDistTable(speak_dist).mUpperY * -1.0f;
                    if (chkPointInArea(cStack_74, attention_info.position, fVar2, fVar3, fVar4, 0))
                    {
                        if (field_0x1648 == 0) {
                            field_0x1648 = cLib_getRndValue(iVar13 * 0.5f, iVar13 * 1.5f);
                        } else if (!cLib_calcTimer(&field_0x1648)) {
                            mMotionSeqMngr.setNo(MOT_UNK_43, 0.0f, 1, 0);
                            /* dSv_event_tmp_flag_c::GUARD_URI - Ordon Village - Rusl is guarding
                             * Uli, Ordon village night */
                            if (dComIfGs_isTmpBit(0x1308)) {
                                mSound.startCreatureVoice(JAISoundID(Z2SE_MOI_V_GUARD_SWING), -1);
                            } else {
                                mSound.startCreatureVoice(JAISoundID(Z2SE_MOI_V_WOUND_SWING), -1);
                            }
                        }
                    } else {
                        field_0x1648 = 0;
                    }
                } else {
                    field_0x1664 = fopAcM_searchPlayerAngleY(this);
                    if (field_0x166b != 0) {
                        mMotionSeqMngr.setNo(MOT_UNK_10, -1.0f, 0, 0);
                    } else {
                        mMotionSeqMngr.setNo(MOT_UNK_11, -1.0f, 0, 0);
                    }
                    field_0x1648 = 0;
                    field_0x166e = 1;
                }
            }
        }
    } else {
        if (field_0x166c == 0) {
            field_0x165c = 202;
        }
        speedF = 0.0f;
        speed.setall(0.0f);
        field_0x166e = 0;
        field_0x1669 = 1;
    }

    if (field_0x1660 == 0) {
        if (field_0x165c != 0) {
            attention_info.flags = fopAc_AttnFlag_TALK_e;
            mSpeakEvent = true;
        }
    } else {
        field_0x1660 -= 1;
    }

    return 1;
}

int daNpc_Moi_c::cutDeliveredSw(int param_1) {
    int rv = 0;
    int iVar6 = -1;
    int uVar5 = 0;
    BOOL bVar1 = FALSE;

    int* piVar1 = dComIfGp_evmng_getMyIntegerP(param_1, "prm");
    if (piVar1) {
        iVar6 = *piVar1;
    }
    piVar1 = dComIfGp_evmng_getMyIntegerP(param_1, "timer");
    if (piVar1) {
        uVar5 = *piVar1;
    }

    if (dComIfGp_getEventManager().getIsAddvance(param_1)) {
        switch (iVar6) {
        case 0:
            mFaceMotionSeqMngr.setNo(MOT_UNK_21, 0.0f, 0, 0);
            mMotionSeqMngr.setNo(45, 0.0f, 1, 0);
            mEventTimer = uVar5;
            break;
        case 1:
            mFaceMotionSeqMngr.setNo(MOT_UNK_21, -1.0f, 0, 0);
            mMotionSeqMngr.setNo(45, -1.0f, 0, 0);
            mEventTimer = uVar5;
            break;
        case 2:
            mFaceMotionSeqMngr.setNo(MOT_UNK_21, -1.0f, 0, 0);
            mMotionSeqMngr.setNo(MOT_UNK_0, -1.0f, 0, 0);
            mEventTimer = uVar5;
            break;
        case 3:
            initTalk(mFlowNodeNo, NULL);
            break;
        case 4:
            mFaceMotionSeqMngr.setNo(MOT_UNK_21, -1.0f, 0, 0);
            mMotionSeqMngr.setNo(MOT_UNK_0, -1.0f, 0, 0);
        }
    }

    switch (iVar6) {
    case 0:
    case 1: {
        mJntAnm.lookNone(0);
        if (cLib_calcTimer(&mEventTimer) == 0) {
            if (iVar6 == 1) {
                speedF = 0.0f;
                speed.setall(0.0f);
                rv = 1;
            }
            bVar1 = TRUE;
        }

        cXyz cStack_3c(0.0f, 0.0f, 500.0f);
        mDoMtx_stack_c::YrotS(mCurAngle.y);
        mDoMtx_stack_c::multVec(&cStack_3c, &cStack_3c);
        cStack_3c = current.pos + cStack_3c;

        if (2500.0f < current.pos.abs2(cStack_3c) && !bVar1) {
            cLib_chaseF(&speedF, 4.0f, 0.55f);
        } else {
            speedF = 0.0f;
        }

        if (bVar1 && cM3d_IsZero(speedF)) {
            speedF = 0.0f;
            speed.setall(0.0f);
            rv = 1;
        }
        break;
    }
    case 2:
        mJntAnm.lookPlayer(0);
        if (cLib_calcTimer(&mEventTimer) == 0) {
            rv = 1;
        }
        break;
    case 3:
        if (talkProc(NULL, 0, NULL, 0) && mFlow.checkEndFlow()) {
            rv = 1;
        }
        break;
    case 4:
        rv = 1;
    }

    return rv;
}

int daNpc_Moi_c::cutAppearanceMoi(int param_1) {
    s16 sVar5;
    cXyz cStack_30;
    int rv = 0;
    int iVar8 = -1;
    int local_78 = 0;
    int iVar7 = 0;

    int* piVar1 = dComIfGp_evmng_getMyIntegerP(param_1, "prm");
    if (piVar1) {
        iVar8 = *piVar1;
    }
    piVar1 = dComIfGp_evmng_getMyIntegerP(param_1, "timer");
    if (piVar1) {
        local_78 = *piVar1;
    }
    piVar1 = dComIfGp_evmng_getMyIntegerP(param_1, "msgNo");
    if (piVar1) {
        iVar7 = *piVar1;
    }

    fopAc_ac_c* local_68[2] = {mActorMngr[3].getActorP(), (fopAc_ac_c*)this};
    dComIfGp_setMesgCameraInfoActor(local_68[0], local_68[1], 0, 0, 0, 0, 0, 0, 0, 0);

    if (dComIfGp_getEventManager().getIsAddvance(param_1)) {
        switch (iVar8) {
        case 0:
            break;
        case 1:
            initTalk(120, &local_68[0]);
            home.angle.y = mPlayerAngle;
            setAngle(home.angle.y);
            break;
        case 2:
            mFaceMotionSeqMngr.setNo(MOT_UNK_21, -1.0f, 0, 0);
            mMotionSeqMngr.setNo(MOT_UNK_45, -1.0f, 0, 0);
            dComIfGp_getEvent()->setPt2(this);
            mEventTimer = local_78;
            break;
        case 3:
            cStack_30.set(0.0f, 0.0f, 200.0f);
            mDoMtx_stack_c::YrotS(home.angle.y);
            mDoMtx_stack_c::multVec(&cStack_30, &cStack_30);
            cStack_30 = daPy_getPlayerActorClass()->current.pos - cStack_30;
            setPos(cStack_30);
            break;
        case 4:
            mFaceMotionSeqMngr.setNo(MOT_UNK_21, -1.0f, 0, 0);
            mMotionSeqMngr.setNo(MOT_UNK_0, -1.0f, 0, 0);
            initTalk(120, &local_68[0]);
            break;
        case 6:
            daNpcT_onEvtBit(0xb8);
        }
    }

    int local_70[2] = {-1, -1};

    switch (iVar8) {
    case 0:
        rv = 1;
        break;
    case 1:
        mJntAnm.lookPlayer(0);
        if (talkProc(NULL, 0, &local_68[0], 0) && mFlow.checkEndFlow()) {
            rv = 1;
        }
        break;
    case 2:
    case 3:
        mJntAnm.lookPlayer(0);
        cStack_30.set(0.0f, 0.0f, 100.0f);
        mDoMtx_stack_c::YrotS(home.angle.y);
        mDoMtx_stack_c::multVec(&cStack_30, &cStack_30);
        cStack_30 = daPy_getPlayerActorClass()->current.pos - cStack_30;
        calcSpeedAndAngle(cStack_30, current.pos.absXZ(cStack_30) < 50.0f, 4, 0x800);
        if (iVar8 == 2) {
            if (mEventTimer != 0 && !cLib_calcTimer(&mEventTimer)) {
                cStack_30 = daPy_getPlayerActorClass()->current.pos;
                sVar5 = fopAcM_searchActorAngleY(daPy_getPlayerActorClass(), this);
                daPy_getPlayerActorClass()->setPlayerPosAndAngle(&cStack_30, sVar5, 0);
            }
            if (mEventTimer == 0) {
                rv = 1;
            }
        } else if (current.pos.absXZ(cStack_30) < 50.0f && cM3d_IsZero(speedF)) {
            rv = 1;
        }
        break;
    case 4:
    case 5:
        mJntAnm.lookPlayer(0);
        local_70[0] = iVar7;
        if (talkProc(local_70, 0, &local_68[0], 0)) {
            if (!iVar7) {
                if (mFlow.checkEndFlow()) {
                    rv = 1;
                }
            } else {
                rv = 1;
            }
        }
        break;
    case 6:
        dStage_changeScene(1, 0.0f, 0, fopAcM_GetRoomNo(this), 0, -1);
    }

    return rv;
}

int daNpc_Moi_c::cutConversationWithUri(int param_1) {
    cXyz cStack_34;
    int rv = 0;
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
            cStack_34 = mPath.getPntPos(0);
            setPos(cStack_34);
            mPath.setIdx(0);
            mPath.setNextIdx(mPath.getNumPnts());
            cStack_34 = mPath.getPntPos(mPath.getIdx());
            setAngle(cLib_targetAngleY(&current.pos, &cStack_34));
            break;
        case 2:
            break;
        }
    }

    switch (iVar6) {
    case 0:
        mJntAnm.lookNone(0);
        rv = 1;
        break;
    case 1:
        rv = 1;
        break;
    case 2:
        daNpc_Uri_c* uri_p = (daNpc_Uri_c*)mActorMngr[2].getActorP();
        if (uri_p) {
            mJntAnm.lookActor(uri_p, 0.0f, 0);
        }
        rv = 1;
    }

    return rv;
}

int daNpc_Moi_c::cutFindWolf(int param_1) {
    cXyz cStack_30;
    int rv = 0;
    int iVar9 = -1;
    int uVar8 = 0;

    int* piVar1 = dComIfGp_evmng_getMyIntegerP(param_1, "prm");
    if (piVar1) {
        iVar9 = *piVar1;
    }

    piVar1 = dComIfGp_evmng_getMyIntegerP(param_1, "timer");
    if (piVar1) {
        uVar8 = *piVar1;
    }

    if (dComIfGp_getEventManager().getIsAddvance(param_1)) {
        switch (iVar9) {
        case 0:
            mMotionSeqMngr.setNo(MOT_UNK_10, 0.0f, 0, 0);
            speedF = 0.0f;
            speed.setall(0.0f);
            break;
        case 1:
            mFaceMotionSeqMngr.setNo(MOT_UNK_8, 0.0f, 0, 0);
            mMotionSeqMngr.setNo(MOT_UNK_16, 0.0f, 0, 0);
            mSound.startCreatureVoice(JAISoundID(Z2SE_MOI_V_KUTT), -1);
            field_0x166b = 1;
            cStack_30 = mPath.getPntPos(0);
            setPos(cStack_30);
            mPath.offReverse();
            mPath.setIdx(0);
            mPath.setNextIdx(mPath.getNumPnts());
            cStack_30 = mPath.getPntPos(mPath.getIdx());
            field_0x1664 = fopAcM_searchPlayerAngleY(this);
            setAngle(field_0x1664);
            mPlayerActorMngr.entry(daPy_getPlayerActorClass());
            mEventTimer = uVar8;
            break;
        case 2:
            mEventTimer = uVar8;
            break;
        }
    }

    switch (iVar9) {
    case 0:
        mJntAnm.lookNone(0);
        rv = 1;
        break;
    case 1:
        mJntAnm.lookPlayer(0);
        if (!cLib_calcTimer(&mEventTimer)) {
            field_0x1660 = 1;
            poise();
            field_0x165c = 0;
            rv = 1;
            break;
        }
        break;
    case 2:
        mJntAnm.lookPlayer(0);
        field_0x1660 = 1;
        poise();
        field_0x165c = 0;
        if (!cLib_calcTimer(&mEventTimer)) {
            rv = 1;
        }
    }

    return rv;
}

int daNpc_Moi_c::wait(void* param_1) {
    int iVar4 = mpHIO->m.field_0x98;

    switch (mMode) {
    case MODE_ENTER:
    case MODE_INIT:
        if (!mStagger.checkStagger()) {
            mFaceMotionSeqMngr.setNo(MOT_UNK_21, -1.0f, 0, 0);
            mMotionSeqMngr.setNo(MOT_UNK_0, -1.0f, 0, 0);
            field_0x1648 = cLib_getRndValue(iVar4 * 0.5f, iVar4 * 1.5f);
            mMode = MODE_RUN;
        }
    case MODE_RUN:
        if (mType == 5 && daNpcT_chkEvtBit(0xb8) && mHide) {
            fopAcM_delete(this);
            return 1;
        } else if (!mStagger.checkStagger()) {
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
                if (home.angle.y != mCurAngle.y) {
                    if (field_0xe34) {
                        if (step(home.angle.y, 21, 44, 15, 0)) {
                            mMode = MODE_INIT;
                        }
                    } else {
                        setAngle(home.angle.y);
                        mMode = MODE_INIT;
                    }
                    attention_info.flags = 0;
                } else {
                    if (mType == 1) {
                        if (field_0x1648) {
                            if (!cLib_calcTimer(&field_0x1648)) {
                                mFaceMotionSeqMngr.setNo(MOT_UNK_21, -1.0f, 0, 0);
                                mMotionSeqMngr.setNo(MOT_UNK_47, -1.0f, 0, 0);
                            }
                        } else {
                            if (mMotionSeqMngr.checkEndSequence() || srchPlayerActor()) {
                                mMode = MODE_INIT;
                                break;
                            }
                            if (mpMorf[0]->checkFrame(13.0f) || mpMorf[0]->checkFrame(29.0f)) {
                                mSound.startCreatureVoice(JAISoundID(Z2SE_MOI_V_SWING_S), -1);
                            } else if (mpMorf[0]->checkFrame(35.0f)) {
                                mSound.startCreatureVoice(JAISoundID(Z2SE_MOI_V_SWING_L), -1);
                            }
                        }
                    }
                    if (!mTwilight) {
                        srchPlayerActor();
                    }
                }
            }
            mJntAnm.getMode();
        }

        break;
    case MODE_EXIT:
        break;
    }

    return 1;
}

int daNpc_Moi_c::walk(void* param_1) {
    cXyz cStack_1c;

    switch (mMode) {
    case MODE_ENTER:
    case MODE_INIT:
        if (!mStagger.checkStagger()) {
            mFaceMotionSeqMngr.setNo(MOT_UNK_21, -1.0f, 0, 0);
            mMotionSeqMngr.setNo(MOT_UNK_45, -1.0f, 0, 0);
            mMode = MODE_RUN;
        }
    case MODE_RUN:
        if (mType == TYPE_4 && getBitSW() != 0xFF) {
            if (dComIfGs_isSwitch(getBitSW(), fopAcM_GetRoomNo(this))) {
                fopAcM_delete(this);
                return 1;
            }
            mEvtNo = 3;
            field_0xe33 = 1;
        }

        if (!mStagger.checkStagger()) {
            mJntAnm.lookNone(0);
            cStack_1c.set(0.0f, 0.0f, 500.0f);
            mDoMtx_stack_c::YrotS(mCurAngle.y);
            mDoMtx_stack_c::multVec(&cStack_1c, &cStack_1c);
            cStack_1c = current.pos + cStack_1c;
            if (2500.0f < current.pos.abs2(cStack_1c)) {
                cLib_chaseF(&speedF, 4.0f, 0.55f);
            }
            mJntAnm.getMode();
        }
        break;
    case MODE_EXIT:
        break;
    }

    return 1;
}

int daNpc_Moi_c::walkOnEggshell(void* param_1) {
    int sVar1 = mpHIO->m.field_0x9a;

    switch (mMode) {
    case MODE_ENTER:
    case MODE_INIT:
        if (!mStagger.checkStagger()) {
            if (field_0x1669) {
                mFaceMotionSeqMngr.setNo(MOT_UNK_21, -1.0f, 0, 0);
                Motion motion_no;
                if (field_0x166b) {
                    motion_no = MOT_UNK_10;
                } else {
                    motion_no = MOT_UNK_11;
                }
                mMotionSeqMngr.setNo(motion_no, -1.0f, 0, 0);
            } else {
                mFaceMotionSeqMngr.setNo(MOT_UNK_21, -1.0f, 0, 0);
                mMotionSeqMngr.setNo(MOT_UNK_40, -1.0f, 0, 0);
                mJntAnm.lookNone(0);
                field_0x164c = cLib_getRndValue(sVar1 * 0.5f, sVar1 * 1.5f);
            }
            mMode = MODE_RUN;
        }
    case MODE_RUN:
        if (!mStagger.checkStagger()) {
            if (mPlayerActorMngr.getActorP()) {
                mJntAnm.lookPlayer(0);
                if (field_0x1658 != 0) {
                    if (chkActorInSight(mPlayerActorMngr.getActorP(), mAttnFovY, mCurAngle.y)) {
                        field_0x1658 = 0;
                    } else {
                        mJntAnm.lookNone(0);
                    }
                }
            }

            if (field_0x166c) {
                mJntAnm.lookPlayer(0);
                poise();
            } else {
                if (field_0x1658) {
                    field_0x1658 = injuryCheck();
                    if (field_0x1658) {
                        if (daPy_getPlayerActorClass()->eventInfo.chkCondition(dEvtCnd_CANTALK_e) !=
                            0)
                        {
                            f32 fVar = daPy_getPlayerActorClass()->attention_info.position.absXZ(
                                attention_info.position);
                        }
                    } else {
                        if (!field_0x1667) {
                            mMode = MODE_INIT;
                            mPlayerActorMngr.remove();
                        }
                    }
                } else {
                    if (mPlayerActorMngr.getActorP()) {
                        mJntAnm.lookPlayer(0);
                        poise();
                    } else {
                        if (field_0x1669) {
                            mMotionSeqMngr.setNo(MOT_UNK_39, -1.0f, 0, 0);
                            field_0x1669 = 0;
                            field_0x165c = 0;
                        } else {
                            if (mMotionSeqMngr.getNo() == MOT_UNK_39) {
                                if (mMotionSeqMngr.checkEndSequence()) {
                                    mMode = MODE_INIT;
                                }
                            } else {
                                injuryWalk();
                            }
                        }
                    }
                }
            }
            srchPlayerActor();
        }

        break;
    case MODE_EXIT:
        break;
    }

    return 1;
}

int daNpc_Moi_c::rest(void* param_1) {
    switch (mMode) {
    case MODE_ENTER:
    case MODE_INIT:
        if (!mStagger.checkStagger()) {
            mFaceMotionSeqMngr.setNo(MOT_UNK_9, -1.0f, 0, 0);
            mMotionSeqMngr.setNo(MOT_UNK_19, -1.0f, 0, 0);
            mMode = MODE_RUN;
        }
    case MODE_RUN:
        if (!mStagger.checkStagger()) {
            if (!dComIfGp_event_runCheck()) {
                mSound.startCreatureVoiceLevel(JAISoundID(Z2SE_MOI_V_SLEEP_UNASARE), -1);
            }
            mJntAnm.lookNone(0);
        }
        break;
    case MODE_EXIT:
        break;
    }

    return 1;
}

int daNpc_Moi_c::talk(void* param_1) {
    switch (mMode) {
    case MODE_ENTER:
    case MODE_INIT:
        if (!mStagger.checkStagger()) {
            if (field_0x165c) {
                if (field_0x1669) {
                    mFaceMotionSeqMngr.setNo(MOT_UNK_21, -1.0f, 0, 0);
                    Motion motion_no;
                    if (field_0x166b) {
                        motion_no = MOT_UNK_10;
                    } else {
                        motion_no = MOT_UNK_11;
                    }
                    mMotionSeqMngr.setNo(motion_no, -1.0f, 0, 0);
                }
                initTalk(field_0x165c, NULL);
            } else {
                initTalk(mFlowNodeNo, NULL);
            }
            mMode = MODE_RUN;
        }
    case MODE_RUN:
        if (!mStagger.checkStagger()) {
            if (mTwilight || mPlayerAngle == mCurAngle.y || mType == TYPE_3) {
                if (talkProc(NULL, 0, NULL, 0) && mFlow.checkEndFlow()) {
                    if (field_0x1669) {
                        field_0x166c = 0;
                        field_0x165c = 0;
                    } else {
                        mPlayerActorMngr.entry(daPy_getPlayerActorClass());
                    }
                    dComIfGp_event_reset();
                    mMode = MODE_EXIT;
                }
                mJntAnm.lookPlayer(0);
                if (mTwilight) {
                    mJntAnm.lookNone(0);
                }
            } else {
                mJntAnm.lookPlayer(0);
                if (field_0x1669) {
                    step(mPlayerAngle, -1, -1, 15, 0);
                } else {
                    step(mPlayerAngle, 21, 44, 15, 0);
                }
            }
        }
        break;
    case MODE_EXIT:
        break;
    }

    return 0;
}

static int daNpc_Moi_Create(void* i_this) {
    return static_cast<daNpc_Moi_c*>(i_this)->create();
}

static int daNpc_Moi_Delete(void* i_this) {
    return static_cast<daNpc_Moi_c*>(i_this)->Delete();
}

static int daNpc_Moi_Execute(void* i_this) {
    return static_cast<daNpc_Moi_c*>(i_this)->Execute();
}

static int daNpc_Moi_Draw(void* i_this) {
    return static_cast<daNpc_Moi_c*>(i_this)->Draw();
}

static int daNpc_Moi_IsDelete(void* i_this) {
    return 1;
}

static actor_method_class daNpc_Moi_MethodTable = {
    (process_method_func)daNpc_Moi_Create,  (process_method_func)daNpc_Moi_Delete,
    (process_method_func)daNpc_Moi_Execute, (process_method_func)daNpc_Moi_IsDelete,
    (process_method_func)daNpc_Moi_Draw,
};

actor_process_profile_definition g_profile_NPC_MOI = {
    fpcLy_CURRENT_e,         // mLayerID
    7,                       // mListID
    fpcPi_CURRENT_e,         // mListPrio
    PROC_NPC_MOI,            // mProcName
    &g_fpcLf_Method.base,    // sub_method
    sizeof(daNpc_Moi_c),     // mSize
    0,                       // mSizeOther
    0,                       // mParameters
    &g_fopAc_Method.base,    // sub_method
    355,                     // mPriority
    &daNpc_Moi_MethodTable,  // sub_method
    0x00040107,              // mStatus
    fopAc_NPC_e,             // mActorType
    fopAc_CULLBOX_CUSTOM_e,  // cullType
};

AUDIO_INSTANCES
