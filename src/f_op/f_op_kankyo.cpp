/**
 * f_op_kankyo.cpp
 * Environment process framework
 */

#include "f_op/f_op_kankyo.h"
#include "d/d_com_inf_game.h"
#include "d/d_s_play.h"
#include "f_ap/f_ap_game.h"
#include "f_op/f_op_draw_tag.h"
#include "f_op/f_op_kankyo_mng.h"

static int fopKy_KANKYO_TYPE;

void fopKy_IsKankyo(void* i_this) {
    fpcM_IsJustType(fopKy_KANKYO_TYPE, ((kankyo_class*)i_this)->type);
}

static int fopKy_Draw(void* i_this) {
    int ret;

#if DEBUG
    fapGm_HIO_c::startCpuTimer();
#endif

    if (!dComIfGp_isPauseFlag()) {
        ret = fpcLf_DrawMethod(((kankyo_class*)i_this)->sub_method, i_this);
    }

#if DEBUG
    fapGm_HIO_c::stopCpuTimer("環境関係（描画処理）"); // Environmental aspects (rendering processing)
#endif

    return ret;
}

static int fopKy_Execute(void* i_this) {
    kankyo_class* a_this = (kankyo_class*)i_this;
    int ret;
    
#if DEBUG
    fapGm_HIO_c::startCpuTimer();
#endif

    if (!dScnPly_c::isPause() && (!dComIfGp_isPauseFlag() || fpcM_GetName(a_this) == PROC_ENVSE)) {
        ret = fpcMtd_Execute(&((kankyo_class*)i_this)->sub_method->base, i_this);
    }

#if DEBUG
    fapGm_HIO_c::stopCpuTimer("環境関係（計算処理）"); // Environmental aspects (computational processing)
#endif

    return ret;
}

static int fopKy_IsDelete(void* i_this) {
    int ret;

    ret = fpcMtd_IsDelete(&((kankyo_class*)i_this)->sub_method->base, i_this);
    if (ret == 1) {
        fopDwTg_DrawQTo(&((kankyo_class*)i_this)->draw_tag);
    }

    return ret;
}

static int fopKy_Delete(void* i_this) {
    kankyo_class* a_this = (kankyo_class*)i_this;

    int ret = fpcMtd_Delete(&a_this->sub_method->base, a_this);
    fopDwTg_DrawQTo(&a_this->draw_tag);

    return ret;
}

static int fopKy_Create(void* i_this) {
    kankyo_class* a_this = (kankyo_class*)i_this;
    int ret;

    if (fpcM_IsFirstCreating(i_this)) {
        kankyo_process_profile_definition* profile = (kankyo_process_profile_definition*)fpcM_GetProfile(i_this);

        a_this->type = fpcM_MakeOfType(&fopKy_KANKYO_TYPE);
        a_this->sub_method = profile->sub_method;

        fopDwTg_Init(&a_this->draw_tag, a_this);
        fopKyM_prm_class* append = fopKyM_GetAppend(a_this);

        if (append != NULL) {
            a_this->pos = append->pos;
            a_this->scale = append->scale;
            a_this->parameters = append->parameters;
        }
    }

    ret = fpcMtd_Create(&a_this->sub_method->base, a_this);
    if (ret == cPhs_COMPLEATE_e) {
        fopDwTg_ToDrawQ(&a_this->draw_tag, fpcM_DrawPriority(a_this));
    }

    return ret;
}

leafdraw_method_class g_fopKy_Method = {
    (process_method_func)fopKy_Create,  (process_method_func)fopKy_Delete,
    (process_method_func)fopKy_Execute, (process_method_func)fopKy_IsDelete,
    (process_method_func)fopKy_Draw,
};
