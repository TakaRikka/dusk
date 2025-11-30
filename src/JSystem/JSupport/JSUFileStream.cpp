#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JSupport/JSUFileStream.h"
#include "JSystem/JKernel/JKRFile.h"

JSUFileInputStream::JSUFileInputStream(JKRFile* pFile) {
    mFile = pFile;
    mPosition = 0;
}

u32 JSUFileInputStream::readData(void* pBuffer, s32 length) {
    s32 lenRead = 0;
    if (mFile->isAvailable()) {
        if (mPosition + length > (u32)mFile->getFileSize()) {
            length = mFile->getFileSize() - mPosition;
        }

        if (length > 0) {
            lenRead = mFile->readData(pBuffer, length, mPosition);
            if (lenRead < 0) {
                return 0;
            } else {
                mPosition += lenRead;
            }
        }
    }

    return lenRead;
}

s32 JSUFileInputStream::seekPos(s32 pos, JSUStreamSeekFrom seekFrom) {
    s32 oldPos = mPosition;

    switch (seekFrom) {
    case JSUStreamSeekFrom_SET:
        mPosition = pos;
        break;
    case JSUStreamSeekFrom_END:
        mPosition = mFile->getFileSize() - pos;
        break;
    case JSUStreamSeekFrom_CUR:
        mPosition += pos;
        break;
    }

    if (mPosition < 0) {
        mPosition = 0;
    }

    if (mPosition > mFile->getFileSize()) {
        mPosition = mFile->getFileSize();
    }

    return mPosition - oldPos;
}

s32 JSUFileInputStream::getLength() const {
    return mFile->getFileSize();
}

s32 JSUFileInputStream::getPosition() const {
    return mPosition;
}
