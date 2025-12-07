#include <revolution.h>
#include <revolution/os.h>
#include <revolution/esp.h>

#include "__os.h"
#include "__dvd.h"

// extern
extern size_t wcslen(const wchar_t *);
extern int __DVDLayoutFormat;

extern volatile u32 BOOT_REGION_START AT_ADDRESS(0x812FDFF0);
extern volatile u32 BOOT_REGION_END AT_ADDRESS(0x812FDFEC);
extern volatile u8 g_unk_800030E2 AT_ADDRESS(0x800030E2);

DVDDiskID id;

static int Prepared;
BOOL __OSInReboot;
u32 __OSNextPartitionType = 0;

static int PackArgs(void* addr, s32 argc, char** argv) {
    s32 numArgs;
    char* bootInfo2;
    char* ptr;
    char** list;
    u32 i;

    bootInfo2 = (char*)addr;
    memset(bootInfo2, 0, 0x2000);

    if (argc == 0) {
        *(u32*)(bootInfo2 + 8) = 0;
    } else {
        numArgs = argc;
        ptr = bootInfo2 + 0x2000;
        while (--argc >= 0) {
            ptr -= strlen(argv[argc]) + 1;
            strcpy(ptr, argv[argc]);
            argv[argc] = (char*)(ptr - bootInfo2);
        }

        ptr = bootInfo2 + ((ptr - bootInfo2) & ~0x3);
        ptr -= ((numArgs + 1) * 4);
        list = (char**)ptr;

        for (i = 0; i < numArgs + 1; i++) {
            list[i] = argv[i];
        }

        ptr -= 4;
        *(u32*)ptr = numArgs;

        ASSERTMSGLINE(401, ptr - bootInfo2 >= 0x1000U, "OSExec: Argument list is too long");

        *(u32*)(bootInfo2 + 8) = (ptr - bootInfo2);
    }

    return 1;
}

static BOOL Utf16ToArg(char* dstArg, u16* srcName) {
    char* srcPtr, *dstPtr;
    u8 i, mask, shift;

    if (srcName != 0) {
        srcPtr = (char*)srcName;
        dstPtr = dstArg;

        while (*srcPtr || *(srcPtr + 1)) {
            for (i = 0; i < 4; i++) {
                mask = (u8)((i & 0x1) ? 0xF : 0xF0);
                shift = (u8)((i & 0x1) ? 0 : 4);

                if (0 <= ((*srcPtr & mask) >> shift) && ((*srcPtr & mask) >> shift) < 0xA) {
                    *dstPtr = (char)(((*srcPtr & mask) >> shift) + 0x30);
                }
                else if(0xA <= ((*srcPtr & mask) >> shift) && ((*srcPtr & mask) >> shift) < 0x10) {
                    *dstPtr = (char)(((*srcPtr & mask) >> shift) + 0x57);
                }
                else {
                    return FALSE;
                }

                dstPtr++;

                if (i & 1) {
                    srcPtr++;
                }
            }
        }

        *dstPtr++ = 0;
        return TRUE;
    }

    return FALSE;
}

BOOL PackInstallerArgs(void* addr, s32 argc, char* argv[]) {
    s32 numArgs;
    char* bootInfo2;
    char* ptr;
    char** list;
    u32 i;

    bootInfo2 = (char*)addr;
    memset(bootInfo2, 0, 0x2000);

    if (argc == 0) {
        *(u32*)(bootInfo2 + 8) = 0;
    } else {
        numArgs = argc;
        ptr = bootInfo2 + 0x2000;

        while (--argc >= 0) {
            if (argc < 2 || (argc % 2)) {
                ptr -= strlen(argv[argc]) + 1;
                strcpy(ptr, argv[argc]);
                argv[argc] = (char*)(ptr - bootInfo2);
            } else {
                ptr -= (wcslen((u16*)argv[argc]) * 4 + 1);
                Utf16ToArg(ptr, (u16*)argv[argc]);
                argv[argc] = (char*)(ptr - bootInfo2);
            }
        }

        ptr = bootInfo2 + ((ptr - bootInfo2) & ~0x3);
        ptr -= ((numArgs + 1) * 4);
        list = (char**)ptr;

        for (i = 0; i < numArgs + 1; i++) {
            list[i] = argv[i];
        }

        ptr -= 4;
        *(u32*)ptr = numArgs;

        ASSERTMSGLINE(524, ptr - bootInfo2 >= 0x1000U, "OSExec: Argument list is too long");

        *(u32*)(bootInfo2 + 8) = (ptr - bootInfo2);
    }

    return TRUE;
}

