#ifndef C_PHASE_H
#define C_PHASE_H

#include <dolphin/types.h>

typedef int (*cPhs__Handler)(void*);

typedef int cPhs_Step;

enum {
    /* 0x0 */ cPhs_INIT_e,
    /* 0x1 */ cPhs_LOADING_e,
    /* 0x2 */ cPhs_NEXT_e,
    /* 0x3 */ cPhs_UNK3_e,  // appears to be an alternate error code, unsure how it differs
    /* 0x4 */ cPhs_COMPLEATE_e,
    /* 0x5 */ cPhs_ERROR_e,
};

typedef struct request_of_phase_process_class {
    cPhs__Handler* mpHandlerTable;
    int id;
} request_of_phase_process_class;

typedef int (*request_of_phase_process_fn)(void*);

void cPhs_Reset(request_of_phase_process_class* pPhase);
void cPhs_Set(request_of_phase_process_class* pPhase, cPhs__Handler* pHandlerTable);
void cPhs_UnCompleate(request_of_phase_process_class* pPhase);
int cPhs_Compleate(request_of_phase_process_class* pPhase);
int cPhs_Next(request_of_phase_process_class* pPhase);
int cPhs_Do(request_of_phase_process_class* pPhase, void* pUserData);
int cPhs_Handler(request_of_phase_process_class* pPhase, cPhs__Handler* pHandlerTable,
                 void* pUserData);

#endif /* C_PHASE_H */
