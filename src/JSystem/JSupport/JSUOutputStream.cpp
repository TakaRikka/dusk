#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JSupport/JSUOutputStream.h"
#include "JSystem/JSupport/JSURandomOutputStream.h"
#include <dolphin/dolphin.h>
#include <cstring.h>

JSUOutputStream::~JSUOutputStream() {
    if (!isGood()) {
        OS_REPORT("JSUOutputStream: occur error.\n");
    }
}

s32 JSUOutputStream::write(const void* buffer, s32 numBytes) {
    s32 bytesWrote = writeData(buffer, numBytes);
    if (bytesWrote != numBytes) {
        setState(IOS_STATE_1);
    }
    return bytesWrote;
}

void JSUOutputStream::write(const char* str) {
    if (str == NULL) {
        u16 spA = 0;
        if (writeData(&spA, sizeof(spA)) != sizeof(spA)) {
            setState(IOS_STATE_1);
        }
    } else {
        int len = strlen(str);
        if (len >= 0x10000) {
            setState(IOS_STATE_2);
        } else {
            u16 sp8 = len;
            if (writeData(&sp8, sizeof(sp8)) != sizeof(sp8) || writeData(str, len) != len) {
                setState(IOS_STATE_1);
            }
        }
    }    
}

s32 JSUOutputStream::skip(s32 count, s8 param_1) {
    s32 skipCount = 0;
    for (; skipCount < count; skipCount++) {
        if (writeData(&param_1, sizeof(param_1)) != sizeof(param_1)) {
            setState(IOS_STATE_1);
            break;
        }
    }

    return skipCount;
}

s32 JSURandomOutputStream::seek(s32 param_0, JSUStreamSeekFrom param_1) {
    s32 seekResult = seekPos(param_0, param_1);
    clrState(IOS_STATE_1);
    return seekResult;
}

s32 JSURandomOutputStream::getAvailable() const {
    return getLength() - getPosition();
}