#ifdef __GEKKO__
static asm void Run(__REGISTER void* entryPoint) {
    fralloc

    bl ICFlashInvalidate
    sync
    isync
    mtctr entryPoint
    bctr

    frfree
    blr
}
#endif

static void StartDol(const OSExecParams* params, void* entry) {
    OSExecParams* paramsWork = OSAllocFromMEM1ArenaLo(sizeof(OSExecParams), 1);

    __OSSetExecParams(params, paramsWork);
    __PIRegs[9] = 7;

    OSDisableInterrupts();
    Run(entry);
}

static void ReadDisc(void* addr, s32 length, s32 offset) {
    DVDCommandBlock block;
    // NOTE: unsure what these are, but required to match debug
    volatile u32 sp18 = 0;
    u8 sp14[] = {0xFF, 0xFF, 0xFF, 0x00};

    DVDReadAbsAsyncPrio(&block, addr, length, offset, NULL, 0);

    while (DVDGetCommandBlockStatus(&block)) {
        if (DVDGetCommandBlockStatus(&block) > 2 || DVDGetCommandBlockStatus(&block) < 0) {
            __OSReturnToMenuForError();
        }
    }
}

static void Callback(s32, DVDCommandBlock*) {
    Prepared = TRUE;
}

static int IsStreamEnabled() {
    if (DVDGetCurrentDiskID()->streaming) {
        return TRUE;
    }

    return FALSE;
}

void __OSGetExecParams(OSExecParams* params) {
    if (0x80000000 <= (u32)__OSExecParams) {
        memcpy(params, __OSExecParams, sizeof(OSExecParams));
    } else {
        params->valid = FALSE;
    }
}

void __OSSetExecParams(const OSExecParams* params, OSExecParams* addr) {
    memcpy(addr, params, sizeof(OSExecParams));
    __OSExecParams = addr;
}

static void StopStreaming() {
    DVDCommandBlock block;

    if (!__OSIsGcam && IsStreamEnabled()) {
        AISetStreamVolLeft(0);
        AISetStreamVolRight(0);
        DVDCancelStreamAsync(&block, NULL);


        while (DVDGetCommandBlockStatus(&block)) {
            if (!DVDCheckDisk()) {
                __OSDoHotReset(0);
            }
        }

        AISetStreamPlayState(0);
    }
}

static int GetApploaderPosition(void) {
    static s32 apploaderPosition;

    if (apploaderPosition != 0) {
        return apploaderPosition;
    }

    if (__OSAppLoaderOffset != 0) {
        u32* tgcHeader;
        s32 apploaderOffsetInTGC;

        tgcHeader = OSAllocFromMEM1ArenaLo(0x40, DOLPHIN_ALIGNMENT);
        ReadDisc(tgcHeader, 0x40, __OSAppLoaderOffset >> 2);
        apploaderOffsetInTGC = tgcHeader[14];
        ASSERTMSGLINE(727, apploaderOffsetInTGC != 0, "OSExec() or OSResetSystem(): Wrong apploader offset. Maybe converted by an\nolder version of gcm2tgc. Use gcm2tgc v.1.20 or later.");

        apploaderPosition = (__OSAppLoaderOffset + apploaderOffsetInTGC) >> 2;
    } else {
        apploaderPosition = 0x910;
    }

    return apploaderPosition;
}

