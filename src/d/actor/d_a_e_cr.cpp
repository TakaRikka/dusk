/**
 * @file d_a_e_cr.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_e_cr.h"
#include "d/d_cc_d.h"
#include "f_op/f_op_actor_enemy.h"

class daE_CR_HIO_c : public JORReflexible {
public:
    daE_CR_HIO_c();
    virtual ~daE_CR_HIO_c() {}

    void genMessage(JORMContext*);

    /* 0x04 */ s8 id;
    /* 0x08 */ f32 base_size;
    /* 0x0C */ f32 move_speed;
    /* 0x10 */ f32 pl_search_range;
};

enum daE_CR_ACTION {
    ACTION_MOVE,
    ACTION_S_DAMAGE = 9,
    ACTION_DAMAGE,
};

daE_CR_HIO_c::daE_CR_HIO_c() {
    id = -1;
    base_size = 3.0f;
    move_speed = 40.0f;
    pl_search_range = 400.0f;
}

static void anm_init(e_cr_class* a_this, int i_anm, f32 i_morf, u8 i_mode, f32 i_speed) {
    a_this->modelMorf->setAnm((J3DAnmTransform*)dComIfG_getObjectRes("E_CR", i_anm), i_mode, i_morf, i_speed, 0.0f, -1.0f);
    a_this->anm = i_anm;
}

static int nodeCallBack(J3DJoint* i_joint, int param_1) {
    if (param_1 == 0) {
        int jnt_no = i_joint->getJntNo();
        J3DModel* model = j3dSys.getModel();
        e_cr_class* a_this = (e_cr_class*)model->getUserArea();
        if (a_this != NULL) {
            if (jnt_no == 2) {
                MTXCopy(model->getAnmMtx(jnt_no), *calc_mtx);
                cMtx_YrotM(*calc_mtx, a_this->head_rot);
                model->setAnmMtx(jnt_no, *calc_mtx);
                MTXCopy(*calc_mtx, J3DSys::mCurrentMtx);
            }
        }
    }

    return 1;
}

static int daE_CR_Draw(e_cr_class* a_this) {
    fopAc_ac_c* actor = &a_this->enemy;

    J3DModel* model = a_this->modelMorf->getModel();
    g_env_light.settingTevStruct(0, &actor->current.pos, &actor->tevStr);
    g_env_light.setLightTevColorType_MAJI(model, &actor->tevStr);
    a_this->modelMorf->entryDL();

    dComIfGd_setSimpleShadow(&actor->current.pos, a_this->acch.GetGroundH(), 40.0f + TREG_F(11), a_this->acch.m_gnd, 0, 1.0f, dDlst_shadowControl_c::getSimpleTex());
    return 1;
}

