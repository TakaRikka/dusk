#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JKernel/JKRAram.h"
#include "JSystem/JKernel/JKRAramPiece.h"
#include "JSystem/JKernel/JKRAramStream.h"
#include "JSystem/JKernel/JKRDecomp.h"
#include "JSystem/JKernel/JKRExpHeap.h"
#include "JSystem/JUtility/JUTException.h"
#include "dolphin/ar.h"
#include <dolphin/os.h>
#include "string.h"

static u8* firstSrcData();
static u8* nextSrcData(u8* param_0);
static int JKRDecompressFromAramToMainRam(u32 src, void* dst, u32 srcLength, u32 dstLength,
                                          u32 offset, u32* resourceSize);
int decompSZS_subroutine(u8* src, u8* dest);

JKRAram* JKRAram::sAramObject;

JKRAram* JKRAram::create(u32 aram_audio_buffer_size, u32 aram_audio_graph_size, s32 stream_priority,
                         s32 decomp_priority, s32 piece_priority) {
    if (!sAramObject) {
        sAramObject = new (JKRHeap::getSystemHeap(), 0)
            JKRAram(aram_audio_buffer_size, aram_audio_graph_size, piece_priority);
    }

    JKRCreateAramStreamManager(stream_priority);
    JKRCreateDecompManager(decomp_priority);
    sAramObject->resume();
    return sAramObject;
}

OSMessage JKRAram::sMessageBuffer[4] = {
    NULL,
    NULL,
    NULL,
    NULL,
};

OSMessageQueue JKRAram::sMessageQueue = {0};

JKRAram::JKRAram(u32 audio_buffer_size, u32 audio_graph_size, s32 priority)
    : JKRThread(0xC00, 0x10, priority) {
    u32 aramBase = ARInit(mStackArray, ARRAY_SIZEU(mStackArray));
    ARQInit();

    u32 aramSize = ARGetSize();

    mAudioMemorySize = audio_buffer_size;
    if (audio_graph_size == 0xFFFFFFFF) {
        mGraphMemorySize = (aramSize - audio_buffer_size) - aramBase;
        mAramMemorySize = 0;
    } else {
        mGraphMemorySize = audio_graph_size;
        mAramMemorySize = (aramSize - (audio_buffer_size + audio_graph_size)) - aramBase;
    }

    mAudioMemoryPtr = ARAlloc(mAudioMemorySize);
    mGraphMemoryPtr = ARAlloc(mGraphMemorySize);

    if (mAramMemorySize) {
        mAramMemoryPtr = ARAlloc(mAramMemorySize);
    } else {
        mAramMemoryPtr = NULL;
    }

    mAramHeap = new (JKRHeap::getSystemHeap(), 0) JKRAramHeap(mGraphMemoryPtr, mGraphMemorySize);
}

JKRAram::~JKRAram() {
    sAramObject = NULL;
    if (mAramHeap)
        delete mAramHeap;
}

void* JKRAram::run(void) {
    int result;
    JKRAMCommand* command;
    JKRAramPiece::Message* message;
    OSInitMessageQueue(&sMessageQueue, sMessageBuffer, 4);
    do {
        OSReceiveMessage(&sMessageQueue, (OSMessage*)&message, OS_MESSAGE_BLOCK);
        result = message->field_0x00;
        command = message->command;
        delete message;

        switch (result) {
        case 1:
            JKRAramPiece::startDMA(command);
            break;
        }
    } while (true);
}

void JKRAram::checkOkAddress(u8* addr, u32 size, JKRAramBlock* block, u32 param_4) {
    if (!IS_ALIGNED((uintptr_t)addr, 0x20) && !IS_ALIGNED(size, 0x20)) {
        JUTException::panic(__FILE__, 219, ":::address not 32Byte aligned.");
    }

    if (block && !IS_ALIGNED((uintptr_t)block->getAddress() + param_4, 0x20)) {
        JUTException::panic(__FILE__, 227, ":::address not 32Byte aligned.");
    }
}

