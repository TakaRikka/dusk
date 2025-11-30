/**
 * @file d_a_npc_zant.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_npc_zant.h"
#include "SSystem/SComponent/c_counter.h"
#include "f_op/f_op_actor_mng.h"
#include "SSystem/SComponent/c_counter.h"

static int l_bmdData[2] = {
    7, 1
};

static daNpcT_evtData_c l_evtList[2] = {
    {"", 0},
    {"NO_RESPONSE", 0}
};

static char* l_resNameList[2] = {
    "",
    "Zant",
};

static s8 l_loadResPtrn0[2] = {
    1, -1
};

static s8* l_loadResPtrnList[2] = {
    l_loadResPtrn0, l_loadResPtrn0,
};

static daNpcT_faceMotionAnmData_c l_faceMotionAnmData = {
    -1, 0, 0, -1, 0, 0, 0
};

static daNpcT_motionAnmData_c l_motionAnmData = {
    4, 2, 1, -1, 0, 0, 0
};

static daNpcT_MotionSeqMngr_c::sequenceStepData_c l_faceMotionSequenceData[4] = {
    {0, -1, 0}, {-1, 0, 0}, 
    {-1, 0, 0}, {-1, 0, 0}
};

static daNpcT_MotionSeqMngr_c::sequenceStepData_c l_motionSequenceData[4] = {
    {0, -1, 0}, {-1, 0, 0}, 
    {-1, 0, 0}, {-1, 0, 0}
};

char* daNpc_Zant_c::mCutNameList = "";

daNpc_Zant_c::cutFunc daNpc_Zant_c::mCutList[1] = {
    NULL
};

static daNpc_Zant_Param_c l_HIO;

daNpc_Zant_c::~daNpc_Zant_c() {
    OS_REPORT("|%06d:%x|daNpc_Zant_c -> デストラクト\n", g_Counter.mCounter0, this);
    if (heap != NULL) {
        mpMorf[0]->stopZelAnime();
    }

    deleteRes(l_loadResPtrnList[mType], (const char **)l_resNameList);
}

daNpc_Zant_Param_c::Data const daNpc_Zant_Param_c::m  = {
    270.0f, -3.0f, 1.0f, 500.0f, 255.0f, 250.0f, 35.0f, 50.0f,
    0.0f, 0.0f, 10.0f, -10.0f, 30.0f, -10.0f, 45.0f, -45.0f,
    0.6f, 12.0f, 3, 6, 5, 6, 110.0f, 0.0f, 0.0f, 0.0f, 60, 8,
    0, 0, 4.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};

int daNpc_Zant_c::create() {
    int rv;

    daNpcT_ct(this, daNpc_Zant_c, &l_faceMotionAnmData, &l_motionAnmData, l_faceMotionSequenceData, 
        4, l_motionSequenceData, 4, l_evtList, l_resNameList);
        
    mType = getType();
    mFlowNodeNo = getFlowNodeNo();
    mTwilight = false;

    rv = loadRes(l_loadResPtrnList[mType], (const char**)l_resNameList);
    if (rv == cPhs_COMPLEATE_e) {
        if (!fopAcM_entrySolidHeap(this, createHeapCallBack, 0)) {
            return cPhs_ERROR_e;
        } else if (isDelete()) {
            return cPhs_ERROR_e;
        } else {
            fopAcM_SetMtx(this, mpMorf[0]->getModel()->getBaseTRMtx());
            fopAcM_setCullSizeBox(this, -300.0f, -50.0f, -300.0f, 300.0f, 450.0f, 300.0f);
            fopAcM_OnStatus(this, 0x8000000);

            mSound.init(&current.pos, &eyePos, 3, 1);
            mAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1, &mAcchCir, &speed, &current.angle, &shape_angle);
            mCcStts.Init(daNpc_Zant_Param_c::m.field_0x10, 0, this);
            
            mCyl.Set(mCcDCyl);
            mCyl.SetStts(&mCcStts);
            mCyl.SetTgHitCallback(tgHitCallBack);

            mAcch.CrrPos(dComIfG_Bgsp());
            mGndChk = mAcch.m_gnd;
            mGroundH = mAcch.m_ground_h;
            if (mGroundH != -G_CM3D_F_INF) {
                setEnvTevColor();
                setRoomNo();
            }
            
            reset();
            mCreating = true;
            Execute();
            mCreating = false;
        }
    }

    return rv;
}

int daNpc_Zant_c::CreateHeap() {
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes(l_resNameList[l_bmdData[1]], l_bmdData[0]);

    mpMorf[0] = new mDoExt_McaMorfSO(modelData, NULL, NULL, NULL, -1, 1.0f, 0, -1, &mSound,
        0x80000, 0x11020284);        
    if (mpMorf[0] != NULL && mpMorf[0]->getModel() == NULL) {
        mpMorf[0]->stopZelAnime();
        mpMorf[0] = NULL;
    }
    if (mpMorf[0] == NULL) {
        return 0;
    }
    
    J3DModel* model = mpMorf[0]->getModel();
    for (u16 i = 0; i < modelData->getJointNum(); i++) {
        modelData->getJointNodePointer(i)->setCallBack(ctrlJointCallBack);
    }

    model->setUserArea((uintptr_t)this);

    if (setFaceMotionAnm(0, false) && setMotionAnm(0, 0.0f, 0))
    {
        return 1;
    }
    return 0;
}

int daNpc_Zant_c::Delete() {
    OS_REPORT("|%06d:%x|daNpc_Zant_c -> Delete\n", g_Counter.mCounter0, this);
    fopAcM_GetID(this);
    this->~daNpc_Zant_c();
    return 1;
}

int daNpc_Zant_c::Execute() {
    return execute();
}

int daNpc_Zant_c::Draw() {
    if (mpMatAnm[0] != NULL) {
        J3DModelData* modelData = mpMorf[0]->getModel()->getModelData();
        modelData->getMaterialNodePointer(getEyeballMaterialNo())->setMaterialAnm(mpMatAnm[0]);
    }
    return daNpcT_c::draw(0, 0, mRealShadowSize, NULL, 0.0f, 1, 0, 0);
}

int daNpc_Zant_c::createHeapCallBack(fopAc_ac_c* i_this) {
    return static_cast<daNpc_Zant_c*>(i_this)->CreateHeap();
}

int daNpc_Zant_c::ctrlJointCallBack(J3DJoint* param_1, int param_2) {
    if (param_2 == 0) {
        J3DModel* model = j3dSys.getModel();
        daNpc_Zant_c* i_this = reinterpret_cast<daNpc_Zant_c*>(model->getUserArea());
        if (i_this != 0) {
            i_this->ctrlJoint(param_1, model);
        }
    }
    return 1;
}

u8 daNpc_Zant_c::getType() {
    switch (fopAcM_GetParam(this) & 0xff) {
        case 0:
            return TYPE_0;
        default:
            return TYPE_1;
    }
}

u32 daNpc_Zant_c::getFlowNodeNo() {
    u16 nodeNo = home.angle.x;
    if (nodeNo == 0xffff) {
        return -1;
    }
    return nodeNo;
}

int daNpc_Zant_c::isDelete() {
    if (mType == TYPE_1) {
        return FALSE;
    }

    switch (mType) {
        case TYPE_0:
            return FALSE;
        default:
            return TRUE;
    }
}

void daNpc_Zant_c::reset() {
    initialize();

    int iVar1 = (u8*)&field_0xf9c - (u8*)&field_0xf84;
    memset(&field_0xf84, 0, iVar1);

    if (mpMatAnm[0]) {
        mpMatAnm[0]->initialize();
    }

    setAngle(home.angle.y);
}

void daNpc_Zant_c::afterJntAnm(int param_0) {
    /* empty function */
}

