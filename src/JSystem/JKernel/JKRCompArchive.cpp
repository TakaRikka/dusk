#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JKernel/JKRCompArchive.h"
#include "JSystem/JKernel/JKRAram.h"
#include "JSystem/JKernel/JKRAramArchive.h"
#include "JSystem/JKernel/JKRDecomp.h"
#include "JSystem/JKernel/JKRDvdAramRipper.h"
#include "JSystem/JKernel/JKRDvdArchive.h"
#include "JSystem/JKernel/JKRDvdFile.h"
#include "JSystem/JKernel/JKRMemArchive.h"
#include "JSystem/JUtility/JUTAssert.h"
#include "JSystem/JUtility/JUTException.h"
#include "math.h"
#include "string.h"
#include <stdint.h>

JKRCompArchive::JKRCompArchive(s32 entryNum, JKRArchive::EMountDirection eMountDirection)
    : JKRArchive(entryNum, MOUNT_COMP) {
    mMountDirection = eMountDirection;

    if (!open(entryNum)) {
        return;
    }

    mVolumeType = 'RARC';
    mVolumeName = &mStringTable[mNodes->name_offset];
    sVolumeList.prepend(&mFileLoaderLink);
    mIsMounted = true;
}

JKRCompArchive::~JKRCompArchive() {
    if (mArcInfoBlock != NULL) {
        SDIFileEntry* file = mFiles;
        for (int i = 0; i < mArcInfoBlock->num_file_entries; i++) {
            if (!((file->type_flags_and_name_offset >> 0x18) & 0x10) && file->data != NULL) {
                JKRFreeToHeap(mHeap, file->data);
            }

            file++;
        }

        JKRFreeToHeap(mHeap, mArcInfoBlock);
        mArcInfoBlock = NULL;
    }

    if (mAramPart != NULL) {
        delete mAramPart;
    }

    if (mExpandedSize != NULL) {
        JKRFree(mExpandedSize);
        mExpandedSize = NULL;
    }

    if (mDvdFile != NULL) {
        delete mDvdFile;
    }

    sVolumeList.remove(&mFileLoaderLink);
    mIsMounted = false;
}

