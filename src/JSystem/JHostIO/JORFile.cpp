#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JHostIO/JORFile.h"
#include "JSystem/JHostIO/JORServer.h"
#include <dolphin/dolphin.h>

JORFile::JORFile()
: mHandle(0),
  mFileLength(0),
  mStatus(EStatus_WAIT),
  mNFileName(0),
  field_0x18(0),
  mDataStream() {}

int JORFile::countMaskSize(const char* mask) {
    u32 var_r28 = 0;
    u32 var_r30 = 0;

    while (true) {
        u32 var_r27 = var_r30;

        while (mask[var_r30] != 0) {
            var_r30++;
            if (var_r30 - var_r27 > 0xFF) {
                return 0;
            }
        }

        var_r30++;
        while (mask[var_r30] != 0) {
            var_r30++;
            if (var_r30 - var_r27 > 0xFF) {
                return 0;
            }
        }

        var_r30++;
        if (mask[var_r30] == 0) {
            return var_r30 + 1;
        }

        var_r28++;
        if (var_r28 > 0x28) {
            return 0;
        }
    }
}

void JORFile::waitMessage_() {
    while (mStatus == EStatus_WAIT) {
        JOR_MESSAGELOOP();
    }
}

int JORFile::open(const char* path, u32 flags, const char* extMask, const char* defaultExt, const char* param_4, const char* fileSuffix) {
    if (mHandle != 0) {
        return 0;
    }

    mStatus = EStatus_WAIT;
    mFlags = flags;

    if (extMask == NULL) {
        extMask = "";
    }

    u32 mask_size = countMaskSize(extMask);
    JORMContext* mctx = JORServer::getInstance()->attachMCTX(MCTX_MSG_FIO);
    mctx->openFile(this, mFlags, path, extMask, mask_size, defaultExt, param_4, fileSuffix);
    JORServer::getInstance()->releaseMCTX(mctx);

    waitMessage_();

    if (mHandle == 0) {
        return 0;
    }

    return 1;
}

void JORFile::close() {
    JORMContext* mctx;
    if (mHandle != 0) {
        mStatus = EStatus_WAIT;
        mctx = JORServer::getInstance()->attachMCTX(MCTX_MSG_FIO);
        mctx->closeFile(this);
        JORServer::getInstance()->releaseMCTX(mctx);

        waitMessage_();
        mHandle = 0;
    }
}

void JORFile::readBegin_(s32 len) {
    mStatus = EStatus_WAIT;
    JORMContext* mctx = JORServer::getInstance()->attachMCTX(MCTX_MSG_FIO);
    mctx->readBegin(this, len);
    JORServer::getInstance()->releaseMCTX(mctx);

    waitMessage_();
}

void JORFile::readLoop_() {
    mStatus = EStatus_WAIT;
    JORMContext* mctx = JORServer::getInstance()->attachMCTX(MCTX_MSG_FIO);
    mctx->readData(this);
    JORServer::getInstance()->releaseMCTX(mctx);

    waitMessage_();
}

s32 JORFile::readData(void* buffer, s32 length) {
    if (mHandle == 0) {
        return 0;
    }

    s32 len = length == 0 ? mFileLength : length;
    setBuffer(buffer, len);
    readBegin_(len);

    do {
        readLoop_();
        if (mStatus == EStatus_READ_BEGIN) {
            setBuffer(NULL, 0);
            return 0;
        }
    } while (mStatus != EStatus_WRITE_END);
    
    setBuffer(NULL, 0);
    DCFlushRange(buffer, len);
    return len;
}

void JORFile::setBuffer(void* buffer, s32 length) {
    mDataStream.setBuffer(buffer, length);
}

void JORFile::writeBegin_(s32 len) {
    mStatus = EStatus_WAIT;
    JORMContext* mctx = JORServer::getInstance()->attachMCTX(MCTX_MSG_FIO);
    mctx->writeBegin(this, mFlags, len);
    JORServer::getInstance()->releaseMCTX(mctx);

    waitMessage_();
}

void JORFile::writeLoop_(const void* pBuffer, s32 size, u32 pos) {
    mStatus = EStatus_WAIT;
    JORMContext* mctx = JORServer::getInstance()->attachMCTX(MCTX_MSG_FIO);
    mctx->writeData(this, pBuffer, size, pos);
    JORServer::getInstance()->releaseMCTX(mctx);

    waitMessage_();
}

void JORFile::writeDone_(s32 len) {
    mStatus = EStatus_WAIT;
    JORMContext* mctx = JORServer::getInstance()->attachMCTX(MCTX_MSG_FIO);
    mctx->writeDone(this, len);
    JORServer::getInstance()->releaseMCTX(mctx);

    waitMessage_();
}

s32 JORFile::writeData(const void* buffer, s32 length) {
    if (mHandle == 0) {
        return 0;
    }

    s32 len = length == 0 ? mFileLength : length;
    writeBegin_(len);

    u32 pos = 0;
    s32 size = 0;
    u8* buf_start = (u8*)buffer;
    do {
        size = len - pos > 0xF00 ? 0xF00 : len - pos;
        writeLoop_(buf_start + pos, size, pos);

        pos += size;
    } while (len - pos != 0);
    
    writeDone_(len);
    return len;
}
