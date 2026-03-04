#include <revolution/card.h>

#include "__card.h"

const char* __CARDVersion = "<< RVL_SDK - CARD \trelease build: Sep  7 2006 18:26:19 (0x4200_60422) >>";

CARDControl __CARDBlock[2];

static u16 __CARDEncode;
static u16 __CARDFastMode;

DVDDiskID __CARDDiskNone;

// prototypes
static void TimeoutHandler(OSAlarm* alarm, OSContext* context);
static void SetupTimeoutAlarm(CARDControl* card);
static s32 Retry(s32 chan);
static void UnlockedCallback(s32 chan, s32 result);
static BOOL OnShutdown(BOOL f, u32 event);

static OSShutdownFunctionInfo ShutdownFunctionInfo = {OnShutdown, 127};

void __CARDDefaultApiCallback(s32 chan, s32 result) {}

void __CARDSyncCallback(s32 chan, s32 result) {
    CARDControl* card;
    card = &__CARDBlock[chan];
    OSWakeupThread(&card->threadQueue);
}

void __CARDExtHandler(s32 chan, OSContext* context) {
    CARDControl* card;
    CARDCallback callback;

    ASSERTLINE(232, 0 <= chan && chan < 2);

    card = &__CARDBlock[chan];
    if (card->attached) {
        ASSERTLINE(239, card->txCallback == 0);
        card->attached = FALSE;
        EXISetExiCallback(chan, 0);
        OSCancelAlarm(&card->alarm);
        callback = card->exiCallback;

        if (callback) {
            card->exiCallback = 0;
            callback(chan, CARD_RESULT_NOCARD);
        }

        if (card->result != CARD_RESULT_BUSY) {
            card->result = CARD_RESULT_NOCARD;
        }

        callback = card->extCallback;
        if (callback && CARD_MAX_MOUNT_STEP <= card->mountStep) {
            card->extCallback = 0;
            callback(chan, CARD_RESULT_NOCARD);
        }
    }
}

void __CARDExiHandler(s32 chan, OSContext* context) {
    CARDControl* card;
    CARDCallback callback;
    u8 status;
    s32 result;

    ASSERTLINE(283, 0 <= chan && chan < 2);
    card = &__CARDBlock[chan];

    OSCancelAlarm(&card->alarm);

    if (!card->attached) {
        return;
    }

    if (!EXILock(chan, 0, 0)) {
        result = CARD_RESULT_FATAL_ERROR;
        goto fatal;
    }

    if ((result = __CARDReadStatus(chan, &status)) < 0 || (result = __CARDClearStatus(chan)) < 0) {
        goto error;
    }

    if ((result = (status & 0x18) ? CARD_RESULT_IOERROR : CARD_RESULT_READY) == CARD_RESULT_IOERROR &&
        --card->retry > 0)
    {
        result = Retry(chan);
        if (result >= 0)
        {
            return;
        }
        goto fatal;
    }

error:
    EXIUnlock(chan);

fatal:
    callback = card->exiCallback;
    if (callback) {
        card->exiCallback = 0;
        callback(chan, result);
    }
}

void __CARDTxHandler(s32 chan, OSContext* context) {
    CARDControl* card;
    CARDCallback callback;
    int err;

    ASSERTLINE(365, 0 <= chan && chan < 2);
    
    card = &__CARDBlock[chan];
    err = !EXIDeselect(chan);
    EXIUnlock(chan);
    callback = card->txCallback;
    if (callback) {
        card->txCallback = NULL;
        callback(chan, (!err && EXIProbe(chan)) ? CARD_RESULT_READY : CARD_RESULT_NOCARD);
    }
}

void __CARDUnlockedHandler(s32 chan, OSContext* context) {
    CARDControl* card;
    CARDCallback callback;

    ASSERTLINE(412, 0 <= chan && chan < 2);

    card = &__CARDBlock[chan];
    callback = card->unlockCallback;
    if (callback) {
        card->unlockCallback = 0;
        callback(chan, EXIProbe(chan) ? CARD_RESULT_UNLOCKED : CARD_RESULT_NOCARD);
    }
}