typedef struct {
    char date[16];
    u32 entry;
    u32 size;
    u32 rebootSize;
    u32 reserved2;
} AppLoaderStruct;

static AppLoaderStruct* LoadApploader() {
    AppLoaderStruct* header;

    header = (AppLoaderStruct*)OSAllocFromMEM1ArenaLo(sizeof(AppLoaderStruct), DOLPHIN_ALIGNMENT);
    ReadDisc(header, sizeof(AppLoaderStruct), GetApploaderPosition());
    ASSERTMSGLINE(758, header->rebootSize != 0, "OSResetSystem(): old apploader");

    ReadDisc((void*)0x81200000, OSRoundUp32B(header->size), GetApploaderPosition() + 0x8);
    ICInvalidateRange((void*)0x81200000, OSRoundUp32B(header->size));
    return header;
}

static void* LoadDol(const OSExecParams* params, AppLoaderCallback getInterface) {
    appInitCallback appInit;
    appGetNextCallback appGetNext;
    appGetEntryCallback appGetEntry;
    void* addr;
    u32 length;
    u32 offset;
    OSExecParams* paramsWork;

    getInterface(&appInit, &appGetNext, &appGetEntry);
    paramsWork = (OSExecParams*)OSAllocFromMEM1ArenaLo(sizeof(OSExecParams), 1);
    __OSSetExecParams(params, paramsWork);
    appInit((void(*)(char*))OSReport);
    OSSetArenaLo(paramsWork);

    while (appGetNext(&addr, &length, &offset) != 0) {
        ReadDisc(addr, length, offset >> __DVDLayoutFormat);
    }

    return appGetEntry();
}

static BOOL IsNewApploader(AppLoaderStruct* header) {
    if (strncmp(header->date, "2004/02/01", 10) > 0) {
        return TRUE;
    } else {
        return FALSE;
    }
}

static u32 DVDLowIntType;

static void callback(u32 param_0) {
    DVDLowIntType = param_0;
}

void CheckDVDLowIntType(int param_0) {
    switch (param_0) {
    case 1:
        break;
    case 2:
        OSReport("\nDisc error occurred!\n");
        __OSReturnToMenuForError();
        break;
    case 16:
        OSReport("\nTimeout error occurred!\n");
        __OSReturnToMenuForError();
        break;
    default:
        OSReport("\nUnexpected error occurred!\n");
        __OSReturnToMenuForError();
    }
}

