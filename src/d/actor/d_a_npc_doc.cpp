/**
 * @file d_a_npc_doc.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_npc_doc.h"
#include "Z2AudioLib/Z2Instances.h"

static int l_bmdData[1][2] = {
    {14, 1},
};

static daNpcT_evtData_c l_evtList[2] = {
    {"", 0},
    {"NO_RESPONSE", 0},
};

static char* l_resNameList[3] = {
    "",
    "Doc",
    "Doc1",
};

static s8 l_loadResPtrn0[] = {
    1,
    2,
    -1,
};

static s8* l_loadResPtrnList[3] = {
    l_loadResPtrn0,
    l_loadResPtrn0,
    l_loadResPtrn0,
};

static daNpcT_faceMotionAnmData_c l_faceMotionAnmData[10] = {
    {-1, 0, 0, 20, 2, 1, 1},
    {6, 0, 1, 20, 2, 1, 1},
    {9, 0, 2, 34, 0, 2, 0},
    {13, 2, 2, 38, 2, 2, 0},
    {10, 0, 2, 35, 0, 2, 0},
    {11, 0, 2, 36, 0, 2, 0},
    {14, 2, 2, 39, 2, 2, 0},
    {12, 0, 2, 37, 0, 2, 0},
    {7, 0, 2, 20, 2, 1, 1},
    {8, 2, 2, 20, 2, 1, 1},
};

static daNpcT_motionAnmData_c l_motionAnmData[14] = {
    {11, 2, 1, 17, 0, 1, 1, 0},
    {8, 0, 1, 17, 0, 1, 1, 0},
    {7, 0, 1, 17, 0, 1, 1, 0},
    {9, 0, 1, 17, 0, 1, 1, 0},
    {10, 2, 1, 17, 0, 1, 1, 0},
    {21, 2, 2, 17, 0, 1, 1, 0},
    {15, 0, 2, 26, 0, 2, 0, 0},
    {16, 2, 2, 27, 2, 2, 0, 0},
    {17, 0, 2, 28, 0, 2, 0, 0},
    {18, 0, 2, 29, 0, 2, 0, 0},
    {19, 2, 2, 30, 2, 2, 0, 0},
    {20, 0, 2, 31, 0, 2, 0, 0},
    {5, 0, 2, 24, 0, 2, 0, 0},
    {6, 2, 2, 25, 2, 2, 0, 0},
};

static daNpcT_MotionSeqMngr_c::sequenceStepData_c l_faceMotionSequenceData[40] = {
    {1, -1, 1}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {2, -1, 1}, {3, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {2, -1, 1}, {3, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {5, -1, 1}, {6, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {6, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {8, -1, 1}, {9, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {3, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {6, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {9, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {0, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
};

static daNpcT_MotionSeqMngr_c::sequenceStepData_c l_motionSequenceData[52] = {
    {0, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {3, -1, 1}, {4, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {4, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {6, -1, 1}, {7, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {7, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {9, -1, 1}, {10, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {10, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {11, -1, 1}, {0, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {12, -1, 1}, {13, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {13, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {1, -1, 1}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {2, -1, 1}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {5, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
};

char* daNpc_Doc_c::mCutNameList[1] = {""};

daNpc_Doc_c::cutFunc daNpc_Doc_c::mCutList[1] = {
    NULL,
};

const daNpc_Doc_HIOParam daNpc_Doc_Param_c::m = {
    140.0f,
    -3.0f,
    1.0f,
    400.0f,
    255.0f,
    110.0f,
    35.0f,
    55.0f,
    0.0f,
    0.0f,
    10.0f,
    -10.0f,
    30.0f,
    -10.0f,
    45.0f,
    -45.0f,
    0.6f,
    12.0f,
    3,
    6,
    5,
    6,
    110.0f,
    0.0f,
    0.0f,
    0.0f,
    60,
    8,
    0,
    0,
    0,
    0,
    0,
    4.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    3.0f,
};

static NPC_DOC_HIO_CLASS l_HIO;

daNpc_Doc_c::~daNpc_Doc_c() {
    OS_REPORT("|%06d:%x|daNpc_Doc_c -> デストラクト\n", g_Counter.mCounter0, this);

    if (mpMorf[0] != NULL) {
        mpMorf[0]->stopZelAnime();
    }

    #if DEBUG
    if (mpHIO != NULL) {
        mpHIO->removeHIO();
    }
    #endif

    deleteRes(l_loadResPtrnList[mType], (const char**)l_resNameList);
}

int daNpc_Doc_c::create() {
    daNpcT_ct(this, daNpc_Doc_c, l_faceMotionAnmData, l_motionAnmData,
              l_faceMotionSequenceData, 4, l_motionSequenceData, 4, l_evtList, l_resNameList);

    mType = getType();
    mFlowNodeNo = getFlowNodeNo();
    mTwilight = dKy_darkworld_check();

    int phase_state = loadRes(l_loadResPtrnList[mType], (const char**)l_resNameList);
    if (phase_state == cPhs_COMPLEATE_e) {
        OS_REPORT("\t(%s:%d) flowNo:%d, PathID:%02x, BitSW:%02x, OnHour:%2d, OffHour:%2d<%08x> ",
                  fopAcM_getProcNameString(this), mType, mFlowNodeNo, getPathID(), getBitSW(), getOnHour(), getOffHour(), fopAcM_GetParam(this) & 0xFF);

        if (isDelete()) {
            OS_REPORT("===>isDelete:TRUE\n");
            return cPhs_ERROR_e;
        }

        OS_REPORT("\n");

        static int const heapSize[3] = {
            0x4850, 0x4850, 0x0,
        };

        if (!fopAcM_entrySolidHeap(this, createHeapCallBack, heapSize[mType])) {
            return cPhs_ERROR_e;
        }

        J3DModelData* mdlData_p = mpMorf[0]->getModel()->getModelData();
        fopAcM_SetMtx(this, mpMorf[0]->getModel()->getBaseTRMtx());
        fopAcM_setCullSizeBox(this, -200.0f, -100.0f, -200.0f, 200.0f, 200.0f, 200.0f);
        mSound.init(&current.pos, &eyePos, 3, 1);
        field_0x9c0.init(&mAcch, 0.0f, 0.0f);

        #if DEBUG
        mpHIO = &l_HIO;
        mpHIO->entryHIO("町医者");
        #endif

        reset();
        
        mAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1, &mAcchCir, fopAcM_GetSpeed_p(this),
                  fopAcM_GetAngle_p(this), fopAcM_GetShapeAngle_p(this));
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

    return phase_state;
}

int daNpc_Doc_c::CreateHeap() {
    void* mdlData_p = NULL;
    J3DModel* model = NULL;
    int resIdx;
    int bmdIdx;

    enum { IDX0 };
    int idx = mTwilight == true ? IDX0 : IDX0;
    resIdx = l_bmdData[idx][1];
    bmdIdx = l_bmdData[idx][0];

    mdlData_p = dComIfG_getObjectRes(l_resNameList[resIdx], bmdIdx);
    if (mdlData_p == NULL) {
        return 0;
    }

    u32 sp1C = 0x11020284;
    mpMorf[0] = new mDoExt_McaMorfSO((J3DModelData*)mdlData_p, NULL, NULL, NULL, -1, 1.0f, 0, -1, &mSound, J3DMdlFlag_DifferedDLBuffer, sp1C);
    if (mpMorf[0] == NULL || mpMorf[0]->getModel() == NULL) {
        return 0;
    }

    model = mpMorf[0]->getModel();
    for (u16 i = 0; i < ((J3DModelData*)mdlData_p)->getJointNum(); i++) {
        ((J3DModelData*)mdlData_p)->getJointNodePointer(i)->setCallBack(ctrlJointCallBack);
    }
    model->setUserArea((uintptr_t)this);

    mpMatAnm[0] = new daNpcT_MatAnm_c();
    if (mpMatAnm[0] == NULL) {
        return 0;
    }

    if (setFaceMotionAnm(1, false) && setMotionAnm(0, 0.0f, FALSE)) {
        return 1;
    }

    return 0;
}

int daNpc_Doc_c::Delete() {
    OS_REPORT("|%06d:%x|daNpc_Doc_c -> Delete\n", g_Counter.mCounter0, this);
    fpc_ProcID id = fopAcM_GetID(this);
    this->~daNpc_Doc_c();
    return 1;
}

int daNpc_Doc_c::Execute() {
    return execute();
}

int daNpc_Doc_c::Draw() {
    if (mpMatAnm[0] != NULL) {
        J3DModelData* mdlData_p = mpMorf[0]->getModel()->getModelData();
        mdlData_p->getMaterialNodePointer(getEyeballMaterialNo())->setMaterialAnm(mpMatAnm[0]);
    }

    return draw(
#if DEBUG
        chkAction(&daNpc_Doc_c::test),
#else
        FALSE,
#endif
        FALSE, mRealShadowSize, NULL, 100.0f, FALSE, FALSE, FALSE
    );
}

int daNpc_Doc_c::createHeapCallBack(fopAc_ac_c* i_this) {
    return ((daNpc_Doc_c*)i_this)->CreateHeap();
}

int daNpc_Doc_c::ctrlJointCallBack(J3DJoint* i_joint, int param_1) {
    if (param_1 == 0) {
        J3DModel* mdl_p = j3dSys.getModel();
        daNpc_Doc_c* actor = (daNpc_Doc_c*)mdl_p->getUserArea();
        if (actor != NULL) {
            actor->ctrlJoint(i_joint, mdl_p);
        }
    }

    return 1;
}

int daNpc_Doc_c::getType() {
    u8 r31 = fopAcM_GetParam(this) & 0xFF;
    switch (r31) {
    case 0:
        return 0;
    case 1:
        return 1;
    default:
        return 2;
    }
}

int daNpc_Doc_c::isDelete() {
    switch (mType) {
    case 0:
        return 0;
    case 1:
        return 0;
    default:
        return 0;
    }
}

void daNpc_Doc_c::reset() {
    csXyz sp10;
    u32 var_r29 = (uintptr_t)&field_0xfc8 - (uintptr_t)&field_0xfac;

    if (mpMatAnm[0] != NULL) {
        mpMatAnm[0]->initialize();
    }

    initialize();
    memset(&field_0xfac, 0, var_r29);

    if (getPathID() != 0xFF) {
        mPath.initialize();
        mPath.setPathInfo(getPathID(), fopAcM_GetRoomNo(this), 0);
    }

    sp10.setall(0);
    sp10.y = home.angle.y;

    switch (mType) {
    case 0:
        break;
    case 1:
        if (daNpcT_chkEvtBit(0x141)) {
            field_0xfc4 = 1;
            int temp_r29 = mPath.getNumPnts() - 1;
            cXyz sp48(mPath.getPntPos(temp_r29));
            cXyz sp3C(mPath.getPntPos(temp_r29 - 1));

            home.pos = sp48;
            home.angle.y = cLib_targetAngleY(&sp3C, &sp48);
            sp10.y = home.angle.y;
            setPos(home.pos);
        }
        break;
    }

    setAngle(sp10);
}

void daNpc_Doc_c::afterJntAnm(int i_joint) {
    if (i_joint == 1) {
        mDoMtx_stack_c::YrotM(mStagger.getAngleZ(1));
        mDoMtx_stack_c::ZrotM(-mStagger.getAngleX(1));
    } else if (i_joint == 4) {
        mDoMtx_stack_c::YrotM(mStagger.getAngleZ(0));
        mDoMtx_stack_c::ZrotM(-mStagger.getAngleX(0));
    }
}

void daNpc_Doc_c::setParam() {
    selectAction();
    srchActors();

    u32 attn_flags = fopAc_AttnFlag_SPEAK_e | fopAc_AttnFlag_TALK_e;

    s16 talk_distance = mpHIO->m.common.talk_distance;
    s16 talk_angle = mpHIO->m.common.talk_angle;
    s16 attention_distance = mpHIO->m.common.attention_distance;
    s16 attention_angle = mpHIO->m.common.attention_angle;

    attention_info.distances[fopAc_attn_LOCK_e] = daNpcT_getDistTableIdx(attention_distance, attention_angle);
    attention_info.distances[fopAc_attn_TALK_e] = attention_info.distances[fopAc_attn_LOCK_e];
    attention_info.distances[fopAc_attn_SPEAK_e] = daNpcT_getDistTableIdx(talk_distance, talk_angle);

    if (daPy_py_c::checkNowWolf()) {
        attn_flags |= fopAc_AttnFlag_UNK_0x800000;
    }
    
    attention_info.flags = attn_flags;

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
}

BOOL daNpc_Doc_c::checkChangeEvt() {
    if (!chkAction(&daNpc_Doc_c::talk)) {
        mPreItemNo = 0;
        if (dComIfGp_event_chkTalkXY()) {
            if (dComIfGp_evmng_ChkPresentEnd()) {
                mPreItemNo = dComIfGp_event_getPreItemNo();
                if (mType == 1) {
                    if (mPreItemNo == fpcNm_ITEM_WOOD_STATUE) {
                        return 0;
                    }

                    if (!daNpcT_chkEvtBit(0x141) && mPreItemNo == fpcNm_ITEM_BILL) {
                        return 0;
                    }
                }

                mEvtNo = 1;
                evtChange();
            }

            return true;
        }
    }

    return false;
}

void daNpc_Doc_c::setAfterTalkMotion() {
    int idx = 9;
    switch (mFaceMotionSeqMngr.getNo()) {
    case 5:
        idx = 8;
        break;
    }

    mFaceMotionSeqMngr.setNo(idx, -1.0f, FALSE, 0);
}

void daNpc_Doc_c::srchActors() {}

BOOL daNpc_Doc_c::evtTalk() {
    if (chkAction(&daNpc_Doc_c::talk)) {
        (this->*mAction)(NULL);
    } else {
        setAction(&daNpc_Doc_c::talk);
    }

    return TRUE;
}

BOOL daNpc_Doc_c::evtCutProc() {
    int staffId = dComIfGp_getEventManager().getMyStaffId("Doc", this, -1);
    if (staffId != -1) {
        mStaffId = staffId;
        int act_idx = dComIfGp_getEventManager().getMyActIdx(mStaffId, mCutNameList, 1, FALSE, FALSE);

        if ((this->*mCutList[act_idx])(mStaffId)) {
            dComIfGp_getEventManager().cutEnd(mStaffId);
        }

        return TRUE;
    }

    return FALSE;
}

void daNpc_Doc_c::action() {
    fopAc_ac_c* actor_p = hitChk(&mCyl, 0xFFFFFFFF);

    if (actor_p != NULL && mCyl.GetTgHitObj()->ChkAtType(AT_TYPE_THROW_OBJ)) {
        mStagger.setParam(this, actor_p, mCurAngle.y);
        setDamage(0, 9, 0);
        mDamageTimerStart = 0;
        mJntAnm.lookNone(1);
        field_0xfc6 = 0;
    }

    if (mStagger.checkRebirth()) {
        mStagger.initialize();
        mMode = MODE_INIT;
    }

    if (field_0xfac) {
        if (mAction == field_0xfac) {
            (this->*mAction)(NULL);
        } else {
            setAction(field_0xfac);
        }
    }
}

void daNpc_Doc_c::beforeMove() {
    if (mType == 0 && field_0xe34 == 0) {
        int hour = 0;
        if (mTwilight != 0) {
            hour = dKy_getDarktime_hour();
        } else {
            hour = dKy_getdaytime_hour();
        }

        mHide = 0;
        if ((getOnHour() >= 0 && hour < getOnHour()) || (getOffHour() >= 0 && getOffHour() < hour)) {
            mHide = 1;
        }
    }

    fopAcM_OffStatus(this, fopAcM_STATUS_UNK_0x8000000);

    if (checkHide()) {
        fopAcM_OnStatus(this, fopAcM_STATUS_UNK_0x8000000);
    }

    if (checkHide() || mNoDraw) {
        attention_info.flags = 0;
    }
}

void daNpc_Doc_c::setAttnPos() {
    cXyz pos(30.0f, 0.0f, 0.0f);

    mStagger.calc(FALSE);
    f32 rad = cM_s2rad((s16)(mCurAngle.y - field_0xd7e.y));
    
    mJntAnm.setParam(this, mpMorf[0]->getModel(), &pos, getBackboneJointNo(), getNeckJointNo(), getHeadJointNo(),
                     mpHIO->m.common.body_angleX_min, mpHIO->m.common.body_angleX_max, mpHIO->m.common.body_angleY_min, mpHIO->m.common.body_angleY_max,
                     mpHIO->m.common.head_angleX_min, mpHIO->m.common.head_angleX_max, mpHIO->m.common.head_angleY_min, mpHIO->m.common.head_angleY_max,
                     mpHIO->m.common.neck_rotation_ratio, rad, NULL);
    mJntAnm.calcJntRad(0.1f, 1.0f, rad);
    setMtx();

    mDoMtx_stack_c::copy(mpMorf[0]->getModel()->getAnmMtx(getHeadJointNo()));
    mDoMtx_stack_c::multVec(&pos, &eyePos);

    mJntAnm.setEyeAngleX(eyePos, 1.0f, -0x1000);
    mJntAnm.setEyeAngleY(eyePos, mCurAngle.y, TRUE, 1.0f, 0);

    pos.set(0.0f, 0.0f, 60.0f);
    pos.y = mpHIO->m.common.attention_offset;

    mDoMtx_stack_c::YrotS(mCurAngle.y);
    mDoMtx_stack_c::multVec(&pos, &pos);
    attention_info.position = pos + current.pos;

    static cXyz prtclScl(1.0f, 1.0f, 1.0f);
    setFootPos();
    if (3.0f < speedF) {
        setFootPrtcl(&prtclScl, 11.0f, 0.0f);
    }
}

void daNpc_Doc_c::setCollision() {
    cXyz pos;

    if (!mHide) {
        u32 co_s_prm = 0x79;
        u32 tg_type = 0xD8FBFDFF;
        u32 tg_s_prm = 0x1F;

        if (mTwilight) {
            tg_type = 0;
            tg_s_prm = 0;
        } else if (mStagger.checkStagger()) {
            tg_type = 0;
            tg_s_prm = 0;
        }

        mCyl.SetCoSPrm(co_s_prm);
        mCyl.SetTgType(tg_type);
        mCyl.SetTgSPrm(tg_s_prm);
        mCyl.OnTgNoHitMark();

        pos.set(0.0f, 0.0f, 20.0f);
        f32 cyl_h = mCylH;
        f32 wall_r = mWallR;

        if (mType == 1) {
            if (!daNpcT_chkEvtBit(0x141)) {
                wall_r = 100.0f;
            }
        }

        mDoMtx_stack_c::YrotS(mCurAngle.y);
        mDoMtx_stack_c::multVec(&pos, &pos);
        pos += current.pos;

        mCyl.SetH(cyl_h);
        mCyl.SetR(wall_r);
        mCyl.SetC(pos);
        dComIfG_Ccsp()->Set(&mCyl);
    }

    mCyl.ClrCoHit();
    mCyl.ClrTgHit();
}

BOOL daNpc_Doc_c::drawDbgInfo() {
    return false;
}

BOOL daNpc_Doc_c::selectAction() {
    field_0xfac = NULL;

    switch (mType) {
    case 1:
        if (!daNpcT_chkEvtBit(0x11B) && field_0xfc4 == 0 && daNpcT_chkEvtBit(0x141)) {
            field_0xfac = &daNpc_Doc_c::walk;
        } else {
            field_0xfac = &daNpc_Doc_c::wait;
        }
        break;
    default:
        field_0xfac = &daNpc_Doc_c::wait;
        break;
    }

    return TRUE;
}

int daNpc_Doc_c::chkAction(int (daNpc_Doc_c::*i_action)(void*)) {
    return mAction == i_action;
}

int daNpc_Doc_c::setAction(int (daNpc_Doc_c::*i_action)(void*)) {
    mMode = MODE_EXIT;

    if (mAction) {
        (this->*mAction)(NULL);
    }

    mMode = MODE_ENTER;

    mAction = i_action;
    if (mAction) {
        (this->*mAction)(NULL);
    }

    return 1;
}

int daNpc_Doc_c::wait(void* param_0) {
    switch (mMode) {
    case 0:
    case 1:
        if (!mStagger.checkStagger()) {
            if (field_0xfc6 != 0 && !daPy_py_c::checkNowWolf()) {
                mFaceMotionSeqMngr.setNo(9, -1.0f, 0, 0);
                mMotionSeqMngr.setNo(0xB, -1.0f, 0, 0);
                field_0xfc6 = 0;
            } else {
                mFaceMotionSeqMngr.setNo(9, -1.0f, 0, 0);
                mMotionSeqMngr.setNo(0, -1.0f, 0, 0);
            }
            mMode = 2;
        }
    case 2:
        if (mType == 1 && !daNpcT_chkEvtBit(0x7A)) {
            mSpeakEvent = 1;
            field_0xe33 = 1;
        }

        if (!mStagger.checkStagger()) {
            if (mMotionSeqMngr.getNo() == 11) {
                mJntAnm.lookNone(0);
                if (mMotionSeqMngr.checkEndSequence()) {
                    mMode = 1;
                }
                attention_info.flags = 0;
            } else {
                if (daPy_py_c::checkNowWolf()) {
                    mPlayerActorMngr.remove();
                }

                if (mPlayerActorMngr.getActorP() != NULL) {
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
                            if (step(home.angle.y, -1, -1, 0x1E, 0)) {
                                mMode = 1;
                                field_0xfc6 = 0;
                            }
                        } else {
                            setAngle(home.angle.y);
                            mMode = 1;
                            field_0xfc6 = 0;
                        }
                        attention_info.flags = 0;
                    } else if (!mTwilight) {
                        srchPlayerActor();
                    }
                }
            }

            switch (mJntAnm.getMode()) {
            case 0:
                break;
            }
        }
        break;
    case 3:
        break;
    }

    return 1;
}

int daNpc_Doc_c::walk(void* param_0) {
    switch (mMode) {
    case 0:
    case 1:
        if (!mStagger.checkStagger()) {
            mFaceMotionSeqMngr.setNo(9, -1.0f, 0, 0);
            mMotionSeqMngr.setNo(0xC, -1.0f, 0, 0);
            mAcch.SetWallNone();
            mMode = 2;
        }
        /* fallthrough */
    case 2:
        if (!mStagger.checkStagger()) {
            cXyz sp20;
            mJntAnm.lookNone(0);

            JUT_ASSERT(1731, NULL != mPath.getPathInfo());
            int var_r29 = mPath.getDstPosH(current.pos, &sp20, mPath.getNumPnts(), 2);
            calcSpeedAndAngle(sp20, var_r29, 4, 0x800);

            if (var_r29 != 0 && cM3d_IsZero(speedF)) {
                field_0xfc4 = 1;
                home.angle.y = current.angle.y;
                mPlayerActorMngr.remove();
            }
        }
        attention_info.flags = 0;
        break;
    case 3:
        mAcch.ClrWallNone();
        break;
    }
    return 1;
}

