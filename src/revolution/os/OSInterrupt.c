#include <revolution.h>
#include <revolution/os.h>

#include "__os.h"

#if DEBUG
u64 __OSSpuriousInterrupts = 0;
#endif
static __OSInterruptHandler* InterruptHandlerTable;

volatile OSTime __OSLastInterruptTime;
volatile __OSInterrupt __OSLastInterrupt;
volatile u32 __OSLastInterruptSrr0;

static OSInterruptMask InterruptPrioTable[] = {
    OS_INTERRUPTMASK_PI_ERROR,
    OS_INTERRUPTMASK_PI_DEBUG,
    OS_INTERRUPTMASK_MEM,
    OS_INTERRUPTMASK_PI_RSW,
    OS_INTERRUPTMASK_PI_VI,
    0x10,
    OS_INTERRUPTMASK_PI_PE,
    OS_INTERRUPTMASK_PI_HSP,
    OS_INTERRUPTMASK_DSP_ARAM | OS_INTERRUPTMASK_DSP_DSP | OS_INTERRUPTMASK_AI |
        OS_INTERRUPTMASK_EXI | OS_INTERRUPTMASK_PI_SI | OS_INTERRUPTMASK_PI_DI,
    OS_INTERRUPTMASK_DSP_AI,
    OS_INTERRUPTMASK_PI_CP,
    0xFFFFFFFF,
};

#if DEBUG
char* __OSInterruptNames[33] = {
    "MEM_0",
    "MEM_1",
    "MEM_2",
    "MEM_3",
    "MEM_ADDRESS",
    "DSP_AI",
    "DSP_ARAM",
    "DSP_DSP",
    "AI_AI",
    "EXI_0_EXI",
    "EXI_0_TC",
    "EXI_0_EXT",
    "EXI_1_EXI",
    "EXI_1_TC",
    "EXI_1_EXT",
    "EXI_2_EXI",
    "EXI_2_TC",
    "PI_CP",
    "PI_PE_TOKEN",
    "PI_PE_FINISH",
    "PI_SI",
    "PI_DI",
    "PI_RSW",
    "PI_ERROR",
    "PI_VI",
    "PI_DEBUG",
    "PI_HSP",
    "PI_ACR",
    "unknown",
    "unknown",
    "unknown",
    "unknown",
    "unknown",
};

char* __OSPIErrors[8] = {
    "No Error",
    "Misaligned address for CPU request",
    "Incorrect transfer type (tt) from CPU",
    "Unsupported transfer size",
    "Address out of range",
    "Write to ROM address space",
    "Read from GX Fifo",
    "Reserved error code",
};
#endif

// prototypes
static void ExternalInterruptHandler(register __OSException exception, register OSContext* context);
extern void __RAS_OSDisableInterrupts_begin(void);
extern void __RAS_OSDisableInterrupts_end(void);

#ifdef __GEKKO__
asm BOOL OSDisableInterrupts(void) {
    nofralloc
entry    __RAS_OSDisableInterrupts_begin
    mfmsr   r3
    rlwinm  r4, r3, 0, 17, 15
    mtmsr   r4
entry    __RAS_OSDisableInterrupts_end
    rlwinm  r3, r3, 17, 31, 31
    blr
}

asm BOOL OSEnableInterrupts(void) {
    nofralloc

    mfmsr   r3
    ori     r4, r3, 0x8000
    mtmsr   r4
    rlwinm  r3, r3, 17, 31, 31
    blr
}

asm BOOL OSRestoreInterrupts(register BOOL level) {
    nofralloc

    cmpwi   level, 0
    mfmsr   r4
    beq     _disable
    ori     r5, r4, 0x8000
    b       _restore
_disable:
    rlwinm  r5, r4, 0, 17, 15
_restore:
    mtmsr   r5
    rlwinm  r3, r4, 17, 31, 31
    blr
}
#endif

