#include <dolphin/card.h>

#include "__card.h"

s32 CARDRenameAsync(s32 chan, const char* old, const char* new, CARDCallback callback) {
    CARDControl* card;
    CARDDir* dir;
    CARDDir* ent;
    s32 result;
    int fileNo;
    int newNo;
    int oldNo;

    ASSERTLINE(0x56, 0 <= chan && chan < 2);
    ASSERTLINE(0x57, *old != 0xff && *new != 0xff);
    ASSERTLINE(0x58, *old != 0x00 && *new != 0x00);

    if (old[0] == 0xFF || new[0] == 0xFF || old[0] == 0 || new[0] == 0)
        return CARD_RESULT_FATAL_ERROR;
    if (CARD_FILENAME_MAX < (u32)strlen(old) || CARD_FILENAME_MAX < (u32)strlen(new))
        return CARD_RESULT_NAMETOOLONG;

    result = __CARDGetControlBlock(chan, &card);
    if (result < 0)
        return result;

    newNo = oldNo = -1;
    dir = __CARDGetDirBlock(card);
    for (fileNo = 0; fileNo < CARD_MAX_FILE; fileNo++) {
        ent = &dir[fileNo];
        if (ent->gameName[0] == 0xFF)
            continue;

        if (memcmp(ent->gameName, card->diskID->gameName, sizeof(ent->gameName)) != 0
         || memcmp(ent->company, card->diskID->company, sizeof(ent->company)) != 0)
            continue;

        if (__CARDCompareFileName(ent, old))
            oldNo = fileNo;
        if (__CARDCompareFileName(ent, new))
            newNo = fileNo;
    }

    if (oldNo == -1)
        return __CARDPutControlBlock(card, CARD_RESULT_NOFILE);
    if (newNo != -1)
        return __CARDPutControlBlock(card, CARD_RESULT_EXIST);

    ent = &dir[oldNo];
    result = __CARDIsWritable(card, ent);
    if (result < 0)
        return __CARDPutControlBlock(card, result);

    strncpy((char*)ent->fileName, new, CARD_FILENAME_MAX);
    ent->time = (u32)OSTicksToSeconds(OSGetTime());

    result = __CARDUpdateDir(chan, callback);
    if (result < 0)
        __CARDPutControlBlock(card, result);

    return result;
}

s32 CARDRename(s32 chan, const char* old, const char* new) {
	s32 result = CARDRenameAsync(chan, old, new, __CARDSyncCallback);
	if (result < 0)
		return result;

	return __CARDSync(chan);
}
