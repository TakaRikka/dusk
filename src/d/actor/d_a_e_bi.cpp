/**
 * @file d_a_e_bi.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_e_bi.h"
#include "d/d_cc_d.h"
#include "d/d_bomb.h"
#include "d/actor/d_a_obj_fw.h"
#include "Z2AudioLib/Z2Instances.h"
#include "f_op/f_op_actor_enemy.h"

class daE_BI_HIO_c : public JORReflexible {
public:
    daE_BI_HIO_c();
    virtual ~daE_BI_HIO_c() {}

#if DEBUG
    void genMessage(JORMContext*);
#endif

    /* 爆弾虫 - Bomb Bug */
    /* 0x04 */ s8 no;
    /* 0x08 */ f32 basic_size;          // 基本サイズ - Basic Size
    /* 0x0C */ f32 search_range;        // サーチ範囲 - Search Range
    /* 0x10 */ f32 track_range;         // 追尾範囲 - Track Range
    /* 0x14 */ s16 time_to_get_going;   // 動き出すまでの時間 - Time To Get Going
    /* 0x18 */ f32 movement_spd;        // 移動速度 - Movement Speed
};

enum E_bi_RES_File_ID {
    /* BCK */
    /* 0x4 */ BCK_BI_APPEAR = 0x4,
    /* 0x5 */ BCK_BI_ATTACK,
    /* 0x6 */ BCK_BI_BOMBPOSE,
    /* 0x7 */ BCK_BI_MOVE,
    /* 0x8 */ BCK_BI_WAIT01,
    /* 0x9 */ BCK_BI_WAIT01TO02,
    /* 0xA */ BCK_BI_WAIT02,
    /* 0xB */ BCK_BI_WAIT02TO01,

    /* BMDR */
    /* 0xE */ BMDR_BI = 0xE,
    /* 0xF */ BMDR_BI_HOME,
};

enum Action {
    /* 0x0 */ ACTION_WAIT,
    /* 0x1 */ ACTION_UP,
    /* 0x2 */ ACTION_MOVE,
    /* 0x5 */ ACTION_EX = 0x5,
    /* 0x6 */ ACTION_WATER,
    /* 0x7 */ ACTION_DISAP,
};

daE_BI_HIO_c::daE_BI_HIO_c() {
    no = -1;
    basic_size = 1.0f;
    search_range = 400.0f;
    track_range = 600.0f;
    time_to_get_going = 30;
    movement_spd = 10.0f;
}

static void anm_init(e_bi_class* i_this, int i_index, f32 i_morf, u8 i_attr, f32 i_rate) {
    i_this->anm_p->setAnm((J3DAnmTransform*)dComIfG_getObjectRes("E_BI", i_index), i_attr, i_morf, i_rate, 0.0f, -1.0f);
    i_this->anm_no = i_index;
}

static int daE_BI_Draw(e_bi_class* i_this) {
    fopAc_ac_c* actor = &i_this->actor;

    if (i_this->ride_time == 0) {
        return 1;
    }
    
    g_env_light.settingTevStruct(0, &actor->current.pos, &actor->tevStr);
    J3DModel* model = i_this->anm_p->getModel();
    g_env_light.setLightTevColorType_MAJI(model, &actor->tevStr);
    J3DModelData* modelData = model->getModelData();
    modelData->getMaterialNodePointer(1)->getTevColor(1)->r = i_this->field_0x698;

    if (i_this->ignition_time != 0) {
        model->getModelData()->getMaterialNodePointer(0)->getShape()->hide();
        model->getModelData()->getMaterialNodePointer(2)->getShape()->hide();
        model->getModelData()->getMaterialNodePointer(1)->getShape()->show();
        model->getModelData()->getMaterialNodePointer(3)->getShape()->show();
    } else {
        model->getModelData()->getMaterialNodePointer(0)->getShape()->show();
        model->getModelData()->getMaterialNodePointer(2)->getShape()->show();
        model->getModelData()->getMaterialNodePointer(1)->getShape()->hide();
        model->getModelData()->getMaterialNodePointer(3)->getShape()->hide();
    }

    i_this->anm_p->entryDL();
    modelData->getMaterialNodePointer(1)->getTevColor(1)->r = 0;

    cXyz pos;
    pos.set(actor->current.pos.x, actor->current.pos.y + 100.0f + BREG_F(18), actor->current.pos.z);
    i_this->shadow_key = dComIfGd_setShadow(i_this->shadow_key, 1, model, &pos,
                                            BREG_F(19) + 800.0f, 0.0f, actor->current.pos.y,
                                            i_this->ObjAcch.GetGroundH(), i_this->ObjAcch.m_gnd,
                                            &actor->tevStr, 0, 1.0f,
                                            dDlst_shadowControl_c::getSimpleTex());

    return 1;
}