__OSInterruptHandler __OSSetInterruptHandler(__OSInterrupt interrupt, __OSInterruptHandler handler) {
    __OSInterruptHandler oldHandler;
 
    ASSERTMSGLINE(482, InterruptHandlerTable, "__OSSetInterruptHandler(): OSInit() must be called in advance.");
    ASSERTMSGLINE(484, interrupt < 0x20, "__OSSetInterruptHandler(): unknown interrupt.");

    oldHandler = InterruptHandlerTable[interrupt];
    InterruptHandlerTable[interrupt] = handler;
    return oldHandler;
}

__OSInterruptHandler __OSGetInterruptHandler(__OSInterrupt interrupt) {
    ASSERTMSGLINE(504, InterruptHandlerTable, "__OSGetInterruptHandler(): OSInit() must be called in advance.");
    ASSERTMSGLINE(506, interrupt < 0x20, "__OSGetInterruptHandler(): unknown interrupt.");
    return InterruptHandlerTable[interrupt];
}

void __OSInterruptInit(void) {
    InterruptHandlerTable = (void*)OSPhysicalToCached(0x3040);

    memset(InterruptHandlerTable, 0, __OS_INTERRUPT_MAX * sizeof(__OSInterruptHandler));

    *(OSInterruptMask*)OSPhysicalToCached(0x00C4) = 0;
    *(OSInterruptMask*)OSPhysicalToCached(0x00C8) = 0;
    *(u32*)0xCC003004 = 240;
    *(u32*)0xCD000034 = 0x40000000;

    __OSMaskInterrupts(OS_INTERRUPTMASK_MEM | OS_INTERRUPTMASK_DSP | OS_INTERRUPTMASK_AI |
                        OS_INTERRUPTMASK_EXI | OS_INTERRUPTMASK_PI);

    __OSSetExceptionHandler(4, ExternalInterruptHandler);
    #if DEBUG
        __PIRegs[0] = 1;
        __OSUnmaskInterrupts(0x100);
    #endif
}

