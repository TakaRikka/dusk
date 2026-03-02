#include <dolphin/card.h>

#include "__card.h"

// prototypes
static void CreateCallbackFat(s32 chan, s32 result);

static void CreateCallbackFat(s32 chan, s32 result) {
    CARDControl* card;
    CARDDir* dir;
    CARDDir* ent;
    CARDCallback callback;

    card = &__CARDBlock[chan];
    callback = card->apiCallback;
    card->apiCallback = NULL;

    if (result >= 0) {
        dir = __CARDGetDirBlock(card);
        ent = &dir[card->freeNo];
        memcpy(ent->gameName, card->diskID->gameName, sizeof(ent->gameName));
        memcpy(ent->company,  card->diskID->company, sizeof(ent->company));
        ent->permission = 4;
        ent->copyTimes = 0;

        ASSERTLINE(111, CARDIsValidBlockNo(card, card->startBlock));
        ent->startBlock = (u16)card->startBlock;
        ent->bannerFormat = 0;
        ent->iconAddr = -1;
        ent->iconFormat = 0;
        ent->iconSpeed = 0;
        ent->commentAddr = -1;

        CARDSetIconSpeed(ent, 0, CARD_STAT_SPEED_FAST);
        card->fileInfo->offset = 0;
        card->fileInfo->iBlock = ent->startBlock;
        ent->time = OSTicksToSeconds(OSGetTime());
        result = __CARDUpdateDir(chan, callback);
        if (result < 0) {
            goto after;
        }
    } else {
after:;
        __CARDPutControlBlock(card, result);
        if (callback) {
            callback(chan, result);
        }
    }
}

s32 CARDCreateAsync(s32 chan, const char* fileName, u32 size, CARDFileInfo* fileInfo, CARDCallback callback) {
    CARDControl* card;
    CARDDir* dir;
    CARDDir* ent;
    u16 fileNo;
    u16 freeNo;
    u16* fat;
    s32 result;

    ASSERTLINE(175, 0 <= chan && chan < 2);
    ASSERTLINE(176, strlen(fileName) <= CARD_FILENAME_MAX);

    if (strlen(fileName) > (u32)CARD_FILENAME_MAX) {
        return CARD_RESULT_NAMETOOLONG;
    }

    result = __CARDGetControlBlock(chan, &card);
    if (result < 0) {
        return result;
    }

    ASSERTLINE(188, 0 < size && (size % card->sectorSize) == 0);

    if (size <= 0 || (size % card->sectorSize) != 0) {        
        return CARD_RESULT_FATAL_ERROR;
    }

    freeNo = (u16)-1;
    dir = __CARDGetDirBlock(card);
    for (fileNo = 0; fileNo < CARD_MAX_FILE; fileNo++) {
        ent = &dir[fileNo];
        if (ent->gameName[0] == 0xff) {
            if (freeNo == (u16)-1) {
                freeNo = fileNo;
            }
        } else if (memcmp(ent->gameName, card->diskID->gameName, sizeof(ent->gameName)) == 0 &&
                             memcmp(ent->company, card->diskID->company, sizeof(ent->company)) == 0 &&
                             __CARDCompareFileName(ent, fileName)) {
            return __CARDPutControlBlock(card, CARD_RESULT_EXIST);
        }
    }

    if (freeNo == (u16)-1) {
        return __CARDPutControlBlock(card, CARD_RESULT_NOENT);
    }

    fat = __CARDGetFatBlock(card);
    if (card->sectorSize * fat[CARD_FAT_FREEBLOCKS] < size) {
        return __CARDPutControlBlock(card, CARD_RESULT_INSSPACE);
    }

    card->apiCallback = callback ? callback : __CARDDefaultApiCallback;
    card->freeNo = freeNo;
    ent = &dir[freeNo];
    ent->length = (u16)(size / card->sectorSize);
    strncpy((char*)ent->fileName, fileName, CARD_FILENAME_MAX);

    card->fileInfo = fileInfo;
    fileInfo->chan = chan;
    fileInfo->fileNo = freeNo;

    result = __CARDAllocBlock(chan, size / card->sectorSize, CreateCallbackFat);
    if (result < 0) {
        return __CARDPutControlBlock(card, result);
    }
    return result;
}

s32 CARDCreate(s32 chan, const char* fileName, u32 size, CARDFileInfo* fileInfo) {
    s32 result = CARDCreateAsync(chan, fileName, size, fileInfo, __CARDSyncCallback);
    if (result < 0) {
        return result;
    }

    return __CARDSync(chan);
}
