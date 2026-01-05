/**
 * @file d_a_npc_lud.cpp
 *
 */

#include "d/dolzel_rel.h"  // IWYU pragma: keep

#include "d/actor/d_a_npc_len.h"
#include "d/actor/d_a_npc_lud.h"

const daNpc_Lud_HIOParam daNpc_Lud_Param_c::m = {
    160.0f,  // attention_offset
    -3.0f,   // gravity
    1.0f,    // scale
    400.0f,  // real_shadow_size
    255.0f,  // weight
    150.0f,  // height
    35.0f,   // knee_length
    30.0f,   // width
    0.0f,    // body_angleX_max
    0.0f,    // body_angleX_min
    30.0f,   // body_angleY_max
    -30.0f,  // body_angleY_min
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
    -15.0f,  // box_min_x
    0.0f,    // box_min_y
    -15.0f,  // box_min_z
    15.0f,   // box_max_x
    40.0f,   // box_max_y
    15.0f,   // box_max_z
    90.0f,   // box_offset
    1.0f,    // play_speed
};

#if DEBUG
daNpc_Lud_HIO_c::daNpc_Lud_HIO_c() {
    m = daNpc_Lud_Param_c::m;
}

void daNpc_Lud_HIO_c::listenPropertyEvent(const JORPropertyEvent* event) {
    // TODO
}

void daNpc_Lud_HIO_c::genMessage(JORMContext* ctext) {
    // TODO
}
#endif

enum Type {
    /* 0x0 */ TYPE_0,
    /* 0x1 */ TYPE_1,
    /* 0x2 */ TYPE_2,
    /* 0x3 */ TYPE_3,
    /* 0x4 */ TYPE_4,
    /* 0x5 */ TYPE_5,
    /* 0x6 */ TYPE_6,
    /* 0x7 */ TYPE_7,
    /* 0x8 */ TYPE_8,
};

enum FaceMotion {
    /* 0x01 */ FACE_MOT_UNK_1 = 1,
    /* 0x02 */ FACE_MOT_UNK_2 = 2,
    /* 0x03 */ FACE_MOT_UNK_3 = 3,
    /* 0x04 */ FACE_MOT_UNK_4 = 4,
    /* 0x05 */ FACE_MOT_UNK_5 = 5,
    /* 0x06 */ FACE_MOT_UNK_6 = 6,
    /* 0x07 */ FACE_MOT_UNK_7 = 7,
    /* 0x08 */ FACE_MOT_UNK_8 = 8,
    /* 0x09 */ FACE_MOT_UNK_9 = 9,
    /* 0x0A */ FACE_MOT_UNK_10 = 10,
    /* 0x0B */ FACE_MOT_UNK_11 = 11,
    /* 0x0C */ FACE_MOT_UNK_12 = 12,
    /* 0x0D */ FACE_MOT_UNK_13 = 13,
    /* 0x0E */ FACE_MOT_UNK_14 = 14,
    /* 0x0F */ FACE_MOT_UNK_15 = 15,
    /* 0x10 */ FACE_MOT_UNK_16 = 16,
    /* 0x11 */ FACE_MOT_UNK_17 = 17,
    /* 0x12 */ FACE_MOT_UNK_18 = 18,
};

enum Motion {
    /* 0x00 */ MOT_UNK_0 = 0,
    /* 0x01 */ MOT_UNK_1 = 1,
    /* 0x02 */ MOT_UNK_2 = 2,
    /* 0x03 */ MOT_UNK_3 = 3,
    /* 0x04 */ MOT_UNK_4 = 4,
    /* 0x06 */ MOT_UNK_6 = 6,
    /* 0x07 */ MOT_UNK_7 = 7,
    /* 0x08 */ MOT_UNK_8 = 8,
    /* 0x09 */ MOT_UNK_9 = 9,
    /* 0x0A */ MOT_UNK_10 = 10,
    /* 0x0B */ MOT_UNK_11 = 11,
    /* 0x0C */ MOT_UNK_12 = 12,
    /* 0x0D */ MOT_UNK_13 = 13,
    /* 0x0E */ MOT_UNK_14 = 14,
    /* 0x0F */ MOT_UNK_15 = 15,
    /* 0x10 */ MOT_UNK_16 = 16,
    /* 0x11 */ MOT_UNK_17 = 17,
    /* 0x12 */ MOT_UNK_18 = 18,
    /* 0x13 */ MOT_UNK_19 = 19,
};

static int l_bmdData[5][2] = {
    {11, 1}, {11, 2}, {39, 4}, {40, 4}, {38, 4},
};

static daNpcT_evtData_c l_evtList[6] = {
    {"", 0},      {"NO_RESPONSE", 0}, {"DEMO13_STB", 0}, {"CONVERSATION_ABOUT_GORON", 4},
    {"NURSE", 4}, {"GO_AHEAD", 4},
};

static char* l_resNameList[6] = {
    "", "Lud", "Lud_TW", "Lud1", "Lud2", "Len1",
};

static s8 l_loadResPtrn0[3] = {
    1,
    3,
    -1,
};

static s8 l_loadResPtrn1[3] = {
    1,
    2,
    -1,
};

static s8 l_loadResPtrn2[3] = {
    1,
    4,
    -1,
};

s8 l_loadResPtrn9[5] = {
    1, 2, 3, 4, -1,
};

static s8* l_loadResPtrnList[9] = {
    l_loadResPtrn0, l_loadResPtrn1, l_loadResPtrn0, l_loadResPtrn0, l_loadResPtrn2,
    l_loadResPtrn2, l_loadResPtrn2, l_loadResPtrn0, l_loadResPtrn9,
};

static daNpcT_faceMotionAnmData_c l_faceMotionAnmData[20] = {
    {-1, 0, 0, 17, 2, 1, 1}, {6, 0, 1, 17, 2, 1, 1},  {18, 2, 4, 51, 2, 4, 0},
    {15, 0, 4, 48, 0, 4, 0}, {16, 2, 4, 49, 2, 4, 0}, {17, 0, 4, 50, 0, 4, 0},
    {21, 0, 4, 54, 0, 4, 0}, {22, 0, 4, 55, 0, 4, 0}, {24, 2, 4, 57, 2, 4, 0},
    {23, 0, 4, 56, 0, 4, 0}, {13, 0, 4, 46, 0, 4, 0}, {10, 0, 4, 43, 0, 4, 0},
    {11, 2, 4, 44, 2, 4, 0}, {12, 0, 4, 45, 0, 4, 0}, {14, 2, 4, 47, 2, 4, 0},
    {19, 2, 4, 52, 2, 4, 0}, {20, 2, 4, 53, 2, 4, 0}, {4, 2, 3, 10, 2, 3, 0},
    {8, 2, 2, 15, 2, 2, 0},  {7, 2, 2, 14, 2, 2, 0},
};

