#include <dolphin/card.h>

#include "__card.h"

BOOL __CARDCompareFileName(CARDDir* ent, const char* fileName) {
    char* entName = (char*)ent->fileName;
    char c1;
    char c2;
    int n = CARD_FILENAME_MAX;

    while (--n >= 0) {
        if ((c1 = *entName++) != (c2 = *fileName++))
            return FALSE;
        else if (c2 == '\0')
            return TRUE;
    }

    if (*fileName == '\0')
        return TRUE;
    return FALSE;
}

s32 __CARDAccess(CARDControl* card, CARDDir* ent) {
    const DVDDiskID* diskID = card->diskID;

    if (ent->gameName[0] == 0xFF)
        return CARD_RESULT_NOFILE;

    if (diskID == &__CARDDiskNone
     || (memcmp(ent->gameName, diskID->gameName, sizeof(ent->gameName)) == 0
      && memcmp(ent->company, diskID->company, sizeof(ent->company)) == 0))
        return CARD_RESULT_READY;

    return CARD_RESULT_NOPERM;
}

s32 __CARDIsWritable(CARDControl* card, CARDDir* ent) {
    const DVDDiskID* diskID = card->diskID;
    s32 result;
    u8 perm;

    result = __CARDAccess(card, ent);
    if (result == CARD_RESULT_NOPERM) {
        perm = ent->permission & __CARDPermMask;
        if (perm & 0x20 && (memcmp(ent->gameName, __CARDDiskNone.gameName, sizeof(ent->gameName)) == 0 &&
                            memcmp(ent->company, __CARDDiskNone.company, sizeof(ent->company)) == 0))
        {
            return CARD_RESULT_READY;
        } else if (perm & 0x40 && (memcmp(ent->gameName, __CARDDiskNone.gameName, sizeof(ent->gameName)) == 0 &&
                                  memcmp(ent->company, diskID->company, sizeof(ent->company)) == 0))
        {
            return CARD_RESULT_READY;
        }
    }

    return result;
}

s32 __CARDIsReadable(CARDControl* card, CARDDir* ent) {
    s32 result = __CARDIsWritable(card, ent);
    if (result == CARD_RESULT_NOPERM && (ent->permission & 0x4)) {
        return CARD_RESULT_READY;
    }

    return result;
}

s32 __CARDGetFileNo(CARDControl* card, const char* fileName, s32* pfileNo) {
    CARDDir* dir;
    CARDDir* ent;
    s32 fileNo;
    s32 result;

    if (!card->attached)
        return CARD_RESULT_NOCARD;

    dir = __CARDGetDirBlock(card);
    for (fileNo = 0; fileNo < CARD_MAX_FILE; fileNo++) {
        ent = &dir[fileNo];
        result = __CARDAccess(card, ent);

        if (result < 0)
            continue;
        if (__CARDCompareFileName(ent, fileName)) {
            *pfileNo = fileNo;
            return CARD_RESULT_READY;
        }
    }

    return CARD_RESULT_NOFILE;
}

s32 CARDFastOpen(s32 chan, s32 fileNo, CARDFileInfo* fileInfo) {
    CARDControl* card;
    CARDDir* dir;
    CARDDir* ent;
    s32 result;

    ASSERTLINE(278, 0 <= fileNo && fileNo < CARD_MAX_FILE);
    ASSERTLINE(279, 0 <= chan && chan < 2);

    if (fileNo < 0 || fileNo >= CARD_MAX_FILE)
        return CARD_RESULT_FATAL_ERROR;

    fileInfo->chan = -1;
    result = __CARDGetControlBlock(chan, &card);
    if (result < 0)
        return result;

    dir = __CARDGetDirBlock(card);
    ent = &dir[fileNo];
    result = __CARDIsReadable(card, ent);
    if (0 <= result) {
        if (!CARDIsValidBlockNo(card, ent->startBlock))
            result = CARD_RESULT_BROKEN;
        else {
            fileInfo->chan = chan;
            fileInfo->fileNo = fileNo;
            fileInfo->offset = 0;
            fileInfo->iBlock = ent->startBlock;
        }
    }
    return __CARDPutControlBlock(card, result);
}

s32 CARDOpen(s32 chan, const char* fileName, CARDFileInfo* fileInfo) {
    CARDControl* card;
    s32 fileNo;
    s32 result;
    CARDDir* dir;
    CARDDir* ent;

    ASSERTLINE(336, 0 <= chan && chan < 2);

    fileInfo->chan = -1;
    result = __CARDGetControlBlock(chan, &card);
    if (result < 0)
        return result;

    result = __CARDGetFileNo(card, fileName, &fileNo);
    if (result >= 0) {
        dir = __CARDGetDirBlock(card);
        ent = &dir[fileNo];
        if (!CARDIsValidBlockNo(card, ent->startBlock))
            result = CARD_RESULT_BROKEN;
        else {
            fileInfo->chan = chan;
            fileInfo->fileNo = fileNo;
            fileInfo->offset = 0;
            fileInfo->iBlock = ent->startBlock;
        }
    }

    return __CARDPutControlBlock(card, result);
}

s32 CARDClose(CARDFileInfo* fileInfo) {
    CARDControl* card;
    s32 result;

    ASSERTLINE(380, 0 <= fileInfo->chan && fileInfo->chan < 2);
    ASSERTLINE(381, 0 <= fileInfo->fileNo && fileInfo->fileNo < CARD_MAX_FILE);

    result = __CARDGetControlBlock(fileInfo->chan, &card);
    if (result < 0)
        return result;

    fileInfo->chan = -1;
    return __CARDPutControlBlock(card, CARD_RESULT_READY);
}

BOOL __CARDIsOpened(CARDControl* card, s32 fileNo) {
    return FALSE;
}
