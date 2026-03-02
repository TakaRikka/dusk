#ifndef _REVOLUTION_CARD_H_
#define _REVOLUTION_CARD_H_

#include <revolution/os.h>
#include <revolution/dsp.h>
#include <revolution/dvd.h>

#ifdef __cplusplus
extern "C" {
#endif

#define CARD_FILENAME_MAX 32
#define CARD_MAX_FILE 127
#define CARD_ICON_MAX 8

typedef void (*CARDCallback)(s32 chan, s32 result);

typedef struct CARDFileInfo {
    s32 chan;
    s32 fileNo;
    s32 offset;
    s32 length;
    u16 iBlock;
} CARDFileInfo;

typedef struct CARDDir {
    u8 gameName[4];
    u8 company[2];
    u8 _padding0;
    u8 bannerFormat;
    u8 fileName[CARD_FILENAME_MAX];
    u32 time;     // seconds since 01/01/2000 midnight
    u32 iconAddr; // 0xffffffff if not used
    u16 iconFormat;
    u16 iconSpeed;
    u8 permission;
    u8 copyTimes;
    u16 startBlock;
    u16 length;
    u8 _padding1[2];
    u32 commentAddr; // 0xffffffff if not used
} CARDDir;

typedef struct CARDControl {
    /* 0x000 */ BOOL attached;
    /* 0x004 */ s32 result;
    /* 0x008 */ u16 size;
    /* 0x00A */ u16 pageSize;
    /* 0x00C */ s32 sectorSize;
    /* 0x010 */ u16 cBlock;
    /* 0x012 */ u16 vendorID;
    /* 0x014 */ s32 latency;
    /* 0x018 */ u8 id[12];
    /* 0x024 */ int mountStep;
    /* 0x028 */ int formatStep;
    /* 0x028 */ u32 scramble;
    /* 0x02C */ DSPTaskInfo task;
    /* 0x080 */ void* workArea;
    /* 0x084 */ CARDDir *currentDir;
    /* 0x088 */ u16* currentFat;
    /* 0x08C */ OSThreadQueue threadQueue;
    /* 0x094 */ u8 cmd[9];
    /* 0x0A0 */ s32 cmdlen;
    /* 0x0A4 */ volatile u32 mode;
    /* 0x0A8 */ int retry;
    /* 0x0AC */ int repeat;
    /* 0x0B0 */ u32 addr;
    /* 0x0B4 */ void* buffer;
    /* 0x0B8 */ s32 xferred;
    /* 0x0BC */ u16 freeNo;
    /* 0x0BE */ u16 startBlock;
    /* 0x0C0 */ CARDFileInfo* fileInfo;
    /* 0x0C4 */ CARDCallback extCallback;
    /* 0x0C8 */ CARDCallback txCallback;
    /* 0x0CC */ CARDCallback exiCallback;
    /* 0x0D0 */ CARDCallback apiCallback;
    /* 0x0D4 */ CARDCallback xferCallback;
    /* 0x0D8 */ CARDCallback eraseCallback;
    /* 0x0DC */ CARDCallback unlockCallback;
    /* 0x0E0 */ OSAlarm alarm;
    /* 0x108 */ u32 cid;
    /* 0x10C */ const DVDDiskID* diskID;
} CARDControl;

typedef struct CARDDecParam {
    /* 0x00 */ u8* inputAddr;
    /* 0x04 */ u32 inputLength;
    /* 0x08 */ u32 aramAddr;
    /* 0x0C */ u8* outputAddr;
} CARDDecParam;

typedef struct CARDID {
    /* 0x000 */ u8 serial[32];
    /* 0x020 */ u16 deviceID;
    /* 0x022 */ u16 size;
    /* 0x024 */ u16 encode;
    /* 0x026 */ u8 padding[470];
    /* 0x1FC */ u16 checkSum;
    /* 0x1FE */ u16 checkSumInv;
} CARDID;

typedef struct CARDDirCheck {
    /* 0x00 */ u8 padding0[56];
    /* 0x38 */ u16 padding1;
    /* 0x3A */ s16 checkCode;
    /* 0x3C */ u16 checkSum;
    /* 0x3E */ u16 checkSumInv;
} CARDDirCheck;

typedef struct CARDStat {
    /* 0x00 */ char fileName[CARD_FILENAME_MAX];
    /* 0x20 */ u32 length;
    /* 0x24 */ u32 time;
    /* 0x28 */ u8 gameName[4];
    /* 0x2C */ u8 company[2];
    /* 0x2E */ u8 bannerFormat;
    /* 0x30 */ u32 iconAddr;
    /* 0x34 */ u16 iconFormat;
    /* 0x36 */ u16 iconSpeed;
    /* 0x38 */ u32 commentAddr;
    /* 0x3C */ u32 offsetBanner;
    /* 0x40 */ u32 offsetBannerTlut;
    /* 0x44 */ u32 offsetIcon[CARD_ICON_MAX];
    /* 0x64 */ u32 offsetIconTlut;
    /* 0x68 */ u32 offsetData;
} CARDStat;

#define CARD_ATTR_PUBLIC  0x04u
#define CARD_ATTR_NO_COPY 0x08u
#define CARD_ATTR_NO_MOVE 0x10u
#define CARD_ATTR_GLOBAL  0x20u
#define CARD_ATTR_COMPANY 0x40u

#define CARD_FAT_AVAIL 0x0000u
#define CARD_FAT_CHECKSUM 0x0000u
#define CARD_FAT_CHECKSUMINV 0x0001u
#define CARD_FAT_CHECKCODE 0x0002u
#define CARD_FAT_FREEBLOCKS 0x0003u
#define CARD_FAT_LASTSLOT 0x0004u

#define CARD_WORKAREA_SIZE (5 * 8 * 1024)

#define CARD_SEG_SIZE 0x200u
#define CARD_PAGE_SIZE 0x80u
#define CARD_MAX_SIZE 0x01000000U

#define CARD_NUM_SYSTEM_BLOCK 5
#define CARD_SYSTEM_BLOCK_SIZE (8 * 1024u)

#define CARD_MAX_MOUNT_STEP (CARD_NUM_SYSTEM_BLOCK + 2)

#define CARD_STAT_SPEED_END 0
#define CARD_STAT_SPEED_FAST 1
#define CARD_STAT_SPEED_MIDDLE 2
#define CARD_STAT_SPEED_SLOW 3
#define CARD_STAT_SPEED_MASK 3

#define CARD_STAT_ANIM_LOOP   0
#define CARD_STAT_ANIM_BOUNCE 4
#define CARD_STAT_ANIM_MASK 0x4

#define CARD_RESULT_UNLOCKED       1
#define CARD_RESULT_READY          0
#define CARD_RESULT_BUSY          -1
#define CARD_RESULT_WRONGDEVICE   -2
#define CARD_RESULT_NOCARD        -3
#define CARD_RESULT_NOFILE        -4
#define CARD_RESULT_IOERROR       -5
#define CARD_RESULT_BROKEN        -6
#define CARD_RESULT_EXIST         -7
#define CARD_RESULT_NOENT         -8
#define CARD_RESULT_INSSPACE      -9
#define CARD_RESULT_NOPERM       -10
#define CARD_RESULT_LIMIT        -11
#define CARD_RESULT_NAMETOOLONG  -12
#define CARD_RESULT_ENCODING     -13
#define CARD_RESULT_CANCELED     -14
#define CARD_RESULT_FATAL_ERROR -128

#define CARDIsValidBlockNo(card, blockNo) ((blockNo) >= CARD_NUM_SYSTEM_BLOCK && (blockNo) < (card)->cBlock)

#define CARD_READ_SIZE 512
#define CARD_COMMENT_SIZE 64

#define CARD_ICON_WIDTH 32
#define CARD_ICON_HEIGHT 32

#define CARD_BANNER_WIDTH 96
#define CARD_BANNER_HEIGHT 32

#define CARD_STAT_ICON_NONE 0
#define CARD_STAT_ICON_C8 1
#define CARD_STAT_ICON_RGB5A3 2
#define CARD_STAT_ICON_MASK 3

#define CARD_STAT_BANNER_NONE 0
#define CARD_STAT_BANNER_C8 1
#define CARD_STAT_BANNER_RGB5A3 2
#define CARD_STAT_BANNER_MASK 3

#define CARD_ENCODE_ANSI 0
#define CARD_ENCODE_SJIS 1

#define CARDGetDirCheck(dir) ((CARDDirCheck*)&(dir)[CARD_MAX_FILE])
#define CARDGetBannerFormat(stat) (((stat)->bannerFormat) & CARD_STAT_BANNER_MASK)
#define CARDGetIconAnim(stat) (((stat)->bannerFormat) & CARD_STAT_ANIM_MASK)
#define CARDGetIconFormat(stat, n) (((stat)->iconFormat >> (2 * (n))) & CARD_STAT_ICON_MASK)
#define CARDGetIconSpeed(stat, n) (((stat)->iconSpeed >> (2 * (n))) & CARD_STAT_SPEED_MASK)
#define CARDSetBannerFormat(stat, f)                                                               \
    ((stat)->bannerFormat = (u8)(((stat)->bannerFormat & ~CARD_STAT_BANNER_MASK) | (f)))
#define CARDSetIconAnim(stat, f)                                                                   \
    ((stat)->bannerFormat = (u8)(((stat)->bannerFormat & ~CARD_STAT_ANIM_MASK) | (f)))
#define CARDSetIconFormat(stat, n, f)                                                              \
    ((stat)->iconFormat =                                                                            \
        (u16)(((stat)->iconFormat & ~(CARD_STAT_ICON_MASK << (2 * (n)))) | ((f) << (2 * (n)))))
#define CARDSetIconSpeed(stat, n, f)                                                               \
    ((stat)->iconSpeed =                                                                             \
        (u16)(((stat)->iconSpeed & ~(CARD_STAT_SPEED_MASK << (2 * (n)))) | ((f) << (2 * (n)))))
#define CARDSetIconAddress(stat, addr) ((stat)->iconAddr = (u32)(addr))
#define CARDSetCommentAddress(stat, addr) ((stat)->commentAddr = (u32)(addr))
#define CARDGetFileNo(fileInfo) ((fileInfo)->fileNo)

extern u32 __CARDFreq;

#if DEBUG
#define CARDFreq __CARDFreq
#else
#define CARDFreq EXI_FREQ_16M
#endif

void CARDInit(void);
s32 CARDGetResultCode(s32 chan);
s32 CARDCheckAsync(s32 chan, CARDCallback callback);
s32 CARDFreeBlocks(s32 chan, s32* byteNotUsed, s32* filesNotUsed);
s32 CARDRenameAsync(s32 chan, const char* oldName, const char* newName, CARDCallback callback);

// CARDBios
void CARDInit(void);
s32 CARDGetResultCode(s32 chan);
s32 CARDFreeBlocks(s32 chan, s32* byteNotUsed, s32* filesNotUsed);
s32 CARDGetEncoding(s32 chan, u16* encode);
s32 CARDGetMemSize(s32 chan, u16* size);
s32 CARDGetSectorSize(s32 chan, u32* size);
const DVDDiskID* CARDGetDiskID(s32 chan);
s32 CARDSetDiskID(s32 chan, const DVDDiskID* diskID);
BOOL CARDSetFastMode(BOOL enable);
BOOL CARDGetFastMode(void);
s32 CARDGetCurrentMode(s32 chan, u32* mode);

// CARDCheck
s32 CARDCheckExAsync(s32 chan, s32* xferBytes, CARDCallback callback);
s32 CARDCheckAsync(s32 chan, CARDCallback callback);
s32 CARDCheckEx(s32 chan, s32* xferBytes);
s32 CARDCheck(s32 chan);

// CARDCreate
s32 CARDCreateAsync(s32 chan, const char* fileName, u32 size, CARDFileInfo* fileInfo, CARDCallback callback);
s32 CARDCreate(s32 chan, const char* fileName, u32 size, CARDFileInfo* fileInfo);

// CARDDelete
s32 CARDFastDeleteAsync(s32 chan, s32 fileNo, CARDCallback callback);
s32 CARDFastDelete(s32 chan, s32 fileNo);
s32 CARDDeleteAsync(s32 chan, const char* fileName, CARDCallback callback);
s32 CARDDelete(s32 chan, const char* fileName);

// CARDErase
s32 CARDEraseAsync(CARDFileInfo* fileInfo, s32 length, s32 offset, CARDCallback callback);
s32 CARDErase(CARDFileInfo* fileInfo, s32 length, s32 offset);

// CARDFormat
s32 CARDFormat(s32 chan);

// CARDMount
int CARDProbe(s32 chan);
s32 CARDProbeEx(s32 chan, s32* memSize, s32* sectorSize);
s32 CARDMountAsync(s32 chan, void* workArea, CARDCallback detachCallback, CARDCallback attachCallback);
s32 CARDMount(s32 chan, void* workArea, CARDCallback detachCallback);
s32 CARDUnmount(s32 chan);

// CARDNet
u16 CARDSetVendorID(u16 vendorID);
u16 CARDGetVendorID();
s32 CARDGetSerialNo(s32 chan, u64* serialNo);
s32 CARDGetUniqueCode(s32 chan, u64* uniqueCode);
s32 CARDGetAttributes(s32 chan, s32 fileNo, u8* attr);
s32 CARDSetAttributesAsync(s32 chan, s32 fileNo, u8 attr, CARDCallback callback);
s32 CARDSetAttributes(s32 chan, s32 fileNo, u8 attr);

// CARDOpen
s32 CARDFastOpen(s32 chan, s32 fileNo, CARDFileInfo* fileInfo);
s32 CARDOpen(s32 chan, const char* fileName, CARDFileInfo* fileInfo);
s32 CARDClose(CARDFileInfo* fileInfo);

// CARDProgram
s32 CARDProgramAsync(CARDFileInfo* fileInfo, void* buf, s32 length, s32 offset, CARDCallback callback);
s32 CARDProgram(CARDFileInfo* fileInfo, void* buf, s32 length, s32 offset);

// CARDRdwr
s32 CARDGetXferredBytes(s32 chan);

// CARDRead
s32 CARDReadAsync(CARDFileInfo* fileInfo, void* buf, s32 length, s32 offset, CARDCallback callback);
s32 CARDRead(CARDFileInfo*  fileInfo, void*  buf, s32 length, s32 offset);
s32 CARDCancel(CARDFileInfo* fileInfo);

// CARDRename
s32 CARDRename(s32 chan, const char* oldName, const char* newName);

// CARDStat
s32 CARDGetStatus(s32 chan, s32 fileNo, CARDStat* stat);
s32 CARDSetStatusAsync(s32 chan, s32 fileNo, CARDStat* stat, CARDCallback callback);
s32 CARDSetStatus(s32 chan, s32 fileNo, CARDStat*  stat);

// CARDWrite
s32 CARDWriteAsync(CARDFileInfo*  fileInfo, void* buf, s32 length, s32 offset, CARDCallback callback);
s32 CARDWrite(CARDFileInfo* fileInfo, void* buf, s32 length, s32 offset);

#ifdef __cplusplus
}
#endif

#endif