s32 __CARDEnableInterrupt(s32 chan, BOOL enable) {
    BOOL err;
    u32 cmd;

    ASSERTLINE(431, 0 <= chan && chan < 2);

    if (!EXISelect(chan, 0, CARDFreq)) {
        return CARD_RESULT_NOCARD;
    }

    cmd = enable ? 0x81010000 : 0x81000000;
    err = FALSE;
    err |= !EXIImm(chan, &cmd, 2, EXI_WRITE, NULL);
    err |= !EXISync(chan);
    err |= !EXIDeselect(chan);
    return err ? CARD_RESULT_NOCARD : CARD_RESULT_READY;
}

s32 __CARDReadStatus(s32 chan, u8* status) {
    BOOL err;
    u32 cmd;

    ASSERTLINE(450, 0 <= chan && chan < 2);

    if (!EXISelect(chan, 0, CARDFreq)) {
        return CARD_RESULT_NOCARD;
    }

    cmd = 0x83000000;
    err = FALSE;
    err |= !EXIImm(chan, &cmd, 2, EXI_WRITE, NULL);
    err |= !EXISync(chan);
    err |= !EXIImm(chan, status, 1, EXI_READ, NULL);
    err |= !EXISync(chan);
    err |= !EXIDeselect(chan);
    return err ? CARD_RESULT_NOCARD : CARD_RESULT_READY;
}

int __CARDReadVendorID(s32 chan, u16* id) {
    BOOL err;
    u32 cmd;

    ASSERTLINE(471, 0 <= chan && chan < 2);

    if (!EXISelect(chan, 0, CARDFreq)) {
        return CARD_RESULT_NOCARD;
    }
    cmd = 0x85000000;
    err = 0;
    err |= !EXIImm(chan, &cmd, 2, EXI_WRITE, 0);
    err |= !EXISync(chan);
    err |= !EXIImm(chan, id, 2, EXI_READ, 0);
    err |= !EXISync(chan);
    err |= !EXIDeselect(chan);
    return err ? CARD_RESULT_NOCARD : CARD_RESULT_READY;
}

s32 __CARDClearStatus(s32 chan) {
    BOOL err;
    u32 cmd;

    ASSERTLINE(492, 0 <= chan && chan < 2);

    if (!EXISelect(chan, 0, CARDFreq)) {
        return CARD_RESULT_NOCARD;
    }

    cmd = 0x89000000;
    err = FALSE;
    err |= !EXIImm(chan, &cmd, 1, EXI_WRITE, 0);
    err |= !EXISync(chan);
    err |= !EXIDeselect(chan);

    return err ? CARD_RESULT_NOCARD : CARD_RESULT_READY;
}

s32 __CARDSleep(s32 chan) {
    int err;
    u32 cmd;

    ASSERTLINE(511, 0 <= chan && chan < 2);

    if (!EXISelect(chan, 0, CARDFreq)) {
        return CARD_RESULT_NOCARD;
    }
    cmd = 0x88000000;
    err = 0;
    err |= !EXIImm(chan, &cmd, 1, EXI_WRITE, 0);
    err |= !EXISync(chan);
    err |= !EXIDeselect(chan);

    if(err) {
        return CARD_RESULT_NOCARD;
    }
    return CARD_RESULT_READY;
}

s32 __CARDWakeup(s32 chan) {
    int err;
    u32 cmd;

    ASSERTLINE(530, 0 <= chan && chan < 2);
    if (!EXISelect(chan, 0, CARDFreq)) {
        return CARD_RESULT_NOCARD;
    }
    cmd = 0x87000000;
    err = 0;
    err |= !EXIImm(chan, &cmd, 1, EXI_WRITE, 0);
    err |= !EXISync(chan);
    err |= !EXIDeselect(chan);

    if(err) {
        return CARD_RESULT_NOCARD;
    }
    return CARD_RESULT_READY;
}

static void TimeoutHandler(OSAlarm* alarm, OSContext* context) {
    s32 chan;
    CARDControl* card;
    CARDCallback callback;
    for (chan = 0; chan < 2; ++chan) {
        card = &__CARDBlock[chan];
        if (alarm == &card->alarm) {
            break;
        }
    }

    ASSERTLINE(578, 0 <= chan && chan < 2);

    if (!card->attached) {
        return;
    }

    EXISetExiCallback(chan, NULL);
    callback = card->exiCallback;
    if (callback) {
        card->exiCallback = 0;
        callback(chan, CARD_RESULT_IOERROR);
    }
}

