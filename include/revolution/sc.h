#ifndef _REVOLUTION_SC_H_
#define _REVOLUTION_SC_H_

#include <revolution/os.h>
#include <revolution/nand.h>
#include <revolution/wpad/bte.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef u8 SCType;

typedef enum {
    SC_STATUS_OK,
    SC_STATUS_BUSY,
    SC_STATUS_FATAL,
    SC_STATUS_PARSE,
} SCStatus;

typedef struct {
    u8 mode;
    u8 led;
} SCIdleModeInfo;

typedef struct {
    union {
        u8 type_u8;
        s8 type_s8;
        u16 type_u16;
        s16 type_s16;
        u32 type_u32;
        s32 type_s32;
        u64 type_u64;
        s64 type_s64;
        u8 longPrecision64[sizeof(u64)];
    } integer;

    SCType typeInteger;
    SCType typeByteArray;
    u32 nameLen;
    u32 dataSize;
    char* name;
    u8* data;
    u32 packedSize;
} SCItem;

typedef void (*SCReloadConfFileCallback)(s32 result);
typedef void (*SCFlushCallback)(u32 result);

typedef enum {
    SC_ITEM_ID_IPL_COUNTER_BIAS,
    SC_ITEM_ID_IPL_ASPECT_RATIO,
    SC_ITEM_ID_IPL_AUTORUN_MODE,
    SC_ITEM_ID_IPL_CONFIG_DONE,
    SC_ITEM_ID_IPL_CONFIG_DONE2,
    SC_ITEM_ID_IPL_DISPLAY_OFFSET_H,
    SC_ITEM_ID_IPL_EURGB60_MODE,
    SC_ITEM_ID_IPL_EULA,
    SC_ITEM_ID_IPL_FREE_CHANNEL_APP_COUNT,
    SC_ITEM_ID_IPL_IDLE_MODE,
    SC_ITEM_ID_IPL_INSTALLED_CHANNEL_APP_COUNT,
    SC_ITEM_ID_IPL_LANGUAGE,
    SC_ITEM_ID_IPL_OWNER_NICKNAME,
    SC_ITEM_ID_IPL_PARENTAL_CONTROL,
    SC_ITEM_ID_IPL_PROGRESSIVE_MODE,
    SC_ITEM_ID_IPL_SCREEN_SAVER_MODE,
    SC_ITEM_ID_IPL_SIMPLE_ADDRESS,
    SC_ITEM_ID_IPL_SOUND_MODE,
    SC_ITEM_ID_IPL_UPDATE_TYPE,
    SC_ITEM_ID_NET_CONFIG,
    SC_ITEM_ID_NET_CONTENT_RESTRICTIONS,
    SC_ITEM_ID_NET_PROFILE,
    SC_ITEM_ID_NET_WC_RESTRICTION,
    SC_ITEM_ID_NET_WC_FLAGS,
    SC_ITEM_ID_DEV_BOOT_MODE,
    SC_ITEM_ID_DEV_VIDEO_MODE,
    SC_ITEM_ID_DEV_COUNTRY_CODE,
    SC_ITEM_ID_DEV_DRIVESAVING_MODE,
    SC_ITEM_ID_BT_DEVICE_INFO,
    SC_ITEM_ID_BT_CMPDEV_INFO,
    SC_ITEM_ID_BT_DPD_SENSIBILITY,
    SC_ITEM_ID_BT_SPEAKER_VOLUME,
    SC_ITEM_ID_BT_MOTOR_MODE,
    SC_ITEM_ID_BT_SENSOR_BAR_POSITION,
    SC_ITEM_ID_DVD_CONFIG,
    SC_ITEM_ID_WWW_RESTRICTION,
    SC_ITEM_ID_MOTION_PLUS_MOVIE,
    SC_ITEM_ID_TEMPORARY_TITLE_ID,

    SC_ITEM_ID_MAX_PLUS1
} SCItemID;

typedef struct {
    OSThreadQueue threadQueue;
    NANDFileInfo nandFileInfo;
    NANDCommandBlock nandCommandBlock;

    union {
        u8 nandType;
        NANDStatus nandStatus;
    } u;

    u8 nandStep;
    u8 nandNeedClose;
    u8 reloadFileCount;
    SCReloadConfFileCallback reloadCallback;
    s32 reloadResult;
    const char* reloadFileName[2];
    u8* reloadBufp[2];
    u32 reloadSizeExpected[2];
    u32 reloadedSize[2];
    SCFlushCallback flushCallback;
    u32 flushResult;
    u32 flushSize;
} SCControl;

