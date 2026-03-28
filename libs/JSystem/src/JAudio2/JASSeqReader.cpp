/**
 * JASSeqReader.cpp
 * 
 */

#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JAudio2/JASSeqReader.h"

void JASSeqReader::init() {
    mBase = 0;
    mCurPos = 0;
    mCurStackDepth = 0;

    for (int i = 0; i < JAS_SEQ_STACK_SIZE; i++) {
        mReturnAddr[i] = NULL;
        mLoopCount[i] = 0;
    }
}

void JASSeqReader::init(void* base) {
    mBase = (u8*)base;
    mCurPos = mBase;
    mCurStackDepth = 0;

    for (int i = 0; i < JAS_SEQ_STACK_SIZE; i++) {
        mReturnAddr[i] = NULL;
        mLoopCount[i] = 0;
    }
    
}

bool JASSeqReader::call(u32 param_0) {
    if (mCurStackDepth >= JAS_SEQ_STACK_SIZE) {
        JUT_WARN(42, "%s", "Cannot exec call command");
        return false;
    }

    mReturnAddr[mCurStackDepth++] = (u16*)mCurPos;
    mCurPos = mBase + param_0;

    return true;
}

bool JASSeqReader::loopStart(u32 param_0) {
    if (JAS_SEQ_STACK_SIZE <= mCurStackDepth) {
        JUT_WARN(53, "%s", "Cannot exec loopStart command");
        return false;
    }

    mReturnAddr[mCurStackDepth] = (u16*)mCurPos;
    mLoopCount[mCurStackDepth++] = param_0;

    return true;
}


bool JASSeqReader::loopEnd() {
    if (mCurStackDepth == 0) {
        JUT_WARN(65, "%s", "cannot loopE for call-stack is NULL");
        return false;
    }

    u16 tmp = mLoopCount[mCurStackDepth - 1];

    if (tmp != 0) {
        tmp--;
    }

    if (!tmp) {
        mCurStackDepth--;
        return true;
    }

    mLoopCount[mCurStackDepth - 1] = tmp;
    mCurPos = (u8*)mReturnAddr[mCurStackDepth - 1];
    return true;
}

bool JASSeqReader::ret() {
    if (mCurStackDepth == 0) {
        return false;
    }

    mCurPos = (u8*)mReturnAddr[--mCurStackDepth];

    return true;
}

int JASSeqReader::readMidiValue() {
    int byte = readByte();

    if (!(byte & 0x80)) {
        return byte;
    }

    byte &= 0x7f;

    int i = 0;

    while (true) {
        if (2 < i) {
            JUT_WARN(100, "%s", "readMidiValue: Too large value");
            return 0;
        }

        u8 newByte = readByte();
        
        byte = byte << 7;
        byte |= newByte & 0x7f;

        if (!(newByte & 0x80)) {
            break;
        }

        i++;
    }

    return byte;
}
