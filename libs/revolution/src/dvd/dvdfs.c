#include <revolution.h>
#include <revolution/dvd.h>

#include "__dvd.h"

typedef struct FSTEntry {
    /* 0x00 */ unsigned int isDirAndStringOff;
    /* 0x04 */ unsigned int parentOrPosition;
    /* 0x08 */ unsigned int nextEntryOrLength;
} FSTEntry;

static OSBootInfo* BootInfo;
static FSTEntry* FstStart;
static char* FstStringStart;
static u32 MaxEntryNum;
static u32 currentDirectory;

OSThreadQueue __DVDThreadQueue;
u32 __DVDLongFileNameFlag = 1;

// prototypes
static BOOL isSame(const char* path, const char* string);
static u32 myStrncpy(char* dest, char* src, u32 maxlen);
static u32 entryToPath(u32 entry, char* path, u32 maxlen);
static BOOL DVDConvertEntrynumToPath(s32 entrynum, char* path, u32 maxlen);
static void cbForReadAsync(s32 result, DVDCommandBlock* block);
static void cbForReadSync(s32 result, DVDCommandBlock* block);
static void cbForSeekAsync(s32 result, DVDCommandBlock* block);
static void cbForSeekSync(s32 result, DVDCommandBlock* block);
static void cbForPrepareStreamAsync(s32 result, DVDCommandBlock* block);
static void cbForPrepareStreamSync(s32 result, DVDCommandBlock* block);

void __DVDFSInit(void) {
    BootInfo = (void*)OSPhysicalToCached(0);
    FstStart = BootInfo->FSTLocation;
    if (FstStart) {
        MaxEntryNum = FstStart->nextEntryOrLength;
        FstStringStart = (char*)FstStart + (MaxEntryNum* sizeof(FSTEntry));
    }
}

/* For convenience */
#define entryIsDir(i) (((FstStart[i].isDirAndStringOff & 0xff000000) == 0) ? FALSE : TRUE)
#define stringOff(i) (FstStart[i].isDirAndStringOff & ~0xff000000)
#define parentDir(i) (FstStart[i].parentOrPosition)
#define nextDir(i) (FstStart[i].nextEntryOrLength)
#define filePosition(i) (FstStart[i].parentOrPosition)
#define fileLength(i) (FstStart[i].nextEntryOrLength)

static BOOL isSame(const char* path, const char* string) {
    while (*string != '\0') {
        if (tolower(*path++) != tolower(*string++)) {
            return FALSE;
        }
    }
    
    if (*path == '/' || *path == '\0') {
        return TRUE;
    }
    
    return FALSE;
}

s32 DVDConvertPathToEntrynum(const char* pathPtr) {
    const char* ptr;
    char* stringPtr;
    BOOL isDir;
    u32 length;
    u32 dirLookAt;
    u32 i;
    const char* origPathPtr = pathPtr;
    const char* extentionStart;
    BOOL illegal;
    BOOL extention;
    
    ASSERTMSGLINE(383, pathPtr, "DVDConvertPathToEntrynum(): null pointer is specified  ");
    
    dirLookAt = currentDirectory;
    
    while (1) {
        if (*pathPtr == '\0') {
            return (s32)dirLookAt;
        } else if (*pathPtr == '/') {
            dirLookAt = 0;
            pathPtr++;
            continue;
        } else if (*pathPtr == '.') {
            if (*(pathPtr + 1) == '.') {
                if (*(pathPtr + 2) == '/') {
                    dirLookAt = parentDir(dirLookAt);
                    pathPtr += 3;
                    continue;
                } else if (*(pathPtr + 2) == '\0') {
                    return (s32)parentDir(dirLookAt);
                }
            } else if (*(pathPtr + 1) == '/') {
                pathPtr += 2;
                continue;
            } else if (*(pathPtr + 1) == '\0') {
                return (s32)dirLookAt;
            }
        }
        
        if (__DVDLongFileNameFlag == 0) {
            extention = FALSE;
            illegal = FALSE;
        
            for (ptr = pathPtr; (*ptr != '\0') && (*ptr != '/'); ptr++) {
                if (*ptr == '.') {
                    if ((ptr - pathPtr > 8) || (extention == TRUE)) {
                        illegal = TRUE;
                        break;
                    }
                    extention = TRUE;
                    extentionStart = ptr + 1;
            
                } else if (*ptr == ' ')
                    illegal = TRUE;
            }
        
            if ((extention == TRUE) && (ptr - extentionStart > 3))
                illegal = TRUE;
        
            if (illegal)
                OSPanic(__FILE__, 452,
                    "DVDConvertEntrynumToPath(possibly DVDOpen or DVDChangeDir or DVDOpenDir): "
                    "specified directory or file (%s) doesn't match standard 8.3 format. This is a "
                    "temporary restriction and will be removed soon\n",
                    origPathPtr);
        } else {
            for (ptr = pathPtr; (*ptr != '\0') && (*ptr != '/'); ptr++)
                ;
        }
        
        isDir = (*ptr == '\0') ? FALSE : TRUE;
        length = (u32)(ptr - pathPtr);
        
        ptr = pathPtr;
        
        for (i = dirLookAt + 1; i < nextDir(dirLookAt); i = entryIsDir(i) ? nextDir(i) : (i + 1)) {
            if ((entryIsDir(i) == FALSE) && (isDir == TRUE)) {
                continue;
            }
        
            stringPtr = FstStringStart + stringOff(i);
        
            if (isSame(ptr, stringPtr) == TRUE) {
                goto next_hier;
            }
        }
        
        return -1;
    
next_hier:
        if (!isDir) {
            return (s32)i;
        }
        
        dirLookAt = i;
        pathPtr += length + 1;
    }
}

