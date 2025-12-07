#include <revolution.h>
#include <revolution/exi.h>
#include <revolution/os.h>
#include <revolution/si.h>
#include <revolution/db.h>
#include <revolution/sc.h>

#include "__os.h"

#define NOP 0x60000000

// external functions
extern void EnableMetroTRKInterrupts(void);
extern void __OSInitMemoryProtection(void);
extern void IPCCltInit(void);
extern BOOL __DVDCheckDevice(void);

#define DB_EXCEPTIONRET_OFFSET 0xC
#define DB_EXCEPTIONDEST_OFFSET 0x8
#define OS_CURRENTCONTEXT_PADDR 0x00C0
#define OS_EXCEPTIONTABLE_ADDR 0x3000
#define OS_DBJUMPPOINT_ADDR 0x60

#ifdef SDK_AUG2010
#define BUILD_DATE "Aug 23 2010"
#if DEBUG
#define BUILD_TIME "17:27:45"
#else
#define BUILD_TIME "17:33:06"
#endif
#elif SDK_SEP2006
#define BUILD_DATE  "Sep 21 2006"
#define BUILD_TIME "14:32:13"
#endif

#ifdef SDK_AUG2010
#if DEBUG
const char* __OSVersion = "<< RVL_SDK - OS \tdebug build: "BUILD_DATE" "BUILD_TIME" (0x4302_145) >>";
#else
const char* __OSVersion = "<< RVL_SDK - OS \trelease build: "BUILD_DATE" "BUILD_TIME" (0x4302_145) >>";
#endif
#elif SDK_SEP2006
const char* __OSVersion = "<< RVL_SDK - OS \trelease build: "BUILD_DATE" "BUILD_TIME" (0x4200_60422) >>";
#endif

static DVDDriveInfo DriveInfo;
static DVDCommandBlock DriveBlock;
OSExecParams __OSRebootParams;

#ifdef SDK_SEP2006
static char GameNameBuffer[5];
#endif

extern u32 __DVDLongFileNameFlag;
extern u32 __PADSpec;

// defined in link script
extern u8 __ArenaLo[];
extern char _stack_addr[];
extern u8 __ArenaHi[];

OSTime __OSStartTime;

static OSBootInfo* BootInfo;
static u32* BI2DebugFlag;
static u32 BI2DebugFlagHolder;

void (**OSExceptionTable)(u8, OSContext*);
BOOL AreWeInitialized;
f32 ZeroPS[2];
f64 ZeroF;
BOOL __OSIsGcam;
BOOL __OSInNandBoot;
BOOL __OSInIPL;

// prototypes
static void __OSInitFPRs(void);
static void OSExceptionInit(void);

// dummy entry points to the OS Exception vector
void __OSEVStart(void);
void __OSEVEnd(void);
void __OSEVSetNumber(void);
void __OSExceptionVector(void);
void __DBVECTOR(void);
void __OSDBINTSTART(void);
void __OSDBINTEND(void);
void __OSDBJUMPSTART(void);
void __OSDBJUMPEND(void);

