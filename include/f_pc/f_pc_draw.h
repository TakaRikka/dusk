#ifndef F_PC_DRAW_H_
#define F_PC_DRAW_H_

#include <dolphin/types.h>

typedef struct base_process_class base_process_class;

typedef int (*fpcDw_HandlerFunc)(void*, void*);
typedef int (*fpcDw_HandlerFuncFunc)(fpcDw_HandlerFunc);

int fpcDw_Execute(base_process_class* i_proc);
int fpcDw_Handler(fpcDw_HandlerFuncFunc i_iterHandler, fpcDw_HandlerFunc i_func);

#endif