BOOL DVDFastOpen(s32 entrynum, DVDFileInfo* fileInfo) {
    ASSERTMSGLINE(536, fileInfo, "DVDFastOpen(): null pointer is specified to file info address  ");
    ASSERTMSG1LINE(539, (entrynum >= 0) && ((u32) entrynum < (u32) MaxEntryNum), "DVDFastOpen(): specified entry number '%d' is out of range  ", entrynum);
    ASSERTMSG1LINE(542, !entryIsDir(entrynum), "DVDFastOpen(): entry number '%d' is assigned to a directory  ", entrynum);
    
    if (entrynum < 0 || entrynum >= MaxEntryNum || entryIsDir(entrynum)) {
        return FALSE;
    }
    
    fileInfo->startAddr = filePosition(entrynum) >> __DVDLayoutFormat;
    fileInfo->length = fileLength(entrynum);
    fileInfo->callback = (DVDCallback)NULL;
    fileInfo->cb.state = DVD_STATE_END;
    
    return TRUE;
}

BOOL DVDOpen(const char* fileName, DVDFileInfo* fileInfo) {
    s32 entry;
    char currentDir[128];
    
    ASSERTMSGLINE(572, fileName, "DVDOpen(): null pointer is specified to file name  ");
    ASSERTMSGLINE(573, fileInfo, "DVDOpen(): null pointer is specified to file info address  ");
    
    entry = DVDConvertPathToEntrynum(fileName);
    
    if (0 > entry) {
        DVDGetCurrentDir(currentDir, 128);
        OSReport("Warning: DVDOpen(): file '%s' was not found under %s.\n", fileName, currentDir);
        return FALSE;
    }
    
    if (entryIsDir(entry)) {
        ASSERTMSG1LINE(587, !entryIsDir(entry), "DVDOpen(): directory '%s' is specified as a filename  ", fileName);
        return FALSE;
    }
    
    fileInfo->startAddr = filePosition(entry) >> __DVDLayoutFormat;
    fileInfo->length = fileLength(entry);
    fileInfo->callback = (DVDCallback)NULL;
    fileInfo->cb.state = DVD_STATE_END;
    
    return TRUE;
}

BOOL DVDClose(DVDFileInfo* fileInfo) {
    ASSERTMSGLINE(611, fileInfo, "DVDClose(): null pointer is specified to file info address  ");
    DVDCancel(&(fileInfo->cb));
    return TRUE;
}

static u32 myStrncpy(char* dest, char* src, u32 maxlen) {
    u32 i = maxlen;
    
    while ((i > 0) && (*src != 0)) {
        *dest++ = *src++;
        i--;
    }
    
    return (maxlen - i);
}

static u32 entryToPath(u32 entry, char* path, u32 maxlen) {
    u32 i;
    char* name;
    u32 loc;
    
    if (entry == 0) {
        return 0;
    }
    
    name = FstStringStart + stringOff(entry);
    if (entryIsDir(entry)) {
        i = parentDir(entry);
    } else {
        i = entry;
        while (i != 0) {
            if (entryIsDir(i) && nextDir(i) > entry) {
                break;
            }

            i--;
        }
    }
    
    loc = entryToPath(i, path, maxlen);
    if (loc == maxlen) {
        return loc;
    }
    
    *(path + loc++) = '/';
    
    loc += myStrncpy(path + loc, name, maxlen - loc);
    return loc;
}

