/**
 * @file d_a_e_bu.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_e_bu.h"
#include "f_op/f_op_actor_enemy.h"

class daE_BU_HIO_c : public JORReflexible {
public:
    daE_BU_HIO_c();
    virtual ~daE_BU_HIO_c() {}

    void genMessage(JORMContext* ctx);

    /* 0x04 */ s8 id;
    /* 0x08 */ f32 base_size;
    /* 0x0C */ f32 fly_speed;
    /* 0x10 */ f32 attack_range;
    /* 0x14 */ f32 attack_speed;
    /* 0x18 */ f32 dash_speed;
};

enum daE_BU_TYPE {
    TYPE_NORMAL,
    TYPE_FIRE,
    TYPE_ICE,
};

enum daE_BU_ACTION {
    ACTION_WAIT,
    ACTION_FIGHT_FLY,
    ACTION_FIGHT,
    ACTION_ATTACK,
    ACTION_FLY = 5,
    ACTION_PATH_FLY,
    ACTION_CHANCE = 10,
    ACTION_HEAD = 15,
    ACTION_DEAD = 21,
};

daE_BU_HIO_c::daE_BU_HIO_c() {
    id = -1;
    base_size = 1.5f;
    fly_speed = 15.0f;
    attack_range = 250.0f;
    attack_speed = 15.0f;
    dash_speed = 40.0f;
}

static void bu_disappear(fopAc_ac_c* i_this) {
    fopAcM_createDisappear(i_this, &i_this->current.pos, 6, 0, 0x1C);

    int swbit = (fopAcM_GetParam(i_this) & 0xFF000000) >> 0x18;
    if (swbit != 0xFF) {
        dComIfGs_onSwitch(swbit, fopAcM_GetRoomNo(i_this));
    }
}

static void anm_init(e_bu_class* i_this, int i_anm, f32 i_morf, u8 i_mode, f32 i_speed) {
    i_this->modelMorf->setAnm((J3DAnmTransform*)dComIfG_getObjectRes("E_BU", i_anm), i_mode, i_morf, i_speed, 0.0f, -1.0f);
    i_this->anm = i_anm;
}

static int nodeCallBack(J3DJoint* i_joint, int param_1) {
    if (param_1 == 0) {
        int jnt_no = i_joint->getJntNo();
        J3DModel* model = j3dSys.getModel();
        e_bu_class* a_this = (e_bu_class*)model->getUserArea();

        if (a_this != NULL) {
            if (jnt_no == TREG_S(0)) {
                MTXCopy(model->getAnmMtx(jnt_no), *calc_mtx);
                cMtx_YrotM(*calc_mtx, TREG_S(1));
                cMtx_XrotM(*calc_mtx, TREG_S(2));
                cMtx_ZrotM(*calc_mtx, TREG_S(3));
                model->setAnmMtx(jnt_no, *calc_mtx);
                MTXCopy(*calc_mtx, J3DSys::mCurrentMtx);
            }

            if (jnt_no == 17) {
                MTXCopy(model->getAnmMtx(jnt_no), *calc_mtx);
                cMtx_ZrotM(*calc_mtx, a_this->jaw_rot);
                model->setAnmMtx(jnt_no, *calc_mtx);
                MTXCopy(*calc_mtx, J3DSys::mCurrentMtx);
            }
        }
    }

    return 1;
}

static u8 hio_set;

static daE_BU_HIO_c l_HIO;

static int daE_BU_Draw(e_bu_class* i_this) {
    fopAc_ac_c* actor = &i_this->enemy;
    J3DModel* model = i_this->modelMorf->getModel();

    g_env_light.settingTevStruct(0, &actor->current.pos, &actor->tevStr);
    g_env_light.setLightTevColorType_MAJI(model, &actor->tevStr);

    i_this->modelMorf->entryDL();
    dComIfGd_setSimpleShadow(&actor->current.pos, i_this->acch.GetGroundH(), (30.0f + TREG_F(11)) * l_HIO.base_size, i_this->acch.m_gnd, 0, 1.0f, dDlst_shadowControl_c::getSimpleTex());
    return 1;
}

static BOOL other_bg_check(e_bu_class* i_this, fopAc_ac_c* i_other) {
    fopAc_ac_c* actor = &i_this->enemy;
    dBgS_LinChk linchk;
    cXyz sp28;
    cXyz startpos;
    cXyz endpos;

    endpos = i_other->current.pos;
    endpos.y += 100.0f;

    startpos = actor->current.pos;
    startpos.y = actor->eyePos.y;

    linchk.Set(&startpos, &endpos, actor);
    if (dComIfG_Bgsp().LineCross(&linchk)) {
        return TRUE;
    }

    return FALSE;
}

static BOOL pl_check(e_bu_class* i_this, f32 i_range, s16 i_angle) {
    fopAc_ac_c* actor = &i_this->enemy;
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    
    if (i_this->dist_to_player < i_range) {
        s16 temp_r28 = actor->shape_angle.y - i_this->angle_to_player;
        if ((i_angle == 1 || (temp_r28 < i_angle && temp_r28 > (s16)-i_angle)) && !other_bg_check(i_this, player)) {
            return TRUE;
        }
    }

    return FALSE;
}

