/**
 * @file d_a_e_tk_ball.cpp
 *
 */

#include "d/dolzel_rel.h"  // IWYU pragma: keep

#include "d/actor/d_a_e_tk_ball.h"
#include "d/actor/d_a_player.h"
#include "d/d_s_play.h"

enum Action {
    /* 0x00 */ ACT_TK_BALL_MOVE,    // Move in a straight line
    /* 0x01 */ ACT_TK_BALL_RETURN,  // Return to sender (when player locks on target and reflects)
    /* 0x02 */ ACT_TK_BALL_DROP,    // Fall to the ground (when reflected without lock-on)
};

enum Mode {
    /* 0x00 */ MODE_TK_BALL_INIT,  // Initialization state
    /* 0x01 */ MODE_TK_BALL_MOVE,  // Active state
};

enum Type {
    /* 0x00 */ TYPE_TK_BALL_WATER,  // Water Toadpoli Ball
    /* 0x01 */ TYPE_TK_BALL_FIRE,   // Fire Toadpoli Ball
    /* 0xFF */ TYPE_TK_BALL_UNK = 0xFF,
};

static int daE_TK_BALL_Draw(e_tk_ball_class* i_this) {
    if (i_this->mSuspended) {
        return 1;
    }

    fopAc_ac_c* actor = i_this;

    g_env_light.settingTevStruct(0, &actor->current.pos, &actor->tevStr);
    g_env_light.setLightTevColorType_MAJI(i_this->mpModel, &actor->tevStr);
    mDoExt_modelUpdateDL(i_this->mpModel);
    return 1;
}

static int simple_bg_check(e_tk_ball_class* i_this) {
    fopAc_ac_c* actor = i_this;

    cXyz current_pos;
    cXyz start_pos;
    cXyz end_pos;
    cXyz ray_offset;
    cXyz mtx_offset;

    current_pos = actor->current.pos;
    current_pos.y += i_this->mArcHeight;
    cMtx_YrotS(*calc_mtx, actor->current.angle.y);

    ray_offset.x = 0.0f;
    ray_offset.y = 30.0f;
    ray_offset.z = -50.0f;

    MtxPosition(&ray_offset, &mtx_offset);
    start_pos = current_pos + mtx_offset;

    ray_offset.y = -30.0f;
    ray_offset.z = 50.0f;
    MtxPosition(&ray_offset, &mtx_offset);
    end_pos = current_pos + mtx_offset;

    dBgS_LinChk line_check;
    line_check.Set(&start_pos, &end_pos, actor);
    if (dComIfG_Bgsp().LineCross(&line_check)) {
        return 1;
    } else {
        return 0;
    }
}

static void impact_eff_set(e_tk_ball_class* i_this) {
    fopAc_ac_c* actor = i_this;
    cXyz pos = actor->current.pos;
    pos.y += i_this->mArcHeight;

    cXyz scale(2.0f + TREG_F(8), 2.0f + TREG_F(8), 2.0f + TREG_F(8));

    csXyz rotation = actor->current.angle;
    ADD_ANGLE(rotation.y, 0x8000);

    if (i_this->mType == TYPE_TK_BALL_WATER) {
        dComIfGp_particle_set(0x819B, &pos, &rotation, &scale);
        dComIfGp_particle_set(0x819C, &pos, &rotation, &scale);
        i_this->mSound.startSound(Z2SE_EN_TK_HIT, 0, -1);
    } else {
        dComIfGp_particle_set(0x8198, &pos, &rotation, &scale);
        dComIfGp_particle_set(0x8199, &pos, &rotation, &scale);
        i_this->mSound.startSound(Z2SE_EN_TK2_HIT, 0, -1);
    }
}

