#ifndef F_PC_MANAGER_H_
#define F_PC_MANAGER_H_

#include "f_op/f_op_scene.h"
#include "f_pc/f_pc_create_iter.h"
#include "f_pc/f_pc_executor.h"
#include "f_pc/f_pc_leaf.h"
#include "f_pc/f_pc_node.h"
#include "f_pc/f_pc_stdcreate_req.h"
#include "f_pc/f_pc_searcher.h"

enum {
    fpcM_ERROR_PROCESS_ID_e = 0xFFFFFFFF
};

typedef int (*FastCreateReqFunc)(void*);
typedef void (*fpcM_ManagementFunc)(void);
typedef int (*fpcM_DrawIteraterFunc)(void*, void*);

inline fpc_ProcID fpcM_GetID(const void* i_process) {
    return i_process != NULL ? ((base_process_class*)i_process)->id : fpcM_ERROR_PROCESS_ID_e;
}

inline s16 fpcM_GetName(const void* i_process) {
    return ((base_process_class*)i_process)->name;
}

inline u32 fpcM_GetParam(const void* i_process) {
    return ((base_process_class*)i_process)->parameters;
}

inline void fpcM_SetParam(void* i_process, u32 param) {
    ((base_process_class*)i_process)->parameters = param;
}

inline s16 fpcM_GetProfName(const void* i_process) {
    return ((base_process_class*)i_process)->profname;
}

inline fpc_ProcID fpcM_Create(s16 i_procName, FastCreateReqFunc i_createFunc, void* i_append) {
    return fpcSCtRq_Request(fpcLy_CurrentLayer(), i_procName, (stdCreateFunc)i_createFunc, NULL,
                            i_append);
}

inline s32 fpcM_DrawPriority(const void* i_process) {
    return (s16)fpcLf_GetPriority((const leafdraw_class*)i_process);
}

inline s32 fpcM_ChangeLayerID(void* i_process, int i_layerID) {
    return fpcPi_Change(&((base_process_class*)i_process)->priority, i_layerID, 0xFFFD, 0xFFFD);
}

inline s32 fpcM_MakeOfType(int* i_type) {
    return fpcBs_MakeOfType(i_type);
}

inline BOOL fpcM_IsJustType(int i_typeA, int i_typeB) {
    return fpcBs_Is_JustOfType(i_typeA, i_typeB);
}

inline bool fpcM_IsFirstCreating(void* i_process) {
    return ((base_process_class*)i_process)->state.init_state == 0;
}

inline process_profile_definition* fpcM_GetProfile(void* i_process) {
    return ((base_process_class*)i_process)->profile;
}

inline void* fpcM_GetAppend(const void* i_process) {
    return ((base_process_class*)i_process)->append;
}

inline BOOL fpcM_IsExecuting(fpc_ProcID id) {
    return fpcEx_IsExist(id);
}

inline void* fpcM_LyJudge(process_node_class* i_node, fpcLyIt_JudgeFunc i_func, void* i_data) {
    return fpcLyIt_Judge(&i_node->layer, i_func, i_data);
}

inline base_process_class* fpcM_Search(fpcLyIt_JudgeFunc i_func, void* i_data) {
    return fpcEx_Search(i_func, i_data);
}

inline base_process_class* fpcM_SearchByName(s16 name) {
    return (base_process_class*)fpcLyIt_AllJudge(fpcSch_JudgeForPName, &name);
}

inline base_process_class* fpcM_SearchByID(fpc_ProcID i_id) {
    return fpcEx_SearchByID(i_id);
}

inline process_node_class* fpcM_Layer(void* i_process) {
    return ((base_process_class*)i_process)->layer_tag.layer->process_node;
}

void fpcM_Draw(void* i_process);
s32 fpcM_DrawIterater(fpcM_DrawIteraterFunc i_drawIterFunc);
s32 fpcM_Execute(void* i_process);
s32 fpcM_Delete(void* i_process);
BOOL fpcM_IsCreating(fpc_ProcID i_id);
void fpcM_Management(fpcM_ManagementFunc i_preExecuteFn, fpcM_ManagementFunc i_postExecuteFn);
void fpcM_Init();
base_process_class* fpcM_FastCreate(s16 i_procname, FastCreateReqFunc i_createReqFunc,
                                    void* i_createData, void* i_append);
s32 fpcM_IsPause(void* i_process, u8 i_flag);
void fpcM_PauseEnable(void* i_process, u8 i_flag);
void fpcM_PauseDisable(void* i_process, u8 i_flag);
void* fpcM_JudgeInLayer(fpc_ProcID i_layerID, fpcCtIt_JudgeFunc i_judgeFunc, void* i_data);

#endif
