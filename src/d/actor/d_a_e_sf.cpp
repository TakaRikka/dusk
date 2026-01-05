/**
 * @file d_a_e_sf.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_e_sf.h"
#include "d/d_cc_d.h"
#include "d/d_camera.h"
#include "f_op/f_op_actor_enemy.h"
#include "f_op/f_op_camera_mng.h"
#include "Z2AudioLib/Z2Instances.h"

enum E_sf_RES_File_ID {
    /* BCK */
    /* 0x04 */ BCK_SF_ATTACK01 = 0x4,
    /* 0x05 */ BCK_SF_ATTACK02,
    /* 0x06 */ BCK_SF_DAMAGE,
    /* 0x07 */ BCK_SF_DIE,
    /* 0x08 */ BCK_SF_DRAWBACK,
    /* 0x09 */ BCK_SF_GETUP,
    /* 0x0A */ BCK_SF_GUARD,
    /* 0x0B */ BCK_SF_OPDEMO,
    /* 0x0C */ BCK_SF_WAIT01,
    /* 0x0D */ BCK_SF_WAIT02,
    /* 0x0E */ BCK_SF_WALK01,
    /* 0x0F */ BCK_SF_WALK02,
    /* 0x10 */ BCK_SF_WALK03,

    /* BMDR */
    /* 0x13 */ BMDR_SF = 0x13,
    /* 0x14 */ BMDR_SF_SHIELDA,
    /* 0x15 */ BMDR_SF_SHIELDB,
    /* 0x16 */ BMDR_SF_SWORDA,
    /* 0x17 */ BMDR_SF_SWORDB,
};

enum Joint {
    /* 0x00 */ JNT_WORLD_ROOT,
    /* 0x01 */ JNT_BACKBONE1,
    /* 0x02 */ JNT_BACKBONE2,
    /* 0x03 */ JNT_NECK,
    /* 0x04 */ JNT_HEAD,
    /* 0x05 */ JNT_CHIN,
    /* 0x06 */ JNT_SHOULDERL,
    /* 0x07 */ JNT_ARML1,
    /* 0x08 */ JNT_HANDL1,
    /* 0x09 */ JNT_HANDL2,
    /* 0x0A */ JNT_SHOULDERPADL,
    /* 0x0B */ JNT_SHOULDERR,
    /* 0x0C */ JNT_ARMR1,
    /* 0x0D */ JNT_HANDR1,
    /* 0x0E */ JNT_HANDR2,
    /* 0x0F */ JNT_SHOULDERPADR,
    /* 0x10 */ JNT_WAIST,
    /* 0x11 */ JNT_LEGL1,
    /* 0x12 */ JNT_LEGL2,
    /* 0x13 */ JNT_FOOTL1,
    /* 0x14 */ JNT_FOOTL2,
    /* 0x15 */ JNT_LEGR1,
    /* 0x16 */ JNT_LEGR2,
    /* 0x17 */ JNT_FOOTR1,
    /* 0x18 */ JNT_FOOTR2,
};

enum Action {
    /* 0x00 */ ACTION_NORMAL,
    /* 0x03 */ ACTION_FIGHT_RUN = 0x3,
    /* 0x04 */ ACTION_ATTACK_0,
    /* 0x05 */ ACTION_ATTACK,
    /* 0x07 */ ACTION_GUARD = 0x7,
    /* 0x08 */ ACTION_DRAWBACK,
    /* 0x14 */ ACTION_S_DAMAGE = 0x14,
    /* 0x1E */ ACTION_CRASH = 0x1E,
    /* 0x1F */ ACTION_GETUP,
    /* 0x20 */ ACTION_CRASHWAIT,
    /* 0x21 */ ACTION_SITWAIT,
};

enum Action_Phase {
    /* 0x0 */ PHASE_INIT,

    /* e_sf_normal */
    /* 0x1 */ NORMAL_PHASE_1 = 0x1,
    /* 0x2 */ NORMAL_PHASE_WALK,
    /* 0x3 */ NORMAL_PHASE_WAIT,

    /* e_sf_drawback */
    /* 0x1 */ DRAWBACK_PHASE_END = 0x1,

    /* e_sf_fight_run */
    /* -10 */ FIGHT_RUN_PHASE_NEG_10 = -10,
    /*  -9 */ FIGHT_RUN_PHASE_NEG_9 = -9,
    /* 0x0 */ FIGHT_RUN_PHASE_0 = 0x0,
    /* 0x1 */ FIGHT_RUN_PHASE_1,
    /* 0x2 */ FIGHT_RUN_PHASE_2,
    /* 0x3 */ FIGHT_RUN_PHASE_3,

    /* e_sf_attack_0 */
    /* 0x1 */ ATTACK_0_PHASE_END = 0x1,

    /* e_sf_attack */
    /* 0x1 */ ATTACK_PHASE_END = 0x1,

    /* e_sf_guard */
    /* 0x1 */ GUARD_PHASE_KNOCKBACK = 0x1,
    /* 0x2 */ GUARD_PHASE_RECOVER,
    /* 0x3 */ GUARD_PHASE_END,

    /* e_sf_s_damage */
    /* 0x1 */ S_DAMAGE_PHASE_END = 0x1,

    /* e_sf_crash */
    /* 0x1 */ CRASH_PHASE_WAIT = 0x1,
    /* 0x2 */ CRASH_PHASE_END,

    /* e_sf_crashwait */
    /* 0x1 */ CRASHWAIT_PHASE_END = 0x1,

    /* e_sf_sitwait */
    /* 0x1 */ SITWAIT_PHASE_1 = 0x1,
    /* 0x2 */ SITWAIT_PHASE_END,

    /* e_sf_getup */
    /* 0x1 */ GETUP_PHASE_GETUP = 0x1,
    /* 0x2 */ GETUP_PHASE_WAIT,
    /* 0x3 */ GETUP_PHASE_END,
};

class daE_SF_HIO_c {
public:
    daE_SF_HIO_c();
    virtual ~daE_SF_HIO_c() {}

    /* スタルフォス - Stalfos */
    /* 0x04 */ s8 id;
    /* 0x08 */ f32 basic_size;              // 基本サイズ - Basic Size
    /* 0x0C */ f32 move_spd;                // 移動速度 - Movement Speed
    /* 0x10 */ f32 charge_spd;              // 突進速度 - Charge Speed
    /* 0x14 */ f32 p_recognize_dist_m;      // P認識距離　M - P Recognition Distance M
    /* 0x18 */ f32 combat_start_range;      // 戦闘開始範囲 - Combat Start Range
    /* 0x1C */ f32 attack_start_range;      // 攻撃開始範囲 - Attack Start Range
    /* 0x20 */ f32 defense_recognize_range; // 防御認識範囲 - Defense Recognition Range
    /* 0x24 */ u8 invulnerable;             // 不死身 - Invulnerable
};

daE_SF_HIO_c::daE_SF_HIO_c() {
    id = -1;
    basic_size = 1.2f;
    move_spd = 5.0f;
    p_recognize_dist_m = 3000.0f;
    charge_spd = 10.0f;
    combat_start_range = 250.0f;
    attack_start_range = 300.0f;
    defense_recognize_range = 300.0f;
    invulnerable = false;
}

static void anm_init(e_sf_class* i_this, int i_index, f32 i_morf, u8 i_attr, f32 i_rate) {
    i_this->mpModelMorf->setAnm((J3DAnmTransform*)dComIfG_getObjectRes("E_sf", i_index), i_attr, i_morf, i_rate, 0.0f, -1.0f);
    i_this->mAnm = i_index;
}

static int nodeCallBack(J3DJoint* i_joint, int param_2) {
    if (param_2 == 0) {
        int jntNo = i_joint->getJntNo();
        J3DModel* model = j3dSys.getModel();
        e_sf_class* i_this = (e_sf_class*)model->getUserArea();

        if (i_this != NULL) {
            MTXCopy(model->getAnmMtx(jntNo), *calc_mtx);

            if (jntNo == JNT_HEAD) {
                cMtx_YrotM(*calc_mtx, i_this->mHeadAngleY + i_this->mHeadBobAngleY);
                cMtx_XrotM(*calc_mtx, i_this->mHeadAngleX);
                cMtx_ZrotM(*calc_mtx, -i_this->mHeadAngleZ);
            }

            model->setAnmMtx(jntNo, *calc_mtx);
            MTXCopy(*calc_mtx, J3DSys::mCurrentMtx);

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

static int daE_SF_Draw(e_sf_class* i_this) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)&i_this->actor;
    J3DModel* model = i_this->mpModelMorf->getModel();

    g_env_light.settingTevStruct(0, &a_this->current.pos, &a_this->tevStr);
    g_env_light.setLightTevColorType_MAJI(model, &a_this->tevStr);

    J3DModelData* modelData = model->getModelData();
    s16 color = i_this->mColor;
    for (u16 i = 0; i < modelData->getMaterialNum(); i++) {
        J3DMaterial* matNode_p = modelData->getMaterialNodePointer(i);
        if (i == 0) {
            matNode_p->getTevColor(2)->r = color;
            matNode_p->getTevColor(2)->g = color;
            matNode_p->getTevColor(2)->b = color;
        }
    }

    i_this->mpModelMorf->entryDL();

    if (i_this->mSwordModel != NULL) {
        g_env_light.setLightTevColorType_MAJI(i_this->mSwordModel, &a_this->tevStr);
        mDoExt_modelUpdateDL(i_this->mSwordModel);
    }

    if (i_this->arg3 != 0xFF) {
        g_env_light.setLightTevColorType_MAJI(i_this->mShieldModel, &a_this->tevStr);
        mDoExt_modelUpdateDL(i_this->mShieldModel);
    }

    cXyz pos;
    pos.set(a_this->current.pos.x, a_this->current.pos.y + 50.0f + BREG_F(18), a_this->current.pos.z);
    i_this->mShadowKey = dComIfGd_setShadow(i_this->mShadowKey, 1, model, &pos, BREG_F(19) + 900.0f, 0.0f,
                                            a_this->current.pos.y, i_this->mBgc.GetGroundH(), i_this->mBgc.m_gnd, &a_this->tevStr,
                                            0, 1.0f, dDlst_shadowControl_c::getSimpleTex());

    if (i_this->mSwordModel != NULL) {
        dComIfGd_addRealShadow(i_this->mShadowKey, i_this->mSwordModel);
    }

    if (i_this->arg3 != 0xFF) {
        dComIfGd_addRealShadow(i_this->mShadowKey, i_this->mShieldModel);
    }

    return 1;
}

static BOOL other_bg_check(e_sf_class* i_this, fopAc_ac_c* i_actor) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)&i_this->actor;
    dBgS_LinChk lin_chk;
    cXyz sp98, start, end;

    end = i_actor->current.pos;
    end.y += 100.0f;

    start = a_this->current.pos;
    start.y = a_this->eyePos.y;

    lin_chk.Set(&start, &end, a_this);
    if (dComIfG_Bgsp().LineCross(&lin_chk)) {
        return TRUE;
    }

    return FALSE;
}

