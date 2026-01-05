/**
 * @file d_a_npc_knj.cpp
 *
 */

#include "d/dolzel_rel.h"  // IWYU pragma: keep

#include "d/actor/d_a_npc_knj.h"

const daNpc_Knj_HIOParam daNpc_Knj_Param_c::m = {
    0.0f,     // attention_offset
    0.0f,     // gravity
    1.0f,     // scale
    4000.0f,  // real_shadow_size
    0.0f,     // weight
    0.0f,     // height
    0.0f,     // knee_length
    0.0f,     // width
    0.0f,     // body_angleX_max
    0.0f,     // body_angleX_min
    0.0f,     // body_angleY_max
    0.0f,     // body_angleY_min
    0.0f,     // head_angleX_max
    0.0f,     // head_angleX_min
    0.0f,     // head_angleY_max
    0.0f,     // head_angleY_min
    0.0f,     // neck_rotation_ratio
    0.0f,     // morf_frame
    0,        // talk_distance
    0,        // talk_angle
    0,        // attention_distance
    0,        // attention_angle
    0.0f,     // fov
    0.0f,     // search_distance
    0.0f,     // search_height
    0.0f,     // search_depth
    0,        // attention_time
    0,        // damage_time
    0,        // face_expression
    0,        // motion
    0,        // look_mode
    0,        // debug_mode_ON
    0,        // debug_info_ON
    0.0f,     // expression_morf_frame
    0.0f,     // box_min_x
    0.0f,     // box_min_y
    0.0f,     // box_min_z
    0.0f,     // box_max_x
    0.0f,     // box_max_y
    0.0f,     // box_max_z
    0.0f,     // box_offset
};

#if DEBUG
daNpc_Knj_HIO_c::daNpc_Knj_HIO_c() {
    m = daNpc_Knj_Param_c::m;
}

void daNpc_Knj_HIO_c::listenPropertyEvent(const JORPropertyEvent* event) {
    // TODO
}

void daNpc_Knj_HIO_c::genMessage(JORMContext* ctext) {
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
};

enum FaceMotion {
    /* 0x00 */ FACE_MOT_UNK_0 = 0,
};

enum Motion {
    /* 0x00 */ MOT_UNK_0 = 0,
};

static int l_bmdData[1][2] = {
    {9, 1},
};

static daNpcT_evtData_c l_evtList[2] = {
    {"", 0},
    {"NO_RESPONSE", 0},
};

static char* l_resNameList[2] = {
    "",
    "Knj",
};

static s8 l_loadResPtrn0[2] = {1, -1};

static s8* l_loadResPtrnList[6] = {
    l_loadResPtrn0, l_loadResPtrn0, l_loadResPtrn0, l_loadResPtrn0, l_loadResPtrn0, l_loadResPtrn0,
};

static daNpcT_faceMotionAnmData_c l_faceMotionAnmData[1] = {
    {-1, 0, 0, -1, 0, 0, 0},
};

static daNpcT_motionAnmData_c l_motionAnmData[1] = {
    {6, 2, 1, -1, 0, 0, 0, 0},
};

static daNpcT_MotionSeqMngr_c::sequenceStepData_c l_faceMotionSequenceData[4] = {
    {-1, 0, 0},
    {-1, 0, 0},
    {-1, 0, 0},
    {-1, 0, 0},
};

static daNpcT_MotionSeqMngr_c::sequenceStepData_c l_motionSequenceData[4] = {
    {0, 0, 0},
    {-1, 0, 0},
    {-1, 0, 0},
    {-1, 0, 0},
};

char* daNpc_Knj_c::mCutNameList[1] = {""};

daNpc_Knj_c::cutFunc daNpc_Knj_c::mCutList[1] = {
    NULL,
};

NPC_KNJ_HIO_CLASS l_HIO;

daNpc_Knj_c::~daNpc_Knj_c() {
    if (heap != NULL) {
        mpMorf[0]->stopZelAnime();
    }
    deleteRes(l_loadResPtrnList[mType], (const char**)l_resNameList);
}

