#include <revolution/os.h>
#include <revolution/esp.h>
#include <revolution/ipc.h>

IOSFd DiFD = -1;

static volatile u8 requestInProgress = FALSE;
static u8 breakRequested;
static u8 callbackInProgress;

static u32 registerBuf[8] ATTRIBUTE_ALIGN(32);
static u32 statusRegister[8] ATTRIBUTE_ALIGN(32);
static u32 controlRegister[8] ATTRIBUTE_ALIGN(32);
static s32 lastTicketError[8] ATTRIBUTE_ALIGN(32);

static u32 readLength;
static u32 spinUpValue;

static diRegVals_t diRegValCache ATTRIBUTE_ALIGN(32);

static u8 DVDLowInitCalled = FALSE;

typedef enum callbackType {
    BOGUS_TYPE = 0,
    TRANSACTION_CB,
    COVER_CB,
    COVER_REG_CB
} callbackType_t;

static int freeCommandBuf = 0;
static diCommand_t* diCommand;
static char* pathBuf;

typedef struct dvdContext {
    DVDLowCallback callback;
    callbackType_t callbackType;
    u8 inUse; 
    u32 contextMagic;
    u32 contextNum;
    u32 pad[3];
} dvdContext_t;

static int freeDvdContext = 0;
static u8 dvdContextsInited = FALSE;
static dvdContext_t dvdContexts[4] ATTRIBUTE_ALIGN(32);
static IOSIoVector ioVec[10] ATTRIBUTE_ALIGN(32);

static void* ddrAllocAligned32(const int size) {
    void* low, *high;

    if ((size & 0x1F) != 0) {
        OSReport("(ddrAllocAligned32) size is not a multiple of 32 !!! \n");
        return 0;
    }

    low = IPCGetBufferLo();
    high = IPCGetBufferHi();

    if (((u32)low & 0x1F) != 0) {
        low = (void*)(((u32)low + 31) & 0x1F);
    }

    if ((u32)low + size > (u32)high) {
        OSReport("(ddrAllocAligned32) Not enough space to allocate %d bytes\n", size);
    }

    IPCSetBufferLo((void*)((u32)low + size));
    return low;
}

static BOOL allocateStructures(void) {
    if ((diCommand = ddrAllocAligned32(sizeof(diCommand_t) * 4)) == 0) {
        OSReport("Allocation of diCommand blocks failed\n");
        return FALSE;
    }

    if ((pathBuf = ddrAllocAligned32(32)) == 0) {
        OSReport("Allocation of pathBuf failed\n");
        return FALSE;
    }

    return TRUE;    
}

static void initDvdContexts(void) {
    {
        u32 i;

        for (i = 0; i < 4; i++) {
            dvdContexts[i].callback = 0;
            dvdContexts[i].callbackType = 0;
            dvdContexts[i].inUse = FALSE;
            dvdContexts[i].contextMagic = 0xFEEBDAED;
            dvdContexts[i].contextNum = i;
        }
    }

    freeDvdContext = 0;
}

IOSError doTransactionCallback(IOSError ret, void* context) {
    dvdContext_t* dvdContext = context;

    if (dvdContext->contextMagic != 0xFEEBDAED) {
        OSReport("(doTransactionCallback) Error - context mangled!\n");
        dvdContext->contextMagic = 0xFEEBDAED;
        goto out;
    }

    requestInProgress = FALSE;

    if (dvdContext->callback != NULL) {
        int callbackArg;
        callbackInProgress = TRUE;
        callbackArg = ret;

        if (breakRequested == TRUE) {
            breakRequested = FALSE;
            callbackArg |= 8;
        }

        if (callbackArg & 1) {
            readLength = 0;
        }

        dvdContext->callback((u32)callbackArg);
        callbackInProgress = FALSE;
    }

out:
    dvdContext->inUse = FALSE;
    return 0;
}

// NOTE: function doesn't exist in TP debug, needed for string data
IOSError doCoverCallback(IOSError err, void* context) {
    OSReport("(doCoverCallback) Error - context mangled!\n");
    return 0;
}

IOSError doPrepareCoverRegisterCallback(IOSError err, void* context) {
    dvdContext_t* dvdContext;
    requestInProgress = FALSE;

    diRegValCache.CoverRegVal = registerBuf[0];
    dvdContext = (dvdContext_t*)context;

    if (dvdContext->contextMagic != 0xFEEBDAED) {
        OSReport("(doTransactionCallback) Error - context mangled!\n");
        dvdContext->contextMagic = 0xFEEBDAED;
    } else {
        IOSError ret;
        if (dvdContext->callback != 0) {
            callbackInProgress = TRUE;
            ret = err;

            if (breakRequested == TRUE) {
                breakRequested = FALSE;
                ret |= 8;
            }

            dvdContext->callback(ret);
            callbackInProgress = FALSE;
        }
    }

    dvdContext->inUse = FALSE;
    return 0;
}