static BOOL player_way_check(e_sf_class* i_this) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)&i_this->actor;
    fopAc_ac_c* player = (fopAc_ac_c*)dComIfGp_getPlayer(0);

    s16 angle_delta = a_this->shape_angle.y - player->shape_angle.y;
    if (angle_delta < 0) {
        angle_delta = -angle_delta;
    }

    if ((u32)(angle_delta & 0xFFFF) < 0x6000) {
        return FALSE;
    }

    return TRUE;
}

static BOOL way_bg_check(e_sf_class* i_this, f32 param_2, f32 param_3) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)&i_this->actor;
    dBgS_LinChk lin_chk;
    cXyz spa8, start, end;

    start = a_this->current.pos;
    start.y += param_3;
    cMtx_YrotS(*calc_mtx, a_this->shape_angle.y);
    spa8.x = 0.0f;
    spa8.y = param_3;
    spa8.z = param_2;
    MtxPosition(&spa8, &end);
    end += a_this->current.pos;

    lin_chk.Set(&start, &end, a_this);
    if (dComIfG_Bgsp().LineCross(&lin_chk)) {
        return TRUE;
    }

    return FALSE;
}

static int pl_check(e_sf_class* i_this, f32 i_distance, s16 param_3) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)&i_this->actor;
    fopAc_ac_c* player = (fopAc_ac_c*)dComIfGp_getPlayer(0);

    if (i_this->mPlayerDistanceXZ < i_distance) {
        s16 angle_delta = a_this->shape_angle.y - i_this->mPlayerAngleY;
        if (angle_delta < param_3 && angle_delta > (s16)-param_3 && !other_bg_check(i_this, player)) {
            return 1;
        }
    }

    for (int i = 0; i <= 2; i++) {
        if (i_this->mCcSphs[i].ChkCoHit()) {
            return 2;
        }
    }

    return 0;
}

static bool hio_set;

static daE_SF_HIO_c l_HIO;

