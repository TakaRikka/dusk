/**
 * @file d_a_npc_soldierA.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_npc_soldierA.h"
#include "d/actor/d_a_npc.h"
#include "d/d_debug_viewer.h"
#include "Z2AudioLib/Z2Instances.h"

enum chtSolA_RES_File_ID {
    /* BCK */
    /* 0x05 */ BCK_SOLA_FEAR_TALK = 0x5,
    /* 0x06 */ BCK_SOLA_FEAR_WAIT,
    /* 0x07 */ BCK_SOLA_TALK_A,
    /* 0x08 */ BCK_SOLA_TALK_B,
    /* 0x09 */ BCK_SOLA_WAIT,
    /* 0x0A */ BCK_SOLA_WAIT_A,

    /* BMDR */
    /* 0x0D */ BMDR_SOLA_SPEAR = 0xD,
    /* 0x0E */ BMDR_SOLA_TW,

    /* EVT */
    /* 0x11 */ EVT_CHTSOLA_EVENT_LIST = 0x11,
};

enum RES_Name {
    /* 0x0 */ CHTSOLA,
};

enum Joint {
    /* 0x00 */ JNT_CENTER,
    /* 0x01 */ JNT_BACKBONE1,
    /* 0x02 */ JNT_BACKBONE2,
    /* 0x03 */ JNT_NECK,
    /* 0x04 */ JNT_HEAD,
    /* 0x05 */ JNT_SHOULDERL,
    /* 0x06 */ JNT_ARM1L,
    /* 0x07 */ JNT_ARM2L,
    /* 0x08 */ JNT_HANDL,
    /* 0x09 */ JNT_FINGERL,
    /* 0x0A */ JNT_THUMBL,
    /* 0x0B */ JNT_SHOULDERR,
    /* 0x0C */ JNT_ARM1R,
    /* 0x0D */ JNT_ARM2R,
    /* 0x0E */ JNT_HANDR,
    /* 0x0F */ JNT_FINGERR,
    /* 0x10 */ JNT_THUMBR,
    /* 0x11 */ JNT_WAIST,
    /* 0x12 */ JNT_LEG1L,
    /* 0x13 */ JNT_LEG2L,
    /* 0x14 */ JNT_FOOTL,
    /* 0x15 */ JNT_LEG1R,
    /* 0x16 */ JNT_LEG2R,
    /* 0x17 */ JNT_FOOTR,
    /* 0x18 */ JNT_SAYA
};

enum Animation {
    /* 0x0 */ ANM_NONE,
    /* 0x1 */ ANM_WAIT_A,
    /* 0x2 */ ANM_FEAR_WAIT,
    /* 0x3 */ ANM_FEAR_TALK,
    /* 0x4 */ ANM_WAIT,
    /* 0x5 */ ANM_TALK_A,
    /* 0x6 */ ANM_TALK_B,
};

enum Motion {
    /* 0x0 */ MOT_WAIT,
    /* 0x1 */ MOT_TALK_B,
    /* 0x2 */ MOT_TALK_A,
    /* 0x3 */ MOT_FEAR_WAIT,
    /* 0x4 */ MOT_FEAR_TALK,
    /* 0x5 */ MOT_WAIT_A,
};

enum Event {
    /* 0x0 */ EVT_NONE,
    /* 0x1 */ EVT_TALK_LAKE,
};

enum Event_Cut_Nums {
    /* 0x3 */ NUM_EVT_CUTS_e = 0x3,
};

static NPC_SOLDIERA_HIO_CLASS l_HIO;

static daNpc_GetParam1 l_bmdGetParamList[2] = {
    {BMDR_SOLA_TW, CHTSOLA},
    {BMDR_SOLA_SPEAR, CHTSOLA},
};

static daNpc_GetParam1 l_bckGetParamList[7] = {
    {-1, CHTSOLA},
    {BCK_SOLA_WAIT_A, CHTSOLA},
    {BCK_SOLA_FEAR_WAIT, CHTSOLA},
    {BCK_SOLA_FEAR_TALK, CHTSOLA},
    {BCK_SOLA_WAIT, CHTSOLA},
    {BCK_SOLA_TALK_A, CHTSOLA},
    {BCK_SOLA_TALK_B, CHTSOLA},
};

static daNpc_GetParam1 l_evtGetParamList[2] = {
    {0, CHTSOLA},
    {1, CHTSOLA},
};

static char* l_evtNames[2] = {
    NULL,
    "TALK_LAKE",
};

static int l_loadRes_SOLDIERaa[3] = {
    CHTSOLA, -1, -1,
};

static int l_loadRes_SOLDIERa0[3] = {
    CHTSOLA, -1, -1,
};

static int* l_loadRes_list[4] = {
    l_loadRes_SOLDIERaa,
    l_loadRes_SOLDIERaa,
    l_loadRes_SOLDIERaa,
    l_loadRes_SOLDIERa0,
};

static char* l_resNames[1] = {"chtSolA"};

static char* l_myName = "chtSolA";

char* daNpc_SoldierA_c::mEvtCutNameList[3] = {
    "",
    "TALK_LAKE",
    "LISTEN_LAKE",
};