static BOOL DVDConvertEntrynumToPath(s32 entrynum, char* path, u32 maxlen) {
    u32 loc;
    
    ASSERTMSG1LINE(721, (entrynum >= 0) && (entrynum < MaxEntryNum), "DVDConvertEntrynumToPath: specified entrynum(%d) is out of range  ", entrynum);
    ASSERTMSG1LINE(723, maxlen > 1, "DVDConvertEntrynumToPath: maxlen should be more than 1 (%d is specified)", maxlen);
    
    loc = entryToPath((u32)entrynum, path, maxlen);
    
    if (loc == maxlen) {
        path[maxlen - 1] = '\0';
        return FALSE;
    }
    
    if (entryIsDir(entrynum)) {
        if (loc == maxlen - 1) {
            path[loc] = '\0';
            return FALSE;
        }
    
        path[loc++] = '/';
    }
    
    path[loc] = '\0';
    return TRUE;
}

BOOL DVDGetCurrentDir(char* path, u32 maxlen) {
    ASSERTMSG1LINE(765, (maxlen > 1), "DVDGetCurrentDir: maxlen should be more than 1 (%d is specified)", maxlen);
    return DVDConvertEntrynumToPath((s32)currentDirectory, path, maxlen);
}

BOOL DVDChangeDir(const char* dirName) {
    s32 entry;
    char currentDir[128];
    
    ASSERTMSGLINE(787, dirName, "DVDChangeDir(): null pointer is specified to directory name  ");  
    entry = DVDConvertPathToEntrynum(dirName);
    ASSERTMSG2LINE(795, entry >= 0, "DVDChangeDir(): directory '%s' is not found under %s  ", dirName, (DVDGetCurrentDir(currentDir, 128), currentDir));
    ASSERTMSG1LINE(799, entryIsDir(entry), "DVDChangeDir(): file '%s' is specified as a directory name  ", dirName);
    
    if (entry < 0 || entryIsDir(entry) == FALSE) {
        return FALSE;
    }
    
    currentDirectory = (u32)entry;
    return TRUE;
}

BOOL DVDReadAsyncPrio(DVDFileInfo* fileInfo, void* addr, s32 length, s32 offset, DVDCallback callback, s32 prio) {
    ASSERTMSGLINE(831, fileInfo, "DVDReadAsync(): null pointer is specified to file info address  ");
    ASSERTMSGLINE(832, addr, "DVDReadAsync(): null pointer is specified to addr  ");
    ASSERTMSGLINE(836, !OFFSET(addr, 32), "DVDReadAsync(): address must be aligned with 32 byte boundaries  ");
    ASSERTMSGLINE(838, !(length & 0x1F), "DVDReadAsync(): length must be  multiple of 32 byte  ");
    ASSERTMSGLINE(840, !(offset & 3), "DVDReadAsync(): offset must be multiple of 4 byte  ");

    DVD_ASSERTMSGLINE(845, (0 <= offset) && (offset <= fileInfo->length), "DVDReadAsync(): specified area is out of the file  ");
    
    DVD_ASSERTMSGLINE(851, (0 <= offset + length) && (offset + length < fileInfo->length + DVD_MIN_TRANSFER_SIZE), "DVDReadAsync(): specified area is out of the file  ");
    
    fileInfo->callback = callback;
    DVDReadAbsAsyncPrio(&(fileInfo->cb), addr, length, (s32)(fileInfo->startAddr + (offset >> 2)),
                        cbForReadAsync, prio);
    return TRUE;
}

#ifndef offsetof
#define offsetof(type, memb) ((u32) & ((type*)0)->memb)
#endif

static void cbForReadAsync(s32 result, DVDCommandBlock* block) {
    DVDFileInfo* fileInfo;

    fileInfo = (DVDFileInfo*)((char*)block - offsetof(DVDFileInfo, cb)); 
    ASSERTLINE(869, (void*) &fileInfo->cb == (void*) block);
    if (fileInfo->callback) {
        (fileInfo->callback)(result, fileInfo);
    }
}

