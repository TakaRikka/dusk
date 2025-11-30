/**
 * @file d_a_npc_seirei.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_npc_seirei.h"
#include "d/d_meter2_info.h"

enum Seirei_RES_File_ID {
    /* BCK */
    /* 0x06 */ BCK_SEIA_WAIT_A = 0x6,
    /* 0x07 */ BCK_SEIA_WAIT_B,

    /* BMDE */
    /* 0x0A */ BMDE_SEIA = 0xA,

    /* BRK */
    /* 0x0D */ BRK_SEIA = 0xD,

    /* BTK */
    /* 0x10 */ BTK_SEIA = 0x10,
};

enum Seirei1_RES_File_ID {
    /* EVT */
    /* 0x3 */ EVT_SEIREI1_EVENT_LIST = 0x3,
};

enum RES_Name {
    /* 0x0 */ NONE,
    /* 0x1 */ SEIREI,
    /* 0x2 */ SEIREI1,  
};

enum Joint {
    /* 0x00 */ JNT_CENTER,
    /* 0x01 */ JNT_BACKBONE_1,
    /* 0x02 */ JNT_BACKBONE_2,
    /* 0x03 */ JNT_FL_1,
    /* 0x04 */ JNT_FL_2,
    /* 0x05 */ JNT_FL_FOOT_1,
    /* 0x06 */ JNT_FL_FOOT_2,
    /* 0x07 */ JNT_FR_1,
    /* 0x08 */ JNT_FR_2,
    /* 0x09 */ JNT_FR_FOOT_1,
    /* 0x0A */ JNT_FR_FOOT_2,
    /* 0x0B */ JNT_NECK,
    /* 0x0C */ JNT_HEAD,
    /* 0x0D */ JNT_CHIN_1,
    /* 0x0E */ JNT_SIZUKU,
    /* 0x0F */ JNT_WAIST,
    /* 0x10 */ JNT_BL_1,
    /* 0x11 */ JNT_BL_2,
    /* 0x12 */ JNT_BL_3,
    /* 0x13 */ JNT_BL_FOOT,
    /* 0x14 */ JNT_BR_1,
    /* 0x15 */ JNT_BR_2,
    /* 0x16 */ JNT_BR_3,
    /* 0x17 */ JNT_BR_FOOT,
    /* 0x18 */ JNT_TAIL_1,
    /* 0x19 */ JNT_TAIL_2,
    /* 0x1A */ JNT_TAIL_3,
    /* 0x1B */ JNT_TAIL_4,
};

enum Event {
    /* 0x0 */ EVT_NONE,
    /* 0x1 */ EVT_CONVERSATION_ABOUT_YM,
    /* 0x2 */ EVT_DEFAULT_GETITEM,
    /* 0x3 */ EVT_NO_RESPONSE,
};

enum Face_Motion {
    /* 0x0 */ FACE_MOT_NONE,
};

enum Motion {
    /* 0x0 */ MOT_WAIT_A,
    /* 0x1 */ MOT_WAIT_B,
    /* 0x2 */ MOT_NONE,
};

enum Type {
    /* 0x0 */ TYPE_0,
    /* 0x1 */ TYPE_1,
    /* 0x2 */ TYPE_2,
    /* 0x3 */ TYPE_3,
};

static int l_bmdData[1][2] = {
    {BMDE_SEIA, SEIREI},
};

static daNpcT_evtData_c l_evtList[4] = {
    {"", 0},
    {"CONVERSATION_ABOUT_YM", 2},
    {"DEFAULT_GETITEM", 0},
    {"NO_RESPONSE", 0},
};

static char* l_resNameList[3] = {
    "",
    "Seirei",
    "Seirei1",
};

static s8 l_loadResPtrn0[2] = {
    SEIREI, -1,
};

static s8 l_loadResPtrn1[2] = {
    SEIREI1, -1,
};

 static s8* l_loadResPtrnList[4] = {
    l_loadResPtrn1,
    l_loadResPtrn1,
    l_loadResPtrn1,
    l_loadResPtrn0,
};

