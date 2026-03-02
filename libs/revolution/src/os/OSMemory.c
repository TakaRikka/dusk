#include <revolution.h>
#include <revolution/os.h>
#include <revolution/dvd.h>

#include "__os.h"

#define TRUNC(n, a) (((u32)(n)) & ~((a)-1))
#define ROUND(n, a) (((u32)(n) + (a)-1) & ~((a)-1))

static BOOL OnShutdown(BOOL final, u32 event);

static OSShutdownFunctionInfo ShutdownFunctionInfo = {
    OnShutdown,
    0x7F,
    NULL,
    NULL
};

u32 OSGetPhysicalMem1Size(void) {
    u32 memSize = *(u32*)OSPhysicalToCached(0x3100);
    return memSize;
}

u32 OSGetPhysicalMem2Size(void) {
    u32 memSize = *(u32*)OSPhysicalToCached(0x3118);
    return memSize;
}

u32 OSGetConsoleSimulatedMem1Size(void) {
    u32 memSize = *(u32*)OSPhysicalToCached(0x3104);
    return memSize;
}

u32 OSGetConsoleSimulatedMem2Size(void) {
    u32 memSize = *(u32*)OSPhysicalToCached(0x311C);
    return memSize;
}

static BOOL OnShutdown(BOOL final, u32 event) {
    if (final != FALSE) {
        __MEMRegs[8] = 0xFF;
        __OSMaskInterrupts(OS_INTERRUPTMASK_MEM_RESET);
    }
    return TRUE;
}

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

    ASSERTLINE(388, chan < 4);
    ASSERTLINE(389, (control & ~(OS_PROTECT_CONTROL_RDWR)) == 0);

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
static asm void ConfigMEM1_24MB(void) {
    nofralloc
    addi r7, r0, 0

    addis   r4,r0,0x00000002@ha
    addi    r4,r4,0x00000002@l
    addis   r3,r0,0x800001ff@ha
    addi    r3,r3,0x800001ff@l

    addis   r6,r0,0x01000002@ha
    addi    r6,r6,0x01000002@l
    addis   r5,r0,0x810000ff@ha
    addi    r5,r5,0x810000ff@l

    isync

    mtspr   dbat0u,r7
    mtspr   dbat0l,r4
    mtspr   dbat0u,r3
    isync

    mtspr   ibat0u,r7
    mtspr   ibat0l,r4
    mtspr   ibat0u,r3
    isync

    mtspr   dbat2u,r7
    mtspr   dbat2l,r6
    mtspr   dbat2u,r5
    isync

    mtspr   ibat2u,r7
    mtspr   ibat2l,r6
    mtspr   ibat2u,r5
    isync

    mfmsr   r3
    ori     r3, r3, 0x30
    mtsrr1  r3

    mflr r3
    mtsrr0 r3
    rfi
}
#endif

#ifdef __GEKKO__
static asm void ConfigMEM1_48MB(void) {
    nofralloc

    addi    r7,r0, 0

    addis   r4,r0,0x00000002@ha
    addi    r4,r4,0x00000002@l
    addis   r3,r0,0x800003ff@ha
    addi    r3,r3,0x800003ff@l

    addis   r6,r0,0x02000002@ha
    addi    r6,r6,0x02000002@l
    addis   r5,r0,0x820001ff@ha
    addi    r5,r5,0x820001ff@l

    isync

    mtspr   dbat0u,r7
    mtspr   dbat0l,r4
    mtspr   dbat0u,r3
    isync

    mtspr   ibat0u,r7
    mtspr   ibat0l,r4
    mtspr   ibat0u,r3
    isync

    mtspr   dbat2u,r7
    mtspr   dbat2l,r6
    mtspr   dbat2u,r5
    isync

    mtspr   ibat2u,r7
    mtspr   ibat2l,r6
    mtspr   ibat2u,r5
    isync

    mfmsr   r3
    ori     r3, r3, 0x30
    mtsrr1  r3

    mflr    r3
    mtsrr0  r3
    rfi
}
#endif

