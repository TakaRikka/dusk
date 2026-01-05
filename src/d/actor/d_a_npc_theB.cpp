/**
 * d_a_npc_theB.cpp
 * Telma B
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_npc_theB.h"
#include "JSystem/JHostIO/JORFile.h"
#include "d/actor/d_a_e_wb.h"
#include "d/actor/d_a_npc_coach.h"
#include "d/actor/d_a_startAndGoal.h"
#include "d/actor/d_a_coach_2D.h"

enum Coach_RES_File_ID {
    /* BCK */
    /* 0x09 */ BCK_HU_JUMP_END = 0x9,
    /* 0x0A */ BCK_HU_JUMP_MIDDLE,
    /* 0x0B */ BCK_HU_JUMP_START,
    /* 0x0C */ BCK_HU_LASH,
    /* 0x0D */ BCK_HU_RUN_FAST,
    /* 0x0E */ BCK_HU_RUN_SLOW,
    /* 0x0F */ BCK_HU_WAIT_01,
    /* 0x10 */ BCK_HU_WALK_FAST,
    /* 0x11 */ BCK_HU_WALK_SLOW,
    /* 0x12 */ BCK_THEB_BEND_TURN,
    /* 0x13 */ BCK_THEB_BEND_WAIT,
    /* 0x14 */ BCK_THEB_F_BEND_TURN,
    /* 0x15 */ BCK_THEB_F_TALK_A,
    /* 0x16 */ BCK_THEB_F_TALK_B,
    /* 0x17 */ BCK_THEB_F_TALK_C,
    /* 0x18 */ BCK_THEB_FH_BEND_WAIT,
    /* 0x19 */ BCK_THEB_FH_TALK_B,
    /* 0x1A */ BCK_THEB_FH_TALK_C,
    /* 0x1B */ BCK_THEB_SIT,
    /* 0x1C */ BCK_THEB_SIT_B,
    /* 0x1D */ BCK_THEB_WHIP,
    /* 0x1E */ BCK_THEB_WHIP_B,
    /* 0x1F */ BCK_YELIA_BEND,
    /* 0x20 */ BCK_YELIA_WAIT,

    /* BMDR */
    /* 0x23 */ BMDR_COACH = 0x23,
    /* 0x24 */ BMDR_HARNESS,
    /* 0x25 */ BMDR_HORSE,
    /* 0x26 */ BMDR_THEB,
    /* 0x27 */ BMDR_YELIA,

    /* BTK */
    /* 0x2A */ BTK_THEB = 0x2A,

    /* BTP */
    /* 0x2D */ BTP_HU_EYE = 0x2D,
    /* 0x2E */ BTP_THEB,
    /* 0x2F */ BTP_THEB_F_TALK_A,

    /* DZB */
    /* 0x32 */ DZB_COACH = 0x32,
    /* 0x33 */ DZB_HARNESS,

    /* EVT */
    /* 0x36 */ EVT_EVENT_LIST = 0x36,

    /* TEX */
    /* 0x39 */ TEX_TAZUNA = 0x39,
};

enum Joint {
    /* 0x00 */ JNT_CENTER,
    /* 0x01 */ JNT_BACKBONE1,
    /* 0x02 */ JNT_BACKBONE2,
    /* 0x03 */ JNT_ARML1,
    /* 0x04 */ JNT_ARML2,
    /* 0x05 */ JNT_HANDL,
    /* 0x06 */ JNT_ARMR1,
    /* 0x07 */ JNT_ARMR2,
    /* 0x08 */ JNT_HANDR,
    /* 0x09 */ JNT_NECK,
    /* 0x0A */ JNT_HEAD,
    /* 0x0B */ JNT_CHIN,
    /* 0x0C */ JNT_HAIR,
    /* 0x0D */ JNT_MOUTH,
    /* 0x0E */ JNT_WAIST,
    /* 0x0F */ JNT_LEGL2,
    /* 0x10 */ JNT_LEGR2,
};

enum Event {
    /* 0x0 */ EVT_NONE,
    /* 0x1 */ EVT_PERSONAL_COMBAT_INTRO,
    /* 0x2 */ EVT_PERSONAL_COMBAT_REVENGE,
    /* 0x3 */ EVT_ANNULATION_FIELD_RACE,
    /* 0x4 */ EVT_THEB_HINT,
    /* 0x5 */ EVT_COACH_GUARD_GAMEOVER,
};

static NPC_THEB_HIO_CLASS l_HIO;

static u16 l_bmdGetParamList[1][2] = {
    {0, BMDR_THEB},
};

static daNpc_GetParam1 l_bckGetParamList[14] = {
    {-1, J3DFrameCtrl::EMode_LOOP},
    {BCK_THEB_F_TALK_A, J3DFrameCtrl::EMode_NONE},
    {BCK_THEB_F_TALK_B, J3DFrameCtrl::EMode_NONE},
    {BCK_THEB_F_TALK_C, J3DFrameCtrl::EMode_NONE},
    {BCK_THEB_F_BEND_TURN, J3DFrameCtrl::EMode_NONE},
    {BCK_THEB_FH_TALK_B, J3DFrameCtrl::EMode_LOOP},
    {BCK_THEB_FH_TALK_C, J3DFrameCtrl::EMode_LOOP},
    {BCK_THEB_FH_BEND_WAIT, J3DFrameCtrl::EMode_LOOP},
    {BCK_THEB_SIT, J3DFrameCtrl::EMode_LOOP},
    {BCK_THEB_SIT_B, J3DFrameCtrl::EMode_LOOP},
    {BCK_THEB_WHIP, J3DFrameCtrl::EMode_NONE},
    {BCK_THEB_WHIP_B, J3DFrameCtrl::EMode_NONE},
    {BCK_THEB_BEND_WAIT, J3DFrameCtrl::EMode_LOOP},
    {BCK_THEB_BEND_TURN, J3DFrameCtrl::EMode_NONE},
};

static daNpc_GetParam1 l_btpGetParamList[1] = {
    {BTP_THEB, J3DFrameCtrl::EMode_LOOP},
};

static daNpc_GetParam1 l_btkGetParamList[1] = {
    {BTK_THEB, J3DFrameCtrl::EMode_LOOP},
};

static char* l_evtNames[6] = {
    NULL,
    "PERSONAL_COMBAT_INTRO",
    "PERSONAL_COMBAT_REVENGE",
    "ANNULATION_FIELD_RACE",
    "THEB_HINT",
    "COACH_GUARD_GAMEOVER",
};

static char* l_arcName = "Coach";

static char* l_myName = "TheB";

#if DEBUG
daNpcTheB_HIO_c::daNpcTheB_HIO_c() {
    m = daNpcTheB_Param_c::m;
}

void daNpcTheB_HIO_c::listenPropertyEvent(const JORPropertyEvent* event) {
    char buffer[2000];
    
    JORReflexible::listenPropertyEvent(event);

    JORFile jorFile;
    int len;

    switch ((u32)event->id) {
        case 0x40000002:
            if (jorFile.open(6, "", NULL, NULL, NULL)) {
                memset(buffer, 0, sizeof(buffer));
                len = 0;
                daNpcF_commonListenPropertyEvent(buffer, &len, &m.common);
                jorFile.writeData(buffer, len);
                jorFile.close();
                OS_REPORT("Write append success!::%6d\n", len);
            } else {
                OS_REPORT("Write append failure!\n");
            }
            break;
    }
}

