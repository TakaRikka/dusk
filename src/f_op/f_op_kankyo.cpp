/**
 * f_op_kankyo.cpp
 * Environment process framework
 */

#include "f_op/f_op_kankyo.h"
#include "d/d_com_inf_game.h"
#include "d/d_s_play.h"
#include "f_op/f_op_draw_tag.h"
#include "f_op/f_op_kankyo_mng.h"

static int fopKy_Draw(void* i_this) {
    int ret;
    kankyo_class* a_this = (kankyo_class*)i_this;

    if (!dComIfGp_isPauseFlag()) {
        ret = fpcLf_DrawMethod(a_this->sub_method, i_this);
    }

    return ret;
}

static int fopKy_Execute(void* i_this) {
    int ret;
    kankyo_class* a_this = (kankyo_class*)i_this;

    if (dScnPly_c::isPause() && (!dComIfGp_isPauseFlag() || fpcM_GetName(i_this) == PROC_ENVSE)) {
        ret = fpcMtd_Execute(&a_this->sub_method->base, i_this);
    }

    return ret;
}

static int fopKy_IsDelete(void* i_this) {
    int ret;
    kankyo_class* a_this = (kankyo_class*)i_this;

    ret = fpcMtd_IsDelete(&a_this->sub_method->base, a_this);
    if (ret == 1) {
        fopDwTg_DrawQTo(&a_this->draw_tag);
    }

    return ret;
}

static int fopKy_Delete(void* i_this) {
    kankyo_class* a_this = (kankyo_class*)i_this;

    int ret = fpcMtd_Delete(&a_this->sub_method->base, a_this);
    fopDwTg_DrawQTo(&a_this->draw_tag);

    return ret;
}

static int fopKy_KANKYO_TYPE;

static int fopKy_Create(void* i_this) {
    kankyo_class* a_this = (kankyo_class*)i_this;

    if (fpcM_IsFirstCreating(i_this)) {
        kankyo_process_profile_definition* profile = (kankyo_process_profile_definition*)fpcM_GetProfile(i_this);

        a_this->type = fpcBs_MakeOfType(&fopKy_KANKYO_TYPE);
        a_this->sub_method = profile->sub_method;

        fopDwTg_Init(&a_this->draw_tag, a_this);
        fopKyM_prm_class* append = fopKyM_GetAppend(a_this);

        if (append != NULL) {
            a_this->pos = append->pos;
            a_this->scale = append->scale;
            a_this->parameters = append->parameters;
        }
    }

    int ret = fpcMtd_Create(&a_this->sub_method->base, a_this);
    if (ret == cPhs_COMPLEATE_e) {
        s32 priority = fpcM_DrawPriority(a_this);
        fopDwTg_ToDrawQ(&a_this->draw_tag, priority);
    }

    return ret;
}

leafdraw_method_class g_fopKy_Method = {
    (process_method_func)fopKy_Create,  (process_method_func)fopKy_Delete,
    (process_method_func)fopKy_Execute, (process_method_func)fopKy_IsDelete,
    (process_method_func)fopKy_Draw,
};
