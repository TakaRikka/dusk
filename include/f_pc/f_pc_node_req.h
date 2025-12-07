#ifndef F_PC_NODE_REQ_H_
#define F_PC_NODE_REQ_H_

#include "SSystem/SComponent/c_phase.h"
#include "f_pc/f_pc_method.h"
#include "f_pc/f_pc_method_tag.h"
#include "f_pc/f_pc_base.h"

typedef struct layer_class layer_class;
typedef struct process_node_class process_node_class;

typedef struct node_create_request_method_class {
    /* 0x0 */ process_method_func execute_method;
    /* 0x4 */ process_method_func cancel_method;
    /* 0x8 */ process_method_func delete_method;
    /* 0xC */ process_method_func post_method;
} node_create_request_method_class;

// needed to match struct copy
typedef struct unk_process_node_class {
    /* 0x0 */ process_node_class* node;
    /* 0x4 */ fpc_ProcID id;
} unk_process_node_class;

typedef struct node_create_request {
    /* 0x00 */ create_tag_class create_tag;
    /* 0x14 */ process_method_tag_class method_tag;
    /* 0x30 */ request_of_phase_process_class phase_request;
    /* 0x38 */ cPhs__Handler* phase_handler;
    /* 0x3C */ node_create_request_method_class* create_req_methods;
    /* 0x40 */ s32 parameters;
    /* 0x44 */ fpc_ProcID request_id;
    /* 0x48 */ unk_process_node_class node_proc;
    /* 0x50 */ layer_class* layer;
    /* 0x54 */ fpc_ProcID creating_id;
    /* 0x58 */ s16 name;
    /* 0x5C */ void* data;
    /* 0x60 */ s16 unk_0x60;
#if DEBUG
    /* 0x64 */ int unk_0x64;
    /* 0x68 */ int unk_0x68;
#endif
} node_create_request;  // Size: 0x64

typedef struct request_node_class {
    /* 0x0 */ node_class node;
    /* 0x4 */ node_create_request* node_create_req;
} request_node_class;

void fpcNdRq_RequestQTo(node_create_request* i_request);
void fpcNdRq_ToRequestQ(node_create_request* i_request);
s32 fpcNdRq_phase_IsCreated(node_create_request* i_request);
s32 fpcNdRq_phase_Create(node_create_request* i_request);
s32 fpcNdRq_phase_IsDeleteTiming(node_create_request* i_request);
s32 fpcNdRq_phase_IsDeleted(node_create_request* i_request);
s32 fpcNdRq_phase_Delete(node_create_request* i_request);
s32 fpcNdRq_DoPhase(node_create_request* i_request);
s32 fpcNdRq_Execute(node_create_request* i_request);
s32 fpcNdRq_Delete(node_create_request* i_request);
s32 fpcNdRq_Cancel(node_create_request* i_request);
s32 fpcNdRq_Handler();
s32 fpcNdRq_IsPossibleTarget(process_node_class* i_procNode);
s32 fpcNdRq_IsIng(process_node_class* i_procNode);
node_create_request* fpcNdRq_Create(u32 i_requestSize);
node_create_request* fpcNdRq_ChangeNode(u32 i_requestSize, process_node_class* i_procNode,
                                        s16 i_procName, void* i_data);
node_create_request* fpcNdRq_DeleteNode(u32 i_requestSize, process_node_class* i_procNode);
node_create_request* fpcNdRq_CreateNode(u32 i_requestSize, s16 i_procName, void* i_data);
node_create_request*
fpcNdRq_Request(u32 i_requestSize, int i_reqType,
                process_node_class* i_procNode, s16 i_procName, void* i_data,
                node_create_request_method_class* i_create_req_methods);
s32 fpcNdRq_ReChangeNode(fpc_ProcID i_requestID, s16 i_procName, void* i_data);
s32 fpcNdRq_ReRequest(fpc_ProcID i_requestID, s16 i_procName, void* i_data);

#endif
