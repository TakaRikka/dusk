#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JKernel/JKRAramArchive.h"
#include "JSystem/JKernel/JKRArchive.h"
#include "JSystem/JKernel/JKRCompArchive.h"
#include "JSystem/JKernel/JKRDvdArchive.h"
#include "JSystem/JKernel/JKRFileFinder.h"
#include "JSystem/JKernel/JKRHeap.h"
#include "JSystem/JKernel/JKRMemArchive.h"
#include "JSystem/JUtility/JUTAssert.h"

JKRArchive* JKRArchive::check_mount_already(s32 entryNum, JKRHeap* heap) {
    if (heap == NULL) {
        heap = JKRGetCurrentHeap();
    }

    for (JSUListIterator<JKRFileLoader> iterator = sVolumeList.getFirst(); iterator != sVolumeList.getEnd(); ++iterator) {
        if (iterator->getVolumeType() == 'RARC') {
            JKRArchive* archive = (JKRArchive*)iterator.operator->();
            if (archive->mEntryNum == entryNum && archive->mHeap == heap) {
                archive->mMountCount++;
                return archive;
            }
        }
    }

    return NULL;
}

JKRArchive* JKRArchive::mount(const char* path, EMountMode mountMode, JKRHeap* heap,
                              EMountDirection mountDirection) {
    s32 entryNum = DVDConvertPathToEntrynum(path);
    if (entryNum < 0)
        return NULL;

    return mount(entryNum, mountMode, heap, mountDirection);
}

JKRArchive* JKRArchive::mount(void* ptr, JKRHeap* heap,
                              EMountDirection mountDirection) {
    JKRArchive* archive = check_mount_already((s32)ptr, heap);
    if (archive) {
        return archive;
    }

    int alignment = mountDirection == MOUNT_DIRECTION_HEAD ? 4 : -4;

    JKRArchive* newArchive = new (heap, alignment) JKRMemArchive(ptr, 0xFFFF, JKRMEMBREAK_FLAG_UNKNOWN0);
    return newArchive;
}

JKRArchive* JKRArchive::mount(s32 entryNum, JKRArchive::EMountMode mountMode, JKRHeap* heap,
                              JKRArchive::EMountDirection mountDirection) {
    JKRArchive* archive = check_mount_already(entryNum, heap);
    if (archive != NULL) {
        return archive;
    }
    int alignment = mountDirection == MOUNT_DIRECTION_HEAD ? 4 : -4;

    JKRArchive* newArchive;
    switch (mountMode) {
    case JKRArchive::MOUNT_MEM:
        newArchive = new (heap, alignment) JKRMemArchive(entryNum, mountDirection);
        break;
    case JKRArchive::MOUNT_ARAM:
        newArchive = new (heap, alignment) JKRAramArchive(entryNum, mountDirection);
        break;
    case JKRArchive::MOUNT_DVD:
        newArchive = new (heap, alignment) JKRDvdArchive(entryNum, mountDirection);
        break;
    case JKRArchive::MOUNT_COMP:
        newArchive = new (heap, alignment) JKRCompArchive(entryNum, mountDirection);
        break;
    }

    if (newArchive && newArchive->mMountMode == JKRArchive::UNKNOWN_MOUNT_MODE) {
        delete newArchive;
        newArchive = NULL;
    }

    return newArchive;
}

bool JKRArchive::becomeCurrent(const char* path) {
    SDIDirEntry* dirEntry = NULL;
    if (*path == '/') {
        path++;

        if (*path == '\0')
            path = NULL;
        dirEntry = findDirectory(path, 0);
    } else {
        dirEntry = findDirectory(path, sCurrentDirID);
    }

    bool found = dirEntry != NULL;
    if (found) {
        sCurrentVolume = this;
        sCurrentDirID = dirEntry - mNodes;
    }

    return found;
}

bool JKRArchive::getDirEntry(SDirEntry* dirEntry, u32 index) const {
    SDIFileEntry* fileEntry = findIdxResource(index);
    if (!fileEntry)
        return false;

    dirEntry->flags = fileEntry->type_flags_and_name_offset >> 24;
    dirEntry->id = fileEntry->file_id;
    dirEntry->name = mStringTable + (fileEntry->type_flags_and_name_offset & 0xFFFFFF);
    return true;
}

void* JKRArchive::getGlbResource(u32 param_1, const char* path, JKRArchive* archive) {
    void* resource = NULL;
    if (archive) {
        return archive->getResource(param_1, path);
    }

    for (JSUListIterator<JKRFileLoader> iterator = sVolumeList.getFirst(); iterator != sVolumeList.getEnd(); ++iterator) {
        if (iterator->getVolumeType() == 'RARC') {
            JKRFileLoader* fileLoader = iterator.operator->();
            resource = fileLoader->getResource(param_1, path);
            if (resource) {
                break;
            }
        }
    }

    return resource;
}

void* JKRArchive::getResource(const char* path) {
    JUT_ASSERT(303, isMounted());
    SDIFileEntry* fileEntry = NULL;
    if (*path == '/') {
        fileEntry = findFsResource(path + 1, 0);
    } else {
        fileEntry = findFsResource(path, sCurrentDirID);
    }

    if (fileEntry) {
        return fetchResource(fileEntry, NULL);
    }

    return NULL;
}

