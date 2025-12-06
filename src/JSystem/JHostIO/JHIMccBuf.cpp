#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JHostIO/JHIMccBuf.h"
#include "JSystem/JKernel/JKRHeap.h"
#include "JSystem/JHostIO/JHIRMcc.h"
#include <dolphin/dolphin.h>
#include <cstring.h>

extern "C" int HIO2Read(u32, u32, void*, u32);
extern "C" int HIO2Write(u32, u32, void*, u32);

void JHIReport(const char* fmt, ...) {}

void JHIHalt(const char* fmt, ...) {}

u8* JHIMccBuf::mTempBuf;
u16 JHIMccBuf::mRefCount;

JHIMccBuf::JHIMccBuf(u16 channel, u16 param_1, u32 param_2) {
    initInstance(channel, param_1, param_2);
    mRefCount++;
}

void JHIMccBuf::initInstance(u16 channel, u16 param_1, u32 param_2) {
    mTag = 'MCHI';
    field_0x8 = 0x20;
    field_0xa = 0x40;
    field_0xc = 0x60;
    mChannel = channel;
    field_0x10 = param_1;
    field_0x20 = param_2;
    mReadOffset = 0;
    mWriteOffset = (int)(param_1 << 13) / 4;
    field_0x1c = ((int)(param_1 << 13) / 4) - field_0xc;
    mPortEnabled = false;
}

void JHIMccBuf::init() {
    if (mTempBuf != NULL) {
        initBuf();
    } else {
        mTempBuf = new (32) u8[0x18000];
        if (mTempBuf == NULL) {
            JHIHalt("ERROR: JHIMccBuf cannot alloc temp buf.\n");
        } else {
            JHIReport("DEBUG: JHIMccBuf alloc at %08x.\n", mTempBuf);
        }

        initBuf();
    }
}

void JHIMccBuf::initBuf() {
    setHeaderToBuf(mTag, 0, 0x20);
    setHeaderToBuf(field_0xc, field_0x8, 0x20);
    setHeaderToBuf(field_0xc, field_0xa, 0x20);

    if (JHIMCCWrite(mChannel, mReadOffset, mTempBuf, 0x60)) {
        JHIReport("DEBUG: Write to Channel:%d , Offset:%05x , Size: %d\n", mChannel, mReadOffset, 0x60);
        
        if (JHIMCCWrite(mChannel, mWriteOffset, mTempBuf, 0x60)) {
            JHIReport("DEBUG: Write to Channel:%d , Offset:%05x , Size: %d\n", mChannel, mWriteOffset, 0x60);
        }
    }
}

JHIMccBuf::~JHIMccBuf() {
    mRefCount--;
    if (mRefCount == 0) {
        delete[] mTempBuf; 
    }
}

void JHIMccBuf::setHeaderToBuf(u32 data, u32 position, u16 size) {
    u8* ptr = mTempBuf + position;
    ptr[0] = (data >> 0x18) & 0xFF;
    ptr[1] = (data >> 0x10) & 0xFF;
    ptr[2] = (data >> 0x08) & 0xFF;
    ptr[3] = (data >> 0x00) & 0xFF;

    for (int i = 4; i < size; i++) {
        ptr[i] = 0;
    }
}

// NONMATCHING - storing var_r28 to stack instead of register
u32 JHIMccBuf::setDataToBuf(void* pData, u16 size) {
    u8* temp_buf = mTempBuf;
    u32 i;
    u8* data_buf = (u8*)pData;

    temp_buf[0] = (mTag >> 0x18) & 0xFF;
    temp_buf[1] = (mTag >> 0x10) & 0xFF;
    temp_buf[2] = (mTag >> 0x08) & 0xFF;
    temp_buf[3] = (mTag >> 0x00) & 0xFF;
    temp_buf[4] = (size >> 0x8) & 0xFF;
    temp_buf[5] = size & 0xFF;

    for (i = 0; i < size; i++) {
        temp_buf[6 + i] = data_buf[i];
    }

    u32 var_r28 = (size + 0x25) & ~0x1F;
    for (i = size + 6; i < var_r28; i++) {
        temp_buf[i] = 0;
    }

    return var_r28;
}

int JHIMccBuf::readData(u32 position, u32* pOutData) {
    u8* temp_buf = mTempBuf;
    u32 var_r30 = position;

    if (!JHIMCCRead(mChannel, var_r30, temp_buf, 0x20)) {
        return 0;
    }

    *pOutData = (temp_buf[0 + (position - var_r30)] << 0x18) |
               (temp_buf[1 + (position - var_r30)] << 0x10) |
               (temp_buf[2 + (position - var_r30)] << 0x08) |
               (temp_buf[3 + (position - var_r30)] << 0x00);
    return 1;
}