#ifdef __GEKKO__
static asm void ConfigMEM2_52MB(void) {
    nofralloc

    addi    r7, r0, 0

    addis   r4,r0,0x10000002@ha
    addi    r4,r4,0x10000002@l
    addis   r3,r0,0x900003ff@ha
    addi    r3,r3,0x900003ff@l

    addis   r6,r0,0x1000002a@ha
    addi    r6,r6,0x1000002a@l
    addis   r5,r0,0xD00007ff@ha
    addi    r5,r5,0xD00007ff@l

    isync

    mtspr   DBAT4U,r7
    mtspr   DBAT4L,r4
    mtspr   DBAT4U,r3
    isync

    mtspr   IBAT4U,r7
    mtspr   IBAT4L,r7

    isync

    mtspr   DBAT5U,r7
    mtspr   DBAT5L,r6
    mtspr   DBAT5U,r5
    isync

    mtspr   IBAT5U,r7
    mtspr   IBAT5L,r7
    isync

    addis   r4,r0,0x12000002@ha
    addi    r4,r4,0x12000002@l
    addis   r3,r0,0x920001ff@ha
    addi    r3,r3,0x920001ff@l

    addis   r6,r0,0x13000002@ha
    addi    r6,r6,0x13000002@l
    addis   r5,r0,0x9300007f@ha
    addi    r5,r5,0x9300007f@l

    isync

    mtspr   DBAT6U,r7
    mtspr   DBAT6L,r4
    mtspr   DBAT6U,r3
    isync

    mtspr   IBAT6U,r7
    mtspr   IBAT6L,r7

    isync

    mtspr   DBAT7U,r7
    mtspr   DBAT7L,r6
    mtspr   DBAT7U,r5
    isync

    mtspr   IBAT7U,r7
    mtspr   IBAT7L,r7

    isync

    mfmsr   r3
    ori     r3, r3, 0x30
    mtsrr1  r3

    mflr    r3
    mtsrr0  r3
    rfi
}
#endif

#ifdef __GEKKO__
static asm void ConfigMEM2_56MB(void) {
    nofralloc

    addi    r7,r0, 0

    addis   r4,r0,0x10000002@ha
    addi    r4,r4,0x10000002@l
    addis   r3,r0,0x900003ff@ha
    addi    r3,r3,0x900003ff@l

    addis   r6,r0,0x1000002a@ha
    addi    r6,r6,0x1000002a@l
    addis   r5,r0,0xD00007ff@ha
    addi    r5,r5,0xD00007ff@l

    isync

    mtspr   DBAT4U,r7
    mtspr   DBAT4L,r4
    mtspr   DBAT4U,r3
    isync

    mtspr   IBAT4U,r7
    mtspr   IBAT4L,r7

    isync

    mtspr   DBAT5U,r7
    mtspr   DBAT5L,r6
    mtspr   DBAT5U,r5
    isync

    mtspr   IBAT5U,r7
    mtspr   IBAT5L,r7
    isync

    addis   r4,r0,0x12000002@ha
    addi    r4,r4,0x12000002@l
    addis   r3,r0,0x920001ff@ha
    addi    r3,r3,0x920001ff@l

    addis   r6,r0,0x13000002@ha
    addi    r6,r6,0x13000002@l
    addis   r5,r0,0x930000ff@ha
    addi    r5,r5,0x930000ff@l

    isync

    mtspr   DBAT6U,r7
    mtspr   DBAT6L,r4
    mtspr   DBAT6U,r3
    isync

    mtspr   IBAT6U,r7
    mtspr   IBAT6L,r7

    isync

    mtspr   DBAT7U,r7
    mtspr   DBAT7L,r6
    mtspr   DBAT7U,r5
    isync

    mtspr   IBAT7U,r7
    mtspr   IBAT7L,r7

    isync

    mfmsr   r3
    ori     r3, r3, 0x30
    mtsrr1  r3

    mflr    r3
    mtsrr0  r3
    rfi
}
#endif

