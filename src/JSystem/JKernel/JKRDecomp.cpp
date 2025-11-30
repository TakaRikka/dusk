#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JKernel/JKRDecomp.h"
#include "JSystem/JKernel/JKRAramPiece.h"
#include "global.h"

JKRDecomp* JKRDecomp::sDecompObject;

JKRDecomp* JKRDecomp::create(s32 priority) {
    if (!sDecompObject) {
        sDecompObject = new (JKRHeap::getSystemHeap(), 0) JKRDecomp(priority);
    }

    return sDecompObject;
}

OSMessage JKRDecomp::sMessageBuffer[8] = {0};

OSMessageQueue JKRDecomp::sMessageQueue = {0};

JKRDecomp::JKRDecomp(s32 priority) : JKRThread(0x800, 0x10, priority) {
    resume();
}

JKRDecomp::~JKRDecomp() {}

void* JKRDecomp::run() {
    OSInitMessageQueue(&sMessageQueue, sMessageBuffer, 8);
    for (;;) {
        OSMessage message;
        OSReceiveMessage(&sMessageQueue, &message, OS_MESSAGE_BLOCK);

        JKRDecompCommand* command = (JKRDecompCommand*)message;
        decode(command->mSrcBuffer, command->mDstBuffer, command->mSrcLength, command->mDstLength);

        if (command->field_0x20 != 0) {
            if (command->field_0x20 == 1) {
                JKRAramPiece::sendCommand(command->mAMCommand);
            }
            continue;
        }

        if (command->mCallback) {
            (*command->mCallback)((u32)command);
            continue;
        }

        if (command->field_0x1c) {
            OSSendMessage(command->field_0x1c, (OSMessage)1, OS_MESSAGE_NOBLOCK);
        } else {
            OSSendMessage(&command->mMessageQueue, (OSMessage)1, OS_MESSAGE_NOBLOCK);
        }
    }
}

JKRDecompCommand* JKRDecomp::prepareCommand(u8* srcBuffer, u8* dstBuffer, u32 srcLength,
                                            u32 dstLength,
                                            JKRDecompCommand::AsyncCallback callback) {
    JKRDecompCommand* command = new (JKRHeap::getSystemHeap(), -4) JKRDecompCommand();
    command->mSrcBuffer = srcBuffer;
    command->mDstBuffer = dstBuffer;
    command->mSrcLength = srcLength;
    command->mDstLength = dstLength;
    command->mCallback = callback;
    return command;
}

void JKRDecomp::sendCommand(JKRDecompCommand* command) {
    OSSendMessage(&sMessageQueue, command, OS_MESSAGE_NOBLOCK);
}

JKRDecompCommand* JKRDecomp::orderAsync(u8* srcBuffer, u8* dstBuffer, u32 srcLength, u32 dstLength,
                                        JKRDecompCommand::AsyncCallback callback) {
    JKRDecompCommand* command =
        prepareCommand(srcBuffer, dstBuffer, srcLength, dstLength, callback);
    sendCommand(command);
    return command;
}

bool JKRDecomp::sync(JKRDecompCommand* command, int isNonBlocking) {
    OSMessage message;
    bool result;
    if (isNonBlocking == JKRDECOMP_SYNC_BLOCKING) {
        OSReceiveMessage(&command->mMessageQueue, &message, OS_MESSAGE_BLOCK);
        result = true;
    } else {
        result =
            OSReceiveMessage(&command->mMessageQueue, &message, OS_MESSAGE_NOBLOCK) != FALSE;
    }

    return result;
}

bool JKRDecomp::orderSync(u8* srcBuffer, u8* dstBuffer, u32 srcLength, u32 dstLength) {
    JKRDecompCommand* command = orderAsync(srcBuffer, dstBuffer, srcLength, dstLength, NULL);
    bool result = sync(command, JKRDECOMP_SYNC_BLOCKING);
    delete command;
    return result;
}

void JKRDecomp::decode(u8* srcBuffer, u8* dstBuffer, u32 srcLength, u32 dstLength) {
    JKRCompression compression = checkCompressed(srcBuffer);
    if (compression == COMPRESSION_YAY0) {
        decodeSZP(srcBuffer, dstBuffer, srcLength, dstLength);
    } else if (compression == COMPRESSION_YAZ0) {
        decodeSZS(srcBuffer, dstBuffer, srcLength, dstLength);
    }
}

