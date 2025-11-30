/**
 * @file d_a_e_gob.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_e_gob.h"
#include "d/d_com_inf_game.h"
#include "d/d_com_inf_actor.h"
#include "d/actor/d_a_player.h"
#include "d/actor/d_a_obj_msima.h"
#include "d/actor/d_a_obj_myogan.h"
#include "d/d_s_play.h"
#include "f_op/f_op_camera_mng.h"
#include "m_Do/m_Do_graphic.h"
#include "c/c_damagereaction.h"
#include "Z2AudioLib/Z2Instances.h"

class daE_GOB_HIO_c : public JORReflexible {
public:
    daE_GOB_HIO_c();

    void genMessage(JORMContext*);
    virtual ~daE_GOB_HIO_c() {}

    /* 0x04 */ s8 id;
    /* 0x08 */ f32 size;
    /* 0x0C */ f32 normal_walk_speed;
    /* 0x10 */ f32 battle_walk_speed;
    /* 0x14 */ f32 roll_speed;
    /* 0x18 */ s16 rotation_speed;
    /* 0x1A */ s16 swing_time_a;
    /* 0x1C */ s16 swing_time_b;
};

enum E_gob_RES_file_ID {
    /* BCK */
    /* 0x04 */ BCK_MG_ATTACK_A_A = 0x4,
    /* 0x05 */ BCK_MG_ATTACK_A_B,
    /* 0x06 */ BCK_MG_ATTACK_A_C,
    /* 0x07 */ BCK_MG_ATTACK_B_A,
    /* 0x08 */ BCK_MG_ATTACK_B_B,
    /* 0x09 */ BCK_MG_ATTACK_B_C,
    /* 0x0A */ BCK_MG_BALL,
    /* 0x0B */ BCK_MG_CUR,
    /* 0x0C */ BCK_MG_DAMAGE_L,
    /* 0x0D */ BCK_MG_DAMAGE_R,
    /* 0x0E */ BCK_MG_DEFENSE_A,
    /* 0x0F */ BCK_MG_DEFENSE_B,
    /* 0x10 */ BCK_MG_DEFENSE_C,
    /* 0x11 */ BCK_MG_DE_GETUP,
    /* 0x12 */ BCK_MG_DE_GETUPWAIT,
    /* 0x13 */ BCK_MG_DE_JUMP_A,
    /* 0x14 */ BCK_MG_DE_JUMP_B,
    /* 0x15 */ BCK_MG_DE_LANDING,
    /* 0x16 */ BCK_MG_DE_SITWAIT,
    /* 0x17 */ BCK_MG_DE_STANDUP_ED,
    /* 0x18 */ BCK_MG_DE_STANDUP_ST,
    /* 0x19 */ BCK_MG_DE_TALK_ST,
    /* 0x1A */ BCK_MG_EXCITE,
    /* 0x1B */ BCK_MG_FIRE,
    /* 0x1C */ BCK_MG_HOLD,
    /* 0x1D */ BCK_MG_LANDING,
    /* 0x1E */ BCK_MG_N_STEP,
    /* 0x1F */ BCK_MG_N_TALK,
    /* 0x20 */ BCK_MG_N_WAIT,
    /* 0x21 */ BCK_MG_SHIELD,
    /* 0x22 */ BCK_MG_STAND,
    /* 0x23 */ BCK_MG_ST_WAIT,
    /* 0x24 */ BCK_MG_ST_WALK,
    /* 0x25 */ BCK_MG_WAIT,
    /* 0x26 */ BCK_MG_WALK,

    /* BMDR */
    /* 0x29 */ BMDR_MG = 0x29,
    /* 0x2A */ BMDR_MG_MET,
};

enum Joint {
    /* 0x00 */ JNT_WORLD_ROOT,
    /* 0x01 */ JNT_BACK_BONE1,
    /* 0x02 */ JNT_BACK_BONE2,
    /* 0x03 */ JNT_BACK_BONE3,
    /* 0x04 */ JNT_ARM_L1,
    /* 0x05 */ JNT_ARM_L2,
    /* 0x06 */ JNT_ARM_L3,
    /* 0x07 */ JNT_ARM_L4,
    /* 0x08 */ JNT_FINGER_L1,
    /* 0x09 */ JNT_FINGER_L2,
    /* 0x0A */ JNT_FINGER_L3,
    /* 0x0B */ JNT_THUMB_L1,
    /* 0x0C */ JNT_THUMB_L2,
    /* 0x0D */ JNT_ARM_R1,
    /* 0x0E */ JNT_ARM_R2,
    /* 0x0F */ JNT_ARM_R3,
    /* 0x10 */ JNT_ARM_R4,
    /* 0x11 */ JNT_FINGER_R1,
    /* 0x12 */ JNT_FINGER_R2,
    /* 0x13 */ JNT_FINGER_R3,
    /* 0x14 */ JNT_THUMB_R1,
    /* 0x15 */ JNT_THUMB_R2,
    /* 0x16 */ JNT_BACK_BONE4,
    /* 0x17 */ JNT_NECK,
    /* 0x18 */ JNT_HEAD,
    /* 0x19 */ JNT_JAW,
    /* 0x1A */ JNT_LIP,
    /* 0x1B */ JNT_BUST,
    /* 0x1C */ JNT_BODY_1,
    /* 0x1D */ JNT_WAIST,
    /* 0x1E */ JNT_LEG_L1,
    /* 0x1F */ JNT_LEG_L2,
    /* 0x20 */ JNT_LEG_L3,
    /* 0x21 */ JNT_LEG_L4,
    /* 0x22 */ JNT_LEG_R1,
    /* 0x23 */ JNT_LEG_R2,
    /* 0x24 */ JNT_LEG_R3,
    /* 0x25 */ JNT_LEG_R4,
};

enum daE_GOB_ACTION {
    ACTION_S_SET,
    ACTION_START,
    ACTION_FIGHT,
    ACTION_ATTACK,
    ACTION_DEFENCE,
    ACTION_BALL,
    ACTION_S_DAMAGE,
    ACTION_DAMAGE,
    ACTION_GLAB,
    ACTION_JUMP,
    ACTION_END,
    ACTION_MESSAGE,
};

daE_GOB_HIO_c::daE_GOB_HIO_c() {
    id = -1;
    size = 1.1f;
    normal_walk_speed = 8.0f;
    battle_walk_speed = 4.0f;
    roll_speed = 30.0f;
    rotation_speed = 7000;
    swing_time_a = 70;
    swing_time_b = 70;
}

static int nodeCallBack(J3DJoint* i_joint, int param_2) {
    if (param_2 == 0) {
        int jnt_no = i_joint->getJntNo();
        J3DModel* model = j3dSys.getModel();
        e_gob_class* enemy = (e_gob_class*)model->getUserArea();

        if (enemy != NULL) {
            if (jnt_no == JNT_BACK_BONE4 || jnt_no == JNT_NECK) {
                MTXCopy(model->getAnmMtx(jnt_no), *calc_mtx);
                cMtx_YrotM(*calc_mtx, enemy->mHeadRotY);
                cMtx_ZrotM(*calc_mtx, enemy->mHeadRotZ);
                model->setAnmMtx(jnt_no, *calc_mtx);
                MTXCopy(*calc_mtx, J3DSys::mCurrentMtx);
            } else if (jnt_no == JNT_BACK_BONE1 || jnt_no == JNT_BACK_BONE2) {
                MTXCopy(model->getAnmMtx(jnt_no), *calc_mtx);
                cMtx_YrotM(*calc_mtx, enemy->mBodyRotY);
                cMtx_ZrotM(*calc_mtx, enemy->mBodyRotZ);
                model->setAnmMtx(jnt_no, *calc_mtx);
                MTXCopy(*calc_mtx, J3DSys::mCurrentMtx);
            }

            if (jnt_no == JNT_BACK_BONE1 || jnt_no == JNT_BACK_BONE2 || jnt_no == JNT_BACK_BONE3 || jnt_no == JNT_ARM_L1
                || jnt_no == JNT_ARM_L2 || jnt_no == JNT_ARM_L3 || jnt_no == JNT_ARM_R1 || jnt_no == JNT_ARM_R2 || jnt_no == JNT_ARM_R3) {
                int sp10 = jnt_no & 3;
                MTXCopy(model->getAnmMtx(jnt_no), *calc_mtx);
                cMtx_YrotM(*calc_mtx, enemy->field_0x6c6[sp10]);
                cMtx_ZrotM(*calc_mtx, enemy->field_0x6ce[sp10]);
                model->setAnmMtx(jnt_no, *calc_mtx);
                MTXCopy(*calc_mtx, J3DSys::mCurrentMtx);
            }

            if (jnt_no == TREG_S(0)) {
                MTXCopy(model->getAnmMtx(jnt_no), *calc_mtx);
                cMtx_YrotM(*calc_mtx, TREG_S(1));
                cMtx_XrotM(*calc_mtx, TREG_S(2));
                cMtx_ZrotM(*calc_mtx, TREG_S(3));
                model->setAnmMtx(jnt_no, *calc_mtx);
                MTXCopy(*calc_mtx, J3DSys::mCurrentMtx);
            }

            if (jnt_no == TREG_S(4)) {
                MTXCopy(model->getAnmMtx(jnt_no), *calc_mtx);
                cMtx_YrotM(*calc_mtx, TREG_S(5));
                cMtx_XrotM(*calc_mtx, TREG_S(6));
                cMtx_ZrotM(*calc_mtx, TREG_S(7));
                model->setAnmMtx(jnt_no, *calc_mtx);
                MTXCopy(*calc_mtx, J3DSys::mCurrentMtx);
            }
        }
    }

    return 1;
}

static int daE_GOB_Draw(e_gob_class* i_this) {
    fopAc_ac_c* actor = (fopAc_ac_c*)i_this;
    J3DModel* model = i_this->mpModelMorf->getModel();

    g_env_light.settingTevStruct(0, &actor->current.pos, &actor->tevStr);
    g_env_light.setLightTevColorType_MAJI(model, &actor->tevStr);

    i_this->mpModelMorf->entryDL();

    if (!dComIfGs_isStageMiddleBoss()) {
        g_env_light.setLightTevColorType_MAJI(i_this->mpArmorModel, &actor->tevStr);
        mDoExt_modelUpdateDL(i_this->mpArmorModel);
    }

    cXyz sp8;
    sp8.set(actor->current.pos.x, 50.0f + actor->current.pos.y + BREG_F(18), actor->current.pos.z);
    i_this->field_0x650 = dComIfGd_setShadow(i_this->field_0x650, 1, model, &sp8, 1400.0f + BREG_F(19), 0.0f, actor->current.pos.y, i_this->mAcch.GetGroundH(), i_this->mAcch.m_gnd, &actor->tevStr, 0, 1.0f, dDlst_shadowControl_c::getSimpleTex());
    return 1;
}

static void anm_init(e_gob_class* i_this, int i_anm, f32 i_morf, u8 i_mode, f32 i_speed) {
    i_this->mpModelMorf->setAnm((J3DAnmTransform*)dComIfG_getObjectRes("E_gob", i_anm), i_mode, i_morf, i_speed, 0.0f, -1.0f);
    i_this->mAnm = i_anm;
}

static void damage_check(e_gob_class* i_this) {
    fopAc_ac_c* actor = (fopAc_ac_c*)i_this;
    dComIfGp_getPlayer(0);

    i_this->mCcStts.Move();

    if (i_this->mDamageInvulnerabilityTimer == 0 && i_this->mCcSph.ChkTgHit()) {
        i_this->mDamageInvulnerabilityTimer = 6;
        i_this->mAtInfo.mpCollider = i_this->mCcSph.GetTgHitObj();

        if (i_this->field_0xd20 != 0) {
            def_se_set(&i_this->mSound, i_this->mCcSph.GetTgHitObj(), 0x2A, NULL);
            return;
        }

        cc_at_check(actor, &i_this->mAtInfo);
        OS_REPORT("//////////////E_GOB AP %d\n", i_this->mAtInfo.mAttackPower);

        if (i_this->mAtInfo.mHitType == 0x10) {
            i_this->mAction = ACTION_S_DAMAGE;
            i_this->mMode = 0;

            i_this->mSound.startCreatureVoice(Z2SE_EN_GOB_V_STOMACH, -1);
            i_this->mSound.startCreatureSound(Z2SE_EN_GOB_STOMACH_BOUND, 0, -1);
            return;
        }

        i_this->field_0x6c4 = JREG_S(2) + 10;

        if (i_this->mAtInfo.mAttackPower >= 30 || daPy_getPlayerActorClass()->getCutCount() >= 4) {
            i_this->mAction = ACTION_DAMAGE;
            i_this->mMode = 0;
            i_this->mPlayerCutLRC = pl_cut_LRC(daPy_getPlayerActorClass()->getCutType());
            i_this->mSound.startCreatureVoice(Z2SE_EN_GOB_V_HIP_DOWN, -1);
        } else {
            i_this->mSound.startCreatureVoice(Z2SE_EN_GOB_V_DAMAGE, -1);
        }

        u16 hitmark;
        if (i_this->mAtInfo.mHitStatus != 0) {
            hitmark = 3;
        } else {
            hitmark = 1;
        }

        cXyz sp24;
        MTXCopy(i_this->mpModelMorf->getModel()->getAnmMtx(JNT_BODY_1), mDoMtx_stack_c::get());
        cXyz sp18(50.0f + VREG_F(17), VREG_F(18), VREG_F(19));
        mDoMtx_stack_c::multVec(&sp18, &sp24);
        dComIfGp_setHitMark(hitmark, actor, &sp24, &actor->shape_angle, NULL, 0);
    }
}