/* clang-format off */
#ifdef __GEKKO__
asm void __OSFPRInit(void) {
    // clang-format off
    nofralloc

    mfmsr r3
    ori r3, r3, 0x2000
    mtmsr r3

    mfspr r3, 0x398
    rlwinm. r3, r3, 3, 0x1f, 0x1f
    beq skip_ps_init

    lis r3, ZeroPS@ha
    addi r3, r3, ZeroPS@l
    psq_l f0, 0(r3), 0, 0
    ps_mr f1, f0
    ps_mr f2, f0
    ps_mr f3, f0
    ps_mr f4, f0
    ps_mr f5, f0
    ps_mr f6, f0
    ps_mr f7, f0
    ps_mr f8, f0
    ps_mr f9, f0
    ps_mr f10, f0
    ps_mr f11, f0
    ps_mr f12, f0
    ps_mr f13, f0
    ps_mr f14, f0
    ps_mr f15, f0
    ps_mr f16, f0
    ps_mr f17, f0
    ps_mr f18, f0
    ps_mr f19, f0
    ps_mr f20, f0
    ps_mr f21, f0
    ps_mr f22, f0
    ps_mr f23, f0
    ps_mr f24, f0
    ps_mr f25, f0
    ps_mr f26, f0
    ps_mr f27, f0
    ps_mr f28, f0
    ps_mr f29, f0
    ps_mr f30, f0
    ps_mr f31, f0

skip_ps_init:
    lfd f0, ZeroF(r13)
    fmr f1, f0
    fmr f2, f0
    fmr f3, f0
    fmr f4, f0
    fmr f5, f0
    fmr f6, f0
    fmr f7, f0
    fmr f8, f0
    fmr f9, f0
    fmr f10, f0
    fmr f11, f0
    fmr f12, f0
    fmr f13, f0
    fmr f14, f0
    fmr f15, f0
    fmr f16, f0
    fmr f17, f0
    fmr f18, f0
    fmr f19, f0
    fmr f20, f0
    fmr f21, f0
    fmr f22, f0
    fmr f23, f0
    fmr f24, f0
    fmr f25, f0
    fmr f26, f0
    fmr f27, f0
    fmr f28, f0
    fmr f29, f0
    fmr f30, f0
    fmr f31, f0

    mtfsf 0xff, f0
    blr
    // clang-format on
}
#endif

static void DisableWriteGatherPipe(void) {
    u32 hid2;

    hid2 = PPCMfhid2();
    hid2 &= ~0x40000000;
    PPCMthid2(hid2);
}

u32 __OSGetHollywoodRev(void) {
    return *(u32*)OSPhysicalToCached(0x3138);
}

void __OSGetIOSRev(OSIOSRev* rev) {
    u32 iosVer, iosBuild;
    iosVer = *(u32*)OSPhysicalToUncached(0x3140);
    iosBuild = *(u32*)OSPhysicalToUncached(0x3144);

    rev->reserved = ((iosVer >> 24) & 0xFF);
    rev->major = ((iosVer >> 16) & 0xFF);
    rev->minor = ((iosVer >> 8) & 0xFF);
    rev->micro = (iosVer & 0xFF);

    rev->month = (((iosBuild >> 20) & 0xF) * 10 + ((iosBuild >> 16) & 0xF));
    rev->date = (((iosBuild >> 12) & 0xF) * 10 + ((iosBuild >> 8) & 0xF));
    rev->year = (((iosBuild >> 4) & 0xF) * 10 + (iosBuild & 0xF) + 2000);
}

// needed for assert
#undef NULL
#define NULL 0

u32 OSGetConsoleType(void) {
    u32 hwRev;
    u32 gddrSize;

    if (BootInfo == NULL || BootInfo->consoleType == 0) {
        return 0x10000002;
    }

    hwRev = __OSGetHollywoodRev();

    if (__OSDeviceCode & 0x8000) {
        switch (__OSDeviceCode & ~0x8000) {
        case 2:
        case 3:
        case 0x203:
            switch (hwRev) {
            case 0:
                return 0x10;
            case 1:
                return 0x11;
            case 2:
                return 0x12;
            case 0x10:
                return 0x20;
            case 0x11:
                return 0x21;
            }

            if (hwRev > 0x11) {
                return 0x21;
            }

        case 0x202:
        case 0x201:
            switch (hwRev) {
            case 0:
                return 0x10000010;
            case 1:
                return 0x10000011;
            case 2:
                return 0x10000012;
            case 0x10:
                return 0x10000020;
            case 0x11:
                return 0x10000021;
            }

            if (hwRev > 0x11) {
                return 0x10000021;
            }

        #if SDK_AUG2010
        case 0x300:
            return 0x100;
        #endif

        default:
            break;
        }
    }

    gddrSize = OSGetPhysicalMem2Size();

    switch (hwRev) {
    case 0:
        if (gddrSize == 0x4000000) {
            return 0x10;
        }
        else {
            return 0x10000010;
        }

    case 0x1:
        if (gddrSize == 0x4000000) {
            return 0x11;
        }
        else {
            return 0x10000011;
        }

    case 0x2:
        if (gddrSize == 0x4000000) {
            return 0x12;
        }
        else {
            return 0x10000012;
        }

    case 0x10:
        if (gddrSize == 0x4000000) {
            return 0x20;
        }
        else {
            return 0x10000020;
        }

    case 0x11:
        if (gddrSize == 0x4000000) {
            return 0x21;
        }
        else {
            return 0x10000021;
        }
    }

    if (hwRev > 0x11) {
        if (gddrSize == 0x4000000) {
            return 0x21;
        }
        else {
            return 0x10000021;
        }
    }

    return BootInfo->consoleType;
}

