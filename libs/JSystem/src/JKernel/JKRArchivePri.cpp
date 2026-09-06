#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JKernel/JKRArchive.h"
#include "JSystem/JKernel/JKRHeap.h"
#include <cctype>
#include <cstring>

#if TARGET_PC
#include <cassert>
#include <cstddef>
#include <cstdlib>
#include <limits>
#include <ranges>
#include <string_view>
#include <borealis/log.hpp>
#include "JSystem/JKernel/JKRDvdRipper.h"
#if _WIN32
#include <malloc.h>
#endif

std::atomic<u64> JKRArchive::sArcOverlayGeneration{0};

namespace {

inline constexpr borealis::Log Log{"JKRArchivePri"};

void* alloc_overlay_buffer(u32 size) {
#if _WIN32
    return _aligned_malloc(size, alignof(std::max_align_t));
#else
    return std::malloc(size);
#endif
}

void free_overlay_buffer(void* data) {
#if _WIN32
    _aligned_free(data);
#else
    std::free(data);
#endif
}

}  // namespace
#endif

DUSK_GAME_DATA u32 JKRArchive::sCurrentDirID;

JKRArchive::JKRArchive() {
    mIsMounted = false;
    mMountDirection = MOUNT_DIRECTION_HEAD;
#if TARGET_PC
    mFileData = nullptr;
#endif
}

JKRArchive::JKRArchive(s32 entryNumber, JKRArchive::EMountMode mountMode) {
    mIsMounted = false;
    mMountMode = mountMode;
    mMountCount = 1;
    field_0x58 = 1;
#if TARGET_PC
    mFileData = nullptr;
#endif

    mHeap = JKRHeap::findFromRoot(this);
    if (mHeap == NULL) {
        mHeap = JKRHeap::getCurrentHeap();
    }

    mEntryNum = entryNumber;
    if (sCurrentVolume == NULL) {
        sCurrentVolume = this;
        sCurrentDirID = 0;
    }
}

JKRArchive::~JKRArchive() {
#if TARGET_PC
    removeAllOverlayResources();
    if (mFileData != nullptr) {
        JKRHeap::getSystemHeap()->free(mFileData);
        mFileData = nullptr;
    }
#endif
}

bool JKRArchive::isSameName(JKRArchive::CArcName& name, u32 nameOffset, u16 nameHash) const {
    u16 hash = name.getHash();
    if (hash != nameHash)
        return false;
    return strcmp(mStringTable + nameOffset, name.getString()) == 0;
}

JKRArchive::SDIDirEntry* JKRArchive::findResType(u32 type) const {
    SDIDirEntry* node = mNodes;
    for (u32 count = 0; count < mArcInfoBlock->num_nodes; count++) {
        if (node->type == type) {
            return node;
        }

        node++;
    }

    return NULL;
}

JKRArchive::SDIDirEntry* JKRArchive::findDirectory(const char* name, u32 directoryId) const {
    if (name == NULL) {
        return mNodes + directoryId;
    }

    CArcName arcName(&name, '/');
    SDIDirEntry* dirEntry = mNodes + directoryId;
    SDIFileEntry* fileEntry = mFiles + dirEntry->first_file_index;

    for (int i = 0; i < dirEntry->num_entries; i++) {
        if (isSameName(arcName, fileEntry->type_flags_and_name_offset & 0xFFFFFF, fileEntry->name_hash)) {
            if ((fileEntry->type_flags_and_name_offset >> 24) & 2) {
                return findDirectory(name, fileEntry->data_offset);
            }
            break;
        }
        fileEntry++;
    }

    return NULL;
}

JKRArchive::SDIFileEntry* JKRArchive::findTypeResource(u32 type, const char* name) const {
    if (type) {
        CArcName arcName(name);
        SDIDirEntry* dirEntry = findResType(type);

        if (dirEntry) {
            SDIFileEntry* fileEntry = mFiles + dirEntry->first_file_index;
            for (int i = 0; i < dirEntry->num_entries; i++) {
                if (isSameName(arcName, fileEntry->type_flags_and_name_offset & 0xFFFFFF, fileEntry->name_hash)) {
                    return fileEntry;
                }
                fileEntry++;
            }
        }
    }

    return NULL;
}

