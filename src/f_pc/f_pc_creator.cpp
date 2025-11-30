/**
 * f_pc_creator.cpp
 * Framework - Process Creator
 */

#include "f_pc/f_pc_creator.h"
#include "f_pc/f_pc_base.h"
#include "f_pc/f_pc_create_req.h"

BOOL fpcCt_IsCreatingByID(fpc_ProcID i_id) {
    return fpcCtRq_IsCreatingByID(i_id);
}

BOOL fpcCt_IsDoing(base_process_class* i_proc) {
    return fpcCtRq_IsDoing(i_proc->create_req);
}

BOOL fpcCt_Abort(base_process_class* i_proc) {
    return fpcCtRq_Cancel(i_proc->create_req);
}

int fpcCt_Handler() {
    return fpcCtRq_Handler();
}