int daNpc_Knj_c::create() {
    daNpcT_ct(this, daNpc_Knj_c, l_faceMotionAnmData, l_motionAnmData, l_faceMotionSequenceData, 4,
              l_motionSequenceData, 4, l_evtList, l_resNameList);

    mType = getType();
    mFlowNodeNo = getFlowNodeNo();
    mTwilight = 0;

    int rv = loadRes(l_loadResPtrnList[mType], (const char**)l_resNameList);
    if (rv == cPhs_COMPLEATE_e) {
        if (!fopAcM_entrySolidHeap(this, createHeapCallBack, 0)) {
            return cPhs_ERROR_e;
        }

        if (isDelete()) {
            return cPhs_ERROR_e;
        }

        J3DModelData* modelData = mpMorf[0]->getModel()->getModelData();
        fopAcM_SetMtx(this, mpMorf[0]->getModel()->getBaseTRMtx());
        mSound.init(&current.pos, &eyePos, 3, 1);
        mAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1, &mAcchCir,
                  fopAcM_GetSpeed_p(this), fopAcM_GetAngle_p(this), fopAcM_GetShapeAngle_p(this));
        mAcch.CrrPos(dComIfG_Bgsp());
        mGndChk = mAcch.m_gnd;
        mGroundH = mAcch.GetGroundH();
        setEnvTevColor();
        setRoomNo();
        mCcStts.Init(mpHIO->m.common.weight, 0, this);
        reset();
        mCreating = true;
        Execute();
        mCreating = false;
    }

    return rv;
}

int daNpc_Knj_c::CreateHeap() {
    J3DModelData* modelData = static_cast<J3DModelData*>(
        dComIfG_getObjectRes(l_resNameList[l_bmdData[0][1]], l_bmdData[0][0]));

    mpMorf[0] = new mDoExt_McaMorfSO(modelData, NULL, NULL, NULL, -1, 1.0f, 0, -1, &mSound, 0x80000,
                                     0x11020084);

    if (mpMorf[0] != NULL && mpMorf[0]->getModel() == NULL) {
        mpMorf[0]->stopZelAnime();
        mpMorf[0] = NULL;
    }

    if (mpMorf[0] == NULL) {
        return 0;
    }

    J3DModel* model = mpMorf[0]->getModel();
    model->setUserArea((uintptr_t)this);

    if (setFaceMotionAnm(0, false) && setMotionAnm(0, 0.0f, 0)) {
        return 1;
    }
    return 0;
}

int daNpc_Knj_c::Delete() {
    fopAcM_GetID(this);
    this->~daNpc_Knj_c();
    return 1;
}

int daNpc_Knj_c::Execute() {
    return daNpcT_c::execute();
}

int daNpc_Knj_c::Draw() {
    if (mpMatAnm[0] != NULL) {
        J3DModelData* modelData = mpMorf[0]->getModel()->getModelData();
        modelData->getMaterialNodePointer(getEyeballMaterialNo())->setMaterialAnm(mpMatAnm[0]);
    }
    return daNpcT_c::draw(0, 0, mRealShadowSize, NULL, 0.0f, 1, 0, 0);
}

int daNpc_Knj_c::createHeapCallBack(fopAc_ac_c* i_this) {
    return static_cast<daNpc_Knj_c*>(i_this)->CreateHeap();
}

u8 daNpc_Knj_c::getType() {
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
    }
    return TYPE_0;
}

int daNpc_Knj_c::getFlowNodeNo() {
    u16 nodeNo = home.angle.x;
    if (nodeNo == 0xffff) {
        return -1;
    }
    return nodeNo;
}

BOOL daNpc_Knj_c::isDelete() {
    return FALSE;
}

void daNpc_Knj_c::reset() {
    initialize();

    memset(&mNextAction, 0, (u8*)&field_0xe64 - (u8*)&mNextAction);

    if (mpMatAnm[0] != NULL) {
        mpMatAnm[0]->initialize();
    }

    setAngle(home.angle.y);
}

