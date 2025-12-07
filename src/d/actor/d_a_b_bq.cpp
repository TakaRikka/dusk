/**
 * d_a_b_bq.cpp
 * Boss Baba (Diababa)
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_b_bq.h"
#include "d/d_com_inf_game.h"
#include "d/actor/d_a_player.h"
#include "d/d_s_play.h"
#include "f_op/f_op_camera_mng.h"
#include "f_op/f_op_msg_mng.h"
#include "m_Do/m_Do_graphic.h"
#include "d/actor/d_a_obj_ystone.h"
#include "JSystem/J3DGraphBase/J3DMaterial.h"
#include "SSystem/SComponent/c_math.h"
#include "c/c_damagereaction.h"
#include "cmath.h"

enum B_bq_RES_File_ID {
    /* BCK */
    /* 0x07 */ BCK_BQ_APPEAR = 0x7,
    /* 0x08 */ BCK_BQ_ATTACK,
    /* 0x09 */ BCK_BQ_ATTACK_A,
    /* 0x0A */ BCK_BQ_ATTACK_B,
    /* 0x0B */ BCK_BQ_ATTACK_C,
    /* 0x0C */ BCK_BQ_BOMBDAMAGE,
    /* 0x0D */ BCK_BQ_COREDAMAGE,
    /* 0x0E */ BCK_BQ_DAMAGEWAIT,
    /* 0x0F */ BCK_BQ_DEAD,
    /* 0x10 */ BCK_BQ_LOOK_M,
    /* 0x11 */ BCK_BQ_NODAMAGE,
    /* 0x12 */ BCK_BQ_RETURN01,
    /* 0x13 */ BCK_BQ_RETURN02,
    /* 0x14 */ BCK_BQ_RUNAWAY,
    /* 0x15 */ BCK_BQ_TESTMOTION,
    /* 0x16 */ BCK_BQ_TODOME,
    /* 0x17 */ BCK_BQ_WAIT01,

    /* BMDR */
    /* 0x1A */ BMDR_BQ_EYEBALL = 0x1A,

    /* BMDV */
    /* 0x1D */ BMDV_BQ = 0x1D,

    /* BRK */
    /* 0x20 */ BRK_BQ_DEAD = 0x20,

    /* BTK */
    /* 0x23 */ BTK_BQ_TODOME = 0x23,
};

class daB_BQ_HIO_c {
public:
    daB_BQ_HIO_c();
    virtual ~daB_BQ_HIO_c() {}

    /* 0x04 */ s8 field_0x4;
    /* 0x08 */ f32 mModelSize;
    /* 0x0C */ s16 mChanceTime;
    /* 0x0E */ s16 mWaterSprayTime;
};

class obj_fw_class : public fopAc_ac_c {
public:
    /* 0x568 */ request_of_phase_process_class mPhase;
    /* 0x570 */ u8 field_0x570;
    /* 0x574 */ J3DModel* mpModel;
    /* 0x578 */ s16 field_0x578;
    /* 0x57A */ s16 mAction;
    /* 0x57C */ u8 field_0x57C[0x57E - 0x57C];
    /* 0x57E */ s16 field_0x57e[2];
    /* 0x584 */ f32 field_0x584;
    /* 0x588 */ f32 field_0x588;
    /* 0x58C */ f32 field_0x58c;
    /* 0x590 */ s16 field_0x590;
    /* 0x592 */ s16 field_0x592;
    /* 0x594 */ f32 field_0x594;
    /* 0x598 */ f32 field_0x598;
    /* 0x59C */ u8 field_0x59C[0x5A0 - 0x59C];
    /* 0x5A0 */ s16 field_0x5a0;
    /* 0x5A2 */ s16 field_0x5a2;
    /* 0x5A4 */ s16 field_0x5a4;
    /* 0x5A8 */ cXyz field_0x5a8;
    /* 0x5B4 */ u8 field_0x5B4[0x5B8 - 0x5B4];
    /* 0x5B8 */ Mtx mBgMtx;
    /* 0x5E8 */ dBgW* mpBgW;
    /* 0x5EC */ Z2SoundObjSimple mSound;
    /* 0x60C */ u8 field_0x60C[0x61C - 0x60C];
    /* 0x61C */ u32 field_0x61c;
    /* 0x620 */ u8 field_0x620[0x624 - 0x620];
    /* 0x624 */ u8 field_0x624;
};

class e_mb_class : public fopEn_enemy_c {
public:
    /* 0x5AC */ request_of_phase_process_class mPhase;
    /* 0x5B4 */ s16 mMode;
    /* 0x5B6 */ u8 field_0x5b6;
    /* 0x5B8 */ cXyz field_0x5b8;
    /* 0x5C4 */ u8 field_0x5C4[0x5C8 - 0x5C4];
    /* 0x5C8 */ mDoExt_McaMorfSO* mpMorf;
    /* 0x5CC */ int mAnmID;
    /* 0x5D0 */ mDoExt_btpAnm* field_0x5d0;
    /* 0x5D4 */ s16 field_0x5d4;
    /* 0x5D8 */ f32 field_0x5d8;
    /* 0x5DC */ cXyz field_0x5dc;
    /* 0x5E8 */ Z2CreatureEnemy mSound;
    /* 0x68C */ s8 field_0x68c;
    /* 0x690 */ mDoExt_McaMorf* field_0x690;
    /* 0x694 */ mDoExt_McaMorf* field_0x694;
    /* 0x698 */ s16 field_0x698;
    /* 0x69A */ s16 mAction;
    /* 0x69C */ s16 field_0x69c[3];
    /* 0x6A2 */ s16 field_0x6a2;
    /* 0x6A4 */ cXyz field_0x6a4;
    /* 0x6B0 */ u8 field_0x6b0;
    /* 0x6B4 */ mDoExt_3DlineMat1_c field_0x6b4;
    /* 0x6F0 */ f32 field_0x6f0;
    /* 0x6F4 */ dCcD_Stts mCcStts;
    /* 0x730 */ dCcD_Sph mCcSph;
    /* 0x868 */ dCcU_AtInfo mAtInfo;
    /* 0x88C */ u8 field_0x88C[0x8C8 - 0x88C];
    /* 0x8C8 */ s8 field_0x8c8;
    /* 0x8C9 */ u8 field_0x8c9;
};

enum daB_BQ_ACT {
    ACTION_STAY,
    ACTION_WAIT,
    ACTION_ATTACK,
    ACTION_DAMAGE,
    ACTION_END,
};

enum daB_BQ_JNT {
    JNT_CENTER,
    JNT_BODY1,
    JNT_BODY2,
    JNT_HAND_RC1,
    JNT_HAND_RC2,
    JNT_HAND_RC3,
    JNT_HAND_RC4,
    JNT_HAND_LC1,
    JNT_HAND_LC2,
    JNT_HAND_LC3,
    JNT_HAND_LC4,
    JNT_HAND_LD1,
    JNT_HAND_LD2,
    JNT_HAND_LD3,
    JNT_HAND_LD4,
    JNT_HAND_RD1,
    JNT_HAND_RD2,
    JNT_HAND_RD3,
    JNT_HAND_RD4,
    JNT_TAIL1,
    JNT_TAIL2,
    JNT_HAND_LA1,
    JNT_HAND_LA2,
    JNT_HAND_LA3,
    JNT_HAND_LA4,
    JNT_HAND_LB1,
    JNT_HAND_LB2,
    JNT_HAND_LB3,
    JNT_HAND_LB4,
    JNT_HAND_L_LOC,
    JNT_HAND_RA1,
    JNT_HAND_RA2,
    JNT_HAND_RA3,
    JNT_HAND_RA4,
    JNT_HAND_RB1,
    JNT_HAND_RB2,
    JNT_HAND_RB3,
    JNT_HAND_RB4,
    JNT_HAND_R_LOC,
    JNT_NECK_1,
    JNT_NECK_2,
    JNT_NECK_3,
    JNT_NECK_4,
    JNT_NECK_5,
    JNT_NECK_6,
    JNT_NECK_7,
    JNT_B_LEAF,
    JNT_HEAD,
    JNT_CORE_1,
    JNT_CORE_2,
    JNT_CORE_3,
    JNT_CORE_4,
    JNT_CORE_5,
    JNT_CORE_6,
    JNT_JAW_T1,
    JNT_JAW_T2,
    JNT_JAW_T3,
    JNT_JAW_U1,
    JNT_JAW_UL1,
    JNT_JAW_UL2,
    JNT_JAW_UR1,
    JNT_JAW_UR2,
    JNT_LEAF_L1,
    JNT_LEAF_L2,
    JNT_LEAF_L3,
    JNT_LEAF_R1,
    JNT_LEAF_R2,
    JNT_LEAF_R3,
    JNT_LEAF_T1,
    JNT_LEAF_T2,
    JNT_LEAF_T3,
    JNT_LEAF_U1,
    JNT_LEAF_U2,
    JNT_LEAF_U3,
    JNT_SLEAF_TL1,
    JNT_SLEAF_TL2,
    JNT_SLEAF_TR1,
    JNT_SLEAF_TR2,
    JNT_SLEAF_UL1,
    JNT_SLEAF_UL2,
    JNT_SLEAF_UR1,
    JNT_SLEAF_UR2,
};

daB_BQ_HIO_c::daB_BQ_HIO_c() {
    field_0x4 = -1;
    mModelSize = 1.0f;
    mChanceTime = 200;
    mWaterSprayTime = 150;
}

static void anm_init(b_bq_class* i_this, int i_anmID, f32 i_morf, u8 i_mode, f32 i_speed) {
    J3DAnmTransform* pbck = (J3DAnmTransform*)dComIfG_getObjectRes("B_bq", i_anmID);
    i_this->mpMorf->setAnm(pbck, i_mode, i_morf, i_speed, 0.0f, -1.0f);
    i_this->mAnmID = i_anmID;
}

static int nodeCallBack(J3DJoint* i_joint, int param_1) {
    if (param_1 == 0) {
        int joint_no = i_joint->getJntNo();
        J3DModel* model = j3dSys.getModel();
        b_bq_class* a_this = (b_bq_class*)model->getUserArea();

        if (a_this != NULL) {
            MTXCopy(model->getAnmMtx(joint_no), *calc_mtx);

            if (joint_no == JNT_NECK_5 || joint_no == JNT_NECK_6 || joint_no == JNT_NECK_7 ||
                joint_no == JNT_HEAD)
            {
                cMtx_YrotM(*calc_mtx, a_this->mHeadRot);
            }

            if (joint_no == TREG_S(0)) {
                cMtx_XrotM(*calc_mtx, TREG_S(1));
                cMtx_YrotM(*calc_mtx, TREG_S(2));
                cMtx_ZrotM(*calc_mtx, TREG_S(3));
            } else if (joint_no == TREG_S(4)) {
                cMtx_XrotM(*calc_mtx, TREG_S(5));
                cMtx_YrotM(*calc_mtx, TREG_S(6));
                cMtx_ZrotM(*calc_mtx, TREG_S(7));
            }

            model->setAnmMtx(joint_no, *calc_mtx);
            MTXCopy(*calc_mtx, J3DSys::mCurrentMtx);
        }
    }

    return 1;
}

static int daB_BQ_Draw(b_bq_class* i_this) {
    if (i_this->mDisableDraw) {
        return 1;
    }

    J3DModel* model = i_this->mpMorf->getModel();
    g_env_light.settingTevStruct(0, &i_this->current.pos, &i_this->tevStr);
    g_env_light.setLightTevColorType_MAJI(model, &i_this->tevStr);

    if (i_this->mpDeadBrk->getPlaySpeed() > 0.0f) {
        i_this->mpDeadBrk->entry(model->getModelData());
    }

    if (i_this->mPlayTodomeBtk) {
        i_this->mpTodomeBtk->entry(model->getModelData());
    }

    if (i_this->mSetDeadColor) {
        J3DModelData* modelData = model->getModelData();

        for (u16 i = 0; i < modelData->getMaterialNum(); i++) {
            J3DMaterial* material = modelData->getMaterialNodePointer(i);
            material->getTevColor(0)->r = i_this->mDeadColor;
            material->getTevColor(0)->g = i_this->mDeadColor;
            material->getTevColor(0)->b = i_this->mDeadColor;
        }
    }

    i_this->mpMorf->entryDL();

    if (i_this->field_0x5c8 != 0) {
        if (i_this->field_0x5c8 == 1) {
            g_env_light.setLightTevColorType_MAJI(i_this->mpEyeballModel, &i_this->tevStr);

            if (i_this->field_0x1178 > 0.0f) {
                i_this->mpEyeballModel->getModelData()
                    ->getMaterialNodePointer(0)
                    ->getTevColor(2)
                    ->r = 255.0f - i_this->field_0x1178 * 195.0f;

                i_this->mpEyeballModel->getModelData()
                    ->getMaterialNodePointer(0)
                    ->getTevColor(2)
                    ->g = 255.0f - i_this->field_0x1178 * 225.0f;

                i_this->mpEyeballModel->getModelData()
                    ->getMaterialNodePointer(0)
                    ->getTevColor(2)
                    ->b = 255.0f - i_this->field_0x1178 * 255.0f;
            }

            mDoExt_modelUpdateDL(i_this->mpEyeballModel);
        }

        model->getModelData()->getMaterialNodePointer(2)->getShape()->hide();
    } else {
        model->getModelData()->getMaterialNodePointer(2)->getShape()->show();
    }

    if (i_this->mBlureRate > 2) {
        mDoGph_gInf_c::setBlureRate(i_this->mBlureRate);
        mDoGph_gInf_c::onBlure();
    } else {
        mDoGph_gInf_c::offBlure();
    }

    return 1;
}

static void* s_fw_sub0(void* i_actor, void* i_data) {
    if (fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_OBJ_FW) {
        static_cast<obj_fw_class*>(i_actor)->field_0x58c = 0.0f;
    }

    return NULL;
}