static void damage_check(e_bu_class* i_this) {
    fopAc_ac_c* actor = &i_this->enemy;
    daPy_py_c* player = (daPy_py_c*)dComIfGp_getPlayer(0);
    
    if (i_this->invulnerabilityTimer == 0) {
        i_this->ccStts.Move();

        if (i_this->ccSph.ChkTgHit()) {
            OS_REPORT("E_BU HP %d\n", actor->health);
            i_this->atInfo.mpCollider = i_this->ccSph.GetTgHitObj();
            cc_at_check(actor, &i_this->atInfo);
            OS_REPORT("E_BU AP %d\n", i_this->atInfo.mAttackPower);

            if (i_this->atInfo.mpCollider->ChkAtType(AT_TYPE_WOLF_ATTACK | AT_TYPE_WOLF_CUT_TURN | AT_TYPE_10000000 | AT_TYPE_MIDNA_LOCK)) {
                i_this->invulnerabilityTimer = 20;
            } else {
                i_this->invulnerabilityTimer = 10;
            }

            if (actor->health <= 0) {
                i_this->sound.startCreatureVoice(Z2SE_EN_BU_V_DEAD, -1);
                i_this->modelMorf->setPlaySpeed(0.2f);
                i_this->is_dead = TRUE;
                i_this->hit_speed = 80.0f + TREG_F(13);
                i_this->hit_rot = i_this->atInfo.mHitDirection.y;
            } else {
                i_this->sound.startCreatureVoice(Z2SE_EN_BU_V_DMG, -1);
                i_this->action = ACTION_HEAD;
                i_this->mode = 0;

                if (i_this->atInfo.mpCollider->ChkAtType(AT_TYPE_HOOKSHOT)) {
                    actor->speedF = 0.0f;
                    i_this->head_rot_y = i_this->atInfo.mHitDirection.y;
                } else if (player->getCutType() == 10 && player->checkCutJumpCancelTurn()) {
                    i_this->invulnerabilityTimer = NREG_S(7) + 3;
                    actor->speedF = 0.0f;
                } else {
                    i_this->head_rot_y = i_this->atInfo.mHitDirection.y + 0x8000;
                    actor->speedF = 40.0f + AREG_F(0);
                }
            }
        }

        if (actor->health <= 1) {
            actor->health = 0;
            i_this->ccSph.SetTgHitMark((CcG_Tg_HitMark)3);
        }
    }
}

static BOOL path_check(e_bu_class* i_this) {
    static u8 check_index[255];

    if (i_this->ppd != NULL) {
        dPnt* pnt;
        fopAc_ac_c* actor = &i_this->enemy;
        dBgS_LinChk linchk;
        cXyz startpos;
        cXyz endpos;

        startpos = actor->current.pos;
        startpos.y += 100.0f;

        pnt = i_this->ppd->m_points;
        for (int i = 0; i < i_this->ppd->m_num; i++, pnt++) {
            endpos.x = pnt->m_position.x;
            endpos.y = 100.0f + pnt->m_position.y;
            endpos.z = pnt->m_position.z;
            linchk.Set(&startpos, &endpos, actor);

            if (!dComIfG_Bgsp().LineCross(&linchk)) {
                check_index[i] = TRUE;
            } else {
                check_index[i] = FALSE;
            }
        }
    
        f32 pnt_search_range = 0.0f;
        u8 pnt_found = FALSE;

        for (int i = 0; i < 100; i++, pnt_search_range += 50.0f) {
            pnt = i_this->ppd->m_points;

            for (int j = 0; j < i_this->ppd->m_num; j++, pnt++) {
                if (check_index[j]) {
                    Vec pnt_vec;
                    pnt_vec.x = actor->current.pos.x - pnt->m_position.x;
                    pnt_vec.y = actor->current.pos.y - pnt->m_position.y;
                    pnt_vec.z = actor->current.pos.z - pnt->m_position.z;

                    if (JMAFastSqrt(SQUARE(pnt_vec.x) + SQUARE(pnt_vec.y) + SQUARE(pnt_vec.z)) < pnt_search_range) {
                        i_this->now_path_pntNo = j - i_this->path_step;
                        if (i_this->now_path_pntNo >= (s8)i_this->ppd->m_num) {
                            i_this->now_path_pntNo = i_this->ppd->m_num;
                        } else if (i_this->now_path_pntNo < 0) {
                            i_this->now_path_pntNo = 0;
                        }
                        pnt_found = TRUE;
                        break;
                    }
                }
            }

            if (pnt_found) {
                break;
            }
        }

        if (!pnt_found) {
            i_this->field_0x5b8 = 0;
        } else {
            i_this->field_0x5b8 = i_this->pathNo + 1;
            return TRUE;
        }
    }

    return FALSE;
}

static void fly_move(e_bu_class* i_this) {
    fopAc_ac_c* actor = &i_this->enemy;
    cXyz sp8;
    f32 dist_x = i_this->move_pos.x - actor->current.pos.x;
    f32 dist_y = i_this->move_pos.y - actor->current.pos.y;
    f32 dist_z = i_this->move_pos.z - actor->current.pos.z;
    
    s16 temp_r28 = cM_atan2s(dist_x, dist_z);
    s16 temp_r27 = -cM_atan2s(dist_y, JMAFastSqrt(SQUARE(dist_x) + SQUARE(dist_z)));
    i_this->field_0x694 = 2000.0f;

    cLib_addCalcAngleS2(&actor->current.angle.y, temp_r28, (TREG_S(3) + 10), (i_this->field_0x694 * i_this->field_0x690));
    cLib_addCalcAngleS2(&actor->current.angle.x, temp_r27, (TREG_S(3) + 10), (i_this->field_0x694 * i_this->field_0x690));
    cLib_addCalc2(&i_this->field_0x690, 1.0f, 1.0f, 0.04f);

    sp8.x = 0.0f;
    sp8.y = 0.0f;
    sp8.z = actor->speedF;
    cMtx_YrotS(*calc_mtx, actor->current.angle.y);
    cMtx_XrotM(*calc_mtx, actor->current.angle.x);
    MtxPosition(&sp8, &actor->speed);
    actor->current.pos += actor->speed;
}