static inline void nextCommandBuf(int* bufNum) {
    (*bufNum)++;

    if (*bufNum >= 4) {
        *bufNum = 0;
    }
}

static inline dvdContext_t* newContext(const DVDLowCallback callback, const callbackType_t type) {
    int returnIndex;

    if ((dvdContexts[freeDvdContext].inUse != 0) == (u32)1)  {
        OSReport("(newContext) ERROR: freeDvdContext.inUse (#%d) is true\n", freeDvdContext);
        OSReport("(newContext) Now spinning in infinite loop\n");

        while (1) {

        }
    }

    if (dvdContexts[freeDvdContext].contextMagic != 0xFEEBDAED) {
        OSReport("(newContext) Something overwrote the context magic - spinning \n");
    
        while (1) {

        }
    }

    dvdContexts[freeDvdContext].callback = callback;
    dvdContexts[freeDvdContext].callbackType = type;
    dvdContexts[freeDvdContext].inUse = TRUE;
    returnIndex = freeDvdContext;
    freeDvdContext++;

    if (freeDvdContext >= 4) {
        freeDvdContext = 0;
    }

    return(dvdContexts + returnIndex);
}

BOOL DVDLowFinalize(void) {
    IOSError ret = IOS_Close(DiFD);

    if (ret != IOS_ERROR_OK) {
        OSReport("(DVDLowFinish) Error: IOS_Close failed\n");
        return FALSE;
    }

    DVDLowInitCalled = FALSE;
    return TRUE;
}

BOOL DVDLowInit(void) {
    IOSError retVal;

    if (DVDLowInitCalled == FALSE) {
        DVDLowInitCalled = TRUE;
        retVal = IPCCltInit();

        if (retVal != IOS_ERROR_OK) {
            OSReport("IPCCltInit returned error: %d\n", retVal);
            return FALSE;
        }

        if (allocateStructures() == FALSE) {
            return FALSE;
        }

        if (dvdContextsInited == FALSE) {
            initDvdContexts();
            dvdContextsInited = TRUE;
        }
    }

    strncpy(pathBuf, "/dev/di", 32);
    DiFD = IOS_Open(pathBuf, 0);

    if (DiFD >= 0) {
        return TRUE;
    } else {
        switch (DiFD) {
        case IOS_ERROR_NOEXISTS:
            OSReport("(DVDLowInit) Error: IOS_Open failed - pathname '/dev/di' does not exist\n");
            return FALSE;
            break;
        case IOS_ERROR_ACCESS:
            OSReport("(DVDLowInit) Error: IOS_Open failed - calling thread lacks permission\n");
            return FALSE;
            break;
        case IOS_ERROR_MAX:
            OSReport("(DVDLowInit) Error: IOS_Open failed - connection limit has been reached\n");
            return FALSE;
            break;
        default:
            OSReport("(DVDLowInit) IOS_Open failed, errorcode = %d\n", DiFD);
            return FALSE;
            break;
        }
    }
}

BOOL DVDLowReadDiskID(DVDDiskID* diskID, DVDLowCallback callback) {
    dvdContext_t* dvdContext;
    IOSError rv;
    requestInProgress = TRUE;
    dvdContext = newContext(callback, 1);

    if (diskID == 0) {
        OSReport("@@@@@@ WARNING - Calling DVDLowReadDiskId with NULL ptr\n");
    }

    nextCommandBuf(&freeCommandBuf);
    diCommand[freeCommandBuf].theCommand = 0x70;

    rv = IOS_IoctlAsync(DiFD, 0x70, &diCommand[freeCommandBuf], sizeof(diCommand_t), diskID, sizeof(DVDDiskID), doTransactionCallback, dvdContext);

    if (rv != IOS_ERROR_OK) {
        OSReport("@@@ (DVDLowReadDiskID) IOS_IoctlAsync returned error: %d\n", rv);
        dvdContext->inUse = FALSE;
        return FALSE;
    }

    return TRUE;
}

