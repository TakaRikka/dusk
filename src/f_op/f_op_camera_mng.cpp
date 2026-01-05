/**
 * f_op_camera_mng.cpp
 * Camera Process Manager
 */

#include "f_op/f_op_camera_mng.h"

static fpc_ProcID l_fopCamM_id[4];

u32 fopCamM_GetParam(camera_class* i_this) {
    return fpcM_GetParam(i_this);
}

void dummy(fpc_ProcID i_procName) {
    fpcM_SearchByID(i_procName);
}

fpc_ProcID fopCamM_Create(int i_cameraIdx, s16 i_procName, void* i_append) {
    l_fopCamM_id[i_cameraIdx] = fpcM_Create(i_procName, NULL, i_append);
    return l_fopCamM_id[i_cameraIdx];
}

void fopCamM_Management() {}

void fopCamM_Init() {}