static void e_bu_wait(e_bu_class* i_this) {
    fopAc_ac_c* actor = &i_this->enemy;
    int anm_frame = i_this->modelMorf->getFrame();

    switch (i_this->mode) {
    case 0:
        anm_init(i_this, 6, 1.0f, 0, 0.0f);
        i_this->mode = 1;
        break;
    case 1:
        if (pl_check(i_this, i_this->pl_search_range, 1)) {
            i_this->mode = 2;
            i_this->modelMorf->setPlaySpeed(1.0f);
        }
        break;
    case 2:
        if (anm_frame == 36) {
            i_this->sound.startCreatureSound(Z2SE_EN_BU_WING, 0, -1);
        }

        if (anm_frame >= 37) {
            cLib_addCalc2(&actor->current.pos.y, 100.0f + actor->home.pos.y, 0.05f, 20.0f);
        }

        if (anm_frame >= JREG_S(0) + 41) {
            i_this->mode = 3;
            anm_init(i_this, 7, 1.0f + JREG_F(12), 2, 1.0f + cM_rndF(0.1f));
            actor->current.pos.y += 35.0f + JREG_F(13);
            i_this->timers[0] = 30;
        }
        break;
    case 3:
        cLib_addCalc2(&actor->current.pos.y, 100.0f + actor->home.pos.y, 0.05f, 20.0f);

        if (i_this->timers[0] == 0) {
            i_this->action = ACTION_FIGHT_FLY;
            i_this->mode = 1;
        }
        break;
    }
}

static void e_bu_fight_fly(e_bu_class* i_this) {
    fopAc_ac_c* actor = &i_this->enemy;
    fopAc_ac_c* player = dComIfGp_getPlayer(0);

    switch (i_this->mode) {
    case 0:
        anm_init(i_this, 7, 10.0f, 2, 1.0f + cM_rndF(0.1f));
        i_this->mode = 1;
        break;
    case 1:
        if (!pl_check(i_this, 2000.0f, 1)) {
            if (!path_check(i_this)) {
                i_this->action = ACTION_FLY;
                i_this->mode = 1;
            } else {
                i_this->action = ACTION_PATH_FLY;
                i_this->mode = 1;
            }
        } else if (pl_check(i_this, l_HIO.attack_range, 1)) {
            i_this->action = ACTION_FIGHT;
            i_this->mode = 0;
        }
        break;
    }

    cLib_addCalc2(&actor->speedF, l_HIO.fly_speed, 1.0f, 0.1f * l_HIO.fly_speed);
    i_this->move_pos = player->current.pos;
    fly_move(i_this);
}

static void e_bu_fight(e_bu_class* i_this) {
    fopAc_ac_c* actor = &i_this->enemy;
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    cXyz sp24;
    cXyz sp18;
    s16 sp8 = player->shape_angle.y;

    switch (i_this->mode) {
    case 0:
        i_this->modelMorf->setPlaySpeed(1.0f);
        i_this->mode = 1;
        i_this->timers[0] = 0;
        i_this->timers[1] = 30.0f + cM_rndF(100.0f);
        break;
    case 1:
        if (i_this->timers[0] == 0) {
            cMtx_YrotS(*calc_mtx, sp8 + (s16)cM_rndFX(12288.0f));
            sp24.x = 0.0f;
            sp24.y = 150.0f + cM_rndF(100.0f);
            sp24.z = 150.0f + cM_rndF(150.0f);
            MtxPosition(&sp24, &i_this->move_pos);
            i_this->move_pos += player->current.pos;
            sp24 = i_this->move_pos - actor->current.pos;
            cMtx_YrotS(*calc_mtx, cM_atan2s(sp24.x, sp24.z));
            cMtx_XrotM(*calc_mtx, -cM_atan2s(sp24.y, JMAFastSqrt((sp24.x * sp24.x) + (sp24.z * sp24.z))));

            sp24.x = 0.0f;
            sp24.y = 0.0f;
            sp24.z = l_HIO.attack_speed;
            MtxPosition(&sp24, &actor->speed);
            i_this->timers[0] = 10.0f + cM_rndF(30.0f);
            i_this->field_0x690 = 0.0f;
        }

        if (i_this->timers[1] == 0) {
            i_this->timers[1] = 30.0f + cM_rndF(100.0f);
            i_this->action = ACTION_ATTACK;
            i_this->mode = 0;
        }
        break;
    }

    cLib_addCalc2(&actor->current.pos.x, i_this->move_pos.x, 0.2f, i_this->field_0x690 * fabsf(actor->speed.x));
    cLib_addCalc2(&actor->current.pos.y, i_this->move_pos.y, 0.2f, i_this->field_0x690 * fabsf(actor->speed.y));
    cLib_addCalc2(&actor->current.pos.z, i_this->move_pos.z, 0.2f, i_this->field_0x690 * fabsf(actor->speed.z));
    cLib_addCalc2(&i_this->field_0x690, 1.0f, 1.0f, 0.1f);
    cLib_addCalcAngleS2(&actor->current.angle.y, i_this->angle_to_player, 4, 0x800);

    if (!pl_check(i_this, 2000.0f, 1)) {
        if (!path_check(i_this)) {
            i_this->action = ACTION_FLY;
            i_this->mode = 1;
        } else {
            i_this->action = ACTION_PATH_FLY;
            i_this->mode = 1;
        }
    }
}

