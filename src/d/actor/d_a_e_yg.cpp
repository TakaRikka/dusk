/**
 * @file d_a_e_yg.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_e_yg.h"
#include "d/d_cc_d.h"
#include "f_op/f_op_kankyo_mng.h"
#include "d/actor/d_a_obj_carry.h"
#include "Z2AudioLib/Z2Instances.h"
#include "f_op/f_op_actor_enemy.h"

enum E_yg_RES_File_ID {
    /* BCK */
    /* 0x04 */ BCK_YG_BITE_DIE = 0x4,
    /* 0x05 */ BCK_YG_DAMAGE,
    /* 0x06 */ BCK_YG_DIE,
    /* 0x07 */ BCK_YG_DIE_FLOAT,
    /* 0x08 */ BCK_YG_FIND,
    /* 0x09 */ BCK_YG_GNAW,
    /* 0x0A */ BCK_YG_JUMP_END,
    /* 0x0B */ BCK_YG_JUMP_MIDDLE,
    /* 0x0C */ BCK_YG_JUMP_START,
    /* 0x0D */ BCK_YG_RUN,
    /* 0x0E */ BCK_YG_SWIM,
    /* 0x0F */ BCK_YG_WAIT,
    /* 0x10 */ BCK_YG_WALK,

    /* BMDR */
    /* 0x13 */ BMDR_YG = 0x13,
};

enum Joint {
    /* 0x0 */ JNT_WORLD_ROOT,
    /* 0x1 */ JNT_BODY,
    /* 0x2 */ JNT_TAIL_01,
    /* 0x3 */ JNT_TAIL_02,
    /* 0x4 */ JNT_TAIL_03,
    /* 0x5 */ JNT_TAIL_04,
};

enum Action {
    /* 0x0 */ ACTION_NORMAL,
    /* 0x1 */ ACTION_ATTACK,
    /* 0x2 */ ACTION_DOKURO,
    /* 0x3 */ ACTION_SWIM,
    /* 0x4 */ ACTION_DAMAGE,
    /* 0x5 */ ACTION_WOLFBITE,
    /* 0xA */ ACTION_STANDBY = 0xA,
};

enum Action_Mode {
    /* 0x0 */ MODE_INIT,

    /* e_yg_normal */
    /* 0x1 */ NORMAL_MODE_1,
    /* 0x2 */ NORMAL_MODE_2,
    /* 0x5 */ NORMAL_MODE_5 = 0x5,
    /* 0x6 */ NORMAL_MODE_6,

    /* e_yg_attack */
    /* 0x01 */ ATTACK_MODE_RUN = 0x1,
    /* 0x02 */ ATTACK_MODE_2,
    /* 0x03 */ ATTACK_MODE_JUMPING,
    /* 0x04 */ ATTACK_MODE_JUMP_END,
    /* 0x05 */ ATTACK_MODE_END,
    /* 0x17 */ ATTACK_MODE_23 = 0x17,

    /* e_yg_swim */
    /* 0x1 */ SWIM_MODE_1 = 0x1,

    /* e_yg_dokuro */
    /* 0x1 */ DOKURO_MODE_RUN = 0x1,
    /* 0x2 */ DOKURO_MODE_2,
    /* 0x3 */ DOKURO_MODE_3,

    /* e_yg_damage */
    /* 0x1 */ DAMAGE_MODE_DIE = 0x1,
    /* 0x2 */ DAMAGE_MODE_DISAPPEAR,

    /* e_yg_wolfbite */
    /* 0x1 */ WOLFBITE_MODE_END = 0x1,
};

class daE_YG_HIO_c : public JORReflexible {
public:
    daE_YG_HIO_c();
    virtual ~daE_YG_HIO_c() {}
    void genMessage(JORMContext*);

    /* 0x04 */ s8 id;
    /* 0x08 */ f32 basic_size;
    /* 0x0C */ f32 movement_spd;
    /* 0x10 */ f32 attack_spd;
    /* 0x14 */ f32 swim_spd;
    /* 0x18 */ f32 pl_recognition_dist;
};

daE_YG_HIO_c::daE_YG_HIO_c() {
    id = -1;
    basic_size = 1.0f;
    movement_spd = 15.0f;
    attack_spd = 40.0f;
    swim_spd = 4.0f;
    pl_recognition_dist = 450.0f;
}