daNpc_SoldierA_c::cutFunc daNpc_SoldierA_c::mEvtCutList[3] = {
    NULL,
    &daNpc_SoldierA_c::ECut_talkLake,
    &daNpc_SoldierA_c::ECut_listenLake,
};

daNpc_SoldierA_c::daNpc_SoldierA_c() {}

daNpc_SoldierA_c::~daNpc_SoldierA_c() {
    for (int i = 0; l_loadRes_list[mType][i] >= 0; i++) {
        dComIfG_resDelete(&mPhases[i], l_resNames[l_loadRes_list[mType][i]]);
    }

    if (heap != NULL) {
        mAnm_p->stopZelAnime();
    }

    #if DEBUG
    if (mHIO != NULL) {
        mHIO->removeHIO();
    }
    #endif
}

daNpc_SoldierA_HIOParam const daNpc_SoldierA_Param_c::m = {
    30.0f,
    -4.0f,
    1.0f,
    600.0f,
    255.0f,
    240.0f,
    35.0f,
    45.0f,
    0.0f,
    0.0f,
    10.0f,
    -10.0f,
    30.0f,
    -30.0f,
    45.0f,
    -45.0f,
    0.6f,
    12.0f,
    5,
    6,
    7,
    6,
    180.0f,
    500.0f,
    300.0f,
    -300.0f,
    60,
    8,
    0,
    0,
    0,
    false,
    false,
};

cPhs__Step daNpc_SoldierA_c::create() {
    mTwilight = dKy_darkworld_check();

    fopAcM_ct(this, daNpc_SoldierA_c);

    mType = getTypeFromParam();

    if (home.angle.x != 0xFFFF) {
        mMsgNo = home.angle.x;
    } else {
        mMsgNo = -1;
    }

    if (isDelete()) {
        return cPhs_ERROR_e;
    }

    int res_count = 0;
    int i = 0;
    for (; l_loadRes_list[mType][i] >= 0; i++) {
        cPhs__Step phase = (cPhs__Step)dComIfG_resLoad(&mPhases[i], l_resNames[l_loadRes_list[mType][i]]);
        if (phase == cPhs_ERROR_e || phase == cPhs_UNK3_e) {
            return cPhs_ERROR_e;
        }

        if (phase == cPhs_COMPLEATE_e) {
            res_count++;
        }
    }

    if (res_count == i) {
        if (!fopAcM_entrySolidHeap(this, createHeapCallBack, 0x2B20)) {
            return cPhs_ERROR_e;
        }

        J3DModelData* modelData = mAnm_p->getModel()->getModelData();
        fopAcM_SetMtx(this, mAnm_p->getModel()->getBaseTRMtx());
        fopAcM_setCullSizeBox(this, -300.0f, -50.0f, -300.0f, 300.0f, 450.0f, 300.0f);
        mSound.init(&current.pos, &eyePos, 3, 1);

        #if DEBUG
        mHIO = &l_HIO;
        mHIO->entryHIO("多人数会話兵士A");
        #endif

        mAcchCir.SetWall(mHIO->m.common.width, mHIO->m.common.knee_length);
        mAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1, &mAcchCir,
                fopAcM_GetSpeed_p(this), fopAcM_GetAngle_p(this), fopAcM_GetShapeAngle_p(this));
        mCcStts.Init(mHIO->m.common.weight, 0, this);
        mCyl.Set(mCcDCyl);
        mCyl.SetStts(&mCcStts);
        mCyl.SetTgHitCallback(tgHitCallBack);
        mAcch.CrrPos(dComIfG_Bgsp());
        mGndChk = mAcch.m_gnd;
        mGroundH = mAcch.GetGroundH();

        setEnvTevColor();
        setRoomNo();
        reset();
        Execute();

        return cPhs_COMPLEATE_e;
    }

    return cPhs_INIT_e;
}

int daNpc_SoldierA_c::CreateHeap() {
    J3DModelData* mdlData_p = NULL;
    J3DModel* model = NULL;

    if (l_bmdGetParamList[0].fileIdx >= 0) {
        mdlData_p = (J3DModelData*)dComIfG_getObjectRes(l_resNames[l_bmdGetParamList[0].arcIdx], l_bmdGetParamList[0].fileIdx);
    }

    JUT_ASSERT(413, NULL != mdlData_p);

    u32 uVar1 = 0x11020284;
    mAnm_p = new mDoExt_McaMorfSO(mdlData_p, NULL, NULL, NULL, -1, 1.0f, 0, -1, &mSound, J3DMdlFlag_DifferedDLBuffer, uVar1);
    if (mAnm_p != NULL && mAnm_p->getModel() == NULL) {
        mAnm_p->stopZelAnime();
        mAnm_p = NULL;
    }

    if (mAnm_p == NULL) {
        return 0;
    }

    model = mAnm_p->getModel();
    for (u16 i = 0; i < mdlData_p->getJointNum(); i++) {
        mdlData_p->getJointNodePointer(i)->setCallBack(ctrlJointCallBack);
    }
    model->setUserArea((uintptr_t)this);

    setMotionAnm(ANM_WAIT, 0.0f);

    int arcIdx = 0;
    arcIdx = l_bmdGetParamList[1].arcIdx;
    mdlData_p = (J3DModelData*)dComIfG_getObjectRes(l_resNames[arcIdx], l_bmdGetParamList[1].fileIdx);
    if (mdlData_p != NULL) {
        mSpearModel = mDoExt_J3DModel__create(mdlData_p, J3DMdlFlag_DifferedDLBuffer, 0x11000084);
        if (mSpearModel == NULL) {
            return 0;
        }
    } else {
        return 0;
    }

    return 1;
}

