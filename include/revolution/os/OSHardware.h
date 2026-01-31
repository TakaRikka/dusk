/**
 * For more details, see:
 * https://www.gc-forever.com/yagcd/chap4.html#sec4
 * https://www.gc-forever.com/yagcd/chap13.html#sec13
 * https://wiibrew.org/wiki/Memory_map
 */

#ifndef _RVL_SDK_OS_HARDWARE_H
#define _RVL_SDK_OS_HARDWARE_H

#include <revolution/dvd.h>
#include <revolution/os.h>
#include <revolution/os/OSContext.h>
#include <revolution/os/OSThread.h>
#include <revolution/types.h>

#ifdef __cplusplus
extern "C" {
#endif

// Forward declarations
typedef struct OSExecParams;

// Derive offsets for use with OSAddress functions
#define __DEF_ADDR_OFFSETS(name, addr)                     \
    static const u32 OS_PHYS_##name = (addr) - 0x80000000; \
    static const u32 OS_CACHED_##name = (addr);            \
    static const u32 OS_UNCACHED_##name = (addr) + (0xC0000000 - 0x80000000);

// Define a global variable in *CACHED* MEM1.
// Can be accessed directly or with OSAddress functions.
#define OS_DEF_GLOBAL_VAR(type, name, addr)     \
    /* Memory-mapped value for direct access */ \
    type OS_##name AT_ADDRESS(addr);            \
    __DEF_ADDR_OFFSETS(name, addr)

// Define a global array in *CACHED* MEM1.
// Can be accessed directly or with OSAddress functions.
#define OS_DEF_GLOBAL_ARR(type, name, arr, addr) \
    /* Memory-mapped value for direct access */  \
    type OS_##name arr AT_ADDRESS(addr);         \
    __DEF_ADDR_OFFSETS(name, addr)

// Define an global variable in the hardware-register range.
#define OS_DEF_HW_REG(type, name, addr)         \
    /* Memory-mapped value for direct access */ \
    type OS_##name AT_ADDRESS(addr);

typedef enum {
    OS_BOOT_MAGIC_BOOTROM = 0xD15EA5E,
    OS_BOOT_MAGIC_JTAG = 0xE5207C22,
} OSBootMagic;

typedef struct OSDebugInterface {
    /* 0x0 */ int usingDebugger;
    /* 0x4 */ u32 exceptionMask;
    /* 0x8 */ void* exceptionHook;
    /* 0xC */ void* exceptionHookLR;
} OSDebugInterface;

typedef struct OSBI2 {
    /* 0x0 */ u32 dbgMonitorSize;
    /* 0x4 */ u32 simulatedMemSize;
    /* 0x8 */ u32 argumentOfs;
    /* 0xC */ u32 debugFlag;
    /* 0x10 */ u32 trackLocation;
    /* 0x14 */ u32 trackSize;
    /* 0x18 */ u32 countryCode;
    u32 WORD_0x1C;
    u32 lastInsert;
    /* 0x24 */ u32 padSpec;
    /* 0x28 */ u32 totalTextDataLimit;
    /* 0x2C */ u32 simulatedMem2Size;
} OSBI2;

/**
 * 0x80000000 - 0x80000100
 */
// clang-format off
OS_DEF_GLOBAL_VAR(OSBootInfo, BOOT_INFO,                   0x80000000);
OS_DEF_GLOBAL_VAR(OSDebugInterface, DEBUG_INTERFACE,       0x80000040);
OS_DEF_GLOBAL_ARR(u8, DB_INTEGRATOR_HOOK, [0x24],          0x80000060);
OS_DEF_GLOBAL_VAR(struct OSContext*, CURRENT_CONTEXT_PHYS, 0x800000C0);
OS_DEF_GLOBAL_VAR(u32, PREV_INTR_MASK,                     0x800000C4);
OS_DEF_GLOBAL_VAR(u32, CURRENT_INTR_MASK,                  0x800000C8);
OS_DEF_GLOBAL_VAR(u32, TV_FORMAT,                          0x800000CC);
OS_DEF_GLOBAL_VAR(u32, ARAM_SIZE,                          0x800000D0);
OS_DEF_GLOBAL_VAR(struct OSContext*, CURRENT_CONTEXT,      0x800000D4);
OS_DEF_GLOBAL_VAR(struct OSContext*, CURRENT_FPU_CONTEXT,  0x800000D8);
OS_DEF_GLOBAL_VAR(OSThreadQueue, THREAD_QUEUE,             0x800000DC);
OS_DEF_GLOBAL_VAR(OSThread*, CURRENT_THREAD,               0x800000E4);
OS_DEF_GLOBAL_VAR(u32, DEBUG_MONITOR_SIZE,                 0x800000E8);
OS_DEF_GLOBAL_VAR(void*, DEBUG_MONITOR,                    0x800000EC);
OS_DEF_GLOBAL_VAR(u32, SIMULATED_MEM_SIZE,                 0x800000F0);
OS_DEF_GLOBAL_VAR(OSBI2*, DVD_BI2,                         0x800000F4);
OS_DEF_GLOBAL_VAR(u32, BUS_CLOCK_SPEED,                    0x800000F8);
OS_DEF_GLOBAL_VAR(u32, CPU_CLOCK_SPEED,                    0x800000FC);
// clang-format on

/**
 * 0x80003000 - 0x80003F00
 */
// clang-format off
OS_DEF_GLOBAL_ARR(void*, EXCEPTION_TABLE, [15],          0x80003000);
OS_DEF_GLOBAL_VAR(void*, INTR_HANDLER_TABLE,             0x80003040);
OS_DEF_GLOBAL_VAR(void*, FIRST_REL,                      0x800030C8);
OS_DEF_GLOBAL_VAR(void*, LAST_REL,                       0x800030CC);
OS_DEF_GLOBAL_VAR(void*, REL_NAME_TABLE,                 0x800030D0);
OS_DEF_GLOBAL_VAR(u32, DOL_TOTAL_TEXT_DATA,              0x800030D4);
OS_DEF_GLOBAL_VAR(s64, SYSTEM_TIME,                      0x800030D8);
OS_DEF_GLOBAL_VAR(u8, PAD_FLAGS,                         0x800030E3);
OS_DEF_GLOBAL_VAR(u16, GC_PAD_3_BTN,                     0x800030E4);
OS_DEF_GLOBAL_VAR(volatile u16, DVD_DEVICE_CODE,         0x800030E6);
OS_DEF_GLOBAL_VAR(u8, BI2_DEBUG_FLAG,                    0x800030E8);
OS_DEF_GLOBAL_VAR(u8, PAD_SPEC,                          0x800030E9);
OS_DEF_GLOBAL_VAR(struct OSExecParams*, DOL_EXEC_PARAMS, 0x800030F0);
OS_DEF_GLOBAL_VAR(u32, PHYSICAL_MEM1_SIZE,               0x80003100);
OS_DEF_GLOBAL_VAR(u32, SIMULATED_MEM1_SIZE,              0x80003104);
OS_DEF_GLOBAL_VAR(void*, USABLE_MEM1_START,              0x8000310C);
OS_DEF_GLOBAL_VAR(void*, USABLE_MEM1_END,                0x80003110);
OS_DEF_GLOBAL_VAR(u32, PHYSICAL_MEM2_SIZE,               0x80003118);
OS_DEF_GLOBAL_VAR(u32, SIMULATED_MEM2_SIZE,              0x8000311C);
OS_DEF_GLOBAL_VAR(void*, ACCESSIBLE_MEM2_END,            0x80003120);
OS_DEF_GLOBAL_VAR(void*, USABLE_MEM2_START,              0x80003124);
OS_DEF_GLOBAL_VAR(void*, USABLE_MEM2_END,                0x80003128);
OS_DEF_GLOBAL_VAR(void*, IPC_BUFFER_START,               0x80003130);
OS_DEF_GLOBAL_VAR(void*, IPC_BUFFER_END,                 0x80003134);
OS_DEF_GLOBAL_VAR(u32, HOLLYWOOD_REV,                    0x80003138);
OS_DEF_GLOBAL_VAR(u32, IOS_VERSION,                      0x80003140);
OS_DEF_GLOBAL_VAR(u32, IOS_BUILD_DATE,                   0x80003144);
OS_DEF_GLOBAL_VAR(void*, IOS_HEAP_START,                 0x80003148);
OS_DEF_GLOBAL_VAR(void*, IOS_HEAP_END,                   0x8000314C);
OS_DEF_GLOBAL_VAR(u32, GDDR_VENDOR_CODE,                 0x80003158);
OS_DEF_GLOBAL_VAR(u8, BOOT_PROGRAM_TARGET,               0x8000315C);
OS_DEF_GLOBAL_VAR(u8, APPLOADER_TARGET,                  0x8000315D);
OS_DEF_GLOBAL_VAR(int, MIOS_SHUTDOWN_FLAG,               0x80003164);
OS_DEF_GLOBAL_VAR(u32, CURRENT_APP_NAME,                 0x80003180);
OS_DEF_GLOBAL_VAR(u8, CURRENT_APP_TYPE,                  0x80003184);
OS_DEF_GLOBAL_VAR(u32, MINIMUM_IOS_VERSION,              0x80003188);
OS_DEF_GLOBAL_VAR(u32, NAND_TITLE_LAUNCH_CODE,           0x8000318C);
OS_DEF_GLOBAL_VAR(u32, NAND_TITLE_RETURN_CODE,           0x80003190);
OS_DEF_GLOBAL_VAR(u32, BOOT_PARTITION_TYPE,              0x80003194);
OS_DEF_GLOBAL_VAR(u32, BOOT_PARTITION_OFFSET,            0x80003198);
OS_DEF_GLOBAL_ARR(u8, NWC24_USER_ID_BUFFER, [32],        0x800031C0);
OS_DEF_GLOBAL_VAR(u64, NWC24_USER_ID,                    0x800031C0);
OS_DEF_GLOBAL_ARR(u8, SC_PRDINFO, [0x100],               0x80003800);
// clang-format on

/**
 * PI hardware globals
 */
volatile u32 PI_HW_REGS[] AT_ADDRESS(0xCC003000);
typedef enum {
    PI_INTSR, //!< 0xCC003000
    PI_INTMR, //!< 0xCC003004
    PI_REG_0x8, //!< 0xCC003008
    PI_REG_0xC, //!< 0xCC00300C
    PI_REG_0x10, //!< 0xCC003010
    PI_REG_0x14, //!< 0xCC003014
    PI_REG_0x18, //!< 0xCC003018
    PI_REG_0x1C, //!< 0xCC00301C
    PI_REG_0x20, //!< 0xCC003020
    PI_RESET, //!< 0xCC003024
    // . . .
} PIHwReg;

// INTSR - Interrupt Cause Register
#define PI_INTSR_ERROR (1 << 0)
#define PI_INTSR_RSW (1 << 1)
#define PI_INTSR_DI (1 << 2)
#define PI_INTSR_SI (1 << 3)
#define PI_INTSR_EXI (1 << 4)
#define PI_INTSR_AI (1 << 5)
#define PI_INTSR_DSP (1 << 6)
#define PI_INTSR_MEM (1 << 7)
#define PI_INTSR_VI (1 << 8)
#define PI_INTSR_PE_TOKEN (1 << 9)
#define PI_INTSR_PE_FINISH (1 << 10)
#define PI_INTSR_CP (1 << 11)
#define PI_INTSR_DEBUG (1 << 12)
#define PI_INTSR_HSP (1 << 13)
#define PI_INTSR_ACR (1 << 14)
#define PI_INTSR_RSWST (1 << 16)

// INTMR - Interrupt Mask Register
#define PI_INTMR_ERROR (1 << 0)
#define PI_INTMR_RSW (1 << 1)
#define PI_INTMR_DI (1 << 2)
#define PI_INTMR_SI (1 << 3)
#define PI_INTMR_EXI (1 << 4)
#define PI_INTMR_AI (1 << 5)
#define PI_INTMR_DSP (1 << 6)
#define PI_INTMR_MEM (1 << 7)
#define PI_INTMR_VI (1 << 8)
#define PI_INTMR_PE_TOKEN (1 << 9)
#define PI_INTMR_PE_FINISH (1 << 10)
#define PI_INTMR_CP (1 << 11)
#define PI_INTMR_DEBUG (1 << 12)
#define PI_INTMR_HSP (1 << 13)
#define PI_INTMR_ACR (1 << 14)

/**
 * MI hardware registers
 */
volatile u16 MI_HW_REGS[] AT_ADDRESS(0xCC004000);
typedef enum {
    MI_PAGE_MEM0_H, //!< 0xCC004000
    MI_PAGE_MEM0_L, //!< 0xCC004002
    MI_PAGE_MEM1_H, //!< 0xCC004004
    MI_PAGE_MEM1_L, //!< 0xCC004006
    MI_PAGE_MEM2_H, //!< 0xCC004008
    MI_PAGE_MEM2_L, //!< 0xCC00400A
    MI_PAGE_MEM3_H, //!< 0xCC00400C
    MI_PAGE_MEM3_L, //!< 0xCC00400E
    MI_PROT_MEM0, //!< 0xCC004010
    MI_PROT_MEM1, //!< 0xCC004012
    MI_PROT_MEM2, //!< 0xCC004014
    MI_PROT_MEM3, //!< 0xCC004016
    MI_REG_0x18, //!< 0xCC004018
    MI_REG_0x1A, //!< 0xCC00401A
    MI_INTMR, //!< 0xCC00401C
    MI_INTSR, //!< 0xCC00401E
    MI_REG_0x20, //!< 0xCC004020
    MI_ADDRLO, //!< 0xCC004022
    MI_ADDRHI, //!< 0xCC004024
    MI_REG_0x26, //!< 0xCC004026
    MI_REG_0x28, //!< 0xCC004028
    // . . .
} MIHwReg;

// INTMR - Interrupt Mask Register
#define MI_INTMR_MEM0 (1 << 0)
#define MI_INTMR_MEM1 (1 << 1)
#define MI_INTMR_MEM2 (1 << 2)
#define MI_INTMR_MEM3 (1 << 3)
#define MI_INTMR_ADDR (1 << 4)

// INTSR - Interrupt Cause Register
#define MI_INTSR_MEM0 (1 << 0)
#define MI_INTSR_MEM1 (1 << 1)
#define MI_INTSR_MEM2 (1 << 2)
#define MI_INTSR_MEM3 (1 << 3)
#define MI_INTSR_ADDR (1 << 4)

/**
 * DI hardware globals
 */
// clang-format off
OS_DEF_HW_REG(volatile unsigned long, DI_DMA_ADDR, 0xCD006014);
OS_DEF_HW_REG(volatile unsigned long, DI_CONFIG,   0xCD006024);
// clang-format on

/**
 * Misc/unknown globals
 */
// clang-format off
OS_DEF_HW_REG(volatile unsigned long, UNK_CD000034, 0xCD000034);
OS_DEF_HW_REG(volatile unsigned long, UNK_CD800180, 0xCD800180);
OS_DEF_HW_REG(volatile unsigned long, UNK_CD8001CC, 0xCD8001CC);
OS_DEF_HW_REG(volatile unsigned long, UNK_CD8001D0, 0xCD8001D0);
// clang-format on

#ifdef __cplusplus
}
#endif

#endif