static void e_bu_attack(e_bu_class* i_this) {
    fopAc_ac_c* actor = &i_this->enemy;
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    cXyz sp18;
    cXyz spC;
    f32 var_f31 = 0.0f;

    i_this->field_0x690 = 0.0f;

    switch (i_this->mode) {
    case 0:
        i_this->modelMorf->setPlaySpeed(2.0f);
        i_this->mode = 1;
        i_this->timers[1] = 20;
        break;
    case 1:
        i_this->move_pos = player->current.pos;
        i_this->move_pos.y += 120.0f;
        i_this->field_0x690 = 2.0f;

        if (i_this->timers[1] == 0) {
            i_this->mode = 2;
            i_this->timers[0] = BREG_S(7) + 10;
            i_this->sound.startCreatureVoice(Z2SE_EN_BU_V_ATK, -1);
        }

        cLib_addCalcAngleS2(&i_this->jaw_rot, -(3000.0f + (2000.0f * cM_ssin(i_this->lifetime * 0x3100))), 1, 0x1000);
        break;
    case 2:
        var_f31 = l_HIO.dash_speed;

        if (i_this->ccSph.ChkAtShieldHit()) {
            i_this->action = ACTION_CHANCE;
            i_this->mode = 0;
            i_this->hit_speed = 70.0f + TREG_F(19);
            i_this->hit_rot = actor->shape_angle.y;
            i_this->is_dead = FALSE;
        } else if (i_this->timers[0] == 0) {
            i_this->mode = 3;
        }
        break;
    case 3:
        if (actor->speedF <= 1.0f) {
            i_this->action = ACTION_FIGHT;
            i_this->mode = 0;
        }
        break;
    }

    cLib_addCalc2(&actor->speedF, var_f31, 1.0f, 0.2f * l_HIO.dash_speed);
    fly_move(i_this);
}

static void e_bu_fly(e_bu_class* i_this) {
    fopAc_ac_c* actor = &i_this->enemy;
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    cXyz sp14;

    switch (i_this->mode) {
    case 0:
        anm_init(i_this, 7, 10.0f, 2, 1.0f + cM_rndF(0.1f));
        i_this->mode = 1;
        break;
    case 1:
        if (i_this->timers[0] == 0) {
            i_this->move_pos.x = actor->home.pos.x + cM_rndFX(500.0f);
            i_this->move_pos.y = actor->home.pos.y + cM_rndFX(200.0f);
            i_this->move_pos.z = actor->home.pos.z + cM_rndFX(500.0f);
            sp14 = i_this->move_pos - actor->current.pos;
            cMtx_YrotS(*calc_mtx, cM_atan2s(sp14.x, sp14.z));
            cMtx_XrotM(*calc_mtx, -cM_atan2s(sp14.y, JMAFastSqrt((sp14.x * sp14.x) + (sp14.z * sp14.z))));
            sp14.x = 0.0f;
            sp14.y = 0.0f;
            sp14.z = l_HIO.attack_speed;
            MtxPosition(&sp14, &actor->speed);
            i_this->timers[0] = 10.0f + cM_rndF(30.0f);
            i_this->field_0x690 = 0.0f;
        }
        break;
    }

    cLib_addCalc2(&actor->speedF, l_HIO.fly_speed, 1.0f, 0.3f * l_HIO.fly_speed);
    fly_move(i_this);

    if (pl_check(i_this, i_this->pl_search_range, 1)) {
        i_this->action = ACTION_FIGHT_FLY;
        i_this->mode = 1;
    }
}

static void e_bu_path_fly(e_bu_class* i_this) {
    fopAc_ac_c* actor = &i_this->enemy;
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    cXyz sp18;
    dPnt* pnt;

    switch (i_this->mode) {
    case 0:
        anm_init(i_this, 7, 3.0f, 2, 1.0f + cM_rndF(0.1f));
        i_this->mode = 1;
        /* fallthrough */
    case 1:
        i_this->now_path_pntNo += i_this->path_step;
        if (i_this->now_path_pntNo >= (s8)i_this->ppd->m_num) {
            if (dPath_ChkClose(i_this->ppd)) {
                i_this->now_path_pntNo = 0;
            } else {
                i_this->path_step = -1;
                i_this->now_path_pntNo = i_this->ppd->m_num - 2;
            }

            int next_path_id = i_this->ppd->m_nextID;
            if (next_path_id != 0xFFFF) {
                i_this->ppd = dPath_GetRoomPath(next_path_id, fopAcM_GetRoomNo(actor));
                JUT_ASSERT(919, i_this->ppd != NULL);
            }
        } else if (i_this->now_path_pntNo < 0) {
            i_this->path_step = 1;
            i_this->now_path_pntNo = 1;
        }
        /* fallthrough */
    case 2:
        i_this->mode = 3;
        
        pnt = i_this->ppd->m_points;
        pnt += i_this->now_path_pntNo;

        i_this->field_0x690 = TREG_F(7);
        i_this->move_pos.x = pnt->m_position.x + cM_rndFX(150.0f);
        i_this->move_pos.y = pnt->m_position.y + cM_rndFX(150.0f);
        i_this->move_pos.z = pnt->m_position.z + cM_rndFX(150.0f);
        break;
    case 3:
        sp18 = i_this->move_pos - actor->current.pos;
        if (sp18.abs() < 200.0f + (10.0f * TREG_F(10))) {
            i_this->mode = 1;
        }
        break;
    }

    cLib_addCalc2(&actor->speedF, l_HIO.fly_speed, 1.0f, 0.3f * l_HIO.fly_speed);
    fly_move(i_this);

    if (pl_check(i_this, i_this->pl_search_range, 1)) {
        i_this->action = ACTION_FIGHT_FLY;
        i_this->mode = 1;
    }
}

static s8 e_bu_chance(e_bu_class* i_this) {
    fopAc_ac_c* actor = &i_this->enemy;
    s8 is_smoke_eff = FALSE;

    switch (i_this->mode) {
    case 0:
        anm_init(i_this, 7, 3.0f, 2, 1.5f);
        i_this->mode = 1;
        i_this->timers[0] = 100.0f + cM_rndF(30.0f);
        actor->speed.set(0.0f, 0.0f, 0.0f);
        break;
    case 1:
        if (i_this->acch.ChkGroundHit()) {
            actor->speed.y = JREG_F(7) + (20.0f + cM_rndF(10.0f));
            actor->speed.x = cM_rndFX(20.0f + JREG_F(6));
            actor->speed.z = cM_rndFX(20.0f + JREG_F(6));

            if (cM_rndF(1.0f) < 0.5f) {
                i_this->field_0x69e.z = 0;
            } else {
                i_this->field_0x69e.z = -0x8000;
            }

            i_this->field_0x69e.y = cM_rndF(65536.0f);
            is_smoke_eff = TRUE;
            i_this->sound.startCreatureSound(Z2SE_EN_BU_NODMG_BOUND, 0, -1);
        }

        if (i_this->timers[0] == 0) {
            actor->current.angle.z = 0;
            i_this->action = ACTION_FIGHT;
            i_this->mode = 0;
            return is_smoke_eff;
        }
        break;
    }

    actor->current.pos += actor->speed;
    actor->speed.y -= 5.0f + JREG_F(8);
    cLib_addCalcAngleS2(&actor->current.angle.y, i_this->field_0x69e.y, 2, 0x1000);
    cLib_addCalcAngleS2(&actor->current.angle.z, i_this->field_0x69e.z, 1, 0x1000);
    return is_smoke_eff;
}