static BOOL other_bg_check(e_cr_class* a_this, fopAc_ac_c* i_other) {
    fopAc_ac_c* actor = &a_this->enemy;
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

static BOOL pl_check(e_cr_class* a_this, f32 i_range, s16 i_angle) {
    fopAc_ac_c* actor = &a_this->enemy;
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    
    if (a_this->dist_to_pl < i_range) {
        s16 angle_diff = actor->shape_angle.y - a_this->angle_to_pl;
        if (angle_diff < i_angle && angle_diff > (s16)-i_angle && !other_bg_check(a_this, player)) {
            return TRUE;
        }
    }

    return FALSE;
}

static void damage_check(e_cr_class* a_this) {
    fopAc_ac_c* actor = &a_this->enemy;
    fopAc_ac_c* player = dComIfGp_getPlayer(0);

    if (a_this->invulnerabilityTimer == 0) {
        a_this->ccStts.Move();

        if (a_this->ccSph.ChkTgHit()) {
            a_this->atInfo.mpCollider = a_this->ccSph.GetTgHitObj();
            cc_at_check(actor, &a_this->atInfo);

            if (a_this->atInfo.mpCollider->ChkAtType(AT_TYPE_WOLF_ATTACK | AT_TYPE_WOLF_CUT_TURN | AT_TYPE_10000000 | AT_TYPE_MIDNA_LOCK)) {
                a_this->invulnerabilityTimer = 20;
            } else {
                a_this->invulnerabilityTimer = 10;
            }

            if (a_this->atInfo.mHitType == 16) {
                a_this->action = ACTION_S_DAMAGE;
            } else {
                a_this->action = ACTION_DAMAGE;
                a_this->sound.startCreatureVoice(Z2SE_EN_CR_V_DEATH, -1);
            }

            actor->current.angle.y = a_this->atInfo.mHitDirection.y;
            a_this->mode = 0;
        }

        if (actor->health <= 10) {
            a_this->ccSph.SetTgHitMark((CcG_Tg_HitMark)3);
        }
    }
}

static BOOL way_bg_check(e_cr_class* a_this, f32 param_1) {
    fopAc_ac_c* actor = &a_this->enemy;
    dBgS_LinChk linchk;
    cXyz sp28;
    cXyz startpos;
    cXyz endpos;

    startpos = actor->current.pos;
    startpos.y += 50.0f;
    cMtx_YrotS(*calc_mtx, actor->shape_angle.y);
    sp28.x = 0.0f;
    sp28.y = 50.0f;
    sp28.z = param_1;
    MtxPosition(&sp28, &endpos);
    endpos += actor->current.pos;

    linchk.Set(&startpos, &endpos, actor);
    if (dComIfG_Bgsp().LineCross(&linchk)) {
        return TRUE;
    }

    return FALSE;
}

static u8 l_HIOInit;

static daE_CR_HIO_c l_HIO;

static void e_cr_move(e_cr_class* a_this) {
    fopAc_ac_c* actor = &a_this->enemy;
    int anm_frame = a_this->modelMorf->getFrame();
    f32 move_speed_target = 0.0f;

    switch (a_this->mode) {
    case 0:
        anm_init(a_this, 9, 3.0f, 2, 1.0f);
        a_this->timers[0] = 30.0f + cM_rndF(60.0f);
        a_this->mode = 1;
        break;
    case 1:
        move_speed_target = l_HIO.move_speed;

        if (a_this->timers[0] == 0 && (anm_frame == 0 || anm_frame == 4)) {
            a_this->timers[0] = 50.0f + cM_rndF(100.0f);
            a_this->mode = 2;
            a_this->modelMorf->setPlaySpeed(0.0f);
        }

        if (a_this->timers[2] == 0 && way_bg_check(a_this, 200.0f)) {
            a_this->timers[2] = 20;
            a_this->timers[1] = 20.0f + cM_rndF(10.0f);
            a_this->angle_target = actor->current.angle.y + 0x8000;
        }

        if (a_this->timers[1] == 0) {
            a_this->angle_target = actor->current.angle.y + (s16)cM_rndFX(30000.0f);
            a_this->timers[1] = 10.0f + cM_rndF(5.0f);
        }
        break;
    case 2:
        a_this->field_0x690 = 1;
        if (a_this->timers[0] == 0) {
            a_this->modelMorf->setPlaySpeed(1.0f);
            a_this->timers[0] = 30.0f + cM_rndF(60.0f);
            a_this->mode = 1;
        }
        break;
    case 3:
        move_speed_target = l_HIO.move_speed;

        if (a_this->timers[0] == 0 && (anm_frame == 0 || anm_frame == 4)) {
            a_this->timers[0] = 50.0f + cM_rndF(100.0f);
            a_this->mode = 2;
            a_this->modelMorf->setPlaySpeed(0.0f);
        }

        if (a_this->timers[1] == 0) {
            if (way_bg_check(a_this, 200.0f)) {
                a_this->angle_target = fopAcM_searchPlayerAngleY(actor) + (s16)cM_rndFX(15000.0f);
            } else {
                a_this->angle_target = (fopAcM_searchPlayerAngleY(actor) + 0x10000 + (s16)cM_rndFX(15000.0f)) - 0x8000;
            }

            a_this->timers[1] = 10.0f + cM_rndF(5.0f);
        }

        if (a_this->timers[3] != 0 && ((a_this->lifetime & 3) == 0) && cM_rndF(1.0f) < 0.5f) {
            cXyz egg_pos(actor->current.pos);
            egg_pos.y += 35.0f * l_HIO.base_size;

            fopAcM_createChild(PROC_E_CR_EGG, fopAcM_GetID(actor), 0, &egg_pos, fopAcM_GetRoomNo(actor), &actor->shape_angle, NULL, -1, NULL);
            a_this->sound.startCreatureSound(Z2SE_EN_CR_LAYEGG, 0, -1);
        }
        break;
    }

    cLib_addCalcAngleS2(&actor->current.angle.y, a_this->angle_target, 4, 0x1800);
    cLib_addCalc2(&actor->speedF, move_speed_target, 1.0f, 0.5f * l_HIO.move_speed);
    cLib_addCalcAngleS2(&actor->shape_angle.y, actor->current.angle.y, 4, 0x2000);

    if (a_this->mode < 3 && pl_check(a_this, l_HIO.pl_search_range, 0x7FFF)) {
        a_this->mode = 3;
        anm_init(a_this, 9, 3.0f, 2, 1.0f);
        a_this->timers[0] = 20.0f + cM_rndF(15.0f);
        a_this->timers[3] = 15;
    }
}

static void e_cr_s_damage(e_cr_class* a_this) {
    fopAc_ac_c* actor = &a_this->enemy;

    switch (a_this->mode) {
    case 0:
        anm_init(a_this, 4, 2.0f, 0, 1.0f);
        a_this->mode = 1;
        actor->speedF = -25.0f;
        break;
    case 1:
        if (a_this->modelMorf->isStop()) {
            anm_init(a_this, 5, 1.0f, 2, 1.0f);
            a_this->timers[0] = 50.0f + cM_rndF(30.0f);
            a_this->mode = 2;
        }
        break;
    case 2:
        if (a_this->timers[0] == 0) {
            a_this->action = ACTION_MOVE;
            a_this->mode = 0;
        }
        break;
    }

    cLib_addCalc0(&actor->speedF, 1.0f, 4.0f);
}

static void e_cr_damage(e_cr_class* a_this) {
    fopAc_ac_c* actor = &a_this->enemy;

    a_this->invulnerabilityTimer = 6;

    switch (a_this->mode) {
    case 0:
        anm_init(a_this, 6, 2.0f, 2, 1.0f);
        a_this->mode = 1;
        actor->speed.y = 38.0f + cM_rndF(8.0f);
        actor->speedF = -30.0f;
        a_this->field_0x68e = cM_rndFX(1000.0f);
        break;
    case 1:
        if (a_this->acch.ChkGroundHit()) {
            actor->speed.y = 25.0f;
            actor->speedF *= 0.5f;

            if (cM_rndF(1.0f) < 0.5f) {
                anm_init(a_this, 7, 15.0f, 0, 1.0f);
            } else {
                anm_init(a_this, 8, 15.0f, 0, 1.0f);
            }

            a_this->mode = 2;
            a_this->timers[0] = 40;
            fopAcM_effSmokeSet2(&a_this->field_0xa48, &a_this->field_0xa4c, &actor->current.pos, &actor->shape_angle, 1.0f + NREG_F(18), &actor->tevStr);
        }
        break;
    case 2:
        if (a_this->acch.ChkGroundHit()) {
            cLib_addCalc0(&actor->speedF, 1.0f, 5.0f);
            cLib_addCalcAngleS2(&a_this->field_0x68e, 0, 1, 100);

            if (a_this->timers[0] == 0) {
                fopAcM_createDisappear(actor, &actor->eyePos, 6, 0, 0xFF);
                fopAcM_create(PROC_NPC_WORM, 0xFFFFFF01, &actor->eyePos, fopAcM_GetRoomNo(actor), NULL, NULL, -1);
                fopAcM_delete(actor);
            }
        }
        break;
    }

    actor->shape_angle.y += a_this->field_0x68e;
}

static void action(e_cr_class* a_this) {
    fopEn_enemy_c* actor = &a_this->enemy;
    cXyz mae;
    cXyz ato;

    a_this->angle_to_pl = fopAcM_searchPlayerAngleY(actor);
    a_this->dist_to_pl = fopAcM_searchPlayerDistance(actor);

    damage_check(a_this);

    a_this->ccSph.OffAtVsPlayerBit();
    BOOL attn_ON = TRUE;

    switch (a_this->action) {
    case ACTION_MOVE:
        e_cr_move(a_this);
        a_this->ccSph.OnAtVsPlayerBit();
        break;
    case ACTION_S_DAMAGE:
        e_cr_s_damage(a_this);
        break;
    case ACTION_DAMAGE:
        e_cr_damage(a_this);
        attn_ON = FALSE;
        break;
    }

    if (attn_ON) {
        fopAcM_OnStatus(actor, 0);
        actor->attention_info.flags = fopAc_AttnFlag_BATTLE_e;
    } else {
        fopAcM_OffStatus(actor, 0);
        actor->attention_info.flags = 0;
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

    a_this->acch.CrrPos(dComIfG_Bgsp());

    if (a_this->field_0x690 != 0) {
        if (a_this->field_0x692 == 0) {
            if (a_this->head_rot_target >= 0) {
                a_this->head_rot_target = -5500;
            } else {
                a_this->head_rot_target = 5500;
            }
            a_this->field_0x692 = 7.0f + cM_rndF(10.0f);
        } else {
            a_this->field_0x692--;
        }
        a_this->field_0x690 = 0;
    } else {
        a_this->head_rot_target = 0;
    }

    cLib_addCalcAngleS2(&a_this->head_rot, a_this->head_rot_target, 2, 0x1000);

    cXyz effsize(0.5f, 0.5f, 0.5f);
    setMidnaBindEffect(actor, &a_this->sound, &actor->eyePos, &effsize);
}

static int daE_CR_Execute(e_cr_class* a_this) {
    fopEn_enemy_c* actor = &a_this->enemy;
    cXyz sp18;
    cXyz spC;

    a_this->lifetime++;

    for (int i = 0; i < 4; i++) {
        if (a_this->timers[i] != 0) {
            a_this->timers[i]--;
        }
    }

    if (a_this->invulnerabilityTimer != 0) {
        a_this->invulnerabilityTimer--;
    }

    action(a_this);

    mDoMtx_stack_c::transS(actor->current.pos.x, actor->current.pos.y, actor->current.pos.z);
    mDoMtx_stack_c::YrotM(actor->shape_angle.y);
    mDoMtx_stack_c::XrotM(actor->shape_angle.x);
    mDoMtx_stack_c::scaleM(l_HIO.base_size, l_HIO.base_size, l_HIO.base_size);

    J3DModel* model = a_this->modelMorf->getModel();
    model->setBaseTRMtx(mDoMtx_stack_c::get());

    a_this->modelMorf->play(0, dComIfGp_getReverb(fopAcM_GetRoomNo(actor)));

    if (a_this->anm == 9 && (a_this->modelMorf->checkFrame(1.0f) || a_this->modelMorf->checkFrame(5.0f))) {
        a_this->sound.startCreatureSound(Z2SE_EN_CR_FOOTNOTE, 0, -1);
    }

    a_this->modelMorf->modelCalc();

    MTXCopy(model->getAnmMtx(1), *calc_mtx);
    sp18.set(0.0f, 0.0f, 0.0f);
    MtxPosition(&sp18, &actor->eyePos);
    actor->attention_info.position = actor->eyePos;
    actor->attention_info.position.y += 40.0f;

    sp18.set(0.0f, 0.0f, 0.0f);
    MtxPosition(&sp18, &spC);
    a_this->ccSph.SetC(spC);
    a_this->ccSph.SetR((20.0f + JREG_F(3)) * l_HIO.base_size);
    dComIfG_Ccsp()->Set(&a_this->ccSph);
    return 1;
}

static int daE_CR_IsDelete(e_cr_class* a_this) {
    return 1;
}

static int daE_CR_Delete(e_cr_class* a_this) {
    fopEn_enemy_c* actor = &a_this->enemy;
    fpc_ProcID id = fopAcM_GetID(a_this);
    dComIfG_resDelete(&a_this->phase, "E_CR");

    if (a_this->HIOInit) {
        l_HIOInit = FALSE;
        mDoHIO_DELETE_CHILD(l_HIO.id);
    }

    if (actor->heap != NULL) {
        a_this->modelMorf->stopZelAnime();
    }

    return 1;
}

static int useHeapInit(fopAc_ac_c* i_this) {
    e_cr_class* a_this = (e_cr_class*)i_this;

    a_this->modelMorf = new mDoExt_McaMorfSO((J3DModelData*)dComIfG_getObjectRes("E_CR", 0xD), NULL, NULL, (J3DAnmTransform*)dComIfG_getObjectRes("E_CR", 9), 2, 1.0f, 0, -1, &a_this->sound, 0x80000, 0x11000084);
    if (a_this->modelMorf == NULL || a_this->modelMorf->getModel() == NULL) {
        return 0;
    }

    J3DModel* model = a_this->modelMorf->getModel();
    model->setUserArea((uintptr_t)a_this);

    for (u16 i = 0; i < model->getModelData()->getJointNum(); i++) {
        if (i == 2) {
            model->getModelData()->getJointNodePointer(i)->setCallBack(nodeCallBack);
        }
    }

    return 1;
}

static int daE_CR_Create(fopAc_ac_c* i_this) {
    e_cr_class* a_this = (e_cr_class*)i_this;
    fopAcM_ct(i_this, e_cr_class);

    int phase_state = dComIfG_resLoad(&a_this->phase, "E_CR");
    if (phase_state == cPhs_COMPLEATE_e) {
        OS_REPORT("E_CR PARAM %x\n", fopAcM_GetParam(a_this));
        a_this->field_0x5b4 = fopAcM_GetParam(i_this);
        a_this->field_0x5b5 = (fopAcM_GetParam(i_this) & 0xF00) >> 8;
    
        OS_REPORT("E_CR//////////////E_CR SET 1 !!\n");
        if (!fopAcM_entrySolidHeap(i_this, useHeapInit, 0x1340)) {
            OS_REPORT("//////////////E_CR SET NON !!\n");
            return cPhs_ERROR_e;
        }
    
        OS_REPORT("//////////////E_CR SET 2 !!\n");
        if (!l_HIOInit) {
            a_this->HIOInit = TRUE;
            l_HIOInit = TRUE;
            l_HIO.id = mDoHIO_CREATE_CHILD("クレイジーランナー", &l_HIO);
        }
    
        i_this->attention_info.flags = fopAc_AttnFlag_BATTLE_e;
        fopAcM_SetMtx(i_this, a_this->modelMorf->getModel()->getBaseTRMtx());

        i_this->health = 1;
        i_this->field_0x560 = 1;

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

        a_this->ccStts.Init(100, 0, i_this);
        a_this->ccSph.Set(cc_sph_src);
        a_this->ccSph.SetStts(&a_this->ccStts);

        a_this->acch.Set(fopAcM_GetPosition_p(i_this), fopAcM_GetOldPosition_p(i_this), i_this, 1, &a_this->acchcir, fopAcM_GetSpeed_p(i_this), NULL, NULL);
        a_this->acchcir.SetWall(50.0f, 50.0f);
    
        a_this->sound.init(&i_this->current.pos, &i_this->eyePos, 3, 1);
        a_this->atInfo.mpSound = &a_this->sound;
        a_this->atInfo.mPowerType = 1;
        a_this->sound.setEnemyName("E_cr");

        daE_CR_Execute(a_this);
    }

    return phase_state;
}

static actor_method_class l_daE_CR_Method = {
    (process_method_func)daE_CR_Create,
    (process_method_func)daE_CR_Delete,
    (process_method_func)daE_CR_Execute,
    (process_method_func)daE_CR_IsDelete,
    (process_method_func)daE_CR_Draw,
};

extern actor_process_profile_definition g_profile_E_CR = {
    fpcLy_CURRENT_e,       // mLayerID
    7,                     // mListID
    fpcPi_CURRENT_e,       // mListPrio
    PROC_E_CR,             // mProcName
    &g_fpcLf_Method.base, // sub_method
    sizeof(e_cr_class),    // mSize
    0,                     // mSizeOther
    0,                     // mParameters
    &g_fopAc_Method.base,  // sub_method
    113,                   // mPriority
    &l_daE_CR_Method,      // sub_method
    0x10040100,            // mStatus
    fopAc_ENEMY_e,         // mActorType
    fopAc_CULLBOX_0_e,     // cullType
};
