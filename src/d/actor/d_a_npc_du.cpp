/**
 * @file d_a_npc_du.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_npc_du.h"
#include "d/d_cc_d.h"
#include "d/d_s_play.h"
#include "c/c_damagereaction.h"

enum Npc_du_RES_File_ID {
    /* BCK */
    /* 0x5 */ BCK_DU_SWIM = 0x5,
    /* 0x6 */ BCK_DU_WAIT,
    /* 0x7 */ BCK_DU_WALK,

    /* BMDR */
    /* 0xA */ BMDR_DU = 0xA,
    
    /* BTP */
    /* 0xD */ BTP_DU = 0xD,
};

enum Joint {
    /* 0x00 */ JNT_WORLD_ROOT,
    /* 0x01 */ JNT_BACKBONE,
    /* 0x02 */ JNT_ARML_1,
    /* 0x03 */ JNT_ARML_2,
    /* 0x04 */ JNT_ARML_3,
    /* 0x05 */ JNT_ARMR_1,
    /* 0x06 */ JNT_ARMR_2,
    /* 0x07 */ JNT_ARMR_3,
    /* 0x08 */ JNT_NECK1,
    /* 0x09 */ JNT_NECK2,
    /* 0x0A */ JNT_HEAD,
    /* 0x0B */ JNT_JAW,
    /* 0x0C */ JNT_WAIST,
    /* 0x0D */ JNT_LEGL,
    /* 0x0E */ JNT_FOOTL,
    /* 0x0F */ JNT_LEGR1,
    /* 0x10 */ JNT_FOOTR,
    /* 0x11 */ JNT_TAIL,
};

enum Action {
    /* 0x0 */ ACTION_NORMAL,
    /* 0x1 */ ACTION_AWAY,
    /* 0x2 */ ACTION_SWIM,
    /* 0x3 */ ACTION_MESSAGE,
};

enum Action_Mode {
    /* 0x0 */ MODE_INIT,

    /* npc_du_normal */
    /* 0x1 */ NORMAL_MODE_EXECUTE,
    /* 0xA */ NORMAL_MODE_RESET = 0xA,

    /* npc_du_away */
    /* 0x1 */ AWAY_MODE_EXECUTE = 0x1,

    /* npc_du_swim */
    /* 0x1 */ SWIM_MODE_1 = 0x1,
    /* 0x2 */ SWIM_MODE_2,

    /* npc_du_message */
    /* 0x1 */ MESSAGE_MODE_1 = 0x1,
};

class daNpc_Du_HIO_c : public JORReflexible {
public:
    daNpc_Du_HIO_c();
    virtual ~daNpc_Du_HIO_c() {}
    void genMessage(JORMContext*);

    /* 0x4 */ s8 id;
    /* 0x8 */ f32 base_size;
};

daNpc_Du_HIO_c::daNpc_Du_HIO_c() {
    id = -1;
    base_size = 1.0f;
}