static void MemClear(void* base, u32 size) {
    void* lastBase = (size > 0x40000) ? (void*)((u32)base + size - 0x40000) : base;
    DCZeroRange(base, size);
    DCFlushRange(lastBase, 0x40000);
}

static void ClearArena(void) {
    if (!OSIsRestart()) {
        MemClear(OSGetArenaLo(), (u32)OSGetArenaHi() - (u32)OSGetArenaLo());
        return;
    }

    if (__OSRebootParams.regionStart == NULL || !OSIsMEM1Region((void*)__OSRebootParams.regionStart)) {
        MemClear(OSGetArenaLo(), (u32)OSGetArenaHi() - (u32)OSGetArenaLo());
        return;
    }

    ASSERTLINE(1068, __OSRebootParams.regionEnd != NULL);

    if ((u32)OSGetArenaLo() < (u32)__OSRebootParams.regionStart) {
        if ((u32)OSGetArenaHi() <= (u32)__OSRebootParams.regionStart) {
            MemClear(OSGetArenaLo(), (u32)OSGetArenaHi() - (u32)OSGetArenaLo());
        } else {
            MemClear(OSGetArenaLo(), (u32)__OSRebootParams.regionStart - (u32)OSGetArenaLo());
        
            if ((u32)OSGetArenaHi() > (u32)__OSRebootParams.regionEnd) {
                MemClear(__OSRebootParams.regionEnd, (u32)OSGetArenaHi() - (u32)__OSRebootParams.regionEnd);
            }
        }
    }
}

static void ClearMEM2Arena(void) {
    if (!OSIsRestart()) {
        MemClear(OSGetMEM2ArenaLo(), (u32)OSGetMEM2ArenaHi() - (u32)OSGetMEM2ArenaLo());
        return;
    }

    if (__OSRebootParams.regionStart == NULL || !OSIsMEM2Region((void*)__OSRebootParams.regionStart)) {
        MemClear(OSGetMEM2ArenaLo(), (u32)OSGetMEM2ArenaHi() - (u32)OSGetMEM2ArenaLo());
        return;
    }

    ASSERTLINE(1106, __OSRebootParams.regionEnd != NULL);

    if ((u32)OSGetMEM2ArenaLo() < (u32)__OSRebootParams.regionStart) {
        if ((u32)OSGetMEM2ArenaHi() <= (u32)__OSRebootParams.regionStart) {
            MemClear(OSGetMEM2ArenaLo(), (u32)OSGetMEM2ArenaHi() - (u32)OSGetMEM2ArenaLo());
        } else {
            MemClear(OSGetMEM2ArenaLo(), (u32)__OSRebootParams.regionStart - (u32)OSGetMEM2ArenaLo());

            if ((u32)OSGetMEM2ArenaHi() > (u32)__OSRebootParams.regionEnd) {
                MemClear(__OSRebootParams.regionEnd, (u32)OSGetMEM2ArenaHi() - (u32)__OSRebootParams.regionEnd);
            }
        }
    }
}

static void InquiryCallback(s32, DVDCommandBlock* block) {
    switch (block->state) {
    case 0:
        __OSDeviceCode = (u16)(0x8000 | DriveInfo.deviceCode);
        break;
    default:
        __OSDeviceCode = 1;
        break;
    }
}