static daNpcT_motionAnmData_c l_motionAnmData[21] = {
    {8, 2, 1, 14, 0, 1, 1, 0},  {6, 0, 3, 14, 0, 1, 1, 0},  {7, 0, 3, 14, 0, 1, 1, 0},
    {7, 0, 1, 14, 0, 1, 1, 0},  {30, 2, 4, 14, 0, 1, 1, 0}, {33, 0, 4, 14, 0, 1, 1, 0},
    {34, 2, 4, 14, 0, 1, 1, 0}, {35, 0, 4, 14, 0, 1, 1, 0}, {25, 0, 4, 14, 0, 1, 0, 0},
    {27, 0, 4, 14, 0, 1, 0, 0}, {28, 2, 4, 14, 0, 1, 0, 0}, {29, 0, 4, 14, 0, 1, 0, 0},
    {6, 0, 4, 14, 0, 1, 0, 0},  {7, 2, 4, 14, 0, 1, 0, 0},  {8, 0, 4, 14, 0, 1, 0, 0},
    {26, 2, 4, 14, 0, 1, 0, 0}, {31, 2, 4, 14, 0, 1, 0, 0}, {32, 2, 4, 14, 0, 1, 0, 0},
    {5, 2, 3, 14, 0, 1, 0, 0},  {6, 2, 2, -1, 0, 0, 0, 0},  {5, 2, 2, -1, 0, 0, 0, 0},
};

static daNpcT_MotionSeqMngr_c::sequenceStepData_c l_faceMotionSequenceData[76] = {
    {1, -1, 1},  {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},  {7, -1, 1},  {8, -1, 0},  {-1, 0, 0},
    {-1, 0, 0},  {15, -1, 0}, {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},  {2, -1, 0},  {-1, 0, 0},
    {-1, 0, 0},  {-1, 0, 0},  {3, 0, 1},   {5, 0, 1},   {-1, 0, 0},  {-1, 0, 0},  {3, 0, 1},
    {4, 0, 0},   {-1, 0, 0},  {-1, 0, 0},  {5, 0, 1},   {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},
    {6, -1, 1},  {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},  {9, -1, 1},  {-1, 0, 0},  {-1, 0, 0},
    {-1, 0, 0},  {17, -1, 0}, {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},  {8, -1, 0},  {-1, 0, 0},
    {-1, 0, 0},  {-1, 0, 0},  {10, -1, 1}, {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},  {11, -1, 1},
    {12, 0, 0},  {-1, 0, 0},  {-1, 0, 0},  {13, -1, 1}, {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},
    {16, -1, 0}, {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},  {18, -1, 0}, {-1, 0, 0},  {-1, 0, 0},
    {-1, 0, 0},  {19, -1, 0}, {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},  {14, -1, 0}, {-1, 0, 0},
    {-1, 0, 0},  {-1, 0, 0},  {0, -1, 0},  {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},
};

static daNpcT_MotionSeqMngr_c::sequenceStepData_c l_motionSequenceData[80] = {
    {0, -1, 0},  {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},  {1, -1, 1},  {0, -1, 0},  {-1, 0, 0},
    {-1, 0, 0},  {4, -1, 0},  {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},  {6, -1, 0},  {-1, 0, 0},
    {-1, 0, 0},  {-1, 0, 0},  {16, -1, 0}, {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},  {2, -1, 1},
    {0, -1, 0},  {-1, 0, 0},  {-1, 0, 0},  {9, 0, 1},   {11, 0, 1},  {-1, 0, 0},  {-1, 0, 0},
    {9, 0, 1},   {10, 0, 0},  {-1, 0, 0},  {-1, 0, 0},  {11, 0, 1},  {-1, 0, 0},  {-1, 0, 0},
    {-1, 0, 0},  {5, -1, 1},  {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},  {7, -1, 1},  {-1, 0, 0},
    {-1, 0, 0},  {-1, 0, 0},  {18, -1, 0}, {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},  {8, -1, 1},
    {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},  {12, -1, 1}, {13, -1, 0}, {-1, 0, 0},  {-1, 0, 0},
    {14, -1, 1}, {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},  {17, -1, 0}, {-1, 0, 0},  {-1, 0, 0},
    {-1, 0, 0},  {19, -1, 0}, {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},  {20, -1, 0}, {-1, 0, 0},
    {-1, 0, 0},  {-1, 0, 0},  {3, 3, 1},   {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},  {15, -1, 0},
    {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},
};

char* daNpc_Lud_c::mCutNameList[8] = {
    "",
    "HURRY",
    "CONVERSATION_IN_HOTEL",
    "CONVERSATION_ABOUT_DEATHMT",
    "CONVERSATION_ABOUT_GORON",
    "NURSE",
    "CLOTH_TRY",
    "THANK_YOU",
};

daNpc_Lud_c::cutFunc daNpc_Lud_c::mCutList[8] = {
    NULL,
    &daNpc_Lud_c::cutHurry,
    &daNpc_Lud_c::cutConversationInHotel,
    &daNpc_Lud_c::cutConversationAboutDeathMt,
    &daNpc_Lud_c::cutConversationAboutGoron,
    &daNpc_Lud_c::cutNurse,
    &daNpc_Lud_c::cutClothTry,
    &daNpc_Lud_c::cutThankYou,
};

daNpc_Lud_c::~daNpc_Lud_c() {
    if (mpMorf[0] != NULL) {
        mpMorf[0]->stopZelAnime();
    }

    if (mpBowlMorf != NULL) {
        mpBowlMorf->stopZelAnime();
    }

    deleteRes(l_loadResPtrnList[mType], (const char**)l_resNameList);
}

int daNpc_Lud_c::create() {
    static int const heapSize[9] = {
        0x3EF0, 0x3EF0, 0x3EF0, 0x3EF0, 0x46D0, 0x5000, 0x46E0, 0x3EF0, 0x0,
    };

    daNpcT_ct(this, daNpc_Lud_c, (daNpcT_faceMotionAnmData_c const*)l_faceMotionAnmData,
                       (const daNpcT_motionAnmData_c*)l_motionAnmData,
                       (const daNpcT_MotionSeqMngr_c::sequenceStepData_c*)l_faceMotionSequenceData,
                       4, (const daNpcT_MotionSeqMngr_c::sequenceStepData_c*)l_motionSequenceData,
                       4, (const daNpcT_evtData_c*)l_evtList, (char**)l_resNameList);

    mType = getType();
    mFlowNodeNo = getFlowNodeNo();
    mTwilight = dKy_darkworld_check();

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
        reset();
        mAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1, &mAcchCir,
                  fopAcM_GetSpeed_p(this), fopAcM_GetAngle_p(this), fopAcM_GetShapeAngle_p(this));
        mCcStts.Init(mpHIO->m.common.weight, 0, this);
        mCyl.Set(mCcDCyl);
        mCyl.SetStts(&mCcStts);
        mCyl.SetTgHitCallback(tgHitCallBack);
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

