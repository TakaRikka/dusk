/**
 * f_op_camera.cpp
 * Camera Process Framework
 */

#include "f_op/f_op_camera.h"
#include "f_op/f_op_camera_mng.h"
#include "d/d_com_inf_game.h"
#include "d/d_s_play.h"
#include "f_op/f_op_draw_tag.h"

static s32 fopCam_Draw(camera_class* i_this) {
    s32 ret = 1;
    if (!dComIfGp_isPauseFlag()) {
        ret = fpcLf_DrawMethod(i_this->submethod, i_this);
    }

    return ret;
}

static int fopCam_Execute(camera_class* i_this) {
    int ret;

    if (!dComIfGp_isPauseFlag() && dScnPly_c::isPause()) {
        ret = fpcMtd_Execute((process_method_class*)i_this->submethod, i_this);
    }

    return ret;
}

int fopCam_IsDelete(camera_class* i_this) {
    int ret = fpcMtd_IsDelete((process_method_class*)i_this->submethod, i_this);
    if (ret == 1) {
        fopDwTg_DrawQTo(&i_this->create_tag);
    }

    return ret;
}

int fopCam_Delete(camera_class* i_this) {
    int ret = fpcMtd_Delete((process_method_class*)i_this->submethod, i_this);
    if (ret == 1) {
        fopDwTg_DrawQTo(&i_this->create_tag);
    }

    return ret;
}

static int fopCam_Create(void* i_this) {
    camera_class* a_this = (camera_class*)i_this;

    if (fpcM_IsFirstCreating(i_this)) {
        camera_process_profile_definition* profile = (camera_process_profile_definition*)fpcM_GetProfile(i_this);
        a_this->submethod = profile->sub_method;

        fopDwTg_Init(&a_this->create_tag, a_this);
        u32* append = (u32*)fpcM_GetAppend(a_this);

        if (append != NULL) {
            fpcM_SetParam(a_this, *append);
        }
    }

    int ret = fpcMtd_Create(&a_this->submethod->base, a_this);
    if (ret == cPhs_COMPLEATE_e) {
        s32 priority = fpcM_DrawPriority(a_this);
        fopDwTg_ToDrawQ(&a_this->create_tag, priority);
    }

    return ret;
}

leafdraw_method_class g_fopCam_Method = {
    (process_method_func)fopCam_Create,  (process_method_func)fopCam_Delete,
    (process_method_func)fopCam_Execute, (process_method_func)fopCam_IsDelete,
    (process_method_func)fopCam_Draw,
};