s32 DVDReadPrio(DVDFileInfo* fileInfo, void* addr, s32 length, s32 offset, s32 prio) {
    int result;
    DVDCommandBlock* block;
    s32 state;
    BOOL enabled;
    s32 retVal;

    ASSERTMSGLINE(901, fileInfo, "DVDRead(): null pointer is specified to file info address  ");
    ASSERTMSGLINE(902, addr, "DVDRead(): null pointer is specified to addr  ");
    ASSERTMSGLINE(906, !OFFSET(addr, 32), "DVDRead(): address must be aligned with 32 byte boundaries  ");
    ASSERTMSGLINE(908, !(length & 0x1F), "DVDRead(): length must be  multiple of 32 byte  ");
    ASSERTMSGLINE(910, !(offset & 3), "DVDRead(): offset must be multiple of 4 byte  ");

    DVD_ASSERTMSGLINE(915, (0 <= offset) && (offset <= fileInfo->length), "DVDRead(): specified area is out of the file  ");
    DVD_ASSERTMSGLINE(921, (0 <= offset + length) && (offset + length < fileInfo->length + DVD_MIN_TRANSFER_SIZE), "DVDRead(): specified area is out of the file  ");

    block = &fileInfo->cb;
    result = DVDReadAbsAsyncPrio(block, addr, length, fileInfo->startAddr + (offset >> 2), cbForReadSync, prio);
    if (result == 0) {
        return -1;
    }
    enabled = OSDisableInterrupts();

    while (1) {
        state = ((volatile DVDCommandBlock*)block)->state;
        if (state == 0) {
            retVal = (s32)block->transferredSize;
            break;
        } else if (state == -1) {
            retVal = -1;
            break;
        } else if (state == 10) {
            retVal = -3;
            break;
        }
        OSSleepThread(&__DVDThreadQueue);
    }

    OSRestoreInterrupts(enabled);
    return retVal;
}

static void cbForReadSync(s32 result, DVDCommandBlock* block) {
    OSWakeupThread(&__DVDThreadQueue);
}

int DVDSeekAsyncPrio(DVDFileInfo* fileInfo, s32 offset, DVDCallback callback, s32 prio) {
    ASSERTMSGLINE(898, fileInfo, "DVDSeek(): null pointer is specified to file info address  ");
    ASSERTMSGLINE(902, !(offset & 3), "DVDSeek(): offset must be multiple of 4 byte  ");

    DVD_ASSERTMSGLINE(907, (0 <= offset) && (offset <= fileInfo->length), "DVDSeek(): offset is out of the file  ");

    fileInfo->callback = callback;
    DVDSeekAbsAsyncPrio(&fileInfo->cb, (u32)(char*)fileInfo->startAddr + offset, cbForSeekAsync, prio);
    return 1;
}

static void cbForSeekAsync(s32 result, DVDCommandBlock* block) {
    DVDFileInfo* fileInfo;

    fileInfo = (DVDFileInfo *)&block->next;
    ASSERTLINE(925, (void*) &fileInfo->cb == (void*) block);
    if (fileInfo->callback) {
        (fileInfo->callback)(result, fileInfo);
    }
}

s32 DVDSeekPrio(DVDFileInfo* fileInfo, s32 offset, s32 prio) {
    int result;
    DVDCommandBlock* block;
    s32 state;
    BOOL enabled;
    s32 retVal;

    ASSERTMSGLINE(955, fileInfo, "DVDSeek(): null pointer is specified to file info address  ");
    ASSERTMSGLINE(959, !(offset & 3), "DVDSeek(): offset must be multiple of 4 byte  ");
    ASSERTMSGLINE(963, (offset >= 0) && ((u32) offset <= (u32) fileInfo->length), "DVDSeek(): offset is out of the file  ");

    block = &fileInfo->cb;
    result = DVDSeekAbsAsyncPrio(block, (u32)(char*)fileInfo->startAddr + offset, cbForSeekSync, prio);
    if (!result) {
        return -1;
    }
    enabled = OSDisableInterrupts();

    while (1) {
        state = ((volatile DVDCommandBlock*)block)->state;
        if (state == 0) {
            retVal = 0;
            break;
        } else if (state == -1) {
            retVal = -1;
            break;
        } else if (state == 10) {
            retVal = -3;
            break;
        }
        OSSleepThread(&__DVDThreadQueue);
    }

    OSRestoreInterrupts(enabled);
    return retVal;
}

