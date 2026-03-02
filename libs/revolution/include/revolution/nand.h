#ifndef _REVOLUTION_NAND_H_
#define _REVOLUTION_NAND_H_

#include <revolution/types.h>
#include <revolution/fs.h>

#ifdef __cplusplus
extern "C" {
#endif

#define NAND_RESULT_OK                0 
#define NAND_RESULT_ACCESS         (-1)
#define NAND_RESULT_ALLOC_FAILED   (-2)
#define NAND_RESULT_BUSY           (-3)
#define NAND_RESULT_CORRUPT        (-4)
#define NAND_RESULT_ECC_CRIT       (-5)
#define NAND_RESULT_EXISTS         (-6)
#define NAND_RESULT_INVALID        (-8)
#define NAND_RESULT_MAXBLOCKS      (-9)
#define NAND_RESULT_MAXFD          (-10)
#define NAND_RESULT_MAXFILES       (-11)
#define NAND_RESULT_NOEXISTS       (-12)
#define NAND_RESULT_NOTEMPTY       (-13)
#define NAND_RESULT_OPENFD         (-14)
#define NAND_RESULT_AUTHENTICATION (-15)
#define NAND_RESULT_MAXDEPTH       (-16)
#define NAND_RESULT_UNKNOWN        (-64)
#define NAND_RESULT_FATAL_ERROR   (-128)

#define NAND_MAX_PATH 64

#define NAND_CHECK_HOME_INSSPACE 0x00000001
#define NAND_CHECK_HOME_INSINODE 0x00000002
#define NAND_CHECK_SYS_INSSPACE 0x00000004
#define NAND_CHECK_SYS_INSINODE 0x00000008

typedef enum {
    NAND_ACCESS_NONE,
    NAND_ACCESS_READ,
    NAND_ACCESS_WRITE,
    NAND_ACCESS_RW
} NANDAccessType;

typedef enum {
    // Read/write by owner
    NAND_PERM_RUSR = (NAND_ACCESS_READ << 4),
    NAND_PERM_WUSR = (NAND_ACCESS_WRITE << 4),
    // Read/write by group
    NAND_PERM_RGRP = (NAND_ACCESS_READ << 2),
    NAND_PERM_WGRP = (NAND_ACCESS_WRITE << 2),
    // Read/write by other
    NAND_PERM_ROTH = (NAND_ACCESS_READ << 0),
    NAND_PERM_WOTH = (NAND_ACCESS_WRITE << 0),
    // Read/write by all
    NAND_PERM_RALL = NAND_PERM_RUSR | NAND_PERM_RGRP | NAND_PERM_ROTH,
    NAND_PERM_WALL = NAND_PERM_WUSR | NAND_PERM_WGRP | NAND_PERM_WOTH,
    NAND_PERM_RWALL = NAND_PERM_RALL | NAND_PERM_WALL
} NANDPermission;

typedef struct NANDFileInfo {
    s32 fileDescriptor;
    s32 origFd;
    char origPath[NAND_MAX_PATH];
    char tmpPath[NAND_MAX_PATH];
    u8 accType;
    u8 stage;
    u8 mark;
} NANDFileInfo;

typedef struct NANDCommandBlock {
    void* userData;
    void* callback;
    void* fileInfo;
    void* bytes;
    void* inodes;
    void* status;
    u32 ownerId;
    u16 groupId;
    u8  nextStage;
    u32 attr;
    u32 ownerAcc;
    u32 groupAcc;
    u32 othersAcc;
    u32 num;
    char absPath[NAND_MAX_PATH];
    u32* length;
    u32* pos;
    int state;
    void* copyBuf;
    u32 bufLength;
    u8* type;
    u32 uniqNo;
    u32 reqBlocks;
    u32 reqInodes;
    u32* answer;
    u32 homeBlocks;
    u32 homeInodes;
    u32 userBlocks;
    u32 userInodes;
    u32 workBlocks;
    u32 workInodes;
    const char* *dir;
    BOOL simpleFlag;
} NANDCommandBlock;

typedef struct NANDStatus {
    u32 ownerId;
    u16 groupId;
    u8 attribute;
    u8 permission;
} NANDStatus;

typedef struct {
    u32 signature;
    u32 flag;
    u16 iconSpeed;
    u8 reserved[22];
    u16 comment[2][32];
    u8 bannerTexture[192 * 64 * 2];
    u8 iconTexture[8][48 * 48 * 2];
} NANDBanner;

#define NAND_STAT_SPEED_END 0
#define NAND_STAT_SPEED_FAST 1
#define NAND_STAT_SPEED_MIDDLE 2
#define NAND_STAT_SPEED_SLOW 3
#define NAND_STAT_SPEED_MASK 3

#define NANDSetIconSpeed(stat, n, f)                                                               \
    ((stat).iconSpeed =                                                                            \
        (u16)(((stat).iconSpeed & ~(NAND_STAT_SPEED_MASK << (2 * (n)))) | ((f) << (2 * (n)))))

typedef void (*NANDCallback)(s32, NANDCommandBlock*);
typedef void (*NANDAsyncCallback)(s32 result, struct NANDCommandBlock* block);

typedef void (*NANDLoggingCallback)(BOOL, s32);

// NAND
s32 NANDCreate(const char* path, const u8 perm, const u8 attr);
s32 NANDPrivateCreate(const char* path, u8 perm, u8 attr);
s32 NANDPrivateCreateAsync(const char *path, u8 perm, u8 attr, NANDCallback cb, NANDCommandBlock* block);

s32 NANDDelete(const char* path);
s32 NANDPrivateDelete(const char* path);
s32 NANDPrivateDeleteAsync(const char* path, NANDCallback cb, NANDCommandBlock* block);

s32 NANDRead(NANDFileInfo* info, void* buf, const u32 length);
s32 NANDReadAsync(NANDFileInfo* info, void* buf, const u32 length,
                  NANDCallback cb, NANDCommandBlock* block);

s32 NANDWrite(NANDFileInfo* info, const void* buf, const u32 length);
s32 NANDWriteAsync(NANDFileInfo* info, const void* buf, const u32 length,
                   NANDCallback cb, NANDCommandBlock* block);

s32 NANDSeek(NANDFileInfo* info, const s32 offset, const s32 whence);
s32 NANDSeekAsync(NANDFileInfo* info, const s32 offset, const s32 whence,
                  NANDCallback cb, NANDCommandBlock* block);

s32 NANDPrivateCreateDir(const char* path, u8 perm, u8 attr);
s32 NANDPrivateCreateDirAsync(const char *path, u8 perm, u8 attr, NANDCallback cb, NANDCommandBlock *block);

s32 NANDMove(const char* path, const char* destDir);

s32 NANDGetLength(NANDFileInfo* info, u32* length);
s32 NANDGetLengthAsync(NANDFileInfo* info, u32* length, NANDCallback cb,
                       NANDCommandBlock* block);

s32 NANDGetStatus(const char* path, NANDStatus* stat);
s32 NANDPrivateGetStatus(const char* path, NANDStatus* stat);
s32 NANDPrivateGetStatusAsync(const char* path, NANDStatus* stat,
                              NANDCallback cb, NANDCommandBlock* block);

void NANDSetUserData(NANDCommandBlock* block, void* data);
void* NANDGetUserData(const NANDCommandBlock* block);

// NANDCore
s32 NANDInit(void);
s32 NANDGetHomeDir(char[NAND_MAX_PATH]);
s32 NANDPrivateGetTypeAsync(const char* path, u8* type, NANDCallback cb, NANDCommandBlock* block);
void NANDInitBanner(NANDBanner* bnr, u32 const flag, const u16* title, const u16* comment);

// NANDCheck
s32 NANDCheck(const u32 fsBlock, const u32 inode, u32* answer);

// NANDOpenClose
s32 NANDOpen(const char*, NANDFileInfo*, u8);
s32 NANDPrivateOpen(const char*, NANDFileInfo*, u8);
s32 NANDOpenAsync(const char*, NANDFileInfo*, u8, NANDCallback, NANDCommandBlock*);
s32 NANDPrivateOpenAsync(const char*, NANDFileInfo*, const u8, NANDCallback, NANDCommandBlock*);

s32 NANDClose(NANDFileInfo*);
s32 NANDCloseAsync(NANDFileInfo*, NANDCallback, NANDCommandBlock*);

#ifdef SDK_SEP2006
s32 NANDSafeOpen(const char* path, NANDFileInfo* info, const u8 accType, void* buf, const u32 length);
s32 NANDSafeClose(NANDFileInfo* info);
#else
s32 NANDSimpleSafeOpen(const char* path, NANDFileInfo* info, const u8 accType, void* buf, const u32 length);
s32 NANDSimpleSafeClose(NANDFileInfo* info);
#endif

s32 NANDPrivateSafeOpenAsync(const char* path, NANDFileInfo* info, const u8 accType, void* buf, const u32 length, NANDCallback cb, NANDCommandBlock* block);
s32 NANDSafeCloseAsync(NANDFileInfo* info, NANDCallback cb, NANDCommandBlock* block);

// NANDLogging
BOOL NANDLoggingAddMessageAsync(NANDLoggingCallback cb, s32 errorCode, const char* fmt, ...);

// NANDErrorMessage
BOOL NANDSetAutoErrorMessaging(BOOL show);
void __NANDPrintErrorMessage(s32 errorCode);


const char* nandGetHomeDir();
void nandGenerateAbsPath(char* absPath, const char* path);
BOOL nandIsPrivatePath(const char* path);
BOOL nandIsInitialized(void);
s32 nandConvertErrorCode(const ISFSError err);
void nandGetRelativeName(char* name, const char* path);
BOOL nandIsUnderPrivatePath(const char* path);
BOOL nandIsRelativePath(const char* path);
void nandGetParentDirectory(char* parentDir, const char* absPath);
BOOL nandIsAbsolutePath(const char* path);

#ifdef __cplusplus
}
#endif

#endif
