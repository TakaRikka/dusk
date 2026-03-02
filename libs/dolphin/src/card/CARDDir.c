#include <dolphin/card.h>

#include "__card.h"

// prototypes
static void WriteCallback(s32 chan, s32 result);
static void EraseCallback(s32 chan, s32 result);

CARDDir* __CARDGetDirBlock(CARDControl* card) {
    ASSERTLINE(54, card->currentDir);
    return card->currentDir;
}

static void WriteCallback(s32 chan, s32 result) {
    CARDControl* card = &__CARDBlock[chan];
    CARDCallback callback;

    if (result >= 0) {
        CARDDir* dir0 = (CARDDir*)((u8*)card->workArea + 0x2000);
        CARDDir* dir1 = (CARDDir*)((u8*)card->workArea + 0x4000);

        ASSERTLINE(79, card->currentDir);

        if (card->currentDir == dir0) {
            card->currentDir = dir1;
            memcpy(dir1, dir0, 0x2000);
        } else {
            ASSERTLINE(87, card->currentDir == dir1);
            card->currentDir = dir0;
            memcpy(dir0, dir1, 0x2000);
        }
    }

    if (!card->apiCallback)
        __CARDPutControlBlock(card, result);

    callback = card->eraseCallback;
    if (callback) {
        card->eraseCallback = NULL;
        callback(chan, result);
    }
}

static void EraseCallback(s32 chan, s32 result) {
    CARDControl* card = &__CARDBlock[chan];
    CARDCallback callback;
    CARDDir* dir;
    u32 addr;

    if (result >= 0) {
        dir = __CARDGetDirBlock(card);
        addr = ((u32)dir - (u32)card->workArea) / 0x2000 * card->sectorSize;
        result = __CARDWrite(chan, addr, 0x2000, dir, WriteCallback);
        if (result >= 0)
            return;
    }

    if (!card->apiCallback)
        __CARDPutControlBlock(card, result);

    callback = card->eraseCallback;
    if (callback) {
        card->eraseCallback = NULL;
        callback(chan, result);
    }
}

s32 __CARDUpdateDir(s32 chan, CARDCallback callback) {
    CARDControl* card;
    CARDDirCheck* check;
    u32 addr;
    CARDDir* dir;

    ASSERTLINE(173, 0 <= chan && chan < 2);

    card = &__CARDBlock[chan];
    if (!card->attached)
        return CARD_RESULT_NOCARD;

    dir = __CARDGetDirBlock(card);
    check = CARDGetDirCheck(dir);
    ++check->checkCode;
    __CARDCheckSum(dir, 0x2000 - sizeof(u32), &check->checkSum, &check->checkSumInv);
    DCStoreRange(dir, 0x2000);

    card->eraseCallback = callback;
    addr = ((u32)dir - (u32)card->workArea) / 0x2000 * card->sectorSize;
    return __CARDEraseSector(chan, addr, EraseCallback);
}
