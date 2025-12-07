#include <dolphin.h>
#include <dolphin/ar.h>
#include "fake_tgmath.h"

#include "__ar.h"

#if DEBUG
const char* __ARVersion = "<< Dolphin SDK - AR\tdebug build: Apr  5 2004 03:56:19 (0x2301) >>";
#else
const char* __ARVersion = "<< Dolphin SDK - AR\trelease build: Apr  5 2004 04:15:03 (0x2301) >>";
#endif

static void (*__AR_Callback)();
static u32 __AR_Size;
static u32 __AR_InternalSize;
static u32 __AR_ExpansionSize;
static u32 __AR_StackPointer;
static u32 __AR_FreeBlocks;
static u32*  __AR_BlockLength;
static BOOL __AR_init_flag;

// prototypes
static void __ARHandler(__OSInterrupt exception, OSContext* context);
static void __ARWaitForDMA(void);
static void __ARWriteDMA(u32 mmem_addr, u32 aram_addr, u32 length);
static void __ARReadDMA(u32 mmem_addr, u32 aram_addr, u32 length);
static void __ARChecksize(void);
static void __ARClearArea(u32 start_addr, u32 length);

ARQCallback ARRegisterDMACallback(ARQCallback callback) {
    ARQCallback old_callback;
    BOOL old;

    old_callback = __AR_Callback;
    old = OSDisableInterrupts();
    __AR_Callback = callback;
    OSRestoreInterrupts(old);
    return old_callback;
}

u32 ARGetDMAStatus(void) {
    BOOL old;
    u32 val;
    
    old = OSDisableInterrupts();
    val = __DSPRegs[5] & 0x200;
    OSRestoreInterrupts(old);
    return val;
}

void ARStartDMA(u32 type, u32 mainmem_addr, u32 aram_addr, u32 length) {
    BOOL old;

    old = OSDisableInterrupts();
    ASSERTMSGLINE(376, !(__DSPRegs[5] & 0x200), "ARAM DMA already in progress\n");
    ASSERTMSGLINE(377, !(mainmem_addr & 0x1F), "AR: Main memory address is not a multiple of 32 bytes!\n");
    ASSERTMSGLINE(378, !(length & 0x1F), "AR: DMA transfer length is not a multiple of 32 bytes!\n");
    __DSPRegs[16] = (__DSPRegs[16] & 0xFFFFFC00 | (mainmem_addr >> 0x10)); 
    __DSPRegs[17] = (__DSPRegs[17] & 0xFFFF001F | ((u16)mainmem_addr));
    __DSPRegs[18] = (__DSPRegs[18] & 0xFFFFFC00 | (aram_addr >> 0x10));
    __DSPRegs[19] = (__DSPRegs[19] & 0xFFFF001F | ((u16)aram_addr));
    __DSPRegs[20] = __DSPRegs[20] & ~0x8000 | ((type << 0xF) & ~0x7FFF);
    __DSPRegs[20] = (__DSPRegs[20] & 0xFFFFFC00) | (length >> 0x10);
    __DSPRegs[21] = (__DSPRegs[21] & 0xFFFF001F) | (length & 0x0000FFFF);
    OSRestoreInterrupts(old);
}

u32 ARAlloc(u32 length) {
    u32 tmp;
    BOOL old;

    old = OSDisableInterrupts();
    ASSERTMSGLINE(430, !(length & 0x1F), "ARAlloc(): length is not multiple of 32bytes!");
    ASSERTMSGLINE(434, length <= (__AR_Size - __AR_StackPointer), "ARAlloc(): Out of ARAM!");
    ASSERTMSGLINE(435, __AR_FreeBlocks, "ARAlloc(): No more free blocks!");

    tmp = __AR_StackPointer;
    __AR_StackPointer += length;
    *__AR_BlockLength = length;
    __AR_BlockLength += 1;
    __AR_FreeBlocks -= 1;
    OSRestoreInterrupts(old);
    return tmp;
}