bool JKRCompArchive::open(s32 entryNum) {
    mArcInfoBlock = NULL;
    field_0x64 = 0;
    mAramPart = NULL;
    field_0x6c = 0;
    mSizeOfMemPart = 0;
    mSizeOfAramPart = 0;
    field_0x7c = 0;
    mNodes = NULL;
    mFiles = NULL;
    mStringTable = NULL;

    mDvdFile = new (JKRGetSystemHeap(), 0) JKRDvdFile(entryNum);
    if(mDvdFile == NULL) {
        mMountMode = 0;
        return 0;
    }
    SArcHeader *arcHeader = (SArcHeader *)JKRAllocFromSysHeap(sizeof(SArcHeader), -32); // NOTE: unconfirmed if this struct is used
    if(arcHeader == NULL) {
        mMountMode = 0;
    }
    else {
        int alignment;

        JKRDvdToMainRam(entryNum, (u8 *)arcHeader, EXPAND_SWITCH_UNKNOWN1, 32, NULL, JKRDvdRipper::ALLOC_DIRECTION_FORWARD, 0, &mCompression, NULL);
        DCInvalidateRange(arcHeader, 32);

        mSizeOfMemPart = arcHeader->field_0x14;
        mSizeOfAramPart = arcHeader->field_0x18;
        JUT_ASSERT(352, ( mSizeOfMemPart & 0x1f ) == 0);
        JUT_ASSERT(353, ( mSizeOfAramPart & 0x1f ) == 0);

        switch (mCompression)
        {
        case COMPRESSION_NONE:
        case COMPRESSION_YAZ0:
            alignment = mMountDirection == 1 ? 32 : -32;
            mArcInfoBlock = (SArcDataInfo *)JKRAllocFromHeap(mHeap, arcHeader->file_data_offset + mSizeOfMemPart, alignment);
            if (mArcInfoBlock == NULL) {
                mMountMode = 0;
            }
            else
            {
                JKRDvdToMainRam(entryNum, (u8 *)mArcInfoBlock, EXPAND_SWITCH_UNKNOWN1, (uintptr_t)arcHeader->file_data_offset + mSizeOfMemPart,
                                NULL, JKRDvdRipper::ALLOC_DIRECTION_FORWARD, 0x20, NULL, NULL);
                DCInvalidateRange(mArcInfoBlock, (uintptr_t)arcHeader->file_data_offset + mSizeOfMemPart);
                field_0x64 = (uintptr_t)mArcInfoBlock + arcHeader->file_data_offset;

                if (mSizeOfAramPart != 0) {
                    mAramPart = (JKRAramBlock*)JKRAllocFromAram(mSizeOfAramPart, JKRAramHeap::HEAD);
                    if(mAramPart == NULL) {
                        mMountMode = 0;
                        break;
                    }

                    JKRDvdToAram(entryNum, mAramPart->getAddress(), EXPAND_SWITCH_UNKNOWN1, arcHeader->header_length + arcHeader->file_data_offset + mSizeOfMemPart, 0, NULL);
                }

                mNodes = (SDIDirEntry*)((uintptr_t)mArcInfoBlock + mArcInfoBlock->node_offset);
                mFiles = (SDIFileEntry *)((uintptr_t)mArcInfoBlock + mArcInfoBlock->file_entry_offset);
                mStringTable = (char*)((uintptr_t)mArcInfoBlock + mArcInfoBlock->string_table_offset);
                field_0x6c = arcHeader->header_length + arcHeader->file_data_offset;
            }
            break;

        case COMPRESSION_YAY0:
            u32 alignedSize = ALIGN_NEXT(mDvdFile->getFileSize(), 32);
            alignment = ((mMountDirection == 1) ? 32 : -32);
            u8 *buf = (u8 *)JKRAllocFromSysHeap(alignedSize, -alignment);

            if(buf == NULL) {
                mMountMode = 0;
            }
            else {
                JKRDvdToMainRam(entryNum, buf, EXPAND_SWITCH_UNKNOWN2, alignedSize, NULL, JKRDvdRipper::ALLOC_DIRECTION_FORWARD, 0, NULL, NULL);
                DCInvalidateRange(buf, alignedSize);
                u32 expandSize = ALIGN_NEXT(JKRDecompExpandSize(buf), 32);
                u8 *mem = (u8 *)JKRAllocFromHeap(mHeap, expandSize, -alignment);

                if(mem == NULL) {
                    mMountMode = 0;
                }
                else {
                    arcHeader = (SArcHeader *)mem;
                    JKRDecompress((u8 *)buf, (u8 *)mem, expandSize, 0);
                    JKRFreeToSysHeap(buf);

                    mArcInfoBlock = (SArcDataInfo *)JKRAllocFromHeap(mHeap, arcHeader->file_data_offset + mSizeOfMemPart, alignment);
                    if(mArcInfoBlock == NULL) {
                        mMountMode = 0;
                    }
                    else {
                        // arcHeader + 1 should lead to 0x20, which is the data after the header
                        JKRHeap::copyMemory((u8 *)mArcInfoBlock, arcHeader + 1, (arcHeader->file_data_offset + mSizeOfMemPart));
                        field_0x64 = (uintptr_t)mArcInfoBlock + arcHeader->file_data_offset;
                        if (mSizeOfAramPart != 0) {
                            mAramPart = (JKRAramBlock*)JKRAllocFromAram(mSizeOfAramPart, JKRAramHeap::HEAD);
                            if(mAramPart == NULL) {
                                mMountMode = 0;
                            }
                            else {
                                JKRMainRamToAram((u8 *)mem + arcHeader->header_length + arcHeader->file_data_offset + mSizeOfMemPart,
                                                 mAramPart->getAddress(), mSizeOfAramPart, EXPAND_SWITCH_UNKNOWN0, 0, NULL, -1, NULL);
                            }
                        }
                    }
                }
            }
            mNodes = (SDIDirEntry *)((uintptr_t)mArcInfoBlock + mArcInfoBlock->node_offset);
            mFiles = (SDIFileEntry *)((uintptr_t)mArcInfoBlock + mArcInfoBlock->file_entry_offset);
            mStringTable = (char *)((uintptr_t)mArcInfoBlock + mArcInfoBlock->string_table_offset);
            field_0x6c = arcHeader->header_length + arcHeader->file_data_offset;
            break;
        }
        mExpandedSize = NULL;
        u8 compressedFiles = 0;
        SDIFileEntry *fileEntry = mFiles;
        for (int i = 0; i < mArcInfoBlock->num_file_entries; i++)
        {
            u8 flag = fileEntry->type_flags_and_name_offset >> 0x18;
            if (((flag & 0x1) != 0) && (((flag)&0x10) == 0))
            {
                compressedFiles = compressedFiles | (flag & 4);
            }
            fileEntry++;
        }

        if (compressedFiles != 0)
        {
            mExpandedSize = (s32 *)JKRAllocFromHeap(mHeap, mArcInfoBlock->num_file_entries * 4, abs(alignment));
            if (mExpandedSize == NULL)
            {
                JKRFreeToSysHeap(mArcInfoBlock);
                mMountMode = 0;
            }
            else
            {
                memset(mExpandedSize, 0, mArcInfoBlock->num_file_entries * 4);
            }
        }
    }

    if (arcHeader != NULL)
    {
        JKRFreeToSysHeap(arcHeader);
    }
    if (mMountMode == 0) {
        OS_REPORT(":::[%s: %d] Cannot alloc memory in mounting CompArchive\n", __FILE__, 567);
        if(mDvdFile != NULL) {
            delete mDvdFile;
        }
        return false;
    }
    return true;
}