void daNpcTheB_HIO_c::genMessage(JORMContext* ctx) {
    daNpcF_commonGenMessage(ctx, &m.common);
    ctx->genButton("ファイル書き出し", 0x40000002, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
}
#endif

daNpcTheB_c::cutFunc daNpcTheB_c::mEvtSeqList[6] = {
    NULL,
    &daNpcTheB_c::EvCut_PersonalCombatIntro,
    &daNpcTheB_c::EvCut_PersonalCombatRevenge,
    &daNpcTheB_c::EvCut_AnnulationFieldRace,
    &daNpcTheB_c::EvCut_TheBHint,
    &daNpcTheB_c::EvCut_CoachGuardGameOver
};

daNpcTheB_c::daNpcTheB_c() {}

daNpcTheB_c::~daNpcTheB_c() {
    dComIfG_resDelete(&mPhase, l_arcName);

    if (heap != NULL) {
        mAnm_p->stopZelAnime();
    }

    #if DEBUG
    if (mpHIO != NULL) {
        mpHIO->removeHIO();
    }
    #endif
}

daNpcTheB_HIOParam const daNpcTheB_Param_c::m = {
    115.0f,
    -3.0f,
    1.0f,
    400.0f,
    255.0f,
    115.0f,
    35.0f,
    40.0f,
    0.0f,
    0.0f,
    30.0f,
    -30.0f,
    45.0f,
    -20.0f,
    45.0f,
    -45.0f,
    0.6f,
    12.0f,
    0xB,
    3,
    0xF,
    3,
    90.0f,
    800.0f,
    600.0f,
    -400.0f,
    60,
    8,
    0,
    0,
    0,
    false,
    false,
};

cPhs__Step daNpcTheB_c::create() {
    fopAcM_ct(this, daNpcTheB_c);

    cPhs__Step phase = (cPhs__Step)dComIfG_resLoad(&mPhase, l_arcName);
    if (phase == cPhs_COMPLEATE_e) {
        if (!fopAcM_entrySolidHeap(this, createHeapCallBack, 0x2CB0)) {
            return cPhs_ERROR_e;
        }

        if (getMessageNo() != 0xFFFF) {
            mMsgNo = getMessageNo();
        } else {
            mMsgNo = -1;
        }

        fopAcM_setStageLayer(this);

        J3DModelData* mdlData_p = mAnm_p->getModel()->getModelData();
        fopAcM_SetMtx(this, mAnm_p->getModel()->getBaseTRMtx());
        fopAcM_setCullSizeBox(this, -50.0f, -60.0f, -50.0f, 50.0f, 130.0f, 70.0f);
        mSound.init(&current.pos, &eyePos, 3, 1);

        #if DEBUG
        mpHIO = &l_HIO;
        mpHIO->entryHIO("馬車テルマ");
        #endif

        mAcchCir.SetWall(mpHIO->m.common.width, mpHIO->m.common.knee_length);
        mAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1, &mAcchCir,
                  fopAcM_GetSpeed_p(this), fopAcM_GetAngle_p(this), fopAcM_GetShapeAngle_p(this));
        mAcch.CrrPos(dComIfG_Bgsp());
        mGndChk = mAcch.m_gnd;
        mGroundH = mAcch.GetGroundH();
        mCcStts.Init(mpHIO->m.common.weight, 0, this);
        mCyl.Set(mCcDCyl);
        mCyl.SetStts(&mCcStts);
        mCyl.SetH(mpHIO->m.common.height);
        mCyl.SetR(mpHIO->m.common.width);

        setEnvTevColor();
        setRoomNo();
        reset();
        Execute();

        return cPhs_COMPLEATE_e;
    }

    return phase;
    
}

int daNpcTheB_c::CreateHeap() {
    J3DModelData* mdlData_p = NULL;
    int unused = 0;
    int unused2 = 0;

    mdlData_p = (J3DModelData*)dComIfG_getObjectRes(l_arcName, 0x26);
    JUT_ASSERT(383, NULL != mdlData_p);

    mAnm_p = new mDoExt_McaMorfSO(mdlData_p, NULL, NULL, (J3DAnmTransform*)dComIfG_getObjectRes(l_arcName, 0x1B),
                                  2, 1.0f, 0, -1, &mSound, J3DMdlFlag_DifferedDLBuffer, 0x11020284);
    if (mAnm_p != NULL && mAnm_p->getModel() == NULL) {
        mAnm_p->stopZelAnime();
        mAnm_p = NULL;
    }

    if (mAnm_p == NULL) {
        return 0;
    }

    mdlData_p->getJointNodePointer(JNT_CENTER)->setCallBack(ctrlJointCallBack);
    mdlData_p->getJointNodePointer(JNT_BACKBONE1)->setCallBack(ctrlJointCallBack);
    mdlData_p->getJointNodePointer(JNT_NECK)->setCallBack(ctrlJointCallBack);
    mdlData_p->getJointNodePointer(JNT_HEAD)->setCallBack(ctrlJointCallBack);
    mdlData_p->getJointNodePointer(JNT_MOUTH)->setCallBack(ctrlJointCallBack);
    mAnm_p->getModel()->setUserArea((uintptr_t)this);

    mpMatAnm = new daNpcF_MatAnm_c();
    if (mpMatAnm == NULL) {
        return 0;
    }

    if (!setExpressionAnm(ANM_FH_TALK_B, false)) {
        return 0;
    }

    setMotionAnm(ANM_SIT, 0.0f);

    return 1;
}

int daNpcTheB_c::Delete() {
    fpc_ProcID id = fopAcM_GetID(this);
    this->~daNpcTheB_c();
    return 1;
}

int daNpcTheB_c::Execute() {
    return daNpcTheB_c::main();
}

int daNpcTheB_c::Draw() {
    J3DModelData* mdlData_p = mAnm_p->getModel()->getModelData();
    mdlData_p->getMaterialNodePointer(1)->setMaterialAnm(mpMatAnm);
    return draw(FALSE, FALSE, mpHIO->m.common.real_shadow_size, NULL, FALSE);
}

