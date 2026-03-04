#ifndef _REVOLUTION_AI_H_
#define _REVOLUTION_AI_H_

#include <revolution/types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*AISCallback)(u32 count);
typedef void (*AIDCallback)();

#define AI_STREAM_START 1
#define AI_STREAM_STOP 0

#define AI_SAMPLERATE_32KHZ  0
#define AI_SAMPLERATE_48KHZ  1

AIDCallback AIRegisterDMACallback(AIDCallback callback);
void AIInitDMA(u32 start_addr, u32 length);
BOOL AIGetDMAEnableFlag(void);
void AIStartDMA(void);
void AIStopDMA(void);
u32 AIGetDMABytesLeft(void);
u32 AIGetDMAStartAddr(void);
u32 AIGetDMALength(void);
BOOL AICheckInit(void);
void AISetDSPSampleRate(u32 rate);
u32 AIGetDSPSampleRate(void);
void AIInit(u8* stack);
void AIReset(void);

#ifdef __cplusplus
}
#endif

#endif