static s8 e_bu_head(e_bu_class* i_this) {
    fopAc_ac_c* actor = &i_this->enemy;
    cXyz sp20;
    cXyz sp14;
    s8 is_smoke_eff = FALSE;

    switch (i_this->mode) {
    case 0:
        anm_init(i_this, 8, 3.0f, 0, 1.0f);
        i_this->mode = 1;
        break;
    case 1:
        i_this->head_rot_x += 6000;

        if (i_this->acch.ChkGroundHit()) {
            actor->speedF *= 0.5f;
            actor->speed.y = 30.0f;
            i_this->mode = 2;
            i_this->head_rot_y += (s16)cM_rndFX(10000.0f);

            if (cM_rndF(1.0f) < 0.5f) {
                i_this->field_0x6bc = 0x4000;
            } else {
                i_this->field_0x6bc = -0x4000;
            }

            is_smoke_eff = TRUE;
            i_this->sound.startCreatureSound(Z2SE_EN_BU_DMG_BOUND, 20, -1);
        }
        break;
    case 2:
        i_this->head_rot_x += (s16)(actor->speedF * (400.0f + AREG_F(1)));

        if (!i_this->acch.ChkGroundHit()) {
            break;
        }

        i_this->mode = 3;
        actor->speed.y = 20.0f;
    case 3:
        i_this->head_rot_x += (s16)(actor->speedF * (400.0f + AREG_F(1)));
        i_this->head_rot_y += i_this->field_0x6b8;

        cLib_addCalc2(&actor->speedF, 10.0f, 1.0f, 2.0f + TREG_F(16));
        if (actor->speedF <= 10.0f) {
            i_this->field_0x6b8 = 0;
            if (i_this->head_rot_x <= 2000 && i_this->head_rot_x > -2000) {
                i_this->mode = 4;
                i_this->timers[1] = 30.0f + cM_rndF(20.0f);
                actor->speedF = 0.0f;
            }
        } else if (i_this->timers[2] == 0) {
            i_this->timers[2] = cM_rndF(10.0f);
            i_this->field_0x6b8 = cM_rndFX(4000.0f);

            actor->speed.y = cM_rndF(actor->speedF);
            if (actor->speed.y > 20.0f) {
                actor->speed.y = 20.0f;
            }
        }

        cLib_addCalcAngleS2(&actor->current.angle.z, i_this->field_0x6bc, 1, 0x400);
        break;
    case 4:
        cLib_addCalcAngleS2(&i_this->head_rot_x, 0, 1, 0x1000);
        cLib_addCalcAngleS2(&actor->current.angle.z, i_this->field_0x6bc, 1, 0x1000);

        if (i_this->timers[1] == 0) {
            i_this->mode = 10;
            anm_init(i_this, 5, 3.0f, 2, 1.0f);
        }
        break;
    case 10:
        if (i_this->timers[1] == 0) {
            i_this->timers[0] = 200.0f + cM_rndF(100.0f);
            i_this->timers[1] = 5.0f + cM_rndF(10.0f);
            i_this->mode = 12;
        }
        break;
    case 11:
        if (i_this->acch.ChkGroundHit()) {
            i_this->timers[1] = 5.0f + cM_rndF(10.0f);
            i_this->mode = 12;
            actor->speedF = 0.0f;
            is_smoke_eff = TRUE;
            i_this->sound.startCreatureSound(Z2SE_EN_BU_MOVE_BOUND, 0, -1);
        }
        break;
    case 12:
        if (i_this->timers[1] == 0) {
            if (i_this->timers[0] == 0) {
                i_this->action = ACTION_FIGHT;
                i_this->mode = 0;
                anm_init(i_this, 7, 10.0f, 2, 1.0f + cM_rndF(0.1f));
            } else {
                actor->current.angle.y += (s16)cM_rndFX(8000.0f);
                i_this->mode = 11;
                actor->speed.y = JREG_F(7) + (20.0f + cM_rndF(10.0f));
                actor->speedF = 10.0f + cM_rndF(10.0f);
            }
        }
        break;
    }

    if (i_this->mode < 10) {
        cMtx_YrotS(*calc_mtx, i_this->head_rot_y);
        if (i_this->timers[0] == 0 && i_this->acch.ChkWallHit()) {
            actor->speedF *= -0.4f;
            i_this->timers[0] = 30;
        }
        i_this->field_0x6a8 = (15.0f + AREG_F(3)) * l_HIO.base_size;
    } else {
        cLib_addCalcAngleS2(&i_this->head_rot_y, 0, 2, 0x800);
        cLib_addCalcAngleS2(&actor->current.angle.z, 0, 2, 0x800);
        cMtx_YrotS(*calc_mtx, actor->current.angle.y);
        cLib_addCalc2(&i_this->field_0x6a8, (20.0f + AREG_F(4)) * l_HIO.base_size, 1.0f, 2.0f);
    }

    sp20.x = 0.0f;
    sp20.y = 0.0f;
    sp20.z = actor->speedF;
    MtxPosition(&sp20, &sp14);
    actor->current.pos.x += sp14.x;
    actor->current.pos.z += sp14.z;
    actor->current.pos.y += actor->speed.y;
    actor->speed.y -= 7.0f + JREG_F(8);
    return is_smoke_eff;
}