#ifdef __GEKKO__
static asm void ConfigMEM2_64MB(void) {
    nofralloc

    addi    r7,r0,0x0000

    addis   r4,r0,0x10000002@ha
    addi    r4,r4,0x10000002@l
    addis   r3,r0,0x900007ff@ha
    addi    r3,r3,0x900007ff@l

    addis   r6,r0,0x1000002a@ha
    addi    r6,r6,0x1000002a@l
    addis   r5,r0,0xD00007ff@ha
    addi    r5,r5,0xD00007ff@l

    isync

    mtspr   DBAT4U,r7
    mtspr   DBAT4L,r4
    mtspr   DBAT4U,r3
    isync

    mtspr   IBAT4U,r7
    mtspr   IBAT4L,r7

    isync

    mtspr   DBAT5U,r7
    mtspr   DBAT5L,r6
    mtspr   DBAT5U,r5
    isync

    mtspr   IBAT5U,r7
    mtspr   IBAT5L,r7
    isync

    mtspr   IBAT6U,r7
    mtspr   IBAT6L,r7
    isync

    mtspr   IBAT7U,r7
    mtspr   IBAT7L,r7
    isync

    mtspr   DBAT6U,r7
    mtspr   DBAT6L,r7
    isync

    mtspr   DBAT7U,r7
    mtspr   DBAT7L,r7
    isync

    mfmsr   r3
    ori     r3, r3, 0x30
    mtsrr1  r3

    mflr    r3
    mtsrr0  r3
    rfi
}
#endif

#ifdef __GEKKO__
static asm void ConfigMEM2_112MB(void) {
    nofralloc

    addi    r7, r0, 0

    addis   r4,r0,0x10000002@ha
    addi    r4,r4,0x10000002@l
    addis   r3,r0,0x900007ff@ha
    addi    r3,r3,0x900007ff@l

    addis   r6,r0,0x1000002a@ha
    addi    r6,r6,0x1000002a@l
    addis   r5,r0,0xD0000fff@ha
    addi    r5,r5,0xD0000fff@l

    isync

    mtspr   DBAT4U,r7
    mtspr   DBAT4L,r4
    mtspr   DBAT4U,r3
    isync

    mtspr   IBAT4U,r7
    mtspr   IBAT4L,r7

    isync

    mtspr   DBAT5U,r7
    mtspr   DBAT5L,r6
    mtspr   DBAT5U,r5
    isync

    mtspr   IBAT5U,r7
    mtspr   IBAT5L,r7
    isync

    addis   r4,r0,0x14000002@ha
    addi    r4,r4,0x14000002@l
    addis   r3,r0,0x940003ff@ha
    addi    r3,r3,0x940003ff@l

    addis   r6,r0,0x16000002@ha
    addi    r6,r6,0x16000002@l
    addis   r5,r0,0x960001ff@ha
    addi    r5,r5,0x960001ff@l

    isync

    mtspr   DBAT6U,r7
    mtspr   DBAT6L,r4
    mtspr   DBAT6U,r3
    isync

    mtspr   IBAT6U,r7
    mtspr   IBAT6L,r7
    isync

    mtspr   DBAT7U,r7
    mtspr   DBAT7L,r6
    mtspr   DBAT7U,r5
    isync

    mtspr   IBAT7U,r7
    mtspr   IBAT7L,r7

    isync

    mfmsr   r3
    ori     r3, r3, 0x30
    mtsrr1  r3

    mflr    r3
    mtsrr0  r3
    rfi
}
#endif

