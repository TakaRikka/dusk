#ifndef _REVOLUTION_PPCARCH
#define _REVOLUTION_PPCARCH

#include "revolution/types.h"

#ifdef __cplusplus
extern "C" {
#endif

#define CTR     9
#define XER     1
#define LR      8

#define UPMC1   937
#define UPMC2   938
#define UPMC3   941
#define UPMC4   942

#define USIA    939

#define UMMCR0  936
#define UMMCR1  940

#define HID0    1008
#define HID1    1009

#define PVR     287

#define IBAT0U  528
#define IBAT0L  529
#define IBAT1U  530
#define IBAT1L  531
#define IBAT2U  532
#define IBAT2L  533
#define IBAT3U  534
#define IBAT3L  535

#define DBAT0U  536
#define DBAT0L  537
#define DBAT1U  538
#define DBAT1L  539
#define DBAT2U  540
#define DBAT2L  541
#define DBAT3U  542
#define DBAT3L  543

#define SDR1    25

#define SPRG0   272
#define SPRG1   273
#define SPRG2   274
#define SPRG3   275

#define DAR     19
#define DSISR   18

#define SRR0    26
#define SRR1    27

#define EAR     282

#define DABR    1013

#define TBL     284
#define TBU     285

#define L2CR    1017

#define DEC     22

#define IABR    1010

#define PMC1    953
#define PMC2    954
#define PMC3    957
#define PMC4    958

#define SIA     955

#define MMCR0   952
#define MMCR1   956

#define THRM1   1020
#define THRM2   1021
#define THRM3   1022

#define ICTC    1019

#define GQR0    912
#define GQR1    913
#define GQR2    914
#define GQR3    915
#define GQR4    916
#define GQR5    917
#define GQR6    918
#define GQR7    919

#define HID2    920

#define WPAR    921

#define DMA_U   922
#define DMA_L   923

#define MSR_POW         0x00040000  // Power Management
#define MSR_ILE         0x00010000  // Interrupt Little Endian
#define MSR_EE          0x00008000  // external interrupt
#define MSR_PR          0x00004000  // privilege level(should be 0)
#define MSR_FP          0x00002000  // floating point available
#define MSR_ME          0x00001000  // machine check enable
#define MSR_FE0         0x00000800  // floating point exception enable
#define MSR_SE          0x00000400  // single step trace enable
#define MSR_BE          0x00000200  // branch trace enable
#define MSR_FE1         0x00000100  // floating point exception enable
#define MSR_IP          0x00000040  // Exception prefix
#define MSR_IR          0x00000020  // instruction relocate
#define MSR_DR          0x00000010  // data relocate
#define MSR_PM          0x00000004  // Performance monitor marked mode
#define MSR_RI          0x00000002  // Recoverable interrupt
#define MSR_LE          0x00000001  // Little Endian

#define MSR_POW_BIT     13          // Power Management
#define MSR_ILE_BIT     15          // Interrupt Little Endian
#define MSR_EE_BIT      16          // external interrupt
#define MSR_PR_BIT      17          // privilege level (should be 0)
#define MSR_FP_BIT      18          // floating point available
#define MSR_ME_BIT      19          // machine check enable
#define MSR_FE0_BIT     20          // floating point exception enable
#define MSR_SE_BIT      21          // single step trace enable
#define MSR_BE_BIT      22          // branch trace enable
#define MSR_FE1_BIT     23          // floating point exception enable
#define MSR_IP_BIT      25          // Exception prefix
#define MSR_IR_BIT      26          // instruction relocate
#define MSR_DR_BIT      27          // data relocate
#define MSR_PM_BIT      29          // Performance monitor marked mode
#define MSR_RI_BIT      30          // Recoverable interrupt
#define MSR_LE_BIT      31          // Little Endian

/*---------------------------------------------------------------------------*
    HID0 bits
 *---------------------------------------------------------------------------*/
#define HID0_EMCP       0x80000000u  // Enable MCP
#define HID0_DBP        0x40000000u  // Enable 60x bus address and data parity chk
#define HID0_EBA        0x20000000u  // Enable 60x address parity checking
#define HID0_EBD        0x10000000u  // Enable 60x data parity checking
#define HID0_BCLK       0x08000000u  // CLK_OUT output enable and clk selection
#define HID0_ECLK       0x02000000u  // CLK_OUT output enable and clk selection
#define HID0_PAR        0x01000000u  // Disable !ARTRY precharge
#define HID0_DOZE       0x00800000u  // Doze mode enable
#define HID0_NAP        0x00400000u  // Nap mode enable
#define HID0_SLEEP      0x00200000u  // Sleep mode enable
#define HID0_DPM        0x00100000u  // Dynamic power management enable
#define HID0_NHR        0x00010000u  // Not hard reset (0 hard reset if s/w set it)
#define HID0_ICE        0x00008000u  // Instruction cache enable
#define HID0_DCE        0x00004000u  // Data cache enable
#define HID0_ILOCK      0x00002000u  // ICache lock
#define HID0_DLOCK      0x00001000u  // DCache lock
#define HID0_ICFI       0x00000800u  // ICache flash invalidate
#define HID0_DCFI       0x00000400u  // DCache flash invalidate
#define HID0_SPD        0x00000200u  // Speculative cache access enable (0 enable)
#define HID0_IFEM       0x00000100u  // Enable M bit on bus for Ifetch
#define HID0_SGE        0x00000080u  // Store gathering enable
#define HID0_DCFA       0x00000040u  // DCache flush assist - set before a flush
#define HID0_BTIC       0x00000020u  // Branch target icache enable
#define HID0_ABE        0x00000008u  // Address bcast enable
#define HID0_BHT        0x00000004u  // Branch history table enable
#define HID0_NOOPTI     0x00000001u  // No-op Dcache touch instructions

#define HID0_ICE_BIT    16          // Instruction cache enable
#define HID0_DCE_BIT    17          // Data cache enable
#define HID0_ILOCK_BIT  18          // ICache lock
#define HID0_DLOCK_BIT  19          // DCache lock

#define HID2_LSQE           0x80000000  // L/S quantize enable
#define HID2_WPE            0x40000000  // Write pipe enable
#define HID2_PSE            0x20000000  // Paired single enable
#define HID2_LCE            0x10000000  // Locked cache enable

#define HID2_DCHERR         0x00800000  // ERROR: dcbz_l cache hit
#define HID2_DNCERR         0x00400000  // ERROR: DMA access to normal cache
#define HID2_DCMERR         0x00200000  // ERROR: DMA cache miss error
#define HID2_DQOERR         0x00100000  // ERROR: DMA queue overflow
#define HID2_DCHEE          0x00080000  // dcbz_l cache hit error enable
#define HID2_DNCEE          0x00040000  // DMA access to normal cache error enable
#define HID2_DCMEE          0x00020000  // DMA cache miss error error enable
#define HID2_DQOEE          0x00010000  // DMA queue overflow error enable

#define HID2_DMAQL_MASK     0x0F000000  // DMA queue length mask
#define HID2_DMAQL_SHIFT    24          // DMA queue shift

#define HID2_LSQE_BIT       0
#define HID2_WPE_BIT        1
#define HID2_PSE_BIT        2
#define HID2_LCE_BIT        3

#define HID2_DCHERR_BIT     8
#define HID2_DNCERR_BIT     9
#define HID2_DCMERR_BIT     10
#define HID2_DQOERR_BIT     11
#define HID2_DCHEE_BIT      12
#define HID2_DNCEE_BIT      13
#define HID2_DCMEE_BIT      14
#define HID2_DQOEE_BIT      15

#define GQR_LOAD_SCALE_MASK         0x3F000000  // load scale field
#define GQR_LOAD_TYPE_MASK          0x00070000  // load type field
#define GQR_STORE_SCALE_MASK        0x00003F00  // store scale field
#define GQR_STORE_TYPE_MASK         0x00000007  // store type field

typedef struct
{
    u32 _pad0       :2;
    u32 loadScale   :6;
    u32 _pad1       :5;
    u32 loadType    :3;
    u32 _pad2       :2;
    u32 storeScale  :6;
    u32 _pad3       :5;
    u32 storeType   :3;
} PPC_GQR_t;

typedef union
{
    u32         val;
    PPC_GQR_t   f;
} PPC_GQR_u;


#define DMA_U_ADDR_MASK             0xFFFFFFE0  // Start addr in memory
#define DMA_U_LEN_U_MASK            0x0000001F  // lines to transfer (U)

#define DMA_L_LC_ADDR_MASK          0xFFFFFFE0  // Start addr in LC
#define DMA_L_LOAD                  0x00000010  // 0 - store, 1 - load
#define DMA_L_STORE                 0x00000000  // 0 - store, 1 - load
#define DMA_L_LEN_MASK              0x0000000C  // lines to transfer (L)
#define DMA_L_TRIGGER               0x00000002  // 0 - cmd inactive, 1 - cmd rdy
#define DMA_L_FLUSH                 0x00000001  // 1 - Flush DMA queue

typedef struct
{
    u32 memAddr     :27;
    u32 dmaLenU     :5;
} PPC_DMA_U_t;

typedef union
{
    u32             val;
    PPC_DMA_U_t     f;
} PPC_DMA_U_u;


typedef struct
{
    u32 lcAddr      :27;
    u32 dmaLd       :1;
    u32 dmaLenL     :2;
    u32 dmaTrigger  :1;
    u32 dmaFlush    :1;
} PPC_DMA_L_t;


typedef union
{
    u32             val;
    PPC_DMA_L_t     f;
} PPC_DMA_L_u;


#define WPAR_ADDR                   0xFFFFFFE0  // 32byte gather address
#define WPAR_BNE                    0x00000001  // Buffer not empty (R)

#define SRR1_DMA_BIT                0x00200000
#define SRR1_L2DP_BIT               0x00100000

#define L2CR_L2E                    0x80000000  // L2 Enable
#define L2CR_L2PE                   0x40000000  // L2 data parity generation and checking enable

#define L2CR_L2SIZ_256K             0x10000000  // L2 size 256K
#define L2CR_L2SIZ_512K             0x20000000  // L2 size 512
#define L2CR_L2SIZ_1M               0x30000000  // L2 size 1M

#define L2CR_L2CLK_1_0              0x02000000  // L2 clock ratio 1
#define L2CR_L2CLK_1_5              0x04000000  // L2 clock ratio 1.5
#define L2CR_L2CLK_2_0              0x08000000  // L2 clock ratio 2
#define L2CR_L2CLK_2_5              0x0A000000  // L2 clock ratio 2.5
#define L2CR_L2CLK_3_0              0x0C000000  // L2 clock ratio 3

#define L2CR_RAM_FLOW_THRU_BURST    0x00000000  // L2 RAM type flow-through sync. burst SRAM
#define L2CR_RAM_PIPELINE_BURST     0x01000000  // L2 RAM type pipelined sync. burst SRAM
#define L2CR_RAM_PIPELINE_LATE      0x01800000  // L2 RAM type pipelined sync. late-write SRAM

#define L2CR_L2DO                   0x00400000  // Data only
#define L2CR_L2I                    0x00200000  // Global invalidate
#define L2CR_L2CTL                  0x00100000  // ZZ enable
#define L2CR_L2WT                   0x00080000  // L2 write through
#define L2CR_L2TS                   0x00040000  // L2 test support

#define L2CR_L2OH_0_5               0x00000000  // L2 output hold 0.5 ns
#define L2CR_L2OH_1_0               0x00010000  // L2 output hold 1.0 ns

#define L2CR_L2SL                   0x00008000  // L2 DLL slow
#define L2CR_L2DF                   0x00004000  // L2 differential clock
#define L2CR_L2BYP                  0x00002000  // L2 DLL bypass
#define L2CR_L2CS                   0x00000200  // L2 clock stop
#define L2CR_L2DRO                  0x00000100  // L2 DLL rollover checkstop enable
#define L2CR_L2CTR_MASK             0x000000FE  // L2 counter value mask
#define L2CR_L2IP                   0x00000001  // L2 global invalidate in progress

#define MMCR0_DIS                   0x80000000  // Disables counting unconditionally
#define MMCR0_DP                    0x40000000  // Disables counting while in supervisor mode
#define MMCR0_DU                    0x20000000  // Disables counting while in user mode
#define MMCR0_DMS                   0x10000000  // Disables counting while MSR[PM] is set
#define MMCR0_DMR                   0x08000000  // Disables counting while MSR[PM] is zero
#define MMCR0_ENINT                 0x04000000  // Enables performance monitor interrupt signaling
#define MMCR0_DISCOUNT              0x02000000  // Disables counting of PMCn when a performance monitor interrupt is signaled or...
#define MMCR0_RTCSELECT_MASK        0x01800000  // 64-bit time base, bit selection enable
#define MMCR0_RTCSELECT_63          0x00000000  // Pick bit 63 to count
#define MMCR0_RTCSELECT_55          0x00800000  // Pick bit 55 to count
#define MMCR0_RTCSELECT_51          0x01000000  // Pick bit 51 to count
#define MMCR0_RTCSELECT_47          0x01800000  // Pick bit 47 to count
#define MMCR0_INTONBITTRANS         0x00400000  // Causes interrupt signaling on bit transition from off to on
#define MMCR0_THRESHOLD_MASK        0x003F0000  // Threshold value
#define MMCR0_THRESHOLD(n)          ((n) << 16) // Threshold value (0 - 63)
#define MMCR0_PMC1INTCONTROL        0x00008000  // Enables interrupt signaling due to PMC1 counter overflow
#define MMCR0_PMC2INTCONTROL        0x00004000  // Enables interrupt signaling due to PMC2-PMC4 counter overflow
#define MMCR0_PMCTRIGGER            0x00002000  // Can be used to trigger counting of PMC2-PMC4 after PMC1 has overflowed or...
#define MMCR0_PMC1SELECT_MASK       0x00001FC0  // PMC1 input selector
#define MMCR0_PMC2SELECT_MASK       0x0000003F  // PMC2 input selector

#define MMCR1_PMC3SELECT_MASK       0xF8000000  // PMC3 input selector
#define MMCR1_PMC4SELECT_MASK       0x07C00000  // PMC4 input selector

#define PMC1_OV                     0x80000000  // Overflow
#define PMC1_COUNTER                0x7FFFFFFF  // Counter value
#define PMC2_OV                     0x80000000  // Overflow
#define PMC2_COUNTER                0x7FFFFFFF  // Counter value
#define PMC3_OV                     0x80000000  // Overflow
#define PMC3_COUNTER                0x7FFFFFFF  // Counter value
#define PMC4_OV                     0x80000000  // Overflow
#define PMC4_COUNTER                0x7FFFFFFF  // Counter value

/*---------------------------------------------------------------------------*
    PMC1 Events
 *---------------------------------------------------------------------------*/
#define MMCR0_PMC1_HOLD             0x00000000  // Register holds current value
#define MMCR0_PMC1_CYCLE            0x00000040  // Processor cycles
#define MMCR0_PMC1_INSTRUCTION      0x00000080  // # of instructions completed.
#define MMCR0_PMC1_TRANSITION       0x000000C0  // # of transitions for 0 to 1
#define MMCR0_PMC1_DISPATCHED       0x00000100  // # of instructions dispatched
#define MMCR0_PMC1_EIEIO            0x00000140  // # of eieio instructions completed
#define MMCR0_PMC1_ITLB_CYCLE       0x00000180  // # of cycles spent performing table search op. for the ITLB
#define MMCR0_PMC1_L2_HIT           0x000001C0  // # of access that hit the L2.
#define MMCR0_PMC1_EA               0x00000200  // # of valid instruction EAs delivered to the memory subsystem
#define MMCR0_PMC1_IABR             0x00000240  // # of time the address of an instruction matches the IABR
#define MMCR0_PMC1_L1_MISS          0x00000280  // # of loads that miss the L1
#define MMCR0_PMC1_Bx_UNRESOLVED    0x000002C0  // # of branches that are unresolved when processed
#define MMCR0_PMC1_Bx_STALL_CYCLE   0x00000300  // # of cycles that dispatcher stalls due to a second
                                                //      unresolved branch in the instruction stream
#define MMCR0_PMC1_IC_FETCH_MISS    0x00000340  // # of times an instruction fetch missed the L1 Icache
#define MMCR0_PMC2_HOLD             0x00000000  // Register holds current value
#define MMCR0_PMC2_CYCLE            0x00000001  // Processor cycles
#define MMCR0_PMC2_INSTRUCTION      0x00000002  // # of instructions completed
#define MMCR0_PMC2_TRANSITION       0x00000003  // # of time-base (lower) bit transitions
#define MMCR0_PMC2_DISPATCHED       0x00000004  // # of instructions dispatched
#define MMCR0_PMC2_IC_MISS          0x00000005  // # of L1 instruction cache misses
#define MMCR0_PMC2_ITLB_MISS        0x00000006  // # of ITLB misses
#define MMCR0_PMC2_L2_I_MISS        0x00000007  // # of L2 instruction misses
#define MMCR0_PMC2_Bx_FALL_TROUGH   0x00000008  // # of fall-through branches
#define MMCR0_PMC2_PR_SWITCH        0x00000009  // # of MSR[PR] bit toggles
#define MMCR0_PMC2_RESERVED_LOAD    0x0000000A  // # of reserved loads completed
#define MMCR0_PMC2_LOAD_STORE       0x0000000B  // # of completed loads and stores
#define MMCR0_PMC2_SNOOP            0x0000000C  // # of snoops
#define MMCR0_PMC2_L1_CASTOUT       0x0000000D  // # of L1 castouts to L2
#define MMCR0_PMC2_SYSTEM           0x0000000E  // # of completed system unit instructions
#define MMCR0_PMC2_IC_FETCH_MISS    0x0000000F  // # of instruction fetch misses in the L1
#define MMCR0_PMC2_Bx_OUT_OF_ORDER  0x00000010  // # of branches allowing out-of-order execution

/*---------------------------------------------------------------------------*
    PMC3 Events
 *---------------------------------------------------------------------------*/
#define MMCR1_PMC3_HOLD             0x00000000  // Register holds current value
#define MMCR1_PMC3_CYCLE            0x08000000  // Processor cycles
#define MMCR1_PMC3_INSTRUCTION      0x10000000  // # of instructions completed
#define MMCR1_PMC3_TRANSITION       0x18000000  // # of time-base (lower) bit transitions
#define MMCR1_PMC3_DISPATCHED       0x20000000  // # of instructions dispatched
#define MMCR1_PMC3_DC_MISS          0x28000000  // # of L1 data cache misses
#define MMCR1_PMC3_DTLB_MISS        0x30000000  // # of DTLB misses
#define MMCR1_PMC3_L2_D_MISS        0x38000000  // # of L2 data misses
#define MMCR1_PMC3_Bx_TAKEN         0x40000000  // # predicted branches that were taken
#define MMCR1_PMC3_PM_SWITCH        0x48000000  // # of transitions between marked and unmarked processes
#define MMCR1_PMC3_COND_STORE       0x50000000  // # of store conditional instructions completed
#define MMCR1_PMC3_FPU              0x58000000  // # of instructions completed from the FPU
#define MMCR1_PMC3_L2_SNOOP_CASTOUT 0x60000000  // # of L2 castout caused by snoops to modified lines
#define MMCR1_PMC3_L2_HIT           0x68000000  // # of cache operations that hit in the L2 cache
#define MMCR1_PMC3_L1_MISS_CYCLE    0x78000000  // # of cycles generated by L1 load misses
#define MMCR1_PMC3_Bx_SECOND        0x80000000  // # of branches in the second speculative branch
                                                //   resolved correctly
#define MMCR1_PMC3_BPU_LR_CR        0x88000000  // # of cycles the BPU stalls due to LR or CR unresolved
                                                //   dependencies

#define MMCR1_PMC4_HOLD             0x00000000  // Register holds current value
#define MMCR1_PMC4_CYCLE            0x00400000  // Processor cycles
#define MMCR1_PMC4_INSTRUCTION      0x00800000  // # of instructions completed
#define MMCR1_PMC4_TRANSITION       0x00C00000  // # of time-base (lower) bit transitions
#define MMCR1_PMC4_DISPATCHED       0x01000000  // # of instructions dispatched
#define MMCR1_PMC4_L2_CASTOUT       0x01400000  // # of L2 castouts
#define MMCR1_PMC4_DTLB_CYCLE       0x01800000  // # of cycles spent performing table searches for DTLB accesses
#define MMCR1_PMC4_Bx_MISSED        0x02000000  // # of mispredicted branches
#define MMCR1_PMC4_COND_STORE_INT   0x02800000  // # of store conditional instructions completed
                                                //   with reservation intact
#define MMCR1_PMC4_SYNC             0x02C00000  // # of completed sync instructions
#define MMCR1_PMC4_SNOOP_RETRY      0x03000000  // # of snoop request retries
#define MMCR1_PMC4_INTEGER          0x03400000  // # of completed integer operations
#define MMCR1_PMC4_BPU_THIRD        0x03800000  // # of cycles the BPU cannot process new branches
                                                //   due to having two unresolved branches
#define MMCR1_PMC4_DC_MISS          0x07C00000  // # of L1 data cache misses

/*---------------------------------------------------------------------------*
    FPSCR bits
 *---------------------------------------------------------------------------*/
#ifndef FPSCR_FX
#define FPSCR_FX            0x80000000  // Exception summary
#define FPSCR_FEX           0x40000000  // Enabled exception summary
#define FPSCR_VX            0x20000000  // Invalid operation
#define FPSCR_OX            0x10000000  // Overflow exception
#define FPSCR_UX            0x08000000  // Underflow exception
#define FPSCR_ZX            0x04000000  // Zero divide exception
#define FPSCR_XX            0x02000000  // Inexact exception
#define FPSCR_VXSNAN        0x01000000  // SNaN
#define FPSCR_VXISI         0x00800000  // Infinity - Infinity
#define FPSCR_VXIDI         0x00400000  // Infinity / Infinity
#define FPSCR_VXZDZ         0x00200000  // 0 / 0
#define FPSCR_VXIMZ         0x00100000  // Infinity * 0
#define FPSCR_VXVC          0x00080000  // Invalid compare
#define FPSCR_FR            0x00040000  // Fraction rounded
#define FPSCR_FI            0x00020000  // Fraction inexact
#define FPSCR_VXSOFT        0x00000400  // Software request
#define FPSCR_VXSQRT        0x00000200  // Invalid square root
#define FPSCR_VXCVI         0x00000100  // Invalid integer convert
#define FPSCR_VE            0x00000080  // Invalid operation exception enable
#define FPSCR_OE            0x00000040  // Overflow exception enable
#define FPSCR_UE            0x00000020  // Underflow exception enable
#define FPSCR_ZE            0x00000010  // Zero divide exception enable
#define FPSCR_XE            0x00000008  // Inexact exception enable
#define FPSCR_NI            0x00000004  // Non-IEEE mode
#endif

#ifndef FPSCR_FX_BIT
#define FPSCR_FX_BIT        0           // Exception summary
#define FPSCR_FEX_BIT       1           // Enabled exception summary
#define FPSCR_VX_BIT        2           // Invalid operation
#define FPSCR_OX_BIT        3           // Overflow exception
#define FPSCR_UX_BIT        4           // Underflow exception
#define FPSCR_ZX_BIT        5           // Zero divide exception
#define FPSCR_XX_BIT        6           // Inexact exception
#define FPSCR_VXSNAN_BIT    7           // SNaN
#define FPSCR_VXISI_BIT     8           // Infinity - Infinity
#define FPSCR_VXIDI_BIT     9           // Infinity / Infinity
#define FPSCR_VXZDZ_BIT     10          // 0 / 0
#define FPSCR_VXIMZ_BIT     11          // Infinity * 0
#define FPSCR_VXVC_BIT      12          // Invalid compare
#define FPSCR_FR_BIT        13          // Fraction rounded
#define FPSCR_FI_BIT        14          // Fraction inexact
#define FPSCR_VXSOFT_BIT    21          // Software request
#define FPSCR_VXSQRT_BIT    22          // Invalid square root
#define FPSCR_VXCVI_BIT     23          // Invalid integer convert
#define FPSCR_VE_BIT        24          // Invalid operation exception enable
#define FPSCR_OE_BIT        25          // Overflow exception enable
#define FPSCR_UE_BIT        26          // Underflow exception enable
#define FPSCR_ZE_BIT        27          // Zero divide exception enable
#define FPSCR_XE_BIT        28          // Inexact exception enable
#define FPSCR_NI_BIT        29          // Non-IEEE mode
#endif

union FpscrUnion {
    f64 f;
    struct {
        u32 fpscr_pad;
        u32 fpscr;
    } u;
};

// PPCArch
u32 PPCMfmsr();
void PPCMtmsr(u32 newMSR);
u32 PPCOrMsr(u32 value);
u32 PPCMfhid0();
void PPCMthid0(u32 newHID0);
u32 PPCMfl2cr();
void PPCMtl2cr(u32 newL2cr);
void PPCMtdec(u32 newDec);
void PPCSync();
void PPCHalt();
u32 PPCMffpscr();
void PPCMtfpscr(u32 newFPSCR);
u32 PPCMfhid2();
void PPCMthid2(u32 newhid2);
u32 PPCMfwpar();
void PPCMtwpar(u32 newwpar);
void PPCEnableSpeculation();
void PPCDisableSpeculation();
void PPCSetFpIEEEMode();
void PPCSetFpNonIEEEMode();
u32 PPCMfpmc4();
u32 PPCMfpmc3();
u32 PPCMfpmc1();
void PPCMtpmc1(u32 newPmc1);
void PPCMtpmc2(u32 newPmc1);
void PPCMtpmc3(u32 newPmc1);
void PPCMtpmc4(u32 newPmc1);
void PPCMthid4(u32);
void PPCMtmmcr0(u32 newMmcr0);
void PPCMtmmcr1(u32 newMmcr0);
void PPCMtdmaU(u32 newdmau);
void PPCMtdmaL(u32 newdmal);
u32 PPCMfdec(void);
u32 PPCMfpmc2(void);
u32 PPCAndMsr(u32 value);
u32 PPCAndCMsr(u32 value);
u32 PPCMfhid1();
void PPCEieio();
u32 PPCMfmmcr0();
u32 PPCMfmmcr1();
u32 PPCMfpmc2();
u32 PPCMfsia();
void PPCMtsia(u32 newSia);
u32 PPCMfdmaL();
u32 PPCMfpvr();
u32 PPCMfdmaU();

// PPCPm
void PMBegin(void);
void PMEnd(void);
void PMCycles(void);
void PML1FetchMisses(void);
void PML1MissCycles(void);
void PMInstructions(void);

#ifdef __cplusplus
}
#endif

#endif // _REVOLUTION_PPCARCH
