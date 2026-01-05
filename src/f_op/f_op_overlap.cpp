/**
 * f_op_overlap.cpp
 * Overlap Process Framework
 */

#include "f_op/f_op_overlap.h"
#include "JSystem/JKernel/JKRExpHeap.h"
#include "d/d_error_msg.h"
#include "f_pc/f_pc_manager.h"
#include "m_Do/m_Do_ext.h"

static s32 fopOvlp_Draw(void* i_this) {
    s32 ret = fpcLf_DrawMethod(((overlap_task_class*)i_this)->submethod, i_this);
    return ret;
}

static s32 fopOvlp_Execute(void* i_this) {
    s32 ret = fpcMtd_Execute(&((overlap_task_class*)i_this)->submethod->base, i_this);
#if VERSION == VERSION_SHIELD || PLATFORM_WII
    dConnectErrorMsg_c::disable();
#endif
    return ret;
}

static s32 fopOvlp_IsDelete(void* i_this) {
    s32 ret = fpcMtd_IsDelete(&((overlap_task_class*)i_this)->submethod->base, i_this);
    return ret;
}

static s32 fopOvlp_Delete(void* i_this) {
    s32 ret = 0;
    ret = fpcMtd_Delete(&((overlap_task_class*)i_this)->submethod->base, i_this);
#if DEBUG
    if (ret == 1 && mDoExt_getSafeZeldaHeapSize() >= 0) {
        mDoExt_addSafeZeldaHeapSize(mDoExt_getZeldaHeap()->getSize(i_this));
    }
#endif
    return ret;
}

static s32 fopOvlp_Create(void* i_this) {
    overlap_task_class* a_this = (overlap_task_class*)i_this;

    if (fpcM_IsFirstCreating(a_this)) {
        overlap_process_profile_definition* profile =
            (overlap_process_profile_definition*)fpcM_GetProfile(a_this);

        cReq_Create(&a_this->request, 1);
        a_this->submethod = profile->sub_method;
        a_this->scene_id = fpcM_ERROR_PROCESS_ID_e;
#if DEBUG
        if (mDoExt_getSafeZeldaHeapSize() >= 0) {
            mDoExt_addSafeZeldaHeapSize(-mDoExt_getZeldaHeap()->getSize(i_this));
        }
#endif
    }

    s32 ret = fpcMtd_Create(&a_this->submethod->base, a_this);
    return ret;
}

leafdraw_method_class g_fopOvlp_Method = {
    (process_method_func)fopOvlp_Create,  (process_method_func)fopOvlp_Delete,
    (process_method_func)fopOvlp_Execute, (process_method_func)fopOvlp_IsDelete,
    (process_method_func)fopOvlp_Draw,
};