void daNpc_Zant_c::setParam() {
    selectAction();
    srchActors();

    s16 sVar1 = daNpc_Zant_Param_c::m.field_0x48;
    s16 sVar2 = daNpc_Zant_Param_c::m.field_0x4a;
    attention_info.distances[0] = daNpcT_getDistTableIdx(daNpc_Zant_Param_c::m.field_0x4c, daNpc_Zant_Param_c::m.field_0x4e);
    attention_info.distances[1] = attention_info.distances[0];
    attention_info.distances[3] = daNpcT_getDistTableIdx(sVar1, sVar2);

    attention_info.flags = fopAc_AttnFlag_SPEAK_e | fopAc_AttnFlag_TALK_e;

    scale.setall(daNpc_Zant_Param_c::m.field_0x8);

    mCcStts.SetWeight(daNpc_Zant_Param_c::m.field_0x10);

    mCylH = daNpc_Zant_Param_c::m.field_0x14;
    mWallR = daNpc_Zant_Param_c::m.field_0x1c;
    mAttnFovY = daNpc_Zant_Param_c::m.field_0x50;

    mAcchCir.SetWallR(mWallR);
    mAcchCir.SetWallH(daNpc_Zant_Param_c::m.field_0x18);

    mRealShadowSize = daNpc_Zant_Param_c::m.field_0xc;
    gravity = daNpc_Zant_Param_c::m.field_0x4;
    mExpressionMorfFrame = daNpc_Zant_Param_c::m.field_0x6c;
    mMorfFrames = daNpc_Zant_Param_c::m.field_0x44;
}

