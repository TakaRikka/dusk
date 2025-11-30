/**
 * @file d_a_e_gs.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_e_gs.h"
#include "d/d_com_inf_game.h"
#include "d/actor/d_a_player.h"
#include "c/c_damagereaction.h"

daE_GS_HIO_c::daE_GS_HIO_c() {
    id = -1;
    base_size = 1.2f;
    appear_time_lag = 5;
    appear_alpha_speed = 60.0f;
    disappear_alpha_speed = 30.0f;
    react_distance = 300.0f;
}

static void anm_init(e_gs_class* a_this, int i_anm, f32 i_morf, u8 i_mode, f32 i_speed) {
    a_this->model_morf->setAnm((J3DAnmTransform*)dComIfG_getObjectRes("E_gs", i_anm), i_mode, i_morf, i_speed, 0.0f, -1.0f);
    a_this->anm = i_anm;
}

static int daE_GS_Draw(e_gs_class* a_this) {
    fopAc_ac_c* actor = &a_this->enemy;

    J3DModel* model = a_this->model_morf->getModel();
    if (a_this->opacity < 1.0f) {
        return 1;
    }

    g_env_light.settingTevStruct(7, &actor->current.pos, &actor->tevStr);
    actor->tevStr.TevColor.a = (u8)a_this->opacity;
    g_env_light.setLightTevColorType_MAJI(model, &actor->tevStr);

    J3DModelData* modelData = model->getModelData();
    u8 alpha = a_this->opacity;

    for (u16 i = 0; i < modelData->getMaterialNum(); i++) {
        modelData->getMaterialNodePointer(i)->getTevKColor(3)->a = alpha;
    }

    a_this->invisible_model.entryDL(NULL);
    return 1;
}

static u8 l_HIOInit;

static daE_GS_HIO_c l_HIO;

static void e_gs_wait(e_gs_class* a_this) {
    switch (a_this->mode) {
    case 0:
        anm_init(a_this, 6, 40.0f, 2, 1.0f);
        a_this->mode = 1;
        break;
    case 1:
        if (a_this->player_dist < l_HIO.react_distance - 50.0f) {
            anm_init(a_this, 4, 10.0f, 0, 1.0f);
            a_this->mode = 2;
        }
        break;
    case 2:
        if (a_this->model_morf->isStop()) {
            anm_init(a_this, 5, 10.0f, 2, 1.0f);
            a_this->mode = 3;
            a_this->timers[1] = 20.0f + cM_rndF(20.0f);
        }
        break;
    case 3:
        if (a_this->player_dist > 50.0f + l_HIO.react_distance && a_this->timers[1] == 0) {
            a_this->mode = 0;
        }
        break;
    }
}

static void action(e_gs_class* a_this) {
    fopAc_ac_c* actor = &a_this->enemy;

    cXyz sp14;
    cXyz sp8;
    a_this->player_dist = fopAcM_searchPlayerDistance(actor);
    a_this->angle_to_player = fopAcM_searchPlayerAngleY(actor);

    switch (a_this->action) {
    case 0:
        e_gs_wait(a_this);
        break;
    }

    cLib_addCalcAngleS2(&actor->shape_angle.y, actor->current.angle.y, 2, 0x2000);
    cLib_addCalcAngleS2(&actor->shape_angle.x, actor->current.angle.x, 2, 0x2000);
    cLib_addCalcAngleS2(&actor->shape_angle.z, actor->current.angle.z, 2, 0x2000);
}

static int daE_GS_Execute(e_gs_class* a_this) {
    fopAc_ac_c* actor = &a_this->enemy;

    f32 alpha_target = 0.0f;
    f32 alpha_speed = l_HIO.disappear_alpha_speed;

    if (daPy_py_c::checkNowWolfPowerUp()) {
        if (a_this->timers[0] == 0) {
            alpha_target = 255.0f;
            alpha_speed = l_HIO.appear_alpha_speed;
        }
    } else {
        a_this->timers[0] = l_HIO.appear_time_lag;
    }

    cLib_addCalc2(&a_this->opacity, alpha_target, 1.0f, alpha_speed);

    if (cDmrNowMidnaTalk() || dComIfGp_event_runCheck()) {
        return 1;
    }

    cXyz sp20;
    cXyz sp14;
    cXyz sp8;
    a_this->lifetime++;

    for (int i = 0; i < 4; i++) {
        if (a_this->timers[i] != 0) {
            a_this->timers[i]--;
        }
    }

    if (a_this->field_0x692 != 0) {
        a_this->field_0x692--;
    }

    action(a_this);

    mDoMtx_stack_c::transS(actor->current.pos.x, actor->current.pos.y, actor->current.pos.z);
    mDoMtx_stack_c::YrotM(actor->shape_angle.y);
    mDoMtx_stack_c::XrotM(actor->shape_angle.x);
    mDoMtx_stack_c::ZrotM(actor->shape_angle.z);
    mDoMtx_stack_c::scaleM(l_HIO.base_size, l_HIO.base_size, l_HIO.base_size);
    a_this->model_morf->getModel()->setBaseTRMtx(mDoMtx_stack_c::get());

    a_this->model_morf->play(0, dComIfGp_getReverb(fopAcM_GetRoomNo(actor)));
    a_this->model_morf->modelCalc();
    return 1;
}

static int daE_GS_IsDelete(e_gs_class* a_this) {
    return 1;
}

static int daE_GS_Delete(e_gs_class* a_this) {
    fopAc_ac_c* actor = &a_this->enemy;

    fopAcM_GetID(actor);
    dComIfG_resDelete(&a_this->phase, "E_gs");

    if (a_this->hio_init) {
        l_HIOInit = FALSE;
        mDoHIO_DELETE_CHILD(l_HIO.id);
    }

    if (actor->heap != NULL) {
        a_this->model_morf->stopZelAnime();
    }

    return 1;
}

static int useHeapInit(fopAc_ac_c* i_this) {
    e_gs_class* a_this = (e_gs_class*)i_this;

    a_this->model_morf = new mDoExt_McaMorfSO((J3DModelData* )dComIfG_getObjectRes("E_gs", 9), NULL, NULL, (J3DAnmTransform*)dComIfG_getObjectRes("E_gs", 6), 2, 1.0f, 0, -1, &a_this->sound, 0x80000, 0x11000084);
    if (a_this->model_morf == NULL || a_this->model_morf->getModel() == NULL) {
        return 0;
    }

    if (!a_this->invisible_model.create(a_this->model_morf->getModel(), 1)) {
        return 0;
    }

    MtxScale(0.0f, 0.0f, 0.0f, 0);
    a_this->model_morf->getModel()->setBaseTRMtx(*calc_mtx);
    return 1;
}

static int daE_GS_Create(fopAc_ac_c* i_this) {
    e_gs_class* a_this = (e_gs_class*)i_this;
    fopAcM_ct(i_this, e_gs_class);

    int phase_state = dComIfG_resLoad(&a_this->phase, "E_gs");
    if (phase_state == cPhs_COMPLEATE_e) {
        OS_REPORT("E_gs PARAM %x\n", fopAcM_GetParam(i_this));
        a_this->arg0 = fopAcM_GetParam(i_this);
        a_this->arg1 = (fopAcM_GetParam(i_this) & 0xFF00) >> 8;

        OS_REPORT("E_GS//////////////E_GS SET 1 !!\n");
        if (!fopAcM_entrySolidHeap(i_this, useHeapInit, 0x4B000)) {
            OS_REPORT("//////////////E_GS SET NON !!\n");
            return cPhs_ERROR_e;
        }

        if (!l_HIOInit) {
            a_this->hio_init = TRUE;
            l_HIOInit = TRUE;
            l_HIO.id = mDoHIO_CREATE_CHILD("幽霊兵士", &l_HIO);
        }

        fopAcM_SetMtx(i_this, a_this->model_morf->getModel()->getBaseTRMtx());
        fopAcM_SetMin(i_this, -200.0f, -200.0f, -200.0f);
        fopAcM_SetMax(i_this, 200.0f, 200.0f, 200.0f);

        i_this->health = 100;
        i_this->field_0x560 = 100;

        a_this->sound.init(&i_this->current.pos, &i_this->eyePos, 3, 1);
        a_this->atInfo.mpSound = &a_this->sound;

        daE_GS_Execute(a_this);
    }

    return phase_state;
}

static actor_method_class l_daE_GS_Method = {
    (process_method_func)daE_GS_Create,
    (process_method_func)daE_GS_Delete,
    (process_method_func)daE_GS_Execute,
    (process_method_func)daE_GS_IsDelete,
    (process_method_func)daE_GS_Draw,
};

extern actor_process_profile_definition g_profile_E_GS = {
  fpcLy_CURRENT_e,        // mLayerID
  7,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_E_GS,              // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(e_gs_class),     // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  115,                    // mPriority
  &l_daE_GS_Method,       // sub_method
  0x00044100,             // mStatus
  fopAc_ENEMY_e,          // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};
