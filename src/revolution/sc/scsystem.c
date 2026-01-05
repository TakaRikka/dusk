#include <revolution/types.h>
#include <revolution/sc.h>
#include <cstring>
// #include <mem.h>

#ifdef SDK_AUG2010
#define BUILD_DATE "Aug 23 2010"
#if DEBUG
#define BUILD_TIME "17:28:26"
#else
#define BUILD_TIME "17:33:06"
#endif
#elif SDK_SEP2006
#define BUILD_DATE "Sep 21 2006"
#define BUILD_TIME "14:32:13"
#endif

#ifdef SDK_AUG2010
#if DEBUG
const char* __SCVersion = "<< RVL_SDK - SC \tdebug build: " BUILD_DATE " " BUILD_TIME " (0x4302_145) >>";
#else
const char* __SCVersion = "<< RVL_SDK - SC \trelease build: " BUILD_DATE " " BUILD_TIME " (0x4302_145) >>";
#endif
#elif SDK_SEP2006
const char* __SCVersion = "<< RVL_SDK - SC \trelease build: " BUILD_DATE " " BUILD_TIME " (0x4200_60422) >>";
#endif

typedef struct {
    char* name;
    SCItemID id;
} NameAndID;

static NameAndID NameAndIDTbl[38] = {
    "IPL.CB",     SC_ITEM_ID_IPL_COUNTER_BIAS,
    "IPL.AR",     SC_ITEM_ID_IPL_ASPECT_RATIO,
    "IPL.ARN",    SC_ITEM_ID_IPL_AUTORUN_MODE,
    "IPL.CD",     SC_ITEM_ID_IPL_CONFIG_DONE,
    "IPL.CD2",    SC_ITEM_ID_IPL_CONFIG_DONE2,
    "IPL.DH",     SC_ITEM_ID_IPL_DISPLAY_OFFSET_H,
    "IPL.E60",    SC_ITEM_ID_IPL_EURGB60_MODE,
    "IPL.EULA",   SC_ITEM_ID_IPL_EULA,
    "IPL.FRC",    SC_ITEM_ID_IPL_FREE_CHANNEL_APP_COUNT,
    "IPL.IDL",    SC_ITEM_ID_IPL_IDLE_MODE,
    "IPL.INC",    SC_ITEM_ID_IPL_INSTALLED_CHANNEL_APP_COUNT,
    "IPL.LNG",    SC_ITEM_ID_IPL_LANGUAGE,
    "IPL.NIK",    SC_ITEM_ID_IPL_OWNER_NICKNAME,
    "IPL.PC",     SC_ITEM_ID_IPL_PARENTAL_CONTROL,
    "IPL.PGS",    SC_ITEM_ID_IPL_PROGRESSIVE_MODE,
    "IPL.SSV",    SC_ITEM_ID_IPL_SCREEN_SAVER_MODE,
    "IPL.SADR",   SC_ITEM_ID_IPL_SIMPLE_ADDRESS,
    "IPL.SND",    SC_ITEM_ID_IPL_SOUND_MODE,
    "IPL.UPT",    SC_ITEM_ID_IPL_UPDATE_TYPE,
    "NET.CNF",    SC_ITEM_ID_NET_CONFIG,
    "NET.CTPC",   SC_ITEM_ID_NET_CONTENT_RESTRICTIONS,
    "NET.PROF",   SC_ITEM_ID_NET_PROFILE,
    "NET.WCPC",   SC_ITEM_ID_NET_WC_RESTRICTION,
    "NET.WCFG",   SC_ITEM_ID_NET_WC_FLAGS,
    "DEV.BTM",    SC_ITEM_ID_DEV_BOOT_MODE,
    "DEV.VIM",    SC_ITEM_ID_DEV_VIDEO_MODE,
    "DEV.CTC",    SC_ITEM_ID_DEV_COUNTRY_CODE,
    "DEV.DSM",    SC_ITEM_ID_DEV_DRIVESAVING_MODE,
    "BT.DINF",    SC_ITEM_ID_BT_DEVICE_INFO,
    "BT.CDIF",    SC_ITEM_ID_BT_CMPDEV_INFO,
    "BT.SENS",    SC_ITEM_ID_BT_DPD_SENSIBILITY,
    "BT.SPKV",    SC_ITEM_ID_BT_SPEAKER_VOLUME,
    "BT.MOT",     SC_ITEM_ID_BT_MOTOR_MODE,
    "BT.BAR",     SC_ITEM_ID_BT_SENSOR_BAR_POSITION,
    "DVD.CNF",    SC_ITEM_ID_DVD_CONFIG,
    "WWW.RST",    SC_ITEM_ID_WWW_RESTRICTION,
    "MPLS.MOVIE", SC_ITEM_ID_MOTION_PLUS_MOVIE,
    "IPL.TID",    SC_ITEM_ID_TEMPORARY_TITLE_ID,
};