BOOL DVDLowOpenPartition(const u32 partitionWordOffset, const ESTicket* const eTicket, const u32 numCertBytes, const u8* const certificates, ESTitleMeta* tmd, DVDLowCallback callback) {
    dvdContext_t* dvdContext;
    IOSError rv;
    
    if (eTicket != 0 && ((u32)eTicket & 0x1F)) {
        OSReport("(DVDLowOpenPartition) eTicket memory is unaligned\n");
        return FALSE;
    }

    if (certificates != 0 && ((u32)certificates & 0x1F)) {
        OSReport("(DVDLowOpenPartition) certificates memory is unaligned\n");
        return FALSE;
    }

    if (tmd != 0 && ((u32)tmd & 0x1F)) {
        OSReport("(DVDLowOpenPartition) certificates memory is unaligned\n");
        return FALSE;
    }

    requestInProgress = TRUE;
    dvdContext = newContext(callback, 1);
    nextCommandBuf(&freeCommandBuf);
    diCommand[freeCommandBuf].theCommand = 0x8B;
    diCommand[freeCommandBuf].arg[0] = partitionWordOffset;
    ioVec[0].base = (u8*)&diCommand[freeCommandBuf];
    ioVec[0].length = sizeof(diCommand_t);

    ioVec[1].base = (u8*)eTicket;
    if (eTicket == 0) {
        ioVec[1].length = 0;
    } else {
        ioVec[1].length = sizeof(ESTicket);
    }

    ioVec[2].base = (u8*)certificates;
    if (certificates == 0) {
        ioVec[2].length = 0;
    } else {
        ioVec[2].length = numCertBytes; 
    }

    ioVec[3].base = (u8*)tmd;
    ioVec[3].length = sizeof(ESTitleMeta);

    ioVec[4].base = (u8*)&lastTicketError[0];
    ioVec[4].length = sizeof(lastTicketError);

    rv = IOS_IoctlvAsync(DiFD, 0x8B, 3, 2, ioVec, doTransactionCallback, dvdContext);

    if (rv != IOS_ERROR_OK) {
        OSReport("@@@ (DVDLowOpenPartition) IOS_IoctlvAsync returned error: %d\n", rv);
        dvdContext->inUse = FALSE;
        return FALSE;
    }

    return TRUE;
}

// NOTE: function doesn't exist in TP debug, needed for string data
BOOL DVDLowOpenPartitionWithTmdAndTicket() {
    OSReport("(%s) eTicket memory is unaligned\n", __FUNCTION__);
    OSReport("(%s) tmd parameter cannot be NULL\n", __FUNCTION__);
    OSReport("(%s) tmd memory is unaligned\n", __FUNCTION__);
    OSReport("(%s) eTicket parameter cannot be NULL\n", __FUNCTION__);
    return TRUE;
}

BOOL DVDLowOpenPartitionWithTmdAndTicketView(const u32 partitionWordOffset, const ESTicketView* const eTicketView, const u32 numTmdBytes, const ESTitleMeta* const tmd, const u32 numCertBytes, const u8* const certificates, DVDLowCallback callback) {
    dvdContext_t* dvdContext;
    IOSError rv;
    
    if (certificates != 0 && ((u32)certificates & 0x1F)) {
        return FALSE;
    }

    if (tmd == 0) {
        OSReport("(%s) tmd parameter cannot be NULL\n", __FUNCTION__);
        return FALSE;
    } else if (((u32)tmd & 0x1F)) {
        OSReport("(%s) tmd memory is unaligned\n", __FUNCTION__);
        return FALSE;
    }

    if (eTicketView == 0) {
        OSReport("(%s) eTicketView parameter cannot be NULL\n", __FUNCTION__);
        return FALSE;
    } else if (((u32)eTicketView & 0x1F)) {
        OSReport("(%s) eTicketView memory is unaligned\n", __FUNCTION__);
        return FALSE;
    }

    requestInProgress = TRUE;
    dvdContext = newContext(callback, 1);
    nextCommandBuf(&freeCommandBuf);
    diCommand[freeCommandBuf].theCommand = 0x94;
    diCommand[freeCommandBuf].arg[0] = partitionWordOffset;
    ioVec[0].base = (u8*)&diCommand[freeCommandBuf];
    ioVec[0].length = sizeof(diCommand_t);

    ioVec[1].base = (u8*)eTicketView;
    ioVec[1].length = sizeof(ESTicketView);

    ioVec[2].base = (u8*)tmd;
    ioVec[2].length = numTmdBytes;

    ioVec[3].base = (u8*)certificates;
    if (certificates == 0) {
        ioVec[3].length = 0;
    } else {
        ioVec[3].length = numCertBytes; 
    }

    ioVec[4].base = (u8*)&lastTicketError[0];
    ioVec[4].length = sizeof(lastTicketError);

    rv = IOS_IoctlvAsync(DiFD, 0x94, 4, 1, ioVec, doTransactionCallback, dvdContext);

    if (rv != IOS_ERROR_OK) {
        OSReport("@@@ (DVDLowOpenPartition) IOS_IoctlvAsync returned error: %d\n", rv);
        dvdContext->inUse = FALSE;
        return FALSE;
    }

    return TRUE;
}

