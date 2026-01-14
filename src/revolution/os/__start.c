#include "revolution/revolution.h"
#include "revolution/os/OSHardware.h"
#include "__ppc_eabi_linker.h"

extern void InitMetroTRK();
extern void exit(int);
extern int main(int argc, char* argv[]);
extern void __init_user(void);
extern void InitMetroTRK_BBA(void);
extern void DBInit();

__declspec(section ".init") extern void __check_pad3(void);
__declspec(section ".init") extern void __set_debug_bba(void);
__declspec(section ".init") extern u8 __get_debug_bba(void);
__declspec(section ".init") extern void __start(void);
__declspec(section ".init") extern void __init_registers(void);
__declspec(section ".init") extern void __init_data(void);
__declspec(section ".init") extern void __init_hardware(void);
__declspec(section ".init") extern void __flush_cache(void* addr, u32 size);

extern u8 Debug_BBA;

__declspec(section ".init") void __check_pad3(void) {
    if ((*(u16*)0x800030E4 & 0xEEF) == 0xEEF) {
        OSResetSystem(0, 0, 0);
    }
}

__declspec(section ".init") void __set_debug_bba(void) { Debug_BBA = 1; }

__declspec(section ".init") u8 __get_debug_bba(void) { return Debug_BBA; }

__declspec(section ".init") __declspec(weak) asm void __start(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc

    // Setup hardware
    bl __init_registers
    bl __init_hardware

    // Create first stack frame
    li r0, -1
    // Parameter save
    stwu r1, -8(r1)
    // LR save (-1 to signify first frame)
    stw r0, 4(r1)
    // Back chain (-1 to signify first frame)
    stw r0, 0(r1)

    // Setup ROM/BSS
    bl __init_data

    // Clear debugger exception mask
    li r0, 0
    lis r6, (OS_DEBUG_INTERFACE + OSDebugInterface.exceptionMask)@ha
    addi r6, r6, (OS_DEBUG_INTERFACE + OSDebugInterface.exceptionMask)@l
    stw r0, 0(r6)

    /**
     * Check the BI2 debug flag to determine how to call InitMetroTRK.
     *
     * This can be done by either checking the value from the DVD
     * (OSBI2.debugFlag), or by checking the global OS_BI2_DEBUG_FLAG.
     *
     * If the DVD BI2 is available, it is prioritized over the global value.
     */
    lis r6, OS_DVD_BI2@ha
    addi r6, r6, OS_DVD_BI2@l
    lwz r6, 0(r6)
    cmplwi r6, 0
    beq _no_dvd_bi2 // <- NULL BI2, try the OS global

    // Use the DVD's flag
    lwz r7, OSBI2.debugFlag(r6)
    b _handle_bi2_debug_flag

    /**
     * At this point, we do one last check to decide whether we want to
     * setup the TRK debugger.
     *
     * If the OS boot info specifies an arena hi, we grab the BI2 debug
     * flag using the global OS_BI2_DEBUG_FLAG.
     *
     * (This must be some heuristic, but I don't understand it)
     */
_no_dvd_bi2:
    lis r5, (OS_BOOT_INFO + OSBootInfo.arenaHi)@ha
    addi r5, r5, (OS_BOOT_INFO + OSBootInfo.arenaHi)@l
    lwz r5, 0(r5)
    cmplwi r5, 0
    beq _check_for_exec_args // <- NULL arena hi, don't setup the debugger

    // Grab the global BI2 debug flag
    lis r7, OS_BI2_DEBUG_FLAG@ha
    addi r7, r7, OS_BI2_DEBUG_FLAG@l
    lwz r7, 0(r7)

    /**
     * The BI2 debug flag/level decides how to set up TRK.
     *
     * Most importantly, it decides how InitMetroTRKCommTable is called, by
     * specifying a type of hardware.
     *
     * ID 1: NDEV hardware
     * ID 2: BBA hardware
     * Anything else: "Unknown" to TRK, defaults to GDEV hardware
     *
     * BI2 debug level maps to TRK comm hardware ID as follows:
     * - BI2 Level 2 -> ID 0 (GDEV)
     * - BI2 Level 3 -> ID 1 (NDEV)
     * - BI2 Level 4 -> ID 2 (BBA)
     */
_handle_bi2_debug_flag:
    // BI2 Debug Level 3: Init TRK as GDEV hardware
    li r5, 0
    cmplwi r7, 2
    beq _call_init_metro_trk

    // BI2 Debug Level 2: Init TRK as NDEV hardware
    cmplwi r7, 3
    li r5, 1
    beq _call_init_metro_trk

    // BI2 Debug Level 4: Init TRK as BBA hardware
    // *Any other debug level is ignored*
    cmplwi r7, 4
    bne _check_for_exec_args // <- Ignore debug level, goto next step
    /**
     * I think at one point this used to call InitMetroTRK as they set
     * r5 here; however, in this version it goes unused.
     *
     * What currently happens is __set_debug_bba sets Debug_BBA, which
     * will result in a call to InitMetroTRK_BBA after the OS is
     * initialized.
     *
     * Maybe this is a leftover from how things worked before
     * InitMetroTRK_BBA was written?
     */
    li r5, 2
    bl __set_debug_bba
    b _check_for_exec_args

    /**
     * Call InitMetroTRK
     * The MetroTRK hardware ID is specified in r5
     */
_call_init_metro_trk:
    lis r6, InitMetroTRK@ha
    addi r6, r6, InitMetroTRK@l
    mtlr r6
    blrl

    /**
     * After setting up the hardware and the debugger, we next setup
     * the program arguments. This label checks whether any arguments
     * exist.
     *
     * BI2 contains an offset from itself to the argument data, which
     * is formatted as follows:
     *
     * typedef struct BI2Args {
     *     int argc;
     *     union {
     *         char* argument;
     *         u32 offset;
     *     } argv[];
     * } BI2Args;
     */
_check_for_exec_args:
    lis r6, OS_DVD_BI2@ha
    addi r6, r6, OS_DVD_BI2@l
    lwz r5, 0(r6)
    cmplwi r5, 0
    beq+ _no_args // <- No BI2 to get args from

    lwz r6, OSBI2.argumentOfs(r5)
    cmplwi r6, 0
    beq+ _no_args // <- Invalid argument offset in BI2

    // Calculate pointer to argument data
    add r6, r5, r6
    // First integer value is the argument count
    lwz r14, 0(r6)
    cmplwi r14, 0
    beq _no_args // <- Argument count is zero

    // BI2 args + 0x8, used to adjust MEM1 later
    addi r15, r6, 4
    // Move argc to the counter to prepare the loop
    mtctr r14

    /**
     * This loop unpacks the arguments by converting them from offsets
     * to pointers, in-place.
     *
     * The offsets are relative to the start of the BI2, so we just
     * add the offset to the BI2 pointer and write it back.
     */
_unpack_args_loop:
    // Skip over argc
    addi r6, r6, 4
    // Load argument offset
    lwz r7, 0(r6)
    // Convert offset to pointer
    add r7, r7, r5
    // Store pointer
    stw r7, 0(r6)
    bdnz _unpack_args_loop

    /**
     * Both the MEM1 arena hi and the MEM1 limit are adjusted to
     * preserve the BI2 args.
     *
     * They are set to eight bytes into the BI2 arguments,
     * rounded down to the nearest 32 bytes.
     */
    lis r5, (OS_BOOT_INFO + OSBootInfo.arenaHi)@ha
    addi r5, r5, (OS_BOOT_INFO + OSBootInfo.arenaHi)@l
    clrrwi r7, r15, 5
    stw r7, 0(r5)

    lis r5, OS_USABLE_MEM1_END@ha
    addi r5, r5, OS_USABLE_MEM1_END@l
    clrrwi r7, r15, 5
    stw r7, 0(r5)
    b _init_os

    /**
     * Either there was no BI2 available, or it did not contain any arguments
     */
_no_args:
    li r14, 0 // argc = 0
    li r15, 0 // argv = NULL

    /**
     * Here, the OS and its debug monitor are initialized, and
     * then we check if we should call __check_pad3.
     *
     * __check_pad3 is called before future initialization if the disk
     * drive device code is 0x0001, or if the OS' inquiry fails (emulation
     * or some debug hardware?)
     *
     * The apploader reads the button state of the fourth GCN controller
     * and writes it to GC_PAD_3_BTN (zero-indexed), which is used in
     * __check_pad3. WiiBrew states that this is for GameCube NR disc
     * support, so that could explain the unusual DVD device code address.
     */
_init_os:
    // Initialize the OS and its debug monitor
    bl DBInit
    bl OSInit

    // Load DVD device code address
    lis r4, OS_DVD_DEVICE_CODE@ha
    addi r4, r4, OS_DVD_DEVICE_CODE@l
    lhz r3, 0(r4)
    // Check whether OS inquiry failed
    andi. r5, r3, DVD_DEVICE_CODE_READ
    beq _call_check_pad3 // <- Bit 0 is NOT set (fail)
    // 0x0001 may be a real ID or a failsafe (see OS.c:InquiryCallback)
    andi. r3, r3, (~DVD_DEVICE_CODE_READ) & 0xFFFF
    cmplwi r3, 0x0001
    bne _check_debug_bba // <- NOT 0x0001 device code address
_call_check_pad3:
    bl __check_pad3

    /**
     * If the BI2 debug level from earlier was set to four, we need to
     * initialize the debugger for BBA hardware.
     */
_check_debug_bba:
    bl __get_debug_bba
    cmplwi r3, 1
    bne _after_init_metro_trk_bba // <- Debug_BBA == false
    bl InitMetroTRK_BBA

    /**
     * 1. Initialize C++ runtime
     * 2. Call main(argc, argv)
     * 3. Teardown C++ runtime
     */
_after_init_metro_trk_bba:
    bl __init_user
    mr r3, r14
    mr r4, r15
    bl main
    b exit // <- Will halt CPU
#endif // clang-format on
}