int daNpcTheB_c::ctrlJoint(J3DJoint* i_joint, J3DModel* i_model) {
    int jntNo = i_joint->getJntNo();

    if (jntNo == JNT_CENTER) {
        mDoMtx_stack_c::copy(mAnm_p->getModel()->getAnmMtx(JNT_BACKBONE1));
        mDoMtx_stack_c::multVecZero(&mLookatPos[0]);
        mDoMtx_stack_c::copy(mAnm_p->getModel()->getAnmMtx(JNT_NECK));
        mDoMtx_stack_c::multVecZero(&mLookatPos[1]);
        mDoMtx_stack_c::copy(mAnm_p->getModel()->getAnmMtx(JNT_HEAD));
        mDoMtx_stack_c::multVecZero(&mLookatPos[2]);

        return 1;
    }

    int lookatJoints[3] = {JNT_BACKBONE1, JNT_NECK, JNT_HEAD};

    switch (jntNo) {
        case JNT_BACKBONE1:
        case JNT_NECK:
        case JNT_HEAD:
            mDoMtx_stack_c::copy(i_model->getAnmMtx(jntNo));
            setLookatMtx(jntNo, lookatJoints, mpHIO->m.common.neck_rotation_ratio);

            if (jntNo == JNT_BACKBONE1) {
                mDoMtx_stack_c::XYZrotM(mBackboneRotX, mBackboneRotY, mBackboneRotZ);
            }

            i_model->setAnmMtx(jntNo, mDoMtx_stack_c::get());

            MTXCopy(mDoMtx_stack_c::get(), J3DSys::mCurrentMtx);
            break;
    }

    if ((jntNo == JNT_HEAD || jntNo == JNT_MOUTH) && (mAnmFlags & ANM_PLAY_BCK)) {
        J3DAnmTransform* anm = mBckAnm.getBckAnm();
        mBckAnm.changeBckOnly(mAnm_p->getAnm());
        mAnm_p->changeAnm(anm);
    }

    return 1;
}

int daNpcTheB_c::createHeapCallBack(fopAc_ac_c* i_this) {
    daNpcTheB_c* actor = (daNpcTheB_c*)i_this;
    return actor->CreateHeap();
}

int daNpcTheB_c::ctrlJointCallBack(J3DJoint* i_joint, int param_2) {
    if (param_2 == 0) {
        J3DModel* mdl_p = j3dSys.getModel();
        daNpcTheB_c* actor = (daNpcTheB_c*)mdl_p->getUserArea();
        if (actor != NULL) {
            actor->ctrlJoint(i_joint, mdl_p);
        }
    }

    return 1;
}

cXyz daNpcTheB_c::getHandPos1(int hand) {
    int jntNo = hand != 0 ? JNT_HANDR : JNT_HANDL;

    Vec handPos[2] = {
        6.5f, 0.0f, 0.0f,
        6.5f, 0.0f, 0.0f
    };
    Vec* handPosP = hand != 0 ? &handPos[0] : &handPos[1];
    cXyz handPos1;
    mAnm_p->modelCalc();
    mDoMtx_stack_c::copy(mAnm_p->getModel()->getAnmMtx(jntNo));
    mDoMtx_stack_c::transM(handPosP->x, handPosP->y, handPosP->z);
    mDoMtx_stack_c::multVecZero(&handPos1);
    return handPos1;
}

cXyz daNpcTheB_c::getHandPos2(int hand) {
    int jntNo = hand != 0 ? JNT_HANDR : JNT_HANDL;

    Vec handPos[2] = {
        10.0f, -4.0f, -4.5f,
        10.0f, -4.0f, 4.5f,
    };
    Vec* handPosP = hand != 0 ? &handPos[0] : &handPos[1];
    cXyz handPos2;
    mAnm_p->modelCalc();
    mDoMtx_stack_c::copy(mAnm_p->getModel()->getAnmMtx(jntNo));
    mDoMtx_stack_c::transM(handPosP->x, handPosP->y, handPosP->z);
    mDoMtx_stack_c::multVecZero(&handPos2);
    return handPos2;
}

void daNpcTheB_c::setHintEvent(s32 i_msgNo, u16 param_2, int i_roomNo) {
    mHintEvtFlag = 1;
    mHintMsgNo = i_msgNo;
    field_0xe04 = param_2;
    mRoomNo = i_roomNo;
}

BOOL daNpcTheB_c::main() {
    setParam();

    if (!doEvent()) {
        doNormalAction();
    }

    if (field_0x9ee) {
        mExpressionMorfOverride = 0.0f;
        mMotionMorfOverride = 0.0f;
        field_0x9ee = false;
    }

    if (dComIfGp_event_runCheck() == FALSE || (mOrderNewEvt && dComIfGp_getEvent()->isOrderOK())) {
        mOrderEvtNo = EVT_NONE;

        if (strcmp(dComIfGp_getStartStageName(), "F_SP121") == 0) {
            if (dComIfG_play_c::getLayerNo(0) == 3) {
                if (mPersonalCombatAfterFlag == 0) {
                    mOrderEvtNo = EVT_PERSONAL_COMBAT_INTRO;
                    dComIfGs_offSaveDunSwitch(52);
                    dComIfGs_offSaveDunSwitch(53);
                    mPersonalCombatAfterFlag = 1;
                } else {
                    fopAc_ac_c* actor_p = getEvtAreaTagP(5, 0);
                    if (actor_p != NULL) {
                        f32 playerDist = fopAcM_searchPlayerDistance(actor_p);
                        f32 scale = fopAcM_GetScale_p(actor_p)->x * 5.0f;
                        if (playerDist < scale) {
                            mOrderEvtNo = EVT_PERSONAL_COMBAT_REVENGE;
                        }
                    }
                }
            } else if (mHintEvtFlag != 0) {
                mOrderEvtNo = EVT_THEB_HINT;
            } else if (mGameOver != 0) {
                mOrderEvtNo = EVT_COACH_GUARD_GAMEOVER;
            }
        }

        if (mOrderEvtNo != 0) {
            eventInfo.setArchiveName(l_arcName);
        }

        orderEvent(mOrderSpeakEvt, l_evtNames[mOrderEvtNo], 0xFFFF, 4, 0xFF, 1);
    }

    playExpression();
    playMotion();
    playAllAnm();
    setAttnPos();
    mOldAngle = mCurAngle;

    return TRUE;
}

void daNpcTheB_c::setParam() {
    actionFunc action = mNextAction;
    s16 talk_distance = mpHIO->m.common.talk_distance;
    s16 talk_angle = mpHIO->m.common.talk_angle;
    s16 attention_distance = mpHIO->m.common.attention_distance;
    s16 attention_angle = mpHIO->m.common.attention_angle;

    if (mNextAction != action) {
        for (int i = 1; i < 1; i++) {
            mActorMngrs[i].initialize();
        }
    }

    field_0xde0 = 0;
    field_0xde4 = 0;

    attention_info.distances[fopAc_attn_LOCK_e] = getDistTableIdx(attention_distance, attention_angle);
    attention_info.distances[fopAc_attn_TALK_e] = attention_info.distances[fopAc_attn_LOCK_e];
    attention_info.distances[fopAc_attn_SPEAK_e] = getDistTableIdx(talk_distance, talk_angle);
    attention_info.distances[fopAc_attn_CHECK_e] = 0x22;
    attention_info.flags = 0;

    if (getCoachSpeed() > 0.0f) {
        attention_info.flags = fopAc_AttnFlag_CHECK_e;
    } else {
        if (mMsgNo > 0 && !dComIfGs_isSaveDunSwitch(55)) {
            attention_info.flags = fopAc_AttnFlag_SPEAK_e | fopAc_AttnFlag_TALK_e;
        }
    }

    mAcchCir.SetWallR(mpHIO->m.common.width);
    mAcchCir.SetWallH(mpHIO->m.common.knee_length);
    gravity = mpHIO->m.common.gravity;
}