// NONMATCHING
void __OSLaunchNextFirmware(void) {
    DVDPartitionInfo* var_r30;
    int rc = -1;
    DVDPartitionInfo* var_r28;
    u32 var_r27;

    u32 ticketCnt = 1;
    ESSysVersion version = 0x100000003;
    ESTicketView* sp30;
    ESTicketView* ticket;

    u32 MEM2Size;
    u32 MEM2End;
    DVDGameTOC* gameTOC;
    DVDPartitionInfo* partitionInfo;
    ESTitleMeta* meta;
    u32 tmd = 0;

    gameTOC = OSAllocFromMEM1ArenaLo(0x20, 0x20);
    partitionInfo = OSAllocFromMEM1ArenaLo(0x800, 0x20);
    meta = OSAllocFromMEM1ArenaLo(0x4A00, 0x40);
    ticket = OSAllocFromMEM1ArenaLo(0xE0, 0x20);

    if (__OSNextPartitionType == *(u32*)0x80003194 && *(u32*)OSPhysicalToCached(0x3198) != 0) {
        rc = ESP_InitLib();
        if (!rc) {
            rc = ESP_DiGetTicketView(NULL, ticket);
        }

        if (!rc) {
            rc = ESP_DiGetTmd(NULL, &tmd);
        }

        if (!rc) {
            rc = ESP_DiGetTmd(meta, &tmd);
        }

        ESP_CloseLib();

        if (OSPlayTimeIsLimited()) {
            __OSPlayTimeType type = OSPLAYTIME_PERMANENT;
            u32 playTime = -1;
            __OSGetPlayTime(ticket, &type, &playTime);
            if (playTime == 0) {
                __OSWriteExpiredFlag();
                __OSReturnToMenuForError();
            }
        }
    }

    
    if (!rc) {
        var_r30 = partitionInfo;
        var_r30->type = *(u32*)OSPhysicalToCached(0x3194);
        var_r30->gamePartition = (DVDGamePartition*)*(u32*)OSPhysicalToCached(0x3198);
    } else {
        u8 i;
        DVDLowIntType = 0;
        DVDLowClosePartition(callback);
        while (DVDLowIntType == 0) {}

        CheckDVDLowIntType(DVDLowIntType);
        DVDLowIntType = 0;
        DVDLowUnencryptedRead(&gameTOC->numGamePartitions, 0x20, 0x10000, callback);
        while (DVDLowIntType == 0) {}

        CheckDVDLowIntType(DVDLowIntType);
        DVDLowIntType = 0;
        DVDLowUnencryptedRead(partitionInfo, 0x800, (u32)gameTOC->partitionInfos, callback);
        while (DVDLowIntType == 0) {}

        CheckDVDLowIntType(DVDLowIntType);

        var_r30 = NULL;
        var_r28 = partitionInfo;

        for (i = 0; i < gameTOC->numGamePartitions; i++) {
            if (var_r28->type == __OSNextPartitionType) {
                var_r30 = var_r28;
            }

            var_r28++;
        }

        if (var_r30 == 0) {
            gameTOC++;
            var_r28 = partitionInfo + 4;

            for (i = 0; i < gameTOC->numGamePartitions; i++) {
                if (var_r28->type == __OSNextPartitionType) {
                    var_r30 = var_r28;
                }

                var_r28 += 2;
            }
        }

        if (var_r30 == 0) {
            OSReport("\nThe specified game doesn\'t exist in the disc\n");
            __OSReturnToMenuForError();
        }

        var_r27 = var_r30->type;
        *(u32*)OSPhysicalToCached(0x3194) = var_r27;

        var_r27 = (u32)var_r30->gamePartition;
        *(u32*)OSPhysicalToCached(0x3198) = var_r27;

        DVDLowIntType = 0;

        if (*(u8*)0x80003187 == 0x80) {
            DVDLowOpenPartitionWithTmdAndTicketView((u32)var_r30->gamePartition, ticket, tmd, meta, 0, 0, callback);
        } else {
            DVDLowOpenPartition((u32)var_r30->gamePartition, NULL, 0, NULL, meta, callback);
        }

        while (DVDLowIntType == 0) {}
        CheckDVDLowIntType(DVDLowIntType);
        DVDLowIntType = 0;
        DVDLowClosePartition(callback);
        
        while (DVDLowIntType == 0) {}
        CheckDVDLowIntType(DVDLowIntType);
    }

    version = meta->head.sysVersion;

    rc = ESP_InitLib();
    if (rc) {
        OSReport("\nOSExec(): Failed to exec %d in %d\n", rc, 1037);
        __OSHotResetForError();
    }

    rc = ESP_GetTicketViews(version, NULL, &ticketCnt);
    if (ticketCnt != 1 || rc) {
        OSReport("\nOSExec(): Failed to exec %d in %d\n", rc, 1046);
        __OSHotResetForError();
    }

    sp30 = OSAllocFromMEM1ArenaLo(OSRoundUp32B(ticketCnt * sizeof(ESTicketView)), 0x20);
    rc = ESP_GetTicketViews(version, sp30, &ticketCnt);
    if (rc) {
        OSReport("\nOSExec(): Failed to exec %d in %d\n", rc, 1055);
        __OSHotResetForError();
    }

    DVDLowFinalize();

    MEM2Size = *(u32*)OSPhysicalToCached(0x311C);
    MEM2End = *(u32*)OSPhysicalToCached(0x3120);
    DCStoreRange(OSPhysicalToCached(0x3100), 0x100);

    rc = ESP_LaunchTitle(version, sp30);
    if (rc) {
        OSReport("\nOSExec(): Failed to exec %d in %d\n", rc, 1071);
        __OSHotResetForError();
    }

    ESP_CloseLib();
    DCInvalidateRange(OSPhysicalToCached(0x3100), 0x100);

    if (MEM2Size < *(u32*)OSPhysicalToCached(0x311C)) {
        var_r27 = *(u32*)OSPhysicalToCached(0x311C);
        var_r27 = MEM2Size - (var_r27 - *(u32*)OSPhysicalToCached(0x3120));
        *(u32*)OSPhysicalToCached(0x3120) = var_r27;

        var_r27 = *(u32*)OSPhysicalToCached(0x311C);
        var_r27 = MEM2Size - (var_r27 - *(u32*)OSPhysicalToCached(0x3128));
        *(u32*)OSPhysicalToCached(0x3128) = var_r27;

        var_r27 = *(u32*)OSPhysicalToCached(0x311C);
        var_r27 = MEM2Size - (var_r27 - *(u32*)OSPhysicalToCached(0x3130));
        *(u32*)OSPhysicalToCached(0x3130) = var_r27;

        var_r27 = *(u32*)OSPhysicalToCached(0x311C);
        var_r27 = MEM2Size - (var_r27 - *(u32*)OSPhysicalToCached(0x3134));
        *(u32*)OSPhysicalToCached(0x3134) = var_r27;

        var_r27 = MEM2Size;
        *(u32*)OSPhysicalToCached(0x311C) = var_r27;
    }

    if (MEM2End < *(u32*)OSPhysicalToCached(0x3120)) {
        __OSInitMemoryProtection();
    }

    __OSInitIPCBuffer();
    IPCReInit();
    IPCCltReInit();

    DVDLowInit();
    DVDLowIntType = 0;
    DVDLowReadDiskID(&id, callback);
    while (DVDLowIntType == 0) {}
    CheckDVDLowIntType(DVDLowIntType);
    DVDLowIntType = 0;

    if (*(u8*)0x80003187 == 0x80) {
        DVDLowOpenPartitionWithTmdAndTicketView((u32)var_r30->gamePartition, ticket, tmd, meta, 0, 0, callback);
    } else {
        DVDLowOpenPartition((u32)var_r30->gamePartition, NULL, 0, NULL, meta, callback);
    }

    while (DVDLowIntType == 0) {}

    CheckDVDLowIntType(DVDLowIntType);
}

