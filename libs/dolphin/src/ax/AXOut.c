#include <dolphin/dolphin.h>
#include <dolphin/dsp.h>
#include <dolphin/ax.h>

#include "__ax.h"

static s16 __AXOutBuffer[3][320];
static s32 __AXOutSBuffer[160];
static u16 __AXDramImage[8192];
static DSPTaskInfo __AXDSPTask;
AXPROFILE __AXLocalProfile;

volatile static u32 __AXOutFrame;
volatile static u32 __AXAiDmaFrame;
volatile static u32 __AXOutDspReady;
volatile static OSTime __AXOsTime;
static void (*__AXUserFrameCallback)();
volatile static int __AXDSPInitFlag;
static int __AXDSPDoneFlag;

static volatile u32 __AXDebugSteppingMode;
static OSThreadQueue __AXOutThreadQueue;
static u32 __AXOutputBufferMode;

// prototypes
static void __AXDSPInitCallback(void* task);
static void __AXDSPResumeCallback(void* task);
static void __AXDSPDoneCallback(void* task);

void __AXOutNewFrame(u32 lessDspCycles) {
    u32 cl;
    AXPROFILE* profile;
    u8* src;
    u8* dest;
    u32 i;

    __AXLocalProfile.axFrameStart = OSGetTime();
    __AXSyncPBs(lessDspCycles);
    __AXPrintStudio();
    cl = __AXGetCommandListAddress();

    DSPSendMailToDSP(0xBABE0180);
    do {} while (DSPCheckMailToDSP() != 0);

    DSPSendMailToDSP(cl);
    do {} while (DSPCheckMailToDSP() != 0);

    __AXServiceCallbackStack();
    __AXLocalProfile.auxProcessingStart = OSGetTime();
    __AXProcessAux();
    __AXLocalProfile.auxProcessingEnd = OSGetTime();
    __AXLocalProfile.userCallbackStart = OSGetTime();

    if (__AXUserFrameCallback) {
        __AXUserFrameCallback();
    }

    __AXLocalProfile.userCallbackEnd = OSGetTime();
    __AXNextFrame(__AXOutSBuffer, &__AXOutBuffer[__AXOutFrame][0]);
    __AXOutFrame += 1;

    if (__AXOutputBufferMode == 1) {
        __AXOutFrame %= 3;
    } else {
        __AXOutFrame &= 1;
        AIInitDMA((u32)&__AXOutBuffer[__AXOutFrame][0], 0x280);
    }

    __AXLocalProfile.axFrameEnd = OSGetTime();
    __AXLocalProfile.axNumVoices = __AXGetNumVoices();
    profile = (void*)__AXGetCurrentProfile();

    if (profile) {
        i = 56;
        dest = (u8*)profile;
        src = (u8*)&__AXLocalProfile;

        while (i != 0) {
            *dest = *src;
            dest++;
            src++;
            i--;
        }
    }
}

void __AXOutAiCallback(void) {
    if (__AXOutDspReady == 0) {
        __AXOsTime = OSGetTime();
    }

    if (__AXOutDspReady == 1) {
        __AXOutDspReady = 0;
        __AXOutNewFrame(0);
    } else {
        __AXOutDspReady = 2;
        DSPAssertTask(&__AXDSPTask);
    }

    if (__AXOutputBufferMode == 1) {
        AIInitDMA((u32)__AXOutBuffer[__AXAiDmaFrame], 0x280);
        __AXAiDmaFrame++;
        __AXAiDmaFrame %= 3;
    }
}

static void __AXDSPInitCallback(void* task) {
    __AXDSPInitFlag = 1;
}

void AXSetStepMode(u32 i) {
    __AXDebugSteppingMode = i;
}

static void __AXDSPResumeCallback(void* task) {
#if DEBUG
    if (__AXDebugSteppingMode != 0) {
        __AXOutDspReady = 1;
        return;
    }
#endif

    if (__AXOutDspReady == 2) {
        __AXOutDspReady = 0;
        __AXOutNewFrame((u32)(OSGetTime() - __AXOsTime) / 4);
        return;
    }
    __AXOutDspReady = 1;
}

static void __AXDSPDoneCallback(void* task) {
    __AXDSPDoneFlag = 1;
    OSWakeupThread(&__AXOutThreadQueue);
}