#ifdef __GEKKO__
static asm void ConfigMEM2_128MB(void) {
    nofralloc

    addi    r7, r0, 0

    addis   r4,r0,0x10000002@ha
    addi    r4,r4,0x10000002@l
    addis   r3,r0,0x90000fff@ha
    addi    r3,r3,0x90000fff@l

    addis   r6,r0,0x1000002a@ha
    addi    r6,r6,0x1000002a@l
    addis   r5,r0,0xD0000fff@ha
    addi    r5,r5,0xD0000fff@l

    isync

    mtspr   DBAT4U,r7
    mtspr   DBAT4L,r4
    mtspr   DBAT4U,r3
    isync

    mtspr   IBAT4U,r7
    mtspr   IBAT4L,r7

    isync

    mtspr   DBAT5U,r7
    mtspr   DBAT5L,r6
    mtspr   DBAT5U,r5
    isync

    mtspr   IBAT5U,r7
    mtspr   IBAT5L,r7
    isync

    mtspr   IBAT6U,r7
    mtspr   IBAT6L,r7
    isync

    mtspr   IBAT7U,r7
    mtspr   IBAT7L,r7
    isync

    mtspr   DBAT6U,r7
    mtspr   DBAT6L,r7
    isync

    mtspr   DBAT7U,r7
    mtspr   DBAT7L,r7
    isync

    mfmsr   r3
    ori     r3, r3, 0x30
    mtsrr1  r3

    mflr    r3
    mtsrr0  r3
    rfi
}
#endif

#ifdef __GEKKO__
static asm void ConfigMEM_ES1_0(void) {
    nofralloc

    addi    r7, r0, 0

    addis   r4,r0,0x00000002@ha
    addi    r4,r4,0x00000002@l
    addis   r3,r0,0x80000fff@ha
    addi    r3,r3,0x80000fff@l

    isync

    mtspr   dbat0u,r7
    mtspr   dbat0l,r4
    mtspr   dbat0u,r3
    isync

    mtspr   ibat0u,r7
    mtspr   ibat0l,r4
    mtspr   ibat0u,r3
    isync

    mfmsr   r3
    ori     r3, r3, 0x30
    mtsrr1  r3

    mflr    r3
    mtsrr0  r3
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

static void BATConfig(u32 param_0) {
    u32 size1, size2, prot;
    size1 = OSGetConsoleSimulatedMem1Size();

    if (size1 < OSGetPhysicalMem1Size() && size1 == 25165824) {
        DCInvalidateRange((void*)0x81800000, 25165824);
        __MEMRegs[0x14] = 2;
    }

    if (size1 <= 25165824) {
        RealMode((u32)ConfigMEM1_24MB);
    }

    size2 = OSGetConsoleSimulatedMem2Size();
    prot = *(u32*)OSPhysicalToCached(0x3120);

    if (size2 <= 67108864) {
        if (prot <= 0x93400000) {
            RealMode((u32)ConfigMEM2_52MB);
        }
        else if (prot <= 0x93800000) {
            RealMode((u32)ConfigMEM2_56MB);
        }
        else {
            RealMode((u32)ConfigMEM2_64MB);
        }
    }
    else if (size2 <= 134217728) {
        if (prot <= 0x97000000) {
            RealMode((u32)ConfigMEM2_112MB);
        }
        else {
            RealMode((u32)ConfigMEM2_128MB);
        }
    }

    while (param_0 != 0xBA2CF) {}
}

void __OSRestoreCodeExecOnMEM1(u32 param_0) {
    RealMode((u32)ConfigMEM1_24MB);
    while (param_0 != 0xBA2CF) {}
}

void __OSInitMemoryProtection(void) {
    static BOOL initialized = FALSE;
#if !DEBUG
    u32 padding[9];
    u32 temp;
#endif
    BOOL enabled;

    if (initialized) {
        return;
    }

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
    OSRegisterShutdownFunction(&ShutdownFunctionInfo);

    BATConfig(0xBA2CF);

    __OSUnmaskInterrupts(OS_INTERRUPTMASK_MEM_ADDRESS);
    initialized = TRUE;
    OSRestoreInterrupts(enabled);
}
