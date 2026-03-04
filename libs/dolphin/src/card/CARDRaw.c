#include <dolphin/card.h>

#include "__card.h"

s32 __CARDRawReadAsync(s32 chan, void* buf, s32 length, s32 offset, CARDCallback callback) {
    CARDControl* card;
    s32 result;

    ASSERTLINE(59, buf && ((u32) buf % 32) == 0);

    result = __CARDGetControlBlock(chan, &card);
    if (result < 0) {
        return __CARDPutControlBlock(card, result);
    }

    ASSERTLINE(67, 0 < length && (length % CARD_SEG_SIZE) == 0 && length < CARD_MAX_SIZE);
    ASSERTLINE(68, (offset % card->sectorSize) == 0);

    DCInvalidateRange(buf, length);
    result = __CARDRead(chan, offset, length, buf, callback);
    if (result < 0) {
        __CARDPutControlBlock(card, result);
    }
    return result;
}

s32 __CARDRawRead(s32 chan, void* buf, s32 length, s32 offset) {
    s32 result = __CARDRawReadAsync(chan, buf, length, offset, __CARDSyncCallback);
    if (result < 0) {
        return result;
    }

    return __CARDSync(chan);
}

static void EraseCallback(s32 chan, s32 result) {
    CARDControl* card;
    CARDCallback callback;

    card = &__CARDBlock[chan];
    callback = card->apiCallback;
    card->apiCallback = NULL;

    __CARDPutControlBlock(card, result);

    ASSERTLINE(117, callback);
    callback(chan, result);
}

s32 __CARDRawEraseAsync(s32 chan, s32 offset, CARDCallback callback) {
    CARDControl* card;
    s32 result;

    result = __CARDGetControlBlock(chan, &card);
    if (result < 0) {
        return __CARDPutControlBlock(card, result);
    }

    if (offset % card->sectorSize) {
        return __CARDPutControlBlock(card, CARD_RESULT_FATAL_ERROR); 
    }

    if ((card->size * 1024 * 1024) / 8 <= offset) {
        return __CARDPutControlBlock(card, CARD_RESULT_LIMIT); 
    }

    card->apiCallback = callback ? callback : __CARDDefaultApiCallback;
    result = __CARDEraseSector(chan, offset, EraseCallback);
    if (result < 0) {
        __CARDPutControlBlock(card, result);
    }
    return result;
}

s32 __CARDRawErase(s32 chan, s32 offset) {
    s32 result = __CARDRawEraseAsync(chan, offset, __CARDSyncCallback);
    if (result < 0) {
        return result;
    }

    return __CARDSync(chan);
}