JKRArchive::SDIFileEntry* JKRArchive::findFsResource(const char* name, u32 directoryId) const {
    if (name) {
        CArcName arcName(&name, '/');
        SDIDirEntry* dirEntry = mNodes + directoryId;
        SDIFileEntry* fileEntry = mFiles + dirEntry->first_file_index;

        for (int i = 0; i < dirEntry->num_entries; i++) {
            if (isSameName(arcName, fileEntry->type_flags_and_name_offset & 0xFFFFFF, fileEntry->name_hash)) {
                if ((fileEntry->type_flags_and_name_offset >> 24) & 2) {
                    return findFsResource(name, fileEntry->data_offset);
                }

                if (name == NULL) {
                    return fileEntry;
                }

                return NULL;
            }
            fileEntry++;
        }
    }

    return NULL;
}

JKRArchive::SDIFileEntry* JKRArchive::findIdxResource(u32 fileIndex) const {
    if (fileIndex < mArcInfoBlock->num_file_entries) {
        return mFiles + fileIndex;
    }

    return NULL;
}

JKRArchive::SDIFileEntry* JKRArchive::findNameResource(const char* name) const {
    SDIFileEntry* fileEntry = mFiles;

    CArcName arcName(name);
    for (int i = 0; i < mArcInfoBlock->num_file_entries; i++) {
        if (isSameName(arcName, fileEntry->type_flags_and_name_offset & 0xFFFFFF, fileEntry->name_hash)) {
            return fileEntry;
        }
        fileEntry++;
    }

    return NULL;
}

JKRArchive::SDIFileEntry* JKRArchive::findPtrResource(const void* resource) const {
    SDIFileEntry* fileEntry = mFiles;
    for (int i = 0; i < mArcInfoBlock->num_file_entries; i++) {
        if (JKAR_DATA(fileEntry) == resource) {
            return fileEntry;
        }
        fileEntry++;
    }

    return NULL;
}

JKRArchive::SDIFileEntry* JKRArchive::findIdResource(u16 id) const {
    if (id != 0xFFFF) {
        SDIFileEntry* fileEntry;
        if (id < mArcInfoBlock->num_file_entries) {
            fileEntry = mFiles + id;
            if (fileEntry->file_id == id && ((fileEntry->type_flags_and_name_offset >> 24) & 1)) {
                return fileEntry;
            }
        }

        fileEntry = mFiles;
        for (int i = 0; i < mArcInfoBlock->num_file_entries; i++) {
            if (fileEntry->file_id == id && ((fileEntry->type_flags_and_name_offset >> 24) & 1)) {
                return fileEntry;
            }
            fileEntry++;
        }
    }

    return NULL;
}

void JKRArchive::CArcName::store(const char* name) {
    mHash = 0;
    s32 length = 0;
    while (*name) {
        s32 ch = tolower(*name);
        mHash = ch + mHash * 3;
        if (length < ARRAY_SIZE(mData)) {
            mData[length++] = ch;
        }
        name++;
    }

    mLength = (u16)length;
    mData[length] = 0;
}

const char* JKRArchive::CArcName::store(const char* name, char endChar) {
    mHash = 0;
    s32 length = 0;
    while (*name && *name != endChar) {
        s32 lch = tolower((int)*name);
        mHash = lch + mHash * 3;
        if (length < ARRAY_SIZE(mData)) {
            mData[length++] = lch;
        }
        name++;
    }

    mLength = (u16)length;
    mData[length] = 0;

    if (*name == 0)
        return NULL;
    return name + 1;
}

void JKRArchive::setExpandSize(SDIFileEntry* fileEntry, u32 expandSize) {
    int index = fileEntry - mFiles;
    if (!mExpandedSize || index >= mArcInfoBlock->num_file_entries)
        return;

    mExpandedSize[index] = expandSize;
}

u32 JKRArchive::getExpandSize(SDIFileEntry* fileEntry) const {
    int index = fileEntry - mFiles;
    if (!mExpandedSize || index >= mArcInfoBlock->num_file_entries)
        return 0;

    return mExpandedSize[index];
}

#if TARGET_PC
void*& JKRArchive::getFileDataPointer(int idx) const {
    assert(mArcInfoBlock);
    assert(idx < mArcInfoBlock->num_file_entries);
    assert(mFileData);

    return mFileData[idx];
}

