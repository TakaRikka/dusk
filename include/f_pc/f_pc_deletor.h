
#ifndef F_PC_DELETOR_H_
#define F_PC_DELETOR_H_

#include "dolphin/types.h"

typedef struct base_process_class base_process_class;

BOOL fpcDt_IsComplete();
int fpcDt_ToDeleteQ(base_process_class* i_proc);
int fpcDt_ToQueue(base_process_class* i_proc);
void fpcDt_Handler();
int fpcDt_Delete(void* i_proc);

#endif