BOOL DVDLowGetNoDiscBufferSizes(const u32 partitionWordOffset, u32* numTmdBytes, u32* numCertBytes, DVDLowCallback callback) {
    dvdContext_t* dvdContext;
    IOSError rv;

    if (numTmdBytes == 0 || numCertBytes == 0) {
        OSReport("(%s) Error: NULL pointer argument\n", __FUNCTION__);
        return FALSE;
    }

    if (((u32)numTmdBytes & 0x1F)) {
        OSReport("(%s) numTmdBytes memory is unaligned\n", __FUNCTION__);
        return FALSE;
    }

    if (((u32)numCertBytes & 0x1F)) {
        OSReport("(%s) certificates memory is unaligned\n", __FUNCTION__);
        return FALSE;
    }

    requestInProgress = TRUE;
    dvdContext = newContext(callback, 1);
    nextCommandBuf(&freeCommandBuf);
    diCommand[freeCommandBuf].theCommand = 0x92;
    diCommand[freeCommandBuf].arg[0] = partitionWordOffset;

    ioVec[0].base = (u8*)&diCommand[freeCommandBuf];
    ioVec[0].length = sizeof(diCommand_t);

    ioVec[1].base = (u8*)numTmdBytes;
    ioVec[1].length = 4;

    ioVec[2].base = (u8*)numCertBytes;
    ioVec[2].length = 4;

    rv = IOS_IoctlvAsync(DiFD, 0x92, 1, 2, ioVec, doTransactionCallback, dvdContext);

    if (rv != IOS_ERROR_OK) {
        OSReport("@@@ (%s) IOS_IoctlvAsync returned error: %d\n", __FUNCTION__, rv);
        dvdContext->inUse = FALSE;
        return FALSE;
    }

    return TRUE;
}

#define is_aligned(addr) (((u32)(addr) & 0x1F) == 0)

BOOL DVDLowGetNoDiscOpenPartitionParams(const u32 partitionWordOffset, ESTicket* eTicket, u32* numTmdBytes, ESTitleMeta* tmd, u32* numCertBytes, u8* certificates, u32* dataWordOffset, u8* h3HashPtr, DVDLowCallback callback) {
    dvdContext_t* dvdContext;
    IOSError rv;

    if (eTicket == 0 || numTmdBytes == 0 || tmd == 0 || numCertBytes == 0 || certificates == 0 || dataWordOffset == 0 || h3HashPtr == 0) {
        OSReport("(%s) Error: NULL pointer argument\n", __FUNCTION__);
        return FALSE;
    }

    if (!is_aligned(eTicket) || !is_aligned(numTmdBytes) || !is_aligned(tmd) || !is_aligned(numCertBytes) || !is_aligned(certificates) || !is_aligned(dataWordOffset) || !is_aligned(h3HashPtr)) {
        OSReport("(%s) pointer argument is unaligned\n", __FUNCTION__);
        return FALSE;
    }

    requestInProgress = TRUE;
    dvdContext = newContext(callback, 1);
    nextCommandBuf(&freeCommandBuf);
    diCommand[freeCommandBuf].theCommand = 0x90;
    diCommand[freeCommandBuf].arg[0] = partitionWordOffset;

    ioVec[0].base = (u8*)&diCommand[freeCommandBuf];
    ioVec[0].length = sizeof(diCommand_t);

    ioVec[1].base = (u8*)numTmdBytes;
    ioVec[1].length = 4;

    ioVec[2].base = (u8*)numCertBytes;
    ioVec[2].length = 4;

    ioVec[3].base = (u8*)eTicket;
    ioVec[3].length = sizeof(ESTicket);

    ioVec[4].base = (u8 *) numTmdBytes;
    ioVec[4].length = 4;

    ioVec[5].base = (u8*)tmd;
    ioVec[5].length = *numTmdBytes;

    ioVec[6].base = (u8*)numCertBytes;
    ioVec[6].length = 4;

    ioVec[7].base = certificates;
    ioVec[7].length = *numCertBytes;

    ioVec[8].base = (u8*)dataWordOffset;
    ioVec[8].length = 4;

    ioVec[9].base = h3HashPtr;
    ioVec[9].length = 98304;

    rv = IOS_IoctlvAsync(DiFD, 0x90, 3, 7, ioVec, doTransactionCallback, dvdContext);

    if (rv != IOS_ERROR_OK) {
        OSReport("@@@ (%s) IOS_IoctlvAsync returned error: %d\n", __FUNCTION__, rv);
        dvdContext->inUse = FALSE;
        return FALSE;
    }

    return TRUE;
}

// NOTE: function doesn't exist in TP debug, needed for string data
BOOL DVDLowNoDiscOpenPartition() {
    OSReport("DVDLowNoDiscOpenPartition");
    return TRUE;
}