int daNpc_Doc_c::talk(void* param_0) {
    switch (mMode) {
    case MODE_ENTER:
    case MODE_INIT:
        if (!mStagger.checkStagger()) {
            if (mPreItemNo == fpcNm_ITEM_BILL) {
                initTalk(0x43F, NULL);
            } else if (mPreItemNo == fpcNm_ITEM_WOOD_STATUE) {
                initTalk(0x447, NULL);
            } else {
                initTalk(mFlowNodeNo, NULL);
            }

            field_0xfc5 = 0;

            if (daPy_py_c::checkNowWolf() || (field_0xfc4 == 1 && mPreItemNo == 0)) {
                field_0xfc5 = 1;
            }

            field_0xfc6 = 0;
            mMode = 2;
        }
        // fallthrough
    case MODE_RUN:
        if (!mStagger.checkStagger()) {
            if (mTwilight || mPlayerAngle == mCurAngle.y || daPy_py_c::checkNowWolf() || (field_0xfc4 == 1 && mPreItemNo == 0 && (!daNpcT_chkEvtBit(0x120) || daNpcT_chkEvtBit(0x164)))) {
                mJntAnm.lookNone(0);
                if (field_0xfc5 == 0) {
                    mMotionSeqMngr.setNo(0xA, -1.0f, 0, 0);
                    field_0xfc5 = 1;
                    field_0xfc6 = 1;
                } else {
                    if (mMotionSeqMngr.getNo() != 10 || mMotionSeqMngr.checkEndSequence()) {
                        int sp8[] = {-1, -1};

                        if (mPreItemNo == fpcNm_ITEM_BILL) {
                            sp8[0] = 0x1457;
                        } else if (mPreItemNo == fpcNm_ITEM_WOOD_STATUE) {
                            sp8[0] = 0x146E;
                            if (daNpcT_chkEvtBit(0x163)) {
                                sp8[0] = 0x1472;
                            }
                        }

                        if (talkProc(sp8, 0, NULL, 0)) {
                            if (!mFlow.checkEndFlow()) {
                                daPy_getPlayerActorClass()->changeOriginalDemo();
                                daPy_getPlayerActorClass()->changeDemoMode(6, 0, 1, 0);
                            } else {
                                mPlayerActorMngr.entry(daPy_getPlayerActorClass());
                                dComIfGp_event_reset();
                                mMode = 3;
                            }
                        }
                    }

                    mJntAnm.lookPlayer(0);
                    if (mTwilight || daPy_py_c::checkNowWolf() || (field_0xfc4 == 1 && mPreItemNo == 0 && (!daNpcT_chkEvtBit(0x120) || daNpcT_chkEvtBit(0x164)))) {
                        mJntAnm.lookNone(0);
                    }
                }
            } else {
                step(mPlayerAngle, -1, -1, 0x1E, 0);
            }
        }
        break;
    case 3:
        break;
    }

    return 0;
}

