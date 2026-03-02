#include <dolphin/card.h>

#include "__card.h"

// prototypes
static void BlockReadCallback(s32 chan, s32 result);
static void BlockWriteCallback(s32 chan, s32 result);

static void BlockReadCallback(s32 chan, s32 result) {
    CARDControl* card;
    CARDCallback callback;

    card = &__CARDBlock[chan];

    if ((result >= 0)) {
        card->xferred += 0x200;
        card->addr += 0x200;
        ((u8*)card->buffer) += 0x200;

        if (--card->repeat > 0) {
            result = __CARDReadSegment(chan, BlockReadCallback);
            if (result >= 0) {
                return;
            }
        }
    }

    if (!card->apiCallback) {
        __CARDPutControlBlock(card, result);
    }

    callback = card->xferCallback;
    if (callback) {
        card->xferCallback = NULL;
        callback(chan, result);
    }
}

s32 __CARDRead(s32 chan, u32 addr, s32 length, void* dst, CARDCallback callback) {
    CARDControl* card;

    ASSERTLINE(91, 0 < length && length % CARD_SEG_SIZE == 0);
    ASSERTLINE(92, 0 <= chan && chan < 2);

    card = &__CARDBlock[chan];
    if (card->attached == 0) {
        return CARD_RESULT_NOCARD;
    }
    card->xferCallback = callback;
    card->repeat = (length / 512u);
    card->addr = addr;
    card->buffer = dst;
    return __CARDReadSegment(chan, BlockReadCallback);
}

static void BlockWriteCallback(s32 chan, s32 result) {
    CARDControl* card;
    CARDCallback callback;

    card = &__CARDBlock[chan];
    if (result >= 0) {
        card->xferred += card->pageSize;
        card->addr += card->pageSize;
        ((u8*)card->buffer) += card->pageSize;

        if (--card->repeat > 0) {
            result = __CARDWritePage(chan, BlockWriteCallback);
            if (result >= 0) {
                return;
            }
        }
    }

    if (!card->apiCallback) {
        __CARDPutControlBlock(card, result);
    }

    callback = card->xferCallback;
    if (callback) {
        card->xferCallback = NULL;
        callback(chan, result);
    }
}

s32 __CARDWrite(s32 chan, u32 addr, s32 length, void* dst, CARDCallback callback) {
    CARDControl* card;
    card = &__CARDBlock[chan];

    ASSERTLINE(153, 0 < length && length % card->pageSize == 0);
    ASSERTLINE(154, 0 <= chan && chan < 2);
    
    if (card->attached == 0) {
        return CARD_RESULT_NOCARD;
    }
    card->xferCallback = callback;
    card->repeat = (length / card->pageSize);
    card->addr = addr;
    card->buffer = dst;
    return __CARDWritePage(chan, BlockWriteCallback);
}

s32 CARDGetXferredBytes(s32 chan) {
    ASSERTLINE(183, 0 <= chan && chan < 2);
    return __CARDBlock[chan].xferred;
}