void JKRArchive::initFileDataPointers() {
    assert(mArcInfoBlock);
    assert(mFiles);

    if (mFileData != nullptr) {
        JKRHeap::getSystemHeap()->free(mFileData);
        mFileData = nullptr;
    }

    mFileData = static_cast<void**>(
        JKRHeap::getSystemHeap()->alloc(mArcInfoBlock->num_file_entries * sizeof(void*), alignof(void*)));

    memset(mFileData, 0, mArcInfoBlock->num_file_entries * sizeof(void*));

    for (u32 i = 0; i < mArcInfoBlock->num_file_entries; i++) {
        mFiles[i].index = i;
    }
}

void JKRArchive::notifyOverlayFilesChanged() {
    sArcOverlayGeneration.fetch_add(1, std::memory_order_release);
}

bool JKRArchive::buildArcOverlaysPath() const {
    if (mArcOverlaysPathResolved) {
        return !mArcOverlaysPath.empty();
    }
    mArcOverlaysPathResolved = true;

    if (mEntryNum < 0) {
        return false;
    }

    char pathBuffer[1024];
    if (!DVDConvertEntrynumToPath(mEntryNum, pathBuffer, sizeof(pathBuffer))) {
        return false;
    }

    std::string path{pathBuffer};
    constexpr std::string_view extension{".arc"};
    if (path.size() < extension.size() ||
        path.compare(path.size() - extension.size(), extension.size(), extension) != 0)
    {
        return false;
    }
    path.resize(path.size() - extension.size());
    path.push_back('/');

    mArcOverlaysPath = std::move(path);
    return true;
}

void JKRArchive::buildIndexToPathMap(u32 dirIndex, const std::string& currentPath) const {
    const SDIDirEntry& dir = mNodes[dirIndex];
    for (int i = 0; i < dir.num_entries; i++) {
        const SDIFileEntry& entry = mFiles[dir.first_file_index + i];
        std::string entryName{&mStringTable[entry.getNameOffset()]};
        if (entryName == "." || entryName == "..") {
            continue;
        }
        if (entry.isDirectory()) {
            buildIndexToPathMap(entry.data_offset, currentPath + entryName + "/");
        } else {
            mIdxToPathMap[entry.index] = currentPath + entryName;
        }
    }
}

bool JKRArchive::getOverlayPath(SDIFileEntry* entry, std::string& path) const {
    if (entry == nullptr || !buildArcOverlaysPath()) {
        return false;
    }

    if (mIdxToPathMap.empty()) {
        buildIndexToPathMap(0, std::string{&mStringTable[mNodes[0].name_offset]} + "/");
    }

    const auto pathIt = mIdxToPathMap.find(entry->index);
    if (pathIt == mIdxToPathMap.end()) {
        return false;
    }

    path = mArcOverlaysPath + pathIt->second;
    return true;
}

void* JKRArchive::getActiveOverlayData(SDIFileEntry* entry, u32* outSize) const {
    const auto activeIt = mActiveArcOverlayResources.find(entry->index);
    if (activeIt == mActiveArcOverlayResources.end()) {
        return nullptr;
    }

    const auto resourceIt = mArcOverlayResources.find(activeIt->second);
    const u64 generation = sArcOverlayGeneration.load(std::memory_order_acquire);
    if (resourceIt == mArcOverlayResources.end() || resourceIt->second.generation != generation) {
        // Keep the allocation owned so raw pointers returned by earlier fetches remain valid.
        mActiveArcOverlayResources.erase(activeIt);
        return nullptr;
    }

    if (outSize != nullptr) {
        *outSize = resourceIt->second.size;
    }
    return resourceIt->first;
}