static void e_tk_ball_move(e_tk_ball_class* i_this) {
    fopAc_ac_c* actor = i_this;
    fopAc_ac_c* player = dComIfGp_getPlayer(0);

    cXyz direction_vec;
    cXyz speed_vec;
    cXyz unk1;

    switch (i_this->mMode) {
    case MODE_TK_BALL_INIT:
        i_this->mMode = MODE_TK_BALL_MOVE;
        i_this->mInitalPosition = actor->current.pos;
        direction_vec = player->eyePos;
        direction_vec.y += -20.0f + TREG_F(9);
        direction_vec -= i_this->mInitalPosition;

        i_this->mInitalDistance = direction_vec.abs();
        if (i_this->mInitalDistance < 10.0f) {
            i_this->mInitalDistance = 10.0f;
        }
        actor->current.angle.y = cM_atan2s(direction_vec.x, direction_vec.z);
        actor->current.angle.x =
            -cM_atan2s(direction_vec.y, JMAFastSqrt(direction_vec.x * direction_vec.x +
                                                    direction_vec.z * direction_vec.z));
        cMtx_YrotS(*calc_mtx, actor->current.angle.y);
        cMtx_XrotM(*calc_mtx, actor->current.angle.x);

        direction_vec.x = 0.0f;
        direction_vec.y = 0.0f;
        direction_vec.z = 50.0f + TREG_F(10);
        MtxPosition(&direction_vec, &actor->speed);

        i_this->mAtSph.OnAtVsPlayerBit();
        i_this->mAtSph.OffAtVsEnemyBit();
        i_this->mAtSph.StartCAt(actor->current.pos);
        i_this->mActionTimer[0] = 100;
        /* [[fallthrough]] */

    case MODE_TK_BALL_MOVE:
        actor->current.pos += actor->speed;
        break;
    }

    cXyz target_center = actor->current.pos;
    target_center.y += i_this->mArcHeight;
    i_this->mTgSph.SetC(target_center);
    dComIfG_Ccsp()->Set(&i_this->mTgSph);

    s8 actor_lockon = false;
    fopAc_ac_c* parent_actor = fopAcM_SearchByID(actor->parentActorID);
    if (parent_actor != NULL) {
        dAttention_c* attention = dComIfGp_getAttention();
        if (attention->Lockon() && parent_actor == attention->LockonTarget(0)) {
            actor_lockon = true;
            parent_actor->attention_info.flags |= fopAc_AttnFlag_UNK_0x200000;
        }
    }
    if (i_this->mTgSph.ChkTgHit() || i_this->mAtSph.ChkAtShieldHit()) {
        impact_eff_set(i_this);
        actor->current.angle.x *= -1;
        if (actor_lockon && daPy_getPlayerActorClass()->getCutType() != daPy_py_c::CUT_TYPE_NONE) {
            i_this->mAction = ACT_TK_BALL_RETURN;
            i_this->mMode = MODE_TK_BALL_INIT;
            ADD_ANGLE(actor->current.angle.y, 0x8000);
        } else {
            i_this->mAction = ACT_TK_BALL_DROP;
            i_this->mMode = MODE_TK_BALL_INIT;
            actor->speed.x *= -0.3f;
            actor->speed.z *= -0.3f;
            actor->speed.y = 0.0f;
            actor->current.pos += actor->speed;
            i_this->mActionTimer[0] = 60;
            return;
        }

        actor->current.pos.y += i_this->mArcHeight;
        actor->home.pos = actor->current.pos;
        direction_vec = i_this->mInitalPosition - actor->home.pos;
        i_this->mInitalDistance = direction_vec.abs();
        speed_vec.x = 0.0;
        speed_vec.y = 0.0;
        if (daPy_getPlayerActorClass()->getCutType() != daPy_py_c::CUT_TYPE_NONE) {
            speed_vec.z = 60.0f + TREG_F(16);
        }
        cMtx_YrotS(*calc_mtx, actor->current.angle.y);
        cMtx_XrotM(*calc_mtx, actor->current.angle.x);
        MtxPosition(&speed_vec, &actor->speed);
        i_this->mAtSph.OffAtVsPlayerBit();
        i_this->mAtSph.OnAtVsEnemyBit();
        i_this->mActionTimer[0] = 100;
        i_this->mActionTimer[1] = 10;
        actor->current.pos += actor->speed;
    } else {
        i_this->mAtSph.MoveCAt(actor->current.pos);
        dComIfG_Ccsp()->Set(&i_this->mAtSph);

        if (simple_bg_check(i_this) || i_this->mAtSph.ChkAtHit()) {
            impact_eff_set(i_this);
            fopAcM_delete(actor);
        }
    }
}