static const char ConfDirName[] = "/shared2/sys";
static const char ConfFileName[] = "/shared2/sys/SYSCONF";
static const char ProductInfoFileName[] = "/title/00000001/00000002/data/setting.txt";

static u8 ConfBuf[0x4000] ATTRIBUTE_ALIGN(32);
static u8 ConfBufForFlush[0x4000] ATTRIBUTE_ALIGN(32);

static u8 Initialized;
static u8 DirtyFlag;
static u8 IsDevKit;
static vu8 BgJobStatus = 0;
static u32 ItemIDOffsetTblOffset;
static u32 ItemIDMaxPlus1;
static u32 ItemNumTotal;
static u32 ItemRestSize;
static SCControl Control;

static void SetBgJobStatus(u32 status);
static u32 ParseConfBuf(u8* bufp, u32 bufSize);
static void ClearConfBuf(u8* bufp);
static BOOL ParseGetBEValue(const u8* bufp, const u8* bufEndp, u32* varp, u32 varSize);
static void OpenCallbackFromReload(s32, NANDCommandBlock*);
static void ReadCallbackFromReload(s32 result, NANDCommandBlock* block);
static void CloseCallbackFromReloadError(s32 result, NANDCommandBlock* block);
static void CloseCallbackFromReload(s32 result, NANDCommandBlock* block);
static void ErrorFromReload(s32 result);
static void FinishFromReload(void);
static BOOL UnpackItem(const u8*, SCItem*);
static void MyNandCallback(s32, NANDCommandBlock* block);
static void FinishFromFlush(void);
static void ErrorFromFlush(void);

static BOOL __SCIsDirty(void);
static void __SCSetDirtyFlag(void);
static void __SCClearDirtyFlag(void);
static u8* __SCGetConfBuf(void);
static u32 __SCGetConfBufSize(void);

void SCInit(void) {
    BOOL enabled = OSDisableInterrupts();

    if (Initialized) {
        OSRestoreInterrupts(enabled);
        return;
    }

    Initialized = TRUE;
    SetBgJobStatus(1);
    OSRestoreInterrupts(enabled);

    OSRegisterVersion(__SCVersion);
    OSInitThreadQueue(&Control.threadQueue);

    if (OSGetConsoleType() & 0x10000000) {
        IsDevKit = TRUE;
    }

    if (NANDInit() != 0 || SCReloadConfFileAsync(__SCGetConfBuf(), __SCGetConfBufSize(), NULL) != 0) {
        SetBgJobStatus(2);
    }
}

static void SetBgJobStatus(u32 status) {
    BgJobStatus = (u8)status;
}

u32 SCCheckStatus(void) {
    BOOL enabled;
    u32 ret;

    enabled = OSDisableInterrupts();
    ret = BgJobStatus;

    if (ret == 3) {
        SetBgJobStatus(1);
        OSRestoreInterrupts(enabled);

        if (ParseConfBuf(Control.reloadBufp[0], Control.reloadedSize[0]) == 0) {
            enabled = OSDisableInterrupts();

            if (__SCGetConfBuf() != Control.reloadBufp[0]) {
                memcpy(__SCGetConfBuf(), Control.reloadBufp[0], __SCGetConfBufSize());
            }

            __SCClearDirtyFlag();
            OSRestoreInterrupts(enabled);
        } else {
            enabled = OSDisableInterrupts();
            ClearConfBuf(Control.reloadBufp[0]);
            __SCClearDirtyFlag();
            OSRestoreInterrupts(enabled);
        }

        ret = 0;
        SetBgJobStatus(ret);
    } else {
        OSRestoreInterrupts(enabled);
    }

    return ret;
}