#define SC_MAX_DEV_ENTRY_FOR_STD 10
#define SC_MAX_DEV_ENTRY_FOR_SMP 6
#define SC_MAX_DEV_ENTRY (SC_MAX_DEV_ENTRY_FOR_STD + SC_MAX_DEV_ENTRY_FOR_SMP)

typedef struct {
    BD_ADDR bd_addr;
    u8 bd_name[64];
    u8 link_key[16];
} SCBtCmpDevInfoSingle;

typedef struct {
    BD_ADDR bd_addr;
    u8 bd_name[64];
} SCBtDeviceInfoSingle;

typedef struct {
    u8 num;
    SCBtCmpDevInfoSingle info[6];
} SCBtCmpDevInfoArray;

typedef struct SCDevInfo {
    char devName[20];  // at 0x0
    char at_0x14[1];
    char UNK_0x15[0xB];
    LINK_KEY linkKey;  // at 0x20
    char UNK_0x30[0x10];
} SCDevInfo;

typedef struct SCBtDeviceInfo {
    BD_ADDR addr;    // at 0x0
    SCDevInfo info;  // at 0x6
} SCBtDeviceInfo;

typedef struct SCBtDeviceInfoArray {
    /* 0x000 */ u8 num;

    union {
        struct {
#if SDK_AUG2010
            /* 0x001 */ SCBtDeviceInfo regist[SC_MAX_DEV_ENTRY_FOR_STD];
            /* 0x2BD */ SCBtDeviceInfo active[SC_MAX_DEV_ENTRY_FOR_SMP];
#else
            /* 0x001 */ SCBtDeviceInfo regist[SC_MAX_DEV_ENTRY];
#endif
        };
    };
#if SDK_AUG2010
    SCBtDeviceInfoSingle info[16];
#endif
} SCBtDeviceInfoArray;

#define SC_LANG_JAPANESE 0u
#define SC_LANG_ENGLISH 1u
#define SC_LANG_GERMAN 2u
#define SC_LANG_FRENCH 3u
#define SC_LANG_SPANISH 4u
#define SC_LANG_ITALIAN 5u
#define SC_LANG_DUTCH 6u
#define SC_LANG_SIMP_CHINESE 7u
#define SC_LANG_TRAD_CHINESE 8u
#define SC_LANG_KOREAN 9u

// scapi
u8 SCGetAspectRatio(void);
s8 SCGetDisplayOffsetH(void);
u8 SCGetEuRgb60Mode(void);
BOOL SCGetIdleMode(SCIdleModeInfo* data);
u8 SCGetLanguage(void);
u8 SCGetProgressiveMode(void);
u8 SCGetScreenSaverMode(void);
u8 SCGetSoundMode(void);
u32 SCGetCounterBias(void);
u8 SCGetWpadSensorBarPosition(void);
u8 SCGetWpadMotorMode(void);
u8 SCGetWpadSpeakerVolume(void);
u8 SCGetBtDpdSensibility(void);
BOOL SCSetWpadSpeakerVolume(u8 volume);
BOOL SCSetWpadMotorMode(u8 mode);

// scapi_prdinfo
BOOL SCGetProductAreaString(char* buf, u32 bufSize);
s8 SCGetProductArea(void);
s8 SCGetProductGameRegion(void);

// scsystem
void SCInit(void);
BOOL SCFindByteArrayItem(void* data, u32 size, SCItemID id);
BOOL SCReplaceByteArrayItem(const void* data, u32 size, SCItemID id);
BOOL SCFindIntegerItem(void* data, SCItemID id, SCType type);
BOOL SCReplaceIntegerItem(const void* data, SCItemID id, SCType type);
BOOL SCFindU8Item(u8* data, SCItemID id);
BOOL SCFindS8Item(s8* data, SCItemID id);
BOOL SCFindU32Item(u32* data, SCItemID id);
BOOL SCReplaceU8Item(u8 data, SCItemID id);
void SCFlushAsync(SCFlushCallback callback);
BOOL SCGetBtDeviceInfoArray(SCBtDeviceInfoArray*);
BOOL SCSetBtDeviceInfoArray(const SCBtDeviceInfoArray*);
u32 SCCheckStatus(void);
s32 SCReloadConfFileAsync(u8* bufp, u32 bufSize, SCReloadConfFileCallback callback);

BOOL SCFlush(void);

#ifdef __cplusplus
}
#endif

#endif
