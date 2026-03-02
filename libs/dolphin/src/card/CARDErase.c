#include <dolphin/card.h>

#include "__card.h"

static void EraseCallback(s32 chan, s32 result) {
    CARDControl* card;
    CARDCallback callback;
    u16* fat;
    CARDDir* dir;
    CARDDir* ent;
    CARDFileInfo* fileInfo;

    card = &__CARDBlock[chan];

    if (result >= 0) {
        fileInfo = card->fileInfo;
        if (fileInfo->length < 0) {
            result = CARD_RESULT_CANCELED;
            goto error;
        }

        fileInfo->length -= card->sectorSize;
        if (fileInfo->length <= 0) {
            dir = __CARDGetDirBlock(card);
            ent = dir + fileInfo->fileNo;
            ent->time = OSTicksToSeconds(OSGetTime());
            callback = card->apiCallback;
            card->apiCallback = NULL;

            result = __CARDUpdateDir(chan, callback);
        } else {
            fat = __CARDGetFatBlock(card);
            fileInfo->offset += card->sectorSize;
            fileInfo->iBlock = fat[fileInfo->iBlock];

            if (fileInfo->iBlock < 5 || fileInfo->iBlock >= card->cBlock) {
                result = CARD_RESULT_BROKEN;
                goto error;
            }

            result = __CARDEraseSector(chan, card->sectorSize * fileInfo->iBlock, EraseCallback);
        }        
        
        if (result < 0) {
            goto error;
        }
        return;
    }

error:
    callback = card->apiCallback;
    card->apiCallback = NULL;
    __CARDPutControlBlock(card, result);
    ASSERTLINE(98, callback);
    callback(chan, result);
}

s32 CARDEraseAsync(CARDFileInfo* fileInfo, s32 length, s32 offset, CARDCallback callback) {
    CARDControl* card;
    s32 result;
    CARDDir* dir;
    CARDDir* ent;

    ASSERTLINE(132, 0 < length);

    result = __CARDSeek(fileInfo, length, offset, &card);
    if (result < 0) {
        return result;
    }
    
    ASSERTLINE(138, OFFSET(offset, card->sectorSize) == 0);
    ASSERTLINE(139, OFFSET(length, card->sectorSize) == 0);

    if (OFFSET(offset, card->sectorSize) != 0 || OFFSET(length, card->sectorSize) != 0) {
        return __CARDPutControlBlock(card, CARD_RESULT_FATAL_ERROR);
    }

    dir = __CARDGetDirBlock(card);
    ent = dir + fileInfo->fileNo;
    result = __CARDIsWritable(card, ent);
    if (result < 0) {
        return __CARDPutControlBlock(card, result);
    }

    card->apiCallback = callback ? callback : __CARDDefaultApiCallback;

    result = __CARDEraseSector(fileInfo->chan, card->sectorSize * fileInfo->iBlock, EraseCallback);
    if (result < 0) {
        __CARDPutControlBlock(card, result);
    }

    return result;
}

s32 CARDErase(CARDFileInfo* fileInfo, s32 length, s32 offset) {
    s32 result = CARDEraseAsync(fileInfo, length, offset, __CARDSyncCallback);
    if (result < 0) {
        return result;
    }

    return __CARDSync(fileInfo->chan);
}