int daNpc_SoldierA_c::Delete() {
    fpc_ProcID id = fopAcM_GetID(this);
    this->~daNpc_SoldierA_c();
    return 1;
}

int daNpc_SoldierA_c::Execute() {
    return execute();
}

int daNpc_SoldierA_c::Draw() {
    return draw(chkAction(&daNpc_SoldierA_c::test), FALSE, mHIO->m.common.real_shadow_size, NULL, FALSE);
}

int daNpc_SoldierA_c::ctrlJoint(J3DJoint* i_joint, J3DModel* i_model) {
    int jntNo = i_joint->getJntNo();
    int i_jointList[3] = {JNT_BACKBONE1, JNT_NECK, JNT_HEAD};

    if (jntNo == JNT_CENTER) {
        mDoMtx_stack_c::copy(mAnm_p->getModel()->getAnmMtx(JNT_BACKBONE1));
        mDoMtx_stack_c::multVecZero(&mLookatPos[0]);
        mDoMtx_stack_c::copy(mAnm_p->getModel()->getAnmMtx(JNT_NECK));
        mDoMtx_stack_c::multVecZero(&mLookatPos[1]);
        mDoMtx_stack_c::copy(mAnm_p->getModel()->getAnmMtx(JNT_HEAD));
        mDoMtx_stack_c::multVecZero(&mLookatPos[2]);
    }

    mDoMtx_stack_c::copy(i_model->getAnmMtx(jntNo));

    switch (jntNo) {
        case JNT_BACKBONE1:
        case JNT_NECK:
        case JNT_HEAD:
            setLookatMtx(jntNo, i_jointList, mHIO->m.common.neck_rotation_ratio);
            break;
    }

    if (jntNo == JNT_BACKBONE1) {
        mDoMtx_stack_c::YrotM(-field_0x908[0].z);
        mDoMtx_stack_c::ZrotM(field_0x908[0].x);
    } else if (jntNo == JNT_HEAD) {
        mDoMtx_stack_c::YrotM(-field_0x908[2].z);
        mDoMtx_stack_c::ZrotM(field_0x908[2].x);
    }

    i_model->setAnmMtx(jntNo, mDoMtx_stack_c::get());
    MTXCopy(mDoMtx_stack_c::get(), J3DSys::mCurrentMtx);

    return 1;
}

int daNpc_SoldierA_c::createHeapCallBack(fopAc_ac_c* i_this) {
    daNpc_SoldierA_c* actor = (daNpc_SoldierA_c*)i_this;
    return actor->CreateHeap();
}

int daNpc_SoldierA_c::ctrlJointCallBack(J3DJoint* i_joint, int param_2) {
    if (param_2 == 0) {
        J3DModel* model = j3dSys.getModel();
        daNpc_SoldierA_c* actor = (daNpc_SoldierA_c*)model->getUserArea();
        if (actor != NULL) {
            actor->ctrlJoint(i_joint, model);
        }
    }

    return 1;
}

void daNpc_SoldierA_c::setParam() {
    actionFunc action = mNextAction;
    u32 flags = fopAc_AttnFlag_SPEAK_e | fopAc_AttnFlag_TALK_e;

    selectAction();
    
    if (mNextAction != action) {
        /* Bug??? - Loop never actually runs */
        for (int i = 3; i < 3; i++) {
            mActorMngrs[i].initialize();
        }
    }

    field_0xe00 = 0;
    field_0xe04 = 0;

    s16 talk_distance, talk_angle, attention_distance, attention_angle;
    attention_distance = 3;
    attention_angle = 6;
    talk_distance = 5;
    talk_angle = 6;

    if (mType == 1 || mType == 2) {
        flags = 0;
    }

    attention_info.distances[fopAc_attn_LOCK_e] = getDistTableIdx(talk_distance, talk_angle);
    attention_info.distances[fopAc_attn_TALK_e] = attention_info.distances[fopAc_attn_LOCK_e];
    attention_info.distances[fopAc_attn_SPEAK_e] = getDistTableIdx(attention_distance, attention_angle);
    attention_info.flags = flags;
    scale.set(mHIO->m.common.scale, mHIO->m.common.scale, mHIO->m.common.scale);
    mAcchCir.SetWallR(mHIO->m.common.width);
    mAcchCir.SetWallH(mHIO->m.common.knee_length);

    if (!dComIfGp_event_runCheck()) {
        gravity = mHIO->m.common.gravity;
    }
}