static BOOL pl_check(e_bi_class* i_this, f32 search_area) {
    fopAc_ac_c* actor = &i_this->actor;

    if (i_this->arg1 == 1) {
        return FALSE;
    }

    fopAc_ac_c* pl = dComIfGp_getPlayer(0);
    if (i_this->dis < search_area && !fopAcM_otherBgCheck(actor, pl)) {
        return TRUE;
    }

    return FALSE;
}

static void damage_check(e_bi_class* i_this) {
    fopAc_ac_c* actor = &i_this->actor;
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    fopAc_ac_c* child_actor;

    if (i_this->damage_time == 0) {
        i_this->Stts.Move();

        if (i_this->at_sph.ChkAtShieldHit()) {
            i_this->action = 5;
            i_this->mode = 0;
            i_this->damage_time = 60;
            actor->speedF = 0.0f;
            i_this->field_0x6a6 = i_this->target_angle + 0x8000;
        } else if (i_this->cc_cyl.ChkTgHit()) {
            i_this->at_info.mpCollider = i_this->cc_cyl.GetTgHitObj();
            at_power_check(&i_this->at_info);
            cc_at_check(actor, &i_this->at_info);

            if (i_this->at_info.mpCollider->ChkAtType(AT_TYPE_HOOKSHOT)) {
                player = dBomb_c::createEnemyBombHookshot(&actor->eyePos, &actor->current.angle, fopAcM_GetRoomNo(actor));
                if (player != NULL) {
                    child_actor = fopAcM_SearchByID(i_this->child_no);
                    if (child_actor != NULL) {
                        child_actor->parentActorID = fopAcM_GetID(player);
                    }

                    i_this->leaf_id = 1;
                }
            }else if (i_this->at_info.mpCollider->ChkAtType(AT_TYPE_BOOMERANG)) {
                player = dBomb_c::createEnemyBombBoomerang(&actor->eyePos, &actor->current.angle, fopAcM_GetRoomNo(actor));
                if (player != NULL) {
                    child_actor = fopAcM_SearchByID(i_this->child_no);
                    if (child_actor != NULL) {
                        child_actor->parentActorID = fopAcM_GetID(player);
                    }

                    i_this->leaf_id = 1;
                }
            } else if (i_this->ignition_time != 0 || i_this->at_info.mpCollider->ChkAtType(AT_TYPE_BOMB)) {
                dBomb_c::createNormalBombExplode(&actor->eyePos);
                i_this->leaf_id = 1;
            } else {
                i_this->action = 5;
                i_this->mode = 0;
                i_this->damage_time = 60;
                actor->speedF = 0.0f;

                if (i_this->at_info.mHitType == HIT_TYPE_STUN) {
                    i_this->field_0x6a6 = player->shape_angle.y;
                } else {
                    i_this->field_0x6a6 = i_this->target_angle + 0x8000;
                }
            }
        }
    }
}

static u8 hio_set;

static daE_BI_HIO_c l_HIO;