static void SetupTimeoutAlarm(CARDControl* card) {
    OSCancelAlarm(&card->alarm);
    switch (card->cmd[0]) {
    case 0xF2: 
        OSSetAlarm(&card->alarm, OSMillisecondsToTicks(100),
                   TimeoutHandler);
        break;
    case 0xF3:
        break;
    case 0xF4:
        if (card->pageSize > 0x80) {
            OSSetAlarm(&card->alarm, OSSecondsToTicks((OSTime)2) * (card->cBlock / 0x40),
                       TimeoutHandler);
            break;
        }
    case 0xF1:
        OSSetAlarm(&card->alarm, OSSecondsToTicks((OSTime)2) * (card->sectorSize / 0x2000),
                   TimeoutHandler);
        break;
    }
}

static s32 Retry(s32 chan) {
    CARDControl* card;

    ASSERTLINE(654, 0 <= chan && chan < 2);
    
    card = &__CARDBlock[chan];
    if (!EXISelect(chan, 0, CARDFreq)) {
        EXIUnlock(chan);
        return CARD_RESULT_NOCARD;
    }

    SetupTimeoutAlarm(card);

    if (!EXIImmEx(chan, card->cmd, card->cmdlen, EXI_WRITE)) {
        EXIDeselect(chan);
        EXIUnlock(chan);
        return CARD_RESULT_NOCARD;
    }

    if (card->cmd[0] == 0x52 &&
        !EXIImmEx(chan, (u8* )card->workArea + sizeof(CARDID), card->latency, EXI_WRITE))
    {
        EXIDeselect(chan);
        EXIUnlock(chan);
        return CARD_RESULT_NOCARD;
    }

    if (card->mode == 0xffffffff) {
        EXIDeselect(chan);
        EXIUnlock(chan);
        return CARD_RESULT_READY;
    }

    if (!EXIDma(chan, card->buffer, (s32)((card->cmd[0] == 0x52) ? 512 : card->pageSize), card->mode,
                __CARDTxHandler))
    {
        EXIDeselect(chan);
        EXIUnlock(chan);
        return CARD_RESULT_NOCARD;
    }

    return CARD_RESULT_READY;
}

static void UnlockedCallback(s32 chan, s32 result) {
    CARDCallback callback;
    CARDControl* card;

    ASSERTLINE(718, 0 <= chan && chan < 2);

    card = &__CARDBlock[chan];
    if (result >= 0) {
        card->unlockCallback = UnlockedCallback;
        if (!EXILock(chan, 0, __CARDUnlockedHandler)) {
            result = CARD_RESULT_READY;
        } else {
            card->unlockCallback = 0;
            result = Retry(chan);
        }
    }

    if (result < 0) {
        switch (card->cmd[0]) {
        case 0x52:
            callback = card->txCallback;
            if (callback) {
                card->txCallback = NULL;
                callback(chan, result);
            }
            break;
        case 0xF2:
        case 0xF4:
        case 0xF1:
            callback = card->exiCallback;
            if (callback) {
                card->exiCallback = 0;
                callback(chan, result);
            }
            break;
        }
    }
}

static s32 __CARDStart(s32 chan, CARDCallback txCallback, CARDCallback exiCallback) {
    BOOL enabled;
    CARDControl* card;
    s32 result;

    enabled = OSDisableInterrupts();

    ASSERTLINE(784, 0 <= chan && chan < 2);

    card = &__CARDBlock[chan];
    if (!card->attached) {
        result = CARD_RESULT_NOCARD;
    } else {
        if (txCallback) {
            card->txCallback = txCallback;
        }

        if (exiCallback) {
            card->exiCallback = exiCallback;
        }

        card->unlockCallback = UnlockedCallback;

        if (!EXILock(chan, 0, __CARDUnlockedHandler)) {
            result = CARD_RESULT_BUSY;
        } else {
            card->unlockCallback = 0;

            if (!EXISelect(chan, 0, CARDFreq)) {
                EXIUnlock(chan);
                result = CARD_RESULT_NOCARD;
            } else {
                SetupTimeoutAlarm(card);
                result = CARD_RESULT_READY;
            }
        }
    }

    OSRestoreInterrupts(enabled);
    return result;
}