s32 SCReloadConfFileAsync(u8* bufp, u32 bufSize, SCReloadConfFileCallback callback) {
    u32 i;

    ASSERTLINE(392, bufp != NULL);
    ASSERTLINE(393, ((u32)bufp & 0x1f) == 0);

    if (bufSize < __SCGetConfBufSize()) {
        return -128;
    }

    SetBgJobStatus(1);
    Control.reloadCallback = callback;
    Control.reloadResult = 0;
    Control.reloadFileCount = 0;

    for (i = 0; i < 2; i++) {
        Control.reloadedSize[i] = 0;
    }

    Control.reloadFileName[0] = ConfFileName;
    Control.reloadFileName[1] = ProductInfoFileName;
    Control.reloadBufp[0] = bufp;
    Control.reloadBufp[1] = (u8*)OSPhysicalToCached(0x3800);
    Control.reloadSizeExpected[0] = __SCGetConfBufSize();
    Control.reloadSizeExpected[1] = 0x100;
    ClearConfBuf(bufp);
    ItemIDOffsetTblOffset = 0;
    ItemNumTotal = 0;
    ItemRestSize = 0;
    Control.nandNeedClose = FALSE;
    return NANDPrivateOpenAsync(Control.reloadFileName[Control.reloadFileCount],
                                &Control.nandFileInfo, 1, OpenCallbackFromReload,
                                &Control.nandCommandBlock);
}

static void OpenCallbackFromReload(s32 result, NANDCommandBlock* block) {
    if (result == 0) {
        Control.nandNeedClose = TRUE;

        if (NANDReadAsync(&Control.nandFileInfo, Control.reloadBufp[Control.reloadFileCount],
                          Control.reloadSizeExpected[Control.reloadFileCount],
                          ReadCallbackFromReload, &Control.nandCommandBlock) == 0)
        {
            return;
        }
    }

    ErrorFromReload(result);
}

static void ReadCallbackFromReload(s32 result, NANDCommandBlock* block) {
    if (result == Control.reloadSizeExpected[Control.reloadFileCount]) {
        Control.reloadedSize[Control.reloadFileCount] = (u32)result;
        Control.nandNeedClose = FALSE;

        if (NANDCloseAsync(&Control.nandFileInfo, CloseCallbackFromReload,
                           &Control.nandCommandBlock) == 0)
        {
            return;
        }
    }

    ErrorFromReload((s32)((result == 0) ? -128 : result));
}

static void CloseCallbackFromReload(s32 result, NANDCommandBlock* block) {
    if (result == 0) {
        FinishFromReload();
        return;
    }

    ErrorFromReload(result);
}

void FinishFromReload(void) {
    u32 status;

nextFile:
    Control.reloadFileCount++;

    if (Control.reloadFileCount < 2) {
        Control.nandNeedClose = FALSE;

        if (NANDPrivateOpenAsync(Control.reloadFileName[Control.reloadFileCount],
                                 &Control.nandFileInfo, 1, OpenCallbackFromReload,
                                 &Control.nandCommandBlock) == 0)
        {
            return;
        }

        goto nextFile;
    }

    switch (Control.reloadResult) {
    case 0:
        status = 3;
        break;
    default:
    case -12:
        ClearConfBuf(Control.reloadBufp[0]);
        Control.reloadedSize[0] = Control.reloadSizeExpected[0];
        status = 3;
        break;
    }

    *(u8*)((u8*)OSPhysicalToCached(0x3800) + 0x100 - 1) = '\0';

    if (Control.reloadCallback) {
        Control.reloadCallback(Control.reloadResult);
        Control.reloadCallback = NULL;
    }

    SetBgJobStatus(status);
}

static void ErrorFromReload(s32 result) {
    if (Control.reloadFileCount == 0) {
        Control.reloadResult = result;
    }

    Control.reloadedSize[Control.reloadFileCount] = 0;

    if (Control.nandNeedClose) {
        if (NANDCloseAsync(&Control.nandFileInfo, CloseCallbackFromReloadError,
                           &Control.nandCommandBlock) == 0)
        {
            return;
        }
    }

    FinishFromReload();
}