int daNpc_Lud_c::CreateHeap() {
    u32 bmd_idx = mTwilight == 1 ? 1 : 0;
    int arc_idx = l_bmdData[bmd_idx][1];
    int res_idx = l_bmdData[bmd_idx][0];

    J3DModelData* modelData =
        static_cast<J3DModelData*>(dComIfG_getObjectRes(l_resNameList[arc_idx], res_idx));
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

    if (mType == TYPE_5) {
        J3DModelData* modelData = NULL;
        bmd_idx = 4;
        arc_idx = l_bmdData[bmd_idx][1];
        res_idx = l_bmdData[bmd_idx][0];
        modelData = (J3DModelData*)dComIfG_getObjectRes(l_resNameList[arc_idx], res_idx);

        mpBowlMorf = new mDoExt_McaMorfSO(modelData, NULL, NULL, NULL, -1, 1.0f, 0, -1, NULL,
                                          0x80000, 0x11000084);
        if (mpBowlMorf == NULL || mpBowlMorf->getModel() == NULL) {
            return 0;
        }
    }

    if (mType == TYPE_4 || mType == TYPE_6) {
        modelData = l_bmdData[2][0] >= 0 ? (J3DModelData*)dComIfG_getObjectRes(
                                               l_resNameList[l_bmdData[2][1]], l_bmdData[2][0]) :
                                           NULL;
        if (modelData != NULL) {
            mpModel[0] = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
        }

        if (mpModel[0] == NULL) {
            return 0;
        }
    }

    if (mType == TYPE_5) {
        modelData = l_bmdData[3][0] >= 0 ? (J3DModelData*)dComIfG_getObjectRes(
                                               l_resNameList[l_bmdData[3][1]], l_bmdData[3][0]) :
                                           NULL;

        if (modelData != NULL) {
            mpModel[1] = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
        }

        if (mpModel[1] == NULL) {
            return 0;
        }
    }

    if (mTwilight) {
        if (setFaceMotionAnm(18, false) && setMotionAnm(19, 0.0f, 0)) {
            return 1;
        }
        return 0;
    }

    if (setFaceMotionAnm(1, false) && setMotionAnm(0, 0, 0)) {
        return 1;
    }
    return 0;
}

int daNpc_Lud_c::Delete() {
    fopAcM_GetID(this);
    this->~daNpc_Lud_c();
    return 1;
}

int daNpc_Lud_c::Execute() {
    return daNpcT_c::execute();
}

int daNpc_Lud_c::Draw() {
    if (mpMatAnm[0] != NULL) {
        J3DModelData* modelData = mpMorf[0]->getModel()->getModelData();
        modelData->getMaterialNodePointer(getEyeballMaterialNo())->setMaterialAnm(mpMatAnm[0]);
    }
    return daNpcT_c::draw(0, 0, mRealShadowSize, NULL, 100.0f, 0, 0, 0);
}

int daNpc_Lud_c::createHeapCallBack(fopAc_ac_c* i_this) {
    return static_cast<daNpc_Lud_c*>(i_this)->CreateHeap();
}

int daNpc_Lud_c::ctrlJointCallBack(J3DJoint* param_0, int param_1) {
    if (param_1 == 0) {
        J3DModel* model = j3dSys.getModel();

        daNpc_Lud_c* i_this = reinterpret_cast<daNpc_Lud_c*>(model->getUserArea());
        if (i_this != 0) {
            i_this->ctrlJoint(param_0, model);
        }
    }

    return 1;
}

u8 daNpc_Lud_c::getType() {
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
    case 5:
        return TYPE_5;
    case 6:
        return TYPE_6;
    case 7:
        return TYPE_7;
    }
    return TYPE_8;
}

BOOL daNpc_Lud_c::isDelete() {
    switch (mType) {
    case TYPE_0:
        return !daNpcT_chkEvtBit(0x3d) || daNpcT_chkEvtBit(0xa4);
    case TYPE_1:
        return false;
    case TYPE_2:
        return !daNpcT_chkEvtBit(0xa4) || daNpcT_chkEvtBit(0x35);
    case TYPE_3:
        return !daNpcT_chkEvtBit(0x35) || daNpcT_chkEvtBit(0x55);
    case TYPE_4:
        return !daNpcT_chkEvtBit(0x55) || daNpcT_chkEvtBit(0x40);
    case TYPE_5:
        return !daNpcT_chkEvtBit(0x40) || daNpcT_chkEvtBit(0x44);
    case TYPE_6:
        return !daNpcT_chkEvtBit(0x44) || daNpcT_chkEvtBit(0x108);
    case TYPE_7:
        return daNpcT_chkEvtBit(0x108) == 0;
    }
    return false;
}

void daNpc_Lud_c::reset() {
    csXyz acStack_20;

    int iVar1 = (u8*)&field_0xfd8 - (u8*)&field_0xfa8;
    if (mpMatAnm[0] != NULL) {
        mpMatAnm[0]->initialize();
    }
    initialize();
    for (int i = 0; i < 3; i++) {
        mActorMngr[i].initialize();
    }
    memset(&field_0xfa8, 0, iVar1);
    acStack_20.setall(0.0f);
    acStack_20.y = home.angle.y;

    switch (mType) {
    case TYPE_0:
    case TYPE_1:
    case TYPE_2:
    case TYPE_3:
    case TYPE_7:
        break;
    case TYPE_4:
        field_0xfd2 = 1;
        break;
    case TYPE_5:
        field_0xfd3 = 1;
        field_0xfd4 = 1;
        break;
    case TYPE_6:
        field_0xfd2 = 1;
        break;
    }

    setAngle(acStack_20);
}

void daNpc_Lud_c::afterJntAnm(int param_1) {
    if (param_1 == 1) {
        mDoMtx_stack_c::YrotM(-mStagger.getAngleZ(1));
        mDoMtx_stack_c::ZrotM(mStagger.getAngleX(1));
    } else if (param_1 == 4) {
        mDoMtx_stack_c::YrotM(-mStagger.getAngleZ(0));
        mDoMtx_stack_c::ZrotM(mStagger.getAngleX(0));
    }
}

