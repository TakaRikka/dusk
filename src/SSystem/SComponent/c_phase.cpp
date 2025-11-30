/**
 * c_phase.cpp
 *
 */

#include "SSystem/SComponent/c_phase.h"

void cPhs_Reset(request_of_phase_process_class* phase) {
    phase->id = cPhs_INIT_e;
}

void cPhs_Set(request_of_phase_process_class* phase, cPhs__Handler* handlerTbl) {
    phase->mpHandlerTable = handlerTbl;
    phase->id = cPhs_INIT_e;
}

void cPhs_UnCompleate(request_of_phase_process_class* phase) {
    phase->mpHandlerTable = NULL;
    cPhs_Reset(phase);
}

int cPhs_Compleate(request_of_phase_process_class* phase) {
    phase->mpHandlerTable = NULL;
    return cPhs_COMPLEATE_e;
}

int cPhs_Next(request_of_phase_process_class* phase) {
    if (phase->mpHandlerTable != NULL) {
        cPhs__Handler* handler = phase->mpHandlerTable;
        phase->id++;
        handler += phase->id;

        // Double null check here actually matters for emitted assembly.
        // Wee old compilers.
        if (*handler == NULL || *handler == NULL) {
            return cPhs_Compleate(phase);
        } else {
            return cPhs_LOADING_e;
        }
    }

    return cPhs_COMPLEATE_e;
}

int cPhs_Do(request_of_phase_process_class* phase, void* data) {
    cPhs__Handler* handler = phase->mpHandlerTable;

    if (handler) {
        handler += phase->id;
        int newStep = (*handler)(data);

        switch (newStep) {
        case cPhs_LOADING_e:
            return cPhs_Next(phase);
        case cPhs_NEXT_e:
            if (cPhs_Next(phase) == cPhs_LOADING_e) {
                return cPhs_NEXT_e;
            } else {
                return cPhs_COMPLEATE_e;
            }
        case cPhs_COMPLEATE_e:
            return cPhs_Compleate(phase);
        case cPhs_UNK3_e:
            cPhs_UnCompleate(phase);
            return cPhs_UNK3_e;
        case cPhs_ERROR_e:
            cPhs_UnCompleate(phase);
            return cPhs_ERROR_e;
        case cPhs_INIT_e:
        default:
            return newStep;
        }
    }

    return cPhs_Compleate(phase);
}

int cPhs_Handler(request_of_phase_process_class* phase, cPhs__Handler* handlerTbl, void* data) {
    phase->mpHandlerTable = handlerTbl;
    return cPhs_Do(phase, data);
}
