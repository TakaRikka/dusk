#include <revolution/arc.h>
#include <revolution.h>
#include <locale>

/* this is here because it won't be inlined otherwise */
inline int tolower(int c) {
    return ((c < 0) || (c >= 0x100)) ? c : (int) (_current_locale.ctype_cmpt_ptr->lower_map_ptr[c]);
}

typedef struct FSTEntry FSTEntry;

struct FSTEntry {
    unsigned int isDirAndStringOff;
    unsigned int parentOrPosition;
    unsigned int nextEntryOrLength;
};

#define entryIsDir(fstStart, i)     \
    ( ( ( fstStart[i].isDirAndStringOff & 0xFF000000 ) == 0 )? FALSE : TRUE )
#define stringOff(fstStart, i)      \
        ( fstStart[i].isDirAndStringOff & 0x00FFFFFF )
#define parentDir(fstStart, i)       \
        ( fstStart[i].parentOrPosition )
#define nextDir(fstStart, i)        \
        ( fstStart[i].nextEntryOrLength )
#define filePosition(fstStart, i)       \
        ( fstStart[i].parentOrPosition )
#define fileLength(fstStart, i)         \
        ( fstStart[i].nextEntryOrLength )

BOOL ARCInitHandle(void* arcStart, ARCHandle* handle) {
    FSTEntry* FSTEntries;
    ARCHeader* arcHeader = (ARCHeader*)arcStart;

    if (arcHeader->magic != 0x55AA382D) {
#if SDK_AUG2010
        OSPanic(__FILE__, 0x4A, "ARCInitHandle: bad archive format");
#else
        OSPanic(__FILE__, 0x47, "ARCInitHandle: bad archive format");
#endif
    }

    handle->archiveStartAddr = arcStart;
    handle->FSTStart = FSTEntries = (void*)((u32)arcStart + arcHeader->fstStart);
    handle->fileStart = (void*)((u32)arcStart + arcHeader->fileStart);
    handle->entryNum = nextDir(FSTEntries, 0);
    handle->FSTStringStart = (char*)&(FSTEntries[handle->entryNum]);
    handle->FSTLength = (u32)arcHeader->fstSize;
    handle->currDir = 0;
    return TRUE;
}

BOOL ARCFastOpen(ARCHandle* handle, s32 entrynum, ARCFileInfo* af) {
    FSTEntry* FSTEntries = (FSTEntry*)handle->FSTStart;

    if ((entrynum < 0) || (entrynum >= handle->entryNum) || entryIsDir(FSTEntries, entrynum)) {
        return FALSE;
    }

    af->handle = handle;
    af->startOffset = filePosition(FSTEntries, entrynum);
    af->length = fileLength(FSTEntries, entrynum);
    return TRUE;
}

static BOOL isSame(const char* path, const char* string) {
    while(*string != '\0') {
        if (tolower(*path++) != tolower(*string++)) {
            return FALSE;
        }
    }

    if ((*path == '/') || (*path == '\0')) {
        return TRUE;
    }

    return FALSE;
}