void JKRAram::changeGroupIdIfNeed(u8* data, int groupId) {
    JKRHeap* currentHeap = JKRHeap::getCurrentHeap();
    if (currentHeap->getHeapType() == 'EXPH' && groupId >= 0) {
        JKRExpHeap::CMemBlock* block = JKRExpHeap::CMemBlock::getBlock(data);
        block->newGroupId(groupId);
    }
}

JKRAramBlock* JKRAram::mainRamToAram(u8* buf, u32 bufSize, u32 alignedSize,
                                     JKRExpandSwitch expandSwitch, u32 fileSize, JKRHeap* heap,
                                     int id, u32* pSize) {
    JKRAramBlock* block = NULL;
    checkOkAddress(buf, bufSize, NULL, 0);
    if (expandSwitch == EXPAND_SWITCH_UNKNOWN1) {
        expandSwitch = (JKRCheckCompressed_noASR(buf) == COMPRESSION_NONE) ?
                           EXPAND_SWITCH_UNKNOWN0 :
                           EXPAND_SWITCH_UNKNOWN1;
    }
    if (expandSwitch == EXPAND_SWITCH_UNKNOWN1) {
        u32 expandSize = JKRDecompExpandSize(buf);
        if (fileSize == 0 || fileSize > expandSize) {
            fileSize = ALIGN_NEXT(expandSize, 32);
        }
        if (bufSize == 0) {
            JKRAramBlock* allocatedBlock =
                (JKRAramBlock*)JKRAllocFromAram(fileSize, JKRAramHeap::HEAD);
            block = (JKRAramBlock*)allocatedBlock;
            if (allocatedBlock == NULL)
                return NULL;

            allocatedBlock->newGroupID(decideAramGroupId(id));
            bufSize = allocatedBlock->getAddress();
        }
        if (alignedSize == 0 || alignedSize > expandSize)
            alignedSize = ALIGN_NEXT(expandSize, 32);

        if (alignedSize > fileSize)
            alignedSize = fileSize;

        void* allocatedMem = JKRAllocFromHeap(heap, fileSize, -32);
        if (allocatedMem == NULL) {
            if (block != NULL) {
                JKRFreeToAram(block);
            }
            block = NULL;
        } else {
            JKRDecompress(buf, (u8*)allocatedMem, fileSize, 0);
            JKRAramPcs(0, (uintptr_t)allocatedMem, bufSize, alignedSize, block);
            JKRFreeToHeap(heap, allocatedMem);
            block = block == NULL ? (JKRAramBlock*)-1 : block;
            if (pSize != NULL) {
                *pSize = alignedSize;
            }
        }
    } else {
        if (fileSize != 0 && alignedSize > fileSize)
            alignedSize = fileSize;
        if (bufSize == 0) {
            JKRAramBlock* allocatedBlock =
                (JKRAramBlock*)JKRAllocFromAram(alignedSize, JKRAramHeap::HEAD);
            block = allocatedBlock;
            block->newGroupID(decideAramGroupId(id));
            if (block == NULL)
                return NULL;

            bufSize = allocatedBlock->getAddress();
        }

        JKRAramPcs(0, (uintptr_t)buf, bufSize, alignedSize, block);
        block = block == NULL ? (JKRAramBlock*)-1 : block;
        if (pSize != NULL)
            *pSize = alignedSize;
    }
    return block;
}

