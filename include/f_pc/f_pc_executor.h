
#ifndef F_PC_EXECUTOR_H_
#define F_PC_EXECUTOR_H_

#include "f_pc/f_pc_layer_iter.h"
#include "f_pc/f_pc_line_iter.h"
#include "f_pc/f_pc_base.h"

typedef struct base_process_class base_process_class;

base_process_class* fpcEx_Search(fpcLyIt_JudgeFunc i_judgeFunc, void* i_data);
base_process_class* fpcEx_SearchByID(fpc_ProcID i_id);
BOOL fpcEx_IsExist(fpc_ProcID i_id);
int fpcEx_ToLineQ(base_process_class* i_proc);
int fpcEx_ExecuteQTo(base_process_class* i_proc);
int fpcEx_Execute(base_process_class* i_proc);
int fpcEx_ToExecuteQ(base_process_class* i_proc);
void fpcEx_Handler(fpcLnIt_QueueFunc i_queueFunc);

#endif