void daNpc_Zant_c::setAfterTalkMotion() {
    mFaceMotionSeqMngr.getNo();
    mFaceMotionSeqMngr.setNo(0, -1.0, 0, 0);
}

void daNpc_Zant_c::srchActors() {
    /* empty function */
}

BOOL daNpc_Zant_c::evtTalk() {
    if (chkAction(&daNpc_Zant_c::talk)) {
        (this->*field_0xf90)(NULL);
    } else {
        mPreItemNo = 0;
        if (dComIfGp_event_chkTalkXY()) {
            if (!dComIfGp_evmng_ChkPresentEnd()) {
                return TRUE;
            } else {
                mEvtNo = 1;
                evtChange();
                return TRUE;
            }
        }
        
        setAction(&daNpc_Zant_c::talk);
    }

    return TRUE;
}

BOOL daNpc_Zant_c::evtCutProc() {
    int rv = 0;
    int staffId = dComIfGp_getEventManager().getMyStaffId("Zant", this, -1);
    if (staffId != -1) {
        mStaffId = staffId;
        int actIdx = dComIfGp_getEventManager().getMyActIdx(mStaffId, &mCutNameList, 1, 0, 0);

        if ((this->*(mCutList[actIdx]))(mStaffId) != 0) {
            dComIfGp_getEventManager().cutEnd(mStaffId);
        }

        rv = 1;
    }

    return rv;
}

void daNpc_Zant_c::action() {
    if (mStagger.checkRebirth()) {
        mStagger.initialize();
        mMode = 1;
    }

    if (field_0xf84 != 0) {
        if (field_0xf90 == field_0xf84) {
            (this->*(field_0xf90))(NULL);
        } else {
            setAction(field_0xf84);
        }
    }
}

void daNpc_Zant_c::beforeMove() {
    if (checkHide() || mNoDraw) {
        attention_info.flags = 0;
    }
}

void daNpc_Zant_c::setAttnPos() {
    cXyz cStack1(0.0f, 0.0f, 0.0f);
    mStagger.calc(0);

    f32 fVar1 = cM_s2rad(mCurAngle.y - field_0xd7e.y);
    J3DModel* model = mpMorf[0]->getModel();
    mJntAnm.setParam(this, model, &cStack1, getBackboneJointNo(),
        getNeckJointNo(), getHeadJointNo(), daNpc_Zant_Param_c::m.field_0x24,
        daNpc_Zant_Param_c::m.field_0x20, daNpc_Zant_Param_c::m.field_0x2c,
        daNpc_Zant_Param_c::m.field_0x28, daNpc_Zant_Param_c::m.field_0x34,
        daNpc_Zant_Param_c::m.field_0x30, daNpc_Zant_Param_c::m.field_0x3c,
        daNpc_Zant_Param_c::m.field_0x38, daNpc_Zant_Param_c::m.field_0x40,
        fVar1, NULL);
    
    mJntAnm.calcJntRad(0.2f, 1.0f, fVar1);
    setMtx();
    mJntAnm.setEyeAngleX(eyePos, 1.0f, 0);
    mJntAnm.setEyeAngleY(eyePos, mCurAngle.y, 1, 1.0f, 0);

    attention_info.position = current.pos;
    attention_info.position.y += daNpc_Zant_Param_c::m.field_0x0;
    eyePos = attention_info.position;
}

void daNpc_Zant_c::setCollision() {
    u32 tgType = 0xd8fbfdff;
    u32 tgSPrm = 0x1f;

    if (!mHide) {
        if (dComIfGp_event_runCheck()) {
            tgType = 0;
            tgSPrm = 0;
        } else {
            if (mTwilight) {
                tgType = 0;
                tgSPrm = 0;
            } else {
                if (mStagger.checkStagger()) {
                    tgType = 0;
                    tgSPrm = 0;
                }
            }
        }

        f32 height = mCylH;
        f32 radius = mWallR;
        cXyz pos = current.pos;

        mCyl.SetCoSPrm(0x79);
        mCyl.SetTgType(tgType);
        mCyl.SetTgSPrm(tgSPrm);
        mCyl.OnTgNoHitMark();
        mCyl.SetH(height);
        mCyl.SetR(radius);
        mCyl.SetC(pos);

        dComIfG_Ccsp()->Set(&mCyl);
    }

    mCyl.ClrCoHit();
    mCyl.ClrTgHit();
}

int daNpc_Zant_c::drawDbgInfo() {
    return 0;
}

