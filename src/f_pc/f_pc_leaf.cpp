/**
 * f_pc_leaf.cpp
 * Framework - Process Leaf
 */

#include "f_pc/f_pc_leaf.h"
#include "f_pc/f_pc_debug_sv.h"

s16 fpcLf_GetPriority(const leafdraw_class* i_leaf) {
    return fpcDwPi_Get(&i_leaf->draw_priority);
}

s32 fpcLf_DrawMethod(leafdraw_method_class* i_methods, void* i_process) {
    return fpcMtd_Method(i_methods->draw_method, i_process);
}

s32 fpcLf_Draw(leafdraw_class* i_leaf) {
    s32 ret = 0;
    if (i_leaf->unk_0xBC == 0)
        ret = fpcLf_DrawMethod(i_leaf->leaf_methods, i_leaf);
    return ret;
}

s32 fpcLf_Execute(leafdraw_class* i_leaf) {
#if DEBUG
    if (fpcBs_Is_JustOfType(g_fpcLf_type, i_leaf->base.subtype) == 0) {
        if (g_fpcDbSv_service[12] != NULL) {
            g_fpcDbSv_service[12](i_leaf);
        }
        return 0;
    }
#endif
    return fpcMtd_Execute(&i_leaf->leaf_methods->base, i_leaf);
}

s32 fpcLf_IsDelete(leafdraw_class* i_leaf) {
    return fpcMtd_IsDelete(&i_leaf->leaf_methods->base, i_leaf);
}

s32 fpcLf_Delete(leafdraw_class* i_leaf) {
    s32 ret = fpcMtd_Delete(&i_leaf->leaf_methods->base, i_leaf);
    if (ret == 1) {
        i_leaf->base.subtype = 0;
    }
    return ret;
}

int g_fpcLf_type;

s32 fpcLf_Create(leafdraw_class* i_leaf) {
    if (i_leaf->base.state.init_state == 0) {
        leaf_process_profile_definition* pprofile = (leaf_process_profile_definition*)i_leaf->base.profile;
        i_leaf->leaf_methods = pprofile->sub_method;
        i_leaf->base.subtype = fpcBs_MakeOfType(&g_fpcLf_type);
        fpcDwPi_Init(&i_leaf->draw_priority, pprofile->priority);
        i_leaf->unk_0xBC = 0;
    }

    int ret = fpcMtd_Create(&i_leaf->leaf_methods->base, i_leaf);
    return ret;
}

leafdraw_method_class g_fpcLf_Method = {
    (process_method_func)fpcLf_Create,  (process_method_func)fpcLf_Delete,
    (process_method_func)fpcLf_Execute, (process_method_func)fpcLf_IsDelete,
    (process_method_func)fpcLf_Draw,
};