#define AD1(x) ((u8)(((x) >> 17) & 0x7f))
#define AD1EX(x) ((u8)(AD1(x) | 0x80));
#define AD2(x) ((u8)(((x) >> 9) & 0xff))
#define AD3(x) ((u8)(((x) >> 7) & 0x03))
#define BA(x) ((u8)((x)&0x7f))

s32 __CARDReadSegment(s32 chan, CARDCallback callback) {
    CARDControl* card;
    s32 result;

    ASSERTLINE(846, 0 <= chan && chan < 2);

    card = &__CARDBlock[chan];
    ASSERTLINE(848, card->addr % CARD_SEG_SIZE == 0);
    ASSERTLINE(849, card->addr < (u32) card->size * 1024 * 1024 / 8);

    card->cmd[0] = 0x52;
    card->cmd[1] = AD1(card->addr);
    card->cmd[2] = AD2(card->addr);
    card->cmd[3] = AD3(card->addr);
    card->cmd[4] = BA(card->addr);
    card->cmdlen = 5;
    card->mode = 0;
    card->retry = 0;

    result = __CARDStart(chan, callback, 0);
    if (result == CARD_RESULT_BUSY) {
        result = CARD_RESULT_READY;
    } else if (result >= 0) {
        if (!EXIImmEx(chan, card->cmd, card->cmdlen, EXI_WRITE) ||
            !EXIImmEx(chan, (u8* )card->workArea + sizeof(CARDID), card->latency,
            EXI_WRITE) || // XXX use DMA if possible
            !EXIDma(chan, card->buffer, 512, card->mode, __CARDTxHandler))
        {
            card->txCallback = NULL;
            EXIDeselect(chan);
            EXIUnlock(chan);
            result = CARD_RESULT_NOCARD;
        } else {
            result = CARD_RESULT_READY;
        }
    }

    return result;
}

s32 __CARDWritePage(s32 chan, CARDCallback callback) {
    CARDControl* card;
    s32 result;

    ASSERTLINE(903, 0 <= chan && chan < 2);

    card = &__CARDBlock[chan];
    ASSERTLINE(905, card->addr % card->pageSize == 0);
    ASSERTLINE(906, card->addr < (u32) card->size * 1024 * 1024 / 8);
    card->cmd[0] = 0xF2;

    if (card->pageSize > 0x80) {
        card->cmd[1] = AD1(card->addr) | 0x80;
    } else {
        card->cmd[1] = AD1(card->addr);
    }

    card->cmd[2] = AD2(card->addr);
    card->cmd[3] = AD3(card->addr);
    card->cmd[4] = BA(card->addr);
    card->cmdlen = 5;
    card->mode = 1;
    card->retry = 3;

    result = __CARDStart(chan, 0, callback);
    if (result == CARD_RESULT_BUSY) {
        result = CARD_RESULT_READY;
    } else if (result >= 0) {
        if (!EXIImmEx(chan, card->cmd, card->cmdlen, EXI_WRITE) ||
            !EXIDma(chan, card->buffer, card->pageSize, card->mode, __CARDTxHandler))
        {
            card->exiCallback = 0;
            EXIDeselect(chan);
            EXIUnlock(chan);
            result = CARD_RESULT_NOCARD;
        } else {
            result = CARD_RESULT_READY;
        }
    }
    
    return result;
}

s32 __CARDErase(s32 chan, CARDCallback callback) {
    CARDControl* card;
    s32 result;

    ASSERTLINE(962, 0 <= chan && chan < 2);

    card = &__CARDBlock[chan];
    card->cmd[0] = 0xF4;
    card->cmd[1] = 0;
    card->cmd[2] = 0;
    card->cmdlen = 3;
    card->mode = -1;
    card->retry = 3;
    result = __CARDStart(chan, 0, callback);
    if (result == CARD_RESULT_BUSY) {
        result = CARD_RESULT_READY;
    } else if (result >= 0) {
        if (EXIImmEx(chan, &card->cmd, card->cmdlen, EXI_WRITE) == 0) {
            result = CARD_RESULT_NOCARD;
            card->exiCallback = 0;
        } else {
            result = CARD_RESULT_READY;
        }

        EXIDeselect(chan);
        EXIUnlock(chan);
    }
    
    return result;
}

