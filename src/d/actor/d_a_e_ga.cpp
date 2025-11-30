/**
 * @file d_a_e_ga.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_e_ga.h"
#include "d/d_com_inf_game.h"
#include "d/d_s_play.h"

daE_Ga_HIO_c::daE_Ga_HIO_c() {
    id = -1;
    base_size = 1.0f;
}

static int daE_Ga_Draw(e_ga_class* a_this) {
    fopAc_ac_c* actor = &a_this->actor;

    g_env_light.settingTevStruct(0, &actor->current.pos, &actor->tevStr);
    
    ga_s* ga_s = a_this->ga_s;
    int room_no = fopAcM_GetRoomNo(actor);

    for (int i = 0; i < a_this->ga_num; i++, ga_s++) {
        if (ga_s->initialized) {
            dComIfGp_entrySimpleModel(ga_s->model, room_no);
        }
    }

    return 1;
}

static void ga_fly(e_ga_class* a_this, ga_s* ga_s) {
    fopAc_ac_c* actor = &a_this->actor;

    cXyz sp20;
    cXyz sp14;
    f32 move_range = 100.0f * a_this->prm2;

    if (ga_s->move_timer == 0) {
        ga_s->move_timer = cM_rndF(10.0f);
        ga_s->pos_target.x = actor->home.pos.x + cM_rndFX(move_range);
        ga_s->pos_target.y = actor->home.pos.y + cM_rndF(200.0f);
        ga_s->pos_target.z = actor->home.pos.z + cM_rndFX(move_range);

        sp20 = ga_s->pos_target - ga_s->position;
        ga_s->rot_target.y = cM_atan2s(sp20.x, sp20.z);
        ga_s->rot_target.x = -cM_atan2s(sp20.y, JMAFastSqrt((sp20.x * sp20.x) + (sp20.z * sp20.z)));
    }

    cLib_addCalcAngleS2(&ga_s->rotation.y, ga_s->rot_target.y, 2, 0x1000);
    cLib_addCalcAngleS2(&ga_s->rotation.x, ga_s->rot_target.x, 2, 0x1000);

    sp20.x = 0.0f;
    sp20.y = 0.0f;
    sp20.z = 10.0f;
    cMtx_YrotS(*calc_mtx, ga_s->rotation.y);
    cMtx_XrotM(*calc_mtx, ga_s->rotation.x);
    MtxPosition(&sp20, &sp14);
    ga_s->position += sp14;

    if (ga_s->position.y < actor->home.pos.y) {
        ga_s->position.y = actor->home.pos.y;
        ga_s->move_timer = 0;
    }

    ga_s->field_0x3c = cM_ssin(ga_s->field_0x44);
    ga_s->field_0x44 += 0x3E00;
}

static void bt_fly(e_ga_class* a_this, ga_s* ga_s) {
    fopAc_ac_c* actor = &a_this->actor;

    cXyz sp20;
    cXyz sp14;
    f32 move_range = 100.0f * a_this->prm2;

    if (ga_s->move_timer == 0) {
        ga_s->move_timer = cM_rndF(12.0f);
        ga_s->pos_target.x = actor->home.pos.x + cM_rndFX(move_range);
        ga_s->pos_target.y = actor->home.pos.y + cM_rndF(200.0f);
        ga_s->pos_target.z = actor->home.pos.z + cM_rndFX(move_range);

        sp20 = ga_s->pos_target - ga_s->position;
        ga_s->rot_target.y = cM_atan2s(sp20.x, sp20.z);
        ga_s->rot_target.x = -cM_atan2s(sp20.y, JMAFastSqrt((sp20.x * sp20.x) + (sp20.z * sp20.z)));
    }

    cLib_addCalcAngleS2(&ga_s->rotation.y, ga_s->rot_target.y, 2, 0x800);
    cLib_addCalcAngleS2(&ga_s->rotation.x, ga_s->rot_target.x, 2, 0x800);

    sp20.x = 0.0f;
    sp20.y = 0.0f;
    sp20.z = 7.0f;
    cMtx_YrotS(*calc_mtx, ga_s->rotation.y);
    cMtx_XrotM(*calc_mtx, ga_s->rotation.x);
    MtxPosition(&sp20, &sp14);
    ga_s->position.x += sp14.x;
    ga_s->position.y += sp14.y;
    ga_s->position.z += sp14.z;

    if (ga_s->position.y < actor->home.pos.y) {
        ga_s->position.y = actor->home.pos.y;
        ga_s->move_timer = 0;
    }

    ga_s->field_0x44 += 0x2E00;
    ga_s->field_0x3c = cM_ssin(ga_s->field_0x44);
}

static void ga_control(e_ga_class* a_this) {
    cXyz sp10;
    ga_s* ga_s = a_this->ga_s;

    f32 offset_y;
    if (a_this->prm1 == 0) {
        offset_y = -15.0f + TREG_F(10);
    } else {
        offset_y = -5.0f + TREG_F(9);
    }

    for (int i = 0; i < a_this->ga_num; i++, ga_s++) {
        if (ga_s->initialized) {
            if (ga_s->move_timer != 0) {
                ga_s->move_timer--;
            }

            if (a_this->prm1 == 0) {
                ga_fly(a_this, ga_s);
            } else {
                bt_fly(a_this, ga_s);
            }

            mDoMtx_stack_c::transS(ga_s->position.x, ga_s->position.y, ga_s->position.z);
            mDoMtx_stack_c::YrotM(ga_s->rotation.y);
            mDoMtx_stack_c::XrotM(ga_s->rotation.x);
            mDoMtx_stack_c::scaleM(ga_s->size, ga_s->size * ga_s->field_0x3c, ga_s->size);
            mDoMtx_stack_c::transM(0.0f, offset_y, 0.0f);
            ga_s->model->setBaseTRMtx(mDoMtx_stack_c::get());
        }
    }
}

static int daE_Ga_Execute(e_ga_class* a_this) {
    ga_control(a_this);
    return 1;
}

static int daE_Ga_IsDelete(e_ga_class* a_this) {
    return 1;
}

static u8 l_HIOInit;

static int daE_Ga_Delete(e_ga_class* a_this) {
    fopAc_ac_c* actor = &a_this->actor;
    fpc_ProcID id = fopAcM_GetID(actor);

    if (a_this->initialized) {
        J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes("E_ga", 3);
        dComIfGp_removeSimpleModel(modelData, fopAcM_GetRoomNo(actor));
    }

    dComIfG_resDelete(&a_this->phase, "E_ga");

    if (a_this->HIOInit) {
        l_HIOInit = FALSE;
    }

    return 1;
}

static int useHeapInit(fopAc_ac_c* i_this) {
    e_ga_class* a_this = (e_ga_class*)i_this;

    void* modelData = dComIfG_getObjectRes("E_Ga", 3);
    JUT_ASSERT(450, modelData != NULL);

    for (int i = 0; i < a_this->ga_num; i++) {
        a_this->ga_s[i].model = mDoExt_J3DModel__create((J3DModelData*)modelData, 0x20000, 0x11000084);
        if (a_this->ga_s[i].model == NULL) {
            return 0;
        }
    }

    return 1;
}

static daE_Ga_HIO_c l_HIO;

static int daE_Ga_Create(fopAc_ac_c* i_this) {
    static const int l_bmdidx[] = {3};

    e_ga_class* a_this = (e_ga_class*)i_this;
    fopAcM_ct(i_this, e_ga_class);

    int phase_state = dComIfG_resLoad(&a_this->phase, "E_Ga");
    if (phase_state == cPhs_COMPLEATE_e) {
        OS_REPORT("E_GA PARAM %x\n", fopAcM_GetParam(i_this));
        a_this->prm0 = fopAcM_GetParam(i_this);
        a_this->prm1 = (fopAcM_GetParam(i_this) & 0xFF00) >> 8;
        a_this->prm2 = (fopAcM_GetParam(i_this) & 0xFF0000) >> 0x10;

        if (a_this->prm1 == 0xFF) {
            a_this->prm1 = 0;
        }

        if (a_this->prm2 == 0xFF) {
            a_this->prm1 = 2;
        }

        a_this->ga_num = a_this->prm0 + 1;
        if (a_this->ga_num > 0x100) {
            a_this->ga_num = 0x100;
        }

        OS_REPORT("E_GA//////////////E_GA SET 1 !!\n");
        if (!fopAcM_entrySolidHeap(i_this, useHeapInit, 0x1000)) {
            OS_REPORT("//////////////E_GA SET NON !!\n");
            return cPhs_ERROR_e;
        }

        OS_REPORT("//////////////E_GA SET 2 !!\n");
        if (!l_HIOInit) {
            a_this->HIOInit = TRUE;
            l_HIOInit = TRUE;
            l_HIO.id = -1;
        }

        for (int i = 0; i < a_this->ga_num; i++) {
            a_this->ga_s[i].initialized = TRUE;
            a_this->ga_s[i].position = i_this->home.pos;
            a_this->ga_s[i].field_0x44 = cM_rndF(0x10000);

            if (a_this->prm1 == 0) {
                a_this->ga_s[i].size = 0.3f + cM_rndF(0.3f);
            } else {
                a_this->ga_s[i].size = 0.8f + cM_rndF(0.2f);
            }
        }

        daE_Ga_Execute(a_this);

        J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes("E_ga", 3);
        JUT_ASSERT(536, modelData != NULL);

        if (dComIfGp_addSimpleModel(modelData, fopAcM_GetRoomNo(i_this), FALSE) == -1) {
            // "Moth: Simple Model addition failed.\n"
            OS_REPORT("\x1B[43;30m蛾：シンプルモデル登録失敗しました。\n\x1B[m");
        }
    
        a_this->initialized = TRUE;
    }

    return phase_state;
}

static actor_method_class l_daE_Ga_Method = {
    (process_method_func)daE_Ga_Create,
    (process_method_func)daE_Ga_Delete,
    (process_method_func)daE_Ga_Execute,
    (process_method_func)daE_Ga_IsDelete,
    (process_method_func)daE_Ga_Draw,
};

extern actor_process_profile_definition g_profile_E_GA = {
  fpcLy_CURRENT_e,        // mLayerID
  7,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_E_GA,              // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(e_ga_class),     // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  138,                    // mPriority
  &l_daE_Ga_Method,       // sub_method
  0x00044000,             // mStatus
  fopAc_ACTOR_e,          // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};