void* JKRCompArchive::fetchResource(SDIFileEntry *fileEntry, u32 *pSize) {
    JUT_ASSERT(597, isMounted());
    u32 ptrSize;
    u32 size = fileEntry->data_size;
    int compression = JKRConvertAttrToCompressionType(fileEntry->type_flags_and_name_offset >> 0x18);

    if(pSize == NULL) {
        pSize = &ptrSize; // this makes barely any sense but ok
    }

    if (fileEntry->data == NULL) {
        u32 flag = fileEntry->type_flags_and_name_offset >> 0x18;
        if(flag & 0x10) {
            fileEntry->data = (void *)(field_0x64 + fileEntry->data_offset);
            *pSize = size;
        }
        else if (flag & 0x20) {
            u8 *data;
            *pSize = JKRAramArchive::fetchResource_subroutine(fileEntry->data_offset + mAramPart->getAddress() - mSizeOfMemPart, size, mHeap, compression, &data);
            fileEntry->data = data;
            if(compression == COMPRESSION_YAZ0) {
                setExpandSize(fileEntry, *pSize);
            }
        }
        else if (flag & 0x40) {
            u8 *data;
            u32 resSize = JKRDvdArchive::fetchResource_subroutine(mEntryNum, field_0x6c + fileEntry->data_offset, fileEntry->data_size, mHeap, compression, mCompression, &data);
            if (pSize != NULL) {
                *pSize = resSize;
            }
            fileEntry->data = data;
            if (compression == COMPRESSION_YAZ0) {
                setExpandSize(fileEntry, *pSize);
            }
        }
    }
    else  {
        if (pSize != NULL) {
            *pSize = fileEntry->data_size;
        }
    }
    return fileEntry->data;
}