void JKRDecomp::decodeSZP(u8* src, u8* dst, u32 srcLength, u32 dstLength) {
    int srcChunkOffset;
    int count;
    int dstOffset;
    u32 length = srcLength;
    int linkInfo;
    int offset;
    int i;

    int decodedSize = READU32_BE(src, 4);
    int linkTableOffset = READU32_BE(src, 8);
    int srcDataOffset = READU32_BE(src, 12);

    dstOffset = 0;
    u32 counter = 0;
    srcChunkOffset = 16;

    u32 chunkBits;
    if (srcLength == 0)
        return;
    if (dstLength > decodedSize)
        return;

    do
    {
        if (counter == 0)
        {
            chunkBits = READU32_BE(src, srcChunkOffset);
            srcChunkOffset += sizeof(u32);
            counter = sizeof(u32) * 8;
        }

        if (chunkBits & 0x80000000)
        {
            if (dstLength == 0)
            {
                dst[dstOffset] = src[srcDataOffset];
                length--;
                if (length == 0)
                    return;
            }
            else
            {
                dstLength--;
            }
            dstOffset++;
            srcDataOffset++;
        }
        else
        {
            linkInfo = src[linkTableOffset] << 8 | src[linkTableOffset + 1];
            linkTableOffset += sizeof(u16);

            offset = dstOffset - (linkInfo & 0xFFF);
            count = (linkInfo >> 12);
            if (count == 0)
            {
                count = (u32)src[srcDataOffset++] + 0x12;
            }
            else
                count += 2;

            if (count > decodedSize - dstOffset)
                count = decodedSize - dstOffset;

            for (i = 0; i < count; i++, dstOffset++, offset++)
            {
                if (dstLength == 0)
                {
                    dst[dstOffset] = dst[offset - 1];
                    length--;
                    if (length == 0)
                        return;
                }
                else
                    dstLength--;
            }
        }

        chunkBits <<= 1;
        counter--;
    } while (dstOffset < decodedSize);
}

void JKRDecomp::decodeSZS(u8* src_buffer, u8* dst_buffer, u32 srcSize, u32 dstSize) {
    u8* decompEnd;
    u8* copyStart;
    s32 copyByteCount;
    s32 chunkBitsLeft = 0;
    s32 chunkBits;

    decompEnd = dst_buffer + *(int*)(src_buffer + 4) - dstSize;

    if (srcSize == 0) {
        return;
    }
    if (dstSize > *(u32*)src_buffer) {
        return;
    }

    u8* curSrcPos = src_buffer + 0x10;
    do {
        if (chunkBitsLeft == 0) {
            chunkBits = curSrcPos[0];
            chunkBitsLeft = 8;
            curSrcPos++;
        }
        if ((chunkBits & 0x80) != 0) {
            if (dstSize == 0) {
                dst_buffer[0] = curSrcPos[0];
                srcSize--;
                dst_buffer++;
                if (srcSize == 0)
                    return;
            } else {
                dstSize--;
            }
            curSrcPos++;
        } else {
            u32 local_28 = curSrcPos[1] | (curSrcPos[0] & 0xF) << 8;
            u32 r31 = curSrcPos[0] >> 4;
            curSrcPos += 2;
            copyStart = dst_buffer - local_28;
            if (r31 == 0) {
                copyByteCount = curSrcPos[0] + 0x12;
                curSrcPos++;
            } else {
                copyByteCount = r31 + 2;
            }
            do {
                if (dstSize == 0) {
                    dst_buffer[0] = copyStart[-1];
                    srcSize--;
                    dst_buffer++;
                    if (srcSize == 0)
                        return;
                } else {
                    dstSize--;
                }
                copyByteCount--;
                copyStart++;
            } while (copyByteCount != 0);
        }
        chunkBits <<= 1;
        chunkBitsLeft--;
    } while (dst_buffer != decompEnd);
}

JKRCompression JKRDecomp::checkCompressed(u8* src) {
    if ((src[0] == 'Y') && (src[1] == 'a') && (src[3] == '0')) {
        if (src[2] == 'y') {
            return COMPRESSION_YAY0;
        }

        if (src[2] == 'z') {
            return COMPRESSION_YAZ0;
        }
    }

    if ((src[0] == 'A') && (src[1] == 'S') && (src[2] == 'R')) {
        return COMPRESSION_ASR;
    }

    return COMPRESSION_NONE;
}

JKRDecompCommand::JKRDecompCommand() {
    OSInitMessageQueue(&mMessageQueue, &mMessage, 1);
    mCallback = NULL;
    field_0x1c = NULL;
    mThis = this;
    field_0x20 = 0;
}

JKRDecompCommand::~JKRDecompCommand() {}
