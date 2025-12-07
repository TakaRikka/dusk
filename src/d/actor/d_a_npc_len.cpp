/**
 * @file d_a_npc_len.cpp
 *
 */

#include "d/dolzel_rel.h"  // IWYU pragma: keep

#include "d/actor/d_a_npc_len.h"
#include "d/actor/d_a_tag_push.h"

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
    /* 0x9 */ TYPE_9,
};

enum FaceMotion {
    /* 0x09 */ FACE_MOT_UNK_9 = 9,
    /* 0x0C */ FACE_MOT_UNK_12 = 12,
    /* 0x0E */ FACE_MOT_UNK_14 = 14,
    /* 0x0F */ FACE_MOT_UNK_15 = 15,
    /* 0x11 */ FACE_MOT_UNK_17 = 17,
};

enum Motion {
    /* 0x00 */ MOT_UNK_0 = 0,
    /* 0x06 */ MOT_UNK_6 = 6,
    /* 0x07 */ MOT_UNK_7 = 7,
    /* 0x08 */ MOT_UNK_8 = 8,
    /* 0x09 */ MOT_UNK_9 = 9,
    /* 0x0A */ MOT_UNK_10 = 10,
    /* 0x0B */ MOT_UNK_11 = 11,
    /* 0x0C */ MOT_UNK_12 = 12,
    /* 0x0D */ MOT_UNK_13 = 13,
    /* 0x10 */ MOT_UNK_16 = 16,
    /* 0x11 */ MOT_UNK_17 = 17,
};

const daNpc_Len_HIOParam daNpc_Len_Param_c::m = {
    230.0f, -3.0f,  1.0f,   450.0f, 255.0f, 210.0f, 35.0f, 40.0f,  0.0f,  0.0f, 15.0f,
    -15.0f, 30.0f,  -10.0f, 30.0f,  -30.0f, 0.6f,   12.0f, 3,      6,     5,    6,
    110.0f, 0.0f,   0.0f,   0.0f,   60,     8,      0,     0,      0,     0,    0,
    4.0f,   -20.0f, 0.0f,   -20.0f, 20.0f,  50.0f,  20.0f, 100.0f, 0x168, 0xB4, 3.0f,
};

#if DEBUG
daNpc_Len_HIO_c::daNpc_Len_HIO_c() {
    m = daNpc_Len_Param_c::m;
}

void daNpc_Len_HIO_c::listenPropertyEvent(const JORPropertyEvent* event) {
    // TODO
}

void daNpc_Len_HIO_c::genMessage(JORMContext* ctext) {
    // TODO
}
#endif

static int l_bmdData[2][2] = {
    {11, 1},
    {13, 2},
};

static daNpcT_evtData_c l_evtList[7] = {
    {"", 0},
    {"DEFAULT_GETITEM", 0},
    {"NO_RESPONSE", 0},
    {"DEMO13_STB", 0},
    {"HURRY", 3},
    {"CONVERSATION_IN_HOTEL1", 3},
    {"CONVERSATION_IN_HOTEL2", 3},
};

static char* l_resNameList[4] = {
    "",
    "Len",
    "Len_TW",
    "Len1",
};

static s8 l_loadResPtrn0[3] = {
    1,
    3,
    -1,
};

static s8 l_loadResPtrn1[4] = {1, 2, 3, -1};

static s8* l_loadResPtrnList[10] = {
    l_loadResPtrn0, l_loadResPtrn1, l_loadResPtrn0, l_loadResPtrn0, l_loadResPtrn0,
    l_loadResPtrn0, l_loadResPtrn0, l_loadResPtrn0, l_loadResPtrn0, l_loadResPtrn1,
};

static daNpcT_faceMotionAnmData_c l_faceMotionAnmData[21] = {
    {-1, 0, 0, 17, 2, 1, 1}, {6, 0, 1, 17, 2, 1, 1},  {16, 0, 3, 46, 0, 3, 0},
    {24, 2, 3, 49, 2, 3, 0}, {18, 0, 3, 47, 0, 3, 0}, {25, 2, 3, 50, 2, 3, 0},
    {17, 2, 3, 17, 2, 1, 1}, {19, 2, 3, 17, 2, 1, 1}, {14, 2, 3, 17, 2, 1, 1},
    {26, 2, 3, 17, 2, 1, 1}, {20, 0, 3, 17, 2, 1, 1}, {15, 0, 3, 45, 0, 3, 0},
    {23, 2, 3, 48, 2, 3, 0}, {11, 0, 3, 42, 0, 3, 0}, {12, 0, 3, 43, 0, 3, 0},
    {21, 2, 3, 17, 2, 1, 1}, {13, 0, 3, 44, 0, 3, 0}, {22, 2, 3, 17, 2, 1, 1},
    {9, 2, 2, 21, 2, 2, 0},  {8, 0, 2, 20, 0, 2, 0},  {10, 2, 2, 22, 2, 2, 0},
};

static daNpcT_motionAnmData_c l_motionAnmData[18] = {
    {8, 2, 1, 14, 0, 1, 1, 0},  {27, 0, 3, 14, 0, 1, 1, 0}, {34, 2, 3, 14, 0, 1, 1, 0},
    {29, 2, 3, 14, 0, 1, 1, 0}, {31, 2, 3, 14, 0, 1, 1, 0}, {30, 0, 3, 14, 0, 1, 1, 0},
    {32, 2, 3, 14, 0, 1, 1, 0}, {33, 0, 3, 14, 0, 1, 1, 0}, {28, 0, 3, 14, 0, 1, 1, 0},
    {6, 0, 3, 37, 0, 3, 0, 0},  {7, 2, 3, 38, 2, 3, 0, 0},  {7, 2, 3, 39, 2, 3, 0, 0},
    {8, 0, 3, 14, 0, 1, 1, 0},  {9, 2, 3, 14, 0, 1, 1, 0},  {10, 0, 3, 14, 0, 1, 1, 0},
    {7, 2, 2, 17, 2, 2, 0, 0},  {6, 2, 2, 16, 2, 2, 0, 0},  {7, 0, 1, 14, 0, 1, 1, 0},
};