void daNpc_Knj_c::setParam() {
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
    attention_info.flags = fopAc_AttnFlag_SPEAK_e | fopAc_AttnFlag_TALK_e;

    scale.set(mpHIO->m.common.scale, mpHIO->m.common.scale, mpHIO->m.common.scale);
    mCcStts.SetWeight(mpHIO->m.common.weight);
    mCylH = mpHIO->m.common.height;
    mWallR = mpHIO->m.common.width;
    mAcchCir.SetWallR(mWallR);
    mAcchCir.SetWallH(mpHIO->m.common.knee_length);
    mRealShadowSize = mpHIO->m.common.real_shadow_size;
    gravity = mpHIO->m.common.gravity;
    mExpressionMorfFrame = mpHIO->m.common.expression_morf_frame;
    mMorfFrames = mpHIO->m.common.morf_frame;
}

void daNpc_Knj_c::srchActors() {}

BOOL daNpc_Knj_c::evtTalk() {
    if (chkAction(&daNpc_Knj_c::talk)) {
        (this->*mAction)(NULL);
    } else {
        mPreItemNo = 0;
        if (dComIfGp_event_chkTalkXY()) {
            if (!dComIfGp_evmng_ChkPresentEnd()) {
                return TRUE;
            }
            mEvtNo = 1;
            evtChange();
            return TRUE;
        }
        setAction(&daNpc_Knj_c::talk);
    }
    return TRUE;
}

BOOL daNpc_Knj_c::evtCutProc() {
    BOOL rv = FALSE;

    int staffId = dComIfGp_getEventManager().getMyStaffId("Knj", this, -1);
    if (staffId != -1) {
        mStaffId = staffId;
        int actIdx =
            dComIfGp_getEventManager().getMyActIdx(mStaffId, (char**)mCutNameList, 1, 0, 0);
        if ((this->*(mCutList[actIdx]))(mStaffId) != 0) {
            dComIfGp_getEventManager().cutEnd(mStaffId);
        }
        rv = TRUE;
    }

    return rv;
}

void daNpc_Knj_c::action() {
    if (mNextAction != NULL) {
        if (mAction == mNextAction) {
            (this->*mAction)(NULL);
        } else {
            setAction(mNextAction);
        }
    }
}

void daNpc_Knj_c::beforeMove() {
    if (checkHide() || mNoDraw != 0) {
        attention_info.flags = 0;
    }
}

void daNpc_Knj_c::setAttnPos() {
    setMtx();
    eyePos = current.pos;
    attention_info.position = current.pos;
}

BOOL daNpc_Knj_c::drawDbgInfo() {
    return FALSE;
}

bool daNpc_Knj_c::afterSetMotionAnm(int param_1, int param_2, f32 param_3, int param_4) {
    static struct {
        int field_0x0;
        u32 field_0x4;
    } btpAnmData[6] = {
        {15, 1}, {16, 1}, {17, 1}, {18, 1}, {19, 1}, {20, 1},
    };

    static struct {
        int field_0x0;
        u32 field_0x4;
    } brkAnmData[1] = {
        {12, 1},
    };

    J3DAnmTexPattern* anm_text = NULL;
    J3DAnmTevRegKey* anm_tev = NULL;
    if (btpAnmData[mType].field_0x0 != -1) {
        anm_text =
            getTexPtrnAnmP(l_resNameList[btpAnmData[mType].field_0x4], btpAnmData[mType].field_0x0);
    }

    if (anm_text) {
        if (mBtpAnm.getBtpAnm() == anm_text) {
            mAnmFlags |= 0x800;
        } else if (setBtpAnm(anm_text, mpMorf[0]->getModel()->getModelData(), 1.0f, param_2)) {
            mAnmFlags |= 0x820;
        }
    }

    if (anm_text == NULL && btpAnmData[mType].field_0x0 != -1) {
        return 0;
    }

    if (brkAnmData[param_1].field_0x0 != -1) {
        anm_tev = getTevRegKeyAnmP(l_resNameList[brkAnmData[param_1].field_0x4],
                                   brkAnmData[param_1].field_0x0);
    }

    if (anm_tev) {
        if (mBrkAnm.getBrkAnm() == anm_tev) {
            mAnmFlags |= 0x100;
        } else if (setBrkAnm(anm_tev, mpMorf[0]->getModel()->getModelData(), 1.0f, param_2)) {
            mBrkAnm.setFrame(mType * 10);
            mAnmFlags |= 0x104;
        }
    }

    if (anm_tev == NULL && brkAnmData[param_1].field_0x0 != -1) {
        return 0;
    }

    return 1;
}

