/**
 * MWCriticalSection_gc.c
 * Description:
 */

#include "TRK_MINNOW_DOLPHIN/utils/gc/MWCriticalSection_gc.h"

extern BOOL OSRestoreInterrupts(unsigned int);
extern unsigned int OSDisableInterrupts();

extern void MWInitializeCriticalSection(unsigned int*) {}

extern void MWEnterCriticalSection(unsigned int* section) {
    *section = OSDisableInterrupts();
}

extern void MWExitCriticalSection(unsigned int* section) {
    OSRestoreInterrupts(*section);
}