static void e_bi_wait(e_bi_class* i_this) {
    fopAc_ac_c* actor = &i_this->actor;
    cXyz pos;

    switch (i_this->mode) {
        case -2:
            anm_init(i_this, BCK_BI_APPEAR, 1.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
            i_this->mode = 1;
            break;

        case 0:
            anm_init(i_this, BCK_BI_WAIT01TO02, 3.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
            i_this->sound.startCreatureSound(Z2SE_EN_BI_CROUCH, 0, -1);
            i_this->mode = 1;
            break;

        case 1:
            if (i_this->anm_p->isStop()) {
                if (i_this->arg1 == 1) {
                    i_this->action = 1;
                    i_this->mode = 0;
                } else {
                    anm_init(i_this, BCK_BI_WAIT02, 2.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
                    i_this->mode = 2;
                    i_this->timer[0] = l_HIO.time_to_get_going;
                }
            }
            break;

        case 2:
            cLib_addCalc2(&actor->current.pos.x, actor->home.pos.x, 1.0f, 5.0f);
            cLib_addCalc2(&actor->current.pos.z, actor->home.pos.z, 1.0f, 5.0f);
            pos = actor->current.pos - actor->home.pos;
            pos.y = 0.0f;

            if (pos.abs() < 1.0f && !pl_check(i_this, l_HIO.search_range + 50.0f)) {
                i_this->action = 1;
                i_this->mode = 0;
            } else if (i_this->timer[0] == 0) {
                i_this->action = 2;
                i_this->mode = 0;
            }
            break;
    }
}

static void e_bi_up(e_bi_class* i_this) {
    fopAc_ac_c* actor = &i_this->actor;

    switch (i_this->mode) {
        case 0:
            anm_init(i_this, BCK_BI_WAIT02TO01, 3.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
            i_this->sound.startCreatureSound(Z2SE_EN_BI_STAND, 0, -1);
            i_this->mode = 1;
            break;

        case 1:
            if (i_this->anm_p->isStop()) {
                anm_init(i_this, BCK_BI_WAIT01, 2.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
                i_this->mode = 2;
            }
            break;

        case 2:
            if (pl_check(i_this, l_HIO.search_range)) {
                i_this->action = 0;
                i_this->mode = 0;
            }
            break;
    }

    cLib_addCalcAngleS2(&actor->current.angle.y, i_this->target_angle, 4, 0x800);
    cLib_addCalcAngleS2(&actor->shape_angle.y, actor->current.angle.y, 4, 0x2000);

    if (i_this->arg1 == 1 && i_this->mode >= 2) {
        fopAc_ac_c* actor_p = fopAcM_SearchByID(i_this->fw_id);
        if (actor_p != NULL && actor_p->field_0x567 != 0) {
            i_this->action = 7;
            i_this->mode = 0;
        }
    }
}

static void e_bi_move(e_bi_class* i_this) {
    fopAc_ac_c* actor = &i_this->actor;
    cXyz mae, ato;
    f32 movement_spd = 0.0f;
    mae = actor->home.pos - actor->current.pos;

    switch (i_this->mode) {
        case 0:
            if (i_this->timer[0] == 0) {
                anm_init(i_this, BCK_BI_MOVE, 3.0f, J3DFrameCtrl::EMode_LOOP, 3.0f);

                if (!pl_check(i_this, l_HIO.track_range)) {
                    i_this->mode = 5;
                } else {
                    i_this->mode = 1;
                }
            }
            break;

        case 1:
            movement_spd = l_HIO.movement_spd;
            i_this->target = i_this->target_angle;

            if (fopAcM_searchPlayerDistance(actor) < KREG_F(7) + 150.0f) {
                i_this->mode = 2;
                anm_init(i_this, BCK_BI_ATTACK, 3.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
                movement_spd = 0.0f;
                actor->speedF = movement_spd;
            } else if (!pl_check(i_this, l_HIO.track_range)) {
                i_this->mode = 5;
            }
            break;

        case 2:
            i_this->field_0x988 = 1;

            if (i_this->anm_p->isStop()) {
                i_this->mode = 0;
                i_this->timer[0] = cM_rndF(20.0f);
                anm_init(i_this, BCK_BI_WAIT02, 3.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
            }
            break;

        case 5:
            movement_spd = l_HIO.movement_spd;
            i_this->target = cM_atan2s(mae.x, mae.z);
            mae.y = 0.0f;

            if (i_this->ignition_time == 0 && mae.abs() < l_HIO.movement_spd * 1.5f) {
                i_this->action = 0;
                anm_init(i_this, BCK_BI_WAIT02, 2.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
                i_this->mode = 2;
                i_this->timer[0] = l_HIO.time_to_get_going;
            } else if (pl_check(i_this, l_HIO.search_range)) {
                i_this->mode = 0;
                i_this->timer[0] = 0;
            }
            break;
    }

    cLib_addCalcAngleS2(&actor->current.angle.y, i_this->target, 2, 0x800);
    cLib_addCalc2(&actor->speedF, movement_spd, 1.0f, l_HIO.movement_spd);
    cLib_addCalcAngleS2(&actor->shape_angle.y, actor->current.angle.y, 4, 0x2000);
}

static void e_bi_ex(e_bi_class* i_this) {
    fopAc_ac_c* actor = &i_this->actor;
    cXyz mae, ato;

    switch (i_this->mode) {
        case 0:
            i_this->mode = 1;
            i_this->ignition_time = 160;
            anm_init(i_this, BCK_BI_BOMBPOSE, 10.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
            f32 rnd;
            rnd = cM_rndF(0.2f) + 0.9f;
            i_this->field_0x6a2 = 0;
            i_this->field_0x6a4 = (3800.0f + JREG_F(5)) * rnd;
            i_this->field_0x6a8 = rnd * 20.0f;
            actor->speed.y = 23.0f;
            break;

        case 1:
            if (i_this->ObjAcch.ChkGroundHit()) {
                i_this->mode = 2;
                actor->speed.y = 10.0f;
            }
            // fallthrough
        case 2:
            cLib_addCalc0(&i_this->field_0x6a8, 1.0f, 1.0f + JREG_F(7));
            cLib_addCalcAngleS2(&i_this->field_0x6a4, 0, 1, 200 + JREG_S(6));
            break;
    }

    cMtx_YrotS(*calc_mtx, i_this->field_0x6a6);
    mae.x = 0.0f;
    mae.y = 0.0f;
    mae.z = i_this->field_0x6a8;
    MtxPosition(&mae, &ato);
    actor->current.pos += ato;
    i_this->field_0x6a2 += i_this->field_0x6a4;

    if (i_this->ignition_time != 0) {
        i_this->ignition_time--;
        i_this->sound.startCreatureSoundLevel(Z2SE_OBJ_BOMB_IGNITION, 0, -1);
        i_this->field_0x696 += (s16) 0x1100;

        if (i_this->ignition_time < 45) {
            i_this->field_0x696 += (s16) 0x1100;

            if (i_this->ignition_time < 30) {
                i_this->field_0x696 += (s16) 0x1100;
            }
        }

        i_this->field_0x698 = 128.0f - cM_scos(i_this->field_0x696) * 127.0f;

        if (i_this->ignition_time == 0) {
            dBomb_c::createNormalBombExplode(&actor->eyePos);
            i_this->leaf_id = 1;
        }

        static u16 ex_eff_id[5] = {
            0x0A0D,
            0x0A0E,
            0x0A0F,
            0x0A10,
            0x0A11,
        };

        for (int i = 0; i < 5; i++) {
            i_this->ex_eff[i] = dComIfGp_particle_set(i_this->ex_eff[i], ex_eff_id[i], &actor->current.pos, &actor->tevStr,
                                                           &actor->shape_angle, NULL, 0xFF, NULL, -1, NULL, NULL, NULL);
            JPABaseEmitter* emitter = dComIfGp_particle_getEmitter(i_this->ex_eff[i]);
            
            if (emitter != NULL) {
                MTXCopy(i_this->anm_p->getModel()->getAnmMtx(0), *calc_mtx);
                emitter->setGlobalRTMatrix(*calc_mtx);
            }
        }
    }

    cLib_addCalcAngleS2(&actor->shape_angle.y, actor->current.angle.y, 4, 0x2000);
}

static void e_bi_water(e_bi_class* i_this) {
    fopAc_ac_c* actor = &i_this->actor;
    cXyz mae, ato;
    mae = actor->home.pos - actor->current.pos;

    switch (i_this->mode) {
        case 0:
            anm_init(i_this, BCK_BI_MOVE, 3.0f, J3DFrameCtrl::EMode_LOOP, 3.0f);
            i_this->mode = 1;
            i_this->timer[0] = 20;
            actor->speed.y = 0.0f;
            // fallthrough
        case 1:
            if (i_this->timer[0] == 0) {
                i_this->anm_p->setPlaySpeed(0.0f);
                actor->current.pos.y += actor->speed.y;
                actor->speed.y -= 0.1f;

                if (actor->speed.y < -2.0f) {
                    actor->speed.y = -2.0f;
                }

                cLib_addCalc0(&actor->scale.x, 1.0f, 0.02f);
                cLib_addCalcAngleS2(&actor->shape_angle.x, 0x7FFF, 0x10, 0x800);

                if (actor->scale.x < 0.01f) {
                    i_this->leaf_id = 1;
                }
            }
            break;
    }

    actor->current.pos.x += actor->speed.x;
    actor->current.pos.z += actor->speed.z;

    i_this->ObjAcch.CrrPos(dComIfG_Bgsp());
    actor->speed.x *= 0.9f;
    actor->speed.z *= 0.9f;

    cXyz pos(actor->current.pos);
    pos.y = i_this->water_offset;

    if (fabsf(pos.y - actor->current.pos.y) < 70.0f) {
        fopAcM_effHamonSet(&i_this->field_0xbe8, &pos, KREG_F(2) + 1.0f, 0.2f);
    }
}

static void e_bi_disap(e_bi_class* i_this) {
    fopAc_ac_c* actor = &i_this->actor;

    switch (i_this->mode) {
        case 0:
            anm_init(i_this, BCK_BI_APPEAR, 10.0f, J3DFrameCtrl::EMode_NONE, 0.0f);
            i_this->disap_time = 40.0f;
            i_this->mode = 1;
            i_this->timer[0] = 10;
            break;

        case 1:
            if (i_this->timer[0] == 0) {
                i_this->disap_time -= 1.0f;

                if (i_this->disap_time < 0.0f) {
                    fopAcM_delete(actor);
                    return;
                }
            }
            break;
    }

    i_this->anm_p->setFrame(i_this->disap_time);
}

static BOOL water_check(e_bi_class* i_this) {
    fopAc_ac_c* actor = &i_this->actor;
    Vec vec;
    dBgS_ObjGndChk_Spl gnd_chk_spl;

    vec.x = actor->current.pos.x;
    vec.y = actor->current.pos.y + 300.0f;
    vec.z = actor->current.pos.z;
    gnd_chk_spl.SetPos(&vec);
    i_this->water_offset = dComIfG_Bgsp().GroundCross(&gnd_chk_spl);

    if (i_this->water_offset > actor->current.pos.y + 50.0f) {
        return TRUE;
    }

    return FALSE;
}

static void action(e_bi_class* i_this) {
    fopAc_ac_c* actor = &i_this->actor;
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    cXyz mae, ato;

    i_this->target_angle = fopAcM_searchPlayerAngleY(actor);
    i_this->dis = (actor->home.pos - player->current.pos).abs();

    damage_check(i_this);

    s8 bVar1 = 1;
    s8 bVar2 = 0;
    s8 bVar3 = 0;
    s8 bVar4 = 1;

    switch (i_this->action) {
        case ACTION_WAIT:
            e_bi_wait(i_this);
            bVar3 = 1;
            break;

        case ACTION_UP:
            e_bi_up(i_this);
            break;

        case ACTION_MOVE:
            e_bi_move(i_this);
            bVar2 = 1;
            break;

        case ACTION_EX:
            e_bi_ex(i_this);
            bVar2 = 1;
            bVar3 = 1;
            break;

        case ACTION_WATER:
            e_bi_water(i_this);
            bVar1 = 0;
            bVar3 = 0;
            bVar4 = 0;
            break;

        case ACTION_DISAP:
            e_bi_disap(i_this);
            break;
    }

    if (fopAcM_GetRoomNo(actor) == 3) {
        dBgS_LinChk lin_chk;
        mae = player->current.pos;
        mae.y += 150.0f;
        lin_chk.Set(&mae, &actor->eyePos, actor);

        if (dComIfG_Bgsp().LineCross(&lin_chk)) {
            bVar1 = 0;
        }
    }

    if (bVar1 != 0) {
        fopAcM_OnStatus(actor, 0);
        cLib_onBit<u32>(actor->attention_info.flags, fopAc_AttnFlag_BATTLE_e);
    } else {
        fopAcM_OffStatus(actor, 0);
        actor->attention_info.flags = 0;
    }

    if (bVar2 != 0) {
        cXyz* ccMoveP = i_this->Stts.GetCCMoveP();
        if (ccMoveP != NULL) {
            actor->current.pos.x += ccMoveP->x * 0.5f;
            actor->current.pos.y += ccMoveP->y * 0.5f;
            actor->current.pos.z += ccMoveP->z * 0.5f;
        }

        cMtx_YrotS(*calc_mtx, actor->current.angle.y);
        mae.x = 0.0f;
        mae.y = 0.0f;
        mae.z = actor->speedF;
        MtxPosition(&mae, &ato);
        actor->speed.x = ato.x;
        actor->speed.z = ato.z;
        actor->current.pos += actor->speed;
        actor->speed.y -= 5.0f;
        i_this->ObjAcch.CrrPos(dComIfG_Bgsp());
    }

    if (bVar3 != 0) {
        cLib_onBit<u32>(actor->attention_info.flags, fopAc_AttnFlag_CARRY_e);

        if (fopAcM_checkCarryNow(actor)) {
            cLib_offBit<u32>(actor->attention_info.flags, fopAc_AttnFlag_CARRY_e);
            fopAc_ac_c* bomb_p = dBomb_c::createEnemyBomb(&actor->current.pos, &actor->current.angle, fopAcM_GetRoomNo(actor));

            if (bomb_p == NULL) {
                OS_REPORT("//////////////LEAF ID 2???\n");
            } else {
                fopAc_ac_c* actor_p = fopAcM_SearchByID(i_this->child_no);
                if (actor_p != NULL) {
                    actor_p->parentActorID = fopAcM_GetID(bomb_p);

                    OS_REPORT("//////////////LEAF ID 1  %d\n", actor_p->parentActorID);
                }

                i_this->leaf_id = 1;

                OS_REPORT("//////////////LEAF ID 1???\n");
            }
        }

        cLib_offBit<u32>(actor->attention_info.flags, fopAc_AttnFlag_BATTLE_e);
    } else {
        cLib_offBit<u32>(actor->attention_info.flags, fopAc_AttnFlag_CARRY_e);
        cLib_onBit<u32>(actor->attention_info.flags, fopAc_AttnFlag_BATTLE_e);
    }

    if (bVar4 != 0 && water_check(i_this)) {
        i_this->action = 6;
        i_this->mode = 0;
        actor->current.pos.y = i_this->water_offset - (WREG_F(11) + 50.0f);
        cXyz pos(actor->current.pos);

        static cXyz sc(0.8f, 0.8f, 0.8f);
        static u16 w_eff_id[4] = {
            0x01B8, 0x01B9, 0x01BA, 0x01BB,
        };

        for (int i = 0; i < 4; i++) {
            i_this->w_eff[i] = dComIfGp_particle_set(i_this->w_eff[i], w_eff_id[i], &pos, &actor->tevStr,
                                                           &actor->shape_angle, &sc, 0xFF, NULL,
                                                           -1, NULL, NULL, NULL);
        }

        i_this->sound.startCreatureSound(Z2SE_CM_BODYFALL_WATER_S, 0, -1);
    }

    cXyz eff_size(0.75f, 0.75f, 0.75f);
    setMidnaBindEffect(&i_this->actor, &i_this->sound, &actor->eyePos, &eff_size);
}

static void* s_fw_sub(void* i_actor, void* i_data) {
    if (fopAc_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_OBJ_FW) {
        cXyz sp28 = ((fopAc_ac_c*)i_actor)->current.pos - ((fopAc_ac_c*)i_data)->current.pos;
        if (sp28.abs() < XREG_F(18) + 500.0f) {
            return i_actor;
        }
    }

    return NULL;
}

static void ride_movebg_init(e_bi_class* i_this) {
    fopAc_ac_c* actor = &i_this->actor;
    cXyz mae, ato;
    Vec vec;
    dBgS_ObjGndChk obj_gnd_chk;

    vec.x = actor->current.pos.x;
    vec.y = actor->current.pos.y + 200.0f;
    vec.z = actor->current.pos.z;

    obj_gnd_chk.SetPos(&vec);
    vec.y = dComIfG_Bgsp().GroundCross(&obj_gnd_chk);
    actor->current.pos.y = vec.y;

    fopAc_ac_c* fw_p = (fopAc_ac_c*)fpcM_Search(s_fw_sub, actor);
    if (fw_p != NULL) {
        mae = actor->current.pos - fw_p->current.pos;
        cMtx_YrotS(*calc_mtx, -fw_p->shape_angle.y);
        MtxPosition(&mae, &i_this->field_0xbb0);
        i_this->fw_id = fopAcM_GetID(fw_p);
        i_this->ride_time++;
    }
}

static int daE_BI_Execute(e_bi_class* i_this) {
    fopAc_ac_c* actor = &i_this->actor;

    if (i_this->arg0 == 53) {
        return 1;
    }

    if (i_this->ride_time == 0) {
        if (i_this->arg1 == 1) {
            if (i_this->field_0xbac == 0) {
                ride_movebg_init(i_this);
            } else {
                i_this->field_0xbac -= 1;
            }
        } else {
            i_this->ObjAcch.CrrPos(dComIfG_Bgsp());
            i_this->ride_time++;
        }
    }

    if (i_this->ride_time == 0) {
        return 1;
    }

    cXyz mae, ato;
    
    i_this->counter++;

    for (int i = 0; i < 3; i++) {
        if (i_this->timer[i] != 0) {
            i_this->timer[i]--;
        }
    }

    if (i_this->damage_time != 0) {
        i_this->damage_time--;
    }

    action(i_this);

    if (i_this->arg1 == 1) {
        obj_fw_class* fw_p = (obj_fw_class*)fopAcM_SearchByID(i_this->fw_id);
        if (fw_p != NULL) {
            mDoMtx_stack_c::transS(fw_p->actor.current.pos.x, fw_p->actor.current.pos.y + fw_p->field_0x594 + fw_p->field_0x588, fw_p->actor.current.pos.z);
            mDoMtx_stack_c::YrotM(fw_p->field_0x5a0);
            mDoMtx_stack_c::XrotM(fw_p->field_0x5a2);
            mDoMtx_stack_c::YrotM(-fw_p->field_0x5a0);
            mDoMtx_stack_c::YrotM(fw_p->actor.shape_angle.y);
            mDoMtx_stack_c::XrotM(fw_p->actor.shape_angle.x + fw_p->field_0x5a4);
            mDoMtx_stack_c::ZrotM(fw_p->actor.shape_angle.z);
            mDoMtx_stack_c::transM(i_this->field_0xbb0.x, i_this->field_0xbb0.y, i_this->field_0xbb0.z);

            mae.set(0.0f, 0.0f, 0.0f);
            mDoMtx_stack_c::multVec(&mae, &actor->current.pos);
            actor->home.pos = actor->current.pos;
        }
    } else {
        mDoMtx_stack_c::transS(actor->current.pos.x, actor->current.pos.y, actor->current.pos.z);

        if (i_this->action == 5) {
            mDoMtx_stack_c::transM(0.0f, JREG_F(8) + 27.0f, 0.0f);
            mDoMtx_stack_c::YrotM(i_this->field_0x6a6);
            mDoMtx_stack_c::XrotM(i_this->field_0x6a2);
            mDoMtx_stack_c::YrotM(-i_this->field_0x6a6);
            mDoMtx_stack_c::transM(0.0f, -(JREG_F(8) + 27.0f), 0.0f);
        }

        mDoMtx_stack_c::YrotM(actor->shape_angle.y);
        mDoMtx_stack_c::XrotM(actor->shape_angle.x);
        mDoMtx_stack_c::ZrotM(actor->shape_angle.z);
    }

    mDoMtx_stack_c::scaleM(l_HIO.basic_size * actor->scale.x, l_HIO.basic_size * actor->scale.x, l_HIO.basic_size * actor->scale.x);
    J3DModel* model = i_this->anm_p->getModel();
    model->setBaseTRMtx(mDoMtx_stack_c::get());
    i_this->anm_p->play(0, dComIfGp_getReverb(fopAcM_GetRoomNo(actor)));

    if (i_this->anm_no == BCK_BI_MOVE && (i_this->anm_p->checkFrame(0.0f) || i_this->anm_p->checkFrame(7.5f))) {
        i_this->sound.startCreatureSound(Z2SE_EN_BI_FOOTNOTE, 0, -1);
    }

    if ((i_this->anm_no == BCK_BI_WAIT01 || i_this->anm_no == BCK_BI_WAIT02) && i_this->anm_p->checkFrame(2.0f)) {
        i_this->sound.startCreatureSound(Z2SE_EN_BI_WAIT, 0, -1);
    }

    if (i_this->anm_no == BCK_BI_APPEAR) {
        if (i_this->anm_p->checkFrame(26.0f)) {
            i_this->sound.startCreatureSound(Z2SE_EN_BI_APPEAR, 0, -1);
        }

        if (i_this->anm_p->checkFrame(7.0f) || i_this->anm_p->checkFrame(18.0f)) {
            i_this->sound.startCreatureSound(Z2SE_EN_BI_FOOTNOTE, 0, -1);
        }
    }

    if (i_this->anm_no == BCK_BI_ATTACK && i_this->anm_p->checkFrame(8.0f)) {
        i_this->sound.startCreatureSound(Z2SE_EN_BI_ATTACK, 0, -1);
    }

    i_this->anm_p->modelCalc();
    MTXCopy(model->getAnmMtx(0), *calc_mtx);
    mae.set(KREG_F(0) + 15.0f, KREG_F(1), KREG_F(2));
    MtxPosition(&mae, &actor->eyePos);
    actor->attention_info.position = actor->eyePos;
    actor->attention_info.position.y += KREG_F(3) + 45.0f;

    f32 center = 0.0f;
    if (dComIfGp_checkPlayerStatus0(0, fopAcM_STATUS_UNK_0x80000)) {
        center = 100.0f;
    }

    ato = actor->current.pos;

    if (i_this->damage_time != 0) {
        ato.x += -20000.0f;
    }

    i_this->cc_cyl.SetC(ato);
    i_this->cc_cyl.SetR(XREG_F(1) + 25.0f + center);

    if (i_this->action == 2) {
        i_this->cc_cyl.SetH(JREG_F(13) + 120.0f);
    } else {
        mae.set(0.0f, 0.0f, 0.0f);
        MtxPosition(&mae, &ato);
        f32 fVar2 = fabsf(ato.y - actor->current.pos.y);
        i_this->cc_cyl.SetH(XREG_F(0) + 90.0f + fVar2 + center);
    }

    dComIfG_Ccsp()->Set(&i_this->cc_cyl);

    mae.set(0.0f, 0.0f, 0.0f);
    MTXCopy(model->getAnmMtx(0), *calc_mtx);
    MtxPosition(&mae, &ato);

    if (i_this->field_0x988 == 0) {
        ato.x += 17000.0f;
    } else {
        i_this->field_0x988 = 0;
    }

    i_this->at_sph.SetC(ato);
    i_this->at_sph.SetR(XREG_F(12) + 30.0f);
    dComIfG_Ccsp()->Set(&i_this->at_sph);

    if (i_this->leaf_id != 0) {
        fopAcM_delete(actor);
    }

    return 1;
}

static int daE_BI_IsDelete(e_bi_class* i_this) {
    return 1;
}

static int daE_BI_Delete(e_bi_class* i_this) {
    fopAc_ac_c* actor = &i_this->actor;
    
    dComIfG_resDelete(&i_this->phase, "E_BI");
    
    if (i_this->hio_init != 0) {
        hio_set = 0;
    }

    if (actor->heap != NULL) {
        i_this->anm_p->stopZelAnime();
    }

    return 1;
}

static int useHeapInit(fopAc_ac_c* actor) {
    e_bi_class* i_this = (e_bi_class*)actor;

    i_this->anm_p = new mDoExt_McaMorfSO((J3DModelData*)dComIfG_getObjectRes("E_BI", BMDR_BI), NULL, NULL,
                                               (J3DAnmTransform*)dComIfG_getObjectRes("E_BI", BCK_BI_WAIT01), 2, 1.0f, 0, -1,
                                               &i_this->sound, 0x80000, 0x11000084);
    if (i_this->anm_p == NULL || i_this->anm_p->getModel() == NULL) {
        return 0;
    }

    return 1;
}

static cPhs__Step daE_BI_Create(fopAc_ac_c* actor) {
    fopAcM_ct(actor, e_bi_class);
    e_bi_class* i_this = (e_bi_class*)actor;

    cPhs__Step phase = (cPhs__Step)dComIfG_resLoad(&i_this->phase, "E_BI");
    if (phase == cPhs_COMPLEATE_e) {
        OS_REPORT("E_BI PARAM %x\n", fopAcM_GetParam(actor));

        i_this->arg0 = fopAcM_GetParam(actor);
        i_this->arg1 = fopAcM_GetParam(actor) >> 8;
        i_this->arg2 = fopAcM_GetParam(actor) >> 16;

        OS_REPORT("E_BI//////////////E_BI SET 1 !!\n");

        if (!fopAcM_entrySolidHeap(actor, useHeapInit, 0x27A0)) {
            OS_REPORT("//////////////E_BI SET NON !!\n");
            
            return cPhs_ERROR_e;
        }

        OS_REPORT("//////////////E_BI SET 2 !!\n");

        if (strcmp(dComIfGp_getStartStageName(), "D_MN05A") == 0 && dComIfGs_isStageBossEnemy()) {
            return cPhs_ERROR_e;
        }

        if (i_this->arg0 == 53) {
            return phase;
        }

        if (hio_set == 0) {
            i_this->hio_init = 1;
            hio_set = 1;
            l_HIO.no = -1;
        }

        actor->attention_info.flags = fopAc_AttnFlag_BATTLE_e;
        fopAcM_SetMtx(actor, i_this->anm_p->getModel()->getBaseTRMtx());
        fopAcM_SetMin(actor, -200.0f, -200.0f, -200.0f);
        fopAcM_SetMax(actor, 200.0f, 200.0f, 200.0f);
        actor->health = 30;
        actor->field_0x560 = 30;

        static dCcD_SrcCyl cc_cyl_src = {
            {
                {0x0, {{0x0, 0x0, 0x0}, {0xd8fbfdff, 0x3}, 0x79}}, // mObj
                {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0}, // mGObjAt
                {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x2}, // mGObjTg
                {0x0}, // mGObjCo
            }, // mObjInf
            {
                {
                    {0.0f, 0.0f, 0.0f}, // mCenter
                    30.0f, // mRadius
                    20.0f // mHeight
                } // mCyl
            }
        };

        static dCcD_SrcSph at_sph_src = {
            {
                {0x0, {{AT_TYPE_CSTATUE_SWING, 0x1, 0xd}, {0x0, 0x0}, 0x0}}, // mObj
                {dCcD_SE_METAL, 0x0, 0x0, 0x0, 0x0}, // mGObjAt
                {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x2}, // mGObjTg
                {0x0}, // mGObjCo
            }, // mObjInf
            {
                {{0.0f, 0.0f, 0.0f}, 35.0f} // mSph
            } // mSphAttr
        };

        i_this->Stts.Init(100, 0, actor);
        i_this->cc_cyl.Set(cc_cyl_src);
        i_this->cc_cyl.SetStts(&i_this->Stts);
        i_this->at_sph.Set(at_sph_src);
        i_this->at_sph.SetStts(&i_this->Stts);

        i_this->ObjAcch.Set(fopAcM_GetPosition_p(actor), fopAcM_GetOldPosition_p(actor), actor, 1, &i_this->AcchCir,
                                fopAcM_GetSpeed_p(actor), NULL, NULL);
        i_this->AcchCir.SetWall(30.0f, 30.0f);

        i_this->sound.init(&actor->current.pos, &actor->eyePos, 3, 1);
        i_this->at_info.mpSound = &i_this->sound;
        i_this->at_info.mPowerType = 1;
        i_this->sound.setEnemyName("E_bi");
        
        i_this->counter = cM_rndF(65535.0f);
        actor->attention_info.distances[fopAc_attn_CARRY_e] = 42;

        if (i_this->arg0 != 1) {
            if (i_this->arg1 == 1) {
                i_this->field_0xbac = 10;
            } else {
                cXyz pos(actor->current.pos);
                pos.y += 100.0f;

                if (fopAcM_gc_c::gndCheck(&pos)) {
                    actor->current.pos.y = fopAcM_gc_c::getGroundY();
                    actor->home.pos.y = actor->current.pos.y;
                }
            }

            i_this->child_no = fopAcM_createChild(PROC_E_BI_LEAF, fopAcM_GetID(actor), i_this->arg1, &actor->current.pos,
                                                        fopAcM_GetRoomNo(actor), &actor->shape_angle, NULL, -1, NULL);
        } else {
            i_this->mode = -2;
            i_this->child_no = actor->parentActorID;
        }

        if (fopAcM_GetRoomNo(actor) == 50) {
            actor->attention_info.flags = fopAc_AttnFlag_BATTLE_e;
            actor->attention_info.distances[fopAc_attn_BATTLE_e] = 22;
        }

        daE_BI_Execute(i_this);
    }

    return phase;
}

AUDIO_INSTANCES;

static actor_method_class l_daE_BI_Method = {
    (process_method_func)daE_BI_Create,
    (process_method_func)daE_BI_Delete,
    (process_method_func)daE_BI_Execute,
    (process_method_func)daE_BI_IsDelete,
    (process_method_func)daE_BI_Draw,
};

actor_process_profile_definition g_profile_E_BI = {
  fpcLy_CURRENT_e,        // mLayerID
  9,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_E_BI,              // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(e_bi_class),     // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  147,                    // mPriority
  &l_daE_BI_Method,       // sub_method
  0x100D0100,             // mStatus
  fopAc_ENEMY_e,          // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};