static void CloseCallbackFromReloadError(s32 result, NANDCommandBlock* block) {
    FinishFromReload();
}

static void ClearConfBuf(u8* bufp) {
    u32 size = __SCGetConfBufSize();
    memset(bufp, 0, size);

    if (size > 12) {
        memcpy(bufp, "SCv0", 4);
        memcpy(bufp + (size - 4), "SCed", 4);
        *((u16*)bufp + 3) = 8;
    }
}

u32 ParseConfBuf(u8* bufp, u32 bufSize) {
    u8 *bufTop, *bufEndp;
    u32 numItems, loopItem;
    SCItem item;
    u32 itemOffset;
    u16* itemOfsp;
    u16* runtimeRefp;
    u32 restSize;
    NameAndID* tblp = NameAndIDTbl;
    NameAndID* tblEndp;
    char* name;

    ASSERTLINE(576, bufp != NULL);
    ASSERTLINE(577, ((u32)bufp & 0x1f) == 0);

    if ((bufSize < 12) || (bufSize > __SCGetConfBufSize())) {
        goto error;
    }

    bufTop = bufp;
    bufEndp = bufp + bufSize - 4;
    ItemIDMaxPlus1 = SC_ITEM_ID_MAX_PLUS1;

    if (memcmp(bufp, "SCv0", 4) || memcmp(bufEndp, "SCed", 4)) {
        goto error;
    }

    bufp += 4;

    if (bufSize < __SCGetConfBufSize()) {
        u32 expandSize = __SCGetConfBufSize() - bufSize;
        memset(bufEndp, 0, expandSize);
        bufEndp += expandSize;
        memcpy(bufEndp, "SCed", 4);
    }

    if (ParseGetBEValue(bufp, bufEndp, &numItems, 2) == FALSE) {
        goto error;
    }

    bufp += 2;
    itemOfsp = (u16*)bufp;
    itemOffset = ((u8*)(itemOfsp + numItems + 1) - bufTop);

    for (loopItem = 0; loopItem < numItems; loopItem++) {
        if (itemOffset > bufSize || ((u8*)&itemOfsp[loopItem] - bufTop) > bufSize) {
            goto error;
        }

        if (itemOffset != itemOfsp[loopItem] || UnpackItem(bufTop + itemOffset, &item) == FALSE) {
            goto error;
        }

        itemOffset += item.packedSize;
    }

    if (itemOffset > bufSize) {
        goto error;
    }

    if (itemOffset != itemOfsp[loopItem]) {
        goto error;
    }

    runtimeRefp = (u16*)(bufEndp - 2 * (SC_ITEM_ID_MAX_PLUS1));

    if ((u8*)(bufTop + itemOffset) > (u8*)runtimeRefp) {
        goto error;
    }

    restSize = (u32)((u8*)runtimeRefp - (u8*)(bufTop + itemOffset));
    memset(runtimeRefp, 0, (u32)(bufEndp - (u8*)runtimeRefp));
    runtimeRefp = (u16*)(bufEndp - 2);
    tblEndp = tblp + ItemIDMaxPlus1;

    while (tblp < tblEndp && (name = tblp->name) != NULL) {
        u32 nameLen;
        u8* p;

        nameLen = strlen(name);

        for (loopItem = 0; loopItem < numItems; loopItem++) {
            p = bufTop + itemOfsp[loopItem];

            if (nameLen == (u32)(((*p) & 0x1F) + 1) &&
                memcmp(name, p + sizeof(SCType), nameLen) == 0)
            {
                runtimeRefp[-tblp->id] = (u16)((u8*)(&itemOfsp[loopItem]) - bufTop);
                break;
            }
        }

        tblp++;
    }

    ItemIDOffsetTblOffset = (u32)((u8*)runtimeRefp - bufTop);
    ItemNumTotal = numItems;
    ItemRestSize = restSize;
    return 0;

error:
    return 2;
}

static BOOL ParseGetBEValue(const u8* bufp, const u8* bufEndp, u32* varp, u32 varSize) {
    u32 value = 0;

    if ((bufp + varSize) > bufEndp) {
        return FALSE;
    }

    while (varSize) {
        value = (value << 8) | *bufp;
        bufp++;
        varSize--;
    }

    *varp = value;
    return TRUE;
}

