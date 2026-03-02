#ifndef _DOLPHIN_OSTIMER_H_
#define _DOLPHIN_OSTIMER_H_

#include <dolphin/types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*OSTimerCallback)(void);

OSTimerCallback OSSetTimerCallback(OSTimerCallback callback);
void OSInitTimer(u32 time, u32 mode);
void OSStartTimer(void);
void OSStopTimer(void);

#ifdef __cplusplus
}
#endif

#endif // _DOLPHIN_OSTIMER_H_