s32 __CARDEraseSector(s32 chan, u32 addr, CARDCallback callback) {
    CARDControl* card;
    s32 result;

    ASSERTLINE(1010, 0 <= chan && chan < 2);

    card = &__CARDBlock[chan];
    ASSERTLINE(1012, addr % card->sectorSize == 0);
    ASSERTLINE(1013, addr < (u32) card->size * 1024 * 1024 / 8);

    if (card->pageSize > 0x80) {
        if (callback) {
            callback(chan, 0);
        }
        return 0;
    }

    card->cmd[0] = 0xF1;
    card->cmd[1] = AD1(addr);
    card->cmd[2] = AD2(addr);
    card->cmdlen = 3;
    card->mode = -1;
    card->retry = 3;

    result = __CARDStart(chan, 0, callback);

    if (result == CARD_RESULT_BUSY) {
        result = CARD_RESULT_READY;
    } else if (result >= 0) {
        if (!EXIImmEx(chan, card->cmd, card->cmdlen, EXI_WRITE)) {
            result = CARD_RESULT_NOCARD;
            card->exiCallback = NULL;
        } else {
            result = CARD_RESULT_READY;
        }

        EXIDeselect(chan);
        EXIUnlock(chan);
    }
    return result;
}

void CARDInit(void) {
    int chan;

    if (__CARDBlock[0].diskID && __CARDBlock[1].diskID) {
        return;
    }

    __CARDEncode = OSGetFontEncode();

    OSRegisterVersion(__CARDVersion);

    DSPInit();

    for (chan = 0; chan < 2; ++chan) {
        CARDControl* card = &__CARDBlock[chan];

        card->result = CARD_RESULT_NOCARD;
        OSInitThreadQueue(&card->threadQueue);
        OSCreateAlarm(&card->alarm);
    }
    __CARDSetDiskID((void*)OSPhysicalToCached(0));

    OSRegisterShutdownFunction(&ShutdownFunctionInfo);
}

u16 __CARDGetFontEncode(void) {
    return __CARDEncode;
}

u16 __CARDSetFontEncode(u16 encode) {
    u16 prev = __CARDEncode;

    switch (encode) {
    case CARD_ENCODE_ANSI:
    case CARD_ENCODE_SJIS:
        __CARDEncode = encode;
        break;
    }
    
    return prev;
}

void __CARDSetDiskID(const DVDDiskID* id) {
    __CARDBlock[0].diskID = id ? id : &__CARDDiskNone;
    __CARDBlock[1].diskID = id ? id : &__CARDDiskNone;
}

const DVDDiskID* CARDGetDiskID(s32 chan) {
    ASSERTLINE(1168, 0 <= chan && chan < 2);
    return __CARDBlock[chan].diskID;
}

s32 CARDSetDiskID(s32 chan, const DVDDiskID* diskID) {
    BOOL enabled;
    CARDControl* card;

    card = &__CARDBlock[chan];
    ASSERTLINE(1189, 0 <= chan && chan < 2);

    enabled = OSDisableInterrupts();

    if (card->result == CARD_RESULT_BUSY) {
        return CARD_RESULT_BUSY;   
    }

    card->diskID = diskID != 0 ? diskID : (const DVDDiskID*)OSPhysicalToCached(0);
    OSRestoreInterrupts(enabled);
    return 0;
}

s32 __CARDGetControlBlock(s32 chan, CARDControl** pcard) {
    BOOL enabled;
    s32 result;
    CARDControl* card;

    card = &__CARDBlock[chan];

    if (chan < 0 || chan >= 2 || card->diskID == 0) {
        return CARD_RESULT_FATAL_ERROR;
    }

    enabled = OSDisableInterrupts();

    if (!card->attached) {
        result = CARD_RESULT_NOCARD;
    } else if (card->result == CARD_RESULT_BUSY) {
        result = CARD_RESULT_BUSY;
    } else {
        card->result = CARD_RESULT_BUSY;
        result = CARD_RESULT_READY;
        card->apiCallback = NULL;
        *pcard = card;
    }

    OSRestoreInterrupts(enabled);
    return result;
}