#define TITLE_ID ((((u64)1) << 32) | (2))

void __OSLaunchMenu(void) {
    u8 i;
    s32 rc;
    u32 ticketCnt = 1;
    ESTicketView* t;
    ESSysVersion version = 0x0000000100000003;
    GXColor bg = { 0, 0, 0, 0};
    GXColor fg = { 255, 255, 255, 0 };

    OSSetArenaLo((void*)0x81280000);
    OSSetArenaHi((void*)0x812F0000);
    rc = ESP_InitLib();

    if (rc != 0) {
        return;
    }

    rc = ESP_GetTicketViews(TITLE_ID, NULL, &ticketCnt);

    if (ticketCnt != 1 || rc != 0) {
        return;
    }

    t = (ESTicketView*)OSAllocFromMEM1ArenaLo(OSRoundUp32B(sizeof(ESTicketView) * ticketCnt), 32);
    rc = ESP_GetTicketViews(TITLE_ID, t, &ticketCnt);

    if (rc != 0) {
        return;
    }

    rc = ESP_LaunchTitle(TITLE_ID, t);

    if (rc != 0) {
        return;
    }

    while (1) { 

    }
}

// NONMATCHING - stack issues
void __OSBootDolSimple(u32 doloffset, u32 restartCode, void* regionStart, void* regionEnd, BOOL argsUseDefault, s32 argc, char** argv) {
    OSExecParams* params;
    void* dolEntry;
    OSBootInfo* bootInfo;
    char* sp20;
    AppLoaderStruct* header;

    OSDisableInterrupts();

    if (__OSInReboot) {
        __OSNextPartitionType = *(u32*)0x80003194;
    }

    __OSRestoreCodeExecOnMEM1(0xBA2CF);

    params = (OSExecParams*)OSAllocFromMEM1ArenaLo(sizeof(OSExecParams), 1);
    params->valid = TRUE;
    params->restartCode = restartCode;
    params->regionStart = regionStart;
    params->regionEnd = regionEnd;
    params->argsUseDefault = argsUseDefault;

    if (!argsUseDefault) {
        params->argsAddr = OSAllocFromMEM1ArenaLo(0x2000, 1);

        if (__OSNextPartitionType == 2 && !__OSInReboot) {
            PackInstallerArgs(params->argsAddr, argc, argv);
        } else {
            PackArgs(params->argsAddr, argc, argv);
        }
    }

    DVDInit();
    DVDSetAutoInvalidation(TRUE);
    DVDResume();

    Prepared = FALSE;

    __DVDPrepareResetAsync(Callback);
    __OSMaskInterrupts(0xFFFFFFF0);
    __OSUnmaskInterrupts(0x10);
    OSEnableInterrupts();

    while (Prepared != TRUE) {}

    __OSLaunchNextFirmware();

    if (restartCode == 0xA0000000 && !__OSInReboot) {
        ESTitleId titleID;
        int rc;

        rc = ESP_InitLib();
        if (rc) {
            return;
        }

        rc = ESP_GetTitleId(&titleID);
        if (rc) {
            return;
        }

        rc = ESP_CloseLib();
        if (rc) {
            return;
        }

        sp20 = (u32)params->argsAddr + argv[1];
        snprintf(sp20, 17, "%016llx", titleID);
    }

    header = LoadApploader();
    if (IsNewApploader(header)) {
        if (doloffset == 0xFFFFFFFF) {
            doloffset = GetApploaderPosition() + ((header->size + 0x20) >> 2);
        }

        params->bootDol = doloffset;
        dolEntry = LoadDol(params, (AppLoaderCallback)header->entry);

        bootInfo = OSPhysicalToCached(0);
        *(u32*)OSPhysicalToCached(0x3180) = *(u32*)bootInfo->DVDDiskID.gameName;
        *(u8*)OSPhysicalToCached(0x3184) = 0x80;

        StartDol(params, dolEntry);
    } else {
        BOOT_REGION_START = (u32)regionStart;
        BOOT_REGION_END = (u32)regionEnd;
        g_unk_800030E2 = 1;

        ReadDisc((void*)0x81330000, OSRoundUp32B(header->rebootSize), GetApploaderPosition() + ((header->size + 0x20) >> 2));
        ICInvalidateRange((void*)0x81330000, OSRoundUp32B(header->rebootSize));
        OSDisableInterrupts();
        ICFlashInvalidate();
        Run((void*)0x81330000);
    }
}