BOOL daNpcTheB_c::ctrlBtk() {
    if (mpMatAnm != NULL) {
        J3DAnmTextureSRTKey* btk = NULL;
        int res;

        if (l_btkGetParamList[0].fileIdx >= 0) {
            btk = getTexSRTKeyAnmP(l_arcName, l_btkGetParamList[0].fileIdx);
        }

        if (btk == mBtkAnm.getBtkAnm()) {
            mpMatAnm->setNowOffsetX(cM_ssin(mEyeAngle.y) * 0.2f);
            mpMatAnm->setNowOffsetY(cM_ssin(mEyeAngle.x) * 0.2f);
            mpMatAnm->onEyeMoveFlag();
            return TRUE;
        }

        mpMatAnm->offEyeMoveFlag();
    }

    return FALSE;
}

void daNpcTheB_c::setAttnPos() {
    mDoMtx_stack_c::copy(mAnm_p->getModel()->getBaseTRMtx());
    mDoMtx_stack_c::multVecZero(&current.pos);
    mDoMtx_MtxToRot(mDoMtx_stack_c::get(), &mCurAngle);
    shape_angle = mCurAngle;
    current.angle = shape_angle;
    mBackboneRotY = shape_angle.z;
    mBackboneRotZ = shape_angle.x;

    if ((mAnmFlags & ANM_PLAY_BCK)) {
        mBckAnm.getBckAnm()->setFrame(mBckAnm.getFrame());
        mAnm_p->modelCalc();
    } else {
        mAnm_p->modelCalc();
    }

    lookat();

    cXyz sp2c(-18.0f, 6.0f, 0.0f);
    mDoMtx_stack_c::copy(mAnm_p->getModel()->getAnmMtx(0xA));
    mDoMtx_stack_c::multVecZero(&mHeadPos);
    mDoMtx_stack_c::multVec(&sp2c, &eyePos);
    sp2c.x = 0.0f;
    mDoMtx_stack_c::multVec(&sp2c, &sp2c);
    mHeadAngle.x = cLib_targetAngleX(&mHeadPos, &sp2c);
    mHeadAngle.y = cLib_targetAngleY(&mHeadPos, &sp2c);

    cXyz* attnPos = mLookat.getAttnPos();
    if (attnPos != NULL) {
        cXyz sp38 = *attnPos - eyePos;
        mEyeAngle.y = -mLookatAngle[2].y - mCurAngle.y;
        mEyeAngle.y += cM_atan2s(sp38.x, sp38.z);
        mEyeAngle.x = -cM_atan2s(sp38.y, sp38.absXZ());
    } else {
        mEyeAngle.y = 0;
        mEyeAngle.x = 0;
    }

    attention_info.position.set(current.pos.x, current.pos.y + mpHIO->m.common.attention_offset, current.pos.z);
    mCyl.SetC(current.pos);

    #if DEBUG
    mCyl.SetH(mpHIO->m.common.height);
    mCyl.SetR(mpHIO->m.common.width);
    #endif

    dComIfG_Ccsp()->Set(&mCyl);
}

bool daNpcTheB_c::setExpressionAnm(int i_idx, bool i_modify) {
    mAnmFlags &= ~ANM_EXPRESSION_FLAGS;

    J3DAnmTransform* anm = NULL;
    int attr = J3DFrameCtrl::EMode_NONE;

    if (l_bckGetParamList[i_idx].fileIdx >= 0) {
        anm = getTrnsfrmKeyAnmP(l_arcName, l_bckGetParamList[i_idx].fileIdx);
        attr = l_bckGetParamList[i_idx].arcIdx;
    }

    bool res = false;
    
    switch (i_idx) {
        case ANM_NONE:
            res = setExpressionBtp(EXPR_BTP_THEB);
            break;

        case ANM_F_TALK_A:
            res = setExpressionBtp(EXPR_BTP_THEB);
            break;

        case ANM_F_TALK_B:
            res = setExpressionBtp(EXPR_BTP_THEB);
            break;

        case ANM_F_TALK_C:
            res = setExpressionBtp(EXPR_BTP_THEB);
            break;

        case ANM_F_BEND_TURN:
            res = setExpressionBtp(EXPR_BTP_THEB);
            break;

        case ANM_FH_TALK_B:
            res = setExpressionBtp(EXPR_BTP_THEB);
            break;

        case ANM_FH_TALK_C:
            res = setExpressionBtp(EXPR_BTP_THEB);
            break;

        case ANM_FH_BEND_WAIT:
            res = setExpressionBtp(EXPR_BTP_THEB);
            break;

        default:
            anm = NULL;
            break;
    }

    if (!res) {
        return false;
    }

    if (anm == NULL) {
        return true;
    }

    if (setBckAnm(anm, 1.0f, attr, 0, -1, i_modify)) {
        mAnmFlags |= ANM_PAUSE_BCK | ANM_PLAY_BCK;
        mExpressionLoops = 0;
        OS_REPORT("%s: 表情Bckアニメーションを登録しました！\n", __FILE__);
        return true;
    }

    OS_REPORT("%s: 表情Bckアニメーションの登録に失敗しました！\n", __FILE__);
    return false;
}

bool daNpcTheB_c::setExpressionBtp(int i_idx) {
    J3DAnmTexPattern* btp = NULL;
    int attr = J3DFrameCtrl::EMode_NONE;

    mAnmFlags &= ~(ANM_PAUSE_BTP | ANM_PLAY_BTP | ANM_FLAG_800);

    if (l_btpGetParamList[i_idx].fileIdx >= 0) {
        btp = getTexPtrnAnmP(l_arcName, l_btpGetParamList[i_idx].fileIdx);
        attr = l_btpGetParamList[i_idx].arcIdx;
    }

    switch (i_idx) {
        case EXPR_BTP_THEB:
            attr = J3DFrameCtrl::EMode_LOOP;
            break;

        default:
            btp = NULL;
            break;
    }

    if (btp == NULL) {
        return true;
    }

    if (setBtpAnm(btp, mAnm_p->getModel()->getModelData(), 1.0f, attr)) {
        mAnmFlags |= ANM_PAUSE_BTP | ANM_PLAY_BTP;
        if (i_idx == 0) {
            mAnmFlags |= ANM_FLAG_800;
        }

        return true;
    }

    OS_REPORT("%s: 表情Btpアニメーションの登録に失敗しました！\n", __FILE__);

    return false;
}