static void CheckTargets(void) {
    switch (*(u8*)OSPhysicalToCached(0x315C)) {
    case 0x80:
        break;
    case 0x81:
        OSReport("OS ERROR: boot program is not for RVL target. Please use correct boot program.\n");
        OSPanic(__FILE__, 1160, "Failed to run app");
        break;
    default:
        break;
    }

    switch (*(u8*)OSPhysicalToCached(0x315D)) {
    case 0x80:
        break;
    case 0x81:
        OSReport("OS ERROR: apploader[D].img is not for RVL target. Please use correct apploader[D].img.\n");
        OSPanic(__FILE__, 1178, "Failed to run app");
        break;
    default:
        break;
    }
}

#ifdef SDK_AUG2010
static void CheckFirmare(void) {
    OSIOSRev iosRev;
    u32 firmware;
    GXColor bg = {0x00, 0x00, 0xFF, 0x00};
    GXColor fg = {0xFF, 0xFF, 0xFF, 0x00};

    __OSGetIOSRev(&iosRev);
    firmware = iosRev.major << 0x10;
    firmware |= iosRev.minor << 0x8;
    firmware |= iosRev.micro;

    if (iosRev.major != (*(u32*)0x80003188 >> 0x10) || (iosRev.major == (*(u32*)0x80003188 >> 0x10) && firmware < *(u32*)0x80003188)) {
        OSReport("OS ERROR: This firmware is an improper version for this SDK. Please use a correct Firmware.\n");
        OSFatal(fg, bg, "\n\nERROR #002\nAn error has occurred.\nPress the Eject Button, remove the\nGame Disc, and turn off the power to \nthe console. \nPlease read the Wii Operations Manual \nfor further instructions.\n");
        OSPanic(__FILE__, 1243, "Failed to run app");
    }
}
#endif

static void ReportOSInfo(void) {
    u32 consoleType;
    u32 sysMemSize;
    OSIOSRev ios;

    OSReport("\nRevolution OS\n");
    OSReport("Kernel built : %s %s\n", BUILD_DATE, BUILD_TIME);
    OSReport("Console Type : " );

    consoleType = OSGetConsoleType();

    switch (consoleType & 0xF0000000) {
    case 0:
        switch (consoleType) {
        case 0x11:
            OSReport("Pre-production board 1\n");
            break;
        case 0x12:
            OSReport("Pre-production board 2-1\n");
            break;
        case 0x20:
            OSReport("Pre-production board 2-2\n");
            break;
        #if SDK_AUG2010
        case 0x100:
            OSReport("RVA 1\n");
            break;
        #endif
        default:
            OSReport("Retail %d\n", consoleType);
            break;
        }
        break;
    case 0x10000000:
        switch (consoleType) {
        case 0x10000021:
            OSReport("NDEV 2.1\n");
            break;
        case 0x10000020:
            OSReport("NDEV 2.0\n");
            break;
        case 0x10000012:
            OSReport("NDEV 1.2\n");
            break;
        case 0x10000011:
            OSReport("NDEV 1.1\n");
            break;
        case 0x10000010:
            OSReport("NDEV 1.0\n");
            break;
        case 0x10000008:
            OSReport("Revolution Emulator\n");
            break;
        default:
            OSReport("Emulation platform (%08x)\n", consoleType);
            break;
        }
        break;
    case 0x20000000:
        OSReport("TDEV-based emulation HW%d\n", (consoleType & ~0xF0000000) - 3);
        break;
    default:
        OSReport("%08x\n", consoleType);
        break;
    }

    __OSGetIOSRev(&ios);
    OSReport("Firmware     : %d.%d.%d ", ios.major, ios.minor, ios.micro);
    OSReport("(%d/%d/%d)\n", ios.month, ios.date, ios.year);
    sysMemSize = (OSGetConsoleSimulatedMem1Size() + OSGetConsoleSimulatedMem2Size());
    OSReport("Memory %d MB\n", sysMemSize/(1024*1024));

    OSReport("MEM1 Arena : 0x%x - 0x%x\n", OSGetMEM1ArenaLo(), OSGetMEM1ArenaHi());
    OSReport("MEM2 Arena : 0x%x - 0x%x\n", OSGetMEM2ArenaLo(), OSGetMEM2ArenaHi());
}

