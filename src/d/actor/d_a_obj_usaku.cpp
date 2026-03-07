/**
 * d_a_obj_usaku.cpp
 * Horse Fence
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_usaku.h"
#include "d/d_bg_w.h"
#include "d/d_com_inf_game.h"
#include "d/d_procname.h"

static int daObj_Usaku_Draw(obj_usaku_class* i_this) {
    fopAc_ac_c* a_this = static_cast<fopAc_ac_c*>(i_this);
    g_env_light.settingTevStruct(0x10, &a_this->current.pos, &a_this->tevStr);
    g_env_light.setLightTevColorType_MAJI(i_this->mpModel, &a_this->tevStr);
    mDoExt_modelUpdateDL(i_this->mpModel);

    return 1;
}

static int daObj_Usaku_Execute(obj_usaku_class* i_this) {
    fopAc_ac_c* a_this = static_cast<fopAc_ac_c*>(i_this);
    mDoMtx_stack_c::transS(a_this->current.pos.x, a_this->current.pos.y, a_this->current.pos.z);
    mDoMtx_stack_c::YrotM(a_this->shape_angle.y);

    i_this->mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
    if (i_this->mpBgW != NULL) {
        MTXCopy(mDoMtx_stack_c::get(), i_this->mBgMtx);
        i_this->mpBgW->Move();
    }

    return 1;
}

static int daObj_Usaku_IsDelete(obj_usaku_class* i_this) {
    return 1;
}

static int daObj_Usaku_Delete(obj_usaku_class* i_this) {
    fopAc_ac_c* a_this = static_cast<fopAc_ac_c*>(i_this);
    fopAcM_RegisterDeleteID(i_this, "Obj_Usaku");
    dComIfG_resDelete(&i_this->mPhase, "Obj_usaku");

    if (i_this->mpBgW != NULL) {
        dComIfG_Bgsp().Release(i_this->mpBgW);
    }
    return 1;
}

static int useHeapInit(fopAc_ac_c* i_this) {
    obj_usaku_class* a_this = (obj_usaku_class*)i_this;

    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes("Obj_usaku", 4);
    JUT_ASSERT(209, modelData != NULL);
    a_this->mpModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
    if (a_this->mpModel == NULL) {
        return 0;
    }
    OS_REPORT("//////////////OBJ_USAKU SET 5 !!\n");

    a_this->mpBgW = new dBgW();
    if (a_this->mpBgW == NULL) {
        return 0;
    }

    if (a_this->mpBgW->Set((cBgD_t*)dComIfG_getObjectRes("Obj_usaku", 7), cBgW::MOVE_BG_e,
                           &a_this->mBgMtx) == 1)
    {
        return 0;
    }

    a_this->mpBgW->SetCrrFunc(dBgS_MoveBGProc_Typical);
    OS_REPORT("//////////////OBJ_USAKU SET 6 !!\n");
    return 1;
}

static int daObj_Usaku_Create(fopAc_ac_c* i_this) {
    obj_usaku_class* a_this = (obj_usaku_class*)i_this;
    fopAcM_ct(a_this, obj_usaku_class);

    int phase = dComIfG_resLoad(&a_this->mPhase, "Obj_usaku");
    if (phase == cPhs_COMPLEATE_e) {
        OS_REPORT("OBJ_USAKU PARAM %x\n", fopAcM_GetParam(i_this));
        a_this->field_0x570 = fopAcM_GetParam(i_this) & 0xFF;
        if (a_this->field_0x570 == 0xFF) {
            a_this->field_0x570 = 0;
        }
        OS_REPORT("OBJ_USAKU//////////////OBJ_USAKU SET 1 !!\n");

        if (!fopAcM_entrySolidHeap(i_this, useHeapInit, 0xC60)) {
            OS_REPORT("//////////////OBJ_USAKU SET NON !!\n");
            return cPhs_ERROR_e;
        }
        OS_REPORT("//////////////OBJ_USAKU SET 2 !!\n");

        OS_REPORT("//////////////OBJ_USAKU SET 3 !!\n");

        if (a_this->mpBgW != NULL && dComIfG_Bgsp().Regist(a_this->mpBgW, a_this)) {
            return cPhs_ERROR_e;
        }
        OS_REPORT("//////////////OBJ_USAKU SET 4 !!\n");

        fopAcM_SetMtx(i_this, a_this->mpModel->getBaseTRMtx());
        fopAcM_SetMin(i_this, -300.0f, -600.0f, -300.0f);
        fopAcM_SetMax(i_this, 300.0f, 600.0f, 300.0f);
        daObj_Usaku_Execute(a_this);
    }

    return phase;
}

static actor_method_class l_daObj_Usaku_Method = {
    (process_method_func)daObj_Usaku_Create,  (process_method_func)daObj_Usaku_Delete,
    (process_method_func)daObj_Usaku_Execute, (process_method_func)daObj_Usaku_IsDelete,
    (process_method_func)daObj_Usaku_Draw,
};

actor_process_profile_definition g_profile_OBJ_USAKU = {
    fpcLy_CURRENT_e,
    7,
    fpcPi_CURRENT_e,
    PROC_OBJ_USAKU,
    &g_fpcLf_Method.base,
    sizeof(obj_usaku_class),
    0,
    0,
    &g_fopAc_Method.base,
    78,
    &l_daObj_Usaku_Method,
    0x40100,
    fopAc_ACTOR_e,
    fopAc_CULLBOX_CUSTOM_e,
};
