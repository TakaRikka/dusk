#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JSupport/JSUInputStream.h"
#include "JSystem/JSupport/JSURandomInputStream.h"
#include <dolphin/dolphin.h>

JSUInputStream::~JSUInputStream() {
    if (!isGood()) {
        OS_REPORT("JSUInputStream: occur error.\n");
    }
}

s32 JSUInputStream::read(void* buffer, s32 numBytes) {
    s32 bytesRead = readData(buffer, numBytes);
    if (bytesRead != numBytes) {
        setState(IOS_STATE_1);
    }
    return bytesRead;
}

char* JSUInputStream::read(char* str) {
    u16 sp8;
    if (readData(&sp8, sizeof(sp8)) != sizeof(sp8)) {
        *str = 0;
        setState(IOS_STATE_1);
        return 0;
    }

    s32 len = readData(str, sp8);
    str[len] = 0;
    if (len != sp8) {
        setState(IOS_STATE_1);
    }
    return str;
}

s32 JSUInputStream::skip(s32 count) {
    s32 skipCount;
    for (skipCount = 0; skipCount < count; skipCount++) {
        u8 buffer;
        if (readData(&buffer, sizeof(buffer)) != sizeof(buffer)) {
            setState(IOS_STATE_1);
            break;
        }
    }

    return skipCount;
}

s32 JSURandomInputStream::align(s32 alignment) {
    s32 seekLen = 0;
    s32 currentPos = getPosition();

    s32 offset = (alignment - 1 + currentPos) & ~(alignment - 1);
    s32 alignmentOffset = offset - currentPos;

    if (alignmentOffset != 0) {
        seekLen = seekPos(offset, JSUStreamSeekFrom_SET);
        if (seekLen != alignmentOffset) {
            setState(IOS_STATE_1);
        }
    }
    return alignmentOffset;
}

s32 JSURandomInputStream::skip(s32 param_0) {
    s32 val = seekPos(param_0, JSUStreamSeekFrom_CUR);
    if (val != param_0) {
        setState(IOS_STATE_1);
    }
    return val;
}

s32 JSURandomInputStream::peek(void* buffer, s32 numBytes) {
    s32 oldPos = getPosition();
    s32 bytesRead = read(buffer, numBytes);
    if (bytesRead != 0) {
        seekPos(oldPos, JSUStreamSeekFrom_SET);
    }
    return bytesRead;
}

s32 JSURandomInputStream::seek(s32 param_0, JSUStreamSeekFrom param_1) {
    s32 seekResult = seekPos(param_0, param_1);
    clrState(IOS_STATE_1);
    return seekResult;
}
