#ifndef _DOLPHIN_AI_H_
#define _DOLPHIN_AI_H_

#include <dolphin/types.h>

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
AISCallback AIRegisterStreamCallback(AISCallback callback);
u32 AIGetStreamSampleCount(void);
void AIResetStreamSampleCount(void);
void AISetStreamTrigger(u32 trigger);
u32 AIGetStreamTrigger(void);
void AISetStreamPlayState(u32 state);
u32 AIGetStreamPlayState(void);
void AISetDSPSampleRate(u32 rate);
u32 AIGetDSPSampleRate(void);
void AISetStreamSampleRate(u32 rate);
u32 AIGetStreamSampleRate(void);
void AISetStreamVolLeft(u8 vol);
u8 AIGetStreamVolLeft(void);
void AISetStreamVolRight(u8 vol);
u8 AIGetStreamVolRight(void);
void AIInit(u8* stack);
void AIReset(void);

#ifdef __cplusplus
}
#endif

#endif
