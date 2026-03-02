/**
 * f_pc_stdcreate_req.cpp
 * Framework - Process Standard Create Request
 */

#include "f_pc/f_pc_stdcreate_req.h"
#include "f_pc/f_pc_load.h"
#include "f_pc/f_pc_node.h"
#include "f_pc/f_pc_manager.h"
#include "f_pc/f_pc_debug_sv.h"
#include "SSystem/SComponent/c_phase.h"
#ifdef __REVOLUTION_SDK__
#include <revolution.h>
#else
#include <dolphin.h>
#endif
#include <cstdio>
#include "os_report.h"

typedef struct standard_create_request_class {
    /* 0x00 */ create_request base;
    /* 0x48 */ request_of_phase_process_class phase_request;
    /* 0x50 */ s16 process_name;
    /* 0x54 */ void* process_append;
    /* 0x58 */ stdCreateFunc create_post_method;
    /* 0x5C */ void* unk_0x5C;
#if DEBUG
    /* 0x60 */ int unk_0x60;
#endif
} standard_create_request_class;

int fpcSCtRq_phase_Load(standard_create_request_class* i_request) {
    int ret = fpcLd_Load(i_request->process_name);
    printf("[DIAG] fpcSCtRq_phase_Load: procName=%d ret=%d\n", i_request->process_name, ret); fflush(stdout);

    switch (ret) {
    case cPhs_INIT_e:
        return cPhs_INIT_e;
    case cPhs_COMPLEATE_e:
        return cPhs_NEXT_e;
    case cPhs_ERROR_e:
        OS_REPORT("fpcSCtRq_phase_Load %d\n", i_request->process_name);
    default:
        return cPhs_ERROR_e;
    }
}

int fpcSCtRq_phase_CreateProcess(standard_create_request_class* i_request) {
    printf("[DIAG] fpcSCtRq_phase_CreateProcess: procName=%d\n", i_request->process_name); fflush(stdout);
    fpcLy_SetCurrentLayer(i_request->base.layer);
    i_request->base.process =
        fpcBs_Create(i_request->process_name, i_request->base.id, i_request->process_append);

    if (i_request->base.process == NULL) {
        printf("[DIAG] fpcSCtRq_phase_CreateProcess: fpcBs_Create FAILED for procName=%d\n", i_request->process_name); fflush(stdout);
        OS_REPORT("fpcSCtRq_phase_CreateProcess %d\n", i_request->process_name);
        fpcLd_Free(i_request->process_name);
        return cPhs_ERROR_e;
    } else {
        printf("[DIAG] fpcSCtRq_phase_CreateProcess: fpcBs_Create OK proc=%p\n", i_request->base.process); fflush(stdout);
        i_request->base.process->create_req = &i_request->base;
        return cPhs_NEXT_e;
    }
}

int fpcSCtRq_phase_SubCreateProcess(standard_create_request_class* i_request) {
    fpcLy_SetCurrentLayer(i_request->base.layer);
    int ret = fpcBs_SubCreate(i_request->base.process);
    static int sSubCreateLogCount = 0;
    if (sSubCreateLogCount < 20) {
        printf("[DIAG] fpcSCtRq_phase_SubCreateProcess: pid=%d procName=%04x ret=%d\n", i_request->base.id, i_request->process_name, ret); fflush(stdout);
        sSubCreateLogCount++;
    }

#if DEBUG
    if (ret == 0 && i_request->unk_0x60-- <= 0) {
        i_request->unk_0x60 = 0;
        if (g_fpcDbSv_service[0] != NULL) {
            g_fpcDbSv_service[0](i_request->base.process);
        }
    }
#endif

    return ret;
}

int fpcSCtRq_phase_IsComplete(standard_create_request_class* i_request) {
    process_node_class* procNode =
        (process_node_class*)((standard_create_request_class*)i_request)->base.process;
    if (fpcBs_Is_JustOfType(g_fpcNd_type, procNode->base.subtype) == TRUE) {
        if (fpcLy_IsCreatingMesg(&procNode->layer) == TRUE) {
            return cPhs_INIT_e;
        }
    }
    return cPhs_NEXT_e;
}

int fpcSCtRq_phase_PostMethod(standard_create_request_class* i_request) {
    stdCreateFunc create_func = i_request->create_post_method;

    if (create_func != NULL) {
        int ret = create_func(i_request->base.process, i_request->unk_0x5C);
        if (ret == 0) {
            return cPhs_INIT_e;
        }
    }

    return cPhs_NEXT_e;
}

int fpcSCtRq_phase_Done(standard_create_request_class* i_request) {
    return cPhs_NEXT_e;
}

int fpcSCtRq_Handler(standard_create_request_class* i_request) {
    int phase_state = cPhs_Do(&i_request->phase_request, i_request);

    switch (phase_state) {
    case cPhs_NEXT_e:
        return fpcSCtRq_Handler(i_request);
    case cPhs_COMPLEATE_e:
        return cPhs_COMPLEATE_e;
    case cPhs_INIT_e:
    case cPhs_UNK3_e:
    case cPhs_ERROR_e:
    default:
        return phase_state;
    }
}

int fpcSCtRq_Delete(standard_create_request_class* i_request) {
    return 1;
}

int fpcSCtRq_Cancel(standard_create_request_class* i_request) {
    return 1;
}

fpc_ProcID fpcSCtRq_Request(layer_class* i_layer, s16 i_procName, stdCreateFunc i_createFunc,
                     void* param_4, void* i_append) {
    static create_request_method_class submethod = {
        (cPhs__Handler)fpcSCtRq_Handler,
        (process_method_func)fpcSCtRq_Cancel,
        (process_method_func)fpcSCtRq_Delete,
    };

    static cPhs__Handler method[7] = {
        (cPhs__Handler)fpcSCtRq_phase_Load,
        (cPhs__Handler)fpcSCtRq_phase_CreateProcess,
        (cPhs__Handler)fpcSCtRq_phase_SubCreateProcess,
        (cPhs__Handler)fpcSCtRq_phase_IsComplete,
        (cPhs__Handler)fpcSCtRq_phase_PostMethod,
        (cPhs__Handler)fpcSCtRq_phase_Done,
        NULL,
    };

    if (i_procName >= 0x7FFF) {
        return fpcM_ERROR_PROCESS_ID_e;
    }

    standard_create_request_class* request =
        (standard_create_request_class*)fpcCtRq_Create(i_layer, sizeof(standard_create_request_class), &submethod);
    if (request == NULL) {
        return fpcM_ERROR_PROCESS_ID_e;
    }

    cPhs_Set(&request->phase_request, method);
    request->process_name = i_procName;
    request->create_post_method = i_createFunc;
    request->unk_0x5C = param_4;
    request->process_append = i_append;
#if DEBUG
    request->unk_0x60 = 60;
#endif

    return request->base.id;
}