static void e_tk_ball_return(e_tk_ball_class* i_this) {
    fopAc_ac_c* actor = i_this;

    cXyz _unk1;
    cXyz _unk2;

    switch (i_this->mMode) {
    case MODE_TK_BALL_INIT:
        actor->current.pos += actor->speed;
        if (i_this->mActionTimer[1] == 0) {
            i_this->mTgSph.SetTgHitMark(CcG_Tg_UNK_MARK_0);
        }
        break;
    }

    cXyz new_center = actor->current.pos;
    new_center.y += i_this->mArcHeight;
    i_this->mAtSph.MoveCAt(new_center);
    dComIfG_Ccsp()->Set(&i_this->mAtSph);

    if (i_this->mActionTimer[1] == 0 && (simple_bg_check(i_this) || i_this->mAtSph.ChkAtHit())) {
        impact_eff_set(i_this);
        fopAcM_delete(actor);
    }
}

static void e_tk_ball_drop(e_tk_ball_class* i_this) {
    fopAc_ac_c* actor = i_this;

    cXyz _unk1;
    cXyz _unk2;

    switch (i_this->mMode) {
    case MODE_TK_BALL_INIT:
        actor->current.pos += actor->speed;
        actor->speed.y -= 2.0f;
        if (actor->speed.y < -50.0f) {
            actor->speed.y = -50.0f;
        }
        break;
    }

    if (simple_bg_check(i_this) || i_this->mActionTimer[0] == 0) {
        impact_eff_set(i_this);
        fopAcM_delete(actor);
    }
}

static void action(e_tk_ball_class* i_this) {
    fopAc_ac_c* actor = i_this;

    cXyz position_diff;
    cXyz _unk2;

    static u16 e_id[3] = {0x819D, 0x819E, 0x819A};

    s8 is_moving = true;
    switch (i_this->mAction) {
    case ACT_TK_BALL_MOVE:
        e_tk_ball_move(i_this);
        break;
    case ACT_TK_BALL_RETURN:
        e_tk_ball_return(i_this);
        break;
    case ACT_TK_BALL_DROP:
        e_tk_ball_drop(i_this);
        is_moving = false;
        break;
    }

    if (is_moving) {
        position_diff = actor->home.pos - actor->current.pos;
        f32 pos_diff_abs = position_diff.abs();
        f32 height_scale = i_this->mInitalDistance * (TREG_F(19) + 0.1f);
        if (height_scale > 300.0f + TREG_F(18)) {
            height_scale = 300.0f + TREG_F(18);
        }
        i_this->mArcHeight =
            height_scale * cM_ssin((pos_diff_abs / i_this->mInitalDistance) * 32768.0f);
        if (pos_diff_abs > i_this->mInitalDistance * 1.5f) {
            fopAcM_delete(actor);
        }
    } else {
        i_this->mArcHeight = 0.0f;
    }

    cXyz particle_position = actor->current.pos;
    particle_position.y += i_this->mArcHeight;

    cXyz particle_scale(2.0f + TREG_F(8), 2.0f + TREG_F(8), 2.0f + TREG_F(8));

    for (int i = 0; i < 2; i++) {
        i_this->mParticleKey[i] = dComIfGp_particle_set(
            i_this->mParticleKey[i], e_id[i + i_this->mType * 2], &particle_position, NULL, NULL);

        JPABaseEmitter* particle_emitter = dComIfGp_particle_getEmitter(i_this->mParticleKey[i]);
        if (particle_emitter != NULL) {
            particle_emitter->setGlobalScale(particle_scale);
            if (i_this->mType == TYPE_TK_BALL_FIRE) {
                particle_emitter->setParticleCallBackPtr(dPa_control_c::getParticleTracePCB());
                i_this->mParticleDirection = particle_position - i_this->mPreviousPosition;
                i_this->mParticleDirection *= 0.8f;
                particle_emitter->setUserWork((uintptr_t)&i_this->mParticleDirection);
                i_this->mPreviousPosition = particle_position;
                break;
            }
        }
    }

    if (i_this->mType == TYPE_TK_BALL_FIRE) {
        i_this->mSound.startLevelSound(Z2SE_EN_TK2_SHOT, 0, -1);
    } else {
        i_this->mSound.startLevelSound(Z2SE_EN_TK_SHOT, 0, -1);
    }
}