int JHIMccBuf::readDataFromBuf(void* pBuf, u32 size) {
    u8* temp_buf = mTempBuf;
    u8* u_buf = (u8*)pBuf;

    u32 var_r28;
    u32 var_r27 = 0;
    u32 var_r26 = 0;
    u32 var_r30 = 0;
    while (var_r30 < size) {
        u32 sp10 = (temp_buf[0 + var_r30] << 0x18) |
                   (temp_buf[1 + var_r30] << 0x10) |
                   (temp_buf[2 + var_r30] << 0x08) |
                   (temp_buf[3 + var_r30] << 0x00);
        if (sp10 != mTag) {
            return 0;
        }

        var_r26 = (temp_buf[4 + var_r30] << 0x8) |
                  (temp_buf[5 + var_r30] << 0x0);
        if (u_buf != NULL) {
            for (var_r28 = 0; var_r28 < var_r26; var_r28++) {
                u_buf[var_r27 + var_r28] = temp_buf[6 + (var_r30 + var_r28)];
            }
        }

        var_r30 += (var_r26 + 0x25) & ~0x1F;
        var_r27 += var_r26;
        var_r26 = 0;
    }

    return var_r27;
}

int JHIMccBuf::JHIRead(u32 channel, u32 offset, void* buffer, u32 size) {
    JHIReport("DEBUG:      MCCRead(%d, %06x, %08x, %d);\n", channel, offset, buffer, size);

    if (!isPort()) {
        memset(buffer, 0, size);
        return 0;
    }

    int attempts = 0;
    DCInvalidateRange(buffer, size);

    if (offset & 3) {
        disablePort();
        return 0;
    }

    if (offset > (field_0x1c + field_0xc) * 2) {
        OS_REPORT("ERROR: Illegal offset specified Error. -HIOWrite()\n");
        OS_REPORT("ERROR: offset: %08x low:%08x high:%08x\n", offset, 0, (field_0x1c + field_0xc) * 2);
        disablePort();
        return 0;
    }

#if VERSION < VERSION_SHIELD_DEBUG
    while (HIORead(offset + field_0x20, buffer, size) == 0 && attempts < 30) {
        attempts++;
    }
#else
    while (HIO2Read(JHIGetHIO2Handle(), offset + field_0x20, buffer, size) == 0 && attempts < 30) {
        attempts++;
    }
#endif

    if (attempts >= 30) {
        OS_REPORT("ERROR: Communication Error. -HIORead()\n");
        memset(buffer, 0, size);
        disablePort();
        return 0;
    }

    return 1;
}

int JHIMccBuf::JHIWrite(u32 channel, u32 offset, void* buffer, u32 size) {
    JHIReport("DEBUG:      MCCWrite(%d, %06x, %08x, %d);\n", channel, offset, buffer, size);

    if (!isPort()) {
        return 0;
    }

    int attempts = 0;
    DCFlushRange(buffer, size);

    if (offset & 3) {
        disablePort();
        return 0;
    }

    if (offset > (field_0x1c + field_0xc) * 2) {
        OS_REPORT("ERROR: Illegal offset specified Error. -HIOWrite()\n");
        OS_REPORT("ERROR: offset: %08x low:%08x high:%08x\n", offset, 0, (field_0x1c + field_0xc) * 2);
        disablePort();
        return 0;
    }

#if VERSION < VERSION_SHIELD_DEBUG
    while (HIOWrite(offset + field_0x20, buffer, size) == 0 && attempts < 30) {
        attempts++;
    }
#else
    while (HIO2Write(JHIGetHIO2Handle(), offset + field_0x20, buffer, size) == 0 && attempts < 30) {
        attempts++;
    }
#endif

    if (attempts >= 30) {
        JHIReport("ERROR: Communication Error. -HIOWrite()\n");
        disablePort();
        return 0;
    }

    return 1;
}

int JHIMccBuf::JHIMCCRead(u32 channel, u32 offset, void* buffer, u32 size) {
    JHIReport("DEBUG:      JHIMCCRead(%d, %06x, %08x, %d);\n", channel, offset, buffer, size);
    if (!JHIRead(channel, offset, buffer, size)) {
        return 0;
    }

    return 1;
}

int JHIMccBuf::JHIMCCWrite(u32 channel, u32 offset, void* buffer, u32 size) {
    JHIReport("DEBUG:      JHIMCCWrite(%d, %06x, %08x, %d);\n", channel, offset, buffer, size);
    if (!JHIWrite(channel, offset, buffer, size)) {
        return 0;
    }

    return 1;
}

JHIMccBufReader::JHIMccBufReader(u16 channel, u16 param_1, u32 param_2)
    : JHIMccBuf(channel, param_1, param_2) {}

JHIMccBufReader::~JHIMccBufReader() {}

u32 JHIMccBufReader::available() {
    u32 var_r30 = 0;

    if (!isPort()) {
        return 0;
    }

    if (!readData(mReadOffset + field_0x8, &mBeginPos)) {
        return 0;
    }

    if (!readData(mReadOffset + field_0xa, &mEndPos)) {
        return 0;
    }

    if (mBeginPos < mEndPos) {
        var_r30 = mEndPos - mBeginPos;
    } else if (mBeginPos > mEndPos) {
        var_r30 = field_0x1c - (mBeginPos - mEndPos);
    }

    return var_r30;
}