void OSInit(void) {
    void* bi2StartAddr;
    void* var_r31;
    void* arenaAddr = (void*)OSRoundUp32B(__ArenaLo);
    u32 sp10 = 0x81730000;

    if (AreWeInitialized == FALSE) {
        AreWeInitialized = TRUE;

        __OSStartTime = __OSGetSystemTime();
        OSDisableInterrupts();

        __OSGetExecParams(&__OSRebootParams);
        PPCMtmmcr0(0);
        PPCMtmmcr1(0);
        PPCMtpmc1(0);
        PPCMtpmc2(0);
        PPCMtpmc3(0);
        PPCMtpmc4(0);
        PPCMthid4(0x83900000);
        PPCDisableSpeculation();
        PPCSetFpNonIEEEMode();

        BootInfo = (OSBootInfo*)OSPhysicalToCached(0);
        BI2DebugFlag = 0;
        __DVDLongFileNameFlag = 0;

        bi2StartAddr = (void*)(*(u32*)OSPhysicalToCached(0xF4));
        if (bi2StartAddr) {
            BI2DebugFlag = (void*)((char*)bi2StartAddr + 0xC);
            __PADSpec = ((u32*)bi2StartAddr)[9];
            *(u8*)OSPhysicalToCached(0x30E8) = *BI2DebugFlag;
            *(u8*)OSPhysicalToCached(0x30E9) = __PADSpec;
        } else if (BootInfo->arenaHi) {
            BI2DebugFlagHolder = *(u8*)OSPhysicalToCached(0x30E8);
            BI2DebugFlag = &BI2DebugFlagHolder;
            __PADSpec = *(u8*)OSPhysicalToCached(0x30E9);
        }

        __DVDLongFileNameFlag = 1;

        var_r31 = (void*)*(u32*)OSPhysicalToCached(0x310C);
        if (var_r31 == NULL) {
            if (OSIsMEM1Region(arenaAddr)) {
                var_r31 = !BootInfo->arenaLo ? arenaAddr : BootInfo->arenaLo;
            
                if (!BootInfo->arenaLo && (BI2DebugFlag && (*(BI2DebugFlag) < 2))) {
                    var_r31 = (void*)OSRoundUp32B(_stack_addr);
                }
            } else {
                var_r31 = (void*)0x80004000;
            }
        }

        OSSetMEM1ArenaLo(var_r31);

        var_r31 = (void*)*(u32*)OSPhysicalToCached(0x3110);
        if (var_r31 == NULL) {
            var_r31 = !BootInfo->arenaHi ? (void*)sp10 : BootInfo->arenaHi;
        }

        OSSetMEM1ArenaHi(var_r31);

        var_r31 = (void*)*(u32*)OSPhysicalToCached(0x3124);
        if (var_r31 != 0) {
            if (OSIsMEM2Region(arenaAddr)) {
                var_r31 = arenaAddr;

                if ((BI2DebugFlag && (*(BI2DebugFlag) < 2))) {
                    var_r31 = (void*)OSRoundUp32B(_stack_addr);
                }
            }
            else {
                if ((u32)var_r31 >= 0x90000000 && (u32)var_r31 < 0x90000800) {
                    var_r31 = (void*)0x90000800;
                }
            }

            OSSetMEM2ArenaLo(var_r31);
        }


        var_r31 = (void*)*(u32*)OSPhysicalToCached(0x3128);
        if (var_r31 != 0) {
            OSSetMEM2ArenaHi(var_r31);
        }

        __OSInitIPCBuffer();
        OSExceptionInit();
        __OSInitSystemCall();
        __OSInitAlarm();
        __OSModuleInit();
        __OSInterruptInit();
        __OSContextInit();
        __OSCacheInit();
        EXIInit();
        SIInit();
        __OSInitSram();
        __OSThreadInit();
        __OSInitAudioSystem();

        DisableWriteGatherPipe();

        if (!__OSInIPL) {
            __OSInitMemoryProtection();
        }

        ReportOSInfo();
        OSRegisterVersion(__OSVersion);

        // if location of debug flag exists, and flag is >= 2, enable MetroTRKInterrupts
        if (BI2DebugFlag && ((*BI2DebugFlag) >= 2)) {
            EnableMetroTRKInterrupts();
        }

        #ifdef SDK_AUG2010
        if (!__OSInNandBoot && !__OSInReboot) {
        #endif
            ClearArena();
            ClearMEM2Arena();
        #ifdef SDK_AUG2010
        }
        #endif

        OSEnableInterrupts();
        IPCCltInit();

        if (!__OSInNandBoot && !__OSInReboot) {
            __OSInitSTM();

            SCInit();

            /* do nothing until SC is not busy */
            while (SCCheckStatus() == 1) {}

            #ifdef SDK_AUG2010
            __OSInitNet();
            #endif
        }

        if (!__OSInIPL) {
            CheckTargets();
            #ifdef SDK_AUG2010
            CheckFirmare();
            #endif
            DVDInit();

            if (__OSIsGcam) {
                __OSDeviceCode = 0x9000;
            } else if (!__OSDeviceCode) {
                DCInvalidateRange(&DriveInfo, sizeof(DriveInfo));
                DVDInquiryAsync(&DriveBlock, &DriveInfo, InquiryCallback);
            }

            #ifdef SDK_AUG2010
            if (OSGetAppType() == 0x80 && !__OSInReboot) {
                if (!__DVDCheckDevice()) {
                    OSReturnToMenu();
                }
            }
            #endif
        }

        #ifdef SDK_AUG2010
        if (!__OSInIPL && !__OSInNandBoot) {
            __OSInitPlayTime();
        }
        #endif

        if (!__OSInIPL && !__OSInNandBoot && !__OSInReboot) {
            __OSStartPlayRecord();
        }
    }
}