static void* s_fw_sub1(void* i_actor, void* i_data) {
    fopAc_ac_c* data = (fopAc_ac_c*)i_data;

    if (fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_OBJ_FW) {
        obj_fw_class* fw = static_cast<obj_fw_class*>(i_actor);

        fw->field_0x567 = 1;

        s16 var_r30 = cM_atan2s(fw->current.pos.x - data->current.pos.x,
                                fw->current.pos.z - data->current.pos.z);
        cMtx_YrotS(*calc_mtx, var_r30);

        cXyz sp4C(0.0f, 0.0f, TREG_F(7) + 1.0f);
        cXyz sp58;
        MtxPosition(&sp4C, &sp58);

        cLib_addCalc2(&fw->speed.x, sp58.x, 0.1f, 0.02f);
        cLib_addCalc2(&fw->speed.z, sp58.z, 0.1f, 0.02f);

        s16* angle_y = &fw->shape_angle.y;
        s16 temp_r0 = (s16)(fw->shape_angle.y - var_r30);
        if (temp_r0 > 0x4000 || temp_r0 < -0x4000) {
            var_r30 += 0x8000;
        }

        cLib_addCalcAngleS2(angle_y, var_r30, 0x20, 0x20);
    }

    return NULL;
}

static u8 data_805BAD50;

static daB_BQ_HIO_c l_HIO;

static s8 data_805BAD70;

static void* s_fw_sub2(void* i_actor, void* i_data) {
    fopAc_ac_c* data = (fopAc_ac_c*)i_data;

    if (fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_OBJ_FW) {
        obj_fw_class* fw = static_cast<obj_fw_class*>(i_actor);

        fw->field_0x567 = 2;

        s16 var_r30 = cM_atan2s(fw->current.pos.x - data->current.pos.x,
                                fw->current.pos.z - data->current.pos.z);
        cMtx_YrotS(*calc_mtx, var_r30);

        cXyz sp4C(0.0f, 0.0f, TREG_F(8) + 20.0f);
        MtxPosition(&sp4C, &fw->speed);

        if (data_805BAD70 == 0) {
            fw->field_0x57e[0] = 5;
        } else if (data_805BAD70 == 1) {
            fw->field_0x57e[0] = 8;
        } else {
            fw->field_0x57e[0] = 15;
        }

        data_805BAD70++;
    }

    return NULL;
}

static void* s_fw_del_sub(void* i_actor, void* i_data) {
    if (fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_OBJ_FW) {
        fopAcM_delete((fopAc_ac_c*)i_actor);
    }

    return NULL;
}

static void* s_bi_del_sub(void* i_actor, void* i_data) {
    if (fopAcM_IsActor(i_actor) &&
        (fopAcM_GetName(i_actor) == PROC_E_BI || fopAcM_GetName(i_actor) == PROC_E_BI_LEAF))
    {
        fopAcM_delete((fopAc_ac_c*)i_actor);
    }

    return NULL;
}