s32 __CARDPutControlBlock(CARDControl* card, s32 result) {
    BOOL enabled;

    ASSERTLINE(1259, result != CARD_RESULT_BUSY);

    enabled = OSDisableInterrupts();
    if (card->attached) {
        card->result = result;
    } else if (card->result == CARD_RESULT_BUSY) {
        card->result = result;
    }

    OSRestoreInterrupts(enabled);
    return result;
}

s32 CARDGetResultCode(s32 chan) {
    CARDControl* card;
    
    ASSERTLINE(1292, 0 <= chan && chan < 2);

    if (chan < 0 || chan >= 2) {
        return CARD_RESULT_FATAL_ERROR;
    }
    card = &__CARDBlock[chan];
    return card->result;
}

s32 CARDFreeBlocks(s32 chan, s32* byteNotUsed, s32* filesNotUsed) {
    CARDControl* card;
    s32 result;
    u16* fat;
    CARDDir* dir;
    CARDDir* ent;
    u16 fileNo;

    result = __CARDGetControlBlock(chan, &card);
    if (result < 0) {
        return result;
    }

    fat = __CARDGetFatBlock(card);
    dir = __CARDGetDirBlock(card);
    if (fat == 0 || dir == 0) {
        return __CARDPutControlBlock(card, CARD_RESULT_BROKEN);
    }

    if (byteNotUsed) {
        *byteNotUsed = (s32)(card->sectorSize * fat[CARD_FAT_FREEBLOCKS]);
    }

    if (filesNotUsed) {
        *filesNotUsed = 0;
        for (fileNo = 0; fileNo < CARD_MAX_FILE; fileNo++) {
            ent = &dir[fileNo];
            if (ent->fileName[0] == 0xff) {
                ++*filesNotUsed;
            }
        }
    }

    return __CARDPutControlBlock(card, CARD_RESULT_READY);
}

s32 CARDGetEncoding(s32 chan, u16* encode) {
    CARDControl* card;
    CARDID* id;
    s32 result;

    result = __CARDGetControlBlock(chan, &card);
    if (result < 0) {
        return result;
    }

    id = card->workArea;
    *encode = id->encode;
    return __CARDPutControlBlock(card, CARD_RESULT_READY);
}

s32 CARDGetMemSize(s32 chan, u16* size) {
    CARDControl* card;
    s32 result;

    result = __CARDGetControlBlock(chan, &card);
    if (result < 0) {
        return result;
    }

    *size = card->size;
    return __CARDPutControlBlock(card, CARD_RESULT_READY);
}

s32 CARDGetSectorSize(s32 chan, u32* size) {
    CARDControl* card;
    s32 result;

    result = __CARDGetControlBlock(chan, &card);
    if (result < 0) {
        return result;
    }

    *size = card->sectorSize;
    return __CARDPutControlBlock(card, CARD_RESULT_READY);
}

s32 __CARDSync(s32 chan) {
    CARDControl* block;
    s32 result;
    s32 enabled;

    block = &__CARDBlock[chan];
    enabled = OSDisableInterrupts();
    while ((result = CARDGetResultCode(chan)) == CARD_RESULT_BUSY) {
        OSSleepThread(&block->threadQueue);
    }

    OSRestoreInterrupts(enabled);
    return result;
}

static BOOL OnShutdown(BOOL final, u32 event) {
    if (!final) {
        if (CARDUnmount(0) == CARD_RESULT_BUSY || CARDUnmount(1) == CARD_RESULT_BUSY) {
            return FALSE;
        }
    }

    return TRUE;
}

BOOL CARDSetFastMode(BOOL enable) {
    u16 prev = __CARDFastMode;
    __CARDFastMode = enable ? TRUE : FALSE;

    return prev ? TRUE : FALSE;
}

BOOL CARDGetFastMode(void) {
    return __CARDFastMode ? TRUE : FALSE;
}

s32 CARDGetCurrentMode(s32 chan, u32* mode) {
    CARDControl* card;
    s32 result;

    result = __CARDGetControlBlock(chan, &card);
    if (result < 0) {
        return result;
    }

    switch (card->pageSize) {
    case 512:
        *mode = 1;
        break;
    case 128:
    default:
        *mode = 0;
        break;
    }

    return __CARDPutControlBlock(card, CARD_RESULT_READY);
}
