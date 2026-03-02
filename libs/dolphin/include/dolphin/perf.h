#ifndef _DOLPHIN_PERF_H_
#define _DOLPHIN_PERF_H_

#include <dolphin/gx.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef u8 PERFId;

typedef enum  {
    PERF_CPU_EVENT,
    PERF_CPU_GP_EVENT,
    PERF_GP_EVENT,
} PerfType;

typedef struct PerfSample {
    /* 0x00 */ u8 id;
    /* 0x04 */ u32 cpuTimeStampStart;
    /* 0x08 */ u32 cpuTimeStampEnd;
    /* 0x0C */ u32 gpTimeStampStart;
    /* 0x10 */ u32 gpTimeStampEnd;
    /* 0x14 */ int interrupted;
    /* 0x18 */ u32 origcpuStart;
    /* 0x1C */ u32 origgpStart;
    /* 0x20 */ u32 cacheMisses[4];
    /* 0x30 */ u32 instructions[4];
    /* 0x40 */ u32 cpReq[2];
    /* 0x48 */ u32 tcReq[2];
    /* 0x50 */ u32 cpuRdReq[2];
    /* 0x58 */ u32 cpuWrReq[2];
    /* 0x60 */ u32 dspReq[2];
    /* 0x68 */ u32 ioReq[2];
    /* 0x70 */ u32 viReq[2];
    /* 0x78 */ u32 peReq[2];
    /* 0x80 */ u32 rfReq[2];
    /* 0x88 */ u32 fiReq[2];
    /* 0x90 */ u32 xfWaitIn[2];
    /* 0x98 */ u32 xfWaitOut[2];
    /* 0xA0 */ u32 rasBusy[2];
    /* 0xA8 */ u32 rasClocks[2];
} PerfSample;

typedef struct {
    PerfSample* samples;
    s32 lastSample;
    u32 end;
    u32 cachemisscycles;
} Frame;

typedef struct {
    char* name;
    PerfType type; 
    s32 currSample;
    GXColor color;
} PerfEvent;

typedef void* (*PERFAllocator)(u32 size);
typedef void (*PERFDeallocator)(void* block);
typedef void (*PERFDrawCallback)(void);

extern void (*GameDrawInit)();

u32 PERFInit(u32 numSamples, u32 numFramesHistory, u32 numTypes, PERFAllocator allocator, PERFDeallocator deallocator, PERFDrawCallback initDraw);
void PERFEventStart(PERFId id);
void PERFEventEnd(PERFId id);
void PERFSetEvent(PERFId id, char* name, PerfType type);
void PERFStartFrame(void);
void PERFEndFrame(void);
void PERFStartAutoSampling(f32 msInterval);
void PERFStopAutoSampling(void);

void PERFPreDraw(void);
void PERFDumpScreen(void);
void PERFPostDraw(void);
void PERFSetDrawBWBarKey(BOOL tf);
void PERFSetDrawBWBar(BOOL tf);
void PERFSetDrawCPUBar(BOOL tf);
void PERFSetDrawXFBars(BOOL tf);
void PERFSetDrawRASBar(BOOL tf);
void PERFToggleDrawBWBarKey(void);
void PERFToggleDrawBWBar(void);
void PERFToggleDrawCPUBar(void);
void PERFToggleDrawXFBars(void);
void PERFToggleDrawRASBar(void);
void PERFShutDown(void);
void PERFSetDrawFrames(u32 frames);

extern Frame* PERFFrames;
extern u32 PERFCurrFrame;
extern PerfEvent* PERFEvents;
extern u32 PERFNumEvents;

#ifdef __cplusplus
}
#endif

#endif
