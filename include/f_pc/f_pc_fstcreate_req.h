#ifndef F_PC_FSTCREATE_REQ_H_
#define F_PC_FSTCREATE_REQ_H_

#include "f_pc/f_pc_create_req.h"

typedef struct base_process_class base_process_class;
typedef struct layer_class layer_class;

typedef int (*fstCreateFunc)(void*, void*);

typedef struct fast_create_request {
    /* 0x00 */ create_request base;
    /* 0x48 */ fstCreateFunc create_func;
    /* 0x4C */ void* data;
} fast_create_request;  // Size: 0x50

int fpcFCtRq_Do(fast_create_request* i_createReq);
int fpcFCtRq_Delete(fast_create_request* i_createReq);
base_process_class* fpcFCtRq_Request(layer_class* i_layer, s16 i_procname,
                                     fstCreateFunc i_createFunc, void* i_createData, void* i_append);

#endif
