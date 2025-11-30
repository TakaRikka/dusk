/**
 * f_pc_searcher.cpp
 * Framework - Process Searcher
 */

#include "f_pc/f_pc_searcher.h"

void* fpcSch_JudgeForPName(void* i_proc, void* i_data) {
    if (((base_process_class*)i_proc)->name == *(s16*)i_data)
        return i_proc;

    return NULL;
}

void* fpcSch_JudgeByID(void* i_proc, void* i_data) {
    if (((base_process_class*)i_proc)->id == *(fpc_ProcID*)i_data)
        return i_proc;

    return NULL;
}