void* JKRArchive::getOverlayData(SDIFileEntry* entry, u32* outSize) {
    if (entry == nullptr) {
        return nullptr;
    }

    if (void* data = getActiveOverlayData(entry, outSize)) {
        return data;
    }

    std::string path;
    if (!getOverlayPath(entry, path)) {
        return nullptr;
    }

    constexpr u32 alignmentMask = 0x1f;
    const u64 generation = sArcOverlayGeneration.load(std::memory_order_acquire);
    DVDFileInfo fileInfo{};
    if (!DVDOpen(path.c_str(), &fileInfo)) {
        return nullptr;
    }

    const u32 logicalSize = fileInfo.length;
    if (logicalSize > static_cast<u32>(std::numeric_limits<s32>::max()) - alignmentMask) {
        DVDClose(&fileInfo);
        return nullptr;
    }

    const u32 readSize = ALIGN_NEXT(logicalSize, 0x20);
    const u32 allocationSize = readSize == 0 ? 1 : readSize;
    void* data = alloc_overlay_buffer(allocationSize);
    if (data == nullptr) {
        DVDClose(&fileInfo);
        return nullptr;
    }

    const s32 status = DVDReadPrio(&fileInfo, data, readSize, 0, 2);
    DVDClose(&fileInfo);
    if (status < DVD_RESULT_GOOD || static_cast<u32>(status) != logicalSize) {
        free_overlay_buffer(data);
        return nullptr;
    }

    mArcOverlayResources.emplace(data, ArcOverlayResource{
                                           .entryIndex = entry->index,
                                           .size = logicalSize,
                                           .generation = generation,
                                       });
    mActiveArcOverlayResources[entry->index] = data;

    if (outSize != nullptr) {
        *outSize = logicalSize;
    }
    return data;
}

bool JKRArchive::copyOverlayData(void* buffer, u32 bufferSize, SDIFileEntry* entry, u32* outSize) {
    u32 overlaySize;
    const void* overlayData = getOverlayData(entry, &overlaySize);
    if (overlayData == nullptr) {
        return false;
    }

    if (overlaySize > bufferSize) {
        std::string path;
        getOverlayPath(entry, path);
        Log.error("Overlay %s is %u bytes but the game reserved %u\n", path.c_str(), overlaySize,
            bufferSize);
        return false;
    }
    if (overlaySize != 0) {
        memcpy(buffer, overlayData, overlaySize);
    }
    if (outSize != nullptr) {
        *outSize = overlaySize;
    }
    return true;
}

bool JKRArchive::getOverlayResourceSize(const void* data, u32* outSize) const {
    const auto resourceIt = mArcOverlayResources.find(const_cast<void*>(data));
    if (resourceIt == mArcOverlayResources.end()) {
        return false;
    }

    if (outSize != nullptr) {
        *outSize = resourceIt->second.size;
    }
    return true;
}

bool JKRArchive::getOverlayFileSize(SDIFileEntry* entry, u32* outSize) const {
    if (entry == nullptr) {
        return false;
    }

    u32 activeSize;
    if (getActiveOverlayData(entry, &activeSize) != nullptr) {
        if (outSize != nullptr) {
            *outSize = activeSize;
        }
        return true;
    }

    std::string path;
    if (!getOverlayPath(entry, path)) {
        return false;
    }

    DVDFileInfo fileInfo{};
    if (!DVDOpen(path.c_str(), &fileInfo)) {
        return false;
    }

    if (outSize != nullptr) {
        *outSize = fileInfo.length;
    }
    DVDClose(&fileInfo);
    return true;
}

u32 JKRArchive::getFileSize(SDIFileEntry* entry) const {
    u32 size;
    if (getOverlayFileSize(entry, &size)) {
        return size;
    }
    return entry != nullptr ? entry->getSize() : 0;
}

bool JKRArchive::removeOverlayResource(void* resource, bool freeResource) {
    const auto resourceIt = mArcOverlayResources.find(resource);
    if (resourceIt == mArcOverlayResources.end()) {
        return false;
    }

    const auto activeIt = mActiveArcOverlayResources.find(resourceIt->second.entryIndex);
    if (activeIt != mActiveArcOverlayResources.end() && activeIt->second == resource) {
        mActiveArcOverlayResources.erase(activeIt);
    }

    if (freeResource) {
        free_overlay_buffer(resource);
    }
    mArcOverlayResources.erase(resourceIt);
    return true;
}

void JKRArchive::removeAllOverlayResources() {
    mActiveArcOverlayResources.clear();
    for (const auto& key : mArcOverlayResources | std::views::keys) {
        free_overlay_buffer(key);
    }
    mArcOverlayResources.clear();
}

#endif