static void damage_check(b_bq_class* i_this) {
    dComIfGp_getPlayer(0);

    i_this->mCcCoStts.Move();
    i_this->mCcStts.Move();

    if (i_this->mAction != ACTION_DAMAGE) {
        if (i_this->mCcSph.ChkTgHit()) {
            i_this->mAtInfo.mpCollider = i_this->mCcSph.GetTgHitObj();
            at_power_check(&i_this->mAtInfo);

            if (i_this->mAtInfo.mHitType == 2) {
                i_this->mAction = ACTION_DAMAGE;
                i_this->mMode = 0;
                i_this->field_0x6de = 30;

                i_this->mSound.startCreatureVoice(Z2SE_EN_BQ_V_BOMBDAMAGE, -1);

                i_this->field_0x11fc++;
                dComIfGs_onOneZoneSwitch(8, -1);
                return;
            }

            def_se_set(&i_this->mSound, i_this->mAtInfo.mpCollider, 0x2D, NULL);
            anm_init(i_this, BCK_BQ_NODAMAGE, 3.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
            i_this->mSound.startCreatureVoice(Z2SE_EN_BQ_V_NODAMAGE, -1);
            i_this->field_0x6de = 10;
        }
    } else if (i_this->field_0x6de == 0 && i_this->mCcCoreSph.ChkTgHit()) {
        i_this->mAtInfo.mpCollider = i_this->mCcCoreSph.GetTgHitObj();
        cc_at_check(i_this, &i_this->mAtInfo);

        OS_REPORT("//////////////B_BQ AP %d\n", i_this->mAtInfo.mAttackPower);
        OS_REPORT("//////////////B_BQ HP %d\n", i_this->health);

        if (daPy_getPlayerActorClass()->getCutType() == daPy_py_c::CUT_TYPE_JUMP &&
            daPy_getPlayerActorClass()->checkCutJumpCancelTurn())
        {
            i_this->field_0x6de = 3;
        } else {
            i_this->field_0x6de = 6;
        }

        if (i_this->mAction == ACTION_DAMAGE) {
            if (i_this->health <= 0 || daPy_getPlayerActorClass()->getCutCount() >= 4) {
                i_this->mMode = 20;
                i_this->field_0x6de = 100;
                return;
            }

            i_this->mMode = 10;
            i_this->mSound.startCreatureVoice(Z2SE_EN_BQ_V_COREDAMAGE, -1);

            if (daPy_getPlayerActorClass()->getCutCount() != 0 && i_this->mTimers[0] < 30) {
                i_this->mTimers[0] = 30;
            }
        }
    }
}

static s8 b_bq_stay(b_bq_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    int anm_frame = i_this->mpMorf->getFrame();

    s8 rt = 0;

    switch (i_this->mMode) {
    case 0:
        anm_init(i_this, BCK_BQ_APPEAR, 1.0f, J3DFrameCtrl::EMode_NONE, 0.0f);
        i_this->mMode = 1;
        break;
    case 2: {
        i_this->mTimers[0] = 150;

        int sw = (fopAcM_GetParam(a_this) >> 0x10) & 0xFF;
        dComIfGs_onSwitch(sw, fopAcM_GetRoomNo(a_this));

        i_this->mMode = 3;

        OS_REPORT("//////////////B_BQ SYUTUGEN SET!!! !!\n");
        dComIfGp_particle_set(0x82B0, &a_this->current.pos, NULL, NULL);
        dComIfGp_particle_set(0x82B1, &a_this->current.pos, NULL, NULL);
        mDoAud_seStart(Z2SE_EN_BQ_ABUKU, &a_this->current.pos, 0, 0);
        break;
    }
    case 3:
        if (i_this->mTimers[0] == 100) {
            mDoAud_seStart(Z2SE_EN_BQ_JINARI, NULL, 0, 0);
        }

        if (i_this->mTimers[0] == 0) {
            anm_init(i_this, BCK_BQ_APPEAR, 1.0f, J3DFrameCtrl::EMode_NONE, 1.0f);

            i_this->mMode = 4;
            i_this->mDisableDraw = false;

            dComIfGp_particle_set(0x82B6, &a_this->current.pos, NULL, NULL);
            dComIfGp_particle_set(0x82B7, &a_this->current.pos, NULL, NULL);
            dComIfGp_particle_set(0x82B8, &a_this->current.pos, NULL, NULL);
            dComIfGp_particle_set(0x82B9, &a_this->current.pos, NULL, NULL);
            dComIfGp_particle_set(0x82F9, &a_this->current.pos, NULL, NULL);
        }
        break;
    case 4:
        rt = 1;

        if (i_this->mpMorf->checkFrame(45)) {
            i_this->mSound.startCreatureSound(Z2SE_EN_BQ_APPEAR, 0, -1);
            i_this->mSound.startCreatureVoice(Z2SE_EN_BQ_V_APPEAR, -1);
        }

        if (i_this->mpMorf->checkFrame(KREG_F(7) + 180)) {
            i_this->mSound.startCreatureVoice(Z2SE_EN_BQ_V_BARK, -1);
        }

        if (i_this->mpMorf->isStop()) {
            i_this->mAction = ACTION_WAIT;
            anm_init(i_this, BCK_BQ_WAIT01, 1.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
            i_this->mMode = 1;
        }
        break;
    }

    return rt;
}

static void b_bq_wait(b_bq_class* i_this) {
    switch (i_this->mMode) {
    case 0:
        anm_init(i_this, BCK_BQ_WAIT01, 10.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
        i_this->mMode = 1;
        // fallthrough
    case 1:
        if (i_this->mTimers[0] == 0) {
            i_this->mTimers[0] = cM_rndF(80) + 50.0f;
            i_this->mSound.startCreatureVoice(Z2SE_EN_BQ_V_WAIT, -1);
        }

#if DEBUG
        if (mDoCPd_c::getTrigRight(PAD_1)) {
            i_this->mAction = ACTION_END;
            i_this->mMode = 0;
            i_this->mDemoMode = 50;
        }
#endif
        break;
    }

    if (i_this->mAnmID == BCK_BQ_NODAMAGE && i_this->mpMorf->isStop()) {
        anm_init(i_this, BCK_BQ_WAIT01, 10.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
    }

    if (i_this->field_0x11fc != 0 && i_this->mTimers[2] == 1) {
        i_this->mAction = ACTION_ATTACK;
        i_this->mMode = 0;
    }
}

static void b_bq_damage(b_bq_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    s8 set_wait_action = false;

    switch (i_this->mMode) {
    case 0:
        anm_init(i_this, BCK_BQ_BOMBDAMAGE, 3.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
        i_this->mMode = 1;
        i_this->mTimers[0] = 1000;
        a_this->health = 50;
        i_this->field_0x1392 = 4;
        // fallthrough
    case 1:
        if (i_this->mpMorf->checkFrame(YREG_F(8) + 113)) {
            dComIfGp_getVibration().StartShock(YREG_S(2) + 5, 14, cXyz(0.0f, 1.0f, 0.0f));
            dComIfGp_getVibration().StartShock(8, 31, cXyz(0.0f, 1.0f, 0.0f));

            i_this->mSound.startCreatureSound(Z2SE_EN_BQ_DOWN, 0, -1);

            MTXCopy(i_this->mpMorf->getModel()->getAnmMtx(YREG_S(1) + 50), *calc_mtx);

            cXyz sp2C(XREG_F(3) + 300.0f, XREG_F(4), XREG_F(5));
            cXyz sp38;
            MtxPosition(&sp2C, &sp38);

            fopAcM_effSmokeSet1(&i_this->field_0x1220, &i_this->field_0x1224, &a_this->eyePos, NULL,
                                TREG_F(11) + 5.0f, &a_this->tevStr, 1);

            if (i_this->mDamageBackCount >= 2) {
                i_this->onDownFlg();
                a_this->health = 50;
            }

            Z2GetAudioMgr()->changeBgmStatus(2);
        }

        if (i_this->mpMorf->isStop()) {
            anm_init(i_this, BCK_BQ_DAMAGEWAIT, 3.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
            i_this->mMode = 2;
            i_this->mTimers[0] = l_HIO.mChanceTime;
        }
        break;
    case 2:
        if (i_this->mTimers[2] == 0) {
            i_this->mTimers[2] = cM_rndF(20) + 10.0f;
            i_this->mSound.startCreatureVoice(Z2SE_EN_BQ_V_DAMAGEWAIT, -1);
        }
        break;
    case 10:
        anm_init(i_this, BCK_BQ_COREDAMAGE, 2.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
        i_this->mMode = 11;
        break;
    case 11:
        if (i_this->mpMorf->isStop()) {
            anm_init(i_this, BCK_BQ_DAMAGEWAIT, JREG_F(15), J3DFrameCtrl::EMode_LOOP, 1.0f);
            i_this->mMode = 2;
        }
        break;
    case 20:
        i_this->mDamageBackCount++;
        if (i_this->mDamageBackCount >= 3 &&
            (i_this->health <= 0 || daPy_getPlayerActorClass()->checkFastSwordCut()))
        {
            i_this->mAction = ACTION_END;
            i_this->mMode = 0;
            i_this->mDemoMode = 50;
            return;
        }

        anm_init(i_this, BCK_BQ_RETURN01, 3.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
        i_this->mMode = 21;
        i_this->mSound.startCreatureVoice(Z2SE_EN_BQ_V_DAMAGEBACK, -1);
        break;
    case 21:
        if (i_this->mpMorf->isStop()) {
            set_wait_action = true;
        }
        break;
    case 30:
        anm_init(i_this, BCK_BQ_RETURN02, 5.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
        i_this->mMode = 31;
        i_this->mSound.startCreatureVoice(Z2SE_EN_BQ_V_BACK, -1);
        break;
    case 31:
        if (i_this->mpMorf->isStop()) {
            set_wait_action = true;
        }
        break;
    case 40:
        i_this->mMode = 41;
        i_this->mTimers[0] = 45;
        // fallthrough
    case 41:
        if (i_this->mTimers[0] == 0) {
            i_this->mAction = ACTION_END;
            i_this->mMode = 0;
            i_this->mDemoMode = 50;
            return;
        }
    }

    if (i_this->mMode < 20 && i_this->mTimers[0] == 0) {
        i_this->mMode = 30;
    }

    if (set_wait_action) {
        i_this->mAction = ACTION_WAIT;
        i_this->mMode = 0;
        i_this->field_0x6fc = 10;
        i_this->field_0x6f6 = 0;
        i_this->mTimers[0] = 0;
        i_this->mTimers[2] = 80;
        Z2GetAudioMgr()->changeBgmStatus(1);
    }

    MTXCopy(i_this->mpMorf->getModel()->getAnmMtx(YREG_S(0) + 53), mDoMtx_stack_c::get());
    cXyz sp44;
    mDoMtx_stack_c::multVecZero(&sp44);

    cXyz sp50 = dComIfGp_getPlayer(0)->current.pos - sp44;
    s16 temp_r3 = cM_atan2s(sp50.x, sp50.z);
    cXyz sp5C;

    cMtx_YrotS(*calc_mtx, temp_r3);

    sp50.x = 0.0f;
    sp50.y = 0.0f;
    sp50.z = (BREG_F(12) + 60.0f) - 30.0f;
    MtxPosition(&sp50, &sp5C);
    sp5C += sp44;
    i_this->setDownPos(&sp5C);

    if (i_this->checkCutDownHitFlg()) {
        anm_init(i_this, BCK_BQ_TODOME, 2.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
        i_this->mPlayTodomeBtk = true;
        i_this->mMode = 40;

        i_this->offCutDownHitFlg();
        i_this->offDownFlg();

        i_this->mSound.startCreatureVoice(Z2SE_EN_BQ_V_TODOME, -1);
        i_this->field_0x6de = 300;

        dComIfGp_particle_set(0x836C, &sp44, NULL, NULL);
        dComIfGp_particle_set(0x836D, &sp44, NULL, NULL);
        dComIfGp_particle_set(0x836E, &sp44, NULL, NULL);
    }
}

static s8 b_bq_attack(b_bq_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;

    int anm_frame = i_this->mpMorf->getFrame();
    s8 set_dokuhaki = false;

    s16 head_rot = cM_ssin(i_this->mTimers[0] * 1500) * (NREG_F(7) + 1200.0f);

    cXyz spCC;
    cXyz spD8;

    MTXCopy(i_this->mpMorf->getModel()->getAnmMtx(JNT_HEAD), *calc_mtx);
    cMtx_ZrotM(*calc_mtx, head_rot);

    spCC.set(0.0f, 0.0f, 0.0f);
    MtxPosition(&spCC, &i_this->mMizuAtStartPos);

    spCC.set(XREG_F(3) + 2300.0f, XREG_F(4) + 500.0f, XREG_F(5));
    MtxPosition(&spCC, &spD8);

    switch (i_this->mMode) {
    case 0:
        anm_init(i_this, BCK_BQ_ATTACK_A, 10.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
        i_this->mMode = 1;

        if (cM_rndF(1.0f) < 0.5f) {
            i_this->field_0x138e = 0x500;
            i_this->field_0x1390 = -0x500;
        } else {
            i_this->field_0x138e = -0x500;
            i_this->field_0x1390 = 0x500;
        }
        break;
    case 1:
        if (anm_frame >= 54) {
            if (anm_frame == 54) {
                for (int i = 0; i < 16; i++) {
                    i_this->field_0x12c8[i] = spD8;
                }

                i_this->mTimers[1] = 30;
            }
            set_dokuhaki = true;
        }

        if (i_this->mpMorf->isStop()) {
            anm_init(i_this, BCK_BQ_ATTACK_B, 2.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
            i_this->mMode = 2;
            i_this->mTimers[0] = l_HIO.mWaterSprayTime;
        }
        break;
    case 2:
        set_dokuhaki = true;

        if (i_this->mTimers[0] == 0) {
            anm_init(i_this, BCK_BQ_ATTACK_B, 2.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
            i_this->mMode = 3;
        }
        break;
    case 3:
        if (i_this->mpMorf->isStop()) {
            i_this->field_0x6de = 1;
        }
        break;
    }

    if (i_this->field_0x6de != 0) {
        i_this->mAction = ACTION_WAIT;
        i_this->mMode = 0;
        i_this->field_0x6fc = 10;
        i_this->field_0x138e = 0;
    }

    cXyz mizu_cc_center[4];
    cXyz cc_center_offset(0.0f, 0.0f, 0.0f);

    if (set_dokuhaki) {
        i_this->mSound.startCreatureVoiceLevel(Z2SE_EN_BQ_V_DOKUHAKI, -1);

        for (int i = 0; i < 4; i++) {
            static u16 fireno[] = {0x82D6, 0x82D7, 0x82D8, 0x82DF};

            i_this->mMizutamaEmtrIDs[i] = dComIfGp_particle_set(
                i_this->mMizutamaEmtrIDs[i], fireno[i], &a_this->current.pos, NULL, NULL);

            JPABaseEmitter* emitter = dComIfGp_particle_getEmitter(i_this->mMizutamaEmtrIDs[i]);
            if (emitter != NULL) {
                MTXCopy(i_this->mpMorf->getModel()->getAnmMtx(JNT_HEAD), *calc_mtx);
                cMtx_ZrotM(*calc_mtx, head_rot);

                emitter->setGlobalRTMatrix(*calc_mtx);
            }
        }

        i_this->field_0x1388++;
        i_this->field_0x1388 &= 15;

        i_this->field_0x12c8[i_this->field_0x1388] = spD8;
        i_this->field_0x12bc = i_this->field_0x12c8[(i_this->field_0x1388 - (ZREG_S(4) + 8)) & 15];

        i_this->field_0x684.startLevelSound(Z2SE_EN_BQ_DOKUHAKI, 0, -1);

        dBgS_LinChk lin_chk;
        lin_chk.Set(&i_this->mMizuAtStartPos, &i_this->field_0x12bc, a_this);

        if (dComIfG_Bgsp().LineCross(&lin_chk)) {
            spD8 = lin_chk.GetCross();

            for (int i = 0; i < 2; i++) {
                static u16 fireno[] = {0x82D4, 0x82D5};

                i_this->mSmokeEmtrIDs[i] =
                    dComIfGp_particle_set(i_this->mSmokeEmtrIDs[i], fireno[i], &spD8, NULL, NULL);
            }

            i_this->field_0x1144 = spD8;

            if (i_this->field_0x1393 >= 10) {
                i_this->field_0x6a4.startLevelSound(Z2SE_EN_BQ_DOKUSMOKE, 0, -1);
            } else {
                i_this->field_0x1393++;
            }
        }

        spCC = i_this->mMizuAtStartPos - spD8;

        i_this->field_0x1138 = spD8 + ((spCC * (i_this->field_0x6c8 & 7)) * 0.05f);

        for (int i = 0; i < 4; i++) {
            mizu_cc_center[i] = spD8 + ((spCC * i) * 0.05f);
        }

        i_this->mColpatType = 5;
    } else {
        i_this->field_0x1393 = 0;
        cc_center_offset.x += -20000.0f;

        for (int i = 0; i < 4; i++) {
            mizu_cc_center[i].set(0.0f, 0.0f, 0.0f);
        }
    }

    for (int i = 0; i < 4; i++) {
        i_this->mCcMizuSph[i].SetC(mizu_cc_center[i] + cc_center_offset);
        i_this->mCcMizuSph[i].SetR(35.0f);
        dComIfG_Ccsp()->Set(&i_this->mCcMizuSph[i]);
    }

    return set_dokuhaki;
}

static void b_bq_end(b_bq_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;

    switch (i_this->mMode) {
    case 0: {
        i_this->mSound.startCreatureVoice(Z2SE_EN_BQ_V_DEAD, -1);

        anm_init(i_this, BCK_BQ_DEAD, 1.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
        i_this->mpDeadBrk->setPlaySpeed(1.0f);
        i_this->mMode = 1;

        int sw = fopAcM_GetParam(a_this) >> 0x18;
        dComIfGs_onSwitch(sw, fopAcM_GetRoomNo(a_this));
        // fallthrough
    }
    case 1:
        break;
    }
}

static void action(b_bq_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    cXyz sp40;
    cXyz sp4C;

    i_this->mAngleToPlayer = fopAcM_searchPlayerAngleY(a_this);
    i_this->mDistToPlayer = fopAcM_searchPlayerDistance(a_this);

    damage_check(i_this);

    s8 set_head_angle = false;
    s8 boss_appeared = true;

    i_this->field_0x6fb = 0;

    s16 dokuhaki_set = false;
    s8 var_r27 = true;

    switch (i_this->mAction) {
    case ACTION_STAY:
        boss_appeared = b_bq_stay(i_this);
        i_this->field_0x6de = 10;
        break;
    case ACTION_WAIT:
        b_bq_wait(i_this);
        set_head_angle = true;
        break;
    case ACTION_ATTACK:
        dokuhaki_set = b_bq_attack(i_this);
        set_head_angle = true;
        i_this->field_0x6fb = 2;
        break;
    case ACTION_DAMAGE:
        b_bq_damage(i_this);
        set_head_angle = false;
        i_this->field_0x6fb = 1;
        break;
    case ACTION_END:
        b_bq_end(i_this);
        i_this->field_0x6fb = 1;

        if (i_this->field_0x1396) {
            var_r27 = false;
        }
        break;
    }

    if (dokuhaki_set && i_this->mTimers[1] == 0) {
        cLib_addCalcAngleS2(&i_this->field_0x138e, i_this->field_0x1390, 8, 0x30);
    }

    cLib_addCalcAngleS2(&i_this->field_0x138c, i_this->field_0x138e, 8, 200);

    s16 head_rot_target = 0;
    if (set_head_angle) {
        e_mb_class* monkey_bomb = (e_mb_class*)fopAcM_SearchByID(i_this->mMonkeyBombID);

        if (monkey_bomb != NULL && monkey_bomb->mAction == 2 && monkey_bomb->field_0x69c[2] == 0) {
            sp40 = monkey_bomb->current.pos - a_this->current.pos;
            head_rot_target = cM_atan2s(sp40.x, sp40.z) - a_this->shape_angle.y;
            head_rot_target /= 6;
        } else {
            head_rot_target =
                ((i_this->mAngleToPlayer + i_this->field_0x138c) - a_this->shape_angle.y) / 3;

            if (head_rot_target > 3500) {
                head_rot_target = 3500;
            } else if (head_rot_target < -3500) {
                head_rot_target = -3500;
            }
        }
    }

    cLib_addCalcAngleS2(&i_this->mHeadRot, head_rot_target, 8, i_this->field_0x6f6);
    cLib_addCalcAngleS2(&i_this->field_0x6f6, 0x100, 1, 1);

    if (boss_appeared && !i_this->mDisableDraw) {
        i_this->field_0x1180 =
            dComIfGp_particle_set(i_this->field_0x1180, 0x82B4, &a_this->current.pos, NULL, NULL);
        i_this->field_0x1184 =
            dComIfGp_particle_set(i_this->field_0x1184, 0x82B5, &a_this->current.pos, NULL, NULL);
    }

    if (var_r27) {
        daPy_getPlayerActorClass()->onBossRoomWait();
    }
}

static void anm_se_set(b_bq_class* i_this) {
    int anm_frame = i_this->mpMorf->getFrame();

    if (i_this->mAnmID == BCK_BQ_APPEAR) {
        if (anm_frame >= 175) {
            i_this->field_0x1188 = dComIfGp_particle_set(i_this->field_0x1188, 0x82BA,
                                                         &i_this->current.pos, NULL, NULL);

            JPABaseEmitter* emitter = dComIfGp_particle_getEmitter(i_this->field_0x1188);
            if (emitter != NULL) {
                emitter->setGlobalRTMatrix(i_this->mpMorf->getModel()->getAnmMtx(JNT_JAW_T3));
            }
        }

        if (anm_frame >= 180) {
            i_this->field_0x118c = dComIfGp_particle_set(i_this->field_0x118c, 0x82B2,
                                                         &i_this->current.pos, NULL, NULL);

            JPABaseEmitter* emitter = dComIfGp_particle_getEmitter(i_this->field_0x118c);
            if (emitter != NULL) {
                emitter->setGlobalRTMatrix(i_this->mpMorf->getModel()->getAnmMtx(JNT_HEAD));
            }

            i_this->field_0x1190 = dComIfGp_particle_set(i_this->field_0x1190, 0x82B3,
                                                         &i_this->current.pos, NULL, NULL);

            emitter = dComIfGp_particle_getEmitter(i_this->field_0x1190);
            if (emitter != NULL) {
                emitter->setGlobalRTMatrix(i_this->mpMorf->getModel()->getAnmMtx(JNT_HEAD));
            }
        }
    } else if (i_this->mAnmID == BCK_BQ_RETURN01) {
        i_this->field_0x1188 =
            dComIfGp_particle_set(i_this->field_0x1188, 0x8322, &i_this->current.pos, NULL, NULL);

        JPABaseEmitter* emitter = dComIfGp_particle_getEmitter(i_this->field_0x1188);
        if (emitter != NULL) {
            emitter->setGlobalRTMatrix(i_this->mpMorf->getModel()->getAnmMtx(JNT_HEAD));
        }

        i_this->field_0x118c =
            dComIfGp_particle_set(i_this->field_0x118c, 0x8323, &i_this->current.pos, NULL, NULL);

        emitter = dComIfGp_particle_getEmitter(i_this->field_0x118c);
        if (emitter != NULL) {
            emitter->setGlobalRTMatrix(i_this->mpMorf->getModel()->getAnmMtx(JNT_HEAD));
        }
    } else if (i_this->mAnmID == BCK_BQ_RETURN02) {
        i_this->field_0x1188 =
            dComIfGp_particle_set(i_this->field_0x1188, 0x8324, &i_this->current.pos, NULL, NULL);

        JPABaseEmitter* emitter = dComIfGp_particle_getEmitter(i_this->field_0x1188);
        if (emitter != NULL) {
            emitter->setGlobalRTMatrix(i_this->mpMorf->getModel()->getAnmMtx(JNT_HEAD));
        }

        i_this->field_0x118c =
            dComIfGp_particle_set(i_this->field_0x118c, 0x8325, &i_this->current.pos, NULL, NULL);

        emitter = dComIfGp_particle_getEmitter(i_this->field_0x118c);
        if (emitter != NULL) {
            emitter->setGlobalRTMatrix(i_this->mpMorf->getModel()->getAnmMtx(JNT_HEAD));
        }
    } else if (i_this->mAnmID == BCK_BQ_BOMBDAMAGE) {
        i_this->field_0x1188 =
            dComIfGp_particle_set(i_this->field_0x1188, 0x82FA, &i_this->current.pos, NULL, NULL);

        JPABaseEmitter* emitter = dComIfGp_particle_getEmitter(i_this->field_0x1188);
        if (emitter != NULL) {
            emitter->setGlobalRTMatrix(i_this->mpMorf->getModel()->getAnmMtx(JNT_HEAD));
        }

        i_this->field_0x118c =
            dComIfGp_particle_set(i_this->field_0x118c, 0x82FB, &i_this->current.pos, NULL, NULL);

        emitter = dComIfGp_particle_getEmitter(i_this->field_0x118c);
        if (emitter != NULL) {
            emitter->setGlobalRTMatrix(i_this->mpMorf->getModel()->getAnmMtx(JNT_HEAD));
        }

        i_this->field_0x1190 =
            dComIfGp_particle_set(i_this->field_0x1190, 0x82FC, &i_this->current.pos, NULL, NULL);

        emitter = dComIfGp_particle_getEmitter(i_this->field_0x1190);
        if (emitter != NULL) {
            emitter->setGlobalRTMatrix(i_this->mpMorf->getModel()->getAnmMtx(JNT_HEAD));
        }
    } else if (i_this->mAnmID == BCK_BQ_DEAD) {
        if (anm_frame == 1) {
            for (int i = 0; i < 19; i++) {
                static u16 g_e_i[] = {0x82FD, 0x82FE, 0x82FF, 0x8300, 0x8301, 0x8302, 0x8303,
                                      0x8304, 0x8311, 0x8312, 0x8313, 0x8314, 0x8315, 0x8316,
                                      0x8317, 0x8318, 0x8319, 0x831A, 0x831B};

                dComIfGp_particle_set(g_e_i[i], &i_this->current.pos, &i_this->current.angle, NULL);
            }
        }

        for (int i = 0; i < 12; i++) {
            static u16 g_d_i[] = {0x8305, 0x830B, 0x8306, 0x830C, 0x8307, 0x830D,
                                  0x8308, 0x830E, 0x8309, 0x830F, 0x830A, 0x8310};

            i_this->field_0x11a8[i] = dComIfGp_particle_set(i_this->field_0x11a8[i], g_d_i[i],
                                                            &i_this->current.pos, NULL, NULL);

            JPABaseEmitter* emitter = dComIfGp_particle_getEmitter(i_this->field_0x11a8[i]);
            if (emitter != NULL) {
                static int g_k_ji[] = {0x28, 0x28, 0x29, 0x29, 0x2A, 0x2A,
                                       0x2B, 0x2B, 0x2C, 0x2C, 0x2D, 0x2D};

                emitter->setGlobalRTMatrix(i_this->mpMorf->getModel()->getAnmMtx(g_k_ji[i]));
            }
        }
    } else {
        f32 scale_target = 1.0f;
        if (i_this->mAnmID != BCK_BQ_WAIT01) {
            scale_target = 0.0f;
        }

        cLib_addCalc2(&i_this->field_0x11d8, scale_target, 1.0f, 0.05f);

        for (int i = 0; i < 5; i++) {
            static u16 g_y_i[] = {0x8327, 0x8328, 0x8329, 0x832A, 0x8326};

            i_this->field_0x1194[i] = dComIfGp_particle_set(i_this->field_0x1194[i], g_y_i[i],
                                                            &i_this->current.pos, NULL, NULL);

            dComIfGp_particle_levelEmitterOnEventMove(i_this->field_0x1194[i]);

            JPABaseEmitter* emitter = dComIfGp_particle_getEmitter(i_this->field_0x1194[i]);
            if (emitter != NULL) {
                static int g_y_ji[] = {0x37, 0x38, 0x3B, 0x3D, 0x2F};

                MTXCopy(i_this->mpMorf->getModel()->getAnmMtx(g_y_ji[i]), *calc_mtx);
                MtxScale(i_this->field_0x11d8, i_this->field_0x11d8, i_this->field_0x11d8, 1);
                emitter->setGlobalRTMatrix(*calc_mtx);
            }
        }
    }

    if (i_this->mSetBossExplode) {
        i_this->mSetBossExplode = false;

        i_this->mSound.startCreatureSound(Z2SE_EN_BOSS_EXPLODE, 0, 0);

        for (int i = 0; i < 19; i++) {
            static u16 g_c_i[] = {0x83F0, 0x83F1, 0x83F2, 0x83F3, 0x83F4, 0x83F5, 0x83F6,
                                  0x83F7, 0x83F8, 0x83F9, 0x83FA, 0x83FB, 0x83FC, 0x83FD,
                                  0x83FE, 0x83FF, 0x8400, 0x8401, 0x8402};

            JPABaseEmitter* emitter =
                dComIfGp_particle_set(g_c_i[i], &i_this->current.pos, NULL, NULL);

            if (emitter != NULL) {
                static int g_c_ji[] = {0x02, 0x30, 0x35, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D,
                                       0x02, 0x30, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D};

                MTXCopy(i_this->mpMorf->getModel()->getAnmMtx(g_c_ji[i]), *calc_mtx);
                emitter->setGlobalRTMatrix(*calc_mtx);
            }
        }
    }
}

static void cam_3d_morf(b_bq_class* i_this, f32 i_scale) {
    cLib_addCalc2(&i_this->mDemoCamCenter.x, i_this->mDemoCamCenterTarget.x, i_scale,
                  i_this->field_0x1288.x * i_this->field_0x129c);
    cLib_addCalc2(&i_this->mDemoCamCenter.y, i_this->mDemoCamCenterTarget.y, i_scale,
                  i_this->field_0x1288.y * i_this->field_0x129c);
    cLib_addCalc2(&i_this->mDemoCamCenter.z, i_this->mDemoCamCenterTarget.z, i_scale,
                  i_this->field_0x1288.z * i_this->field_0x129c);

    cLib_addCalc2(&i_this->mDemoCamEye.x, i_this->mDemoCamEyeTarget.x, i_scale,
                  i_this->field_0x127c.x * i_this->field_0x129c);
    cLib_addCalc2(&i_this->mDemoCamEye.y, i_this->mDemoCamEyeTarget.y, i_scale,
                  i_this->field_0x127c.y * i_this->field_0x129c);
    cLib_addCalc2(&i_this->mDemoCamEye.z, i_this->mDemoCamEyeTarget.z, i_scale,
                  i_this->field_0x127c.z * i_this->field_0x129c);
}

static void fish_set(b_bq_class* i_this) {
    cXyz pos;

    for (int i = 0; i < 2; i++) {
        static struct {
            /* 0x0 */ u8 param;
            /* 0x4 */ f32 pos_x;
            /* 0x8 */ f32 pos_z;
        } lf_pos[] = {
            {0x14, -1000.0f, 100.0f},
            {0x14, 1000.0f, 100.0f},
        };

        pos.x = lf_pos[i].pos_x;
        pos.y = -200.0f;
        pos.z = lf_pos[i].pos_z;

        fopAcM_create(PROC_NPC_LF, lf_pos[i].param, &pos, fopAcM_GetRoomNo(i_this), NULL, NULL, -1);
    }
}

static void demo_camera(b_bq_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    camera_class* camera = dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0));
    camera_class* camera0 = dComIfGp_getCamera(0);
    daPy_py_c* player = (daPy_py_c*)dComIfGp_getPlayer(0);
    fopAc_ac_c* tentacle = fopAcM_SearchByID(i_this->mTentacleIDs[i_this->field_0x123c]);
    e_mb_class* monkeybomb = (e_mb_class*)fopAcM_SearchByID(i_this->mMonkeyBombID);

    cXyz spF0;
    cXyz spFC;
    cXyz sp108;
    cXyz sp114;

    f32 var_f31 = 0.0f;
    s8 var_r28 = false;

    switch (i_this->mDemoMode) {
    case 1:
        i_this->mDemoModeTimer = 0;
        i_this->mDemoMode = 2;
        // fallthrough
    case 2:
        i_this->mDemoModeTimer++;
        if (i_this->mDemoModeTimer < 7) {
            return;
        }

        if (!a_this->eventInfo.checkCommandDemoAccrpt()) {
            fopAcM_orderPotentialEvent(a_this, 2, 0xFFFF, 0);
            a_this->eventInfo.onCondition(dEvtCnd_CANDEMO_e);
            return;
        }

        camera->mCamera.Stop();

        i_this->mDemoMode = 3;
        i_this->mDemoModeTimer = 0;
        i_this->mDemoCamFovy = 55.0f;

        camera->mCamera.SetTrimSize(3);

        daPy_getPlayerActorClass()->changeOriginalDemo();
        daPy_getPlayerActorClass()->changeDemoMode(12, 0, 0, 0);

        Z2GetAudioMgr()->bgmStop(30, 0);

        dComIfGs_onOneZoneSwitch(6, -1);
        // fallthrough
    case 3:
        if (dComIfG_play_c::getLayerNo(0) == 4) {
            i_this->mDemoModeTimer = 290;
        }

        if (i_this->mDemoModeTimer == 0) {
            i_this->field_0x1298 = ZREG_F(0) + 1000.0f;

            i_this->mDemoCamEye.x = tentacle->current.pos.x;
            i_this->mDemoCamEye.y = tentacle->current.pos.y + ZREG_F(2);
            i_this->mDemoCamEye.z = tentacle->current.pos.z + i_this->field_0x1298;

            i_this->mDemoCamCenter.x = tentacle->current.pos.x;
            i_this->mDemoCamCenter.y = tentacle->current.pos.y + ZREG_F(1);
            i_this->mDemoCamCenter.z = tentacle->current.pos.z;
        } else {
            spFC = tentacle->current.pos;
            spFC.z += i_this->field_0x1298;

            cLib_addCalc2(&i_this->field_0x1298, 1500.0f, 0.05f, 5.0f);
            if (spFC.y < 300.0f) {
                spFC = i_this->mDemoCamEye;
                spFC.y = 300.0f;
            }

            cLib_addCalc2(&i_this->mDemoCamEye.x, spFC.x, 0.1f, 20.0f);
            cLib_addCalc2(&i_this->mDemoCamEye.y, spFC.y, 0.1f, 20.0f);
            cLib_addCalc2(&i_this->mDemoCamEye.z, spFC.z, 0.1f, 20.0f);

            spFC = tentacle->current.pos;
            if (spFC.y < 0.0f) {
                spFC = i_this->mDemoCamCenter;
                spFC.y = 0.0f;
            }

            cLib_addCalc2(&i_this->mDemoCamCenter.x, spFC.x, 0.1f, 50.0f);
            cLib_addCalc2(&i_this->mDemoCamCenter.y, spFC.y, 0.1f, 50.0f);
            cLib_addCalc2(&i_this->mDemoCamCenter.z, spFC.z, 0.1f, 50.0f);
        }

        if (i_this->mDemoModeTimer == 205) {
            i_this->mColpatType = 2;
        }

        var_r28 = 1;

        if (i_this->mDemoModeTimer != 290) {
            break;
        }

        i_this->mDemoMode = 4;
        i_this->mDemoModeTimer = 0;
        i_this->mDemoCamFovy = 30.0f;
        i_this->field_0x129c = 0.0f;

        i_this->mMode++;

        i_this->mDemoCamEyeTarget.x = KREG_F(0) + 400.0f;
        i_this->mDemoCamEyeTarget.y = KREG_F(1) + 1000.0f;
        i_this->mDemoCamEyeTarget.z = KREG_F(2) + 2200.0f;

        i_this->mDemoCamCenter = a_this->current.pos;

        spFC.set(173.0f, 0.0f, 1695.0f);
        daPy_getPlayerActorClass()->setPlayerPosAndAngle(&spFC, 0x8000, 0);
        // fallthrough
    case 4:
        if (i_this->mDemoModeTimer == 150) {
            fpcM_Search(s_bi_del_sub, i_this);
        }

        if (i_this->mDemoModeTimer == 270) {
            i_this->field_0x1394 = 20;
        }

        i_this->mDemoCamEye = a_this->current.pos;
        i_this->mDemoCamEye += i_this->mDemoCamEyeTarget;

        if (i_this->mDemoModeTimer >= 320) {
            cLib_addCalc2(&i_this->mDemoCamCenter.x, 300.0f - KREG_F(11), 0.1f, 13.0f);
        }

        if (i_this->mDemoModeTimer >= KREG_S(7) + 80) {
            cLib_addCalc2(&i_this->mDemoCamEyeTarget.x, KREG_F(15) + -100.0f, 0.05f,
                          i_this->field_0x12a0 * 750.0f);
            cLib_addCalc2(&i_this->mDemoCamEyeTarget.y, KREG_F(16) + 100.0f, 0.05f,
                          i_this->field_0x12a0 * 1350.0f);
            cLib_addCalc2(&i_this->field_0x12a0, 0.01f, 1.0f, 0.0005f);

            if (i_this->mDemoModeTimer >= 195) {
                cLib_addCalc2(&i_this->mDemoCamCenter.y, a_this->eyePos.y + 200.0f + NREG_F(8),
                              0.1f, 40.0f);
            }

            if (i_this->mDemoModeTimer > 200 && i_this->mDemoModeTimer < 280) {
                i_this->mBlureRateTarget = 180;
            } else {
                i_this->mBlureRateTarget = 0;
            }
        }

        if (i_this->mDemoModeTimer >= 20) {
            if (i_this->mDemoModeTimer == 20) {
                dComIfGp_getVibration().StartQuake(2, 1, cXyz(0.0f, 1.0f, 0.0f));
            }

            if (i_this->mDemoModeTimer < 190) {
                fpcM_Search(s_fw_sub1, i_this);
                cLib_addCalc2(&i_this->field_0x12a8, 10.0f, 1.0f, 1.05f);
            } else if (i_this->mDemoModeTimer == 190) {
                dComIfGp_getVibration().StopQuake(31);
                i_this->field_0x12a8 = 30.0f;

                dComIfGp_getVibration().StartQuake(8, 1, cXyz(0.0f, 1.0f, 0.0f));
                data_805BAD70 = 0;

                fpcM_Search(s_fw_sub2, i_this);
                i_this->field_0x6f9 = 4;
                daPy_getPlayerActorClass()->changeDemoMode(0x17, 1, 2, 0);
                i_this->mColpatType = 3;
            }

            if (i_this->mDemoModeTimer == 210) {
                dComIfGp_getVibration().StartQuake(6, 1, cXyz(0.0f, 1.0f, 0.0f));
            }

            if (i_this->mDemoModeTimer == 230) {
                dComIfGp_getVibration().StartQuake(4, 1, cXyz(0.0f, 1.0f, 0.0f));
            }

            if (i_this->mDemoModeTimer == 250) {
                dComIfGp_getVibration().StartQuake(2, 1, cXyz(0.0f, 1.0f, 0.0f));
            }

            if (i_this->mDemoModeTimer == 260) {
                dComIfGp_getVibration().StopQuake(31);
            }

            if (i_this->mDemoModeTimer == 340) {
                dComIfGp_getVibration().StartQuake(3, 1, cXyz(0.0f, 1.0f, 0.0f));
            }

            if (i_this->mDemoModeTimer == 390) {
                dComIfGp_getVibration().StopQuake(31);
            }

            cLib_addCalc2(&i_this->mDemoCamFovy, 55.0f, 0.05f, i_this->field_0x129c);
            cLib_addCalc2(&i_this->field_0x129c, 0.1f, 1.0f, 0.005f);
        } else {
            fpcM_Search(s_fw_sub0, i_this);
        }

        if (i_this->mDemoModeTimer == KREG_S(4) + 340) {
            player->changeDemoMode(0x22, 0, 0, 0);
        }

        if (i_this->mDemoModeTimer == 430) {
            i_this->mDemoMode = 100;
            Z2GetAudioMgr()->bgmStart(Z2BGM_BOSSBABA_2, 0, 0);
            i_this->field_0x1392 = 3;
        }
        break;
    case 10:
        if (!a_this->eventInfo.checkCommandDemoAccrpt()) {
            fopAcM_orderPotentialEvent(a_this, 2, 0xFFFF, 0);
            a_this->eventInfo.onCondition(dEvtCnd_CANDEMO_e);
            return;
        }

        camera->mCamera.Stop();

        i_this->mDemoMode = 11;
        i_this->mDemoModeTimer = 0;
        i_this->mDemoCamFovy = 50.0f;

        camera->mCamera.SetTrimSize(3);

        daPy_getPlayerActorClass()->changeOriginalDemo();

        i_this->mDemoCamEye = camera0->lookat.eye;
        i_this->mDemoCamCenter = camera0->lookat.center;

        dComIfGp_getEvent().startCheckSkipEdge(a_this);
        // fallthrough
    case 11:
        if (i_this->mDemoModeTimer == (s16)(VREG_S(0) + 8)) {
            daPy_getPlayerActorClass()->changeDemoMode(0x19, 0, 0, 0);
        }

        if (i_this->mDemoModeTimer < 50) {
            break;
        }

        if (i_this->mDemoModeTimer == 50) {
            i_this->mDemoCamFovy = 55.0f;

            i_this->mDemoCamCenter.set(58.0f, 355.0f, 1800.0f);
            i_this->field_0x1264 = i_this->mDemoCamCenter;
            i_this->mDemoCamEye.set(240.0f, 274.0f, 2075.0f);
            i_this->mDemoCamCenterTarget.set(411.0f, 355.0f, 1793.0f);
            i_this->mDemoCamEyeTarget.set(240.0f, 274.0f, 2075.0f);

            i_this->field_0x1288.x =
                fabsf(i_this->mDemoCamCenterTarget.x - i_this->mDemoCamCenter.x);
            i_this->field_0x1288.y =
                fabsf(i_this->mDemoCamCenterTarget.y - i_this->mDemoCamCenter.y);
            i_this->field_0x1288.z =
                fabsf(i_this->mDemoCamCenterTarget.z - i_this->mDemoCamCenter.z);
        }

        spFC.set(0.0f, 0.0f, 1700.0f);
        daPy_getPlayerActorClass()->setPlayerPosAndAngle(&spFC, 0x8000, 0);

        if (i_this->mDemoModeTimer > 70) {
            cam_3d_morf(i_this, 0.1f);
            cLib_addCalc2(&i_this->field_0x129c, 0.005f, 1.0f, 0.00015f);

            spF0 = i_this->mDemoCamCenterTarget - i_this->field_0x1264;
            f32 temp_f31 = spF0.abs();

            spF0 = i_this->mDemoCamCenterTarget - i_this->mDemoCamCenter;
            f32 temp_f30 = spF0.abs();

            f32 f28 = temp_f31 * (BREG_F(19) + 0.1f);
            s16 sp0C = (temp_f30 / temp_f31) * (f32)0x8000;
            var_f31 = f28 * cM_ssin(sp0C);
        }

        if (i_this->mDemoModeTimer != (s16)(VREG_S(8) + 320)) {
            break;
        }

        i_this->mDemoCamCenter.set(1444.0f, 121.0f, 1783.0f);
        i_this->mDemoCamEye.set(1685.0f, 95.0f, 2021.0f);
        i_this->mDemoCamCenterTarget.set(76.0f, 204.0f, 1782.0f);
        i_this->mDemoCamEyeTarget.set(-41.0f, 261.0f, 2095.0f);

        i_this->field_0x127c.x = fabsf(i_this->mDemoCamEyeTarget.x - i_this->mDemoCamEye.x);
        i_this->field_0x127c.y = fabsf(i_this->mDemoCamEyeTarget.y - i_this->mDemoCamEye.y);
        i_this->field_0x127c.z = fabsf(i_this->mDemoCamEyeTarget.z - i_this->mDemoCamEye.z);

        i_this->field_0x1288.x =
            fabsf(i_this->mDemoCamCenterTarget.x - i_this->mDemoCamCenter.x);
        i_this->field_0x1288.y =
            fabsf(i_this->mDemoCamCenterTarget.y - i_this->mDemoCamCenter.y);
        i_this->field_0x1288.z =
            fabsf(i_this->mDemoCamCenterTarget.z - i_this->mDemoCamCenter.z);

        i_this->field_0x129c = 0.0f;
        i_this->mDemoMode = 12;
        i_this->mDemoModeTimer = 0;
        // fallthrough
    case 12:
        if (i_this->mDemoModeTimer == (s16)(VREG_S(0) + 260)) {
            i_this->field_0x6f9 = 2;
        }

        cam_3d_morf(i_this, 0.1f);
        cLib_addCalc2(&i_this->field_0x129c, 0.005f, 1.0f, 0.00015f);

        if (i_this->mDemoModeTimer == (s16)(VREG_S(2) + 340)) {
            i_this->mDemoCamCenter = tentacle->current.pos;
            i_this->mDemoCamCenter.y = VREG_F(6) + 0.0f;

            i_this->mDemoCamEye.set(1394.0f, 313.0f, -300.0f);
            i_this->mDemoMode = 13;
            i_this->mDemoModeTimer = 0;
            i_this->field_0x129c = 0.0f;

            mDoAud_seStart(Z2SE_EN_BH_JINARI, NULL, 0, 0);
            Z2GetAudioMgr()->subBgmStart(Z2BGM_BOSSBABA_0);
        }
        break;
    case 13:
        if (i_this->mDemoModeTimer == 2) {
            dComIfGp_getVibration().StartQuake(1, 1, cXyz(0.0f, 1.0f, 0.0f));
        }

        if (i_this->mDemoModeTimer == 30) {
            dComIfGp_getVibration().StartQuake(1, 1, cXyz(0.0f, 1.0f, 0.0f));
        }

        if (i_this->mDemoModeTimer == 60) {
            dComIfGp_getVibration().StartQuake(2, 1, cXyz(0.0f, 1.0f, 0.0f));
        }

        if (i_this->mDemoModeTimer == 111) {
            dComIfGp_getVibration().StopQuake(31);
        }

        if (i_this->mDemoModeTimer == 113) {
            dComIfGp_getVibration().StartQuake(6, 1, cXyz(0.0f, 1.0f, 0.0f));
        }

        if (i_this->mDemoModeTimer == 150) {
            dComIfGp_getVibration().StopQuake(31);
        }

        if (i_this->mDemoModeTimer == (s16)(VREG_S(2) + 100)) {
            i_this->field_0x6f9 = 2;
        }

        if (i_this->mDemoModeTimer == (s16)(VREG_S(4) + 53)) {
            i_this->mColpatType = 15;
        }

        if (i_this->mDemoModeTimer >= (s16)(VREG_S(3) + 110)) {
            cLib_addCalc2(&i_this->mDemoCamCenter.x, tentacle->current.pos.x, 0.1f,
                          VREG_F(5) + 20.0f);

            if (tentacle->current.pos.y + 100.0f > 0.0f) {
                cLib_addCalc2(&i_this->mDemoCamCenter.y, tentacle->current.pos.y + 100.0f, 0.8f,
                              VREG_F(5) + 60.0f);
            }

            cLib_addCalc2(&i_this->mDemoCamCenter.z, tentacle->current.pos.z, 0.1f,
                          VREG_F(5) + 20.0f);

            if (i_this->mDemoModeTimer == (s16)(VREG_S(4) + 113)) {
                i_this->field_0x6f9 = 2;
                i_this->mColpatType = 1;
            }

            if (i_this->mDemoModeTimer >= (s16)(VREG_S(5) + 150)) {
                cLib_addCalc2(&i_this->mDemoCamEye.y, VREG_F(6) + 1413.0f, 0.1f,
                              i_this->field_0x129c * (VREG_F(7) + 40.0f));
                cLib_addCalc2(&i_this->field_0x129c, 1.0f, 1.0f, 0.02f);

                if (i_this->mDemoModeTimer == (s16)(VREG_S(6) + 200)) {
                    daPy_getPlayerActorClass()->changeDemoMode(0x17, 1, 2, 0);
                }
            }
        }

        if (i_this->mDemoModeTimer != (s16)(VREG_S(7) + 280)) {
            break;
        }

        i_this->mDemoCamCenter.set(0.0f, 430.0f, 524.0f);
        i_this->mDemoCamEye.set(0.0f, 240.0f, 1440.0f);
        i_this->mDemoCamCenterTarget.set(0.0f, 278.0f, 1252.0f);
        i_this->mDemoCamEyeTarget.set(0.0f, 86.0f, 2167.0f);

        i_this->field_0x127c.x = fabsf(i_this->mDemoCamEyeTarget.x - i_this->mDemoCamEye.x);
        i_this->field_0x127c.y = fabsf(i_this->mDemoCamEyeTarget.y - i_this->mDemoCamEye.y);
        i_this->field_0x127c.z = fabsf(i_this->mDemoCamEyeTarget.z - i_this->mDemoCamEye.z);

        i_this->field_0x1288.x =
            fabsf(i_this->mDemoCamCenterTarget.x - i_this->mDemoCamCenter.x);
        i_this->field_0x1288.y =
            fabsf(i_this->mDemoCamCenterTarget.y - i_this->mDemoCamCenter.y);
        i_this->field_0x1288.z =
            fabsf(i_this->mDemoCamCenterTarget.z - i_this->mDemoCamCenter.z);

        i_this->field_0x129c = 0.0f;
        i_this->mDemoMode = 14;
        i_this->mDemoModeTimer = 0;
        // fallthrough
    case 14:
        cam_3d_morf(i_this, 0.2f);
        cLib_addCalc2(&i_this->field_0x129c, 0.02f, 1.0f, 0.001f);

        if (i_this->mDemoModeTimer == 20) {
            fopMsgM_messageSetDemo(1151);  // Twilit Parasite DIABABA
        }

        if (i_this->mDemoModeTimer == 130) {
            i_this->mDemoMode = 100;
            i_this->field_0x6f9 = 2;

            Z2GetAudioMgr()->bgmStart(Z2BGM_BOSSBABA_1, 0, 0);
            i_this->field_0x1392 = 2;
        }
        break;
    case 20:
        i_this->mDemoModeTimer = 0;
        i_this->mDemoMode = 21;
        // fallthrough
    case 21:
        i_this->mDemoModeTimer++;
        if (i_this->mDemoModeTimer < 7) {
            return;
        }

        if (!a_this->eventInfo.checkCommandDemoAccrpt()) {
            fopAcM_orderPotentialEvent(a_this, 2, 0xFFFF, 0);
            a_this->eventInfo.onCondition(dEvtCnd_CANDEMO_e);
            return;
        }

        camera->mCamera.Stop();

        i_this->mDemoMode = 22;
        i_this->mDemoModeTimer = 0;
        i_this->mDemoCamFovy = 55.0f;

        camera->mCamera.SetTrimSize(3);

        daPy_getPlayerActorClass()->changeOriginalDemo();
        dComIfGs_onOneZoneSwitch(5, -1);
        // fallthrough
    case 22:
        if (i_this->mDemoModeTimer == 0) {
            i_this->field_0x1298 = ZREG_F(0) + 1000.0f;

            i_this->mDemoCamEye.x = tentacle->current.pos.x;
            i_this->mDemoCamEye.y = tentacle->current.pos.y + ZREG_F(2);
            i_this->mDemoCamEye.z = tentacle->current.pos.z + i_this->field_0x1298;

            i_this->mDemoCamCenter.x = tentacle->current.pos.x;
            i_this->mDemoCamCenter.y = tentacle->current.pos.y + ZREG_F(1);
            i_this->mDemoCamCenter.z = tentacle->current.pos.z;
        } else {
            spFC = tentacle->current.pos;
            spFC.z += i_this->field_0x1298;

            cLib_addCalc2(&i_this->field_0x1298, 1500.0f, 0.05f, 5.0f);
            if (spFC.y < 300.0f) {
                spFC = i_this->mDemoCamEye;
                spFC.y = 300.0f;
            }

            cLib_addCalc2(&i_this->mDemoCamEye.x, spFC.x, 0.1f, 20.0f);
            cLib_addCalc2(&i_this->mDemoCamEye.y, spFC.y, 0.1f, 20.0f);
            cLib_addCalc2(&i_this->mDemoCamEye.z, spFC.z, 0.1f, 20.0f);

            spFC = tentacle->current.pos;
            if (spFC.y < 0.0f) {
                spFC = i_this->mDemoCamCenter;
                spFC.y = 0.0f;
            }

            cLib_addCalc2(&i_this->mDemoCamCenter.x, spFC.x, 0.1f, 50.0f);
            cLib_addCalc2(&i_this->mDemoCamCenter.y, spFC.y, 0.1f, 50.0f);
            cLib_addCalc2(&i_this->mDemoCamCenter.z, spFC.z, 0.1f, 50.0f);
        }

        var_r28 = true;

        if (i_this->mDemoModeTimer == 230) {
            i_this->mDemoMode = 100;
        }
        break;
    case 30:
        if (!monkeybomb->eventInfo.checkCommandDemoAccrpt()) {
            fopAcM_orderPotentialEvent(monkeybomb, 2, 0xFFFF, 0);
            monkeybomb->eventInfo.onCondition(dEvtCnd_CANDEMO_e);
            return;
        }

        camera->mCamera.Stop();

        i_this->mDemoMode = 31;
        i_this->mDemoModeTimer = 0;
        i_this->mDemoCamFovy = 55.0f;

        camera->mCamera.SetTrimSize(3);

        daPy_getPlayerActorClass()->changeOriginalDemo();
        daPy_getPlayerActorClass()->changeDemoMode(1, 1, 0, 0);
        Z2GetAudioMgr()->changeBgmStatus(1);
        dComIfGs_onOneZoneSwitch(7, -1);
        // fallthrough
    case 31:
        i_this->mDemoMode = 0x20;
        // fallthrough
    case 32:
    case 33:
        spFC.set(0.0f, 0.0f, 2200.0f);
        daPy_getPlayerActorClass()->setPlayerPosAndAngle(&spFC, 0x8000, 0);

        if (i_this->mDemoModeTimer == 0) {
            i_this->mDemoCamCenter = monkeybomb->current.pos;
        }

        i_this->mDemoCamEye.x = BREG_F(10) + -900.0f;
        i_this->mDemoCamEye.y = BREG_F(11) + 1000.0f;
        i_this->mDemoCamEye.z = BREG_F(12) + 1471.0f;

        MTXCopy(monkeybomb->mpMorf->getModel()->getAnmMtx(0x1C), mDoMtx_stack_c::get());

        mDoMtx_stack_c::transM(JREG_F(17) + 60.0f, JREG_F(18) + -35.0f, JREG_F(19) + 15.0f);
        mDoMtx_stack_c::multVecZero(&sp114);

        cLib_addCalc2(&i_this->mDemoCamCenter.x, sp114.x, BREG_F(14) + 0.6f, 100.0f);
        cLib_addCalc2(&i_this->mDemoCamCenter.y, sp114.y + 50.0f + BREG_F(13), BREG_F(14) + 0.6f,
                      100.0f);
        cLib_addCalc2(&i_this->mDemoCamCenter.z, sp114.z, BREG_F(14) + 0.6f, 100.0f);

        if (i_this->mDemoMode != 33) {
            break;
        }

        i_this->mDemoMode = 34;

        i_this->mDemoCamCenter.set(-766.0f, 423.0f, 1593.0f);
        i_this->mDemoCamEye.set(71.0f, 114.0f, 2304.0f);
        i_this->mDemoCamCenterTarget.set(-2243.0f, 1340.0f, 977.0f);
        i_this->mDemoCamEyeTarget.set(-1226.0f, 980.0f, 1350.0f);

        i_this->field_0x127c.x = fabsf(i_this->mDemoCamEyeTarget.x - i_this->mDemoCamEye.x);
        i_this->field_0x127c.y = fabsf(i_this->mDemoCamEyeTarget.y - i_this->mDemoCamEye.y);
        i_this->field_0x127c.z = fabsf(i_this->mDemoCamEyeTarget.z - i_this->mDemoCamEye.z);

        i_this->field_0x1288.x =
            fabsf(i_this->mDemoCamCenterTarget.x - i_this->mDemoCamCenter.x);
        i_this->field_0x1288.y =
            fabsf(i_this->mDemoCamCenterTarget.y - i_this->mDemoCamCenter.y);
        i_this->field_0x1288.z =
            fabsf(i_this->mDemoCamCenterTarget.z - i_this->mDemoCamCenter.z);

        i_this->field_0x129c = 0.0f;
        i_this->mDemoMode = 34;
        i_this->mDemoModeTimer = 0;
        // fallthrough
    case 34:
        if (i_this->mDemoModeTimer > 40) {
            cam_3d_morf(i_this, 0.2f);
            cLib_addCalc2(&i_this->field_0x129c, 0.1f, 1.0f, 0.005f);

            if (i_this->mDemoModeTimer > (s16)(JREG_S(7) + 133)) {
                cLib_addCalc2(&i_this->mDemoCamFovy, 30.0f, 0.5f, 4.0f);
            }
        }

        if (i_this->mDemoModeTimer != 210) {
            break;
        }

        i_this->mDemoCamEye.set(-560.0f, 717.0f, 1948.0f);
        i_this->mDemoCamCenter = monkeybomb->current.pos;
        i_this->mDemoCamCenter.y += JREG_F(12) + 200.0f;

        i_this->mDemoMode = 35;
        i_this->mDemoModeTimer = 0;
        i_this->mDemoCamFovy = 55.0f;

        monkeybomb->mMode++;
        // fallthrough
    case 35:
        MTXCopy(monkeybomb->mpMorf->getModel()->getAnmMtx(0), mDoMtx_stack_c::get());
        mDoMtx_stack_c::multVecZero(&sp114);

        cLib_addCalc2(&i_this->mDemoCamCenter.x, sp114.x, BREG_F(14) + 0.6f, 100.0f);
        cLib_addCalc2(&i_this->mDemoCamCenter.y, sp114.y + 200.0f + JREG_F(12), BREG_F(14) + 0.6f,
                      100.0f);
        cLib_addCalc2(&i_this->mDemoCamCenter.z, sp114.z, BREG_F(14) + 0.6f, 100.0f);

        if (i_this->mDemoModeTimer == 50) {
            i_this->mDemoMode = 100;
        }
        break;
    case 50: {
        if (!a_this->eventInfo.checkCommandDemoAccrpt()) {
            fopAcM_orderPotentialEvent(a_this, 2, 0xFFFF, 0);
            a_this->eventInfo.onCondition(dEvtCnd_CANDEMO_e);
            return;
        }

        camera->mCamera.Stop();

        i_this->mDemoMode = 51;
        i_this->mDemoModeTimer = 0;
        i_this->mDemoCamFovy = 55.0f;

        camera->mCamera.SetTrimSize(3);

        daPy_getPlayerActorClass()->changeOriginalDemo();

        i_this->mDemoCamCenter = a_this->eyePos;
        i_this->mDemoCamEye.set(95.0f, 50.0f, 2800.0f);
        i_this->mDemoCamEyeTarget.set(72.0f, 52.0f, 2153.0f);

        i_this->field_0x127c.x = fabsf(i_this->mDemoCamEyeTarget.x - i_this->mDemoCamEye.x);
        i_this->field_0x127c.y = fabsf(i_this->mDemoCamEyeTarget.y - i_this->mDemoCamEye.y);
        i_this->field_0x127c.z = fabsf(i_this->mDemoCamEyeTarget.z - i_this->mDemoCamEye.z);

        i_this->field_0x1288.set(0.0f, 0.0f, 0.0f);
        i_this->field_0x129c = 0.0f;

        Z2GetAudioMgr()->bgmStop(30, 0);
        Z2GetAudioMgr()->bgmStreamPrepare(0x2000016);
        Z2GetAudioMgr()->bgmStreamPlay();

        dComIfGs_BossLife_public_Set(0);

        e_mb_class* monkey_bomb = (e_mb_class*)fopAcM_SearchByID(i_this->mMonkeyBombID);
        monkey_bomb->mAction = 5;
        // fallthrough
    }
    case 51:
        if (i_this->mDemoModeTimer == 24 || i_this->mDemoModeTimer == 64 ||
            i_this->mDemoModeTimer == 95 || i_this->mDemoModeTimer == 122)
        {
            dComIfGp_getVibration().StartShock(4, 1, cXyz(0.0f, 1.0f, 0.0f));
        }

        if (i_this->mDemoModeTimer == 140) {
            dComIfGp_getVibration().StartQuake(2, 1, cXyz(0.0f, 1.0f, 0.0f));
        }

        if (i_this->mDemoModeTimer == 200) {
            dComIfGp_getVibration().StopQuake(31);
        }

        if (i_this->mDemoModeTimer < 200) {
            i_this->mBlureRateTarget = 180;
        } else {
            i_this->mBlureRateTarget = 0;
        }

        if (i_this->mDemoModeTimer == 260) {
            i_this->mColpatType = 4;
        }

        if (i_this->mDemoModeTimer >= (s16)(TREG_S(5) + 380)) {
            cam_3d_morf(i_this, 0.1f);
            cLib_addCalc2(&i_this->field_0x129c, 0.05f, 1.0f, 0.001f);
        }

        if (i_this->mDemoModeTimer == (s16)(TREG_S(6) + 430)) {
            i_this->field_0x5c8 = 1;
            i_this->field_0x5cc = a_this->eyePos;
        }

        if (i_this->mDemoModeTimer < 350) {
            spFC.set(BREG_F(5) + -150.0f, 0.0f, BREG_F(6) + 2288.0f);
        } else {
            spFC.set(BREG_F(5) + -30.0f, 0.0f, BREG_F(6) + 1938.0f);
        }

        daPy_getPlayerActorClass()->setPlayerPosAndAngle(&spFC, BREG_S(5) + 0x8000, 0);

        if (i_this->field_0x5c8 != 0) {
            i_this->field_0x5cc.y += i_this->field_0x5d8;
            i_this->field_0x5d8 -= 5.0f;

            if (i_this->field_0x5cc.y <= BREG_F(10) + 25.0f) {
                i_this->field_0x5cc.y = BREG_F(10) + 25.0f;

                if (i_this->field_0x5d8 < -30.0f) {
                    fopAcM_effSmokeSet1(&i_this->field_0x1220, &i_this->field_0x1224,
                                        &i_this->field_0x5cc, NULL, TREG_F(11) + 1.0f,
                                        &a_this->tevStr, 1);
                    mDoAud_seStart(Z2SE_EN_BQ_EYE_LAND, NULL, 0, 0);
                    i_this->field_0x5d8 = 15.0f;

                    dComIfGp_getVibration().StartShock(6, 31, cXyz(0.0f, 1.0f, 0.0f));
                } else {
                    i_this->field_0x5d8 = 0.0f;
                }
            } else if (i_this->mDemoModeTimer >= (s16)(TREG_S(6) + 435)) {
                i_this->field_0x5cc.z += BREG_F(9) + 6.0f;
            }

            cLib_addCalcAngleS2(&i_this->field_0x5dc, -0x4000, 8, 0x300);

            MtxTrans(i_this->field_0x5cc.x, i_this->field_0x5cc.y, i_this->field_0x5cc.z, 0);
            cMtx_YrotM(*calc_mtx, a_this->shape_angle.y);
            cMtx_XrotM(*calc_mtx, i_this->field_0x5dc);
            i_this->mpEyeballModel->setBaseTRMtx(*calc_mtx);

            cLib_addCalc2(&i_this->mDemoCamCenter.x, i_this->field_0x5cc.x, 0.1f, 2.0f);
            cLib_addCalc2(&i_this->mDemoCamCenter.z, i_this->field_0x5cc.z, 0.1f, 2.0f);
            cLib_addCalc2(&i_this->mDemoCamCenter.y, i_this->field_0x5cc.y + 30.0f + BREG_F(14),
                          BREG_F(19) + 0.3f, BREG_F(17) + 100.0f);

            if (i_this->mDemoModeTimer == 535) {
                i_this->field_0x5c8 = 2;

                for (int i = 0; i < 7; i++) {
                    static u16 g_b_i[] = {0x831C, 0x831D, 0x831E, 0x831F, 0x8320, 0x8321, 0x8369};

                    dComIfGp_particle_set(g_b_i[i], &i_this->field_0x5cc, NULL, NULL);
                }

                mDoAud_seStart(Z2SE_EN_BQ_EYE_EXPLODE, &i_this->field_0x5cc, 0, 0);
                dComIfGp_getVibration().StartShock(5, 1, cXyz(0.0f, 1.0f, 0.0f));

                cXyz sp120(1.0f, 1.0f, 1.0f);
                fopAcM_createItemForBoss(&i_this->field_0x5cc, fpcNm_ITEM_UTAWA_HEART,
                                         fopAcM_GetRoomNo(a_this), &a_this->shape_angle, &sp120,
                                         0.0f, 0.0f, -1);

                fopAc_ac_c* sp28;
                sp28 = fopAcM_SearchByID(i_this->mTentacleIDs[0]);
                fopAcM_delete(sp28);
                sp28 = fopAcM_SearchByID(i_this->mTentacleIDs[1]);
                fopAcM_delete(sp28);
            }

            if (i_this->mDemoModeTimer == 535) {
                i_this->mDemoMode = 52;
                i_this->field_0x129c = 0.0f;
                i_this->mDemoModeTimer = 0;
                i_this->mDemoCamFovy = 55.0f;
            }

            if (i_this->mDemoModeTimer >= 480) {
                cLib_addCalc2(&i_this->field_0x1178, 1.0f, 1.0f, 0.03f);
            }
        } else {
            sp114 = a_this->eyePos;
            sp114.y += BREG_F(14) + 40.0f;

            if (sp114.y > BREG_F(12) + 1800.0f) {
                sp114.y = BREG_F(12) + 1800.0f;
            } else if (sp114.y < BREG_F(13) + 700.0f) {
                sp114.y = BREG_F(13) + 700.0f;
            }

            cLib_addCalc2(&i_this->mDemoCamCenter.x, sp114.x, 0.1f, BREG_F(17) + 30.0f);
            cLib_addCalc2(&i_this->mDemoCamCenter.y, sp114.y, 0.1f, BREG_F(17) + 30.0f);
            cLib_addCalc2(&i_this->mDemoCamCenter.z, sp114.z, 0.1f, BREG_F(17) + 30.0f);
        }
        break;
    case 52:
        if (i_this->mDemoModeTimer == 30) {
            i_this->field_0x1396 = 1;
            player->changeDemoMode(30, 1, 0, 0);
        }

        if (i_this->mDemoModeTimer == 140) {
            daPy_getPlayerActorClass()->changeDemoMode(1, 0, 0, 0);
            i_this->mColpatType = 7;

            i_this->mDemoCamCenter.set(92.0f, 500.0f, 2654.0f);
            i_this->mDemoCamEye.set(110.0f, 560.0f, 3090.0f);
            i_this->mDemoCamCenterTarget.set(939.0f, 434.0f, 2362.0f);
            i_this->mDemoCamEyeTarget.set(1214.0f, 350.0f, 2696.0f);

            i_this->field_0x127c.x =
                fabsf(i_this->mDemoCamEyeTarget.x - i_this->mDemoCamEye.x);
            i_this->field_0x127c.y =
                fabsf(i_this->mDemoCamEyeTarget.y - i_this->mDemoCamEye.y);
            i_this->field_0x127c.z =
                fabsf(i_this->mDemoCamEyeTarget.z - i_this->mDemoCamEye.z);

            i_this->field_0x1288.x =
                fabsf(i_this->mDemoCamCenterTarget.x - i_this->mDemoCamCenter.x);
            i_this->field_0x1288.y =
                fabsf(i_this->mDemoCamCenterTarget.y - i_this->mDemoCamCenter.y);
            i_this->field_0x1288.z =
                fabsf(i_this->mDemoCamCenterTarget.z - i_this->mDemoCamCenter.z);

            i_this->field_0x129c = 0.0f;
        } else if (i_this->mDemoModeTimer < 140) {
            if (i_this->mDemoModeTimer == 1) {
                i_this->mDemoCamCenterTarget.set(2.0f, 111.0f, 1689.0f);
                i_this->mDemoCamEyeTarget.set(23.0f, 108.0f, 2155.0f);

                i_this->field_0x127c.x =
                    fabsf(i_this->mDemoCamEyeTarget.x - i_this->mDemoCamEye.x);
                i_this->field_0x127c.y =
                    fabsf(i_this->mDemoCamEyeTarget.y - i_this->mDemoCamEye.y);
                i_this->field_0x127c.z =
                    fabsf(i_this->mDemoCamEyeTarget.z - i_this->mDemoCamEye.z);

                i_this->field_0x1288.x =
                    fabsf(i_this->mDemoCamCenterTarget.x - i_this->mDemoCamCenter.x);
                i_this->field_0x1288.y =
                    fabsf(i_this->mDemoCamCenterTarget.y - i_this->mDemoCamCenter.y);
                i_this->field_0x1288.z =
                    fabsf(i_this->mDemoCamCenterTarget.z - i_this->mDemoCamCenter.z);
            }

            cam_3d_morf(i_this, 0.1f);
            cLib_addCalc2(&i_this->field_0x129c, 0.02f, 1.0f, 0.0003f);
        }

        if (i_this->mDemoModeTimer >= 170) {
            cam_3d_morf(i_this, 0.05f);
            cLib_addCalc2(&i_this->field_0x129c, 0.007f, 1.0f, 0.00012f);

            if (i_this->mDemoModeTimer == 260) {
                i_this->mSetDeadColor = true;
            }

            if (i_this->mDemoModeTimer == 410) {
                i_this->mSetBossExplode = true;
            }

            if (i_this->mDemoModeTimer == 412) {
                i_this->mDisableDraw = true;
            }

            if (i_this->mDemoModeTimer >= 415) {
                cLib_addCalc2(&i_this->mDemoCamFovy, 70.0f, 0.05f, 0.1f);
            }

            if (i_this->mDemoModeTimer == 532) {
                i_this->mDemoMode = 53;
                i_this->mDemoModeTimer = 0;
                i_this->mDemoCamFovy = 55.0f;

                i_this->mDemoCamCenter.set(1072.0f, 1121.0f, -379.0f);
                i_this->mDemoCamEye.set(1320.0f, 1293.0f, -701.0f);
                i_this->mDemoCamCenterTarget.set(758.0f, 853.0f, 332.0f);
                i_this->mDemoCamEyeTarget.set(953.0f, 997.0f, -36.0f);

                i_this->field_0x127c.x =
                    fabsf(i_this->mDemoCamEyeTarget.x - i_this->mDemoCamEye.x);
                i_this->field_0x127c.y =
                    fabsf(i_this->mDemoCamEyeTarget.y - i_this->mDemoCamEye.y);
                i_this->field_0x127c.z =
                    fabsf(i_this->mDemoCamEyeTarget.z - i_this->mDemoCamEye.z);

                i_this->field_0x1288.x =
                    fabsf(i_this->mDemoCamCenterTarget.x - i_this->mDemoCamCenter.x);
                i_this->field_0x1288.y =
                    fabsf(i_this->mDemoCamCenterTarget.y - i_this->mDemoCamCenter.y);
                i_this->field_0x1288.z =
                    fabsf(i_this->mDemoCamCenterTarget.z - i_this->mDemoCamCenter.z);

                cXyz pos(0.0f, 0.0f, 0.0f);
                for (int i = 0; i < 5; i++) {
                    static u16 g_e_i[] = {0x83EB, 0x83EC, 0x83ED, 0x83EE, 0x83EF};

                    dComIfGp_particle_set(g_e_i[i], &pos, NULL, NULL);
                }

                i_this->mSound.startCreatureSound(Z2SE_EN_BOSS_CONVERGE, 0, 0);
            }
        }
        break;
    case 53:
        if (i_this->mDemoModeTimer >= 100) {
            cam_3d_morf(i_this, 0.2f);
            cLib_addCalc2(&i_this->field_0x129c, 0.15f, 1.0f, 0.002f);
        }

        if (i_this->mDemoModeTimer == 100) {
            cMtx_YrotS(*calc_mtx, player->shape_angle.y);
            spF0.set(0.0f, JREG_F(8) + 300.0f, JREG_F(9) + 100.0f);
            MtxPosition(&spF0, &spFC);
            spFC += player->current.pos;

            OS_REPORT("///YSTONE POS  %d,%d,%d\n", (int)spFC.x, (int)spFC.y, (int)spFC.z);
            fopAcM_create(PROC_OBJ_YSTONE, 0, &spFC, fopAcM_GetRoomNo(a_this), NULL, NULL, -1);
        }

        if (i_this->mDemoModeTimer == 120) {
            i_this->field_0x1151 = 1;

            obj_ystone_class* ystone = (obj_ystone_class*)fopAcM_SearchByName(PROC_OBJ_YSTONE);
            if (ystone != NULL) {
                ystone->field_0x59b = 0;
            }
        }

        if (i_this->mDemoModeTimer == 180) {
            i_this->mDemoMode = 54;
            i_this->mDemoModeTimer = 0;

            spFC.set(121.0f, 0.0f, 2486.0f);
            daPy_getPlayerActorClass()->setPlayerPosAndAngle(&spFC, 0x8000, 0);
            i_this->field_0x1298 = JREG_F(8) + 300.0f;
        }
        break;
    case 54:
        i_this->mDemoCamCenter.set(118.0f, 240.0f, 2349.0f);
        i_this->mDemoCamEye.set(116.0f, 100.0f, 2766.0f);

        cMtx_YrotS(*calc_mtx, player->shape_angle.y);
        spF0.set(0.0f, i_this->field_0x1298, JREG_F(9) + 100.0f);
        MtxPosition(&spF0, &spFC);
        spFC += player->current.pos;

        cLib_addCalc2(&i_this->field_0x1298, 210.0f, 0.05f, 0.5f);

        obj_ystone_class* ystone = (obj_ystone_class*)fopAcM_SearchByName(PROC_OBJ_YSTONE);
        if (ystone != NULL) {
            ystone->setCurrentPos(spFC);
        }

        if (i_this->mDemoModeTimer == 10) {
            cXyz pos(-234.0f, 0.0f, 2191.0f);
            csXyz angle(0, 0, 0);

            fopAcM_createWarpHole(&pos, &angle, fopAcM_GetRoomNo(a_this), 0, 1, 0xFF);
        }

        if (i_this->mDemoModeTimer == 110) {
            i_this->mDemoMode = 100;
            i_this->field_0x1151 = 1;

            dComIfGs_onStageBossEnemy();
            int sw = (fopAcM_GetParam(a_this) & 0x00FF0000) >> 0x10;
            dComIfGs_offSwitch(sw, fopAcM_GetRoomNo(a_this));

            fish_set(i_this);
        }
    }

    if (i_this->mDemoMode >= 11 && i_this->mDemoMode < 20) {
        if (dComIfGp_getEvent().checkSkipEdge()) {
            cDmr_SkipInfo = 20;
            Z2GetAudioMgr()->subBgmStop();
            OS_REPORT("//////////////B_BQ SKIP 11\n");

            int sw = (fopAcM_GetParam(a_this) & 0x0000FF00) >> 8;
            dComIfGs_onSwitch(sw, fopAcM_GetRoomNo(a_this));
            dStage_changeScene(1, 0.0f, 0, fopAcM_GetRoomNo(a_this), 0, -1);
            i_this->mDemoMode = 1000;
            dComIfGp_event_reset();
            return;
        }
    }

    if (i_this->mDemoMode == 100) {
        camera->mCamera.Reset(i_this->mDemoCamCenter, i_this->mDemoCamEye);
        camera->mCamera.Start();
        camera->mCamera.SetTrimSize(0);
        dComIfGp_event_reset();

        daPy_getPlayerActorClass()->cancelOriginalDemo();
        i_this->mDemoMode = 0;
        OS_REPORT("//////////////B_BQ SKIP 12\n");
    }

    if (i_this->mDemoMode != 0) {
        s16 bank = i_this->field_0x12a8 * cM_scos(i_this->mDemoModeTimer * 0x2C00) * 7.5f;

        cXyz center(i_this->mDemoCamCenter);
        center.y += var_f31;

        cXyz eye(i_this->mDemoCamEye);

        if (var_r28) {
            dBgS_LinChk lin_chk;
            lin_chk.Set(&center, &eye, a_this);

            if (dComIfG_Bgsp().LineCross(&lin_chk)) {
                eye = lin_chk.GetCross();
            }
        }

        camera->mCamera.Set(center, eye, bank, i_this->mDemoCamFovy);

        cLib_addCalc0(&i_this->field_0x12a8, 1.0f, 1.0f);
        i_this->mDemoModeTimer++;
    }

    if (i_this->mSetDeadColor) {
        cLib_addCalc2(&i_this->mDeadColor, -20.0f, 1.0f, 0.16667f);
    }
}

static int daB_BQ_Execute(b_bq_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;

    if (cDmrNowMidnaTalk()) {
        return 1;
    }

    fopAc_ac_c* player = (fopAc_ac_c*)dComIfGp_getPlayer(0);
    cXyz sp44;
    cXyz sp50;

    if (i_this->mDemoMode == 1000) {
        return 1;
    }

    if (cDmr_SkipInfo != 0) {
        cDmr_SkipInfo--;
    }

    i_this->field_0x6c8++;

    for (int i = 0; i < 5; i++) {
        if (i_this->mTimers[i] != 0) {
            i_this->mTimers[i]--;
        }
    }

    if (i_this->field_0x6de != 0) {
        i_this->field_0x6de--;
    }

    if (i_this->field_0x6fc != 0) {
        i_this->field_0x6fc--;
    }

    if (i_this->field_0x6fe != 0) {
        i_this->field_0x6fe--;
    }

    action(i_this);

    mDoMtx_stack_c::transS(a_this->current.pos.x, a_this->current.pos.y, a_this->current.pos.z);
    mDoMtx_stack_c::YrotM(a_this->shape_angle.y);
    mDoMtx_stack_c::scaleM(l_HIO.mModelSize, l_HIO.mModelSize, l_HIO.mModelSize);

    J3DModel* model = i_this->mpMorf->getModel();
    model->setBaseTRMtx(mDoMtx_stack_c::get());

    i_this->mpMorf->play(0, dComIfGp_getReverb(fopAcM_GetRoomNo(a_this)));

    if (i_this->mpDeadBrk->getPlaySpeed() > 0.0f) {
        i_this->mpDeadBrk->play();
    }

    if (i_this->mPlayTodomeBtk) {
        i_this->mpTodomeBtk->play();
    }

    i_this->mpMorf->modelCalc();
    anm_se_set(i_this);

    cXyz sp5C(-20000.0f, -40000.0f, 30000.0f);
    MTXCopy(model->getAnmMtx(YREG_S(0) + 0x35), *calc_mtx);

    if (i_this->mAction == ACTION_END) {
        sp44.set(XREG_F(3), XREG_F(4), XREG_F(5));
    } else {
        sp44.set(XREG_F(3) + 30.0f, XREG_F(4), XREG_F(5));
    }

    MtxPosition(&sp44, &a_this->eyePos);
    a_this->attention_info.position = a_this->eyePos;
    a_this->attention_info.position.y += XREG_F(6) + 70.0f;

    cXyz sp68(0.0f, 0.0f, 0.0f);

    if (i_this->mAction == ACTION_END) {
        sp68.y += 10000.0f;
    }

    if (i_this->mAction >= ACTION_DAMAGE) {
        i_this->mCcCoreSph.SetR(YREG_F(15) + 55.0f);

        sp50 = a_this->eyePos;
        i_this->mCcCoreSph.SetC(sp50 + sp68);

        MTXCopy(model->getAnmMtx(YREG_S(2) + (int)JNT_CORE_3), *calc_mtx);
        sp44.set(YREG_F(3) + -250.0f, YREG_F(4), YREG_F(5));
        MtxPosition(&sp44, &sp50);

        i_this->mCcSph.SetC(sp50 + sp68);
        i_this->mCcSph.SetR(YREG_F(14) + 350.0f);
    } else {
        MTXCopy(model->getAnmMtx(YREG_S(1) + (int)JNT_CORE_3), *calc_mtx);
        sp44.set((XREG_F(3) + 300.0f) - 70.0f, XREG_F(4) + 50.0f, XREG_F(5));
        MtxPosition(&sp44, &a_this->eyePos);
        a_this->attention_info.position = a_this->eyePos;
        a_this->attention_info.position.y += XREG_F(6) + 250.0f;

        sp44.set(YREG_F(3), YREG_F(4), YREG_F(5));
        MtxPosition(&sp44, &sp50);
        i_this->mCcSph.SetC(sp50 + sp68);

        if (i_this->mDistToPlayer > 1000.0f && dComIfGp_checkPlayerStatus0(0, 0x80000)) {
            i_this->mCcSph.SetR(YREG_F(12) + 300.0f);
        } else {
            i_this->mCcSph.SetR(YREG_F(13) + 150.0f + 50.0f);
        }

        sp50.set(20000.0f, -20000.0f, 10000.0f);
        i_this->mCcCoreSph.SetC(sp50);
    }

    dComIfG_Ccsp()->Set(&i_this->mCcSph);
    dComIfG_Ccsp()->Set(&i_this->mCcCoreSph);

    sp50 = a_this->eyePos;
    if (daPy_getPlayerActorClass()->getCutType() != 0x27 && i_this->mAction >= ACTION_DAMAGE) {
        sp50.y = player->current.pos.y + 70.0f + KREG_F(7);
    } else {
        sp50.x -= 20000.0f;
    }

    i_this->mCcCoreCoSph.SetC(sp50 + sp68);
    dComIfG_Ccsp()->Set(&i_this->mCcCoreCoSph);

    MTXCopy(model->getAnmMtx(JNT_BODY2), *calc_mtx);
    sp44.set(0.0f, 0.0f, 0.0f);
    MtxPosition(&sp44, &sp50);

    i_this->mCcBodyCoSph.SetC(sp50 + sp68);
    i_this->mCcBodyCoSph.SetR(TREG_F(17) + 520.0f);
    dComIfG_Ccsp()->Set(&i_this->mCcBodyCoSph);

    demo_camera(i_this);

    cLib_addCalcAngleS2(&i_this->mBlureRate, i_this->mBlureRateTarget, 1, 8);

    f32 blend_target = 0.0f;
    f32 blend_step = 0.1f;

    if (i_this->mColpatType == 0) {
        dKy_custom_colset(0, 1, i_this->mColpatBlend);
    } else if (i_this->mColpatType == 1) {
        dKy_custom_colset(0, 1, i_this->mColpatBlend);
        blend_target = 1.0f;
        blend_step = 0.2f;
    } else if (i_this->mColpatType == 15) {
        dKy_custom_colset(0, 1, i_this->mColpatBlend);
        blend_target = 0.5f;
        blend_step = 0.0125f;
    } else if (i_this->mColpatType == 2) {
        dKy_custom_colset(2, 1, i_this->mColpatBlend);
        blend_step = 0.008f;
    } else if (i_this->mColpatType == 3) {
        dKy_custom_colset(2, 3, i_this->mColpatBlend);
        blend_target = 1.0f;
    } else if (i_this->mColpatType == 4) {
        dKy_custom_colset(4, 3, i_this->mColpatBlend);
        blend_step = 0.005f;
    } else if (i_this->mColpatType == 5) {
        dKy_custom_colset(5, 3, i_this->mColpatBlend);
        blend_step = 0.05f;
        i_this->mColpatType = 6;
    } else if (i_this->mColpatType == 6) {
        dKy_custom_colset(5, 3, i_this->mColpatBlend);
        blend_target = 1.0f;
        blend_step = 0.05f;
    } else if (i_this->mColpatType == 7) {
        dKy_custom_colset(4, 6, i_this->mColpatBlend);
        blend_target = 1.0f;
        blend_step = 0.0025f;
    }

    cLib_addCalc2(&i_this->mColpatBlend, blend_target, 1.0f, blend_step);

    i_this->field_0x684.framework(0, dComIfGp_getReverb(fopAcM_GetRoomNo(a_this)));
    i_this->field_0x6a4.framework(0, dComIfGp_getReverb(fopAcM_GetRoomNo(a_this)));

    if (i_this->field_0x1394 != 0) {
        i_this->field_0x1394--;

        if (i_this->field_0x1394 == 20) {
            fpcM_Search(s_bi_del_sub, i_this);
        }

        if (i_this->field_0x1394 == 10) {
            fpcM_Search(s_fw_del_sub, i_this);
        } else if (i_this->field_0x1394 == 0) {
            fopAcM_createChild(PROC_E_BI, fopAcM_GetID(a_this), 0xFFFFFF35, &a_this->current.pos,
                               fopAcM_GetRoomNo(a_this), NULL, NULL, -1, NULL);
        }
    }

    for (int i = 0; i < 4; i++) {
        static u16 takino[] = {0x8359, 0x835A, 0x835B, 0x835C};
        static u16 takino2[] = {0x842D, 0x842E, 0x842F, 0x8430};

        if (i_this->field_0x1151) {
            i_this->field_0x1228[i] = dComIfGp_particle_set(i_this->field_0x1228[i], takino2[i],
                                                            &a_this->home.pos, NULL, NULL);
        } else {
            i_this->field_0x1228[i] = dComIfGp_particle_set(i_this->field_0x1228[i], takino[i],
                                                            &a_this->home.pos, NULL, NULL);
        }
    }

    if (i_this->field_0x1151) {
        fopAcM_OffStatus(a_this, 0);
        a_this->attention_info.flags = 0;
    }

    return 1;
}

static int daB_BQ_IsDelete(b_bq_class* i_this) {
    return 1;
}

static int daB_BQ_Delete(b_bq_class* i_this) {
    dComIfG_resDelete(&i_this->mPhase, "B_bq");

    if (i_this->mInitHIO) {
        data_805BAD50 = 0;
    }

    if (i_this->heap != NULL) {
        i_this->mpMorf->stopZelAnime();
        i_this->field_0x684.deleteObject();
        i_this->field_0x6a4.deleteObject();
    }

    return 1;
}

static int useHeapInit(fopAc_ac_c* i_this) {
    b_bq_class* a_this = (b_bq_class*)i_this;

    a_this->mpMorf = new mDoExt_McaMorfSO((J3DModelData*)dComIfG_getObjectRes("B_bq", BMDV_BQ), NULL,
                                          NULL, NULL, J3DFrameCtrl::EMode_LOOP, 0.6f, 0, -1,
                                          &a_this->mSound, 0, 0x11000284);
    if (a_this->mpMorf == NULL || a_this->mpMorf->getModel() == NULL) {
        return false;
    }

    J3DModel* model = a_this->mpMorf->getModel();
    model->setUserArea((uintptr_t)a_this);

    for (u16 i = 0; i < model->getModelData()->getJointNum(); i++) {
        model->getModelData()->getJointNodePointer(i)->setCallBack(nodeCallBack);
    }

    a_this->mpDeadBrk = new mDoExt_brkAnm();
    if (a_this->mpDeadBrk == NULL) {
        return false;
    }

    if (!a_this->mpDeadBrk->init(a_this->mpMorf->getModel()->getModelData(),
                                 (J3DAnmTevRegKey*)dComIfG_getObjectRes("B_bq", BRK_BQ_DEAD), TRUE,
                                 J3DFrameCtrl::EMode_NONE, 1.0f, 0, -1))
    {
        return false;
    }

    a_this->mpDeadBrk->setPlaySpeed(0.0f);

    a_this->mpTodomeBtk = new mDoExt_btkAnm();
    if (a_this->mpTodomeBtk == NULL) {
        return false;
    }

    if (!a_this->mpTodomeBtk->init(a_this->mpMorf->getModel()->getModelData(),
                                   (J3DAnmTextureSRTKey*)dComIfG_getObjectRes("B_bq", BTK_BQ_TODOME), TRUE,
                                   J3DFrameCtrl::EMode_NONE, 1.0f, 0, -1))
    {
        return false;
    }

    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes("B_bq", BMDR_BQ_EYEBALL);
    JUT_ASSERT(0, modelData != NULL);

    a_this->mpEyeballModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
    if (a_this->mpEyeballModel == NULL) {
        return false;
    }

    a_this->field_0x684.init(&a_this->field_0x1138, 1);
    a_this->field_0x6a4.init(&a_this->field_0x1144, 1);
    return true;
}

static int daB_BQ_Create(fopAc_ac_c* i_this) {
    b_bq_class* a_this = (b_bq_class*)i_this;
    fopAcM_ct(a_this, b_bq_class);

    int phase_state = dComIfG_resLoad(&a_this->mPhase, "B_bq");
    if (phase_state == cPhs_COMPLEATE_e) {
        if (dComIfGs_isStageBossEnemy()) {
            // "After B_BQ defeated, so don't re-set\n"
            OS_REPORT("B_BQ やられ後なので再セットしません\n");

            cXyz pos(-234.0f, 0.0f, 2191.0f);
            csXyz angle(0, 0, 0);
            fopAcM_createWarpHole(&pos, &angle, fopAcM_GetRoomNo(i_this), 0, 0, 0xFF);

            if (!dComIfGs_isStageLife()) {
                cXyz scale(1.0f, 1.0f, 1.0f);
                pos.set(60.0f, 0.0f, 1778.0f);
                fopAcM_createItemForBoss(&pos, fpcNm_ITEM_UTAWA_HEART, fopAcM_GetRoomNo(i_this),
                                         &i_this->shape_angle, &scale, 0.0f, 0.0f, -1);
            }

            g_env_light.mColpatWeather = 6;
            g_env_light.wether_pat0 = 6;
            g_env_light.wether_pat1 = 6;
            fish_set(a_this);
            return cPhs_ERROR_e;
        }

        dComIfGs_offSwitch((fopAcM_GetParam(i_this) >> 0x10) & 0xFF, fopAcM_GetRoomNo(i_this));

        OS_REPORT("B_BQ PARAM %x\n", fopAcM_GetParam(i_this));
        OS_REPORT("B_BQ//////////////B_BQ SET 1 !!\n");

        if (!fopAcM_entrySolidHeap(i_this, useHeapInit, 0x6FA0)) {
            OS_REPORT("//////////////B_BQ SET NON !!\n");
            return cPhs_ERROR_e;
        }

        OS_REPORT("B_BQ//////////////B_BQ SET 2 !!\n");

        if (!data_805BAD50) {
            a_this->mInitHIO = 1;
            data_805BAD50 = true;
            l_HIO.field_0x4 = -1;
        }

        a_this->attention_info.flags = fopAc_AttnFlag_BATTLE_e;
        a_this->attention_info.distances[fopAc_attn_BATTLE_e] = 0x16;

        static dCcD_SrcSph cc_sph_src = {
            {
                {0x0, {{0x0, 0x0, 0x0}, {0xd8fbfdff, 0x3}, 0x75}},  // mObj
                {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0},                 // mGObjAt
                {dCcD_SE_HARD_BODY, 0x0, 0x0, 0x0, 0x2},            // mGObjTg
                {0x0},                                              // mGObjCo
            },                                                      // mObjInf
            {
                {{0.0f, 0.0f, 0.0f}, 40.0f}  // mSph
            }                                // mSphAttr
        };

        a_this->mCcStts.Init(100, 0, i_this);
        a_this->mCcSph.Set(cc_sph_src);
        a_this->mCcSph.SetStts(&a_this->mCcStts);
        a_this->mCcSph.OnTgNoHitMark();

        static dCcD_SrcSph core_sph_src = {
            {
                {0x0, {{0x0, 0x0, 0x0}, {0xd8fbfdff, 0x3}, 0x0}},  // mObj
                {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0},                // mGObjAt
                {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x2},                // mGObjTg
                {0x0},                                             // mGObjCo
            },                                                     // mObjInf
            {
                {{0.0f, 0.0f, 0.0f}, 40.0f}  // mSph
            }                                // mSphAttr
        };

        a_this->mCcCoreSph.Set(core_sph_src);
        a_this->mCcCoreSph.SetStts(&a_this->mCcStts);

        static dCcD_SrcSph core_co_sph_src = {
            {
                {0x0, {{0x0, 0x0, 0x0}, {0x0, 0x0}, 0x75}},  // mObj
                {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0},          // mGObjAt
                {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x2},          // mGObjTg
                {0x0},                                       // mGObjCo
            },                                               // mObjInf
            {
                {{0.0f, 0.0f, 0.0f}, 60.0f}  // mSph
            }                                // mSphAttr
        };

        a_this->mCcCoStts.Init(0xFF, 0, i_this);
        a_this->mCcCoreCoSph.Set(core_co_sph_src);
        a_this->mCcCoreCoSph.SetStts(&a_this->mCcCoStts);

        static dCcD_SrcSph body_co_sph_src = {
            {
                {0x0, {{0x0, 0x0, 0x0}, {0x0, 0x0}, 0x75}},  // mObj
                {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0},          // mGObjAt
                {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x2},          // mGObjTg
                {0x0},                                       // mGObjCo
            },                                               // mObjInf
            {
                {{0.0f, 0.0f, 0.0f}, 60.0f}  // mSph
            }                                // mSphAttr
        };

        a_this->mCcBodyCoSph.Set(body_co_sph_src);
        a_this->mCcBodyCoSph.SetStts(&a_this->mCcCoStts);

        a_this->mSound.init(&i_this->eyePos, &i_this->eyePos, 3, 1);
        a_this->mAtInfo.mpSound = &a_this->mSound;

        static dCcD_SrcSph mizu_sph_src = {
            {
                {0x0, {{AT_TYPE_CSTATUE_SWING, 0x2, 0xd}, {0x0, 0x0}, 0x0}},  // mObj
                {dCcD_SE_SOFT_BODY, 0x0, 0xd, 0x0, 0x0},                      // mGObjAt
                {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x2},                           // mGObjTg
                {0x0},                                                        // mGObjCo
            },                                                                // mObjInf
            {
                {{0.0f, 0.0f, 0.0f}, 35.0f}  // mSph
            }                                // mSphAttr
        };

        for (int i = 0; i < 4; i++) {
            a_this->mCcMizuSph[i].Set(mizu_sph_src);
            a_this->mCcMizuSph[i].SetStts(&a_this->mCcStts);
        }

        u32 params = fopAcM_GetParam(i_this) & 0xFFFFFF00;
        cXyz offset;
        cXyz tentacle_pos;

        cMtx_YrotS(*calc_mtx, i_this->shape_angle.y);
        offset.x = TREG_F(8) + 700.0f;
        offset.y = 0.0f;
        offset.z = TREG_F(9) + 100.0f;

        for (int i = 0; i < 2; i++) {
            MtxPosition(&offset, &tentacle_pos);
            tentacle_pos += i_this->current.pos;

            a_this->mTentacleIDs[i] =
                fopAcM_createChild(PROC_B_BH, fopAcM_GetID(i_this), params | i, &tentacle_pos,
                                   fopAcM_GetRoomNo(i_this), NULL, NULL, -1, NULL);
            offset.x *= -1.0f;
        }

        a_this->mMonkeyBombID =
            fopAcM_createChild(PROC_E_MB, fopAcM_GetID(i_this), 0xFFFFFFFF, &i_this->current.pos,
                               fopAcM_GetRoomNo(i_this), NULL, NULL, -1, NULL);

        if (dComIfG_play_c::getLayerNo(0) == 4) {
            a_this->mDemoMode = 1;
        }

        a_this->field_0x6ec = 2;

        OS_REPORT("//////////////B_BQ SKIP INFO %d\n", cDmr_SkipInfo);
        if (cDmr_SkipInfo == 0) {
            a_this->mDemoMode = 10;
            a_this->mAction = ACTION_STAY;
            a_this->mDisableDraw = true;
        } else {
            a_this->mAction = ACTION_STAY;
            Z2GetAudioMgr()->bgmStart(Z2BGM_BOSSBABA_1, 0, 0);
            a_this->field_0x1392 = 2;
            a_this->mDisableDraw = true;
            a_this->mColpatType = 1;
            a_this->mColpatBlend = 1.0f;
        }

        a_this->field_0x6fa = cM_rndF(2.99f) + 2.0f;
        a_this->mDamageBackCount = 1;

        daB_BQ_Execute(a_this);
    }

    return phase_state;
}

b_bq_class::b_bq_class() {}

static actor_method_class l_daB_BQ_Method = {
    (process_method_func)daB_BQ_Create,  (process_method_func)daB_BQ_Delete,
    (process_method_func)daB_BQ_Execute, (process_method_func)daB_BQ_IsDelete,
    (process_method_func)daB_BQ_Draw,
};

extern actor_process_profile_definition g_profile_B_BQ = {
    fpcLy_CURRENT_e,
    7,
    fpcPi_CURRENT_e,
    PROC_B_BQ,
    &g_fpcLf_Method.base,
    sizeof(b_bq_class),
    0,
    0,
    &g_fopAc_Method.base,
    212,
    &l_daB_BQ_Method,
    0x44000,
    fopAc_ENEMY_e,
    fopAc_CULLBOX_CUSTOM_e,
};