static BOOL UnpackItem(const u8* bufp, SCItem* itemp) {
    SCType type;

    ASSERTLINE(698, bufp != NULL);
    ASSERTLINE(699, itemp != NULL);

    memset(itemp, 0, sizeof(SCItem));
    type = (u8)(*bufp & 0xE0);
    itemp->name = (char*)(bufp + sizeof(SCType));
    itemp->nameLen = (u32)((*bufp & 0x1F) + 1);
    itemp->data = (u8*)(bufp + sizeof(SCType) + itemp->nameLen);

    switch (type) {
    case 0x60:
    case 0xE0:
        itemp->dataSize = sizeof(u8);
        break;
    case 0x80:
        itemp->dataSize = sizeof(u16);
        break;
    case 0xA0:
        itemp->dataSize = sizeof(u32);
        break;
    case 0xC0:
        itemp->dataSize = sizeof(u64);
        break;
    case 0x40:
        itemp->dataSize = (u32)(*(itemp->data) + 1);
        itemp->data++;
        itemp->packedSize++;
        break;
    case 0x20:
        itemp->dataSize = (u32)((((*(itemp->data)) << 8) | (*(itemp->data + 1))) + 1);
        itemp->data += 2;
        itemp->packedSize += 2;
        break;
    default:
        goto err;
    }

    if (type == 0x40 || type == 0x20) {
        itemp->typeByteArray = 0x40;
    } else {
        itemp->typeInteger = type;
        memcpy(&itemp->integer, itemp->data, itemp->dataSize);
    }

    itemp->packedSize += sizeof(SCType) + itemp->nameLen + itemp->dataSize;

    {
        const char* typeString;
        switch (type) {
        case 0x60:
            typeString = "U8/S8";
            break;
        case 0xE0:
            typeString = "BOOL";
            break;
        case 0x80:
            typeString = "U16/S16";
            break;
        case 0xA0:
            typeString = "U32/S32";
            break;
        case 0xC0:
            typeString = "U64/S64";
            break;
        case 0x40:
        case 0x20:
            typeString = "ARRAY";
            break;
        default:
            typeString = "UNKNOWN";
        }
    }

err:
    return (itemp->dataSize != 0);
}

static BOOL FindItemByID(SCItemID id, SCItem* itemp) {
    u8* conf = __SCGetConfBuf();
    u16* refp;

    if (id < ItemIDMaxPlus1 && ItemIDOffsetTblOffset != 0) {
        refp = (u16*)(conf + ItemIDOffsetTblOffset);
        if (refp[-id] != 0) {
            return UnpackItem(conf + *(u16*)(conf + refp[-id]), itemp);
        }
    }

    return FALSE;
}

void DeleteItemByID(SCItemID id) {
    u8* conf = __SCGetConfBuf();
    u32 targetRef, initialTopOfFreeSpace, moveSize, shrinkSize, i;
    u16 *refp, *itemOfsTop, *itemOfsTargetp, *itemOfsEndp, *itemOfsp;

    if (id < ItemIDMaxPlus1 && ItemIDOffsetTblOffset != 0) {
        refp = (u16*)(conf + ItemIDOffsetTblOffset);
        targetRef = refp[-id];

        if (targetRef != 0 && ItemNumTotal != 0) {
            itemOfsTop = (u16*)(conf + 4);
            itemOfsTargetp = (u16*)(conf + targetRef);
            itemOfsEndp = itemOfsTop + ItemNumTotal;
            initialTopOfFreeSpace = *itemOfsEndp;
            shrinkSize = 2 + (itemOfsTargetp[1] - itemOfsTargetp[0]);

            moveSize = *itemOfsTargetp - (targetRef + 4);
            memmove(conf + targetRef, conf + targetRef + 4, moveSize);

            for (itemOfsp = itemOfsEndp - 1; itemOfsp >= itemOfsTop; itemOfsp--) {
                if (itemOfsp < itemOfsTargetp) {
                    *itemOfsp -= 2;
                } else {
                    *itemOfsp -= shrinkSize;
                }
            }

            memmove(conf + itemOfsTargetp[0], conf + (itemOfsTargetp[0] + shrinkSize),
                    initialTopOfFreeSpace - (itemOfsTargetp[0] + shrinkSize));
            memset(conf + (initialTopOfFreeSpace - shrinkSize), 0, shrinkSize);

            for (i = 0; i < ItemIDMaxPlus1; i++) {
                if (refp[-i] < targetRef) {
                    // empty branch?
                } else if (refp[-i] > targetRef) {
                    refp[-i] -= 2;
                } else {
                    refp[-i] = 0;
                }
            }
            ItemRestSize += shrinkSize;
            ItemNumTotal--;
            *(u16*)(conf + 4) = (u16)ItemNumTotal;
            __SCSetDirtyFlag();
        }
    }
}