static void e_sf_normal(e_sf_class* i_this) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)&i_this->actor;
    cXyz angle_delta, sp54;
    f32 target = 0.0f;
    s16 angle = 0x4000;
    s16 y_offset;

    switch (i_this->mActionPhase) {
        case PHASE_INIT:
        case NORMAL_PHASE_1:
        case NORMAL_PHASE_WALK:
            if (i_this->mTimers[0] == 0) {
                if (way_bg_check(i_this, 200.0f, 50.0f)) {
                    y_offset = cM_rndFX(10000.0f) + 32768.0f;
                    i_this->mTimers[1] = 40;
                } else {
                    sp54.x = a_this->home.pos.x + cM_rndFX(600.0f);
                    sp54.y = a_this->home.pos.y;
                    sp54.z = a_this->home.pos.z + cM_rndFX(600.0f);
                    angle_delta = sp54 - a_this->current.pos;

                    y_offset = cM_atan2s(angle_delta.x, angle_delta.z) - a_this->current.angle.y;
                    if (y_offset > 0x3000) {
                        y_offset = 0x3000;
                    } else if (y_offset < -0x3000) {
                        y_offset = -0x3000;
                    }
                }

                i_this->mAngleYOffset = a_this->current.angle.y + y_offset;
                anm_init(i_this, BCK_SF_WALK01, 10.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
                i_this->mActionPhase = NORMAL_PHASE_WAIT;
                i_this->mTimers[0] = cM_rndF(100.0f) + 100.0f;
            } else {
                angle = 0x7000;
            }
            break;
        
        case NORMAL_PHASE_WAIT:
            target = l_HIO.move_spd;
            cLib_addCalcAngleS2(&a_this->current.angle.y, i_this->mAngleYOffset, 8, 0x400);

            if (i_this->mTimers[0] == 0 || (i_this->mTimers[1] == 0 && way_bg_check(i_this, 200.0f, 50.0f))) {
                i_this->mActionPhase = NORMAL_PHASE_WALK;
                i_this->mTimers[0] = cM_rndF(100.0f) + 50.0f;

                if (i_this->mType == 0) {
                    anm_init(i_this, BCK_SF_WAIT01, 10.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
                } else {
                    anm_init(i_this, BCK_SF_WAIT02, 10.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
                }
            }
            break;
    }

    cLib_addCalc2(&a_this->speedF, target, 1.0f, 3.0f);

    BOOL isOto = fopAcM_otoCheck(a_this, 2000.0f);
    if (((i_this->mFrameCounter & 0xF) == 0 || isOto) && (isOto || pl_check(i_this, i_this->mRecognizeDist, angle) != 0)) {
        i_this->mAction = ACTION_FIGHT_RUN;
        i_this->mActionPhase = FIGHT_RUN_PHASE_NEG_10;
        i_this->mTimers[0] = 60;
    }
}

static void e_sf_drawback(e_sf_class* i_this) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)&i_this->actor;
    fopAc_ac_c* player = (fopAc_ac_c*)dComIfGp_getPlayer(0);

    switch (i_this->mActionPhase) {
        case PHASE_INIT:
            anm_init(i_this, BCK_SF_DRAWBACK, 5.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
            i_this->mActionPhase = DRAWBACK_PHASE_END;
            a_this->speedF = KREG_F(4) + -5.0f;
            i_this->mInvulnerabilityTimer = 10;
            i_this->mSound.startCreatureVoice(Z2SE_EN_SF_V_DRAWBACK, -1);
            i_this->mAngleYOffset = player->shape_angle.y + 0x8000;
            break;
        
        case DRAWBACK_PHASE_END:
            a_this->onHeadLockFlg();
            cLib_addCalcAngleS2(&a_this->current.angle.y, i_this->mAngleYOffset, 4, 0x400);
            cLib_addCalc0(&a_this->speedF, 1.0f, KREG_F(5) + 0.1f);

            if (i_this->mpModelMorf->isStop()) {
                i_this->mAction = ACTION_FIGHT_RUN;
                i_this->mActionPhase = FIGHT_RUN_PHASE_0;
            }
            break;
    }
}

static void e_sf_fight_run(e_sf_class* i_this) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)&i_this->actor;
    cXyz sp44, sp50;
    f32 target = 0.0f;
    int frame = i_this->mpModelMorf->getFrame();

    switch (i_this->mActionPhase) {
        case FIGHT_RUN_PHASE_NEG_10:
            if (i_this->mType == 0) {
                anm_init(i_this, BCK_SF_WAIT01, 10.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
            } else {
                anm_init(i_this, BCK_SF_WAIT02, 10.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
            }

            i_this->mTimers[1] = cM_rndF(10.0f) + 15.0f;
            i_this->mSound.startCreatureVoice(Z2SE_EN_SF_V_NOTICE, -1);
            i_this->mActionPhase = FIGHT_RUN_PHASE_NEG_9;
            break;

        case FIGHT_RUN_PHASE_NEG_9:
            if (i_this->mTimers[1] == 0) {
                i_this->mActionPhase = FIGHT_RUN_PHASE_0;
            }
            break;
        
        case FIGHT_RUN_PHASE_0:
            if (i_this->mType == 0) {
                anm_init(i_this, BCK_SF_WALK02, 5.0f, J3DFrameCtrl::EMode_LOOP, cM_rndFX(0.05f) + 1.0f);
            } else {
                anm_init(i_this, BCK_SF_WALK03, 5.0f, J3DFrameCtrl::EMode_LOOP, cM_rndFX(0.05f) + 1.0f);
            }

            i_this->mActionPhase = FIGHT_RUN_PHASE_1;
            // fallthrough
        case FIGHT_RUN_PHASE_1:
            target = l_HIO.charge_spd;

            if (i_this->mPlayerDistanceXZ < l_HIO.combat_start_range) {
                i_this->mActionPhase = FIGHT_RUN_PHASE_2;
            }
            break;

        case FIGHT_RUN_PHASE_2:
            target = l_HIO.move_spd;

            if (i_this->mPlayerDistanceXZ > l_HIO.combat_start_range + 50.0f) {
                i_this->mActionPhase = FIGHT_RUN_PHASE_0;
            } else if (i_this->mPlayerDistanceXZ < l_HIO.combat_start_range - 50.0f) {
                i_this->mActionPhase = FIGHT_RUN_PHASE_3;
                if (i_this->mType == 0) {
                    anm_init(i_this, BCK_SF_WALK02, 5.0f, J3DFrameCtrl::EMode_LOOP, -1.0f);
                } else {
                    anm_init(i_this, BCK_SF_WALK03, 5.0f, J3DFrameCtrl::EMode_LOOP, -1.0f);
                }
            }
            break;

        case FIGHT_RUN_PHASE_3:
            target = -l_HIO.move_spd;

            if (i_this->mPlayerDistanceXZ > l_HIO.combat_start_range) {
                i_this->mActionPhase = FIGHT_RUN_PHASE_2;
            }
            break;
    }

    cLib_addCalc2(&a_this->speedF, target, 1.0f, 3.0f);

    if (i_this->mActionPhase >= FIGHT_RUN_PHASE_0) {
        cLib_addCalcAngleS2(&a_this->current.angle.y, i_this->mPlayerAngleY, 4, 0x800);
    }

    if (pl_check(i_this, i_this->mRecognizeDist + 50.0f, 0x7FFF) == 0 && i_this->mTimers[0] == 0) {
        i_this->mAction = ACTION_NORMAL;
        i_this->mActionPhase = PHASE_INIT;
        i_this->mTimers[0] = cM_rndF(50.0f) + 50.0f;

        if (i_this->mType == 0) {
            anm_init(i_this, BCK_SF_WAIT01, 10.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
        } else {
            anm_init(i_this, BCK_SF_WAIT02, 10.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
        }
    } else {
        if (i_this->mPlayerAngleX < 0x1000 && i_this->mPlayerAngleX > -0x1000 && player_way_check(i_this) &&
            i_this->mPlayerDistanceXZ < l_HIO.attack_start_range && i_this->mTimers[2] == 0) {
            i_this->mTimers[2] = cM_rndF(20.0f) + 10.0f;

            if (cM_rndF(1.0f) < 0.5f) {
                if (i_this->mType == 0) {
                    i_this->mAction = ACTION_ATTACK;
                } else {
                    i_this->mAction = ACTION_ATTACK_0;
                }

                i_this->mActionPhase = PHASE_INIT;
            }
        }

        i_this->field_0x6ae = 1;
    }
}

static fopAc_ac_c* at_hit_check(e_sf_class* i_this) {
    dComIfGp_getPlayer(0);

    if (i_this->mActionPhase >= 10) {
        return NULL;
    }

    if (i_this->mAtSph.ChkAtHit()) {
        return dCc_GetAc(i_this->mAtSph.GetAtHitObj()->GetAc());
    }

    return NULL;
}

static f32 dummy() {
    return 40.0f;
}

static void e_sf_attack_0(e_sf_class* i_this) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)&i_this->actor;
    cXyz sp34, sp40;
    int frame = i_this->mpModelMorf->getFrame();

    i_this->field_0x6ae = 1;

    switch (i_this->mActionPhase) {
        case PHASE_INIT:
            anm_init(i_this, BCK_SF_ATTACK02, TREG_F(14) + 6.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
            i_this->mSound.startCreatureVoice(Z2SE_EN_SF_V_ATTACK, -1);
            i_this->mActionPhase = ATTACK_0_PHASE_END;
            // fallthrough
        case ATTACK_0_PHASE_END:
            if (frame >= 15 && frame <= 22) {
                i_this->mHitCheckFlag = 1;
            }

            if (frame == 15) {
                i_this->mSound.startCreatureSound(Z2SE_EN_SF_SWING_SWORD_S, 0, -1);
            }

            if (i_this->mpModelMorf->isStop()) {
                i_this->mAction = ACTION_FIGHT_RUN;
                i_this->mActionPhase = FIGHT_RUN_PHASE_0;
            }
            break;
    }

    cLib_addCalc0(&a_this->speedF, 1.0f, 5.0f);

    if (i_this->mHitCheckFlag) {
        fopAc_ac_c* actor_p = at_hit_check(i_this);
        if (actor_p != NULL && fopAcM_GetName(actor_p) == PROC_ALINK) {
            if (daPy_getPlayerActorClass()->checkPlayerGuard()) {
                i_this->mpModelMorf->setPlaySpeed(0.0f);
                i_this->mAction = ACTION_FIGHT_RUN;
                i_this->mActionPhase = FIGHT_RUN_PHASE_0;
                i_this->mTimers[2] = cM_rndF(20.0f) + 10.0f;
            }
        }
    }
}

static void e_sf_attack(e_sf_class* i_this) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)&i_this->actor;
    cXyz sp34, sp40;
    int frame = i_this->mpModelMorf->getFrame();

    i_this->field_0x6ae = 1;

    switch (i_this->mActionPhase) {
        case PHASE_INIT:
            anm_init(i_this, BCK_SF_ATTACK01, TREG_F(14) + 8.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
            i_this->mSound.startCreatureVoice(Z2SE_EN_SF_V_ATTACK, -1);
            i_this->mActionPhase = ATTACK_PHASE_END;
            // fallthrough
        case ATTACK_PHASE_END:
            if (frame < 58) {
                cLib_addCalcAngleS2(&a_this->current.angle.y, i_this->mPlayerAngleY, 2, 0x800);
            }

            if (frame == (int)(TREG_F(7) + 10.0f)) {
                anm_init(i_this, BCK_SF_ATTACK01, 5.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
                i_this->mpModelMorf->setFrame(55.0f);
            }

            if (frame >= 81 && frame <= 86) {
                i_this->mHitCheckFlag = 1;
            }

            if (frame == 78) {
                i_this->mSound.startCreatureSound(Z2SE_EN_SF_SWING_SWORD_L, 0, -1);
            }

            if (frame >= (int)(TREG_F(8) + 100.0f)) {
                i_this->mAction = ACTION_FIGHT_RUN;
                i_this->mActionPhase = FIGHT_RUN_PHASE_0;
            }
            break;
    }

    cLib_addCalc0(&a_this->speedF, 1.0f, 5.0f);

    if (i_this->mHitCheckFlag) {
        fopAc_ac_c* actor_p = at_hit_check(i_this);
        if (actor_p != NULL && fopAcM_GetName(actor_p) == PROC_ALINK) {
            if (daPy_getPlayerActorClass()->checkPlayerGuard()) {
                i_this->mpModelMorf->setPlaySpeed(0.0f);
                i_this->mAction = ACTION_FIGHT_RUN;
                i_this->mActionPhase = FIGHT_RUN_PHASE_0;
                i_this->mTimers[2] = cM_rndF(20.0f) + 10.0f;
            }
        }
    }
}

static int e_sf_guard(e_sf_class* i_this) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)&i_this->actor;

    i_this->field_0x6ab = 1;
    i_this->field_0x6ae = 1;

    int rv = 0;

    switch (i_this->mActionPhase) {
        case PHASE_INIT:
            anm_init(i_this, BCK_SF_GUARD, 2.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
            i_this->mSound.startCreatureVoice(Z2SE_EN_SF_V_GUARD, -1);
            i_this->mActionPhase = GUARD_PHASE_KNOCKBACK;
            break;

        case GUARD_PHASE_KNOCKBACK:
            i_this->mInvulnerabilityTimer = 5;

            if (i_this->mBgc.ChkGroundHit()) {
                cLib_addCalc0(&a_this->speedF, 1.0f, 10.0f);
            }

            if (i_this->mpModelMorf->getFrame() >= 8.0f) {
                i_this->mpModelMorf->setPlaySpeed(0.0f);
                i_this->mActionPhase = GUARD_PHASE_RECOVER;
                a_this->speedF = 0.0f;
                break;
            }

            cLib_addCalcAngleS2(&a_this->current.angle.y, i_this->mPlayerAngleY, 2, 0x800);
            break;
        
        case GUARD_PHASE_RECOVER:
            if (cc_pl_cut_bit_get() == 0x100 || cc_pl_cut_bit_get() == 0x80 || cc_pl_cut_bit_get() == 0x400) {
                i_this->mTimers[0] = KREG_S(0) + 20;
            } else {
                i_this->mTimers[0] = 0;
            }

            if (i_this->mTimers[0] != 0) {
                break;
            }

            i_this->mActionPhase = GUARD_PHASE_END;
            i_this->mpModelMorf->setPlaySpeed(1.0f);
            // fallthrough
        case GUARD_PHASE_END:
            if (i_this->mpModelMorf->isStop()) {
                i_this->mAction = ACTION_FIGHT_RUN;
                i_this->mActionPhase = FIGHT_RUN_PHASE_0;
            }

            rv = 1;
    }

    return rv;
}

static void e_sf_s_damage(e_sf_class* i_this) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)&i_this->actor;

    switch (i_this->mActionPhase) {
        case PHASE_INIT:
            i_this->mActionPhase = S_DAMAGE_PHASE_END;
            anm_init(i_this, BCK_SF_DAMAGE, 5.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
            i_this->mSound.startCreatureVoice(Z2SE_EN_SF_V_DAMAGE, -1);
            i_this->mSound.startCreatureSound(Z2SE_EN_SF_SHAKE_BONES, 0, -1);
            break;
        
        case S_DAMAGE_PHASE_END:
            if (i_this->mpModelMorf->isStop()) {
                if (i_this->mTimers[1] != 0) {
                    i_this->mAction = ACTION_GUARD;
                    i_this->mActionPhase = PHASE_INIT;
                    i_this->mTimers[0] = 0;
                    i_this->mInvulnerabilityTimer = 15;
                } else {
                    i_this->mAction = ACTION_FIGHT_RUN;
                    i_this->mActionPhase = FIGHT_RUN_PHASE_0;
                    i_this->mTimers[0] = 30;
                }
            }
            break;
    }

    cLib_addCalc0(&a_this->speedF, 1.0f, 3.0f);
}

