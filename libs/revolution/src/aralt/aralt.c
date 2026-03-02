#include <revolution.h>
#include <revolution/aralt.h>
#include "sdk_math.h"

#include <cstring>

#include "__ar.h"

static void (*__AR_Callback)();
static u32 __AR_Size;
static u32 __AR_InternalSize;
static u32 __ARH_BaseAdr;
static ARQRequest* __ARQRequestQueueHi;
static ARQRequest* __ARQRequestQueueLo;
static ARQRequest* __ARQRequestPendingHi;
static ARQRequest* __ARQRequestPendingLo;
static ARQCallback __ARQCallbackHi;
static ARQCallback __ARQCallbackLo;
static u32 __ARQChunkSize;
static BOOL __ARQ_init_flag;
static BOOL __AR_init_flag;

static u32 __ARALT_AramStartAdr = 0x90000000;
static u32 __ARH_MemoryTop = 0x90000000;

ARQCallback ARRegisterDMACallback(ARQCallback callback) {
    ARQCallback old_callback;
    BOOL old;

    old_callback = __AR_Callback;
    old = OSDisableInterrupts();
    __AR_Callback = callback;
    OSRestoreInterrupts(old);
    return old_callback;
}

void ARStartDMA(u32 type, u32 mainmem_addr, u32 aram_addr, u32 length) {
    OSDisableScheduler();

    if (type == 0) {
        aram_addr += __ARH_BaseAdr;
        DCInvalidateRange((u32*)mainmem_addr, length);
        memcpy((u32*)aram_addr, (u32*)mainmem_addr, length);
        DCFlushRange((u32*)aram_addr, length);
    } else if (type == 1) {
        mainmem_addr += __ARH_BaseAdr;
        DCFlushRange((u32*)mainmem_addr, length);
        memcpy((u32*)aram_addr, (u32*)mainmem_addr, length);
        DCFlushRange((u32*)aram_addr, length);
    }

    OSEnableScheduler();
    
    if (__AR_Callback != NULL) {
        (*__AR_Callback)();
    }
}

u32 ARAlloc(u32 length) {
    u32 tmp;
    BOOL old;

    old = OSDisableInterrupts();
    tmp = __ARH_MemoryTop;
    OSRestoreInterrupts(old);

    __ARH_MemoryTop += length;
    return tmp - __ARH_BaseAdr;
}

u32 ARInit(u32* stack_index_addr, u32 num_entries) {
    BOOL old;

    if (__AR_init_flag) {
        return ARGetBaseAddress();
    }

    old = OSDisableInterrupts();
    __AR_Callback = NULL;
    __AR_init_flag = TRUE;
    OSRestoreInterrupts(old);

    {
        u32 start;
        u32 end;
        start = __ARALT_AramStartAdr;
        end = start + 0x1100000;

        __ARH_MemoryTop = ARGetBaseAddress() + start;
        __ARH_BaseAdr = start;
        OSReport("ARInit : Dummy ARAM enabled (RVL), area %p -> %p (size 0x%x)\n", start, end, end - start);
        __AR_Size = end - start;
        __AR_InternalSize = __AR_Size;
    }

    return ARGetBaseAddress();
}

u32 ARGetBaseAddress(void) {
    return 0x4000;
}

u32 ARGetSize(void) {
    return __AR_Size;
}

void __ARQPopTaskQueueHi(void) {
    if (__ARQRequestQueueHi) {
        if (__ARQRequestQueueHi->type == 0) {
            ARStartDMA(__ARQRequestQueueHi->type, __ARQRequestQueueHi->source, __ARQRequestQueueHi->dest, __ARQRequestQueueHi->length);
        } else {
            ARStartDMA(__ARQRequestQueueHi->type, __ARQRequestQueueHi->dest, __ARQRequestQueueHi->source, __ARQRequestQueueHi->length);
        }
        __ARQCallbackHi = __ARQRequestQueueHi->callback;
        __ARQRequestPendingHi = __ARQRequestQueueHi;
        __ARQRequestQueueHi = __ARQRequestQueueHi->next;
    }
}

void __ARQServiceQueueLo(void) {
    if (__ARQRequestPendingLo == NULL && __ARQRequestQueueLo) {
        __ARQRequestPendingLo = __ARQRequestQueueLo;
        __ARQRequestQueueLo = __ARQRequestQueueLo->next;
    }

    if (__ARQRequestPendingLo) {
        if (__ARQRequestPendingLo->length <= __ARQChunkSize) {
            if (__ARQRequestPendingLo->type == 0) {
                ARStartDMA(__ARQRequestPendingLo->type, __ARQRequestPendingLo->source, __ARQRequestPendingLo->dest, __ARQRequestPendingLo->length);
            } else {
                ARStartDMA(__ARQRequestPendingLo->type, __ARQRequestPendingLo->dest, __ARQRequestPendingLo->source, __ARQRequestPendingLo->length);
            }
            __ARQCallbackLo = __ARQRequestPendingLo->callback;
        } else if (__ARQRequestPendingLo->type == 0) {
            ARStartDMA(__ARQRequestPendingLo->type, __ARQRequestPendingLo->source, __ARQRequestPendingLo->dest, __ARQChunkSize);
        } else {
            ARStartDMA(__ARQRequestPendingLo->type, __ARQRequestPendingLo->dest, __ARQRequestPendingLo->source, __ARQChunkSize);
        }

        __ARQRequestPendingLo->length -= __ARQChunkSize;
        __ARQRequestPendingLo->source += __ARQChunkSize;
        __ARQRequestPendingLo->dest += __ARQChunkSize;
    }
}

void __ARQInterruptServiceRoutine(u32) {
    if (__ARQCallbackHi) {
        __ARQCallbackHi((u32)__ARQRequestPendingHi);
        __ARQRequestPendingHi = NULL;
        __ARQCallbackHi = NULL;        
    } else if (__ARQCallbackLo) {
        __ARQCallbackLo((u32)__ARQRequestPendingLo);
        __ARQRequestPendingLo = NULL;
        __ARQCallbackLo = NULL;        
    }

    __ARQPopTaskQueueHi();

    if (__ARQRequestPendingHi == NULL) {
        __ARQServiceQueueLo();
    }
}

void ARQInit(void) {
    if (__ARQ_init_flag != TRUE) {
        __ARQRequestQueueHi = __ARQRequestQueueLo = NULL;
        __ARQChunkSize = 0x1000;
        ARRegisterDMACallback(__ARQInterruptServiceRoutine);
        __ARQRequestPendingHi = NULL;
        __ARQRequestPendingLo = NULL;
        __ARQCallbackHi = NULL;
        __ARQCallbackLo = NULL;
        __ARQ_init_flag = TRUE;
    }
}