BOOL CreateItemByID(SCItemID id, SCType type, const u8* data, u32 size) {
    u8* conf = __SCGetConfBuf();
    u8* p;
    u32 nameLen;
    u32 packedSize = sizeof(SCType);
    NameAndID* tblp = NameAndIDTbl;
    char* name;
    u32 topOfFreeSpace;
    u16* refp;
    u16* itemOfsTop;
    u16* itemOfsEndp;
    u16* itemOfsp;

    if (id < ItemIDMaxPlus1 && data != NULL && ItemNumTotal < 0xFFFF && ItemIDOffsetTblOffset != 0) {
        switch (type) {
        case 0xE0:
        case 0x60:
            size = 1;
            break;
        case 0x80:
            size = 2;
            break;
        case 0xA0:
            size = 4;
            break;
        case 0xC0:
            size = 8;
            break;
        case 0x40:
            if (size == 0 || size > 65536) {
                goto error;
            }
            if (size > 256) {
                packedSize += 2;
                type = 0x20;
            } else {
                packedSize += 1;
            }
            break;

        default:
            goto error;
        }

        packedSize += size;

        while ((name = tblp->name) != NULL) {
            if (tblp->id == id) {
                break;
            }

            tblp++;
        }

        if (name == NULL) {
            goto error;
        }

        nameLen = strlen(name);
        if (nameLen > 32) {
            goto error;
        }

        packedSize += nameLen;
        if (ItemRestSize < (2 + packedSize)) {
            goto error;
        }

        itemOfsTop = (u16*)(conf + 6);
        itemOfsEndp = itemOfsTop + ItemNumTotal;
        topOfFreeSpace = *itemOfsEndp;
        memmove(conf + (itemOfsTop[0] + 2), conf + itemOfsTop[0], topOfFreeSpace - itemOfsTop[0]);

        itemOfsp = itemOfsTop;
        do {
            *itemOfsp += 2;
            itemOfsp++;
        } while (itemOfsp <= itemOfsEndp);

        topOfFreeSpace = *itemOfsEndp;
        p = (u8*)(conf + topOfFreeSpace);

        *p = (SCType)(type | (nameLen - 1));
        memcpy(p + sizeof(SCType), name, nameLen);
        p += sizeof(SCType) + nameLen;
        if (type == 0x40) {
            *p++ = (u8)(size - 1);
        } else if (type == 0x20) {
            *p++ = (u8)((size - 1) >> 8);
            *p++ = (u8)(size - 1);
        }
        memcpy(p, data, size);
        p += size;

        refp = (u16*)(conf + ItemIDOffsetTblOffset);
        refp[-id] = (u16)((u8*)itemOfsEndp - conf);
        itemOfsEndp[1] = (u16)(itemOfsEndp[0] + packedSize);
        ItemRestSize -= 2 + packedSize;
        ItemNumTotal++;
        *(u16*)(conf + 4) = (u16)ItemNumTotal;
        __SCSetDirtyFlag();

        return TRUE;
    }

error:
    return FALSE;
}

BOOL SCFindByteArrayItem(void* data, u32 size, SCItemID id) {
    SCItem item;
    BOOL result = FALSE;
    BOOL enabled = OSDisableInterrupts();

    if (data != NULL && FindItemByID(id, &item) && item.typeByteArray != 0 && item.dataSize == size) {
        memcpy(data, item.data, size);
        result = TRUE;
    }

    OSRestoreInterrupts(enabled);
    return result;
}

