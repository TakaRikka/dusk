#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JAudio2/JAUStreamFileTable.h"

JAUStreamFileTable::JAUStreamFileTable() {
    mData = NULL;
}

void JAUStreamFileTable::init(void const* data) {
    if (data == NULL) {
        mData = NULL;
        return;
    }
    const BinaryStreamFileTable* binaryTable = (const BinaryStreamFileTable*)data;
    const char* ident = binaryTable->mIdentifier;
    if (ident[0] == 'b' && ident[1] == 's' && ident[2] == 'f' && ident[3] == 't') {
        mData = binaryTable;
    }
}

int JAUStreamFileTable::getNumFiles() const {
    return mData->mNumFiles;
}

const char* JAUStreamFileTable::getFilePath(int index) const {
    if (mData == NULL) {
        return 0;
    }
    ASSERT(index >= 0);
    ASSERT(index < getNumFiles());
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