void __OSBootDol(u32 doloffset, u32 restartCode, const char** argv) {
    char doloffInString[20];
    s32 argvlen;
    char** argvToPass;
    s32 i;
    void* saveStart;
    void* saveEnd;

    OSGetSaveRegion(&saveStart, &saveEnd);
    sprintf(doloffInString, "%d", doloffset);
    argvlen = 0;

    if (argv != 0) {
        while (argv[argvlen] != 0) {
            argvlen++;
        }
    }

    argvlen++;
    argvToPass = OSAllocFromMEM1ArenaLo((argvlen + 1) * 4, 1);
    *argvToPass = doloffInString;

    for (i = 1; i < argvlen; i++) {
        argvToPass[i] = (char*)argv[i - 1];
    }

    __OSBootDolSimple(-1, restartCode, saveStart, saveEnd, FALSE, argvlen, argvToPass);
}

void OSLaunchDisk(void) {
    // NOTE: Function doesn't exist in TP debug, but string data is leftover
    OSReport("OSLaunchDisk(): You can't call this API from DVD application.  \n");
    OSReport("OSLaunchDisk(): You must call DVDPrepareDisk before launching disc.\n");
    OSReport("OSLaunchDisk(): Specified id is different from one specified to DVDPrepareDisk.\n");
}