static s8 action(e_bu_class* i_this) {
    fopAc_ac_c* actor = &i_this->enemy;
    daPy_py_c* player = (daPy_py_c*)dComIfGp_getPlayer(0);
    cXyz sp30;
    cXyz sp24;

    i_this->angle_to_player = fopAcM_searchPlayerAngleY(actor);
    i_this->dist_to_player = fopAcM_searchPlayerDistance(actor);

    damage_check(i_this);
    i_this->ccSph.OffAtVsPlayerBit();

    s8 do_smoke_eff = FALSE;
    s8 is_mtrl_eff = TRUE;
    s8 down_status = 0;

    switch (i_this->action) {
    case ACTION_WAIT:
        e_bu_wait(i_this);
        i_this->field_0x6a8 = 0.0f;
        is_mtrl_eff = FALSE;
        break;
    case ACTION_FIGHT_FLY:
        e_bu_fight_fly(i_this);
        break;
    case ACTION_FIGHT:
        e_bu_fight(i_this);
        break;
    case ACTION_ATTACK:
        e_bu_attack(i_this);
        i_this->ccSph.OnAtVsPlayerBit();
        break;
    case ACTION_FLY:
        e_bu_fly(i_this);
        break;
    case ACTION_PATH_FLY:
        e_bu_path_fly(i_this);
        break;
    case ACTION_CHANCE:
        do_smoke_eff = e_bu_chance(i_this);
        is_mtrl_eff = FALSE;
        down_status = 1;
        break;
    case ACTION_HEAD:
        do_smoke_eff = e_bu_head(i_this);
        is_mtrl_eff = FALSE;
        down_status = 1;
        break;
    case ACTION_DEAD:
        actor->current.pos.y += actor->speed.y;
        actor->speed.y -= 7.0f + JREG_F(8);
        i_this->invulnerabilityTimer = 10;

        if (i_this->timers[3] == 0) {
            bu_disappear(actor);
            fopAcM_delete(actor);
        }

        is_mtrl_eff = FALSE;
        down_status = 2;
        break;
    }

    fopEn_enemy_c* enemy = (fopEn_enemy_c*)actor;

    if (down_status == 1) {
        enemy->onDownFlg();
        enemy->setDownPos(&actor->current.pos);
    } else {
        enemy->offDownFlg();
    }

    if (fopAcM_checkHookCarryNow(actor)) {
        actor->speed.y = 0.0f;
        actor->speedF = 0.0f;
    }

    if (enemy->checkCutDownHitFlg()) {
        enemy->offCutDownHitFlg();
        enemy->offDownFlg();
        actor->health = 0;
        i_this->timers[3] = 10;
        i_this->action = ACTION_DEAD;

        i_this->sound.startCreatureVoice(Z2SE_EN_BU_V_DEAD, -1);
        i_this->modelMorf->setPlaySpeed(0.0f);
        i_this->ccSph.OffCoSetBit();
    }

    if (i_this->hit_speed > 0.1f) {
        sp30.x = 0.0f;
        sp30.y = 0.0f;
        sp30.z = -i_this->hit_speed;
        cMtx_YrotS(*calc_mtx, i_this->hit_rot);
        MtxPosition(&sp30, &sp24);
        actor->current.pos += sp24;

        cLib_addCalc0(&i_this->hit_speed, 1.0f, 5.0f);

        if (i_this->is_dead) {
            i_this->invulnerabilityTimer = 3;
            actor->shape_angle.y += 0x1300;
            actor->shape_angle.z += 0x1700;

            if (i_this->hit_speed <= 1.0f || i_this->acch.ChkWallHit()) {
                bu_disappear(actor);
                fopAcM_delete(actor);
            }
        }
    } else {
        cLib_addCalcAngleS2(&actor->shape_angle.y, actor->current.angle.y, 4, 0x2000);
        cLib_addCalcAngleS2(&actor->shape_angle.x, 0, 4, 0x2000);
        cLib_addCalcAngleS2(&actor->shape_angle.z, actor->current.angle.z, 4, 0x2000);
    }

    actor->current.pos.y -= i_this->field_0x6a8;
    actor->old.pos.y -= i_this->field_0x6a8;
    i_this->acch.CrrPos(dComIfG_Bgsp());
    actor->current.pos.y += i_this->field_0x6a8;
    actor->old.pos.y += i_this->field_0x6a8;

    if (down_status != 2) {
        cLib_addCalc2(&i_this->field_0x6a8, 30.0f * l_HIO.base_size, 1.0f, 1.0f);
    }

    if (player->current.pos.y - 30.0f < actor->current.pos.y && i_this->action != ACTION_WAIT) {
        if (fopAcM_otherBgCheck(player, actor)) {
            fopAcM_OffStatus(actor, 0);
            actor->attention_info.flags = 0;
        } else {
            fopAcM_OnStatus(actor, 0);
            actor->attention_info.flags = fopAc_AttnFlag_BATTLE_e;
        }
    } else {
        fopAcM_OffStatus(actor, 0);
        actor->attention_info.flags = 0;
    }

    if (do_smoke_eff) {
        fopAcM_effSmokeSet1(&i_this->field_0xa7c, &i_this->field_0xa80, &actor->current.pos, &actor->shape_angle, 0.8f + NREG_F(18), &actor->tevStr, 1);
    }

    cLib_addCalcAngleS2(&i_this->jaw_rot, 0, 2, 0x400);

    cXyz effsize(0.75f, 0.75f, 0.75f);
    setMidnaBindEffect((fopEn_enemy_c*)actor, &i_this->sound, &actor->eyePos, &effsize);
    return is_mtrl_eff;
}

