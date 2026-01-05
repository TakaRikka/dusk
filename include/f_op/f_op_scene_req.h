#ifndef F_F_OP_SCENE_REQ_H_
#define F_F_OP_SCENE_REQ_H_

#include "f_pc/f_pc_node_req.h"

class scene_class;
class scene_request_class {
public:
    /* 0x00 */ node_create_request create_request;
    /* 0x64 */ scene_request_class* fade_request;
    /* 0x68 */ request_of_phase_process_class phase_request;
    /* 0x00 */ u8 field_0x70[4];
};

fpc_ProcID fopScnRq_Request(int i_reqType, scene_class* i_scene, s16 i_procName, void* i_data, s16 param_5,
                     u16 param_6);
s32 fopScnRq_ReRequest(fpc_ProcID i_requestId, s16 i_procName, void* i_data);
int fopScnRq_Handler();

#endif
