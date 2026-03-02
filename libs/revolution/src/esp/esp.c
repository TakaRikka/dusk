#include <revolution/esp.h>
#include <revolution/ipc/ipcclt.h>
#include <revolution/private/iosrestypes.h>

static s32 __esFd = -1;

s32 ESP_InitLib(void) {
    s32 ret = 0;

    if (__esFd < 0) {
        __esFd = IOS_Open("/dev/es", 0);

        if (__esFd < 0) {
            ret = __esFd;
        }
    }

    return ret;
}

s32 ESP_CloseLib(void) {
    s32 ret = 0;

    if (__esFd >= 0) {
        ret = IOS_Close(__esFd);
        if (ret == 0) {
            __esFd = -1;
        }
    }

    return ret;
}

s32 ESP_LaunchTitle(u64 titleID, ESTicketView* pTicketView) {
    s32 ret = 0;
    u8 buf[256] ATTRIBUTE_ALIGN(32);
    IOSIoVector* vec = (IOSIoVector*)(buf + 208);
    u64* id = (u64*)buf;

    if (__esFd < 0) {
        ret = -0x3F9;
        goto out;
    }

    // check if ptr is aligned to 0x20
    if ((u32)pTicketView & 31) {
        ret = -0x3F9;
        goto out;
    }

    *id = titleID;
    vec[0].base = (u8*)id;
    vec[0].length = 8;
    vec[1].base = (u8*)pTicketView;
    vec[1].length = 0xD8;

    ret = IOS_IoctlvReboot(__esFd, 8, 2, 0, vec);
    __esFd = -1;

out:
    return ret;
}

s32 ESP_GetTicketViews(ESTitleId titleId, ESTicketView* ticketViewList, u32* ticketViewCnt) {
    s32 rv = 0;
    u8 __esBuf[256] ATTRIBUTE_ALIGN(32);
    IOSIoVector* v = (IOSIoVector*)(__esBuf + 256 - 6 * sizeof(IOSIoVector));
    ESTitleId* p1 = (ESTitleId*)__esBuf;
    u32* p2 = (u32*)(__esBuf + 32);

    if (__esFd < 0 || ticketViewCnt == NULL) {
        rv = -1017;
        goto out;
    }

    if (((u32)ticketViewList & 31)) {
        rv = -1017;
        goto out;
    }

    *p1 = titleId;
    if (ticketViewList == NULL) {
        v[0].base = (u8*)p1;
        v[0].length = sizeof(ESTitleId);
        v[1].base = (u8*)p2;
        v[1].length = 4;
        rv = IOS_Ioctlv(__esFd, 18, 1, 1, v);

        if (rv == 0) {
            *ticketViewCnt = *p2;
        }
    } else {
        if (*ticketViewCnt == 0) {
            rv = -1017;
            goto out;
        }

        *p2 = *ticketViewCnt;
        v[0].base = (u8*)p1;
        v[0].length = sizeof(ESTitleId);
        v[1].base = (u8*)p2;
        v[1].length = 4;
        v[2].base = (u8*)ticketViewList;
        v[2].length = sizeof(ESTicketView) * (*ticketViewCnt);
        rv = IOS_Ioctlv(__esFd, 19, 2, 1, v);
    }

out:
    return rv;
}

s32 ESP_DiGetTicketView(const void* ticket, ESTicketView* ticketView) {
    s32 rv = 0;
    u8 __esBuf[256] ATTRIBUTE_ALIGN(32);
    IOSIoVector* v = (IOSIoVector*)(__esBuf + 256 - 6 * sizeof(IOSIoVector));

    if (__esFd < 0 || ticketView == NULL) {
        rv = -1017;
        goto out;
    }

    if (((u32)ticket & 31) || ((u32)ticketView & 31)) {
        rv = -1017;
        goto out;
    }

    v[0].base = (u8*)ticket;

    if (ticket == NULL) {
        v[0].length = 0;
    } else {
        v[0].length = sizeof(ESTicket);
    }

    v[1].base = (u8*)ticketView;
    v[1].length = sizeof(ESTicketView);
    rv = IOS_Ioctlv(__esFd, 27, 1, 1, v);

out:
    return rv;
}

s32 ESP_DiGetTmd(ESTitleMeta* tmd, u32* tmdSize) {
    s32 rv = 0;
    u8 __esBuf[256] ATTRIBUTE_ALIGN(32);
    IOSIoVector* v = (IOSIoVector*)(__esBuf + 256 - 6 * sizeof(IOSIoVector));
    u32* p1 = (u32*)__esBuf;

    if (__esFd < 0 || tmdSize == NULL) {
        rv = -1017;
        goto out;
    }

    if (((u32)tmd & 31)) {
        rv = -1017;
        goto out;
    }

    if (tmd == NULL) {
        v[0].base = (u8*)p1;
        v[0].length = 4;
        rv = IOS_Ioctlv(__esFd, 57, 0, 1, v);

        if (rv == 0) {
            *tmdSize = *p1;
        }
    } else {
        if (*tmdSize == 0) {
            rv = -1017;
            goto out;
        }

        *p1 = *tmdSize;
        v[0].base = (u8*)p1;
        v[0].length = 4;
        v[1].base = (u8*)tmd;
        v[1].length = *tmdSize;
        rv = IOS_Ioctlv(__esFd, 58, 1, 1, v);
    }

out:
    return rv;
}

