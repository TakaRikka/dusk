#ifndef _DOLPHIN_AR_INTERNAL_H_
#define _DOLPHIN_AR_INTERNAL_H_

#include <dolphin/ar.h>

#ifdef __cplusplus
extern "C" {
#endif

void __ARClearInterrupt(void);
u16 __ARGetInterruptStatus(void);

void __ARQPopTaskQueueHi(void);
void __ARQServiceQueueLo(void);
void __ARQCallbackHack(u32 pointerToARQRequest);
void __ARQInterruptServiceRoutine(void);
void __ARQInitTempQueue(void);
void __ARQPushTempQueue(ARQRequest* task);

#ifdef __cplusplus
}
#endif

#endif // _DOLPHIN_AR_INTERNAL_H_
