#ifndef _REVOLUTION_IOSRESTYPES_H_
#define _REVOLUTION_IOSRESTYPES_H_

#include <revolution/types.h>

typedef u32 IOSUid;
typedef u16 IOSGid;
typedef u32 IOSResourceHandle;

#define IOS_ERROR_OK                     0
#define IOS_ERROR_ACCESS                -1
#define IOS_ERROR_EXISTS                -2
#define IOS_ERROR_INTR                  -3
#define IOS_ERROR_INVALID               -4
#define IOS_ERROR_MAX                   -5
#define IOS_ERROR_NOEXISTS              -6
#define IOS_ERROR_QEMPTY                -7
#define IOS_ERROR_QFULL                 -8
#define IOS_ERROR_UNKNOWN               -9
#define IOS_ERROR_NOTREADY             -10
#define IOS_ERROR_ECC                  -11
#define IOS_ERROR_ECC_CRIT             -12
#define IOS_ERROR_BADBLOCK             -13

#define IOS_ERROR_INVALID_OBJTYPE      -14
#define IOS_ERROR_INVALID_RNG          -15
#define IOS_ERROR_INVALID_FLAG         -16
#define IOS_ERROR_INVALID_FORMAT       -17
#define IOS_ERROR_INVALID_VERSION      -18
#define IOS_ERROR_INVALID_SIGNER       -19
#define IOS_ERROR_FAIL_CHECKVALUE      -20
#define IOS_ERROR_FAIL_INTERNAL        -21
#define IOS_ERROR_FAIL_ALLOC           -22
#define IOS_ERROR_INVALID_SIZE         -23

typedef struct {
    u8* path;
    u32 flags;
    IOSUid uid; 
    IOSGid gid;
} IOSResourceOpen;

typedef struct {
    u8* outPtr;
    u32 outLen;
} IOSResourceRead;

typedef struct {
    u8* inPtr;
    u32 inLen;
} IOSResourceWrite;

typedef struct {
    s32 offset;
    u32 whence;
} IOSResourceSeek;

typedef struct {
    u32 cmd;
    u8* inPtr;
    u32 inLen;
    u8* outPtr;
    u32 outLen;
} IOSResourceIoctl;

typedef struct {
    u8* base;
    u32 length;
} IOSIoVector;

typedef struct {
    u32 cmd;
    u32 readCount;
    u32 writeCount;
    IOSIoVector* vector;
} IOSResourceIoctlv;

typedef struct {
    u32 cmd;
    s32 status;
    IOSResourceHandle handle;
    union {
        IOSResourceOpen open;
        IOSResourceRead read;
        IOSResourceWrite write;
        IOSResourceSeek seek;
        IOSResourceIoctl ioctl;
        IOSResourceIoctlv ioctlv;
    } args;
} IOSResourceRequest;

#endif