static int daE_BU_Execute(e_bu_class* i_this) {
    fopAc_ac_c* actor = &i_this->enemy;
    cXyz sp34;
    cXyz sp28;

    i_this->lifetime++;

    for (int i = 0; i < 4; i++) {
        if (i_this->timers[i] != 0) {
            i_this->timers[i]--;
        }
    }

    if (i_this->invulnerabilityTimer != 0) {
        i_this->invulnerabilityTimer--;
    }

    s8 do_mtrl_eff = action(i_this);

    mDoMtx_stack_c::transS(actor->current.pos.x, actor->current.pos.y, actor->current.pos.z);
    mDoMtx_stack_c::YrotM(i_this->head_rot_y);
    mDoMtx_stack_c::XrotM(i_this->head_rot_x);
    mDoMtx_stack_c::YrotM(-i_this->head_rot_y);
    mDoMtx_stack_c::YrotM(actor->shape_angle.y);
    mDoMtx_stack_c::ZrotM(actor->shape_angle.z);
    mDoMtx_stack_c::scaleM(l_HIO.base_size, l_HIO.base_size, l_HIO.base_size);

    J3DModel* model = i_this->modelMorf->getModel();
    model->setBaseTRMtx(mDoMtx_stack_c::get());

    i_this->modelMorf->play(0, dComIfGp_getReverb(fopAcM_GetRoomNo(actor)));

    if (i_this->anm == 7 && i_this->modelMorf->checkFrame(4.0f)) {
        if (i_this->modelMorf->getPlaySpeed() > 1.2f) {
            i_this->sound.startCreatureSound(Z2SE_EN_BU_WING_L, 0, -1);
        } else {
            i_this->sound.startCreatureSound(Z2SE_EN_BU_WING, 0, -1);
        }
    }

    i_this->anm_frame = i_this->modelMorf->getFrame();
    i_this->modelMorf->modelCalc();

    actor->eyePos = actor->current.pos;
    actor->eyePos.y += 30.0f;

    actor->attention_info.position = actor->eyePos;
    actor->attention_info.position.y += 20.0f;

    sp34.set(0.0f, 0.0f, 0.0f);
    MtxPosition(&sp34, &sp28);

    if (i_this->invulnerabilityTimer != 0) {
        sp28.z += 30000.0f;
    }

    i_this->ccSph.SetC(sp28);
    i_this->ccSph.SetR((30.0f + JREG_F(3)) * l_HIO.base_size);
    dComIfG_Ccsp()->Set(&i_this->ccSph);

    if (do_mtrl_eff && i_this->type != TYPE_NORMAL) {
        i_this->field_0x6c0 = actor->current.pos - actor->old.pos;
        i_this->field_0x6c0 *= 0.8f;

        if (i_this->type == TYPE_FIRE) {
            i_this->sound.startCreatureSoundLevel(Z2SE_EN_BU_LV_FIRE, 0, -1);

            for (int i = 0; i < 4; i++) {
                static u16 fire_name[] = {0x821D, 0x821E, 0x821F, 0x8220};
                i_this->mtrl_emitter_id[i] = dComIfGp_particle_set(i_this->mtrl_emitter_id[i], fire_name[i], &actor->current.pos, NULL, NULL);
                
                JPABaseEmitter* emitter = dComIfGp_particle_getEmitter(i_this->mtrl_emitter_id[i]);
                if (emitter != NULL) {
                    emitter->setGlobalSRTMatrix(i_this->modelMorf->getModel()->getAnmMtx(0));
                    emitter->setParticleCallBackPtr(dPa_control_c::getParticleTracePCB());
                    emitter->setUserWork((uintptr_t)&i_this->field_0x6c0);
                }
            }
        } else {
            for (int i = 0; i < 3; i++) {
                static u16 ice_name[] = {0x8221, 0x8222, 0x8223};
                i_this->mtrl_emitter_id[i] = dComIfGp_particle_set(i_this->mtrl_emitter_id[i], ice_name[i], &actor->current.pos, NULL, NULL);
                
                JPABaseEmitter* emitter = dComIfGp_particle_getEmitter(i_this->mtrl_emitter_id[i]);
                if (emitter != NULL) {
                    emitter->setGlobalSRTMatrix(i_this->modelMorf->getModel()->getAnmMtx(0));
                    emitter->setParticleCallBackPtr(dPa_control_c::getParticleTracePCB());
                    emitter->setUserWork((uintptr_t)&i_this->field_0x6c0);
                }
            }
        }
    }

    return 1;
}

static int daE_BU_IsDelete(e_bu_class* i_this) {
    return 1;
}

static int daE_BU_Delete(e_bu_class* i_this) {
    fopAc_ac_c* actor = &i_this->enemy;
    fpc_ProcID id = fopAcM_GetID(actor);
    dComIfG_resDelete(&i_this->phase, "E_BU");

    if (i_this->HIOInit != 0) {
        hio_set = 0;
        mDoHIO_DELETE_CHILD(l_HIO.id);
    }

    if (actor->heap != NULL) {
        i_this->modelMorf->stopZelAnime();
    }

    return 1;
}

static int useHeapInit(fopAc_ac_c* i_this) {
    e_bu_class* a_this = (e_bu_class*)i_this;

    a_this->modelMorf = new mDoExt_McaMorfSO((J3DModelData*)dComIfG_getObjectRes("E_BU", 11), NULL, NULL, (J3DAnmTransform*)dComIfG_getObjectRes("E_BU", 7), 2, 1.0f, 0, -1, &a_this->sound, 0x80000, 0x11000084);
    if (a_this->modelMorf == NULL || a_this->modelMorf->getModel() == NULL) {
        return 0;
    }

    J3DModel* model = a_this->modelMorf->getModel();
    model->setUserArea((uintptr_t)a_this);

    for (u16 i = 0; i < model->getModelData()->getJointNum(); i++) {
        model->getModelData()->getJointNodePointer(i)->setCallBack(nodeCallBack);
    }

    return 1;
}

