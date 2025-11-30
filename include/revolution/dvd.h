#ifndef _REVOLUTION_DVD_H_
#define _REVOLUTION_DVD_H_

#include <revolution/types.h>
#include <revolution/esp.h>

#ifdef __cplusplus
extern "C" {
#endif

#define DVD_ASSERTMSGLINE(line, cond, msg) \
    if (!(cond)) \
        OSPanic(__FILE__, line, msg)

#define DVD_ASSERTMSG1LINE(line, cond, msg, arg1) \
    if (!(cond)) \
        OSPanic(__FILE__, line, msg, arg1)

#define DVD_ASSERTMSG2LINE(line, cond, msg, arg1, arg2) \
    if (!(cond)) \
        OSPanic(__FILE__, line, msg, arg1, arg2)

#define DVD_RESULT_GOOD        0
#define DVD_RESULT_FATAL_ERROR -1
#define DVD_RESULT_IGNORED     -2
#define DVD_RESULT_CANCELED    -6

#define DVD_STATE_FATAL_ERROR   -1
#define DVD_STATE_END            0
#define DVD_STATE_BUSY           1
#define DVD_STATE_WAITING        2
#define DVD_STATE_COVER_CLOSED   3
#define DVD_STATE_NO_DISK        4
#define DVD_STATE_COVER_OPEN     5
#define DVD_STATE_WRONG_DISK     6
#define DVD_STATE_MOTOR_STOPPED  7
#define DVD_STATE_PAUSING        8
#define DVD_STATE_IGNORED        9
#define DVD_STATE_CANCELED       10
#define DVD_STATE_RETRY          11

#define DVD_MIN_TRANSFER_SIZE    32

// could be bitfields
#define DVD_INTTYPE_TC 1
#define DVD_INTTYPE_DE 2
// unk type 3
#define DVD_INTTYPE_CVR 4

// DVD Commands
#define DVD_COMMAND_NONE 0
#define DVD_COMMAND_READ 1
#define DVD_COMMAND_SEEK 2
#define DVD_COMMAND_CHANGE_DISK 3
#define DVD_COMMAND_BSREAD 4
#define DVD_COMMAND_READID 5
#define DVD_COMMAND_INITSTREAM 6
#define DVD_COMMAND_CANCELSTREAM 7
#define DVD_COMMAND_STOP_STREAM_AT_END 8
#define DVD_COMMAND_REQUEST_AUDIO_ERROR 9
#define DVD_COMMAND_REQUEST_PLAY_ADDR 10
#define DVD_COMMAND_REQUEST_START_ADDR 11
#define DVD_COMMAND_REQUEST_LENGTH 12
#define DVD_COMMAND_AUDIO_BUFFER_CONFIG 13
#define DVD_COMMAND_INQUIRY 14
#define DVD_COMMAND_BS_CHANGE_DISK 15
#define DVD_COMMAND_UNK_16 16

typedef struct DVDDiskID {
    char gameName[4];
    char company[2];
    u8 diskNumber;
    u8 gameVersion;
    u8 streaming;
    u8 streamingBufSize;
    u8 padding[22];
} DVDDiskID;

typedef struct DVDCommandBlock DVDCommandBlock;
typedef void (*DVDCBCallback)(s32 result, DVDCommandBlock* block);
typedef void (*DVDLowCallback)(u32);

typedef void (*DVDCommandCheckerCallback)(u32);
typedef void (*DVDCommandChecker)(DVDCommandBlock*, DVDCommandCheckerCallback);

struct DVDCommandBlock {
    /* 0x00 */ DVDCommandBlock* next;
    /* 0x04 */ DVDCommandBlock* prev;
    /* 0x08 */ u32 command;
    /* 0x0C */ s32 state;
    /* 0x10 */ u32 offset;
    /* 0x14 */ u32 length;
    /* 0x18 */ void* addr;
    /* 0x1C */ u32 currTransferSize;
    /* 0x20 */ u32 transferredSize;
    /* 0x24 */ DVDDiskID* id;
    /* 0x28 */ DVDCBCallback callback;
    /* 0x2C */ void* userData;
};

typedef struct DVDFileInfo DVDFileInfo;
typedef void (*DVDCallback)(s32 result, DVDFileInfo *fileInfo);
struct DVDFileInfo {
	/* 0x00 */ DVDCommandBlock cb;
    /* 0x30 */ u32 startAddr;
    /* 0x34 */ u32 length;
    /* 0x38 */ DVDCallback callback;
};

typedef struct {
    u32 entryNum;
    u32 location;
    u32 next;
} DVDDir;

typedef struct {
    u32 entryNum;
    BOOL isDir;
    char* name;
} DVDDirEntry;

typedef struct DVDBB2 {
    /* 0x00 */ u32 bootFilePosition;
    /* 0x04 */ u32 FSTPosition;
    /* 0x08 */ u32 FSTLength;
    /* 0x0C */ u32 FSTMaxLength;
    /* 0x10 */ void* FSTAddress;
    /* 0x14 */ u32 userPosition;
    /* 0x18 */ u32 userLength;
    /* 0x1C */ u32 padding0;
} DVDBB2;

typedef struct DVDDriveInfo {
    /* 0x00 */ u16 revisionLevel;
    /* 0x02 */ u16 deviceCode;
    /* 0x04 */ u32 releaseDate;
    /* 0x08 */ u8 padding[24];
} DVDDriveInfo;

typedef struct DVDGamePartition {
    ESTicket ticket;
    u32 tmdSize;
    ESTitleMeta* tmd;
    u32 certBlobSize;
    void* certBlob;      
    u8* h3Hashes;
    u8* encryptedArea;
} DVDGamePartition;

typedef struct DVDPartitionInfo {
    DVDGamePartition* gamePartition;
    u32 type;
} DVDPartitionInfo;

typedef struct DVDGameTOC {
    u32 numGamePartitions;
    DVDPartitionInfo* partitionInfos;
} DVDGameTOC;

// DVD
void DVDInit(void);
int DVDReadAbsAsyncPrio(DVDCommandBlock* block, void* addr, s32 length, s32 offset, DVDCBCallback callback, s32 prio);
int DVDSeekAbsAsyncPrio(DVDCommandBlock* block, s32 offset, DVDCBCallback callback, s32 prio);
int DVDReadAbsAsyncForBS(DVDCommandBlock* block, void* addr, s32 length, s32 offset, DVDCBCallback callback);
int DVDReadDiskID(DVDCommandBlock* block, DVDDiskID* diskID, DVDCBCallback callback);
int DVDPrepareStreamAbsAsync(DVDCommandBlock* block, u32 length, u32 offset, DVDCBCallback callback);
int DVDCancelStreamAsync(DVDCommandBlock* block, DVDCBCallback callback);
s32 DVDCancelStream(DVDCommandBlock* block);
int DVDStopStreamAtEndAsync(DVDCommandBlock* block, DVDCBCallback callback);
s32 DVDStopStreamAtEnd(DVDCommandBlock* block);
int DVDGetStreamErrorStatusAsync(DVDCommandBlock* block, DVDCBCallback callback);
s32 DVDGetStreamErrorStatus(DVDCommandBlock* block);
int DVDGetStreamPlayAddrAsync(DVDCommandBlock* block, DVDCBCallback callback);
s32 DVDGetStreamPlayAddr(DVDCommandBlock* block);
int DVDGetStreamStartAddrAsync(DVDCommandBlock* block, DVDCBCallback callback);
s32 DVDGetStreamStartAddr(DVDCommandBlock* block);
int DVDGetStreamLengthAsync(DVDCommandBlock* block, DVDCBCallback callback);
s32 DVDGetStreamLength(DVDCommandBlock* block);
int DVDChangeDiskAsyncForBS(DVDCommandBlock* block, DVDCBCallback callback);
int DVDChangeDiskAsync(DVDCommandBlock* block, DVDDiskID* id, DVDCBCallback callback);
s32 DVDChangeDisk(DVDCommandBlock* block, DVDDiskID* id);
int DVDStopMotorAsync(DVDCommandBlock* block, DVDCBCallback callback);
s32 DVDStopMotor(DVDCommandBlock* block);
int DVDInquiryAsync(DVDCommandBlock* block, DVDDriveInfo* info, DVDCBCallback callback);
s32 DVDInquiry(DVDCommandBlock* block, DVDDriveInfo* info);
void DVDReset(void);
int DVDResetRequired(void);
s32 DVDGetCommandBlockStatus(const DVDCommandBlock* block);
s32 DVDGetDriveStatus(void);
BOOL DVDSetAutoInvalidation(BOOL autoInval);
void DVDPause(void);
void DVDResume(void);
int DVDCancelAsync(DVDCommandBlock* block, DVDCBCallback callback);
s32 DVDCancel(volatile DVDCommandBlock* block);
int DVDCancelAllAsync(DVDCBCallback callback);
s32 DVDCancelAll(void);
DVDDiskID* DVDGetCurrentDiskID(void);
BOOL DVDCheckDisk(void);

// DVD FATAL
int DVDSetAutoFatalMessaging(BOOL enable);

// DVD FS
s32 DVDConvertPathToEntrynum(const char* pathPtr);
BOOL DVDFastOpen(s32 entrynum, DVDFileInfo* fileInfo);
BOOL DVDOpen(const char* fileName, DVDFileInfo* fileInfo);
BOOL DVDClose(DVDFileInfo* fileInfo);
BOOL DVDGetCurrentDir(char* path, u32 maxlen);
BOOL DVDChangeDir(const char* dirName);
BOOL DVDReadAsyncPrio(DVDFileInfo* fileInfo, void* addr, s32 length, s32 offset,
                      DVDCallback callback, s32 prio);
s32 DVDReadPrio(DVDFileInfo* fileInfo, void* addr, s32 length, s32 offset, s32 prio);
int DVDSeekAsyncPrio(DVDFileInfo* fileInfo, s32 offset, void (* callback)(s32, DVDFileInfo *), s32 prio);
s32 DVDSeekPrio(DVDFileInfo* fileInfo, s32 offset, s32 prio);
s32 DVDGetFileInfoStatus(const DVDFileInfo* fileInfo);
BOOL DVDFastOpenDir(s32 entrynum, DVDDir* dir);
int DVDOpenDir(const char* dirName, DVDDir* dir);
int DVDReadDir(DVDDir* dir, DVDDirEntry* dirent);
int DVDCloseDir(DVDDir* dir);
void DVDRewindDir(DVDDir* dir);
void* DVDGetFSTLocation(void);
BOOL DVDPrepareStreamAsync(DVDFileInfo* fileInfo, u32 length, u32 offset, DVDCallback callback);
s32 DVDPrepareStream(DVDFileInfo* fileInfo, u32 length, u32 offset);
s32 DVDGetTransferredSize(DVDFileInfo* fileinfo);

#define DVDReadAsync(fileInfo, addr, length, offset, callback) \
    DVDReadAsyncPrio((fileInfo), (addr), (length), (offset), (callback), 2)

// DVD ID UTILS
int DVDCompareDiskID(const DVDDiskID* id1, const DVDDiskID* id2);
DVDDiskID* DVDGenerateDiskID(DVDDiskID* id, const char* game, const char* company, u8 diskNum, u8 version);

// DVD LOW
BOOL DVDLowRead(void* addr, u32 length, u32 offset, DVDLowCallback callback);
BOOL DVDLowSeek(u32 offset, DVDLowCallback callback);
BOOL DVDLowWaitCoverClose(DVDLowCallback callback);
BOOL DVDLowReadDiskID(DVDDiskID* diskID, DVDLowCallback callback);
BOOL DVDLowStopMotor(DVDLowCallback callback);
BOOL DVDLowRequestError(DVDLowCallback callback);
BOOL DVDLowInquiry(DVDDriveInfo* info, DVDLowCallback callback);
BOOL DVDLowAudioStream(u32 subcmd, u32 length, u32 offset, DVDLowCallback callback);
BOOL DVDLowRequestAudioStatus(u32 subcmd, DVDLowCallback callback);
BOOL DVDLowAudioBufferConfig(BOOL enable, u32 size, DVDLowCallback callback);
void DVDLowReset(void);
DVDLowCallback DVDLowSetResetCoverCallback(DVDLowCallback callback);
BOOL DVDLowBreak(void);
DVDLowCallback DVDLowClearCallback(void);
u32 DVDLowGetCoverStatus(void);

// DVD QUEUE
void DVDDumpWaitingQueue(void);


// unsorted revo
BOOL DVDLowUnencryptedRead(void*, u32, u32, DVDLowCallback);
BOOL DVDLowClosePartition(DVDLowCallback);
BOOL DVDLowOpenPartitionWithTmdAndTicketView(const u32, const ESTicketView* const, const u32, const ESTitleMeta* const, const u32, const u8* const, DVDLowCallback);
BOOL DVDLowOpenPartition(const u32, const ESTicket* const, const u32, const u8* const, ESTitleMeta*, DVDLowCallback);

#ifdef __cplusplus
}
#endif

#endif