static fopAc_ac_c* target_info[10];

static int target_info_count;

static int ret_ct;

static void e_sf_crash(e_sf_class* i_this) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)&i_this->actor;

    for (int i = 0; i < 3; i++) {
        i_this->mCcSphs[i].SetTgType(0x400020);
    }

    i_this->mStts.Init(0xFF, 0, a_this);

    switch (i_this->mActionPhase) {
        case PHASE_INIT: {
            i_this->mActionPhase = CRASH_PHASE_WAIT;
            anm_init(i_this, BCK_SF_DIE, 3.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
            a_this->speedF = 0.0f;
            fopAcM_OffStatus(a_this, 0);
            a_this->attention_info.flags = 0;
            cXyz scale(l_HIO.basic_size, l_HIO.basic_size, l_HIO.basic_size);
            dComIfGp_particle_set(dPa_RM(ID_ZI_S_SF_BARASMOKE_A), &a_this->current.pos, &a_this->shape_angle, &scale);
            
            if (dComIfG_Bgsp().GetPolyAtt0(i_this->mBgc.m_gnd) == 4) {
                dComIfGp_particle_set(dPa_RM(ID_ZI_S_SF_BARAKUSA_A), &a_this->current.pos, &a_this->shape_angle, &scale);
            }

            i_this->mSound.startCreatureVoice(Z2SE_EN_SF_V_FALLDOWN, -1);
            i_this->mSound.startCreatureSound(Z2SE_EN_SF_BREAK_DOWN, 0, -1);
            break;
        }
        
        case CRASH_PHASE_WAIT:
            if (i_this->mpModelMorf->isStop()) {
                i_this->mTimers[0] = cM_rndF(50.0f) + 300.0f;
                i_this->mActionPhase = CRASH_PHASE_END;
            }
            break;

        case CRASH_PHASE_END:
            if (i_this->mTimers[0] == 40 && i_this->field_0xffc == 0) {
                if (strcmp(dComIfGp_getStartStageName(), "D_MN10") == 0 && fopAcM_GetRoomNo(a_this) == 7) {
                    i_this->mDemoMode = 10;
                    i_this->field_0xffc = 1;
                }
            }

            if (i_this->mTimers[0] == 0) {
                i_this->mAction = ACTION_GETUP;
                i_this->mActionPhase = PHASE_INIT;
                
                ret_ct++;
                if (ret_ct >= 3) {
                    dComIfGs_onOneZoneSwitch(5, -1);
                }

                for (int i = 0; i < 3; i++) {
                    i_this->mCcSphs[i].SetTgType(0xD8FBFDFF);
                }

                i_this->mStts.Init(200, 0, a_this);
                i_this->mInvulnerabilityTimer = 5;
            }
            break;
    }
}

static void e_sf_crashwait(e_sf_class* i_this) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)&i_this->actor;

    i_this->mInvulnerabilityTimer = 5;

    switch (i_this->mActionPhase) {
        case PHASE_INIT:
            i_this->mActionPhase = CRASHWAIT_PHASE_END;
            anm_init(i_this, BCK_SF_DIE, 3.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
            i_this->mpModelMorf->setFrame(39.0f);
            a_this->speedF = 0.0f;
            fopAcM_OffStatus(a_this, 0);
            a_this->attention_info.flags = 0;
            // fallthrough
        case CRASHWAIT_PHASE_END:
            if (i_this->mPlayerDistanceXZ < i_this->arg1 * 100.0f) {
                i_this->mAction = ACTION_GETUP;
                i_this->mActionPhase = PHASE_INIT;
            }
            break;
    }
}

static s16 e_sf_sitwait(e_sf_class* i_this) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)&i_this->actor;
    dComIfGp_getPlayer(0);

    i_this->mInvulnerabilityTimer = 5;

    s16 rv = 0;

    switch (i_this->mActionPhase) {
        case PHASE_INIT:
            i_this->mActionPhase = SITWAIT_PHASE_1;
            anm_init(i_this, BCK_SF_OPDEMO, 0.0f, J3DFrameCtrl::EMode_NONE, 0.0f);
            fopAcM_OffStatus(a_this, 0);
            a_this->attention_info.flags = 0;
            // fallthrough
        case SITWAIT_PHASE_1:
            if (dComIfGs_isOneZoneSwitch(0, -1)) {
                i_this->mActionPhase = SITWAIT_PHASE_END;
                i_this->mpModelMorf->setPlaySpeed(1.0f);
                i_this->mDemoMode = 1;
            }
            break;
        
        case SITWAIT_PHASE_END:
            rv = 0xFF;
            i_this->mSound.startCreatureVoiceLevel(Z2SE_EN_SF_V_FIRST_DEMO, -1);

            if (dComIfGp_getEvent()->checkSkipEdge() || i_this->mpModelMorf->isStop()) {
                i_this->mAction = ACTION_FIGHT_RUN;
                i_this->mActionPhase = FIGHT_RUN_PHASE_NEG_10;
                i_this->mTimers[0] = 60;
                fopAcM_OnStatus(a_this, 0);
                a_this->attention_info.flags = fopAc_AttnFlag_BATTLE_e;
                i_this->mDemoMode = 100;
            }
    }

    return rv;
}

static s16 e_sf_getup(e_sf_class* i_this) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)&i_this->actor;

    i_this->mInvulnerabilityTimer = 5;

    s16 rv = 0;

    switch (i_this->mActionPhase) {
        case PHASE_INIT: {
            i_this->mTimers[0] = 40;
            csXyz rotation(a_this->shape_angle);
            cXyz scale(l_HIO.basic_size, l_HIO.basic_size, l_HIO.basic_size);

            for (int i = 0; i <= 2; i++) {
                dComIfGp_particle_set(dPa_RM(ID_ZI_S_SF_GETUP_A), &a_this->current.pos, &rotation, &scale);
                rotation.y += 0x5555;
            }

            i_this->mActionPhase = GETUP_PHASE_GETUP;
            i_this->mSound.startCreatureVoice(Z2SE_EN_SF_V_RESURRECTION, -1);
            break;
        }

        case GETUP_PHASE_GETUP:
            if (i_this->mTimers[0] == 0) {
                i_this->mActionPhase = GETUP_PHASE_WAIT;
                anm_init(i_this, BCK_SF_GETUP, 5.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
                i_this->mSound.startCreatureSound(Z2SE_EN_SF_BUILD_UP, 0, -1);
            }
            break;
        
        case GETUP_PHASE_WAIT:
            if (i_this->mpModelMorf->isStop()) {
                if (i_this->mType == 0) {
                    anm_init(i_this, BCK_SF_WAIT01, 10.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
                } else {
                    anm_init(i_this, BCK_SF_WAIT02, 10.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
                }

                i_this->mTimers[0] = 40;
                i_this->mActionPhase = GETUP_PHASE_END;
            }
            break;

        case GETUP_PHASE_END:
            rv = 0xFF;

            if (i_this->mTimers[0] == 0) {
                i_this->mAction = ACTION_FIGHT_RUN;
                i_this->mActionPhase = FIGHT_RUN_PHASE_NEG_10;
                i_this->mTimers[0] = 60;
                a_this->health = 200;
                fopAcM_OnStatus(a_this, 0);
                a_this->attention_info.flags = fopAc_AttnFlag_BATTLE_e;
            }
            break;
    }

    return rv;
}

static void crash_eff(e_sf_class* i_this) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)&i_this->actor;
    cXyz scale(1.5f, 1.5f, 1.5f);

    static u16 ap_name[3] = {
        dPa_RM(ID_ZI_S_BS_KONAGONA_A),
        dPa_RM(ID_ZI_S_BS_KONAGONA_B),
        dPa_RM(ID_ZI_S_BS_KONAGONA_C),
    };

    for (int i = 0; i <= 2; i++) {
        dComIfGp_particle_set(ap_name[i], &a_this->current.pos, &a_this->shape_angle, &scale);
    }
}

