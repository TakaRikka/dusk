#ifndef _REVOLUTION_OSPLAYTIME_H_
#define _REVOLUTION_OSPLAYTIME_H_

#include <revolution/types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    OSPLAYTIME_PERMANENT        = 0,
    OSPLAYTIME_LIMITED          = 1,
    OSPLAYTIME_LIMITED_LAUNCH   = 4,
    OSPLAYTIME_OTHER            = 9
} __OSPlayTimeType;

typedef void (*OSPlayTimeCallbackFunc)(void);

BOOL OSPlayTimeIsLimited(void);

#ifdef __cplusplus
}
#endif

#endif
