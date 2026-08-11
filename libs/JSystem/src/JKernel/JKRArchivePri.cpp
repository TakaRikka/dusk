#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JKernel/JKRArchive.h"
#include "JSystem/JKernel/JKRHeap.h"
#include <cctype>
#include <cstring>

#if TARGET_PC
#include <cassert>
#include "JSystem/JKernel/JKRDvdRipper.h"
#include "dusk/logging.h"
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

void JKRArchive::buildArcOverlaysPath() {
    if (!mArcOverlaysPath.empty() || mEntryNum < 0) {
        return;
    }

    char buf[128];
    if (!DVDConvertEntrynumToPath(mEntryNum, buf, 100)) {
        DuskLog.warn("Attempted to convert DVD entry number {} to path failed! Is it too long?", mEntryNum);
        return;
    }

    std::string path = std::string(buf);
    size_t pos = path.rfind(".arc");

    if (pos == std::string::npos) {
        return;
    }
    path.erase(pos); // Remove .arc from the end of the path
    path.push_back('/');

    mArcOverlaysPath = std::move(path);
}

void JKRArchive::buildIdToPathMap(u32 dirIndex, const std::string& currentPath) {
    const SDIDirEntry& dir = mNodes[dirIndex];
    for (int i = 0; i < dir.num_entries; i++) {
        const SDIFileEntry& entry = mFiles[dir.first_file_index + i];
        std::string entryName = std::string(&mStringTable[entry.getNameOffset()]);
        if (entryName == "." || entryName == "..") {
            continue;
        }
        if (entry.isDirectory()) {
            buildIdToPathMap(entry.data_offset, currentPath + entryName + "/");
        }else {
            mIdToPathMap[entry.file_id] = currentPath + entryName;
        }
    }
}

void* JKRArchive::getOverlayData(JKRArchive::SDIFileEntry* fileEntry, u32* out_size) {
    if (mArcOverlaysPath.empty()) {
        buildArcOverlaysPath();
    }

    // First, check if any overlays are applied on this directory
    if (DVDConvertPathToEntrynum(mArcOverlaysPath.c_str()) < 0) {
        return nullptr;
    }

    // Build a map of file Ids -> Local path names
    if (mIdToPathMap.empty()) {
        buildIdToPathMap(0, std::string(&mStringTable[mNodes[0].name_offset])+"/");
    }

    const auto& it = mFileIdToArcOverlayData.find(fileEntry->file_id);
    if (it != mFileIdToArcOverlayData.end()) {
        if (out_size) {
            *out_size = it->second.size();
        }
        return (void*)it->second.data();
    }

    const auto& it2 = mIdToPathMap.find(fileEntry->file_id);
    if (it2 == mIdToPathMap.end()) {
        return nullptr;
    }
    
    DVDFileInfo fileInfo;
    if (!DVDOpen(std::string(mArcOverlaysPath+it2->second).c_str(),&fileInfo)) {
        return nullptr;
    }

    if (out_size) {
        *out_size = fileInfo.length;
    }

    std::vector<u8> buffer(ALIGN_NEXT(fileInfo.length,0x20));
    s32 status = DVDReadPrio(&fileInfo, buffer.data(), ALIGN_NEXT(fileInfo.length,0x20), 0, 2);
    DVDClose(&fileInfo);

    if (status < DVD_RESULT_GOOD) {
        return nullptr;
    }    

    void* out_data = buffer.data();
    mFileIdToArcOverlayData[fileEntry->file_id] = std::move(buffer);
    return out_data;
}

void* JKRArchive::getOverlayCopyData(void* buffer, u32 bufferSize, SDIFileEntry* entry, u32* out_size) {
    u32 overlaySize;
    void* overlay_data = getOverlayData(entry, &overlaySize);
    if (overlay_data) {
        if (overlaySize <= bufferSize) {
            if (out_size) {
            *out_size = overlaySize;
            }
            memcpy(buffer, overlay_data, overlaySize);
            return buffer;
        }else{
            DuskLog.error("Attempted to load overlay for {}{}, but its size of {} was greater than the allocated buffer's size of {}",mArcOverlaysPath,mIdToPathMap[entry->file_id],overlaySize,bufferSize);
        }
    }
    return nullptr;
}

#endif
