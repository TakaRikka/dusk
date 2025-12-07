/**
 * f_pc_create_req.cpp
 * Framework - Process Create Request
 */

#include "f_pc/f_pc_create_req.h"
#include "SSystem/SComponent/c_malloc.h"
#include "f_pc/f_pc_base.h"
#include "f_pc/f_pc_create_iter.h"
#include "f_pc/f_pc_deletor.h"
#include "f_pc/f_pc_executor.h"
#include "f_pc/f_pc_layer.h"
#include "f_pc/f_pc_debug_sv.h"

BOOL fpcCtRq_isCreatingByID(create_tag* i_createTag, fpc_ProcID* i_id) {
    create_request* req = (create_request*)i_createTag->base.mpTagData;
    if (req->id == *i_id) {
        return TRUE;
    } else {
        return FALSE;
    }
}

BOOL fpcCtRq_IsCreatingByID(fpc_ProcID i_id) {
    if (fpcCtIt_Judge((fpcLyIt_JudgeFunc)fpcCtRq_isCreatingByID, &i_id) != NULL) {
        return TRUE;
    } else {
        return FALSE;
    }
}

void fpcCtRq_CreateQTo(create_request* i_request) {
    fpcCtTg_CreateQTo(&i_request->base);
    fpcLy_CreatedMesg(i_request->layer);
    fpcLy_CancelQTo(&i_request->method_tag);
}

void fpcCtRq_ToCreateQ(create_request* i_request) {
    fpcLy_CreatingMesg(i_request->layer);
    fpcLy_ToCancelQ(i_request->layer, &i_request->method_tag);
    fpcCtTg_ToCreateQ(&i_request->base);
}

BOOL fpcCtRq_Delete(create_request* i_request) {
    fpcCtRq_CreateQTo(i_request);
    if (i_request->methods != NULL && fpcMtd_Method(i_request->methods->delete_method, i_request) == 0) {
        return FALSE;
    }

    if (i_request->process != NULL) {
        i_request->process->create_req = NULL;
    }

    cMl::free(i_request);
    return TRUE;
}

BOOL fpcCtRq_Cancel(create_request* i_request) {
    if (i_request != NULL && !i_request->is_cancel) {
        i_request->is_cancel = TRUE;

        if (i_request->process != NULL && !fpcDt_Delete(i_request->process))
            return FALSE;

        if (i_request->methods != NULL && !fpcMtd_Method(i_request->methods->cancel_method, i_request))
            return FALSE;

        return fpcCtRq_Delete(i_request);
    }

    return TRUE;
}

BOOL fpcCtRq_IsDoing(create_request* i_request) {
    if (i_request != NULL)
        return i_request->is_doing;
    else
        return FALSE;
}

BOOL fpcCtRq_Do(create_request* i_request) {
    int phase = cPhs_COMPLEATE_e;

    if (i_request->methods != NULL) {
        if (i_request->methods->phase_handler != NULL) {
            cPhs__Handler pHandler = i_request->methods->phase_handler;

            i_request->is_doing = TRUE;
            phase = pHandler(i_request);
            i_request->is_doing = FALSE;
        }
    }

    switch (phase) {
    case cPhs_COMPLEATE_e: {
        if (fpcEx_ToExecuteQ(i_request->process) == 0)
            return fpcCtRq_Cancel(i_request);
        else
            return fpcCtRq_Delete(i_request);
    }
    case cPhs_UNK3_e:
    case cPhs_ERROR_e:
#if DEBUG
        if (g_fpcDbSv_service[2] != NULL) {
            g_fpcDbSv_service[2](i_request->process);
        }
#endif
        return fpcCtRq_Cancel(i_request);
    }

    return 1;
}

int fpcCtRq_Handler() {
#if DEBUG
    if (g_fpcDbSv_service[3] != NULL) {
        g_fpcDbSv_service[3](&g_fpcCtTg_Queue.mSize);
    }
#endif
    return fpcCtIt_Method((fpcCtIt_MethodFunc)fpcCtRq_Do, NULL);
}

create_request* fpcCtRq_Create(layer_class* i_layer, u32 i_size, create_request_method_class* i_methods) {
    create_request* req = (create_request*)cMl::memalignB(-4, i_size);

    if (req != NULL) {
        create_tag* tag = &req->base;
        fpcCtTg_Init(tag, req);
        fpcMtdTg_Init(&req->method_tag, (process_method_tag_func)fpcCtRq_Cancel, req);
        req->layer = i_layer;
        req->methods = i_methods;
        req->id = fpcBs_MakeOfId();
        req->process = NULL;
        req->is_cancel = FALSE;
        fpcCtRq_ToCreateQ(req);
    }

    return req;
}