void daNpc_Lud_c::setParam() {
    selectAction();
    srchActors();

    s16 talk_distance = mpHIO->m.common.talk_distance;
    s16 talk_angle = mpHIO->m.common.talk_angle;
    s16 attention_distance = mpHIO->m.common.attention_distance;
    s16 attention_angle = mpHIO->m.common.attention_angle;

    switch (mType) {
    case TYPE_0:
        talk_angle = 3;
        attention_angle = 3;
        break;
    case TYPE_2:
        talk_angle = 3;
        attention_angle = 3;
        break;
    case TYPE_3:
        talk_distance = 13;
        attention_distance = 15;
        break;
    case TYPE_4:
    case TYPE_5:
    case TYPE_6:
        talk_distance = 1;
        attention_distance = 1;
        break;
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
    if (mTwilight) {
        mCylH = 120.0f;
    }
    mAttnFovY = mpHIO->m.common.fov;
    mAcchCir.SetWallR(mWallR);
    mAcchCir.SetWallH(mpHIO->m.common.knee_length);
    mRealShadowSize = mpHIO->m.common.real_shadow_size;
    if (mType == TYPE_4 || mType == TYPE_5 || mType == TYPE_6) {
        mRealShadowSize = 500.0f;
    }
    mExpressionMorfFrame = mpHIO->m.common.expression_morf_frame;
    mMorfFrames = mpHIO->m.common.morf_frame;
    gravity = mpHIO->m.common.gravity;
}

BOOL daNpc_Lud_c::checkChangeEvt() {
    if (!chkAction(&daNpc_Lud_c::talk)) {
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
        case TYPE_1:
        case TYPE_2:
        case TYPE_3:
        case TYPE_7:
            break;
        case TYPE_4:
            mEvtNo = 3;
            evtChange();
            return TRUE;
        case TYPE_5:
            mEvtNo = 4;
            evtChange();
            return TRUE;
        case TYPE_6:
            mEvtNo = 5;
            evtChange();
            return TRUE;
        }
    }

    return FALSE;
}

void daNpc_Lud_c::setAfterTalkMotion() {
    int iVar2 = FACE_MOT_UNK_18;
    switch (mFaceMotionSeqMngr.getNo()) {
    case FACE_MOT_UNK_1:
        iVar2 = FACE_MOT_UNK_10;
        break;
    }
    mFaceMotionSeqMngr.setNo(iVar2, -1.0f, 0, 0);
}

void daNpc_Lud_c::srchActors() {
    switch (mType) {
    case TYPE_1:
        if (mActorMngr[2].getActorP() == NULL) {
            mActorMngr[2].entry(getNearestActorP(PROC_NPC_LEN));
        }
        break;
    case TYPE_5:
        if (mActorMngr[0].getActorP() == NULL) {
            mActorMngr[0].entry(getNearestActorP(PROC_NPC_BESU));
        }
        if (mActorMngr[1].getActorP() == NULL) {
            mActorMngr[1].entry(getNearestActorP(PROC_NPC_KOLINB));
        }
        break;
    }
}

BOOL daNpc_Lud_c::evtTalk() {
    if (chkAction(&daNpc_Lud_c::talk)) {
        (this->*mAction)(NULL);
    } else {
        setAction(&daNpc_Lud_c::talk);
    }
    return TRUE;
}

BOOL daNpc_Lud_c::evtCutProc() {
    int staffId = dComIfGp_getEventManager().getMyStaffId("Lud", this, -1);
    if (staffId != -1) {
        mStaffId = staffId;
        int actIdx =
            dComIfGp_getEventManager().getMyActIdx(mStaffId, (char**)mCutNameList, 8, 0, 0);
        if ((this->*(mCutList[actIdx]))(mStaffId) != 0) {
            dComIfGp_getEventManager().cutEnd(mStaffId);
        }
        return true;
    }
    return false;
}

void daNpc_Lud_c::action() {
    fopAc_ac_c* hitActor = NULL;
    if (!mTwilight) {
        hitActor = hitChk(&mCyl, 0xffffffff);
    }

    if (hitActor) {
        if (mType != TYPE_4 && mType != TYPE_5 && mType != TYPE_6) {
            cCcD_ObjHitInf* hit_obj = mCyl.GetTgHitObj();
            if (hit_obj->ChkAtType(AT_TYPE_THROW_OBJ)) {
                mStagger.setParam(this, hitActor, mCurAngle.y);
                setDamage(0, 18, 0);
                mDamageTimerStart = 0;
                mJntAnm.lookNone(1);
            }
        }
    }

    if (mStagger.checkRebirth()) {
        mStagger.initialize();
        mMode = MODE_INIT;
    }

    if (field_0xfa8 != NULL) {
        if (mAction == field_0xfa8) {
            (this->*mAction)(NULL);
        } else {
            setAction(field_0xfa8);
        }
    }

    cLib_calcTimer(&field_0xfc8);
    cLib_calcTimer(&field_0xfcc);
}

void daNpc_Lud_c::beforeMove() {
    fopAcM_OffStatus(this, fopAcM_STATUS_UNK_0x8000000);
    if (checkHide()) {
        fopAcM_OnStatus(this, fopAcM_STATUS_UNK_0x8000000);
    }

    if (checkHide() || mNoDraw != 0) {
        attention_info.flags = 0;
    }
}

NPC_LUD_HIO_CLASS l_HIO;