static u32 __OSExceptionLocations[] = {
    0x00000100, 0x00000200, 0x00000300, 0x00000400, 0x00000500, 0x00000600, 0x00000700, 0x00000800,
    0x00000900, 0x00000C00, 0x00000D00, 0x00000F00, 0x00001300, 0x00001400, 0x00001700,
};

#if DEBUG
char * __OSExceptionNames[17] = {
	"System reset",
	"MachineCheck",
	"DSI",
	"ISI",
	"External Int.",
	"Alignment",
	"Program",
	"FP Unavailable",
	"Decrementer",
	"System call",
	"Trace",
	"Perf mon",
	"IABR",
	"SMI",
	"Thermal Int.",
    "Protection error",
    "FP Exception",
};
#endif

static void OSExceptionInit(void) {
    __OSException exception;
    void* destAddr;
    
    // These two vars help us change the exception number embedded
    // in the exception handler code.
    u32* opCodeAddr;
    u32 oldOpCode;
    
    // Address range of the actual code to be copied.
    u8* handlerStart;
    u32 handlerSize;
    
    ASSERTMSGLINE(1830, ((u32)&__OSEVEnd - (u32)&__OSEVStart) <= 0x100, "OSExceptionInit(): too big exception vector code.");
      
    // Install the first level exception vector.
    opCodeAddr = (u32*)__OSEVSetNumber;
    oldOpCode = *opCodeAddr;
    handlerStart = (u8*)__OSEVStart;
    handlerSize = (u32)((u8*)__OSEVEnd - (u8*)__OSEVStart);
    
    // Install the DB integrator, only if we are the first OSInit to be run
    destAddr = (void*)OSPhysicalToCached(OS_DBJUMPPOINT_ADDR);
    if (*(u32*)destAddr == 0) // Lomem should be zero cleared only once by BS2
    {
        #ifdef SDK_SEP2006
        DBPrintf("Installing OSDBIntegrator\n");
        #endif
        memcpy(destAddr, (void*)__OSDBINTSTART, (u32)__OSDBINTEND - (u32)__OSDBINTSTART);
        DCFlushRangeNoSync(destAddr, (u32)__OSDBINTEND - (u32)__OSDBINTSTART);
        __sync();
        ICInvalidateRange(destAddr, (u32)__OSDBINTEND - (u32)__OSDBINTSTART);
    }
    
    // Copy the right vector into the table
    for (exception = 0; exception < __OS_EXCEPTION_MAX; exception++) {
        if (BI2DebugFlag && (*BI2DebugFlag >= 2)) {
            #ifdef SDK_SEP2006
            if (__DBIsExceptionMarked(exception)) {
                DBPrintf(">>> OSINIT: exception %d commandeered by TRK\n", exception);
                continue;
            }
            #else
            continue;
            #endif
        }
        
        // Modify the copy of code in text before transferring
        // to the exception table.
        *opCodeAddr = oldOpCode | exception;
        
        // Modify opcodes at __DBVECTOR if necessary
        #ifdef SDK_SEP2006
        if (__DBIsExceptionMarked(exception)) {
            DBPrintf(">>> OSINIT: exception %d vectored to debugger\n", exception);
            memcpy((void*)__DBVECTOR, (void*)__OSDBJUMPSTART, (u32)__OSDBJUMPEND - (u32)__OSDBJUMPSTART);
        } else
        #endif
        {
            // make sure the opcodes are still nop
            u32* ops = (u32*)__DBVECTOR;
            int cb;
            
            for (cb = 0; cb < (u32)__OSDBJUMPEND - (u32)__OSDBJUMPSTART; cb += sizeof(u32)) {
                *ops++ = NOP;
            }
        }
        
        // Install the modified handler.
        destAddr = (void*)OSPhysicalToCached(__OSExceptionLocations[(u32)exception]);
        memcpy(destAddr, handlerStart, handlerSize);
        DCFlushRangeNoSync(destAddr, handlerSize);
        __sync();
        ICInvalidateRange(destAddr, handlerSize);
    }
    // initialize pointer to exception table
    OSExceptionTable = (void*)OSPhysicalToCached(OS_EXCEPTIONTABLE_ADDR);
    
    // install default exception handlers
    for (exception = 0; exception < __OS_EXCEPTION_MAX; exception++) {
        __OSSetExceptionHandler(exception, OSDefaultExceptionHandler);
    }
    
    // restore the old opcode, so that we can re-start an application without
    // downloading the text segments
    *opCodeAddr = oldOpCode;

    #ifdef SDK_SEP2006
    DBPrintf("Exceptions initialized...\n");
    #endif
}

