#ifndef __METROTRK_TRK_H__
#define __METROTRK_TRK_H__

#include <dolphin/types.h>

#ifdef __cplusplus
extern "C" {
#endif

//////////// TRK ENUMS /////////////
// Hardware types.
typedef enum {
    HARDWARE_AMC_DDH = 0,
    HARDWARE_GDEV = 1,
    HARDWARE_BBA = 2,
} HardwareType;

// DS Error returns.
typedef enum {
    DS_NoError = 0x0,
    DS_StepError = 0x1,
    DS_ParameterError = 0x2,

    DS_EventQueueFull = 0x100,

    DS_NoMessageBufferAvailable = 0x300,
    DS_MessageBufferOverflow = 0x301,
    DS_MessageBufferReadError = 0x302,

    DS_DispatchError = 0x500,

    DS_InvalidMemory = 0x700,
    DS_InvalidRegister = 0x701,
    DS_CWDSException = 0x702,
    DS_UnsupportedError = 0x703,
    DS_InvalidProcessID = 0x704,
    DS_InvalidThreadID = 0x705,
    DS_OSError = 0x706,

    DS_Error800 = 0x800,
} DSError;

// Where to read/write.
typedef enum {
    DS_Stdin = 0,
    DS_Stdout = 1,
    DS_Stderr = 2,
} DSFileHandle;

// IO returns.
typedef enum {
    DS_IONoError = 0,
    DS_IOError = 1,
    DS_IOEOF = 2,
} DSIOResult;

// Message command IDs
typedef enum {
    DSMSG_Ping = 0x0,
    DSMSG_Connect = 0x1,
    DSMSG_Disconnect = 0x2,
    DSMSG_Reset = 0x3,
    DSMSG_Versions = 0x4,
    DSMSG_SupportMask = 0x5,
    DSMSG_Override = 0x7,

    DSMSG_ReadMemory = 0x10,
    DSMSG_WriteMemory = 0x11,
    DSMSG_ReadRegisters = 0x12,
    DSMSG_WriteRegisters = 0x13,
    DSMSG_SetOption = 0x17,
    DSMSG_Continue = 0x18,
    DSMSG_Step = 0x19,
    DSMSG_Stop = 0x1A,

    DSMSG_ReplyACK = 0x80,

    DSMSG_NotifyStopped = 0x90,
    DSMSG_NotifyException = 0x91,

    DSMSG_WriteFile = 0xD0,
    DSMSG_ReadFile = 0xD1,
    DSMSG_OpenFile = 0xD2,
    DSMSG_CloseFile = 0xD3,
    DSMSG_PositionFile = 0xD4,

    DSMSG_ReplyNAK = 0xFF,
} MessageCommandID;

// Register commands.
typedef enum {
    DSREG_Default = 0,
    DSREG_FP = 1,
    DSREG_Extended1 = 2,
    DSREG_Extended2 = 3,
} DSMessageRegisterOptions;

// Step commands.
typedef enum {
    DSSTEP_IntoCount = 0x0,
    DSSTEP_IntoRange = 0x1,
    DSSTEP_OverCount = 0x10,
    DSSTEP_OverRange = 0x11,
} DSMessageStepOptions;

typedef enum {
    DSREPLY_NoError = 0x0,
    DSREPLY_Error = 0x1,
    DSREPLY_PacketSizeError = 0x2,
    DSREPLY_CWDSError = 0x3,
    DSREPLY_EscapeError = 0x4,
    DSREPLY_BadFCS = 0x5,
    DSREPLY_Overflow = 0x6,
    DSREPLY_SequenceMissing = 0x7,

    DSREPLY_UnsupportedCommandError = 0x10,
    DSREPLY_ParameterError = 0x11,
    DSREPLY_UnsupportedOptionError = 0x12,
    DSREPLY_InvalidMemoryRange = 0x13,
    DSREPLY_InvalidRegisterRange = 0x14,
    DSREPLY_CWDSException = 0x15,
    DSREPLY_NotStopped = 0x16,
    DSREPLY_BreakpointsFull = 0x17,
    DSREPLY_BreakpointConflict = 0x18,

    DSREPLY_OSError = 0x20,
    DSREPLY_InvalidProcessID = 0x21,
    DSREPLY_InvalidThreadID = 0x22,
    DSREPLY_DebugSecurityError = 0x23,
} DSReplyError;

typedef enum {
    DSRECV_Wait = 0,
    DSRECV_Found = 1,
    DSRECV_InFrame = 2,
    DSRECV_FrameOverflow = 3,
} ReceiverState;

typedef enum {
    DSMSGMEMORY_Segmented = 0x01, /* non-flat addr space */
    DSMSGMEMORY_Extended = 0x02,  /* > 32-bit data addr */
    DSMSGMEMORY_Protected = 0x04, /* non-user memory */
    DSMSGMEMORY_Userview = 0x08,  /* breakpoints are invisible */
    DSMSGMEMORY_Space_program = 0x00,
    DSMSGMEMORY_Space_data = 0x40,
    DSMSGMEMORY_Space_io = 0x80
};

typedef enum {
    NUBEVENT_Null = 0,
    NUBEVENT_Shutdown = 1,
    NUBEVENT_Request = 2,
    NUBEVENT_Breakpoint = 3,
    NUBEVENT_Exception = 4,
    NUBEVENT_Support = 5,
} NubEventType;

typedef enum {
    VALIDMEM_Readable = 0,
    VALIDMEM_Writeable = 1,
} ValidMemoryOptions;

typedef enum {
    MEMACCESS_UserMemory = 0,
    MEMACCESS_DebuggerMemory = 1,
} MemoryAccessOptions;

typedef int UARTError;

typedef enum {
    UART_NoError = 0,
    UART_UnknownBaudRate = 1,
    UART_ConfigurationError = 2,
    UART_BufferOverflow = 3,  // specified buffer was too small
    UART_NoData = 4,          // no data available from polling
} UARTErrorOptions;

typedef enum {
    kBaudHWSet = -1,  // use HW settings such as DIP switches
    kBaud300 = 300,   // valid baud rates
    kBaud600 = 600,
    kBaud1200 = 1200,
    kBaud1800 = 1800,
    kBaud2000 = 2000,
    kBaud2400 = 2400,
    kBaud3600 = 3600,
    kBaud4800 = 4800,
    kBaud7200 = 7200,
    kBaud9600 = 9600,
    kBaud19200 = 19200,
    kBaud38400 = 38400,
    kBaud57600 = 57600,
    kBaud115200 = 115200,
    kBaud230400 = 230400
} UARTBaudRate;

////////////////////////////////////

typedef int MessageBufferID;

#define TRKMSGBUF_SIZE (0x800 + 0x80)

typedef struct TRKBuffer {
	/* 0x00 */ u32 _00;
	/* 0x04 */ BOOL isInUse;
	/* 0x08 */ u32 length;
	/* 0x0C */ u32 position;
	/* 0x10 */ u8 data[TRKMSGBUF_SIZE];
} TRKBuffer;

typedef struct TRKFramingState {
    MessageBufferID msgBufID;    // _00
    TRKBuffer* buffer;           // _04
    ReceiverState receiveState;  // _08
    BOOL isEscape;               // _0C
    u8 fcsType;                  // _10
} TRKFramingState;

typedef struct TRKState_PPC {
	u32 GPR[32];         // 0x0
	u32 LR;              // 0x80
	u32 CTR;             // 0x84
	u32 XER;             // 0x88
	u32 MSR;             // 0x8c
	u32 DAR;             // 0x90
	u32 DSISR;           // 0x94
	BOOL stopped;        // 0x98
	BOOL inputActivated; // 0x9c
	u8* inputPendingPtr; // 0xA0
} TRKState_PPC;

typedef struct CommandReply {
    u32 _00;  // _00
    union {
        u8 b;
        MessageCommandID m;
    } commandID;  // _04, use MessageCommandID enum
    union {
        u8 b;
        DSReplyError r;
    } replyError;  // _08, use DSReplyError enum - should be enum type? check size.
    u32 _0C;       // _0C
    u8 _10[0x30];  // _10, unknown
} CommandReply;

typedef struct ProcessorRestoreFlags_PPC {
	u8 TBR;
	u8 DEC;
	u8 linker_padding[0x9 - 0x2];
} ProcessorRestoreFlags_PPC;

void TRKSaveExtended1Block();

#define SPR_XER    1
#define SPR_LR     8
#define SPR_CTR    9
#define SPR_DSISR  18
#define SPR_DAR    19
#define SPR_DEC    22
#define SPR_SDR1   25
#define SPR_SRR0   26
#define SPR_SRR1   27
#define SPR_SPRG0  272
#define SPR_SPRG1  273
#define SPR_SPRG2  274
#define SPR_SPRG3  275
#define SPR_EAR    282
#define SPR_TBL    284
#define SPR_TBU    285
#define SPR_PVR    287
#define SPR_IBAT0U 528
#define SPR_IBAT0L 529
#define SPR_IBAT1U 530
#define SPR_IBAT1L 531
#define SPR_IBAT2U 532
#define SPR_IBAT2L 533
#define SPR_IBAT3U 534
#define SPR_IBAT3L 535
#define SPR_IBAT4U 560
#define SPR_IBAT4L 561
#define SPR_IBAT5U 562
#define SPR_IBAT5L 563
#define SPR_IBAT6U 564
#define SPR_IBAT6L 565
#define SPR_IBAT7U 566
#define SPR_IBAT7L 567
#define SPR_DBAT0U 536
#define SPR_DBAT0L 537
#define SPR_DBAT1U 538
#define SPR_DBAT1L 539
#define SPR_DBAT2U 540
#define SPR_DBAT2L 541
#define SPR_DBAT3U 542
#define SPR_DBAT3L 543
#define SPR_DBAT4U 568
#define SPR_DBAT4L 569
#define SPR_DBAT5U 570
#define SPR_DBAT5L 571
#define SPR_DBAT6U 572
#define SPR_DBAT6L 573
#define SPR_DBAT7U 574
#define SPR_DBAT7L 575
#define SPR_GQR0   912
#define SPR_GQR1   913
#define SPR_GQR2   914
#define SPR_GQR3   915
#define SPR_GQR4   916
#define SPR_GQR5   917
#define SPR_GQR6   918
#define SPR_GQR7   919
#define SPR_HID2   920
#define SPR_WPAR   921
#define SPR_DMA_U  922
#define SPR_DMA_L  923
#define SPR_UMMCR0 936
#define SPR_UPMC1  937
#define SPR_UPMC2  938
#define SPR_USIA   939
#define SPR_UMMCR1 940
#define SPR_UPMC3  941
#define SPR_UPMC4  942
#define SPR_USDA   943
#define SPR_MMCR0  952
#define SPR_PMC1   953
#define SPR_PMC2   954
#define SPR_SIA    955
#define SPR_MMCR1  956
#define SPR_PMC3   957
#define SPR_PMC4   958
#define SPR_SDA    959
#define SPR_HID0   1008
#define SPR_HID1   1009
#define SPR_IABR   1010
#define SPR_HID4   1011
#define SPR_DABR   1013
#define SPR_L2CR   1017
#define SPR_ICTC   1019
#define SPR_THRM1  1020
#define SPR_THRM2  1021
#define SPR_FPECR  1022

// PPC exceptions
// 0x000 is reserved
#define PPC_SystemReset             0x100
#define PPC_MachineCheck            0x200
#define PPC_DataStorage             0x300
#define PPC_InstructionStorage      0x400
#define PPC_ExternalInterrupt       0x500
#define PPC_Alignment               0x600
#define PPC_Program                 0x700
#define PPC_FloatingPointUnavaiable 0x800
#define PPC_Decrementer             0x900
// 0xA00-0xB00 are reserved
#define PPC_SystemCall          0xC00
#define PPC_Trace               0xD00
#define PPC_FloatingPointAssist 0xE00 // unimplemented in 750CL
#define PPC_PerformanceMonitor  0xF00 // Dolphin/Broadway specific
// 0x1000-0x1200 are unimplemented in 750CL
#define PPC_InstructionAddressBreakpoint 0x1300 // Dolphin/Broadway specific
// 0x1400-0x2F00 are reserved, but TRK uses some
#define PPC_SystemManagementInterrupt 0x1400
// 0x1500-0x1600 are unimplemented in 750CL
#define PPC_ThermalManagementInterrupt 0x1700
#define PPC_1800Exception              0x1800
#define PPC_1900Exception              0x1900
#define PPC_1A00Exception              0x1A00
#define PPC_1B00Exception              0x1B00
#define PPC_1C00Exception              0x1C00 // Data breakpoint?
#define PPC_1D00Exception              0x1D00 // Instruction breakpoint?
#define PPC_1E00Exception              0x1E00 // Peripheral breakpoint?
#define PPC_1F00Exception              0x1F00 // Non maskable development port?
#define PPC_2000Exception              0x2000

#ifdef __cplusplus
}
#endif

#endif /* __METROTRK_TRK_H__ */