static daNpcT_faceMotionAnmData_c l_faceMotionAnmData[1] = {
    {-1, J3DFrameCtrl::EMode_NONE, NONE, -1, J3DFrameCtrl::EMode_NONE, NONE, 0},
};

static daNpcT_motionAnmData_c l_motionAnmData[2] = {
    {BCK_SEIA_WAIT_A, J3DFrameCtrl::EMode_LOOP, SEIREI, -1, J3DFrameCtrl::EMode_NONE, NONE, 0, 0},
    {BCK_SEIA_WAIT_B, J3DFrameCtrl::EMode_NONE, SEIREI, -1, J3DFrameCtrl::EMode_NONE, NONE, 0, 0},
};

static daNpcT_MotionSeqMngr_c::sequenceStepData_c l_faceMotionSequenceData[4] = {
    {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
};

static daNpcT_MotionSeqMngr_c::sequenceStepData_c l_motionSequenceData[12] = {
    {0, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {1, 0, 1}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
};

char* daNpc_Seirei_c::mCutNameList[2] = {
    "",
    "CONVERSATION",
};

daNpc_Seirei_c::cutFunc daNpc_Seirei_c::mCutList[2] = {
    NULL,
    &daNpc_Seirei_c::cutConversation,
};

daNpc_Seirei_c::~daNpc_Seirei_c() {
    if (mpMorf[0] != NULL) {
        mpMorf[0]->stopZelAnime();
    }

    deleteRes(l_loadResPtrnList[mType], (const char**)l_resNameList);
}

daNpc_Seirei_HIOParam const daNpc_Seirei_Param_c::m = {
    600.0f,
    0.0f,
    1.0f,
    4000.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0,
    0,
    0,
    0,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0,
    0,
    0,
    0,
    0,
    false,
    false,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    1200.0f,
};

cPhs__Step daNpc_Seirei_c::create() {
    daNpcT_ct(this, daNpc_Seirei_c, l_faceMotionAnmData, l_motionAnmData, l_faceMotionSequenceData, 4,
                       l_motionSequenceData, 4, l_evtList, l_resNameList);

    mType = getType();
    mFlowNodeNo = getFlowNodeNo();
    mTwilight = false;
    arg0 = fopAcM_GetParam(this) >> 28;

    cPhs__Step phase = (cPhs__Step)loadRes(l_loadResPtrnList[mType], (const char**)l_resNameList);
    if (phase == cPhs_COMPLEATE_e) {
        if (arg0 != 0 && !fopAcM_entrySolidHeap(this, createHeapCallBack, 0)) {
            return cPhs_ERROR_e;
        }

        OS_REPORT("\t(%s:%d) flowNo:%d, SWBit:%02x, DoBtnToChk:%d<%08x> ", fopAcM_getProcNameString(this), mType, mFlowNodeNo, (getBitSW() >> 32) & 0xFF,
                  getDoBtnChkFlag() & 0xFF, fopAcM_GetParam(this));

        if (isDelete()) {
            OS_REPORT("===>isDelete:TRUE\n");
            return cPhs_ERROR_e;
        }

        OS_REPORT("\n");

        if (arg0 != 0) {
            mpMorf[0]->getModel()->getModelData();
            fopAcM_SetMtx(this, mpMorf[0]->getModel()->getBaseTRMtx());
            mSound.init(&current.pos, &eyePos, 3, 1);
        }

        mAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1, &mAcchCir, fopAcM_GetSpeed_p(this),
                  fopAcM_GetAngle_p(this), fopAcM_GetShapeAngle_p(this));
        mCcStts.Init(daNpc_Seirei_Param_c::m.common.weight, 0, this);
        mAcch.CrrPos(dComIfG_Bgsp());
        mGndChk = mAcch.m_gnd;
        mGroundH = mAcch.GetGroundH();

        if (mGroundH != -G_CM3D_F_INF) {
            setEnvTevColor();
            setRoomNo();
        }

        current.pos = home.pos;
        old.pos = current.pos;

        reset();
        mCreating = true;
        Execute();
        mCreating = false;
    }

    return phase;
}

int daNpc_Seirei_c::CreateHeap() {
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes(l_resNameList[l_bmdData[0][1]], l_bmdData[0][0]);
    if (modelData == NULL) {
        return 0;
    }

    mpMorf[0] = new mDoExt_McaMorfSO(modelData, NULL, NULL, NULL, -1, 1.0f, 0, -1, &mSound, 0, 0x11020284);
    if (mpMorf[0] == NULL || mpMorf[0]->getModel() == NULL) {
        return 0;
    }

    mpMorf[0]->getModel()->setUserArea((uintptr_t)this);

    if (setFaceMotionAnm(FACE_MOT_NONE, false) && setMotionAnm(MOT_WAIT_A, 0.0f, FALSE)) {
        return 1;
    }

    return 0;
}

int daNpc_Seirei_c::Delete() {
    this->~daNpc_Seirei_c();
    return 1;
}

int daNpc_Seirei_c::Execute() {
    return execute();
}

int daNpc_Seirei_c::Draw() {
    if (arg0 != 0) {
        if (mpMatAnm[0] != NULL) {
            J3DModelData* modelData = mpMorf[0]->getModel()->getModelData();
            modelData->getMaterialNodePointer(getEyeballMaterialNo())->setMaterialAnm(mpMatAnm[0]);
        }

        return draw(FALSE, TRUE, mRealShadowSize, NULL, 100.0f, FALSE, FALSE, FALSE);
    }

    return 1;
}

int daNpc_Seirei_c::createHeapCallBack(fopAc_ac_c* a_this) {
    return static_cast<daNpc_Seirei_c*>(a_this)->CreateHeap();
}

u8 daNpc_Seirei_c::getType() {
    switch (fopAcM_GetParam(this) & 0xFF) {
        case 0:
            return 0;
        
        case 1:
            return 1;

        case 2:
            return 2;

        default:
            return 3;
    }
}

BOOL daNpc_Seirei_c::isDelete() {
    return FALSE;
}

void daNpc_Seirei_c::reset() {
    int size = (u8*)&field_0xe84 - (u8*)&mNextAction;
    
    if (mpMatAnm[0] != NULL) {
        mpMatAnm[0]->initialize();
    }

    initialize();

    memset(&mNextAction, 0, size);

    switch (mType) {
        case TYPE_0:
            mItemPartnerId = fpcM_ERROR_PROCESS_ID_e;
            break;

        case TYPE_1:
        case TYPE_2:
        case TYPE_3:
            break;
    }

    if (arg0 != 0) {
        mMotionSeqMngr.setNo(MOT_WAIT_A, -1.0f, FALSE, 0);
        field_0xe60 = 3;
    } else {
        mMotionSeqMngr.setNo(MOT_NONE, -1.0f, FALSE, 0);
    }

    setAngle(home.angle.y);
}

void daNpc_Seirei_c::setParam() {
    selectAction();
    srchActors();
    dComIfGp_getAttention();

    dComIfGp_getAttention()->getDistTable(0x28).mDistMax = daNpc_Seirei_Param_c::m.talk_dist;
    dComIfGp_getAttention()->getDistTable(0x28).mDistMaxRelease = daNpc_Seirei_Param_c::m.talk_dist;
    dComIfGp_getAttention()->getDistTable(0x27).mDistMax = daNpc_Seirei_Param_c::m.talk_dist;
    dComIfGp_getAttention()->getDistTable(0x27).mDistMaxRelease = daNpc_Seirei_Param_c::m.talk_dist;

    attention_info.distances[fopAc_attn_LOCK_e] = 39;
    attention_info.distances[fopAc_attn_TALK_e] = 39;
    attention_info.distances[fopAc_attn_SPEAK_e] = 39;

    if (getBitSW() != 0xFF && dComIfGs_isSwitch(getBitSW(), fopAcM_GetRoomNo(this))) {
        attention_info.flags = fopAc_AttnFlag_SPEAK_e;
    } else {
        attention_info.flags = 0;
    }

    mCcStts.SetWeight(daNpc_Seirei_Param_c::m.common.weight);
    mCylH = daNpc_Seirei_Param_c::m.common.height;
    mWallR = daNpc_Seirei_Param_c::m.common.width;
    mAttnFovY = daNpc_Seirei_Param_c::m.common.fov;
    mAcchCir.SetWallR(mWallR);
    mAcchCir.SetWallH(daNpc_Seirei_Param_c::m.common.knee_length);
    mRealShadowSize = daNpc_Seirei_Param_c::m.common.real_shadow_size;
    mExpressionMorfFrame = daNpc_Seirei_Param_c::m.common.expression_morf_frame;
    mMorfFrames = daNpc_Seirei_Param_c::m.common.morf_frame;
    gravity = daNpc_Seirei_Param_c::m.common.gravity;
}

BOOL daNpc_Seirei_c::checkChangeEvt() {
    return FALSE;
}

void daNpc_Seirei_c::srchActors() {
    /* empty function */
}

BOOL daNpc_Seirei_c::evtTalk() {
    if (chkAction(&daNpc_Seirei_c::talk)) {
        (this->*mAction)(NULL);
    } else {
        mPreItemNo = 0;

        if (dComIfGp_event_chkTalkXY()) {
            if(!dComIfGp_evmng_ChkPresentEnd()) {
                return TRUE;
            }

            mEvtNo = EVT_NO_RESPONSE;
            evtChange();
            return TRUE;
        }

        setAction(&daNpc_Seirei_c::talk);
    }

    return TRUE;
}

BOOL daNpc_Seirei_c::evtCutProc() {
    s32 staffId = dComIfGp_getEventManager().getMyStaffId("Seirei", this, -1);

    if (staffId != -1) {
        mStaffId = staffId;
        int actIdx = dComIfGp_getEventManager().getMyActIdx(mStaffId, mCutNameList, 2, 0, 0);
        
        if ((this->*mCutList[actIdx])(mStaffId)) {
            dComIfGp_getEventManager().cutEnd(mStaffId);
        }

        return TRUE;
    }

    return FALSE;
}

void daNpc_Seirei_c::action() {
    if (mNextAction) {
        if (mAction == mNextAction) {
            (this->*mAction)(NULL);
        } else {
            setAction(mNextAction);
        }
    }
}

void daNpc_Seirei_c::beforeMove() {
    if (checkHide() || mNoDraw) {
        attention_info.flags = 0;
    }

    if (arg0 != 0) {
        ctrlWaitAnm();
    }
}

void daNpc_Seirei_c::setAttnPos() {
    cXyz sp18;

    if (arg0 != 0) {
        setMtx();
        sp18.set(300.0f, 40.0f, 0.0f);
        mDoMtx_stack_c::copy(mpMorf[0]->getModel()->getAnmMtx(JNT_HEAD));
        mDoMtx_stack_c::multVec(&sp18, &eyePos);
        sp18.set(0.0f, daNpc_Seirei_Param_c::m.common.attention_offset, 800.0f);
        mDoMtx_stack_c::YrotS(mCurAngle.y);
        mDoMtx_stack_c::multVec(&sp18, &attention_info.position);
        attention_info.position += current.pos;
    } else {
        attention_info.position = current.pos;
        attention_info.position.y += daNpc_Seirei_Param_c::m.common.attention_offset - 350.0f;
        eyePos = attention_info.position;
    }

    setPrtcls();
}

int daNpc_Seirei_c::drawDbgInfo() {
    return 0;
}

bool daNpc_Seirei_c::afterSetMotionAnm(int i_index, int i_mode, f32 param_3, int param_4) {
    static struct {
        int fileIdx;
        int arcIdx;
    } btkAnmData[2] = {
        {BTK_SEIA, SEIREI},
        {BTK_SEIA, SEIREI},
    };

    static struct {
        int fileIdx;
        int arcIdx;
    } brkAnmData[2] = {
        {BRK_SEIA, SEIREI},
        {BRK_SEIA, SEIREI},
    };

    J3DAnmTextureSRTKey* i_btk = NULL;
    J3DAnmTevRegKey* i_brk = NULL;

    if (btkAnmData[i_index].fileIdx != -1) {
        i_btk = getTexSRTKeyAnmP(l_resNameList[btkAnmData[i_index].arcIdx], btkAnmData[i_index].fileIdx);
    }

    if (i_btk != NULL) {
        if (mBtkAnm.getBtkAnm() == i_btk) {
            mAnmFlags |= ANM_PLAY_BTK;
        } else {
            if (setBtkAnm(i_btk, mpMorf[0]->getModel()->getModelData(), 1.0f, i_mode)) {
                mAnmFlags |= ANM_PLAY_BTK | ANM_PAUSE_BTK;
            }
        }
    }

    if (i_btk == NULL && btkAnmData[i_index].fileIdx != -1) {
        return false;
    }

    if (brkAnmData[i_index].fileIdx != -1) {
        i_brk = getTevRegKeyAnmP(l_resNameList[brkAnmData[i_index].arcIdx], brkAnmData[i_index].fileIdx);
    }

    if (i_brk != NULL) {
        if (mBrkAnm.getBrkAnm() == i_brk) {
            mAnmFlags |= ANM_PLAY_BRK;
        } else {
            if (setBrkAnm(i_brk, mpMorf[0]->getModel()->getModelData(), 1.0f, i_mode)) {
                mAnmFlags |= ANM_PLAY_BRK | ANM_PAUSE_BRK;
            }
        }
    }

    if (i_brk == NULL && brkAnmData[i_index].fileIdx != -1) {
        return false;
    }

    return true;
}

BOOL daNpc_Seirei_c::selectAction() {
    mNextAction = NULL;
    mNextAction = &daNpc_Seirei_c::wait;
    return TRUE;
}

BOOL daNpc_Seirei_c::chkAction(actionFunc action) {
    return mAction == action;
}

BOOL daNpc_Seirei_c::setAction(actionFunc action) {
    mMode = MODE_EXIT;

    if (mAction) {
        (this->*mAction)(NULL);
    }

    mMode = MODE_ENTER;
    mAction = action;

    if (mAction) {
        (this->*mAction)(NULL);
    }

    return TRUE;
}

void daNpc_Seirei_c::ctrlWaitAnm() {
    if (field_0xe60 != 0 && mMotionSeqMngr.getNo() == MOT_WAIT_A && field_0xe60 <= mMorfLoops) {
        field_0xe60 = 0;
    }

    if (field_0xe60 == 0) {
        if (mMotionSeqMngr.getNo() == MOT_WAIT_B) {
            if (mMotionSeqMngr.checkEndSequence()) {
                mMotionSeqMngr.setNo(MOT_WAIT_A, -1.0f, FALSE, 0);
                field_0xe60 = 3;
            }
        } else {
            mMotionSeqMngr.setNo(MOT_WAIT_B, -1.0f, FALSE, 0);
        }
    }
}

static u16 const id1[7] = {
    dPa_RM(ID_ZI_S_SEIA_SHIZUKU_A),
    dPa_RM(ID_ZI_S_SEIA_SHIZUKU_B),
    dPa_RM(ID_ZI_S_SEIA_SHIZUKU_C),
    dPa_RM(ID_ZI_S_SEIA_SHIZUKU_D),
    dPa_RM(ID_ZI_S_SEIA_SHIZUKU_E),
    dPa_RM(ID_ZI_S_SEIA_SHIZUKU_F),
    dPa_RM(ID_ZI_S_SEIA_SHIZUKU_G),
};

static u16 const id2[7] = {
    dPa_RM(ID_ZF_S_SEIREI00_SPRIT),
    dPa_RM(ID_ZF_S_SEIREI01_CIR),
    0xFFFF,
    0xFFFF,
    0xFFFF,
    0xFFFF,
    0xFFFF,
};

void daNpc_Seirei_c::setPrtcls() {
    cXyz i_pos;
    cXyz i_scale(1.0f, 1.0f, 1.0f);

    int i = 0;
    int j = 0;
    for (; i < 7; i++, j++) {
        if (arg0 != 0) {
            mDoMtx_stack_c::copy(mpMorf[0]->getModel()->getAnmMtx(JNT_SIZUKU));
            mDoMtx_stack_c::multVecZero(&i_pos);
            mPrtcls[j] = dComIfGp_particle_set(mPrtcls[j], id1[i], &i_pos, NULL, &i_scale);
        } else if (getBitSW() != 0xFF) {
            if (dComIfGs_isSwitch(getBitSW(), fopAcM_GetRoomNo(this)) && id2[i] != 0xFFFF) {
                i_pos = attention_info.position;
                mPrtcls[j] = dComIfGp_particle_set(mPrtcls[j], id2[i], &i_pos, &mCurAngle, &i_scale);
                mDoAud_seStartLevel(Z2SE_FAIRY_STAY, &i_pos, 0, 0);
            }
        }

        JPABaseEmitter* emitter = dComIfGp_particle_getEmitter(mPrtcls[j]);
        if (emitter != NULL) {
            emitter->setGlobalTranslation(i_pos.x, i_pos.y, i_pos.z);
        }
    }
}

BOOL daNpc_Seirei_c::chkFirstMeeting() {
    switch (mType) {
        case TYPE_0:
            return daNpcT_chkEvtBit(153) == FALSE; // dSv_event_flag_c::F_0055 - Faron Woods - Received Vessel of Light from Faron spirit
        
        case TYPE_1:
            return daNpcT_chkEvtBit(221) == FALSE; // dSv_event_flag_c::F_0221 - Kakariko Village - Received vessel of light from spirit

        case TYPE_2:
            return daNpcT_chkEvtBit(67) == FALSE; // dSv_event_flag_c::M_034 - Cutscene - [cutscene: 19] Reunion with Ilia (After Lakebed Temple?)
    }

    return FALSE;
}

int daNpc_Seirei_c::cutConversation(int i_staffId) {
    BOOL rv = FALSE;
    int prm = -1;
    int timer = 0;

    int* piVar1 = dComIfGp_evmng_getMyIntegerP(i_staffId, "prm");
    if (piVar1 != NULL) {
        prm = *piVar1;
    }

    piVar1 = dComIfGp_evmng_getMyIntegerP(i_staffId, "timer");
    if (piVar1 != NULL) {
        timer = *piVar1;
    }

    if (dComIfGp_getEventManager().getIsAddvance(i_staffId)) {
        switch (prm) {
            case 0:
                daNpcT_onTmpBit(11);
                dMeter2Info_setPauseStatus(3);
                mEventTimer = timer;
                break;
            
            case 1:
                initTalk(mFlowNodeNo, NULL);
                break;
        }
    }

    switch (prm) {
        case 0:
            if (mEventTimer != 0) {
                if (cLib_calcTimer(&mEventTimer) == 0) {
                    field_0xe80 = false;
                }
            } else {
                if (dComIfGp_getEventManager().getIsAddvance(i_staffId)) {
                    field_0xe80 = false;
                }
            }

            rv = field_0xe80 == false;
            break;
        
        case 1:
            field_0xe26 = false;

            if (talkProc(NULL, FALSE, NULL, FALSE) && mFlow.checkEndFlow()) {
                rv = 1;
            }
            break;
    }

    return rv;
}

int daNpc_Seirei_c::wait(void* param_1) {
    switch (mMode) {
        case MODE_ENTER:
        case MODE_INIT:
            mMode = MODE_RUN;
            // fallthrough
        case MODE_RUN:
            if (field_0xe80) {
                if (mType == TYPE_0) {
                    mEvtNo = EVT_CONVERSATION_ABOUT_YM;
                    field_0xe33 = true;
                } else {
                    mSpeakEvent = true;
                    field_0xe33 = true;
                }
            }

            if (arg0 == 0 && chkFirstMeeting() && getDoBtnChkFlag() && daNpcT_chkDoBtnIsSpeak(this)) {
                dComIfGp_setDoStatus(8, 0);
            }
            break;

        case MODE_EXIT:
            break;
    }

    return 1;
}

int daNpc_Seirei_c::talk(void* param_1) {
    switch (mMode) {
        case MODE_ENTER:
        case MODE_INIT:
            initTalk(mFlowNodeNo, NULL);
            mMode = MODE_RUN;
            // fallthrough
        case MODE_RUN:
            if (talkProc(NULL, FALSE, NULL, FALSE)) {
                int i_itemNo;
                if ((mFlow.getEventId(&i_itemNo) & 0xFFFF) == 1) {
                    if (mItemPartnerId == fpcM_ERROR_PROCESS_ID_e) {
                        mItemPartnerId = fopAcM_createItemForPresentDemo(&current.pos, i_itemNo, 0, -1, -1, NULL, NULL);
                    }

                    if (fopAcM_IsExecuting(mItemPartnerId)) {
                        field_0xe80 = true;
                        mEvtNo = EVT_DEFAULT_GETITEM;
                        evtChange();
                    }
                } else if (mFlow.checkEndFlow()) {
                    dComIfGp_event_reset();
                    field_0xe80 = false;
                    mMode = MODE_EXIT;
                }
            }

            field_0xe26 = false;
            break;

        case MODE_EXIT:
            break;
    }

    return 0;
}

static int daNpc_Seirei_Create(void* a_this) {
    return static_cast<daNpc_Seirei_c*>(a_this)->create();
}

static int daNpc_Seirei_Delete(void* a_this) {
    return static_cast<daNpc_Seirei_c*>(a_this)->Delete();
}

static int daNpc_Seirei_Execute(void* a_this) {
    return static_cast<daNpc_Seirei_c*>(a_this)->Execute();
}

static int daNpc_Seirei_Draw(void* a_this) {
    return static_cast<daNpc_Seirei_c*>(a_this)->Draw();
}

static int daNpc_Seirei_IsDelete(void* a_this) {
    return 1;
}

static daNpc_Seirei_Param_c l_HIO;

static actor_method_class daNpc_Seirei_MethodTable = {
    (process_method_func)daNpc_Seirei_Create,
    (process_method_func)daNpc_Seirei_Delete,
    (process_method_func)daNpc_Seirei_Execute,
    (process_method_func)daNpc_Seirei_IsDelete,
    (process_method_func)daNpc_Seirei_Draw,
};

extern actor_process_profile_definition g_profile_NPC_SEIREI = {
  fpcLy_CURRENT_e,           // mLayerID
  7,                         // mListID
  fpcPi_CURRENT_e,           // mListPrio
  PROC_NPC_SEIREI,           // mProcName
  &g_fpcLf_Method.base,     // sub_method
  sizeof(daNpc_Seirei_c),    // mSize
  0,                         // mSizeOther
  0,                         // mParameters
  &g_fopAc_Method.base,      // sub_method
  363,                       // mPriority
  &daNpc_Seirei_MethodTable, // sub_method
  0x00044000,                // mStatus
  fopAc_ACTOR_e,             // mActorType
  fopAc_CULLBOX_CUSTOM_e,    // cullType
};