void daNpcTheB_c::setMotionAnm(int i_idx, f32 i_morf) {
    J3DAnmTransformKey* anm = NULL;
    int attr = J3DFrameCtrl::EMode_LOOP;

    mAnmFlags &= ~ANM_MOTION_FLAGS;

    if (l_bckGetParamList[i_idx].fileIdx >= 0) {
        anm = getTrnsfrmKeyAnmP(l_arcName, l_bckGetParamList[i_idx].fileIdx);
        attr = l_bckGetParamList[i_idx].arcIdx;
    }

    J3DAnmTextureSRTKey* btk = NULL;
    int btkAttr = J3DFrameCtrl::EMode_LOOP;

    switch (i_idx) {
        case ANM_SIT:
        case ANM_WHIP:
        case ANM_WHIP_B:
            break;
            
        case ANM_SIT_B:
            setExpressionAnm(ANM_FH_TALK_B, true);
            break;
        
        case ANM_BEND_WAIT:
            setExpressionAnm(ANM_FH_BEND_WAIT, true);
            break;

        case ANM_BEND_TURN:
            setExpressionAnm(ANM_F_BEND_TURN, true);
            break;

        default:
            anm = NULL;
            btk = NULL;
            break;
    }

    if (l_btkGetParamList[0].fileIdx >= 0) {
        btk = getTexSRTKeyAnmP(l_arcName, l_btkGetParamList[0].fileIdx);
        btkAttr = l_btkGetParamList[0].arcIdx;
    }

    if (btk != NULL) {
        if (setBtkAnm(btk, mAnm_p->getModel()->getModelData(), 1.0f, btkAttr)) {
            mAnmFlags |= ANM_PLAY_BTK | ANM_PAUSE_BTK;
        }
    }

    if (anm != NULL && setMcaMorfAnm(anm, 1.0f, i_morf, attr, 0, -1)) {
        mAnmFlags |= ANM_PLAY_MORF | ANM_PAUSE_MORF;
        mMotionLoops = 0;
    }
}

BOOL daNpcTheB_c::drawDbgInfo() {
    return FALSE;
}

void daNpcTheB_c::reset() {
    initialize();

    if (strcmp(dComIfGp_getStartStageName(), "F_SP123") == 0) {
        fopAcM_OnStatus(this, fopAcM_STATUS_UNK_0x4000);
    }

    mpMatAnm->initialize();
    mLookat.initialize();
    for (int i = 0; i < 1; i++) {
        mActorMngrs[i].initialize();
    }

    mNextAction = NULL;
    mAction = NULL;
    field_0xde0 = 0;
    field_0xde4 = 0;
    field_0xde8 = 0;
    field_0xdec = 0;
    mLookMode = -1;
    mMode = 0;
    mPersonalCombatAfterFlag = 0;
    mPersonalCombatAfterMode = 0;
    current.pos = home.pos;
    old.pos = current.pos;
    current.angle.set(0, home.angle.y, 0);
    old.angle = current.angle;
    shape_angle = current.angle;
    mCurAngle = current.angle;
    mOldAngle = mCurAngle;
    speedF = 0.0f;
    speed.setall(0.0f);
    mUnkFlag = dComIfGs_isSaveDunSwitch(55);
    daNpcF_c::setMtx();
    mNextAction = &daNpcTheB_c::wait;
    field_0x9ee = true;
}