static daNpcT_MotionSeqMngr_c::sequenceStepData_c l_faceMotionSequenceData[72] = {
    {1, -1, 1},  {-1, 0, 0}, {-1, 0, 0},  {-1, 0, 0},  {4, -1, 1},  {5, 0, 0},   {-1, 0, 0},
    {-1, 0, 0},  {8, -1, 1}, {9, 0, 0},   {-1, 0, 0},  {-1, 0, 0},  {11, -1, 1}, {12, 0, 0},
    {-1, 0, 0},  {-1, 0, 0}, {13, -1, 1}, {14, 0, 1},  {15, 0, 0},  {-1, 0, 0},  {16, -1, 1},
    {-1, 0, 0},  {-1, 0, 0}, {-1, 0, 0},  {2, -1, 1},  {3, 0, 0},   {-1, 0, 0},  {-1, 0, 0},
    {19, -1, 1}, {20, 0, 0}, {-1, 0, 0},  {-1, 0, 0},  {5, -1, 0},  {-1, 0, 0},  {-1, 0, 0},
    {-1, 0, 0},  {6, -1, 1}, {5, 0, 0},   {-1, 0, 0},  {-1, 0, 0},  {12, -1, 0}, {-1, 0, 0},
    {-1, 0, 0},  {-1, 0, 0}, {15, -1, 0}, {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},  {9, -1, 0},
    {-1, 0, 0},  {-1, 0, 0}, {-1, 0, 0},  {17, -1, 0}, {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},
    {18, -1, 0}, {-1, 0, 0}, {-1, 0, 0},  {-1, 0, 0},  {20, -1, 0}, {-1, 0, 0},  {-1, 0, 0},
    {-1, 0, 0},  {3, -1, 0}, {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0},  {0, -1, 0},  {-1, 0, 0},
    {-1, 0, 0},  {-1, 0, 0},
};