void daNpc_Lud_c::setAttnPos() {
    cXyz cStack_70(5.0f, 30.0f, 0.0f);
    Mtx mtx;

    if (field_0xfd5 != 0) {
        daPy_getPlayerActorClass()->onWolfEyeKeep();
    }

    mStagger.calc(0);

    f32 dVar8 = cM_s2rad(mCurAngle.y - field_0xd7e.y);

    if (mType == TYPE_4 || mType == TYPE_5 || mType == TYPE_6) {
        mJntAnm.setParam(this, mpMorf[0]->getModel(), &cStack_70, getBackboneJointNo(),
                         getNeckJointNo(), getHeadJointNo(), mpHIO->m.common.body_angleX_min,
                         mpHIO->m.common.body_angleX_max, 0.0f, 0.0f,
                         mpHIO->m.common.head_angleX_min, mpHIO->m.common.head_angleX_max,
                         mpHIO->m.common.head_angleY_min, mpHIO->m.common.head_angleY_max,
                         mpHIO->m.common.neck_rotation_ratio, dVar8, NULL);
    } else {
        mJntAnm.setParam(this, mpMorf[0]->getModel(), &cStack_70, getBackboneJointNo(),
                         getNeckJointNo(), getHeadJointNo(), mpHIO->m.common.body_angleX_min,
                         mpHIO->m.common.body_angleX_max, mpHIO->m.common.body_angleY_min,
                         mpHIO->m.common.body_angleY_max, mpHIO->m.common.head_angleX_min,
                         mpHIO->m.common.head_angleX_max, mpHIO->m.common.head_angleY_min,
                         mpHIO->m.common.head_angleY_max, mpHIO->m.common.neck_rotation_ratio,
                         dVar8, NULL);
    }

    mJntAnm.calcJntRad(0.2f, 1.0f, dVar8);
    mpMorf[0]->setPlaySpeed(mpHIO->m.play_speed);
    setMtx();

    if (mpBowlMorf != NULL) {
        mpBowlMorf->play(0, 0);
        mDoMtx_stack_c::copy(mpMorf[0]->getModel()->getAnmMtx(14));
        cMtx_copy(mDoMtx_stack_c::get(), mtx);
        mpBowlMorf->getModel()->setBaseTRMtx(mtx);
        mpBowlMorf->modelCalc();
    }

    mDoMtx_stack_c::copy(mpMorf[0]->getModel()->getAnmMtx(getHeadJointNo()));
    mDoMtx_stack_c::multVec(&cStack_70, &eyePos);

    if (mType == TYPE_4 || mType == TYPE_5 || mType == TYPE_6) {
        s16 angle = field_0xfd0;
        if (angle != 0) {
            mJntAnm.setEyeAngleY(angle, 1.0f);
        } else {
            mJntAnm.setEyeAngleX(eyePos, 1.0f, 0);
            mJntAnm.setEyeAngleY(eyePos, mCurAngle.y, 1, 1.0f, 0);
        }
    } else {
        mJntAnm.setEyeAngleX(eyePos, 1.0f, 0);
        mJntAnm.setEyeAngleY(eyePos, mCurAngle.y, 1, 1.0f, 0);
    }

    if (mType == TYPE_4 || mType == TYPE_5 || mType == TYPE_6) {
        cStack_70.set(94.0f, 160.0f, -79.0f);
        mDoMtx_stack_c::copy(mpMorf[0]->getModel()->getBaseTRMtx());
        mDoMtx_stack_c::multVec(&cStack_70, &attention_info.position);
    } else {
        cStack_70.set(0.0f, 0.0f, 0.0f);
        cStack_70.y = mpHIO->m.common.attention_offset;
        mDoMtx_stack_c::YrotS(mCurAngle.y);
        mDoMtx_stack_c::multVec(&cStack_70, &cStack_70);
        attention_info.position = current.pos + cStack_70;
    }

    if (mType == TYPE_3) {
        mSound.startCreatureVoiceLevel(JAISoundID(Z2SE_LUD_V_HOUSHIN), -1);
    }
}