BOOL SCReplaceByteArrayItem(const void* data, u32 size, SCItemID id) {
    SCItem item;
    BOOL result = FALSE;
    BOOL enabled = OSDisableInterrupts();

    if (data != NULL) {
        if (FindItemByID(id, &item)) {
            if (item.typeByteArray != 0 && item.dataSize == size) {
                if (memcmp(item.data, data, size) != 0) {
                    memcpy(item.data, data, size);
                    __SCSetDirtyFlag();
                }

                result = TRUE;
                goto finish;
            } else {
                DeleteItemByID(id);
            }
        }
        result = CreateItemByID(id, 0x40, data, size);
    }

finish:
    OSRestoreInterrupts(enabled);
    return result;
}

BOOL SCFindIntegerItem(void* data, SCItemID id, SCType type) {
    SCItem item;
    BOOL result = FALSE;
    BOOL enabled = OSDisableInterrupts();

    ASSERTLINE(1126, data != NULL);
    ASSERTLINE(1127, type != 0);

    if (FindItemByID(id, &item) && item.typeInteger == type) {
        memcpy(data, item.data, item.dataSize);
        result = TRUE;
    }

    OSRestoreInterrupts(enabled);
    return result;
}

BOOL SCReplaceIntegerItem(const void* data, SCItemID id, SCType type) {
    SCItem item;
    BOOL result = FALSE;
    BOOL enabled = OSDisableInterrupts();

    if (FindItemByID(id, &item)) {
        if (item.typeInteger == type) {
            if (memcmp(item.data, data, item.dataSize) != 0) {
                memcpy(item.data, data, item.dataSize);
                __SCSetDirtyFlag();
            }

            result = TRUE;
            goto finish;
        } else {
            DeleteItemByID(id);
        }
    }

    result = CreateItemByID(id, type, data, 0);

finish:
    OSRestoreInterrupts(enabled);
    return result;
}

BOOL SCFindU8Item(u8* data, SCItemID id) {
    return SCFindIntegerItem(data, id, 0x60);
}

BOOL SCFindS8Item(s8* data, SCItemID id) {
    return SCFindIntegerItem(data, id, 0x60);
}

BOOL SCFindU32Item(u32* data, SCItemID id) {
    return SCFindIntegerItem(data, id, 0xA0);
}

BOOL SCReplaceU8Item(u8 data, SCItemID id) {
    return SCReplaceIntegerItem(&data, id, 0x60);
}

static void __SCFlushSyncCallback(u32 status) {
    OSWakeupThread(&Control.threadQueue);
}

void SCFlushAsync(SCFlushCallback callback) {
    SCControl* ctrl;
    BOOL enabled;
    u8 status;

    ctrl = &Control;
    enabled = OSDisableInterrupts();
    status = BgJobStatus;

    if (status == 0) {
        SetBgJobStatus(1);

        if (callback == NULL) {
            callback = __SCFlushSyncCallback;
        }

        ctrl->flushCallback = callback;
        ctrl->flushResult = 0;
        ctrl->nandNeedClose = FALSE;
        ctrl->flushSize = __SCGetConfBufSize();

        if (!__SCIsDirty()) {
            OSRestoreInterrupts(enabled);
            FinishFromFlush();
        } else {
            __SCClearDirtyFlag();
            memcpy(ConfBufForFlush, __SCGetConfBuf(), __SCGetConfBufSize());

            OSRestoreInterrupts(enabled);
            ctrl->nandStep = 0;

            if (NANDPrivateGetTypeAsync(ConfFileName, &ctrl->u.nandType, MyNandCallback, &ctrl->nandCommandBlock) != NAND_RESULT_OK) {
                ErrorFromFlush();
            }
        }
    } else {
        if (callback != NULL) {
            callback(status == 1 ? status : 2);
        }

        OSRestoreInterrupts(enabled);
    }
}