static void damage_check(e_sf_class* i_this) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)&i_this->actor;
    daPy_py_c* player = (daPy_py_c*)dComIfGp_getPlayer(0);

    i_this->mStts.Move();

    if (i_this->mInvulnerabilityTimer != 0) {
        return;
    }

    if (l_HIO.invulnerable) {
        a_this->health = 1000;
    }

    for (int i = 0; i <= 2; i++) {
        if (i_this->mCcSphs[i].ChkTgHit()) {
            i_this->mAtInfo.mpCollider = i_this->mCcSphs[i].GetTgHitObj();
            cc_at_check(a_this, &i_this->mAtInfo);

            if (i_this->mAtInfo.mHitType == 0x10) {
                if (i_this->mAtInfo.mpCollider->ChkAtType(AT_TYPE_BOOMERANG)) {
                    i_this->mAction = ACTION_S_DAMAGE;
                    i_this->mActionPhase = PHASE_INIT;
                    i_this->field_0x6c4 = 0.0f;
                } else {
                    i_this->mAction = ACTION_DRAWBACK;
                    i_this->mActionPhase = PHASE_INIT;
                    i_this->mInvulnerabilityTimer = 6;
                }
            } else {
                s8 unkFlag1 = false;
                s8 unkFlag2 = false;

                if (i_this->field_0x6ad != 0) {
                    i_this->field_0x6ad = 0;
                    unkFlag2 = true;
                }

                if (i_this->mAtInfo.mpCollider->ChkAtType(AT_TYPE_BOMB) || (daPy_getPlayerActorClass()->getCutType() == daPy_py_c::CUT_TYPE_HEAD_JUMP || i_this->mAtInfo.mpCollider->ChkAtType(AT_TYPE_IRON_BALL))) {
                    if (i_this->mAction == ACTION_CRASH || i_this->mAtInfo.mpCollider->ChkAtType(AT_TYPE_BOMB)) {
                        fopAcM_createDisappear(a_this, &a_this->current.pos, 15, 0, 0x29);
                        fopAcM_delete(a_this);

                        if (i_this->mSwBit != 0xFF) {
                            dComIfGs_onSwitch(i_this->mSwBit, fopAcM_GetRoomNo(a_this));
                        }
                        return;
                    }

                    crash_eff(i_this);
                    unkFlag1 = true;
                }

                if (i_this->mAtInfo.mpCollider->ChkAtType(AT_TYPE_UNK)) {
                    i_this->mInvulnerabilityTimer = 20;
                } else if (player->getCutType() == daPy_py_c::CUT_TYPE_JUMP && player->checkCutJumpCancelTurn()) {
                    i_this->mInvulnerabilityTimer = 3;
                    i_this->field_0x6ad = 1;
                } else {
                    i_this->mInvulnerabilityTimer = 6;
                }

                if (a_this->health <= 0 || unkFlag1 || unkFlag2) {
                    i_this->mAction = ACTION_CRASH;
                    i_this->mActionPhase = PHASE_INIT;
                    a_this->health = 0;
                } else {
                    i_this->mAction = ACTION_S_DAMAGE;
                    i_this->mActionPhase = PHASE_INIT;
                    i_this->field_0x6c4 = TREG_F(0) + 25.0f;
                    i_this->mHitDirectionY = i_this->mAtInfo.mHitDirection.y;

                    if (i_this->arg3 != 0xFF) {
                        if (daPy_getPlayerActorClass()->getCutCount() >= 4) {
                            i_this->mInvulnerabilityTimer = 40;
                            i_this->mTimers[1] = 40;
                        }
                    }
                }

                if (i_this->mAtInfo.mAttackPower <= 1) {
                    i_this->mInvulnerabilityTimer = KREG_S(8) + 10;
                }

                a_this->speedF = 0.0f;
            }
            break;
        }
    }

    for (int i = 0; i <= 2; i++) {
        if (a_this->health <= 1) {
            a_this->health = 0;
            i_this->mCcSphs[i].SetTgHitMark(CcG_Tg_UNK_MARK_3);
        }
    }
}

static void action(e_sf_class* i_this) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)&i_this->actor;
    fopAc_ac_c* player = (fopAc_ac_c*)dComIfGp_getPlayer(0);
    cXyz spcc, spd8;

    i_this->field_0x6ae = 0;
    i_this->mPlayerDistanceXZ = fopAcM_searchPlayerDistanceXZ(a_this);
    i_this->mPlayerAngleY = fopAcM_searchPlayerAngleY(a_this);
    i_this->mPlayerAngleX = fopAcM_searchPlayerAngleX(a_this);
    i_this->mRecognizeDist = l_HIO.p_recognize_dist_m;
    damage_check(i_this);
    s8 unkFlag1 = 0;
    s8 isLinkSearch = 0;
    s16 colorTarget = 0xFF;
    a_this->offHeadLockFlg();

    switch (i_this->mAction) {
        case ACTION_NORMAL:
            e_sf_normal(i_this);
            break;

        case ACTION_FIGHT_RUN:
            e_sf_fight_run(i_this);
            unkFlag1 = 1;
            isLinkSearch = 1;
            break;

        case ACTION_ATTACK_0:
            e_sf_attack_0(i_this);
            unkFlag1 = 1;
            isLinkSearch = 1;
            break;

        case ACTION_ATTACK:
            e_sf_attack(i_this);
            unkFlag1 = 1;
            isLinkSearch = 1;
            break;

        case ACTION_GUARD:
            if (e_sf_guard(i_this) != 0) {
                unkFlag1 = 1;
            }
            
            isLinkSearch = 1;
            break;

        case ACTION_DRAWBACK:
            e_sf_drawback(i_this);
            isLinkSearch = 1;
            break;

        case ACTION_S_DAMAGE:
            e_sf_s_damage(i_this);
            isLinkSearch = 1;
            break;

        case ACTION_CRASH:
            e_sf_crash(i_this);
            colorTarget = 0;
            i_this->field_0x6ae = -1;
            break;

        case ACTION_GETUP:
            colorTarget = e_sf_getup(i_this);
            i_this->field_0x6ae = -1;
            break;

        case ACTION_CRASHWAIT:
            e_sf_crashwait(i_this);
            colorTarget = 0;
            i_this->field_0x6ae = -1;
            break;

        case ACTION_SITWAIT:
            colorTarget = e_sf_sitwait(i_this);
            i_this->field_0x6ae = -1;
            break;
    }

    if (isLinkSearch) {
        i_this->mSound.setLinkSearch(true);
    } else {
        i_this->mSound.setLinkSearch(false);
    }

    cLib_addCalcAngleS2(&i_this->mColor, colorTarget, 1, 4);

    if (i_this->arg3 != 0xFF && unkFlag1 && i_this->mPlayerAngleX < 0x1000 && i_this->mPlayerAngleX > -0x1000 && player_way_check(i_this)) {
        u32 uVar1;
        if (i_this->arg3 == 0 && cM_rndF(1.0f) < 0.2f) {
            uVar1 = 0x3FF;
        } else {
            uVar1 = 0x3FF;
        }

        if (i_this->mPlayerDistanceXZ < l_HIO.defense_recognize_range) {
            int cutBit = (cc_pl_cut_bit_get() & uVar1);
            if (cutBit != 0) {
                i_this->mAction = ACTION_GUARD;
                i_this->mActionPhase = PHASE_INIT;
                i_this->mTimers[0] = 0;
                i_this->mInvulnerabilityTimer = 15;
            }
        }
    }

    cLib_addCalcAngleS2(&a_this->shape_angle.y, a_this->current.angle.y, 2, 0x2000);
    cLib_addCalcAngleS2(&a_this->shape_angle.x, a_this->current.angle.x, 2, 0x2000);
    cLib_addCalcAngleS2(&a_this->shape_angle.z, a_this->current.angle.z, 2, 0x2000);

    if (i_this->field_0x6c0) {
        cMtx_YrotS(*calc_mtx, i_this->field_0x6e4.y);

        if (i_this->mUnkTimer1 == 0) {
            spcc.z = i_this->field_0x6c0;
        } else {
            spcc.z = 0.0f;
            a_this->speed.y = 0.0f;
        }

        a_this->gravity = -4.0f;
    } else {
        cMtx_YrotS(*calc_mtx, a_this->current.angle.y);
        spcc.z = a_this->speedF;
    }
    spcc.x = 0.0f;
    spcc.y = 0.0f;
    MtxPosition(&spcc, &spd8);
    a_this->speed.x = spd8.x;
    a_this->speed.z = spd8.z;
    a_this->current.pos += a_this->speed * l_HIO.basic_size;
    a_this->speed.y += a_this->gravity;
    a_this->gravity = -5.0f;

    if (a_this->speed.y < -120.0f) {
        a_this->speed.y = -120.0f;
    }

    cXyz* ccMoveP = i_this->mStts.GetCCMoveP();
    if (ccMoveP != NULL) {
        a_this->current.pos.x += ccMoveP->x;
        a_this->current.pos.y += ccMoveP->y;
        a_this->current.pos.z += ccMoveP->z;
    }

    if (i_this->field_0x6c4 > 0.1f) {
        spcc.x = 0.0f;
        spcc.y = 0.0f;
        spcc.z = -i_this->field_0x6c4;
        cMtx_YrotS(*calc_mtx, i_this->mHitDirectionY);
        MtxPosition(&spcc, &spd8);
        a_this->current.pos += spd8;
        cLib_addCalc0(&i_this->field_0x6c4, 1.0f, TREG_F(12) + 7.0f);
    }
    
    i_this->mBgc.CrrPos(dComIfG_Bgsp());

    if (i_this->field_0x6ae > 0) {
        if (i_this->field_0x6ae == 5) {
            if ((i_this->mFrameCounter & 0xF) == 0 && cM_rndF(1.0f) < 0.3f) {
                i_this->mTargetHeadBobAngleY = cM_rndFX(2500.0f);
            }
        } else {
            if (i_this->field_0x6ae == 1) {
                spcc = player->eyePos - a_this->current.pos;
            } else if (i_this->field_0x6ae == 2) {
                spcc = i_this->field_0x6f8 - a_this->current.pos;
            } else {
                spcc = player->eyePos - a_this->current.pos;
            }
            spcc.y += -(TREG_F(2) + 150.0f) * l_HIO.basic_size;
            s16 targetAngleY = cM_atan2s(spcc.x, spcc.z) - a_this->shape_angle.y;
            s16 targetAngleZ = a_this->shape_angle.x + cM_atan2s(spcc.y, JMAFastSqrt(spcc.x * spcc.x + spcc.z * spcc.z));

            if (targetAngleY > 10000) {
                targetAngleY = 10000;
            } else if (targetAngleY < -10000) {
                targetAngleY = -10000;
            }

            if (targetAngleZ > 10000) {
                targetAngleZ = 10000;
            } else if (targetAngleZ < -10000) {
                targetAngleZ = -10000;
            }

            cLib_addCalcAngleS2(&i_this->mHeadAngleY, (s16)targetAngleY, 2, 0x1000);
            cLib_addCalcAngleS2(&i_this->mHeadAngleZ, (s16)targetAngleZ, 2, 0x1000);
        }

        i_this->mTargetHeadBobAngleY = 0;
    } else {
        cLib_addCalcAngleS2(&i_this->mHeadAngleY, 0, 2, 0x1000);
        cLib_addCalcAngleS2(&i_this->mHeadAngleZ, 0, 2, 0x1000);
        i_this->mTargetHeadBobAngleY = 0;
    }

    cLib_addCalcAngleS2(&i_this->mHeadBobAngleY, i_this->mTargetHeadBobAngleY, 4, 0x400);

    if (i_this->field_0x6ae >= 0) {
        if ((i_this->mFrameCounter & 0x1F) == 0 && cM_rndF(1.0f) < 0.5f) {
            i_this->mTargetHeadAngleX = cM_rndFX(4000.0f);
        }
    } else {
        i_this->mTargetHeadAngleX = 0;
    }

    cLib_addCalcAngleS2(&i_this->mHeadAngleX, i_this->mTargetHeadAngleX, 4, 0x400);

    if (i_this->field_0x6bc != 0) {
        i_this->field_0x6bc--;
        i_this->mHeadBobAngleY = (BREG_F(18) + 200.0f) * (i_this->field_0x6bc * cM_ssin(i_this->field_0x6bc * (BREG_S(5) + 12000)));
    }

    if (i_this->field_0x6f6) {
        i_this->field_0x6f6--;
        if (i_this->field_0x6e4.x != 0) {
            Vec spe4;
            cXyz pos, spfc;
            dBgS_GndChk gnd_chk;
            s16 sVar6 = 0;
            s16 sVar7 = 0;
            f32 fVar1 = 100.0f;

            MTXCopy(i_this->mpModelMorf->getModel()->getAnmMtx(JNT_BACKBONE2), *calc_mtx);
            spcc.set(0.0f, 0.0f, 0.0f);
            MtxPosition(&spcc, &pos);
            pos.y += 100.0f;
            pos.y += 100.0f;
            gnd_chk.SetPos(&pos);
            pos.y = dComIfG_Bgsp().GroundCross(&gnd_chk);

            if (pos.y != -1e9f) {
                spe4.x = pos.x;
                spe4.y = pos.y + 100.0f;
                spe4.z = pos.z + fVar1;
                gnd_chk.SetPos(&spe4);
                spe4.y = dComIfG_Bgsp().GroundCross(&gnd_chk);

                if (spe4.y != -1e9f) {
                    sVar7 = -cM_atan2s(spe4.y - pos.y, spe4.z - pos.z);
                    if (sVar7 > 0x3000 || sVar7 < -0x3000) {
                        sVar7 = 0;
                    }
                }

                spe4.x = pos.x + fVar1;
                spe4.y = pos.y + 100.0f;
                spe4.z = pos.z;
                gnd_chk.SetPos(&spe4);
                spe4.y = dComIfG_Bgsp().GroundCross(&gnd_chk);

                if (spe4.y != -1e9f) {
                    sVar6 = (s16)cM_atan2s(spe4.y - pos.y, spe4.x - pos.x);
                    if (sVar6 > 0x3000 || sVar6 < -0x3000) {
                        sVar6 = 0;
                    }
                }
            }

            i_this->field_0x6f0.x = sVar7;
            i_this->field_0x6f0.z = sVar6;
        }
    }

    cLib_addCalcAngleS2(&i_this->field_0x6ea.x, i_this->field_0x6f0.x, 1, 0x400);
    cLib_addCalcAngleS2(&i_this->field_0x6ea.z, i_this->field_0x6f0.z, 1, 0x400);

    if (i_this->field_0xfb8 != 0) {
        J3DModel* model = i_this->mpModelMorf->getModel();
        spcc.set(0.0f, 0.0f, 0.0f);

        static int foot_idx[2] = {
            JNT_FOOTL2, JNT_FOOTR2,
        };

        for (int i = 0; i < 2; i++) {
            if (i_this->field_0xfb8 == 3 || (i_this->field_0xfb8 == 2 && i == 1) || (i_this->field_0xfb8 == 1 && i == 0)) {
                MTXCopy(model->getAnmMtx(foot_idx[i]), *calc_mtx);
                MtxPosition(&spcc, &spd8);
                fopAcM_effSmokeSet2(&i_this->field_0xfbc[i], &i_this->field_0xfc4[i], &spd8, &a_this->shape_angle, TREG_F(8) + 1.0f, &a_this->tevStr);
            }
        }

        i_this->field_0xfb8 = 0;
    }

    cXyz i_effSize(1.5f, 1.5f, 1.5f);
    setMidnaBindEffect(a_this, &i_this->mSound, &a_this->eyePos, &i_effSize);
}