#define BUFFER_MEMSET(buffer, size)    \
    {                                  \
        u32* p = (u32*)&buffer;        \
        int i;                         \
        for (i = 0; i < size; i++) {   \
            *p = 0;                    \
            p++;                       \
        }                              \
    }

void __AXOutInitDSP(void) {
    __AXDSPTask.iram_mmem_addr = axDspSlave;
    __AXDSPTask.iram_length = axDspSlaveLength;
    __AXDSPTask.iram_addr = 0;
    __AXDSPTask.dram_mmem_addr = __AXDramImage;
    __AXDSPTask.dram_length = 0x2000;
    __AXDSPTask.dram_addr = 0;
    __AXDSPTask.dsp_init_vector = 0x10;
    __AXDSPTask.dsp_resume_vector = 0x30;
    __AXDSPTask.init_cb = __AXDSPInitCallback;
    __AXDSPTask.res_cb = __AXDSPResumeCallback;
    __AXDSPTask.done_cb = __AXDSPDoneCallback;
    __AXDSPTask.req_cb = NULL;
    __AXDSPTask.priority = 0;
    __AXDSPInitFlag = 0;
    __AXDSPDoneFlag = 0;

    OSInitThreadQueue(&__AXOutThreadQueue);
    if (DSPCheckInit() == 0) {
        DSPInit();
    }

    DSPAddTask(&__AXDSPTask);
    do {} while (__AXDSPInitFlag == 0);
}

void __AXOutInit(u32 outputBufferMode) {
#if DEBUG
    OSReport("Initializing AXOut code module\n");
#endif
    ASSERTLINE(404, ((u32)&__AXOutBuffer[0][0] & 0x1F) == 0);
    ASSERTLINE(405, ((u32)&__AXOutBuffer[1][0] & 0x1F) == 0);
    ASSERTLINE(406, ((u32)&__AXOutBuffer[2][0] & 0x1F) == 0);
    ASSERTLINE(407, ((u32)&__AXOutSBuffer[0] & 0x1F) == 0);

    __AXOutputBufferMode = outputBufferMode;
    __AXOutFrame = 0;
    __AXAiDmaFrame = 0;
    __AXDebugSteppingMode = 0;

    BUFFER_MEMSET(__AXOutBuffer, 0x1E0);
    DCFlushRange(__AXOutBuffer, sizeof(__AXOutBuffer));

    BUFFER_MEMSET(__AXOutSBuffer, 0xA0);
    DCFlushRange(__AXOutSBuffer, sizeof(__AXOutSBuffer));

    __AXOutInitDSP();
    AIRegisterDMACallback(__AXOutAiCallback);

    if (__AXOutputBufferMode == 1) {
        __AXNextFrame(__AXOutSBuffer, &__AXOutBuffer[2][0]);
    } else {
        __AXNextFrame(__AXOutSBuffer, &__AXOutBuffer[1][0]);
    }

    __AXOutDspReady = 1;
    __AXUserFrameCallback = NULL;

    if (__AXOutputBufferMode == 1) {
        AIInitDMA((u32)&__AXOutBuffer[__AXAiDmaFrame][0], sizeof(__AXOutBuffer[0]));
        __AXAiDmaFrame++;
        __AXAiDmaFrame &= 1;
    } else {
        AIInitDMA((u32)&__AXOutBuffer[__AXOutFrame][0], sizeof(__AXOutBuffer[0]));
    }

    AIStartDMA();
}

void __AXOutQuit(void) {
    BOOL old;
#if DEBUG
    OSReport("Shutting down AXOut code module\n");
#endif
    old = OSDisableInterrupts();
    __AXUserFrameCallback = NULL;
    DSPCancelTask(&__AXDSPTask);
    OSSleepThread(&__AXOutThreadQueue);
    AIStopDMA();
    OSRestoreInterrupts(old);
}

AXCallback AXRegisterCallback(AXCallback callback) {
    BOOL enabled;
    AXCallback oldCB;

    oldCB = __AXUserFrameCallback;
    enabled = OSDisableInterrupts();
    __AXUserFrameCallback = callback;

    OSRestoreInterrupts(enabled);
    return oldCB;
}
