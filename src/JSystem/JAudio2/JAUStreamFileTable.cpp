#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JAudio2/JAUStreamFileTable.h"

JAUStreamFileTable::JAUStreamFileTable() {
    mData = NULL;
}

void JAUStreamFileTable::init(void const* data) {
    const BinaryStreamFileTable* binaryTable = (const BinaryStreamFileTable*)data;
    if (binaryTable == NULL) {
        mData = NULL;
        return;
    }
    if (binaryTable->mIdentifier[0] == 'b' && binaryTable->mIdentifier[1] == 's' && binaryTable->mIdentifier[2] == 'f' && binaryTable->mIdentifier[3] == 't') {
        mData = binaryTable;
    }
}

u32 JAUStreamFileTable::getNumFiles() const {
    return mData->mNumFiles;
}

const char* JAUStreamFileTable::getFilePath(int index) const {
    if (mData == NULL) {
        return 0;
    }
    JUT_ASSERT(52, index >= 0);
    JUT_ASSERT(53, index < getNumFiles());
    char* data = (char*)mData;
    return (char*)(data + *(int*)(data + 8 + (index * sizeof(s32))));
}

s32 JAUStreamDataMgr_StreamFileTable::getStreamFileEntry(JAISoundID soundId) {
    const char* filePath = getFilePath(soundId.id_.info.waveID);
    if (filePath == NULL) {
        return -1;
    }
    return DVDConvertPathToEntrynum(filePath);
}
