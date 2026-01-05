/**
 * f_op_scene.cpp
 * Scene Process Framework
 */

#include "f_op/f_op_scene.h"
#include "f_pc/f_pc_manager.h"
#include "f_op/f_op_scene_mng.h"
#include "m_Do/m_Do_hostIO.h"

static int fopScn_Draw(scene_class* i_this) {
    int ret = fpcNd_DrawMethod((nodedraw_method_class*)i_this->submethod, i_this);
    return ret;
}

static int fopScn_Execute(scene_class* i_this) {
    int ret = fpcMtd_Execute((process_method_class*)i_this->submethod, i_this);
    return ret;
}

static int fopScn_IsDelete(void* i_this) {
    int ret = fpcMtd_IsDelete((process_method_class*)((scene_class*) i_this)->submethod, i_this);
    return ret;
}

static int fopScn_Delete(void* i_this) {
    scene_class* scene = (scene_class*)i_this;
    int ret = 0;
    ret = fpcMtd_Delete((process_method_class*)scene->submethod, scene);
    if (ret == 1) {
        fopScnTg_QueueTo(&scene->scene_tag);
    }
#if DEBUG
    mDoHIO_update();
#endif
    return ret;
}

static int fopScn_Create(void* i_this) {
    scene_class* scene = (scene_class*)i_this;
    int ret;

    if (fpcM_IsFirstCreating(i_this)) {
        scene_process_profile_definition* profile = (scene_process_profile_definition*)fpcM_GetProfile(i_this);
        scene->submethod = profile->submethod;

        fopScnTg_Init(&scene->scene_tag, i_this);
        fopScnTg_ToQueue(&scene->scene_tag);

        u32* append = (u32*)fopScnM_GetAppend(i_this);
        if (append != NULL) {
            fopScnM_SetParam(i_this, *append);
        }
    }


    ret = fpcMtd_Create((process_method_class*)scene->submethod, scene);
    return ret;
}

leafdraw_method_class g_fopScn_Method = {
    (process_method_func)fopScn_Create,  
    (process_method_func)fopScn_Delete,
    (process_method_func)fopScn_Execute, 
    (process_method_func)fopScn_IsDelete,
    (process_method_func)fopScn_Draw
};