u8* JKRAram::aramToMainRam(u32 address, u8* buf, u32 p3, JKRExpandSwitch expandSwitch, u32 p5,
                           JKRHeap* heap, int id, u32* pSize) {
    JKRCompression compression = COMPRESSION_NONE;
    if (pSize != NULL)
        *pSize = 0;

    checkOkAddress(buf, address, NULL, 0);

    u32 expandSize;
    if (expandSwitch == EXPAND_SWITCH_UNKNOWN1) {
        u8 buffer[64];
        u8* bufPtr = (u8*)ALIGN_NEXT((uintptr_t)buffer, 32);
        JKRAramPcs(1, address, (uintptr_t)bufPtr, sizeof(buffer) / 2,
                   NULL);  // probably change sizeof(buffer) / 2 to 32
        compression = JKRCheckCompressed_noASR(bufPtr);
        expandSize = JKRDecompExpandSize(bufPtr);
    }

    if (compression == COMPRESSION_YAZ0) {  // SZS
        if (p5 != 0 && p5 < expandSize)
            expandSize = p5;

        if (buf == NULL)
            buf = (u8*)JKRAllocFromHeap(heap, expandSize, 32);
        if (buf == NULL)
            return NULL;
        else {
            changeGroupIdIfNeed(buf, id);
            JKRDecompressFromAramToMainRam(address, buf, p3, expandSize, 0, pSize);
            return buf;
        }
    } else if (compression == COMPRESSION_YAY0) {  // SZP
        u8* szpSpace = (u8*)JKRAllocFromHeap(heap, p3, -32);
        if (szpSpace == NULL) {
            return NULL;
        } else {
            JKRAramPcs(1, address, (uintptr_t)szpSpace, p3, NULL);
            if (p5 != 0 && p5 < expandSize)
                expandSize = p5;

            u8* rv;
            if (buf == NULL) {
                rv = (u8*)JKRAllocFromHeap(heap, expandSize, 32);
            } else {
                rv = buf;
            }

            if (rv == NULL) {
                JKRFree(szpSpace);
                return NULL;
            } else {
                changeGroupIdIfNeed(rv, id);
                JKRDecompress(szpSpace, rv, expandSize, 0);
                JKRFreeToHeap(heap, szpSpace);
                if (pSize != NULL) {
                    *pSize = expandSize;
                }
                return rv;
            }
        }
    } else {  // Not compressed or ASR
        if (buf == NULL)
            buf = (u8*)JKRAllocFromHeap(heap, p3, 32);
        if (buf == NULL) {
            return NULL;
        } else {
            changeGroupIdIfNeed(buf, id);
            JKRAramPcs(1, address, (uintptr_t)buf, p3, NULL);
            if (pSize != NULL) {
                *pSize = p3;
            }
            return buf;
        }
    }
}

JSUList<JKRAMCommand> JKRAram::sAramCommandList;

static OSMutex decompMutex;

u32 JKRAram::sSZSBufferSize = 0x00000400;

static u8* szpBuf;

static u8* szpEnd;

static u8* refBuf;

static u8* refEnd;

static u8* refCurrent;

static u32 srcOffset;

static u32 transLeft;

static u8* srcLimit;

static u32 srcAddress;

static u32 fileOffset;

static u32 readCount;

static u32 maxDest;

static bool s_is_decompress_mutex_initialized;

static u32* tsPtr;

static u32 tsArea;

static int JKRDecompressFromAramToMainRam(u32 src, void* dst, u32 srcLength, u32 dstLength,
                                          u32 offset, u32* resourceSize) {
    BOOL interrupts = OSDisableInterrupts();
    if (s_is_decompress_mutex_initialized == false) {
        OSInitMutex(&decompMutex);
        s_is_decompress_mutex_initialized = true;
    }
    OSRestoreInterrupts(interrupts);
    OSLockMutex(&decompMutex);

    u32 szsBufferSize = JKRAram::getSZSBufferSize();
    szpBuf = (u8*)JKRAllocFromSysHeap(szsBufferSize, 32);

    szpEnd = szpBuf + szsBufferSize;
    if (offset != 0) {
        refBuf = (u8*)JKRAllocFromSysHeap(0x1120, 0);
        refEnd = refBuf + 0x1120;
        refCurrent = refBuf;
    } else {
        refBuf = NULL;
    }
    srcAddress = src;
    srcOffset = 0;
    transLeft = (srcLength != 0) ? srcLength : -1;
    fileOffset = offset;
    readCount = 0;
    maxDest = dstLength;
    tsPtr = (resourceSize != 0) ? resourceSize : &tsArea;
    *tsPtr = 0;

    decompSZS_subroutine(firstSrcData(), (u8*)dst);
    JKRFree(szpBuf);
    if (refBuf) {
        JKRFree(refBuf);
    }

    DCStoreRangeNoSync(dst, *tsPtr);
    OSUnlockMutex(&decompMutex);

    return 0;
}