BOOL DVDLowClosePartition(DVDLowCallback callback) {
    dvdContext_t* dvdContext;
    IOSError rv;

    nextCommandBuf(&freeCommandBuf);
    diCommand[freeCommandBuf].theCommand = 0x8C;

    requestInProgress = TRUE;
    dvdContext = newContext(callback, 1);
    rv = IOS_IoctlAsync(DiFD, 0x8C, &diCommand[freeCommandBuf], sizeof(diCommand_t), 0, 0, doTransactionCallback, dvdContext);

    if (rv != IOS_ERROR_OK) {
        OSReport("@@@ (DVDLowClosePartition) IOS_IoctlAsync returned error: %d\n", rv);
        dvdContext->inUse = FALSE;
        return FALSE;
    }

    return TRUE;
}

BOOL DVDLowUnencryptedRead(void* destAddr, u32 length, u32 wordOffset, DVDLowCallback callback) {
    dvdContext_t* dvdContext;
    IOSError rv;

    requestInProgress = TRUE;
    dvdContext = newContext(callback, 1);
    readLength = length;
    nextCommandBuf(&freeCommandBuf);
    diCommand[freeCommandBuf].theCommand = 0x8D;
    diCommand[freeCommandBuf].arg[0] = length;
    diCommand[freeCommandBuf].arg[1] = wordOffset;

    rv = IOS_IoctlAsync(DiFD, 0x8D, &diCommand[freeCommandBuf], sizeof(diCommand_t), destAddr, length, doTransactionCallback, dvdContext);

    if (rv != IOS_ERROR_OK) {
        OSReport("@@@ (DVDLowUnencryptedRead) IOS_IoctlAsync returned error: %d\n", rv);
        dvdContext->inUse = FALSE;
        return FALSE;
    }

    return TRUE;
}

BOOL DVDLowStopMotor(u8 eject, u8 saving, DVDLowCallback callback) {
    dvdContext_t* dvdContext;
    IOSError rv;

    requestInProgress = TRUE;
    dvdContext = newContext(callback, 1);
    nextCommandBuf(&freeCommandBuf);
    diCommand[freeCommandBuf].theCommand = 0xE3;
    diCommand[freeCommandBuf].arg[0] = eject;
    diCommand[freeCommandBuf].arg[1] = saving;

    rv = IOS_IoctlAsync(DiFD, 0xE3, &diCommand[freeCommandBuf], sizeof(diCommand_t), &diRegValCache, sizeof(diRegVals_t), doTransactionCallback, dvdContext);

    if (rv != IOS_ERROR_OK) {
        OSReport("@@@ (DVDLowStopMotor) IOS_IoctlAsync returned error: %d\n", rv);
        dvdContext->inUse = FALSE;
        return FALSE;
    }

    return TRUE;
}

// NOTE: function doesn't exist in TP debug, needed for string data
BOOL DVDLowWaitForCoverClose() {
    OSReport("@@@ (DVDLowWaitForCoverClose) IOS_IoctlAsync returned error: %d\n");
    return TRUE;
}

BOOL DVDLowInquiry(DVDDriveInfo* info, DVDLowCallback callback) {
    dvdContext_t* dvdContext;
    IOSError rv;

    if (breakRequested == 0) {}
    requestInProgress = TRUE;
    dvdContext = newContext(callback, 1);
    nextCommandBuf(&freeCommandBuf);
    diCommand[freeCommandBuf].theCommand = 0x12;
    rv = IOS_IoctlAsync(DiFD, 0x12, &diCommand[freeCommandBuf], sizeof(diCommand_t), info, sizeof(DVDDriveInfo), doTransactionCallback, dvdContext);

    if (rv != IOS_ERROR_OK) {
        OSReport("@@@ (DVDLowInquiry) IOS_IoctlAsync returned error: %d\n", rv);
        dvdContext->inUse = FALSE;
        return FALSE;
    }

    return TRUE;
}

BOOL DVDLowRequestError(DVDLowCallback callback) {
    dvdContext_t* dvdContext;
    IOSError rv;

    requestInProgress = TRUE;
    dvdContext = newContext(callback, 1);
    nextCommandBuf(&freeCommandBuf);
    diCommand[freeCommandBuf].theCommand = 0xE0;
    rv = IOS_IoctlAsync(DiFD, 0xE0, &diCommand[freeCommandBuf], sizeof(diCommand_t), &diRegValCache, sizeof(diRegVals_t), doTransactionCallback, dvdContext);

    if (rv != IOS_ERROR_OK) {
        OSReport("@@@ (DVDLowRequestError) IOS_IoctlAsync returned error: %d\n", rv);
        dvdContext->inUse = FALSE;
        return FALSE;
    }

    return TRUE;
}