int JHIMccBufReader::readBegin() {
    if (!isPort()) {
        return 0;
    }

    if (!readData(mReadOffset + field_0x8, &mBeginPos)) {
        return 0;
    }

    if (!readData(mReadOffset + field_0xa, &mEndPos)) {
        return 0;
    }

    JHIReport("DEBUG: readBegin: R:%08x W:%08x\n", mBeginPos, mEndPos);
    return 1;
}

int JHIMccBufReader::read(void* pBuf) {
    if (!isPort()) {
        return 0;
    }

    u32 size;
    int var_r28 = 0;

    if (mBeginPos < mEndPos) {
        size = mEndPos - mBeginPos;
        if (!JHIMCCRead(mChannel, mReadOffset + mBeginPos, mTempBuf, size)) {
            return 0;
        }
        
        var_r28 = readDataFromBuf(pBuf, size);
    } else if (mBeginPos > mEndPos) {
        size = field_0x1c - (mBeginPos - mEndPos);
        u32 var_r25 = (field_0xc + field_0x1c);
        u32 var_r27 = var_r25 - mBeginPos;
        u32 var_r26 = size - var_r27;

        if (!JHIMCCRead(mChannel, mReadOffset + mBeginPos, mTempBuf, var_r27)) {
            return 0;
        } else if (var_r26 != 0) {
            if (!JHIMCCRead(mChannel, mReadOffset + field_0xc, mTempBuf + var_r27, var_r26)) {
                return 0;
            }
        }

        var_r28 = readDataFromBuf(pBuf, size);
    }

    return var_r28;
}

void JHIMccBufReader::readEnd() {
    if (isPort()) {
        setHeaderToBuf(mEndPos, 0, 0x20);

        JHIReport("DEBUG: readEnd: Point:%08x\n", mEndPos);
        
        if (!JHIMCCWrite(mChannel, mReadOffset + field_0x8, mTempBuf, 0x20)) {
            return;
        }
    }
}

JHIMccBufWriter::JHIMccBufWriter(u16 channel, u16 param_1, u32 param_2)
    : JHIMccBuf(channel, param_1, param_2) {}

JHIMccBufWriter::~JHIMccBufWriter() {}

int JHIMccBufWriter::writeBegin() {
    int var_r29 = 0;

    if (!isPort()) {
        return 0;
    }

    if (!readData(mWriteOffset + field_0x8, &mBeginPos)) {
        return 0;
    }

    if (!readData(mWriteOffset + field_0xa, &mEndPos)) {
        return 0;
    }

    u32 var_r30;
    if (mBeginPos == field_0xc) {
        var_r30 = field_0x1c + field_0xc;
        var_r30 -= 0x20;
    } else {
        var_r30 = mBeginPos - 0x20;
    }

    if (mEndPos > var_r30) {
        var_r29 = (field_0x1c - (mEndPos - var_r30)) - 0x20;
    } else if (mEndPos < var_r30) {
        var_r29 = (var_r30 - mEndPos) - 0x20;
    }

    JHIReport("DEBUG: writeBegin: R:%08x W:%08x\n", mBeginPos, mEndPos);
    return var_r29;
}

int JHIMccBufWriter::write(void* pBuffer, u32 size) {
    if (!isPort()) {
        return 0;
    }

    u32 var_r26;
    u32 var_r27;
    u32 var_r28;

    // Probably fake match
    var_r28 = var_r28;

    u32 var_r29 = setDataToBuf(pBuffer, size);
    if (mBeginPos == field_0xc) {
        var_r27 = field_0x1c + field_0xc;
        var_r27 -= 0x20;
    } else {
        var_r27 = mBeginPos - 0x20;
    }

    if (mEndPos > var_r27) {
        int x;
        var_r26 = field_0x1c - (mEndPos - var_r27);
        var_r28 = (field_0x1c + field_0xc) - mEndPos;

        if (var_r28 >= var_r29) {
            if (!JHIMCCWrite(mChannel, mWriteOffset + mEndPos, mTempBuf, var_r29)) {
                return 0;
            }

            mEndPos += var_r29;
        } else {
            if (!JHIMCCWrite(mChannel, mWriteOffset + mEndPos, mTempBuf, var_r28)) {
                return 0;
            } else if (var_r29 - var_r28 != 0) {
                if (!JHIMCCWrite(mChannel, mWriteOffset + field_0xc, mTempBuf + var_r28, var_r29 - var_r28)) {
                    return 0;
                }
            }

            mEndPos = field_0xc + (var_r29 - var_r28);
        }
    } else if (mEndPos < var_r27) {
        var_r26 = var_r27 - mEndPos;

        if (!JHIMCCWrite(mChannel, mWriteOffset + mEndPos, mTempBuf, var_r29)) {
            return 0;
        }

        mEndPos += var_r29;
    }

    if (mEndPos == field_0xc + field_0x1c) {
        mEndPos = field_0xc;
    }

    return var_r29;
}

void JHIMccBufWriter::writeEnd() {
    if (isPort()) {
        setHeaderToBuf(mEndPos, 0, 0x20);

        JHIReport("DEBUG: writeEnd: Point:%08x\n", mEndPos);
        
        if (!JHIMCCWrite(mChannel, mWriteOffset + field_0xa, mTempBuf, 0x20)) {
            return;
        }
    }
}
