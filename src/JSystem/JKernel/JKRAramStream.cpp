#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JKernel/JKRAramStream.h"
#include "JSystem/JKernel/JKRAramPiece.h"
#include "JSystem/JSupport/JSUFileStream.h"
#include "JSystem/JUtility/JUTException.h"
#include <stdint.h>

#if PLATFORM_GCN
const u32 stack_size = 0xc00;
#else
const u32 stack_size = 0x4000;
#endif

JKRAramStream* JKRAramStream::sAramStreamObject;

JKRAramStream* JKRAramStream::create(s32 priority) {
    if (!sAramStreamObject) {
        sAramStreamObject = new (JKRGetSystemHeap(), 0) JKRAramStream(priority);
        JKRResetAramTransferBuffer();
    }

    return sAramStreamObject;
}

void* JKRAramStream::sMessageBuffer[4] = {
    NULL,
    NULL,
    NULL,
    NULL,
};

OSMessageQueue JKRAramStream::sMessageQueue = {0};

JKRAramStream::JKRAramStream(s32 priority) : JKRThread(stack_size, 0x10, priority) {
    resume();
}

JKRAramStream::~JKRAramStream() {}

void* JKRAramStream::run() {
    OSInitMessageQueue(&sMessageQueue, sMessageBuffer, ARRAY_SIZEU(sMessageBuffer));

    for (;;) {
        OSMessage message;
        OSReceiveMessage(&sMessageQueue, &message, OS_MESSAGE_BLOCK);
        JKRAramStreamCommand* command = (JKRAramStreamCommand*)message;

        switch (command->mType) {
        case JKRAramStreamCommand::READ:
            readFromAram();
            break;
        case JKRAramStreamCommand::WRITE:
            writeToAram(command);
            break;
        }
    }
}

s32 JKRAramStream::readFromAram() {
    return 1;
}

s32 JKRAramStream::writeToAram(JKRAramStreamCommand* command) {
    u32 dstSize = command->mSize;
    u32 offset = command->mOffset;
    u32 writtenLength = 0;
    u32 destination = command->mAddress;
    u8* buffer = command->mTransferBuffer;
    u32 bufferSize = command->mTransferBufferSize;
    JKRHeap* heap = command->mHeap;

    if (buffer) {
        bufferSize = bufferSize == 0 ? 0x8000 : bufferSize;

        command->mTransferBufferSize = bufferSize;
        command->mAllocatedTransferBuffer = false;
    } else {
        bufferSize = bufferSize == 0 ? 0x8000 : bufferSize;

        if (heap) {
            buffer = (u8*)JKRAllocFromHeap(heap, bufferSize, -0x20);
            command->mTransferBuffer = buffer;
        } else {
            buffer = (u8*)JKRAllocFromSysHeap(bufferSize, -0x20);
            command->mTransferBuffer = buffer;
        }

        command->mTransferBufferSize = bufferSize;
        command->mAllocatedTransferBuffer = true;
    }

    if (!buffer) {
        if (!heap) {
            JKRGetCurrentHeap()->dump();
        } else {
            heap->dump();
        }

        JUTException::panic(__FILE__, 172, ":::Cannot alloc memory\n");
    }

    if (buffer) {
        command->mStream->seek(offset, JSUStreamSeekFrom_SET);
        while (dstSize != 0) {
            u32 length = (dstSize > bufferSize) ? bufferSize : dstSize;

            s32 readLength = command->mStream->read(buffer, length);
            if (readLength == 0) {
                writtenLength = 0;
                break;
            }

            JKRAramPcs(0, (uintptr_t)buffer, destination, length, NULL);
            dstSize -= length;
            offset += length;
            writtenLength += length;
            destination += length;

            if (command->mReturnSize) {
                *command->mReturnSize += length;
            }
        }

        if (command->mAllocatedTransferBuffer) {
            JKRFree(buffer);
            command->mAllocatedTransferBuffer = false;
        }
    }

    OSSendMessage(&command->mMessageQueue, (OSMessage)writtenLength, OS_MESSAGE_NOBLOCK);
    return writtenLength;
}

u8* JKRAramStream::transBuffer;

u32 JKRAramStream::transSize;

JKRHeap* JKRAramStream::transHeap;

JKRAramStreamCommand* JKRAramStream::write_StreamToAram_Async(JSUFileInputStream* stream, u32 addr,
                                                              u32 size, u32 offset,
                                                              u32* returnSize) {
    JKRAramStreamCommand* command = new (JKRGetSystemHeap(), -4) JKRAramStreamCommand();
    command->mType = JKRAramStreamCommand::WRITE;
    command->mAddress = addr;
    command->mSize = size;
    command->mStream = stream;
    command->field_0x2c = 0;
    command->mOffset = offset;
    command->mTransferBuffer = transBuffer;
    command->mHeap = transHeap;
    command->mTransferBufferSize = transSize;
    command->mReturnSize = returnSize;
    if (returnSize) {
        *returnSize = 0;
    }

    OSInitMessageQueue(&command->mMessageQueue, &command->mMessage, 1);
    OSSendMessage(&sMessageQueue, command, OS_MESSAGE_BLOCK);
    return command;
}

JKRAramStreamCommand* JKRAramStream::sync(JKRAramStreamCommand* command, BOOL isNonBlocking) {
    OSMessage message;
    if (isNonBlocking == 0) {
        OSReceiveMessage(&command->mMessageQueue, &message, OS_MESSAGE_BLOCK);
        if (message == NULL) {
            return NULL;
        } else {
            return command;
        }
    } else {
        if (OSReceiveMessage(&command->mMessageQueue, &message, OS_MESSAGE_NOBLOCK) == FALSE) {
            return NULL;
        } else if (message == NULL) {
            return NULL;
        } else {
            return command;
        }
    }
}

void JKRAramStream::setTransBuffer(u8* buffer, u32 bufferSize, JKRHeap* heap) {
    transBuffer = NULL;
    transSize = 0x8000;
    transHeap = NULL;

    if (buffer) {
        transBuffer = (u8*)ALIGN_NEXT((uintptr_t)buffer, 0x20);
    }

    if (bufferSize) {
        transSize = ALIGN_PREV(bufferSize, 0x20);
    }

    if (heap && !buffer) {
        transHeap = heap;
    }
}

JKRAramStreamCommand::JKRAramStreamCommand() {
    mAllocatedTransferBuffer = false;
}

static void dummy(JSURandomInputStream* stream) {
    stream->getAvailable();
}
