#ifndef ARC_H
#define ARC_H

#include <revolution/types.h>

#ifdef __cplusplus
extern "C" {
#endif

    typedef struct {
        unsigned int magic;
        int fstStart;
        int fstSize;
        int fileStart;
        int pad[4];
    } ARCHeader;

    typedef struct {
        void* archiveStartAddr;     // 0x0
        void* FSTStart;             // 0x4
        void* fileStart;            // 0x8
        u32 entryNum;               // 0xC
        char* FSTStringStart;       // 0x10
        u32 FSTLength;              // 0x14
        u32 currDir;                // 0x18
    } ARCHandle;

    typedef struct {
        ARCHandle* handle;
        u32 startOffset;
        u32 length;
    } ARCFileInfo;

    typedef struct {
        ARCHandle* handle;
        u32 entryNum;
        u32 location;
        u32 next;
    } ARCDir;

    typedef struct  {
        ARCHandle* handle;
        u32 entryNum;
        BOOL isDir;
        char* name;
    } ARCDirEntry;

    BOOL ARCInitHandle(void *, ARCHandle *);
    BOOL ARCFastOpen(ARCHandle *, s32, ARCFileInfo *);
    s32 ARCConvertPathToEntrynum(ARCHandle *, const char *);
    void* ARCGetStartAddrInMem(ARCFileInfo *);
    u32 ARCGetLength(ARCFileInfo *);
    BOOL ARCClose(ARCFileInfo *);
    BOOL ARCChangeDir(ARCHandle *, const char *);
    BOOL ARCGetCurrentDir(ARCHandle *, char *, u32);

    BOOL ARCOpenDir(ARCHandle *, const char *, ARCDir *);
    BOOL ARCReadDir(ARCDir *, ARCDirEntry *);
    BOOL ARCCloseDir(ARCDir *);

#ifdef __cplusplus
}
#endif

#endif // ARC_H
