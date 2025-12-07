#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JKernel/JKRMemArchive.h"
#include "JSystem/JKernel/JKRDecomp.h"
#include "JSystem/JKernel/JKRDvdRipper.h"
#include "JSystem/JUtility/JUTAssert.h"
#include "JSystem/JUtility/JUTException.h"
#include "string.h"
#include "global.h"
#include <stdint.h>

JKRMemArchive::JKRMemArchive(s32 entryNum, JKRArchive::EMountDirection mountDirection)
    : JKRArchive(entryNum, MOUNT_MEM) {
    mIsMounted = false;
    mMountDirection = mountDirection;
    if (!open(entryNum, mMountDirection)) {
        return;
    }

    mVolumeType = 'RARC';
    mVolumeName = mStringTable + mNodes->name_offset;

    getVolumeList().prepend(&mFileLoaderLink);
    mIsMounted = true;
}

JKRMemArchive::JKRMemArchive(void* buffer, u32 bufferSize, JKRMemBreakFlag param_3)
    : JKRArchive((s32)buffer, MOUNT_MEM) {
    mIsMounted = false;
    if (!open(buffer, bufferSize, param_3)) {
        return;
    }

    mVolumeType = 'RARC';
    mVolumeName = mStringTable + mNodes->name_offset;

    getVolumeList().prepend(&mFileLoaderLink);
    mIsMounted = true;
}

JKRMemArchive::~JKRMemArchive() {
    if (mIsMounted == true) {
        if (mIsOpen) {
            if (mArcHeader)
                JKRFreeToHeap(mHeap, mArcHeader);
        }

        getVolumeList().remove(&mFileLoaderLink);
        mIsMounted = false;
    }
}

bool JKRMemArchive::open(s32 entryNum, JKRArchive::EMountDirection mountDirection) {
    mArcHeader = NULL;
    mArcInfoBlock = NULL;
    mArchiveData = NULL;
    mNodes = NULL;
    mFiles = NULL;
    mStringTable = NULL;
    mIsOpen = false;
    mMountDirection = mountDirection;

    if (mMountDirection == JKRArchive::MOUNT_DIRECTION_HEAD) {
        u32 loadedSize;
        mArcHeader = (SArcHeader *)JKRDvdToMainRam(
            entryNum, NULL, EXPAND_SWITCH_UNKNOWN1, 0, mHeap, JKRDvdRipper::ALLOC_DIRECTION_FORWARD,
            0, (int *)&mCompression, &loadedSize);
        if (mArcHeader) {
            DCInvalidateRange(mArcHeader, loadedSize);
        }
    }
    else {
        u32 loadedSize;
        mArcHeader = (SArcHeader *)JKRDvdToMainRam(
            entryNum, NULL, EXPAND_SWITCH_UNKNOWN1, 0, mHeap,
            JKRDvdRipper::ALLOC_DIRECTION_BACKWARD, 0, (int *)&mCompression, &loadedSize);
        if (mArcHeader) {
            DCInvalidateRange(mArcHeader, loadedSize);
        }
    }

    if (!mArcHeader) {
        mMountMode = UNKNOWN_MOUNT_MODE;
    }
    else {
        JUT_ASSERT(438, mArcHeader->signature =='RARC');
        mArcInfoBlock = (SArcDataInfo *)((u8 *)mArcHeader + mArcHeader->header_length);
        mNodes = (SDIDirEntry *)((u8 *)&mArcInfoBlock->num_nodes + mArcInfoBlock->node_offset);
        mFiles = (SDIFileEntry *)((u8 *)&mArcInfoBlock->num_nodes + mArcInfoBlock->file_entry_offset);
        mStringTable = (char *)((u8 *)&mArcInfoBlock->num_nodes + mArcInfoBlock->string_table_offset);

        mArchiveData =
            (u8 *)((uintptr_t)mArcHeader + mArcHeader->header_length + mArcHeader->file_data_offset);
        mIsOpen = true;
    }

#if DEBUG
    if (mMountMode == 0) {
        OSReport(":::Cannot alloc memory [%s][%d]\n", __FILE__, 460);
    }
#endif

    return (mMountMode == UNKNOWN_MOUNT_MODE) ? false : true;
}

