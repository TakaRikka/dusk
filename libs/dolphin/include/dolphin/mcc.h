#ifndef _DOLPHIN_MCC_H_
#define _DOLPHIN_MCC_H_

#include <dolphin/types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    MCC_CHANNEL_SYSTEM = 0,
    MCC_CHANNEL_1 = 1,
    MCC_CHANNEL_2 = 2,
    MCC_CHANNEL_3 = 3,
    MCC_CHANNEL_4 = 4,
    MCC_CHANNEL_5 = 5,
    MCC_CHANNEL_6 = 6,
    MCC_CHANNEL_7 = 7,
    MCC_CHANNEL_8 = 8,
    MCC_CHANNEL_9 = 9,
    MCC_CHANNEL_10 = 10,
    MCC_CHANNEL_11 = 11,
    MCC_CHANNEL_12 = 12,
    MCC_CHANNEL_13 = 13,
    MCC_CHANNEL_14 = 14,
    MCC_CHANNEL_15 = 15,
} MCC_CHANNEL;

typedef struct {
    /* 0x00 */ u8 firstBlock;
    /* 0x01 */ u8 blockLength;
    /* 0x02 */ u8 connect;
    /* 0x03 */ u8 isLocked;
} MCC_Info;

typedef enum {
    MCC_EXI_0 = 0,
    MCC_EXI_1 = 1,
    MCC_EXI_2 = 2,
} MCC_EXI;

typedef enum {
    MCC_SYSEVENT_UNKNOWN = 0,
    MCC_SYSEVENT_REBOOT = 1,
    MCC_SYSEVENT_INITIALIZED = 2,
    MCC_SYSEVENT_PING = 3,
    MCC_SYSEVENT_PING_RESULT = 4,
    MCC_SYSEVENT_FLUSH = 5,
} MCC_SYSEVENT;

typedef void (*MCC_CBEvent)(MCC_CHANNEL channel, u32, u32);
typedef void (*MCC_CBSysEvent)(MCC_SYSEVENT sysEvent);
typedef int (*MCC_CBEnumDevices)(s32);

typedef struct {
    /* 0x00 */ u32 length;
    /* 0x04 */ u16 rsvd;
    /* 0x06 */ u16 protocol;
} MCC_Hdr;

typedef struct {
    /* 0x00 */ u32 code;
    /* 0x04 */ u32 number;
} MCC_HdrFio;

typedef struct {
    /* 0x00 */ MCC_Info info;
    /* 0x04 */ MCC_CBEvent callbackEvent;
    /* 0x08 */ u32 eventMask;
    /* 0x0C */ int isStreamDone;
    /* 0x10 */ int isStreamConnection;
    /* 0x14 */ int isStreamOpened;
} MCC_ChannelInfo;

typedef enum {
    MCC_MODE_MIN = 0,
    MCC_MODE_MAX = 1,
    MCC_MODE_ALL = 2,
} MCC_MODE;

typedef enum {
    MCC_CONNECT_DISCONNECT = 0,
    MCC_CONNECT_HOST_OPEN = 1,
    MCC_CONNECT_TARGET_OPEN = 2,
    MCC_CONNECT_CONNECTED = 3,
} MCC_CONNECT;

typedef enum {
    MCC_EVENT_CONNECT = 0x1,
    MCC_EVENT_DISCONNECT = 0x2,
    MCC_EVENT_LOCK = 0x4,
    MCC_EVENT_UNLOCK = 0x8,
    MCC_EVENT_READ = 0x10,
    MCC_EVENT_WRITE = 0x20,
    MCC_EVENT_READ_DONE_INSIDE = 0x40,
    MCC_EVENT_WRITE_DONE_INSIDE = 0x80,
    MCC_EVENT_UNK_0x100 = 0x100,
} MCC_EVENT;

typedef enum {
    MCC_SYNC = 0,
    MCC_ASYNC = 1,
} MCC_SYNC_STATE;

typedef enum {
    FIO_ASYNC_STATE_IDOL = 0,
    FIO_ASYNC_STATE_BUSY = 1,
    FIO_ASYNC_STATE_DONE = 2,
} FIO_ASYNC_STATE;

typedef struct {
    /* 0x00 */ u16 year;
    /* 0x02 */ u8 month;
    /* 0x03 */ u8 day;
} FIO_Date;

typedef struct {
    /* 0x00 */ u8 hour;
    /* 0x01 */ u8 minute;
    /* 0x02 */ u8 second;
    /* 0x03 */ u8 reserved;
} FIO_Daytime;