void MyNandCallback(s32 result, NANDCommandBlock* block) {
    SCControl* ctrl = &Control;

    switch (ctrl->nandStep) {
    case 0:
        if (result == NAND_RESULT_OK && ctrl->u.nandType == 1) {
            ctrl->nandStep = 1;

            if (NANDPrivateGetStatusAsync(ConfFileName, &ctrl->u.nandStatus, MyNandCallback, &ctrl->nandCommandBlock) != NAND_RESULT_OK) {
                break;
            }
        } else {
            goto _case_1_lbl;
        }
        return;
    case 1:
        if (result == NAND_RESULT_OK && ctrl->u.nandStatus.permission == NAND_PERM_RWALL) {
            goto _case_5_lbl;
        }
    _case_1_lbl:
        ctrl->nandStep = 2;

        if (NANDPrivateDeleteAsync(ConfFileName, MyNandCallback, &ctrl->nandCommandBlock) != NAND_RESULT_OK) {
            break;
        }
        return;
    case 2:
        ctrl->nandStep = 3;

        if (NANDPrivateGetTypeAsync(ConfDirName, &ctrl->u.nandType, MyNandCallback, &ctrl->nandCommandBlock) != NAND_RESULT_OK) {
            break;
        }
        return;
    case 3:
        if (result == NAND_RESULT_OK && ctrl->u.nandType == 2) {
            goto _case_4_lbl;
        }

        ctrl->nandStep = 4;

        if (NANDPrivateCreateDirAsync(ConfDirName, NAND_PERM_RWALL, 0, MyNandCallback, &ctrl->nandCommandBlock) != NAND_RESULT_OK) {
            break;
        }
        return;
    case 4:
    _case_4_lbl:
        ctrl->nandStep = 5;
        if (NANDPrivateCreateAsync(ConfFileName, NAND_PERM_RWALL, 0, MyNandCallback, &ctrl->nandCommandBlock) != NAND_RESULT_OK) {
            break;
        }
        return;
    case 5:
    _case_5_lbl:
        ctrl->nandStep = 6;
        if (NANDPrivateOpenAsync(ConfFileName, &ctrl->nandFileInfo, NAND_ACCESS_WRITE, MyNandCallback, &ctrl->nandCommandBlock) != NAND_RESULT_OK) {
            break;
        }
        return;
    case 6:
        if (result != NAND_RESULT_OK) {
            break;
        }

        ctrl->nandNeedClose = TRUE;
        ctrl->nandStep = 7;

        if (NANDWriteAsync(&ctrl->nandFileInfo, ConfBufForFlush, ctrl->flushSize, MyNandCallback, &ctrl->nandCommandBlock) != NAND_RESULT_OK) {
            break;
        }
        return;
    case 7:
        if (result != ctrl->flushSize) {
            break;
        }

        ctrl->nandNeedClose = FALSE;
        ctrl->nandStep = 8;

        if (NANDCloseAsync(&ctrl->nandFileInfo, MyNandCallback, &ctrl->nandCommandBlock) != NAND_RESULT_OK) {
            break;
        }
        return;
    case 8:
        if (result != NAND_RESULT_OK) {
            break;
        }
    case 9:
        FinishFromFlush();
    default:
        return;
    }

    ErrorFromFlush();
}

static void FinishFromFlush(void) {
    SCControl* ctrl;
    SCFlushCallback callback;

    ctrl = &Control;
    if (ctrl->flushResult != 0) {
        __SCSetDirtyFlag();
    }

    callback = ctrl->flushCallback;
    if (callback) {
        ctrl->flushCallback = NULL;
        callback(ctrl->flushResult);

        if (ctrl->threadQueue.head != NULL) {
            OSWakeupThread(&ctrl->threadQueue);
        }
    }

    SetBgJobStatus(ctrl->flushResult);
}

static void ErrorFromFlush(void) {
    SCControl* ctrl;

    ctrl = &Control;
    ctrl->flushResult = 2;

    if (ctrl->nandNeedClose) {
        ctrl->nandStep = 9;

        if (NANDCloseAsync(&ctrl->nandFileInfo, MyNandCallback, &ctrl->nandCommandBlock) == NAND_RESULT_OK) {
            return;
        }
    }

    FinishFromFlush();
}

static BOOL __SCIsDirty(void) {
    return DirtyFlag ? TRUE : FALSE;
}

static void __SCSetDirtyFlag(void) {
    DirtyFlag = TRUE;
}

static void __SCClearDirtyFlag(void) {
    DirtyFlag = FALSE;
}

static u8* __SCGetConfBuf(void) {
    return ConfBuf;
}

static u32 __SCGetConfBufSize(void) {
    return sizeof(ConfBuf);
}
