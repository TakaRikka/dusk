#ifndef OS_DOLPHIN_TARGET_OPTIONS_H
#define OS_DOLPHIN_TARGET_OPTIONS_H

#include <types.h>

#ifdef __cplusplus
extern "C" {
#endif

u8 GetUseSerialIO(void);
void SetUseSerialIO(u8);

#ifdef __cplusplus
}
#endif

#endif /* OS_DOLPHIN_TARGET_OPTIONS_H */