BOOL daNpc_SoldierA_c::main() {
    if (!doEvent()) {
        doNormalAction(1);
    }

    if (checkHide()) {
        attention_info.flags = 0;
    }

    if (!mHIO->m.common.debug_mode_ON) {
        if (!dComIfGp_event_runCheck() || (mOrderNewEvt && dComIfGp_getEvent().isOrderOK())) {
            if (mOrderEvtNo != EVT_NONE) {
                eventInfo.setArchiveName(l_resNames[l_evtGetParamList[mOrderEvtNo].arcIdx]);
            }

            orderEvent(mOrderSpeakEvt, l_evtNames[l_evtGetParamList[mOrderEvtNo].fileIdx], 0xFFFF, 0x28, 0xFF, 1);
        }
    }

    if (field_0x9ee) {
        mExpressionMorfOverride = 0.0f;
        mMotionMorfOverride = 0.0f;
        field_0x9ee = false;
    }

    playMotion();

    return TRUE;
}

BOOL daNpc_SoldierA_c::ctrlBtk() {
    return FALSE;
}

void daNpc_SoldierA_c::setAttnPos() {
    static cXyz eyeOffset(0.0f, 20.0f, 0.0f);
    cXyz sp74, sp80, sp8c, sp98;
    f32 attn_offset = mHIO->m.common.attention_offset;

    mDoMtx_stack_c::YrotS(field_0x990);
    cLib_addCalc2(&field_0x984[0], 0.0f, 0.1f, 125.0f);
    cLib_addCalc2(&field_0x984[2], 0.0f, 0.1f, 125.0f);

    for (int i = 0; i < 3; i++) {
        sp8c.set(0.0f, 0.0f, field_0x984[i] * cM_ssin(field_0x992));
        mDoMtx_stack_c::multVec(&sp8c, &sp98);
        field_0x908[i].x = -sp98.z;
        field_0x908[i].z = -sp98.x;
    }

    cLib_chaseS(&field_0x992, 0, 0x555);

    if (mLookMode == LOOK_RESET) {
        for (int i = 0; i < 3; i++) {
            mLookatAngle[i].setall(0);
        }
    }

    setMtx();
    lookat();

    mDoMtx_stack_c::copy(mAnm_p->getModel()->getAnmMtx(JNT_HEAD));
    mDoMtx_stack_c::multVecZero(&mHeadPos);
    mDoMtx_stack_c::multVec(&eyeOffset, &eyePos);
    mDoMtx_stack_c::multVec(&eyeOffset, &sp8c);
    mHeadAngle.x = cLib_targetAngleX(&mHeadPos, &sp8c);
    mHeadAngle.y = cLib_targetAngleY(&mHeadPos, &sp8c);

    cXyz* attn_pos = mLookat.getAttnPos();
    if (attn_pos != NULL) {
        sp80 = *attn_pos - eyePos;
        mEyeAngle.y = -(mLookatAngle[2].y + mCurAngle.y);
        mEyeAngle.y += cM_atan2s(sp80.x, sp80.z);
        mEyeAngle.x = -cM_atan2s(sp80.y, sp80.absXZ());
        mEyeAngle.x += mHeadAngle.x;
    } else {
        mEyeAngle.y = 0;
        mEyeAngle.x = 0;
    }

    f32 fVar2 = 0.0f;
    f32 fVar3 = 0.0f;
    f32 fVar4 = 0.0f;
    sp80.set(0.0f, 0.0f, 0.0f);
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::ZXYrotM(mCurAngle.x, mCurAngle.y, mCurAngle.z);
    mDoMtx_stack_c::multVec(&sp80, &sp74);
    attention_info.position.set(mHeadPos.x, mHeadPos.y + attn_offset, mHeadPos.z);

    if (!mHide) {
        if (!mIsDamaged) {
            mCyl.SetTgType(0xD8FBFDFF);
            mCyl.SetTgSPrm(0x1F);
            mCyl.OnTgNoHitMark();
        } else {
            mCyl.SetTgType(0);
            mCyl.SetTgSPrm(0);
        }

        mCyl.SetC(sp74);
        mCyl.SetH(mHIO->m.common.height + fVar3);
        mCyl.SetR(mHIO->m.common.width + fVar4);
        dComIfG_Ccsp()->Set(&mCyl);
    }

    mCyl.ClrTgHit();
}

void daNpc_SoldierA_c::setMotionAnm(int i_idx, f32 i_morf) {
    J3DAnmTransformKey* anm = NULL;
    int attr = J3DFrameCtrl::EMode_LOOP;

    mAnmFlags &= ~ANM_MOTION_FLAGS;

    if (l_bckGetParamList[i_idx].fileIdx >= 0) {
        anm = getTrnsfrmKeyAnmP(l_resNames[l_bckGetParamList[i_idx].arcIdx], l_bckGetParamList[i_idx].fileIdx);
    }

    mAnm = i_idx;

    switch (i_idx) {
        case ANM_WAIT_A:
        case ANM_FEAR_WAIT:
            break;
        
        case ANM_FEAR_TALK:
        case ANM_TALK_A:
        case ANM_TALK_B:
            attr = J3DFrameCtrl::EMode_NONE;
            break;

        case ANM_WAIT:
            break;

        default:
            anm = NULL;
            break;
    }

    if (anm != NULL && setMcaMorfAnm(anm, 1.0f, i_morf, attr, 0, -1)) {
        mAnmFlags |= ANM_PLAY_MORF | ANM_PAUSE_MORF;
        mMotionLoops = 0;
    }
}

