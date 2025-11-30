/**
 * f_pc_method.cpp
 * Framework - Process Method
 */

#include "f_pc/f_pc_method.h"

s32 fpcMtd_Method(process_method_func i_method, void* i_process) {
    if (i_method != NULL)
        return i_method(i_process);
    else
        return 1;
}

s32 fpcMtd_Execute(process_method_class* i_methods, void* i_process) {
    return fpcMtd_Method(i_methods->execute_method, i_process);
}

s32 fpcMtd_IsDelete(process_method_class* i_methods, void* i_process) {
    return fpcMtd_Method(i_methods->is_delete_method, i_process);
}

s32 fpcMtd_Delete(process_method_class* i_methods, void* i_process) {
    return fpcMtd_Method(i_methods->delete_method, i_process);
}

s32 fpcMtd_Create(process_method_class* i_methods, void* i_process) {
    return fpcMtd_Method(i_methods->create_method, i_process);
}
