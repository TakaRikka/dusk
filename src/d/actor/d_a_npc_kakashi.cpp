/**
 * @file d_a_npc_kakashi.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_npc_kakashi.h"
#include "d/d_com_static.h"
#include "d/d_s_play.h"
#include "Z2AudioLib/Z2Instances.h"

static int l_bmdData[3][2] = {
    {10, 1},
    {8, 1},
    {9, 1},
};

static daNpcT_evtData_c l_evtList[21] = {
    {"", 0},
    {"NO_RESPONSE", 0},
    {"MAROS_WHISPER", 1},
    {"SWDTUTORIAL_DESCRIPTION", 1},
    {"", 1},
    {"SWDTUTORIAL_YOKOGIRI_FAILURE", 1},
    {"SWDTUTORIAL_YOKOGIRI_CLEAR", 1},
    {"SWDTUTORIAL_TATEGIRI_SUCCESS", 1},
    {"SWDTUTORIAL_TATEGIRI_FAILURE", 1},
    {"SWDTUTORIAL_TATEGIRI_FAILURE2", 1},
    {"SWDTUTORIAL_TATEGIRI_CLEAR", 1},
    {"", 1},
    {"SWDTUTORIAL_TUKI_FAILURE", 1},
    {"SWDTUTORIAL_TUKI_CLEAR", 1},
    {"", 1},
    {"SWDTUTORIAL_KAITENGIRI_FAILURE", 1},
    {"SWDTUTORIAL_KAITENGIRI_CLEAR", 1},
    {"", 1},
    {"SWDTUTORIAL_JUMPGIRI_FAILURE", 1},
    {"SWDTUTORIAL_JUMPGIRI_CLEAR", 1},
    {"SWDTUTORIAL_JUMPGIRI_CLEAR2", 1},
};

static char* l_resNameList[2] = {
    "",
    "Kakashi",
};

static s8 l_loadResPtrn0[] = {1, -1};

static s8* l_loadResPtrnList[6] = {
    l_loadResPtrn0, l_loadResPtrn0, l_loadResPtrn0,
    l_loadResPtrn0, l_loadResPtrn0, NULL,
};

static daNpcT_faceMotionAnmData_c l_faceMotionAnmData[1] = {
    {-1, 0, 0, -1, 0, 0, 0},
};

static daNpcT_motionAnmData_c l_motionAnmData[1] = {
    {-1, 0, 0, -1, 0, 0, 0, 0},
};

static daNpcT_MotionSeqMngr_c::sequenceStepData_c l_faceMotionSequenceData[4] = {
    {-1, 0, 0},
    {-1, 0, 0},
    {-1, 0, 0},
    {-1, 0, 0},
};

static daNpcT_MotionSeqMngr_c::sequenceStepData_c l_motionSequenceData[4] = {
    {-1, 0, 0},
    {-1, 0, 0},
    {-1, 0, 0},
    {-1, 0, 0},
};

char* daNpc_Kakashi_c::mCutNameList[4] = {
    "",
    "SWDTUTORIAL",
    "GET_WOODSWD",
    "MAROS_WHISPER",
};

int (daNpc_Kakashi_c::*daNpc_Kakashi_c::mCutList[])(int) = {
    NULL,
    &daNpc_Kakashi_c::cutSwdTutorial,
    &daNpc_Kakashi_c::cutGetWoodSwd,
    &daNpc_Kakashi_c::cutMarosWhisper,
};

daNpc_Kakashi_c::~daNpc_Kakashi_c() {
    OS_REPORT("|%06d:%x|daNpc_Kakashi_c -> デストラクト\n", g_Counter.mCounter0, this);

    if (heap != NULL) {
        mpMorf[0]->stopZelAnime();
        mSound.deleteObject();
    }

    deleteRes(l_loadResPtrnList[mType], (const char**)l_resNameList);
}

const daNpc_Kakashi_HIOParam daNpc_Kakashi_Param_c::m = {
    190.0f,
    -6.0f,
    1.0f,
    700.0f,
    255.0f,
    40.0f,
    0.0f,
    30.0f,
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
    0,
    0,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    30.0f,
    30.0f,
    27.0f,
};

int daNpc_Kakashi_c::create() {
    daNpcT_ct(this, daNpc_Kakashi_c, l_faceMotionAnmData, l_motionAnmData,
                       l_faceMotionSequenceData, 4, l_motionSequenceData, 4,
                       l_evtList, l_resNameList);

    mType = getType();
    mFlowNodeNo = getFlowNodeNo();
    mTwilight = FALSE;

    int phase_state = loadRes(l_loadResPtrnList[mType], (const char**)l_resNameList);
    if (phase_state == cPhs_COMPLEATE_e) {
        OS_REPORT("\t(%s:%d) flowNo:%d, SWBit:%02x, SWBit2:%02x<%08x> ",
                  fopAcM_getProcNameString(this), mType, mFlowNodeNo, getBitSW(), getBitSW2(), fopAcM_GetParam(this), fopAcM_GetRoomNo(this));

        if (isDelete()) {
            OS_REPORT("===>isDelete:TRUE\n");
            return cPhs_ERROR_e;
        }

        OS_REPORT("\n");

        if (!fopAcM_entrySolidHeap(this, createHeapCallBack, 0xDA0)) {
            return cPhs_ERROR_e;
        }

        J3DModelData* modelData = mpMorf[0]->getModel()->getModelData();
        fopAcM_SetMtx(this, mpMorf[0]->getModel()->getBaseTRMtx());
        fopAcM_setCullSizeBox(this, -300.0f, -50.0f, -300.0f, 300.0f, 450.0f, 300.0f);
        fopAcM_OnStatus(this, fopAcM_STATUS_UNK_0x8000000);
        
        mSound.init(&current.pos, 3);

        reset();

        mAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1, &mAcchCir, fopAcM_GetSpeed_p(this), fopAcM_GetAngle_p(this), fopAcM_GetShapeAngle_p(this));
    
        mCcStts.Init(daNpc_Kakashi_Param_c::m.common.weight, 0, this);
        for (int i = 0; i < 3; i++) {
            mCcSph[i].Set(mCcDSph);
            mCcSph[i].SetStts(&mCcStts);
        }

        mCcCyl.Set(mCcDCyl);
        mCcCyl.SetStts(&mCcStts);
        mCcCyl.SetTgHitCallback(tgHitCallBack);

        mAcch.CrrPos(dComIfG_Bgsp());
        mGndChk = mAcch.m_gnd;
        mGroundH = mAcch.GetGroundH();

        if (mGroundH != -G_CM3D_F_INF) {
            setEnvTevColor();
            setRoomNo();
        }

        mCreating = TRUE;
        Execute();
        mCreating = FALSE;
    }

    return phase_state;
}

static f32 dummyFloat(int _) {
    switch (_) {
    case 0: return 0.0f;
    case 1: return 65536.0f;
    default: return 1.0f / 5.0f;
    }
}

int daNpc_Kakashi_c::CreateHeap() {
    void* mdlData_p = NULL;
    J3DModel* mdl_p = NULL;
    int var_r25 = 0;
    const char* arcname = l_resNameList[l_bmdData[var_r25][1]];
    int bmd_index = l_bmdData[var_r25][0];

    mdlData_p = dComIfG_getObjectRes(arcname, bmd_index);
    if (mdlData_p == NULL) {
        return 0;
    }

    u32 diff_flags = J3D_DIFF_FLAG(FALSE, FALSE, TRUE, 8, 2, FALSE, 2, 0, TRUE, FALSE, FALSE, FALSE, TRUE, FALSE);
    mpMorf[0] = new mDoExt_McaMorfSO((J3DModelData*)mdlData_p, NULL, NULL, NULL, -1, 1.0f, 0, -1, NULL, J3DMdlFlag_DifferedDLBuffer, diff_flags);
    if (mpMorf[0] == NULL || mpMorf[0]->getModel() == NULL) {
        return 0;
    }

    mdl_p = mpMorf[0]->getModel();
    for (u16 i = 0; i < ((J3DModelData*)mdlData_p)->getJointNum(); i++) {
        ((J3DModelData*)mdlData_p)->getJointNodePointer(i)->setCallBack(ctrlJointCallBack);
    }

    mdl_p->setUserArea((uintptr_t)this);
    return 1;
}

int daNpc_Kakashi_c::Delete() {
    OS_REPORT("|%06d:%x|daNpc_Kakashi_c -> Delete\n", g_Counter.mCounter0, this);
    fopAcM_GetID(this);
    this->~daNpc_Kakashi_c();
    return 1;
}

int daNpc_Kakashi_c::Execute() {
    execute();
    mSound.framework(0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
    return 1;
}

int daNpc_Kakashi_c::Draw() {
    return draw(FALSE, FALSE, mRealShadowSize, NULL, 100.0f, FALSE, FALSE, FALSE);
}

int daNpc_Kakashi_c::ctrlJoint(J3DJoint* i_joint, J3DModel* i_model) {
    u16 jnt_no = i_joint->getJntNo();
    mDoMtx_stack_c::copy(i_model->getAnmMtx(jnt_no));

    switch (jnt_no) {
    case 0:
        mDoMtx_stack_c::YrotM(field_0x1364[0].x);
        mDoMtx_stack_c::ZrotM(field_0x1364[0].z);
        break;
    case 1:
        mDoMtx_stack_c::YrotM(field_0x1364[1].x);
        mDoMtx_stack_c::ZrotM(field_0x1364[1].z);
        break;
    case 3:
        mDoMtx_stack_c::YrotM(field_0x1364[2].x);
        mDoMtx_stack_c::ZrotM(-field_0x1364[2].z);
        break;
    case 4:
    case 5:
        if (health <= 1) {
            mDoMtx_stack_c::scaleM(0.0f, 0.0f, 0.0f);
        }
        break;
    case 7:
        if (health == 0) {
            mDoMtx_stack_c::scaleM(0.0f, 0.0f, 0.0f);
        }
        break;
    }

    i_model->setAnmMtx(jnt_no, mDoMtx_stack_c::get());
    cMtx_copy(mDoMtx_stack_c::get(), J3DSys::mCurrentMtx);
    return 1;
}

int daNpc_Kakashi_c::createHeapCallBack(fopAc_ac_c* i_this) {
    return ((daNpc_Kakashi_c*)i_this)->CreateHeap();
}

int daNpc_Kakashi_c::ctrlJointCallBack(J3DJoint* i_joint, int param_1) {
    if (param_1 == 0) {
        J3DModel* mdl_p = j3dSys.getModel();
        daNpc_Kakashi_c* i_this = (daNpc_Kakashi_c*)mdl_p->getUserArea();
        if (i_this != NULL) {
            i_this->ctrlJoint(i_joint, mdl_p);
        }
    }

    return 1;
}

int daNpc_Kakashi_c::getType() {
    switch (fopAcM_GetParam(this) & 0xFF) {
    case 0:
        return 0;
    case 1:
        return 1;
    case 2:
        return 2;
    case 3:
        return 3;
    case 4:
        return 4;
    default:
        return 5;
    }
}

int daNpc_Kakashi_c::isDelete() {
    switch (mType) {
    case 0:
        return daNpcT_chkEvtBit(0x12);
    case 1:
        return daNpcT_chkEvtBit(0x12) == FALSE;
    case 2:
        return FALSE;
    case 3:
        return FALSE;
    case 4:
        return FALSE;
    default:
        return FALSE;
    }
}

void daNpc_Kakashi_c::reset() {
    csXyz angle;
    u32 var_r30 = (uintptr_t)&field_0x1394 - (uintptr_t)&mSelectAction;

    if (mpMatAnm[0] != NULL) {
        mpMatAnm[0]->initialize();
    }

    initialize();
    memset(&mSelectAction, 0, var_r30);

    angle.setall(0);
    angle.y = home.angle.y;

    switch (mType) {
    case 0:
        daNpcKakashi_setSwdTutorialStep(0);
        daNpcKakashi_clrSuccessCount();
        daNpcT_offTmpBit(0x11);
        daNpcT_offTmpBit(0x12);
        health = 2;
        break;
    case 1:
        health = 1;
        break;
    case 2:
        health = 2;
        break;
    case 3:
        health = 2;
        break;
    case 4:
        health = 2;
        break;
    }

    if (health == 2) {
        if (strcmp(dComIfGp_getStartStageName(), "F_SP108") == 0) {
            health = 1;
        }
    }

    setAngle(angle);
}

void daNpc_Kakashi_c::setParam() {
    selectAction();
    srchActors();

    attention_info.distances[fopAc_attn_LOCK_e] = 3;

    u32 attn_flags = fopAc_AttnFlag_LOCK_e;
    if (mType == 4 && health < 2) {
        attn_flags = 0;
    }

    attention_info.flags = attn_flags;

    scale.setall(daNpc_Kakashi_Param_c::m.common.scale);

    mCcStts.SetWeight(daNpc_Kakashi_Param_c::m.common.weight);
    mCylH = daNpc_Kakashi_Param_c::m.common.height;
    mWallR = daNpc_Kakashi_Param_c::m.common.width;
    mAttnFovY = daNpc_Kakashi_Param_c::m.common.fov;
    mAcchCir.SetWallR(mWallR);
    mAcchCir.SetWallH(daNpc_Kakashi_Param_c::m.common.knee_length);
    mRealShadowSize = daNpc_Kakashi_Param_c::m.common.real_shadow_size;
    mExpressionMorfFrame = daNpc_Kakashi_Param_c::m.common.expression_morf_frame;
    mMorfFrames = daNpc_Kakashi_Param_c::m.common.morf_frame;
    gravity = daNpc_Kakashi_Param_c::m.common.gravity;
}

void daNpc_Kakashi_c::srchActors() {}

BOOL daNpc_Kakashi_c::checkChangeEvt() {
    if (!chkAction(&daNpc_Kakashi_c::talk)) {
        switch (mType) {
        case 0:
            if (getTalkPattern() == 2) {
                mEvtNo = 2;
                evtChange();
                return 1;
            }
            break;
        case 1:
        case 2:
        case 3:
        case 4:
            break;
        }        
    }

    return 0;
}

BOOL daNpc_Kakashi_c::evtTalk() {
    if (chkAction(&daNpc_Kakashi_c::talk)) {
        (this->*mAction)(NULL);
    } else {
        setAction(&daNpc_Kakashi_c::talk);
    }

    return TRUE;
}

BOOL daNpc_Kakashi_c::evtEndProc() {
    field_0x138e = 0;
    field_0x138f = 0;
    return TRUE;
}

BOOL daNpc_Kakashi_c::evtCutProc() {
    int staffId = dComIfGp_getEventManager().getMyStaffId("Kakashi", this, -1);
    if (staffId != -1) {
        mStaffId = staffId;
        int actIdx = dComIfGp_getEventManager().getMyActIdx(mStaffId, mCutNameList, ARRAY_SIZE(mCutNameList), 0, 0);

        if ((this->*mCutList[actIdx])(mStaffId)) {
            dComIfGp_getEventManager().cutEnd(mStaffId);
        }

        return TRUE;
    }

    return FALSE;
}

void daNpc_Kakashi_c::action() {
    fopAc_ac_c* hit_actor = hitChk();
    if (hit_actor != NULL) {
        setStaggerParam(hit_actor);
        setDamage(0, -1, -1);
        mDamageTimerStart = 0;
    }

    if (mSelectAction != NULL) {
        if (mAction == mSelectAction) {
            (this->*mAction)(NULL);
        } else {
            setAction(mSelectAction);
        }
    }
}

void daNpc_Kakashi_c::beforeMove() {
    if (checkHide() || mNoDraw) {
        attention_info.flags = 0;
    }
}

void daNpc_Kakashi_c::setAttnPos() {
    cXyz sp20(0.0f, 0.0f, 0.0f);
    cXyz sp14;
    cXyz sp8;

    f32 attn_offset = daNpc_Kakashi_Param_c::m.common.attention_offset;

    if (field_0x1392 == 0) {
        mDoMtx_stack_c::YrotS(field_0x138c);
    }

    for (int i = 0; i < 3; i++) {
        if (field_0x1392 != 0) {
            mDoMtx_stack_c::YrotS(field_0x1384[i]);

            sp14.set(0.0f, 0.0f, field_0x1378[i]);
            mDoMtx_stack_c::multVec(&sp14, &sp8);

            field_0x1364[i].x = -sp8.z;
            field_0x1364[i].z = -sp8.x;

            if (i == 2) {
                field_0x1384[i] += cM_deg2s(45.0f);
            } else {
                field_0x1384[i] -= cM_deg2s(45.0f);
            }
        } else {
            sp14.set(0.0f, 0.0f, field_0x1378[i] * cM_ssin(field_0x1384[i]));
            mDoMtx_stack_c::multVec(&sp14, &sp8);

            field_0x1364[i].x = -sp8.z;
            field_0x1364[i].z = -sp8.x;

            field_0x1384[i] += cM_deg2s(45.0f);
        }
    }

    cLib_addCalc2(&field_0x1378[2], 0.0f, 0.075f, 50.0f);
    cLib_addCalc2(&field_0x1378[0], 0.0f, 0.1f, 125.0f);
    cLib_addCalc2(&field_0x1378[1], 0.0f, 0.1f, 125.0f);

    setMtx();

    mDoMtx_stack_c::copy(mpMorf[0]->getModel()->getAnmMtx(7));
    mDoMtx_stack_c::multVec(&sp20, &eyePos);

    if (health == 0) {
        eyePos.y -= 40.0f;
    }

    attention_info.position = current.pos;
    attention_info.position.y += attn_offset;

    if (health == 2) {
        attention_info.position.y += 50.0f;
    }
}

static daNpc_Kakashi_Param_c l_HIO;

void daNpc_Kakashi_c::setCollision() {
    if (!mHide) {
        static cXyz armROfst(80.0f, 0.0f, 0.0f);
        static cXyz armLOfst(-80.0f, 0.0f, 0.0f);
        static cXyz headOfst(20.0f, 0.0f, 0.0f);

        cXyz sp8;
        for (int i = 0; i < 3; i++) {
            if (i == 0) {
                mCcSph[i].SetCoSPrm(0);
                mCcSph[i].SetTgType(0x4060A0);
                mCcSph[i].SetTgSPrm(7);
            } else {
                mCcSph[i].SetTgType(0);
                mCcSph[i].SetTgSPrm(0);
            }
        }

        if (health != 0) {
            mDoMtx_stack_c::copy(mpMorf[0]->getModel()->getAnmMtx(7));
            mDoMtx_stack_c::multVec(&headOfst, &sp8);
            mCcSph[0].SetC(sp8);
            mCcSph[0].SetR(30.0f);
            dComIfG_Ccsp()->Set(&mCcSph[0]);
        }

        mDoMtx_stack_c::copy(mpMorf[0]->getModel()->getAnmMtx(2));

        mDoMtx_stack_c::multVec(&armROfst, &sp8);
        mCcSph[1].SetCoSPrm(0x19);
        mCcSph[1].SetC(sp8);
        mCcSph[1].SetR(30.0f);
        dComIfG_Ccsp()->Set(&mCcSph[1]);

        mDoMtx_stack_c::multVec(&armLOfst, &sp8);
        mCcSph[2].SetCoSPrm(0x19);
        mCcSph[2].SetC(sp8);
        mCcSph[2].SetR(30.0f);
        dComIfG_Ccsp()->Set(&mCcSph[2]);

        for (int i = 0; i < 3; i++) {
            mCcSph[i].ClrCoHit();
            mCcSph[i].ClrTgHit();
        }

        field_0x1393 = 0;

        mCcCyl.SetTgType(0);
        mCcCyl.SetTgSPrm(0);

        if (mDamageTimer == 0 && daNpcT_chkActorInScreen(this, -20.0f, 0.0f, -20.0f, 20.0f, 100.0f, 20.0f, 80.0f, 0)) {
            mCcCyl.SetTgType(0xD8FBFDFF);
            mCcCyl.SetTgHitMark(CcG_Tg_UNK_MARK_1);
            mCcCyl.SetTgSPrm(0x1F);
        }

        sp8 = current.pos;
        sp8.y += 80.0f;

        mCcCyl.SetH(mCylH);
        mCcCyl.SetR(mWallR);
        mCcCyl.SetC(sp8);
        dComIfG_Ccsp()->Set(&mCcCyl);
    }

    mCcCyl.ClrCoHit();
    mCcCyl.ClrTgHit();
}

int daNpc_Kakashi_c::drawDbgInfo() {
    return 0;
}

int daNpc_Kakashi_c::selectAction() {
    mSelectAction = NULL;

    switch (mType) {
    case 0:
        if (daNpcKakashi_chkSwdTutorialStage()) {
            mSelectAction = &daNpc_Kakashi_c::swdTutorial;
        } else {
            mSelectAction = &daNpc_Kakashi_c::wait;
        }
        break;
    case 1:
    case 2:
    case 3:
    case 4:
        mSelectAction = &daNpc_Kakashi_c::wait;
        break;
    default:
        mSelectAction = &daNpc_Kakashi_c::wait;
        break;
    }

    return 1;
}

int daNpc_Kakashi_c::chkAction(int (daNpc_Kakashi_c::*i_action)(void*)) {
    return mAction == i_action;
}

int daNpc_Kakashi_c::setAction(int (daNpc_Kakashi_c::*i_action)(void*)) {
    mMode = 3;
    if (mAction != NULL) {
        (this->*mAction)(NULL);
    }

    mMode = 0;
    mAction = i_action;
    if (mAction != NULL) {
        (this->*mAction)(NULL);
    }

    return 1;
}

fopAc_ac_c* daNpc_Kakashi_c::hitChk() {
    dCcD_GObjInf* hit_obj = NULL;
    fopAc_ac_c* hit_actor = NULL;

    mCcStts.Move();

    if (mDamageTimer == 0) {
        if (mCcSph[0].ChkTgHit()) {
            hit_actor = mCcSph[0].GetTgHitAc();
            hit_obj = (dCcD_GObjInf*)mCcSph[0].GetTgHitObj();
            field_0x1393 = 1;
        } else if (mCcCyl.ChkTgHit()) {
            hit_actor = mCcCyl.GetTgHitAc();
            hit_obj = (dCcD_GObjInf*)mCcCyl.GetTgHitObj();
        }
    }

    if (hit_obj != NULL) {
        field_0x1390 = 0;

        switch (mType) {
        case 0:
            if (hit_obj->ChkAtType(AT_TYPE_NORMAL_SWORD)) {
                field_0x1390 = 1;
            }
            break;
        case 2:
            if (hit_obj->ChkAtType(AT_TYPE_NORMAL_SWORD) && getBitSW() != 0xFF) {
                if (!dComIfGs_isSwitch(getBitSW(), fopAcM_GetRoomNo(this))) {
                    dComIfGs_onSwitch(getBitSW(), fopAcM_GetRoomNo(this));
                }
            }
            break;
        case 3:
            if (hit_obj->ChkAtType(AT_TYPE_ARROW) && getBitSW() != 0xFF) {
                if (!dComIfGs_isSwitch(getBitSW(), fopAcM_GetRoomNo(this))) {
                    dComIfGs_onSwitch(getBitSW(), fopAcM_GetRoomNo(this));
                }
            }
            break;
        case 4:
            u8 var_r28;
            if (field_0x1393 != 0) {
                if (getBitSW2() != 0xFF) {
                    if (!dComIfGs_isSwitch(getBitSW2(), fopAcM_GetRoomNo(this))) {
                        dComIfGs_onSwitch(getBitSW2(), fopAcM_GetRoomNo(this));
                    }
                }
                var_r28 = 1;
                dComIfGs_setTmpReg(0xF4FF, dComIfGs_getTmpReg(0xF4FF) + 1);
            } else {
                if (getBitSW() != 0xFF) {
                    if (!dComIfGs_isSwitch(getBitSW(), fopAcM_GetRoomNo(this))) {
                        dComIfGs_onSwitch(getBitSW(), fopAcM_GetRoomNo(this));
                    }
                }
                var_r28 = 2;
            }

            if (dComIfGp_getAttention()->GetLockonList(0) != NULL && dComIfGp_getAttention()->LockonTruth() && this == dComIfGp_getAttention()->GetLockonList(0)->getActor()) {
                var_r28 |= 0x80;
            }

            dComIfGs_setTmpReg(0xF3FF, var_r28);
            break;
        }

        u32 hit_se = dCcD_GObjInf::getHitSeID(hit_obj->GetAtSe(), 0);
        u32 var_r27 = 0x2E;
        if (mCutType == daPy_py_c::CUT_TYPE_JUMP ||
            mCutType == daPy_py_c::CUT_TYPE_LARGE_JUMP_INIT ||
            mCutType == daPy_py_c::CUT_TYPE_LARGE_JUMP ||
            mCutType == daPy_py_c::CUT_TYPE_LARGE_JUMP_FINISH ||
            field_0x1393 != 0)
        {
            switch (health) {
            case 2:
                if (field_0x1391 != 0) {
                    var_r27 = 0x31;
                } else {
                    var_r27 = 0x30;
                }
                break;
            case 1:
                var_r27 = 0x33;
                break;
            }
        } else if (mCutType == daPy_py_c::CUT_TYPE_TURN_RIGHT ||
                   mCutType == daPy_py_c::CUT_TYPE_TURN_LEFT ||
                   mCutType == daPy_py_c::CUT_TYPE_LARGE_TURN_LEFT ||
                   mCutType == daPy_py_c::CUT_TYPE_LARGE_TURN_RIGHT)
        {
            var_r27 = 0x2F;
        }

        mSound.startCollisionSE(hit_se, var_r27, NULL);
    }

    return hit_actor;
}

void daNpc_Kakashi_c::setStaggerParam(fopAc_ac_c* i_hitActor) {
    csXyz sp14;
    f32 var_f31 = 1.0f;
    f32 sp28[3] = {daNpc_Kakashi_Param_c::m.field_0x8c, daNpc_Kakashi_Param_c::m.field_0x90, daNpc_Kakashi_Param_c::m.field_0x94};

    field_0x138c = 0;

    if (i_hitActor != NULL) {
        field_0x138c = (fopAcM_searchActorAngleY(this, i_hitActor) - mCurAngle.y) + 0x8000;
    }

    field_0x1392 = 0;

    if (mCutType == daPy_py_c::CUT_TYPE_NM_RIGHT ||
        mCutType == daPy_py_c::CUT_TYPE_NM_LEFT ||
        mCutType == daPy_py_c::CUT_TYPE_FINISH_LEFT ||
        mCutType == daPy_py_c::CUT_TYPE_FINISH_VERTICAL)
    {
        if (mCutType == daPy_py_c::CUT_TYPE_NM_RIGHT) {
            field_0x138c -= 0x4000;
        } else if (mCutType == daPy_py_c::CUT_TYPE_NM_LEFT) {
            field_0x138c += 0x4000;
        }

        dScnPly_c::setPauseTimer(5);
        var_f31 = 0.6f;
    } else if (mCutType == daPy_py_c::CUT_TYPE_TURN_RIGHT ||
               mCutType == daPy_py_c::CUT_TYPE_TURN_LEFT ||
               mCutType == daPy_py_c::CUT_TYPE_LARGE_TURN_LEFT ||
               mCutType == daPy_py_c::CUT_TYPE_LARGE_TURN_RIGHT)
    {
        dScnPly_c::setPauseTimer(8);
        field_0x1392 = 1;
    } else {
        if (daPy_getPlayerActorClass() == i_hitActor) {
            dScnPly_c::setPauseTimer(5);
        }
        var_f31 = 0.6f;
    }

    for (int i = 0; i < 3; i++) {
        field_0x1378[i] = cM_deg2s(sp28[i]);

        if (i != 2) {
            field_0x1378[i] *= var_f31;
        }

        field_0x1384[i] = 0x4000;
    }

    if ((mCutType == daPy_py_c::CUT_TYPE_JUMP ||
        mCutType == daPy_py_c::CUT_TYPE_LARGE_JUMP_INIT ||
        mCutType == daPy_py_c::CUT_TYPE_LARGE_JUMP ||
        mCutType == daPy_py_c::CUT_TYPE_LARGE_JUMP_FINISH || field_0x1393 != 0) &&
        (&daNpc_Kakashi_c::swdTutorial != mSelectAction || daNpcKakashi_getSwdTutorialStep() == 6))
    {
        cXyz pos;
        mDoMtx_stack_c::copy(mpMorf[0]->getModel()->getAnmMtx(7));
        mDoMtx_stack_c::multVecZero(&pos);
        pos.y += 40.0f;

        switch (health) {
        case 2:
            sp14 = mCurAngle;
            setPrtcls(0);
            sp14.y = fopAcM_searchPlayerAngleY(this);

            fopAcM_createChild(PROC_OBJ_KBACKET, fopAcM_GetID(this), 0x80000000, &pos, fopAcM_GetRoomNo(this), &sp14, NULL, -1, NULL);
            fopAcM_createChild(PROC_OBJ_KBACKET, fopAcM_GetID(this), 0x80000001, &pos, fopAcM_GetRoomNo(this), &sp14, NULL, -1, NULL);

            if (field_0x1393 == 0) {
                break;
            }

            health = 1;
        case 1:
            setPrtcls(1);
        }

        cLib_calcTimer<s16>(&health);
    }

    mSound.startSound(Z2SE_OBJ_KAKASHI_SHAKE, 0, -1);
}

void daNpc_Kakashi_c::setPrtcls(int i_type) {
    cXyz pos;
    if (i_type == 0 || i_type == 1) {
        mDoMtx_stack_c::copy(mpMorf[0]->getModel()->getAnmMtx(7));
        mDoMtx_stack_c::multVecZero(&pos);
    } else {
        pos = current.pos;
    }

    if (i_type >= 0 && i_type < 2) {
        static const u16 id[] = {
            dPa_RM(ID_ZI_S_KAKASHI_WARA_A),
            dPa_RM(ID_ZI_S_KAKASHI_PUMP_A),
        };
        dComIfGp_particle_set(id[i_type], &pos, &mCurAngle, NULL);
    }
}

int daNpc_Kakashi_c::judgeSwdTutorial() {
    switch (daNpcKakashi_getSwdTutorialStep()) {
    case 2:
        if (mCutType == daPy_py_c::CUT_TYPE_NM_RIGHT ||
            mCutType == daPy_py_c::CUT_TYPE_NM_LEFT ||
            mCutType == daPy_py_c::CUT_TYPE_FINISH_LEFT ||
            mCutType == daPy_py_c::CUT_TYPE_FINISH_VERTICAL)
        {
            return TRUE;
        }
    case 3:
        if (mCutType == daPy_py_c::CUT_TYPE_NM_VERTICAL) {
            return TRUE;
        }
        break;
    case 4:
        if (mCutType == daPy_py_c::CUT_TYPE_NM_STAB ||
            mCutType == daPy_py_c::CUT_TYPE_FINISH_STAB ||
            mCutType == daPy_py_c::CUT_TYPE_COMBO_STAB)
        {
            return TRUE;
        }
        break;
    case 5:
        if (mCutType == daPy_py_c::CUT_TYPE_TURN_RIGHT ||
            mCutType == daPy_py_c::CUT_TYPE_TURN_LEFT ||
            mCutType == daPy_py_c::CUT_TYPE_LARGE_TURN_LEFT ||
            mCutType == daPy_py_c::CUT_TYPE_LARGE_TURN_RIGHT)
        {
            return TRUE;
        }
        break;
    case 6:
        if (mCutType == daPy_py_c::CUT_TYPE_JUMP ||
            mCutType == daPy_py_c::CUT_TYPE_LARGE_JUMP_INIT ||
            mCutType == daPy_py_c::CUT_TYPE_LARGE_JUMP ||
            mCutType == daPy_py_c::CUT_TYPE_LARGE_JUMP_FINISH)
        {
            return TRUE;
        }
        break;
    }

    return FALSE;
}

int daNpc_Kakashi_c::cutSwdTutorial(int i_staffId) {
    int rt = 0;
    int prm = -1;

    int* prm_p = dComIfGp_evmng_getMyIntegerP(i_staffId, "prm");
    if (prm_p != NULL) {
        prm = *prm_p;
    }

    if (dComIfGp_getEventManager().getIsAddvance(i_staffId)) {
        switch (prm) {
        case 0:
            break;
        case 1:
            s16 angle = fopAcM_searchActorAngleY(daPy_getPlayerActorClass(), this);
            daPy_getPlayerActorClass()->setPlayerPosAndAngle(&daPy_getPlayerActorClass()->current.pos, angle, 0);
            daNpcKakashi_setSwdTutorialStep(2);
            daNpcKakashi_clrSuccessCount();
        }
    }

    switch (prm) {
    case 0:
    case 1:
        rt = 1;
    }

    return rt;
}

int daNpc_Kakashi_c::cutGetWoodSwd(int i_staffId) {
    int rt = 0;
    int prm = -1;

    int* prm_p = dComIfGp_evmng_getMyIntegerP(i_staffId, "prm");
    if (prm_p != NULL) {
        prm = *prm_p;
    }

    if (dComIfGp_getEventManager().getIsAddvance(i_staffId)) {

    }

    switch (prm) {
    case 0:
        rt = 1;
    }

    return rt;
}

int daNpc_Kakashi_c::cutMarosWhisper(int i_staffId) {
    int rt = 0;
    int prm = -1;

    int* prm_p = dComIfGp_evmng_getMyIntegerP(i_staffId, "prm");
    if (prm_p != NULL) {
        prm = *prm_p;
    }

    if (dComIfGp_getEventManager().getIsAddvance(i_staffId)) {

    }

    return rt;
}

int daNpc_Kakashi_c::wait(void*) {
    switch (mMode) {
    case 0:
    case 1:
        mMode = 2;
        break;
    case 2:
    case 3:
        break;
    }

    return 1;
}

int daNpc_Kakashi_c::swdTutorial(void*) {
    switch (mMode) {
    case 0:
    case 1:
        mMode = 2;
    case 2: {
        u8 tutorial_step = daNpcKakashi_getSwdTutorialStep();
        if (tutorial_step == 0) {
            mEvtNo = 3;
            field_0xe33 = 1;
        } else {
            if (field_0x1390 != 0 && mCutType != 0) {
                if (judgeSwdTutorial()) {
                    daNpcKakashi_setSwdTutorialResult(1);
                    daNpcKakashi_clrSuccessCount();

                    if ((tutorial_step == 2 || tutorial_step == 3) && mCutType != daPy_py_c::CUT_TYPE_NM_VERTICAL) {
                        daNpcKakashi_incSuccessCount();
                    }

                    field_0x138e = 1;
                } else {
                    daNpcKakashi_setSwdTutorialResult(0);
                    field_0x138f = 1;
                }
            }

            u8 tutorial_step = daNpcKakashi_getSwdTutorialStep();
            daNpcKakashi_getSuccessCount();

            if (field_0x138e != 0) {
                switch (tutorial_step) {
                case 0:
                case 1:
                    field_0x138e = 0;
                    break;
                case 2:
                case 3:
                    if (mCutType == daPy_py_c::CUT_TYPE_NM_VERTICAL) {
                        mEvtNo = 10;
                    } else {
                        mEvtNo = 6;
                    }
                    break;
                case 4:
                    mEvtNo = 13;
                    break;
                case 5:
                    mEvtNo = 16;
                    field_0x1391 = 1;
                    break;
                case 6:
                    mEvtNo = 19;
                    break;
                default:
                    field_0x138e = 0;
                }
            }

            if (field_0x138f != 0) {
                switch (tutorial_step) {
                case 0:
                case 1:
                    field_0x138f = 0;
                    break;
                case 2:
                case 3:
                    mEvtNo = 5;
                    break;
                case 4:
                    mEvtNo = 12;
                    break;
                case 5:
                    mEvtNo = 15;
                    break;
                case 6:
                    mEvtNo = 18;
                    break;
                default:
                    field_0x138f = 0;
                }
            }
        }
        break;
    }
    case 3:
        break;
    }

    return 1;
}

int daNpc_Kakashi_c::talk(void*) {
    switch (mMode) {
    case 0:
    case 1:
        if (!mStagger.checkStagger()) {
            initTalk(mFlowNodeNo, NULL);
            mMode = 2;
        }
    case 2:
       if (!mStagger.checkStagger() && talkProc(NULL, 0, NULL, 0) && mFlow.checkEndFlow()) {
            dComIfGp_event_reset();
            mMode = 3;
        }
        break;
    case 3:
        break;
    }

    return 0;
}

static int daNpc_Kakashi_Create(void* i_this) {
    return ((daNpc_Kakashi_c*)i_this)->create();
}

static int daNpc_Kakashi_Delete(void* i_this) {
    return ((daNpc_Kakashi_c*)i_this)->Delete();
}

static int daNpc_Kakashi_Execute(void* i_this) {
    return ((daNpc_Kakashi_c*)i_this)->Execute();
}

static int daNpc_Kakashi_Draw(void* i_this) {
    return ((daNpc_Kakashi_c*)i_this)->Draw();
}

static int daNpc_Kakashi_IsDelete(void* i_this) {
    return 1;
}

static actor_method_class daNpc_Kakashi_MethodTable = {
    (process_method_func)daNpc_Kakashi_Create,
    (process_method_func)daNpc_Kakashi_Delete,
    (process_method_func)daNpc_Kakashi_Execute,
    (process_method_func)daNpc_Kakashi_IsDelete,
    (process_method_func)daNpc_Kakashi_Draw,
};

extern actor_process_profile_definition g_profile_NPC_KAKASHI = {
  fpcLy_CURRENT_e,            // mLayerID
  7,                          // mListID
  fpcPi_CURRENT_e,            // mListPrio
  PROC_NPC_KAKASHI,           // mProcName
  &g_fpcLf_Method.base,      // sub_method
  sizeof(daNpc_Kakashi_c),    // mSize
  0,                          // mSizeOther
  0,                          // mParameters
  &g_fopAc_Method.base,       // sub_method
  319,                        // mPriority
  &daNpc_Kakashi_MethodTable, // sub_method
  0x00044000,                 // mStatus
  fopAc_NPC_e,                // mActorType
  fopAc_CULLBOX_CUSTOM_e,     // cullType
};

AUDIO_INSTANCES