#ifdef __GEKKO__
static asm void __OSDBIntegrator(void) {
    nofralloc
entry __OSDBINTSTART
    li      r5, OS_DBINTERFACE_ADDR
    mflr    r3
    stw     r3, DB_EXCEPTIONRET_OFFSET(r5)
    lwz     r3, DB_EXCEPTIONDEST_OFFSET(r5)
    oris    r3, r3, OS_CACHED_REGION_PREFIX
    mtlr    r3
    li      r3, 0x30 // MSR_IR | MSR_DR     // turn on memory addressing
    mtmsr   r3
    blr
entry __OSDBINTEND
}
#endif

#ifdef __GEKKO__
static asm void __OSDBJump(void) {
    nofralloc
entry __OSDBJUMPSTART
    bla     OS_DBJUMPPOINT_ADDR
entry __OSDBJUMPEND
}
#endif

__OSExceptionHandler __OSSetExceptionHandler(__OSException exception, __OSExceptionHandler handler) {
    __OSExceptionHandler oldHandler;
    
    ASSERTMSGLINE(1974, exception < __OS_EXCEPTION_MAX, "__OSSetExceptionHandler(): unknown exception."); 
    
    oldHandler = OSExceptionTable[exception];
    OSExceptionTable[exception] = handler;
    return oldHandler;
}

__OSExceptionHandler __OSGetExceptionHandler(__OSException exception) {
    ASSERTMSGLINE(1997, exception < __OS_EXCEPTION_MAX, "__OSGetExceptionHandler(): unknown exception.");
    return OSExceptionTable[exception];
}

