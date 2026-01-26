#ifndef UTILS_GC_MWCRITICALSECTION_GC_H
#define UTILS_GC_MWCRITICALSECTION_GC_H

#include <dolphin/types.h>

#ifdef __cplusplus
extern "C" {
#endif

void MWExitCriticalSection(unsigned int* section);
void MWEnterCriticalSection(unsigned int* section);
void MWInitializeCriticalSection(unsigned int*);

#ifdef __cplusplus
}
#endif

#endif /* UTILS_GC_MWCRITICALSECTION_GC_H */