static int daE_TK_BALL_Execute(e_tk_ball_class* i_this) {
    if (i_this->mSuspended) {
        return 1;
    }

    fopAc_ac_c* actor = i_this;

    cXyz _unk1;
    cXyz _unk2;

    i_this->mLifetime++;
    for (int i = 0; i < 2; i++) {
        if (i_this->mActionTimer[i] != 0) {
            i_this->mActionTimer[i]--;
        }
    }
    if (i_this->mInvincibilityTimer != 0) {
        i_this->mInvincibilityTimer--;
    }

    action(i_this);

    ADD_ANGLE(actor->shape_angle.y, 0x1000);
    ADD_ANGLE(actor->shape_angle.x, 0xE00);

    mDoMtx_stack_c::transS(actor->current.pos.x, actor->current.pos.y + i_this->mArcHeight,
                           actor->current.pos.z);
    mDoMtx_stack_c::YrotM(actor->shape_angle.y);
    mDoMtx_stack_c::XrotM(actor->shape_angle.x);
    f32 scale = 2.0f + TREG_F(8);
    mDoMtx_stack_c::scaleM(scale, scale, scale);
    i_this->mpModel->setBaseTRMtx(mDoMtx_stack_c::get());

    i_this->mSound.framework(0, dComIfGp_getReverb(fopAcM_GetRoomNo(actor)));
    return 1;
}

static int daE_TK_BALL_IsDelete(e_tk_ball_class* i_this) {
    return 1;
}

static int daE_TK_BALL_Delete(e_tk_ball_class* i_this) {
    fopAc_ac_c* actor = i_this;
    fopAcM_RegisterDeleteID(i_this, "E_TK_BALL");
    if (i_this->mType == TYPE_TK_BALL_WATER) {
        dComIfG_resDelete(&i_this->mPhaseReq, "E_tk");
    } else {
        dComIfG_resDelete(&i_this->mPhaseReq, "E_tk2");
    }
    if (actor->heap != NULL) {
        i_this->mSound.deleteObject();
    }
    return 1;
}

static int useHeapInit(fopAc_ac_c* i_this) {
    e_tk_ball_class* actor = static_cast<e_tk_ball_class*>(i_this);
    J3DModelData* modelData;

    if (actor->mType == TYPE_TK_BALL_WATER) {
        modelData = (J3DModelData*)dComIfG_getObjectRes("E_tk", 0xD);
    } else {
        modelData = (J3DModelData*)dComIfG_getObjectRes("E_tk2", 0xD);
    }
    JUT_ASSERT(679, modelData != 0);
    actor->mpModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);

    if (actor->mpModel == NULL) {
        return 0;
    } else {
        return 1;
    }
}

