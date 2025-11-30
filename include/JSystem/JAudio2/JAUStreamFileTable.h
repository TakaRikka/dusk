#ifndef JAUSTREAMFILETABLE_H
#define JAUSTREAMFILETABLE_H

#include "JSystem/JAudio2/JAIStreamDataMgr.h"

struct BinaryStreamFileTable {
    /* 0x0 */ char mIdentifier[4];
    /* 0x4 */ int mNumFiles;
    /* 0x8 */ int mFilePathOffsets[];
};

/**
 * @ingroup jsystem-jaudio
 * 
 */
struct JAUStreamFileTable {
    JAUStreamFileTable();
    void init(void const*);
    int getNumFiles() const;
    const char* getFilePath(int) const;

    bool isValid() { return mData; }

    /* 0x0 */ const BinaryStreamFileTable* mData;
};

/**
 * @ingroup jsystem-jaudio
 * 
 */
struct JAUStreamDataMgr_StreamFileTable : public JAIStreamDataMgr, JAUStreamFileTable {
    virtual s32 getStreamFileEntry(JAISoundID);
    virtual ~JAUStreamDataMgr_StreamFileTable() {}
};

#endif /* JAUSTREAMFILETABLE_H */
