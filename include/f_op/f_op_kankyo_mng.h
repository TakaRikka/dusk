#ifndef F_F_OP_KANKYO_MNG_H_
#define F_F_OP_KANKYO_MNG_H_

#include "f_op/f_op_kankyo.h"
#include "f_pc/f_pc_manager.h"

struct fopKyM_prm_class {
    /* 0x00 */ cXyz pos;
    /* 0x0C */ cXyz scale;
    /* 0x18 */ int parameters;
};  // Size: 0x1C

typedef int (*fopKyM_CreateFunc)(void*);

fopKyM_prm_class* fopKyM_CreateAppend();
void fopKyM_Delete(void* i_process);
fpc_ProcID fopKyM_create(s16 i_procName, int i_param, cXyz* i_pos, cXyz* i_scale,
                         fopKyM_CreateFunc i_createFunc);
fpc_ProcID fopKyM_Create(s16 i_procName, fopKyM_CreateFunc i_createFunc, void* i_append);
base_process_class* fopKyM_fastCreate(s16 i_procName, int i_param, cXyz* i_pos, cXyz* i_scale,
                                      fopKyM_CreateFunc i_createFunc);
fpc_ProcID fopKyM_createWpillar(cXyz const* i_pos, f32 scale, int i_param);
fpc_ProcID fopKyM_createMpillar(cXyz const* i_pos, f32 i_size);
BOOL fopKyM_IsKy(void* i_process);

inline fopKyM_prm_class* fopKyM_GetAppend(void* i_process) {
    return (fopKyM_prm_class*)fpcM_GetAppend(i_process);
}

#endif
