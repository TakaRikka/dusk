/**
 * f_op_kankyo.cpp
 * Environment process manager
 */

#include "f_op/f_op_kankyo_mng.h"
#include "SSystem/SComponent/c_malloc.h"
#include "d/d_com_inf_game.h"

void dummy(fpcLyIt_JudgeFunc i_createFunc, void* i_this) {
    fpcM_Search(i_createFunc, i_this);
}

void fopKyM_IsKy(void* i_this) {
    fopKy_IsKankyo((fopKyM_prm_class*)i_this);
}

fopKyM_prm_class* fopKyM_CreateAppend() {
    fopKyM_prm_class* append = (fopKyM_prm_class*)cMl::memalignB(-4, sizeof(fopKyM_prm_class));

    if (append != NULL) {
        cLib_memSet(append, 0, sizeof(fopKyM_prm_class));
        append->scale.x = 1.0f;
        append->scale.y = 1.0f;
        append->scale.z = 1.0f;
    }
    return append;
}

static fopKyM_prm_class* createAppend(int i_param, cXyz* i_pos, cXyz* i_scale) {
    fopKyM_prm_class* append = fopKyM_CreateAppend();
    if (append == NULL) {
        return NULL;
    }

    if (i_pos != NULL) {
        append->pos = *i_pos;
    }

    if (i_scale != NULL) {
        append->scale = *i_scale;
    }

    append->parameters = i_param;

    return append;
}

void fopKyM_Delete(void* i_process) {
    fpcM_Delete(i_process);
}

fpc_ProcID fopKyM_Create(s16 i_procName, fopKyM_CreateFunc i_createFunc, void* i_append) {
    return fpcM_Create(i_procName, i_createFunc, i_append);
}

fpc_ProcID fopKyM_create(s16 i_procName, int i_param, cXyz* i_pos, cXyz* i_scale,
                         fopKyM_CreateFunc i_createFunc) {
    fopKyM_prm_class* append = createAppend(i_param, i_pos, i_scale);
    if (append == NULL) {
        return fpcM_ERROR_PROCESS_ID_e;
    }

    return fopKyM_Create(i_procName, i_createFunc, append);
}

base_process_class* fopKyM_fastCreate(s16 i_procName, int i_param, cXyz* i_pos, cXyz* i_scale,
                                      fopKyM_CreateFunc i_createFunc) {
    fopKyM_prm_class* append = createAppend(i_param, i_pos, i_scale);
    if (append == NULL) {
        return NULL;
    }

    return fpcM_FastCreate(i_procName, i_createFunc, NULL, append);
}

fpc_ProcID fopKyM_createWpillar(cXyz const* i_pos, f32 scale, int i_param) {
    fopKyM_prm_class* append = fopKyM_CreateAppend();
    if (append == NULL) {
        return fpcM_ERROR_PROCESS_ID_e;
    }

    append->pos = *i_pos;
    append->scale.set(scale, scale, scale);
    append->parameters = i_param;

    return fopKyM_Create(PROC_WPILLAR, NULL, append);
}

fpc_ProcID fopKyM_createMpillar(cXyz const* i_pos, f32 i_size) {
    static u16 m_name[2] = {
        0x8757,
        0x8758,
    };

    cXyz scale;
    scale.setall(i_size);

    for (int i = 0; i < 2; i++) {
        dComIfGp_particle_set(m_name[i], i_pos, NULL, &scale);
    }

    return fpcM_ERROR_PROCESS_ID_e;
}
