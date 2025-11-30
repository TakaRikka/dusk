#include <revolution.h>
#include <revolution/os.h>

#include "__os.h"

void __OSRelaunchTitle(u32 resetCode) {
    s32 rc = 0;
    u32 ticketCnt = 1;
    ESTicketView* tik = NULL;
    ESTitleId titleId ATTRIBUTE_ALIGN(32);
    __OSPlayTimeType type = OSPLAYTIME_PERMANENT;
    u32 remain = 0;
    u8* bi2 = NULL;
    OSNandbootInfo* info = NULL;
    OSStateFlags state;

    OSSetArenaLo((void*)0x81280000);
    OSSetArenaHi((void*)0x812F0000);

    rc = ESP_InitLib();
    if (rc != 0) {
        __OSReturnToMenuForError();
    }

    rc = ESP_GetTitleId(&titleId);
    if (rc != 0) {
        __OSReturnToMenuForError();
    }

    tik = (ESTicketView*)(OSAllocFromMEM1ArenaLo)(OSRoundUp32B(sizeof(ESTicketView)), 32);
    if (tik == NULL) {
        __OSReturnToMenuForError();
    }

    memset(tik, 0, OSRoundUp32B(sizeof(ESTicketView)));
    rc = ESP_DiGetTicketView(NULL, tik);
    if (rc == -1017) {
        ESTicketView* tmp = NULL;
        rc = ESP_GetTicketViews(titleId, NULL, &ticketCnt);
        if (rc != 0) {
            __OSReturnToMenuForError();
        }

        tmp = (ESTicketView*)OSAllocFromMEM1ArenaLo(OSRoundUp32B(ticketCnt * sizeof(ESTicketView)), 32);
        if (tmp == NULL) {
            __OSReturnToMenuForError();
        }

        rc = ESP_GetTicketViews(titleId, tmp, &ticketCnt);
        if (rc != 0) {
            __OSReturnToMenuForError();
        }

        memcpy(tik, tmp, sizeof(ESTicketView));
    } else if (rc != 0) {
        __OSReturnToMenuForError();
    } else {
        if (OSPlayTimeIsLimited()) {
            __OSPlayTimeType type = OSPLAYTIME_PERMANENT;
            u32 remain = 0xFFFFFFFF;
            __OSGetPlayTime(tik, &type, &remain);
            if (remain == 0) {
                __OSWriteExpiredFlag();
                __OSReturnToMenuForError();
            }
        }
    }

    bi2 = (u8*)OSAllocFromMEM1ArenaLo(0x2000, 64);
    info = (OSNandbootInfo*)((u32)bi2 + (0x2000 - sizeof(OSNandbootInfo)));
    memset(bi2, 0, 0x2000);
    info->LastTitleId = titleId;
    info->LastAppType = OSGetAppType();
    info->ReturnType = 1;
    info->ArgValue = resetCode | 0x80000000;

    __OSCreateNandbootInfo();
    __OSWriteNandbootInfo(info);
    __OSReadStateFlags(&state);
    state.lastShutdown = 3;
    __OSWriteStateFlags(&state);
    rc = ESP_LaunchTitle(titleId, tik);

    if (rc != 0) {
        __OSReturnToMenuForError();
    }

    while (1) {

    }
}
