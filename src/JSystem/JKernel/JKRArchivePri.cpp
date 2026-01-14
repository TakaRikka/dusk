#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JKernel/JKRArchive.h"
#include "JSystem/JKernel/JKRHeap.h"
#include <cctype>
#include <string>

u32 JKRArchive::sCurrentDirID;

JKRArchive::JKRArchive() {
    mIsMounted = false;
    mMountDirection = MOUNT_DIRECTION_HEAD;
}

JKRArchive::JKRArchive(s32 entryNumber, JKRArchive::EMountMode mountMode) {
    mIsMounted = false;
    mMountMode = mountMode;
    mMountCount = 1;
    field_0x58 = 1;

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

JKRArchive::~JKRArchive() {}

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
        if (fileEntry->data == resource) {
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
