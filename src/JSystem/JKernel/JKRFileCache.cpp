#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JKernel/JKRFileCache.h"
#include "JSystem/JKernel/JKRDvdFile.h"
#include "JSystem/JKernel/JKRFileFinder.h"
#include "JSystem/JKernel/JKRHeap.h"
#include "JSystem/JUtility/JUTAssert.h"

#include <cctype>
#include <string>
#include "global.h"

JKRFileCache* JKRFileCache::mount(const char* path, JKRHeap* heap, const char* param_3) {
    if (!path || *path != '/') {
        return NULL;
    }

    u32 pathLength = strlen(path);
    if (pathLength != 1 && path[pathLength - 1] == '/') {
        return NULL;
    }

    for (JSUListIterator<JKRFileLoader> iterator = sVolumeList.getFirst(); iterator != sVolumeList.getEnd(); ++iterator) {
        if (iterator->getVolumeType() == 'CASH') {
            JKRFileCache* fileCache = (JKRFileCache*)iterator.operator->();
            if (fileCache->mRootPath) {
                if (strcmp(fileCache->mRootPath, path) == 0) {
                    fileCache->mMountCount++;
                    return fileCache;
                }
            }
        }
    }


    JKRFileCache* fileCache = new (heap, 0) JKRFileCache(path, param_3);
    return fileCache;
}

JKRFileCache::JKRFileCache(const char* path, const char* volume) {
    mParentHeap = JKRHeap::findFromRoot(this);
    mMountCount = 1;
    mVolumeType = 'CASH';

    u32 pathLength = strlen(path);
    mRootPath = (char*)JKRAllocFromHeap(mParentHeap, pathLength + 1, 1);
    mCurrentPath = (char*)JKRAllocFromSysHeap(pathLength + 2, 1);
    strcpy(mRootPath, path);
    strcpy(mCurrentPath, path);

    if (path[1] != '\0') {
        convStrLower(mRootPath);
        convStrLower(mCurrentPath);
        strcat(mCurrentPath, "/");

        const char* volumePath = volume;
        if (!volumePath) {
            volumePath = strrchr(mRootPath, '/') + 1;
        }

        mVolumePath = (char*)JKRAllocFromSysHeap(strlen(volumePath) + 1, 0);
        strcpy(mVolumePath, volumePath);
        convStrLower(mVolumePath);
        mVolumeName = mVolumePath;
    } else {
        const char* volumePath = volume;
        if (!volumePath) {
            volumePath = "dvd";
        }

        mVolumePath = (char*)JKRAllocFromSysHeap(strlen(volumePath) + 1, 0);
        strcpy(mVolumePath, volumePath);
        convStrLower(mVolumePath);
        mVolumeName = mVolumePath;
    }

    sVolumeList.prepend(&mFileLoaderLink);
    mIsMounted = true;
}

JKRFileCache::~JKRFileCache() {
    removeResourceAll();
    if (mRootPath)
        JKRFreeToHeap(mParentHeap, mRootPath);
    if (mCurrentPath)
        JKRFreeToSysHeap(mCurrentPath);
    if (mVolumePath)
        JKRFreeToSysHeap(mVolumePath);

    sVolumeList.remove(&mFileLoaderLink);
}

bool JKRFileCache::becomeCurrent(const char* path) {
    char* dvdPathName = getDvdPathName(path);

    bool result = DVDChangeDir(dvdPathName);
    if (result) {
        sCurrentVolume = this;
        JKRFreeToSysHeap(mCurrentPath);
        mCurrentPath = dvdPathName;
        if (mCurrentPath[1] != '\0') {
            strcat(mCurrentPath, "/");
        }
    } else {
        JKRFreeToSysHeap(dvdPathName);
    }

    return result;
}

