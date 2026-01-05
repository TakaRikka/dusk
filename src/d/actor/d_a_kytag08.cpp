/**
 * d_a_kytag08.cpp
 * Fog Avoidance Tag
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_kytag08.h"
#include "d/actor/d_a_player.h"
#include "d/d_com_inf_game.h"
#include "d/actor/d_a_spinner.h"

static int daKytag08_Draw(kytag08_class* i_this) {
    return 1;
}

static int daKytag08_Execute(kytag08_class* i_this) {
    dScnKy_env_light_c* env_light = dKy_getEnvlight();
    daPy_py_c* player_p = daPy_getPlayerActorClass();
    JGeometry::TVec3<f32> particle_scale;

    i_this->field_0x5d8 = 1;
    if (i_this->field_0x5d8 == 0) {
        i_this->current.pos.x = player_p->current.pos.x;
        i_this->current.pos.y = 100.0f;
        i_this->current.pos.z = player_p->current.pos.z;
    } else if (env_light->field_0x0c18[0].field_0x26 == 1) {
        i_this->current.pos.x = env_light->field_0x0c18[0].mPos.x;
        i_this->current.pos.y = 100.0f;
        i_this->current.pos.z = env_light->field_0x0c18[0].mPos.z;
    }

    if (i_this->mAvoidPos.abs(i_this->current.pos) < 2000.0f && !dComIfGp_event_runCheck()) {
        if (i_this->mSizeTimer > 100) {
            f32 max_step;
            f32 accel_rate;
            if (i_this->field_0x5d8 != 0) {
                max_step = 20.0f;
                accel_rate = 0.1f;
            } else {
                max_step = 10.0f;
                accel_rate = 0.2f;
            }

            cLib_addCalc(&i_this->mAvoidPos.x, i_this->mTargetAvoidPos.x, accel_rate, max_step,
                         0.00001f);
            cLib_addCalc(&i_this->mAvoidPos.z, i_this->mTargetAvoidPos.z, accel_rate, max_step,
                         0.00001f);
        } else {
            f32 max_step;
            f32 accel_rate;
            if (i_this->field_0x5d8 != 0) {
                max_step = 5.0f;
                accel_rate = 0.02f;
            } else {
                max_step = 8.0f;
                accel_rate = 0.05f;
            }

            cLib_addCalc(&i_this->mAvoidPos.x, i_this->mTargetAvoidPos.x, accel_rate, max_step,
                         0.1f);
            cLib_addCalc(&i_this->mAvoidPos.z, i_this->mTargetAvoidPos.z, accel_rate, max_step,
                         0.1f);
        }
    } else {
        i_this->mAvoidPos.x = i_this->current.pos.x;
        i_this->mAvoidPos.z = i_this->current.pos.z;
    }

    if (!dKy_shadow_mode_check(2)) {
        if (!dComIfGp_event_runCheck()) {
            cLib_addCalc(&i_this->mSizeScale, 0.0f, 0.2f, 0.005f, 0.001f);
        }
    } else if (i_this->mSizeTimer != 0) {
        cLib_addCalc(&i_this->mSizeScale, 1.0f, 0.5f, 0.01f, 0.001f);
    } else if (!dComIfGp_event_runCheck()) {
        cLib_addCalc(&i_this->mSizeScale, 0.25f, 0.01f, 0.002f, 0.001f);
    }

    mDoAud_setFogWipeWidth(i_this->mSizeScale);

    if (i_this->field_0x5d8 != 0) {
        f32 width = 37.5f;
        if (i_this->mAvoidPos.abs(i_this->current.pos) >
            i_this->mSize.x * width * i_this->mSizeScale)
        {
            dComIfGs_BossLife_public_Set(2);
        }
    }

    if ((daPy_getPlayerActorClass()->checkKandelaarSwing(TRUE) && i_this->mSizeTimer < 100) ||
        dComIfGs_BossLife_public_Get() == 1)
    {
        dComIfGs_BossLife_public_Set(0);
        i_this->mTargetAvoidPos = i_this->current.pos;
        i_this->mSizeTimer = 180;
        mDoAud_startFogWipeTrigger(&i_this->current.pos);
    } else {
        f32 max_step;
        f32 accel_rate;
        if (i_this->field_0x5d8 != 0) {
            max_step = 4000.0f;
            accel_rate = 0.5f;
        } else {
            max_step = 4.0f;
            accel_rate = 0.025f;
        }

        cLib_addCalc(&i_this->mTargetAvoidPos.x, i_this->current.pos.x, accel_rate, max_step,
                     0.01f);
        cLib_addCalc(&i_this->mTargetAvoidPos.z, i_this->current.pos.z, accel_rate, max_step,
                     0.01f);
    }

    if (i_this->mSizeTimer != 0) {
        i_this->mSizeTimer--;
    }

    if (i_this->mpEmitter1 != NULL) {
        i_this->mpEmitter1->setGlobalTranslation(i_this->mAvoidPos.x, i_this->mAvoidPos.y,
                                                 i_this->mAvoidPos.z);
        i_this->mpEmitter1->setVolumeSize(i_this->mSize.x * 50.0f * i_this->mSizeScale);

        f32 temp_f0_7 = 1.0f - i_this->mSizeScale;
        particle_scale.x = 1.0f - (temp_f0_7 * temp_f0_7);
        particle_scale.y = 1.0f - (temp_f0_7 * temp_f0_7);
        particle_scale.z = 1.0f - (temp_f0_7 * temp_f0_7);
        i_this->mpEmitter1->setGlobalParticleScale(particle_scale);
    }

    if (i_this->mSizeScale <= 0.001f) {
        i_this->mAvoidPos.x = i_this->current.pos.x;
        i_this->mAvoidPos.z = i_this->current.pos.z;

        i_this->mTargetAvoidPos = i_this->mAvoidPos;
    }

    f32 fog_height = 20.0f;
    if (dKy_darkworld_check()) {
        fog_height = 56.0f;
    }

    f32 player_y_pos;
    if (player_p->getSpinnerActor()) {
        player_y_pos = player_p->getSpinnerActor()->current.pos.y;
    } else {
        player_y_pos = player_p->current.pos.y;
    }

    if (player_y_pos < fog_height) {
        cXyz player_pos(player_p->current.pos);
        player_pos.y = 100.0f;

        if (i_this->mAvoidPos.abs(player_pos) > i_this->mSize.x * 45.0f * i_this->mSizeScale ||
            i_this->mSizeScale <= 0.01f)
        {
            player_p->onFogFade();
        }
    }

    if (player_p != NULL && i_this->mpEmitter2 != NULL) {
        i_this->mpEmitter2->setGlobalTranslation(player_p->current.pos.x, 100.0f,
                                                 player_p->current.pos.z);

        particle_scale.z = 0.0f;
        particle_scale.y = 0.0f;
        particle_scale.x = 0.0f;

        if (player_p->current.pos.y < 100.0f) {
            cXyz player_pos(player_p->current.pos);
            player_pos.y = 100.0f;

            if (i_this->mAvoidPos.abs(player_pos) > i_this->mSize.x * 45.0f * i_this->mSizeScale) {
                f32 scale_factor = (100.0f - player_p->current.pos.y) / 30.0f;

                if (scale_factor > 1.0f) {
                    scale_factor = 1.0f;
                }

                particle_scale.z = scale_factor;
                particle_scale.y = scale_factor;
                particle_scale.x = scale_factor;
            }
        }

        i_this->mpEmitter2->setGlobalParticleScale(particle_scale);
    }

    return 1;
}

static int daKytag08_IsDelete(kytag08_class* i_this) {
    return 1;
}

static int daKytag08_Delete(kytag08_class* i_this) {
    if (i_this->mpEmitter1 != NULL) {
        i_this->mpEmitter1->deleteAllParticle();
        i_this->mpEmitter1->becomeInvalidEmitter();
        i_this->mpEmitter1->quitImmortalEmitter();
        i_this->mpEmitter1->setEmitterCallBackPtr(NULL);
        i_this->mpEmitter1 = NULL;
    }

    if (i_this->mpEmitter2 != NULL) {
        i_this->mpEmitter2->deleteAllParticle();
        i_this->mpEmitter2->becomeInvalidEmitter();
        i_this->mpEmitter2->quitImmortalEmitter();
        i_this->mpEmitter2->setEmitterCallBackPtr(NULL);
        i_this->mpEmitter2 = NULL;
    }

    dComIfG_resDelete(&i_this->mPhase, "Kytag08");
    return 1;
}

static int useHeapInit(fopAc_ac_c* i_this) {
    kytag08_class* a_this = (kytag08_class*)i_this;

    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes("Kytag08", 3);
    JUT_ASSERT(0, modelData != NULL);

    a_this->mpModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11020202);

    // wtf, necessary to match
    if (a_this->mpModel == NULL) {
        return 0;
    } else if (a_this->mpModel != NULL) {
        return 1;
    }
    return 1;
}

static int daKytag08_Create(fopAc_ac_c* i_this) {
    kytag08_class* a_this = (kytag08_class*)i_this;
    cXyz scale(1.0f, 1.0f, 1.0f);
    dScnKy_env_light_c* env_light = dKy_getEnvlight();

    fopAcM_ct(a_this, kytag08_class);

    int phase = dComIfG_resLoad(&a_this->mPhase, "Kytag08");
    if (phase == cPhs_COMPLEATE_e) {
        if (!fopAcM_entrySolidHeap(i_this, useHeapInit, 0x550)) {
            return cPhs_ERROR_e;
        }

        a_this->mSize.x = 20.0f;
        a_this->mSize.y = 20.0f;
        a_this->mSize.z = 20.0f;
        a_this->mSizeScale = 0.0f;

        a_this->mpEmitter1 = dComIfGp_particle_set(0x84A0, &i_this->current.pos, NULL, &scale);

        if (!dKy_darkworld_check()) {
            a_this->mpEmitter2 = dComIfGp_particle_set(0x84A1, &i_this->current.pos, NULL, &scale);
        } else {
            a_this->mpEmitter2 = dComIfGp_particle_set(0x84A2, &i_this->current.pos, NULL, &scale);
        }

        a_this->mAvoidPos = i_this->current.pos;
        a_this->mAvoidPos.y = 100.0f;
        a_this->mTargetAvoidPos = a_this->mAvoidPos;

        env_light->fog_avoid_tag = a_this;
    }

    return phase;
}

static actor_method_class l_daKytag08_Method = {
    (process_method_func)daKytag08_Create,  (process_method_func)daKytag08_Delete,
    (process_method_func)daKytag08_Execute, (process_method_func)daKytag08_IsDelete,
    (process_method_func)daKytag08_Draw,
};

actor_process_profile_definition g_profile_KYTAG08 = {
    fpcLy_CURRENT_e,
    7,
    fpcPi_CURRENT_e,
    PROC_KYTAG08,
    &g_fpcLf_Method.base,
    sizeof(kytag08_class),
    0,
    0,
    &g_fopAc_Method.base,
    102,
    &l_daKytag08_Method,
    0x44000,
    fopAc_ACTOR_e,
    fopAc_CULLBOX_0_e,
};
