/**
 * f_op_view.cpp
 * View Process Framework
 */

#include "f_op/f_op_view.h"
#include "f_pc/f_pc_manager.h"

void fopVw_Draw(view_class* i_this) {
    fpcLf_DrawMethod(i_this->sub_method, i_this);
}

void fopVw_Execute(view_class* i_this) {
    fpcMtd_Execute(&i_this->sub_method->base, i_this);
}

s32 fopVw_IsDelete(void* i_this) {
    view_class* a_this = (view_class*)i_this;
    return fpcMtd_IsDelete(&a_this->sub_method->base, a_this);
}

s32 fopVw_Delete(view_class* i_this) {
    return fpcMtd_Delete(&i_this->sub_method->base, i_this);
}

s32 fopVw_Create(void* i_this) {
    view_class* a_this = (view_class*)i_this;

    view_process_profile_definition* profile = (view_process_profile_definition*)fpcM_GetProfile(a_this);
    a_this->sub_method = profile->sub_method;
    a_this->field_0xc4 = profile->unk_0x28;

    return fpcMtd_Create(&a_this->sub_method->base, a_this);
}

leafdraw_method_class g_fopVw_Method = {
    (process_method_func)fopVw_Create,  (process_method_func)fopVw_Delete,
    (process_method_func)fopVw_Execute, (process_method_func)fopVw_IsDelete,
    (process_method_func)fopVw_Draw,
};