BOOL DVDLowSetSpinupFlag(u32 spinUp) {
    "(DVDLowSetSpinupFlag): Synch functions can't be called in callbacks\n";  // dead string

    spinUpValue = spinUp;
    return TRUE;
}

// NOTE: function doesn't exist in TP debug, needed for string data
BOOL DVDLowNotifyReset() {
    OSReport("@@@ (DVDLowNotifyReset) IOS_IoctlAsync returned error: %d\n");
    return TRUE;
}

BOOL DVDLowReset(DVDLowCallback callback) {
    dvdContext_t* dvdContext;
    IOSError rv;

    requestInProgress = TRUE;
    dvdContext = newContext(callback, 1);
    nextCommandBuf(&freeCommandBuf);
    diCommand[freeCommandBuf].theCommand = 0x8A;
    diCommand[freeCommandBuf].arg[0] = spinUpValue;
    rv = IOS_IoctlAsync(DiFD, 0x8A, &diCommand[freeCommandBuf], sizeof(diCommand_t), 0, 0, doTransactionCallback, dvdContext);

    if (rv != IOS_ERROR_OK) {
        OSReport("@@@ (DVDLowReset) IOS_IoctlAsync returned error: %d\n", rv);
        dvdContext->inUse = FALSE;
        return FALSE;
    }

    return TRUE;
}

BOOL DVDLowAudioBufferConfig(u8 enable, u32 size, DVDLowCallback callback) {
    dvdContext_t* dvdContext;
    IOSError rv;

    requestInProgress = TRUE;
    dvdContext = newContext(callback, 1);
    nextCommandBuf(&freeCommandBuf);
    diCommand[freeCommandBuf].theCommand = 0xE4;
    diCommand[freeCommandBuf].arg[0] = enable;
    diCommand[freeCommandBuf].arg[1] = size;
    rv = IOS_IoctlAsync(DiFD, 0xE4, &diCommand[freeCommandBuf], sizeof(diCommand_t), &diRegValCache, sizeof(diRegVals_t), doTransactionCallback, dvdContext);

    if (rv != IOS_ERROR_OK) {
        OSReport("@@@ (DVDLowAudioBufferConfig) IOS_IoctlAsync returned error: %d\n", rv);
        dvdContext->inUse = FALSE;
        return FALSE;
    }

    return TRUE;
}

// NOTE: function doesn't exist in TP debug, needed for string data
u32 DVDLowGetCoverStatus() {
    OSReport("(DVDLowGetCoverStatus): Synch functions can't be called in callbacks\n");
    OSReport("@@@ (DVDLowGetCoverStatus) IOS_Ioctl returned error: %d\n");
    return TRUE;
}

// NOTE: function doesn't exist in TP debug, needed for string data
u32 DVDLowReadDVD() {
    OSReport("@@@ (DVDLowReadDVD) IOS_IoctlAsync returned error: %d\n");
    return TRUE;
}

// NOTE: function doesn't exist in TP debug, needed for string data
BOOL DVDLowReadDVDConfig() {
    OSReport("@@@ (DVDLowReadDVDConfig) IOS_IoctlAsync returned error: %d\n");
    return TRUE;
}

// NOTE: function doesn't exist in TP debug, needed for string data
BOOL DVDLowReadDvdCopyright() {
    OSReport("@@@ (DVDLowReadDvdCopyright) IOS_IoctlAsync returned error: %d\n");
    return TRUE;
}

// NOTE: function doesn't exist in TP debug, needed for string data
BOOL DVDLowReadDvdPhysical() {
    OSReport("@@@ (DVDLowReadDvdPhysical) IOS_IoctlAsync returned error: %d\n");
    return TRUE;
}

// NOTE: function doesn't exist in TP debug, needed for string data
BOOL DVDLowReadDvdDiscKey() {
    OSReport("@@@ (DVDLowReadDvdDiscKey) IOS_IoctlAsync returned error: %d\n");
    return TRUE;
}

BOOL DVDLowReportKey(DVDVideoReportKey* reportKey, u32 format, u32 lsn, DVDLowCallback callback) {
    dvdContext_t* dvdContext;
    IOSError rv;

    requestInProgress = TRUE;
    dvdContext = newContext(callback, 1);
    nextCommandBuf(&freeCommandBuf);
    diCommand[freeCommandBuf].theCommand = 0xA4;
    diCommand[freeCommandBuf].arg[0] = format >> 16;
    diCommand[freeCommandBuf].arg[1] = lsn;

    rv = IOS_IoctlAsync(DiFD, 0xA4, &diCommand[freeCommandBuf], sizeof(diCommand_t), reportKey, sizeof(DVDVideoReportKey), doTransactionCallback, dvdContext);

    if (rv != IOS_ERROR_OK) {
        OSReport("@@@ (DVDLowReportKey) IOS_IoctlAsync returned error: %d\n", rv);
        dvdContext->inUse = FALSE;
        return FALSE;
    }

    return TRUE;
}