static void anm_se_set(e_sf_class* i_this) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)&i_this->actor;
    s8 play_footnote = 0;

    if (i_this->mAnm == BCK_SF_ATTACK01) {
        if (i_this->mpModelMorf->checkFrame(83.0f)) {
            play_footnote = 1;
        }
    } else if (i_this->mAnm == BCK_SF_DAMAGE) {
        if (i_this->mpModelMorf->checkFrame(3.5f)) {
            play_footnote = 1;
        }
    } else if (i_this->mAnm == BCK_SF_DRAWBACK) {
        if (i_this->mpModelMorf->checkFrame(4.0f) || i_this->mpModelMorf->checkFrame(13.0f) || i_this->mpModelMorf->checkFrame(24.0f) ||
            i_this->mpModelMorf->checkFrame(38.0f) || i_this->mpModelMorf->checkFrame(57.0f)) {
            play_footnote = 1;
        }
    } else if (i_this->mAnm == BCK_SF_WALK01) {
        if (i_this->mpModelMorf->checkFrame(0.5f) || i_this->mpModelMorf->checkFrame(22.0f)) {
            play_footnote = 1;
        }
    } else if (i_this->mAnm == BCK_SF_WALK02) {
        if (i_this->mpModelMorf->checkFrame(11.0f) || i_this->mpModelMorf->checkFrame(22.0f)) {
            play_footnote = 1;
        }
    } else if (i_this->mAnm == BCK_SF_WALK03 && (i_this->mpModelMorf->checkFrame(8.0f) || i_this->mpModelMorf->checkFrame(18.0f))) {
        play_footnote = 1;
    }

    if (play_footnote) {
        i_this->mSound.startCreatureSound(Z2SE_STALFOS_FOOTNOTE, 0, -1);
    }
}

