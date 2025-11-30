#ifndef _REVOLUTION_NAND_H_
#define _REVOLUTION_NAND_H_

#include <revolution/types.h>

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

typedef void (*NANDCallback)(s32, NANDCommandBlock*);
typedef void (*NANDAsyncCallback)(s32 result, struct NANDCommandBlock* block);

s32 NANDInit(void);

s32 NANDCreate(const char*, u8, u8);
s32 NANDPrivateCreate(const char*, u8, u8);

s32 NANDOpen(const char*, NANDFileInfo*, u8);
s32 NANDPrivateOpen(const char*, NANDFileInfo*, u8);
s32 NANDOpenAsync(const char*, NANDFileInfo*, u8, NANDCallback, NANDCommandBlock*);
s32 NANDPrivateOpenAsync(const char*, NANDFileInfo*, const u8, NANDCallback, NANDCommandBlock*);

s32 NANDClose(NANDFileInfo*);
s32 NANDCloseAsync(NANDFileInfo*, NANDCallback, NANDCommandBlock*);
s32 NANDRead(NANDFileInfo*, void*, u32);
s32 NANDReadAsync(NANDFileInfo*, void*, u32, NANDCallback, NANDCommandBlock*);

s32 NANDGetLength(NANDFileInfo*, u32*);

s32 NANDDelete(const char*);

s32 NANDMove(const char*, const char*);

s32 NANDCheck(u32, u32, u32*);

s32 NANDWrite(NANDFileInfo*, const void*, u32);
s32 NANDWriteAsync(NANDFileInfo*, const void*, u32, NANDCallback, NANDCommandBlock*);

s32 NANDSeekAsync(NANDFileInfo*, s32, s32, NANDCallback, NANDCommandBlock*);

s32 NANDPrivateGetStatus(const char*, NANDStatus*);
s32 NANDPrivateDelete(const char*);
s32 NANDPrivateCreate(const char*, u8, u8);

s32 NANDGetHomeDir(char[NAND_MAX_PATH]);

s32 NANDGetStatus(const char*, NANDStatus*);

s32 NANDSecretGetUsage(const char*, u32*, u32*);

#ifdef __cplusplus
}
#endif

#endif
