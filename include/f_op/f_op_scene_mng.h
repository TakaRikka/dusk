#ifndef F_F_OP_SCENE_MNG_H_
#define F_F_OP_SCENE_MNG_H_

#include "f_op/f_op_scene_pause.h"
#include "f_pc/f_pc_manager.h"
#include "f_op/f_op_scene.h"

typedef struct base_process_class base_process_class;

scene_class* fopScnM_SearchByID(fpc_ProcID id);
int fopScnM_ChangeReq(scene_class* i_scene, s16 i_procName, s16 param_3, u16 param_4);
fpc_ProcID fopScnM_DeleteReq(scene_class* i_scene);
int fopScnM_CreateReq(s16 i_procName, s16 param_2, u16 param_3, u32 i_data);
u32 fopScnM_ReRequest(s16 i_procName, u32 i_data);
void fopScnM_Management();
void fopScnM_Init();

inline fpc_ProcID fpcM_LayerID(const void* i_process) {
    if (fpcBs_Is_JustOfType(g_fpcNd_type, ((base_process_class*)i_process)->subtype) != FALSE) {
        return ((scene_class*)i_process)->base.layer.layer_id;
    }
    return fpcM_ERROR_PROCESS_ID_e;
}

inline fpc_ProcID fopScnM_GetID(void* i_scene) {
    return fpcM_GetID(i_scene);
}

inline fpc_ProcID fopScnM_LayerID(void* i_scene) {
    return fpcM_LayerID(i_scene);
}

inline u32 fopScnM_GetParam(void* i_scene) {
    return fpcM_GetParam(i_scene);
}

inline void fopScnM_SetParam(void* i_scene, u32 i_param) {
    fpcM_SetParam(i_scene, i_param);
}

inline void* fopScnM_GetAppend(void* i_scene) {
    return fpcM_GetAppend(i_scene);
}

inline int fopScnM_PauseEnable(scene_class* i_scene) {
    return fopScnPause_Enable(i_scene);
}

inline int fopScnM_PauseDisable(scene_class* i_scene) {
    return fopScnPause_Disable(i_scene);
}

#endif