/* 806DF2C5 0003+00 data_806DF2C5 None */
static u8 l_initHIO;

static daE_GOB_HIO_c l_HIO;

static obj_msima_class* ms;

static obj_myogan_class* my;

static cXyz base_sc(1.1f, 1.1f, 1.1f);

static void fight(e_gob_class* i_this) {
    fopAc_ac_c* actor = (fopAc_ac_c*)i_this;
    int anm_frame = i_this->mpModelMorf->getFrame();
    f32 speed_target = 0.0f;
    s16 angle_step = 0x200;
    cXyz sp28;
    cXyz sp1C;

    s16 angle_target = i_this->mPlayerAngleDist;

    switch (i_this->mMode) {
    case -1:
        actor->home.pos.y = actor->current.pos.y;
        /* fallthrough */
    case 0:
        anm_init(i_this, BCK_MG_WALK, 10.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
        i_this->mMode = 1;
        i_this->mTimers[0] = 20.0f + cM_rndF(20.0f);
        actor->home.pos.y = actor->current.pos.y;
        break;

    case 1:
        speed_target = l_HIO.normal_walk_speed;

        if (i_this->mPlayerDist > 600.0f) {
            if (i_this->mTimers[2] == 0) {
                anm_init(i_this, BCK_MG_EXCITE, 10.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
                i_this->mSound.startCreatureVoice(Z2SE_EN_GOB_V_PROVOKE_A, -1);
                i_this->mMode = 5;
            }
        } else {
            i_this->mMode = 2;
        }
        break;

    case 2:
        i_this->mTimers[2] = 200.0f + cM_rndF(70.0f);

        if (i_this->mPlayerDist > 650.0f) {
            i_this->mMode = 0;
        } else if (i_this->mPlayerDist < 350.0f) {
            speed_target = -l_HIO.battle_walk_speed;
        } else if (i_this->mPlayerDist > 450.0f) {
            speed_target = l_HIO.battle_walk_speed;
        }

        if (0.0f != speed_target) {
            if (i_this->mAnm != BCK_MG_ST_WALK) {
                anm_init(i_this, BCK_MG_ST_WALK, 10.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
            }
        } else if (i_this->mAnm != BCK_MG_ST_WAIT && anm_frame == 0) {
            anm_init(i_this, BCK_MG_ST_WAIT, 10.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
        }

        if (speed_target <= 0.0f) {
            if (i_this->mTimers[0] == 0) {
                i_this->mAction = ACTION_ATTACK;
                i_this->mMode = 0;

                if (cM_rndF(1.0f) < 0.7f) {
                    i_this->field_0xd1f = 0;
                } else {
                    i_this->field_0xd1f = 1;
                }
                return;
            }
            break;
        }

        i_this->mTimers[0] = 20.0f + cM_rndF(20.0f);
        break;

    case 5:
        if (anm_frame == 102) {
            i_this->mSound.startCreatureVoice(Z2SE_EN_GOB_V_PROVOKE_B, -1);
        }

        if (anm_frame == 32 || anm_frame == 49) {
            dComIfGp_particle_set(0x8428, &actor->current.pos, &actor->shape_angle, &base_sc);
            i_this->mSound.startCreatureExtraSound(Z2SE_EN_GOB_PROVOKE_KNUCKLES, 0, -1);
        }

        if (i_this->mpModelMorf->isStop()) {
            i_this->mMode = 0;
            i_this->mTimers[2] = 200.0f + cM_rndF(70.0f);
        }
        break;

    case 10:
        speed_target = l_HIO.normal_walk_speed;
        angle_target = i_this->field_0x668;

        if (i_this->mTimers[0] == 0) {
            anm_init(i_this, BCK_MG_WAIT, 5.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
            i_this->mMode = 11;
            i_this->mTimers[0] = 60.0f + cM_rndF(30.0f);
        }
        break;

    case 11:
        angle_target = i_this->field_0x668;
        angle_step = 0;

        if (i_this->mTimers[0] == 0) {
            anm_init(i_this, BCK_MG_WALK, 5.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
            i_this->mMode = 10;
            i_this->mTimers[0] = 90.0f + cM_rndF(30.0f);

            sp28.x = (actor->home.pos.x + cM_rndFX(400.0f)) - actor->current.pos.x;
            sp28.z = (actor->home.pos.z + cM_rndFX(400.0f)) - actor->current.pos.z;
            i_this->field_0x668 = cM_atan2s(sp28.x, sp28.z);
        }
        break;
    }

    cLib_addCalc2(&actor->speedF, speed_target, 1.0f, 2.0f);
    cLib_addCalcAngleS2(&actor->current.angle.y, angle_target, 1, angle_step);
    cLib_addCalcAngleS2(&i_this->field_0x6b2, 0, 1, 0x800);

    if (dComIfGp_checkPlayerStatus0(0, 0x100)) {
        if (i_this->mMode < 10) {
            i_this->mMode = 11;
            i_this->mTimers[0] = 0;
        }
    } else if (i_this->mMode >= 10) {
        i_this->mMode = 0;
    }
}

static u8 attack(e_gob_class* i_this) {
    fopAc_ac_c* actor = (fopAc_ac_c*)i_this;
    s8 var_r26 = 0;
    s8 var_r28 = 0;
    int anm_frame = i_this->mpModelMorf->getFrame();
    u8 var_r25 = 1;

    switch (i_this->mMode) {
    case 0:
        if (i_this->field_0xd1f == 0) {
            anm_init(i_this, BCK_MG_ATTACK_A_A, 5.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
            i_this->mMode = 1;
            i_this->mSound.startCreatureVoice(Z2SE_EN_GOB_V_ATK_A_READY, -1);
        } else {
            anm_init(i_this, BCK_MG_ATTACK_B_A, 5.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
            i_this->mMode = 11;
            i_this->mSound.startCreatureVoice(Z2SE_EN_GOB_V_ATK_B_READY, -1);
        }
        break;

    case 1:
        var_r28 = 1;

        if (i_this->mpModelMorf->isStop()) {
            i_this->mMode = 2;
            anm_init(i_this, BCK_MG_ATTACK_A_B, 1.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
            i_this->mTimers[0] = l_HIO.swing_time_a;
        }
        break;

    case 2:
        var_r28 = 1;
        if (anm_frame == 1) {
            i_this->mSound.startCreatureExtraSound(Z2SE_EN_GOB_ARM_SWING_WAIT, 0, -1);
        }

        i_this->mSound.startCreatureVoiceLevel(Z2SE_EN_GOB_V_ATK_A_WAIT, -1);

        if (i_this->mTimers[0] == 0) {
            anm_init(i_this, BCK_MG_ATTACK_A_C, 3.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
            i_this->mSound.startCreatureExtraSound(Z2SE_EN_GOB_ARM_SWING_ATTACK, 0, -1);
            dComIfGp_particle_set(0x8422, &actor->current.pos, &actor->shape_angle, &base_sc);
            dComIfGp_particle_set(0x8423, &actor->current.pos, &actor->shape_angle, &base_sc);
            i_this->mSound.startCreatureVoice(Z2SE_EN_GOB_V_ATK_A, -1);
            i_this->mMode = 3;
        }
        break;

    case 3:
        var_r25 = 0;
        i_this->field_0x6a8 = 0x400;

        if (anm_frame < 10) {
            i_this->field_0xd1d = 1;
        }

        if (i_this->mpModelMorf->isStop() ) {
            var_r26 = 1;
        }
        break;

    case 11:
        var_r28 = 1;

        if (i_this->mpModelMorf->isStop()) {
            i_this->mMode = 12;
            anm_init(i_this, BCK_MG_ATTACK_B_B, 1.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
            i_this->mTimers[0] = l_HIO.swing_time_b;
        }
        break;

    case 12:
        var_r28 = 1;
        i_this->mSound.startCreatureVoiceLevel(Z2SE_EN_GOB_V_ATK_B_WAIT, -1);

        if (i_this->mTimers[0] == 0) {
            i_this->mSound.startCreatureExtraSound(Z2SE_EN_GOB_ARM_DOWN_WIND, 0, -1);
            anm_init(i_this, BCK_MG_ATTACK_B_C, 3.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
            dComIfGp_particle_set(0x8424, &actor->current.pos, &actor->shape_angle, &base_sc);
            dComIfGp_particle_set(0x8425, &actor->current.pos, &actor->shape_angle, &base_sc);
            i_this->mSound.startCreatureVoice(Z2SE_EN_GOB_V_ATK_B, -1);
            i_this->mMode = 13;
        }
        break;

    case 13:
        var_r25 = 0;
        i_this->field_0x6a8 = 0x400;

        if (anm_frame == 4) {
            i_this->mSound.startCreatureExtraSound(Z2SE_EN_GOB_ARM_DOWN_FLOOR, 0, -1);
        }

        if (anm_frame < 10) {
            i_this->field_0xd1d = 1;
        }

        if (anm_frame == 5) {
            dComIfGp_getVibration().StartShock(VIBMODE_S_POWER5, 0x1F, cXyz(0.0f, 1.0f, 0.0f));
            ms->field_0x59c = 700.0f;
            ms->field_0x598 = 2050;
            ms->field_0x59a = 1800;
        }

        if (i_this->mpModelMorf->isStop()) {
            var_r26 = 1;
        }
        break;
    }

    cLib_addCalc0(&actor->speedF, 1.0f, 2.0f);

    if (var_r26 != 0) {
        i_this->mAction = ACTION_FIGHT;
        i_this->mMode = 0;
    }

    if (var_r28 != 0) {
        cLib_addCalcAngleS2(&actor->current.angle.y, i_this->mPlayerAngleDist, 2, 0x800);
    }

    return var_r25;
}

static void defence(e_gob_class* i_this) {
    fopAc_ac_c* actor = (fopAc_ac_c*)i_this;
    s8 anm_finished = FALSE;
    s8 do_move_angle = FALSE;
    int anm_frame = i_this->mpModelMorf->getFrame();
    i_this->field_0xd20 = 1;

    switch (i_this->mMode) {
    case 0:
        if (i_this->mPlayerCutLRC == 0) {
            anm_init(i_this, BCK_MG_DEFENSE_A, 3.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
        } else if (i_this->mPlayerCutLRC == 1) {
            anm_init(i_this, BCK_MG_DEFENSE_B, 3.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
        } else {
            anm_init(i_this, BCK_MG_DEFENSE_C, 3.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
        }

        i_this->mSound.startCreatureVoice(Z2SE_EN_GOB_V_GUARD, -1);
        i_this->mMode = 1;
        break;

    case 1:
        if (i_this->mpModelMorf->isStop()) {
            anm_finished = TRUE;
        }
        break;
    }

    cLib_addCalc0(&actor->speedF, 1.0f, 2.0f);

    if (anm_finished) {
        i_this->mAction = ACTION_FIGHT;
        i_this->mMode = 0;
    }

    if (do_move_angle) {
        cLib_addCalcAngleS2(&actor->current.angle.y, i_this->mPlayerAngleDist, 2, 0x800);
    }
}

static u8 ball(e_gob_class* i_this) {
    fopAc_ac_c* actor = (fopAc_ac_c*)i_this;
    int anm_frame = i_this->mpModelMorf->getFrame();
    f32 speed_target = 0.0f;
    f32 speed_step = 1.0f;

    i_this->field_0x6b4 = 0;
    i_this->field_0xd20 = 1;
    u8 sp8 = 0;

    switch (i_this->mMode) {
    case 0:
        anm_init(i_this, BCK_MG_CUR, 5.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
        i_this->mMode = 1;
        i_this->field_0xd1c = 0;
        /* fallthrough */
    case 1:
        if (i_this->mpModelMorf->checkFrame(5.0f)) {
            i_this->mSound.startCreatureSound(Z2SE_EN_GOB_HIP_DOWN, 0, -1);
        }

        if (i_this->mpModelMorf->checkFrame(21.5f)) {
            i_this->mSound.startCreatureSound(Z2SE_EN_GOB_ROLLING, 0, -1);
        }

        i_this->field_0x668 = i_this->mPlayerAngleDist;

        speed_target = -(2.0f + TREG_F(11));
        speed_step = 2.0f + TREG_F(11);

        if (i_this->mpModelMorf->isStop()) {
            anm_init(i_this, BCK_MG_BALL, 5.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
            i_this->mMode = 2;
            i_this->mTimers[0] = 60;
            i_this->mTimers[2] = 300.0f + cM_rndF(100.0f);
        case 2:
            i_this->field_0x6b4 = l_HIO.rotation_speed;
            i_this->field_0x668 = i_this->mPlayerAngleDist;
            speed_step = 2.0f + TREG_F(4);

            if (i_this->mTimers[0] == 0) {
                i_this->mMode = 3;
                i_this->mTimers[1] = 30;
            }
        }
        break;

    case 3:
        i_this->field_0x6b4 = l_HIO.rotation_speed;
        speed_target = l_HIO.roll_speed;
        speed_step = 1.0f + TREG_F(5);

        if (i_this->mTimers[1] == 0 && i_this->field_0x6ab != 0) {
            i_this->mTimers[0] = 35.0f + cM_rndF(10.0f);
            i_this->mMode = 2;
        }

        if (i_this->mTimers[2] == 0 || dComIfGp_checkPlayerStatus0(0, 0x100)) {
            i_this->mMode = 4;
        }
        break;

    case 4:
        speed_step = 2.0f;
        cLib_addCalcAngleS2(&i_this->field_0x6b2, 0, 1, 0x800);
        cLib_addCalcAngleS2(&actor->current.angle.y, i_this->mPlayerAngleDist, 2, 0x700);

        if (i_this->field_0x6b2 == 0) {
            anm_init(i_this, BCK_MG_STAND, 2.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
            i_this->mMode = 5;
            dComIfGp_particle_set(0x8429, &actor->current.pos, &actor->shape_angle, &base_sc);
            dComIfGp_particle_set(0x842A, &actor->current.pos, &actor->shape_angle, &base_sc);
            i_this->mSound.startCreatureVoice(Z2SE_EN_GOB_V_RECOVER, -1);
        }
        break;

    case 5:
        if (anm_frame == 16) {
            i_this->mSound.startCreatureSound(Z2SE_EN_GOB_KNUCKLE_GROUND, 0, -1);
        }

        sp8 = 1;
        speed_step = 5.0f;
        cLib_addCalcAngleS2(&actor->current.angle.y, i_this->mPlayerAngleDist, 2, 0x800);

        if (i_this->mpModelMorf->isStop()) {
            i_this->mAction = ACTION_FIGHT;
            i_this->mMode = 0;
        }
        break;
    }

    if (i_this->field_0x6b2 != 0 || i_this->field_0x6b4 > 100) {
        i_this->field_0xd30 = dComIfGp_particle_set(i_this->field_0xd30, 0x8426, &actor->current.pos, &actor->shape_angle, &base_sc);
        i_this->field_0xd34 = dComIfGp_particle_set(i_this->field_0xd34, 0x8427, &actor->current.pos, &actor->shape_angle, &base_sc);
        dComIfGp_particle_levelEmitterOnEventMove(i_this->field_0xd30);
        dComIfGp_particle_levelEmitterOnEventMove(i_this->field_0xd34);
    }

    s16 spA = i_this->field_0x6b2;
    i_this->field_0x6b2 += i_this->field_0x6b4;

    if (spA < 0 && i_this->field_0x6b2 >= 0) {
        i_this->mSound.startCreatureSound(Z2SE_EN_GOB_ROLLING, 0, -1);
    }

    cLib_addCalcAngleS2(&actor->current.angle.y, i_this->field_0x668, 1, 0x300);
    cLib_addCalc2(&actor->speedF, speed_target, 1.0f, speed_step);

    if (actor->speedF > 10.0f && !daPy_getPlayerActorClass()->checkEquipHeavyBoots()) {
        i_this->field_0xd1d = 2;
    } else if (i_this->field_0x6b4 > 100) {
        fopEn_enemy_c* enemy = (fopEn_enemy_c*)i_this;
        cXyz sp2C;
        f32 temp_f27 = 100.0f + NREG_F(6);
        f32 var_f29 = (150.0f + (350.0f + NREG_F(7))) - 180.0f;
        f32 var_f28 = 70.0f + NREG_F(8);

        if (actor->speedF >= 10.0f) {
            var_f29 = 800.0f;
            var_f28 = 100.0f;
        }
    
        mDoMtx_stack_c::YrotS(-actor->shape_angle.y);
        mDoMtx_stack_c::transM(-actor->current.pos.x, -actor->current.pos.y, -actor->current.pos.z);
        mDoMtx_stack_c::multVec(&daPy_getPlayerActorClass()->current.pos, &sp2C);
    
        if (fabsf(sp2C.x) < var_f28 && sp2C.z > temp_f27 && sp2C.z < var_f29) {
            actor->attention_info.flags |= fopAc_AttnFlag_CARRY_e;
        }

        if (enemy->checkThrowMode(2)) {
            OS_REPORT("   GOB SET_MODE_CATCH \n");
            enemy->offThrowMode(2);
            i_this->mAction = ACTION_GLAB;
            i_this->mMode = 0;
            dComIfGoat_SetThrow(i_this);
            i_this->mSound.startCreatureVoice(Z2SE_EN_GOB_V_CATCHED, -1);
        } else if (!mDoCPd_c::getHoldA(PAD_1) && (i_this->field_0xd1c != 0 || actor->speedF >= l_HIO.roll_speed - 10.0f)) {
            i_this->field_0xd1d = 2;
            i_this->field_0xd1c = 1;
        }
    }

    return sp8;
}

static void s_damage(e_gob_class* i_this) {
    fopAc_ac_c* actor = (fopAc_ac_c*)i_this;
    f32 speed_target = 0.0f;

    switch (i_this->mMode) {
    case 0:
        anm_init(i_this, BCK_MG_SHIELD, 5.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
        i_this->mMode = 1;
        /* fallthrough */
    case 1:
        speed_target = 0.0f;
        if (i_this->mpModelMorf->isStop()) {
            i_this->mAction = ACTION_FIGHT;
            i_this->mMode = 0;
        }
        break;
    }

    cLib_addCalc2(&actor->speedF, speed_target, 1.0f, 2.0f);
}

static void damage(e_gob_class* i_this) {
    fopAc_ac_c* actor = (fopAc_ac_c*)i_this;
    f32 speed_target = 0.0f;

    i_this->mDamageInvulnerabilityTimer = 5;

    switch (i_this->mMode) {
    case 0:
    case 1:
        i_this->mAction = ACTION_BALL;
        i_this->mMode = 0;
        i_this->mDamageInvulnerabilityTimer = 30;
        /* dSv_event_flag_c::F_0672 - Goron Mines - First time seeing magnet goron tumbling */
        dComIfGs_onEventBit(dSv_event_flag_c::saveBitLabels[672]);
    }

    cLib_addCalc2(&actor->speedF, speed_target, 1.0f, 2.0f);
}

static u8 glab(e_gob_class* i_this) {
    fopAc_ac_c* actor = (fopAc_ac_c*)i_this;
    daPy_py_c* player = (daPy_py_c*)dComIfGp_getPlayer(0);
    fopEn_enemy_c* enemy = (fopEn_enemy_c*)i_this;
    dBgS_ObjGndChk gndchk;

    int anm_frame = i_this->mpModelMorf->getFrame();
    s8 spC = 2;
    s8 spB = 0;

    cXyz spC4;
    cXyz spB8;
    cXyz spAC;
    spC4.set(0.0f, 0.0f, 0.0f);

    MTXCopy(daPy_getPlayerActorClass()->getLeftItemMatrix(), *calc_mtx);
    MtxPosition(&spC4, &spB8);

    MTXCopy(daPy_getPlayerActorClass()->getRightItemMatrix(), *calc_mtx);
    MtxPosition(&spC4, &spAC);

    cXyz spA0 = spB8 + ((spAC - spB8) * 0.5f);

    spC4 = spB8 - spAC;
    s16 sp10 = (cM_atan2s(spC4.x, spC4.z) - 0x4000);
    s16 spE = -cM_atan2s(spC4.y, JMAFastSqrt((spC4.x * spC4.x) + (spC4.z * spC4.z)));

    i_this->mDamageInvulnerabilityTimer = 10;
    i_this->field_0xd20 = 1;
    i_this->field_0x6b2 += i_this->field_0x6b4;
    cLib_addCalcAngleS2(&i_this->field_0x6b4, 0, 1, 0x200);

    s8 spA = 0;
    s8 sp9 = 0;

    switch (i_this->mMode) {
    case 0:
        i_this->mMode = 1;
        spA = 1;
        actor->speedF = 0.0f;
        anm_init(i_this, BCK_MG_HOLD, 5.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
        break;

    case 1:
        if (enemy->checkThrowMode(8)) {
            enemy->offThrowMode(8);
            i_this->mMode = 2;
            i_this->mCounter = 0;
            i_this->field_0x6b6 = 0x800;
        }

        if (enemy->checkThrowMode(0x10)) {
            enemy->offThrowMode(0x10);
            i_this->mMode = 2;
            i_this->mCounter = 0;
            i_this->field_0x6b6 = -0x800;
        }

        spA = 1;
        actor->speed.y = 0.0f;
        break;

    case 2:
        if (anm_frame == 1) {
            i_this->mSound.startCreatureVoice(Z2SE_EN_GOB_V_CATCHED_ANNOY, -1);
        }

        if (anm_frame == 8) {
            i_this->mSound.startCreatureSound(Z2SE_EN_GOB_CATCHED_FRICTION, 0, -1);
        }

        if (i_this->mCounter == KREG_S(7) + 32) {
            i_this->mSound.startCreatureVoice(Z2SE_EN_GOB_V_THROWN, -1);
            i_this->mMode = 3;
            actor->speed.y = 50.0f;

            if (i_this->field_0x6b6 > 0) {
                cMtx_YrotS(*calc_mtx, (player->shape_angle.y + (KREG_S(5) + 32000)));
            } else {
                cMtx_YrotS(*calc_mtx, (player->shape_angle.y - (KREG_S(5) + 32000)));
            }

            spC4.x = 0.0f;
            spC4.y = 0.0f;
            spC4.z = 40.0f;
            MtxPosition(&spC4, &i_this->field_0x6b8);
        } else {
            spA = 1;
            i_this->speed.y = 0.0f;
        }
        break;

    case 3:
    case 4:
        if (i_this->field_0x6d8 >= 2) {
            dBgS_ObjGndChk_Spl sp124;
            sp124.SetPos(&actor->current.pos);
            
            dBgS_ObjGndChk spD0;
            spD0.SetPos(&actor->current.pos);
            
            f32 temp_f27 = dComIfG_Bgsp().GroundCross(&sp124);
            if (dComIfG_Bgsp().GroundCross(&spD0) < temp_f27 && i_this->mDemoCamMode == 0) {
                i_this->mDemoCamMode = 10;
            }
        }

        actor->current.pos.x += i_this->field_0x6b8.x;
        actor->current.pos.z += i_this->field_0x6b8.z;
        actor->current.angle.y += i_this->field_0x6b6;

        if (i_this->mAcch.ChkGroundHit()) {
            i_this->field_0x6b8.x *= 0.8f;
            i_this->field_0x6b8.z *= 0.8f;
            cLib_addCalcAngleS2(&i_this->field_0x6b6, 0, 1, 50);

            if (i_this->mMode == 3) {
                i_this->mMode = 4;
                actor->speed.y = 25.0f;

                dComIfGp_getVibration().StartShock(VIBMODE_S_POWER5, 0x1F, cXyz(0.0f, 1.0f, 0.0f));
                i_this->mSound.startCreatureSound(Z2SE_EN_GOB_ROLLING_BOUND, 0, -1);
            }

            if (i_this->field_0x6b6 == 0) {
                sp9 = 1;
            }
        }
        break;

    case 5: {
        actor->speed.y = 0.0f;
        cLib_addCalcAngleS2(&i_this->field_0x6b2, 0, 1, 0x800);

        spC4 = i_this->field_0x65c;
        spC4.y += 1000.0f;
        gndchk.SetPos(&spC4);

        f32 gnd_height = dComIfG_Bgsp().GroundCross(&gndchk);
        if (gnd_height > 0.0f) {
            i_this->field_0x65c.y = gnd_height;
        }

        spC4 = i_this->field_0x65c - actor->current.pos;
        cMtx_YrotS(*calc_mtx, cM_atan2s(spC4.x, spC4.z));
        cMtx_XrotM(*calc_mtx, -cM_atan2s(spC4.y, JMAFastSqrt((spC4.x * spC4.x) + (spC4.z * spC4.z))));

        spC4.x = 0.0f;
        spC4.y = 0.0f;
        spC4.z = 30.0f + NREG_F(0);
        MtxPosition(&spC4, &spB8);
        actor->current.pos += spB8;

        spC4 = i_this->field_0x65c - actor->current.pos;
        if (spC4.abs() < 50.0f) {
            actor->current.pos = i_this->field_0x65c;
            dComIfGp_getVibration().StartShock(VIBMODE_S_POWER8, 0x4F, cXyz(0.0f, 1.0f, 0.0f));

            ms->field_0x59c = 1000.0f;
            ms->field_0x598 = 2050;
            ms->field_0x59a = 1800;

            if (i_this->mDemoCamMode != 0) {
                i_this->mAction = ACTION_END;
                i_this->mMode = 0;
            } else {
                anm_init(i_this, BCK_MG_LANDING, 2.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
                i_this->mSound.startCreatureVoice(Z2SE_EN_GOB_V_RECOVER, -1);
                i_this->mMode = 6;

                if (i_this->field_0x6d8 == 1) {
                    Z2GetAudioMgr()->changeSubBgmStatus(2);
                } else {
                    Z2GetAudioMgr()->changeSubBgmStatus(3);
                }
            }
        } else {
            spC = 0;
        }

        spC4 = i_this->field_0x65c - i_this->field_0x690;
        spC4.y = 0.0f;
        f32 temp_f29 = spC4.abs();

        spC4 = i_this->field_0x65c - actor->current.pos;
        spC4.y = 0.0f;
        f32 temp_f28 = spC4.abs();

        f32 var_f31 = temp_f29 * (1.0f + BREG_F(19));
        if (var_f31 > 1000.0f + BREG_F(18)) {
            var_f31 = 1000.0f + BREG_F(18);
        }

        i_this->field_0x69c = var_f31 * cM_ssin(32768.0f * (temp_f28 / temp_f29));
        actor->current.angle.z = 0;
        cLib_addCalcAngleS2(&actor->current.angle.y, i_this->mPlayerAngleDist, 1, 0x200);
        break;
    }
    // fallthrough
    case 6:
        spB = 1;
        cLib_addCalcAngleS2(&actor->current.angle.y, i_this->mPlayerAngleDist, 1, 0x200);

        if (i_this->mpModelMorf->isStop()) {
            i_this->mAction = ACTION_FIGHT;
            i_this->mMode = 0;
        }
        break;
    }

    if (spA != 0) {
        cMtx_YrotS(*calc_mtx, sp10);
        spC4.x = BREG_F(6);
        spC4.y = -50.0f + BREG_F(7);
        spC4.z = 170.0f + BREG_F(8);
        MtxPosition(&spC4, &spB8);
        spB8 += spA0;

        cLib_addCalc2(&actor->current.pos.x, spB8.x, 1.0f, 30.0f);
        cLib_addCalc2(&actor->current.pos.y, spB8.y, 1.0f, 30.0f);
        cLib_addCalc2(&actor->current.pos.z, spB8.z, 1.0f, 30.0f);

        cLib_addCalcAngleS2(&actor->current.angle.y, (sp10 + 0x8000), 1, 0x800);
        cLib_addCalcAngleS2(&actor->current.angle.z, spE / 2, 4, 0x1000);
        cLib_addCalcAngleS2(&i_this->field_0x6b2, (AREG_S(2) - 5000), 1, 0x800);
        spC = 0;
    }

    if (enemy->checkThrowMode(4)) {
        enemy->offThrowMode(4);
        sp9 = 1;
    }

    if (sp9 != 0) {
        i_this->mAction = ACTION_BALL;
        i_this->mMode = 4;
        actor->current.angle.z = 0;
        spC = 2;
        actor->speed.y = 0.0f;
        i_this->mTimers[4] = 20;
    }

    u8 sp8 = spC | spB;
    return sp8;
}

static void jump(e_gob_class* i_this) {
    fopAc_ac_c* actor = (fopAc_ac_c*)i_this;
    cXyz sp58;
    cXyz sp4C;
    s8 sp8 = 0;

    switch (i_this->mMode) {
    case 0:
        actor->current.angle.z = 0;
        actor->speedF = 0.0f;
        sp8 = 1;

        i_this->mSound.startCreatureVoice(Z2SE_EN_GOB_V_MAGMA, -1);
        anm_init(i_this, BCK_MG_FIRE, 3.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
        i_this->mMode = 1;
        actor->speed.y = 110.0f + KREG_F(2);
        i_this->field_0x6b6 = cM_rndFX(2000.0f);

        if (i_this->mDemoCamMode != 0) {
            i_this->field_0x6b8.set(0.0f, 0.0f, 0.0f);
            Z2GetAudioMgr()->subBgmStop();
            Z2GetAudioMgr()->muteSceneBgm(0, 0.0f);
        } else {
            sp4C = actor->current.pos - actor->home.pos;
            s16 spA = cM_atan2s(sp4C.x, sp4C.z);
            if (cM_rndF(1.0f) < 0.5f) {
                cMtx_YrotS(*calc_mtx, (spA + 0x4000));
            } else {
                cMtx_YrotS(*calc_mtx, (spA - 0x4000));
            }

            sp4C.x = 0.0f;
            sp4C.y = 0.0f;
            sp4C.z = 10.0f + cM_rndF(10.0f);
            MtxPosition(&sp4C, &i_this->field_0x6b8);

            if (i_this->field_0x6d8 == 1) {
                Z2GetAudioMgr()->changeSubBgmStatus(1);
            } else {
                Z2GetAudioMgr()->changeSubBgmStatus(4);
            }
        }
        break;

    case 1:
        actor->current.pos.x += i_this->field_0x6b8.x;
        actor->current.pos.z += i_this->field_0x6b8.z;
        actor->current.pos.y += actor->speed.y;

        actor->speed.y += actor->gravity;
        actor->current.angle.y += i_this->field_0x6b6;

        if (actor->current.pos.y < KREG_F(3)) {
            actor->current.pos.y = KREG_F(3);
            i_this->field_0x6d6++;

            if (i_this->field_0x6d6 >= 2) {
                i_this->mAction = ACTION_GLAB;
                i_this->mMode = 5;

                if (i_this->mDemoCamMode == 0) {
                    i_this->field_0x65c = actor->home.pos + ((actor->current.pos - actor->home.pos) * 0.5f);

                    i_this->mSound.startCreatureVoice(Z2SE_EN_GOB_V_MAGMA, -1);
                } else {
                    i_this->field_0x65c.set(0.0f, actor->home.pos.y, 500.0f + VREG_F(11));
                    i_this->mSound.startCreatureVoice(Z2SE_EN_GOB_V_MAGMA_LAST, -1);
                }

                i_this->field_0x690 = actor->current.pos;
                i_this->field_0x6d9 = 10;
                sp8 = 1;
            } else {
                i_this->mMode = 0;
            }
        }
    }

    if (sp8 != 0) {
        i_this->mSound.startCreatureSound(Z2SE_EN_GOB_MAGMA_JUMP, 0, -1);

        sp58 = actor->current.pos;
        sp58.y = 0.0f;

        dComIfGp_particle_set(0x845B, &sp58, &actor->shape_angle, &base_sc);
        dComIfGp_particle_set(0x845C, &sp58, &actor->shape_angle, &base_sc);
        dComIfGp_particle_set(0x845D, &sp58, &actor->shape_angle, &base_sc);
    }
}

static void start(e_gob_class* i_this) {
    fopAc_ac_c* actor = (fopAc_ac_c*)i_this;
    dComIfGp_getPlayer(0);
    i_this->mDamageInvulnerabilityTimer = 5;

    switch (i_this->mMode) {
    case 0:
        anm_init(i_this, BCK_MG_DE_SITWAIT, 0.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
        i_this->mMode = 1;
        break;

    case 1:
        if (!dComIfGp_checkPlayerStatus0(0, 0x100) && ms != NULL && ms->field_0x5c4[0] != 0) {
            i_this->mDemoCamMode = 1;
            i_this->mMode = 2;
            i_this->mTimers[0] = 120;
        }
        break;

    case 2:
        if (i_this->mTimers[0] == 1) {
            anm_init(i_this, BCK_MG_DE_STANDUP_ST, 10.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
        }

        if (i_this->mAnm == BCK_MG_DE_STANDUP_ST && i_this->mpModelMorf->isStop()) {
            anm_init(i_this, BCK_MG_DE_TALK_ST, 10.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
        }
        break;

    case 3:
        if (i_this->mAnm == BCK_MG_DE_JUMP_A && i_this->mpModelMorf->isStop()) {
            actor->speed.y = 150.0f;
            i_this->mMode = 4;
            anm_init(i_this, BCK_MG_DE_JUMP_B, 1.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
            i_this->mSound.startCreatureVoice(Z2SE_EN_GOB_V_JUMP, -1);
        }
        break;

    case 4:
        if (actor->speed.y <= 0.0f) {
            actor->current.pos.z = 0.0f;

            if (i_this->mAcch.ChkGroundHit()) {
                i_this->mMode = 5;

                dComIfGp_getVibration().StartShock(VIBMODE_S_POWER8, 0x1F, cXyz(0.0f, 1.0f, 0.0f));
                anm_init(i_this, BCK_MG_LANDING, 0.0f, J3DFrameCtrl::EMode_NONE, 1.0f);

                i_this->field_0xd8c = 15.0f + BREG_F(7);
                i_this->mSound.startCreatureVoice(Z2SE_EN_GOB_V_RECOVER, -1);
            }
        }
        break;

    case 5:
        if (i_this->mpModelMorf->isStop()) {
            anm_init(i_this, BCK_MG_ST_WAIT, 5.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
            i_this->mMode = 6;
        }
        break;

    case 6:
        break;
    }
}

static void end(e_gob_class* i_this) {
    fopAc_ac_c* actor = (fopAc_ac_c*)i_this;
    dComIfGp_getPlayer(0);

    i_this->mDamageInvulnerabilityTimer = 5;
    cLib_addCalcAngleS2(&actor->current.angle.y, i_this->mPlayerAngleDist, 1, 0x1000);

    switch (i_this->mMode) {
    case 0:
        anm_init(i_this, BCK_MG_DE_LANDING, 0.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
        i_this->mSound.startCreatureVoice(Z2SE_EN_GOB_V_FALL_LAST, -1);
        i_this->mSound.startCreatureSound(Z2SE_EN_GOB_HIP_DOWN, 0, -1);
        i_this->mMode = 1;
        break;

    case 1:
        break;

    case 2:
        anm_init(i_this, BCK_MG_DE_GETUP, 5.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
        i_this->mSound.startCreatureVoice(Z2SE_EN_GOB_V_GETUP_LAST, -1);
        i_this->mMode = 3;
        break;

    case 3:
        if (i_this->mpModelMorf->isStop()) {
            anm_init(i_this, BCK_MG_DE_GETUPWAIT, 5.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
        }
        break;

    case 4:
        anm_init(i_this, BCK_MG_DE_STANDUP_ED, 5.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
        i_this->mMode = 5;
        break;

    case 5:
        if (i_this->mpModelMorf->isStop()) {
            anm_init(i_this, BCK_MG_WAIT, 5.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
        }
    }
}

static void s_set(e_gob_class* i_this) {
    if ((dComIfGp_getPlayer(0)->current.pos.y - i_this->current.pos.y) < 300.0f) {
        i_this->mAction = ACTION_FIGHT;
        i_this->mMode = -1;
    }
}

static void message(e_gob_class* i_this) {
    fopAc_ac_c* actor = (fopAc_ac_c*)i_this;

    if (i_this->field_0xd90 != 0) {
        if (i_this->mMsgFlow.doFlow(i_this, NULL, 0)) {
            dComIfGp_event_reset();
            i_this->field_0xd90 = 0;
        }

        if (i_this->mAnm != BCK_MG_N_TALK) {
            anm_init(i_this, BCK_MG_N_TALK, 5.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
        }
        return;
    }

    if (i_this->mPlayerDist < 600.0f) {
        cLib_onBit<u32>(actor->attention_info.flags, fopAc_AttnFlag_SPEAK_e | fopAc_AttnFlag_TALK_e);
        i_this->eventInfo.onCondition(1);

        if (dComIfGp_event_runCheck()) {
            if (actor->eventInfo.checkCommandTalk() && i_this->field_0xd90 == 0) {
                i_this->mMsgFlow.init(i_this, 203, 0, NULL);
                i_this->field_0xd90 = 1;
            }
        } else {
            i_this->field_0xd90 = 0;
        }

        s16 temp_r0 = i_this->current.angle.y - i_this->mPlayerAngleDist;
        if (temp_r0 > 0x800 || temp_r0 < -0x800) {
            if (i_this->mAnm != BCK_MG_N_STEP) {
                anm_init(i_this, BCK_MG_N_STEP, 5.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
                i_this->mTimers[1] = 15;
            }
            cLib_addCalcAngleS2(&i_this->current.angle.y, i_this->mPlayerAngleDist, 2, 0x200);
        } else if (i_this->mTimers[1] == 0 && i_this->mAnm != BCK_MG_N_WAIT) {
            anm_init(i_this, BCK_MG_N_WAIT, 10.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
        }
    } else if (i_this->mAnm != BCK_MG_N_WAIT) {
        anm_init(i_this, BCK_MG_N_WAIT, 10.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
    }

    if (i_this->mTimers[3] != 0 && dComIfGp_event_runCheck()) {
        i_this->current.pos.y = 2120.0f;
        i_this->speed.y = 0.0f;
        i_this->old.pos = i_this->current.pos;
    }
}

static void action(e_gob_class* i_this) {
    fopAc_ac_c* actor = (fopAc_ac_c*)i_this;
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    cXyz sp70;
    cXyz sp64;

    if (dComIfGp_checkPlayerStatus0(0, 0x100)) {
        sp70.x = actor->home.pos.x - actor->current.pos.x;
        sp70.z = actor->home.pos.z - actor->current.pos.z;
        i_this->field_0x668 = cM_atan2s(sp70.x, sp70.z);
    } else {
        i_this->mPlayerAngleDist = fopAcM_searchPlayerAngleY(actor);
    }

    i_this->mPlayerDist = fopAcM_searchPlayerDistance(actor);

    u8 on_co_bit = TRUE;
    u8 sp10 = 1;
    u8 spF = 0;
    u8 on_attention = TRUE;
    u8 spD = 0;
    u8 spC = 1;
    u8 spB = 1;
    u8 spA = 1;
    u8 sp9 = 1;
    u8 stop_subbgm = TRUE;
    int sp2C = 4;

    cLib_offBit<u32>(actor->attention_info.flags, fopAc_AttnFlag_CARRY_e | fopAc_AttnFlag_SPEAK_e | fopAc_AttnFlag_TALK_e);
    int sp28 = 1;

    switch (i_this->mAction) {
    case 0:
        s_set(i_this);
        on_attention = FALSE;
        sp10 = 0;
        stop_subbgm = FALSE;
        sp28 = 0;
        break;

    case 1:
        start(i_this);
        on_attention = FALSE;
        sp9 = 0;
        stop_subbgm = FALSE;
        sp28 = 0;
        break;

    case 2:
        fight(i_this);
        i_this->field_0x6ac = 1;
        spD = 1;
        break;

    case 3:
        spB = attack(i_this);
        i_this->field_0x6ac = 1;
        break;

    case 4:
        defence(i_this);
        i_this->field_0x6ac = 1;
        spD = 1;
        break;

    case 5:
        spC = ball(i_this);
        spF = 1;
        sp2C = 1;
        break;

    case 6:
        s_damage(i_this);
        break;

    case 7:
        damage(i_this);
        break;

    case 8:
        sp10 = glab(i_this);
        on_co_bit = FALSE;
        spC = (sp10 & 1);
        sp10 &= 2;
        spF = 1;
        sp9 = 0;
        if (i_this->mMode >= 5) {
            spA = 0;
        }
        break;

    case 9:
        jump(i_this);
        on_attention = FALSE;
        on_co_bit = FALSE;
        spD = 0;
        spC = 0;
        sp10 = 0;
        spA = 0;
        sp9 = 0;
        break;

    case 10:
        end(i_this);
        on_attention = FALSE;
        spA = 0;
        sp9 = 0;
        break;

    case 11:
        spA = 0;
        sp9 = 0;
        i_this->mCcSph.OffTgSetBit();
        i_this->field_0x6ac = 1;
        message(i_this);
        break;
    }

    if (stop_subbgm && player->current.pos.y < 100.0f) {
        Z2GetAudioMgr()->subBgmStop();
    }

    if (on_co_bit) {
        i_this->mCcSph.OnCoSetBit();
    } else {
        i_this->mCcSph.OffCoSetBit();
    }

    if (on_attention) {
        fopAcM_OnStatus(i_this, 0);
        actor->attention_info.flags |= fopAc_AttnFlag_BATTLE_e;
    } else {
        fopAcM_OffStatus(i_this, 0);
        actor->attention_info.flags &= ~fopAc_AttnFlag_BATTLE_e;
    }

    if (spD && i_this->mPlayerDist < 400.0f && daPy_getPlayerActorClass()->getCutType() != 0x29 && daPy_getPlayerActorClass()->getCutAtFlg() != 0 && i_this->field_0x6aa == 0) {
        i_this->mAction = ACTION_DEFENCE;
        i_this->mMode = 0;
        i_this->mPlayerCutLRC = pl_cut_LRC(daPy_getPlayerActorClass()->getCutType());
        i_this->field_0xd20 = 1;
        i_this->field_0x6aa = 20;
    }

    if (on_co_bit) {
        i_this->mCcSph.OnCoSetBit();
    } else {
        i_this->mCcSph.OffCoSetBit();
    }

    cLib_addCalcAngleS2(&actor->shape_angle.y, actor->current.angle.y, 2, 0x1000);
    cLib_addCalcAngleS2(&actor->shape_angle.z, actor->current.angle.z, 2, 0x100);

    if (sp10) {
        if (i_this->mTimers[4] != 0) {
            actor->speed.y = 0.0f;
            actor->current.pos.y += 2.0f;
            actor->old.pos.y += 2.0f;
        }

        cMtx_YrotS(*calc_mtx, actor->current.angle.y);
        sp70.x = 0.0f;
        sp70.y = 0.0f;
        sp70.z = actor->speedF * l_HIO.size;
        MtxPosition(&sp70, &sp64);
        actor->speed.x = sp64.x;
        actor->speed.z = sp64.z;

        actor->current.pos += actor->speed;
        actor->speed.y += actor->gravity;
        actor->current.pos.y -= i_this->field_0x688;
        actor->old.pos.y -= i_this->field_0x688;
        i_this->mAcch.CrrPos(dComIfG_Bgsp());
        actor->current.pos.y += i_this->field_0x688;
        actor->old.pos.y += i_this->field_0x688;

        if (i_this->mTimers[4] == 0 && i_this->mAcch.ChkGroundHit()) {
            actor->speed.y = -30.0f;
        }
    }

    if (sp9) {
        sp70 = actor->current.pos - actor->home.pos;
        sp70.y = 0.0f;
        if (sp70.abs() > 1350.0f + KREG_F(11)) {
            cMtx_YrotS(*calc_mtx, cM_atan2s(sp70.x, sp70.z));
            sp70.x = 0.0f;
            sp70.y = 0.0f;
            sp70.z = 1350.0f + KREG_F(11);
            MtxPosition(&sp70, &sp64);
            actor->current.pos.x = actor->home.pos.x + sp64.x;
            actor->current.pos.z = actor->home.pos.z + sp64.z;
        }
    }

    if (spF) {
        cLib_addCalc2(&i_this->field_0x680, 120.0f + AREG_F(0), 1.0f, 12.0f);
        cLib_addCalc2(&i_this->field_0x684, 50.0f + AREG_F(1), 1.0f, 5.0f);
        cLib_addCalc2(&i_this->field_0x688, 25.0f + AREG_F(2), 1.0f, 1.0f);
    } else {
        cLib_addCalc0(&i_this->field_0x680, 1.0f, 12.0f);
        cLib_addCalc0(&i_this->field_0x684, 1.0f, 5.0f);
        cLib_addCalc0(&i_this->field_0x688, 1.0f, 1.0f);
    }

    if (i_this->field_0x6d9 != 0) {
        i_this->field_0x6d9--;
    }

    if (spA && i_this->field_0x6d9 == 0 && actor->current.pos.y + i_this->field_0x69c < KREG_F(3)) {
        actor->current.pos.y = KREG_F(3) - i_this->field_0x69c;
        i_this->field_0x6d9 = 10;
        i_this->mAction = ACTION_JUMP;
        i_this->mMode = 0;
        i_this->field_0x6d6 = 0;
        /* dSv_event_flag_c::F_0671 - Goron Mines - Through magnet goron into lava once */
        dComIfGs_onEventBit(dSv_event_flag_c::saveBitLabels[671]);

        i_this->field_0x6d8++;
        OS_REPORT("//////////////E_GOB DOWN CT %d\n", i_this->field_0x6d8);
    }

    dBgS_ObjGndChk sp124;
    dBgS_ObjGndChk_Spl spD0;
    i_this->field_0x6ab = 0;

    f32 var_f31 = 1.0f;
    if (actor->speedF >= 10.0f) {
        var_f31 = 2.0f + TREG_F(7);
    }

    for (int i = 0; i < sp2C; i++) {
        static f32 bg_x[] = {0.0f, 300.0f, -300.0f, 0.0f};
        static f32 bg_z[] = {300.0f, 0.0f, 0.0f, -300.0f};

        cMtx_YrotS(*calc_mtx, actor->shape_angle.y);
        sp70.x = var_f31 * bg_x[i];
        sp70.y = 300.0f * var_f31;
        sp70.z = var_f31 * bg_z[i];
        MtxScale(l_HIO.size, l_HIO.size, l_HIO.size, 1);
        MtxPosition(&sp70, &sp64);
        sp64 += actor->current.pos;

        sp124.SetPos(&sp64);
        spD0.SetPos(&sp64);

        f32 temp_f27 = dComIfG_Bgsp().GroundCross(&spD0);
        f32 temp_f26 = dComIfG_Bgsp().GroundCross(&sp124);
        if (temp_f27 >= temp_f26) {
            i_this->field_0x6ab = 1;
            break;
        }
    }

    s16 sp1E = 0;
    s16 sp1C = 0;
    if (i_this->field_0x6ac != 0) {
        sp70 = player->eyePos - actor->current.pos;
        sp70.y += -170.0f + TREG_F(2);

        sp1E = -(cM_atan2s(sp70.x, sp70.z) - actor->shape_angle.y);
        sp1C = -(i_this->shape_angle.x + cM_atan2s(sp70.y, JMAFastSqrt((sp70.x * sp70.x) + (sp70.z * sp70.z))));
        if (sp1E > 6000) {
            sp1E = 6000;
        } else if (sp1E < -6000) {
            sp1E = -6000;
        }

        if (sp1C > 6000) {
            sp1C = 6000;
        } else if (sp1C < -6000) {
            sp1C = -6000;
        }
    }

    cLib_addCalcAngleS2(&i_this->mHeadRotY, sp1E / 2, 2, 0x800);
    cLib_addCalcAngleS2(&i_this->mHeadRotZ, sp1C / 2, 2, 0x800);

    s16 sp1A = 0;
    s16 sp18 = 0;
    s16 sp16 = 0;
    s16 sp14 = 0;

    if (spC && i_this->mAcch.ChkGroundHit()) {
        Vec sp58;
        cXyz sp4C;
        dBgS_GndChk sp7C;
        
        f32 temp_f28 = 50.0f;
        sp4C = actor->current.pos;
        sp4C.y += 50.0f;
        sp7C.SetPos(&sp4C);
        
        sp4C.y = dComIfG_Bgsp().GroundCross(&sp7C);
        if (-G_CM3D_F_INF != sp4C.y) {
            sp58.x = sp4C.x;
            sp58.y = 50.0f + sp4C.y;
            sp58.z = sp4C.z + temp_f28;
            sp7C.SetPos(&sp58);

            sp58.y = dComIfG_Bgsp().GroundCross(&sp7C);
            if (-G_CM3D_F_INF != sp58.y) {
                f32 var_f30 = sp58.y - sp4C.y;
                f32 var_f24 = sp58.z - sp4C.z;
                sp1A = -cM_atan2s(var_f30, var_f24);
                if (sp1A > 0x2000 || sp1A < -0x2000) {
                    sp1A = 0;
                }
            }

            sp58.x = sp4C.x + temp_f28;
            sp58.y = 50.0f + sp4C.y;
            sp58.z = sp4C.z;
            sp7C.SetPos(&sp58);

            sp58.y = dComIfG_Bgsp().GroundCross(&sp7C);
            if (-G_CM3D_F_INF != sp58.y) {
                f32 var_f30 = sp58.y - sp4C.y;
                f32 var_f25 = sp58.x - sp4C.x;
                sp18 = (s16)cM_atan2s(var_f30, var_f25);
                if (sp18 > 0x2000 || sp18 < -0x2000) {
                    sp18 = 0;
                }
            }
        }

        if (spB) {
            cMtx_XrotS(*calc_mtx, i_this->field_0x6a0);
            cMtx_ZrotM(*calc_mtx, i_this->field_0x6a2);
            sp70.set(0.0f, 16384.0f, 0.0f);
            MtxPosition(&sp70, &sp64);
            cMtx_YrotS(*calc_mtx, -actor->shape_angle.y);
            MtxPosition(&sp64, &sp70);

            sp14 = sp70.x * (0.3f+ KREG_F(7));
            sp16 = -sp70.z * (0.3f+ KREG_F(8));
        }
    }

    cLib_addCalcAngleS2(&i_this->field_0x6a0, sp1A, 2, 0x100);
    cLib_addCalcAngleS2(&i_this->field_0x6a2, sp18, 2, 0x100);
    cLib_addCalcAngleS2(&i_this->mBodyRotZ, sp16, 4, i_this->field_0x6a8);
    cLib_addCalcAngleS2(&i_this->mBodyRotY, sp14, 4, i_this->field_0x6a8);
    i_this->field_0x6a8 = 20;

    if (i_this->field_0x6c4 != 0) {
        i_this->field_0x6c4--;
        f32 var_f29 = (f32)i_this->field_0x6c4 * (0.01f + JREG_F(7));
        if (var_f29 > 0.2f + JREG_F(8)) {
            var_f29 = 0.2f + JREG_F(8);
        }
 
        s16 sp12 = var_f29 * (4000.0f + JREG_F(9));

        for (int i = 0; i < 4; i++) {
            i_this->field_0x6ce[i] = (f32)sp12 *  cM_ssin((i_this->field_0x6c4 * (JREG_S(4) + 0x3A00)) + (i * (JREG_S(5) - 10000)));
            i_this->field_0x6c6[i] = (f32)sp12 *  cM_ssin((i_this->field_0x6c4 * (JREG_S(6) + 0x3200)) + (i * (JREG_S(7) - 10000)));
        }
    }
}

static void cam_3d_morf(e_gob_class* i_this, f32 param_1) {
    cLib_addCalc2(&i_this->mDemoCamCenter.x, i_this->mDemoCamCenterTarget.x, param_1, i_this->mDemoCamCenterSpd.x * i_this->field_0xd88);
    cLib_addCalc2(&i_this->mDemoCamCenter.y, i_this->mDemoCamCenterTarget.y, param_1, i_this->mDemoCamCenterSpd.y * i_this->field_0xd88);
    cLib_addCalc2(&i_this->mDemoCamCenter.z, i_this->mDemoCamCenterTarget.z, param_1, i_this->mDemoCamCenterSpd.z * i_this->field_0xd88);
    cLib_addCalc2(&i_this->mDemoCamEye.x, i_this->mDemoCamEyeTarget.x, param_1, i_this->mDemoCamEyeSpd.x * i_this->field_0xd88);
    cLib_addCalc2(&i_this->mDemoCamEye.y, i_this->mDemoCamEyeTarget.y, param_1, i_this->mDemoCamEyeSpd.y * i_this->field_0xd88);
    cLib_addCalc2(&i_this->mDemoCamEye.z, i_this->mDemoCamEyeTarget.z, param_1, i_this->mDemoCamEyeSpd.z * i_this->field_0xd88);
}

static void demo_camera(e_gob_class* i_this) {
    fopAc_ac_c* actor = (fopAc_ac_c*)i_this;
    daPy_py_c* player = (daPy_py_c*)dComIfGp_getPlayer(0);
    camera_class* camera = dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0));
    camera_class* camera0 = dComIfGp_getCamera(0);
    cXyz sp94;
    cXyz sp88;
    cXyz cam_eye;
    cXyz cam_center;
    int spC = -1;

    switch (i_this->mDemoCamMode) {
    case 0:
        break;

    case 1:
        if (!actor->eventInfo.checkCommandDemoAccrpt()) {
            fopAcM_orderPotentialEvent(actor, 2, 0xFFFF, 0);
            actor->eventInfo.onCondition(2);
            return;
        }

        camera->mCamera.Stop();
        i_this->mDemoCamMode = 2;
        i_this->mDemoCamTimer = 0;

        i_this->mDemoCamFovy = 55.0f;
        camera->mCamera.SetTrimSize(3);
        i_this->mDemoCamCenter.set(-104.0f, 2205.0f, 1078.0f);
        i_this->mDemoCamEye.set(-175.0f, 2246.0f, 1418.0f);
        camera->mCamera.SetTrimSize(3);

        player->changeOriginalDemo();
        Z2GetAudioMgr()->subBgmStart(Z2BGM_MAGNE_GORON_D01);
        dComIfGp_getEvent().startCheckSkipEdge(i_this);
        // fallthrough
    case 2:
        if (i_this->mDemoCamTimer < 20) {
            sp88.set(0.0f, player->current.pos.y, 1100.0f);
            daPy_getPlayerActorClass()->setPlayerPosAndAngle(&sp88, -0x8000, 0);
        }

        if (i_this->mDemoCamTimer == 50) {
            i_this->mDemoCamMode = 3;
            i_this->mDemoCamTimer = 0;
            i_this->mDemoCamCenterTarget.set(0.0f, 2289.0f, -1694.0f);
            i_this->mDemoCamEyeTarget.set(0.0f, 2293.0f, -1349.0f);
            i_this->mDemoCamEyeSpd.x = fabsf(i_this->mDemoCamEyeTarget.x - i_this->mDemoCamEye.x);
            i_this->mDemoCamEyeSpd.y = fabsf(i_this->mDemoCamEyeTarget.y - i_this->mDemoCamEye.y);
            i_this->mDemoCamEyeSpd.z = fabsf(i_this->mDemoCamEyeTarget.z - i_this->mDemoCamEye.z);
            i_this->mDemoCamCenterSpd.x = fabsf(i_this->mDemoCamCenterTarget.x - i_this->mDemoCamCenter.x);
            i_this->mDemoCamCenterSpd.y = fabsf(i_this->mDemoCamCenterTarget.y - i_this->mDemoCamCenter.y);
            i_this->mDemoCamCenterSpd.z = fabsf(i_this->mDemoCamCenterTarget.z - i_this->mDemoCamCenter.z);
        }
        break;

    case 3:
        cam_3d_morf(i_this, 0.2f);
        cLib_addCalc2(&i_this->field_0xd88, 0.1f + VREG_F(1), 1.0f, 0.002f + VREG_F(2));

        if (i_this->mDemoCamTimer >= 150) {
            i_this->mDemoCamTimer = 150;
        }

        if (i_this->mDemoCamTimer >= 60) {
            if (i_this->mDemoCamTimer == 60) {
                i_this->mMsgFlow.init(i_this, 201, 0, NULL);
            }

            if (i_this->mMsgFlow.doFlow(i_this, NULL, 0)) {
                anm_init(i_this, BCK_MG_DE_JUMP_A, 5.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
                i_this->mDemoCamMode = 4;
                i_this->mDemoCamTimer = 0;
                i_this->mMode = 3;
            }
        }
        break;

    case 4:
        if (i_this->mDemoCamTimer > 36) {
            i_this->mDemoCamMode = 5;
            i_this->mDemoCamTimer = 0;
            i_this->mDemoCamCenter.set(-104.0f, 2205.0f, 1078.0f);
            i_this->mDemoCamEye.set(-175.0f, 2246.0f, 1418.0f);
            Z2GetAudioMgr()->subBgmStart(Z2BGM_MAGNE_GORON_D02);
        }
        break;

    case 5:
        if (i_this->mDemoCamTimer == 10) {
            player->changeDemoMode(1, 1, 2, 0);
        }

        if (i_this->mDemoCamTimer == 52) {
            player->changeDemoMode(0x22, 0, 0, 0);
        }
    
        if (i_this->mDemoCamTimer < 30) {
            actor->current.pos.y = 3000.0f;
            actor->speed.y = 0.0f;
        }
    
        if (i_this->mDemoCamTimer == 47) {
            ms->mAction = obj_msima_class::ACTION_DROP;
            ms->field_0x594 = ms->field_0x596 = 0;
            ms->field_0x59c = 700.0f;
            ms->field_0x586[0] = 40;
        }
    
        if (i_this->mDemoCamTimer == 54) {
            spC = 2;
        } else if (i_this->mDemoCamTimer == 57) {
            spC = 3;
        } else if (i_this->mDemoCamTimer == 59) {
            spC = 1;
        } else if (i_this->mDemoCamTimer == 62) {
            spC = 0;
        }
    
        if (spC >= 0) {
            ms->mChains[spC].field_0x92 = 1;
            ms->mChains[spC].field_0x8c = 200.0f + TREG_F(12);
            dComIfGp_particle_set(0x8455, &ms->mChains[spC].mPos, NULL, NULL);
            mDoAud_seStart(Z2SE_OBJ_GOBFLOOR_CHAIN_BREAK, &ms->mChains[spC].mPos, 0, dComIfGp_getReverb(fopAcM_GetRoomNo(actor)));
        }
    
        if (i_this->mDemoCamTimer == 82) {
            i_this->mDemoCamMode = 6;
            i_this->mDemoCamTimer = 0;
            i_this->mDemoCamCenter.set(-293.0f, 279.0f, 1988.0f);
            i_this->mDemoCamEye.set(-341.0f, 24.0f, 2223.0f);
        }
        break;

    case 6:
        if (i_this->mDemoCamTimer == 30) {
            dComIfGp_getVibration().StartShock(VIBMODE_S_POWER8, 0x1F, cXyz(0.0f, 1.0f, 0.0f));
            i_this->field_0xd8c = 15.0f + BREG_F(8);
        }

        if (i_this->mDemoCamTimer > 90) {
            i_this->mDemoCamMode = 7;
            i_this->mDemoCamTimer = 0;

            i_this->mDemoCamCenterTarget.set(7.4f, 945.0f, 806.0f);
            i_this->mDemoCamEyeTarget.set(-168.0f, 921.0f, 1450.0f);
            i_this->mDemoCamEyeSpd.x = fabsf(i_this->mDemoCamEyeTarget.x - i_this->mDemoCamEye.x);
            i_this->mDemoCamEyeSpd.y = 2.0f * fabsf(i_this->mDemoCamEyeTarget.y - i_this->mDemoCamEye.y);
            i_this->mDemoCamEyeSpd.z = fabsf(i_this->mDemoCamEyeTarget.z - i_this->mDemoCamEye.z);
            i_this->mDemoCamCenterSpd.x = fabsf(i_this->mDemoCamCenterTarget.x - i_this->mDemoCamCenter.x);
            i_this->mDemoCamCenterSpd.y = 2.0f * fabsf(i_this->mDemoCamCenterTarget.y - i_this->mDemoCamCenter.y);
            i_this->mDemoCamCenterSpd.z = fabsf(i_this->mDemoCamCenterTarget.z - i_this->mDemoCamCenter.z);
            i_this->field_0xd88 = 0.0f;

            ms->mAction = obj_msima_class::ACTION_FLOAT_1;
            ms->field_0x584 = 0;
        }
        break;

    case 7:
        cam_3d_morf(i_this, 0.2f);
        cLib_addCalc2(&i_this->field_0xd88, 0.05f + VREG_F(1), 1.0f, 0.001f + VREG_F(2));

        if (i_this->mDemoCamTimer > 110) {
            i_this->mDemoCamMode = 100;
            i_this->mAction = ACTION_FIGHT;
            i_this->mMode = -1;
            Z2GetAudioMgr()->subBgmStart(Z2BGM_MAGNE_GORON);
        }
        break;

    case 10:
        player->cancelGoronThrowEvent();
        i_this->mDemoCamMode = 11;
        // fallthrough
    case 11:
        if (!actor->eventInfo.checkCommandDemoAccrpt()) {
            fopAcM_orderPotentialEvent(actor, 2, 0xFFFF, 0);
            actor->eventInfo.onCondition(2);
            return;
        }

        camera->mCamera.Stop();
        i_this->mDemoCamMode = 0xC;
        i_this->mDemoCamTimer = 0;
        i_this->mDemoCamFovy = 60.0f;
        camera->mCamera.SetTrimSize(3);
        actor->speed.y = 0.0f;
        actor->current.pos.x = 0.0f;
        actor->current.pos.y = 700.0f;
        actor->current.pos.z = 2000.0f;
        i_this->field_0x6b8.x = 0.0f;
        i_this->field_0x6b8.z = 30.0f;
        i_this->mDemoCamEye.set(-500.0f, 100.0f, 1800.0f);
        i_this->mDemoCamCenter = actor->current.pos;
        camera->mCamera.SetTrimSize(3);
        sp88.set(0.0f, 500.0f + player->current.pos.y, -400.0f);
        daPy_getPlayerActorClass()->setPlayerPosAndAngle(&sp88, 0, 0);
        // fallthrough
    case 12:
        if (i_this->mDemoCamTimer == 2) {
            daPy_getPlayerActorClass()->changeDemoMode(0x17, 1, 2, 0);
        }

        sp88.set(0.0f, player->current.pos.y, -200.0f);
        daPy_getPlayerActorClass()->setPlayerPosAndAngle(&sp88, 0, 0);

        if (i_this->field_0x6d6 >= 2) {
            cLib_addCalc2(&i_this->mDemoCamEye.x, -500.0f + VREG_F(3), 0.1f, 20.0f);
            cLib_addCalc2(&i_this->mDemoCamEye.z, 700.0f + VREG_F(4), 0.1f, 20.0f);
            cLib_addCalc2(&i_this->mDemoCamEye.y, 1400.0f + VREG_F(8), 0.1f, 35.0f);
            cLib_addCalc2(&i_this->mDemoCamCenter.x, (actor->current.pos.x + VREG_F(5)) - 200.0f, 0.2f, 100.0f);
            cLib_addCalc2(&i_this->mDemoCamCenter.y, 250.0f + (VREG_F(6) + (actor->current.pos.y + i_this->field_0x69c)), 0.2f, 200.0f);
            cLib_addCalc2(&i_this->mDemoCamCenter.z, (actor->current.pos.z + VREG_F(7)) - 100.0f, 0.2f, 100.0f);

            if (i_this->mDemoCamTimer == 128) {
                i_this->field_0x6da = 1;
                i_this->field_0x6e8.x = -5.0f;
                i_this->field_0x6e8.y = 50.0f;
                i_this->field_0x6e8.z = -50.0f + KREG_F(7);
                mDoAud_seStart(Z2SE_EN_GOB_HELMET_OFF, &i_this->field_0x6dc, 0, dComIfGp_getReverb(fopAcM_GetRoomNo(actor)));
            }
        } else {
            cLib_addCalc2(&i_this->mDemoCamCenter.x, actor->current.pos.x + VREG_F(5), 0.2f, 100.0f);
            cLib_addCalc2(&i_this->mDemoCamCenter.y, VREG_F(6) + (actor->current.pos.y + i_this->field_0x69c), 0.2f, 200.0f);
            cLib_addCalc2(&i_this->mDemoCamCenter.z, actor->current.pos.z + VREG_F(7), 0.2f, 100.0f);
        }

        if (i_this->mDemoCamTimer == 190) {
            i_this->field_0x6dc.y = 0.0f;
            mDoAud_seStart(Z2SE_EN_GOB_HELMET_MELT, &i_this->field_0x6dc, 0, dComIfGp_getReverb(fopAcM_GetRoomNo(actor)));
            OS_REPORT("/////GOB MET MELT %d\n");
        }

        if (i_this->mDemoCamTimer != 200) {
            break;
        }

        i_this->mDemoCamMode = 13;
        i_this->mDemoCamTimer = 0;
        i_this->mDemoCamEye.set(306.0f, 1187.0f, -372.0f);
        i_this->mDemoCamCenter = actor->eyePos;
        // fallthrough
    case 13:
        sp88.set(0.0f, player->current.pos.y, -200.0f);
        daPy_getPlayerActorClass()->setPlayerPosAndAngle(&sp88, 0, 0);

        if (i_this->mDemoCamTimer == 20) {
            i_this->mMode = 2;
        }

        if (i_this->mDemoCamTimer >= 150) {
            i_this->mDemoCamTimer = 150;
        }

        if (i_this->mDemoCamTimer >= 60) {
            if (i_this->mDemoCamTimer == 60) {
                i_this->mMsgFlow.init(i_this, 202, 0, NULL);
                Z2GetAudioMgr()->unMuteSceneBgm(45);
            }

            if ((i_this->mMsgFlow.getMsgNo() == 0x1777 || mDoCPd_c::getTrigA(PAD_1)) && i_this->mMode < 4) {
                i_this->mMode = 4;
            }

            if (i_this->mMsgFlow.doFlow(i_this, NULL, 0)) {
                i_this->mDemoCamMode = 14;
                i_this->mDemoCamTimer = 0;
            }
        }
        // fallthrough
    case 14:
        cLib_addCalc2(&i_this->mDemoCamCenter.x, actor->eyePos.x + NREG_F(7), 0.1f, 50.0f);
        cLib_addCalc2(&i_this->mDemoCamCenter.y, actor->eyePos.y + NREG_F(8), 0.1f, 50.0f);
        cLib_addCalc2(&i_this->mDemoCamCenter.z, actor->eyePos.z + NREG_F(9), 0.1f, 50.0f);

        if (i_this->mDemoCamMode == 14 && i_this->mDemoCamTimer == 20) {
            mDoGph_gInf_c::fadeOut(0.1f, g_blackColor);
        }

        if (i_this->mDemoCamMode != 14 || i_this->mDemoCamTimer != 30) {
            break;
        }

        i_this->mDemoCamMode = 15;
        i_this->mDemoCamTimer = 0;
        ms->mAction = obj_msima_class::ACTION_FLOAT_2;
        my->mAction = 1;
        actor->current = player->current;
        actor->shape_angle = player->shape_angle;
        my->current.pos.y = 1000.0f + (ms->field_0x58c - 700.0f) + YREG_F(1);
        dComIfGp_getVibration().StartQuake(VIBMODE_Q_POWER2, 1, cXyz(0.0f, 1.0f, 0.0f));
        // fallthrough
    case 15:
        if (i_this->mDemoCamTimer == 2) {
            i_this->mDemoCamFovy = 60.0f;
            i_this->mDemoCamCenter.set(-2218.0f, 3100.0f, 1256.0f);
            i_this->mDemoCamEye.set(-2486.0f, 3256.0f, 1418.0f);
            mDoGph_gInf_c::fadeIn(0.1f, g_blackColor);
        }

        cLib_addCalc2(&ms->field_0x58c, 1150.0f + YREG_F(0), 0.03f, 5.0f);

        if (ms->field_0x58c < (1150.0f + YREG_F(0)) - 50.0f) {
            i_this->field_0xd8c = 0.75f + YREG_F(2);
            i_this->mDemoCamTimer++;

            cXyz sp64(0.0f, 0.0f, 0.0f);
            i_this->field_0xd30 = dComIfGp_particle_set(i_this->field_0xd30, 0x844D, &sp64, NULL, NULL);
            dComIfGp_particle_levelEmitterOnEventMove(i_this->field_0xd30);

            sp88.set(0.0f, player->current.pos.y, 400.0f);
            daPy_getPlayerActorClass()->setPlayerPosAndAngle(&sp88, -0x8000, 0);
        } else if (i_this->mDemoCamTimer == 480) {
            dComIfGp_getVibration().StopQuake(0x1F);
        }

        my->current.pos.y = 1000.0f + (ms->field_0x58c - 700.0f) + YREG_F(1);

        if (i_this->mDemoCamTimer == 550) {
            i_this->mDemoCamMode = 100;
            i_this->mAction = ACTION_MESSAGE;
            i_this->mMode = 0;
            dComIfGs_onStageMiddleBoss();
            fopAcM_onSwitch(actor, 14);
        }
        break;

    case 100:
        camera->mCamera.Reset(i_this->mDemoCamCenter, i_this->mDemoCamEye, i_this->mDemoCamFovy, 0);
        camera->mCamera.Start();
        camera->mCamera.SetTrimSize(0);
        dComIfGp_event_reset();
        daPy_getPlayerActorClass()->cancelOriginalDemo();
        i_this->mDemoCamMode = 0;
        break;
    }

    if (i_this->mDemoCamMode != 0) {
        cam_center = i_this->mDemoCamCenter;
        cam_eye = i_this->mDemoCamEye;
        s16 bank = 0;

        if (i_this->field_0xd8c > 0.0f) {
            i_this->mDemoCamCenter.y += i_this->field_0xd8c * cM_ssin((i_this->mDemoCamTimer * 0x1200));
            i_this->mDemoCamEye.y -= i_this->field_0xd8c * cM_ssin((i_this->mDemoCamTimer * 0x1200));
            bank = (20.0f + BREG_F(4)) * (i_this->field_0xd8c * cM_scos((i_this->mDemoCamTimer * 0x1300)));
            cLib_addCalc0(&i_this->field_0xd8c, 1.0f, 0.5f + BREG_F(5));
        }

        camera->mCamera.Set(cam_center, cam_eye, bank, i_this->mDemoCamFovy);

        i_this->mDemoCamTimer++;
        if (i_this->mDemoCamTimer > 10000) {
            i_this->mDemoCamTimer = 10000;
        }

        if (i_this->mDemoCamMode >= 2 && i_this->mDemoCamMode < 10 && dComIfGp_getEvent().checkSkipEdge()) {
            cDmr_SkipInfo = 20;
            i_this->mDemoCamMode = 1000;
            dStage_changeScene(2, 0.0f, 0, fopAcM_GetRoomNo(actor), 0, -1);
        }
    }
}

static int daE_GOB_Execute(e_gob_class* i_this) {
    if (cDmr_SkipInfo != 0) {
        cDmr_SkipInfo--;
    }

    if (i_this->mDemoCamMode == 1000) {
        return 1;
    }

    cXyz sp48;
    cXyz sp3C;

    ms = (obj_msima_class*)fopAcM_SearchByName(PROC_OBJ_MSIMA);
    my = (obj_myogan_class*)fopAcM_SearchByName(PROC_OBJ_MYOGAN);
    i_this->mCounter++;

    for (int i = 0; i < 5; i++) {
        if (i_this->mTimers[i] != 0) {
            i_this->mTimers[i]--;
        }
    }

    if (i_this->mDamageInvulnerabilityTimer != 0) {
        i_this->mDamageInvulnerabilityTimer--;
    }

    if (i_this->field_0x6aa != 0) {
        i_this->field_0x6aa--;
    }

    i_this->field_0x69c = 0.0f;

    action(i_this);
    damage_check(i_this);

    mDoMtx_stack_c::transS(i_this->current.pos.x, i_this->current.pos.y + i_this->field_0x69c, i_this->current.pos.z);
    mDoMtx_stack_c::XrotM(i_this->field_0x6a0);
    mDoMtx_stack_c::ZrotM(i_this->field_0x6a2);
    mDoMtx_stack_c::YrotM(i_this->shape_angle.y);
    mDoMtx_stack_c::XrotM(i_this->shape_angle.x);
    mDoMtx_stack_c::ZrotM(i_this->shape_angle.z);
    mDoMtx_stack_c::scaleM(l_HIO.size, l_HIO.size, l_HIO.size);
    mDoMtx_stack_c::transM(0.0f, i_this->field_0x680, i_this->field_0x684);
    mDoMtx_stack_c::XrotM(i_this->field_0x6b2);
    mDoMtx_stack_c::transM(0.0f, -i_this->field_0x680, -i_this->field_0x684);

    J3DModel* sp34 = i_this->mpModelMorf->getModel();
    sp34->setBaseTRMtx(mDoMtx_stack_c::get());

    i_this->mpModelMorf->play(0, dComIfGp_getReverb(fopAcM_GetRoomNo(i_this)));

    if (i_this->mAnm == BCK_MG_FIRE) {
        i_this->field_0xd30 = dComIfGp_particle_set(i_this->field_0xd30, 0x8459, &i_this->current.pos, &i_this->shape_angle, &base_sc);
        i_this->field_0xd34 = dComIfGp_particle_set(i_this->field_0xd34, 0x845A, &i_this->current.pos, &i_this->shape_angle, &base_sc);
        dComIfGp_particle_levelEmitterOnEventMove(i_this->field_0xd30);
        dComIfGp_particle_levelEmitterOnEventMove(i_this->field_0xd34);
    }

    if (i_this->mAnm == BCK_MG_WALK) {
        if (i_this->mpModelMorf->checkFrame(21.0f) || i_this->mpModelMorf->checkFrame(44.0f)) {
            i_this->mSound.startCreatureSound(Z2SE_FN_GOB_WALK, 0, -1);
        }
    } else if (i_this->mAnm == BCK_MG_STAND) {
        if (i_this->mpModelMorf->checkFrame(27.0f)) {
            i_this->mSound.startCreatureSound(Z2SE_FN_GOB_WALK, 0, -1);
        }
    } else if (i_this->mAnm == BCK_MG_ATTACK_A_A) {
        if (i_this->mpModelMorf->checkFrame(9.0f)) {
            i_this->mSound.startCreatureSound(Z2SE_FN_GOB_WALK, 0, -1);
        }
    } else if (i_this->mAnm == BCK_MG_DE_JUMP_A) {
        if (i_this->mpModelMorf->checkFrame(18.0f)) {
            i_this->mSound.startCreatureSound(Z2SE_FN_GOB_WALK, 0, -1);
        }
    } else if (i_this->mAnm == BCK_MG_DE_GETUP) {
        if (i_this->mpModelMorf->checkFrame(32.5f)) {
            i_this->mSound.startCreatureSound(Z2SE_FN_GOB_WALK, 0, -1);
        }

        if (i_this->mpModelMorf->checkFrame(10.0f) || i_this->mpModelMorf->checkFrame(24.0f)) {
            i_this->mSound.startCreatureSound(Z2SE_FN_GOB_STEP, 0, -1);
        }
    } else if (i_this->mAnm == BCK_MG_ST_WALK) {
        if (i_this->mpModelMorf->checkFrame(7.5f) || i_this->mpModelMorf->checkFrame(15.0f) || i_this->mpModelMorf->checkFrame(22.5f) || i_this->mpModelMorf->checkFrame(29.5f)) {
            i_this->mSound.startCreatureSound(Z2SE_FN_GOB_STEP, 0, -1);
        }
    } else if (i_this->mAnm == BCK_MG_DE_STANDUP_ST || i_this->mAnm == BCK_MG_DE_STANDUP_ED) {
        if (i_this->mpModelMorf->checkFrame(24.5f) || i_this->mpModelMorf->checkFrame(41.0f)) {
            i_this->mSound.startCreatureSound(Z2SE_FN_GOB_STEP, 0, -1);
        }
    } else if (i_this->mAnm == BCK_MG_LANDING) {
        if (i_this->mpModelMorf->checkFrame(2.5f)) {
            i_this->mSound.startCreatureSound(Z2SE_FN_GOB_JUMP, 0, -1);
        }
    } else if (i_this->mAnm == BCK_MG_DE_TALK_ST) {
        if (i_this->mpModelMorf->checkFrame(16.0f)) {
            i_this->mSound.startCreatureSound(Z2SE_EN_GOB_KNUCKLES_FRICTION, 0, -1);
        }
    } else if (i_this->mAnm == BCK_MG_EXCITE && i_this->mpModelMorf->checkFrame(60.0f)) {
        i_this->mSound.startCreatureSound(Z2SE_EN_GOB_KNUCKLES_FRICTION, 0, -1);
    }

    i_this->mpModelMorf->modelCalc();

    if (i_this->field_0x6da == 0) {
        MTXCopy(sp34->getAnmMtx(JNT_NECK), mDoMtx_stack_c::get());
        mDoMtx_stack_c::multVecZero(&i_this->field_0x6dc);
    } else {
        i_this->field_0x6dc += i_this->field_0x6e8;
        i_this->field_0x6e8.y -= 5.0f;

        dBgS_ObjGndChk sp54;
        sp48 = i_this->field_0x6dc;
        sp48.y += 500.0f;
        sp54.SetPos(&sp48);

        f32 temp_f1 = dComIfG_Bgsp().GroundCross(&sp54);
        if (i_this->field_0x6dc.y <= temp_f1 + NREG_F(7)) {
            i_this->field_0x6dc.y = temp_f1 + NREG_F(7);
            if (i_this->field_0x6da == 1) {
                i_this->field_0x6e8.y = 50.0f;
                i_this->field_0x6e8.z *= 0.5f;
                i_this->field_0x6e8.x = -30.0f;
                i_this->field_0x6da = 2;
                dComIfGp_particle_set(0x8455, &i_this->field_0x6dc, NULL, NULL);
                mDoAud_seStart(Z2SE_EN_GOB_HELMET_BOUND, &i_this->field_0x6dc, 0, dComIfGp_getReverb(fopAcM_GetRoomNo(i_this)));
            } else {
                i_this->field_0x6e8.y = -5.0f;
                i_this->field_0x6e8.z = 0.0f;
            }
        } else if (i_this->field_0x6da == 1) {
            i_this->field_0x6f4.y += 2000;
            i_this->field_0x6f4.x += 800;
        } else {
            i_this->field_0x6f4.y -= 7000;
            i_this->field_0x6f4.x -= 2000;
        }
    
        mDoMtx_stack_c::transS(i_this->field_0x6dc.x, i_this->field_0x6dc.y, i_this->field_0x6dc.z);
        mDoMtx_stack_c::YrotM(i_this->field_0x6f4.y);
        mDoMtx_stack_c::XrotM(i_this->field_0x6f4.x);
        mDoMtx_stack_c::ZrotM(i_this->field_0x6f4.z);
        mDoMtx_stack_c::scaleM(l_HIO.size, l_HIO.size, l_HIO.size);
    }

    i_this->mpArmorModel->setBaseTRMtx(mDoMtx_stack_c::get());

    if (i_this->mAction == ACTION_BALL || i_this->mAction == ACTION_GLAB) {
        i_this->eyePos = i_this->current.pos;
        i_this->eyePos.y += 150.0f + i_this->field_0x69c + JREG_F(3);
        i_this->attention_info.position = i_this->eyePos;
        i_this->attention_info.position.y += 170.0f + JREG_F(2);
    } else {
        MTXCopy(sp34->getAnmMtx(JNT_HEAD), *calc_mtx);
        sp48.set(0.0f, 0.0f, 0.0f);
        MtxPosition(&sp48, &i_this->eyePos);
        i_this->attention_info.position = i_this->eyePos;
        i_this->attention_info.position.y += 120.0f + JREG_F(0);
    }

    MTXCopy(sp34->getAnmMtx(JNT_BACK_BONE1), *calc_mtx);
    sp48.set(50.0f + JREG_F(1), JREG_F(2), JREG_F(3));
    MtxPosition(&sp48, &sp3C);
    if (i_this->mDamageInvulnerabilityTimer != 0) {
        sp3C.z += 12000.0f;
    }
    i_this->mCcSph.SetC(sp3C);
    i_this->mCcSph.SetR(150.0f + YREG_F(15));
    dComIfG_Ccsp()->Set(&i_this->mCcSph);

    if (i_this->field_0xd20 != 0) {
        i_this->mCcSph.OnTgShield();
        i_this->mCcSph.SetTgHitMark((CcG_Tg_HitMark)2);
        i_this->field_0xd20 = 0;
    } else {
        i_this->mCcSph.OffTgShield();
        i_this->mCcSph.SetTgHitMark((CcG_Tg_HitMark)0);
    }

    MTXCopy(sp34->getAnmMtx(JNT_ARM_R4), *calc_mtx);
    sp48.set(0.0f, 0.0f, 0.0f);
    MtxPosition(&sp48, &i_this->field_0xd24);

    if (i_this->field_0xd1d != 0) {
        if (i_this->field_0xd1d == 1) {
            if (i_this->field_0xd1e == 0) {
                i_this->field_0xd1e = 1;
                i_this->mAtSph.StartCAt(i_this->field_0xd24);
            } else {
                i_this->mAtSph.MoveCAt(i_this->field_0xd24);
            }
            i_this->mAtSph.SetR((100.0f + TREG_F(8)) * l_HIO.size);
        } else {
            MTXCopy(sp34->getAnmMtx(JNT_BACK_BONE1), *calc_mtx);
            sp48.set(70.0f + NREG_F(1), 50.0f + NREG_F(2), NREG_F(3));
            MtxPosition(&sp48, &sp3C);
            i_this->mAtSph.SetC(sp3C);
            i_this->mAtSph.SetR((120.0f + NREG_F(18)) * l_HIO.size);
        }

        i_this->field_0xd1d = 0;

        if (daPy_getPlayerActorClass()->checkEquipHeavyBoots()) {
            i_this->mAtSph.SetAtSpl((dCcG_At_Spl)2);
        } else {
            i_this->mAtSph.SetAtSpl((dCcG_At_Spl)7);
        }
    } else {
        i_this->field_0xd1e = 0;

        sp3C = i_this->field_0xd24;
        sp3C.x += 13000.0f;
        i_this->mAtSph.SetC(sp3C);
    }

    dComIfG_Ccsp()->Set(&i_this->mAtSph);
    demo_camera(i_this);
    return 1;
}

static int daE_GOB_IsDelete(e_gob_class* i_this) {
    return 1;
}

static int daE_GOB_Delete(e_gob_class* i_this) {
    fopAcM_GetID(i_this);
    dComIfG_resDelete(&i_this->mPhase, "E_gob");

    if (i_this->mInitHIO) {
        l_initHIO = 0;
        mDoHIO_DELETE_CHILD(l_HIO.id);
    }

    if (i_this->heap != NULL) {
        i_this->mpModelMorf->stopZelAnime();
    }

    return 1;
}

static int useHeapInit(fopAc_ac_c* i_this) {
    e_gob_class* a_this = (e_gob_class*)i_this;

    a_this->mpModelMorf = new mDoExt_McaMorfSO((J3DModelData*)dComIfG_getObjectRes("E_gob", BMDR_MG), NULL, NULL, (J3DAnmTransform*)dComIfG_getObjectRes("E_gob", BCK_MG_WAIT), 2, 1.0f, 0, -1, &a_this->mSound, 0x80000, 0x11000084);
    if (a_this->mpModelMorf == NULL || a_this->mpModelMorf->getModel() == NULL) {
        return 0;
    }

    J3DModel* model = a_this->mpModelMorf->getModel();
    model->setUserArea((uintptr_t)a_this);

    for (u16 i = 0; i < model->getModelData()->getJointNum(); i++) {
        model->getModelData()->getJointNodePointer(i)->setCallBack(nodeCallBack);
    }

    void* modelData = dComIfG_getObjectRes("E_gob", BMDR_MG_MET);
    JUT_ASSERT(3631, modelData != NULL);
    a_this->mpArmorModel = mDoExt_J3DModel__create((J3DModelData*)modelData, 0x80000, 0x11000084);
    if (a_this->mpArmorModel == NULL) {
        return 0;
    }

    return 1;
}

static int daE_GOB_Create(fopAc_ac_c* i_this) {
    e_gob_class* a_this = (e_gob_class*)i_this;
    fopAcM_ct(a_this, e_gob_class);

    OS_REPORT("E_GOB//////////////E_GOB SET 0 !!\n");

    int sp14 = dComIfG_resLoad(&a_this->mPhase, "E_gob");
    if (sp14 == cPhs_COMPLEATE_e) {
        OS_REPORT("E_GOB PARAM %x\n", fopAcM_GetParam(i_this));

        if (dComIfGs_isStageBossEnemy()) {
            OS_REPORT("本ボス攻略後なので再セットしません\n");
            return cPhs_ERROR_e;
        }

        OS_REPORT("E_GOB//////////////E_GOB SET 1 !!\n");

        if (!fopAcM_entrySolidHeap(i_this, useHeapInit, 0x4B000)) {
            OS_REPORT("//////////////E_GOB SET NON !!\n");
            return cPhs_ERROR_e;
        }

        OS_REPORT("//////////////E_GOB SET 2 !!\n");

        if (l_initHIO == 0) {
            a_this->mInitHIO = 1;
            l_initHIO = 1;
            l_HIO.id = mDoHIO_CREATE_CHILD("マグネゴロン", &l_HIO);
        }
    
        i_this->attention_info.flags = fopAc_AttnFlag_BATTLE_e;

        fopAcM_SetMtx(i_this, a_this->mpModelMorf->getModel()->getBaseTRMtx());
        fopAcM_SetMin(i_this, -400.0f, -500.0f, -400.0f);
        fopAcM_SetMax(i_this, 400.0f, 500.0f, 400.0f);

        i_this->attention_info.distances[fopAc_attn_BATTLE_e] = 54;
        i_this->attention_info.distances[fopAc_attn_CARRY_e] = 40;
        i_this->attention_info.distances[fopAc_attn_LOCK_e] = 22;
        i_this->attention_info.distances[fopAc_attn_ETC_e] = 22;
        i_this->attention_info.distances[fopAc_attn_TALK_e] = 22;
        i_this->attention_info.distances[fopAc_attn_SPEAK_e] = 22;

        i_this->health = 1000;
        i_this->field_0x560 = 1000;
    
        a_this->mAcch.Set(fopAcM_GetPosition_p(i_this), fopAcM_GetOldPosition_p(i_this), i_this, 1, &a_this->mAcchCir, fopAcM_GetSpeed_p(i_this), NULL, NULL);
        a_this->mAcchCir.SetWall(50.0f, 50.0f);

        a_this->mSound.init(&i_this->current.pos, &i_this->eyePos, &a_this->field_0xd24, 3, 1, 2);
        a_this->mAtInfo.mpSound = &a_this->mSound;

        i_this->gravity = -7.0f;

        static dCcD_SrcSph cc_sph_src = {
            {
                {0x0, {{0x0, 0x0, 0x0}, {0xd8fbfdff, 0x3}, 0x75}}, // mObj
                {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0}, // mGObjAt
                {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x2}, // mGObjTg
                {0x0}, // mGObjCo
            }, // mObjInf
            {
                {{0.0f, 0.0f, 0.0f}, 40.0f} // mSph
            } // mSphAttr
        };

        static dCcD_SrcSph at_sph_src = {
            {
                {0x0, {{AT_TYPE_CSTATUE_SWING, 0x2, 0xd}, {0x0, 0x0}, 0x0}}, // mObj
                {dCcD_SE_METAL, 0x0, 0x2, 0x0, 0x0}, // mGObjAt
                {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x2}, // mGObjTg
                {0x0}, // mGObjCo
            }, // mObjInf
            {
                {{0.0f, 0.0f, 0.0f}, 35.0f} // mSph
            } // mSphAttr
        };

        a_this->mCcStts.Init(100, 0, i_this);

        a_this->mCcSph.Set(cc_sph_src);
        a_this->mCcSph.SetStts(&a_this->mCcStts);
        a_this->mCcSph.OnTgNoHitMark();

        a_this->mAtSph.Set(at_sph_src);
        a_this->mAtSph.SetStts(&a_this->mCcStts);
    
        if (dComIfGs_isStageMiddleBoss()) {
            a_this->mAction = ACTION_MESSAGE;
            a_this->mMode = 0;
            i_this->current.pos.x = 0.0f;
            i_this->current.pos.y = 2120.0f;
            i_this->current.pos.z = -500.0f;
            i_this->old.pos = i_this->current.pos;
            a_this->mTimers[3] = 60;
        } else {
            i_this->home.pos.x = 0.0f;
            i_this->home.pos.z = 0.0f;

            if (cDmr_SkipInfo != 0 || dComIfGs_isZoneSwitch(5, -1)) {
                a_this->mAction = ACTION_S_SET;
                i_this->current.pos.x = 0.0f;
                i_this->current.pos.y = 1000.0f;
                i_this->current.pos.z = -300.0f;
                Z2GetAudioMgr()->subBgmStart(Z2BGM_MAGNE_GORON);
            } else {
                a_this->mAction = ACTION_START;
                i_this->current.pos.x = 0.0f;
                i_this->current.pos.y = 2100.0f;
                i_this->current.pos.z = -2180.0f;
            }

            i_this->old.pos = i_this->current.pos;
        }

        dComIfGs_onZoneSwitch(5, -1);
        /* dSv_event_flag_c::F_0671 - Goron Mines - Through magnet goron into lava once */
        dComIfGs_offEventBit(dSv_event_flag_c::saveBitLabels[671]);
        /* dSv_event_flag_c::F_0672 - Goron Mines - First time seeing magnet goron tumbling */
        dComIfGs_offEventBit(dSv_event_flag_c::saveBitLabels[672]);
        daE_GOB_Execute(a_this);
    }

    return sp14;
}

e_gob_class::e_gob_class() {}

static actor_method_class l_daE_GOB_Method = {
    (process_method_func)daE_GOB_Create,
    (process_method_func)daE_GOB_Delete,
    (process_method_func)daE_GOB_Execute,
    (process_method_func)daE_GOB_IsDelete,
    (process_method_func)daE_GOB_Draw,
};

extern actor_process_profile_definition g_profile_E_GOB = {
  fpcLy_CURRENT_e,        // mLayerID
  7,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_E_GOB,             // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(e_gob_class),    // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  116,                    // mPriority
  &l_daE_GOB_Method,      // sub_method
  0x00040100,             // mStatus
  fopAc_ENEMY_e,          // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};

AUDIO_INSTANCES