void *JKRCompArchive::fetchResource(void *data, u32 compressedSize, SDIFileEntry *fileEntry, u32 *pSize)
{
    u32 size = 0;
    JUT_ASSERT(708, isMounted());
    u32 fileSize = fileEntry->data_size;
    u32 alignedSize = ALIGN_NEXT(fileSize, 32);
    u32 fileFlag = fileEntry->type_flags_and_name_offset >> 0x18;
    int compression = JKRConvertAttrToCompressionType(fileFlag);

    if(fileEntry->data != NULL) {
        if (compression == COMPRESSION_YAZ0) {
            u32 expandSize = getExpandSize(fileEntry);
            if (expandSize != 0) {
                fileSize = expandSize;
            }
        }

        if (fileSize > compressedSize) {
            fileSize = compressedSize;
        }

        JKRHeap::copyMemory(data, fileEntry->data, fileSize);
        size = fileSize;
        }
    else {
        if (fileFlag & 0x10) {
            size = JKRMemArchive::fetchResource_subroutine((u8 *)(field_0x64 + fileEntry->data_offset), alignedSize, (u8 *)data,
                                                           compressedSize & ~31, compression);
        }
        else if (fileFlag & 0x20) {
            size = JKRAramArchive::fetchResource_subroutine(fileEntry->data_offset + mAramPart->getAddress() - mSizeOfMemPart,
                                                            alignedSize, (u8 *)data, compressedSize & ~31, compression);
        }
        else if (fileFlag & 0x40){
            size = JKRDvdArchive::fetchResource_subroutine(mEntryNum, field_0x6c + fileEntry->data_offset, alignedSize, (u8 *)data,
                                                           compressedSize & ~31, compression, mCompression);
        } else {
            JUTException::panic(__FILE__, 776, "illegal archive.");
        }
    }

    if(pSize != NULL) {
        *pSize = size;
    }
    return data;
}

void JKRCompArchive::removeResourceAll() {
    if (mArcInfoBlock != NULL && mMountMode != MOUNT_MEM) {
        SDIFileEntry* fileEntry = mFiles;
        for (int i = 0; i < mArcInfoBlock->num_file_entries; i++) {
            int tmp = fileEntry->type_flags_and_name_offset >> 0x18;

            if (fileEntry->data != NULL) {
                if (!(tmp & 0x10)) {
                    JKRFreeToHeap(mHeap, fileEntry->data);
                }

                fileEntry->data = NULL;
            }
        }
    }
}

bool JKRCompArchive::removeResource(void* resource) {
    SDIFileEntry* fileEntry = findPtrResource(resource);
    if (!fileEntry)
        return false;

    if (!((fileEntry->type_flags_and_name_offset >> 0x18) & 0x10)) {
        JKRFreeToHeap(mHeap, resource);
    }

    fileEntry->data = NULL;
    return true;
}


u32 JKRCompArchive::getExpandedResSize(const void *resource) const
{
    if (mExpandedSize == NULL) {
        return getResSize(resource);
    }

    SDIFileEntry *fileEntry = findPtrResource(resource);
    if(!fileEntry) {
        return 0xffffffff;
    }

    u8 flags = (fileEntry->type_flags_and_name_offset >> 0x18);
    if((flags & 4) == 0) { // not compressed
        return getResSize(resource);
    }

    if ((flags & 0x10) != 0) {
        return JKRDecompExpandSize((u8 *)resource);
    }

    u8 buf[64];
    u8 *bufPtr = (u8 *)ALIGN_NEXT((uintptr_t)buf, 32);
    if ((flags & 0x20) != 0) {
        u32 addr = mAramPart->mAddress;
        addr = fileEntry->data_offset + addr;
        JKRAramToMainRam(addr, bufPtr, sizeof(buf) / 2, EXPAND_SWITCH_UNKNOWN0, 0, NULL, -1, NULL);
        DCInvalidateRange(bufPtr, sizeof(buf) / 2);
    }
    else if ((flags & 0x40) != 0) {
        JKRDvdToMainRam(mEntryNum, bufPtr, EXPAND_SWITCH_UNKNOWN2, sizeof(buf) / 2, NULL, JKRDvdRipper::ALLOC_DIRECTION_FORWARD, field_0x6c + fileEntry->data_offset, NULL, NULL);
        DCInvalidateRange(bufPtr, sizeof(buf) / 2);
    }
    else {
        JUTException::panic(__FILE__, 943, "illegal resource.");
    }
    u32 expandSize = JKRDecompExpandSize(bufPtr);
    const_cast<JKRCompArchive *>(this)->setExpandSize(fileEntry, expandSize);
    return expandSize;
}
