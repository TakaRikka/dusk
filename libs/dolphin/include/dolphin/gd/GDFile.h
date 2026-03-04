#ifndef _DOLPHIN_GD_FILE_H
#define _DOLPHIN_GD_FILE_H

#include <dolphin/types.h>

#ifdef __cplusplus
extern "C" {
#endif

#define GD_FILE_VERSION_NUMBER 0x11223344

typedef struct _GDGList {
    void* ptr;
    u32 byteLength;
} GDGList;

typedef struct _GDFileHeader {
    u32 versionNumber;
    u32 numDLs;
    u32 numPLs;
    GDGList* DLDescArray;
    GDGList* PLDescArray;
} GDFileHeader;

s32 GDReadDLFile(const char* fName, u32* numDLs, u32* numPLs, GDGList** DLDescArray, GDGList** PLDescArray);

#ifdef __cplusplus
}
#endif

#endif