#if DEBUG
void daE_YG_HIO_c::genMessage(JORMContext* ctx) {
    ctx->genLabel("  グース", 0x80000001, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    ctx->genSlider("基本サイズ", &basic_size, 0.0f, 3.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    ctx->genSlider("移動速度", &movement_spd, 0.0f, 50.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    ctx->genSlider("攻撃速度", &attack_spd, 0.0f, 50.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    ctx->genSlider("泳ぎ速度", &swim_spd, 0.0f, 50.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    ctx->genSlider("ＰＬ認識距離", &pl_recognition_dist, 0.0f, 2000.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
}
#endif

static void anm_init(e_yg_class* i_this, int i_index, f32 i_morf, u8 i_attr, f32 i_rate) {
    i_this->mpMorf->setAnm((J3DAnmTransform*)dComIfG_getObjectRes("E_YG", i_index), i_attr, i_morf, i_rate, 0.0f, -1.0f);
    i_this->mAnm = i_index;
}

static BOOL pl_check(e_yg_class* i_this, f32 i_dist) {
    fopAc_ac_c* actor = (fopAc_ac_c*)&i_this->actor;
    fopAc_ac_c* player = dComIfGp_getPlayer(0);

    if (i_this->mPlayerDist < i_dist && !fopAcM_otherBgCheck(actor, player)) {
        return TRUE;
    }

    return FALSE;
}

static int daE_YG_Draw(e_yg_class* i_this) {    
    if (i_this->mDispFlag) {
        return 1;
    }

    fopAc_ac_c* actor = (fopAc_ac_c*)&i_this->actor;

    J3DModel* model = i_this->mpMorf->getModel();
    g_env_light.settingTevStruct(2, &actor->current.pos, &actor->tevStr);
    g_env_light.setLightTevColorType_MAJI(model, &i_this->actor.tevStr);
    dComIfGd_setListDark();
    i_this->mpMorf->entryDL();

    cXyz pos;
    pos.set(actor->current.pos.x, actor->current.pos.y + 100.0f, actor->current.pos.z);
    i_this->mShadowKey = dComIfGd_setShadow(i_this->mShadowKey, 1, model, &pos, 400.0f, 40.0f, actor->current.pos.y,
                                            i_this->mBgc.GetGroundH(), i_this->mBgc.m_gnd, &i_this->actor.tevStr, 0, 1.0f,
                                            dDlst_shadowControl_c::getSimpleTex());

    GXColor color;
    color.r = JREG_S(0) + 20;
    color.g = JREG_S(1) + 20;
    color.b = JREG_S(2) + 20;
    color.a = 0xFF;
    i_this->mLineMat.update(10, color, &actor->tevStr);
    dComIfGd_set3DlineMatDark(&i_this->mLineMat);
    dComIfGd_setList();

    return 1;
}

static void sibuki_set(e_yg_class* i_this) {
    fopAc_ac_c* actor = (fopAc_ac_c*)&i_this->actor;

    if (i_this->mSplashTimer == 0) {
        i_this->mSplashTimer = 20;
        cXyz pos(actor->current.pos);
        pos.y = i_this->mGroundCross;

        fopKyM_createWpillar(&pos, 1.0f, 0);

        f32 fVar1 = i_this->mGroundCross - i_this->mBgc.GetGroundH();
        if (fVar1 > 50.0f) {
            i_this->mSound.startCreatureSound(Z2SE_CM_BODYFALL_ASASE_S, 0, -1);
        } else {
            i_this->mSound.startCreatureSound(Z2SE_EN_YG_FALLWATER, 0, -1);
        }
    }
}

static bool l_initHIO;

static daE_YG_HIO_c l_HIO;

static void ms_disappear(e_yg_class* i_this) {
    fopAc_ac_c* actor = (fopAc_ac_c*)&i_this->actor;
    cXyz unused, prtcl_pos;
    cXyz prtcl_a_scale(0.65f, 0.65f, 0.65f);
    cXyz prtcl_b_scale(0.6f, 1.0f, 0.6f);
    static cXyz e_pos[3] = {
        cXyz(0.0f, 60.0f, 36.0f),
        cXyz(-18.0f, 22.0f, 45.0f),
        cXyz(0.0f, -2.0f, -68.0f),
    };
    
    dComIfGp_particle_set(dPa_RM(ID_ZI_S_YAMIDEAD_NORMAL_A), &actor->current.pos, NULL, &prtcl_a_scale);

    int idx;
    if (i_this->mAnm == BCK_YG_BITE_DIE) {
        idx = 0;
    } else if (i_this->mAnm == BCK_YG_DIE) {
        idx = 1;
    } else {
        idx = 2;
    }

    cMtx_YrotS(*calc_mtx, actor->shape_angle.y);
    MtxPosition(&e_pos[idx], &prtcl_pos);
    prtcl_pos += actor->current.pos;

    dComIfGp_particle_set(dPa_RM(ID_ZI_S_YAMIDEAD_NORMAL_B), &prtcl_pos, NULL, &prtcl_b_scale);
    fopAcM_seStart(actor, Z2SE_DARK_VANISH, 0);

    if (i_this->arg0 == 1) {
        actor->current = actor->home;
        actor->old = actor->current;
        actor->health = 1;
        i_this->mAction = ACTION_NORMAL;
        i_this->mActionMode = MODE_INIT;

        if (i_this->mSwBit != 0xFF) {
            i_this->mUnkFlag2 = 1;
        }
    } else {
        fopAcM_delete(actor);
    }

    fopAcM_createItemFromEnemyID(Z2_ENEMY_NULL, &actor->current.pos, -1, -1, NULL, NULL, NULL, NULL);

    int swBit = (fopAcM_GetParam(actor) & 0xFF0000) >> 16;
    if (swBit != 0xFF) {
        dComIfGs_onSwitch(swBit, fopAcM_GetRoomNo(actor));
    }
}

static fopAc_ac_c* target_info[20];

static int target_info_count;

static void* s_d_sub(void* i_actor, void* i_data) {
    daObjCarry_c* actor = (daObjCarry_c*)i_actor;
    
    if (fopAcM_IsActor(actor) && fopAcM_GetName(actor) == PROC_Obj_Carry && actor->getType() == 5 && target_info_count < 20) {
        target_info[target_info_count] = actor;
        target_info_count++;
    }

    return NULL;
}

static fopAc_ac_c* search_dokuro(e_yg_class* i_this) {
    fopAc_ac_c* actor = (fopAc_ac_c*)&i_this->actor;

    target_info_count = 0;
    for (int i = 0; i < 20; i++) {
        target_info[i] = NULL;
    }

    fpcM_Search(s_d_sub, i_this);
    f32 fVar1 = 100.0f;

    if (target_info_count != 0) {
        cXyz pos_delta;
        fopAc_ac_c* target_actor;
        for (int i = 0; i < target_info_count;) {
            target_actor = target_info[i];
            pos_delta = target_actor->current.pos - actor->current.pos;

            f32 sqrt_val = JMAFastSqrt(pos_delta.x * pos_delta.x + pos_delta.z * pos_delta.z);
            if (sqrt_val < fVar1 && !fopAcM_otherBgCheck(actor, target_info[i])) {
                return target_actor;
            }

            if (++i == target_info_count) {
                i = 0;
                fVar1 += 100.0f;

                if (fVar1 > 1000.0f) {
                    return NULL;
                }
            }
        }
    } else {
        return NULL;
    }

    return NULL;
}

static BOOL way_set(e_yg_class* i_this) {
    fopAc_ac_c* actor = (fopAc_ac_c*)&i_this->actor;
    cXyz start, end, work;
    s16 y_rot = cM_rndF(65535.0f);
    f32 pos_z_val = 1000.0f;

    start = actor->current.pos;
    start.y += 50.0f;

    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 16; j++) {
            cMtx_YrotS(*calc_mtx, y_rot);
            work.x = 0.0f;
            work.y = 0.0f;
            work.z = pos_z_val;
            MtxPosition(&work, &end);
            end += start;

            dBgS_LinChk lin_chk;
            lin_chk.Set(&start, &end, actor);
            if (dComIfG_Bgsp().LineCross(&lin_chk)) {
                y_rot += (s16)0x1000;
            } else {
                i_this->mCurrentAngleYTarget = y_rot;
                return TRUE;
            }
        }

        pos_z_val -= 150.0f;
    }

    return FALSE;
}

static s8 e_yg_normal(e_yg_class* i_this) {
    fopAc_ac_c* actor = (fopAc_ac_c*)&i_this->actor;
    f32 target = 0.0f;
    cXyz work;
    s8 rv = 1;

    switch (i_this->mActionMode) {
        case MODE_INIT:
            anm_init(i_this, BCK_YG_RUN, 3.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
            i_this->mTimers[0] = cM_rndF(60.0f) + 30.0f;
            i_this->mActionMode = NORMAL_MODE_1;
            i_this->mFrameCounter = cM_rndF(65535.0f);
            i_this->mTimers[2] = 20;
            break;

        case NORMAL_MODE_1:
            target = l_HIO.movement_spd;

            if (i_this->mpMorf->checkFrame(1.0f)) {
                i_this->mCurrentAngleYTarget += (s16)cM_rndFX(2000.0f);
            }

            if (i_this->mTimers[0] == 0 || (i_this->mTimers[2] == 0 && fopAcM_wayBgCheck(actor, 200.0f, 50.0f))) {
                i_this->mTimers[0] = cM_rndF(60.0f) + 30.0f;
                i_this->mActionMode = NORMAL_MODE_2;
                anm_init(i_this, BCK_YG_WAIT, 3.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
            }
            break;
        
        case NORMAL_MODE_2:
            if (i_this->mTimers[0] == 0) {
                if (way_set(i_this)) {
                    i_this->mActionMode = MODE_INIT;
                } else {
                    i_this->mTimers[0] = 10;
                }
            }
            break;

        case NORMAL_MODE_5:
            rv = 0;

            if (i_this->mTimers[0] == 0) {
                anm_init(i_this, BCK_YG_RUN, 3.0f, J3DFrameCtrl::EMode_LOOP, 1.5f);
                i_this->mTimers[0] = cM_rndF(60.0f) + 30.0f;
                i_this->mActionMode = NORMAL_MODE_6;
                i_this->mCurrentAngleYTarget = i_this->mPlayerAngleY + 0x8000;
            }
            break;
            
        case NORMAL_MODE_6:
            rv = 0;
            target = l_HIO.movement_spd * 1.5f;

            if (i_this->mpMorf->checkFrame(1.0f)) {
                i_this->mCurrentAngleYTarget += (s16)cM_rndFX(4000.0f);
            }

            if (i_this->mTimers[2] == 0 && fopAcM_wayBgCheck(actor, 200.0f, 50.0f)) {
                i_this->mTimers[2] = 20;
                i_this->mCurrentAngleYTarget = actor->current.angle.y + 0x8000;
            }

            if (i_this->mTimers[0] == 0) {
                i_this->mActionMode = MODE_INIT;
            }
            break;
    }

    if (fopAcM_SearchByID(actor->parentActorID) == NULL && dComIfGp_event_runCheck()) {
        target = 0.0f;
    }

    cLib_addCalcAngleS2(&actor->current.angle.y, i_this->mCurrentAngleYTarget, 2, 0x1000);
    cLib_addCalc2(&actor->speedF, target, 1.0f, l_HIO.movement_spd * 0.25f);

    if (i_this->mUnkFlag2 || (rv && pl_check(i_this, i_this->mDistance))) {
        i_this->mAction = ACTION_ATTACK;
        i_this->mActionMode = MODE_INIT;
    }

    return rv;
}

static s8 e_yg_attack(e_yg_class* i_this) {
    fopAc_ac_c* actor = (fopAc_ac_c*)&i_this->actor;
    f32 target = 0.0f;
    cXyz work;
    s8 rv = 1;
    s8 end_attack_flag = false;

    switch (i_this->mActionMode) {
        /* Initialize */
        case MODE_INIT:
            anm_init(i_this, BCK_YG_FIND, 3.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
            i_this->mActionMode = ATTACK_MODE_RUN;
            break;

        /* Run towards the player */
        case ATTACK_MODE_RUN:
            i_this->mCurrentAngleYTarget = i_this->mPlayerAngleY;

            if (i_this->mpMorf->isStop()) {
                anm_init(i_this, BCK_YG_RUN, 3.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
                i_this->mActionMode = ATTACK_MODE_2;
            }
            break;

        case ATTACK_MODE_2:
            target = l_HIO.movement_spd;
            i_this->mCurrentAngleYTarget = i_this->mPlayerAngleY;

            if (i_this->mPlayerDist < TREG_F(4) + 300.0f) {
                /* If Phantom Zant actor exists */
                if (fopAcM_SearchByName(PROC_E_PZ) != NULL) {
                    i_this->mTimers[0] = fopAcM_GetID(actor) & 0x1F;
                    i_this->mActionMode = ATTACK_MODE_23;
                    anm_init(i_this, BCK_YG_WAIT, 3.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
                } else {
                    i_this->mActionMode = ATTACK_MODE_JUMPING;
                    anm_init(i_this, BCK_YG_JUMP_START, 3.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
                }
            }
            break;

        case ATTACK_MODE_23:
            i_this->mCurrentAngleYTarget = i_this->mPlayerAngleY;

            if (i_this->mTimers[0] == 0) {
                i_this->mActionMode = ATTACK_MODE_JUMPING;
                anm_init(i_this, BCK_YG_JUMP_START, 3.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
            }
            break;

        case ATTACK_MODE_JUMPING:
            i_this->mCurrentAngleYTarget = i_this->mPlayerAngleY;

            if (i_this->mpMorf->isStop()) {
                anm_init(i_this, BCK_YG_JUMP_MIDDLE, 1.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
                actor->speed.y = TREG_F(5) + 30.0f;
                actor->speedF = l_HIO.attack_spd;
                i_this->mActionMode = ATTACK_MODE_JUMP_END;
            }
            break;

        case ATTACK_MODE_JUMP_END:
            rv = 0;
            i_this->mUnkFlag1 = 1;

            if (i_this->mSph2.ChkAtShieldHit()) {
                dComIfGp_getVibration().StartShock(VIBMODE_S_POWER4, 31, cXyz(0.0f, 1.0f, 0.0f));
                i_this->mUnkFlag1 = 0;
                #if DEBUG
                target = 0.0f;
                actor->speedF = target;
                #else
                actor->speedF = 0.0f;
                #endif
            }

            if (i_this->mUnkFlag1 == 0 || i_this->mBgc.ChkGroundHit()) {
                anm_init(i_this, BCK_YG_JUMP_END, 1.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
                i_this->mActionMode = ATTACK_MODE_END;
            }
            break;

        case ATTACK_MODE_END:
            rv = 0;

            if (i_this->mpMorf->isStop()) {
                end_attack_flag = true;
                i_this->mUnkFlag2 = 0;
            }
            break;
    }

    cLib_addCalcAngleS2(&actor->current.angle.y, i_this->mCurrentAngleYTarget, 2, 0x1000);

    if (i_this->mActionMode == ATTACK_MODE_JUMP_END) {
        if (i_this->mBgc.ChkWallHit()) {
            actor->speedF = 0.0f;
        }
    } else {
        cLib_addCalc2(&actor->speedF, target, 1.0f, 10.0f);
    }

    if (i_this->mUnkFlag2 == 0 && (end_attack_flag || !pl_check(i_this, i_this->mDistance + 100.0f))) {
        i_this->mAction = ACTION_NORMAL;
        i_this->mActionMode = MODE_INIT;
    }

    return rv;
}

static void search_ground_1(e_yg_class* i_this) {
    fopAc_ac_c* actor = (fopAc_ac_c*)&i_this->actor;
    dBgS_LinChk lin_chk;
    cXyz start, end, work;
    cXyz pos[16];
    s16 y_rot = actor->shape_angle.y;
    s8 line_cross_flag = false;

    for (int i = 0; i < 16; i++) {
        start = actor->current.pos;
        start.y = i_this->mGroundCross;
        cMtx_YrotS(*calc_mtx, y_rot);
        work.x = 0.0f;
        work.y = 0.0f;
        work.z = 2000.0f;
        MtxPosition(&work, &end);
        end += start;
        lin_chk.Set(&start, &end, actor);
        
        if (dComIfG_Bgsp().LineCross(&lin_chk)) {
            end = lin_chk.GetCross();
            work.x = 0.0f;
            work.y = 30.0f;
            work.z = KREG_F(9) + 100.0f;
            MtxPosition(&work, &start);
            start += end;
            end = start;
            end.y -= 70.0f;
            lin_chk.Set(&start, &end, actor);

            if (dComIfG_Bgsp().LineCross(&lin_chk)) {
                pos[i] = lin_chk.GetCross();
                line_cross_flag = true;
            } else {
                pos[i] = actor->current.pos;
                pos[i].x += 10000.0f;
            }
        }

        y_rot += (s16)0x1000;
    }

    if (line_cross_flag) {
        int idx = 0;
        for (int i = 1; i < 16; i++) {
            if ((pos[idx] - actor->current.pos).abs() > (pos[i] - actor->current.pos).abs()) {
                idx = i;
            }
        }

        i_this->field_0x5bc = pos[idx];
    } else {
        i_this->field_0x5bc.x = actor->current.pos.x + cM_rndFX(300.0f);
        i_this->field_0x5bc.y = actor->current.pos.y;
        i_this->field_0x5bc.z = actor->current.pos.z + cM_rndFX(300.0f);
    }
}

static void e_yg_swim(e_yg_class* i_this) {
    fopAc_ac_c* actor = (fopAc_ac_c*)&i_this->actor;
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    cXyz work;
    s16 maxStep = 0;
    f32 target = l_HIO.swim_spd;
    f32 play_spd = 1.0f;

    switch (i_this->mActionMode) {
        case MODE_INIT:
            anm_init(i_this, BCK_YG_SWIM, 5.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
            i_this->mActionMode++;
            actor->speedF = 0.0f;
            search_ground_1(i_this);
            i_this->mTimers[1] = 50;
            // fallthrough
        case SWIM_MODE_1:
            if (i_this->mPlayerDist < 300.0f) {
                maxStep = 0x800;
                play_spd = 2.0f;
                target = 8.0f;
                i_this->mCurrentAngleYTarget = i_this->mPlayerAngleY;
                i_this->mUnkFlag1 = 1;
            } else {
                maxStep = 0x300;

                if ((i_this->mFrameCounter & 0x3F) == 0 && cM_rndF(1.0f) < 0.5f) {
                    search_ground_1(i_this);
                }

                work = i_this->field_0x5bc - actor->current.pos;
                i_this->mCurrentAngleYTarget = cM_atan2s(work.x, work.z);
            }
            break;
    }

    cLib_addCalc2(&actor->speedF, target, 1.0f, 0.5f);
    i_this->mpMorf->setPlaySpeed(play_spd);
    cLib_addCalcAngleS2(&actor->current.angle.y, i_this->mCurrentAngleYTarget, 8, maxStep);
    actor->gravity = actor->speed.y = 0.0f;
    cLib_addCalc2(&actor->current.pos.y, i_this->mGroundCross - 40.0f + KREG_F(4), 1.0f, 5.0f);

    if (i_this->mTimers[1] == 0 && i_this->mBgc.ChkWallHit()) {
        actor->current.pos.y += 10.0f;
        actor->speed.y = nREG_F(8) + 30.0f;
        actor->speedF = 20.0f;
        i_this->mAction = ACTION_NORMAL;
        i_this->mActionMode = MODE_INIT;
    }
}

static void e_yg_dokuro(e_yg_class* i_this) {
    fopAc_ac_c* actor = (fopAc_ac_c*)&i_this->actor;
    fopAc_ac_c* skull_p = fopAcM_SearchByID(i_this->mSkullActorID);

    if (skull_p == NULL || i_this->mBgc.ChkWallHit() || fopAcM_checkCarryNow(skull_p)) {
        i_this->mAction = ACTION_NORMAL;
        i_this->mActionMode = MODE_INIT;
    } else {
        cXyz pos_delta(skull_p->current.pos - actor->current.pos);
        f32 abs_val = pos_delta.abs();
        i_this->mCurrentAngleYTarget = (s16)cM_atan2s(pos_delta.x, pos_delta.z);
        f32 target = 0.0f;

        switch (i_this->mActionMode) {
            case MODE_INIT:
                anm_init(i_this, BCK_YG_FIND, 3.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
                i_this->mActionMode = DOKURO_MODE_RUN;
                break;

            case DOKURO_MODE_RUN:
                if (i_this->mpMorf->isStop()) {
                    anm_init(i_this, BCK_YG_RUN, 3.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
                    i_this->mActionMode = DOKURO_MODE_2;
                }
                break;
            
            case DOKURO_MODE_2:
                if (abs_val < TREG_F(14) + 75.0f) {
                    anm_init(i_this, BCK_YG_GNAW, 3.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
                    i_this->mTimers[0] = cM_rndF(60.0f) + 90.0f;
                    i_this->mActionMode = DOKURO_MODE_3;
                } else {
                    target = l_HIO.movement_spd;
                }
                break;

            case DOKURO_MODE_3:
                if (abs_val < TREG_F(15) + 65.0f) {
                    target = -2.0f;
                } else if (abs_val > TREG_F(16) + 70.0f) {
                    target = 2.0f;
                }
                break;
        }

        cLib_addCalcAngleS2(&actor->current.angle.y, i_this->mCurrentAngleYTarget, 2, 0x2000);
        cLib_addCalc2(&actor->speedF, target, 1.0f, 10.0f);

        if (pl_check(i_this, i_this->mDistance)) {
            i_this->mAction = ACTION_ATTACK;
            i_this->mActionMode = MODE_INIT;
        }
    }
}

static void e_yg_damage(e_yg_class* i_this) {
    fopAc_ac_c* actor = (fopAc_ac_c*)&i_this->actor;

    i_this->mInvulnerabilityTimer = 6;

    switch (i_this->mActionMode) {
        case MODE_INIT:
            anm_init(i_this, BCK_YG_DAMAGE, 2.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
            i_this->mActionMode = DAMAGE_MODE_DIE;
            actor->speed.y = cM_rndF(10.0f) + 40.0f;
            actor->speedF = -30.0f;
            break;
        
        case DAMAGE_MODE_DIE:
            if (i_this->mBgc.ChkGroundHit()) {
                if (actor->health <= 0) {
                    i_this->mDeathFlag = 1;
                    anm_init(i_this, BCK_YG_DIE, 5.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
                    i_this->mTimers[0] = cM_rndF(10.0f) + 5.0f;
                    i_this->mActionMode = DAMAGE_MODE_DISAPPEAR;
                    actor->speed.y = 10.0f;
                    actor->speedF *= 0.5f;
                } else {
                    i_this->mAction = ACTION_ATTACK;
                    i_this->mActionMode = MODE_INIT;
                }

                i_this->mSound.startCreatureSound(Z2SE_CM_BODYFALL_S, 0, -1);
            } else if (actor->speed.y <= 0.0f && i_this->mWaterFlag) {
                i_this->mDeathFlag = 1;
                anm_init(i_this, BCK_YG_DIE_FLOAT, 3.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
                sibuki_set(i_this);
                i_this->mTimers[0] = 30;
                i_this->mActionMode = DAMAGE_MODE_DISAPPEAR;
            }
            break;

        case DAMAGE_MODE_DISAPPEAR:
            if (actor->speed.y <= 0.0f && (i_this->mBgc.ChkGroundHit() || i_this->mWaterFlag)) {
                actor->speedF *= 0.5f;
            }

            if (i_this->mTimers[0] == 0) {
                ms_disappear(i_this);
            }
            break;
    }

    if (actor->speed.y <= 0.0f && actor->current.pos.y <= i_this->mGroundCross - 20.0f + KREG_F(0)) {
        actor->gravity = actor->speed.y = 0.0f;
        cLib_addCalc2(&actor->current.pos.y, i_this->mGroundCross - 20.0f + KREG_F(4), 1.0f, 5.0f);
    }
}

static void e_yg_wolfbite(e_yg_class* i_this) {
    fopAc_ac_c* actor = (fopAc_ac_c*)&i_this->actor;
    daPy_py_c* player = (daPy_py_c*)dComIfGp_getPlayer(0);

    switch (i_this->mActionMode) {
        case MODE_INIT:
            i_this->mDeathFlag = 1;
            anm_init(i_this, BCK_YG_BITE_DIE, 0.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
            i_this->mActionMode = WOLFBITE_MODE_END;
            i_this->mSound.startCreatureVoice(Z2SE_EN_YG_V_DAMAGE, -1);
            break;
        
        case WOLFBITE_MODE_END:
            if (!player->checkWolfEnemyCatchOwn(actor)) {
                i_this->mAction = ACTION_DAMAGE;
                i_this->mActionMode = MODE_INIT;
                i_this->mSound.startCreatureVoice(Z2SE_EN_YG_V_DEATH, -1);
                actor->health = 0;

                if (player->checkWolfEnemyLeftThrow()) {
                    actor->current.angle.y = player->shape_angle.y - 0x4000;
                } else {
                    actor->current.angle.y = player->shape_angle.y + 0x4000;
                }
            }
            break;
    }
}

static void e_yg_standby(e_yg_class* i_this) {
    fopAc_ac_c* actor = (fopAc_ac_c*)&i_this->actor;

    i_this->mInvulnerabilityTimer = 6;

    if (dComIfGs_isSwitch(i_this->mSwBit, fopAcM_GetRoomNo(actor))) {
        i_this->mDispFlag = 0;
        i_this->mAction = ACTION_NORMAL;
        i_this->mActionMode = MODE_INIT;
    }
}

static void damage_check(e_yg_class* i_this) {
    fopAc_ac_c* actor = (fopAc_ac_c*)&i_this->actor;
    daPy_py_c* player = (daPy_py_c*)dComIfGp_getPlayer(0);

    if (i_this->mInvulnerabilityTimer == 0) {
        i_this->mStts.Move();

        if (i_this->mSph1.ChkTgHit()) {
            i_this->mAtInfo.mpCollider = i_this->mSph1.GetTgHitObj();
            cc_at_check(actor, &i_this->mAtInfo);

            if (
                i_this->mAtInfo.mpCollider->ChkAtType(AT_TYPE_HOOKSHOT) ||
                i_this->mAtInfo.mpCollider->ChkAtType(AT_TYPE_SLINGSHOT) ||
                i_this->mAtInfo.mpCollider->ChkAtType(AT_TYPE_2000000)
            ) {
                actor->health = 0;
            }

            if (i_this->mAtInfo.mpCollider->ChkAtType(AT_TYPE_UNK)) {
                i_this->mInvulnerabilityTimer = 20;
            } else {
                i_this->mInvulnerabilityTimer = 10;
            }

            if (i_this->mAtInfo.mpCollider->ChkAtType(AT_TYPE_WOLF_ATTACK) && player->onWolfEnemyCatch(actor)) {
                i_this->mAction = ACTION_WOLFBITE;
                i_this->mActionMode = 0;
                i_this->mInvulnerabilityTimer = 200;
                dScnPly_c::setPauseTimer(0);
                return;
            }

            i_this->mAction = ACTION_DAMAGE;
            i_this->mActionMode = MODE_INIT;
            actor->current.angle.y = i_this->mAtInfo.mHitDirection.y;

            if (actor->health != 0 && i_this->mAtInfo.mHitType == HIT_TYPE_STUN) {
                actor->health = 1;
            } else {
                i_this->mSound.startCreatureVoice(Z2SE_EN_YG_V_DEATH, -1);
            }
        }

        if (actor->health <= 10) {
            i_this->mSph1.SetTgHitMark(CcG_Tg_UNK_MARK_3);
        }
    }
}

static void ke_control(e_yg_class* i_this, yg_ke_s* yg_p, int param_3, f32 i_posZ) {
    cXyz work, pos_offset;
    int i;
    cXyz* pcVar1 = &yg_p->field_0x0[1];
    cXyz* pcVar2 = &yg_p->field_0x78[1];

    work.x = 0.0f;
    work.y = 0.0f;
    work.z = i_posZ;

    cXyz spc4;
    f32 fVar2, fVar3, fVar4;
    f32 fVar1;
    if (i_this->mWaterFlag) {
        fVar1 = ZREG_F(9) + -5.0f;
    } else {
        fVar1 = ZREG_F(8) + -12.0f;
    }

    s16 sVar1 = cM_rndF2(65536.0f);
    
    fVar2 = JREG_F(8) + 3.0f;
    f32 fVar5 = JREG_F(5) + (i_this->mBgc.GetGroundH() + 3.0f);
    if (fVar5 < i_this->mGroundCross) {
        fVar5 = i_this->mGroundCross;
    }
    
    f32 fVar6 = JREG_F(17) + 0.8f;

    for (i = 1; i < 10; i++, pcVar1++, pcVar2++) {
        f32 fVar7 = fVar2 * cM_ssin(sVar1 + i * 7000);
        f32 fVar8 = fVar2 * cM_ssin(sVar1 + 10000 + i * 6000);
        f32 fVar9 = (10 - i) * 0.1f;

        spc4.x = pcVar2->x + (fVar7 + yg_p->field_0xf0.x * fVar9);
        spc4.y = pcVar2->y + yg_p->field_0xf0.y * fVar9;
        spc4.z = pcVar2->z + (fVar8 + yg_p->field_0xf0.z * fVar9);
        
        fVar3 = spc4.x + (pcVar1->x - pcVar1[-1].x);
        
        f32 fVar10;
        f32 fVar11 = spc4.z + (pcVar1->z - pcVar1[-1].z);

        fVar4 = fVar1 + (pcVar1->y + spc4.y);
        if (fVar4 < fVar5) {
            fVar4 = fVar5;
        }

        fVar10 = fVar4 - pcVar1[-1].y;
        s16 x_rot = -cM_atan2s(fVar10, fVar11);
        s16 y_rot = (s16)cM_atan2s(fVar3, JMAFastSqrt(fVar10 * fVar10 + fVar11 * fVar11));
        cMtx_XrotS(*calc_mtx, x_rot);
        cMtx_YrotM(*calc_mtx, y_rot);
        MtxPosition(&work, &pos_offset);

        *pcVar2 = *pcVar1;
        
        pcVar1->x = pcVar1[-1].x + pos_offset.x;
        pcVar1->y = pcVar1[-1].y + pos_offset.y;
        pcVar1->z = pcVar1[-1].z + pos_offset.z;

        pcVar2->x = fVar6 * (pcVar1->x - pcVar2->x);
        pcVar2->y = fVar6 * (pcVar1->y - pcVar2->y);
        pcVar2->z = fVar6 * (pcVar1->z - pcVar2->z);
    }
}

static void ke_move(e_yg_class* i_this, mDoExt_3DlineMat0_c* i_lineMat, yg_ke_s* yg_p, int i_idx, f32 i_posZ) {
    ke_control(i_this, yg_p, i_idx, i_posZ);
    cXyz* pos_p = i_lineMat->getPos(i_idx);
    f32* size_p = i_lineMat->getSize(i_idx);

    for (int i = 0; i < 10; i++, pos_p++, size_p++) {
        *pos_p = yg_p->field_0x0[i];

        if (i == JREG_S(7) + 8) {
            *size_p = (JREG_S(8) + 6) * 0.1f;
        } else if (i < 5) {
            *size_p = 3.5f;
        } else {
            *size_p = cM_rndF2(1.0f) + 1.8f;
        }
    }
}

static cXyz ke_p[13] = {
    cXyz(8.0f, 5.0f, 0.0f),
    cXyz(30.0f, 13.0f, 0.0f),
    cXyz(50.0f, 8.0f, 0.0f),
    cXyz(5.0f, 5.0f, -10.0f),
    cXyz(30.0f, 5.0f, -10.0f),
    cXyz(50.0f, 5.0f, -5.0f),
    cXyz(5.0f, 5.0f, 10.0f),
    cXyz(30.0f, 5.0f, 10.0f),
    cXyz(50.0f, 5.0f, 5.0f),
    cXyz(10.0f, -2.0f, 10.0f),
    cXyz(10.0f, -2.0f, -10.0f),
    cXyz(40.0f, -2.0f, 10.0f),
    cXyz(40.0f, -2.0f, -10.0f),
};

static csXyz ke_a[13] = {
    csXyz(0, 0, 5000),
    csXyz(0, 0, 3000),
    csXyz(0, 0, 2000),
    csXyz(0, -8000, 5000),
    csXyz(0, -8000, 0),
    csXyz(0, -8000, -3000),
    csXyz(0, 8000, 5000),
    csXyz(0, 8000, 0),
    csXyz(0, 8000, -3000),
    csXyz(0, 20000, 5000),
    csXyz(0, -20000, 5000),
    csXyz(0, 20000, -5000),
    csXyz(0, -20000, -5000),
};

static void ke_set(e_yg_class* i_this) {
    fopAc_ac_c* actor = (fopAc_ac_c*)&i_this->actor;
    cXyz work, unused;

    cM_initRnd2(12, 123, fopAcM_GetID(actor) * 2 + 50);
    MTXCopy(i_this->mpMorf->getModel()->getAnmMtx(JNT_BODY), *calc_mtx);
    f32 pos_z = ZREG_F(10) + 10.0f;

    for (int i = 0; i < 13; i++) {
        MtxPush();
        MtxTrans(ke_p[i].x, ke_p[i].y, ke_p[i].z, 1);
        cMtx_YrotM(*calc_mtx, ke_a[i].y);
        cMtx_XrotM(*calc_mtx, ke_a[i].x);
        cMtx_ZrotM(*calc_mtx, ke_a[i].z);

        work.set(0.0f, 0.0f, 0.0f);
        MtxPosition(&work, &i_this->mYgKes[i].field_0x0[0]);

        if (i_this->mWaterFlag) {
            work.set(0.0f, ZREG_F(5) + 5.0f, 0.0f);
        } else {
            work.set(0.0f, ZREG_F(4) + 15.0f, 0.0f);
        }
        MtxPosition(&work, &i_this->mYgKes[i].field_0xf0);
        
        i_this->mYgKes[i].field_0xf0 -= i_this->mYgKes[i].field_0x0[0];
        ke_move(i_this, &i_this->mLineMat, &i_this->mYgKes[i], i, pos_z);
        MtxPull();
    }
}

static void action(e_yg_class* i_this) {
    fopAc_ac_c* actor = (fopAc_ac_c*)&i_this->actor;
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    cXyz work, speed_xz;
    
    i_this->mPlayerDist = fopAcM_searchPlayerDistance(actor);
    i_this->mPlayerAngleY = fopAcM_searchPlayerAngleY(actor);

    damage_check(i_this);
    s8 unk_flag_1 = 0;
    s8 unk_flag_2 = 1;
    s8 skull_flag = 0;
    s8 status_flag = 1;
    s8 co_bit_flag = 1;
    dBgS_ObjGndChk_Spl gnd_chk_spl;

    work = actor->current.pos;
    work.y += 200.0f;
    gnd_chk_spl.SetPos(&work);
    i_this->mGroundCross = dComIfG_Bgsp().GroundCross(&gnd_chk_spl);
    s8 link_search_flag = 0;

    switch (i_this->mAction) {
        case ACTION_NORMAL:
            unk_flag_1 = e_yg_normal(i_this);
            skull_flag = 1;
            break;

        case ACTION_ATTACK:
            unk_flag_1 = e_yg_attack(i_this);
            link_search_flag = 1;
            break;

        case ACTION_DOKURO:
            e_yg_dokuro(i_this);
            break;

        case ACTION_SWIM:
            e_yg_swim(i_this);
            unk_flag_2 = 0;
            break;

        case ACTION_DAMAGE:
            e_yg_damage(i_this);
            unk_flag_2 = -1;
            co_bit_flag = 0;
            status_flag = 0;
            break;

        case ACTION_WOLFBITE:
            e_yg_wolfbite(i_this);
            co_bit_flag = 0;
            unk_flag_2 = -1;
            status_flag = 0;
            break;

        case ACTION_STANDBY:
            e_yg_standby(i_this);
            return;
    }

    if (link_search_flag) {
        i_this->mSound.setLinkSearch(true);
    } else {
        i_this->mSound.setLinkSearch(false);
    }

    if (status_flag && actor->health > 0) {
        fopAcM_OnStatus(actor, 0);
        actor->attention_info.flags = fopAc_AttnFlag_BATTLE_e;
    } else {
        fopAcM_OffStatus(actor, 0);
        actor->attention_info.flags = 0;
    }

    if (skull_flag) {
        i_this->mSkullActorID = fpcM_ERROR_PROCESS_ID_e;

        if ((i_this->mFrameCounter & 15) == 0) {
            fopAc_ac_c* skull_p = search_dokuro(i_this);
            if (skull_p != NULL) {
                work = skull_p->current.pos - actor->current.pos;
                
                if (work.abs() < 500.0f) {
                    i_this->mSkullActorID = fopAcM_GetID(skull_p);
                    i_this->mAction = ACTION_DOKURO;
                    i_this->mActionMode = MODE_INIT;
                }
            }
        }
    }

    if (unk_flag_1 && i_this->mPlayerDist < 500.0f) {
        if (daPy_getPlayerActorClass()->checkWolfBark()) {
            i_this->mAction = ACTION_NORMAL;
            i_this->mActionMode = NORMAL_MODE_5;
            anm_init(i_this, BCK_YG_FIND, 1.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
            i_this->mTimers[0] = cM_rndF(10.0f) + 5.0f;
        }
    }

    cLib_addCalcAngleS2(&actor->shape_angle.y, actor->current.angle.y, 2, 0x2000);
    cLib_addCalcAngleS2(&actor->shape_angle.x, actor->current.angle.x, 2, 0x2000);
    cLib_addCalcAngleS2(&actor->shape_angle.z, actor->current.angle.z, 2, 0x2000);
    cMtx_YrotS(*calc_mtx, actor->current.angle.y);
    work.x = 0.0f;
    work.y = 0.0f;
    work.z = actor->speedF;
    MtxPosition(&work, &speed_xz);
    actor->speed.x = speed_xz.x;
    actor->speed.z = speed_xz.z;
    actor->current.pos += actor->speed * l_HIO.basic_size;
    actor->speed.y += actor->gravity;
    actor->gravity = TREG_F(3) + -7.0f;

    if (actor->speed.y < -80.0f) {
        actor->speed.y = -80.0f;
    }

    if (co_bit_flag) {
        i_this->mSph1.OnCoSetBit();
        cXyz* ccMoveP = i_this->mStts.GetCCMoveP();
        if (ccMoveP != NULL) {
            actor->current.pos.x += ccMoveP->x * 0.3f;
            actor->current.pos.z += ccMoveP->z * 0.3f;
        }
    } else {
        i_this->mSph1.OffCoSetBit();
    }

    i_this->mBgc.CrrPos(dComIfG_Bgsp());

    if (actor->current.pos.y <= i_this->mGroundCross - 10.0f + KREG_F(10)) {
        i_this->mWaterFlag = 1;
    } else {
        i_this->mWaterFlag = 0;
    }

    f32 target = 0.0f;
    if (unk_flag_2 == 1) {
        if (actor->current.pos.y <= i_this->mGroundCross - 45.0f + KREG_F(0)) {
            i_this->mAction = ACTION_SWIM;
            i_this->mActionMode = MODE_INIT;
            sibuki_set(i_this);
        }
    } else if (unk_flag_2 == 0) {
        target = KREG_F(2) + 47.0f;
        cXyz pos(actor->current.pos);
        pos.y = i_this->mGroundCross;
        fopAcM_effHamonSet(&i_this->mHamonPrtcl, &pos, 1.0f, KREG_F(11) + 0.1f);

        if (actor->current.pos.y > i_this->mGroundCross - 35.0f + KREG_F(1)) {
            i_this->mAction = ACTION_NORMAL;
            i_this->mActionMode = MODE_INIT;
        }
    }

    cLib_addCalc2(&i_this->mCurrentPosYOffset, target, 0.4f, 5.0f);
}

static void anm_se_set(e_yg_class* i_this) {
    fopAc_ac_c* actor = (fopAc_ac_c*)&i_this->actor;
    s8 footnote_flag = false;

    if (i_this->mAnm == BCK_YG_WAIT) {
        if (i_this->mpMorf->checkFrame(0.0f)) {
            i_this->mSound.startCreatureVoice(Z2SE_EN_YG_V_NAKU, -1);
            footnote_flag = true;
        }
    } else if (i_this->mAnm == BCK_YG_WALK) {
        if (i_this->mpMorf->checkFrame(0.0f)) {
            i_this->mSound.startCreatureVoice(Z2SE_EN_YG_V_NAKU, -1);
        }

        if (i_this->mpMorf->checkFrame(0.0f) || i_this->mpMorf->checkFrame(9.5f)) {
            footnote_flag = true;
        }
    } else if (i_this->mAnm == BCK_YG_RUN) {
        if (i_this->mpMorf->checkFrame(0.0f)) {
            i_this->mSound.startCreatureVoice(Z2SE_EN_YG_V_NAKU, -1);
        }

        if (i_this->mpMorf->checkFrame(0.0f) || i_this->mpMorf->checkFrame(5.5f)) {
            footnote_flag = true;
        } 
    } else if (i_this->mAnm == BCK_YG_JUMP_START) {
        if (i_this->mpMorf->checkFrame(6.5f) || i_this->mpMorf->checkFrame(8.0f)) {
            footnote_flag = true;
        }
    } else if (i_this->mAnm == BCK_YG_JUMP_END) {
        if (i_this->mpMorf->checkFrame(2.5f) || i_this->mpMorf->checkFrame(6.0f)) {
            footnote_flag = true;
        }
    } else if (i_this->mAnm == BCK_YG_FIND) {
        if (i_this->mpMorf->checkFrame(1.0f)) {
            i_this->mSound.startCreatureVoice(Z2SE_EN_YG_V_FIND, -1);
        }
    } else if (i_this->mAnm == BCK_YG_GNAW) {
        if (i_this->mpMorf->checkFrame(0.0f) || i_this->mpMorf->checkFrame(4.5f) || i_this->mpMorf->checkFrame(9.5f) || i_this->mpMorf->checkFrame(15.0f)) {
            i_this->mSound.startCreatureSound(Z2SE_EN_YG_BITE, 0, -1);
        }
    } else if (i_this->mAnm == BCK_YG_SWIM && (i_this->mpMorf->checkFrame(0.0f) || i_this->mpMorf->checkFrame(9.5f))) {
        i_this->mSound.startCreatureSound(Z2SE_EN_YG_SWIM, 0, -1);
    }

    if (footnote_flag) {
        if (i_this->mWaterFlag) {
            i_this->mSound.startCreatureSound(Z2SE_EN_YG_FN_WATER, 0, -1);
        } else {
            i_this->mSound.startCreatureSound(Z2SE_EN_YG_FOOTNOTE, 0, -1);
        }
    }
}

static int daE_YG_Execute(e_yg_class* i_this) {
    fopEn_enemy_c* actor = (fopEn_enemy_c*)&i_this->actor;
    cXyz work, center, unused;

    i_this->mFrameCounter++;

    for (int i = 0; i < 4; i++) {
        if (i_this->mTimers[i] != 0) {
            i_this->mTimers[i]--;
        }
    }

    if (i_this->mInvulnerabilityTimer != 0) {
        i_this->mInvulnerabilityTimer--;
    }

    if (i_this->mSplashTimer != 0) {
        i_this->mSplashTimer--;
    }

    J3DModel* model = i_this->mpMorf->getModel();

    action(i_this);

    if (i_this->mAction == ACTION_STANDBY) {
        return 1;
    }

    if (i_this->mAction == ACTION_WOLFBITE) {
        MTXCopy(daPy_getLinkPlayerActorClass()->getWolfMouthMatrix(), mDoMtx_stack_c::get());
        mDoMtx_stack_c::multVecZero(&actor->current.pos);
        mDoMtx_stack_c::YrotM(KREG_S(0));
        mDoMtx_stack_c::XrotM(KREG_S(1) + (s16)0x8000);
        mDoMtx_stack_c::ZrotM(KREG_S(2) + 2500);
        mDoMtx_stack_c::transM(KREG_F(0) + 10.0f, KREG_F(1) + -60.0f, KREG_F(2) + -20.0f);
        model->setBaseTRMtx(mDoMtx_stack_c::get());
    } else {
        mDoMtx_stack_c::transS(actor->current.pos.x, actor->current.pos.y + i_this->mCurrentPosYOffset, actor->current.pos.z);
        mDoMtx_stack_c::YrotM((s16)actor->shape_angle.y);
        mDoMtx_stack_c::XrotM((s16)actor->shape_angle.x);
        mDoMtx_stack_c::ZrotM(actor->shape_angle.z);
    }

    mDoMtx_stack_c::scaleM(l_HIO.basic_size, l_HIO.basic_size, l_HIO.basic_size);
    model->setBaseTRMtx(mDoMtx_stack_c::get());

    i_this->mpMorf->play(0, dComIfGp_getReverb(fopAcM_GetRoomNo(actor)));
    i_this->mpMorf->modelCalc();
    ke_set(i_this);
    anm_se_set(i_this);

    MTXCopy(model->getAnmMtx(JNT_BODY), *calc_mtx);
    work.set(TREG_F(0) + 30.0f, TREG_F(1), TREG_F(2));
    MtxPosition(&work, &actor->eyePos);
    actor->attention_info.position = actor->eyePos;
    actor->attention_info.position.y += 40.0f;

    work.set(BREG_F(0) + 10.0f, BREG_F(1) + 10.0f, BREG_F(2));
    MtxPosition(&work, &center);
    i_this->mSph1.SetC(center);
    i_this->mSph1.SetR((JREG_F(3) + 35.0f) * l_HIO.basic_size);
    dComIfG_Ccsp()->Set(&i_this->mSph1);

    if (i_this->mUnkFlag1 == 0) {
        center.x += 10000.0f;
    } else {
        work.set(TREG_F(8), TREG_F(9), TREG_F(10));
        MtxPosition(&work, &center);
        i_this->mUnkFlag1 = 0;
    }

    i_this->mSph2.SetC(center);
    i_this->mSph2.SetR((JREG_F(2) + 20.0f) * l_HIO.basic_size);
    dComIfG_Ccsp()->Set(&i_this->mSph2);

    if (i_this->mWaterFlag && i_this->mAnm == BCK_YG_RUN && (i_this->mpMorf->checkFrame(1.0f) || i_this->mpMorf->checkFrame(7.0f)) && cM_rndF(1.0f) < 0.5f) {
        static cXyz sc(0.3f, 0.3f, 0.3f);
        static u16 eff_id[4] = {
            ID_ZI_J_DOWNWTRA_A, ID_ZI_J_DOWNWTRA_B, ID_ZI_J_DOWNWTRA_C, ID_ZI_J_DOWNWTRA_D,
        };

        for (int i = 0; i < 4; i++) {
            i_this->mDownWtrAPrtcls[i] = dComIfGp_particle_set(i_this->mDownWtrAPrtcls[i], eff_id[i], &actor->current.pos, &actor->tevStr,
                                                            &actor->shape_angle, &sc, 0xFF, NULL, -1, NULL, NULL, NULL);
        }
    }

    if (i_this->mDeathFlag == 0) {
        i_this->mUnkPrtcl = dComIfGp_particle_set(i_this->mUnkPrtcl, dPa_RM(ID_ZF_S_YGCONST00), &actor->current.pos, NULL, NULL);
        JPABaseEmitter* emitter = dComIfGp_particle_getEmitter(i_this->mUnkPrtcl);
        if (emitter != NULL) {
            emitter->setGlobalRTMatrix(model->getAnmMtx(JNT_BODY));
        }
    }

    cXyz i_effSize(0.5f, 0.5f, 0.5f);
    setMidnaBindEffect(actor, &i_this->mSound, &actor->eyePos, &i_effSize);

    return 1;
}

static int daE_YG_IsDelete(e_yg_class* i_this) {
    return 1;
}

static int daE_YG_Delete(e_yg_class* i_this) {
    fopAc_ac_c* actor = (fopAc_ac_c*)&i_this->actor;
    fpc_ProcID id = fopAcM_GetID(i_this);

    dComIfG_resDelete(&i_this->mPhase, "E_YG");

    if (i_this->mIsFirstSpawn) {
        l_initHIO = false;
        mDoHIO_DELETE_CHILD(l_HIO.id);
    }

    if (actor->heap != NULL) {
        i_this->mpMorf->stopZelAnime();
    }

    return 1;
}

static int useHeapInit(fopAc_ac_c* a_this) {
    e_yg_class* i_this = (e_yg_class*)a_this;

    i_this->mpMorf = new mDoExt_McaMorfSO((J3DModelData*)dComIfG_getObjectRes("E_YG", 0x13), NULL, NULL,
                                          (J3DAnmTransform*)dComIfG_getObjectRes("E_YG", 0xF), 2, 1.0f, 0, -1,
                                          &i_this->mSound, J3DMdlFlag_DifferedDLBuffer, 0x11000084);
    if (i_this->mpMorf == NULL || i_this->mpMorf->getModel() == NULL) {
        return 0;
    }

    if (!i_this->mLineMat.init(13, 10, 1)) {
        return 0;
    }

    return 1;
}

static cPhs__Step daE_YG_Create(fopAc_ac_c* actor) {
    e_yg_class* i_this = (e_yg_class*)actor;

    fopAcM_ct(&i_this->actor, e_yg_class);

    cPhs__Step phase = (cPhs__Step)dComIfG_resLoad(&i_this->mPhase, "E_YG");
    if (phase == cPhs_COMPLEATE_e) {
        OS_REPORT("E_YG PARAM %x\n", fopAcM_GetParam(actor));

        s32 swBit = (fopAcM_GetParam(actor) & 0xFF0000) >> 16;
        if (swBit != 0xFF) {
            if (dComIfGs_isSwitch(swBit, fopAcM_GetRoomNo(actor))) {
                return cPhs_ERROR_e;
            }
        }

        i_this->arg0 = fopAcM_GetParam(actor);
        i_this->arg1 = (fopAcM_GetParam(actor) & 0xff00) >> 8;
        i_this->mSwBit = (fopAcM_GetParam(actor) & 0xff000000) >> 24;

        OS_REPORT("E_YG//////////////E_YG SET 1 !!\n");

        if (!fopAcM_entrySolidHeap(actor, useHeapInit, 0x3c60)) {
            OS_REPORT("//////////////E_YG SET NON !!\n");
            return cPhs_ERROR_e;
        }

        if (!l_initHIO) {
            i_this->mIsFirstSpawn = 1;
            l_initHIO = true;
            l_HIO.id = mDoHIO_CREATE_CHILD("グース", &l_HIO);
        }

        fopAcM_SetMtx(actor, i_this->mpMorf->getModel()->getBaseTRMtx());

        actor->field_0x560 = actor->health = 1;

        static dCcD_SrcSph cc_sph_src = {
            {
                {0x0, {{0x0, 0x0, 0x0}, {0xdafbfdff, 0x3}, 0x75}}, // mObj
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
                {0x0, {{AT_TYPE_CSTATUE_SWING, 0x1, 0xd}, {0x0, 0x0}, 0x0}}, // mObj
                #if DEBUG
                {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0}, // mGObjAt
                #else
                {dCcD_SE_SOFT_BODY, 0x0, 0x0, 0x0, 0x0}, // mGObjAt
                #endif
                {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x2}, // mGObjTg
                {0x0}, // mGObjCo
            }, // mObjInf
            {
                {{0.0f, 0.0f, 0.0f}, 20.0f} // mSph
            } // mSphAttr
        };

        i_this->mStts.Init(30, 0, actor);
        i_this->mSph1.Set(cc_sph_src);
        i_this->mSph1.SetStts(&i_this->mStts);
        i_this->mSph2.Set(at_sph_src);
        i_this->mSph2.SetStts(&i_this->mStts);
        i_this->mBgc.Set(fopAcM_GetPosition_p(actor), fopAcM_GetOldPosition_p(actor), actor, 1,
                         &i_this->mAcchCir, fopAcM_GetSpeed_p(actor), NULL, NULL);
        i_this->mAcchCir.SetWall(30.0f, 41.0f);

        i_this->mSound.init(&actor->current.pos, &actor->eyePos, 3, 1);
        i_this->mSound.setEnemyName("E_yg");

        i_this->mAtInfo.mpSound = &i_this->mSound;

        i_this->mFrameCounter = (s16)cM_rndF(65535.0f);

        if (i_this->mSwBit != 0xFF) {
            if (!dComIfGs_isSwitch(i_this->mSwBit, fopAcM_GetRoomNo(actor))) {
                i_this->mDispFlag = 1;
                i_this->mAction = ACTION_STANDBY;
            }

            i_this->mUnkFlag2 = 1;
        }

        i_this->mDistance = l_HIO.pl_recognition_dist;

        /* If Phantom Zant actor exists */
        if (fopAcM_SearchByName(PROC_E_PZ) != 0) {
            i_this->mDistance = 10000.0f;
            i_this->mUnkFlag2 = 1;
        }

        daE_YG_Execute(i_this);
    }

    return phase;
}

AUDIO_INSTANCES;

static actor_method_class l_daE_YG_Method = {
    (process_method_func)daE_YG_Create,
    (process_method_func)daE_YG_Delete,
    (process_method_func)daE_YG_Execute,
    (process_method_func)daE_YG_IsDelete,
    (process_method_func)daE_YG_Draw,
};

extern actor_process_profile_definition g_profile_E_YG = {
  fpcLy_CURRENT_e,       // mLayerID
  7,                     // mListID
  fpcPi_CURRENT_e,       // mListPrio
  PROC_E_YG,             // mProcName
  &g_fpcLf_Method.base, // sub_method
  sizeof(e_yg_class),    // mSize
  0,                     // mSizeOther
  0,                     // mParameters
  &g_fopAc_Method.base,  // sub_method
  190,                   // mPriority
  &l_daE_YG_Method,      // sub_method
  0x00050120,            // mStatus
  fopAc_ENEMY_e,         // mActorType
  fopAc_CULLBOX_0_e,     // cullType
};