void* JKRFileCache::getResource(const char* path) {
    JUT_ASSERT(237, isMounted());

    void* buffer = NULL;
    char* name = getDvdPathName(path);

    JKRDvdFile dvdFile(name);
    if (dvdFile.isAvailable()) {
        CCacheBlock* cacheBlock = findCacheBlock(dvdFile.getFileID());
        if (!cacheBlock) {
            u32 alignedSize = ALIGN_NEXT(dvdFile.getFileInfo()->length, 0x20);
            buffer = JKRAllocFromHeap(mParentHeap, alignedSize, 0x20);
            if (buffer) {
                dvdFile.read(buffer, alignedSize, 0);

                cacheBlock = new (JKRGetSystemHeap(), 0)
                    CCacheBlock(dvdFile.getFileID(), dvdFile.getFileInfo()->length, buffer);
                mCacheBlockList.append(&cacheBlock->mCacheBlockLink);
            }
        } else {
            cacheBlock->mReferenceCount++;
            buffer = cacheBlock->mMemoryPtr;
        }
    }

    JKRFreeToSysHeap(name);
    return buffer;
}

void* JKRFileCache::getResource(u32, const char* path) {
    JUT_ASSERT(303, isMounted());

    char finalPath[256];
    char* filePath = finalPath + strlen(mRootPath);
    strcpy(finalPath, mRootPath);

    if (findFile(finalPath, path)) {
        return getResource(filePath);
    }
    return NULL;
}

u32 JKRFileCache::readResource(void* dst, u32 dstLength, const char* path) {
    JUT_ASSERT(344, isMounted());

    char* name = getDvdPathName(path);
    JKRDvdFile dvdFile(name);

    u32 resourceSize = 0;

// !@bug: (maybe?) Infinite Loop: Because dvdFile.isAvailable() is never updated in the loop-body
// will would never exit the loop.
    while (true) {
        if (!dvdFile.isAvailable()) {
            break;
        }
        resourceSize = ALIGN_NEXT(dvdFile.getFileInfo()->length, 0x20);
        dstLength = ALIGN_PREV(dstLength, 0x20);
        if (resourceSize > dstLength) {
            resourceSize = dstLength;
        }

        CCacheBlock* cacheBlock = findCacheBlock(dvdFile.getFileID());
        if (!cacheBlock) {
            dvdFile.read(dst, resourceSize, 0);
        } else {
            memcpy(dst, cacheBlock->mMemoryPtr, resourceSize);
        }
    }

    JKRFreeToSysHeap(name);
    return resourceSize;
}

u32 JKRFileCache::readResource(void* dst, u32 dstLength, u32, const char* path) {
    JUT_ASSERT(412, isMounted());

    char finalPath[256];
    char* filePath = finalPath + strlen(mRootPath);
    strcpy(finalPath, mRootPath);

    if (findFile(finalPath, path)) {
        return readResource(dst, dstLength, filePath);
    }
    return NULL;
}

void JKRFileCache::removeResourceAll(void) {
    JUT_ASSERT(441, isMounted());

    JSUListIterator<CCacheBlock> iterator = mCacheBlockList.getFirst();
    while (iterator != mCacheBlockList.getEnd()) {
        JKRFreeToHeap(mParentHeap, iterator->mMemoryPtr);
        mCacheBlockList.remove(&iterator.getObject()->mCacheBlockLink);
        delete (iterator++).getObject();
    }
}

bool JKRFileCache::removeResource(void* resource) {
    JUT_ASSERT(463, isMounted());

    CCacheBlock* cacheBlock = findCacheBlock(resource);
    if (!cacheBlock)
        return false;

    if (--cacheBlock->mReferenceCount == 0) {
        JKRFreeToHeap(mParentHeap, resource);
        mCacheBlockList.remove(&cacheBlock->mCacheBlockLink);
        delete cacheBlock;
    }

    return true;
}

bool JKRFileCache::detachResource(void* resource) {
    JUT_ASSERT(490, isMounted());

    CCacheBlock* cacheBlock = findCacheBlock(resource);
    if (!cacheBlock)
        return false;

    mCacheBlockList.remove(&cacheBlock->mCacheBlockLink);
    delete cacheBlock;
    return true;
}

u32 JKRFileCache::getResSize(const void* resource) const {
    CCacheBlock* cacheBlock = findCacheBlock(resource);
    if (cacheBlock == NULL) {
        return -1;
    } else {
        return cacheBlock->mFileSize;
    }
}