static void demo_camera(e_sf_class* i_this) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)&i_this->actor;
    fopAc_ac_c* player = (fopAc_ac_c*)dComIfGp_getPlayer(0);
    camera_class* camera = dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0));
    dComIfGp_getCamera(0);
    cXyz sp30, sp3c, sp48, sp54;
    s8 sVar1 = 0;

    switch (i_this->mDemoMode) {
        case 0:
            break;
        
        case 1:
            if (!a_this->eventInfo.checkCommandDemoAccrpt()) {
                fopAcM_orderPotentialEvent(a_this, 2, 0xFFFF, 0);
                a_this->eventInfo.onCondition(dEvtCnd_CANDEMO_e);
                return;
            }

            camera->mCamera.Stop();
            i_this->mDemoMode = 2;
            i_this->field_0xfda = 0;
            i_this->mDemoCamFovy = 75.0f;
            camera->mCamera.SetTrimSize(3);
            dComIfGp_getEvent()->startCheckSkipEdge(a_this);
            daPy_getPlayerActorClass()->changeOriginalDemo();
            daPy_getPlayerActorClass()->changeDemoMode(0x17, 1, 2, 0);
            // fallthrough
        case 2:
            cMtx_YrotS(*calc_mtx, a_this->shape_angle.y);
            sp30.x = NREG_F(0);
            sp30.y = NREG_F(1) + 50.0f;
            sp30.z = NREG_F(2) + 400.0f;
            MtxPosition(&sp30, &i_this->mDemoCamEye);
            i_this->mDemoCamEye += a_this->current.pos;
            sp54.x = a_this->current.pos.x;
            sp54.z = a_this->current.pos.z;
            sp54.y = a_this->eyePos.y - 20.0f + NREG_F(3);

            if (sp54.y < a_this->current.pos.y + NREG_F(4) + 90.0f) {
                sp54.y = a_this->current.pos.y + NREG_F(4) + 90.0f;
            }

            if (i_this->field_0xfda == 0) {
                i_this->mDemoCamCenter = sp54;
            } else {
                cLib_addCalc2(&i_this->mDemoCamCenter.y, sp54.y, 0.1f, 20.0f);
                cLib_addCalc2(&i_this->mDemoCamFovy, 55.0f, 0.05f, 0.25f);
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
            i_this->field_0xfda = 0;
            i_this->mDemoCamFovy = 75.0f;
            camera->mCamera.SetTrimSize(3);
            daPy_getPlayerActorClass()->changeOriginalDemo();
            daPy_getPlayerActorClass()->changeDemoMode(0x17, 1, 2, 0);
            // fallthrough
        case 11: {
            sp30.x = -8918.0f - a_this->current.pos.x;
            sp30.z = -462.0f - a_this->current.pos.z;
            s16 angle = cM_atan2s(sp30.x, sp30.z);
            a_this->current.angle.y = angle;
            a_this->shape_angle.y = angle;
            cMtx_YrotS(*calc_mtx, a_this->shape_angle.y);
            sp30.x = NREG_F(0);
            sp30.y = NREG_F(1) + 50.0f;
            sp30.z = NREG_F(2) + 400.0f;
            MtxPosition(&sp30, &i_this->mDemoCamEye);
            i_this->mDemoCamEye += a_this->current.pos;
            sp54.x = a_this->current.pos.x;
            sp54.z = a_this->current.pos.z;
            sp54.y = a_this->eyePos.y - 20.0f + NREG_F(3);

            if (sp54.y < a_this->current.pos.y + NREG_F(4) + 90.0f) {
                sp54.y = a_this->current.pos.y + NREG_F(4) + 90.0f;
            }

            if (i_this->field_0xfda == 0) {
                i_this->mDemoCamCenter = sp54;
                sp30.x = 0.0f;
                sp30.y = 0.0f;
                sp30.z = 700.0f;
                MtxPosition(&sp30, &sp3c);
                sp3c += a_this->current.pos;
                daPy_getPlayerActorClass()->setPlayerPosAndAngle(&sp3c, a_this->shape_angle.y + 0x8000, 0);
            } else {
                cLib_addCalc2(&i_this->mDemoCamCenter.y, sp54.y, 0.1f, 20.0f);
                cLib_addCalc2(&i_this->mDemoCamFovy, 55.0f, 0.05f, 0.2f);
            }

            if (i_this->field_0xfda == 220) {
                i_this->mDemoMode = 100;
            }
            break;
        }

        case 100:
            sVar1 = 1;
            cMtx_YrotS(*calc_mtx, player->shape_angle.y);
            sp30.x = 0.0f;
            sp30.y = JREG_F(1) + 100.0f;
            sp30.z = JREG_F(2) + -250.0f;
            MtxPosition(&sp30, &i_this->mDemoCamEye);
            i_this->mDemoCamEye += player->current.pos;
            i_this->mDemoCamCenter = a_this->eyePos;
            break;
    }

    if (sVar1) {
        camera->mCamera.Reset(i_this->mDemoCamCenter, i_this->mDemoCamEye);
        camera->mCamera.Start();
        camera->mCamera.SetTrimSize(0);
        dComIfGp_event_reset();
        daPy_getPlayerActorClass()->cancelOriginalDemo();
        i_this->mDemoMode = 0;
    }

    if (i_this->mDemoMode > 0) {
        cXyz center = i_this->mDemoCamCenter;
        cXyz eye = i_this->mDemoCamEye;
        camera->mCamera.Set(center, eye, i_this->mDemoCamFovy, 0);
        i_this->field_0xfda++;
    }
}

static int daE_SF_Execute(e_sf_class* i_this) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)&i_this->actor;
    cXyz sp2c, sp38, sp44;

    i_this->mFrameCounter++;

    for (int i = 0; i < 4; i++) {
        if (i_this->mTimers[i] != 0) {
            i_this->mTimers[i]--;
        }
    }

    if (i_this->mInvulnerabilityTimer != 0) {
        i_this->mInvulnerabilityTimer--;
    }

    if (i_this->mUnkTimer1 != 0) {
        i_this->mUnkTimer1--;
    }

    if (i_this->mUnkTimer2 != 0) {
        i_this->mUnkTimer2--;
    }

    i_this->mCoSetBitFlag = 1;
    action(i_this);
    mDoMtx_stack_c::transS(a_this->current.pos.x, a_this->current.pos.y, a_this->current.pos.z);
    mDoMtx_stack_c::XrotM(i_this->field_0x6ea.x);
    mDoMtx_stack_c::ZrotM(i_this->field_0x6ea.z);
    mDoMtx_stack_c::YrotM(i_this->field_0x6e4.y);
    mDoMtx_stack_c::XrotM(i_this->field_0x6e4.x);
    mDoMtx_stack_c::YrotM(-i_this->field_0x6e4.y);
    mDoMtx_stack_c::YrotM(a_this->shape_angle.y);
    mDoMtx_stack_c::XrotM(a_this->shape_angle.x);
    mDoMtx_stack_c::ZrotM(a_this->shape_angle.z);
    mDoMtx_stack_c::scaleM(l_HIO.basic_size * a_this->scale.x, l_HIO.basic_size * a_this->scale.x, l_HIO.basic_size * a_this->scale.x);

    J3DModel* model = i_this->mpModelMorf->getModel();
    model->setBaseTRMtx(mDoMtx_stack_c::get());
    i_this->mpModelMorf->play(0, dComIfGp_getReverb(fopAcM_GetRoomNo(a_this)));
    anm_se_set(i_this);
    i_this->mpModelMorf->modelCalc();
    cXyz sp50(0.0f, 0.0f, 0.0f);
    cXyz sp5c(-200000.0f, -200000.0f, -150000.0f);

    if (i_this->mInvulnerabilityTimer != 0) {
        sp50 += sp5c;
    }

    MTXCopy(model->getAnmMtx(JNT_HEAD), *calc_mtx);
    sp2c.set(KREG_F(12) + -20.0f, KREG_F(13) + -30.0f, KREG_F(14));
    MtxPosition(&sp2c, &a_this->eyePos);
    i_this->mCcSphs[0].SetC(a_this->eyePos + sp50);
    i_this->mCcSphs[0].SetR(l_HIO.basic_size * 30.0f);
    a_this->attention_info.position = a_this->eyePos;
    a_this->attention_info.position.y += BREG_F(7) + 80.0f;

    MTXCopy(model->getAnmMtx(JNT_BACKBONE2), *calc_mtx);
    sp2c.set(BREG_F(11), BREG_F(12), BREG_F(13));
    MtxPosition(&sp2c, &sp38);
    i_this->mCcSphs[1].SetC(sp38 + sp50);
    i_this->mCcSphs[1].SetR(l_HIO.basic_size * 35.0f);
    
    MTXCopy(model->getAnmMtx(JNT_WAIST), *calc_mtx);
    sp2c.set(BREG_F(14) + 30.0f, BREG_F(15), BREG_F(16));
    MtxPosition(&sp2c, &sp38);
    i_this->mCcSphs[2].SetC(sp38 + sp50);
    i_this->mCcSphs[2].SetR(l_HIO.basic_size * 35.0f);

    for (int i = 0; i <= 2; i++) {
        if (i_this->mCoSetBitFlag != 0) {
            i_this->mCcSphs[i].OnCoSetBit();
        } else {
            i_this->mCcSphs[i].OffCoSetBit();
        }
        dComIfG_Ccsp()->Set(&i_this->mCcSphs[i]);
    }

    if (i_this->mSwordModel != NULL) {
        MTXCopy(i_this->mpModelMorf->getModel()->getAnmMtx(JNT_HANDR1), *calc_mtx);
        i_this->mSwordModel->setBaseTRMtx(*calc_mtx);

        if (i_this->mHitCheckFlag) {
            if (i_this->mType == 0) {
                sp2c.set(BREG_F(7), BREG_F(8), BREG_F(9) + -60.0f);
            } else {
                sp2c.set(BREG_F(7), BREG_F(8), BREG_F(9) + -30.0f);
            }

            MtxPosition(&sp2c, &sp38);

            if (i_this->field_0x6aa == 0) {
                i_this->field_0x6aa = 1;
                i_this->mAtSph.StartCAt(sp38);
            } else {
                i_this->mAtSph.MoveCAt(sp38);
            }

            i_this->mAtSph.SetR(l_HIO.basic_size * 50.0f);
        }
    }

    if (i_this->arg3 != 0xFF) {
        MTXCopy(i_this->mpModelMorf->getModel()->getAnmMtx(JNT_HANDL1), *calc_mtx);
        i_this->mShieldModel->setBaseTRMtx(*calc_mtx);
    }

    if (i_this->mHitCheckFlag == 0) {
        i_this->mAtSph.SetC(sp5c);
        i_this->field_0x6aa = 0;
    } else {
        i_this->mHitCheckFlag = 0;
    }
    
    dComIfG_Ccsp()->Set(&i_this->mAtSph);

    if (i_this->field_0x6ab != 0) {
        MTXCopy(i_this->mpModelMorf->getModel()->getAnmMtx(JNT_HANDL1), *calc_mtx);
        sp2c.set(JREG_F(5), JREG_F(6) + 40.0f, JREG_F(7));
        MtxPosition(&sp2c, &sp38);
        i_this->mTateSph.SetC(sp38);
        i_this->mTateSph.SetR((JREG_F(8) + 50.0f) * l_HIO.basic_size);
        i_this->field_0x6ab = 0;

        if (i_this->mUnkTimer2 == 0 && i_this->mTateSph.ChkTgHit()) {
            i_this->mUnkTimer2 = 15;
            dScnPly_c::setPauseTimer(3);

            if (i_this->arg3 == 1) {
                i_this->mAtInfo.mpCollider = i_this->mTateSph.GetTgHitObj();
                at_power_check(&i_this->mAtInfo);

                if (i_this->mAtInfo.mAttackPower <= 10) {
                    i_this->field_0x5c0 -= i_this->mAtInfo.mAttackPower;
                } else {
                    i_this->field_0x5c0 = 0;
                }

                if (i_this->field_0x5c0 <= 0) {
                    i_this->arg3 = 0xFF;
                    cXyz scale(l_HIO.basic_size, l_HIO.basic_size, l_HIO.basic_size);

                    static u16 p_name[2] = {
                        dPa_RM(ID_ZI_S_SF_SHIELD_A),
                        dPa_RM(ID_ZI_S_SF_SHIELD_B),
                    };

                    for (int i = 0; i <= 1; i++) {
                        JPABaseEmitter* emitter = dComIfGp_particle_set(p_name[i], &a_this->current.pos, &a_this->shape_angle, &scale);
                        if (emitter != NULL) {
                            MTXCopy(i_this->mShieldModel->getAnmMtx(JNT_WORLD_ROOT), *calc_mtx);
                            emitter->setGlobalRTMatrix(*calc_mtx);
                        }
                    }

                    i_this->mSound.startCreatureSound(Z2SE_OBJ_WOODSHIELD_BREAK, 0, -1);
                } else {
                    def_se_set(&i_this->mSound, i_this->mTateSph.GetTgHitObj(), 0x29, 0);
                }
            } else {
                def_se_set(&i_this->mSound, i_this->mTateSph.GetTgHitObj(), 0x28, 0);
            }
        }
    } else {
        i_this->mTateSph.SetC(sp38 + sp5c);
    }

    dComIfG_Ccsp()->Set(&i_this->mTateSph);

    demo_camera(i_this);
    a_this->attention_info.flags |= fopAc_AttnFlag_UNK_0x200000;
    cXyz pos(a_this->eyePos);
    pos.y += NREG_F(7) + 180.0f;
    a_this->setHeadLockPos(&pos);
    a_this->attention_info.flags |= fopAc_AttnFlag_UNK_0x200000;

    return 1;
}