static u32 SetInterruptMask(OSInterruptMask mask, OSInterruptMask current) {
    u32 reg;

    switch (__cntlzw(mask)) {
    case __OS_INTERRUPT_MEM_0:
    case __OS_INTERRUPT_MEM_1:
    case __OS_INTERRUPT_MEM_2:
    case __OS_INTERRUPT_MEM_3:
    case __OS_INTERRUPT_MEM_ADDRESS:
        reg = 0;
        if (!(current & OS_INTERRUPTMASK_MEM_0))
            reg |= 0x1;
        if (!(current & OS_INTERRUPTMASK_MEM_1))
            reg |= 0x2;
        if (!(current & OS_INTERRUPTMASK_MEM_2))
            reg |= 0x4;
        if (!(current & OS_INTERRUPTMASK_MEM_3))
            reg |= 0x8;
        if (!(current & OS_INTERRUPTMASK_MEM_ADDRESS))
            reg |= 0x10;
        __MEMRegs[0x0000000e] = (u16)reg;
        mask &= ~OS_INTERRUPTMASK_MEM;
        break;
    case __OS_INTERRUPT_DSP_AI:
    case __OS_INTERRUPT_DSP_ARAM:
    case __OS_INTERRUPT_DSP_DSP:
        reg = __DSPRegs[0x00000005];
        reg &= ~0x1F8;
        if (!(current & OS_INTERRUPTMASK_DSP_AI))
            reg |= 0x10;
        if (!(current & OS_INTERRUPTMASK_DSP_ARAM))
            reg |= 0x40;
        if (!(current & OS_INTERRUPTMASK_DSP_DSP))
            reg |= 0x100;
        __DSPRegs[0x00000005] = (u16)reg;
        mask &= ~OS_INTERRUPTMASK_DSP;
        break;
    case __OS_INTERRUPT_AI_AI:
        reg = __AIRegs[0];
        reg &= ~0x2C;
        if (!(current & OS_INTERRUPTMASK_AI_AI))
            reg |= 0x4;
        __AIRegs[0] = reg;
        mask &= ~OS_INTERRUPTMASK_AI;
        break;
    case __OS_INTERRUPT_EXI_0_EXI:
    case __OS_INTERRUPT_EXI_0_TC:
    case __OS_INTERRUPT_EXI_0_EXT:
        reg = __EXIRegs[0];
        reg &= ~0x2C0F;
        if (!(current & OS_INTERRUPTMASK_EXI_0_EXI))
            reg |= 0x1;
        if (!(current & OS_INTERRUPTMASK_EXI_0_TC))
            reg |= 0x4;
        if (!(current & OS_INTERRUPTMASK_EXI_0_EXT))
            reg |= 0x400;
        __EXIRegs[0] = reg;
        mask &= ~OS_INTERRUPTMASK_EXI_0;
        break;
    case __OS_INTERRUPT_EXI_1_EXI:
    case __OS_INTERRUPT_EXI_1_TC:
    case __OS_INTERRUPT_EXI_1_EXT:
        reg = __EXIRegs[5];
        reg &= ~0xC0F;

        if (!(current & OS_INTERRUPTMASK_EXI_1_EXI))
            reg |= 0x1;
        if (!(current & OS_INTERRUPTMASK_EXI_1_TC))
            reg |= 0x4;
        if (!(current & OS_INTERRUPTMASK_EXI_1_EXT))
            reg |= 0x400;
        __EXIRegs[5] = reg;
        mask &= ~OS_INTERRUPTMASK_EXI_1;
        break;
    case __OS_INTERRUPT_EXI_2_EXI:
    case __OS_INTERRUPT_EXI_2_TC:
        reg = __EXIRegs[10];
        reg &= ~0xF;
        if (!(current & OS_INTERRUPTMASK_EXI_2_EXI))
            reg |= 0x1;
        if (!(current & OS_INTERRUPTMASK_EXI_2_TC))
            reg |= 0x4;

        __EXIRegs[10] = reg;
        mask &= ~OS_INTERRUPTMASK_EXI_2;
        break;
    case __OS_INTERRUPT_PI_CP:
    case __OS_INTERRUPT_PI_SI:
    case __OS_INTERRUPT_PI_DI:
    case __OS_INTERRUPT_PI_RSW:
    case __OS_INTERRUPT_PI_ERROR:
    case __OS_INTERRUPT_PI_VI:
    case __OS_INTERRUPT_PI_DEBUG:
    case __OS_INTERRUPT_PI_PE_TOKEN:
    case __OS_INTERRUPT_PI_PE_FINISH:
    case __OS_INTERRUPT_PI_HSP:
    case __OS_INTERRUPT_PI_ACR:
        reg = 0xF0;

        if (!(current & OS_INTERRUPTMASK_PI_CP)) {
            reg |= 0x800;
        }
        if (!(current & OS_INTERRUPTMASK_PI_SI)) {
            reg |= 0x8;
        }
        if (!(current & OS_INTERRUPTMASK_PI_DI)) {
            reg |= 0x4;
        }
        if (!(current & OS_INTERRUPTMASK_PI_RSW)) {
            reg |= 0x2;
        }
        if (!(current & OS_INTERRUPTMASK_PI_ERROR)) {
            reg |= 0x1;
        }
        if (!(current & OS_INTERRUPTMASK_PI_VI)) {
            reg |= 0x100;
        }
        if (!(current & OS_INTERRUPTMASK_PI_DEBUG)) {
            reg |= 0x1000;
        }
        if (!(current & OS_INTERRUPTMASK_PI_PE_TOKEN)) {
            reg |= 0x200;
        }
        if (!(current & OS_INTERRUPTMASK_PI_PE_FINISH)) {
            reg |= 0x400;
        }
        if (!(current & OS_INTERRUPTMASK_PI_HSP)) {
            reg |= 0x2000;
        }
        if (!(current & OS_INTERRUPTMASK_PI_ACR)) {
            reg |= 0x4000;
        }

        __PIRegs[1] = reg;
        mask &= ~OS_INTERRUPTMASK_PI;
        break;
    default:
        break;
    }
    return mask;
}

OSInterruptMask OSGetInterruptMask(void) { 
    return *(OSInterruptMask *)OSPhysicalToCached(0x00C8);
}

