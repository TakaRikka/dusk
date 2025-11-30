#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JSupport/JSUMemoryStream.h"
#include "string.h"

void JSUMemoryInputStream::setBuffer(void const* pBuffer, s32 length) {
    mBuffer = pBuffer;
    mLength = length;
    mPosition = 0;
}

u32 JSUMemoryInputStream::readData(void* pData, s32 length) {
    if (mPosition + length > mLength) {
        length = mLength - mPosition;
    }

    if (length > 0) {
        memcpy(pData, (void*)((s32)mBuffer + mPosition), length);
        mPosition += length;
    }

    return length;
}

s32 JSUMemoryInputStream::seekPos(s32 pos, JSUStreamSeekFrom seekFrom) {
    s32 oldPos = mPosition;

    switch (seekFrom) {
    case JSUStreamSeekFrom_SET:
        mPosition = pos;
        break;
    case JSUStreamSeekFrom_END:
        mPosition = mLength - pos;
        break;
    case JSUStreamSeekFrom_CUR:
        mPosition += pos;
        break;
    }

    if (mPosition < 0) {
        mPosition = 0;
    }

    if (mPosition > mLength) {
        mPosition = mLength;
    }

    return mPosition - oldPos;
}

s32 JSUMemoryInputStream::getLength() const {
    return mLength;
}

s32 JSUMemoryInputStream::getPosition() const {
    return mPosition;
}

void JSUMemoryOutputStream::setBuffer(void* pBuffer, s32 length) {
    mBuffer = pBuffer;
    mLength = length;
    mPosition = 0;
}

s32 JSUMemoryOutputStream::writeData(const void* pData, s32 length) {
    if (mPosition + length > mLength) {
        length = mLength - mPosition;
    }

    if (length > 0) {
        memcpy((void*)((s32)mBuffer + mPosition), pData, length);
        mPosition += length;
    }

    return length;
}

s32 JSUMemoryOutputStream::seekPos(s32 pos, JSUStreamSeekFrom seekFrom) {
    s32 oldPos = mPosition;

    switch (seekFrom) {
    case JSUStreamSeekFrom_SET:
        mPosition = pos;
        break;
    case JSUStreamSeekFrom_END:
        mPosition = mLength - pos;
        break;
    case JSUStreamSeekFrom_CUR:
        mPosition += pos;
        break;
    }

    if (mPosition < 0) {
        mPosition = 0;
    }

    if (mPosition > mLength) {
        mPosition = mLength;
    }

    return mPosition - oldPos;
}

s32 JSUMemoryOutputStream::getLength() const {
    return mLength;
}