int decompSZS_subroutine(u8* src, u8* dest) {
    u8* endPtr;
    s32 validBitCount = 0;
    s32 currCodeByte = 0;
    u32 ts = 0;

    if (src[0] != 'Y' || src[1] != 'a' || src[2] != 'z' || src[3] != '0') {
        return -1;
    }

    SYaz0Header* header = (SYaz0Header*)src;
    endPtr = dest + (header->length - fileOffset);
    if (endPtr > dest + maxDest) {
        endPtr = dest + maxDest;
    }

    src += 0x10;
    do {
        if (validBitCount == 0) {
            if ((src > srcLimit) && transLeft) {
                src = nextSrcData(src);
            }
            currCodeByte = *src;
            validBitCount = 8;
            src++;
        }
        if (currCodeByte & 0x80) {
            if (fileOffset != 0) {
                if (readCount >= fileOffset) {
                    *dest = *src;
                    dest++;
                    ts++;
                    if (dest == endPtr) {
                        break;
                    }
                }
                *(refCurrent++) = *src;
                if (refCurrent == refEnd) {
                    refCurrent = refBuf;
                }
                src++;
            } else {
                *dest = *src;
                dest++;
                src++;
                ts++;
                if (dest == endPtr) {
                    break;
                }
            }
            readCount++;
        } else {
            u32 dist = ((src[0] & 0x0f) << 8) | src[1];
            s32 numBytes = src[0] >> 4;
            src += 2;
            u8* copySource;
            if (fileOffset != 0) {
                copySource = refCurrent - dist - 1;
                if (copySource < refBuf) {
                    copySource += refEnd - refBuf;
                }
            } else {
                copySource = dest - dist - 1;
            }
            if (numBytes == 0) {
                numBytes = *src + 0x12;
                src += 1;
            } else {
                numBytes += 2;
            }
            if (fileOffset != 0) {
                do {
                    if (readCount >= fileOffset) {
                        *dest = *copySource;
                        dest++;
                        ts++;
                        if (dest == endPtr) {
                            break;
                        }
                    }
                    *(refCurrent++) = *copySource;
                    if (refCurrent == refEnd) {
                        refCurrent = refBuf;
                    }
                    copySource++;
                    if (copySource == refEnd) {
                        copySource = refBuf;
                    }
                    readCount++;
                    numBytes--;
                } while (numBytes != 0);
            } else {
                do {
                    *dest = *copySource;
                    dest++;
                    ts++;
                    if (dest == endPtr) {
                        break;
                    }
                    readCount++;
                    numBytes--;
                    copySource++;
                } while (numBytes != 0);
            }
        }
        currCodeByte <<= 1;
        validBitCount--;
    } while (dest < endPtr);
    *tsPtr = ts;
    return 0;
}

static u8* firstSrcData() {
    srcLimit = szpEnd - 0x19;
    u8* buffer = szpBuf;

    u32 length;
    u32 size = szpEnd - szpBuf;
    if (transLeft < size) {
        length = transLeft;
    } else {
        length = size;
    }

    u32 src = (uintptr_t)(srcAddress + srcOffset);
    u32 dst = (uintptr_t)buffer;
    u32 alignedLength = ALIGN_NEXT(length, 0x20);
    JKRAramPcs(1, src, dst, alignedLength, NULL);

    srcOffset += length;
    transLeft -= length;
    if (!transLeft) {
        srcLimit = buffer + length;
    }

    return buffer;
}

static u8* nextSrcData(u8* current) {
    u8* dest;
    u32 left = (uintptr_t)(szpEnd - current);
    if (IS_NOT_ALIGNED(left, 0x20))
        dest = szpBuf + 0x20 - (left & (0x20 - 1));
    else
        dest = szpBuf;

    memcpy(dest, current, left);
    u32 transSize = (uintptr_t)(szpEnd - (dest + left));
    if (transSize > transLeft)
        transSize = transLeft;

    JKRAramPcs(1, (uintptr_t)(srcAddress + srcOffset), ((uintptr_t)dest + left), ALIGN_NEXT(transSize, 0x20),
               NULL);
    srcOffset += transSize;
    transLeft -= transSize;

    if (transLeft == 0)
        srcLimit = (dest + left) + transSize;

    return dest;
}

static const char* stringBase_8039D0A6 = "bad aramSync\n";