typedef struct {
    /* 0x00 */ FIO_Date date;
    /* 0x04 */ FIO_Daytime time;
} FIO_Timestamp;

typedef struct {
    /* 0x00 */ u32 fileAttributes;
    /* 0x04 */ FIO_Timestamp creationTime;
    /* 0x0C */ FIO_Timestamp lastAccessTime;
    /* 0x14 */ FIO_Timestamp lastWriteTime;
    /* 0x1C */ u32 fileSizeHigh;
    /* 0x20 */ u32 fileSizeLow;
} FIO_Stat;

typedef struct {
    /* 0x00 */ FIO_Stat stat;
    /* 0x24 */ char filename[256];
} FIO_Finddata;

#define FIO_ERROR_NONE               0x00
#define FIO_ERROR_WRONG_CODE         0x81
#define FIO_ERROR_WRONG_SEQUENCE     0x82
#define FIO_ERROR_PACKET_WRITE       0x83
#define FIO_ERROR_PACKET_READ        0x84
#define FIO_ERROR_MCC                0x87
#define FIO_ERROR_TIMEOUT            0x88
#define FIO_ERROR_MSG_TOO_LONG       0x89
#define FIO_ERROR_UNK_0x91           0x91
#define FIO_ERROR_ASYNC_SIZE_TOO_BIG 0xA0
#define FIO_ERROR_ASYNC_BUSY         0xA1
#define FIO_ERROR_INVALID_PARAMETERS 0xB0

// FIO
int FIOInit(MCC_EXI exiChannel, MCC_CHANNEL chID, u8 blockSize);
void FIOExit(void);
int FIOQuery(void);
u8 FIOGetLastError();
int FIOFopen(const char* filename, u32 mode);
int FIOFclose(int handle);
u32 FIOFread(int handle, void* data, u32 size);
u32 FIOFwrite(int handle, void*  data, u32 size);
u32 FIOFseek(int handle, s32 offset, u32 mode);
int FIOFprintf(int handle, const char* format, ...);
int FIOFflush(int handle);
int FIOFstat(int handle, FIO_Stat* stat);
int FIOFerror(int handle);
int FIOFindFirst(const char* filename, FIO_Finddata* finddata);
int FIOFindNext(FIO_Finddata* finddata);
u32 FIOGetAsyncBufferSize(void);
int FIOFreadAsync(int handle, void* data, u32 size);
int FIOFwriteAsync(int handle, void* data, u32 size);
int FIOCheckAsyncDone(u32* result);

// MCC
BOOL MCCStreamOpen(MCC_CHANNEL chID, u8 blockSize);
int MCCStreamClose(MCC_CHANNEL chID);
int MCCStreamWrite(MCC_CHANNEL chID, void* data, u32 dataBlockSize);
u32 MCCStreamRead(MCC_CHANNEL chID, void* data);
int MCCInit(MCC_EXI exiChannel, u8 timeout, MCC_CBSysEvent callbackSysEvent);
void MCCExit(void);
int MCCPing(void);
int MCCEnumDevices(MCC_CBEnumDevices callbackEnumDevices);
u8 MCCGetFreeBlocks(MCC_MODE mode);
u8 MCCGetLastError(void);
int MCCGetChannelInfo(MCC_CHANNEL chID, MCC_Info* info);
int MCCGetConnectionStatus(MCC_CHANNEL chID, MCC_CONNECT* connect);
int MCCNotify(MCC_CHANNEL chID, u32 notify);
u32 MCCSetChannelEventMask(MCC_CHANNEL chID, u32 event);
int MCCOpen(MCC_CHANNEL chID, u8 blockSize, MCC_CBEvent callbackEvent);
int MCCClose(MCC_CHANNEL chID);
int MCCLock(MCC_CHANNEL chID);
int MCCUnlock(MCC_CHANNEL chID);
int MCCRead(MCC_CHANNEL chID, u32 offset, void* data, s32 size, MCC_SYNC_STATE async);
int MCCWrite(MCC_CHANNEL chID, u32 offset, void* data, s32 size, MCC_SYNC_STATE async);
int MCCCheckAsyncDone();

// TTY
int TTYInit(MCC_EXI exiChannel, MCC_CHANNEL chID);
void TTYExit(void);
int TTYQuery(void);
int TTYPrintf(const char* format, ...);
int TTYFlush(void);

#ifdef __cplusplus
}
#endif

#endif // _DOLPHIN_MCC_H_
