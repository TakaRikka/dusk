#pragma once

#include <revolution/private/iosrestypes.h>
#include <revolution/private/iostypes.h>
#include <revolution/types.h>

typedef IOSError (*IOSIpcCb) (IOSError, void *);

s32 IOS_Read(s32, void*, u32);
