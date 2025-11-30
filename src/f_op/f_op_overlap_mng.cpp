/**
 * f_op_overlap_mng.cpp
 * Overlap Process Manager
 */

#include "SSystem/SComponent/c_request.h"
#include "f_op/f_op_overlap_mng.h"
#include "f_op/f_op_overlap_req.h"
#include "f_op/f_op_scene_pause.h"
#include "f_pc/f_pc_executor.h"

// making it not an array put it in .bss
static overlap_request_class* l_fopOvlpM_overlap[1] = {NULL};

int fopOvlpM_SceneIsStop() {
    if (l_fopOvlpM_overlap[0] != NULL) {
        return fopScnPause_Enable(
            (scene_class*)fpcEx_SearchByID(l_fopOvlpM_overlap[0]->overlap_task->scene_id));
    }
    
    return 0;
}

int fopOvlpM_SceneIsStart() {
    if (l_fopOvlpM_overlap[0] != NULL) {
        return fopScnPause_Disable(
            (scene_class*)fpcEx_SearchByID(l_fopOvlpM_overlap[0]->overlap_task->scene_id));
    }
    
    return 0;
}

int fopOvlpM_IsOutReq(overlap_task_class* i_overlap_task) {
    return i_overlap_task->request.flag2 == 2;
}

void fopOvlpM_Done(overlap_task_class* i_overlap_task) {
    cReq_Done(&i_overlap_task->request);
}

void fopOvlpM_ToldAboutID(fpc_ProcID i_sceneId) {
    if (l_fopOvlpM_overlap[0] != NULL) {
        l_fopOvlpM_overlap[0]->overlap_task->scene_id = i_sceneId;
    }
}

int fopOvlpM_IsPeek() {
    return l_fopOvlpM_overlap[0] != NULL ? l_fopOvlpM_overlap[0]->field_0x8 : FALSE;
}

int fopOvlpM_IsDone() {
    return l_fopOvlpM_overlap[0] != NULL ? cReq_Is_Done(&l_fopOvlpM_overlap[0]->base) : FALSE;
}

int fopOvlpM_IsDoingReq() {
    if (l_fopOvlpM_overlap[0] != NULL && l_fopOvlpM_overlap[0]->field_0x4 == 1) {
        return 1;
    }

    return 0;
}

int fopOvlpM_ClearOfReq() {
    return l_fopOvlpM_overlap[0] != NULL ? fopOvlpReq_OverlapClr(l_fopOvlpM_overlap[0]) : 0;
}

static overlap_request_class l_fopOvlpM_Request;

overlap_request_class* fopOvlpM_Request(s16 i_procname, u16 i_peektime) {
    if (l_fopOvlpM_overlap[0] == NULL) {
        l_fopOvlpM_overlap[0] = fopOvlpReq_Request(&l_fopOvlpM_Request, i_procname, i_peektime);
        return l_fopOvlpM_overlap[0];
    }

    return NULL;
}

void fopOvlpM_Management() {
    if (l_fopOvlpM_overlap[0] != NULL) {
        switch (fopOvlpReq_Handler(l_fopOvlpM_overlap[0])) {
        case cPhs_UNK3_e:
        case cPhs_COMPLEATE_e:
        case cPhs_ERROR_e:
            l_fopOvlpM_overlap[0] = NULL;
            break;
        }
    }
}

int fopOvlpM_Cancel() {
    if (l_fopOvlpM_overlap[0] == NULL) {
        return 1;
    }

    if (fopOvlpReq_Cancel(l_fopOvlpM_overlap[0]) == true) {
        l_fopOvlpM_overlap[0] = NULL;
        return 1;
    }

    return 0;
}

void fopOvlpM_Init() {}