void daNpc_Knj_c::drawGhost() {
    J3DModel* model = mpMorf[0]->getModel();
    g_env_light.settingTevStruct(5, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(model, &tevStr);
    mpMorf[0]->entryDL();
}

int daNpc_Knj_c::selectAction() {
    mNextAction = NULL;
    mNextAction = &daNpc_Knj_c::wait;
    return 1;
}

BOOL daNpc_Knj_c::chkAction(actionFunc param_0) {
    return mAction == param_0;
}

int daNpc_Knj_c::setAction(actionFunc param_0) {
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

int daNpc_Knj_c::wait(void* param_0) {
    BOOL uVar2;

    switch (mMode) {
    case 0:
    case 1:
        mFaceMotionSeqMngr.setNo(FACE_MOT_UNK_0, -1.0f, 0, 0);
        mMotionSeqMngr.setNo(MOT_UNK_0, -1.0f, 0, 0);
        mMode = 2;
    case 2:
        if (!mStagger.checkStagger()) {
            if (srchPlayerActor()) {
                mJntAnm.lookPlayer(0);
                uVar2 = checkStep();
            } else {
                mJntAnm.lookNone(0);
                uVar2 = home.angle.y != mCurAngle.y;
            }
            if (uVar2 != 0 && step(home.angle.y, -1, -1, 15, 0)) {
                mMode = 1;
            }
        }
        break;
    case 3:
        break;
    }

    return 1;
}

int daNpc_Knj_c::talk(void* param_0) {
    BOOL bVar1 = FALSE;

    switch (mMode) {
    case 0:
    case 1:
        initTalk(mFlowNodeNo, NULL);
        mMode = 2;
    case 2:
        if (mTwilight) {
            bVar1 = TRUE;
        } else {
            mJntAnm.lookPlayer(0);
            if (mCurAngle.y != fopAcM_searchPlayerAngleY(this)) {
                step(fopAcM_searchPlayerAngleY(this), -1, -1, 15, 0);
            } else {
                bVar1 = TRUE;
            }
        }
        if (bVar1 && talkProc(NULL, FALSE, NULL, FALSE)) {
            mPlayerActorMngr.entry(daPy_getPlayerActorClass());
            dComIfGp_event_reset();
            mMode = 3;
        }
        break;
    case 3:
        break;
    }

    return 0;
}

static int daNpc_Knj_Create(void* i_this) {
    return static_cast<daNpc_Knj_c*>(i_this)->create();
}

static int daNpc_Knj_Delete(void* i_this) {
    return static_cast<daNpc_Knj_c*>(i_this)->Delete();
}

static int daNpc_Knj_Execute(void* i_this) {
    return static_cast<daNpc_Knj_c*>(i_this)->Execute();
}

static int daNpc_Knj_Draw(void* i_this) {
    return static_cast<daNpc_Knj_c*>(i_this)->Draw();
}

static int daNpc_Knj_IsDelete(void* i_this) {
    return 1;
}

static actor_method_class daNpc_Knj_MethodTable = {
    (process_method_func)daNpc_Knj_Create,  (process_method_func)daNpc_Knj_Delete,
    (process_method_func)daNpc_Knj_Execute, (process_method_func)daNpc_Knj_IsDelete,
    (process_method_func)daNpc_Knj_Draw,
};

actor_process_profile_definition g_profile_NPC_KNJ = {
    fpcLy_CURRENT_e,         // mLayerID
    7,                       // mListID
    fpcPi_CURRENT_e,         // mListPrio
    PROC_NPC_KNJ,            // mProcName
    &g_fpcLf_Method.base,    // sub_method
    sizeof(daNpc_Knj_c),     // mSize
    0,                       // mSizeOther
    0,                       // mParameters
    &g_fopAc_Method.base,    // sub_method
    349,                     // mPriority
    &daNpc_Knj_MethodTable,  // sub_method
    0x00044000,              // mStatus
    fopAc_ACTOR_e,           // mActorType
    fopAc_CULLBOX_CUSTOM_e,  // cullType
};
