/**
 * d_a_disappear.cpp
 * Disappear death effect when an enemy is defeated
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_disappear.h"
#include "d/d_com_inf_game.h"
#include "f_pc/f_pc_name.h"

static int daDisappear_Draw(disappear_class* i_this) {
    return 1;
}

static void* s_ks_sub(void* i_actor, void*) {
    fopAc_ac_c* actor_p = (fopAc_ac_c*)i_actor;

    if (fopAcM_IsActor(actor_p) && fopAcM_GetName(actor_p) == fpcNm_NPC_KS_e) {
        actor_p->health = 1;
    }

    return NULL;
}

static int daDisappear_Execute(disappear_class* i_this) {
    if (i_this->mDeleteTimer != 0) {
        i_this->mDeleteTimer--;
    } else {
        fopAcM_delete(i_this);
    }

    return 1;
}

static int daDisappear_IsDelete(disappear_class* i_this) {
    return 1;
}

static int daDisappear_Delete(disappear_class* i_this) {
    return 1;
}

static void yami_disappear(disappear_class* i_this, f32 i_scale) {
    static u16 da_name[] = {0x826C, 0x826D, 0x826E, 0x826B};

    cXyz scale;
    scale.set(i_scale, i_scale, i_scale);

    int n = 3;
    if (i_this->field_0x567 == 2) {
        n = 4;
    }

    for (int i = 0; i < n; i++) {
        dComIfGp_particle_set(da_name[i], &i_this->current.pos, NULL, &scale);
    }
}

static void ghost_disappear(disappear_class* i_this, f32 i_scale) {
    static u16 da_name[] = {0x86A7, 0x86A8, 0x86A9, 0x86AA, 0x86AB};

    cXyz scale;
    scale.set(i_scale, i_scale, i_scale);

    for (int i = 0; i < 5; i++) {
        dComIfGp_particle_set(da_name[i], &i_this->current.pos, NULL, &scale);
    }
}

static void set_disappear(disappear_class* i_this, f32 i_scale) {
    static u16 da_name[] = {0x0108, 0x0109, 0x010A, 0x010B, 0x010C, 0x0154, 0x0155};

    dKy_Sound_set(i_this->current.pos, 120, fopAcM_GetID(i_this), 10);

    fpcM_Search(s_ks_sub, i_this);

    if (i_this->field_0x567 == 3) {
        fopAcM_seStart(i_this, JA_SE_CM_MONS_EXPLODE, 0);
        ghost_disappear(i_this, i_scale);
    } else if (i_this->field_0x567 >= 1) {
        fopAcM_seStart(i_this, Z2SE_DARK_VANISH, 0);
        yami_disappear(i_this, i_scale * 1.666f);
    } else {
        fopAcM_seStart(i_this, JA_SE_CM_MONS_EXPLODE, 0);

        cXyz scale;
        scale.set(i_scale, i_scale, i_scale);

        i_this->mDeleteTimer = 58;
        for (int i = 0; i < 7; i++) {
            dComIfGp_particle_set(da_name[i], &i_this->current.pos, NULL, &scale);
        }
    }
}

static int daDisappear_Create(fopAc_ac_c* i_this) {
    fopAcM_ct(i_this, disappear_class);
    disappear_class* this_ = (disappear_class*)i_this;

    this_->field_0x567 = fopAcM_GetParam(this_);

    f32 particle_scale = (fopAcM_GetParam(this_) >> 0x8) & 0xFF;
    if (particle_scale >= 50.0f) {
        particle_scale *= 0.01f;
    } else {
        particle_scale *= 0.1f;
    }

    u8 enemy_id = fopAcM_GetParam(this_) >> 0x10;
    if (enemy_id != 0xFF) {
        fopAcM_createItemFromEnemyID(enemy_id, &this_->current.pos, -1, -1, NULL, NULL, NULL, NULL);
    }

    set_disappear(this_, particle_scale);
    return cPhs_COMPLEATE_e;
}

static actor_method_class l_daDisappear_Method = {
    (process_method_func)daDisappear_Create,  (process_method_func)daDisappear_Delete,
    (process_method_func)daDisappear_Execute, (process_method_func)daDisappear_IsDelete,
    (process_method_func)daDisappear_Draw,
};

actor_process_profile_definition g_profile_DISAPPEAR = {
    /* Layer ID     */ fpcLy_CURRENT_e,
    /* List ID      */ 7,
    /* List Prio    */ fpcPi_CURRENT_e,
    /* Proc Name    */ fpcNm_DISAPPEAR_e,
    /* Proc SubMtd  */ &g_fpcLf_Method.base,
    /* Size         */ sizeof(disappear_class),
    /* Size Other   */ 0,
    /* Parameters   */ 0,
    /* Leaf SubMtd  */ &g_fopAc_Method.base,
    /* Draw Prio    */ fpcDwPi_DISAPPEAR_e,
    /* Actor SubMtd */ &l_daDisappear_Method,
    /* Status       */ fopAcStts_UNK_0x40000_e | fopAcStts_UNK_0x4000_e,
    /* Group        */ fopAc_ACTOR_e,
    /* Cull Type    */ fopAc_CULLBOX_0_e,
};