u32 ARFree(u32* length) {
    BOOL old;

    old = OSDisableInterrupts();
    __AR_BlockLength -= 1;
    if (length) {
        *length = *__AR_BlockLength;
    }
    __AR_StackPointer -= *__AR_BlockLength;
    __AR_FreeBlocks += 1;
    OSRestoreInterrupts(old);
    return __AR_StackPointer;
}

BOOL ARCheckInit(void) {
    return __AR_init_flag;
}

u32 ARInit(u32* stack_index_addr, u32 num_entries) {
    BOOL old;
    u16 refresh;

    if (__AR_init_flag == TRUE) {
        return 0x4000;
    }

    OSRegisterVersion(__ARVersion);

    old = OSDisableInterrupts();
    __AR_Callback = NULL;
    __OSSetInterruptHandler(6, __ARHandler);
    __OSUnmaskInterrupts(0x02000000);
    __AR_StackPointer = 0x4000;
    __AR_FreeBlocks = num_entries;
    __AR_BlockLength = stack_index_addr;
    refresh = __DSPRegs[13] & 0xFF;

    ASSERTMSGLINE(590, (refresh <= 196.0f), "ARInit(): ILLEGAL SDRAM REFRESH VALUE\n");
    __DSPRegs[13] = (u16)((__DSPRegs[13] & ~0xFF) | (refresh & 0xFF));

    __ARChecksize();
    __AR_init_flag = TRUE;
    OSRestoreInterrupts(old);
    return __AR_StackPointer;
}

void ARReset(void) {
    __AR_init_flag = FALSE;
}

void ARSetSize(void) {
#if DEBUG
    OSReport("ARSetSize(): I don't do anything anymore!\n");
#endif
}

u32 ARGetBaseAddress(void) {
    return 0x4000;
}

u32 ARGetSize(void) {
    return __AR_Size;
}

u32 ARGetInternalSize(void) {
    return __AR_InternalSize;
}

void ARClear(u32 flag) {
    switch (flag) {
    case 0:
        if (__AR_InternalSize != 0) {
            __ARClearArea(0, __AR_InternalSize);
        }
        return;
    case 1:
        if (__AR_InternalSize != 0) {
            __ARClearArea(0x4000, __AR_InternalSize - 0x4000);
        }
        break;
    case 2:
        if (__AR_InternalSize != 0 && __AR_ExpansionSize != 0) {
            __ARClearArea(__AR_InternalSize, __AR_ExpansionSize);
        }
        break;
    default:
        ASSERTMSGLINE(774, 0, "ARClear(): Unknown flag.\n");
        break;
    }
}

static void __ARHandler(__OSInterrupt exception, OSContext* context) {
    OSContext exceptionContext;
    u16 tmp;

    tmp = __DSPRegs[5];
    tmp = (tmp & ~0x88) | 0x20;
    __DSPRegs[5] = (tmp);
    OSClearContext(&exceptionContext);
    OSSetCurrentContext(&exceptionContext);
    if (__AR_Callback) {
        __AR_Callback();
    }
    OSClearContext(&exceptionContext);
    OSSetCurrentContext(context);
}

void __ARClearInterrupt(void) {
    u16 tmp;

    tmp = __DSPRegs[5];
    tmp = (tmp & ~0x88) | 0x20;
    __DSPRegs[5] = (tmp);
}

u16 __ARGetInterruptStatus(void) {
    return __DSPRegs[5] & 0x20;
}

static void __ARWaitForDMA(void) {
    while (__DSPRegs[5] & 0x200);
}