#ifdef __GEKKO__
static asm void OSExceptionVector(void) {
    nofralloc

entry __OSEVStart
    // Save r4 into SPRG0
    mtsprg  0, r4

    // Load current context physical address into r4
    lwz     r4, OS_CURRENTCONTEXT_PADDR

    // Save r3 - r5 into the current context
    stw     r3, OS_CONTEXT_R3(r4)
    mfsprg  r3, 0
    stw     r3, OS_CONTEXT_R4(r4)
    stw     r5, OS_CONTEXT_R5(r4)

    lhz     r3, OS_CONTEXT_STATE(r4)
    ori     r3, r3, OS_CONTEXT_STATE_EXC
    sth     r3, OS_CONTEXT_STATE(r4)

    // Save misc registers
    mfcr    r3
    stw     r3, OS_CONTEXT_CR(r4)
    mflr    r3
    stw     r3, OS_CONTEXT_LR(r4)
    mfctr   r3
    stw     r3, OS_CONTEXT_CTR(r4)
    mfxer   r3
    stw     r3, OS_CONTEXT_XER(r4)
    mfsrr0  r3
    stw     r3, OS_CONTEXT_SRR0(r4)
    mfsrr1  r3
    stw     r3, OS_CONTEXT_SRR1(r4)
    mr      r5, r3

entry __DBVECTOR
    nop

    // Set SRR1[IR|DR] to turn on address
    // translation at the next RFI
    mfmsr   r3
    ori     r3, r3, 0x30
    mtsrr1  r3

    // This lets us change the exception number based on the
    // exception we're installing.
entry __OSEVSetNumber
    addi    r3, 0, 0x0000

    // Load current context virtual address into r4
    lwz     r4, 0xD4

    // Check non-recoverable interrupt
    rlwinm. r5, r5, 0, MSR_RI_BIT, MSR_RI_BIT
    bne     recoverable
    addis   r5, 0,  OSDefaultExceptionHandler@ha
    addi    r5, r5, OSDefaultExceptionHandler@l
    mtsrr0  r5
    rfi
    // NOT REACHED HERE

recoverable:
    // Locate exception handler.
    rlwinm  r5, r3, 2, 22, 29               // r5 contains exception*4
    lwz     r5, OS_EXCEPTIONTABLE_ADDR(r5)
    mtsrr0  r5

    // Final state
    // r3 - exception number
    // r4 - pointer to context
    // r5 - garbage
    // srr0 - exception handler
    // srr1 - address translation enalbed, not yet recoverable

    rfi
    // NOT REACHED HERE
    // The handler will restore state

entry __OSEVEnd
    nop
}
#endif

void __OSUnhandledException(__OSException exception, OSContext* context, u32 dsisr, u32 dar);

#ifdef __GEKKO__
asm void OSDefaultExceptionHandler(__REGISTER __OSException exception, __REGISTER OSContext* context) {
    nofralloc
    OS_EXCEPTION_SAVE_GPRS(context)
    mfdsisr r5
    mfdar   r6

    stwu r1, -8(r1)
    b       __OSUnhandledException
}
#endif

#ifdef __GEKKO__
void __OSPSInit(void) {
	PPCMthid2(PPCMfhid2() | 0x80000000 | 0x20000000);
	ICFlashInvalidate();
	__sync();

    asm
    {
        li      r3, 0
        mtspr   GQR0, r3
        mtspr   GQR1, r3
        mtspr   GQR2, r3
        mtspr   GQR3, r3
        mtspr   GQR4, r3
        mtspr   GQR5, r3
        mtspr   GQR6, r3
        mtspr   GQR7, r3
    }
}
#endif

u32 __OSGetDIConfig(void) {
    return (__DIRegs[9] & 0xFF);
}

void OSRegisterVersion(const char* id) {
    OSReport("%s\n", id);
}

#if SDK_SEP2006
static const char* AppGameNameForSysMenu = "HAEA";

const char* OSGetAppGamename(void) {
    const char* appNameSrc = (char*)OSPhysicalToCached(0x3180);

    if (__OSInIPL) {
        appNameSrc = AppGameNameForSysMenu;
    }

    GameNameBuffer[0] = *appNameSrc++;
    GameNameBuffer[1] = *appNameSrc++;
    GameNameBuffer[2] = *appNameSrc++;
    GameNameBuffer[3] = *appNameSrc;
    GameNameBuffer[4] = 0x00;
    return GameNameBuffer;
}
#endif

u8 OSGetAppType(void) {
    if (__OSInIPL) {
        return 0x40;
    }

    return *((u8*)OSPhysicalToCached(0x3184)) & 0xFF;
}