static void cbForSeekSync(s32 result, DVDCommandBlock* block) {
    OSWakeupThread(&__DVDThreadQueue);
}

s32 DVDGetFileInfoStatus(const DVDFileInfo* fileInfo) {
    return DVDGetCommandBlockStatus(&fileInfo->cb);
}

BOOL DVDFastOpenDir(s32 entrynum, DVDDir* dir) {
    ASSERTMSGLINE(1048, dir, "DVDFastOpenDir(): null pointer is specified to dir structure address  ");
    ASSERTMSG1LINE(1051, entrynum >= 0 && entrynum < MaxEntryNum, "DVDFastOpenDir(): specified entry number \'%d\' is out of range  ", entrynum);
    ASSERTMSG1LINE(1054, entryIsDir(entrynum), "DVDFastOpenDir(): entry number \'%d\' is assigned to a file  ", entrynum);

    if (entrynum < 0 || entrynum >= MaxEntryNum || !entryIsDir(entrynum)) {
        return FALSE;
    }

    dir->entryNum = entrynum;
    dir->location = entrynum + 1;
    dir->next = FstStart[entrynum].nextEntryOrLength;
    return TRUE;
}

BOOL DVDOpenDir(const char* dirName, DVDDir* dir) {
    s32 entry;
    char currentDir[128];

    ASSERTMSGLINE(1178, dirName, "DVDOpendir(): null pointer is specified to directory name  ");
    ASSERTMSGLINE(1179, dir, "DVDOpenDir(): null pointer is specified to dir structure address  ");
    
    entry = DVDConvertPathToEntrynum(dirName);
    if (entry < 0) {
        DVDGetCurrentDir(currentDir, sizeof(currentDir));
        OSReport("Warning: DVDOpenDir(): file \'%s\' was not found under %s.\n", dirName, currentDir);
        return FALSE;
    }

    if (!entryIsDir(entry)) {
        ASSERTMSG1LINE(1193, entryIsDir(entry), "DVDOpendir(): file \'%s\' is specified as a directory name  ", dirName);
        return FALSE;
    }

    dir->entryNum = entry;
    dir->location = entry + 1;
    dir->next = nextDir(entry);    
    return TRUE;
}

int DVDReadDir(DVDDir* dir, DVDDirEntry* dirent) {
    u32 loc;

    loc = dir->location;
    if ((loc <= (u32) dir->entryNum) || ((u32) dir->next <= loc)) {
        return 0;
    }
    dirent->entryNum = loc;
    dirent->isDir = entryIsDir(loc);
    dirent->name = FstStringStart + stringOff(loc);
    dir->location = entryIsDir(loc) ? nextDir(loc) : loc + 1;
    return 1;
}

int DVDCloseDir(DVDDir* dir) {
    return 1;
}

void DVDRewindDir(DVDDir* dir) {
    dir->location = dir->entryNum + 1;
}

void* DVDGetFSTLocation(void) {
    return BootInfo->FSTLocation;
}

s32 DVDGetTransferredSize(DVDFileInfo* fileinfo) {
    s32 bytes;
    DVDCommandBlock* cb;

    cb = &(fileinfo->cb);

    switch (cb->state) {
    case DVD_STATE_COVER_CLOSED:
    case DVD_STATE_NO_DISK:
    case DVD_STATE_COVER_OPEN:
    case DVD_STATE_WRONG_DISK:
    case DVD_STATE_FATAL_ERROR:
    case DVD_STATE_MOTOR_STOPPED:
    case DVD_STATE_CANCELED:
    case DVD_STATE_RETRY:
    case DVD_STATE_END:
        bytes = (s32)cb->transferredSize;
        break;
    case DVD_STATE_WAITING:
        bytes = 0;
        break;
    case DVD_STATE_BUSY:
        bytes = (s32)(cb->transferredSize + (cb->currTransferSize - __DIRegs[6]));
        break;
    default:
        ASSERTMSG1LINE(1391, FALSE, "DVDGetTransferredSize(): Illegal state (%d)", cb->state);
        break;
    }

    return bytes;
}

// NOTE: function doesn't exist in TP debug, needed for string data
void DVDGetEntrynum() {
    OSReport("DVDGetEntrynum(): null pointer is specified to file info address  ");
}

// NOTE: function doesn't exist in TP debug, needed for string data
void DVDGetEntryName() {
    OSReport("DVDGetEntryName: specified entrynum(%d) is out of range  ");
}
