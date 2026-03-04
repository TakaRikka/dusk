#include <dolphin/card.h>

#include "__card.h"

#define TRUNC(n, a) (((u32)(n)) & ~((a)-1))
#define CARD_PROGRAM_SIZE 128

static void ProgramCallback(s32 chan, s32 result) {
    CARDControl* card;
    CARDCallback callback;
    u16* fat;
    CARDFileInfo* fileInfo;
    s32 length;

    card = &__CARDBlock[chan];

    if (result >= 0) {
        fileInfo = card->fileInfo;
        if (fileInfo->length < 0) {
            result = CARD_RESULT_CANCELED;
            goto error;
        }

        length = TRUNC(fileInfo->offset + card->sectorSize, card->sectorSize) - fileInfo->offset;
        fileInfo->length -= length;
        if (fileInfo->length > 0) {
            fat = __CARDGetFatBlock(card);
            fileInfo->offset += length;
            fileInfo->iBlock = fat[fileInfo->iBlock];

            if (fileInfo->iBlock < 5 || fileInfo->iBlock >= card->cBlock) {
                result = CARD_RESULT_BROKEN;
                goto error;
            }

            ASSERTLINE(94, OFFSET(fileInfo->length, CARD_PROGRAM_SIZE) == 0);
            ASSERTLINE(95, OFFSET(fileInfo->offset, card->sectorSize) == 0);

            result = __CARDWrite(chan, card->sectorSize * fileInfo->iBlock, fileInfo->length < card->sectorSize ? fileInfo->length : card->sectorSize, card->buffer, ProgramCallback);
            if (result >= 0) {
                return;
            }
        }
    }

error:
    callback = card->apiCallback;
    card->apiCallback = NULL;
    __CARDPutControlBlock(card, result);
    ASSERTLINE(114, callback);
    callback(chan, result);
}

s32 CARDProgramAsync(CARDFileInfo* fileInfo, void* buf, s32 length, s32 offset, CARDCallback callback) {
    CARDControl* card;
    s32 result;
    CARDDir* dir;
    CARDDir* ent;

    ASSERTLINE(147, buf && OFFSET(buf, 32) == 0);
    ASSERTLINE(148, OFFSET(offset, CARD_PROGRAM_SIZE) == 0);
    ASSERTLINE(149, 0 < length && OFFSET(length, CARD_PROGRAM_SIZE) == 0);

    if (offset & 0x7F || length & 0x7F) {
        return CARD_RESULT_FATAL_ERROR;
    }

    result = __CARDSeek(fileInfo, length, offset, &card);
    if (result < 0) {
        return result;
    }

    dir = __CARDGetDirBlock(card);
    ent = &dir[fileInfo->fileNo];
    result = __CARDIsWritable(card, ent);
    if (result < 0) {
        return __CARDPutControlBlock(card, result);
    }

    DCStoreRange(buf, length);
    
    card->apiCallback = callback ? callback : &__CARDDefaultApiCallback;
    offset = fileInfo->offset & (card->sectorSize - 1);
    length = length < (card->sectorSize - offset) ? length : card->sectorSize - offset;

    result = __CARDWrite(fileInfo->chan, offset + (card->sectorSize * fileInfo->iBlock), length, buf, ProgramCallback);
    if (result < 0) {
        __CARDPutControlBlock(card, result);
    }

    return result;
}

s32 CARDProgram(CARDFileInfo* fileInfo, void* buf, s32 length, s32 offset) {
    s32 result = CARDProgramAsync(fileInfo, buf, length, offset, __CARDSyncCallback);
    if (result < 0)
        return result;

    return __CARDSync(fileInfo->chan);
}
