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
    /* 0x04 */ s8 field_0x4;
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
    field_0x4 = -1;
    basic_size = 1.0f;
    search_range = 400.0f;
    track_range = 600.0f;
    time_to_get_going = 30;
    movement_spd = 10.0f;
}

static void anm_init(e_bi_class* i_this, int i_index, f32 i_morf, u8 i_attr, f32 i_rate) {
    i_this->mpModelMorf->setAnm((J3DAnmTransform*)dComIfG_getObjectRes("E_BI", i_index), i_attr, i_morf, i_rate, 0.0f, -1.0f);
    i_this->mAnm = i_index;
}

static int daE_BI_Draw(e_bi_class* i_this) {
    fopAc_ac_c* a_this = &i_this->actor;

    if (i_this->field_0xbad == 0) {
        return 1;
    }
    
    g_env_light.settingTevStruct(0, &a_this->current.pos, &a_this->tevStr);
    J3DModel* model = i_this->mpModelMorf->getModel();
    g_env_light.setLightTevColorType_MAJI(model, &a_this->tevStr);
    J3DModelData* modelData = model->getModelData();
    modelData->getMaterialNodePointer(1)->getTevColor(1)->r = i_this->field_0x698;

    if (i_this->field_0x694 != 0) {
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

    i_this->mpModelMorf->entryDL();
    modelData->getMaterialNodePointer(1)->getTevColor(1)->r = 0;

    cXyz i_pos;
    i_pos.set(a_this->current.pos.x, a_this->current.pos.y + 100.0f + BREG_F(18), a_this->current.pos.z);
    i_this->mShadowKey = dComIfGd_setShadow(i_this->mShadowKey, 1, model, &i_pos,
                                            BREG_F(19) + 800.0f, 0.0f, a_this->current.pos.y,
                                            i_this->mObjAcch.GetGroundH(), i_this->mObjAcch.m_gnd,
                                            &a_this->tevStr, 0, 1.0f,
                                            dDlst_shadowControl_c::getSimpleTex());

    return 1;
}

static BOOL pl_check(e_bi_class* i_this, f32 param_2) {
    fopAc_ac_c* a_this = &i_this->actor;

    if (i_this->field_0x5b5 == 1) {
        return FALSE;
    }

    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    if (i_this->field_0x688 < param_2 && !fopAcM_otherBgCheck(a_this, player)) {
        return TRUE;
    }

    return FALSE;
}

static void damage_check(e_bi_class* i_this) {
    fopAc_ac_c* a_this = &i_this->actor;
    fopAc_ac_c* actor_p = dComIfGp_getPlayer(0);
    fopAc_ac_c* actor_p_2;

    if (i_this->field_0x692 == 0) {
        i_this->mStts.Move();

        if (i_this->field_0x82c.ChkAtShieldHit()) {
            i_this->mAction = 5;
            i_this->field_0x670 = 0;
            i_this->field_0x692 = 60;
            a_this->speedF = 0.0f;
            i_this->field_0x6a6 = i_this->field_0x684 + 0x8000;
        } else if (i_this->field_0x6f0.ChkTgHit()) {
            i_this->mAtInfo.mpCollider = i_this->field_0x6f0.GetTgHitObj();
            at_power_check(&i_this->mAtInfo);
            cc_at_check(a_this, &i_this->mAtInfo);

            if (i_this->mAtInfo.mpCollider->ChkAtType(AT_TYPE_HOOKSHOT)) {
                actor_p = dBomb_c::createEnemyBombHookshot(&a_this->eyePos, &a_this->current.angle, fopAcM_GetRoomNo(a_this));
                if (actor_p != NULL) {
                    actor_p_2 = fopAcM_SearchByID(i_this->field_0xba4);
                    if (actor_p_2 != NULL) {
                        actor_p_2->parentActorID = fopAcM_GetID(actor_p);
                    }

                    i_this->field_0x6b0 = 1;
                }
            }else if (i_this->mAtInfo.mpCollider->ChkAtType(AT_TYPE_BOOMERANG)) {
                actor_p = dBomb_c::createEnemyBombBoomerang(&a_this->eyePos, &a_this->current.angle, fopAcM_GetRoomNo(a_this));
                if (actor_p != NULL) {
                    actor_p_2 = fopAcM_SearchByID(i_this->field_0xba4);
                    if (actor_p_2 != NULL) {
                        actor_p_2->parentActorID = fopAcM_GetID(actor_p);
                    }

                    i_this->field_0x6b0 = 1;
                }
            } else if (i_this->field_0x694 != 0 || i_this->mAtInfo.mpCollider->ChkAtType(AT_TYPE_BOMB)) {
                dBomb_c::createNormalBombExplode(&a_this->eyePos);
                i_this->field_0x6b0 = 1;
            } else {
                i_this->mAction = 5;
                i_this->field_0x670 = 0;
                i_this->field_0x692 = 60;
                a_this->speedF = 0.0f;

                if (i_this->mAtInfo.mHitType == HIT_TYPE_STUN) {
                    i_this->field_0x6a6 = actor_p->shape_angle.y;
                } else {
                    i_this->field_0x6a6 = i_this->field_0x684 + 0x8000;
                }
            }
        }
    }
}

/* 8068DB1C 0001+00 data_8068DB1C @1009 */
/* 8068DB1D 0003+00 data_8068DB1D None */
static u8 l_initHIO;

static daE_BI_HIO_c l_HIO;

static void e_bi_wait(e_bi_class* i_this) {
    fopAc_ac_c* a_this = &i_this->actor;
    cXyz sp28;

    switch (i_this->field_0x670) {
        case -2:
            anm_init(i_this, BCK_BI_APPEAR, 1.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
            i_this->field_0x670 = 1;
            break;

        case 0:
            anm_init(i_this, BCK_BI_WAIT01TO02, 3.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
            i_this->mSound.startCreatureSound(Z2SE_EN_BI_CROUCH, 0, -1);
            i_this->field_0x670 = 1;
            break;

        case 1:
            if (i_this->mpModelMorf->isStop()) {
                if (i_this->field_0x5b5 == 1) {
                    i_this->mAction = 1;
                    i_this->field_0x670 = 0;
                } else {
                    anm_init(i_this, BCK_BI_WAIT02, 2.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
                    i_this->field_0x670 = 2;
                    i_this->field_0x68c[0] = l_HIO.time_to_get_going;
                }
            }
            break;

        case 2:
            cLib_addCalc2(&a_this->current.pos.x, a_this->home.pos.x, 1.0f, 5.0f);
            cLib_addCalc2(&a_this->current.pos.z, a_this->home.pos.z, 1.0f, 5.0f);
            sp28 = a_this->current.pos - a_this->home.pos;
            sp28.y = 0.0f;

            if (sp28.abs() < 1.0f && !pl_check(i_this, l_HIO.search_range + 50.0f)) {
                i_this->mAction = 1;
                i_this->field_0x670 = 0;
            } else if (i_this->field_0x68c[0] == 0) {
                i_this->mAction = 2;
                i_this->field_0x670 = 0;
            }
            break;
    }
}

static void e_bi_up(e_bi_class* i_this) {
    fopAc_ac_c* a_this = &i_this->actor;

    switch (i_this->field_0x670) {
        case 0:
            anm_init(i_this, BCK_BI_WAIT02TO01, 3.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
            i_this->mSound.startCreatureSound(Z2SE_EN_BI_STAND, 0, -1);
            i_this->field_0x670 = 1;
            break;

        case 1:
            if (i_this->mpModelMorf->isStop()) {
                anm_init(i_this, BCK_BI_WAIT01, 2.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
                i_this->field_0x670 = 2;
            }
            break;

        case 2:
            if (pl_check(i_this, l_HIO.search_range)) {
                i_this->mAction = 0;
                i_this->field_0x670 = 0;
            }
            break;
    }

    cLib_addCalcAngleS2(&a_this->current.angle.y, i_this->field_0x684, 4, 0x800);
    cLib_addCalcAngleS2(&a_this->shape_angle.y, a_this->current.angle.y, 4, 0x2000);

    if (i_this->field_0x5b5 == 1 && i_this->field_0x670 >= 2) {
        fopAc_ac_c* actor_p = fopAcM_SearchByID(i_this->field_0xba8);
        if (actor_p != NULL && actor_p->field_0x567 != 0) {
            i_this->mAction = 7;
            i_this->field_0x670 = 0;
        }
    }
}

static void e_bi_move(e_bi_class* i_this) {
    fopAc_ac_c* a_this = &i_this->actor;
    cXyz sp40, sp4c;
    f32 fVar1 = 0.0f;
    sp40 = a_this->home.pos - a_this->current.pos;

    switch (i_this->field_0x670) {
        case 0:
            if (i_this->field_0x68c[0] == 0) {
                anm_init(i_this, BCK_BI_MOVE, 3.0f, J3DFrameCtrl::EMode_LOOP, 3.0f);

                if (!pl_check(i_this, l_HIO.track_range)) {
                    i_this->field_0x670 = 5;
                } else {
                    i_this->field_0x670 = 1;
                }
            }
            break;

        case 1:
            fVar1 = l_HIO.movement_spd;
            i_this->field_0x680 = i_this->field_0x684;

            if (fopAcM_searchPlayerDistance(a_this) < KREG_F(7) + 150.0f) {
                i_this->field_0x670 = 2;
                anm_init(i_this, BCK_BI_ATTACK, 3.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
                fVar1 = 0.0f;
                a_this->speedF = fVar1;
            } else if (!pl_check(i_this, l_HIO.track_range)) {
                i_this->field_0x670 = 5;
            }
            break;

        case 2:
            i_this->field_0x988 = 1;

            if (i_this->mpModelMorf->isStop()) {
                i_this->field_0x670 = 0;
                i_this->field_0x68c[0] = cM_rndF(20.0f);
                anm_init(i_this, BCK_BI_WAIT02, 3.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
            }
            break;

        case 5:
            fVar1 = l_HIO.movement_spd;
            i_this->field_0x680 = cM_atan2s(sp40.x, sp40.z);
            sp40.y = 0.0f;

            if (i_this->field_0x694 == 0 && sp40.abs() < l_HIO.movement_spd * 1.5f) {
                i_this->mAction = 0;
                anm_init(i_this, BCK_BI_WAIT02, 2.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
                i_this->field_0x670 = 2;
                i_this->field_0x68c[0] = l_HIO.time_to_get_going;
            } else if (pl_check(i_this, l_HIO.search_range)) {
                i_this->field_0x670 = 0;
                i_this->field_0x68c[0] = 0;
            }
            break;
    }

    cLib_addCalcAngleS2(&a_this->current.angle.y, i_this->field_0x680, 2, 0x800);
    cLib_addCalc2(&a_this->speedF, fVar1, 1.0f, l_HIO.movement_spd);
    cLib_addCalcAngleS2(&a_this->shape_angle.y, a_this->current.angle.y, 4, 0x2000);
}

static void e_bi_ex(e_bi_class* i_this) {
    fopAc_ac_c* a_this = &i_this->actor;
    cXyz sp48, sp54;

    switch (i_this->field_0x670) {
        case 0:
            i_this->field_0x670 = 1;
            i_this->field_0x694 = 160;
            anm_init(i_this, BCK_BI_BOMBPOSE, 10.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
            f32 fVar1 = cM_rndF(0.2f) + 0.9f;
            i_this->field_0x6a2 = 0;
            i_this->field_0x6a4 = 3800.0f * fVar1;
            i_this->field_0x6a8 = fVar1 * 20.0f;
            a_this->speed.y = 23.0f;
            break;

        case 1:
            if (i_this->mObjAcch.ChkGroundHit()) {
                i_this->field_0x670 = 2;
                a_this->speed.y = 10.0f;
            }
            // fallthrough
        case 2:
            cLib_addCalc0(&i_this->field_0x6a8, 1.0f, 1.0f);
            cLib_addCalcAngleS2(&i_this->field_0x6a4, 0, 1, 200);
            break;
    }

    cMtx_YrotS(*calc_mtx, i_this->field_0x6a6);
    sp48.x = 0.0f;
    sp48.y = 0.0f;
    sp48.z = i_this->field_0x6a8;
    MtxPosition(&sp48, &sp54);
    a_this->current.pos += sp54;
    i_this->field_0x6a2 += i_this->field_0x6a4;

    if (i_this->field_0x694 != 0) {
        i_this->field_0x694--;
        i_this->mSound.startCreatureSoundLevel(Z2SE_OBJ_BOMB_IGNITION, 0, -1);
        i_this->field_0x696 += 0x1100;

        if (i_this->field_0x694 < 45) {
            i_this->field_0x696 += 0x1100;

            if (i_this->field_0x694 < 30) {
                i_this->field_0x696 += 0x1100;
            }
        }

        i_this->field_0x698 = 128.0f - cM_scos(i_this->field_0x696) * 127.0f;

        if (i_this->field_0x694 == 0) {
            dBomb_c::createNormalBombExplode(&a_this->eyePos);
            i_this->field_0x6b0 = 1;
        }

        static u16 ex_eff_id[5] = {
            0x0A0D,
            0x0A0E,
            0x0A0F,
            0x0A10,
            0x0A11,
        };

        for (int i = 0; i < 5; i++) {
            i_this->field_0xbc4[i] = dComIfGp_particle_set(i_this->field_0xbc4[i], ex_eff_id[i], &a_this->current.pos, &a_this->tevStr,
                                                           &a_this->shape_angle, NULL, 0xFF, NULL, -1, NULL, NULL, NULL);
            JPABaseEmitter* emitter = dComIfGp_particle_getEmitter(i_this->field_0xbc4[i]);
            
            if (emitter != NULL) {
                MTXCopy(i_this->mpModelMorf->getModel()->getAnmMtx(0), *calc_mtx);
                emitter->setGlobalRTMatrix(*calc_mtx);
            }
        }
    }

    cLib_addCalcAngleS2(&a_this->shape_angle.y, a_this->current.angle.y, 4, 0x2000);
}

static void e_bi_water(e_bi_class* i_this) {
    fopAc_ac_c* a_this = &i_this->actor;
    cXyz sp34, sp40;
    sp34 = a_this->home.pos - a_this->current.pos;

    switch (i_this->field_0x670) {
        case 0:
            anm_init(i_this, BCK_BI_MOVE, 3.0f, J3DFrameCtrl::EMode_LOOP, 3.0f);
            i_this->field_0x670 = 1;
            i_this->field_0x68c[0] = 20;
            a_this->speed.y = 0.0f;
            // fallthrough
        case 1:
            if (i_this->field_0x68c[0] == 0) {
                i_this->mpModelMorf->setPlaySpeed(0.0f);
                a_this->current.pos.y += a_this->speed.y;
                a_this->speed.y -= 0.1f;

                if (a_this->speed.y < -2.0f) {
                    a_this->speed.y = -2.0f;
                }

                cLib_addCalc0(&a_this->scale.x, 1.0f, 0.02f);
                cLib_addCalcAngleS2(&a_this->shape_angle.x, 0x7FFF, 0x10, 0x800);

                if (a_this->scale.x < 0.01f) {
                    i_this->field_0x6b0 = 1;
                }
            }
            break;
    }

    a_this->current.pos.x += a_this->speed.x;
    a_this->current.pos.z += a_this->speed.z;

    i_this->mObjAcch.CrrPos(dComIfG_Bgsp());
    a_this->speed.x *= 0.9f;
    a_this->speed.z *= 0.9f;

    cXyz sp4c(a_this->current.pos);
    sp4c.y = i_this->field_0x6ac;

    if (fabsf(sp4c.y - a_this->current.pos.y) < 70.0f) {
        fopAcM_effHamonSet(&i_this->field_0xbe8, &sp4c, KREG_F(2) + 1.0f, 0.2f);
    }
}

static void e_bi_disap(e_bi_class* i_this) {
    fopAc_ac_c* a_this = &i_this->actor;

    switch (i_this->field_0x670) {
        case 0:
            anm_init(i_this, BCK_BI_APPEAR, 10.0f, J3DFrameCtrl::EMode_NONE, 0.0f);
            i_this->field_0x660 = 40.0f;
            i_this->field_0x670 = 1;
            i_this->field_0x68c[0] = 10;
            break;

        case 1:
            if (i_this->field_0x68c[0] == 0) {
                i_this->field_0x660 -= 1.0f;

                if (i_this->field_0x660 < 0.0f) {
                    fopAcM_delete(a_this);
                    return;
                }
            }
            break;
    }

    i_this->mpModelMorf->setFrame(i_this->field_0x660);
}

static BOOL water_check(e_bi_class* i_this) {
    fopAc_ac_c* a_this = &i_this->actor;
    Vec i_pos;
    dBgS_ObjGndChk_Spl gnd_chk_spl;

    i_pos.x = a_this->current.pos.x;
    i_pos.y = a_this->current.pos.y + 300.0f;
    i_pos.z = a_this->current.pos.z;
    gnd_chk_spl.SetPos(&i_pos);
    i_this->field_0x6ac = dComIfG_Bgsp().GroundCross(&gnd_chk_spl);

    if (i_this->field_0x6ac > a_this->current.pos.y + 50.0f) {
        return TRUE;
    }

    return FALSE;
}

static void action(e_bi_class* i_this) {
    fopAc_ac_c* a_this = &i_this->actor;
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    cXyz sp9c, spa8;

    i_this->field_0x684 = fopAcM_searchPlayerAngleY(a_this);
    i_this->field_0x688 = (a_this->home.pos - player->current.pos).abs();

    damage_check(i_this);

    s8 bVar1 = 1;
    s8 bVar2 = 0;
    s8 bVar3 = 0;
    s8 bVar4 = 1;

    switch (i_this->mAction) {
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

    if (fopAcM_GetRoomNo(a_this) == 3) {
        dBgS_LinChk lin_chk;
        sp9c = player->current.pos;
        sp9c.y += 150.0f;
        lin_chk.Set(&sp9c, &a_this->eyePos, a_this);

        if (dComIfG_Bgsp().LineCross(&lin_chk)) {
            bVar1 = 0;
        }
    }

    if (bVar1 != 0) {
        fopAcM_OnStatus(a_this, 0);
        cLib_onBit<u32>(a_this->attention_info.flags, fopAc_AttnFlag_BATTLE_e);
    } else {
        fopAcM_OffStatus(a_this, 0);
        a_this->attention_info.flags = 0;
    }

    if (bVar2 != 0) {
        cXyz* ccMoveP = i_this->mStts.GetCCMoveP();
        if (ccMoveP != NULL) {
            a_this->current.pos.x += ccMoveP->x * 0.5f;
            a_this->current.pos.y += ccMoveP->y * 0.5f;
            a_this->current.pos.z += ccMoveP->z * 0.5f;
        }

        cMtx_YrotS(*calc_mtx, a_this->current.angle.y);
        sp9c.x = 0.0f;
        sp9c.y = 0.0f;
        sp9c.z = a_this->speedF;
        MtxPosition(&sp9c, &spa8);
        a_this->speed.x = spa8.x;
        a_this->speed.z = spa8.z;
        a_this->current.pos += a_this->speed;
        a_this->speed.y -= 5.0f;
        i_this->mObjAcch.CrrPos(dComIfG_Bgsp());
    }

    if (bVar3 != 0) {
        cLib_onBit<u32>(a_this->attention_info.flags, fopAc_AttnFlag_CARRY_e);

        if (fopAcM_checkCarryNow(a_this)) {
            cLib_offBit<u32>(a_this->attention_info.flags, fopAc_AttnFlag_CARRY_e);
            fopAc_ac_c* bomb_p = dBomb_c::createEnemyBomb(&a_this->current.pos, &a_this->current.angle, fopAcM_GetRoomNo(a_this));

            if (bomb_p == NULL) {
                OS_REPORT("//////////////LEAF ID 2???\n");
            } else {
                fopAc_ac_c* actor_p = fopAcM_SearchByID(i_this->field_0xba4);
                if (actor_p != NULL) {
                    actor_p->parentActorID = fopAcM_GetID(bomb_p);

                    OS_REPORT("//////////////LEAF ID 1  %d\n", actor_p->parentActorID);
                }

                i_this->field_0x6b0 = 1;

                OS_REPORT("//////////////LEAF ID 1???\n");
            }
        }

        cLib_offBit<u32>(a_this->attention_info.flags, fopAc_AttnFlag_BATTLE_e);
    } else {
        cLib_offBit<u32>(a_this->attention_info.flags, fopAc_AttnFlag_CARRY_e);
        cLib_onBit<u32>(a_this->attention_info.flags, fopAc_AttnFlag_BATTLE_e);
    }

    if (bVar4 != 0 && water_check(i_this)) {
        i_this->mAction = 6;
        i_this->field_0x670 = 0;
        a_this->current.pos.y = i_this->field_0x6ac - (WREG_F(11) + 50.0f);
        cXyz i_pos(a_this->current.pos);

        static cXyz sc(0.8f, 0.8f, 0.8f);
        static u16 w_eff_id[4] = {
            0x01B8, 0x01B9, 0x01BA, 0x01BB,
        };

        for (int i = 0; i < 4; i++) {
            i_this->field_0xbd8[i] = dComIfGp_particle_set(i_this->field_0xbd8[i], w_eff_id[i], &i_pos, &a_this->tevStr,
                                                           &a_this->shape_angle, &sc, 0xFF, NULL,
                                                           -1, NULL, NULL, NULL);
        }

        i_this->mSound.startCreatureSound(Z2SE_CM_BODYFALL_WATER_S, 0, -1);
    }

    cXyz i_effSize(0.75f, 0.75f, 0.75f);
    setMidnaBindEffect(&i_this->actor, &i_this->mSound, &a_this->eyePos, &i_effSize);
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
    fopAc_ac_c* a_this = &i_this->actor;
    cXyz sp74, sp80;
    Vec i_pos;
    dBgS_ObjGndChk obj_gnd_chk;

    i_pos.x = a_this->current.pos.x;
    i_pos.y = a_this->current.pos.y + 200.0f;
    i_pos.z = a_this->current.pos.z;

    obj_gnd_chk.SetPos(&i_pos);
    i_pos.y = dComIfG_Bgsp().GroundCross(&obj_gnd_chk);
    a_this->current.pos.y = i_pos.y;

    fopAc_ac_c* actor_p = (fopAc_ac_c*)fpcM_Search(s_fw_sub, a_this);
    if (actor_p != NULL) {
        sp74 = a_this->current.pos - actor_p->current.pos;
        cMtx_YrotS(*calc_mtx, -actor_p->shape_angle.y);
        MtxPosition(&sp74, &i_this->field_0xbb0);
        i_this->field_0xba8 = fopAcM_GetID(actor_p);
        i_this->field_0xbad++;
    }
}

static int daE_BI_Execute(e_bi_class* i_this) {
    fopAc_ac_c* a_this = &i_this->actor;

    if (i_this->field_0x5b4 == 53) {
        return 1;
    }

    if (i_this->field_0xbad == 0) {
        if (i_this->field_0x5b5 == 1) {
            if (i_this->field_0xbac == 0) {
                ride_movebg_init(i_this);
            } else {
                i_this->field_0xbac -= 1;
            }
        } else {
            i_this->mObjAcch.CrrPos(dComIfG_Bgsp());
            i_this->field_0xbad++;
        }
    }

    if (i_this->field_0xbad == 0) {
        return 1;
    }

    cXyz sp60, sp6c;
    
    i_this->field_0x66c++;

    for (int i = 0; i < 3; i++) {
        if (i_this->field_0x68c[i] != 0) {
            i_this->field_0x68c[i]--;
        }
    }

    if (i_this->field_0x692 != 0) {
        i_this->field_0x692--;
    }

    action(i_this);

    if (i_this->field_0x5b5 == 1) {
        obj_fw_class* fw_p = (obj_fw_class*)fopAcM_SearchByID(i_this->field_0xba8);
        if (fw_p != NULL) {
            mDoMtx_stack_c::transS(fw_p->actor.current.pos.x, fw_p->actor.current.pos.y + fw_p->field_0x594 + fw_p->field_0x588, fw_p->actor.current.pos.z);
            mDoMtx_stack_c::YrotM(fw_p->field_0x5a0);
            mDoMtx_stack_c::XrotM(fw_p->field_0x5a2);
            mDoMtx_stack_c::YrotM(-fw_p->field_0x5a0);
            mDoMtx_stack_c::YrotM(fw_p->actor.shape_angle.y);
            mDoMtx_stack_c::XrotM(fw_p->actor.shape_angle.x + fw_p->field_0x5a4);
            mDoMtx_stack_c::ZrotM(fw_p->actor.shape_angle.z);
            mDoMtx_stack_c::transM(i_this->field_0xbb0.x, i_this->field_0xbb0.y, i_this->field_0xbb0.z);

            sp60.set(0.0f, 0.0f, 0.0f);
            mDoMtx_stack_c::multVec(&sp60, &a_this->current.pos);
            a_this->home.pos = a_this->current.pos;
        }
    } else {
        mDoMtx_stack_c::transS(a_this->current.pos.x, a_this->current.pos.y, a_this->current.pos.z);

        if (i_this->mAction == 5) {
            mDoMtx_stack_c::transM(0.0f, JREG_F(8) + 27.0f, 0.0f);
            mDoMtx_stack_c::YrotM(i_this->field_0x6a6);
            mDoMtx_stack_c::XrotM(i_this->field_0x6a2);
            mDoMtx_stack_c::YrotM(-i_this->field_0x6a6);
            mDoMtx_stack_c::transM(0.0f, -(JREG_F(8) + 27.0f), 0.0f);
        }

        mDoMtx_stack_c::YrotM(a_this->shape_angle.y);
        mDoMtx_stack_c::XrotM(a_this->shape_angle.x);
        mDoMtx_stack_c::ZrotM(a_this->shape_angle.z);
    }

    mDoMtx_stack_c::scaleM(l_HIO.basic_size * a_this->scale.x, l_HIO.basic_size * a_this->scale.x, l_HIO.basic_size * a_this->scale.x);
    J3DModel* model = i_this->mpModelMorf->getModel();
    model->setBaseTRMtx(mDoMtx_stack_c::get());
    i_this->mpModelMorf->play(0, dComIfGp_getReverb(fopAcM_GetRoomNo(a_this)));

    if (i_this->mAnm == BCK_BI_MOVE && (i_this->mpModelMorf->checkFrame(0.0f) || i_this->mpModelMorf->checkFrame(7.5f))) {
        i_this->mSound.startCreatureSound(Z2SE_EN_BI_FOOTNOTE, 0, -1);
    }

    if ((i_this->mAnm == BCK_BI_WAIT01 || i_this->mAnm == BCK_BI_WAIT02) && i_this->mpModelMorf->checkFrame(2.0f)) {
        i_this->mSound.startCreatureSound(Z2SE_EN_BI_WAIT, 0, -1);
    }

    if (i_this->mAnm == BCK_BI_APPEAR) {
        if (i_this->mpModelMorf->checkFrame(26.0f)) {
            i_this->mSound.startCreatureSound(Z2SE_EN_BI_APPEAR, 0, -1);
        }

        if (i_this->mpModelMorf->checkFrame(7.0f) || i_this->mpModelMorf->checkFrame(18.0f)) {
            i_this->mSound.startCreatureSound(Z2SE_EN_BI_FOOTNOTE, 0, -1);
        }
    }

    if (i_this->mAnm == BCK_BI_ATTACK && i_this->mpModelMorf->checkFrame(8.0f)) {
        i_this->mSound.startCreatureSound(Z2SE_EN_BI_ATTACK, 0, -1);
    }

    i_this->mpModelMorf->modelCalc();
    MTXCopy(model->getAnmMtx(0), *calc_mtx);
    sp60.set(KREG_F(0) + 15.0f, KREG_F(1), KREG_F(2));
    MtxPosition(&sp60, &a_this->eyePos);
    a_this->attention_info.position = a_this->eyePos;
    a_this->attention_info.position.y += KREG_F(3) + 45.0f;

    f32 fVar1 = 0.0f;
    if (dComIfGp_checkPlayerStatus0(0, fopAcM_STATUS_UNK_0x80000)) {
        fVar1 = 100.0f;
    }

    sp6c = a_this->current.pos;

    if (i_this->field_0x692 != 0) {
        sp6c.x += -20000.0f;
    }

    i_this->field_0x6f0.SetC(sp6c);
    i_this->field_0x6f0.SetR(XREG_F(1) + 25.0f + fVar1);

    if (i_this->mAction == 2) {
        i_this->field_0x6f0.SetH(JREG_F(13) + 120.0f);
    } else {
        sp60.set(0.0f, 0.0f, 0.0f);
        MtxPosition(&sp60, &sp6c);
        f32 fVar2 = fabsf(sp6c.y - a_this->current.pos.y);
        i_this->field_0x6f0.SetH(XREG_F(0) + 90.0f + fVar2 + fVar1);
    }

    dComIfG_Ccsp()->Set(&i_this->field_0x6f0);

    sp60.set(0.0f, 0.0f, 0.0f);
    MTXCopy(model->getAnmMtx(0), *calc_mtx);
    MtxPosition(&sp60, &sp6c);

    if (i_this->field_0x988 == 0) {
        sp6c.x += 17000.0f;
    } else {
        i_this->field_0x988 = 0;
    }

    i_this->field_0x82c.SetC(sp6c);
    i_this->field_0x82c.SetR(XREG_F(12) + 30.0f);
    dComIfG_Ccsp()->Set(&i_this->field_0x82c);

    if (i_this->field_0x6b0 != 0) {
        fopAcM_delete(a_this);
    }

    return 1;
}

static int daE_BI_IsDelete(e_bi_class* i_this) {
    return 1;
}

static int daE_BI_Delete(e_bi_class* i_this) {
    fopAc_ac_c* a_this = &i_this->actor;
    
    dComIfG_resDelete(&i_this->mPhase, "E_BI");
    
    if (i_this->field_0xbfc != 0) {
        l_initHIO = 0;
    }

    if (a_this->heap != NULL) {
        i_this->mpModelMorf->stopZelAnime();
    }

    return 1;
}

static int useHeapInit(fopAc_ac_c* a_this) {
    e_bi_class* i_this = (e_bi_class*)a_this;

    i_this->mpModelMorf = new mDoExt_McaMorfSO((J3DModelData*)dComIfG_getObjectRes("E_BI", BMDR_BI), NULL, NULL,
                                               (J3DAnmTransform*)dComIfG_getObjectRes("E_BI", BCK_BI_WAIT01), 2, 1.0f, 0, -1,
                                               &i_this->mSound, 0x80000, 0x11000084);
    if (i_this->mpModelMorf == NULL || i_this->mpModelMorf->getModel() == NULL) {
        return 0;
    }

    return 1;
}

static cPhs__Step daE_BI_Create(fopAc_ac_c* a_this) {
    fopAcM_ct(a_this, e_bi_class);
    e_bi_class* i_this = (e_bi_class*)a_this;

    cPhs__Step phase = (cPhs__Step)dComIfG_resLoad(&i_this->mPhase, "E_BI");
    if (phase == cPhs_COMPLEATE_e) {
        OS_REPORT("E_BI PARAM %x\n", fopAcM_GetParam(a_this));

        i_this->field_0x5b4 = fopAcM_GetParam(a_this);
        i_this->field_0x5b5 = fopAcM_GetParam(a_this) >> 8;
        i_this->field_0x5b6 = fopAcM_GetParam(a_this) >> 16;

        OS_REPORT("E_BI//////////////E_BI SET 1 !!\n");

        if (!fopAcM_entrySolidHeap(a_this, useHeapInit, 0x27A0)) {
            OS_REPORT("//////////////E_BI SET NON !!\n");
            
            return cPhs_ERROR_e;
        }

        OS_REPORT("//////////////E_BI SET 2 !!\n");

        if (strcmp(dComIfGp_getStartStageName(), "D_MN05A") == 0 && dComIfGs_isStageBossEnemy()) {
            return cPhs_ERROR_e;
        }

        if (i_this->field_0x5b4 == 53) {
            return phase;
        }

        if (l_initHIO == 0) {
            i_this->field_0xbfc = 1;
            l_initHIO = 1;
            l_HIO.field_0x4 = -1;
        }

        a_this->attention_info.flags = fopAc_AttnFlag_BATTLE_e;
        fopAcM_SetMtx(a_this, i_this->mpModelMorf->getModel()->getBaseTRMtx());
        fopAcM_SetMin(a_this, -200.0f, -200.0f, -200.0f);
        fopAcM_SetMax(a_this, 200.0f, 200.0f, 200.0f);
        a_this->health = 30;
        a_this->field_0x560 = 30;

        static dCcD_SrcCyl cc_cyl_src = {
            {
                {0x0, {{0x0, 0x0, 0x0}, {0xd8fbfdff, 0x3}, 0x79}}, // mObj
                {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0}, // mGObjAt
                {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x2}, // mGObjTg
                {0x0}, // mGObjCo
            }, // mObjInf
            {
                {0.0f, 0.0f, 0.0f}, // mCenter
                30.0f, // mRadius
                20.0f // mHeight
            } // mCyl
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

        i_this->mStts.Init(100, 0, a_this);
        i_this->field_0x6f0.Set(cc_cyl_src);
        i_this->field_0x6f0.SetStts(&i_this->mStts);
        i_this->field_0x82c.Set(at_sph_src);
        i_this->field_0x82c.SetStts(&i_this->mStts);

        i_this->mObjAcch.Set(fopAcM_GetPosition_p(a_this), fopAcM_GetOldPosition_p(a_this), a_this, 1, &i_this->mAcchCir,
                                fopAcM_GetSpeed_p(a_this), NULL, NULL);
        i_this->mAcchCir.SetWall(30.0f, 30.0f);

        i_this->mSound.init(&a_this->current.pos, &a_this->eyePos, 3, 1);
        i_this->mAtInfo.mpSound = &i_this->mSound;
        i_this->mAtInfo.mPowerType = 1;
        i_this->mSound.setEnemyName("E_bi");
        
        i_this->field_0x66c = cM_rndF(65535.0f);
        a_this->attention_info.distances[fopAc_attn_CARRY_e] = 42;

        if (i_this->field_0x5b4 != 1) {
            if (i_this->field_0x5b5 == 1) {
                i_this->field_0xbac = 10;
            } else {
                cXyz sp38(a_this->current.pos);
                sp38.y += 100.0f;

                if (fopAcM_gc_c::gndCheck(&sp38)) {
                    a_this->current.pos.y = fopAcM_gc_c::getGroundY();
                    a_this->home.pos.y = a_this->current.pos.y;
                }
            }

            i_this->field_0xba4 = fopAcM_createChild(PROC_E_BI_LEAF, fopAcM_GetID(a_this), i_this->field_0x5b5, &a_this->current.pos,
                                                        fopAcM_GetRoomNo(a_this), &a_this->shape_angle, NULL, -1, NULL);
        } else {
            i_this->field_0x670 = -2;
            i_this->field_0xba4 = a_this->parentActorID;
        }

        if (fopAcM_GetRoomNo(a_this) == 50) {
            a_this->attention_info.flags = fopAc_AttnFlag_BATTLE_e;
            a_this->attention_info.distances[fopAc_attn_BATTLE_e] = 22;
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

extern actor_process_profile_definition g_profile_E_BI = {
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