u32 JKRFileCache::countFile(const char* path) const {
    u32 count = 0;
    char* name = getDvdPathName(path);
    DVDDir dir;
    if (DVDOpenDir(name, &dir) != 0) {
        DVDDirEntry dirEntry;
        while (DVDReadDir(&dir, &dirEntry)) {
            count = count + 1;
        }

        DVDCloseDir(&dir);
    }

    JKRFreeToSysHeap(name);
    return count;
}

JKRFileFinder* JKRFileCache::getFirstFile(const char* path) const {
    char* name = getDvdPathName(path);
    JKRDvdFinder* finder = new (JKRGetSystemHeap(), 0) JKRDvdFinder(name);
    JKRFreeToSysHeap(name);

    if (finder->isAvailable() != true) {
        delete finder;
        return NULL;
    }

    return finder;
}

JKRFileCache::CCacheBlock* JKRFileCache::findCacheBlock(const void* resource) const {
    for (JSUListIterator<CCacheBlock> iterator = mCacheBlockList.getFirst(); iterator != mCacheBlockList.getEnd(); ++iterator) {
        if (iterator->mMemoryPtr == resource) {
            return iterator.getObject();
        }
    }

    return NULL;
}

JKRFileCache::CCacheBlock* JKRFileCache::findCacheBlock(u32 fileID) const {
    for (JSUListIterator<CCacheBlock> iterator = mCacheBlockList.getFirst(); iterator != mCacheBlockList.getEnd(); ++iterator) {
        if (iterator->mFileId == fileID) {
            return iterator.getObject();
        }
    }

    return NULL;
}

bool JKRFileCache::findFile(char* path, const char* fileName) const {
    bool result = false;
    u32 pathLength = strlen(path);
    DVDDir dir;
    if (DVDOpenDir(path, &dir)) {
        DVDDirEntry dirEntry;
        while (DVDReadDir(&dir, &dirEntry)) {
            if (dirEntry.isDir) {
                path[pathLength] = '/';
                strcpy(path + pathLength + 1, dirEntry.name);
                result = findFile(path, fileName);
                if (result)
                    break;
                path[pathLength] = '\0';
            } else {
                result = (strcmp(fileName, dirEntry.name) == 0);
                if (result) {
                    strcat(path, "/");
                    strcat(path, fileName);
                    break;
                }
            }
        }

        DVDCloseDir(&dir);
    }

    return result;
}

char* JKRFileCache::getDvdPathName(const char* path) const {
    char* newPath;
    if (path[0] == '/') {
        newPath = (char*)JKRAllocFromSysHeap(strlen(mRootPath) + strlen(path) + 2, 1);
        strcpy(newPath, mRootPath);
        if (path[1]) {
            if (mRootPath[1] == 0) {
                strcat(newPath, path + 1);
            } else {
                strcat(newPath, path);
            }
        }
    } else {
        newPath = (char*)JKRAllocFromSysHeap(strlen(mCurrentPath) + strlen(path) + 2, 1);
        strcpy(newPath, mCurrentPath);
        strcat(newPath, path);
    }

    convStrLower(newPath);
    return newPath;
}

void JKRFileCache::convStrLower(char* buffer) const {
    while (*buffer) {
        *buffer++ = tolower(*buffer);
    }
}

JKRFileCache::CCacheBlock::CCacheBlock(u32 fileId, u32 fileSize, const void* resource)
    : mCacheBlockLink(this) {
    mReferenceCount = 1;
    mFileId = fileId;
    mFileSize = fileSize;
    mMemoryPtr = (void*)resource;  // todo: don't cast away const
}

void* JKRFileCache::getFsResource(const char* path) {
    return getResource(path);
}

void* JKRFileCache::getNameResource(u32 type, const char* path) {
    return getResource(type, path);
}

u32 JKRFileCache::readFsResource(void* dst, u32 dstLength, const char* path) {
    return readResource(dst, dstLength, path);
}

u32 JKRFileCache::readNameResource(void* dst, u32 dstLength, u32 type, const char* path) {
    return readResource(dst, dstLength, type, path);
}