// NOTE: function doesn't exist in TP debug, needed for string data
BOOL DVDLowOffset() {
    OSReport("@@@ (DVDLowOffset) IOS_IoctlAsync returned error: %d\n");
    return TRUE;
}

// NOTE: function doesn't exist in TP debug, needed for string data
BOOL DVDLowStopLaser() {
    OSReport("@@@ (DVDLowStopLaser) IOS_IoctlAsync returned error: %d\n");
    return TRUE;
}

// NOTE: function doesn't exist in TP debug, needed for string data
BOOL DVDLowReadDiskBca() {
    OSReport("@@@ (DVDLowReadDiskBca) IOS_IoctlAsync returned error: %d\n");
    return TRUE;
}

// NOTE: function doesn't exist in TP debug, needed for string data
BOOL DVDLowSerMeasControl() {
    OSReport("@@@ (DVDLowSerMeasControl) IOS_IoctlAsync returned error: %d\n");
    return TRUE;
}

// NOTE: function doesn't exist in TP debug, needed for string data
BOOL DVDLowRequestDiscStatus() {
    OSReport("@@@ (DVDLowRequestDiscStatus) IOS_IoctlAsync returned error: %d\n");
    return TRUE;
}

// NOTE: function doesn't exist in TP debug, needed for string data
BOOL DVDLowRequestRetryNumber() {
    OSReport("@@@ (DVDLowRequestRetryNumber) IOS_IoctlAsync returned error: %d\n");
    return TRUE;
}

BOOL DVDLowSetMaximumRotation(u32 subcmd, DVDLowCallback callback) {
    dvdContext_t* dvdContext;
    IOSError rv;

    requestInProgress = TRUE;
    dvdContext = newContext(callback, 1);
    nextCommandBuf(&freeCommandBuf);
    diCommand[freeCommandBuf].theCommand = 0xDD;
    diCommand[freeCommandBuf].arg[0] = (subcmd >> 16) & 3;
    rv = IOS_IoctlAsync(DiFD, 0xDD, &diCommand[freeCommandBuf], sizeof(diCommand_t), 0, 0, doTransactionCallback, dvdContext);

    if (rv != IOS_ERROR_OK) {
        OSReport("@@@ (DVDLowSetMaxRotation) IOS_IoctlAsync returned error: %d\n", rv);
        dvdContext->inUse = FALSE;
        return FALSE;
    }

    return TRUE;
}

BOOL DVDLowRead(void* destAddr, u32 length, u32 wordOffset, DVDLowCallback callback) {
    dvdContext_t* dvdContext;
    IOSError rv;

    if (((u32)destAddr & 0x1F) != 0) {
        OSReport("(DVDLowRead): ERROR - destAddr buffer is not 32 byte aligned\n");
        return FALSE;
    }

    requestInProgress = TRUE;
    dvdContext = newContext(callback, 1);
    readLength = length;
    nextCommandBuf(&freeCommandBuf);
    diCommand[freeCommandBuf].theCommand = 0x71;
    diCommand[freeCommandBuf].arg[0] = length;
    diCommand[freeCommandBuf].arg[1] = wordOffset;
    rv = IOS_IoctlAsync(DiFD, 0x71, &diCommand[freeCommandBuf], sizeof(diCommand_t), destAddr, length, doTransactionCallback, dvdContext);

    if (rv != IOS_ERROR_OK) {
        OSReport("@@@ (DVDLowRead) IOS_IoctlAsync returned error: %d\n", rv);
        dvdContext->inUse = FALSE;
        return FALSE;
    }

    return TRUE;
}

BOOL DVDLowSeek(u32 wordOffset, DVDLowCallback callback) {
    dvdContext_t* dvdContext;
    IOSError rv;

    requestInProgress = TRUE;
    dvdContext = newContext(callback, 1);
    nextCommandBuf(&freeCommandBuf);
    diCommand[freeCommandBuf].theCommand = 0xAB;
    diCommand[freeCommandBuf].arg[0] = wordOffset;
    rv = IOS_IoctlAsync(DiFD, 0xAB, &diCommand[freeCommandBuf], sizeof(diCommand_t), 0, 0, doTransactionCallback, dvdContext);

    if (rv != IOS_ERROR_OK) {
        OSReport("@@@ (DVDLowSeek) IOS_IoctlAsync returned error: %d\n", rv);
        dvdContext->inUse = FALSE;
        return FALSE;
    }

    return TRUE;
}

