#ifndef F_PC_STDCREATE_H_
#define F_PC_STDCREATE_H_

#include "f_pc/f_pc_create_req.h"

typedef struct layer_class layer_class;

typedef int (*stdCreateFunc)(void*, void*);

fpc_ProcID fpcSCtRq_Request(layer_class* i_layer, s16 i_procName, stdCreateFunc i_createFunc,
                            void* param_4, void* i_append);

#endif
