#ifndef _REVOLUTION_OSRESETSW_H_
#define _REVOLUTION_OSRESETSW_H_

#include <revolution/types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*OSResetCallback)(void);
typedef void (*OSPowerCallback)(void);

OSResetCallback OSSetResetCallback(OSResetCallback callback);
BOOL OSGetResetSwitchState(void);
BOOL OSGetResetButtonState(void);

#ifdef __cplusplus
}
#endif

#endif