static int daE_SF_IsDelete(e_sf_class* i_this) {
    return 1;
}

static int daE_SF_Delete(e_sf_class* i_this) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)&i_this->actor;

    fopAcM_GetID(a_this);
    dComIfG_resDelete(&i_this->mPhase, "E_sf");

    if (i_this->mIsFirstSpawn) {
        hio_set = false;
    }

    if (a_this->heap != NULL) {
        i_this->mpModelMorf->stopZelAnime();
    }

    return 1;
}

static int useHeapInit(fopAc_ac_c* a_this) {
    e_sf_class* i_this = (e_sf_class*)a_this;

    i_this->mpModelMorf = new mDoExt_McaMorfSO((J3DModelData*)dComIfG_getObjectRes("E_sf", BMDR_SF), NULL, NULL,
                                               (J3DAnmTransform*)dComIfG_getObjectRes("E_sf", BCK_SF_WAIT01), 0, 1.0f, 0, -1,
                                               &i_this->mSound, 0x80000, 0x11000084);
    if (i_this->mpModelMorf == NULL || i_this->mpModelMorf->getModel() == NULL) {
        return 0;
    }

    J3DModel* model = i_this->mpModelMorf->getModel();
    model->setUserArea((uintptr_t)a_this);
    for (u16 i = 0; i < model->getModelData()->getJointNum(); i++) {
        if (i != 0) {
            model->getModelData()->getJointNodePointer(i)->setCallBack(nodeCallBack);
        }
    }

    static int wepon_data[2] = {
        BMDR_SF_SWORDA, BMDR_SF_SWORDB,
    };

    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes("E_sf", wepon_data[i_this->mType]);
    JUT_ASSERT(2752, modelData != NULL);
    
    i_this->mSwordModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
    if (i_this->mSwordModel == NULL) {
        return 0;
    }

    static int tate_data[2] = {
        BMDR_SF_SHIELDA, BMDR_SF_SHIELDB,
    };

    modelData = (J3DModelData*)dComIfG_getObjectRes("E_sf", tate_data[i_this->arg3]);
    JUT_ASSERT(2762, modelData != NULL);

    i_this->mShieldModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
    if (i_this->mShieldModel == NULL) {
        return 0;
    }

    return 1;
}

static cPhs__Step daE_SF_Create(fopAc_ac_c* a_this) {
    e_sf_class* i_this = (e_sf_class*)a_this;
    fopAcM_ct(a_this, e_sf_class);

    cPhs__Step phase = (cPhs__Step)dComIfG_resLoad(&i_this->mPhase, "E_sf");
    if (phase == cPhs_COMPLEATE_e) {
        i_this->mSwBit = a_this->current.angle.z & 0xFF;
        if (i_this->mSwBit != 0xFF) {
            if (dComIfGs_isSwitch(i_this->mSwBit, fopAcM_GetRoomNo(a_this))) {
                return cPhs_ERROR_e;
            }
        }

        a_this->shape_angle.z = 0;
        a_this->current.angle.z = 0;
        OS_REPORT("E_SF PARAM %x\n", fopAcM_GetParam(a_this));

        i_this->mWaitType = fopAcM_GetParam(a_this);
        if (i_this->mWaitType == 0xFF) {
            i_this->mWaitType = 0;
        }

        if (fopAcM_GetRoomNo(a_this) == 7) {
            i_this->mWaitType = 2;
        }

        i_this->arg1 = fopAcM_GetParam(a_this) >> 8;
        if (i_this->arg1 == 0xFF) {
            i_this->arg1 = 3;
        }

        i_this->mType = (fopAcM_GetParam(a_this) >> 24) & 0xF;
        i_this->arg3 = fopAcM_GetParam(a_this) >> 28;
        if (i_this->mType > 1) {
            i_this->mType = 0;
        }
        if (i_this->arg3 > 1) {
            i_this->arg3 = 0;
        }

        OS_REPORT("E_SF//////////////E_SF SET 1 !!\n");

        if (!fopAcM_entrySolidHeap(a_this, useHeapInit, 0x2F60)) {
            OS_REPORT("//////////////E_SF SET NON !!\n");
            return cPhs_ERROR_e;
        }

        if (!hio_set) {
            i_this->mIsFirstSpawn = true;
            hio_set = 1;
            l_HIO.id = -1;
        }

        fopAcM_OnStatus(a_this, fopAcM_STATUS_UNK_0x100);
        a_this->attention_info.flags = fopAc_AttnFlag_BATTLE_e;
        
        fopAcM_SetMtx(a_this, i_this->mpModelMorf->getModel()->getBaseTRMtx());
        fopAcM_SetMin(a_this, -200.0f, -200.0f, -200.0f);
        fopAcM_SetMax(a_this, 200.0f, 200.0f, 200.0f);
        i_this->mBgc.Set(fopAcM_GetPosition_p(a_this), fopAcM_GetOldPosition_p(a_this), a_this, 1, &i_this->mAcchCir,
                         fopAcM_GetSpeed_p(a_this), NULL, NULL);
        i_this->mAcchCir.SetWall(80.0f, 100.0f);
        a_this->health = 200;
        a_this->field_0x560 = 200;
        i_this->mStts.Init(200, 0, a_this);

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
                {dCcD_SE_METAL, 0x0, 0x0, 0x0, 0x0}, // mGObjAt
                {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x2}, // mGObjTg
                {0x0}, // mGObjCo
            }, // mObjInf
            {
                {{0.0f, 0.0f, 0.0f}, 35.0f} // mSph
            } // mSphAttr
        };

        static dCcD_SrcSph tate_sph_src = {
            {
                {0x0, {{0x0, 0x0, 0x0}, {0xd8fbfdff, 0x3}, 0x0}}, // mObj
                {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0}, // mGObjAt
                {dCcD_SE_STONE, 0x2, 0x0, 0x0, 0x3}, // mGObjTg
                {0x0}, // mGObjCo
            }, // mObjInf
            {
                {{0.0f, 0.0f, 0.0f}, 100.0f} // mSph
            } // mSphAttr
        };

        for (int i = 0; i <= 2; i++) {
            i_this->mCcSphs[i].Set(cc_sph_src);
            i_this->mCcSphs[i].SetStts(&i_this->mStts);
        }

        i_this->mAtSph.Set(at_sph_src);
        i_this->mAtSph.SetStts(&i_this->mStts);
        i_this->mTateSph.Set(tate_sph_src);
        i_this->mTateSph.SetStts(&i_this->mStts);

        i_this->mFrameCounter = (int)cM_rndF(65535.0f) & 0xFF00;
        i_this->mSound.init(&a_this->current.pos, &a_this->eyePos, 3, 1);
        i_this->mSound.setEnemyName("E_sf");
        i_this->mAtInfo.mpSound = &i_this->mSound;
        i_this->mAtInfo.field_0x18 = 34;
        i_this->field_0x5c0 = 30;

        if (i_this->mWaitType == 0) {
            i_this->mAction = ACTION_NORMAL;
        } else if (i_this->mWaitType == 1) {
            i_this->mAction = ACTION_CRASHWAIT;
        } else if (i_this->mWaitType == 2) {
            i_this->mAction = ACTION_SITWAIT;
        }

        ret_ct = 0;
        daE_SF_Execute(i_this);
    }

    return phase;
}

AUDIO_INSTANCES;

static actor_method_class l_daE_SF_Method = {
    (process_method_func)daE_SF_Create,
    (process_method_func)daE_SF_Delete,
    (process_method_func)daE_SF_Execute,
    (process_method_func)daE_SF_IsDelete,
    (process_method_func)daE_SF_Draw,
};

actor_process_profile_definition g_profile_E_SF = {
  fpcLy_CURRENT_e,        // mLayerID
  7,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_E_SF,              // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(e_sf_class),     // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  123,                    // mPriority
  &l_daE_SF_Method,       // sub_method
  0x00040000,             // mStatus
  fopAc_ENEMY_e,          // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};