void daNpc_SoldierA_c::setMotion(int i_motion, f32 i_morf, int param_3) {
    s16 motion = i_motion;
    if ((param_3 != 0 || mMotion != motion) && i_motion >= 0 && i_motion < 6) {
        mMotion = motion;
        mMotionMorfOverride = i_morf;
        mMotionPrevPhase = -1;
        mMotionPhase = 0;
    }
}

int daNpc_SoldierA_c::drawDbgInfo() {
    #if DEBUG
    if (mHIO->m.common.debug_info_ON) {
        cXyz i_end;
        f32 i_radius1 = dComIfGp_getAttention()->getDistTable(attention_info.distances[fopAc_attn_SPEAK_e]).mDistMax;
        f32 i_radius2 = dComIfGp_getAttention()->getDistTable(attention_info.distances[fopAc_attn_TALK_e]).mDistMax;
        s16 angle_x = cLib_targetAngleX(&current.pos, &eyePos);
        s16 angle_y = cLib_targetAngleY(&current.pos, &eyePos);

        dDbVw_drawCircleOpa(attention_info.position, i_radius1, (GXColor){0x00, 0xC8, 0x00, 0xFF}, 1, 12);
        dDbVw_drawCircleOpa(attention_info.position, i_radius2, (GXColor){0xC8, 0x00, 0x00, 0xFF}, 1, 12);

        if (mHIO->m.common.fov != 180.0f) {
            cXyz work(0.0f, 0.0f, mHIO->m.common.search_distance);
            mDoMtx_stack_c::transS(attention_info.position);
            mDoMtx_stack_c::YrotM(mHeadAngle.y);
            mDoMtx_stack_c::YrotM(cM_deg2s(-mHIO->m.common.fov));
            mDoMtx_stack_c::multVec(&work, &i_end);

            dDbVw_drawLineOpa(attention_info.position, i_end, (GXColor){0x00, 0x00, 0xC8, 0xFF}, 1, 12);

            mDoMtx_stack_c::transS(attention_info.position);
            mDoMtx_stack_c::YrotM(mHeadAngle.y);
            mDoMtx_stack_c::YrotM(cM_deg2s(mHIO->m.common.fov));
            mDoMtx_stack_c::multVec(&work, &i_end);

            dDbVw_drawLineOpa(attention_info.position, i_end, (GXColor){0x00, 0x00, 0xC8, 0xFF}, 1, 12);
        }

        dDbVw_drawCircleOpa(attention_info.position, mHIO->m.common.search_distance, (GXColor){0x00, 0x00, 0xC8, 0xFF}, 1, 12);
        dDbVw_drawSphereXlu(eyePos, 18.0f, (GXColor){0x80, 0x80, 0x80, 0xA0}, 1);
        dDbVw_drawSphereXlu(mLookPos, 18.0f, (GXColor){0x80, 0x80, 0x80, 0xA0}, 1);
        
        int idx = getDistTableIdx(field_0xe00, field_0xe04);
        dist_entry& entry = dComIfGp_getAttention()->getDistTable(idx);
        dDbVw_drawCircleOpa(attention_info.position, entry.mDistMax, (GXColor){0xC8, 0xC8, 0xC8, 0xFF}, 1, 12);
    }
    #endif

    return 0;
}

void daNpc_SoldierA_c::drawOtherMdls() {
    g_env_light.setLightTevColorType_MAJI(mSpearModel, &tevStr);
    mDoMtx_stack_c::copy(mAnm_p->getModel()->getAnmMtx(JNT_HANDR));
    mDoMtx_stack_c::scaleM(scale);
    mSpearModel->setBaseTRMtx(mDoMtx_stack_c::get());
    J3DModelData* mdlData_p = mSpearModel->getModelData();
    fopAcM_setEffectMtx(this, mdlData_p);
    mDoExt_modelUpdateDL(mSpearModel);
    dComIfGd_addRealShadow(mShadowKey, mSpearModel);
}