static int daE_BU_Create(fopAc_ac_c* i_this) {
    e_bu_class* a_this = (e_bu_class*)i_this;
    fopAcM_ct(i_this, e_bu_class);

    int phase_state = dComIfG_resLoad(&a_this->phase, "E_BU");
    if (phase_state == cPhs_COMPLEATE_e) {
        OS_REPORT("E_BU PARAM %x\n", fopAcM_GetParam(i_this));
        
        int swbit = (fopAcM_GetParam(i_this) & 0xFF000000) >> 0x18;
        if (swbit != 0xFF && dComIfGs_isSwitch(swbit, fopAcM_GetRoomNo(i_this))) {
            return cPhs_ERROR_e;
        }
    
        a_this->prm0 = fopAcM_GetParam(i_this);
        a_this->prm1 = (fopAcM_GetParam(i_this) & 0xF00) >> 8;
        a_this->type = (fopAcM_GetParam(i_this) & 0xF000) >> 0xC;
        a_this->pathNo = (fopAcM_GetParam(i_this) & 0xFF0000) >> 0x10;

        if (a_this->prm0 == 0xFF) {
            a_this->prm0 = 0;
        }

        if (a_this->prm1 == 0xFF) {
            a_this->prm1 = 10;
        }

        if (a_this->type == 0xF) {
            a_this->type = TYPE_NORMAL;
        }

        a_this->pl_search_range = 100.0f * a_this->prm1;
        OS_REPORT("E_BU//////////////E_BU SET 1 !!\n");

        if (!fopAcM_entrySolidHeap(i_this, useHeapInit, 0x15C0)) {
            OS_REPORT("//////////////E_BU SET NON !!\n");
            return cPhs_ERROR_e;
        }
    
        OS_REPORT("//////////////E_BU SET 2 !!\n");

        if (a_this->pathNo != 0xFF) {
            a_this->ppd = dPath_GetRoomPath(a_this->pathNo, fopAcM_GetRoomNo(i_this));
            if (a_this->ppd == NULL) {
                return cPhs_ERROR_e;
            }

            a_this->field_0x5b8 = a_this->pathNo + 1;
            a_this->path_step = 1;
            a_this->action = ACTION_PATH_FLY;
        } else if (a_this->prm0 == 1) {
            a_this->action = ACTION_FLY;
        }

        if (!hio_set) {
            a_this->HIOInit = TRUE;
            hio_set = TRUE;
            l_HIO.id = mDoHIO_CREATE_CHILD("バブル", &l_HIO);
        }

        i_this->attention_info.flags = fopAc_AttnFlag_BATTLE_e;
        fopAcM_SetMtx(i_this, a_this->modelMorf->getModel()->getBaseTRMtx());
        fopAcM_SetMin(i_this, -200.0f, -200.0f, -200.0f);
        fopAcM_SetMax(i_this, 200.0f, 200.0f, 200.0f);

        i_this->health = 40;
        i_this->field_0x560 = 40;

        static dCcD_SrcSph cc_sph_src = {
            {
                {0x0, {{AT_TYPE_CSTATUE_SWING, 0x1, 0xd}, {0xd8fbfdff, 0x3}, 0x75}}, // mObj
                {dCcD_SE_HARD_BODY, 0x0, 0x0, 0x0, 0x0}, // mGObjAt
                {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x2}, // mGObjTg
                {0x0}, // mGObjCo
            }, // mObjInf
            {
                {{0.0f, 0.0f, 0.0f}, 40.0f} // mSph
            } // mSphAttr
        };

        a_this->ccStts.Init(Z2SE_SY_TALK_CURSOR_OK, 0, i_this);
        a_this->ccSph.Set(cc_sph_src);
        a_this->ccSph.SetStts(&a_this->ccStts);

        if (a_this->type == TYPE_FIRE) {
            a_this->ccSph.SetAtType(AT_TYPE_100);
            a_this->ccSph.SetAtMtrl(dCcD_MTRL_FIRE);
        } else if (a_this->type == TYPE_ICE) {
            a_this->ccSph.SetAtType(AT_TYPE_100);
            a_this->ccSph.SetAtMtrl(dCcD_MTRL_ICE);
        }

        a_this->acch.Set(fopAcM_GetPosition_p(i_this), fopAcM_GetOldPosition_p(i_this), i_this, 1, &a_this->acchcir, fopAcM_GetSpeed_p(i_this), NULL, NULL);
        a_this->acchcir.SetWall(55.0f, 55.0f);

        a_this->sound.init(&i_this->current.pos, &i_this->eyePos, 3, 1);
        a_this->atInfo.mpSound = &a_this->sound;
        a_this->sound.setEnemyName("E_bu");
        a_this->atInfo.mPowerType = 4;

        daE_BU_Execute(a_this);
    }

    return phase_state;
}

static actor_method_class l_daE_BU_Method = {
    (process_method_func)daE_BU_Create,
    (process_method_func)daE_BU_Delete,
    (process_method_func)daE_BU_Execute,
    (process_method_func)daE_BU_IsDelete,
    (process_method_func)daE_BU_Draw,
};

actor_process_profile_definition g_profile_E_BU = {
    fpcLy_CURRENT_e,        // mLayerID
    7,                      // mListID
    fpcPi_CURRENT_e,        // mListPrio
    PROC_E_BU,              // mProcName
    &g_fpcLf_Method.base,  // sub_method
    sizeof(e_bu_class),     // mSize
    0,                      // mSizeOther
    0,                      // mParameters
    &g_fopAc_Method.base,   // sub_method
    181,                    // mPriority
    &l_daE_BU_Method,       // sub_method
    0x100C0120,             // mStatus
    fopAc_ENEMY_e,          // mActorType
    fopAc_CULLBOX_CUSTOM_e, // cullType
};