OSInterruptMask OSSetInterruptMask(OSInterruptMask local) {
    BOOL enabled;
    OSInterruptMask global;
    OSInterruptMask prev;
    OSInterruptMask mask;

    enabled = OSDisableInterrupts();
    global = *(OSInterruptMask *)OSPhysicalToCached(0x00C4);
    prev = *(OSInterruptMask *)OSPhysicalToCached(0x00C8);
    mask = (global | prev) ^ local;
    *(OSInterruptMask *)OSPhysicalToCached(0x00C8) = local;
    while (mask) {
        mask = SetInterruptMask(mask, global | local);
    }
    OSRestoreInterrupts(enabled);
    return prev;
}

OSInterruptMask __OSMaskInterrupts(OSInterruptMask global) {
    BOOL enabled;
    OSInterruptMask prev;
    OSInterruptMask local;
    OSInterruptMask mask;

    enabled = OSDisableInterrupts();
    prev = *(OSInterruptMask *)OSPhysicalToCached(0x00C4);
    local = *(OSInterruptMask *)OSPhysicalToCached(0x00C8);
    mask = ~(prev | local) & global;
    global |= prev;
    *(OSInterruptMask *)OSPhysicalToCached(0x00C4) = global;
    while (mask) {
        mask = SetInterruptMask(mask, global | local);
    }
    OSRestoreInterrupts(enabled);
    return prev;
}

OSInterruptMask __OSUnmaskInterrupts(OSInterruptMask global) {
    BOOL enabled;
    OSInterruptMask prev;
    OSInterruptMask local;
    OSInterruptMask mask;

    enabled = OSDisableInterrupts();
    prev = *(OSInterruptMask *)OSPhysicalToCached(0x00C4);
    local = *(OSInterruptMask *)OSPhysicalToCached(0x00C8);
    mask = (prev | local) & global;
    global = prev & ~global;
    *(OSInterruptMask *)OSPhysicalToCached(0x00C4) = global;
    while (mask) {
        mask = SetInterruptMask(mask, global | local);
    }
    OSRestoreInterrupts(enabled);
    return prev;
}