s32 ARCConvertPathToEntrynum(ARCHandle* handle, const char* pathPtr)
{
    const char*  ptr;
    char* stringPtr;
    BOOL isDir;
    s32 length;
    u32 dirLookAt;
    u32 i;
    const char* origPathPtr = pathPtr;
    FSTEntry* FSTEntries;

    dirLookAt = handle->currDir;
    FSTEntries = (FSTEntry*)handle->FSTStart;

    while (1) {
        if (*pathPtr == '\0') {
            return (s32)dirLookAt;
        }
        else if (*pathPtr == '/') {
            dirLookAt = 0;
            pathPtr++;
            continue;
        }
        else if (*pathPtr == '.') {
            if (*(pathPtr + 1) == '.') {
                if (*(pathPtr + 2) == '/') {
                    dirLookAt = parentDir(FSTEntries, dirLookAt);
                    pathPtr += 3;
                    continue;
                }
                else if (*(pathPtr + 2) == '\0') {
                    return (s32)parentDir(FSTEntries, dirLookAt);
                }
            }
            else if (*(pathPtr + 1) == '/') {
                pathPtr += 2;
                continue;
            }
            else if (*(pathPtr + 1) == '\0') {
                return (s32)dirLookAt;
            }
        }

        for(ptr = pathPtr; (*ptr != '\0') && (*ptr != '/'); ptr++);
        isDir = (*ptr == '\0')? FALSE : TRUE;
        length = (s32)(ptr - pathPtr);
        ptr = pathPtr;

        for(i = dirLookAt + 1; i < nextDir(FSTEntries, dirLookAt);
            i = entryIsDir(FSTEntries, i)? nextDir(FSTEntries, i): (i+1) )
        {
dot:
            if ((entryIsDir(FSTEntries, i) == FALSE) && (isDir == TRUE)) {
                continue;
            }

            stringPtr = handle->FSTStringStart + stringOff(FSTEntries, i);

            if (*stringPtr == '.' && *(stringPtr + 1) == '\0') {
                i++;
                goto dot;
            }

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

    // the world ends if this is reached
}

static u32 myStrncpy(char* dest, char* src, u32 maxlen) {
    u32 i = maxlen;

    while ((i > 0) && (*src != 0)) {
        *dest++ = *src++;
        i--;
    }

    return (maxlen - i);
}

static u32 entryToPath(ARCHandle* handle, u32 entry, char* path, u32 maxlen) {
    char* name;
    u32 loc;
    FSTEntry* FSTEntries = (FSTEntry*)handle->FSTStart;

    if (entry == 0) {
        return 0;
    }

    name = handle->FSTStringStart + stringOff(FSTEntries, entry);
    loc = entryToPath(handle, parentDir(FSTEntries, entry), path, maxlen);

    if (loc == maxlen) {
        return loc;
    }

    *(path + loc++) = '/';
    loc += myStrncpy(path + loc, name, maxlen - loc);
    return loc;
}

static BOOL ARCConvertEntrynumToPath(ARCHandle* handle, s32 entrynum, char* path, u32 maxlen) {
    u32 loc;
    FSTEntry* FSTEntries = (FSTEntry*)handle->FSTStart;

    loc = entryToPath(handle, (u32)entrynum, path, maxlen);

    if (loc == maxlen) {
        path[maxlen - 1] = '\0';
        return FALSE;
    }

    if (entryIsDir(FSTEntries, entrynum)) {
        if (loc == maxlen - 1) {
            path[loc] = '\0';
            return FALSE;
        }

        path[loc++] = '/';
    }

    path[loc] = '\0';
    return TRUE;
}

static BOOL ARCGetCurrentDir(ARCHandle* handle, char* path, u32 maxlen) {
    return ARCConvertEntrynumToPath(handle, (s32)handle->currDir, path, maxlen);
}

void* ARCGetStartAddrInMem(ARCFileInfo* af) {
    ARCHandle* handle = af->handle;
    return (void*)((u32)handle->archiveStartAddr + af->startOffset);
}

u32 ARCGetLength(ARCFileInfo* af) {
    return af->length;
}

BOOL ARCClose(ARCFileInfo* af) {
    return TRUE;
}

BOOL ARCChangeDir(ARCHandle* handle, const char* dirName) {
    s32 entry;
    FSTEntry* FSTEntries;

    entry = ARCConvertPathToEntrynum(handle, dirName);
    FSTEntries = (FSTEntry*)handle->FSTStart;

    if ((entry < 0) || (entryIsDir(FSTEntries, entry) == FALSE)) {
        return FALSE;
    }

    handle->currDir = (u32)entry;
    return TRUE;
}

BOOL ARCOpenDir(ARCHandle* handle, const char* dirName, ARCDir* dir) {
    s32         entry;
    FSTEntry*   FSTEntries;

    entry = ARCConvertPathToEntrynum(handle, dirName);
    FSTEntries = (FSTEntry*)handle->FSTStart;

    if ((entry < 0) || (entryIsDir(FSTEntries, entry) == FALSE)) {
        return FALSE;
    }

    dir->handle = handle;
    dir->entryNum = (u32)entry;
    dir->location = (u32)entry + 1;
    dir->next = nextDir(FSTEntries, entry);
    return TRUE;
}

BOOL ARCReadDir(ARCDir* dir, ARCDirEntry* dirent) {
    u32 loc;
    FSTEntry* FSTEntries;
    ARCHandle* handle;

    handle = dir->handle;
    FSTEntries = (FSTEntry*)handle->FSTStart;
    loc = dir->location;
retry:
    if ((loc <= dir->entryNum) || (dir->next <= loc)) {
        return FALSE;
    }

    dirent->handle = handle;
    dirent->entryNum = loc;
    dirent->isDir = entryIsDir(FSTEntries, loc);
    dirent->name = handle->FSTStringStart + stringOff(FSTEntries, loc);

    if (dirent->name[0] == '.' && dirent->name[1] == '\0') {
        loc++;
        goto retry;
    }

    dir->location = entryIsDir(FSTEntries, loc)? nextDir(FSTEntries, loc) : (loc+1);
    return TRUE;
}

BOOL ARCCloseDir(ARCDir* dir) {
    return TRUE;
}