static int daE_TK_BALL_Create(fopAc_ac_c* i_this) {
    e_tk_ball_class* actor = (e_tk_ball_class*)i_this;
    fopAcM_ct(actor, e_tk_ball_class);

    actor->mType = fopAcM_GetParam(i_this);
    if (actor->mType == TYPE_TK_BALL_UNK) {
        actor->mType = TYPE_TK_BALL_WATER;
    }

    cPhs_Step phase;
    u32 size;
    if (actor->mType == TYPE_TK_BALL_WATER) {
        phase = dComIfG_resLoad(&actor->mPhaseReq, "E_tk");
        size = 0x820;
    } else {
        phase = dComIfG_resLoad(&actor->mPhaseReq, "E_tk2");
        size = 0xEE0;
    }

    if (phase == cPhs_COMPLEATE_e) {
        OS_REPORT("E_tk_BALL PARAM %x\n", fopAcM_GetParam(i_this));
        actor->mArg1 = (fopAcM_GetParam(i_this) & 0xFF00) >> 8;
        if (actor->mArg1 == 0xff) {
            actor->mArg1 = 0x00;
        }

        OS_REPORT("E_tk_BALL//////////////E_TK_BALL SET 1 !!\n");

        if (!fopAcM_entrySolidHeap(i_this, useHeapInit, size)) {
            OS_REPORT("//////////////E_TK_BALL SET NON !!\n");
            return cPhs_ERROR_e;
        }

        OS_REPORT("//////////////E_TK_BALL SET 2 !!\n");

        static dCcD_SrcSph at_sph_src = {
            {
                {0x0, {{AT_TYPE_CSTATUE_SWING, 0x1, 0xd}, {0x0, 0x0}, 0x0}},  // mObj
                {dCcD_SE_METAL, 0x1, 0x0, 0x0, 0x0},                          // mGObjAt
                {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x2},                           // mGObjTg
                {0x0},                                                        // mGObjCo
            },                                                                // mObjInf
            {
                {{0.0f, 0.0f, 0.0f}, 20.0f}  // mSph
            }  // mSphAttr
        };

        static dCcD_SrcSph tg_sph_src = {
            {
                {0x0, {{0x0, 0x0, 0x0}, {0xd8fbfdff, 0x3}, 0x0}},  // mObj
                {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0},                // mGObjAt
                {dCcD_SE_METAL, 0x5, 0x0, 0x0, 0x2},               // mGObjTg
                {0x0},                                             // mGObjCo
            },                                                     // mObjInf
            {
                {{0.0f, 0.0f, 0.0f}, 35.0f}  // mSph
            }  // mSphAttr
        };

        fopAcM_SetMtx(i_this, actor->mpModel->getBaseTRMtx());
        actor->mStts.Init(0xff, 0, i_this);
        actor->mAtSph.Set(at_sph_src);
        actor->mAtSph.SetStts(&actor->mStts);
        if (actor->mType == TYPE_TK_BALL_FIRE) {
            actor->mAtSph.SetAtType(AT_TYPE_100);
            actor->mAtSph.SetAtMtrl(dCcD_MTRL_FIRE);
        }

        actor->mTgSph.Set(tg_sph_src);
        actor->mTgSph.SetStts(&actor->mStts);

        actor->mSound.init(&i_this->current.pos, 1);
        i_this->shape_angle.y = cM_rndFX(32768.0f);
        i_this->shape_angle.x = cM_rndFX(32768.0f);
        actor->mSuspended = true;

        mDoMtx_stack_c::scaleS(0.0f, 0.0f, 0.0f);
        actor->mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
        daE_TK_BALL_Execute(actor);
    }
    return phase;
}

static actor_method_class l_daE_TK_BALL_Method = {
    (process_method_func)daE_TK_BALL_Create,  (process_method_func)daE_TK_BALL_Delete,
    (process_method_func)daE_TK_BALL_Execute, (process_method_func)daE_TK_BALL_IsDelete,
    (process_method_func)daE_TK_BALL_Draw,
};

actor_process_profile_definition g_profile_E_TK_BALL = {
    fpcLy_CURRENT_e,          // mLayerID
    7,                        // mListID
    fpcPi_CURRENT_e,          // mListPrio
    PROC_E_TK_BALL,           // mProcName
    &g_fpcLf_Method.base,     // sub_method
    sizeof(e_tk_ball_class),  // mSize
    0,                        // mSizeOther
    0,                        // mParameters
    &g_fopAc_Method.base,     // sub_method
    151,                      // mPriority
    &l_daE_TK_BALL_Method,    // sub_method
    0x00040120,               // mStatus
    fopAc_ACTOR_e,            // mActorType
    fopAc_CULLBOX_0_e,        // cullType
};
