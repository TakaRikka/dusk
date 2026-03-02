#include <dolphin/dolphin.h>
#include <dolphin/card.h>

#include "__card.h"

s32 __CARDGetStatusEx(s32 chan, s32 fileNo, CARDDir* dirent) {
    ASSERTLINE(85, 0 <= chan && chan < 2);
    ASSERTLINE(86, 0 <= fileNo && fileNo < CARD_MAX_FILE);

    if ((fileNo < 0) || (fileNo >= CARD_MAX_FILE)) {
        return CARD_RESULT_FATAL_ERROR;
    }

    {
        CARDControl* card;
        CARDDir* dir;
        CARDDir* ent;
        s32 result = __CARDGetControlBlock(chan, &card);

        if (result < 0) {
            return result;
        }
    
        dir = __CARDGetDirBlock(card);
        ent = &dir[fileNo];
        result = __CARDIsReadable(card, ent);
        if (result >= 0) {
            memcpy(dirent, ent, 0x40);
        }
        return __CARDPutControlBlock(card, result);
    }
}

s32 __CARDSetStatusExAsync(s32 chan, s32 fileNo, CARDDir* dirent, CARDCallback callback) {
    CARDControl* card;
    CARDDir* dir;
    CARDDir* ent;
    s32 result;
    u8* p;
    s32 i;

    ASSERTLINE(142, 0 <= fileNo && fileNo < CARD_MAX_FILE);
    ASSERTLINE(143, 0 <= chan && chan < 2);
    ASSERTLINE(144, *dirent->fileName != 0xff && *dirent->fileName != 0x00);

    ASSERTMSGLINE(152, dirent->iconAddr == 0xffffffff || dirent->iconAddr < CARD_READ_SIZE, "CARDSetStatus[Async](): stat->iconAddr must be 0xffffffff or less than CARD_READ_SIZE.");
    ASSERTMSGLINE(155, dirent->commentAddr == 0xffffffff || (dirent->commentAddr & 0x1FFF) <= 8128, "CARDSetStatus[Async](): comment strings (set by stat->commentAddr) must not cross 8KB byte boundary.");

    if ((fileNo < 0) || (fileNo >= CARD_MAX_FILE) || ((u8) dirent->fileName[0] == 0xFF) || ((u8) dirent->fileName[0] == 0)) {
        return CARD_RESULT_FATAL_ERROR;
    }

    result = __CARDGetControlBlock(chan, &card);
    if (result < 0) {
        return result;
    }

    dir = __CARDGetDirBlock(card);
    ent = &dir[fileNo];
    result = __CARDIsWritable(card, ent);
    if (result < 0) {
        return __CARDPutControlBlock(card, result);
    }

    for (p = dirent->fileName; p < (u8*)&dirent->time; p++) {
        if (*p != 0) {
            continue;
        }
        while ((++p) < (u8*)&dirent->time) {
            *p = 0;
        }
        break;
    }

    if (dirent->permission & 0x20) {
        memset(dirent->gameName, 0, sizeof(dirent->gameName));
        memset(dirent->company, 0, sizeof(dirent->company));
    }

    if (dirent->permission & 0x40) {
        memset(dirent->gameName, 0, sizeof(dirent->gameName));
    }

    if ((memcmp(&ent->fileName, &dirent->fileName, 32) != 0) || (memcmp(ent->gameName, dirent->gameName, 4) != 0) || (memcmp(ent->company, dirent->company, 2) != 0)) {
        for(i = 0; i < CARD_MAX_FILE; i++) {
            if (i != fileNo) {
                CARDDir* ent = &dir[i]; // sure, just redeclare ent again...
                if (((u8) ent->gameName[0] != 0xFF) 
                    && (memcmp(&ent->gameName, &dirent->gameName, 4) == 0) 
                    && (memcmp(&ent->company, &dirent->company, 2) == 0) 
                    && (memcmp(&ent->fileName, &dirent->fileName, 0x20) == 0)) {
                        return __CARDPutControlBlock(card, -7);
                }
            }
        }
        memcpy(&ent->fileName, &dirent->fileName, 0x20);
        memcpy(&ent->gameName, &dirent->gameName, 4);
        memcpy(&ent->company, &dirent->company, 2);
    }

    ent->time = dirent->time;
    ent->bannerFormat = dirent->bannerFormat;
    ent->iconAddr = dirent->iconAddr;
    ent->iconFormat = dirent->iconFormat;
    ent->iconSpeed = dirent->iconSpeed;
    ent->commentAddr = dirent->commentAddr;
    ent->permission = dirent->permission;
    ent->copyTimes = dirent->copyTimes;

    result = __CARDUpdateDir(chan, callback);
    if (result < 0) {
        __CARDPutControlBlock(card, result);
    }
    return result;
}

s32 __CARDSetStatusEx(s32 chan, s32 fileNo, CARDDir* dirent) {
    s32 result = __CARDSetStatusExAsync(chan, fileNo, dirent, &__CARDSyncCallback);
    if (result < 0) {
        return result;
    }

    return __CARDSync(chan);
}
