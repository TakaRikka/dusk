/**
 * f_pc_fstcreate_req.cpp
 * Framework - Process Fast Create Request
 */

#include "f_pc/f_pc_fstcreate_req.h"
#include "f_pc/f_pc_base.h"
#include "f_pc/f_pc_layer.h"
#include "f_pc/f_pc_load.h"

s32 fpcFCtRq_Do(fast_create_request* i_createReq) {
    if (i_createReq->create_func != NULL &&
        i_createReq->create_func(i_createReq->base.process, i_createReq->data) == 0)
    {
        return cPhs_UNK3_e;
    } else {
        return cPhs_COMPLEATE_e;
    }
}

s32 fpcFCtRq_Delete(fast_create_request* i_createReq) {
    return 1;
}

base_process_class* fpcFCtRq_Request(layer_class* i_layer, s16 i_procname,
                                     fstCreateFunc i_createFunc, void* i_createData, void* i_append) {
    static create_request_method_class submethod = {(cPhs__Handler)fpcFCtRq_Do, NULL,
                                                    (process_method_func)fpcFCtRq_Delete};

    if (!fpcLd_Use(i_procname)) {
        return NULL;
    }

    fast_create_request* request =
        (fast_create_request*)fpcCtRq_Create(i_layer, sizeof(fast_create_request), &submethod);
    if (request != NULL) {
        base_process_class* proc;
        fpcLy_SetCurrentLayer(i_layer);

        proc = fpcBs_Create(i_procname, fpcBs_MakeOfId(), i_append);
        if (proc != NULL) {
            proc->create_req = &request->base;
            request->base.process = proc;
            request->base.id = proc->id;

            if (fpcBs_SubCreate(proc) == 2) {
                request->create_func = i_createFunc;
                request->data = i_createData;
                return proc;
            }
        }

        fpcCtRq_Cancel(&request->base);
    }

    return NULL;
}