void daNpcTheB_c::playExpression() {
    daNpcF_anmPlayData dat0 = {ANM_F_TALK_A, mpHIO->m.common.morf_frame, 1};
    daNpcF_anmPlayData* pDat0[1] = {&dat0};
    daNpcF_anmPlayData dat1a = {ANM_F_TALK_B, mpHIO->m.common.morf_frame, 1};
    daNpcF_anmPlayData dat1b = {ANM_FH_TALK_B, mpHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat1[2] = {&dat1a, &dat1b};
    daNpcF_anmPlayData dat2a = {ANM_F_TALK_C, mpHIO->m.common.morf_frame, 1};
    daNpcF_anmPlayData dat2b = {ANM_FH_TALK_C, mpHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat2[2] = {&dat2a, &dat2b};
    daNpcF_anmPlayData dat3 = {ANM_NONE, mpHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat3[1] = {&dat3};

    daNpcF_anmPlayData** ppDat[4] = {
        pDat0,
        pDat1,
        pDat2,
        pDat3,
    };

    if (mExpression >= 0 && mExpression < 4) {
        playExpressionAnm(ppDat);
    }
}

void daNpcTheB_c::playMotion() {
    daNpcF_anmPlayData dat0 = {ANM_SIT, mpHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat0[1] = {&dat0};
    daNpcF_anmPlayData dat1 = {ANM_SIT_B, mpHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat1[1] = {&dat1};
    daNpcF_anmPlayData dat2 = {ANM_BEND_WAIT, mpHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat2[1] = {&dat2};
    daNpcF_anmPlayData dat3a = {ANM_WHIP, mpHIO->m.common.morf_frame, 1};
    daNpcF_anmPlayData dat3b = {ANM_SIT, 0.0f, 0};
    daNpcF_anmPlayData* pDat3[2] = {&dat3a, &dat3b};
    daNpcF_anmPlayData dat4a = {ANM_WHIP_B, mpHIO->m.common.morf_frame, 1};
    daNpcF_anmPlayData dat4b = {ANM_SIT_B, 0.0f, 0};
    daNpcF_anmPlayData* pDat4[2] = {&dat4a, &dat4b};
    daNpcF_anmPlayData dat5a = {ANM_BEND_TURN, mpHIO->m.common.morf_frame, 1};
    daNpcF_anmPlayData dat5b = {ANM_BEND_WAIT, 0.0f, 0};
    daNpcF_anmPlayData* pDat5[2] = {&dat5a, &dat5b};

    daNpcF_anmPlayData** ppDat[6] = {
        pDat0,
        pDat1,
        pDat2,
        pDat3,
        pDat4,
        pDat5,
    };

    if (mMotion >= 0 && mMotion < 6) {
        playMotionAnm(ppDat);
    }
}

void daNpcTheB_c::doNormalAction() {
    mOrderEvtNo = EVT_NONE;

    if (mNextAction) {
        if (mAction == mNextAction) {
            (this->*mAction)(NULL);
        } else {
            setAction(mNextAction);
        }
    }
}

BOOL daNpcTheB_c::doEvent() {
    fopAc_ac_c* actor_p;
    e_wb_class* wb_p;
    BOOL rv = FALSE;
    s32 staffId;
    
    if (dComIfGp_event_runCheck()) {
        if (eventInfo.checkCommandTalk()) {
            if (chkAction(&daNpcTheB_c::talk)) {
                (this->*mAction)(NULL);
            } else if (dComIfGp_event_chkTalkXY() == FALSE || dComIfGp_evmng_ChkPresentEnd()) {
                setAction(&daNpcTheB_c::talk);

                fopAc_ac_c* actor_p = fopAcM_SearchByID(parentActorID);
                if (actor_p != NULL) {
                    fopAcM_OnStatus(actor_p, fopAcM_STATUS_UNK_0x800);
                }
            }

            rv = TRUE;
        } else {
            dEvent_manager_c* eventManager = &dComIfGp_getEventManager();
            staffId = eventManager->getMyStaffId(l_myName, NULL, 0);
            if (staffId != -1) {
                mStaffID = staffId;
                JUT_ASSERT(1357, NULL != mEvtSeqList[mOrderEvtNo]);
                
                actor_p = fopAcM_SearchByID(parentActorID);
                if (actor_p != NULL) {
                    fopAcM_OnStatus(actor_p, fopAcM_STATUS_UNK_0x800);
                }

                if ((this->*mEvtSeqList[mOrderEvtNo])(staffId)) {
                    eventManager->cutEnd(staffId);
                }

                rv = TRUE;
            } else if (strcmp(dComIfGp_getStartStageName(), "F_SP123") == 0) {
                if (mPersonalCombatAfterFlag != 0) {
                    EvCut_PersonalCombatAfter();
                    rv = TRUE;
                } else {
                    wb_p = (e_wb_class*)fopAcM_SearchByName(PROC_E_WB);
                    if (wb_p->field_0x169e == 0x60 && wb_p->field_0x16a0 == 1) {
                        mPersonalCombatAfterFlag = 1;
                        EvCut_PersonalCombatAfter();
                        rv = TRUE;
                    }
                }
            }

            if (eventInfo.checkCommandDemoAccrpt() && mEventIdx != -1 && eventManager->endCheck(mEventIdx)) {
                switch (mOrderEvtNo) {
                    default:
                        break;
                }
                
                dComIfGp_event_reset();
                mOrderEvtNo = EVT_NONE;
                mEventIdx = -1;
            }
        }
    } else {
        mMsgTimer = 0;

        if (mStaffID != -1) {
            mMode = 1;
            mStaffID = -1;
        }
    }

    return rv;
}

void daNpcTheB_c::lookat() {
    daPy_py_c* player = NULL;
    J3DModel* model = mAnm_p->getModel();

    f32 body_angleX_min = mpHIO->m.common.body_angleX_min;
    f32 body_angleX_max = mpHIO->m.common.body_angleX_max;
    f32 body_angleY_min = mpHIO->m.common.body_angleY_min;
    f32 body_angleY_max = mpHIO->m.common.body_angleY_max;
    f32 head_angleX_min = mpHIO->m.common.head_angleX_min;
    f32 head_angleX_max = mpHIO->m.common.head_angleX_max;
    f32 head_angleY_min = mpHIO->m.common.head_angleY_min;
    f32 head_angleY_max = mpHIO->m.common.head_angleY_max;

    s16 angle_delta = mCurAngle.y - mOldAngle.y;

    cXyz lookat_pos[3] = {mLookatPos[0], mLookatPos[1], mLookatPos[2]};
    csXyz* lookat_angle[3] = {&mLookatAngle[0], &mLookatAngle[1], &mLookatAngle[2]};

    switch (mLookMode) {
        case LOOK_NONE:
            break;
        
        case LOOK_RESET:
        case LOOK_PLAYER:
            player = daPy_getPlayerActorClass();

            if (mLookMode == LOOK_PLAYER) {
                head_angleY_min = -80.0f;
                head_angleY_max = 80.0f;
            }
            break;
    }

    if (player != NULL) {
        mLookPos = player->attention_info.position;
        mLookat.setAttnPos(&mLookPos);
    } else {
        mLookat.setAttnPos(NULL);
    }

    mLookat.setParam(body_angleX_min, body_angleX_max, body_angleY_min, body_angleY_max, 0.0f, 0.0f,
                     0.0f, 0.0f, head_angleX_min, head_angleX_max, head_angleY_min, head_angleY_max,
                     mCurAngle.y, lookat_pos);
    mLookat.calc(this, model->getBaseTRMtx(), lookat_angle, FALSE, angle_delta, false);
}

inline void daNpcTheB_c::setWaitAnimation() {
    if (
        mAnm_p->getAnm() == getTrnsfrmKeyAnmP(l_arcName, l_bckGetParamList[8].fileIdx)
        || mAnm_p->getAnm() == getTrnsfrmKeyAnmP(l_arcName, l_bckGetParamList[10].fileIdx)
    ) {
        if (dComIfGs_isSaveDunSwitch(55)) {
            setMotion(MOT_BEND_TURN, mpHIO->m.common.morf_frame, 0);
        } else if (getCoachSpeed() > 0.0f) {
            if (mTimer-- <= 0) {
                setMotion(MOT_WHIP, 3.0f, 1);
                mTimer = 200;
            }
        } else {
            setMotion(MOT_SIT, mpHIO->m.common.morf_frame, 0);
        }
    } else if (
        mAnm_p->getAnm() == getTrnsfrmKeyAnmP(l_arcName, l_bckGetParamList[9].fileIdx) ||
        mAnm_p->getAnm() == getTrnsfrmKeyAnmP(l_arcName, l_bckGetParamList[11].fileIdx)
    ) {
        if (dComIfGs_isSaveDunSwitch(55)) {
            setMotion(MOT_BEND_TURN, mpHIO->m.common.morf_frame, 0);
        } else if (getCoachSpeed() > 0.0f) {
            if (mTimer-- <= 0) {
                setMotion(MOT_WHIP_B, 3.0f, 1);
                mTimer = 150;
            }
        } else {
            setMotion(MOT_SIT, mpHIO->m.common.morf_frame, 0);
        }
    } else if (
        mAnm_p->getAnm() == getTrnsfrmKeyAnmP(l_arcName, l_bckGetParamList[12].fileIdx) ||
        mAnm_p->getAnm() == getTrnsfrmKeyAnmP(l_arcName, l_bckGetParamList[13].fileIdx)
    ) {
        if (!dComIfGs_isSaveDunSwitch(55)) {
            if (getCoachSpeed() == 0.0f) {
                setMotion(MOT_SIT, mpHIO->m.common.morf_frame, 0);
            } else if (getCoachSpeed() < 20.0f) {
                setMotion(MOT_WHIP, mpHIO->m.common.morf_frame, 0);
            } else {
                setMotion(MOT_WHIP_B, mpHIO->m.common.morf_frame, 0);
            }
        } else if (getCoachSpeed() > 0.0f && mTimer-- <= 0) {
            setMotion(MOT_BEND_TURN, mpHIO->m.common.morf_frame, 1);
            mTimer = 100;
        }
    }
}

int daNpcTheB_c::wait(void* param_1) {
    switch (mMode) {
        case 0:
            mAttnChangeTimer = 0;
            // fallthrough
        case 1:
            setExpression(EXPR_NONE, -1.0f);
            setMotion(MOT_SIT, -1.0f, 0);
            mTurnMode = 0;
            mMode = 2;
            break;
        
        case 2: {
            bool bVar1 = mActorMngrs[0].getActorP() != NULL;

            if (!dComIfGs_isSaveDunSwitch(55)) {
                BOOL isFindPlayer2 = chkFindPlayer2(bVar1, mCurAngle.y);
                if (isFindPlayer2) {
                    if (!bVar1) {
                        mActorMngrs[0].entry(daPy_getPlayerActorClass());
                        mTurnMode = 0;
                    }

                    setLookMode(LOOK_RESET);
                    mAttnChangeTimer = 0;
                } else {
                    if (bVar1) {
                        mActorMngrs[0].remove();
                        mTurnMode = 0;
                    }

                    setLookMode(LOOK_NONE);
                }

                mUnkFlag = 0;
            } else {
                if (bVar1) {
                    mActorMngrs[0].remove();
                    mTurnMode = 0;
                }

                setLookMode(LOOK_NONE);

                if (mUnkFlag == 0) {
                    mSound.startCreatureVoice(Z2SE_THE_V_BURNING, -1);
                }

                mUnkFlag = 1;
            }

            setWaitAnimation();
            break;
        }

        case 3:
            break;
    }

    return 1;
}

void daNpcTheB_c::setMotion(int i_motion, f32 i_morf, int param_3) {
    s16 motion = i_motion;
    if ((param_3 != 0 || mMotion != motion) && i_motion >= 0 && i_motion < 6) {
        mMotion = motion;
        mMotionMorfOverride = i_morf;
        mMotionPrevPhase = -1;
        mMotionPhase = 0;
    }
}

void daNpcTheB_c::setExpression(int i_expression, f32 i_morf) {
    if (i_expression >= 0 && i_expression < 4) {
        mExpression = i_expression;
        mExpressionMorfOverride = i_morf;
        mExpressionPrevPhase = -1;
        mExpressionPhase = 0;
    }
}

int daNpcTheB_c::talk(void* param_1) {
    int rv = 0;

    switch (mMode) {
        case 0:
        case 1:
            initTalk(mMsgNo, NULL);
            setExpression(EXPR_NONE, -1.0f);
            setMotion(MOT_SIT, -1.0f, 0);
            setLookMode(LOOK_PLAYER);
            mActorMngrs[0].entry(daPy_getPlayerActorClass());
            mAttnChangeTimer = 0;
            mTurnMode = 0;
            mMode = 2;
            break;
        
        case 2:
            if (talkProc(NULL, TRUE, NULL)) {
                field_0x9ec = false;
                
                if (!field_0x9ec) {
                    dComIfGp_event_reset();
                }

                rv = 1;
                mMode = 3;
            } else {
                int msgTimer = mMsgTimer;
                int expression, motion;

                if (ctrlMsgAnm(expression, motion, this, FALSE)) {
                    if (!field_0x9eb) {
                        setExpression(expression, -1.0f);
                        setMotion(motion, -1.0f, 0);
                    }
                } else if (msgTimer != 0 && !field_0x9eb) {
                    setExpressionTalkAfter();
                }
            }
            break;

        case 3:
            break;
    }

    return rv;
}

int daNpcTheB_c::EvCut_PersonalCombatIntro(int i_staffId) {
    dEvent_manager_c& eventManager = dComIfGp_getEventManager();
    int* cutName = (int*)eventManager.getMyNowCutName(i_staffId);

    if (eventManager.getIsAddvance(i_staffId)) {
        switch (*cutName) {
            case '0x0001':
                break;
                
            case '0x0002':
                initTalk(0x16, NULL);
                setLookMode(LOOK_PLAYER);
                mActorMngrs[0].entry(daPy_getPlayerActorClass());
                break;

            default:
                JUT_ASSERT(1728, FALSE);
                break;
        }
    }

    int msgTimer = mMsgTimer;
    int expression, motion;

    if (ctrlMsgAnm(expression, motion, this, FALSE)) {
        if (!field_0x9eb) {
            setExpression(expression, -1.0f);
            setMotion(motion, -1.0f, 0);
        }
    } else if (msgTimer != 0 && !field_0x9eb) {
        setExpressionTalkAfter();
    }

    switch (*cutName) {
        case '0x0001':
            return 1;
        
        case '0x0002':
            if (talkProc(NULL, TRUE, NULL)) {
                dComIfGs_onSaveDunSwitch(52);
                dComIfGs_onSaveDunSwitch(53);
                return 1;
            }
            break;

        default:
            JUT_ASSERT(1760, FALSE);
            break;
    }

    return 0;
}

int daNpcTheB_c::EvCut_PersonalCombatRevenge(int i_staffId) {
    dEvent_manager_c& eventManager = dComIfGp_getEventManager();
    int* cutName = (int*)eventManager.getMyNowCutName(i_staffId);

    if (eventManager.getIsAddvance(i_staffId)) {
        switch (*cutName) {
            case '0x0001': {
                fopAc_ac_c* actor_p = getEvtAreaTagP(5, 0);
                cXyz* pos = dComIfGp_evmng_getMyXyzP(i_staffId, "pos");
                int* angle = dComIfGp_evmng_getMyIntegerP(i_staffId, "angle");

                if (pos != NULL && angle != NULL) {
                    daNpcCoach_c* coach_p = (daNpcCoach_c*)fopAcM_SearchByID(parentActorID);
                    if (
                        fopAcM_GetPosition(actor_p).abs2(fopAcM_GetPosition(coach_p)) > fopAcM_GetPosition(actor_p).abs2(*pos)
                    ) {
                        csXyz posAngle(0, *angle, 0);
                        coach_p->setPosAngle(*pos, posAngle);
                    }
                }

                dComIfGp_getEvent()->startCheckSkipEdge(this);
                break;
            }
            
            case '0x0002':
                initTalk(0x17, NULL);
                setLookMode(LOOK_PLAYER);
                mActorMngrs[0].entry(daPy_getPlayerActorClass());
                break;

            case '0x0003':
                break;

            default:
                JUT_ASSERT(1804, FALSE);
                break;
        }
    }

    if (dComIfGp_getEvent()->checkSkipEdge()) {
        daNpcF_clearMessageTmpBit();
        dComIfGp_getEvent()->onSkipFade();
        dComIfGp_setNextStage("F_SP123", 0, 13, 0);
    }

    int msgTimer = mMsgTimer;
    int expression, motion;

    if (ctrlMsgAnm(expression, motion, this, FALSE)) {
        if (!field_0x9eb) {
            setExpression(expression, -1.0f);
            setMotion(motion, -1.0f, 0);
        }
    } else if (msgTimer != 0 && !field_0x9eb) {
        setExpressionTalkAfter();
    }

    switch (*cutName) {
        case '0x0001':
            if (getCoachSpeed() == 0.0f) {
                return 1;
            }
            break;
        
        case '0x0002':
            if (talkProc(NULL, TRUE, NULL)) {
                return 1;
            }
            break;

        case '0x0003':
            return 1;

        default:
            JUT_ASSERT(1841, FALSE);
            break;
    }

    return 0;
}

void daNpcTheB_c::EvCut_PersonalCombatAfter() {
    daPy_py_c* player;
    daNpcCoach_c* coach_p = (daNpcCoach_c*)fopAcM_SearchByID(parentActorID);

    switch (mPersonalCombatAfterMode) {
        case 0: {
            player = daPy_getPlayerActorClass();
            cXyz sp30(-230.0f, 0.0f, -1000.0f);
            mDoMtx_stack_c::transS(*fopAcM_GetPosition_p(player));
            mDoMtx_stack_c::YrotM(fopAcM_GetShapeAngle_p(player)->y);
            mDoMtx_stack_c::multVec(&sp30, &sp30);

            if (fopAcM_gc_c::gndCheck(&sp30)) {
                sp30.y = fopAcM_gc_c::getGroundY();
            }

            coach_p->setPosAngle(sp30, *fopAcM_GetShapeAngle_p(player));
            setLookMode(LOOK_PLAYER);
            mActorMngrs[0].entry(player);
            mPersonalCombatAfterTimer = 104;
            mPersonalCombatAfterMode++;
        }
        // fallthrough
        case 1:
            coach_p->setAimSpeed(20.0f);

            if (mPersonalCombatAfterTimer-- == 0) {
                mPersonalCombatAfterMode++;
            }
            break;

        case 2:
            coach_p->setAimSpeed(0.0f);

            if (getCoachSpeed() <= 0.0f) {
                initTalk(0x18, NULL);
                mPersonalCombatAfterMode++;
            }
            break;

        case 3:
            if (talkProc(NULL, TRUE, NULL)) {
                setLookMode(LOOK_NONE);
                mActorMngrs[0].remove();
                mPersonalCombatAfterTimer = 15;
                mPersonalCombatAfterMode++;
            } else {
                int msgTimer = mMsgTimer;
                int expression, motion;

                if (ctrlMsgAnm(expression, motion, this, FALSE)) {
                    if (!field_0x9eb) {
                        setExpression(expression, -1.0f);
                        setMotion(motion, -1.0f, 0);
                    }
                } else if (msgTimer != 0 && !field_0x9eb) {
                    setExpressionTalkAfter();
                }
            }
            break;

        case 4:
            if (mPersonalCombatAfterTimer-- == 0) {
                setMotion(MOT_WHIP, mpHIO->m.common.morf_frame, 1);
                mPersonalCombatAfterTimer = 25;
                mPersonalCombatAfterMode++;
            }
            break;

        case 5:
            if (mPersonalCombatAfterTimer-- == 0) {
                mPersonalCombatAfterTimer = 250;
                mPersonalCombatAfterMode++;
            }
            break;

        case 6:
            coach_p->setAimSpeed(20.0f);

            if (mPersonalCombatAfterTimer-- == 0) {
                mPersonalCombatAfterMode++;
            }
            break;

        case 7:
            coach_p->setAimSpeed(0.0f);
            break;

        default:
            JUT_ASSERT(1928, FALSE);
            break;
    }
}

int daNpcTheB_c::EvCut_AnnulationFieldRace(int i_staffId) {
    dEvent_manager_c& eventManager = dComIfGp_getEventManager();
    daStartAndGoal_c* startAndGoal_p = (daStartAndGoal_c*)fpcM_SearchByName(PROC_START_AND_GOAL);

    if (startAndGoal_p == NULL) {
        return 0;
    }

    int* cutName = (int*)eventManager.getMyNowCutName(i_staffId);

    if (eventManager.getIsAddvance(i_staffId)) {
        switch (*cutName) {
            case '0x0001':
                if (startAndGoal_p != NULL) {
                    startAndGoal_p->readyStartTimer();
                }
                break;

            default:
                JUT_ASSERT(1955, FALSE);
                break;
        }
    }

    switch (*cutName) {
        case '0x0001':
            if (startAndGoal_p != NULL && startAndGoal_p->isStartCheck()) {
                return 1;
            }
            break;

        default:
            JUT_ASSERT(1967, FALSE);
            break;
    }

    return 0;
}

int daNpcTheB_c::EvCut_TheBHint(int i_staffId) {
    dEvent_manager_c& eventManager = dComIfGp_getEventManager();
    int* cutName = (int*)eventManager.getMyNowCutName(i_staffId);

    if (eventManager.getIsAddvance(i_staffId)) {
        switch (*cutName) {
            case '0x0001':
                setMotionAnm(ANM_SIT, 0.0f);
                break;
            
            case '0x0002':
                initTalk(mHintMsgNo, NULL);
                break;

            case '0x0003': {
                cXyz pos;
                csXyz angle;
                daNpcF_getPlayerInfoFromPlayerList(field_0xe04, mRoomNo, pos, angle);
                daPy_getPlayerActorClass()->setPlayerPosAndAngle(&pos, angle.y, 0);
                break;
            }

            default:
                JUT_ASSERT(2007, FALSE);
                break;
        }
    }

    int msgTimer = mMsgTimer;
    int expression, motion;

    if (ctrlMsgAnm(expression, motion, this, FALSE)) {
        if (!field_0x9eb) {
            setExpression(expression, -1.0f);
            setMotion(motion, -1.0f, 0);
        }
    } else if (msgTimer != 0 && !field_0x9eb) {
        setExpressionTalkAfter();
    }

    switch (*cutName) {
        case '0x0001':
        case '0x0003':
            return 1;
        
        case '0x0002':
            if (talkProc(NULL, TRUE, NULL)) {
                mHintEvtFlag = 0;
                return 1;
            }
            break;

        default:
            JUT_ASSERT(2039, FALSE);
            break;
    }

    return 0;
}

int daNpcTheB_c::EvCut_CoachGuardGameOver(int i_staffId) {
    dEvent_manager_c& eventManager = dComIfGp_getEventManager();
    int* cutName = (int*)eventManager.getMyNowCutName(i_staffId);

    if (eventManager.getIsAddvance(i_staffId)) {
        switch (*cutName) {
            case '0x0001':
                Z2GetAudioMgr()->bgmStart(Z2BGM_GAME_OVER, 0, 0);
                break;

            case '0x0002': {
                daNpcCoach_c* coach_p = (daNpcCoach_c*)fopAcM_SearchByID(parentActorID);
                cXyz pos(0.0f, -30000.0f, 0.0f);
                coach_p->setPosAngle(pos, shape_angle);
                break;
            }
            
            case '0x0003':
                ((daCoach2D_c*)fpcM_SearchByName(PROC_COACH2D))->hide();
                break;

            case '0x0004':
                break;

            default:
                JUT_ASSERT(2076, FALSE);
                break;
        }
    }

    switch (*cutName) {
        case '0x0001':
        case '0x0002':
        case '0x0003':
            return 1;
        
        case '0x0004':
            daPy_getPlayerActorClass()->onForceGameOver();
            return 1;

        default:
            JUT_ASSERT(2092, FALSE);
            break;
    }

    return 0;
}

static int daNpcTheB_Create(void* i_this) {
    return static_cast<daNpcTheB_c*>(i_this)->create();
}

static int daNpcTheB_Delete(void* i_this) {
    return static_cast<daNpcTheB_c*>(i_this)->Delete();
}

static int daNpcTheB_Execute(void* i_this) {
    return static_cast<daNpcTheB_c*>(i_this)->Execute();
}

static int daNpcTheB_Draw(void* i_this) {
    return static_cast<daNpcTheB_c*>(i_this)->Draw();
}

static int daNpcTheB_IsDelete(void* i_this) {
    return 1;
}

static actor_method_class daNpcTheB_MethodTable = {
    (process_method_func)daNpcTheB_Create,
    (process_method_func)daNpcTheB_Delete,
    (process_method_func)daNpcTheB_Execute,
    (process_method_func)daNpcTheB_IsDelete,
    (process_method_func)daNpcTheB_Draw,
};

actor_process_profile_definition g_profile_NPC_THEB = {
  fpcLy_CURRENT_e,        // mLayerID
  3,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_NPC_THEB,          // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(daNpcTheB_c),    // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  331,                    // mPriority
  &daNpcTheB_MethodTable, // sub_method
  0x00040108,             // mStatus
  fopAc_NPC_e,            // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};
