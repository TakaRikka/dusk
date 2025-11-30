#pragma once

#include <revolution/types.h>

typedef s32 IOSError;
typedef s32 IOSCid;
typedef s32 IOSFd;
typedef s32 IOSMessageQueueId;
typedef s32 IOSMessage;
typedef s32 IOSTimerId;
typedef s32 IOSHeapId;
typedef s32 IOSThreadId;
typedef s32 IOSProcessId;
typedef s32 IOSInterfaceId;
typedef u32 IOSTime;
typedef u32 IOSEvent;

typedef u8 IOSCName[64];
typedef u8 IOSCSigDummy[60];
typedef u8 IOSCHash[20];

typedef u8 CSLOSEccPublicKey[60];
typedef u8 CSLOSRsaSig2048[256];

typedef u8 CSLOSAesKey[16];

typedef CSLOSEccPublicKey IOSCEccPublicKey;
typedef CSLOSRsaSig2048 IOSCRsaSig2048;

typedef CSLOSAesKey IOSCAesKey;

typedef enum {
    IOSC_SIG_RSA4096 = 0x00010000,
    IOSC_SIG_RSA2048,
    IOSC_SIG_ECC
} IOSCCertSigType;

typedef struct {
    IOSCCertSigType sigType;
    IOSCRsaSig2048 sig;
    IOSCSigDummy dummy;
    IOSCName issuer;
} IOSCSigRsa2048;