u8 daNpc_SoldierA_c::getTypeFromParam() {
    u8 param = fopAcM_GetParam(this) & 0xFF;
    switch (param) {
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

BOOL daNpc_SoldierA_c::isDelete() {
    if (mType == 0 || mType == 1 || mType == 2 || mType == 3) {
        return FALSE;
    }

    return TRUE;
}

void daNpc_SoldierA_c::reset() {
    initialize();
    mLookat.initialize();
    for (int i = 0; i < 3; i++) {
        mActorMngrs[i].initialize();
    }

    mNextAction = NULL;
    mAction = NULL;
    field_0xe00 = 0;
    field_0xe04 = 0;
    field_0xe08 = 0;
    field_0xe0c = 0;
    mLookMode = -1;
    mMode = 0;
    current.pos = home.pos;
    old.pos = current.pos;
    current.angle.set(0, home.angle.y, 0);
    old.angle = current.angle;
    shape_angle = current.angle;
    mCurAngle = current.angle;
    mOldAngle = mCurAngle;
    speedF = 0.0f;
    speed.setall(0.0f);
    eventInfo.setIdx(mType + 1);
    field_0x9ee = true;
}

void daNpc_SoldierA_c::playMotion() {
    daNpcF_anmPlayData dat0 = {ANM_WAIT, mHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat0[1] = {&dat0};
    daNpcF_anmPlayData dat1a = {ANM_TALK_B, mHIO->m.common.morf_frame, 1};
    daNpcF_anmPlayData dat1b = {ANM_WAIT, 0.0f, 0};
    daNpcF_anmPlayData* pDat1[2] = {&dat1a, &dat1b};
    daNpcF_anmPlayData dat2a = {ANM_TALK_A, mHIO->m.common.morf_frame, 1};
    daNpcF_anmPlayData dat2b = {ANM_WAIT, 0.0f, 0};
    daNpcF_anmPlayData* pDat2[2] = {&dat2a, &dat2b};
    daNpcF_anmPlayData dat3 = {ANM_FEAR_WAIT, mHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat3[1] = {&dat3};
    daNpcF_anmPlayData dat4a = {ANM_FEAR_TALK, mHIO->m.common.morf_frame, 1};
    daNpcF_anmPlayData dat4b = {ANM_FEAR_WAIT, 0.0f, 0};
    daNpcF_anmPlayData* pDat4[2] = {&dat4a, &dat4b};
    daNpcF_anmPlayData dat5 = {ANM_WAIT_A, mHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat5[1] = {&dat5};

    daNpcF_anmPlayData** ppDat[6] = {
        pDat0,
        pDat1,
        pDat2,
        pDat3,
        pDat4,
        pDat5
    };

    if (mMotion >= 0 && mMotion < 6) {
        playMotionAnm(ppDat);
    }
}

BOOL daNpc_SoldierA_c::chkAction(actionFunc action) {
    return mAction == action;
}

BOOL daNpc_SoldierA_c::setAction(actionFunc action) {
    mMode = 3;

    if (mAction) {
        (this->*mAction)(NULL);
    }

    mMode = 0;
    mAction = action;

    if (mAction) {
        (this->*mAction)(NULL);
    }

    return TRUE;
}

BOOL daNpc_SoldierA_c::selectAction() {
    mNextAction = NULL;

    if (mHIO->m.common.debug_mode_ON) {
        mNextAction = &daNpc_SoldierA_c::test;
    } else {
        mNextAction = &daNpc_SoldierA_c::wait;
    }

    return TRUE;
}

void daNpc_SoldierA_c::doNormalAction(int param_1) {
    if (param_1 != 0 && hitChk2(&mCyl, TRUE, FALSE)) {
        int damage_time = mCutType == daPy_py_c::CUT_TYPE_TURN_RIGHT ? 20 : mHIO->m.common.damage_time;
        setDamage(damage_time, -1, 0);
        setLookMode(LOOK_RESET);
    } else if (mIsDamaged && mDamageTimer == 0) {
        mMode = 0;
        mIsDamaged = false;
    }

    mOrderEvtNo = EVT_NONE;
    if (mNextAction) {
        if (mAction == mNextAction) {
            (this->*mAction)(NULL);
        } else {
            setAction(mNextAction);
        }
    }
}

BOOL daNpc_SoldierA_c::doEvent() {
    int unused = 0;
    BOOL rv = FALSE;

    if (dComIfGp_event_runCheck() != FALSE) {
        dEvent_manager_c& event_manager = dComIfGp_getEventManager();

        if (eventInfo.checkCommandTalk() || eventInfo.checkCommandDemoAccrpt()) {
            mOrderNewEvt = false;
        }

        if (eventInfo.checkCommandTalk()) {
            if (chkAction(&daNpc_SoldierA_c::talk)) {
                (this->*mAction)(NULL);
            } else if (dComIfGp_event_chkTalkXY() == FALSE || dComIfGp_evmng_ChkPresentEnd()) {
                if (mType == 0) {
                    mOrderEvtNo = EVT_TALK_LAKE;
                    changeEvent(l_resNames[l_evtGetParamList[mOrderEvtNo].arcIdx], l_evtNames[l_evtGetParamList[mOrderEvtNo].fileIdx], 2, 0xFFFF);
                } else {
                    setAction(&daNpc_SoldierA_c::talk);
                }
            }

            rv = TRUE;
        } else {
            int staffId = event_manager.getMyStaffId(l_myName, this, 0);
            if (staffId != -1) {
                mStaffID = staffId;
                int evtCutNo = event_manager.getMyActIdx(staffId, mEvtCutNameList, 3, FALSE, FALSE);
                JUT_ASSERT(1410, (0 <= evtCutNo) && (evtCutNo < NUM_EVT_CUTS_e));
                JUT_ASSERT(1411, NULL != mEvtCutList[evtCutNo]);

                if ((this->*mEvtCutList[evtCutNo])(staffId)) {
                    event_manager.cutEnd(staffId);
                }

                rv = TRUE;
            }

            if (eventInfo.checkCommandDemoAccrpt() && mEventIdx != -1 && event_manager.endCheck(mEventIdx)) {
                dComIfGp_event_reset();
                mOrderEvtNo = EVT_NONE;
                mEventIdx = -1;
            }
        }

        int i_expression, i_motion;
        int msgTimer = mMsgTimer;
        if (ctrlMsgAnm(i_expression, i_motion, this, FALSE)) {
            if (!field_0x9eb) {
                setMotion(i_motion, -1.0f, 0);
            }
        }
        #if DEBUG
        else if (msgTimer != 0) {
            rv = ((field_0x9eb & 0xFF) == false);
        }
        #endif
    } else {
        mMsgTimer = 0;

        if (mStaffID != -1) {
            mAction = NULL;
            mStaffID = -1;
        }
    }

    return rv;
}

void daNpc_SoldierA_c::setLookMode(int i_lookMode) {
    if (i_lookMode >= 0 && i_lookMode < 5 && i_lookMode != mLookMode) {
        mLookMode = i_lookMode;
    }
}

void daNpc_SoldierA_c::lookat() {
    daPy_py_c* player = NULL;
    J3DModel* model = mAnm_p->getModel();
    BOOL i_snap = FALSE;
    f32 body_angleX_min = mHIO->m.common.body_angleX_min;
    f32 body_angleX_max = mHIO->m.common.body_angleX_max;
    f32 body_angleY_min = mHIO->m.common.body_angleY_min;
    f32 body_angleY_max = mHIO->m.common.body_angleY_max;
    f32 head_angleX_min = mHIO->m.common.head_angleX_min;
    f32 head_angleX_max = mHIO->m.common.head_angleX_max;
    f32 head_angleY_min = mHIO->m.common.head_angleY_min;
    f32 head_angleY_max = mHIO->m.common.head_angleY_max;
    s16 angle_delta = mCurAngle.y - mOldAngle.y;
    cXyz lookatPos[3] = {mLookatPos[0], mLookatPos[1], mLookatPos[2]};
    csXyz* lookatAngle[3] = {&mLookatAngle[0], &mLookatAngle[1], &mLookatAngle[2]};
    cXyz spe8;

    switch (mLookMode) {
        case LOOK_NONE:
            break;
        
        case LOOK_RESET:
            i_snap = TRUE;
            break;

        case LOOK_PLAYER:
        case LOOK_PLAYER_TALK:
            player = daPy_getPlayerActorClass();

            if (mLookMode == LOOK_PLAYER_TALK) {
                head_angleY_min = -80.0f;
                head_angleY_max = 80.0f;
            }
            break;

        case LOOK_ACTOR:
            player = (daPy_py_c*)mActorMngrs[1].getActorP();
            break;
    }

    if (player != NULL) {
        mLookPos = player->attention_info.position;

        if (mLookMode != LOOK_PLAYER && mLookMode != LOOK_PLAYER_TALK && mLookMode != LOOK_ACTOR) {
            mLookPos.y -= 40.0f;
        }

        mLookat.setAttnPos(&mLookPos);
    } else {
        mLookat.setAttnPos(NULL);
    }

    mLookat.setParam(body_angleX_min, body_angleX_max, body_angleY_min, body_angleY_max,
                     0.0f, 0.0f, 0.0f, 0.0f,
                     head_angleX_min, head_angleX_max, head_angleY_min, head_angleY_max,
                     mCurAngle.y, lookatPos);
    mLookat.calc(this, model->getBaseTRMtx(), lookatAngle, i_snap, angle_delta, FALSE);
}

int daNpc_SoldierA_c::wait(void* param_1) {
    switch (mMode) {
        case 0:
            setMotion(MOT_WAIT, -1.0f, 0);
            setLookMode(LOOK_NONE);
            mTurnMode = 0;
            field_0x9ea = true;
            mMode = 2;
            // fallthrough
        case 2:
            if (!mIsDamaged) {
                if (mActorMngrs[0].getActorP() != NULL) {
                    setLookMode(LOOK_PLAYER);
                } else {
                    setLookMode(LOOK_NONE);

                    if (home.angle.y != mCurAngle.y && step(home.angle.y, -1, -1, 15)) {
                        mMode = 0;
                    }
                }

                if (home.angle.y == mCurAngle.y) {
                    BOOL player_attn = mActorMngrs[0].getActorP() != NULL;
                    fopAc_ac_c* actor = getAttnActorP(player_attn, srchAttnActor1, mHIO->m.common.search_distance, mHIO->m.common.search_height,
                                                      mHIO->m.common.search_depth, mHIO->m.common.fov, shape_angle.y, 120, TRUE);
                    if (actor != NULL) {
                        mActorMngrs[1].entry(actor);
                        setLookMode(LOOK_ACTOR);
                    }
                } else {
                    mAttnChangeTimer = 0;
                }
            }
            break;

        case 3:
            break;
    }

    return 1;
}

int daNpc_SoldierA_c::talk(void* param_1) {
    int rv = 0;
    BOOL unkFlag = FALSE;

    switch (mMode) {
        case 0: {
            if (mIsDamaged) {
                break;
            }

            int msgNo = mMsgNo;
            mOrderSpeakEvt = false;
            initTalk(msgNo, NULL);
            mTurnMode = 0;
            mMode = 2;
            // fallthrough
        }
        case 2:
            if (field_0x9ea) {
                unkFlag = TRUE;
            } else {
                setLookMode(LOOK_PLAYER_TALK);
                mActorMngrs[0].entry(daPy_getPlayerActorClass());

                if (mCurAngle.y == fopAcM_searchPlayerAngleY(this)) {
                    unkFlag = TRUE;
                } else if (step(fopAcM_searchPlayerAngleY(this), -1, -1, 15)) {
                    setMotion(MOT_WAIT, -1.0f, 0);
                    mTurnMode = 0;
                }
            }

            if (unkFlag && talkProc(NULL, TRUE, NULL)) {
                rv = 1;
            }

            if (rv) {
                mMode = 3;

                if (!field_0x9ec) {
                    dComIfGp_event_reset();
                }

                field_0x9ec = false;
            }
            break;

        case 3:
            break;
    }

    return rv;
}

int daNpc_SoldierA_c::ECut_talkLake(int i_staffId) {
    dEvent_manager_c& eventManager = dComIfGp_getEventManager();
    int unused = 0;
    daPy_py_c* player = daPy_getPlayerActorClass();
    int rv = 0;
    int* piVar1 = NULL;
    int prm = -1;
    int unused2 = 0;

    piVar1 = dComIfGp_evmng_getMyIntegerP(i_staffId, "prm");
    if (piVar1 != NULL) {
        prm = *piVar1;
    }

    if (eventManager.getIsAddvance(i_staffId)) {
        switch (prm) {
            case 0:
                break;
            
            case 10:
                initTalk(mMsgNo, NULL);
                break;

            case 20:
                initTalk(mMsgNo, NULL);
                break;
        }
    }

    switch (prm) {
        case 0:
            rv = 1;
            break;
        
        case 10:
            if (talkProc(NULL, TRUE, NULL)) {
                rv = 1;
            }
            break;

        case 20:
            if (talkProc(NULL, TRUE, NULL)) {
                rv = 1;
            }
            break;

        default:
            rv = 1;
            break;
    }

    return rv;
}

int daNpc_SoldierA_c::ECut_listenLake(int i_staffId) {
    dEvent_manager_c& eventManager = dComIfGp_getEventManager();
    int unused = 0;
    daPy_py_c* player = daPy_getPlayerActorClass();
    int rv = 0;
    int* piVar1 = NULL;
    int prm = -1;
    int unused2 = 0;

    piVar1 = dComIfGp_evmng_getMyIntegerP(i_staffId, "prm");
    if (piVar1 != NULL) {
        prm = *piVar1;
    }

    if (eventManager.getIsAddvance(i_staffId)) {
        switch (prm) {
            case 0:
                break;
            
            case 10:
                setMotion(MOT_TALK_A, -1.0f, 0);
                break;

            case 20:
                break;
        }
    }

    switch (prm) {
        case 0:
            rv = 1;
            break;
        
        case 10:
            if (mMotionPhase > 0) {
                rv = 1;
            }
            break;

        case 20:
            rv = 1;
            break;

        default:
            rv = 1;
            break;
    }

    return rv;
}

int daNpc_SoldierA_c::test(void* param_1) {
    switch (mMode) {
        case 0:
            speedF = 0.0f;
            speed.setall(0.0f);
            mMode = 2;
            // fallthrough
        case 2:
            setMotion(mHIO->m.common.motion, mHIO->m.common.morf_frame, 0);
            setLookMode(mHIO->m.common.look_mode);
            mOrderEvtNo = EVT_NONE;
            attention_info.flags = 0;
            break;

        case 3:
            break;
    }

    return 1;
}

static int daNpc_SoldierA_Create(void* i_this) {
    return static_cast<daNpc_SoldierA_c*>(i_this)->create();
}

static int daNpc_SoldierA_Delete(void* i_this) {
    return static_cast<daNpc_SoldierA_c*>(i_this)->Delete();
}

static int daNpc_SoldierA_Execute(void* i_this) {
    return static_cast<daNpc_SoldierA_c*>(i_this)->Execute();
}

static int daNpc_SoldierA_Draw(void* i_this) {
    return static_cast<daNpc_SoldierA_c*>(i_this)->Draw();
}

static int daNpc_SoldierA_IsDelete(void* i_this) {
    return 1;
}

AUDIO_INSTANCES;

static actor_method_class daNpc_SoldierA_MethodTable = {
    (process_method_func)daNpc_SoldierA_Create,
    (process_method_func)daNpc_SoldierA_Delete,
    (process_method_func)daNpc_SoldierA_Execute,
    (process_method_func)daNpc_SoldierA_IsDelete,
    (process_method_func)daNpc_SoldierA_Draw,
};

extern actor_process_profile_definition g_profile_NPC_SOLDIERa = {
  fpcLy_CURRENT_e,             // mLayerID
  7,                           // mListID
  fpcPi_CURRENT_e,             // mListPrio
  PROC_NPC_SOLDIERa,           // mProcName
  &g_fpcLf_Method.base,       // sub_method
  sizeof(daNpc_SoldierA_c),    // mSize
  0,                           // mSizeOther
  0,                           // mParameters
  &g_fopAc_Method.base,        // sub_method
  399,                         // mPriority
  &daNpc_SoldierA_MethodTable, // sub_method
  0x00044100,                  // mStatus
  fopAc_NPC_e,                 // mActorType
  fopAc_CULLBOX_CUSTOM_e,      // cullType
};