void __OSDispatchInterrupt(__OSException exception, OSContext* context) {
    u32 intsr;
    u32 mask;
    u32 reg;
    OSInterruptMask cause;
    OSInterruptMask unmasked;
    OSInterruptMask* prio;
    __OSInterrupt interrupt;
    __OSInterruptHandler handler;

    intsr = __PIRegs[0];
    intsr &= ~0x00010000;
    mask = __PIRegs[1];

    if (intsr == 0 || (intsr & mask) == 0) {
    #if DEBUG
        __OSSpuriousInterrupts++;
    #endif
        OSLoadContext(context);
    }

    cause = 0;

    if (intsr & 0x00000080) {
        reg = __MEMRegs[15];
        if (reg & 0x1)
            cause |= OS_INTERRUPTMASK_MEM_0;
        if (reg & 0x2)
            cause |= OS_INTERRUPTMASK_MEM_1;
        if (reg & 0x4)
            cause |= OS_INTERRUPTMASK_MEM_2;
        if (reg & 0x8)
            cause |= OS_INTERRUPTMASK_MEM_3;
        if (reg & 0x10)
            cause |= OS_INTERRUPTMASK_MEM_ADDRESS;
    }

    if (intsr & 0x00000040) {
        reg = __DSPRegs[5];
        if (reg & 0x8)
            cause |= OS_INTERRUPTMASK_DSP_AI;
        if (reg & 0x20)
            cause |= OS_INTERRUPTMASK_DSP_ARAM;
        if (reg & 0x80)
            cause |= OS_INTERRUPTMASK_DSP_DSP;
    }

    if (intsr & 0x00000020) {
        reg = __AIRegs[0];
        if (reg & 0x8)
            cause |= OS_INTERRUPTMASK_AI_AI;
    }

    if (intsr & 0x00000010) {
        reg = __EXIRegs[0];
        if (reg & 0x2)
            cause |= OS_INTERRUPTMASK_EXI_0_EXI;
        if (reg & 0x8)
            cause |= OS_INTERRUPTMASK_EXI_0_TC;
        if (reg & 0x800)
            cause |= OS_INTERRUPTMASK_EXI_0_EXT;
        reg = __EXIRegs[5];
        if (reg & 0x2)
            cause |= OS_INTERRUPTMASK_EXI_1_EXI;
        if (reg & 0x8)
            cause |= OS_INTERRUPTMASK_EXI_1_TC;
        if (reg & 0x800)
            cause |= OS_INTERRUPTMASK_EXI_1_EXT;
        reg = __EXIRegs[10];
        if (reg & 0x2)
            cause |= OS_INTERRUPTMASK_EXI_2_EXI;
        if (reg & 0x8)
            cause |= OS_INTERRUPTMASK_EXI_2_TC;
    }

    if (intsr & 0x00002000)
        cause |= OS_INTERRUPTMASK_PI_HSP;
    if (intsr & 0x00001000)
        cause |= OS_INTERRUPTMASK_PI_DEBUG;
    if (intsr & 0x00000400)
        cause |= OS_INTERRUPTMASK_PI_PE_FINISH;
    if (intsr & 0x00000200)
        cause |= OS_INTERRUPTMASK_PI_PE_TOKEN;
    if (intsr & 0x00000100)
        cause |= OS_INTERRUPTMASK_PI_VI;
    if (intsr & 0x00000008)
        cause |= OS_INTERRUPTMASK_PI_SI;
    if (intsr & 0x00000004)
        cause |= OS_INTERRUPTMASK_PI_DI;
    if (intsr & 0x00000002)
        cause |= OS_INTERRUPTMASK_PI_RSW;
    if (intsr & 0x00000800)
        cause |= OS_INTERRUPTMASK_PI_CP;
    if (intsr & 0x00000001)
        cause |= OS_INTERRUPTMASK_PI_ERROR;
    if (intsr & 0x4000)
        cause |= OS_INTERRUPTMASK_PI_ACR;

    #if DEBUG
    if (cause & OS_INTERRUPTMASK_PI_ERROR) {
        OSReport("PI ERROR\n");
        OSDumpContext(context);
        OSReport("\nPIESR = 0x%08x                  PIEAR  = 0x%08x\n", __PIRegs[7], __PIRegs[8]);
        __PIRegs[0] = 1;
        OSReport("PI Error = %s\n", __OSPIErrors[__PIRegs[7]]);
        OSReport("Offending address = 0x%x (from PIEAR)\n", __PIRegs[8]);
    }
    #endif

    unmasked = cause & ~(*(OSInterruptMask *)OSPhysicalToCached(0x00C4) |
                        *(OSInterruptMask *)OSPhysicalToCached(0x00C8));
    if (unmasked) {
        for (prio = InterruptPrioTable;; ++prio) {
            if (unmasked & *prio) {
                interrupt = (__OSInterrupt)__cntlzw(unmasked & *prio);
                break;
            }
        }

        handler = __OSGetInterruptHandler(interrupt);
        if (handler) {
            if (__OS_INTERRUPT_MEM_ADDRESS < interrupt) {
                __OSLastInterrupt = interrupt;
                __OSLastInterruptTime = OSGetTime();
                __OSLastInterruptSrr0 = context->srr0;
            }
            OSDisableScheduler();
            handler(interrupt, context);
            OSEnableScheduler();
            __OSReschedule();
            OSLoadContext(context);
        }
    }

    #if DEBUG
    OSReport("Unhandled Interrupt(s): cause %08x  intsr %08x\n", cause, intsr);
    while (cause) {
        interrupt = __cntlzw(cause);
        cause &= ~(1 << (0x1F - __cntlzw(cause)));
        OSReport("    %s\n", __OSInterruptNames[interrupt]);
    }
    #endif

    OSLoadContext(context);
}

#ifdef __GEKKO__
static asm void ExternalInterruptHandler(register __OSException exception,
                                         register OSContext* context) {
#pragma unused(exception)
    nofralloc 
    OS_EXCEPTION_SAVE_GPRS(context)

    stwu r1, -0x8(r1)
    b __OSDispatchInterrupt
}
#endif