static inline void __copy_rom_section(void* dst, const void* src, size_t size) {
    if (size == 0 || dst == src) {
        return;
    }

    memcpy(dst, src, size);
    __flush_cache(dst, size);
}

static inline void __init_bss_section(void* dst, size_t size) {
    if (size == 0) {
        return;
    }

    memset(dst, 0, size);
}

__declspec(section ".init") static asm void __init_registers(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc

    li r0, 0
    li r3, 0
    li r4, 0
    li r5, 0
    li r6, 0
    li r7, 0
    li r8, 0
    li r9, 0
    li r10, 0
    li r11, 0
    li r12, 0
    li r14, 0
    li r15, 0
    li r16, 0
    li r17, 0
    li r18, 0
    li r19, 0
    li r20, 0
    li r21, 0
    li r22, 0
    li r23, 0
    li r24, 0
    li r25, 0
    li r26, 0
    li r27, 0
    li r28, 0
    li r29, 0
    li r30, 0
    li r31, 0

    lis r1, _stack_addr@h
    ori r1, r1, _stack_addr@l
    lis r2, _SDA2_BASE_@h
    ori r2, r2, _SDA2_BASE_@l
    lis r13, _SDA_BASE_@h
    ori r13, r13, _SDA_BASE_@l

    blr
#endif // clang-format on
}

__declspec(section ".init") static void __init_data(void) {
    const __rom_copy_info* rs;
    const __bss_init_info* bs;

    rs = _rom_copy_info;
    while (1) {
        if (rs->size == 0) {
            break;
        }

        __copy_rom_section(rs->addr, rs->rom, rs->size);
        rs++;
    }

    bs = _bss_init_info;
    while (1) {
        if (bs->size == 0) {
            break;
        }

        __init_bss_section(bs->addr, bs->size);
        bs++;
    }
}