s32 ESP_GetTmdView(ESTitleId titleId, ESTmdView* tmdView, u32* size) {
    s32 rv = 0;
    u8 __esBuf[256] ATTRIBUTE_ALIGN(32);
    IOSIoVector* v = (IOSIoVector*)(__esBuf + 256 - 6 * sizeof(IOSIoVector));
    ESTitleId* p1 = (ESTitleId*)__esBuf;
    u32* p2 = (u32*)(__esBuf + 32);

    if (__esFd < 0 || size == NULL) {
        rv = -1017;
        goto out;
    }

    if (((u32)tmdView & 31)) {
        rv = -1017;
        goto out;
    }

    *p1 = titleId;

    if (tmdView == NULL) {
        v[0].base = (u8*)p1;
        v[0].length = sizeof(ESTitleId);
        v[1].base = (u8*)p2;
        v[1].length = 4;
        rv = IOS_Ioctlv(__esFd, 20, 1, 1, v);

        if (rv == 0) {
            *size = *p2;
        }
    } else {
        if (*size == 0) {
            rv = -1017;
            goto out;
        }

        *p2 = *size;
        v[0].base = (u8*)p1;
        v[0].length = sizeof(ESTitleId);
        v[1].base = (u8*)p2;
        v[1].length = 4;
        v[2].base = (u8*)tmdView;
        v[2].length = *size;
        rv = IOS_Ioctlv(__esFd, 21, 2, 1, v);
    }

out:
    return rv;
}

s32 ESP_GetDataDir(ESTitleId titleId, char* dataDir) {
    s32 rv = 0;
    u8 __esBuf[256] ATTRIBUTE_ALIGN(32);
    IOSIoVector* v = (IOSIoVector*)(__esBuf + 256 - 6 * sizeof(IOSIoVector));
    ESTitleId* p1 = (ESTitleId*)__esBuf;

    if (__esFd < 0 || dataDir == NULL) {
        rv = -1017;
        goto out;
    }

    if (((u32)dataDir & 31)) {
        rv = -1017;
        goto out;
    }

    *p1 = titleId;
    v[0].base = (u8*)p1;
    v[0].length = sizeof(ESTitleId);
    v[1].base = (u8*)dataDir;
    v[1].length = 0x1E;
    rv = IOS_Ioctlv(__esFd, 29, 1, 1, v);

out:
    return rv;
}

s32 ESP_GetTitleId(ESTitleId* titleId) {
    s32 rv = 0;
    u8 __esBuf[256] ATTRIBUTE_ALIGN(32);
    IOSIoVector* v = (IOSIoVector*)(__esBuf + 256 - 6 * sizeof(IOSIoVector));

    if (__esFd < 0 || titleId == NULL) {
        rv = -1017;
        goto out;
    }

    v[0].base = __esBuf;
    v[0].length = sizeof(ESTitleId);

    rv = IOS_Ioctlv(__esFd, 32, 0, 1, v);

    if (rv == 0) {
        *titleId = *(ESTitleId*)__esBuf;
    }

out:
    return rv;
}

s32 ESP_GetConsumption(ESTicketId ticketId, ESLpEntry* entries, u32* nEntries) {
    s32 rv = 0;
    u8 __esBuf[256] ATTRIBUTE_ALIGN(32);
    IOSIoVector* v = (IOSIoVector*)(__esBuf + 256 - 6 * sizeof(IOSIoVector));
    ESTicketId* p1 = (ESTicketId*)__esBuf;
    u32* p2 = (u32*)(__esBuf + 32);

    if (__esFd < 0) {
        rv = -1017;
        goto out;
    }

    if (((u32)entries & 31)) {
        rv = -1017;
        goto out;
    }

    *p1 = ticketId;
    v[0].base = (u8*)p1;
    v[0].length = sizeof(ESTicketId);

    if (entries == NULL) {
        v[1].base = NULL;
        v[1].length = 0;
    } else {
        v[1].base = (u8*)entries;
        v[1].length = sizeof(ESLpEntry) * (*nEntries);
        *p2 = *nEntries;
    }

    v[2].base = (u8*)p2;
    v[2].length = 4;
    rv = IOS_Ioctlv(__esFd, 22, 1, 2, v);
    *nEntries = *p2;
out:
    return rv;
}