bool JKRMemArchive::open(void* buffer, u32 bufferSize, JKRMemBreakFlag flag) {
    mArcHeader = (SArcHeader *)buffer;
    JUT_ASSERT(491, mArcHeader->signature =='RARC');
    mArcInfoBlock = (SArcDataInfo *)((u8 *)mArcHeader + mArcHeader->header_length);
    mNodes = (SDIDirEntry *)((u8 *)&mArcInfoBlock->num_nodes + mArcInfoBlock->node_offset);
    mFiles = (SDIFileEntry *)((u8 *)&mArcInfoBlock->num_nodes + mArcInfoBlock->file_entry_offset);
    mStringTable = (char *)((u8 *)&mArcInfoBlock->num_nodes + mArcInfoBlock->string_table_offset);
    mArchiveData = (u8 *)(((uintptr_t)mArcHeader + mArcHeader->header_length) + mArcHeader->file_data_offset);
    mIsOpen = (flag == JKRMEMBREAK_FLAG_UNKNOWN1) ? true : false; // mIsOpen might be u8
    mHeap = JKRHeap::findFromRoot(buffer);
    mCompression = COMPRESSION_NONE;
    return true;
}

void* JKRMemArchive::fetchResource(SDIFileEntry* fileEntry, u32* resourceSize) {
    JUT_ASSERT(555, isMounted());
    if (!fileEntry->data) {
        fileEntry->data = mArchiveData + fileEntry->data_offset;
    }

    if (resourceSize) {
        *resourceSize = fileEntry->data_size;
    }

    return fileEntry->data;
}

void* JKRMemArchive::fetchResource(void* buffer, u32 bufferSize, SDIFileEntry* fileEntry,
                                   u32* resourceSize) {
    JUT_ASSERT(595, isMounted());
    u32 srcLength = fileEntry->data_size;
    if (srcLength > bufferSize) {
        srcLength = bufferSize;
    }

    if (fileEntry->data != NULL) {
        memcpy(buffer, fileEntry->data, srcLength);
    } else {
        JKRCompression compression = JKRConvertAttrToCompressionType(fileEntry->getAttr());
        void* data = mArchiveData + fileEntry->data_offset;
        srcLength =
            fetchResource_subroutine((u8*)data, srcLength, (u8*)buffer, bufferSize, compression);
    }

    if (resourceSize) {
        *resourceSize = srcLength;
    }

    return buffer;
}

void JKRMemArchive::removeResourceAll(void) {
    JUT_ASSERT(642, isMounted());

    if (mArcInfoBlock == NULL)
        return;
    if (mMountMode == MOUNT_MEM)
        return;

    // !@bug: looping over file entries without incrementing the fileEntry pointer. Thus, only the
    // first fileEntry will clear/remove the resource data.
    SDIFileEntry* fileEntry = mFiles;
    for (int i = 0; i < mArcInfoBlock->num_file_entries; i++) {
        if (fileEntry->data) {
            fileEntry->data = NULL;
        }
    }
}

bool JKRMemArchive::removeResource(void* resource) {
    JUT_ASSERT(673, isMounted());

    SDIFileEntry* fileEntry = findPtrResource(resource);
    if (!fileEntry)
        return false;

    fileEntry->data = NULL;
    return true;
}

u32 JKRMemArchive::fetchResource_subroutine(u8* src, u32 srcLength, u8* dst, u32 dstLength,
                                            JKRCompression compression) {
    switch (compression) {
    case COMPRESSION_NONE:
        if (srcLength > dstLength) {
            srcLength = dstLength;
        }

        memcpy(dst, src, srcLength);
        return srcLength;

    case COMPRESSION_YAY0:
    case COMPRESSION_YAZ0: {
        u32 expendedSize = JKRDecompExpandSize(src);
        #if VERSION != VERSION_SHIELD_DEBUG
        srcLength = expendedSize;
        #endif
        if (expendedSize > dstLength) {
            srcLength = dstLength;
        }

        JKRDecompress(src, dst, srcLength, 0);
        return srcLength;
    }

    default: {
        JUTException::panic(__FILE__, 723, "??? bad sequence\n");
    } break;
    }

    return 0;
}

u32 JKRMemArchive::getExpandedResSize(const void* resource) const {
    SDIFileEntry* fileEntry = findPtrResource(resource);
    if (fileEntry == NULL)
        return -1;

    if (fileEntry->isCompressed() == false) {
        return getResSize(resource);
    } else {
        return JKRDecompExpandSize((u8*)resource);
    }
}