// NOTE: function doesn't exist in TP debug, needed for string data
BOOL DVDLowGetCoverReg() {
    OSReport("(DVDLowGetCoverReg): Synch functions can't be called in callbacks\n");
    OSReport("@@@ (DVDLowGetCoverReg) IOS_Ioctl returned error: %d\n");
    return TRUE;
}

u32 DVDLowGetCoverRegister(void) {
    return diRegValCache.CoverRegVal;
}

u32 DVDLowGetStatusRegister(void) {
    return statusRegister[0];
}

u32 DVDLowGetControlRegister(void) {
    return controlRegister[0];
}

BOOL DVDLowPrepareCoverRegister(DVDLowCallback callback) {
    dvdContext_t* dvdContext;
    IOSError rv;

    nextCommandBuf(&freeCommandBuf);
    diCommand[freeCommandBuf].theCommand = 0x7A;
    requestInProgress = TRUE;
    dvdContext = newContext(callback, 1);
    rv = IOS_IoctlAsync(DiFD, 0x7A, &diCommand[freeCommandBuf], sizeof(diCommand_t), registerBuf, sizeof(registerBuf), doTransactionCallback, dvdContext);

    if (rv != IOS_ERROR_OK) {
        OSReport("@@@ (DVDLowPrepareCoverRegsiter) IOS_IoctlAsync returned error: %d\n", rv);
        dvdContext->inUse = FALSE;
        return FALSE;
    }

    return TRUE;
}

BOOL DVDLowPrepareStatusRegister(DVDLowCallback callback) {
    dvdContext_t* dvdContext;
    IOSError rv;

    nextCommandBuf(&freeCommandBuf);
    diCommand[freeCommandBuf].theCommand = 0x95;
    requestInProgress = TRUE;
    dvdContext = newContext(callback, 1);
    rv = IOS_IoctlAsync(DiFD, 0x95, &diCommand[freeCommandBuf], sizeof(diCommand_t), statusRegister, sizeof(statusRegister), doTransactionCallback, dvdContext);

    if (rv != IOS_ERROR_OK) {
        OSReport("@@@ (DVDLowPrepareStatusRegsiter) IOS_IoctlAsync returned error: %d\n", rv);
        dvdContext->inUse = FALSE;
        return FALSE;
    }

    return TRUE;
}

BOOL DVDLowPrepareControlRegister(DVDLowCallback callback) {
    dvdContext_t* dvdContext;
    IOSError rv;

    nextCommandBuf(&freeCommandBuf);
    diCommand[freeCommandBuf].theCommand = 0x96;
    requestInProgress = TRUE;
    dvdContext = newContext(callback, 1);
    rv = IOS_IoctlAsync(DiFD, 0x96, &diCommand[freeCommandBuf], sizeof(diCommand_t), controlRegister, sizeof(controlRegister), doTransactionCallback, dvdContext);

    if (rv != IOS_ERROR_OK) {
        OSReport("@@@ (DVDLowPrepareControlRegister) IOS_IoctlAsync returned error: %d\n", rv);
        dvdContext->inUse = FALSE;
        return FALSE;
    }

    return TRUE;
}

u32 DVDLowGetImmBufferReg(void) {
    return diRegValCache.ImmRegVal;
}

BOOL DVDLowUnmaskStatusInterrupts(void) {
    return TRUE;
}

BOOL DVDLowMaskCoverInterrupt(void) {
    return TRUE;
}

BOOL DVDLowClearCoverInterrupt(DVDLowCallback callback) {
    dvdContext_t* dvdContext;
    IOSError rv;

    nextCommandBuf(&freeCommandBuf);
    diCommand[freeCommandBuf].theCommand = 0x86;
    requestInProgress = TRUE;
    dvdContext = newContext(callback, 1);
    rv = IOS_IoctlAsync(DiFD, 0x86, &diCommand[freeCommandBuf], sizeof(diCommand_t), 0, 0, doTransactionCallback, dvdContext);

    if (rv != IOS_ERROR_OK) {
        OSReport("@@@ (DVDLowClearCoverInterrupt) IOS_IoctlAsync returned error: %d\n", rv);
        dvdContext->inUse = FALSE;
        return FALSE;
    }

    return TRUE;
}

BOOL __DVDLowTestAlarm(const OSAlarm *) {
    return FALSE;
}

// NOTE: function doesn't exist in TP debug, needed for string data
BOOL DVDLowEnableDvdVideo() {
    OSReport("@@@ (DVDLowEnableDvdVideo) IOS_IoctlAsync returned error: %d\n");
    return TRUE;
}