static void __ARWriteDMA(u32 mmem_addr, u32 aram_addr, u32 length) {
	// Main mem address
	__DSPRegs[DSP_ARAM_DMA_MM_HI] = (u16)((__DSPRegs[DSP_ARAM_DMA_MM_HI] & ~0x03ff) | (u16)(mmem_addr >> 16));
	__DSPRegs[DSP_ARAM_DMA_MM_LO] = (u16)((__DSPRegs[DSP_ARAM_DMA_MM_LO] & ~0xffe0) | (u16)(mmem_addr & 0xffff));

	// ARAM address
	__DSPRegs[DSP_ARAM_DMA_ARAM_HI] = (u16)((__DSPRegs[DSP_ARAM_DMA_ARAM_HI] & ~0x03ff) | (u16)(aram_addr >> 16));
	__DSPRegs[DSP_ARAM_DMA_ARAM_LO] = (u16)((__DSPRegs[DSP_ARAM_DMA_ARAM_LO] & ~0xffe0) | (u16)(aram_addr & 0xffff));

	// DMA buffer size
	__DSPRegs[DSP_ARAM_DMA_SIZE_HI] = (u16)(__DSPRegs[DSP_ARAM_DMA_SIZE_HI] & ~0x8000);

	__DSPRegs[DSP_ARAM_DMA_SIZE_HI] = (u16)((__DSPRegs[DSP_ARAM_DMA_SIZE_HI] & ~0x03ff) | (u16)(length >> 16));
	__DSPRegs[DSP_ARAM_DMA_SIZE_LO] = (u16)((__DSPRegs[DSP_ARAM_DMA_SIZE_LO] & ~0xffe0) | (u16)(length & 0xffff));

	__ARWaitForDMA();
    __ARClearInterrupt();
}

static void __ARReadDMA(u32 mmem_addr, u32 aram_addr, u32 length) {
	// Main mem address
	__DSPRegs[DSP_ARAM_DMA_MM_HI] = (u16)((__DSPRegs[DSP_ARAM_DMA_MM_HI] & ~0x03ff) | (u16)(mmem_addr >> 16));
	__DSPRegs[DSP_ARAM_DMA_MM_LO] = (u16)((__DSPRegs[DSP_ARAM_DMA_MM_LO] & ~0xffe0) | (u16)(mmem_addr & 0xffff));

	// ARAM address
	__DSPRegs[DSP_ARAM_DMA_ARAM_HI] = (u16)((__DSPRegs[DSP_ARAM_DMA_ARAM_HI] & ~0x03ff) | (u16)(aram_addr >> 16));
	__DSPRegs[DSP_ARAM_DMA_ARAM_LO] = (u16)((__DSPRegs[DSP_ARAM_DMA_ARAM_LO] & ~0xffe0) | (u16)(aram_addr & 0xffff));

	// DMA buffer size
	__DSPRegs[DSP_ARAM_DMA_SIZE_HI] = (u16)(__DSPRegs[DSP_ARAM_DMA_SIZE_HI] | 0x8000);

	__DSPRegs[DSP_ARAM_DMA_SIZE_HI] = (u16)((__DSPRegs[DSP_ARAM_DMA_SIZE_HI] & ~0x03ff) | (u16)(length >> 16));
	__DSPRegs[DSP_ARAM_DMA_SIZE_LO] = (u16)((__DSPRegs[DSP_ARAM_DMA_SIZE_LO] & ~0xffe0) | (u16)(length & 0xffff));

	__ARWaitForDMA();
    __ARClearInterrupt();
}