static daNpcT_MotionSeqMngr_c::sequenceStepData_c l_motionSequenceData[56] = {
    {0, -1, 0},  {-1, 0, 0}, {-1, 0, 0},  {-1, 0, 0},  {1, -1, 1}, {0, 0, 0},  {-1, 0, 0},
    {-1, 0, 0},  {4, -1, 0}, {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0}, {8, -1, 1}, {0, -1, 0},
    {-1, 0, 0},  {-1, 0, 0}, {9, -1, 1},  {10, 0, 1},  {11, 0, 0}, {-1, 0, 0}, {12, -1, 1},
    {13, 0, 0},  {-1, 0, 0}, {-1, 0, 0},  {15, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {16, -1, 0}, {-1, 0, 0}, {-1, 0, 0},  {-1, 0, 0},  {5, -1, 1}, {4, 0, 0},  {-1, 0, 0},
    {-1, 0, 0},  {2, -1, 0}, {-1, 0, 0},  {-1, 0, 0},  {-1, 0, 0}, {3, -1, 0}, {-1, 0, 0},
    {-1, 0, 0},  {-1, 0, 0}, {14, -1, 1}, {0, 0, 0},   {-1, 0, 0}, {-1, 0, 0}, {6, -1, 0},
    {-1, 0, 0},  {-1, 0, 0}, {-1, 0, 0},  {17, 4, 1},  {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
};

char* daNpc_Len_c::mCutNameList[4] = {
    "",
    "HURRY",
    "CONVERSATION_IN_HOTEL",
    "TAKE_WOODSTATUE",
};

daNpc_Len_c::cutFunc daNpc_Len_c::mCutList[4] = {
    NULL,
    &daNpc_Len_c::cutHurry,
    &daNpc_Len_c::cutConversationInHotel,
    &daNpc_Len_c::cutTakeWoodStatue,
};

daNpc_Len_c::~daNpc_Len_c() {
    if (mpMorf[0] != NULL) {
        mpMorf[0]->stopZelAnime();
    }

    deleteRes(l_loadResPtrnList[mType], (const char**)l_resNameList);
}

int daNpc_Len_c::create() {
    static const int heapSize[10] = {
        0x4380, 0x4380, 0x4380, 0x4380, 0x4380, 0x4380, 0x4380, 0x4380, 0x4380, 0x0,
    };

    daNpcT_ct(this, daNpc_Len_c, (daNpcT_faceMotionAnmData_c const*)l_faceMotionAnmData,
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
        field_0x9c0.init(&mAcch, 0.0f, 0.0f);
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

int daNpc_Len_c::CreateHeap() {
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

    if (mTwilight) {
        if (setFaceMotionAnm(18, false) && setMotionAnm(15, 0.0f, FALSE)) {
            return 1;
        }
        return 0;
    }
    if (setFaceMotionAnm(1, false) && setMotionAnm(0, 0.0f, FALSE)) {
        return 1;
    }
    return 0;
}

int daNpc_Len_c::Delete() {
    fopAcM_GetID(this);
    this->~daNpc_Len_c();
    return 1;
}

int daNpc_Len_c::Execute() {
    return daNpcT_c::execute();
}

int daNpc_Len_c::Draw() {
    if (mpMatAnm[0] != NULL) {
        J3DModelData* modelData = mpMorf[0]->getModel()->getModelData();
        modelData->getMaterialNodePointer(getEyeballMaterialNo())->setMaterialAnm(mpMatAnm[0]);
    }
    return daNpcT_c::draw(0, 0, mRealShadowSize, NULL, 100.0f, 0, 0, 0);
}

int daNpc_Len_c::createHeapCallBack(fopAc_ac_c* i_this) {
    return static_cast<daNpc_Len_c*>(i_this)->CreateHeap();
}

int daNpc_Len_c::ctrlJointCallBack(J3DJoint* param_0, int param_1) {
    if (param_1 == 0) {
        J3DModel* model = j3dSys.getModel();

        daNpc_Len_c* i_this = reinterpret_cast<daNpc_Len_c*>(model->getUserArea());
        if (i_this != 0) {
            i_this->ctrlJoint(param_0, model);
        }
    }

    return 1;
}

u8 daNpc_Len_c::getType() {
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
    case 8:
        return TYPE_8;
    }
    return TYPE_9;
}

BOOL daNpc_Len_c::isDelete() {
    switch (mType) {
    case TYPE_0:
        /* dSv_event_flag_c::M_028 - Cutscene - [cutscene: 14] restore mountain spirit - Reunion
         * with Colin et al. */
        return !daNpcT_chkEvtBit(0x3D) ||
               /* dSv_event_flag_c::F_0066 - Death Mountain - First saw Goron cutscene on mountain
                  path */
               daNpcT_chkEvtBit(0xA4);
    case TYPE_1:
        return false;
    case TYPE_2:
        /* dSv_event_flag_c::F_0066 - Death Mountain - First saw Goron cutscene on mountain path */
        return !daNpcT_chkEvtBit(0xA4) ||
               /* dSv_event_flag_c::M_020 - Cutscene - [cutscene: ] Colin kidnapped : ON once
                  watched */
               daNpcT_chkEvtBit(0x35);
    case TYPE_3:
        /* dSv_event_flag_c::M_020 - Cutscene - [cutscene: ] Colin kidnapped : ON once watched */
        return !daNpcT_chkEvtBit(0x35) ||
               /* dSv_event_flag_c::M_052 - Main Event - Horseback battle clear */
               daNpcT_chkEvtBit(0x55);
    case TYPE_4:
        /* dSv_event_flag_c::M_052 - Main Event - Horseback battle clear */
        return !daNpcT_chkEvtBit(0x55) ||
               /* dSv_event_flag_c::M_031 - Goron Mines - Goron Mines clear */
               daNpcT_chkEvtBit(0x40);
    case TYPE_5:
        /* dSv_event_flag_c::F_288 - Cutscene - [cutscene: ] Ilia gets her memories back */
        if (daNpcT_chkEvtBit(0x120) == FALSE) {
            /* dSv_event_flag_c::M_031 - Goron Mines - Goron Mines clear */
            return !daNpcT_chkEvtBit(0x40) ||
                   /* dSv_event_flag_c::M_035 - Cutscene - [cutscene: 35] after carriage guarding
                      event */
                   daNpcT_chkEvtBit(0x44);
        }
        return false;
    case TYPE_6:
        /* dSv_event_flag_c::F_288 - Cutscene - [cutscene: ] Ilia gets her memories back */
        if (daNpcT_chkEvtBit(0x120) == FALSE) {
            /* dSv_event_flag_c::M_031 - Goron Mines - Goron Mines clear */
            return !daNpcT_chkEvtBit(0x40) ||
                   /* dSv_event_flag_c::M_035 - Cutscene - [cutscene: 35] after carriage guarding
                      event */
                   daNpcT_chkEvtBit(0x44);
        }
        return false;
    case TYPE_7:
        /* dSv_event_flag_c::M_035 - Cutscene - [cutscene: 35] after carriage guarding event */
        return !daNpcT_chkEvtBit(0x44) ||
               /* dSv_event_flag_c::F_0264 - Cutscene - Get master sword */
               daNpcT_chkEvtBit(0x108);
    case TYPE_8:
        /* dSv_event_flag_c::F_0264 - Cutscene - Get master sword */
        return !daNpcT_chkEvtBit(0x108) ||
               /* dSv_event_flag_c::F_288 - Cutscene - [cutscene: ] Ilia gets her memories back */
               daNpcT_chkEvtBit(0x120);
    }
    return false;
}

void daNpc_Len_c::reset() {
    int iVar1 = (u8*)&field_0xff0 - (u8*)&field_0xfcc;
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
    memset(&field_0xfcc, 0, iVar1);
    csXyz acStack_20;
    acStack_20.setall(0.0f);
    acStack_20.y = home.angle.y;
    setAngle(acStack_20);
}

void daNpc_Len_c::afterJntAnm(int param_1) {
    if (param_1 == 1) {
        mDoMtx_stack_c::YrotM(mStagger.getAngleZ(1));
        mDoMtx_stack_c::ZrotM(-mStagger.getAngleX(1));
    } else if (param_1 == 4) {
        mDoMtx_stack_c::YrotM(mStagger.getAngleZ(0));
        mDoMtx_stack_c::ZrotM(mStagger.getAngleX(0));
    }
    if (param_1 == 6) {
        mDoMtx_stack_c::YrotM(-mJntAnm.getJntRadY2S(0) * 0.6f);
    }
}

void daNpc_Len_c::setParam() {
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
        talk_distance = 9;
        talk_angle = 4;
        attention_distance = 17;
        attention_angle = 4;
        break;
    case TYPE_3:
        field_0xd8a.y = -10000;
        talk_distance = 13;
        attention_distance = 15;
        break;
    case TYPE_5:
        talk_angle = 4;
        break;
    }

    if (daPy_getPlayerActorClass()->checkHorseRide()) {
        if (talk_distance < 7) {
            talk_distance = 7;
        }
        if (attention_distance < 9) {
            attention_distance = 9;
        }
    }

    attention_info.distances[fopAc_attn_LOCK_e] =
        daNpcT_getDistTableIdx(attention_distance, attention_angle);
    attention_info.distances[fopAc_attn_TALK_e] = attention_info.distances[fopAc_attn_LOCK_e];
    attention_info.distances[fopAc_attn_SPEAK_e] =
        daNpcT_getDistTableIdx(talk_distance, talk_angle);
    attention_info.flags = fopAc_AttnFlag_SPEAK_e | fopAc_AttnFlag_TALK_e;

    scale.set(mpHIO->m.common.scale, mpHIO->m.common.scale, mpHIO->m.common.scale);

    if (mType == TYPE_5) {
        mCcStts.SetWeight(109);
    } else {
        mCcStts.SetWeight(mpHIO->m.common.weight);
    }

    mCylH = mpHIO->m.common.height;
    mWallR = mpHIO->m.common.width;
    if (mType == TYPE_1) {
        mCylH = 160.0f;
    }

    mAttnFovY = mpHIO->m.common.fov;
    mAcchCir.SetWallR(mWallR);
    mAcchCir.SetWallH(mpHIO->m.common.knee_length);
    mRealShadowSize = mpHIO->m.common.real_shadow_size;
    mExpressionMorfFrame = mpHIO->m.common.expression_morf_frame;
    mMorfFrames = mpHIO->m.common.morf_frame;
    gravity = mpHIO->m.common.gravity;
}

BOOL daNpc_Len_c::checkChangeEvt() {
    if (!chkAction(&daNpc_Len_c::talk)) {
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
        case TYPE_1:
        case TYPE_2:
        case TYPE_3:
        case TYPE_5:
        case TYPE_6:
        case TYPE_7:
        case TYPE_8:
        case TYPE_9:
            break;
        case TYPE_4:
            /* dSv_event_flag_c::T_0057 - Kakariko Village (inside) - Barnes bomb shop <purchase> */
            if (daNpcT_chkTmpBit(0x3A)) {
                mEvtNo = 6;
            } else {
                mEvtNo = 5;
            }
            evtChange();
            return TRUE;
        }
    }

    return FALSE;
}

void daNpc_Len_c::setAfterTalkMotion() {
    int iVar2 = MOT_UNK_17;
    switch (mFaceMotionSeqMngr.getNo()) {
    case 1:
        iVar2 = MOT_UNK_8;
        break;
    case 2:
        return;
    case 3:
        iVar2 = MOT_UNK_10;
        break;
    case 4:
        iVar2 = MOT_UNK_11;
        break;
    case 5:
        iVar2 = MOT_UNK_13;
        break;
    case 6:
        iVar2 = MOT_UNK_16;
        break;
    case 9:
        iVar2 = MOT_UNK_8;
        break;
    }
    mFaceMotionSeqMngr.setNo(iVar2, -1.0f, 0, 0);
}

void daNpc_Len_c::srchActors() {
    switch (mType) {
    case TYPE_3:
        break;
    case TYPE_4:
        if (mActorMngr[0].getActorP() == NULL) {
            mActorMngr[0].entry(getNearestActorP(PROC_NPC_KOLINB));
        }
        break;
    case TYPE_8:
        if (mActorMngr[1].getActorP() == NULL) {
            mActorMngr[1].entry(getNearestActorP(PROC_NPC_YELIA));
        }
        if (mActorMngr[2].getActorP() == NULL) {
            mActorMngr[2].entry(getNearestActorP(PROC_NPC_GRD));
        }
        if (mActorMngr[3].getActorP() == NULL) {
            mActorMngr[3].entry(getNearestActorP(PROC_NPC_GRZ));
        }
        break;
    }
}

BOOL daNpc_Len_c::evtTalk() {
    if (chkAction(&daNpc_Len_c::talk)) {
        (this->*mAction)(NULL);
    } else {
        setAction(&daNpc_Len_c::talk);
    }
    return TRUE;
}

BOOL daNpc_Len_c::evtCutProc() {
    int staffId = dComIfGp_getEventManager().getMyStaffId("Len", this, -1);
    if (staffId != -1) {
        mStaffId = staffId;
        int actIdx =
            dComIfGp_getEventManager().getMyActIdx(mStaffId, (char**)mCutNameList, 4, 0, 0);
        if ((this->*(mCutList[actIdx]))(mStaffId) != 0) {
            dComIfGp_getEventManager().cutEnd(mStaffId);
        }
        return true;
    }
    return false;
}

void daNpc_Len_c::action() {
    fopAc_ac_c* hit_actor = NULL;

    if (!mTwilight) {
        hit_actor = hitChk(&mCyl, 0xffffffff);
    }

    if (hit_actor) {
        cCcD_ObjHitInf* hit_obj = mCyl.GetTgHitObj();
        if (hit_obj->ChkAtType(AT_TYPE_THROW_OBJ)) {
            mStagger.setParam(this, hit_actor, mCurAngle.y);
            setDamage(0, 17, 0);
            mDamageTimerStart = 0;
            mJntAnm.lookNone(1);
        }
    }

    if (mStagger.checkRebirth()) {
        // FIXME: Fakematch - same issue in npc_besu
#if DEBUG
        mStagger.initialize();
#else
        for (int i = 0; i < 2; i++) {
            mStagger.mAngle[i].setall(0);
            mStagger.mPower[i] = 0.0f;
        }
        mStagger.mStagger = 0;
        mStagger.field_0x16 = 0;
        mStagger.mRebirth = 0;
#endif
        mMode = 1;
    }

    if (field_0xfcc != NULL) {
        if (mAction == field_0xfcc) {
            (this->*mAction)(NULL);
        } else {
            setAction(field_0xfcc);
        }
    }

    daTag_Push_c* push_tag_p = (daTag_Push_c*)field_0xba0.getActorP();

    if (push_tag_p != NULL) {
        switch (push_tag_p->getId()) {
        case 3:
            mEvtNo = 4;
            break;
        }
    }
}

void daNpc_Len_c::beforeMove() {
    fopAcM_OffStatus(this, fopAcM_STATUS_UNK_0x8000000);
    if (checkHide()) {
        fopAcM_OnStatus(this, fopAcM_STATUS_UNK_0x8000000);
    }

    if (checkHide() || mNoDraw != 0) {
        attention_info.flags = 0;
    }
}

NPC_LEN_HIO_CLASS l_HIO;

void daNpc_Len_c::setAttnPos() {
    cXyz acStack_40(-30.0f, 15.0f, 0.0f);
    cXyz cStack_4c(0.0f, 15.0f, 0.0f);

    if (field_0xfec != 0) {
        daPy_getPlayerActorClass()->onWolfEyeKeep();
    }

    mStagger.calc(0);

    f32 dVar5 = cM_s2rad(mCurAngle.y - field_0xd7e.y);

    mJntAnm.setParam(this, mpMorf[0]->getModel(), &acStack_40, getBackboneJointNo(),
                     getNeckJointNo(), getHeadJointNo(), mpHIO->m.common.body_angleX_min,
                     mpHIO->m.common.body_angleX_max, mpHIO->m.common.body_angleY_min,
                     mpHIO->m.common.body_angleY_max, mpHIO->m.common.head_angleX_min,
                     mpHIO->m.common.head_angleX_max, mpHIO->m.common.head_angleY_min,
                     mpHIO->m.common.head_angleY_max, mpHIO->m.common.neck_rotation_ratio, dVar5,
                     &cStack_4c);

    if (mJntAnm.getMode() == daNpcT_JntAnm_c::LOOK_MODE_7) {
        mJntAnm.calcJntRad(0.1f, 1.0f, dVar5);
    } else {
        mJntAnm.calcJntRad(0.2f, 1.0f, dVar5);
    }

    setMtx();

    mDoMtx_stack_c::copy(mpMorf[0]->getModel()->getAnmMtx(getHeadJointNo()));
    mDoMtx_stack_c::multVec(&acStack_40, &eyePos);

    mJntAnm.setEyeAngleX(eyePos, 1.0f, 0);
    mJntAnm.setEyeAngleY(eyePos, mCurAngle.y + field_0xd8a.y, 1, 1.0f, 0);

    if (mType == TYPE_1) {
        acStack_40.set(0.0f, 190.0f, 0.0f);
    } else {
        acStack_40.set(0.0f, 0.0f, 0.0f);
        acStack_40.y = mpHIO->m.common.attention_offset;
    }

    mDoMtx_stack_c::YrotS(mCurAngle.y);
    mDoMtx_stack_c::multVec(&acStack_40, &acStack_40);
    attention_info.position = current.pos + acStack_40;

    setFootPos();
}

void daNpc_Len_c::setCollision() {
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

int daNpc_Len_c::drawDbgInfo() {
    return 0;
}

int daNpc_Len_c::selectAction() {
    field_0xfcc = NULL;

    switch (mType) {
    case TYPE_5:
        field_0xfcc = &daNpc_Len_c::patrol;
        break;
    default:
        field_0xfcc = &daNpc_Len_c::wait;
    }

    return 1;
}

BOOL daNpc_Len_c::chkAction(actionFunc param_0) {
    return mAction == param_0;
}

int daNpc_Len_c::setAction(actionFunc param_0) {
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

BOOL daNpc_Len_c::checkStartDemo13StbEvt(fopAc_ac_c* i_npc_p, f32 i_box_min_x, f32 i_box_min_y,
                                         f32 i_box_min_z, f32 i_box_max_x, f32 i_box_max_y,
                                         f32 i_box_max_z, f32 i_box_offset) {
    if (strcmp(dComIfGp_getStartStageName(), "R_SP109") == 0 && ((daNpcT_c*)i_npc_p)->mTwilight &&
        daPy_py_c::checkNowWolfEyeUp() &&
        /* dSv_event_flag_c::M_027 - Cutscene - [cutscene: 13] kids in the church (beast eyes) */
        !daNpcT_chkEvtBit(0x3C) &&
        daNpcT_chkActorInScreen(i_npc_p, i_box_min_x, i_box_min_y, i_box_min_z, i_box_max_x,
                                i_box_max_y, i_box_max_z, i_box_offset, 0))
    {
        return TRUE;
    }
    return FALSE;
}

int daNpc_Len_c::cutConversationInHotel(int param_0) {
    fopAc_ac_c* actor_p;
    int rv = 0;
    int iVar6 = -1;
    int local_34 = 0;
    int iVar5 = 0;

    int* piVar1 = dComIfGp_evmng_getMyIntegerP(param_0, "prm");
    if (piVar1) {
        iVar6 = *piVar1;
    }

    piVar1 = dComIfGp_evmng_getMyIntegerP(param_0, "timer");
    if (piVar1) {
        local_34 = *piVar1;
    }

    piVar1 = dComIfGp_evmng_getMyIntegerP(param_0, "msgNo");
    if (piVar1) {
        iVar5 = *piVar1;
    }

    if (dComIfGp_getEventManager().getIsAddvance(param_0)) {
        switch (iVar6) {
        case 0:
            mFaceMotionSeqMngr.setNo(FACE_MOT_UNK_17, -1.0f, 0, 0);
            mMotionSeqMngr.setNo(MOT_UNK_0, -1.0f, 0, 0);
            /* dSv_event_tmp_flag_c::T_0010 - General use - General use temporary flag (flow
             * control) A */
            daNpcT_offTmpBit(0xB);
            initTalk(mFlowNodeNo, NULL);
            break;
        case 2:
            mEventTimer = local_34;
            initTalk(mFlowNodeNo, NULL);
            break;
        case 3:
            mJntAnm.lookNone(1);
            break;
        case 4:
            break;
        }
    }

    int local_30[2] = {-1, -1};

    switch (iVar6) {
    case 0:
        mJntAnm.lookPlayer(0);
        if (mPlayerAngle != mCurAngle.y) {
            step(mPlayerAngle, 17, 13, 15, 0);
        } else {
            rv = 1;
        }
        break;
    case 1:
        if (talkProc(NULL, FALSE, NULL, FALSE) && mFlow.checkEndFlow()) {
            rv = 1;
        }
        break;
    case 2:
        actor_p = mActorMngr[0].getActorP();
        if (actor_p != NULL) {
            mJntAnm.lookActor(actor_p, -40.0f, 0);
        }
        if (cLib_calcTimer(&mEventTimer) == 0 && home.angle.y == mCurAngle.y &&
            talkProc(NULL, FALSE, NULL, FALSE) && mFlow.checkEndFlow())
        {
            rv = 1;
        }
        if (home.angle.y != mCurAngle.y && step(home.angle.y, 17, 13, 15, 0)) {
            mFaceMotionSeqMngr.setNo(FACE_MOT_UNK_17, -1.0f, 0, 0);
            mMotionSeqMngr.setNo(MOT_UNK_0, -1.0f, 0, 0);
        }
        break;
    case 3:
        rv = 1;
        break;
    case 4:
        mJntAnm.lookPlayer(0);
        local_30[0] = iVar5;
        if (talkProc(local_30, FALSE, NULL, FALSE)) {
            if (iVar5 == 0) {
                if (mFlow.checkEndFlow()) {
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

int daNpc_Len_c::cutHurry(int param_0) {
    int rv = 0;
    int iVar5 = -1;
    daTag_Push_c* push_tag_p;

    int* piVar1 = dComIfGp_evmng_getMyIntegerP(param_0, "prm");
    if (piVar1) {
        iVar5 = *piVar1;
    }

    if (dComIfGp_getEventManager().getIsAddvance(param_0)) {
        switch (iVar5) {
        case 0:
            push_tag_p = (daTag_Push_c*)field_0xba0.getActorP();
            mFaceMotionSeqMngr.setNo(FACE_MOT_UNK_9, -1.0f, 0, 0);
            mMotionSeqMngr.setNo(MOT_UNK_8, -1.0f, 0, 0);
            mJntAnm.lookNone(1);
            push_tag_p->pushBackPlayer(0);
            break;
        case 1:
            push_tag_p = (daTag_Push_c*)field_0xba0.getActorP();
            initTalk(push_tag_p->getFlowNodeNo(), NULL);
            break;
        }
    }

    switch (iVar5) {
    case 0:
        if (mMotionSeqMngr.getStepNo() > 0) {
            rv = 1;
        }
        break;
    case 1:
        if (talkProc(NULL, FALSE, NULL, FALSE) && mFlow.checkEndFlow()) {
            field_0xba0.remove();
            rv = 1;
        }
        break;
    }

    return rv;
}

int daNpc_Len_c::cutTakeWoodStatue(int param_0) {
    fopAc_ac_c* actor_p;
    int iVar7 = -1;
    int local_34 = 0;

    int* piVar1 = dComIfGp_evmng_getMyIntegerP(param_0, "prm");
    if (piVar1) {
        iVar7 = *piVar1;
    }

    piVar1 = dComIfGp_evmng_getMyIntegerP(param_0, "timer");
    if (piVar1) {
        local_34 = *piVar1;
    }

    if (dComIfGp_getEventManager().getIsAddvance(param_0)) {
        switch (iVar7) {
        case 0:
            actor_p = mActorMngr[1].getActorP();
            mJntAnm.lookPos(&actor_p->eyePos, 0);
            break;
        case 1:
            actor_p = mActorMngr[2].getActorP();
            mJntAnm.lookPos(&actor_p->eyePos, 0);
            setAngle(fopAcM_searchActorAngleY(this, actor_p));
            mEventTimer = local_34;
            mHide = 1;
            break;
        case 2:
            actor_p = mActorMngr[3].getActorP();
            mJntAnm.lookPos(&actor_p->eyePos, 0);
            break;
        case 3:
            mHide = 1;
            break;
        case 4:
        case 99:
            mFaceMotionSeqMngr.setNo(FACE_MOT_UNK_17, 0.0f, 1, 0);
            mMotionSeqMngr.setNo(MOT_UNK_0, 0.0f, 1, 0);
            mJntAnm.lookNone(1);
            setPos(home.pos);
            setAngle(home.angle.y);
            mHide = 0;
            break;
        case 5:
            mHide = 0;
            break;
        }
    }

    switch (iVar7) {
    case 0:
    case 2:
    case 3:
    case 4:
    case 5:
    case 99:
        return 1;
    case 1:
        return 1;
    }

    return 1;
}

int daNpc_Len_c::wait(void* param_0) {
    switch (mMode) {
    case 0:
    case 1:
        if (!mStagger.checkStagger()) {
            switch (mType) {
            case TYPE_1:
                if (getBitSW() != 0xFF && dComIfGs_isSwitch(getBitSW(), fopAcM_GetRoomNo(this))) {
                    mFaceMotionSeqMngr.setNo(FACE_MOT_UNK_15, -1.0f, 0, 0);
                    mMotionSeqMngr.setNo(MOT_UNK_7, -1.0f, 0, 0);
                } else {
                    mFaceMotionSeqMngr.setNo(FACE_MOT_UNK_14, -1.0f, 0, 0);
                    mMotionSeqMngr.setNo(MOT_UNK_6, -1.0f, 0, 0);
                }
                break;
            case TYPE_2:
                mFaceMotionSeqMngr.setNo(FACE_MOT_UNK_17, -1.0f, 0, 0);
                mMotionSeqMngr.setNo(MOT_UNK_0, -1.0f, 0, 0);
                if (getBitSW() != 0xFF && !dComIfGs_isSwitch(getBitSW(), fopAcM_GetRoomNo(this))) {
                    mPlayerActorMngr.entry(daPy_getPlayerActorClass());
                    setAngle(fopAcM_searchPlayerAngleY(this));
                }
                break;
            case TYPE_3:
                mFaceMotionSeqMngr.setNo(FACE_MOT_UNK_17, -1.0f, 0, 0);
                mMotionSeqMngr.setNo(MOT_UNK_10, -1.0f, 0, 0);
                break;
            case TYPE_7:
                mFaceMotionSeqMngr.setNo(FACE_MOT_UNK_12, -1.0f, 0, 0);
                mMotionSeqMngr.setNo(MOT_UNK_12, -1.0f, 0, 0);
                break;
            default:
                mFaceMotionSeqMngr.setNo(FACE_MOT_UNK_17, -1.0f, 0, 0);
                mMotionSeqMngr.setNo(MOT_UNK_0, -1.0f, 0, 0);
            }

            mMode = 2;
        }
    case 2:
        if (field_0xfee != 0) {
            mPlayerActorMngr.entry(daPy_getPlayerActorClass());
            mSpeakEvent = true;
            field_0xe33 = true;
        } else {
            switch (mType) {
            case TYPE_1:
                if (checkStartDemo13StbEvt(this, mpHIO->m.common.box_min_x,
                                           mpHIO->m.common.box_min_y, mpHIO->m.common.box_min_z,
                                           mpHIO->m.common.box_max_x, mpHIO->m.common.box_max_y,
                                           mpHIO->m.common.box_max_z, mpHIO->m.common.box_offset))
                {
                    mEvtNo = 3;
                    field_0xfec = 1;
                }
                /* dSv_event_flag_c::M_027 - Cutscene - [cutscene: 13] kids in the church (beast
                 * eyes) */
                if (field_0xfec != 0 && daNpcT_chkEvtBit(0x3C) && !dComIfGp_event_runCheck()) {
                    field_0xfec = 0;
                }
                break;
            case TYPE_2:
                if (getBitSW() != 0xFF && !dComIfGs_isSwitch(getBitSW(), fopAcM_GetRoomNo(this))) {
                    mSpeakEvent = true;
                    field_0xe33 = true;
                }
                break;
            }
        }
        if (!mStagger.checkStagger()) {
            if (mType == TYPE_1) {
                mJntAnm.lookNone(0);
                if (getBitSW() != 0xFF && dComIfGs_isSwitch(getBitSW(), fopAcM_GetRoomNo(this)) &&
                    mMotionSeqMngr.getNo() != MOT_UNK_7)
                {
                    mFaceMotionSeqMngr.setNo(FACE_MOT_UNK_15, -1.0f, 0, 0);
                    mMotionSeqMngr.setNo(MOT_UNK_7, -1.0f, 0, 0);
                }
            } else {
                if (mType == TYPE_3) {
                    mPlayerActorMngr.remove();
                }
                if (mPlayerActorMngr.getActorP() != NULL && !mTwilight &&
                    (mType != TYPE_4 && mType != TYPE_7 || home.angle.y == mCurAngle.y))
                {
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
                            if (step(home.angle.y, 17, 13, 15, 0)) {
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
            mJntAnm.getMode();
            switch (mType) {
            case TYPE_3:
                attention_info.flags = 0;
                break;
            }
        }
        break;
    case 3:
        break;
    }
    return 1;
}

int daNpc_Len_c::patrol(void* param_0) {
    cXyz cStack_48;

    int sVar1 = mpHIO->m.field_0x8c;
    int sVar2 = mpHIO->m.field_0x8e;

    switch (mMode) {
    case 0:
    case 1:
        if (!mStagger.checkStagger()) {
            mFaceMotionSeqMngr.setNo(FACE_MOT_UNK_17, -1.0f, 0, 0);
            mMotionSeqMngr.setNo(MOT_UNK_0, -1.0f, 0, 0);
            field_0xfe8 = 0;
            field_0xfe4 = 0;
            mMode = 2;
        }
    case 2:
        if (!mStagger.checkStagger()) {
            int iVar3 = mPath.getDstPosH(current.pos, &cStack_48, mPath.getNumPnts(), 4);

            BOOL bVar8 = iVar3 != 0 || field_0xfe4 == 0;

            if (bVar8) {
                speedF = 0.0f;
            } else {
                current.angle.y = cLib_targetAngleY(&current.pos, &cStack_48);
                cLib_addCalcAngleS2(&shape_angle.y, current.angle.y, 8, 0x400);
                mCurAngle.y = shape_angle.y;
                cLib_chaseF(&speedF, mpHIO->m.field_0x90, 0.55f);
            }

            if (bVar8) {
                if (cM3d_IsZero(speedF)) {
                    if (field_0xfe4 == 0) {
                        if (field_0xfe8 == 0) {
                            mFaceMotionSeqMngr.setNo(FACE_MOT_UNK_17, -1.0f, 0, 0);
                            mMotionSeqMngr.setNo(MOT_UNK_0, -1.0f, 0, 0);
                            field_0xfe8 = cLib_getRndValue((int)(sVar2 / 2), (int)sVar2);
                        }
                    } else {
                        home.angle.y = 0x4000;
                        if (home.angle.y == mCurAngle.y) {
                            mPlayerActorMngr.remove();
                        } else {
                            step(home.angle.y, 17, 13, 15, 0);
                        }
                    }
                }
            }

            mJntAnm.lookAround(0, 0x5A);

            if (mPlayerActorMngr.getActorP() != NULL && !mTwilight) {
                if (chkActorInSight(mPlayerActorMngr.getActorP(), mAttnFovY, mCurAngle.y)) {
                    mJntAnm.lookPlayer(0);
                }
                if (!srchPlayerActor() && home.angle.y == mCurAngle.y) {
                    mMode = 1;
                }
            } else if (!mTwilight) {
                srchPlayerActor();
            }

            mJntAnm.getMode();

            if (field_0xfe4 == 0) {
                if (cLib_calcTimer(&field_0xfe8) == 0) {
                    mFaceMotionSeqMngr.setNo(FACE_MOT_UNK_17, -1.0f, 0, 0);
                    mMotionSeqMngr.setNo(MOT_UNK_9, -1.0f, 0, 0);
                    field_0xfe4 = cLib_getRndValue((int)(sVar1 / 2), (int)sVar1);
                }
            } else {
                cLib_calcTimer(&field_0xfe4);
                field_0xfe8 = 0;
            }
        }
        break;
    case 3:
        break;
    }

    return 1;
}

int daNpc_Len_c::talk(void* param_0) {
    switch (mMode) {
    case 0:
    case 1:
        if (!mStagger.checkStagger()) {
            if (field_0xfee == 0) {
                /* dSv_event_tmp_flag_c::T_0010 - General use - General use temporary flag (flow
                 * control) A */
                daNpcT_offTmpBit(0xB);
            }
            if (mPreItemNo == fpcNm_ITEM_WOOD_STATUE) {
                initTalk(0x28, NULL);
            } else {
                initTalk(mFlowNodeNo, NULL);
            }
            switch (mType) {
            case TYPE_7:
                mFaceMotionSeqMngr.setNo(FACE_MOT_UNK_17, -1.0f, 0, 0);
                mMotionSeqMngr.setNo(MOT_UNK_0, -1.0f, 0, 0);
                break;
            }
            mMode = 2;
        }
    case 2:
        if (!mStagger.checkStagger()) {
            if (mTwilight || mPlayerAngle == mCurAngle.y || mType == 0 || mType == 2) {
                if (talkProc(NULL, FALSE, NULL, FALSE)) {
                    int local_18;
                    int evt_id = mFlow.getEventId(&local_18);
                    field_0xfee = 0;
                    switch (evt_id) {
                    case 1:
                        if (mItemPartnerId == -1) {
                            mItemPartnerId = fopAcM_createItemForPresentDemo(&current.pos, local_18,
                                                                             0, -1, -1, 0, 0);
                        }
                        if (fopAcM_IsExecuting(mItemPartnerId)) {
                            mEvtNo = 1;
                            field_0xfee = 1;
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
                if (mTwilight) {
                    mJntAnm.lookNone(0);
                }
            } else {
                mJntAnm.lookPlayer(0);
                step(mPlayerAngle, 17, 13, 15, 0);
            }
        }
        break;
    case 3:
        break;
    }

    return 0;
}

static int daNpc_Len_Create(void* i_this) {
    return static_cast<daNpc_Len_c*>(i_this)->create();
}

static int daNpc_Len_Delete(void* i_this) {
    return static_cast<daNpc_Len_c*>(i_this)->Delete();
}

static int daNpc_Len_Execute(void* i_this) {
    return static_cast<daNpc_Len_c*>(i_this)->Execute();
}

static int daNpc_Len_Draw(void* i_this) {
    return static_cast<daNpc_Len_c*>(i_this)->Draw();
}

static int daNpc_Len_IsDelete(void* i_this) {
    return 1;
}

static actor_method_class daNpc_Len_MethodTable = {
    (process_method_func)daNpc_Len_Create,  (process_method_func)daNpc_Len_Delete,
    (process_method_func)daNpc_Len_Execute, (process_method_func)daNpc_Len_IsDelete,
    (process_method_func)daNpc_Len_Draw,
};

extern actor_process_profile_definition g_profile_NPC_LEN = {
    fpcLy_CURRENT_e,         // mLayerID
    7,                       // mListID
    fpcPi_CURRENT_e,         // mListPrio
    PROC_NPC_LEN,            // mProcName
    &g_fpcLf_Method.base,    // sub_method
    sizeof(daNpc_Len_c),     // mSize
    0,                       // mSizeOther
    0,                       // mParameters
    &g_fopAc_Method.base,    // sub_method
    337,                     // mPriority
    &daNpc_Len_MethodTable,  // sub_method
    0x00040107,              // mStatus
    fopAc_NPC_e,             // mActorType
    fopAc_CULLBOX_CUSTOM_e,  // cullType
};