#if DEBUG
void daNpc_Du_HIO_c::genMessage(JORMContext* ctx) {
    ctx->genLabel("　アヒル", 0x80000001, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    ctx->genSlider("基本大きさ", &base_size, 0.0f, 3.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
}
#endif

static void anm_init(npc_du_class* i_this, int i_index, f32 i_morf, u8 i_mode, f32 i_speed) {
    i_this->mpMorf->setAnm((J3DAnmTransform*)dComIfG_getObjectRes("Npc_du", i_index), i_mode, i_morf, i_speed, 0.0f, -1.0f, NULL);
    i_this->mAnm = i_index;
}

static int nodeCallBack(J3DJoint* i_joint, int param_2) {
    if (param_2 == 0) {
        int jntNo = i_joint->getJntNo();
        J3DModel* model = j3dSys.getModel();
        npc_du_class* actor = (npc_du_class*)model->getUserArea();
        if (actor != NULL) {
            if (jntNo == JNT_NECK2) {
                MTXCopy(model->getAnmMtx(jntNo), *calc_mtx);
                
                cMtx_XrotM(*calc_mtx, actor->mNeck2XRot);
                model->setAnmMtx(jntNo, *calc_mtx);

                MTXCopy(*calc_mtx, J3DSys::mCurrentMtx);
            }

            if (jntNo == TREG_S(0)) {
                MTXCopy(model->getAnmMtx(jntNo), *calc_mtx);

                cMtx_YrotM(*calc_mtx, TREG_S(1));
                cMtx_XrotM(*calc_mtx, TREG_S(2));
                cMtx_ZrotM(*calc_mtx, TREG_S(3));
                model->setAnmMtx(jntNo, *calc_mtx);

                MTXCopy(*calc_mtx, J3DSys::mCurrentMtx);
            }

            if (jntNo == TREG_S(4)) {
                MTXCopy(model->getAnmMtx(jntNo), *calc_mtx);

                cMtx_YrotM(*calc_mtx, TREG_S(5));
                cMtx_XrotM(*calc_mtx, TREG_S(6));
                cMtx_ZrotM(*calc_mtx, TREG_S(7));
                model->setAnmMtx(jntNo, *calc_mtx);

                MTXCopy(*calc_mtx, J3DSys::mCurrentMtx);
            }
        }
    }

    return 1;
}

static int daNpc_Du_Draw(npc_du_class* i_this) {
    fopAc_ac_c* actor = (fopAc_ac_c*)&i_this->actor;
    J3DModel* model = i_this->mpMorf->getModel();

    g_env_light.settingTevStruct(0, &actor->current.pos, &actor->tevStr);
    g_env_light.setLightTevColorType_MAJI(model, &actor->tevStr);
    i_this->mpBtpAnm->entry(model->getModelData());
    i_this->mpMorf->entryDL();

    if (i_this->mAction != ACTION_SWIM) {
        cXyz pos;
        pos.set(actor->current.pos.x, actor->current.pos.y + 100.0f, actor->current.pos.z);
        i_this->mShadowKey = dComIfGd_setShadow(i_this->mShadowKey, 1, model, &pos, 400.0f, 40.0f, actor->current.pos.y,
                                                i_this->mBgc.GetGroundH(), i_this->mBgc.m_gnd, &i_this->actor.tevStr, 0, 1.0f,
                                                dDlst_shadowControl_c::getSimpleTex());
    }

    return 1;
}

static void npc_du_normal(npc_du_class* i_this) {
    fopAc_ac_c* actor = (fopAc_ac_c*)&i_this->actor;
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    cXyz pos_delta, sp48;
    f32 target = 0.0f;

    switch (i_this->mActionMode) {
        case MODE_INIT:
            i_this->mTimers[0] = cM_rndF(70.0f) + 50.0f;
            anm_init(i_this, BCK_DU_WAIT, 10.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
            i_this->mActionMode++;
            break;
        
        case NORMAL_MODE_EXECUTE:
            if (i_this->mTimers[0] == 0) {
                i_this->mActionMode = NORMAL_MODE_RESET;

                if (fopAcM_wayBgCheck(actor, 100.0f, 30.0f)) {
                    i_this->mCurrentAngleYTarget = (s16)cM_rndFX(8000.0f) + (actor->current.angle.y + 0x8000);
                    i_this->mTimers[1] = 30;
                } else {
                    pos_delta = actor->home.pos - actor->current.pos;
                    pos_delta.x += cM_rndFX(500.0f);
                    pos_delta.z += cM_rndFX(500.0f);
                    i_this->mCurrentAngleYTarget = (s16)cM_atan2s(pos_delta.x, pos_delta.z);
                }

                i_this->mTimers[0] = cM_rndF(60.0f) + 30.0f;
                anm_init(i_this, BCK_DU_WALK, 10.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
            }
            break;

        case NORMAL_MODE_RESET:
            target = WREG_F(0) + 2.0f;
            cLib_addCalcAngleS2(&actor->current.angle.y, i_this->mCurrentAngleYTarget, 4, TREG_S(2) + 0x100);

            if (i_this->mTimers[0] == 0 || (fopAcM_wayBgCheck(actor, 100.0f, 30.0f) && i_this->mTimers[1] == 0)) {
                i_this->mActionMode = MODE_INIT;
            }
            break;
    }

    cLib_addCalc2(&actor->speedF, target, 1.0f, 5.0f);
    if (!daPy_py_c::checkNowWolf() && i_this->mPlayerDist < 200.0f) {
        i_this->mAction = ACTION_AWAY;
        i_this->mActionMode = MODE_INIT;
    }
}

static void npc_du_away(npc_du_class* i_this) {
    // Fakematch???
    fopAc_ac_c* actor = (fopAc_ac_c*)&i_this->actor;
    #if PLATFORM_SHIELD
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    #endif
    cXyz pos_delta, sp5c;
    f32 target = 0.0f;
    f32 maxStepF = 2.0f;
    s16 maxStepS = 0x1800;

    #if PLATFORM_SHIELD
    pos_delta = actor->current.pos - player->current.pos;
    #else
    pos_delta = actor->current.pos - dComIfGp_getPlayer(0)->current.pos;
    #endif

    switch (i_this->mActionMode) {
        case MODE_INIT:
            anm_init(i_this, BCK_DU_WALK, 5.0f, J3DFrameCtrl::EMode_LOOP, TREG_F(9) + 2.5f);
            i_this->mActionMode++;
            i_this->mTimers[0] = 20;
            // fallthrough
        case AWAY_MODE_EXECUTE:
            target = WREG_F(1) + 8.0f;

            if (i_this->mTimers[0] == 0 && i_this->mPlayerDist > 300.0f) {
                i_this->mAction = ACTION_NORMAL;
                i_this->mActionMode = MODE_INIT;
            }

            if (i_this->mTimers[1] == 0) {
                i_this->mCurrentAngleYTarget = (s16)cM_atan2s(pos_delta.x, pos_delta.z);

                if (fopAcM_wayBgCheck(actor, 100.0f, 30.0f)) {
                    i_this->mTimers[1] = 30;
                    i_this->mCurrentAngleYTarget = (s16)cM_rndFX(10000.0f) + (actor->current.angle.y + 0x8000);
                }
            }
            break;
    }

    cLib_addCalc2(&actor->speedF, target, 1.0f, maxStepF);
    cLib_addCalcAngleS2(&actor->current.angle.y, i_this->mCurrentAngleYTarget, 2, maxStepS);
}

static void npc_du_swim(npc_du_class* i_this) {
    fopAc_ac_c* actor = (fopAc_ac_c*)&i_this->actor;
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    cXyz pos_delta, sp48;
    f32 target = 0.0f;
    s16 maxStep = 0x100;

    switch (i_this->mActionMode) {
        case MODE_INIT:
            anm_init(i_this, BCK_DU_SWIM, 10.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
            i_this->mActionMode++;
            // fallthrough
        case SWIM_MODE_1:
            if (fopAcM_wayBgCheck(actor, 100.0f, 30.0f)) {
                i_this->mCurrentAngleYTarget = (s16)cM_rndFX(8000.0f) + (actor->current.angle.y + 0x8000);
                i_this->mTimers[0] = i_this->mTimers[1] = 128;
                // i_this->mTimers[0] = 128;
            } else if (i_this->mPlayerDist < 500.0f) {
                i_this->mCurrentAngleYTarget = (s16)cM_rndFX(8000.0f) + (i_this->mPlayerAngleY + 0x8000);
                i_this->mTimers[0] = cM_rndF(10.0f) + 20.0f;
                i_this->mTimers[1] = i_this->mTimers[0];
            } else {
                pos_delta = actor->home.pos - actor->current.pos;
                pos_delta.x += cM_rndFX(500.0f);
                pos_delta.z += cM_rndFX(500.0f);
                i_this->mCurrentAngleYTarget = (s16)cM_atan2s(pos_delta.x, pos_delta.z);
                i_this->mTimers[0] = cM_rndF(60.0f) + 30.0f;
            }

            i_this->mActionMode++;
            break;
        
        case SWIM_MODE_2:
            if (i_this->mTimers[1] != 0) {
                target = TREG_F(15) + 3.0f;
                i_this->mpMorf->setPlaySpeed(1.5f);
                maxStep = 0x200;
            } else {
                target = TREG_F(15) + 2.0f;
                i_this->mpMorf->setPlaySpeed(1.0f);
            }

            cLib_addCalcAngleS2(&actor->current.angle.y, i_this->mCurrentAngleYTarget, 4, maxStep);

            if (i_this->mTimers[0] == 0 || (fopAcM_wayBgCheck(actor, 100.0f, 30.0f) && i_this->mTimers[1] == 0)) {
                i_this->mActionMode = SWIM_MODE_1;
            }
            break;
    }

    cLib_addCalc2(&actor->speedF, target, 1.0f, 1.0f);

    if (i_this->mActionTimer == 0 && i_this->mSwimFlag == 0) {
        i_this->mAction = ACTION_NORMAL;
        i_this->mActionMode = MODE_INIT;
        i_this->mActionTimer = 30;
    }
}

static void npc_du_message(npc_du_class* i_this) {
    fopAc_ac_c* actor = (fopAc_ac_c*)&i_this->actor;

    switch (i_this->mActionMode) {
        case MODE_INIT:
            if (i_this->mSwimFlag == 0) {
                anm_init(i_this, BCK_DU_WAIT, 10.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
            } else {
                anm_init(i_this, BCK_DU_SWIM, 10.0f, J3DFrameCtrl::EMode_LOOP, 0.5f);
            }

            i_this->mActionMode = MESSAGE_MODE_1;
            // fallthrough
        case MESSAGE_MODE_1:
            if (i_this->mSwimFlag == 0) {
                if (i_this->mAnm != BCK_DU_WAIT) {
                    anm_init(i_this, BCK_DU_WAIT, 10.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
                }
            } else if (i_this->mAnm != BCK_DU_SWIM) {
                anm_init(i_this, BCK_DU_SWIM, 10.0f, J3DFrameCtrl::EMode_LOOP, 0.5f);
            }
            break;
    }

    cLib_addCalc0(&actor->speedF, 1.0f, 2.0f);

    s16 maxStep;
    if (i_this->mSwimFlag == 0) {
        maxStep = 0x1000;
    } else {
        maxStep = 0x400;
    }

    cLib_addCalcAngleS2(&actor->current.angle.y, i_this->mPlayerAngleY, 2, maxStep);

    if (dComIfGp_event_runCheck() == FALSE && i_this->mPlayerDist > 220.0f) {
        if (i_this->mSwimFlag == 0) {
            i_this->mAction = ACTION_NORMAL;
        } else {
            i_this->mAction = ACTION_SWIM;
        }

        i_this->mActionMode = MODE_INIT;
    }
}

static void action(npc_du_class* i_this) {
    fopAc_ac_c* actor = (fopAc_ac_c*)&i_this->actor;
    cXyz sp9c, spa8;

    i_this->mPlayerDist = fopAcM_searchPlayerDistance(actor);
    i_this->mPlayerAngleY = fopAcM_searchPlayerAngleY(actor);
    s8 sVar1 = 0;
    i_this->field_0x980 = 0;

    switch (i_this->mAction) {
        case ACTION_NORMAL:
            npc_du_normal(i_this);
            sVar1 = 1;
            i_this->field_0x980 = 1;
            break;

        case ACTION_AWAY:
            npc_du_away(i_this);
            sVar1 = 1;
            break;

        case ACTION_SWIM:
            npc_du_swim(i_this);
            i_this->field_0x980 = 1;
            break;

        case ACTION_MESSAGE:
            npc_du_message(i_this);
            i_this->field_0x980 = 2;
            break;
    }

    if (i_this->mActionTimer == 0 && sVar1 && i_this->mSwimFlag != 0) {
        i_this->mAction = ACTION_SWIM;
        i_this->mActionMode = MODE_INIT;
        i_this->mActionTimer = 30;
        actor->speedF *= 0.5f;
        fopAcM_seStart(actor, Z2SE_DUCK_INTO_WTR, 0);
    }

    if (i_this->field_0x980 == 1 && daPy_py_c::checkNowWolf() && i_this->mPlayerDist < 150.0f) {
        i_this->mAction = ACTION_MESSAGE;
        i_this->mActionMode = MODE_INIT;
    }

    cLib_addCalcAngleS2(&actor->shape_angle.y, actor->current.angle.y, 2, 0x2000);
    cLib_addCalcAngleS2(&actor->shape_angle.x, actor->current.angle.x, 2, 0x2000);
    cMtx_YrotS(*calc_mtx, actor->current.angle.y);
    sp9c.x = 0.0f;
    sp9c.y = 0.0f;
    sp9c.z = actor->speedF;
    MtxPosition(&sp9c, &spa8);
    actor->speed.x = spa8.x;
    actor->speed.z = spa8.z;
    actor->current.pos += actor->speed;
    actor->speed.y += actor->gravity;

    if (actor->speed.y < -80.0f) {
        actor->speed.y = -80.0f;
    }

    cXyz* ccMoveP = i_this->mStts.GetCCMoveP();
    if (ccMoveP != NULL) {
        actor->current.pos.x += ccMoveP->x;
        actor->current.pos.y += ccMoveP->y;
        actor->current.pos.z += ccMoveP->z;
    }

    i_this->mBgc.CrrPos(dComIfG_Bgsp());
    dBgS_ObjGndChk_Spl gnd_chk_spl;
    i_this->mSwimFlag = 0;
    sp9c = actor->current.pos;
    sp9c.y += 100.0f;
    gnd_chk_spl.SetPos(&sp9c);
    i_this->mGroundCross = dComIfG_Bgsp().GroundCross(&gnd_chk_spl);

    if (actor->current.pos.y <= (i_this->mGroundCross - 15.0f) + WREG_F(2)) {
        cLib_addCalc2(&actor->current.pos.y, (i_this->mGroundCross - 15.0f) + WREG_F(2), 0.2f, 1.0f);
        actor->speed.y = 0.0f;
        i_this->mSwimFlag = 1;
        cXyz spb4(actor->current.pos);
        spb4.y = i_this->mGroundCross;

        f32 fVar1;
        if (actor->speedF >= 10.0f) {
            fVar1 = KREG_F(1) + 0.2f;
        } else {
            fVar1 = KREG_F(0) + 0.05f;
        }

        fopAcM_effHamonSet(&i_this->mHamonPrtcl, &spb4, KREG_F(2) + 1.0f, fVar1);
    }

    if ((i_this->mFrameCounter & 7) == 0 && cM_rndF(1.0f) < 0.3f) {
        i_this->mNeck2XRotTarget = cM_rndFX(TREG_F(13) + 5000.0f);
    }

    cLib_addCalcAngleS2(&i_this->mNeck2XRot, i_this->mNeck2XRotTarget, 2, 0x1000);
}

static BOOL message(npc_du_class* i_this) {
    fopAc_ac_c* actor = (fopAc_ac_c*)&i_this->actor;

    if (i_this->field_0x982 != 0) {
        i_this->mUnkTimer = 10;

        if (i_this->mFlow.doFlow(actor, NULL, 0)) {
            dComIfGp_event_reset();
            i_this->field_0x982 = 0;
        }

        return TRUE;
    }

    if (dComIfGp_event_runCheck() && actor->eventInfo.checkCommandTalk()) {
        #if DEBUG
        // Dear mapper in charge, please specify the message flow number
        static const char* mes = "担当マッパーさん\nメッセージフローNoを\n指定してチョーだい";
        
        if (i_this->mMsgFNo != 0) {
            i_this->mFlow.init(actor, i_this->mMsgFNo, 0, NULL);
        } else {
            i_this->mFlow.initWord(actor, mes, 0xFF, 0, NULL);
        }
        #else
        i_this->mFlow.init(actor, i_this->mMsgFNo, 0, NULL);
        #endif

        i_this->field_0x982 = 1;
        OS_REPORT("////////DU MSG FNO %d\n", i_this->mMsgFNo);
    }

    if (i_this->field_0x980 == 2 && i_this->mMsgFNo != -1 && daPy_py_c::checkNowWolf()) {
        fopAcM_OnStatus(actor, 0);
        cLib_onBit<u32>(actor->attention_info.flags, fopAc_AttnFlag_SPEAK_e | fopAc_AttnFlag_TALK_e);
        actor->eventInfo.onCondition(dEvtCnd_CANTALK_e);
    } else {
        fopAcM_OffStatus(actor, 0);
        cLib_offBit<u32>(actor->attention_info.flags, fopAc_AttnFlag_SPEAK_e | fopAc_AttnFlag_TALK_e);
    }

    return FALSE;
}

static bool l_initHIO;

static u8 lit_3757[12];

static daNpc_Du_HIO_c l_HIO;

static int daNpc_Du_Execute(npc_du_class* i_this) {
    if (cDmrNowMidnaTalk()) {
        return 1;
    }

    fopAc_ac_c* actor = (fopAc_ac_c*)&i_this->actor;
    cXyz sp2c, sp38;

    i_this->mFrameCounter++;

    for (int i = 0; i < 4; i++) {
        if (i_this->mTimers[i] != 0) {
            i_this->mTimers[i]--;
        }
    }

    if (i_this->mUnkTimer != 0) {
        i_this->mUnkTimer--;
    }

    if (i_this->mActionTimer != 0) {
        i_this->mActionTimer--;
    }

    action(i_this);

    mDoMtx_stack_c::transS(actor->current.pos.x, actor->current.pos.y, actor->current.pos.z);
    mDoMtx_stack_c::YrotM((s16)actor->shape_angle.y);
    mDoMtx_stack_c::XrotM((s16)actor->shape_angle.x);
    mDoMtx_stack_c::ZrotM(actor->shape_angle.z);
    mDoMtx_stack_c::scaleM(l_HIO.base_size, l_HIO.base_size, l_HIO.base_size);

    J3DModel* model = i_this->mpMorf->getModel();
    model->setBaseTRMtx(mDoMtx_stack_c::get());
    i_this->mpMorf->play(&actor->eyePos, 0, 0);
    i_this->mpBtpAnm->setFrame(i_this->arg0);
    i_this->mpMorf->modelCalc();

    static u32 walk_se[2] = {
        Z2SE_DUCK_SING, Z2SE_DUCK_SING_S,
    };

    static u32 run_se[2] = {
        Z2SE_DUCK_CRY, Z2SE_DUCK_CRY_S,
    };

    if (i_this->mAnm == BCK_DU_WALK && i_this->mpMorf->checkFrame(0.0f)) {
        if (i_this->mAction == ACTION_AWAY) {
            fopAcM_seStart(actor, run_se[i_this->arg0], 0);
        } else {
            fopAcM_seStart(actor, walk_se[i_this->arg0], 0);
        }
    } else if (i_this->mAnm == BCK_DU_SWIM && i_this->mpMorf->checkFrame(0.0f)) {
        fopAcM_seStart(actor, Z2SE_DUCK_SWIM, 0);
    }

    MTXCopy(model->getAnmMtx(JNT_HEAD), *calc_mtx);
    sp2c.set(0.0f, 0.0f, 0.0f);
    MtxPosition(&sp2c, &actor->eyePos);
    actor->attention_info.position = actor->eyePos;
    actor->attention_info.position.y += 30.0f;

    i_this->mSph.SetC(actor->current.pos);
    i_this->mSph.SetR(l_HIO.base_size * 30.0f);
    dComIfG_Ccsp()->Set(&i_this->mSph);
    i_this->mSound.framework(0, dComIfGp_getReverb(fopAcM_GetRoomNo(actor)));

    message(i_this);

    return 1;
}

static int daNpc_Du_IsDelete(npc_du_class* i_this) {
    return 1;
}

static int daNpc_Du_Delete(npc_du_class* i_this) {
    fopAc_ac_c* actor = (fopAc_ac_c*)&i_this->actor;
    fpc_ProcID id = fopAcM_GetID(i_this);

    dComIfG_resDelete(&i_this->mPhase, "Npc_Du");

    if (i_this->mIsFirstSpawn) {
        l_initHIO = 0;
        mDoHIO_DELETE_CHILD(l_HIO.id);
    }

    if (actor->heap != NULL) {
        i_this->mSound.deleteObject();
    }

    return 1;
}

static int useHeapInit(fopAc_ac_c* actor) {
    npc_du_class* i_this = (npc_du_class*)actor;

    i_this->mpMorf = new mDoExt_McaMorf((J3DModelData*)dComIfG_getObjectRes("Npc_Du", 0xA), NULL, NULL,
                                        (J3DAnmTransform*)dComIfG_getObjectRes("Npc_Du", 6), 0, 1.0f, 0, -1, 1, NULL,
                                        J3DMdlFlag_DifferedDLBuffer, 0x11020084);
    if (i_this->mpMorf == NULL || i_this->mpMorf->getModel() == NULL) {
        return 0;
    }

    J3DModel* model = i_this->mpMorf->getModel();
    model->setUserArea((uintptr_t)actor);
    for (u16 i = 0; i < model->getModelData()->getJointNum(); i++) {
        model->getModelData()->getJointNodePointer(i)->setCallBack(nodeCallBack);
    }

    i_this->mpBtpAnm = new mDoExt_btpAnm();
    if (i_this->mpBtpAnm == NULL) {
        return 0;
    }

    if (
        !i_this->mpBtpAnm->init(i_this->mpMorf->getModel()->getModelData(),
                                (J3DAnmTexPattern*)dComIfG_getObjectRes("Npc_Du", 0xD), 1, J3DFrameCtrl::EMode_NONE,
                                1.0f, 0, -1)
    ) {
        return 0;
    }

    return 1;
}

static cPhs__Step daNpc_Du_Create(fopAc_ac_c* a_this) {
    npc_du_class* i_this = (npc_du_class*)a_this;
    fopAcM_ct(a_this, npc_du_class);

    cPhs__Step phase = (cPhs__Step)dComIfG_resLoad(&i_this->mPhase, "Npc_Du");
    if (phase == cPhs_COMPLEATE_e) {
        OS_REPORT("NPC_DU PARAM %x\n", fopAcM_GetParam(a_this));
        i_this->arg0 = fopAcM_GetParam(a_this);
        i_this->arg1 = (fopAcM_GetParam(a_this) & 0xFF00) >> 8;
        i_this->mMsgFNo = a_this->current.angle.x;
        a_this->current.angle.x = a_this->shape_angle.x = 0;

        if (i_this->arg0 == 1) {
            i_this->mMsgFNo = 0x2EF;
        } else {
            i_this->mMsgFNo = 0x2F0;
        }

        OS_REPORT("NPC_DU//////////////NPC_DU SET 1 !!\n");

        if (!fopAcM_entrySolidHeap(a_this, useHeapInit, 0x1280)) {
            OS_REPORT("//////////////NPC_DU SET NON !!\n");
            return cPhs_ERROR_e;
        }

        OS_REPORT("//////////////NPC_DU SET 2 !!\n");
        if (!l_initHIO) {
            i_this->mIsFirstSpawn = 1;
            l_initHIO = true;
            l_HIO.id = mDoHIO_CREATE_CHILD("アヒル", &l_HIO);
        }

        i_this->mAction = ACTION_NORMAL;

        fopAcM_SetMtx(a_this, i_this->mpMorf->getModel()->getBaseTRMtx());
        i_this->mBgc.Set(fopAcM_GetPosition_p(a_this), fopAcM_GetOldPosition_p(a_this), a_this, 1, &i_this->mAcchCir,
                         fopAcM_GetSpeed_p(a_this), NULL, NULL);
        i_this->mAcchCir.SetWall(20.0f, 25.0f);
        i_this->mStts.Init(100, 0, a_this);

        static dCcD_SrcSph cc_sph_src = {
            {
                {0x0, {{0x0, 0x0, 0x0}, {0x0, 0x0}, 0x75}}, // mObj
                {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0}, // mGObjAt
                {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x2}, // mGObjTg
                {0x0}, // mGObjCo
            }, // mObjInf
            {
                {{0.0f, 0.0f, 0.0f}, 20.0f} // mSph
            } // mSphAttr
        };

        i_this->mSph.Set(cc_sph_src);
        i_this->mSph.SetStts(&i_this->mStts);
        i_this->mFrameCounter = cM_rndF(65536.0f);
        a_this->gravity = -5.0f;
        daNpc_Du_Execute(i_this);
    }

    return phase;
}

static actor_method_class l_daNpc_Du_Method = {
    (process_method_func)daNpc_Du_Create,
    (process_method_func)daNpc_Du_Delete,
    (process_method_func)daNpc_Du_Execute,
    (process_method_func)daNpc_Du_IsDelete,
    (process_method_func)daNpc_Du_Draw,
};

extern actor_process_profile_definition g_profile_NPC_DU = {
  fpcLy_CURRENT_e,       // mLayerID
  7,                     // mListID
  fpcPi_CURRENT_e,       // mListPrio
  PROC_NPC_DU,           // mProcName
  &g_fpcLf_Method.base, // sub_method
  sizeof(npc_du_class),  // mSize
  0,                     // mSizeOther
  0,                     // mParameters
  &g_fopAc_Method.base,  // sub_method
  722,                   // mPriority
  &l_daNpc_Du_Method,    // sub_method
  0x08044100,            // mStatus
  fopAc_NPC_e,           // mActorType
  fopAc_CULLBOX_0_e,     // cullType
};
