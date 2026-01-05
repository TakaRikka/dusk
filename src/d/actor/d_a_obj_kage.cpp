/**
 * @file d_a_obj_kage.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_kage.h"
#include "d/actor/d_a_player.h"
#include "d/d_s_play.h"

static int daObj_Kage_Draw(obj_kage_class* i_this) {
    fopAc_ac_c* actor = &i_this->actor;

    g_env_light.settingTevStruct(16, &actor->current.pos, &actor->tevStr);
    g_env_light.setLightTevColorType_MAJI(i_this->model, &actor->tevStr);
    mDoExt_modelUpdateDL(i_this->model);
    return 1;
}

static int daObj_Kage_Execute(obj_kage_class* i_this) {
    fopAc_ac_c* actor = &i_this->actor;
    cXyz sp14;
    cXyz sp8;

    i_this->counter++;

    for (int i = 0; i < 2; i++) {
        if (i_this->timers[i] != 0) {
            i_this->timers[i]--;
        }
    }

    if (i_this->field_0x59e != 0) {
        i_this->field_0x59e--;
    }

    f32 var_f31 = 0.0f;
    s16 angle_x;
    s16 angle_z;
    if (i_this->field_0x574 != 1) {
        angle_x = i_this->field_0x5a4 * cM_ssin(i_this->counter * 1700);
        angle_z = i_this->field_0x5a4 * cM_ssin(i_this->counter * 1500);
        cLib_addCalc2(&i_this->field_0x5a4, i_this->field_0x5a8, 0.5f, 40.0f);
        cLib_addCalc2(&i_this->field_0x5a8, 120.0f + KREG_F(11), 0.1f, 2.0f);
    } else {
        var_f31 = -120.0f + XREG_F(16);
        if (daPy_getPlayerActorClass()->checkFrontRollCrash() && fopAcM_searchPlayerDistanceXZ(actor) < 200.0f) {
            i_this->field_0x5a4 = 500.0f + XREG_F(17);
        }

        angle_x = i_this->field_0x5a4 * cM_ssin(i_this->counter * 4700);
        angle_z = i_this->field_0x5a4 * cM_ssin(i_this->counter * 4400);
        cLib_addCalc2(&i_this->field_0x5a4, 0.0f, 1.0f, 10.0f);
    }

    actor->shape_angle.x = angle_x;
    actor->shape_angle.z = angle_z;

    mDoMtx_stack_c::transS(actor->current.pos.x, actor->current.pos.y + var_f31, actor->current.pos.z);
    mDoMtx_stack_c::YrotM(actor->shape_angle.y);
    mDoMtx_stack_c::XrotM(actor->shape_angle.x);
    mDoMtx_stack_c::ZrotM(actor->shape_angle.z);
    mDoMtx_stack_c::transM(0.0f, -var_f31, 0.0f);
    i_this->model->setBaseTRMtx(mDoMtx_stack_c::get());
    return 1;
}

static int daObj_Kage_IsDelete(obj_kage_class* i_this) {
    return 1;
}

static int daObj_Kage_Delete(obj_kage_class* i_this) {
    fopAc_ac_c* actor = &i_this->actor;
    fpc_ProcID id = fopAcM_GetID(i_this);
    dComIfG_resDelete(&i_this->phase, "Obj_kage");
    return 1;
}

static int useHeapInit(fopAc_ac_c* actor) {
    obj_kage_class* i_this = (obj_kage_class*)actor;

    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes("Obj_kage", 3);
    JUT_ASSERT(267, modelData != NULL);

    i_this->model = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
    if (i_this->model == NULL) {
        return 0;
    }

    return 1;
}

static int daObj_Kage_Create(fopAc_ac_c* actor) {
    fopAcM_ct(actor, obj_kage_class);
    obj_kage_class* i_this = (obj_kage_class*)actor;

    int phase_state = dComIfG_resLoad(&i_this->phase, "Obj_kage");
    if (phase_state == cPhs_COMPLEATE_e) {
        OS_REPORT("OBJ_KAGE PARAM %x\n", fopAcM_GetParam(actor));
        i_this->field_0x574 = fopAcM_GetParam(actor);

        OS_REPORT("OBJ_KAGE//////////////OBJ_KAGE SET 1 !!\n");
        if (!fopAcM_entrySolidHeap(actor, useHeapInit, 0xEC0)) {
            OS_REPORT("//////////////OBJ_KAGE SET NON !!\n");
            return cPhs_ERROR_e;
        }

        OS_REPORT("//////////////OBJ_KAGE SET 2 !!\n");
        i_this->counter = cM_rndF(65536.0f);

        fopAcM_SetMtx(actor, i_this->model->getBaseTRMtx());
        fopAcM_SetMin(actor, -200.0f, -200.0f, -200.0f);
        fopAcM_SetMax(actor, 200.0f, 200.0f, 200.0f);

        i_this->inko_child_id = fopAcM_createChild(PROC_NPC_INKO, fopAcM_GetID(actor), 0, &actor->current.pos, fopAcM_GetRoomNo(actor), NULL, NULL, -1, NULL);

        daObj_Kage_Execute(i_this);
    }

    return phase_state;
}

static actor_method_class l_daObj_Kage_Method = {
    (process_method_func)daObj_Kage_Create,
    (process_method_func)daObj_Kage_Delete,
    (process_method_func)daObj_Kage_Execute,
    (process_method_func)daObj_Kage_IsDelete,
    (process_method_func)daObj_Kage_Draw,
};

actor_process_profile_definition g_profile_OBJ_KAGE = {
  fpcLy_CURRENT_e,        // mLayerID
  7,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_OBJ_KAGE,          // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(obj_kage_class), // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  62,                     // mPriority
  &l_daObj_Kage_Method,   // sub_method
  0x00040100,             // mStatus
  fopAc_ACTOR_e,          // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};