static void __ARChecksize(void) {
    u8 test_data_pad[63];
    u8 dummy_data_pad[63];
    u8 buffer_pad[63];
    u8 save_pad_1[63];
    u8 save_pad_2[63];
    u8 save_pad_3[63];
    u8 save_pad_4[63];
    u8 save_pad_5[63];
    u32* test_data;
    u32* dummy_data;
    u32* buffer;
    u32* save1;
    u32* save2;
    u32* save3;
    u32* save4;
    u32* save5;
    u16 ARAM_mode = 0;
    u32 ARAM_size = 0;
    u32 i;

    do {} while(!(__DSPRegs[11] & 1));

    ARAM_mode = 3;
    ARAM_size = __AR_InternalSize = 0x1000000;
    __DSPRegs[9] = ((__DSPRegs[9] & 0xFFFFFFC0) | 3) | 0x20;

    test_data = (u32*)(OSRoundUp32B((u32)(test_data_pad)));
    dummy_data = (u32*)(OSRoundUp32B((u32)(dummy_data_pad)));
    buffer = (u32*)(OSRoundUp32B((u32)(buffer_pad)));

    save1 = (u32*)(OSRoundUp32B((u32)(save_pad_1)));
    save2 = (u32*)(OSRoundUp32B((u32)(save_pad_2)));
    save3 = (u32*)(OSRoundUp32B((u32)(save_pad_3)));
    save4 = (u32*)(OSRoundUp32B((u32)(save_pad_4)));
    save5 = (u32*)(OSRoundUp32B((u32)(save_pad_5)));

    for (i = 0; i < 8; i++) {
        *(test_data + i) = 0xDEADBEEF;
        *(dummy_data + i) = 0xBAD0BAD0;
    }

    DCFlushRange((void*)test_data, 0x20);
    DCFlushRange((void*)dummy_data, 0x20);

    __AR_ExpansionSize = 0;

    DCInvalidateRange((void*)save1, 0x20);
    __ARReadDMA((u32)save1, ARAM_size + 0, 0x20);
    PPCSync();

    __ARWriteDMA((u32)test_data, ARAM_size + 0x0000000, 0x20);

    memset((void*)buffer, 0, 0x20);
    DCFlushRange((void*)buffer, 0x20);

    __ARReadDMA((u32)buffer, ARAM_size + 0x0000000, 0x20);
    PPCSync();

    if (buffer[0] == test_data[0]) {
        DCInvalidateRange((void*)save2, 0x20);
        __ARReadDMA((u32)save2, ARAM_size + 0x0200000, 0x20);
        PPCSync();

        DCInvalidateRange((void*)save3, 0x20);
        __ARReadDMA((u32)save3, ARAM_size + 0x1000000, 0x20);
        PPCSync();

        DCInvalidateRange((void*)save4, 0x20);
        __ARReadDMA((u32)save4, ARAM_size + 0x0000200, 0x20);
        PPCSync();

        DCInvalidateRange((void*)save5, 0x20);
        __ARReadDMA((u32)save5, ARAM_size + 0x0400000, 0x20);
        PPCSync();

        __ARWriteDMA((u32)dummy_data, ARAM_size + 0x0200000, 0x20);
        __ARWriteDMA((u32)test_data, ARAM_size + 0x0000000, 0x20);

        memset((void*)buffer, 0, 0x20);
        DCFlushRange((void*)buffer, 0x20);

        __ARReadDMA((u32)buffer, ARAM_size + 0x0200000, 0x20);
        PPCSync();

        if (buffer[0] == test_data[0]) {
            __ARWriteDMA((u32)save1, ARAM_size + 0x0000000, 0x20);

            ARAM_mode |= 0 << 1;
            ARAM_size += 0x0200000;
            __AR_ExpansionSize = 0x0200000;
        } else {
            __ARWriteDMA((u32)dummy_data, ARAM_size + 0x1000000, 0x20);
            __ARWriteDMA((u32)test_data, ARAM_size + 0x0000000, 0x20);

            memset((void*)buffer, 0, 0x20);
            DCFlushRange((void*)buffer, 0x20);

            __ARReadDMA((u32)buffer, ARAM_size + 0x1000000, 0x20);
            PPCSync();

            if (buffer[0] == test_data[0]) {
                __ARWriteDMA((u32)save1, ARAM_size + 0x0000000, 0x20);
                __ARWriteDMA((u32)save2, ARAM_size + 0x0200000, 0x20);

                ARAM_mode |= 4 << 1;
                ARAM_size += 0x0400000;
                __AR_ExpansionSize = 0x0400000;
            } else {
                __ARWriteDMA((u32)dummy_data, ARAM_size + 0x0000200, 0x20);
                __ARWriteDMA((u32)test_data, ARAM_size + 0x0000000, 0x20);

                memset((void*)buffer, 0, 0x20);
                DCFlushRange((void*)buffer, 0x20);

                __ARReadDMA((u32)buffer, ARAM_size + 0x0000200, 0x20);
                PPCSync();

                if (buffer[0] == test_data[0]) {
                    __ARWriteDMA((u32)save1, ARAM_size + 0x0000000, 0x20);
                    __ARWriteDMA((u32)save2, ARAM_size + 0x0200000, 0x20);
                    __ARWriteDMA((u32)save3, ARAM_size + 0x1000000, 0x20);

                    ARAM_mode |= 8 << 1;
                    ARAM_size += 0x0800000;
                    __AR_ExpansionSize = 0x0800000;
                } else {
                    __ARWriteDMA((u32)dummy_data, ARAM_size + 0x0400000, 0x20);

                    __ARWriteDMA((u32)test_data, ARAM_size + 0x0000000, 0x20);

                    memset((void*)buffer, 0, 0x20);
                    DCFlushRange((void*)buffer, 0x20);

                    __ARReadDMA((u32)buffer, ARAM_size + 0x0400000, 0x20);
                    PPCSync();

                    if (buffer[0] == test_data[0]) {
                        __ARWriteDMA((u32)save1, ARAM_size + 0x0000000, 0x20);
                        __ARWriteDMA((u32)save2, ARAM_size + 0x0200000, 0x20);
                        __ARWriteDMA((u32)save3, ARAM_size + 0x1000000, 0x20);
                        __ARWriteDMA((u32)save4, ARAM_size + 0x0000200, 0x20);

                        ARAM_mode |= 12 << 1;
                        ARAM_size += 0x1000000;
                        __AR_ExpansionSize = 0x1000000;
                    } else {
                        __ARWriteDMA((u32)save1, ARAM_size + 0x0000000, 0x20);
                        __ARWriteDMA((u32)save2, ARAM_size + 0x0200000, 0x20);
                        __ARWriteDMA((u32)save3, ARAM_size + 0x1000000, 0x20);
                        __ARWriteDMA((u32)save4, ARAM_size + 0x0000200, 0x20);
                        __ARWriteDMA((u32)save5, ARAM_size + 0x0400000, 0x20);

                        ARAM_mode |= 16 << 1;
                        ARAM_size += 0x2000000;
                        __AR_ExpansionSize = 0x2000000;
                    }
                }
            }
        }

#if DEBUG
        OSReport("__ARChecksize(): ARAM Expansion present.\n");
#endif
        __DSPRegs[9] = (u16)((__DSPRegs[9] & ~(0x07 | 0x38)) | ARAM_mode);
    }

    *(u32*)OSPhysicalToUncached(0x00D0) = ARAM_size;
    __AR_Size = ARAM_size;
}

static void __ARClearArea(u32 start_addr, u32 length) {
    u8 zero_buffer[2079];
    u8* ptr;
    u32 curr_addr;
    u32 curr_len;
    u32 end_addr;
    u32 remainder;

    ASSERTMSGLINE(0x529, !(start_addr & 0x1F), "__ARClearArea(): Destination address not 32-byte aligned.\n");
    ASSERTMSGLINE(0x52A, !(length & 0x1F), "__ARClearArea(): Length not multiple of 32 bytes.\n");

    ptr = (u8*)(OSRoundUp32B((u32)(zero_buffer)));

    do {} while(!(__DSPRegs[11] & 1));

    memset(ptr, 0, 0x800);
    DCFlushRange(ptr, 0x800);

    curr_addr = start_addr;
    end_addr = start_addr + length;

    while (curr_addr < end_addr) {
        remainder = end_addr - curr_addr;
 
        curr_len = OSRoundUp32B(remainder < 0x800 ? remainder : 0x800);
        __ARWriteDMA((u32)ptr, curr_addr, curr_len);
        curr_addr += curr_len;
    }
}
