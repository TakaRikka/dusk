/**
 * f_op_overlap_mng.cpp
 * Overlap Process Manager
 */

#include "SSystem/SComponent/c_request.h"
#include "m_Do/m_Do_controller_pad.h"
#include "f_op/f_op_overlap_mng.h"
#include "f_op/f_op_overlap_req.h"
#include "f_op/f_op_scene_mng.h"
#include "f_op/f_op_scene_pause.h"

// making it not an array put it in .bss
static overlap_request_class* l_fopOvlpM_overlap[1] = {NULL};

int fopOvlpM_SceneIsStop() {
    if (l_fopOvlpM_overlap[0] != NULL) {
        fpc_ProcID scene_id = l_fopOvlpM_overlap[0]->overlap_task->scene_id;
        scene_class* cls = (scene_class*)fpcM_SearchByID(scene_id);
        return fopScnM_PauseEnable(cls);
    }

    return 0;
}

int fopOvlpM_SceneIsStart() {
    if (l_fopOvlpM_overlap[0] != NULL) {
        fpc_ProcID scene_id = l_fopOvlpM_overlap[0]->overlap_task->scene_id;
        scene_class* cls = (scene_class*)fpcM_SearchByID(scene_id);
        return fopScnM_PauseDisable(cls);
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
    if (l_fopOvlpM_overlap[0] != NULL) {
        return l_fopOvlpM_overlap[0]->field_0x8;
    }
    return FALSE;
}

int fopOvlpM_IsDone() {
    if (l_fopOvlpM_overlap[0] != NULL) {
        return cReq_Is_Done(&l_fopOvlpM_overlap[0]->base);
    }
    return FALSE;
}

int fopOvlpM_IsDoingReq() {
    if (l_fopOvlpM_overlap[0] != NULL && l_fopOvlpM_overlap[0]->field_0x4 == 1) {
        return 1;
    }
    return FALSE;
}

int fopOvlpM_ClearOfReq() {
    if (l_fopOvlpM_overlap[0] != NULL) {
        return fopOvlpReq_OverlapClr(l_fopOvlpM_overlap[0]);
    }
    return FALSE;
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
#if DEBUG
        BOOL state = mDoCPd_c::isConnect(PAD_3);
        if (state) {
            JUT_WARN(336, "%s", "fopOvlpM_Cancel SUCCESSED");
        }
#endif
        return 1;
    }

    return 0;
}

void fopOvlpM_Init() {}
