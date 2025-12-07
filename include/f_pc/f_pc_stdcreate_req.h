#ifndef F_PC_STDCREATE_H_
#define F_PC_STDCREATE_H_

#include "f_pc/f_pc_create_req.h"

typedef struct layer_class layer_class;

typedef int (*stdCreateFunc)(void*, void*);

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

s32 fpcSCtRq_phase_CreateProcess(standard_create_request_class* i_request);
s32 fpcSCtRq_phase_SubCreateProcess(standard_create_request_class* i_request);
s32 fpcSCtRq_phase_IsComplete(standard_create_request_class* i_request);
s32 fpcSCtRq_phase_PostMethod(standard_create_request_class* i_request);
s32 fpcSCtRq_phase_Done(standard_create_request_class* i_request);
s32 fpcSCtRq_Handler(standard_create_request_class* i_request);
s32 fpcSCtRq_Delete(standard_create_request_class* i_request);
s32 fpcSCtRq_Cancel(standard_create_request_class* i_request);
fpc_ProcID fpcSCtRq_Request(layer_class* i_layer, s16 i_procName, stdCreateFunc i_createFunc,
                            void* param_4, void* i_append);

#endif
