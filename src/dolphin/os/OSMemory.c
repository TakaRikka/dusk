#include <dolphin.h>
#include <dolphin/os.h>
#include <dolphin/dvd.h>

#include "__os.h"

#define TRUNC(n, a) (((u32)(n)) & ~((a)-1))
#define ROUND(n, a) (((u32)(n) + (a)-1) & ~((a)-1))

static BOOL OnReset(BOOL final);

static OSResetFunctionInfo ResetFunctionInfo = {
    OnReset,
    0x7F,
    NULL,
    NULL
};

u32 OSGetPhysicalMemSize(void) {
#if DEBUG
    OSBootInfo* BootInfo = (OSBootInfo*)OSPhysicalToCached(0);

    return BootInfo->memorySize;
#else
    return __OSPhysicalMemSize;
#endif
}

u32 OSGetConsoleSimulatedMemSize(void) {
#if DEBUG
    u32* memSize = (u32*)OSPhysicalToCached(0xF0);

    return *memSize;
#else
    return __OSSimulatedMemSize;
#endif
}

static BOOL OnReset(BOOL final) {
    if (final != FALSE) {
        __MEMRegs[8] = 0xFF;
        __OSMaskInterrupts(OS_INTERRUPTMASK_MEM_RESET);
    }
    return TRUE;
}

void (*__OSErrorTable[])(u16, OSContext*, ...);

static void MEMIntrruptHandler(__OSInterrupt interrupt, OSContext* context) {
    u32 addr;
    u32 cause;

    cause = __MEMRegs[0xf];
    addr = (((u32)__MEMRegs[0x12] & 0x3ff) << 16) | __MEMRegs[0x11];
    __MEMRegs[0x10] = 0;

    if (__OSErrorTable[__OS_EXCEPTION_MEMORY_PROTECTION]) {
        __OSErrorTable[__OS_EXCEPTION_MEMORY_PROTECTION](__OS_EXCEPTION_MEMORY_PROTECTION, context, cause, addr);
        return;
    }

    __OSUnhandledException(__OS_EXCEPTION_MEMORY_PROTECTION, context, cause, addr);
}

void OSProtectRange(u32 chan, void* addr, u32 nBytes, u32 control) {
    BOOL enabled;
    u32 start;
    u32 end;
    u16 reg;

    ASSERTLINE(206, chan < 4);
    ASSERTLINE(207, (control & ~(OS_PROTECT_CONTROL_RDWR)) == 0);

    if (4 <= chan) {
        return;
    }

    control &= 3;

    end = (u32)addr + nBytes;
    start = TRUNC(addr, 1u << 10);
    end = ROUND(end, 1u << 10);

    DCFlushRange((void*)start, end - start);

    enabled = OSDisableInterrupts();

    __OSMaskInterrupts(OS_INTERRUPTMASK(__OS_INTERRUPT_MEM_0 + chan));

    __MEMRegs[0 + 2 * chan] = (u16)(start >> 10);
    __MEMRegs[1 + 2 * chan] = (u16)(end >> 10);

    reg = __MEMRegs[8];
    reg &= ~(3 << 2 * chan);
    reg |= control << 2 * chan;
    __MEMRegs[8] = reg;

    if (control != 3) {
        __OSUnmaskInterrupts(OS_INTERRUPTMASK(__OS_INTERRUPT_MEM_0 + chan));
    }

    OSRestoreInterrupts(enabled);
}

#ifdef __GEKKO__
static asm void Config24MB(void) {
    nofralloc
    li r7, 0x0
    lis r4, 0x0
    addi r4, r4, 0x2
    lis r3, 0x8000
    addi r3, r3, 0x1ff
    lis r6, 0x100
    addi r6, r6, 0x2
    lis r5, 0x8100
    addi r5, r5, 0xff
    isync
    mtdbatu 0, r7
    mtdbatl 0, r4
    mtdbatu 0, r3
    isync
    mtibatu 0, r7
    mtibatl 0, r4
    mtibatu 0, r3
    isync
    mtdbatu 2, r7
    mtdbatl 2, r6
    mtdbatu 2, r5
    isync
    mtibatu 2, r7
    mtibatl 2, r6
    mtibatu 2, r5
    isync
    mfmsr r3
    ori r3, r3, 0x30
    mtsrr1 r3
    mflr r3
    mtsrr0 r3
    rfi
}
#endif

#ifdef __GEKKO__
static asm void Config48MB(void) {
    nofralloc
    li r7, 0x0
    lis r4, 0x0
    addi r4, r4, 0x2
    lis r3, 0x8000
    addi r3, r3, 0x3ff
    lis r6, 0x200
    addi r6, r6, 0x2
    lis r5, 0x8200
    addi r5, r5, 0x1ff
    isync
    mtdbatu 0, r7
    mtdbatl 0, r4
    mtdbatu 0, r3
    isync
    mtibatu 0, r7
    mtibatl 0, r4
    mtibatu 0, r3
    isync
    mtdbatu 2, r7
    mtdbatl 2, r6
    mtdbatu 2, r5
    isync
    mtibatu 2, r7
    mtibatl 2, r6
    mtibatu 2, r5
    isync
    mfmsr r3
    ori r3, r3, 0x30
    mtsrr1 r3
    mflr r3
    mtsrr0 r3
    rfi
}
#endif

#ifdef __GEKKO__
static asm void RealMode(__REGISTER u32 addr) {
    nofralloc
    clrlwi addr, addr, 2
    mtsrr0 addr
    mfmsr addr
    rlwinm addr, addr, 0, 28, 25
    mtsrr1 addr
    rfi
}
#endif

void __OSInitMemoryProtection(void) {
#if !DEBUG
    u32 padding[9];
    u32 temp;
#endif
    BOOL enabled;
    u32 size;

    size = OSGetConsoleSimulatedMemSize();
    enabled = OSDisableInterrupts();

    __MEMRegs[16] = 0;
    __MEMRegs[8] = 0xFF;

    __OSMaskInterrupts(OS_INTERRUPTMASK_MEM_0 | OS_INTERRUPTMASK_MEM_1 | OS_INTERRUPTMASK_MEM_2 |
                       OS_INTERRUPTMASK_MEM_3);
    __OSSetInterruptHandler(__OS_INTERRUPT_MEM_0, MEMIntrruptHandler);
    __OSSetInterruptHandler(__OS_INTERRUPT_MEM_1, MEMIntrruptHandler);
    __OSSetInterruptHandler(__OS_INTERRUPT_MEM_2, MEMIntrruptHandler);
    __OSSetInterruptHandler(__OS_INTERRUPT_MEM_3, MEMIntrruptHandler);
    __OSSetInterruptHandler(__OS_INTERRUPT_MEM_ADDRESS, MEMIntrruptHandler);
    OSRegisterResetFunction(&ResetFunctionInfo);

#if DEBUG
    if (OSGetConsoleSimulatedMemSize() < OSGetPhysicalMemSize() && OSGetConsoleSimulatedMemSize() == 0x1800000)
#else
    temp = OSGetConsoleSimulatedMemSize();  // not sure how else to get the order right on retail
    if (temp < OSGetPhysicalMemSize() && temp == 0x1800000)
#endif
    {
        DCInvalidateRange((void*)0x81800000, 0x1800000);
        __MEMRegs[20] = 2;
    }

    if (size <= 0x1800000) {
        RealMode((u32)&Config24MB);
    } else if (size <= 0x3000000) {
        RealMode((u32)&Config48MB);
    }

    __OSUnmaskInterrupts(OS_INTERRUPTMASK_MEM_ADDRESS);
    OSRestoreInterrupts(enabled);
}
