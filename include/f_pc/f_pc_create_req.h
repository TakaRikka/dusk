
#ifndef F_PC_CREATE_REQ_H_
#define F_PC_CREATE_REQ_H_

#include "f_pc/f_pc_create_tag.h"
#include "f_pc/f_pc_method.h"
#include "f_pc/f_pc_method_tag.h"
#include "f_pc/f_pc_base.h"

typedef struct base_process_class base_process_class;
typedef struct layer_class layer_class;

// declare our own typedef instead of cPhs__Handler to avoid pulling in c_phase.h
// (which causes RTTI ordering issues in relation to cM3dGCyl)
typedef int (*fpcCtRq_phase_handler)(void*);

typedef struct create_request_method_class {
    /* 0x0 */ fpcCtRq_phase_handler phase_handler;
    /* 0x4 */ process_method_func cancel_method;
    /* 0x8 */ process_method_func delete_method;
} create_request_method_class;

typedef struct create_request {
    /* 0x00 */ create_tag base;
    /* 0x14 */ s8 is_doing;
    /* 0x15 */ s8 is_cancel;
    /* 0x18 */ process_method_tag_class method_tag;
    /* 0x34 */ create_request_method_class* methods;
    /* 0x38 */ void* unk_0x38;
    /* 0x3C */ fpc_ProcID id;
    /* 0x40 */ struct base_process_class* process;
    /* 0x44 */ layer_class* layer;
} create_request;  // Size: 0x48

BOOL fpcCtRq_isCreatingByID(create_tag* i_createTag, fpc_ProcID* i_id);
BOOL fpcCtRq_IsCreatingByID(fpc_ProcID i_id);
void fpcCtRq_CreateQTo(create_request* i_request);
void fpcCtRq_ToCreateQ(create_request* i_request);
BOOL fpcCtRq_Delete(create_request* i_request);
BOOL fpcCtRq_Cancel(create_request* i_request);
BOOL fpcCtRq_IsDoing(create_request* i_request);
int fpcCtRq_Handler();
create_request* fpcCtRq_Create(layer_class* i_layer, u32 i_size,
                               create_request_method_class* i_methods);

#endif