void* JKRArchive::getResource(u32 type, const char* path) {
    JUT_ASSERT(347, isMounted());
    SDIFileEntry* fileEntry;
    if (type == 0 || type == '????') {
        fileEntry = findNameResource(path);
    } else {
        fileEntry = findTypeResource(type, path);
    }

    if (fileEntry) {
        return fetchResource(fileEntry, NULL);
    }

    return NULL;
}

void* JKRArchive::getIdxResource(u32 index) {
    JUT_ASSERT(384, isMounted());
    SDIFileEntry* fileEntry = findIdxResource(index);
    if (fileEntry) {
        return fetchResource(fileEntry, NULL);
    }

    return NULL;
}

void* JKRArchive::getResource(u16 id) {
    JUT_ASSERT(409, isMounted());
    SDIFileEntry* fileEntry = findIdResource(id);
    if (fileEntry) {
        return fetchResource(fileEntry, NULL);
    }

    return NULL;
}

u32 JKRArchive::readResource(void* buffer, u32 bufferSize, u32 type, const char* path) {
    JUT_ASSERT(493, isMounted());
    SDIFileEntry* fileEntry;
    if (type == 0 || type == '????') {
        fileEntry = findNameResource(path);
    } else {
        fileEntry = findTypeResource(type, path);
    }

    if (fileEntry) {
        u32 resourceSize;
        fetchResource(buffer, bufferSize, fileEntry, &resourceSize);
        return resourceSize;
    }

    return 0;
}

u32 JKRArchive::readResource(void* buffer, u32 bufferSize, const char* path) {
    JUT_ASSERT(539, isMounted());
    SDIFileEntry* fileEntry = NULL;
    if (*path == '/') {
        fileEntry = findFsResource(path + 1, 0);
    } else {
        fileEntry = findFsResource(path, sCurrentDirID);
    }

    if (fileEntry) {
        u32 resourceSize;
        fetchResource(buffer, bufferSize, fileEntry, &resourceSize);
        return resourceSize;
    }

    return 0;
}

u32 JKRArchive::readIdxResource(void* buffer, u32 bufferSize, u32 index) {
    JUT_ASSERT(593, isMounted());
    SDIFileEntry* fileEntry = findIdxResource(index);
    if (fileEntry) {
        u32 resourceSize;
        fetchResource(buffer, bufferSize, fileEntry, &resourceSize);
        return resourceSize;
    }

    return 0;
}

u32 JKRArchive::readResource(void* buffer, u32 bufferSize, u16 id) {
    JUT_ASSERT(625, isMounted());
    SDIFileEntry* fileEntry = findIdResource(id);
    if (fileEntry) {
        u32 resourceSize;
        fetchResource(buffer, bufferSize, fileEntry, &resourceSize);
        return resourceSize;
    }

    return 0;
}

void JKRArchive::removeResourceAll() {
    if (mArcInfoBlock && mMountMode != MOUNT_MEM) {
        SDIFileEntry* fileEntry = mFiles;
        for (int i = 0; i < mArcInfoBlock->num_file_entries; i++) {
            if (fileEntry->data) {
                JKRFreeToHeap(mHeap, fileEntry->data);
                fileEntry->data = NULL;
            }
            fileEntry++;
        }
    }
}

bool JKRArchive::removeResource(void* resource) {
    JUT_ASSERT(678, resource != NULL);
    SDIFileEntry* fileEntry = findPtrResource(resource);
    if (fileEntry == NULL)
        return false;

    fileEntry->data = NULL;
    JKRFreeToHeap(mHeap, resource);
    return true;
}

bool JKRArchive::detachResource(void* resource) {
    JUT_ASSERT(707, resource != NULL);
    SDIFileEntry* fileEntry = findPtrResource(resource);
    if (fileEntry == NULL)
        return false;

    fileEntry->data = NULL;
    return true;
}

u32 JKRArchive::getResSize(const void* resource) const {
    JUT_ASSERT(732, resource != NULL);
    SDIFileEntry* fileEntry = findPtrResource(resource);
    if (fileEntry == NULL)
        return -1;

    return fileEntry->data_size;
}

u32 JKRArchive::countResource() const {
    u32 count = 0;
    for (int i = 0; i < mArcInfoBlock->num_file_entries; i++) {
        if ((mFiles[i].type_flags_and_name_offset >> 24) & 1) {
            count++;
        }
    }
    return count;
}

u32 JKRArchive::countFile(const char* path) const {
    SDIDirEntry* dirEntry = NULL;
    if (*path == '/') {
        path++;

        if (*path == '\0')
            path = NULL;
        dirEntry = findDirectory(path, 0);
    } else {
        dirEntry = findDirectory(path, sCurrentDirID);
    }

    if (dirEntry) {
        return dirEntry->num_entries;
    }

    return 0;
}

JKRFileFinder* JKRArchive::getFirstFile(const char* path) const {
    SDIDirEntry* dirEntry = NULL;
    if (*path == '/') {
        path++;

        if (*path == '\0')
            path = NULL;
        dirEntry = findDirectory(path, 0);
    } else {
        dirEntry = findDirectory(path, sCurrentDirID);
    }

    if (dirEntry) {
        // don't know what is correct here... for now we're casting away const
        return new (JKRGetSystemHeap(), 0)
            JKRArcFinder((JKRArchive*)this, dirEntry->first_file_index, (u32)dirEntry->num_entries);
    }

    return NULL;
}

u32 JKRArchive::getFileAttribute(u32 index) const {
    SDIFileEntry* fileEntry = findIdxResource(index);
    if (fileEntry) {
        return u8(fileEntry->type_flags_and_name_offset >> 24);
    }

    return 0;
}