void daNpc_Lud_c::setCollision() {
    cXyz cStack_48;

    if (mHide == 0) {
        u32 tgType = 0xd8fbfdff;
        u32 tgSPrm = 0x1f;

        if (mTwilight != 0) {
            tgType = 0;
            tgSPrm = 0;
        } else if (mStagger.checkStagger()) {
            tgType = 0;
            tgSPrm = 0;
        }

        mCyl.SetCoSPrm(0x79);
        mCyl.SetTgType(tgType);
        mCyl.SetTgSPrm(tgSPrm);
        mCyl.OnTgNoHitMark();

        cStack_48.set(0.0f, 0.0f, 0.0f);

        f32 cylHeight = mCylH;
        f32 cylRadius = mWallR;

        if (mType == TYPE_1) {
            cStack_48.set(133.06f, 0.0f, 2.64f);
        } else if (mType == TYPE_4 || mType == TYPE_5 || mType == TYPE_6) {
            cStack_48.set(94.0f, 0.0f, -89.0f);
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

int daNpc_Lud_c::drawDbgInfo() {
    return 0;
}

void daNpc_Lud_c::drawOtherMdl() {
    static int const jointNo[2] = {20, 20};

    J3DModel* model_p = mpMorf[0]->getModel();

    if (mpBowlMorf != NULL) {
        g_env_light.setLightTevColorType_MAJI(mpBowlMorf->getModel(), &tevStr);

        if (field_0xfd4 == 0) {
            mpBowlMorf->getModel()->getModelData()->hide();
        } else {
            mpBowlMorf->getModel()->getModelData()->show();
        }

        mpBowlMorf->entryDL();
        dComIfGd_addRealShadow(mShadowKey, mpBowlMorf->getModel());
    }

    for (int i = 0; i < 2; i++) {
        if (mpModel[i] != NULL && ((i == 0 && field_0xfd2 != 0) || (i == 1 && field_0xfd3 != 0))) {
            g_env_light.setLightTevColorType_MAJI(mpModel[i], &tevStr);

            mDoMtx_stack_c::copy(model_p->getAnmMtx(jointNo[i]));
            Mtx jointAnmMtx;
            cMtx_copy(mDoMtx_stack_c::get(), jointAnmMtx);
            mpModel[i]->setBaseTRMtx(jointAnmMtx);
            mDoExt_modelUpdateDL(mpModel[i]);
            dComIfGd_addRealShadow(mShadowKey, mpModel[i]);
        }
    }
}

bool daNpc_Lud_c::setBowlAnm(int i_index, int i_attr, f32 i_morf) {
    static struct {
        int resIdx;
        int arcIdx;
    } bowlAnmData[21] = {
        {-1, 0}, {-1, 0}, {-1, 0}, {-1, 0}, {-1, 0}, {-1, 0}, {-1, 0},
        {-1, 0}, {-1, 0}, {-1, 0}, {-1, 0}, {-1, 0}, {-1, 0}, {-1, 0},
        {-1, 0}, {-1, 0}, {9, 4},  {9, 4},  {-1, 0}, {-1, 0}, {-1, 0},
    };

    J3DAnmTransform* anmTransform = NULL;

    if (mpBowlMorf != NULL) {
        if (bowlAnmData[i_index].resIdx > 0) {
            anmTransform = getTrnsfrmKeyAnmP(l_resNameList[bowlAnmData[i_index].arcIdx],
                                             bowlAnmData[i_index].resIdx);
        }

        if (anmTransform != NULL && anmTransform != mpBowlMorf->getAnm()) {
            mpBowlMorf->setAnm(anmTransform, i_attr, i_morf, 1.0f, 0.0f, -1.0f);
        }
    }

    return true;
}

bool daNpc_Lud_c::afterSetMotionAnm(int param_0, int param_1, f32 param_2, int param_3) {
    if (mCreating == true) {
        param_2 = 0.0f;
    }
    return setBowlAnm(param_0, param_1, param_2);
}

int daNpc_Lud_c::selectAction() {
    field_0xfa8 = NULL;

    switch (mType) {
    case TYPE_4:
        field_0xfa8 = &daNpc_Lud_c::nurse;
        break;
    case TYPE_5:
        field_0xfa8 = &daNpc_Lud_c::giveSoup;
        break;
    case TYPE_6:
        field_0xfa8 = &daNpc_Lud_c::nurse;
        break;
    default:
        field_0xfa8 = &daNpc_Lud_c::wait;
    }

    return 1;
}

BOOL daNpc_Lud_c::chkAction(actionFunc param_0) {
    return mAction == param_0;
}

int daNpc_Lud_c::setAction(actionFunc param_0) {
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

void daNpc_Lud_c::mop(int param_0, int param_1) {
    if (field_0xfc8 != 0) {
        action();
        if (field_0xfc8 == 0) {
            mFaceMotionSeqMngr.setNo(FACE_MOT_UNK_3, 4.0f, 0, 0);
            mMotionSeqMngr.setNo(MOT_UNK_2, 4.0f, 0, 0);
            field_0xfcc = param_1;
        }
    } else if (field_0xfcc != 0) {
        action();
        if (field_0xfcc == 0) {
            mFaceMotionSeqMngr.setNo(FACE_MOT_UNK_4, 4.0f, 0, 0);
            mMotionSeqMngr.setNo(MOT_UNK_6, 4.0f, 0, 0);
            field_0xfc8 = param_0;
        }
    }
}

int daNpc_Lud_c::cutHurry(int param_0) {
    int rv = 0;
    int iVar3 = -1;

    int* piVar1 = dComIfGp_evmng_getMyIntegerP(param_0, "prm");
    if (piVar1) {
        iVar3 = *piVar1;
    }

    if (dComIfGp_getEventManager().getIsAddvance(param_0)) {
        switch (iVar3) {
        case 0:
            mFaceMotionSeqMngr.setNo(FACE_MOT_UNK_9, -1.0f, 0, 0);
            mMotionSeqMngr.setNo(MOT_UNK_11, -1.0f, 0, 0);
            mJntAnm.setMode(0, FALSE);
            mJntAnm.setDirect(1);
            break;
        }
    }

    switch (iVar3) {
    case 0:
        rv = 1;
        break;
    }

    return rv;
}

int daNpc_Lud_c::cutConversationInHotel(int param_0) {
    int rv = 0;
    int iVar7 = -1;
    int uVar4 = 0;
    int iVar6 = 0;
    int iVar5 = 0;

    int* piVar1 = dComIfGp_evmng_getMyIntegerP(param_0, "prm");
    if (piVar1) {
        iVar7 = *piVar1;
    }

    piVar1 = dComIfGp_evmng_getMyIntegerP(param_0, "timer");
    if (piVar1) {
        uVar4 = *piVar1;
    }

    piVar1 = dComIfGp_evmng_getMyIntegerP(param_0, "timer1");
    if (piVar1) {
        iVar6 = *piVar1;
    }

    piVar1 = dComIfGp_evmng_getMyIntegerP(param_0, "timer2");
    if (piVar1) {
        iVar5 = *piVar1;
    }

    if (dComIfGp_getEventManager().getIsAddvance(param_0)) {
        switch (iVar7) {
        case 0:
            field_0xfc8 = 0;
            field_0xfcc = 1;
            break;
        case 1:
            mFaceMotionSeqMngr.setNo(FACE_MOT_UNK_13, 0.0f, 1, 0);
            mMotionSeqMngr.setNo(MOT_UNK_14, 0.0f, 1, 0);
            break;
        case 2:
            mEventTimer = uVar4;
            break;
        case 3:
            mFaceMotionSeqMngr.setNo(FACE_MOT_UNK_3, 0.0f, 1, 0);
            mMotionSeqMngr.setNo(MOT_UNK_2, 0.0f, 1, 0);
            break;
        }
    }

    switch (iVar7) {
    case 0:
        mop(iVar6, iVar5);
        rv = 1;
        break;
    case 1:
        rv = 1;
        break;
    case 2:
        if (mEventTimer) {
            if (!cLib_calcTimer(&mEventTimer)) {
                mFaceMotionSeqMngr.setNo(FACE_MOT_UNK_12, 0.0f, 0, 0);
                mMotionSeqMngr.setNo(MOT_UNK_13, 0.0f, 0, 0);
                mSound.startCreatureVoice(JAISoundID(Z2SE_LUD_V_SHIKARARE), -1);
            } else {
                mop(iVar6, iVar5);
            }
        } else {
            rv = 1;
        }
        break;
    case 3:
        rv = 1;
        break;
    }

    return rv;
}

int daNpc_Lud_c::cutConversationAboutDeathMt(int param_0) {
    int rv = 0;
    int iVar3 = -1;

    int* piVar1 = dComIfGp_evmng_getMyIntegerP(param_0, "prm");
    if (piVar1) {
        iVar3 = *piVar1;
    }

    if (dComIfGp_getEventManager().getIsAddvance(param_0)) {
        switch (iVar3) {
        case 0:
            mFaceMotionSeqMngr.setNo(FACE_MOT_UNK_5, -1.0f, 0, 0);
            mMotionSeqMngr.setNo(MOT_UNK_7, -1.0f, 0, 0);
            break;
        case 1:
            mFaceMotionSeqMngr.setNo(FACE_MOT_UNK_6, -1.0f, 0, 0);
            mMotionSeqMngr.setNo(MOT_UNK_8, -1.0f, 0, 0);
            field_0xfc8 = 32;
            break;
        case 2:
            mFaceMotionSeqMngr.setNo(FACE_MOT_UNK_5, 0.0f, 1, 0);
            mMotionSeqMngr.setNo(MOT_UNK_7, 0.0f, 1, 0);
            break;
        case 3:
            mFaceMotionSeqMngr.setNo(FACE_MOT_UNK_6, -1.0f, 0, 0);
            mMotionSeqMngr.setNo(MOT_UNK_8, -1.0f, 0, 0);
            field_0xfc8 = 32;
            break;
        }
    }

    switch (iVar3) {
    case 0:
        action();
        rv = 1;
        break;
    case 1:
        action();
        if (!field_0xfc8) {
            rv = 1;
        }
        break;
    case 2:
        rv = 1;
        break;
    case 3:
        if (!cLib_calcTimer(&field_0xfc8)) {
            rv = 1;
        }
        break;
    }

    return rv;
}

int daNpc_Lud_c::cutConversationAboutGoron(int param_0) {
    int rv = 0;
    int iVar4 = -1;

    int* piVar1 = dComIfGp_evmng_getMyIntegerP(param_0, "prm");
    if (piVar1) {
        iVar4 = *piVar1;
    }

    if (dComIfGp_getEventManager().getIsAddvance(param_0)) {
        switch (iVar4) {
        case 0:
            mFaceMotionSeqMngr.setNo(FACE_MOT_UNK_1, -1.0f, 0, 0);
            mMotionSeqMngr.setNo(MOT_UNK_3, -1.0f, 0, 0);
            initTalk(mFlowNodeNo, NULL);
            break;
        case 1:
            mFaceMotionSeqMngr.setNo(FACE_MOT_UNK_7, -1.0f, 0, 0);
            mMotionSeqMngr.setNo(MOT_UNK_9, -1.0f, 0, 0);
            break;
        case 2:
            mFaceMotionSeqMngr.setNo(FACE_MOT_UNK_8, -1.0f, 0, 0);
            mMotionSeqMngr.setNo(MOT_UNK_10, -1.0f, 0, 0);
            break;
        }
    }

    switch (iVar4) {
    case 0:
        if (talkProc(NULL, 0, NULL, 0) && mFlow.checkEndFlow()) {
            rv = 1;
        }
        break;
    case 1:
    case 2:
        if (mMotionSeqMngr.checkEndSequence()) {
            if (iVar4 == 2) {
                field_0xfd0 = 0;
            }
            rv = 1;
            break;
        }
    }

    if (mType == TYPE_6) {
        if (iVar4 == 0 || iVar4 == 1) {
            cLib_chaseS(&field_0xfd0, 0xe00, 0x166);
        } else {
            cLib_chaseS(&field_0xfd0, 0, 0x166);
        }
    }

    return rv;
}

int daNpc_Lud_c::cutNurse(int param_0) {
    int rv = 0;
    int iVar3 = -1;

    int* piVar1 = dComIfGp_evmng_getMyIntegerP(param_0, "prm");
    if (piVar1) {
        iVar3 = *piVar1;
    }

    dComIfGp_evmng_getMyIntegerP(param_0, "msgNo");

    fopAc_ac_c* speakers[3] = {(fopAc_ac_c*)this, mActorMngr[0].getActorP(),
                               mActorMngr[1].getActorP()};
    dComIfGp_setMesgCameraInfoActor(speakers[0], speakers[1], speakers[2], 0, 0, 0, 0, 0, 0, 0);

    if (dComIfGp_getEventManager().getIsAddvance(param_0)) {
        switch (iVar3) {
        case 0:
            initTalk(mFlowNodeNo, &speakers[0]);
            break;
        case 1:
            mFaceMotionSeqMngr.setNo(FACE_MOT_UNK_14, -1.0f, 0, 0);
            mMotionSeqMngr.setNo(MOT_UNK_15, -1.0f, 0, 0);
            break;
        case 2:
            mFaceMotionSeqMngr.setNo(FACE_MOT_UNK_2, 0.0f, 1, 0);
            mMotionSeqMngr.setNo(MOT_UNK_4, 0.0f, 1, 0);
            break;
        }
    }

    int local_40[3] = {-1, -1, -1};

    switch (iVar3) {
    case 0:
        field_0xe26 = false;
        local_40[0] = 0x13c2;
        local_40[1] = 0x13c6;
        if (talkProc(local_40, 0, speakers, 0)) {
            rv = 1;
        }
        break;
    case 1:
        field_0xe26 = false;
        if (talkProc(NULL, 0, speakers, 0) && mFlow.checkEndFlow()) {
            rv = 1;
        }
        break;
    case 2:
        field_0xe26 = false;
        rv = 1;
        break;
    }

    return rv;
}

int daNpc_Lud_c::cutClothTry(int param_0) {
    int rv = 0;
    int iVar2 = -1;

    int* piVar1 = dComIfGp_evmng_getMyIntegerP(param_0, "prm");
    if (piVar1) {
        iVar2 = *piVar1;
    }

    dComIfGp_getEventManager().getIsAddvance(param_0);
    switch (iVar2) {
    case 0:
        action();
        rv = 1;
        break;
    }

    return rv;
}

int daNpc_Lud_c::cutThankYou(int param_0) {
    int rv = 0;
    int iVar3 = -1;

    int* piVar1 = dComIfGp_evmng_getMyIntegerP(param_0, "prm");
    if (piVar1) {
        iVar3 = *piVar1;
    }

    if (dComIfGp_getEventManager().getIsAddvance(param_0)) {
        switch (iVar3) {
        case 0:
            break;
        case 1:
            mFaceMotionSeqMngr.setNo(FACE_MOT_UNK_17, 0.0f, 0, 0);
            mMotionSeqMngr.setNo(MOT_UNK_19, 0.0f, 0, 0);
            break;
        }
    }

    switch (iVar3) {
    case 0:
        action();
        rv = 1;
        break;
    case 1:
        rv = 1;
        break;
    }

    return rv;
}

int daNpc_Lud_c::wait(void* param_0) {
    switch (mMode) {
    case 0:
    case 1:
        if (!mStagger.checkStagger()) {
            switch (mType) {
            case TYPE_1:
                if (getBitSW() != 0xFF && dComIfGs_isSwitch(getBitSW(), fopAcM_GetRoomNo(this))) {
                    mFaceMotionSeqMngr.setNo(FACE_MOT_UNK_16, -1.0f, 0, 0);
                    mMotionSeqMngr.setNo(MOT_UNK_17, -1.0f, 0, 0);
                    break;
                }
                mFaceMotionSeqMngr.setNo(FACE_MOT_UNK_15, -1.0f, 0, 0);
                mMotionSeqMngr.setNo(MOT_UNK_16, -1.0f, 0, 0);
                break;
            case TYPE_3:
                mFaceMotionSeqMngr.setNo(FACE_MOT_UNK_9, -1.0f, 0, 0);
                mMotionSeqMngr.setNo(MOT_UNK_11, -1.0f, 0, 0);
                break;
            case TYPE_2:
            default:
                mFaceMotionSeqMngr.setNo(FACE_MOT_UNK_18, -1.0f, 0, 0);
                mMotionSeqMngr.setNo(MOT_UNK_0, -1.0f, 0, 0);
                break;
            }
            field_0xfc0 = 0;
            field_0xfc4 = 0;
            mMode = 2;
        }
    case 2:
        switch (mType) {
        case TYPE_1:
            fopAc_ac_c* actor_p = (daNpc_Len_c*)mActorMngr[2].getActorP();
            if (actor_p != NULL &&
                ((daNpc_Len_c*)actor_p)
                    ->checkStartDemo13StbEvt(this, mpHIO->m.common.box_min_x,
                                             mpHIO->m.common.box_min_y, mpHIO->m.common.box_min_z,
                                             mpHIO->m.common.box_max_x, mpHIO->m.common.box_max_y,
                                             mpHIO->m.common.box_max_z, mpHIO->m.common.box_offset))
            {
                mEvtNo = 2;
                field_0xfd5 = 1;
            }

            if (field_0xfd5 != 0 && daNpcT_chkEvtBit(0x3C) && !dComIfGp_event_runCheck()) {
                field_0xfd5 = 0;
            }
            break;
        }

        if (!mStagger.checkStagger()) {
            if (mType == TYPE_1) {
                mJntAnm.lookNone(0);
                if (getBitSW() != 0xFF && dComIfGs_isSwitch(getBitSW(), fopAcM_GetRoomNo(this)) &&
                    mMotionSeqMngr.getNo() != MOT_UNK_17)
                {
                    mMode = 1;
                }
                attention_info.flags = 0;
            } else {
                if (mType == TYPE_3) {
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
                            if (step(home.angle.y, 18, 18, 15, 0)) {
                                mMode = 1;
                            }
                        } else {
                            setAngle(home.angle.y);
                            mMode = 1;
                        }
                        attention_info.flags = 0;
                    } else if (!mTwilight) {
                        srchPlayerActor();
                    }
                }
            }
            switch (mType) {
            case TYPE_3:
                attention_info.flags = 0;
                break;
            }
        }
    case 3:
        break;
    }

    return 1;
}

int daNpc_Lud_c::nurse(void* param_0) {
    switch (mMode) {
    case 0:
    case 1:
        if (mFaceMotionSeqMngr.getNo() != FACE_MOT_UNK_13) {
            mFaceMotionSeqMngr.setNo(FACE_MOT_UNK_3, -1.0f, 0, 0);
        }
        if (mMotionSeqMngr.getNo() != MOT_UNK_14) {
            mMotionSeqMngr.setNo(MOT_UNK_2, -1.0f, 0, 0);
        }
        mMorfLoops = 0;
        if (mType == TYPE_6) {
            field_0xfc8 = 0;
            field_0xfcc = 1;
        }
        mMode = 2;

    case 2:
        mJntAnm.lookNone(0);
        mAcch.SetWallNone();

        switch (mMotionSeqMngr.getNo()) {
        case MOT_UNK_2:
            if (strcmp(dComIfGp_getEventManager().getRunEventName(),
                       "EVT_CONVERSATION_IN_HOTEL1_e") &&
                strcmp(dComIfGp_getEventManager().getRunEventName(),
                       "EVT_CONVERSATION_IN_HOTEL2_e") &&
                mType != TYPE_6 && mMorfLoops >= 2)
            {
                mFaceMotionSeqMngr.setNo(FACE_MOT_UNK_11, -1.0f, 0, 0);
                mMotionSeqMngr.setNo(MOT_UNK_12, -1.0f, 0, 0);
            }
            break;
        case MOT_UNK_12:
        case MOT_UNK_14:
            if (mMotionSeqMngr.checkEndSequence()) {
                mFaceMotionSeqMngr.setNo(FACE_MOT_UNK_3, -1.0f, 0, 0);
                mMotionSeqMngr.setNo(MOT_UNK_2, -1.0f, 0, 0);
            }
        }
        if (mType == TYPE_6) {
            if (field_0xfc8 != 0) {
                cLib_calcTimer(&field_0xfc8);
                cLib_calcTimer(&field_0xfcc);
                if (field_0xfc8 == 0) {
                    mFaceMotionSeqMngr.setNo(FACE_MOT_UNK_3, -1.0f, 0, 0);
                    mMotionSeqMngr.setNo(MOT_UNK_2, -1.0f, 0, 0);
                    field_0xfcc = 92;
                }
            } else if (field_0xfcc != 0) {
                cLib_calcTimer(&field_0xfc8);
                cLib_calcTimer(&field_0xfcc);
                if (field_0xfcc == 0) {
                    mFaceMotionSeqMngr.setNo(FACE_MOT_UNK_4, -1.0f, 0, 0);
                    mMotionSeqMngr.setNo(MOT_UNK_6, -1.0f, 0, 0);
                    field_0xfc8 = 92;
                }
            }
        }
        break;
    case 3:
        break;
    }

    return 1;
}

int daNpc_Lud_c::giveSoup(void* param_0) {
    switch (mMode) {
    case 0:
    case 1:
        mFaceMotionSeqMngr.setNo(FACE_MOT_UNK_2, -1.0f, 0, 0);
        mMotionSeqMngr.setNo(MOT_UNK_4, -1.0f, 0, 0);
        mMode = 2;
    case 2:
        mJntAnm.lookNone(0);
        mAcch.SetWallNone();
        break;
    case 3:
        break;
    }
    return 1;
}

int daNpc_Lud_c::talk(void* param_0) {
    switch (mMode) {
    case 0:
    case 1:
        if (!mStagger.checkStagger()) {
            initTalk(mFlowNodeNo, NULL);
            mMode = 2;
        }
    case 2:
        if (!mStagger.checkStagger()) {
            if (mTwilight || mPlayerAngle == mCurAngle.y || mType == 0 || mType == 2 ||
                mType == 4 || mType == 5 || mType == 6)
            {
                if (talkProc(NULL, 0, NULL, 0) && mFlow.checkEndFlow()) {
                    mPlayerActorMngr.entry(daPy_getPlayerActorClass());
                    dComIfGp_event_reset();
                    mMode = 3;
                }
                mJntAnm.lookPlayer(0);
                if (mTwilight || mType == 4 || mType == 5 || mType == 6) {
                    mJntAnm.lookNone(0);
                }
            } else {
                mJntAnm.lookPlayer(0);
                step(mPlayerAngle, 18, 18, 15, 0);
            }
        }
        break;
    case 3:
        break;
    }

    return 0;
}

static int daNpc_Lud_Create(void* i_this) {
    return static_cast<daNpc_Lud_c*>(i_this)->create();
}

static int daNpc_Lud_Delete(void* i_this) {
    return static_cast<daNpc_Lud_c*>(i_this)->Delete();
}

static int daNpc_Lud_Execute(void* i_this) {
    return static_cast<daNpc_Lud_c*>(i_this)->Execute();
}

static int daNpc_Lud_Draw(void* i_this) {
    return static_cast<daNpc_Lud_c*>(i_this)->Draw();
}

static int daNpc_Lud_IsDelete(void* i_this) {
    return 1;
}

static actor_method_class daNpc_Lud_MethodTable = {
    (process_method_func)daNpc_Lud_Create,  (process_method_func)daNpc_Lud_Delete,
    (process_method_func)daNpc_Lud_Execute, (process_method_func)daNpc_Lud_IsDelete,
    (process_method_func)daNpc_Lud_Draw,
};

actor_process_profile_definition g_profile_NPC_LUD = {
    fpcLy_CURRENT_e,         // mLayerID
    7,                       // mListID
    fpcPi_CURRENT_e,         // mListPrio
    PROC_NPC_LUD,            // mProcName
    &g_fpcLf_Method.base,    // sub_method
    sizeof(daNpc_Lud_c),     // mSize
    0,                       // mSizeOther
    0,                       // mParameters
    &g_fopAc_Method.base,    // sub_method
    338,                     // mPriority
    &daNpc_Lud_MethodTable,  // sub_method
    0x00040108,              // mStatus
    fopAc_NPC_e,             // mActorType
    fopAc_CULLBOX_CUSTOM_e,  // cullType
};