int daNpc_Doc_c::test(void* i_this) {
    // DEBUG NONMATCHING
}

static int daNpc_Doc_Create(void* i_this) {
    return ((daNpc_Doc_c*)i_this)->create();
}

static int daNpc_Doc_Delete(void* i_this) {
    return ((daNpc_Doc_c*)i_this)->Delete();
}

static int daNpc_Doc_Execute(void* i_this) {
    return ((daNpc_Doc_c*)i_this)->Execute();
}

static int daNpc_Doc_Draw(void* i_this) {
    return ((daNpc_Doc_c*)i_this)->Draw();
}

static int daNpc_Doc_IsDelete(void* i_this) {
    return 1;
}

static actor_method_class daNpc_Doc_MethodTable = {
    (process_method_func)daNpc_Doc_Create,
    (process_method_func)daNpc_Doc_Delete,
    (process_method_func)daNpc_Doc_Execute,
    (process_method_func)daNpc_Doc_IsDelete,
    (process_method_func)daNpc_Doc_Draw,
};

extern actor_process_profile_definition g_profile_NPC_DOC = {
  fpcLy_CURRENT_e,        // mLayerID
  7,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_NPC_DOC,           // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(daNpc_Doc_c),    // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  335,                    // mPriority
  &daNpc_Doc_MethodTable, // sub_method
  0x00040107,             // mStatus
  fopAc_NPC_e,            // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};

AUDIO_INSTANCES;
