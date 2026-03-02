#include <dolphin/card.h>

#include "os/__os.h"
#include "__card.h"

static void FormatCallback(s32 chan, s32 result) {
    CARDControl* card;
    CARDCallback callback;

    card = &__CARDBlock[chan];
    if (result < 0)
        goto error;

    ++card->formatStep;
    if (card->formatStep < CARD_NUM_SYSTEM_BLOCK) {
        result = __CARDEraseSector(chan, (u32)card->sectorSize * card->formatStep, FormatCallback);
        if (result >= 0)
            return;
    } else if (card->formatStep < 2 * CARD_NUM_SYSTEM_BLOCK) {
        int step = card->formatStep - CARD_NUM_SYSTEM_BLOCK;
        result = __CARDWrite(chan, (u32)card->sectorSize * step, CARD_SYSTEM_BLOCK_SIZE,
                             (u8* )card->workArea + (CARD_SYSTEM_BLOCK_SIZE * step), FormatCallback);
        if (result >= 0)
            return;
    } else {
        card->currentDir = (CARDDir*)((u8*)card->workArea + (1 + 0) * CARD_SYSTEM_BLOCK_SIZE);
        memcpy(card->currentDir, (u8*)card->workArea + (1 + 1) * CARD_SYSTEM_BLOCK_SIZE, CARD_SYSTEM_BLOCK_SIZE);
        card->currentFat = (u16*)((u8*)card->workArea + (3 + 0) * CARD_SYSTEM_BLOCK_SIZE);
        memcpy(card->currentFat, (u8*)card->workArea + (3 + 1) * CARD_SYSTEM_BLOCK_SIZE, CARD_SYSTEM_BLOCK_SIZE);
    }

error:
    callback = card->apiCallback;
    card->apiCallback = NULL;
    __CARDPutControlBlock(card, result);
    ASSERTLINE(133, callback);
    callback(chan, result);
}

s32 __CARDFormatRegionAsync(s32 chan, u16 encode, CARDCallback callback) {
    CARDControl* card;
    CARDID* id;
    CARDDir* dir;
    u16* fat;
    s16 i;
    s32 result;
    OSSram* sram;
    OSSramEx* sramEx;
    u16 dvdstatus;
    OSTime time;
    OSTime rand;

    ASSERTLINE(167, encode == CARD_ENCODE_ANSI || encode == CARD_ENCODE_SJIS);
    ASSERTLINE(168, 0 <= chan && chan < 2);

    result = __CARDGetControlBlock(chan, &card);
    if (result < 0)
        return result;

    id = (CARDID*)card->workArea;
    memset(id, 0xff, CARD_SYSTEM_BLOCK_SIZE);
    dvdstatus = __VIRegs[55];

    id->encode = encode;
    sram = __OSLockSram();
    *(u32*)&id->serial[20] = sram->counterBias;
    *(u32*)&id->serial[24] = sram->language;
    __OSUnlockSram(FALSE);

    rand = time = OSGetTime();

    sramEx = __OSLockSramEx();
    for (i = 0; i < 12; i++) {
        rand = (rand * 1103515245 + 12345) >> 16;
        id->serial[i] = (u8)(sramEx->flashID[chan][i] + rand);
        rand = ((rand * 1103515245 + 12345) >> 16) & 0x7FFF;
    }
    __OSUnlockSramEx(FALSE);

    *(u32*)&id->serial[28] = dvdstatus;
    *(OSTime*)&id->serial[12] = time;

    id->deviceID = 0;
    id->size = card->size;
    __CARDCheckSum(id, sizeof(CARDID) - sizeof(u32), &id->checkSum, &id->checkSumInv);

    for (i = 0; i < 2; i++) {
        CARDDirCheck* check;

        dir = (CARDDir*)((u8*)card->workArea + (1 + i) * CARD_SYSTEM_BLOCK_SIZE);
        memset(dir, 0xff, CARD_SYSTEM_BLOCK_SIZE);
        check = CARDGetDirCheck(dir);
        check->checkCode = i;
        __CARDCheckSum(dir, CARD_SYSTEM_BLOCK_SIZE - sizeof(u32), &check->checkSum, &check->checkSumInv);
    }

    for (i = 0; i < 2; i++) {
        fat = (u16*)((u8*)card->workArea + (3 + i) * CARD_SYSTEM_BLOCK_SIZE);
        memset(fat, 0x00, CARD_SYSTEM_BLOCK_SIZE);
        fat[CARD_FAT_CHECKCODE] = (u16)i;
        fat[CARD_FAT_FREEBLOCKS] = (u16)(card->cBlock - CARD_NUM_SYSTEM_BLOCK);
        fat[CARD_FAT_LASTSLOT] = CARD_NUM_SYSTEM_BLOCK - 1;
        __CARDCheckSum(&fat[CARD_FAT_CHECKCODE], CARD_SYSTEM_BLOCK_SIZE - sizeof(u32), &fat[CARD_FAT_CHECKSUM],
                       &fat[CARD_FAT_CHECKSUMINV]);
    }

    card->apiCallback = callback ? callback : __CARDDefaultApiCallback;
    DCStoreRange(card->workArea, CARD_WORKAREA_SIZE);

    card->formatStep = 0;
    result = __CARDEraseSector(chan, (u32)card->sectorSize * card->formatStep, FormatCallback);
    if (result < 0)
        __CARDPutControlBlock(card, result);
    return result;
}

s32 __CARDFormatRegion(s32 chan, u16 encode) {
    s32 result = __CARDFormatRegionAsync(chan, encode, &__CARDSyncCallback);
    if (result < 0) {
        return result;
    }

    return __CARDSync(chan);
}

s32 CARDFormatAsync(s32 chan, CARDCallback callback) {
    return __CARDFormatRegionAsync(chan, __CARDGetFontEncode(), callback);
}

s32 CARDFormat(s32 chan) {
    s32 result = __CARDFormatRegionAsync(chan, __CARDGetFontEncode(), &__CARDSyncCallback);
    if (result < 0) {
        return result;
    }

    return __CARDSync(chan);
}
