/**
 * f_op_overlap.cpp
 * Overlap Process Framework
 */

#include "f_op/f_op_overlap.h"
#include "f_pc/f_pc_manager.h"

static s32 fopOvlp_Draw(void* i_this) {
    overlap_task_class* a_this = (overlap_task_class*)i_this;
    return fpcLf_DrawMethod(a_this->submethod, i_this);
}

static s32 fopOvlp_Execute(void* i_this) {
    overlap_task_class* a_this = (overlap_task_class*)i_this;
    return fpcMtd_Execute(&a_this->submethod->base, i_this);
}

static s32 fopOvlp_IsDelete(void* i_this) {
    overlap_task_class* a_this = (overlap_task_class*)i_this;
    return fpcMtd_IsDelete(&a_this->submethod->base, i_this);
}

static s32 fopOvlp_Delete(void* i_this) {
    overlap_task_class* a_this = (overlap_task_class*)i_this;
    return fpcMtd_Delete(&a_this->submethod->base, i_this);
}

static s32 fopOvlp_Create(void* i_this) {
    overlap_task_class* a_this = (overlap_task_class*)i_this;

    if (fpcM_IsFirstCreating(a_this)) {
        overlap_process_profile_definition* profile =
            (overlap_process_profile_definition*)fpcM_GetProfile(a_this);

        cReq_Create(&a_this->request, 1);
        a_this->submethod = profile->sub_method;
        a_this->scene_id = fpcM_ERROR_PROCESS_ID_e;
    }

    return fpcMtd_Create(&a_this->submethod->base, a_this);
}

leafdraw_method_class g_fopOvlp_Method = {
    (process_method_func)fopOvlp_Create,  (process_method_func)fopOvlp_Delete,
    (process_method_func)fopOvlp_Execute, (process_method_func)fopOvlp_IsDelete,
    (process_method_func)fopOvlp_Draw,
};