void OSLaunchDiskl(void) {
    // NOTE: Function doesn't exist in TP debug, but string data is leftover
    OSReport("0000000000000000");
    OSReport("OSLaunchDiskl(): Arguments too long");
}

static void ExecCommon(const char* dolfile, const char** argv) {
    DVDFileInfo fileInfo;
    u32 doloff;

    if ((s8)*dolfile == '\0') {
        doloff = 0;
    } else if (DVDOpen((char*)dolfile, &fileInfo)) {
        doloff = fileInfo.startAddr;
    } else {
        ASSERTMSGLINE(0, 0, "Warning: OSExec(): The specified file doesn't exist. \n");
        return;
    }

    __OSBootDol(doloff, 0xC0000000, argv);
}

void OSExecv(const char* dolfile, const char** argv) {
    ASSERTMSGLINE(0, dolfile != 0, "OSExecv(): null pointer was specified for the dol file name.");
    ASSERTMSGLINE(0, argv != 0, "OSExecv(): null pointer was specified for argv.");

    OSDisableScheduler();
    __OSShutdownDevices(FALSE);
    OSEnableScheduler();
    OSSetArenaLo((void*)0x81280000);
    OSSetArenaHi((void*)0x812F0000);
    ExecCommon(dolfile, argv);
}

void OSExecl(const char* dolfile, const char* arg0, ...) {
    va_list vl;
    char* ptr;
    s32 i;
    char** argv;

    ASSERTMSGLINE(0, dolfile != 0, "OSExecl(): null pointer was specified for the dol file name.");

    OSDisableScheduler();
    __OSShutdownDevices(FALSE);
    OSEnableScheduler();
    OSSetArenaLo((void*)0x81280000);
    OSSetArenaHi((void*)0x812F0000);

    argv = OSAllocFromMEM1ArenaLo(4, 0x1000);
    va_start(vl, arg0);

    i = 0;
    ptr = (char*)arg0;
    goto setarg;

    do {
        ptr = va_arg(vl, char*);
setarg:
        argv[i++] = ptr;
    } while (ptr != 0);

    va_end(vl);
    ASSERTMSGLINE(0, i < 0x400U, "OSExecl(): Arguments too long");

    ExecCommon(dolfile, (const char**)argv);
}

void OSLaunchPartition(void) {
    // NOTE: Function doesn't exist in TP debug, but string data is leftover
    OSReport("OSLaunchPartition(): You can't call this API from NAND application.  \n");
    OSReport("OSLaunchPartition(): Specified title ID is not DISC application.  \n");
    OSReport("/title/%08x/%08x");
    OSReport("\nOSLaunchPartition(): Failed to get free i-nodes number\n");
    OSReport("\nOSLaunchPartition(): There are not enough i-nodes to launch next partition\n");
    OSReport("\nOSLaunchPartition(): Failed to get nand status\n");
}

void OSLaunchPartitionl(void) {
    // NOTE: Function doesn't exist in TP debug, but string data is leftover
    OSReport("OSLaunchPartitionl(): Arguments too long");
}