void daNpc_Zant_c::drawGhost() {
    J3DModel* model = mpMorf[0]->getModel();
    g_env_light.settingTevStruct(3, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(model, &tevStr);
    mpMorf[0]->entryDL();
}

int daNpc_Zant_c::selectAction() {
    field_0xf84 = NULL;
    field_0xf84 = &daNpc_Zant_c::wait;
    return 1;
}

int daNpc_Zant_c::chkAction(int (daNpc_Zant_c::*action)(void*)) {
    return field_0xf90 == action;
}

int daNpc_Zant_c::setAction(int (daNpc_Zant_c::*action)(void*)) {
    mMode = 3;
    if (field_0xf90) {
        (this->*(field_0xf90))(NULL);
    }

    mMode = 0;
    field_0xf90 = action;
    if (field_0xf90) {
        (this->*(field_0xf90))(NULL);
    }

    return 1;
}

int daNpc_Zant_c::wait(void* param_1) {
    int rv;
    switch (mMode) {
        case 0:
        case 1:
            if (!mStagger.checkStagger()) {
                mFaceMotionSeqMngr.setNo(0, -1.0f, 0, 0);
                mMotionSeqMngr.setNo(0, -1.0f, 0, 0);
                mMode = 2;
            }
        case 2:
            if (!mStagger.checkStagger()) {
                if (mPlayerActorMngr.getActorP()) {
                    mJntAnm.lookNone(0);
                    if (chkActorInSight(mPlayerActorMngr.getActorP(), mAttnFovY, mCurAngle.y)) {
                        mJntAnm.lookPlayer(0);
                    }

                    if (!srchPlayerActor() && home.angle.y == mCurAngle.y) {
                        mMode = 1;
                    }
                } else {
                    mJntAnm.lookNone(0);
                    if (home.angle.y != mCurAngle.y) {
                        if (field_0xe34 == 0) {
                            setAngle(home.angle.y);
                            mMode = 1;
                        } else {
                            if (step(home.angle.y, -1, -1, 15, 0)) {
                                mMode = 1;
                            }
                        }
                        attention_info.flags = 0;
                    } else {
                        srchPlayerActor();
                    }
                }
            }
        case 3:
        default:
            return 1;
    }
}

int daNpc_Zant_c::talk(void* param_1) {
    switch (mMode) {
        case 0:
        case 1:
            if (!mStagger.checkStagger()) {
                initTalk(mFlowNodeNo, NULL);
                if (checkStep()) {
                    mStepMode = 0;
                }
                mMode = 2;
            }
        case 2:
            if (!mStagger.checkStagger()) {
                if (mTwilight || mCurAngle.y == fopAcM_searchPlayerAngleY(this)) {
                    if (talkProc(NULL, 0, NULL, 0)) {
                        mPlayerActorMngr.entry(daPy_getPlayerActorClass());
                        dComIfGp_event_reset();
                        mMode = 3;
                    }

                    mJntAnm.lookPlayer(0);
                    if (mTwilight) {
                        mJntAnm.lookNone(0);
                    }
                } else {
                    mJntAnm.lookPlayer(0);
                    step(fopAcM_searchPlayerAngleY(this), -1, -1, 15, 0);
                }
            }
        case 3:
        default:
            return 0;
    }
}

static int daNpc_Zant_Create(void* i_this) {
    return static_cast<daNpc_Zant_c*>(i_this)->create();
}

static int daNpc_Zant_Delete(void* i_this) {
    return static_cast<daNpc_Zant_c*>(i_this)->Delete();
}

static int daNpc_Zant_Execute(void* i_this) {
    return static_cast<daNpc_Zant_c*>(i_this)->Execute();
}

static int daNpc_Zant_Draw(void* i_this) {
    return static_cast<daNpc_Zant_c*>(i_this)->Draw();
}

static int daNpc_Zant_IsDelete(void* i_this) {
    return 1;
}

static actor_method_class daNpc_Zant_MethodTable = {
    (process_method_func)daNpc_Zant_Create,
    (process_method_func)daNpc_Zant_Delete,
    (process_method_func)daNpc_Zant_Execute,
    (process_method_func)daNpc_Zant_IsDelete,
    (process_method_func)daNpc_Zant_Draw,
};

extern actor_process_profile_definition g_profile_NPC_ZANT = {
  fpcLy_CURRENT_e,         // mLayerID
  7,                       // mListID
  fpcPi_CURRENT_e,         // mListPrio
  PROC_NPC_ZANT,           // mProcName
  &g_fpcLf_Method.base,   // sub_method
  sizeof(daNpc_Zant_c),    // mSize
  0,                       // mSizeOther
  0,                       // mParameters
  &g_fopAc_Method.base,    // sub_method
  383,                     // mPriority
  &daNpc_Zant_MethodTable, // sub_method
  0x00044107,              // mStatus
  fopAc_NPC_e,             // mActorType
  fopAc_CULLBOX_CUSTOM_e,  // cullType
};
