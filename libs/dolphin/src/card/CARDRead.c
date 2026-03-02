#include <dolphin/card.h>

#include "__card.h"

#define TRUNC(n, a) (((u32)(n)) & ~((a)-1))

// prototypes
static void ReadCallback(s32 chan, s32 result);

s32 __CARDSeek(CARDFileInfo* fileInfo, s32 length, s32 offset, CARDControl** pcard) {
    CARDControl* card;
    CARDDir* dir;
    CARDDir* ent;
    s32 result;
    u16* fat;

    ASSERTLINE(98, 0 <= fileInfo->chan && fileInfo->chan < 2);
    ASSERTLINE(99, 0 <= fileInfo->fileNo && fileInfo->fileNo < CARD_MAX_FILE);

    result = __CARDGetControlBlock(fileInfo->chan, &card);
    if (result < 0)
        return result;

    ASSERTLINE(106, CARDIsValidBlockNo(card, fileInfo->iBlock));
    ASSERTLINE(107, fileInfo->offset < card->cBlock * card->sectorSize);

    if (!CARDIsValidBlockNo(card, fileInfo->iBlock) || card->cBlock * card->sectorSize <= fileInfo->offset)
        return __CARDPutControlBlock(card, CARD_RESULT_FATAL_ERROR);

    dir = __CARDGetDirBlock(card);
    ent = &dir[fileInfo->fileNo];

    ASSERTLINE(117, ent->gameName[0] != 0xff);

    if (ent->length * card->sectorSize <= offset || ent->length * card->sectorSize < offset + length)
        return __CARDPutControlBlock(card, CARD_RESULT_LIMIT);

    card->fileInfo = fileInfo;
    fileInfo->length = length;
    if (offset < fileInfo->offset) {
        fileInfo->offset = 0;
        fileInfo->iBlock = ent->startBlock;
        if (!CARDIsValidBlockNo(card, fileInfo->iBlock))
            return __CARDPutControlBlock(card, CARD_RESULT_BROKEN);
    }

    fat = __CARDGetFatBlock(card);
    while (fileInfo->offset < TRUNC(offset, card->sectorSize)) {
        fileInfo->offset += card->sectorSize;
        fileInfo->iBlock = fat[fileInfo->iBlock];
        if (!CARDIsValidBlockNo(card, fileInfo->iBlock))
            return __CARDPutControlBlock(card, CARD_RESULT_BROKEN);
    }

    fileInfo->offset = offset;

    *pcard = card;
    return CARD_RESULT_READY;
}

static void ReadCallback(s32 chan, s32 result) {
    CARDControl* card;
    CARDCallback callback;
    u16* fat;
    CARDFileInfo* fileInfo;
    s32 length;

    card = &__CARDBlock[chan];
    if (result < 0)
        goto error;

    fileInfo = card->fileInfo;
    if (fileInfo->length < 0) {
        result = CARD_RESULT_CANCELED;
        goto error;
    }

    length = TRUNC(fileInfo->offset + card->sectorSize, card->sectorSize) - fileInfo->offset;
    fileInfo->length -= length;
    if (fileInfo->length <= 0)
        goto error;

    fat = __CARDGetFatBlock(card);
    fileInfo->offset += length;
    fileInfo->iBlock = fat[fileInfo->iBlock];
    if (!CARDIsValidBlockNo(card, fileInfo->iBlock)) {
        result = CARD_RESULT_BROKEN;
        goto error;
    }

    ASSERTLINE(199, OFFSET(fileInfo->length, CARD_SEG_SIZE) == 0);
    ASSERTLINE(200, OFFSET(fileInfo->offset, card->sectorSize) == 0);

    result = __CARDRead(chan, card->sectorSize * (u32)fileInfo->iBlock,
                        (fileInfo->length < card->sectorSize) ? fileInfo->length : card->sectorSize, card->buffer,
                        ReadCallback);
    if (result < 0)
        goto error;

    return;

error:
    callback = card->apiCallback;
    card->apiCallback = NULL;
    __CARDPutControlBlock(card, result);
    ASSERTLINE(217, callback);
    callback(chan, result);
}

s32 CARDReadAsync(CARDFileInfo* fileInfo, void* buf, s32 length, s32 offset, CARDCallback callback) {
    CARDControl* card;
    s32 result;
    CARDDir* dir;
    CARDDir* ent;

    ASSERTLINE(250, buf && OFFSET(buf, 32) == 0);
    ASSERTLINE(251, OFFSET(offset, CARD_SEG_SIZE) == 0);
    ASSERTLINE(252, 0 < length && OFFSET(length, CARD_SEG_SIZE) == 0);

    if (OFFSET(offset, CARD_SEG_SIZE) != 0 || OFFSET(length, CARD_SEG_SIZE) != 0)
        return CARD_RESULT_FATAL_ERROR;

    result = __CARDSeek(fileInfo, length, offset, &card);
    if (result < 0)
        return result;

    dir = __CARDGetDirBlock(card);
    ent = &dir[fileInfo->fileNo];
    result = __CARDIsReadable(card, ent);
    if (result < 0)
        return __CARDPutControlBlock(card, result);

    DCInvalidateRange(buf, (u32)length);
    card->apiCallback = callback ? callback : __CARDDefaultApiCallback;

    offset = (s32)OFFSET(fileInfo->offset, card->sectorSize);
    length = (length < card->sectorSize - offset) ? length : card->sectorSize - offset;
    result = __CARDRead(fileInfo->chan, card->sectorSize * (u32)fileInfo->iBlock + offset, length, buf, ReadCallback);
    if (result < 0)
        __CARDPutControlBlock(card, result);
    return result;
}

s32 CARDRead(CARDFileInfo* fileInfo, void* buf, s32 length, s32 offset) {
    s32 result = CARDReadAsync(fileInfo, buf, length, offset, __CARDSyncCallback);
    if (result < 0) {
        return result;
    }

    return __CARDSync(fileInfo->chan);
}

s32 CARDCancel(CARDFileInfo* fileInfo) {
    BOOL enabled;
    s32 result;
    CARDControl* card;

    ASSERTLINE(338, 0 <= fileInfo->chan && fileInfo->chan < 2);
    ASSERTLINE(339, 0 <= fileInfo->fileNo && fileInfo->fileNo < CARD_MAX_FILE);

    enabled = OSDisableInterrupts();
    
    card = &__CARDBlock[fileInfo->chan];
    result = CARD_RESULT_READY;
    if (!card->attached)
        result = CARD_RESULT_NOCARD;
    else if (card->result == CARD_RESULT_BUSY && card->fileInfo == fileInfo) {
        fileInfo->length = -1;
        result = CARD_RESULT_CANCELED;
    }

    OSRestoreInterrupts(enabled);
    return result;
}
