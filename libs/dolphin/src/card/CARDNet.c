#include <dolphin/card.h>

#include "os/__os.h"
#include "__card.h"

u16 __CARDVendorID = 0xFFFF;
u8 __CARDPermMask = 0x1C;

u16 CARDSetVendorID(u16 vendorID) {
    u16 prevID = __CARDVendorID;
    __CARDVendorID = vendorID;

    return prevID;
}

u16 CARDGetVendorID() {
    return __CARDVendorID;
}

s32 CARDGetSerialNo(s32 chan, u64* serialNo) {
    CARDControl* card;
    s32 result;
    CARDID* id;
    u64 code;
    int i;

    ASSERTLINE(105, 0 <= chan && chan < 2);

    if (!(0 <= chan && chan < 2)) {
        return CARD_RESULT_FATAL_ERROR;
    }

    result = __CARDGetControlBlock(chan, &card);
    if (result < 0) {
        return result;
    }

    id = (CARDID*)card->workArea;
    for (code = 0, i = 0; i < sizeof(id->serial) / sizeof(u64); ++i) {
        code ^= *(u64*)&id->serial[sizeof(u64) * i];
    }
    *serialNo = code;

    return __CARDPutControlBlock(card, CARD_RESULT_READY);
}

s32 CARDGetUniqueCode(s32 chan, u64* uniqueCode) {
    CARDControl* card;
    s32 result;
    OSSramEx* sram;

    ASSERTLINE(146, 0 <= chan && chan < 2);

    if (!(0 <= chan && chan < 2)) {
        return CARD_RESULT_FATAL_ERROR;
    }

    result = __CARDGetControlBlock(chan, &card);
    if (result < 0) {
        return result;
    }

    sram = __OSLockSramEx();
    memcpy(uniqueCode, &sram->flashID[chan][4], 8);
    __OSUnlockSramEx(0);
    return __CARDPutControlBlock(card, CARD_RESULT_READY);
}

s32 CARDGetAttributes(s32 chan, s32 fileNo, u8* attr) {
    CARDDir dirent;
    s32 result;

    result = __CARDGetStatusEx(chan, fileNo, &dirent);
    if (result == 0) {
        *attr = dirent.permission;
    }

    return result;
}

#define CARDCheckAttr(attr, flag) ((u32)(attr & flag) != 0)

s32 CARDSetAttributesAsync(s32 chan, s32 fileNo, u8 attr, CARDCallback callback) {
    CARDDir dirent;
    s32 result;

    if (attr & ~__CARDPermMask) {
        return CARD_RESULT_NOPERM;
    }

    result = __CARDGetStatusEx(chan, fileNo, &dirent);
    if (result < 0) {
        return result;
    }

    if ((CARDCheckAttr(dirent.permission, 0x20) && !CARDCheckAttr(attr, 0x20)) || (CARDCheckAttr(dirent.permission, 0x40) && !CARDCheckAttr(attr, 0x40))) {
        return CARD_RESULT_NOPERM;
    }

    if ((CARDCheckAttr(attr, 0x20) && CARDCheckAttr(attr, 0x40)) || (CARDCheckAttr(attr, 0x20) && CARDCheckAttr(dirent.permission, 0x40)) || (CARDCheckAttr(attr, 0x40) && CARDCheckAttr(dirent.permission, 0x20))) {
        return CARD_RESULT_NOPERM;
    }

    dirent.permission = attr;
    return __CARDSetStatusExAsync(chan, fileNo, &dirent, callback);
}

s32 CARDSetAttributes(s32 chan, s32 fileNo, u8 attr) {
    s32 result;

    result = CARDSetAttributesAsync(chan, fileNo, attr, __CARDSyncCallback);
    if (result < 0) {
        return result;
    }

    return __CARDSync(chan);
}

static int __CARDEnablePerm(u8 perm, BOOL enable) {
    int prev;
    prev = __CARDPermMask & perm ? TRUE : FALSE;

    if (enable) {
        __CARDPermMask |= perm;
    } else {
        __CARDPermMask &= ~perm;
    }

    return prev;
}

int __CARDEnableGlobal(BOOL enable) {
    return __CARDEnablePerm(0x20, enable);
}

int __CARDEnableCompany(BOOL enable) {
    return __CARDEnablePerm(0x40, enable);
}
