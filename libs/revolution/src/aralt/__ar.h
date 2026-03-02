#ifndef _REVOLUTION_AR_INTERNAL_H_
#define _REVOLUTION_AR_INTERNAL_H_

#include <revolution/aralt.h>

#ifdef __cplusplus
extern "C" {
#endif

void __ARQPopTaskQueueHi(void);
void __ARQServiceQueueLo(void);
void __ARQInterruptServiceRoutine(u32);

#ifdef __cplusplus
}
#endif

#endif // _REVOLUTION_AR_INTERNAL_H_
